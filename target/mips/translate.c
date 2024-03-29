/*
 *  MIPS emulation for QEMU - main translation routines
 *
 *  Copyright (c) 2004-2005 Jocelyn Mayer
 *  Copyright (c) 2006 Marius Groeger (FPU operations)
 *  Copyright (c) 2006 Thiemo Seufer (MIPS32R2 support)
 *  Copyright (c) 2009 CodeSourcery (MIPS16 and microMIPS support)
 *  Copyright (c) 2012 Jia Liu & Dongxue Zhang (MIPS ASE DSP support)
 *  Copyright (c) 2020 Philippe Mathieu-Daudé
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
#include "tcg/tcg-op.h"
#include "exec/translator.h"
#include "exec/helper-proto.h"
#include "exec/helper-gen.h"
#include "semihosting/semihost.h"

#include "target/mips/trace.h"
#include "trace-tcg.h"
#include "exec/translator.h"
#include "exec/log.h"
#include "exec/log_instr.h"
#include "qemu/qemu-print.h"
#include "fpu_helper.h"
#include "translate.h"
#ifdef TARGET_CHERI
#include "cheri-helper-utils.h"
#endif


enum {
    /* indirect opcode tables */
    OPC_SPECIAL  = (0x00 << 26),
    OPC_REGIMM   = (0x01 << 26),
    OPC_CP0      = (0x10 << 26),
    OPC_CP2      = (0x12 << 26),
    OPC_CP3      = (0x13 << 26),
    OPC_SPECIAL2 = (0x1C << 26),
    OPC_SPECIAL3 = (0x1F << 26),
    /* arithmetic with immediate */
    OPC_ADDI     = (0x08 << 26),
    OPC_ADDIU    = (0x09 << 26),
    OPC_SLTI     = (0x0A << 26),
    OPC_SLTIU    = (0x0B << 26),
    /* logic with immediate */
    OPC_ANDI     = (0x0C << 26),
    OPC_ORI      = (0x0D << 26),
    OPC_XORI     = (0x0E << 26),
    OPC_LUI      = (0x0F << 26),
    /* arithmetic with immediate */
    OPC_DADDI    = (0x18 << 26),
    OPC_DADDIU   = (0x19 << 26),
    /* Jump and branches */
    OPC_J        = (0x02 << 26),
    OPC_JAL      = (0x03 << 26),
    OPC_BEQ      = (0x04 << 26),  /* Unconditional if rs = rt = 0 (B) */
    OPC_BEQL     = (0x14 << 26),
    OPC_BNE      = (0x05 << 26),
    OPC_BNEL     = (0x15 << 26),
    OPC_BLEZ     = (0x06 << 26),
    OPC_BLEZL    = (0x16 << 26),
    OPC_BGTZ     = (0x07 << 26),
    OPC_BGTZL    = (0x17 << 26),
    OPC_JALX     = (0x1D << 26),
    OPC_DAUI     = (0x1D << 26),
    /* Load and stores */
    OPC_LDL      = (0x1A << 26),
    OPC_LDR      = (0x1B << 26),
    OPC_LB       = (0x20 << 26),
    OPC_LH       = (0x21 << 26),
    OPC_LWL      = (0x22 << 26),
    OPC_LW       = (0x23 << 26),
    OPC_LWPC     = OPC_LW | 0x5,
    OPC_LBU      = (0x24 << 26),
    OPC_LHU      = (0x25 << 26),
    OPC_LWR      = (0x26 << 26),
    OPC_LWU      = (0x27 << 26),
    OPC_SB       = (0x28 << 26),
    OPC_SH       = (0x29 << 26),
    OPC_SWL      = (0x2A << 26),
    OPC_SW       = (0x2B << 26),
    OPC_SDL      = (0x2C << 26),
    OPC_SDR      = (0x2D << 26),
    OPC_SWR      = (0x2E << 26),
    OPC_LL       = (0x30 << 26),
    OPC_LLD      = (0x34 << 26),
    OPC_LD       = (0x37 << 26),
    OPC_LDPC     = OPC_LD | 0x5,
    OPC_SC       = (0x38 << 26),
    OPC_SCD      = (0x3C << 26),
    OPC_SD       = (0x3F << 26),
    /* Floating point load/store */
    OPC_LWC1     = (0x31 << 26),
    OPC_LWC2     = (0x32 << 26),
    OPC_LDC1     = (0x35 << 26),
    OPC_LDC2     = (0x36 << 26),
    OPC_SWC1     = (0x39 << 26),
    OPC_SWC2     = (0x3A << 26),
    OPC_SDC1     = (0x3D << 26),
    OPC_SDC2     = (0x3E << 26),
    /* Compact Branches */
    OPC_BLEZALC  = (0x06 << 26),
    OPC_BGEZALC  = (0x06 << 26),
    OPC_BGEUC    = (0x06 << 26),
    OPC_BGTZALC  = (0x07 << 26),
    OPC_BLTZALC  = (0x07 << 26),
    OPC_BLTUC    = (0x07 << 26),
    OPC_BOVC     = (0x08 << 26),
    OPC_BEQZALC  = (0x08 << 26),
    OPC_BEQC     = (0x08 << 26),
    OPC_BLEZC    = (0x16 << 26),
    OPC_BGEZC    = (0x16 << 26),
    OPC_BGEC     = (0x16 << 26),
    OPC_BGTZC    = (0x17 << 26),
    OPC_BLTZC    = (0x17 << 26),
    OPC_BLTC     = (0x17 << 26),
    OPC_BNVC     = (0x18 << 26),
    OPC_BNEZALC  = (0x18 << 26),
    OPC_BNEC     = (0x18 << 26),
    OPC_BC       = (0x32 << 26),
#if defined(TARGET_CHERI)
    /* Load Via Capability Register */
    OPC_CLOAD    = (0x32 << 26),
    /* Load Capability Register */
    OPC_CLOADC   = (0x36 << 26),
    /* Store Via Capability Register */
    OPC_CSTORE   = (0x3A << 26),
    /* Store Capability Register */
    OPC_CSTOREC  = (0x3E << 26),
#endif /* TARGET_CHERI */
    OPC_BEQZC    = (0x36 << 26),
    OPC_JIC      = (0x36 << 26),
    OPC_BALC     = (0x3A << 26),
    OPC_BNEZC    = (0x3E << 26),
    OPC_JIALC    = (0x3E << 26),
    /* MDMX ASE specific */
    OPC_MDMX     = (0x1E << 26),
    /* Cache and prefetch */
    OPC_CACHE    = (0x2F << 26),
    OPC_PREF     = (0x33 << 26),
    /* PC-relative address computation / loads */
    OPC_PCREL    = (0x3B << 26),

// XXXAR: experimental CHERI instructions
#if defined(TARGET_CHERI)
    // For the new experimental CLC we reuse the JALX (since mode switch to
    // micromips is not supported) and DAUI (add upper immediate, MIPS64R6 only)
    OPC_CLOADC_LargeImm = OPC_JALX,
    // For the new large immediate CStoreC (may not be needed but symmetry is
    // nice) we use
    OPC_CSTOREC_LargeImm = OPC_MDMX,

    // why can't this table be ordered by opcode number rather than group?
    // would make it a lot easier to find conflicting values
#endif
};

/* PC-relative address computation / loads  */
#define MASK_OPC_PCREL_TOP2BITS(op) (MASK_OP_MAJOR(op) | (op & (3 << 19)))
#define MASK_OPC_PCREL_TOP5BITS(op) (MASK_OP_MAJOR(op) | (op & (0x1f << 16)))
enum {
    /* Instructions determined by bits 19 and 20 */
    OPC_ADDIUPC = OPC_PCREL | (0 << 19),
    R6_OPC_LWPC = OPC_PCREL | (1 << 19),
    OPC_LWUPC   = OPC_PCREL | (2 << 19),

    /* Instructions determined by bits 16 ... 20 */
    OPC_AUIPC   = OPC_PCREL | (0x1e << 16),
    OPC_ALUIPC  = OPC_PCREL | (0x1f << 16),

    /* Other */
    R6_OPC_LDPC = OPC_PCREL | (6 << 18),
};

/* MIPS special opcodes */
#define MASK_SPECIAL(op)            (MASK_OP_MAJOR(op) | (op & 0x3F))

enum {
    /* Shifts */
    OPC_SLL      = 0x00 | OPC_SPECIAL,
    /* NOP is SLL r0, r0, 0   */
    /* SSNOP is SLL r0, r0, 1 */
    /* EHB is SLL r0, r0, 3 */
    OPC_SRL      = 0x02 | OPC_SPECIAL, /* also ROTR */
    OPC_ROTR     = OPC_SRL | (1 << 21),
    OPC_SRA      = 0x03 | OPC_SPECIAL,
    OPC_SLLV     = 0x04 | OPC_SPECIAL,
    OPC_SRLV     = 0x06 | OPC_SPECIAL, /* also ROTRV */
    OPC_ROTRV    = OPC_SRLV | (1 << 6),
    OPC_SRAV     = 0x07 | OPC_SPECIAL,
    OPC_DSLLV    = 0x14 | OPC_SPECIAL,
    OPC_DSRLV    = 0x16 | OPC_SPECIAL, /* also DROTRV */
    OPC_DROTRV   = OPC_DSRLV | (1 << 6),
    OPC_DSRAV    = 0x17 | OPC_SPECIAL,
    OPC_DSLL     = 0x38 | OPC_SPECIAL,
    OPC_DSRL     = 0x3A | OPC_SPECIAL, /* also DROTR */
    OPC_DROTR    = OPC_DSRL | (1 << 21),
    OPC_DSRA     = 0x3B | OPC_SPECIAL,
    OPC_DSLL32   = 0x3C | OPC_SPECIAL,
    OPC_DSRL32   = 0x3E | OPC_SPECIAL, /* also DROTR32 */
    OPC_DROTR32  = OPC_DSRL32 | (1 << 21),
    OPC_DSRA32   = 0x3F | OPC_SPECIAL,
    /* Multiplication / division */
    OPC_MULT     = 0x18 | OPC_SPECIAL,
    OPC_MULTU    = 0x19 | OPC_SPECIAL,
    OPC_DIV      = 0x1A | OPC_SPECIAL,
    OPC_DIVU     = 0x1B | OPC_SPECIAL,
    OPC_DMULT    = 0x1C | OPC_SPECIAL,
    OPC_DMULTU   = 0x1D | OPC_SPECIAL,
    OPC_DDIV     = 0x1E | OPC_SPECIAL,
    OPC_DDIVU    = 0x1F | OPC_SPECIAL,

    /* 2 registers arithmetic / logic */
    OPC_ADD      = 0x20 | OPC_SPECIAL,
    OPC_ADDU     = 0x21 | OPC_SPECIAL,
    OPC_SUB      = 0x22 | OPC_SPECIAL,
    OPC_SUBU     = 0x23 | OPC_SPECIAL,
    OPC_AND      = 0x24 | OPC_SPECIAL,
    OPC_OR       = 0x25 | OPC_SPECIAL,
    OPC_XOR      = 0x26 | OPC_SPECIAL,
    OPC_NOR      = 0x27 | OPC_SPECIAL,
    OPC_SLT      = 0x2A | OPC_SPECIAL,
    OPC_SLTU     = 0x2B | OPC_SPECIAL,
    OPC_DADD     = 0x2C | OPC_SPECIAL,
    OPC_DADDU    = 0x2D | OPC_SPECIAL,
    OPC_DSUB     = 0x2E | OPC_SPECIAL,
    OPC_DSUBU    = 0x2F | OPC_SPECIAL,
    /* Jumps */
    OPC_JR       = 0x08 | OPC_SPECIAL, /* Also JR.HB */
    OPC_JALR     = 0x09 | OPC_SPECIAL, /* Also JALR.HB */
    /* Traps */
    OPC_TGE      = 0x30 | OPC_SPECIAL,
    OPC_TGEU     = 0x31 | OPC_SPECIAL,
    OPC_TLT      = 0x32 | OPC_SPECIAL,
    OPC_TLTU     = 0x33 | OPC_SPECIAL,
    OPC_TEQ      = 0x34 | OPC_SPECIAL,
    OPC_TNE      = 0x36 | OPC_SPECIAL,
    /* HI / LO registers load & stores */
    OPC_MFHI     = 0x10 | OPC_SPECIAL,
    OPC_MTHI     = 0x11 | OPC_SPECIAL,
    OPC_MFLO     = 0x12 | OPC_SPECIAL,
    OPC_MTLO     = 0x13 | OPC_SPECIAL,
    /* Conditional moves */
    OPC_MOVZ     = 0x0A | OPC_SPECIAL,
    OPC_MOVN     = 0x0B | OPC_SPECIAL,

    OPC_SELEQZ   = 0x35 | OPC_SPECIAL,
    OPC_SELNEZ   = 0x37 | OPC_SPECIAL,

    OPC_MOVCI    = 0x01 | OPC_SPECIAL,

    /* Special */
    OPC_PMON     = 0x05 | OPC_SPECIAL, /* unofficial */
    OPC_SYSCALL  = 0x0C | OPC_SPECIAL,
    OPC_BREAK    = 0x0D | OPC_SPECIAL,
    OPC_SPIM     = 0x0E | OPC_SPECIAL, /* unofficial */
    OPC_SYNC     = 0x0F | OPC_SPECIAL,

    OPC_SPECIAL28_RESERVED = 0x28 | OPC_SPECIAL,
    OPC_SPECIAL29_RESERVED = 0x29 | OPC_SPECIAL,
    OPC_SPECIAL39_RESERVED = 0x39 | OPC_SPECIAL,
    OPC_SPECIAL3D_RESERVED = 0x3D | OPC_SPECIAL,
};

/*
 * R6 Multiply and Divide instructions have the same opcode
 * and function field as legacy OPC_MULT[U]/OPC_DIV[U]
 */
#define MASK_R6_MULDIV(op)          (MASK_SPECIAL(op) | (op & (0x7ff)))

enum {
    R6_OPC_MUL   = OPC_MULT  | (2 << 6),
    R6_OPC_MUH   = OPC_MULT  | (3 << 6),
    R6_OPC_MULU  = OPC_MULTU | (2 << 6),
    R6_OPC_MUHU  = OPC_MULTU | (3 << 6),
    R6_OPC_DIV   = OPC_DIV   | (2 << 6),
    R6_OPC_MOD   = OPC_DIV   | (3 << 6),
    R6_OPC_DIVU  = OPC_DIVU  | (2 << 6),
    R6_OPC_MODU  = OPC_DIVU  | (3 << 6),

    R6_OPC_DMUL   = OPC_DMULT  | (2 << 6),
    R6_OPC_DMUH   = OPC_DMULT  | (3 << 6),
    R6_OPC_DMULU  = OPC_DMULTU | (2 << 6),
    R6_OPC_DMUHU  = OPC_DMULTU | (3 << 6),
    R6_OPC_DDIV   = OPC_DDIV   | (2 << 6),
    R6_OPC_DMOD   = OPC_DDIV   | (3 << 6),
    R6_OPC_DDIVU  = OPC_DDIVU  | (2 << 6),
    R6_OPC_DMODU  = OPC_DDIVU  | (3 << 6),

    R6_OPC_CLZ      = 0x10 | OPC_SPECIAL,
    R6_OPC_CLO      = 0x11 | OPC_SPECIAL,
    R6_OPC_DCLZ     = 0x12 | OPC_SPECIAL,
    R6_OPC_DCLO     = 0x13 | OPC_SPECIAL,
    R6_OPC_SDBBP    = 0x0e | OPC_SPECIAL,
};

/* Multiplication variants of the vr54xx. */
#define MASK_MUL_VR54XX(op)         (MASK_SPECIAL(op) | (op & (0x1F << 6)))

enum {
    OPC_VR54XX_MULS    = (0x03 << 6) | OPC_MULT,
    OPC_VR54XX_MULSU   = (0x03 << 6) | OPC_MULTU,
    OPC_VR54XX_MACC    = (0x05 << 6) | OPC_MULT,
    OPC_VR54XX_MACCU   = (0x05 << 6) | OPC_MULTU,
    OPC_VR54XX_MSAC    = (0x07 << 6) | OPC_MULT,
    OPC_VR54XX_MSACU   = (0x07 << 6) | OPC_MULTU,
    OPC_VR54XX_MULHI   = (0x09 << 6) | OPC_MULT,
    OPC_VR54XX_MULHIU  = (0x09 << 6) | OPC_MULTU,
    OPC_VR54XX_MULSHI  = (0x0B << 6) | OPC_MULT,
    OPC_VR54XX_MULSHIU = (0x0B << 6) | OPC_MULTU,
    OPC_VR54XX_MACCHI  = (0x0D << 6) | OPC_MULT,
    OPC_VR54XX_MACCHIU = (0x0D << 6) | OPC_MULTU,
    OPC_VR54XX_MSACHI  = (0x0F << 6) | OPC_MULT,
    OPC_VR54XX_MSACHIU = (0x0F << 6) | OPC_MULTU,
};

/* REGIMM (rt field) opcodes */
#define MASK_REGIMM(op)             (MASK_OP_MAJOR(op) | (op & (0x1F << 16)))

enum {
    OPC_BLTZ     = (0x00 << 16) | OPC_REGIMM,
    OPC_BLTZL    = (0x02 << 16) | OPC_REGIMM,
    OPC_BGEZ     = (0x01 << 16) | OPC_REGIMM,
    OPC_BGEZL    = (0x03 << 16) | OPC_REGIMM,
    OPC_BLTZAL   = (0x10 << 16) | OPC_REGIMM,
    OPC_BLTZALL  = (0x12 << 16) | OPC_REGIMM,
    OPC_BGEZAL   = (0x11 << 16) | OPC_REGIMM,
    OPC_BGEZALL  = (0x13 << 16) | OPC_REGIMM,
    OPC_TGEI     = (0x08 << 16) | OPC_REGIMM,
    OPC_TGEIU    = (0x09 << 16) | OPC_REGIMM,
    OPC_TLTI     = (0x0A << 16) | OPC_REGIMM,
    OPC_TLTIU    = (0x0B << 16) | OPC_REGIMM,
    OPC_TEQI     = (0x0C << 16) | OPC_REGIMM,
    OPC_TNEI     = (0x0E << 16) | OPC_REGIMM,
    OPC_SIGRIE   = (0x17 << 16) | OPC_REGIMM,
    OPC_SYNCI    = (0x1F << 16) | OPC_REGIMM,

    OPC_DAHI     = (0x06 << 16) | OPC_REGIMM,
    OPC_DATI     = (0x1e << 16) | OPC_REGIMM,
};

/* Special2 opcodes */
#define MASK_SPECIAL2(op)           (MASK_OP_MAJOR(op) | (op & 0x3F))

enum {
    /* Multiply & xxx operations */
    OPC_MADD     = 0x00 | OPC_SPECIAL2,
    OPC_MADDU    = 0x01 | OPC_SPECIAL2,
    OPC_MUL      = 0x02 | OPC_SPECIAL2,
    OPC_MSUB     = 0x04 | OPC_SPECIAL2,
    OPC_MSUBU    = 0x05 | OPC_SPECIAL2,
    /* Loongson 2F */
    OPC_MULT_G_2F   = 0x10 | OPC_SPECIAL2,
    OPC_DMULT_G_2F  = 0x11 | OPC_SPECIAL2,
    OPC_MULTU_G_2F  = 0x12 | OPC_SPECIAL2,
    OPC_DMULTU_G_2F = 0x13 | OPC_SPECIAL2,
    OPC_DIV_G_2F    = 0x14 | OPC_SPECIAL2,
    OPC_DDIV_G_2F   = 0x15 | OPC_SPECIAL2,
    OPC_DIVU_G_2F   = 0x16 | OPC_SPECIAL2,
    OPC_DDIVU_G_2F  = 0x17 | OPC_SPECIAL2,
    OPC_MOD_G_2F    = 0x1c | OPC_SPECIAL2,
    OPC_DMOD_G_2F   = 0x1d | OPC_SPECIAL2,
    OPC_MODU_G_2F   = 0x1e | OPC_SPECIAL2,
    OPC_DMODU_G_2F  = 0x1f | OPC_SPECIAL2,
    /* Misc */
    OPC_CLZ      = 0x20 | OPC_SPECIAL2,
    OPC_CLO      = 0x21 | OPC_SPECIAL2,
    OPC_DCLZ     = 0x24 | OPC_SPECIAL2,
    OPC_DCLO     = 0x25 | OPC_SPECIAL2,
    /* Special */
    OPC_SDBBP    = 0x3F | OPC_SPECIAL2,
};

/* Special3 opcodes */
#define MASK_SPECIAL3(op)           (MASK_OP_MAJOR(op) | (op & 0x3F))

enum {
    OPC_EXT      = 0x00 | OPC_SPECIAL3,
    OPC_DEXTM    = 0x01 | OPC_SPECIAL3,
    OPC_DEXTU    = 0x02 | OPC_SPECIAL3,
    OPC_DEXT     = 0x03 | OPC_SPECIAL3,
    OPC_INS      = 0x04 | OPC_SPECIAL3,
    OPC_DINSM    = 0x05 | OPC_SPECIAL3,
    OPC_DINSU    = 0x06 | OPC_SPECIAL3,
    OPC_DINS     = 0x07 | OPC_SPECIAL3,
    OPC_FORK     = 0x08 | OPC_SPECIAL3,
    OPC_YIELD    = 0x09 | OPC_SPECIAL3,
    OPC_BSHFL    = 0x20 | OPC_SPECIAL3,
    OPC_DBSHFL   = 0x24 | OPC_SPECIAL3,
    OPC_RDHWR    = 0x3B | OPC_SPECIAL3,
    OPC_GINV     = 0x3D | OPC_SPECIAL3,

    /* Loongson 2E */
    OPC_MULT_G_2E   = 0x18 | OPC_SPECIAL3,
    OPC_MULTU_G_2E  = 0x19 | OPC_SPECIAL3,
    OPC_DIV_G_2E    = 0x1A | OPC_SPECIAL3,
    OPC_DIVU_G_2E   = 0x1B | OPC_SPECIAL3,
    OPC_DMULT_G_2E  = 0x1C | OPC_SPECIAL3,
    OPC_DMULTU_G_2E = 0x1D | OPC_SPECIAL3,
    OPC_DDIV_G_2E   = 0x1E | OPC_SPECIAL3,
    OPC_DDIVU_G_2E  = 0x1F | OPC_SPECIAL3,
    OPC_MOD_G_2E    = 0x22 | OPC_SPECIAL3,
    OPC_MODU_G_2E   = 0x23 | OPC_SPECIAL3,
    OPC_DMOD_G_2E   = 0x26 | OPC_SPECIAL3,
    OPC_DMODU_G_2E  = 0x27 | OPC_SPECIAL3,

    /* MIPS DSP Load */
    OPC_LX_DSP         = 0x0A | OPC_SPECIAL3,
    /* MIPS DSP Arithmetic */
    OPC_ADDU_QB_DSP    = 0x10 | OPC_SPECIAL3,
    OPC_ADDU_OB_DSP    = 0x14 | OPC_SPECIAL3,
    OPC_ABSQ_S_PH_DSP  = 0x12 | OPC_SPECIAL3,
    OPC_ABSQ_S_QH_DSP  = 0x16 | OPC_SPECIAL3,
    /* OPC_ADDUH_QB_DSP is same as OPC_MULT_G_2E.  */
    /* OPC_ADDUH_QB_DSP   = 0x18 | OPC_SPECIAL3,  */
    OPC_CMPU_EQ_QB_DSP = 0x11 | OPC_SPECIAL3,
    OPC_CMPU_EQ_OB_DSP = 0x15 | OPC_SPECIAL3,
    /* MIPS DSP GPR-Based Shift Sub-class */
    OPC_SHLL_QB_DSP    = 0x13 | OPC_SPECIAL3,
    OPC_SHLL_OB_DSP    = 0x17 | OPC_SPECIAL3,
    /* MIPS DSP Multiply Sub-class insns */
    /* OPC_MUL_PH_DSP is same as OPC_ADDUH_QB_DSP.  */
    /* OPC_MUL_PH_DSP     = 0x18 | OPC_SPECIAL3,  */
    OPC_DPA_W_PH_DSP   = 0x30 | OPC_SPECIAL3,
    OPC_DPAQ_W_QH_DSP  = 0x34 | OPC_SPECIAL3,
    /* DSP Bit/Manipulation Sub-class */
    OPC_INSV_DSP       = 0x0C | OPC_SPECIAL3,
    OPC_DINSV_DSP      = 0x0D | OPC_SPECIAL3,
    /* MIPS DSP Append Sub-class */
    OPC_APPEND_DSP     = 0x31 | OPC_SPECIAL3,
    OPC_DAPPEND_DSP    = 0x35 | OPC_SPECIAL3,
    /* MIPS DSP Accumulator and DSPControl Access Sub-class */
    OPC_EXTR_W_DSP     = 0x38 | OPC_SPECIAL3,
    OPC_DEXTR_W_DSP    = 0x3C | OPC_SPECIAL3,

    /* EVA */
    OPC_LWLE           = 0x19 | OPC_SPECIAL3,
    OPC_LWRE           = 0x1A | OPC_SPECIAL3,
    OPC_CACHEE         = 0x1B | OPC_SPECIAL3,
    OPC_SBE            = 0x1C | OPC_SPECIAL3,
    OPC_SHE            = 0x1D | OPC_SPECIAL3,
    OPC_SCE            = 0x1E | OPC_SPECIAL3,
    OPC_SWE            = 0x1F | OPC_SPECIAL3,
    OPC_SWLE           = 0x21 | OPC_SPECIAL3,
    OPC_SWRE           = 0x22 | OPC_SPECIAL3,
    OPC_PREFE          = 0x23 | OPC_SPECIAL3,
    OPC_LBUE           = 0x28 | OPC_SPECIAL3,
    OPC_LHUE           = 0x29 | OPC_SPECIAL3,
    OPC_LBE            = 0x2C | OPC_SPECIAL3,
    OPC_LHE            = 0x2D | OPC_SPECIAL3,
    OPC_LLE            = 0x2E | OPC_SPECIAL3,
    OPC_LWE            = 0x2F | OPC_SPECIAL3,

    /* R6 */
    R6_OPC_PREF        = 0x35 | OPC_SPECIAL3,
    R6_OPC_CACHE       = 0x25 | OPC_SPECIAL3,
    R6_OPC_LL          = 0x36 | OPC_SPECIAL3,
    R6_OPC_SC          = 0x26 | OPC_SPECIAL3,
    R6_OPC_LLD         = 0x37 | OPC_SPECIAL3,
    R6_OPC_SCD         = 0x27 | OPC_SPECIAL3,
};

/* Loongson EXT load/store quad word opcodes */
#define MASK_LOONGSON_GSLSQ(op)           (MASK_OP_MAJOR(op) | (op & 0x8020))
enum {
    OPC_GSLQ        = 0x0020 | OPC_LWC2,
    OPC_GSLQC1      = 0x8020 | OPC_LWC2,
    OPC_GSSHFL      = OPC_LWC2,
    OPC_GSSQ        = 0x0020 | OPC_SWC2,
    OPC_GSSQC1      = 0x8020 | OPC_SWC2,
    OPC_GSSHFS      = OPC_SWC2,
};

/* Loongson EXT shifted load/store opcodes */
#define MASK_LOONGSON_GSSHFLS(op)         (MASK_OP_MAJOR(op) | (op & 0xc03f))
enum {
    OPC_GSLWLC1     = 0x4 | OPC_GSSHFL,
    OPC_GSLWRC1     = 0x5 | OPC_GSSHFL,
    OPC_GSLDLC1     = 0x6 | OPC_GSSHFL,
    OPC_GSLDRC1     = 0x7 | OPC_GSSHFL,
    OPC_GSSWLC1     = 0x4 | OPC_GSSHFS,
    OPC_GSSWRC1     = 0x5 | OPC_GSSHFS,
    OPC_GSSDLC1     = 0x6 | OPC_GSSHFS,
    OPC_GSSDRC1     = 0x7 | OPC_GSSHFS,
};

/* Loongson EXT LDC2/SDC2 opcodes */
#define MASK_LOONGSON_LSDC2(op)           (MASK_OP_MAJOR(op) | (op & 0x7))

enum {
    OPC_GSLBX      = 0x0 | OPC_LDC2,
    OPC_GSLHX      = 0x1 | OPC_LDC2,
    OPC_GSLWX      = 0x2 | OPC_LDC2,
    OPC_GSLDX      = 0x3 | OPC_LDC2,
    OPC_GSLWXC1    = 0x6 | OPC_LDC2,
    OPC_GSLDXC1    = 0x7 | OPC_LDC2,
    OPC_GSSBX      = 0x0 | OPC_SDC2,
    OPC_GSSHX      = 0x1 | OPC_SDC2,
    OPC_GSSWX      = 0x2 | OPC_SDC2,
    OPC_GSSDX      = 0x3 | OPC_SDC2,
    OPC_GSSWXC1    = 0x6 | OPC_SDC2,
    OPC_GSSDXC1    = 0x7 | OPC_SDC2,
};

/* BSHFL opcodes */
#define MASK_BSHFL(op)              (MASK_SPECIAL3(op) | (op & (0x1F << 6)))

enum {
    OPC_WSBH      = (0x02 << 6) | OPC_BSHFL,
    OPC_SEB       = (0x10 << 6) | OPC_BSHFL,
    OPC_SEH       = (0x18 << 6) | OPC_BSHFL,
    OPC_ALIGN     = (0x08 << 6) | OPC_BSHFL, /* 010.bp (010.00 to 010.11) */
    OPC_ALIGN_1   = (0x09 << 6) | OPC_BSHFL,
    OPC_ALIGN_2   = (0x0A << 6) | OPC_BSHFL,
    OPC_ALIGN_3   = (0x0B << 6) | OPC_BSHFL,
    OPC_BITSWAP   = (0x00 << 6) | OPC_BSHFL  /* 00000 */
};

/* DBSHFL opcodes */
#define MASK_DBSHFL(op)             (MASK_SPECIAL3(op) | (op & (0x1F << 6)))

enum {
    OPC_DSBH       = (0x02 << 6) | OPC_DBSHFL,
    OPC_DSHD       = (0x05 << 6) | OPC_DBSHFL,
    OPC_DALIGN     = (0x08 << 6) | OPC_DBSHFL, /* 01.bp (01.000 to 01.111) */
    OPC_DALIGN_1   = (0x09 << 6) | OPC_DBSHFL,
    OPC_DALIGN_2   = (0x0A << 6) | OPC_DBSHFL,
    OPC_DALIGN_3   = (0x0B << 6) | OPC_DBSHFL,
    OPC_DALIGN_4   = (0x0C << 6) | OPC_DBSHFL,
    OPC_DALIGN_5   = (0x0D << 6) | OPC_DBSHFL,
    OPC_DALIGN_6   = (0x0E << 6) | OPC_DBSHFL,
    OPC_DALIGN_7   = (0x0F << 6) | OPC_DBSHFL,
    OPC_DBITSWAP   = (0x00 << 6) | OPC_DBSHFL, /* 00000 */
};

/* MIPS DSP REGIMM opcodes */
enum {
    OPC_BPOSGE32 = (0x1C << 16) | OPC_REGIMM,
    OPC_BPOSGE64 = (0x1D << 16) | OPC_REGIMM,
};

#define MASK_LX(op)                 (MASK_SPECIAL3(op) | (op & (0x1F << 6)))
/* MIPS DSP Load */
enum {
    OPC_LBUX = (0x06 << 6) | OPC_LX_DSP,
    OPC_LHX  = (0x04 << 6) | OPC_LX_DSP,
    OPC_LWX  = (0x00 << 6) | OPC_LX_DSP,
    OPC_LDX = (0x08 << 6) | OPC_LX_DSP,
};

#define MASK_ADDU_QB(op)            (MASK_SPECIAL3(op) | (op & (0x1F << 6)))
enum {
    /* MIPS DSP Arithmetic Sub-class */
    OPC_ADDQ_PH        = (0x0A << 6) | OPC_ADDU_QB_DSP,
    OPC_ADDQ_S_PH      = (0x0E << 6) | OPC_ADDU_QB_DSP,
    OPC_ADDQ_S_W       = (0x16 << 6) | OPC_ADDU_QB_DSP,
    OPC_ADDU_QB        = (0x00 << 6) | OPC_ADDU_QB_DSP,
    OPC_ADDU_S_QB      = (0x04 << 6) | OPC_ADDU_QB_DSP,
    OPC_ADDU_PH        = (0x08 << 6) | OPC_ADDU_QB_DSP,
    OPC_ADDU_S_PH      = (0x0C << 6) | OPC_ADDU_QB_DSP,
    OPC_SUBQ_PH        = (0x0B << 6) | OPC_ADDU_QB_DSP,
    OPC_SUBQ_S_PH      = (0x0F << 6) | OPC_ADDU_QB_DSP,
    OPC_SUBQ_S_W       = (0x17 << 6) | OPC_ADDU_QB_DSP,
    OPC_SUBU_QB        = (0x01 << 6) | OPC_ADDU_QB_DSP,
    OPC_SUBU_S_QB      = (0x05 << 6) | OPC_ADDU_QB_DSP,
    OPC_SUBU_PH        = (0x09 << 6) | OPC_ADDU_QB_DSP,
    OPC_SUBU_S_PH      = (0x0D << 6) | OPC_ADDU_QB_DSP,
    OPC_ADDSC          = (0x10 << 6) | OPC_ADDU_QB_DSP,
    OPC_ADDWC          = (0x11 << 6) | OPC_ADDU_QB_DSP,
    OPC_MODSUB         = (0x12 << 6) | OPC_ADDU_QB_DSP,
    OPC_RADDU_W_QB     = (0x14 << 6) | OPC_ADDU_QB_DSP,
    /* MIPS DSP Multiply Sub-class insns */
    OPC_MULEU_S_PH_QBL = (0x06 << 6) | OPC_ADDU_QB_DSP,
    OPC_MULEU_S_PH_QBR = (0x07 << 6) | OPC_ADDU_QB_DSP,
    OPC_MULQ_RS_PH     = (0x1F << 6) | OPC_ADDU_QB_DSP,
    OPC_MULEQ_S_W_PHL  = (0x1C << 6) | OPC_ADDU_QB_DSP,
    OPC_MULEQ_S_W_PHR  = (0x1D << 6) | OPC_ADDU_QB_DSP,
    OPC_MULQ_S_PH      = (0x1E << 6) | OPC_ADDU_QB_DSP,
};

#define OPC_ADDUH_QB_DSP OPC_MULT_G_2E
#define MASK_ADDUH_QB(op)           (MASK_SPECIAL3(op) | (op & (0x1F << 6)))
enum {
    /* MIPS DSP Arithmetic Sub-class */
    OPC_ADDUH_QB   = (0x00 << 6) | OPC_ADDUH_QB_DSP,
    OPC_ADDUH_R_QB = (0x02 << 6) | OPC_ADDUH_QB_DSP,
    OPC_ADDQH_PH   = (0x08 << 6) | OPC_ADDUH_QB_DSP,
    OPC_ADDQH_R_PH = (0x0A << 6) | OPC_ADDUH_QB_DSP,
    OPC_ADDQH_W    = (0x10 << 6) | OPC_ADDUH_QB_DSP,
    OPC_ADDQH_R_W  = (0x12 << 6) | OPC_ADDUH_QB_DSP,
    OPC_SUBUH_QB   = (0x01 << 6) | OPC_ADDUH_QB_DSP,
    OPC_SUBUH_R_QB = (0x03 << 6) | OPC_ADDUH_QB_DSP,
    OPC_SUBQH_PH   = (0x09 << 6) | OPC_ADDUH_QB_DSP,
    OPC_SUBQH_R_PH = (0x0B << 6) | OPC_ADDUH_QB_DSP,
    OPC_SUBQH_W    = (0x11 << 6) | OPC_ADDUH_QB_DSP,
    OPC_SUBQH_R_W  = (0x13 << 6) | OPC_ADDUH_QB_DSP,
    /* MIPS DSP Multiply Sub-class insns */
    OPC_MUL_PH     = (0x0C << 6) | OPC_ADDUH_QB_DSP,
    OPC_MUL_S_PH   = (0x0E << 6) | OPC_ADDUH_QB_DSP,
    OPC_MULQ_S_W   = (0x16 << 6) | OPC_ADDUH_QB_DSP,
    OPC_MULQ_RS_W  = (0x17 << 6) | OPC_ADDUH_QB_DSP,
};

#define MASK_ABSQ_S_PH(op)          (MASK_SPECIAL3(op) | (op & (0x1F << 6)))
enum {
    /* MIPS DSP Arithmetic Sub-class */
    OPC_ABSQ_S_QB       = (0x01 << 6) | OPC_ABSQ_S_PH_DSP,
    OPC_ABSQ_S_PH       = (0x09 << 6) | OPC_ABSQ_S_PH_DSP,
    OPC_ABSQ_S_W        = (0x11 << 6) | OPC_ABSQ_S_PH_DSP,
    OPC_PRECEQ_W_PHL    = (0x0C << 6) | OPC_ABSQ_S_PH_DSP,
    OPC_PRECEQ_W_PHR    = (0x0D << 6) | OPC_ABSQ_S_PH_DSP,
    OPC_PRECEQU_PH_QBL  = (0x04 << 6) | OPC_ABSQ_S_PH_DSP,
    OPC_PRECEQU_PH_QBR  = (0x05 << 6) | OPC_ABSQ_S_PH_DSP,
    OPC_PRECEQU_PH_QBLA = (0x06 << 6) | OPC_ABSQ_S_PH_DSP,
    OPC_PRECEQU_PH_QBRA = (0x07 << 6) | OPC_ABSQ_S_PH_DSP,
    OPC_PRECEU_PH_QBL   = (0x1C << 6) | OPC_ABSQ_S_PH_DSP,
    OPC_PRECEU_PH_QBR   = (0x1D << 6) | OPC_ABSQ_S_PH_DSP,
    OPC_PRECEU_PH_QBLA  = (0x1E << 6) | OPC_ABSQ_S_PH_DSP,
    OPC_PRECEU_PH_QBRA  = (0x1F << 6) | OPC_ABSQ_S_PH_DSP,
    /* DSP Bit/Manipulation Sub-class */
    OPC_BITREV          = (0x1B << 6) | OPC_ABSQ_S_PH_DSP,
    OPC_REPL_QB         = (0x02 << 6) | OPC_ABSQ_S_PH_DSP,
    OPC_REPLV_QB        = (0x03 << 6) | OPC_ABSQ_S_PH_DSP,
    OPC_REPL_PH         = (0x0A << 6) | OPC_ABSQ_S_PH_DSP,
    OPC_REPLV_PH        = (0x0B << 6) | OPC_ABSQ_S_PH_DSP,
};

#define MASK_CMPU_EQ_QB(op)         (MASK_SPECIAL3(op) | (op & (0x1F << 6)))
enum {
    /* MIPS DSP Arithmetic Sub-class */
    OPC_PRECR_QB_PH      = (0x0D << 6) | OPC_CMPU_EQ_QB_DSP,
    OPC_PRECRQ_QB_PH     = (0x0C << 6) | OPC_CMPU_EQ_QB_DSP,
    OPC_PRECR_SRA_PH_W   = (0x1E << 6) | OPC_CMPU_EQ_QB_DSP,
    OPC_PRECR_SRA_R_PH_W = (0x1F << 6) | OPC_CMPU_EQ_QB_DSP,
    OPC_PRECRQ_PH_W      = (0x14 << 6) | OPC_CMPU_EQ_QB_DSP,
    OPC_PRECRQ_RS_PH_W   = (0x15 << 6) | OPC_CMPU_EQ_QB_DSP,
    OPC_PRECRQU_S_QB_PH  = (0x0F << 6) | OPC_CMPU_EQ_QB_DSP,
    /* DSP Compare-Pick Sub-class */
    OPC_CMPU_EQ_QB       = (0x00 << 6) | OPC_CMPU_EQ_QB_DSP,
    OPC_CMPU_LT_QB       = (0x01 << 6) | OPC_CMPU_EQ_QB_DSP,
    OPC_CMPU_LE_QB       = (0x02 << 6) | OPC_CMPU_EQ_QB_DSP,
    OPC_CMPGU_EQ_QB      = (0x04 << 6) | OPC_CMPU_EQ_QB_DSP,
    OPC_CMPGU_LT_QB      = (0x05 << 6) | OPC_CMPU_EQ_QB_DSP,
    OPC_CMPGU_LE_QB      = (0x06 << 6) | OPC_CMPU_EQ_QB_DSP,
    OPC_CMPGDU_EQ_QB     = (0x18 << 6) | OPC_CMPU_EQ_QB_DSP,
    OPC_CMPGDU_LT_QB     = (0x19 << 6) | OPC_CMPU_EQ_QB_DSP,
    OPC_CMPGDU_LE_QB     = (0x1A << 6) | OPC_CMPU_EQ_QB_DSP,
    OPC_CMP_EQ_PH        = (0x08 << 6) | OPC_CMPU_EQ_QB_DSP,
    OPC_CMP_LT_PH        = (0x09 << 6) | OPC_CMPU_EQ_QB_DSP,
    OPC_CMP_LE_PH        = (0x0A << 6) | OPC_CMPU_EQ_QB_DSP,
    OPC_PICK_QB          = (0x03 << 6) | OPC_CMPU_EQ_QB_DSP,
    OPC_PICK_PH          = (0x0B << 6) | OPC_CMPU_EQ_QB_DSP,
    OPC_PACKRL_PH        = (0x0E << 6) | OPC_CMPU_EQ_QB_DSP,
};

#define MASK_SHLL_QB(op)            (MASK_SPECIAL3(op) | (op & (0x1F << 6)))
enum {
    /* MIPS DSP GPR-Based Shift Sub-class */
    OPC_SHLL_QB    = (0x00 << 6) | OPC_SHLL_QB_DSP,
    OPC_SHLLV_QB   = (0x02 << 6) | OPC_SHLL_QB_DSP,
    OPC_SHLL_PH    = (0x08 << 6) | OPC_SHLL_QB_DSP,
    OPC_SHLLV_PH   = (0x0A << 6) | OPC_SHLL_QB_DSP,
    OPC_SHLL_S_PH  = (0x0C << 6) | OPC_SHLL_QB_DSP,
    OPC_SHLLV_S_PH = (0x0E << 6) | OPC_SHLL_QB_DSP,
    OPC_SHLL_S_W   = (0x14 << 6) | OPC_SHLL_QB_DSP,
    OPC_SHLLV_S_W  = (0x16 << 6) | OPC_SHLL_QB_DSP,
    OPC_SHRL_QB    = (0x01 << 6) | OPC_SHLL_QB_DSP,
    OPC_SHRLV_QB   = (0x03 << 6) | OPC_SHLL_QB_DSP,
    OPC_SHRL_PH    = (0x19 << 6) | OPC_SHLL_QB_DSP,
    OPC_SHRLV_PH   = (0x1B << 6) | OPC_SHLL_QB_DSP,
    OPC_SHRA_QB    = (0x04 << 6) | OPC_SHLL_QB_DSP,
    OPC_SHRA_R_QB  = (0x05 << 6) | OPC_SHLL_QB_DSP,
    OPC_SHRAV_QB   = (0x06 << 6) | OPC_SHLL_QB_DSP,
    OPC_SHRAV_R_QB = (0x07 << 6) | OPC_SHLL_QB_DSP,
    OPC_SHRA_PH    = (0x09 << 6) | OPC_SHLL_QB_DSP,
    OPC_SHRAV_PH   = (0x0B << 6) | OPC_SHLL_QB_DSP,
    OPC_SHRA_R_PH  = (0x0D << 6) | OPC_SHLL_QB_DSP,
    OPC_SHRAV_R_PH = (0x0F << 6) | OPC_SHLL_QB_DSP,
    OPC_SHRA_R_W   = (0x15 << 6) | OPC_SHLL_QB_DSP,
    OPC_SHRAV_R_W  = (0x17 << 6) | OPC_SHLL_QB_DSP,
};

#define MASK_DPA_W_PH(op)           (MASK_SPECIAL3(op) | (op & (0x1F << 6)))
enum {
    /* MIPS DSP Multiply Sub-class insns */
    OPC_DPAU_H_QBL    = (0x03 << 6) | OPC_DPA_W_PH_DSP,
    OPC_DPAU_H_QBR    = (0x07 << 6) | OPC_DPA_W_PH_DSP,
    OPC_DPSU_H_QBL    = (0x0B << 6) | OPC_DPA_W_PH_DSP,
    OPC_DPSU_H_QBR    = (0x0F << 6) | OPC_DPA_W_PH_DSP,
    OPC_DPA_W_PH      = (0x00 << 6) | OPC_DPA_W_PH_DSP,
    OPC_DPAX_W_PH     = (0x08 << 6) | OPC_DPA_W_PH_DSP,
    OPC_DPAQ_S_W_PH   = (0x04 << 6) | OPC_DPA_W_PH_DSP,
    OPC_DPAQX_S_W_PH  = (0x18 << 6) | OPC_DPA_W_PH_DSP,
    OPC_DPAQX_SA_W_PH = (0x1A << 6) | OPC_DPA_W_PH_DSP,
    OPC_DPS_W_PH      = (0x01 << 6) | OPC_DPA_W_PH_DSP,
    OPC_DPSX_W_PH     = (0x09 << 6) | OPC_DPA_W_PH_DSP,
    OPC_DPSQ_S_W_PH   = (0x05 << 6) | OPC_DPA_W_PH_DSP,
    OPC_DPSQX_S_W_PH  = (0x19 << 6) | OPC_DPA_W_PH_DSP,
    OPC_DPSQX_SA_W_PH = (0x1B << 6) | OPC_DPA_W_PH_DSP,
    OPC_MULSAQ_S_W_PH = (0x06 << 6) | OPC_DPA_W_PH_DSP,
    OPC_DPAQ_SA_L_W   = (0x0C << 6) | OPC_DPA_W_PH_DSP,
    OPC_DPSQ_SA_L_W   = (0x0D << 6) | OPC_DPA_W_PH_DSP,
    OPC_MAQ_S_W_PHL   = (0x14 << 6) | OPC_DPA_W_PH_DSP,
    OPC_MAQ_S_W_PHR   = (0x16 << 6) | OPC_DPA_W_PH_DSP,
    OPC_MAQ_SA_W_PHL  = (0x10 << 6) | OPC_DPA_W_PH_DSP,
    OPC_MAQ_SA_W_PHR  = (0x12 << 6) | OPC_DPA_W_PH_DSP,
    OPC_MULSA_W_PH    = (0x02 << 6) | OPC_DPA_W_PH_DSP,
};

#define MASK_INSV(op)               (MASK_SPECIAL3(op) | (op & (0x1F << 6)))
enum {
    /* DSP Bit/Manipulation Sub-class */
    OPC_INSV = (0x00 << 6) | OPC_INSV_DSP,
};

#define MASK_APPEND(op)             (MASK_SPECIAL3(op) | (op & (0x1F << 6)))
enum {
    /* MIPS DSP Append Sub-class */
    OPC_APPEND  = (0x00 << 6) | OPC_APPEND_DSP,
    OPC_PREPEND = (0x01 << 6) | OPC_APPEND_DSP,
    OPC_BALIGN  = (0x10 << 6) | OPC_APPEND_DSP,
};

#define MASK_EXTR_W(op)             (MASK_SPECIAL3(op) | (op & (0x1F << 6)))
enum {
    /* MIPS DSP Accumulator and DSPControl Access Sub-class */
    OPC_EXTR_W     = (0x00 << 6) | OPC_EXTR_W_DSP,
    OPC_EXTR_R_W   = (0x04 << 6) | OPC_EXTR_W_DSP,
    OPC_EXTR_RS_W  = (0x06 << 6) | OPC_EXTR_W_DSP,
    OPC_EXTR_S_H   = (0x0E << 6) | OPC_EXTR_W_DSP,
    OPC_EXTRV_S_H  = (0x0F << 6) | OPC_EXTR_W_DSP,
    OPC_EXTRV_W    = (0x01 << 6) | OPC_EXTR_W_DSP,
    OPC_EXTRV_R_W  = (0x05 << 6) | OPC_EXTR_W_DSP,
    OPC_EXTRV_RS_W = (0x07 << 6) | OPC_EXTR_W_DSP,
    OPC_EXTP       = (0x02 << 6) | OPC_EXTR_W_DSP,
    OPC_EXTPV      = (0x03 << 6) | OPC_EXTR_W_DSP,
    OPC_EXTPDP     = (0x0A << 6) | OPC_EXTR_W_DSP,
    OPC_EXTPDPV    = (0x0B << 6) | OPC_EXTR_W_DSP,
    OPC_SHILO      = (0x1A << 6) | OPC_EXTR_W_DSP,
    OPC_SHILOV     = (0x1B << 6) | OPC_EXTR_W_DSP,
    OPC_MTHLIP     = (0x1F << 6) | OPC_EXTR_W_DSP,
    OPC_WRDSP      = (0x13 << 6) | OPC_EXTR_W_DSP,
    OPC_RDDSP      = (0x12 << 6) | OPC_EXTR_W_DSP,
};

#define MASK_ABSQ_S_QH(op)          (MASK_SPECIAL3(op) | (op & (0x1F << 6)))
enum {
    /* MIPS DSP Arithmetic Sub-class */
    OPC_PRECEQ_L_PWL    = (0x14 << 6) | OPC_ABSQ_S_QH_DSP,
    OPC_PRECEQ_L_PWR    = (0x15 << 6) | OPC_ABSQ_S_QH_DSP,
    OPC_PRECEQ_PW_QHL   = (0x0C << 6) | OPC_ABSQ_S_QH_DSP,
    OPC_PRECEQ_PW_QHR   = (0x0D << 6) | OPC_ABSQ_S_QH_DSP,
    OPC_PRECEQ_PW_QHLA  = (0x0E << 6) | OPC_ABSQ_S_QH_DSP,
    OPC_PRECEQ_PW_QHRA  = (0x0F << 6) | OPC_ABSQ_S_QH_DSP,
    OPC_PRECEQU_QH_OBL  = (0x04 << 6) | OPC_ABSQ_S_QH_DSP,
    OPC_PRECEQU_QH_OBR  = (0x05 << 6) | OPC_ABSQ_S_QH_DSP,
    OPC_PRECEQU_QH_OBLA = (0x06 << 6) | OPC_ABSQ_S_QH_DSP,
    OPC_PRECEQU_QH_OBRA = (0x07 << 6) | OPC_ABSQ_S_QH_DSP,
    OPC_PRECEU_QH_OBL   = (0x1C << 6) | OPC_ABSQ_S_QH_DSP,
    OPC_PRECEU_QH_OBR   = (0x1D << 6) | OPC_ABSQ_S_QH_DSP,
    OPC_PRECEU_QH_OBLA  = (0x1E << 6) | OPC_ABSQ_S_QH_DSP,
    OPC_PRECEU_QH_OBRA  = (0x1F << 6) | OPC_ABSQ_S_QH_DSP,
    OPC_ABSQ_S_OB       = (0x01 << 6) | OPC_ABSQ_S_QH_DSP,
    OPC_ABSQ_S_PW       = (0x11 << 6) | OPC_ABSQ_S_QH_DSP,
    OPC_ABSQ_S_QH       = (0x09 << 6) | OPC_ABSQ_S_QH_DSP,
    /* DSP Bit/Manipulation Sub-class */
    OPC_REPL_OB         = (0x02 << 6) | OPC_ABSQ_S_QH_DSP,
    OPC_REPL_PW         = (0x12 << 6) | OPC_ABSQ_S_QH_DSP,
    OPC_REPL_QH         = (0x0A << 6) | OPC_ABSQ_S_QH_DSP,
    OPC_REPLV_OB        = (0x03 << 6) | OPC_ABSQ_S_QH_DSP,
    OPC_REPLV_PW        = (0x13 << 6) | OPC_ABSQ_S_QH_DSP,
    OPC_REPLV_QH        = (0x0B << 6) | OPC_ABSQ_S_QH_DSP,
};

#define MASK_ADDU_OB(op)            (MASK_SPECIAL3(op) | (op & (0x1F << 6)))
enum {
    /* MIPS DSP Multiply Sub-class insns */
    OPC_MULEQ_S_PW_QHL = (0x1C << 6) | OPC_ADDU_OB_DSP,
    OPC_MULEQ_S_PW_QHR = (0x1D << 6) | OPC_ADDU_OB_DSP,
    OPC_MULEU_S_QH_OBL = (0x06 << 6) | OPC_ADDU_OB_DSP,
    OPC_MULEU_S_QH_OBR = (0x07 << 6) | OPC_ADDU_OB_DSP,
    OPC_MULQ_RS_QH     = (0x1F << 6) | OPC_ADDU_OB_DSP,
    /* MIPS DSP Arithmetic Sub-class */
    OPC_RADDU_L_OB     = (0x14 << 6) | OPC_ADDU_OB_DSP,
    OPC_SUBQ_PW        = (0x13 << 6) | OPC_ADDU_OB_DSP,
    OPC_SUBQ_S_PW      = (0x17 << 6) | OPC_ADDU_OB_DSP,
    OPC_SUBQ_QH        = (0x0B << 6) | OPC_ADDU_OB_DSP,
    OPC_SUBQ_S_QH      = (0x0F << 6) | OPC_ADDU_OB_DSP,
    OPC_SUBU_OB        = (0x01 << 6) | OPC_ADDU_OB_DSP,
    OPC_SUBU_S_OB      = (0x05 << 6) | OPC_ADDU_OB_DSP,
    OPC_SUBU_QH        = (0x09 << 6) | OPC_ADDU_OB_DSP,
    OPC_SUBU_S_QH      = (0x0D << 6) | OPC_ADDU_OB_DSP,
    OPC_SUBUH_OB       = (0x19 << 6) | OPC_ADDU_OB_DSP,
    OPC_SUBUH_R_OB     = (0x1B << 6) | OPC_ADDU_OB_DSP,
    OPC_ADDQ_PW        = (0x12 << 6) | OPC_ADDU_OB_DSP,
    OPC_ADDQ_S_PW      = (0x16 << 6) | OPC_ADDU_OB_DSP,
    OPC_ADDQ_QH        = (0x0A << 6) | OPC_ADDU_OB_DSP,
    OPC_ADDQ_S_QH      = (0x0E << 6) | OPC_ADDU_OB_DSP,
    OPC_ADDU_OB        = (0x00 << 6) | OPC_ADDU_OB_DSP,
    OPC_ADDU_S_OB      = (0x04 << 6) | OPC_ADDU_OB_DSP,
    OPC_ADDU_QH        = (0x08 << 6) | OPC_ADDU_OB_DSP,
    OPC_ADDU_S_QH      = (0x0C << 6) | OPC_ADDU_OB_DSP,
    OPC_ADDUH_OB       = (0x18 << 6) | OPC_ADDU_OB_DSP,
    OPC_ADDUH_R_OB     = (0x1A << 6) | OPC_ADDU_OB_DSP,
};

#define MASK_CMPU_EQ_OB(op)         (MASK_SPECIAL3(op) | (op & (0x1F << 6)))
enum {
    /* DSP Compare-Pick Sub-class */
    OPC_CMP_EQ_PW         = (0x10 << 6) | OPC_CMPU_EQ_OB_DSP,
    OPC_CMP_LT_PW         = (0x11 << 6) | OPC_CMPU_EQ_OB_DSP,
    OPC_CMP_LE_PW         = (0x12 << 6) | OPC_CMPU_EQ_OB_DSP,
    OPC_CMP_EQ_QH         = (0x08 << 6) | OPC_CMPU_EQ_OB_DSP,
    OPC_CMP_LT_QH         = (0x09 << 6) | OPC_CMPU_EQ_OB_DSP,
    OPC_CMP_LE_QH         = (0x0A << 6) | OPC_CMPU_EQ_OB_DSP,
    OPC_CMPGDU_EQ_OB      = (0x18 << 6) | OPC_CMPU_EQ_OB_DSP,
    OPC_CMPGDU_LT_OB      = (0x19 << 6) | OPC_CMPU_EQ_OB_DSP,
    OPC_CMPGDU_LE_OB      = (0x1A << 6) | OPC_CMPU_EQ_OB_DSP,
    OPC_CMPGU_EQ_OB       = (0x04 << 6) | OPC_CMPU_EQ_OB_DSP,
    OPC_CMPGU_LT_OB       = (0x05 << 6) | OPC_CMPU_EQ_OB_DSP,
    OPC_CMPGU_LE_OB       = (0x06 << 6) | OPC_CMPU_EQ_OB_DSP,
    OPC_CMPU_EQ_OB        = (0x00 << 6) | OPC_CMPU_EQ_OB_DSP,
    OPC_CMPU_LT_OB        = (0x01 << 6) | OPC_CMPU_EQ_OB_DSP,
    OPC_CMPU_LE_OB        = (0x02 << 6) | OPC_CMPU_EQ_OB_DSP,
    OPC_PACKRL_PW         = (0x0E << 6) | OPC_CMPU_EQ_OB_DSP,
    OPC_PICK_OB           = (0x03 << 6) | OPC_CMPU_EQ_OB_DSP,
    OPC_PICK_PW           = (0x13 << 6) | OPC_CMPU_EQ_OB_DSP,
    OPC_PICK_QH           = (0x0B << 6) | OPC_CMPU_EQ_OB_DSP,
    /* MIPS DSP Arithmetic Sub-class */
    OPC_PRECR_OB_QH       = (0x0D << 6) | OPC_CMPU_EQ_OB_DSP,
    OPC_PRECR_SRA_QH_PW   = (0x1E << 6) | OPC_CMPU_EQ_OB_DSP,
    OPC_PRECR_SRA_R_QH_PW = (0x1F << 6) | OPC_CMPU_EQ_OB_DSP,
    OPC_PRECRQ_OB_QH      = (0x0C << 6) | OPC_CMPU_EQ_OB_DSP,
    OPC_PRECRQ_PW_L       = (0x1C << 6) | OPC_CMPU_EQ_OB_DSP,
    OPC_PRECRQ_QH_PW      = (0x14 << 6) | OPC_CMPU_EQ_OB_DSP,
    OPC_PRECRQ_RS_QH_PW   = (0x15 << 6) | OPC_CMPU_EQ_OB_DSP,
    OPC_PRECRQU_S_OB_QH   = (0x0F << 6) | OPC_CMPU_EQ_OB_DSP,
};

#define MASK_DAPPEND(op)            (MASK_SPECIAL3(op) | (op & (0x1F << 6)))
enum {
    /* DSP Append Sub-class */
    OPC_DAPPEND  = (0x00 << 6) | OPC_DAPPEND_DSP,
    OPC_PREPENDD = (0x03 << 6) | OPC_DAPPEND_DSP,
    OPC_PREPENDW = (0x01 << 6) | OPC_DAPPEND_DSP,
    OPC_DBALIGN  = (0x10 << 6) | OPC_DAPPEND_DSP,
};

#define MASK_DEXTR_W(op)            (MASK_SPECIAL3(op) | (op & (0x1F << 6)))
enum {
    /* MIPS DSP Accumulator and DSPControl Access Sub-class */
    OPC_DMTHLIP     = (0x1F << 6) | OPC_DEXTR_W_DSP,
    OPC_DSHILO      = (0x1A << 6) | OPC_DEXTR_W_DSP,
    OPC_DEXTP       = (0x02 << 6) | OPC_DEXTR_W_DSP,
    OPC_DEXTPDP     = (0x0A << 6) | OPC_DEXTR_W_DSP,
    OPC_DEXTPDPV    = (0x0B << 6) | OPC_DEXTR_W_DSP,
    OPC_DEXTPV      = (0x03 << 6) | OPC_DEXTR_W_DSP,
    OPC_DEXTR_L     = (0x10 << 6) | OPC_DEXTR_W_DSP,
    OPC_DEXTR_R_L   = (0x14 << 6) | OPC_DEXTR_W_DSP,
    OPC_DEXTR_RS_L  = (0x16 << 6) | OPC_DEXTR_W_DSP,
    OPC_DEXTR_W     = (0x00 << 6) | OPC_DEXTR_W_DSP,
    OPC_DEXTR_R_W   = (0x04 << 6) | OPC_DEXTR_W_DSP,
    OPC_DEXTR_RS_W  = (0x06 << 6) | OPC_DEXTR_W_DSP,
    OPC_DEXTR_S_H   = (0x0E << 6) | OPC_DEXTR_W_DSP,
    OPC_DEXTRV_L    = (0x11 << 6) | OPC_DEXTR_W_DSP,
    OPC_DEXTRV_R_L  = (0x15 << 6) | OPC_DEXTR_W_DSP,
    OPC_DEXTRV_RS_L = (0x17 << 6) | OPC_DEXTR_W_DSP,
    OPC_DEXTRV_S_H  = (0x0F << 6) | OPC_DEXTR_W_DSP,
    OPC_DEXTRV_W    = (0x01 << 6) | OPC_DEXTR_W_DSP,
    OPC_DEXTRV_R_W  = (0x05 << 6) | OPC_DEXTR_W_DSP,
    OPC_DEXTRV_RS_W = (0x07 << 6) | OPC_DEXTR_W_DSP,
    OPC_DSHILOV     = (0x1B << 6) | OPC_DEXTR_W_DSP,
};

#define MASK_DINSV(op)              (MASK_SPECIAL3(op) | (op & (0x1F << 6)))
enum {
    /* DSP Bit/Manipulation Sub-class */
    OPC_DINSV = (0x00 << 6) | OPC_DINSV_DSP,
};

#define MASK_DPAQ_W_QH(op)          (MASK_SPECIAL3(op) | (op & (0x1F << 6)))
enum {
    /* MIPS DSP Multiply Sub-class insns */
    OPC_DMADD         = (0x19 << 6) | OPC_DPAQ_W_QH_DSP,
    OPC_DMADDU        = (0x1D << 6) | OPC_DPAQ_W_QH_DSP,
    OPC_DMSUB         = (0x1B << 6) | OPC_DPAQ_W_QH_DSP,
    OPC_DMSUBU        = (0x1F << 6) | OPC_DPAQ_W_QH_DSP,
    OPC_DPA_W_QH      = (0x00 << 6) | OPC_DPAQ_W_QH_DSP,
    OPC_DPAQ_S_W_QH   = (0x04 << 6) | OPC_DPAQ_W_QH_DSP,
    OPC_DPAQ_SA_L_PW  = (0x0C << 6) | OPC_DPAQ_W_QH_DSP,
    OPC_DPAU_H_OBL    = (0x03 << 6) | OPC_DPAQ_W_QH_DSP,
    OPC_DPAU_H_OBR    = (0x07 << 6) | OPC_DPAQ_W_QH_DSP,
    OPC_DPS_W_QH      = (0x01 << 6) | OPC_DPAQ_W_QH_DSP,
    OPC_DPSQ_S_W_QH   = (0x05 << 6) | OPC_DPAQ_W_QH_DSP,
    OPC_DPSQ_SA_L_PW  = (0x0D << 6) | OPC_DPAQ_W_QH_DSP,
    OPC_DPSU_H_OBL    = (0x0B << 6) | OPC_DPAQ_W_QH_DSP,
    OPC_DPSU_H_OBR    = (0x0F << 6) | OPC_DPAQ_W_QH_DSP,
    OPC_MAQ_S_L_PWL   = (0x1C << 6) | OPC_DPAQ_W_QH_DSP,
    OPC_MAQ_S_L_PWR   = (0x1E << 6) | OPC_DPAQ_W_QH_DSP,
    OPC_MAQ_S_W_QHLL  = (0x14 << 6) | OPC_DPAQ_W_QH_DSP,
    OPC_MAQ_SA_W_QHLL = (0x10 << 6) | OPC_DPAQ_W_QH_DSP,
    OPC_MAQ_S_W_QHLR  = (0x15 << 6) | OPC_DPAQ_W_QH_DSP,
    OPC_MAQ_SA_W_QHLR = (0x11 << 6) | OPC_DPAQ_W_QH_DSP,
    OPC_MAQ_S_W_QHRL  = (0x16 << 6) | OPC_DPAQ_W_QH_DSP,
    OPC_MAQ_SA_W_QHRL = (0x12 << 6) | OPC_DPAQ_W_QH_DSP,
    OPC_MAQ_S_W_QHRR  = (0x17 << 6) | OPC_DPAQ_W_QH_DSP,
    OPC_MAQ_SA_W_QHRR = (0x13 << 6) | OPC_DPAQ_W_QH_DSP,
    OPC_MULSAQ_S_L_PW = (0x0E << 6) | OPC_DPAQ_W_QH_DSP,
    OPC_MULSAQ_S_W_QH = (0x06 << 6) | OPC_DPAQ_W_QH_DSP,
};

#define MASK_SHLL_OB(op)            (MASK_SPECIAL3(op) | (op & (0x1F << 6)))
enum {
    /* MIPS DSP GPR-Based Shift Sub-class */
    OPC_SHLL_PW    = (0x10 << 6) | OPC_SHLL_OB_DSP,
    OPC_SHLL_S_PW  = (0x14 << 6) | OPC_SHLL_OB_DSP,
    OPC_SHLLV_OB   = (0x02 << 6) | OPC_SHLL_OB_DSP,
    OPC_SHLLV_PW   = (0x12 << 6) | OPC_SHLL_OB_DSP,
    OPC_SHLLV_S_PW = (0x16 << 6) | OPC_SHLL_OB_DSP,
    OPC_SHLLV_QH   = (0x0A << 6) | OPC_SHLL_OB_DSP,
    OPC_SHLLV_S_QH = (0x0E << 6) | OPC_SHLL_OB_DSP,
    OPC_SHRA_PW    = (0x11 << 6) | OPC_SHLL_OB_DSP,
    OPC_SHRA_R_PW  = (0x15 << 6) | OPC_SHLL_OB_DSP,
    OPC_SHRAV_OB   = (0x06 << 6) | OPC_SHLL_OB_DSP,
    OPC_SHRAV_R_OB = (0x07 << 6) | OPC_SHLL_OB_DSP,
    OPC_SHRAV_PW   = (0x13 << 6) | OPC_SHLL_OB_DSP,
    OPC_SHRAV_R_PW = (0x17 << 6) | OPC_SHLL_OB_DSP,
    OPC_SHRAV_QH   = (0x0B << 6) | OPC_SHLL_OB_DSP,
    OPC_SHRAV_R_QH = (0x0F << 6) | OPC_SHLL_OB_DSP,
    OPC_SHRLV_OB   = (0x03 << 6) | OPC_SHLL_OB_DSP,
    OPC_SHRLV_QH   = (0x1B << 6) | OPC_SHLL_OB_DSP,
    OPC_SHLL_OB    = (0x00 << 6) | OPC_SHLL_OB_DSP,
    OPC_SHLL_QH    = (0x08 << 6) | OPC_SHLL_OB_DSP,
    OPC_SHLL_S_QH  = (0x0C << 6) | OPC_SHLL_OB_DSP,
    OPC_SHRA_OB    = (0x04 << 6) | OPC_SHLL_OB_DSP,
    OPC_SHRA_R_OB  = (0x05 << 6) | OPC_SHLL_OB_DSP,
    OPC_SHRA_QH    = (0x09 << 6) | OPC_SHLL_OB_DSP,
    OPC_SHRA_R_QH  = (0x0D << 6) | OPC_SHLL_OB_DSP,
    OPC_SHRL_OB    = (0x01 << 6) | OPC_SHLL_OB_DSP,
    OPC_SHRL_QH    = (0x19 << 6) | OPC_SHLL_OB_DSP,
};

/* Coprocessor 0 (rs field) */
#define MASK_CP0(op)                (MASK_OP_MAJOR(op) | (op & (0x1F << 21)))

enum {
    OPC_MFC0     = (0x00 << 21) | OPC_CP0,
    OPC_DMFC0    = (0x01 << 21) | OPC_CP0,
    OPC_MFHC0    = (0x02 << 21) | OPC_CP0,
    OPC_MTC0     = (0x04 << 21) | OPC_CP0,
    OPC_DMTC0    = (0x05 << 21) | OPC_CP0,
    OPC_MTHC0    = (0x06 << 21) | OPC_CP0,
    OPC_MFTR     = (0x08 << 21) | OPC_CP0,
    OPC_RDPGPR   = (0x0A << 21) | OPC_CP0,
    OPC_MFMC0    = (0x0B << 21) | OPC_CP0,
    OPC_MTTR     = (0x0C << 21) | OPC_CP0,
    OPC_WRPGPR   = (0x0E << 21) | OPC_CP0,
    OPC_C0       = (0x10 << 21) | OPC_CP0,
    OPC_C0_1     = (0x11 << 21) | OPC_CP0,
    OPC_C0_2     = (0x12 << 21) | OPC_CP0,
    OPC_C0_3     = (0x13 << 21) | OPC_CP0,
    OPC_C0_4     = (0x14 << 21) | OPC_CP0,
    OPC_C0_5     = (0x15 << 21) | OPC_CP0,
    OPC_C0_6     = (0x16 << 21) | OPC_CP0,
    OPC_C0_7     = (0x17 << 21) | OPC_CP0,
    OPC_C0_8     = (0x18 << 21) | OPC_CP0,
    OPC_C0_9     = (0x19 << 21) | OPC_CP0,
    OPC_C0_A     = (0x1A << 21) | OPC_CP0,
    OPC_C0_B     = (0x1B << 21) | OPC_CP0,
    OPC_C0_C     = (0x1C << 21) | OPC_CP0,
    OPC_C0_D     = (0x1D << 21) | OPC_CP0,
    OPC_C0_E     = (0x1E << 21) | OPC_CP0,
    OPC_C0_F     = (0x1F << 21) | OPC_CP0,
};

/* MFMC0 opcodes */
#define MASK_MFMC0(op)              (MASK_CP0(op) | (op & 0xFFFF))

enum {
    OPC_DMT      = 0x01 | (0 << 5) | (0x0F << 6) | (0x01 << 11) | OPC_MFMC0,
    OPC_EMT      = 0x01 | (1 << 5) | (0x0F << 6) | (0x01 << 11) | OPC_MFMC0,
    OPC_DVPE     = 0x01 | (0 << 5) | OPC_MFMC0,
    OPC_EVPE     = 0x01 | (1 << 5) | OPC_MFMC0,
    OPC_DI       = (0 << 5) | (0x0C << 11) | OPC_MFMC0,
    OPC_EI       = (1 << 5) | (0x0C << 11) | OPC_MFMC0,
    OPC_DVP      = 0x04 | (0 << 3) | (1 << 5) | (0 << 11) | OPC_MFMC0,
    OPC_EVP      = 0x04 | (0 << 3) | (0 << 5) | (0 << 11) | OPC_MFMC0,
};

/* Coprocessor 0 (with rs == C0) */
#define MASK_C0(op)                 (MASK_CP0(op) | (op & 0x3F))

enum {
    OPC_TLBR     = 0x01 | OPC_C0,
    OPC_TLBWI    = 0x02 | OPC_C0,
    OPC_TLBINV   = 0x03 | OPC_C0,
    OPC_TLBINVF  = 0x04 | OPC_C0,
    OPC_TLBWR    = 0x06 | OPC_C0,
    OPC_TLBP     = 0x08 | OPC_C0,
    OPC_RFE      = 0x10 | OPC_C0,
    OPC_ERET     = 0x18 | OPC_C0,
    OPC_DERET    = 0x1F | OPC_C0,
    OPC_WAIT     = 0x20 | OPC_C0,
};

#define MASK_CP2(op)                (MASK_OP_MAJOR(op) | (op & (0x1F << 21)))

enum {
    OPC_MFC2    = (0x00 << 21) | OPC_CP2,
    OPC_DMFC2   = (0x01 << 21) | OPC_CP2,
    OPC_CFC2    = (0x02 << 21) | OPC_CP2,
    OPC_MFHC2   = (0x03 << 21) | OPC_CP2,
    OPC_MTC2    = (0x04 << 21) | OPC_CP2,
    OPC_DMTC2   = (0x05 << 21) | OPC_CP2,
    OPC_CTC2    = (0x06 << 21) | OPC_CP2,
    OPC_MTHC2   = (0x07 << 21) | OPC_CP2,
    OPC_BC2     = (0x08 << 21) | OPC_CP2,
    OPC_BC2EQZ  = (0x09 << 21) | OPC_CP2,
    OPC_BC2NEZ  = (0x0D << 21) | OPC_CP2,
};

#if defined(TARGET_CHERI)

#define MASK_CAP3(op)       (MASK_CP2(op) | ((op) & 0x7))
#define MASK_CAP4(op)       (MASK_CP2(op) | ((op) & 0xf))
#define MASK_CAP6(op)       (MASK_CP2(op) | ((op) & 0x3f))
#define MASK_CAP7(op)       (MASK_CP2(op) | ((op) & (0x1f << 6)) |  (0x3f))
#define MASK_CAP8(op)       (MASK_CP2(op) | ((op) & (0x1f << 11)) | \
        (0x1f << 6) |  (0x3f))

enum {
    OPC_CGET        = OPC_CP2 | (0x00 << 21),
    OPC_CSETBOUNDS  = OPC_CP2 | (0x01 << 21),
    OPC_CSEAL       = OPC_CP2 | (0x02 << 21),
    OPC_CUNSEAL     = OPC_CP2 | (0x03 << 21),
    OPC_CMISC       = OPC_CP2 | (0x04 << 21),
    OPC_CCALL       = OPC_CP2 | (0x05 << 21),
    OPC_CRETURN     = OPC_CP2 | (0x06 << 21),
    OPC_CJALR       = OPC_CP2 | (0x07 << 21),
    OPC_CJR         = OPC_CP2 | (0x08 << 21),
    OPC_CBTU        = OPC_CP2 | (0x09 << 21),
    OPC_CBTS        = OPC_CP2 | (0x0a << 21),
    OPC_CCHECK      = OPC_CP2 | (0x0b << 21),
    OPC_CTOPTR      = OPC_CP2 | (0x0c << 21),
    OPC_COFFSET     = OPC_CP2 | (0x0d << 21),
    OPC_CPTRCMP     = OPC_CP2 | (0x0e << 21),
    OPC_CCLEARREGS  = OPC_CP2 | (0x0f << 21),
    OPC_CLL         = OPC_CP2 | (0x10 << 21),
};

enum {
    OPC_CGETPERM        = OPC_CGET | (0x00),
    OPC_CGETTYPE        = OPC_CGET | (0x01),
    OPC_CGETBASE        = OPC_CGET | (0x02),
    OPC_CGETLEN         = OPC_CGET | (0x03),
    OPC_CGETCAUSE       = OPC_CGET | (0x04),
    OPC_CGETTAG         = OPC_CGET | (0x05),
    OPC_CGETSEALED      = OPC_CGET | (0x06),
    OPC_CGETPCC         = OPC_CGET | (0x07),

    OPC_CSETBOUNDSEXACT = OPC_CGET | (0x09),
    OPC_CSUB            = OPC_CGET | (0x0a),
};

enum {
    OPC_CANDPERM    = OPC_CMISC | (0x0),
    /* 0x2 was CIncBase (prior to CHERI-128) */
    /* 0x3 was CSetLen (prior to CHERI-128) */
    OPC_CSETCAUSE   = OPC_CMISC | (0x4),
    OPC_CCLEARTAG   = OPC_CMISC | (0x5),
    OPC_MTC2SEL6    = OPC_CMISC | (0x6),
    OPC_CFROMPTR    = OPC_CMISC | (0x7),
};

enum {
    OPC_CCHECKPERM  = OPC_CCHECK | (0x0),
    OPC_CCHECKTYPE  = OPC_CCHECK | (0x1),
};

enum {
    OPC_CINCOFFSET  = OPC_COFFSET | (0x0),
    OPC_CSETOFFSET  = OPC_COFFSET | (0x1),
    OPC_CGETOFFSET  = OPC_COFFSET | (0x2),
};

enum {
    OPC_CEQ         = OPC_CPTRCMP | (0x0),
    OPC_CNE         = OPC_CPTRCMP | (0x1),
    OPC_CLT         = OPC_CPTRCMP | (0x2),
    OPC_CLE         = OPC_CPTRCMP | (0x3),
    OPC_CLTU        = OPC_CPTRCMP | (0x4),
    OPC_CLEU        = OPC_CPTRCMP | (0x5),
    OPC_CEXEQ       = OPC_CPTRCMP | (0x6),
    OPC_CNEXEQ      = OPC_CPTRCMP | (0x7),
};

enum {
    OPC_CSCB        = OPC_CLL | (0x0),
    OPC_CSCH        = OPC_CLL | (0x1),
    OPC_CSCW        = OPC_CLL | (0x2),
    OPC_CSCD        = OPC_CLL | (0x3),

    OPC_CSCC        = OPC_CLL | (0x7),

    OPC_CLLBU       = OPC_CLL | (0x8),
    OPC_CLLHU       = OPC_CLL | (0x9),
    OPC_CLLWU       = OPC_CLL | (0xa),
    OPC_CLLD        = OPC_CLL | (0xb),
    OPC_CLLB        = OPC_CLL | (0xc),
    OPC_CLLH        = OPC_CLL | (0xd),
    OPC_CLLW        = OPC_CLL | (0xe),

    OPC_CLLC        = OPC_CLL | (0xf),
};

#define MASK_CLDST_OFFSET(opc)   ((opc >> 3) & 0xff)
#define MASK_CLDST_OPC(opc)     ((opc) & ((0x3f << 26) | 0x7))

/* Load Via Capability Register */
enum {
    OPC_CLBU        = OPC_CLOAD | (0x0),
    OPC_CLHU        = OPC_CLOAD | (0x1),
    OPC_CLWU        = OPC_CLOAD | (0x2),
    OPC_CLDU        = OPC_CLOAD | (0x3),
    OPC_CLB         = OPC_CLOAD | (0x4),
    OPC_CLH         = OPC_CLOAD | (0x5),
    OPC_CLW         = OPC_CLOAD | (0x6),
    OPC_CLD         = OPC_CLOAD | (0x7),
};

/* Store Via Capability Register */
enum {
    OPC_CSB         = OPC_CSTORE | (0x0),
    OPC_CSH         = OPC_CSTORE | (0x1),
    OPC_CSW         = OPC_CSTORE | (0x2),
    OPC_CSD         = OPC_CSTORE | (0x3),
};

/* Version 1.17 and 1.22 ISA encodings (*_NI) to replace above. */
enum {
    /* Common new ISA encoding blocks */
    /* non-immediate capability instructions */
    OPC_CAP_NI          = OPC_CP2 | (0x00 << 21),
    /* 2-operand capability instructions */
    OPC_C2OPERAND_NI    = OPC_CAP_NI | (0x3f),
    /* 1-operand capability instructions */
    OPC_C1OPERAND_NI    = OPC_C2OPERAND_NI | (0x1f << 6),
};

enum {
    /* One operand instructions */
    OPC_CGETPCC_NI      = OPC_C1OPERAND_NI | (0x00 << 11),
    OPC_CGETCAUSE_NI    = OPC_C1OPERAND_NI | (0x01 << 11),
    OPC_CSETCAUSE_NI    = OPC_C1OPERAND_NI | (0x02 << 11),
    OPC_CJR_NI          = OPC_C1OPERAND_NI | (0x03 << 11),
};

enum {
    /* Two Operand Instructions */
    OPC_CGETPERM_NI     = OPC_C2OPERAND_NI | (0x00 << 6),
    OPC_CGETTYPE_NI     = OPC_C2OPERAND_NI | (0x01 << 6),
    OPC_CGETBASE_NI     = OPC_C2OPERAND_NI | (0x02 << 6),
    OPC_CGETLEN_NI      = OPC_C2OPERAND_NI | (0x03 << 6),
    OPC_CGETTAG_NI      = OPC_C2OPERAND_NI | (0x04 << 6),
    OPC_CGETSEALED_NI   = OPC_C2OPERAND_NI | (0x05 << 6),
    OPC_CGETOFFSET_NI   = OPC_C2OPERAND_NI | (0x06 << 6),
    OPC_CGETPCCSETOFF_NI = OPC_C2OPERAND_NI | (0x07 << 6),
    OPC_CCHECKPERM_NI   = OPC_C2OPERAND_NI | (0x08 << 6),
    OPC_CCHECKTYPE_NI   = OPC_C2OPERAND_NI | (0x09 << 6),
    OPC_CMOVE_NI        = OPC_C2OPERAND_NI | (0x0a << 6),
    OPC_CCLEARTAG_NI    = OPC_C2OPERAND_NI | (0x0b << 6),
    OPC_CJALR_NI        = OPC_C2OPERAND_NI | (0x0c << 6),
    OPC_CREADHWR_NI     = OPC_C2OPERAND_NI | (0x0d << 6),
    OPC_CWRITEHWR_NI    = OPC_C2OPERAND_NI | (0x0e << 6),
    OPC_CGETADDR_NI     = OPC_C2OPERAND_NI | (0x0f << 6),
    OPC_CRAP_NI         = OPC_C2OPERAND_NI | (0x10 << 6),
    OPC_CRAM_NI         = OPC_C2OPERAND_NI | (0x11 << 6),
    OPC_CGETFLAGS_NI    = OPC_C2OPERAND_NI | (0x12 << 6),
    OPC_CGETPCCINCOFF_NI = OPC_C2OPERAND_NI | (0x13 << 6),
    OPC_CGETPCCSETADDR_NI = OPC_C2OPERAND_NI | (0x14 << 6),
    OPC_CSEALENTRY_NI   = OPC_C2OPERAND_NI | (0x1d << 6),
    OPC_CLOADTAGS_NI    = OPC_C2OPERAND_NI | (0x1e << 6),
};

enum {
    /* Three operand instructions 1.22 */
    OPC_CSEAL_NI        = OPC_CAP_NI | (0x0b),
    OPC_CUNSEAL_NI      = OPC_CAP_NI | (0x0c),
    OPC_CANDPERM_NI     = OPC_CAP_NI | (0x0d),
    OPC_CSETFLAGS_NI    = OPC_CAP_NI | (0x0e),
    OPC_CSETOFFSET_NI   = OPC_CAP_NI | (0x0f),
    OPC_CSETBOUNDS_NI   = OPC_CAP_NI | (0x08),
    /* OPC_SETBOUNDSEXACT_NI = OPC_CAP_NI | (0x09), unchanged OPC_SETBOUNDSEXACT */
    OPC_CINCOFFSET_NI   = OPC_CAP_NI | (0x11),
    OPC_CTOPTR_NI       = OPC_CAP_NI | (0x12),
    OPC_CFROMPTR_NI     = OPC_CAP_NI | (0x13),
    /* OPC_CSUB_NI      = OPC_CAP_NI | (0x0a), unchanged OPC_CSUB */
    OPC_CEQ_NI          = OPC_CAP_NI | (0x14),
    OPC_CNE_NI          = OPC_CAP_NI | (0x15),
    OPC_CLT_NI          = OPC_CAP_NI | (0x16),
    OPC_CLE_NI          = OPC_CAP_NI | (0x17),
    OPC_CLTU_NI         = OPC_CAP_NI | (0x18),
    OPC_CLEU_NI         = OPC_CAP_NI | (0x19),
    OPC_CEXEQ_NI        = OPC_CAP_NI | (0x1a),
    OPC_CMOVZ_NI        = OPC_CAP_NI | (0x1b),
    OPC_CMOVN_NI        = OPC_CAP_NI | (0x1c),
    OPC_CBUILDCAP_NI    = OPC_CAP_NI | (0x1d),
    OPC_CCOPYTYPE_NI    = OPC_CAP_NI | (0x1e),
    OPC_CCSEAL_NI       = OPC_CAP_NI | (0x1f),
    OPC_CTESTSUBSET_NI  = OPC_CAP_NI | (0x20),
    OPC_CNEXEQ_NI       = OPC_CAP_NI | (0x21),
    OPC_CSETADDR_NI     = OPC_CAP_NI | (0x22),
    OPC_CGETANDADDR_NI  = OPC_CAP_NI | (0x23),
    OPC_CANDADDR_NI     = OPC_CAP_NI | (0x24),
};

enum {
    /* instructions with immediate values 1.22 */
    OPC_CSETBOUNDSIMM_NI   = OPC_CP2 | (0x14 << 21),
    OPC_CINCOFFSETIMM_NI = OPC_CP2 | (0x13 << 21),
    /* OPC_CBTU_NI           = OPC_CP2 | (0x09 << 21), unchanged OPC_CBTU */
    /* OPC_CBTS_NI           = OPC_CP2 | (0x0a << 21), unchanged OPC_CBTS */
    OPC_CBEZ_NI          = OPC_CP2 | (0x11 << 21),
    OPC_CBNZ_NI          = OPC_CP2 | (0x12 << 21),
    /* OPC_CCALL_NI          = OPC_CP2 | (0x05 << 21), unchanged OPC_CCALL */
    /* OPC_CCLEARREGS variants unchanged */
    OPC_CRETURN_NI       = OPC_CP2 | (0x05 << 21 | 0x7ff)
};

#endif /* TARGET_CHERI */



#define MASK_LMMI(op)    (MASK_OP_MAJOR(op) | (op & (0x1F << 21)) | (op & 0x1F))

enum {
    OPC_PADDSH      = (24 << 21) | (0x00) | OPC_CP2,
    OPC_PADDUSH     = (25 << 21) | (0x00) | OPC_CP2,
    OPC_PADDH       = (26 << 21) | (0x00) | OPC_CP2,
    OPC_PADDW       = (27 << 21) | (0x00) | OPC_CP2,
    OPC_PADDSB      = (28 << 21) | (0x00) | OPC_CP2,
    OPC_PADDUSB     = (29 << 21) | (0x00) | OPC_CP2,
    OPC_PADDB       = (30 << 21) | (0x00) | OPC_CP2,
    OPC_PADDD       = (31 << 21) | (0x00) | OPC_CP2,

    OPC_PSUBSH      = (24 << 21) | (0x01) | OPC_CP2,
    OPC_PSUBUSH     = (25 << 21) | (0x01) | OPC_CP2,
    OPC_PSUBH       = (26 << 21) | (0x01) | OPC_CP2,
    OPC_PSUBW       = (27 << 21) | (0x01) | OPC_CP2,
    OPC_PSUBSB      = (28 << 21) | (0x01) | OPC_CP2,
    OPC_PSUBUSB     = (29 << 21) | (0x01) | OPC_CP2,
    OPC_PSUBB       = (30 << 21) | (0x01) | OPC_CP2,
    OPC_PSUBD       = (31 << 21) | (0x01) | OPC_CP2,

    OPC_PSHUFH      = (24 << 21) | (0x02) | OPC_CP2,
    OPC_PACKSSWH    = (25 << 21) | (0x02) | OPC_CP2,
    OPC_PACKSSHB    = (26 << 21) | (0x02) | OPC_CP2,
    OPC_PACKUSHB    = (27 << 21) | (0x02) | OPC_CP2,
    OPC_XOR_CP2     = (28 << 21) | (0x02) | OPC_CP2,
    OPC_NOR_CP2     = (29 << 21) | (0x02) | OPC_CP2,
    OPC_AND_CP2     = (30 << 21) | (0x02) | OPC_CP2,
    OPC_PANDN       = (31 << 21) | (0x02) | OPC_CP2,

    OPC_PUNPCKLHW   = (24 << 21) | (0x03) | OPC_CP2,
    OPC_PUNPCKHHW   = (25 << 21) | (0x03) | OPC_CP2,
    OPC_PUNPCKLBH   = (26 << 21) | (0x03) | OPC_CP2,
    OPC_PUNPCKHBH   = (27 << 21) | (0x03) | OPC_CP2,
    OPC_PINSRH_0    = (28 << 21) | (0x03) | OPC_CP2,
    OPC_PINSRH_1    = (29 << 21) | (0x03) | OPC_CP2,
    OPC_PINSRH_2    = (30 << 21) | (0x03) | OPC_CP2,
    OPC_PINSRH_3    = (31 << 21) | (0x03) | OPC_CP2,

    OPC_PAVGH       = (24 << 21) | (0x08) | OPC_CP2,
    OPC_PAVGB       = (25 << 21) | (0x08) | OPC_CP2,
    OPC_PMAXSH      = (26 << 21) | (0x08) | OPC_CP2,
    OPC_PMINSH      = (27 << 21) | (0x08) | OPC_CP2,
    OPC_PMAXUB      = (28 << 21) | (0x08) | OPC_CP2,
    OPC_PMINUB      = (29 << 21) | (0x08) | OPC_CP2,

    OPC_PCMPEQW     = (24 << 21) | (0x09) | OPC_CP2,
    OPC_PCMPGTW     = (25 << 21) | (0x09) | OPC_CP2,
    OPC_PCMPEQH     = (26 << 21) | (0x09) | OPC_CP2,
    OPC_PCMPGTH     = (27 << 21) | (0x09) | OPC_CP2,
    OPC_PCMPEQB     = (28 << 21) | (0x09) | OPC_CP2,
    OPC_PCMPGTB     = (29 << 21) | (0x09) | OPC_CP2,

    OPC_PSLLW       = (24 << 21) | (0x0A) | OPC_CP2,
    OPC_PSLLH       = (25 << 21) | (0x0A) | OPC_CP2,
    OPC_PMULLH      = (26 << 21) | (0x0A) | OPC_CP2,
    OPC_PMULHH      = (27 << 21) | (0x0A) | OPC_CP2,
    OPC_PMULUW      = (28 << 21) | (0x0A) | OPC_CP2,
    OPC_PMULHUH     = (29 << 21) | (0x0A) | OPC_CP2,

    OPC_PSRLW       = (24 << 21) | (0x0B) | OPC_CP2,
    OPC_PSRLH       = (25 << 21) | (0x0B) | OPC_CP2,
    OPC_PSRAW       = (26 << 21) | (0x0B) | OPC_CP2,
    OPC_PSRAH       = (27 << 21) | (0x0B) | OPC_CP2,
    OPC_PUNPCKLWD   = (28 << 21) | (0x0B) | OPC_CP2,
    OPC_PUNPCKHWD   = (29 << 21) | (0x0B) | OPC_CP2,

    OPC_ADDU_CP2    = (24 << 21) | (0x0C) | OPC_CP2,
    OPC_OR_CP2      = (25 << 21) | (0x0C) | OPC_CP2,
    OPC_ADD_CP2     = (26 << 21) | (0x0C) | OPC_CP2,
    OPC_DADD_CP2    = (27 << 21) | (0x0C) | OPC_CP2,
    OPC_SEQU_CP2    = (28 << 21) | (0x0C) | OPC_CP2,
    OPC_SEQ_CP2     = (29 << 21) | (0x0C) | OPC_CP2,

    OPC_SUBU_CP2    = (24 << 21) | (0x0D) | OPC_CP2,
    OPC_PASUBUB     = (25 << 21) | (0x0D) | OPC_CP2,
    OPC_SUB_CP2     = (26 << 21) | (0x0D) | OPC_CP2,
    OPC_DSUB_CP2    = (27 << 21) | (0x0D) | OPC_CP2,
    OPC_SLTU_CP2    = (28 << 21) | (0x0D) | OPC_CP2,
    OPC_SLT_CP2     = (29 << 21) | (0x0D) | OPC_CP2,

    OPC_SLL_CP2     = (24 << 21) | (0x0E) | OPC_CP2,
    OPC_DSLL_CP2    = (25 << 21) | (0x0E) | OPC_CP2,
    OPC_PEXTRH      = (26 << 21) | (0x0E) | OPC_CP2,
    OPC_PMADDHW     = (27 << 21) | (0x0E) | OPC_CP2,
    OPC_SLEU_CP2    = (28 << 21) | (0x0E) | OPC_CP2,
    OPC_SLE_CP2     = (29 << 21) | (0x0E) | OPC_CP2,

    OPC_SRL_CP2     = (24 << 21) | (0x0F) | OPC_CP2,
    OPC_DSRL_CP2    = (25 << 21) | (0x0F) | OPC_CP2,
    OPC_SRA_CP2     = (26 << 21) | (0x0F) | OPC_CP2,
    OPC_DSRA_CP2    = (27 << 21) | (0x0F) | OPC_CP2,
    OPC_BIADD       = (28 << 21) | (0x0F) | OPC_CP2,
    OPC_PMOVMSKB    = (29 << 21) | (0x0F) | OPC_CP2,
};


#define MASK_CP3(op)                (MASK_OP_MAJOR(op) | (op & 0x3F))

enum {
    OPC_LWXC1       = 0x00 | OPC_CP3,
    OPC_LDXC1       = 0x01 | OPC_CP3,
    OPC_LUXC1       = 0x05 | OPC_CP3,
    OPC_SWXC1       = 0x08 | OPC_CP3,
    OPC_SDXC1       = 0x09 | OPC_CP3,
    OPC_SUXC1       = 0x0D | OPC_CP3,
    OPC_PREFX       = 0x0F | OPC_CP3,
    OPC_ALNV_PS     = 0x1E | OPC_CP3,
    OPC_MADD_S      = 0x20 | OPC_CP3,
    OPC_MADD_D      = 0x21 | OPC_CP3,
    OPC_MADD_PS     = 0x26 | OPC_CP3,
    OPC_MSUB_S      = 0x28 | OPC_CP3,
    OPC_MSUB_D      = 0x29 | OPC_CP3,
    OPC_MSUB_PS     = 0x2E | OPC_CP3,
    OPC_NMADD_S     = 0x30 | OPC_CP3,
    OPC_NMADD_D     = 0x31 | OPC_CP3,
    OPC_NMADD_PS    = 0x36 | OPC_CP3,
    OPC_NMSUB_S     = 0x38 | OPC_CP3,
    OPC_NMSUB_D     = 0x39 | OPC_CP3,
    OPC_NMSUB_PS    = 0x3E | OPC_CP3,
};

/*
 *     MMI (MultiMedia Instruction) encodings
 *     ======================================
 *
 * MMI instructions encoding table keys:
 *
 *     *   This code is reserved for future use. An attempt to execute it
 *         causes a Reserved Instruction exception.
 *     %   This code indicates an instruction class. The instruction word
 *         must be further decoded by examining additional tables that show
 *         the values for other instruction fields.
 *     #   This code is reserved for the unsupported instructions DMULT,
 *         DMULTU, DDIV, DDIVU, LL, LLD, SC, SCD, LWC2 and SWC2. An attempt
 *         to execute it causes a Reserved Instruction exception.
 *
 * MMI instructions encoded by opcode field (MMI, LQ, SQ):
 *
 *  31    26                                        0
 * +--------+----------------------------------------+
 * | opcode |                                        |
 * +--------+----------------------------------------+
 *
 *   opcode  bits 28..26
 *     bits |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7
 *   31..29 |  000  |  001  |  010  |  011  |  100  |  101  |  110  |  111
 *   -------+-------+-------+-------+-------+-------+-------+-------+-------
 *    0 000 |SPECIAL| REGIMM|   J   |  JAL  |  BEQ  |  BNE  |  BLEZ |  BGTZ
 *    1 001 |  ADDI | ADDIU |  SLTI | SLTIU |  ANDI |  ORI  |  XORI |  LUI
 *    2 010 |  COP0 |  COP1 |   *   |   *   |  BEQL |  BNEL | BLEZL | BGTZL
 *    3 011 | DADDI | DADDIU|  LDL  |  LDR  |  MMI% |   *   |   LQ  |   SQ
 *    4 100 |   LB  |   LH  |  LWL  |   LW  |  LBU  |  LHU  |  LWR  |  LWU
 *    5 101 |   SB  |   SH  |  SWL  |   SW  |  SDL  |  SDR  |  SWR  | CACHE
 *    6 110 |   #   |  LWC1 |   #   |  PREF |   #   |  LDC1 |   #   |   LD
 *    7 111 |   #   |  SWC1 |   #   |   *   |   #   |  SDC1 |   #   |   SD
 */

enum {
    MMI_OPC_CLASS_MMI = 0x1C << 26,    /* Same as OPC_SPECIAL2 */
    MMI_OPC_LQ        = 0x1E << 26,    /* Same as OPC_MSA */
    MMI_OPC_SQ        = 0x1F << 26,    /* Same as OPC_SPECIAL3 */
};

/*
 * MMI instructions with opcode field = MMI:
 *
 *  31    26                                 5      0
 * +--------+-------------------------------+--------+
 * |   MMI  |                               |function|
 * +--------+-------------------------------+--------+
 *
 * function  bits 2..0
 *     bits |   0   |   1   |   2   |   3   |   4   |   5   |   6   |   7
 *     5..3 |  000  |  001  |  010  |  011  |  100  |  101  |  110  |  111
 *   -------+-------+-------+-------+-------+-------+-------+-------+-------
 *    0 000 |  MADD | MADDU |   *   |   *   | PLZCW |   *   |   *   |   *
 *    1 001 | MMI0% | MMI2% |   *   |   *   |   *   |   *   |   *   |   *
 *    2 010 | MFHI1 | MTHI1 | MFLO1 | MTLO1 |   *   |   *   |   *   |   *
 *    3 011 | MULT1 | MULTU1|  DIV1 | DIVU1 |   *   |   *   |   *   |   *
 *    4 100 | MADD1 | MADDU1|   *   |   *   |   *   |   *   |   *   |   *
 *    5 101 | MMI1% | MMI3% |   *   |   *   |   *   |   *   |   *   |   *
 *    6 110 | PMFHL | PMTHL |   *   |   *   | PSLLH |   *   | PSRLH | PSRAH
 *    7 111 |   *   |   *   |   *   |   *   | PSLLW |   *   | PSRLW | PSRAW
 */

#define MASK_MMI(op) (MASK_OP_MAJOR(op) | ((op) & 0x3F))
enum {
    MMI_OPC_MADD       = 0x00 | MMI_OPC_CLASS_MMI, /* Same as OPC_MADD */
    MMI_OPC_MADDU      = 0x01 | MMI_OPC_CLASS_MMI, /* Same as OPC_MADDU */
    MMI_OPC_MULT1      = 0x18 | MMI_OPC_CLASS_MMI, /* Same minor as OPC_MULT */
    MMI_OPC_MULTU1     = 0x19 | MMI_OPC_CLASS_MMI, /* Same min. as OPC_MULTU */
    MMI_OPC_DIV1       = 0x1A | MMI_OPC_CLASS_MMI, /* Same minor as OPC_DIV  */
    MMI_OPC_DIVU1      = 0x1B | MMI_OPC_CLASS_MMI, /* Same minor as OPC_DIVU */
    MMI_OPC_MADD1      = 0x20 | MMI_OPC_CLASS_MMI,
    MMI_OPC_MADDU1     = 0x21 | MMI_OPC_CLASS_MMI,
};

/* global register indices */
TCGv cpu_gpr[32], cpu_PC;
/*
 * For CPUs using 128-bit GPR registers, we put the lower halves in cpu_gpr[])
 * and the upper halves in cpu_gpr_hi[].
 */
TCGv_i64 cpu_gpr_hi[32];
TCGv cpu_HI[MIPS_DSP_ACC], cpu_LO[MIPS_DSP_ACC];
static TCGv cpu_dspctrl, btarget;
TCGv bcond;
static TCGv cpu_lladdr, cpu_llval;
static TCGv_i32 hflags;
TCGv_i32 fpu_fcr0, fpu_fcr31;
TCGv_i64 fpu_f64[32];
static TCGv cpu_statcounters_icount_kernel, cpu_statcounters_icount_user;


#include "exec/gen-icount.h"

#define gen_helper_0e0i(name, arg) do {                           \
    TCGv_i32 helper_tmp = tcg_const_i32(arg);                     \
    gen_helper_##name(cpu_env, helper_tmp);                       \
    tcg_temp_free_i32(helper_tmp);                                \
    } while (0)

#define gen_helper_0e1i(name, arg1, arg2) do {                    \
    TCGv_i32 helper_tmp = tcg_const_i32(arg2);                    \
    gen_helper_##name(cpu_env, arg1, helper_tmp);                 \
    tcg_temp_free_i32(helper_tmp);                                \
    } while (0)

#define gen_helper_1e0i(name, ret, arg1) do {                     \
    TCGv_i32 helper_tmp = tcg_const_i32(arg1);                    \
    gen_helper_##name(ret, cpu_env, helper_tmp);                  \
    tcg_temp_free_i32(helper_tmp);                                \
    } while (0)

#define gen_helper_1e1i(name, ret, arg1, arg2) do {               \
    TCGv_i32 helper_tmp = tcg_const_i32(arg2);                    \
    gen_helper_##name(ret, cpu_env, arg1, helper_tmp);            \
    tcg_temp_free_i32(helper_tmp);                                \
    } while (0)

#define gen_helper_0e2i(name, arg1, arg2, arg3) do {              \
    TCGv_i32 helper_tmp = tcg_const_i32(arg3);                    \
    gen_helper_##name(cpu_env, arg1, arg2, helper_tmp);           \
    tcg_temp_free_i32(helper_tmp);                                \
    } while (0)

#define gen_helper_1e2i(name, ret, arg1, arg2, arg3) do {         \
    TCGv_i32 helper_tmp = tcg_const_i32(arg3);                    \
    gen_helper_##name(ret, cpu_env, arg1, arg2, helper_tmp);      \
    tcg_temp_free_i32(helper_tmp);                                \
    } while (0)

#define gen_helper_0e3i(name, arg1, arg2, arg3, arg4) do {        \
    TCGv_i32 helper_tmp = tcg_const_i32(arg4);                    \
    gen_helper_##name(cpu_env, arg1, arg2, arg3, helper_tmp);     \
    tcg_temp_free_i32(helper_tmp);                                \
    } while (0)


#define gen_helper_2_consti32(name, arg, arg2) do {               \
    TCGv_i32 helper_tmp = tcg_const_i32(arg);                     \
    TCGv_i32 helper_tmp2 = tcg_const_i32(arg2);                   \
    gen_helper_##name(cpu_env, helper_tmp, helper_tmp2);          \
    tcg_temp_free_i32(helper_tmp);                                \
    tcg_temp_free_i32(helper_tmp2);                               \
    } while(0)


#define DISAS_STOP       DISAS_TARGET_0
#define DISAS_EXIT       DISAS_TARGET_1

static inline void mips_update_statcounters_icount(DisasContext *ctx);

/* General purpose registers moves. */
void gen_load_gpr(TCGv t, int reg)
{
    if (reg == 0) {
        tcg_gen_movi_tl(t, 0);
    } else {
        tcg_gen_mov_tl(t, cpu_gpr[reg]);
    }
}

void _gen_store_gpr(DisasContext *ctx, TCGv t, int reg)
{
    if (reg != 0) {
        tcg_gen_mov_tl(cpu_gpr[reg], t);
        gen_log_instr_gpr_update(ctx, reg);
    }
}


#if defined(TARGET_MIPS64)
void gen_load_gpr_hi(TCGv_i64 t, int reg)
{
    if (reg == 0) {
        tcg_gen_movi_i64(t, 0);
    } else {
        tcg_gen_mov_i64(t, cpu_gpr_hi[reg]);
    }
}

void gen_store_gpr_hi(TCGv_i64 t, int reg)
{
    if (reg != 0) {
        tcg_gen_mov_i64(cpu_gpr_hi[reg], t);
    }
}
#endif /* TARGET_MIPS64 */

/* Moves to/from shadow registers. */
static inline void gen_load_srsgpr(DisasContext *ctx, int from, int to)
{
    TCGv t0 = tcg_temp_new();

    if (from == 0) {
        tcg_gen_movi_tl(t0, 0);
    } else {
        TCGv_i32 t2 = tcg_temp_new_i32();
        TCGv_ptr addr = tcg_temp_new_ptr();

        tcg_gen_ld_i32(t2, cpu_env, offsetof(CPUMIPSState, CP0_SRSCtl));
        tcg_gen_shri_i32(t2, t2, CP0SRSCtl_PSS);
        tcg_gen_andi_i32(t2, t2, 0xf);
        tcg_gen_muli_i32(t2, t2, sizeof(target_ulong) * 32);
        tcg_gen_ext_i32_ptr(addr, t2);
        tcg_gen_add_ptr(addr, cpu_env, addr);

        tcg_gen_ld_tl(t0, addr, sizeof(target_ulong) * from);
        tcg_temp_free_ptr(addr);
        tcg_temp_free_i32(t2);
    }
    gen_store_gpr(t0, to);
    tcg_temp_free(t0);
}

static inline void gen_store_srsgpr(int from, int to)
{
    if (to != 0) {
        TCGv t0 = tcg_temp_new();
        TCGv_i32 t2 = tcg_temp_new_i32();
        TCGv_ptr addr = tcg_temp_new_ptr();

        gen_load_gpr(t0, from);
        tcg_gen_ld_i32(t2, cpu_env, offsetof(CPUMIPSState, CP0_SRSCtl));
        tcg_gen_shri_i32(t2, t2, CP0SRSCtl_PSS);
        tcg_gen_andi_i32(t2, t2, 0xf);
        tcg_gen_muli_i32(t2, t2, sizeof(target_ulong) * 32);
        tcg_gen_ext_i32_ptr(addr, t2);
        tcg_gen_add_ptr(addr, cpu_env, addr);

        tcg_gen_st_tl(t0, addr, sizeof(target_ulong) * to);
        tcg_temp_free_ptr(addr);
        tcg_temp_free_i32(t2);
        tcg_temp_free(t0);
    }
}

/* Tests */
static inline void gen_save_pc(target_ulong pc)
{
    tcg_gen_movi_tl(cpu_PC, pc);
#ifdef CONFIG_DEBUG_TCG
    tcg_gen_movi_tl(_pc_is_current, 1); // PC has been updated.
#endif
}
#include "cheri-translate-utils.h"

static inline void save_cpu_state(DisasContext *ctx, int do_save_pc)
{
    LOG_DISAS("hflags %08x saved %08x\n", ctx->hflags, ctx->saved_hflags);
    if (do_save_pc && ctx->base.pc_next != ctx->saved_pc) {
        gen_save_pc(ctx->base.pc_next);
        ctx->saved_pc = ctx->base.pc_next;
    }
    if (ctx->hflags != ctx->saved_hflags) {
        tcg_gen_movi_i32(hflags, ctx->hflags);
        ctx->saved_hflags = ctx->hflags;
        switch (ctx->hflags & MIPS_HFLAG_BMASK_BASE) {
        case MIPS_HFLAG_BR:
            break;
#ifdef TARGET_CHERI
        case MIPS_HFLAG_BRCCALL:
        case MIPS_HFLAG_BRC:
            break;
#endif
        case MIPS_HFLAG_BC:
        case MIPS_HFLAG_BL:
        case MIPS_HFLAG_B:
            tcg_gen_movi_tl(btarget, ctx->btarget);
            break;
        }
    }
}

static inline void restore_cpu_state(CPUMIPSState *env, DisasContext *ctx)
{
    ctx->saved_hflags = ctx->hflags;
    switch (ctx->hflags & MIPS_HFLAG_BMASK_BASE) {
    case MIPS_HFLAG_BR:
        break;
#ifdef TARGET_CHERI
    case MIPS_HFLAG_BRCCALL:
    case MIPS_HFLAG_BRC:
        break;
#endif
    case MIPS_HFLAG_BC:
    case MIPS_HFLAG_BL:
    case MIPS_HFLAG_B:
        ctx->btarget = env->btarget;
        break;
    }
}

void generate_exception_err(DisasContext *ctx, MipsExcp excp, int err)
{
    TCGv_i32 texcp = tcg_const_i32(excp);
    TCGv_i32 terr = tcg_const_i32(err);
    save_cpu_state(ctx, 1);
    gen_helper_raise_exception_err(cpu_env, texcp, terr);
    tcg_temp_free_i32(terr);
    tcg_temp_free_i32(texcp);
    ctx->base.is_jmp = DISAS_NORETURN;
}

void generate_exception(DisasContext *ctx, int excp)
{
    gen_helper_0e0i(raise_exception, excp);
}

void generate_exception_end(DisasContext *ctx, int excp)
{
    generate_exception_err(ctx, excp, 0);
}

void cheri_tcg_save_pc(DisasContextBase *db) { gen_save_pc(db->pc_next); }
// We have to save the current PC before setting DISAS_NORETURN (see
// generate_exception_err())
void cheri_tcg_prepare_for_unconditional_exception(DisasContextBase *db)
{
    cheri_tcg_save_pc(db);
    db->is_jmp = DISAS_NORETURN;
}

void gen_reserved_instruction(DisasContext *ctx)
{
    generate_exception_end(ctx, EXCP_RI);
}

/* Floating point register moves. */
void gen_load_fpr32(DisasContext *ctx, TCGv_i32 t, int reg)
{
    if (ctx->hflags & MIPS_HFLAG_FRE) {
        generate_exception(ctx, EXCP_RI);
    }
    tcg_gen_extrl_i64_i32(t, fpu_f64[reg]);
}

void gen_store_fpr32(DisasContext *ctx, TCGv_i32 t, int reg)
{
    TCGv_i64 t64;
    if (ctx->hflags & MIPS_HFLAG_FRE) {
        generate_exception(ctx, EXCP_RI);
    }
    t64 = tcg_temp_new_i64();
    tcg_gen_extu_i32_i64(t64, t);
    tcg_gen_deposit_i64(fpu_f64[reg], fpu_f64[reg], t64, 0, 32);
    tcg_temp_free_i64(t64);
}

static void gen_load_fpr32h(DisasContext *ctx, TCGv_i32 t, int reg)
{
    if (ctx->hflags & MIPS_HFLAG_F64) {
        tcg_gen_extrh_i64_i32(t, fpu_f64[reg]);
    } else {
        gen_load_fpr32(ctx, t, reg | 1);
    }
}

static void gen_store_fpr32h(DisasContext *ctx, TCGv_i32 t, int reg)
{
    if (ctx->hflags & MIPS_HFLAG_F64) {
        TCGv_i64 t64 = tcg_temp_new_i64();
        tcg_gen_extu_i32_i64(t64, t);
        tcg_gen_deposit_i64(fpu_f64[reg], fpu_f64[reg], t64, 32, 32);
        tcg_temp_free_i64(t64);
    } else {
        gen_store_fpr32(ctx, t, reg | 1);
    }
}

void gen_load_fpr64(DisasContext *ctx, TCGv_i64 t, int reg)
{
    if (ctx->hflags & MIPS_HFLAG_F64) {
        tcg_gen_mov_i64(t, fpu_f64[reg]);
    } else {
        tcg_gen_concat32_i64(t, fpu_f64[reg & ~1], fpu_f64[reg | 1]);
    }
}

void gen_store_fpr64(DisasContext *ctx, TCGv_i64 t, int reg)
{
    if (ctx->hflags & MIPS_HFLAG_F64) {
        tcg_gen_mov_i64(fpu_f64[reg], t);
    } else {
        TCGv_i64 t0;
        tcg_gen_deposit_i64(fpu_f64[reg & ~1], fpu_f64[reg & ~1], t, 0, 32);
        t0 = tcg_temp_new_i64();
        tcg_gen_shri_i64(t0, t, 32);
        tcg_gen_deposit_i64(fpu_f64[reg | 1], fpu_f64[reg | 1], t0, 0, 32);
        tcg_temp_free_i64(t0);
    }
}

int get_fp_bit(int cc)
{
    if (cc) {
        return 24 + cc;
    } else {
        return 23;
    }
}

/* Addresses computation */
void gen_op_addr_add(DisasContext *ctx, TCGv ret, TCGv arg0, TCGv arg1)
{
    tcg_gen_add_tl(ret, arg0, arg1);

#if defined(TARGET_MIPS64)
    if (ctx->hflags & MIPS_HFLAG_AWRAP) {
        tcg_gen_ext32s_i64(ret, ret);
    }
#endif
}

#include "translate_cheri.c"

#ifndef TARGET_CHERI
static inline void gen_op_addr_addi(DisasContext *ctx, TCGv ret, TCGv base,
                                    target_long ofs)
{
    tcg_gen_addi_tl(ret, base, ofs);

#if defined(TARGET_MIPS64)
    if (ctx->hflags & MIPS_HFLAG_AWRAP) {
        tcg_gen_ext32s_i64(ret, ret);
    }
#endif
}
#endif

/* Addresses computation (translation time) */
static target_long addr_add(DisasContext *ctx, target_long base,
                            target_long offset)
{
    target_long sum = base + offset;

#if defined(TARGET_MIPS64)
    if (ctx->hflags & MIPS_HFLAG_AWRAP) {
        sum = (int32_t)sum;
    }
#endif
    return sum;
}

/* Sign-extract the low 32-bits to a target_long.  */
void gen_move_low32(TCGv ret, TCGv_i64 arg)
{
#if defined(TARGET_MIPS64)
    tcg_gen_ext32s_i64(ret, arg);
#else
    tcg_gen_extrl_i64_i32(ret, arg);
#endif
}

/* Sign-extract the high 32-bits to a target_long.  */
void gen_move_high32(TCGv ret, TCGv_i64 arg)
{
#if defined(TARGET_MIPS64)
    tcg_gen_sari_i64(ret, arg, 32);
#else
    tcg_gen_extrh_i64_i32(ret, arg);
#endif
}

void check_cp0_enabled(DisasContext *ctx)
{
    if (unlikely(!(ctx->hflags & MIPS_HFLAG_CP0))) {
        generate_exception_end(ctx, EXCP_CpU);
    }
}

void check_cp1_enabled(DisasContext *ctx)
{
    if (unlikely(!(ctx->hflags & MIPS_HFLAG_FPU))) {
        generate_exception_err(ctx, EXCP_CpU, 1);
    }
}

/*
 * Verify that the processor is running with COP1X instructions enabled.
 * This is associated with the nabla symbol in the MIPS32 and MIPS64
 * opcode tables.
 */
void check_cop1x(DisasContext *ctx)
{
    if (unlikely(!(ctx->hflags & MIPS_HFLAG_COP1X))) {
        gen_reserved_instruction(ctx);
    }
}

/*
 * Verify that the processor is running with 64-bit floating-point
 * operations enabled.
 */
void check_cp1_64bitmode(DisasContext *ctx)
{
    if (unlikely(~ctx->hflags & (MIPS_HFLAG_F64 | MIPS_HFLAG_COP1X))) {
        gen_reserved_instruction(ctx);
    }
}

/*
 * Verify if floating point register is valid; an operation is not defined
 * if bit 0 of any register specification is set and the FR bit in the
 * Status register equals zero, since the register numbers specify an
 * even-odd pair of adjacent coprocessor general registers. When the FR bit
 * in the Status register equals one, both even and odd register numbers
 * are valid. This limitation exists only for 64 bit wide (d,l,ps) registers.
 *
 * Multiple 64 bit wide registers can be checked by calling
 * gen_op_cp1_registers(freg1 | freg2 | ... | fregN);
 */
void check_cp1_registers(DisasContext *ctx, int regs)
{
    if (unlikely(!(ctx->hflags & MIPS_HFLAG_F64) && (regs & 1))) {
        gen_reserved_instruction(ctx);
    }
}

/*
 * Verify that the processor is running with DSP instructions enabled.
 * This is enabled by CP0 Status register MX(24) bit.
 */
static inline void check_dsp(DisasContext *ctx)
{
    if (unlikely(!(ctx->hflags & MIPS_HFLAG_DSP))) {
        if (ctx->insn_flags & ASE_DSP) {
            generate_exception_end(ctx, EXCP_DSPDIS);
        } else {
            gen_reserved_instruction(ctx);
        }
    }
}

static inline void check_dsp_r2(DisasContext *ctx)
{
    if (unlikely(!(ctx->hflags & MIPS_HFLAG_DSP_R2))) {
        if (ctx->insn_flags & ASE_DSP) {
            generate_exception_end(ctx, EXCP_DSPDIS);
        } else {
            gen_reserved_instruction(ctx);
        }
    }
}

#ifndef TARGET_CHERI
static inline void check_dsp_r3(DisasContext *ctx)
{
    if (unlikely(!(ctx->hflags & MIPS_HFLAG_DSP_R3))) {
        if (ctx->insn_flags & ASE_DSP) {
            generate_exception_end(ctx, EXCP_DSPDIS);
        } else {
            gen_reserved_instruction(ctx);
        }
    }
}
#endif

/*
 * This code generates a "reserved instruction" exception if the
 * CPU does not support the instruction set corresponding to flags.
 */
void check_insn(DisasContext *ctx, uint64_t flags)
{
    if (unlikely(!(ctx->insn_flags & flags))) {
        gen_reserved_instruction(ctx);
    }
}

/*
 * This code generates a "reserved instruction" exception if the
 * CPU has corresponding flag set which indicates that the instruction
 * has been removed.
 */
static inline void check_insn_opc_removed(DisasContext *ctx, uint64_t flags)
{
    if (unlikely(ctx->insn_flags & flags)) {
        gen_reserved_instruction(ctx);
    }
}

/*
 * The Linux kernel traps certain reserved instruction exceptions to
 * emulate the corresponding instructions. QEMU is the kernel in user
 * mode, so those traps are emulated by accepting the instructions.
 *
 * A reserved instruction exception is generated for flagged CPUs if
 * QEMU runs in system mode.
 */
static inline void check_insn_opc_user_only(DisasContext *ctx, uint64_t flags)
{
#ifndef CONFIG_USER_ONLY
    check_insn_opc_removed(ctx, flags);
#endif
}

/*
 * This code generates a "reserved instruction" exception if the
 * CPU does not support 64-bit paired-single (PS) floating point data type.
 */
static inline void check_ps(DisasContext *ctx)
{
    if (unlikely(!ctx->ps)) {
        generate_exception(ctx, EXCP_RI);
    }
    check_cp1_64bitmode(ctx);
}

/*
 * This code generates a "reserved instruction" exception if cpu is not
 * 64-bit or 64-bit instructions are not enabled.
 */
void check_mips_64(DisasContext *ctx)
{
    if (unlikely((TARGET_LONG_BITS != 64) || !(ctx->hflags & MIPS_HFLAG_64))) {
        gen_reserved_instruction(ctx);
    }
}

#ifndef CONFIG_USER_ONLY
static inline void check_mvh(DisasContext *ctx)
{
    if (unlikely(!ctx->mvh)) {
        generate_exception(ctx, EXCP_RI);
    }
}
#endif

/*
 * This code generates a "reserved instruction" exception if the
 * Config5 XNP bit is set.
 */
#ifndef TARGET_CHERI
static inline void check_xnp(DisasContext *ctx)
{
    if (unlikely(ctx->CP0_Config5 & (1 << CP0C5_XNP))) {
        gen_reserved_instruction(ctx);
    }
}
#endif

#ifndef CONFIG_USER_ONLY
/*
 * This code generates a "reserved instruction" exception if the
 * Config3 PW bit is NOT set.
 */
static inline void check_pw(DisasContext *ctx)
{
    if (unlikely(!(ctx->CP0_Config3 & (1 << CP0C3_PW)))) {
        gen_reserved_instruction(ctx);
    }
}
#endif

/*
 * This code generates a "reserved instruction" exception if the
 * Config3 MT bit is NOT set.
 */
static inline void check_mt(DisasContext *ctx)
{
    if (unlikely(!(ctx->CP0_Config3 & (1 << CP0C3_MT)))) {
        gen_reserved_instruction(ctx);
    }
}

#ifndef CONFIG_USER_ONLY
/*
 * This code generates a "coprocessor unusable" exception if CP0 is not
 * available, and, if that is not the case, generates a "reserved instruction"
 * exception if the Config5 MT bit is NOT set. This is needed for availability
 * control of some of MT ASE instructions.
 */
static inline void check_cp0_mt(DisasContext *ctx)
{
    if (unlikely(!(ctx->hflags & MIPS_HFLAG_CP0))) {
        generate_exception_end(ctx, EXCP_CpU);
    } else {
        if (unlikely(!(ctx->CP0_Config3 & (1 << CP0C3_MT)))) {
            gen_reserved_instruction(ctx);
        }
    }
}
#endif

/*
 * This code generates a "reserved instruction" exception if the
 * Config5 NMS bit is set.
 */
#ifndef TARGET_CHERI
static inline void check_nms(DisasContext *ctx)
{
    if (unlikely(ctx->CP0_Config5 & (1 << CP0C5_NMS))) {
        gen_reserved_instruction(ctx);
    }
}

/*
 * This code generates a "reserved instruction" exception if the
 * Config5 NMS bit is set, and Config1 DL, Config1 IL, Config2 SL,
 * Config2 TL, and Config5 L2C are unset.
 */
static inline void check_nms_dl_il_sl_tl_l2c(DisasContext *ctx)
{
    if (unlikely((ctx->CP0_Config5 & (1 << CP0C5_NMS)) &&
                 !(ctx->CP0_Config1 & (1 << CP0C1_DL)) &&
                 !(ctx->CP0_Config1 & (1 << CP0C1_IL)) &&
                 !(ctx->CP0_Config2 & (1 << CP0C2_SL)) &&
                 !(ctx->CP0_Config2 & (1 << CP0C2_TL)) &&
                 !(ctx->CP0_Config5 & (1 << CP0C5_L2C)))) {
        gen_reserved_instruction(ctx);
    }
}

/*
 * This code generates a "reserved instruction" exception if the
 * Config5 EVA bit is NOT set.
 */
static inline void check_eva(DisasContext *ctx)
{
    if (unlikely(!(ctx->CP0_Config5 & (1 << CP0C5_EVA)))) {
        gen_reserved_instruction(ctx);
    }
}
#endif // !TARGET_CHERI


/*
 * Define small wrappers for gen_load_fpr* so that we have a uniform
 * calling interface for 32 and 64-bit FPRs.  No sense in changing
 * all callers for gen_load_fpr32 when we need the CTX parameter for
 * this one use.
 */
#define gen_ldcmp_fpr32(ctx, x, y) gen_load_fpr32(ctx, x, y)
#define gen_ldcmp_fpr64(ctx, x, y) gen_load_fpr64(ctx, x, y)
#define FOP_CONDS(type, abs, fmt, ifmt, bits)                                 \
static inline void gen_cmp ## type ## _ ## fmt(DisasContext *ctx, int n,      \
                                               int ft, int fs, int cc)        \
{                                                                             \
    TCGv_i##bits fp0 = tcg_temp_new_i##bits();                                \
    TCGv_i##bits fp1 = tcg_temp_new_i##bits();                                \
    switch (ifmt) {                                                           \
    case FMT_PS:                                                              \
        check_ps(ctx);                                                        \
        break;                                                                \
    case FMT_D:                                                               \
        if (abs) {                                                            \
            check_cop1x(ctx);                                                 \
        }                                                                     \
        check_cp1_registers(ctx, fs | ft);                                    \
        break;                                                                \
    case FMT_S:                                                               \
        if (abs) {                                                            \
            check_cop1x(ctx);                                                 \
        }                                                                     \
        break;                                                                \
    }                                                                         \
    gen_ldcmp_fpr##bits(ctx, fp0, fs);                                        \
    gen_ldcmp_fpr##bits(ctx, fp1, ft);                                        \
    switch (n) {                                                              \
    case  0:                                                                  \
        gen_helper_0e2i(cmp ## type ## _ ## fmt ## _f, fp0, fp1, cc);         \
    break;                                                                    \
    case  1:                                                                  \
        gen_helper_0e2i(cmp ## type ## _ ## fmt ## _un, fp0, fp1, cc);        \
    break;                                                                    \
    case  2:                                                                  \
        gen_helper_0e2i(cmp ## type ## _ ## fmt ## _eq, fp0, fp1, cc);        \
    break;                                                                    \
    case  3:                                                                  \
        gen_helper_0e2i(cmp ## type ## _ ## fmt ## _ueq, fp0, fp1, cc);       \
    break;                                                                    \
    case  4:                                                                  \
        gen_helper_0e2i(cmp ## type ## _ ## fmt ## _olt, fp0, fp1, cc);       \
    break;                                                                    \
    case  5:                                                                  \
        gen_helper_0e2i(cmp ## type ## _ ## fmt ## _ult, fp0, fp1, cc);       \
    break;                                                                    \
    case  6:                                                                  \
        gen_helper_0e2i(cmp ## type ## _ ## fmt ## _ole, fp0, fp1, cc);       \
    break;                                                                    \
    case  7:                                                                  \
        gen_helper_0e2i(cmp ## type ## _ ## fmt ## _ule, fp0, fp1, cc);       \
    break;                                                                    \
    case  8:                                                                  \
        gen_helper_0e2i(cmp ## type ## _ ## fmt ## _sf, fp0, fp1, cc);        \
    break;                                                                    \
    case  9:                                                                  \
        gen_helper_0e2i(cmp ## type ## _ ## fmt ## _ngle, fp0, fp1, cc);      \
    break;                                                                    \
    case 10:                                                                  \
        gen_helper_0e2i(cmp ## type ## _ ## fmt ## _seq, fp0, fp1, cc);       \
    break;                                                                    \
    case 11:                                                                  \
        gen_helper_0e2i(cmp ## type ## _ ## fmt ## _ngl, fp0, fp1, cc);       \
    break;                                                                    \
    case 12:                                                                  \
        gen_helper_0e2i(cmp ## type ## _ ## fmt ## _lt, fp0, fp1, cc);        \
    break;                                                                    \
    case 13:                                                                  \
        gen_helper_0e2i(cmp ## type ## _ ## fmt ## _nge, fp0, fp1, cc);       \
    break;                                                                    \
    case 14:                                                                  \
        gen_helper_0e2i(cmp ## type ## _ ## fmt ## _le, fp0, fp1, cc);        \
    break;                                                                    \
    case 15:                                                                  \
        gen_helper_0e2i(cmp ## type ## _ ## fmt ## _ngt, fp0, fp1, cc);       \
    break;                                                                    \
    default:                                                                  \
        abort();                                                              \
    }                                                                         \
    tcg_temp_free_i##bits(fp0);                                               \
    tcg_temp_free_i##bits(fp1);                                               \
}

FOP_CONDS(, 0, d, FMT_D, 64)
FOP_CONDS(abs, 1, d, FMT_D, 64)
FOP_CONDS(, 0, s, FMT_S, 32)
FOP_CONDS(abs, 1, s, FMT_S, 32)
FOP_CONDS(, 0, ps, FMT_PS, 64)
FOP_CONDS(abs, 1, ps, FMT_PS, 64)
#undef FOP_CONDS

#define FOP_CONDNS(fmt, ifmt, bits, STORE)                              \
static inline void gen_r6_cmp_ ## fmt(DisasContext *ctx, int n,         \
                                      int ft, int fs, int fd)           \
{                                                                       \
    TCGv_i ## bits fp0 = tcg_temp_new_i ## bits();                      \
    TCGv_i ## bits fp1 = tcg_temp_new_i ## bits();                      \
    if (ifmt == FMT_D) {                                                \
        check_cp1_registers(ctx, fs | ft | fd);                         \
    }                                                                   \
    gen_ldcmp_fpr ## bits(ctx, fp0, fs);                                \
    gen_ldcmp_fpr ## bits(ctx, fp1, ft);                                \
    switch (n) {                                                        \
    case  0:                                                            \
        gen_helper_r6_cmp_ ## fmt ## _af(fp0, cpu_env, fp0, fp1);       \
        break;                                                          \
    case  1:                                                            \
        gen_helper_r6_cmp_ ## fmt ## _un(fp0, cpu_env, fp0, fp1);       \
        break;                                                          \
    case  2:                                                            \
        gen_helper_r6_cmp_ ## fmt ## _eq(fp0, cpu_env, fp0, fp1);       \
        break;                                                          \
    case  3:                                                            \
        gen_helper_r6_cmp_ ## fmt ## _ueq(fp0, cpu_env, fp0, fp1);      \
        break;                                                          \
    case  4:                                                            \
        gen_helper_r6_cmp_ ## fmt ## _lt(fp0, cpu_env, fp0, fp1);       \
        break;                                                          \
    case  5:                                                            \
        gen_helper_r6_cmp_ ## fmt ## _ult(fp0, cpu_env, fp0, fp1);      \
        break;                                                          \
    case  6:                                                            \
        gen_helper_r6_cmp_ ## fmt ## _le(fp0, cpu_env, fp0, fp1);       \
        break;                                                          \
    case  7:                                                            \
        gen_helper_r6_cmp_ ## fmt ## _ule(fp0, cpu_env, fp0, fp1);      \
        break;                                                          \
    case  8:                                                            \
        gen_helper_r6_cmp_ ## fmt ## _saf(fp0, cpu_env, fp0, fp1);      \
        break;                                                          \
    case  9:                                                            \
        gen_helper_r6_cmp_ ## fmt ## _sun(fp0, cpu_env, fp0, fp1);      \
        break;                                                          \
    case 10:                                                            \
        gen_helper_r6_cmp_ ## fmt ## _seq(fp0, cpu_env, fp0, fp1);      \
        break;                                                          \
    case 11:                                                            \
        gen_helper_r6_cmp_ ## fmt ## _sueq(fp0, cpu_env, fp0, fp1);     \
        break;                                                          \
    case 12:                                                            \
        gen_helper_r6_cmp_ ## fmt ## _slt(fp0, cpu_env, fp0, fp1);      \
        break;                                                          \
    case 13:                                                            \
        gen_helper_r6_cmp_ ## fmt ## _sult(fp0, cpu_env, fp0, fp1);     \
        break;                                                          \
    case 14:                                                            \
        gen_helper_r6_cmp_ ## fmt ## _sle(fp0, cpu_env, fp0, fp1);      \
        break;                                                          \
    case 15:                                                            \
        gen_helper_r6_cmp_ ## fmt ## _sule(fp0, cpu_env, fp0, fp1);     \
        break;                                                          \
    case 17:                                                            \
        gen_helper_r6_cmp_ ## fmt ## _or(fp0, cpu_env, fp0, fp1);       \
        break;                                                          \
    case 18:                                                            \
        gen_helper_r6_cmp_ ## fmt ## _une(fp0, cpu_env, fp0, fp1);      \
        break;                                                          \
    case 19:                                                            \
        gen_helper_r6_cmp_ ## fmt ## _ne(fp0, cpu_env, fp0, fp1);       \
        break;                                                          \
    case 25:                                                            \
        gen_helper_r6_cmp_ ## fmt ## _sor(fp0, cpu_env, fp0, fp1);      \
        break;                                                          \
    case 26:                                                            \
        gen_helper_r6_cmp_ ## fmt ## _sune(fp0, cpu_env, fp0, fp1);     \
        break;                                                          \
    case 27:                                                            \
        gen_helper_r6_cmp_ ## fmt ## _sne(fp0, cpu_env, fp0, fp1);      \
        break;                                                          \
    default:                                                            \
        abort();                                                        \
    }                                                                   \
    STORE;                                                              \
    tcg_temp_free_i ## bits(fp0);                                       \
    tcg_temp_free_i ## bits(fp1);                                       \
}

FOP_CONDNS(d, FMT_D, 64, gen_store_fpr64(ctx, fp0, fd))
FOP_CONDNS(s, FMT_S, 32, gen_store_fpr32(ctx, fp0, fd))
#undef FOP_CONDNS
#undef gen_ldcmp_fpr32
#undef gen_ldcmp_fpr64

/* load/store instructions. */
#ifdef CONFIG_USER_ONLY
#define OP_LD_ATOMIC(insn, fname)                                          \
static inline void op_ld_##insn(TCGv ret, TCGv arg1, int mem_idx,          \
                                DisasContext *ctx)                         \
{                                                                          \
    TCGv t0 = tcg_temp_new();                                              \
    tcg_gen_mov_tl(t0, arg1);                                              \
    tcg_gen_qemu_##fname(ret, arg1, ctx->mem_idx);                         \
    tcg_gen_st_tl(t0, cpu_env, offsetof(CPUMIPSState, lladdr));            \
    tcg_gen_st_tl(ret, cpu_env, offsetof(CPUMIPSState, llval));            \
    tcg_temp_free(t0);                                                     \
}
#else
#define OP_LD_ATOMIC(insn, fname)                                          \
static inline void op_ld_##insn(TCGv ret, TCGv_cap_checked_ptr arg1,       \
                                int mem_idx, DisasContext *ctx) {          \
    gen_helper_1e1i(insn, ret, arg1, mem_idx);                             \
}
#endif
OP_LD_ATOMIC(ll, ld32s);
#if defined(TARGET_MIPS64)
OP_LD_ATOMIC(lld, ld64);
#endif
#undef OP_LD_ATOMIC

void gen_base_offset_addr(DisasContext *ctx, TCGv addr, int base, int offset)
{
    if (base == 0) {
        tcg_gen_movi_tl(addr, offset);
    } else if (offset == 0) {
        gen_load_gpr(addr, base);
    } else {
        tcg_gen_movi_tl(addr, offset);
        gen_op_addr_add(ctx, addr, cpu_gpr[base], addr);
    }
}

static target_ulong pc_relative_pc(DisasContext *ctx)
{
    target_ulong pc = ctx->base.pc_next;

    if (ctx->hflags & MIPS_HFLAG_BMASK) {
        int branch_bytes = ctx->hflags & MIPS_HFLAG_BDS16 ? 2 : 4;

        pc -= branch_bytes;
    }

    pc &= ~(target_ulong)3;
    return pc;
}

/* Load */
static void gen_ld(DisasContext *ctx, uint32_t opc,
                   int rt, int base, int offset)
{
    TCGv t0, t1, t2;
    int mem_idx = ctx->mem_idx;

    if (rt == 0 && ctx->insn_flags & (INSN_LOONGSON2E | INSN_LOONGSON2F |
                                      INSN_LOONGSON3A)) {
        /*
         * Loongson CPU uses a load to zero register for prefetch.
         * We emulate it as a NOP. On other CPU we must perform the
         * actual memory access.
         */
        return;
    }

    t0 = tcg_temp_new();
    gen_base_offset_addr(ctx, t0, base, offset);

#if defined(TARGET_CHERI) || defined(CONFIG_TCG_LOG_INSTR)
    TCGv_cap_checked_ptr ddc_interposed = tcg_temp_new_cap_checked();
#else
    TCGv_cap_checked_ptr ddc_interposed = t0;
#endif /* defined(TARGET_CHERI) || defined(CONFIG_TCG_LOG_INSTR) */

    switch (opc) {
#if defined(TARGET_MIPS64)
    case OPC_LWU:
        gen_ddc_interposed_ld_tl(ctx, t0, ddc_interposed, t0, mem_idx,
                                 MO_TEUL | ctx->default_tcg_memop_mask);
        gen_store_gpr(t0, rt);
        break;
    case OPC_LD:
        gen_ddc_interposed_ld_tl(ctx, t0, ddc_interposed, t0, mem_idx,
                                 MO_TEQ | ctx->default_tcg_memop_mask);
        gen_store_gpr(t0, rt);
        break;
    case OPC_LLD:
    case R6_OPC_LLD:
        generate_ddc_checked_load_ptr(ddc_interposed, ctx, t0, 8);
        op_ld_lld(t0, ddc_interposed, mem_idx, ctx);
        gen_store_gpr(t0, rt);
        break;
    case OPC_LDL:
        generate_ddc_checked_load_ptr(ddc_interposed, ctx, t0, 8);
        t1 = tcg_temp_new();
        /*
         * Do a byte access to possibly trigger a page
         * fault with the unaligned address.
         */
        tcg_gen_qemu_ld_tl_with_checked_addr(t1, ddc_interposed, mem_idx, MO_UB);
        tcg_gen_andi_tl(t1, (TCGv)ddc_interposed, 7);
#ifndef TARGET_WORDS_BIGENDIAN
        tcg_gen_xori_tl(t1, t1, 7);
#endif
        tcg_gen_shli_tl(t1, t1, 3);
        tcg_gen_andi_tl((TCGv)ddc_interposed, (TCGv)ddc_interposed, ~7);
        tcg_gen_qemu_ld_tl_with_checked_addr(t0, ddc_interposed, mem_idx, MO_TEQ);
        tcg_gen_shl_tl(t0, t0, t1);
        t2 = tcg_const_tl(-1);
        tcg_gen_shl_tl(t2, t2, t1);
        gen_load_gpr(t1, rt);
        tcg_gen_andc_tl(t1, t1, t2);
        tcg_temp_free(t2);
        tcg_gen_or_tl(t0, t0, t1);
        tcg_temp_free(t1);
        gen_store_gpr(t0, rt);
        break;
    case OPC_LDR:
        generate_ccheck_load_right(ddc_interposed, t0, 8);
        t1 = tcg_temp_new();
        /*
         * Do a byte access to possibly trigger a page
         * fault with the unaligned address.
         */
        tcg_gen_qemu_ld_tl_with_checked_addr(t1, ddc_interposed, mem_idx, MO_UB);
        tcg_gen_andi_tl(t1, (TCGv)ddc_interposed, 7);
#ifdef TARGET_WORDS_BIGENDIAN
        tcg_gen_xori_tl(t1, t1, 7);
#endif
        tcg_gen_shli_tl(t1, t1, 3);
        tcg_gen_andi_tl((TCGv)ddc_interposed, (TCGv)ddc_interposed, ~7);
        tcg_gen_qemu_ld_tl_with_checked_addr(t0, ddc_interposed, mem_idx, MO_TEQ);
        tcg_gen_shr_tl(t0, t0, t1);
        tcg_gen_xori_tl(t1, t1, 63);
        t2 = tcg_const_tl(0xfffffffffffffffeull);
        tcg_gen_shl_tl(t2, t2, t1);
        gen_load_gpr(t1, rt);
        tcg_gen_and_tl(t1, t1, t2);
        tcg_temp_free(t2);
        tcg_gen_or_tl(t0, t0, t1);
        tcg_temp_free(t1);
        gen_store_gpr(t0, rt);
        break;
    case OPC_LDPC:
        t1 = tcg_const_tl(pc_relative_pc(ctx));
        gen_op_addr_add(ctx, t0, t0, t1);
        tcg_temp_free(t1);
        generate_ccheck_load_pcrel(t0, 8);
        tcg_gen_qemu_ld_tl_with_checked_addr(t0, PCC_CHECKED(t0), mem_idx, MO_TEQ);
        gen_store_gpr(t0, rt);
        break;
#endif
    case OPC_LWPC:
        t1 = tcg_const_tl(pc_relative_pc(ctx));
        gen_op_addr_add(ctx, t0, t0, t1);
        generate_ccheck_load_pcrel(t0, 4);
        tcg_gen_mov_tl(t1, t0);
        tcg_gen_qemu_ld_tl_with_checked_addr(t0, PCC_CHECKED(t0), mem_idx, MO_TESL);
        tcg_temp_free(t1);
        gen_store_gpr(t0, rt);
        break;
    case OPC_LWE:
        mem_idx = MIPS_HFLAG_UM;
        /* fall through */
    case OPC_LW:
        gen_ddc_interposed_ld_tl(ctx, t0, ddc_interposed, t0, mem_idx,
                                 MO_TESL | ctx->default_tcg_memop_mask);
        gen_store_gpr(t0, rt);
        break;
    case OPC_LHE:
        mem_idx = MIPS_HFLAG_UM;
        /* fall through */
    case OPC_LH:
        gen_ddc_interposed_ld_tl(ctx, t0, ddc_interposed, t0, mem_idx,
                                 MO_TESW | ctx->default_tcg_memop_mask);
        gen_store_gpr(t0, rt);
        break;
    case OPC_LHUE:
        mem_idx = MIPS_HFLAG_UM;
        /* fall through */
    case OPC_LHU:
        gen_ddc_interposed_ld_tl(ctx, t0, ddc_interposed, t0, mem_idx,
                                 MO_TEUW | ctx->default_tcg_memop_mask);
        gen_store_gpr(t0, rt);
        break;
    case OPC_LBE:
        mem_idx = MIPS_HFLAG_UM;
        /* fall through */
    case OPC_LB:
        gen_ddc_interposed_ld_tl(ctx, t0, ddc_interposed, t0, mem_idx, MO_SB);
        gen_store_gpr(t0, rt);
        break;
    case OPC_LBUE:
        mem_idx = MIPS_HFLAG_UM;
        /* fall through */
    case OPC_LBU:
        gen_ddc_interposed_ld_tl(ctx, t0, ddc_interposed, t0, mem_idx, MO_UB);
        gen_store_gpr(t0, rt);
        break;
    case OPC_LWLE:
        mem_idx = MIPS_HFLAG_UM;
        /* fall through */
    case OPC_LWL:
        t1 = tcg_temp_new();
        generate_ddc_checked_load_ptr(ddc_interposed, ctx, t0, 4);
        /*
         * Do a byte access to possibly trigger a page
         * fault with the unaligned address.
         */
        tcg_gen_qemu_ld_tl_with_checked_addr(t1, ddc_interposed, mem_idx, MO_UB);
        tcg_gen_andi_tl(t1, (TCGv)ddc_interposed, 3);
#ifndef TARGET_WORDS_BIGENDIAN
        tcg_gen_xori_tl(t1, t1, 3);
#endif
        tcg_gen_shli_tl(t1, t1, 3);
        tcg_gen_andi_tl((TCGv)ddc_interposed, (TCGv)ddc_interposed, ~3);
        tcg_gen_qemu_ld_tl_with_checked_addr(t0, ddc_interposed, mem_idx, MO_TEUL);
        tcg_gen_shl_tl(t0, t0, t1);
        t2 = tcg_const_tl(-1);
        tcg_gen_shl_tl(t2, t2, t1);
        gen_load_gpr(t1, rt);
        tcg_gen_andc_tl(t1, t1, t2);
        tcg_temp_free(t2);
        tcg_gen_or_tl(t0, t0, t1);
        tcg_temp_free(t1);
        tcg_gen_ext32s_tl(t0, t0);
        gen_store_gpr(t0, rt);
        break;
    case OPC_LWRE:
        mem_idx = MIPS_HFLAG_UM;
        /* fall through */
    case OPC_LWR:
        generate_ccheck_load_right(ddc_interposed, t0, 4);
        t1 = tcg_temp_new();
        /*
         * Do a byte access to possibly trigger a page
         * fault with the unaligned address.
         */
        tcg_gen_qemu_ld_tl_with_checked_addr(t1, ddc_interposed, mem_idx, MO_UB);
        tcg_gen_andi_tl(t1, (TCGv)ddc_interposed, 3);
#ifdef TARGET_WORDS_BIGENDIAN
        tcg_gen_xori_tl(t1, t1, 3);
#endif
        tcg_gen_shli_tl(t1, t1, 3);
        tcg_gen_andi_tl((TCGv)ddc_interposed, (TCGv)ddc_interposed, ~3);
        tcg_gen_qemu_ld_tl_with_checked_addr(t0, ddc_interposed, mem_idx, MO_TEUL);
        tcg_gen_shr_tl(t0, t0, t1);
        tcg_gen_xori_tl(t1, t1, 31);
        t2 = tcg_const_tl(0xfffffffeull);
        tcg_gen_shl_tl(t2, t2, t1);
        gen_load_gpr(t1, rt);
        tcg_gen_and_tl(t1, t1, t2);
        tcg_temp_free(t2);
        tcg_gen_or_tl(t0, t0, t1);
        tcg_temp_free(t1);
        tcg_gen_ext32s_tl(t0, t0);
        gen_store_gpr(t0, rt);
        break;
    case OPC_LLE:
        mem_idx = MIPS_HFLAG_UM;
        /* fall through */
    case OPC_LL:
    case R6_OPC_LL:
        generate_ddc_checked_load_ptr(ddc_interposed, ctx, t0, 4);
        op_ld_ll(t0, ddc_interposed, mem_idx, ctx);
        gen_store_gpr(t0, rt);
        break;
    }
    tcg_temp_free(t0);
#if defined(TARGET_CHERI) || defined(CONFIG_TCG_LOG_INSTR)
    tcg_temp_free_cap_checked(ddc_interposed);
#endif /* defined(TARGET_CHERI) || defined(CONFIG_TCG_LOG_INSTR) */
}

#ifndef TARGET_CHERI
static void gen_llwp(DisasContext *ctx, uint32_t base, int16_t offset,
                    uint32_t reg1, uint32_t reg2)
{
    TCGv taddr = tcg_temp_new();
    TCGv_i64 tval = tcg_temp_new_i64();
    TCGv tmp1 = tcg_temp_new();
    TCGv tmp2 = tcg_temp_new();

    gen_base_offset_addr(ctx, taddr, base, offset);
    tcg_gen_qemu_ld64(tval, taddr, ctx->mem_idx);
#ifdef TARGET_WORDS_BIGENDIAN
    tcg_gen_extr_i64_tl(tmp2, tmp1, tval);
#else
    tcg_gen_extr_i64_tl(tmp1, tmp2, tval);
#endif
    gen_store_gpr(tmp1, reg1);
    tcg_temp_free(tmp1);
    gen_store_gpr(tmp2, reg2);
    tcg_temp_free(tmp2);
    tcg_gen_st_i64(tval, cpu_env, offsetof(CPUMIPSState, llval_wp));
    tcg_temp_free_i64(tval);
    tcg_gen_st_tl(taddr, cpu_env, offsetof(CPUMIPSState, lladdr));
    tcg_temp_free(taddr);
}
#endif

/* Store */
static void gen_st(DisasContext *ctx, uint32_t opc, int rt,
                   int base, int offset)
{
    TCGv t0 = tcg_temp_new();
    TCGv t1 = tcg_temp_new();
    int mem_idx = ctx->mem_idx;

    gen_base_offset_addr(ctx, t0, base, offset);
    gen_load_gpr(t1, rt);
    switch (opc) {
#if defined(TARGET_MIPS64)
    case OPC_SD:
        gen_ddc_interposed_st_tl(ctx, t1, NULL/* add $ddc to t0*/, t0, mem_idx,
                                 MO_TEQ | ctx->default_tcg_memop_mask);
        break;
    case OPC_SDL:
        gen_helper_0e2i(sdl, t1, t0, mem_idx);
        break;
    case OPC_SDR:
        gen_helper_0e2i(sdr, t1, t0, mem_idx);
        break;
#endif
    case OPC_SWE:
        mem_idx = MIPS_HFLAG_UM;
        /* fall through */
    case OPC_SW:
        gen_ddc_interposed_st_tl(ctx, t1, NULL/* add $ddc to t0*/, t0, mem_idx,
                                 MO_TEUL | ctx->default_tcg_memop_mask);
        break;
    case OPC_SHE:
        mem_idx = MIPS_HFLAG_UM;
        /* fall through */
    case OPC_SH:
        gen_ddc_interposed_st_tl(ctx, t1, NULL/* add $ddc to t0*/, t0, mem_idx,
                                 MO_TEUW | ctx->default_tcg_memop_mask);
        break;
    case OPC_SBE:
        mem_idx = MIPS_HFLAG_UM;
        /* fall through */
    case OPC_SB:
        gen_ddc_interposed_st_tl(ctx, t1, NULL/* add $ddc to t0*/, t0, mem_idx, MO_8);
        break;
    case OPC_SWLE:
        mem_idx = MIPS_HFLAG_UM;
        /* fall through */
    case OPC_SWL:
        gen_helper_0e2i(swl, t1, t0, mem_idx);
        break;
    case OPC_SWRE:
        mem_idx = MIPS_HFLAG_UM;
        /* fall through */
    case OPC_SWR:
        gen_helper_0e2i(swr, t1, t0, mem_idx);
        break;
    }
    tcg_temp_free(t0);
    tcg_temp_free(t1);
}


/* Store conditional */
static void gen_st_cond(DisasContext *ctx, int rt, int base, int offset,
                        MemOp tcg_mo, bool eva, int opc)
{
    TCGv t0, val;
    TCGv_cap_checked_ptr addr;
    TCGLabel *l1 = gen_new_label();
    TCGLabel *done = gen_new_label();
    TCGLabel *not_misaligned = gen_new_label();

    t0 = tcg_temp_new();
    // IMPORTANT! addr must be allocated using tcg_temp_local_new since we
    // reference it in two different basic blocks.
    // The naming makes no sense... But is documented in tcg/README.
    // I should read the documentation before debugging :(
    addr = tcg_temp_local_new_cap_checked();
    gen_base_offset_addr(ctx, t0, base, offset);
    /* We have to add $ddc to the address for the alignment check and the memory access */
    generate_ddc_checked_store_ptr(addr, ctx, t0, memop_size(tcg_mo));
    /*
     * Alignment must be checked even if the CPU supports unaligned accesses:
     *
     * The effective address must be naturally-aligned. If either of the 2/3
     * least-significant bits of the address is non-zero, an Address Error
     * exception occurs.
     */
    tcg_gen_andi_tl(t0, (TCGv)addr, memop_size(tcg_mo) - 1);
    tcg_gen_brcondi_tl(TCG_COND_EQ, t0, 0x0, not_misaligned);
    generate_exception(ctx, EXCP_AdES);
    gen_set_label(not_misaligned);

    //    DEBUG_VALUE((TCGv)addr);
    //    DEBUG_VALUE(cpu_lladdr);
    /* compare the address against that of the preceeding LL */
    tcg_gen_brcond_tl(TCG_COND_EQ, (TCGv)addr, cpu_lladdr, l1);
    tcg_gen_movi_tl(t0, 0);
    gen_store_gpr(t0, rt);
    tcg_gen_br(done);

    gen_set_label(l1);
    /* generate cmpxchg */
    val = tcg_temp_local_new(); // local_new since used in another BB
    gen_load_gpr(val, rt);

    //    DEBUG_VALUE((TCGv)addr);
    //    DEBUG_VALUE(cpu_llval);
    //    DEBUG_VALUE(val);
    tcg_gen_atomic_cmpxchg_tl_with_checked_addr(
        t0, addr, cpu_llval, val, eva ? MIPS_HFLAG_UM : ctx->mem_idx, tcg_mo);

    //    DEBUG_VALUE(t0);
    tcg_gen_setcond_tl(TCG_COND_EQ, t0, t0, cpu_llval);
    //    DEBUG_VALUE(t0);
    gen_store_gpr(t0, rt);
    tcg_temp_free(val);

    gen_set_label(done);
    tcg_temp_free_cap_checked(addr);
    tcg_temp_free(t0);
}

#ifndef TARGET_CHERI
static void gen_scwp(DisasContext *ctx, uint32_t base, int16_t offset,
                    uint32_t reg1, uint32_t reg2, bool eva)
{
    TCGv taddr = tcg_temp_local_new();
    TCGv lladdr = tcg_temp_local_new();
    TCGv_i64 tval = tcg_temp_new_i64();
    TCGv_i64 llval = tcg_temp_new_i64();
    TCGv_i64 val = tcg_temp_new_i64();
    TCGv tmp1 = tcg_temp_new();
    TCGv tmp2 = tcg_temp_new();
    TCGLabel *lab_fail = gen_new_label();
    TCGLabel *lab_done = gen_new_label();

    gen_base_offset_addr(ctx, taddr, base, offset);

    tcg_gen_ld_tl(lladdr, cpu_env, offsetof(CPUMIPSState, lladdr));
    tcg_gen_brcond_tl(TCG_COND_NE, taddr, lladdr, lab_fail);

    gen_load_gpr(tmp1, reg1);
    gen_load_gpr(tmp2, reg2);

#ifdef TARGET_WORDS_BIGENDIAN
    tcg_gen_concat_tl_i64(tval, tmp2, tmp1);
#else
    tcg_gen_concat_tl_i64(tval, tmp1, tmp2);
#endif

    tcg_gen_ld_i64(llval, cpu_env, offsetof(CPUMIPSState, llval_wp));
    tcg_gen_atomic_cmpxchg_i64(val, taddr, llval, tval,
                               eva ? MIPS_HFLAG_UM : ctx->mem_idx, MO_64);
    if (reg1 != 0) {
        tcg_gen_movi_tl(cpu_gpr[reg1], 1);
    }
    tcg_gen_brcond_i64(TCG_COND_EQ, val, llval, lab_done);

    gen_set_label(lab_fail);

    if (reg1 != 0) {
        tcg_gen_movi_tl(cpu_gpr[reg1], 0);
    }
    gen_set_label(lab_done);
    tcg_gen_movi_tl(lladdr, -1);
    tcg_gen_st_tl(lladdr, cpu_env, offsetof(CPUMIPSState, lladdr));
}
#endif

/* Load and store */
static void gen_flt_ldst(DisasContext *ctx, uint32_t opc, int ft,
                         TCGv t0)
{
#if defined(TARGET_CHERI) || defined(CONFIG_TCG_LOG_INSTR)
    TCGv t1 = tcg_temp_new();
#endif /* defined(TARGET_CHERI) || defined(CONFIG_TCG_LOG_INSTR) */
    /*
     * Don't do NOP if destination is zero: we must perform the actual
     * memory access.
     */
    switch (opc) {
    case OPC_LWC1:
        {
            TCGv_i32 fp0 = tcg_temp_new_i32();
            gen_ddc_interposed_ld_i32(ctx, fp0, NULL/* add $ddc to t0*/, t0, ctx->mem_idx,
                                      MO_TESL | ctx->default_tcg_memop_mask);
            gen_store_fpr32(ctx, fp0, ft);
            tcg_temp_free_i32(fp0);
        }
        break;
    case OPC_SWC1:
        {
            TCGv_i32 fp0 = tcg_temp_new_i32();
            gen_load_fpr32(ctx, fp0, ft);
            gen_ddc_interposed_st_i32(ctx, fp0, NULL/* add $ddc to t0*/, t0, ctx->mem_idx,
                                      MO_TEUL | ctx->default_tcg_memop_mask);
            tcg_temp_free_i32(fp0);
        }
        break;
    case OPC_LDC1:
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();
            gen_ddc_interposed_ld_i64(ctx, fp0, NULL/* add $ddc to t0*/, t0, ctx->mem_idx, MO_TEQ |
                                      ctx->default_tcg_memop_mask);
            gen_store_fpr64(ctx, fp0, ft);
            tcg_temp_free_i64(fp0);
        }
        break;
    case OPC_SDC1:
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();
            gen_load_fpr64(ctx, fp0, ft);
            gen_ddc_interposed_st_i64(ctx, fp0, NULL/* add $ddc to t0*/, t0, ctx->mem_idx, MO_TEQ |
                                      ctx->default_tcg_memop_mask);
            tcg_temp_free_i64(fp0);
        }
        break;
    default:
        MIPS_INVAL("flt_ldst");
        gen_reserved_instruction(ctx);
        break;
    }
#if defined(TARGET_CHERI) || defined(CONFIG_TCG_LOG_INSTR)
    tcg_temp_free(t1);
#endif /* defined(TARGET_CHERI) || defined(CONFIG_TCG_LOG_INSTR) */
}

static void gen_cop1_ldst(DisasContext *ctx, uint32_t op, int rt,
                          int rs, int16_t imm)
{
    TCGv t0 = tcg_temp_new();

    if (ctx->CP0_Config1 & (1 << CP0C1_FP)) {
        check_cp1_enabled(ctx);
        switch (op) {
        case OPC_LDC1:
        case OPC_SDC1:
            check_insn(ctx, ISA_MIPS2);
            /* Fallthrough */
        default:
            gen_base_offset_addr(ctx, t0, rs, imm);
            gen_flt_ldst(ctx, op, rt, t0);
        }
    } else {
        generate_exception_err(ctx, EXCP_CpU, 1);
    }
    tcg_temp_free(t0);
}

/* Arithmetic with immediate operand */
static void gen_arith_imm(DisasContext *ctx, uint32_t opc,
                          int rt, int rs, int imm)
{
    target_ulong uimm = (target_long)imm; /* Sign extend to 32/64 bits */

    if (rt == 0 && opc != OPC_ADDI && opc != OPC_DADDI) {
        /*
         * If no destination, treat it as a NOP.
         * For addi, we must generate the overflow exception when needed.
         */
        return;
    }
    switch (opc) {
    case OPC_ADDI:
        {
            TCGv t0 = tcg_temp_local_new();
            TCGv t1 = tcg_temp_new();
            TCGv t2 = tcg_temp_new();
            TCGLabel *l1 = gen_new_label();

            gen_load_gpr(t1, rs);
            tcg_gen_addi_tl(t0, t1, uimm);
            tcg_gen_ext32s_tl(t0, t0);

            tcg_gen_xori_tl(t1, t1, ~uimm);
            tcg_gen_xori_tl(t2, t0, uimm);
            tcg_gen_and_tl(t1, t1, t2);
            tcg_temp_free(t2);
            tcg_gen_brcondi_tl(TCG_COND_GE, t1, 0, l1);
            tcg_temp_free(t1);
            /* operands of same sign, result different sign */
            generate_exception(ctx, EXCP_OVERFLOW);
            gen_set_label(l1);
            tcg_gen_ext32s_tl(t0, t0);
            gen_store_gpr(t0, rt);
            tcg_temp_free(t0);
        }
        break;
    case OPC_ADDIU:
        if (rs != 0) {
            tcg_gen_addi_tl(cpu_gpr[rt], cpu_gpr[rs], uimm);
            tcg_gen_ext32s_tl(cpu_gpr[rt], cpu_gpr[rt]);
        } else {
            tcg_gen_movi_tl(cpu_gpr[rt], uimm);
        }
        gen_log_instr_gpr_update(ctx, rt);
        break;
#if defined(TARGET_MIPS64)
    case OPC_DADDI:
        {
            TCGv t0 = tcg_temp_local_new();
            TCGv t1 = tcg_temp_new();
            TCGv t2 = tcg_temp_new();
            TCGLabel *l1 = gen_new_label();

            gen_load_gpr(t1, rs);
            tcg_gen_addi_tl(t0, t1, uimm);

            tcg_gen_xori_tl(t1, t1, ~uimm);
            tcg_gen_xori_tl(t2, t0, uimm);
            tcg_gen_and_tl(t1, t1, t2);
            tcg_temp_free(t2);
            tcg_gen_brcondi_tl(TCG_COND_GE, t1, 0, l1);
            tcg_temp_free(t1);
            /* operands of same sign, result different sign */
            generate_exception(ctx, EXCP_OVERFLOW);
            gen_set_label(l1);
            gen_store_gpr(t0, rt);
            tcg_temp_free(t0);
        }
        break;
    case OPC_DADDIU:
        if (rs != 0) {
            tcg_gen_addi_tl(cpu_gpr[rt], cpu_gpr[rs], uimm);
        } else {
            tcg_gen_movi_tl(cpu_gpr[rt], uimm);
        }
        gen_log_instr_gpr_update(ctx, rt);
        break;
#endif
    }
}

#define GEN_INSTR_TRACE_HELPER(env, name)                                      \
    {                                                                          \
        TCGv_i64 tpc = tcg_const_i64(ctx->base.pc_next);                       \
        gen_save_pc(ctx->base.pc_next);                                        \
        gen_helper_##name(env, tpc);                                           \
        tcg_temp_free_i64(tpc);                                                \
        /* Exit translation block since tracing flag may change */             \
        if (ctx->hflags & MIPS_HFLAG_BMASK) {                                  \
            warn_report("warning: magic trace helper "                         \
                "in delay / forbidden slot at PC 0x" TARGET_FMT_lx             \
                " may not work as expected\n", ctx->base.pc_next);             \
            return; /* We are already exiting the TB */                        \
        }                                                                      \
        gen_save_pc(ctx->base.pc_next + 4);                                    \
        ctx->base.is_jmp = DISAS_EXIT;                                         \
        return;                                                                \
    }

/* Logic with immediate operand */
static void gen_logic_imm(DisasContext *ctx, uint32_t opc,
                          int rt, int rs, int16_t imm)
{
    target_ulong uimm;

    if (rt == 0) {
        /* If no destination, treat it as a NOP. */
#ifdef CONFIG_TCG_LOG_INSTR
        if (opc == OPC_ORI && rs == 0) {

            /* With 'li $0, 0xbeef' turn on instruction trace logging. */
            if ((uint16_t)imm == 0xbeef)
                GEN_INSTR_TRACE_HELPER(cpu_env, qemu_log_instr_start);

            /* With 'li $0, 0xdead' turn off instruction trace logging. */
            if ((uint16_t)imm == 0xdead)
                GEN_INSTR_TRACE_HELPER(cpu_env, qemu_log_instr_stop);

            /* With 'li $0, 0xdeaf' switch to userspace-only instruction trace logging. */
            if ((uint16_t)imm == 0xdeaf)
                GEN_INSTR_TRACE_HELPER(cpu_env, qemu_log_instr_user_start);

            /* With 'li $0, 0xfaed' switch off userspace-only instruction trace logging. */
            if ((uint16_t)imm == 0xfaed)
                GEN_INSTR_TRACE_HELPER(cpu_env, qemu_log_instr_stop);

            if ((uint16_t)imm == 0xface)
                GEN_INSTR_TRACE_HELPER(cpu_env, cheri_debug_message);

            /* With 0xcode invoke QEMU helper functions such as fast memset, memcpy etc.
             * They are designed to take the same register arguments as the libc function:
             * Currently supported values are:
             * $v1 = 1 -> memset(ptr=$a0, c=$a1, len=$a2)
             * $v1 = 2 -> purecap memset/memset_c(ptr=$c3, c=$a0, len=$a1)
             * $v1 = 3 -> memcpy(dst=$a0, src=$a1, len=$a2)
             * $v1 = 4 -> purecap memcpy/memcpy_c(dst=$c3, src=$c4, len=$a0)
             * $v1 = 5 -> memmove(dst=$a0, src=$a1, len=$a2)
             * $v1 = 6 -> purecap memmmove/memmove_c(dst=$c3, src=$c4, len=$a0)
             * $v1 = 7 -> bcopy(src=$a0, dst=$a1, len=$a2)
             * TODO: strlen? str{l,n}cpy?
             */
            if ((uint16_t)imm == 0xC0DE) {
                save_cpu_state(ctx, 1);
                gen_helper_magic_library_function(cpu_env, cpu_gpr[3]);
            }

            /* With 'li $0, 0xea1d' perform smp yield. */
            if ((uint16_t)imm == 0xea1d) {
                gen_save_pc(ctx->base.pc_next + 4);
                gen_helper_smp_yield(cpu_env);
            }

            /* Buffered tracing switches, same as RISC-V */
            if ((uint16_t)imm == 0x01 || (uint16_t)imm == 0x02) {
                TCGv_i32 ttmp = tcg_const_i32(((uint16_t)imm == 0x01));
                gen_helper_qemu_log_instr_buffered_mode(cpu_env, ttmp);
                tcg_temp_free_i32(ttmp);
            }
            if ((uint16_t)imm == 0x03)
                gen_helper_qemu_log_instr_buffer_flush(cpu_env);

        }
#endif /* CONFIG_TCG_LOG_INSTR */
        return;
    }
    uimm = (uint16_t)imm;
    switch (opc) {
    case OPC_ANDI:
        if (likely(rs != 0)) {
            tcg_gen_andi_tl(cpu_gpr[rt], cpu_gpr[rs], uimm);
        } else {
            tcg_gen_movi_tl(cpu_gpr[rt], 0);
        }
        gen_log_instr_gpr_update(ctx, rt);
        break;
    case OPC_ORI:
        if (rs != 0) {
            tcg_gen_ori_tl(cpu_gpr[rt], cpu_gpr[rs], uimm);
        } else {
            tcg_gen_movi_tl(cpu_gpr[rt], uimm);
        }
        gen_log_instr_gpr_update(ctx, rt);
        break;
    case OPC_XORI:
        if (likely(rs != 0)) {
            tcg_gen_xori_tl(cpu_gpr[rt], cpu_gpr[rs], uimm);
        } else {
            tcg_gen_movi_tl(cpu_gpr[rt], uimm);
        }
        gen_log_instr_gpr_update(ctx, rt);
        break;
    case OPC_LUI:
        if (rs != 0 && (ctx->insn_flags & ISA_MIPS_R6)) {
            /* OPC_AUI */
            tcg_gen_addi_tl(cpu_gpr[rt], cpu_gpr[rs], imm << 16);
            tcg_gen_ext32s_tl(cpu_gpr[rt], cpu_gpr[rt]);
        } else {
            tcg_gen_movi_tl(cpu_gpr[rt], imm << 16);
        }
        gen_log_instr_gpr_update(ctx, rt);
        break;

    default:
        break;
    }
}

/* Set on less than with immediate operand */
static void gen_slt_imm(DisasContext *ctx, uint32_t opc,
                        int rt, int rs, int16_t imm)
{
    target_ulong uimm = (target_long)imm; /* Sign extend to 32/64 bits */
    TCGv t0;

    if (rt == 0) {
        /* If no destination, treat it as a NOP. */
        return;
    }
    t0 = tcg_temp_new();
    gen_load_gpr(t0, rs);
    switch (opc) {
    case OPC_SLTI:
        tcg_gen_setcondi_tl(TCG_COND_LT, cpu_gpr[rt], t0, uimm);
        break;
    case OPC_SLTIU:
        tcg_gen_setcondi_tl(TCG_COND_LTU, cpu_gpr[rt], t0, uimm);
        break;
    }
    gen_log_instr_gpr_update(ctx, rt);
    tcg_temp_free(t0);
}

/* Shifts with immediate operand */
static void gen_shift_imm(DisasContext *ctx, uint32_t opc,
                          int rt, int rs, int16_t imm)
{
    target_ulong uimm = ((uint16_t)imm) & 0x1f;
    TCGv t0;

    if (rt == 0) {
        /* If no destination, treat it as a NOP. */
        return;
    }

    t0 = tcg_temp_new();
    gen_load_gpr(t0, rs);
    switch (opc) {
    case OPC_SLL:
        tcg_gen_shli_tl(t0, t0, uimm);
        tcg_gen_ext32s_tl(cpu_gpr[rt], t0);
        break;
    case OPC_SRA:
        tcg_gen_sari_tl(cpu_gpr[rt], t0, uimm);
        break;
    case OPC_SRL:
        if (uimm != 0) {
            tcg_gen_ext32u_tl(t0, t0);
            tcg_gen_shri_tl(cpu_gpr[rt], t0, uimm);
        } else {
            tcg_gen_ext32s_tl(cpu_gpr[rt], t0);
        }
        break;
    case OPC_ROTR:
        if (uimm != 0) {
            TCGv_i32 t1 = tcg_temp_new_i32();

            tcg_gen_trunc_tl_i32(t1, t0);
            tcg_gen_rotri_i32(t1, t1, uimm);
            tcg_gen_ext_i32_tl(cpu_gpr[rt], t1);
            tcg_temp_free_i32(t1);
        } else {
            tcg_gen_ext32s_tl(cpu_gpr[rt], t0);
        }
        break;
#if defined(TARGET_MIPS64)
    case OPC_DSLL:
        tcg_gen_shli_tl(cpu_gpr[rt], t0, uimm);
        break;
    case OPC_DSRA:
        tcg_gen_sari_tl(cpu_gpr[rt], t0, uimm);
        break;
    case OPC_DSRL:
        tcg_gen_shri_tl(cpu_gpr[rt], t0, uimm);
        break;
    case OPC_DROTR:
        if (uimm != 0) {
            tcg_gen_rotri_tl(cpu_gpr[rt], t0, uimm);
        } else {
            tcg_gen_mov_tl(cpu_gpr[rt], t0);
        }
        break;
    case OPC_DSLL32:
        tcg_gen_shli_tl(cpu_gpr[rt], t0, uimm + 32);
        break;
    case OPC_DSRA32:
        tcg_gen_sari_tl(cpu_gpr[rt], t0, uimm + 32);
        break;
    case OPC_DSRL32:
        tcg_gen_shri_tl(cpu_gpr[rt], t0, uimm + 32);
        break;
    case OPC_DROTR32:
        tcg_gen_rotri_tl(cpu_gpr[rt], t0, uimm + 32);
        break;
#endif
    }
    gen_log_instr_gpr_update(ctx, rt);
    tcg_temp_free(t0);
}

/* Arithmetic */
static void gen_arith(DisasContext *ctx, uint32_t opc,
                      int rd, int rs, int rt)
{
    if (rd == 0 && opc != OPC_ADD && opc != OPC_SUB
       && opc != OPC_DADD && opc != OPC_DSUB) {
        /*
         * If no destination, treat it as a NOP.
         * For add & sub, we must generate the overflow exception when needed.
         */
        return;
    }

    switch (opc) {
    case OPC_ADD:
        {
            TCGv t0 = tcg_temp_local_new();
            TCGv t1 = tcg_temp_new();
            TCGv t2 = tcg_temp_new();
            TCGLabel *l1 = gen_new_label();

            gen_load_gpr(t1, rs);
            gen_load_gpr(t2, rt);
            tcg_gen_add_tl(t0, t1, t2);
            tcg_gen_ext32s_tl(t0, t0);
            tcg_gen_xor_tl(t1, t1, t2);
            tcg_gen_xor_tl(t2, t0, t2);
            tcg_gen_andc_tl(t1, t2, t1);
            tcg_temp_free(t2);
            tcg_gen_brcondi_tl(TCG_COND_GE, t1, 0, l1);
            tcg_temp_free(t1);
            /* operands of same sign, result different sign */
            generate_exception(ctx, EXCP_OVERFLOW);
            gen_set_label(l1);
            gen_store_gpr(t0, rd);
            tcg_temp_free(t0);
        }
        break;
    case OPC_ADDU:
        if (rs != 0 && rt != 0) {
            tcg_gen_add_tl(cpu_gpr[rd], cpu_gpr[rs], cpu_gpr[rt]);
            tcg_gen_ext32s_tl(cpu_gpr[rd], cpu_gpr[rd]);
        } else if (rs == 0 && rt != 0) {
            tcg_gen_mov_tl(cpu_gpr[rd], cpu_gpr[rt]);
        } else if (rs != 0 && rt == 0) {
            tcg_gen_mov_tl(cpu_gpr[rd], cpu_gpr[rs]);
        } else {
            tcg_gen_movi_tl(cpu_gpr[rd], 0);
        }
        gen_log_instr_gpr_update(ctx, rd);
        break;
    case OPC_SUB:
        {
            TCGv t0 = tcg_temp_local_new();
            TCGv t1 = tcg_temp_new();
            TCGv t2 = tcg_temp_new();
            TCGLabel *l1 = gen_new_label();

            gen_load_gpr(t1, rs);
            gen_load_gpr(t2, rt);
            tcg_gen_sub_tl(t0, t1, t2);
            tcg_gen_ext32s_tl(t0, t0);
            tcg_gen_xor_tl(t2, t1, t2);
            tcg_gen_xor_tl(t1, t0, t1);
            tcg_gen_and_tl(t1, t1, t2);
            tcg_temp_free(t2);
            tcg_gen_brcondi_tl(TCG_COND_GE, t1, 0, l1);
            tcg_temp_free(t1);
            /*
             * operands of different sign, first operand and the result
             * of different sign
             */
            generate_exception(ctx, EXCP_OVERFLOW);
            gen_set_label(l1);
            gen_store_gpr(t0, rd);
            tcg_temp_free(t0);
        }
        break;
    case OPC_SUBU:
        if (rs != 0 && rt != 0) {
            tcg_gen_sub_tl(cpu_gpr[rd], cpu_gpr[rs], cpu_gpr[rt]);
            tcg_gen_ext32s_tl(cpu_gpr[rd], cpu_gpr[rd]);
        } else if (rs == 0 && rt != 0) {
            tcg_gen_neg_tl(cpu_gpr[rd], cpu_gpr[rt]);
            tcg_gen_ext32s_tl(cpu_gpr[rd], cpu_gpr[rd]);
        } else if (rs != 0 && rt == 0) {
            tcg_gen_mov_tl(cpu_gpr[rd], cpu_gpr[rs]);
        } else {
            tcg_gen_movi_tl(cpu_gpr[rd], 0);
        }
        gen_log_instr_gpr_update(ctx, rd);
        break;
#if defined(TARGET_MIPS64)
    case OPC_DADD:
        {
            TCGv t0 = tcg_temp_local_new();
            TCGv t1 = tcg_temp_new();
            TCGv t2 = tcg_temp_new();
            TCGLabel *l1 = gen_new_label();

            gen_load_gpr(t1, rs);
            gen_load_gpr(t2, rt);
            tcg_gen_add_tl(t0, t1, t2);
            tcg_gen_xor_tl(t1, t1, t2);
            tcg_gen_xor_tl(t2, t0, t2);
            tcg_gen_andc_tl(t1, t2, t1);
            tcg_temp_free(t2);
            tcg_gen_brcondi_tl(TCG_COND_GE, t1, 0, l1);
            tcg_temp_free(t1);
            /* operands of same sign, result different sign */
            generate_exception(ctx, EXCP_OVERFLOW);
            gen_set_label(l1);
            gen_store_gpr(t0, rd);
            tcg_temp_free(t0);
        }
        break;
    case OPC_DADDU:
        if (rs != 0 && rt != 0) {
            tcg_gen_add_tl(cpu_gpr[rd], cpu_gpr[rs], cpu_gpr[rt]);
        } else if (rs == 0 && rt != 0) {
            tcg_gen_mov_tl(cpu_gpr[rd], cpu_gpr[rt]);
        } else if (rs != 0 && rt == 0) {
            tcg_gen_mov_tl(cpu_gpr[rd], cpu_gpr[rs]);
        } else {
            tcg_gen_movi_tl(cpu_gpr[rd], 0);
        }
        gen_log_instr_gpr_update(ctx, rd);
        break;
    case OPC_DSUB:
        {
            TCGv t0 = tcg_temp_local_new();
            TCGv t1 = tcg_temp_new();
            TCGv t2 = tcg_temp_new();
            TCGLabel *l1 = gen_new_label();

            gen_load_gpr(t1, rs);
            gen_load_gpr(t2, rt);
            tcg_gen_sub_tl(t0, t1, t2);
            tcg_gen_xor_tl(t2, t1, t2);
            tcg_gen_xor_tl(t1, t0, t1);
            tcg_gen_and_tl(t1, t1, t2);
            tcg_temp_free(t2);
            tcg_gen_brcondi_tl(TCG_COND_GE, t1, 0, l1);
            tcg_temp_free(t1);
            /*
             * Operands of different sign, first operand and result different
             * sign.
             */
            generate_exception(ctx, EXCP_OVERFLOW);
            gen_set_label(l1);
            gen_store_gpr(t0, rd);
            tcg_temp_free(t0);
        }
        break;
    case OPC_DSUBU:
        if (rs != 0 && rt != 0) {
            tcg_gen_sub_tl(cpu_gpr[rd], cpu_gpr[rs], cpu_gpr[rt]);
        } else if (rs == 0 && rt != 0) {
            tcg_gen_neg_tl(cpu_gpr[rd], cpu_gpr[rt]);
        } else if (rs != 0 && rt == 0) {
            tcg_gen_mov_tl(cpu_gpr[rd], cpu_gpr[rs]);
        } else {
            tcg_gen_movi_tl(cpu_gpr[rd], 0);
        }
        gen_log_instr_gpr_update(ctx, rd);
        break;
#endif
    case OPC_MUL:
        if (likely(rs != 0 && rt != 0)) {
            tcg_gen_mul_tl(cpu_gpr[rd], cpu_gpr[rs], cpu_gpr[rt]);
            tcg_gen_ext32s_tl(cpu_gpr[rd], cpu_gpr[rd]);
        } else {
            tcg_gen_movi_tl(cpu_gpr[rd], 0);
        }
        gen_log_instr_gpr_update(ctx, rd);
        break;
    }
}

/* Conditional move */
static void gen_cond_move(DisasContext *ctx, uint32_t opc,
                          int rd, int rs, int rt)
{
    TCGv t0, t1, t2;

    if (rd == 0) {
        /* If no destination, treat it as a NOP. */
        return;
    }

    t0 = tcg_temp_new();
    gen_load_gpr(t0, rt);
    t1 = tcg_const_tl(0);
    t2 = tcg_temp_new();
    gen_load_gpr(t2, rs);
    switch (opc) {
    case OPC_MOVN:
        tcg_gen_movcond_tl(TCG_COND_NE, cpu_gpr[rd], t0, t1, t2, cpu_gpr[rd]);
        break;
    case OPC_MOVZ:
        tcg_gen_movcond_tl(TCG_COND_EQ, cpu_gpr[rd], t0, t1, t2, cpu_gpr[rd]);
        break;
    case OPC_SELNEZ:
        tcg_gen_movcond_tl(TCG_COND_NE, cpu_gpr[rd], t0, t1, t2, t1);
        break;
    case OPC_SELEQZ:
        tcg_gen_movcond_tl(TCG_COND_EQ, cpu_gpr[rd], t0, t1, t2, t1);
        break;
    }
    gen_log_instr_gpr_update(ctx, rd);
    tcg_temp_free(t2);
    tcg_temp_free(t1);
    tcg_temp_free(t0);
}

/* Logic */
static void gen_logic(DisasContext *ctx, uint32_t opc,
                      int rd, int rs, int rt)
{
    if (rd == 0) {
        /* If no destination, treat it as a NOP. */
        return;
    }

    switch (opc) {
    case OPC_AND:
        if (likely(rs != 0 && rt != 0)) {
            tcg_gen_and_tl(cpu_gpr[rd], cpu_gpr[rs], cpu_gpr[rt]);
        } else {
            tcg_gen_movi_tl(cpu_gpr[rd], 0);
        }
        break;
    case OPC_NOR:
        if (rs != 0 && rt != 0) {
            tcg_gen_nor_tl(cpu_gpr[rd], cpu_gpr[rs], cpu_gpr[rt]);
        } else if (rs == 0 && rt != 0) {
            tcg_gen_not_tl(cpu_gpr[rd], cpu_gpr[rt]);
        } else if (rs != 0 && rt == 0) {
            tcg_gen_not_tl(cpu_gpr[rd], cpu_gpr[rs]);
        } else {
            tcg_gen_movi_tl(cpu_gpr[rd], ~((target_ulong)0));
        }
        break;
    case OPC_OR:
        if (likely(rs != 0 && rt != 0)) {
            tcg_gen_or_tl(cpu_gpr[rd], cpu_gpr[rs], cpu_gpr[rt]);
        } else if (rs == 0 && rt != 0) {
            tcg_gen_mov_tl(cpu_gpr[rd], cpu_gpr[rt]);
        } else if (rs != 0 && rt == 0) {
            tcg_gen_mov_tl(cpu_gpr[rd], cpu_gpr[rs]);
        } else {
            tcg_gen_movi_tl(cpu_gpr[rd], 0);
        }
        break;
    case OPC_XOR:
        if (likely(rs != 0 && rt != 0)) {
            tcg_gen_xor_tl(cpu_gpr[rd], cpu_gpr[rs], cpu_gpr[rt]);
        } else if (rs == 0 && rt != 0) {
            tcg_gen_mov_tl(cpu_gpr[rd], cpu_gpr[rt]);
        } else if (rs != 0 && rt == 0) {
            tcg_gen_mov_tl(cpu_gpr[rd], cpu_gpr[rs]);
        } else {
            tcg_gen_movi_tl(cpu_gpr[rd], 0);
        }
        break;
    }
    gen_log_instr_gpr_update(ctx, rd);
}

/* Set on lower than */
static void gen_slt(DisasContext *ctx, uint32_t opc,
                    int rd, int rs, int rt)
{
    TCGv t0, t1;

    if (rd == 0) {
        /* If no destination, treat it as a NOP. */
        return;
    }

    t0 = tcg_temp_new();
    t1 = tcg_temp_new();
    gen_load_gpr(t0, rs);
    gen_load_gpr(t1, rt);
    switch (opc) {
    case OPC_SLT:
        tcg_gen_setcond_tl(TCG_COND_LT, cpu_gpr[rd], t0, t1);
        break;
    case OPC_SLTU:
        tcg_gen_setcond_tl(TCG_COND_LTU, cpu_gpr[rd], t0, t1);
        break;
    }
    gen_log_instr_gpr_update(ctx, rd);
    tcg_temp_free(t0);
    tcg_temp_free(t1);
}

/* Shifts */
static void gen_shift(DisasContext *ctx, uint32_t opc,
                      int rd, int rs, int rt)
{
    TCGv t0, t1;

    if (rd == 0) {
        /*
         * If no destination, treat it as a NOP.
         * For add & sub, we must generate the overflow exception when needed.
         */
        return;
    }

    t0 = tcg_temp_new();
    t1 = tcg_temp_new();
    gen_load_gpr(t0, rs);
    gen_load_gpr(t1, rt);
    switch (opc) {
    case OPC_SLLV:
        tcg_gen_andi_tl(t0, t0, 0x1f);
        tcg_gen_shl_tl(t0, t1, t0);
        tcg_gen_ext32s_tl(cpu_gpr[rd], t0);
        break;
    case OPC_SRAV:
        tcg_gen_andi_tl(t0, t0, 0x1f);
        tcg_gen_sar_tl(cpu_gpr[rd], t1, t0);
        break;
    case OPC_SRLV:
        tcg_gen_ext32u_tl(t1, t1);
        tcg_gen_andi_tl(t0, t0, 0x1f);
        tcg_gen_shr_tl(t0, t1, t0);
        tcg_gen_ext32s_tl(cpu_gpr[rd], t0);
        break;
    case OPC_ROTRV:
        {
            TCGv_i32 t2 = tcg_temp_new_i32();
            TCGv_i32 t3 = tcg_temp_new_i32();

            tcg_gen_trunc_tl_i32(t2, t0);
            tcg_gen_trunc_tl_i32(t3, t1);
            tcg_gen_andi_i32(t2, t2, 0x1f);
            tcg_gen_rotr_i32(t2, t3, t2);
            tcg_gen_ext_i32_tl(cpu_gpr[rd], t2);
            tcg_temp_free_i32(t2);
            tcg_temp_free_i32(t3);
        }
        break;
#if defined(TARGET_MIPS64)
    case OPC_DSLLV:
        tcg_gen_andi_tl(t0, t0, 0x3f);
        tcg_gen_shl_tl(cpu_gpr[rd], t1, t0);
        break;
    case OPC_DSRAV:
        tcg_gen_andi_tl(t0, t0, 0x3f);
        tcg_gen_sar_tl(cpu_gpr[rd], t1, t0);
        break;
    case OPC_DSRLV:
        tcg_gen_andi_tl(t0, t0, 0x3f);
        tcg_gen_shr_tl(cpu_gpr[rd], t1, t0);
        break;
    case OPC_DROTRV:
        tcg_gen_andi_tl(t0, t0, 0x3f);
        tcg_gen_rotr_tl(cpu_gpr[rd], t1, t0);
        break;
#endif
    }
    gen_log_instr_gpr_update(ctx, rd);
    tcg_temp_free(t0);
    tcg_temp_free(t1);
}

/* Arithmetic on HI/LO registers */
static void gen_HILO(DisasContext *ctx, uint32_t opc, int acc, int reg)
{
    if (reg == 0 && (opc == OPC_MFHI || opc == OPC_MFLO)) {
        /* Treat as NOP. */
        return;
    }

    if (acc != 0) {
        check_dsp(ctx);
    }

    switch (opc) {
    case OPC_MFHI:
#if defined(TARGET_MIPS64)
        if (acc != 0) {
            tcg_gen_ext32s_tl(cpu_gpr[reg], cpu_HI[acc]);
        } else
#endif
        {
            tcg_gen_mov_tl(cpu_gpr[reg], cpu_HI[acc]);
        }
        gen_log_instr_gpr_update(ctx, reg);
        break;
    case OPC_MFLO:
#if defined(TARGET_MIPS64)
        if (acc != 0) {
            tcg_gen_ext32s_tl(cpu_gpr[reg], cpu_LO[acc]);
        } else
#endif
        {
            tcg_gen_mov_tl(cpu_gpr[reg], cpu_LO[acc]);
        }
        gen_log_instr_gpr_update(ctx, reg);
        break;
    case OPC_MTHI:
        if (reg != 0) {
#if defined(TARGET_MIPS64)
            if (acc != 0) {
                tcg_gen_ext32s_tl(cpu_HI[acc], cpu_gpr[reg]);
            } else
#endif
            {
                tcg_gen_mov_tl(cpu_HI[acc], cpu_gpr[reg]);
            }
        } else {
            tcg_gen_movi_tl(cpu_HI[acc], 0);
        }
        gen_log_instr_hilo_update(ctx, /*hiLO*/0, acc, reg);
        break;
    case OPC_MTLO:
        if (reg != 0) {
#if defined(TARGET_MIPS64)
            if (acc != 0) {
                tcg_gen_ext32s_tl(cpu_LO[acc], cpu_gpr[reg]);
            } else
#endif
            {
                tcg_gen_mov_tl(cpu_LO[acc], cpu_gpr[reg]);
            }
        } else {
            tcg_gen_movi_tl(cpu_LO[acc], 0);
        }
        gen_log_instr_hilo_update(ctx, /*hiLO*/1, acc, reg);
        break;
    }
}

static inline void gen_r6_pcrel_ld(DisasContext *ctx, target_long addr, int reg,
                                   int memidx, MemOp memop)
{
    TCGv t0 = tcg_const_tl(addr);
    TCGv tval = tcg_temp_new();
    generate_ccheck_load_pcrel(t0, memop_size(memop));
    tcg_gen_qemu_ld_tl_with_checked_addr(tval, PCC_CHECKED(t0), memidx, memop);
    gen_store_gpr(tval, reg);
    tcg_temp_free(tval);
    tcg_temp_free(t0);
}

static inline void gen_pcrel(DisasContext *ctx, int opc, target_ulong pc,
                             int rs)
{
    target_long offset;
    target_long addr;

    switch (MASK_OPC_PCREL_TOP2BITS(opc)) {
    case OPC_ADDIUPC:
        if (rs != 0) {
            offset = sextract32(ctx->opcode << 2, 0, 21);
            /* For CHERI the result is an offset relative to PC. */
            addr = addr_add(ctx, pc - pcc_reloc(ctx), offset);
            tcg_gen_movi_tl(cpu_gpr[rs], addr);
            gen_log_instr_gpr_update(ctx, rs);
        }
        break;
    case R6_OPC_LWPC:
        offset = sextract32(ctx->opcode << 2, 0, 21);
        addr = addr_add(ctx, pc, offset);
        // For CHERI we check that the absolute PC address is within PCC
        gen_r6_pcrel_ld(ctx, addr, rs, ctx->mem_idx, MO_TESL);
        break;
#if defined(TARGET_MIPS64)
    case OPC_LWUPC:
        check_mips_64(ctx);
        offset = sextract32(ctx->opcode << 2, 0, 21);
        addr = addr_add(ctx, pc, offset);
        // For CHERI we check that the absolute PC address is within PCC
        gen_r6_pcrel_ld(ctx, addr, rs, ctx->mem_idx, MO_TEUL);
        break;
#endif
    default:
        switch (MASK_OPC_PCREL_TOP5BITS(opc)) {
        case OPC_AUIPC:
            if (rs != 0) {
                offset = sextract32(ctx->opcode, 0, 16) << 16;
                /* For CHERI the result is an offset relative to PC. */
                addr = addr_add(ctx, pc - pcc_reloc(ctx), offset);
                tcg_gen_movi_tl(cpu_gpr[rs], addr);
                gen_log_instr_gpr_update(ctx, rs);
            }
            break;
        case OPC_ALUIPC:
            if (rs != 0) {
                offset = sextract32(ctx->opcode, 0, 16) << 16;
                /* For CHERI the result is an offset relative to PC. */
                addr = ~0xFFFF & addr_add(ctx, pc - pcc_reloc(ctx), offset);
                tcg_gen_movi_tl(cpu_gpr[rs], addr);
                gen_log_instr_gpr_update(ctx, rs);
            }
            break;
#if defined(TARGET_MIPS64)
        case R6_OPC_LDPC: /* bits 16 and 17 are part of immediate */
        case R6_OPC_LDPC + (1 << 16):
        case R6_OPC_LDPC + (2 << 16):
        case R6_OPC_LDPC + (3 << 16):
            check_mips_64(ctx);
            offset = sextract32(ctx->opcode << 3, 0, 21);
            addr = addr_add(ctx, (pc & ~0x7), offset);
            // For CHERI we check that the absolute PC address is within PCC
            gen_r6_pcrel_ld(ctx, addr, rs, ctx->mem_idx, MO_TEQ);
            break;
#endif
        default:
            MIPS_INVAL("OPC_PCREL");
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    }
}

static void gen_r6_muldiv(DisasContext *ctx, int opc, int rd, int rs, int rt)
{
    TCGv t0, t1;

    if (rd == 0) {
        /* Treat as NOP. */
        return;
    }

    t0 = tcg_temp_new();
    t1 = tcg_temp_new();

    gen_load_gpr(t0, rs);
    gen_load_gpr(t1, rt);

    switch (opc) {
    case R6_OPC_DIV:
        {
            TCGv t2 = tcg_temp_new();
            TCGv t3 = tcg_temp_new();
            tcg_gen_ext32s_tl(t0, t0);
            tcg_gen_ext32s_tl(t1, t1);
            tcg_gen_setcondi_tl(TCG_COND_EQ, t2, t0, INT_MIN);
            tcg_gen_setcondi_tl(TCG_COND_EQ, t3, t1, -1);
            tcg_gen_and_tl(t2, t2, t3);
            tcg_gen_setcondi_tl(TCG_COND_EQ, t3, t1, 0);
            tcg_gen_or_tl(t2, t2, t3);
            tcg_gen_movi_tl(t3, 0);
            tcg_gen_movcond_tl(TCG_COND_NE, t1, t2, t3, t2, t1);
            tcg_gen_div_tl(cpu_gpr[rd], t0, t1);
            tcg_gen_ext32s_tl(cpu_gpr[rd], cpu_gpr[rd]);
            tcg_temp_free(t3);
            tcg_temp_free(t2);
        }
        break;
    case R6_OPC_MOD:
        {
            TCGv t2 = tcg_temp_new();
            TCGv t3 = tcg_temp_new();
            tcg_gen_ext32s_tl(t0, t0);
            tcg_gen_ext32s_tl(t1, t1);
            tcg_gen_setcondi_tl(TCG_COND_EQ, t2, t0, INT_MIN);
            tcg_gen_setcondi_tl(TCG_COND_EQ, t3, t1, -1);
            tcg_gen_and_tl(t2, t2, t3);
            tcg_gen_setcondi_tl(TCG_COND_EQ, t3, t1, 0);
            tcg_gen_or_tl(t2, t2, t3);
            tcg_gen_movi_tl(t3, 0);
            tcg_gen_movcond_tl(TCG_COND_NE, t1, t2, t3, t2, t1);
            tcg_gen_rem_tl(cpu_gpr[rd], t0, t1);
            tcg_gen_ext32s_tl(cpu_gpr[rd], cpu_gpr[rd]);
            tcg_temp_free(t3);
            tcg_temp_free(t2);
        }
        break;
    case R6_OPC_DIVU:
        {
            TCGv t2 = tcg_const_tl(0);
            TCGv t3 = tcg_const_tl(1);
            tcg_gen_ext32u_tl(t0, t0);
            tcg_gen_ext32u_tl(t1, t1);
            tcg_gen_movcond_tl(TCG_COND_EQ, t1, t1, t2, t3, t1);
            tcg_gen_divu_tl(cpu_gpr[rd], t0, t1);
            tcg_gen_ext32s_tl(cpu_gpr[rd], cpu_gpr[rd]);
            tcg_temp_free(t3);
            tcg_temp_free(t2);
        }
        break;
    case R6_OPC_MODU:
        {
            TCGv t2 = tcg_const_tl(0);
            TCGv t3 = tcg_const_tl(1);
            tcg_gen_ext32u_tl(t0, t0);
            tcg_gen_ext32u_tl(t1, t1);
            tcg_gen_movcond_tl(TCG_COND_EQ, t1, t1, t2, t3, t1);
            tcg_gen_remu_tl(cpu_gpr[rd], t0, t1);
            tcg_gen_ext32s_tl(cpu_gpr[rd], cpu_gpr[rd]);
            tcg_temp_free(t3);
            tcg_temp_free(t2);
        }
        break;
    case R6_OPC_MUL:
        {
            TCGv_i32 t2 = tcg_temp_new_i32();
            TCGv_i32 t3 = tcg_temp_new_i32();
            tcg_gen_trunc_tl_i32(t2, t0);
            tcg_gen_trunc_tl_i32(t3, t1);
            tcg_gen_mul_i32(t2, t2, t3);
            tcg_gen_ext_i32_tl(cpu_gpr[rd], t2);
            tcg_temp_free_i32(t2);
            tcg_temp_free_i32(t3);
        }
        break;
    case R6_OPC_MUH:
        {
            TCGv_i32 t2 = tcg_temp_new_i32();
            TCGv_i32 t3 = tcg_temp_new_i32();
            tcg_gen_trunc_tl_i32(t2, t0);
            tcg_gen_trunc_tl_i32(t3, t1);
            tcg_gen_muls2_i32(t2, t3, t2, t3);
            tcg_gen_ext_i32_tl(cpu_gpr[rd], t3);
            tcg_temp_free_i32(t2);
            tcg_temp_free_i32(t3);
        }
        break;
    case R6_OPC_MULU:
        {
            TCGv_i32 t2 = tcg_temp_new_i32();
            TCGv_i32 t3 = tcg_temp_new_i32();
            tcg_gen_trunc_tl_i32(t2, t0);
            tcg_gen_trunc_tl_i32(t3, t1);
            tcg_gen_mul_i32(t2, t2, t3);
            tcg_gen_ext_i32_tl(cpu_gpr[rd], t2);
            tcg_temp_free_i32(t2);
            tcg_temp_free_i32(t3);
        }
        break;
    case R6_OPC_MUHU:
        {
            TCGv_i32 t2 = tcg_temp_new_i32();
            TCGv_i32 t3 = tcg_temp_new_i32();
            tcg_gen_trunc_tl_i32(t2, t0);
            tcg_gen_trunc_tl_i32(t3, t1);
            tcg_gen_mulu2_i32(t2, t3, t2, t3);
            tcg_gen_ext_i32_tl(cpu_gpr[rd], t3);
            tcg_temp_free_i32(t2);
            tcg_temp_free_i32(t3);
        }
        break;
#if defined(TARGET_MIPS64)
    case R6_OPC_DDIV:
        {
            TCGv t2 = tcg_temp_new();
            TCGv t3 = tcg_temp_new();
            tcg_gen_setcondi_tl(TCG_COND_EQ, t2, t0, -1LL << 63);
            tcg_gen_setcondi_tl(TCG_COND_EQ, t3, t1, -1LL);
            tcg_gen_and_tl(t2, t2, t3);
            tcg_gen_setcondi_tl(TCG_COND_EQ, t3, t1, 0);
            tcg_gen_or_tl(t2, t2, t3);
            tcg_gen_movi_tl(t3, 0);
            tcg_gen_movcond_tl(TCG_COND_NE, t1, t2, t3, t2, t1);
            tcg_gen_div_tl(cpu_gpr[rd], t0, t1);
            tcg_temp_free(t3);
            tcg_temp_free(t2);
        }
        break;
    case R6_OPC_DMOD:
        {
            TCGv t2 = tcg_temp_new();
            TCGv t3 = tcg_temp_new();
            tcg_gen_setcondi_tl(TCG_COND_EQ, t2, t0, -1LL << 63);
            tcg_gen_setcondi_tl(TCG_COND_EQ, t3, t1, -1LL);
            tcg_gen_and_tl(t2, t2, t3);
            tcg_gen_setcondi_tl(TCG_COND_EQ, t3, t1, 0);
            tcg_gen_or_tl(t2, t2, t3);
            tcg_gen_movi_tl(t3, 0);
            tcg_gen_movcond_tl(TCG_COND_NE, t1, t2, t3, t2, t1);
            tcg_gen_rem_tl(cpu_gpr[rd], t0, t1);
            tcg_temp_free(t3);
            tcg_temp_free(t2);
        }
        break;
    case R6_OPC_DDIVU:
        {
            TCGv t2 = tcg_const_tl(0);
            TCGv t3 = tcg_const_tl(1);
            tcg_gen_movcond_tl(TCG_COND_EQ, t1, t1, t2, t3, t1);
            tcg_gen_divu_i64(cpu_gpr[rd], t0, t1);
            tcg_temp_free(t3);
            tcg_temp_free(t2);
        }
        break;
    case R6_OPC_DMODU:
        {
            TCGv t2 = tcg_const_tl(0);
            TCGv t3 = tcg_const_tl(1);
            tcg_gen_movcond_tl(TCG_COND_EQ, t1, t1, t2, t3, t1);
            tcg_gen_remu_i64(cpu_gpr[rd], t0, t1);
            tcg_temp_free(t3);
            tcg_temp_free(t2);
        }
        break;
    case R6_OPC_DMUL:
        tcg_gen_mul_i64(cpu_gpr[rd], t0, t1);
        break;
    case R6_OPC_DMUH:
        {
            TCGv t2 = tcg_temp_new();
            tcg_gen_muls2_i64(t2, cpu_gpr[rd], t0, t1);
            tcg_temp_free(t2);
        }
        break;
    case R6_OPC_DMULU:
        tcg_gen_mul_i64(cpu_gpr[rd], t0, t1);
        break;
    case R6_OPC_DMUHU:
        {
            TCGv t2 = tcg_temp_new();
            tcg_gen_mulu2_i64(t2, cpu_gpr[rd], t0, t1);
            tcg_temp_free(t2);
        }
        break;
#endif
    default:
        MIPS_INVAL("r6 mul/div");
        gen_reserved_instruction(ctx);
        goto out;
    }
    gen_log_instr_gpr_update(ctx, rd);
 out:
    tcg_temp_free(t0);
    tcg_temp_free(t1);
}

#if defined(TARGET_MIPS64)
static void gen_div1_tx79(DisasContext *ctx, uint32_t opc, int rs, int rt)
{
    TCGv t0, t1;

    t0 = tcg_temp_new();
    t1 = tcg_temp_new();

    gen_load_gpr(t0, rs);
    gen_load_gpr(t1, rt);

    switch (opc) {
    case MMI_OPC_DIV1:
        {
            TCGv t2 = tcg_temp_new();
            TCGv t3 = tcg_temp_new();
            tcg_gen_ext32s_tl(t0, t0);
            tcg_gen_ext32s_tl(t1, t1);
            tcg_gen_setcondi_tl(TCG_COND_EQ, t2, t0, INT_MIN);
            tcg_gen_setcondi_tl(TCG_COND_EQ, t3, t1, -1);
            tcg_gen_and_tl(t2, t2, t3);
            tcg_gen_setcondi_tl(TCG_COND_EQ, t3, t1, 0);
            tcg_gen_or_tl(t2, t2, t3);
            tcg_gen_movi_tl(t3, 0);
            tcg_gen_movcond_tl(TCG_COND_NE, t1, t2, t3, t2, t1);
            tcg_gen_div_tl(cpu_LO[1], t0, t1);
            tcg_gen_rem_tl(cpu_HI[1], t0, t1);
            tcg_gen_ext32s_tl(cpu_LO[1], cpu_LO[1]);
            tcg_gen_ext32s_tl(cpu_HI[1], cpu_HI[1]);
            tcg_temp_free(t3);
            tcg_temp_free(t2);
        }
        break;
    case MMI_OPC_DIVU1:
        {
            TCGv t2 = tcg_const_tl(0);
            TCGv t3 = tcg_const_tl(1);
            tcg_gen_ext32u_tl(t0, t0);
            tcg_gen_ext32u_tl(t1, t1);
            tcg_gen_movcond_tl(TCG_COND_EQ, t1, t1, t2, t3, t1);
            tcg_gen_divu_tl(cpu_LO[1], t0, t1);
            tcg_gen_remu_tl(cpu_HI[1], t0, t1);
            tcg_gen_ext32s_tl(cpu_LO[1], cpu_LO[1]);
            tcg_gen_ext32s_tl(cpu_HI[1], cpu_HI[1]);
            tcg_temp_free(t3);
            tcg_temp_free(t2);
        }
        break;
    default:
        MIPS_INVAL("div1 TX79");
        gen_reserved_instruction(ctx);
        goto out;
    }
 out:
    tcg_temp_free(t0);
    tcg_temp_free(t1);
}
#endif

static void gen_muldiv(DisasContext *ctx, uint32_t opc,
                       int acc, int rs, int rt)
{
    TCGv t0, t1;

    t0 = tcg_temp_new();
    t1 = tcg_temp_new();

    gen_load_gpr(t0, rs);
    gen_load_gpr(t1, rt);

    if (acc != 0) {
        check_dsp(ctx);
    }

    switch (opc) {
    case OPC_DIV:
        {
            TCGv t2 = tcg_temp_new();
            TCGv t3 = tcg_temp_new();
            tcg_gen_ext32s_tl(t0, t0);
            tcg_gen_ext32s_tl(t1, t1);
            tcg_gen_setcondi_tl(TCG_COND_EQ, t2, t0, INT_MIN);
            tcg_gen_setcondi_tl(TCG_COND_EQ, t3, t1, -1);
            tcg_gen_and_tl(t2, t2, t3);
            tcg_gen_setcondi_tl(TCG_COND_EQ, t3, t1, 0);
            tcg_gen_or_tl(t2, t2, t3);
            tcg_gen_movi_tl(t3, 0);
            tcg_gen_movcond_tl(TCG_COND_NE, t1, t2, t3, t2, t1);
            tcg_gen_div_tl(cpu_LO[acc], t0, t1);
            tcg_gen_rem_tl(cpu_HI[acc], t0, t1);
            tcg_gen_ext32s_tl(cpu_LO[acc], cpu_LO[acc]);
            tcg_gen_ext32s_tl(cpu_HI[acc], cpu_HI[acc]);
            tcg_temp_free(t3);
            tcg_temp_free(t2);
        }
        break;
    case OPC_DIVU:
        {
            TCGv t2 = tcg_const_tl(0);
            TCGv t3 = tcg_const_tl(1);
            tcg_gen_ext32u_tl(t0, t0);
            tcg_gen_ext32u_tl(t1, t1);
            tcg_gen_movcond_tl(TCG_COND_EQ, t1, t1, t2, t3, t1);
            tcg_gen_divu_tl(cpu_LO[acc], t0, t1);
            tcg_gen_remu_tl(cpu_HI[acc], t0, t1);
            tcg_gen_ext32s_tl(cpu_LO[acc], cpu_LO[acc]);
            tcg_gen_ext32s_tl(cpu_HI[acc], cpu_HI[acc]);
            tcg_temp_free(t3);
            tcg_temp_free(t2);
        }
        break;
    case OPC_MULT:
        {
            TCGv_i32 t2 = tcg_temp_new_i32();
            TCGv_i32 t3 = tcg_temp_new_i32();
            tcg_gen_trunc_tl_i32(t2, t0);
            tcg_gen_trunc_tl_i32(t3, t1);
            tcg_gen_muls2_i32(t2, t3, t2, t3);
            tcg_gen_ext_i32_tl(cpu_LO[acc], t2);
            tcg_gen_ext_i32_tl(cpu_HI[acc], t3);
            tcg_temp_free_i32(t2);
            tcg_temp_free_i32(t3);
        }
        break;
    case OPC_MULTU:
        {
            TCGv_i32 t2 = tcg_temp_new_i32();
            TCGv_i32 t3 = tcg_temp_new_i32();
            tcg_gen_trunc_tl_i32(t2, t0);
            tcg_gen_trunc_tl_i32(t3, t1);
            tcg_gen_mulu2_i32(t2, t3, t2, t3);
            tcg_gen_ext_i32_tl(cpu_LO[acc], t2);
            tcg_gen_ext_i32_tl(cpu_HI[acc], t3);
            tcg_temp_free_i32(t2);
            tcg_temp_free_i32(t3);
        }
        break;
#if defined(TARGET_MIPS64)
    case OPC_DDIV:
        {
            TCGv t2 = tcg_temp_new();
            TCGv t3 = tcg_temp_new();
            tcg_gen_setcondi_tl(TCG_COND_EQ, t2, t0, -1LL << 63);
            tcg_gen_setcondi_tl(TCG_COND_EQ, t3, t1, -1LL);
            tcg_gen_and_tl(t2, t2, t3);
            tcg_gen_setcondi_tl(TCG_COND_EQ, t3, t1, 0);
            tcg_gen_or_tl(t2, t2, t3);
            tcg_gen_movi_tl(t3, 0);
            tcg_gen_movcond_tl(TCG_COND_NE, t1, t2, t3, t2, t1);
            tcg_gen_div_tl(cpu_LO[acc], t0, t1);
            tcg_gen_rem_tl(cpu_HI[acc], t0, t1);
            tcg_temp_free(t3);
            tcg_temp_free(t2);
        }
        break;
    case OPC_DDIVU:
        {
            TCGv t2 = tcg_const_tl(0);
            TCGv t3 = tcg_const_tl(1);
            tcg_gen_movcond_tl(TCG_COND_EQ, t1, t1, t2, t3, t1);
            tcg_gen_divu_i64(cpu_LO[acc], t0, t1);
            tcg_gen_remu_i64(cpu_HI[acc], t0, t1);
            tcg_temp_free(t3);
            tcg_temp_free(t2);
        }
        break;
    case OPC_DMULT:
        tcg_gen_muls2_i64(cpu_LO[acc], cpu_HI[acc], t0, t1);
        break;
    case OPC_DMULTU:
        tcg_gen_mulu2_i64(cpu_LO[acc], cpu_HI[acc], t0, t1);
        break;
#endif
    case OPC_MADD:
        {
            TCGv_i64 t2 = tcg_temp_new_i64();
            TCGv_i64 t3 = tcg_temp_new_i64();

            tcg_gen_ext_tl_i64(t2, t0);
            tcg_gen_ext_tl_i64(t3, t1);
            tcg_gen_mul_i64(t2, t2, t3);
            tcg_gen_concat_tl_i64(t3, cpu_LO[acc], cpu_HI[acc]);
            tcg_gen_add_i64(t2, t2, t3);
            tcg_temp_free_i64(t3);
            gen_move_low32(cpu_LO[acc], t2);
            gen_move_high32(cpu_HI[acc], t2);
            tcg_temp_free_i64(t2);
        }
        break;
    case OPC_MADDU:
        {
            TCGv_i64 t2 = tcg_temp_new_i64();
            TCGv_i64 t3 = tcg_temp_new_i64();

            tcg_gen_ext32u_tl(t0, t0);
            tcg_gen_ext32u_tl(t1, t1);
            tcg_gen_extu_tl_i64(t2, t0);
            tcg_gen_extu_tl_i64(t3, t1);
            tcg_gen_mul_i64(t2, t2, t3);
            tcg_gen_concat_tl_i64(t3, cpu_LO[acc], cpu_HI[acc]);
            tcg_gen_add_i64(t2, t2, t3);
            tcg_temp_free_i64(t3);
            gen_move_low32(cpu_LO[acc], t2);
            gen_move_high32(cpu_HI[acc], t2);
            tcg_temp_free_i64(t2);
        }
        break;
    case OPC_MSUB:
        {
            TCGv_i64 t2 = tcg_temp_new_i64();
            TCGv_i64 t3 = tcg_temp_new_i64();

            tcg_gen_ext_tl_i64(t2, t0);
            tcg_gen_ext_tl_i64(t3, t1);
            tcg_gen_mul_i64(t2, t2, t3);
            tcg_gen_concat_tl_i64(t3, cpu_LO[acc], cpu_HI[acc]);
            tcg_gen_sub_i64(t2, t3, t2);
            tcg_temp_free_i64(t3);
            gen_move_low32(cpu_LO[acc], t2);
            gen_move_high32(cpu_HI[acc], t2);
            tcg_temp_free_i64(t2);
        }
        break;
    case OPC_MSUBU:
        {
            TCGv_i64 t2 = tcg_temp_new_i64();
            TCGv_i64 t3 = tcg_temp_new_i64();

            tcg_gen_ext32u_tl(t0, t0);
            tcg_gen_ext32u_tl(t1, t1);
            tcg_gen_extu_tl_i64(t2, t0);
            tcg_gen_extu_tl_i64(t3, t1);
            tcg_gen_mul_i64(t2, t2, t3);
            tcg_gen_concat_tl_i64(t3, cpu_LO[acc], cpu_HI[acc]);
            tcg_gen_sub_i64(t2, t3, t2);
            tcg_temp_free_i64(t3);
            gen_move_low32(cpu_LO[acc], t2);
            gen_move_high32(cpu_HI[acc], t2);
            tcg_temp_free_i64(t2);
        }
        break;
    default:
        MIPS_INVAL("mul/div");
        gen_reserved_instruction(ctx);
        goto out;
    }
 out:
    tcg_temp_free(t0);
    tcg_temp_free(t1);
}

/*
 * These MULT[U] and MADD[U] instructions implemented in for example
 * the Toshiba/Sony R5900 and the Toshiba TX19, TX39 and TX79 core
 * architectures are special three-operand variants with the syntax
 *
 *     MULT[U][1] rd, rs, rt
 *
 * such that
 *
 *     (rd, LO, HI) <- rs * rt
 *
 * and
 *
 *     MADD[U][1] rd, rs, rt
 *
 * such that
 *
 *     (rd, LO, HI) <- (LO, HI) + rs * rt
 *
 * where the low-order 32-bits of the result is placed into both the
 * GPR rd and the special register LO. The high-order 32-bits of the
 * result is placed into the special register HI.
 *
 * If the GPR rd is omitted in assembly language, it is taken to be 0,
 * which is the zero register that always reads as 0.
 */
static void gen_mul_txx9(DisasContext *ctx, uint32_t opc,
                         int rd, int rs, int rt)
{
    TCGv t0 = tcg_temp_new();
    TCGv t1 = tcg_temp_new();
    int acc = 0;

    gen_load_gpr(t0, rs);
    gen_load_gpr(t1, rt);

    switch (opc) {
    case MMI_OPC_MULT1:
        acc = 1;
        /* Fall through */
    case OPC_MULT:
        {
            TCGv_i32 t2 = tcg_temp_new_i32();
            TCGv_i32 t3 = tcg_temp_new_i32();
            tcg_gen_trunc_tl_i32(t2, t0);
            tcg_gen_trunc_tl_i32(t3, t1);
            tcg_gen_muls2_i32(t2, t3, t2, t3);
            if (rd) {
                tcg_gen_ext_i32_tl(cpu_gpr[rd], t2);
            }
            tcg_gen_ext_i32_tl(cpu_LO[acc], t2);
            tcg_gen_ext_i32_tl(cpu_HI[acc], t3);
            tcg_temp_free_i32(t2);
            tcg_temp_free_i32(t3);
        }
        break;
    case MMI_OPC_MULTU1:
        acc = 1;
        /* Fall through */
    case OPC_MULTU:
        {
            TCGv_i32 t2 = tcg_temp_new_i32();
            TCGv_i32 t3 = tcg_temp_new_i32();
            tcg_gen_trunc_tl_i32(t2, t0);
            tcg_gen_trunc_tl_i32(t3, t1);
            tcg_gen_mulu2_i32(t2, t3, t2, t3);
            if (rd) {
                tcg_gen_ext_i32_tl(cpu_gpr[rd], t2);
            }
            tcg_gen_ext_i32_tl(cpu_LO[acc], t2);
            tcg_gen_ext_i32_tl(cpu_HI[acc], t3);
            tcg_temp_free_i32(t2);
            tcg_temp_free_i32(t3);
        }
        break;
    case MMI_OPC_MADD1:
        acc = 1;
        /* Fall through */
    case MMI_OPC_MADD:
        {
            TCGv_i64 t2 = tcg_temp_new_i64();
            TCGv_i64 t3 = tcg_temp_new_i64();

            tcg_gen_ext_tl_i64(t2, t0);
            tcg_gen_ext_tl_i64(t3, t1);
            tcg_gen_mul_i64(t2, t2, t3);
            tcg_gen_concat_tl_i64(t3, cpu_LO[acc], cpu_HI[acc]);
            tcg_gen_add_i64(t2, t2, t3);
            tcg_temp_free_i64(t3);
            gen_move_low32(cpu_LO[acc], t2);
            gen_move_high32(cpu_HI[acc], t2);
            if (rd) {
                gen_move_low32(cpu_gpr[rd], t2);
            }
            tcg_temp_free_i64(t2);
        }
        break;
    case MMI_OPC_MADDU1:
        acc = 1;
        /* Fall through */
    case MMI_OPC_MADDU:
        {
            TCGv_i64 t2 = tcg_temp_new_i64();
            TCGv_i64 t3 = tcg_temp_new_i64();

            tcg_gen_ext32u_tl(t0, t0);
            tcg_gen_ext32u_tl(t1, t1);
            tcg_gen_extu_tl_i64(t2, t0);
            tcg_gen_extu_tl_i64(t3, t1);
            tcg_gen_mul_i64(t2, t2, t3);
            tcg_gen_concat_tl_i64(t3, cpu_LO[acc], cpu_HI[acc]);
            tcg_gen_add_i64(t2, t2, t3);
            tcg_temp_free_i64(t3);
            gen_move_low32(cpu_LO[acc], t2);
            gen_move_high32(cpu_HI[acc], t2);
            if (rd) {
                gen_move_low32(cpu_gpr[rd], t2);
            }
            tcg_temp_free_i64(t2);
        }
        break;
    default:
        MIPS_INVAL("mul/madd TXx9");
        gen_reserved_instruction(ctx);
        goto out;
    }
    if (rd) {
        gen_log_instr_gpr_update(ctx, rd);
    }
 out:
    tcg_temp_free(t0);
    tcg_temp_free(t1);
}

static void gen_mul_vr54xx(DisasContext *ctx, uint32_t opc,
                           int rd, int rs, int rt)
{
    TCGv t0 = tcg_temp_new();
    TCGv t1 = tcg_temp_new();

    gen_load_gpr(t0, rs);
    gen_load_gpr(t1, rt);

    switch (opc) {
    case OPC_VR54XX_MULS:
        gen_helper_muls(t0, cpu_env, t0, t1);
        break;
    case OPC_VR54XX_MULSU:
        gen_helper_mulsu(t0, cpu_env, t0, t1);
        break;
    case OPC_VR54XX_MACC:
        gen_helper_macc(t0, cpu_env, t0, t1);
        break;
    case OPC_VR54XX_MACCU:
        gen_helper_maccu(t0, cpu_env, t0, t1);
        break;
    case OPC_VR54XX_MSAC:
        gen_helper_msac(t0, cpu_env, t0, t1);
        break;
    case OPC_VR54XX_MSACU:
        gen_helper_msacu(t0, cpu_env, t0, t1);
        break;
    case OPC_VR54XX_MULHI:
        gen_helper_mulhi(t0, cpu_env, t0, t1);
        break;
    case OPC_VR54XX_MULHIU:
        gen_helper_mulhiu(t0, cpu_env, t0, t1);
        break;
    case OPC_VR54XX_MULSHI:
        gen_helper_mulshi(t0, cpu_env, t0, t1);
        break;
    case OPC_VR54XX_MULSHIU:
        gen_helper_mulshiu(t0, cpu_env, t0, t1);
        break;
    case OPC_VR54XX_MACCHI:
        gen_helper_macchi(t0, cpu_env, t0, t1);
        break;
    case OPC_VR54XX_MACCHIU:
        gen_helper_macchiu(t0, cpu_env, t0, t1);
        break;
    case OPC_VR54XX_MSACHI:
        gen_helper_msachi(t0, cpu_env, t0, t1);
        break;
    case OPC_VR54XX_MSACHIU:
        gen_helper_msachiu(t0, cpu_env, t0, t1);
        break;
    default:
        MIPS_INVAL("mul vr54xx");
        gen_reserved_instruction(ctx);
        goto out;
    }
    gen_store_gpr(t0, rd);

 out:
    tcg_temp_free(t0);
    tcg_temp_free(t1);
}

static void gen_cl(DisasContext *ctx, uint32_t opc,
                   int rd, int rs)
{
    TCGv t0;

    if (rd == 0) {
        /* Treat as NOP. */
        return;
    }
    t0 = cpu_gpr[rd];
    gen_load_gpr(t0, rs);

    switch (opc) {
    case OPC_CLO:
    case R6_OPC_CLO:
#if defined(TARGET_MIPS64)
    case OPC_DCLO:
    case R6_OPC_DCLO:
#endif
        tcg_gen_not_tl(t0, t0);
        break;
    }

    switch (opc) {
    case OPC_CLO:
    case R6_OPC_CLO:
    case OPC_CLZ:
    case R6_OPC_CLZ:
        tcg_gen_ext32u_tl(t0, t0);
        tcg_gen_clzi_tl(t0, t0, TARGET_LONG_BITS);
        tcg_gen_subi_tl(t0, t0, TARGET_LONG_BITS - 32);
        break;
#if defined(TARGET_MIPS64)
    case OPC_DCLO:
    case R6_OPC_DCLO:
    case OPC_DCLZ:
    case R6_OPC_DCLZ:
        tcg_gen_clzi_i64(t0, t0, 64);
        break;
#endif
    }
}

/* Godson integer instructions */
static void gen_loongson_integer(DisasContext *ctx, uint32_t opc,
                                 int rd, int rs, int rt)
{
    TCGv t0, t1;

    if (rd == 0) {
        /* Treat as NOP. */
        return;
    }

    switch (opc) {
    case OPC_MULT_G_2E:
    case OPC_MULT_G_2F:
    case OPC_MULTU_G_2E:
    case OPC_MULTU_G_2F:
#if defined(TARGET_MIPS64)
    case OPC_DMULT_G_2E:
    case OPC_DMULT_G_2F:
    case OPC_DMULTU_G_2E:
    case OPC_DMULTU_G_2F:
#endif
        t0 = tcg_temp_new();
        t1 = tcg_temp_new();
        break;
    default:
        t0 = tcg_temp_local_new();
        t1 = tcg_temp_local_new();
        break;
    }

    gen_load_gpr(t0, rs);
    gen_load_gpr(t1, rt);

    switch (opc) {
    case OPC_MULT_G_2E:
    case OPC_MULT_G_2F:
        tcg_gen_mul_tl(cpu_gpr[rd], t0, t1);
        tcg_gen_ext32s_tl(cpu_gpr[rd], cpu_gpr[rd]);
        break;
    case OPC_MULTU_G_2E:
    case OPC_MULTU_G_2F:
        tcg_gen_ext32u_tl(t0, t0);
        tcg_gen_ext32u_tl(t1, t1);
        tcg_gen_mul_tl(cpu_gpr[rd], t0, t1);
        tcg_gen_ext32s_tl(cpu_gpr[rd], cpu_gpr[rd]);
        break;
    case OPC_DIV_G_2E:
    case OPC_DIV_G_2F:
        {
            TCGLabel *l1 = gen_new_label();
            TCGLabel *l2 = gen_new_label();
            TCGLabel *l3 = gen_new_label();
            tcg_gen_ext32s_tl(t0, t0);
            tcg_gen_ext32s_tl(t1, t1);
            tcg_gen_brcondi_tl(TCG_COND_NE, t1, 0, l1);
            tcg_gen_movi_tl(cpu_gpr[rd], 0);
            tcg_gen_br(l3);
            gen_set_label(l1);
            tcg_gen_brcondi_tl(TCG_COND_NE, t0, INT_MIN, l2);
            tcg_gen_brcondi_tl(TCG_COND_NE, t1, -1, l2);
            tcg_gen_mov_tl(cpu_gpr[rd], t0);
            tcg_gen_br(l3);
            gen_set_label(l2);
            tcg_gen_div_tl(cpu_gpr[rd], t0, t1);
            tcg_gen_ext32s_tl(cpu_gpr[rd], cpu_gpr[rd]);
            gen_set_label(l3);
        }
        break;
    case OPC_DIVU_G_2E:
    case OPC_DIVU_G_2F:
        {
            TCGLabel *l1 = gen_new_label();
            TCGLabel *l2 = gen_new_label();
            tcg_gen_ext32u_tl(t0, t0);
            tcg_gen_ext32u_tl(t1, t1);
            tcg_gen_brcondi_tl(TCG_COND_NE, t1, 0, l1);
            tcg_gen_movi_tl(cpu_gpr[rd], 0);
            tcg_gen_br(l2);
            gen_set_label(l1);
            tcg_gen_divu_tl(cpu_gpr[rd], t0, t1);
            tcg_gen_ext32s_tl(cpu_gpr[rd], cpu_gpr[rd]);
            gen_set_label(l2);
        }
        break;
    case OPC_MOD_G_2E:
    case OPC_MOD_G_2F:
        {
            TCGLabel *l1 = gen_new_label();
            TCGLabel *l2 = gen_new_label();
            TCGLabel *l3 = gen_new_label();
            tcg_gen_ext32u_tl(t0, t0);
            tcg_gen_ext32u_tl(t1, t1);
            tcg_gen_brcondi_tl(TCG_COND_EQ, t1, 0, l1);
            tcg_gen_brcondi_tl(TCG_COND_NE, t0, INT_MIN, l2);
            tcg_gen_brcondi_tl(TCG_COND_NE, t1, -1, l2);
            gen_set_label(l1);
            tcg_gen_movi_tl(cpu_gpr[rd], 0);
            tcg_gen_br(l3);
            gen_set_label(l2);
            tcg_gen_rem_tl(cpu_gpr[rd], t0, t1);
            tcg_gen_ext32s_tl(cpu_gpr[rd], cpu_gpr[rd]);
            gen_set_label(l3);
        }
        break;
    case OPC_MODU_G_2E:
    case OPC_MODU_G_2F:
        {
            TCGLabel *l1 = gen_new_label();
            TCGLabel *l2 = gen_new_label();
            tcg_gen_ext32u_tl(t0, t0);
            tcg_gen_ext32u_tl(t1, t1);
            tcg_gen_brcondi_tl(TCG_COND_NE, t1, 0, l1);
            tcg_gen_movi_tl(cpu_gpr[rd], 0);
            tcg_gen_br(l2);
            gen_set_label(l1);
            tcg_gen_remu_tl(cpu_gpr[rd], t0, t1);
            tcg_gen_ext32s_tl(cpu_gpr[rd], cpu_gpr[rd]);
            gen_set_label(l2);
        }
        break;
#if defined(TARGET_MIPS64)
    case OPC_DMULT_G_2E:
    case OPC_DMULT_G_2F:
        tcg_gen_mul_tl(cpu_gpr[rd], t0, t1);
        break;
    case OPC_DMULTU_G_2E:
    case OPC_DMULTU_G_2F:
        tcg_gen_mul_tl(cpu_gpr[rd], t0, t1);
        break;
    case OPC_DDIV_G_2E:
    case OPC_DDIV_G_2F:
        {
            TCGLabel *l1 = gen_new_label();
            TCGLabel *l2 = gen_new_label();
            TCGLabel *l3 = gen_new_label();
            tcg_gen_brcondi_tl(TCG_COND_NE, t1, 0, l1);
            tcg_gen_movi_tl(cpu_gpr[rd], 0);
            tcg_gen_br(l3);
            gen_set_label(l1);
            tcg_gen_brcondi_tl(TCG_COND_NE, t0, -1LL << 63, l2);
            tcg_gen_brcondi_tl(TCG_COND_NE, t1, -1LL, l2);
            tcg_gen_mov_tl(cpu_gpr[rd], t0);
            tcg_gen_br(l3);
            gen_set_label(l2);
            tcg_gen_div_tl(cpu_gpr[rd], t0, t1);
            gen_set_label(l3);
        }
        break;
    case OPC_DDIVU_G_2E:
    case OPC_DDIVU_G_2F:
        {
            TCGLabel *l1 = gen_new_label();
            TCGLabel *l2 = gen_new_label();
            tcg_gen_brcondi_tl(TCG_COND_NE, t1, 0, l1);
            tcg_gen_movi_tl(cpu_gpr[rd], 0);
            tcg_gen_br(l2);
            gen_set_label(l1);
            tcg_gen_divu_tl(cpu_gpr[rd], t0, t1);
            gen_set_label(l2);
        }
        break;
    case OPC_DMOD_G_2E:
    case OPC_DMOD_G_2F:
        {
            TCGLabel *l1 = gen_new_label();
            TCGLabel *l2 = gen_new_label();
            TCGLabel *l3 = gen_new_label();
            tcg_gen_brcondi_tl(TCG_COND_EQ, t1, 0, l1);
            tcg_gen_brcondi_tl(TCG_COND_NE, t0, -1LL << 63, l2);
            tcg_gen_brcondi_tl(TCG_COND_NE, t1, -1LL, l2);
            gen_set_label(l1);
            tcg_gen_movi_tl(cpu_gpr[rd], 0);
            tcg_gen_br(l3);
            gen_set_label(l2);
            tcg_gen_rem_tl(cpu_gpr[rd], t0, t1);
            gen_set_label(l3);
        }
        break;
    case OPC_DMODU_G_2E:
    case OPC_DMODU_G_2F:
        {
            TCGLabel *l1 = gen_new_label();
            TCGLabel *l2 = gen_new_label();
            tcg_gen_brcondi_tl(TCG_COND_NE, t1, 0, l1);
            tcg_gen_movi_tl(cpu_gpr[rd], 0);
            tcg_gen_br(l2);
            gen_set_label(l1);
            tcg_gen_remu_tl(cpu_gpr[rd], t0, t1);
            gen_set_label(l2);
        }
        break;
#endif
    }
    gen_log_instr_gpr_update(ctx, rd);
    tcg_temp_free(t0);
    tcg_temp_free(t1);
}

#if !defined(TARGET_CHERI)
/* Loongson multimedia instructions */
static void gen_loongson_multimedia(DisasContext *ctx, int rd, int rs, int rt)
{
    uint32_t opc, shift_max;
    TCGv_i64 t0, t1;
    TCGCond cond;

    opc = MASK_LMMI(ctx->opcode);
    switch (opc) {
    case OPC_ADD_CP2:
    case OPC_SUB_CP2:
    case OPC_DADD_CP2:
    case OPC_DSUB_CP2:
        t0 = tcg_temp_local_new_i64();
        t1 = tcg_temp_local_new_i64();
        break;
    default:
        t0 = tcg_temp_new_i64();
        t1 = tcg_temp_new_i64();
        break;
    }

    check_cp1_enabled(ctx);
    gen_load_fpr64(ctx, t0, rs);
    gen_load_fpr64(ctx, t1, rt);

    switch (opc) {
    case OPC_PADDSH:
        gen_helper_paddsh(t0, t0, t1);
        break;
    case OPC_PADDUSH:
        gen_helper_paddush(t0, t0, t1);
        break;
    case OPC_PADDH:
        gen_helper_paddh(t0, t0, t1);
        break;
    case OPC_PADDW:
        gen_helper_paddw(t0, t0, t1);
        break;
    case OPC_PADDSB:
        gen_helper_paddsb(t0, t0, t1);
        break;
    case OPC_PADDUSB:
        gen_helper_paddusb(t0, t0, t1);
        break;
    case OPC_PADDB:
        gen_helper_paddb(t0, t0, t1);
        break;

    case OPC_PSUBSH:
        gen_helper_psubsh(t0, t0, t1);
        break;
    case OPC_PSUBUSH:
        gen_helper_psubush(t0, t0, t1);
        break;
    case OPC_PSUBH:
        gen_helper_psubh(t0, t0, t1);
        break;
    case OPC_PSUBW:
        gen_helper_psubw(t0, t0, t1);
        break;
    case OPC_PSUBSB:
        gen_helper_psubsb(t0, t0, t1);
        break;
    case OPC_PSUBUSB:
        gen_helper_psubusb(t0, t0, t1);
        break;
    case OPC_PSUBB:
        gen_helper_psubb(t0, t0, t1);
        break;

    case OPC_PSHUFH:
        gen_helper_pshufh(t0, t0, t1);
        break;
    case OPC_PACKSSWH:
        gen_helper_packsswh(t0, t0, t1);
        break;
    case OPC_PACKSSHB:
        gen_helper_packsshb(t0, t0, t1);
        break;
    case OPC_PACKUSHB:
        gen_helper_packushb(t0, t0, t1);
        break;

    case OPC_PUNPCKLHW:
        gen_helper_punpcklhw(t0, t0, t1);
        break;
    case OPC_PUNPCKHHW:
        gen_helper_punpckhhw(t0, t0, t1);
        break;
    case OPC_PUNPCKLBH:
        gen_helper_punpcklbh(t0, t0, t1);
        break;
    case OPC_PUNPCKHBH:
        gen_helper_punpckhbh(t0, t0, t1);
        break;
    case OPC_PUNPCKLWD:
        gen_helper_punpcklwd(t0, t0, t1);
        break;
    case OPC_PUNPCKHWD:
        gen_helper_punpckhwd(t0, t0, t1);
        break;

    case OPC_PAVGH:
        gen_helper_pavgh(t0, t0, t1);
        break;
    case OPC_PAVGB:
        gen_helper_pavgb(t0, t0, t1);
        break;
    case OPC_PMAXSH:
        gen_helper_pmaxsh(t0, t0, t1);
        break;
    case OPC_PMINSH:
        gen_helper_pminsh(t0, t0, t1);
        break;
    case OPC_PMAXUB:
        gen_helper_pmaxub(t0, t0, t1);
        break;
    case OPC_PMINUB:
        gen_helper_pminub(t0, t0, t1);
        break;

    case OPC_PCMPEQW:
        gen_helper_pcmpeqw(t0, t0, t1);
        break;
    case OPC_PCMPGTW:
        gen_helper_pcmpgtw(t0, t0, t1);
        break;
    case OPC_PCMPEQH:
        gen_helper_pcmpeqh(t0, t0, t1);
        break;
    case OPC_PCMPGTH:
        gen_helper_pcmpgth(t0, t0, t1);
        break;
    case OPC_PCMPEQB:
        gen_helper_pcmpeqb(t0, t0, t1);
        break;
    case OPC_PCMPGTB:
        gen_helper_pcmpgtb(t0, t0, t1);
        break;

    case OPC_PSLLW:
        gen_helper_psllw(t0, t0, t1);
        break;
    case OPC_PSLLH:
        gen_helper_psllh(t0, t0, t1);
        break;
    case OPC_PSRLW:
        gen_helper_psrlw(t0, t0, t1);
        break;
    case OPC_PSRLH:
        gen_helper_psrlh(t0, t0, t1);
        break;
    case OPC_PSRAW:
        gen_helper_psraw(t0, t0, t1);
        break;
    case OPC_PSRAH:
        gen_helper_psrah(t0, t0, t1);
        break;

    case OPC_PMULLH:
        gen_helper_pmullh(t0, t0, t1);
        break;
    case OPC_PMULHH:
        gen_helper_pmulhh(t0, t0, t1);
        break;
    case OPC_PMULHUH:
        gen_helper_pmulhuh(t0, t0, t1);
        break;
    case OPC_PMADDHW:
        gen_helper_pmaddhw(t0, t0, t1);
        break;

    case OPC_PASUBUB:
        gen_helper_pasubub(t0, t0, t1);
        break;
    case OPC_BIADD:
        gen_helper_biadd(t0, t0);
        break;
    case OPC_PMOVMSKB:
        gen_helper_pmovmskb(t0, t0);
        break;

    case OPC_PADDD:
        tcg_gen_add_i64(t0, t0, t1);
        break;
    case OPC_PSUBD:
        tcg_gen_sub_i64(t0, t0, t1);
        break;
    case OPC_XOR_CP2:
        tcg_gen_xor_i64(t0, t0, t1);
        break;
    case OPC_NOR_CP2:
        tcg_gen_nor_i64(t0, t0, t1);
        break;
    case OPC_AND_CP2:
        tcg_gen_and_i64(t0, t0, t1);
        break;
    case OPC_OR_CP2:
        tcg_gen_or_i64(t0, t0, t1);
        break;

    case OPC_PANDN:
        tcg_gen_andc_i64(t0, t1, t0);
        break;

    case OPC_PINSRH_0:
        tcg_gen_deposit_i64(t0, t0, t1, 0, 16);
        break;
    case OPC_PINSRH_1:
        tcg_gen_deposit_i64(t0, t0, t1, 16, 16);
        break;
    case OPC_PINSRH_2:
        tcg_gen_deposit_i64(t0, t0, t1, 32, 16);
        break;
    case OPC_PINSRH_3:
        tcg_gen_deposit_i64(t0, t0, t1, 48, 16);
        break;

    case OPC_PEXTRH:
        tcg_gen_andi_i64(t1, t1, 3);
        tcg_gen_shli_i64(t1, t1, 4);
        tcg_gen_shr_i64(t0, t0, t1);
        tcg_gen_ext16u_i64(t0, t0);
        break;

    case OPC_ADDU_CP2:
        tcg_gen_add_i64(t0, t0, t1);
        tcg_gen_ext32s_i64(t0, t0);
        break;
    case OPC_SUBU_CP2:
        tcg_gen_sub_i64(t0, t0, t1);
        tcg_gen_ext32s_i64(t0, t0);
        break;

    case OPC_SLL_CP2:
        shift_max = 32;
        goto do_shift;
    case OPC_SRL_CP2:
        shift_max = 32;
        goto do_shift;
    case OPC_SRA_CP2:
        shift_max = 32;
        goto do_shift;
    case OPC_DSLL_CP2:
        shift_max = 64;
        goto do_shift;
    case OPC_DSRL_CP2:
        shift_max = 64;
        goto do_shift;
    case OPC_DSRA_CP2:
        shift_max = 64;
        goto do_shift;
    do_shift:
        /* Make sure shift count isn't TCG undefined behaviour.  */
        tcg_gen_andi_i64(t1, t1, shift_max - 1);

        switch (opc) {
        case OPC_SLL_CP2:
        case OPC_DSLL_CP2:
            tcg_gen_shl_i64(t0, t0, t1);
            break;
        case OPC_SRA_CP2:
        case OPC_DSRA_CP2:
            /*
             * Since SRA is UndefinedResult without sign-extended inputs,
             * we can treat SRA and DSRA the same.
             */
            tcg_gen_sar_i64(t0, t0, t1);
            break;
        case OPC_SRL_CP2:
            /* We want to shift in zeros for SRL; zero-extend first.  */
            tcg_gen_ext32u_i64(t0, t0);
            /* FALLTHRU */
        case OPC_DSRL_CP2:
            tcg_gen_shr_i64(t0, t0, t1);
            break;
        }

        if (shift_max == 32) {
            tcg_gen_ext32s_i64(t0, t0);
        }

        /* Shifts larger than MAX produce zero.  */
        tcg_gen_setcondi_i64(TCG_COND_LTU, t1, t1, shift_max);
        tcg_gen_neg_i64(t1, t1);
        tcg_gen_and_i64(t0, t0, t1);
        break;

    case OPC_ADD_CP2:
    case OPC_DADD_CP2:
        {
            TCGv_i64 t2 = tcg_temp_new_i64();
            TCGLabel *lab = gen_new_label();

            tcg_gen_mov_i64(t2, t0);
            tcg_gen_add_i64(t0, t1, t2);
            if (opc == OPC_ADD_CP2) {
                tcg_gen_ext32s_i64(t0, t0);
            }
            tcg_gen_xor_i64(t1, t1, t2);
            tcg_gen_xor_i64(t2, t2, t0);
            tcg_gen_andc_i64(t1, t2, t1);
            tcg_temp_free_i64(t2);
            tcg_gen_brcondi_i64(TCG_COND_GE, t1, 0, lab);
            generate_exception(ctx, EXCP_OVERFLOW);
            gen_set_label(lab);
            break;
        }

    case OPC_SUB_CP2:
    case OPC_DSUB_CP2:
        {
            TCGv_i64 t2 = tcg_temp_new_i64();
            TCGLabel *lab = gen_new_label();

            tcg_gen_mov_i64(t2, t0);
            tcg_gen_sub_i64(t0, t1, t2);
            if (opc == OPC_SUB_CP2) {
                tcg_gen_ext32s_i64(t0, t0);
            }
            tcg_gen_xor_i64(t1, t1, t2);
            tcg_gen_xor_i64(t2, t2, t0);
            tcg_gen_and_i64(t1, t1, t2);
            tcg_temp_free_i64(t2);
            tcg_gen_brcondi_i64(TCG_COND_GE, t1, 0, lab);
            generate_exception(ctx, EXCP_OVERFLOW);
            gen_set_label(lab);
            break;
        }

    case OPC_PMULUW:
        tcg_gen_ext32u_i64(t0, t0);
        tcg_gen_ext32u_i64(t1, t1);
        tcg_gen_mul_i64(t0, t0, t1);
        break;

    case OPC_SEQU_CP2:
    case OPC_SEQ_CP2:
        cond = TCG_COND_EQ;
        goto do_cc_cond;
        break;
    case OPC_SLTU_CP2:
        cond = TCG_COND_LTU;
        goto do_cc_cond;
        break;
    case OPC_SLT_CP2:
        cond = TCG_COND_LT;
        goto do_cc_cond;
        break;
    case OPC_SLEU_CP2:
        cond = TCG_COND_LEU;
        goto do_cc_cond;
        break;
    case OPC_SLE_CP2:
        cond = TCG_COND_LE;
    do_cc_cond:
        {
            int cc = (ctx->opcode >> 8) & 0x7;
            TCGv_i64 t64 = tcg_temp_new_i64();
            TCGv_i32 t32 = tcg_temp_new_i32();

            tcg_gen_setcond_i64(cond, t64, t0, t1);
            tcg_gen_extrl_i64_i32(t32, t64);
            tcg_gen_deposit_i32(fpu_fcr31, fpu_fcr31, t32,
                                get_fp_bit(cc), 1);

            tcg_temp_free_i32(t32);
            tcg_temp_free_i64(t64);
        }
        goto no_rd;
        break;
    default:
        MIPS_INVAL("loongson_cp2");
        gen_reserved_instruction(ctx);
        return;
    }

    gen_store_fpr64(ctx, t0, rd);

no_rd:
    tcg_temp_free_i64(t0);
    tcg_temp_free_i64(t1);
}

static void gen_loongson_lswc2(DisasContext *ctx, int rt,
                               int rs, int rd)
{
    TCGv t0, t1, t2;
    TCGv_i32 fp0;
#if defined(TARGET_MIPS64)
    int lsq_rt1 = ctx->opcode & 0x1f;
    int lsq_offset = sextract32(ctx->opcode, 6, 9) << 4;
#endif
    int shf_offset = sextract32(ctx->opcode, 6, 8);

    t0 = tcg_temp_new();

    switch (MASK_LOONGSON_GSLSQ(ctx->opcode)) {
#if defined(TARGET_MIPS64)
    case OPC_GSLQ:
        t1 = tcg_temp_new();
        gen_base_offset_addr(ctx, t0, rs, lsq_offset);
        tcg_gen_qemu_ld_tl(t1, t0, ctx->mem_idx, MO_TEQ |
                           ctx->default_tcg_memop_mask);
        gen_base_offset_addr(ctx, t0, rs, lsq_offset + 8);
        tcg_gen_qemu_ld_tl(t0, t0, ctx->mem_idx, MO_TEQ |
                           ctx->default_tcg_memop_mask);
        gen_store_gpr(t1, rt);
        gen_store_gpr(t0, lsq_rt1);
        tcg_temp_free(t1);
        break;
    case OPC_GSLQC1:
        check_cp1_enabled(ctx);
        t1 = tcg_temp_new();
        gen_base_offset_addr(ctx, t0, rs, lsq_offset);
        tcg_gen_qemu_ld_tl(t1, t0, ctx->mem_idx, MO_TEQ |
                           ctx->default_tcg_memop_mask);
        gen_base_offset_addr(ctx, t0, rs, lsq_offset + 8);
        tcg_gen_qemu_ld_tl(t0, t0, ctx->mem_idx, MO_TEQ |
                           ctx->default_tcg_memop_mask);
        gen_store_fpr64(ctx, t1, rt);
        gen_store_fpr64(ctx, t0, lsq_rt1);
        tcg_temp_free(t1);
        break;
    case OPC_GSSQ:
        t1 = tcg_temp_new();
        gen_base_offset_addr(ctx, t0, rs, lsq_offset);
        gen_load_gpr(t1, rt);
        tcg_gen_qemu_st_tl(t1, t0, ctx->mem_idx, MO_TEQ |
                           ctx->default_tcg_memop_mask);
        gen_base_offset_addr(ctx, t0, rs, lsq_offset + 8);
        gen_load_gpr(t1, lsq_rt1);
        tcg_gen_qemu_st_tl(t1, t0, ctx->mem_idx, MO_TEQ |
                           ctx->default_tcg_memop_mask);
        tcg_temp_free(t1);
        break;
    case OPC_GSSQC1:
        check_cp1_enabled(ctx);
        t1 = tcg_temp_new();
        gen_base_offset_addr(ctx, t0, rs, lsq_offset);
        gen_load_fpr64(ctx, t1, rt);
        tcg_gen_qemu_st_tl(t1, t0, ctx->mem_idx, MO_TEQ |
                           ctx->default_tcg_memop_mask);
        gen_base_offset_addr(ctx, t0, rs, lsq_offset + 8);
        gen_load_fpr64(ctx, t1, lsq_rt1);
        tcg_gen_qemu_st_tl(t1, t0, ctx->mem_idx, MO_TEQ |
                           ctx->default_tcg_memop_mask);
        tcg_temp_free(t1);
        break;
#endif
    case OPC_GSSHFL:
        switch (MASK_LOONGSON_GSSHFLS(ctx->opcode)) {
        case OPC_GSLWLC1:
            check_cp1_enabled(ctx);
            gen_base_offset_addr(ctx, t0, rs, shf_offset);
            t1 = tcg_temp_new();
            tcg_gen_qemu_ld_tl(t1, t0, ctx->mem_idx, MO_UB);
            tcg_gen_andi_tl(t1, t0, 3);
#ifndef TARGET_WORDS_BIGENDIAN
            tcg_gen_xori_tl(t1, t1, 3);
#endif
            tcg_gen_shli_tl(t1, t1, 3);
            tcg_gen_andi_tl(t0, t0, ~3);
            tcg_gen_qemu_ld_tl(t0, t0, ctx->mem_idx, MO_TEUL);
            tcg_gen_shl_tl(t0, t0, t1);
            t2 = tcg_const_tl(-1);
            tcg_gen_shl_tl(t2, t2, t1);
            fp0 = tcg_temp_new_i32();
            gen_load_fpr32(ctx, fp0, rt);
            tcg_gen_ext_i32_tl(t1, fp0);
            tcg_gen_andc_tl(t1, t1, t2);
            tcg_temp_free(t2);
            tcg_gen_or_tl(t0, t0, t1);
            tcg_temp_free(t1);
#if defined(TARGET_MIPS64)
            tcg_gen_extrl_i64_i32(fp0, t0);
#else
            tcg_gen_ext32s_tl(fp0, t0);
#endif
            gen_store_fpr32(ctx, fp0, rt);
            tcg_temp_free_i32(fp0);
            break;
        case OPC_GSLWRC1:
            check_cp1_enabled(ctx);
            gen_base_offset_addr(ctx, t0, rs, shf_offset);
            t1 = tcg_temp_new();
            tcg_gen_qemu_ld_tl(t1, t0, ctx->mem_idx, MO_UB);
            tcg_gen_andi_tl(t1, t0, 3);
#ifdef TARGET_WORDS_BIGENDIAN
            tcg_gen_xori_tl(t1, t1, 3);
#endif
            tcg_gen_shli_tl(t1, t1, 3);
            tcg_gen_andi_tl(t0, t0, ~3);
            tcg_gen_qemu_ld_tl(t0, t0, ctx->mem_idx, MO_TEUL);
            tcg_gen_shr_tl(t0, t0, t1);
            tcg_gen_xori_tl(t1, t1, 31);
            t2 = tcg_const_tl(0xfffffffeull);
            tcg_gen_shl_tl(t2, t2, t1);
            fp0 = tcg_temp_new_i32();
            gen_load_fpr32(ctx, fp0, rt);
            tcg_gen_ext_i32_tl(t1, fp0);
            tcg_gen_and_tl(t1, t1, t2);
            tcg_temp_free(t2);
            tcg_gen_or_tl(t0, t0, t1);
            tcg_temp_free(t1);
#if defined(TARGET_MIPS64)
            tcg_gen_extrl_i64_i32(fp0, t0);
#else
            tcg_gen_ext32s_tl(fp0, t0);
#endif
            gen_store_fpr32(ctx, fp0, rt);
            tcg_temp_free_i32(fp0);
            break;
#if defined(TARGET_MIPS64)
        case OPC_GSLDLC1:
            check_cp1_enabled(ctx);
            gen_base_offset_addr(ctx, t0, rs, shf_offset);
            t1 = tcg_temp_new();
            tcg_gen_qemu_ld_tl(t1, t0, ctx->mem_idx, MO_UB);
            tcg_gen_andi_tl(t1, t0, 7);
#ifndef TARGET_WORDS_BIGENDIAN
            tcg_gen_xori_tl(t1, t1, 7);
#endif
            tcg_gen_shli_tl(t1, t1, 3);
            tcg_gen_andi_tl(t0, t0, ~7);
            tcg_gen_qemu_ld_tl(t0, t0, ctx->mem_idx, MO_TEQ);
            tcg_gen_shl_tl(t0, t0, t1);
            t2 = tcg_const_tl(-1);
            tcg_gen_shl_tl(t2, t2, t1);
            gen_load_fpr64(ctx, t1, rt);
            tcg_gen_andc_tl(t1, t1, t2);
            tcg_temp_free(t2);
            tcg_gen_or_tl(t0, t0, t1);
            tcg_temp_free(t1);
            gen_store_fpr64(ctx, t0, rt);
            break;
        case OPC_GSLDRC1:
            check_cp1_enabled(ctx);
            gen_base_offset_addr(ctx, t0, rs, shf_offset);
            t1 = tcg_temp_new();
            tcg_gen_qemu_ld_tl(t1, t0, ctx->mem_idx, MO_UB);
            tcg_gen_andi_tl(t1, t0, 7);
#ifdef TARGET_WORDS_BIGENDIAN
            tcg_gen_xori_tl(t1, t1, 7);
#endif
            tcg_gen_shli_tl(t1, t1, 3);
            tcg_gen_andi_tl(t0, t0, ~7);
            tcg_gen_qemu_ld_tl(t0, t0, ctx->mem_idx, MO_TEQ);
            tcg_gen_shr_tl(t0, t0, t1);
            tcg_gen_xori_tl(t1, t1, 63);
            t2 = tcg_const_tl(0xfffffffffffffffeull);
            tcg_gen_shl_tl(t2, t2, t1);
            gen_load_fpr64(ctx, t1, rt);
            tcg_gen_and_tl(t1, t1, t2);
            tcg_temp_free(t2);
            tcg_gen_or_tl(t0, t0, t1);
            tcg_temp_free(t1);
            gen_store_fpr64(ctx, t0, rt);
            break;
#endif
        default:
            MIPS_INVAL("loongson_gsshfl");
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case OPC_GSSHFS:
        switch (MASK_LOONGSON_GSSHFLS(ctx->opcode)) {
        case OPC_GSSWLC1:
            check_cp1_enabled(ctx);
            t1 = tcg_temp_new();
            gen_base_offset_addr(ctx, t0, rs, shf_offset);
            fp0 = tcg_temp_new_i32();
            gen_load_fpr32(ctx, fp0, rt);
            tcg_gen_ext_i32_tl(t1, fp0);
            gen_helper_0e2i(swl, t1, t0, ctx->mem_idx);
            tcg_temp_free_i32(fp0);
            tcg_temp_free(t1);
            break;
        case OPC_GSSWRC1:
            check_cp1_enabled(ctx);
            t1 = tcg_temp_new();
            gen_base_offset_addr(ctx, t0, rs, shf_offset);
            fp0 = tcg_temp_new_i32();
            gen_load_fpr32(ctx, fp0, rt);
            tcg_gen_ext_i32_tl(t1, fp0);
            gen_helper_0e2i(swr, t1, t0, ctx->mem_idx);
            tcg_temp_free_i32(fp0);
            tcg_temp_free(t1);
            break;
#if defined(TARGET_MIPS64)
        case OPC_GSSDLC1:
            check_cp1_enabled(ctx);
            t1 = tcg_temp_new();
            gen_base_offset_addr(ctx, t0, rs, shf_offset);
            gen_load_fpr64(ctx, t1, rt);
            gen_helper_0e2i(sdl, t1, t0, ctx->mem_idx);
            tcg_temp_free(t1);
            break;
        case OPC_GSSDRC1:
            check_cp1_enabled(ctx);
            t1 = tcg_temp_new();
            gen_base_offset_addr(ctx, t0, rs, shf_offset);
            gen_load_fpr64(ctx, t1, rt);
            gen_helper_0e2i(sdr, t1, t0, ctx->mem_idx);
            tcg_temp_free(t1);
            break;
#endif
        default:
            MIPS_INVAL("loongson_gsshfs");
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    default:
        MIPS_INVAL("loongson_gslsq");
        gen_reserved_instruction(ctx);
        break;
    }
    tcg_temp_free(t0);
}

/* Loongson EXT LDC2/SDC2 */
static void gen_loongson_lsdc2(DisasContext *ctx, int rt,
                               int rs, int rd)
{
    int offset = sextract32(ctx->opcode, 3, 8);
    uint32_t opc = MASK_LOONGSON_LSDC2(ctx->opcode);
    TCGv t0, t1;
    TCGv_i32 fp0;

    /* Pre-conditions */
    switch (opc) {
    case OPC_GSLBX:
    case OPC_GSLHX:
    case OPC_GSLWX:
    case OPC_GSLDX:
        /* prefetch, implement as NOP */
        if (rt == 0) {
            return;
        }
        break;
    case OPC_GSSBX:
    case OPC_GSSHX:
    case OPC_GSSWX:
    case OPC_GSSDX:
        break;
    case OPC_GSLWXC1:
#if defined(TARGET_MIPS64)
    case OPC_GSLDXC1:
#endif
        check_cp1_enabled(ctx);
        /* prefetch, implement as NOP */
        if (rt == 0) {
            return;
        }
        break;
    case OPC_GSSWXC1:
#if defined(TARGET_MIPS64)
    case OPC_GSSDXC1:
#endif
        check_cp1_enabled(ctx);
        break;
    default:
        MIPS_INVAL("loongson_lsdc2");
        gen_reserved_instruction(ctx);
        return;
        break;
    }

    t0 = tcg_temp_new();

    gen_base_offset_addr(ctx, t0, rs, offset);
    gen_op_addr_add(ctx, t0, cpu_gpr[rd], t0);

    switch (opc) {
    case OPC_GSLBX:
        tcg_gen_qemu_ld_tl(t0, t0, ctx->mem_idx, MO_SB);
        gen_store_gpr(t0, rt);
        break;
    case OPC_GSLHX:
        tcg_gen_qemu_ld_tl(t0, t0, ctx->mem_idx, MO_TESW |
                           ctx->default_tcg_memop_mask);
        gen_store_gpr(t0, rt);
        break;
    case OPC_GSLWX:
        gen_base_offset_addr(ctx, t0, rs, offset);
        if (rd) {
            gen_op_addr_add(ctx, t0, cpu_gpr[rd], t0);
        }
        tcg_gen_qemu_ld_tl(t0, t0, ctx->mem_idx, MO_TESL |
                           ctx->default_tcg_memop_mask);
        gen_store_gpr(t0, rt);
        break;
#if defined(TARGET_MIPS64)
    case OPC_GSLDX:
        gen_base_offset_addr(ctx, t0, rs, offset);
        if (rd) {
            gen_op_addr_add(ctx, t0, cpu_gpr[rd], t0);
        }
        tcg_gen_qemu_ld_tl(t0, t0, ctx->mem_idx, MO_TEQ |
                           ctx->default_tcg_memop_mask);
        gen_store_gpr(t0, rt);
        break;
#endif
    case OPC_GSLWXC1:
        check_cp1_enabled(ctx);
        gen_base_offset_addr(ctx, t0, rs, offset);
        if (rd) {
            gen_op_addr_add(ctx, t0, cpu_gpr[rd], t0);
        }
        fp0 = tcg_temp_new_i32();
        tcg_gen_qemu_ld_i32(fp0, t0, ctx->mem_idx, MO_TESL |
                            ctx->default_tcg_memop_mask);
        gen_store_fpr32(ctx, fp0, rt);
        tcg_temp_free_i32(fp0);
        break;
#if defined(TARGET_MIPS64)
    case OPC_GSLDXC1:
        check_cp1_enabled(ctx);
        gen_base_offset_addr(ctx, t0, rs, offset);
        if (rd) {
            gen_op_addr_add(ctx, t0, cpu_gpr[rd], t0);
        }
        tcg_gen_qemu_ld_tl(t0, t0, ctx->mem_idx, MO_TEQ |
                           ctx->default_tcg_memop_mask);
        gen_store_fpr64(ctx, t0, rt);
        break;
#endif
    case OPC_GSSBX:
        t1 = tcg_temp_new();
        gen_load_gpr(t1, rt);
        tcg_gen_qemu_st_tl(t1, t0, ctx->mem_idx, MO_SB);
        tcg_temp_free(t1);
        break;
    case OPC_GSSHX:
        t1 = tcg_temp_new();
        gen_load_gpr(t1, rt);
        tcg_gen_qemu_st_tl(t1, t0, ctx->mem_idx, MO_TEUW |
                           ctx->default_tcg_memop_mask);
        tcg_temp_free(t1);
        break;
    case OPC_GSSWX:
        t1 = tcg_temp_new();
        gen_load_gpr(t1, rt);
        tcg_gen_qemu_st_tl(t1, t0, ctx->mem_idx, MO_TEUL |
                           ctx->default_tcg_memop_mask);
        tcg_temp_free(t1);
        break;
#if defined(TARGET_MIPS64)
    case OPC_GSSDX:
        t1 = tcg_temp_new();
        gen_load_gpr(t1, rt);
        tcg_gen_qemu_st_tl(t1, t0, ctx->mem_idx, MO_TEQ |
                           ctx->default_tcg_memop_mask);
        tcg_temp_free(t1);
        break;
#endif
    case OPC_GSSWXC1:
        fp0 = tcg_temp_new_i32();
        gen_load_fpr32(ctx, fp0, rt);
        tcg_gen_qemu_st_i32(fp0, t0, ctx->mem_idx, MO_TEUL |
                            ctx->default_tcg_memop_mask);
        tcg_temp_free_i32(fp0);
        break;
#if defined(TARGET_MIPS64)
    case OPC_GSSDXC1:
        t1 = tcg_temp_new();
        gen_load_fpr64(ctx, t1, rt);
        tcg_gen_qemu_st_i64(t1, t0, ctx->mem_idx, MO_TEQ |
                            ctx->default_tcg_memop_mask);
        tcg_temp_free(t1);
        break;
#endif
    default:
        break;
    }

    tcg_temp_free(t0);
}
#endif /* !defined(TARGET_CHERI) */

/* Traps */
static void gen_trap(DisasContext *ctx, uint32_t opc,
                     int rs, int rt, int16_t imm)
{
    int cond;
    TCGv t0 = tcg_temp_new();
    TCGv t1 = tcg_temp_new();

    cond = 0;
    /* Load needed operands */
    switch (opc) {
    case OPC_TEQ:
    case OPC_TGE:
    case OPC_TGEU:
    case OPC_TLT:
    case OPC_TLTU:
    case OPC_TNE:
        /* Compare two registers */
        if (rs != rt) {
            gen_load_gpr(t0, rs);
            gen_load_gpr(t1, rt);
            cond = 1;
        }
        break;
    case OPC_TEQI:
    case OPC_TGEI:
    case OPC_TGEIU:
    case OPC_TLTI:
    case OPC_TLTIU:
    case OPC_TNEI:
        /* Compare register to immediate */
        if (rs != 0 || imm != 0) {
            gen_load_gpr(t0, rs);
            tcg_gen_movi_tl(t1, (int32_t)imm);
            cond = 1;
        }
        break;
    }
    if (cond == 0) {
        switch (opc) {
        case OPC_TEQ:   /* rs == rs */
        case OPC_TEQI:  /* r0 == 0  */
        case OPC_TGE:   /* rs >= rs */
        case OPC_TGEI:  /* r0 >= 0  */
        case OPC_TGEU:  /* rs >= rs unsigned */
        case OPC_TGEIU: /* r0 >= 0  unsigned */
            /* Always trap */
            generate_exception_end(ctx, EXCP_TRAP);
            break;
        case OPC_TLT:   /* rs < rs           */
        case OPC_TLTI:  /* r0 < 0            */
        case OPC_TLTU:  /* rs < rs unsigned  */
        case OPC_TLTIU: /* r0 < 0  unsigned  */
        case OPC_TNE:   /* rs != rs          */
        case OPC_TNEI:  /* r0 != 0           */
            /* Never trap: treat as NOP. */
            break;
        }
    } else {
        TCGLabel *l1 = gen_new_label();

        switch (opc) {
        case OPC_TEQ:
        case OPC_TEQI:
            tcg_gen_brcond_tl(TCG_COND_NE, t0, t1, l1);
            break;
        case OPC_TGE:
        case OPC_TGEI:
            tcg_gen_brcond_tl(TCG_COND_LT, t0, t1, l1);
            break;
        case OPC_TGEU:
        case OPC_TGEIU:
            tcg_gen_brcond_tl(TCG_COND_LTU, t0, t1, l1);
            break;
        case OPC_TLT:
        case OPC_TLTI:
            tcg_gen_brcond_tl(TCG_COND_GE, t0, t1, l1);
            break;
        case OPC_TLTU:
        case OPC_TLTIU:
            tcg_gen_brcond_tl(TCG_COND_GEU, t0, t1, l1);
            break;
        case OPC_TNE:
        case OPC_TNEI:
            tcg_gen_brcond_tl(TCG_COND_EQ, t0, t1, l1);
            break;
        }
        generate_exception(ctx, EXCP_TRAP);
        gen_set_label(l1);
    }
    tcg_temp_free(t0);
    tcg_temp_free(t1);
}

static inline bool use_goto_tb(DisasContext *ctx, target_ulong dest)
{
    if (unlikely(ctx->base.singlestep_enabled)) {
        return false;
    }

#ifndef CONFIG_USER_ONLY
    return (ctx->base.tb->pc & TARGET_PAGE_MASK) == (dest & TARGET_PAGE_MASK);
#else
    return true;
#endif
}

static inline void gen_goto_tb(DisasContext *ctx, int n, target_ulong dest)
{
    if (use_goto_tb(ctx, dest)) {
        tcg_gen_goto_tb(n);
        gen_save_pc(dest);
        tcg_gen_exit_tb(ctx->base.tb, n);
    } else {
        gen_save_pc(dest);
        if (ctx->base.singlestep_enabled) {
            save_cpu_state(ctx, 0);
            gen_helper_raise_exception_debug(cpu_env);
        }
        tcg_gen_lookup_and_goto_ptr();
    }
}

/* Branches (before delay slot) */
static void gen_compute_branch(DisasContext *ctx, uint32_t opc,
                               int insn_bytes,
                               int rs, int rt, int32_t offset,
                               int delayslot_size)
{
    target_ulong btgt = -1;
    int blink = 0;
    int bcond_compute = 0;
    TCGv t0 = tcg_temp_new();
    TCGv t1 = tcg_temp_new();

    // Note: For CHERI btgt is an absolute address not an offset relative
    // to PCC.base.
#if defined(TARGET_CHERI)
    bool btarget_checked = false;
    // Some debug assertions to ensure all branch cases are bounds checked
#define SET_BTARGET_CHECKED(value) \
    do { tcg_debug_assert(!btarget_checked); btarget_checked = value; } while (false)
#else
#define SET_BTARGET_CHECKED(value)
#endif // defined(TARGET_CHERI)

    if (ctx->hflags & MIPS_HFLAG_BMASK) {
#ifdef MIPS_DEBUG_DISAS
        LOG_DISAS("Branch in delay / forbidden slot at PC 0x"
                  TARGET_FMT_lx "\n", ctx->base.pc_next);
#endif
        gen_reserved_instruction(ctx);
        SET_BTARGET_CHECKED(true); // exception raised -> no need to check
        goto out;
    }

    /* Load needed operands */
    switch (opc) {
    case OPC_BEQ:
    case OPC_BEQL:
    case OPC_BNE:
    case OPC_BNEL:
        /* Compare two registers */
        if (rs != rt) {
            gen_load_gpr(t0, rs);
            gen_load_gpr(t1, rt);
            bcond_compute = 1;
        }
        btgt = ctx->base.pc_next + insn_bytes + offset;
        break;
    case OPC_BGEZ:
    case OPC_BGEZAL:
    case OPC_BGEZALL:
    case OPC_BGEZL:
    case OPC_BGTZ:
    case OPC_BGTZL:
    case OPC_BLEZ:
    case OPC_BLEZL:
    case OPC_BLTZ:
    case OPC_BLTZAL:
    case OPC_BLTZALL:
    case OPC_BLTZL:
        /* Compare to zero */
        if (rs != 0) {
            gen_load_gpr(t0, rs);
            bcond_compute = 1;
        }
        btgt = ctx->base.pc_next + insn_bytes + offset;
        break;
    case OPC_BPOSGE32:
#if defined(TARGET_MIPS64)
    case OPC_BPOSGE64:
        tcg_gen_andi_tl(t0, cpu_dspctrl, 0x7F);
#else
        tcg_gen_andi_tl(t0, cpu_dspctrl, 0x3F);
#endif
        bcond_compute = 1;
        btgt = ctx->base.pc_next + insn_bytes + offset;
        break;
    case OPC_J:
    case OPC_JAL:
#ifndef TARGET_CHERI
    case OPC_JALX:
#endif
        /* Jump to immediate (taking PCC.base into account) */
        btgt = (((ctx->base.pc_next + insn_bytes - pcc_reloc(ctx)) &
                 (int32_t)0xF0000000) |
                (uint32_t)offset) +
               pcc_reloc(ctx);
        break;
    case OPC_JR:
    case OPC_JALR:
        /* Jump to register */
        if (offset != 0 && offset != 16) {
            /*
             * Hint = 0 is JR/JALR, hint 16 is JR.HB/JALR.HB, the
             * others are reserved.
             */
            MIPS_INVAL("jump hint");
            gen_reserved_instruction(ctx);
            SET_BTARGET_CHECKED(true); // exception raised -> no need to check
            goto out;
        }
        gen_load_gpr(btarget, rs);
#ifdef TARGET_CHERI
        /* Add PCC.base to rs (jr/jalr is relative to PCC) */
        tcg_gen_addi_tl(btarget, btarget, pcc_reloc(ctx));
#endif /* TARGET_CHERI */
        gen_check_branch_target_dynamic(ctx, btarget);
        SET_BTARGET_CHECKED(true);
        break;
    default:
        MIPS_INVAL("branch/jump");
        gen_reserved_instruction(ctx);
        SET_BTARGET_CHECKED(true); // exception raised -> no need to check
        goto out;
    }
    if (bcond_compute == 0) {
        /* No condition to be computed */
        switch (opc) {
        case OPC_BEQ:     /* rx == rx        */
        case OPC_BEQL:    /* rx == rx likely */
        case OPC_BGEZ:    /* 0 >= 0          */
        case OPC_BGEZL:   /* 0 >= 0 likely   */
        case OPC_BLEZ:    /* 0 <= 0          */
        case OPC_BLEZL:   /* 0 <= 0 likely   */
            /* Always take */
            ctx->hflags |= MIPS_HFLAG_B;
            break;
        case OPC_BGEZAL:  /* 0 >= 0          */
        case OPC_BGEZALL: /* 0 >= 0 likely   */
            /* Always take and link */
            blink = 31;
            ctx->hflags |= MIPS_HFLAG_B;
            break;
        case OPC_BNE:     /* rx != rx        */
        case OPC_BGTZ:    /* 0 > 0           */
        case OPC_BLTZ:    /* 0 < 0           */
            /* Treat as NOP. */
            SET_BTARGET_CHECKED(true); // not taken -> no need to check
            goto out;
        case OPC_BLTZAL:  /* 0 < 0           */
            /*
             * Handle as an unconditional branch to get correct delay
             * slot checking.
             */
            blink = 31;
            btgt = ctx->base.pc_next + insn_bytes + delayslot_size;
            ctx->hflags |= MIPS_HFLAG_B;
            break;
        case OPC_BLTZALL: /* 0 < 0 likely */
            /* For CHERI, we have to subtract PCC.base from r31 */
            tcg_gen_movi_tl(cpu_gpr[31],
                            ctx->base.pc_next + 8 - pcc_reloc(ctx));
            gen_log_instr_gpr_update(ctx, 31);
            /* Skip the instruction in the delay slot */
            ctx->base.pc_next += 4;
            SET_BTARGET_CHECKED(true); // not taken -> no need to check
            goto out;
        case OPC_BNEL:    /* rx != rx likely */
        case OPC_BGTZL:   /* 0 > 0 likely */
        case OPC_BLTZL:   /* 0 < 0 likely */
            /* Skip the instruction in the delay slot */
            ctx->base.pc_next += 4;
            SET_BTARGET_CHECKED(true); // not taken -> no need to check
            goto out;
        case OPC_J:
            ctx->hflags |= MIPS_HFLAG_B;
            break;
#ifndef TARGET_CHERI
        case OPC_JALX:
            ctx->hflags |= MIPS_HFLAG_BX;
            /* Fallthrough */
#endif
        case OPC_JAL:
            blink = 31;
            ctx->hflags |= MIPS_HFLAG_B;
            break;
        case OPC_JR:
            ctx->hflags |= MIPS_HFLAG_BR;
            break;
        case OPC_JALR:
            blink = rt;
            ctx->hflags |= MIPS_HFLAG_BR;
            break;
        default:
            MIPS_INVAL("branch/jump");
            gen_reserved_instruction(ctx);
            SET_BTARGET_CHECKED(true); // exception raised -> no need to check
            goto out;
        }
    } else {
        switch (opc) {
        case OPC_BEQ:
            tcg_gen_setcond_tl(TCG_COND_EQ, bcond, t0, t1);
            goto not_likely;
        case OPC_BEQL:
            tcg_gen_setcond_tl(TCG_COND_EQ, bcond, t0, t1);
            goto likely;
        case OPC_BNE:
            tcg_gen_setcond_tl(TCG_COND_NE, bcond, t0, t1);
            goto not_likely;
        case OPC_BNEL:
            tcg_gen_setcond_tl(TCG_COND_NE, bcond, t0, t1);
            goto likely;
        case OPC_BGEZ:
            tcg_gen_setcondi_tl(TCG_COND_GE, bcond, t0, 0);
            goto not_likely;
        case OPC_BGEZL:
            tcg_gen_setcondi_tl(TCG_COND_GE, bcond, t0, 0);
            goto likely;
        case OPC_BGEZAL:
            tcg_gen_setcondi_tl(TCG_COND_GE, bcond, t0, 0);
            blink = 31;
            goto not_likely;
        case OPC_BGEZALL:
            tcg_gen_setcondi_tl(TCG_COND_GE, bcond, t0, 0);
            blink = 31;
            goto likely;
        case OPC_BGTZ:
            tcg_gen_setcondi_tl(TCG_COND_GT, bcond, t0, 0);
            goto not_likely;
        case OPC_BGTZL:
            tcg_gen_setcondi_tl(TCG_COND_GT, bcond, t0, 0);
            goto likely;
        case OPC_BLEZ:
            tcg_gen_setcondi_tl(TCG_COND_LE, bcond, t0, 0);
            goto not_likely;
        case OPC_BLEZL:
            tcg_gen_setcondi_tl(TCG_COND_LE, bcond, t0, 0);
            goto likely;
        case OPC_BLTZ:
            tcg_gen_setcondi_tl(TCG_COND_LT, bcond, t0, 0);
            goto not_likely;
        case OPC_BLTZL:
            tcg_gen_setcondi_tl(TCG_COND_LT, bcond, t0, 0);
            goto likely;
        case OPC_BPOSGE32:
            tcg_gen_setcondi_tl(TCG_COND_GE, bcond, t0, 32);
            goto not_likely;
#if defined(TARGET_MIPS64)
        case OPC_BPOSGE64:
            tcg_gen_setcondi_tl(TCG_COND_GE, bcond, t0, 64);
            goto not_likely;
#endif
        case OPC_BLTZAL:
            tcg_gen_setcondi_tl(TCG_COND_LT, bcond, t0, 0);
            blink = 31;
        not_likely:
            ctx->hflags |= MIPS_HFLAG_BC;
            break;
        case OPC_BLTZALL:
            tcg_gen_setcondi_tl(TCG_COND_LT, bcond, t0, 0);
            blink = 31;
        likely:
            ctx->hflags |= MIPS_HFLAG_BL;
            break;
        default:
            MIPS_INVAL("conditional branch/jump");
            gen_reserved_instruction(ctx);
            SET_BTARGET_CHECKED(true); // not taken -> no need to check
            goto out;
        }
    }

    ctx->btarget = btgt;

#ifdef TARGET_CHERI
    if (bcond_compute) {
        // Check that the conditional branch target is in range (but only if the branch is taken)
        tcg_debug_assert(btgt != -1 && "btgt should have been set!");
        gen_check_cond_branch_target(ctx, bcond, btgt);
        SET_BTARGET_CHECKED(true);
    } else if (!btarget_checked) {
        tcg_debug_assert(btgt != -1 && "btgt should have been set!");
        gen_check_branch_target(ctx, btgt);
        SET_BTARGET_CHECKED(true);
    }
#endif

    switch (delayslot_size) {
    case 2:
        ctx->hflags |= MIPS_HFLAG_BDS16;
        break;
    case 4:
        ctx->hflags |= MIPS_HFLAG_BDS32;
        break;
    }

    if (blink > 0) {
        int post_delay = insn_bytes + delayslot_size;
        int lowbit = !!(ctx->hflags & MIPS_HFLAG_M16);

        tcg_gen_movi_tl(cpu_gpr[blink],
                        /* Subtract PCC.base from r[blink] */
                        ctx->base.pc_next + post_delay + lowbit -
                            pcc_reloc(ctx));
        gen_log_instr_gpr_update(ctx, blink);
    }

 out:
    if (insn_bytes == 2) {
        ctx->hflags |= MIPS_HFLAG_B16;
    }
    tcg_temp_free(t0);
    tcg_temp_free(t1);
#ifdef TARGET_CHERI
    tcg_debug_assert(btarget_checked);
#endif

}

#ifndef TARGET_CHERI
/* nanoMIPS Branches */
static void gen_compute_branch_nm(DisasContext *ctx, uint32_t opc,
                                int insn_bytes,
                                int rs, int rt, int32_t offset)
{
    target_ulong btgt = -1;
    int bcond_compute = 0;
    TCGv t0 = tcg_temp_new();
    TCGv t1 = tcg_temp_new();

    /* Load needed operands */
    switch (opc) {
    case OPC_BEQ:
    case OPC_BNE:
        /* Compare two registers */
        if (rs != rt) {
            gen_load_gpr(t0, rs);
            gen_load_gpr(t1, rt);
            bcond_compute = 1;
        }
        btgt = ctx->base.pc_next + insn_bytes + offset;
        break;
    case OPC_BGEZAL:
        /* Compare to zero */
        if (rs != 0) {
            gen_load_gpr(t0, rs);
            bcond_compute = 1;
        }
        btgt = ctx->base.pc_next + insn_bytes + offset;
        break;
    case OPC_BPOSGE32:
        tcg_gen_andi_tl(t0, cpu_dspctrl, 0x3F);
        bcond_compute = 1;
        btgt = ctx->base.pc_next + insn_bytes + offset;
        break;
    case OPC_JR:
    case OPC_JALR:
        /* Jump to register */
        if (offset != 0 && offset != 16) {
            /*
             * Hint = 0 is JR/JALR, hint 16 is JR.HB/JALR.HB, the
             * others are reserved.
             */
            MIPS_INVAL("jump hint");
            gen_reserved_instruction(ctx);
            goto out;
        }
        gen_load_gpr(btarget, rs);
        break;
    default:
        MIPS_INVAL("branch/jump");
        gen_reserved_instruction(ctx);
        goto out;
    }
    if (bcond_compute == 0) {
        /* No condition to be computed */
        switch (opc) {
        case OPC_BEQ:     /* rx == rx        */
            /* Always take */
            ctx->hflags |= MIPS_HFLAG_B;
            break;
        case OPC_BGEZAL:  /* 0 >= 0          */
            /* Always take and link */
            tcg_gen_movi_tl(cpu_gpr[31],
                            ctx->base.pc_next + insn_bytes);
            gen_log_instr_gpr_update(ctx, 31);
            ctx->hflags |= MIPS_HFLAG_B;
            break;
        case OPC_BNE:     /* rx != rx        */
            tcg_gen_movi_tl(cpu_gpr[31], ctx->base.pc_next + 8);
            gen_log_instr_gpr_update(ctx, 31);
            /* Skip the instruction in the delay slot */
            ctx->base.pc_next += 4;
            goto out;
        case OPC_JR:
            ctx->hflags |= MIPS_HFLAG_BR;
            break;
        case OPC_JALR:
            if (rt > 0) {
                tcg_gen_movi_tl(cpu_gpr[rt],
                                ctx->base.pc_next + insn_bytes);
                gen_log_instr_gpr_update(ctx, 31);
            }
            ctx->hflags |= MIPS_HFLAG_BR;
            break;
        default:
            MIPS_INVAL("branch/jump");
            gen_reserved_instruction(ctx);
            goto out;
        }
    } else {
        switch (opc) {
        case OPC_BEQ:
            tcg_gen_setcond_tl(TCG_COND_EQ, bcond, t0, t1);
            goto not_likely;
        case OPC_BNE:
            tcg_gen_setcond_tl(TCG_COND_NE, bcond, t0, t1);
            goto not_likely;
        case OPC_BGEZAL:
            tcg_gen_setcondi_tl(TCG_COND_GE, bcond, t0, 0);
            tcg_gen_movi_tl(cpu_gpr[31],
                            ctx->base.pc_next + insn_bytes);
            gen_log_instr_gpr_update(ctx, 31);
            goto not_likely;
        case OPC_BPOSGE32:
            tcg_gen_setcondi_tl(TCG_COND_GE, bcond, t0, 32);
        not_likely:
            ctx->hflags |= MIPS_HFLAG_BC;
            break;
        default:
            MIPS_INVAL("conditional branch/jump");
            gen_reserved_instruction(ctx);
            goto out;
        }
    }

    ctx->btarget = btgt;

 out:
    if (insn_bytes == 2) {
        ctx->hflags |= MIPS_HFLAG_B16;
    }
    tcg_temp_free(t0);
    tcg_temp_free(t1);
}
#endif

/* special3 bitfield operations */
static void gen_bitops(DisasContext *ctx, uint32_t opc, int rt,
                       int rs, int lsb, int msb)
{
    TCGv t0 = tcg_temp_new();
    TCGv t1 = tcg_temp_new();

    gen_load_gpr(t1, rs);
    switch (opc) {
    case OPC_EXT:
        if (lsb + msb > 31) {
            goto fail;
        }
        if (msb != 31) {
            tcg_gen_extract_tl(t0, t1, lsb, msb + 1);
        } else {
            /*
             * The two checks together imply that lsb == 0,
             * so this is a simple sign-extension.
             */
            tcg_gen_ext32s_tl(t0, t1);
        }
        break;
#if defined(TARGET_MIPS64)
    case OPC_DEXTU:
        lsb += 32;
        goto do_dext;
    case OPC_DEXTM:
        msb += 32;
        goto do_dext;
    case OPC_DEXT:
    do_dext:
        if (lsb + msb > 63) {
            goto fail;
        }
        tcg_gen_extract_tl(t0, t1, lsb, msb + 1);
        break;
#endif
    case OPC_INS:
        if (lsb > msb) {
            goto fail;
        }
        gen_load_gpr(t0, rt);
        tcg_gen_deposit_tl(t0, t0, t1, lsb, msb - lsb + 1);
        tcg_gen_ext32s_tl(t0, t0);
        break;
#if defined(TARGET_MIPS64)
    case OPC_DINSU:
        lsb += 32;
        /* FALLTHRU */
    case OPC_DINSM:
        msb += 32;
        /* FALLTHRU */
    case OPC_DINS:
        if (lsb > msb) {
            goto fail;
        }
        gen_load_gpr(t0, rt);
        tcg_gen_deposit_tl(t0, t0, t1, lsb, msb - lsb + 1);
        break;
#endif
    default:
fail:
        MIPS_INVAL("bitops");
        gen_reserved_instruction(ctx);
        tcg_temp_free(t0);
        tcg_temp_free(t1);
        return;
    }
    gen_store_gpr(t0, rt);
    tcg_temp_free(t0);
    tcg_temp_free(t1);
}

static void gen_bshfl(DisasContext *ctx, uint32_t op2, int rt, int rd)
{
    TCGv t0;

    if (rd == 0) {
        /* If no destination, treat it as a NOP. */
        return;
    }

    t0 = tcg_temp_new();
    gen_load_gpr(t0, rt);
    switch (op2) {
    case OPC_WSBH:
        {
            TCGv t1 = tcg_temp_new();
            TCGv t2 = tcg_const_tl(0x00FF00FF);

            tcg_gen_shri_tl(t1, t0, 8);
            tcg_gen_and_tl(t1, t1, t2);
            tcg_gen_and_tl(t0, t0, t2);
            tcg_gen_shli_tl(t0, t0, 8);
            tcg_gen_or_tl(t0, t0, t1);
            tcg_temp_free(t2);
            tcg_temp_free(t1);
            tcg_gen_ext32s_tl(cpu_gpr[rd], t0);
        }
        break;
    case OPC_SEB:
        tcg_gen_ext8s_tl(cpu_gpr[rd], t0);
        break;
    case OPC_SEH:
        tcg_gen_ext16s_tl(cpu_gpr[rd], t0);
        break;
#if defined(TARGET_MIPS64)
    case OPC_DSBH:
        {
            TCGv t1 = tcg_temp_new();
            TCGv t2 = tcg_const_tl(0x00FF00FF00FF00FFULL);

            tcg_gen_shri_tl(t1, t0, 8);
            tcg_gen_and_tl(t1, t1, t2);
            tcg_gen_and_tl(t0, t0, t2);
            tcg_gen_shli_tl(t0, t0, 8);
            tcg_gen_or_tl(cpu_gpr[rd], t0, t1);
            tcg_temp_free(t2);
            tcg_temp_free(t1);
        }
        break;
    case OPC_DSHD:
        {
            TCGv t1 = tcg_temp_new();
            TCGv t2 = tcg_const_tl(0x0000FFFF0000FFFFULL);

            tcg_gen_shri_tl(t1, t0, 16);
            tcg_gen_and_tl(t1, t1, t2);
            tcg_gen_and_tl(t0, t0, t2);
            tcg_gen_shli_tl(t0, t0, 16);
            tcg_gen_or_tl(t0, t0, t1);
            tcg_gen_shri_tl(t1, t0, 32);
            tcg_gen_shli_tl(t0, t0, 32);
            tcg_gen_or_tl(cpu_gpr[rd], t0, t1);
            tcg_temp_free(t2);
            tcg_temp_free(t1);
        }
        break;
#endif
    default:
        MIPS_INVAL("bsfhl");
        gen_reserved_instruction(ctx);
        tcg_temp_free(t0);
        return;
    }
    gen_log_instr_gpr_update(ctx, rd);
    tcg_temp_free(t0);
}

static void gen_align_bits(DisasContext *ctx, int wordsz, int rd, int rs,
                           int rt, int bits)
{
    TCGv t0;
    if (rd == 0) {
        /* Treat as NOP. */
        return;
    }
    t0 = tcg_temp_new();
    if (bits == 0 || bits == wordsz) {
        if (bits == 0) {
            gen_load_gpr(t0, rt);
        } else {
            gen_load_gpr(t0, rs);
        }
        switch (wordsz) {
        case 32:
            tcg_gen_ext32s_tl(cpu_gpr[rd], t0);
            break;
#if defined(TARGET_MIPS64)
        case 64:
            tcg_gen_mov_tl(cpu_gpr[rd], t0);
            break;
#endif
        }
    } else {
        TCGv t1 = tcg_temp_new();
        gen_load_gpr(t0, rt);
        gen_load_gpr(t1, rs);
        switch (wordsz) {
        case 32:
            {
                TCGv_i64 t2 = tcg_temp_new_i64();
                tcg_gen_concat_tl_i64(t2, t1, t0);
                tcg_gen_shri_i64(t2, t2, 32 - bits);
                gen_move_low32(cpu_gpr[rd], t2);
                tcg_temp_free_i64(t2);
            }
            break;
#if defined(TARGET_MIPS64)
        case 64:
            tcg_gen_shli_tl(t0, t0, bits);
            tcg_gen_shri_tl(t1, t1, 64 - bits);
            tcg_gen_or_tl(cpu_gpr[rd], t1, t0);
            break;
#endif
        }
        tcg_temp_free(t1);
    }
    gen_log_instr_gpr_update(ctx, rd);

    tcg_temp_free(t0);
}

static void gen_align(DisasContext *ctx, int wordsz, int rd, int rs, int rt,
                      int bp)
{
    gen_align_bits(ctx, wordsz, rd, rs, rt, bp * 8);
}

#ifndef TARGET_CHERI
static void gen_ext(DisasContext *ctx, int wordsz, int rd, int rs, int rt,
                    int shift)
{
    gen_align_bits(ctx, wordsz, rd, rs, rt, wordsz - shift);
}
#endif

static void gen_bitswap(DisasContext *ctx, int opc, int rd, int rt)
{
    TCGv t0;
    if (rd == 0) {
        /* Treat as NOP. */
        return;
    }
    t0 = tcg_temp_new();
    gen_load_gpr(t0, rt);
    switch (opc) {
    case OPC_BITSWAP:
        gen_helper_bitswap(cpu_gpr[rd], t0);
        break;
#if defined(TARGET_MIPS64)
    case OPC_DBITSWAP:
        gen_helper_dbitswap(cpu_gpr[rd], t0);
        break;
#endif
    }
    gen_log_instr_gpr_update(ctx, rd);

    tcg_temp_free(t0);
}

#ifndef CONFIG_USER_ONLY
/* CP0 (MMU and control) */
static inline void gen_mthc0_entrylo(TCGv arg, target_ulong off)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();

    tcg_gen_ext_tl_i64(t0, arg);
    tcg_gen_ld_i64(t1, cpu_env, off);
#if defined(TARGET_MIPS64)
    tcg_gen_deposit_i64(t1, t1, t0, 30, 32);
#else
    tcg_gen_concat32_i64(t1, t1, t0);
#endif
    tcg_gen_st_i64(t1, cpu_env, off);
    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
}

static inline void gen_mthc0_store64(TCGv arg, target_ulong off)
{
    TCGv_i64 t0 = tcg_temp_new_i64();
    TCGv_i64 t1 = tcg_temp_new_i64();

    tcg_gen_ext_tl_i64(t0, arg);
    tcg_gen_ld_i64(t1, cpu_env, off);
    tcg_gen_concat32_i64(t1, t1, t0);
    tcg_gen_st_i64(t1, cpu_env, off);
    tcg_temp_free_i64(t1);
    tcg_temp_free_i64(t0);
}

static inline void gen_mfhc0_entrylo(TCGv arg, target_ulong off)
{
    TCGv_i64 t0 = tcg_temp_new_i64();

    tcg_gen_ld_i64(t0, cpu_env, off);
#if defined(TARGET_MIPS64)
    tcg_gen_shri_i64(t0, t0, 30);
#else
    tcg_gen_shri_i64(t0, t0, 32);
#endif
    gen_move_low32(arg, t0);
    tcg_temp_free_i64(t0);
}

static inline void gen_mfhc0_load64(TCGv arg, target_ulong off, int shift)
{
    TCGv_i64 t0 = tcg_temp_new_i64();

    tcg_gen_ld_i64(t0, cpu_env, off);
    tcg_gen_shri_i64(t0, t0, 32 + shift);
    gen_move_low32(arg, t0);
    tcg_temp_free_i64(t0);
}

static inline void gen_mfc0_load32(TCGv arg, target_ulong off)
{
    TCGv_i32 t0 = tcg_temp_new_i32();

    tcg_gen_ld_i32(t0, cpu_env, off);
    tcg_gen_ext_i32_tl(arg, t0);
    tcg_temp_free_i32(t0);
}

static inline void gen_mfc0_load64(TCGv arg, target_ulong off)
{
    tcg_gen_ld_tl(arg, cpu_env, off);
    tcg_gen_ext32s_tl(arg, arg);
}

static inline void gen_mtc0_store32(
    DisasContext *ctx, TCGv arg, int reg, int sel, target_ulong off)
{
    TCGv_i32 t0 = tcg_temp_new_i32();

    tcg_gen_trunc_tl_i32(t0, arg);
    tcg_gen_st_i32(t0, cpu_env, off);
    tcg_temp_free_i32(t0);
    gen_log_instr_cop0_update(ctx, reg, sel, off);
}

static inline void gen_mtc0_store(
    DisasContext *ctx, TCGv arg, int reg, int sel, target_ulong off)
{
    tcg_gen_st_tl(arg, cpu_env, off);
    gen_log_instr_cop0_update(ctx, reg, sel, off);
}

#define CP0_CHECK(c)                            \
    do {                                        \
        if (!(c)) {                             \
            goto cp0_unimplemented;             \
        }                                       \
    } while (0)

static void gen_mfhc0(DisasContext *ctx, TCGv arg, int reg, int sel)
{
    const char *register_name = "invalid";

    switch (reg) {
    case CP0_REGISTER_02:
        switch (sel) {
        case 0:
            CP0_CHECK(ctx->hflags & MIPS_HFLAG_ELPA);
            gen_mfhc0_entrylo(arg, offsetof(CPUMIPSState, CP0_EntryLo0));
            register_name = "EntryLo0";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_03:
        switch (sel) {
        case CP0_REG03__ENTRYLO1:
            CP0_CHECK(ctx->hflags & MIPS_HFLAG_ELPA);
            gen_mfhc0_entrylo(arg, offsetof(CPUMIPSState, CP0_EntryLo1));
            register_name = "EntryLo1";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_09:
        switch (sel) {
#if !defined(TARGET_CHERI) /* SAAR Conflicts with RTC */
        case CP0_REG09__SAAR:
            CP0_CHECK(ctx->saar);
            gen_helper_mfhc0_saar(arg, cpu_env);
            register_name = "SAAR";
            break;
#endif
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_17:
        switch (sel) {
        case CP0_REG17__LLADDR:
            gen_mfhc0_load64(arg, offsetof(CPUMIPSState, CP0_LLAddr),
                             ctx->CP0_LLAddr_shift);
            register_name = "LLAddr";
            break;
        case CP0_REG17__MAAR:
            CP0_CHECK(ctx->mrp);
            gen_helper_mfhc0_maar(arg, cpu_env);
            register_name = "MAAR";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_19:
        switch (sel) {
        case CP0_REG19__WATCHHI0:
        case CP0_REG19__WATCHHI1:
        case CP0_REG19__WATCHHI2:
        case CP0_REG19__WATCHHI3:
        case CP0_REG19__WATCHHI4:
        case CP0_REG19__WATCHHI5:
        case CP0_REG19__WATCHHI6:
        case CP0_REG19__WATCHHI7:
            /* upper 32 bits are only available when Config5MI != 0 */
            CP0_CHECK(ctx->mi);
            gen_mfhc0_load64(arg, offsetof(CPUMIPSState, CP0_WatchHi[sel]), 0);
            register_name = "WatchHi";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_28:
        switch (sel) {
        case 0:
        case 2:
        case 4:
        case 6:
            gen_mfhc0_load64(arg, offsetof(CPUMIPSState, CP0_TagLo), 0);
            register_name = "TagLo";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    default:
        goto cp0_unimplemented;
    }
    trace_mips_translate_c0("mfhc0", register_name, reg, sel);
    return;

cp0_unimplemented:
    qemu_log_mask(LOG_UNIMP, "mfhc0 %s (reg %d sel %d)\n",
                  register_name, reg, sel);
    tcg_gen_movi_tl(arg, 0);
}

static void gen_mthc0(DisasContext *ctx, TCGv arg, int reg, int sel)
{
    const char *register_name = "invalid";
    uint64_t mask = ctx->PAMask >> 36;

    switch (reg) {
    case CP0_REGISTER_02:
        switch (sel) {
        case 0:
            CP0_CHECK(ctx->hflags & MIPS_HFLAG_ELPA);
            tcg_gen_andi_tl(arg, arg, mask);
            gen_mthc0_entrylo(arg, offsetof(CPUMIPSState, CP0_EntryLo0));
            register_name = "EntryLo0";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_03:
        switch (sel) {
        case CP0_REG03__ENTRYLO1:
            CP0_CHECK(ctx->hflags & MIPS_HFLAG_ELPA);
            tcg_gen_andi_tl(arg, arg, mask);
            gen_mthc0_entrylo(arg, offsetof(CPUMIPSState, CP0_EntryLo1));
            register_name = "EntryLo1";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_09:
        switch (sel) {
#if !defined(TARGET_CHERI) /* SAAR Conflicts with RTC */
        case CP0_REG09__SAAR:
            CP0_CHECK(ctx->saar);
            gen_helper_mthc0_saar(cpu_env, arg);
            register_name = "SAAR";
            break;
#endif
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_17:
        switch (sel) {
        case CP0_REG17__LLADDR:
            /*
             * LLAddr is read-only (the only exception is bit 0 if LLB is
             * supported); the CP0_LLAddr_rw_bitmask does not seem to be
             * relevant for modern MIPS cores supporting MTHC0, therefore
             * treating MTHC0 to LLAddr as NOP.
             */
            register_name = "LLAddr";
            break;
        case CP0_REG17__MAAR:
            CP0_CHECK(ctx->mrp);
            gen_helper_mthc0_maar(cpu_env, arg);
            register_name = "MAAR";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_19:
        switch (sel) {
        case CP0_REG19__WATCHHI0:
        case CP0_REG19__WATCHHI1:
        case CP0_REG19__WATCHHI2:
        case CP0_REG19__WATCHHI3:
        case CP0_REG19__WATCHHI4:
        case CP0_REG19__WATCHHI5:
        case CP0_REG19__WATCHHI6:
        case CP0_REG19__WATCHHI7:
            /* upper 32 bits are only available when Config5MI != 0 */
            CP0_CHECK(ctx->mi);
            gen_helper_0e1i(mthc0_watchhi, arg, sel);
            register_name = "WatchHi";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_28:
        switch (sel) {
        case 0:
        case 2:
        case 4:
        case 6:
            tcg_gen_andi_tl(arg, arg, mask);
            gen_mthc0_store64(arg, offsetof(CPUMIPSState, CP0_TagLo));
            register_name = "TagLo";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    default:
        goto cp0_unimplemented;
    }
    trace_mips_translate_c0("mthc0", register_name, reg, sel);

cp0_unimplemented:
    qemu_log_mask(LOG_UNIMP, "mthc0 %s (reg %d sel %d)\n",
                  register_name, reg, sel);
}

static inline void gen_mfc0_unimplemented(DisasContext *ctx, TCGv arg)
{
    if (ctx->insn_flags & ISA_MIPS_R6) {
        tcg_gen_movi_tl(arg, 0);
    } else {
        tcg_gen_movi_tl(arg, ~0);
    }
}

static void gen_mfc0(DisasContext *ctx, TCGv arg, int reg, int sel)
{
    const char *register_name = "invalid";

#ifndef TARGET_CHERI
    if (sel != 0) {
        check_insn(ctx, ISA_MIPS_R1);
    }
#endif

    switch (reg) {
    case CP0_REGISTER_00:
        switch (sel) {
        case CP0_REG00__INDEX:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_Index));
            register_name = "Index";
            break;
        case CP0_REG00__MVPCONTROL:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mfc0_mvpcontrol(arg, cpu_env);
            register_name = "MVPControl";
            break;
        case CP0_REG00__MVPCONF0:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mfc0_mvpconf0(arg, cpu_env);
            register_name = "MVPConf0";
            break;
        case CP0_REG00__MVPCONF1:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mfc0_mvpconf1(arg, cpu_env);
            register_name = "MVPConf1";
            break;
        case CP0_REG00__VPCONTROL:
            CP0_CHECK(ctx->vp);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_VPControl));
            register_name = "VPControl";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_01:
        switch (sel) {
        case CP0_REG01__RANDOM:
            CP0_CHECK(!(ctx->insn_flags & ISA_MIPS_R6));
            gen_helper_mfc0_random(arg, cpu_env);
            register_name = "Random";
            break;
        case CP0_REG01__VPECONTROL:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_VPEControl));
            register_name = "VPEControl";
            break;
        case CP0_REG01__VPECONF0:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_VPEConf0));
            register_name = "VPEConf0";
            break;
        case CP0_REG01__VPECONF1:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_VPEConf1));
            register_name = "VPEConf1";
            break;
        case CP0_REG01__YQMASK:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_mfc0_load64(arg, offsetof(CPUMIPSState, CP0_YQMask));
            register_name = "YQMask";
            break;
        case CP0_REG01__VPESCHEDULE:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_mfc0_load64(arg, offsetof(CPUMIPSState, CP0_VPESchedule));
            register_name = "VPESchedule";
            break;
        case CP0_REG01__VPESCHEFBACK:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_mfc0_load64(arg, offsetof(CPUMIPSState, CP0_VPEScheFBack));
            register_name = "VPEScheFBack";
            break;
        case CP0_REG01__VPEOPT:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_VPEOpt));
            register_name = "VPEOpt";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_02:
        switch (sel) {
        case CP0_REG02__ENTRYLO0:
            {
                TCGv_i64 tmp = tcg_temp_new_i64();
                tcg_gen_ld_i64(tmp, cpu_env,
                               offsetof(CPUMIPSState, CP0_EntryLo0));
#if defined(TARGET_MIPS64)
                if (ctx->rxi) {
                    /* Move RI/XI fields to bits 31:30 */
#if defined(TARGET_CHERI)
                    tcg_gen_shri_tl(arg, tmp, CP0EnLo_L);
#else
                    tcg_gen_shri_tl(arg, tmp, CP0EnLo_XI);
#endif /* TARGET_CHERI */
                    tcg_gen_deposit_tl(tmp, tmp, arg, 30, 2);
                }
#endif
                gen_move_low32(arg, tmp);
                tcg_temp_free_i64(tmp);
            }
            register_name = "EntryLo0";
            break;
        case CP0_REG02__TCSTATUS:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mfc0_tcstatus(arg, cpu_env);
            register_name = "TCStatus";
            break;
        case CP0_REG02__TCBIND:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mfc0_tcbind(arg, cpu_env);
            register_name = "TCBind";
            break;
        case CP0_REG02__TCRESTART:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mfc0_tcrestart(arg, cpu_env);
            register_name = "TCRestart";
            break;
        case CP0_REG02__TCHALT:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mfc0_tchalt(arg, cpu_env);
            register_name = "TCHalt";
            break;
        case CP0_REG02__TCCONTEXT:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mfc0_tccontext(arg, cpu_env);
            register_name = "TCContext";
            break;
        case CP0_REG02__TCSCHEDULE:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mfc0_tcschedule(arg, cpu_env);
            register_name = "TCSchedule";
            break;
        case CP0_REG02__TCSCHEFBACK:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mfc0_tcschefback(arg, cpu_env);
            register_name = "TCScheFBack";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_03:
        switch (sel) {
        case CP0_REG03__ENTRYLO1:
            {
                TCGv_i64 tmp = tcg_temp_new_i64();
                tcg_gen_ld_i64(tmp, cpu_env,
                               offsetof(CPUMIPSState, CP0_EntryLo1));
#if defined(TARGET_MIPS64)
                if (ctx->rxi) {
                    /* Move RI/XI fields to bits 31:30 */
#if defined(TARGET_CHERI)
                    tcg_gen_shri_tl(arg, tmp, CP0EnLo_L);
#else
                    tcg_gen_shri_tl(arg, tmp, CP0EnLo_XI);
#endif /* TARGET_CHERI */
                    tcg_gen_deposit_tl(tmp, tmp, arg, 30, 2);
                }
#endif
                gen_move_low32(arg, tmp);
                tcg_temp_free_i64(tmp);
            }
            register_name = "EntryLo1";
            break;
        case CP0_REG03__GLOBALNUM:
            CP0_CHECK(ctx->vp);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_GlobalNumber));
            register_name = "GlobalNumber";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_04:
        switch (sel) {
        case CP0_REG04__CONTEXT:
            tcg_gen_ld_tl(arg, cpu_env, offsetof(CPUMIPSState, CP0_Context));
            tcg_gen_ext32s_tl(arg, arg);
            register_name = "Context";
            break;
        case CP0_REG04__CONTEXTCONFIG:
            /* SmartMIPS ASE */
            /* gen_helper_mfc0_contextconfig(arg); */
            register_name = "ContextConfig";
            goto cp0_unimplemented;
        case CP0_REG04__USERLOCAL:
            CP0_CHECK(ctx->ulri);
            tcg_gen_ld_tl(arg, cpu_env,
                          offsetof(CPUMIPSState, active_tc.CP0_UserLocal));
            tcg_gen_ext32s_tl(arg, arg);
            register_name = "UserLocal";
            break;
        case CP0_REG04__MMID:
            CP0_CHECK(ctx->mi);
            gen_helper_mtc0_memorymapid(cpu_env, arg);
            register_name = "MMID";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_05:
        switch (sel) {
        case CP0_REG05__PAGEMASK:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_PageMask));
            register_name = "PageMask";
            break;
        case CP0_REG05__PAGEGRAIN:
            check_insn(ctx, ISA_MIPS_R2);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_PageGrain));
            register_name = "PageGrain";
            break;
        case CP0_REG05__SEGCTL0:
            CP0_CHECK(ctx->sc);
            tcg_gen_ld_tl(arg, cpu_env, offsetof(CPUMIPSState, CP0_SegCtl0));
            tcg_gen_ext32s_tl(arg, arg);
            register_name = "SegCtl0";
            break;
        case CP0_REG05__SEGCTL1:
            CP0_CHECK(ctx->sc);
            tcg_gen_ld_tl(arg, cpu_env, offsetof(CPUMIPSState, CP0_SegCtl1));
            tcg_gen_ext32s_tl(arg, arg);
            register_name = "SegCtl1";
            break;
        case CP0_REG05__SEGCTL2:
            CP0_CHECK(ctx->sc);
            tcg_gen_ld_tl(arg, cpu_env, offsetof(CPUMIPSState, CP0_SegCtl2));
            tcg_gen_ext32s_tl(arg, arg);
            register_name = "SegCtl2";
            break;
        case CP0_REG05__PWBASE:
            check_pw(ctx);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_PWBase));
            register_name = "PWBase";
            break;
        case CP0_REG05__PWFIELD:
            check_pw(ctx);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_PWField));
            register_name = "PWField";
            break;
        case CP0_REG05__PWSIZE:
            check_pw(ctx);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_PWSize));
            register_name = "PWSize";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_06:
        switch (sel) {
        case CP0_REG06__WIRED:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_Wired));
            register_name = "Wired";
            break;
        case CP0_REG06__SRSCONF0:
            check_insn(ctx, ISA_MIPS_R2);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_SRSConf0));
            register_name = "SRSConf0";
            break;
        case CP0_REG06__SRSCONF1:
            check_insn(ctx, ISA_MIPS_R2);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_SRSConf1));
            register_name = "SRSConf1";
            break;
        case CP0_REG06__SRSCONF2:
            check_insn(ctx, ISA_MIPS_R2);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_SRSConf2));
            register_name = "SRSConf2";
            break;
        case CP0_REG06__SRSCONF3:
            check_insn(ctx, ISA_MIPS_R2);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_SRSConf3));
            register_name = "SRSConf3";
            break;
        case CP0_REG06__SRSCONF4:
            check_insn(ctx, ISA_MIPS_R2);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_SRSConf4));
            register_name = "SRSConf4";
            break;
        case CP0_REG06__PWCTL:
            check_pw(ctx);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_PWCtl));
            register_name = "PWCtl";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_07:
        switch (sel) {
        case CP0_REG07__HWRENA:
            check_insn(ctx, ISA_MIPS_R2);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_HWREna));
            register_name = "HWREna";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_08:
        switch (sel) {
        case CP0_REG08__BADVADDR:
            tcg_gen_ld_tl(arg, cpu_env, offsetof(CPUMIPSState, CP0_BadVAddr));
            tcg_gen_ext32s_tl(arg, arg);
            register_name = "BadVAddr";
            break;
        case CP0_REG08__BADINSTR:
            CP0_CHECK(ctx->bi);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_BadInstr));
            register_name = "BadInstr";
            break;
        case CP0_REG08__BADINSTRP:
            CP0_CHECK(ctx->bp);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_BadInstrP));
            register_name = "BadInstrP";
            break;
        case CP0_REG08__BADINSTRX:
            CP0_CHECK(ctx->bi);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_BadInstrX));
            tcg_gen_andi_tl(arg, arg, ~0xffff);
            register_name = "BadInstrX";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_09:
        switch (sel) {
        case CP0_REG09__COUNT:
            /* Mark as an IO operation because we read the time.  */
            if (tb_cflags(ctx->base.tb) & CF_USE_ICOUNT) {
                gen_io_start();
            }
            gen_helper_mfc0_count(arg, cpu_env);
            /*
             * Break the TB to be able to take timer interrupts immediately
             * after reading count. DISAS_STOP isn't sufficient, we need to
             * ensure we break completely out of translated code.
             */
            gen_save_pc(ctx->base.pc_next + 4);
            ctx->base.is_jmp = DISAS_EXIT;
            register_name = "Count";
            break;
#if !defined(TARGET_CHERI) /* Conflicts with RTC */
        case CP0_REG09__SAARI:
            CP0_CHECK(ctx->saar);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_SAARI));
            register_name = "SAARI";
            break;
        case CP0_REG09__SAAR:
            CP0_CHECK(ctx->saar);
            gen_helper_mfc0_saar(arg, cpu_env);
            register_name = "SAAR";
            break;
#endif
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_10:
        switch (sel) {
        case CP0_REG10__ENTRYHI:
            tcg_gen_ld_tl(arg, cpu_env, offsetof(CPUMIPSState, CP0_EntryHi));
            tcg_gen_ext32s_tl(arg, arg);
            register_name = "EntryHi";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_11:
        switch (sel) {
        case CP0_REG11__COMPARE:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_Compare));
            register_name = "Compare";
            break;
        /* 6,7 are implementation dependent */
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_12:
        switch (sel) {
        case CP0_REG12__STATUS:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_Status));
            register_name = "Status";
            break;
        case CP0_REG12__INTCTL:
            check_insn(ctx, ISA_MIPS_R2);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_IntCtl));
            register_name = "IntCtl";
            break;
        case CP0_REG12__SRSCTL:
            check_insn(ctx, ISA_MIPS_R2);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_SRSCtl));
            register_name = "SRSCtl";
            break;
        case CP0_REG12__SRSMAP:
            check_insn(ctx, ISA_MIPS_R2);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_SRSMap));
            register_name = "SRSMap";
            break;
        default:
            goto cp0_unimplemented;
       }
        break;
    case CP0_REGISTER_13:
        switch (sel) {
        case CP0_REG13__CAUSE:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_Cause));
            register_name = "Cause";
            break;
        default:
            goto cp0_unimplemented;
       }
        break;
    case CP0_REGISTER_14:
        switch (sel) {
        case CP0_REG14__EPC:
#ifdef TARGET_CHERI
            gen_helper_mfc0_epc(arg, cpu_env);
#else
            tcg_gen_ld_tl(arg, cpu_env, offsetof(CPUMIPSState, CP0_EPC));
#endif
            tcg_gen_ext32s_tl(arg, arg);
            register_name = "EPC";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_15:
        switch (sel) {
        case CP0_REG15__PRID:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_PRid));
            register_name = "PRid";
            break;
        case CP0_REG15__EBASE:
            check_insn(ctx, ISA_MIPS_R2);
            tcg_gen_ld_tl(arg, cpu_env, offsetof(CPUMIPSState, CP0_EBase));
            tcg_gen_ext32s_tl(arg, arg);
            register_name = "EBase";
            break;
        case CP0_REG15__CMGCRBASE:
            check_insn(ctx, ISA_MIPS_R2);
            CP0_CHECK(ctx->cmgcr);
            tcg_gen_ld_tl(arg, cpu_env, offsetof(CPUMIPSState, CP0_CMGCRBase));
            tcg_gen_ext32s_tl(arg, arg);
            register_name = "CMGCRBase";
            break;
#ifdef TARGET_CHERI
        case 6:
            /*
             * See section 7.3.5 Core Identification (CPO Register 15,
             * Select 6 of the BERI Hardware Reference.
             */
            gen_helper_mfc0_coreid(arg, cpu_env);
            register_name = "CoreID";
            break;
        case 7:
            /*
             * See section 7.3.6 Thread Identification (CPO Register 15,
             * Select 7 of the BERI Hardware Reference.
             */
            tcg_gen_movi_tl(arg, 0); /* currently unimplemented */
            register_name = "ThreadID";
            break;
#endif /* TARGET_CHERI */
        default:
            goto cp0_unimplemented;
       }
        break;
    case CP0_REGISTER_16:
        switch (sel) {
        case CP0_REG16__CONFIG:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_Config0));
            register_name = "Config";
            break;
        case CP0_REG16__CONFIG1:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_Config1));
            register_name = "Config1";
            break;
        case CP0_REG16__CONFIG2:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_Config2));
            register_name = "Config2";
            break;
        case CP0_REG16__CONFIG3:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_Config3));
            register_name = "Config3";
            break;
        case CP0_REG16__CONFIG4:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_Config4));
            register_name = "Config4";
            break;
        case CP0_REG16__CONFIG5:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_Config5));
            register_name = "Config5";
            break;
        /* 6,7 are implementation dependent */
        case CP0_REG16__CONFIG6:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_Config6));
            register_name = "Config6";
            break;
        case CP0_REG16__CONFIG7:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_Config7));
            register_name = "Config7";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_17:
        switch (sel) {
        case CP0_REG17__LLADDR:
            gen_helper_mfc0_lladdr(arg, cpu_env);
            register_name = "LLAddr";
            break;
        case CP0_REG17__MAAR:
            CP0_CHECK(ctx->mrp);
            gen_helper_mfc0_maar(arg, cpu_env);
            register_name = "MAAR";
            break;
        case CP0_REG17__MAARI:
            CP0_CHECK(ctx->mrp);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_MAARI));
            register_name = "MAARI";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_18:
        switch (sel) {
        case CP0_REG18__WATCHLO0:
        case CP0_REG18__WATCHLO1:
        case CP0_REG18__WATCHLO2:
        case CP0_REG18__WATCHLO3:
        case CP0_REG18__WATCHLO4:
        case CP0_REG18__WATCHLO5:
        case CP0_REG18__WATCHLO6:
        case CP0_REG18__WATCHLO7:
            CP0_CHECK(ctx->CP0_Config1 & (1 << CP0C1_WR));
            gen_helper_1e0i(mfc0_watchlo, arg, sel);
            register_name = "WatchLo";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_19:
        switch (sel) {
        case CP0_REG19__WATCHHI0:
        case CP0_REG19__WATCHHI1:
        case CP0_REG19__WATCHHI2:
        case CP0_REG19__WATCHHI3:
        case CP0_REG19__WATCHHI4:
        case CP0_REG19__WATCHHI5:
        case CP0_REG19__WATCHHI6:
        case CP0_REG19__WATCHHI7:
            CP0_CHECK(ctx->CP0_Config1 & (1 << CP0C1_WR));
            gen_helper_1e0i(mfc0_watchhi, arg, sel);
            register_name = "WatchHi";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_20:
        switch (sel) {
        case CP0_REG20__XCONTEXT:
#if defined(TARGET_MIPS64)
            check_insn(ctx, ISA_MIPS3);
            tcg_gen_ld_tl(arg, cpu_env, offsetof(CPUMIPSState, CP0_XContext));
            tcg_gen_ext32s_tl(arg, arg);
            register_name = "XContext";
            break;
#endif
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_21:
       /* Officially reserved, but sel 0 is used for R1x000 framemask */
        CP0_CHECK(!(ctx->insn_flags & ISA_MIPS_R6));
        switch (sel) {
        case 0:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_Framemask));
            register_name = "Framemask";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_22:
        tcg_gen_movi_tl(arg, 0); /* unimplemented */
        register_name = "'Diagnostic"; /* implementation dependent */
        break;
    case CP0_REGISTER_23:
        switch (sel) {
        case CP0_REG23__DEBUG:
            gen_helper_mfc0_debug(arg, cpu_env); /* EJTAG support */
            register_name = "Debug";
            break;
        case CP0_REG23__TRACECONTROL:
            /* PDtrace support */
            /* gen_helper_mfc0_tracecontrol(arg);  */
            register_name = "TraceControl";
            goto cp0_unimplemented;
        case CP0_REG23__TRACECONTROL2:
            /* PDtrace support */
            /* gen_helper_mfc0_tracecontrol2(arg); */
            register_name = "TraceControl2";
            goto cp0_unimplemented;
        case CP0_REG23__USERTRACEDATA1:
            /* PDtrace support */
            /* gen_helper_mfc0_usertracedata1(arg);*/
            register_name = "UserTraceData1";
            goto cp0_unimplemented;
        case CP0_REG23__TRACEIBPC:
            /* PDtrace support */
            /* gen_helper_mfc0_traceibpc(arg);     */
            register_name = "TraceIBPC";
            goto cp0_unimplemented;
        case CP0_REG23__TRACEDBPC:
            /* PDtrace support */
            /* gen_helper_mfc0_tracedbpc(arg);     */
            register_name = "TraceDBPC";
            goto cp0_unimplemented;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_24:
        switch (sel) {
        case CP0_REG24__DEPC:
            /* EJTAG support */
            tcg_gen_ld_tl(arg, cpu_env, offsetof(CPUMIPSState, CP0_DEPC));
            tcg_gen_ext32s_tl(arg, arg);
            register_name = "DEPC";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_25:
        switch (sel) {
        case CP0_REG25__PERFCTL0:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_Performance0));
            register_name = "Performance0";
            break;
        case CP0_REG25__PERFCNT0:
            /* gen_helper_mfc0_performance1(arg); */
            register_name = "Performance1";
            goto cp0_unimplemented;
        case CP0_REG25__PERFCTL1:
            /* gen_helper_mfc0_performance2(arg); */
            register_name = "Performance2";
            goto cp0_unimplemented;
        case CP0_REG25__PERFCNT1:
            /* gen_helper_mfc0_performance3(arg); */
            register_name = "Performance3";
            goto cp0_unimplemented;
        case CP0_REG25__PERFCTL2:
            /* gen_helper_mfc0_performance4(arg); */
            register_name = "Performance4";
            goto cp0_unimplemented;
        case CP0_REG25__PERFCNT2:
            /* gen_helper_mfc0_performance5(arg); */
            register_name = "Performance5";
            goto cp0_unimplemented;
        case CP0_REG25__PERFCTL3:
            /* gen_helper_mfc0_performance6(arg); */
            register_name = "Performance6";
            goto cp0_unimplemented;
        case CP0_REG25__PERFCNT3:
            /* gen_helper_mfc0_performance7(arg); */
            register_name = "Performance7";
            goto cp0_unimplemented;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_26:
        switch (sel) {
        case CP0_REG26__ERRCTL:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_ErrCtl));
            register_name = "ErrCtl";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_27:
        switch (sel) {
        case CP0_REG27__CACHERR:
            tcg_gen_movi_tl(arg, 0); /* unimplemented */
            register_name = "CacheErr";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_28:
        switch (sel) {
        case CP0_REG28__TAGLO:
        case CP0_REG28__TAGLO1:
        case CP0_REG28__TAGLO2:
        case CP0_REG28__TAGLO3:
            {
                TCGv_i64 tmp = tcg_temp_new_i64();
                tcg_gen_ld_i64(tmp, cpu_env, offsetof(CPUMIPSState, CP0_TagLo));
                gen_move_low32(arg, tmp);
                tcg_temp_free_i64(tmp);
            }
            register_name = "TagLo";
            break;
        case CP0_REG28__DATALO:
        case CP0_REG28__DATALO1:
        case CP0_REG28__DATALO2:
        case CP0_REG28__DATALO3:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_DataLo));
            register_name = "DataLo";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_29:
        switch (sel) {
        case CP0_REG29__TAGHI:
        case CP0_REG29__TAGHI1:
        case CP0_REG29__TAGHI2:
        case CP0_REG29__TAGHI3:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_TagHi));
            register_name = "TagHi";
            break;
        case CP0_REG29__DATAHI:
        case CP0_REG29__DATAHI1:
        case CP0_REG29__DATAHI2:
        case CP0_REG29__DATAHI3:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_DataHi));
            register_name = "DataHi";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_30:
        switch (sel) {
        case CP0_REG30__ERROREPC:
#ifdef TARGET_CHERI
            gen_helper_mfc0_error_epc(arg, cpu_env);
#else
            tcg_gen_ld_tl(arg, cpu_env, offsetof(CPUMIPSState, CP0_ErrorEPC));
#endif
            tcg_gen_ext32s_tl(arg, arg);
            register_name = "ErrorEPC";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_31:
        switch (sel) {
        case CP0_REG31__DESAVE:
            /* EJTAG support */
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_DESAVE));
            register_name = "DESAVE";
            break;
        case CP0_REG31__KSCRATCH1:
        case CP0_REG31__KSCRATCH2:
        case CP0_REG31__KSCRATCH3:
        case CP0_REG31__KSCRATCH4:
        case CP0_REG31__KSCRATCH5:
        case CP0_REG31__KSCRATCH6:
            CP0_CHECK(ctx->kscrexist & (1 << sel));
            tcg_gen_ld_tl(arg, cpu_env,
                          offsetof(CPUMIPSState, CP0_KScratch[sel - 2]));
            tcg_gen_ext32s_tl(arg, arg);
            register_name = "KScratch";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    default:
       goto cp0_unimplemented;
    }
    trace_mips_translate_c0("mfc0", register_name, reg, sel);
    return;

cp0_unimplemented:
    qemu_log_mask(LOG_UNIMP, "mfc0 %s (reg %d sel %d)\n",
                  register_name, reg, sel);
    gen_mfc0_unimplemented(ctx, arg);
}

static void gen_mtc0(DisasContext *ctx, TCGv arg, int reg, int sel)
{
    const char *register_name = "invalid";

    if (sel != 0) {
        check_insn(ctx, ISA_MIPS_R1);
    }

    if (tb_cflags(ctx->base.tb) & CF_USE_ICOUNT) {
        gen_io_start();
    }

    switch (reg) {
    case CP0_REGISTER_00:
        switch (sel) {
        case CP0_REG00__INDEX:
            gen_helper_mtc0_index(cpu_env, arg);
            register_name = "Index";
            break;
        case CP0_REG00__MVPCONTROL:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mtc0_mvpcontrol(cpu_env, arg);
            register_name = "MVPControl";
            break;
        case CP0_REG00__MVPCONF0:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            /* ignored */
            register_name = "MVPConf0";
            break;
        case CP0_REG00__MVPCONF1:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            /* ignored */
            register_name = "MVPConf1";
            break;
        case CP0_REG00__VPCONTROL:
            CP0_CHECK(ctx->vp);
            /* ignored */
            register_name = "VPControl";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_01:
        switch (sel) {
        case CP0_REG01__RANDOM:
            /* ignored */
            register_name = "Random";
            break;
        case CP0_REG01__VPECONTROL:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mtc0_vpecontrol(cpu_env, arg);
            register_name = "VPEControl";
            break;
        case CP0_REG01__VPECONF0:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mtc0_vpeconf0(cpu_env, arg);
            register_name = "VPEConf0";
            break;
        case CP0_REG01__VPECONF1:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mtc0_vpeconf1(cpu_env, arg);
            register_name = "VPEConf1";
            break;
        case CP0_REG01__YQMASK:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mtc0_yqmask(cpu_env, arg);
            register_name = "YQMask";
            break;
        case CP0_REG01__VPESCHEDULE:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_mtc0_store(ctx, arg, reg, sel,
                           offsetof(CPUMIPSState, CP0_VPESchedule));
            register_name = "VPESchedule";
            break;
        case CP0_REG01__VPESCHEFBACK:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_mtc0_store(ctx, arg, reg, sel,
                           offsetof(CPUMIPSState, CP0_VPEScheFBack));
            register_name = "VPEScheFBack";
            break;
        case CP0_REG01__VPEOPT:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mtc0_vpeopt(cpu_env, arg);
            register_name = "VPEOpt";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_02:
        switch (sel) {
        case CP0_REG02__ENTRYLO0:
            gen_helper_mtc0_entrylo0(cpu_env, arg);
            register_name = "EntryLo0";
            break;
        case CP0_REG02__TCSTATUS:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mtc0_tcstatus(cpu_env, arg);
            register_name = "TCStatus";
            break;
        case CP0_REG02__TCBIND:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mtc0_tcbind(cpu_env, arg);
            register_name = "TCBind";
            break;
        case CP0_REG02__TCRESTART:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mtc0_tcrestart(cpu_env, arg);
            register_name = "TCRestart";
            break;
        case CP0_REG02__TCHALT:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mtc0_tchalt(cpu_env, arg);
            register_name = "TCHalt";
            break;
        case CP0_REG02__TCCONTEXT:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mtc0_tccontext(cpu_env, arg);
            register_name = "TCContext";
            break;
        case CP0_REG02__TCSCHEDULE:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mtc0_tcschedule(cpu_env, arg);
            register_name = "TCSchedule";
            break;
        case CP0_REG02__TCSCHEFBACK:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mtc0_tcschefback(cpu_env, arg);
            register_name = "TCScheFBack";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_03:
        switch (sel) {
        case CP0_REG03__ENTRYLO1:
            gen_helper_mtc0_entrylo1(cpu_env, arg);
            register_name = "EntryLo1";
            break;
        case CP0_REG03__GLOBALNUM:
            CP0_CHECK(ctx->vp);
            /* ignored */
            register_name = "GlobalNumber";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_04:
        switch (sel) {
        case CP0_REG04__CONTEXT:
            gen_helper_mtc0_context(cpu_env, arg);
            register_name = "Context";
            break;
        case CP0_REG04__CONTEXTCONFIG:
            /* SmartMIPS ASE */
            /* gen_helper_mtc0_contextconfig(arg); */
            register_name = "ContextConfig";
            goto cp0_unimplemented;
        case CP0_REG04__USERLOCAL:
            CP0_CHECK(ctx->ulri);
            gen_mtc0_store(ctx, arg, reg, sel,
                           offsetof(CPUMIPSState, active_tc.CP0_UserLocal));
            register_name = "UserLocal";
            break;
        case CP0_REG04__MMID:
            CP0_CHECK(ctx->mi);
            gen_mtc0_store32(ctx, arg, reg, sel,
                             offsetof(CPUMIPSState, CP0_MemoryMapID));
            register_name = "MMID";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_05:
        switch (sel) {
        case CP0_REG05__PAGEMASK:
            gen_helper_mtc0_pagemask(cpu_env, arg);
            register_name = "PageMask";
            break;
        case CP0_REG05__PAGEGRAIN:
            check_insn(ctx, ISA_MIPS_R2);
            gen_helper_mtc0_pagegrain(cpu_env, arg);
            register_name = "PageGrain";
            ctx->base.is_jmp = DISAS_STOP;
            break;
        case CP0_REG05__SEGCTL0:
            CP0_CHECK(ctx->sc);
            gen_helper_mtc0_segctl0(cpu_env, arg);
            register_name = "SegCtl0";
            break;
        case CP0_REG05__SEGCTL1:
            CP0_CHECK(ctx->sc);
            gen_helper_mtc0_segctl1(cpu_env, arg);
            register_name = "SegCtl1";
            break;
        case CP0_REG05__SEGCTL2:
            CP0_CHECK(ctx->sc);
            gen_helper_mtc0_segctl2(cpu_env, arg);
            register_name = "SegCtl2";
            break;
        case CP0_REG05__PWBASE:
            check_pw(ctx);
            gen_mtc0_store32(ctx, arg, reg, sel,
                             offsetof(CPUMIPSState, CP0_PWBase));
            register_name = "PWBase";
            break;
        case CP0_REG05__PWFIELD:
            check_pw(ctx);
            gen_helper_mtc0_pwfield(cpu_env, arg);
            register_name = "PWField";
            break;
        case CP0_REG05__PWSIZE:
            check_pw(ctx);
            gen_helper_mtc0_pwsize(cpu_env, arg);
            register_name = "PWSize";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_06:
        switch (sel) {
        case CP0_REG06__WIRED:
            gen_helper_mtc0_wired(cpu_env, arg);
            register_name = "Wired";
            break;
        case CP0_REG06__SRSCONF0:
            check_insn(ctx, ISA_MIPS_R2);
            gen_helper_mtc0_srsconf0(cpu_env, arg);
            register_name = "SRSConf0";
            break;
        case CP0_REG06__SRSCONF1:
            check_insn(ctx, ISA_MIPS_R2);
            gen_helper_mtc0_srsconf1(cpu_env, arg);
            register_name = "SRSConf1";
            break;
        case CP0_REG06__SRSCONF2:
            check_insn(ctx, ISA_MIPS_R2);
            gen_helper_mtc0_srsconf2(cpu_env, arg);
            register_name = "SRSConf2";
            break;
        case CP0_REG06__SRSCONF3:
            check_insn(ctx, ISA_MIPS_R2);
            gen_helper_mtc0_srsconf3(cpu_env, arg);
            register_name = "SRSConf3";
            break;
        case CP0_REG06__SRSCONF4:
            check_insn(ctx, ISA_MIPS_R2);
            gen_helper_mtc0_srsconf4(cpu_env, arg);
            register_name = "SRSConf4";
            break;
        case CP0_REG06__PWCTL:
            check_pw(ctx);
            gen_helper_mtc0_pwctl(cpu_env, arg);
            register_name = "PWCtl";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_07:
        switch (sel) {
        case CP0_REG07__HWRENA:
            check_insn(ctx, ISA_MIPS_R2);
            gen_helper_mtc0_hwrena(cpu_env, arg);
            ctx->base.is_jmp = DISAS_STOP;
            register_name = "HWREna";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_08:
        switch (sel) {
        case CP0_REG08__BADVADDR:
            /* ignored */
            register_name = "BadVAddr";
            break;
        case CP0_REG08__BADINSTR:
            /* ignored */
            register_name = "BadInstr";
            break;
        case CP0_REG08__BADINSTRP:
            /* ignored */
            register_name = "BadInstrP";
            break;
        case CP0_REG08__BADINSTRX:
            /* ignored */
            register_name = "BadInstrX";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_09:
        switch (sel) {
        case CP0_REG09__COUNT:
            gen_helper_mtc0_count(cpu_env, arg);
            register_name = "Count";
            break;
#if !defined(TARGET_CHERI) /* Conflicts with RTC */
        case CP0_REG09__SAARI:
            CP0_CHECK(ctx->saar);
            gen_helper_mtc0_saari(cpu_env, arg);
            register_name = "SAARI";
            break;
        case CP0_REG09__SAAR:
            CP0_CHECK(ctx->saar);
            gen_helper_mtc0_saar(cpu_env, arg);
            register_name = "SAAR";
            break;
#endif
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_10:
        switch (sel) {
        case CP0_REG10__ENTRYHI:
            gen_helper_mtc0_entryhi(cpu_env, arg);
            register_name = "EntryHi";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_11:
        switch (sel) {
        case CP0_REG11__COMPARE:
            gen_helper_mtc0_compare(cpu_env, arg);
            register_name = "Compare";
            break;
        /* 6,7 are implementation dependent */
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_12:
        switch (sel) {
        case CP0_REG12__STATUS:
            save_cpu_state(ctx, 1);
            gen_helper_mtc0_status(cpu_env, arg);
            /* DISAS_STOP isn't good enough here, hflags may have changed. */
            gen_save_pc(ctx->base.pc_next + 4);
            ctx->base.is_jmp = DISAS_EXIT;
            register_name = "Status";
            break;
        case CP0_REG12__INTCTL:
            check_insn(ctx, ISA_MIPS_R2);
            gen_helper_mtc0_intctl(cpu_env, arg);
            /* Stop translation as we may have switched the execution mode */
            ctx->base.is_jmp = DISAS_STOP;
            register_name = "IntCtl";
            break;
        case CP0_REG12__SRSCTL:
            check_insn(ctx, ISA_MIPS_R2);
            gen_helper_mtc0_srsctl(cpu_env, arg);
            /* Stop translation as we may have switched the execution mode */
            ctx->base.is_jmp = DISAS_STOP;
            register_name = "SRSCtl";
            break;
        case CP0_REG12__SRSMAP:
            check_insn(ctx, ISA_MIPS_R2);
            gen_mtc0_store32(ctx, arg, reg, sel,
                             offsetof(CPUMIPSState, CP0_SRSMap));
            /* Stop translation as we may have switched the execution mode */
            ctx->base.is_jmp = DISAS_STOP;
            register_name = "SRSMap";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_13:
        switch (sel) {
        case CP0_REG13__CAUSE:
            save_cpu_state(ctx, 1);
            gen_helper_mtc0_cause(cpu_env, arg);
            /*
             * Stop translation as we may have triggered an interrupt.
             * DISAS_STOP isn't sufficient, we need to ensure we break out of
             * translated code to check for pending interrupts.
             */
            gen_save_pc(ctx->base.pc_next + 4);
            ctx->base.is_jmp = DISAS_EXIT;
            register_name = "Cause";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_14:
        switch (sel) {
        case CP0_REG14__EPC:
#ifdef TARGET_CHERI
            gen_helper_mtc0_epc(cpu_env, arg);
#else
            gen_mtc0_store(ctx, arg, reg, sel,
                           offsetof(CPUMIPSState, CP0_EPC));
#endif
            register_name = "EPC";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_15:
        switch (sel) {
        case CP0_REG15__PRID:
            /* ignored */
            register_name = "PRid";
            break;
        case CP0_REG15__EBASE:
            check_insn(ctx, ISA_MIPS_R2);
            gen_helper_mtc0_ebase(cpu_env, arg);
            register_name = "EBase";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_16:
        switch (sel) {
        case CP0_REG16__CONFIG:
            gen_helper_mtc0_config0(cpu_env, arg);
            register_name = "Config";
            /* Stop translation as we may have switched the execution mode */
            ctx->base.is_jmp = DISAS_STOP;
            break;
        case CP0_REG16__CONFIG1:
            /* ignored, read only */
            register_name = "Config1";
            break;
        case CP0_REG16__CONFIG2:
            gen_helper_mtc0_config2(cpu_env, arg);
            register_name = "Config2";
            /* Stop translation as we may have switched the execution mode */
            ctx->base.is_jmp = DISAS_STOP;
            break;
        case CP0_REG16__CONFIG3:
            gen_helper_mtc0_config3(cpu_env, arg);
            register_name = "Config3";
            /* Stop translation as we may have switched the execution mode */
            ctx->base.is_jmp = DISAS_STOP;
            break;
        case CP0_REG16__CONFIG4:
            gen_helper_mtc0_config4(cpu_env, arg);
            register_name = "Config4";
            ctx->base.is_jmp = DISAS_STOP;
            break;
        case CP0_REG16__CONFIG5:
            gen_helper_mtc0_config5(cpu_env, arg);
            register_name = "Config5";
            /* Stop translation as we may have switched the execution mode */
            ctx->base.is_jmp = DISAS_STOP;
            break;
        /* 6,7 are implementation dependent */
        case CP0_REG16__CONFIG6:
            /* ignored */
            register_name = "Config6";
            break;
        case CP0_REG16__CONFIG7:
            /* ignored */
            register_name = "Config7";
            break;
        default:
            register_name = "Invalid config selector";
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_17:
        switch (sel) {
        case CP0_REG17__LLADDR:
            gen_helper_mtc0_lladdr(cpu_env, arg);
            register_name = "LLAddr";
            break;
        case CP0_REG17__MAAR:
            CP0_CHECK(ctx->mrp);
            gen_helper_mtc0_maar(cpu_env, arg);
            register_name = "MAAR";
            break;
        case CP0_REG17__MAARI:
            CP0_CHECK(ctx->mrp);
            gen_helper_mtc0_maari(cpu_env, arg);
            register_name = "MAARI";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_18:
        switch (sel) {
        case CP0_REG18__WATCHLO0:
        case CP0_REG18__WATCHLO1:
        case CP0_REG18__WATCHLO2:
        case CP0_REG18__WATCHLO3:
        case CP0_REG18__WATCHLO4:
        case CP0_REG18__WATCHLO5:
        case CP0_REG18__WATCHLO6:
        case CP0_REG18__WATCHLO7:
            CP0_CHECK(ctx->CP0_Config1 & (1 << CP0C1_WR));
            gen_helper_0e1i(mtc0_watchlo, arg, sel);
            register_name = "WatchLo";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_19:
        switch (sel) {
        case CP0_REG19__WATCHHI0:
        case CP0_REG19__WATCHHI1:
        case CP0_REG19__WATCHHI2:
        case CP0_REG19__WATCHHI3:
        case CP0_REG19__WATCHHI4:
        case CP0_REG19__WATCHHI5:
        case CP0_REG19__WATCHHI6:
        case CP0_REG19__WATCHHI7:
            CP0_CHECK(ctx->CP0_Config1 & (1 << CP0C1_WR));
            gen_helper_0e1i(mtc0_watchhi, arg, sel);
            register_name = "WatchHi";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_20:
        switch (sel) {
        case CP0_REG20__XCONTEXT:
#if defined(TARGET_MIPS64)
            check_insn(ctx, ISA_MIPS3);
            gen_helper_mtc0_xcontext(cpu_env, arg);
            register_name = "XContext";
            break;
#endif
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_21:
       /* Officially reserved, but sel 0 is used for R1x000 framemask */
        CP0_CHECK(!(ctx->insn_flags & ISA_MIPS_R6));
        switch (sel) {
        case 0:
            gen_helper_mtc0_framemask(cpu_env, arg);
            register_name = "Framemask";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_22:
        /* ignored */
        register_name = "Diagnostic"; /* implementation dependent */
        break;
    case CP0_REGISTER_23:
        switch (sel) {
        case CP0_REG23__DEBUG:
            save_cpu_state(ctx, 1); // Need to sync PC (PCC.cursor)
            gen_helper_mtc0_debug(cpu_env, arg); /* EJTAG support */
            /* DISAS_STOP isn't good enough here, hflags may have changed. */
            gen_save_pc(ctx->base.pc_next + 4);
            ctx->base.is_jmp = DISAS_EXIT;
            register_name = "Debug";
            break;
        case CP0_REG23__TRACECONTROL:
            /* PDtrace support */
            /* gen_helper_mtc0_tracecontrol(cpu_env, arg);  */
            register_name = "TraceControl";
            /* Stop translation as we may have switched the execution mode */
            ctx->base.is_jmp = DISAS_STOP;
            goto cp0_unimplemented;
        case CP0_REG23__TRACECONTROL2:
            /* PDtrace support */
            /* gen_helper_mtc0_tracecontrol2(cpu_env, arg); */
            register_name = "TraceControl2";
            /* Stop translation as we may have switched the execution mode */
            ctx->base.is_jmp = DISAS_STOP;
            goto cp0_unimplemented;
        case CP0_REG23__USERTRACEDATA1:
            /* Stop translation as we may have switched the execution mode */
            ctx->base.is_jmp = DISAS_STOP;
            /* PDtrace support */
            /* gen_helper_mtc0_usertracedata1(cpu_env, arg);*/
            register_name = "UserTraceData";
            /* Stop translation as we may have switched the execution mode */
            ctx->base.is_jmp = DISAS_STOP;
            goto cp0_unimplemented;
        case CP0_REG23__TRACEIBPC:
            /* PDtrace support */
            /* gen_helper_mtc0_traceibpc(cpu_env, arg);     */
            /* Stop translation as we may have switched the execution mode */
            ctx->base.is_jmp = DISAS_STOP;
            register_name = "TraceIBPC";
            goto cp0_unimplemented;
        case CP0_REG23__TRACEDBPC:
            /* PDtrace support */
            /* gen_helper_mtc0_tracedbpc(cpu_env, arg);     */
            /* Stop translation as we may have switched the execution mode */
            ctx->base.is_jmp = DISAS_STOP;
            register_name = "TraceDBPC";
            goto cp0_unimplemented;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_24:
        switch (sel) {
        case CP0_REG24__DEPC:
            /* EJTAG support */
            gen_mtc0_store(ctx, arg, reg, sel,
                           offsetof(CPUMIPSState, CP0_DEPC));
            register_name = "DEPC";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_25:
        switch (sel) {
        case CP0_REG25__PERFCTL0:
            gen_helper_mtc0_performance0(cpu_env, arg);
            register_name = "Performance0";
            break;
        case CP0_REG25__PERFCNT0:
            /* gen_helper_mtc0_performance1(arg); */
            register_name = "Performance1";
            goto cp0_unimplemented;
        case CP0_REG25__PERFCTL1:
            /* gen_helper_mtc0_performance2(arg); */
            register_name = "Performance2";
            goto cp0_unimplemented;
        case CP0_REG25__PERFCNT1:
            /* gen_helper_mtc0_performance3(arg); */
            register_name = "Performance3";
            goto cp0_unimplemented;
        case CP0_REG25__PERFCTL2:
            /* gen_helper_mtc0_performance4(arg); */
            register_name = "Performance4";
            goto cp0_unimplemented;
        case CP0_REG25__PERFCNT2:
            /* gen_helper_mtc0_performance5(arg); */
            register_name = "Performance5";
            goto cp0_unimplemented;
        case CP0_REG25__PERFCTL3:
            /* gen_helper_mtc0_performance6(arg); */
            register_name = "Performance6";
            goto cp0_unimplemented;
        case CP0_REG25__PERFCNT3:
            /* gen_helper_mtc0_performance7(arg); */
            register_name = "Performance7";
            goto cp0_unimplemented;
        default:
            goto cp0_unimplemented;
        }
       break;
    case CP0_REGISTER_26:
        save_cpu_state(ctx, 1); // Need to sync PC (PCC.cursor)
        gen_helper_mtc0_dumpstate(cpu_env, arg); /* CHERI: dump reg state */
#if !defined(TARGET_CHERI)
        switch (sel) {
        case CP0_REG26__ERRCTL:
            gen_helper_mtc0_errctl(cpu_env, arg);
            ctx->base.is_jmp = DISAS_STOP;
            register_name = "ErrCtl";
            break;
        default:
            goto cp0_unimplemented;
        }
#endif /* TARGET_CHERI */
        break;
    case CP0_REGISTER_27:
        switch (sel) {
        case CP0_REG27__CACHERR:
            /* ignored */
            register_name = "CacheErr";
            break;
        default:
            goto cp0_unimplemented;
        }
       break;
    case CP0_REGISTER_28:
        switch (sel) {
        case CP0_REG28__TAGLO:
        case CP0_REG28__TAGLO1:
        case CP0_REG28__TAGLO2:
        case CP0_REG28__TAGLO3:
            gen_helper_mtc0_taglo(cpu_env, arg);
            register_name = "TagLo";
            break;
        case CP0_REG28__DATALO:
        case CP0_REG28__DATALO1:
        case CP0_REG28__DATALO2:
        case CP0_REG28__DATALO3:
            gen_helper_mtc0_datalo(cpu_env, arg);
            register_name = "DataLo";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_29:
        switch (sel) {
        case CP0_REG29__TAGHI:
        case CP0_REG29__TAGHI1:
        case CP0_REG29__TAGHI2:
        case CP0_REG29__TAGHI3:
            gen_helper_mtc0_taghi(cpu_env, arg);
            register_name = "TagHi";
            break;
        case CP0_REG29__DATAHI:
        case CP0_REG29__DATAHI1:
        case CP0_REG29__DATAHI2:
        case CP0_REG29__DATAHI3:
            gen_helper_mtc0_datahi(cpu_env, arg);
            register_name = "DataHi";
            break;
        default:
            register_name = "invalid sel";
            goto cp0_unimplemented;
        }
       break;
    case CP0_REGISTER_30:
        switch (sel) {
        case CP0_REG30__ERROREPC:
#ifdef TARGET_CHERI
            gen_helper_mtc0_error_epc(cpu_env, arg);
#else
            gen_mtc0_store(ctx, arg, reg, sel,
                           offsetof(CPUMIPSState, CP0_ErrorEPC));
#endif
            register_name = "ErrorEPC";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_31:
        switch (sel) {
        case CP0_REG31__DESAVE:
            /* EJTAG support */
            gen_mtc0_store32(ctx, arg, reg, sel,
                             offsetof(CPUMIPSState, CP0_DESAVE));
            register_name = "DESAVE";
            break;
        case CP0_REG31__KSCRATCH1:
        case CP0_REG31__KSCRATCH2:
        case CP0_REG31__KSCRATCH3:
        case CP0_REG31__KSCRATCH4:
        case CP0_REG31__KSCRATCH5:
        case CP0_REG31__KSCRATCH6:
            CP0_CHECK(ctx->kscrexist & (1 << sel));
            gen_mtc0_store(ctx, arg, reg, sel,
                           offsetof(CPUMIPSState, CP0_KScratch[sel - 2]));
            register_name = "KScratch";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    default:
       goto cp0_unimplemented;
    }
    trace_mips_translate_c0("mtc0", register_name, reg, sel);

    /* For simplicity assume that all writes can cause interrupts.  */
    if (tb_cflags(ctx->base.tb) & CF_USE_ICOUNT) {
        /*
         * DISAS_STOP isn't sufficient, we need to ensure we break out of
         * translated code to check for pending interrupts.
         */
        gen_save_pc(ctx->base.pc_next + 4);
        ctx->base.is_jmp = DISAS_EXIT;
    }
    return;

cp0_unimplemented:
    qemu_log_mask(LOG_UNIMP, "mtc0 %s (reg %d sel %d)\n",
                  register_name, reg, sel);
}

#if defined(TARGET_MIPS64)
static void gen_dmfc0(DisasContext *ctx, TCGv arg, int reg, int sel)
{
    const char *register_name = "invalid";

#ifndef TARGET_CHERI
    if (sel != 0) {
        check_insn(ctx, ISA_MIPS_R1);
    }
#endif /* ! TARGET_CHERI */

    switch (reg) {
    case CP0_REGISTER_00:
        switch (sel) {
        case CP0_REG00__INDEX:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_Index));
            register_name = "Index";
            break;
        case CP0_REG00__MVPCONTROL:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mfc0_mvpcontrol(arg, cpu_env);
            register_name = "MVPControl";
            break;
        case CP0_REG00__MVPCONF0:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mfc0_mvpconf0(arg, cpu_env);
            register_name = "MVPConf0";
            break;
        case CP0_REG00__MVPCONF1:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mfc0_mvpconf1(arg, cpu_env);
            register_name = "MVPConf1";
            break;
        case CP0_REG00__VPCONTROL:
            CP0_CHECK(ctx->vp);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_VPControl));
            register_name = "VPControl";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_01:
        switch (sel) {
        case CP0_REG01__RANDOM:
            CP0_CHECK(!(ctx->insn_flags & ISA_MIPS_R6));
            gen_helper_mfc0_random(arg, cpu_env);
            register_name = "Random";
            break;
        case CP0_REG01__VPECONTROL:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_VPEControl));
            register_name = "VPEControl";
            break;
        case CP0_REG01__VPECONF0:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_VPEConf0));
            register_name = "VPEConf0";
            break;
        case CP0_REG01__VPECONF1:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_VPEConf1));
            register_name = "VPEConf1";
            break;
        case CP0_REG01__YQMASK:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            tcg_gen_ld_tl(arg, cpu_env,
                          offsetof(CPUMIPSState, CP0_YQMask));
            register_name = "YQMask";
            break;
        case CP0_REG01__VPESCHEDULE:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            tcg_gen_ld_tl(arg, cpu_env,
                          offsetof(CPUMIPSState, CP0_VPESchedule));
            register_name = "VPESchedule";
            break;
        case CP0_REG01__VPESCHEFBACK:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            tcg_gen_ld_tl(arg, cpu_env,
                          offsetof(CPUMIPSState, CP0_VPEScheFBack));
            register_name = "VPEScheFBack";
            break;
        case CP0_REG01__VPEOPT:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_VPEOpt));
            register_name = "VPEOpt";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_02:
        switch (sel) {
        case CP0_REG02__ENTRYLO0:
            tcg_gen_ld_tl(arg, cpu_env,
                          offsetof(CPUMIPSState, CP0_EntryLo0));
            register_name = "EntryLo0";
            break;
        case CP0_REG02__TCSTATUS:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mfc0_tcstatus(arg, cpu_env);
            register_name = "TCStatus";
            break;
        case CP0_REG02__TCBIND:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mfc0_tcbind(arg, cpu_env);
            register_name = "TCBind";
            break;
        case CP0_REG02__TCRESTART:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_dmfc0_tcrestart(arg, cpu_env);
            register_name = "TCRestart";
            break;
        case CP0_REG02__TCHALT:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_dmfc0_tchalt(arg, cpu_env);
            register_name = "TCHalt";
            break;
        case CP0_REG02__TCCONTEXT:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_dmfc0_tccontext(arg, cpu_env);
            register_name = "TCContext";
            break;
        case CP0_REG02__TCSCHEDULE:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_dmfc0_tcschedule(arg, cpu_env);
            register_name = "TCSchedule";
            break;
        case CP0_REG02__TCSCHEFBACK:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_dmfc0_tcschefback(arg, cpu_env);
            register_name = "TCScheFBack";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_03:
        switch (sel) {
        case CP0_REG03__ENTRYLO1:
            tcg_gen_ld_tl(arg, cpu_env, offsetof(CPUMIPSState, CP0_EntryLo1));
            register_name = "EntryLo1";
            break;
        case CP0_REG03__GLOBALNUM:
            CP0_CHECK(ctx->vp);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_GlobalNumber));
            register_name = "GlobalNumber";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_04:
        switch (sel) {
        case CP0_REG04__CONTEXT:
            tcg_gen_ld_tl(arg, cpu_env, offsetof(CPUMIPSState, CP0_Context));
            register_name = "Context";
            break;
        case CP0_REG04__CONTEXTCONFIG:
            /* SmartMIPS ASE */
            /* gen_helper_dmfc0_contextconfig(arg); */
            register_name = "ContextConfig";
            goto cp0_unimplemented;
        case CP0_REG04__USERLOCAL:
            CP0_CHECK(ctx->ulri);
            tcg_gen_ld_tl(arg, cpu_env,
                          offsetof(CPUMIPSState, active_tc.CP0_UserLocal));
            register_name = "UserLocal";
            break;
        case CP0_REG04__MMID:
            CP0_CHECK(ctx->mi);
            gen_helper_mtc0_memorymapid(cpu_env, arg);
            register_name = "MMID";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_05:
        switch (sel) {
        case CP0_REG05__PAGEMASK:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_PageMask));
            register_name = "PageMask";
            break;
        case CP0_REG05__PAGEGRAIN:
            check_insn(ctx, ISA_MIPS_R2);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_PageGrain));
            register_name = "PageGrain";
            break;
        case CP0_REG05__SEGCTL0:
            CP0_CHECK(ctx->sc);
            tcg_gen_ld_tl(arg, cpu_env, offsetof(CPUMIPSState, CP0_SegCtl0));
            register_name = "SegCtl0";
            break;
        case CP0_REG05__SEGCTL1:
            CP0_CHECK(ctx->sc);
            tcg_gen_ld_tl(arg, cpu_env, offsetof(CPUMIPSState, CP0_SegCtl1));
            register_name = "SegCtl1";
            break;
        case CP0_REG05__SEGCTL2:
            CP0_CHECK(ctx->sc);
            tcg_gen_ld_tl(arg, cpu_env, offsetof(CPUMIPSState, CP0_SegCtl2));
            register_name = "SegCtl2";
            break;
        case CP0_REG05__PWBASE:
            check_pw(ctx);
            tcg_gen_ld_tl(arg, cpu_env, offsetof(CPUMIPSState, CP0_PWBase));
            register_name = "PWBase";
            break;
        case CP0_REG05__PWFIELD:
            check_pw(ctx);
            tcg_gen_ld_tl(arg, cpu_env, offsetof(CPUMIPSState, CP0_PWField));
            register_name = "PWField";
            break;
        case CP0_REG05__PWSIZE:
            check_pw(ctx);
            tcg_gen_ld_tl(arg, cpu_env, offsetof(CPUMIPSState, CP0_PWSize));
            register_name = "PWSize";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_06:
        switch (sel) {
        case CP0_REG06__WIRED:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_Wired));
            register_name = "Wired";
            break;
        case CP0_REG06__SRSCONF0:
            check_insn(ctx, ISA_MIPS_R2);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_SRSConf0));
            register_name = "SRSConf0";
            break;
        case CP0_REG06__SRSCONF1:
            check_insn(ctx, ISA_MIPS_R2);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_SRSConf1));
            register_name = "SRSConf1";
            break;
        case CP0_REG06__SRSCONF2:
            check_insn(ctx, ISA_MIPS_R2);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_SRSConf2));
            register_name = "SRSConf2";
            break;
        case CP0_REG06__SRSCONF3:
            check_insn(ctx, ISA_MIPS_R2);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_SRSConf3));
            register_name = "SRSConf3";
            break;
        case CP0_REG06__SRSCONF4:
            check_insn(ctx, ISA_MIPS_R2);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_SRSConf4));
            register_name = "SRSConf4";
            break;
        case CP0_REG06__PWCTL:
            check_pw(ctx);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_PWCtl));
            register_name = "PWCtl";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_07:
        switch (sel) {
        case CP0_REG07__HWRENA:
            check_insn(ctx, ISA_MIPS_R2);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_HWREna));
            register_name = "HWREna";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_08:
        switch (sel) {
        case CP0_REG08__BADVADDR:
            tcg_gen_ld_tl(arg, cpu_env, offsetof(CPUMIPSState, CP0_BadVAddr));
            register_name = "BadVAddr";
            break;
        case CP0_REG08__BADINSTR:
            CP0_CHECK(ctx->bi);
            tcg_gen_ld32u_tl(arg, cpu_env, offsetof(CPUMIPSState, CP0_BadInstr));
            register_name = "BadInstr";
            break;
        case CP0_REG08__BADINSTRP:
            CP0_CHECK(ctx->bp);
            tcg_gen_ld32u_tl(arg, cpu_env, offsetof(CPUMIPSState, CP0_BadInstrP));
            register_name = "BadInstrP";
            break;
        case CP0_REG08__BADINSTRX:
            CP0_CHECK(ctx->bi);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_BadInstrX));
            tcg_gen_andi_tl(arg, arg, ~0xffff);
            register_name = "BadInstrX";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_09:
        switch (sel) {
        case CP0_REG09__COUNT:
            /* Mark as an IO operation because we read the time.  */
            if (tb_cflags(ctx->base.tb) & CF_USE_ICOUNT) {
                gen_io_start();
            }
            gen_helper_mfc0_count(arg, cpu_env);
            /*
             * Break the TB to be able to take timer interrupts immediately
             * after reading count. DISAS_STOP isn't sufficient, we need to
             * ensure we break completely out of translated code.
             */
            gen_save_pc(ctx->base.pc_next + 4);
            ctx->base.is_jmp = DISAS_EXIT;
            register_name = "Count";
            break;
#if !defined(TARGET_CHERI) /* SAAR Conflicts with RTC */
        case CP0_REG09__SAARI:
            CP0_CHECK(ctx->saar);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_SAARI));
            register_name = "SAARI";
            break;
        case CP0_REG09__SAAR:
            CP0_CHECK(ctx->saar);
            gen_helper_dmfc0_saar(arg, cpu_env);
            register_name = "SAAR";
            break;
#else
        /* 6,7 are implementation dependent */
        case 6:
            // QEMU-CHERI extension:
            gen_helper_mfc0_rtc64(arg, cpu_env);
            register_name = "RTC";
            break;
#endif
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_10:
        switch (sel) {
        case CP0_REG10__ENTRYHI:
            tcg_gen_ld_tl(arg, cpu_env, offsetof(CPUMIPSState, CP0_EntryHi));
            register_name = "EntryHi";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_11:
        switch (sel) {
        case CP0_REG11__COMPARE:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_Compare));
            register_name = "Compare";
            break;
        /* 6,7 are implementation dependent */
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_12:
        switch (sel) {
        case CP0_REG12__STATUS:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_Status));
            register_name = "Status";
            break;
        case CP0_REG12__INTCTL:
            check_insn(ctx, ISA_MIPS_R2);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_IntCtl));
            register_name = "IntCtl";
            break;
        case CP0_REG12__SRSCTL:
            check_insn(ctx, ISA_MIPS_R2);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_SRSCtl));
            register_name = "SRSCtl";
            break;
        case CP0_REG12__SRSMAP:
            check_insn(ctx, ISA_MIPS_R2);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_SRSMap));
            register_name = "SRSMap";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_13:
        switch (sel) {
        case CP0_REG13__CAUSE:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_Cause));
            register_name = "Cause";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_14:
        switch (sel) {
        case CP0_REG14__EPC:
#ifdef TARGET_CHERI
            gen_helper_mfc0_epc(arg, cpu_env);
#else
            tcg_gen_ld_tl(arg, cpu_env, offsetof(CPUMIPSState, CP0_EPC));
#endif
            register_name = "EPC";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_15:
        switch (sel) {
        case CP0_REG15__PRID:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_PRid));
            register_name = "PRid";
            break;
        case CP0_REG15__EBASE:
            check_insn(ctx, ISA_MIPS_R2);
            tcg_gen_ld_tl(arg, cpu_env, offsetof(CPUMIPSState, CP0_EBase));
            register_name = "EBase";
            break;
        case CP0_REG15__CMGCRBASE:
            check_insn(ctx, ISA_MIPS_R2);
            CP0_CHECK(ctx->cmgcr);
            tcg_gen_ld_tl(arg, cpu_env, offsetof(CPUMIPSState, CP0_CMGCRBase));
            register_name = "CMGCRBase";
            break;
        case 6:
            /*
             * See section 7.3.5 Core Identification (CPO Register 15,
             * Select 6 of the BERI Hardware Reference.
             */
            gen_helper_mfc0_coreid(arg, cpu_env);
            register_name = "CoreID";
            break;
        case 7:
            /*
             * See section 7.3.6 Thread Identification (CPO Register 15,
             * Select 7 of the BERI Hardware Reference.
             */
            tcg_gen_movi_tl(arg, 0); /* currently unimplemented */
            register_name = "ThreadID";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_16:
        switch (sel) {
        case CP0_REG16__CONFIG:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_Config0));
            register_name = "Config";
            break;
        case CP0_REG16__CONFIG1:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_Config1));
            register_name = "Config1";
            break;
        case CP0_REG16__CONFIG2:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_Config2));
            register_name = "Config2";
            break;
        case CP0_REG16__CONFIG3:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_Config3));
            register_name = "Config3";
            break;
        case CP0_REG16__CONFIG4:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_Config4));
            register_name = "Config4";
            break;
        case CP0_REG16__CONFIG5:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_Config5));
            register_name = "Config5";
            break;
        /* 6,7 are implementation dependent */
        case CP0_REG16__CONFIG6:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_Config6));
            register_name = "Config6";
            break;
        case CP0_REG16__CONFIG7:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_Config7));
            register_name = "Config7";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_17:
        switch (sel) {
        case CP0_REG17__LLADDR:
            gen_helper_dmfc0_lladdr(arg, cpu_env);
            register_name = "LLAddr";
            break;
        case CP0_REG17__MAAR:
            CP0_CHECK(ctx->mrp);
            gen_helper_dmfc0_maar(arg, cpu_env);
            register_name = "MAAR";
            break;
        case CP0_REG17__MAARI:
            CP0_CHECK(ctx->mrp);
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_MAARI));
            register_name = "MAARI";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_18:
        switch (sel) {
        case CP0_REG18__WATCHLO0:
        case CP0_REG18__WATCHLO1:
        case CP0_REG18__WATCHLO2:
        case CP0_REG18__WATCHLO3:
        case CP0_REG18__WATCHLO4:
        case CP0_REG18__WATCHLO5:
        case CP0_REG18__WATCHLO6:
        case CP0_REG18__WATCHLO7:
            CP0_CHECK(ctx->CP0_Config1 & (1 << CP0C1_WR));
            gen_helper_1e0i(dmfc0_watchlo, arg, sel);
            register_name = "WatchLo";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_19:
        switch (sel) {
        case CP0_REG19__WATCHHI0:
        case CP0_REG19__WATCHHI1:
        case CP0_REG19__WATCHHI2:
        case CP0_REG19__WATCHHI3:
        case CP0_REG19__WATCHHI4:
        case CP0_REG19__WATCHHI5:
        case CP0_REG19__WATCHHI6:
        case CP0_REG19__WATCHHI7:
            CP0_CHECK(ctx->CP0_Config1 & (1 << CP0C1_WR));
            gen_helper_1e0i(dmfc0_watchhi, arg, sel);
            register_name = "WatchHi";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_20:
        switch (sel) {
        case CP0_REG20__XCONTEXT:
            check_insn(ctx, ISA_MIPS3);
            tcg_gen_ld_tl(arg, cpu_env, offsetof(CPUMIPSState, CP0_XContext));
            register_name = "XContext";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_21:
        /* Officially reserved, but sel 0 is used for R1x000 framemask */
        CP0_CHECK(!(ctx->insn_flags & ISA_MIPS_R6));
        switch (sel) {
        case 0:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_Framemask));
            register_name = "Framemask";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_22:
        tcg_gen_movi_tl(arg, 0); /* unimplemented */
        register_name = "'Diagnostic"; /* implementation dependent */
        break;
    case CP0_REGISTER_23:
        switch (sel) {
        case CP0_REG23__DEBUG:
            gen_helper_mfc0_debug(arg, cpu_env); /* EJTAG support */
            register_name = "Debug";
            break;
        case CP0_REG23__TRACECONTROL:
            /* PDtrace support */
            /* gen_helper_dmfc0_tracecontrol(arg, cpu_env);  */
            register_name = "TraceControl";
            goto cp0_unimplemented;
        case CP0_REG23__TRACECONTROL2:
            /* PDtrace support */
            /* gen_helper_dmfc0_tracecontrol2(arg, cpu_env); */
            register_name = "TraceControl2";
            goto cp0_unimplemented;
        case CP0_REG23__USERTRACEDATA1:
            /* PDtrace support */
            /* gen_helper_dmfc0_usertracedata1(arg, cpu_env);*/
            register_name = "UserTraceData1";
            goto cp0_unimplemented;
        case CP0_REG23__TRACEIBPC:
            /* PDtrace support */
            /* gen_helper_dmfc0_traceibpc(arg, cpu_env);     */
            register_name = "TraceIBPC";
            goto cp0_unimplemented;
        case CP0_REG23__TRACEDBPC:
            /* PDtrace support */
            /* gen_helper_dmfc0_tracedbpc(arg, cpu_env);     */
            register_name = "TraceDBPC";
            goto cp0_unimplemented;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_24:
        switch (sel) {
        case CP0_REG24__DEPC:
            /* EJTAG support */
            tcg_gen_ld_tl(arg, cpu_env, offsetof(CPUMIPSState, CP0_DEPC));
            register_name = "DEPC";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_25:
        switch (sel) {
        case CP0_REG25__PERFCTL0:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_Performance0));
            register_name = "Performance0";
            break;
        case CP0_REG25__PERFCNT0:
            /* gen_helper_dmfc0_performance1(arg); */
            register_name = "Performance1";
            goto cp0_unimplemented;
        case CP0_REG25__PERFCTL1:
            /* gen_helper_dmfc0_performance2(arg); */
            register_name = "Performance2";
            goto cp0_unimplemented;
        case CP0_REG25__PERFCNT1:
            /* gen_helper_dmfc0_performance3(arg); */
            register_name = "Performance3";
            goto cp0_unimplemented;
        case CP0_REG25__PERFCTL2:
            /* gen_helper_dmfc0_performance4(arg); */
            register_name = "Performance4";
            goto cp0_unimplemented;
        case CP0_REG25__PERFCNT2:
            /* gen_helper_dmfc0_performance5(arg); */
            register_name = "Performance5";
            goto cp0_unimplemented;
        case CP0_REG25__PERFCTL3:
            /* gen_helper_dmfc0_performance6(arg); */
            register_name = "Performance6";
            goto cp0_unimplemented;
        case CP0_REG25__PERFCNT3:
            /* gen_helper_dmfc0_performance7(arg); */
            register_name = "Performance7";
            goto cp0_unimplemented;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_26:
        switch (sel) {
        case CP0_REG26__ERRCTL:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_ErrCtl));
            register_name = "ErrCtl";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_27:
        switch (sel) {
        /* ignored */
        case CP0_REG27__CACHERR:
            tcg_gen_movi_tl(arg, 0); /* unimplemented */
            register_name = "CacheErr";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_28:
        switch (sel) {
        case CP0_REG28__TAGLO:
        case CP0_REG28__TAGLO1:
        case CP0_REG28__TAGLO2:
        case CP0_REG28__TAGLO3:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_TagLo));
            register_name = "TagLo";
            break;
        case CP0_REG28__DATALO:
        case CP0_REG28__DATALO1:
        case CP0_REG28__DATALO2:
        case CP0_REG28__DATALO3:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_DataLo));
            register_name = "DataLo";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_29:
        switch (sel) {
        case CP0_REG29__TAGHI:
        case CP0_REG29__TAGHI1:
        case CP0_REG29__TAGHI2:
        case CP0_REG29__TAGHI3:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_TagHi));
            register_name = "TagHi";
            break;
        case CP0_REG29__DATAHI:
        case CP0_REG29__DATAHI1:
        case CP0_REG29__DATAHI2:
        case CP0_REG29__DATAHI3:
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_DataHi));
            register_name = "DataHi";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_30:
        switch (sel) {
        case CP0_REG30__ERROREPC:
#ifdef TARGET_CHERI
            gen_helper_mfc0_error_epc(arg, cpu_env);
#else
            tcg_gen_ld_tl(arg, cpu_env, offsetof(CPUMIPSState, CP0_ErrorEPC));
#endif
            register_name = "ErrorEPC";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_31:
        switch (sel) {
        case CP0_REG31__DESAVE:
            /* EJTAG support */
            gen_mfc0_load32(arg, offsetof(CPUMIPSState, CP0_DESAVE));
            register_name = "DESAVE";
            break;
        case CP0_REG31__KSCRATCH1:
        case CP0_REG31__KSCRATCH2:
        case CP0_REG31__KSCRATCH3:
        case CP0_REG31__KSCRATCH4:
        case CP0_REG31__KSCRATCH5:
        case CP0_REG31__KSCRATCH6:
            CP0_CHECK(ctx->kscrexist & (1 << sel));
            tcg_gen_ld_tl(arg, cpu_env,
                          offsetof(CPUMIPSState, CP0_KScratch[sel - 2]));
            register_name = "KScratch";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    default:
        goto cp0_unimplemented;
    }
    trace_mips_translate_c0("dmfc0", register_name, reg, sel);
    return;

cp0_unimplemented:
    qemu_log_mask(LOG_UNIMP, "dmfc0 %s (reg %d sel %d)\n",
                  register_name, reg, sel);
    gen_mfc0_unimplemented(ctx, arg);
}

static void gen_dmtc0(DisasContext *ctx, TCGv arg, int reg, int sel)
{
    const char *register_name = "invalid";

    if (sel != 0) {
        check_insn(ctx, ISA_MIPS_R1);
    }

    if (tb_cflags(ctx->base.tb) & CF_USE_ICOUNT) {
        gen_io_start();
    }

    switch (reg) {
    case CP0_REGISTER_00:
        switch (sel) {
        case CP0_REG00__INDEX:
            gen_helper_mtc0_index(cpu_env, arg);
            register_name = "Index";
            break;
        case CP0_REG00__MVPCONTROL:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mtc0_mvpcontrol(cpu_env, arg);
            register_name = "MVPControl";
            break;
        case CP0_REG00__MVPCONF0:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            /* ignored */
            register_name = "MVPConf0";
            break;
        case CP0_REG00__MVPCONF1:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            /* ignored */
            register_name = "MVPConf1";
            break;
        case CP0_REG00__VPCONTROL:
            CP0_CHECK(ctx->vp);
            /* ignored */
            register_name = "VPControl";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_01:
        switch (sel) {
        case CP0_REG01__RANDOM:
            /* ignored */
            register_name = "Random";
            break;
        case CP0_REG01__VPECONTROL:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mtc0_vpecontrol(cpu_env, arg);
            register_name = "VPEControl";
            break;
        case CP0_REG01__VPECONF0:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mtc0_vpeconf0(cpu_env, arg);
            register_name = "VPEConf0";
            break;
        case CP0_REG01__VPECONF1:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mtc0_vpeconf1(cpu_env, arg);
            register_name = "VPEConf1";
            break;
        case CP0_REG01__YQMASK:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mtc0_yqmask(cpu_env, arg);
            register_name = "YQMask";
            break;
        case CP0_REG01__VPESCHEDULE:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_mtc0_store(ctx, arg, reg, sel,
                           offsetof(CPUMIPSState, CP0_VPESchedule));
            register_name = "VPESchedule";
            break;
        case CP0_REG01__VPESCHEFBACK:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_mtc0_store(ctx, arg, reg, sel,
                          offsetof(CPUMIPSState, CP0_VPEScheFBack));
            register_name = "VPEScheFBack";
            break;
        case CP0_REG01__VPEOPT:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mtc0_vpeopt(cpu_env, arg);
            register_name = "VPEOpt";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_02:
        switch (sel) {
        case CP0_REG02__ENTRYLO0:
            gen_helper_dmtc0_entrylo0(cpu_env, arg);
            register_name = "EntryLo0";
            break;
        case CP0_REG02__TCSTATUS:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mtc0_tcstatus(cpu_env, arg);
            register_name = "TCStatus";
            break;
        case CP0_REG02__TCBIND:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mtc0_tcbind(cpu_env, arg);
            register_name = "TCBind";
            break;
        case CP0_REG02__TCRESTART:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mtc0_tcrestart(cpu_env, arg);
            register_name = "TCRestart";
            break;
        case CP0_REG02__TCHALT:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mtc0_tchalt(cpu_env, arg);
            register_name = "TCHalt";
            break;
        case CP0_REG02__TCCONTEXT:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mtc0_tccontext(cpu_env, arg);
            register_name = "TCContext";
            break;
        case CP0_REG02__TCSCHEDULE:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mtc0_tcschedule(cpu_env, arg);
            register_name = "TCSchedule";
            break;
        case CP0_REG02__TCSCHEFBACK:
            CP0_CHECK(ctx->insn_flags & ASE_MT);
            gen_helper_mtc0_tcschefback(cpu_env, arg);
            register_name = "TCScheFBack";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_03:
        switch (sel) {
        case CP0_REG03__ENTRYLO1:
            gen_helper_dmtc0_entrylo1(cpu_env, arg);
            register_name = "EntryLo1";
            break;
        case CP0_REG03__GLOBALNUM:
            CP0_CHECK(ctx->vp);
            /* ignored */
            register_name = "GlobalNumber";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_04:
        switch (sel) {
        case CP0_REG04__CONTEXT:
            gen_helper_mtc0_context(cpu_env, arg);
            register_name = "Context";
            break;
        case CP0_REG04__CONTEXTCONFIG:
            /* SmartMIPS ASE */
            /* gen_helper_dmtc0_contextconfig(arg); */
            register_name = "ContextConfig";
            goto cp0_unimplemented;
        case CP0_REG04__USERLOCAL:
            CP0_CHECK(ctx->ulri);
            gen_mtc0_store(ctx, arg, reg, sel,
                          offsetof(CPUMIPSState, active_tc.CP0_UserLocal));
            register_name = "UserLocal";
            break;
        case CP0_REG04__MMID:
            CP0_CHECK(ctx->mi);
            gen_mtc0_store32(ctx, arg, reg, sel,
                             offsetof(CPUMIPSState, CP0_MemoryMapID));
            register_name = "MMID";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_05:
        switch (sel) {
        case CP0_REG05__PAGEMASK:
            gen_helper_mtc0_pagemask(cpu_env, arg);
            register_name = "PageMask";
            break;
        case CP0_REG05__PAGEGRAIN:
            check_insn(ctx, ISA_MIPS_R2);
            gen_helper_mtc0_pagegrain(cpu_env, arg);
            register_name = "PageGrain";
            break;
        case CP0_REG05__SEGCTL0:
            CP0_CHECK(ctx->sc);
            gen_helper_mtc0_segctl0(cpu_env, arg);
            register_name = "SegCtl0";
            break;
        case CP0_REG05__SEGCTL1:
            CP0_CHECK(ctx->sc);
            gen_helper_mtc0_segctl1(cpu_env, arg);
            register_name = "SegCtl1";
            break;
        case CP0_REG05__SEGCTL2:
            CP0_CHECK(ctx->sc);
            gen_helper_mtc0_segctl2(cpu_env, arg);
            register_name = "SegCtl2";
            break;
        case CP0_REG05__PWBASE:
            check_pw(ctx);
            gen_mtc0_store(ctx, arg, reg, sel,
                           offsetof(CPUMIPSState, CP0_PWBase));
            register_name = "PWBase";
            break;
        case CP0_REG05__PWFIELD:
            check_pw(ctx);
            gen_helper_mtc0_pwfield(cpu_env, arg);
            register_name = "PWField";
            break;
        case CP0_REG05__PWSIZE:
            check_pw(ctx);
            gen_helper_mtc0_pwsize(cpu_env, arg);
            register_name = "PWSize";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_06:
        switch (sel) {
        case CP0_REG06__WIRED:
            gen_helper_mtc0_wired(cpu_env, arg);
            register_name = "Wired";
            break;
        case CP0_REG06__SRSCONF0:
            check_insn(ctx, ISA_MIPS_R2);
            gen_helper_mtc0_srsconf0(cpu_env, arg);
            register_name = "SRSConf0";
            break;
        case CP0_REG06__SRSCONF1:
            check_insn(ctx, ISA_MIPS_R2);
            gen_helper_mtc0_srsconf1(cpu_env, arg);
            register_name = "SRSConf1";
            break;
        case CP0_REG06__SRSCONF2:
            check_insn(ctx, ISA_MIPS_R2);
            gen_helper_mtc0_srsconf2(cpu_env, arg);
            register_name = "SRSConf2";
            break;
        case CP0_REG06__SRSCONF3:
            check_insn(ctx, ISA_MIPS_R2);
            gen_helper_mtc0_srsconf3(cpu_env, arg);
            register_name = "SRSConf3";
            break;
        case CP0_REG06__SRSCONF4:
            check_insn(ctx, ISA_MIPS_R2);
            gen_helper_mtc0_srsconf4(cpu_env, arg);
            register_name = "SRSConf4";
            break;
        case CP0_REG06__PWCTL:
            check_pw(ctx);
            gen_helper_mtc0_pwctl(cpu_env, arg);
            register_name = "PWCtl";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_07:
        switch (sel) {
        case CP0_REG07__HWRENA:
            check_insn(ctx, ISA_MIPS_R2);
            gen_helper_mtc0_hwrena(cpu_env, arg);
            ctx->base.is_jmp = DISAS_STOP;
            register_name = "HWREna";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_08:
        switch (sel) {
        case CP0_REG08__BADVADDR:
            /* ignored */
            register_name = "BadVAddr";
            break;
        case CP0_REG08__BADINSTR:
            /* ignored */
            register_name = "BadInstr";
            break;
        case CP0_REG08__BADINSTRP:
            /* ignored */
            register_name = "BadInstrP";
            break;
        case CP0_REG08__BADINSTRX:
            /* ignored */
            register_name = "BadInstrX";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_09:
        switch (sel) {
        case CP0_REG09__COUNT:
            gen_helper_mtc0_count(cpu_env, arg);
            register_name = "Count";
            break;
#if !defined(TARGET_CHERI) /* Conflicts with RTC */
        case CP0_REG09__SAARI:
            CP0_CHECK(ctx->saar);
            gen_helper_mtc0_saari(cpu_env, arg);
            register_name = "SAARI";
            break;
        case CP0_REG09__SAAR:
            CP0_CHECK(ctx->saar);
            gen_helper_mtc0_saar(cpu_env, arg);
            register_name = "SAAR";
            break;
#else
        /* 6,7 are implementation dependent */
        case 6:
            gen_helper_mtc0_rtc64(cpu_env, arg);
            register_name = "RTC";
            break;
#endif
        default:
            goto cp0_unimplemented;
        }
        /* Stop translation as we may have switched the execution mode */
        ctx->base.is_jmp = DISAS_STOP;
        break;
    case CP0_REGISTER_10:
        switch (sel) {
        case CP0_REG10__ENTRYHI:
            gen_helper_mtc0_entryhi(cpu_env, arg);
            register_name = "EntryHi";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_11:
        switch (sel) {
        case CP0_REG11__COMPARE:
            gen_helper_mtc0_compare(cpu_env, arg);
            register_name = "Compare";
            break;
        /* 6,7 are implementation dependent */
        default:
            goto cp0_unimplemented;
        }
        /* Stop translation as we may have switched the execution mode */
        ctx->base.is_jmp = DISAS_STOP;
        break;
    case CP0_REGISTER_12:
        switch (sel) {
        case CP0_REG12__STATUS:
            save_cpu_state(ctx, 1);
            gen_helper_mtc0_status(cpu_env, arg);
            /* DISAS_STOP isn't good enough here, hflags may have changed. */
            gen_save_pc(ctx->base.pc_next + 4);
            ctx->base.is_jmp = DISAS_EXIT;
            register_name = "Status";
            break;
        case CP0_REG12__INTCTL:
            check_insn(ctx, ISA_MIPS_R2);
            gen_helper_mtc0_intctl(cpu_env, arg);
            /* Stop translation as we may have switched the execution mode */
            ctx->base.is_jmp = DISAS_STOP;
            register_name = "IntCtl";
            break;
        case CP0_REG12__SRSCTL:
            check_insn(ctx, ISA_MIPS_R2);
            gen_helper_mtc0_srsctl(cpu_env, arg);
            /* Stop translation as we may have switched the execution mode */
            ctx->base.is_jmp = DISAS_STOP;
            register_name = "SRSCtl";
            break;
        case CP0_REG12__SRSMAP:
            check_insn(ctx, ISA_MIPS_R2);
            gen_mtc0_store32(ctx, arg, reg, sel,
                             offsetof(CPUMIPSState, CP0_SRSMap));
            /* Stop translation as we may have switched the execution mode */
            ctx->base.is_jmp = DISAS_STOP;
            register_name = "SRSMap";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_13:
        switch (sel) {
        case CP0_REG13__CAUSE:
            save_cpu_state(ctx, 1);
            gen_helper_mtc0_cause(cpu_env, arg);
            /*
             * Stop translation as we may have triggered an interrupt.
             * DISAS_STOP isn't sufficient, we need to ensure we break out of
             * translated code to check for pending interrupts.
             */
            gen_save_pc(ctx->base.pc_next + 4);
            ctx->base.is_jmp = DISAS_EXIT;
            register_name = "Cause";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_14:
        switch (sel) {
        case CP0_REG14__EPC:
#ifdef TARGET_CHERI
            gen_helper_mtc0_epc(cpu_env, arg);
#else
            gen_mtc0_store(ctx, arg, reg, sel,
                           offsetof(CPUMIPSState, CP0_EPC));
#endif
            register_name = "EPC";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_15:
        switch (sel) {
        case CP0_REG15__PRID:
            /* ignored */
            register_name = "PRid";
            break;
        case CP0_REG15__EBASE:
            check_insn(ctx, ISA_MIPS_R2);
            gen_helper_mtc0_ebase(cpu_env, arg);
            register_name = "EBase";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_16:
        switch (sel) {
        case CP0_REG16__CONFIG:
            gen_helper_mtc0_config0(cpu_env, arg);
            register_name = "Config";
            /* Stop translation as we may have switched the execution mode */
            ctx->base.is_jmp = DISAS_STOP;
            break;
        case CP0_REG16__CONFIG1:
            /* ignored, read only */
            register_name = "Config1";
            break;
        case CP0_REG16__CONFIG2:
            gen_helper_mtc0_config2(cpu_env, arg);
            register_name = "Config2";
            /* Stop translation as we may have switched the execution mode */
            ctx->base.is_jmp = DISAS_STOP;
            break;
        case CP0_REG16__CONFIG3:
            gen_helper_mtc0_config3(cpu_env, arg);
            register_name = "Config3";
            /* Stop translation as we may have switched the execution mode */
            ctx->base.is_jmp = DISAS_STOP;
            break;
        case CP0_REG16__CONFIG4:
            /* currently ignored */
            register_name = "Config4";
            break;
        case CP0_REG16__CONFIG5:
            gen_helper_mtc0_config5(cpu_env, arg);
            register_name = "Config5";
            /* Stop translation as we may have switched the execution mode */
            ctx->base.is_jmp = DISAS_STOP;
            break;
        /* 6,7 are implementation dependent */
        default:
            register_name = "Invalid config selector";
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_17:
        switch (sel) {
        case CP0_REG17__LLADDR:
            gen_helper_mtc0_lladdr(cpu_env, arg);
            register_name = "LLAddr";
            break;
        case CP0_REG17__MAAR:
            CP0_CHECK(ctx->mrp);
            gen_helper_mtc0_maar(cpu_env, arg);
            register_name = "MAAR";
            break;
        case CP0_REG17__MAARI:
            CP0_CHECK(ctx->mrp);
            gen_helper_mtc0_maari(cpu_env, arg);
            register_name = "MAARI";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_18:
        switch (sel) {
        case CP0_REG18__WATCHLO0:
        case CP0_REG18__WATCHLO1:
        case CP0_REG18__WATCHLO2:
        case CP0_REG18__WATCHLO3:
        case CP0_REG18__WATCHLO4:
        case CP0_REG18__WATCHLO5:
        case CP0_REG18__WATCHLO6:
        case CP0_REG18__WATCHLO7:
            CP0_CHECK(ctx->CP0_Config1 & (1 << CP0C1_WR));
            gen_helper_0e1i(mtc0_watchlo, arg, sel);
            register_name = "WatchLo";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_19:
        switch (sel) {
        case CP0_REG19__WATCHHI0:
        case CP0_REG19__WATCHHI1:
        case CP0_REG19__WATCHHI2:
        case CP0_REG19__WATCHHI3:
        case CP0_REG19__WATCHHI4:
        case CP0_REG19__WATCHHI5:
        case CP0_REG19__WATCHHI6:
        case CP0_REG19__WATCHHI7:
            CP0_CHECK(ctx->CP0_Config1 & (1 << CP0C1_WR));
            gen_helper_0e1i(mtc0_watchhi, arg, sel);
            register_name = "WatchHi";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_20:
        switch (sel) {
        case CP0_REG20__XCONTEXT:
            check_insn(ctx, ISA_MIPS3);
            gen_helper_mtc0_xcontext(cpu_env, arg);
            register_name = "XContext";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_21:
       /* Officially reserved, but sel 0 is used for R1x000 framemask */
        CP0_CHECK(!(ctx->insn_flags & ISA_MIPS_R6));
        switch (sel) {
        case 0:
            gen_helper_mtc0_framemask(cpu_env, arg);
            register_name = "Framemask";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_22:
        /* ignored */
        register_name = "Diagnostic"; /* implementation dependent */
        break;
    case CP0_REGISTER_23:
        switch (sel) {
        case CP0_REG23__DEBUG:
            save_cpu_state(ctx, 1); // Need to sync PC (PCC.cursor)
            gen_helper_mtc0_debug(cpu_env, arg); /* EJTAG support */
            /* DISAS_STOP isn't good enough here, hflags may have changed. */
            gen_save_pc(ctx->base.pc_next + 4);
            ctx->base.is_jmp = DISAS_EXIT;
            register_name = "Debug";
            break;
        case CP0_REG23__TRACECONTROL:
            /* PDtrace support */
            /* gen_helper_mtc0_tracecontrol(cpu_env, arg);  */
            /* Stop translation as we may have switched the execution mode */
            ctx->base.is_jmp = DISAS_STOP;
            register_name = "TraceControl";
            goto cp0_unimplemented;
        case CP0_REG23__TRACECONTROL2:
            /* PDtrace support */
            /* gen_helper_mtc0_tracecontrol2(cpu_env, arg); */
            /* Stop translation as we may have switched the execution mode */
            ctx->base.is_jmp = DISAS_STOP;
            register_name = "TraceControl2";
            goto cp0_unimplemented;
        case CP0_REG23__USERTRACEDATA1:
            /* PDtrace support */
            /* gen_helper_mtc0_usertracedata1(cpu_env, arg);*/
            /* Stop translation as we may have switched the execution mode */
            ctx->base.is_jmp = DISAS_STOP;
            register_name = "UserTraceData1";
            goto cp0_unimplemented;
        case CP0_REG23__TRACEIBPC:
            /* PDtrace support */
            /* gen_helper_mtc0_traceibpc(cpu_env, arg);     */
            /* Stop translation as we may have switched the execution mode */
            ctx->base.is_jmp = DISAS_STOP;
            register_name = "TraceIBPC";
            goto cp0_unimplemented;
        case CP0_REG23__TRACEDBPC:
            /* PDtrace support */
            /* gen_helper_mtc0_tracedbpc(cpu_env, arg);     */
            /* Stop translation as we may have switched the execution mode */
            ctx->base.is_jmp = DISAS_STOP;
            register_name = "TraceDBPC";
            goto cp0_unimplemented;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_24:
        switch (sel) {
        case CP0_REG24__DEPC:
            /* EJTAG support */
            gen_mtc0_store(ctx, arg, reg, sel, offsetof(CPUMIPSState, CP0_DEPC));
            register_name = "DEPC";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_25:
        switch (sel) {
        case CP0_REG25__PERFCTL0:
            gen_helper_mtc0_performance0(cpu_env, arg);
            register_name = "Performance0";
            break;
        case CP0_REG25__PERFCNT0:
            /* gen_helper_mtc0_performance1(cpu_env, arg); */
            register_name = "Performance1";
            goto cp0_unimplemented;
        case CP0_REG25__PERFCTL1:
            /* gen_helper_mtc0_performance2(cpu_env, arg); */
            register_name = "Performance2";
            goto cp0_unimplemented;
        case CP0_REG25__PERFCNT1:
            /* gen_helper_mtc0_performance3(cpu_env, arg); */
            register_name = "Performance3";
            goto cp0_unimplemented;
        case CP0_REG25__PERFCTL2:
            /* gen_helper_mtc0_performance4(cpu_env, arg); */
            register_name = "Performance4";
            goto cp0_unimplemented;
        case CP0_REG25__PERFCNT2:
            /* gen_helper_mtc0_performance5(cpu_env, arg); */
            register_name = "Performance5";
            goto cp0_unimplemented;
        case CP0_REG25__PERFCTL3:
            /* gen_helper_mtc0_performance6(cpu_env, arg); */
            register_name = "Performance6";
            goto cp0_unimplemented;
        case CP0_REG25__PERFCNT3:
            /* gen_helper_mtc0_performance7(cpu_env, arg); */
            register_name = "Performance7";
            goto cp0_unimplemented;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_26:
        save_cpu_state(ctx, 1); // Need to sync PC (PCC.cursor)
        gen_helper_mtc0_dumpstate(cpu_env, arg); /* CHERI: dump reg state */
#if !defined(TARGET_CHERI)
        switch (sel) {
        case CP0_REG26__ERRCTL:
            gen_helper_mtc0_errctl(cpu_env, arg);
            ctx->base.is_jmp = DISAS_STOP;
            register_name = "ErrCtl";
            break;
        default:
            goto cp0_unimplemented;
        }
#endif /* TARGET_CHERI */
        break;
    case CP0_REGISTER_27:
        switch (sel) {
        case CP0_REG27__CACHERR:
            /* ignored */
            register_name = "CacheErr";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_28:
        switch (sel) {
        case CP0_REG28__TAGLO:
        case CP0_REG28__TAGLO1:
        case CP0_REG28__TAGLO2:
        case CP0_REG28__TAGLO3:
            gen_helper_mtc0_taglo(cpu_env, arg);
            register_name = "TagLo";
            break;
        case CP0_REG28__DATALO:
        case CP0_REG28__DATALO1:
        case CP0_REG28__DATALO2:
        case CP0_REG28__DATALO3:
            gen_helper_mtc0_datalo(cpu_env, arg);
            register_name = "DataLo";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_29:
        switch (sel) {
        case CP0_REG29__TAGHI:
        case CP0_REG29__TAGHI1:
        case CP0_REG29__TAGHI2:
        case CP0_REG29__TAGHI3:
            gen_helper_mtc0_taghi(cpu_env, arg);
            register_name = "TagHi";
            break;
        case CP0_REG29__DATAHI:
        case CP0_REG29__DATAHI1:
        case CP0_REG29__DATAHI2:
        case CP0_REG29__DATAHI3:
            gen_helper_mtc0_datahi(cpu_env, arg);
            register_name = "DataHi";
            break;
        default:
            register_name = "invalid sel";
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_30:
        switch (sel) {
        case CP0_REG30__ERROREPC:
#ifdef TARGET_CHERI
            gen_helper_mtc0_error_epc(cpu_env, arg);
#else
            gen_mtc0_store(ctx, arg, reg, sel,
                           offsetof(CPUMIPSState, CP0_ErrorEPC));
#endif
            register_name = "ErrorEPC";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    case CP0_REGISTER_31:
        switch (sel) {
        case CP0_REG31__DESAVE:
            /* EJTAG support */
            gen_mtc0_store32(ctx, arg, reg, sel,
                             offsetof(CPUMIPSState, CP0_DESAVE));
            register_name = "DESAVE";
            break;
        case CP0_REG31__KSCRATCH1:
        case CP0_REG31__KSCRATCH2:
        case CP0_REG31__KSCRATCH3:
        case CP0_REG31__KSCRATCH4:
        case CP0_REG31__KSCRATCH5:
        case CP0_REG31__KSCRATCH6:
            CP0_CHECK(ctx->kscrexist & (1 << sel));
            gen_mtc0_store(ctx, arg, reg, sel,
                           offsetof(CPUMIPSState, CP0_KScratch[sel - 2]));
            register_name = "KScratch";
            break;
        default:
            goto cp0_unimplemented;
        }
        break;
    default:
        goto cp0_unimplemented;
    }
    trace_mips_translate_c0("dmtc0", register_name, reg, sel);

    /* For simplicity assume that all writes can cause interrupts.  */
    if (tb_cflags(ctx->base.tb) & CF_USE_ICOUNT) {
        /*
         * DISAS_STOP isn't sufficient, we need to ensure we break out of
         * translated code to check for pending interrupts.
         */
        gen_save_pc(ctx->base.pc_next + 4);
        ctx->base.is_jmp = DISAS_EXIT;
    }
    return;

cp0_unimplemented:
    qemu_log_mask(LOG_UNIMP, "dmtc0 %s (reg %d sel %d)\n",
                  register_name, reg, sel);
}
#endif /* TARGET_MIPS64 */

static void gen_mftr(CPUMIPSState *env, DisasContext *ctx, int rt, int rd,
                     int u, int sel, int h)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    TCGv t0 = tcg_temp_local_new();

    if ((env->CP0_VPEConf0 & (1 << CP0VPEC0_MVP)) == 0 &&
        ((env->tcs[other_tc].CP0_TCBind & (0xf << CP0TCBd_CurVPE)) !=
         (env->active_tc.CP0_TCBind & (0xf << CP0TCBd_CurVPE)))) {
        tcg_gen_movi_tl(t0, -1);
    } else if ((env->CP0_VPEControl & (0xff << CP0VPECo_TargTC)) >
               (env->mvp->CP0_MVPConf0 & (0xff << CP0MVPC0_PTC))) {
        tcg_gen_movi_tl(t0, -1);
    } else if (u == 0) {
        switch (rt) {
        case 1:
            switch (sel) {
            case 1:
                gen_helper_mftc0_vpecontrol(t0, cpu_env);
                break;
            case 2:
                gen_helper_mftc0_vpeconf0(t0, cpu_env);
                break;
            default:
                goto die;
                break;
            }
            break;
        case 2:
            switch (sel) {
            case 1:
                gen_helper_mftc0_tcstatus(t0, cpu_env);
                break;
            case 2:
                gen_helper_mftc0_tcbind(t0, cpu_env);
                break;
            case 3:
                gen_helper_mftc0_tcrestart(t0, cpu_env);
                break;
            case 4:
                gen_helper_mftc0_tchalt(t0, cpu_env);
                break;
            case 5:
                gen_helper_mftc0_tccontext(t0, cpu_env);
                break;
            case 6:
                gen_helper_mftc0_tcschedule(t0, cpu_env);
                break;
            case 7:
                gen_helper_mftc0_tcschefback(t0, cpu_env);
                break;
            default:
                gen_mfc0(ctx, t0, rt, sel);
                break;
            }
            break;
        case 10:
            switch (sel) {
            case 0:
                gen_helper_mftc0_entryhi(t0, cpu_env);
                break;
            default:
                gen_mfc0(ctx, t0, rt, sel);
                break;
            }
            break;
        case 12:
            switch (sel) {
            case 0:
                gen_helper_mftc0_status(t0, cpu_env);
                break;
            default:
                gen_mfc0(ctx, t0, rt, sel);
                break;
            }
            break;
        case 13:
            switch (sel) {
            case 0:
                gen_helper_mftc0_cause(t0, cpu_env);
                break;
            default:
                goto die;
                break;
            }
            break;
        case 14:
            switch (sel) {
            case 0:
                gen_helper_mftc0_epc(t0, cpu_env);
                break;
            default:
                goto die;
                break;
            }
            break;
        case 15:
            switch (sel) {
            case 1:
                gen_helper_mftc0_ebase(t0, cpu_env);
                break;
            default:
                goto die;
                break;
            }
            break;
        case 16:
            switch (sel) {
            case 0:
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            case 7:
                gen_helper_mftc0_configx(t0, cpu_env, tcg_const_tl(sel));
                break;
            default:
                goto die;
                break;
            }
            break;
        case 23:
            switch (sel) {
            case 0:
                gen_helper_mftc0_debug(t0, cpu_env);
                break;
            default:
                gen_mfc0(ctx, t0, rt, sel);
                break;
            }
            break;
        default:
            gen_mfc0(ctx, t0, rt, sel);
        }
    } else {
        switch (sel) {
        /* GPR registers. */
        case 0:
            gen_helper_1e0i(mftgpr, t0, rt);
            break;
        /* Auxiliary CPU registers */
        case 1:
            switch (rt) {
            case 0:
                gen_helper_1e0i(mftlo, t0, 0);
                break;
            case 1:
                gen_helper_1e0i(mfthi, t0, 0);
                break;
            case 2:
                gen_helper_1e0i(mftacx, t0, 0);
                break;
            case 4:
                gen_helper_1e0i(mftlo, t0, 1);
                break;
            case 5:
                gen_helper_1e0i(mfthi, t0, 1);
                break;
            case 6:
                gen_helper_1e0i(mftacx, t0, 1);
                break;
            case 8:
                gen_helper_1e0i(mftlo, t0, 2);
                break;
            case 9:
                gen_helper_1e0i(mfthi, t0, 2);
                break;
            case 10:
                gen_helper_1e0i(mftacx, t0, 2);
                break;
            case 12:
                gen_helper_1e0i(mftlo, t0, 3);
                break;
            case 13:
                gen_helper_1e0i(mfthi, t0, 3);
                break;
            case 14:
                gen_helper_1e0i(mftacx, t0, 3);
                break;
            case 16:
                gen_helper_mftdsp(t0, cpu_env);
                break;
            default:
                goto die;
            }
            break;
        /* Floating point (COP1). */
        case 2:
            /* XXX: For now we support only a single FPU context. */
            if (h == 0) {
                TCGv_i32 fp0 = tcg_temp_new_i32();

                gen_load_fpr32(ctx, fp0, rt);
                tcg_gen_ext_i32_tl(t0, fp0);
                tcg_temp_free_i32(fp0);
            } else {
                TCGv_i32 fp0 = tcg_temp_new_i32();

                gen_load_fpr32h(ctx, fp0, rt);
                tcg_gen_ext_i32_tl(t0, fp0);
                tcg_temp_free_i32(fp0);
            }
            break;
        case 3:
            /* XXX: For now we support only a single FPU context. */
            gen_helper_1e0i(cfc1, t0, rt);
            break;
        /* COP2: Not implemented. */
        case 4:
        case 5:
            /* fall through */
        default:
            goto die;
        }
    }
    trace_mips_translate_tr("mftr", rt, u, sel, h);
    gen_store_gpr(t0, rd);
    tcg_temp_free(t0);
    return;

die:
    tcg_temp_free(t0);
    LOG_DISAS("mftr (reg %d u %d sel %d h %d)\n", rt, u, sel, h);
    gen_reserved_instruction(ctx);
}

static void gen_mttr(CPUMIPSState *env, DisasContext *ctx, int rd, int rt,
                     int u, int sel, int h)
{
    int other_tc = env->CP0_VPEControl & (0xff << CP0VPECo_TargTC);
    TCGv t0 = tcg_temp_local_new();

    gen_load_gpr(t0, rt);
    if ((env->CP0_VPEConf0 & (1 << CP0VPEC0_MVP)) == 0 &&
        ((env->tcs[other_tc].CP0_TCBind & (0xf << CP0TCBd_CurVPE)) !=
         (env->active_tc.CP0_TCBind & (0xf << CP0TCBd_CurVPE)))) {
        /* NOP */
        ;
    } else if ((env->CP0_VPEControl & (0xff << CP0VPECo_TargTC)) >
             (env->mvp->CP0_MVPConf0 & (0xff << CP0MVPC0_PTC))) {
        /* NOP */
        ;
    } else if (u == 0) {
        switch (rd) {
        case 1:
            switch (sel) {
            case 1:
                gen_helper_mttc0_vpecontrol(cpu_env, t0);
                break;
            case 2:
                gen_helper_mttc0_vpeconf0(cpu_env, t0);
                break;
            default:
                goto die;
                break;
            }
            break;
        case 2:
            switch (sel) {
            case 1:
                gen_helper_mttc0_tcstatus(cpu_env, t0);
                break;
            case 2:
                gen_helper_mttc0_tcbind(cpu_env, t0);
                break;
            case 3:
                gen_helper_mttc0_tcrestart(cpu_env, t0);
                break;
            case 4:
                gen_helper_mttc0_tchalt(cpu_env, t0);
                break;
            case 5:
                gen_helper_mttc0_tccontext(cpu_env, t0);
                break;
            case 6:
                gen_helper_mttc0_tcschedule(cpu_env, t0);
                break;
            case 7:
                gen_helper_mttc0_tcschefback(cpu_env, t0);
                break;
            default:
                gen_mtc0(ctx, t0, rd, sel);
                break;
            }
            break;
        case 10:
            switch (sel) {
            case 0:
                gen_helper_mttc0_entryhi(cpu_env, t0);
                break;
            default:
                gen_mtc0(ctx, t0, rd, sel);
                break;
            }
            break;
        case 12:
            switch (sel) {
            case 0:
                gen_helper_mttc0_status(cpu_env, t0);
                break;
            default:
                gen_mtc0(ctx, t0, rd, sel);
                break;
            }
            break;
        case 13:
            switch (sel) {
            case 0:
                gen_helper_mttc0_cause(cpu_env, t0);
                break;
            default:
                goto die;
                break;
            }
            break;
        case 15:
            switch (sel) {
            case 1:
                gen_helper_mttc0_ebase(cpu_env, t0);
                break;
            default:
                goto die;
                break;
            }
            break;
        case 23:
            switch (sel) {
            case 0:
                gen_helper_mttc0_debug(cpu_env, t0);
                break;
            default:
                gen_mtc0(ctx, t0, rd, sel);
                break;
            }
            break;
        default:
            gen_mtc0(ctx, t0, rd, sel);
        }
    } else {
        switch (sel) {
        /* GPR registers. */
        case 0:
            gen_helper_0e1i(mttgpr, t0, rd);
            break;
        /* Auxiliary CPU registers */
        case 1:
            switch (rd) {
            case 0:
                gen_helper_0e1i(mttlo, t0, 0);
                break;
            case 1:
                gen_helper_0e1i(mtthi, t0, 0);
                break;
            case 2:
                gen_helper_0e1i(mttacx, t0, 0);
                break;
            case 4:
                gen_helper_0e1i(mttlo, t0, 1);
                break;
            case 5:
                gen_helper_0e1i(mtthi, t0, 1);
                break;
            case 6:
                gen_helper_0e1i(mttacx, t0, 1);
                break;
            case 8:
                gen_helper_0e1i(mttlo, t0, 2);
                break;
            case 9:
                gen_helper_0e1i(mtthi, t0, 2);
                break;
            case 10:
                gen_helper_0e1i(mttacx, t0, 2);
                break;
            case 12:
                gen_helper_0e1i(mttlo, t0, 3);
                break;
            case 13:
                gen_helper_0e1i(mtthi, t0, 3);
                break;
            case 14:
                gen_helper_0e1i(mttacx, t0, 3);
                break;
            case 16:
                gen_helper_mttdsp(cpu_env, t0);
                break;
            default:
                goto die;
            }
            break;
        /* Floating point (COP1). */
        case 2:
            /* XXX: For now we support only a single FPU context. */
            if (h == 0) {
                TCGv_i32 fp0 = tcg_temp_new_i32();

                tcg_gen_trunc_tl_i32(fp0, t0);
                gen_store_fpr32(ctx, fp0, rd);
                tcg_temp_free_i32(fp0);
            } else {
                TCGv_i32 fp0 = tcg_temp_new_i32();

                tcg_gen_trunc_tl_i32(fp0, t0);
                gen_store_fpr32h(ctx, fp0, rd);
                tcg_temp_free_i32(fp0);
            }
            break;
        case 3:
            /* XXX: For now we support only a single FPU context. */
            {
                TCGv_i32 fs_tmp = tcg_const_i32(rd);

                gen_helper_0e2i(ctc1, t0, fs_tmp, rt);
                tcg_temp_free_i32(fs_tmp);
            }
            /* Stop translation as we may have changed hflags */
            ctx->base.is_jmp = DISAS_STOP;
            break;
        /* COP2: Not implemented. */
        case 4:
        case 5:
            /* fall through */
        default:
            goto die;
        }
    }
    trace_mips_translate_tr("mttr", rd, u, sel, h);
    tcg_temp_free(t0);
    return;

die:
    tcg_temp_free(t0);
    LOG_DISAS("mttr (reg %d u %d sel %d h %d)\n", rd, u, sel, h);
    gen_reserved_instruction(ctx);
}

static void gen_cp0(CPUMIPSState *env, DisasContext *ctx, uint32_t opc,
                    int rt, int rd)
{
    const char *opn = "ldst";

    check_cp0_enabled(ctx);
    switch (opc) {
    case OPC_MFC0:
        if (rt == 0) {
            /* Treat as NOP. */
            return;
        }
        gen_mfc0(ctx, cpu_gpr[rt], rd, ctx->opcode & 0x7);
        gen_log_instr_gpr_update(ctx, rt);
        opn = "mfc0";
        break;
    case OPC_MTC0:
        {
            TCGv t0 = tcg_temp_new();

            gen_load_gpr(t0, rt);
            gen_mtc0(ctx, t0, rd, ctx->opcode & 0x7);
            tcg_temp_free(t0);
        }
        opn = "mtc0";
        break;
#if defined(TARGET_MIPS64)
    case OPC_DMFC0:
        check_insn(ctx, ISA_MIPS3);
        if (rt == 0) {
            /* Treat as NOP. */
            return;
        }
        gen_dmfc0(ctx, cpu_gpr[rt], rd, ctx->opcode & 0x7);
        gen_log_instr_gpr_update(ctx, rt);
        opn = "dmfc0";
        break;
    case OPC_DMTC0:
        check_insn(ctx, ISA_MIPS3);
        {
            TCGv t0 = tcg_temp_new();

            gen_load_gpr(t0, rt);
            gen_dmtc0(ctx, t0, rd, ctx->opcode & 0x7);
            tcg_temp_free(t0);
        }
        opn = "dmtc0";
        break;
#endif
    case OPC_MFHC0:
        check_mvh(ctx);
        if (rt == 0) {
            /* Treat as NOP. */
            return;
        }
        gen_mfhc0(ctx, cpu_gpr[rt], rd, ctx->opcode & 0x7);
        gen_log_instr_gpr_update(ctx, rt);
        opn = "mfhc0";
        break;
    case OPC_MTHC0:
        check_mvh(ctx);
        {
            TCGv t0 = tcg_temp_new();
            gen_load_gpr(t0, rt);
            gen_mthc0(ctx, t0, rd, ctx->opcode & 0x7);
            tcg_temp_free(t0);
        }
        opn = "mthc0";
        break;
    case OPC_MFTR:
        check_cp0_enabled(ctx);
        if (rd == 0) {
            /* Treat as NOP. */
            return;
        }
        gen_mftr(env, ctx, rt, rd, (ctx->opcode >> 5) & 1,
                 ctx->opcode & 0x7, (ctx->opcode >> 4) & 1);
        opn = "mftr";
        break;
    case OPC_MTTR:
        check_cp0_enabled(ctx);
        gen_mttr(env, ctx, rd, rt, (ctx->opcode >> 5) & 1,
                 ctx->opcode & 0x7, (ctx->opcode >> 4) & 1);
        opn = "mttr";
        break;
    case OPC_TLBWI:
        opn = "tlbwi";
        if (!env->tlb->helper_tlbwi) {
            goto die;
        }
        gen_helper_tlbwi(cpu_env);
        break;
    case OPC_TLBINV:
        opn = "tlbinv";
        if (ctx->ie >= 2) {
            if (!env->tlb->helper_tlbinv) {
                goto die;
            }
            gen_helper_tlbinv(cpu_env);
        } /* treat as nop if TLBINV not supported */
        break;
    case OPC_TLBINVF:
        opn = "tlbinvf";
        if (ctx->ie >= 2) {
            if (!env->tlb->helper_tlbinvf) {
                goto die;
            }
            gen_helper_tlbinvf(cpu_env);
        } /* treat as nop if TLBINV not supported */
        break;
    case OPC_TLBWR:
        opn = "tlbwr";
        if (!env->tlb->helper_tlbwr) {
            goto die;
        }
        gen_helper_tlbwr(cpu_env);
        break;
    case OPC_TLBP:
        opn = "tlbp";
        if (!env->tlb->helper_tlbp) {
            goto die;
        }
        gen_helper_tlbp(cpu_env);
        break;
    case OPC_TLBR:
        opn = "tlbr";
        if (!env->tlb->helper_tlbr) {
            goto die;
        }
        gen_helper_tlbr(cpu_env);
        break;
    case OPC_ERET: /* OPC_ERETNC */
        if ((ctx->insn_flags & ISA_MIPS_R6) &&
            (ctx->hflags & MIPS_HFLAG_BMASK)) {
            goto die;
        } else {
            int bit_shift = (ctx->hflags & MIPS_HFLAG_M16) ? 16 : 6;
            gen_save_pc(ctx->base.pc_next);
            if (ctx->opcode & (1 << bit_shift)) {
                /* OPC_ERETNC */
                opn = "eretnc";
                check_insn(ctx, ISA_MIPS_R5);
                gen_helper_eretnc(cpu_env);
            } else {
                /* OPC_ERET */
                opn = "eret";
                check_insn(ctx, ISA_MIPS2);
                gen_helper_eret(cpu_env);
            }
            ctx->base.is_jmp = DISAS_EXIT;
        }
        break;
    case OPC_DERET:
        opn = "deret";
        check_insn(ctx, ISA_MIPS_R1);
        if ((ctx->insn_flags & ISA_MIPS_R6) &&
            (ctx->hflags & MIPS_HFLAG_BMASK)) {
            goto die;
        }
        if (!(ctx->hflags & MIPS_HFLAG_DM)) {
            MIPS_INVAL(opn);
            gen_reserved_instruction(ctx);
        } else {
            gen_helper_deret(cpu_env);
            ctx->base.is_jmp = DISAS_EXIT;
        }
        break;
    case OPC_WAIT:
        opn = "wait";
        check_insn(ctx, ISA_MIPS3 | ISA_MIPS_R1);
        if ((ctx->insn_flags & ISA_MIPS_R6) &&
            (ctx->hflags & MIPS_HFLAG_BMASK)) {
            goto die;
        }
        /* If we get an exception, we want to restart at next instruction */
        ctx->base.pc_next += 4;
        save_cpu_state(ctx, 1);
        ctx->base.pc_next -= 4;
        gen_helper_wait(cpu_env);
        ctx->base.is_jmp = DISAS_NORETURN;
        break;
    default:
 die:
        MIPS_INVAL(opn);
        gen_reserved_instruction(ctx);
        return;
    }
    (void)opn; /* avoid a compiler warning */
}
#endif /* !CONFIG_USER_ONLY */

/* CP1 Branches (before delay slot) */
static void gen_compute_branch1(DisasContext *ctx, uint32_t op,
                                int32_t cc, int32_t offset)
{
    target_ulong btarget;
    TCGv_i32 t0 = tcg_temp_new_i32();

    if (ctx->hflags & MIPS_HFLAG_BMASK) {
        gen_reserved_instruction(ctx);
        goto out;
    }

    if (cc != 0) {
        check_insn(ctx, ISA_MIPS4 | ISA_MIPS_R1);
    }

    btarget = ctx->base.pc_next + 4 + offset;

    switch (op) {
    case OPC_BC1F:
        tcg_gen_shri_i32(t0, fpu_fcr31, get_fp_bit(cc));
        tcg_gen_not_i32(t0, t0);
        tcg_gen_andi_i32(t0, t0, 1);
        tcg_gen_extu_i32_tl(bcond, t0);
        goto not_likely;
    case OPC_BC1FL:
        tcg_gen_shri_i32(t0, fpu_fcr31, get_fp_bit(cc));
        tcg_gen_not_i32(t0, t0);
        tcg_gen_andi_i32(t0, t0, 1);
        tcg_gen_extu_i32_tl(bcond, t0);
        goto likely;
    case OPC_BC1T:
        tcg_gen_shri_i32(t0, fpu_fcr31, get_fp_bit(cc));
        tcg_gen_andi_i32(t0, t0, 1);
        tcg_gen_extu_i32_tl(bcond, t0);
        goto not_likely;
    case OPC_BC1TL:
        tcg_gen_shri_i32(t0, fpu_fcr31, get_fp_bit(cc));
        tcg_gen_andi_i32(t0, t0, 1);
        tcg_gen_extu_i32_tl(bcond, t0);
    likely:
        ctx->hflags |= MIPS_HFLAG_BL;
        break;
    case OPC_BC1FANY2:
        {
            TCGv_i32 t1 = tcg_temp_new_i32();
            tcg_gen_shri_i32(t0, fpu_fcr31, get_fp_bit(cc));
            tcg_gen_shri_i32(t1, fpu_fcr31, get_fp_bit(cc + 1));
            tcg_gen_nand_i32(t0, t0, t1);
            tcg_temp_free_i32(t1);
            tcg_gen_andi_i32(t0, t0, 1);
            tcg_gen_extu_i32_tl(bcond, t0);
        }
        goto not_likely;
    case OPC_BC1TANY2:
        {
            TCGv_i32 t1 = tcg_temp_new_i32();
            tcg_gen_shri_i32(t0, fpu_fcr31, get_fp_bit(cc));
            tcg_gen_shri_i32(t1, fpu_fcr31, get_fp_bit(cc + 1));
            tcg_gen_or_i32(t0, t0, t1);
            tcg_temp_free_i32(t1);
            tcg_gen_andi_i32(t0, t0, 1);
            tcg_gen_extu_i32_tl(bcond, t0);
        }
        goto not_likely;
    case OPC_BC1FANY4:
        {
            TCGv_i32 t1 = tcg_temp_new_i32();
            tcg_gen_shri_i32(t0, fpu_fcr31, get_fp_bit(cc));
            tcg_gen_shri_i32(t1, fpu_fcr31, get_fp_bit(cc + 1));
            tcg_gen_and_i32(t0, t0, t1);
            tcg_gen_shri_i32(t1, fpu_fcr31, get_fp_bit(cc + 2));
            tcg_gen_and_i32(t0, t0, t1);
            tcg_gen_shri_i32(t1, fpu_fcr31, get_fp_bit(cc + 3));
            tcg_gen_nand_i32(t0, t0, t1);
            tcg_temp_free_i32(t1);
            tcg_gen_andi_i32(t0, t0, 1);
            tcg_gen_extu_i32_tl(bcond, t0);
        }
        goto not_likely;
    case OPC_BC1TANY4:
        {
            TCGv_i32 t1 = tcg_temp_new_i32();
            tcg_gen_shri_i32(t0, fpu_fcr31, get_fp_bit(cc));
            tcg_gen_shri_i32(t1, fpu_fcr31, get_fp_bit(cc + 1));
            tcg_gen_or_i32(t0, t0, t1);
            tcg_gen_shri_i32(t1, fpu_fcr31, get_fp_bit(cc + 2));
            tcg_gen_or_i32(t0, t0, t1);
            tcg_gen_shri_i32(t1, fpu_fcr31, get_fp_bit(cc + 3));
            tcg_gen_or_i32(t0, t0, t1);
            tcg_temp_free_i32(t1);
            tcg_gen_andi_i32(t0, t0, 1);
            tcg_gen_extu_i32_tl(bcond, t0);
        }
    not_likely:
        ctx->hflags |= MIPS_HFLAG_BC;
        break;
    default:
        MIPS_INVAL("cp1 cond branch");
        gen_reserved_instruction(ctx);
        goto out;
    }
    // Check that the conditional branch target is in range (but only if the
    // branch is taken).
    gen_check_cond_branch_target(ctx, bcond, btarget);

    ctx->btarget = btarget;
    ctx->hflags |= MIPS_HFLAG_BDS32;
 out:
    tcg_temp_free_i32(t0);
}

/* R6 CP1 Branches */
static void gen_compute_branch1_r6(DisasContext *ctx, uint32_t op,
                                   int32_t ft, int32_t offset,
                                   int delayslot_size)
{
    target_ulong btarget;
    TCGv_i64 t0 = tcg_temp_new_i64();

    if (ctx->hflags & MIPS_HFLAG_BMASK) {
#ifdef MIPS_DEBUG_DISAS
        LOG_DISAS("Branch in delay / forbidden slot at PC 0x" TARGET_FMT_lx
                  "\n", ctx->base.pc_next);
#endif
        gen_reserved_instruction(ctx);
        goto out;
    }

    gen_load_fpr64(ctx, t0, ft);
    tcg_gen_andi_i64(t0, t0, 1);

    btarget = addr_add(ctx, ctx->base.pc_next + 4, offset);

    switch (op) {
    case OPC_BC1EQZ:
        tcg_gen_xori_i64(t0, t0, 1);
        ctx->hflags |= MIPS_HFLAG_BC;
        break;
    case OPC_BC1NEZ:
        /* t0 already set */
        ctx->hflags |= MIPS_HFLAG_BC;
        break;
    default:
        MIPS_INVAL("cp1 cond branch");
        gen_reserved_instruction(ctx);
        goto out;
    }

    tcg_gen_trunc_i64_tl(bcond, t0);

    // Check that the conditional branch target is in range (but only if the
    // branch is taken)
    gen_check_cond_branch_target(ctx, bcond, btarget);
    ctx->btarget = btarget;

    switch (delayslot_size) {
    case 2:
        ctx->hflags |= MIPS_HFLAG_BDS16;
        break;
    case 4:
        ctx->hflags |= MIPS_HFLAG_BDS32;
        break;
    }

out:
    tcg_temp_free_i64(t0);
}

/* Coprocessor 1 (FPU) */

#define FOP(func, fmt) (((fmt) << 21) | (func))

enum fopcode {
    OPC_ADD_S = FOP(0, FMT_S),
    OPC_SUB_S = FOP(1, FMT_S),
    OPC_MUL_S = FOP(2, FMT_S),
    OPC_DIV_S = FOP(3, FMT_S),
    OPC_SQRT_S = FOP(4, FMT_S),
    OPC_ABS_S = FOP(5, FMT_S),
    OPC_MOV_S = FOP(6, FMT_S),
    OPC_NEG_S = FOP(7, FMT_S),
    OPC_ROUND_L_S = FOP(8, FMT_S),
    OPC_TRUNC_L_S = FOP(9, FMT_S),
    OPC_CEIL_L_S = FOP(10, FMT_S),
    OPC_FLOOR_L_S = FOP(11, FMT_S),
    OPC_ROUND_W_S = FOP(12, FMT_S),
    OPC_TRUNC_W_S = FOP(13, FMT_S),
    OPC_CEIL_W_S = FOP(14, FMT_S),
    OPC_FLOOR_W_S = FOP(15, FMT_S),
    OPC_SEL_S = FOP(16, FMT_S),
    OPC_MOVCF_S = FOP(17, FMT_S),
    OPC_MOVZ_S = FOP(18, FMT_S),
    OPC_MOVN_S = FOP(19, FMT_S),
    OPC_SELEQZ_S = FOP(20, FMT_S),
    OPC_RECIP_S = FOP(21, FMT_S),
    OPC_RSQRT_S = FOP(22, FMT_S),
    OPC_SELNEZ_S = FOP(23, FMT_S),
    OPC_MADDF_S = FOP(24, FMT_S),
    OPC_MSUBF_S = FOP(25, FMT_S),
    OPC_RINT_S = FOP(26, FMT_S),
    OPC_CLASS_S = FOP(27, FMT_S),
    OPC_MIN_S = FOP(28, FMT_S),
    OPC_RECIP2_S = FOP(28, FMT_S),
    OPC_MINA_S = FOP(29, FMT_S),
    OPC_RECIP1_S = FOP(29, FMT_S),
    OPC_MAX_S = FOP(30, FMT_S),
    OPC_RSQRT1_S = FOP(30, FMT_S),
    OPC_MAXA_S = FOP(31, FMT_S),
    OPC_RSQRT2_S = FOP(31, FMT_S),
    OPC_CVT_D_S = FOP(33, FMT_S),
    OPC_CVT_W_S = FOP(36, FMT_S),
    OPC_CVT_L_S = FOP(37, FMT_S),
    OPC_CVT_PS_S = FOP(38, FMT_S),
    OPC_CMP_F_S = FOP(48, FMT_S),
    OPC_CMP_UN_S = FOP(49, FMT_S),
    OPC_CMP_EQ_S = FOP(50, FMT_S),
    OPC_CMP_UEQ_S = FOP(51, FMT_S),
    OPC_CMP_OLT_S = FOP(52, FMT_S),
    OPC_CMP_ULT_S = FOP(53, FMT_S),
    OPC_CMP_OLE_S = FOP(54, FMT_S),
    OPC_CMP_ULE_S = FOP(55, FMT_S),
    OPC_CMP_SF_S = FOP(56, FMT_S),
    OPC_CMP_NGLE_S = FOP(57, FMT_S),
    OPC_CMP_SEQ_S = FOP(58, FMT_S),
    OPC_CMP_NGL_S = FOP(59, FMT_S),
    OPC_CMP_LT_S = FOP(60, FMT_S),
    OPC_CMP_NGE_S = FOP(61, FMT_S),
    OPC_CMP_LE_S = FOP(62, FMT_S),
    OPC_CMP_NGT_S = FOP(63, FMT_S),

    OPC_ADD_D = FOP(0, FMT_D),
    OPC_SUB_D = FOP(1, FMT_D),
    OPC_MUL_D = FOP(2, FMT_D),
    OPC_DIV_D = FOP(3, FMT_D),
    OPC_SQRT_D = FOP(4, FMT_D),
    OPC_ABS_D = FOP(5, FMT_D),
    OPC_MOV_D = FOP(6, FMT_D),
    OPC_NEG_D = FOP(7, FMT_D),
    OPC_ROUND_L_D = FOP(8, FMT_D),
    OPC_TRUNC_L_D = FOP(9, FMT_D),
    OPC_CEIL_L_D = FOP(10, FMT_D),
    OPC_FLOOR_L_D = FOP(11, FMT_D),
    OPC_ROUND_W_D = FOP(12, FMT_D),
    OPC_TRUNC_W_D = FOP(13, FMT_D),
    OPC_CEIL_W_D = FOP(14, FMT_D),
    OPC_FLOOR_W_D = FOP(15, FMT_D),
    OPC_SEL_D = FOP(16, FMT_D),
    OPC_MOVCF_D = FOP(17, FMT_D),
    OPC_MOVZ_D = FOP(18, FMT_D),
    OPC_MOVN_D = FOP(19, FMT_D),
    OPC_SELEQZ_D = FOP(20, FMT_D),
    OPC_RECIP_D = FOP(21, FMT_D),
    OPC_RSQRT_D = FOP(22, FMT_D),
    OPC_SELNEZ_D = FOP(23, FMT_D),
    OPC_MADDF_D = FOP(24, FMT_D),
    OPC_MSUBF_D = FOP(25, FMT_D),
    OPC_RINT_D = FOP(26, FMT_D),
    OPC_CLASS_D = FOP(27, FMT_D),
    OPC_MIN_D = FOP(28, FMT_D),
    OPC_RECIP2_D = FOP(28, FMT_D),
    OPC_MINA_D = FOP(29, FMT_D),
    OPC_RECIP1_D = FOP(29, FMT_D),
    OPC_MAX_D = FOP(30, FMT_D),
    OPC_RSQRT1_D = FOP(30, FMT_D),
    OPC_MAXA_D = FOP(31, FMT_D),
    OPC_RSQRT2_D = FOP(31, FMT_D),
    OPC_CVT_S_D = FOP(32, FMT_D),
    OPC_CVT_W_D = FOP(36, FMT_D),
    OPC_CVT_L_D = FOP(37, FMT_D),
    OPC_CMP_F_D = FOP(48, FMT_D),
    OPC_CMP_UN_D = FOP(49, FMT_D),
    OPC_CMP_EQ_D = FOP(50, FMT_D),
    OPC_CMP_UEQ_D = FOP(51, FMT_D),
    OPC_CMP_OLT_D = FOP(52, FMT_D),
    OPC_CMP_ULT_D = FOP(53, FMT_D),
    OPC_CMP_OLE_D = FOP(54, FMT_D),
    OPC_CMP_ULE_D = FOP(55, FMT_D),
    OPC_CMP_SF_D = FOP(56, FMT_D),
    OPC_CMP_NGLE_D = FOP(57, FMT_D),
    OPC_CMP_SEQ_D = FOP(58, FMT_D),
    OPC_CMP_NGL_D = FOP(59, FMT_D),
    OPC_CMP_LT_D = FOP(60, FMT_D),
    OPC_CMP_NGE_D = FOP(61, FMT_D),
    OPC_CMP_LE_D = FOP(62, FMT_D),
    OPC_CMP_NGT_D = FOP(63, FMT_D),

    OPC_CVT_S_W = FOP(32, FMT_W),
    OPC_CVT_D_W = FOP(33, FMT_W),
    OPC_CVT_S_L = FOP(32, FMT_L),
    OPC_CVT_D_L = FOP(33, FMT_L),
    OPC_CVT_PS_PW = FOP(38, FMT_W),

    OPC_ADD_PS = FOP(0, FMT_PS),
    OPC_SUB_PS = FOP(1, FMT_PS),
    OPC_MUL_PS = FOP(2, FMT_PS),
    OPC_DIV_PS = FOP(3, FMT_PS),
    OPC_ABS_PS = FOP(5, FMT_PS),
    OPC_MOV_PS = FOP(6, FMT_PS),
    OPC_NEG_PS = FOP(7, FMT_PS),
    OPC_MOVCF_PS = FOP(17, FMT_PS),
    OPC_MOVZ_PS = FOP(18, FMT_PS),
    OPC_MOVN_PS = FOP(19, FMT_PS),
    OPC_ADDR_PS = FOP(24, FMT_PS),
    OPC_MULR_PS = FOP(26, FMT_PS),
    OPC_RECIP2_PS = FOP(28, FMT_PS),
    OPC_RECIP1_PS = FOP(29, FMT_PS),
    OPC_RSQRT1_PS = FOP(30, FMT_PS),
    OPC_RSQRT2_PS = FOP(31, FMT_PS),

    OPC_CVT_S_PU = FOP(32, FMT_PS),
    OPC_CVT_PW_PS = FOP(36, FMT_PS),
    OPC_CVT_S_PL = FOP(40, FMT_PS),
    OPC_PLL_PS = FOP(44, FMT_PS),
    OPC_PLU_PS = FOP(45, FMT_PS),
    OPC_PUL_PS = FOP(46, FMT_PS),
    OPC_PUU_PS = FOP(47, FMT_PS),
    OPC_CMP_F_PS = FOP(48, FMT_PS),
    OPC_CMP_UN_PS = FOP(49, FMT_PS),
    OPC_CMP_EQ_PS = FOP(50, FMT_PS),
    OPC_CMP_UEQ_PS = FOP(51, FMT_PS),
    OPC_CMP_OLT_PS = FOP(52, FMT_PS),
    OPC_CMP_ULT_PS = FOP(53, FMT_PS),
    OPC_CMP_OLE_PS = FOP(54, FMT_PS),
    OPC_CMP_ULE_PS = FOP(55, FMT_PS),
    OPC_CMP_SF_PS = FOP(56, FMT_PS),
    OPC_CMP_NGLE_PS = FOP(57, FMT_PS),
    OPC_CMP_SEQ_PS = FOP(58, FMT_PS),
    OPC_CMP_NGL_PS = FOP(59, FMT_PS),
    OPC_CMP_LT_PS = FOP(60, FMT_PS),
    OPC_CMP_NGE_PS = FOP(61, FMT_PS),
    OPC_CMP_LE_PS = FOP(62, FMT_PS),
    OPC_CMP_NGT_PS = FOP(63, FMT_PS),
};

enum r6_f_cmp_op {
    R6_OPC_CMP_AF_S   = FOP(0, FMT_W),
    R6_OPC_CMP_UN_S   = FOP(1, FMT_W),
    R6_OPC_CMP_EQ_S   = FOP(2, FMT_W),
    R6_OPC_CMP_UEQ_S  = FOP(3, FMT_W),
    R6_OPC_CMP_LT_S   = FOP(4, FMT_W),
    R6_OPC_CMP_ULT_S  = FOP(5, FMT_W),
    R6_OPC_CMP_LE_S   = FOP(6, FMT_W),
    R6_OPC_CMP_ULE_S  = FOP(7, FMT_W),
    R6_OPC_CMP_SAF_S  = FOP(8, FMT_W),
    R6_OPC_CMP_SUN_S  = FOP(9, FMT_W),
    R6_OPC_CMP_SEQ_S  = FOP(10, FMT_W),
    R6_OPC_CMP_SEUQ_S = FOP(11, FMT_W),
    R6_OPC_CMP_SLT_S  = FOP(12, FMT_W),
    R6_OPC_CMP_SULT_S = FOP(13, FMT_W),
    R6_OPC_CMP_SLE_S  = FOP(14, FMT_W),
    R6_OPC_CMP_SULE_S = FOP(15, FMT_W),
    R6_OPC_CMP_OR_S   = FOP(17, FMT_W),
    R6_OPC_CMP_UNE_S  = FOP(18, FMT_W),
    R6_OPC_CMP_NE_S   = FOP(19, FMT_W),
    R6_OPC_CMP_SOR_S  = FOP(25, FMT_W),
    R6_OPC_CMP_SUNE_S = FOP(26, FMT_W),
    R6_OPC_CMP_SNE_S  = FOP(27, FMT_W),

    R6_OPC_CMP_AF_D   = FOP(0, FMT_L),
    R6_OPC_CMP_UN_D   = FOP(1, FMT_L),
    R6_OPC_CMP_EQ_D   = FOP(2, FMT_L),
    R6_OPC_CMP_UEQ_D  = FOP(3, FMT_L),
    R6_OPC_CMP_LT_D   = FOP(4, FMT_L),
    R6_OPC_CMP_ULT_D  = FOP(5, FMT_L),
    R6_OPC_CMP_LE_D   = FOP(6, FMT_L),
    R6_OPC_CMP_ULE_D  = FOP(7, FMT_L),
    R6_OPC_CMP_SAF_D  = FOP(8, FMT_L),
    R6_OPC_CMP_SUN_D  = FOP(9, FMT_L),
    R6_OPC_CMP_SEQ_D  = FOP(10, FMT_L),
    R6_OPC_CMP_SEUQ_D = FOP(11, FMT_L),
    R6_OPC_CMP_SLT_D  = FOP(12, FMT_L),
    R6_OPC_CMP_SULT_D = FOP(13, FMT_L),
    R6_OPC_CMP_SLE_D  = FOP(14, FMT_L),
    R6_OPC_CMP_SULE_D = FOP(15, FMT_L),
    R6_OPC_CMP_OR_D   = FOP(17, FMT_L),
    R6_OPC_CMP_UNE_D  = FOP(18, FMT_L),
    R6_OPC_CMP_NE_D   = FOP(19, FMT_L),
    R6_OPC_CMP_SOR_D  = FOP(25, FMT_L),
    R6_OPC_CMP_SUNE_D = FOP(26, FMT_L),
    R6_OPC_CMP_SNE_D  = FOP(27, FMT_L),
};

static void gen_cp1(DisasContext *ctx, uint32_t opc, int rt, int fs)
{
    TCGv t0 = tcg_temp_new();

    switch (opc) {
    case OPC_MFC1:
        {
            TCGv_i32 fp0 = tcg_temp_new_i32();

            gen_load_fpr32(ctx, fp0, fs);
            tcg_gen_ext_i32_tl(t0, fp0);
            tcg_temp_free_i32(fp0);
        }
        gen_store_gpr(t0, rt);
        break;
    case OPC_MTC1:
        gen_load_gpr(t0, rt);
        {
            TCGv_i32 fp0 = tcg_temp_new_i32();

            tcg_gen_trunc_tl_i32(fp0, t0);
            gen_store_fpr32(ctx, fp0, fs);
            tcg_temp_free_i32(fp0);
        }
        break;
    case OPC_CFC1:
        gen_helper_1e0i(cfc1, t0, fs);
        gen_store_gpr(t0, rt);
        break;
    case OPC_CTC1:
        gen_load_gpr(t0, rt);
        save_cpu_state(ctx, 0);
        {
            TCGv_i32 fs_tmp = tcg_const_i32(fs);

            gen_helper_0e2i(ctc1, t0, fs_tmp, rt);
            tcg_temp_free_i32(fs_tmp);
        }
        /* Stop translation as we may have changed hflags */
        ctx->base.is_jmp = DISAS_STOP;
        break;
#if defined(TARGET_MIPS64)
    case OPC_DMFC1:
        gen_load_fpr64(ctx, t0, fs);
        gen_store_gpr(t0, rt);
        break;
    case OPC_DMTC1:
        gen_load_gpr(t0, rt);
        gen_store_fpr64(ctx, t0, fs);
        break;
#endif
    case OPC_MFHC1:
        {
            TCGv_i32 fp0 = tcg_temp_new_i32();

            gen_load_fpr32h(ctx, fp0, fs);
            tcg_gen_ext_i32_tl(t0, fp0);
            tcg_temp_free_i32(fp0);
        }
        gen_store_gpr(t0, rt);
        break;
    case OPC_MTHC1:
        gen_load_gpr(t0, rt);
        {
            TCGv_i32 fp0 = tcg_temp_new_i32();

            tcg_gen_trunc_tl_i32(fp0, t0);
            gen_store_fpr32h(ctx, fp0, fs);
            tcg_temp_free_i32(fp0);
        }
        break;
    default:
        MIPS_INVAL("cp1 move");
        gen_reserved_instruction(ctx);
        goto out;
    }

 out:
    tcg_temp_free(t0);
}

static void gen_movci(DisasContext *ctx, int rd, int rs, int cc, int tf)
{
    TCGLabel *l1;
    TCGCond cond;
    TCGv_i32 t0;

    if (rd == 0) {
        /* Treat as NOP. */
        return;
    }

    if (tf) {
        cond = TCG_COND_EQ;
    } else {
        cond = TCG_COND_NE;
    }

    l1 = gen_new_label();
    t0 = tcg_temp_new_i32();
    tcg_gen_andi_i32(t0, fpu_fcr31, 1 << get_fp_bit(cc));
    tcg_gen_brcondi_i32(cond, t0, 0, l1);
    tcg_temp_free_i32(t0);
    gen_load_gpr(cpu_gpr[rd], rs);
    gen_log_instr_gpr_update(ctx, rd);
    gen_set_label(l1);
}

static inline void gen_movcf_s(DisasContext *ctx, int fs, int fd, int cc,
                               int tf)
{
    int cond;
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGLabel *l1 = gen_new_label();

    if (tf) {
        cond = TCG_COND_EQ;
    } else {
        cond = TCG_COND_NE;
    }

    tcg_gen_andi_i32(t0, fpu_fcr31, 1 << get_fp_bit(cc));
    tcg_gen_brcondi_i32(cond, t0, 0, l1);
    gen_load_fpr32(ctx, t0, fs);
    gen_store_fpr32(ctx, t0, fd);
    gen_set_label(l1);
    tcg_temp_free_i32(t0);
}

static inline void gen_movcf_d(DisasContext *ctx, int fs, int fd, int cc,
                               int tf)
{
    int cond;
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGv_i64 fp0;
    TCGLabel *l1 = gen_new_label();

    if (tf) {
        cond = TCG_COND_EQ;
    } else {
        cond = TCG_COND_NE;
    }

    tcg_gen_andi_i32(t0, fpu_fcr31, 1 << get_fp_bit(cc));
    tcg_gen_brcondi_i32(cond, t0, 0, l1);
    tcg_temp_free_i32(t0);
    fp0 = tcg_temp_new_i64();
    gen_load_fpr64(ctx, fp0, fs);
    gen_store_fpr64(ctx, fp0, fd);
    tcg_temp_free_i64(fp0);
    gen_set_label(l1);
}

static inline void gen_movcf_ps(DisasContext *ctx, int fs, int fd,
                                int cc, int tf)
{
    int cond;
    TCGv_i32 t0 = tcg_temp_new_i32();
    TCGLabel *l1 = gen_new_label();
    TCGLabel *l2 = gen_new_label();

    if (tf) {
        cond = TCG_COND_EQ;
    } else {
        cond = TCG_COND_NE;
    }

    tcg_gen_andi_i32(t0, fpu_fcr31, 1 << get_fp_bit(cc));
    tcg_gen_brcondi_i32(cond, t0, 0, l1);
    gen_load_fpr32(ctx, t0, fs);
    gen_store_fpr32(ctx, t0, fd);
    gen_set_label(l1);

    tcg_gen_andi_i32(t0, fpu_fcr31, 1 << get_fp_bit(cc + 1));
    tcg_gen_brcondi_i32(cond, t0, 0, l2);
    gen_load_fpr32h(ctx, t0, fs);
    gen_store_fpr32h(ctx, t0, fd);
    tcg_temp_free_i32(t0);
    gen_set_label(l2);
}

static void gen_sel_s(DisasContext *ctx, enum fopcode op1, int fd, int ft,
                      int fs)
{
    TCGv_i32 t1 = tcg_const_i32(0);
    TCGv_i32 fp0 = tcg_temp_new_i32();
    TCGv_i32 fp1 = tcg_temp_new_i32();
    TCGv_i32 fp2 = tcg_temp_new_i32();
    gen_load_fpr32(ctx, fp0, fd);
    gen_load_fpr32(ctx, fp1, ft);
    gen_load_fpr32(ctx, fp2, fs);

    switch (op1) {
    case OPC_SEL_S:
        tcg_gen_andi_i32(fp0, fp0, 1);
        tcg_gen_movcond_i32(TCG_COND_NE, fp0, fp0, t1, fp1, fp2);
        break;
    case OPC_SELEQZ_S:
        tcg_gen_andi_i32(fp1, fp1, 1);
        tcg_gen_movcond_i32(TCG_COND_EQ, fp0, fp1, t1, fp2, t1);
        break;
    case OPC_SELNEZ_S:
        tcg_gen_andi_i32(fp1, fp1, 1);
        tcg_gen_movcond_i32(TCG_COND_NE, fp0, fp1, t1, fp2, t1);
        break;
    default:
        MIPS_INVAL("gen_sel_s");
        gen_reserved_instruction(ctx);
        break;
    }

    gen_store_fpr32(ctx, fp0, fd);
    tcg_temp_free_i32(fp2);
    tcg_temp_free_i32(fp1);
    tcg_temp_free_i32(fp0);
    tcg_temp_free_i32(t1);
}

static void gen_sel_d(DisasContext *ctx, enum fopcode op1, int fd, int ft,
                      int fs)
{
    TCGv_i64 t1 = tcg_const_i64(0);
    TCGv_i64 fp0 = tcg_temp_new_i64();
    TCGv_i64 fp1 = tcg_temp_new_i64();
    TCGv_i64 fp2 = tcg_temp_new_i64();
    gen_load_fpr64(ctx, fp0, fd);
    gen_load_fpr64(ctx, fp1, ft);
    gen_load_fpr64(ctx, fp2, fs);

    switch (op1) {
    case OPC_SEL_D:
        tcg_gen_andi_i64(fp0, fp0, 1);
        tcg_gen_movcond_i64(TCG_COND_NE, fp0, fp0, t1, fp1, fp2);
        break;
    case OPC_SELEQZ_D:
        tcg_gen_andi_i64(fp1, fp1, 1);
        tcg_gen_movcond_i64(TCG_COND_EQ, fp0, fp1, t1, fp2, t1);
        break;
    case OPC_SELNEZ_D:
        tcg_gen_andi_i64(fp1, fp1, 1);
        tcg_gen_movcond_i64(TCG_COND_NE, fp0, fp1, t1, fp2, t1);
        break;
    default:
        MIPS_INVAL("gen_sel_d");
        gen_reserved_instruction(ctx);
        break;
    }

    gen_store_fpr64(ctx, fp0, fd);
    tcg_temp_free_i64(fp2);
    tcg_temp_free_i64(fp1);
    tcg_temp_free_i64(fp0);
    tcg_temp_free_i64(t1);
}

static void gen_farith(DisasContext *ctx, enum fopcode op1,
                       int ft, int fs, int fd, int cc)
{
    uint32_t func = ctx->opcode & 0x3f;
    switch (op1) {
    case OPC_ADD_S:
        {
            TCGv_i32 fp0 = tcg_temp_new_i32();
            TCGv_i32 fp1 = tcg_temp_new_i32();

            gen_load_fpr32(ctx, fp0, fs);
            gen_load_fpr32(ctx, fp1, ft);
            gen_helper_float_add_s(fp0, cpu_env, fp0, fp1);
            tcg_temp_free_i32(fp1);
            gen_store_fpr32(ctx, fp0, fd);
            tcg_temp_free_i32(fp0);
        }
        break;
    case OPC_SUB_S:
        {
            TCGv_i32 fp0 = tcg_temp_new_i32();
            TCGv_i32 fp1 = tcg_temp_new_i32();

            gen_load_fpr32(ctx, fp0, fs);
            gen_load_fpr32(ctx, fp1, ft);
            gen_helper_float_sub_s(fp0, cpu_env, fp0, fp1);
            tcg_temp_free_i32(fp1);
            gen_store_fpr32(ctx, fp0, fd);
            tcg_temp_free_i32(fp0);
        }
        break;
    case OPC_MUL_S:
        {
            TCGv_i32 fp0 = tcg_temp_new_i32();
            TCGv_i32 fp1 = tcg_temp_new_i32();

            gen_load_fpr32(ctx, fp0, fs);
            gen_load_fpr32(ctx, fp1, ft);
            gen_helper_float_mul_s(fp0, cpu_env, fp0, fp1);
            tcg_temp_free_i32(fp1);
            gen_store_fpr32(ctx, fp0, fd);
            tcg_temp_free_i32(fp0);
        }
        break;
    case OPC_DIV_S:
        {
            TCGv_i32 fp0 = tcg_temp_new_i32();
            TCGv_i32 fp1 = tcg_temp_new_i32();

            gen_load_fpr32(ctx, fp0, fs);
            gen_load_fpr32(ctx, fp1, ft);
            gen_helper_float_div_s(fp0, cpu_env, fp0, fp1);
            tcg_temp_free_i32(fp1);
            gen_store_fpr32(ctx, fp0, fd);
            tcg_temp_free_i32(fp0);
        }
        break;
    case OPC_SQRT_S:
        {
            TCGv_i32 fp0 = tcg_temp_new_i32();

            gen_load_fpr32(ctx, fp0, fs);
            gen_helper_float_sqrt_s(fp0, cpu_env, fp0);
            gen_store_fpr32(ctx, fp0, fd);
            tcg_temp_free_i32(fp0);
        }
        break;
    case OPC_ABS_S:
        {
            TCGv_i32 fp0 = tcg_temp_new_i32();

            gen_load_fpr32(ctx, fp0, fs);
            if (ctx->abs2008) {
                tcg_gen_andi_i32(fp0, fp0, 0x7fffffffUL);
            } else {
                gen_helper_float_abs_s(fp0, fp0);
            }
            gen_store_fpr32(ctx, fp0, fd);
            tcg_temp_free_i32(fp0);
        }
        break;
    case OPC_MOV_S:
        {
            TCGv_i32 fp0 = tcg_temp_new_i32();

            gen_load_fpr32(ctx, fp0, fs);
            gen_store_fpr32(ctx, fp0, fd);
            tcg_temp_free_i32(fp0);
        }
        break;
    case OPC_NEG_S:
        {
            TCGv_i32 fp0 = tcg_temp_new_i32();

            gen_load_fpr32(ctx, fp0, fs);
            if (ctx->abs2008) {
                tcg_gen_xori_i32(fp0, fp0, 1UL << 31);
            } else {
                gen_helper_float_chs_s(fp0, fp0);
            }
            gen_store_fpr32(ctx, fp0, fd);
            tcg_temp_free_i32(fp0);
        }
        break;
    case OPC_ROUND_L_S:
        check_cp1_64bitmode(ctx);
        {
            TCGv_i32 fp32 = tcg_temp_new_i32();
            TCGv_i64 fp64 = tcg_temp_new_i64();

            gen_load_fpr32(ctx, fp32, fs);
            if (ctx->nan2008) {
                gen_helper_float_round_2008_l_s(fp64, cpu_env, fp32);
            } else {
                gen_helper_float_round_l_s(fp64, cpu_env, fp32);
            }
            tcg_temp_free_i32(fp32);
            gen_store_fpr64(ctx, fp64, fd);
            tcg_temp_free_i64(fp64);
        }
        break;
    case OPC_TRUNC_L_S:
        check_cp1_64bitmode(ctx);
        {
            TCGv_i32 fp32 = tcg_temp_new_i32();
            TCGv_i64 fp64 = tcg_temp_new_i64();

            gen_load_fpr32(ctx, fp32, fs);
            if (ctx->nan2008) {
                gen_helper_float_trunc_2008_l_s(fp64, cpu_env, fp32);
            } else {
                gen_helper_float_trunc_l_s(fp64, cpu_env, fp32);
            }
            tcg_temp_free_i32(fp32);
            gen_store_fpr64(ctx, fp64, fd);
            tcg_temp_free_i64(fp64);
        }
        break;
    case OPC_CEIL_L_S:
        check_cp1_64bitmode(ctx);
        {
            TCGv_i32 fp32 = tcg_temp_new_i32();
            TCGv_i64 fp64 = tcg_temp_new_i64();

            gen_load_fpr32(ctx, fp32, fs);
            if (ctx->nan2008) {
                gen_helper_float_ceil_2008_l_s(fp64, cpu_env, fp32);
            } else {
                gen_helper_float_ceil_l_s(fp64, cpu_env, fp32);
            }
            tcg_temp_free_i32(fp32);
            gen_store_fpr64(ctx, fp64, fd);
            tcg_temp_free_i64(fp64);
        }
        break;
    case OPC_FLOOR_L_S:
        check_cp1_64bitmode(ctx);
        {
            TCGv_i32 fp32 = tcg_temp_new_i32();
            TCGv_i64 fp64 = tcg_temp_new_i64();

            gen_load_fpr32(ctx, fp32, fs);
            if (ctx->nan2008) {
                gen_helper_float_floor_2008_l_s(fp64, cpu_env, fp32);
            } else {
                gen_helper_float_floor_l_s(fp64, cpu_env, fp32);
            }
            tcg_temp_free_i32(fp32);
            gen_store_fpr64(ctx, fp64, fd);
            tcg_temp_free_i64(fp64);
        }
        break;
    case OPC_ROUND_W_S:
        {
            TCGv_i32 fp0 = tcg_temp_new_i32();

            gen_load_fpr32(ctx, fp0, fs);
            if (ctx->nan2008) {
                gen_helper_float_round_2008_w_s(fp0, cpu_env, fp0);
            } else {
                gen_helper_float_round_w_s(fp0, cpu_env, fp0);
            }
            gen_store_fpr32(ctx, fp0, fd);
            tcg_temp_free_i32(fp0);
        }
        break;
    case OPC_TRUNC_W_S:
        {
            TCGv_i32 fp0 = tcg_temp_new_i32();

            gen_load_fpr32(ctx, fp0, fs);
            if (ctx->nan2008) {
                gen_helper_float_trunc_2008_w_s(fp0, cpu_env, fp0);
            } else {
                gen_helper_float_trunc_w_s(fp0, cpu_env, fp0);
            }
            gen_store_fpr32(ctx, fp0, fd);
            tcg_temp_free_i32(fp0);
        }
        break;
    case OPC_CEIL_W_S:
        {
            TCGv_i32 fp0 = tcg_temp_new_i32();

            gen_load_fpr32(ctx, fp0, fs);
            if (ctx->nan2008) {
                gen_helper_float_ceil_2008_w_s(fp0, cpu_env, fp0);
            } else {
                gen_helper_float_ceil_w_s(fp0, cpu_env, fp0);
            }
            gen_store_fpr32(ctx, fp0, fd);
            tcg_temp_free_i32(fp0);
        }
        break;
    case OPC_FLOOR_W_S:
        {
            TCGv_i32 fp0 = tcg_temp_new_i32();

            gen_load_fpr32(ctx, fp0, fs);
            if (ctx->nan2008) {
                gen_helper_float_floor_2008_w_s(fp0, cpu_env, fp0);
            } else {
                gen_helper_float_floor_w_s(fp0, cpu_env, fp0);
            }
            gen_store_fpr32(ctx, fp0, fd);
            tcg_temp_free_i32(fp0);
        }
        break;
    case OPC_SEL_S:
        check_insn(ctx, ISA_MIPS_R6);
        gen_sel_s(ctx, op1, fd, ft, fs);
        break;
    case OPC_SELEQZ_S:
        check_insn(ctx, ISA_MIPS_R6);
        gen_sel_s(ctx, op1, fd, ft, fs);
        break;
    case OPC_SELNEZ_S:
        check_insn(ctx, ISA_MIPS_R6);
        gen_sel_s(ctx, op1, fd, ft, fs);
        break;
    case OPC_MOVCF_S:
        check_insn_opc_removed(ctx, ISA_MIPS_R6);
        gen_movcf_s(ctx, fs, fd, (ft >> 2) & 0x7, ft & 0x1);
        break;
    case OPC_MOVZ_S:
        check_insn_opc_removed(ctx, ISA_MIPS_R6);
        {
            TCGLabel *l1 = gen_new_label();
            TCGv_i32 fp0;

            if (ft != 0) {
                tcg_gen_brcondi_tl(TCG_COND_NE, cpu_gpr[ft], 0, l1);
                gen_log_instr_gpr_update(ctx, ft);
            }
            fp0 = tcg_temp_new_i32();
            gen_load_fpr32(ctx, fp0, fs);
            gen_store_fpr32(ctx, fp0, fd);
            tcg_temp_free_i32(fp0);
            gen_set_label(l1);
        }
        break;
    case OPC_MOVN_S:
        check_insn_opc_removed(ctx, ISA_MIPS_R6);
        {
            TCGLabel *l1 = gen_new_label();
            TCGv_i32 fp0;

            if (ft != 0) {
                tcg_gen_brcondi_tl(TCG_COND_EQ, cpu_gpr[ft], 0, l1);
                gen_log_instr_gpr_update(ctx, ft);
                fp0 = tcg_temp_new_i32();
                gen_load_fpr32(ctx, fp0, fs);
                gen_store_fpr32(ctx, fp0, fd);
                tcg_temp_free_i32(fp0);
                gen_set_label(l1);
            }
        }
        break;
    case OPC_RECIP_S:
        {
            TCGv_i32 fp0 = tcg_temp_new_i32();

            gen_load_fpr32(ctx, fp0, fs);
            gen_helper_float_recip_s(fp0, cpu_env, fp0);
            gen_store_fpr32(ctx, fp0, fd);
            tcg_temp_free_i32(fp0);
        }
        break;
    case OPC_RSQRT_S:
        {
            TCGv_i32 fp0 = tcg_temp_new_i32();

            gen_load_fpr32(ctx, fp0, fs);
            gen_helper_float_rsqrt_s(fp0, cpu_env, fp0);
            gen_store_fpr32(ctx, fp0, fd);
            tcg_temp_free_i32(fp0);
        }
        break;
    case OPC_MADDF_S:
        check_insn(ctx, ISA_MIPS_R6);
        {
            TCGv_i32 fp0 = tcg_temp_new_i32();
            TCGv_i32 fp1 = tcg_temp_new_i32();
            TCGv_i32 fp2 = tcg_temp_new_i32();
            gen_load_fpr32(ctx, fp0, fs);
            gen_load_fpr32(ctx, fp1, ft);
            gen_load_fpr32(ctx, fp2, fd);
            gen_helper_float_maddf_s(fp2, cpu_env, fp0, fp1, fp2);
            gen_store_fpr32(ctx, fp2, fd);
            tcg_temp_free_i32(fp2);
            tcg_temp_free_i32(fp1);
            tcg_temp_free_i32(fp0);
        }
        break;
    case OPC_MSUBF_S:
        check_insn(ctx, ISA_MIPS_R6);
        {
            TCGv_i32 fp0 = tcg_temp_new_i32();
            TCGv_i32 fp1 = tcg_temp_new_i32();
            TCGv_i32 fp2 = tcg_temp_new_i32();
            gen_load_fpr32(ctx, fp0, fs);
            gen_load_fpr32(ctx, fp1, ft);
            gen_load_fpr32(ctx, fp2, fd);
            gen_helper_float_msubf_s(fp2, cpu_env, fp0, fp1, fp2);
            gen_store_fpr32(ctx, fp2, fd);
            tcg_temp_free_i32(fp2);
            tcg_temp_free_i32(fp1);
            tcg_temp_free_i32(fp0);
        }
        break;
    case OPC_RINT_S:
        check_insn(ctx, ISA_MIPS_R6);
        {
            TCGv_i32 fp0 = tcg_temp_new_i32();
            gen_load_fpr32(ctx, fp0, fs);
            gen_helper_float_rint_s(fp0, cpu_env, fp0);
            gen_store_fpr32(ctx, fp0, fd);
            tcg_temp_free_i32(fp0);
        }
        break;
    case OPC_CLASS_S:
        check_insn(ctx, ISA_MIPS_R6);
        {
            TCGv_i32 fp0 = tcg_temp_new_i32();
            gen_load_fpr32(ctx, fp0, fs);
            gen_helper_float_class_s(fp0, cpu_env, fp0);
            gen_store_fpr32(ctx, fp0, fd);
            tcg_temp_free_i32(fp0);
        }
        break;
    case OPC_MIN_S: /* OPC_RECIP2_S */
        if (ctx->insn_flags & ISA_MIPS_R6) {
            /* OPC_MIN_S */
            TCGv_i32 fp0 = tcg_temp_new_i32();
            TCGv_i32 fp1 = tcg_temp_new_i32();
            TCGv_i32 fp2 = tcg_temp_new_i32();
            gen_load_fpr32(ctx, fp0, fs);
            gen_load_fpr32(ctx, fp1, ft);
            gen_helper_float_min_s(fp2, cpu_env, fp0, fp1);
            gen_store_fpr32(ctx, fp2, fd);
            tcg_temp_free_i32(fp2);
            tcg_temp_free_i32(fp1);
            tcg_temp_free_i32(fp0);
        } else {
            /* OPC_RECIP2_S */
            check_cp1_64bitmode(ctx);
            {
                TCGv_i32 fp0 = tcg_temp_new_i32();
                TCGv_i32 fp1 = tcg_temp_new_i32();

                gen_load_fpr32(ctx, fp0, fs);
                gen_load_fpr32(ctx, fp1, ft);
                gen_helper_float_recip2_s(fp0, cpu_env, fp0, fp1);
                tcg_temp_free_i32(fp1);
                gen_store_fpr32(ctx, fp0, fd);
                tcg_temp_free_i32(fp0);
            }
        }
        break;
    case OPC_MINA_S: /* OPC_RECIP1_S */
        if (ctx->insn_flags & ISA_MIPS_R6) {
            /* OPC_MINA_S */
            TCGv_i32 fp0 = tcg_temp_new_i32();
            TCGv_i32 fp1 = tcg_temp_new_i32();
            TCGv_i32 fp2 = tcg_temp_new_i32();
            gen_load_fpr32(ctx, fp0, fs);
            gen_load_fpr32(ctx, fp1, ft);
            gen_helper_float_mina_s(fp2, cpu_env, fp0, fp1);
            gen_store_fpr32(ctx, fp2, fd);
            tcg_temp_free_i32(fp2);
            tcg_temp_free_i32(fp1);
            tcg_temp_free_i32(fp0);
        } else {
            /* OPC_RECIP1_S */
            check_cp1_64bitmode(ctx);
            {
                TCGv_i32 fp0 = tcg_temp_new_i32();

                gen_load_fpr32(ctx, fp0, fs);
                gen_helper_float_recip1_s(fp0, cpu_env, fp0);
                gen_store_fpr32(ctx, fp0, fd);
                tcg_temp_free_i32(fp0);
            }
        }
        break;
    case OPC_MAX_S: /* OPC_RSQRT1_S */
        if (ctx->insn_flags & ISA_MIPS_R6) {
            /* OPC_MAX_S */
            TCGv_i32 fp0 = tcg_temp_new_i32();
            TCGv_i32 fp1 = tcg_temp_new_i32();
            gen_load_fpr32(ctx, fp0, fs);
            gen_load_fpr32(ctx, fp1, ft);
            gen_helper_float_max_s(fp1, cpu_env, fp0, fp1);
            gen_store_fpr32(ctx, fp1, fd);
            tcg_temp_free_i32(fp1);
            tcg_temp_free_i32(fp0);
        } else {
            /* OPC_RSQRT1_S */
            check_cp1_64bitmode(ctx);
            {
                TCGv_i32 fp0 = tcg_temp_new_i32();

                gen_load_fpr32(ctx, fp0, fs);
                gen_helper_float_rsqrt1_s(fp0, cpu_env, fp0);
                gen_store_fpr32(ctx, fp0, fd);
                tcg_temp_free_i32(fp0);
            }
        }
        break;
    case OPC_MAXA_S: /* OPC_RSQRT2_S */
        if (ctx->insn_flags & ISA_MIPS_R6) {
            /* OPC_MAXA_S */
            TCGv_i32 fp0 = tcg_temp_new_i32();
            TCGv_i32 fp1 = tcg_temp_new_i32();
            gen_load_fpr32(ctx, fp0, fs);
            gen_load_fpr32(ctx, fp1, ft);
            gen_helper_float_maxa_s(fp1, cpu_env, fp0, fp1);
            gen_store_fpr32(ctx, fp1, fd);
            tcg_temp_free_i32(fp1);
            tcg_temp_free_i32(fp0);
        } else {
            /* OPC_RSQRT2_S */
            check_cp1_64bitmode(ctx);
            {
                TCGv_i32 fp0 = tcg_temp_new_i32();
                TCGv_i32 fp1 = tcg_temp_new_i32();

                gen_load_fpr32(ctx, fp0, fs);
                gen_load_fpr32(ctx, fp1, ft);
                gen_helper_float_rsqrt2_s(fp0, cpu_env, fp0, fp1);
                tcg_temp_free_i32(fp1);
                gen_store_fpr32(ctx, fp0, fd);
                tcg_temp_free_i32(fp0);
            }
        }
        break;
    case OPC_CVT_D_S:
        check_cp1_registers(ctx, fd);
        {
            TCGv_i32 fp32 = tcg_temp_new_i32();
            TCGv_i64 fp64 = tcg_temp_new_i64();

            gen_load_fpr32(ctx, fp32, fs);
            gen_helper_float_cvtd_s(fp64, cpu_env, fp32);
            tcg_temp_free_i32(fp32);
            gen_store_fpr64(ctx, fp64, fd);
            tcg_temp_free_i64(fp64);
        }
        break;
    case OPC_CVT_W_S:
        {
            TCGv_i32 fp0 = tcg_temp_new_i32();

            gen_load_fpr32(ctx, fp0, fs);
            if (ctx->nan2008) {
                gen_helper_float_cvt_2008_w_s(fp0, cpu_env, fp0);
            } else {
                gen_helper_float_cvt_w_s(fp0, cpu_env, fp0);
            }
            gen_store_fpr32(ctx, fp0, fd);
            tcg_temp_free_i32(fp0);
        }
        break;
    case OPC_CVT_L_S:
        check_cp1_64bitmode(ctx);
        {
            TCGv_i32 fp32 = tcg_temp_new_i32();
            TCGv_i64 fp64 = tcg_temp_new_i64();

            gen_load_fpr32(ctx, fp32, fs);
            if (ctx->nan2008) {
                gen_helper_float_cvt_2008_l_s(fp64, cpu_env, fp32);
            } else {
                gen_helper_float_cvt_l_s(fp64, cpu_env, fp32);
            }
            tcg_temp_free_i32(fp32);
            gen_store_fpr64(ctx, fp64, fd);
            tcg_temp_free_i64(fp64);
        }
        break;
    case OPC_CVT_PS_S:
        check_ps(ctx);
        {
            TCGv_i64 fp64 = tcg_temp_new_i64();
            TCGv_i32 fp32_0 = tcg_temp_new_i32();
            TCGv_i32 fp32_1 = tcg_temp_new_i32();

            gen_load_fpr32(ctx, fp32_0, fs);
            gen_load_fpr32(ctx, fp32_1, ft);
            tcg_gen_concat_i32_i64(fp64, fp32_1, fp32_0);
            tcg_temp_free_i32(fp32_1);
            tcg_temp_free_i32(fp32_0);
            gen_store_fpr64(ctx, fp64, fd);
            tcg_temp_free_i64(fp64);
        }
        break;
    case OPC_CMP_F_S:
    case OPC_CMP_UN_S:
    case OPC_CMP_EQ_S:
    case OPC_CMP_UEQ_S:
    case OPC_CMP_OLT_S:
    case OPC_CMP_ULT_S:
    case OPC_CMP_OLE_S:
    case OPC_CMP_ULE_S:
    case OPC_CMP_SF_S:
    case OPC_CMP_NGLE_S:
    case OPC_CMP_SEQ_S:
    case OPC_CMP_NGL_S:
    case OPC_CMP_LT_S:
    case OPC_CMP_NGE_S:
    case OPC_CMP_LE_S:
    case OPC_CMP_NGT_S:
        check_insn_opc_removed(ctx, ISA_MIPS_R6);
        if (ctx->opcode & (1 << 6)) {
            gen_cmpabs_s(ctx, func - 48, ft, fs, cc);
        } else {
            gen_cmp_s(ctx, func - 48, ft, fs, cc);
        }
        break;
    case OPC_ADD_D:
        check_cp1_registers(ctx, fs | ft | fd);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();
            TCGv_i64 fp1 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, fs);
            gen_load_fpr64(ctx, fp1, ft);
            gen_helper_float_add_d(fp0, cpu_env, fp0, fp1);
            tcg_temp_free_i64(fp1);
            gen_store_fpr64(ctx, fp0, fd);
            tcg_temp_free_i64(fp0);
        }
        break;
    case OPC_SUB_D:
        check_cp1_registers(ctx, fs | ft | fd);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();
            TCGv_i64 fp1 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, fs);
            gen_load_fpr64(ctx, fp1, ft);
            gen_helper_float_sub_d(fp0, cpu_env, fp0, fp1);
            tcg_temp_free_i64(fp1);
            gen_store_fpr64(ctx, fp0, fd);
            tcg_temp_free_i64(fp0);
        }
        break;
    case OPC_MUL_D:
        check_cp1_registers(ctx, fs | ft | fd);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();
            TCGv_i64 fp1 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, fs);
            gen_load_fpr64(ctx, fp1, ft);
            gen_helper_float_mul_d(fp0, cpu_env, fp0, fp1);
            tcg_temp_free_i64(fp1);
            gen_store_fpr64(ctx, fp0, fd);
            tcg_temp_free_i64(fp0);
        }
        break;
    case OPC_DIV_D:
        check_cp1_registers(ctx, fs | ft | fd);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();
            TCGv_i64 fp1 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, fs);
            gen_load_fpr64(ctx, fp1, ft);
            gen_helper_float_div_d(fp0, cpu_env, fp0, fp1);
            tcg_temp_free_i64(fp1);
            gen_store_fpr64(ctx, fp0, fd);
            tcg_temp_free_i64(fp0);
        }
        break;
    case OPC_SQRT_D:
        check_cp1_registers(ctx, fs | fd);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, fs);
            gen_helper_float_sqrt_d(fp0, cpu_env, fp0);
            gen_store_fpr64(ctx, fp0, fd);
            tcg_temp_free_i64(fp0);
        }
        break;
    case OPC_ABS_D:
        check_cp1_registers(ctx, fs | fd);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, fs);
            if (ctx->abs2008) {
                tcg_gen_andi_i64(fp0, fp0, 0x7fffffffffffffffULL);
            } else {
                gen_helper_float_abs_d(fp0, fp0);
            }
            gen_store_fpr64(ctx, fp0, fd);
            tcg_temp_free_i64(fp0);
        }
        break;
    case OPC_MOV_D:
        check_cp1_registers(ctx, fs | fd);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, fs);
            gen_store_fpr64(ctx, fp0, fd);
            tcg_temp_free_i64(fp0);
        }
        break;
    case OPC_NEG_D:
        check_cp1_registers(ctx, fs | fd);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, fs);
            if (ctx->abs2008) {
                tcg_gen_xori_i64(fp0, fp0, 1ULL << 63);
            } else {
                gen_helper_float_chs_d(fp0, fp0);
            }
            gen_store_fpr64(ctx, fp0, fd);
            tcg_temp_free_i64(fp0);
        }
        break;
    case OPC_ROUND_L_D:
        check_cp1_64bitmode(ctx);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, fs);
            if (ctx->nan2008) {
                gen_helper_float_round_2008_l_d(fp0, cpu_env, fp0);
            } else {
                gen_helper_float_round_l_d(fp0, cpu_env, fp0);
            }
            gen_store_fpr64(ctx, fp0, fd);
            tcg_temp_free_i64(fp0);
        }
        break;
    case OPC_TRUNC_L_D:
        check_cp1_64bitmode(ctx);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, fs);
            if (ctx->nan2008) {
                gen_helper_float_trunc_2008_l_d(fp0, cpu_env, fp0);
            } else {
                gen_helper_float_trunc_l_d(fp0, cpu_env, fp0);
            }
            gen_store_fpr64(ctx, fp0, fd);
            tcg_temp_free_i64(fp0);
        }
        break;
    case OPC_CEIL_L_D:
        check_cp1_64bitmode(ctx);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, fs);
            if (ctx->nan2008) {
                gen_helper_float_ceil_2008_l_d(fp0, cpu_env, fp0);
            } else {
                gen_helper_float_ceil_l_d(fp0, cpu_env, fp0);
            }
            gen_store_fpr64(ctx, fp0, fd);
            tcg_temp_free_i64(fp0);
        }
        break;
    case OPC_FLOOR_L_D:
        check_cp1_64bitmode(ctx);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, fs);
            if (ctx->nan2008) {
                gen_helper_float_floor_2008_l_d(fp0, cpu_env, fp0);
            } else {
                gen_helper_float_floor_l_d(fp0, cpu_env, fp0);
            }
            gen_store_fpr64(ctx, fp0, fd);
            tcg_temp_free_i64(fp0);
        }
        break;
    case OPC_ROUND_W_D:
        check_cp1_registers(ctx, fs);
        {
            TCGv_i32 fp32 = tcg_temp_new_i32();
            TCGv_i64 fp64 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp64, fs);
            if (ctx->nan2008) {
                gen_helper_float_round_2008_w_d(fp32, cpu_env, fp64);
            } else {
                gen_helper_float_round_w_d(fp32, cpu_env, fp64);
            }
            tcg_temp_free_i64(fp64);
            gen_store_fpr32(ctx, fp32, fd);
            tcg_temp_free_i32(fp32);
        }
        break;
    case OPC_TRUNC_W_D:
        check_cp1_registers(ctx, fs);
        {
            TCGv_i32 fp32 = tcg_temp_new_i32();
            TCGv_i64 fp64 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp64, fs);
            if (ctx->nan2008) {
                gen_helper_float_trunc_2008_w_d(fp32, cpu_env, fp64);
            } else {
                gen_helper_float_trunc_w_d(fp32, cpu_env, fp64);
            }
            tcg_temp_free_i64(fp64);
            gen_store_fpr32(ctx, fp32, fd);
            tcg_temp_free_i32(fp32);
        }
        break;
    case OPC_CEIL_W_D:
        check_cp1_registers(ctx, fs);
        {
            TCGv_i32 fp32 = tcg_temp_new_i32();
            TCGv_i64 fp64 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp64, fs);
            if (ctx->nan2008) {
                gen_helper_float_ceil_2008_w_d(fp32, cpu_env, fp64);
            } else {
                gen_helper_float_ceil_w_d(fp32, cpu_env, fp64);
            }
            tcg_temp_free_i64(fp64);
            gen_store_fpr32(ctx, fp32, fd);
            tcg_temp_free_i32(fp32);
        }
        break;
    case OPC_FLOOR_W_D:
        check_cp1_registers(ctx, fs);
        {
            TCGv_i32 fp32 = tcg_temp_new_i32();
            TCGv_i64 fp64 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp64, fs);
            if (ctx->nan2008) {
                gen_helper_float_floor_2008_w_d(fp32, cpu_env, fp64);
            } else {
                gen_helper_float_floor_w_d(fp32, cpu_env, fp64);
            }
            tcg_temp_free_i64(fp64);
            gen_store_fpr32(ctx, fp32, fd);
            tcg_temp_free_i32(fp32);
        }
        break;
    case OPC_SEL_D:
        check_insn(ctx, ISA_MIPS_R6);
        gen_sel_d(ctx, op1, fd, ft, fs);
        break;
    case OPC_SELEQZ_D:
        check_insn(ctx, ISA_MIPS_R6);
        gen_sel_d(ctx, op1, fd, ft, fs);
        break;
    case OPC_SELNEZ_D:
        check_insn(ctx, ISA_MIPS_R6);
        gen_sel_d(ctx, op1, fd, ft, fs);
        break;
    case OPC_MOVCF_D:
        check_insn_opc_removed(ctx, ISA_MIPS_R6);
        gen_movcf_d(ctx, fs, fd, (ft >> 2) & 0x7, ft & 0x1);
        break;
    case OPC_MOVZ_D:
        check_insn_opc_removed(ctx, ISA_MIPS_R6);
        {
            TCGLabel *l1 = gen_new_label();
            TCGv_i64 fp0;

            if (ft != 0) {
                tcg_gen_brcondi_tl(TCG_COND_NE, cpu_gpr[ft], 0, l1);
                gen_log_instr_gpr_update(ctx, ft);
            }
            fp0 = tcg_temp_new_i64();
            gen_load_fpr64(ctx, fp0, fs);
            gen_store_fpr64(ctx, fp0, fd);
            tcg_temp_free_i64(fp0);
            gen_set_label(l1);
        }
        break;
    case OPC_MOVN_D:
        check_insn_opc_removed(ctx, ISA_MIPS_R6);
        {
            TCGLabel *l1 = gen_new_label();
            TCGv_i64 fp0;

            if (ft != 0) {
                tcg_gen_brcondi_tl(TCG_COND_EQ, cpu_gpr[ft], 0, l1);
                gen_log_instr_gpr_update(ctx, ft);
                fp0 = tcg_temp_new_i64();
                gen_load_fpr64(ctx, fp0, fs);
                gen_store_fpr64(ctx, fp0, fd);
                tcg_temp_free_i64(fp0);
                gen_set_label(l1);
            }
        }
        break;
    case OPC_RECIP_D:
        check_cp1_registers(ctx, fs | fd);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, fs);
            gen_helper_float_recip_d(fp0, cpu_env, fp0);
            gen_store_fpr64(ctx, fp0, fd);
            tcg_temp_free_i64(fp0);
        }
        break;
    case OPC_RSQRT_D:
        check_cp1_registers(ctx, fs | fd);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, fs);
            gen_helper_float_rsqrt_d(fp0, cpu_env, fp0);
            gen_store_fpr64(ctx, fp0, fd);
            tcg_temp_free_i64(fp0);
        }
        break;
    case OPC_MADDF_D:
        check_insn(ctx, ISA_MIPS_R6);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();
            TCGv_i64 fp1 = tcg_temp_new_i64();
            TCGv_i64 fp2 = tcg_temp_new_i64();
            gen_load_fpr64(ctx, fp0, fs);
            gen_load_fpr64(ctx, fp1, ft);
            gen_load_fpr64(ctx, fp2, fd);
            gen_helper_float_maddf_d(fp2, cpu_env, fp0, fp1, fp2);
            gen_store_fpr64(ctx, fp2, fd);
            tcg_temp_free_i64(fp2);
            tcg_temp_free_i64(fp1);
            tcg_temp_free_i64(fp0);
        }
        break;
    case OPC_MSUBF_D:
        check_insn(ctx, ISA_MIPS_R6);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();
            TCGv_i64 fp1 = tcg_temp_new_i64();
            TCGv_i64 fp2 = tcg_temp_new_i64();
            gen_load_fpr64(ctx, fp0, fs);
            gen_load_fpr64(ctx, fp1, ft);
            gen_load_fpr64(ctx, fp2, fd);
            gen_helper_float_msubf_d(fp2, cpu_env, fp0, fp1, fp2);
            gen_store_fpr64(ctx, fp2, fd);
            tcg_temp_free_i64(fp2);
            tcg_temp_free_i64(fp1);
            tcg_temp_free_i64(fp0);
        }
        break;
    case OPC_RINT_D:
        check_insn(ctx, ISA_MIPS_R6);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();
            gen_load_fpr64(ctx, fp0, fs);
            gen_helper_float_rint_d(fp0, cpu_env, fp0);
            gen_store_fpr64(ctx, fp0, fd);
            tcg_temp_free_i64(fp0);
        }
        break;
    case OPC_CLASS_D:
        check_insn(ctx, ISA_MIPS_R6);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();
            gen_load_fpr64(ctx, fp0, fs);
            gen_helper_float_class_d(fp0, cpu_env, fp0);
            gen_store_fpr64(ctx, fp0, fd);
            tcg_temp_free_i64(fp0);
        }
        break;
    case OPC_MIN_D: /* OPC_RECIP2_D */
        if (ctx->insn_flags & ISA_MIPS_R6) {
            /* OPC_MIN_D */
            TCGv_i64 fp0 = tcg_temp_new_i64();
            TCGv_i64 fp1 = tcg_temp_new_i64();
            gen_load_fpr64(ctx, fp0, fs);
            gen_load_fpr64(ctx, fp1, ft);
            gen_helper_float_min_d(fp1, cpu_env, fp0, fp1);
            gen_store_fpr64(ctx, fp1, fd);
            tcg_temp_free_i64(fp1);
            tcg_temp_free_i64(fp0);
        } else {
            /* OPC_RECIP2_D */
            check_cp1_64bitmode(ctx);
            {
                TCGv_i64 fp0 = tcg_temp_new_i64();
                TCGv_i64 fp1 = tcg_temp_new_i64();

                gen_load_fpr64(ctx, fp0, fs);
                gen_load_fpr64(ctx, fp1, ft);
                gen_helper_float_recip2_d(fp0, cpu_env, fp0, fp1);
                tcg_temp_free_i64(fp1);
                gen_store_fpr64(ctx, fp0, fd);
                tcg_temp_free_i64(fp0);
            }
        }
        break;
    case OPC_MINA_D: /* OPC_RECIP1_D */
        if (ctx->insn_flags & ISA_MIPS_R6) {
            /* OPC_MINA_D */
            TCGv_i64 fp0 = tcg_temp_new_i64();
            TCGv_i64 fp1 = tcg_temp_new_i64();
            gen_load_fpr64(ctx, fp0, fs);
            gen_load_fpr64(ctx, fp1, ft);
            gen_helper_float_mina_d(fp1, cpu_env, fp0, fp1);
            gen_store_fpr64(ctx, fp1, fd);
            tcg_temp_free_i64(fp1);
            tcg_temp_free_i64(fp0);
        } else {
            /* OPC_RECIP1_D */
            check_cp1_64bitmode(ctx);
            {
                TCGv_i64 fp0 = tcg_temp_new_i64();

                gen_load_fpr64(ctx, fp0, fs);
                gen_helper_float_recip1_d(fp0, cpu_env, fp0);
                gen_store_fpr64(ctx, fp0, fd);
                tcg_temp_free_i64(fp0);
            }
        }
        break;
    case OPC_MAX_D: /*  OPC_RSQRT1_D */
        if (ctx->insn_flags & ISA_MIPS_R6) {
            /* OPC_MAX_D */
            TCGv_i64 fp0 = tcg_temp_new_i64();
            TCGv_i64 fp1 = tcg_temp_new_i64();
            gen_load_fpr64(ctx, fp0, fs);
            gen_load_fpr64(ctx, fp1, ft);
            gen_helper_float_max_d(fp1, cpu_env, fp0, fp1);
            gen_store_fpr64(ctx, fp1, fd);
            tcg_temp_free_i64(fp1);
            tcg_temp_free_i64(fp0);
        } else {
            /* OPC_RSQRT1_D */
            check_cp1_64bitmode(ctx);
            {
                TCGv_i64 fp0 = tcg_temp_new_i64();

                gen_load_fpr64(ctx, fp0, fs);
                gen_helper_float_rsqrt1_d(fp0, cpu_env, fp0);
                gen_store_fpr64(ctx, fp0, fd);
                tcg_temp_free_i64(fp0);
            }
        }
        break;
    case OPC_MAXA_D: /* OPC_RSQRT2_D */
        if (ctx->insn_flags & ISA_MIPS_R6) {
            /* OPC_MAXA_D */
            TCGv_i64 fp0 = tcg_temp_new_i64();
            TCGv_i64 fp1 = tcg_temp_new_i64();
            gen_load_fpr64(ctx, fp0, fs);
            gen_load_fpr64(ctx, fp1, ft);
            gen_helper_float_maxa_d(fp1, cpu_env, fp0, fp1);
            gen_store_fpr64(ctx, fp1, fd);
            tcg_temp_free_i64(fp1);
            tcg_temp_free_i64(fp0);
        } else {
            /* OPC_RSQRT2_D */
            check_cp1_64bitmode(ctx);
            {
                TCGv_i64 fp0 = tcg_temp_new_i64();
                TCGv_i64 fp1 = tcg_temp_new_i64();

                gen_load_fpr64(ctx, fp0, fs);
                gen_load_fpr64(ctx, fp1, ft);
                gen_helper_float_rsqrt2_d(fp0, cpu_env, fp0, fp1);
                tcg_temp_free_i64(fp1);
                gen_store_fpr64(ctx, fp0, fd);
                tcg_temp_free_i64(fp0);
            }
        }
        break;
    case OPC_CMP_F_D:
    case OPC_CMP_UN_D:
    case OPC_CMP_EQ_D:
    case OPC_CMP_UEQ_D:
    case OPC_CMP_OLT_D:
    case OPC_CMP_ULT_D:
    case OPC_CMP_OLE_D:
    case OPC_CMP_ULE_D:
    case OPC_CMP_SF_D:
    case OPC_CMP_NGLE_D:
    case OPC_CMP_SEQ_D:
    case OPC_CMP_NGL_D:
    case OPC_CMP_LT_D:
    case OPC_CMP_NGE_D:
    case OPC_CMP_LE_D:
    case OPC_CMP_NGT_D:
        check_insn_opc_removed(ctx, ISA_MIPS_R6);
        if (ctx->opcode & (1 << 6)) {
            gen_cmpabs_d(ctx, func - 48, ft, fs, cc);
        } else {
            gen_cmp_d(ctx, func - 48, ft, fs, cc);
        }
        break;
    case OPC_CVT_S_D:
        check_cp1_registers(ctx, fs);
        {
            TCGv_i32 fp32 = tcg_temp_new_i32();
            TCGv_i64 fp64 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp64, fs);
            gen_helper_float_cvts_d(fp32, cpu_env, fp64);
            tcg_temp_free_i64(fp64);
            gen_store_fpr32(ctx, fp32, fd);
            tcg_temp_free_i32(fp32);
        }
        break;
    case OPC_CVT_W_D:
        check_cp1_registers(ctx, fs);
        {
            TCGv_i32 fp32 = tcg_temp_new_i32();
            TCGv_i64 fp64 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp64, fs);
            if (ctx->nan2008) {
                gen_helper_float_cvt_2008_w_d(fp32, cpu_env, fp64);
            } else {
                gen_helper_float_cvt_w_d(fp32, cpu_env, fp64);
            }
            tcg_temp_free_i64(fp64);
            gen_store_fpr32(ctx, fp32, fd);
            tcg_temp_free_i32(fp32);
        }
        break;
    case OPC_CVT_L_D:
        check_cp1_64bitmode(ctx);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, fs);
            if (ctx->nan2008) {
                gen_helper_float_cvt_2008_l_d(fp0, cpu_env, fp0);
            } else {
                gen_helper_float_cvt_l_d(fp0, cpu_env, fp0);
            }
            gen_store_fpr64(ctx, fp0, fd);
            tcg_temp_free_i64(fp0);
        }
        break;
    case OPC_CVT_S_W:
        {
            TCGv_i32 fp0 = tcg_temp_new_i32();

            gen_load_fpr32(ctx, fp0, fs);
            gen_helper_float_cvts_w(fp0, cpu_env, fp0);
            gen_store_fpr32(ctx, fp0, fd);
            tcg_temp_free_i32(fp0);
        }
        break;
    case OPC_CVT_D_W:
        check_cp1_registers(ctx, fd);
        {
            TCGv_i32 fp32 = tcg_temp_new_i32();
            TCGv_i64 fp64 = tcg_temp_new_i64();

            gen_load_fpr32(ctx, fp32, fs);
            gen_helper_float_cvtd_w(fp64, cpu_env, fp32);
            tcg_temp_free_i32(fp32);
            gen_store_fpr64(ctx, fp64, fd);
            tcg_temp_free_i64(fp64);
        }
        break;
    case OPC_CVT_S_L:
        check_cp1_64bitmode(ctx);
        {
            TCGv_i32 fp32 = tcg_temp_new_i32();
            TCGv_i64 fp64 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp64, fs);
            gen_helper_float_cvts_l(fp32, cpu_env, fp64);
            tcg_temp_free_i64(fp64);
            gen_store_fpr32(ctx, fp32, fd);
            tcg_temp_free_i32(fp32);
        }
        break;
    case OPC_CVT_D_L:
        check_cp1_64bitmode(ctx);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, fs);
            gen_helper_float_cvtd_l(fp0, cpu_env, fp0);
            gen_store_fpr64(ctx, fp0, fd);
            tcg_temp_free_i64(fp0);
        }
        break;
    case OPC_CVT_PS_PW:
        check_ps(ctx);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, fs);
            gen_helper_float_cvtps_pw(fp0, cpu_env, fp0);
            gen_store_fpr64(ctx, fp0, fd);
            tcg_temp_free_i64(fp0);
        }
        break;
    case OPC_ADD_PS:
        check_ps(ctx);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();
            TCGv_i64 fp1 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, fs);
            gen_load_fpr64(ctx, fp1, ft);
            gen_helper_float_add_ps(fp0, cpu_env, fp0, fp1);
            tcg_temp_free_i64(fp1);
            gen_store_fpr64(ctx, fp0, fd);
            tcg_temp_free_i64(fp0);
        }
        break;
    case OPC_SUB_PS:
        check_ps(ctx);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();
            TCGv_i64 fp1 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, fs);
            gen_load_fpr64(ctx, fp1, ft);
            gen_helper_float_sub_ps(fp0, cpu_env, fp0, fp1);
            tcg_temp_free_i64(fp1);
            gen_store_fpr64(ctx, fp0, fd);
            tcg_temp_free_i64(fp0);
        }
        break;
    case OPC_MUL_PS:
        check_ps(ctx);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();
            TCGv_i64 fp1 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, fs);
            gen_load_fpr64(ctx, fp1, ft);
            gen_helper_float_mul_ps(fp0, cpu_env, fp0, fp1);
            tcg_temp_free_i64(fp1);
            gen_store_fpr64(ctx, fp0, fd);
            tcg_temp_free_i64(fp0);
        }
        break;
    case OPC_ABS_PS:
        check_ps(ctx);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, fs);
            gen_helper_float_abs_ps(fp0, fp0);
            gen_store_fpr64(ctx, fp0, fd);
            tcg_temp_free_i64(fp0);
        }
        break;
    case OPC_MOV_PS:
        check_ps(ctx);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, fs);
            gen_store_fpr64(ctx, fp0, fd);
            tcg_temp_free_i64(fp0);
        }
        break;
    case OPC_NEG_PS:
        check_ps(ctx);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, fs);
            gen_helper_float_chs_ps(fp0, fp0);
            gen_store_fpr64(ctx, fp0, fd);
            tcg_temp_free_i64(fp0);
        }
        break;
    case OPC_MOVCF_PS:
        check_ps(ctx);
        gen_movcf_ps(ctx, fs, fd, (ft >> 2) & 0x7, ft & 0x1);
        break;
    case OPC_MOVZ_PS:
        check_ps(ctx);
        {
            TCGLabel *l1 = gen_new_label();
            TCGv_i64 fp0;

            if (ft != 0) {
                tcg_gen_brcondi_tl(TCG_COND_NE, cpu_gpr[ft], 0, l1);
                gen_log_instr_gpr_update(ctx, ft);
            }
            fp0 = tcg_temp_new_i64();
            gen_load_fpr64(ctx, fp0, fs);
            gen_store_fpr64(ctx, fp0, fd);
            tcg_temp_free_i64(fp0);
            gen_set_label(l1);
        }
        break;
    case OPC_MOVN_PS:
        check_ps(ctx);
        {
            TCGLabel *l1 = gen_new_label();
            TCGv_i64 fp0;

            if (ft != 0) {
                tcg_gen_brcondi_tl(TCG_COND_EQ, cpu_gpr[ft], 0, l1);
                gen_log_instr_gpr_update(ctx, ft);
                fp0 = tcg_temp_new_i64();
                gen_load_fpr64(ctx, fp0, fs);
                gen_store_fpr64(ctx, fp0, fd);
                tcg_temp_free_i64(fp0);
                gen_set_label(l1);
            }
        }
        break;
    case OPC_ADDR_PS:
        check_ps(ctx);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();
            TCGv_i64 fp1 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, ft);
            gen_load_fpr64(ctx, fp1, fs);
            gen_helper_float_addr_ps(fp0, cpu_env, fp0, fp1);
            tcg_temp_free_i64(fp1);
            gen_store_fpr64(ctx, fp0, fd);
            tcg_temp_free_i64(fp0);
        }
        break;
    case OPC_MULR_PS:
        check_ps(ctx);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();
            TCGv_i64 fp1 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, ft);
            gen_load_fpr64(ctx, fp1, fs);
            gen_helper_float_mulr_ps(fp0, cpu_env, fp0, fp1);
            tcg_temp_free_i64(fp1);
            gen_store_fpr64(ctx, fp0, fd);
            tcg_temp_free_i64(fp0);
        }
        break;
    case OPC_RECIP2_PS:
        check_ps(ctx);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();
            TCGv_i64 fp1 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, fs);
            gen_load_fpr64(ctx, fp1, ft);
            gen_helper_float_recip2_ps(fp0, cpu_env, fp0, fp1);
            tcg_temp_free_i64(fp1);
            gen_store_fpr64(ctx, fp0, fd);
            tcg_temp_free_i64(fp0);
        }
        break;
    case OPC_RECIP1_PS:
        check_ps(ctx);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, fs);
            gen_helper_float_recip1_ps(fp0, cpu_env, fp0);
            gen_store_fpr64(ctx, fp0, fd);
            tcg_temp_free_i64(fp0);
        }
        break;
    case OPC_RSQRT1_PS:
        check_ps(ctx);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, fs);
            gen_helper_float_rsqrt1_ps(fp0, cpu_env, fp0);
            gen_store_fpr64(ctx, fp0, fd);
            tcg_temp_free_i64(fp0);
        }
        break;
    case OPC_RSQRT2_PS:
        check_ps(ctx);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();
            TCGv_i64 fp1 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, fs);
            gen_load_fpr64(ctx, fp1, ft);
            gen_helper_float_rsqrt2_ps(fp0, cpu_env, fp0, fp1);
            tcg_temp_free_i64(fp1);
            gen_store_fpr64(ctx, fp0, fd);
            tcg_temp_free_i64(fp0);
        }
        break;
    case OPC_CVT_S_PU:
        check_cp1_64bitmode(ctx);
        {
            TCGv_i32 fp0 = tcg_temp_new_i32();

            gen_load_fpr32h(ctx, fp0, fs);
            gen_helper_float_cvts_pu(fp0, cpu_env, fp0);
            gen_store_fpr32(ctx, fp0, fd);
            tcg_temp_free_i32(fp0);
        }
        break;
    case OPC_CVT_PW_PS:
        check_ps(ctx);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, fs);
            gen_helper_float_cvtpw_ps(fp0, cpu_env, fp0);
            gen_store_fpr64(ctx, fp0, fd);
            tcg_temp_free_i64(fp0);
        }
        break;
    case OPC_CVT_S_PL:
        check_cp1_64bitmode(ctx);
        {
            TCGv_i32 fp0 = tcg_temp_new_i32();

            gen_load_fpr32(ctx, fp0, fs);
            gen_helper_float_cvts_pl(fp0, cpu_env, fp0);
            gen_store_fpr32(ctx, fp0, fd);
            tcg_temp_free_i32(fp0);
        }
        break;
    case OPC_PLL_PS:
        check_ps(ctx);
        {
            TCGv_i32 fp0 = tcg_temp_new_i32();
            TCGv_i32 fp1 = tcg_temp_new_i32();

            gen_load_fpr32(ctx, fp0, fs);
            gen_load_fpr32(ctx, fp1, ft);
            gen_store_fpr32h(ctx, fp0, fd);
            gen_store_fpr32(ctx, fp1, fd);
            tcg_temp_free_i32(fp0);
            tcg_temp_free_i32(fp1);
        }
        break;
    case OPC_PLU_PS:
        check_ps(ctx);
        {
            TCGv_i32 fp0 = tcg_temp_new_i32();
            TCGv_i32 fp1 = tcg_temp_new_i32();

            gen_load_fpr32(ctx, fp0, fs);
            gen_load_fpr32h(ctx, fp1, ft);
            gen_store_fpr32(ctx, fp1, fd);
            gen_store_fpr32h(ctx, fp0, fd);
            tcg_temp_free_i32(fp0);
            tcg_temp_free_i32(fp1);
        }
        break;
    case OPC_PUL_PS:
        check_ps(ctx);
        {
            TCGv_i32 fp0 = tcg_temp_new_i32();
            TCGv_i32 fp1 = tcg_temp_new_i32();

            gen_load_fpr32h(ctx, fp0, fs);
            gen_load_fpr32(ctx, fp1, ft);
            gen_store_fpr32(ctx, fp1, fd);
            gen_store_fpr32h(ctx, fp0, fd);
            tcg_temp_free_i32(fp0);
            tcg_temp_free_i32(fp1);
        }
        break;
    case OPC_PUU_PS:
        check_ps(ctx);
        {
            TCGv_i32 fp0 = tcg_temp_new_i32();
            TCGv_i32 fp1 = tcg_temp_new_i32();

            gen_load_fpr32h(ctx, fp0, fs);
            gen_load_fpr32h(ctx, fp1, ft);
            gen_store_fpr32(ctx, fp1, fd);
            gen_store_fpr32h(ctx, fp0, fd);
            tcg_temp_free_i32(fp0);
            tcg_temp_free_i32(fp1);
        }
        break;
    case OPC_CMP_F_PS:
    case OPC_CMP_UN_PS:
    case OPC_CMP_EQ_PS:
    case OPC_CMP_UEQ_PS:
    case OPC_CMP_OLT_PS:
    case OPC_CMP_ULT_PS:
    case OPC_CMP_OLE_PS:
    case OPC_CMP_ULE_PS:
    case OPC_CMP_SF_PS:
    case OPC_CMP_NGLE_PS:
    case OPC_CMP_SEQ_PS:
    case OPC_CMP_NGL_PS:
    case OPC_CMP_LT_PS:
    case OPC_CMP_NGE_PS:
    case OPC_CMP_LE_PS:
    case OPC_CMP_NGT_PS:
        if (ctx->opcode & (1 << 6)) {
            gen_cmpabs_ps(ctx, func - 48, ft, fs, cc);
        } else {
            gen_cmp_ps(ctx, func - 48, ft, fs, cc);
        }
        break;
    default:
        MIPS_INVAL("farith");
        gen_reserved_instruction(ctx);
        return;
    }
}

/* Coprocessor 3 (FPU) */
static void gen_flt3_ldst(DisasContext *ctx, uint32_t opc,
                          int fd, int fs, int base, int index)
{
    TCGv t0 = tcg_temp_new();
#if defined(TARGET_CHERI) || defined(CONFIG_TCG_LOG_INSTR)
    TCGv t1 = tcg_temp_new();
#endif /* defined(TARGET_CHERI) || defined(CONFIG_TCG_LOG_INSTR) */

    if (base == 0) {
        gen_load_gpr(t0, index);
    } else if (index == 0) {
        gen_load_gpr(t0, base);
    } else {
        gen_op_addr_add(ctx, t0, cpu_gpr[base], cpu_gpr[index]);
    }
    /*
     * Don't do NOP if destination is zero: we must perform the actual
     * memory access.
     */
    switch (opc) {
    case OPC_LWXC1:
        check_cop1x(ctx);
        {
            TCGv_i32 fp0 = tcg_temp_new_i32();

            gen_ddc_interposed_ld_tl(ctx, t0, NULL/* add $ddc to t0*/, t0, ctx->mem_idx, MO_TESL);
            tcg_gen_trunc_tl_i32(fp0, t0);
            gen_store_fpr32(ctx, fp0, fd);
            tcg_temp_free_i32(fp0);
        }
        break;
    case OPC_LDXC1:
        check_cop1x(ctx);
        check_cp1_registers(ctx, fd);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();

            gen_ddc_interposed_ld_i64(ctx, fp0, NULL/* add $ddc to t0*/, t0, ctx->mem_idx, MO_TEQ);
            gen_store_fpr64(ctx, fp0, fd);
            tcg_temp_free_i64(fp0);
        }
        break;
    case OPC_LUXC1:
        check_cp1_64bitmode(ctx);
        tcg_gen_andi_tl(t0, t0, ~0x7);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();

            gen_ddc_interposed_ld_i64(ctx, fp0, NULL/* add $ddc to t0*/, t0, ctx->mem_idx, MO_TEQ);
            gen_store_fpr64(ctx, fp0, fd);
            tcg_temp_free_i64(fp0);
        }
        break;
    case OPC_SWXC1:
        check_cop1x(ctx);
        {
            TCGv_i32 fp0 = tcg_temp_new_i32();

            gen_load_fpr32(ctx, fp0, fs);
            gen_ddc_interposed_st_i32(ctx, fp0, NULL/* add $ddc to t0*/, t0, ctx->mem_idx, MO_TEUL);
            tcg_temp_free_i32(fp0);
        }
        break;
    case OPC_SDXC1:
        check_cop1x(ctx);
        check_cp1_registers(ctx, fs);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, fs);
            gen_ddc_interposed_st_i64(ctx, fp0, NULL/* add $ddc to t0*/, t0, ctx->mem_idx, MO_TEQ);
            tcg_temp_free_i64(fp0);
        }
        break;
    case OPC_SUXC1:
        check_cp1_64bitmode(ctx);
        tcg_gen_andi_tl(t0, t0, ~0x7);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, fs);
            gen_ddc_interposed_st_i64(ctx, fp0, NULL/* add $ddc to t0*/, t0, ctx->mem_idx, MO_TEQ);
            tcg_temp_free_i64(fp0);
        }
        break;
    }
    tcg_temp_free(t0);
#if defined(TARGET_CHERI) || defined(CONFIG_TCG_LOG_INSTR)
    tcg_temp_free(t1);
#endif /* defined(TARGET_CHERI) || defined(CONFIG_TCG_LOG_INSTR) */
}

static void gen_flt3_arith(DisasContext *ctx, uint32_t opc,
                           int fd, int fr, int fs, int ft)
{
    switch (opc) {
    case OPC_ALNV_PS:
        check_ps(ctx);
        {
            TCGv t0 = tcg_temp_local_new();
            TCGv_i32 fp = tcg_temp_new_i32();
            TCGv_i32 fph = tcg_temp_new_i32();
            TCGLabel *l1 = gen_new_label();
            TCGLabel *l2 = gen_new_label();

            gen_load_gpr(t0, fr);
            tcg_gen_andi_tl(t0, t0, 0x7);

            tcg_gen_brcondi_tl(TCG_COND_NE, t0, 0, l1);
            gen_load_fpr32(ctx, fp, fs);
            gen_load_fpr32h(ctx, fph, fs);
            gen_store_fpr32(ctx, fp, fd);
            gen_store_fpr32h(ctx, fph, fd);
            tcg_gen_br(l2);
            gen_set_label(l1);
            tcg_gen_brcondi_tl(TCG_COND_NE, t0, 4, l2);
            tcg_temp_free(t0);
#ifdef TARGET_WORDS_BIGENDIAN
            gen_load_fpr32(ctx, fp, fs);
            gen_load_fpr32h(ctx, fph, ft);
            gen_store_fpr32h(ctx, fp, fd);
            gen_store_fpr32(ctx, fph, fd);
#else
            gen_load_fpr32h(ctx, fph, fs);
            gen_load_fpr32(ctx, fp, ft);
            gen_store_fpr32(ctx, fph, fd);
            gen_store_fpr32h(ctx, fp, fd);
#endif
            gen_set_label(l2);
            tcg_temp_free_i32(fp);
            tcg_temp_free_i32(fph);
        }
        break;
    case OPC_MADD_S:
        check_cop1x(ctx);
        {
            TCGv_i32 fp0 = tcg_temp_new_i32();
            TCGv_i32 fp1 = tcg_temp_new_i32();
            TCGv_i32 fp2 = tcg_temp_new_i32();

            gen_load_fpr32(ctx, fp0, fs);
            gen_load_fpr32(ctx, fp1, ft);
            gen_load_fpr32(ctx, fp2, fr);
            gen_helper_float_madd_s(fp2, cpu_env, fp0, fp1, fp2);
            tcg_temp_free_i32(fp0);
            tcg_temp_free_i32(fp1);
            gen_store_fpr32(ctx, fp2, fd);
            tcg_temp_free_i32(fp2);
        }
        break;
    case OPC_MADD_D:
        check_cop1x(ctx);
        check_cp1_registers(ctx, fd | fs | ft | fr);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();
            TCGv_i64 fp1 = tcg_temp_new_i64();
            TCGv_i64 fp2 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, fs);
            gen_load_fpr64(ctx, fp1, ft);
            gen_load_fpr64(ctx, fp2, fr);
            gen_helper_float_madd_d(fp2, cpu_env, fp0, fp1, fp2);
            tcg_temp_free_i64(fp0);
            tcg_temp_free_i64(fp1);
            gen_store_fpr64(ctx, fp2, fd);
            tcg_temp_free_i64(fp2);
        }
        break;
    case OPC_MADD_PS:
        check_ps(ctx);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();
            TCGv_i64 fp1 = tcg_temp_new_i64();
            TCGv_i64 fp2 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, fs);
            gen_load_fpr64(ctx, fp1, ft);
            gen_load_fpr64(ctx, fp2, fr);
            gen_helper_float_madd_ps(fp2, cpu_env, fp0, fp1, fp2);
            tcg_temp_free_i64(fp0);
            tcg_temp_free_i64(fp1);
            gen_store_fpr64(ctx, fp2, fd);
            tcg_temp_free_i64(fp2);
        }
        break;
    case OPC_MSUB_S:
        check_cop1x(ctx);
        {
            TCGv_i32 fp0 = tcg_temp_new_i32();
            TCGv_i32 fp1 = tcg_temp_new_i32();
            TCGv_i32 fp2 = tcg_temp_new_i32();

            gen_load_fpr32(ctx, fp0, fs);
            gen_load_fpr32(ctx, fp1, ft);
            gen_load_fpr32(ctx, fp2, fr);
            gen_helper_float_msub_s(fp2, cpu_env, fp0, fp1, fp2);
            tcg_temp_free_i32(fp0);
            tcg_temp_free_i32(fp1);
            gen_store_fpr32(ctx, fp2, fd);
            tcg_temp_free_i32(fp2);
        }
        break;
    case OPC_MSUB_D:
        check_cop1x(ctx);
        check_cp1_registers(ctx, fd | fs | ft | fr);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();
            TCGv_i64 fp1 = tcg_temp_new_i64();
            TCGv_i64 fp2 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, fs);
            gen_load_fpr64(ctx, fp1, ft);
            gen_load_fpr64(ctx, fp2, fr);
            gen_helper_float_msub_d(fp2, cpu_env, fp0, fp1, fp2);
            tcg_temp_free_i64(fp0);
            tcg_temp_free_i64(fp1);
            gen_store_fpr64(ctx, fp2, fd);
            tcg_temp_free_i64(fp2);
        }
        break;
    case OPC_MSUB_PS:
        check_ps(ctx);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();
            TCGv_i64 fp1 = tcg_temp_new_i64();
            TCGv_i64 fp2 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, fs);
            gen_load_fpr64(ctx, fp1, ft);
            gen_load_fpr64(ctx, fp2, fr);
            gen_helper_float_msub_ps(fp2, cpu_env, fp0, fp1, fp2);
            tcg_temp_free_i64(fp0);
            tcg_temp_free_i64(fp1);
            gen_store_fpr64(ctx, fp2, fd);
            tcg_temp_free_i64(fp2);
        }
        break;
    case OPC_NMADD_S:
        check_cop1x(ctx);
        {
            TCGv_i32 fp0 = tcg_temp_new_i32();
            TCGv_i32 fp1 = tcg_temp_new_i32();
            TCGv_i32 fp2 = tcg_temp_new_i32();

            gen_load_fpr32(ctx, fp0, fs);
            gen_load_fpr32(ctx, fp1, ft);
            gen_load_fpr32(ctx, fp2, fr);
            gen_helper_float_nmadd_s(fp2, cpu_env, fp0, fp1, fp2);
            tcg_temp_free_i32(fp0);
            tcg_temp_free_i32(fp1);
            gen_store_fpr32(ctx, fp2, fd);
            tcg_temp_free_i32(fp2);
        }
        break;
    case OPC_NMADD_D:
        check_cop1x(ctx);
        check_cp1_registers(ctx, fd | fs | ft | fr);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();
            TCGv_i64 fp1 = tcg_temp_new_i64();
            TCGv_i64 fp2 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, fs);
            gen_load_fpr64(ctx, fp1, ft);
            gen_load_fpr64(ctx, fp2, fr);
            gen_helper_float_nmadd_d(fp2, cpu_env, fp0, fp1, fp2);
            tcg_temp_free_i64(fp0);
            tcg_temp_free_i64(fp1);
            gen_store_fpr64(ctx, fp2, fd);
            tcg_temp_free_i64(fp2);
        }
        break;
    case OPC_NMADD_PS:
        check_ps(ctx);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();
            TCGv_i64 fp1 = tcg_temp_new_i64();
            TCGv_i64 fp2 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, fs);
            gen_load_fpr64(ctx, fp1, ft);
            gen_load_fpr64(ctx, fp2, fr);
            gen_helper_float_nmadd_ps(fp2, cpu_env, fp0, fp1, fp2);
            tcg_temp_free_i64(fp0);
            tcg_temp_free_i64(fp1);
            gen_store_fpr64(ctx, fp2, fd);
            tcg_temp_free_i64(fp2);
        }
        break;
    case OPC_NMSUB_S:
        check_cop1x(ctx);
        {
            TCGv_i32 fp0 = tcg_temp_new_i32();
            TCGv_i32 fp1 = tcg_temp_new_i32();
            TCGv_i32 fp2 = tcg_temp_new_i32();

            gen_load_fpr32(ctx, fp0, fs);
            gen_load_fpr32(ctx, fp1, ft);
            gen_load_fpr32(ctx, fp2, fr);
            gen_helper_float_nmsub_s(fp2, cpu_env, fp0, fp1, fp2);
            tcg_temp_free_i32(fp0);
            tcg_temp_free_i32(fp1);
            gen_store_fpr32(ctx, fp2, fd);
            tcg_temp_free_i32(fp2);
        }
        break;
    case OPC_NMSUB_D:
        check_cop1x(ctx);
        check_cp1_registers(ctx, fd | fs | ft | fr);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();
            TCGv_i64 fp1 = tcg_temp_new_i64();
            TCGv_i64 fp2 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, fs);
            gen_load_fpr64(ctx, fp1, ft);
            gen_load_fpr64(ctx, fp2, fr);
            gen_helper_float_nmsub_d(fp2, cpu_env, fp0, fp1, fp2);
            tcg_temp_free_i64(fp0);
            tcg_temp_free_i64(fp1);
            gen_store_fpr64(ctx, fp2, fd);
            tcg_temp_free_i64(fp2);
        }
        break;
    case OPC_NMSUB_PS:
        check_ps(ctx);
        {
            TCGv_i64 fp0 = tcg_temp_new_i64();
            TCGv_i64 fp1 = tcg_temp_new_i64();
            TCGv_i64 fp2 = tcg_temp_new_i64();

            gen_load_fpr64(ctx, fp0, fs);
            gen_load_fpr64(ctx, fp1, ft);
            gen_load_fpr64(ctx, fp2, fr);
            gen_helper_float_nmsub_ps(fp2, cpu_env, fp0, fp1, fp2);
            tcg_temp_free_i64(fp0);
            tcg_temp_free_i64(fp1);
            gen_store_fpr64(ctx, fp2, fd);
            tcg_temp_free_i64(fp2);
        }
        break;
    default:
        MIPS_INVAL("flt3_arith");
        gen_reserved_instruction(ctx);
        return;
    }
}

void gen_rdhwr(DisasContext *ctx, int rt, int rd, int sel)
{
    TCGv t0;

#if !defined(CONFIG_USER_ONLY)
    /*
     * The Linux kernel will emulate rdhwr if it's not supported natively.
     * Therefore only check the ISA in system mode.
     */
    check_insn(ctx, ISA_MIPS_R2);
#endif
    t0 = tcg_temp_new();

    switch (rd) {
    case 0:
        gen_helper_rdhwr_cpunum(t0, cpu_env);
        gen_store_gpr(t0, rt);
        break;
    case 1:
        gen_helper_rdhwr_synci_step(t0, cpu_env);
        gen_store_gpr(t0, rt);
        break;
    case 2:
        if (tb_cflags(ctx->base.tb) & CF_USE_ICOUNT) {
            gen_io_start();
        }
        gen_helper_rdhwr_cc(t0, cpu_env);
        gen_store_gpr(t0, rt);
        /*
         * Break the TB to be able to take timer interrupts immediately
         * after reading count. DISAS_STOP isn't sufficient, we need to ensure
         * we break completely out of translated code.
         */
        gen_save_pc(ctx->base.pc_next + 4);
        ctx->base.is_jmp = DISAS_EXIT;
        break;
    case 3:
        gen_helper_rdhwr_ccres(t0, cpu_env);
        gen_store_gpr(t0, rt);
        break;
#if !defined(TARGET_CHERI)
    case 4:
        check_insn(ctx, ISA_MIPS_R6);
        if (sel != 0) {
            /*
             * Performance counter registers are not implemented other than
             * control register 0.
             */
            generate_exception(ctx, EXCP_RI);
        }
        gen_helper_rdhwr_performance(t0, cpu_env);
        gen_store_gpr(t0, rt);
        break;
    case 5:
        check_insn(ctx, ISA_MIPS_R6);
        gen_helper_rdhwr_xnp(t0, cpu_env);
        gen_store_gpr(t0, rt);
        break;
#endif
    case 29:
#if defined(CONFIG_USER_ONLY)
        tcg_gen_ld_tl(t0, cpu_env,
                      offsetof(CPUMIPSState, active_tc.CP0_UserLocal));
        gen_store_gpr(t0, rt);
        break;
#else
        if ((ctx->hflags & MIPS_HFLAG_CP0) ||
            (ctx->hflags & MIPS_HFLAG_HWRENA_ULR)) {
            tcg_gen_ld_tl(t0, cpu_env,
                          offsetof(CPUMIPSState, active_tc.CP0_UserLocal));
            gen_store_gpr(t0, rt);
        } else {
            gen_reserved_instruction(ctx);
        }
        break;
#endif
#if defined(TARGET_CHERI)
    /*
     * Fake registers to keep libstatcounters from triggering segfaultr
     */
    case 4: /* ICOUNT */
        gen_helper_1e0i(rdhwr_statcounters_icount, t0, sel);
        gen_store_gpr(t0, rt);
        break;
    case 5: /* ITLB MISS */
        gen_helper_rdhwr_statcounters_itlb_miss(t0, cpu_env);
        gen_store_gpr(t0, rt);
        break;
    case 6: /* DTLB MISS */
        gen_helper_rdhwr_statcounters_dtlb_miss(t0, cpu_env);
        gen_store_gpr(t0, rt);
        break;
    case 7: /* RESET */
        gen_helper_rdhwr_statcounters_reset(t0, cpu_env);
        gen_store_gpr(t0, rt);
        break;
    case 11:
        gen_helper_1e0i(rdhwr_statcounters_memory, t0, sel);
        gen_store_gpr(t0, rt);
        break;
    case 8:
    case 9:
    case 10:
    case 12:
    case 13:
    case 14:
        gen_helper_1e0i(rdhwr_statcounters_ignored, t0, rd);
        gen_store_gpr(t0, rt);
        break;
#endif
    default:            /* Invalid */
        MIPS_INVAL("rdhwr");
        gen_reserved_instruction(ctx);
        break;
    }
    tcg_temp_free(t0);
}

static inline void clear_branch_hflags(DisasContext *ctx)
{
    ctx->hflags &= ~MIPS_HFLAG_BMASK;
    if (ctx->base.is_jmp == DISAS_NEXT) {
        save_cpu_state(ctx, 0);
    } else {
        /*
         * It is not safe to save ctx->hflags as hflags may be changed
         * in execution time by the instruction in delay / forbidden slot.
         */
        tcg_gen_andi_i32(hflags, hflags, ~MIPS_HFLAG_BMASK);
    }
}

static void gen_branch(DisasContext *ctx, int insn_bytes)
{
    if (ctx->hflags & MIPS_HFLAG_BMASK) {
        int proc_hflags = ctx->hflags & MIPS_HFLAG_BMASK;
        /* Branches completion */
        clear_branch_hflags(ctx);
        ctx->base.is_jmp = DISAS_NORETURN;
        /* FIXME: Need to clear can_do_io.  */
        switch (proc_hflags & MIPS_HFLAG_BMASK_BASE) {
        case MIPS_HFLAG_FBNSLOT:
            gen_goto_tb(ctx, 0, ctx->base.pc_next + insn_bytes);
            break;
        case MIPS_HFLAG_B:
            /* unconditional branch */
            if (proc_hflags & MIPS_HFLAG_BX) {
                tcg_gen_xori_i32(hflags, hflags, MIPS_HFLAG_M16);
            }
            gen_goto_tb(ctx, 0, ctx->btarget);
            break;
        case MIPS_HFLAG_BL:
            /* blikely taken case */
            gen_goto_tb(ctx, 0, ctx->btarget);
            break;
        case MIPS_HFLAG_BC:
            /* Conditional branch */
            {
                TCGLabel *l1 = gen_new_label();

                tcg_gen_brcondi_tl(TCG_COND_NE, bcond, 0, l1);
                gen_goto_tb(ctx, 1, ctx->base.pc_next + insn_bytes);
                gen_set_label(l1);
                gen_goto_tb(ctx, 0, ctx->btarget);
            }
            break;
        case MIPS_HFLAG_BR:
            /* unconditional branch to register */
            if (ctx->insn_flags & (ASE_MIPS16 | ASE_MICROMIPS)) {
                TCGv t0 = tcg_temp_new();
                TCGv_i32 t1 = tcg_temp_new_i32();

                tcg_gen_andi_tl(t0, btarget, 0x1);
                tcg_gen_trunc_tl_i32(t1, t0);
                tcg_temp_free(t0);
                tcg_gen_andi_i32(hflags, hflags, ~(uint32_t)MIPS_HFLAG_M16);
                tcg_gen_shli_i32(t1, t1, MIPS_HFLAG_M16_SHIFT);
                tcg_gen_or_i32(hflags, hflags, t1);
                tcg_temp_free_i32(t1);

                tcg_gen_andi_tl(cpu_PC, btarget, ~(target_ulong)0x1);
            } else {
                tcg_gen_mov_tl(cpu_PC, btarget);
            }
#ifdef CONFIG_DEBUG_TCG
            tcg_gen_movi_tl(_pc_is_current, 1); // PC has been updated.
#endif
            if (ctx->base.singlestep_enabled) {
                save_cpu_state(ctx, 0);
                gen_helper_raise_exception_debug(cpu_env);
            }
            tcg_gen_lookup_and_goto_ptr();
            break;
#ifdef TARGET_CHERI
        case MIPS_HFLAG_BRCCALL:
            /* unconditional branch to capability register from a ccall.
             * Can fall through since otype and seal are not copied anyway.
             */
            /* fallthrough */
        case MIPS_HFLAG_BRC:
            /* unconditional branch to capability register */

            tcg_gen_mov_tl(cpu_PC, btarget);
            /* Update PCC with capability register */
            gen_helper_copy_cap_btarget_to_pcc(cpu_env);
#ifdef CONFIG_DEBUG_TCG
            tcg_gen_movi_tl(_pc_is_current, 1); // PC has been updated.
#endif

            if (ctx->base.singlestep_enabled) {
                save_cpu_state(ctx, 0);
                gen_helper_0e0i(raise_exception, EXCP_DEBUG);
            }
            tcg_gen_exit_tb(NULL, 0);
            break;
#endif /* TARGET_CHERI */
        default:
            fprintf(stderr, "unknown branch 0x%x\n", proc_hflags);
            abort();
        }
    }
}

/* Compact Branches */
static void gen_compute_compact_branch(DisasContext *ctx, uint32_t opc,
                                       int rs, int rt, int32_t offset)
{
    int bcond_compute = 0;
    TCGv t0 = tcg_temp_new();
    TCGv t1 = tcg_temp_new();
    int m16_lowbit = (ctx->hflags & MIPS_HFLAG_M16) != 0;

    if (ctx->hflags & MIPS_HFLAG_BMASK) {
#ifdef MIPS_DEBUG_DISAS
        LOG_DISAS("Branch in delay / forbidden slot at PC 0x" TARGET_FMT_lx
                  "\n", ctx->base.pc_next);
#endif
        gen_reserved_instruction(ctx);
        goto out;
    }

    /* Load needed operands and calculate btarget */
    switch (opc) {
    /* compact branch */
    case OPC_BOVC: /* OPC_BEQZALC, OPC_BEQC */
    case OPC_BNVC: /* OPC_BNEZALC, OPC_BNEC */
        gen_load_gpr(t0, rs);
        gen_load_gpr(t1, rt);
        bcond_compute = 1;
        ctx->btarget = addr_add(ctx, ctx->base.pc_next + 4, offset);
        if (rs <= rt && rs == 0) {
            /* OPC_BEQZALC, OPC_BNEZALC */
            tcg_gen_movi_tl(cpu_gpr[31], ctx->base.pc_next + 4 + m16_lowbit);
            gen_log_instr_gpr_update(ctx, 31);
        }
        break;
    case OPC_BLEZC: /* OPC_BGEZC, OPC_BGEC */
    case OPC_BGTZC: /* OPC_BLTZC, OPC_BLTC */
        gen_load_gpr(t0, rs);
        gen_load_gpr(t1, rt);
        bcond_compute = 1;
        ctx->btarget = addr_add(ctx, ctx->base.pc_next + 4, offset);
        break;
    case OPC_BLEZALC: /* OPC_BGEZALC, OPC_BGEUC */
    case OPC_BGTZALC: /* OPC_BLTZALC, OPC_BLTUC */
        if (rs == 0 || rs == rt) {
            /* OPC_BLEZALC, OPC_BGEZALC */
            /* OPC_BGTZALC, OPC_BLTZALC */
            tcg_gen_movi_tl(cpu_gpr[31], ctx->base.pc_next + 4 + m16_lowbit);
            gen_log_instr_gpr_update(ctx, 31);
        }
        gen_load_gpr(t0, rs);
        gen_load_gpr(t1, rt);
        bcond_compute = 1;
        ctx->btarget = addr_add(ctx, ctx->base.pc_next + 4, offset);
        break;
    case OPC_BC:
    case OPC_BALC:
        ctx->btarget = addr_add(ctx, ctx->base.pc_next + 4, offset);
        break;
    case OPC_BEQZC:
    case OPC_BNEZC:
        if (rs != 0) {
            /* OPC_BEQZC, OPC_BNEZC */
            gen_load_gpr(t0, rs);
            bcond_compute = 1;
            ctx->btarget = addr_add(ctx, ctx->base.pc_next + 4, offset);
        } else {
            /* OPC_JIC, OPC_JIALC */
            TCGv tbase = tcg_temp_new();
            TCGv toffset = tcg_temp_new();

            gen_load_gpr(tbase, rt);
            tcg_gen_movi_tl(toffset, offset);
            gen_op_addr_add(ctx, btarget, tbase, toffset);
            tcg_temp_free(tbase);
            tcg_temp_free(toffset);
        }
        break;
    default:
        MIPS_INVAL("Compact branch/jump");
        gen_reserved_instruction(ctx);
        goto out;
    }

    if (bcond_compute == 0) {
        /* Uncoditional compact branch */
        switch (opc) {
        case OPC_JIALC:
            tcg_gen_movi_tl(cpu_gpr[31], ctx->base.pc_next + 4 + m16_lowbit);
            gen_log_instr_gpr_update(ctx, 31);
            /* Fallthrough */
        case OPC_JIC:
            ctx->hflags |= MIPS_HFLAG_BR;
            break;
        case OPC_BALC:
            tcg_gen_movi_tl(cpu_gpr[31], ctx->base.pc_next + 4 + m16_lowbit);
            gen_log_instr_gpr_update(ctx, 31);
            /* Fallthrough */
        case OPC_BC:
            ctx->hflags |= MIPS_HFLAG_B;
            break;
        default:
            MIPS_INVAL("Compact branch/jump");
            gen_reserved_instruction(ctx);
            goto out;
        }
        // Bounds check against $pcc for unconditional branches.
        gen_check_branch_target(ctx, ctx->btarget);
        /* Generating branch here as compact branches don't have delay slot */
        gen_branch(ctx, 4);
    } else {
        /* Conditional compact branch */
        TCGLabel *fs = gen_new_label();
        save_cpu_state(ctx, 0);

        switch (opc) {
        case OPC_BLEZALC: /* OPC_BGEZALC, OPC_BGEUC */
            if (rs == 0 && rt != 0) {
                /* OPC_BLEZALC */
                tcg_gen_brcondi_tl(tcg_invert_cond(TCG_COND_LE), t1, 0, fs);
            } else if (rs != 0 && rt != 0 && rs == rt) {
                /* OPC_BGEZALC */
                tcg_gen_brcondi_tl(tcg_invert_cond(TCG_COND_GE), t1, 0, fs);
            } else {
                /* OPC_BGEUC */
                tcg_gen_brcond_tl(tcg_invert_cond(TCG_COND_GEU), t0, t1, fs);
            }
            break;
        case OPC_BGTZALC: /* OPC_BLTZALC, OPC_BLTUC */
            if (rs == 0 && rt != 0) {
                /* OPC_BGTZALC */
                tcg_gen_brcondi_tl(tcg_invert_cond(TCG_COND_GT), t1, 0, fs);
            } else if (rs != 0 && rt != 0 && rs == rt) {
                /* OPC_BLTZALC */
                tcg_gen_brcondi_tl(tcg_invert_cond(TCG_COND_LT), t1, 0, fs);
            } else {
                /* OPC_BLTUC */
                tcg_gen_brcond_tl(tcg_invert_cond(TCG_COND_LTU), t0, t1, fs);
            }
            break;
        case OPC_BLEZC: /* OPC_BGEZC, OPC_BGEC */
            if (rs == 0 && rt != 0) {
                /* OPC_BLEZC */
                tcg_gen_brcondi_tl(tcg_invert_cond(TCG_COND_LE), t1, 0, fs);
            } else if (rs != 0 && rt != 0 && rs == rt) {
                /* OPC_BGEZC */
                tcg_gen_brcondi_tl(tcg_invert_cond(TCG_COND_GE), t1, 0, fs);
            } else {
                /* OPC_BGEC */
                tcg_gen_brcond_tl(tcg_invert_cond(TCG_COND_GE), t0, t1, fs);
            }
            break;
        case OPC_BGTZC: /* OPC_BLTZC, OPC_BLTC */
            if (rs == 0 && rt != 0) {
                /* OPC_BGTZC */
                tcg_gen_brcondi_tl(tcg_invert_cond(TCG_COND_GT), t1, 0, fs);
            } else if (rs != 0 && rt != 0 && rs == rt) {
                /* OPC_BLTZC */
                tcg_gen_brcondi_tl(tcg_invert_cond(TCG_COND_LT), t1, 0, fs);
            } else {
                /* OPC_BLTC */
                tcg_gen_brcond_tl(tcg_invert_cond(TCG_COND_LT), t0, t1, fs);
            }
            break;
        case OPC_BOVC: /* OPC_BEQZALC, OPC_BEQC */
        case OPC_BNVC: /* OPC_BNEZALC, OPC_BNEC */
            if (rs >= rt) {
                /* OPC_BOVC, OPC_BNVC */
                TCGv t2 = tcg_temp_new();
                TCGv t3 = tcg_temp_new();
                TCGv t4 = tcg_temp_new();
                TCGv input_overflow = tcg_temp_new();

                gen_load_gpr(t0, rs);
                gen_load_gpr(t1, rt);
                tcg_gen_ext32s_tl(t2, t0);
                tcg_gen_setcond_tl(TCG_COND_NE, input_overflow, t2, t0);
                tcg_gen_ext32s_tl(t3, t1);
                tcg_gen_setcond_tl(TCG_COND_NE, t4, t3, t1);
                tcg_gen_or_tl(input_overflow, input_overflow, t4);

                tcg_gen_add_tl(t4, t2, t3);
                tcg_gen_ext32s_tl(t4, t4);
                tcg_gen_xor_tl(t2, t2, t3);
                tcg_gen_xor_tl(t3, t4, t3);
                tcg_gen_andc_tl(t2, t3, t2);
                tcg_gen_setcondi_tl(TCG_COND_LT, t4, t2, 0);
                tcg_gen_or_tl(t4, t4, input_overflow);
                if (opc == OPC_BOVC) {
                    /* OPC_BOVC */
                    tcg_gen_brcondi_tl(tcg_invert_cond(TCG_COND_NE), t4, 0, fs);
                } else {
                    /* OPC_BNVC */
                    tcg_gen_brcondi_tl(tcg_invert_cond(TCG_COND_EQ), t4, 0, fs);
                }
                tcg_temp_free(input_overflow);
                tcg_temp_free(t4);
                tcg_temp_free(t3);
                tcg_temp_free(t2);
            } else if (rs < rt && rs == 0) {
                /* OPC_BEQZALC, OPC_BNEZALC */
                if (opc == OPC_BEQZALC) {
                    /* OPC_BEQZALC */
                    tcg_gen_brcondi_tl(tcg_invert_cond(TCG_COND_EQ), t1, 0, fs);
                } else {
                    /* OPC_BNEZALC */
                    tcg_gen_brcondi_tl(tcg_invert_cond(TCG_COND_NE), t1, 0, fs);
                }
            } else {
                /* OPC_BEQC, OPC_BNEC */
                if (opc == OPC_BEQC) {
                    /* OPC_BEQC */
                    tcg_gen_brcond_tl(tcg_invert_cond(TCG_COND_EQ), t0, t1, fs);
                } else {
                    /* OPC_BNEC */
                    tcg_gen_brcond_tl(tcg_invert_cond(TCG_COND_NE), t0, t1, fs);
                }
            }
            break;
        case OPC_BEQZC:
            tcg_gen_brcondi_tl(tcg_invert_cond(TCG_COND_EQ), t0, 0, fs);
            break;
        case OPC_BNEZC:
            tcg_gen_brcondi_tl(tcg_invert_cond(TCG_COND_NE), t0, 0, fs);
            break;
        default:
            MIPS_INVAL("Compact conditional branch/jump");
            gen_reserved_instruction(ctx);
            goto out;
        }
        // Bounds check against $pcc for conditional branches.
        gen_check_cond_branch_target(ctx, bcond, ctx->btarget);

        /* Generating branch here as compact branches don't have delay slot */
        gen_goto_tb(ctx, 1, ctx->btarget);
        gen_set_label(fs);

        ctx->hflags |= MIPS_HFLAG_FBNSLOT;
    }

out:
    tcg_temp_free(t0);
    tcg_temp_free(t1);
}

/* ISA extensions (ASEs) */
/* MIPS16 extension to MIPS32 */
#ifndef TARGET_CHERI

/* MIPS16 major opcodes */
enum {
  M16_OPC_ADDIUSP = 0x00,
  M16_OPC_ADDIUPC = 0x01,
  M16_OPC_B = 0x02,
  M16_OPC_JAL = 0x03,
  M16_OPC_BEQZ = 0x04,
  M16_OPC_BNEQZ = 0x05,
  M16_OPC_SHIFT = 0x06,
  M16_OPC_LD = 0x07,
  M16_OPC_RRIA = 0x08,
  M16_OPC_ADDIU8 = 0x09,
  M16_OPC_SLTI = 0x0a,
  M16_OPC_SLTIU = 0x0b,
  M16_OPC_I8 = 0x0c,
  M16_OPC_LI = 0x0d,
  M16_OPC_CMPI = 0x0e,
  M16_OPC_SD = 0x0f,
  M16_OPC_LB = 0x10,
  M16_OPC_LH = 0x11,
  M16_OPC_LWSP = 0x12,
  M16_OPC_LW = 0x13,
  M16_OPC_LBU = 0x14,
  M16_OPC_LHU = 0x15,
  M16_OPC_LWPC = 0x16,
  M16_OPC_LWU = 0x17,
  M16_OPC_SB = 0x18,
  M16_OPC_SH = 0x19,
  M16_OPC_SWSP = 0x1a,
  M16_OPC_SW = 0x1b,
  M16_OPC_RRR = 0x1c,
  M16_OPC_RR = 0x1d,
  M16_OPC_EXTEND = 0x1e,
  M16_OPC_I64 = 0x1f
};

/* I8 funct field */
enum {
  I8_BTEQZ = 0x0,
  I8_BTNEZ = 0x1,
  I8_SWRASP = 0x2,
  I8_ADJSP = 0x3,
  I8_SVRS = 0x4,
  I8_MOV32R = 0x5,
  I8_MOVR32 = 0x7
};

/* RRR f field */
enum {
  RRR_DADDU = 0x0,
  RRR_ADDU = 0x1,
  RRR_DSUBU = 0x2,
  RRR_SUBU = 0x3
};

/* RR funct field */
enum {
  RR_JR = 0x00,
  RR_SDBBP = 0x01,
  RR_SLT = 0x02,
  RR_SLTU = 0x03,
  RR_SLLV = 0x04,
  RR_BREAK = 0x05,
  RR_SRLV = 0x06,
  RR_SRAV = 0x07,
  RR_DSRL = 0x08,
  RR_CMP = 0x0a,
  RR_NEG = 0x0b,
  RR_AND = 0x0c,
  RR_OR = 0x0d,
  RR_XOR = 0x0e,
  RR_NOT = 0x0f,
  RR_MFHI = 0x10,
  RR_CNVT = 0x11,
  RR_MFLO = 0x12,
  RR_DSRA = 0x13,
  RR_DSLLV = 0x14,
  RR_DSRLV = 0x16,
  RR_DSRAV = 0x17,
  RR_MULT = 0x18,
  RR_MULTU = 0x19,
  RR_DIV = 0x1a,
  RR_DIVU = 0x1b,
  RR_DMULT = 0x1c,
  RR_DMULTU = 0x1d,
  RR_DDIV = 0x1e,
  RR_DDIVU = 0x1f
};

/* I64 funct field */
enum {
  I64_LDSP = 0x0,
  I64_SDSP = 0x1,
  I64_SDRASP = 0x2,
  I64_DADJSP = 0x3,
  I64_LDPC = 0x4,
  I64_DADDIU5 = 0x5,
  I64_DADDIUPC = 0x6,
  I64_DADDIUSP = 0x7
};

/* RR ry field for CNVT */
enum {
  RR_RY_CNVT_ZEB = 0x0,
  RR_RY_CNVT_ZEH = 0x1,
  RR_RY_CNVT_ZEW = 0x2,
  RR_RY_CNVT_SEB = 0x4,
  RR_RY_CNVT_SEH = 0x5,
  RR_RY_CNVT_SEW = 0x6,
};

static int xlat(int r)
{
  static int map[] = { 16, 17, 2, 3, 4, 5, 6, 7 };

  return map[r];
}

static void gen_mips16_save(DisasContext *ctx,
                            int xsregs, int aregs,
                            int do_ra, int do_s0, int do_s1,
                            int framesize)
{
    TCGv t0 = tcg_temp_new();
    TCGv t1 = tcg_temp_new();
    TCGv t2 = tcg_temp_new();
    int args, astatic;

    switch (aregs) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 11:
        args = 0;
        break;
    case 4:
    case 5:
    case 6:
    case 7:
        args = 1;
        break;
    case 8:
    case 9:
    case 10:
        args = 2;
        break;
    case 12:
    case 13:
        args = 3;
        break;
    case 14:
        args = 4;
        break;
    default:
        gen_reserved_instruction(ctx);
        return;
    }

    switch (args) {
    case 4:
        gen_base_offset_addr(ctx, t0, 29, 12);
        gen_load_gpr(t1, 7);
        tcg_gen_qemu_st_tl(t1, t0, ctx->mem_idx, MO_TEUL);
        /* Fall through */
    case 3:
        gen_base_offset_addr(ctx, t0, 29, 8);
        gen_load_gpr(t1, 6);
        tcg_gen_qemu_st_tl(t1, t0, ctx->mem_idx, MO_TEUL);
        /* Fall through */
    case 2:
        gen_base_offset_addr(ctx, t0, 29, 4);
        gen_load_gpr(t1, 5);
        tcg_gen_qemu_st_tl(t1, t0, ctx->mem_idx, MO_TEUL);
        /* Fall through */
    case 1:
        gen_base_offset_addr(ctx, t0, 29, 0);
        gen_load_gpr(t1, 4);
        tcg_gen_qemu_st_tl(t1, t0, ctx->mem_idx, MO_TEUL);
    }

    gen_load_gpr(t0, 29);

#define DECR_AND_STORE(reg) do {                                 \
        tcg_gen_movi_tl(t2, -4);                                 \
        gen_op_addr_add(ctx, t0, t0, t2);                        \
        gen_load_gpr(t1, reg);                                   \
        tcg_gen_qemu_st_tl(t1, t0, ctx->mem_idx, MO_TEUL);       \
        gen_log_instr_gpr_update(ctx, reg);                      \
    } while (0)

    if (do_ra) {
        DECR_AND_STORE(31);
    }

    switch (xsregs) {
    case 7:
        DECR_AND_STORE(30);
        /* Fall through */
    case 6:
        DECR_AND_STORE(23);
        /* Fall through */
    case 5:
        DECR_AND_STORE(22);
        /* Fall through */
    case 4:
        DECR_AND_STORE(21);
        /* Fall through */
    case 3:
        DECR_AND_STORE(20);
        /* Fall through */
    case 2:
        DECR_AND_STORE(19);
        /* Fall through */
    case 1:
        DECR_AND_STORE(18);
    }

    if (do_s1) {
        DECR_AND_STORE(17);
    }
    if (do_s0) {
        DECR_AND_STORE(16);
    }

    switch (aregs) {
    case 0:
    case 4:
    case 8:
    case 12:
    case 14:
        astatic = 0;
        break;
    case 1:
    case 5:
    case 9:
    case 13:
        astatic = 1;
        break;
    case 2:
    case 6:
    case 10:
        astatic = 2;
        break;
    case 3:
    case 7:
        astatic = 3;
        break;
    case 11:
        astatic = 4;
        break;
    default:
        gen_reserved_instruction(ctx);
        return;
    }

    if (astatic > 0) {
        DECR_AND_STORE(7);
        if (astatic > 1) {
            DECR_AND_STORE(6);
            if (astatic > 2) {
                DECR_AND_STORE(5);
                if (astatic > 3) {
                    DECR_AND_STORE(4);
                }
            }
        }
    }
#undef DECR_AND_STORE

    tcg_gen_movi_tl(t2, -framesize);
    gen_op_addr_add(ctx, cpu_gpr[29], cpu_gpr[29], t2);
    gen_log_instr_gpr_update(ctx, 29);
    tcg_temp_free(t0);
    tcg_temp_free(t1);
    tcg_temp_free(t2);
}

static void gen_mips16_restore(DisasContext *ctx,
                               int xsregs, int aregs,
                               int do_ra, int do_s0, int do_s1,
                               int framesize)
{
    int astatic;
    TCGv t0 = tcg_temp_new();
    TCGv t1 = tcg_temp_new();
    TCGv t2 = tcg_temp_new();

    tcg_gen_movi_tl(t2, framesize);
    gen_op_addr_add(ctx, t0, cpu_gpr[29], t2);

#define DECR_AND_LOAD(reg) do {                            \
        tcg_gen_movi_tl(t2, -4);                           \
        gen_op_addr_add(ctx, t0, t0, t2);                  \
        tcg_gen_qemu_ld_tl(t1, t0, ctx->mem_idx, MO_TESL); \
        gen_store_gpr(t1, reg);                            \
    } while (0)

    if (do_ra) {
        DECR_AND_LOAD(31);
    }

    switch (xsregs) {
    case 7:
        DECR_AND_LOAD(30);
        /* Fall through */
    case 6:
        DECR_AND_LOAD(23);
        /* Fall through */
    case 5:
        DECR_AND_LOAD(22);
        /* Fall through */
    case 4:
        DECR_AND_LOAD(21);
        /* Fall through */
    case 3:
        DECR_AND_LOAD(20);
        /* Fall through */
    case 2:
        DECR_AND_LOAD(19);
        /* Fall through */
    case 1:
        DECR_AND_LOAD(18);
    }

    if (do_s1) {
        DECR_AND_LOAD(17);
    }
    if (do_s0) {
        DECR_AND_LOAD(16);
    }

    switch (aregs) {
    case 0:
    case 4:
    case 8:
    case 12:
    case 14:
        astatic = 0;
        break;
    case 1:
    case 5:
    case 9:
    case 13:
        astatic = 1;
        break;
    case 2:
    case 6:
    case 10:
        astatic = 2;
        break;
    case 3:
    case 7:
        astatic = 3;
        break;
    case 11:
        astatic = 4;
        break;
    default:
        gen_reserved_instruction(ctx);
        return;
    }

    if (astatic > 0) {
        DECR_AND_LOAD(7);
        if (astatic > 1) {
            DECR_AND_LOAD(6);
            if (astatic > 2) {
                DECR_AND_LOAD(5);
                if (astatic > 3) {
                    DECR_AND_LOAD(4);
                }
            }
        }
    }
#undef DECR_AND_LOAD

    tcg_gen_movi_tl(t2, framesize);
    gen_op_addr_add(ctx, cpu_gpr[29], cpu_gpr[29], t2);
    gen_log_instr_gpr_update(ctx, 29);
    tcg_temp_free(t0);
    tcg_temp_free(t1);
    tcg_temp_free(t2);
}

#endif // !TARGET_CHERI

static void gen_addiupc(DisasContext *ctx, int rx, int imm,
                        int is_64_bit, int extended)
{
    TCGv t0;

    if (extended && (ctx->hflags & MIPS_HFLAG_BMASK)) {
        gen_reserved_instruction(ctx);
        return;
    }

    t0 = tcg_temp_new();

    tcg_gen_movi_tl(t0, pc_relative_pc(ctx));
    tcg_gen_addi_tl(cpu_gpr[rx], t0, imm);
    if (!is_64_bit) {
        tcg_gen_ext32s_tl(cpu_gpr[rx], cpu_gpr[rx]);
    }
    gen_log_instr_gpr_update(ctx, rx);

    tcg_temp_free(t0);
}

static void gen_cache_operation(DisasContext *ctx, uint32_t op, int base,
                                int16_t offset)
{
    TCGv_i32 t0 = tcg_const_i32(op);
    TCGv t1 = tcg_temp_new();
    gen_base_offset_addr(ctx, t1, base, offset);
    gen_helper_cache(cpu_env, t1, t0);
    tcg_temp_free(t1);
    tcg_temp_free_i32(t0);
}

#ifndef TARGET_CHERI // No MIPS16 with TARGET_CHERI

#if defined(TARGET_MIPS64)
static void decode_i64_mips16(DisasContext *ctx,
                              int ry, int funct, int16_t offset,
                              int extended)
{
    switch (funct) {
    case I64_LDSP:
        check_insn(ctx, ISA_MIPS3);
        check_mips_64(ctx);
        offset = extended ? offset : offset << 3;
        gen_ld(ctx, OPC_LD, ry, 29, offset);
        break;
    case I64_SDSP:
        check_insn(ctx, ISA_MIPS3);
        check_mips_64(ctx);
        offset = extended ? offset : offset << 3;
        gen_st(ctx, OPC_SD, ry, 29, offset);
        break;
    case I64_SDRASP:
        check_insn(ctx, ISA_MIPS3);
        check_mips_64(ctx);
        offset = extended ? offset : (ctx->opcode & 0xff) << 3;
        gen_st(ctx, OPC_SD, 31, 29, offset);
        break;
    case I64_DADJSP:
        check_insn(ctx, ISA_MIPS3);
        check_mips_64(ctx);
        offset = extended ? offset : ((int8_t)ctx->opcode) << 3;
        gen_arith_imm(ctx, OPC_DADDIU, 29, 29, offset);
        break;
    case I64_LDPC:
        check_insn(ctx, ISA_MIPS3);
        check_mips_64(ctx);
        if (extended && (ctx->hflags & MIPS_HFLAG_BMASK)) {
            gen_reserved_instruction(ctx);
        } else {
            offset = extended ? offset : offset << 3;
            gen_ld(ctx, OPC_LDPC, ry, 0, offset);
        }
        break;
    case I64_DADDIU5:
        check_insn(ctx, ISA_MIPS3);
        check_mips_64(ctx);
        offset = extended ? offset : ((int8_t)(offset << 3)) >> 3;
        gen_arith_imm(ctx, OPC_DADDIU, ry, ry, offset);
        break;
    case I64_DADDIUPC:
        check_insn(ctx, ISA_MIPS3);
        check_mips_64(ctx);
        offset = extended ? offset : offset << 2;
        gen_addiupc(ctx, ry, offset, 1, extended);
        break;
    case I64_DADDIUSP:
        check_insn(ctx, ISA_MIPS3);
        check_mips_64(ctx);
        offset = extended ? offset : offset << 2;
        gen_arith_imm(ctx, OPC_DADDIU, ry, 29, offset);
        break;
    }
}
#endif

static int decode_extended_mips16_opc(CPUMIPSState *env, DisasContext *ctx)
{
    int extend = translator_lduw(env, ctx->base.pc_next + 2);
    int op, rx, ry, funct, sa;
    int16_t imm, offset;

    ctx->opcode = (ctx->opcode << 16) | extend;
    op = (ctx->opcode >> 11) & 0x1f;
    sa = (ctx->opcode >> 22) & 0x1f;
    funct = (ctx->opcode >> 8) & 0x7;
    rx = xlat((ctx->opcode >> 8) & 0x7);
    ry = xlat((ctx->opcode >> 5) & 0x7);
    offset = imm = (int16_t) (((ctx->opcode >> 16) & 0x1f) << 11
                              | ((ctx->opcode >> 21) & 0x3f) << 5
                              | (ctx->opcode & 0x1f));

    /*
     * The extended opcodes cleverly reuse the opcodes from their 16-bit
     * counterparts.
     */
    switch (op) {
    case M16_OPC_ADDIUSP:
        gen_arith_imm(ctx, OPC_ADDIU, rx, 29, imm);
        break;
    case M16_OPC_ADDIUPC:
        gen_addiupc(ctx, rx, imm, 0, 1);
        break;
    case M16_OPC_B:
        gen_compute_branch(ctx, OPC_BEQ, 4, 0, 0, offset << 1, 0);
        /* No delay slot, so just process as a normal instruction */
        break;
    case M16_OPC_BEQZ:
        gen_compute_branch(ctx, OPC_BEQ, 4, rx, 0, offset << 1, 0);
        /* No delay slot, so just process as a normal instruction */
        break;
    case M16_OPC_BNEQZ:
        gen_compute_branch(ctx, OPC_BNE, 4, rx, 0, offset << 1, 0);
        /* No delay slot, so just process as a normal instruction */
        break;
    case M16_OPC_SHIFT:
        switch (ctx->opcode & 0x3) {
        case 0x0:
            gen_shift_imm(ctx, OPC_SLL, rx, ry, sa);
            break;
        case 0x1:
#if defined(TARGET_MIPS64)
            check_mips_64(ctx);
            gen_shift_imm(ctx, OPC_DSLL, rx, ry, sa);
#else
            gen_reserved_instruction(ctx);
#endif
            break;
        case 0x2:
            gen_shift_imm(ctx, OPC_SRL, rx, ry, sa);
            break;
        case 0x3:
            gen_shift_imm(ctx, OPC_SRA, rx, ry, sa);
            break;
        }
        break;
#if defined(TARGET_MIPS64)
    case M16_OPC_LD:
        check_insn(ctx, ISA_MIPS3);
        check_mips_64(ctx);
        gen_ld(ctx, OPC_LD, ry, rx, offset);
        break;
#endif
    case M16_OPC_RRIA:
        imm = ctx->opcode & 0xf;
        imm = imm | ((ctx->opcode >> 20) & 0x7f) << 4;
        imm = imm | ((ctx->opcode >> 16) & 0xf) << 11;
        imm = (int16_t) (imm << 1) >> 1;
        if ((ctx->opcode >> 4) & 0x1) {
#if defined(TARGET_MIPS64)
            check_mips_64(ctx);
            gen_arith_imm(ctx, OPC_DADDIU, ry, rx, imm);
#else
            gen_reserved_instruction(ctx);
#endif
        } else {
            gen_arith_imm(ctx, OPC_ADDIU, ry, rx, imm);
        }
        break;
    case M16_OPC_ADDIU8:
        gen_arith_imm(ctx, OPC_ADDIU, rx, rx, imm);
        break;
    case M16_OPC_SLTI:
        gen_slt_imm(ctx, OPC_SLTI, 24, rx, imm);
        break;
    case M16_OPC_SLTIU:
        gen_slt_imm(ctx, OPC_SLTIU, 24, rx, imm);
        break;
    case M16_OPC_I8:
        switch (funct) {
        case I8_BTEQZ:
            gen_compute_branch(ctx, OPC_BEQ, 4, 24, 0, offset << 1, 0);
            break;
        case I8_BTNEZ:
            gen_compute_branch(ctx, OPC_BNE, 4, 24, 0, offset << 1, 0);
            break;
        case I8_SWRASP:
            gen_st(ctx, OPC_SW, 31, 29, imm);
            break;
        case I8_ADJSP:
            gen_arith_imm(ctx, OPC_ADDIU, 29, 29, imm);
            break;
        case I8_SVRS:
            check_insn(ctx, ISA_MIPS_R1);
            {
                int xsregs = (ctx->opcode >> 24) & 0x7;
                int aregs = (ctx->opcode >> 16) & 0xf;
                int do_ra = (ctx->opcode >> 6) & 0x1;
                int do_s0 = (ctx->opcode >> 5) & 0x1;
                int do_s1 = (ctx->opcode >> 4) & 0x1;
                int framesize = (((ctx->opcode >> 20) & 0xf) << 4
                                 | (ctx->opcode & 0xf)) << 3;

                if (ctx->opcode & (1 << 7)) {
                    gen_mips16_save(ctx, xsregs, aregs,
                                    do_ra, do_s0, do_s1,
                                    framesize);
                } else {
                    gen_mips16_restore(ctx, xsregs, aregs,
                                       do_ra, do_s0, do_s1,
                                       framesize);
                }
            }
            break;
        default:
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case M16_OPC_LI:
        tcg_gen_movi_tl(cpu_gpr[rx], (uint16_t) imm);
        gen_log_instr_gpr_update(ctx, rx);
        break;
    case M16_OPC_CMPI:
        tcg_gen_xori_tl(cpu_gpr[24], cpu_gpr[rx], (uint16_t) imm);
        gen_log_instr_gpr_update(ctx, 24);
        break;
#if defined(TARGET_MIPS64)
    case M16_OPC_SD:
        check_insn(ctx, ISA_MIPS3);
        check_mips_64(ctx);
        gen_st(ctx, OPC_SD, ry, rx, offset);
        break;
#endif
    case M16_OPC_LB:
        gen_ld(ctx, OPC_LB, ry, rx, offset);
        break;
    case M16_OPC_LH:
        gen_ld(ctx, OPC_LH, ry, rx, offset);
        break;
    case M16_OPC_LWSP:
        gen_ld(ctx, OPC_LW, rx, 29, offset);
        break;
    case M16_OPC_LW:
        gen_ld(ctx, OPC_LW, ry, rx, offset);
        break;
    case M16_OPC_LBU:
        gen_ld(ctx, OPC_LBU, ry, rx, offset);
        break;
    case M16_OPC_LHU:
        gen_ld(ctx, OPC_LHU, ry, rx, offset);
        break;
    case M16_OPC_LWPC:
        gen_ld(ctx, OPC_LWPC, rx, 0, offset);
        break;
#if defined(TARGET_MIPS64)
    case M16_OPC_LWU:
        check_insn(ctx, ISA_MIPS3);
        check_mips_64(ctx);
        gen_ld(ctx, OPC_LWU, ry, rx, offset);
        break;
#endif
    case M16_OPC_SB:
        gen_st(ctx, OPC_SB, ry, rx, offset);
        break;
    case M16_OPC_SH:
        gen_st(ctx, OPC_SH, ry, rx, offset);
        break;
    case M16_OPC_SWSP:
        gen_st(ctx, OPC_SW, rx, 29, offset);
        break;
    case M16_OPC_SW:
        gen_st(ctx, OPC_SW, ry, rx, offset);
        break;
#if defined(TARGET_MIPS64)
    case M16_OPC_I64:
        decode_i64_mips16(ctx, ry, funct, offset, 1);
        break;
#endif
    default:
        gen_reserved_instruction(ctx);
        break;
    }

    return 4;
}
#endif // !TARGET_CHERI

static inline bool is_uhi(int sdbbp_code)
{
#ifdef CONFIG_USER_ONLY
    return false;
#else
    return semihosting_enabled() && sdbbp_code == 1;
#endif
}

#ifdef CONFIG_USER_ONLY
/* The above should dead-code away any calls to this..*/
static inline void gen_helper_do_semihosting(void *env)
{
    g_assert_not_reached();
}
#endif

#ifndef TARGET_CHERI /* MIPS16 not supported */

static int decode_mips16_opc(CPUMIPSState *env, DisasContext *ctx)
{
    int rx, ry;
    int sa;
    int op, cnvt_op, op1, offset;
    int funct;
    int n_bytes;

    op = (ctx->opcode >> 11) & 0x1f;
    sa = (ctx->opcode >> 2) & 0x7;
    sa = sa == 0 ? 8 : sa;
    rx = xlat((ctx->opcode >> 8) & 0x7);
    cnvt_op = (ctx->opcode >> 5) & 0x7;
    ry = xlat((ctx->opcode >> 5) & 0x7);
    op1 = offset = ctx->opcode & 0x1f;

    n_bytes = 2;

    switch (op) {
    case M16_OPC_ADDIUSP:
        {
            int16_t imm = ((uint8_t) ctx->opcode) << 2;

            gen_arith_imm(ctx, OPC_ADDIU, rx, 29, imm);
        }
        break;
    case M16_OPC_ADDIUPC:
        gen_addiupc(ctx, rx, ((uint8_t) ctx->opcode) << 2, 0, 0);
        break;
    case M16_OPC_B:
        offset = (ctx->opcode & 0x7ff) << 1;
        offset = (int16_t)(offset << 4) >> 4;
        gen_compute_branch(ctx, OPC_BEQ, 2, 0, 0, offset, 0);
        /* No delay slot, so just process as a normal instruction */
        break;
    case M16_OPC_JAL:
        offset = translator_lduw(env, ctx->base.pc_next + 2);
        offset = (((ctx->opcode & 0x1f) << 21)
                  | ((ctx->opcode >> 5) & 0x1f) << 16
                  | offset) << 2;
        op = ((ctx->opcode >> 10) & 0x1) ? OPC_JALX : OPC_JAL;
        gen_compute_branch(ctx, op, 4, rx, ry, offset, 2);
        n_bytes = 4;
        break;
    case M16_OPC_BEQZ:
        gen_compute_branch(ctx, OPC_BEQ, 2, rx, 0,
                           ((int8_t)ctx->opcode) << 1, 0);
        /* No delay slot, so just process as a normal instruction */
        break;
    case M16_OPC_BNEQZ:
        gen_compute_branch(ctx, OPC_BNE, 2, rx, 0,
                           ((int8_t)ctx->opcode) << 1, 0);
        /* No delay slot, so just process as a normal instruction */
        break;
    case M16_OPC_SHIFT:
        switch (ctx->opcode & 0x3) {
        case 0x0:
            gen_shift_imm(ctx, OPC_SLL, rx, ry, sa);
            break;
        case 0x1:
#if defined(TARGET_MIPS64)
            check_insn(ctx, ISA_MIPS3);
            check_mips_64(ctx);
            gen_shift_imm(ctx, OPC_DSLL, rx, ry, sa);
#else
            gen_reserved_instruction(ctx);
#endif
            break;
        case 0x2:
            gen_shift_imm(ctx, OPC_SRL, rx, ry, sa);
            break;
        case 0x3:
            gen_shift_imm(ctx, OPC_SRA, rx, ry, sa);
            break;
        }
        break;
#if defined(TARGET_MIPS64)
    case M16_OPC_LD:
        check_insn(ctx, ISA_MIPS3);
        check_mips_64(ctx);
        gen_ld(ctx, OPC_LD, ry, rx, offset << 3);
        break;
#endif
    case M16_OPC_RRIA:
        {
            int16_t imm = (int8_t)((ctx->opcode & 0xf) << 4) >> 4;

            if ((ctx->opcode >> 4) & 1) {
#if defined(TARGET_MIPS64)
                check_insn(ctx, ISA_MIPS3);
                check_mips_64(ctx);
                gen_arith_imm(ctx, OPC_DADDIU, ry, rx, imm);
#else
                gen_reserved_instruction(ctx);
#endif
            } else {
                gen_arith_imm(ctx, OPC_ADDIU, ry, rx, imm);
            }
        }
        break;
    case M16_OPC_ADDIU8:
        {
            int16_t imm = (int8_t) ctx->opcode;

            gen_arith_imm(ctx, OPC_ADDIU, rx, rx, imm);
        }
        break;
    case M16_OPC_SLTI:
        {
            int16_t imm = (uint8_t) ctx->opcode;
            gen_slt_imm(ctx, OPC_SLTI, 24, rx, imm);
        }
        break;
    case M16_OPC_SLTIU:
        {
            int16_t imm = (uint8_t) ctx->opcode;
            gen_slt_imm(ctx, OPC_SLTIU, 24, rx, imm);
        }
        break;
    case M16_OPC_I8:
        {
            int reg32;

            funct = (ctx->opcode >> 8) & 0x7;
            switch (funct) {
            case I8_BTEQZ:
                gen_compute_branch(ctx, OPC_BEQ, 2, 24, 0,
                                   ((int8_t)ctx->opcode) << 1, 0);
                break;
            case I8_BTNEZ:
                gen_compute_branch(ctx, OPC_BNE, 2, 24, 0,
                                   ((int8_t)ctx->opcode) << 1, 0);
                break;
            case I8_SWRASP:
                gen_st(ctx, OPC_SW, 31, 29, (ctx->opcode & 0xff) << 2);
                break;
            case I8_ADJSP:
                gen_arith_imm(ctx, OPC_ADDIU, 29, 29,
                              ((int8_t)ctx->opcode) << 3);
                break;
            case I8_SVRS:
                check_insn(ctx, ISA_MIPS_R1);
                {
                    int do_ra = ctx->opcode & (1 << 6);
                    int do_s0 = ctx->opcode & (1 << 5);
                    int do_s1 = ctx->opcode & (1 << 4);
                    int framesize = ctx->opcode & 0xf;

                    if (framesize == 0) {
                        framesize = 128;
                    } else {
                        framesize = framesize << 3;
                    }

                    if (ctx->opcode & (1 << 7)) {
                        gen_mips16_save(ctx, 0, 0,
                                        do_ra, do_s0, do_s1, framesize);
                    } else {
                        gen_mips16_restore(ctx, 0, 0,
                                           do_ra, do_s0, do_s1, framesize);
                    }
                }
                break;
            case I8_MOV32R:
                {
                    int rz = xlat(ctx->opcode & 0x7);

                    reg32 = (((ctx->opcode >> 3) & 0x3) << 3) |
                        ((ctx->opcode >> 5) & 0x7);
                    gen_arith(ctx, OPC_ADDU, reg32, rz, 0);
                }
                break;
            case I8_MOVR32:
                reg32 = ctx->opcode & 0x1f;
                gen_arith(ctx, OPC_ADDU, ry, reg32, 0);
                break;
            default:
                gen_reserved_instruction(ctx);
                break;
            }
        }
        break;
    case M16_OPC_LI:
        {
            int16_t imm = (uint8_t) ctx->opcode;

            gen_arith_imm(ctx, OPC_ADDIU, rx, 0, imm);
        }
        break;
    case M16_OPC_CMPI:
        {
            int16_t imm = (uint8_t) ctx->opcode;
            gen_logic_imm(ctx, OPC_XORI, 24, rx, imm);
        }
        break;
#if defined(TARGET_MIPS64)
    case M16_OPC_SD:
        check_insn(ctx, ISA_MIPS3);
        check_mips_64(ctx);
        gen_st(ctx, OPC_SD, ry, rx, offset << 3);
        break;
#endif
    case M16_OPC_LB:
        gen_ld(ctx, OPC_LB, ry, rx, offset);
        break;
    case M16_OPC_LH:
        gen_ld(ctx, OPC_LH, ry, rx, offset << 1);
        break;
    case M16_OPC_LWSP:
        gen_ld(ctx, OPC_LW, rx, 29, ((uint8_t)ctx->opcode) << 2);
        break;
    case M16_OPC_LW:
        gen_ld(ctx, OPC_LW, ry, rx, offset << 2);
        break;
    case M16_OPC_LBU:
        gen_ld(ctx, OPC_LBU, ry, rx, offset);
        break;
    case M16_OPC_LHU:
        gen_ld(ctx, OPC_LHU, ry, rx, offset << 1);
        break;
    case M16_OPC_LWPC:
        gen_ld(ctx, OPC_LWPC, rx, 0, ((uint8_t)ctx->opcode) << 2);
        break;
#if defined(TARGET_MIPS64)
    case M16_OPC_LWU:
        check_insn(ctx, ISA_MIPS3);
        check_mips_64(ctx);
        gen_ld(ctx, OPC_LWU, ry, rx, offset << 2);
        break;
#endif
    case M16_OPC_SB:
        gen_st(ctx, OPC_SB, ry, rx, offset);
        break;
    case M16_OPC_SH:
        gen_st(ctx, OPC_SH, ry, rx, offset << 1);
        break;
    case M16_OPC_SWSP:
        gen_st(ctx, OPC_SW, rx, 29, ((uint8_t)ctx->opcode) << 2);
        break;
    case M16_OPC_SW:
        gen_st(ctx, OPC_SW, ry, rx, offset << 2);
        break;
    case M16_OPC_RRR:
        {
            int rz = xlat((ctx->opcode >> 2) & 0x7);
            int mips32_op;

            switch (ctx->opcode & 0x3) {
            case RRR_ADDU:
                mips32_op = OPC_ADDU;
                break;
            case RRR_SUBU:
                mips32_op = OPC_SUBU;
                break;
#if defined(TARGET_MIPS64)
            case RRR_DADDU:
                mips32_op = OPC_DADDU;
                check_insn(ctx, ISA_MIPS3);
                check_mips_64(ctx);
                break;
            case RRR_DSUBU:
                mips32_op = OPC_DSUBU;
                check_insn(ctx, ISA_MIPS3);
                check_mips_64(ctx);
                break;
#endif
            default:
                gen_reserved_instruction(ctx);
                goto done;
            }

            gen_arith(ctx, mips32_op, rz, rx, ry);
        done:
            ;
        }
        break;
    case M16_OPC_RR:
        switch (op1) {
        case RR_JR:
            {
                int nd = (ctx->opcode >> 7) & 0x1;
                int link = (ctx->opcode >> 6) & 0x1;
                int ra = (ctx->opcode >> 5) & 0x1;

                if (nd) {
                    check_insn(ctx, ISA_MIPS_R1);
                }

                if (link) {
                    op = OPC_JALR;
                } else {
                    op = OPC_JR;
                }

                gen_compute_branch(ctx, op, 2, ra ? 31 : rx, 31, 0,
                                   (nd ? 0 : 2));
            }
            break;
        case RR_SDBBP:
            if (is_uhi(extract32(ctx->opcode, 5, 6))) {
                gen_helper_do_semihosting(cpu_env);
            } else {
                /*
                 * XXX: not clear which exception should be raised
                 *      when in debug mode...
                 */
                check_insn(ctx, ISA_MIPS_R1);
                generate_exception_end(ctx, EXCP_DBp);
            }
            break;
        case RR_SLT:
            gen_slt(ctx, OPC_SLT, 24, rx, ry);
            break;
        case RR_SLTU:
            gen_slt(ctx, OPC_SLTU, 24, rx, ry);
            break;
        case RR_BREAK:
            generate_exception_end(ctx, EXCP_BREAK);
            break;
        case RR_SLLV:
            gen_shift(ctx, OPC_SLLV, ry, rx, ry);
            break;
        case RR_SRLV:
            gen_shift(ctx, OPC_SRLV, ry, rx, ry);
            break;
        case RR_SRAV:
            gen_shift(ctx, OPC_SRAV, ry, rx, ry);
            break;
#if defined(TARGET_MIPS64)
        case RR_DSRL:
            check_insn(ctx, ISA_MIPS3);
            check_mips_64(ctx);
            gen_shift_imm(ctx, OPC_DSRL, ry, ry, sa);
            break;
#endif
        case RR_CMP:
            gen_logic(ctx, OPC_XOR, 24, rx, ry);
            break;
        case RR_NEG:
            gen_arith(ctx, OPC_SUBU, rx, 0, ry);
            break;
        case RR_AND:
            gen_logic(ctx, OPC_AND, rx, rx, ry);
            break;
        case RR_OR:
            gen_logic(ctx, OPC_OR, rx, rx, ry);
            break;
        case RR_XOR:
            gen_logic(ctx, OPC_XOR, rx, rx, ry);
            break;
        case RR_NOT:
            gen_logic(ctx, OPC_NOR, rx, ry, 0);
            break;
        case RR_MFHI:
            gen_HILO(ctx, OPC_MFHI, 0, rx);
            break;
        case RR_CNVT:
            check_insn(ctx, ISA_MIPS_R1);
            switch (cnvt_op) {
            case RR_RY_CNVT_ZEB:
                tcg_gen_ext8u_tl(cpu_gpr[rx], cpu_gpr[rx]);
                break;
            case RR_RY_CNVT_ZEH:
                tcg_gen_ext16u_tl(cpu_gpr[rx], cpu_gpr[rx]);
                break;
            case RR_RY_CNVT_SEB:
                tcg_gen_ext8s_tl(cpu_gpr[rx], cpu_gpr[rx]);
                break;
            case RR_RY_CNVT_SEH:
                tcg_gen_ext16s_tl(cpu_gpr[rx], cpu_gpr[rx]);
                break;
#if defined(TARGET_MIPS64)
            case RR_RY_CNVT_ZEW:
                check_insn(ctx, ISA_MIPS_R1);
                check_mips_64(ctx);
                tcg_gen_ext32u_tl(cpu_gpr[rx], cpu_gpr[rx]);
                break;
            case RR_RY_CNVT_SEW:
                check_insn(ctx, ISA_MIPS_R1);
                check_mips_64(ctx);
                tcg_gen_ext32s_tl(cpu_gpr[rx], cpu_gpr[rx]);
                break;
#endif
            default:
                gen_reserved_instruction(ctx);
                break;
            }
            gen_log_instr_gpr_update(ctx, rx);
            break;
        case RR_MFLO:
            gen_HILO(ctx, OPC_MFLO, 0, rx);
            break;
#if defined(TARGET_MIPS64)
        case RR_DSRA:
            check_insn(ctx, ISA_MIPS3);
            check_mips_64(ctx);
            gen_shift_imm(ctx, OPC_DSRA, ry, ry, sa);
            break;
        case RR_DSLLV:
            check_insn(ctx, ISA_MIPS3);
            check_mips_64(ctx);
            gen_shift(ctx, OPC_DSLLV, ry, rx, ry);
            break;
        case RR_DSRLV:
            check_insn(ctx, ISA_MIPS3);
            check_mips_64(ctx);
            gen_shift(ctx, OPC_DSRLV, ry, rx, ry);
            break;
        case RR_DSRAV:
            check_insn(ctx, ISA_MIPS3);
            check_mips_64(ctx);
            gen_shift(ctx, OPC_DSRAV, ry, rx, ry);
            break;
#endif
        case RR_MULT:
            gen_muldiv(ctx, OPC_MULT, 0, rx, ry);
            break;
        case RR_MULTU:
            gen_muldiv(ctx, OPC_MULTU, 0, rx, ry);
            break;
        case RR_DIV:
            gen_muldiv(ctx, OPC_DIV, 0, rx, ry);
            break;
        case RR_DIVU:
            gen_muldiv(ctx, OPC_DIVU, 0, rx, ry);
            break;
#if defined(TARGET_MIPS64)
        case RR_DMULT:
            check_insn(ctx, ISA_MIPS3);
            check_mips_64(ctx);
            gen_muldiv(ctx, OPC_DMULT, 0, rx, ry);
            break;
        case RR_DMULTU:
            check_insn(ctx, ISA_MIPS3);
            check_mips_64(ctx);
            gen_muldiv(ctx, OPC_DMULTU, 0, rx, ry);
            break;
        case RR_DDIV:
            check_insn(ctx, ISA_MIPS3);
            check_mips_64(ctx);
            gen_muldiv(ctx, OPC_DDIV, 0, rx, ry);
            break;
        case RR_DDIVU:
            check_insn(ctx, ISA_MIPS3);
            check_mips_64(ctx);
            gen_muldiv(ctx, OPC_DDIVU, 0, rx, ry);
            break;
#endif
        default:
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case M16_OPC_EXTEND:
#ifdef TARGET_CHERI
        generate_exception_end(ctx, EXCP_RI); // Not supported, needs more changes
#else
        decode_extended_mips16_opc(env, ctx);
        n_bytes = 4;
#endif
        break;
#if defined(TARGET_MIPS64)
    case M16_OPC_I64:
        funct = (ctx->opcode >> 8) & 0x7;
        decode_i64_mips16(ctx, ry, funct, offset, 0);
        break;
#endif
    default:
        gen_reserved_instruction(ctx);
        break;
    }

    return n_bytes;
}

#endif
/* microMIPS extension to MIPS32/MIPS64 */

/*
 * microMIPS32/microMIPS64 major opcodes
 *
 * 1. MIPS Architecture for Programmers Volume II-B:
 *      The microMIPS32 Instruction Set (Revision 3.05)
 *
 *    Table 6.2 microMIPS32 Encoding of Major Opcode Field
 *
 * 2. MIPS Architecture For Programmers Volume II-A:
 *      The MIPS64 Instruction Set (Revision 3.51)
 */

enum {
    POOL32A = 0x00,
    POOL16A = 0x01,
    LBU16 = 0x02,
    MOVE16 = 0x03,
    ADDI32 = 0x04,
    R6_LUI = 0x04,
    AUI = 0x04,
    LBU32 = 0x05,
    SB32 = 0x06,
    LB32 = 0x07,

    POOL32B = 0x08,
    POOL16B = 0x09,
    LHU16 = 0x0a,
    ANDI16 = 0x0b,
    ADDIU32 = 0x0c,
    LHU32 = 0x0d,
    SH32 = 0x0e,
    LH32 = 0x0f,

    POOL32I = 0x10,
    POOL16C = 0x11,
    LWSP16 = 0x12,
    POOL16D = 0x13,
    ORI32 = 0x14,
    POOL32F = 0x15,
    POOL32S = 0x16,  /* MIPS64 */
    DADDIU32 = 0x17, /* MIPS64 */

    POOL32C = 0x18,
    LWGP16 = 0x19,
    LW16 = 0x1a,
    POOL16E = 0x1b,
    XORI32 = 0x1c,
    JALS32 = 0x1d,
    BOVC = 0x1d,
    BEQC = 0x1d,
    BEQZALC = 0x1d,
    ADDIUPC = 0x1e,
    PCREL = 0x1e,
    BNVC = 0x1f,
    BNEC = 0x1f,
    BNEZALC = 0x1f,

    R6_BEQZC = 0x20,
    JIC = 0x20,
    POOL16F = 0x21,
    SB16 = 0x22,
    BEQZ16 = 0x23,
    BEQZC16 = 0x23,
    SLTI32 = 0x24,
    BEQ32 = 0x25,
    BC = 0x25,
    SWC132 = 0x26,
    LWC132 = 0x27,

    /* 0x29 is reserved */
    RES_29 = 0x29,
    R6_BNEZC = 0x28,
    JIALC = 0x28,
    SH16 = 0x2a,
    BNEZ16 = 0x2b,
    BNEZC16 = 0x2b,
    SLTIU32 = 0x2c,
    BNE32 = 0x2d,
    BALC = 0x2d,
    SDC132 = 0x2e,
    LDC132 = 0x2f,

    /* 0x31 is reserved */
    RES_31 = 0x31,
    BLEZALC = 0x30,
    BGEZALC = 0x30,
    BGEUC = 0x30,
    SWSP16 = 0x32,
    B16 = 0x33,
    BC16 = 0x33,
    ANDI32 = 0x34,
    J32 = 0x35,
    BGTZC = 0x35,
    BLTZC = 0x35,
    BLTC = 0x35,
    SD32 = 0x36, /* MIPS64 */
    LD32 = 0x37, /* MIPS64 */

    /* 0x39 is reserved */
    RES_39 = 0x39,
    BGTZALC = 0x38,
    BLTZALC = 0x38,
    BLTUC = 0x38,
    SW16 = 0x3a,
    LI16 = 0x3b,
    JALX32 = 0x3c,
    JAL32 = 0x3d,
    BLEZC = 0x3d,
    BGEZC = 0x3d,
    BGEC = 0x3d,
    SW32 = 0x3e,
    LW32 = 0x3f
};

/* PCREL Instructions perform PC-Relative address calculation. bits 20..16 */
enum {
    ADDIUPC_00 = 0x00,
    ADDIUPC_01 = 0x01,
    ADDIUPC_02 = 0x02,
    ADDIUPC_03 = 0x03,
    ADDIUPC_04 = 0x04,
    ADDIUPC_05 = 0x05,
    ADDIUPC_06 = 0x06,
    ADDIUPC_07 = 0x07,
    AUIPC = 0x1e,
    ALUIPC = 0x1f,
    LWPC_08 = 0x08,
    LWPC_09 = 0x09,
    LWPC_0A = 0x0A,
    LWPC_0B = 0x0B,
    LWPC_0C = 0x0C,
    LWPC_0D = 0x0D,
    LWPC_0E = 0x0E,
    LWPC_0F = 0x0F,
};

/* POOL32A encoding of minor opcode field */

enum {
    /*
     * These opcodes are distinguished only by bits 9..6; those bits are
     * what are recorded below.
     */
    SLL32 = 0x0,
    SRL32 = 0x1,
    SRA = 0x2,
    ROTR = 0x3,
    SELEQZ = 0x5,
    SELNEZ = 0x6,
    R6_RDHWR = 0x7,

    SLLV = 0x0,
    SRLV = 0x1,
    SRAV = 0x2,
    ROTRV = 0x3,
    ADD = 0x4,
    ADDU32 = 0x5,
    SUB = 0x6,
    SUBU32 = 0x7,
    MUL = 0x8,
    AND = 0x9,
    OR32 = 0xa,
    NOR = 0xb,
    XOR32 = 0xc,
    SLT = 0xd,
    SLTU = 0xe,

    MOVN = 0x0,
    R6_MUL  = 0x0,
    MOVZ = 0x1,
    MUH  = 0x1,
    MULU = 0x2,
    MUHU = 0x3,
    LWXS = 0x4,
    R6_DIV  = 0x4,
    MOD  = 0x5,
    R6_DIVU = 0x6,
    MODU = 0x7,

    /* The following can be distinguished by their lower 6 bits. */
    BREAK32 = 0x07,
    INS = 0x0c,
    LSA = 0x0f,
    ALIGN = 0x1f,
    EXT = 0x2c,
    POOL32AXF = 0x3c,
    SIGRIE = 0x3f
};

/* POOL32AXF encoding of minor opcode field extension */

/*
 * 1. MIPS Architecture for Programmers Volume II-B:
 *      The microMIPS32 Instruction Set (Revision 3.05)
 *
 *    Table 6.5 POOL32Axf Encoding of Minor Opcode Extension Field
 *
 * 2. MIPS Architecture for Programmers VolumeIV-e:
 *      The MIPS DSP Application-Specific Extension
 *        to the microMIPS32 Architecture (Revision 2.34)
 *
 *    Table 5.5 POOL32Axf Encoding of Minor Opcode Extension Field
 */

enum {
    /* bits 11..6 */
    TEQ = 0x00,
    TGE = 0x08,
    TGEU = 0x10,
    TLT = 0x20,
    TLTU = 0x28,
    TNE = 0x30,

    MFC0 = 0x03,
    MTC0 = 0x0b,

    /* begin of microMIPS32 DSP */

    /* bits 13..12 for 0x01 */
    MFHI_ACC = 0x0,
    MFLO_ACC = 0x1,
    MTHI_ACC = 0x2,
    MTLO_ACC = 0x3,

    /* bits 13..12 for 0x2a */
    MADD_ACC = 0x0,
    MADDU_ACC = 0x1,
    MSUB_ACC = 0x2,
    MSUBU_ACC = 0x3,

    /* bits 13..12 for 0x32 */
    MULT_ACC = 0x0,
    MULTU_ACC = 0x1,

    /* end of microMIPS32 DSP */

    /* bits 15..12 for 0x2c */
    BITSWAP = 0x0,
    SEB = 0x2,
    SEH = 0x3,
    CLO = 0x4,
    CLZ = 0x5,
    RDHWR = 0x6,
    WSBH = 0x7,
    MULT = 0x8,
    MULTU = 0x9,
    DIV = 0xa,
    DIVU = 0xb,
    MADD = 0xc,
    MADDU = 0xd,
    MSUB = 0xe,
    MSUBU = 0xf,

    /* bits 15..12 for 0x34 */
    MFC2 = 0x4,
    MTC2 = 0x5,
    MFHC2 = 0x8,
    MTHC2 = 0x9,
    CFC2 = 0xc,
    CTC2 = 0xd,

    /* bits 15..12 for 0x3c */
    JALR = 0x0,
    JR = 0x0,                   /* alias */
    JALRC = 0x0,
    JRC = 0x0,
    JALR_HB = 0x1,
    JALRC_HB = 0x1,
    JALRS = 0x4,
    JALRS_HB = 0x5,

    /* bits 15..12 for 0x05 */
    RDPGPR = 0xe,
    WRPGPR = 0xf,

    /* bits 15..12 for 0x0d */
    TLBP = 0x0,
    TLBR = 0x1,
    TLBWI = 0x2,
    TLBWR = 0x3,
    TLBINV = 0x4,
    TLBINVF = 0x5,
    WAIT = 0x9,
    IRET = 0xd,
    DERET = 0xe,
    ERET = 0xf,

    /* bits 15..12 for 0x15 */
    DMT = 0x0,
    DVPE = 0x1,
    EMT = 0x2,
    EVPE = 0x3,

    /* bits 15..12 for 0x1d */
    DI = 0x4,
    EI = 0x5,

    /* bits 15..12 for 0x2d */
    SYNC = 0x6,
    SYSCALL = 0x8,
    SDBBP = 0xd,

    /* bits 15..12 for 0x35 */
    MFHI32 = 0x0,
    MFLO32 = 0x1,
    MTHI32 = 0x2,
    MTLO32 = 0x3,
};

/* POOL32B encoding of minor opcode field (bits 15..12) */

enum {
    LWC2 = 0x0,
    LWP = 0x1,
    LDP = 0x4,
    LWM32 = 0x5,
    CACHE = 0x6,
    LDM = 0x7,
    SWC2 = 0x8,
    SWP = 0x9,
    SDP = 0xc,
    SWM32 = 0xd,
    SDM = 0xf
};

/* POOL32C encoding of minor opcode field (bits 15..12) */

enum {
    LWL = 0x0,
    SWL = 0x8,
    LWR = 0x1,
    SWR = 0x9,
    PREF = 0x2,
    ST_EVA = 0xa,
    LL = 0x3,
    SC = 0xb,
    LDL = 0x4,
    SDL = 0xc,
    LDR = 0x5,
    SDR = 0xd,
    LD_EVA = 0x6,
    LWU = 0xe,
    LLD = 0x7,
    SCD = 0xf
};

/* POOL32C LD-EVA encoding of minor opcode field (bits 11..9) */

enum {
    LBUE = 0x0,
    LHUE = 0x1,
    LWLE = 0x2,
    LWRE = 0x3,
    LBE = 0x4,
    LHE = 0x5,
    LLE = 0x6,
    LWE = 0x7,
};

/* POOL32C ST-EVA encoding of minor opcode field (bits 11..9) */

enum {
    SWLE = 0x0,
    SWRE = 0x1,
    PREFE = 0x2,
    CACHEE = 0x3,
    SBE = 0x4,
    SHE = 0x5,
    SCE = 0x6,
    SWE = 0x7,
};

/* POOL32F encoding of minor opcode field (bits 5..0) */

enum {
    /* These are the bit 7..6 values */
    ADD_FMT = 0x0,

    SUB_FMT = 0x1,

    MUL_FMT = 0x2,

    DIV_FMT = 0x3,

    /* These are the bit 8..6 values */
    MOVN_FMT = 0x0,
    RSQRT2_FMT = 0x0,
    MOVF_FMT = 0x0,
    RINT_FMT = 0x0,
    SELNEZ_FMT = 0x0,

    MOVZ_FMT = 0x1,
    LWXC1 = 0x1,
    MOVT_FMT = 0x1,
    CLASS_FMT = 0x1,
    SELEQZ_FMT = 0x1,

    PLL_PS = 0x2,
    SWXC1 = 0x2,
    SEL_FMT = 0x2,

    PLU_PS = 0x3,
    LDXC1 = 0x3,

    MOVN_FMT_04 = 0x4,
    PUL_PS = 0x4,
    SDXC1 = 0x4,
    RECIP2_FMT = 0x4,

    MOVZ_FMT_05 = 0x05,
    PUU_PS = 0x5,
    LUXC1 = 0x5,

    CVT_PS_S = 0x6,
    SUXC1 = 0x6,
    ADDR_PS = 0x6,
    PREFX = 0x6,
    MADDF_FMT = 0x6,

    MULR_PS = 0x7,
    MSUBF_FMT = 0x7,

    MADD_S = 0x01,
    MADD_D = 0x09,
    MADD_PS = 0x11,
    ALNV_PS = 0x19,
    MSUB_S = 0x21,
    MSUB_D = 0x29,
    MSUB_PS = 0x31,

    NMADD_S = 0x02,
    NMADD_D = 0x0a,
    NMADD_PS = 0x12,
    NMSUB_S = 0x22,
    NMSUB_D = 0x2a,
    NMSUB_PS = 0x32,

    MIN_FMT = 0x3,
    MAX_FMT = 0xb,
    MINA_FMT = 0x23,
    MAXA_FMT = 0x2b,
    POOL32FXF = 0x3b,

    CABS_COND_FMT = 0x1c,              /* MIPS3D */
    C_COND_FMT = 0x3c,

    CMP_CONDN_S = 0x5,
    CMP_CONDN_D = 0x15
};

/* POOL32Fxf encoding of minor opcode extension field */

enum {
    CVT_L = 0x04,
    RSQRT_FMT = 0x08,
    FLOOR_L = 0x0c,
    CVT_PW_PS = 0x1c,
    CVT_W = 0x24,
    SQRT_FMT = 0x28,
    FLOOR_W = 0x2c,
    CVT_PS_PW = 0x3c,
    CFC1 = 0x40,
    RECIP_FMT = 0x48,
    CEIL_L = 0x4c,
    CTC1 = 0x60,
    CEIL_W = 0x6c,
    MFC1 = 0x80,
    CVT_S_PL = 0x84,
    TRUNC_L = 0x8c,
    MTC1 = 0xa0,
    CVT_S_PU = 0xa4,
    TRUNC_W = 0xac,
    MFHC1 = 0xc0,
    ROUND_L = 0xcc,
    MTHC1 = 0xe0,
    ROUND_W = 0xec,

    MOV_FMT = 0x01,
    MOVF = 0x05,
    ABS_FMT = 0x0d,
    RSQRT1_FMT = 0x1d,
    MOVT = 0x25,
    NEG_FMT = 0x2d,
    CVT_D = 0x4d,
    RECIP1_FMT = 0x5d,
    CVT_S = 0x6d
};

/* POOL32I encoding of minor opcode field (bits 25..21) */

enum {
    BLTZ = 0x00,
    BLTZAL = 0x01,
    BGEZ = 0x02,
    BGEZAL = 0x03,
    BLEZ = 0x04,
    BNEZC = 0x05,
    BGTZ = 0x06,
    BEQZC = 0x07,
    TLTI = 0x08,
    BC1EQZC = 0x08,
    TGEI = 0x09,
    BC1NEZC = 0x09,
    TLTIU = 0x0a,
    BC2EQZC = 0x0a,
    TGEIU = 0x0b,
    BC2NEZC = 0x0a,
    TNEI = 0x0c,
    R6_SYNCI = 0x0c,
    LUI = 0x0d,
    TEQI = 0x0e,
    SYNCI = 0x10,
    BLTZALS = 0x11,
    BGEZALS = 0x13,
    BC2F = 0x14,
    BC2T = 0x15,
    BPOSGE64 = 0x1a,
    BPOSGE32 = 0x1b,
    /* These overlap and are distinguished by bit16 of the instruction */
    BC1F = 0x1c,
    BC1T = 0x1d,
    BC1ANY2F = 0x1c,
    BC1ANY2T = 0x1d,
    BC1ANY4F = 0x1e,
    BC1ANY4T = 0x1f
};

/* POOL16A encoding of minor opcode field */

enum {
    ADDU16 = 0x0,
    SUBU16 = 0x1
};

/* POOL16B encoding of minor opcode field */

enum {
    SLL16 = 0x0,
    SRL16 = 0x1
};

/* POOL16C encoding of minor opcode field */

enum {
    NOT16 = 0x00,
    XOR16 = 0x04,
    AND16 = 0x08,
    OR16 = 0x0c,
    LWM16 = 0x10,
    SWM16 = 0x14,
    JR16 = 0x18,
    JRC16 = 0x1a,
    JALR16 = 0x1c,
    JALR16S = 0x1e,
    MFHI16 = 0x20,
    MFLO16 = 0x24,
    BREAK16 = 0x28,
    SDBBP16 = 0x2c,
    JRADDIUSP = 0x30
};

/* R6 POOL16C encoding of minor opcode field (bits 0..5) */

enum {
    R6_NOT16    = 0x00,
    R6_AND16    = 0x01,
    R6_LWM16    = 0x02,
    R6_JRC16    = 0x03,
    MOVEP       = 0x04,
    MOVEP_05    = 0x05,
    MOVEP_06    = 0x06,
    MOVEP_07    = 0x07,
    R6_XOR16    = 0x08,
    R6_OR16     = 0x09,
    R6_SWM16    = 0x0a,
    JALRC16     = 0x0b,
    MOVEP_0C    = 0x0c,
    MOVEP_0D    = 0x0d,
    MOVEP_0E    = 0x0e,
    MOVEP_0F    = 0x0f,
    JRCADDIUSP  = 0x13,
    R6_BREAK16  = 0x1b,
    R6_SDBBP16  = 0x3b
};

/* POOL16D encoding of minor opcode field */

enum {
    ADDIUS5 = 0x0,
    ADDIUSP = 0x1
};

/* POOL16E encoding of minor opcode field */

enum {
    ADDIUR2 = 0x0,
    ADDIUR1SP = 0x1
};

static int mmreg(int r)
{
    static const int map[] = { 16, 17, 2, 3, 4, 5, 6, 7 };

    return map[r];
}

/* Used for 16-bit store instructions.  */
static int mmreg2(int r)
{
    static const int map[] = { 0, 17, 2, 3, 4, 5, 6, 7 };

    return map[r];
}

#define uMIPS_RD(op) ((op >> 7) & 0x7)
#define uMIPS_RS(op) ((op >> 4) & 0x7)
#define uMIPS_RS2(op) uMIPS_RS(op)
#define uMIPS_RS1(op) ((op >> 1) & 0x7)
#define uMIPS_RD5(op) ((op >> 5) & 0x1f)
#define uMIPS_RS5(op) (op & 0x1f)

/* Signed immediate */
#define SIMM(op, start, width)                                          \
    ((int32_t)(((op >> start) & ((~0U) >> (32 - width)))                \
               << (32 - width))                                         \
     >> (32 - width))
/* Zero-extended immediate */
#define ZIMM(op, start, width) ((op >> start) & ((~0U) >> (32 - width)))

static void gen_addiur1sp(DisasContext *ctx)
{
    int rd = mmreg(uMIPS_RD(ctx->opcode));

    gen_arith_imm(ctx, OPC_ADDIU, rd, 29, ((ctx->opcode >> 1) & 0x3f) << 2);
}

static void gen_addiur2(DisasContext *ctx)
{
    static const int decoded_imm[] = { 1, 4, 8, 12, 16, 20, 24, -1 };
    int rd = mmreg(uMIPS_RD(ctx->opcode));
    int rs = mmreg(uMIPS_RS(ctx->opcode));

    gen_arith_imm(ctx, OPC_ADDIU, rd, rs, decoded_imm[ZIMM(ctx->opcode, 1, 3)]);
}

static void gen_addiusp(DisasContext *ctx)
{
    int encoded = ZIMM(ctx->opcode, 1, 9);
    int decoded;

    if (encoded <= 1) {
        decoded = 256 + encoded;
    } else if (encoded <= 255) {
        decoded = encoded;
    } else if (encoded <= 509) {
        decoded = encoded - 512;
    } else {
        decoded = encoded - 768;
    }

    gen_arith_imm(ctx, OPC_ADDIU, 29, 29, decoded << 2);
}

static void gen_addius5(DisasContext *ctx)
{
    int imm = SIMM(ctx->opcode, 1, 4);
    int rd = (ctx->opcode >> 5) & 0x1f;

    gen_arith_imm(ctx, OPC_ADDIU, rd, rd, imm);
}

static void gen_andi16(DisasContext *ctx)
{
    static const int decoded_imm[] = { 128, 1, 2, 3, 4, 7, 8, 15, 16,
                                 31, 32, 63, 64, 255, 32768, 65535 };
    int rd = mmreg(uMIPS_RD(ctx->opcode));
    int rs = mmreg(uMIPS_RS(ctx->opcode));
    int encoded = ZIMM(ctx->opcode, 0, 4);

    gen_logic_imm(ctx, OPC_ANDI, rd, rs, decoded_imm[encoded]);
}

static void gen_ldst_multiple(DisasContext *ctx, uint32_t opc, int reglist,
                              int base, int16_t offset)
{
    TCGv t0, t1;
    TCGv_i32 t2;

    if (ctx->hflags & MIPS_HFLAG_BMASK) {
        gen_reserved_instruction(ctx);
        return;
    }

    t0 = tcg_temp_new();

    gen_base_offset_addr(ctx, t0, base, offset);

    t1 = tcg_const_tl(reglist);
    t2 = tcg_const_i32(ctx->mem_idx);

    save_cpu_state(ctx, 1);
    switch (opc) {
    case LWM32:
        gen_helper_lwm(cpu_env, t0, t1, t2);
        break;
    case SWM32:
        gen_helper_swm(cpu_env, t0, t1, t2);
        break;
#ifdef TARGET_MIPS64
    case LDM:
        gen_helper_ldm(cpu_env, t0, t1, t2);
        break;
    case SDM:
        gen_helper_sdm(cpu_env, t0, t1, t2);
        break;
#endif
    }
    tcg_temp_free(t0);
    tcg_temp_free(t1);
    tcg_temp_free_i32(t2);
}


static void gen_pool16c_insn(DisasContext *ctx)
{
    int rd = mmreg((ctx->opcode >> 3) & 0x7);
    int rs = mmreg(ctx->opcode & 0x7);

    switch (((ctx->opcode) >> 4) & 0x3f) {
    case NOT16 + 0:
    case NOT16 + 1:
    case NOT16 + 2:
    case NOT16 + 3:
        gen_logic(ctx, OPC_NOR, rd, rs, 0);
        break;
    case XOR16 + 0:
    case XOR16 + 1:
    case XOR16 + 2:
    case XOR16 + 3:
        gen_logic(ctx, OPC_XOR, rd, rd, rs);
        break;
    case AND16 + 0:
    case AND16 + 1:
    case AND16 + 2:
    case AND16 + 3:
        gen_logic(ctx, OPC_AND, rd, rd, rs);
        break;
    case OR16 + 0:
    case OR16 + 1:
    case OR16 + 2:
    case OR16 + 3:
        gen_logic(ctx, OPC_OR, rd, rd, rs);
        break;
    case LWM16 + 0:
    case LWM16 + 1:
    case LWM16 + 2:
    case LWM16 + 3:
        {
            static const int lwm_convert[] = { 0x11, 0x12, 0x13, 0x14 };
            int offset = ZIMM(ctx->opcode, 0, 4);

            gen_ldst_multiple(ctx, LWM32, lwm_convert[(ctx->opcode >> 4) & 0x3],
                              29, offset << 2);
        }
        break;
    case SWM16 + 0:
    case SWM16 + 1:
    case SWM16 + 2:
    case SWM16 + 3:
        {
            static const int swm_convert[] = { 0x11, 0x12, 0x13, 0x14 };
            int offset = ZIMM(ctx->opcode, 0, 4);

            gen_ldst_multiple(ctx, SWM32, swm_convert[(ctx->opcode >> 4) & 0x3],
                              29, offset << 2);
        }
        break;
    case JR16 + 0:
    case JR16 + 1:
        {
            int reg = ctx->opcode & 0x1f;

            gen_compute_branch(ctx, OPC_JR, 2, reg, 0, 0, 4);
        }
        break;
    case JRC16 + 0:
    case JRC16 + 1:
        {
            int reg = ctx->opcode & 0x1f;
            gen_compute_branch(ctx, OPC_JR, 2, reg, 0, 0, 0);
            /*
             * Let normal delay slot handling in our caller take us
             * to the branch target.
             */
        }
        break;
    case JALR16 + 0:
    case JALR16 + 1:
        gen_compute_branch(ctx, OPC_JALR, 2, ctx->opcode & 0x1f, 31, 0, 4);
        ctx->hflags |= MIPS_HFLAG_BDS_STRICT;
        break;
    case JALR16S + 0:
    case JALR16S + 1:
        gen_compute_branch(ctx, OPC_JALR, 2, ctx->opcode & 0x1f, 31, 0, 2);
        ctx->hflags |= MIPS_HFLAG_BDS_STRICT;
        break;
    case MFHI16 + 0:
    case MFHI16 + 1:
        gen_HILO(ctx, OPC_MFHI, 0, uMIPS_RS5(ctx->opcode));
        break;
    case MFLO16 + 0:
    case MFLO16 + 1:
        gen_HILO(ctx, OPC_MFLO, 0, uMIPS_RS5(ctx->opcode));
        break;
    case BREAK16:
        generate_exception_end(ctx, EXCP_BREAK);
        break;
    case SDBBP16:
        if (is_uhi(extract32(ctx->opcode, 0, 4))) {
            gen_helper_do_semihosting(cpu_env);
        } else {
            /*
             * XXX: not clear which exception should be raised
             *      when in debug mode...
             */
            check_insn(ctx, ISA_MIPS_R1);
            generate_exception_end(ctx, EXCP_DBp);
        }
        break;
    case JRADDIUSP + 0:
    case JRADDIUSP + 1:
        {
            int imm = ZIMM(ctx->opcode, 0, 5);
            gen_compute_branch(ctx, OPC_JR, 2, 31, 0, 0, 0);
            gen_arith_imm(ctx, OPC_ADDIU, 29, 29, imm << 2);
            /*
             * Let normal delay slot handling in our caller take us
             * to the branch target.
             */
        }
        break;
    default:
        gen_reserved_instruction(ctx);
        break;
    }
}

static inline void gen_movep(DisasContext *ctx, int enc_dest, int enc_rt,
                             int enc_rs)
{
    int rd, re;
    static const int rd_enc[] = { 5, 5, 6, 4, 4, 4, 4, 4 };
    static const int re_enc[] = { 6, 7, 7, 21, 22, 5, 6, 7 };
    static const int rs_rt_enc[] = { 0, 17, 2, 3, 16, 18, 19, 20 };

    rd = rd_enc[enc_dest];
    re = re_enc[enc_dest];
    gen_load_gpr(cpu_gpr[rd], rs_rt_enc[enc_rs]);
    gen_log_instr_gpr_update(ctx, rd);
    gen_load_gpr(cpu_gpr[re], rs_rt_enc[enc_rt]);
    gen_log_instr_gpr_update(ctx, re);
}

static void gen_pool16c_r6_insn(DisasContext *ctx)
{
    int rt = mmreg((ctx->opcode >> 7) & 0x7);
    int rs = mmreg((ctx->opcode >> 4) & 0x7);

    switch (ctx->opcode & 0xf) {
    case R6_NOT16:
        gen_logic(ctx, OPC_NOR, rt, rs, 0);
        break;
    case R6_AND16:
        gen_logic(ctx, OPC_AND, rt, rt, rs);
        break;
    case R6_LWM16:
        {
            int lwm_converted = 0x11 + extract32(ctx->opcode, 8, 2);
            int offset = extract32(ctx->opcode, 4, 4);
            gen_ldst_multiple(ctx, LWM32, lwm_converted, 29, offset << 2);
        }
        break;
    case R6_JRC16: /* JRCADDIUSP */
        if ((ctx->opcode >> 4) & 1) {
            /* JRCADDIUSP */
            int imm = extract32(ctx->opcode, 5, 5);
            gen_compute_branch(ctx, OPC_JR, 2, 31, 0, 0, 0);
            gen_arith_imm(ctx, OPC_ADDIU, 29, 29, imm << 2);
        } else {
            /* JRC16 */
            rs = extract32(ctx->opcode, 5, 5);
            gen_compute_branch(ctx, OPC_JR, 2, rs, 0, 0, 0);
        }
        break;
    case MOVEP:
    case MOVEP_05:
    case MOVEP_06:
    case MOVEP_07:
    case MOVEP_0C:
    case MOVEP_0D:
    case MOVEP_0E:
    case MOVEP_0F:
        {
            int enc_dest = uMIPS_RD(ctx->opcode);
            int enc_rt = uMIPS_RS2(ctx->opcode);
            int enc_rs = (ctx->opcode & 3) | ((ctx->opcode >> 1) & 4);
            gen_movep(ctx, enc_dest, enc_rt, enc_rs);
        }
        break;
    case R6_XOR16:
        gen_logic(ctx, OPC_XOR, rt, rt, rs);
        break;
    case R6_OR16:
        gen_logic(ctx, OPC_OR, rt, rt, rs);
        break;
    case R6_SWM16:
        {
            int swm_converted = 0x11 + extract32(ctx->opcode, 8, 2);
            int offset = extract32(ctx->opcode, 4, 4);
            gen_ldst_multiple(ctx, SWM32, swm_converted, 29, offset << 2);
        }
        break;
    case JALRC16: /* BREAK16, SDBBP16 */
        switch (ctx->opcode & 0x3f) {
        case JALRC16:
        case JALRC16 + 0x20:
            /* JALRC16 */
            gen_compute_branch(ctx, OPC_JALR, 2, (ctx->opcode >> 5) & 0x1f,
                               31, 0, 0);
            break;
        case R6_BREAK16:
            /* BREAK16 */
            generate_exception(ctx, EXCP_BREAK);
            break;
        case R6_SDBBP16:
            /* SDBBP16 */
            if (is_uhi(extract32(ctx->opcode, 6, 4))) {
                gen_helper_do_semihosting(cpu_env);
            } else {
                if (ctx->hflags & MIPS_HFLAG_SBRI) {
                    generate_exception(ctx, EXCP_RI);
                } else {
                    generate_exception(ctx, EXCP_DBp);
                }
            }
            break;
        }
        break;
    default:
        generate_exception(ctx, EXCP_RI);
        break;
    }
}

static void gen_ldxs(DisasContext *ctx, int base, int index, int rd, int opc)
{
    TCGv t0 = tcg_temp_new();
    TCGv t1 = tcg_temp_new();

    gen_load_gpr(t0, base);

    if (index != 0) {
        gen_load_gpr(t1, index);
        tcg_gen_shli_tl(t1, t1, 2);
        gen_op_addr_add(ctx, t0, t1, t0);
    }
    gen_ddc_interposed_ld_tl(ctx, t1, NULL/* add $ddc to t0*/, t0, ctx->mem_idx, MO_TESL);
    gen_store_gpr(t1, rd);

    tcg_temp_free(t0);
    tcg_temp_free(t1);
}

static void gen_ldst_pair(DisasContext *ctx, uint32_t opc, int rd,
                          int base, int16_t offset)
{
    TCGv t0, t1;

    if (ctx->hflags & MIPS_HFLAG_BMASK || rd == 31) {
        gen_reserved_instruction(ctx);
        return;
    }

    t0 = tcg_temp_new();
    t1 = tcg_temp_new();
    TCGv_cap_checked_ptr ddc_adjusted = tcg_temp_new_cap_checked();

    gen_base_offset_addr(ctx, t0, base, offset);

    switch (opc) {
    case LWP:
        if (rd == base) {
            gen_reserved_instruction(ctx);
            return;
        }
        // FIXME: should LWP fail early or allow loading one word for $ddc errors?
        generate_ddc_checked_load_ptr(ddc_adjusted, ctx, t0, memop_size(MO_TESL) * 2);
        tcg_gen_qemu_ld_tl_with_checked_addr(t1, ddc_adjusted, ctx->mem_idx, MO_TESL);
        gen_store_gpr(t1, rd);
        tcg_gen_movi_tl(t1, 4);
        gen_op_addr_add(ctx, (TCGv)ddc_adjusted, (TCGv)ddc_adjusted, t1);
        tcg_gen_qemu_ld_tl_with_checked_addr(t1, ddc_adjusted, ctx->mem_idx, MO_TESL);
        gen_store_gpr(t1, rd + 1);
        break;
    case SWP:
        generate_ddc_checked_store_ptr(ddc_adjusted, ctx, t0, memop_size(MO_TESL) * 2);
        gen_load_gpr(t1, rd);
        tcg_gen_qemu_st_tl_with_checked_addr(t1, ddc_adjusted, ctx->mem_idx, MO_TEUL);
        tcg_gen_movi_tl(t1, 4);
        gen_op_addr_add(ctx, (TCGv)ddc_adjusted, (TCGv)ddc_adjusted, t1);
        gen_load_gpr(t1, rd + 1);
        tcg_gen_qemu_st_tl_with_checked_addr(t1, ddc_adjusted, ctx->mem_idx, MO_TEUL);
        break;
#ifdef TARGET_MIPS64
    case LDP:
        if (rd == base) {
            gen_reserved_instruction(ctx);
            return;
        }
        generate_ddc_checked_load_ptr(ddc_adjusted, ctx, t0, memop_size(MO_TEQ) * 2);
        tcg_gen_qemu_ld_tl_with_checked_addr(t1, ddc_adjusted, ctx->mem_idx, MO_TEQ);
        gen_store_gpr(t1, rd);
        tcg_gen_movi_tl(t1, 8);
        gen_op_addr_add(ctx, (TCGv)ddc_adjusted, (TCGv)ddc_adjusted, t1);
        tcg_gen_qemu_ld_tl_with_checked_addr(t1, ddc_adjusted, ctx->mem_idx, MO_TEQ);
        gen_store_gpr(t1, rd + 1);
        break;
    case SDP:
        generate_ddc_checked_store_ptr(ddc_adjusted, ctx, t0, memop_size(MO_TEQ) * 2);
        gen_load_gpr(t1, rd);
        tcg_gen_qemu_st_tl_with_checked_addr(t1, ddc_adjusted, ctx->mem_idx, MO_TEQ);
        tcg_gen_movi_tl(t1, 8);
        gen_op_addr_add(ctx, (TCGv)ddc_adjusted, (TCGv)ddc_adjusted, t1);
        gen_load_gpr(t1, rd + 1);
        tcg_gen_qemu_st_tl_with_checked_addr(t1, ddc_adjusted, ctx->mem_idx, MO_TEQ);
        break;
#endif
    }
    tcg_temp_free(t0);
    tcg_temp_free(t1);
    tcg_temp_free_cap_checked(ddc_adjusted);
}

static void gen_sync(int stype)
{
    TCGBar tcg_mo = TCG_BAR_SC;

    switch (stype) {
    case 0x4: /* SYNC_WMB */
        tcg_mo |= TCG_MO_ST_ST;
        break;
    case 0x10: /* SYNC_MB */
        tcg_mo |= TCG_MO_ALL;
        break;
    case 0x11: /* SYNC_ACQUIRE */
        tcg_mo |= TCG_MO_LD_LD | TCG_MO_LD_ST;
        break;
    case 0x12: /* SYNC_RELEASE */
        tcg_mo |= TCG_MO_ST_ST | TCG_MO_LD_ST;
        break;
    case 0x13: /* SYNC_RMB */
        tcg_mo |= TCG_MO_LD_LD;
        break;
    default:
        tcg_mo |= TCG_MO_ALL;
        break;
    }

    tcg_gen_mb(tcg_mo);
}

static void gen_pool32axf(CPUMIPSState *env, DisasContext *ctx, int rt, int rs)
{
    int extension = (ctx->opcode >> 6) & 0x3f;
    int minor = (ctx->opcode >> 12) & 0xf;
    uint32_t mips32_op;

    switch (extension) {
    case TEQ:
        mips32_op = OPC_TEQ;
        goto do_trap;
    case TGE:
        mips32_op = OPC_TGE;
        goto do_trap;
    case TGEU:
        mips32_op = OPC_TGEU;
        goto do_trap;
    case TLT:
        mips32_op = OPC_TLT;
        goto do_trap;
    case TLTU:
        mips32_op = OPC_TLTU;
        goto do_trap;
    case TNE:
        mips32_op = OPC_TNE;
    do_trap:
        gen_trap(ctx, mips32_op, rs, rt, -1);
        break;
#ifndef CONFIG_USER_ONLY
    case MFC0:
    case MFC0 + 32:
        check_cp0_enabled(ctx);
        if (rt == 0) {
            /* Treat as NOP. */
            break;
        }
        gen_mfc0(ctx, cpu_gpr[rt], rs, (ctx->opcode >> 11) & 0x7);
        gen_log_instr_gpr_update(ctx, rt);
        break;
    case MTC0:
    case MTC0 + 32:
        check_cp0_enabled(ctx);
        {
            TCGv t0 = tcg_temp_new();

            gen_load_gpr(t0, rt);
            gen_mtc0(ctx, t0, rs, (ctx->opcode >> 11) & 0x7);
            tcg_temp_free(t0);
        }
        break;
#endif
    case 0x2a:
        switch (minor & 3) {
        case MADD_ACC:
            gen_muldiv(ctx, OPC_MADD, (ctx->opcode >> 14) & 3, rs, rt);
            break;
        case MADDU_ACC:
            gen_muldiv(ctx, OPC_MADDU, (ctx->opcode >> 14) & 3, rs, rt);
            break;
        case MSUB_ACC:
            gen_muldiv(ctx, OPC_MSUB, (ctx->opcode >> 14) & 3, rs, rt);
            break;
        case MSUBU_ACC:
            gen_muldiv(ctx, OPC_MSUBU, (ctx->opcode >> 14) & 3, rs, rt);
            break;
        default:
            goto pool32axf_invalid;
        }
        break;
    case 0x32:
        switch (minor & 3) {
        case MULT_ACC:
            gen_muldiv(ctx, OPC_MULT, (ctx->opcode >> 14) & 3, rs, rt);
            break;
        case MULTU_ACC:
            gen_muldiv(ctx, OPC_MULTU, (ctx->opcode >> 14) & 3, rs, rt);
            break;
        default:
            goto pool32axf_invalid;
        }
        break;
    case 0x2c:
        switch (minor) {
        case BITSWAP:
            check_insn(ctx, ISA_MIPS_R6);
            gen_bitswap(ctx, OPC_BITSWAP, rs, rt);
            break;
        case SEB:
            gen_bshfl(ctx, OPC_SEB, rs, rt);
            break;
        case SEH:
            gen_bshfl(ctx, OPC_SEH, rs, rt);
            break;
        case CLO:
            mips32_op = OPC_CLO;
            goto do_cl;
        case CLZ:
            mips32_op = OPC_CLZ;
        do_cl:
            check_insn(ctx, ISA_MIPS_R1);
            gen_cl(ctx, mips32_op, rt, rs);
            break;
        case RDHWR:
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            gen_rdhwr(ctx, rt, rs, 0);
            break;
        case WSBH:
            gen_bshfl(ctx, OPC_WSBH, rs, rt);
            break;
        case MULT:
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            mips32_op = OPC_MULT;
            goto do_mul;
        case MULTU:
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            mips32_op = OPC_MULTU;
            goto do_mul;
        case DIV:
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            mips32_op = OPC_DIV;
            goto do_div;
        case DIVU:
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            mips32_op = OPC_DIVU;
            goto do_div;
        do_div:
            check_insn(ctx, ISA_MIPS_R1);
            gen_muldiv(ctx, mips32_op, 0, rs, rt);
            break;
        case MADD:
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            mips32_op = OPC_MADD;
            goto do_mul;
        case MADDU:
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            mips32_op = OPC_MADDU;
            goto do_mul;
        case MSUB:
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            mips32_op = OPC_MSUB;
            goto do_mul;
        case MSUBU:
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            mips32_op = OPC_MSUBU;
        do_mul:
            check_insn(ctx, ISA_MIPS_R1);
            gen_muldiv(ctx, mips32_op, 0, rs, rt);
            break;
        default:
            goto pool32axf_invalid;
        }
        break;
    case 0x34:
        switch (minor) {
        case MFC2:
        case MTC2:
        case MFHC2:
        case MTHC2:
        case CFC2:
        case CTC2:
            generate_exception_err(ctx, EXCP_CpU, 2);
            break;
        default:
            goto pool32axf_invalid;
        }
        break;
    case 0x3c:
        switch (minor) {
        case JALR:    /* JALRC */
        case JALR_HB: /* JALRC_HB */
            if (ctx->insn_flags & ISA_MIPS_R6) {
                /* JALRC, JALRC_HB */
                gen_compute_branch(ctx, OPC_JALR, 4, rs, rt, 0, 0);
            } else {
                /* JALR, JALR_HB */
                gen_compute_branch(ctx, OPC_JALR, 4, rs, rt, 0, 4);
                ctx->hflags |= MIPS_HFLAG_BDS_STRICT;
            }
            break;
        case JALRS:
        case JALRS_HB:
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            gen_compute_branch(ctx, OPC_JALR, 4, rs, rt, 0, 2);
            ctx->hflags |= MIPS_HFLAG_BDS_STRICT;
            break;
        default:
            goto pool32axf_invalid;
        }
        break;
    case 0x05:
        switch (minor) {
        case RDPGPR:
            check_cp0_enabled(ctx);
            check_insn(ctx, ISA_MIPS_R2);
            gen_load_srsgpr(ctx, rs, rt);
            break;
        case WRPGPR:
            check_cp0_enabled(ctx);
            check_insn(ctx, ISA_MIPS_R2);
            gen_store_srsgpr(rs, rt);
            break;
        default:
            goto pool32axf_invalid;
        }
        break;
#ifndef CONFIG_USER_ONLY
    case 0x0d:
        switch (minor) {
        case TLBP:
            mips32_op = OPC_TLBP;
            goto do_cp0;
        case TLBR:
            mips32_op = OPC_TLBR;
            goto do_cp0;
        case TLBWI:
            mips32_op = OPC_TLBWI;
            goto do_cp0;
        case TLBWR:
            mips32_op = OPC_TLBWR;
            goto do_cp0;
        case TLBINV:
            mips32_op = OPC_TLBINV;
            goto do_cp0;
        case TLBINVF:
            mips32_op = OPC_TLBINVF;
            goto do_cp0;
        case WAIT:
            mips32_op = OPC_WAIT;
            goto do_cp0;
        case DERET:
            mips32_op = OPC_DERET;
            goto do_cp0;
        case ERET:
            mips32_op = OPC_ERET;
        do_cp0:
            gen_cp0(env, ctx, mips32_op, rt, rs);
            break;
        default:
            goto pool32axf_invalid;
        }
        break;
    case 0x1d:
        switch (minor) {
        case DI:
            check_cp0_enabled(ctx);
            {
                TCGv t0 = tcg_temp_new();

                save_cpu_state(ctx, 1);
                gen_helper_di(t0, cpu_env);
                gen_store_gpr(t0, rs);
                /*
                 * Stop translation as we may have switched the execution
                 * mode.
                 */
                ctx->base.is_jmp = DISAS_STOP;
                tcg_temp_free(t0);
            }
            break;
        case EI:
            check_cp0_enabled(ctx);
            {
                TCGv t0 = tcg_temp_new();

                save_cpu_state(ctx, 1);
                gen_helper_ei(t0, cpu_env);
                gen_store_gpr(t0, rs);
                /*
                 * DISAS_STOP isn't sufficient, we need to ensure we break out
                 * of translated code to check for pending interrupts.
                 */
                gen_save_pc(ctx->base.pc_next + 4);
                ctx->base.is_jmp = DISAS_EXIT;
                tcg_temp_free(t0);
            }
            break;
        default:
            goto pool32axf_invalid;
        }
        break;
#endif
    case 0x2d:
        switch (minor) {
        case SYNC:
            gen_sync(extract32(ctx->opcode, 16, 5));
            break;
        case SYSCALL:
            generate_exception_end(ctx, EXCP_SYSCALL);
            break;
        case SDBBP:
            if (is_uhi(extract32(ctx->opcode, 16, 10))) {
                gen_helper_do_semihosting(cpu_env);
            } else {
                check_insn(ctx, ISA_MIPS_R1);
                if (ctx->hflags & MIPS_HFLAG_SBRI) {
                    gen_reserved_instruction(ctx);
                } else {
                    generate_exception_end(ctx, EXCP_DBp);
                }
            }
            break;
        default:
            goto pool32axf_invalid;
        }
        break;
    case 0x01:
        switch (minor & 3) {
        case MFHI_ACC:
            gen_HILO(ctx, OPC_MFHI, minor >> 2, rs);
            break;
        case MFLO_ACC:
            gen_HILO(ctx, OPC_MFLO, minor >> 2, rs);
            break;
        case MTHI_ACC:
            gen_HILO(ctx, OPC_MTHI, minor >> 2, rs);
            break;
        case MTLO_ACC:
            gen_HILO(ctx, OPC_MTLO, minor >> 2, rs);
            break;
        default:
            goto pool32axf_invalid;
        }
        break;
    case 0x35:
        check_insn_opc_removed(ctx, ISA_MIPS_R6);
        switch (minor) {
        case MFHI32:
            gen_HILO(ctx, OPC_MFHI, 0, rs);
            break;
        case MFLO32:
            gen_HILO(ctx, OPC_MFLO, 0, rs);
            break;
        case MTHI32:
            gen_HILO(ctx, OPC_MTHI, 0, rs);
            break;
        case MTLO32:
            gen_HILO(ctx, OPC_MTLO, 0, rs);
            break;
        default:
            goto pool32axf_invalid;
        }
        break;
    default:
    pool32axf_invalid:
        MIPS_INVAL("pool32axf");
        gen_reserved_instruction(ctx);
        break;
    }
}

/*
 * Values for microMIPS fmt field.  Variable-width, depending on which
 * formats the instruction supports.
 */
enum {
    FMT_SD_S = 0,
    FMT_SD_D = 1,

    FMT_SDPS_S = 0,
    FMT_SDPS_D = 1,
    FMT_SDPS_PS = 2,

    FMT_SWL_S = 0,
    FMT_SWL_W = 1,
    FMT_SWL_L = 2,

    FMT_DWL_D = 0,
    FMT_DWL_W = 1,
    FMT_DWL_L = 2
};

static void gen_pool32fxf(DisasContext *ctx, int rt, int rs)
{
    int extension = (ctx->opcode >> 6) & 0x3ff;
    uint32_t mips32_op;

#define FLOAT_1BIT_FMT(opc, fmt)    ((fmt << 8) | opc)
#define FLOAT_2BIT_FMT(opc, fmt)    ((fmt << 7) | opc)
#define COND_FLOAT_MOV(opc, cond)   ((cond << 7) | opc)

    switch (extension) {
    case FLOAT_1BIT_FMT(CFC1, 0):
        mips32_op = OPC_CFC1;
        goto do_cp1;
    case FLOAT_1BIT_FMT(CTC1, 0):
        mips32_op = OPC_CTC1;
        goto do_cp1;
    case FLOAT_1BIT_FMT(MFC1, 0):
        mips32_op = OPC_MFC1;
        goto do_cp1;
    case FLOAT_1BIT_FMT(MTC1, 0):
        mips32_op = OPC_MTC1;
        goto do_cp1;
    case FLOAT_1BIT_FMT(MFHC1, 0):
        mips32_op = OPC_MFHC1;
        goto do_cp1;
    case FLOAT_1BIT_FMT(MTHC1, 0):
        mips32_op = OPC_MTHC1;
    do_cp1:
        gen_cp1(ctx, mips32_op, rt, rs);
        break;

        /* Reciprocal square root */
    case FLOAT_1BIT_FMT(RSQRT_FMT, FMT_SD_S):
        mips32_op = OPC_RSQRT_S;
        goto do_unaryfp;
    case FLOAT_1BIT_FMT(RSQRT_FMT, FMT_SD_D):
        mips32_op = OPC_RSQRT_D;
        goto do_unaryfp;

        /* Square root */
    case FLOAT_1BIT_FMT(SQRT_FMT, FMT_SD_S):
        mips32_op = OPC_SQRT_S;
        goto do_unaryfp;
    case FLOAT_1BIT_FMT(SQRT_FMT, FMT_SD_D):
        mips32_op = OPC_SQRT_D;
        goto do_unaryfp;

        /* Reciprocal */
    case FLOAT_1BIT_FMT(RECIP_FMT, FMT_SD_S):
        mips32_op = OPC_RECIP_S;
        goto do_unaryfp;
    case FLOAT_1BIT_FMT(RECIP_FMT, FMT_SD_D):
        mips32_op = OPC_RECIP_D;
        goto do_unaryfp;

        /* Floor */
    case FLOAT_1BIT_FMT(FLOOR_L, FMT_SD_S):
        mips32_op = OPC_FLOOR_L_S;
        goto do_unaryfp;
    case FLOAT_1BIT_FMT(FLOOR_L, FMT_SD_D):
        mips32_op = OPC_FLOOR_L_D;
        goto do_unaryfp;
    case FLOAT_1BIT_FMT(FLOOR_W, FMT_SD_S):
        mips32_op = OPC_FLOOR_W_S;
        goto do_unaryfp;
    case FLOAT_1BIT_FMT(FLOOR_W, FMT_SD_D):
        mips32_op = OPC_FLOOR_W_D;
        goto do_unaryfp;

        /* Ceiling */
    case FLOAT_1BIT_FMT(CEIL_L, FMT_SD_S):
        mips32_op = OPC_CEIL_L_S;
        goto do_unaryfp;
    case FLOAT_1BIT_FMT(CEIL_L, FMT_SD_D):
        mips32_op = OPC_CEIL_L_D;
        goto do_unaryfp;
    case FLOAT_1BIT_FMT(CEIL_W, FMT_SD_S):
        mips32_op = OPC_CEIL_W_S;
        goto do_unaryfp;
    case FLOAT_1BIT_FMT(CEIL_W, FMT_SD_D):
        mips32_op = OPC_CEIL_W_D;
        goto do_unaryfp;

        /* Truncation */
    case FLOAT_1BIT_FMT(TRUNC_L, FMT_SD_S):
        mips32_op = OPC_TRUNC_L_S;
        goto do_unaryfp;
    case FLOAT_1BIT_FMT(TRUNC_L, FMT_SD_D):
        mips32_op = OPC_TRUNC_L_D;
        goto do_unaryfp;
    case FLOAT_1BIT_FMT(TRUNC_W, FMT_SD_S):
        mips32_op = OPC_TRUNC_W_S;
        goto do_unaryfp;
    case FLOAT_1BIT_FMT(TRUNC_W, FMT_SD_D):
        mips32_op = OPC_TRUNC_W_D;
        goto do_unaryfp;

        /* Round */
    case FLOAT_1BIT_FMT(ROUND_L, FMT_SD_S):
        mips32_op = OPC_ROUND_L_S;
        goto do_unaryfp;
    case FLOAT_1BIT_FMT(ROUND_L, FMT_SD_D):
        mips32_op = OPC_ROUND_L_D;
        goto do_unaryfp;
    case FLOAT_1BIT_FMT(ROUND_W, FMT_SD_S):
        mips32_op = OPC_ROUND_W_S;
        goto do_unaryfp;
    case FLOAT_1BIT_FMT(ROUND_W, FMT_SD_D):
        mips32_op = OPC_ROUND_W_D;
        goto do_unaryfp;

        /* Integer to floating-point conversion */
    case FLOAT_1BIT_FMT(CVT_L, FMT_SD_S):
        mips32_op = OPC_CVT_L_S;
        goto do_unaryfp;
    case FLOAT_1BIT_FMT(CVT_L, FMT_SD_D):
        mips32_op = OPC_CVT_L_D;
        goto do_unaryfp;
    case FLOAT_1BIT_FMT(CVT_W, FMT_SD_S):
        mips32_op = OPC_CVT_W_S;
        goto do_unaryfp;
    case FLOAT_1BIT_FMT(CVT_W, FMT_SD_D):
        mips32_op = OPC_CVT_W_D;
        goto do_unaryfp;

        /* Paired-foo conversions */
    case FLOAT_1BIT_FMT(CVT_S_PL, 0):
        mips32_op = OPC_CVT_S_PL;
        goto do_unaryfp;
    case FLOAT_1BIT_FMT(CVT_S_PU, 0):
        mips32_op = OPC_CVT_S_PU;
        goto do_unaryfp;
    case FLOAT_1BIT_FMT(CVT_PW_PS, 0):
        mips32_op = OPC_CVT_PW_PS;
        goto do_unaryfp;
    case FLOAT_1BIT_FMT(CVT_PS_PW, 0):
        mips32_op = OPC_CVT_PS_PW;
        goto do_unaryfp;

        /* Floating-point moves */
    case FLOAT_2BIT_FMT(MOV_FMT, FMT_SDPS_S):
        mips32_op = OPC_MOV_S;
        goto do_unaryfp;
    case FLOAT_2BIT_FMT(MOV_FMT, FMT_SDPS_D):
        mips32_op = OPC_MOV_D;
        goto do_unaryfp;
    case FLOAT_2BIT_FMT(MOV_FMT, FMT_SDPS_PS):
        mips32_op = OPC_MOV_PS;
        goto do_unaryfp;

        /* Absolute value */
    case FLOAT_2BIT_FMT(ABS_FMT, FMT_SDPS_S):
        mips32_op = OPC_ABS_S;
        goto do_unaryfp;
    case FLOAT_2BIT_FMT(ABS_FMT, FMT_SDPS_D):
        mips32_op = OPC_ABS_D;
        goto do_unaryfp;
    case FLOAT_2BIT_FMT(ABS_FMT, FMT_SDPS_PS):
        mips32_op = OPC_ABS_PS;
        goto do_unaryfp;

        /* Negation */
    case FLOAT_2BIT_FMT(NEG_FMT, FMT_SDPS_S):
        mips32_op = OPC_NEG_S;
        goto do_unaryfp;
    case FLOAT_2BIT_FMT(NEG_FMT, FMT_SDPS_D):
        mips32_op = OPC_NEG_D;
        goto do_unaryfp;
    case FLOAT_2BIT_FMT(NEG_FMT, FMT_SDPS_PS):
        mips32_op = OPC_NEG_PS;
        goto do_unaryfp;

        /* Reciprocal square root step */
    case FLOAT_2BIT_FMT(RSQRT1_FMT, FMT_SDPS_S):
        mips32_op = OPC_RSQRT1_S;
        goto do_unaryfp;
    case FLOAT_2BIT_FMT(RSQRT1_FMT, FMT_SDPS_D):
        mips32_op = OPC_RSQRT1_D;
        goto do_unaryfp;
    case FLOAT_2BIT_FMT(RSQRT1_FMT, FMT_SDPS_PS):
        mips32_op = OPC_RSQRT1_PS;
        goto do_unaryfp;

        /* Reciprocal step */
    case FLOAT_2BIT_FMT(RECIP1_FMT, FMT_SDPS_S):
        mips32_op = OPC_RECIP1_S;
        goto do_unaryfp;
    case FLOAT_2BIT_FMT(RECIP1_FMT, FMT_SDPS_D):
        mips32_op = OPC_RECIP1_S;
        goto do_unaryfp;
    case FLOAT_2BIT_FMT(RECIP1_FMT, FMT_SDPS_PS):
        mips32_op = OPC_RECIP1_PS;
        goto do_unaryfp;

        /* Conversions from double */
    case FLOAT_2BIT_FMT(CVT_D, FMT_SWL_S):
        mips32_op = OPC_CVT_D_S;
        goto do_unaryfp;
    case FLOAT_2BIT_FMT(CVT_D, FMT_SWL_W):
        mips32_op = OPC_CVT_D_W;
        goto do_unaryfp;
    case FLOAT_2BIT_FMT(CVT_D, FMT_SWL_L):
        mips32_op = OPC_CVT_D_L;
        goto do_unaryfp;

        /* Conversions from single */
    case FLOAT_2BIT_FMT(CVT_S, FMT_DWL_D):
        mips32_op = OPC_CVT_S_D;
        goto do_unaryfp;
    case FLOAT_2BIT_FMT(CVT_S, FMT_DWL_W):
        mips32_op = OPC_CVT_S_W;
        goto do_unaryfp;
    case FLOAT_2BIT_FMT(CVT_S, FMT_DWL_L):
        mips32_op = OPC_CVT_S_L;
    do_unaryfp:
        gen_farith(ctx, mips32_op, -1, rs, rt, 0);
        break;

        /* Conditional moves on floating-point codes */
    case COND_FLOAT_MOV(MOVT, 0):
    case COND_FLOAT_MOV(MOVT, 1):
    case COND_FLOAT_MOV(MOVT, 2):
    case COND_FLOAT_MOV(MOVT, 3):
    case COND_FLOAT_MOV(MOVT, 4):
    case COND_FLOAT_MOV(MOVT, 5):
    case COND_FLOAT_MOV(MOVT, 6):
    case COND_FLOAT_MOV(MOVT, 7):
        check_insn_opc_removed(ctx, ISA_MIPS_R6);
        gen_movci(ctx, rt, rs, (ctx->opcode >> 13) & 0x7, 1);
        break;
    case COND_FLOAT_MOV(MOVF, 0):
    case COND_FLOAT_MOV(MOVF, 1):
    case COND_FLOAT_MOV(MOVF, 2):
    case COND_FLOAT_MOV(MOVF, 3):
    case COND_FLOAT_MOV(MOVF, 4):
    case COND_FLOAT_MOV(MOVF, 5):
    case COND_FLOAT_MOV(MOVF, 6):
    case COND_FLOAT_MOV(MOVF, 7):
        check_insn_opc_removed(ctx, ISA_MIPS_R6);
        gen_movci(ctx, rt, rs, (ctx->opcode >> 13) & 0x7, 0);
        break;
    default:
        MIPS_INVAL("pool32fxf");
        gen_reserved_instruction(ctx);
        break;
    }
}

static void decode_micromips32_opc(CPUMIPSState *env, DisasContext *ctx)
{
    int32_t offset;
    uint16_t insn;
    int rt, rs, rd, rr;
    int16_t imm;
    uint32_t op, minor, minor2, mips32_op;
    uint32_t cond, fmt, cc;

    insn = translator_lduw(env, ctx->base.pc_next + 2);
    ctx->opcode = (ctx->opcode << 16) | insn;

    rt = (ctx->opcode >> 21) & 0x1f;
    rs = (ctx->opcode >> 16) & 0x1f;
    rd = (ctx->opcode >> 11) & 0x1f;
    rr = (ctx->opcode >> 6) & 0x1f;
    imm = (int16_t) ctx->opcode;

    op = (ctx->opcode >> 26) & 0x3f;
    switch (op) {
    case POOL32A:
        minor = ctx->opcode & 0x3f;
        switch (minor) {
        case 0x00:
            minor = (ctx->opcode >> 6) & 0xf;
            switch (minor) {
            case SLL32:
                mips32_op = OPC_SLL;
                goto do_shifti;
            case SRA:
                mips32_op = OPC_SRA;
                goto do_shifti;
            case SRL32:
                mips32_op = OPC_SRL;
                goto do_shifti;
            case ROTR:
                mips32_op = OPC_ROTR;
            do_shifti:
                gen_shift_imm(ctx, mips32_op, rt, rs, rd);
                break;
            case SELEQZ:
                check_insn(ctx, ISA_MIPS_R6);
                gen_cond_move(ctx, OPC_SELEQZ, rd, rs, rt);
                break;
            case SELNEZ:
                check_insn(ctx, ISA_MIPS_R6);
                gen_cond_move(ctx, OPC_SELNEZ, rd, rs, rt);
                break;
            case R6_RDHWR:
                check_insn(ctx, ISA_MIPS_R6);
                gen_rdhwr(ctx, rt, rs, extract32(ctx->opcode, 11, 3));
                break;
            default:
                goto pool32a_invalid;
            }
            break;
        case 0x10:
            minor = (ctx->opcode >> 6) & 0xf;
            switch (minor) {
                /* Arithmetic */
            case ADD:
                mips32_op = OPC_ADD;
                goto do_arith;
            case ADDU32:
                mips32_op = OPC_ADDU;
                goto do_arith;
            case SUB:
                mips32_op = OPC_SUB;
                goto do_arith;
            case SUBU32:
                mips32_op = OPC_SUBU;
                goto do_arith;
            case MUL:
                check_insn_opc_removed(ctx, ISA_MIPS_R6);
                mips32_op = OPC_MUL;
            do_arith:
                gen_arith(ctx, mips32_op, rd, rs, rt);
                break;
                /* Shifts */
            case SLLV:
                mips32_op = OPC_SLLV;
                goto do_shift;
            case SRLV:
                mips32_op = OPC_SRLV;
                goto do_shift;
            case SRAV:
                mips32_op = OPC_SRAV;
                goto do_shift;
            case ROTRV:
                mips32_op = OPC_ROTRV;
            do_shift:
                gen_shift(ctx, mips32_op, rd, rs, rt);
                break;
                /* Logical operations */
            case AND:
                mips32_op = OPC_AND;
                goto do_logic;
            case OR32:
                mips32_op = OPC_OR;
                goto do_logic;
            case NOR:
                mips32_op = OPC_NOR;
                goto do_logic;
            case XOR32:
                mips32_op = OPC_XOR;
            do_logic:
                gen_logic(ctx, mips32_op, rd, rs, rt);
                break;
                /* Set less than */
            case SLT:
                mips32_op = OPC_SLT;
                goto do_slt;
            case SLTU:
                mips32_op = OPC_SLTU;
            do_slt:
                gen_slt(ctx, mips32_op, rd, rs, rt);
                break;
            default:
                goto pool32a_invalid;
            }
            break;
        case 0x18:
            minor = (ctx->opcode >> 6) & 0xf;
            switch (minor) {
                /* Conditional moves */
            case MOVN: /* MUL */
                if (ctx->insn_flags & ISA_MIPS_R6) {
                    /* MUL */
                    gen_r6_muldiv(ctx, R6_OPC_MUL, rd, rs, rt);
                } else {
                    /* MOVN */
                    gen_cond_move(ctx, OPC_MOVN, rd, rs, rt);
                }
                break;
            case MOVZ: /* MUH */
                if (ctx->insn_flags & ISA_MIPS_R6) {
                    /* MUH */
                    gen_r6_muldiv(ctx, R6_OPC_MUH, rd, rs, rt);
                } else {
                    /* MOVZ */
                    gen_cond_move(ctx, OPC_MOVZ, rd, rs, rt);
                }
                break;
            case MULU:
                check_insn(ctx, ISA_MIPS_R6);
                gen_r6_muldiv(ctx, R6_OPC_MULU, rd, rs, rt);
                break;
            case MUHU:
                check_insn(ctx, ISA_MIPS_R6);
                gen_r6_muldiv(ctx, R6_OPC_MUHU, rd, rs, rt);
                break;
            case LWXS: /* DIV */
                if (ctx->insn_flags & ISA_MIPS_R6) {
                    /* DIV */
                    gen_r6_muldiv(ctx, R6_OPC_DIV, rd, rs, rt);
                } else {
                    /* LWXS */
                    gen_ldxs(ctx, rs, rt, rd, op);
                }
                break;
            case MOD:
                check_insn(ctx, ISA_MIPS_R6);
                gen_r6_muldiv(ctx, R6_OPC_MOD, rd, rs, rt);
                break;
            case R6_DIVU:
                check_insn(ctx, ISA_MIPS_R6);
                gen_r6_muldiv(ctx, R6_OPC_DIVU, rd, rs, rt);
                break;
            case MODU:
                check_insn(ctx, ISA_MIPS_R6);
                gen_r6_muldiv(ctx, R6_OPC_MODU, rd, rs, rt);
                break;
            default:
                goto pool32a_invalid;
            }
            break;
        case INS:
            gen_bitops(ctx, OPC_INS, rt, rs, rr, rd);
            return;
        case LSA:
            check_insn(ctx, ISA_MIPS_R6);
            gen_lsa(ctx, rd, rt, rs, extract32(ctx->opcode, 9, 2));
            break;
        case ALIGN:
            check_insn(ctx, ISA_MIPS_R6);
            gen_align(ctx, 32, rd, rs, rt, extract32(ctx->opcode, 9, 2));
            break;
        case EXT:
            gen_bitops(ctx, OPC_EXT, rt, rs, rr, rd);
            return;
        case POOL32AXF:
            gen_pool32axf(env, ctx, rt, rs);
            break;
        case BREAK32:
            generate_exception_end(ctx, EXCP_BREAK);
            break;
        case SIGRIE:
            check_insn(ctx, ISA_MIPS_R6);
            gen_reserved_instruction(ctx);
            break;
        default:
        pool32a_invalid:
                MIPS_INVAL("pool32a");
                gen_reserved_instruction(ctx);
                break;
        }
        break;
    case POOL32B:
        minor = (ctx->opcode >> 12) & 0xf;
        switch (minor) {
        case CACHE:
            check_cp0_enabled(ctx);
            if (ctx->hflags & MIPS_HFLAG_ITC_CACHE) {
                gen_cache_operation(ctx, rt, rs, imm);
            }
            break;
        case LWC2:
        case SWC2:
            /* COP2: Not implemented. */
            generate_exception_err(ctx, EXCP_CpU, 2);
            break;
#ifdef TARGET_MIPS64
        case LDP:
        case SDP:
            check_insn(ctx, ISA_MIPS3);
            check_mips_64(ctx);
#endif
            /* fall through */
        case LWP:
        case SWP:
            gen_ldst_pair(ctx, minor, rt, rs, SIMM(ctx->opcode, 0, 12));
            break;
#ifdef TARGET_MIPS64
        case LDM:
        case SDM:
            check_insn(ctx, ISA_MIPS3);
            check_mips_64(ctx);
#endif
            /* fall through */
        case LWM32:
        case SWM32:
            gen_ldst_multiple(ctx, minor, rt, rs, SIMM(ctx->opcode, 0, 12));
            break;
        default:
            MIPS_INVAL("pool32b");
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case POOL32F:
        if (ctx->CP0_Config1 & (1 << CP0C1_FP)) {
            minor = ctx->opcode & 0x3f;
            check_cp1_enabled(ctx);
            switch (minor) {
            case ALNV_PS:
                check_insn_opc_removed(ctx, ISA_MIPS_R6);
                mips32_op = OPC_ALNV_PS;
                goto do_madd;
            case MADD_S:
                check_insn_opc_removed(ctx, ISA_MIPS_R6);
                mips32_op = OPC_MADD_S;
                goto do_madd;
            case MADD_D:
                check_insn_opc_removed(ctx, ISA_MIPS_R6);
                mips32_op = OPC_MADD_D;
                goto do_madd;
            case MADD_PS:
                check_insn_opc_removed(ctx, ISA_MIPS_R6);
                mips32_op = OPC_MADD_PS;
                goto do_madd;
            case MSUB_S:
                check_insn_opc_removed(ctx, ISA_MIPS_R6);
                mips32_op = OPC_MSUB_S;
                goto do_madd;
            case MSUB_D:
                check_insn_opc_removed(ctx, ISA_MIPS_R6);
                mips32_op = OPC_MSUB_D;
                goto do_madd;
            case MSUB_PS:
                check_insn_opc_removed(ctx, ISA_MIPS_R6);
                mips32_op = OPC_MSUB_PS;
                goto do_madd;
            case NMADD_S:
                check_insn_opc_removed(ctx, ISA_MIPS_R6);
                mips32_op = OPC_NMADD_S;
                goto do_madd;
            case NMADD_D:
                check_insn_opc_removed(ctx, ISA_MIPS_R6);
                mips32_op = OPC_NMADD_D;
                goto do_madd;
            case NMADD_PS:
                check_insn_opc_removed(ctx, ISA_MIPS_R6);
                mips32_op = OPC_NMADD_PS;
                goto do_madd;
            case NMSUB_S:
                check_insn_opc_removed(ctx, ISA_MIPS_R6);
                mips32_op = OPC_NMSUB_S;
                goto do_madd;
            case NMSUB_D:
                check_insn_opc_removed(ctx, ISA_MIPS_R6);
                mips32_op = OPC_NMSUB_D;
                goto do_madd;
            case NMSUB_PS:
                check_insn_opc_removed(ctx, ISA_MIPS_R6);
                mips32_op = OPC_NMSUB_PS;
            do_madd:
                gen_flt3_arith(ctx, mips32_op, rd, rr, rs, rt);
                break;
            case CABS_COND_FMT:
                check_insn_opc_removed(ctx, ISA_MIPS_R6);
                cond = (ctx->opcode >> 6) & 0xf;
                cc = (ctx->opcode >> 13) & 0x7;
                fmt = (ctx->opcode >> 10) & 0x3;
                switch (fmt) {
                case 0x0:
                    gen_cmpabs_s(ctx, cond, rt, rs, cc);
                    break;
                case 0x1:
                    gen_cmpabs_d(ctx, cond, rt, rs, cc);
                    break;
                case 0x2:
                    gen_cmpabs_ps(ctx, cond, rt, rs, cc);
                    break;
                default:
                    goto pool32f_invalid;
                }
                break;
            case C_COND_FMT:
                check_insn_opc_removed(ctx, ISA_MIPS_R6);
                cond = (ctx->opcode >> 6) & 0xf;
                cc = (ctx->opcode >> 13) & 0x7;
                fmt = (ctx->opcode >> 10) & 0x3;
                switch (fmt) {
                case 0x0:
                    gen_cmp_s(ctx, cond, rt, rs, cc);
                    break;
                case 0x1:
                    gen_cmp_d(ctx, cond, rt, rs, cc);
                    break;
                case 0x2:
                    gen_cmp_ps(ctx, cond, rt, rs, cc);
                    break;
                default:
                    goto pool32f_invalid;
                }
                break;
            case CMP_CONDN_S:
                check_insn(ctx, ISA_MIPS_R6);
                gen_r6_cmp_s(ctx, (ctx->opcode >> 6) & 0x1f, rt, rs, rd);
                break;
            case CMP_CONDN_D:
                check_insn(ctx, ISA_MIPS_R6);
                gen_r6_cmp_d(ctx, (ctx->opcode >> 6) & 0x1f, rt, rs, rd);
                break;
            case POOL32FXF:
                gen_pool32fxf(ctx, rt, rs);
                break;
            case 0x00:
                /* PLL foo */
                switch ((ctx->opcode >> 6) & 0x7) {
                case PLL_PS:
                    mips32_op = OPC_PLL_PS;
                    goto do_ps;
                case PLU_PS:
                    mips32_op = OPC_PLU_PS;
                    goto do_ps;
                case PUL_PS:
                    mips32_op = OPC_PUL_PS;
                    goto do_ps;
                case PUU_PS:
                    mips32_op = OPC_PUU_PS;
                    goto do_ps;
                case CVT_PS_S:
                    check_insn_opc_removed(ctx, ISA_MIPS_R6);
                    mips32_op = OPC_CVT_PS_S;
                do_ps:
                    gen_farith(ctx, mips32_op, rt, rs, rd, 0);
                    break;
                default:
                    goto pool32f_invalid;
                }
                break;
            case MIN_FMT:
                check_insn(ctx, ISA_MIPS_R6);
                switch ((ctx->opcode >> 9) & 0x3) {
                case FMT_SDPS_S:
                    gen_farith(ctx, OPC_MIN_S, rt, rs, rd, 0);
                    break;
                case FMT_SDPS_D:
                    gen_farith(ctx, OPC_MIN_D, rt, rs, rd, 0);
                    break;
                default:
                    goto pool32f_invalid;
                }
                break;
            case 0x08:
                /* [LS][WDU]XC1 */
                switch ((ctx->opcode >> 6) & 0x7) {
                case LWXC1:
                    check_insn_opc_removed(ctx, ISA_MIPS_R6);
                    mips32_op = OPC_LWXC1;
                    goto do_ldst_cp1;
                case SWXC1:
                    check_insn_opc_removed(ctx, ISA_MIPS_R6);
                    mips32_op = OPC_SWXC1;
                    goto do_ldst_cp1;
                case LDXC1:
                    check_insn_opc_removed(ctx, ISA_MIPS_R6);
                    mips32_op = OPC_LDXC1;
                    goto do_ldst_cp1;
                case SDXC1:
                    check_insn_opc_removed(ctx, ISA_MIPS_R6);
                    mips32_op = OPC_SDXC1;
                    goto do_ldst_cp1;
                case LUXC1:
                    check_insn_opc_removed(ctx, ISA_MIPS_R6);
                    mips32_op = OPC_LUXC1;
                    goto do_ldst_cp1;
                case SUXC1:
                    check_insn_opc_removed(ctx, ISA_MIPS_R6);
                    mips32_op = OPC_SUXC1;
                do_ldst_cp1:
                    gen_flt3_ldst(ctx, mips32_op, rd, rd, rt, rs);
                    break;
                default:
                    goto pool32f_invalid;
                }
                break;
            case MAX_FMT:
                check_insn(ctx, ISA_MIPS_R6);
                switch ((ctx->opcode >> 9) & 0x3) {
                case FMT_SDPS_S:
                    gen_farith(ctx, OPC_MAX_S, rt, rs, rd, 0);
                    break;
                case FMT_SDPS_D:
                    gen_farith(ctx, OPC_MAX_D, rt, rs, rd, 0);
                    break;
                default:
                    goto pool32f_invalid;
                }
                break;
            case 0x18:
                /* 3D insns */
                check_insn_opc_removed(ctx, ISA_MIPS_R6);
                fmt = (ctx->opcode >> 9) & 0x3;
                switch ((ctx->opcode >> 6) & 0x7) {
                case RSQRT2_FMT:
                    switch (fmt) {
                    case FMT_SDPS_S:
                        mips32_op = OPC_RSQRT2_S;
                        goto do_3d;
                    case FMT_SDPS_D:
                        mips32_op = OPC_RSQRT2_D;
                        goto do_3d;
                    case FMT_SDPS_PS:
                        mips32_op = OPC_RSQRT2_PS;
                        goto do_3d;
                    default:
                        goto pool32f_invalid;
                    }
                    break;
                case RECIP2_FMT:
                    switch (fmt) {
                    case FMT_SDPS_S:
                        mips32_op = OPC_RECIP2_S;
                        goto do_3d;
                    case FMT_SDPS_D:
                        mips32_op = OPC_RECIP2_D;
                        goto do_3d;
                    case FMT_SDPS_PS:
                        mips32_op = OPC_RECIP2_PS;
                        goto do_3d;
                    default:
                        goto pool32f_invalid;
                    }
                    break;
                case ADDR_PS:
                    mips32_op = OPC_ADDR_PS;
                    goto do_3d;
                case MULR_PS:
                    mips32_op = OPC_MULR_PS;
                do_3d:
                    gen_farith(ctx, mips32_op, rt, rs, rd, 0);
                    break;
                default:
                    goto pool32f_invalid;
                }
                break;
            case 0x20:
                /* MOV[FT].fmt, PREFX, RINT.fmt, CLASS.fmt*/
                cc = (ctx->opcode >> 13) & 0x7;
                fmt = (ctx->opcode >> 9) & 0x3;
                switch ((ctx->opcode >> 6) & 0x7) {
                case MOVF_FMT: /* RINT_FMT */
                    if (ctx->insn_flags & ISA_MIPS_R6) {
                        /* RINT_FMT */
                        switch (fmt) {
                        case FMT_SDPS_S:
                            gen_farith(ctx, OPC_RINT_S, 0, rt, rs, 0);
                            break;
                        case FMT_SDPS_D:
                            gen_farith(ctx, OPC_RINT_D, 0, rt, rs, 0);
                            break;
                        default:
                            goto pool32f_invalid;
                        }
                    } else {
                        /* MOVF_FMT */
                        switch (fmt) {
                        case FMT_SDPS_S:
                            gen_movcf_s(ctx, rs, rt, cc, 0);
                            break;
                        case FMT_SDPS_D:
                            gen_movcf_d(ctx, rs, rt, cc, 0);
                            break;
                        case FMT_SDPS_PS:
                            check_ps(ctx);
                            gen_movcf_ps(ctx, rs, rt, cc, 0);
                            break;
                        default:
                            goto pool32f_invalid;
                        }
                    }
                    break;
                case MOVT_FMT: /* CLASS_FMT */
                    if (ctx->insn_flags & ISA_MIPS_R6) {
                        /* CLASS_FMT */
                        switch (fmt) {
                        case FMT_SDPS_S:
                            gen_farith(ctx, OPC_CLASS_S, 0, rt, rs, 0);
                            break;
                        case FMT_SDPS_D:
                            gen_farith(ctx, OPC_CLASS_D, 0, rt, rs, 0);
                            break;
                        default:
                            goto pool32f_invalid;
                        }
                    } else {
                        /* MOVT_FMT */
                        switch (fmt) {
                        case FMT_SDPS_S:
                            gen_movcf_s(ctx, rs, rt, cc, 1);
                            break;
                        case FMT_SDPS_D:
                            gen_movcf_d(ctx, rs, rt, cc, 1);
                            break;
                        case FMT_SDPS_PS:
                            check_ps(ctx);
                            gen_movcf_ps(ctx, rs, rt, cc, 1);
                            break;
                        default:
                            goto pool32f_invalid;
                        }
                    }
                    break;
                case PREFX:
                    check_insn_opc_removed(ctx, ISA_MIPS_R6);
                    break;
                default:
                    goto pool32f_invalid;
                }
                break;
#define FINSN_3ARG_SDPS(prfx)                           \
                switch ((ctx->opcode >> 8) & 0x3) {     \
                case FMT_SDPS_S:                        \
                    mips32_op = OPC_##prfx##_S;         \
                    goto do_fpop;                       \
                case FMT_SDPS_D:                        \
                    mips32_op = OPC_##prfx##_D;         \
                    goto do_fpop;                       \
                case FMT_SDPS_PS:                       \
                    check_ps(ctx);                      \
                    mips32_op = OPC_##prfx##_PS;        \
                    goto do_fpop;                       \
                default:                                \
                    goto pool32f_invalid;               \
                }
            case MINA_FMT:
                check_insn(ctx, ISA_MIPS_R6);
                switch ((ctx->opcode >> 9) & 0x3) {
                case FMT_SDPS_S:
                    gen_farith(ctx, OPC_MINA_S, rt, rs, rd, 0);
                    break;
                case FMT_SDPS_D:
                    gen_farith(ctx, OPC_MINA_D, rt, rs, rd, 0);
                    break;
                default:
                    goto pool32f_invalid;
                }
                break;
            case MAXA_FMT:
                check_insn(ctx, ISA_MIPS_R6);
                switch ((ctx->opcode >> 9) & 0x3) {
                case FMT_SDPS_S:
                    gen_farith(ctx, OPC_MAXA_S, rt, rs, rd, 0);
                    break;
                case FMT_SDPS_D:
                    gen_farith(ctx, OPC_MAXA_D, rt, rs, rd, 0);
                    break;
                default:
                    goto pool32f_invalid;
                }
                break;
            case 0x30:
                /* regular FP ops */
                switch ((ctx->opcode >> 6) & 0x3) {
                case ADD_FMT:
                    FINSN_3ARG_SDPS(ADD);
                    break;
                case SUB_FMT:
                    FINSN_3ARG_SDPS(SUB);
                    break;
                case MUL_FMT:
                    FINSN_3ARG_SDPS(MUL);
                    break;
                case DIV_FMT:
                    fmt = (ctx->opcode >> 8) & 0x3;
                    if (fmt == 1) {
                        mips32_op = OPC_DIV_D;
                    } else if (fmt == 0) {
                        mips32_op = OPC_DIV_S;
                    } else {
                        goto pool32f_invalid;
                    }
                    goto do_fpop;
                default:
                    goto pool32f_invalid;
                }
                break;
            case 0x38:
                /* cmovs */
                switch ((ctx->opcode >> 6) & 0x7) {
                case MOVN_FMT: /* SELEQZ_FMT */
                    if (ctx->insn_flags & ISA_MIPS_R6) {
                        /* SELEQZ_FMT */
                        switch ((ctx->opcode >> 9) & 0x3) {
                        case FMT_SDPS_S:
                            gen_sel_s(ctx, OPC_SELEQZ_S, rd, rt, rs);
                            break;
                        case FMT_SDPS_D:
                            gen_sel_d(ctx, OPC_SELEQZ_D, rd, rt, rs);
                            break;
                        default:
                            goto pool32f_invalid;
                        }
                    } else {
                        /* MOVN_FMT */
                        FINSN_3ARG_SDPS(MOVN);
                    }
                    break;
                case MOVN_FMT_04:
                    check_insn_opc_removed(ctx, ISA_MIPS_R6);
                    FINSN_3ARG_SDPS(MOVN);
                    break;
                case MOVZ_FMT: /* SELNEZ_FMT */
                    if (ctx->insn_flags & ISA_MIPS_R6) {
                        /* SELNEZ_FMT */
                        switch ((ctx->opcode >> 9) & 0x3) {
                        case FMT_SDPS_S:
                            gen_sel_s(ctx, OPC_SELNEZ_S, rd, rt, rs);
                            break;
                        case FMT_SDPS_D:
                            gen_sel_d(ctx, OPC_SELNEZ_D, rd, rt, rs);
                            break;
                        default:
                            goto pool32f_invalid;
                        }
                    } else {
                        /* MOVZ_FMT */
                        FINSN_3ARG_SDPS(MOVZ);
                    }
                    break;
                case MOVZ_FMT_05:
                    check_insn_opc_removed(ctx, ISA_MIPS_R6);
                    FINSN_3ARG_SDPS(MOVZ);
                    break;
                case SEL_FMT:
                    check_insn(ctx, ISA_MIPS_R6);
                    switch ((ctx->opcode >> 9) & 0x3) {
                    case FMT_SDPS_S:
                        gen_sel_s(ctx, OPC_SEL_S, rd, rt, rs);
                        break;
                    case FMT_SDPS_D:
                        gen_sel_d(ctx, OPC_SEL_D, rd, rt, rs);
                        break;
                    default:
                        goto pool32f_invalid;
                    }
                    break;
                case MADDF_FMT:
                    check_insn(ctx, ISA_MIPS_R6);
                    switch ((ctx->opcode >> 9) & 0x3) {
                    case FMT_SDPS_S:
                        mips32_op = OPC_MADDF_S;
                        goto do_fpop;
                    case FMT_SDPS_D:
                        mips32_op = OPC_MADDF_D;
                        goto do_fpop;
                    default:
                        goto pool32f_invalid;
                    }
                    break;
                case MSUBF_FMT:
                    check_insn(ctx, ISA_MIPS_R6);
                    switch ((ctx->opcode >> 9) & 0x3) {
                    case FMT_SDPS_S:
                        mips32_op = OPC_MSUBF_S;
                        goto do_fpop;
                    case FMT_SDPS_D:
                        mips32_op = OPC_MSUBF_D;
                        goto do_fpop;
                    default:
                        goto pool32f_invalid;
                    }
                    break;
                default:
                    goto pool32f_invalid;
                }
                break;
            do_fpop:
                gen_farith(ctx, mips32_op, rt, rs, rd, 0);
                break;
            default:
            pool32f_invalid:
                MIPS_INVAL("pool32f");
                gen_reserved_instruction(ctx);
                break;
            }
        } else {
            generate_exception_err(ctx, EXCP_CpU, 1);
        }
        break;
    case POOL32I:
        minor = (ctx->opcode >> 21) & 0x1f;
        switch (minor) {
        case BLTZ:
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            gen_compute_branch(ctx, OPC_BLTZ, 4, rs, -1, imm << 1, 4);
            break;
        case BLTZAL:
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            gen_compute_branch(ctx, OPC_BLTZAL, 4, rs, -1, imm << 1, 4);
            ctx->hflags |= MIPS_HFLAG_BDS_STRICT;
            break;
        case BLTZALS:
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            gen_compute_branch(ctx, OPC_BLTZAL, 4, rs, -1, imm << 1, 2);
            ctx->hflags |= MIPS_HFLAG_BDS_STRICT;
            break;
        case BGEZ:
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            gen_compute_branch(ctx, OPC_BGEZ, 4, rs, -1, imm << 1, 4);
            break;
        case BGEZAL:
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            gen_compute_branch(ctx, OPC_BGEZAL, 4, rs, -1, imm << 1, 4);
            ctx->hflags |= MIPS_HFLAG_BDS_STRICT;
            break;
        case BGEZALS:
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            gen_compute_branch(ctx, OPC_BGEZAL, 4, rs, -1, imm << 1, 2);
            ctx->hflags |= MIPS_HFLAG_BDS_STRICT;
            break;
        case BLEZ:
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            gen_compute_branch(ctx, OPC_BLEZ, 4, rs, -1, imm << 1, 4);
            break;
        case BGTZ:
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            gen_compute_branch(ctx, OPC_BGTZ, 4, rs, -1, imm << 1, 4);
            break;

            /* Traps */
        case TLTI: /* BC1EQZC */
            if (ctx->insn_flags & ISA_MIPS_R6) {
                /* BC1EQZC */
                check_cp1_enabled(ctx);
                gen_compute_branch1_r6(ctx, OPC_BC1EQZ, rs, imm << 1, 0);
            } else {
                /* TLTI */
                mips32_op = OPC_TLTI;
                goto do_trapi;
            }
            break;
        case TGEI: /* BC1NEZC */
            if (ctx->insn_flags & ISA_MIPS_R6) {
                /* BC1NEZC */
                check_cp1_enabled(ctx);
                gen_compute_branch1_r6(ctx, OPC_BC1NEZ, rs, imm << 1, 0);
            } else {
                /* TGEI */
                mips32_op = OPC_TGEI;
                goto do_trapi;
            }
            break;
        case TLTIU:
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            mips32_op = OPC_TLTIU;
            goto do_trapi;
        case TGEIU:
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            mips32_op = OPC_TGEIU;
            goto do_trapi;
        case TNEI: /* SYNCI */
            if (ctx->insn_flags & ISA_MIPS_R6) {
                /* SYNCI */
                /*
                 * Break the TB to be able to sync copied instructions
                 * immediately.
                 */
                ctx->base.is_jmp = DISAS_STOP;
            } else {
                /* TNEI */
                mips32_op = OPC_TNEI;
                goto do_trapi;
            }
            break;
        case TEQI:
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            mips32_op = OPC_TEQI;
        do_trapi:
            gen_trap(ctx, mips32_op, rs, -1, imm);
            break;

        case BNEZC:
        case BEQZC:
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            gen_compute_branch(ctx, minor == BNEZC ? OPC_BNE : OPC_BEQ,
                               4, rs, 0, imm << 1, 0);
            /*
             * Compact branches don't have a delay slot, so just let
             * the normal delay slot handling take us to the branch
             * target.
             */
            break;
        case LUI:
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            gen_logic_imm(ctx, OPC_LUI, rs, 0, imm);
            break;
        case SYNCI:
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            /*
             * Break the TB to be able to sync copied instructions
             * immediately.
             */
            ctx->base.is_jmp = DISAS_STOP;
            break;
        case BC2F:
        case BC2T:
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            /* COP2: Not implemented. */
            generate_exception_err(ctx, EXCP_CpU, 2);
            break;
        case BC1F:
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            mips32_op = (ctx->opcode & (1 << 16)) ? OPC_BC1FANY2 : OPC_BC1F;
            goto do_cp1branch;
        case BC1T:
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            mips32_op = (ctx->opcode & (1 << 16)) ? OPC_BC1TANY2 : OPC_BC1T;
            goto do_cp1branch;
        case BC1ANY4F:
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            mips32_op = OPC_BC1FANY4;
            goto do_cp1mips3d;
        case BC1ANY4T:
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            mips32_op = OPC_BC1TANY4;
        do_cp1mips3d:
            check_cop1x(ctx);
            check_insn(ctx, ASE_MIPS3D);
            /* Fall through */
        do_cp1branch:
            if (env->CP0_Config1 & (1 << CP0C1_FP)) {
                check_cp1_enabled(ctx);
                gen_compute_branch1(ctx, mips32_op,
                                    (ctx->opcode >> 18) & 0x7, imm << 1);
            } else {
                generate_exception_err(ctx, EXCP_CpU, 1);
            }
            break;
        case BPOSGE64:
        case BPOSGE32:
            /* MIPS DSP: not implemented */
            /* Fall through */
        default:
            MIPS_INVAL("pool32i");
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case POOL32C:
        minor = (ctx->opcode >> 12) & 0xf;
        offset = sextract32(ctx->opcode, 0,
                            (ctx->insn_flags & ISA_MIPS_R6) ? 9 : 12);
        switch (minor) {
        case LWL:
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            mips32_op = OPC_LWL;
            goto do_ld_lr;
        case SWL:
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            mips32_op = OPC_SWL;
            goto do_st_lr;
        case LWR:
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            mips32_op = OPC_LWR;
            goto do_ld_lr;
        case SWR:
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            mips32_op = OPC_SWR;
            goto do_st_lr;
#if defined(TARGET_MIPS64)
        case LDL:
            check_insn(ctx, ISA_MIPS3);
            check_mips_64(ctx);
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            mips32_op = OPC_LDL;
            goto do_ld_lr;
        case SDL:
            check_insn(ctx, ISA_MIPS3);
            check_mips_64(ctx);
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            mips32_op = OPC_SDL;
            goto do_st_lr;
        case LDR:
            check_insn(ctx, ISA_MIPS3);
            check_mips_64(ctx);
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            mips32_op = OPC_LDR;
            goto do_ld_lr;
        case SDR:
            check_insn(ctx, ISA_MIPS3);
            check_mips_64(ctx);
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            mips32_op = OPC_SDR;
            goto do_st_lr;
        case LWU:
            check_insn(ctx, ISA_MIPS3);
            check_mips_64(ctx);
            mips32_op = OPC_LWU;
            goto do_ld_lr;
        case LLD:
            check_insn(ctx, ISA_MIPS3);
            check_mips_64(ctx);
            mips32_op = OPC_LLD;
            goto do_ld_lr;
#endif
        case LL:
            mips32_op = OPC_LL;
            goto do_ld_lr;
        do_ld_lr:
            gen_ld(ctx, mips32_op, rt, rs, offset);
            break;
        do_st_lr:
            gen_st(ctx, mips32_op, rt, rs, offset);
            break;
        case SC:
            gen_st_cond(ctx, rt, rs, offset, MO_TESL, false, OPC_SC);
            break;
#if defined(TARGET_MIPS64)
        case SCD:
            check_insn(ctx, ISA_MIPS3);
            check_mips_64(ctx);
            gen_st_cond(ctx, rt, rs, offset, MO_TEQ, false, OPC_SCD);
            break;
#endif
        case LD_EVA:
            if (!ctx->eva) {
                MIPS_INVAL("pool32c ld-eva");
                gen_reserved_instruction(ctx);
                break;
            }
            check_cp0_enabled(ctx);

            minor2 = (ctx->opcode >> 9) & 0x7;
            offset = sextract32(ctx->opcode, 0, 9);
            switch (minor2) {
            case LBUE:
                mips32_op = OPC_LBUE;
                goto do_ld_lr;
            case LHUE:
                mips32_op = OPC_LHUE;
                goto do_ld_lr;
            case LWLE:
                check_insn_opc_removed(ctx, ISA_MIPS_R6);
                mips32_op = OPC_LWLE;
                goto do_ld_lr;
            case LWRE:
                check_insn_opc_removed(ctx, ISA_MIPS_R6);
                mips32_op = OPC_LWRE;
                goto do_ld_lr;
            case LBE:
                mips32_op = OPC_LBE;
                goto do_ld_lr;
            case LHE:
                mips32_op = OPC_LHE;
                goto do_ld_lr;
            case LLE:
                mips32_op = OPC_LLE;
                goto do_ld_lr;
            case LWE:
                mips32_op = OPC_LWE;
                goto do_ld_lr;
            };
            break;
        case ST_EVA:
            if (!ctx->eva) {
                MIPS_INVAL("pool32c st-eva");
                gen_reserved_instruction(ctx);
                break;
            }
            check_cp0_enabled(ctx);

            minor2 = (ctx->opcode >> 9) & 0x7;
            offset = sextract32(ctx->opcode, 0, 9);
            switch (minor2) {
            case SWLE:
                check_insn_opc_removed(ctx, ISA_MIPS_R6);
                mips32_op = OPC_SWLE;
                goto do_st_lr;
            case SWRE:
                check_insn_opc_removed(ctx, ISA_MIPS_R6);
                mips32_op = OPC_SWRE;
                goto do_st_lr;
            case PREFE:
                /* Treat as no-op */
                if ((ctx->insn_flags & ISA_MIPS_R6) && (rt >= 24)) {
                    /* hint codes 24-31 are reserved and signal RI */
                    generate_exception(ctx, EXCP_RI);
                }
                break;
            case CACHEE:
                /* Treat as no-op */
                if (ctx->hflags & MIPS_HFLAG_ITC_CACHE) {
                    gen_cache_operation(ctx, rt, rs, offset);
                }
                break;
            case SBE:
                mips32_op = OPC_SBE;
                goto do_st_lr;
            case SHE:
                mips32_op = OPC_SHE;
                goto do_st_lr;
            case SCE:
                gen_st_cond(ctx, rt, rs, offset, MO_TESL, true, OPC_SCE);
                break;
            case SWE:
                mips32_op = OPC_SWE;
                goto do_st_lr;
            };
            break;
        case PREF:
            /* Treat as no-op */
            if ((ctx->insn_flags & ISA_MIPS_R6) && (rt >= 24)) {
                /* hint codes 24-31 are reserved and signal RI */
                generate_exception(ctx, EXCP_RI);
            }
            break;
        default:
            MIPS_INVAL("pool32c");
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case ADDI32: /* AUI, LUI */
        if (ctx->insn_flags & ISA_MIPS_R6) {
            /* AUI, LUI */
            gen_logic_imm(ctx, OPC_LUI, rt, rs, imm);
        } else {
            /* ADDI32 */
            mips32_op = OPC_ADDI;
            goto do_addi;
        }
        break;
    case ADDIU32:
        mips32_op = OPC_ADDIU;
    do_addi:
        gen_arith_imm(ctx, mips32_op, rt, rs, imm);
        break;

        /* Logical operations */
    case ORI32:
        mips32_op = OPC_ORI;
        goto do_logici;
    case XORI32:
        mips32_op = OPC_XORI;
        goto do_logici;
    case ANDI32:
        mips32_op = OPC_ANDI;
    do_logici:
        gen_logic_imm(ctx, mips32_op, rt, rs, imm);
        break;

        /* Set less than immediate */
    case SLTI32:
        mips32_op = OPC_SLTI;
        goto do_slti;
    case SLTIU32:
        mips32_op = OPC_SLTIU;
    do_slti:
        gen_slt_imm(ctx, mips32_op, rt, rs, imm);
        break;
    case JALX32:
        check_insn_opc_removed(ctx, ISA_MIPS_R6);
        offset = (int32_t)(ctx->opcode & 0x3FFFFFF) << 2;
        gen_compute_branch(ctx, OPC_JALX, 4, rt, rs, offset, 4);
        ctx->hflags |= MIPS_HFLAG_BDS_STRICT;
        break;
    case JALS32: /* BOVC, BEQC, BEQZALC */
        if (ctx->insn_flags & ISA_MIPS_R6) {
            if (rs >= rt) {
                /* BOVC */
                mips32_op = OPC_BOVC;
            } else if (rs < rt && rs == 0) {
                /* BEQZALC */
                mips32_op = OPC_BEQZALC;
            } else {
                /* BEQC */
                mips32_op = OPC_BEQC;
            }
            gen_compute_compact_branch(ctx, mips32_op, rs, rt, imm << 1);
        } else {
            /* JALS32 */
            offset = (int32_t)(ctx->opcode & 0x3FFFFFF) << 1;
            gen_compute_branch(ctx, OPC_JAL, 4, rt, rs, offset, 2);
            ctx->hflags |= MIPS_HFLAG_BDS_STRICT;
        }
        break;
    case BEQ32: /* BC */
        if (ctx->insn_flags & ISA_MIPS_R6) {
            /* BC */
            gen_compute_compact_branch(ctx, OPC_BC, 0, 0,
                                       sextract32(ctx->opcode << 1, 0, 27));
        } else {
            /* BEQ32 */
            gen_compute_branch(ctx, OPC_BEQ, 4, rt, rs, imm << 1, 4);
        }
        break;
    case BNE32: /* BALC */
        if (ctx->insn_flags & ISA_MIPS_R6) {
            /* BALC */
            gen_compute_compact_branch(ctx, OPC_BALC, 0, 0,
                                       sextract32(ctx->opcode << 1, 0, 27));
        } else {
            /* BNE32 */
            gen_compute_branch(ctx, OPC_BNE, 4, rt, rs, imm << 1, 4);
        }
        break;
    case J32: /* BGTZC, BLTZC, BLTC */
        if (ctx->insn_flags & ISA_MIPS_R6) {
            if (rs == 0 && rt != 0) {
                /* BGTZC */
                mips32_op = OPC_BGTZC;
            } else if (rs != 0 && rt != 0 && rs == rt) {
                /* BLTZC */
                mips32_op = OPC_BLTZC;
            } else {
                /* BLTC */
                mips32_op = OPC_BLTC;
            }
            gen_compute_compact_branch(ctx, mips32_op, rs, rt, imm << 1);
        } else {
            /* J32 */
            gen_compute_branch(ctx, OPC_J, 4, rt, rs,
                               (int32_t)(ctx->opcode & 0x3FFFFFF) << 1, 4);
        }
        break;
    case JAL32: /* BLEZC, BGEZC, BGEC */
        if (ctx->insn_flags & ISA_MIPS_R6) {
            if (rs == 0 && rt != 0) {
                /* BLEZC */
                mips32_op = OPC_BLEZC;
            } else if (rs != 0 && rt != 0 && rs == rt) {
                /* BGEZC */
                mips32_op = OPC_BGEZC;
            } else {
                /* BGEC */
                mips32_op = OPC_BGEC;
            }
            gen_compute_compact_branch(ctx, mips32_op, rs, rt, imm << 1);
        } else {
            /* JAL32 */
            gen_compute_branch(ctx, OPC_JAL, 4, rt, rs,
                               (int32_t)(ctx->opcode & 0x3FFFFFF) << 1, 4);
            ctx->hflags |= MIPS_HFLAG_BDS_STRICT;
        }
        break;
        /* Floating point (COP1) */
    case LWC132:
        mips32_op = OPC_LWC1;
        goto do_cop1;
    case LDC132:
        mips32_op = OPC_LDC1;
        goto do_cop1;
    case SWC132:
        mips32_op = OPC_SWC1;
        goto do_cop1;
    case SDC132:
        mips32_op = OPC_SDC1;
    do_cop1:
        gen_cop1_ldst(ctx, mips32_op, rt, rs, imm);
        break;
    case ADDIUPC: /* PCREL: ADDIUPC, AUIPC, ALUIPC, LWPC */
        if (ctx->insn_flags & ISA_MIPS_R6) {
            /* PCREL: ADDIUPC, AUIPC, ALUIPC, LWPC */
            switch ((ctx->opcode >> 16) & 0x1f) {
            case ADDIUPC_00:
            case ADDIUPC_01:
            case ADDIUPC_02:
            case ADDIUPC_03:
            case ADDIUPC_04:
            case ADDIUPC_05:
            case ADDIUPC_06:
            case ADDIUPC_07:
                gen_pcrel(ctx, OPC_ADDIUPC, ctx->base.pc_next & ~0x3, rt);
                break;
            case AUIPC:
                gen_pcrel(ctx, OPC_AUIPC, ctx->base.pc_next, rt);
                break;
            case ALUIPC:
                gen_pcrel(ctx, OPC_ALUIPC, ctx->base.pc_next, rt);
                break;
            case LWPC_08:
            case LWPC_09:
            case LWPC_0A:
            case LWPC_0B:
            case LWPC_0C:
            case LWPC_0D:
            case LWPC_0E:
            case LWPC_0F:
                gen_pcrel(ctx, R6_OPC_LWPC, ctx->base.pc_next & ~0x3, rt);
                break;
            default:
                generate_exception(ctx, EXCP_RI);
                break;
            }
        } else {
            /* ADDIUPC */
            int reg = mmreg(ZIMM(ctx->opcode, 23, 3));
            offset = SIMM(ctx->opcode, 0, 23) << 2;

            gen_addiupc(ctx, reg, offset, 0, 0);
        }
        break;
    case BNVC: /* BNEC, BNEZALC */
        check_insn(ctx, ISA_MIPS_R6);
        if (rs >= rt) {
            /* BNVC */
            mips32_op = OPC_BNVC;
        } else if (rs < rt && rs == 0) {
            /* BNEZALC */
            mips32_op = OPC_BNEZALC;
        } else {
            /* BNEC */
            mips32_op = OPC_BNEC;
        }
        gen_compute_compact_branch(ctx, mips32_op, rs, rt, imm << 1);
        break;
    case R6_BNEZC: /* JIALC */
        check_insn(ctx, ISA_MIPS_R6);
        if (rt != 0) {
            /* BNEZC */
            gen_compute_compact_branch(ctx, OPC_BNEZC, rt, 0,
                                       sextract32(ctx->opcode << 1, 0, 22));
        } else {
            /* JIALC */
            gen_compute_compact_branch(ctx, OPC_JIALC, 0, rs, imm);
        }
        break;
    case R6_BEQZC: /* JIC */
        check_insn(ctx, ISA_MIPS_R6);
        if (rt != 0) {
            /* BEQZC */
            gen_compute_compact_branch(ctx, OPC_BEQZC, rt, 0,
                                       sextract32(ctx->opcode << 1, 0, 22));
        } else {
            /* JIC */
            gen_compute_compact_branch(ctx, OPC_JIC, 0, rs, imm);
        }
        break;
    case BLEZALC: /* BGEZALC, BGEUC */
        check_insn(ctx, ISA_MIPS_R6);
        if (rs == 0 && rt != 0) {
            /* BLEZALC */
            mips32_op = OPC_BLEZALC;
        } else if (rs != 0 && rt != 0 && rs == rt) {
            /* BGEZALC */
            mips32_op = OPC_BGEZALC;
        } else {
            /* BGEUC */
            mips32_op = OPC_BGEUC;
        }
        gen_compute_compact_branch(ctx, mips32_op, rs, rt, imm << 1);
        break;
    case BGTZALC: /* BLTZALC, BLTUC */
        check_insn(ctx, ISA_MIPS_R6);
        if (rs == 0 && rt != 0) {
            /* BGTZALC */
            mips32_op = OPC_BGTZALC;
        } else if (rs != 0 && rt != 0 && rs == rt) {
            /* BLTZALC */
            mips32_op = OPC_BLTZALC;
        } else {
            /* BLTUC */
            mips32_op = OPC_BLTUC;
        }
        gen_compute_compact_branch(ctx, mips32_op, rs, rt, imm << 1);
        break;
        /* Loads and stores */
    case LB32:
        mips32_op = OPC_LB;
        goto do_ld;
    case LBU32:
        mips32_op = OPC_LBU;
        goto do_ld;
    case LH32:
        mips32_op = OPC_LH;
        goto do_ld;
    case LHU32:
        mips32_op = OPC_LHU;
        goto do_ld;
    case LW32:
        mips32_op = OPC_LW;
        goto do_ld;
#ifdef TARGET_MIPS64
    case LD32:
        check_insn(ctx, ISA_MIPS3);
        check_mips_64(ctx);
        mips32_op = OPC_LD;
        goto do_ld;
    case SD32:
        check_insn(ctx, ISA_MIPS3);
        check_mips_64(ctx);
        mips32_op = OPC_SD;
        goto do_st;
#endif
    case SB32:
        mips32_op = OPC_SB;
        goto do_st;
    case SH32:
        mips32_op = OPC_SH;
        goto do_st;
    case SW32:
        mips32_op = OPC_SW;
        goto do_st;
    do_ld:
        gen_ld(ctx, mips32_op, rt, rs, imm);
        break;
    do_st:
        gen_st(ctx, mips32_op, rt, rs, imm);
        break;
    default:
        gen_reserved_instruction(ctx);
        break;
    }
}

static int decode_micromips_opc(CPUMIPSState *env, DisasContext *ctx)
{
    uint32_t op;

    /* make sure instructions are on a halfword boundary */
    if (ctx->base.pc_next & 0x1) {
        env->CP0_BadVAddr = ctx->base.pc_next;
        generate_exception_end(ctx, EXCP_AdEL);
        return 2;
    }

    op = (ctx->opcode >> 10) & 0x3f;
    /* Enforce properly-sized instructions in a delay slot */
    if (ctx->hflags & MIPS_HFLAG_BDS_STRICT) {
        switch (op & 0x7) { /* MSB-3..MSB-5 */
        case 0:
        /* POOL32A, POOL32B, POOL32I, POOL32C */
        case 4:
        /* ADDI32, ADDIU32, ORI32, XORI32, SLTI32, SLTIU32, ANDI32, JALX32 */
        case 5:
        /* LBU32, LHU32, POOL32F, JALS32, BEQ32, BNE32, J32, JAL32 */
        case 6:
        /* SB32, SH32, ADDIUPC, SWC132, SDC132, SW32 */
        case 7:
        /* LB32, LH32, LWC132, LDC132, LW32 */
            if (ctx->hflags & MIPS_HFLAG_BDS16) {
                gen_reserved_instruction(ctx);
                return 2;
            }
            break;
        case 1:
        /* POOL16A, POOL16B, POOL16C, LWGP16, POOL16F */
        case 2:
        /* LBU16, LHU16, LWSP16, LW16, SB16, SH16, SWSP16, SW16 */
        case 3:
        /* MOVE16, ANDI16, POOL16D, POOL16E, BEQZ16, BNEZ16, B16, LI16 */
            if (ctx->hflags & MIPS_HFLAG_BDS32) {
                gen_reserved_instruction(ctx);
                return 2;
            }
            break;
        }
    }

    switch (op) {
    case POOL16A:
        {
            int rd = mmreg(uMIPS_RD(ctx->opcode));
            int rs1 = mmreg(uMIPS_RS1(ctx->opcode));
            int rs2 = mmreg(uMIPS_RS2(ctx->opcode));
            uint32_t opc = 0;

            switch (ctx->opcode & 0x1) {
            case ADDU16:
                opc = OPC_ADDU;
                break;
            case SUBU16:
                opc = OPC_SUBU;
                break;
            }
            if (ctx->insn_flags & ISA_MIPS_R6) {
                /*
                 * In the Release 6, the register number location in
                 * the instruction encoding has changed.
                 */
                gen_arith(ctx, opc, rs1, rd, rs2);
            } else {
                gen_arith(ctx, opc, rd, rs1, rs2);
            }
        }
        break;
    case POOL16B:
        {
            int rd = mmreg(uMIPS_RD(ctx->opcode));
            int rs = mmreg(uMIPS_RS(ctx->opcode));
            int amount = (ctx->opcode >> 1) & 0x7;
            uint32_t opc = 0;
            amount = amount == 0 ? 8 : amount;

            switch (ctx->opcode & 0x1) {
            case SLL16:
                opc = OPC_SLL;
                break;
            case SRL16:
                opc = OPC_SRL;
                break;
            }

            gen_shift_imm(ctx, opc, rd, rs, amount);
        }
        break;
    case POOL16C:
        if (ctx->insn_flags & ISA_MIPS_R6) {
            gen_pool16c_r6_insn(ctx);
        } else {
            gen_pool16c_insn(ctx);
        }
        break;
    case LWGP16:
        {
            int rd = mmreg(uMIPS_RD(ctx->opcode));
            int rb = 28;            /* GP */
            int16_t offset = SIMM(ctx->opcode, 0, 7) << 2;

            gen_ld(ctx, OPC_LW, rd, rb, offset);
        }
        break;
    case POOL16F:
        check_insn_opc_removed(ctx, ISA_MIPS_R6);
        if (ctx->opcode & 1) {
            gen_reserved_instruction(ctx);
        } else {
            /* MOVEP */
            int enc_dest = uMIPS_RD(ctx->opcode);
            int enc_rt = uMIPS_RS2(ctx->opcode);
            int enc_rs = uMIPS_RS1(ctx->opcode);
            gen_movep(ctx, enc_dest, enc_rt, enc_rs);
        }
        break;
    case LBU16:
        {
            int rd = mmreg(uMIPS_RD(ctx->opcode));
            int rb = mmreg(uMIPS_RS(ctx->opcode));
            int16_t offset = ZIMM(ctx->opcode, 0, 4);
            offset = (offset == 0xf ? -1 : offset);

            gen_ld(ctx, OPC_LBU, rd, rb, offset);
        }
        break;
    case LHU16:
        {
            int rd = mmreg(uMIPS_RD(ctx->opcode));
            int rb = mmreg(uMIPS_RS(ctx->opcode));
            int16_t offset = ZIMM(ctx->opcode, 0, 4) << 1;

            gen_ld(ctx, OPC_LHU, rd, rb, offset);
        }
        break;
    case LWSP16:
        {
            int rd = (ctx->opcode >> 5) & 0x1f;
            int rb = 29;            /* SP */
            int16_t offset = ZIMM(ctx->opcode, 0, 5) << 2;

            gen_ld(ctx, OPC_LW, rd, rb, offset);
        }
        break;
    case LW16:
        {
            int rd = mmreg(uMIPS_RD(ctx->opcode));
            int rb = mmreg(uMIPS_RS(ctx->opcode));
            int16_t offset = ZIMM(ctx->opcode, 0, 4) << 2;

            gen_ld(ctx, OPC_LW, rd, rb, offset);
        }
        break;
    case SB16:
        {
            int rd = mmreg2(uMIPS_RD(ctx->opcode));
            int rb = mmreg(uMIPS_RS(ctx->opcode));
            int16_t offset = ZIMM(ctx->opcode, 0, 4);

            gen_st(ctx, OPC_SB, rd, rb, offset);
        }
        break;
    case SH16:
        {
            int rd = mmreg2(uMIPS_RD(ctx->opcode));
            int rb = mmreg(uMIPS_RS(ctx->opcode));
            int16_t offset = ZIMM(ctx->opcode, 0, 4) << 1;

            gen_st(ctx, OPC_SH, rd, rb, offset);
        }
        break;
    case SWSP16:
        {
            int rd = (ctx->opcode >> 5) & 0x1f;
            int rb = 29;            /* SP */
            int16_t offset = ZIMM(ctx->opcode, 0, 5) << 2;

            gen_st(ctx, OPC_SW, rd, rb, offset);
        }
        break;
    case SW16:
        {
            int rd = mmreg2(uMIPS_RD(ctx->opcode));
            int rb = mmreg(uMIPS_RS(ctx->opcode));
            int16_t offset = ZIMM(ctx->opcode, 0, 4) << 2;

            gen_st(ctx, OPC_SW, rd, rb, offset);
        }
        break;
    case MOVE16:
        {
            int rd = uMIPS_RD5(ctx->opcode);
            int rs = uMIPS_RS5(ctx->opcode);

            gen_arith(ctx, OPC_ADDU, rd, rs, 0);
        }
        break;
    case ANDI16:
        gen_andi16(ctx);
        break;
    case POOL16D:
        switch (ctx->opcode & 0x1) {
        case ADDIUS5:
            gen_addius5(ctx);
            break;
        case ADDIUSP:
            gen_addiusp(ctx);
            break;
        }
        break;
    case POOL16E:
        switch (ctx->opcode & 0x1) {
        case ADDIUR2:
            gen_addiur2(ctx);
            break;
        case ADDIUR1SP:
            gen_addiur1sp(ctx);
            break;
        }
        break;
    case B16: /* BC16 */
        gen_compute_branch(ctx, OPC_BEQ, 2, 0, 0,
                           sextract32(ctx->opcode, 0, 10) << 1,
                           (ctx->insn_flags & ISA_MIPS_R6) ? 0 : 4);
        break;
    case BNEZ16: /* BNEZC16 */
    case BEQZ16: /* BEQZC16 */
        gen_compute_branch(ctx, op == BNEZ16 ? OPC_BNE : OPC_BEQ, 2,
                           mmreg(uMIPS_RD(ctx->opcode)),
                           0, sextract32(ctx->opcode, 0, 7) << 1,
                           (ctx->insn_flags & ISA_MIPS_R6) ? 0 : 4);

        break;
    case LI16:
        {
            int reg = mmreg(uMIPS_RD(ctx->opcode));
            int imm = ZIMM(ctx->opcode, 0, 7);

            imm = (imm == 0x7f ? -1 : imm);
            tcg_gen_movi_tl(cpu_gpr[reg], imm);
            gen_log_instr_gpr_update(ctx, reg);
        }
        break;
    case RES_29:
    case RES_31:
    case RES_39:
        gen_reserved_instruction(ctx);
        break;
    default:
        decode_micromips32_opc(env, ctx);
        return 4;
    }

    return 2;
}

// Disable nanoMIPS for TARGET_CHERI since to avoid having to add $ddc checks
#ifndef TARGET_CHERI
/*
 *
 * nanoMIPS opcodes
 *
 */

/* MAJOR, P16, and P32 pools opcodes */
enum {
    NM_P_ADDIU      = 0x00,
    NM_ADDIUPC      = 0x01,
    NM_MOVE_BALC    = 0x02,
    NM_P16_MV       = 0x04,
    NM_LW16         = 0x05,
    NM_BC16         = 0x06,
    NM_P16_SR       = 0x07,

    NM_POOL32A      = 0x08,
    NM_P_BAL        = 0x0a,
    NM_P16_SHIFT    = 0x0c,
    NM_LWSP16       = 0x0d,
    NM_BALC16       = 0x0e,
    NM_P16_4X4      = 0x0f,

    NM_P_GP_W       = 0x10,
    NM_P_GP_BH      = 0x11,
    NM_P_J          = 0x12,
    NM_P16C         = 0x14,
    NM_LWGP16       = 0x15,
    NM_P16_LB       = 0x17,

    NM_P48I         = 0x18,
    NM_P16_A1       = 0x1c,
    NM_LW4X4        = 0x1d,
    NM_P16_LH       = 0x1f,

    NM_P_U12        = 0x20,
    NM_P_LS_U12     = 0x21,
    NM_P_BR1        = 0x22,
    NM_P16_A2       = 0x24,
    NM_SW16         = 0x25,
    NM_BEQZC16      = 0x26,

    NM_POOL32F      = 0x28,
    NM_P_LS_S9      = 0x29,
    NM_P_BR2        = 0x2a,

    NM_P16_ADDU     = 0x2c,
    NM_SWSP16       = 0x2d,
    NM_BNEZC16      = 0x2e,
    NM_MOVEP        = 0x2f,

    NM_POOL32S      = 0x30,
    NM_P_BRI        = 0x32,
    NM_LI16         = 0x34,
    NM_SWGP16       = 0x35,
    NM_P16_BR       = 0x36,

    NM_P_LUI        = 0x38,
    NM_ANDI16       = 0x3c,
    NM_SW4X4        = 0x3d,
    NM_MOVEPREV     = 0x3f,
};

/* POOL32A instruction pool */
enum {
    NM_POOL32A0    = 0x00,
    NM_SPECIAL2    = 0x01,
    NM_COP2_1      = 0x02,
    NM_UDI         = 0x03,
    NM_POOL32A5    = 0x05,
    NM_POOL32A7    = 0x07,
};

/* P.GP.W instruction pool */
enum {
    NM_ADDIUGP_W = 0x00,
    NM_LWGP      = 0x02,
    NM_SWGP      = 0x03,
};

/* P48I instruction pool */
enum {
    NM_LI48        = 0x00,
    NM_ADDIU48     = 0x01,
    NM_ADDIUGP48   = 0x02,
    NM_ADDIUPC48   = 0x03,
    NM_LWPC48      = 0x0b,
    NM_SWPC48      = 0x0f,
};

/* P.U12 instruction pool */
enum {
    NM_ORI      = 0x00,
    NM_XORI     = 0x01,
    NM_ANDI     = 0x02,
    NM_P_SR     = 0x03,
    NM_SLTI     = 0x04,
    NM_SLTIU    = 0x05,
    NM_SEQI     = 0x06,
    NM_ADDIUNEG = 0x08,
    NM_P_SHIFT  = 0x0c,
    NM_P_ROTX   = 0x0d,
    NM_P_INS    = 0x0e,
    NM_P_EXT    = 0x0f,
};

/* POOL32F instruction pool */
enum {
    NM_POOL32F_0   = 0x00,
    NM_POOL32F_3   = 0x03,
    NM_POOL32F_5   = 0x05,
};

/* POOL32S instruction pool */
enum {
    NM_POOL32S_0   = 0x00,
    NM_POOL32S_4   = 0x04,
};

/* P.LUI instruction pool */
enum {
    NM_LUI      = 0x00,
    NM_ALUIPC   = 0x01,
};

/* P.GP.BH instruction pool */
enum {
    NM_LBGP      = 0x00,
    NM_SBGP      = 0x01,
    NM_LBUGP     = 0x02,
    NM_ADDIUGP_B = 0x03,
    NM_P_GP_LH   = 0x04,
    NM_P_GP_SH   = 0x05,
    NM_P_GP_CP1  = 0x06,
};

/* P.LS.U12 instruction pool */
enum {
    NM_LB        = 0x00,
    NM_SB        = 0x01,
    NM_LBU       = 0x02,
    NM_P_PREFU12 = 0x03,
    NM_LH        = 0x04,
    NM_SH        = 0x05,
    NM_LHU       = 0x06,
    NM_LWU       = 0x07,
    NM_LW        = 0x08,
    NM_SW        = 0x09,
    NM_LWC1      = 0x0a,
    NM_SWC1      = 0x0b,
    NM_LDC1      = 0x0e,
    NM_SDC1      = 0x0f,
};

/* P.LS.S9 instruction pool */
enum {
    NM_P_LS_S0         = 0x00,
    NM_P_LS_S1         = 0x01,
    NM_P_LS_E0         = 0x02,
    NM_P_LS_WM         = 0x04,
    NM_P_LS_UAWM       = 0x05,
};

/* P.BAL instruction pool */
enum {
    NM_BC       = 0x00,
    NM_BALC     = 0x01,
};

/* P.J instruction pool */
enum {
    NM_JALRC    = 0x00,
    NM_JALRC_HB = 0x01,
    NM_P_BALRSC = 0x08,
};

/* P.BR1 instruction pool */
enum {
    NM_BEQC     = 0x00,
    NM_P_BR3A   = 0x01,
    NM_BGEC     = 0x02,
    NM_BGEUC    = 0x03,
};

/* P.BR2 instruction pool */
enum {
    NM_BNEC     = 0x00,
    NM_BLTC     = 0x02,
    NM_BLTUC    = 0x03,
};

/* P.BRI instruction pool */
enum {
    NM_BEQIC    = 0x00,
    NM_BBEQZC   = 0x01,
    NM_BGEIC    = 0x02,
    NM_BGEIUC   = 0x03,
    NM_BNEIC    = 0x04,
    NM_BBNEZC   = 0x05,
    NM_BLTIC    = 0x06,
    NM_BLTIUC   = 0x07,
};

/* P16.SHIFT instruction pool */
enum {
    NM_SLL16    = 0x00,
    NM_SRL16    = 0x01,
};

/* POOL16C instruction pool */
enum {
    NM_POOL16C_0  = 0x00,
    NM_LWXS16     = 0x01,
};

/* P16.A1 instruction pool */
enum {
    NM_ADDIUR1SP = 0x01,
};

/* P16.A2 instruction pool */
enum {
    NM_ADDIUR2  = 0x00,
    NM_P_ADDIURS5  = 0x01,
};

/* P16.ADDU instruction pool */
enum {
    NM_ADDU16     = 0x00,
    NM_SUBU16     = 0x01,
};

/* P16.SR instruction pool */
enum {
    NM_SAVE16        = 0x00,
    NM_RESTORE_JRC16 = 0x01,
};

/* P16.4X4 instruction pool */
enum {
    NM_ADDU4X4      = 0x00,
    NM_MUL4X4       = 0x01,
};

/* P16.LB instruction pool */
enum {
    NM_LB16       = 0x00,
    NM_SB16       = 0x01,
    NM_LBU16      = 0x02,
};

/* P16.LH  instruction pool */
enum {
    NM_LH16     = 0x00,
    NM_SH16     = 0x01,
    NM_LHU16    = 0x02,
};

/* P.RI instruction pool */
enum {
    NM_SIGRIE       = 0x00,
    NM_P_SYSCALL    = 0x01,
    NM_BREAK        = 0x02,
    NM_SDBBP        = 0x03,
};

/* POOL32A0 instruction pool */
enum {
    NM_P_TRAP   = 0x00,
    NM_SEB      = 0x01,
    NM_SLLV     = 0x02,
    NM_MUL      = 0x03,
    NM_MFC0     = 0x06,
    NM_MFHC0    = 0x07,
    NM_SEH      = 0x09,
    NM_SRLV     = 0x0a,
    NM_MUH      = 0x0b,
    NM_MTC0     = 0x0e,
    NM_MTHC0    = 0x0f,
    NM_SRAV     = 0x12,
    NM_MULU     = 0x13,
    NM_ROTRV    = 0x1a,
    NM_MUHU     = 0x1b,
    NM_ADD      = 0x22,
    NM_DIV      = 0x23,
    NM_ADDU     = 0x2a,
    NM_MOD      = 0x2b,
    NM_SUB      = 0x32,
    NM_DIVU     = 0x33,
    NM_RDHWR    = 0x38,
    NM_SUBU     = 0x3a,
    NM_MODU     = 0x3b,
    NM_P_CMOVE  = 0x42,
    NM_FORK     = 0x45,
    NM_MFTR     = 0x46,
    NM_MFHTR    = 0x47,
    NM_AND      = 0x4a,
    NM_YIELD    = 0x4d,
    NM_MTTR     = 0x4e,
    NM_MTHTR    = 0x4f,
    NM_OR       = 0x52,
    NM_D_E_MT_VPE = 0x56,
    NM_NOR      = 0x5a,
    NM_XOR      = 0x62,
    NM_SLT      = 0x6a,
    NM_P_SLTU   = 0x72,
    NM_SOV      = 0x7a,
};

/* CRC32 instruction pool */
enum {
    NM_CRC32B   = 0x00,
    NM_CRC32H   = 0x01,
    NM_CRC32W   = 0x02,
    NM_CRC32CB  = 0x04,
    NM_CRC32CH  = 0x05,
    NM_CRC32CW  = 0x06,
};

/* POOL32A5 instruction pool */
enum {
    NM_CMP_EQ_PH        = 0x00,
    NM_CMP_LT_PH        = 0x08,
    NM_CMP_LE_PH        = 0x10,
    NM_CMPGU_EQ_QB      = 0x18,
    NM_CMPGU_LT_QB      = 0x20,
    NM_CMPGU_LE_QB      = 0x28,
    NM_CMPGDU_EQ_QB     = 0x30,
    NM_CMPGDU_LT_QB     = 0x38,
    NM_CMPGDU_LE_QB     = 0x40,
    NM_CMPU_EQ_QB       = 0x48,
    NM_CMPU_LT_QB       = 0x50,
    NM_CMPU_LE_QB       = 0x58,
    NM_ADDQ_S_W         = 0x60,
    NM_SUBQ_S_W         = 0x68,
    NM_ADDSC            = 0x70,
    NM_ADDWC            = 0x78,

    NM_ADDQ_S_PH   = 0x01,
    NM_ADDQH_R_PH  = 0x09,
    NM_ADDQH_R_W   = 0x11,
    NM_ADDU_S_QB   = 0x19,
    NM_ADDU_S_PH   = 0x21,
    NM_ADDUH_R_QB  = 0x29,
    NM_SHRAV_R_PH  = 0x31,
    NM_SHRAV_R_QB  = 0x39,
    NM_SUBQ_S_PH   = 0x41,
    NM_SUBQH_R_PH  = 0x49,
    NM_SUBQH_R_W   = 0x51,
    NM_SUBU_S_QB   = 0x59,
    NM_SUBU_S_PH   = 0x61,
    NM_SUBUH_R_QB  = 0x69,
    NM_SHLLV_S_PH  = 0x71,
    NM_PRECR_SRA_R_PH_W = 0x79,

    NM_MULEU_S_PH_QBL   = 0x12,
    NM_MULEU_S_PH_QBR   = 0x1a,
    NM_MULQ_RS_PH       = 0x22,
    NM_MULQ_S_PH        = 0x2a,
    NM_MULQ_RS_W        = 0x32,
    NM_MULQ_S_W         = 0x3a,
    NM_APPEND           = 0x42,
    NM_MODSUB           = 0x52,
    NM_SHRAV_R_W        = 0x5a,
    NM_SHRLV_PH         = 0x62,
    NM_SHRLV_QB         = 0x6a,
    NM_SHLLV_QB         = 0x72,
    NM_SHLLV_S_W        = 0x7a,

    NM_SHILO            = 0x03,

    NM_MULEQ_S_W_PHL    = 0x04,
    NM_MULEQ_S_W_PHR    = 0x0c,

    NM_MUL_S_PH         = 0x05,
    NM_PRECR_QB_PH      = 0x0d,
    NM_PRECRQ_QB_PH     = 0x15,
    NM_PRECRQ_PH_W      = 0x1d,
    NM_PRECRQ_RS_PH_W   = 0x25,
    NM_PRECRQU_S_QB_PH  = 0x2d,
    NM_PACKRL_PH        = 0x35,
    NM_PICK_QB          = 0x3d,
    NM_PICK_PH          = 0x45,

    NM_SHRA_R_W         = 0x5e,
    NM_SHRA_R_PH        = 0x66,
    NM_SHLL_S_PH        = 0x76,
    NM_SHLL_S_W         = 0x7e,

    NM_REPL_PH          = 0x07
};

/* POOL32A7 instruction pool */
enum {
    NM_P_LSX        = 0x00,
    NM_LSA          = 0x01,
    NM_EXTW         = 0x03,
    NM_POOL32AXF    = 0x07,
};

/* P.SR instruction pool */
enum {
    NM_PP_SR           = 0x00,
    NM_P_SR_F          = 0x01,
};

/* P.SHIFT instruction pool */
enum {
    NM_P_SLL        = 0x00,
    NM_SRL          = 0x02,
    NM_SRA          = 0x04,
    NM_ROTR         = 0x06,
};

/* P.ROTX instruction pool */
enum {
    NM_ROTX         = 0x00,
};

/* P.INS instruction pool */
enum {
    NM_INS          = 0x00,
};

/* P.EXT instruction pool */
enum {
    NM_EXT          = 0x00,
};

/* POOL32F_0 (fmt) instruction pool */
enum {
    NM_RINT_S              = 0x04,
    NM_RINT_D              = 0x44,
    NM_ADD_S               = 0x06,
    NM_SELEQZ_S            = 0x07,
    NM_SELEQZ_D            = 0x47,
    NM_CLASS_S             = 0x0c,
    NM_CLASS_D             = 0x4c,
    NM_SUB_S               = 0x0e,
    NM_SELNEZ_S            = 0x0f,
    NM_SELNEZ_D            = 0x4f,
    NM_MUL_S               = 0x16,
    NM_SEL_S               = 0x17,
    NM_SEL_D               = 0x57,
    NM_DIV_S               = 0x1e,
    NM_ADD_D               = 0x26,
    NM_SUB_D               = 0x2e,
    NM_MUL_D               = 0x36,
    NM_MADDF_S             = 0x37,
    NM_MADDF_D             = 0x77,
    NM_DIV_D               = 0x3e,
    NM_MSUBF_S             = 0x3f,
    NM_MSUBF_D             = 0x7f,
};

/* POOL32F_3  instruction pool */
enum {
    NM_MIN_FMT         = 0x00,
    NM_MAX_FMT         = 0x01,
    NM_MINA_FMT        = 0x04,
    NM_MAXA_FMT        = 0x05,
    NM_POOL32FXF       = 0x07,
};

/* POOL32F_5  instruction pool */
enum {
    NM_CMP_CONDN_S     = 0x00,
    NM_CMP_CONDN_D     = 0x02,
};

/* P.GP.LH instruction pool */
enum {
    NM_LHGP    = 0x00,
    NM_LHUGP   = 0x01,
};

/* P.GP.SH instruction pool */
enum {
    NM_SHGP    = 0x00,
};

/* P.GP.CP1 instruction pool */
enum {
    NM_LWC1GP       = 0x00,
    NM_SWC1GP       = 0x01,
    NM_LDC1GP       = 0x02,
    NM_SDC1GP       = 0x03,
};

/* P.LS.S0 instruction pool */
enum {
    NM_LBS9     = 0x00,
    NM_LHS9     = 0x04,
    NM_LWS9     = 0x08,
    NM_LDS9     = 0x0c,

    NM_SBS9     = 0x01,
    NM_SHS9     = 0x05,
    NM_SWS9     = 0x09,
    NM_SDS9     = 0x0d,

    NM_LBUS9    = 0x02,
    NM_LHUS9    = 0x06,
    NM_LWC1S9   = 0x0a,
    NM_LDC1S9   = 0x0e,

    NM_P_PREFS9 = 0x03,
    NM_LWUS9    = 0x07,
    NM_SWC1S9   = 0x0b,
    NM_SDC1S9   = 0x0f,
};

/* P.LS.S1 instruction pool */
enum {
    NM_ASET_ACLR = 0x02,
    NM_UALH      = 0x04,
    NM_UASH      = 0x05,
    NM_CACHE     = 0x07,
    NM_P_LL      = 0x0a,
    NM_P_SC      = 0x0b,
};

/* P.LS.E0 instruction pool */
enum {
    NM_LBE      = 0x00,
    NM_SBE      = 0x01,
    NM_LBUE     = 0x02,
    NM_P_PREFE  = 0x03,
    NM_LHE      = 0x04,
    NM_SHE      = 0x05,
    NM_LHUE     = 0x06,
    NM_CACHEE   = 0x07,
    NM_LWE      = 0x08,
    NM_SWE      = 0x09,
    NM_P_LLE    = 0x0a,
    NM_P_SCE    = 0x0b,
};

/* P.PREFE instruction pool */
enum {
    NM_SYNCIE   = 0x00,
    NM_PREFE    = 0x01,
};

/* P.LLE instruction pool */
enum {
    NM_LLE      = 0x00,
    NM_LLWPE    = 0x01,
};

/* P.SCE instruction pool */
enum {
    NM_SCE      = 0x00,
    NM_SCWPE    = 0x01,
};

/* P.LS.WM instruction pool */
enum {
    NM_LWM       = 0x00,
    NM_SWM       = 0x01,
};

/* P.LS.UAWM instruction pool */
enum {
    NM_UALWM       = 0x00,
    NM_UASWM       = 0x01,
};

/* P.BR3A instruction pool */
enum {
    NM_BC1EQZC          = 0x00,
    NM_BC1NEZC          = 0x01,
    NM_BC2EQZC          = 0x02,
    NM_BC2NEZC          = 0x03,
    NM_BPOSGE32C        = 0x04,
};

/* P16.RI instruction pool */
enum {
    NM_P16_SYSCALL  = 0x01,
    NM_BREAK16      = 0x02,
    NM_SDBBP16      = 0x03,
};

/* POOL16C_0 instruction pool */
enum {
    NM_POOL16C_00      = 0x00,
};

/* P16.JRC instruction pool */
enum {
    NM_JRC          = 0x00,
    NM_JALRC16      = 0x01,
};

/* P.SYSCALL instruction pool */
enum {
    NM_SYSCALL      = 0x00,
    NM_HYPCALL      = 0x01,
};

/* P.TRAP instruction pool */
enum {
    NM_TEQ          = 0x00,
    NM_TNE          = 0x01,
};

/* P.CMOVE instruction pool */
enum {
    NM_MOVZ            = 0x00,
    NM_MOVN            = 0x01,
};

/* POOL32Axf instruction pool */
enum {
    NM_POOL32AXF_1 = 0x01,
    NM_POOL32AXF_2 = 0x02,
    NM_POOL32AXF_4 = 0x04,
    NM_POOL32AXF_5 = 0x05,
    NM_POOL32AXF_7 = 0x07,
};

/* POOL32Axf_1 instruction pool */
enum {
    NM_POOL32AXF_1_0 = 0x00,
    NM_POOL32AXF_1_1 = 0x01,
    NM_POOL32AXF_1_3 = 0x03,
    NM_POOL32AXF_1_4 = 0x04,
    NM_POOL32AXF_1_5 = 0x05,
    NM_POOL32AXF_1_7 = 0x07,
};

/* POOL32Axf_2 instruction pool */
enum {
    NM_POOL32AXF_2_0_7     = 0x00,
    NM_POOL32AXF_2_8_15    = 0x01,
    NM_POOL32AXF_2_16_23   = 0x02,
    NM_POOL32AXF_2_24_31   = 0x03,
};

/* POOL32Axf_7 instruction pool */
enum {
    NM_SHRA_R_QB    = 0x0,
    NM_SHRL_PH      = 0x1,
    NM_REPL_QB      = 0x2,
};

/* POOL32Axf_1_0 instruction pool */
enum {
    NM_MFHI = 0x0,
    NM_MFLO = 0x1,
    NM_MTHI = 0x2,
    NM_MTLO = 0x3,
};

/* POOL32Axf_1_1 instruction pool */
enum {
    NM_MTHLIP = 0x0,
    NM_SHILOV = 0x1,
};

/* POOL32Axf_1_3 instruction pool */
enum {
    NM_RDDSP    = 0x0,
    NM_WRDSP    = 0x1,
    NM_EXTP     = 0x2,
    NM_EXTPDP   = 0x3,
};

/* POOL32Axf_1_4 instruction pool */
enum {
    NM_SHLL_QB  = 0x0,
    NM_SHRL_QB  = 0x1,
};

/* POOL32Axf_1_5 instruction pool */
enum {
    NM_MAQ_S_W_PHR   = 0x0,
    NM_MAQ_S_W_PHL   = 0x1,
    NM_MAQ_SA_W_PHR  = 0x2,
    NM_MAQ_SA_W_PHL  = 0x3,
};

/* POOL32Axf_1_7 instruction pool */
enum {
    NM_EXTR_W       = 0x0,
    NM_EXTR_R_W     = 0x1,
    NM_EXTR_RS_W    = 0x2,
    NM_EXTR_S_H     = 0x3,
};

/* POOL32Axf_2_0_7 instruction pool */
enum {
    NM_DPA_W_PH     = 0x0,
    NM_DPAQ_S_W_PH  = 0x1,
    NM_DPS_W_PH     = 0x2,
    NM_DPSQ_S_W_PH  = 0x3,
    NM_BALIGN       = 0x4,
    NM_MADD         = 0x5,
    NM_MULT         = 0x6,
    NM_EXTRV_W      = 0x7,
};

/* POOL32Axf_2_8_15 instruction pool */
enum {
    NM_DPAX_W_PH    = 0x0,
    NM_DPAQ_SA_L_W  = 0x1,
    NM_DPSX_W_PH    = 0x2,
    NM_DPSQ_SA_L_W  = 0x3,
    NM_MADDU        = 0x5,
    NM_MULTU        = 0x6,
    NM_EXTRV_R_W    = 0x7,
};

/* POOL32Axf_2_16_23 instruction pool */
enum {
    NM_DPAU_H_QBL       = 0x0,
    NM_DPAQX_S_W_PH     = 0x1,
    NM_DPSU_H_QBL       = 0x2,
    NM_DPSQX_S_W_PH     = 0x3,
    NM_EXTPV            = 0x4,
    NM_MSUB             = 0x5,
    NM_MULSA_W_PH       = 0x6,
    NM_EXTRV_RS_W       = 0x7,
};

/* POOL32Axf_2_24_31 instruction pool */
enum {
    NM_DPAU_H_QBR       = 0x0,
    NM_DPAQX_SA_W_PH    = 0x1,
    NM_DPSU_H_QBR       = 0x2,
    NM_DPSQX_SA_W_PH    = 0x3,
    NM_EXTPDPV          = 0x4,
    NM_MSUBU            = 0x5,
    NM_MULSAQ_S_W_PH    = 0x6,
    NM_EXTRV_S_H        = 0x7,
};

/* POOL32Axf_{4, 5} instruction pool */
enum {
    NM_CLO      = 0x25,
    NM_CLZ      = 0x2d,

    NM_TLBP     = 0x01,
    NM_TLBR     = 0x09,
    NM_TLBWI    = 0x11,
    NM_TLBWR    = 0x19,
    NM_TLBINV   = 0x03,
    NM_TLBINVF  = 0x0b,
    NM_DI       = 0x23,
    NM_EI       = 0x2b,
    NM_RDPGPR   = 0x70,
    NM_WRPGPR   = 0x78,
    NM_WAIT     = 0x61,
    NM_DERET    = 0x71,
    NM_ERETX    = 0x79,

    /* nanoMIPS DSP instructions */
    NM_ABSQ_S_QB        = 0x00,
    NM_ABSQ_S_PH        = 0x08,
    NM_ABSQ_S_W         = 0x10,
    NM_PRECEQ_W_PHL     = 0x28,
    NM_PRECEQ_W_PHR     = 0x30,
    NM_PRECEQU_PH_QBL   = 0x38,
    NM_PRECEQU_PH_QBR   = 0x48,
    NM_PRECEU_PH_QBL    = 0x58,
    NM_PRECEU_PH_QBR    = 0x68,
    NM_PRECEQU_PH_QBLA  = 0x39,
    NM_PRECEQU_PH_QBRA  = 0x49,
    NM_PRECEU_PH_QBLA   = 0x59,
    NM_PRECEU_PH_QBRA   = 0x69,
    NM_REPLV_PH         = 0x01,
    NM_REPLV_QB         = 0x09,
    NM_BITREV           = 0x18,
    NM_INSV             = 0x20,
    NM_RADDU_W_QB       = 0x78,

    NM_BITSWAP          = 0x05,
    NM_WSBH             = 0x3d,
};

/* PP.SR instruction pool */
enum {
    NM_SAVE         = 0x00,
    NM_RESTORE      = 0x02,
    NM_RESTORE_JRC  = 0x03,
};

/* P.SR.F instruction pool */
enum {
    NM_SAVEF        = 0x00,
    NM_RESTOREF     = 0x01,
};

/* P16.SYSCALL  instruction pool */
enum {
    NM_SYSCALL16     = 0x00,
    NM_HYPCALL16     = 0x01,
};

/* POOL16C_00 instruction pool */
enum {
    NM_NOT16           = 0x00,
    NM_XOR16           = 0x01,
    NM_AND16           = 0x02,
    NM_OR16            = 0x03,
};

/* PP.LSX and PP.LSXS instruction pool */
enum {
    NM_LBX      = 0x00,
    NM_LHX      = 0x04,
    NM_LWX      = 0x08,
    NM_LDX      = 0x0c,

    NM_SBX      = 0x01,
    NM_SHX      = 0x05,
    NM_SWX      = 0x09,
    NM_SDX      = 0x0d,

    NM_LBUX     = 0x02,
    NM_LHUX     = 0x06,
    NM_LWC1X    = 0x0a,
    NM_LDC1X    = 0x0e,

    NM_LWUX     = 0x07,
    NM_SWC1X    = 0x0b,
    NM_SDC1X    = 0x0f,

    NM_LHXS     = 0x04,
    NM_LWXS     = 0x08,
    NM_LDXS     = 0x0c,

    NM_SHXS     = 0x05,
    NM_SWXS     = 0x09,
    NM_SDXS     = 0x0d,

    NM_LHUXS    = 0x06,
    NM_LWC1XS   = 0x0a,
    NM_LDC1XS   = 0x0e,

    NM_LWUXS    = 0x07,
    NM_SWC1XS   = 0x0b,
    NM_SDC1XS   = 0x0f,
};

/* ERETx instruction pool */
enum {
    NM_ERET     = 0x00,
    NM_ERETNC   = 0x01,
};

/* POOL32FxF_{0, 1} insturction pool */
enum {
    NM_CFC1     = 0x40,
    NM_CTC1     = 0x60,
    NM_MFC1     = 0x80,
    NM_MTC1     = 0xa0,
    NM_MFHC1    = 0xc0,
    NM_MTHC1    = 0xe0,

    NM_CVT_S_PL = 0x84,
    NM_CVT_S_PU = 0xa4,

    NM_CVT_L_S     = 0x004,
    NM_CVT_L_D     = 0x104,
    NM_CVT_W_S     = 0x024,
    NM_CVT_W_D     = 0x124,

    NM_RSQRT_S     = 0x008,
    NM_RSQRT_D     = 0x108,

    NM_SQRT_S      = 0x028,
    NM_SQRT_D      = 0x128,

    NM_RECIP_S     = 0x048,
    NM_RECIP_D     = 0x148,

    NM_FLOOR_L_S   = 0x00c,
    NM_FLOOR_L_D   = 0x10c,

    NM_FLOOR_W_S   = 0x02c,
    NM_FLOOR_W_D   = 0x12c,

    NM_CEIL_L_S    = 0x04c,
    NM_CEIL_L_D    = 0x14c,
    NM_CEIL_W_S    = 0x06c,
    NM_CEIL_W_D    = 0x16c,
    NM_TRUNC_L_S   = 0x08c,
    NM_TRUNC_L_D   = 0x18c,
    NM_TRUNC_W_S   = 0x0ac,
    NM_TRUNC_W_D   = 0x1ac,
    NM_ROUND_L_S   = 0x0cc,
    NM_ROUND_L_D   = 0x1cc,
    NM_ROUND_W_S   = 0x0ec,
    NM_ROUND_W_D   = 0x1ec,

    NM_MOV_S       = 0x01,
    NM_MOV_D       = 0x81,
    NM_ABS_S       = 0x0d,
    NM_ABS_D       = 0x8d,
    NM_NEG_S       = 0x2d,
    NM_NEG_D       = 0xad,
    NM_CVT_D_S     = 0x04d,
    NM_CVT_D_W     = 0x0cd,
    NM_CVT_D_L     = 0x14d,
    NM_CVT_S_D     = 0x06d,
    NM_CVT_S_W     = 0x0ed,
    NM_CVT_S_L     = 0x16d,
};

/* P.LL instruction pool */
enum {
    NM_LL       = 0x00,
    NM_LLWP     = 0x01,
};

/* P.SC instruction pool */
enum {
    NM_SC       = 0x00,
    NM_SCWP     = 0x01,
};

/* P.DVP instruction pool */
enum {
    NM_DVP      = 0x00,
    NM_EVP      = 0x01,
};


/*
 *
 * nanoMIPS decoding engine
 *
 */


/* extraction utilities */

#define NANOMIPS_EXTRACT_RT3(op) ((op >> 7) & 0x7)
#define NANOMIPS_EXTRACT_RS3(op) ((op >> 4) & 0x7)
#define NANOMIPS_EXTRACT_RD3(op) ((op >> 1) & 0x7)
#define NANOMIPS_EXTRACT_RD5(op) ((op >> 5) & 0x1f)
#define NANOMIPS_EXTRACT_RS5(op) (op & 0x1f)

/* Implement nanoMIPS pseudocode decode_gpr(encoded_gpr, 'gpr3'). */
static inline int decode_gpr_gpr3(int r)
{
    static const int map[] = { 16, 17, 18, 19,  4,  5,  6,  7 };

    return map[r & 0x7];
}

/* Implement nanoMIPS pseudocode decode_gpr(encoded_gpr, 'gpr3.src.store'). */
static inline int decode_gpr_gpr3_src_store(int r)
{
    static const int map[] = {  0, 17, 18, 19,  4,  5,  6,  7 };

    return map[r & 0x7];
}

/* Implement nanoMIPS pseudocode decode_gpr(encoded_gpr, 'gpr4'). */
static inline int decode_gpr_gpr4(int r)
{
    static const int map[] = {  8,  9, 10, 11,  4,  5,  6,  7,
                               16, 17, 18, 19, 20, 21, 22, 23 };

    return map[r & 0xf];
}

/* Implement nanoMIPS pseudocode decode_gpr(encoded_gpr, 'gpr4.zero'). */
static inline int decode_gpr_gpr4_zero(int r)
{
    static const int map[] = {  8,  9, 10,  0,  4,  5,  6,  7,
                               16, 17, 18, 19, 20, 21, 22, 23 };

    return map[r & 0xf];
}


static void gen_adjust_sp(DisasContext *ctx, int u)
{
    gen_op_addr_addi(ctx, cpu_gpr[29], cpu_gpr[29], u);
    gen_log_instr_gpr_update(ctx, 29);
}

static void gen_save(DisasContext *ctx, uint8_t rt, uint8_t count,
                     uint8_t gp, uint16_t u)
{
    int counter = 0;
    TCGv va = tcg_temp_new();
    TCGv t0 = tcg_temp_new();

    while (counter != count) {
        bool use_gp = gp && (counter == count - 1);
        int this_rt = use_gp ? 28 : (rt & 0x10) | ((rt + counter) & 0x1f);
        int this_offset = -((counter + 1) << 2);
        gen_base_offset_addr(ctx, va, 29, this_offset);
        gen_load_gpr(t0, this_rt);
        tcg_gen_qemu_st_tl(t0, va, ctx->mem_idx,
                           (MO_TEUL | ctx->default_tcg_memop_mask));
        counter++;
    }

    /* adjust stack pointer */
    gen_adjust_sp(ctx, -u);

    tcg_temp_free(t0);
    tcg_temp_free(va);
}

static void gen_restore(DisasContext *ctx, uint8_t rt, uint8_t count,
                        uint8_t gp, uint16_t u)
{
    int counter = 0;
    TCGv va = tcg_temp_new();
    TCGv t0 = tcg_temp_new();

    while (counter != count) {
        bool use_gp = gp && (counter == count - 1);
        int this_rt = use_gp ? 28 : (rt & 0x10) | ((rt + counter) & 0x1f);
        int this_offset = u - ((counter + 1) << 2);
        gen_base_offset_addr(ctx, va, 29, this_offset);
        tcg_gen_qemu_ld_tl(t0, va, ctx->mem_idx, MO_TESL |
                        ctx->default_tcg_memop_mask);
        tcg_gen_ext32s_tl(t0, t0);
        gen_store_gpr(t0, this_rt);
        counter++;
    }

    /* adjust stack pointer */
    gen_adjust_sp(ctx, u);

    tcg_temp_free(t0);
    tcg_temp_free(va);
}

static void gen_pool16c_nanomips_insn(DisasContext *ctx)
{
    int rt = decode_gpr_gpr3(NANOMIPS_EXTRACT_RT3(ctx->opcode));
    int rs = decode_gpr_gpr3(NANOMIPS_EXTRACT_RS3(ctx->opcode));

    switch (extract32(ctx->opcode, 2, 2)) {
    case NM_NOT16:
        gen_logic(ctx, OPC_NOR, rt, rs, 0);
        break;
    case NM_AND16:
        gen_logic(ctx, OPC_AND, rt, rt, rs);
        break;
    case NM_XOR16:
        gen_logic(ctx, OPC_XOR, rt, rt, rs);
        break;
    case NM_OR16:
        gen_logic(ctx, OPC_OR, rt, rt, rs);
        break;
    }
}

static void gen_pool32a0_nanomips_insn(CPUMIPSState *env, DisasContext *ctx)
{
    int rt = extract32(ctx->opcode, 21, 5);
    int rs = extract32(ctx->opcode, 16, 5);
    int rd = extract32(ctx->opcode, 11, 5);

    switch (extract32(ctx->opcode, 3, 7)) {
    case NM_P_TRAP:
        switch (extract32(ctx->opcode, 10, 1)) {
        case NM_TEQ:
            check_nms(ctx);
            gen_trap(ctx, OPC_TEQ, rs, rt, -1);
            break;
        case NM_TNE:
            check_nms(ctx);
            gen_trap(ctx, OPC_TNE, rs, rt, -1);
            break;
        }
        break;
    case NM_RDHWR:
        check_nms(ctx);
        gen_rdhwr(ctx, rt, rs, extract32(ctx->opcode, 11, 3));
        break;
    case NM_SEB:
        check_nms(ctx);
        gen_bshfl(ctx, OPC_SEB, rs, rt);
        break;
    case NM_SEH:
        gen_bshfl(ctx, OPC_SEH, rs, rt);
        break;
    case NM_SLLV:
        gen_shift(ctx, OPC_SLLV, rd, rt, rs);
        break;
    case NM_SRLV:
        gen_shift(ctx, OPC_SRLV, rd, rt, rs);
        break;
    case NM_SRAV:
        gen_shift(ctx, OPC_SRAV, rd, rt, rs);
        break;
    case NM_ROTRV:
        gen_shift(ctx, OPC_ROTRV, rd, rt, rs);
        break;
    case NM_ADD:
        gen_arith(ctx, OPC_ADD, rd, rs, rt);
        break;
    case NM_ADDU:
        gen_arith(ctx, OPC_ADDU, rd, rs, rt);
        break;
    case NM_SUB:
        check_nms(ctx);
        gen_arith(ctx, OPC_SUB, rd, rs, rt);
        break;
    case NM_SUBU:
        gen_arith(ctx, OPC_SUBU, rd, rs, rt);
        break;
    case NM_P_CMOVE:
        switch (extract32(ctx->opcode, 10, 1)) {
        case NM_MOVZ:
            gen_cond_move(ctx, OPC_MOVZ, rd, rs, rt);
            break;
        case NM_MOVN:
            gen_cond_move(ctx, OPC_MOVN, rd, rs, rt);
            break;
        }
        break;
    case NM_AND:
        gen_logic(ctx, OPC_AND, rd, rs, rt);
        break;
    case NM_OR:
        gen_logic(ctx, OPC_OR, rd, rs, rt);
        break;
    case NM_NOR:
        gen_logic(ctx, OPC_NOR, rd, rs, rt);
        break;
    case NM_XOR:
        gen_logic(ctx, OPC_XOR, rd, rs, rt);
        break;
    case NM_SLT:
        gen_slt(ctx, OPC_SLT, rd, rs, rt);
        break;
    case NM_P_SLTU:
        if (rd == 0) {
            /* P_DVP */
#ifndef CONFIG_USER_ONLY
            TCGv t0 = tcg_temp_new();
            switch (extract32(ctx->opcode, 10, 1)) {
            case NM_DVP:
                if (ctx->vp) {
                    check_cp0_enabled(ctx);
                    gen_helper_dvp(t0, cpu_env);
                    gen_store_gpr(t0, rt);
                }
                break;
            case NM_EVP:
                if (ctx->vp) {
                    check_cp0_enabled(ctx);
                    gen_helper_evp(t0, cpu_env);
                    gen_store_gpr(t0, rt);
                }
                break;
            }
            tcg_temp_free(t0);
#endif
        } else {
            gen_slt(ctx, OPC_SLTU, rd, rs, rt);
        }
        break;
    case NM_SOV:
        {
            TCGv t0 = tcg_temp_new();
            TCGv t1 = tcg_temp_new();
            TCGv t2 = tcg_temp_new();

            gen_load_gpr(t1, rs);
            gen_load_gpr(t2, rt);
            tcg_gen_add_tl(t0, t1, t2);
            tcg_gen_ext32s_tl(t0, t0);
            tcg_gen_xor_tl(t1, t1, t2);
            tcg_gen_xor_tl(t2, t0, t2);
            tcg_gen_andc_tl(t1, t2, t1);

            /* operands of same sign, result different sign */
            tcg_gen_setcondi_tl(TCG_COND_LT, t0, t1, 0);
            gen_store_gpr(t0, rd);

            tcg_temp_free(t0);
            tcg_temp_free(t1);
            tcg_temp_free(t2);
        }
        break;
    case NM_MUL:
        gen_r6_muldiv(ctx, R6_OPC_MUL, rd, rs, rt);
        break;
    case NM_MUH:
        gen_r6_muldiv(ctx, R6_OPC_MUH, rd, rs, rt);
        break;
    case NM_MULU:
        gen_r6_muldiv(ctx, R6_OPC_MULU, rd, rs, rt);
        break;
    case NM_MUHU:
        gen_r6_muldiv(ctx, R6_OPC_MUHU, rd, rs, rt);
        break;
    case NM_DIV:
        gen_r6_muldiv(ctx, R6_OPC_DIV, rd, rs, rt);
        break;
    case NM_MOD:
        gen_r6_muldiv(ctx, R6_OPC_MOD, rd, rs, rt);
        break;
    case NM_DIVU:
        gen_r6_muldiv(ctx, R6_OPC_DIVU, rd, rs, rt);
        break;
    case NM_MODU:
        gen_r6_muldiv(ctx, R6_OPC_MODU, rd, rs, rt);
        break;
#ifndef CONFIG_USER_ONLY
    case NM_MFC0:
        check_cp0_enabled(ctx);
        if (rt == 0) {
            /* Treat as NOP. */
            break;
        }
        gen_mfc0(ctx, cpu_gpr[rt], rs, extract32(ctx->opcode, 11, 3));
        gen_log_instr_gpr_update(ctx, rt);
        break;
    case NM_MTC0:
        check_cp0_enabled(ctx);
        {
            TCGv t0 = tcg_temp_new();

            gen_load_gpr(t0, rt);
            gen_mtc0(ctx, t0, rs, extract32(ctx->opcode, 11, 3));
            tcg_temp_free(t0);
        }
        break;
    case NM_D_E_MT_VPE:
        {
            uint8_t sc = extract32(ctx->opcode, 10, 1);
            TCGv t0 = tcg_temp_new();

            switch (sc) {
            case 0:
                if (rs == 1) {
                    /* DMT */
                    check_cp0_mt(ctx);
                    gen_helper_dmt(t0);
                    gen_store_gpr(t0, rt);
                } else if (rs == 0) {
                    /* DVPE */
                    check_cp0_mt(ctx);
                    gen_helper_dvpe(t0, cpu_env);
                    gen_store_gpr(t0, rt);
                } else {
                    gen_reserved_instruction(ctx);
                }
                break;
            case 1:
                if (rs == 1) {
                    /* EMT */
                    check_cp0_mt(ctx);
                    gen_helper_emt(t0);
                    gen_store_gpr(t0, rt);
                } else if (rs == 0) {
                    /* EVPE */
                    check_cp0_mt(ctx);
                    gen_helper_evpe(t0, cpu_env);
                    gen_store_gpr(t0, rt);
                } else {
                    gen_reserved_instruction(ctx);
                }
                break;
            }

            tcg_temp_free(t0);
        }
        break;
    case NM_FORK:
        check_mt(ctx);
        {
            TCGv t0 = tcg_temp_new();
            TCGv t1 = tcg_temp_new();

            gen_load_gpr(t0, rt);
            gen_load_gpr(t1, rs);
            gen_helper_fork(t0, t1);
            tcg_temp_free(t0);
            tcg_temp_free(t1);
        }
        break;
    case NM_MFTR:
    case NM_MFHTR:
        check_cp0_enabled(ctx);
        if (rd == 0) {
            /* Treat as NOP. */
            return;
        }
        gen_mftr(env, ctx, rs, rt, extract32(ctx->opcode, 10, 1),
                 extract32(ctx->opcode, 11, 5), extract32(ctx->opcode, 3, 1));
        break;
    case NM_MTTR:
    case NM_MTHTR:
        check_cp0_enabled(ctx);
        gen_mttr(env, ctx, rs, rt, extract32(ctx->opcode, 10, 1),
                 extract32(ctx->opcode, 11, 5), extract32(ctx->opcode, 3, 1));
        break;
    case NM_YIELD:
        check_mt(ctx);
        {
            TCGv t0 = tcg_temp_new();

            gen_load_gpr(t0, rs);
            gen_helper_yield(t0, cpu_env, t0);
            gen_store_gpr(t0, rt);
            tcg_temp_free(t0);
        }
        break;
#endif
    default:
        gen_reserved_instruction(ctx);
        break;
    }
}

/* dsp */
static void gen_pool32axf_1_5_nanomips_insn(DisasContext *ctx, uint32_t opc,
                                            int ret, int v1, int v2)
{
    TCGv_i32 t0;
    TCGv v0_t;
    TCGv v1_t;

    t0 = tcg_temp_new_i32();

    v0_t = tcg_temp_new();
    v1_t = tcg_temp_new();

    tcg_gen_movi_i32(t0, v2 >> 3);

    gen_load_gpr(v0_t, ret);
    gen_load_gpr(v1_t, v1);

    switch (opc) {
    case NM_MAQ_S_W_PHR:
        check_dsp(ctx);
        gen_helper_maq_s_w_phr(t0, v1_t, v0_t, cpu_env);
        break;
    case NM_MAQ_S_W_PHL:
        check_dsp(ctx);
        gen_helper_maq_s_w_phl(t0, v1_t, v0_t, cpu_env);
        break;
    case NM_MAQ_SA_W_PHR:
        check_dsp(ctx);
        gen_helper_maq_sa_w_phr(t0, v1_t, v0_t, cpu_env);
        break;
    case NM_MAQ_SA_W_PHL:
        check_dsp(ctx);
        gen_helper_maq_sa_w_phl(t0, v1_t, v0_t, cpu_env);
        break;
    default:
        gen_reserved_instruction(ctx);
        break;
    }

    tcg_temp_free_i32(t0);

    tcg_temp_free(v0_t);
    tcg_temp_free(v1_t);
}


static void gen_pool32axf_1_nanomips_insn(DisasContext *ctx, uint32_t opc,
                                    int ret, int v1, int v2)
{
    int16_t imm;
    TCGv t0 = tcg_temp_new();
    TCGv t1 = tcg_temp_new();
    TCGv v0_t = tcg_temp_new();

    gen_load_gpr(v0_t, v1);

    switch (opc) {
    case NM_POOL32AXF_1_0:
        check_dsp(ctx);
        switch (extract32(ctx->opcode, 12, 2)) {
        case NM_MFHI:
            gen_HILO(ctx, OPC_MFHI, v2 >> 3, ret);
            break;
        case NM_MFLO:
            gen_HILO(ctx, OPC_MFLO, v2 >> 3, ret);
            break;
        case NM_MTHI:
            gen_HILO(ctx, OPC_MTHI, v2 >> 3, v1);
            break;
        case NM_MTLO:
            gen_HILO(ctx, OPC_MTLO, v2 >> 3, v1);
            break;
        }
        break;
    case NM_POOL32AXF_1_1:
        check_dsp(ctx);
        switch (extract32(ctx->opcode, 12, 2)) {
        case NM_MTHLIP:
            tcg_gen_movi_tl(t0, v2);
            gen_helper_mthlip(t0, v0_t, cpu_env);
            break;
        case NM_SHILOV:
            tcg_gen_movi_tl(t0, v2 >> 3);
            gen_helper_shilo(t0, v0_t, cpu_env);
            break;
        default:
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case NM_POOL32AXF_1_3:
        check_dsp(ctx);
        imm = extract32(ctx->opcode, 14, 7);
        switch (extract32(ctx->opcode, 12, 2)) {
        case NM_RDDSP:
            tcg_gen_movi_tl(t0, imm);
            gen_helper_rddsp(t0, t0, cpu_env);
            gen_store_gpr(t0, ret);
            break;
        case NM_WRDSP:
            gen_load_gpr(t0, ret);
            tcg_gen_movi_tl(t1, imm);
            gen_helper_wrdsp(t0, t1, cpu_env);
            break;
        case NM_EXTP:
            tcg_gen_movi_tl(t0, v2 >> 3);
            tcg_gen_movi_tl(t1, v1);
            gen_helper_extp(t0, t0, t1, cpu_env);
            gen_store_gpr(t0, ret);
            break;
        case NM_EXTPDP:
            tcg_gen_movi_tl(t0, v2 >> 3);
            tcg_gen_movi_tl(t1, v1);
            gen_helper_extpdp(t0, t0, t1, cpu_env);
            gen_store_gpr(t0, ret);
            break;
        }
        break;
    case NM_POOL32AXF_1_4:
        check_dsp(ctx);
        tcg_gen_movi_tl(t0, v2 >> 2);
        switch (extract32(ctx->opcode, 12, 1)) {
        case NM_SHLL_QB:
            gen_helper_shll_qb(t0, t0, v0_t, cpu_env);
            gen_store_gpr(t0, ret);
            break;
        case NM_SHRL_QB:
            gen_helper_shrl_qb(t0, t0, v0_t);
            gen_store_gpr(t0, ret);
            break;
        }
        break;
    case NM_POOL32AXF_1_5:
        opc = extract32(ctx->opcode, 12, 2);
        gen_pool32axf_1_5_nanomips_insn(ctx, opc, ret, v1, v2);
        break;
    case NM_POOL32AXF_1_7:
        check_dsp(ctx);
        tcg_gen_movi_tl(t0, v2 >> 3);
        tcg_gen_movi_tl(t1, v1);
        switch (extract32(ctx->opcode, 12, 2)) {
        case NM_EXTR_W:
            gen_helper_extr_w(t0, t0, t1, cpu_env);
            gen_store_gpr(t0, ret);
            break;
        case NM_EXTR_R_W:
            gen_helper_extr_r_w(t0, t0, t1, cpu_env);
            gen_store_gpr(t0, ret);
            break;
        case NM_EXTR_RS_W:
            gen_helper_extr_rs_w(t0, t0, t1, cpu_env);
            gen_store_gpr(t0, ret);
            break;
        case NM_EXTR_S_H:
            gen_helper_extr_s_h(t0, t0, t1, cpu_env);
            gen_store_gpr(t0, ret);
            break;
        }
        break;
    default:
        gen_reserved_instruction(ctx);
        break;
    }

    tcg_temp_free(t0);
    tcg_temp_free(t1);
    tcg_temp_free(v0_t);
}

static void gen_pool32axf_2_multiply(DisasContext *ctx, uint32_t opc,
                                    TCGv v0, TCGv v1, int rd)
{
    TCGv_i32 t0;

    t0 = tcg_temp_new_i32();

    tcg_gen_movi_i32(t0, rd >> 3);

    switch (opc) {
    case NM_POOL32AXF_2_0_7:
        switch (extract32(ctx->opcode, 9, 3)) {
        case NM_DPA_W_PH:
            check_dsp_r2(ctx);
            gen_helper_dpa_w_ph(t0, v1, v0, cpu_env);
            break;
        case NM_DPAQ_S_W_PH:
            check_dsp(ctx);
            gen_helper_dpaq_s_w_ph(t0, v1, v0, cpu_env);
            break;
        case NM_DPS_W_PH:
            check_dsp_r2(ctx);
            gen_helper_dps_w_ph(t0, v1, v0, cpu_env);
            break;
        case NM_DPSQ_S_W_PH:
            check_dsp(ctx);
            gen_helper_dpsq_s_w_ph(t0, v1, v0, cpu_env);
            break;
        default:
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case NM_POOL32AXF_2_8_15:
        switch (extract32(ctx->opcode, 9, 3)) {
        case NM_DPAX_W_PH:
            check_dsp_r2(ctx);
            gen_helper_dpax_w_ph(t0, v0, v1, cpu_env);
            break;
        case NM_DPAQ_SA_L_W:
            check_dsp(ctx);
            gen_helper_dpaq_sa_l_w(t0, v0, v1, cpu_env);
            break;
        case NM_DPSX_W_PH:
            check_dsp_r2(ctx);
            gen_helper_dpsx_w_ph(t0, v0, v1, cpu_env);
            break;
        case NM_DPSQ_SA_L_W:
            check_dsp(ctx);
            gen_helper_dpsq_sa_l_w(t0, v0, v1, cpu_env);
            break;
        default:
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case NM_POOL32AXF_2_16_23:
        switch (extract32(ctx->opcode, 9, 3)) {
        case NM_DPAU_H_QBL:
            check_dsp(ctx);
            gen_helper_dpau_h_qbl(t0, v0, v1, cpu_env);
            break;
        case NM_DPAQX_S_W_PH:
            check_dsp_r2(ctx);
            gen_helper_dpaqx_s_w_ph(t0, v0, v1, cpu_env);
            break;
        case NM_DPSU_H_QBL:
            check_dsp(ctx);
            gen_helper_dpsu_h_qbl(t0, v0, v1, cpu_env);
            break;
        case NM_DPSQX_S_W_PH:
            check_dsp_r2(ctx);
            gen_helper_dpsqx_s_w_ph(t0, v0, v1, cpu_env);
            break;
        case NM_MULSA_W_PH:
            check_dsp_r2(ctx);
            gen_helper_mulsa_w_ph(t0, v0, v1, cpu_env);
            break;
        default:
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case NM_POOL32AXF_2_24_31:
        switch (extract32(ctx->opcode, 9, 3)) {
        case NM_DPAU_H_QBR:
            check_dsp(ctx);
            gen_helper_dpau_h_qbr(t0, v1, v0, cpu_env);
            break;
        case NM_DPAQX_SA_W_PH:
            check_dsp_r2(ctx);
            gen_helper_dpaqx_sa_w_ph(t0, v1, v0, cpu_env);
            break;
        case NM_DPSU_H_QBR:
            check_dsp(ctx);
            gen_helper_dpsu_h_qbr(t0, v1, v0, cpu_env);
            break;
        case NM_DPSQX_SA_W_PH:
            check_dsp_r2(ctx);
            gen_helper_dpsqx_sa_w_ph(t0, v1, v0, cpu_env);
            break;
        case NM_MULSAQ_S_W_PH:
            check_dsp(ctx);
            gen_helper_mulsaq_s_w_ph(t0, v1, v0, cpu_env);
            break;
        default:
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    default:
        gen_reserved_instruction(ctx);
        break;
    }

    tcg_temp_free_i32(t0);
}

static void gen_pool32axf_2_nanomips_insn(DisasContext *ctx, uint32_t opc,
                                          int rt, int rs, int rd)
{
    int ret = rt;
    TCGv t0 = tcg_temp_new();
    TCGv t1 = tcg_temp_new();
    TCGv v0_t = tcg_temp_new();
    TCGv v1_t = tcg_temp_new();

    gen_load_gpr(v0_t, rt);
    gen_load_gpr(v1_t, rs);

    switch (opc) {
    case NM_POOL32AXF_2_0_7:
        switch (extract32(ctx->opcode, 9, 3)) {
        case NM_DPA_W_PH:
        case NM_DPAQ_S_W_PH:
        case NM_DPS_W_PH:
        case NM_DPSQ_S_W_PH:
            gen_pool32axf_2_multiply(ctx, opc, v0_t, v1_t, rd);
            break;
        case NM_BALIGN:
            check_dsp_r2(ctx);
            if (rt != 0) {
                gen_load_gpr(t0, rs);
                rd &= 3;
                if (rd != 0 && rd != 2) {
                    tcg_gen_shli_tl(cpu_gpr[ret], cpu_gpr[ret], 8 * rd);
                    tcg_gen_ext32u_tl(t0, t0);
                    tcg_gen_shri_tl(t0, t0, 8 * (4 - rd));
                    tcg_gen_or_tl(cpu_gpr[ret], cpu_gpr[ret], t0);
                }
                tcg_gen_ext32s_tl(cpu_gpr[ret], cpu_gpr[ret]);
                gen_log_instr_gpr_update(ctx, ret);
            }
            break;
        case NM_MADD:
            check_dsp(ctx);
            {
                int acc = extract32(ctx->opcode, 14, 2);
                TCGv_i64 t2 = tcg_temp_new_i64();
                TCGv_i64 t3 = tcg_temp_new_i64();

                gen_load_gpr(t0, rt);
                gen_load_gpr(t1, rs);
                tcg_gen_ext_tl_i64(t2, t0);
                tcg_gen_ext_tl_i64(t3, t1);
                tcg_gen_mul_i64(t2, t2, t3);
                tcg_gen_concat_tl_i64(t3, cpu_LO[acc], cpu_HI[acc]);
                tcg_gen_add_i64(t2, t2, t3);
                tcg_temp_free_i64(t3);
                gen_move_low32(cpu_LO[acc], t2);
                gen_move_high32(cpu_HI[acc], t2);
                tcg_temp_free_i64(t2);
            }
            break;
        case NM_MULT:
            check_dsp(ctx);
            {
                int acc = extract32(ctx->opcode, 14, 2);
                TCGv_i32 t2 = tcg_temp_new_i32();
                TCGv_i32 t3 = tcg_temp_new_i32();

                gen_load_gpr(t0, rs);
                gen_load_gpr(t1, rt);
                tcg_gen_trunc_tl_i32(t2, t0);
                tcg_gen_trunc_tl_i32(t3, t1);
                tcg_gen_muls2_i32(t2, t3, t2, t3);
                tcg_gen_ext_i32_tl(cpu_LO[acc], t2);
                tcg_gen_ext_i32_tl(cpu_HI[acc], t3);
                tcg_temp_free_i32(t2);
                tcg_temp_free_i32(t3);
            }
            break;
        case NM_EXTRV_W:
            check_dsp(ctx);
            gen_load_gpr(v1_t, rs);
            tcg_gen_movi_tl(t0, rd >> 3);
            gen_helper_extr_w(t0, t0, v1_t, cpu_env);
            gen_store_gpr(t0, ret);
            break;
        }
        break;
    case NM_POOL32AXF_2_8_15:
        switch (extract32(ctx->opcode, 9, 3)) {
        case NM_DPAX_W_PH:
        case NM_DPAQ_SA_L_W:
        case NM_DPSX_W_PH:
        case NM_DPSQ_SA_L_W:
            gen_pool32axf_2_multiply(ctx, opc, v0_t, v1_t, rd);
            break;
        case NM_MADDU:
            check_dsp(ctx);
            {
                int acc = extract32(ctx->opcode, 14, 2);
                TCGv_i64 t2 = tcg_temp_new_i64();
                TCGv_i64 t3 = tcg_temp_new_i64();

                gen_load_gpr(t0, rs);
                gen_load_gpr(t1, rt);
                tcg_gen_ext32u_tl(t0, t0);
                tcg_gen_ext32u_tl(t1, t1);
                tcg_gen_extu_tl_i64(t2, t0);
                tcg_gen_extu_tl_i64(t3, t1);
                tcg_gen_mul_i64(t2, t2, t3);
                tcg_gen_concat_tl_i64(t3, cpu_LO[acc], cpu_HI[acc]);
                tcg_gen_add_i64(t2, t2, t3);
                tcg_temp_free_i64(t3);
                gen_move_low32(cpu_LO[acc], t2);
                gen_move_high32(cpu_HI[acc], t2);
                tcg_temp_free_i64(t2);
            }
            break;
        case NM_MULTU:
            check_dsp(ctx);
            {
                int acc = extract32(ctx->opcode, 14, 2);
                TCGv_i32 t2 = tcg_temp_new_i32();
                TCGv_i32 t3 = tcg_temp_new_i32();

                gen_load_gpr(t0, rs);
                gen_load_gpr(t1, rt);
                tcg_gen_trunc_tl_i32(t2, t0);
                tcg_gen_trunc_tl_i32(t3, t1);
                tcg_gen_mulu2_i32(t2, t3, t2, t3);
                tcg_gen_ext_i32_tl(cpu_LO[acc], t2);
                tcg_gen_ext_i32_tl(cpu_HI[acc], t3);
                tcg_temp_free_i32(t2);
                tcg_temp_free_i32(t3);
            }
            break;
        case NM_EXTRV_R_W:
            check_dsp(ctx);
            tcg_gen_movi_tl(t0, rd >> 3);
            gen_helper_extr_r_w(t0, t0, v1_t, cpu_env);
            gen_store_gpr(t0, ret);
            break;
        default:
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case NM_POOL32AXF_2_16_23:
        switch (extract32(ctx->opcode, 9, 3)) {
        case NM_DPAU_H_QBL:
        case NM_DPAQX_S_W_PH:
        case NM_DPSU_H_QBL:
        case NM_DPSQX_S_W_PH:
        case NM_MULSA_W_PH:
            gen_pool32axf_2_multiply(ctx, opc, v0_t, v1_t, rd);
            break;
        case NM_EXTPV:
            check_dsp(ctx);
            tcg_gen_movi_tl(t0, rd >> 3);
            gen_helper_extp(t0, t0, v1_t, cpu_env);
            gen_store_gpr(t0, ret);
            break;
        case NM_MSUB:
            check_dsp(ctx);
            {
                int acc = extract32(ctx->opcode, 14, 2);
                TCGv_i64 t2 = tcg_temp_new_i64();
                TCGv_i64 t3 = tcg_temp_new_i64();

                gen_load_gpr(t0, rs);
                gen_load_gpr(t1, rt);
                tcg_gen_ext_tl_i64(t2, t0);
                tcg_gen_ext_tl_i64(t3, t1);
                tcg_gen_mul_i64(t2, t2, t3);
                tcg_gen_concat_tl_i64(t3, cpu_LO[acc], cpu_HI[acc]);
                tcg_gen_sub_i64(t2, t3, t2);
                tcg_temp_free_i64(t3);
                gen_move_low32(cpu_LO[acc], t2);
                gen_move_high32(cpu_HI[acc], t2);
                tcg_temp_free_i64(t2);
            }
            break;
        case NM_EXTRV_RS_W:
            check_dsp(ctx);
            tcg_gen_movi_tl(t0, rd >> 3);
            gen_helper_extr_rs_w(t0, t0, v1_t, cpu_env);
            gen_store_gpr(t0, ret);
            break;
        }
        break;
    case NM_POOL32AXF_2_24_31:
        switch (extract32(ctx->opcode, 9, 3)) {
        case NM_DPAU_H_QBR:
        case NM_DPAQX_SA_W_PH:
        case NM_DPSU_H_QBR:
        case NM_DPSQX_SA_W_PH:
        case NM_MULSAQ_S_W_PH:
            gen_pool32axf_2_multiply(ctx, opc, v0_t, v1_t, rd);
            break;
        case NM_EXTPDPV:
            check_dsp(ctx);
            tcg_gen_movi_tl(t0, rd >> 3);
            gen_helper_extpdp(t0, t0, v1_t, cpu_env);
            gen_store_gpr(t0, ret);
            break;
        case NM_MSUBU:
            check_dsp(ctx);
            {
                int acc = extract32(ctx->opcode, 14, 2);
                TCGv_i64 t2 = tcg_temp_new_i64();
                TCGv_i64 t3 = tcg_temp_new_i64();

                gen_load_gpr(t0, rs);
                gen_load_gpr(t1, rt);
                tcg_gen_ext32u_tl(t0, t0);
                tcg_gen_ext32u_tl(t1, t1);
                tcg_gen_extu_tl_i64(t2, t0);
                tcg_gen_extu_tl_i64(t3, t1);
                tcg_gen_mul_i64(t2, t2, t3);
                tcg_gen_concat_tl_i64(t3, cpu_LO[acc], cpu_HI[acc]);
                tcg_gen_sub_i64(t2, t3, t2);
                tcg_temp_free_i64(t3);
                gen_move_low32(cpu_LO[acc], t2);
                gen_move_high32(cpu_HI[acc], t2);
                tcg_temp_free_i64(t2);
            }
            break;
        case NM_EXTRV_S_H:
            check_dsp(ctx);
            tcg_gen_movi_tl(t0, rd >> 3);
            gen_helper_extr_s_h(t0, t0, v0_t, cpu_env);
            gen_store_gpr(t0, ret);
            break;
        }
        break;
    default:
        gen_reserved_instruction(ctx);
        break;
    }

    tcg_temp_free(t0);
    tcg_temp_free(t1);

    tcg_temp_free(v0_t);
    tcg_temp_free(v1_t);
}

static void gen_pool32axf_4_nanomips_insn(DisasContext *ctx, uint32_t opc,
                                          int rt, int rs)
{
    int ret = rt;
    TCGv t0 = tcg_temp_new();
    TCGv v0_t = tcg_temp_new();

    gen_load_gpr(v0_t, rs);

    switch (opc) {
    case NM_ABSQ_S_QB:
        check_dsp_r2(ctx);
        gen_helper_absq_s_qb(v0_t, v0_t, cpu_env);
        gen_store_gpr(v0_t, ret);
        break;
    case NM_ABSQ_S_PH:
        check_dsp(ctx);
        gen_helper_absq_s_ph(v0_t, v0_t, cpu_env);
        gen_store_gpr(v0_t, ret);
        break;
    case NM_ABSQ_S_W:
        check_dsp(ctx);
        gen_helper_absq_s_w(v0_t, v0_t, cpu_env);
        gen_store_gpr(v0_t, ret);
        break;
    case NM_PRECEQ_W_PHL:
        check_dsp(ctx);
        tcg_gen_andi_tl(v0_t, v0_t, 0xFFFF0000);
        tcg_gen_ext32s_tl(v0_t, v0_t);
        gen_store_gpr(v0_t, ret);
        break;
    case NM_PRECEQ_W_PHR:
        check_dsp(ctx);
        tcg_gen_andi_tl(v0_t, v0_t, 0x0000FFFF);
        tcg_gen_shli_tl(v0_t, v0_t, 16);
        tcg_gen_ext32s_tl(v0_t, v0_t);
        gen_store_gpr(v0_t, ret);
        break;
    case NM_PRECEQU_PH_QBL:
        check_dsp(ctx);
        gen_helper_precequ_ph_qbl(v0_t, v0_t);
        gen_store_gpr(v0_t, ret);
        break;
    case NM_PRECEQU_PH_QBR:
        check_dsp(ctx);
        gen_helper_precequ_ph_qbr(v0_t, v0_t);
        gen_store_gpr(v0_t, ret);
        break;
    case NM_PRECEQU_PH_QBLA:
        check_dsp(ctx);
        gen_helper_precequ_ph_qbla(v0_t, v0_t);
        gen_store_gpr(v0_t, ret);
        break;
    case NM_PRECEQU_PH_QBRA:
        check_dsp(ctx);
        gen_helper_precequ_ph_qbra(v0_t, v0_t);
        gen_store_gpr(v0_t, ret);
        break;
    case NM_PRECEU_PH_QBL:
        check_dsp(ctx);
        gen_helper_preceu_ph_qbl(v0_t, v0_t);
        gen_store_gpr(v0_t, ret);
        break;
    case NM_PRECEU_PH_QBR:
        check_dsp(ctx);
        gen_helper_preceu_ph_qbr(v0_t, v0_t);
        gen_store_gpr(v0_t, ret);
        break;
    case NM_PRECEU_PH_QBLA:
        check_dsp(ctx);
        gen_helper_preceu_ph_qbla(v0_t, v0_t);
        gen_store_gpr(v0_t, ret);
        break;
    case NM_PRECEU_PH_QBRA:
        check_dsp(ctx);
        gen_helper_preceu_ph_qbra(v0_t, v0_t);
        gen_store_gpr(v0_t, ret);
        break;
    case NM_REPLV_PH:
        check_dsp(ctx);
        tcg_gen_ext16u_tl(v0_t, v0_t);
        tcg_gen_shli_tl(t0, v0_t, 16);
        tcg_gen_or_tl(v0_t, v0_t, t0);
        tcg_gen_ext32s_tl(v0_t, v0_t);
        gen_store_gpr(v0_t, ret);
        break;
    case NM_REPLV_QB:
        check_dsp(ctx);
        tcg_gen_ext8u_tl(v0_t, v0_t);
        tcg_gen_shli_tl(t0, v0_t, 8);
        tcg_gen_or_tl(v0_t, v0_t, t0);
        tcg_gen_shli_tl(t0, v0_t, 16);
        tcg_gen_or_tl(v0_t, v0_t, t0);
        tcg_gen_ext32s_tl(v0_t, v0_t);
        gen_store_gpr(v0_t, ret);
        break;
    case NM_BITREV:
        check_dsp(ctx);
        gen_helper_bitrev(v0_t, v0_t);
        gen_store_gpr(v0_t, ret);
        break;
    case NM_INSV:
        check_dsp(ctx);
        {
            TCGv tv0 = tcg_temp_new();

            gen_load_gpr(tv0, rt);
            gen_helper_insv(v0_t, cpu_env, v0_t, tv0);
            gen_store_gpr(v0_t, ret);
            tcg_temp_free(tv0);
        }
        break;
    case NM_RADDU_W_QB:
        check_dsp(ctx);
        gen_helper_raddu_w_qb(v0_t, v0_t);
        gen_store_gpr(v0_t, ret);
        break;
    case NM_BITSWAP:
        gen_bitswap(ctx, OPC_BITSWAP, ret, rs);
        break;
    case NM_CLO:
        check_nms(ctx);
        gen_cl(ctx, OPC_CLO, ret, rs);
        break;
    case NM_CLZ:
        check_nms(ctx);
        gen_cl(ctx, OPC_CLZ, ret, rs);
        break;
    case NM_WSBH:
        gen_bshfl(ctx, OPC_WSBH, ret, rs);
        break;
    default:
        gen_reserved_instruction(ctx);
        break;
    }

    tcg_temp_free(v0_t);
    tcg_temp_free(t0);
}

static void gen_pool32axf_7_nanomips_insn(DisasContext *ctx, uint32_t opc,
                                          int rt, int rs, int rd)
{
    TCGv t0 = tcg_temp_new();
    TCGv rs_t = tcg_temp_new();

    gen_load_gpr(rs_t, rs);

    switch (opc) {
    case NM_SHRA_R_QB:
        check_dsp_r2(ctx);
        tcg_gen_movi_tl(t0, rd >> 2);
        switch (extract32(ctx->opcode, 12, 1)) {
        case 0:
            /* NM_SHRA_QB */
            gen_helper_shra_qb(t0, t0, rs_t);
            gen_store_gpr(t0, rt);
            break;
        case 1:
            /* NM_SHRA_R_QB */
            gen_helper_shra_r_qb(t0, t0, rs_t);
            gen_store_gpr(t0, rt);
            break;
        }
        break;
    case NM_SHRL_PH:
        check_dsp_r2(ctx);
        tcg_gen_movi_tl(t0, rd >> 1);
        gen_helper_shrl_ph(t0, t0, rs_t);
        gen_store_gpr(t0, rt);
        break;
    case NM_REPL_QB:
        check_dsp(ctx);
        {
            int16_t imm;
            target_long result;
            imm = extract32(ctx->opcode, 13, 8);
            result = (uint32_t)imm << 24 |
                     (uint32_t)imm << 16 |
                     (uint32_t)imm << 8  |
                     (uint32_t)imm;
            result = (int32_t)result;
            tcg_gen_movi_tl(t0, result);
            gen_store_gpr(t0, rt);
        }
        break;
    default:
        gen_reserved_instruction(ctx);
        break;
    }
    tcg_temp_free(t0);
    tcg_temp_free(rs_t);
}


static void gen_pool32axf_nanomips_insn(CPUMIPSState *env, DisasContext *ctx)
{
    int rt = extract32(ctx->opcode, 21, 5);
    int rs = extract32(ctx->opcode, 16, 5);
    int rd = extract32(ctx->opcode, 11, 5);

    switch (extract32(ctx->opcode, 6, 3)) {
    case NM_POOL32AXF_1:
        {
            int32_t op1 = extract32(ctx->opcode, 9, 3);
            gen_pool32axf_1_nanomips_insn(ctx, op1, rt, rs, rd);
        }
        break;
    case NM_POOL32AXF_2:
        {
            int32_t op1 = extract32(ctx->opcode, 12, 2);
            gen_pool32axf_2_nanomips_insn(ctx, op1, rt, rs, rd);
        }
        break;
    case NM_POOL32AXF_4:
        {
            int32_t op1 = extract32(ctx->opcode, 9, 7);
            gen_pool32axf_4_nanomips_insn(ctx, op1, rt, rs);
        }
        break;
    case NM_POOL32AXF_5:
        switch (extract32(ctx->opcode, 9, 7)) {
#ifndef CONFIG_USER_ONLY
        case NM_TLBP:
            gen_cp0(env, ctx, OPC_TLBP, 0, 0);
            break;
        case NM_TLBR:
            gen_cp0(env, ctx, OPC_TLBR, 0, 0);
            break;
        case NM_TLBWI:
            gen_cp0(env, ctx, OPC_TLBWI, 0, 0);
            break;
        case NM_TLBWR:
            gen_cp0(env, ctx, OPC_TLBWR, 0, 0);
            break;
        case NM_TLBINV:
            gen_cp0(env, ctx, OPC_TLBINV, 0, 0);
            break;
        case NM_TLBINVF:
            gen_cp0(env, ctx, OPC_TLBINVF, 0, 0);
            break;
        case NM_DI:
            check_cp0_enabled(ctx);
            {
                TCGv t0 = tcg_temp_new();

                save_cpu_state(ctx, 1);
                gen_helper_di(t0, cpu_env);
                gen_store_gpr(t0, rt);
            /* Stop translation as we may have switched the execution mode */
                ctx->base.is_jmp = DISAS_STOP;
                tcg_temp_free(t0);
            }
            break;
        case NM_EI:
            check_cp0_enabled(ctx);
            {
                TCGv t0 = tcg_temp_new();

                save_cpu_state(ctx, 1);
                gen_helper_ei(t0, cpu_env);
                gen_store_gpr(t0, rt);
            /* Stop translation as we may have switched the execution mode */
                ctx->base.is_jmp = DISAS_STOP;
                tcg_temp_free(t0);
            }
            break;
        case NM_RDPGPR:
            gen_load_srsgpr(ctx, rs, rt);
            break;
        case NM_WRPGPR:
            gen_store_srsgpr(rs, rt);
            break;
        case NM_WAIT:
            gen_cp0(env, ctx, OPC_WAIT, 0, 0);
            break;
        case NM_DERET:
            gen_cp0(env, ctx, OPC_DERET, 0, 0);
            break;
        case NM_ERETX:
            gen_cp0(env, ctx, OPC_ERET, 0, 0);
            break;
#endif
        default:
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case NM_POOL32AXF_7:
        {
            int32_t op1 = extract32(ctx->opcode, 9, 3);
            gen_pool32axf_7_nanomips_insn(ctx, op1, rt, rs, rd);
        }
        break;
    default:
        gen_reserved_instruction(ctx);
        break;
    }
}

/* Immediate Value Compact Branches */
static void gen_compute_imm_branch(DisasContext *ctx, uint32_t opc,
                                   int rt, int32_t imm, int32_t offset)
{
    TCGCond cond = TCG_COND_ALWAYS;
    TCGv t0 = tcg_temp_new();
    TCGv t1 = tcg_temp_new();

    gen_load_gpr(t0, rt);
    tcg_gen_movi_tl(t1, imm);
    ctx->btarget = addr_add(ctx, ctx->base.pc_next + 4, offset);

    /* Load needed operands and calculate btarget */
    switch (opc) {
    case NM_BEQIC:
        if (rt == 0 && imm == 0) {
            /* Unconditional branch */
        } else if (rt == 0 && imm != 0) {
            /* Treat as NOP */
            goto out;
        } else {
            cond = TCG_COND_EQ;
        }
        break;
    case NM_BBEQZC:
    case NM_BBNEZC:
        check_nms(ctx);
        if (imm >= 32 && !(ctx->hflags & MIPS_HFLAG_64)) {
            gen_reserved_instruction(ctx);
            goto out;
        } else if (rt == 0 && opc == NM_BBEQZC) {
            /* Unconditional branch */
        } else if (rt == 0 && opc == NM_BBNEZC) {
            /* Treat as NOP */
            goto out;
        } else {
            tcg_gen_shri_tl(t0, t0, imm);
            tcg_gen_andi_tl(t0, t0, 1);
            tcg_gen_movi_tl(t1, 0);
            if (opc == NM_BBEQZC) {
                cond = TCG_COND_EQ;
            } else {
                cond = TCG_COND_NE;
            }
        }
        break;
    case NM_BNEIC:
        if (rt == 0 && imm == 0) {
            /* Treat as NOP */
            goto out;
        } else if (rt == 0 && imm != 0) {
            /* Unconditional branch */
        } else {
            cond = TCG_COND_NE;
        }
        break;
    case NM_BGEIC:
        if (rt == 0 && imm == 0) {
            /* Unconditional branch */
        } else  {
            cond = TCG_COND_GE;
        }
        break;
    case NM_BLTIC:
        cond = TCG_COND_LT;
        break;
    case NM_BGEIUC:
        if (rt == 0 && imm == 0) {
            /* Unconditional branch */
        } else  {
            cond = TCG_COND_GEU;
        }
        break;
    case NM_BLTIUC:
        cond = TCG_COND_LTU;
        break;
    default:
        MIPS_INVAL("Immediate Value Compact branch");
        gen_reserved_instruction(ctx);
        goto out;
    }

    /* branch completion */
    clear_branch_hflags(ctx);
    ctx->base.is_jmp = DISAS_NORETURN;

    if (cond == TCG_COND_ALWAYS) {
        /* Uncoditional compact branch */
        gen_goto_tb(ctx, 0, ctx->btarget);
    } else {
        /* Conditional compact branch */
        TCGLabel *fs = gen_new_label();

        tcg_gen_brcond_tl(tcg_invert_cond(cond), t0, t1, fs);

        gen_goto_tb(ctx, 1, ctx->btarget);
        gen_set_label(fs);

        gen_goto_tb(ctx, 0, ctx->base.pc_next + 4);
    }

out:
    tcg_temp_free(t0);
    tcg_temp_free(t1);
}

/* P.BALRSC type nanoMIPS R6 branches: BALRSC and BRSC */
static void gen_compute_nanomips_pbalrsc_branch(DisasContext *ctx, int rs,
                                                int rt)
{
    TCGv t0 = tcg_temp_new();
    TCGv t1 = tcg_temp_new();

    /* load rs */
    gen_load_gpr(t0, rs);

    /* link */
    if (rt != 0) {
        tcg_gen_movi_tl(cpu_gpr[rt], ctx->base.pc_next + 4);
        gen_log_instr_gpr_update(ctx, rt);
    }

    /* calculate btarget */
    tcg_gen_shli_tl(t0, t0, 1);
    tcg_gen_movi_tl(t1, ctx->base.pc_next + 4);
    gen_op_addr_add(ctx, btarget, t1, t0);

    /* branch completion */
    clear_branch_hflags(ctx);
    ctx->base.is_jmp = DISAS_NORETURN;

    /* unconditional branch to register */
    tcg_gen_mov_tl(cpu_PC, btarget);
#ifdef CONFIG_DEBUG_TCG
    tcg_gen_movi_tl(_pc_is_current, 1); // PC has been updated.
#endif
    tcg_gen_lookup_and_goto_ptr();

    tcg_temp_free(t0);
    tcg_temp_free(t1);
}

/* nanoMIPS Branches */
static void gen_compute_compact_branch_nm(DisasContext *ctx, uint32_t opc,
                                       int rs, int rt, int32_t offset)
{
    int bcond_compute = 0;
    TCGv t0 = tcg_temp_new();
    TCGv t1 = tcg_temp_new();

    /* Load needed operands and calculate btarget */
    switch (opc) {
    /* compact branch */
    case OPC_BGEC:
    case OPC_BLTC:
        gen_load_gpr(t0, rs);
        gen_load_gpr(t1, rt);
        bcond_compute = 1;
        ctx->btarget = addr_add(ctx, ctx->base.pc_next + 4, offset);
        break;
    case OPC_BGEUC:
    case OPC_BLTUC:
        if (rs == 0 || rs == rt) {
            /* OPC_BLEZALC, OPC_BGEZALC */
            /* OPC_BGTZALC, OPC_BLTZALC */
            tcg_gen_movi_tl(cpu_gpr[31], ctx->base.pc_next + 4);
        }
        gen_load_gpr(t0, rs);
        gen_load_gpr(t1, rt);
        bcond_compute = 1;
        ctx->btarget = addr_add(ctx, ctx->base.pc_next + 4, offset);
        break;
    case OPC_BC:
        ctx->btarget = addr_add(ctx, ctx->base.pc_next + 4, offset);
        break;
    case OPC_BEQZC:
        if (rs != 0) {
            /* OPC_BEQZC, OPC_BNEZC */
            gen_load_gpr(t0, rs);
            bcond_compute = 1;
            ctx->btarget = addr_add(ctx, ctx->base.pc_next + 4, offset);
        } else {
            /* OPC_JIC, OPC_JIALC */
            TCGv tbase = tcg_temp_new();
            TCGv toffset = tcg_temp_new();

            gen_load_gpr(tbase, rt);
            tcg_gen_movi_tl(toffset, offset);
            gen_op_addr_add(ctx, btarget, tbase, toffset);
            tcg_temp_free(tbase);
            tcg_temp_free(toffset);
        }
        break;
    default:
        MIPS_INVAL("Compact branch/jump");
        gen_reserved_instruction(ctx);
        goto out;
    }

    if (bcond_compute == 0) {
        /* Uncoditional compact branch */
        switch (opc) {
        case OPC_BC:
            gen_goto_tb(ctx, 0, ctx->btarget);
            break;
        default:
            MIPS_INVAL("Compact branch/jump");
            gen_reserved_instruction(ctx);
            goto out;
        }
    } else {
        /* Conditional compact branch */
        TCGLabel *fs = gen_new_label();

        switch (opc) {
        case OPC_BGEUC:
            if (rs == 0 && rt != 0) {
                /* OPC_BLEZALC */
                tcg_gen_brcondi_tl(tcg_invert_cond(TCG_COND_LE), t1, 0, fs);
            } else if (rs != 0 && rt != 0 && rs == rt) {
                /* OPC_BGEZALC */
                tcg_gen_brcondi_tl(tcg_invert_cond(TCG_COND_GE), t1, 0, fs);
            } else {
                /* OPC_BGEUC */
                tcg_gen_brcond_tl(tcg_invert_cond(TCG_COND_GEU), t0, t1, fs);
            }
            break;
        case OPC_BLTUC:
            if (rs == 0 && rt != 0) {
                /* OPC_BGTZALC */
                tcg_gen_brcondi_tl(tcg_invert_cond(TCG_COND_GT), t1, 0, fs);
            } else if (rs != 0 && rt != 0 && rs == rt) {
                /* OPC_BLTZALC */
                tcg_gen_brcondi_tl(tcg_invert_cond(TCG_COND_LT), t1, 0, fs);
            } else {
                /* OPC_BLTUC */
                tcg_gen_brcond_tl(tcg_invert_cond(TCG_COND_LTU), t0, t1, fs);
            }
            break;
        case OPC_BGEC:
            if (rs == 0 && rt != 0) {
                /* OPC_BLEZC */
                tcg_gen_brcondi_tl(tcg_invert_cond(TCG_COND_LE), t1, 0, fs);
            } else if (rs != 0 && rt != 0 && rs == rt) {
                /* OPC_BGEZC */
                tcg_gen_brcondi_tl(tcg_invert_cond(TCG_COND_GE), t1, 0, fs);
            } else {
                /* OPC_BGEC */
                tcg_gen_brcond_tl(tcg_invert_cond(TCG_COND_GE), t0, t1, fs);
            }
            break;
        case OPC_BLTC:
            if (rs == 0 && rt != 0) {
                /* OPC_BGTZC */
                tcg_gen_brcondi_tl(tcg_invert_cond(TCG_COND_GT), t1, 0, fs);
            } else if (rs != 0 && rt != 0 && rs == rt) {
                /* OPC_BLTZC */
                tcg_gen_brcondi_tl(tcg_invert_cond(TCG_COND_LT), t1, 0, fs);
            } else {
                /* OPC_BLTC */
                tcg_gen_brcond_tl(tcg_invert_cond(TCG_COND_LT), t0, t1, fs);
            }
            break;
        case OPC_BEQZC:
            tcg_gen_brcondi_tl(tcg_invert_cond(TCG_COND_EQ), t0, 0, fs);
            break;
        default:
            MIPS_INVAL("Compact conditional branch/jump");
            gen_reserved_instruction(ctx);
            goto out;
        }

        /* branch completion */
        clear_branch_hflags(ctx);
        ctx->base.is_jmp = DISAS_NORETURN;

        /* Generating branch here as compact branches don't have delay slot */
        gen_goto_tb(ctx, 1, ctx->btarget);
        gen_set_label(fs);

        gen_goto_tb(ctx, 0, ctx->base.pc_next + 4);
    }

out:
    tcg_temp_free(t0);
    tcg_temp_free(t1);
}


/* nanoMIPS CP1 Branches */
static void gen_compute_branch_cp1_nm(DisasContext *ctx, uint32_t op,
                                   int32_t ft, int32_t offset)
{
    target_ulong btarget;
    TCGv_i64 t0 = tcg_temp_new_i64();

    gen_load_fpr64(ctx, t0, ft);
    tcg_gen_andi_i64(t0, t0, 1);

    btarget = addr_add(ctx, ctx->base.pc_next + 4, offset);

    switch (op) {
    case NM_BC1EQZC:
        tcg_gen_xori_i64(t0, t0, 1);
        ctx->hflags |= MIPS_HFLAG_BC;
        break;
    case NM_BC1NEZC:
        /* t0 already set */
        ctx->hflags |= MIPS_HFLAG_BC;
        break;
    default:
        MIPS_INVAL("cp1 cond branch");
        gen_reserved_instruction(ctx);
        goto out;
    }

    tcg_gen_trunc_i64_tl(bcond, t0);

    ctx->btarget = btarget;

out:
    tcg_temp_free_i64(t0);
}


static void gen_p_lsx(DisasContext *ctx, int rd, int rs, int rt)
{
    TCGv t0, t1;
    t0 = tcg_temp_new();
    t1 = tcg_temp_new();

    gen_load_gpr(t0, rs);
    gen_load_gpr(t1, rt);

    if ((extract32(ctx->opcode, 6, 1)) == 1) {
        /* PP.LSXS instructions require shifting */
        switch (extract32(ctx->opcode, 7, 4)) {
        case NM_SHXS:
            check_nms(ctx);
            /* fall through */
        case NM_LHXS:
        case NM_LHUXS:
            tcg_gen_shli_tl(t0, t0, 1);
            break;
        case NM_SWXS:
            check_nms(ctx);
            /* fall through */
        case NM_LWXS:
        case NM_LWC1XS:
        case NM_SWC1XS:
            tcg_gen_shli_tl(t0, t0, 2);
            break;
        case NM_LDC1XS:
        case NM_SDC1XS:
            tcg_gen_shli_tl(t0, t0, 3);
            break;
        }
    }
    gen_op_addr_add(ctx, t0, t0, t1);

    switch (extract32(ctx->opcode, 7, 4)) {
    case NM_LBX:
        tcg_gen_qemu_ld_tl(t0, t0, ctx->mem_idx,
                           MO_SB);
        gen_store_gpr(t0, rd);
        break;
    case NM_LHX:
    /*case NM_LHXS:*/
        tcg_gen_qemu_ld_tl(t0, t0, ctx->mem_idx,
                           MO_TESW);
        gen_store_gpr(t0, rd);
        break;
    case NM_LWX:
    /*case NM_LWXS:*/
        tcg_gen_qemu_ld_tl(t0, t0, ctx->mem_idx,
                           MO_TESL);
        gen_store_gpr(t0, rd);
        break;
    case NM_LBUX:
        tcg_gen_qemu_ld_tl(t0, t0, ctx->mem_idx,
                           MO_UB);
        gen_store_gpr(t0, rd);
        break;
    case NM_LHUX:
    /*case NM_LHUXS:*/
        tcg_gen_qemu_ld_tl(t0, t0, ctx->mem_idx,
                           MO_TEUW);
        gen_store_gpr(t0, rd);
        break;
    case NM_SBX:
        check_nms(ctx);
        gen_load_gpr(t1, rd);
        tcg_gen_qemu_st_tl(t1, t0, ctx->mem_idx,
                           MO_8);
        break;
    case NM_SHX:
    /*case NM_SHXS:*/
        check_nms(ctx);
        gen_load_gpr(t1, rd);
        tcg_gen_qemu_st_tl(t1, t0, ctx->mem_idx,
                           MO_TEUW);
        break;
    case NM_SWX:
    /*case NM_SWXS:*/
        check_nms(ctx);
        gen_load_gpr(t1, rd);
        tcg_gen_qemu_st_tl(t1, t0, ctx->mem_idx,
                           MO_TEUL);
        break;
    case NM_LWC1X:
    /*case NM_LWC1XS:*/
    case NM_LDC1X:
    /*case NM_LDC1XS:*/
    case NM_SWC1X:
    /*case NM_SWC1XS:*/
    case NM_SDC1X:
    /*case NM_SDC1XS:*/
        if (ctx->CP0_Config1 & (1 << CP0C1_FP)) {
            check_cp1_enabled(ctx);
            switch (extract32(ctx->opcode, 7, 4)) {
            case NM_LWC1X:
            /*case NM_LWC1XS:*/
                gen_flt_ldst(ctx, OPC_LWC1, rd, t0);
                break;
            case NM_LDC1X:
            /*case NM_LDC1XS:*/
                gen_flt_ldst(ctx, OPC_LDC1, rd, t0);
                break;
            case NM_SWC1X:
            /*case NM_SWC1XS:*/
                gen_flt_ldst(ctx, OPC_SWC1, rd, t0);
                break;
            case NM_SDC1X:
            /*case NM_SDC1XS:*/
                gen_flt_ldst(ctx, OPC_SDC1, rd, t0);
                break;
            }
        } else {
            generate_exception_err(ctx, EXCP_CpU, 1);
        }
        break;
    default:
        gen_reserved_instruction(ctx);
        break;
    }

    tcg_temp_free(t0);
    tcg_temp_free(t1);
}

static void gen_pool32f_nanomips_insn(DisasContext *ctx)
{
    int rt, rs, rd;

    rt = extract32(ctx->opcode, 21, 5);
    rs = extract32(ctx->opcode, 16, 5);
    rd = extract32(ctx->opcode, 11, 5);

    if (!(ctx->CP0_Config1 & (1 << CP0C1_FP))) {
        gen_reserved_instruction(ctx);
        return;
    }
    check_cp1_enabled(ctx);
    switch (extract32(ctx->opcode, 0, 3)) {
    case NM_POOL32F_0:
        switch (extract32(ctx->opcode, 3, 7)) {
        case NM_RINT_S:
            gen_farith(ctx, OPC_RINT_S, 0, rt, rs, 0);
            break;
        case NM_RINT_D:
            gen_farith(ctx, OPC_RINT_D, 0, rt, rs, 0);
            break;
        case NM_CLASS_S:
            gen_farith(ctx, OPC_CLASS_S, 0, rt, rs, 0);
            break;
        case NM_CLASS_D:
            gen_farith(ctx, OPC_CLASS_D, 0, rt, rs, 0);
            break;
        case NM_ADD_S:
            gen_farith(ctx, OPC_ADD_S, rt, rs, rd, 0);
            break;
        case NM_ADD_D:
            gen_farith(ctx, OPC_ADD_D, rt, rs, rd, 0);
            break;
        case NM_SUB_S:
            gen_farith(ctx, OPC_SUB_S, rt, rs, rd, 0);
            break;
        case NM_SUB_D:
            gen_farith(ctx, OPC_SUB_D, rt, rs, rd, 0);
            break;
        case NM_MUL_S:
            gen_farith(ctx, OPC_MUL_S, rt, rs, rd, 0);
            break;
        case NM_MUL_D:
            gen_farith(ctx, OPC_MUL_D, rt, rs, rd, 0);
            break;
        case NM_DIV_S:
            gen_farith(ctx, OPC_DIV_S, rt, rs, rd, 0);
            break;
        case NM_DIV_D:
            gen_farith(ctx, OPC_DIV_D, rt, rs, rd, 0);
            break;
        case NM_SELEQZ_S:
            gen_sel_s(ctx, OPC_SELEQZ_S, rd, rt, rs);
            break;
        case NM_SELEQZ_D:
            gen_sel_d(ctx, OPC_SELEQZ_D, rd, rt, rs);
            break;
        case NM_SELNEZ_S:
            gen_sel_s(ctx, OPC_SELNEZ_S, rd, rt, rs);
            break;
        case NM_SELNEZ_D:
            gen_sel_d(ctx, OPC_SELNEZ_D, rd, rt, rs);
            break;
        case NM_SEL_S:
            gen_sel_s(ctx, OPC_SEL_S, rd, rt, rs);
            break;
        case NM_SEL_D:
            gen_sel_d(ctx, OPC_SEL_D, rd, rt, rs);
            break;
        case NM_MADDF_S:
            gen_farith(ctx, OPC_MADDF_S, rt, rs, rd, 0);
            break;
        case NM_MADDF_D:
            gen_farith(ctx, OPC_MADDF_D, rt, rs, rd, 0);
            break;
        case NM_MSUBF_S:
            gen_farith(ctx, OPC_MSUBF_S, rt, rs, rd, 0);
            break;
        case NM_MSUBF_D:
            gen_farith(ctx, OPC_MSUBF_D, rt, rs, rd, 0);
            break;
        default:
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case NM_POOL32F_3:
        switch (extract32(ctx->opcode, 3, 3)) {
        case NM_MIN_FMT:
            switch (extract32(ctx->opcode, 9, 1)) {
            case FMT_SDPS_S:
                gen_farith(ctx, OPC_MIN_S, rt, rs, rd, 0);
                break;
            case FMT_SDPS_D:
                gen_farith(ctx, OPC_MIN_D, rt, rs, rd, 0);
                break;
            }
            break;
        case NM_MAX_FMT:
            switch (extract32(ctx->opcode, 9, 1)) {
            case FMT_SDPS_S:
                gen_farith(ctx, OPC_MAX_S, rt, rs, rd, 0);
                break;
            case FMT_SDPS_D:
                gen_farith(ctx, OPC_MAX_D, rt, rs, rd, 0);
                break;
            }
            break;
        case NM_MINA_FMT:
            switch (extract32(ctx->opcode, 9, 1)) {
            case FMT_SDPS_S:
                gen_farith(ctx, OPC_MINA_S, rt, rs, rd, 0);
                break;
            case FMT_SDPS_D:
                gen_farith(ctx, OPC_MINA_D, rt, rs, rd, 0);
                break;
            }
            break;
        case NM_MAXA_FMT:
            switch (extract32(ctx->opcode, 9, 1)) {
            case FMT_SDPS_S:
                gen_farith(ctx, OPC_MAXA_S, rt, rs, rd, 0);
                break;
            case FMT_SDPS_D:
                gen_farith(ctx, OPC_MAXA_D, rt, rs, rd, 0);
                break;
            }
            break;
        case NM_POOL32FXF:
            switch (extract32(ctx->opcode, 6, 8)) {
            case NM_CFC1:
                gen_cp1(ctx, OPC_CFC1, rt, rs);
                break;
            case NM_CTC1:
                gen_cp1(ctx, OPC_CTC1, rt, rs);
                break;
            case NM_MFC1:
                gen_cp1(ctx, OPC_MFC1, rt, rs);
                break;
            case NM_MTC1:
                gen_cp1(ctx, OPC_MTC1, rt, rs);
                break;
            case NM_MFHC1:
                gen_cp1(ctx, OPC_MFHC1, rt, rs);
                break;
            case NM_MTHC1:
                gen_cp1(ctx, OPC_MTHC1, rt, rs);
                break;
            case NM_CVT_S_PL:
                gen_farith(ctx, OPC_CVT_S_PL, -1, rs, rt, 0);
                break;
            case NM_CVT_S_PU:
                gen_farith(ctx, OPC_CVT_S_PU, -1, rs, rt, 0);
                break;
            default:
                switch (extract32(ctx->opcode, 6, 9)) {
                case NM_CVT_L_S:
                    gen_farith(ctx, OPC_CVT_L_S, -1, rs, rt, 0);
                    break;
                case NM_CVT_L_D:
                    gen_farith(ctx, OPC_CVT_L_D, -1, rs, rt, 0);
                    break;
                case NM_CVT_W_S:
                    gen_farith(ctx, OPC_CVT_W_S, -1, rs, rt, 0);
                    break;
                case NM_CVT_W_D:
                    gen_farith(ctx, OPC_CVT_W_D, -1, rs, rt, 0);
                    break;
                case NM_RSQRT_S:
                    gen_farith(ctx, OPC_RSQRT_S, -1, rs, rt, 0);
                    break;
                case NM_RSQRT_D:
                    gen_farith(ctx, OPC_RSQRT_D, -1, rs, rt, 0);
                    break;
                case NM_SQRT_S:
                    gen_farith(ctx, OPC_SQRT_S, -1, rs, rt, 0);
                    break;
                case NM_SQRT_D:
                    gen_farith(ctx, OPC_SQRT_D, -1, rs, rt, 0);
                    break;
                case NM_RECIP_S:
                    gen_farith(ctx, OPC_RECIP_S, -1, rs, rt, 0);
                    break;
                case NM_RECIP_D:
                    gen_farith(ctx, OPC_RECIP_D, -1, rs, rt, 0);
                    break;
                case NM_FLOOR_L_S:
                    gen_farith(ctx, OPC_FLOOR_L_S, -1, rs, rt, 0);
                    break;
                case NM_FLOOR_L_D:
                    gen_farith(ctx, OPC_FLOOR_L_D, -1, rs, rt, 0);
                    break;
                case NM_FLOOR_W_S:
                    gen_farith(ctx, OPC_FLOOR_W_S, -1, rs, rt, 0);
                    break;
                case NM_FLOOR_W_D:
                    gen_farith(ctx, OPC_FLOOR_W_D, -1, rs, rt, 0);
                    break;
                case NM_CEIL_L_S:
                    gen_farith(ctx, OPC_CEIL_L_S, -1, rs, rt, 0);
                    break;
                case NM_CEIL_L_D:
                    gen_farith(ctx, OPC_CEIL_L_D, -1, rs, rt, 0);
                    break;
                case NM_CEIL_W_S:
                    gen_farith(ctx, OPC_CEIL_W_S, -1, rs, rt, 0);
                    break;
                case NM_CEIL_W_D:
                    gen_farith(ctx, OPC_CEIL_W_D, -1, rs, rt, 0);
                    break;
                case NM_TRUNC_L_S:
                    gen_farith(ctx, OPC_TRUNC_L_S, -1, rs, rt, 0);
                    break;
                case NM_TRUNC_L_D:
                    gen_farith(ctx, OPC_TRUNC_L_D, -1, rs, rt, 0);
                    break;
                case NM_TRUNC_W_S:
                    gen_farith(ctx, OPC_TRUNC_W_S, -1, rs, rt, 0);
                    break;
                case NM_TRUNC_W_D:
                    gen_farith(ctx, OPC_TRUNC_W_D, -1, rs, rt, 0);
                    break;
                case NM_ROUND_L_S:
                    gen_farith(ctx, OPC_ROUND_L_S, -1, rs, rt, 0);
                    break;
                case NM_ROUND_L_D:
                    gen_farith(ctx, OPC_ROUND_L_D, -1, rs, rt, 0);
                    break;
                case NM_ROUND_W_S:
                    gen_farith(ctx, OPC_ROUND_W_S, -1, rs, rt, 0);
                    break;
                case NM_ROUND_W_D:
                    gen_farith(ctx, OPC_ROUND_W_D, -1, rs, rt, 0);
                    break;
                case NM_MOV_S:
                    gen_farith(ctx, OPC_MOV_S, -1, rs, rt, 0);
                    break;
                case NM_MOV_D:
                    gen_farith(ctx, OPC_MOV_D, -1, rs, rt, 0);
                    break;
                case NM_ABS_S:
                    gen_farith(ctx, OPC_ABS_S, -1, rs, rt, 0);
                    break;
                case NM_ABS_D:
                    gen_farith(ctx, OPC_ABS_D, -1, rs, rt, 0);
                    break;
                case NM_NEG_S:
                    gen_farith(ctx, OPC_NEG_S, -1, rs, rt, 0);
                    break;
                case NM_NEG_D:
                    gen_farith(ctx, OPC_NEG_D, -1, rs, rt, 0);
                    break;
                case NM_CVT_D_S:
                    gen_farith(ctx, OPC_CVT_D_S, -1, rs, rt, 0);
                    break;
                case NM_CVT_D_W:
                    gen_farith(ctx, OPC_CVT_D_W, -1, rs, rt, 0);
                    break;
                case NM_CVT_D_L:
                    gen_farith(ctx, OPC_CVT_D_L, -1, rs, rt, 0);
                    break;
                case NM_CVT_S_D:
                    gen_farith(ctx, OPC_CVT_S_D, -1, rs, rt, 0);
                    break;
                case NM_CVT_S_W:
                    gen_farith(ctx, OPC_CVT_S_W, -1, rs, rt, 0);
                    break;
                case NM_CVT_S_L:
                    gen_farith(ctx, OPC_CVT_S_L, -1, rs, rt, 0);
                    break;
                default:
                    gen_reserved_instruction(ctx);
                    break;
                }
                break;
            }
            break;
        }
        break;
    case NM_POOL32F_5:
        switch (extract32(ctx->opcode, 3, 3)) {
        case NM_CMP_CONDN_S:
            gen_r6_cmp_s(ctx, extract32(ctx->opcode, 6, 5), rt, rs, rd);
            break;
        case NM_CMP_CONDN_D:
            gen_r6_cmp_d(ctx, extract32(ctx->opcode, 6, 5), rt, rs, rd);
            break;
        default:
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    default:
        gen_reserved_instruction(ctx);
        break;
    }
}

static void gen_pool32a5_nanomips_insn(DisasContext *ctx, int opc,
                                       int rd, int rs, int rt)
{
    int ret = rd;
    TCGv t0 = tcg_temp_new();
    TCGv v1_t = tcg_temp_new();
    TCGv v2_t = tcg_temp_new();

    gen_load_gpr(v1_t, rs);
    gen_load_gpr(v2_t, rt);

    switch (opc) {
    case NM_CMP_EQ_PH:
        check_dsp(ctx);
        gen_helper_cmp_eq_ph(v1_t, v2_t, cpu_env);
        break;
    case NM_CMP_LT_PH:
        check_dsp(ctx);
        gen_helper_cmp_lt_ph(v1_t, v2_t, cpu_env);
        break;
    case NM_CMP_LE_PH:
        check_dsp(ctx);
        gen_helper_cmp_le_ph(v1_t, v2_t, cpu_env);
        break;
    case NM_CMPU_EQ_QB:
        check_dsp(ctx);
        gen_helper_cmpu_eq_qb(v1_t, v2_t, cpu_env);
        break;
    case NM_CMPU_LT_QB:
        check_dsp(ctx);
        gen_helper_cmpu_lt_qb(v1_t, v2_t, cpu_env);
        break;
    case NM_CMPU_LE_QB:
        check_dsp(ctx);
        gen_helper_cmpu_le_qb(v1_t, v2_t, cpu_env);
        break;
    case NM_CMPGU_EQ_QB:
        check_dsp(ctx);
        gen_helper_cmpgu_eq_qb(v1_t, v1_t, v2_t);
        gen_store_gpr(v1_t, ret);
        break;
    case NM_CMPGU_LT_QB:
        check_dsp(ctx);
        gen_helper_cmpgu_lt_qb(v1_t, v1_t, v2_t);
        gen_store_gpr(v1_t, ret);
        break;
    case NM_CMPGU_LE_QB:
        check_dsp(ctx);
        gen_helper_cmpgu_le_qb(v1_t, v1_t, v2_t);
        gen_store_gpr(v1_t, ret);
        break;
    case NM_CMPGDU_EQ_QB:
        check_dsp_r2(ctx);
        gen_helper_cmpgu_eq_qb(v1_t, v1_t, v2_t);
        tcg_gen_deposit_tl(cpu_dspctrl, cpu_dspctrl, v1_t, 24, 4);
        gen_store_gpr(v1_t, ret);
        break;
    case NM_CMPGDU_LT_QB:
        check_dsp_r2(ctx);
        gen_helper_cmpgu_lt_qb(v1_t, v1_t, v2_t);
        tcg_gen_deposit_tl(cpu_dspctrl, cpu_dspctrl, v1_t, 24, 4);
        gen_store_gpr(v1_t, ret);
        break;
    case NM_CMPGDU_LE_QB:
        check_dsp_r2(ctx);
        gen_helper_cmpgu_le_qb(v1_t, v1_t, v2_t);
        tcg_gen_deposit_tl(cpu_dspctrl, cpu_dspctrl, v1_t, 24, 4);
        gen_store_gpr(v1_t, ret);
        break;
    case NM_PACKRL_PH:
        check_dsp(ctx);
        gen_helper_packrl_ph(v1_t, v1_t, v2_t);
        gen_store_gpr(v1_t, ret);
        break;
    case NM_PICK_QB:
        check_dsp(ctx);
        gen_helper_pick_qb(v1_t, v1_t, v2_t, cpu_env);
        gen_store_gpr(v1_t, ret);
        break;
    case NM_PICK_PH:
        check_dsp(ctx);
        gen_helper_pick_ph(v1_t, v1_t, v2_t, cpu_env);
        gen_store_gpr(v1_t, ret);
        break;
    case NM_ADDQ_S_W:
        check_dsp(ctx);
        gen_helper_addq_s_w(v1_t, v1_t, v2_t, cpu_env);
        gen_store_gpr(v1_t, ret);
        break;
    case NM_SUBQ_S_W:
        check_dsp(ctx);
        gen_helper_subq_s_w(v1_t, v1_t, v2_t, cpu_env);
        gen_store_gpr(v1_t, ret);
        break;
    case NM_ADDSC:
        check_dsp(ctx);
        gen_helper_addsc(v1_t, v1_t, v2_t, cpu_env);
        gen_store_gpr(v1_t, ret);
        break;
    case NM_ADDWC:
        check_dsp(ctx);
        gen_helper_addwc(v1_t, v1_t, v2_t, cpu_env);
        gen_store_gpr(v1_t, ret);
        break;
    case NM_ADDQ_S_PH:
        check_dsp(ctx);
        switch (extract32(ctx->opcode, 10, 1)) {
        case 0:
            /* ADDQ_PH */
            gen_helper_addq_ph(v1_t, v1_t, v2_t, cpu_env);
            gen_store_gpr(v1_t, ret);
            break;
        case 1:
            /* ADDQ_S_PH */
            gen_helper_addq_s_ph(v1_t, v1_t, v2_t, cpu_env);
            gen_store_gpr(v1_t, ret);
            break;
        }
        break;
    case NM_ADDQH_R_PH:
        check_dsp_r2(ctx);
        switch (extract32(ctx->opcode, 10, 1)) {
        case 0:
            /* ADDQH_PH */
            gen_helper_addqh_ph(v1_t, v1_t, v2_t);
            gen_store_gpr(v1_t, ret);
            break;
        case 1:
            /* ADDQH_R_PH */
            gen_helper_addqh_r_ph(v1_t, v1_t, v2_t);
            gen_store_gpr(v1_t, ret);
            break;
        }
        break;
    case NM_ADDQH_R_W:
        check_dsp_r2(ctx);
        switch (extract32(ctx->opcode, 10, 1)) {
        case 0:
            /* ADDQH_W */
            gen_helper_addqh_w(v1_t, v1_t, v2_t);
            gen_store_gpr(v1_t, ret);
            break;
        case 1:
            /* ADDQH_R_W */
            gen_helper_addqh_r_w(v1_t, v1_t, v2_t);
            gen_store_gpr(v1_t, ret);
            break;
        }
        break;
    case NM_ADDU_S_QB:
        check_dsp(ctx);
        switch (extract32(ctx->opcode, 10, 1)) {
        case 0:
            /* ADDU_QB */
            gen_helper_addu_qb(v1_t, v1_t, v2_t, cpu_env);
            gen_store_gpr(v1_t, ret);
            break;
        case 1:
            /* ADDU_S_QB */
            gen_helper_addu_s_qb(v1_t, v1_t, v2_t, cpu_env);
            gen_store_gpr(v1_t, ret);
            break;
        }
        break;
    case NM_ADDU_S_PH:
        check_dsp_r2(ctx);
        switch (extract32(ctx->opcode, 10, 1)) {
        case 0:
            /* ADDU_PH */
            gen_helper_addu_ph(v1_t, v1_t, v2_t, cpu_env);
            gen_store_gpr(v1_t, ret);
            break;
        case 1:
            /* ADDU_S_PH */
            gen_helper_addu_s_ph(v1_t, v1_t, v2_t, cpu_env);
            gen_store_gpr(v1_t, ret);
            break;
        }
        break;
    case NM_ADDUH_R_QB:
        check_dsp_r2(ctx);
        switch (extract32(ctx->opcode, 10, 1)) {
        case 0:
            /* ADDUH_QB */
            gen_helper_adduh_qb(v1_t, v1_t, v2_t);
            gen_store_gpr(v1_t, ret);
            break;
        case 1:
            /* ADDUH_R_QB */
            gen_helper_adduh_r_qb(v1_t, v1_t, v2_t);
            gen_store_gpr(v1_t, ret);
            break;
        }
        break;
    case NM_SHRAV_R_PH:
        check_dsp(ctx);
        switch (extract32(ctx->opcode, 10, 1)) {
        case 0:
            /* SHRAV_PH */
            gen_helper_shra_ph(v1_t, v1_t, v2_t);
            gen_store_gpr(v1_t, ret);
            break;
        case 1:
            /* SHRAV_R_PH */
            gen_helper_shra_r_ph(v1_t, v1_t, v2_t);
            gen_store_gpr(v1_t, ret);
            break;
        }
        break;
    case NM_SHRAV_R_QB:
        check_dsp_r2(ctx);
        switch (extract32(ctx->opcode, 10, 1)) {
        case 0:
            /* SHRAV_QB */
            gen_helper_shra_qb(v1_t, v1_t, v2_t);
            gen_store_gpr(v1_t, ret);
            break;
        case 1:
            /* SHRAV_R_QB */
            gen_helper_shra_r_qb(v1_t, v1_t, v2_t);
            gen_store_gpr(v1_t, ret);
            break;
        }
        break;
    case NM_SUBQ_S_PH:
        check_dsp(ctx);
        switch (extract32(ctx->opcode, 10, 1)) {
        case 0:
            /* SUBQ_PH */
            gen_helper_subq_ph(v1_t, v1_t, v2_t, cpu_env);
            gen_store_gpr(v1_t, ret);
            break;
        case 1:
            /* SUBQ_S_PH */
            gen_helper_subq_s_ph(v1_t, v1_t, v2_t, cpu_env);
            gen_store_gpr(v1_t, ret);
            break;
        }
        break;
    case NM_SUBQH_R_PH:
        check_dsp_r2(ctx);
        switch (extract32(ctx->opcode, 10, 1)) {
        case 0:
            /* SUBQH_PH */
            gen_helper_subqh_ph(v1_t, v1_t, v2_t);
            gen_store_gpr(v1_t, ret);
            break;
        case 1:
            /* SUBQH_R_PH */
            gen_helper_subqh_r_ph(v1_t, v1_t, v2_t);
            gen_store_gpr(v1_t, ret);
            break;
        }
        break;
    case NM_SUBQH_R_W:
        check_dsp_r2(ctx);
        switch (extract32(ctx->opcode, 10, 1)) {
        case 0:
            /* SUBQH_W */
            gen_helper_subqh_w(v1_t, v1_t, v2_t);
            gen_store_gpr(v1_t, ret);
            break;
        case 1:
            /* SUBQH_R_W */
            gen_helper_subqh_r_w(v1_t, v1_t, v2_t);
            gen_store_gpr(v1_t, ret);
            break;
        }
        break;
    case NM_SUBU_S_QB:
        check_dsp(ctx);
        switch (extract32(ctx->opcode, 10, 1)) {
        case 0:
            /* SUBU_QB */
            gen_helper_subu_qb(v1_t, v1_t, v2_t, cpu_env);
            gen_store_gpr(v1_t, ret);
            break;
        case 1:
            /* SUBU_S_QB */
            gen_helper_subu_s_qb(v1_t, v1_t, v2_t, cpu_env);
            gen_store_gpr(v1_t, ret);
            break;
        }
        break;
    case NM_SUBU_S_PH:
        check_dsp_r2(ctx);
        switch (extract32(ctx->opcode, 10, 1)) {
        case 0:
            /* SUBU_PH */
            gen_helper_subu_ph(v1_t, v1_t, v2_t, cpu_env);
            gen_store_gpr(v1_t, ret);
            break;
        case 1:
            /* SUBU_S_PH */
            gen_helper_subu_s_ph(v1_t, v1_t, v2_t, cpu_env);
            gen_store_gpr(v1_t, ret);
            break;
        }
        break;
    case NM_SUBUH_R_QB:
        check_dsp_r2(ctx);
        switch (extract32(ctx->opcode, 10, 1)) {
        case 0:
            /* SUBUH_QB */
            gen_helper_subuh_qb(v1_t, v1_t, v2_t);
            gen_store_gpr(v1_t, ret);
            break;
        case 1:
            /* SUBUH_R_QB */
            gen_helper_subuh_r_qb(v1_t, v1_t, v2_t);
            gen_store_gpr(v1_t, ret);
            break;
        }
        break;
    case NM_SHLLV_S_PH:
        check_dsp(ctx);
        switch (extract32(ctx->opcode, 10, 1)) {
        case 0:
            /* SHLLV_PH */
            gen_helper_shll_ph(v1_t, v1_t, v2_t, cpu_env);
            gen_store_gpr(v1_t, ret);
            break;
        case 1:
            /* SHLLV_S_PH */
            gen_helper_shll_s_ph(v1_t, v1_t, v2_t, cpu_env);
            gen_store_gpr(v1_t, ret);
            break;
        }
        break;
    case NM_PRECR_SRA_R_PH_W:
        check_dsp_r2(ctx);
        switch (extract32(ctx->opcode, 10, 1)) {
        case 0:
            /* PRECR_SRA_PH_W */
            {
                TCGv_i32 sa_t = tcg_const_i32(rd);
                gen_helper_precr_sra_ph_w(v1_t, sa_t, v1_t,
                                          cpu_gpr[rt]);
                gen_store_gpr(v1_t, rt);
                tcg_temp_free_i32(sa_t);
            }
            break;
        case 1:
            /* PRECR_SRA_R_PH_W */
            {
                TCGv_i32 sa_t = tcg_const_i32(rd);
                gen_helper_precr_sra_r_ph_w(v1_t, sa_t, v1_t,
                                            cpu_gpr[rt]);
                gen_store_gpr(v1_t, rt);
                tcg_temp_free_i32(sa_t);
            }
            break;
       }
        break;
    case NM_MULEU_S_PH_QBL:
        check_dsp(ctx);
        gen_helper_muleu_s_ph_qbl(v1_t, v1_t, v2_t, cpu_env);
        gen_store_gpr(v1_t, ret);
        break;
    case NM_MULEU_S_PH_QBR:
        check_dsp(ctx);
        gen_helper_muleu_s_ph_qbr(v1_t, v1_t, v2_t, cpu_env);
        gen_store_gpr(v1_t, ret);
        break;
    case NM_MULQ_RS_PH:
        check_dsp(ctx);
        gen_helper_mulq_rs_ph(v1_t, v1_t, v2_t, cpu_env);
        gen_store_gpr(v1_t, ret);
        break;
    case NM_MULQ_S_PH:
        check_dsp_r2(ctx);
        gen_helper_mulq_s_ph(v1_t, v1_t, v2_t, cpu_env);
        gen_store_gpr(v1_t, ret);
        break;
    case NM_MULQ_RS_W:
        check_dsp_r2(ctx);
        gen_helper_mulq_rs_w(v1_t, v1_t, v2_t, cpu_env);
        gen_store_gpr(v1_t, ret);
        break;
    case NM_MULQ_S_W:
        check_dsp_r2(ctx);
        gen_helper_mulq_s_w(v1_t, v1_t, v2_t, cpu_env);
        gen_store_gpr(v1_t, ret);
        break;
    case NM_APPEND:
        check_dsp_r2(ctx);
        gen_load_gpr(t0, rs);
        if (rd != 0) {
            tcg_gen_deposit_tl(cpu_gpr[rt], t0, cpu_gpr[rt], rd, 32 - rd);
        }
        tcg_gen_ext32s_tl(cpu_gpr[rt], cpu_gpr[rt]);
        break;
    case NM_MODSUB:
        check_dsp(ctx);
        gen_helper_modsub(v1_t, v1_t, v2_t);
        gen_store_gpr(v1_t, ret);
        break;
    case NM_SHRAV_R_W:
        check_dsp(ctx);
        gen_helper_shra_r_w(v1_t, v1_t, v2_t);
        gen_store_gpr(v1_t, ret);
        break;
    case NM_SHRLV_PH:
        check_dsp_r2(ctx);
        gen_helper_shrl_ph(v1_t, v1_t, v2_t);
        gen_store_gpr(v1_t, ret);
        break;
    case NM_SHRLV_QB:
        check_dsp(ctx);
        gen_helper_shrl_qb(v1_t, v1_t, v2_t);
        gen_store_gpr(v1_t, ret);
        break;
    case NM_SHLLV_QB:
        check_dsp(ctx);
        gen_helper_shll_qb(v1_t, v1_t, v2_t, cpu_env);
        gen_store_gpr(v1_t, ret);
        break;
    case NM_SHLLV_S_W:
        check_dsp(ctx);
        gen_helper_shll_s_w(v1_t, v1_t, v2_t, cpu_env);
        gen_store_gpr(v1_t, ret);
        break;
    case NM_SHILO:
        check_dsp(ctx);
        {
            TCGv tv0 = tcg_temp_new();
            TCGv tv1 = tcg_temp_new();
            int16_t imm = extract32(ctx->opcode, 16, 7);

            tcg_gen_movi_tl(tv0, rd >> 3);
            tcg_gen_movi_tl(tv1, imm);
            gen_helper_shilo(tv0, tv1, cpu_env);
        }
        break;
    case NM_MULEQ_S_W_PHL:
        check_dsp(ctx);
        gen_helper_muleq_s_w_phl(v1_t, v1_t, v2_t, cpu_env);
        gen_store_gpr(v1_t, ret);
        break;
    case NM_MULEQ_S_W_PHR:
        check_dsp(ctx);
        gen_helper_muleq_s_w_phr(v1_t, v1_t, v2_t, cpu_env);
        gen_store_gpr(v1_t, ret);
        break;
    case NM_MUL_S_PH:
        check_dsp_r2(ctx);
        switch (extract32(ctx->opcode, 10, 1)) {
        case 0:
            /* MUL_PH */
            gen_helper_mul_ph(v1_t, v1_t, v2_t, cpu_env);
            gen_store_gpr(v1_t, ret);
            break;
        case 1:
            /* MUL_S_PH */
            gen_helper_mul_s_ph(v1_t, v1_t, v2_t, cpu_env);
            gen_store_gpr(v1_t, ret);
            break;
        }
        break;
    case NM_PRECR_QB_PH:
        check_dsp_r2(ctx);
        gen_helper_precr_qb_ph(v1_t, v1_t, v2_t);
        gen_store_gpr(v1_t, ret);
        break;
    case NM_PRECRQ_QB_PH:
        check_dsp(ctx);
        gen_helper_precrq_qb_ph(v1_t, v1_t, v2_t);
        gen_store_gpr(v1_t, ret);
        break;
    case NM_PRECRQ_PH_W:
        check_dsp(ctx);
        gen_helper_precrq_ph_w(v1_t, v1_t, v2_t);
        gen_store_gpr(v1_t, ret);
        break;
    case NM_PRECRQ_RS_PH_W:
        check_dsp(ctx);
        gen_helper_precrq_rs_ph_w(v1_t, v1_t, v2_t, cpu_env);
        gen_store_gpr(v1_t, ret);
        break;
    case NM_PRECRQU_S_QB_PH:
        check_dsp(ctx);
        gen_helper_precrqu_s_qb_ph(v1_t, v1_t, v2_t, cpu_env);
        gen_store_gpr(v1_t, ret);
        break;
    case NM_SHRA_R_W:
        check_dsp(ctx);
        tcg_gen_movi_tl(t0, rd);
        gen_helper_shra_r_w(v1_t, t0, v1_t);
        gen_store_gpr(v1_t, rt);
        break;
    case NM_SHRA_R_PH:
        check_dsp(ctx);
        tcg_gen_movi_tl(t0, rd >> 1);
        switch (extract32(ctx->opcode, 10, 1)) {
        case 0:
            /* SHRA_PH */
            gen_helper_shra_ph(v1_t, t0, v1_t);
            gen_store_gpr(v1_t, rt);
            break;
        case 1:
            /* SHRA_R_PH */
            gen_helper_shra_r_ph(v1_t, t0, v1_t);
            gen_store_gpr(v1_t, rt);
            break;
        }
        break;
    case NM_SHLL_S_PH:
        check_dsp(ctx);
        tcg_gen_movi_tl(t0, rd >> 1);
        switch (extract32(ctx->opcode, 10, 2)) {
        case 0:
            /* SHLL_PH */
            gen_helper_shll_ph(v1_t, t0, v1_t, cpu_env);
            gen_store_gpr(v1_t, rt);
            break;
        case 2:
            /* SHLL_S_PH */
            gen_helper_shll_s_ph(v1_t, t0, v1_t, cpu_env);
            gen_store_gpr(v1_t, rt);
            break;
        default:
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case NM_SHLL_S_W:
        check_dsp(ctx);
        tcg_gen_movi_tl(t0, rd);
        gen_helper_shll_s_w(v1_t, t0, v1_t, cpu_env);
        gen_store_gpr(v1_t, rt);
        break;
    case NM_REPL_PH:
        check_dsp(ctx);
        {
            int16_t imm;
            imm = sextract32(ctx->opcode, 11, 11);
            imm = (int16_t)(imm << 6) >> 6;
            if (rt != 0) {
                tcg_gen_movi_tl(cpu_gpr[rt], dup_const(MO_16, imm));
            }
        }
        break;
    default:
        gen_reserved_instruction(ctx);
        break;
    }
}

static int decode_nanomips_32_48_opc(CPUMIPSState *env, DisasContext *ctx)
{
    uint16_t insn;
    uint32_t op;
    int rt, rs, rd;
    int offset;
    int imm;

    insn = translator_lduw(env, ctx->base.pc_next + 2);
    ctx->opcode = (ctx->opcode << 16) | insn;

    rt = extract32(ctx->opcode, 21, 5);
    rs = extract32(ctx->opcode, 16, 5);
    rd = extract32(ctx->opcode, 11, 5);

    op = extract32(ctx->opcode, 26, 6);
    switch (op) {
    case NM_P_ADDIU:
        if (rt == 0) {
            /* P.RI */
            switch (extract32(ctx->opcode, 19, 2)) {
            case NM_SIGRIE:
            default:
                gen_reserved_instruction(ctx);
                break;
            case NM_P_SYSCALL:
                if ((extract32(ctx->opcode, 18, 1)) == NM_SYSCALL) {
                    generate_exception_end(ctx, EXCP_SYSCALL);
                } else {
                    gen_reserved_instruction(ctx);
                }
                break;
            case NM_BREAK:
                generate_exception_end(ctx, EXCP_BREAK);
                break;
            case NM_SDBBP:
                if (is_uhi(extract32(ctx->opcode, 0, 19))) {
                    gen_helper_do_semihosting(cpu_env);
                } else {
                    if (ctx->hflags & MIPS_HFLAG_SBRI) {
                        gen_reserved_instruction(ctx);
                    } else {
                        generate_exception_end(ctx, EXCP_DBp);
                    }
                }
                break;
            }
        } else {
            /* NM_ADDIU */
            imm = extract32(ctx->opcode, 0, 16);
            if (rs != 0) {
                tcg_gen_addi_tl(cpu_gpr[rt], cpu_gpr[rs], imm);
            } else {
                tcg_gen_movi_tl(cpu_gpr[rt], imm);
            }
            tcg_gen_ext32s_tl(cpu_gpr[rt], cpu_gpr[rt]);
        }
        break;
    case NM_ADDIUPC:
        if (rt != 0) {
            offset = sextract32(ctx->opcode, 0, 1) << 21 |
                     extract32(ctx->opcode, 1, 20) << 1;
            target_long addr = addr_add(ctx, ctx->base.pc_next + 4, offset);
            tcg_gen_movi_tl(cpu_gpr[rt], addr);
        }
        break;
    case NM_POOL32A:
        switch (ctx->opcode & 0x07) {
        case NM_POOL32A0:
            gen_pool32a0_nanomips_insn(env, ctx);
            break;
        case NM_POOL32A5:
            {
                int32_t op1 = extract32(ctx->opcode, 3, 7);
                gen_pool32a5_nanomips_insn(ctx, op1, rd, rs, rt);
            }
            break;
        case NM_POOL32A7:
            switch (extract32(ctx->opcode, 3, 3)) {
            case NM_P_LSX:
                gen_p_lsx(ctx, rd, rs, rt);
                break;
            case NM_LSA:
                /*
                 * In nanoMIPS, the shift field directly encodes the shift
                 * amount, meaning that the supported shift values are in
                 * the range 0 to 3 (instead of 1 to 4 in MIPSR6).
                 */
                gen_lsa(ctx, rd, rt, rs, extract32(ctx->opcode, 9, 2) - 1);
                break;
            case NM_EXTW:
                gen_ext(ctx, 32, rd, rs, rt, extract32(ctx->opcode, 6, 5));
                break;
            case NM_POOL32AXF:
                gen_pool32axf_nanomips_insn(env, ctx);
                break;
            default:
                gen_reserved_instruction(ctx);
                break;
            }
            break;
        default:
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case NM_P_GP_W:
        switch (ctx->opcode & 0x03) {
        case NM_ADDIUGP_W:
            if (rt != 0) {
                offset = extract32(ctx->opcode, 0, 21);
                gen_op_addr_addi(ctx, cpu_gpr[rt], cpu_gpr[28], offset);
            }
            break;
        case NM_LWGP:
            gen_ld(ctx, OPC_LW, rt, 28, extract32(ctx->opcode, 2, 19) << 2);
            break;
        case NM_SWGP:
            gen_st(ctx, OPC_SW, rt, 28, extract32(ctx->opcode, 2, 19) << 2);
            break;
        default:
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case NM_P48I:
        {
            insn = translator_lduw(env, ctx->base.pc_next + 4);
            target_long addr_off = extract32(ctx->opcode, 0, 16) | insn << 16;
            switch (extract32(ctx->opcode, 16, 5)) {
            case NM_LI48:
                check_nms(ctx);
                if (rt != 0) {
                    tcg_gen_movi_tl(cpu_gpr[rt], addr_off);
                }
                break;
            case NM_ADDIU48:
                check_nms(ctx);
                if (rt != 0) {
                    tcg_gen_addi_tl(cpu_gpr[rt], cpu_gpr[rt], addr_off);
                    tcg_gen_ext32s_tl(cpu_gpr[rt], cpu_gpr[rt]);
                }
                break;
            case NM_ADDIUGP48:
                check_nms(ctx);
                if (rt != 0) {
                    gen_op_addr_addi(ctx, cpu_gpr[rt], cpu_gpr[28], addr_off);
                }
                break;
            case NM_ADDIUPC48:
                check_nms(ctx);
                if (rt != 0) {
                    target_long addr = addr_add(ctx, ctx->base.pc_next + 6,
                                                addr_off);

                    tcg_gen_movi_tl(cpu_gpr[rt], addr);
                }
                break;
            case NM_LWPC48:
                check_nms(ctx);
                if (rt != 0) {
                    TCGv t0;
                    t0 = tcg_temp_new();

                    target_long addr = addr_add(ctx, ctx->base.pc_next + 6,
                                                addr_off);

                    tcg_gen_movi_tl(t0, addr);
                    tcg_gen_qemu_ld_tl(cpu_gpr[rt], t0, ctx->mem_idx, MO_TESL);
                    tcg_temp_free(t0);
                }
                break;
            case NM_SWPC48:
                check_nms(ctx);
                {
                    TCGv t0, t1;
                    t0 = tcg_temp_new();
                    t1 = tcg_temp_new();

                    target_long addr = addr_add(ctx, ctx->base.pc_next + 6,
                                                addr_off);

                    tcg_gen_movi_tl(t0, addr);
                    gen_load_gpr(t1, rt);

                    tcg_gen_qemu_st_tl(t1, t0, ctx->mem_idx, MO_TEUL);

                    tcg_temp_free(t0);
                    tcg_temp_free(t1);
                }
                break;
            default:
                gen_reserved_instruction(ctx);
                break;
            }
            return 6;
        }
    case NM_P_U12:
        switch (extract32(ctx->opcode, 12, 4)) {
        case NM_ORI:
            gen_logic_imm(ctx, OPC_ORI, rt, rs, extract32(ctx->opcode, 0, 12));
            break;
        case NM_XORI:
            gen_logic_imm(ctx, OPC_XORI, rt, rs, extract32(ctx->opcode, 0, 12));
            break;
        case NM_ANDI:
            gen_logic_imm(ctx, OPC_ANDI, rt, rs, extract32(ctx->opcode, 0, 12));
            break;
        case NM_P_SR:
            switch (extract32(ctx->opcode, 20, 1)) {
            case NM_PP_SR:
                switch (ctx->opcode & 3) {
                case NM_SAVE:
                    gen_save(ctx, rt, extract32(ctx->opcode, 16, 4),
                             extract32(ctx->opcode, 2, 1),
                             extract32(ctx->opcode, 3, 9) << 3);
                    break;
                case NM_RESTORE:
                case NM_RESTORE_JRC:
                    gen_restore(ctx, rt, extract32(ctx->opcode, 16, 4),
                                extract32(ctx->opcode, 2, 1),
                                extract32(ctx->opcode, 3, 9) << 3);
                    if ((ctx->opcode & 3) == NM_RESTORE_JRC) {
                        gen_compute_branch_nm(ctx, OPC_JR, 2, 31, 0, 0);
                    }
                    break;
                default:
                    gen_reserved_instruction(ctx);
                    break;
                }
                break;
            case NM_P_SR_F:
                gen_reserved_instruction(ctx);
                break;
            }
            break;
        case NM_SLTI:
            gen_slt_imm(ctx, OPC_SLTI, rt, rs, extract32(ctx->opcode, 0, 12));
            break;
        case NM_SLTIU:
            gen_slt_imm(ctx, OPC_SLTIU, rt, rs, extract32(ctx->opcode, 0, 12));
            break;
        case NM_SEQI:
            {
                TCGv t0 = tcg_temp_new();

                imm = extract32(ctx->opcode, 0, 12);
                gen_load_gpr(t0, rs);
                tcg_gen_setcondi_tl(TCG_COND_EQ, t0, t0, imm);
                gen_store_gpr(t0, rt);

                tcg_temp_free(t0);
            }
            break;
        case NM_ADDIUNEG:
            imm = (int16_t) extract32(ctx->opcode, 0, 12);
            gen_arith_imm(ctx, OPC_ADDIU, rt, rs, -imm);
            break;
        case NM_P_SHIFT:
            {
                int shift = extract32(ctx->opcode, 0, 5);
                switch (extract32(ctx->opcode, 5, 4)) {
                case NM_P_SLL:
                    if (rt == 0 && shift == 0) {
                        /* NOP */
                    } else if (rt == 0 && shift == 3) {
                        /* EHB - treat as NOP */
                    } else if (rt == 0 && shift == 5) {
                        /* PAUSE - treat as NOP */
                    } else if (rt == 0 && shift == 6) {
                        /* SYNC */
                        gen_sync(extract32(ctx->opcode, 16, 5));
                    } else {
                        /* SLL */
                        gen_shift_imm(ctx, OPC_SLL, rt, rs,
                                      extract32(ctx->opcode, 0, 5));
                    }
                    break;
                case NM_SRL:
                    gen_shift_imm(ctx, OPC_SRL, rt, rs,
                                  extract32(ctx->opcode, 0, 5));
                    break;
                case NM_SRA:
                    gen_shift_imm(ctx, OPC_SRA, rt, rs,
                                  extract32(ctx->opcode, 0, 5));
                    break;
                case NM_ROTR:
                    gen_shift_imm(ctx, OPC_ROTR, rt, rs,
                                  extract32(ctx->opcode, 0, 5));
                    break;
                }
            }
            break;
        case NM_P_ROTX:
            check_nms(ctx);
            if (rt != 0) {
                TCGv t0 = tcg_temp_new();
                TCGv_i32 shift = tcg_const_i32(extract32(ctx->opcode, 0, 5));
                TCGv_i32 shiftx = tcg_const_i32(extract32(ctx->opcode, 7, 4)
                                                << 1);
                TCGv_i32 stripe = tcg_const_i32(extract32(ctx->opcode, 6, 1));

                gen_load_gpr(t0, rs);
                gen_helper_rotx(cpu_gpr[rt], t0, shift, shiftx, stripe);
                tcg_temp_free(t0);

                tcg_temp_free_i32(shift);
                tcg_temp_free_i32(shiftx);
                tcg_temp_free_i32(stripe);
            }
            break;
        case NM_P_INS:
            switch (((ctx->opcode >> 10) & 2) |
                    (extract32(ctx->opcode, 5, 1))) {
            case NM_INS:
                check_nms(ctx);
                gen_bitops(ctx, OPC_INS, rt, rs, extract32(ctx->opcode, 0, 5),
                           extract32(ctx->opcode, 6, 5));
                break;
            default:
                gen_reserved_instruction(ctx);
                break;
            }
            break;
        case NM_P_EXT:
            switch (((ctx->opcode >> 10) & 2) |
                    (extract32(ctx->opcode, 5, 1))) {
            case NM_EXT:
                check_nms(ctx);
                gen_bitops(ctx, OPC_EXT, rt, rs, extract32(ctx->opcode, 0, 5),
                           extract32(ctx->opcode, 6, 5));
                break;
            default:
                gen_reserved_instruction(ctx);
                break;
            }
            break;
        default:
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case NM_POOL32F:
        gen_pool32f_nanomips_insn(ctx);
        break;
    case NM_POOL32S:
        break;
    case NM_P_LUI:
        switch (extract32(ctx->opcode, 1, 1)) {
        case NM_LUI:
            if (rt != 0) {
                tcg_gen_movi_tl(cpu_gpr[rt],
                                sextract32(ctx->opcode, 0, 1) << 31 |
                                extract32(ctx->opcode, 2, 10) << 21 |
                                extract32(ctx->opcode, 12, 9) << 12);
            }
            break;
        case NM_ALUIPC:
            if (rt != 0) {
                offset = sextract32(ctx->opcode, 0, 1) << 31 |
                         extract32(ctx->opcode, 2, 10) << 21 |
                         extract32(ctx->opcode, 12, 9) << 12;
                target_long addr;
                addr = ~0xFFF & addr_add(ctx, ctx->base.pc_next + 4, offset);
                tcg_gen_movi_tl(cpu_gpr[rt], addr);
            }
            break;
        }
        break;
    case NM_P_GP_BH:
        {
            uint32_t u = extract32(ctx->opcode, 0, 18);

            switch (extract32(ctx->opcode, 18, 3)) {
            case NM_LBGP:
                gen_ld(ctx, OPC_LB, rt, 28, u);
                break;
            case NM_SBGP:
                gen_st(ctx, OPC_SB, rt, 28, u);
                break;
            case NM_LBUGP:
                gen_ld(ctx, OPC_LBU, rt, 28, u);
                break;
            case NM_ADDIUGP_B:
                if (rt != 0) {
                    gen_op_addr_addi(ctx, cpu_gpr[rt], cpu_gpr[28], u);
                }
                break;
            case NM_P_GP_LH:
                u &= ~1;
                switch (ctx->opcode & 1) {
                case NM_LHGP:
                    gen_ld(ctx, OPC_LH, rt, 28, u);
                    break;
                case NM_LHUGP:
                    gen_ld(ctx, OPC_LHU, rt, 28, u);
                    break;
                }
                break;
            case NM_P_GP_SH:
                u &= ~1;
                switch (ctx->opcode & 1) {
                case NM_SHGP:
                    gen_st(ctx, OPC_SH, rt, 28, u);
                    break;
                default:
                    gen_reserved_instruction(ctx);
                    break;
                }
                break;
            case NM_P_GP_CP1:
                u &= ~0x3;
                switch (ctx->opcode & 0x3) {
                case NM_LWC1GP:
                    gen_cop1_ldst(ctx, OPC_LWC1, rt, 28, u);
                    break;
                case NM_LDC1GP:
                    gen_cop1_ldst(ctx, OPC_LDC1, rt, 28, u);
                    break;
                case NM_SWC1GP:
                    gen_cop1_ldst(ctx, OPC_SWC1, rt, 28, u);
                    break;
                case NM_SDC1GP:
                    gen_cop1_ldst(ctx, OPC_SDC1, rt, 28, u);
                    break;
                }
                break;
            default:
                gen_reserved_instruction(ctx);
                break;
            }
        }
        break;
    case NM_P_LS_U12:
        {
            uint32_t u = extract32(ctx->opcode, 0, 12);

            switch (extract32(ctx->opcode, 12, 4)) {
            case NM_P_PREFU12:
                if (rt == 31) {
                    /* SYNCI */
                    /*
                     * Break the TB to be able to sync copied instructions
                     * immediately.
                     */
                    ctx->base.is_jmp = DISAS_STOP;
                } else {
                    /* PREF */
                    /* Treat as NOP. */
                }
                break;
            case NM_LB:
                gen_ld(ctx, OPC_LB, rt, rs, u);
                break;
            case NM_LH:
                gen_ld(ctx, OPC_LH, rt, rs, u);
                break;
            case NM_LW:
                gen_ld(ctx, OPC_LW, rt, rs, u);
                break;
            case NM_LBU:
                gen_ld(ctx, OPC_LBU, rt, rs, u);
                break;
            case NM_LHU:
                gen_ld(ctx, OPC_LHU, rt, rs, u);
                break;
            case NM_SB:
                gen_st(ctx, OPC_SB, rt, rs, u);
                break;
            case NM_SH:
                gen_st(ctx, OPC_SH, rt, rs, u);
                break;
            case NM_SW:
                gen_st(ctx, OPC_SW, rt, rs, u);
                break;
            case NM_LWC1:
                gen_cop1_ldst(ctx, OPC_LWC1, rt, rs, u);
                break;
            case NM_LDC1:
                gen_cop1_ldst(ctx, OPC_LDC1, rt, rs, u);
                break;
            case NM_SWC1:
                gen_cop1_ldst(ctx, OPC_SWC1, rt, rs, u);
                break;
            case NM_SDC1:
                gen_cop1_ldst(ctx, OPC_SDC1, rt, rs, u);
                break;
            default:
                gen_reserved_instruction(ctx);
                break;
            }
        }
        break;
    case NM_P_LS_S9:
        {
            int32_t s = (sextract32(ctx->opcode, 15, 1) << 8) |
                        extract32(ctx->opcode, 0, 8);

            switch (extract32(ctx->opcode, 8, 3)) {
            case NM_P_LS_S0:
                switch (extract32(ctx->opcode, 11, 4)) {
                case NM_LBS9:
                    gen_ld(ctx, OPC_LB, rt, rs, s);
                    break;
                case NM_LHS9:
                    gen_ld(ctx, OPC_LH, rt, rs, s);
                    break;
                case NM_LWS9:
                    gen_ld(ctx, OPC_LW, rt, rs, s);
                    break;
                case NM_LBUS9:
                    gen_ld(ctx, OPC_LBU, rt, rs, s);
                    break;
                case NM_LHUS9:
                    gen_ld(ctx, OPC_LHU, rt, rs, s);
                    break;
                case NM_SBS9:
                    gen_st(ctx, OPC_SB, rt, rs, s);
                    break;
                case NM_SHS9:
                    gen_st(ctx, OPC_SH, rt, rs, s);
                    break;
                case NM_SWS9:
                    gen_st(ctx, OPC_SW, rt, rs, s);
                    break;
                case NM_LWC1S9:
                    gen_cop1_ldst(ctx, OPC_LWC1, rt, rs, s);
                    break;
                case NM_LDC1S9:
                    gen_cop1_ldst(ctx, OPC_LDC1, rt, rs, s);
                    break;
                case NM_SWC1S9:
                    gen_cop1_ldst(ctx, OPC_SWC1, rt, rs, s);
                    break;
                case NM_SDC1S9:
                    gen_cop1_ldst(ctx, OPC_SDC1, rt, rs, s);
                    break;
                case NM_P_PREFS9:
                    if (rt == 31) {
                        /* SYNCI */
                        /*
                         * Break the TB to be able to sync copied instructions
                         * immediately.
                         */
                        ctx->base.is_jmp = DISAS_STOP;
                    } else {
                        /* PREF */
                        /* Treat as NOP. */
                    }
                    break;
                default:
                    gen_reserved_instruction(ctx);
                    break;
                }
                break;
            case NM_P_LS_S1:
                switch (extract32(ctx->opcode, 11, 4)) {
                case NM_UALH:
                case NM_UASH:
                    check_nms(ctx);
                    {
                        TCGv t0 = tcg_temp_new();
                        TCGv t1 = tcg_temp_new();

                        gen_base_offset_addr(ctx, t0, rs, s);

                        switch (extract32(ctx->opcode, 11, 4)) {
                        case NM_UALH:
                            tcg_gen_qemu_ld_tl(t0, t0, ctx->mem_idx, MO_TESW |
                                               MO_UNALN);
                            gen_store_gpr(t0, rt);
                            break;
                        case NM_UASH:
                            gen_load_gpr(t1, rt);
                            tcg_gen_qemu_st_tl(t1, t0, ctx->mem_idx, MO_TEUW |
                                               MO_UNALN);
                            break;
                        }
                        tcg_temp_free(t0);
                        tcg_temp_free(t1);
                    }
                    break;
                case NM_P_LL:
                    switch (ctx->opcode & 0x03) {
                    case NM_LL:
                        gen_ld(ctx, OPC_LL, rt, rs, s);
                        break;
                    case NM_LLWP:
                        check_xnp(ctx);
                        gen_llwp(ctx, rs, 0, rt, extract32(ctx->opcode, 3, 5));
                        break;
                    }
                    break;
                case NM_P_SC:
                    switch (ctx->opcode & 0x03) {
                    case NM_SC:
                        gen_st_cond(ctx, rt, rs, s, MO_TESL, false, OPC_SC);
                        break;
                    case NM_SCWP:
                        check_xnp(ctx);
                        gen_scwp(ctx, rs, 0, rt, extract32(ctx->opcode, 3, 5),
                                 false);
                        break;
                    }
                    break;
                case NM_CACHE:
                    check_cp0_enabled(ctx);
                    if (ctx->hflags & MIPS_HFLAG_ITC_CACHE) {
                        gen_cache_operation(ctx, rt, rs, s);
                    }
                    break;
                }
                break;
            case NM_P_LS_E0:
                switch (extract32(ctx->opcode, 11, 4)) {
                case NM_LBE:
                    check_eva(ctx);
                    check_cp0_enabled(ctx);
                    gen_ld(ctx, OPC_LBE, rt, rs, s);
                    break;
                case NM_SBE:
                    check_eva(ctx);
                    check_cp0_enabled(ctx);
                    gen_st(ctx, OPC_SBE, rt, rs, s);
                    break;
                case NM_LBUE:
                    check_eva(ctx);
                    check_cp0_enabled(ctx);
                    gen_ld(ctx, OPC_LBUE, rt, rs, s);
                    break;
                case NM_P_PREFE:
                    if (rt == 31) {
                        /* case NM_SYNCIE */
                        check_eva(ctx);
                        check_cp0_enabled(ctx);
                        /*
                         * Break the TB to be able to sync copied instructions
                         * immediately.
                         */
                        ctx->base.is_jmp = DISAS_STOP;
                    } else {
                        /* case NM_PREFE */
                        check_eva(ctx);
                        check_cp0_enabled(ctx);
                        /* Treat as NOP. */
                    }
                    break;
                case NM_LHE:
                    check_eva(ctx);
                    check_cp0_enabled(ctx);
                    gen_ld(ctx, OPC_LHE, rt, rs, s);
                    break;
                case NM_SHE:
                    check_eva(ctx);
                    check_cp0_enabled(ctx);
                    gen_st(ctx, OPC_SHE, rt, rs, s);
                    break;
                case NM_LHUE:
                    check_eva(ctx);
                    check_cp0_enabled(ctx);
                    gen_ld(ctx, OPC_LHUE, rt, rs, s);
                    break;
                case NM_CACHEE:
                    check_nms_dl_il_sl_tl_l2c(ctx);
                    gen_cache_operation(ctx, rt, rs, s);
                    break;
                case NM_LWE:
                    check_eva(ctx);
                    check_cp0_enabled(ctx);
                    gen_ld(ctx, OPC_LWE, rt, rs, s);
                    break;
                case NM_SWE:
                    check_eva(ctx);
                    check_cp0_enabled(ctx);
                    gen_st(ctx, OPC_SWE, rt, rs, s);
                    break;
                case NM_P_LLE:
                    switch (extract32(ctx->opcode, 2, 2)) {
                    case NM_LLE:
                        check_xnp(ctx);
                        check_eva(ctx);
                        check_cp0_enabled(ctx);
                        gen_ld(ctx, OPC_LLE, rt, rs, s);
                        break;
                    case NM_LLWPE:
                        check_xnp(ctx);
                        check_eva(ctx);
                        check_cp0_enabled(ctx);
                        gen_llwp(ctx, rs, 0, rt, extract32(ctx->opcode, 3, 5));
                        break;
                    default:
                        gen_reserved_instruction(ctx);
                        break;
                    }
                    break;
                case NM_P_SCE:
                    switch (extract32(ctx->opcode, 2, 2)) {
                    case NM_SCE:
                        check_xnp(ctx);
                        check_eva(ctx);
                        check_cp0_enabled(ctx);
                        gen_st_cond(ctx, rt, rs, s, MO_TESL, true, OPC_SCE);
                        break;
                    case NM_SCWPE:
                        check_xnp(ctx);
                        check_eva(ctx);
                        check_cp0_enabled(ctx);
                        gen_scwp(ctx, rs, 0, rt, extract32(ctx->opcode, 3, 5),
                                 true);
                        break;
                    default:
                        gen_reserved_instruction(ctx);
                        break;
                    }
                    break;
                }
                break;
            case NM_P_LS_WM:
            case NM_P_LS_UAWM:
                check_nms(ctx);
                {
                    int count = extract32(ctx->opcode, 12, 3);
                    int counter = 0;

                    offset = sextract32(ctx->opcode, 15, 1) << 8 |
                             extract32(ctx->opcode, 0, 8);
                    TCGv va = tcg_temp_new();
                    TCGv t1 = tcg_temp_new();
                    MemOp memop = (extract32(ctx->opcode, 8, 3)) ==
                                      NM_P_LS_UAWM ? MO_UNALN : 0;

                    count = (count == 0) ? 8 : count;
                    while (counter != count) {
                        int this_rt = ((rt + counter) & 0x1f) | (rt & 0x10);
                        int this_offset = offset + (counter << 2);

                        gen_base_offset_addr(ctx, va, rs, this_offset);

                        switch (extract32(ctx->opcode, 11, 1)) {
                        case NM_LWM:
                            tcg_gen_qemu_ld_tl(t1, va, ctx->mem_idx,
                                               memop | MO_TESL);
                            gen_store_gpr(t1, this_rt);
                            if ((this_rt == rs) &&
                                (counter != (count - 1))) {
                                /* UNPREDICTABLE */
                            }
                            break;
                        case NM_SWM:
                            this_rt = (rt == 0) ? 0 : this_rt;
                            gen_load_gpr(t1, this_rt);
                            tcg_gen_qemu_st_tl(t1, va, ctx->mem_idx,
                                               memop | MO_TEUL);
                            break;
                        }
                        counter++;
                    }
                    tcg_temp_free(va);
                    tcg_temp_free(t1);
                }
                break;
            default:
                gen_reserved_instruction(ctx);
                break;
            }
        }
        break;
    case NM_MOVE_BALC:
        check_nms(ctx);
        {
            TCGv t0 = tcg_temp_new();
            int32_t s = sextract32(ctx->opcode, 0, 1) << 21 |
                        extract32(ctx->opcode, 1, 20) << 1;
            rd = (extract32(ctx->opcode, 24, 1)) == 0 ? 4 : 5;
            rt = decode_gpr_gpr4_zero(extract32(ctx->opcode, 25, 1) << 3 |
                            extract32(ctx->opcode, 21, 3));
            gen_load_gpr(t0, rt);
            tcg_gen_mov_tl(cpu_gpr[rd], t0);
            gen_compute_branch_nm(ctx, OPC_BGEZAL, 4, 0, 0, s);
            tcg_temp_free(t0);
        }
        break;
    case NM_P_BAL:
        {
            int32_t s = sextract32(ctx->opcode, 0, 1) << 25 |
                        extract32(ctx->opcode, 1, 24) << 1;

            if ((extract32(ctx->opcode, 25, 1)) == 0) {
                /* BC */
                gen_compute_branch_nm(ctx, OPC_BEQ, 4, 0, 0, s);
            } else {
                /* BALC */
                gen_compute_branch_nm(ctx, OPC_BGEZAL, 4, 0, 0, s);
            }
        }
        break;
    case NM_P_J:
        switch (extract32(ctx->opcode, 12, 4)) {
        case NM_JALRC:
        case NM_JALRC_HB:
            gen_compute_branch_nm(ctx, OPC_JALR, 4, rs, rt, 0);
            break;
        case NM_P_BALRSC:
            gen_compute_nanomips_pbalrsc_branch(ctx, rs, rt);
            break;
        default:
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case NM_P_BR1:
        {
            int32_t s = sextract32(ctx->opcode, 0, 1) << 14 |
                        extract32(ctx->opcode, 1, 13) << 1;
            switch (extract32(ctx->opcode, 14, 2)) {
            case NM_BEQC:
                check_nms(ctx);
                gen_compute_branch_nm(ctx, OPC_BEQ, 4, rs, rt, s);
                break;
            case NM_P_BR3A:
                s = sextract32(ctx->opcode, 0, 1) << 14 |
                    extract32(ctx->opcode, 1, 13) << 1;
                check_cp1_enabled(ctx);
                switch (extract32(ctx->opcode, 16, 5)) {
                case NM_BC1EQZC:
                    gen_compute_branch_cp1_nm(ctx, OPC_BC1EQZ, rt, s);
                    break;
                case NM_BC1NEZC:
                    gen_compute_branch_cp1_nm(ctx, OPC_BC1NEZ, rt, s);
                    break;
                case NM_BPOSGE32C:
                    check_dsp_r3(ctx);
                    {
                        int32_t imm = extract32(ctx->opcode, 1, 13) |
                                      extract32(ctx->opcode, 0, 1) << 13;

                        gen_compute_branch_nm(ctx, OPC_BPOSGE32, 4, -1, -2,
                                              imm);
                    }
                    break;
                default:
                    gen_reserved_instruction(ctx);
                    break;
                }
                break;
            case NM_BGEC:
                if (rs == rt) {
                    gen_compute_compact_branch_nm(ctx, OPC_BC, rs, rt, s);
                } else {
                    gen_compute_compact_branch_nm(ctx, OPC_BGEC, rs, rt, s);
                }
                break;
            case NM_BGEUC:
                if (rs == rt || rt == 0) {
                    gen_compute_compact_branch_nm(ctx, OPC_BC, 0, 0, s);
                } else if (rs == 0) {
                    gen_compute_compact_branch_nm(ctx, OPC_BEQZC, rt, 0, s);
                } else {
                    gen_compute_compact_branch_nm(ctx, OPC_BGEUC, rs, rt, s);
                }
                break;
            }
        }
        break;
    case NM_P_BR2:
        {
            int32_t s = sextract32(ctx->opcode, 0, 1) << 14 |
                        extract32(ctx->opcode, 1, 13) << 1;
            switch (extract32(ctx->opcode, 14, 2)) {
            case NM_BNEC:
                check_nms(ctx);
                gen_compute_branch_nm(ctx, OPC_BNE, 4, rs, rt, s);
                break;
            case NM_BLTC:
                if (rs != 0 && rt != 0 && rs == rt) {
                    /* NOP */
                    ctx->hflags |= MIPS_HFLAG_FBNSLOT;
                } else {
                    gen_compute_compact_branch_nm(ctx, OPC_BLTC, rs, rt, s);
                }
                break;
            case NM_BLTUC:
                if (rs == 0 || rs == rt) {
                    /* NOP */
                    ctx->hflags |= MIPS_HFLAG_FBNSLOT;
                } else {
                    gen_compute_compact_branch_nm(ctx, OPC_BLTUC, rs, rt, s);
                }
                break;
            default:
                gen_reserved_instruction(ctx);
                break;
            }
        }
        break;
    case NM_P_BRI:
        {
            int32_t s = sextract32(ctx->opcode, 0, 1) << 11 |
                        extract32(ctx->opcode, 1, 10) << 1;
            uint32_t u = extract32(ctx->opcode, 11, 7);

            gen_compute_imm_branch(ctx, extract32(ctx->opcode, 18, 3),
                                   rt, u, s);
        }
        break;
    default:
        gen_reserved_instruction(ctx);
        break;
    }
    return 4;
}

static int decode_nanomips_opc(CPUMIPSState *env, DisasContext *ctx)
{
    uint32_t op;
    int rt = decode_gpr_gpr3(NANOMIPS_EXTRACT_RT3(ctx->opcode));
    int rs = decode_gpr_gpr3(NANOMIPS_EXTRACT_RS3(ctx->opcode));
    int rd = decode_gpr_gpr3(NANOMIPS_EXTRACT_RD3(ctx->opcode));
    int offset;
    int imm;

    /* make sure instructions are on a halfword boundary */
    if (ctx->base.pc_next & 0x1) {
        TCGv tmp = tcg_const_tl(ctx->base.pc_next);
        tcg_gen_st_tl(tmp, cpu_env, offsetof(CPUMIPSState, CP0_BadVAddr));
        tcg_temp_free(tmp);
        generate_exception_end(ctx, EXCP_AdEL);
        return 2;
    }

    op = extract32(ctx->opcode, 10, 6);
    switch (op) {
    case NM_P16_MV:
        rt = NANOMIPS_EXTRACT_RD5(ctx->opcode);
        if (rt != 0) {
            /* MOVE */
            rs = NANOMIPS_EXTRACT_RS5(ctx->opcode);
            gen_arith(ctx, OPC_ADDU, rt, rs, 0);
        } else {
            /* P16.RI */
            switch (extract32(ctx->opcode, 3, 2)) {
            case NM_P16_SYSCALL:
                if (extract32(ctx->opcode, 2, 1) == 0) {
                    generate_exception_end(ctx, EXCP_SYSCALL);
                } else {
                    gen_reserved_instruction(ctx);
                }
                break;
            case NM_BREAK16:
                generate_exception_end(ctx, EXCP_BREAK);
                break;
            case NM_SDBBP16:
                if (is_uhi(extract32(ctx->opcode, 0, 3))) {
                    gen_helper_do_semihosting(cpu_env);
                } else {
                    if (ctx->hflags & MIPS_HFLAG_SBRI) {
                        gen_reserved_instruction(ctx);
                    } else {
                        generate_exception_end(ctx, EXCP_DBp);
                    }
                }
                break;
            default:
                gen_reserved_instruction(ctx);
                break;
            }
        }
        break;
    case NM_P16_SHIFT:
        {
            int shift = extract32(ctx->opcode, 0, 3);
            uint32_t opc = 0;
            shift = (shift == 0) ? 8 : shift;

            switch (extract32(ctx->opcode, 3, 1)) {
            case NM_SLL16:
                opc = OPC_SLL;
                break;
            case NM_SRL16:
                opc = OPC_SRL;
                break;
            }
            gen_shift_imm(ctx, opc, rt, rs, shift);
        }
        break;
    case NM_P16C:
        switch (ctx->opcode & 1) {
        case NM_POOL16C_0:
            gen_pool16c_nanomips_insn(ctx);
            break;
        case NM_LWXS16:
            gen_ldxs(ctx, rt, rs, rd, op);
            break;
        }
        break;
    case NM_P16_A1:
        switch (extract32(ctx->opcode, 6, 1)) {
        case NM_ADDIUR1SP:
            imm = extract32(ctx->opcode, 0, 6) << 2;
            gen_arith_imm(ctx, OPC_ADDIU, rt, 29, imm);
            break;
        default:
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case NM_P16_A2:
        switch (extract32(ctx->opcode, 3, 1)) {
        case NM_ADDIUR2:
            imm = extract32(ctx->opcode, 0, 3) << 2;
            gen_arith_imm(ctx, OPC_ADDIU, rt, rs, imm);
            break;
        case NM_P_ADDIURS5:
            rt = extract32(ctx->opcode, 5, 5);
            if (rt != 0) {
                /* imm = sign_extend(s[3] . s[2:0] , from_nbits = 4) */
                imm = (sextract32(ctx->opcode, 4, 1) << 3) |
                      (extract32(ctx->opcode, 0, 3));
                gen_arith_imm(ctx, OPC_ADDIU, rt, rt, imm);
            }
            break;
        }
        break;
    case NM_P16_ADDU:
        switch (ctx->opcode & 0x1) {
        case NM_ADDU16:
            gen_arith(ctx, OPC_ADDU, rd, rs, rt);
            break;
        case NM_SUBU16:
            gen_arith(ctx, OPC_SUBU, rd, rs, rt);
            break;
        }
        break;
    case NM_P16_4X4:
        rt = (extract32(ctx->opcode, 9, 1) << 3) |
              extract32(ctx->opcode, 5, 3);
        rs = (extract32(ctx->opcode, 4, 1) << 3) |
              extract32(ctx->opcode, 0, 3);
        rt = decode_gpr_gpr4(rt);
        rs = decode_gpr_gpr4(rs);
        switch ((extract32(ctx->opcode, 7, 2) & 0x2) |
                (extract32(ctx->opcode, 3, 1))) {
        case NM_ADDU4X4:
            check_nms(ctx);
            gen_arith(ctx, OPC_ADDU, rt, rs, rt);
            break;
        case NM_MUL4X4:
            check_nms(ctx);
            gen_r6_muldiv(ctx, R6_OPC_MUL, rt, rs, rt);
            break;
        default:
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case NM_LI16:
        {
            int imm = extract32(ctx->opcode, 0, 7);
            imm = (imm == 0x7f ? -1 : imm);
            if (rt != 0) {
                tcg_gen_movi_tl(cpu_gpr[rt], imm);
            }
        }
        break;
    case NM_ANDI16:
        {
            uint32_t u = extract32(ctx->opcode, 0, 4);
            u = (u == 12) ? 0xff :
                (u == 13) ? 0xffff : u;
            gen_logic_imm(ctx, OPC_ANDI, rt, rs, u);
        }
        break;
    case NM_P16_LB:
        offset = extract32(ctx->opcode, 0, 2);
        switch (extract32(ctx->opcode, 2, 2)) {
        case NM_LB16:
            gen_ld(ctx, OPC_LB, rt, rs, offset);
            break;
        case NM_SB16:
            rt = decode_gpr_gpr3_src_store(
                     NANOMIPS_EXTRACT_RT3(ctx->opcode));
            gen_st(ctx, OPC_SB, rt, rs, offset);
            break;
        case NM_LBU16:
            gen_ld(ctx, OPC_LBU, rt, rs, offset);
            break;
        default:
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case NM_P16_LH:
        offset = extract32(ctx->opcode, 1, 2) << 1;
        switch ((extract32(ctx->opcode, 3, 1) << 1) | (ctx->opcode & 1)) {
        case NM_LH16:
            gen_ld(ctx, OPC_LH, rt, rs, offset);
            break;
        case NM_SH16:
            rt = decode_gpr_gpr3_src_store(
                     NANOMIPS_EXTRACT_RT3(ctx->opcode));
            gen_st(ctx, OPC_SH, rt, rs, offset);
            break;
        case NM_LHU16:
            gen_ld(ctx, OPC_LHU, rt, rs, offset);
            break;
        default:
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case NM_LW16:
        offset = extract32(ctx->opcode, 0, 4) << 2;
        gen_ld(ctx, OPC_LW, rt, rs, offset);
        break;
    case NM_LWSP16:
        rt = NANOMIPS_EXTRACT_RD5(ctx->opcode);
        offset = extract32(ctx->opcode, 0, 5) << 2;
        gen_ld(ctx, OPC_LW, rt, 29, offset);
        break;
    case NM_LW4X4:
        check_nms(ctx);
        rt = (extract32(ctx->opcode, 9, 1) << 3) |
             extract32(ctx->opcode, 5, 3);
        rs = (extract32(ctx->opcode, 4, 1) << 3) |
             extract32(ctx->opcode, 0, 3);
        offset = (extract32(ctx->opcode, 3, 1) << 3) |
                 (extract32(ctx->opcode, 8, 1) << 2);
        rt = decode_gpr_gpr4(rt);
        rs = decode_gpr_gpr4(rs);
        gen_ld(ctx, OPC_LW, rt, rs, offset);
        break;
    case NM_SW4X4:
        check_nms(ctx);
        rt = (extract32(ctx->opcode, 9, 1) << 3) |
             extract32(ctx->opcode, 5, 3);
        rs = (extract32(ctx->opcode, 4, 1) << 3) |
             extract32(ctx->opcode, 0, 3);
        offset = (extract32(ctx->opcode, 3, 1) << 3) |
                 (extract32(ctx->opcode, 8, 1) << 2);
        rt = decode_gpr_gpr4_zero(rt);
        rs = decode_gpr_gpr4(rs);
        gen_st(ctx, OPC_SW, rt, rs, offset);
        break;
    case NM_LWGP16:
        offset = extract32(ctx->opcode, 0, 7) << 2;
        gen_ld(ctx, OPC_LW, rt, 28, offset);
        break;
    case NM_SWSP16:
        rt = NANOMIPS_EXTRACT_RD5(ctx->opcode);
        offset = extract32(ctx->opcode, 0, 5) << 2;
        gen_st(ctx, OPC_SW, rt, 29, offset);
        break;
    case NM_SW16:
        rt = decode_gpr_gpr3_src_store(
                 NANOMIPS_EXTRACT_RT3(ctx->opcode));
        rs = decode_gpr_gpr3(NANOMIPS_EXTRACT_RS3(ctx->opcode));
        offset = extract32(ctx->opcode, 0, 4) << 2;
        gen_st(ctx, OPC_SW, rt, rs, offset);
        break;
    case NM_SWGP16:
        rt = decode_gpr_gpr3_src_store(
                 NANOMIPS_EXTRACT_RT3(ctx->opcode));
        offset = extract32(ctx->opcode, 0, 7) << 2;
        gen_st(ctx, OPC_SW, rt, 28, offset);
        break;
    case NM_BC16:
        gen_compute_branch_nm(ctx, OPC_BEQ, 2, 0, 0,
                           (sextract32(ctx->opcode, 0, 1) << 10) |
                           (extract32(ctx->opcode, 1, 9) << 1));
        break;
    case NM_BALC16:
        gen_compute_branch_nm(ctx, OPC_BGEZAL, 2, 0, 0,
                           (sextract32(ctx->opcode, 0, 1) << 10) |
                           (extract32(ctx->opcode, 1, 9) << 1));
        break;
    case NM_BEQZC16:
        gen_compute_branch_nm(ctx, OPC_BEQ, 2, rt, 0,
                           (sextract32(ctx->opcode, 0, 1) << 7) |
                           (extract32(ctx->opcode, 1, 6) << 1));
        break;
    case NM_BNEZC16:
        gen_compute_branch_nm(ctx, OPC_BNE, 2, rt, 0,
                           (sextract32(ctx->opcode, 0, 1) << 7) |
                           (extract32(ctx->opcode, 1, 6) << 1));
        break;
    case NM_P16_BR:
        switch (ctx->opcode & 0xf) {
        case 0:
            /* P16.JRC */
            switch (extract32(ctx->opcode, 4, 1)) {
            case NM_JRC:
                gen_compute_branch_nm(ctx, OPC_JR, 2,
                                   extract32(ctx->opcode, 5, 5), 0, 0);
                break;
            case NM_JALRC16:
                gen_compute_branch_nm(ctx, OPC_JALR, 2,
                                   extract32(ctx->opcode, 5, 5), 31, 0);
                break;
            }
            break;
        default:
            {
                /* P16.BRI */
                uint32_t opc = extract32(ctx->opcode, 4, 3) <
                               extract32(ctx->opcode, 7, 3) ? OPC_BEQ : OPC_BNE;
                gen_compute_branch_nm(ctx, opc, 2, rs, rt,
                                   extract32(ctx->opcode, 0, 4) << 1);
            }
            break;
        }
        break;
    case NM_P16_SR:
        {
            int count = extract32(ctx->opcode, 0, 4);
            int u = extract32(ctx->opcode, 4, 4) << 4;

            rt = 30 + extract32(ctx->opcode, 9, 1);
            switch (extract32(ctx->opcode, 8, 1)) {
            case NM_SAVE16:
                gen_save(ctx, rt, count, 0, u);
                break;
            case NM_RESTORE_JRC16:
                gen_restore(ctx, rt, count, 0, u);
                gen_compute_branch_nm(ctx, OPC_JR, 2, 31, 0, 0);
                break;
            }
        }
        break;
    case NM_MOVEP:
    case NM_MOVEPREV:
        check_nms(ctx);
        {
            static const int gpr2reg1[] = {4, 5, 6, 7};
            static const int gpr2reg2[] = {5, 6, 7, 8};
            int re;
            int rd2 = extract32(ctx->opcode, 3, 1) << 1 |
                      extract32(ctx->opcode, 8, 1);
            int r1 = gpr2reg1[rd2];
            int r2 = gpr2reg2[rd2];
            int r3 = extract32(ctx->opcode, 4, 1) << 3 |
                     extract32(ctx->opcode, 0, 3);
            int r4 = extract32(ctx->opcode, 9, 1) << 3 |
                     extract32(ctx->opcode, 5, 3);
            TCGv t0 = tcg_temp_new();
            TCGv t1 = tcg_temp_new();
            if (op == NM_MOVEP) {
                rd = r1;
                re = r2;
                rs = decode_gpr_gpr4_zero(r3);
                rt = decode_gpr_gpr4_zero(r4);
            } else {
                rd = decode_gpr_gpr4(r3);
                re = decode_gpr_gpr4(r4);
                rs = r1;
                rt = r2;
            }
            gen_load_gpr(t0, rs);
            gen_load_gpr(t1, rt);
            tcg_gen_mov_tl(cpu_gpr[rd], t0);
            tcg_gen_mov_tl(cpu_gpr[re], t1);
            tcg_temp_free(t0);
            tcg_temp_free(t1);
        }
        break;
    default:
        return decode_nanomips_32_48_opc(env, ctx);
    }

    return 2;
}


#endif // !TARGET_CHERI

/* SmartMIPS extension to MIPS32 */

#if defined(TARGET_MIPS64)

/* MDMX extension to MIPS64 */

#endif

/* MIPSDSP functions. */
static void gen_mipsdsp_ld(DisasContext *ctx, uint32_t opc,
                           int rd, int base, int offset)
{
    TCGv t0;

    check_dsp(ctx);
    t0 = tcg_temp_new();

    if (base == 0) {
        gen_load_gpr(t0, offset);
    } else if (offset == 0) {
        gen_load_gpr(t0, base);
    } else {
        gen_op_addr_add(ctx, t0, cpu_gpr[base], cpu_gpr[offset]);
    }

    switch (opc) {
    case OPC_LBUX:
        gen_ddc_interposed_ld_tl(ctx, t0, NULL/* add $ddc to t0*/, t0, ctx->mem_idx, MO_UB);
        gen_store_gpr(t0, rd);
        break;
    case OPC_LHX:
        gen_ddc_interposed_ld_tl(ctx, t0, NULL/* add $ddc to t0*/, t0, ctx->mem_idx, MO_TESW);
        gen_store_gpr(t0, rd);
        break;
    case OPC_LWX:
        gen_ddc_interposed_ld_tl(ctx, t0, NULL/* add $ddc to t0*/, t0, ctx->mem_idx, MO_TESL);
        gen_store_gpr(t0, rd);
        break;
#if defined(TARGET_MIPS64)
    case OPC_LDX:
        gen_ddc_interposed_ld_tl(ctx, t0, NULL/* add $ddc to t0*/, t0, ctx->mem_idx, MO_TEQ);
        gen_store_gpr(t0, rd);
        break;
#endif
    }
    tcg_temp_free(t0);
}

static void gen_mipsdsp_arith(DisasContext *ctx, uint32_t op1, uint32_t op2,
                              int ret, int v1, int v2)
{
    TCGv v1_t;
    TCGv v2_t;

    if (ret == 0) {
        /* Treat as NOP. */
        return;
    }

    v1_t = tcg_temp_new();
    v2_t = tcg_temp_new();

    gen_load_gpr(v1_t, v1);
    gen_load_gpr(v2_t, v2);

    switch (op1) {
    /* OPC_MULT_G_2E is equal OPC_ADDUH_QB_DSP */
    case OPC_MULT_G_2E:
        check_dsp_r2(ctx);
        switch (op2) {
        case OPC_ADDUH_QB:
            gen_helper_adduh_qb(cpu_gpr[ret], v1_t, v2_t);
            break;
        case OPC_ADDUH_R_QB:
            gen_helper_adduh_r_qb(cpu_gpr[ret], v1_t, v2_t);
            break;
        case OPC_ADDQH_PH:
            gen_helper_addqh_ph(cpu_gpr[ret], v1_t, v2_t);
            break;
        case OPC_ADDQH_R_PH:
            gen_helper_addqh_r_ph(cpu_gpr[ret], v1_t, v2_t);
            break;
        case OPC_ADDQH_W:
            gen_helper_addqh_w(cpu_gpr[ret], v1_t, v2_t);
            break;
        case OPC_ADDQH_R_W:
            gen_helper_addqh_r_w(cpu_gpr[ret], v1_t, v2_t);
            break;
        case OPC_SUBUH_QB:
            gen_helper_subuh_qb(cpu_gpr[ret], v1_t, v2_t);
            break;
        case OPC_SUBUH_R_QB:
            gen_helper_subuh_r_qb(cpu_gpr[ret], v1_t, v2_t);
            break;
        case OPC_SUBQH_PH:
            gen_helper_subqh_ph(cpu_gpr[ret], v1_t, v2_t);
            break;
        case OPC_SUBQH_R_PH:
            gen_helper_subqh_r_ph(cpu_gpr[ret], v1_t, v2_t);
            break;
        case OPC_SUBQH_W:
            gen_helper_subqh_w(cpu_gpr[ret], v1_t, v2_t);
            break;
        case OPC_SUBQH_R_W:
            gen_helper_subqh_r_w(cpu_gpr[ret], v1_t, v2_t);
            break;
        }
        break;
    case OPC_ABSQ_S_PH_DSP:
        switch (op2) {
        case OPC_ABSQ_S_QB:
            check_dsp_r2(ctx);
            gen_helper_absq_s_qb(cpu_gpr[ret], v2_t, cpu_env);
            break;
        case OPC_ABSQ_S_PH:
            check_dsp(ctx);
            gen_helper_absq_s_ph(cpu_gpr[ret], v2_t, cpu_env);
            break;
        case OPC_ABSQ_S_W:
            check_dsp(ctx);
            gen_helper_absq_s_w(cpu_gpr[ret], v2_t, cpu_env);
            break;
        case OPC_PRECEQ_W_PHL:
            check_dsp(ctx);
            tcg_gen_andi_tl(cpu_gpr[ret], v2_t, 0xFFFF0000);
            tcg_gen_ext32s_tl(cpu_gpr[ret], cpu_gpr[ret]);
            break;
        case OPC_PRECEQ_W_PHR:
            check_dsp(ctx);
            tcg_gen_andi_tl(cpu_gpr[ret], v2_t, 0x0000FFFF);
            tcg_gen_shli_tl(cpu_gpr[ret], cpu_gpr[ret], 16);
            tcg_gen_ext32s_tl(cpu_gpr[ret], cpu_gpr[ret]);
            break;
        case OPC_PRECEQU_PH_QBL:
            check_dsp(ctx);
            gen_helper_precequ_ph_qbl(cpu_gpr[ret], v2_t);
            break;
        case OPC_PRECEQU_PH_QBR:
            check_dsp(ctx);
            gen_helper_precequ_ph_qbr(cpu_gpr[ret], v2_t);
            break;
        case OPC_PRECEQU_PH_QBLA:
            check_dsp(ctx);
            gen_helper_precequ_ph_qbla(cpu_gpr[ret], v2_t);
            break;
        case OPC_PRECEQU_PH_QBRA:
            check_dsp(ctx);
            gen_helper_precequ_ph_qbra(cpu_gpr[ret], v2_t);
            break;
        case OPC_PRECEU_PH_QBL:
            check_dsp(ctx);
            gen_helper_preceu_ph_qbl(cpu_gpr[ret], v2_t);
            break;
        case OPC_PRECEU_PH_QBR:
            check_dsp(ctx);
            gen_helper_preceu_ph_qbr(cpu_gpr[ret], v2_t);
            break;
        case OPC_PRECEU_PH_QBLA:
            check_dsp(ctx);
            gen_helper_preceu_ph_qbla(cpu_gpr[ret], v2_t);
            break;
        case OPC_PRECEU_PH_QBRA:
            check_dsp(ctx);
            gen_helper_preceu_ph_qbra(cpu_gpr[ret], v2_t);
            break;
        }
        break;
    case OPC_ADDU_QB_DSP:
        switch (op2) {
        case OPC_ADDQ_PH:
            check_dsp(ctx);
            gen_helper_addq_ph(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_ADDQ_S_PH:
            check_dsp(ctx);
            gen_helper_addq_s_ph(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_ADDQ_S_W:
            check_dsp(ctx);
            gen_helper_addq_s_w(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_ADDU_QB:
            check_dsp(ctx);
            gen_helper_addu_qb(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_ADDU_S_QB:
            check_dsp(ctx);
            gen_helper_addu_s_qb(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_ADDU_PH:
            check_dsp_r2(ctx);
            gen_helper_addu_ph(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_ADDU_S_PH:
            check_dsp_r2(ctx);
            gen_helper_addu_s_ph(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_SUBQ_PH:
            check_dsp(ctx);
            gen_helper_subq_ph(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_SUBQ_S_PH:
            check_dsp(ctx);
            gen_helper_subq_s_ph(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_SUBQ_S_W:
            check_dsp(ctx);
            gen_helper_subq_s_w(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_SUBU_QB:
            check_dsp(ctx);
            gen_helper_subu_qb(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_SUBU_S_QB:
            check_dsp(ctx);
            gen_helper_subu_s_qb(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_SUBU_PH:
            check_dsp_r2(ctx);
            gen_helper_subu_ph(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_SUBU_S_PH:
            check_dsp_r2(ctx);
            gen_helper_subu_s_ph(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_ADDSC:
            check_dsp(ctx);
            gen_helper_addsc(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_ADDWC:
            check_dsp(ctx);
            gen_helper_addwc(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_MODSUB:
            check_dsp(ctx);
            gen_helper_modsub(cpu_gpr[ret], v1_t, v2_t);
            break;
        case OPC_RADDU_W_QB:
            check_dsp(ctx);
            gen_helper_raddu_w_qb(cpu_gpr[ret], v1_t);
            break;
        }
        break;
    case OPC_CMPU_EQ_QB_DSP:
        switch (op2) {
        case OPC_PRECR_QB_PH:
            check_dsp_r2(ctx);
            gen_helper_precr_qb_ph(cpu_gpr[ret], v1_t, v2_t);
            break;
        case OPC_PRECRQ_QB_PH:
            check_dsp(ctx);
            gen_helper_precrq_qb_ph(cpu_gpr[ret], v1_t, v2_t);
            break;
        case OPC_PRECR_SRA_PH_W:
            check_dsp_r2(ctx);
            {
                TCGv_i32 sa_t = tcg_const_i32(v2);
                gen_helper_precr_sra_ph_w(cpu_gpr[ret], sa_t, v1_t,
                                          cpu_gpr[ret]);
                tcg_temp_free_i32(sa_t);
                break;
            }
        case OPC_PRECR_SRA_R_PH_W:
            check_dsp_r2(ctx);
            {
                TCGv_i32 sa_t = tcg_const_i32(v2);
                gen_helper_precr_sra_r_ph_w(cpu_gpr[ret], sa_t, v1_t,
                                            cpu_gpr[ret]);
                tcg_temp_free_i32(sa_t);
                break;
            }
        case OPC_PRECRQ_PH_W:
            check_dsp(ctx);
            gen_helper_precrq_ph_w(cpu_gpr[ret], v1_t, v2_t);
            break;
        case OPC_PRECRQ_RS_PH_W:
            check_dsp(ctx);
            gen_helper_precrq_rs_ph_w(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_PRECRQU_S_QB_PH:
            check_dsp(ctx);
            gen_helper_precrqu_s_qb_ph(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        }
        break;
#ifdef TARGET_MIPS64
    case OPC_ABSQ_S_QH_DSP:
        switch (op2) {
        case OPC_PRECEQ_L_PWL:
            check_dsp(ctx);
            tcg_gen_andi_tl(cpu_gpr[ret], v2_t, 0xFFFFFFFF00000000ull);
            break;
        case OPC_PRECEQ_L_PWR:
            check_dsp(ctx);
            tcg_gen_shli_tl(cpu_gpr[ret], v2_t, 32);
            break;
        case OPC_PRECEQ_PW_QHL:
            check_dsp(ctx);
            gen_helper_preceq_pw_qhl(cpu_gpr[ret], v2_t);
            break;
        case OPC_PRECEQ_PW_QHR:
            check_dsp(ctx);
            gen_helper_preceq_pw_qhr(cpu_gpr[ret], v2_t);
            break;
        case OPC_PRECEQ_PW_QHLA:
            check_dsp(ctx);
            gen_helper_preceq_pw_qhla(cpu_gpr[ret], v2_t);
            break;
        case OPC_PRECEQ_PW_QHRA:
            check_dsp(ctx);
            gen_helper_preceq_pw_qhra(cpu_gpr[ret], v2_t);
            break;
        case OPC_PRECEQU_QH_OBL:
            check_dsp(ctx);
            gen_helper_precequ_qh_obl(cpu_gpr[ret], v2_t);
            break;
        case OPC_PRECEQU_QH_OBR:
            check_dsp(ctx);
            gen_helper_precequ_qh_obr(cpu_gpr[ret], v2_t);
            break;
        case OPC_PRECEQU_QH_OBLA:
            check_dsp(ctx);
            gen_helper_precequ_qh_obla(cpu_gpr[ret], v2_t);
            break;
        case OPC_PRECEQU_QH_OBRA:
            check_dsp(ctx);
            gen_helper_precequ_qh_obra(cpu_gpr[ret], v2_t);
            break;
        case OPC_PRECEU_QH_OBL:
            check_dsp(ctx);
            gen_helper_preceu_qh_obl(cpu_gpr[ret], v2_t);
            break;
        case OPC_PRECEU_QH_OBR:
            check_dsp(ctx);
            gen_helper_preceu_qh_obr(cpu_gpr[ret], v2_t);
            break;
        case OPC_PRECEU_QH_OBLA:
            check_dsp(ctx);
            gen_helper_preceu_qh_obla(cpu_gpr[ret], v2_t);
            break;
        case OPC_PRECEU_QH_OBRA:
            check_dsp(ctx);
            gen_helper_preceu_qh_obra(cpu_gpr[ret], v2_t);
            break;
        case OPC_ABSQ_S_OB:
            check_dsp_r2(ctx);
            gen_helper_absq_s_ob(cpu_gpr[ret], v2_t, cpu_env);
            break;
        case OPC_ABSQ_S_PW:
            check_dsp(ctx);
            gen_helper_absq_s_pw(cpu_gpr[ret], v2_t, cpu_env);
            break;
        case OPC_ABSQ_S_QH:
            check_dsp(ctx);
            gen_helper_absq_s_qh(cpu_gpr[ret], v2_t, cpu_env);
            break;
        }
        break;
    case OPC_ADDU_OB_DSP:
        switch (op2) {
        case OPC_RADDU_L_OB:
            check_dsp(ctx);
            gen_helper_raddu_l_ob(cpu_gpr[ret], v1_t);
            break;
        case OPC_SUBQ_PW:
            check_dsp(ctx);
            gen_helper_subq_pw(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_SUBQ_S_PW:
            check_dsp(ctx);
            gen_helper_subq_s_pw(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_SUBQ_QH:
            check_dsp(ctx);
            gen_helper_subq_qh(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_SUBQ_S_QH:
            check_dsp(ctx);
            gen_helper_subq_s_qh(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_SUBU_OB:
            check_dsp(ctx);
            gen_helper_subu_ob(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_SUBU_S_OB:
            check_dsp(ctx);
            gen_helper_subu_s_ob(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_SUBU_QH:
            check_dsp_r2(ctx);
            gen_helper_subu_qh(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_SUBU_S_QH:
            check_dsp_r2(ctx);
            gen_helper_subu_s_qh(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_SUBUH_OB:
            check_dsp_r2(ctx);
            gen_helper_subuh_ob(cpu_gpr[ret], v1_t, v2_t);
            break;
        case OPC_SUBUH_R_OB:
            check_dsp_r2(ctx);
            gen_helper_subuh_r_ob(cpu_gpr[ret], v1_t, v2_t);
            break;
        case OPC_ADDQ_PW:
            check_dsp(ctx);
            gen_helper_addq_pw(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_ADDQ_S_PW:
            check_dsp(ctx);
            gen_helper_addq_s_pw(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_ADDQ_QH:
            check_dsp(ctx);
            gen_helper_addq_qh(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_ADDQ_S_QH:
            check_dsp(ctx);
            gen_helper_addq_s_qh(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_ADDU_OB:
            check_dsp(ctx);
            gen_helper_addu_ob(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_ADDU_S_OB:
            check_dsp(ctx);
            gen_helper_addu_s_ob(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_ADDU_QH:
            check_dsp_r2(ctx);
            gen_helper_addu_qh(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_ADDU_S_QH:
            check_dsp_r2(ctx);
            gen_helper_addu_s_qh(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_ADDUH_OB:
            check_dsp_r2(ctx);
            gen_helper_adduh_ob(cpu_gpr[ret], v1_t, v2_t);
            break;
        case OPC_ADDUH_R_OB:
            check_dsp_r2(ctx);
            gen_helper_adduh_r_ob(cpu_gpr[ret], v1_t, v2_t);
            break;
        }
        break;
    case OPC_CMPU_EQ_OB_DSP:
        switch (op2) {
        case OPC_PRECR_OB_QH:
            check_dsp_r2(ctx);
            gen_helper_precr_ob_qh(cpu_gpr[ret], v1_t, v2_t);
            break;
        case OPC_PRECR_SRA_QH_PW:
            check_dsp_r2(ctx);
            {
                TCGv_i32 ret_t = tcg_const_i32(ret);
                gen_helper_precr_sra_qh_pw(v2_t, v1_t, v2_t, ret_t);
                tcg_temp_free_i32(ret_t);
                break;
            }
        case OPC_PRECR_SRA_R_QH_PW:
            check_dsp_r2(ctx);
            {
                TCGv_i32 sa_v = tcg_const_i32(ret);
                gen_helper_precr_sra_r_qh_pw(v2_t, v1_t, v2_t, sa_v);
                tcg_temp_free_i32(sa_v);
                break;
            }
        case OPC_PRECRQ_OB_QH:
            check_dsp(ctx);
            gen_helper_precrq_ob_qh(cpu_gpr[ret], v1_t, v2_t);
            break;
        case OPC_PRECRQ_PW_L:
            check_dsp(ctx);
            gen_helper_precrq_pw_l(cpu_gpr[ret], v1_t, v2_t);
            break;
        case OPC_PRECRQ_QH_PW:
            check_dsp(ctx);
            gen_helper_precrq_qh_pw(cpu_gpr[ret], v1_t, v2_t);
            break;
        case OPC_PRECRQ_RS_QH_PW:
            check_dsp(ctx);
            gen_helper_precrq_rs_qh_pw(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_PRECRQU_S_OB_QH:
            check_dsp(ctx);
            gen_helper_precrqu_s_ob_qh(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        }
        break;
#endif
    }

    tcg_temp_free(v1_t);
    tcg_temp_free(v2_t);
}

static void gen_mipsdsp_shift(DisasContext *ctx, uint32_t opc,
                              int ret, int v1, int v2)
{
    uint32_t op2;
    TCGv t0;
    TCGv v1_t;
    TCGv v2_t;

    if (ret == 0) {
        /* Treat as NOP. */
        return;
    }

    t0 = tcg_temp_new();
    v1_t = tcg_temp_new();
    v2_t = tcg_temp_new();

    tcg_gen_movi_tl(t0, v1);
    gen_load_gpr(v1_t, v1);
    gen_load_gpr(v2_t, v2);

    switch (opc) {
    case OPC_SHLL_QB_DSP:
        {
            op2 = MASK_SHLL_QB(ctx->opcode);
            switch (op2) {
            case OPC_SHLL_QB:
                check_dsp(ctx);
                gen_helper_shll_qb(cpu_gpr[ret], t0, v2_t, cpu_env);
                break;
            case OPC_SHLLV_QB:
                check_dsp(ctx);
                gen_helper_shll_qb(cpu_gpr[ret], v1_t, v2_t, cpu_env);
                break;
            case OPC_SHLL_PH:
                check_dsp(ctx);
                gen_helper_shll_ph(cpu_gpr[ret], t0, v2_t, cpu_env);
                break;
            case OPC_SHLLV_PH:
                check_dsp(ctx);
                gen_helper_shll_ph(cpu_gpr[ret], v1_t, v2_t, cpu_env);
                break;
            case OPC_SHLL_S_PH:
                check_dsp(ctx);
                gen_helper_shll_s_ph(cpu_gpr[ret], t0, v2_t, cpu_env);
                break;
            case OPC_SHLLV_S_PH:
                check_dsp(ctx);
                gen_helper_shll_s_ph(cpu_gpr[ret], v1_t, v2_t, cpu_env);
                break;
            case OPC_SHLL_S_W:
                check_dsp(ctx);
                gen_helper_shll_s_w(cpu_gpr[ret], t0, v2_t, cpu_env);
                break;
            case OPC_SHLLV_S_W:
                check_dsp(ctx);
                gen_helper_shll_s_w(cpu_gpr[ret], v1_t, v2_t, cpu_env);
                break;
            case OPC_SHRL_QB:
                check_dsp(ctx);
                gen_helper_shrl_qb(cpu_gpr[ret], t0, v2_t);
                break;
            case OPC_SHRLV_QB:
                check_dsp(ctx);
                gen_helper_shrl_qb(cpu_gpr[ret], v1_t, v2_t);
                break;
            case OPC_SHRL_PH:
                check_dsp_r2(ctx);
                gen_helper_shrl_ph(cpu_gpr[ret], t0, v2_t);
                break;
            case OPC_SHRLV_PH:
                check_dsp_r2(ctx);
                gen_helper_shrl_ph(cpu_gpr[ret], v1_t, v2_t);
                break;
            case OPC_SHRA_QB:
                check_dsp_r2(ctx);
                gen_helper_shra_qb(cpu_gpr[ret], t0, v2_t);
                break;
            case OPC_SHRA_R_QB:
                check_dsp_r2(ctx);
                gen_helper_shra_r_qb(cpu_gpr[ret], t0, v2_t);
                break;
            case OPC_SHRAV_QB:
                check_dsp_r2(ctx);
                gen_helper_shra_qb(cpu_gpr[ret], v1_t, v2_t);
                break;
            case OPC_SHRAV_R_QB:
                check_dsp_r2(ctx);
                gen_helper_shra_r_qb(cpu_gpr[ret], v1_t, v2_t);
                break;
            case OPC_SHRA_PH:
                check_dsp(ctx);
                gen_helper_shra_ph(cpu_gpr[ret], t0, v2_t);
                break;
            case OPC_SHRA_R_PH:
                check_dsp(ctx);
                gen_helper_shra_r_ph(cpu_gpr[ret], t0, v2_t);
                break;
            case OPC_SHRAV_PH:
                check_dsp(ctx);
                gen_helper_shra_ph(cpu_gpr[ret], v1_t, v2_t);
                break;
            case OPC_SHRAV_R_PH:
                check_dsp(ctx);
                gen_helper_shra_r_ph(cpu_gpr[ret], v1_t, v2_t);
                break;
            case OPC_SHRA_R_W:
                check_dsp(ctx);
                gen_helper_shra_r_w(cpu_gpr[ret], t0, v2_t);
                break;
            case OPC_SHRAV_R_W:
                check_dsp(ctx);
                gen_helper_shra_r_w(cpu_gpr[ret], v1_t, v2_t);
                break;
            default:            /* Invalid */
                MIPS_INVAL("MASK SHLL.QB");
                gen_reserved_instruction(ctx);
                break;
            }
            break;
        }
#ifdef TARGET_MIPS64
    case OPC_SHLL_OB_DSP:
        op2 = MASK_SHLL_OB(ctx->opcode);
        switch (op2) {
        case OPC_SHLL_PW:
            check_dsp(ctx);
            gen_helper_shll_pw(cpu_gpr[ret], v2_t, t0, cpu_env);
            break;
        case OPC_SHLLV_PW:
            check_dsp(ctx);
            gen_helper_shll_pw(cpu_gpr[ret], v2_t, v1_t, cpu_env);
            break;
        case OPC_SHLL_S_PW:
            check_dsp(ctx);
            gen_helper_shll_s_pw(cpu_gpr[ret], v2_t, t0, cpu_env);
            break;
        case OPC_SHLLV_S_PW:
            check_dsp(ctx);
            gen_helper_shll_s_pw(cpu_gpr[ret], v2_t, v1_t, cpu_env);
            break;
        case OPC_SHLL_OB:
            check_dsp(ctx);
            gen_helper_shll_ob(cpu_gpr[ret], v2_t, t0, cpu_env);
            break;
        case OPC_SHLLV_OB:
            check_dsp(ctx);
            gen_helper_shll_ob(cpu_gpr[ret], v2_t, v1_t, cpu_env);
            break;
        case OPC_SHLL_QH:
            check_dsp(ctx);
            gen_helper_shll_qh(cpu_gpr[ret], v2_t, t0, cpu_env);
            break;
        case OPC_SHLLV_QH:
            check_dsp(ctx);
            gen_helper_shll_qh(cpu_gpr[ret], v2_t, v1_t, cpu_env);
            break;
        case OPC_SHLL_S_QH:
            check_dsp(ctx);
            gen_helper_shll_s_qh(cpu_gpr[ret], v2_t, t0, cpu_env);
            break;
        case OPC_SHLLV_S_QH:
            check_dsp(ctx);
            gen_helper_shll_s_qh(cpu_gpr[ret], v2_t, v1_t, cpu_env);
            break;
        case OPC_SHRA_OB:
            check_dsp_r2(ctx);
            gen_helper_shra_ob(cpu_gpr[ret], v2_t, t0);
            break;
        case OPC_SHRAV_OB:
            check_dsp_r2(ctx);
            gen_helper_shra_ob(cpu_gpr[ret], v2_t, v1_t);
            break;
        case OPC_SHRA_R_OB:
            check_dsp_r2(ctx);
            gen_helper_shra_r_ob(cpu_gpr[ret], v2_t, t0);
            break;
        case OPC_SHRAV_R_OB:
            check_dsp_r2(ctx);
            gen_helper_shra_r_ob(cpu_gpr[ret], v2_t, v1_t);
            break;
        case OPC_SHRA_PW:
            check_dsp(ctx);
            gen_helper_shra_pw(cpu_gpr[ret], v2_t, t0);
            break;
        case OPC_SHRAV_PW:
            check_dsp(ctx);
            gen_helper_shra_pw(cpu_gpr[ret], v2_t, v1_t);
            break;
        case OPC_SHRA_R_PW:
            check_dsp(ctx);
            gen_helper_shra_r_pw(cpu_gpr[ret], v2_t, t0);
            break;
        case OPC_SHRAV_R_PW:
            check_dsp(ctx);
            gen_helper_shra_r_pw(cpu_gpr[ret], v2_t, v1_t);
            break;
        case OPC_SHRA_QH:
            check_dsp(ctx);
            gen_helper_shra_qh(cpu_gpr[ret], v2_t, t0);
            break;
        case OPC_SHRAV_QH:
            check_dsp(ctx);
            gen_helper_shra_qh(cpu_gpr[ret], v2_t, v1_t);
            break;
        case OPC_SHRA_R_QH:
            check_dsp(ctx);
            gen_helper_shra_r_qh(cpu_gpr[ret], v2_t, t0);
            break;
        case OPC_SHRAV_R_QH:
            check_dsp(ctx);
            gen_helper_shra_r_qh(cpu_gpr[ret], v2_t, v1_t);
            break;
        case OPC_SHRL_OB:
            check_dsp(ctx);
            gen_helper_shrl_ob(cpu_gpr[ret], v2_t, t0);
            break;
        case OPC_SHRLV_OB:
            check_dsp(ctx);
            gen_helper_shrl_ob(cpu_gpr[ret], v2_t, v1_t);
            break;
        case OPC_SHRL_QH:
            check_dsp_r2(ctx);
            gen_helper_shrl_qh(cpu_gpr[ret], v2_t, t0);
            break;
        case OPC_SHRLV_QH:
            check_dsp_r2(ctx);
            gen_helper_shrl_qh(cpu_gpr[ret], v2_t, v1_t);
            break;
        default:            /* Invalid */
            MIPS_INVAL("MASK SHLL.OB");
            gen_reserved_instruction(ctx);
            break;
        }
        break;
#endif
    }

    tcg_temp_free(t0);
    tcg_temp_free(v1_t);
    tcg_temp_free(v2_t);
}

static void gen_mipsdsp_multiply(DisasContext *ctx, uint32_t op1, uint32_t op2,
                                 int ret, int v1, int v2, int check_ret)
{
    TCGv_i32 t0;
    TCGv v1_t;
    TCGv v2_t;

    if ((ret == 0) && (check_ret == 1)) {
        /* Treat as NOP. */
        return;
    }

    t0 = tcg_temp_new_i32();
    v1_t = tcg_temp_new();
    v2_t = tcg_temp_new();

    tcg_gen_movi_i32(t0, ret);
    gen_load_gpr(v1_t, v1);
    gen_load_gpr(v2_t, v2);

    switch (op1) {
    /*
     * OPC_MULT_G_2E, OPC_ADDUH_QB_DSP, OPC_MUL_PH_DSP have
     * the same mask and op1.
     */
    case OPC_MULT_G_2E:
        check_dsp_r2(ctx);
        switch (op2) {
        case  OPC_MUL_PH:
            gen_helper_mul_ph(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case  OPC_MUL_S_PH:
            gen_helper_mul_s_ph(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_MULQ_S_W:
            gen_helper_mulq_s_w(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_MULQ_RS_W:
            gen_helper_mulq_rs_w(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        }
        break;
    case OPC_DPA_W_PH_DSP:
        switch (op2) {
        case OPC_DPAU_H_QBL:
            check_dsp(ctx);
            gen_helper_dpau_h_qbl(t0, v1_t, v2_t, cpu_env);
            break;
        case OPC_DPAU_H_QBR:
            check_dsp(ctx);
            gen_helper_dpau_h_qbr(t0, v1_t, v2_t, cpu_env);
            break;
        case OPC_DPSU_H_QBL:
            check_dsp(ctx);
            gen_helper_dpsu_h_qbl(t0, v1_t, v2_t, cpu_env);
            break;
        case OPC_DPSU_H_QBR:
            check_dsp(ctx);
            gen_helper_dpsu_h_qbr(t0, v1_t, v2_t, cpu_env);
            break;
        case OPC_DPA_W_PH:
            check_dsp_r2(ctx);
            gen_helper_dpa_w_ph(t0, v1_t, v2_t, cpu_env);
            break;
        case OPC_DPAX_W_PH:
            check_dsp_r2(ctx);
            gen_helper_dpax_w_ph(t0, v1_t, v2_t, cpu_env);
            break;
        case OPC_DPAQ_S_W_PH:
            check_dsp(ctx);
            gen_helper_dpaq_s_w_ph(t0, v1_t, v2_t, cpu_env);
            break;
        case OPC_DPAQX_S_W_PH:
            check_dsp_r2(ctx);
            gen_helper_dpaqx_s_w_ph(t0, v1_t, v2_t, cpu_env);
            break;
        case OPC_DPAQX_SA_W_PH:
            check_dsp_r2(ctx);
            gen_helper_dpaqx_sa_w_ph(t0, v1_t, v2_t, cpu_env);
            break;
        case OPC_DPS_W_PH:
            check_dsp_r2(ctx);
            gen_helper_dps_w_ph(t0, v1_t, v2_t, cpu_env);
            break;
        case OPC_DPSX_W_PH:
            check_dsp_r2(ctx);
            gen_helper_dpsx_w_ph(t0, v1_t, v2_t, cpu_env);
            break;
        case OPC_DPSQ_S_W_PH:
            check_dsp(ctx);
            gen_helper_dpsq_s_w_ph(t0, v1_t, v2_t, cpu_env);
            break;
        case OPC_DPSQX_S_W_PH:
            check_dsp_r2(ctx);
            gen_helper_dpsqx_s_w_ph(t0, v1_t, v2_t, cpu_env);
            break;
        case OPC_DPSQX_SA_W_PH:
            check_dsp_r2(ctx);
            gen_helper_dpsqx_sa_w_ph(t0, v1_t, v2_t, cpu_env);
            break;
        case OPC_MULSAQ_S_W_PH:
            check_dsp(ctx);
            gen_helper_mulsaq_s_w_ph(t0, v1_t, v2_t, cpu_env);
            break;
        case OPC_DPAQ_SA_L_W:
            check_dsp(ctx);
            gen_helper_dpaq_sa_l_w(t0, v1_t, v2_t, cpu_env);
            break;
        case OPC_DPSQ_SA_L_W:
            check_dsp(ctx);
            gen_helper_dpsq_sa_l_w(t0, v1_t, v2_t, cpu_env);
            break;
        case OPC_MAQ_S_W_PHL:
            check_dsp(ctx);
            gen_helper_maq_s_w_phl(t0, v1_t, v2_t, cpu_env);
            break;
        case OPC_MAQ_S_W_PHR:
            check_dsp(ctx);
            gen_helper_maq_s_w_phr(t0, v1_t, v2_t, cpu_env);
            break;
        case OPC_MAQ_SA_W_PHL:
            check_dsp(ctx);
            gen_helper_maq_sa_w_phl(t0, v1_t, v2_t, cpu_env);
            break;
        case OPC_MAQ_SA_W_PHR:
            check_dsp(ctx);
            gen_helper_maq_sa_w_phr(t0, v1_t, v2_t, cpu_env);
            break;
        case OPC_MULSA_W_PH:
            check_dsp_r2(ctx);
            gen_helper_mulsa_w_ph(t0, v1_t, v2_t, cpu_env);
            break;
        }
        break;
#ifdef TARGET_MIPS64
    case OPC_DPAQ_W_QH_DSP:
        {
            int ac = ret & 0x03;
            tcg_gen_movi_i32(t0, ac);

            switch (op2) {
            case OPC_DMADD:
                check_dsp(ctx);
                gen_helper_dmadd(v1_t, v2_t, t0, cpu_env);
                break;
            case OPC_DMADDU:
                check_dsp(ctx);
                gen_helper_dmaddu(v1_t, v2_t, t0, cpu_env);
                break;
            case OPC_DMSUB:
                check_dsp(ctx);
                gen_helper_dmsub(v1_t, v2_t, t0, cpu_env);
                break;
            case OPC_DMSUBU:
                check_dsp(ctx);
                gen_helper_dmsubu(v1_t, v2_t, t0, cpu_env);
                break;
            case OPC_DPA_W_QH:
                check_dsp_r2(ctx);
                gen_helper_dpa_w_qh(v1_t, v2_t, t0, cpu_env);
                break;
            case OPC_DPAQ_S_W_QH:
                check_dsp(ctx);
                gen_helper_dpaq_s_w_qh(v1_t, v2_t, t0, cpu_env);
                break;
            case OPC_DPAQ_SA_L_PW:
                check_dsp(ctx);
                gen_helper_dpaq_sa_l_pw(v1_t, v2_t, t0, cpu_env);
                break;
            case OPC_DPAU_H_OBL:
                check_dsp(ctx);
                gen_helper_dpau_h_obl(v1_t, v2_t, t0, cpu_env);
                break;
            case OPC_DPAU_H_OBR:
                check_dsp(ctx);
                gen_helper_dpau_h_obr(v1_t, v2_t, t0, cpu_env);
                break;
            case OPC_DPS_W_QH:
                check_dsp_r2(ctx);
                gen_helper_dps_w_qh(v1_t, v2_t, t0, cpu_env);
                break;
            case OPC_DPSQ_S_W_QH:
                check_dsp(ctx);
                gen_helper_dpsq_s_w_qh(v1_t, v2_t, t0, cpu_env);
                break;
            case OPC_DPSQ_SA_L_PW:
                check_dsp(ctx);
                gen_helper_dpsq_sa_l_pw(v1_t, v2_t, t0, cpu_env);
                break;
            case OPC_DPSU_H_OBL:
                check_dsp(ctx);
                gen_helper_dpsu_h_obl(v1_t, v2_t, t0, cpu_env);
                break;
            case OPC_DPSU_H_OBR:
                check_dsp(ctx);
                gen_helper_dpsu_h_obr(v1_t, v2_t, t0, cpu_env);
                break;
            case OPC_MAQ_S_L_PWL:
                check_dsp(ctx);
                gen_helper_maq_s_l_pwl(v1_t, v2_t, t0, cpu_env);
                break;
            case OPC_MAQ_S_L_PWR:
                check_dsp(ctx);
                gen_helper_maq_s_l_pwr(v1_t, v2_t, t0, cpu_env);
                break;
            case OPC_MAQ_S_W_QHLL:
                check_dsp(ctx);
                gen_helper_maq_s_w_qhll(v1_t, v2_t, t0, cpu_env);
                break;
            case OPC_MAQ_SA_W_QHLL:
                check_dsp(ctx);
                gen_helper_maq_sa_w_qhll(v1_t, v2_t, t0, cpu_env);
                break;
            case OPC_MAQ_S_W_QHLR:
                check_dsp(ctx);
                gen_helper_maq_s_w_qhlr(v1_t, v2_t, t0, cpu_env);
                break;
            case OPC_MAQ_SA_W_QHLR:
                check_dsp(ctx);
                gen_helper_maq_sa_w_qhlr(v1_t, v2_t, t0, cpu_env);
                break;
            case OPC_MAQ_S_W_QHRL:
                check_dsp(ctx);
                gen_helper_maq_s_w_qhrl(v1_t, v2_t, t0, cpu_env);
                break;
            case OPC_MAQ_SA_W_QHRL:
                check_dsp(ctx);
                gen_helper_maq_sa_w_qhrl(v1_t, v2_t, t0, cpu_env);
                break;
            case OPC_MAQ_S_W_QHRR:
                check_dsp(ctx);
                gen_helper_maq_s_w_qhrr(v1_t, v2_t, t0, cpu_env);
                break;
            case OPC_MAQ_SA_W_QHRR:
                check_dsp(ctx);
                gen_helper_maq_sa_w_qhrr(v1_t, v2_t, t0, cpu_env);
                break;
            case OPC_MULSAQ_S_L_PW:
                check_dsp(ctx);
                gen_helper_mulsaq_s_l_pw(v1_t, v2_t, t0, cpu_env);
                break;
            case OPC_MULSAQ_S_W_QH:
                check_dsp(ctx);
                gen_helper_mulsaq_s_w_qh(v1_t, v2_t, t0, cpu_env);
                break;
            }
        }
        break;
#endif
    case OPC_ADDU_QB_DSP:
        switch (op2) {
        case OPC_MULEU_S_PH_QBL:
            check_dsp(ctx);
            gen_helper_muleu_s_ph_qbl(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_MULEU_S_PH_QBR:
            check_dsp(ctx);
            gen_helper_muleu_s_ph_qbr(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_MULQ_RS_PH:
            check_dsp(ctx);
            gen_helper_mulq_rs_ph(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_MULEQ_S_W_PHL:
            check_dsp(ctx);
            gen_helper_muleq_s_w_phl(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_MULEQ_S_W_PHR:
            check_dsp(ctx);
            gen_helper_muleq_s_w_phr(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_MULQ_S_PH:
            check_dsp_r2(ctx);
            gen_helper_mulq_s_ph(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        }
        break;
#ifdef TARGET_MIPS64
    case OPC_ADDU_OB_DSP:
        switch (op2) {
        case OPC_MULEQ_S_PW_QHL:
            check_dsp(ctx);
            gen_helper_muleq_s_pw_qhl(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_MULEQ_S_PW_QHR:
            check_dsp(ctx);
            gen_helper_muleq_s_pw_qhr(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_MULEU_S_QH_OBL:
            check_dsp(ctx);
            gen_helper_muleu_s_qh_obl(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_MULEU_S_QH_OBR:
            check_dsp(ctx);
            gen_helper_muleu_s_qh_obr(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_MULQ_RS_QH:
            check_dsp(ctx);
            gen_helper_mulq_rs_qh(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        }
        break;
#endif
    }

    tcg_temp_free_i32(t0);
    tcg_temp_free(v1_t);
    tcg_temp_free(v2_t);
}

static void gen_mipsdsp_bitinsn(DisasContext *ctx, uint32_t op1, uint32_t op2,
                                int ret, int val)
{
    int16_t imm;
    TCGv t0;
    TCGv val_t;

    if (ret == 0) {
        /* Treat as NOP. */
        return;
    }

    t0 = tcg_temp_new();
    val_t = tcg_temp_new();
    gen_load_gpr(val_t, val);

    switch (op1) {
    case OPC_ABSQ_S_PH_DSP:
        switch (op2) {
        case OPC_BITREV:
            check_dsp(ctx);
            gen_helper_bitrev(cpu_gpr[ret], val_t);
            break;
        case OPC_REPL_QB:
            check_dsp(ctx);
            {
                target_long result;
                imm = (ctx->opcode >> 16) & 0xFF;
                result = (uint32_t)imm << 24 |
                         (uint32_t)imm << 16 |
                         (uint32_t)imm << 8  |
                         (uint32_t)imm;
                result = (int32_t)result;
                tcg_gen_movi_tl(cpu_gpr[ret], result);
            }
            break;
        case OPC_REPLV_QB:
            check_dsp(ctx);
            tcg_gen_ext8u_tl(cpu_gpr[ret], val_t);
            tcg_gen_shli_tl(t0, cpu_gpr[ret], 8);
            tcg_gen_or_tl(cpu_gpr[ret], cpu_gpr[ret], t0);
            tcg_gen_shli_tl(t0, cpu_gpr[ret], 16);
            tcg_gen_or_tl(cpu_gpr[ret], cpu_gpr[ret], t0);
            tcg_gen_ext32s_tl(cpu_gpr[ret], cpu_gpr[ret]);
            break;
        case OPC_REPL_PH:
            check_dsp(ctx);
            {
                imm = (ctx->opcode >> 16) & 0x03FF;
                imm = (int16_t)(imm << 6) >> 6;
                tcg_gen_movi_tl(cpu_gpr[ret], \
                                (target_long)((int32_t)imm << 16 | \
                                (uint16_t)imm));
            }
            break;
        case OPC_REPLV_PH:
            check_dsp(ctx);
            tcg_gen_ext16u_tl(cpu_gpr[ret], val_t);
            tcg_gen_shli_tl(t0, cpu_gpr[ret], 16);
            tcg_gen_or_tl(cpu_gpr[ret], cpu_gpr[ret], t0);
            tcg_gen_ext32s_tl(cpu_gpr[ret], cpu_gpr[ret]);
            break;
        }
        break;
#ifdef TARGET_MIPS64
    case OPC_ABSQ_S_QH_DSP:
        switch (op2) {
        case OPC_REPL_OB:
            check_dsp(ctx);
            {
                target_long temp;

                imm = (ctx->opcode >> 16) & 0xFF;
                temp = ((uint64_t)imm << 8) | (uint64_t)imm;
                temp = (temp << 16) | temp;
                temp = (temp << 32) | temp;
                tcg_gen_movi_tl(cpu_gpr[ret], temp);
                break;
            }
        case OPC_REPL_PW:
            check_dsp(ctx);
            {
                target_long temp;

                imm = (ctx->opcode >> 16) & 0x03FF;
                imm = (int16_t)(imm << 6) >> 6;
                temp = ((target_long)imm << 32) \
                       | ((target_long)imm & 0xFFFFFFFF);
                tcg_gen_movi_tl(cpu_gpr[ret], temp);
                break;
            }
        case OPC_REPL_QH:
            check_dsp(ctx);
            {
                target_long temp;

                imm = (ctx->opcode >> 16) & 0x03FF;
                imm = (int16_t)(imm << 6) >> 6;

                temp = ((uint64_t)(uint16_t)imm << 48) |
                       ((uint64_t)(uint16_t)imm << 32) |
                       ((uint64_t)(uint16_t)imm << 16) |
                       (uint64_t)(uint16_t)imm;
                tcg_gen_movi_tl(cpu_gpr[ret], temp);
                break;
            }
        case OPC_REPLV_OB:
            check_dsp(ctx);
            tcg_gen_ext8u_tl(cpu_gpr[ret], val_t);
            tcg_gen_shli_tl(t0, cpu_gpr[ret], 8);
            tcg_gen_or_tl(cpu_gpr[ret], cpu_gpr[ret], t0);
            tcg_gen_shli_tl(t0, cpu_gpr[ret], 16);
            tcg_gen_or_tl(cpu_gpr[ret], cpu_gpr[ret], t0);
            tcg_gen_shli_tl(t0, cpu_gpr[ret], 32);
            tcg_gen_or_tl(cpu_gpr[ret], cpu_gpr[ret], t0);
            break;
        case OPC_REPLV_PW:
            check_dsp(ctx);
            tcg_gen_ext32u_i64(cpu_gpr[ret], val_t);
            tcg_gen_shli_tl(t0, cpu_gpr[ret], 32);
            tcg_gen_or_tl(cpu_gpr[ret], cpu_gpr[ret], t0);
            break;
        case OPC_REPLV_QH:
            check_dsp(ctx);
            tcg_gen_ext16u_tl(cpu_gpr[ret], val_t);
            tcg_gen_shli_tl(t0, cpu_gpr[ret], 16);
            tcg_gen_or_tl(cpu_gpr[ret], cpu_gpr[ret], t0);
            tcg_gen_shli_tl(t0, cpu_gpr[ret], 32);
            tcg_gen_or_tl(cpu_gpr[ret], cpu_gpr[ret], t0);
            break;
        }
        break;
#endif
    }
    tcg_temp_free(t0);
    tcg_temp_free(val_t);
}

static void gen_mipsdsp_add_cmp_pick(DisasContext *ctx,
                                     uint32_t op1, uint32_t op2,
                                     int ret, int v1, int v2, int check_ret)
{
    TCGv t1;
    TCGv v1_t;
    TCGv v2_t;

    if ((ret == 0) && (check_ret == 1)) {
        /* Treat as NOP. */
        return;
    }

    t1 = tcg_temp_new();
    v1_t = tcg_temp_new();
    v2_t = tcg_temp_new();

    gen_load_gpr(v1_t, v1);
    gen_load_gpr(v2_t, v2);

    switch (op1) {
    case OPC_CMPU_EQ_QB_DSP:
        switch (op2) {
        case OPC_CMPU_EQ_QB:
            check_dsp(ctx);
            gen_helper_cmpu_eq_qb(v1_t, v2_t, cpu_env);
            break;
        case OPC_CMPU_LT_QB:
            check_dsp(ctx);
            gen_helper_cmpu_lt_qb(v1_t, v2_t, cpu_env);
            break;
        case OPC_CMPU_LE_QB:
            check_dsp(ctx);
            gen_helper_cmpu_le_qb(v1_t, v2_t, cpu_env);
            break;
        case OPC_CMPGU_EQ_QB:
            check_dsp(ctx);
            gen_helper_cmpgu_eq_qb(cpu_gpr[ret], v1_t, v2_t);
            break;
        case OPC_CMPGU_LT_QB:
            check_dsp(ctx);
            gen_helper_cmpgu_lt_qb(cpu_gpr[ret], v1_t, v2_t);
            break;
        case OPC_CMPGU_LE_QB:
            check_dsp(ctx);
            gen_helper_cmpgu_le_qb(cpu_gpr[ret], v1_t, v2_t);
            break;
        case OPC_CMPGDU_EQ_QB:
            check_dsp_r2(ctx);
            gen_helper_cmpgu_eq_qb(t1, v1_t, v2_t);
            tcg_gen_mov_tl(cpu_gpr[ret], t1);
            tcg_gen_andi_tl(cpu_dspctrl, cpu_dspctrl, 0xF0FFFFFF);
            tcg_gen_shli_tl(t1, t1, 24);
            tcg_gen_or_tl(cpu_dspctrl, cpu_dspctrl, t1);
            break;
        case OPC_CMPGDU_LT_QB:
            check_dsp_r2(ctx);
            gen_helper_cmpgu_lt_qb(t1, v1_t, v2_t);
            tcg_gen_mov_tl(cpu_gpr[ret], t1);
            tcg_gen_andi_tl(cpu_dspctrl, cpu_dspctrl, 0xF0FFFFFF);
            tcg_gen_shli_tl(t1, t1, 24);
            tcg_gen_or_tl(cpu_dspctrl, cpu_dspctrl, t1);
            break;
        case OPC_CMPGDU_LE_QB:
            check_dsp_r2(ctx);
            gen_helper_cmpgu_le_qb(t1, v1_t, v2_t);
            tcg_gen_mov_tl(cpu_gpr[ret], t1);
            tcg_gen_andi_tl(cpu_dspctrl, cpu_dspctrl, 0xF0FFFFFF);
            tcg_gen_shli_tl(t1, t1, 24);
            tcg_gen_or_tl(cpu_dspctrl, cpu_dspctrl, t1);
            break;
        case OPC_CMP_EQ_PH:
            check_dsp(ctx);
            gen_helper_cmp_eq_ph(v1_t, v2_t, cpu_env);
            break;
        case OPC_CMP_LT_PH:
            check_dsp(ctx);
            gen_helper_cmp_lt_ph(v1_t, v2_t, cpu_env);
            break;
        case OPC_CMP_LE_PH:
            check_dsp(ctx);
            gen_helper_cmp_le_ph(v1_t, v2_t, cpu_env);
            break;
        case OPC_PICK_QB:
            check_dsp(ctx);
            gen_helper_pick_qb(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_PICK_PH:
            check_dsp(ctx);
            gen_helper_pick_ph(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_PACKRL_PH:
            check_dsp(ctx);
            gen_helper_packrl_ph(cpu_gpr[ret], v1_t, v2_t);
            break;
        }
        break;
#ifdef TARGET_MIPS64
    case OPC_CMPU_EQ_OB_DSP:
        switch (op2) {
        case OPC_CMP_EQ_PW:
            check_dsp(ctx);
            gen_helper_cmp_eq_pw(v1_t, v2_t, cpu_env);
            break;
        case OPC_CMP_LT_PW:
            check_dsp(ctx);
            gen_helper_cmp_lt_pw(v1_t, v2_t, cpu_env);
            break;
        case OPC_CMP_LE_PW:
            check_dsp(ctx);
            gen_helper_cmp_le_pw(v1_t, v2_t, cpu_env);
            break;
        case OPC_CMP_EQ_QH:
            check_dsp(ctx);
            gen_helper_cmp_eq_qh(v1_t, v2_t, cpu_env);
            break;
        case OPC_CMP_LT_QH:
            check_dsp(ctx);
            gen_helper_cmp_lt_qh(v1_t, v2_t, cpu_env);
            break;
        case OPC_CMP_LE_QH:
            check_dsp(ctx);
            gen_helper_cmp_le_qh(v1_t, v2_t, cpu_env);
            break;
        case OPC_CMPGDU_EQ_OB:
            check_dsp_r2(ctx);
            gen_helper_cmpgdu_eq_ob(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_CMPGDU_LT_OB:
            check_dsp_r2(ctx);
            gen_helper_cmpgdu_lt_ob(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_CMPGDU_LE_OB:
            check_dsp_r2(ctx);
            gen_helper_cmpgdu_le_ob(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_CMPGU_EQ_OB:
            check_dsp(ctx);
            gen_helper_cmpgu_eq_ob(cpu_gpr[ret], v1_t, v2_t);
            break;
        case OPC_CMPGU_LT_OB:
            check_dsp(ctx);
            gen_helper_cmpgu_lt_ob(cpu_gpr[ret], v1_t, v2_t);
            break;
        case OPC_CMPGU_LE_OB:
            check_dsp(ctx);
            gen_helper_cmpgu_le_ob(cpu_gpr[ret], v1_t, v2_t);
            break;
        case OPC_CMPU_EQ_OB:
            check_dsp(ctx);
            gen_helper_cmpu_eq_ob(v1_t, v2_t, cpu_env);
            break;
        case OPC_CMPU_LT_OB:
            check_dsp(ctx);
            gen_helper_cmpu_lt_ob(v1_t, v2_t, cpu_env);
            break;
        case OPC_CMPU_LE_OB:
            check_dsp(ctx);
            gen_helper_cmpu_le_ob(v1_t, v2_t, cpu_env);
            break;
        case OPC_PACKRL_PW:
            check_dsp(ctx);
            gen_helper_packrl_pw(cpu_gpr[ret], v1_t, v2_t);
            break;
        case OPC_PICK_OB:
            check_dsp(ctx);
            gen_helper_pick_ob(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_PICK_PW:
            check_dsp(ctx);
            gen_helper_pick_pw(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        case OPC_PICK_QH:
            check_dsp(ctx);
            gen_helper_pick_qh(cpu_gpr[ret], v1_t, v2_t, cpu_env);
            break;
        }
        break;
#endif
    }

    tcg_temp_free(t1);
    tcg_temp_free(v1_t);
    tcg_temp_free(v2_t);
}

static void gen_mipsdsp_append(CPUMIPSState *env, DisasContext *ctx,
                               uint32_t op1, int rt, int rs, int sa)
{
    TCGv t0;

    check_dsp_r2(ctx);

    if (rt == 0) {
        /* Treat as NOP. */
        return;
    }

    t0 = tcg_temp_new();
    gen_load_gpr(t0, rs);

    switch (op1) {
    case OPC_APPEND_DSP:
        switch (MASK_APPEND(ctx->opcode)) {
        case OPC_APPEND:
            if (sa != 0) {
                tcg_gen_deposit_tl(cpu_gpr[rt], t0, cpu_gpr[rt], sa, 32 - sa);
            }
            tcg_gen_ext32s_tl(cpu_gpr[rt], cpu_gpr[rt]);
            break;
        case OPC_PREPEND:
            if (sa != 0) {
                tcg_gen_ext32u_tl(cpu_gpr[rt], cpu_gpr[rt]);
                tcg_gen_shri_tl(cpu_gpr[rt], cpu_gpr[rt], sa);
                tcg_gen_shli_tl(t0, t0, 32 - sa);
                tcg_gen_or_tl(cpu_gpr[rt], cpu_gpr[rt], t0);
            }
            tcg_gen_ext32s_tl(cpu_gpr[rt], cpu_gpr[rt]);
            break;
        case OPC_BALIGN:
            sa &= 3;
            if (sa != 0 && sa != 2) {
                tcg_gen_shli_tl(cpu_gpr[rt], cpu_gpr[rt], 8 * sa);
                tcg_gen_ext32u_tl(t0, t0);
                tcg_gen_shri_tl(t0, t0, 8 * (4 - sa));
                tcg_gen_or_tl(cpu_gpr[rt], cpu_gpr[rt], t0);
            }
            tcg_gen_ext32s_tl(cpu_gpr[rt], cpu_gpr[rt]);
            break;
        default:            /* Invalid */
            MIPS_INVAL("MASK APPEND");
            gen_reserved_instruction(ctx);
            break;
        }
        break;
#ifdef TARGET_MIPS64
    case OPC_DAPPEND_DSP:
        switch (MASK_DAPPEND(ctx->opcode)) {
        case OPC_DAPPEND:
            if (sa != 0) {
                tcg_gen_deposit_tl(cpu_gpr[rt], t0, cpu_gpr[rt], sa, 64 - sa);
            }
            break;
        case OPC_PREPENDD:
            tcg_gen_shri_tl(cpu_gpr[rt], cpu_gpr[rt], 0x20 | sa);
            tcg_gen_shli_tl(t0, t0, 64 - (0x20 | sa));
            tcg_gen_or_tl(cpu_gpr[rt], t0, t0);
            break;
        case OPC_PREPENDW:
            if (sa != 0) {
                tcg_gen_shri_tl(cpu_gpr[rt], cpu_gpr[rt], sa);
                tcg_gen_shli_tl(t0, t0, 64 - sa);
                tcg_gen_or_tl(cpu_gpr[rt], cpu_gpr[rt], t0);
            }
            break;
        case OPC_DBALIGN:
            sa &= 7;
            if (sa != 0 && sa != 2 && sa != 4) {
                tcg_gen_shli_tl(cpu_gpr[rt], cpu_gpr[rt], 8 * sa);
                tcg_gen_shri_tl(t0, t0, 8 * (8 - sa));
                tcg_gen_or_tl(cpu_gpr[rt], cpu_gpr[rt], t0);
            }
            break;
        default:            /* Invalid */
            MIPS_INVAL("MASK DAPPEND");
            gen_reserved_instruction(ctx);
            break;
        }
        break;
#endif
    }
    tcg_temp_free(t0);
}

static void gen_mipsdsp_accinsn(DisasContext *ctx, uint32_t op1, uint32_t op2,
                                int ret, int v1, int v2, int check_ret)

{
    TCGv t0;
    TCGv t1;
    TCGv v1_t;
    TCGv v2_t;
    int16_t imm;

    if ((ret == 0) && (check_ret == 1)) {
        /* Treat as NOP. */
        return;
    }

    t0 = tcg_temp_new();
    t1 = tcg_temp_new();
    v1_t = tcg_temp_new();
    v2_t = tcg_temp_new();

    gen_load_gpr(v1_t, v1);
    gen_load_gpr(v2_t, v2);

    switch (op1) {
    case OPC_EXTR_W_DSP:
        check_dsp(ctx);
        switch (op2) {
        case OPC_EXTR_W:
            tcg_gen_movi_tl(t0, v2);
            tcg_gen_movi_tl(t1, v1);
            gen_helper_extr_w(cpu_gpr[ret], t0, t1, cpu_env);
            break;
        case OPC_EXTR_R_W:
            tcg_gen_movi_tl(t0, v2);
            tcg_gen_movi_tl(t1, v1);
            gen_helper_extr_r_w(cpu_gpr[ret], t0, t1, cpu_env);
            break;
        case OPC_EXTR_RS_W:
            tcg_gen_movi_tl(t0, v2);
            tcg_gen_movi_tl(t1, v1);
            gen_helper_extr_rs_w(cpu_gpr[ret], t0, t1, cpu_env);
            break;
        case OPC_EXTR_S_H:
            tcg_gen_movi_tl(t0, v2);
            tcg_gen_movi_tl(t1, v1);
            gen_helper_extr_s_h(cpu_gpr[ret], t0, t1, cpu_env);
            break;
        case OPC_EXTRV_S_H:
            tcg_gen_movi_tl(t0, v2);
            gen_helper_extr_s_h(cpu_gpr[ret], t0, v1_t, cpu_env);
            break;
        case OPC_EXTRV_W:
            tcg_gen_movi_tl(t0, v2);
            gen_helper_extr_w(cpu_gpr[ret], t0, v1_t, cpu_env);
            break;
        case OPC_EXTRV_R_W:
            tcg_gen_movi_tl(t0, v2);
            gen_helper_extr_r_w(cpu_gpr[ret], t0, v1_t, cpu_env);
            break;
        case OPC_EXTRV_RS_W:
            tcg_gen_movi_tl(t0, v2);
            gen_helper_extr_rs_w(cpu_gpr[ret], t0, v1_t, cpu_env);
            break;
        case OPC_EXTP:
            tcg_gen_movi_tl(t0, v2);
            tcg_gen_movi_tl(t1, v1);
            gen_helper_extp(cpu_gpr[ret], t0, t1, cpu_env);
            break;
        case OPC_EXTPV:
            tcg_gen_movi_tl(t0, v2);
            gen_helper_extp(cpu_gpr[ret], t0, v1_t, cpu_env);
            break;
        case OPC_EXTPDP:
            tcg_gen_movi_tl(t0, v2);
            tcg_gen_movi_tl(t1, v1);
            gen_helper_extpdp(cpu_gpr[ret], t0, t1, cpu_env);
            break;
        case OPC_EXTPDPV:
            tcg_gen_movi_tl(t0, v2);
            gen_helper_extpdp(cpu_gpr[ret], t0, v1_t, cpu_env);
            break;
        case OPC_SHILO:
            imm = (ctx->opcode >> 20) & 0x3F;
            tcg_gen_movi_tl(t0, ret);
            tcg_gen_movi_tl(t1, imm);
            gen_helper_shilo(t0, t1, cpu_env);
            break;
        case OPC_SHILOV:
            tcg_gen_movi_tl(t0, ret);
            gen_helper_shilo(t0, v1_t, cpu_env);
            break;
        case OPC_MTHLIP:
            tcg_gen_movi_tl(t0, ret);
            gen_helper_mthlip(t0, v1_t, cpu_env);
            break;
        case OPC_WRDSP:
            imm = (ctx->opcode >> 11) & 0x3FF;
            tcg_gen_movi_tl(t0, imm);
            gen_helper_wrdsp(v1_t, t0, cpu_env);
            break;
        case OPC_RDDSP:
            imm = (ctx->opcode >> 16) & 0x03FF;
            tcg_gen_movi_tl(t0, imm);
            gen_helper_rddsp(cpu_gpr[ret], t0, cpu_env);
            break;
        }
        break;
#ifdef TARGET_MIPS64
    case OPC_DEXTR_W_DSP:
        check_dsp(ctx);
        switch (op2) {
        case OPC_DMTHLIP:
            tcg_gen_movi_tl(t0, ret);
            gen_helper_dmthlip(v1_t, t0, cpu_env);
            break;
        case OPC_DSHILO:
            {
                int shift = (ctx->opcode >> 19) & 0x7F;
                int ac = (ctx->opcode >> 11) & 0x03;
                tcg_gen_movi_tl(t0, shift);
                tcg_gen_movi_tl(t1, ac);
                gen_helper_dshilo(t0, t1, cpu_env);
                break;
            }
        case OPC_DSHILOV:
            {
                int ac = (ctx->opcode >> 11) & 0x03;
                tcg_gen_movi_tl(t0, ac);
                gen_helper_dshilo(v1_t, t0, cpu_env);
                break;
            }
        case OPC_DEXTP:
            tcg_gen_movi_tl(t0, v2);
            tcg_gen_movi_tl(t1, v1);

            gen_helper_dextp(cpu_gpr[ret], t0, t1, cpu_env);
            break;
        case OPC_DEXTPV:
            tcg_gen_movi_tl(t0, v2);
            gen_helper_dextp(cpu_gpr[ret], t0, v1_t, cpu_env);
            break;
        case OPC_DEXTPDP:
            tcg_gen_movi_tl(t0, v2);
            tcg_gen_movi_tl(t1, v1);
            gen_helper_dextpdp(cpu_gpr[ret], t0, t1, cpu_env);
            break;
        case OPC_DEXTPDPV:
            tcg_gen_movi_tl(t0, v2);
            gen_helper_dextpdp(cpu_gpr[ret], t0, v1_t, cpu_env);
            break;
        case OPC_DEXTR_L:
            tcg_gen_movi_tl(t0, v2);
            tcg_gen_movi_tl(t1, v1);
            gen_helper_dextr_l(cpu_gpr[ret], t0, t1, cpu_env);
            break;
        case OPC_DEXTR_R_L:
            tcg_gen_movi_tl(t0, v2);
            tcg_gen_movi_tl(t1, v1);
            gen_helper_dextr_r_l(cpu_gpr[ret], t0, t1, cpu_env);
            break;
        case OPC_DEXTR_RS_L:
            tcg_gen_movi_tl(t0, v2);
            tcg_gen_movi_tl(t1, v1);
            gen_helper_dextr_rs_l(cpu_gpr[ret], t0, t1, cpu_env);
            break;
        case OPC_DEXTR_W:
            tcg_gen_movi_tl(t0, v2);
            tcg_gen_movi_tl(t1, v1);
            gen_helper_dextr_w(cpu_gpr[ret], t0, t1, cpu_env);
            break;
        case OPC_DEXTR_R_W:
            tcg_gen_movi_tl(t0, v2);
            tcg_gen_movi_tl(t1, v1);
            gen_helper_dextr_r_w(cpu_gpr[ret], t0, t1, cpu_env);
            break;
        case OPC_DEXTR_RS_W:
            tcg_gen_movi_tl(t0, v2);
            tcg_gen_movi_tl(t1, v1);
            gen_helper_dextr_rs_w(cpu_gpr[ret], t0, t1, cpu_env);
            break;
        case OPC_DEXTR_S_H:
            tcg_gen_movi_tl(t0, v2);
            tcg_gen_movi_tl(t1, v1);
            gen_helper_dextr_s_h(cpu_gpr[ret], t0, t1, cpu_env);
            break;
        case OPC_DEXTRV_S_H:
            tcg_gen_movi_tl(t0, v2);
            tcg_gen_movi_tl(t1, v1);
            gen_helper_dextr_s_h(cpu_gpr[ret], t0, t1, cpu_env);
            break;
        case OPC_DEXTRV_L:
            tcg_gen_movi_tl(t0, v2);
            gen_helper_dextr_l(cpu_gpr[ret], t0, v1_t, cpu_env);
            break;
        case OPC_DEXTRV_R_L:
            tcg_gen_movi_tl(t0, v2);
            gen_helper_dextr_r_l(cpu_gpr[ret], t0, v1_t, cpu_env);
            break;
        case OPC_DEXTRV_RS_L:
            tcg_gen_movi_tl(t0, v2);
            gen_helper_dextr_rs_l(cpu_gpr[ret], t0, v1_t, cpu_env);
            break;
        case OPC_DEXTRV_W:
            tcg_gen_movi_tl(t0, v2);
            gen_helper_dextr_w(cpu_gpr[ret], t0, v1_t, cpu_env);
            break;
        case OPC_DEXTRV_R_W:
            tcg_gen_movi_tl(t0, v2);
            gen_helper_dextr_r_w(cpu_gpr[ret], t0, v1_t, cpu_env);
            break;
        case OPC_DEXTRV_RS_W:
            tcg_gen_movi_tl(t0, v2);
            gen_helper_dextr_rs_w(cpu_gpr[ret], t0, v1_t, cpu_env);
            break;
        }
        break;
#endif
    }

    tcg_temp_free(t0);
    tcg_temp_free(t1);
    tcg_temp_free(v1_t);
    tcg_temp_free(v2_t);
}

/* End MIPSDSP functions. */

static void decode_opc_special_r6(CPUMIPSState *env, DisasContext *ctx)
{
    int rs, rt, rd, sa;
    uint32_t op1, op2;

    rs = (ctx->opcode >> 21) & 0x1f;
    rt = (ctx->opcode >> 16) & 0x1f;
    rd = (ctx->opcode >> 11) & 0x1f;
    sa = (ctx->opcode >> 6) & 0x1f;

    op1 = MASK_SPECIAL(ctx->opcode);
    switch (op1) {
    case OPC_MULT:
    case OPC_MULTU:
    case OPC_DIV:
    case OPC_DIVU:
        op2 = MASK_R6_MULDIV(ctx->opcode);
        switch (op2) {
        case R6_OPC_MUL:
        case R6_OPC_MUH:
        case R6_OPC_MULU:
        case R6_OPC_MUHU:
        case R6_OPC_DIV:
        case R6_OPC_MOD:
        case R6_OPC_DIVU:
        case R6_OPC_MODU:
            gen_r6_muldiv(ctx, op2, rd, rs, rt);
            break;
        default:
            MIPS_INVAL("special_r6 muldiv");
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case OPC_SELEQZ:
    case OPC_SELNEZ:
        gen_cond_move(ctx, op1, rd, rs, rt);
        break;
    case R6_OPC_CLO:
    case R6_OPC_CLZ:
        if (rt == 0 && sa == 1) {
            /*
             * Major opcode and function field is shared with preR6 MFHI/MTHI.
             * We need additionally to check other fields.
             */
            gen_cl(ctx, op1, rd, rs);
        } else {
            gen_reserved_instruction(ctx);
        }
        break;
    case R6_OPC_SDBBP:
        if (is_uhi(extract32(ctx->opcode, 6, 20))) {
            gen_helper_do_semihosting(cpu_env);
        } else {
            if (ctx->hflags & MIPS_HFLAG_SBRI) {
                gen_reserved_instruction(ctx);
            } else {
                generate_exception_end(ctx, EXCP_DBp);
            }
        }
        break;
#if defined(TARGET_MIPS64)
    case R6_OPC_DCLO:
    case R6_OPC_DCLZ:
        if (rt == 0 && sa == 1) {
            /*
             * Major opcode and function field is shared with preR6 MFHI/MTHI.
             * We need additionally to check other fields.
             */
            check_mips_64(ctx);
            gen_cl(ctx, op1, rd, rs);
        } else {
            gen_reserved_instruction(ctx);
        }
        break;
    case OPC_DMULT:
    case OPC_DMULTU:
    case OPC_DDIV:
    case OPC_DDIVU:

        op2 = MASK_R6_MULDIV(ctx->opcode);
        switch (op2) {
        case R6_OPC_DMUL:
        case R6_OPC_DMUH:
        case R6_OPC_DMULU:
        case R6_OPC_DMUHU:
        case R6_OPC_DDIV:
        case R6_OPC_DMOD:
        case R6_OPC_DDIVU:
        case R6_OPC_DMODU:
            check_mips_64(ctx);
            gen_r6_muldiv(ctx, op2, rd, rs, rt);
            break;
        default:
            MIPS_INVAL("special_r6 muldiv");
            gen_reserved_instruction(ctx);
            break;
        }
        break;
#endif
    default:            /* Invalid */
        MIPS_INVAL("special_r6");
        gen_reserved_instruction(ctx);
        break;
    }
}

static void decode_opc_special_tx79(CPUMIPSState *env, DisasContext *ctx)
{
    int rs = extract32(ctx->opcode, 21, 5);
    int rt = extract32(ctx->opcode, 16, 5);
    int rd = extract32(ctx->opcode, 11, 5);
    uint32_t op1 = MASK_SPECIAL(ctx->opcode);

    switch (op1) {
    case OPC_MOVN:         /* Conditional move */
    case OPC_MOVZ:
        gen_cond_move(ctx, op1, rd, rs, rt);
        break;
    case OPC_MFHI:          /* Move from HI/LO */
    case OPC_MFLO:
        gen_HILO(ctx, op1, 0, rd);
        break;
    case OPC_MTHI:
    case OPC_MTLO:          /* Move to HI/LO */
        gen_HILO(ctx, op1, 0, rs);
        break;
    case OPC_MULT:
    case OPC_MULTU:
        gen_mul_txx9(ctx, op1, rd, rs, rt);
        break;
    case OPC_DIV:
    case OPC_DIVU:
        gen_muldiv(ctx, op1, 0, rs, rt);
        break;
#if defined(TARGET_MIPS64)
    case OPC_DMULT:
    case OPC_DMULTU:
    case OPC_DDIV:
    case OPC_DDIVU:
        check_insn_opc_user_only(ctx, INSN_R5900);
        gen_muldiv(ctx, op1, 0, rs, rt);
        break;
#endif
    case OPC_JR:
        gen_compute_branch(ctx, op1, 4, rs, 0, 0, 4);
        break;
    default:            /* Invalid */
        MIPS_INVAL("special_tx79");
        gen_reserved_instruction(ctx);
        break;
    }
}

static void decode_opc_special_legacy(CPUMIPSState *env, DisasContext *ctx)
{
    int rs, rt, rd, sa;
    uint32_t op1;

    rs = (ctx->opcode >> 21) & 0x1f;
    rt = (ctx->opcode >> 16) & 0x1f;
    rd = (ctx->opcode >> 11) & 0x1f;
    sa = (ctx->opcode >> 6) & 0x1f;

    op1 = MASK_SPECIAL(ctx->opcode);
    switch (op1) {
    case OPC_MOVN:         /* Conditional move */
    case OPC_MOVZ:
        check_insn(ctx, ISA_MIPS4 | ISA_MIPS_R1 |
                   INSN_LOONGSON2E | INSN_LOONGSON2F);
        gen_cond_move(ctx, op1, rd, rs, rt);
        break;
    case OPC_MFHI:          /* Move from HI/LO */
    case OPC_MFLO:
        gen_HILO(ctx, op1, rs & 3, rd);
        break;
    case OPC_MTHI:
    case OPC_MTLO:          /* Move to HI/LO */
        gen_HILO(ctx, op1, rd & 3, rs);
        break;
    case OPC_MOVCI:
        check_insn(ctx, ISA_MIPS4 | ISA_MIPS_R1);
        if (env->CP0_Config1 & (1 << CP0C1_FP)) {
            check_cp1_enabled(ctx);
            gen_movci(ctx, rd, rs, (ctx->opcode >> 18) & 0x7,
                      (ctx->opcode >> 16) & 1);
        } else {
            generate_exception_err(ctx, EXCP_CpU, 1);
        }
        break;
    case OPC_MULT:
    case OPC_MULTU:
        if (sa) {
            check_insn(ctx, INSN_VR54XX);
            op1 = MASK_MUL_VR54XX(ctx->opcode);
            gen_mul_vr54xx(ctx, op1, rd, rs, rt);
        } else {
            gen_muldiv(ctx, op1, rd & 3, rs, rt);
        }
        break;
    case OPC_DIV:
    case OPC_DIVU:
        gen_muldiv(ctx, op1, 0, rs, rt);
        break;
#if defined(TARGET_MIPS64)
    case OPC_DMULT:
    case OPC_DMULTU:
    case OPC_DDIV:
    case OPC_DDIVU:
        check_insn(ctx, ISA_MIPS3);
        check_mips_64(ctx);
        gen_muldiv(ctx, op1, 0, rs, rt);
        break;
#endif
    case OPC_JR:
        gen_compute_branch(ctx, op1, 4, rs, rd, sa, 4);
        break;
    case OPC_SPIM:
#ifdef MIPS_STRICT_STANDARD
        MIPS_INVAL("SPIM");
        gen_reserved_instruction(ctx);
#else
        /* Implemented as RI exception for now. */
        MIPS_INVAL("spim (unofficial)");
        gen_reserved_instruction(ctx);
#endif
        break;
    default:            /* Invalid */
        MIPS_INVAL("special_legacy");
        gen_reserved_instruction(ctx);
        break;
    }
}

static void decode_opc_special(CPUMIPSState *env, DisasContext *ctx)
{
    int rs, rt, rd, sa;
    uint32_t op1;

    rs = (ctx->opcode >> 21) & 0x1f;
    rt = (ctx->opcode >> 16) & 0x1f;
    rd = (ctx->opcode >> 11) & 0x1f;
    sa = (ctx->opcode >> 6) & 0x1f;

    op1 = MASK_SPECIAL(ctx->opcode);
    switch (op1) {
    case OPC_SLL:          /* Shift with immediate */
        if (sa == 5 && rd == 0 &&
            rs == 0 && rt == 0) { /* PAUSE */
            if ((ctx->insn_flags & ISA_MIPS_R6) &&
                (ctx->hflags & MIPS_HFLAG_BMASK)) {
                gen_reserved_instruction(ctx);
                break;
            }
        }
        /* Fallthrough */
    case OPC_SRA:
        gen_shift_imm(ctx, op1, rd, rt, sa);
        break;
    case OPC_SRL:
        switch ((ctx->opcode >> 21) & 0x1f) {
        case 1:
            /* rotr is decoded as srl on non-R2 CPUs */
            if (ctx->insn_flags & ISA_MIPS_R2) {
                op1 = OPC_ROTR;
            }
            /* Fallthrough */
        case 0:
            gen_shift_imm(ctx, op1, rd, rt, sa);
            break;
        default:
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case OPC_ADD:
    case OPC_ADDU:
    case OPC_SUB:
    case OPC_SUBU:
        gen_arith(ctx, op1, rd, rs, rt);
        break;
    case OPC_SLLV:         /* Shifts */
    case OPC_SRAV:
        gen_shift(ctx, op1, rd, rs, rt);
        break;
    case OPC_SRLV:
        switch ((ctx->opcode >> 6) & 0x1f) {
        case 1:
            /* rotrv is decoded as srlv on non-R2 CPUs */
            if (ctx->insn_flags & ISA_MIPS_R2) {
                op1 = OPC_ROTRV;
            }
            /* Fallthrough */
        case 0:
            gen_shift(ctx, op1, rd, rs, rt);
            break;
        default:
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case OPC_SLT:          /* Set on less than */
    case OPC_SLTU:
        gen_slt(ctx, op1, rd, rs, rt);
        break;
    case OPC_AND:          /* Logic*/
    case OPC_OR:
    case OPC_NOR:
    case OPC_XOR:
        gen_logic(ctx, op1, rd, rs, rt);
        break;
    case OPC_JALR:
        gen_compute_branch(ctx, op1, 4, rs, rd, sa, 4);
        break;
    case OPC_TGE: /* Traps */
    case OPC_TGEU:
    case OPC_TLT:
    case OPC_TLTU:
    case OPC_TEQ:
    case OPC_TNE:
        check_insn(ctx, ISA_MIPS2);
        gen_trap(ctx, op1, rs, rt, -1);
        break;
    case OPC_PMON:
        /* Pmon entry point, also R4010 selsl */
#ifdef MIPS_STRICT_STANDARD
        MIPS_INVAL("PMON / selsl");
        gen_reserved_instruction(ctx);
#else
        gen_helper_0e0i(pmon, sa);
#endif
        break;
    case OPC_SYSCALL:
        generate_exception_end(ctx, EXCP_SYSCALL);
        break;
    case OPC_BREAK:
        generate_exception_end(ctx, EXCP_BREAK);
        break;
    case OPC_SYNC:
        check_insn(ctx, ISA_MIPS2);
        gen_sync(extract32(ctx->opcode, 6, 5));
        break;

#if defined(TARGET_MIPS64)
        /* MIPS64 specific opcodes */
    case OPC_DSLL:
    case OPC_DSRA:
    case OPC_DSLL32:
    case OPC_DSRA32:
        check_insn(ctx, ISA_MIPS3);
        check_mips_64(ctx);
        gen_shift_imm(ctx, op1, rd, rt, sa);
        break;
    case OPC_DSRL:
        switch ((ctx->opcode >> 21) & 0x1f) {
        case 1:
            /* drotr is decoded as dsrl on non-R2 CPUs */
            if (ctx->insn_flags & ISA_MIPS_R2) {
                op1 = OPC_DROTR;
            }
            /* Fallthrough */
        case 0:
            check_insn(ctx, ISA_MIPS3);
            check_mips_64(ctx);
            gen_shift_imm(ctx, op1, rd, rt, sa);
            break;
        default:
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case OPC_DSRL32:
        switch ((ctx->opcode >> 21) & 0x1f) {
        case 1:
            /* drotr32 is decoded as dsrl32 on non-R2 CPUs */
            if (ctx->insn_flags & ISA_MIPS_R2) {
                op1 = OPC_DROTR32;
            }
            /* Fallthrough */
        case 0:
            check_insn(ctx, ISA_MIPS3);
            check_mips_64(ctx);
            gen_shift_imm(ctx, op1, rd, rt, sa);
            break;
        default:
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case OPC_DADD:
    case OPC_DADDU:
    case OPC_DSUB:
    case OPC_DSUBU:
        check_insn(ctx, ISA_MIPS3);
        check_mips_64(ctx);
        gen_arith(ctx, op1, rd, rs, rt);
        break;
    case OPC_DSLLV:
    case OPC_DSRAV:
        check_insn(ctx, ISA_MIPS3);
        check_mips_64(ctx);
        gen_shift(ctx, op1, rd, rs, rt);
        break;
    case OPC_DSRLV:
        switch ((ctx->opcode >> 6) & 0x1f) {
        case 1:
            /* drotrv is decoded as dsrlv on non-R2 CPUs */
            if (ctx->insn_flags & ISA_MIPS_R2) {
                op1 = OPC_DROTRV;
            }
            /* Fallthrough */
        case 0:
            check_insn(ctx, ISA_MIPS3);
            check_mips_64(ctx);
            gen_shift(ctx, op1, rd, rs, rt);
            break;
        default:
            gen_reserved_instruction(ctx);
            break;
        }
        break;
#endif
    default:
        if (ctx->insn_flags & ISA_MIPS_R6) {
            decode_opc_special_r6(env, ctx);
        } else if (ctx->insn_flags & INSN_R5900) {
            decode_opc_special_tx79(env, ctx);
        } else {
            decode_opc_special_legacy(env, ctx);
        }
    }
}


static void decode_opc_special2_legacy(CPUMIPSState *env, DisasContext *ctx)
{
    int rs, rt, rd;
    uint32_t op1;

    rs = (ctx->opcode >> 21) & 0x1f;
    rt = (ctx->opcode >> 16) & 0x1f;
    rd = (ctx->opcode >> 11) & 0x1f;

    op1 = MASK_SPECIAL2(ctx->opcode);
    switch (op1) {
    case OPC_MADD: /* Multiply and add/sub */
    case OPC_MADDU:
    case OPC_MSUB:
    case OPC_MSUBU:
        check_insn(ctx, ISA_MIPS_R1);
        gen_muldiv(ctx, op1, rd & 3, rs, rt);
        break;
    case OPC_MUL:
        gen_arith(ctx, op1, rd, rs, rt);
        break;
    case OPC_DIV_G_2F:
    case OPC_DIVU_G_2F:
    case OPC_MULT_G_2F:
    case OPC_MULTU_G_2F:
    case OPC_MOD_G_2F:
    case OPC_MODU_G_2F:
        check_insn(ctx, INSN_LOONGSON2F | ASE_LEXT);
        gen_loongson_integer(ctx, op1, rd, rs, rt);
        break;
    case OPC_CLO:
    case OPC_CLZ:
        check_insn(ctx, ISA_MIPS_R1);
        gen_cl(ctx, op1, rd, rs);
        break;
    case OPC_SDBBP:
        if (is_uhi(extract32(ctx->opcode, 6, 20))) {
            gen_helper_do_semihosting(cpu_env);
        } else {
            /*
             * XXX: not clear which exception should be raised
             *      when in debug mode...
             */
            check_insn(ctx, ISA_MIPS_R1);
            generate_exception_end(ctx, EXCP_DBp);
        }
        break;
#if defined(TARGET_MIPS64)
    case OPC_DCLO:
    case OPC_DCLZ:
        check_insn(ctx, ISA_MIPS_R1);
        check_mips_64(ctx);
        gen_cl(ctx, op1, rd, rs);
        break;
    case OPC_DMULT_G_2F:
    case OPC_DMULTU_G_2F:
    case OPC_DDIV_G_2F:
    case OPC_DDIVU_G_2F:
    case OPC_DMOD_G_2F:
    case OPC_DMODU_G_2F:
        check_insn(ctx, INSN_LOONGSON2F | ASE_LEXT);
        gen_loongson_integer(ctx, op1, rd, rs, rt);
        break;
#endif
    default:            /* Invalid */
        MIPS_INVAL("special2_legacy");
        gen_reserved_instruction(ctx);
        break;
    }
}

static void decode_opc_special3_r6(CPUMIPSState *env, DisasContext *ctx)
{
    int rs, rt, rd, sa;
    uint32_t op1, op2;
    int16_t imm;

    rs = (ctx->opcode >> 21) & 0x1f;
    rt = (ctx->opcode >> 16) & 0x1f;
    rd = (ctx->opcode >> 11) & 0x1f;
    sa = (ctx->opcode >> 6) & 0x1f;
    imm = (int16_t)ctx->opcode >> 7;

    op1 = MASK_SPECIAL3(ctx->opcode);
    switch (op1) {
    case R6_OPC_PREF:
        if (rt >= 24) {
            /* hint codes 24-31 are reserved and signal RI */
            gen_reserved_instruction(ctx);
        }
        /* Treat as NOP. */
        break;
    case R6_OPC_CACHE:
        check_cp0_enabled(ctx);
        if (ctx->hflags & MIPS_HFLAG_ITC_CACHE) {
            gen_cache_operation(ctx, rt, rs, imm);
        }
        break;
    case R6_OPC_SC:
        gen_st_cond(ctx, rt, rs, imm, MO_TESL, false, R6_OPC_SC);
        break;
    case R6_OPC_LL:
        gen_ld(ctx, op1, rt, rs, imm);
        break;
    case OPC_BSHFL:
        {
            if (rd == 0) {
                /* Treat as NOP. */
                break;
            }
            op2 = MASK_BSHFL(ctx->opcode);
            switch (op2) {
            case OPC_ALIGN:
            case OPC_ALIGN_1:
            case OPC_ALIGN_2:
            case OPC_ALIGN_3:
                gen_align(ctx, 32, rd, rs, rt, sa & 3);
                break;
            case OPC_BITSWAP:
                gen_bitswap(ctx, op2, rd, rt);
                break;
            }
        }
        break;
#ifndef CONFIG_USER_ONLY
    case OPC_GINV:
        if (unlikely(ctx->gi <= 1)) {
            gen_reserved_instruction(ctx);
        }
        check_cp0_enabled(ctx);
        switch ((ctx->opcode >> 6) & 3) {
        case 0:    /* GINVI */
            /* Treat as NOP. */
            break;
        case 2:    /* GINVT */
            gen_helper_0e1i(ginvt, cpu_gpr[rs], extract32(ctx->opcode, 8, 2));
            break;
        default:
            gen_reserved_instruction(ctx);
            break;
        }
        break;
#endif
#if defined(TARGET_MIPS64)
    case R6_OPC_SCD:
        gen_st_cond(ctx, rt, rs, imm, MO_TEQ, false, R6_OPC_SCD);
        break;
    case R6_OPC_LLD:
        gen_ld(ctx, op1, rt, rs, imm);
        break;
    case OPC_DBSHFL:
        check_mips_64(ctx);
        {
            if (rd == 0) {
                /* Treat as NOP. */
                break;
            }
            op2 = MASK_DBSHFL(ctx->opcode);
            switch (op2) {
            case OPC_DALIGN:
            case OPC_DALIGN_1:
            case OPC_DALIGN_2:
            case OPC_DALIGN_3:
            case OPC_DALIGN_4:
            case OPC_DALIGN_5:
            case OPC_DALIGN_6:
            case OPC_DALIGN_7:
                gen_align(ctx, 64, rd, rs, rt, sa & 7);
                break;
            case OPC_DBITSWAP:
                gen_bitswap(ctx, op2, rd, rt);
                break;
            }

        }
        break;
#endif
    default:            /* Invalid */
        MIPS_INVAL("special3_r6");
        gen_reserved_instruction(ctx);
        break;
    }
}

static void decode_opc_special3_legacy(CPUMIPSState *env, DisasContext *ctx)
{
    int rs, rt, rd;
    uint32_t op1, op2;

#ifdef CONFIG_TCG_LOG_INSTR
    if (unlikely(ctx->base.log_instr_enabled)) {
        warn_report("DSP instruction tracing is not implemented\n");
    }
#endif

    rs = (ctx->opcode >> 21) & 0x1f;
    rt = (ctx->opcode >> 16) & 0x1f;
    rd = (ctx->opcode >> 11) & 0x1f;

    op1 = MASK_SPECIAL3(ctx->opcode);
    switch (op1) {
    case OPC_DIV_G_2E:
    case OPC_DIVU_G_2E:
    case OPC_MOD_G_2E:
    case OPC_MODU_G_2E:
    case OPC_MULT_G_2E:
    case OPC_MULTU_G_2E:
        /*
         * OPC_MULT_G_2E, OPC_ADDUH_QB_DSP, OPC_MUL_PH_DSP have
         * the same mask and op1.
         */
        if ((ctx->insn_flags & ASE_DSP_R2) && (op1 == OPC_MULT_G_2E)) {
            op2 = MASK_ADDUH_QB(ctx->opcode);
            switch (op2) {
            case OPC_ADDUH_QB:
            case OPC_ADDUH_R_QB:
            case OPC_ADDQH_PH:
            case OPC_ADDQH_R_PH:
            case OPC_ADDQH_W:
            case OPC_ADDQH_R_W:
            case OPC_SUBUH_QB:
            case OPC_SUBUH_R_QB:
            case OPC_SUBQH_PH:
            case OPC_SUBQH_R_PH:
            case OPC_SUBQH_W:
            case OPC_SUBQH_R_W:
                gen_mipsdsp_arith(ctx, op1, op2, rd, rs, rt);
                break;
            case OPC_MUL_PH:
            case OPC_MUL_S_PH:
            case OPC_MULQ_S_W:
            case OPC_MULQ_RS_W:
                gen_mipsdsp_multiply(ctx, op1, op2, rd, rs, rt, 1);
                break;
            default:
                MIPS_INVAL("MASK ADDUH.QB");
                gen_reserved_instruction(ctx);
                break;
            }
        } else if (ctx->insn_flags & INSN_LOONGSON2E) {
            gen_loongson_integer(ctx, op1, rd, rs, rt);
        } else {
            gen_reserved_instruction(ctx);
        }
        break;
    case OPC_LX_DSP:
        op2 = MASK_LX(ctx->opcode);
        switch (op2) {
#if defined(TARGET_MIPS64)
        case OPC_LDX:
#endif
        case OPC_LBUX:
        case OPC_LHX:
        case OPC_LWX:
            gen_mipsdsp_ld(ctx, op2, rd, rs, rt);
            break;
        default:            /* Invalid */
            MIPS_INVAL("MASK LX");
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case OPC_ABSQ_S_PH_DSP:
        op2 = MASK_ABSQ_S_PH(ctx->opcode);
        switch (op2) {
        case OPC_ABSQ_S_QB:
        case OPC_ABSQ_S_PH:
        case OPC_ABSQ_S_W:
        case OPC_PRECEQ_W_PHL:
        case OPC_PRECEQ_W_PHR:
        case OPC_PRECEQU_PH_QBL:
        case OPC_PRECEQU_PH_QBR:
        case OPC_PRECEQU_PH_QBLA:
        case OPC_PRECEQU_PH_QBRA:
        case OPC_PRECEU_PH_QBL:
        case OPC_PRECEU_PH_QBR:
        case OPC_PRECEU_PH_QBLA:
        case OPC_PRECEU_PH_QBRA:
            gen_mipsdsp_arith(ctx, op1, op2, rd, rs, rt);
            break;
        case OPC_BITREV:
        case OPC_REPL_QB:
        case OPC_REPLV_QB:
        case OPC_REPL_PH:
        case OPC_REPLV_PH:
            gen_mipsdsp_bitinsn(ctx, op1, op2, rd, rt);
            break;
        default:
            MIPS_INVAL("MASK ABSQ_S.PH");
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case OPC_ADDU_QB_DSP:
        op2 = MASK_ADDU_QB(ctx->opcode);
        switch (op2) {
        case OPC_ADDQ_PH:
        case OPC_ADDQ_S_PH:
        case OPC_ADDQ_S_W:
        case OPC_ADDU_QB:
        case OPC_ADDU_S_QB:
        case OPC_ADDU_PH:
        case OPC_ADDU_S_PH:
        case OPC_SUBQ_PH:
        case OPC_SUBQ_S_PH:
        case OPC_SUBQ_S_W:
        case OPC_SUBU_QB:
        case OPC_SUBU_S_QB:
        case OPC_SUBU_PH:
        case OPC_SUBU_S_PH:
        case OPC_ADDSC:
        case OPC_ADDWC:
        case OPC_MODSUB:
        case OPC_RADDU_W_QB:
            gen_mipsdsp_arith(ctx, op1, op2, rd, rs, rt);
            break;
        case OPC_MULEU_S_PH_QBL:
        case OPC_MULEU_S_PH_QBR:
        case OPC_MULQ_RS_PH:
        case OPC_MULEQ_S_W_PHL:
        case OPC_MULEQ_S_W_PHR:
        case OPC_MULQ_S_PH:
            gen_mipsdsp_multiply(ctx, op1, op2, rd, rs, rt, 1);
            break;
        default:            /* Invalid */
            MIPS_INVAL("MASK ADDU.QB");
            gen_reserved_instruction(ctx);
            break;

        }
        break;
    case OPC_CMPU_EQ_QB_DSP:
        op2 = MASK_CMPU_EQ_QB(ctx->opcode);
        switch (op2) {
        case OPC_PRECR_SRA_PH_W:
        case OPC_PRECR_SRA_R_PH_W:
            gen_mipsdsp_arith(ctx, op1, op2, rt, rs, rd);
            break;
        case OPC_PRECR_QB_PH:
        case OPC_PRECRQ_QB_PH:
        case OPC_PRECRQ_PH_W:
        case OPC_PRECRQ_RS_PH_W:
        case OPC_PRECRQU_S_QB_PH:
            gen_mipsdsp_arith(ctx, op1, op2, rd, rs, rt);
            break;
        case OPC_CMPU_EQ_QB:
        case OPC_CMPU_LT_QB:
        case OPC_CMPU_LE_QB:
        case OPC_CMP_EQ_PH:
        case OPC_CMP_LT_PH:
        case OPC_CMP_LE_PH:
            gen_mipsdsp_add_cmp_pick(ctx, op1, op2, rd, rs, rt, 0);
            break;
        case OPC_CMPGU_EQ_QB:
        case OPC_CMPGU_LT_QB:
        case OPC_CMPGU_LE_QB:
        case OPC_CMPGDU_EQ_QB:
        case OPC_CMPGDU_LT_QB:
        case OPC_CMPGDU_LE_QB:
        case OPC_PICK_QB:
        case OPC_PICK_PH:
        case OPC_PACKRL_PH:
            gen_mipsdsp_add_cmp_pick(ctx, op1, op2, rd, rs, rt, 1);
            break;
        default:            /* Invalid */
            MIPS_INVAL("MASK CMPU.EQ.QB");
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case OPC_SHLL_QB_DSP:
        gen_mipsdsp_shift(ctx, op1, rd, rs, rt);
        break;
    case OPC_DPA_W_PH_DSP:
        op2 = MASK_DPA_W_PH(ctx->opcode);
        switch (op2) {
        case OPC_DPAU_H_QBL:
        case OPC_DPAU_H_QBR:
        case OPC_DPSU_H_QBL:
        case OPC_DPSU_H_QBR:
        case OPC_DPA_W_PH:
        case OPC_DPAX_W_PH:
        case OPC_DPAQ_S_W_PH:
        case OPC_DPAQX_S_W_PH:
        case OPC_DPAQX_SA_W_PH:
        case OPC_DPS_W_PH:
        case OPC_DPSX_W_PH:
        case OPC_DPSQ_S_W_PH:
        case OPC_DPSQX_S_W_PH:
        case OPC_DPSQX_SA_W_PH:
        case OPC_MULSAQ_S_W_PH:
        case OPC_DPAQ_SA_L_W:
        case OPC_DPSQ_SA_L_W:
        case OPC_MAQ_S_W_PHL:
        case OPC_MAQ_S_W_PHR:
        case OPC_MAQ_SA_W_PHL:
        case OPC_MAQ_SA_W_PHR:
        case OPC_MULSA_W_PH:
            gen_mipsdsp_multiply(ctx, op1, op2, rd, rs, rt, 0);
            break;
        default:            /* Invalid */
            MIPS_INVAL("MASK DPAW.PH");
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case OPC_INSV_DSP:
        op2 = MASK_INSV(ctx->opcode);
        switch (op2) {
        case OPC_INSV:
            check_dsp(ctx);
            {
                TCGv t0, t1;

                if (rt == 0) {
                    break;
                }

                t0 = tcg_temp_new();
                t1 = tcg_temp_new();

                gen_load_gpr(t0, rt);
                gen_load_gpr(t1, rs);

                gen_helper_insv(cpu_gpr[rt], cpu_env, t1, t0);

                tcg_temp_free(t0);
                tcg_temp_free(t1);
                break;
            }
        default:            /* Invalid */
            MIPS_INVAL("MASK INSV");
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case OPC_APPEND_DSP:
        gen_mipsdsp_append(env, ctx, op1, rt, rs, rd);
        break;
    case OPC_EXTR_W_DSP:
        op2 = MASK_EXTR_W(ctx->opcode);
        switch (op2) {
        case OPC_EXTR_W:
        case OPC_EXTR_R_W:
        case OPC_EXTR_RS_W:
        case OPC_EXTR_S_H:
        case OPC_EXTRV_S_H:
        case OPC_EXTRV_W:
        case OPC_EXTRV_R_W:
        case OPC_EXTRV_RS_W:
        case OPC_EXTP:
        case OPC_EXTPV:
        case OPC_EXTPDP:
        case OPC_EXTPDPV:
            gen_mipsdsp_accinsn(ctx, op1, op2, rt, rs, rd, 1);
            break;
        case OPC_RDDSP:
            gen_mipsdsp_accinsn(ctx, op1, op2, rd, rs, rt, 1);
            break;
        case OPC_SHILO:
        case OPC_SHILOV:
        case OPC_MTHLIP:
        case OPC_WRDSP:
            gen_mipsdsp_accinsn(ctx, op1, op2, rd, rs, rt, 0);
            break;
        default:            /* Invalid */
            MIPS_INVAL("MASK EXTR.W");
            gen_reserved_instruction(ctx);
            break;
        }
        break;
#if defined(TARGET_MIPS64)
    case OPC_DDIV_G_2E:
    case OPC_DDIVU_G_2E:
    case OPC_DMULT_G_2E:
    case OPC_DMULTU_G_2E:
    case OPC_DMOD_G_2E:
    case OPC_DMODU_G_2E:
        check_insn(ctx, INSN_LOONGSON2E);
        gen_loongson_integer(ctx, op1, rd, rs, rt);
        break;
    case OPC_ABSQ_S_QH_DSP:
        op2 = MASK_ABSQ_S_QH(ctx->opcode);
        switch (op2) {
        case OPC_PRECEQ_L_PWL:
        case OPC_PRECEQ_L_PWR:
        case OPC_PRECEQ_PW_QHL:
        case OPC_PRECEQ_PW_QHR:
        case OPC_PRECEQ_PW_QHLA:
        case OPC_PRECEQ_PW_QHRA:
        case OPC_PRECEQU_QH_OBL:
        case OPC_PRECEQU_QH_OBR:
        case OPC_PRECEQU_QH_OBLA:
        case OPC_PRECEQU_QH_OBRA:
        case OPC_PRECEU_QH_OBL:
        case OPC_PRECEU_QH_OBR:
        case OPC_PRECEU_QH_OBLA:
        case OPC_PRECEU_QH_OBRA:
        case OPC_ABSQ_S_OB:
        case OPC_ABSQ_S_PW:
        case OPC_ABSQ_S_QH:
            gen_mipsdsp_arith(ctx, op1, op2, rd, rs, rt);
            break;
        case OPC_REPL_OB:
        case OPC_REPL_PW:
        case OPC_REPL_QH:
        case OPC_REPLV_OB:
        case OPC_REPLV_PW:
        case OPC_REPLV_QH:
            gen_mipsdsp_bitinsn(ctx, op1, op2, rd, rt);
            break;
        default:            /* Invalid */
            MIPS_INVAL("MASK ABSQ_S.QH");
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case OPC_ADDU_OB_DSP:
        op2 = MASK_ADDU_OB(ctx->opcode);
        switch (op2) {
        case OPC_RADDU_L_OB:
        case OPC_SUBQ_PW:
        case OPC_SUBQ_S_PW:
        case OPC_SUBQ_QH:
        case OPC_SUBQ_S_QH:
        case OPC_SUBU_OB:
        case OPC_SUBU_S_OB:
        case OPC_SUBU_QH:
        case OPC_SUBU_S_QH:
        case OPC_SUBUH_OB:
        case OPC_SUBUH_R_OB:
        case OPC_ADDQ_PW:
        case OPC_ADDQ_S_PW:
        case OPC_ADDQ_QH:
        case OPC_ADDQ_S_QH:
        case OPC_ADDU_OB:
        case OPC_ADDU_S_OB:
        case OPC_ADDU_QH:
        case OPC_ADDU_S_QH:
        case OPC_ADDUH_OB:
        case OPC_ADDUH_R_OB:
            gen_mipsdsp_arith(ctx, op1, op2, rd, rs, rt);
            break;
        case OPC_MULEQ_S_PW_QHL:
        case OPC_MULEQ_S_PW_QHR:
        case OPC_MULEU_S_QH_OBL:
        case OPC_MULEU_S_QH_OBR:
        case OPC_MULQ_RS_QH:
            gen_mipsdsp_multiply(ctx, op1, op2, rd, rs, rt, 1);
            break;
        default:            /* Invalid */
            MIPS_INVAL("MASK ADDU.OB");
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case OPC_CMPU_EQ_OB_DSP:
        op2 = MASK_CMPU_EQ_OB(ctx->opcode);
        switch (op2) {
        case OPC_PRECR_SRA_QH_PW:
        case OPC_PRECR_SRA_R_QH_PW:
            /* Return value is rt. */
            gen_mipsdsp_arith(ctx, op1, op2, rt, rs, rd);
            break;
        case OPC_PRECR_OB_QH:
        case OPC_PRECRQ_OB_QH:
        case OPC_PRECRQ_PW_L:
        case OPC_PRECRQ_QH_PW:
        case OPC_PRECRQ_RS_QH_PW:
        case OPC_PRECRQU_S_OB_QH:
            gen_mipsdsp_arith(ctx, op1, op2, rd, rs, rt);
            break;
        case OPC_CMPU_EQ_OB:
        case OPC_CMPU_LT_OB:
        case OPC_CMPU_LE_OB:
        case OPC_CMP_EQ_QH:
        case OPC_CMP_LT_QH:
        case OPC_CMP_LE_QH:
        case OPC_CMP_EQ_PW:
        case OPC_CMP_LT_PW:
        case OPC_CMP_LE_PW:
            gen_mipsdsp_add_cmp_pick(ctx, op1, op2, rd, rs, rt, 0);
            break;
        case OPC_CMPGDU_EQ_OB:
        case OPC_CMPGDU_LT_OB:
        case OPC_CMPGDU_LE_OB:
        case OPC_CMPGU_EQ_OB:
        case OPC_CMPGU_LT_OB:
        case OPC_CMPGU_LE_OB:
        case OPC_PACKRL_PW:
        case OPC_PICK_OB:
        case OPC_PICK_PW:
        case OPC_PICK_QH:
            gen_mipsdsp_add_cmp_pick(ctx, op1, op2, rd, rs, rt, 1);
            break;
        default:            /* Invalid */
            MIPS_INVAL("MASK CMPU_EQ.OB");
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case OPC_DAPPEND_DSP:
        gen_mipsdsp_append(env, ctx, op1, rt, rs, rd);
        break;
    case OPC_DEXTR_W_DSP:
        op2 = MASK_DEXTR_W(ctx->opcode);
        switch (op2) {
        case OPC_DEXTP:
        case OPC_DEXTPDP:
        case OPC_DEXTPDPV:
        case OPC_DEXTPV:
        case OPC_DEXTR_L:
        case OPC_DEXTR_R_L:
        case OPC_DEXTR_RS_L:
        case OPC_DEXTR_W:
        case OPC_DEXTR_R_W:
        case OPC_DEXTR_RS_W:
        case OPC_DEXTR_S_H:
        case OPC_DEXTRV_L:
        case OPC_DEXTRV_R_L:
        case OPC_DEXTRV_RS_L:
        case OPC_DEXTRV_S_H:
        case OPC_DEXTRV_W:
        case OPC_DEXTRV_R_W:
        case OPC_DEXTRV_RS_W:
            gen_mipsdsp_accinsn(ctx, op1, op2, rt, rs, rd, 1);
            break;
        case OPC_DMTHLIP:
        case OPC_DSHILO:
        case OPC_DSHILOV:
            gen_mipsdsp_accinsn(ctx, op1, op2, rd, rs, rt, 0);
            break;
        default:            /* Invalid */
            MIPS_INVAL("MASK EXTR.W");
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case OPC_DPAQ_W_QH_DSP:
        op2 = MASK_DPAQ_W_QH(ctx->opcode);
        switch (op2) {
        case OPC_DPAU_H_OBL:
        case OPC_DPAU_H_OBR:
        case OPC_DPSU_H_OBL:
        case OPC_DPSU_H_OBR:
        case OPC_DPA_W_QH:
        case OPC_DPAQ_S_W_QH:
        case OPC_DPS_W_QH:
        case OPC_DPSQ_S_W_QH:
        case OPC_MULSAQ_S_W_QH:
        case OPC_DPAQ_SA_L_PW:
        case OPC_DPSQ_SA_L_PW:
        case OPC_MULSAQ_S_L_PW:
            gen_mipsdsp_multiply(ctx, op1, op2, rd, rs, rt, 0);
            break;
        case OPC_MAQ_S_W_QHLL:
        case OPC_MAQ_S_W_QHLR:
        case OPC_MAQ_S_W_QHRL:
        case OPC_MAQ_S_W_QHRR:
        case OPC_MAQ_SA_W_QHLL:
        case OPC_MAQ_SA_W_QHLR:
        case OPC_MAQ_SA_W_QHRL:
        case OPC_MAQ_SA_W_QHRR:
        case OPC_MAQ_S_L_PWL:
        case OPC_MAQ_S_L_PWR:
        case OPC_DMADD:
        case OPC_DMADDU:
        case OPC_DMSUB:
        case OPC_DMSUBU:
            gen_mipsdsp_multiply(ctx, op1, op2, rd, rs, rt, 0);
            break;
        default:            /* Invalid */
            MIPS_INVAL("MASK DPAQ.W.QH");
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case OPC_DINSV_DSP:
        op2 = MASK_INSV(ctx->opcode);
        switch (op2) {
        case OPC_DINSV:
        {
            TCGv t0, t1;

            if (rt == 0) {
                break;
            }
            check_dsp(ctx);

            t0 = tcg_temp_new();
            t1 = tcg_temp_new();

            gen_load_gpr(t0, rt);
            gen_load_gpr(t1, rs);

            gen_helper_dinsv(cpu_gpr[rt], cpu_env, t1, t0);

            tcg_temp_free(t0);
            tcg_temp_free(t1);
            break;
        }
        default:            /* Invalid */
            MIPS_INVAL("MASK DINSV");
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case OPC_SHLL_OB_DSP:
        gen_mipsdsp_shift(ctx, op1, rd, rs, rt);
        break;
#endif
    default:            /* Invalid */
        MIPS_INVAL("special3_legacy");
        gen_reserved_instruction(ctx);
        break;
    }
}


#if defined(TARGET_MIPS64)

static void decode_mmi(CPUMIPSState *env, DisasContext *ctx)
{
    uint32_t opc = MASK_MMI(ctx->opcode);
    int rs = extract32(ctx->opcode, 21, 5);
    int rt = extract32(ctx->opcode, 16, 5);
    int rd = extract32(ctx->opcode, 11, 5);

#ifdef CONFIG_TCG_LOG_INSTR
    if (unlikely(ctx->base.log_instr_enabled)) {
        warn_report("OPC_SPECIAL2 MMI instruction tracing is "
                    "not implemented\n");
    }
#endif

    switch (opc) {
    case MMI_OPC_MULT1:
    case MMI_OPC_MULTU1:
    case MMI_OPC_MADD:
    case MMI_OPC_MADDU:
    case MMI_OPC_MADD1:
    case MMI_OPC_MADDU1:
        gen_mul_txx9(ctx, opc, rd, rs, rt);
        break;
    case MMI_OPC_DIV1:
    case MMI_OPC_DIVU1:
        gen_div1_tx79(ctx, opc, rs, rt);
        break;
    default:
        MIPS_INVAL("TX79 MMI class");
        gen_reserved_instruction(ctx);
        break;
    }
}

#if !defined(TARGET_CHERI)
static void gen_mmi_lq(CPUMIPSState *env, DisasContext *ctx)
{
    gen_reserved_instruction(ctx);    /* TODO: MMI_OPC_LQ */
}
#endif

static void gen_mmi_sq(DisasContext *ctx, int base, int rt, int offset)
{
    gen_reserved_instruction(ctx);    /* TODO: MMI_OPC_SQ */
}

/*
 * The TX79-specific instruction Store Quadword
 *
 * +--------+-------+-------+------------------------+
 * | 011111 |  base |   rt  |           offset       | SQ
 * +--------+-------+-------+------------------------+
 *      6       5       5                 16
 *
 * has the same opcode as the Read Hardware Register instruction
 *
 * +--------+-------+-------+-------+-------+--------+
 * | 011111 | 00000 |   rt  |   rd  | 00000 | 111011 | RDHWR
 * +--------+-------+-------+-------+-------+--------+
 *      6       5       5       5       5        6
 *
 * that is required, trapped and emulated by the Linux kernel. However, all
 * RDHWR encodings yield address error exceptions on the TX79 since the SQ
 * offset is odd. Therefore all valid SQ instructions can execute normally.
 * In user mode, QEMU must verify the upper and lower 11 bits to distinguish
 * between SQ and RDHWR, as the Linux kernel does.
 */
static void decode_mmi_sq(CPUMIPSState *env, DisasContext *ctx)
{
    int base = extract32(ctx->opcode, 21, 5);
    int rt = extract32(ctx->opcode, 16, 5);
    int offset = extract32(ctx->opcode, 0, 16);

#ifdef CONFIG_USER_ONLY
    uint32_t op1 = MASK_SPECIAL3(ctx->opcode);
    uint32_t op2 = extract32(ctx->opcode, 6, 5);

    if (base == 0 && op2 == 0 && op1 == OPC_RDHWR) {
        int rd = extract32(ctx->opcode, 11, 5);

        gen_rdhwr(ctx, rt, rd, 0);
        return;
    }
#endif

    gen_mmi_sq(ctx, base, rt, offset);
}

#endif

static void decode_opc_special3(CPUMIPSState *env, DisasContext *ctx)
{
    int rs, rt, rd, sa;
    uint32_t op1, op2;
    int16_t imm;

    rs = (ctx->opcode >> 21) & 0x1f;
    rt = (ctx->opcode >> 16) & 0x1f;
    rd = (ctx->opcode >> 11) & 0x1f;
    sa = (ctx->opcode >> 6) & 0x1f;
    imm = sextract32(ctx->opcode, 7, 9);

    op1 = MASK_SPECIAL3(ctx->opcode);

    /*
     * EVA loads and stores overlap Loongson 2E instructions decoded by
     * decode_opc_special3_legacy(), so be careful to allow their decoding when
     * EVA is absent.
     */
    if (ctx->eva) {
        switch (op1) {
        case OPC_LWLE:
        case OPC_LWRE:
        case OPC_LBUE:
        case OPC_LHUE:
        case OPC_LBE:
        case OPC_LHE:
        case OPC_LLE:
        case OPC_LWE:
            check_cp0_enabled(ctx);
            gen_ld(ctx, op1, rt, rs, imm);
            return;
        case OPC_SWLE:
        case OPC_SWRE:
        case OPC_SBE:
        case OPC_SHE:
        case OPC_SWE:
            check_cp0_enabled(ctx);
            gen_st(ctx, op1, rt, rs, imm);
            return;
        case OPC_SCE:
            check_cp0_enabled(ctx);
            gen_st_cond(ctx, rt, rs, imm, MO_TESL, true, OPC_SCE);
            return;
        case OPC_CACHEE:
            check_cp0_enabled(ctx);
            if (ctx->hflags & MIPS_HFLAG_ITC_CACHE) {
                gen_cache_operation(ctx, rt, rs, imm);
            }
            /* Treat as NOP. */
            return;
        case OPC_PREFE:
            check_cp0_enabled(ctx);
            /* Treat as NOP. */
            return;
        }
    }

    switch (op1) {
    case OPC_EXT:
    case OPC_INS:
        check_insn(ctx, ISA_MIPS_R2);
        gen_bitops(ctx, op1, rt, rs, sa, rd);
        break;
    case OPC_BSHFL:
        op2 = MASK_BSHFL(ctx->opcode);
        switch (op2) {
        case OPC_ALIGN:
        case OPC_ALIGN_1:
        case OPC_ALIGN_2:
        case OPC_ALIGN_3:
        case OPC_BITSWAP:
            check_insn(ctx, ISA_MIPS_R6);
            decode_opc_special3_r6(env, ctx);
            break;
        default:
            check_insn(ctx, ISA_MIPS_R2);
            gen_bshfl(ctx, op2, rt, rd);
            break;
        }
        break;
#if defined(TARGET_MIPS64)
    case OPC_DEXTM:
    case OPC_DEXTU:
    case OPC_DEXT:
    case OPC_DINSM:
    case OPC_DINSU:
    case OPC_DINS:
        check_insn(ctx, ISA_MIPS_R2);
        check_mips_64(ctx);
        gen_bitops(ctx, op1, rt, rs, sa, rd);
        break;
    case OPC_DBSHFL:
        op2 = MASK_DBSHFL(ctx->opcode);
        switch (op2) {
        case OPC_DALIGN:
        case OPC_DALIGN_1:
        case OPC_DALIGN_2:
        case OPC_DALIGN_3:
        case OPC_DALIGN_4:
        case OPC_DALIGN_5:
        case OPC_DALIGN_6:
        case OPC_DALIGN_7:
        case OPC_DBITSWAP:
            check_insn(ctx, ISA_MIPS_R6);
            decode_opc_special3_r6(env, ctx);
            break;
        default:
            check_insn(ctx, ISA_MIPS_R2);
            check_mips_64(ctx);
            op2 = MASK_DBSHFL(ctx->opcode);
            gen_bshfl(ctx, op2, rt, rd);
            break;
        }
        break;
#endif
    case OPC_RDHWR:
#ifdef TARGET_CHERI
        // For CHERI/BERI statcounters we need a 4 bit selector instead of 3
        gen_rdhwr(ctx, rt, rd, extract32(ctx->opcode, 6, 4));
#else
        gen_rdhwr(ctx, rt, rd, extract32(ctx->opcode, 6, 3));
#endif
        break;
    case OPC_FORK:
        check_mt(ctx);
        {
            TCGv t0 = tcg_temp_new();
            TCGv t1 = tcg_temp_new();

            gen_load_gpr(t0, rt);
            gen_load_gpr(t1, rs);
            gen_helper_fork(t0, t1);
            tcg_temp_free(t0);
            tcg_temp_free(t1);
        }
        break;
    case OPC_YIELD:
        check_mt(ctx);
        {
            TCGv t0 = tcg_temp_new();

            gen_load_gpr(t0, rs);
            gen_helper_yield(t0, cpu_env, t0);
            gen_store_gpr(t0, rd);
            tcg_temp_free(t0);
        }
        break;
    default:
        if (ctx->insn_flags & ISA_MIPS_R6) {
            decode_opc_special3_r6(env, ctx);
        } else {
            decode_opc_special3_legacy(env, ctx);
        }
    }
}

static bool decode_opc_legacy(CPUMIPSState *env, DisasContext *ctx)
{
    int32_t offset;
    int rs, rt, rd, sa;
    uint32_t op, op1;
    int16_t imm;

    op = MASK_OP_MAJOR(ctx->opcode);
    rs = (ctx->opcode >> 21) & 0x1f;
    rt = (ctx->opcode >> 16) & 0x1f;
    rd = (ctx->opcode >> 11) & 0x1f;
    sa = (ctx->opcode >> 6) & 0x1f;
    imm = (int16_t)ctx->opcode;
    switch (op) {
    case OPC_SPECIAL:
        decode_opc_special(env, ctx);
        break;
    case OPC_SPECIAL2:
#if defined(TARGET_MIPS64)
        if ((ctx->insn_flags & INSN_R5900) && (ctx->insn_flags & ASE_MMI)) {
            decode_mmi(env, ctx);
            break;
        }
#endif
        if (TARGET_LONG_BITS == 32 && (ctx->insn_flags & ASE_MXU)) {
            if (MASK_SPECIAL2(ctx->opcode) == OPC_MUL) {
                gen_arith(ctx, OPC_MUL, rd, rs, rt);
            } else {
                decode_ase_mxu(ctx, ctx->opcode);
            }
            break;
        }
        decode_opc_special2_legacy(env, ctx);
        break;
    case OPC_SPECIAL3:
#ifdef CONFIG_TCG_LOG_INSTR
    if (unlikely(ctx->base.log_instr_enabled)) {
        warn_report("OPC_SPECIAL3 (mmi) instruction tracing is "
                    "not implemented\n");
    }
#endif
#if defined(TARGET_MIPS64)
        if (ctx->insn_flags & INSN_R5900) {
            decode_mmi_sq(env, ctx);    /* MMI_OPC_SQ */
        } else {
            decode_opc_special3(env, ctx);
        }
#else
        decode_opc_special3(env, ctx);
#endif
        break;
    case OPC_REGIMM:
        op1 = MASK_REGIMM(ctx->opcode);
        switch (op1) {
        case OPC_BLTZL: /* REGIMM branches */
        case OPC_BGEZL:
        case OPC_BLTZALL:
        case OPC_BGEZALL:
            check_insn(ctx, ISA_MIPS2);
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            /* Fallthrough */
        case OPC_BLTZ:
        case OPC_BGEZ:
            gen_compute_branch(ctx, op1, 4, rs, -1, imm << 2, 4);
            break;
        case OPC_BLTZAL:
        case OPC_BGEZAL:
            if (ctx->insn_flags & ISA_MIPS_R6) {
                if (rs == 0) {
                    /* OPC_NAL, OPC_BAL */
                    gen_compute_branch(ctx, op1, 4, 0, -1, imm << 2, 4);
                } else {
                    gen_reserved_instruction(ctx);
                }
            } else {
                gen_compute_branch(ctx, op1, 4, rs, -1, imm << 2, 4);
            }
            break;
        case OPC_TGEI: /* REGIMM traps */
        case OPC_TGEIU:
        case OPC_TLTI:
        case OPC_TLTIU:
        case OPC_TEQI:

        case OPC_TNEI:
            check_insn(ctx, ISA_MIPS2);
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            gen_trap(ctx, op1, rs, -1, imm);
            break;
        case OPC_SIGRIE:
            check_insn(ctx, ISA_MIPS_R6);
            gen_reserved_instruction(ctx);
            break;
        case OPC_SYNCI:
            check_insn(ctx, ISA_MIPS_R2);
            /*
             * Break the TB to be able to sync copied instructions
             * immediately.
             */
            ctx->base.is_jmp = DISAS_STOP;
            break;
        case OPC_BPOSGE32:    /* MIPS DSP branch */
#if defined(TARGET_MIPS64)
        case OPC_BPOSGE64:
#endif
            check_dsp(ctx);
            gen_compute_branch(ctx, op1, 4, -1, -2, (int32_t)imm << 2, 4);
            break;
#if defined(TARGET_MIPS64)
        case OPC_DAHI:
            check_insn(ctx, ISA_MIPS_R6);
            check_mips_64(ctx);
            if (rs != 0) {
                tcg_gen_addi_tl(cpu_gpr[rs], cpu_gpr[rs], (int64_t)imm << 32);
                gen_log_instr_gpr_update(ctx, rs);
            }
            break;
        case OPC_DATI:
            check_insn(ctx, ISA_MIPS_R6);
            check_mips_64(ctx);
            if (rs != 0) {
                tcg_gen_addi_tl(cpu_gpr[rs], cpu_gpr[rs], (int64_t)imm << 48);
                gen_log_instr_gpr_update(ctx, rs);
            }
            break;
#endif
        default:            /* Invalid */
            MIPS_INVAL("regimm");
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case OPC_CP0:
        check_cp0_enabled(ctx);
        op1 = MASK_CP0(ctx->opcode);
        switch (op1) {
        case OPC_MFC0:
        case OPC_MTC0:
        case OPC_MFTR:
        case OPC_MTTR:
        case OPC_MFHC0:
        case OPC_MTHC0:
#if defined(TARGET_MIPS64)
        case OPC_DMFC0:
        case OPC_DMTC0:
#endif
#ifndef CONFIG_USER_ONLY
            gen_cp0(env, ctx, op1, rt, rd);
#endif /* !CONFIG_USER_ONLY */
            break;
        case OPC_C0:
        case OPC_C0_1:
        case OPC_C0_2:
        case OPC_C0_3:
        case OPC_C0_4:
        case OPC_C0_5:
        case OPC_C0_6:
        case OPC_C0_7:
        case OPC_C0_8:
        case OPC_C0_9:
        case OPC_C0_A:
        case OPC_C0_B:
        case OPC_C0_C:
        case OPC_C0_D:
        case OPC_C0_E:
        case OPC_C0_F:
#ifndef CONFIG_USER_ONLY
            gen_cp0(env, ctx, MASK_C0(ctx->opcode), rt, rd);
#endif /* !CONFIG_USER_ONLY */
            break;
        case OPC_MFMC0:
#ifndef CONFIG_USER_ONLY
            {
                uint32_t op2;
                TCGv t0 = tcg_temp_new();

                op2 = MASK_MFMC0(ctx->opcode);
                switch (op2) {
                case OPC_DMT:
                    check_cp0_mt(ctx);
                    gen_helper_dmt(t0);
                    gen_store_gpr(t0, rt);
                    break;
                case OPC_EMT:
                    check_cp0_mt(ctx);
                    gen_helper_emt(t0);
                    gen_store_gpr(t0, rt);
                    break;
                case OPC_DVPE:
                    check_cp0_mt(ctx);
                    gen_helper_dvpe(t0, cpu_env);
                    gen_store_gpr(t0, rt);
                    break;
                case OPC_EVPE:
                    check_cp0_mt(ctx);
                    gen_helper_evpe(t0, cpu_env);
                    gen_store_gpr(t0, rt);
                    break;
                case OPC_DVP:
                    check_insn(ctx, ISA_MIPS_R6);
                    if (ctx->vp) {
                        gen_helper_dvp(t0, cpu_env);
                        gen_store_gpr(t0, rt);
                    }
                    break;
                case OPC_EVP:
                    check_insn(ctx, ISA_MIPS_R6);
                    if (ctx->vp) {
                        gen_helper_evp(t0, cpu_env);
                        gen_store_gpr(t0, rt);
                    }
                    break;
                case OPC_DI:
                    check_insn(ctx, ISA_MIPS_R2);
                    save_cpu_state(ctx, 1);
                    gen_helper_di(t0, cpu_env);
                    gen_store_gpr(t0, rt);
                    /*
                     * Stop translation as we may have switched
                     * the execution mode.
                     */
                    ctx->base.is_jmp = DISAS_STOP;
                    break;
                case OPC_EI:
                    check_insn(ctx, ISA_MIPS_R2);
                    save_cpu_state(ctx, 1);
                    gen_helper_ei(t0, cpu_env);
                    gen_store_gpr(t0, rt);
                    /*
                     * DISAS_STOP isn't sufficient, we need to ensure we break
                     * out of translated code to check for pending interrupts.
                     */
                    gen_save_pc(ctx->base.pc_next + 4);
                    ctx->base.is_jmp = DISAS_EXIT;
                    break;
                default:            /* Invalid */
                    MIPS_INVAL("mfmc0");
                    gen_reserved_instruction(ctx);
                    break;
                }
                tcg_temp_free(t0);
            }
#endif /* !CONFIG_USER_ONLY */
            break;
        case OPC_RDPGPR:
            check_insn(ctx, ISA_MIPS_R2);
            gen_load_srsgpr(ctx, rt, rd);
            break;
        case OPC_WRPGPR:
            check_insn(ctx, ISA_MIPS_R2);
            gen_store_srsgpr(rt, rd);
            break;
        default:
            MIPS_INVAL("cp0");
            gen_reserved_instruction(ctx);
            break;
        }
        break;
    case OPC_BOVC: /* OPC_BEQZALC, OPC_BEQC, OPC_ADDI */
        if (ctx->insn_flags & ISA_MIPS_R6) {
            /* OPC_BOVC, OPC_BEQZALC, OPC_BEQC */
            gen_compute_compact_branch(ctx, op, rs, rt, imm << 2);
        } else {
            /* OPC_ADDI */
            /* Arithmetic with immediate opcode */
            gen_arith_imm(ctx, op, rt, rs, imm);
        }
        break;
    case OPC_ADDIU:
         gen_arith_imm(ctx, op, rt, rs, imm);
         break;
    case OPC_SLTI: /* Set on less than with immediate opcode */
    case OPC_SLTIU:
         gen_slt_imm(ctx, op, rt, rs, imm);
         break;
    case OPC_ANDI: /* Arithmetic with immediate opcode */
    case OPC_LUI: /* OPC_AUI */
    case OPC_ORI:
    case OPC_XORI:
         gen_logic_imm(ctx, op, rt, rs, imm);
         break;
    case OPC_J: /* Jump */
    case OPC_JAL:
         offset = (int32_t)(ctx->opcode & 0x3FFFFFF) << 2;
         gen_compute_branch(ctx, op, 4, rs, rt, offset, 4);
         break;
    /* Branch */
    case OPC_BLEZC: /* OPC_BGEZC, OPC_BGEC, OPC_BLEZL */
        if (ctx->insn_flags & ISA_MIPS_R6) {
            if (rt == 0) {
                gen_reserved_instruction(ctx);
                break;
            }
            /* OPC_BLEZC, OPC_BGEZC, OPC_BGEC */
            gen_compute_compact_branch(ctx, op, rs, rt, imm << 2);
        } else {
            /* OPC_BLEZL */
            gen_compute_branch(ctx, op, 4, rs, rt, imm << 2, 4);
        }
        break;
    case OPC_BGTZC: /* OPC_BLTZC, OPC_BLTC, OPC_BGTZL */
        if (ctx->insn_flags & ISA_MIPS_R6) {
            if (rt == 0) {
                gen_reserved_instruction(ctx);
                break;
            }
            /* OPC_BGTZC, OPC_BLTZC, OPC_BLTC */
            gen_compute_compact_branch(ctx, op, rs, rt, imm << 2);
        } else {
            /* OPC_BGTZL */
            gen_compute_branch(ctx, op, 4, rs, rt, imm << 2, 4);
        }
        break;
    case OPC_BLEZALC: /* OPC_BGEZALC, OPC_BGEUC, OPC_BLEZ */
        if (rt == 0) {
            /* OPC_BLEZ */
            gen_compute_branch(ctx, op, 4, rs, rt, imm << 2, 4);
        } else {
            check_insn(ctx, ISA_MIPS_R6);
            /* OPC_BLEZALC, OPC_BGEZALC, OPC_BGEUC */
            gen_compute_compact_branch(ctx, op, rs, rt, imm << 2);
        }
        break;
    case OPC_BGTZALC: /* OPC_BLTZALC, OPC_BLTUC, OPC_BGTZ */
        if (rt == 0) {
            /* OPC_BGTZ */
            gen_compute_branch(ctx, op, 4, rs, rt, imm << 2, 4);
        } else {
            check_insn(ctx, ISA_MIPS_R6);
            /* OPC_BGTZALC, OPC_BLTZALC, OPC_BLTUC */
            gen_compute_compact_branch(ctx, op, rs, rt, imm << 2);
        }
        break;
    case OPC_BEQL:
    case OPC_BNEL:
        check_insn(ctx, ISA_MIPS2);
         check_insn_opc_removed(ctx, ISA_MIPS_R6);
        /* Fallthrough */
    case OPC_BEQ:
    case OPC_BNE:
         gen_compute_branch(ctx, op, 4, rs, rt, imm << 2, 4);
         break;
    case OPC_LL: /* Load and stores */
        check_insn(ctx, ISA_MIPS2);
        if (ctx->insn_flags & INSN_R5900) {
            check_insn_opc_user_only(ctx, INSN_R5900);
        }
        /* Fallthrough */
    case OPC_LWL:
    case OPC_LWR:
    case OPC_LB:
    case OPC_LH:
    case OPC_LW:
    case OPC_LWPC:
    case OPC_LBU:
    case OPC_LHU:
         gen_ld(ctx, op, rt, rs, imm);
         break;
    case OPC_SWL:
    case OPC_SWR:
    case OPC_SB:
    case OPC_SH:
    case OPC_SW:
         gen_st(ctx, op, rt, rs, imm);
         break;
    case OPC_SC:
        check_insn(ctx, ISA_MIPS2);
        if (ctx->insn_flags & INSN_R5900) {
            check_insn_opc_user_only(ctx, INSN_R5900);
        }
        gen_st_cond(ctx, rt, rs, imm, MO_TESL, false, OPC_SC);
        break;
    case OPC_CACHE:
        check_cp0_enabled(ctx);
        check_insn(ctx, ISA_MIPS3 | ISA_MIPS_R1);
        if (ctx->hflags & MIPS_HFLAG_ITC_CACHE) {
            gen_cache_operation(ctx, rt, rs, imm);
        }
        /* Treat as NOP. */
        break;
    case OPC_PREF:
        if (ctx->insn_flags & INSN_R5900) {
            /* Treat as NOP. */
        } else {
            check_insn(ctx, ISA_MIPS4 | ISA_MIPS_R1);
            /* Treat as NOP. */
        }
        break;

    /* Floating point (COP1). */
    case OPC_LWC1:
    case OPC_LDC1:
    case OPC_SWC1:
    case OPC_SDC1:
        gen_cop1_ldst(ctx, op, rt, rs, imm);
        break;

    case OPC_CP1:
        op1 = MASK_CP1(ctx->opcode);

        switch (op1) {
        case OPC_MFHC1:
        case OPC_MTHC1:
            check_cp1_enabled(ctx);
            check_insn(ctx, ISA_MIPS_R2);
            /* fall through */
        case OPC_MFC1:
        case OPC_CFC1:
        case OPC_MTC1:
        case OPC_CTC1:
            check_cp1_enabled(ctx);
            gen_cp1(ctx, op1, rt, rd);
            break;
#if defined(TARGET_MIPS64)
        case OPC_DMFC1:
        case OPC_DMTC1:
            check_cp1_enabled(ctx);
            check_insn(ctx, ISA_MIPS3);
            check_mips_64(ctx);
            gen_cp1(ctx, op1, rt, rd);
            break;
#endif
        case OPC_BC1EQZ: /* OPC_BC1ANY2 */
            check_cp1_enabled(ctx);
            if (ctx->insn_flags & ISA_MIPS_R6) {
                /* OPC_BC1EQZ */
                gen_compute_branch1_r6(ctx, MASK_CP1(ctx->opcode),
                                       rt, imm << 2, 4);
            } else {
                /* OPC_BC1ANY2 */
                check_cop1x(ctx);
                check_insn(ctx, ASE_MIPS3D);
                gen_compute_branch1(ctx, MASK_BC1(ctx->opcode),
                                    (rt >> 2) & 0x7, imm << 2);
            }
            break;
        case OPC_BC1NEZ:
            check_cp1_enabled(ctx);
            check_insn(ctx, ISA_MIPS_R6);
            gen_compute_branch1_r6(ctx, MASK_CP1(ctx->opcode),
                                   rt, imm << 2, 4);
            break;
        case OPC_BC1ANY4:
            check_cp1_enabled(ctx);
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            check_cop1x(ctx);
            check_insn(ctx, ASE_MIPS3D);
            /* fall through */
        case OPC_BC1:
            check_cp1_enabled(ctx);
            check_insn_opc_removed(ctx, ISA_MIPS_R6);
            gen_compute_branch1(ctx, MASK_BC1(ctx->opcode),
                                (rt >> 2) & 0x7, imm << 2);
            break;
        case OPC_PS_FMT:
            check_ps(ctx);
            /* fall through */
        case OPC_S_FMT:
        case OPC_D_FMT:
            check_cp1_enabled(ctx);
            gen_farith(ctx, ctx->opcode & FOP(0x3f, 0x1f), rt, rd, sa,
                       (imm >> 8) & 0x7);
            break;
        case OPC_W_FMT:
        case OPC_L_FMT:
        {
            int r6_op = ctx->opcode & FOP(0x3f, 0x1f);
            check_cp1_enabled(ctx);
            if (ctx->insn_flags & ISA_MIPS_R6) {
                switch (r6_op) {
                case R6_OPC_CMP_AF_S:
                case R6_OPC_CMP_UN_S:
                case R6_OPC_CMP_EQ_S:
                case R6_OPC_CMP_UEQ_S:
                case R6_OPC_CMP_LT_S:
                case R6_OPC_CMP_ULT_S:
                case R6_OPC_CMP_LE_S:
                case R6_OPC_CMP_ULE_S:
                case R6_OPC_CMP_SAF_S:
                case R6_OPC_CMP_SUN_S:
                case R6_OPC_CMP_SEQ_S:
                case R6_OPC_CMP_SEUQ_S:
                case R6_OPC_CMP_SLT_S:
                case R6_OPC_CMP_SULT_S:
                case R6_OPC_CMP_SLE_S:
                case R6_OPC_CMP_SULE_S:
                case R6_OPC_CMP_OR_S:
                case R6_OPC_CMP_UNE_S:
                case R6_OPC_CMP_NE_S:
                case R6_OPC_CMP_SOR_S:
                case R6_OPC_CMP_SUNE_S:
                case R6_OPC_CMP_SNE_S:
                    gen_r6_cmp_s(ctx, ctx->opcode & 0x1f, rt, rd, sa);
                    break;
                case R6_OPC_CMP_AF_D:
                case R6_OPC_CMP_UN_D:
                case R6_OPC_CMP_EQ_D:
                case R6_OPC_CMP_UEQ_D:
                case R6_OPC_CMP_LT_D:
                case R6_OPC_CMP_ULT_D:
                case R6_OPC_CMP_LE_D:
                case R6_OPC_CMP_ULE_D:
                case R6_OPC_CMP_SAF_D:
                case R6_OPC_CMP_SUN_D:
                case R6_OPC_CMP_SEQ_D:
                case R6_OPC_CMP_SEUQ_D:
                case R6_OPC_CMP_SLT_D:
                case R6_OPC_CMP_SULT_D:
                case R6_OPC_CMP_SLE_D:
                case R6_OPC_CMP_SULE_D:
                case R6_OPC_CMP_OR_D:
                case R6_OPC_CMP_UNE_D:
                case R6_OPC_CMP_NE_D:
                case R6_OPC_CMP_SOR_D:
                case R6_OPC_CMP_SUNE_D:
                case R6_OPC_CMP_SNE_D:
                    gen_r6_cmp_d(ctx, ctx->opcode & 0x1f, rt, rd, sa);
                    break;
                default:
                    gen_farith(ctx, ctx->opcode & FOP(0x3f, 0x1f),
                               rt, rd, sa, (imm >> 8) & 0x7);

                    break;
                }
            } else {
                gen_farith(ctx, ctx->opcode & FOP(0x3f, 0x1f), rt, rd, sa,
                           (imm >> 8) & 0x7);
            }
            break;
        }
        default:
            MIPS_INVAL("cp1");
            gen_reserved_instruction(ctx);
            break;
        }
        break;

#if defined(TARGET_CHERI)
    case OPC_CLOAD:     /* Load Via Capability Register */
        {
            uint32_t opc = ctx->opcode;

            check_cop2x(ctx);

            switch(MASK_CLDST_OPC(opc)) {
            case OPC_CLBU:
                generate_cap_load(ctx, rs, rt, rd, MASK_CLDST_OFFSET(opc),
                                  MO_UB | ctx->default_tcg_memop_mask,
                                  MASK_CLDST_OPC(opc));
                break;
            case OPC_CLHU:
                generate_cap_load(ctx, rs, rt, rd, MASK_CLDST_OFFSET(opc),
                                  MO_TEUW | ctx->default_tcg_memop_mask,
                                  MASK_CLDST_OPC(opc));
                break;
            case OPC_CLWU:
                generate_cap_load(ctx, rs, rt, rd, MASK_CLDST_OFFSET(opc),
                                  MO_TEUL | ctx->default_tcg_memop_mask,
                                  MASK_CLDST_OPC(opc));
                break;
            case OPC_CLDU:
                generate_cap_load(ctx, rs, rt, rd, MASK_CLDST_OFFSET(opc),
                                  MO_TEQ | ctx->default_tcg_memop_mask,
                                  MASK_CLDST_OPC(opc));
                break;
            case OPC_CLB:
                generate_cap_load(ctx, rs, rt, rd, MASK_CLDST_OFFSET(opc),
                                  MO_SB | ctx->default_tcg_memop_mask,
                                  MASK_CLDST_OPC(opc));
                break;
            case OPC_CLH:
                generate_cap_load(ctx, rs, rt, rd, MASK_CLDST_OFFSET(opc),
                                  MO_TESW | ctx->default_tcg_memop_mask,
                                  MASK_CLDST_OPC(opc));
                break;
            case OPC_CLW:
                generate_cap_load(ctx, rs, rt, rd, MASK_CLDST_OFFSET(opc),
                                  MO_TESL | ctx->default_tcg_memop_mask,
                                  MASK_CLDST_OPC(opc));
                break;
            case OPC_CLD:
                generate_cap_load(ctx, rs, rt, rd, MASK_CLDST_OFFSET(opc),
                                  MO_TEQ | ctx->default_tcg_memop_mask,
                                  MASK_CLDST_OPC(opc));
                break;
            default:
                MIPS_INVAL("cl");
                generate_exception (ctx, EXCP_RI);
                break;
            }
        }
        break;
    case OPC_CLOADC:    /* Load Capability Register */
        check_cop2x(ctx);
        generate_clc(ctx, rs, rt, rd, ctx->opcode & 0x7ff, false);
        break;
    case OPC_CLOADC_LargeImm: /* Load Capability Register with 16bit immediate */
        check_cop2x(ctx);
        generate_clc(ctx, rs, rt, 0, ctx->opcode & 0xffff, true);
        break;
    case OPC_CSTORE:    /* Store Via Capability Register */
        {
            uint32_t opc = ctx->opcode;

            check_cop2x(ctx);

/*
 * XXX CHERI seems to be ignoring bit 2 given how 'cscdr' is encoded.
 *     For now, just ignore bit 2 by masking it.
 *
 *          switch(MASK_CLDST_OPC(opc)) {
 */
            uint32_t inst_opc = MASK_CLDST_OPC(opc) & ~0x4;
            switch(inst_opc) {
            case OPC_CSB:
                generate_cstore(ctx, rs, rt, rd, MASK_CLDST_OFFSET(opc), MO_8,
                                inst_opc);
                break;
            case OPC_CSH:
                generate_cstore(ctx, rs, rt, rd, MASK_CLDST_OFFSET(opc),
                                MO_TEUW | ctx->default_tcg_memop_mask,
                                inst_opc);
                break;
            case OPC_CSW:
                generate_cstore(ctx, rs, rt, rd, MASK_CLDST_OFFSET(opc),
                                MO_TEUL | ctx->default_tcg_memop_mask,
                                inst_opc);
                break;
            case OPC_CSD:
                generate_cstore(ctx, rs, rt, rd, MASK_CLDST_OFFSET(opc),
                                MO_TEQ | ctx->default_tcg_memop_mask, inst_opc);
                break;
            default:
                MIPS_INVAL("cs");
                generate_exception (ctx, EXCP_RI);
                break;
            }
        }
        break;
    case OPC_CSTOREC:   /* Store Capability Register */
        check_cop2x(ctx);
        generate_csc(ctx, rs, rt, rd, imm & 0x7ff, false);
        break;
    case OPC_CSTOREC_LargeImm:   /* Store Capability Register with 16-bit immediate */
        check_cop2x(ctx);
        generate_csc(ctx, rs, rt, 0, imm & 0xffff, true);
        break;
#else /* ! TARGET_CHERI */
    /* Compact branches [R6] and COP2 [non-R6] */
    case OPC_BC: /* OPC_LWC2 */
    case OPC_BALC: /* OPC_SWC2 */
        if (ctx->insn_flags & ISA_MIPS_R6) {
            /* OPC_BC, OPC_BALC */
            gen_compute_compact_branch(ctx, op, 0, 0,
                                       sextract32(ctx->opcode << 2, 0, 28));
        } else if (ctx->insn_flags & ASE_LEXT) {
            gen_loongson_lswc2(ctx, rt, rs, rd);
        } else {
            /* OPC_LWC2, OPC_SWC2 */
            /* COP2: Not implemented. */
            generate_exception_err(ctx, EXCP_CpU, 2);
        }
        break;
    case OPC_BEQZC: /* OPC_JIC, OPC_LDC2 */
    case OPC_BNEZC: /* OPC_JIALC, OPC_SDC2 */
        if (ctx->insn_flags & ISA_MIPS_R6) {
            if (rs != 0) {
                /* OPC_BEQZC, OPC_BNEZC */
                gen_compute_compact_branch(ctx, op, rs, 0,
                                           sextract32(ctx->opcode << 2, 0, 23));
            } else {
                /* OPC_JIC, OPC_JIALC */
                gen_compute_compact_branch(ctx, op, 0, rt, imm);
            }
        } else if (ctx->insn_flags & ASE_LEXT) {
            gen_loongson_lsdc2(ctx, rt, rs, rd);
        } else {
            /* OPC_LWC2, OPC_SWC2 */
            /* COP2: Not implemented. */
            generate_exception_err(ctx, EXCP_CpU, 2);
        }
        break;
#endif /* ! TARGET_CHERI */
    case OPC_CP2:
#if defined(TARGET_CHERI)
#if defined(TARGET_MIPS64)
        check_mips_64(ctx);
#endif
        gen_cp2(ctx, ctx->opcode, rt, rd, sa);
        break;
#else /* ! TARGET_CHERI */
        check_insn(ctx, ASE_LMMI);
        /* Note that these instructions use different fields.  */
        gen_loongson_multimedia(ctx, sa, rd, rt);
        break;
#endif /* ! TARGET_CHERI */
    case OPC_CP3:
        if (ctx->CP0_Config1 & (1 << CP0C1_FP)) {
            check_cp1_enabled(ctx);
            op1 = MASK_CP3(ctx->opcode);
            switch (op1) {
            case OPC_LUXC1:
            case OPC_SUXC1:
                check_insn(ctx, ISA_MIPS5 | ISA_MIPS_R2);
                /* Fallthrough */
            case OPC_LWXC1:
            case OPC_LDXC1:
            case OPC_SWXC1:
            case OPC_SDXC1:
                check_insn(ctx, ISA_MIPS4 | ISA_MIPS_R2);
                gen_flt3_ldst(ctx, op1, sa, rd, rs, rt);
                break;
            case OPC_PREFX:
                check_insn(ctx, ISA_MIPS4 | ISA_MIPS_R2);
                /* Treat as NOP. */
                break;
            case OPC_ALNV_PS:
                check_insn(ctx, ISA_MIPS5 | ISA_MIPS_R2);
                /* Fallthrough */
            case OPC_MADD_S:
            case OPC_MADD_D:
            case OPC_MADD_PS:
            case OPC_MSUB_S:
            case OPC_MSUB_D:
            case OPC_MSUB_PS:
            case OPC_NMADD_S:
            case OPC_NMADD_D:
            case OPC_NMADD_PS:
            case OPC_NMSUB_S:
            case OPC_NMSUB_D:
            case OPC_NMSUB_PS:
                check_insn(ctx, ISA_MIPS4 | ISA_MIPS_R2);
                gen_flt3_arith(ctx, op1, sa, rs, rd, rt);
                break;
            default:
                MIPS_INVAL("cp3");
                gen_reserved_instruction(ctx);
                break;
            }
        } else {
            generate_exception_err(ctx, EXCP_CpU, 1);
        }
        break;

#if defined(TARGET_MIPS64)
    /* MIPS64 opcodes */
    case OPC_LLD:
        if (ctx->insn_flags & INSN_R5900) {
            check_insn_opc_user_only(ctx, INSN_R5900);
        }
        /* fall through */
    case OPC_LDL:
    case OPC_LDR:
    case OPC_LWU:
    case OPC_LD:
        check_insn(ctx, ISA_MIPS3);
        check_mips_64(ctx);
        gen_ld(ctx, op, rt, rs, imm);
        break;
    case OPC_SDL:
    case OPC_SDR:
    case OPC_SD:
        check_insn(ctx, ISA_MIPS3);
        check_mips_64(ctx);
        gen_st(ctx, op, rt, rs, imm);
        break;
    case OPC_SCD:
        check_insn(ctx, ISA_MIPS3);
        if (ctx->insn_flags & INSN_R5900) {
            check_insn_opc_user_only(ctx, INSN_R5900);
        }
        check_mips_64(ctx);
        gen_st_cond(ctx, rt, rs, imm, MO_TEQ, false, OPC_SCD);
        break;
    case OPC_BNVC: /* OPC_BNEZALC, OPC_BNEC, OPC_DADDI */
        if (ctx->insn_flags & ISA_MIPS_R6) {
            /* OPC_BNVC, OPC_BNEZALC, OPC_BNEC */
            gen_compute_compact_branch(ctx, op, rs, rt, imm << 2);
        } else {
            /* OPC_DADDI */
            check_insn(ctx, ISA_MIPS3);
            check_mips_64(ctx);
            gen_arith_imm(ctx, op, rt, rs, imm);
        }
        break;
    case OPC_DADDIU:
        check_insn(ctx, ISA_MIPS3);
        check_mips_64(ctx);
        gen_arith_imm(ctx, op, rt, rs, imm);
        break;
#else
    case OPC_BNVC: /* OPC_BNEZALC, OPC_BNEC */
        if (ctx->insn_flags & ISA_MIPS_R6) {
            gen_compute_compact_branch(ctx, op, rs, rt, imm << 2);
        } else {
            MIPS_INVAL("major opcode");
            gen_reserved_instruction(ctx);
        }
        break;
#endif

#if !defined(TARGET_CHERI) /* CHERI reuses these opcodes for experimental instrs */
    case OPC_DAUI: /* OPC_JALX */
        if (ctx->insn_flags & ISA_MIPS_R6) {
#if defined(TARGET_MIPS64)
            /* OPC_DAUI */
            check_mips_64(ctx);
            if (rs == 0) {
                generate_exception(ctx, EXCP_RI);
            } else if (rt != 0) {
                TCGv t0 = tcg_temp_new();
                gen_load_gpr(t0, rs);
                tcg_gen_addi_tl(cpu_gpr[rt], t0, imm << 16);
                gen_log_instr_gpr_update(ctx, rt);
                tcg_temp_free(t0);
            }
#else
            gen_reserved_instruction(ctx);
            MIPS_INVAL("major opcode");
#endif
        } else {
            /* OPC_JALX */
            if (ctx->insn_flags & (ASE_MIPS16 | ASE_MICROMIPS)) {
                offset = (int32_t)(ctx->opcode & 0x3FFFFFF) << 2;
                gen_compute_branch(ctx, op, 4, rs, rt, offset, 4);
            } else {
                gen_reserved_instruction(ctx);
            }
        }
        break;
    case OPC_MDMX: /* MMI_OPC_LQ */
        if (ctx->insn_flags & INSN_R5900) {
#if defined(TARGET_MIPS64)
            gen_mmi_lq(env, ctx);
#endif
        } else {
            /* MDMX: Not implemented. */
            gen_reserved_instruction(ctx);
        }
        break;
#endif /* ! TAGET_CHERI */
    case OPC_PCREL:
        check_insn(ctx, ISA_MIPS_R6);
        gen_pcrel(ctx, ctx->opcode, ctx->base.pc_next, rs);
        break;
    default:            /* Invalid */
        MIPS_INVAL("major opcode");
        return false;
    }
    return true;
}

static void decode_opc(CPUMIPSState *env, DisasContext *ctx)
{
    /* make sure instructions are on a word boundary */
    if (ctx->base.pc_next & 0x3) {
        env->CP0_BadVAddr = ctx->base.pc_next;
        generate_exception_err(ctx, EXCP_AdEL, EXCP_INST_NOTAVAIL);
        return;
    }

    /* Handle blikely not taken case */
    if ((ctx->hflags & MIPS_HFLAG_BMASK_BASE) == MIPS_HFLAG_BL) {
        TCGLabel *l1 = gen_new_label();

        tcg_gen_brcondi_tl(TCG_COND_NE, bcond, 0, l1);
        tcg_gen_movi_i32(hflags, ctx->hflags & ~MIPS_HFLAG_BMASK);
        gen_goto_tb(ctx, 1, ctx->base.pc_next + 4);
        gen_set_label(l1);
    }

    /* Transition to the auto-generated decoder.  */

    /* ISA extensions */
    if (ase_msa_available(env) && decode_ase_msa(ctx, ctx->opcode)) {
        return;
    }

    /* ISA (from latest to oldest) */
    if (cpu_supports_isa(env, ISA_MIPS_R6) && decode_isa_rel6(ctx, ctx->opcode)) {
        return;
    }
    if (cpu_supports_isa(env, INSN_R5900) && decode_ext_txx9(ctx, ctx->opcode)) {
        return;
    }

    if (decode_opc_legacy(env, ctx)) {
        return;
    }

    gen_reserved_instruction(ctx);
}

static void mips_tr_init_disas_context(DisasContextBase *dcbase, CPUState *cs)
{
    DisasContext *ctx = container_of(dcbase, DisasContext, base);
    CPUMIPSState *env = cs->env_ptr;

    ctx->page_start = ctx->base.pc_first & TARGET_PAGE_MASK;
    ctx->saved_pc = -1;
    ctx->insn_flags = env->insn_flags;
    ctx->CP0_Config1 = env->CP0_Config1;
    ctx->CP0_Config2 = env->CP0_Config2;
    ctx->CP0_Config3 = env->CP0_Config3;
    ctx->CP0_Config5 = env->CP0_Config5;
    ctx->btarget = 0;
    ctx->kscrexist = (env->CP0_Config4 >> CP0C4_KScrExist) & 0xff;
    ctx->rxi = (env->CP0_Config3 >> CP0C3_RXI) & 1;
    ctx->ie = (env->CP0_Config4 >> CP0C4_IE) & 3;
    ctx->bi = (env->CP0_Config3 >> CP0C3_BI) & 1;
    ctx->bp = (env->CP0_Config3 >> CP0C3_BP) & 1;
    ctx->PAMask = env->PAMask;
    ctx->mvh = (env->CP0_Config5 >> CP0C5_MVH) & 1;
    ctx->eva = (env->CP0_Config5 >> CP0C5_EVA) & 1;
    ctx->sc = (env->CP0_Config3 >> CP0C3_SC) & 1;
    ctx->CP0_LLAddr_shift = env->CP0_LLAddr_shift;
    ctx->cmgcr = (env->CP0_Config3 >> CP0C3_CMGCR) & 1;
    /* Restore delay slot state from the tb context.  */
    ctx->hflags = (uint32_t)ctx->base.tb->flags; /* FIXME: maybe use 64 bits? */
    ctx->ulri = (env->CP0_Config3 >> CP0C3_ULRI) & 1;
    ctx->ps = ((env->active_fpu.fcr0 >> FCR0_PS) & 1) ||
             (env->insn_flags & (INSN_LOONGSON2E | INSN_LOONGSON2F));
    ctx->vp = (env->CP0_Config5 >> CP0C5_VP) & 1;
    ctx->mrp = (env->CP0_Config5 >> CP0C5_MRP) & 1;
    ctx->nan2008 = (env->active_fpu.fcr31 >> FCR31_NAN2008) & 1;
    ctx->abs2008 = (env->active_fpu.fcr31 >> FCR31_ABS2008) & 1;
    ctx->mi = (env->CP0_Config5 >> CP0C5_MI) & 1;
    ctx->gi = (env->CP0_Config5 >> CP0C5_GI) & 3;
    restore_cpu_state(env, ctx);
#ifdef CONFIG_USER_ONLY
        ctx->mem_idx = MIPS_HFLAG_UM;
#else
        ctx->mem_idx = hflags_mmu_index(ctx->hflags);
#endif
#if defined(CHERI_UNALIGNED)
    ctx->default_tcg_memop_mask = MO_UNALN;
#else
    ctx->default_tcg_memop_mask = (ctx->insn_flags & (ISA_MIPS_R6 |
                                  INSN_LOONGSON3A)) ? MO_UNALN : MO_ALIGN;
#endif
    LOG_DISAS("\ntb %p idx %d hflags %04x\n", ctx->base.tb, ctx->mem_idx,
              ctx->hflags);
}

static inline void mips_update_statcounters_icount(DisasContext *ctx)
{
    // FIXME: Don't do this for every executed instruction.
#ifdef NOTYET
    _debug_value(cpu_PC, "mips_update_icount");
    if (ctx->icount_already_added == ctx->base.num_insns) {
        qemu_log("%s: icount already updated: %d\n", __func__, ctx->base.num_insns);
        return;
    }
    target_ulong diff = ctx->base.num_insns - ctx->icount_already_added;
#else
    target_ulong diff = 1;
#endif
    if (ctx->hflags & MIPS_HFLAG_UM) {
        tcg_gen_addi_tl(cpu_statcounters_icount_user,
                        cpu_statcounters_icount_user, diff);
    } else {
        tcg_gen_addi_tl(cpu_statcounters_icount_kernel,
                        cpu_statcounters_icount_kernel, diff);
    }
#ifdef NOTYET
    ctx->icount_already_added = ctx->base.num_insns;
#endif
}

static void mips_tr_tb_start(DisasContextBase *dcbase, CPUState *cs)
{
}

static void mips_tr_insn_start(DisasContextBase *dcbase, CPUState *cs)
{
    DisasContext *ctx = container_of(dcbase, DisasContext, base);

    /* If QEMU was started with -bc option insert a check for breakcount */
    if (unlikely(cs->breakcount)) {
        gen_helper_check_breakcount(cpu_env);
    }

    tcg_gen_insn_start(ctx->base.pc_next, ctx->hflags & MIPS_HFLAG_BMASK,
                       ctx->btarget);

    /* Update the icount statcounter */
    // TODO: it would be nice if we could do this at TB end but exceptions
    // use longjmp to jump out of the tb so it is quite difficult.
    mips_update_statcounters_icount(ctx); // Increment the current icount value
}

static bool mips_tr_breakpoint_check(DisasContextBase *dcbase, CPUState *cs,
                                     const CPUBreakpoint *bp)
{
    DisasContext *ctx = container_of(dcbase, DisasContext, base);

    save_cpu_state(ctx, 1);
    ctx->base.is_jmp = DISAS_NORETURN;
    gen_helper_raise_exception_debug(cpu_env);
    /*
     * The address covered by the breakpoint must be included in
     * [tb->pc, tb->pc + tb->size) in order to for it to be
     * properly cleared -- thus we increment the PC here so that
     * the logic setting tb->size below does the right thing.
     */
    ctx->base.pc_next += 4;
    return true;
}

#ifdef CONFIG_TCG_LOG_INSTR
static inline void gen_mips_log_instr_unsupported(
    DisasContext *ctx, const char *what)
{
    if (unlikely(ctx->base.log_instr_enabled)) {
        warn_report("%s instruction tracing is not implemented\n", what);
        gen_helper_mips_log_instr_drop(cpu_env);
    }
}

static inline void gen_mips_log_instr32(DisasContext *ctx)
{
    if (unlikely(ctx->base.log_instr_enabled)) {
        TCGv pc = tcg_const_tl(ctx->base.pc_next);
        TCGv_i32 opc = tcg_constant_i32(ctx->opcode);
        gen_helper_mips_log_instr32(cpu_env, pc, opc);
        tcg_temp_free(pc);
    }
}
#else
#define gen_mips_log_instr_unsupported(ctx, what) ((void)0)
#define gen_mips_log_instr32(ctx) ((void)0)
#endif

static void mips_tr_translate_insn(DisasContextBase *dcbase, CPUState *cs)
{
    CPUMIPSState *env = cs->env_ptr;
    DisasContext *ctx = container_of(dcbase, DisasContext, base);
    int insn_bytes;
    int is_slot;
    // XXX: we don't support micromips, etc. so we can hardcode 4 bytes as the
    // instruction size (see assert below).
    gen_check_pcc_bounds_next_inst(ctx, 4);
    is_slot = ctx->hflags & MIPS_HFLAG_BMASK;
    if (ctx->insn_flags & ISA_NANOMIPS32) {
#ifdef TARGET_CHERI
        /* NanoMIPS not supported */
        gen_reserved_instruction(ctx);
        g_assert(ctx->base.is_jmp == DISAS_NORETURN);
        return;
#else
        gen_mips_log_instr_unsupported(ctx, "nanomips");
        ctx->opcode = translator_lduw(env, ctx->base.pc_next);
        insn_bytes = decode_nanomips_opc(env, ctx);
#endif
    } else if (!(ctx->hflags & MIPS_HFLAG_M16)) {
        ctx->opcode = translator_ldl(env, ctx->base.pc_next);
        insn_bytes = 4;
        gen_mips_log_instr32(ctx);
        decode_opc(env, ctx);
    } else if (ctx->insn_flags & ASE_MICROMIPS) {
        gen_mips_log_instr_unsupported(ctx, "micromips");
        ctx->opcode = translator_lduw(env, ctx->base.pc_next);
        insn_bytes = decode_micromips_opc(env, ctx);
#ifndef TARGET_CHERI /* MIPS16 not supported */
    } else if (ctx->insn_flags & ASE_MIPS16) {
        gen_mips_log_instr_unsupported(ctx, "mips16");
        ctx->opcode = translator_lduw(env, ctx->base.pc_next);
        insn_bytes = decode_mips16_opc(env, ctx);
#endif
    } else {
        gen_reserved_instruction(ctx);
        g_assert(ctx->base.is_jmp == DISAS_NORETURN);
        return;
    }
#ifdef TARGET_CHERI
    tcg_debug_assert(insn_bytes == 4 && "Compressed insns not supported");
#endif

    if (ctx->hflags & MIPS_HFLAG_BMASK) {
        if (!(ctx->hflags & (MIPS_HFLAG_BDS16 | MIPS_HFLAG_BDS32 |
                             MIPS_HFLAG_FBNSLOT))) {
            /*
             * Force to generate branch as there is neither delay nor
             * forbidden slot.
             */
            is_slot = 1;
        }
        if ((ctx->hflags & MIPS_HFLAG_M16) &&
            (ctx->hflags & MIPS_HFLAG_FBNSLOT)) {
            /*
             * Force to generate branch as microMIPS R6 doesn't restrict
             * branches in the forbidden slot.
             */
            is_slot = 1;
        }
    }
    if (is_slot) {
        gen_branch(ctx, insn_bytes);
    }
    ctx->base.pc_next += insn_bytes;

    if (ctx->base.is_jmp != DISAS_NEXT) {
        return;
    }
    /*
     * Execute a branch and its delay slot as a single instruction.
     * This is what GDB expects and is consistent with what the
     * hardware does (e.g. if a delay slot instruction faults, the
     * reported PC is the PC of the branch).
     */
    if (ctx->base.singlestep_enabled &&
        (ctx->hflags & MIPS_HFLAG_BMASK) == 0) {
        ctx->base.is_jmp = DISAS_TOO_MANY;
    }
    if (ctx->base.pc_next - ctx->page_start >= TARGET_PAGE_SIZE) {
        ctx->base.is_jmp = DISAS_TOO_MANY;
    }
}

static void mips_tr_tb_stop(DisasContextBase *dcbase, CPUState *cs)
{
    DisasContext *ctx = container_of(dcbase, DisasContext, base);
    // tcg_debug_assert(ctx->icount_already_added == dcbase->num_insns);

    if (ctx->base.singlestep_enabled && ctx->base.is_jmp != DISAS_NORETURN) {
        save_cpu_state(ctx, ctx->base.is_jmp != DISAS_EXIT);
        gen_helper_raise_exception_debug(cpu_env);
    } else {
        switch (ctx->base.is_jmp) {
        case DISAS_STOP:
            gen_save_pc(ctx->base.pc_next);
            tcg_gen_lookup_and_goto_ptr();
            break;
        case DISAS_NEXT:
        case DISAS_TOO_MANY:
            save_cpu_state(ctx, 0);
            gen_goto_tb(ctx, 0, ctx->base.pc_next);
            break;
        case DISAS_EXIT:
            tcg_gen_exit_tb(NULL, 0);
            break;
        case DISAS_NORETURN:
            break;
        default:
            g_assert_not_reached();
        }
    }
}

static void mips_tr_disas_log(const DisasContextBase *dcbase, CPUState *cs)
{
    qemu_log("IN: %s\n", lookup_symbol(dcbase->pc_first));
    log_target_disas(cs, dcbase->pc_first, dcbase->tb->size);
}

static const TranslatorOps mips_tr_ops = {
    .init_disas_context = mips_tr_init_disas_context,
    .tb_start           = mips_tr_tb_start,
    .insn_start         = mips_tr_insn_start,
    .breakpoint_check   = mips_tr_breakpoint_check,
    .translate_insn     = mips_tr_translate_insn,
    .tb_stop            = mips_tr_tb_stop,
    .disas_log          = mips_tr_disas_log,
};

void gen_intermediate_code(CPUState *cs, TranslationBlock *tb, int max_insns)
{
    DisasContext ctx;

    translator_loop(&mips_tr_ops, &ctx.base, cs, tb, max_insns);
}

static void fpu_dump_state(CPUMIPSState *env, FILE * f, int flags)
{
    int i;
    int is_fpu64 = !!(env->hflags & MIPS_HFLAG_F64);

#define printfpr(fp)                                                    \
    do {                                                                \
        if (is_fpu64)                                                   \
            qemu_fprintf(f, "w:%08x d:%016" PRIx64                      \
                         " fd:%13g fs:%13g psu: %13g\n",                \
                         (fp)->w[FP_ENDIAN_IDX], (fp)->d,               \
                         (double)(fp)->fd,                              \
                         (double)(fp)->fs[FP_ENDIAN_IDX],               \
                         (double)(fp)->fs[!FP_ENDIAN_IDX]);             \
        else {                                                          \
            fpr_t tmp;                                                  \
            tmp.w[FP_ENDIAN_IDX] = (fp)->w[FP_ENDIAN_IDX];              \
            tmp.w[!FP_ENDIAN_IDX] = ((fp) + 1)->w[FP_ENDIAN_IDX];       \
            qemu_fprintf(f, "w:%08x d:%016" PRIx64                      \
                         " fd:%13g fs:%13g psu:%13g\n",                 \
                         tmp.w[FP_ENDIAN_IDX], tmp.d,                   \
                         (double)tmp.fd,                                \
                         (double)tmp.fs[FP_ENDIAN_IDX],                 \
                         (double)tmp.fs[!FP_ENDIAN_IDX]);               \
        }                                                               \
    } while (0)


    qemu_fprintf(f,
                 "CP1 FCR0 0x%08x  FCR31 0x%08x  SR.FR %d  fp_status 0x%02x\n",
                 env->active_fpu.fcr0, env->active_fpu.fcr31, is_fpu64,
                 get_float_exception_flags(&env->active_fpu.fp_status));
    for (i = 0; i < 32; (is_fpu64) ? i++ : (i += 2)) {
        qemu_fprintf(f, "%3s: ", mips_fregnames[i]);
        printfpr(&env->active_fpu.fpr[i]);
    }

#undef printfpr
}

void mips_cpu_dump_state(CPUState *cs, FILE *f, int flags)
{
    MIPSCPU *cpu = MIPS_CPU(cs);
    CPUMIPSState *env = &cpu->env;
    int i;

    qemu_fprintf(f, "pc=0x" TARGET_FMT_lx " HI=0x" TARGET_FMT_lx
                 " LO=0x" TARGET_FMT_lx " ds %04x "
                 TARGET_FMT_lx " " TARGET_FMT_ld "\n",
                 PC_ADDR(env), env->active_tc.HI[0], env->active_tc.LO[0],
                 env->hflags, env->btarget, env->bcond);
    for (i = 0; i < 32; i++) {
        if ((i & 3) == 0) {
            qemu_fprintf(f, "GPR%02d:", i);
        }
        qemu_fprintf(f, " %s " TARGET_FMT_lx,
                     mips_gp_regnames[i], env->active_tc.gpr[i]);
        if ((i & 3) == 3) {
            qemu_fprintf(f, "\n");
        }
    }

    qemu_fprintf(f, "CP0 Status  0x%08x Cause   0x%08x EPC    0x"
                 TARGET_FMT_lx "\n",
                 env->CP0_Status, env->CP0_Cause, get_CP0_EPC(env));
    qemu_fprintf(f, "    Config0 0x%08x Config1 0x%08x LLAddr 0x%016"
                 PRIx64 "\n",
                 env->CP0_Config0, env->CP0_Config1, env->CP0_LLAddr);
    qemu_fprintf(f, "    Config2 0x%08x Config3 0x%08x\n",
                 env->CP0_Config2, env->CP0_Config3);
    qemu_fprintf(f, "    Config4 0x%08x Config5 0x%08x\n",
                 env->CP0_Config4, env->CP0_Config5);
    if ((flags & CPU_DUMP_FPU) && (env->hflags & MIPS_HFLAG_FPU)) {
        fpu_dump_state(env, f, flags);
    }
}

void mips_tcg_init(void)
{
    int i;

    cpu_gpr[0] = NULL;
    for (i = 1; i < 32; i++)
        cpu_gpr[i] = tcg_global_mem_new(cpu_env,
                                        offsetof(CPUMIPSState,
                                                 active_tc.gpr[i]),
                                        mips_gp_regnames[i]);
#if defined(TARGET_MIPS64)
    cpu_gpr_hi[0] = NULL;

    for (unsigned i = 1; i < 32; i++) {
        g_autofree char *rname = g_strdup_printf("%s[hi]", mips_gp_regnames[i]);

        cpu_gpr_hi[i] = tcg_global_mem_new_i64(cpu_env,
                                               offsetof(CPUMIPSState,
                                                        active_tc.gpr_hi[i]),
                                               rname);
    }
#endif /* !TARGET_MIPS64 */
    for (i = 0; i < 32; i++) {
        int off = offsetof(CPUMIPSState, active_fpu.fpr[i].wr.d[0]);

        fpu_f64[i] = tcg_global_mem_new_i64(cpu_env, off, mips_fregnames[i]);
    }
#ifdef TARGET_CHERI
    cpu_PC = tcg_global_mem_new(cpu_env,
                                offsetof(CPUMIPSState, active_tc.PCC._cr_cursor), "PC");
    /// XXXAR: We currently interpose using DDC.cursor and not DDC.base!
    ddc_interposition = tcg_global_mem_new(cpu_env,
                                offsetof(CPUMIPSState, active_tc.CHWR.DDC._cr_cursor), "ddc_interpose");
#else
    msa_translate_init();
    cpu_PC = tcg_global_mem_new(cpu_env,
                                offsetof(CPUMIPSState, active_tc.PC), "PC");
#endif
#ifdef CONFIG_DEBUG_TCG
    _pc_is_current = tcg_global_mem_new(
        cpu_env, offsetof(CPUArchState, active_tc._pc_is_current),
        "_pc_is_current");
#endif
    for (i = 0; i < MIPS_DSP_ACC; i++) {
        cpu_HI[i] = tcg_global_mem_new(cpu_env,
                                       offsetof(CPUMIPSState, active_tc.HI[i]),
                                       mips_regnames_HI[i]);
        cpu_LO[i] = tcg_global_mem_new(cpu_env,
                                       offsetof(CPUMIPSState, active_tc.LO[i]),
                                       mips_regnames_LO[i]);
    }
    cpu_dspctrl = tcg_global_mem_new(cpu_env,
                                     offsetof(CPUMIPSState,
                                              active_tc.DSPControl),
                                     "DSPControl");
    bcond = tcg_global_mem_new(cpu_env,
                               offsetof(CPUMIPSState, bcond), "bcond");
    btarget = tcg_global_mem_new(cpu_env,
                                 offsetof(CPUMIPSState, btarget), "btarget");
    hflags = tcg_global_mem_new_i32(cpu_env,
                                    offsetof(CPUMIPSState, hflags), "hflags");

    fpu_fcr0 = tcg_global_mem_new_i32(cpu_env,
                                      offsetof(CPUMIPSState, active_fpu.fcr0),
                                      "fcr0");
    fpu_fcr31 = tcg_global_mem_new_i32(cpu_env,
                                       offsetof(CPUMIPSState, active_fpu.fcr31),
                                       "fcr31");
    cpu_lladdr = tcg_global_mem_new(cpu_env, offsetof(CPUMIPSState, lladdr),
                                    "lladdr");
    cpu_llval = tcg_global_mem_new(cpu_env, offsetof(CPUMIPSState, llval),
                                   "llval");
    cpu_statcounters_icount_kernel = tcg_global_mem_new(
        cpu_env, offsetof(CPUMIPSState, statcounters_icount_kernel),
        "statcounters_icount_kernel");
    cpu_statcounters_icount_user = tcg_global_mem_new(
        cpu_env, offsetof(CPUMIPSState, statcounters_icount_user),
        "statcounters_icount_user");

    if (TARGET_LONG_BITS == 32) {
        mxu_translate_init();
    }
}

void restore_state_to_opc(CPUMIPSState *env, TranslationBlock *tb,
                          target_ulong *data)
{
    mips_update_pc(env, data[0], /*can_be_unrepresentable=*/false);
    env->hflags &= ~MIPS_HFLAG_BMASK;
    env->hflags |= data[1];
    switch (env->hflags & MIPS_HFLAG_BMASK_BASE) {
    case MIPS_HFLAG_BR:
#ifdef TARGET_CHERI
    case MIPS_HFLAG_BRCCALL:
    case MIPS_HFLAG_BRC:
#endif
        break;
    case MIPS_HFLAG_BC:
    case MIPS_HFLAG_BL:
    case MIPS_HFLAG_B:
        env->btarget = data[2];
        break;
    }
}

void gen_cheri_break_loadlink(TCGv_cap_checked_ptr out_addr)
{
    tcg_gen_movi_tl(cpu_lladdr, 0);
}
