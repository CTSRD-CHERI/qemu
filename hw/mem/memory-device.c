/*
 * Memory Device Interface
 *
 * Copyright ProfitBricks GmbH 2012
 * Copyright (C) 2014 Red Hat Inc
 * Copyright (c) 2018 Red Hat Inc
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or later.
 * See the COPYING file in the top-level directory.
 */

#include "qemu/osdep.h"
#include "hw/mem/memory-device.h"
#include "hw/qdev.h"
#include "qapi/error.h"
#include "hw/boards.h"
#include "qemu/range.h"
#include "hw/virtio/vhost.h"
#include "sysemu/kvm.h"

static gint memory_device_addr_sort(gconstpointer a, gconstpointer b)
{
    const MemoryDeviceState *md_a = MEMORY_DEVICE(a);
    const MemoryDeviceState *md_b = MEMORY_DEVICE(b);
    const MemoryDeviceClass *mdc_a = MEMORY_DEVICE_GET_CLASS(a);
    const MemoryDeviceClass *mdc_b = MEMORY_DEVICE_GET_CLASS(b);
    const uint64_t addr_a = mdc_a->get_addr(md_a);
    const uint64_t addr_b = mdc_b->get_addr(md_b);

    if (addr_a > addr_b) {
        return 1;
    } else if (addr_a < addr_b) {
        return -1;
    }
    return 0;
}

static int memory_device_build_list(Object *obj, void *opaque)
{
    GSList **list = opaque;

    if (object_dynamic_cast(obj, TYPE_MEMORY_DEVICE)) {
        DeviceState *dev = DEVICE(obj);
        if (dev->realized) { /* only realized memory devices matter */
            *list = g_slist_insert_sorted(*list, dev, memory_device_addr_sort);
        }
    }

    object_child_foreach(obj, memory_device_build_list, opaque);
    return 0;
}

static int memory_device_used_region_size(Object *obj, void *opaque)
{
    uint64_t *size = opaque;

    if (object_dynamic_cast(obj, TYPE_MEMORY_DEVICE)) {
        const DeviceState *dev = DEVICE(obj);
        const MemoryDeviceState *md = MEMORY_DEVICE(obj);
        const MemoryDeviceClass *mdc = MEMORY_DEVICE_GET_CLASS(obj);

        if (dev->realized) {
            *size += mdc->get_region_size(md);
        }
    }

    object_child_foreach(obj, memory_device_used_region_size, opaque);
    return 0;
}

static void memory_device_check_addable(MachineState *ms, uint64_t size,
                                        Error **errp)
{
    uint64_t used_region_size = 0;

    /* we will need a new memory slot for kvm and vhost */
    if (kvm_enabled() && !kvm_has_free_slot(ms)) {
        error_setg(errp, "hypervisor has no free memory slots left");
        return;
    }
    if (!vhost_has_free_slot()) {
        error_setg(errp, "a used vhost backend has no free memory slots left");
        return;
    }

    /* will we exceed the total amount of memory specified */
    memory_device_used_region_size(OBJECT(ms), &used_region_size);
    if (used_region_size + size > ms->maxram_size - ms->ram_size) {
        error_setg(errp, "not enough space, currently 0x%" PRIx64
                   " in use of total hot pluggable 0x" RAM_ADDR_FMT,
                   used_region_size, ms->maxram_size - ms->ram_size);
        return;
    }

}

