#include "qemu/osdep.h"
#include "qapi/error.h"
#include "qemu/log.h"
#include "hw/sysbus.h"
#include "chardev/char.h"
#include "hw/hw.h"
#include "hw/irq.h"
#include "hw/core/cpu.h"
#include "hw/vdev/vdev.h"
#include "sysemu/cpus.h"
#include "qemu/main-loop.h" /* iothread mutex */

#define	EPW_READ_ADDRESS		0x0000	/* read only */
#define	EPW_READ_FLIT_SIZE		0x0008	/* read only */
#define	EPW_READ_BURST_COUNT		0x000C	/* read only */
#define	EPW_READ_RESPONSE_DATA		0x0040	/* read/write */
#define	EPW_READ_SIZE			0x0048	/* read only */
#define	EPW_WRITE_ADDRESS		0x1000	/* read only */
#define	EPW_WRITE_BYTE_ENABLE		0x1008	/* read only */
#define	EPW_WRITE_DATA			0x1040	/* read only */
#define	EPW_WRITE_SIZE			0x1048	/* read only */
#define	EPW_TIME_STAMP			0x2000	/* read only */
#define	EPW_REQUEST_ID			0x2004	/* read only */
#define	EPW_REQUEST_IS_WRITE		0x2006	/* read only */
#define	EPW_REQUEST_LEVEL_SEND_RESPONSE	0x2007	/* read/write */
#define	EPW_ENABLE_DEVICE_EMULATION	0x2008	/* read/write */

struct epw_request {
	uint64_t read_addr;
	uint64_t write_addr;
	uint8_t is_write;
	uint64_t flit_size;
	uint32_t burst_count;
	uint32_t byte_enable;
	//uint8_t data[32];
	uint64_t data64;
	uint8_t data_len;
	uint8_t pending;
	uint8_t done;
	uint8_t enabled;
	uint32_t read_size;
	uint32_t write_size;
};

static struct epw_request req;
StatusInfo *qmp_query_status(Error **errp);

static uint64_t
vdev_read(void *opaque, hwaddr addr, unsigned int size)
{
	CPUState *cpu;
	vdevState *s;
	//uint8_t *v;

	s = opaque;

	fprintf(stderr, "%s: addr %jx size %x\n", __func__, addr, size);

	switch (addr) {
	case EPW_REQUEST_LEVEL_SEND_RESPONSE:
		return (req.pending);
	case EPW_REQUEST_IS_WRITE:
		return (req.is_write);
	case EPW_WRITE_ADDRESS:
	case EPW_READ_ADDRESS:
		return (req.read_addr);
	case EPW_READ_SIZE:
		return (req.read_size);
	case EPW_WRITE_SIZE:
		return (req.write_size);
	case EPW_READ_FLIT_SIZE:
		return (req.flit_size);
	case EPW_READ_BURST_COUNT:
		return (req.burst_count);
	case EPW_WRITE_BYTE_ENABLE:
		return (req.byte_enable);
	case EPW_WRITE_DATA:
		return (req.data64);
#if 0
	case EPW_WRITE_DATA ... (EPW_WRITE_DATA + 0x8):
		v = (uint8_t *)&req.data64;
		return (v[addr - 0x1040] & 0xff);
#endif
	};

	qemu_log_mask(LOG_GUEST_ERROR, "%s: read: addr=0x%x size=%d\n",
	    __func__, (int)addr,size);

	while (1);

	CPU_FOREACH(cpu) {
		//fprintf(stderr, "CPU #%d:\n", cpu->cpu_index);
		if (cpu->cpu_index == 0)
			cpu->stop = true;
	};

	cpu = current_cpu;

	return s->reg[addr];
}

