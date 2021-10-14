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

#include "qemu/osdep.h"
#include "qemu/units.h"
#include "qemu/cutils.h"
#include "qapi/error.h"
#include "qemu/error-report.h"
#include "cpu.h"
#include "hw/boards.h"
#include "hw/loader.h"
#include "hw/arm/boot.h"
#include "cheri_tagmem.h"
#include "sysemu/sysemu.h"
#include "hw/arm/armtrickbox.h"

/* A basic morello board. There is no need to use this if only a morello CPU
 * is required (use the virt board instead). This board is designed to be a
 * minimal platform for testing, with optional support for the ARM ACK.
 */

#define MORELLO_MAX_CPUS 4

typedef struct MorelloMachineState {
    /*< private >*/
    MachineState parent_obj;
    /*< public >*/
    struct {
        ARMCPU core;
    } cpus[MORELLO_MAX_CPUS];
    bool is_ack;
} MorelloMachineState;

typedef struct MorelloMachineClass {
    /*< private >*/
    MachineClass parent_obj;
    /*< public >*/
} MorelloMachineClass;

#define TYPE_MORELLO_MACHINE MACHINE_TYPE_NAME("morello")
#define MORELLO_MACHINE(obj)                                                   \
    OBJECT_CHECK(MorelloMachineState, (obj), TYPE_MORELLO_MACHINE)

#define MORELLO_MACHINE_CLASS(klass)                                           \
    OBJECT_CLASS_CHECK(MorelloMachineClass, (klass), TYPE_MORELLO_MACHINE)
#define MORELLO_MACHINE_GET_CLASS(obj)                                         \
    OBJECT_GET_CLASS(MorelloMachineClass, (obj), TYPE_MORELLO_MACHINE)

static void error_and_die(const char *message)
{
    error_report("%s\n", message);
    assert(0);
    exit(1);
}

/* Lifetime is for the lifetime of the program */
static struct arm_boot_info binfo;

/* ACK test ELSs do not define an entry point. Instead, look for this symbol and
 * jump there. */
uint64_t RVBAR_exec;
static void find_rvbar_exec(const char *st_name, int st_info, uint64_t st_value,
                            uint64_t st_size)
{
    if (strcmp(st_name, "__RVBAR_ENTRY") == 0) {
        RVBAR_exec = st_value;
        printf("%lx\n", RVBAR_exec);
    }
}

struct mem_region {
    uint64_t start, end;
    bool alias_sec;
    const char *name;
    const char *name_sec;
};

static MemoryRegion *create_mem_from_desc(MemoryRegion *parent,
                                          struct mem_region *el, bool sec,
                                          MemoryRegion *alias)
{
    MemoryRegion *mem = g_new(MemoryRegion, 1);

    if (alias) {
        memory_region_init_alias(mem, NULL, sec ? el->name_sec : el->name,
                                 alias, 0, el->end - el->start);
    } else {
        memory_region_init_ram(mem, NULL, sec ? el->name_sec : el->name,
                               el->end - el->start, &error_fatal);
#ifdef TARGET_CHERI
        cheri_tag_init(mem, el->end - el->start);
#endif
    }

    memory_region_add_subregion(parent, el->start, mem);

    return mem;
}

