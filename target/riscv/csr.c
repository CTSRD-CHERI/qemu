/*
 * RISC-V Control and Status Registers.
 *
 * Copyright (c) 2016-2017 Sagar Karandikar, sagark@eecs.berkeley.edu
 * Copyright (c) 2017-2018 SiFive, Inc.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2 or later, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "qemu/osdep.h"
#include "qemu/log.h"
#include "cpu.h"
#include "qemu/main-loop.h"
#include "exec/exec-all.h"
#include "exec/log_instr.h"
#ifdef TARGET_CHERI
#include "cheri-helper-utils.h"
#endif

/* CSR update logging API */
#if CONFIG_TCG_LOG_INSTR
void riscv_log_instr_csr_changed(CPURISCVState *env, int csrno)
{
    target_ulong value;

    if (qemu_log_instr_enabled(env)) {
#ifdef TARGET_CHERI
        /* Handle extended/added capability registers as well */
        riscv_csr_cap_ops *cap_ops = get_csr_cap_info(csrno);
        if (cap_ops) {
            cap_register_t cap_value = cap_ops->read(env, cap_ops);
            qemu_log_instr_cap(env, cap_ops->name, &cap_value, csrno,
                               LRI_CSR_ACCESS);
            return;
        }
#endif

        if (csr_ops[csrno].read)
            csr_ops[csrno].read(env, csrno, &value);
        else if (csr_ops[csrno].op)
            csr_ops[csrno].op(env, csrno, &value, 0, /*write_mask*/0);
        else
            return;
        if (csr_ops[csrno].log_update)
            csr_ops[csrno].log_update(env, csrno, value);
    }
}
#endif

/* CSR function table public API */
void riscv_get_csr_ops(int csrno, riscv_csr_operations *ops)
{
    *ops = csr_ops[csrno & (CSR_TABLE_SIZE - 1)];
}

void riscv_set_csr_ops(int csrno, riscv_csr_operations *ops)
{
    csr_ops[csrno & (CSR_TABLE_SIZE - 1)] = *ops;
}

/* Predicates */
static RISCVException fs(CPURISCVState *env, int csrno)
{
#if !defined(CONFIG_USER_ONLY)
    /* loose check condition for fcsr in vector extension */
    if ((csrno == CSR_FCSR) && (env->misa_ext & RVV)) {
        return RISCV_EXCP_NONE;
    }
    if (!env->debugger && !riscv_cpu_fp_enabled(env)) {
        return RISCV_EXCP_ILLEGAL_INST;
    }
#endif
    return RISCV_EXCP_NONE;
}

static RISCVException vs(CPURISCVState *env, int csrno)
{
    if (env->misa_ext & RVV) {
        return RISCV_EXCP_NONE;
    }
    return RISCV_EXCP_ILLEGAL_INST;
}

static RISCVException ctr(CPURISCVState *env, int csrno)
{
#if !defined(CONFIG_USER_ONLY)
    CPUState *cs = env_cpu(env);
    RISCVCPU *cpu = RISCV_CPU(cs);

    if (!cpu->cfg.ext_counters) {
        /* The Counters extensions is not enabled */
        return RISCV_EXCP_ILLEGAL_INST;
    }

    if (riscv_cpu_virt_enabled(env)) {
        switch (csrno) {
        case CSR_CYCLE:
            if (!get_field(env->hcounteren, COUNTEREN_CY) &&
                get_field(env->mcounteren, COUNTEREN_CY)) {
                return RISCV_EXCP_VIRT_INSTRUCTION_FAULT;
            }
            break;
        case CSR_TIME:
            if (!get_field(env->hcounteren, COUNTEREN_TM) &&
                get_field(env->mcounteren, COUNTEREN_TM)) {
                return RISCV_EXCP_VIRT_INSTRUCTION_FAULT;
            }
            break;
        case CSR_INSTRET:
            if (!get_field(env->hcounteren, COUNTEREN_IR) &&
                get_field(env->mcounteren, COUNTEREN_IR)) {
                return RISCV_EXCP_VIRT_INSTRUCTION_FAULT;
            }
            break;
        case CSR_HPMCOUNTER3...CSR_HPMCOUNTER31:
            if (!get_field(env->hcounteren, 1 << (csrno - CSR_HPMCOUNTER3)) &&
                get_field(env->mcounteren, 1 << (csrno - CSR_HPMCOUNTER3))) {
                return RISCV_EXCP_VIRT_INSTRUCTION_FAULT;
            }
            break;
        }
        if (riscv_cpu_mxl(env) == MXL_RV32) {
            switch (csrno) {
            case CSR_CYCLEH:
                if (!get_field(env->hcounteren, COUNTEREN_CY) &&
                    get_field(env->mcounteren, COUNTEREN_CY)) {
                    return RISCV_EXCP_VIRT_INSTRUCTION_FAULT;
                }
                break;
            case CSR_TIMEH:
                if (!get_field(env->hcounteren, COUNTEREN_TM) &&
                    get_field(env->mcounteren, COUNTEREN_TM)) {
                    return RISCV_EXCP_VIRT_INSTRUCTION_FAULT;
                }
                break;
            case CSR_INSTRETH:
                if (!get_field(env->hcounteren, COUNTEREN_IR) &&
                    get_field(env->mcounteren, COUNTEREN_IR)) {
                    return RISCV_EXCP_VIRT_INSTRUCTION_FAULT;
                }
                break;
            case CSR_HPMCOUNTER3H...CSR_HPMCOUNTER31H:
                if (!get_field(env->hcounteren, 1 << (csrno - CSR_HPMCOUNTER3H)) &&
                    get_field(env->mcounteren, 1 << (csrno - CSR_HPMCOUNTER3H))) {
                    return RISCV_EXCP_VIRT_INSTRUCTION_FAULT;
                }
                break;
            }
        }
    }
#endif
    return RISCV_EXCP_NONE;
}

static RISCVException ctr32(CPURISCVState *env, int csrno)
{
    if (riscv_cpu_mxl(env) != MXL_RV32) {
        return RISCV_EXCP_ILLEGAL_INST;
    }

    return ctr(env, csrno);
}

#if !defined(CONFIG_USER_ONLY)
static RISCVException any(CPURISCVState *env, int csrno)
{
    return RISCV_EXCP_NONE;
}

static RISCVException any32(CPURISCVState *env, int csrno)
{
    if (riscv_cpu_mxl(env) != MXL_RV32) {
        return RISCV_EXCP_ILLEGAL_INST;
    }

    return any(env, csrno);

}

#ifdef TARGET_CHERI_RISCV_V9
static RISCVException umode(CPURISCVState *env, int csrno)
{
    if (riscv_has_ext(env, RVU)) {
        return RISCV_EXCP_NONE;
    }

    return RISCV_EXCP_ILLEGAL_INST;
}
#endif

static RISCVException smode(CPURISCVState *env, int csrno)
{
    if (riscv_has_ext(env, RVS)) {
        return RISCV_EXCP_NONE;
    }

    return RISCV_EXCP_ILLEGAL_INST;
}

static RISCVException hmode(CPURISCVState *env, int csrno)
{
    if (riscv_has_ext(env, RVS) &&
        riscv_has_ext(env, RVH)) {
        /* Hypervisor extension is supported */
        if ((env->priv == PRV_S && !riscv_cpu_virt_enabled(env)) ||
            env->priv == PRV_M) {
            return RISCV_EXCP_NONE;
        } else {
            return RISCV_EXCP_VIRT_INSTRUCTION_FAULT;
        }
    }

    return RISCV_EXCP_ILLEGAL_INST;
}

static RISCVException hmode32(CPURISCVState *env, int csrno)
{
    if (riscv_cpu_mxl(env) != MXL_RV32) {
        if (riscv_cpu_virt_enabled(env)) {
            return RISCV_EXCP_ILLEGAL_INST;
        } else {
            return RISCV_EXCP_VIRT_INSTRUCTION_FAULT;
        }
    }

    return hmode(env, csrno);

}

/* Checks if PointerMasking registers could be accessed */
static RISCVException pointer_masking(CPURISCVState *env, int csrno)
{
    /* Check if j-ext is present */
    if (riscv_has_ext(env, RVJ)) {
        return RISCV_EXCP_NONE;
    }
    return RISCV_EXCP_ILLEGAL_INST;
}

static RISCVException pmp(CPURISCVState *env, int csrno)
{
    if (riscv_feature(env, RISCV_FEATURE_PMP)) {
        return RISCV_EXCP_NONE;
    }

    return RISCV_EXCP_ILLEGAL_INST;
}

static RISCVException epmp(CPURISCVState *env, int csrno)
{
    if (env->priv == PRV_M && riscv_feature(env, RISCV_FEATURE_EPMP)) {
        return RISCV_EXCP_NONE;
    }

    return RISCV_EXCP_ILLEGAL_INST;
}
#endif

/* User Floating-Point CSRs */
static RISCVException read_fflags(CPURISCVState *env, int csrno,
                                  target_ulong *val)
{
    *val = riscv_cpu_get_fflags(env);
    return RISCV_EXCP_NONE;
}

static RISCVException write_fflags(CPURISCVState *env, int csrno,
                                   target_ulong val)
{
#if !defined(CONFIG_USER_ONLY)
    env->mstatus |= MSTATUS_FS;
#endif
    riscv_cpu_set_fflags(env, val & (FSR_AEXC >> FSR_AEXC_SHIFT));
    return RISCV_EXCP_NONE;
}

static RISCVException read_frm(CPURISCVState *env, int csrno,
                               target_ulong *val)
{
    *val = env->frm;
    return RISCV_EXCP_NONE;
}

static RISCVException write_frm(CPURISCVState *env, int csrno,
                                target_ulong val)
{
#if !defined(CONFIG_USER_ONLY)
    env->mstatus |= MSTATUS_FS;
#endif
    env->frm = val & (FSR_RD >> FSR_RD_SHIFT);
    return RISCV_EXCP_NONE;
}

static RISCVException read_fcsr(CPURISCVState *env, int csrno,
                                target_ulong *val)
{
    *val = (riscv_cpu_get_fflags(env) << FSR_AEXC_SHIFT)
        | (env->frm << FSR_RD_SHIFT);
    if (vs(env, csrno) >= 0) {
        *val |= (env->vxrm << FSR_VXRM_SHIFT)
                | (env->vxsat << FSR_VXSAT_SHIFT);
    }
    return RISCV_EXCP_NONE;
}

static RISCVException write_fcsr(CPURISCVState *env, int csrno,
                                 target_ulong val)
{
#if !defined(CONFIG_USER_ONLY)
    env->mstatus |= MSTATUS_FS;
    log_changed_special_reg(env, "mstatus", env->mstatus, CSR_MSTATUS,
                            LRI_CSR_ACCESS);
#endif
    env->frm = (val & FSR_RD) >> FSR_RD_SHIFT;
    if (vs(env, csrno) >= 0) {
        env->vxrm = (val & FSR_VXRM) >> FSR_VXRM_SHIFT;
        env->vxsat = (val & FSR_VXSAT) >> FSR_VXSAT_SHIFT;
    }
    riscv_cpu_set_fflags(env, (val & FSR_AEXC) >> FSR_AEXC_SHIFT);
    return RISCV_EXCP_NONE;
}

static RISCVException read_vtype(CPURISCVState *env, int csrno,
                                 target_ulong *val)
{
    *val = env->vtype;
    return RISCV_EXCP_NONE;
}

static RISCVException read_vl(CPURISCVState *env, int csrno,
                              target_ulong *val)
{
    *val = env->vl;
    return RISCV_EXCP_NONE;
}

static RISCVException read_vxrm(CPURISCVState *env, int csrno,
                                target_ulong *val)
{
    *val = env->vxrm;
    return RISCV_EXCP_NONE;
}

static RISCVException write_vxrm(CPURISCVState *env, int csrno,
                                 target_ulong val)
{
    env->vxrm = val;
    return RISCV_EXCP_NONE;
}

static RISCVException read_vxsat(CPURISCVState *env, int csrno,
                                 target_ulong *val)
{
    *val = env->vxsat;
    return RISCV_EXCP_NONE;
}

static RISCVException write_vxsat(CPURISCVState *env, int csrno,
                                  target_ulong val)
{
    env->vxsat = val;
    return RISCV_EXCP_NONE;
}

static RISCVException read_vstart(CPURISCVState *env, int csrno,
                                  target_ulong *val)
{
    *val = env->vstart;
    return RISCV_EXCP_NONE;
}

static RISCVException write_vstart(CPURISCVState *env, int csrno,
                                   target_ulong val)
{
    env->vstart = val;
    return RISCV_EXCP_NONE;
}

