/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2021 Ruslan Bukin <br@bsdpad.com>
 * All rights reserved.
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory (Department of Computer Science and
 * Technology) under DARPA contract HR0011-18-C-0016 ("ECATS"), as part of the
 * DARPA SSITH research programme.
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
#include "qapi/error.h"
#include "qemu/log.h"
#include "hw/sysbus.h"
#include "chardev/char.h"
#include "hw/hw.h"
#include "hw/irq.h"
#include "hw/core/cpu.h"
#include "hw/vdev/vdev.h"
#include "sysemu/cpus.h"
#include "qemu/main-loop.h"

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
	uint64_t data;
	uint8_t pending;
	uint8_t enabled;
	uint32_t read_size;
	uint32_t write_size;
};

static struct epw_request req;

static uint64_t
vdev_read(void *opaque, hwaddr addr, unsigned int size)
{
	vdevState *s;

	s = opaque;

	qemu_log_mask(LOG_GUEST_ERROR, "%s: read: addr=0x%jx size=%d\n",
		__func__, addr, size);

	switch (addr) {
	case EPW_REQUEST_LEVEL_SEND_RESPONSE:
		return (req.pending);
	case EPW_REQUEST_IS_WRITE:
		return (req.is_write);
	case EPW_WRITE_ADDRESS:
		return (req.write_addr);
	case EPW_READ_ADDRESS:
		return (req.read_addr);
	case EPW_READ_SIZE:
		return (req.read_size);
	case EPW_WRITE_SIZE:
		return (req.write_size);
	case EPW_WRITE_DATA:
		return (req.data);
	default:
		return (0);
	};

	return (0);
}

static void
vdev_write(void *opaque, hwaddr addr, uint64_t val64, unsigned int size)
{
	vdevState *s;

	s = opaque;

	qemu_log_mask(LOG_GUEST_ERROR, "%s: write: addr=0x%jx v=0x%jx\n",
	    __func__, addr, val64);

	switch (addr) {
	case EPW_ENABLE_DEVICE_EMULATION:
		req.enabled = 1;
		break;
	case EPW_READ_RESPONSE_DATA:
		req.data = val64;
		break;
	case EPW_REQUEST_LEVEL_SEND_RESPONSE:
		req.pending = 0;
		break;
	default:
		printf("%s: unknown req: addr %jx\n", __func__, addr);
		break;
	};
}

static uint64_t
vdev_window_read(void *opaque, hwaddr addr, unsigned int size)
{
	vdevState *w;

	w = opaque;

	if (req.enabled == 0)
		return (0);

	req.is_write = 0;
	req.read_addr = addr + w->base;
	req.read_size = size;
	req.pending = 1;

	qemu_mutex_unlock_iothread();
	while (req.pending != 0)
		usleep(1000);
	qemu_mutex_lock_iothread();

	return (req.data);
}

static void
vdev_window_write(void *opaque, hwaddr addr, uint64_t val64, unsigned int size)
{
	vdevState *w;

	w = opaque;

	if (req.enabled == 0)
		return;

	req.is_write = 1;
	req.write_addr = addr + w->base;
	req.write_size = size;
	req.data = val64;
	req.pending = 1;

	qemu_mutex_unlock_iothread();
	while (req.pending != 0)
		usleep(1000);
	qemu_mutex_lock_iothread();
}

static void
vdev_init(vdevState *s)
{

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
