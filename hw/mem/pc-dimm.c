/*
 * Dimm device for Memory Hotplug
 *
 * Copyright ProfitBricks GmbH 2012
 * Copyright (C) 2014 Red Hat Inc
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>
 */

#include "qemu/osdep.h"
#include "hw/mem/pc-dimm.h"
#include "hw/mem/nvdimm.h"
#include "hw/mem/memory-device.h"
#include "qapi/error.h"
#include "qapi/visitor.h"
#include "sysemu/numa.h"
#include "trace.h"

typedef struct pc_dimms_capacity {
     uint64_t size;
     Error    **errp;
} pc_dimms_capacity;

void pc_dimm_memory_plug(DeviceState *dev, MachineState *machine,
                         uint64_t align, Error **errp)
{
    int slot;
    PCDIMMDevice *dimm = PC_DIMM(dev);
    PCDIMMDeviceClass *ddc = PC_DIMM_GET_CLASS(dimm);
    MemoryRegion *vmstate_mr = ddc->get_vmstate_memory_region(dimm);
    Error *local_err = NULL;
    MemoryRegion *mr;
    uint64_t addr;

    mr = ddc->get_memory_region(dimm, &local_err);
    if (local_err) {
        goto out;
    }

    addr = object_property_get_uint(OBJECT(dimm),
                                    PC_DIMM_ADDR_PROP, &local_err);
    if (local_err) {
        goto out;
    }

    addr = memory_device_get_free_addr(machine, !addr ? NULL : &addr, align,
                                       memory_region_size(mr), &local_err);
    if (local_err) {
        goto out;
    }

    object_property_set_uint(OBJECT(dev), addr, PC_DIMM_ADDR_PROP, &local_err);
    if (local_err) {
        goto out;
    }
    trace_mhp_pc_dimm_assigned_address(addr);

    slot = object_property_get_int(OBJECT(dev), PC_DIMM_SLOT_PROP, &local_err);
    if (local_err) {
        goto out;
    }

    slot = pc_dimm_get_free_slot(slot == PC_DIMM_UNASSIGNED_SLOT ? NULL : &slot,
                                 machine->ram_slots, &local_err);
    if (local_err) {
        goto out;
    }
    object_property_set_int(OBJECT(dev), slot, PC_DIMM_SLOT_PROP, &local_err);
    if (local_err) {
        goto out;
    }
    trace_mhp_pc_dimm_assigned_slot(slot);

    memory_device_plug_region(machine, mr, addr);
    vmstate_register_ram(vmstate_mr, dev);

out:
    error_propagate(errp, local_err);
}

void pc_dimm_memory_unplug(DeviceState *dev, MachineState *machine)
{
    PCDIMMDevice *dimm = PC_DIMM(dev);
    PCDIMMDeviceClass *ddc = PC_DIMM_GET_CLASS(dimm);
    MemoryRegion *vmstate_mr = ddc->get_vmstate_memory_region(dimm);
    MemoryRegion *mr = ddc->get_memory_region(dimm, &error_abort);

    memory_device_unplug_region(machine, mr);
    vmstate_unregister_ram(vmstate_mr, dev);
}

static int pc_dimm_slot2bitmap(Object *obj, void *opaque)
{
    unsigned long *bitmap = opaque;

    if (object_dynamic_cast(obj, TYPE_PC_DIMM)) {
        DeviceState *dev = DEVICE(obj);
        if (dev->realized) { /* count only realized DIMMs */
            PCDIMMDevice *d = PC_DIMM(obj);
            set_bit(d->slot, bitmap);
        }
    }

    object_child_foreach(obj, pc_dimm_slot2bitmap, opaque);
    return 0;
}

