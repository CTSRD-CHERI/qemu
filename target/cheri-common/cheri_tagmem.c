/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2015-2016 Stacey Son <sson@FreeBSD.org>
 * Copyright (c) 2016-2018 Alfredo Mazzinghi <am2419@cl.cam.ac.uk>
 * Copyright (c) 2016-2018 Alex Richardson <Alexander.Richardson@cl.cam.ac.uk>
 * All rights reserved.
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory under DARPA/AFRL contract FA8750-10-C-0237
 * ("CTSRD"), as part of the DARPA CRASH research programme.
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
#include "cheri_tagmem.h"
#include "exec/exec-all.h"
#include "exec/log.h"
#include "exec/ramblock.h"
#include "cheri_tagmem.h"
#include "cheri-helper-utils.h"
// XXX: use hbitmap? Or a different data structure?
#include "qemu/bitmap.h"
#include "glib/ghash.h"

#if defined(TARGET_MIPS)
#include "cheri_utils.h"
#include "internal.h"
#endif

#if !defined(TARGET_CHERI)
#error "Should only be included for TARGET_CHERI"
#endif

// XXX: use secure/target_tlb_bit0/target_tlb_bit1 for cheri TLB permissions?

/*
 * Tagged Memory Emulation
 *
 * CHERI requires a 1-bit tag for every capability-aligned,
 * capability-sized word in physical memory.  This allows capabilities
 * to be safely loaded and stored in meory without loss of integrity.
 *
 * For emulation purposes the tag is an one-byte flag.  This makes it
 * easy to set or unset a tag without the need of locking or atomics.
 * This requires eight times the memory, however.  To reduce the amount
 * of memory needed the tag flag array is allocated sparsely, 4K at at
 * time, and on demand.
 *
 * XXX Should consider adding a reference count per tag block so that
 * blocks can be deallocated when no longer used maybe.
 *
 * FIXME: rewrite using somethign more like the upcoming MTE changes (https://github.com/rth7680/qemu/commits/tgt-arm-mte-user)
 */

#if defined(CHERI_MAGIC128) || defined(CHERI_128)
#define CAP_TAG_SHFT        4           // 5 for 256-bit caps, 4 for 128-bit
#else /* !(CHERI_MAGIC128 || CHERI_128) */
#define CAP_TAG_SHFT        5           // 5 for 256-bit caps, 4 for 128-bit
#endif /* !(CHERI_MAGIC128 || CHERI_128) */
#define CAP_SIZE            (1 << CAP_TAG_SHFT)
#define CAP_MASK            ((1 << CAP_TAG_SHFT) - 1)
#define CAP_TAGBLK_SHFT     12          // 2^12 or 4096 tags per block
#define CAP_TAGBLK_MSK      ((1 << CAP_TAGBLK_SHFT) - 1)
#define CAP_TAGBLK_SIZE       (1 << CAP_TAGBLK_SHFT)
#define CAP_TAGBLK_IDX(tag_idx) ((tag_idx) & CAP_TAGBLK_MSK)
#ifdef CHERI_MAGIC128
// "Magic" table to store the additional 128 metadata bits for the magic128
// configuration of QEMU
static GHashTable *magic128_table;
struct Magic128Data {
    uint64_t tps;    // type, permissions, sealed
    uint64_t length; // length of capability
};
#endif

static inline size_t num_tagblocks(RAMBlock* ram)
{
    uint64_t memory_size = memory_region_size(ram->mr);
    size_t result = DIV_ROUND_UP(memory_size, CHERI_CAP_SIZE * CAP_TAGBLK_SIZE);
    assert(result == (memory_size >> CAP_TAG_SHFT) >> CAP_TAGBLK_SHFT);
    return result;
}

#define TAGMEM_USE_BITMAP 0
#if TAGMEM_USE_BITMAP
// Use one bit per tag:
typedef struct CheriTagBlock {
    DECLARE_BITMAP(tag_bitmap, CAP_TAGBLK_SIZE);
} CheriTagBlock;
#else
// Use one byte per tag:
typedef struct CheriTagBlock {
    uint8_t tags[CAP_TAGBLK_SIZE];
} CheriTagBlock;
#endif

CheriTagBlock **_cheri_tagmem = NULL;
ram_addr_t cheri_covered_start = 0;
uint64_t cheri_ntagblks = 0ul;


static inline CheriTagBlock* get_cheri_tagmem(size_t index) {
    assert(index < cheri_ntagblks && "Tag index out of bounds");
    return _cheri_tagmem[index];
}

