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

#define RAM_SIZE 0x13000000

#define MORELLO_MAX_CPUS 4

typedef struct MorelloMachineState {
    /*< private >*/
    MachineState parent_obj;
    /*< public >*/
    struct {
        ARMCPU core;
    } cpus[MORELLO_MAX_CPUS];
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

// Lifetime is for the lifetime of the program
static struct arm_boot_info binfo;

static void morello_machine_init(MachineState *machine)
{
    MorelloMachineState *morelloMachineState = MORELLO_MACHINE(machine);
    Error *errp = NULL;

#ifndef TARGET_CHERI
#error "Morello board should be used with TARGET_CHERI"
#endif

    /* Add RAM */
    memory_region_add_subregion(get_system_memory(), 0, machine->ram);

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
        if (!qdev_realize(DEVICE(&morelloMachineState->cpus[i].core), NULL,
                          &errp)) {
            error_report_err(errp);
            assert(0);
        }
    }

    /* Create the trickbox used for testing */
    arm_trickbox_mm_init_default();

    /* Start logging */
#ifdef CONFIG_TCG_LOG_INSTR
    qemu_log_instr_init(CPU(ARM_CPU(first_cpu)));
#endif

    binfo.ram_size = ram_size;
    binfo.nb_cpus = machine->smp.cpus;
    arm_load_kernel(ARM_CPU(first_cpu), machine, &binfo);

    assert(binfo.entry < ram_size);
}

static void morello_machine_class_init(ObjectClass *oc, void *data)
{
    MachineClass *mc = MACHINE_CLASS(oc);

    mc->desc = "Morello Machine";
    mc->init = morello_machine_init;
    mc->default_cpus = mc->min_cpus = 1;
    mc->max_cpus = MORELLO_MAX_CPUS;
    mc->default_ram_size = RAM_SIZE;
    mc->default_ram_id = "ram";
    mc->default_cpu_type = ARM_CPU_TYPE_NAME("morello");
    mc->minimum_page_bits = 12;
}

static const TypeInfo morello_machine_types[] = {{
    .name = TYPE_MORELLO_MACHINE,
    .parent = TYPE_MACHINE,
    .class_init = morello_machine_class_init,
    .instance_size = sizeof(MorelloMachineState),
    .class_size = sizeof(MorelloMachineClass),
}};

DEFINE_TYPES(morello_machine_types)