static void
vdev_write(void *opaque, hwaddr addr, uint64_t val64, unsigned int size)
{
	unsigned char ch;
	uint32_t value;
	vdevState *s;
	uint8_t *v;

	/* dummy code for future development */
	s = opaque;
	value = val64;
	ch = value;

	fprintf(stderr, "%s: addr %jx val %jx size %x\n",
	    __func__, addr, val64, size);

	switch (addr) {
	case EPW_ENABLE_DEVICE_EMULATION:
		req.enabled = 1;
		break;
	case EPW_READ_RESPONSE_DATA:
		req.data64 = val64;
		break;
#if 0
	case EPW_READ_RESPONSE_DATA ... (EPW_READ_RESPONSE_DATA + 0x8):
		v = (uint8_t *)&req.data64;
		v[addr - 0x40] = val64 & 0xff;
		break;
#endif
	case EPW_REQUEST_LEVEL_SEND_RESPONSE:
		req.pending = 0;
		break;
	default:
		printf("unknown req: addr %jx\n", addr);
		while (1);
	};

	qemu_log_mask(LOG_GUEST_ERROR, "%s: write: addr=0x%x v=0x%x\n",
	    __func__, (int)addr, (int)value);
}

static uint64_t
vdev_window_read(void *opaque, hwaddr addr, unsigned int size)
{
	CPUState *cpu;
	vdevState *w;

	w = opaque;

	fprintf(stderr, "%s: addr %jx size %x\n", __func__, addr, size);

#if 0
	StatusInfo *info;
	info = qmp_query_status(NULL);

	fprintf(stderr,
	    "%s: current_run_state %x\n", __func__, info->status);

	CPU_FOREACH(cpu) {
		fprintf(stderr, "CPU #%d: stopped %d\n", cpu->cpu_index,
		    cpu->stop);
	};
#endif

	if (req.enabled == 0)
		return (0);

	req.is_write = 0;
	req.flit_size = 1;
	req.burst_count = 4;
	req.read_addr = addr + w->base;
	req.read_size = size;
	req.pending = 1;

	qemu_mutex_unlock_iothread();
	while (req.pending != 0) {
		fprintf(stderr, "%s: sleeping for 1 sec\n", __func__);
		usleep(100000);
	}
	qemu_mutex_lock_iothread();

	return (req.data64);
}

static void
vdev_window_write(void *opaque, hwaddr addr, uint64_t val64, unsigned int size)
{
	vdevState *w;

	w = opaque;

	fprintf(stderr, "%s: addr %jx val %jx size %x\n",
	    __func__, addr, val64, size);

	if (req.enabled == 0)
		return;

	req.is_write = 1;
	req.byte_enable = size;
	req.write_addr = addr + w->base;
	req.write_size = size;
	req.data64 = val64;
	req.pending = 1;

	qemu_mutex_unlock_iothread();
	while (req.pending != 0) {
		fprintf(stderr, "%s: sleeping for 1 sec\n", __func__);
		usleep(100000);
	}
	qemu_mutex_lock_iothread();
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
	.endianness = DEVICE_LITTLE_ENDIAN,
	.valid = {
		.min_access_size = 1,
		.max_access_size = 8
	},
	.impl = {
		.min_access_size = 1,
		.max_access_size = 8,
	},
};

static const MemoryRegionOps vdev_window_ops = {
	.read = vdev_window_read,
	.write = vdev_window_write,
	.endianness = DEVICE_LITTLE_ENDIAN,
	.valid = {
		.min_access_size = 1,
		.max_access_size = 8
	},
	.impl = {
		.min_access_size = 1,
		.max_access_size = 8,
	},
};

vdevState *
vdev_create(MemoryRegion *address_space, hwaddr base, hwaddr base_size,
    hwaddr window, hwaddr window_size)
{
	vdevState *s, *w;

	s = g_malloc0(sizeof(vdevState));
	vdev_init(s);
	s->base = base;
	s->size = base_size;
	memory_region_init_io(&s->mmio, NULL, &vdev_ops,
	    s, TYPE_VIRTUAL_DEVICE, base_size);
	memory_region_add_subregion(address_space, base, &s->mmio);

	w = g_malloc0(sizeof(vdevState));
	w->base = window;
	w->size = window_size;
	vdev_init(w);
	memory_region_init_io(&w->mmio, NULL, &vdev_window_ops,
	    w, TYPE_VIRTUAL_DEVICE, window_size);
	memory_region_add_subregion(address_space, window, &w->mmio);

	bzero(&req, sizeof(struct epw_request));

	return (s);
}