/* User Timers and Counters */
static RISCVException read_instret(CPURISCVState *env, int csrno,
                                   target_ulong *val)
{
#if !defined(CONFIG_USER_ONLY)
    if (icount_enabled()) {
        *val = icount_get();
    } else {
        *val = cpu_get_host_ticks();
    }
#else
    *val = cpu_get_host_ticks();
#endif
    return RISCV_EXCP_NONE;
}

static RISCVException read_instreth(CPURISCVState *env, int csrno,
                                    target_ulong *val)
{
#if !defined(CONFIG_USER_ONLY)
    if (icount_enabled()) {
        *val = icount_get() >> 32;
    } else {
        *val = cpu_get_host_ticks() >> 32;
    }
#else
    *val = cpu_get_host_ticks() >> 32;
#endif
    return RISCV_EXCP_NONE;
}

#if defined(CONFIG_USER_ONLY)
static RISCVException read_time(CPURISCVState *env, int csrno,
                                target_ulong *val)
{
    *val = cpu_get_host_ticks();
    return RISCV_EXCP_NONE;
}

static RISCVException read_timeh(CPURISCVState *env, int csrno,
                                 target_ulong *val)
{
    *val = cpu_get_host_ticks() >> 32;
    return RISCV_EXCP_NONE;
}

#else /* CONFIG_USER_ONLY */

static RISCVException read_time(CPURISCVState *env, int csrno,
                                target_ulong *val)
{
    uint64_t delta = riscv_cpu_virt_enabled(env) ? env->htimedelta : 0;

    if (!env->rdtime_fn) {
        return RISCV_EXCP_ILLEGAL_INST;
    }

    *val = env->rdtime_fn(env->rdtime_fn_arg) + delta;
    return RISCV_EXCP_NONE;
}

static RISCVException read_timeh(CPURISCVState *env, int csrno,
                                 target_ulong *val)
{
    uint64_t delta = riscv_cpu_virt_enabled(env) ? env->htimedelta : 0;

    if (!env->rdtime_fn) {
        return RISCV_EXCP_ILLEGAL_INST;
    }

    *val = (env->rdtime_fn(env->rdtime_fn_arg) + delta) >> 32;
    return RISCV_EXCP_NONE;
}

/* Machine constants */

#define M_MODE_INTERRUPTS  (MIP_MSIP | MIP_MTIP | MIP_MEIP)
#define S_MODE_INTERRUPTS  (MIP_SSIP | MIP_STIP | MIP_SEIP)
#define VS_MODE_INTERRUPTS (MIP_VSSIP | MIP_VSTIP | MIP_VSEIP)

static const target_ulong delegable_ints = S_MODE_INTERRUPTS |
                                           VS_MODE_INTERRUPTS;
static const target_ulong vs_delegable_ints = VS_MODE_INTERRUPTS;
static const target_ulong all_ints = M_MODE_INTERRUPTS | S_MODE_INTERRUPTS |
                                     VS_MODE_INTERRUPTS;

#ifdef TARGET_CHERI
#if !defined(TARGET_RISCV32) && !defined(TARGET_CHERI_RISCV_STD_093)
#define CHERI_DELEGABLE_EXCPS ( \
        (1ULL << (RISCV_EXCP_LOAD_CAP_PAGE_FAULT)) | \
        (1ULL << (RISCV_EXCP_STORE_AMO_CAP_PAGE_FAULT)) | \
        (1ULL << (RISCV_EXCP_CHERI)))
#else
#define CHERI_DELEGABLE_EXCPS (1ULL << (RISCV_EXCP_CHERI))
#endif
#else
#define CHERI_DELEGABLE_EXCPS 0
#endif
#define DELEGABLE_EXCPS ((1ULL << (RISCV_EXCP_INST_ADDR_MIS)) | \
                         (1ULL << (RISCV_EXCP_INST_ACCESS_FAULT)) | \
                         (1ULL << (RISCV_EXCP_ILLEGAL_INST)) | \
                         (1ULL << (RISCV_EXCP_BREAKPOINT)) | \
                         (1ULL << (RISCV_EXCP_LOAD_ADDR_MIS)) | \
                         (1ULL << (RISCV_EXCP_LOAD_ACCESS_FAULT)) | \
                         (1ULL << (RISCV_EXCP_STORE_AMO_ADDR_MIS)) | \
                         (1ULL << (RISCV_EXCP_STORE_AMO_ACCESS_FAULT)) | \
                         (1ULL << (RISCV_EXCP_U_ECALL)) | \
                         (1ULL << (RISCV_EXCP_S_ECALL)) | \
                         (1ULL << (RISCV_EXCP_VS_ECALL)) | \
                         (1ULL << (RISCV_EXCP_M_ECALL)) | \
                         (1ULL << (RISCV_EXCP_INST_PAGE_FAULT)) | \
                         (1ULL << (RISCV_EXCP_LOAD_PAGE_FAULT)) | \
                         (1ULL << (RISCV_EXCP_STORE_PAGE_FAULT)) | \
                         (1ULL << (RISCV_EXCP_INST_GUEST_PAGE_FAULT)) | \
                         (1ULL << (RISCV_EXCP_LOAD_GUEST_ACCESS_FAULT)) | \
                         (1ULL << (RISCV_EXCP_VIRT_INSTRUCTION_FAULT)) | \
                         (1ULL << (RISCV_EXCP_STORE_GUEST_AMO_ACCESS_FAULT)) | \
                         (CHERI_DELEGABLE_EXCPS))
static const target_ulong vs_delegable_excps = DELEGABLE_EXCPS &
    ~((1ULL << (RISCV_EXCP_S_ECALL)) |
      (1ULL << (RISCV_EXCP_VS_ECALL)) |
      (1ULL << (RISCV_EXCP_M_ECALL)) |
      (1ULL << (RISCV_EXCP_INST_GUEST_PAGE_FAULT)) |
      (1ULL << (RISCV_EXCP_LOAD_GUEST_ACCESS_FAULT)) |
      (1ULL << (RISCV_EXCP_VIRT_INSTRUCTION_FAULT)) |
      (1ULL << (RISCV_EXCP_STORE_GUEST_AMO_ACCESS_FAULT)));
static const target_ulong sstatus_v1_10_mask = SSTATUS_SIE | SSTATUS_SPIE |
    SSTATUS_UIE | SSTATUS_UPIE | SSTATUS_SPP | SSTATUS_FS | SSTATUS_XS |
    SSTATUS_SUM | SSTATUS_MXR | (target_ulong)SSTATUS64_UXL;
static const target_ulong sip_writable_mask = SIP_SSIP | MIP_USIP | MIP_UEIP;
static const target_ulong hip_writable_mask = MIP_VSSIP;
static const target_ulong hvip_writable_mask = MIP_VSSIP | MIP_VSTIP | MIP_VSEIP;
static const target_ulong vsip_writable_mask = MIP_VSSIP;

static const char valid_vm_1_10_32[16] = {
    [VM_1_10_MBARE] = 1,
    [VM_1_10_SV32] = 1
};

static const char valid_vm_1_10_64[16] = {
    [VM_1_10_MBARE] = 1,
    [VM_1_10_SV39] = 1,
    [VM_1_10_SV48] = 1,
    [VM_1_10_SV57] = 1
};

/* Machine Information Registers */
static RISCVException read_zero(CPURISCVState *env, int csrno,
                                target_ulong *val)
{
    *val = 0;
    return RISCV_EXCP_NONE;
}

static RISCVException read_mhartid(CPURISCVState *env, int csrno,
                                   target_ulong *val)
{
    *val = env->mhartid;
    return RISCV_EXCP_NONE;
}

/* Machine Trap Setup */

/* We do not store SD explicitly, only compute it on demand. */
static uint64_t add_status_sd(RISCVMXL xl, uint64_t status)
{
    if ((status & MSTATUS_FS) == MSTATUS_FS ||
        (status & MSTATUS_XS) == MSTATUS_XS) {
        switch (xl) {
        case MXL_RV32:
            return status | MSTATUS32_SD;
        case MXL_RV64:
            return status | MSTATUS64_SD;
        default:
            g_assert_not_reached();
        }
    }
    return status;
}

static RISCVException read_mstatus(CPURISCVState *env, int csrno,
                                   target_ulong *val)
{
    *val = add_status_sd(riscv_cpu_mxl(env), env->mstatus);
    return RISCV_EXCP_NONE;
}

static int validate_vm(CPURISCVState *env, target_ulong vm)
{
    if (riscv_cpu_mxl(env) == MXL_RV32) {
        return valid_vm_1_10_32[vm & 0xf];
    } else {
        return valid_vm_1_10_64[vm & 0xf];
    }
}

static RISCVException write_mstatus(CPURISCVState *env, int csrno,
                                    target_ulong val)
{
    uint64_t mstatus = env->mstatus;
    uint64_t mask = 0;

    /* flush tlb on mstatus fields that affect VM */
    if ((val ^ mstatus) & (MSTATUS_MXR | MSTATUS_MPP | MSTATUS_MPV |
            MSTATUS_MPRV | MSTATUS_SUM)) {
        tlb_flush(env_cpu(env));
    }
    mask = MSTATUS_SIE | MSTATUS_SPIE | MSTATUS_MIE | MSTATUS_MPIE |
        MSTATUS_SPP | MSTATUS_FS | MSTATUS_MPRV | MSTATUS_SUM |
        MSTATUS_MPP | MSTATUS_MXR | MSTATUS_TVM | MSTATUS_TSR |
        MSTATUS_TW;

    if (riscv_cpu_mxl(env) != MXL_RV32) {
        /*
         * RV32: MPV and GVA are not in mstatus. The current plan is to
         * add them to mstatush. For now, we just don't support it.
         */
        if (riscv_has_ext(env, RVH)) {
            mask |= MSTATUS_MPV | MSTATUS_GVA;
        }
    }

    mstatus = (mstatus & ~mask) | (val & mask);

    if (riscv_cpu_mxl(env) == MXL_RV64) {
        /* SXL and UXL fields are for now read only */
        mstatus = set_field(mstatus, MSTATUS64_SXL, MXL_RV64);
        mstatus = set_field(mstatus, MSTATUS64_UXL, MXL_RV64);
    }
    env->mstatus = mstatus;

    return RISCV_EXCP_NONE;
}

static RISCVException read_mstatush(CPURISCVState *env, int csrno,
                                    target_ulong *val)
{
    *val = env->mstatus >> 32;
    return RISCV_EXCP_NONE;
}

static RISCVException write_mstatush(CPURISCVState *env, int csrno,
                                     target_ulong val)
{
    uint64_t valh = (uint64_t)val << 32;
    uint64_t mask = MSTATUS_MPV | MSTATUS_GVA;

    if ((valh ^ env->mstatus) & (MSTATUS_MPV)) {
        tlb_flush(env_cpu(env));
    }

    env->mstatus = (env->mstatus & ~mask) | (valh & mask);

    return RISCV_EXCP_NONE;
}

static RISCVException read_misa(CPURISCVState *env, int csrno,
                                target_ulong *val)
{
    target_ulong misa;

    switch (env->misa_mxl) {
    case MXL_RV32:
        misa = (target_ulong)MXL_RV32 << 30;
        break;
#ifdef TARGET_RISCV64
    case MXL_RV64:
        misa = (target_ulong)MXL_RV64 << 62;
        break;
#endif
    default:
        g_assert_not_reached();
    }

    *val = misa | env->misa_ext;
    return RISCV_EXCP_NONE;
}

static RISCVException write_misa(CPURISCVState *env, int csrno,
                                 target_ulong val)
{
    if (!riscv_feature(env, RISCV_FEATURE_MISA)) {
        /* drop write to misa */
        return RISCV_EXCP_NONE;
    }

    /*
     * XXXAR: this code is completely broken:
     * 1) you can only turn **on** misa.C if PC is not aligned to 4 bytes???
     * 2) They use GETPC() for this check! This is a QEMU internal program
     * counter (the current return address, so not even the TCG generated code
     * address since we could be multiple call stack levels down).
     *
     * Fortunately RISCV_FEATURE_MISA should never be enabled so we can't end
     * up here... If we ever do, abort() is the only safe way out!
     */
    abort();

    /* 'I' or 'E' must be present */
    if (!(val & (RVI | RVE))) {
        /* It is not, drop write to misa */
        return RISCV_EXCP_NONE;
    }

    /* 'E' excludes all other extensions */
    if (val & RVE) {
        /* when we support 'E' we can do "val = RVE;" however
         * for now we just drop writes if 'E' is present.
         */
        return RISCV_EXCP_NONE;
    }

    /*
     * misa.MXL writes are not supported by QEMU.
     * Drop writes to those bits.
     */

    /* Mask extensions that are not supported by this hart */
    val &= env->misa_ext_mask;

    /* Mask extensions that are not supported by QEMU */
    val &= (RVI | RVE | RVM | RVA | RVF | RVD | RVC | RVS | RVU);

    /* 'D' depends on 'F', so clear 'D' if 'F' is not present */
    if ((val & RVD) && !(val & RVF)) {
        val &= ~RVD;
    }

    /* Suppress 'C' if next instruction is not aligned
     * TODO: this should check next_pc
     */
    if ((val & RVC) && (GETPC() & ~3) != 0) {
        val &= ~RVC;
    }

    /* If nothing changed, do nothing. */
    if (val == env->misa_ext) {
        return RISCV_EXCP_NONE;
    }

    /* flush translation cache */
    tb_flush(env_cpu(env));
    env->misa_ext = val;
    return RISCV_EXCP_NONE;
}

