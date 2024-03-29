/*
 * MIPS TLB (Translation lookaside buffer) helpers.
 *
 *  Copyright (c) 2004-2005 Jocelyn Mayer
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */
#include "qemu/osdep.h"

#include "cpu.h"
#include "internal.h"
#include "exec/exec-all.h"
#include "exec/cpu_ldst.h"
#include "exec/log.h"
#include "exec/log_instr.h"
#include "qemu/atomic.h"
#include "qemu/error-report.h"
#include "hw/mips/cpudevs.h"

#ifdef TARGET_CHERI
#include "cheri-helper-utils.h"
#endif

enum {
#ifdef TARGET_CHERI
    TLBRET_S = -5,
#else
    TLBRET_XI = -6,
    TLBRET_RI = -5,
#endif /* TARGET_CHERI */
    TLBRET_DIRTY = -4,
    TLBRET_INVALID = -3,
    TLBRET_NOMATCH = -2,
    TLBRET_BADADDR = -1,
    TLBRET_MATCH = 0
};

#if !defined(CONFIG_USER_ONLY)

/* no MMU emulation */
int no_mmu_map_address(CPUMIPSState *env, hwaddr *physical, int *prot,
                       target_ulong address, MMUAccessType access_type)
{
    *physical = address;
    *prot = PAGE_READ | PAGE_WRITE | PAGE_EXEC;
    return TLBRET_MATCH;
}

/* fixed mapping MMU emulation */
int fixed_mmu_map_address(CPUMIPSState *env, hwaddr *physical, int *prot,
                          target_ulong address, MMUAccessType access_type)
{
    if (address <= (int32_t)0x7FFFFFFFUL) {
        if (!(env->CP0_Status & (1 << CP0St_ERL))) {
            *physical = address + 0x40000000UL;
        } else {
            *physical = address;
        }
    } else if (address <= (int32_t)0xBFFFFFFFUL) {
        *physical = address & 0x1FFFFFFF;
    } else {
        *physical = address;
    }

    *prot = PAGE_READ | PAGE_WRITE | PAGE_EXEC;
    return TLBRET_MATCH;
}