void cheri_tag_init(MemoryRegion *mr, uint64_t memory_size)
{
    // printf("%s: memory_size=0x%lx\n", __func__, memory_size);
    assert(memory_region_is_ram(mr));
    assert(memory_region_size(mr) == memory_size &&
           "Incorrect tag mem size passed?");
    if (_cheri_tagmem != NULL)
        return;

    cheri_covered_start = memory_region_get_ram_addr(mr);
    cheri_ntagblks = num_tagblocks(mr->ram_block);
    _cheri_tagmem = (CheriTagBlock **)g_malloc0(cheri_ntagblks * sizeof(CheriTagBlock *));
    if (_cheri_tagmem == NULL) {
        error_report("%s: Can't allocated tag memory", __func__);
        exit (-1);
    }
#ifdef CHERI_MAGIC128
    if (!magic128_table) {
        magic128_table = g_hash_table_new(g_int64_hash, NULL);
    }
#endif
    if (qemu_tcg_mttcg_enabled()) {
        warn_report("The CHERI tagged memory implementation is not thread-safe "
                    "and therefore not compatible with MTTCG. Capability tags "
                    "may mysteriously appear/disappear. Run with \"--accel "
                    "tcg,thread=single\" to fix.");
    }
}

static inline hwaddr v2p_addr(CPUArchState *env, target_ulong vaddr,
                              MMUAccessType rw, int reg, uintptr_t pc,
                              int *prot)
{
    hwaddr paddr = 0;

#ifdef TARGET_MIPS
    paddr = cpu_mips_translate_address_c2(env, vaddr, rw, reg, prot, pc);
#elif defined(TARGET_RISCV)
    paddr = cpu_riscv_translate_address_tagmem(env, vaddr, rw, reg, prot, pc);
#else
#error "TODO: IMPLEMENT cpu_translate_address_tagmem"
#endif
    assert(paddr != -1LL && "Should have raised an MMU fault on failure");
    return paddr;
}

static inline void check_tagmem_writable(CPUArchState *env, target_ulong vaddr,
                                         hwaddr paddr, ram_addr_t ram_offset,
                                         MemoryRegion *mr, uintptr_t pc)
{
    if (memory_region_is_rom(mr) || memory_region_is_romd(mr)) {
        error_report(
            "QEMU ERROR: attempting change tag bit on read-only memory:");
        error_report("%s: vaddr=0x%jx -> %s+0x%jx (paddr=0x%jx)", __func__,
                     (uintmax_t)vaddr, memory_region_name(mr),
                     (uintmax_t)ram_offset, (uintmax_t)paddr);
        cpu_transaction_failed(env_cpu(env), paddr, vaddr, CHERI_CAP_SIZE,
                               MMU_DATA_STORE, // TODO: MMU_DATA_CAP_STORE
                               cpu_mmu_index(env, false),
                               MEMTXATTRS_UNSPECIFIED, MEMTX_ERROR, pc);
    }
}

static inline ram_addr_t p2r_addr(CPUArchState *env, hwaddr addr, MemoryRegion** mrp)
{
    hwaddr offset, l;
    MemoryRegion *mr;
    CPUState *cs = env_cpu(env);

    mr = address_space_translate(cs->as, addr, &offset, &l, false, MEMTXATTRS_UNSPECIFIED);
    if (mrp)
        *mrp = mr;

    // ROM/ROMD regions can have "RAM" addresses, but for our purposes we want
    // them to not have tags and so return RAM_ADDR_INVALID.
    if (memory_region_is_rom(mr) || memory_region_is_romd(mr)) {
        return RAM_ADDR_INVALID;
    }
    return memory_region_get_ram_addr(mr) + offset - cheri_covered_start;
}

static inline ram_addr_t v2r_addr(CPUArchState *env, target_ulong vaddr,
                                  hwaddr *ret_paddr, MMUAccessType rw, int reg,
                                  uintptr_t pc)
{
    int prot;
    MemoryRegion* mr = NULL;
    hwaddr paddr = v2p_addr(env, vaddr, rw, reg, pc, &prot);
    if (ret_paddr)
        *ret_paddr = paddr;
    ram_addr_t ram_addr = p2r_addr(env, paddr, &mr);
    if (rw == MMU_DATA_CAP_STORE || rw == MMU_DATA_STORE)
        check_tagmem_writable(env, vaddr, paddr, ram_addr, mr, pc);
    return ram_addr;
}