static RISCVException read_medeleg(CPURISCVState *env, int csrno,
                                   target_ulong *val)
{
    *val = env->medeleg;
    return RISCV_EXCP_NONE;
}

static RISCVException write_medeleg(CPURISCVState *env, int csrno,
                                    target_ulong val)
{
    env->medeleg = (env->medeleg & ~DELEGABLE_EXCPS) | (val & DELEGABLE_EXCPS);
    return RISCV_EXCP_NONE;
}

static RISCVException read_mideleg(CPURISCVState *env, int csrno,
                                   target_ulong *val)
{
    *val = env->mideleg;
    return RISCV_EXCP_NONE;
}

static RISCVException write_mideleg(CPURISCVState *env, int csrno,
                                    target_ulong val)
{
    env->mideleg = (env->mideleg & ~delegable_ints) | (val & delegable_ints);
    if (riscv_has_ext(env, RVH)) {
        env->mideleg |= VS_MODE_INTERRUPTS;
    }
    return RISCV_EXCP_NONE;
}

static RISCVException read_mie(CPURISCVState *env, int csrno,
                               target_ulong *val)
{
    *val = env->mie;
    return RISCV_EXCP_NONE;
}

static RISCVException write_mie(CPURISCVState *env, int csrno,
                                target_ulong val)
{
    env->mie = (env->mie & ~all_ints) | (val & all_ints);
    return RISCV_EXCP_NONE;
}

static RISCVException read_mtvec(CPURISCVState *env, int csrno,
                                 target_ulong *val)
{
    *val = GET_SPECIAL_REG_ARCH(env, mtvec, mtvecc);
    return RISCV_EXCP_NONE;
}

static RISCVException write_mtvec(CPURISCVState *env, int csrno,
                                  target_ulong val)
{
    /* bits [1:0] encode mode; 0 = direct, 1 = vectored, 2 >= reserved */
    if ((val & 3) < 2) {
        SET_SPECIAL_REG(env, mtvec, mtvecc, val);
    } else {
        qemu_log_mask(LOG_UNIMP, "CSR_MTVEC: reserved mode not supported\n");
    }
    return RISCV_EXCP_NONE;
}

static RISCVException read_mcounteren(CPURISCVState *env, int csrno,
                                      target_ulong *val)
{
    *val = env->mcounteren;
    return RISCV_EXCP_NONE;
}

static RISCVException write_mcounteren(CPURISCVState *env, int csrno,
                                       target_ulong val)
{
    env->mcounteren = val;
    return RISCV_EXCP_NONE;
}

/* Machine Trap Handling */
static RISCVException read_mscratch(CPURISCVState *env, int csrno,
                                    target_ulong *val)
{
    *val = GET_SPECIAL_REG_ARCH(env, mscratch, mscratchc);
    return RISCV_EXCP_NONE;
}

static RISCVException write_mscratch(CPURISCVState *env, int csrno,
                                     target_ulong val)
{
    SET_SPECIAL_REG(env, mscratch, mscratchc, val);
    return RISCV_EXCP_NONE;
}

static RISCVException read_mepc(CPURISCVState *env, int csrno,
                                     target_ulong *val)
{
    *val = GET_SPECIAL_REG_ARCH(env, mepc, mepcc);
    // RISC-V privileged spec 3.1.15 Machine Exception Program Counter (mepc):
    // "The low bit of mepc (mepc[0]) is always zero. [...] Whenever IALIGN=32,
    // mepc[1] is masked on reads so that it appears to be 0."
    *val &= ~(target_ulong)(riscv_has_ext(env, RVC) ? 1 : 3);
    return RISCV_EXCP_NONE;
}

static RISCVException write_mepc(CPURISCVState *env, int csrno,
                                     target_ulong val)
{
    SET_SPECIAL_REG(env, mepc, mepcc, val);
    return RISCV_EXCP_NONE;
}

static RISCVException read_mcause(CPURISCVState *env, int csrno,
                                     target_ulong *val)
{
    *val = env->mcause;
    return RISCV_EXCP_NONE;
}

static RISCVException write_mcause(CPURISCVState *env, int csrno,
                                     target_ulong val)
{
    env->mcause = val;
    return RISCV_EXCP_NONE;
}

static RISCVException read_mtval(CPURISCVState *env, int csrno,
                                 target_ulong *val)
{
    *val = env->mtval;
    return RISCV_EXCP_NONE;
}

static RISCVException write_mtval(CPURISCVState *env, int csrno,
                                  target_ulong val)
{
    env->mtval = val;
    return RISCV_EXCP_NONE;
}

static RISCVException rmw_mip(CPURISCVState *env, int csrno,
                              target_ulong *ret_value,
                              target_ulong new_value, target_ulong write_mask)
{
    RISCVCPU *cpu = env_archcpu(env);
    /* Allow software control of delegable interrupts not claimed by hardware */
    target_ulong mask = write_mask & delegable_ints & ~env->miclaim;
    uint32_t old_mip;

    if (mask) {
        old_mip = riscv_cpu_update_mip(cpu, mask, (new_value & mask));
    } else {
        old_mip = env->mip;
    }

    if (ret_value) {
        *ret_value = old_mip;
    }

    return RISCV_EXCP_NONE;
}

/* Supervisor Trap Setup */
static RISCVException read_sstatus(CPURISCVState *env, int csrno,
                                   target_ulong *val)
{
    target_ulong mask = (sstatus_v1_10_mask);

    /* TODO: Use SXL not MXL. */
    *val = add_status_sd(riscv_cpu_mxl(env), env->mstatus & mask);
    return RISCV_EXCP_NONE;
}

static RISCVException write_sstatus(CPURISCVState *env, int csrno,
                                    target_ulong val)
{
    target_ulong mask = (sstatus_v1_10_mask);
    target_ulong newval = (env->mstatus & ~mask) | (val & mask);
    return write_mstatus(env, CSR_MSTATUS, newval);
}

static RISCVException read_vsie(CPURISCVState *env, int csrno,
                                target_ulong *val)
{
    /* Shift the VS bits to their S bit location in vsie */
    *val = (env->mie & env->hideleg & VS_MODE_INTERRUPTS) >> 1;
    return RISCV_EXCP_NONE;
}

static RISCVException read_sie(CPURISCVState *env, int csrno,
                               target_ulong *val)
{
    if (riscv_cpu_virt_enabled(env)) {
        read_vsie(env, CSR_VSIE, val);
    } else {
        *val = env->mie & env->mideleg;
    }
    return RISCV_EXCP_NONE;
}

static RISCVException write_vsie(CPURISCVState *env, int csrno,
                                 target_ulong val)
{
    /* Shift the S bits to their VS bit location in mie */
    target_ulong newval = (env->mie & ~VS_MODE_INTERRUPTS) |
                          ((val << 1) & env->hideleg & VS_MODE_INTERRUPTS);
    return write_mie(env, CSR_MIE, newval);
}

static int write_sie(CPURISCVState *env, int csrno, target_ulong val)
{
    if (riscv_cpu_virt_enabled(env)) {
        write_vsie(env, CSR_VSIE, val);
    } else {
        target_ulong newval = (env->mie & ~S_MODE_INTERRUPTS) |
                              (val & S_MODE_INTERRUPTS);
        write_mie(env, CSR_MIE, newval);
    }

    return RISCV_EXCP_NONE;
}

static RISCVException read_stvec(CPURISCVState *env, int csrno,
                                 target_ulong *val)
{
    *val = GET_SPECIAL_REG_ARCH(env, stvec, stvecc);
    return RISCV_EXCP_NONE;
}

static RISCVException write_stvec(CPURISCVState *env, int csrno,
                                  target_ulong val)
{
    /* bits [1:0] encode mode; 0 = direct, 1 = vectored, 2 >= reserved */
    if ((val & 3) < 2) {
        SET_SPECIAL_REG(env, stvec, stvecc, val);
    } else {
        qemu_log_mask(LOG_UNIMP, "CSR_STVEC: reserved mode not supported\n");
    }
    return RISCV_EXCP_NONE;
}

static RISCVException read_scounteren(CPURISCVState *env, int csrno,
                                      target_ulong *val)
{
    *val = env->scounteren;
    return RISCV_EXCP_NONE;
}

static RISCVException write_scounteren(CPURISCVState *env, int csrno,
                                       target_ulong val)
{
    env->scounteren = val;
    return RISCV_EXCP_NONE;
}

/* Supervisor Trap Handling */
static RISCVException read_sscratch(CPURISCVState *env, int csrno,
                                    target_ulong *val)
{
    *val = GET_SPECIAL_REG_ARCH(env, sscratch, sscratchc);
    return RISCV_EXCP_NONE;
}

static RISCVException write_sscratch(CPURISCVState *env, int csrno,
                                     target_ulong val)
{
    SET_SPECIAL_REG(env, sscratch, sscratchc, val);
    return RISCV_EXCP_NONE;
}

static RISCVException read_sepc(CPURISCVState *env, int csrno,
                                target_ulong *val)
{
    *val = GET_SPECIAL_REG_ARCH(env, sepc, sepcc);
    // RISC-V privileged spec 4.1.7 Supervisor Exception Program Counter (sepc)
    // "The low bit of sepc (sepc[0]) is always zero. [...] Whenever IALIGN=32,
    // sepc[1] is masked on reads so that it appears to be 0."
    *val &= ~(target_ulong)(riscv_has_ext(env, RVC) ? 1 : 3);
    return RISCV_EXCP_NONE;
}

static RISCVException write_sepc(CPURISCVState *env, int csrno,
                                 target_ulong val)
{
    SET_SPECIAL_REG(env, sepc, sepcc, val);
    return RISCV_EXCP_NONE;
}

static RISCVException read_scause(CPURISCVState *env, int csrno,
                                  target_ulong *val)
{
    *val = env->scause;
    return RISCV_EXCP_NONE;
}

static RISCVException write_scause(CPURISCVState *env, int csrno,
                                   target_ulong val)
{
    env->scause = val;
    return RISCV_EXCP_NONE;
}

static RISCVException read_stval(CPURISCVState *env, int csrno,
                                 target_ulong *val)
{
    *val = env->stval;
    return RISCV_EXCP_NONE;
}

static RISCVException write_stval(CPURISCVState *env, int csrno,
                                  target_ulong val)
{
    env->stval = val;
    return RISCV_EXCP_NONE;
}

static RISCVException rmw_vsip(CPURISCVState *env, int csrno,
                               target_ulong *ret_value,
                               target_ulong new_value, target_ulong write_mask)
{
    /* Shift the S bits to their VS bit location in mip */
    int ret = rmw_mip(env, 0, ret_value, new_value << 1,
                      (write_mask << 1) & vsip_writable_mask & env->hideleg);

    if (ret_value) {
        *ret_value &= VS_MODE_INTERRUPTS;
        /* Shift the VS bits to their S bit location in vsip */
        *ret_value >>= 1;
    }
    return ret;
}

static RISCVException rmw_sip(CPURISCVState *env, int csrno,
                              target_ulong *ret_value,
                              target_ulong new_value, target_ulong write_mask)
{
    int ret;

    if (riscv_cpu_virt_enabled(env)) {
        ret = rmw_vsip(env, CSR_VSIP, ret_value, new_value, write_mask);
    } else {
        ret = rmw_mip(env, CSR_MSTATUS, ret_value, new_value,
                      write_mask & env->mideleg & sip_writable_mask);
    }

    if (ret_value) {
        *ret_value &= env->mideleg;
    }
    return ret;
}

/* Supervisor Protection and Translation */
static RISCVException read_satp(CPURISCVState *env, int csrno,
                                target_ulong *val)
{
    if (!riscv_feature(env, RISCV_FEATURE_MMU)) {
        *val = 0;
        return RISCV_EXCP_NONE;
    }

    if (env->priv == PRV_S && get_field(env->mstatus, MSTATUS_TVM)) {
        return RISCV_EXCP_ILLEGAL_INST;
    } else {
        *val = env->satp;
    }

    return RISCV_EXCP_NONE;
}

