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
#include "hw/registerfields.h"
#include "exec/cpu-defs.h"
#include "qemu/units.h"
#include "qemu/cpu-float.h"
#include "qom/object.h"
#include "qemu/int128.h"
#include "cpu_bits.h"
#include "rvfi_dii.h"

#define TCG_GUEST_DEFAULT_MO 0

/*
 * RISC-V-specific extra insn start words:
 * 1: Original instruction opcode
 */
#define TARGET_INSN_START_EXTRA_WORDS 1

#define TYPE_RISCV_CPU "riscv-cpu"

#define RISCV_CPU_TYPE_SUFFIX "-" TYPE_RISCV_CPU
#define RISCV_CPU_TYPE_NAME(name) (name RISCV_CPU_TYPE_SUFFIX)
#define CPU_RESOLVING_TYPE TYPE_RISCV_CPU

#define TYPE_RISCV_CPU_ANY              RISCV_CPU_TYPE_NAME("any")
#define TYPE_RISCV_CPU_BASE32           RISCV_CPU_TYPE_NAME("rv32")
#define TYPE_RISCV_CPU_BASE64           RISCV_CPU_TYPE_NAME("rv64")
#define TYPE_RISCV_CPU_BASE128          RISCV_CPU_TYPE_NAME("x-rv128")
#define TYPE_RISCV_CPU_IBEX             RISCV_CPU_TYPE_NAME("lowrisc-ibex")
#define TYPE_RISCV_CPU_SHAKTI_C         RISCV_CPU_TYPE_NAME("shakti-c")
#define TYPE_RISCV_CPU_SIFIVE_E31       RISCV_CPU_TYPE_NAME("sifive-e31")
#define TYPE_RISCV_CPU_SIFIVE_E34       RISCV_CPU_TYPE_NAME("sifive-e34")
#define TYPE_RISCV_CPU_SIFIVE_E51       RISCV_CPU_TYPE_NAME("sifive-e51")
#define TYPE_RISCV_CPU_SIFIVE_U34       RISCV_CPU_TYPE_NAME("sifive-u34")
#define TYPE_RISCV_CPU_SIFIVE_U54       RISCV_CPU_TYPE_NAME("sifive-u54")
#define TYPE_RISCV_CPU_CODASIP_A730     RISCV_CPU_TYPE_NAME("codasip-a730")
#define TYPE_RISCV_CPU_CODASIP_L730     RISCV_CPU_TYPE_NAME("codasip-l730")
#define TYPE_RISCV_CPU_HOST             RISCV_CPU_TYPE_NAME("host")

#if defined(TARGET_RISCV32)
# define TYPE_RISCV_CPU_BASE            TYPE_RISCV_CPU_BASE32
#elif defined(TARGET_RISCV64)
# define TYPE_RISCV_CPU_BASE            TYPE_RISCV_CPU_BASE64
#endif

#define RV(x) ((target_ulong)1 << (x - 'A'))

#define RVI RV('I')
#define RVE RV('E') /* E and I are mutually exclusive */
#define RVM RV('M')
#define RVA RV('A')
#define RVF RV('F')
#define RVD RV('D')
#define RVV RV('V')
#define RVC RV('C')
#define RVS RV('S')
#define RVU RV('U')
#define RVH RV('H')
#define RVJ RV('J')

/* S extension denotes that Supervisor mode exists, however it is possible
   to have a core that support S mode but does not have an MMU and there
   is currently no bit in misa to indicate whether an MMU exists or not
   so a cpu features bitfield is required, likewise for optional PMP support */
enum {
    RISCV_FEATURE_MMU,
    RISCV_FEATURE_PMP,
    RISCV_FEATURE_EPMP,
    RISCV_FEATURE_MISA,
    RISCV_FEATURE_DEBUG,
    RISCV_FEATURE_CHERI,
    RISCV_FEATURE_CHERI_HYBRID,
    RISCV_FEATURE_STID,
};

/* Privileged specification version */
enum {
    PRIV_VERSION_1_10_0 = 0,
    PRIV_VERSION_1_11_0,
    PRIV_VERSION_1_12_0,
};

#define VEXT_VERSION_1_00_0 0x00010000

enum {
    TRANSLATE_SUCCESS,
    TRANSLATE_FAIL,
    TRANSLATE_PMP_FAIL,
    TRANSLATE_G_STAGE_FAIL,
#if defined(TARGET_CHERI) && !defined(TARGET_RISCV32)
    TRANSLATE_CHERI_FAIL,
#endif
};

#define MMU_USER_IDX 3

#define MAX_RISCV_PMPS (16)

typedef struct CPUArchState CPURISCVState;

#ifdef TARGET_CHERI
#include "cheri-lazy-capregs-types.h"
#endif

#if !defined(CONFIG_USER_ONLY)
#include "pmp.h"
#include "debug.h"
#endif

#define RV_VLEN_MAX 1024
#define RV_MAX_MHPMEVENTS 32
#define RV_MAX_MHPMCOUNTERS 32

FIELD(VTYPE, VLMUL, 0, 3)
FIELD(VTYPE, VSEW, 3, 3)
FIELD(VTYPE, VTA, 6, 1)
FIELD(VTYPE, VMA, 7, 1)
FIELD(VTYPE, VEDIV, 8, 2)
FIELD(VTYPE, RESERVED, 10, sizeof(target_ulong) * 8 - 11)

