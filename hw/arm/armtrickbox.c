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

// The 'trickbox' device used by the test suite for Morello
// Is an infinitely deep memory mapped fifo that will kill QEMU if \004 is sent
// or otherwise print to the terminal There may be some other magic numbers this
// accepts, I have not really reverse engineered the test cases yet.

#include "hw/arm/armtrickbox.h"
#include "qemu/osdep.h"
#include "hw/sysbus.h"
#include "qemu/module.h"
#include "qapi/error.h"
#include "hw/qdev-properties.h"

#define BUF_SIZE 100

typedef struct {
    SysBusDevice parent_obj;
    MemoryRegion iomem;
    char buf[BUF_SIZE + 1];
    size_t ndx;
} arm_trickbox_state;

#define TYPE_ARM_TRICKBOX "arm-trickbox"
#define ARM_TRICKBOX(obj)                                                      \
    OBJECT_CHECK(arm_trickbox_state, (obj), TYPE_ARM_TRICKBOX)

static void empty_buffer(arm_trickbox_state *tb)
{
    tb->buf[tb->ndx] = '\0';
    // Again, is there a QEMU way of doing this?
    fprintf(stdout, "%s", tb->buf);
    tb->ndx = 0;
}

static void arm_trickbox_write(void *opaque, hwaddr addr, uint64_t val,
                               unsigned size)
{
    arm_trickbox_state *tb = ARM_TRICKBOX(opaque);

    assert(size == 1);
    char c = (val & 0xFF);

    switch (c) {
    case ARM_TRICKBOX_EXIT_C:
        if (tb->ndx != 0)
            empty_buffer(tb);
        exit(0);
    default:
        // Is there are QEMU specific way to do this?
        tb->buf[tb->ndx++] = c;
    }

    if (tb->ndx == BUF_SIZE || (c == '\n'))
        empty_buffer(tb);
}

static uint64_t arm_trickbox_read(void *opaque, hwaddr addr, unsigned size)
{
    return 0;
}

static void arm_trickbox_reset(DeviceState *dev) {}

static const MemoryRegionOps arm_trickbox_ops = {
    .read = arm_trickbox_read,
    .write = arm_trickbox_write,
    .endianness = DEVICE_NATIVE_ENDIAN,
};

static void arm_trickbox_instance_init(Object *obj)
{
    SysBusDevice *dev = SYS_BUS_DEVICE(obj);
    arm_trickbox_state *s = ARM_TRICKBOX(dev);
    memory_region_init_io(&s->iomem, obj, &arm_trickbox_ops, s, "trickbox", 1);
    sysbus_init_mmio(dev, &s->iomem);
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

    static void arm_trickbox_mm_init(hwaddr base)
{
    DeviceState *dev;
    dev = qdev_new(TYPE_ARM_TRICKBOX);
    sysbus_realize_and_unref(SYS_BUS_DEVICE(dev), &error_fatal);
    sysbus_mmio_map(SYS_BUS_DEVICE(dev), 0, base);
}

void arm_trickbox_mm_init_default(void)
{
    arm_trickbox_mm_init(ARM_TRICKBOX_ADDRESS);
}
