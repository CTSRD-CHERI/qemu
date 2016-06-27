#if !defined (__MIPS_CPU_H__)
#define __MIPS_CPU_H__

//#define DEBUG_OP

#define ALIGNED_ONLY

#define ELF_MACHINE	EM_MIPS

#define CPUArchState struct CPUMIPSState

#include "config.h"
#include "qemu-common.h"
#include "mips-defs.h"
#include "exec/cpu-defs.h"
#include "fpu/softfloat.h"

struct CPUMIPSState;

typedef struct r4k_tlb_t r4k_tlb_t;
struct r4k_tlb_t {
    target_ulong VPN;
    uint32_t PageMask;
    uint_fast8_t ASID;
    uint_fast16_t G:1;
    uint_fast16_t C0:3;
    uint_fast16_t C1:3;
    uint_fast16_t V0:1;
    uint_fast16_t V1:1;
    uint_fast16_t D0:1;
    uint_fast16_t D1:1;
#if defined(TARGET_CHERI)
    uint_fast16_t L0:1;
    uint_fast16_t L1:1;
    uint_fast16_t S0:1;
    uint_fast16_t S1:1;
#else
    uint_fast16_t XI0:1;
    uint_fast16_t XI1:1;
    uint_fast16_t RI0:1;
    uint_fast16_t RI1:1;
#endif /* TARGET_CHERI */
    uint_fast16_t EHINV:1;
    uint64_t PFN[2];
};

#if !defined(CONFIG_USER_ONLY)
typedef struct CPUMIPSTLBContext CPUMIPSTLBContext;
struct CPUMIPSTLBContext {
    uint32_t nb_tlb;
    uint32_t tlb_in_use;
    int (*map_address) (struct CPUMIPSState *env, hwaddr *physical, int *prot, target_ulong address, int rw, int access_type);
    void (*helper_tlbwi)(struct CPUMIPSState *env);
    void (*helper_tlbwr)(struct CPUMIPSState *env);
    void (*helper_tlbp)(struct CPUMIPSState *env);
    void (*helper_tlbr)(struct CPUMIPSState *env);
    void (*helper_tlbinv)(struct CPUMIPSState *env);
    void (*helper_tlbinvf)(struct CPUMIPSState *env);
    union {
        struct {
            r4k_tlb_t tlb[MIPS_TLB_MAX];
        } r4k;
    } mmu;
};
#endif

/* MSA Context */
#define MSA_WRLEN (128)

enum CPUMIPSMSADataFormat {
    DF_BYTE = 0,
    DF_HALF,
    DF_WORD,
    DF_DOUBLE
};

typedef union wr_t wr_t;
union wr_t {
    int8_t  b[MSA_WRLEN/8];
    int16_t h[MSA_WRLEN/16];
    int32_t w[MSA_WRLEN/32];
    int64_t d[MSA_WRLEN/64];
};

typedef union fpr_t fpr_t;
union fpr_t {
    float64  fd;   /* ieee double precision */
    float32  fs[2];/* ieee single precision */
    uint64_t d;    /* binary double fixed-point */
    uint32_t w[2]; /* binary single fixed-point */
/* FPU/MSA register mapping is not tested on big-endian hosts. */
    wr_t     wr;   /* vector data */
};
/* define FP_ENDIAN_IDX to access the same location
 * in the fpr_t union regardless of the host endianness
 */
#if defined(HOST_WORDS_BIGENDIAN)
#  define FP_ENDIAN_IDX 1
#else
#  define FP_ENDIAN_IDX 0
#endif

typedef struct CPUMIPSFPUContext CPUMIPSFPUContext;
struct CPUMIPSFPUContext {
    /* Floating point registers */
    fpr_t fpr[32];
    float_status fp_status;
    /* fpu implementation/revision register (fir) */
    uint32_t fcr0;
#define FCR0_FREP 29
#define FCR0_UFRP 28
#define FCR0_F64 22
#define FCR0_L 21
#define FCR0_W 20
#define FCR0_3D 19
#define FCR0_PS 18
#define FCR0_D 17
#define FCR0_S 16
#define FCR0_PRID 8
#define FCR0_REV 0
    /* fcsr */
    uint32_t fcr31;
#define SET_FP_COND(num,env)     do { ((env).fcr31) |= ((num) ? (1 << ((num) + 24)) : (1 << 23)); } while(0)
#define CLEAR_FP_COND(num,env)   do { ((env).fcr31) &= ~((num) ? (1 << ((num) + 24)) : (1 << 23)); } while(0)
#define GET_FP_COND(env)         ((((env).fcr31 >> 24) & 0xfe) | (((env).fcr31 >> 23) & 0x1))
#define GET_FP_CAUSE(reg)        (((reg) >> 12) & 0x3f)
#define GET_FP_ENABLE(reg)       (((reg) >>  7) & 0x1f)
#define GET_FP_FLAGS(reg)        (((reg) >>  2) & 0x1f)
#define SET_FP_CAUSE(reg,v)      do { (reg) = ((reg) & ~(0x3f << 12)) | ((v & 0x3f) << 12); } while(0)
#define SET_FP_ENABLE(reg,v)     do { (reg) = ((reg) & ~(0x1f <<  7)) | ((v & 0x1f) << 7); } while(0)
#define SET_FP_FLAGS(reg,v)      do { (reg) = ((reg) & ~(0x1f <<  2)) | ((v & 0x1f) << 2); } while(0)
#define UPDATE_FP_FLAGS(reg,v)   do { (reg) |= ((v & 0x1f) << 2); } while(0)
#define FP_INEXACT        1
#define FP_UNDERFLOW      2
#define FP_OVERFLOW       4
#define FP_DIV0           8
#define FP_INVALID        16
#define FP_UNIMPLEMENTED  32
};

#define NB_MMU_MODES 3

typedef struct CPUMIPSMVPContext CPUMIPSMVPContext;
struct CPUMIPSMVPContext {
    int32_t CP0_MVPControl;
#define CP0MVPCo_CPA	3
#define CP0MVPCo_STLB	2
#define CP0MVPCo_VPC	1
#define CP0MVPCo_EVP	0
    int32_t CP0_MVPConf0;
#define CP0MVPC0_M	31
#define CP0MVPC0_TLBS	29
#define CP0MVPC0_GS	28
#define CP0MVPC0_PCP	27
#define CP0MVPC0_PTLBE	16
#define CP0MVPC0_TCA	15
#define CP0MVPC0_PVPE	10
#define CP0MVPC0_PTC	0
    int32_t CP0_MVPConf1;
#define CP0MVPC1_CIM	31
#define CP0MVPC1_CIF	30
#define CP0MVPC1_PCX	20
#define CP0MVPC1_PCP2	10
#define CP0MVPC1_PCP1	0
};

typedef struct mips_def_t mips_def_t;

#define MIPS_SHADOW_SET_MAX 16
#define MIPS_TC_MAX 5
#define MIPS_FPU_MAX 1
#define MIPS_DSP_ACC 4
#define MIPS_KSCRATCH_NUM 6

