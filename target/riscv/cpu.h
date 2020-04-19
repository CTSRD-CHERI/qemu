/*
 * QEMU RISC-V CPU
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

#ifndef RISCV_CPU_H
#define RISCV_CPU_H

#include "hw/core/cpu.h"
#include "exec/cpu-defs.h"
#include "exec/cpu_log_instr.h"
#include "qemu/units.h"
#include "fpu/softfloat-types.h"
#include "rvfi_dii.h"

#define TCG_GUEST_DEFAULT_MO 0

#define TYPE_RISCV_CPU "riscv-cpu"

#define RISCV_CPU_TYPE_SUFFIX "-" TYPE_RISCV_CPU
#define RISCV_CPU_TYPE_NAME(name) (name RISCV_CPU_TYPE_SUFFIX)
#define CPU_RESOLVING_TYPE TYPE_RISCV_CPU

#define TYPE_RISCV_CPU_ANY              RISCV_CPU_TYPE_NAME("any")
#define TYPE_RISCV_CPU_BASE32           RISCV_CPU_TYPE_NAME("rv32")
#define TYPE_RISCV_CPU_BASE64           RISCV_CPU_TYPE_NAME("rv64")
#define TYPE_RISCV_CPU_SIFIVE_E31       RISCV_CPU_TYPE_NAME("sifive-e31")
#define TYPE_RISCV_CPU_SIFIVE_E51       RISCV_CPU_TYPE_NAME("sifive-e51")
#define TYPE_RISCV_CPU_SIFIVE_U34       RISCV_CPU_TYPE_NAME("sifive-u34")
#define TYPE_RISCV_CPU_SIFIVE_U54       RISCV_CPU_TYPE_NAME("sifive-u54")
/* Deprecated */
#define TYPE_RISCV_CPU_RV32IMACU_NOMMU  RISCV_CPU_TYPE_NAME("rv32imacu-nommu")
#define TYPE_RISCV_CPU_RV32GCSU_V1_09_1 RISCV_CPU_TYPE_NAME("rv32gcsu-v1.9.1")
#define TYPE_RISCV_CPU_RV32GCSU_V1_10_0 RISCV_CPU_TYPE_NAME("rv32gcsu-v1.10.0")
#define TYPE_RISCV_CPU_RV64IMACU_NOMMU  RISCV_CPU_TYPE_NAME("rv64imacu-nommu")
#define TYPE_RISCV_CPU_RV64GCSU_V1_09_1 RISCV_CPU_TYPE_NAME("rv64gcsu-v1.9.1")
#define TYPE_RISCV_CPU_RV64GCSU_V1_10_0 RISCV_CPU_TYPE_NAME("rv64gcsu-v1.10.0")

#define RV32 ((target_ulong)1 << (TARGET_LONG_BITS - 2))
#define RV64 ((target_ulong)2 << (TARGET_LONG_BITS - 2))

#if defined(TARGET_RISCV32)
#define RVXLEN RV32
#elif defined(TARGET_RISCV64)
#define RVXLEN RV64
#endif

#define RV(x) ((target_ulong)1 << (x - 'A'))

#define RVI RV('I')
#define RVE RV('E') /* E and I are mutually exclusive */
#define RVM RV('M')
#define RVA RV('A')
#define RVF RV('F')
#define RVD RV('D')
#define RVC RV('C')
#define RVS RV('S')
#define RVU RV('U')
#define RVH RV('H')

/* S extension denotes that Supervisor mode exists, however it is possible
   to have a core that support S mode but does not have an MMU and there
   is currently no bit in misa to indicate whether an MMU exists or not
   so a cpu features bitfield is required, likewise for optional PMP support */
enum {
    RISCV_FEATURE_MMU,
    RISCV_FEATURE_PMP,
    RISCV_FEATURE_MISA
};

#define PRIV_VERSION_1_09_1 0x00010901
#define PRIV_VERSION_1_10_0 0x00011000
#define PRIV_VERSION_1_11_0 0x00011100