/* MIPS32/MIPS64 R4000-style MMU emulation */
int r4k_map_address(CPUMIPSState *env, hwaddr *physical, int *prot,
                    target_ulong address, MMUAccessType access_type)
{
    uint16_t ASID = env->CP0_EntryHi & env->CP0_EntryHi_ASID_mask;
    uint32_t MMID = env->CP0_MemoryMapID;
    bool mi = !!((env->CP0_Config5 >> CP0C5_MI) & 1);
    uint32_t tlb_mmid;
    int i;

    MMID = mi ? MMID : (uint32_t) ASID;

#if defined(TARGET_CHERI)
    unsigned gclg_bit;
    if (address < 0x4000000000000000) {
        /* useg, xuseg */
        gclg_bit = CP0EnHi_CLGU;
    } else if (address < 0x8000000000000000) {
        /* xsseg */
        gclg_bit = CP0EnHi_CLGS;
    } else if (address < 0xFFFFFFFFC0000000) {
        /* xkphys (won't be called), xkseg, kseg0, kseg1 */
        gclg_bit = CP0EnHi_CLGK;
    } else if (address < 0xFFFFFFFFE0000000) {
        /* sseg */
        gclg_bit = CP0EnHi_CLGS;
    } else {
        /* kseg3 */
        gclg_bit = CP0EnHi_CLGK;
    }
    bool gclg = !!(env->CP0_EntryHi & (1UL << gclg_bit));
#endif

    for (i = 0; i < env->tlb->tlb_in_use; i++) {
        r4k_tlb_t *tlb = &env->tlb->mmu.r4k.tlb[i];
        /* 1k pages are not supported. */
        target_ulong mask = tlb->PageMask | ~(TARGET_PAGE_MASK << 1);
        target_ulong tag = address & ~mask;
        target_ulong VPN = tlb->VPN & ~mask;
#if defined(TARGET_MIPS64)
        tag &= env->SEGMask;
#endif

        /* Check ASID/MMID, virtual page number & size */
        tlb_mmid = mi ? tlb->MMID : (uint32_t) tlb->ASID;
        if ((tlb->G == 1 || tlb_mmid == MMID) && VPN == tag && !tlb->EHINV) {
            /* TLB match */
            int n = !!(address & mask & ~(mask >> 1));
            /* Check access rights */
            if (!(n ? tlb->V1 : tlb->V0)) {
                return TLBRET_INVALID;
            }
#if defined(TARGET_CHERI)
            if (access_type == MMU_DATA_CAP_STORE) {
                /*
                 * If we're trying to do a cap-store, first check for the
                 * dirty/store-permitted bit before looking at the the
                 * store-capability inhibit.
                 */
                if (!(n ? tlb->D1 : tlb->D0)) {
                    return TLBRET_DIRTY;
                }
                if (n ? tlb->S1 : tlb->S0) {
                    return TLBRET_S;
                }
            }

            if (n ? tlb->S1 : tlb->S0) {
                *prot |= PAGE_SC_TRAP;
            }
#else
            if (access_type == MMU_INST_FETCH && (n ? tlb->XI1 : tlb->XI0)) {
                return TLBRET_XI;
            }
            if (access_type == MMU_DATA_LOAD && (n ? tlb->RI1 : tlb->RI0)) {
                return TLBRET_RI;
            }
#endif /* TARGET_CHERI */
            if (((access_type != MMU_DATA_STORE)
#if defined(TARGET_CHERI)
                  && (access_type != MMU_DATA_CAP_STORE)
#endif
                ) || (n ? tlb->D1 : tlb->D0)) {

                *physical = tlb->PFN[n] | (address & (mask >> 1));
                *prot = PAGE_READ;
                if (n ? tlb->D1 : tlb->D0) {
                    *prot |= PAGE_WRITE;
                }
#if !defined(TARGET_CHERI)
                if (!(n ? tlb->XI1 : tlb->XI0)) {
#else
                if (true) {
#endif
                    *prot |= PAGE_EXEC;
                }

#if defined(TARGET_CHERI)
                if (n ? tlb->L1 : tlb->L0) {
                    *prot |= PAGE_LC_CLEAR;
                }
                bool pclg = n ? tlb->CLG1 : tlb->CLG0;
                if (pclg != gclg) {
                    *prot |= PAGE_LC_TRAP;
                }
#endif

                return TLBRET_MATCH;
            }
            return TLBRET_DIRTY;
        }
    }
    return TLBRET_NOMATCH;
}

static void no_mmu_init(CPUMIPSState *env, const mips_def_t *def)
{
    env->tlb->nb_tlb = 1;
    env->tlb->map_address = &no_mmu_map_address;
}

static void fixed_mmu_init(CPUMIPSState *env, const mips_def_t *def)
{
    env->tlb->nb_tlb = 1;
    env->tlb->map_address = &fixed_mmu_map_address;
}

static void r4k_mmu_init(CPUMIPSState *env, const mips_def_t *def)
{
    env->tlb->nb_tlb = 1 + ((def->CP0_Config1 >> CP0C1_MMU) & 63);
    env->tlb->map_address = &r4k_map_address;
    env->tlb->helper_tlbwi = r4k_helper_tlbwi;
    env->tlb->helper_tlbwr = r4k_helper_tlbwr;
    env->tlb->helper_tlbp = r4k_helper_tlbp;
    env->tlb->helper_tlbr = r4k_helper_tlbr;
    env->tlb->helper_tlbinv = r4k_helper_tlbinv;
    env->tlb->helper_tlbinvf = r4k_helper_tlbinvf;
}

void mmu_init(CPUMIPSState *env, const mips_def_t *def)
{
    env->tlb = g_malloc0(sizeof(CPUMIPSTLBContext));

    switch (def->mmu_type) {
    case MMU_TYPE_NONE:
        no_mmu_init(env, def);
        break;
    case MMU_TYPE_R4000:
        r4k_mmu_init(env, def);
        break;
    case MMU_TYPE_FMT:
        fixed_mmu_init(env, def);
        break;
    case MMU_TYPE_R3000:
    case MMU_TYPE_R6000:
    case MMU_TYPE_R8000:
    default:
        cpu_abort(env_cpu(env), "MMU type not supported\n");
    }
}

static int is_seg_am_mapped(unsigned int am, bool eu, int mmu_idx)
{
    /*
     * Interpret access control mode and mmu_idx.
     *           AdE?     TLB?
     *      AM  K S U E  K S U E
     * UK    0  0 1 1 0  0 - - 0
     * MK    1  0 1 1 0  1 - - !eu
     * MSK   2  0 0 1 0  1 1 - !eu
     * MUSK  3  0 0 0 0  1 1 1 !eu
     * MUSUK 4  0 0 0 0  0 1 1 0
     * USK   5  0 0 1 0  0 0 - 0
     * -     6  - - - -  - - - -
     * UUSK  7  0 0 0 0  0 0 0 0
     */
    int32_t adetlb_mask;

    switch (mmu_idx) {
    case 3: /* ERL */
        /* If EU is set, always unmapped */
        if (eu) {
            return 0;
        }
        /* fall through */
    case MIPS_HFLAG_KM:
        /* Never AdE, TLB mapped if AM={1,2,3} */
        adetlb_mask = 0x70000000;
        goto check_tlb;

    case MIPS_HFLAG_SM:
        /* AdE if AM={0,1}, TLB mapped if AM={2,3,4} */
        adetlb_mask = 0xc0380000;
        goto check_ade;

    case MIPS_HFLAG_UM:
        /* AdE if AM={0,1,2,5}, TLB mapped if AM={3,4} */
        adetlb_mask = 0xe4180000;
        /* fall through */
    check_ade:
        /* does this AM cause AdE in current execution mode */
        if ((adetlb_mask << am) < 0) {
            return TLBRET_BADADDR;
        }
        adetlb_mask <<= 8;
        /* fall through */
    check_tlb:
        /* is this AM mapped in current execution mode */
        return ((adetlb_mask << am) < 0);
    default:
        assert(0);
        return TLBRET_BADADDR;
    };
}

static int get_seg_physical_address(CPUMIPSState *env, hwaddr *physical,
                                    int *prot, target_ulong real_address,
                                    MMUAccessType access_type, int mmu_idx,
                                    unsigned int am, bool eu,
                                    target_ulong segmask,
                                    hwaddr physical_base)
{
    int mapped = is_seg_am_mapped(am, eu, mmu_idx);

    if (mapped < 0) {
        /* is_seg_am_mapped can report TLBRET_BADADDR */
        return mapped;
    } else if (mapped) {
        /* The segment is TLB mapped */
        return env->tlb->map_address(env, physical, prot, real_address,
                                     access_type);
    } else {
        /* The segment is unmapped */
        *physical = physical_base | (real_address & segmask);
        *prot = PAGE_READ | PAGE_WRITE | PAGE_EXEC;
        return TLBRET_MATCH;
    }
}

static int get_segctl_physical_address(CPUMIPSState *env, hwaddr *physical,
                                       int *prot, target_ulong real_address,
                                       MMUAccessType access_type, int mmu_idx,
                                       uint16_t segctl, target_ulong segmask)
{
    unsigned int am = (segctl & CP0SC_AM_MASK) >> CP0SC_AM;
    bool eu = (segctl >> CP0SC_EU) & 1;
    hwaddr pa = ((hwaddr)segctl & CP0SC_PA_MASK) << 20;

    return get_seg_physical_address(env, physical, prot, real_address,
                                    access_type, mmu_idx, am, eu, segmask,
                                    pa & ~(hwaddr)segmask);
}

static int get_physical_address(CPUMIPSState *env, hwaddr *physical,
                                int *prot, target_ulong real_address,
                                MMUAccessType access_type, int mmu_idx)
{
    /* User mode can only access useg/xuseg */
#if defined(TARGET_MIPS64)
    int user_mode = mmu_idx == MIPS_HFLAG_UM;
    int supervisor_mode = mmu_idx == MIPS_HFLAG_SM;
    int kernel_mode = !user_mode && !supervisor_mode;
    int UX = (env->CP0_Status & (1 << CP0St_UX)) != 0;
    int SX = (env->CP0_Status & (1 << CP0St_SX)) != 0;
    int KX = (env->CP0_Status & (1 << CP0St_KX)) != 0;
#endif
    int ret = TLBRET_MATCH;
    /* effective address (modified for KVM T&E kernel segments) */
    target_ulong address = real_address;

#define USEG_LIMIT      ((target_ulong)(int32_t)0x7FFFFFFFUL)
#define KSEG0_BASE      ((target_ulong)(int32_t)0x80000000UL)
#define KSEG1_BASE      ((target_ulong)(int32_t)0xA0000000UL)
#define KSEG2_BASE      ((target_ulong)(int32_t)0xC0000000UL)
#define KSEG3_BASE      ((target_ulong)(int32_t)0xE0000000UL)

#define KVM_KSEG0_BASE  ((target_ulong)(int32_t)0x40000000UL)
#define KVM_KSEG2_BASE  ((target_ulong)(int32_t)0x60000000UL)

    if (mips_um_ksegs_enabled()) {
        /* KVM T&E adds guest kernel segments in useg */
        if (real_address >= KVM_KSEG0_BASE) {
            if (real_address < KVM_KSEG2_BASE) {
                /* kseg0 */
                address += KSEG0_BASE - KVM_KSEG0_BASE;
            } else if (real_address <= USEG_LIMIT) {
                /* kseg2/3 */
                address += KSEG2_BASE - KVM_KSEG2_BASE;
            }
        }
    }

    if (address <= USEG_LIMIT) {
        /* useg */
        uint16_t segctl;

        if (address >= 0x40000000UL) {
            segctl = env->CP0_SegCtl2;
        } else {
            segctl = env->CP0_SegCtl2 >> 16;
        }
        ret = get_segctl_physical_address(env, physical, prot,
                                          real_address, access_type,
                                          mmu_idx, segctl, 0x3FFFFFFF);
#if defined(TARGET_MIPS64)
    } else if (address < 0x4000000000000000ULL) {
        /* xuseg */
        if (UX && address <= (0x3FFFFFFFFFFFFFFFULL & env->SEGMask)) {
            ret = env->tlb->map_address(env, physical, prot,
                                        real_address, access_type);
        } else {
            ret = TLBRET_BADADDR;
        }
    } else if (address < 0x8000000000000000ULL) {
        /* xsseg */
        if ((supervisor_mode || kernel_mode) &&
            SX && address <= (0x7FFFFFFFFFFFFFFFULL & env->SEGMask)) {
            ret = env->tlb->map_address(env, physical, prot,
                                        real_address, access_type);
        } else {
            ret = TLBRET_BADADDR;
        }
    } else if (address < 0xC000000000000000ULL) {
        /* xkphys */
        if ((address & 0x07FFFFFFFFFFFFFFULL) <= env->PAMask) {
            /* KX/SX/UX bit to check for each xkphys EVA access mode */
            static const uint8_t am_ksux[8] = {
                [CP0SC_AM_UK]    = (1u << CP0St_KX),
                [CP0SC_AM_MK]    = (1u << CP0St_KX),
                [CP0SC_AM_MSK]   = (1u << CP0St_SX),
                [CP0SC_AM_MUSK]  = (1u << CP0St_UX),
                [CP0SC_AM_MUSUK] = (1u << CP0St_UX),
                [CP0SC_AM_USK]   = (1u << CP0St_SX),
                [6]              = (1u << CP0St_KX),
                [CP0SC_AM_UUSK]  = (1u << CP0St_UX),
            };
            unsigned int am = CP0SC_AM_UK;
            unsigned int xr = (env->CP0_SegCtl2 & CP0SC2_XR_MASK) >> CP0SC2_XR;

            if (xr & (1 << ((address >> 59) & 0x7))) {
                am = (env->CP0_SegCtl1 & CP0SC1_XAM_MASK) >> CP0SC1_XAM;
            }
            /* Does CP0_Status.KX/SX/UX permit the access mode (am) */
            if (env->CP0_Status & am_ksux[am]) {
                ret = get_seg_physical_address(env, physical, prot,
                                               real_address, access_type,
                                               mmu_idx, am, false, env->PAMask,
                                               0);
            } else {
                ret = TLBRET_BADADDR;
            }
        } else {
            ret = TLBRET_BADADDR;
        }
    } else if (address < 0xFFFFFFFF80000000ULL) {
        /* xkseg */
        if (kernel_mode && KX &&
            address <= (0xFFFFFFFF7FFFFFFFULL & env->SEGMask)) {
            ret = env->tlb->map_address(env, physical, prot,
                                        real_address, access_type);
        } else {
            ret = TLBRET_BADADDR;
        }
#endif
    /*
     * XXXAR: I am not entirely sure deleting the previous code using  is the correct way to make commit
     * a78cda6e0212ccb16711d0dd30c2bb3480b36415 (40 bits for CHERI) to work with
     * the new get_segctl_physical_address added in commit
     * 480e79aedd322fcfac17052caff21626ea7c78e2. As far as I can see from the
     * BERI hardware reference we should still be using 0x1FFFFFFF as the mask
     * for kseg0 and kseg1
     */
    } else if (address < KSEG1_BASE) {
        /* kseg0 */
        ret = get_segctl_physical_address(env, physical, prot, real_address,
                                          access_type, mmu_idx,
                                          env->CP0_SegCtl1 >> 16, 0x1FFFFFFF);
    } else if (address < KSEG2_BASE) {
        /* kseg1 */
        ret = get_segctl_physical_address(env, physical, prot, real_address,
                                          access_type, mmu_idx,
                                          env->CP0_SegCtl1, 0x1FFFFFFF);
    } else if (address < KSEG3_BASE) {
        /* sseg (kseg2) */
        ret = get_segctl_physical_address(env, physical, prot, real_address,
                                          access_type, mmu_idx,
                                          env->CP0_SegCtl0 >> 16, 0x1FFFFFFF);
    } else {
        /*
         * kseg3
         * XXX: debug segment is not emulated
         */
        ret = get_segctl_physical_address(env, physical, prot, real_address,
                                          access_type, mmu_idx,
                                          env->CP0_SegCtl0, 0x1FFFFFFF);
    }
    return ret;
}

void cpu_mips_tlb_flush(CPUMIPSState *env)
{
    /* Flush qemu's TLB and discard all shadowed entries.  */
    tlb_flush(env_cpu(env));
    env->tlb->tlb_in_use = env->tlb->nb_tlb;
}

#endif /* !CONFIG_USER_ONLY */

#ifdef TARGET_CHERI
static void raise_mmu_exception(CPUMIPSState *env, target_ulong address,
                                MMUAccessType access_type, int tlb_error, int reg)
#else
static void raise_mmu_exception(CPUMIPSState *env, target_ulong address,
                                MMUAccessType access_type, int tlb_error)
#endif
{
    CPUState *cs = env_cpu(env);
    MipsExcp exception = 0;
    int error_code = 0;

    if (access_type == MMU_INST_FETCH) {
        error_code |= EXCP_INST_NOTAVAIL;
    }
    switch (tlb_error) {
    default:
    case TLBRET_BADADDR:
        /* Reference to kernel address from user mode or supervisor mode */
        /* Reference to supervisor address from user mode */
        if (access_type == MMU_DATA_STORE || access_type == MMU_DATA_CAP_STORE) {
            exception = EXCP_AdES;
        } else {
            exception = EXCP_AdEL;
        }
        break;
    case TLBRET_NOMATCH:
        /* No TLB match for a mapped address */
        if (access_type == MMU_DATA_STORE || access_type == MMU_DATA_CAP_STORE) {
            exception = EXCP_TLBS;
        } else {
            exception = EXCP_TLBL;
        }
        error_code |= EXCP_TLB_NOMATCH;
        break;
    case TLBRET_INVALID:
        /* TLB match with no valid bit */
        if (access_type == MMU_DATA_STORE || access_type == MMU_DATA_CAP_STORE) {
            exception = EXCP_TLBS;
        } else {
            exception = EXCP_TLBL;
        }
        break;
    case TLBRET_DIRTY:
        /* TLB match but 'D' bit is cleared */
        exception = EXCP_LTLBL;
        break;
#ifdef TARGET_CHERI
    case TLBRET_S:
        /* TLB capability store bit was set, blocking capability store. */
        cpu_mips_store_capcause(env, reg, CapEx_TLBNoStoreCap);
        exception = EXCP_C2E;
        break;
#else
    case TLBRET_XI:
        /* Execute-Inhibit Exception */
        if (env->CP0_PageGrain & (1 << CP0PG_IEC)) {
            exception = EXCP_TLBXI;
        } else {
            exception = EXCP_TLBL;
        }
        break;
    case TLBRET_RI:
        /* Read-Inhibit Exception */
        if (env->CP0_PageGrain & (1 << CP0PG_IEC)) {
            exception = EXCP_TLBRI;
        } else {
            exception = EXCP_TLBL;
        }
        break;
#endif /* TARGET_CHERI */
    }
    /* Raise exception */
    if (!(env->hflags & MIPS_HFLAG_DM)) {
        env->CP0_BadVAddr = address;
    }
    env->CP0_Context = (env->CP0_Context & ~0x007fffff) |
                       ((address >> 9) & 0x007ffff0);
    env->CP0_EntryHi = (env->CP0_EntryHi & env->CP0_EntryHi_ASID_mask) |
                       (env->CP0_EntryHi & CP0EnHi_CLG_MASK) |
                       (env->CP0_EntryHi & (1 << CP0EnHi_EHINV)) |
                       (address & (TARGET_PAGE_MASK << 1));
#if defined(TARGET_MIPS64)
    env->CP0_EntryHi &= env->SEGMask | CP0EnHi_CLG_MASK;
    env->CP0_XContext =
        (env->CP0_XContext & ((~0ULL) << (env->SEGBITS - 7))) | /* PTEBase */
        (extract64(address, 62, 2) << (env->SEGBITS - 9)) |     /* R       */
        (extract64(address, 13, env->SEGBITS - 13) << 4);       /* BadVPN2 */
#endif
    cs->exception_index = exception;
    env->error_code = error_code;
#ifdef TARGET_CHERI
    if (access_type == MMU_INST_FETCH)
        env->statcounters_itlb_miss++;
    else
        env->statcounters_dtlb_miss++;
#endif
}

#if !defined(CONFIG_USER_ONLY)

hwaddr mips_cpu_get_phys_page_debug(CPUState *cs, vaddr addr)
{
    MIPSCPU *cpu = MIPS_CPU(cs);
    CPUMIPSState *env = &cpu->env;
    hwaddr phys_addr;
    int prot;

    if (get_physical_address(env, &phys_addr, &prot, addr, MMU_DATA_LOAD,
                             cpu_mmu_index(env, false)) != 0) {
        return -1;
    }
    return phys_addr;
}

#if !defined(TARGET_MIPS64)

/*
 * Perform hardware page table walk
 *
 * Memory accesses are performed using the KERNEL privilege level.
 * Synchronous exceptions detected on memory accesses cause a silent exit
 * from page table walking, resulting in a TLB or XTLB Refill exception.
 *
 * Implementations are not required to support page table walk memory
 * accesses from mapped memory regions. When an unsupported access is
 * attempted, a silent exit is taken, resulting in a TLB or XTLB Refill
 * exception.
 *
 * Note that if an exception is caused by AddressTranslation or LoadMemory
 * functions, the exception is not taken, a silent exit is taken,
 * resulting in a TLB or XTLB Refill exception.
 */

static bool get_pte(CPUMIPSState *env, uint64_t vaddr, int entry_size,
        uint64_t *pte)
{
    if ((vaddr & ((entry_size >> 3) - 1)) != 0) {
        return false;
    }
    if (entry_size == 64) {
        *pte = cpu_ldq_code(env, vaddr);
    } else {
        *pte = cpu_ldl_code(env, vaddr);
    }
    return true;
}

static uint64_t get_tlb_entry_layout(CPUMIPSState *env, uint64_t entry,
        int entry_size, int ptei)
{
    uint64_t result = entry;
    uint64_t rixi;
    if (ptei > entry_size) {
        ptei -= 32;
    }
    result >>= (ptei - 2);
    rixi = result & 3;
    result >>= 2;
    result |= rixi << CP0EnLo_XI;
    return result;
}

static int walk_directory(CPUMIPSState *env, uint64_t *vaddr,
        int directory_index, bool *huge_page, bool *hgpg_directory_hit,
        uint64_t *pw_entrylo0, uint64_t *pw_entrylo1)
{
    int dph = (env->CP0_PWCtl >> CP0PC_DPH) & 0x1;
    int psn = (env->CP0_PWCtl >> CP0PC_PSN) & 0x3F;
    int hugepg = (env->CP0_PWCtl >> CP0PC_HUGEPG) & 0x1;
    int pf_ptew = (env->CP0_PWField >> CP0PF_PTEW) & 0x3F;
    int ptew = (env->CP0_PWSize >> CP0PS_PTEW) & 0x3F;
    int native_shift = (((env->CP0_PWSize >> CP0PS_PS) & 1) == 0) ? 2 : 3;
    int directory_shift = (ptew > 1) ? -1 :
            (hugepg && (ptew == 1)) ? native_shift + 1 : native_shift;
    int leaf_shift = (ptew > 1) ? -1 :
            (ptew == 1) ? native_shift + 1 : native_shift;
    uint32_t direntry_size = 1 << (directory_shift + 3);
    uint32_t leafentry_size = 1 << (leaf_shift + 3);
    uint64_t entry;
    uint64_t paddr;
    int prot;
    uint64_t lsb = 0;
    uint64_t w = 0;

    if (get_physical_address(env, &paddr, &prot, *vaddr, MMU_DATA_LOAD,
                             cpu_mmu_index(env, false)) !=
                             TLBRET_MATCH) {
        /* wrong base address */
        return 0;
    }
    if (!get_pte(env, *vaddr, direntry_size, &entry)) {
        return 0;
    }

    if ((entry & (1 << psn)) && hugepg) {
        *huge_page = true;
        *hgpg_directory_hit = true;
        entry = get_tlb_entry_layout(env, entry, leafentry_size, pf_ptew);
        w = directory_index - 1;
        if (directory_index & 0x1) {
            /* Generate adjacent page from same PTE for odd TLB page */
            lsb = (1 << w) >> 6;
            *pw_entrylo0 = entry & ~lsb; /* even page */
            *pw_entrylo1 = entry | lsb; /* odd page */
        } else if (dph) {
            int oddpagebit = 1 << leaf_shift;
            uint64_t vaddr2 = *vaddr ^ oddpagebit;
            if (*vaddr & oddpagebit) {
                *pw_entrylo1 = entry;
            } else {
                *pw_entrylo0 = entry;
            }
            if (get_physical_address(env, &paddr, &prot, vaddr2, MMU_DATA_LOAD,
                                     cpu_mmu_index(env, false)) !=
                                     TLBRET_MATCH) {
                return 0;
            }
            if (!get_pte(env, vaddr2, leafentry_size, &entry)) {
                return 0;
            }
            entry = get_tlb_entry_layout(env, entry, leafentry_size, pf_ptew);
            if (*vaddr & oddpagebit) {
                *pw_entrylo0 = entry;
            } else {
                *pw_entrylo1 = entry;
            }
        } else {
            return 0;
        }
        return 1;
    } else {
        *vaddr = entry;
        return 2;
    }
}

static bool page_table_walk_refill(CPUMIPSState *env, vaddr address,
                                   int mmu_idx)
{
    int gdw = (env->CP0_PWSize >> CP0PS_GDW) & 0x3F;
    int udw = (env->CP0_PWSize >> CP0PS_UDW) & 0x3F;
    int mdw = (env->CP0_PWSize >> CP0PS_MDW) & 0x3F;
    int ptw = (env->CP0_PWSize >> CP0PS_PTW) & 0x3F;
    int ptew = (env->CP0_PWSize >> CP0PS_PTEW) & 0x3F;

    /* Initial values */
    bool huge_page = false;
    bool hgpg_bdhit = false;
    bool hgpg_gdhit = false;
    bool hgpg_udhit = false;
    bool hgpg_mdhit = false;

    int32_t pw_pagemask = 0;
    target_ulong pw_entryhi = 0;
    uint64_t pw_entrylo0 = 0;
    uint64_t pw_entrylo1 = 0;

    /* Native pointer size */
    /*For the 32-bit architectures, this bit is fixed to 0.*/
    int native_shift = (((env->CP0_PWSize >> CP0PS_PS) & 1) == 0) ? 2 : 3;

    /* Indices from PWField */
    int pf_gdw = (env->CP0_PWField >> CP0PF_GDW) & 0x3F;
    int pf_udw = (env->CP0_PWField >> CP0PF_UDW) & 0x3F;
    int pf_mdw = (env->CP0_PWField >> CP0PF_MDW) & 0x3F;
    int pf_ptw = (env->CP0_PWField >> CP0PF_PTW) & 0x3F;
    int pf_ptew = (env->CP0_PWField >> CP0PF_PTEW) & 0x3F;

    /* Indices computed from faulting address */
    int gindex = (address >> pf_gdw) & ((1 << gdw) - 1);
    int uindex = (address >> pf_udw) & ((1 << udw) - 1);
    int mindex = (address >> pf_mdw) & ((1 << mdw) - 1);
    int ptindex = (address >> pf_ptw) & ((1 << ptw) - 1);

    /* Other HTW configs */
    int hugepg = (env->CP0_PWCtl >> CP0PC_HUGEPG) & 0x1;

    /* HTW Shift values (depend on entry size) */
    int directory_shift = (ptew > 1) ? -1 :
            (hugepg && (ptew == 1)) ? native_shift + 1 : native_shift;
    int leaf_shift = (ptew > 1) ? -1 :
            (ptew == 1) ? native_shift + 1 : native_shift;

    /* Offsets into tables */
    int goffset = gindex << directory_shift;
    int uoffset = uindex << directory_shift;
    int moffset = mindex << directory_shift;
    int ptoffset0 = (ptindex >> 1) << (leaf_shift + 1);
    int ptoffset1 = ptoffset0 | (1 << (leaf_shift));

    uint32_t leafentry_size = 1 << (leaf_shift + 3);

    /* Starting address - Page Table Base */
    uint64_t vaddr = env->CP0_PWBase;

    uint64_t dir_entry;
    uint64_t paddr;
    int prot;
    int m;

    if (!(env->CP0_Config3 & (1 << CP0C3_PW))) {
        /* walker is unimplemented */
        return false;
    }
    if (!(env->CP0_PWCtl & (1 << CP0PC_PWEN))) {
        /* walker is disabled */
        return false;
    }
    if (!(gdw > 0 || udw > 0 || mdw > 0)) {
        /* no structure to walk */
        return false;
    }
    if ((directory_shift == -1) || (leaf_shift == -1)) {
        return false;
    }

    /* Global Directory */
    if (gdw > 0) {
        vaddr |= goffset;
        switch (walk_directory(env, &vaddr, pf_gdw, &huge_page, &hgpg_gdhit,
                               &pw_entrylo0, &pw_entrylo1))
        {
        case 0:
            return false;
        case 1:
            goto refill;
        case 2:
        default:
            break;
        }
    }

    /* Upper directory */
    if (udw > 0) {
        vaddr |= uoffset;
        switch (walk_directory(env, &vaddr, pf_udw, &huge_page, &hgpg_udhit,
                               &pw_entrylo0, &pw_entrylo1))
        {
        case 0:
            return false;
        case 1:
            goto refill;
        case 2:
        default:
            break;
        }
    }

    /* Middle directory */
    if (mdw > 0) {
        vaddr |= moffset;
        switch (walk_directory(env, &vaddr, pf_mdw, &huge_page, &hgpg_mdhit,
                               &pw_entrylo0, &pw_entrylo1))
        {
        case 0:
            return false;
        case 1:
            goto refill;
        case 2:
        default:
            break;
        }
    }

    /* Leaf Level Page Table - First half of PTE pair */
    vaddr |= ptoffset0;
    if (get_physical_address(env, &paddr, &prot, vaddr, MMU_DATA_LOAD,
                             cpu_mmu_index(env, false)) !=
                             TLBRET_MATCH) {
        return false;
    }
    if (!get_pte(env, vaddr, leafentry_size, &dir_entry)) {
        return false;
    }
    dir_entry = get_tlb_entry_layout(env, dir_entry, leafentry_size, pf_ptew);
    pw_entrylo0 = dir_entry;

    /* Leaf Level Page Table - Second half of PTE pair */
    vaddr |= ptoffset1;
    if (get_physical_address(env, &paddr, &prot, vaddr, MMU_DATA_LOAD,
                             cpu_mmu_index(env, false)) !=
                             TLBRET_MATCH) {
        return false;
    }
    if (!get_pte(env, vaddr, leafentry_size, &dir_entry)) {
        return false;
    }
    dir_entry = get_tlb_entry_layout(env, dir_entry, leafentry_size, pf_ptew);
    pw_entrylo1 = dir_entry;

refill:

    m = (1 << pf_ptw) - 1;

    if (huge_page) {
        switch (hgpg_bdhit << 3 | hgpg_gdhit << 2 | hgpg_udhit << 1 |
                hgpg_mdhit)
        {
        case 4:
            m = (1 << pf_gdw) - 1;
            if (pf_gdw & 1) {
                m >>= 1;
            }
            break;
        case 2:
            m = (1 << pf_udw) - 1;
            if (pf_udw & 1) {
                m >>= 1;
            }
            break;
        case 1:
            m = (1 << pf_mdw) - 1;
            if (pf_mdw & 1) {
                m >>= 1;
            }
            break;
        }
    }
    pw_pagemask = m >> TARGET_PAGE_BITS_MIN;
    update_pagemask(env, pw_pagemask << CP0PM_MASK, &pw_pagemask);
    pw_entryhi = (address & ~0x1fff) |
                 (env->CP0_EntryHi & (0xFF | CP0EnHi_CLG_MASK));
    {
        target_ulong tmp_entryhi = env->CP0_EntryHi;
        int32_t tmp_pagemask = env->CP0_PageMask;
        uint64_t tmp_entrylo0 = env->CP0_EntryLo0;
        uint64_t tmp_entrylo1 = env->CP0_EntryLo1;

        env->CP0_EntryHi = pw_entryhi;
        env->CP0_PageMask = pw_pagemask;
        env->CP0_EntryLo0 = pw_entrylo0;
        env->CP0_EntryLo1 = pw_entrylo1;

        /*
         * The hardware page walker inserts a page into the TLB in a manner
         * identical to a TLBWR instruction as executed by the software refill
         * handler.
         */
        r4k_helper_tlbwr(env);

        env->CP0_EntryHi = tmp_entryhi;
        env->CP0_PageMask = tmp_pagemask;
        env->CP0_EntryLo0 = tmp_entrylo0;
        env->CP0_EntryLo1 = tmp_entrylo1;
    }
    return true;
}
#endif
#endif /* !CONFIG_USER_ONLY */

bool mips_cpu_tlb_fill(CPUState *cs, vaddr address, int size,
                       MMUAccessType access_type, int mmu_idx,
                       bool probe, uintptr_t retaddr)
{
    MIPSCPU *cpu = MIPS_CPU(cs);
    CPUMIPSState *env = &cpu->env;
#if !defined(CONFIG_USER_ONLY)
    hwaddr physical;
    int prot;
#endif
    int ret = TLBRET_BADADDR;

    /* data access */
#if !defined(CONFIG_USER_ONLY)
    MemTxAttrs attrs = MEMTXATTRS_UNSPECIFIED;
#ifdef TARGET_CHERI
    attrs.tag_setting = access_type == MMU_DATA_CAP_STORE;
#endif
    /* XXX: put correct access by using cpu_restore_state() correctly */
    ret = get_physical_address(env, &physical, &prot, address,
                               access_type, mmu_idx);
    switch (ret) {
    case TLBRET_MATCH:
        qemu_log_mask(CPU_LOG_MMU,
                      "%s address=%" VADDR_PRIx " physical " TARGET_FMT_plx
                      " prot %d\n", __func__, address, physical, prot);
        break;
    default:
        qemu_log_mask(CPU_LOG_MMU,
                      "%s address=%" VADDR_PRIx " ret %d\n", __func__, address,
                      ret);
        break;
    }
    if (ret == TLBRET_MATCH) {
        tlb_set_page_with_attrs(cs, address & TARGET_PAGE_MASK,
                                physical & TARGET_PAGE_MASK, attrs, prot,
                                mmu_idx, TARGET_PAGE_SIZE);
        return true;
    }
#if !defined(TARGET_MIPS64)
    if ((ret == TLBRET_NOMATCH) && (env->tlb->nb_tlb > 1)) {
        /*
         * Memory reads during hardware page table walking are performed
         * as if they were kernel-mode load instructions.
         */
        int mode = (env->hflags & MIPS_HFLAG_KSU);
        bool ret_walker;
        env->hflags &= ~MIPS_HFLAG_KSU;
        ret_walker = page_table_walk_refill(env, address, mmu_idx);
        env->hflags |= mode;
        if (ret_walker) {
            ret = get_physical_address(env, &physical, &prot, address,
                                       access_type, mmu_idx);
            if (ret == TLBRET_MATCH) {
                tlb_set_page_with_attrs(cs, address & TARGET_PAGE_MASK,
                                        physical & TARGET_PAGE_MASK, attrs,
                                        prot, mmu_idx, TARGET_PAGE_SIZE);
                return true;
            }
        }
    }
#endif
    if (probe) {
        return false;
    }
#endif

#ifdef TARGET_CHERI
    /*
     * NOTE: The register for the exception has possibly already been set.
     * An extra reg argument might have been preferable, but this interferes
     * less with the other targets, which don't pass a register number here.
     */
    raise_mmu_exception(env, address, access_type, ret, 0xff);
#else
    raise_mmu_exception(env, address, access_type, ret);
#endif
    do_raise_exception_err(env, cs->exception_index, env->error_code, retaddr);
}

#ifndef CONFIG_USER_ONLY
hwaddr cpu_mips_translate_address(CPUMIPSState *env, target_ulong address,
                                  MMUAccessType access_type)
{
    hwaddr physical;
    int prot;
    int ret = 0;

    /* data access */
    ret = get_physical_address(env, &physical, &prot, address, access_type,
                               cpu_mmu_index(env, false));
    if (ret != TLBRET_MATCH) {
#ifdef TARGET_CHERI
        raise_mmu_exception(env, address, access_type, ret, 0xff);
#else
        raise_mmu_exception(env, address, access_type, ret);
#endif
        return -1LL;
    } else {
        return physical;
    }
}

static void set_hflags_for_handler(CPUMIPSState *env)
{
    /* Exception handlers are entered in 32-bit mode.  */
    env->hflags &= ~(MIPS_HFLAG_M16);
    /* ...except that microMIPS lets you choose.  */
    if (env->insn_flags & ASE_MICROMIPS) {
        env->hflags |= (!!(env->CP0_Config3 &
                           (1 << CP0C3_ISA_ON_EXC))
                        << MIPS_HFLAG_M16_SHIFT);
    }
}

static inline void set_badinstr_registers(CPUMIPSState *env)
{
    if (env->insn_flags & ISA_NANOMIPS32) {
        if (env->CP0_Config3 & (1 << CP0C3_BI)) {
            uint32_t instr = (cpu_lduw_code(env, PC_ADDR(env))) << 16;
            if ((instr & 0x10000000) == 0) {
                instr |= cpu_lduw_code(env, PC_ADDR(env) + 2);
            }
            env->CP0_BadInstr = instr;

            if ((instr & 0xFC000000) == 0x60000000) {
                instr = cpu_lduw_code(env, PC_ADDR(env) + 4) << 16;
                env->CP0_BadInstrX = instr;
            }
        }
        return;
    }

    if (env->hflags & MIPS_HFLAG_M16) {
        /* TODO: add BadInstr support for microMIPS */
        return;
    }
    if (env->CP0_Config3 & (1 << CP0C3_BI)) {
        env->CP0_BadInstr = cpu_ldl_code(env, PC_ADDR(env));
    }
    if ((env->CP0_Config3 & (1 << CP0C3_BP)) &&
        (env->hflags & MIPS_HFLAG_BMASK)) {
        env->CP0_BadInstrP = cpu_ldl_code(env, PC_ADDR(env) - 4);
    }
}

#endif /* !CONFIG_USER_ONLY */

static inline void mips_update_pc_for_exc_handler(CPUMIPSState *env,
                                                  target_ulong new_pc)
{
#ifdef TARGET_CHERI
    /* always set PCC from KCC even with EXL */
    cheri_update_pcc_for_exc_handler(&env->active_tc.PCC,
                                     &env->active_tc.CHWR.KCC, new_pc);
#else
    mips_update_pc(env, new_pc, /*can_be_unrep=*/true);
#endif
}

void mips_cpu_do_interrupt(CPUState *cs)
{
#if !defined(CONFIG_USER_ONLY)
    MIPSCPU *cpu = MIPS_CPU(cs);
    CPUMIPSState *env = &cpu->env;
    tcg_debug_assert(pc_is_current(env));
    bool update_badinstr = 0;
    target_ulong offset;
    int cause = -1;

    /* Log interrupt extra debug info */
    if (qemu_log_instr_or_mask_enabled(env, CPU_LOG_INT) &&
        cs->exception_index != EXCP_EXT_INTERRUPT) {
        qemu_log_instr_or_mask_msg(
            env, CPU_LOG_INT,
            "%s enter: PC " TARGET_FMT_lx " EPC " TARGET_FMT_lx
            " %s exception, (hflags & MIPS_HFLAG_BMASK)=%x, hflags=%x\n",
            __func__, PC_ADDR(env), get_CP0_EPC(env),
            mips_exception_name(cs->exception_index),
            env->hflags & MIPS_HFLAG_BMASK, env->hflags);
#ifdef TARGET_CHERI
        qemu_log_instr_or_mask_msg(env, CPU_LOG_INT,
            "\tPCC=" PRINT_CAP_FMTSTR "\n\tKCC= " PRINT_CAP_FMTSTR
            "\n\tEPCC=" PRINT_CAP_FMTSTR "\n",
            PRINT_CAP_ARGS(cheri_get_current_pcc(env)),
            PRINT_CAP_ARGS(&env->active_tc.CHWR.KCC),
            PRINT_CAP_ARGS(&env->active_tc.CHWR.EPCC));
#endif
    }

    if (cs->exception_index == EXCP_EXT_INTERRUPT &&
        (env->hflags & MIPS_HFLAG_DM)) {
        cs->exception_index = EXCP_DINT;
    }
    offset = 0x180;
    switch (cs->exception_index) {
    case EXCP_DSS:
        env->CP0_Debug |= 1 << CP0DB_DSS;
        /*
         * Debug single step cannot be raised inside a delay slot and
         * resume will always occur on the next instruction
         * (but we assume the pc has always been updated during
         * code translation).
         */
        env->CP0_DEPC = PC_ADDR(env) | !!(env->hflags & MIPS_HFLAG_M16);
        goto enter_debug_mode;
    case EXCP_DINT:
        env->CP0_Debug |= 1 << CP0DB_DINT;
        goto set_DEPC;
    case EXCP_DIB:
        env->CP0_Debug |= 1 << CP0DB_DIB;
        goto set_DEPC;
    case EXCP_DBp:
        env->CP0_Debug |= 1 << CP0DB_DBp;
        /* Setup DExcCode - SDBBP instruction */
        env->CP0_Debug = (env->CP0_Debug & ~(0x1fULL << CP0DB_DEC)) |
                         (9 << CP0DB_DEC);
        goto set_DEPC;
    case EXCP_DDBS:
        env->CP0_Debug |= 1 << CP0DB_DDBS;
        goto set_DEPC;
    case EXCP_DDBL:
        env->CP0_Debug |= 1 << CP0DB_DDBL;
    set_DEPC:
        env->CP0_DEPC = exception_resume_pc(env);
        env->hflags &= ~MIPS_HFLAG_BMASK;
 enter_debug_mode:
        if (env->insn_flags & ISA_MIPS3) {
            env->hflags |= MIPS_HFLAG_64;
            if (!(env->insn_flags & ISA_MIPS_R6) ||
                env->CP0_Status & (1 << CP0St_KX)) {
                env->hflags &= ~MIPS_HFLAG_AWRAP;
            }
        }
        env->hflags |= MIPS_HFLAG_DM | MIPS_HFLAG_CP0;
        env->hflags &= ~(MIPS_HFLAG_KSU);
        /* EJTAG probe trap enable is not implemented... */
        if (!(env->CP0_Status & (1 << CP0St_EXL))) {
            env->CP0_Cause &= ~(1U << CP0Ca_BD);
        }
        mips_update_pc_for_exc_handler(env, env->exception_base + 0x480);
        set_hflags_for_handler(env);
        break;
    case EXCP_RESET:
        cpu_reset(CPU(cpu));
        break;
    case EXCP_SRESET:
        env->CP0_Status |= (1 << CP0St_SR);
        memset(env->CP0_WatchLo, 0, sizeof(env->CP0_WatchLo));
        goto set_error_EPC;
    case EXCP_NMI:
        env->CP0_Status |= (1 << CP0St_NMI);
 set_error_EPC:
#ifdef TARGET_CHERI
        env->active_tc.CHWR.ErrorEPCC = *cheri_get_current_pcc(env);
        // Note: set_CP0_ErrorEPC() handles the special cases of sealed/unrep EPCC
#endif /* TARGET_CHERI */
        set_CP0_ErrorEPC(env, exception_resume_pc(env));
        env->hflags &= ~MIPS_HFLAG_BMASK;
        env->CP0_Status |= (1 << CP0St_ERL) | (1 << CP0St_BEV);
        if (env->insn_flags & ISA_MIPS3) {
            env->hflags |= MIPS_HFLAG_64;
            if (!(env->insn_flags & ISA_MIPS_R6) ||
                env->CP0_Status & (1 << CP0St_KX)) {
                env->hflags &= ~MIPS_HFLAG_AWRAP;
            }
        }
        env->hflags |= MIPS_HFLAG_CP0;
        env->hflags &= ~(MIPS_HFLAG_KSU);
        if (!(env->CP0_Status & (1 << CP0St_EXL))) {
            env->CP0_Cause &= ~(1U << CP0Ca_BD);
        }
        mips_update_pc_for_exc_handler(env, env->exception_base);
        set_hflags_for_handler(env);
        break;
    case EXCP_EXT_INTERRUPT:
        cause = 0;
        if (env->CP0_Cause & (1 << CP0Ca_IV)) {
            uint32_t spacing = (env->CP0_IntCtl >> CP0IntCtl_VS) & 0x1f;

            if ((env->CP0_Status & (1 << CP0St_BEV)) || spacing == 0) {
                offset = 0x200;
            } else {
                uint32_t vector = 0;
                uint32_t pending = (env->CP0_Cause & CP0Ca_IP_mask) >> CP0Ca_IP;

                if (env->CP0_Config3 & (1 << CP0C3_VEIC)) {
                    /*
                     * For VEIC mode, the external interrupt controller feeds
                     * the vector through the CP0Cause IP lines.
                     */
                    vector = pending;
                } else {
                    /*
                     * Vectored Interrupts
                     * Mask with Status.IM7-IM0 to get enabled interrupts.
                     */
                    pending &= (env->CP0_Status >> CP0St_IM) & 0xff;
                    /* Find the highest-priority interrupt. */
                    while (pending >>= 1) {
                        vector++;
                    }
                }
                offset = 0x200 + (vector * (spacing << 5));
            }
        }
        goto set_EPC;
    case EXCP_LTLBL:
        cause = 1;
        update_badinstr = !(env->error_code & EXCP_INST_NOTAVAIL);
        goto set_EPC;
    case EXCP_TLBL:
        cause = 2;
        update_badinstr = !(env->error_code & EXCP_INST_NOTAVAIL);
        if ((env->error_code & EXCP_TLB_NOMATCH) &&
            !(env->CP0_Status & (1 << CP0St_EXL))) {
#if defined(TARGET_MIPS64)
            int R = env->CP0_BadVAddr >> 62;
            int UX = (env->CP0_Status & (1 << CP0St_UX)) != 0;
            int KX = (env->CP0_Status & (1 << CP0St_KX)) != 0;

            if ((R != 0 || UX) && (R != 3 || KX) &&
                (!(env->insn_flags & (INSN_LOONGSON2E | INSN_LOONGSON2F)))) {
                offset = 0x080;
            } else {
#endif
                offset = 0x000;
#if defined(TARGET_MIPS64)
            }
#endif
        }
        goto set_EPC;
    case EXCP_TLBS:
        cause = 3;
        update_badinstr = 1;
        if ((env->error_code & EXCP_TLB_NOMATCH) &&
            !(env->CP0_Status & (1 << CP0St_EXL))) {
#if defined(TARGET_MIPS64)
            int R = env->CP0_BadVAddr >> 62;
            int UX = (env->CP0_Status & (1 << CP0St_UX)) != 0;
            int KX = (env->CP0_Status & (1 << CP0St_KX)) != 0;

            if ((R != 0 || UX) && (R != 3 || KX) &&
                (!(env->insn_flags & (INSN_LOONGSON2E | INSN_LOONGSON2F)))) {
                offset = 0x080;
            } else {
#endif
                offset = 0x000;
#if defined(TARGET_MIPS64)
            }
#endif
        }
        goto set_EPC;
    case EXCP_AdEL:
        cause = 4;
        update_badinstr = !(env->error_code & EXCP_INST_NOTAVAIL);
        goto set_EPC;
    case EXCP_AdES:
        cause = 5;
        update_badinstr = 1;
        goto set_EPC;
    case EXCP_IBE:
        update_badinstr = 0;
        cause = 6;
        goto set_EPC;
    case EXCP_DBE:
        cause = 7;
        goto set_EPC;
    case EXCP_SYSCALL:
        cause = 8;
        update_badinstr = 1;
        goto set_EPC;
    case EXCP_BREAK:
        cause = 9;
        update_badinstr = 1;
        goto set_EPC;
    case EXCP_RI:
        cause = 10;
        update_badinstr = 1;
        goto set_EPC;
    case EXCP_CpU:
        cause = 11;
        update_badinstr = 1;
        env->CP0_Cause = (env->CP0_Cause & ~(0x3 << CP0Ca_CE)) |
                         (env->error_code << CP0Ca_CE);
        goto set_EPC;
    case EXCP_OVERFLOW:
        cause = 12;
        update_badinstr = 1;
        goto set_EPC;
    case EXCP_TRAP:
        cause = 13;
        update_badinstr = 1;
        goto set_EPC;
    case EXCP_MSAFPE:
        cause = 14;
        update_badinstr = 1;
        goto set_EPC;
    case EXCP_FPE:
        cause = 15;
        update_badinstr = 1;
        goto set_EPC;
    case EXCP_C2E:
        cause = 18;
        update_badinstr = !(env->error_code & EXCP_INST_NOTAVAIL);
#ifdef TARGET_CHERI
        if ((env->CP2_CapCause >> 8) == CapEx_CallTrap ||
                (env->CP2_CapCause >> 8) == CapEx_ReturnTrap)
            offset = 0x280;
#endif /* TARGET_CHERI */
        goto set_EPC;
    case EXCP_TLBRI:
        cause = 19;
        update_badinstr = 1;
        goto set_EPC;
    case EXCP_TLBXI:
        cause = 20;
        goto set_EPC;
    case EXCP_MSADIS:
        cause = 21;
        update_badinstr = 1;
        goto set_EPC;
    case EXCP_MDMX:
        cause = 22;
        goto set_EPC;
    case EXCP_DWATCH:
        cause = 23;
        /* XXX: TODO: manage deferred watch exceptions */
        goto set_EPC;
    case EXCP_MCHECK:
        cause = 24;
        goto set_EPC;
    case EXCP_THREAD:
        cause = 25;
        goto set_EPC;
    case EXCP_DSPDIS:
        cause = 26;
        goto set_EPC;
    case EXCP_CACHE:
        cause = 30;
        offset = 0x100;
 set_EPC:
        if (!(env->CP0_Status & (1 << CP0St_EXL))) {
#ifdef TARGET_CHERI
            env->active_tc.CHWR.EPCC = *cheri_get_current_pcc(env);
            // Note: set_CP0_EPC() handles the special cases of sealed/unrep EPCC
#endif /* TARGET_CHERI */
            set_CP0_EPC(env, exception_resume_pc(env));
            if (update_badinstr) {
                set_badinstr_registers(env);
            }
            if (env->hflags & MIPS_HFLAG_BMASK) {
                env->CP0_Cause |= (1U << CP0Ca_BD);
            } else {
                env->CP0_Cause &= ~(1U << CP0Ca_BD);
            }
            env->CP0_Status |= (1 << CP0St_EXL);
            if (env->insn_flags & ISA_MIPS3) {
                env->hflags |= MIPS_HFLAG_64;
                if (!(env->insn_flags & ISA_MIPS_R6) ||
                    env->CP0_Status & (1 << CP0St_KX)) {
                    env->hflags &= ~MIPS_HFLAG_AWRAP;
                }
            }
            env->hflags |= MIPS_HFLAG_CP0;
            env->hflags &= ~(MIPS_HFLAG_KSU);
        }
        env->hflags &= ~MIPS_HFLAG_BMASK;
        target_ulong new_pc;
        if (env->CP0_Status & (1 << CP0St_BEV)) {
            new_pc = env->exception_base + 0x200;
        } else if (cause == 30 && !(env->CP0_Config3 & (1 << CP0C3_SC) &&
                                    env->CP0_Config5 & (1 << CP0C5_CV))) {
            /* Force KSeg1 for cache errors */
            new_pc = KSEG1_BASE | (env->CP0_EBase & 0x1FFFF000);
        } else {
            new_pc = env->CP0_EBase & ~0xfff;
        }
        new_pc += offset;
        mips_update_pc_for_exc_handler(env, new_pc);
        set_hflags_for_handler(env);
        env->CP0_Cause = (env->CP0_Cause & ~(0x1f << CP0Ca_EC)) |
                         (cause << CP0Ca_EC);
        break;
    default:
        abort();
    }

#ifdef TARGET_CHERI
    // We may have to change the CP0 access flag since CHERI may have previously
    // disabled it by installing a $pcc without the Access_Sys_Regs flag
    update_cp0_access_for_pc(env);
    assert(can_access_cp0(env) && "Installing $pcc without ASR in exception?");
    assert(!cheri_get_current_pcc(env)->cr_tag ||
           cap_is_representable(cheri_get_current_pcc(env)));
#endif /* TARGET_CHERI */

#ifdef CONFIG_TCG_LOG_INSTR
    if (qemu_log_instr_enabled(env)) {
        uint32_t log_cause;
        /* Log generic exception/interrupt info */
        if (cs->exception_index == EXCP_EXT_INTERRUPT) {
            log_cause = (env->CP0_Cause & CP0Ca_IP_mask) >> CP0Ca_IP;
            qemu_log_instr_interrupt(env, log_cause, PC_ADDR(env));
        } else {
            /*
             * Note on CHERI-MIPS the logged cause is the concatenation
             * {cap_cause | cause}.
             */
#ifdef TARGET_CHERI
            log_cause = cause | (((uint32_t)env->CP2_CapCause) << 8);
#else
            log_cause = cause;
#endif
            qemu_log_instr_exception(env, log_cause, PC_ADDR(env),
                                     env->CP0_BadVAddr);
        }
#ifdef TARGET_CHERI
        /* Log extra changed register information */
        qemu_log_instr_cap(env, "PCC", cheri_get_current_pcc(env));
        qemu_log_instr_cap(env, "EPCC", &env->active_tc.CHWR.EPCC);
        qemu_log_instr_cap(env, "ErrorEPCC", &env->active_tc.CHWR.ErrorEPCC);
#endif
        qemu_log_instr_reg(env, "ErrorEPC", get_CP0_ErrorEPC(env));
    }
#endif /* CONFIG_TCG_LOG_INSTR */

    // TODO(am2419): log these as extra changed registers?
    if (qemu_log_instr_or_mask_enabled(env, CPU_LOG_INT) &&
        cs->exception_index != EXCP_EXT_INTERRUPT) {
        qemu_log_instr_or_mask_msg(env, CPU_LOG_INT,
            "%s: PC " TARGET_FMT_lx " EPC " TARGET_FMT_lx " cause %d\n"
            "    S %08x C %08x A " TARGET_FMT_lx " D " TARGET_FMT_lx "\n",
            __func__, PC_ADDR(env), get_CP0_EPC(env), cause,
            env->CP0_Status, env->CP0_Cause, env->CP0_BadVAddr,
            env->CP0_DEPC);
    }
#endif /* ! CONFIG_USER_ONLY */
    cs->exception_index = EXCP_NONE;

#ifdef CONFIG_TCG_LOG_INSTR
    mips_log_instr_mode_changed(env, cpu_get_recent_pc(env));
#endif
}

#if !defined(CONFIG_USER_ONLY)
bool r4k_lookup_tlb(CPUMIPSState *env, int *matching, bool use_extra)
{
    bool mi = !!((env->CP0_Config5 >> CP0C5_MI) & 1);
    r4k_tlb_t *tlb;
    target_ulong mask;
    target_ulong tag;
    target_ulong VPN;
    uint16_t ASID = env->CP0_EntryHi & env->CP0_EntryHi_ASID_mask;
    uint32_t MMID = env->CP0_MemoryMapID;
    uint32_t tlb_mmid;
    int i;
    int limit = (use_extra) ? env->tlb->tlb_in_use : env->tlb->nb_tlb;

    MMID = mi ? MMID : (uint32_t) ASID;
    for (i = 0; i < limit; i++) {
        tlb = &env->tlb->mmu.r4k.tlb[i];
        /* 1k pages are not supported. */
        mask = tlb->PageMask | ~(TARGET_PAGE_MASK << 1);
        tag = env->CP0_EntryHi & ~mask;
        VPN = tlb->VPN & ~mask;
#if defined(TARGET_MIPS64)
        tag &= env->SEGMask;
#endif
        tlb_mmid = mi ? tlb->MMID : (uint32_t) tlb->ASID;
        /* Check ASID/MMID, virtual page number & size */
        if ((tlb->G == 1 || tlb_mmid == MMID) && VPN == tag && !tlb->EHINV) {
            if (matching)
                *matching = i;
            return true;
        }
    }

    return false;
}

void r4k_invalidate_tlb(CPUMIPSState *env, int idx, int use_extra)
{
    CPUState *cs = env_cpu(env);
    r4k_tlb_t *tlb;
    target_ulong addr;
    target_ulong end;
    uint16_t ASID = env->CP0_EntryHi & env->CP0_EntryHi_ASID_mask;
    uint32_t MMID = env->CP0_MemoryMapID;
    bool mi = !!((env->CP0_Config5 >> CP0C5_MI) & 1);
    uint32_t tlb_mmid;
    target_ulong mask;

    MMID = mi ? MMID : (uint32_t) ASID;

    tlb = &env->tlb->mmu.r4k.tlb[idx];
    /*
     * The qemu TLB is flushed when the ASID/MMID changes, so no need to
     * flush these entries again.
     */
    tlb_mmid = mi ? tlb->MMID : (uint32_t) tlb->ASID;
    if (tlb->G == 0 && tlb_mmid != MMID) {
        return;
    }

    if (use_extra && env->tlb->tlb_in_use < MIPS_TLB_MAX) {
        /*
         * For tlbwr, we can shadow the discarded entry into
         * a new (fake) TLB entry, as long as the guest can not
         * tell that it's there.
         */
        env->tlb->mmu.r4k.tlb[env->tlb->tlb_in_use] = *tlb;
        env->tlb->tlb_in_use++;
        return;
    }

    /* 1k pages are not supported. */
    mask = tlb->PageMask | ~(TARGET_PAGE_MASK << 1);
    if (tlb->V0) {
        addr = tlb->VPN & ~mask;
#if defined(TARGET_MIPS64)
        if (addr >= (0xFFFFFFFF80000000ULL & env->SEGMask)) {
            addr |= 0x3FFFFF0000000000ULL;
        }
#endif
        end = addr | (mask >> 1);
        while (addr < end) {
            tlb_flush_page(cs, addr);
            addr += TARGET_PAGE_SIZE;
        }
    }
    if (tlb->V1) {
        addr = (tlb->VPN & ~mask) | ((mask >> 1) + 1);
#if defined(TARGET_MIPS64)
        if (addr >= (0xFFFFFFFF80000000ULL & env->SEGMask)) {
            addr |= 0x3FFFFF0000000000ULL;
        }
#endif
        end = addr | mask;
        while (addr - 1 < end) {
            tlb_flush_page(cs, addr);
            addr += TARGET_PAGE_SIZE;
        }
    }
}
#endif /* !CONFIG_USER_ONLY */