#if defined(TARGET_CHERI)
/*
 * Please note if this structure is changed then the TCG gen_branch() in
 * translate.c may need to be changed as well.
 */
struct cap_register {
                        /* offset = cursor - base */
    uint64_t cr_offset; /* Capability offset */
    uint64_t cr_base;   /* Capability base addr */
    uint64_t cr_length; /* Capability length */
    uint32_t cr_perms;  /* Permissions */
    uint32_t cr_uperms; /* User Permissions */
#ifdef CHERI_128
    uint64_t cr_pesbt;  /* Perms, E, Sealed, Bot, & Top bits (128-bit) */
#endif
    uint32_t cr_otype;  /* Object Type, 24 bits */
    uint8_t  cr_sealed; /* Sealed flag */
    uint8_t  cr_tag;    /* Tag */
};
typedef struct cap_register cap_register_t;

static inline cap_register_t *null_capability(cap_register_t *cp)
{
    cp->cr_otype = 0;
    cp->cr_perms = 0;
    cp->cr_uperms = 0;
    // cp->cr_cursor = 0UL;
    cp->cr_offset = 0UL;
    cp->cr_base = 0UL;
    cp->cr_length = 0UL;
    cp->cr_tag = 0;
    cp->cr_sealed = 0;
#ifdef CHERI_128
    cp->cr_pesbt = 0UL;
#endif

    return cp;
}

#define CAP_MAX_OTYPE           (0xffffff)

#define CAP_PERM_GLOBAL         (1 << 0)
#define CAP_PERM_EXECUTE        (1 << 1)
#define CAP_PERM_LOAD           (1 << 2)
#define CAP_PERM_STORE          (1 << 3)
#define CAP_PERM_LOAD_CAP       (1 << 4)
#define CAP_PERM_STORE_CAP      (1 << 5)
#define CAP_PERM_STORE_LOCAL    (1 << 6)
#define CAP_PERM_SEAL           (1 << 7)
#ifdef NOTYET
#define CAP_ACCESS_SYS_REGS     (1 << 8)
#define CAP_RESERVED1           (1 << 9)
#define CAP_RESERVED2           (1 << 10)
#define CAP_RESERVED3           (1 << 11)
#define CAP_RESERVED4           (1 << 12)
#define CAP_RESERVED5           (1 << 13)
#define CAP_RESERVED6           (1 << 14)
#else
#define CAP_ACCESS_SYS_REGS     (1 << 8)
#define CAP_RESERVED2           (1 << 9)
#define CAP_ACCESS_EPCC         (1 << 10)
#define CAP_ACCESS_KDC          (1 << 11)
#define CAP_ACCESS_KCC          (1 << 12)
#define CAP_ACCESS_KR1C         (1 << 13)
#define CAP_ACCESS_KR2C         (1 << 14)
#endif /* NOTYET */
/* 15-18 Software-defined */
#define CAP_ACCESS_LEGACY_ALL   (1 << 10) | (1 << 11) | (1 << 12) | \
                                (1 << 13) | (1 << 14)
#if defined(CHERI_128) || defined(CHERI_MAGIC128)
#define CAP_PERMS_ALL           (0x7fff)     /* [0...14] */
#define CAP_UPERMS_ALL          (0xf)        /* [15...18] */
#define CAP_UPERMS_MAX          (3)
#define CAP_UPERMS_SHFT         (15)
#else /* ! CHERI_128 */
#define CAP_PERMS_ALL           (0x7fff)     /* [0...14] */
#define CAP_UPERMS_ALL          (0xffff)     /* [15...30] */
#define CAP_UPERMS_MAX          (15)
#define CAP_UPERMS_SHFT         (15)
#endif /* ! CHERI_128 */

struct cvtrace {
    uint8_t version;
#define CVT_GPR     1   /* GPR change (val2) */
#define CVT_LD_GPR  2   /* Load into GPR (val2) from address (val1) */
#define CVT_ST_GPR  3   /* Store from GPR (val2) to address (val1) */
#define CVT_NO_REG  4   /* No register is changed. */
#define CVT_CAP     11  /* Cap change (val2,val3,val4,val5) */
#define CVT_LD_CAP  12  /* Load Cap (val2,val3,val4,val5) from addr (val1) */
#define CVT_ST_CAP  13  /* Store Cap (val2,val3,val4,val5) to addr (val1) */
    uint8_t exception;  /* 0=none, 1=TLB Mod, 2=TLB Load, 3=TLB Store, etc. */
    uint16_t cycles;    /* Currently not used. */
    uint32_t inst;      /* Encoded instruction. */
    uint64_t pc;        /* PC value of instruction. */
    uint64_t val1;      /* val1 is used for memory address. */
    uint64_t val2;      /* val2, val3, val4, val5 are used for reg content. */
    uint64_t val3;
    uint64_t val4;
    uint64_t val5;
    uint8_t thread;     /* Hardware thread/CPU (i.e. cpu->cpu_index ) */
    uint8_t asid;       /* Address Space ID (i.e. CP0_TCStatus & 0xff) */
} __attribute__((packed));
typedef struct cvtrace cvtrace_t;

/* Version 3 Cheri Stream Trace header info */
#define CVT_QEMU_VERSION    (0x80 + 3)
#define CVT_QEMU_MAGIC      "CheriTraceV03"
#endif /* TARGET_CHERI */

typedef struct TCState TCState;
struct TCState {
    target_ulong gpr[32];
    target_ulong PC;
    target_ulong HI[MIPS_DSP_ACC];
    target_ulong LO[MIPS_DSP_ACC];
    target_ulong ACX[MIPS_DSP_ACC];
    target_ulong DSPControl;
    int32_t CP0_TCStatus;
#define CP0TCSt_TCU3	31
#define CP0TCSt_TCU2	30
#define CP0TCSt_TCU1	29
#define CP0TCSt_TCU0	28
#define CP0TCSt_TMX	27
#define CP0TCSt_RNST	23
#define CP0TCSt_TDS	21
#define CP0TCSt_DT	20
#define CP0TCSt_DA	15
#define CP0TCSt_A	13
#define CP0TCSt_TKSU	11
#define CP0TCSt_IXMT	10
#define CP0TCSt_TASID	0
    int32_t CP0_TCBind;
#define CP0TCBd_CurTC	21
#define CP0TCBd_TBE	17
#define CP0TCBd_CurVPE	0
    target_ulong CP0_TCHalt;
    target_ulong CP0_TCContext;
    target_ulong CP0_TCSchedule;
    target_ulong CP0_TCScheFBack;
    int32_t CP0_Debug_tcstatus;
    target_ulong CP0_UserLocal;

    int32_t msacsr;

#define MSACSR_FS       24
#define MSACSR_FS_MASK  (1 << MSACSR_FS)
#define MSACSR_NX       18
#define MSACSR_NX_MASK  (1 << MSACSR_NX)
#define MSACSR_CEF      2
#define MSACSR_CEF_MASK (0xffff << MSACSR_CEF)
#define MSACSR_RM       0
#define MSACSR_RM_MASK  (0x3 << MSACSR_RM)
#define MSACSR_MASK     (MSACSR_RM_MASK | MSACSR_CEF_MASK | MSACSR_NX_MASK | \
        MSACSR_FS_MASK)

    float_status msa_fp_status;
