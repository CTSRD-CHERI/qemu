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

static void qemu_ram_init(RAMBlock *block, ram_addr_t size, ram_addr_t host)
{

    assert(size > 0);

    block->size = TARGET_PAGE_ALIGN(size);
    block->host = host;

    block->length = block->size;
    block->cursor = block->host;

    block->dirty = false;
    g_atomic_int_set(&block->refcount, 0);
    block->parent = NULL;
}

ram_addr_t qemu_ram_get_cursor(const RAMBlock *block)
{

    return block->cursor;
}

ram_addr_t qemu_ram_get_length(const RAMBlock *block)
{

    return block->length;
}

static void qemu_ram_add(RAMBlock *block)
{

    assert(block != NULL);

    g_tree_insert(cheri_tagtree, block, block);
}

static void qemu_ram_remove(RAMBlock *block)
{
    gboolean result;

    assert(block != NULL);

    result = g_tree_remove(cheri_tagtree, block);
    assert(result);
}

bool offset_in_ramblock(RAMBlock *b, ram_addr_t offset)
{
    return (b && offset < qemu_ram_get_length(b)) ? true : false;
}

void *ramblock_ptr(RAMBlock *block, ram_addr_t offset)
{
    assert(offset_in_ramblock(block, offset));
    return (void *)qemu_ram_get_cursor(block) + offset;
}

RAMBlock *qemu_ram_alloc_from_ptr(ram_addr_t size, void *host)
{
    RAMBlock *new_block;

    new_block = g_malloc0(sizeof(*new_block));
    qemu_ram_init(new_block, size, (ram_addr_t)host);
    cheri_tag_init(new_block, new_block->size);
    qemu_ram_add(new_block);
    return (new_block);
}

/*
 * qemu_ram_duplicate() duplicates a RAM block but not its tag blocks.
 *
 * The returned object uses the same tag blocks as the parent object. The tag
 * blocks are freed when all duplicates are freed.
 */
static RAMBlock *qemu_ram_duplicate(RAMBlock *block)
{
    RAMBlock *new_block;

    new_block = g_malloc0(sizeof(*new_block));
    memcpy(new_block, block, sizeof(*new_block));
    new_block->parent = block;
    block->refcount++;
    return (new_block);
}

static void qemu_ram_free(RAMBlock *block)
{
    RAMBlock *parent;

    if (!block->dirty)
        qemu_ram_remove(block);

    parent = block->parent;
    if (parent != NULL) {
        assert(parent->refcount > 0);
        parent->refcount--;

        if (parent->refcount == 0 && parent->dirty)
            qemu_ram_free(parent);
    }

    if (block->refcount == 0) {
        if (parent == NULL)
            cheri_tag_free(block);

        g_free(block);
    } else {
        block->dirty = true;
    }
}

ram_addr_t qemu_ram_block_host_offset(RAMBlock *rb, void *host)
{
    ram_addr_t res;

    assert((ram_addr_t)host >= rb->host);
    res = (ram_addr_t)host - rb->host;
    assert(res < rb->size);

    return res;
}

void *qemu_ram_ptr_length(RAMBlock *ram_block, ram_addr_t addr,
                                 ram_addr_t *size, bool lock)
{
    RAMBlock *block;

    if (*size == 0) {
        return NULL;
    }

    block = ram_block;
    if (block == NULL) {
        block = qemu_get_ram_block(addr);
        addr -= qemu_ram_get_cursor(block);
    }

    *size = MIN(*size, qemu_ram_get_length(block) - addr);
    return ramblock_ptr(block, addr);
}

static void qemu_ram_narrow(RAMBlock *block, ram_addr_t cursor,
    ram_addr_t length)
{

    assert(block != NULL);
    assert(block->cursor <= cursor);
    assert(cursor + length <= block->cursor + block->length);

    block->cursor = cursor;
    block->length = length;
}

void qemu_ram_clear_region(ram_addr_t size, void *host)
{
    RAMBlock *block, *newblock, region;
    GTreeNode *node;
    ram_addr_t blockend, blockstart, leftblockend, leftblockstart;
    ram_addr_t regionend, regionstart, rightblockend, rightblockstart;

    qemu_ram_init(&region, size, (ram_addr_t)host);

    node = g_tree_lower_bound(cheri_tagtree, &region);
    while (node != NULL) {
        regionstart = qemu_ram_get_cursor(&region);
        regionend = regionstart + qemu_ram_get_length(&region) - 1;
        block = g_tree_node_value(node);
        blockstart = qemu_ram_get_cursor(block);
        blockend = blockstart + qemu_ram_get_length(block) - 1;
        node = g_tree_node_next(node);

        /*
         * The lower bound must include or end after the region's start address.
         */
        assert(regionstart <= blockend);

        if (regionend < blockstart) {
            /*
             * Stop traversing the tree once there's no overlap, i.e.
             * the block starts after the region.
             */
            break;
        }

        assert(blockstart <= regionend);

        if (regionstart <= blockstart && blockend <= regionend) {
            /*
             * The region strictly contains a block.
             *
             * Free the block.
             */
            qemu_ram_free(block);
        } else if (regionstart <= blockstart && blockstart <= regionend) {
            /*
             * The block overlaps with the region on its left-hand side.
             *
             * Narrow the block to the remaining right-hand side.
             */
            rightblockstart = regionend + 1;
            rightblockend = blockend;

            qemu_ram_remove(block);
            qemu_ram_narrow(block, rightblockstart,
                rightblockend - rightblockstart + 1);
            qemu_ram_add(block);
        } else if (regionstart <= blockend && blockend <= regionend) {
            /*
             * The block overlaps with the region on its right-hand side.
             *
             * Narrow the block to the remaining left-hand side.
             */
            leftblockstart = blockstart;
            leftblockend = regionstart - 1;

            qemu_ram_remove(block);
            qemu_ram_narrow(block, leftblockstart,
                leftblockend - leftblockstart + 1);
            qemu_ram_add(block);
        } else if (blockstart < regionstart && regionend < blockend) {
            /*
             * The block strictly contains the region and has different both
             * start and end addresses.
             *
             * Duplicate the block, narrow the duplicate block to the remaining
             * left-hand side and narrow the block to the remaining right-hand
             * side.
             */
            leftblockstart = blockstart;
            leftblockend = regionstart - 1;
            rightblockstart = regionend + 1;
            rightblockend = blockend;

            qemu_ram_remove(block);
            newblock = qemu_ram_duplicate(block);
            qemu_ram_narrow(newblock, leftblockstart,
                leftblockend - leftblockstart + 1);
            qemu_ram_narrow(block, rightblockstart,
                rightblockend - rightblockstart + 1);
            qemu_ram_add(newblock);
            qemu_ram_add(block);
        } else {
            assert("Unexpected case");
        }
    }
}