#define TRANSLATE_PMP_FAIL 2
#define TRANSLATE_FAIL 1
#define TRANSLATE_SUCCESS 0
#define MMU_USER_IDX 3

#define MAX_RISCV_PMPS (16)

typedef struct CPURISCVState CPURISCVState;

#ifdef TARGET_CHERI
#include "cheri-lazy-capregs-types.h"
#endif
#include "pmp.h"

struct CPURISCVState {
#ifdef TARGET_CHERI
    struct GPCapRegs gpcapregs;
#else
    target_ulong gpr[32];
#endif
    uint64_t fpr[32]; /* assume both F and D extensions */
#ifdef TARGET_CHERI
    cap_register_t PCC; // SCR 0 Program counter cap. (PCC) TODO: implement this properly
    cap_register_t DDC; // SCR 1 Default data cap. (DDC)
#else
    target_ulong pc;
#endif
#ifdef CONFIG_DEBUG_TCG
    target_ulong _pc_is_current;
#endif

    target_ulong load_res;
    target_ulong load_val;
#ifdef TARGET_CHERI
    target_ulong load_pesbt;
    bool load_tag;
#endif

    target_ulong frm;

    target_ulong badaddr;
    target_ulong guest_phys_fault_addr;
#ifdef TARGET_CHERI
    // The cause field reports the cause of the last capability exception,
    // following the encoding described in Table 3.9.2.
    // See enum CheriCapExc in cheri-archspecific.h
    uint8_t cap_cause; //
    // The cap idx field reports the index of the capability register that
    // caused the last ex- ception. When the most significant bit is set, the 5
    // least significant bits are used to index the special purpose capability
    // register file described in Table 5.3, otherwise, they index the
    // general-purpose capability register file.
    uint8_t cap_index;
#endif

    target_ulong priv_ver;
    target_ulong misa;
    target_ulong misa_mask;

    uint32_t features;

#ifdef CONFIG_USER_ONLY
    uint32_t elf_flags;
#endif

#ifndef CONFIG_USER_ONLY
    target_ulong priv;
    /* This contains QEMU specific information about the virt state. */
    target_ulong virt;
    target_ulong resetvec;

    target_ulong mhartid;
    target_ulong mstatus;

    target_ulong mip;

#ifdef TARGET_RISCV32
    target_ulong mstatush;
#endif

    uint32_t miclaim;

    target_ulong mie;
    target_ulong mideleg;

    target_ulong sptbr;  /* until: priv-1.9.1 */
    target_ulong satp;   /* since: priv-1.10.0 */
    target_ulong sbadaddr;
    target_ulong mbadaddr;
    target_ulong medeleg;

#ifdef TARGET_CHERI
    // XXX: not implemented properly
    cap_register_t UTCC; // SCR 4 User trap code cap. (UTCC)
    cap_register_t UTDC; // SCR 5 User trap data cap. (UTDC)
    cap_register_t UScratchC; // SCR 6 User scratch cap. (UScratchC)
    cap_register_t UEPCC; // SCR 7 User exception PC cap. (UEPCC)
#endif

#ifdef TARGET_CHERI
    cap_register_t STCC;      // SCR 12 Supervisor trap code cap. (STCC)
    cap_register_t STDC;      // SCR 13 Supervisor trap data cap. (STDC)
    cap_register_t SScratchC; // SCR 14 Supervisor scratch cap. (SScratchC)
    cap_register_t SEPCC;     // SCR 15 Supervisor exception PC cap. (SEPCC)
#else
    target_ulong stvec;
    target_ulong sepc;
#endif
    target_ulong scause;

#ifdef TARGET_CHERI
    cap_register_t MTCC;      // SCR 28 Machine trap code cap. (MTCC)
    cap_register_t MTDC;      // SCR 29 Machine trap data cap. (MTDC)
    cap_register_t MScratchC; // SCR 30 Machine scratch cap. (MScratchC)
    cap_register_t MEPCC;     // SCR 31 Machine exception PC cap. (MEPCC)
#else
    target_ulong mtvec;
    target_ulong mepc;
#endif
    target_ulong mcause;
    target_ulong mtval;  /* since: priv-1.10.0 */