#if defined(TARGET_CHERI)
    cap_register_t PCC;
    cap_register_t C[32];
#define CP2CAP_RCC  24  /* Return Code Capability */
#define CP2CAP_IDC  26  /* Invoked Data Capability */
#define CP2CAP_KR1C 27  /* Reserved Kernel Cap #1 */
#define CP2CAP_KR2C 28  /* Reserved Kernel Cap #2 */
#define CP2CAP_KCC  29  /* Kernel Code Capability */
#define CP2CAP_KDC  30  /* Kernel Data Capability */
#define CP2CAP_EPCC 31  /* Exception PC Capability */
#endif /* TARGET_CHERI */
};

typedef struct CPUMIPSState CPUMIPSState;
struct CPUMIPSState {
    TCState active_tc;
    CPUMIPSFPUContext active_fpu;

    uint32_t current_tc;
    uint32_t current_fpu;

    uint32_t SEGBITS;
    uint32_t PABITS;
#if defined(TARGET_MIPS64)
# define PABITS_BASE 36
#else
# define PABITS_BASE 32
#endif
    target_ulong SEGMask;
    uint64_t PAMask;
#define PAMASK_BASE ((1ULL << PABITS_BASE) - 1)

    int32_t msair;
#define MSAIR_ProcID    8
#define MSAIR_Rev       0

