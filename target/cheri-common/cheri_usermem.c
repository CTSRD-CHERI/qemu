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

#include "qemu/osdep.h"
#include "exec/exec-all.h"
#include "exec/ramblock.h"

#if !defined(TARGET_CHERI)
#error "Should only be included for TARGET_CHERI"
#endif

#if !defined(CONFIG_USER_ONLY)
#error "Should only be included for CONFIG_USER_ONLY"
#endif

#include "cheri_tagmem.h"
#include "cheri_tagtree.h"
#include "cheri_usermem.h"

/*
 * This file implements functions from softmmu/physmem.c required by the tags
 * implementation for the user mode.
 */

/*
 * In the user mode, RAM addresses are equivalent to host addresses.
 */
ram_addr_t qemu_ram_addr_from_host(void *ptr)
{

    return (ram_addr_t)(uintptr_t)ptr;
}

RAMBlock *qemu_find_ram_block(ram_addr_t addr)
{

    return (cheri_tagtree_find(addr));
}

RAMBlock *qemu_get_ram_block(ram_addr_t addr)
{
    RAMBlock *block;

    block = qemu_find_ram_block(addr);
    if (block == NULL) {
        fprintf(stderr, "Bad ram offset %" PRIx64 "\n", (uint64_t)addr);
        abort();
    }
    return block;
}

RAMBlock *qemu_ram_alloc_from_ptr(ram_addr_t size, void *host)
{
    RAMBlock *new_block;

    new_block = g_malloc0(sizeof(*new_block));
    new_block->size = TARGET_PAGE_ALIGN(size);
    new_block->host = host;
    cheri_tag_init(new_block, new_block->size);
    g_tree_insert(cheri_tagtree, new_block, new_block);
    return (new_block);
}

ram_addr_t qemu_ram_block_host_offset(RAMBlock *rb, void *host)
{
    ram_addr_t res = (uint8_t *)host - (uint8_t *)rb->host;
    assert((uintptr_t)host >= (uintptr_t)rb->host);
    assert(res < rb->size);

    return res;
}

void *qemu_ram_ptr_length(RAMBlock *ram_block, ram_addr_t addr,
                                 hwaddr *size, bool lock)
{
    RAMBlock *block;

    if (*size == 0) {
        return NULL;
    }

    block = ram_block;
    if (block == NULL) {
        block = qemu_get_ram_block(addr);
        assert(block->host != NULL);
        addr -= (uintptr_t)block->host;
    }

    *size = MIN(*size, block->size - addr);
    return ramblock_ptr(block, addr);
}