    /* Hypervisor CSRs */
    target_ulong hstatus;
    target_ulong hedeleg;
    target_ulong hideleg;
    target_ulong hcounteren;
    target_ulong htval;
    target_ulong htinst;
    target_ulong hgatp;
    uint64_t htimedelta;

    /* Virtual CSRs */
#ifdef TARGET_CHERI
    cap_register_t VSTCC;
    cap_register_t VSTDC;
    cap_register_t VSScratchC;
    cap_register_t VSEPCC;
#else
    target_ulong vstvec;
    target_ulong vsepc;
#endif
    target_ulong vsstatus;
    target_ulong vsscratch;
    target_ulong vscause;
    target_ulong vstval;
    target_ulong vsatp;
#ifdef TARGET_RISCV32
    target_ulong vsstatush;
#endif

    target_ulong mtval2;
    target_ulong mtinst;

    /* HS Backup CSRs */
#ifdef TARGET_CHERI
    cap_register_t STCC_HS;
    cap_register_t SEPCC_HS;
#else
    target_ulong stvec_hs;
    target_ulong sepc_hs;
#endif
    target_ulong sscratch_hs;
    target_ulong scause_hs;
    target_ulong stval_hs;
    target_ulong satp_hs;
    target_ulong mstatus_hs;
#ifdef TARGET_RISCV32
    target_ulong mstatush_hs;
#endif

    target_ulong scounteren;
    target_ulong mcounteren;

    target_ulong sscratch;
    target_ulong mscratch;

    /* temporary htif regs */
    uint64_t mfromhost;
    uint64_t mtohost;
    uint64_t timecmp;

    /* physical memory protection */
    pmp_table_t pmp_state;

    /* machine specific rdtime callback */
    uint64_t (*rdtime_fn)(void);

    /* True if in debugger mode.  */
    bool debugger;
#endif

    float_status fp_status;

#ifdef CONFIG_RVFI_DII
    rvfi_dii_trace_t rvfi_dii_trace;
    bool rvfi_dii_have_injected_insn;
#endif
#ifdef TARGET_CHERI
    // Some statcounters:
    uint64_t statcounters_cap_read;
    uint64_t statcounters_cap_read_tagged;
    uint64_t statcounters_cap_write;
    uint64_t statcounters_cap_write_tagged;

    uint64_t statcounters_imprecise_setbounds;
    uint64_t statcounters_unrepresentable_caps;

#endif

    /* Fields from here on are preserved across CPU reset. */
    QEMUTimer *timer; /* Internal timer */
};

// From 5.3.6 Special Capability Registers (SCRs)
// Where an SCR extends a RISC-V CSR, e.g. MTCC extending mtvec, any read to the
// CSR shall return the offset of the corresponding SCR. Similarly, any write to
// the CSR shall set the offset of the SCR to the value written. This shall be
// equivalent to a CSetOffset instruction, but with any exception condition
// instead just clearing the tag of the SCR. This allows sealed capabilities to
// be held in SCRs without allowing them to be modified in a tag-preserving way,
// while also pre- venting exceptions when installing trap vectors: something
// that can be problematic where the task is delegated to a higher privilege
// level.
#ifdef TARGET_CHERI
#define GET_SPECIAL_REG(env, name, cheri_name)                                 \
    ((target_ulong)cap_get_offset(&((env)->cheri_name)))
void update_special_register_offset(CPURISCVState *env, cap_register_t *scr,
                                    const char *name, target_ulong value);