typedef struct PMUCTRState {
    /* Current value of a counter */
    target_ulong mhpmcounter_val;
    /* Current value of a counter in RV32*/
    target_ulong mhpmcounterh_val;
    /* Snapshot values of counter */
    target_ulong mhpmcounter_prev;
    /* Snapshort value of a counter in RV32 */
    target_ulong mhpmcounterh_prev;
    bool started;
    /* Value beyond UINT32_MAX/UINT64_MAX before overflow interrupt trigger */
    target_ulong irq_overflow_left;
} PMUCTRState;

struct CPUArchState {
#ifdef TARGET_CHERI
    struct GPCapRegs gpcapregs;
#else
    target_ulong gpr[32];
    target_ulong gprh[32]; /* 64 top bits of the 128-bit registers */
#endif
    uint64_t fpr[32]; /* assume both F and D extensions */

    /* vector coprocessor state. */
    uint64_t vreg[32 * RV_VLEN_MAX / 64] QEMU_ALIGNED(16);
    target_ulong vxrm;
    target_ulong vxsat;
    target_ulong vl;
    target_ulong vstart;
    target_ulong vtype;
    bool vill;

#ifdef TARGET_CHERI
    cap_register_t pcc; // SCR 0 Program counter cap. (PCC) TODO: implement this properly
    cap_register_t ddc; // SCR 1 Default data cap. (DDC)
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
    target_ulong bins;

    target_ulong guest_phys_fault_addr;

#ifdef TARGET_CHERI
    // The cause field reports the cause of the last capability exception,
    // following the encoding described in Table 3.9.2.
    // See enum CheriCapExc in cheri-archspecific.h
    uint8_t last_cap_cause; // Used to populate xtval
    // The cap idx field reports the index of the capability register that
    // caused the last exception. When the most significant bit is set, the 5
    // least significant bits are used to index the special purpose capability
    // register file described in Table 5.3, otherwise, they index the
    // general-purpose capability register file.
    uint8_t last_cap_index; /* Used to populate xtval in v9 (now just debug). */
#ifdef TARGET_CHERI_RISCV_STD_093
    /* Cheri093CapExcType */ uint8_t last_cap_type; /* To populate xtval2 */
#endif
#else
    /* 128-bit helpers upper part return value */
    target_ulong retxh;
#endif

#ifdef CONFIG_USER_ONLY
    uint32_t elf_flags;
#endif

#ifndef CONFIG_USER_ONLY
    target_ulong priv;
    /* This contains QEMU specific information about the virt state. */
    target_ulong virt;
    target_ulong geilen;
    uint64_t resetvec;

    /*
     * For RV32 this is 32-bit mstatus and 32-bit mstatush.
     * For RV64 this is a 64-bit mstatus.
     */
    uint64_t mstatus;

    uint64_t mip;
    /*
     * MIP contains the software writable version of SEIP ORed with the
     * external interrupt value. The MIP register is always up-to-date.
     * To keep track of the current source, we also save booleans of the values
     * here.
     */
    bool external_seip;
    bool software_seip;

    uint64_t miclaim;

    uint64_t mie;
    uint64_t mideleg;

    target_ulong satp;   /* since: priv-1.10.0 */
    target_ulong stval;
    target_ulong medeleg;

#if defined(TARGET_CHERI) && !defined(TARGET_RISCV32)
    target_ulong sccsr;
#endif

#ifdef TARGET_CHERI
    cap_register_t stvecc;    // SCR 12 Supervisor trap code cap. (STCC)
    cap_register_t sscratchc; // SCR 14 Supervisor scratch cap. (SScratchC)
    cap_register_t sepcc;     // SCR 15 Supervisor exception PC cap. (SEPCC)
#else
    target_ulong stvec;
    target_ulong sepc;
    target_ulong sscratch;
#endif
    target_ulong scause;

#ifdef TARGET_CHERI
    cap_register_t mtvecc;    // SCR 28 Machine trap code cap. (MTCC)
    cap_register_t mscratchc; // SCR 30 Machine scratch cap. (MScratchC)
    cap_register_t mepcc;     // SCR 31 Machine exception PC cap. (MEPCC)
#else
    target_ulong mtvec;
    target_ulong mepc;
    target_ulong mscratch;
#endif
    target_ulong mcause;
    target_ulong mtval;  /* since: priv-1.10.0 */

    /* Machine and Supervisor interrupt priorities */
    uint8_t miprio[64];
    uint8_t siprio[64];

    /* AIA CSRs */
    target_ulong miselect;
    target_ulong siselect;

    /* Hypervisor CSRs */
    target_ulong hstatus;
    target_ulong hedeleg;
    uint64_t hideleg;
    target_ulong hcounteren;
    target_ulong htval;
    target_ulong htinst;
    target_ulong hgatp;
    target_ulong hgeie;
    target_ulong hgeip;
    uint64_t htimedelta;

    /* Hypervisor controlled virtual interrupt priorities */
    target_ulong hvictl;
    uint8_t hviprio[64];

    /* Virtual CSRs */
#ifdef TARGET_CHERI
    cap_register_t vstvecc;
    cap_register_t vsscratchc;
    cap_register_t vsepcc;
#else
    /* Upper 64-bits of 128-bit CSRs */
    uint64_t mscratchh;
    uint64_t sscratchh;