static RISCVException write_satp(CPURISCVState *env, int csrno,
                                 target_ulong val)
{
    target_ulong vm, mask, asid;

    if (!riscv_feature(env, RISCV_FEATURE_MMU)) {
        return RISCV_EXCP_NONE;
    }

    if (riscv_cpu_mxl(env) == MXL_RV32) {
        vm = validate_vm(env, get_field(val, SATP32_MODE));
        mask = (val ^ env->satp) & (SATP32_MODE | SATP32_ASID | SATP32_PPN);
        asid = (val ^ env->satp) & SATP32_ASID;
    } else {
        vm = validate_vm(env, get_field(val, SATP64_MODE));
        mask = (val ^ env->satp) & (SATP64_MODE | SATP64_ASID | SATP64_PPN);
        asid = (val ^ env->satp) & SATP64_ASID;
    }

    if (vm && mask) {
        if (env->priv == PRV_S && get_field(env->mstatus, MSTATUS_TVM)) {
            return RISCV_EXCP_ILLEGAL_INST;
        } else {
            if (asid) {
                tlb_flush(env_cpu(env));
            }
            env->satp = val;
        }
    }
    return RISCV_EXCP_NONE;
}

/* Hypervisor Extensions */
static RISCVException read_hstatus(CPURISCVState *env, int csrno,
                                   target_ulong *val)
{
    *val = env->hstatus;
    if (riscv_cpu_mxl(env) != MXL_RV32) {
        /* We only support 64-bit VSXL */
        *val = set_field(*val, HSTATUS_VSXL, 2);
    }
    /* We only support little endian */
    *val = set_field(*val, HSTATUS_VSBE, 0);
    return RISCV_EXCP_NONE;
}

static RISCVException write_hstatus(CPURISCVState *env, int csrno,
                                    target_ulong val)
{
    env->hstatus = val;
    if (riscv_cpu_mxl(env) != MXL_RV32 && get_field(val, HSTATUS_VSXL) != 2) {
        qemu_log_mask(LOG_UNIMP, "QEMU does not support mixed HSXLEN options.");
    }
    if (get_field(val, HSTATUS_VSBE) != 0) {
        qemu_log_mask(LOG_UNIMP, "QEMU does not support big endian guests.");
    }
    return RISCV_EXCP_NONE;
}

static RISCVException read_hedeleg(CPURISCVState *env, int csrno,
                                   target_ulong *val)
{
    *val = env->hedeleg;
    return RISCV_EXCP_NONE;
}

static RISCVException write_hedeleg(CPURISCVState *env, int csrno,
                                    target_ulong val)
{
    env->hedeleg = val & vs_delegable_excps;
    return RISCV_EXCP_NONE;
}

static RISCVException read_hideleg(CPURISCVState *env, int csrno,
                                   target_ulong *val)
{
    *val = env->hideleg;
    return RISCV_EXCP_NONE;
}

static RISCVException write_hideleg(CPURISCVState *env, int csrno,
                                    target_ulong val)
{
    env->hideleg = val & vs_delegable_ints;
    return RISCV_EXCP_NONE;
}

static RISCVException rmw_hvip(CPURISCVState *env, int csrno,
                               target_ulong *ret_value,
                               target_ulong new_value, target_ulong write_mask)
{
    int ret = rmw_mip(env, 0, ret_value, new_value,
                      write_mask & hvip_writable_mask);

    if (ret_value) {
        *ret_value &= hvip_writable_mask;
    }
    return ret;
}

static RISCVException rmw_hip(CPURISCVState *env, int csrno,
                              target_ulong *ret_value,
                              target_ulong new_value, target_ulong write_mask)
{
    int ret = rmw_mip(env, 0, ret_value, new_value,
                      write_mask & hip_writable_mask);

    if (ret_value) {
        *ret_value &= hip_writable_mask;
    }
    return ret;
}

static RISCVException read_hie(CPURISCVState *env, int csrno,
                               target_ulong *val)
{
    *val = env->mie & VS_MODE_INTERRUPTS;
    return RISCV_EXCP_NONE;
}

static RISCVException write_hie(CPURISCVState *env, int csrno,
                                target_ulong val)
{
    target_ulong newval = (env->mie & ~VS_MODE_INTERRUPTS) | (val & VS_MODE_INTERRUPTS);
    return write_mie(env, CSR_MIE, newval);
}

static RISCVException read_hcounteren(CPURISCVState *env, int csrno,
                                      target_ulong *val)
{
    *val = env->hcounteren;
    return RISCV_EXCP_NONE;
}

static RISCVException write_hcounteren(CPURISCVState *env, int csrno,
                                       target_ulong val)
{
    env->hcounteren = val;
    return RISCV_EXCP_NONE;
}

static RISCVException write_hgeie(CPURISCVState *env, int csrno,
                                  target_ulong val)
{
    if (val) {
        qemu_log_mask(LOG_UNIMP, "No support for a non-zero GEILEN.");
    }
    return RISCV_EXCP_NONE;
}

static RISCVException read_htval(CPURISCVState *env, int csrno,
                                 target_ulong *val)
{
    *val = env->htval;
    return RISCV_EXCP_NONE;
}

static RISCVException write_htval(CPURISCVState *env, int csrno,
                                  target_ulong val)
{
    env->htval = val;
    return RISCV_EXCP_NONE;
}

static RISCVException read_htinst(CPURISCVState *env, int csrno,
                                  target_ulong *val)
{
    *val = env->htinst;
    return RISCV_EXCP_NONE;
}

static RISCVException write_htinst(CPURISCVState *env, int csrno,
                                   target_ulong val)
{
    return RISCV_EXCP_NONE;
}

static RISCVException write_hgeip(CPURISCVState *env, int csrno,
                                  target_ulong val)
{
    if (val) {
        qemu_log_mask(LOG_UNIMP, "No support for a non-zero GEILEN.");
    }
    return RISCV_EXCP_NONE;
}

static RISCVException read_hgatp(CPURISCVState *env, int csrno,
                                 target_ulong *val)
{
    *val = env->hgatp;
    return RISCV_EXCP_NONE;
}

static RISCVException write_hgatp(CPURISCVState *env, int csrno,
                                  target_ulong val)
{
    env->hgatp = val;
    return RISCV_EXCP_NONE;
}

static RISCVException read_htimedelta(CPURISCVState *env, int csrno,
                                      target_ulong *val)
{
    if (!env->rdtime_fn) {
        return RISCV_EXCP_ILLEGAL_INST;
    }

    *val = env->htimedelta;
    return RISCV_EXCP_NONE;
}

static RISCVException write_htimedelta(CPURISCVState *env, int csrno,
                                       target_ulong val)
{
    if (!env->rdtime_fn) {
        return RISCV_EXCP_ILLEGAL_INST;
    }

    if (riscv_cpu_mxl(env) == MXL_RV32) {
        env->htimedelta = deposit64(env->htimedelta, 0, 32, (uint64_t)val);
    } else {
        env->htimedelta = val;
    }
    return RISCV_EXCP_NONE;
}

static RISCVException read_htimedeltah(CPURISCVState *env, int csrno,
                                       target_ulong *val)
{
    if (!env->rdtime_fn) {
        return RISCV_EXCP_ILLEGAL_INST;
    }

    *val = env->htimedelta >> 32;
    return RISCV_EXCP_NONE;
}

static RISCVException write_htimedeltah(CPURISCVState *env, int csrno,
                                        target_ulong val)
{
    if (!env->rdtime_fn) {
        return RISCV_EXCP_ILLEGAL_INST;
    }

    env->htimedelta = deposit64(env->htimedelta, 32, 32, (uint64_t)val);
    return RISCV_EXCP_NONE;
}

/* Virtual CSR Registers */
static RISCVException read_vsstatus(CPURISCVState *env, int csrno,
                                    target_ulong *val)
{
    *val = env->vsstatus;
    return RISCV_EXCP_NONE;
}

static RISCVException write_vsstatus(CPURISCVState *env, int csrno,
                                     target_ulong val)
{
    uint64_t mask = (target_ulong)-1;
    env->vsstatus = (env->vsstatus & ~mask) | (uint64_t)val;
    return RISCV_EXCP_NONE;
}

static int read_vstvec(CPURISCVState *env, int csrno, target_ulong *val)
{
    *val = GET_SPECIAL_REG_ARCH(env, vstvec, vstvecc);
    return RISCV_EXCP_NONE;
}

static RISCVException write_vstvec(CPURISCVState *env, int csrno,
                                   target_ulong val)
{
    SET_SPECIAL_REG(env, vstvec, vstvecc, val);
    return RISCV_EXCP_NONE;
}

static RISCVException read_vsscratch(CPURISCVState *env, int csrno,
                                     target_ulong *val)
{
    *val = GET_SPECIAL_REG_ARCH(env, vsscratch, vsscratchc);
    return RISCV_EXCP_NONE;
}

static RISCVException write_vsscratch(CPURISCVState *env, int csrno,
                                      target_ulong val)
{
    SET_SPECIAL_REG(env, vsscratch, vsscratchc, val);
    return RISCV_EXCP_NONE;
}

static RISCVException read_vsepc(CPURISCVState *env, int csrno,
                                 target_ulong *val)
{
    *val = GET_SPECIAL_REG_ARCH(env, vsepc, vsepcc);
    return RISCV_EXCP_NONE;
}

static RISCVException write_vsepc(CPURISCVState *env, int csrno,
                                  target_ulong val)
{
    SET_SPECIAL_REG(env, vsepc, vsepcc, val);
    return RISCV_EXCP_NONE;
}

static RISCVException read_vscause(CPURISCVState *env, int csrno,
                                   target_ulong *val)
{
    *val = env->vscause;
    return RISCV_EXCP_NONE;
}

static RISCVException write_vscause(CPURISCVState *env, int csrno,
                                    target_ulong val)
{
    env->vscause = val;
    return RISCV_EXCP_NONE;
}

static RISCVException read_vstval(CPURISCVState *env, int csrno,
                                  target_ulong *val)
{
    *val = env->vstval;
    return RISCV_EXCP_NONE;
}

static RISCVException write_vstval(CPURISCVState *env, int csrno,
                                   target_ulong val)
{
    env->vstval = val;
    return RISCV_EXCP_NONE;
}

static RISCVException read_vsatp(CPURISCVState *env, int csrno,
                                 target_ulong *val)
{
    *val = env->vsatp;
    return RISCV_EXCP_NONE;
}

static RISCVException write_vsatp(CPURISCVState *env, int csrno,
                                  target_ulong val)
{
    env->vsatp = val;
    return RISCV_EXCP_NONE;
}

static RISCVException read_mtval2(CPURISCVState *env, int csrno,
                                  target_ulong *val)
{
    *val = env->mtval2;
    return RISCV_EXCP_NONE;
}

static RISCVException write_mtval2(CPURISCVState *env, int csrno,
                                   target_ulong val)
{
    env->mtval2 = val;
    return RISCV_EXCP_NONE;
}

static RISCVException read_mtinst(CPURISCVState *env, int csrno,
                                  target_ulong *val)
{
    *val = env->mtinst;
    return RISCV_EXCP_NONE;
}

static RISCVException write_mtinst(CPURISCVState *env, int csrno,
                                   target_ulong val)
{
    env->mtinst = val;
    return RISCV_EXCP_NONE;
}

static RISCVException read_menvcfg(CPURISCVState *env, int csrno,
                                   target_ulong *val)
{
    *val = env->menvcfg;
    return RISCV_EXCP_NONE;
}

static RISCVException write_menvcfg(CPURISCVState *env, int csrno,
                                    target_ulong val)
{
    env->menvcfg = val;
    return RISCV_EXCP_NONE;
}

static RISCVException read_senvcfg(CPURISCVState *env, int csrno,
                                   target_ulong *val)
{
    *val = env->senvcfg;
    return RISCV_EXCP_NONE;
}

static RISCVException write_senvcfg(CPURISCVState *env, int csrno,
                                    target_ulong val)
{
    env->senvcfg = val;
    return RISCV_EXCP_NONE;
}

/* Physical Memory Protection */
static RISCVException read_mseccfg(CPURISCVState *env, int csrno,
                                   target_ulong *val)
{
    *val = mseccfg_csr_read(env);
    return RISCV_EXCP_NONE;
}

static RISCVException write_mseccfg(CPURISCVState *env, int csrno,
                         target_ulong val)
{
    mseccfg_csr_write(env, val);
    return RISCV_EXCP_NONE;
}

static RISCVException read_pmpcfg(CPURISCVState *env, int csrno,
                                  target_ulong *val)
{
    *val = pmpcfg_csr_read(env, csrno - CSR_PMPCFG0);
    return RISCV_EXCP_NONE;
}