int pc_dimm_get_free_slot(const int *hint, int max_slots, Error **errp)
{
    unsigned long *bitmap;
    int slot = 0;

    if (max_slots <= 0) {
        error_setg(errp, "no slots where allocated, please specify "
                   "the 'slots' option");
        return slot;
    }

    bitmap = bitmap_new(max_slots);
    object_child_foreach(qdev_get_machine(), pc_dimm_slot2bitmap, bitmap);

    /* check if requested slot is not occupied */
    if (hint) {
        if (*hint >= max_slots) {
            error_setg(errp, "invalid slot# %d, should be less than %d",
                       *hint, max_slots);
        } else if (!test_bit(*hint, bitmap)) {
            slot = *hint;
        } else {
            error_setg(errp, "slot %d is busy", *hint);
        }
        goto out;
    }

    /* search for free slot */
    slot = find_first_zero_bit(bitmap, max_slots);
    if (slot == max_slots) {
        error_setg(errp, "no free slots available");
    }
out:
    g_free(bitmap);
    return slot;
}

static Property pc_dimm_properties[] = {
    DEFINE_PROP_UINT64(PC_DIMM_ADDR_PROP, PCDIMMDevice, addr, 0),
    DEFINE_PROP_UINT32(PC_DIMM_NODE_PROP, PCDIMMDevice, node, 0),
    DEFINE_PROP_INT32(PC_DIMM_SLOT_PROP, PCDIMMDevice, slot,
                      PC_DIMM_UNASSIGNED_SLOT),
    DEFINE_PROP_LINK(PC_DIMM_MEMDEV_PROP, PCDIMMDevice, hostmem,
                     TYPE_MEMORY_BACKEND, HostMemoryBackend *),
    DEFINE_PROP_END_OF_LIST(),
};

static void pc_dimm_get_size(Object *obj, Visitor *v, const char *name,
                             void *opaque, Error **errp)
{
    uint64_t value;
    MemoryRegion *mr;
    PCDIMMDevice *dimm = PC_DIMM(obj);
    PCDIMMDeviceClass *ddc = PC_DIMM_GET_CLASS(obj);

    mr = ddc->get_memory_region(dimm, errp);
    if (!mr) {
        return;
    }
    value = memory_region_size(mr);

    visit_type_uint64(v, name, &value, errp);
}

static void pc_dimm_init(Object *obj)
{
    object_property_add(obj, PC_DIMM_SIZE_PROP, "uint64", pc_dimm_get_size,
                        NULL, NULL, NULL, &error_abort);
}

static void pc_dimm_realize(DeviceState *dev, Error **errp)
{
    PCDIMMDevice *dimm = PC_DIMM(dev);
    PCDIMMDeviceClass *ddc = PC_DIMM_GET_CLASS(dimm);

    if (!dimm->hostmem) {
        error_setg(errp, "'" PC_DIMM_MEMDEV_PROP "' property is not set");
        return;
    } else if (host_memory_backend_is_mapped(dimm->hostmem)) {
        char *path = object_get_canonical_path_component(OBJECT(dimm->hostmem));
        error_setg(errp, "can't use already busy memdev: %s", path);
        g_free(path);
        return;
    }
    if (((nb_numa_nodes > 0) && (dimm->node >= nb_numa_nodes)) ||
        (!nb_numa_nodes && dimm->node)) {
        error_setg(errp, "'DIMM property " PC_DIMM_NODE_PROP " has value %"
                   PRIu32 "' which exceeds the number of numa nodes: %d",
                   dimm->node, nb_numa_nodes ? nb_numa_nodes : 1);
        return;
    }

    if (ddc->realize) {
        ddc->realize(dimm, errp);
    }

    host_memory_backend_set_mapped(dimm->hostmem, true);
}

static void pc_dimm_unrealize(DeviceState *dev, Error **errp)
{
    PCDIMMDevice *dimm = PC_DIMM(dev);

    host_memory_backend_set_mapped(dimm->hostmem, false);
}

static MemoryRegion *pc_dimm_get_memory_region(PCDIMMDevice *dimm, Error **errp)
{
    if (!dimm->hostmem) {
        error_setg(errp, "'" PC_DIMM_MEMDEV_PROP "' property must be set");
        return NULL;
    }

    return host_memory_backend_get_memory(dimm->hostmem, errp);
}