    target_ulong vstvec;
    target_ulong vsepc;
    target_ulong vsscratch;
#endif
    /*
     * For RV32 this is 32-bit vsstatus and 32-bit vsstatush.
     * For RV64 this is a 64-bit vsstatus.
     */
    uint64_t vsstatus;
    target_ulong vscause;
    target_ulong vstval;
    target_ulong vsatp;

    /* AIA VS-mode CSRs */
    target_ulong vsiselect;

    target_ulong mtval2;
    target_ulong mtinst;

    /* HS Backup CSRs */
#ifdef TARGET_CHERI
    cap_register_t stcc_hs;
    cap_register_t sepcc_hs;
    cap_register_t sscratchc_hs;

#ifdef TARGET_CHERI_RISCV_STD_093
    target_ulong stval2;
    target_ulong vstval2;
    target_ulong stval2_hs;
#endif
#else
    target_ulong stvec_hs;
    target_ulong sepc_hs;
    target_ulong sscratch_hs;
#endif
    target_ulong scause_hs;
    target_ulong stval_hs;
    target_ulong satp_hs;
    uint64_t mstatus_hs;

    /* Signals whether the current exception occurred with two-stage address
       translation active. */
    bool two_stage_lookup;
    /*
     * Signals whether the current exception occurred while doing two-stage
     * address translation for the VS-stage page table walk.
     */
    bool two_stage_indirect_lookup;

    target_ulong scounteren;
    target_ulong mcounteren;

    target_ulong mcountinhibit;

    /* PMU counter state */
    PMUCTRState pmu_ctrs[RV_MAX_MHPMCOUNTERS];

    /* PMU event selector configured values. First three are unused*/
    target_ulong mhpmevent_val[RV_MAX_MHPMEVENTS];

#ifdef TARGET_CHERI
    /* zstid registers */
    cap_register_t mtidc;
    cap_register_t stidc;
    cap_register_t utidc;
    cap_register_t vstidc;
    cap_register_t stidc_hs;
#else
    /* zstid registers in integer mode */
    target_ulong mtid;
    target_ulong stid;
    target_ulong utid;
    target_ulong vstid;
    target_ulong stid_hs;
#endif

#ifdef TARGET_CHERI_RISCV_V9
    /* V9-only special capability registers */
    cap_register_t mtdc;  /* Machine trap data cap */
    cap_register_t stdc;  /* Supervisor trap data cap */
    cap_register_t vstdc; /* Virtual Supervisor trap data cap */
#endif

    /* PMU event selector configured values for RV32*/
    target_ulong mhpmeventh_val[RV_MAX_MHPMEVENTS];

    /* temporary htif regs */
    uint64_t mfromhost;
    uint64_t mtohost;

    /* Sstc CSRs */
    uint64_t stimecmp;

    uint64_t vstimecmp;

    /* physical memory protection */
    pmp_table_t pmp_state;
    target_ulong mseccfg;

    /* trigger module */
    target_ulong trigger_cur;
    target_ulong tdata1[RV_MAX_TRIGGERS];
    target_ulong tdata2[RV_MAX_TRIGGERS];
    target_ulong tdata3[RV_MAX_TRIGGERS];
    struct CPUBreakpoint *cpu_breakpoint[RV_MAX_TRIGGERS];
    struct CPUWatchpoint *cpu_watchpoint[RV_MAX_TRIGGERS];

    /* machine specific rdtime callback */
    uint64_t (*rdtime_fn)(void *);
    void *rdtime_fn_arg;

    /* machine specific AIA ireg read-modify-write callback */
#define AIA_MAKE_IREG(__isel, __priv, __virt, __vgein, __xlen) \
    ((((__xlen) & 0xff) << 24) | \
     (((__vgein) & 0x3f) << 20) | \
     (((__virt) & 0x1) << 18) | \
     (((__priv) & 0x3) << 16) | \
     (__isel & 0xffff))
#define AIA_IREG_ISEL(__ireg)                  ((__ireg) & 0xffff)
#define AIA_IREG_PRIV(__ireg)                  (((__ireg) >> 16) & 0x3)
#define AIA_IREG_VIRT(__ireg)                  (((__ireg) >> 18) & 0x1)
#define AIA_IREG_VGEIN(__ireg)                 (((__ireg) >> 20) & 0x3f)
#define AIA_IREG_XLEN(__ireg)                  (((__ireg) >> 24) & 0xff)
    int (*aia_ireg_rmw_fn[4])(void *arg, target_ulong reg,
        target_ulong *val, target_ulong new_val, target_ulong write_mask);
    void *aia_ireg_rmw_fn_arg[4];

    /* True if in debugger mode.  */
    bool debugger;

    /*
     * CSRs for PointerMasking extension
     */
    target_ulong mmte;
    target_ulong mpmmask;
    target_ulong mpmbase;
    target_ulong spmmask;
    target_ulong spmbase;
    target_ulong upmmask;
    target_ulong upmbase;

    /* CSRs for execution enviornment configuration */
    uint64_t menvcfg;
    target_ulong senvcfg;
    uint64_t henvcfg;
#endif
    target_ulong cur_pmmask;
    target_ulong cur_pmbase;