    int32_t CP0_Index;
    /* CP0_MVP* are per MVP registers. */
    int32_t CP0_Random;
    int32_t CP0_VPEControl;
#define CP0VPECo_YSI	21
#define CP0VPECo_GSI	20
#define CP0VPECo_EXCPT	16
#define CP0VPECo_TE	15
#define CP0VPECo_TargTC	0
    int32_t CP0_VPEConf0;
#define CP0VPEC0_M	31
#define CP0VPEC0_XTC	21
#define CP0VPEC0_TCS	19
#define CP0VPEC0_SCS	18
#define CP0VPEC0_DSC	17
#define CP0VPEC0_ICS	16
#define CP0VPEC0_MVP	1
#define CP0VPEC0_VPA	0
    int32_t CP0_VPEConf1;
#define CP0VPEC1_NCX	20
#define CP0VPEC1_NCP2	10
#define CP0VPEC1_NCP1	0
    target_ulong CP0_YQMask;
    target_ulong CP0_VPESchedule;
    target_ulong CP0_VPEScheFBack;
    int32_t CP0_VPEOpt;
#define CP0VPEOpt_IWX7	15
#define CP0VPEOpt_IWX6	14
#define CP0VPEOpt_IWX5	13
#define CP0VPEOpt_IWX4	12
#define CP0VPEOpt_IWX3	11
#define CP0VPEOpt_IWX2	10
#define CP0VPEOpt_IWX1	9
#define CP0VPEOpt_IWX0	8
#define CP0VPEOpt_DWX7	7
#define CP0VPEOpt_DWX6	6
#define CP0VPEOpt_DWX5	5
#define CP0VPEOpt_DWX4	4
#define CP0VPEOpt_DWX3	3
#define CP0VPEOpt_DWX2	2
#define CP0VPEOpt_DWX1	1
#define CP0VPEOpt_DWX0	0
    uint64_t CP0_EntryLo0;
    uint64_t CP0_EntryLo1;
#if defined(TARGET_MIPS64)
#if defined(TARGET_CHERI)
# define CP0EnLo_S 63
# define CP0EnLo_L 62
#else
# define CP0EnLo_RI 63
# define CP0EnLo_XI 62
#endif /* TARGET_CHERI */
#else
# define CP0EnLo_RI 31
# define CP0EnLo_XI 30
#endif
    target_ulong CP0_Context;
    target_ulong CP0_KScratch[MIPS_KSCRATCH_NUM];
    int32_t CP0_PageMask;
    int32_t CP0_PageGrain_rw_bitmask;
    int32_t CP0_PageGrain;
#define CP0PG_RIE 31
#define CP0PG_XIE 30
#define CP0PG_ELPA 29
#define CP0PG_IEC 27
    int32_t CP0_Wired;
    int32_t CP0_SRSConf0_rw_bitmask;
    int32_t CP0_SRSConf0;
#define CP0SRSC0_M	31
#define CP0SRSC0_SRS3	20
#define CP0SRSC0_SRS2	10
#define CP0SRSC0_SRS1	0
    int32_t CP0_SRSConf1_rw_bitmask;
    int32_t CP0_SRSConf1;
#define CP0SRSC1_M	31
#define CP0SRSC1_SRS6	20
#define CP0SRSC1_SRS5	10
#define CP0SRSC1_SRS4	0
    int32_t CP0_SRSConf2_rw_bitmask;
    int32_t CP0_SRSConf2;
#define CP0SRSC2_M	31
#define CP0SRSC2_SRS9	20
#define CP0SRSC2_SRS8	10
#define CP0SRSC2_SRS7	0
    int32_t CP0_SRSConf3_rw_bitmask;
    int32_t CP0_SRSConf3;
#define CP0SRSC3_M	31
#define CP0SRSC3_SRS12	20
#define CP0SRSC3_SRS11	10
#define CP0SRSC3_SRS10	0
    int32_t CP0_SRSConf4_rw_bitmask;
    int32_t CP0_SRSConf4;
#define CP0SRSC4_SRS15	20
#define CP0SRSC4_SRS14	10
#define CP0SRSC4_SRS13	0
    int32_t CP0_HWREna;
    target_ulong CP0_BadVAddr;
    uint32_t CP0_BadInstr;
    uint32_t CP0_BadInstrP;
    int32_t CP0_Count;
    target_ulong CP0_EntryHi;
#define CP0EnHi_EHINV 10
    int32_t CP0_Compare;
    int32_t CP0_Status;
#define CP0St_CU3   31
#define CP0St_CU2   30
#define CP0St_CU1   29
#define CP0St_CU0   28
#define CP0St_RP    27
#define CP0St_FR    26
#define CP0St_RE    25
#define CP0St_MX    24
#define CP0St_PX    23
#define CP0St_BEV   22
#define CP0St_TS    21
#define CP0St_SR    20
#define CP0St_NMI   19
#define CP0St_IM    8
#define CP0St_KX    7
#define CP0St_SX    6
#define CP0St_UX    5
#define CP0St_KSU   3
#define CP0St_ERL   2
#define CP0St_EXL   1
#define CP0St_IE    0
    int32_t CP0_IntCtl;
#define CP0IntCtl_IPTI 29
#define CP0IntCtl_IPPC1 26
#define CP0IntCtl_VS 5
    int32_t CP0_SRSCtl;
#define CP0SRSCtl_HSS 26
#define CP0SRSCtl_EICSS 18
#define CP0SRSCtl_ESS 12
#define CP0SRSCtl_PSS 6
#define CP0SRSCtl_CSS 0
    int32_t CP0_SRSMap;
#define CP0SRSMap_SSV7 28
#define CP0SRSMap_SSV6 24
#define CP0SRSMap_SSV5 20
#define CP0SRSMap_SSV4 16
#define CP0SRSMap_SSV3 12
#define CP0SRSMap_SSV2 8
#define CP0SRSMap_SSV1 4
#define CP0SRSMap_SSV0 0
    int32_t CP0_Cause;
#define CP0Ca_BD   31
#define CP0Ca_TI   30
#define CP0Ca_CE   28
#define CP0Ca_DC   27
#define CP0Ca_PCI  26
#define CP0Ca_IV   23
#define CP0Ca_WP   22
#define CP0Ca_IP    8
#define CP0Ca_IP_mask 0x0000FF00
#define CP0Ca_EC    2
    target_ulong CP0_EPC;
    int32_t CP0_PRid;
    int32_t CP0_EBase;
    int32_t CP0_Config0;
#define CP0C0_M    31
#define CP0C0_K23  28
#define CP0C0_KU   25
#define CP0C0_MDU  20
#define CP0C0_MM   18
#define CP0C0_BM   16
#define CP0C0_BE   15
#define CP0C0_AT   13
#define CP0C0_AR   10
#define CP0C0_MT   7
#define CP0C0_VI   3
#define CP0C0_K0   0
    int32_t CP0_Config1;
#define CP0C1_M    31
#define CP0C1_MMU  25
#define CP0C1_IS   22
#define CP0C1_IL   19
#define CP0C1_IA   16
#define CP0C1_DS   13
#define CP0C1_DL   10
#define CP0C1_DA   7
#define CP0C1_C2   6
#define CP0C1_MD   5
#define CP0C1_PC   4
#define CP0C1_WR   3
#define CP0C1_CA   2
#define CP0C1_EP   1
#define CP0C1_FP   0
    int32_t CP0_Config2;
#define CP0C2_M    31
#define CP0C2_TU   28
#define CP0C2_TS   24
#define CP0C2_TL   20
#define CP0C2_TA   16
#define CP0C2_SU   12
#define CP0C2_SS   8
#define CP0C2_SL   4
#define CP0C2_SA   0
    int32_t CP0_Config3;
#define CP0C3_M    31
#define CP0C3_BPG  30
#define CP0C3_CMCGR 29
#define CP0C3_MSAP  28
#define CP0C3_BP 27
#define CP0C3_BI 26
#define CP0C3_IPLW 21
#define CP0C3_MMAR 18
#define CP0C3_MCU  17
#define CP0C3_ISA_ON_EXC 16
#define CP0C3_ISA  14
#define CP0C3_ULRI 13
#define CP0C3_RXI  12
#define CP0C3_DSP2P 11
#define CP0C3_DSPP 10
#define CP0C3_LPA  7
#define CP0C3_VEIC 6
#define CP0C3_VInt 5
#define CP0C3_SP   4
#define CP0C3_CDMM 3
#define CP0C3_MT   2
#define CP0C3_SM   1
#define CP0C3_TL   0
    int32_t CP0_Config4;
    int32_t CP0_Config4_rw_bitmask;
#define CP0C4_M    31
#define CP0C4_IE   29
#define CP0C4_KScrExist 16
#define CP0C4_MMUExtDef 14
#define CP0C4_FTLBPageSize 8
#define CP0C4_FTLBWays 4
#define CP0C4_FTLBSets 0
#define CP0C4_MMUSizeExt 0
    int32_t CP0_Config5;
    int32_t CP0_Config5_rw_bitmask;
#define CP0C5_M          31
#define CP0C5_K          30
#define CP0C5_CV         29
#define CP0C5_EVA        28
#define CP0C5_MSAEn      27
#define CP0C5_UFE        9
#define CP0C5_FRE        8
#define CP0C5_SBRI       6
#define CP0C5_MVH        5
#define CP0C5_LLB        4
#define CP0C5_UFR        2
#define CP0C5_NFExists   0
    int32_t CP0_Config6;
    int32_t CP0_Config7;
    /* XXX: Maybe make LLAddr per-TC? */
    uint64_t lladdr;
    target_ulong llval;
    target_ulong llnewval;
    target_ulong llreg;
#ifdef TARGET_CHERI
    uint64_t linkedflag;
    int32_t TLB_L;
    int32_t TLB_S;
#endif
    uint64_t CP0_LLAddr_rw_bitmask;
    int CP0_LLAddr_shift;
    target_ulong CP0_WatchLo[8];
    int32_t CP0_WatchHi[8];
    target_ulong CP0_XContext;
    int32_t CP0_Framemask;
    int32_t CP0_Debug;
#define CP0DB_DBD  31
#define CP0DB_DM   30
#define CP0DB_LSNM 28
#define CP0DB_Doze 27
#define CP0DB_Halt 26
#define CP0DB_CNT  25
#define CP0DB_IBEP 24
#define CP0DB_DBEP 21
#define CP0DB_IEXI 20
#define CP0DB_VER  15
#define CP0DB_DEC  10
#define CP0DB_SSt  8
#define CP0DB_DINT 5
#define CP0DB_DIB  4
#define CP0DB_DDBS 3
#define CP0DB_DDBL 2
#define CP0DB_DBp  1
#define CP0DB_DSS  0
    target_ulong CP0_DEPC;
    int32_t CP0_Performance0;
    uint64_t CP0_TagLo;
    int32_t CP0_DataLo;
    int32_t CP0_TagHi;
    int32_t CP0_DataHi;
    target_ulong CP0_ErrorEPC;
    int32_t CP0_DESAVE;
    /* We waste some space so we can handle shadow registers like TCs. */
    TCState tcs[MIPS_SHADOW_SET_MAX];
    CPUMIPSFPUContext fpus[MIPS_FPU_MAX];
    /* QEMU */
    int error_code;
#define EXCP_TLB_NOMATCH   0x1
#define EXCP_INST_NOTAVAIL 0x2 /* No valid instruction word for BadInstr */
    uint32_t hflags;    /* CPU State */
    /* TMASK defines different execution modes */
#ifdef TARGET_CHERI
#define MIPS_HFLAG_TMASK  0xF5807FF
#else
#define MIPS_HFLAG_TMASK  0x75807FF
#endif /* TARGET_CHERI */
#define MIPS_HFLAG_MODE   0x00007 /* execution modes                    */
    /* The KSU flags must be the lowest bits in hflags. The flag order
       must be the same as defined for CP0 Status. This allows to use
       the bits as the value of mmu_idx. */
#define MIPS_HFLAG_KSU    0x00003 /* kernel/supervisor/user mode mask   */
#define MIPS_HFLAG_UM     0x00002 /* user mode flag                     */
#define MIPS_HFLAG_SM     0x00001 /* supervisor mode flag               */
#define MIPS_HFLAG_KM     0x00000 /* kernel mode flag                   */
#define MIPS_HFLAG_DM     0x00004 /* Debug mode                         */
#define MIPS_HFLAG_64     0x00008 /* 64-bit instructions enabled        */
#define MIPS_HFLAG_CP0    0x00010 /* CP0 enabled                        */
#define MIPS_HFLAG_FPU    0x00020 /* FPU enabled                        */
#define MIPS_HFLAG_F64    0x00040 /* 64-bit FPU enabled                 */
    /* True if the MIPS IV COP1X instructions can be used.  This also
       controls the non-COP1X instructions RECIP.S, RECIP.D, RSQRT.S
       and RSQRT.D.  */
#define MIPS_HFLAG_COP1X  0x00080 /* COP1X instructions enabled         */
#define MIPS_HFLAG_RE     0x00100 /* Reversed endianness                */
#define MIPS_HFLAG_AWRAP  0x00200 /* 32-bit compatibility address wrapping */
#define MIPS_HFLAG_M16    0x00400 /* MIPS16 mode flag                   */
#define MIPS_HFLAG_M16_SHIFT 10
    /* If translation is interrupted between the branch instruction and
     * the delay slot, record what type of branch it is so that we can
     * resume translation properly.  It might be possible to reduce
     * this from three bits to two.  */
#define MIPS_HFLAG_BMASK_BASE  0x803800
#define MIPS_HFLAG_B      0x00800 /* Unconditional branch               */
#define MIPS_HFLAG_BC     0x01000 /* Conditional branch                 */
#define MIPS_HFLAG_BL     0x01800 /* Likely branch                      */
#define MIPS_HFLAG_BR     0x02000 /* branch to register (can't link TB) */
#ifdef TARGET_CHERI
#define MIPS_HFLAG_BRC    0x02800 /* branch to register and load PCC    */
#endif /* TARGET_CHERI */
    /* Extra flags about the current pending branch.  */
#define MIPS_HFLAG_BMASK_EXT 0x7C000
#define MIPS_HFLAG_B16    0x04000 /* branch instruction was 16 bits     */
#define MIPS_HFLAG_BDS16  0x08000 /* branch requires 16-bit delay slot  */
#define MIPS_HFLAG_BDS32  0x10000 /* branch requires 32-bit delay slot  */
#define MIPS_HFLAG_BDS_STRICT  0x20000 /* Strict delay slot size */
#define MIPS_HFLAG_BX     0x40000 /* branch exchanges execution mode    */
#define MIPS_HFLAG_BMASK  (MIPS_HFLAG_BMASK_BASE | MIPS_HFLAG_BMASK_EXT)
    /* MIPS DSP resources access. */
#define MIPS_HFLAG_DSP   0x080000  /* Enable access to MIPS DSP resources. */
#define MIPS_HFLAG_DSPR2 0x100000  /* Enable access to MIPS DSPR2 resources. */
    /* Extra flag about HWREna register. */
#define MIPS_HFLAG_HWRENA_ULR 0x200000 /* ULR bit from HWREna is set. */
#define MIPS_HFLAG_SBRI  0x400000 /* R6 SDBBP causes RI excpt. in user mode */
#define MIPS_HFLAG_FBNSLOT 0x800000 /* Forbidden slot                   */
#define MIPS_HFLAG_MSA   0x1000000
#define MIPS_HFLAG_FRE   0x2000000 /* FRE enabled */
#define MIPS_HFLAG_ELPA  0x4000000
#ifdef TARGET_CHERI
#define MIPS_HFLAG_COP2X   0x8000000 /* CHERI/CP2 enabled              */
    int btcr;                    /* cjr/cjalr Cap register target      */
#endif /* TARGET_CHERI */
    target_ulong btarget;        /* Jump / branch target               */
    target_ulong bcond;          /* Branch condition (if needed)       */