static MemoryRegion *pc_dimm_get_vmstate_memory_region(PCDIMMDevice *dimm)
{
    return host_memory_backend_get_memory(dimm->hostmem, &error_abort);
}

static uint64_t pc_dimm_md_get_addr(const MemoryDeviceState *md)
{
    const PCDIMMDevice *dimm = PC_DIMM(md);

    return dimm->addr;
}

static uint64_t pc_dimm_md_get_region_size(const MemoryDeviceState *md)
{
    /* dropping const here is fine as we don't touch the memory region */
    PCDIMMDevice *dimm = PC_DIMM(md);
    const PCDIMMDeviceClass *ddc = PC_DIMM_GET_CLASS(md);
    MemoryRegion *mr;

    mr = ddc->get_memory_region(dimm, &error_abort);
    if (!mr) {
        return 0;
    }

    return memory_region_size(mr);
}

static void pc_dimm_md_fill_device_info(const MemoryDeviceState *md,
                                        MemoryDeviceInfo *info)
{
    PCDIMMDeviceInfo *di = g_new0(PCDIMMDeviceInfo, 1);
    const DeviceClass *dc = DEVICE_GET_CLASS(md);
    const PCDIMMDevice *dimm = PC_DIMM(md);
    const DeviceState *dev = DEVICE(md);

    if (dev->id) {
        di->has_id = true;
        di->id = g_strdup(dev->id);
    }
    di->hotplugged = dev->hotplugged;
    di->hotpluggable = dc->hotpluggable;
    di->addr = dimm->addr;
    di->slot = dimm->slot;
    di->node = dimm->node;
    di->size = object_property_get_uint(OBJECT(dimm), PC_DIMM_SIZE_PROP,
                                        NULL);
    di->memdev = object_get_canonical_path(OBJECT(dimm->hostmem));

    if (object_dynamic_cast(OBJECT(dev), TYPE_NVDIMM)) {
        info->u.nvdimm.data = di;
        info->type = MEMORY_DEVICE_INFO_KIND_NVDIMM;
    } else {
        info->u.dimm.data = di;
        info->type = MEMORY_DEVICE_INFO_KIND_DIMM;
    }
}

static void pc_dimm_class_init(ObjectClass *oc, void *data)
{
    DeviceClass *dc = DEVICE_CLASS(oc);
    PCDIMMDeviceClass *ddc = PC_DIMM_CLASS(oc);
    MemoryDeviceClass *mdc = MEMORY_DEVICE_CLASS(oc);

    dc->realize = pc_dimm_realize;
    dc->unrealize = pc_dimm_unrealize;
    dc->props = pc_dimm_properties;
    dc->desc = "DIMM memory module";

    ddc->get_memory_region = pc_dimm_get_memory_region;
    ddc->get_vmstate_memory_region = pc_dimm_get_vmstate_memory_region;

    mdc->get_addr = pc_dimm_md_get_addr;
    /* for a dimm plugged_size == region_size */
    mdc->get_plugged_size = pc_dimm_md_get_region_size;
    mdc->get_region_size = pc_dimm_md_get_region_size;
    mdc->fill_device_info = pc_dimm_md_fill_device_info;
}

static TypeInfo pc_dimm_info = {
    .name          = TYPE_PC_DIMM,
    .parent        = TYPE_DEVICE,
    .instance_size = sizeof(PCDIMMDevice),
    .instance_init = pc_dimm_init,
    .class_init    = pc_dimm_class_init,
    .class_size    = sizeof(PCDIMMDeviceClass),
    .interfaces = (InterfaceInfo[]) {
        { TYPE_MEMORY_DEVICE },
        { }
    },
};

static void pc_dimm_register_types(void)
{
    type_register_static(&pc_dimm_info);
}

type_init(pc_dimm_register_types)