    float_status fp_status;

#ifdef TARGET_CHERI
    // Some statcounters:
    uint64_t statcounters_cap_read;
    uint64_t statcounters_cap_read_tagged;
    uint64_t statcounters_cap_write;
    uint64_t statcounters_cap_write_tagged;

    uint64_t statcounters_imprecise_setbounds;
    uint64_t statcounters_unrepresentable_caps;

#endif

    /* Fields up to this point are cleared by a TestRIG reset */
    struct {} end_testrig_reset_fields;


#ifdef CONFIG_RVFI_DII
    struct {
        struct rvfi_dii_instruction_metadata INST;
        struct rvfi_dii_pc_data PC;
        struct rvfi_dii_integer_data INTEGER;
        struct rvfi_dii_memory_access_data MEM;
        // TODO: struct rvfi_dii_csr_data CSR;
        // TODO: struct rvfi_dii_fp_data FP;
        // TODO: struct rvfi_dii_cheri_data CHERI;
        // TODO: struct rvfi_dii_cheri_scr_data CHERI_SCR;
        // TODO: struct rvfi_dii_trap_data TRAP;
        uint32_t available_fields;
    } rvfi_dii_trace;
    bool rvfi_dii_have_injected_insn;
    uint32_t rvfi_dii_injected_insn;
#endif

    target_ulong priv_ver;
    target_ulong bext_ver;
    target_ulong vext_ver;

    /* RISCVMXL, but uint32_t for vmstate migration */
    uint32_t misa_mxl;      /* current mxl */
    uint32_t misa_mxl_max;  /* max mxl for this cpu */
    uint32_t misa_ext;      /* current extensions */
    uint32_t misa_ext_mask; /* max ext for this cpu */
    uint32_t xl;            /* current xlen */

    target_ulong mhartid;

    uint32_t features;

#ifdef CONFIG_USER_ONLY
    uint32_t elf_flags;
#endif


    /* Fields from here on are preserved across CPU reset. */
    QEMUTimer *stimer; /* Internal timer for S-mode interrupt */
    QEMUTimer *vstimer; /* Internal timer for VS-mode interrupt */
    bool vstime_irq;

    hwaddr kernel_addr;
    hwaddr fdt_addr;

    /* kvm timer */
    bool kvm_timer_dirty;
    uint64_t kvm_timer_time;
    uint64_t kvm_timer_compare;
    uint64_t kvm_timer_state;
    uint64_t kvm_timer_frequency;
};

OBJECT_DECLARE_CPU_TYPE(RISCVCPU, RISCVCPUClass, RISCV_CPU)

/**
 * RISCVCPUClass:
 * @parent_realize: The parent class' realize handler.
 * @parent_reset: The parent class' reset handler.
 *
 * A RISCV CPU model.
 */
struct RISCVCPUClass {
    /*< private >*/
    CPUClass parent_class;
    /*< public >*/
    DeviceRealize parent_realize;
    DeviceReset parent_reset;
};

struct RISCVCPUConfig {
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
    bool ext_j;
    bool ext_v;
    bool ext_zba;
    bool ext_zbb;
    bool ext_zbc;
    bool ext_zbkb;
    bool ext_zbkc;
    bool ext_zbkx;
    bool ext_zbs;
    bool ext_zca;
    bool ext_zcb;
    bool ext_zcd;
    bool ext_zcf;
    bool ext_zk;
    bool ext_zkn;
    bool ext_zknd;
    bool ext_zkne;
    bool ext_zknh;
    bool ext_zkr;
    bool ext_zks;
    bool ext_zksed;
    bool ext_zksh;
    bool ext_zkt;
    bool ext_ifencei;
    bool ext_zihintpause;
    bool ext_icsr;
    bool ext_sstc;
    bool ext_svinval;
#if !defined(TARGET_CHERI_RISCV_V9)
    /* Incompatible with ISAv9 */
    bool ext_svnapot;
    bool ext_svpbmt;
#endif
    bool ext_zdinx;
    bool ext_zfh;
    bool ext_zfhmin;
    bool ext_zfinx;
    bool ext_zhinx;
    bool ext_zhinxmin;
    bool ext_zve32f;
    bool ext_zve64f;
    bool ext_zmmul;
    bool ext_smaia;
    bool ext_ssaia;
    bool ext_sscofpmf;
    bool rvv_ta_all_1s;
    bool rvv_ma_all_1s;

    bool ext_icbom;
    bool ext_icboz;
#if defined(TARGET_CHERI_RISCV_STD_093)
    bool ext_zish4add;
#endif

#ifdef TARGET_CHERI
    bool ext_cheri;
#ifdef TARGET_CHERI_RISCV_V9
    bool ext_cheri_v9; /* Temporary flag to support new semantics. */
#elif defined(TARGET_CHERI_RISCV_STD)
    bool ext_zyhybrid;
    bool ext_zylevels1;
    /* number of levels (Zcherilevels): 0 invalid, 1 disabled (default)) */
    uint8_t _compat_cheri_levels; /* To keep existing scripts working */
    uint8_t lvbits; /* Only 0 and 1 (Zylevels1) are currently supported. */
    bool cheri_pte;
#endif
#endif

    uint32_t mvendorid;
    uint64_t marchid;
    uint64_t mimpid;

    /* Vendor-specific custom extensions */
    bool ext_XVentanaCondOps;