void cheri_tag_invalidate(CPUArchState *env, target_ulong vaddr, int32_t size, uintptr_t pc)
{
    // This must not cross a page boundary since we are only translating once!
    assert(size > 0);
    if (unlikely((vaddr & TARGET_PAGE_MASK) !=
                 ((vaddr + size - 1) & TARGET_PAGE_MASK))) {
#ifdef CHERI_UNALIGNED
        // this can happen with unaligned stores
        if (size == 2 || size == 4 || size == 8) {
            warn_report("Got unaligned load in %d-byte store across page boundary at 0x" TARGET_FMT_lx "\r\n", size, vaddr);
            size_t remaining_in_page = TARGET_PAGE_SIZE - (vaddr & ~TARGET_PAGE_MASK);
            assert(remaining_in_page < (size_t)size);
            // invalidate tags for both pages (two lookups required!)
            cheri_tag_invalidate(env, vaddr, remaining_in_page, pc);
            cheri_tag_invalidate(env, vaddr + remaining_in_page, size - remaining_in_page, pc);
            return;
        }
#endif
        qemu_log_flush();
        error_report("FATAL: %s: " TARGET_FMT_lx "+%d crosses a page boundary\r", __func__, vaddr, size);
        char buffer[256];
        FILE* f = fmemopen(buffer, sizeof(buffer), "w");
        assert(f);
        fprintf(f, "Probably caused by guest instruction: ");
        target_disas(f, env_cpu(env), cpu_get_current_pc(env, pc, false),
                     /* Only one instr*/ -1);
        fprintf(f, "\r");
        fclose(f);
        buffer[sizeof(buffer) - 1] = '\0';
        error_report("%s", buffer);
        exit(1);
    }

    /*
     * When resolving this address in the TLB, treat it like a data store
     * (MMU_DATA_STORE) rather than a capability store (MMU_DATA_CAP_STORE),
     * so that we don't require that the SC inhibit be clear.
     */
    void* host_addr = probe_write(env, vaddr, size, cpu_mmu_index(env, false), pc);
    // Only RAM and ROM regions are backed by host addresses so if probe_write()
    // returns NULL we know that we can't write the tagmem.
    if (unlikely(!host_addr))
        return;

    ram_addr_t offset;
    RAMBlock *block = qemu_ram_block_from_host(host_addr, false, &offset);
    if (unlikely(!block)) {
        // Not backed by RAM?
        error_report("%s: vaddr=0x%jx -> host_addr=%p not backed by RAM?",
                     __func__, (uintmax_t)vaddr, host_addr);
        return;
    }
    ram_addr_t ram_addr = block->offset + offset;
    MemoryRegion* mr = block->mr;

    // XXX: I am not sure if this can ever trigger since probe_write should have
    // returned NULL in that case?
    cheri_debug_assert(!memory_region_is_rom(mr) && !memory_region_is_romd(mr));
    cheri_tag_phys_invalidate(env, ram_addr, size, &vaddr);
}

void cheri_tag_phys_invalidate(CPUArchState *env, ram_addr_t ram_addr, ram_addr_t len, const target_ulong* vaddr)
{
    uint64_t tag, tagmem_idx;
    CheriTagBlock *tagblk;

    ram_addr_t endaddr = (uint64_t)(ram_addr + len);
    ram_addr_t startaddr = QEMU_ALIGN_DOWN(ram_addr, CAP_SIZE);

    for(ram_addr_t addr = startaddr; addr < endaddr; addr += CAP_SIZE) {
        tag = addr >> CAP_TAG_SHFT;
        tagmem_idx = tag >> CAP_TAGBLK_SHFT;
        if (tagmem_idx >= cheri_ntagblks) {
            qemu_log_mask(CPU_LOG_INSTR, "Could not find tag block for RAM addr " RAM_ADDR_FMT "\n", addr);
            return ;
        }
        tagblk = get_cheri_tagmem(tagmem_idx);

        if (tagblk != NULL) {
            const size_t tagblk_index = CAP_TAGBLK_IDX(tag);
            if (unlikely(env && vaddr && should_log_mem_access(env, CPU_LOG_INSTR, *vaddr))) {
                ram_addr_t write_vaddr = QEMU_ALIGN_DOWN(*vaddr, CAP_SIZE) + (addr - startaddr);
                qemu_log("    Cap Tag Write [" RAM_ADDR_FMT "] %d -> 0\n", write_vaddr,
                         tagblk->tags[tagblk_index]);
            }
            if (unlikely(env && should_log_mem_access(env, CPU_LOG_INSTR, addr))) {
                qemu_log("    Cap Tag ramaddr Write [" RAM_ADDR_FMT "] %d -> 0\n", addr,
                         tagblk->tags[tagblk_index]);
            }
            // changed |= tagblk[CAP_TAGBLK_IDX(tag)];
            tagblk->tags[tagblk_index] = 0;
        }
    }
#ifdef TARGET_MIPS
    /* If a tag was cleared, unset the linkedflag and reset lladdr: */
    /* Check RAM address to see if the linkedflag needs to be reset. */
    if (env && QEMU_ALIGN_DOWN(ram_addr, CHERI_CAP_SIZE) ==
        QEMU_ALIGN_DOWN(env->CP0_LLAddr, CHERI_CAP_SIZE)) {
        env->linkedflag = 0;
        env->lladdr = 1;
    }
#endif
}

