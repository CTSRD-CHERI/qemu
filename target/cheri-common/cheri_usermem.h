/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2023 Konrad Witaszczyk
 *
 * This software was developed by the University of Cambridge Computer
 * Laboratory (Department of Computer Science and Technology) under Office of
 * Naval Research (ONR) Contract No. N00014-22-1-2463 ("SoftWare Integrated
 * with Secure Hardware (SWISH)").
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

#ifndef _CHERI_USERMEM_H_
#define _CHERI_USERMEM_H_

#include "qemu/osdep.h"
#include "exec/exec-all.h"
#include "exec/ramblock.h"

#if !defined(TARGET_CHERI)
#error "Should only be included for TARGET_CHERI"
#endif

#if !defined(CONFIG_USER_ONLY)
#error "Should only be included for CONFIG_USER_ONLY"
#endif

ram_addr_t qemu_ram_addr_from_host(void *ptr);
RAMBlock *qemu_find_ram_block(ram_addr_t addr);
RAMBlock *qemu_get_ram_block(ram_addr_t addr);
ram_addr_t qemu_ram_get_cursor(const RAMBlock *block);
ram_addr_t qemu_ram_get_length(const RAMBlock *block);
RAMBlock *qemu_ram_alloc_from_ptr(ram_addr_t size, void *host);

ram_addr_t qemu_ram_block_host_offset(RAMBlock *rb, void *host);
void *qemu_ram_ptr_length(RAMBlock *ram_block, ram_addr_t addr,
                                 hwaddr *size, bool lock);
void qemu_ram_clear_region(ram_addr_t size, void *host);

void *ramblock_ptr(RAMBlock *block, ram_addr_t offset);
bool offset_in_ramblock(RAMBlock *b, ram_addr_t offset);


#endif /* !_CHERI_USERMEM_H_ */