    uint8_t pmu_num;
    char *priv_spec;
    char *user_spec;
    char *bext_spec;
    char *vext_spec;
    uint16_t vlen;
    uint16_t elen;
    uint16_t cbom_blocksize;
    uint16_t cboz_blocksize;
    bool mmu;
    bool pmp;
    bool epmp;
    bool debug;

    bool short_isa_string;
};

typedef struct RISCVCPUConfig RISCVCPUConfig;

/**
 * RISCVCPU:
 * @env: #CPURISCVState
 *
 * A RISCV CPU.
 */
struct ArchCPU {
    /*< private >*/
    CPUState parent_obj;
    /*< public >*/
    CPUNegativeOffsetState neg;
    CPURISCVState env;

    char *dyn_csr_xml;
    char *dyn_vreg_xml;
#ifdef TARGET_CHERI_RISCV_STD
    char *dyn_ycsr_xml;
#elif defined(TARGET_CHERI_RISCV_V9)
    char *dyn_scr_xml;
#endif

    /* Configuration Settings */
    RISCVCPUConfig cfg;

    QEMUTimer *pmu_timer;
    /* A bitmask of Available programmable counters */
    uint32_t pmu_avail_ctrs;
    /* Mapping of events to counters */
    GHashTable *pmu_event_ctr_map;
};

static inline int riscv_has_ext(CPURISCVState *env, target_ulong ext)
{
    return (env->misa_ext & ext) != 0;
}

static inline bool riscv_feature(CPURISCVState *env, int feature)
{
    return env->features & (1ULL << feature);
}

static inline void riscv_set_feature(CPURISCVState *env, int feature)
{
    env->features |= (1ULL << feature);
}

#include "cpu_user.h"

extern const char * const riscv_int_regnames[];
extern const char * const riscv_int_regnamesh[];
extern const char * const riscv_fpr_regnames[];
#ifdef TARGET_CHERI
/* Needed for cheri-common logging */
extern const char * const cheri_gp_regnames[];
#endif

#ifdef CONFIG_TCG_LOG_INSTR
void riscv_log_instr_csr_changed(CPURISCVState *env, int csrno);

#define log_changed_special_reg(env, name, newval, index, type)                \
    do {                                                                       \
        if (qemu_log_instr_enabled(env))                                       \
            qemu_log_instr_reg(env, name, newval, index, type);                \
    } while (0)
#else /* !CONFIG_TCG_LOG_INSTR */
#define log_changed_special_reg(env, name, newval) ((void)0)
#define riscv_log_instr_csr_changed(env, csrno) ((void)0)
#endif /* !CONFIG_TCG_LOG_INSTR */

#define CHK_BLK_POW2(prop) \
do { \
    if ((cpu->cfg.prop == 0) || \
            (cpu->cfg.prop & (cpu->cfg.prop - 1))) { \
        error_setg(errp, "%s must be a power of 2.", tostring(prop)); \
        return; \
    } \
} while (0)

/*
 * From 5.3.6 Special Capability Registers (SCRs)
 * Where an SCR extends a RISC-V CSR, e.g. MTCC extending mtvec, any read to the
 * CSR shall return the address (offset for ISAv8) of the corresponding SCR.
 * Similarly, any write to the CSR shall set the address (offset for ISAv8) of
 * the SCR to the value written.
 */
#ifdef TARGET_CHERI
#define SCR_TO_PROGRAM_COUNTER(env, scr)                                       \
    (CHERI_NO_RELOCATION(env) ? cap_get_cursor(scr)                            \
                              : (target_ulong)cap_get_offset(scr))
/**
 * @returns the architectural view of the underlying SCR,address/offset
 * depending on CHERI ISA version.
 */
#define GET_SPECIAL_REG_ARCH(env, name, cheri_name)                            \
    SCR_TO_PROGRAM_COUNTER(env, &((env)->cheri_name))
/**
 * @returns the address of a given SCR as we feed our PC around as an address
 * not the architectural offset.
 */
#define GET_SPECIAL_REG_ADDR(env, name, cheri_name)                            \
    ((target_ulong)cap_get_cursor(&((env)->cheri_name)))
void update_special_register(CPURISCVState *env, cap_register_t *scr,
                             const char *name, target_ulong value);
#define SCR_SET_PROGRAM_COUNTER(env, scr, name, value)                         \
    update_special_register(env, scr, name, value)