    int SYNCI_Step; /* Address step size for SYNCI */
    int CCRes; /* Cycle count resolution/divisor */
    uint32_t CP0_Status_rw_bitmask; /* Read/write bits in CP0_Status */
    uint32_t CP0_TCStatus_rw_bitmask; /* Read/write bits in CP0_TCStatus */
    int insn_flags; /* Supported instruction set */

#if defined(TARGET_CHERI)
    /*
     * See section 4.4.2 (Table 4.3) of the CHERI Architecture Reference.
     */
    uint16_t CP2_CapCause; /* Upper 8 bits exception code; lower reg# */
#define CP2Ca_NONE          0x00    /* None */
#define CP2Ca_LENGTH        0x01    /* Length Violation */
#define CP2Ca_TAG           0x02    /* Tag Violation */
#define CP2Ca_SEAL          0x03    /* Seal Violation */
#define CP2Ca_TYPE          0x04    /* Type Violation */
#define CP2Ca_CALL          0x05    /* Call Trap */
#define CP2Ca_RETURN        0x06    /* Return Trap */
#define CP2Ca_UNDERFLOW     0x07    /* Underflow of trusted system stack */
#define CP2Ca_USRDEFINE     0x08    /* User-defined Permission Violation */
#define CP2Ca_TLB_STORE     0x09    /* TLB prohibits store capability */
#define CP2Ca_INEXACT       0x0A    /* Bounds cannot be represented exactly */
// 0x0b-0x0f Reserved
#define CP2Ca_GLOBAL        0x10 /* Global Violation */
#define CP2Ca_PERM_EXE      0x11 /* Permit_Execute Violation */
#define CP2Ca_PERM_LD       0x12 /* Permit_Load Violation */
#define CP2Ca_PERM_ST       0x13 /* Permit_Store Violation */
#define CP2Ca_PERM_LD_CAP   0x14 /* Permit_Load_Capability Violation */
#define CP2Ca_PERM_ST_CAP   0x15 /* Permit_Store_Capability Violation */
#define CP2Ca_PERM_ST_LC_CAP 0x16 /* Permit_Store_Local_Capability Violation */
#define CP2Ca_PERM_SEAL     0x17 /* Permit_Seal Violation */
#define CP2Ca_ACCESS_SYS_REGS 0x18 /* Access System Registers Violation */
#ifdef NOTYET
// 0x19-0x1f Reserved
#else
// 0x19 Reserved
#define CP2Ca_ACCESS_EPCC   0x1A /* Access_EPCC Violation */
#define CP2Ca_ACCESS_KDC    0x1B /* Access_KDC Violation */
#define CP2Ca_ACCESS_KCC    0x1C /* Access_KCC Violation */
#define CP2Ca_ACCESS_KR1C   0x1D /* Access_KR1C Violation */
#define CP2Ca_ACCESS_KR2C   0x1E /* Access_KR2C Violation */
// 0x1f Reserved
#endif /* NOTYET */
#endif /* TARGET_CHERI */

    CPU_COMMON

    /* Fields from here on are preserved across CPU reset. */
    CPUMIPSMVPContext *mvp;
#if !defined(CONFIG_USER_ONLY)
    CPUMIPSTLBContext *tlb;
#endif

    const mips_def_t *cpu_model;
    void *irq[8];
    QEMUTimer *timer; /* Internal timer */
#ifdef TARGET_CHERI
    /*
     * Processor state after the last instruction.
     * Used for instruction tracing.
     */
    target_ulong last_gpr[32];
    target_ulong last_cop0[32*8];
    const char *last_mode;
    cap_register_t last_C[32];