static CheriTagBlock *cheri_tag_new_tagblk(uint64_t tag)
{
    CheriTagBlock *tagblk, *old;

    tagblk = g_malloc0(sizeof(CheriTagBlock));
    if (tagblk == NULL) {
        error_report("Can't allocate tag block.");
        exit(1);
    }

    /* Possible race here so use atomic compare and swap. */
    assert((tag >> CAP_TAGBLK_SHFT) < cheri_ntagblks && "Tag index out of range");
    old = atomic_cmpxchg(&_cheri_tagmem[tag >> CAP_TAGBLK_SHFT],
            NULL, tagblk);
    if (old != NULL) {
        /* Lost the race, free. */
        g_free(tagblk);
        return old;
    } else {
        return tagblk;
    }
}

void cheri_tag_set(CPUArchState *env, target_ulong vaddr, int reg, hwaddr* ret_paddr, uintptr_t pc)
{
    ram_addr_t ram_addr;
    uint64_t tag;
    CheriTagBlock *tagblk;

    /*
     * This attempt to resolve a virtual address may cause both a data store
     * TLB fault (entry missing or D bit clear) and a capability store TLB
     * fault (SC bit set).  v2r_addr bypasses the generic QEMU TCG soft-TLB
     * (env->tlb_table and friends) but will populate the MIPS-specific TLB
     * (env->tlb->mmu.r4k.tlb).  Therefore, while a data store to vaddr
     * after a tag has been set might miss in the QEMU TCG soft-TLB, it
     * won't miss in the MIPS model and therefore won't raise a processor
     * exception (and will populate the QEMU TCG soft-TLB for subsequent
     * data stores).
     */

    ram_addr = v2r_addr(env, vaddr, ret_paddr, MMU_DATA_CAP_STORE, reg, pc);
    if (ram_addr == RAM_ADDR_INVALID)
        return;

    /* Get the tag number and tag block ptr. */
    tag = ram_addr >> CAP_TAG_SHFT;
    tagblk = get_cheri_tagmem(tag >> CAP_TAGBLK_SHFT);

    if (tagblk == NULL) {
        /* Allocated a tag block. */
        tagblk = cheri_tag_new_tagblk(tag);
    }
    if (unlikely(should_log_mem_access(env, CPU_LOG_INSTR, vaddr))) {
        qemu_log("    Cap Tag Write [" TARGET_FMT_lx "] %d -> 1\n", vaddr,
                 tagblk->tags[CAP_TAGBLK_IDX(tag)]);
    }
    tagblk->tags[CAP_TAGBLK_IDX(tag)] = 1;

#ifdef TARGET_MIPS
    /* Check RAM address to see if the linkedflag needs to be reset. */
    // FIXME: we should really be using a different approach for LL/SC
    if (QEMU_ALIGN_DOWN(ram_addr, CHERI_CAP_SIZE) ==
        QEMU_ALIGN_DOWN(env->CP0_LLAddr, CHERI_CAP_SIZE)) {
        env->linkedflag = 0;
        env->lladdr = 1;
    }
#endif
}

static CheriTagBlock *
cheri_tag_get_block(CPUArchState *env, target_ulong vaddr, MMUAccessType at,
    int reg, int xshift, uintptr_t pc,
    hwaddr *ret_paddr, ram_addr_t *ret_ram_addr, uint64_t *ret_tag, int *prot)
{
    hwaddr paddr;
    ram_addr_t ram_addr;
    uint64_t tag;

    paddr = v2p_addr(env, vaddr, at, reg, pc, prot);

    if (ret_paddr)
        *ret_paddr = paddr;

    ram_addr = p2r_addr(env, paddr, NULL);
    if (ram_addr == RAM_ADDR_INVALID)
        return NULL;

    if (ret_ram_addr)
        *ret_ram_addr = ram_addr;

    /* Get the tag number and tag block ptr. */
    tag = (ram_addr >> (CAP_TAG_SHFT + xshift)) << xshift;
    if (ret_tag)
        *ret_tag = tag;

    return get_cheri_tagmem(tag >> CAP_TAGBLK_SHFT);
}

