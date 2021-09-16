/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2020 Lawrence Esswood
 *
 * This work was supported by Innovate UK project 105694, "Digital Security
 * by Design (DSbD) Technology Platform Prototype".
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

// The 'trickbox' device used by the test suite for Morello.
// This is not an official implementation of any arm technology.

// Also includes some of ETMv4

#include "qemu/osdep.h"
#include "hw/sysbus.h"
#include "hw/arm/armtrickbox.h"
#include "qemu/module.h"
#include "qapi/error.h"
#include "hw/qdev-properties.h"
#include "qemu/qemu-print.h"
#include "map.h"

#define BUF_SIZE 100


// List of registers to handle just by getting/setting a field in arm_trickbox_state
// ITEM(name, reset_value, read_mask, write_mask, write_helper, ... aliases)
#define REGISTER_LIST(ITEM)                                         \
    ITEM(Configuration, 0x00004002, 0xFFFFFFFE, 0xFFFFFFFF, NULL)   \
    ITEM(TargetCPU, 0xF, 0xF, 0xF, NULL)                            \
    ITEM(abortrgnlo1, 0x40000000, 0xFFFFFFFF, 0xFFFFFFFF, axi_abort_range_changed, AbortRgn64Lo1) \
    ITEM(abortrgnhi1, 0x50000000, 0xFFFFFFFF, 0xFFFFFFFF, axi_abort_range_changed, AbortRgn64Hi1) \
    ITEM(abortrgnlo2, 0x3F000000, 0xFFFFFFFF, 0xFFFFFFFF, axi_abort_range_changed, AbortRgn64Lo2) \
    ITEM(abortrgnhi2, 0x3F080000, 0xFFFFFFFF, 0xFFFFFFFF, axi_abort_range_changed, AbortRgn64Hi2) \
    ITEM(AbortRgn64Lo1_Hi, 0x0, 0xFFFFFFFF, 0xFFFFFFFF, axi_abort_range_changed) \
    ITEM(AbortRgn64Hi1_Hi, 0x0, 0xFFFFFFFF, 0xFFFFFFFF, axi_abort_range_changed) \
    ITEM(AbortRgn64Lo2_Hi, 0x0, 0xFFFFFFFF, 0xFFFFFFFF, axi_abort_range_changed) \
    ITEM(AbortRgn64Hi2_Hi, 0x0, 0xFFFFFFFF, 0xFFFFFFFF, axi_abort_range_changed) \
    ITEM(AXIAbortCtl, 0x0, 0xFFFFFFFF, 0xFFFFFFFF, axi_abort_changed) \
    ITEM(DBGControl, 0x00000181, 0xFFFFFFFF, 0xC1, NULL)

typedef struct {
    SysBusDevice parent_obj;
    MemoryRegion iomem;
    MemoryRegion iomem_secure;
    // Tube buffer for more readable output
    char buf[BUF_SIZE + 1];
    size_t ndx;
    // Registers that are simply backed
#define DECLARE_BACKING(name, ...) uint32_t name;
    REGISTER_LIST(DECLARE_BACKING)
} arm_trickbox_state;

#define TYPE_ARM_TRICKBOX "arm-trickbox"
#define ARM_TRICKBOX(obj)                                                      \
    OBJECT_CHECK(arm_trickbox_state, (obj), TYPE_ARM_TRICKBOX)

typedef void write_helper_fn(arm_trickbox_state *tb, hwaddr addr, uint32_t *v,
                             uint32_t old);

static void axi_abort_changed(arm_trickbox_state *tb, hwaddr addr, uint32_t *v,
                              uint32_t old)
{
    if (~*v & (ARM_TRICKBOX_AXIAbortCtl_Rgn1_Rd_Disable |
               ARM_TRICKBOX_AXIAbortCtl_Rgn1_Wr_Disable |
               ARM_TRICKBOX_AXIAbortCtl_Rgn2_Rd_Disable |
               ARM_TRICKBOX_AXIAbortCtl_Rgn2_Wr_Disable)) {
        printf("TRICKBOX: WARN: Some AXI aborts are enabled but this is not "
               "yet supported: %x\n",
               *v);
    } else {
        printf("TRICKBOX: INFO: AXI aborts disabled\n");
    }
}

static void axi_abort_range_changed(arm_trickbox_state *tb, hwaddr addr,
                                    uint32_t *v, uint32_t old)
{
    // Writes to the 32-bit registers zero extend
    if (addr == ARM_TRICKBOX_REGISTER_abortrgnlo1)
        tb->AbortRgn64Lo1_Hi = 0;
    else if (addr == ARM_TRICKBOX_REGISTER_abortrgnhi1)
        tb->AbortRgn64Hi1_Hi = 0;
    else if (addr == ARM_TRICKBOX_REGISTER_abortrgnlo2)
        tb->AbortRgn64Lo2_Hi = 0;
    else if (addr == ARM_TRICKBOX_REGISTER_abortrgnhi2)
        tb->AbortRgn64Hi2_Hi = 0;

    axi_abort_changed(tb, ARM_TRICKBOX_REGISTER_AXIAbortCtl, &tb->AXIAbortCtl,
                      tb->AXIAbortCtl);
}

static void empty_buffer(arm_trickbox_state *tb)
{
    tb->buf[tb->ndx] = '\0';
    qemu_printf("%s", tb->buf);
    tb->ndx = 0;
}