static RISCVException write_pmpcfg(CPURISCVState *env, int csrno,
                                   target_ulong val)
{
    pmpcfg_csr_write(env, csrno - CSR_PMPCFG0, val);
#ifdef CONFIG_TCG_LOG_INSTR
    if (qemu_log_instr_enabled(env)) {
        char buf[16];
        snprintf(buf, sizeof(buf), "pmpcfg%d", csrno - CSR_PMPCFG0);
        qemu_log_instr_reg(env, buf, val, csrno, LRI_CSR_ACCESS);
    }
#endif
    return RISCV_EXCP_NONE;
}

static RISCVException read_pmpaddr(CPURISCVState *env, int csrno,
                                   target_ulong *val)
{
    *val = pmpaddr_csr_read(env, csrno - CSR_PMPADDR0);
    return RISCV_EXCP_NONE;
}

static RISCVException write_pmpaddr(CPURISCVState *env, int csrno,
                                    target_ulong val)
{
    pmpaddr_csr_write(env, csrno - CSR_PMPADDR0, val);
#ifdef CONFIG_TCG_LOG_INSTR
    if (qemu_log_instr_enabled(env)) {
        char buf[16];
        snprintf(buf, sizeof(buf), "pmpaddr%d", csrno - CSR_PMPADDR0);
        qemu_log_instr_reg(env, buf, val, csrno, LRI_CSR_ACCESS);
    }
#endif
    return RISCV_EXCP_NONE;
}

/*
 * Functions to access Pointer Masking feature registers
 * We have to check if current priv lvl could modify
 * csr in given mode
 */
static bool check_pm_current_disabled(CPURISCVState *env, int csrno)
{
    int csr_priv = get_field(csrno, 0x300);
    int pm_current;

    /*
     * If priv lvls differ that means we're accessing csr from higher priv lvl,
     * so allow the access
     */
    if (env->priv != csr_priv) {
        return false;
    }
    switch (env->priv) {
    case PRV_M:
        pm_current = get_field(env->mmte, M_PM_CURRENT);
        break;
    case PRV_S:
        pm_current = get_field(env->mmte, S_PM_CURRENT);
        break;
    case PRV_U:
        pm_current = get_field(env->mmte, U_PM_CURRENT);
        break;
    default:
        g_assert_not_reached();
    }
    /* It's same priv lvl, so we allow to modify csr only if pm.current==1 */
    return !pm_current;
}

static RISCVException read_mmte(CPURISCVState *env, int csrno,
                                target_ulong *val)
{
    *val = env->mmte & MMTE_MASK;
    return RISCV_EXCP_NONE;
}

static RISCVException write_mmte(CPURISCVState *env, int csrno,
                                 target_ulong val)
{
    uint64_t mstatus;
    target_ulong wpri_val = val & MMTE_MASK;

    if (val != wpri_val) {
        qemu_log_mask(LOG_GUEST_ERROR, "%s" TARGET_FMT_lx " %s" TARGET_FMT_lx "\n",
                      "MMTE: WPRI violation written 0x", val,
                      "vs expected 0x", wpri_val);
    }
    /* for machine mode pm.current is hardwired to 1 */
    wpri_val |= MMTE_M_PM_CURRENT;

    /* hardwiring pm.instruction bit to 0, since it's not supported yet */
    wpri_val &= ~(MMTE_M_PM_INSN | MMTE_S_PM_INSN | MMTE_U_PM_INSN);
    env->mmte = wpri_val | PM_EXT_DIRTY;

    /* Set XS and SD bits, since PM CSRs are dirty */
    mstatus = env->mstatus | MSTATUS_XS;
    write_mstatus(env, csrno, mstatus);
    return RISCV_EXCP_NONE;
}

static RISCVException read_smte(CPURISCVState *env, int csrno,
                                target_ulong *val)
{
    *val = env->mmte & SMTE_MASK;
    return RISCV_EXCP_NONE;
}

static RISCVException write_smte(CPURISCVState *env, int csrno,
                                 target_ulong val)
{
    target_ulong wpri_val = val & SMTE_MASK;

    if (val != wpri_val) {
        qemu_log_mask(LOG_GUEST_ERROR, "%s" TARGET_FMT_lx " %s" TARGET_FMT_lx "\n",
                      "SMTE: WPRI violation written 0x", val,
                      "vs expected 0x", wpri_val);
    }

    /* if pm.current==0 we can't modify current PM CSRs */
    if (check_pm_current_disabled(env, csrno)) {
        return RISCV_EXCP_NONE;
    }

    wpri_val |= (env->mmte & ~SMTE_MASK);
    write_mmte(env, csrno, wpri_val);
    return RISCV_EXCP_NONE;
}

static RISCVException read_umte(CPURISCVState *env, int csrno,
                                target_ulong *val)
{
    *val = env->mmte & UMTE_MASK;
    return RISCV_EXCP_NONE;
}

static RISCVException write_umte(CPURISCVState *env, int csrno,
                                 target_ulong val)
{
    target_ulong wpri_val = val & UMTE_MASK;

    if (val != wpri_val) {
        qemu_log_mask(LOG_GUEST_ERROR, "%s" TARGET_FMT_lx " %s" TARGET_FMT_lx "\n",
                      "UMTE: WPRI violation written 0x", val,
                      "vs expected 0x", wpri_val);
    }

    if (check_pm_current_disabled(env, csrno)) {
        return RISCV_EXCP_NONE;
    }

    wpri_val |= (env->mmte & ~UMTE_MASK);
    write_mmte(env, csrno, wpri_val);
    return RISCV_EXCP_NONE;
}

static RISCVException read_mpmmask(CPURISCVState *env, int csrno,
                                   target_ulong *val)
{
    *val = env->mpmmask;
    return RISCV_EXCP_NONE;
}

static RISCVException write_mpmmask(CPURISCVState *env, int csrno,
                                    target_ulong val)
{
    uint64_t mstatus;

    env->mpmmask = val;
    env->mmte |= PM_EXT_DIRTY;

    /* Set XS and SD bits, since PM CSRs are dirty */
    mstatus = env->mstatus | MSTATUS_XS;
    write_mstatus(env, csrno, mstatus);
    return RISCV_EXCP_NONE;
}

static RISCVException read_spmmask(CPURISCVState *env, int csrno,
                                   target_ulong *val)
{
    *val = env->spmmask;
    return RISCV_EXCP_NONE;
}

static RISCVException write_spmmask(CPURISCVState *env, int csrno,
                                    target_ulong val)
{
    uint64_t mstatus;

    /* if pm.current==0 we can't modify current PM CSRs */
    if (check_pm_current_disabled(env, csrno)) {
        return RISCV_EXCP_NONE;
    }
    env->spmmask = val;
    env->mmte |= PM_EXT_DIRTY;

    /* Set XS and SD bits, since PM CSRs are dirty */
    mstatus = env->mstatus | MSTATUS_XS;
    write_mstatus(env, csrno, mstatus);
    return RISCV_EXCP_NONE;
}

static RISCVException read_upmmask(CPURISCVState *env, int csrno,
                                   target_ulong *val)
{
    *val = env->upmmask;
    return RISCV_EXCP_NONE;
}

static RISCVException write_upmmask(CPURISCVState *env, int csrno,
                                    target_ulong val)
{
    uint64_t mstatus;

    /* if pm.current==0 we can't modify current PM CSRs */
    if (check_pm_current_disabled(env, csrno)) {
        return RISCV_EXCP_NONE;
    }
    env->upmmask = val;
    env->mmte |= PM_EXT_DIRTY;

    /* Set XS and SD bits, since PM CSRs are dirty */
    mstatus = env->mstatus | MSTATUS_XS;
    write_mstatus(env, csrno, mstatus);
    return RISCV_EXCP_NONE;
}

static RISCVException read_mpmbase(CPURISCVState *env, int csrno,
                                   target_ulong *val)
{
    *val = env->mpmbase;
    return RISCV_EXCP_NONE;
}

static RISCVException write_mpmbase(CPURISCVState *env, int csrno,
                                    target_ulong val)
{
    uint64_t mstatus;

    env->mpmbase = val;
    env->mmte |= PM_EXT_DIRTY;

    /* Set XS and SD bits, since PM CSRs are dirty */
    mstatus = env->mstatus | MSTATUS_XS;
    write_mstatus(env, csrno, mstatus);
    return RISCV_EXCP_NONE;
}

static RISCVException read_spmbase(CPURISCVState *env, int csrno,
                                   target_ulong *val)
{
    *val = env->spmbase;
    return RISCV_EXCP_NONE;
}

static RISCVException write_spmbase(CPURISCVState *env, int csrno,
                                    target_ulong val)
{
    uint64_t mstatus;

    /* if pm.current==0 we can't modify current PM CSRs */
    if (check_pm_current_disabled(env, csrno)) {
        return RISCV_EXCP_NONE;
    }
    env->spmbase = val;
    env->mmte |= PM_EXT_DIRTY;

    /* Set XS and SD bits, since PM CSRs are dirty */
    mstatus = env->mstatus | MSTATUS_XS;
    write_mstatus(env, csrno, mstatus);
    return RISCV_EXCP_NONE;
}

static RISCVException read_upmbase(CPURISCVState *env, int csrno,
                                   target_ulong *val)
{
    *val = env->upmbase;
    return RISCV_EXCP_NONE;
}

static RISCVException write_upmbase(CPURISCVState *env, int csrno,
                                    target_ulong val)
{
    uint64_t mstatus;

    /* if pm.current==0 we can't modify current PM CSRs */
    if (check_pm_current_disabled(env, csrno)) {
        return RISCV_EXCP_NONE;
    }
    env->upmbase = val;
    env->mmte |= PM_EXT_DIRTY;

    /* Set XS and SD bits, since PM CSRs are dirty */
    mstatus = env->mstatus | MSTATUS_XS;
    write_mstatus(env, csrno, mstatus);
    return RISCV_EXCP_NONE;
}
#ifndef TARGET_CHERI
static RISCVException stid(CPURISCVState *env, int csrno)
{
    if (riscv_feature(env, RISCV_FEATURE_STID)) {
        return RISCV_EXCP_NONE;
    }

    return RISCV_EXCP_ILLEGAL_INST;
}
/* Thread ID (Zstid) */
static RISCVException read_mtid(CPURISCVState *env, int csrno,
                                target_ulong *val)
{
    *val = env->mtid;
    return RISCV_EXCP_NONE;
}

static RISCVException write_mtid(CPURISCVState *env, int csrno,
                                 target_ulong val)
{
    env->mtid = val;
    return RISCV_EXCP_NONE;
}

static RISCVException read_stid(CPURISCVState *env, int csrno,
                                target_ulong *val)
{
    *val = env->stid;
    return RISCV_EXCP_NONE;
}

static RISCVException write_stid(CPURISCVState *env, int csrno,
                                 target_ulong val)
{
    env->stid = val;
    return RISCV_EXCP_NONE;
}

static RISCVException read_vstid(CPURISCVState *env, int csrno,
                                 target_ulong *val)
{
    *val = env->vstid;
    return RISCV_EXCP_NONE;
}

static RISCVException write_vstid(CPURISCVState *env, int csrno,
                                  target_ulong val)
{
    env->vstid = val;
    return RISCV_EXCP_NONE;
}

static RISCVException read_utid(CPURISCVState *env, int csrno,
                                target_ulong *val)
{
    *val = env->utid;
    return RISCV_EXCP_NONE;
}

static RISCVException write_utid(CPURISCVState *env, int csrno,
                                 target_ulong val)
{
    env->utid = val;
    return RISCV_EXCP_NONE;
}
#endif

#endif

#ifdef TARGET_CHERI
/* handlers for capability csr registers */

cap_register_t *get_cap_csr(CPUArchState *env, uint32_t index)
{
    switch (index) {
    case CSR_MSCRATCHC:
        return &env->mscratchc;
    case CSR_MTVECC:
        return &env->mtvecc;
    case CSR_STVECC:
        return &env->stvecc;
    case CSR_MEPCC:
        return &env->mepcc;
    case CSR_SEPCC:
        return &env->sepcc;
    case CSR_SSCRATCHC:
        return &env->sscratchc;
    case CSR_DDC:
        return &env->ddc;
    case CSR_MTIDC:
        return &env->mtidc;
    case CSR_STIDC:
        return &env->stidc;
    case CSR_UTIDC:
        return &env->utidc;
    case CSR_VSTIDC:
        return &env->vstidc;
    case CSR_VSSCRATCHC:
        return &env->vsscratchc;
    case CSR_VSEPCC:
        return &env->vsepcc;
    case CSR_VSTVECC:
        return &env->vstvecc;
#ifdef TARGET_CHERI_RISCV_V9
    case CSR_MTDC:
        return &env->mtdc;
    case CSR_STDC:
        return &env->stdc;
    case CSR_VSTDC:
        return &env->vstdc;
    case CSR_PCC:
        return &env->pcc;
#endif
    default:
        assert(false && "Should have raised an invalid inst trap!");
    }
}