#define SET_SPECIAL_REG(env, name, cheri_name, value)                          \
    update_special_register_offset(env, &((env)->cheri_name), #cheri_name, value)

#define COPY_SPECIAL_REG(env, name, cheri_name, new_reg, new_cheri_reg)        \
    do {                                                                       \
        env->cheri_name = env->new_cheri_reg;                                  \
        log_changed_capreg(env, #cheri_name, &((env)->cheri_name));            \
    } while (false)
#else
#define GET_SPECIAL_REG(env, name, cheri_name) ((env)->name)
#define SET_SPECIAL_REG(env, name, cheri_name, value)                          \
    do {                                                                       \
        env->name = value;                                                     \
        log_changed_special_reg(env, #name, value);                            \
    } while (false)
#define COPY_SPECIAL_REG(env, name, cheri_name, new_reg, new_cheri_reg)        \
    do {                                                                       \
        env->name = env->new_reg;                                              \
        log_changed_special_reg(env, #name, ((env)->name));                    \
    } while (false)
#endif
#ifdef CONFIG_MIPS_LOG_INSTR
#define log_changed_special_reg(env, name, newval)                             \
    qemu_log_mask_and_addr(CPU_LOG_INSTR, cpu_get_recent_pc(env),              \
                           "  %s <- " TARGET_FMT_lx "\n", name, newval)
#else
#define log_changed_special_reg(env, name, newval) ((void)0)
#endif

static inline bool pc_is_current(CPURISCVState *env)
{
#ifdef CONFIG_DEBUG_TCG
    return env->_pc_is_current;
#else
    return true;
#endif
}

// Note: the pc does not have to be up-to-date, tb start is fine.
// We may miss a few dumps or print too many if -dfilter is on but
// that shouldn't really matter.
static inline target_ulong cpu_get_recent_pc(CPURISCVState *env) {
#ifdef TARGET_CHERI
    return env->PCC._cr_cursor;
#else
    return env->pc;
#endif
}

#define RISCV_CPU_CLASS(klass) \
    OBJECT_CLASS_CHECK(RISCVCPUClass, (klass), TYPE_RISCV_CPU)
#define RISCV_CPU(obj) \
    OBJECT_CHECK(RISCVCPU, (obj), TYPE_RISCV_CPU)
#define RISCV_CPU_GET_CLASS(obj) \
    OBJECT_GET_CLASS(RISCVCPUClass, (obj), TYPE_RISCV_CPU)

/**
 * RISCVCPUClass:
 * @parent_realize: The parent class' realize handler.
 * @parent_reset: The parent class' reset handler.
 *
 * A RISCV CPU model.
 */
typedef struct RISCVCPUClass {
    /*< private >*/
    CPUClass parent_class;
    /*< public >*/
    DeviceRealize parent_realize;
    DeviceReset parent_reset;
} RISCVCPUClass;

/**
 * RISCVCPU:
 * @env: #CPURISCVState
 *
 * A RISCV CPU.
 */
typedef struct RISCVCPU {
    /*< private >*/
    CPUState parent_obj;
    /*< public >*/
    CPUNegativeOffsetState neg;
    CPURISCVState env;

    /* Configuration Settings */
    struct {
        bool ext_i;
        bool ext_e;
        bool ext_g;
        bool ext_m;
        bool ext_a;
        bool ext_f;
        bool ext_d;
        bool ext_c;
        bool ext_s;
        bool ext_u;
        bool ext_h;
        bool ext_counters;
        bool ext_ifencei;
        bool ext_icsr;
#ifdef TARGET_CHERI
        bool ext_cheri;
#endif

        char *priv_spec;
        char *user_spec;
        bool mmu;
        bool pmp;
    } cfg;
} RISCVCPU;

static inline int riscv_has_ext(CPURISCVState *env, target_ulong ext)
{
    return (env->misa & ext) != 0;
}

static inline bool riscv_feature(CPURISCVState *env, int feature)
{
    return env->features & (1ULL << feature);
}

#include "cpu_user.h"
#include "cpu_bits.h"

extern const char * const riscv_int_regnames[];
extern const char * const riscv_fpr_regnames[];
extern const char * const riscv_excp_names[];
extern const char * const riscv_intr_names[];

#ifdef CONFIG_RVFI_DII
#define RVFI_DII_RAM_START 0x80000000
#define RVFI_DII_RAM_SIZE (8 * MiB)
#define RVFI_DII_RAM_END (RVFI_DII_RAM_START + RVFI_DII_RAM_SIZE)
void rvfi_dii_communicate(CPUState *cs, CPURISCVState *env);
#define rvfi_dii_offset(field)                                                 \
    offsetof(CPURISCVState, rvfi_dii_trace.rvfi_dii_##field)
#define gen_rvfi_dii_set_field(field, arg)                                     \
    tcg_gen_st_tl((TCGv)arg, cpu_env, rvfi_dii_offset(field))
#define gen_rvfi_dii_set_field_const(field, constant)                          \
    do {                                                                       \
        TCGv_i64 rvfi_tc = tcg_const_i64(constant);                            \
        tcg_gen_st_i64(rvfi_tc, cpu_env, rvfi_dii_offset(field));              \
        tcg_temp_free_i64(rvfi_tc);                                            \
    } while (0)
#else
#define gen_rvfi_dii_set_field(value, field) ((void)0)
#define gen_rvfi_dii_set_field_const(value, field) ((void)0)
#endif

void riscv_cpu_do_interrupt(CPUState *cpu);
int riscv_cpu_gdb_read_register(CPUState *cpu, GByteArray *buf, int reg);
int riscv_cpu_gdb_write_register(CPUState *cpu, uint8_t *buf, int reg);
bool riscv_cpu_exec_interrupt(CPUState *cs, int interrupt_request);
bool riscv_cpu_fp_enabled(CPURISCVState *env);
bool riscv_cpu_virt_enabled(CPURISCVState *env);
void riscv_cpu_set_virt_enabled(CPURISCVState *env, bool enable);
bool riscv_cpu_force_hs_excep_enabled(CPURISCVState *env);
void riscv_cpu_set_force_hs_excep(CPURISCVState *env, bool enable);
int riscv_cpu_mmu_index(CPURISCVState *env, bool ifetch);
hwaddr riscv_cpu_get_phys_page_debug(CPUState *cpu, vaddr addr);
#ifdef TARGET_CHERI
hwaddr cpu_riscv_translate_address_tagmem(CPURISCVState *env,
                                          target_ulong address,
                                          MMUAccessType rw, int reg, int *prot,
                                          uintptr_t retpc);
#endif
void  riscv_cpu_do_unaligned_access(CPUState *cs, vaddr addr,
                                    MMUAccessType access_type, int mmu_idx,
                                    uintptr_t retaddr);
bool riscv_cpu_tlb_fill(CPUState *cs, vaddr address, int size,
                        MMUAccessType access_type, int mmu_idx,
                        bool probe, uintptr_t retaddr);
void riscv_cpu_do_transaction_failed(CPUState *cs, hwaddr physaddr,
                                     vaddr addr, unsigned size,
                                     MMUAccessType access_type,
                                     int mmu_idx, MemTxAttrs attrs,
                                     MemTxResult response, uintptr_t retaddr);
char *riscv_isa_string(RISCVCPU *cpu);
void riscv_cpu_list(void);

#define cpu_signal_handler riscv_cpu_signal_handler
#define cpu_list riscv_cpu_list
#define cpu_mmu_index riscv_cpu_mmu_index

#ifndef CONFIG_USER_ONLY
void riscv_cpu_swap_hypervisor_regs(CPURISCVState *env);
int riscv_cpu_claim_interrupts(RISCVCPU *cpu, uint32_t interrupts);
uint32_t riscv_cpu_update_mip(RISCVCPU *cpu, uint32_t mask, uint32_t value);
#define BOOL_TO_MASK(x) (-!!(x)) /* helper for riscv_cpu_update_mip value */
void riscv_cpu_set_rdtime_fn(CPURISCVState *env, uint64_t (*fn)(void));
#endif
void riscv_cpu_set_mode(CPURISCVState *env, target_ulong newpriv);

void riscv_translate_init(void);
int riscv_cpu_signal_handler(int host_signum, void *pinfo, void *puc);
void QEMU_NORETURN riscv_raise_exception(CPURISCVState *env,
                                         uint32_t exception, uintptr_t pc);

target_ulong riscv_cpu_get_fflags(CPURISCVState *env);
void riscv_cpu_set_fflags(CPURISCVState *env, target_ulong);

int riscv_csrrw(CPURISCVState *env, int csrno, target_ulong *ret_value,
                target_ulong new_value, target_ulong write_mask, uintptr_t retpc);
int riscv_csrrw_debug(CPURISCVState *env, int csrno, target_ulong *ret_value,
                      target_ulong new_value, target_ulong write_mask);

static inline void riscv_csr_write(CPURISCVState *env, int csrno,
                                   target_ulong val, uintptr_t retpc)
{
    riscv_csrrw(env, csrno, NULL, val, MAKE_64BIT_MASK(0, TARGET_LONG_BITS), retpc);
}

static inline target_ulong riscv_csr_read(CPURISCVState *env, int csrno, uintptr_t retpc)
{
    target_ulong val = 0;
    riscv_csrrw(env, csrno, &val, 0, 0, retpc);
    return val;
}

typedef int (*riscv_csr_predicate_fn)(CPURISCVState *env, int csrno);
typedef int (*riscv_csr_read_fn)(CPURISCVState *env, int csrno,
    target_ulong *ret_value);
typedef int (*riscv_csr_write_fn)(CPURISCVState *env, int csrno,
    target_ulong new_value);
typedef int (*riscv_csr_op_fn)(CPURISCVState *env, int csrno,
    target_ulong *ret_value, target_ulong new_value, target_ulong write_mask);

typedef struct {
    riscv_csr_predicate_fn predicate;
    riscv_csr_read_fn read;
    riscv_csr_write_fn write;
    riscv_csr_op_fn op;
} riscv_csr_operations;

void riscv_get_csr_ops(int csrno, riscv_csr_operations *ops);
void riscv_set_csr_ops(int csrno, riscv_csr_operations *ops);

void riscv_cpu_register_gdb_regs_for_features(CPUState *cs);

typedef CPURISCVState CPUArchState;
typedef RISCVCPU ArchCPU;

#define TB_FLAGS_MMU_MASK   3
#define TB_FLAGS_MSTATUS_FS MSTATUS_FS

#include "exec/cpu-all.h"
#include "cpu_cheri.h"

static inline void
riscv_cpu_get_tb_cpu_state(CPURISCVState *env, target_ulong *pc,
                           target_ulong *cs_base, target_ulong *cs_top,
                           uint32_t *cheri_flags, uint32_t *flags)
{
    *pc = PC_ADDR(env); // We want the full virtual address here (no offset)
#ifdef CONFIG_USER_ONLY
    *flags = TB_FLAGS_MSTATUS_FS;
#else
    *flags = cpu_mmu_index(env, 0);
    if (riscv_cpu_fp_enabled(env)) {
        *flags |= env->mstatus & MSTATUS_FS;
    }
#endif
#ifdef TARGET_CHERI
    cheri_cpu_get_tb_cpu_state(&env->PCC, &env->DDC, cs_base, cs_top,
                               cheri_flags);
#else
    *cs_base = 0;
#endif
}
// Ugly macro hack to avoid having to modify cpu_get_tb_cpu_state in all targets
#define cpu_get_tb_cpu_state_6 riscv_cpu_get_tb_cpu_state

#ifdef CONFIG_CHERI_LOG_INSTR
/* TODO(am2419): Document these as required to support a new target.
 * New common log API arch-specific helpers.
 */
static inline bool cpu_in_user_mode(CPURISCVState *env)
{
    return false;
}

// TODO(am2419) should probably rename as cpu_get_asid()
static inline unsigned cheri_get_asid(CPURISCVState *env) {
    uint16_t ASID = 0; // TODO: implement?
    return ASID;
}
#endif

#endif /* RISCV_CPU_H */