#define SET_SPECIAL_REG(env, name, cheri_name, value)                          \
    SCR_SET_PROGRAM_COUNTER(env, &((env)->cheri_name), #cheri_name, value)

#else /* ! TARGET_CHERI */
#define GET_SPECIAL_REG_ARCH(env, name, cheri_name) ((env)->name)
#define GET_SPECIAL_REG_ADDR(env, name, cheri_name) ((env)->name)
#define SET_SPECIAL_REG(env, name, cheri_name, value)                          \
    do {                                                                       \
        env->name = value;                                                     \
        log_changed_special_reg(env, #name, value, 0, LRI_CSR_ACCESS);         \
    } while (false)
#endif /* ! TARGET_CHERI */

const char *riscv_cpu_get_trap_name(target_ulong cause, bool async);
void riscv_cpu_do_interrupt(CPUState *cpu);
int riscv_cpu_write_elf64_note(WriteCoreDumpFunction f, CPUState *cs,
                               int cpuid, DumpState *s);
int riscv_cpu_write_elf32_note(WriteCoreDumpFunction f, CPUState *cs,
                               int cpuid, DumpState *s);
int riscv_cpu_gdb_read_register(CPUState *cpu, GByteArray *buf, int reg);
int riscv_cpu_gdb_write_register(CPUState *cpu, uint8_t *buf, int reg);
int riscv_cpu_hviprio_index2irq(int index, int *out_irq, int *out_rdzero);
uint8_t riscv_cpu_default_priority(int irq);
uint64_t riscv_cpu_all_pending(CPURISCVState *env);
int riscv_cpu_mirq_pending(CPURISCVState *env);
int riscv_cpu_sirq_pending(CPURISCVState *env);
int riscv_cpu_vsirq_pending(CPURISCVState *env);
bool riscv_cpu_fp_enabled(CPURISCVState *env);
target_ulong riscv_cpu_get_geilen(CPURISCVState *env);
void riscv_cpu_set_geilen(CPURISCVState *env, target_ulong geilen);
bool riscv_cpu_vector_enabled(CPURISCVState *env);
bool riscv_cpu_virt_enabled(CPURISCVState *env);
void riscv_cpu_set_virt_enabled(CPURISCVState *env, bool enable);
bool riscv_cpu_two_stage_lookup(int mmu_idx);
int riscv_cpu_mmu_index(CPURISCVState *env, bool ifetch);
hwaddr riscv_cpu_get_phys_page_debug(CPUState *cpu, vaddr addr);
#ifdef TARGET_CHERI
hwaddr cpu_riscv_translate_address_tagmem(CPURISCVState *env,
                                          target_ulong address,
                                          MMUAccessType rw, int reg, int *prot,
                                          uintptr_t retpc);
#endif
G_NORETURN void  riscv_cpu_do_unaligned_access(CPUState *cs, vaddr addr,
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

#ifdef TARGET_CHERI
static inline bool riscv_cpu_mode_cre(CPURISCVState *env);
#endif

#define cpu_list riscv_cpu_list
#define cpu_mmu_index riscv_cpu_mmu_index

#ifndef CONFIG_USER_ONLY
bool riscv_cpu_exec_interrupt(CPUState *cs, int interrupt_request);
void riscv_cpu_swap_hypervisor_regs(CPURISCVState *env, bool hs_mode_trap);
int riscv_cpu_claim_interrupts(RISCVCPU *cpu, uint64_t interrupts);
uint64_t riscv_cpu_update_mip(RISCVCPU *cpu, uint64_t mask, uint64_t value);
#define BOOL_TO_MASK(x) (-!!(x)) /* helper for riscv_cpu_update_mip value */
void riscv_cpu_set_rdtime_fn(CPURISCVState *env, uint64_t (*fn)(void *),
                             void *arg);
void riscv_cpu_set_aia_ireg_rmw_fn(CPURISCVState *env, uint32_t priv,
                                   int (*rmw_fn)(void *arg,
                                                 target_ulong reg,
                                                 target_ulong *val,
                                                 target_ulong new_val,
                                                 target_ulong write_mask),
                                   void *rmw_fn_arg);
#endif
void riscv_cpu_set_mode(CPURISCVState *env, target_ulong newpriv);

void riscv_translate_init(void);
G_NORETURN void riscv_raise_exception(CPURISCVState *env,
                                      uint32_t exception, uintptr_t pc);

target_ulong riscv_cpu_get_fflags(CPURISCVState *env);
void riscv_cpu_set_fflags(CPURISCVState *env, target_ulong);
bool csr_needs_asr(uint32_t csrno, bool write);

#define TB_FLAGS_PRIV_MMU_MASK                3
#define TB_FLAGS_PRIV_HYP_ACCESS_MASK   (1 << 2)
#define TB_FLAGS_MSTATUS_FS MSTATUS_FS
#define TB_FLAGS_MSTATUS_VS MSTATUS_VS

#include "exec/cpu-all.h"
#include "cpu_cheri.h"

static inline bool pc_is_current(CPURISCVState *env)
{
#ifdef CONFIG_DEBUG_TCG
    return env->_pc_is_current;
#else
    return true;
#endif
}

/*
 * Note: the pc does not have to be up-to-date, tb start is fine.
 * We may miss a few dumps or print too many if -dfilter is on but
 * that shouldn't really matter.
 */
static inline target_ulong cpu_get_recent_pc(CPURISCVState *env)
{
#ifdef TARGET_CHERI
    return env->pcc._cr_cursor;
#else
    return env->pc;
#endif
}

FIELD(TB_FLAGS, MEM_IDX, 0, 3)
FIELD(TB_FLAGS, LMUL, 3, 3)
FIELD(TB_FLAGS, SEW, 6, 3)
/* Skip MSTATUS_VS (0x600) bits */
FIELD(TB_FLAGS, VL_EQ_VLMAX, 11, 1)
FIELD(TB_FLAGS, VILL, 12, 1)
/* Skip MSTATUS_FS (0x6000) bits */
/* Is a Hypervisor instruction load/store allowed? */
FIELD(TB_FLAGS, HLSX, 15, 1)
FIELD(TB_FLAGS, MSTATUS_HS_FS, 16, 2)
FIELD(TB_FLAGS, MSTATUS_HS_VS, 18, 2)
/* The combination of MXL/SXL/UXL that applies to the current cpu mode. */
FIELD(TB_FLAGS, XL, 20, 2)
/* If PointerMasking should be applied */
FIELD(TB_FLAGS, PM_MASK_ENABLED, 22, 1)
FIELD(TB_FLAGS, PM_BASE_ENABLED, 23, 1)
FIELD(TB_FLAGS, VTA, 24, 1)
FIELD(TB_FLAGS, VMA, 25, 1)

#ifdef TARGET_RISCV32
#define riscv_cpu_mxl(env)  ((void)(env), MXL_RV32)
#else
static inline RISCVMXL riscv_cpu_mxl(CPURISCVState *env)
{
    return env->misa_mxl;
}
#endif
#define riscv_cpu_mxl_bits(env) (1UL << (4 + riscv_cpu_mxl(env)))

#if defined(TARGET_RISCV32)
#define cpu_recompute_xl(env)  ((void)(env), MXL_RV32)
#else
static inline RISCVMXL cpu_recompute_xl(CPURISCVState *env)
{
    RISCVMXL xl = env->misa_mxl;
#if !defined(CONFIG_USER_ONLY)
    /*
     * When emulating a 32-bit-only cpu, use RV32.
     * When emulating a 64-bit cpu, and MXL has been reduced to RV32,
     * MSTATUSH doesn't have UXL/SXL, therefore XLEN cannot be widened
     * back to RV64 for lower privs.
     */
    if (xl != MXL_RV32) {
        switch (env->priv) {
        case PRV_M:
            break;
        case PRV_U:
            xl = get_field(env->mstatus, MSTATUS64_UXL);
            break;
        default: /* PRV_S | PRV_H */
            xl = get_field(env->mstatus, MSTATUS64_SXL);
            break;
        }
    }
#endif
    return xl;
}
#endif

static inline int riscv_cpu_xlen(CPURISCVState *env)
{
    return 16 << env->xl;
}

#ifdef TARGET_RISCV32
#define riscv_cpu_sxl(env)  ((void)(env), MXL_RV32)
#else
static inline RISCVMXL riscv_cpu_sxl(CPURISCVState *env)
{
#ifdef CONFIG_USER_ONLY
    return env->misa_mxl;
#else
    return get_field(env->mstatus, MSTATUS64_SXL);
#endif
}
#endif

/*
 * Encode LMUL to lmul as follows:
 *     LMUL    vlmul    lmul
 *      1       000       0
 *      2       001       1
 *      4       010       2
 *      8       011       3
 *      -       100       -
 *     1/8      101      -3
 *     1/4      110      -2
 *     1/2      111      -1
 *
 * then, we can calculate VLMAX = vlen >> (vsew + 3 - lmul)
 * e.g. vlen = 256 bits, SEW = 16, LMUL = 1/8
 *      => VLMAX = vlen >> (1 + 3 - (-3))
 *               = 256 >> 7
 *               = 2
 */
static inline uint32_t vext_get_vlmax(RISCVCPU *cpu, target_ulong vtype)
{
    uint8_t sew = FIELD_EX64(vtype, VTYPE, VSEW);
    int8_t lmul = sextract32(FIELD_EX64(vtype, VTYPE, VLMUL), 0, 3);
    return cpu->cfg.vlen >> (sew + 3 - lmul);
}

void riscv_cpu_get_tb_cpu_state(CPURISCVState *env, target_ulong *pc,
                                target_ulong *cs_base, target_ulong *pcc_base,
                                target_ulong *pcc_top, uint32_t *cheri_flags,
                                uint32_t *pflags);

// Ugly macro hack to avoid having to modify cpu_get_tb_cpu_state in all targets
#define cpu_get_tb_cpu_state_ext riscv_cpu_get_tb_cpu_state

#ifdef CONFIG_TCG_LOG_INSTR
#define RISCV_LOG_INSTR_CPU_U QEMU_LOG_INSTR_CPU_USER
#define RISCV_LOG_INSTR_CPU_S QEMU_LOG_INSTR_CPU_SUPERVISOR
#define RISCV_LOG_INSTR_CPU_H QEMU_LOG_INSTR_CPU_HYPERVISOR
#define RISCV_LOG_INSTR_CPU_M QEMU_LOG_INSTR_CPU_TARGET1
extern const char * const riscv_cpu_mode_names[];

static inline bool cpu_in_user_mode(CPURISCVState *env)
{
    return env->priv == PRV_U;
}

static inline unsigned cpu_get_asid(CPURISCVState *env, target_ulong pc)
{
    return get_field(env->satp, SATP_ASID);
}

static inline const char *cpu_get_mode_name(qemu_log_instr_cpu_mode_t mode)
{
    if (riscv_cpu_mode_names[mode])
        return riscv_cpu_mode_names[mode];
    return "<invalid>";
}
#endif

void riscv_cpu_update_mask(CPURISCVState *env);

RISCVException riscv_csrrw_check(CPURISCVState *env, int csrno,
                                 target_ulong write_mask, RISCVCPU *cpu);
RISCVException riscv_csrrw(CPURISCVState *env, int csrno,
                           target_ulong *ret_value,
                           target_ulong new_value, target_ulong write_mask,
                           uintptr_t retpc);
RISCVException riscv_csrrw_debug(CPURISCVState *env, int csrno,
                                 target_ulong *ret_value,
                                 target_ulong new_value,
                                 target_ulong write_mask);

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

typedef RISCVException (*riscv_csr_predicate_fn)(CPURISCVState *env,
                                                 int csrno);
typedef RISCVException (*riscv_csr_read_fn)(CPURISCVState *env, int csrno,
                                            target_ulong *ret_value);
typedef RISCVException (*riscv_csr_write_fn)(CPURISCVState *env, int csrno,
                                             target_ulong new_value);
typedef RISCVException (*riscv_csr_op_fn)(CPURISCVState *env, int csrno,
                                          target_ulong *ret_value,
                                          target_ulong new_value,
                                          target_ulong write_mask);
typedef void (*riscv_csr_log_update_fn)(CPURISCVState *env, int csrno,
                                        target_ulong new_value);

RISCVException riscv_csrrw_i128(CPURISCVState *env, int csrno,
                                Int128 *ret_value,
                                Int128 new_value, Int128 write_mask,
                                uintptr_t retpc);

typedef RISCVException (*riscv_csr_read128_fn)(CPURISCVState *env, int csrno,
                                               Int128 *ret_value);
typedef RISCVException (*riscv_csr_write128_fn)(CPURISCVState *env, int csrno,
                                             Int128 new_value);

typedef struct {
    const char *name;
    riscv_csr_predicate_fn predicate;
    riscv_csr_read_fn read;
    riscv_csr_write_fn write;
    riscv_csr_op_fn op;
    riscv_csr_read128_fn read128;
    riscv_csr_write128_fn write128;
    /* The default priv spec version should be PRIV_VERSION_1_10_0 (i.e 0) */
    uint32_t min_priv_ver;
} riscv_csr_operations;

/* CSR function table constants */
enum {
    CSR_TABLE_SIZE = 0x1000
};

/**
 * The event id are encoded based on the encoding specified in the
 * SBI specification v0.3
 */

enum riscv_pmu_event_idx {
    RISCV_PMU_EVENT_HW_CPU_CYCLES = 0x01,
    RISCV_PMU_EVENT_HW_INSTRUCTIONS = 0x02,
    RISCV_PMU_EVENT_CACHE_DTLB_READ_MISS = 0x10019,
    RISCV_PMU_EVENT_CACHE_DTLB_WRITE_MISS = 0x1001B,
    RISCV_PMU_EVENT_CACHE_ITLB_PREFETCH_MISS = 0x10021,
};

/* CSR function table */
extern riscv_csr_operations csr_ops[CSR_TABLE_SIZE];

void riscv_get_csr_ops(int csrno, riscv_csr_operations *ops);
void riscv_set_csr_ops(int csrno, riscv_csr_operations *ops);

void riscv_cpu_register_gdb_regs_for_features(CPUState *cs);

#ifdef TARGET_CHERI
typedef struct _csr_cap_ops riscv_csr_cap_ops;
typedef cap_register_t (*riscv_csr_cap_read_fn)(CPURISCVState *env,
                                                riscv_csr_cap_ops *cap);
typedef void (*riscv_csr_cap_write_fn)(CPURISCVState *env,
                                       riscv_csr_cap_ops *cap,
                                       cap_register_t src, target_ulong newval,
                                       bool clen);

#define CSR_OP_REQUIRE_CRE   (1 << 0)
#define CSR_OP_IA_CONVERSION (1 << 1)
#define CSR_OP_UPDATE_SCADDR (1 << 2)
#define CSR_OP_EXTENDED_REG  (1 << 3)
#define CSR_OP_IS_CODE_PTR   (1 << 4)
#define CSR_OP_DIRECT_WRITE  (0)

struct _csr_cap_ops {
    const char *name;
    uint32_t reg_num;
    riscv_csr_cap_read_fn read;
    riscv_csr_cap_write_fn write;
    uint8_t flags;
};
riscv_csr_cap_ops *get_csr_cap_info(uint32_t csrnum);
cap_register_t *get_cap_csr(CPUArchState *env, uint32_t index);

/* Do the CRE bits allow cheri access in the current CPU mode? */
static inline bool riscv_cpu_mode_cre(CPURISCVState *env)
{
#ifdef TARGET_CHERI_RISCV_V9
    return env_archcpu(env)->cfg.ext_cheri;
#else
    /*
     * CRE bits are defined only if Zcherihybrid is supported.
     * For Zcheripurecap, cheri register access is always allowed.
     */
    if (!riscv_feature(env, RISCV_FEATURE_CHERI_HYBRID)) {
        return true;
    }

    if (env->mseccfg & MSECCFG_CRE) {
        /* CRE bits allow cheri in M mode */
        if (env->priv == PRV_M)
            return true;

        if (env->menvcfg & MENVCFG_CRE) {
            /* CRE bits allow cheri in S mode (and in M mode) */
            if (env->priv == PRV_S)
                return true;

            if (env->senvcfg & SENVCFG_CRE) {
                /* CRE bits allow cheri in U mode (and in M, S modes) */
                if (env->priv == PRV_U)
                    return true;
            }
        }
    }

    /*
     * For now, we do not support the hypervisor extension. It'll probably
     * have another CRE bit for H mode.
     */

    return false;
#endif
}
#endif

#endif /* RISCV_CPU_H */