/*
 * Reads a capability length csr register taking into account the current
 * CHERI execution mode
 */
static cap_register_t read_capcsr_reg(CPURISCVState *env,
                                      riscv_csr_cap_ops *csr_cap_info)
{
    cap_register_t retval = *get_cap_csr(env, csr_cap_info->reg_num);
    return retval;
}


#define get_bit(reg, x) (reg & (1 << x) ? true : false)

// Borrow the signextend function from capstone
static inline int64_t SignExtend64(uint64_t X, unsigned B)
{
    return (int64_t)(X << (64 - B)) >> (64 - B);
}

static inline uint8_t topbit_for_address_mode(CPUArchState *env)
{
    uint64_t vm = get_field(
        env->vsatp, riscv_cpu_mxl(env) == MXL_RV32 ? SATP32_MODE : SATP64_MODE);
    uint8_t checkbit = 0;
    switch (vm) {
    case VM_1_10_SV32:
        checkbit = 31;
        break;
    case VM_1_10_SV39:
        checkbit = 38;
        break;
    case VM_1_10_SV48:
        checkbit = 47;
        break;
    case VM_1_10_SV57:
        checkbit = 56;
        break;
    default:
        g_assert_not_reached();
    }
    return checkbit;
}

/*
Check if the address is valid for the target capability.
This depends on the addrress mode
• For Sv39, bits [63:39] must equal bit 38
• For Sv48, bits [63:48] must equal bit 47
• For Sv57, bits [63:57] must equal bit 56
If address translation is not active or we are using sv32 then treat the address
as valid.
This only applies for rv64
*/
static inline bool is_address_valid_for_cap(CPUArchState *env,
                                            cap_register_t cap,
                                            target_ulong addr)
{
#ifdef TARGET_RISCV32
    return true;
#endif
    uint64_t vm = get_field(
        env->vsatp, riscv_cpu_mxl(env) == MXL_RV32 ? SATP32_MODE : SATP64_MODE);
    if (vm == VM_1_10_MBARE || vm == VM_1_10_SV32) {
        return true;
    }
    uint8_t checkbit = topbit_for_address_mode(env);
    target_ulong address = cap_get_cursor(&cap);
    target_ulong extend_address = SignExtend64(address, checkbit);
    if (address == extend_address) {
        // this is a valid address
        return true;
    }
    // need to check for infinite bounds.
    if (cap_get_base(&cap) == 0 && cap_get_top_full(&cap) == CAP_MAX_TOP) {
        return true;
    }
    return false;
}

/*
Return a valid capability address field.
This is implementation dependant and depends on the address translation mode
*/
static inline target_ulong get_valid_cap_address(CPUArchState *env,
                                                 target_ulong addr)
{
    uint64_t vm = get_field(
        env->vsatp, riscv_cpu_mxl(env) == MXL_RV32 ? SATP32_MODE : SATP64_MODE);
    if (vm == VM_1_10_MBARE || vm == VM_1_10_SV32) {
        return addr;
    }
    uint8_t checkbit = topbit_for_address_mode(env);
    target_ulong extend_address = SignExtend64(addr, checkbit);
    return extend_address;
}

/*
Given a capability and address turn the address into a valid address for that
capability and return true if the address was changed
*/
static inline bool validate_cap_address(CPUArchState *env, cap_register_t *cap,
                                        target_ulong *address)
{
    if (is_address_valid_for_cap(env, *cap, *address)) {
        return false;
    }
    *address = get_valid_cap_address(env, *address);
    return true;
}

/*
The function takes both the source capability as well as the cursor value.
For CLEN writes the source capabilities bounds would be taken into account
when computing the invalid address conversion..

*/
static void write_cap_csr_reg(CPURISCVState *env,
                              riscv_csr_cap_ops *csr_cap_info,
                              cap_register_t src, target_ulong newval,
                              bool clen)
{
    cap_register_t csr = *get_cap_csr(env, csr_cap_info->reg_num);
    /* CLEN writes only for csrrw calls, all other writes are XLEN */
    if (clen) {
        if (csr_cap_info->flags & CSR_OP_IA_CONVERSION) {
            bool changed = validate_cap_address(env, &src, &newval);
            if (csr_cap_info->flags & CSR_OP_UPDATE_SCADDR) {
                /* E.g. xtvec always invalidates sealed caps */
                src = cap_scaddr(newval, src);
            } else if (changed) {
                /* Only use scaddr if validate changed the address (e.g. epc) */
                src = cap_scaddr(newval, src);
            }
        }
        /* Otherwise just fall through to direct write */
    } else {
        if (csr_cap_info->flags & CSR_OP_IA_CONVERSION) {
            /* For XLEN writes we ignore the result as we always use scaddr */
            (void)validate_cap_address(env, &csr, &newval);
        }
        src = cap_scaddr(newval, csr);
    }
    /* Log the value and write it. */
    *get_cap_csr(env, csr_cap_info->reg_num) = src;
    cheri_log_instr_changed_capreg(env, csr_cap_info->name, &src,
                                   csr_cap_info->reg_num, LRI_CSR_ACCESS);
}

static void write_xtvecc(CPURISCVState *env, riscv_csr_cap_ops *csr_cap_info,
                         cap_register_t src, target_ulong new_tvec, bool clen)
{
    bool valid = true;
    cap_register_t *csr = get_cap_csr(env, csr_cap_info->reg_num);
    /* The low two bits encode the mode, but only 0 and 1 are valid. */
    if ((new_tvec & 3) > 1) {
        /* Invalid mode, keep the old one. */
        new_tvec &= ~(target_ulong)3;
        new_tvec |= cap_get_cursor(csr) & 3;
    }

    // the function needs to know if if it using the src capability or the csr's
    // existing capability in order to do the representable check.
    cap_register_t *auth;
    if (clen) { // use the source capability for checking the vector range
        auth = &src;
    } else { // use the csr register
        auth = csr;
    }

    if (!is_representable_cap_with_addr(auth, new_tvec + RISCV_HICAUSE * 4)) {
        error_report("Attempting to set vector register with unrepresentable "
                     "range (0x" TARGET_FMT_lx ") on %s: " PRINT_CAP_FMTSTR
                     "\r\n",
                     new_tvec, csr_cap_info->name, PRINT_CAP_ARGS(auth));
        qemu_log_instr_extra(
            env,
            "Attempting to set unrepresentable vector register with "
            "unrepresentable range (0x" TARGET_FMT_lx
            ") on %s: " PRINT_CAP_FMTSTR "\r\n",
            new_tvec, csr_cap_info->name, PRINT_CAP_ARGS(auth));
        valid = false;
    }
    if (!valid) {
        // caution this directly modifies the tareget csr register in integer
        // mode this should be ok, as it is invalidating the tag which is the
        // intended action
        cap_mark_unrepresentable(new_tvec, auth);
    }

    write_cap_csr_reg(env, csr_cap_info, src, new_tvec, clen);
}

static void write_xepcc(CPURISCVState *env, riscv_csr_cap_ops *csr_cap_info,
                        cap_register_t src, target_ulong new_xepcc, bool clen)
{
    new_xepcc &= (~0x1); // Zero bit zero
    write_cap_csr_reg(env, csr_cap_info, src, new_xepcc, clen);
}

// Common read function for the mepcc and sepcc registers
static cap_register_t read_xepcc(CPURISCVState *env,
                                 riscv_csr_cap_ops *csr_cap_info)
{
    cap_register_t retval = *get_cap_csr(env, csr_cap_info->reg_num);
    target_ulong val = cap_get_cursor(&retval);

    // RISC-V privileged spec 4.1.7 Supervisor Exception Program Counter
    // (sepc) "The low bit of sepc (sepc[0]) is always zero. [...] Whenever
    // IALIGN=32, sepc[1] is masked on reads so that it appears to be 0."
    val &= ~(target_ulong)(riscv_has_ext(env, RVC) ? 1 : 3);
    if (val != cap_get_cursor(&retval)) {
        warn_report("Clearing low bit(s) of %s (contained an unaligned "
                    "capability): " PRINT_CAP_FMTSTR, csr_cap_info->name,
                    PRINT_CAP_ARGS(&retval));

        if (!cap_is_unsealed(&retval)) {
            warn_report("Invalidating sealed %s (contained an unaligned "
                        "capability): " PRINT_CAP_FMTSTR,
                        csr_cap_info->name, PRINT_CAP_ARGS(&retval));
            retval.cr_tag = false;
        }

        cap_set_cursor(&retval, val);
    }

    return retval;
}

#ifdef TARGET_CHERI_RISCV_V9
static RISCVException read_ccsr(CPURISCVState *env, int csrno, target_ulong *val)
{
    // We report the same values for all modes and don't perform dirty tracking
    // The capability cause has moved to xTVAL so we don't report it here.
    RISCVCPU *cpu = env_archcpu(env);
    target_ulong ccsr = 0;
    ccsr = set_field(ccsr, XCCSR_ENABLE, cpu->cfg.ext_cheri);
    /* Read-only feature bits. */
    ccsr = set_field(ccsr, XCCSR_TAG_CLEARING, CHERI_TAG_CLEAR_ON_INVALID(env));
    ccsr = set_field(ccsr, XCCSR_NO_RELOCATION, CHERI_NO_RELOCATION(env));

#if !defined(TARGET_RISCV32)
    if (csrno == CSR_SCCSR)
        ccsr |= env->sccsr;
#endif

    qemu_log_mask(CPU_LOG_INT, "Reading xCCSR(%#x): %x\n", csrno, (int)ccsr);
    *val = ccsr;
    return RISCV_EXCP_NONE;
}

static RISCVException write_ccsr(CPURISCVState *env, int csrno, target_ulong val)
{
    switch (csrno) {
    default:
        error_report("Attempting to write " TARGET_FMT_lx
                     "to xCCSR(%#x), this is not supported (yet?).",
                     val, csrno);
        return RISCV_EXCP_INST_ACCESS_FAULT;
#if !defined(TARGET_RISCV32)
    case CSR_SCCSR: {
        static const target_ulong gclgmask = (SCCSR_SGCLG | SCCSR_UGCLG);
        /* Take the GCLG bits from the store and update state bits */
        env->sccsr = set_field(env->sccsr, gclgmask, get_field(val, gclgmask));

        /*
         * Our TLB effectively caches whether the PTE and CCSR bits match at the
         * time the PTE is copied up into the TLB.  While PTE updates use
         * SFENCE.VMA to ensure visibility in the TLB, the CCSR writes must
         * implicitly cause TLB invalidation.
         */
        tlb_flush(env_cpu(env));
        break;
      }
#endif
    }

    return RISCV_EXCP_NONE;
}
#endif

bool csr_needs_asr(uint32_t csrno, bool is_write)
{
    /*
     * Based on CSR number and write mask determineif the CSR is privileged
     * based on bits 8-9 being set.
     * See Privileged Spec, Section 2.1 CSR Address Mapping Conventions.
     * However, the *TID registers behave differently and are readable without
     * ASR in all privileged levels and require ASR for all writes.
     */
    switch (csrno) {
#ifdef TARGET_CHERI_RISCV_V9
    /* Special cases for the placeholder csr numbers for v9 compat */
    case CSR_PCC:
        return false;
    case CSR_MTDC:
    case CSR_STDC:
    case CSR_VSTDC:
        return true;
#endif
    case CSR_STIDC:
    case CSR_MTIDC:
    case CSR_UTIDC:
    case CSR_VSTIDC:
        return is_write; /* the TID registers only require asr for writes */
    default:
        return get_field(csrno, 0x300) != 0;
    }
}


#ifdef TARGET_CHERI_RISCV_STD_093
static RISCVException read_stval2(CPURISCVState *env, int csrno,
                                  target_ulong *val)
{
    *val = env->stval2;
    return RISCV_EXCP_NONE;
}

static RISCVException write_stval2(CPURISCVState *env, int csrno,
                                   target_ulong val)
{
    env->stval2 = val;
    return RISCV_EXCP_NONE;
}

static RISCVException read_vstval2(CPURISCVState *env, int csrno,
                                  target_ulong *val)
{
    *val = env->vstval2;
    return RISCV_EXCP_NONE;
}

static RISCVException write_vstval2(CPURISCVState *env, int csrno,
                                   target_ulong val)
{
    env->vstval2 = val;
    return RISCV_EXCP_NONE;
}
#endif /* TARGET_CHERI_RISCV_STD_093 */
#endif /* TARGET_CHERI */

