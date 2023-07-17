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

/*
 * cheri_tagtree consists of nodes that represent distinct memory allocations
 * with their respective tag memory blocks.
 */
GTree *cheri_tagtree;

/*
 * cheri_tagtree_cmp_lookup() compares two non-overlapping or same blocks a and
 * b, and returns:
 * - -1 if a < b (a.host + a.size < b.host)
 * -  0 if a = b (a.host = b.host and a.size = b.size)
 * -  1 if a > b (b.host + b.size < a.host)
 *
 * This function is used to insert new blocks into a tree.
 */
static gint cheri_tagtree_cmp_lookup(gconstpointer a, gconstpointer b,
    gpointer user_data)
{
    const struct RAMBlock *blocka, *blockb;
    vm_offset_t hostaddra, hostaddrb;

    blocka = (const struct RAMBlock *)a;
    blockb = (const struct RAMBlock *)b;

    /*
     * Order blocks by their start addresses.
     */
    hostaddra = (uintptr_t)blocka->host;
    hostaddrb = (uintptr_t)blockb->host;
    if (hostaddra < hostaddrb) {
        assert(hostaddra + blocka->size <= hostaddrb);
        return (-1);
    } else if (hostaddrb < hostaddra) {
        assert(hostaddrb + blockb->size <= hostaddra);
        return (1);
    } else {
        assert(blocka->size == blockb->size);
        return (0);
    }
}

/*
 * cheri_tagtree_cmp_search() compares a block block with an address addr,
 * and returns:
 * - -1 if addr < block (addr < block.host)
 * -  0 if addr in block (block.host <= addr and addr < block.host + block.size)
 * -  1 if addr > block (block.host + block.size < addr)
 *
 * This function is used to find a block in a tree that contains an address.
 */
static gint cheri_tagtree_cmp_search(gconstpointer a, gconstpointer b)
{
    const struct RAMBlock *block;
    vm_offset_t addr;

    block = (const struct RAMBlock *)a;
    addr = (vm_offset_t)b;

    if ((vm_offset_t)block->host <= addr) {
        if (addr < (vm_offset_t)block->host + block->size) {
            /* 
             * The current block starts before the address and does include the
             * address.
             *
             * Return the current block.
             */
            return (0);
        } else {
            /*
             * The current block starts before the address and does not include
             * the address.
             *
             * Look for another block after the current block that includes the
             * address.
             */
            return (1);
        }
    } else {
        /*
         * The current block starts after the address.
         *
         * Look for another block before the current block.
         */
        return (-1);
    }
}

void cheri_tagtree_init(void)
{

    cheri_tagtree = g_tree_new_full(cheri_tagtree_cmp_lookup, NULL, NULL, NULL);
}

struct RAMBlock *cheri_tagtree_find(ram_addr_t addr)
{
    RAMBlock *result;

    result = g_tree_search(cheri_tagtree, cheri_tagtree_cmp_search,
        (gconstpointer)(uintptr_t)addr);
    return (result);
}