    cvtrace_t cvtrace;
#endif /* TARGET_CHERI */
};

#include "cpu-qom.h"

#if !defined(CONFIG_USER_ONLY)
int no_mmu_map_address (CPUMIPSState *env, hwaddr *physical, int *prot,
                        target_ulong address, int rw, int access_type);
int fixed_mmu_map_address (CPUMIPSState *env, hwaddr *physical, int *prot,
                           target_ulong address, int rw, int access_type);
int r4k_map_address (CPUMIPSState *env, hwaddr *physical, int *prot,
                     target_ulong address, int rw, int access_type);
void r4k_helper_tlbwi(CPUMIPSState *env);
void r4k_helper_tlbwr(CPUMIPSState *env);
void r4k_helper_tlbp(CPUMIPSState *env);
void r4k_helper_tlbr(CPUMIPSState *env);
void r4k_helper_tlbinv(CPUMIPSState *env);
void r4k_helper_tlbinvf(CPUMIPSState *env);

void mips_cpu_unassigned_access(CPUState *cpu, hwaddr addr,
                                bool is_write, bool is_exec, int unused,
                                unsigned size);
#endif

void mips_cpu_list (FILE *f, fprintf_function cpu_fprintf);

#define cpu_exec cpu_mips_exec
#define cpu_gen_code cpu_mips_gen_code
#define cpu_signal_handler cpu_mips_signal_handler
#define cpu_list mips_cpu_list

extern void cpu_wrdsp(uint32_t rs, uint32_t mask_num, CPUMIPSState *env);
extern uint32_t cpu_rddsp(uint32_t mask_num, CPUMIPSState *env);

/* MMU modes definitions. We carefully match the indices with our
   hflags layout. */
#define MMU_MODE0_SUFFIX _kernel
#define MMU_MODE1_SUFFIX _super
#define MMU_MODE2_SUFFIX _user
#define MMU_USER_IDX 2
static inline int cpu_mmu_index (CPUMIPSState *env)
{
    return env->hflags & MIPS_HFLAG_KSU;
}

static inline int cpu_mips_hw_interrupts_pending(CPUMIPSState *env)
{
    int32_t pending;
    int32_t status;
    int r;

    if (!(env->CP0_Status & (1 << CP0St_IE)) ||
        (env->CP0_Status & (1 << CP0St_EXL)) ||
        (env->CP0_Status & (1 << CP0St_ERL)) ||
        /* Note that the TCStatus IXMT field is initialized to zero,
           and only MT capable cores can set it to one. So we don't
           need to check for MT capabilities here.  */
        (env->active_tc.CP0_TCStatus & (1 << CP0TCSt_IXMT)) ||
        (env->hflags & MIPS_HFLAG_DM)) {
        /* Interrupts are disabled */
        return 0;
    }

    pending = env->CP0_Cause & CP0Ca_IP_mask;
    status = env->CP0_Status & CP0Ca_IP_mask;

    if (env->CP0_Config3 & (1 << CP0C3_VEIC)) {
        /* A MIPS configured with a vectorizing external interrupt controller
           will feed a vector into the Cause pending lines. The core treats
           the status lines as a vector level, not as indiviual masks.  */
        r = pending > status;
    } else {
        /* A MIPS configured with compatibility or VInt (Vectored Interrupts)
           treats the pending lines as individual interrupt lines, the status
           lines are individual masks.  */
        r = pending & status;
    }
    return r;
}

#include "exec/cpu-all.h"

/* Memory access type :
 * may be needed for precise access rights control and precise exceptions.
 */
enum {
    /* 1 bit to define user level / supervisor access */
    ACCESS_USER  = 0x00,
    ACCESS_SUPER = 0x01,
    /* 1 bit to indicate direction */
    ACCESS_STORE = 0x02,
    /* Type of instruction that generated the access */
    ACCESS_CODE  = 0x10, /* Code fetch access                */
    ACCESS_INT   = 0x20, /* Integer load/store access        */
    ACCESS_FLOAT = 0x30, /* floating point load/store access */
};

/* Exceptions */
enum {
    EXCP_NONE          = -1,
    EXCP_RESET         = 0,
    EXCP_SRESET,
    EXCP_DSS,
    EXCP_DINT,
    EXCP_DDBL,
    EXCP_DDBS,
    EXCP_NMI,
    EXCP_MCHECK,
    EXCP_EXT_INTERRUPT, /* 8 */
    EXCP_DFWATCH,
    EXCP_DIB,
    EXCP_IWATCH,
    EXCP_AdEL,
    EXCP_AdES,
    EXCP_TLBF,
    EXCP_IBE,
    EXCP_DBp, /* 16 */
    EXCP_SYSCALL,
    EXCP_BREAK,
    EXCP_CpU,
    EXCP_RI,
    EXCP_OVERFLOW,
    EXCP_TRAP,
    EXCP_FPE,
    EXCP_DWATCH, /* 24 */
    EXCP_LTLBL,
    EXCP_TLBL,
    EXCP_TLBS,
    EXCP_DBE,
    EXCP_THREAD,
    EXCP_MDMX,
    EXCP_C2E,
    EXCP_CACHE, /* 32 */
    EXCP_DSPDIS,
    EXCP_MSADIS,
    EXCP_MSAFPE,
    EXCP_TLBXI,
    EXCP_TLBRI,

    EXCP_LAST = EXCP_TLBRI,
};
/* Dummy exception for conditional stores.  */
#define EXCP_SC 0x100

/*
 * This is an interrnally generated WAKE request line.
 * It is driven by the CPU itself. Raised when the MT
 * block wants to wake a VPE from an inactive state and
 * cleared when VPE goes from active to inactive.
 */
#define CPU_INTERRUPT_WAKE CPU_INTERRUPT_TGT_INT_0

int cpu_mips_exec(CPUState *cpu);
void mips_tcg_init(void);
MIPSCPU *cpu_mips_init(const char *cpu_model);
int cpu_mips_signal_handler(int host_signum, void *pinfo, void *puc);

#define cpu_init(cpu_model) CPU(cpu_mips_init(cpu_model))

/* TODO QOM'ify CPU reset and remove */
void cpu_state_reset(CPUMIPSState *s);

/* mips_timer.c */
uint32_t cpu_mips_get_random (CPUMIPSState *env);
uint32_t cpu_mips_get_count (CPUMIPSState *env);
void cpu_mips_store_count (CPUMIPSState *env, uint32_t value);
void cpu_mips_store_compare (CPUMIPSState *env, uint32_t value);
void cpu_mips_start_count(CPUMIPSState *env);
void cpu_mips_stop_count(CPUMIPSState *env);

/* mips_int.c */
void cpu_mips_soft_irq(CPUMIPSState *env, int irq, int level);

/* helper.c */
int mips_cpu_handle_mmu_fault(CPUState *cpu, vaddr address, int rw,
                              int mmu_idx);
