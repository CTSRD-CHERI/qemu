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
};

vdevState *vdev_create(MemoryRegion *address_space, hwaddr base);

#endif /* !_HW_VDEV_ */