/*
 * riscv_csrrw - read and/or update control and status register
 *
 * csrr   <->  riscv_csrrw(env, csrno, ret_value, 0, 0);
 * csrrw  <->  riscv_csrrw(env, csrno, ret_value, value, -1);
 * csrrs  <->  riscv_csrrw(env, csrno, ret_value, -1, value);
 * csrrc  <->  riscv_csrrw(env, csrno, ret_value, 0, value);
 */

RISCVException riscv_csr_accessible(CPURISCVState *env, int csrno,
                                    bool is_write)
{
    RISCVException ret;
    RISCVCPU *cpu = env_archcpu(env);
    int read_only = get_field(csrno, 0xC00) == 3;

    /* check privileges and return RISCV_EXCP_ILLEGAL_INST if check fails */
#if !defined(CONFIG_USER_ONLY)
    int effective_priv = env->priv;

    if (riscv_has_ext(env, RVH) && env->priv == PRV_S &&
        !riscv_cpu_virt_enabled(env)) {
        /*
         * We are in S mode without virtualisation, therefore we are in HS Mode.
         * Add 1 to the effective privledge level to allow us to access the
         * Hypervisor CSRs.
         */
        effective_priv++;
    }

    if (!env->debugger && (effective_priv < get_field(csrno, 0x300))) {
        return RISCV_EXCP_ILLEGAL_INST;
    }
#endif
    if (is_write && read_only) {
        return RISCV_EXCP_ILLEGAL_INST;
    }

    /* ensure the CSR extension is enabled. */
    if (!cpu->cfg.ext_icsr) {
        return RISCV_EXCP_ILLEGAL_INST;
    }
    /* check predicate */
    if (!csr_ops[csrno].predicate) {
#ifdef TARGET_CHERI
        /* CHERI-extended or added CSRs might not be in the predicates table */
        /* TODO: merge the two tables */
        if (get_csr_cap_info(csrno) == NULL) {
            return RISCV_EXCP_ILLEGAL_INST;
        }
#else
        return RISCV_EXCP_ILLEGAL_INST;
#endif
    } else {
        ret = csr_ops[csrno].predicate(env, csrno);
        if (ret != RISCV_EXCP_NONE) {
            return ret;
        }
    }
    /*
     * When CHERI is enabled, only certain CSRs can be accessed without the
     * Access_System_Registers permission in PCC.
     * TODO: could merge this with predicate callback?
     */
#ifdef TARGET_CHERI
    if (!cheri_have_access_sysregs(env) && csr_needs_asr(csrno, is_write)) {
#if !defined(CONFIG_USER_ONLY)
        if (env->debugger) {
            return RISCV_EXCP_INST_ACCESS_FAULT;
        }
        return RISCV_EXCP_CHERI;
#endif
    }
#endif // TARGET_CHERI
    return RISCV_EXCP_NONE;
}

RISCVException riscv_csrrw(CPURISCVState *env, int csrno,
                           target_ulong *ret_value, target_ulong new_value,
                           target_ulong write_mask, uintptr_t retpc)
{
    RISCVException ret;
    target_ulong old_value;

    /* check privileges and return RISCV_EXCP_ILLEGAL_INST if check fails */
    ret = riscv_csr_accessible(env, csrno, write_mask);
    if (ret != RISCV_EXCP_NONE) {
#ifdef TARGET_CHERI
        if (ret == RISCV_EXCP_CHERI)
            raise_cheri_exception_impl(env, CapEx_AccessSystemRegsViolation,
                                       /*regnum=*/0, 0, true, retpc);
#endif
        return ret;
    }

    /* execute combined read/write operation if it exists */
    if (csr_ops[csrno].op) {
        ret = csr_ops[csrno].op(env, csrno, ret_value, new_value, write_mask);
#ifdef CONFIG_TCG_LOG_INSTR
        if (ret >= 0 && csr_ops[csrno].log_update) {
            csr_ops[csrno].log_update(env, csrno, new_value);
        }
#endif
        return ret;
    }

    /* if no accessor exists then return failure */
    if (!csr_ops[csrno].read) {
        return RISCV_EXCP_ILLEGAL_INST;
    }
    /* read old value */
    ret = csr_ops[csrno].read(env, csrno, &old_value);
    if (ret != RISCV_EXCP_NONE) {
        return ret;
    }

    /* write value if writable and write mask set, otherwise drop writes */
    if (write_mask) {
        new_value = (old_value & ~write_mask) | (new_value & write_mask);
        if (csr_ops[csrno].write) {
            ret = csr_ops[csrno].write(env, csrno, new_value);
            if (ret != RISCV_EXCP_NONE) {
                return ret;
            }
#ifdef CONFIG_TCG_LOG_INSTR
            if (csr_ops[csrno].log_update) {
                csr_ops[csrno].read(env, csrno, &new_value);
                csr_ops[csrno].log_update(env, csrno, new_value);
            }
#endif
        }
    }

    /* return old value */
    if (ret_value) {
        *ret_value = old_value;
    }

    return RISCV_EXCP_NONE;
}

/*
 * Debugger support.  If not in user mode, set env->debugger before the
 * riscv_csrrw call and clear it after the call.
 */
RISCVException riscv_csrrw_debug(CPURISCVState *env, int csrno,
                                 target_ulong *ret_value,
                                 target_ulong new_value,
                                 target_ulong write_mask)
{
    RISCVException ret;
#if !defined(CONFIG_USER_ONLY)
    env->debugger = true;
#endif
    ret = riscv_csrrw(env, csrno, ret_value, new_value, write_mask, 0);
#if !defined(CONFIG_USER_ONLY)
    env->debugger = false;
#endif
    return ret;
}


#ifdef CONFIG_TCG_LOG_INSTR
static void log_changed_csr_fn(CPURISCVState *env, int csrno,
                               target_ulong value)
{
    if (qemu_log_instr_enabled(env)) {
        qemu_log_instr_reg(env, csr_ops[csrno].name, value, csrno,
                           LRI_CSR_ACCESS);
    }
}
#else
#define log_changed_csr_fn (NULL)
#endif

/* Define csr_ops entry for read-only CSR register */
#define CSR_OP_FN_R(pred, readfn, csr_name)                        \
    {.predicate=pred, .read=readfn, .write=NULL, .op=NULL,         \
     .log_update=NULL, .name=csr_name}

/* Shorthand for functions following the read_<csr> pattern */
#define CSR_OP_R(pred, name)                                    \
    CSR_OP_FN_R(pred, glue(read_, name), stringify(name))

/* Internal - use CSR_OP_FN_RW, CSR_OP_RW and CSR_OP_NOLOG_RW */
#define _CSR_OP_FN_RW(pred, readfn, writefn, logfn, csr_name)      \
    {.predicate=pred, .read=readfn, .write=writefn,                \
     .op=NULL, .log_update=logfn, .name=csr_name}

/* Define csr_ops entry for read-write CSR register */
#define CSR_OP_FN_RW(pred, readfn, writefn, name)                  \
    _CSR_OP_FN_RW(pred, readfn, writefn, log_changed_csr_fn, name)

/* Shorthand for functions following the read/write_<csr> pattern */
#define CSR_OP_RW(pred, name)                                      \
    CSR_OP_FN_RW(pred, glue(read_, name), glue(write_, name),      \
                 stringify(name))

/*
 * Shorthand for functions following the read/write_<csr> pattern,
 * with custom write logging.
 */
#define CSR_OP_NOLOG_RW(pred, name)                                \
    _CSR_OP_FN_RW(pred, glue(read_, name), glue(write_, name),     \
                  NULL, stringify(name))

#define CSR_OP_NOLOG_FN_RW(pred, readfn, writefn, name)         \
    _CSR_OP_FN_RW(pred, readfn, writefn, NULL, stringify(name))

/* Define csr_ops entry for read-modify-write CSR register */
#define CSR_OP_RMW(pred, csr_name)                                 \
    {.predicate=pred, .read=NULL, .write=NULL,                     \
     .op=glue(rmw_, csr_name), .log_update=log_changed_csr_fn,     \
     .name=stringify(csr_name)}