#if !defined(CONFIG_USER_ONLY)
void r4k_invalidate_tlb (CPUMIPSState *env, int idx, int use_extra);
hwaddr cpu_mips_translate_address (CPUMIPSState *env, target_ulong address,
		                               int rw);
#endif
target_ulong exception_resume_pc (CPUMIPSState *env);
#ifdef TARGET_CHERI
void mips_dump_changed_state(CPUMIPSState *env);
#endif /* TARGET_CHERI */

/* op_helper.c */
extern unsigned int ieee_rm[];
int ieee_ex_to_mips(int xcpt);

static inline void restore_rounding_mode(CPUMIPSState *env)
{
    set_float_rounding_mode(ieee_rm[env->active_fpu.fcr31 & 3],
                            &env->active_fpu.fp_status);
}

static inline void restore_flush_mode(CPUMIPSState *env)
{
    set_flush_to_zero((env->active_fpu.fcr31 & (1 << 24)) != 0,
                      &env->active_fpu.fp_status);
}

static inline void restore_fp_status(CPUMIPSState *env)
{
    restore_rounding_mode(env);
    restore_flush_mode(env);
}

static inline void restore_msa_fp_status(CPUMIPSState *env)
{
    float_status *status = &env->active_tc.msa_fp_status;
    int rounding_mode = (env->active_tc.msacsr & MSACSR_RM_MASK) >> MSACSR_RM;
    bool flush_to_zero = (env->active_tc.msacsr & MSACSR_FS_MASK) != 0;

    set_float_rounding_mode(ieee_rm[rounding_mode], status);
    set_flush_to_zero(flush_to_zero, status);
    set_flush_inputs_to_zero(flush_to_zero, status);
}

static inline void restore_pamask(CPUMIPSState *env)
{
    if (env->hflags & MIPS_HFLAG_ELPA) {
        env->PAMask = (1ULL << env->PABITS) - 1;
    } else {
        env->PAMask = PAMASK_BASE;
    }
}

static inline void cpu_get_tb_cpu_state(CPUMIPSState *env, target_ulong *pc,
                                        target_ulong *cs_base, int *flags)
{
    *pc = env->active_tc.PC;
    *cs_base = 0;
    *flags = env->hflags & (MIPS_HFLAG_TMASK | MIPS_HFLAG_BMASK |
                            MIPS_HFLAG_HWRENA_ULR);
}

static inline int mips_vpe_active(CPUMIPSState *env)
{
    int active = 1;

    /* Check that the VPE is enabled.  */
    if (!(env->mvp->CP0_MVPControl & (1 << CP0MVPCo_EVP))) {
        active = 0;
    }
    /* Check that the VPE is activated.  */
    if (!(env->CP0_VPEConf0 & (1 << CP0VPEC0_VPA))) {
        active = 0;
    }

    /* Now verify that there are active thread contexts in the VPE.

       This assumes the CPU model will internally reschedule threads
       if the active one goes to sleep. If there are no threads available
       the active one will be in a sleeping state, and we can turn off
       the entire VPE.  */
    if (!(env->active_tc.CP0_TCStatus & (1 << CP0TCSt_A))) {
        /* TC is not activated.  */
        active = 0;
    }
    if (env->active_tc.CP0_TCHalt & 1) {
        /* TC is in halt state.  */
        active = 0;
    }

    return active;
}

#include "exec/exec-all.h"

static inline void compute_hflags(CPUMIPSState *env)
{
    env->hflags &= ~(MIPS_HFLAG_COP1X | MIPS_HFLAG_64 | MIPS_HFLAG_CP0 |
                     MIPS_HFLAG_F64 | MIPS_HFLAG_FPU | MIPS_HFLAG_KSU |
                     MIPS_HFLAG_AWRAP | MIPS_HFLAG_DSP | MIPS_HFLAG_DSPR2 |
                     MIPS_HFLAG_SBRI | MIPS_HFLAG_MSA | MIPS_HFLAG_FRE |
#ifdef TARGET_CHERI
                     MIPS_HFLAG_ELPA | MIPS_HFLAG_COP2X);
#else
                     MIPS_HFLAG_ELPA);
#endif /* TARGET_CHERI */
    if (!(env->CP0_Status & (1 << CP0St_EXL)) &&
        !(env->CP0_Status & (1 << CP0St_ERL)) &&
        !(env->hflags & MIPS_HFLAG_DM)) {
        env->hflags |= (env->CP0_Status >> CP0St_KSU) & MIPS_HFLAG_KSU;
    }
#if defined(TARGET_MIPS64)
    if ((env->insn_flags & ISA_MIPS3) &&
        (((env->hflags & MIPS_HFLAG_KSU) != MIPS_HFLAG_UM) ||
         (env->CP0_Status & (1 << CP0St_PX)) ||
         (env->CP0_Status & (1 << CP0St_UX)))) {
        env->hflags |= MIPS_HFLAG_64;
    }

    if (!(env->insn_flags & ISA_MIPS3)) {
        env->hflags |= MIPS_HFLAG_AWRAP;
    } else if (((env->hflags & MIPS_HFLAG_KSU) == MIPS_HFLAG_UM) &&
               !(env->CP0_Status & (1 << CP0St_UX))) {
        env->hflags |= MIPS_HFLAG_AWRAP;
    } else if (env->insn_flags & ISA_MIPS64R6) {
        /* Address wrapping for Supervisor and Kernel is specified in R6 */
        if ((((env->hflags & MIPS_HFLAG_KSU) == MIPS_HFLAG_SM) &&
             !(env->CP0_Status & (1 << CP0St_SX))) ||
            (((env->hflags & MIPS_HFLAG_KSU) == MIPS_HFLAG_KM) &&
             !(env->CP0_Status & (1 << CP0St_KX)))) {
            env->hflags |= MIPS_HFLAG_AWRAP;
        }
    }
#endif
    if (((env->CP0_Status & (1 << CP0St_CU0)) &&
         !(env->insn_flags & ISA_MIPS32R6)) ||
        !(env->hflags & MIPS_HFLAG_KSU)) {
        env->hflags |= MIPS_HFLAG_CP0;
    }
    if (env->CP0_Status & (1 << CP0St_CU1)) {
        env->hflags |= MIPS_HFLAG_FPU;
    }
    if (env->CP0_Status & (1 << CP0St_FR)) {
        env->hflags |= MIPS_HFLAG_F64;
    }
#ifdef TARGET_CHERI
    if (env->CP0_Status & (1 << CP0St_CU2)) {
        env->hflags |= MIPS_HFLAG_COP2X;
    }