const char *const exit_string = NULL; // "FAIL";
const char *progress = exit_string;
static void exit_on_string(char c)
{
    if (exit_string) {
        if (*progress != c)
            progress = exit_string;
        if (*progress == c)
            progress++;
        if (!*progress)
            assert(0);
    }
}
static void tube_write(arm_trickbox_state *tb, hwaddr addr, uint64_t val,
                       unsigned size)
{
    char c = (val & 0xFF);

    exit_on_string(c);

    switch (c) {
    case ARM_TRICKBOX_TUBE_EXIT:
        if (tb->ndx != 0)
            empty_buffer(tb);
        exit(0);
    default:
        tb->buf[tb->ndx++] = c;
    }

    if (tb->ndx == BUF_SIZE || (c == '\n'))
        empty_buffer(tb);
}

static void arm_trickbox_write(void *opaque, hwaddr addr, uint64_t val,
                               unsigned size)
{
    arm_trickbox_state *tb = ARM_TRICKBOX(opaque);

    // Most registers are aligned to 4 bytes
    hwaddr register_addr = (addr) & ~(4 - 1);

    // printf("T-write %lx\n", register_addr);

    uint32_t old;

    switch (register_addr) {

#define CASE_HANDLE(name) case ARM_TRICKBOX_REGISTER_##name:
#define HANDLE_WRITE(name, resetv, readmask, writemask, writefn, ...)          \
    MAP(CASE_HANDLE, name, ##__VA_ARGS__)                                      \
    old = tb->name;                                                            \
    tb->name = (tb->name & ~writemask) | (val & writemask);                    \
    if (writefn) {                                                             \
        ((write_helper_fn *)writefn)(tb, addr, &tb->name, old);                \
    }                                                                          \
    break;
        REGISTER_LIST(HANDLE_WRITE)

    case ARM_TRICKBOX_REGISTER_Tube:
        tube_write(tb, addr, val, size);
        break;
    default:
        // TODO: Once most functionality is implemented, make this a log.
        printf("Write at %lx\n", register_addr);
        assert(0);
    }
}

static uint64_t arm_trickbox_read(void *opaque, hwaddr addr, unsigned size)
{
    arm_trickbox_state *tb = ARM_TRICKBOX(opaque);

    // Most registers are aligned to 4 bytes
    hwaddr register_addr = (addr) & ~(4 - 1);

    // printf("T-read %lx\n", register_addr);

    switch (register_addr) {

#define HANDLE_READ(name, resetv, readmask, writemask, writefn, ...)           \
    MAP(CASE_HANDLE, name, ##__VA_ARGS__)                                      \
    return tb->name & readmask;
        REGISTER_LIST(HANDLE_READ)

    case ARM_TRICKBOX_REGISTER_Tube:
        return 0;
    default:
        // TODO: Once most functionality is implemented, make this a log.
        assert(0);
    }
}

static void arm_trickbox_reset(DeviceState *dev)
{
    arm_trickbox_state *tb = ARM_TRICKBOX(dev);

#define RESET_REG(name, resetv, ...) tb->name = resetv;
    REGISTER_LIST(RESET_REG)

    axi_abort_changed(tb, ARM_TRICKBOX_REGISTER_AXIAbortCtl, &tb->AXIAbortCtl,
                      tb->AXIAbortCtl);
}

static const MemoryRegionOps arm_trickbox_ops = {
    .read = arm_trickbox_read,
    .write = arm_trickbox_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
};

static void arm_trickbox_instance_init(Object *obj)
{
    SysBusDevice *dev = SYS_BUS_DEVICE(obj);
    arm_trickbox_state *s = ARM_TRICKBOX(dev);
    memory_region_init_io(&s->iomem, obj, &arm_trickbox_ops, s, "trickbox",
                          ARM_TRICKBOX_SIZE);
    memory_region_init_io(&s->iomem_secure, obj, &arm_trickbox_ops, s,
                          "trickbox_sec", ARM_TRICKBOX_SIZE);
    sysbus_init_mmio(dev, &s->iomem);
    sysbus_init_mmio(dev, &s->iomem_secure);
}

static void arm_trickbox_realize(DeviceState *dev, Error **errp) {}

static Property arm_trickbox_properties[] = {
    DEFINE_PROP_END_OF_LIST(),
};

static void arm_trickbox_class_init(ObjectClass *oc, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(oc);

    dc->realize = arm_trickbox_realize;
    dc->reset = arm_trickbox_reset;
    device_class_set_props(dc, arm_trickbox_properties);
    set_bit(DEVICE_CATEGORY_INPUT, dc->categories);
}

static const TypeInfo arm_trickbox_info = {
    .name = TYPE_ARM_TRICKBOX,
    .parent = TYPE_SYS_BUS_DEVICE,
    .instance_size = sizeof(arm_trickbox_state),
    .instance_init = arm_trickbox_instance_init,
    .class_init = arm_trickbox_class_init,
};

static void arm_trickbox_register(void)
{
    type_register_static(&arm_trickbox_info);
}

type_init(arm_trickbox_register)

    static SysBusDevice *arm_trickbox_mm_init(hwaddr base)
{
    DeviceState *dev;
    dev = qdev_new(TYPE_ARM_TRICKBOX);
    sysbus_realize_and_unref(SYS_BUS_DEVICE(dev), &error_fatal);
    sysbus_mmio_map(SYS_BUS_DEVICE(dev), 0, base);
    return SYS_BUS_DEVICE(dev);
}

SysBusDevice *arm_trickbox_mm_init_default(void)
{
    return arm_trickbox_mm_init(ARM_TRICKBOX_ADDRESS);
}
