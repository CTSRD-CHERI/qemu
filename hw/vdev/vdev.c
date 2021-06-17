#include "qemu/osdep.h"
#include "qapi/error.h"
#include "qemu/log.h"
#include "hw/sysbus.h"
#include "chardev/char.h"
#include "hw/hw.h"
#include "hw/irq.h"
#include "hw/vdev/vdev.h"

static uint64_t
vdev_read(void *opaque, hwaddr addr, unsigned int size)
{
	vdevState *s;

	s = opaque;

	qemu_log_mask(LOG_GUEST_ERROR, "%s: read: addr=0x%x size=%d\n",
	    __func__, (int)addr,size);

	return s->reg[addr];
}

static void
vdev_write(void *opaque, hwaddr addr, uint64_t val64, unsigned int size)
{
	unsigned char ch;
	uint32_t value;
	vdevState *s;

	/* dummy code for future development */
	s = opaque;
	value = val64;
	ch = value;

	qemu_log_mask(LOG_GUEST_ERROR, "%s: write: addr=0x%x v=0x%x\n",
	    __func__, (int)addr, (int)value);
}

static void
vdev_init(vdevState *s)
{

	s->reg[0]='B';
	s->reg[1]='U';
	s->reg[2]='T';
	s->reg[3]='T';
	s->reg[4]='E';
	s->reg[5]='R';
}

static const MemoryRegionOps vdev_ops = {
	.read = vdev_read,
	.write = vdev_write,
	.endianness = DEVICE_NATIVE_ENDIAN,
	.valid = {
		.min_access_size = 1,
		.max_access_size = 6
	}
};

vdevState *
vdev_create(MemoryRegion *address_space, hwaddr base)
{
	vdevState *s;

	s = g_malloc0(sizeof(vdevState));
	vdev_init(s);
	memory_region_init_io(&s->mmio, NULL, &vdev_ops,
	    s, TYPE_VIRTUAL_DEVICE, 32);
	memory_region_add_subregion(address_space, base, &s->mmio);

	return (s);
}