#endif /* TARGET_CHERI */
    if (((env->hflags & MIPS_HFLAG_KSU) != MIPS_HFLAG_KM) &&
        (env->CP0_Config5 & (1 << CP0C5_SBRI))) {
        env->hflags |= MIPS_HFLAG_SBRI;
    }
    if (env->insn_flags & ASE_DSPR2) {
        /* Enables access MIPS DSP resources, now our cpu is DSP ASER2,
           so enable to access DSPR2 resources. */
        if (env->CP0_Status & (1 << CP0St_MX)) {
            env->hflags |= MIPS_HFLAG_DSP | MIPS_HFLAG_DSPR2;
        }

    } else if (env->insn_flags & ASE_DSP) {
        /* Enables access MIPS DSP resources, now our cpu is DSP ASE,
           so enable to access DSP resources. */
        if (env->CP0_Status & (1 << CP0St_MX)) {
            env->hflags |= MIPS_HFLAG_DSP;
        }

    }
    if (env->insn_flags & ISA_MIPS32R2) {
#ifdef TARGET_CHERI
        /* COP1X enables CP1X instructions such as MADD.S, and is
           orthogonal to whether the FPU is in 64-bit mode. In MIPS32,
           the COP1Xto whether the FPU is in 64-bit mode. In MIPS32, the
           COP1X instructions are always available when the FPU is
           enabled. */
        env->hflags |= MIPS_HFLAG_COP1X;
#else /* ! TARGET_CHERI */
        if (env->active_fpu.fcr0 & (1 << FCR0_F64)) {
            env->hflags |= MIPS_HFLAG_COP1X;
        }
#endif /* ! TARGET_CHERI */
    } else if (env->insn_flags & ISA_MIPS32) {
        if (env->hflags & MIPS_HFLAG_64) {
            env->hflags |= MIPS_HFLAG_COP1X;
        }
    } else if (env->insn_flags & ISA_MIPS4) {
        /* All supported MIPS IV CPUs use the XX (CU3) to enable
           and disable the MIPS IV extensions to the MIPS III ISA.
           Some other MIPS IV CPUs ignore the bit, so the check here
           would be too restrictive for them.  */
        if (env->CP0_Status & (1U << CP0St_CU3)) {
            env->hflags |= MIPS_HFLAG_COP1X;
        }
    }
    if (env->insn_flags & ASE_MSA) {
        if (env->CP0_Config5 & (1 << CP0C5_MSAEn)) {
            env->hflags |= MIPS_HFLAG_MSA;
        }
    }
    if (env->active_fpu.fcr0 & (1 << FCR0_FREP)) {
        if (env->CP0_Config5 & (1 << CP0C5_FRE)) {
            env->hflags |= MIPS_HFLAG_FRE;
        }
    }
    if (env->CP0_Config3 & (1 << CP0C3_LPA)) {
        if (env->CP0_PageGrain & (1 << CP0PG_ELPA)) {
            env->hflags |= MIPS_HFLAG_ELPA;
        }
    }
}

#ifndef CONFIG_USER_ONLY
/* Called for updates to CP0_Status.  */
static inline void sync_c0_status(CPUMIPSState *env, CPUMIPSState *cpu, int tc)
{
    int32_t tcstatus, *tcst;
    uint32_t v = cpu->CP0_Status;
    uint32_t cu, mx, asid, ksu;
    uint32_t mask = ((1 << CP0TCSt_TCU3)
                       | (1 << CP0TCSt_TCU2)
                       | (1 << CP0TCSt_TCU1)
                       | (1 << CP0TCSt_TCU0)
                       | (1 << CP0TCSt_TMX)
                       | (3 << CP0TCSt_TKSU)
                       | (0xff << CP0TCSt_TASID));

    cu = (v >> CP0St_CU0) & 0xf;
    mx = (v >> CP0St_MX) & 0x1;
    ksu = (v >> CP0St_KSU) & 0x3;
    asid = env->CP0_EntryHi & 0xff;

    tcstatus = cu << CP0TCSt_TCU0;
    tcstatus |= mx << CP0TCSt_TMX;
    tcstatus |= ksu << CP0TCSt_TKSU;
    tcstatus |= asid;

    if (tc == cpu->current_tc) {
        tcst = &cpu->active_tc.CP0_TCStatus;
    } else {
        tcst = &cpu->tcs[tc].CP0_TCStatus;
    }

    *tcst &= ~mask;
    *tcst |= tcstatus;
    compute_hflags(cpu);
}

static inline void cpu_mips_store_status(CPUMIPSState *env, target_ulong val)
{
    uint32_t mask = env->CP0_Status_rw_bitmask;

    if (env->insn_flags & ISA_MIPS32R6) {
        bool has_supervisor = extract32(mask, CP0St_KSU, 2) == 0x3;

        if (has_supervisor && extract32(val, CP0St_KSU, 2) == 0x3) {
            mask &= ~(3 << CP0St_KSU);
        }
        mask &= ~(((1 << CP0St_SR) | (1 << CP0St_NMI)) & val);
    }

    env->CP0_Status = (env->CP0_Status & ~mask) | (val & mask);
    if (env->CP0_Config3 & (1 << CP0C3_MT)) {
        sync_c0_status(env, env, env->current_tc);
    } else {
        compute_hflags(env);
    }
}

static inline void cpu_mips_store_cause(CPUMIPSState *env, target_ulong val)
{
    uint32_t mask = 0x00C00300;
    uint32_t old = env->CP0_Cause;
    int i;

    if (env->insn_flags & ISA_MIPS32R2) {
        mask |= 1 << CP0Ca_DC;
    }
    if (env->insn_flags & ISA_MIPS32R6) {
        mask &= ~((1 << CP0Ca_WP) & val);
    }

    env->CP0_Cause = (env->CP0_Cause & ~mask) | (val & mask);

    if ((old ^ env->CP0_Cause) & (1 << CP0Ca_DC)) {
        if (env->CP0_Cause & (1 << CP0Ca_DC)) {
            cpu_mips_stop_count(env);
        } else {
            cpu_mips_start_count(env);
        }
    }

    /* Set/reset software interrupts */
    for (i = 0 ; i < 2 ; i++) {
        if ((old ^ env->CP0_Cause) & (1 << (CP0Ca_IP + i))) {
            cpu_mips_soft_irq(env, i, env->CP0_Cause & (1 << (CP0Ca_IP + i)));
        }
    }
}

#if defined(TARGET_CHERI)

void cheri_tag_init(uint64_t memory_size);
void cheri_tag_invalidate(CPUMIPSState *env, target_ulong vaddr, int32_t size);
void cheri_tag_dma_invalidate(uint64_t paddr, uint64_t len);
int  cheri_tag_get(CPUMIPSState *env, target_ulong vaddr, int reg,
        hwaddr *ret_paddr);
void cheri_tag_set(CPUMIPSState *env, target_ulong vaddr, int reg);
#ifdef CHERI_MAGIC128
int  cheri_tag_get_m128(CPUMIPSState *env, target_ulong vaddr, int reg,
        uint64_t *tps, uint64_t *length);
void cheri_tag_set_m128(CPUMIPSState *env, target_ulong vaddr, int reg,
        uint8_t tag, uint64_t tps, uint64_t length);
#endif /* CHERI_MAGIC128 */

static inline void cpu_mips_store_capcause(CPUMIPSState *env, uint16_t reg_num,
        uint16_t exc_code)
{

    env->CP2_CapCause = (exc_code << 8) | (reg_num & 0xff);
}
#endif /* TARGET_CHERI */
#endif /* CONFIG_USER_ONLY */

#endif /* !defined (__MIPS_CPU_H__) */