/* Control and Status Register function table */
riscv_csr_operations csr_ops[CSR_TABLE_SIZE] = {
    /* User Floating-Point CSRs */
    [CSR_FFLAGS] =              CSR_OP_RW(fs, fflags),
    [CSR_FRM] =                 CSR_OP_RW(fs, frm),
    [CSR_FCSR] =                CSR_OP_RW(fs, fcsr),

    /* Vector CSRs */
    [CSR_VSTART] =              CSR_OP_RW(vs, vstart),
    [CSR_VXSAT] =               CSR_OP_RW(vs, vxsat),
    [CSR_VXRM] =                CSR_OP_RW(vs, vxrm),
    [CSR_VL] =                  CSR_OP_R(vs, vl),
    [CSR_VTYPE] =               CSR_OP_R(vs, vtype),
    /* User Timers and Counters */
    [CSR_CYCLE] =               CSR_OP_FN_R(ctr, read_instret, "cycle"),
    [CSR_INSTRET] =             CSR_OP_FN_R(ctr, read_instret, "instret"),
    [CSR_CYCLEH] =              CSR_OP_FN_R(ctr32, read_instreth, "cycleh"),
    [CSR_INSTRETH] =            CSR_OP_FN_R(ctr32, read_instreth, "instreth"),

    /*
     * In privileged mode, the monitor will have to emulate TIME CSRs only if
     * rdtime callback is not provided by machine/platform emulation.
     */
    [CSR_TIME] =                CSR_OP_R(ctr, time),
    [CSR_TIMEH] =               CSR_OP_R(ctr32, timeh),

#if !defined(CONFIG_USER_ONLY)
    /* Machine Timers and Counters */
    [CSR_MCYCLE] =              CSR_OP_FN_R(any, read_instret, "mcycle"),
    [CSR_MINSTRET] =            CSR_OP_FN_R(any, read_instret, "minstret"),
    [CSR_MCYCLEH] =             CSR_OP_FN_R(any32, read_instreth, "mcycleh"),
    [CSR_MINSTRETH] =           CSR_OP_FN_R(any32, read_instreth, "minstreth"),

    /* Machine Information Registers */
    [CSR_MVENDORID] =           CSR_OP_FN_R(any, read_zero, "mvendorid"),
    [CSR_MARCHID] =             CSR_OP_FN_R(any, read_zero, "marchid"),
    [CSR_MIMPID] =              CSR_OP_FN_R(any, read_zero, "mimppid"),
    [CSR_MHARTID] =             CSR_OP_R(any, mhartid),

    /* Machine Trap Setup */
    [CSR_MSTATUS] =             CSR_OP_RW(any, mstatus),
    [CSR_MISA] =                CSR_OP_RW(any, misa),
    [CSR_MIDELEG] =             CSR_OP_RW(any, mideleg),
    [CSR_MEDELEG] =             CSR_OP_RW(any, medeleg),
    [CSR_MIE] =                 CSR_OP_RW(any, mie),
    [CSR_MTVEC] =               CSR_OP_RW(any, mtvec),
    [CSR_MCOUNTEREN] =          CSR_OP_RW(any, mcounteren),

    [CSR_MSTATUSH] =            CSR_OP_RW(any32, mstatush),

    /* Machine Trap Handling */
    [CSR_MSCRATCH] =            CSR_OP_RW(any, mscratch),
    [CSR_MEPC] =                CSR_OP_RW(any, mepc),
    [CSR_MCAUSE] =              CSR_OP_RW(any, mcause),
    [CSR_MTVAL] =               CSR_OP_RW(any, mtval),
    [CSR_MIP] =                 CSR_OP_RMW(any, mip),

    /* Supervisor Trap Setup */
    [CSR_SSTATUS] =             CSR_OP_RW(smode, sstatus),
    [CSR_SIE] =                 CSR_OP_RW(smode, sie),
    [CSR_STVEC] =               CSR_OP_RW(smode, stvec),
    [CSR_SCOUNTEREN] =          CSR_OP_RW(smode, scounteren),

    /* Supervisor Trap Handling */
    [CSR_SSCRATCH] =            CSR_OP_RW(smode, sscratch),
    [CSR_SEPC] =                CSR_OP_RW(smode, sepc),
    [CSR_SCAUSE] =              CSR_OP_RW(smode, scause),
    [CSR_STVAL] =               CSR_OP_RW(smode, stval),
    [CSR_SIP] =                 CSR_OP_RMW(smode, sip),

    /* Supervisor Protection and Translation */
    [CSR_SATP] =                CSR_OP_RW(smode, satp),

    [CSR_HSTATUS] =             CSR_OP_RW(hmode, hstatus),
    [CSR_HEDELEG] =             CSR_OP_RW(hmode, hedeleg),
    [CSR_HIDELEG] =             CSR_OP_RW(hmode, hideleg),
    [CSR_HVIP] =                CSR_OP_RMW(hmode, hvip),
    [CSR_HIP] =                 CSR_OP_RMW(hmode, hip),
    [CSR_HIE] =                 CSR_OP_RW(hmode, hie),
    [CSR_HCOUNTEREN] =          CSR_OP_RW(hmode, hcounteren),
    [CSR_HGEIE] =               CSR_OP_FN_RW(hmode, read_zero, write_hgeie, "hgeie"),
    [CSR_HTVAL] =               CSR_OP_RW(hmode, htval),
    [CSR_HTINST] =              CSR_OP_RW(hmode, htinst),
    [CSR_HGEIP] =               CSR_OP_FN_RW(hmode, read_zero, write_hgeip, "hgeip"),
    [CSR_HGATP] =               CSR_OP_RW(hmode, hgatp),
    [CSR_HTIMEDELTA] =          CSR_OP_RW(hmode, htimedelta),
    [CSR_HTIMEDELTAH] =         CSR_OP_RW(hmode32, htimedeltah),

    [CSR_VSSTATUS] =            CSR_OP_RW(hmode, vsstatus),
    [CSR_VSIP] =                CSR_OP_RMW(hmode, vsip),
    [CSR_VSIE] =                CSR_OP_RW(hmode, vsie),
    [CSR_VSTVEC] =              CSR_OP_RW(hmode, vstvec),
    [CSR_VSSCRATCH] =           CSR_OP_RW(hmode, vsscratch),
    [CSR_VSEPC] =               CSR_OP_RW(hmode, vsepc),
    [CSR_VSCAUSE] =             CSR_OP_RW(hmode, vscause),
    [CSR_VSTVAL] =              CSR_OP_RW(hmode, vstval),
    [CSR_VSATP] =               CSR_OP_RW(hmode, vsatp),

#ifdef TARGET_CHERI_RISCV_STD_093
    [CSR_MTVAL2] =              CSR_OP_RW(any, mtval2),
#else
    [CSR_MTVAL2] =              CSR_OP_RW(hmode, mtval2),
#endif
    [CSR_MTINST] =              CSR_OP_RW(hmode, mtinst),

#ifdef TARGET_CHERI_RISCV_STD_093
    [CSR_STVAL2] =              CSR_OP_RW(any, stval2),
    [CSR_VSTVAL2] =             CSR_OP_RW(hmode, vstval2),
#endif

#ifdef TARGET_CHERI_RISCV_V9
    // CHERI CSRs: For now we always report enabled and dirty and don't support
    // turning off CHERI.  sccsr contains global capability load generation bits
    // that can be written, but the other two are constant.
    [CSR_UCCSR] =               CSR_OP_FN_RW(umode, read_ccsr, write_ccsr, "uccsr"),
    [CSR_SCCSR] =               CSR_OP_FN_RW(smode, read_ccsr, write_ccsr, "sccsr"),
    [CSR_MCCSR] =               CSR_OP_FN_RW(any, read_ccsr, write_ccsr, "mccsr"),
#endif

    [CSR_SENVCFG] =             CSR_OP_RW(any, senvcfg),
    [CSR_MENVCFG] =             CSR_OP_RW(any, menvcfg),
    /* Physical Memory Protection */
    [CSR_MSECCFG]    = CSR_OP_FN_RW(epmp, read_mseccfg, write_mseccfg, "mseccfg"),
    [CSR_PMPCFG0]    = CSR_OP_FN_RW(pmp, read_pmpcfg, write_pmpcfg, "pmpcfg0"),
    [CSR_PMPCFG1]    = CSR_OP_FN_RW(pmp, read_pmpcfg, write_pmpcfg, "pmpcfg1"),
    [CSR_PMPCFG2]    = CSR_OP_FN_RW(pmp, read_pmpcfg, write_pmpcfg, "pmpcfg2"),
    [CSR_PMPCFG3]    = CSR_OP_FN_RW(pmp, read_pmpcfg, write_pmpcfg, "pmpcfg3"),
    [CSR_PMPADDR0]   = CSR_OP_FN_RW(pmp, read_pmpaddr, write_pmpaddr, "pmpaddr0"),
    [CSR_PMPADDR1]   = CSR_OP_FN_RW(pmp, read_pmpaddr, write_pmpaddr, "pmpaddr1"),
    [CSR_PMPADDR2]   = CSR_OP_FN_RW(pmp, read_pmpaddr, write_pmpaddr, "pmpaddr2"),
    [CSR_PMPADDR3]   = CSR_OP_FN_RW(pmp, read_pmpaddr, write_pmpaddr, "pmpaddr3"),
    [CSR_PMPADDR4]   = CSR_OP_FN_RW(pmp, read_pmpaddr, write_pmpaddr, "pmpaddr4"),
    [CSR_PMPADDR5]   = CSR_OP_FN_RW(pmp, read_pmpaddr, write_pmpaddr, "pmpaddr5"),
    [CSR_PMPADDR6]   = CSR_OP_FN_RW(pmp, read_pmpaddr, write_pmpaddr, "pmpaddr6"),
    [CSR_PMPADDR7]   = CSR_OP_FN_RW(pmp, read_pmpaddr, write_pmpaddr, "pmpaddr7"),
    [CSR_PMPADDR8]   = CSR_OP_FN_RW(pmp, read_pmpaddr, write_pmpaddr, "pmpaddr8"),
    [CSR_PMPADDR9]   = CSR_OP_FN_RW(pmp, read_pmpaddr, write_pmpaddr, "pmpaddr9"),
    [CSR_PMPADDR10]  = CSR_OP_FN_RW(pmp, read_pmpaddr, write_pmpaddr, "pmpaddr10"),
    [CSR_PMPADDR11]  = CSR_OP_FN_RW(pmp, read_pmpaddr, write_pmpaddr, "pmpaddr11"),
    [CSR_PMPADDR12]  = CSR_OP_FN_RW(pmp, read_pmpaddr, write_pmpaddr, "pmpaddr12"),
    [CSR_PMPADDR13]  = CSR_OP_FN_RW(pmp, read_pmpaddr, write_pmpaddr, "pmpaddr13"),
    [CSR_PMPADDR14]  = CSR_OP_FN_RW(pmp, read_pmpaddr, write_pmpaddr, "pmpaddr14"),
    [CSR_PMPADDR15]  = CSR_OP_FN_RW(pmp, read_pmpaddr, write_pmpaddr, "pmpaddr15"),

    /* User Pointer Masking */
    [CSR_UMTE]    =    { "umte",    pointer_masking, read_umte,    write_umte    },
    [CSR_UPMMASK] =    { "upmmask", pointer_masking, read_upmmask, write_upmmask },
    [CSR_UPMBASE] =    { "upmbase", pointer_masking, read_upmbase, write_upmbase },
    /* Machine Pointer Masking */
    [CSR_MMTE]    =    { "mmte",    pointer_masking, read_mmte,    write_mmte    },
    [CSR_MPMMASK] =    { "mpmmask", pointer_masking, read_mpmmask, write_mpmmask },
    [CSR_MPMBASE] =    { "mpmbase", pointer_masking, read_mpmbase, write_mpmbase },
    /* Supervisor Pointer Masking */
    [CSR_SMTE]    =    { "smte",    pointer_masking, read_smte,    write_smte    },
    [CSR_SPMMASK] =    { "spmmask", pointer_masking, read_spmmask, write_spmmask },
    [CSR_SPMBASE] =    { "spmbase", pointer_masking, read_spmbase, write_spmbase },

    /* Performance Counters */
    [CSR_HPMCOUNTER3   ... CSR_HPMCOUNTER31] =    CSR_OP_FN_R(ctr, read_zero, "hpmcounterN"),
    [CSR_MHPMCOUNTER3  ... CSR_MHPMCOUNTER31] =   CSR_OP_FN_R(any, read_zero, "mhpmcounterN"),
    [CSR_MHPMEVENT3    ... CSR_MHPMEVENT31] =     CSR_OP_FN_R(any, read_zero, "mhpmeventN"),
    [CSR_HPMCOUNTER3H  ... CSR_HPMCOUNTER31H] =   CSR_OP_FN_R(ctr32, read_zero, "hpmcounterNh"),
    [CSR_MHPMCOUNTER3H ... CSR_MHPMCOUNTER31H] =  CSR_OP_FN_R(any32, read_zero, "mhpmcounterNh"),

#if !defined(TARGET_CHERI)
    [CSR_MTID] =                CSR_OP_RW(stid, mtid),
    [CSR_STID] =                CSR_OP_RW(stid, stid),
    [CSR_UTID] =                CSR_OP_RW(stid, utid),
    [CSR_VSTID] =               CSR_OP_RW(stid, vstid),
#endif /* !TARGET_CHERI */
#endif /* !CONFIG_USER_ONLY */
};

#ifdef TARGET_CHERI
/*
 * We don't have as many CSR Cap ops, and haven't fully defined what we need in
 * the table, so keep this table separate instead of merging it into the main
 * table for now.
 */

static riscv_csr_cap_ops csr_cap_ops[] = {
    { "mscratchc", CSR_MSCRATCHC, read_capcsr_reg, write_cap_csr_reg,
      CSR_OP_DIRECT_WRITE | CSR_OP_EXTENDED_REG },
    { "mtvecc", CSR_MTVECC, read_capcsr_reg, write_xtvecc,
      CSR_OP_IA_CONVERSION | CSR_OP_UPDATE_SCADDR | CSR_OP_EXTENDED_REG },
    { "stvecc", CSR_STVECC, read_capcsr_reg, write_xtvecc,
      CSR_OP_IA_CONVERSION | CSR_OP_UPDATE_SCADDR | CSR_OP_EXTENDED_REG },
    { "mepcc", CSR_MEPCC, read_xepcc, write_xepcc,
      CSR_OP_IA_CONVERSION | CSR_OP_EXTENDED_REG },
    { "sepcc", CSR_SEPCC, read_xepcc, write_xepcc,
      CSR_OP_IA_CONVERSION | CSR_OP_EXTENDED_REG },
    { "sscratchc", CSR_SSCRATCHC, read_capcsr_reg, write_cap_csr_reg,
      CSR_OP_DIRECT_WRITE | CSR_OP_EXTENDED_REG },
    { "ddc", CSR_DDC, read_capcsr_reg, write_cap_csr_reg,
      CSR_OP_REQUIRE_CRE | CSR_OP_IA_CONVERSION },
    { "mtidc", CSR_MTIDC, read_capcsr_reg, write_cap_csr_reg,
      CSR_OP_DIRECT_WRITE | CSR_OP_EXTENDED_REG },
    { "stidc", CSR_STIDC, read_capcsr_reg, write_cap_csr_reg,
      CSR_OP_DIRECT_WRITE | CSR_OP_EXTENDED_REG },
    { "utidc", CSR_UTIDC, read_capcsr_reg, write_cap_csr_reg,
      CSR_OP_DIRECT_WRITE | CSR_OP_EXTENDED_REG },
    { "vstidc", CSR_VSTIDC, read_capcsr_reg, write_cap_csr_reg,
      CSR_OP_DIRECT_WRITE | CSR_OP_EXTENDED_REG },
    { "vsepcc", CSR_VSEPCC, read_xepcc, write_xepcc,
      CSR_OP_IA_CONVERSION | CSR_OP_EXTENDED_REG },
    { "vsscratchc", CSR_VSSCRATCHC, read_capcsr_reg, write_cap_csr_reg,
      CSR_OP_DIRECT_WRITE | CSR_OP_EXTENDED_REG },
    { "vstvecc", CSR_VSTVECC, read_capcsr_reg, write_xtvecc,
      CSR_OP_IA_CONVERSION | CSR_OP_UPDATE_SCADDR | CSR_OP_EXTENDED_REG },
#ifdef TARGET_CHERI_RISCV_V9
    /* For backwards compatibility add the *tdc registers */
    { "mtdc", CSR_MTDC, read_capcsr_reg, write_cap_csr_reg,
      CSR_OP_REQUIRE_CRE },
    { "stdc", CSR_STDC, read_capcsr_reg, write_cap_csr_reg,
      CSR_OP_REQUIRE_CRE },
    { "vstdc", CSR_VSTDC, read_capcsr_reg, write_cap_csr_reg,
      CSR_OP_REQUIRE_CRE },
    { "pcc", CSR_PCC, read_capcsr_reg, /*write=*/NULL, CSR_OP_REQUIRE_CRE },
#endif
};

riscv_csr_cap_ops *get_csr_cap_info(uint32_t csrnum)
{
    for (int i = 0; i < ARRAY_SIZE(csr_cap_ops); i++) {
        if (csr_cap_ops[i].reg_num == csrnum)
            return &csr_cap_ops[i];
    }
    return NULL;
}
#endif
