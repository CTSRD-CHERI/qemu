#ifndef _HW_VDEV_
#define _HW_VDEV_

#include "hw/sysbus.h"
#include "qom/object.h"

#define TYPE_VIRTUAL_DEVICE "VIRTUAL_DEVICE"

typedef struct vdevState vdevState;

DECLARE_INSTANCE_CHECKER(vdevState, VIRTUAL_DEVICE, TYPE_VIRTUAL_DEVICE)

struct vdevState
{
    MemoryRegion mmio;
    unsigned char reg[6];
    hwaddr base;
    hwaddr size;
};

vdevState *vdev_create(MemoryRegion *address_space, hwaddr base, hwaddr base_size, hwaddr window, hwaddr window_size);

#endif /* !_HW_VDEV_ */