int cheri_tag_get(CPUArchState *env, target_ulong vaddr, int reg,
        hwaddr *ret_paddr, int *prot, uintptr_t pc)
{
    uint64_t tag;
    CheriTagBlock *tagblk = cheri_tag_get_block(env, vaddr, MMU_DATA_CAP_LOAD, reg,
                                          0, pc, ret_paddr, NULL, &tag, prot);
    bool result;
    if (tagblk == NULL)
        result = false;
    else
        result = tagblk->tags[CAP_TAGBLK_IDX(tag)];
    if (unlikely(should_log_mem_access(env, CPU_LOG_INSTR, vaddr))) {
        qemu_log("    Cap Tag Read [" TARGET_FMT_lx "] -> %d\n", vaddr,
                 result);
    }
    return result;
}

/* QEMU currently tells the kernel that there are no caches installed
 * (xref target/mips/translate_init.inc.c MIPS_CONFIG1 definition)
 * so we're kind of free to make up a line size here.  For simplicity,
 * we pretend that our cache lines always contain 8 capabilities.
 */
#define CAP_TAG_GET_MANY_SHFT    3
int cheri_tag_get_many(CPUArchState *env, target_ulong vaddr, int reg,
        hwaddr *ret_paddr, uintptr_t pc)
{
    uint64_t tag;
    int prot;
    CheriTagBlock *tagblk = cheri_tag_get_block(env, vaddr, MMU_DATA_CAP_LOAD, reg,
                                          CAP_TAG_GET_MANY_SHFT, pc, ret_paddr,
                                          NULL, &tag, &prot);

    /*
     * XXX Right now, the sole consumer of this function is CLoadTags, and
     * we let it "see around" the TLB capability load inhibit.  That should
     * perhaps change?
     */
    (void) prot;

    if (tagblk == NULL)
        return 0;
    else {
#define TAG_BYTE_TO_BIT(ix) (tagblk->tags[CAP_TAGBLK_IDX(tag+ix)] ? (1 << ix) : 0)
        return TAG_BYTE_TO_BIT(0)
             | TAG_BYTE_TO_BIT(1)
             | TAG_BYTE_TO_BIT(2)
             | TAG_BYTE_TO_BIT(3)
             | TAG_BYTE_TO_BIT(4)
             | TAG_BYTE_TO_BIT(5)
             | TAG_BYTE_TO_BIT(6)
             | TAG_BYTE_TO_BIT(7);
#undef TAG_BYTE_TO_BIT
    }
}

#ifdef CHERI_MAGIC128
void cheri_tag_set_m128(CPUArchState *env, target_ulong vaddr, int reg,
                        uint8_t tagbit, uint64_t tps, uint64_t length,
                        hwaddr *ret_paddr, uintptr_t pc)
{
    // We index the "magic" table by physical address:
    hwaddr paddr;
    cheri_tag_set(env, vaddr, reg, &paddr, pc);
    gpointer htable_key = GINT_TO_POINTER(paddr);
    struct Magic128Data *data = g_hash_table_lookup(magic128_table, htable_key);
    if (!data) {
        data = g_malloc(sizeof(struct Magic128Data));
        g_hash_table_insert(magic128_table, htable_key, data);
    }
    data->tps = tps;
    data->length = length;
    if (ret_paddr)
        *ret_paddr = paddr;
}

int cheri_tag_get_m128(CPUArchState *env, target_ulong vaddr, int reg,
                       uint64_t *ret_tps, uint64_t *ret_length,
                       hwaddr *ret_paddr, int *prot, uintptr_t pc)
{
    hwaddr paddr;
    bool tag = cheri_tag_get(env, vaddr, reg, &paddr, prot, pc);
    // Only fetch the extra data if the value is tagged
    struct Magic128Data *data = NULL;
    if (tag) {
        data = g_hash_table_lookup(magic128_table, GINT_TO_POINTER(paddr));
    }
    if (data) {
        *ret_tps = data->tps;
        *ret_length = data->length;
    } else {
        *ret_tps = 0ULL;
        *ret_length = 0ULL;
    }
    if (ret_paddr)
        *ret_paddr = paddr;
    return tag;
}
#endif /* CHERI_MAGIC128 */