static void morello_machine_init(MachineState *machine)
{
    MorelloMachineState *morelloMachineState = MORELLO_MACHINE(machine);
    Error *errp = NULL;

#ifndef TARGET_CHERI
#error "Morello board should be used with TARGET_CHERI"
#endif

    /* Set to true to get separate physical address spaces for regions specified
     * below with secure aliases with non secure = false. */
    bool seperate_s_ns = false;

    /* This map was decided on just by running ACK. It would be good to have a
     * better idea of what the actual memory map is. It also suffices for the
     * other test suites we have so we don't change it based on ACK support.
     */
#define NAME(N) N, N "_sec"
    struct mem_region mem_map[] = {
        /* {start, end, secure aliases with non secure, name} */
        { 0x0, 0x13000000, true,
          NAME("LOW") }, /* The trickbox goes between LOW and HIGH */
        { 0x13200000, 0x100000000, true, NAME("HIGH") },
        { 0x800000000000, 0x800010000000, true, NAME("HIGHEST") },
        { 0xfffffffe0000, 0x1000000000000, true, NAME("mem.NORMAL_MAX_PA") },
    };

    MemoryRegion *secure_memory = NULL;
    if (seperate_s_ns) {
        secure_memory = g_new(MemoryRegion, 1);
        memory_region_init(secure_memory, NULL, "cpu-secure-memory",
                           UINT64_MAX);
    }

    /* Add RAM */
    for (struct mem_region *el = mem_map;
         el != (struct mem_region *)((char *)mem_map + sizeof(mem_map)); el++) {
        MemoryRegion *ns =
            create_mem_from_desc(get_system_memory(), el, false, NULL);
        if (seperate_s_ns) {
            create_mem_from_desc(secure_memory, el, true,
                                 el->alias_sec ? ns : NULL);
        }
    }

    if (strcmp(machine->cpu_type, ARM_CPU_TYPE_NAME("morello")) != 0) {
        error_and_die(
            "Use the special 'morello' cpu type for the Morello board.");
    }

    /* Create CPUS */

    for (size_t i = 0; i != machine->smp.cpus; i++) {
        object_initialize_child(OBJECT(machine), "cpu[*]",
                                &morelloMachineState->cpus[i].core,
                                machine->cpu_type);
        object_property_set_bool(OBJECT(&morelloMachineState->cpus[i].core),
                                 "mpidr_mt", true, NULL);
        morelloMachineState->cpus[i].core.secure_memory = secure_memory;
        if (!qdev_realize(DEVICE(&morelloMachineState->cpus[i].core), NULL,
                          &errp)) {
            error_report_err(errp);
            assert(0);
        }
    }

    /* Create the trickbox used for testing */
    SysBusDevice *trickbox = arm_trickbox_mm_init_default();

    if (seperate_s_ns) {
        /* Put the trickbox in the secure space as well  */
        trickbox->mmio[1].addr = trickbox->mmio[0].addr;
        memory_region_add_subregion(secure_memory, trickbox->mmio[1].addr,
                                    trickbox->mmio[1].memory);
    }

    /* Start logging */
#ifdef CONFIG_TCG_LOG_INSTR
    qemu_log_instr_init(CPU(ARM_CPU(first_cpu)));
#endif

    /* The real memory map is specified above. This is just to make
     * arm_load_kernel happy,
     * as it will assert if ram_size is not large enough to hold the ELF */

    binfo.ram_size = 0x13000000;
    binfo.nb_cpus = machine->smp.cpus;
    binfo.secure_boot = seperate_s_ns;

    if (morelloMachineState->is_ack) {
        binfo.sym_cb = &find_rvbar_exec;
    }

    arm_load_kernel(ARM_CPU(first_cpu), machine, &binfo);

    if (morelloMachineState->is_ack) {
        assert(RVBAR_exec != 0);
        binfo.entry = RVBAR_exec;
    }
}

static bool morello_get_ack(Object *obj, Error **errp)
{
    MorelloMachineState *morelloMachineState = MORELLO_MACHINE(obj);
    return morelloMachineState->is_ack;
}

static void morello_set_ack(Object *obj, bool value, Error **errp)
{
    MorelloMachineState *morelloMachineState = MORELLO_MACHINE(obj);
    morelloMachineState->is_ack = value;
}

static void morello_instance_init(Object *obj)
{
    MorelloMachineState *morelloMachineState = MORELLO_MACHINE(obj);
    morelloMachineState->is_ack = false;
    object_property_add_bool(obj, "ack", morello_get_ack, morello_set_ack);
    object_property_set_description(
        obj, "ack", "Set on/off to enable/disable support for the ACK");
}

static void morello_machine_class_init(ObjectClass *oc, void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);

    mc->desc = "Morello Machine";
    mc->init = morello_machine_init;
    mc->default_cpus = mc->min_cpus = 1;
    mc->max_cpus = MORELLO_MAX_CPUS;
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("morello");
    mc->minimum_page_bits = 12;
}

static const TypeInfo morello_machine_types[] = { {
    .name = TYPE_MORELLO_MACHINE,
    .parent = TYPE_MACHINE,
    .class_init = morello_machine_class_init,
    .instance_init = morello_instance_init,
    .instance_size = sizeof(MorelloMachineState),
    .class_size = sizeof(MorelloMachineClass),
} };

DEFINE_TYPES(morello_machine_types)