uint64_t memory_device_get_free_addr(MachineState *ms, const uint64_t *hint,
                                     uint64_t align, uint64_t size,
                                     Error **errp)
{
    uint64_t address_space_start, address_space_end;
    GSList *list = NULL, *item;
    uint64_t new_addr = 0;

    if (!ms->device_memory) {
        error_setg(errp, "memory devices (e.g. for memory hotplug) are not "
                         "supported by the machine");
        return 0;
    }

    if (!memory_region_size(&ms->device_memory->mr)) {
        error_setg(errp, "memory devices (e.g. for memory hotplug) are not "
                         "enabled, please specify the maxmem option");
        return 0;
    }
    address_space_start = ms->device_memory->base;
    address_space_end = address_space_start +
                        memory_region_size(&ms->device_memory->mr);
    g_assert(QEMU_ALIGN_UP(address_space_start, align) == address_space_start);
    g_assert(address_space_end >= address_space_start);

    memory_device_check_addable(ms, size, errp);
    if (*errp) {
        return 0;
    }

    if (hint && QEMU_ALIGN_UP(*hint, align) != *hint) {
        error_setg(errp, "address must be aligned to 0x%" PRIx64 " bytes",
                   align);
        return 0;
    }

    if (QEMU_ALIGN_UP(size, align) != size) {
        error_setg(errp, "backend memory size must be multiple of 0x%"
                   PRIx64, align);
        return 0;
    }

    if (hint) {
        new_addr = *hint;
        if (new_addr < address_space_start) {
            error_setg(errp, "can't add memory [0x%" PRIx64 ":0x%" PRIx64
                       "] at 0x%" PRIx64, new_addr, size, address_space_start);
            return 0;
        } else if ((new_addr + size) > address_space_end) {
            error_setg(errp, "can't add memory [0x%" PRIx64 ":0x%" PRIx64
                       "] beyond 0x%" PRIx64, new_addr, size,
                       address_space_end);
            return 0;
        }
    } else {
        new_addr = address_space_start;
    }

    /* find address range that will fit new memory device */
    object_child_foreach(OBJECT(ms), memory_device_build_list, &list);
    for (item = list; item; item = g_slist_next(item)) {
        const MemoryDeviceState *md = item->data;
        const MemoryDeviceClass *mdc = MEMORY_DEVICE_GET_CLASS(OBJECT(md));
        uint64_t md_size, md_addr;

        md_addr = mdc->get_addr(md);
        md_size = mdc->get_region_size(md);
        if (*errp) {
            goto out;
        }

        if (ranges_overlap(md_addr, md_size, new_addr, size)) {
            if (hint) {
                const DeviceState *d = DEVICE(md);
                error_setg(errp, "address range conflicts with '%s'", d->id);
                goto out;
            }
            new_addr = QEMU_ALIGN_UP(md_addr + md_size, align);
        }
    }

    if (new_addr + size > address_space_end) {
        error_setg(errp, "could not find position in guest address space for "
                   "memory device - memory fragmented due to alignments");
        goto out;
    }
out:
    g_slist_free(list);
    return new_addr;
}

MemoryDeviceInfoList *qmp_memory_device_list(void)
{
    GSList *devices = NULL, *item;
    MemoryDeviceInfoList *list = NULL, *prev = NULL;

    object_child_foreach(qdev_get_machine(), memory_device_build_list,
                         &devices);

    for (item = devices; item; item = g_slist_next(item)) {
        const MemoryDeviceState *md = MEMORY_DEVICE(item->data);
        const MemoryDeviceClass *mdc = MEMORY_DEVICE_GET_CLASS(item->data);
        MemoryDeviceInfoList *elem = g_new0(MemoryDeviceInfoList, 1);
        MemoryDeviceInfo *info = g_new0(MemoryDeviceInfo, 1);

        mdc->fill_device_info(md, info);

        elem->value = info;
        elem->next = NULL;
        if (prev) {
            prev->next = elem;
        } else {
            list = elem;
        }
        prev = elem;
    }

    g_slist_free(devices);

    return list;
}

static int memory_device_plugged_size(Object *obj, void *opaque)
{
    uint64_t *size = opaque;

    if (object_dynamic_cast(obj, TYPE_MEMORY_DEVICE)) {
        const DeviceState *dev = DEVICE(obj);
        const MemoryDeviceState *md = MEMORY_DEVICE(obj);
        const MemoryDeviceClass *mdc = MEMORY_DEVICE_GET_CLASS(obj);

        if (dev->realized) {
            *size += mdc->get_plugged_size(md);
        }
    }

    object_child_foreach(obj, memory_device_plugged_size, opaque);
    return 0;
}

uint64_t get_plugged_memory_size(void)
{
    uint64_t size = 0;

    memory_device_plugged_size(qdev_get_machine(), &size);

    return size;
}

void memory_device_plug_region(MachineState *ms, MemoryRegion *mr,
                               uint64_t addr)
{
    /* we expect a previous call to memory_device_get_free_addr() */
    g_assert(ms->device_memory);

    memory_region_add_subregion(&ms->device_memory->mr,
                                addr - ms->device_memory->base, mr);
}

void memory_device_unplug_region(MachineState *ms, MemoryRegion *mr)
{
    /* we expect a previous call to memory_device_get_free_addr() */
    g_assert(ms->device_memory);

    memory_region_del_subregion(&ms->device_memory->mr, mr);
}

static const TypeInfo memory_device_info = {
    .name          = TYPE_MEMORY_DEVICE,
    .parent        = TYPE_INTERFACE,
    .class_size = sizeof(MemoryDeviceClass),
};

static void memory_device_register_types(void)
{
    type_register_static(&memory_device_info);
}

type_init(memory_device_register_types)
