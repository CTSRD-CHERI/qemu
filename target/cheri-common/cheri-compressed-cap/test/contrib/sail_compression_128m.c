#include "sail.h"
#include "rts.h"
#include "elf.h"
void (*sail_rts_set_coverage_file)(const char *) = NULL;
#ifdef __cplusplus
extern "C" {
#endif

// enum write_kind
enum zwrite_kind { zWrite_plain, zWrite_conditional, zWrite_release, zWrite_exclusive, zWrite_exclusive_release, zWrite_RISCV_release, zWrite_RISCV_strong_release, zWrite_RISCV_conditional, zWrite_RISCV_conditional_release, zWrite_RISCV_conditional_strong_release, zWrite_X86_locked };

static bool EQUAL(zwrite_kind)(enum zwrite_kind op1, enum zwrite_kind op2) {
  return op1 == op2;
}

static enum zwrite_kind UNDEFINED(zwrite_kind)(unit u) { return zWrite_plain; }

// enum trans_kind
enum ztrans_kind { zTransaction_start, zTransaction_commit, zTransaction_abort };

static bool EQUAL(ztrans_kind)(enum ztrans_kind op1, enum ztrans_kind op2) {
  return op1 == op2;
}

static enum ztrans_kind UNDEFINED(ztrans_kind)(unit u) { return zTransaction_start; }

// enum signal
enum zsignal { zLOW, zHIGH };

static bool EQUAL(zsignal)(enum zsignal op1, enum zsignal op2) {
  return op1 == op2;
}

static enum zsignal UNDEFINED(zsignal)(unit u) { return zLOW; }

// enum read_kind
enum zread_kind { zRead_plain, zRead_reserve, zRead_acquire, zRead_exclusive, zRead_exclusive_acquire, zRead_stream, zRead_ifetch, zRead_RISCV_acquire, zRead_RISCV_strong_acquire, zRead_RISCV_reserved, zRead_RISCV_reserved_acquire, zRead_RISCV_reserved_strong_acquire, zRead_X86_locked };

static bool EQUAL(zread_kind)(enum zread_kind op1, enum zread_kind op2) {
  return op1 == op2;
}

static enum zread_kind UNDEFINED(zread_kind)(unit u) { return zRead_plain; }

// union exception
enum kind_zexception { Kind_zError_ConstrainedUnpredictable, Kind_zError_ExceptionTaken, Kind_zError_ImplementationDefined, Kind_zError_ReservedEncoding, Kind_zError_SError, Kind_zError_See, Kind_zError_Undefined, Kind_zError_Unpredictable };

struct zexception {
  enum kind_zexception kind;
  union {
    struct { unit zError_ConstrainedUnpredictable; };
    struct { unit zError_ExceptionTaken; };
    struct { sail_string zError_ImplementationDefined; };
    struct { unit zError_ReservedEncoding; };
    struct { unit zError_SError; };
    struct { sail_string zError_See; };
    struct { unit zError_Undefined; };
    struct { unit zError_Unpredictable; };
  };
};

static void CREATE(zexception)(struct zexception *op) {
  op->kind = Kind_zError_ConstrainedUnpredictable;
}

static void RECREATE(zexception)(struct zexception *op) {

}

static void KILL(zexception)(struct zexception *op) {
  if (op->kind == Kind_zError_ImplementationDefined) {
    KILL(sail_string)(&op->zError_ImplementationDefined);
  } else if (op->kind == Kind_zError_See) {
    KILL(sail_string)(&op->zError_See);
  } else {}
}

static void COPY(zexception)(struct zexception *rop, struct zexception op) {
  if (rop->kind == Kind_zError_ImplementationDefined) {
    KILL(sail_string)(&rop->zError_ImplementationDefined);
  } else if (rop->kind == Kind_zError_See) {
    KILL(sail_string)(&rop->zError_See);
  } else {};
  rop->kind = op.kind;
  if (op.kind == Kind_zError_ConstrainedUnpredictable) {
    rop->zError_ConstrainedUnpredictable = op.zError_ConstrainedUnpredictable;
  } else if (op.kind == Kind_zError_ExceptionTaken) {
    rop->zError_ExceptionTaken = op.zError_ExceptionTaken;
  } else if (op.kind == Kind_zError_ImplementationDefined) {
    CREATE(sail_string)(&rop->zError_ImplementationDefined); COPY(sail_string)(&rop->zError_ImplementationDefined, op.zError_ImplementationDefined);
  } else if (op.kind == Kind_zError_ReservedEncoding) {
    rop->zError_ReservedEncoding = op.zError_ReservedEncoding;
  } else if (op.kind == Kind_zError_SError) {
    rop->zError_SError = op.zError_SError;
  } else if (op.kind == Kind_zError_See) {
    CREATE(sail_string)(&rop->zError_See); COPY(sail_string)(&rop->zError_See, op.zError_See);
  } else if (op.kind == Kind_zError_Undefined) {
    rop->zError_Undefined = op.zError_Undefined;
  } else if (op.kind == Kind_zError_Unpredictable) {
    rop->zError_Unpredictable = op.zError_Unpredictable;
  }
}

static bool EQUAL(zexception)(struct zexception op1, struct zexception op2) {
  if (op1.kind == Kind_zError_ConstrainedUnpredictable && op2.kind == Kind_zError_ConstrainedUnpredictable) {
    return EQUAL(unit)(op1.zError_ConstrainedUnpredictable, op2.zError_ConstrainedUnpredictable);
  } else if (op1.kind == Kind_zError_ExceptionTaken && op2.kind == Kind_zError_ExceptionTaken) {
    return EQUAL(unit)(op1.zError_ExceptionTaken, op2.zError_ExceptionTaken);
  } else if (op1.kind == Kind_zError_ImplementationDefined && op2.kind == Kind_zError_ImplementationDefined) {
    return EQUAL(sail_string)(op1.zError_ImplementationDefined, op2.zError_ImplementationDefined);
  } else if (op1.kind == Kind_zError_ReservedEncoding && op2.kind == Kind_zError_ReservedEncoding) {
    return EQUAL(unit)(op1.zError_ReservedEncoding, op2.zError_ReservedEncoding);
  } else if (op1.kind == Kind_zError_SError && op2.kind == Kind_zError_SError) {
    return EQUAL(unit)(op1.zError_SError, op2.zError_SError);
  } else if (op1.kind == Kind_zError_See && op2.kind == Kind_zError_See) {
    return EQUAL(sail_string)(op1.zError_See, op2.zError_See);
  } else if (op1.kind == Kind_zError_Undefined && op2.kind == Kind_zError_Undefined) {
    return EQUAL(unit)(op1.zError_Undefined, op2.zError_Undefined);
  } else if (op1.kind == Kind_zError_Unpredictable && op2.kind == Kind_zError_Unpredictable) {
    return EQUAL(unit)(op1.zError_Unpredictable, op2.zError_Unpredictable);
  } else return false;
}

static void sailgen_Error_ConstrainedUnpredictable(struct zexception *rop, unit op) {
  if (rop->kind == Kind_zError_ImplementationDefined) {
    KILL(sail_string)(&rop->zError_ImplementationDefined);
  } else if (rop->kind == Kind_zError_See) {
    KILL(sail_string)(&rop->zError_See);
  } else {}
  rop->kind = Kind_zError_ConstrainedUnpredictable;
  rop->zError_ConstrainedUnpredictable = op;
}

static void sailgen_Error_ExceptionTaken(struct zexception *rop, unit op) {
  if (rop->kind == Kind_zError_ImplementationDefined) {
    KILL(sail_string)(&rop->zError_ImplementationDefined);
  } else if (rop->kind == Kind_zError_See) {
    KILL(sail_string)(&rop->zError_See);
  } else {}
  rop->kind = Kind_zError_ExceptionTaken;
  rop->zError_ExceptionTaken = op;
}

static void sailgen_Error_ImplementationDefined(struct zexception *rop, const_sail_string op) {
  if (rop->kind == Kind_zError_ImplementationDefined) {
    KILL(sail_string)(&rop->zError_ImplementationDefined);
  } else if (rop->kind == Kind_zError_See) {
    KILL(sail_string)(&rop->zError_See);
  } else {}
  rop->kind = Kind_zError_ImplementationDefined;
  CREATE(sail_string)(&rop->zError_ImplementationDefined);
  COPY(sail_string)(&rop->zError_ImplementationDefined, op);
}

static void sailgen_Error_ReservedEncoding(struct zexception *rop, unit op) {
  if (rop->kind == Kind_zError_ImplementationDefined) {
    KILL(sail_string)(&rop->zError_ImplementationDefined);
  } else if (rop->kind == Kind_zError_See) {
    KILL(sail_string)(&rop->zError_See);
  } else {}
  rop->kind = Kind_zError_ReservedEncoding;
  rop->zError_ReservedEncoding = op;
}

static void sailgen_Error_SError(struct zexception *rop, unit op) {
  if (rop->kind == Kind_zError_ImplementationDefined) {
    KILL(sail_string)(&rop->zError_ImplementationDefined);
  } else if (rop->kind == Kind_zError_See) {
    KILL(sail_string)(&rop->zError_See);
  } else {}
  rop->kind = Kind_zError_SError;
  rop->zError_SError = op;
}

static void sailgen_Error_See(struct zexception *rop, const_sail_string op) {
  if (rop->kind == Kind_zError_ImplementationDefined) {
    KILL(sail_string)(&rop->zError_ImplementationDefined);
  } else if (rop->kind == Kind_zError_See) {
    KILL(sail_string)(&rop->zError_See);
  } else {}
  rop->kind = Kind_zError_See;
  CREATE(sail_string)(&rop->zError_See);
  COPY(sail_string)(&rop->zError_See, op);
}

static void sailgen_Error_Undefined(struct zexception *rop, unit op) {
  if (rop->kind == Kind_zError_ImplementationDefined) {
    KILL(sail_string)(&rop->zError_ImplementationDefined);
  } else if (rop->kind == Kind_zError_See) {
    KILL(sail_string)(&rop->zError_See);
  } else {}
  rop->kind = Kind_zError_Undefined;
  rop->zError_Undefined = op;
}

static void sailgen_Error_Unpredictable(struct zexception *rop, unit op) {
  if (rop->kind == Kind_zError_ImplementationDefined) {
    KILL(sail_string)(&rop->zError_ImplementationDefined);
  } else if (rop->kind == Kind_zError_See) {
    KILL(sail_string)(&rop->zError_See);
  } else {}
  rop->kind = Kind_zError_Unpredictable;
  rop->zError_Unpredictable = op;
}

struct zexception *current_exception = NULL;
bool have_exception = false;
sail_string *throw_location = NULL;

// enum cache_op_kind
enum zcache_op_kind { zCache_op_D_IVAC, zCache_op_D_ISW, zCache_op_D_CSW, zCache_op_D_CISW, zCache_op_D_ZVA, zCache_op_D_CVAC, zCache_op_D_CVAU, zCache_op_D_CIVAC, zCache_op_I_IALLUIS, zCache_op_I_IALLU, zCache_op_I_IVAU };

static bool EQUAL(zcache_op_kind)(enum zcache_op_kind op1, enum zcache_op_kind op2) {
  return op1 == op2;
}

static enum zcache_op_kind UNDEFINED(zcache_op_kind)(unit u) { return zCache_op_D_IVAC; }

// enum a64_barrier_type
enum za64_barrier_type { zA64_barrier_all, zA64_barrier_LD, zA64_barrier_ST };

static bool EQUAL(za64_barrier_type)(enum za64_barrier_type op1, enum za64_barrier_type op2) {
  return op1 == op2;
}

static enum za64_barrier_type UNDEFINED(za64_barrier_type)(unit u) { return zA64_barrier_all; }

// enum a64_barrier_domain
enum za64_barrier_domain { zA64_FullShare, zA64_InnerShare, zA64_OuterShare, zA64_NonShare };

static bool EQUAL(za64_barrier_domain)(enum za64_barrier_domain op1, enum za64_barrier_domain op2) {
  return op1 == op2;
}

static enum za64_barrier_domain UNDEFINED(za64_barrier_domain)(unit u) { return zA64_FullShare; }

// enum __InstrEnc
enum z__InstrEnc { z__A64, z__A32, z__T16, z__T32 };

static bool EQUAL(z__InstrEnc)(enum z__InstrEnc op1, enum z__InstrEnc op2) {
  return op1 == op2;
}

static enum z__InstrEnc UNDEFINED(z__InstrEnc)(unit u) { return z__A64; }

// enum VirtualAddressType
enum zVirtualAddressType { zVA_Bits64, zVA_Capability };

static bool EQUAL(zVirtualAddressType)(enum zVirtualAddressType op1, enum zVirtualAddressType op2) {
  return op1 == op2;
}

static enum zVirtualAddressType UNDEFINED(zVirtualAddressType)(unit u) { return zVA_Bits64; }

// enum VBitOp
enum zVBitOp { zVBitOp_VBIF, zVBitOp_VBIT, zVBitOp_VBSL, zVBitOp_VEOR };

static bool EQUAL(zVBitOp)(enum zVBitOp op1, enum zVBitOp op2) {
  return op1 == op2;
}

static enum zVBitOp UNDEFINED(zVBitOp)(unit u) { return zVBitOp_VBIF; }

// enum Unpredictable
enum zUnpredictable { zUnpredictable_WBOVERLAPLD, zUnpredictable_WBOVERLAPST, zUnpredictable_LDPOVERLAP, zUnpredictable_BASEOVERLAP, zUnpredictable_DATAOVERLAP, zUnpredictable_DEVPAGE2, zUnpredictable_INSTRDEVICE, zUnpredictable_RESCPACR, zUnpredictable_RESMAIR, zUnpredictable_RESTEXCB, zUnpredictable_RESPRRR, zUnpredictable_RESDACR, zUnpredictable_RESVTCRS, zUnpredictable_RESTnSZ, zUnpredictable_LARGEIPA, zUnpredictable_ESRCONDPASS, zUnpredictable_ILZEROIT, zUnpredictable_ILZEROT, zUnpredictable_BPVECTORCATCHPRI, zUnpredictable_VCMATCHHALF, zUnpredictable_VCMATCHDAPA, zUnpredictable_WPMASKANDBAS, zUnpredictable_WPBASCONTIGUOUS, zUnpredictable_RESWPMASK, zUnpredictable_WPMASKEDBITS, zUnpredictable_RESBPWPCTRL, zUnpredictable_BPNOTIMPL, zUnpredictable_RESBPTYPE, zUnpredictable_BPNOTCTXCMP, zUnpredictable_BPMATCHHALF, zUnpredictable_BPMISMATCHHALF, zUnpredictable_RESTARTALIGNPC, zUnpredictable_RESTARTZEROUPPERPC, zUnpredictable_ZEROUPPER, zUnpredictable_ERETZEROUPPERPC, zUnpredictable_A32FORCEALIGNPC, zUnpredictable_SMD, zUnpredictable_AFUPDATE, zUnpredictable_IESBinDebug, zUnpredictable_BADPMSFCR, zUnpredictable_LINKTRANSFEROVERLAPLD, zUnpredictable_LINKBASEOVERLAPLD, zUnpredictable_CLEARERRITEZERO };

static bool EQUAL(zUnpredictable)(enum zUnpredictable op1, enum zUnpredictable op2) {
  return op1 == op2;
}

static enum zUnpredictable UNDEFINED(zUnpredictable)(unit u) { return zUnpredictable_WBOVERLAPLD; }

// enum SystemHintOp
enum zSystemHintOp { zSystemHintOp_NOP, zSystemHintOp_YIELD, zSystemHintOp_WFE, zSystemHintOp_WFI, zSystemHintOp_SEV, zSystemHintOp_SEVL, zSystemHintOp_ESB, zSystemHintOp_PSB, zSystemHintOp_CSDB };

static bool EQUAL(zSystemHintOp)(enum zSystemHintOp op1, enum zSystemHintOp op2) {
  return op1 == op2;
}

static enum zSystemHintOp UNDEFINED(zSystemHintOp)(unit u) { return zSystemHintOp_NOP; }

// enum ShiftType
enum zShiftType { zShiftType_LSL, zShiftType_LSR, zShiftType_ASR, zShiftType_ROR };

static bool EQUAL(zShiftType)(enum zShiftType op1, enum zShiftType op2) {
  return op1 == op2;
}

static enum zShiftType UNDEFINED(zShiftType)(unit u) { return zShiftType_LSL; }

// enum ReduceOp
enum zReduceOp { zReduceOp_FMINNUM, zReduceOp_FMAXNUM, zReduceOp_FMIN, zReduceOp_FMAX, zReduceOp_FADD, zReduceOp_ADD };

static bool EQUAL(zReduceOp)(enum zReduceOp op1, enum zReduceOp op2) {
  return op1 == op2;
}

static enum zReduceOp UNDEFINED(zReduceOp)(unit u) { return zReduceOp_FMINNUM; }

// struct ProcState
struct zProcState {
  uint64_t zA;
  uint64_t zC;
  uint64_t zC64;
  uint64_t zD;
  uint64_t zDIT;
  uint64_t zE;
  uint64_t zEL;
  uint64_t zF;
  uint64_t zGE;
  uint64_t zI;
  uint64_t zIL;
  uint64_t zIT;
  uint64_t zJ;
  uint64_t zM;
  uint64_t zN;
  uint64_t zPAN;
  uint64_t zQ;
  uint64_t zSP;
  uint64_t zSS;
  uint64_t zSSBS;
  uint64_t zT;
  uint64_t zTCO;
  uint64_t zUAO;
  uint64_t zV;
  uint64_t zZ;
  uint64_t znRW;
};

static void COPY(zProcState)(struct zProcState *rop, const struct zProcState op) {
  rop->zA = op.zA;
  rop->zC = op.zC;
  rop->zC64 = op.zC64;
  rop->zD = op.zD;
  rop->zDIT = op.zDIT;
  rop->zE = op.zE;
  rop->zEL = op.zEL;
  rop->zF = op.zF;
  rop->zGE = op.zGE;
  rop->zI = op.zI;
  rop->zIL = op.zIL;
  rop->zIT = op.zIT;
  rop->zJ = op.zJ;
  rop->zM = op.zM;
  rop->zN = op.zN;
  rop->zPAN = op.zPAN;
  rop->zQ = op.zQ;
  rop->zSP = op.zSP;
  rop->zSS = op.zSS;
  rop->zSSBS = op.zSSBS;
  rop->zT = op.zT;
  rop->zTCO = op.zTCO;
  rop->zUAO = op.zUAO;
  rop->zV = op.zV;
  rop->zZ = op.zZ;
  rop->znRW = op.znRW;
}

static bool EQUAL(zProcState)(struct zProcState op1, struct zProcState op2) {
  return EQUAL(fbits)(op1.zA, op2.zA) && EQUAL(fbits)(op1.zC, op2.zC) && EQUAL(fbits)(op1.zC64, op2.zC64) && EQUAL(fbits)(op1.zD, op2.zD) && EQUAL(fbits)(op1.zDIT, op2.zDIT) && EQUAL(fbits)(op1.zE, op2.zE) && EQUAL(fbits)(op1.zEL, op2.zEL) && EQUAL(fbits)(op1.zF, op2.zF) && EQUAL(fbits)(op1.zGE, op2.zGE) && EQUAL(fbits)(op1.zI, op2.zI) && EQUAL(fbits)(op1.zIL, op2.zIL) && EQUAL(fbits)(op1.zIT, op2.zIT) && EQUAL(fbits)(op1.zJ, op2.zJ) && EQUAL(fbits)(op1.zM, op2.zM) && EQUAL(fbits)(op1.zN, op2.zN) && EQUAL(fbits)(op1.zPAN, op2.zPAN) && EQUAL(fbits)(op1.zQ, op2.zQ) && EQUAL(fbits)(op1.zSP, op2.zSP) && EQUAL(fbits)(op1.zSS, op2.zSS) && EQUAL(fbits)(op1.zSSBS, op2.zSSBS) && EQUAL(fbits)(op1.zT, op2.zT) && EQUAL(fbits)(op1.zTCO, op2.zTCO) && EQUAL(fbits)(op1.zUAO, op2.zUAO) && EQUAL(fbits)(op1.zV, op2.zV) && EQUAL(fbits)(op1.zZ, op2.zZ) && EQUAL(fbits)(op1.znRW, op2.znRW);
}

// enum PrefetchHint
enum zPrefetchHint { zPrefetch_READ, zPrefetch_WRITE, zPrefetch_EXEC };

static bool EQUAL(zPrefetchHint)(enum zPrefetchHint op1, enum zPrefetchHint op2) {
  return op1 == op2;
}

static enum zPrefetchHint UNDEFINED(zPrefetchHint)(unit u) { return zPrefetch_READ; }

// enum PSTATEField
enum zPSTATEField { zPSTATEField_DAIFSet, zPSTATEField_DAIFClr, zPSTATEField_PAN, zPSTATEField_UAO, zPSTATEField_SSBS, zPSTATEField_SP };

static bool EQUAL(zPSTATEField)(enum zPSTATEField op1, enum zPSTATEField op2) {
  return op1 == op2;
}

static enum zPSTATEField UNDEFINED(zPSTATEField)(unit u) { return zPSTATEField_DAIFSet; }

// enum MoveWideOp
enum zMoveWideOp { zMoveWideOp_N, zMoveWideOp_Z, zMoveWideOp_K };

static bool EQUAL(zMoveWideOp)(enum zMoveWideOp op1, enum zMoveWideOp op2) {
  return op1 == op2;
}

static enum zMoveWideOp UNDEFINED(zMoveWideOp)(unit u) { return zMoveWideOp_N; }

// enum MemType
enum zMemType { zMemType_Normal, zMemType_Device };

static bool EQUAL(zMemType)(enum zMemType op1, enum zMemType op2) {
  return op1 == op2;
}

static enum zMemType UNDEFINED(zMemType)(unit u) { return zMemType_Normal; }

// enum MemOp
enum zMemOp { zMemOp_LOAD, zMemOp_STORE, zMemOp_PREFETCH };

static bool EQUAL(zMemOp)(enum zMemOp op1, enum zMemOp op2) {
  return op1 == op2;
}

static enum zMemOp UNDEFINED(zMemOp)(unit u) { return zMemOp_LOAD; }

// enum MemAtomicOp
enum zMemAtomicOp { zMemAtomicOp_ADD, zMemAtomicOp_BIC, zMemAtomicOp_EOR, zMemAtomicOp_ORR, zMemAtomicOp_SMAX, zMemAtomicOp_SMIN, zMemAtomicOp_UMAX, zMemAtomicOp_UMIN, zMemAtomicOp_SWP };

static bool EQUAL(zMemAtomicOp)(enum zMemAtomicOp op1, enum zMemAtomicOp op2) {
  return op1 == op2;
}

static enum zMemAtomicOp UNDEFINED(zMemAtomicOp)(unit u) { return zMemAtomicOp_ADD; }

// enum MBReqTypes
enum zMBReqTypes { zMBReqTypes_Reads, zMBReqTypes_Writes, zMBReqTypes_All };

static bool EQUAL(zMBReqTypes)(enum zMBReqTypes op1, enum zMBReqTypes op2) {
  return op1 == op2;
}

static enum zMBReqTypes UNDEFINED(zMBReqTypes)(unit u) { return zMBReqTypes_Reads; }

// enum MBReqDomain
enum zMBReqDomain { zMBReqDomain_Nonshareable, zMBReqDomain_InnerShareable, zMBReqDomain_OuterShareable, zMBReqDomain_FullSystem };

static bool EQUAL(zMBReqDomain)(enum zMBReqDomain op1, enum zMBReqDomain op2) {
  return op1 == op2;
}

static enum zMBReqDomain UNDEFINED(zMBReqDomain)(unit u) { return zMBReqDomain_Nonshareable; }

// enum LogicalOp
enum zLogicalOp { zLogicalOp_AND, zLogicalOp_EOR, zLogicalOp_ORR };

static bool EQUAL(zLogicalOp)(enum zLogicalOp op1, enum zLogicalOp op2) {
  return op1 == op2;
}

static enum zLogicalOp UNDEFINED(zLogicalOp)(unit u) { return zLogicalOp_AND; }

// enum ImmediateOp
enum zImmediateOp { zImmediateOp_MOVI, zImmediateOp_MVNI, zImmediateOp_ORR, zImmediateOp_BIC };

static bool EQUAL(zImmediateOp)(enum zImmediateOp op1, enum zImmediateOp op2) {
  return op1 == op2;
}

static enum zImmediateOp UNDEFINED(zImmediateOp)(unit u) { return zImmediateOp_MOVI; }

// enum Fault
enum zFault { zFault_None, zFault_AccessFlag, zFault_Alignment, zFault_Background, zFault_Domain, zFault_Permission, zFault_Translation, zFault_AddressSizze, zFault_SyncExternal, zFault_SyncExternalOnWalk, zFault_SyncParity, zFault_SyncParityOnWalk, zFault_AsyncParity, zFault_AsyncExternal, zFault_Debug, zFault_TLBConflict, zFault_HWUpdateAccessFlag, zFault_CapTag, zFault_CapSeal, zFault_CapBounds, zFault_CapPerm, zFault_CapPagePerm, zFault_Lockdown, zFault_Exclusive, zFault_ICacheMaint };

static bool EQUAL(zFault)(enum zFault op1, enum zFault op2) {
  return op1 == op2;
}

static enum zFault UNDEFINED(zFault)(unit u) { return zFault_None; }

// enum FPUnaryOp
enum zFPUnaryOp { zFPUnaryOp_ABS, zFPUnaryOp_MOV, zFPUnaryOp_NEG, zFPUnaryOp_SQRT };

static bool EQUAL(zFPUnaryOp)(enum zFPUnaryOp op1, enum zFPUnaryOp op2) {
  return op1 == op2;
}

static enum zFPUnaryOp UNDEFINED(zFPUnaryOp)(unit u) { return zFPUnaryOp_ABS; }

// enum FPType
enum zFPType { zFPType_Nonzzero, zFPType_Zero, zFPType_Infinity, zFPType_QNaN, zFPType_SNaN };

static bool EQUAL(zFPType)(enum zFPType op1, enum zFPType op2) {
  return op1 == op2;
}

static enum zFPType UNDEFINED(zFPType)(unit u) { return zFPType_Nonzzero; }

// enum FPRounding
enum zFPRounding { zFPRounding_TIEEVEN, zFPRounding_POSINF, zFPRounding_NEGINF, zFPRounding_ZERO, zFPRounding_TIEAWAY, zFPRounding_ODD };

static bool EQUAL(zFPRounding)(enum zFPRounding op1, enum zFPRounding op2) {
  return op1 == op2;
}

static enum zFPRounding UNDEFINED(zFPRounding)(unit u) { return zFPRounding_TIEEVEN; }

// enum FPMaxMinOp
enum zFPMaxMinOp { zFPMaxMinOp_MAX, zFPMaxMinOp_MIN, zFPMaxMinOp_MAXNUM, zFPMaxMinOp_MINNUM };

static bool EQUAL(zFPMaxMinOp)(enum zFPMaxMinOp op1, enum zFPMaxMinOp op2) {
  return op1 == op2;
}

static enum zFPMaxMinOp UNDEFINED(zFPMaxMinOp)(unit u) { return zFPMaxMinOp_MAX; }

// enum FPExc
enum zFPExc { zFPExc_InvalidOp, zFPExc_DivideByZero, zFPExc_Overflow, zFPExc_Underflow, zFPExc_Inexact, zFPExc_InputDenorm };

static bool EQUAL(zFPExc)(enum zFPExc op1, enum zFPExc op2) {
  return op1 == op2;
}

static enum zFPExc UNDEFINED(zFPExc)(unit u) { return zFPExc_InvalidOp; }

// enum FPConvOp
enum zFPConvOp { zFPConvOp_CVT_FtoI, zFPConvOp_CVT_ItoF, zFPConvOp_MOV_FtoI, zFPConvOp_MOV_ItoF };

static bool EQUAL(zFPConvOp)(enum zFPConvOp op1, enum zFPConvOp op2) {
  return op1 == op2;
}

static enum zFPConvOp UNDEFINED(zFPConvOp)(unit u) { return zFPConvOp_CVT_FtoI; }

// enum ExtendType
enum zExtendType { zExtendType_SXTB, zExtendType_SXTH, zExtendType_SXTW, zExtendType_SXTX, zExtendType_UXTB, zExtendType_UXTH, zExtendType_UXTW, zExtendType_UXTX };

static bool EQUAL(zExtendType)(enum zExtendType op1, enum zExtendType op2) {
  return op1 == op2;
}

static enum zExtendType UNDEFINED(zExtendType)(unit u) { return zExtendType_SXTB; }

// enum Exception
enum zException { zException_Uncategorizzed, zException_WFxTrap, zException_CP15RTTrap, zException_CP15RRTTrap, zException_CP14RTTrap, zException_CP14DTTrap, zException_AdvSIMDFPAccessTrap, zException_FPIDTrap, zException_CP14RRTTrap, zException_IllegalState, zException_SupervisorCall, zException_HypervisorCall, zException_MonitorCall, zException_SystemRegisterTrap, zException_InstructionAbort, zException_PCAlignment, zException_DataAbort, zException_SPAlignment, zException_FPTrappedException, zException_SError, zException_Breakpoint, zException_SoftwareStep, zException_Watchpoint, zException_SoftwareBreakpoint, zException_VectorCatch, zException_IRQ, zException_CapabilitySysRegTrap, zException_CapabilityAccess, zException_FIQ };

static bool EQUAL(zException)(enum zException op1, enum zException op2) {
  return op1 == op2;
}

static enum zException UNDEFINED(zException)(unit u) { return zException_Uncategorizzed; }

// enum DeviceType
enum zDeviceType { zDeviceType_GRE, zDeviceType_nGRE, zDeviceType_nGnRE, zDeviceType_nGnRnE };

static bool EQUAL(zDeviceType)(enum zDeviceType op1, enum zDeviceType op2) {
  return op1 == op2;
}

static enum zDeviceType UNDEFINED(zDeviceType)(unit u) { return zDeviceType_GRE; }

// enum CrossTriggerIn
enum zCrossTriggerIn { zCrossTriggerIn_CrossHalt, zCrossTriggerIn_PMUOverflow, zCrossTriggerIn_RSVD2, zCrossTriggerIn_RSVD3, zCrossTriggerIn_TraceExtOut0, zCrossTriggerIn_TraceExtOut1, zCrossTriggerIn_TraceExtOut2, zCrossTriggerIn_TraceExtOut3 };

static bool EQUAL(zCrossTriggerIn)(enum zCrossTriggerIn op1, enum zCrossTriggerIn op2) {
  return op1 == op2;
}

static enum zCrossTriggerIn UNDEFINED(zCrossTriggerIn)(unit u) { return zCrossTriggerIn_CrossHalt; }

// enum CountOp
enum zCountOp { zCountOp_CLZ, zCountOp_CLS, zCountOp_CNT };

static bool EQUAL(zCountOp)(enum zCountOp op1, enum zCountOp op2) {
  return op1 == op2;
}

static enum zCountOp UNDEFINED(zCountOp)(unit u) { return zCountOp_CLZ; }

// enum Constraint
enum zConstraint { zConstraint_NONE, zConstraint_UNKNOWN, zConstraint_UNDEF, zConstraint_UNDEFEL0, zConstraint_NOP, zConstraint_TRUE, zConstraint_FALSE, zConstraint_DISABLED, zConstraint_UNCOND, zConstraint_COND, zConstraint_ADDITIONAL_DECODE, zConstraint_WBSUPPRESS, zConstraint_FAULT, zConstraint_FORCE, zConstraint_FORCENOSLCHECK };

static bool EQUAL(zConstraint)(enum zConstraint op1, enum zConstraint op2) {
  return op1 == op2;
}

static enum zConstraint UNDEFINED(zConstraint)(unit u) { return zConstraint_NONE; }

// enum CompareOp
enum zCompareOp { zCompareOp_GT, zCompareOp_GE, zCompareOp_EQ, zCompareOp_LE, zCompareOp_LT };

static bool EQUAL(zCompareOp)(enum zCompareOp op1, enum zCompareOp op2) {
  return op1 == op2;
}

static enum zCompareOp UNDEFINED(zCompareOp)(unit u) { return zCompareOp_GT; }

// enum BranchType
enum zBranchType { zBranchType_DIRCALL, zBranchType_INDCALL, zBranchType_ERET, zBranchType_DBGEXIT, zBranchType_RET, zBranchType_DIR, zBranchType_INDIR, zBranchType_EXCEPTION, zBranchType_RESET, zBranchType_UNKNOWN };

static bool EQUAL(zBranchType)(enum zBranchType op1, enum zBranchType op2) {
  return op1 == op2;
}

static enum zBranchType UNDEFINED(zBranchType)(unit u) { return zBranchType_DIRCALL; }

// enum ArchVersion
enum zArchVersion { zARMv8p0, zARMv8p1, zARMv8p2 };

static bool EQUAL(zArchVersion)(enum zArchVersion op1, enum zArchVersion op2) {
  return op1 == op2;
}

static enum zArchVersion UNDEFINED(zArchVersion)(unit u) { return zARMv8p0; }

// enum AccType
enum zAccType { zAccType_NORMAL, zAccType_VEC, zAccType_STREAM, zAccType_VECSTREAM, zAccType_ATOMIC, zAccType_ATOMICRW, zAccType_ORDERED, zAccType_ORDEREDRW, zAccType_ORDEREDATOMIC, zAccType_ORDEREDATOMICRW, zAccType_LIMITEDORDERED, zAccType_UNPRIV, zAccType_IFETCH, zAccType_PTW, zAccType_DC, zAccType_DC_UNPRIV, zAccType_IC, zAccType_DCZVA, zAccType_AT };

static bool EQUAL(zAccType)(enum zAccType op1, enum zAccType op2) {
  return op1 == op2;
}

static enum zAccType UNDEFINED(zAccType)(unit u) { return zAccType_NORMAL; }

















static bool sailgen_neq_int(sail_int, sail_int);

static bool sailgen_neq_int(sail_int zx, sail_int zy)
{
  __label__ end_function_1, end_block_exception_2;

  bool zcbz30;
  bool zgaz30;
  zgaz30 = eq_int(zx, zy);
  zcbz30 = not(zgaz30);

end_function_1: ;
  return zcbz30;
end_block_exception_2: ;

  return false;
}









static void sailgen___id(sail_int *rop, sail_int);

static void sailgen___id(sail_int *zcbz31, sail_int zx)
{
  __label__ end_function_4, end_block_exception_5, end_function_287;

  COPY(sail_int)((*(&zcbz31)), zx);
end_function_4: ;
  goto end_function_287;
end_block_exception_5: ;
  goto end_function_287;
end_function_287: ;
}













static bool sailgen_neq_bits(lbits zx, lbits zy)
{
  __label__ end_function_7, end_block_exception_8;

  bool zcbz32;
  bool zgaz31;
  zgaz31 = eq_bits(zx, zy);
  zcbz32 = not(zgaz31);

end_function_7: ;
  return zcbz32;
end_block_exception_8: ;

  return false;
}































static void sailgen_sail_ones(lbits *rop, sail_int);

lbits zghz30;

static void startup_sailgen_sail_ones(void)
{    CREATE(lbits)(&zghz30);
}

static void sailgen_sail_ones(lbits *zcbz33, sail_int zn)
{
  __label__ end_function_10, end_block_exception_11, end_function_286;

  RECREATE(lbits)(&zghz30);
  zeros(&zghz30, zn);
  not_bits((*(&zcbz33)), zghz30);
end_function_10: ;
  goto end_function_286;
end_block_exception_11: ;
  goto end_function_286;
end_function_286: ;
}



static void finish_sailgen_sail_ones(void)
{    KILL(lbits)(&zghz30);
}









static bool sailgen_eq_bits_int(lbits, sail_int);

sail_int zghz31;

static void startup_sailgen_eq_bits_int(void)
{    CREATE(sail_int)(&zghz31);
}

static bool sailgen_eq_bits_int(lbits zx, sail_int zy)
{
  __label__ end_function_13, end_block_exception_14;

  bool zcbz34;
  RECREATE(sail_int)(&zghz31);
  sail_unsigned(&zghz31, zx);
  zcbz34 = eq_int(zghz31, zy);
end_function_13: ;
  return zcbz34;
end_block_exception_14: ;

  return false;
}



static void finish_sailgen_eq_bits_int(void)
{    KILL(sail_int)(&zghz31);
}



static void sailgen_Ones(lbits *rop, sail_int);

static void sailgen_Ones(lbits *zcbz35, sail_int zn)
{
  __label__ end_function_16, end_block_exception_17, end_function_285;

  sailgen_sail_ones((*(&zcbz35)), zn);
end_function_16: ;
  goto end_function_285;
end_block_exception_17: ;
  goto end_function_285;
end_function_285: ;
}

static void sailgen_Zeros(lbits *rop, sail_int);

static void sailgen_Zeros(lbits *zcbz36, sail_int zn)
{
  __label__ end_function_19, end_block_exception_20, end_function_284;

  zeros((*(&zcbz36)), zn);
end_function_19: ;
  goto end_function_284;
end_block_exception_20: ;
  goto end_function_284;
end_function_284: ;
}

static fbits sailgen_Bit(uint64_t);

static fbits sailgen_Bit(uint64_t zb)
{
  __label__ end_function_22, end_block_exception_23;

  fbits zcbz37;
  zcbz37 = (UINT64_C(1) & (zb >> INT64_C(0)));
end_function_22: ;
  return zcbz37;
end_block_exception_23: ;

  return UINT64_C(0);
}

static void sailgen_integer_subrange(lbits *rop, sail_int, sail_int, sail_int);

sail_int zghz32;
sail_int zghz33;
sail_int zghz34;

static void startup_sailgen_integer_subrange(void)
{
  CREATE(sail_int)(&zghz32);
  CREATE(sail_int)(&zghz33);
  CREATE(sail_int)(&zghz34);
}

static void sailgen_integer_subrange(lbits *zcbz38, sail_int zi, sail_int zhi, sail_int zlo)
{
  __label__ end_function_25, end_block_exception_26, end_function_283;

  RECREATE(sail_int)(&zghz32);
  {
    RECREATE(sail_int)(&zghz33);
    sub_int(&zghz33, zhi, zlo);
    {
      RECREATE(sail_int)(&zghz34);
      CONVERT_OF(sail_int, mach_int)(&zghz34, INT64_C(1));
      add_int(&zghz32, zghz33, zghz34);
    }
  }
  get_slice_int((*(&zcbz38)), zghz32, zi, zlo);
end_function_25: ;
  goto end_function_283;
end_block_exception_26: ;
  goto end_function_283;
end_function_283: ;
}



static void finish_sailgen_integer_subrange(void)
{
  KILL(sail_int)(&zghz34);
  KILL(sail_int)(&zghz33);
  KILL(sail_int)(&zghz32);
}







static void sailgen_Slice_int(lbits *rop, sail_int, sail_int, sail_int);

static void sailgen_Slice_int(lbits *zcbz39, sail_int zi, sail_int zl, sail_int zn)
{
  __label__ end_function_28, end_block_exception_29, end_function_282;

  get_slice_int((*(&zcbz39)), zn, zi, zl);
end_function_28: ;
  goto end_function_282;
end_block_exception_29: ;
  goto end_function_282;
end_function_282: ;
}

static enum zsignal sailgen_undefined_signal(unit);

static enum zsignal sailgen_undefined_signal(unit zgsz30)
{
  __label__ end_function_31, end_block_exception_32;

  enum zsignal zcbz310;
  zcbz310 = zLOW;
end_function_31: ;
  return zcbz310;
end_block_exception_32: ;

  return zHIGH;
}

static unit sailgen___SetConfig(const_sail_string, sail_int);

static unit sailgen___ListConfig(unit);

// register SEE
static sail_int zSEE;

// register PCC
static lbits zPCC;

// register CNTHPS_CTL_EL2
static uint64_t zCNTHPS_CTL_EL2;

// register CNTHPS_CVAL_EL2
static uint64_t zCNTHPS_CVAL_EL2;

// register CNTHPS_TVAL_EL2
static uint64_t zCNTHPS_TVAL_EL2;

// register CNTHVS_CTL_EL2
static uint64_t zCNTHVS_CTL_EL2;

// register CNTHVS_CVAL_EL2
static uint64_t zCNTHVS_CVAL_EL2;

// register CNTHVS_TVAL_EL2
static uint64_t zCNTHVS_TVAL_EL2;

static uint64_t zEL1;
static void create_letbind_0(void) {


  uint64_t zgsz31;
  zgsz31 = UINT64_C(0b01);
  zEL1 = zgsz31;

let_end_33: ;
}
static void kill_letbind_0(void) {
}

static uint64_t zEL2;
static void create_letbind_1(void) {


  uint64_t zgsz32;
  zgsz32 = UINT64_C(0b10);
  zEL2 = zgsz32;

let_end_34: ;
}
static void kill_letbind_1(void) {
}

static uint64_t zEL3;
static void create_letbind_2(void) {


  uint64_t zgsz33;
  zgsz33 = UINT64_C(0b11);
  zEL3 = zgsz33;

let_end_35: ;
}
static void kill_letbind_2(void) {
}

// register SCR_EL3
static uint64_t zSCR_EL3;

// register HCR_EL2
static uint64_t zHCR_EL2;

static struct zProcState sailgen_undefined_ProcState(unit);

struct zProcState zghz35;

static void startup_sailgen_undefined_ProcState(void)
{
}

static struct zProcState sailgen_undefined_ProcState(unit zgsz34)
{
  __label__ end_function_37, end_block_exception_38;

  struct zProcState zghz35;
  zghz35.zA = UINT64_C(0b0);
  zghz35.zC = UINT64_C(0b0);
  zghz35.zC64 = UINT64_C(0b0);
  zghz35.zD = UINT64_C(0b0);
  zghz35.zDIT = UINT64_C(0b0);
  zghz35.zE = UINT64_C(0b0);
  zghz35.zEL = UINT64_C(0b00);
  zghz35.zF = UINT64_C(0b0);
  zghz35.zGE = UINT64_C(0x0);
  zghz35.zI = UINT64_C(0b0);
  zghz35.zIL = UINT64_C(0b0);
  zghz35.zIT = UINT64_C(0x00);
  zghz35.zJ = UINT64_C(0b0);
  zghz35.zM = UINT64_C(0b00000);
  zghz35.zN = UINT64_C(0b0);
  zghz35.zPAN = UINT64_C(0b0);
  zghz35.zQ = UINT64_C(0b0);
  zghz35.zSP = UINT64_C(0b0);
  zghz35.zSS = UINT64_C(0b0);
  zghz35.zSSBS = UINT64_C(0b0);
  zghz35.zT = UINT64_C(0b0);
  zghz35.zTCO = UINT64_C(0b0);
  zghz35.zUAO = UINT64_C(0b0);
  zghz35.zV = UINT64_C(0b0);
  zghz35.zZ = UINT64_C(0b0);
  zghz35.znRW = UINT64_C(0b0);
end_function_37: ;
  return zghz35;
end_block_exception_38: ;
  struct zProcState zcbz338 = { .zA = UINT64_C(0xdeadc0de), .zC = UINT64_C(0xdeadc0de), .zC64 = UINT64_C(0xdeadc0de), .zD = UINT64_C(0xdeadc0de), .zDIT = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zEL = UINT64_C(0xdeadc0de), .zF = UINT64_C(0xdeadc0de), .zGE = UINT64_C(0xdeadc0de), .zI = UINT64_C(0xdeadc0de), .zIL = UINT64_C(0xdeadc0de), .zIT = UINT64_C(0xdeadc0de), .zJ = UINT64_C(0xdeadc0de), .zM = UINT64_C(0xdeadc0de), .zN = UINT64_C(0xdeadc0de), .zPAN = UINT64_C(0xdeadc0de), .zQ = UINT64_C(0xdeadc0de), .zSP = UINT64_C(0xdeadc0de), .zSS = UINT64_C(0xdeadc0de), .zSSBS = UINT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zTCO = UINT64_C(0xdeadc0de), .zUAO = UINT64_C(0xdeadc0de), .zV = UINT64_C(0xdeadc0de), .zZ = UINT64_C(0xdeadc0de), .znRW = UINT64_C(0xdeadc0de) };
  return zcbz338;
}



static void finish_sailgen_undefined_ProcState(void)
{
}

// register PSTATE
static struct zProcState zPSTATE;

static uint64_t zEL0;
static void create_letbind_3(void) {


  uint64_t zgsz36;
  zgsz36 = UINT64_C(0b00);
  zEL0 = zgsz36;

let_end_39: ;
}
static void kill_letbind_3(void) {
}

// register _R00
static lbits z_R00;

// register _R01
static lbits z_R01;

// register _R02
static lbits z_R02;

// register _R03
static lbits z_R03;

// register _R04
static lbits z_R04;

// register _R05
static lbits z_R05;

// register _R06
static lbits z_R06;

// register _R07
static lbits z_R07;

// register _R08
static lbits z_R08;

// register _R09
static lbits z_R09;

// register _R10
static lbits z_R10;

// register _R11
static lbits z_R11;

// register _R12
static lbits z_R12;

// register _R13
static lbits z_R13;

// register _R14
static lbits z_R14;

// register _R15
static lbits z_R15;

// register _R16
static lbits z_R16;

// register _R17
static lbits z_R17;

// register _R18
static lbits z_R18;

// register _R19
static lbits z_R19;

// register _R20
static lbits z_R20;

// register _R21
static lbits z_R21;

// register _R22
static lbits z_R22;

// register _R23
static lbits z_R23;

// register _R24
static lbits z_R24;

// register _R25
static lbits z_R25;

// register _R26
static lbits z_R26;

// register _R27
static lbits z_R27;

// register _R28
static lbits z_R28;

// register _R29
static lbits z_R29;

// register _R30
static lbits z_R30;

// register ESR_EL1
static uint64_t zESR_EL1;

// register ESR_EL2
static uint64_t zESR_EL2;

// register ESR_EL3
static uint64_t zESR_EL3;

// register FAR_EL1
static uint64_t zFAR_EL1;

// register FAR_EL2
static uint64_t zFAR_EL2;

// register FAR_EL3
static uint64_t zFAR_EL3;

// register HPFAR_EL2
static uint64_t zHPFAR_EL2;

// register TCR_EL1
static uint64_t zTCR_EL1;

// register TCR_EL2
static uint64_t zTCR_EL2;

// register TCR_EL3
static uint64_t zTCR_EL3;

static int64_t zCAP_FLAGS_LO_BIT;
static void create_letbind_4(void) {


  int64_t zgsz37;
  zgsz37 = INT64_C(56);
  zCAP_FLAGS_LO_BIT = zgsz37;

let_end_40: ;
}
static void kill_letbind_4(void) {
}

static int64_t zCAP_VALUE_HI_BIT;
static void create_letbind_5(void) {


  int64_t zgsz38;
  zgsz38 = INT64_C(63);
  zCAP_VALUE_HI_BIT = zgsz38;

let_end_41: ;
}
static void kill_letbind_5(void) {
}

static int64_t zCAP_VALUE_LO_BIT;
static void create_letbind_6(void) {


  int64_t zgsz39;
  zgsz39 = INT64_C(0);
  zCAP_VALUE_LO_BIT = zgsz39;

let_end_42: ;
}
static void kill_letbind_6(void) {
}

static int64_t zCAP_VALUE_NUM_BITS;
static void create_letbind_7(void) {


  int64_t zgsz310;
  int64_t zgaz36;
  {
    sail_int zgsz3669;
    CREATE(sail_int)(&zgsz3669);
    CONVERT_OF(sail_int, mach_int)(&zgsz3669, zCAP_VALUE_HI_BIT);
    sail_int zgsz3670;
    CREATE(sail_int)(&zgsz3670);
    CONVERT_OF(sail_int, mach_int)(&zgsz3670, zCAP_VALUE_LO_BIT);
    sail_int zgsz3671;
    CREATE(sail_int)(&zgsz3671);
    sub_int(&zgsz3671, zgsz3669, zgsz3670);
    zgaz36 = CONVERT_OF(mach_int, sail_int)(zgsz3671);
    KILL(sail_int)(&zgsz3671);
    KILL(sail_int)(&zgsz3670);
    KILL(sail_int)(&zgsz3669);
  }
  zgsz310 = (zgaz36 + INT64_C(1));

  zCAP_VALUE_NUM_BITS = zgsz310;

let_end_43: ;
}
static void kill_letbind_7(void) {
}

static int64_t zCAP_BASE_HI_BIT;
static void create_letbind_8(void) {


  int64_t zgsz311;
  zgsz311 = INT64_C(79);
  zCAP_BASE_HI_BIT = zgsz311;

let_end_44: ;
}
static void kill_letbind_8(void) {
}

static int64_t zCAP_BASE_LO_BIT;
static void create_letbind_9(void) {


  int64_t zgsz312;
  zgsz312 = INT64_C(64);
  zCAP_BASE_LO_BIT = zgsz312;

let_end_45: ;
}
static void kill_letbind_9(void) {
}

static int64_t zCAP_MW;
static void create_letbind_10(void) {


  int64_t zgsz313;
  int64_t zgaz37;
  {
    sail_int zgsz3672;
    CREATE(sail_int)(&zgsz3672);
    CONVERT_OF(sail_int, mach_int)(&zgsz3672, zCAP_BASE_HI_BIT);
    sail_int zgsz3673;
    CREATE(sail_int)(&zgsz3673);
    CONVERT_OF(sail_int, mach_int)(&zgsz3673, zCAP_BASE_LO_BIT);
    sail_int zgsz3674;
    CREATE(sail_int)(&zgsz3674);
    sub_int(&zgsz3674, zgsz3672, zgsz3673);
    zgaz37 = CONVERT_OF(mach_int, sail_int)(zgsz3674);
    KILL(sail_int)(&zgsz3674);
    KILL(sail_int)(&zgsz3673);
    KILL(sail_int)(&zgsz3672);
  }
  zgsz313 = (zgaz37 + INT64_C(1));

  zCAP_MW = zgsz313;

let_end_46: ;
}
static void kill_letbind_10(void) {
}

static bool sailgen_CapBoundsUsesValue(sail_int);

sail_int zghz36;
sail_int zghz37;
sail_int zghz38;

static void startup_sailgen_CapBoundsUsesValue(void)
{
  CREATE(sail_int)(&zghz36);
  CREATE(sail_int)(&zghz37);
  CREATE(sail_int)(&zghz38);
}

static bool sailgen_CapBoundsUsesValue(sail_int zexp)
{
  __label__ cleanup_49, end_cleanup_50, end_function_48, end_block_exception_51;

  bool zcbz312;
  {
    RECREATE(sail_int)(&zghz36);
    {
      RECREATE(sail_int)(&zghz38);
      CONVERT_OF(sail_int, mach_int)(&zghz38, zCAP_MW);
      add_int(&zghz36, zexp, zghz38);
    }
    {
      RECREATE(sail_int)(&zghz37);
      CONVERT_OF(sail_int, mach_int)(&zghz37, zCAP_VALUE_NUM_BITS);
      zcbz312 = lt(zghz36, zghz37);
    }
  }
  goto cleanup_49;
  /* unreachable after return */
  goto end_cleanup_50;
cleanup_49: ;
  goto end_function_48;
end_cleanup_50: ;
end_function_48: ;
  return zcbz312;
end_block_exception_51: ;

  return false;
}



static void finish_sailgen_CapBoundsUsesValue(void)
{
  KILL(sail_int)(&zghz38);
  KILL(sail_int)(&zghz37);
  KILL(sail_int)(&zghz36);
}

static int64_t zCAP_BASE_EXP_HI_BIT;
static void create_letbind_11(void) {


  int64_t zgsz314;
  zgsz314 = INT64_C(66);
  zCAP_BASE_EXP_HI_BIT = zgsz314;

let_end_52: ;
}
static void kill_letbind_11(void) {
}

static int64_t zCAP_LIMIT_EXP_HI_BIT;
static void create_letbind_12(void) {


  int64_t zgsz315;
  zgsz315 = INT64_C(82);
  zCAP_LIMIT_EXP_HI_BIT = zgsz315;

let_end_53: ;
}
static void kill_letbind_12(void) {
}

static int64_t zCAP_LIMIT_LO_BIT;
static void create_letbind_13(void) {


  int64_t zgsz316;
  zgsz316 = INT64_C(80);
  zCAP_LIMIT_LO_BIT = zgsz316;

let_end_54: ;
}
static void kill_letbind_13(void) {
}

static int64_t zCAP_IE_BIT;
static void create_letbind_14(void) {


  int64_t zgsz317;
  zgsz317 = INT64_C(94);
  zCAP_IE_BIT = zgsz317;

let_end_55: ;
}
static void kill_letbind_14(void) {
}

static bool sailgen_CapIsInternalExponent(lbits);

sail_int zghz39;

static void startup_sailgen_CapIsInternalExponent(void)
{    CREATE(sail_int)(&zghz39);
}

static bool sailgen_CapIsInternalExponent(lbits zc)
{
  __label__ cleanup_58, end_cleanup_59, end_function_57, end_block_exception_60;

  bool zcbz313;
  {
    uint64_t zgaz311;
    {
      fbits zgaz310;
      {
        RECREATE(sail_int)(&zghz39);
        CONVERT_OF(sail_int, mach_int)(&zghz39, zCAP_IE_BIT);
        zgaz310 = bitvector_access(zc, zghz39);
      }
      zgaz311 = UINT64_C(0b0);
      zgaz311 = update_fbits(zgaz311, INT64_C(0), zgaz310);
    }
    zcbz313 = (zgaz311 == UINT64_C(0b0));
  }
  goto cleanup_58;
  /* unreachable after return */
  goto end_cleanup_59;
cleanup_58: ;
  goto end_function_57;
end_cleanup_59: ;
end_function_57: ;
  return zcbz313;
end_block_exception_60: ;

  return false;
}



static void finish_sailgen_CapIsInternalExponent(void)
{    KILL(sail_int)(&zghz39);
}

static int64_t sailgen_CapGetExponent(lbits);

lbits zghz310;
sail_int zghz311;
sail_int zghz312;
sail_int zghz313;
lbits zghz314;
sail_int zghz315;
sail_int zghz316;
lbits zghz317;

static void startup_sailgen_CapGetExponent(void)
{
  CREATE(lbits)(&zghz310);
  CREATE(sail_int)(&zghz311);
  CREATE(sail_int)(&zghz312);
  CREATE(sail_int)(&zghz313);
  CREATE(lbits)(&zghz314);
  CREATE(sail_int)(&zghz315);
  CREATE(sail_int)(&zghz316);
  CREATE(lbits)(&zghz317);
}

static int64_t sailgen_CapGetExponent(lbits zc)
{
  __label__ end_function_62, end_block_exception_67;

  int64_t zcbz314;
  bool zgaz313;
  zgaz313 = sailgen_CapIsInternalExponent(zc);
  if (zgaz313) {
    __label__ cleanup_65, end_cleanup_66;

    uint64_t znexp;
    {
      uint64_t zgaz316;
      {
        RECREATE(sail_int)(&zghz315);
        CONVERT_OF(sail_int, mach_int)(&zghz315, zCAP_LIMIT_EXP_HI_BIT);
        RECREATE(sail_int)(&zghz316);
        CONVERT_OF(sail_int, mach_int)(&zghz316, zCAP_LIMIT_LO_BIT);
        RECREATE(lbits)(&zghz317);
        vector_subrange_lbits(&zghz317, zc, zghz315, zghz316);
        zgaz316 = CONVERT_OF(fbits, lbits)(zghz317, true);
      }
      uint64_t zgaz317;
      {
        RECREATE(sail_int)(&zghz312);
        CONVERT_OF(sail_int, mach_int)(&zghz312, zCAP_BASE_EXP_HI_BIT);
        RECREATE(sail_int)(&zghz313);
        CONVERT_OF(sail_int, mach_int)(&zghz313, zCAP_BASE_LO_BIT);
        RECREATE(lbits)(&zghz314);
        vector_subrange_lbits(&zghz314, zc, zghz312, zghz313);
        zgaz317 = CONVERT_OF(fbits, lbits)(zghz314, true);
      }
      znexp = (zgaz316 << 3) | zgaz317;
    }
    int64_t zgaz315;
    {
      uint64_t zgaz314;
      zgaz314 = (~(znexp) & UINT64_C(0b111111));
      {
        RECREATE(lbits)(&zghz310);
        CONVERT_OF(lbits, fbits)(&zghz310, zgaz314, UINT64_C(6) , true);
        RECREATE(sail_int)(&zghz311);
        sail_unsigned(&zghz311, zghz310);
        zgaz315 = CONVERT_OF(mach_int, sail_int)(zghz311);
      }
    }
    zcbz314 = zgaz315;
    goto cleanup_65;
    /* unreachable after return */
    goto end_cleanup_66;
  cleanup_65: ;
    goto end_function_62;
  end_cleanup_66: ;
  } else {
    __label__ cleanup_63, end_cleanup_64;

    zcbz314 = INT64_C(0);
    goto cleanup_63;
    /* unreachable after return */
    goto end_cleanup_64;
  cleanup_63: ;
    goto end_function_62;
  end_cleanup_64: ;
  }

end_function_62: ;
  return zcbz314;
end_block_exception_67: ;

  return INT64_C(0xdeadc0de);
}



static void finish_sailgen_CapGetExponent(void)
{
  KILL(lbits)(&zghz317);
  KILL(sail_int)(&zghz316);
  KILL(sail_int)(&zghz315);
  KILL(lbits)(&zghz314);
  KILL(sail_int)(&zghz313);
  KILL(sail_int)(&zghz312);
  KILL(sail_int)(&zghz311);
  KILL(lbits)(&zghz310);
}

static uint64_t sailgen_CapGetValue(lbits);

sail_int zghz318;
sail_int zghz319;
lbits zghz320;

static void startup_sailgen_CapGetValue(void)
{
  CREATE(sail_int)(&zghz318);
  CREATE(sail_int)(&zghz319);
  CREATE(lbits)(&zghz320);
}

static uint64_t sailgen_CapGetValue(lbits zc)
{
  __label__ cleanup_70, end_cleanup_71, end_function_69, end_block_exception_72;

  uint64_t zcbz315;
  {
    RECREATE(sail_int)(&zghz318);
    CONVERT_OF(sail_int, mach_int)(&zghz318, zCAP_VALUE_HI_BIT);
    RECREATE(sail_int)(&zghz319);
    CONVERT_OF(sail_int, mach_int)(&zghz319, zCAP_VALUE_LO_BIT);
    RECREATE(lbits)(&zghz320);
    vector_subrange_lbits(&zghz320, zc, zghz318, zghz319);
    zcbz315 = CONVERT_OF(fbits, lbits)(zghz320, true);
  }
  goto cleanup_70;
  /* unreachable after return */
  goto end_cleanup_71;
cleanup_70: ;
  goto end_function_69;
end_cleanup_71: ;
end_function_69: ;
  return zcbz315;
end_block_exception_72: ;

  return UINT64_C(0xdeadc0de);
}



static void finish_sailgen_CapGetValue(void)
{
  KILL(lbits)(&zghz320);
  KILL(sail_int)(&zghz319);
  KILL(sail_int)(&zghz318);
}

static int64_t zCAP_BOUND_NUM_BITS;
static void create_letbind_15(void) {


  int64_t zgsz319;
  zgsz319 = (zCAP_VALUE_NUM_BITS + INT64_C(1));
  zCAP_BOUND_NUM_BITS = zgsz319;

let_end_73: ;
}
static void kill_letbind_15(void) {
}

static lbits zCAP_BOUND_MAX;
static void create_letbind_16(void) {    CREATE(lbits)(&zCAP_BOUND_MAX);


  lbits zgsz320;
  CREATE(lbits)(&zgsz320);
  sail_int zgaz319;
  CREATE(sail_int)(&zgaz319);
  {
    sail_int zgsz3691;
    CREATE(sail_int)(&zgsz3691);
    CONVERT_OF(sail_int, mach_int)(&zgsz3691, INT64_C(1));
    sail_int zgsz3692;
    CREATE(sail_int)(&zgsz3692);
    CONVERT_OF(sail_int, mach_int)(&zgsz3692, zCAP_VALUE_NUM_BITS);
    shl_int(&zgaz319, zgsz3691, zgsz3692);
    KILL(sail_int)(&zgsz3692);
    KILL(sail_int)(&zgsz3691);
  }
  {
    sail_int zgsz3689;
    CREATE(sail_int)(&zgsz3689);
    CONVERT_OF(sail_int, mach_int)(&zgsz3689, INT64_C(0));
    sail_int zgsz3690;
    CREATE(sail_int)(&zgsz3690);
    CONVERT_OF(sail_int, mach_int)(&zgsz3690, zCAP_BOUND_NUM_BITS);
    sailgen_Slice_int(&zgsz320, zgaz319, zgsz3689, zgsz3690);
    KILL(sail_int)(&zgsz3690);
    KILL(sail_int)(&zgsz3689);
  }
  KILL(sail_int)(&zgaz319);
  COPY(lbits)(&zCAP_BOUND_MAX, zgsz320);
  KILL(lbits)(&zgsz320);
let_end_74: ;
}
static void kill_letbind_16(void) {    KILL(lbits)(&zCAP_BOUND_MAX);
}

static lbits zCAP_BOUND_MIN;
static void create_letbind_17(void) {    CREATE(lbits)(&zCAP_BOUND_MIN);


  lbits zgsz321;
  CREATE(lbits)(&zgsz321);
  {
    sail_int zgsz3693;
    CREATE(sail_int)(&zgsz3693);
    CONVERT_OF(sail_int, mach_int)(&zgsz3693, INT64_C(0));
    sail_int zgsz3694;
    CREATE(sail_int)(&zgsz3694);
    CONVERT_OF(sail_int, mach_int)(&zgsz3694, INT64_C(0));
    sail_int zgsz3695;
    CREATE(sail_int)(&zgsz3695);
    CONVERT_OF(sail_int, mach_int)(&zgsz3695, zCAP_BOUND_NUM_BITS);
    sailgen_Slice_int(&zgsz321, zgsz3693, zgsz3694, zgsz3695);
    KILL(sail_int)(&zgsz3695);
    KILL(sail_int)(&zgsz3694);
    KILL(sail_int)(&zgsz3693);
  }
  COPY(lbits)(&zCAP_BOUND_MIN, zgsz321);
  KILL(lbits)(&zgsz321);
let_end_75: ;
}
static void kill_letbind_17(void) {    KILL(lbits)(&zCAP_BOUND_MIN);
}

static int64_t zCAP_MAX_ENCODEABLE_EXPONENT;
static void create_letbind_18(void) {


  int64_t zgsz322;
  zgsz322 = INT64_C(63);
  zCAP_MAX_ENCODEABLE_EXPONENT = zgsz322;

let_end_76: ;
}
static void kill_letbind_18(void) {
}

static int64_t zCAP_MAX_EXPONENT;
static void create_letbind_19(void) {


  int64_t zgsz323;
  int64_t zgaz320;
  {
    sail_int zgsz3696;
    CREATE(sail_int)(&zgsz3696);
    CONVERT_OF(sail_int, mach_int)(&zgsz3696, zCAP_VALUE_NUM_BITS);
    sail_int zgsz3697;
    CREATE(sail_int)(&zgsz3697);
    CONVERT_OF(sail_int, mach_int)(&zgsz3697, zCAP_MW);
    sail_int zgsz3698;
    CREATE(sail_int)(&zgsz3698);
    sub_int(&zgsz3698, zgsz3696, zgsz3697);
    zgaz320 = CONVERT_OF(mach_int, sail_int)(zgsz3698);
    KILL(sail_int)(&zgsz3698);
    KILL(sail_int)(&zgsz3697);
    KILL(sail_int)(&zgsz3696);
  }
  zgsz323 = (zgaz320 + INT64_C(2));

  zCAP_MAX_EXPONENT = zgsz323;

let_end_77: ;
}
static void kill_letbind_19(void) {
}

static uint64_t sailgen_CapBoundsAddress(uint64_t);

lbits zghz321;
sail_int zghz322;
lbits zghz323;
sail_int zghz324;
sail_int zghz325;
sail_int zghz326;

static void startup_sailgen_CapBoundsAddress(void)
{
  CREATE(lbits)(&zghz321);
  CREATE(sail_int)(&zghz322);
  CREATE(lbits)(&zghz323);
  CREATE(sail_int)(&zghz324);
  CREATE(sail_int)(&zghz325);
  CREATE(sail_int)(&zghz326);
}

static uint64_t sailgen_CapBoundsAddress(uint64_t zaddress)
{
  __label__ cleanup_80, end_cleanup_81, end_function_79, end_block_exception_82;

  uint64_t zcbz316;
  {
    uint64_t zgaz322;
    {
      int64_t zgaz321;
      {
        RECREATE(sail_int)(&zghz324);
        CONVERT_OF(sail_int, mach_int)(&zghz324, zCAP_FLAGS_LO_BIT);
        RECREATE(sail_int)(&zghz325);
        CONVERT_OF(sail_int, mach_int)(&zghz325, INT64_C(1));
        RECREATE(sail_int)(&zghz326);
        sub_int(&zghz326, zghz324, zghz325);
        zgaz321 = CONVERT_OF(mach_int, sail_int)(zghz326);
      }
      zgaz322 = (safe_rshift(UINT64_MAX, 64 - 56) & (zaddress >> INT64_C(0)));
    }
    {
      RECREATE(lbits)(&zghz321);
      CONVERT_OF(lbits, fbits)(&zghz321, zgaz322, UINT64_C(56) , true);
      RECREATE(sail_int)(&zghz322);
      CONVERT_OF(sail_int, mach_int)(&zghz322, zCAP_VALUE_NUM_BITS);
      RECREATE(lbits)(&zghz323);
      sign_extend(&zghz323, zghz321, zghz322);
      zcbz316 = CONVERT_OF(fbits, lbits)(zghz323, true);
    }
  }
  goto cleanup_80;
  /* unreachable after return */
  goto end_cleanup_81;
cleanup_80: ;
  goto end_function_79;
end_cleanup_81: ;
end_function_79: ;
  return zcbz316;
end_block_exception_82: ;

  return UINT64_C(0xdeadc0de);
}



static void finish_sailgen_CapBoundsAddress(void)
{
  KILL(sail_int)(&zghz326);
  KILL(sail_int)(&zghz325);
  KILL(sail_int)(&zghz324);
  KILL(lbits)(&zghz323);
  KILL(sail_int)(&zghz322);
  KILL(lbits)(&zghz321);
}

static int64_t zCAP_BASE_MANTISSA_LO_BIT;
static void create_letbind_20(void) {


  int64_t zgsz324;
  zgsz324 = INT64_C(67);
  zCAP_BASE_MANTISSA_LO_BIT = zgsz324;

let_end_83: ;
}
static void kill_letbind_20(void) {
}

static uint64_t sailgen_CapGetBottom(lbits);

sail_int zghz327;
sail_int zghz328;
lbits zghz329;
sail_int zghz330;
sail_int zghz331;
lbits zghz332;

static void startup_sailgen_CapGetBottom(void)
{
  CREATE(sail_int)(&zghz327);
  CREATE(sail_int)(&zghz328);
  CREATE(lbits)(&zghz329);
  CREATE(sail_int)(&zghz330);
  CREATE(sail_int)(&zghz331);
  CREATE(lbits)(&zghz332);
}

static uint64_t sailgen_CapGetBottom(lbits zc)
{
  __label__ end_function_85, end_block_exception_90;

  uint64_t zcbz317;
  bool zgaz324;
  zgaz324 = sailgen_CapIsInternalExponent(zc);
  if (zgaz324) {
    __label__ cleanup_88, end_cleanup_89;

    uint64_t zgaz326;
    {
      uint64_t zgaz325;
      {
        RECREATE(sail_int)(&zghz330);
        CONVERT_OF(sail_int, mach_int)(&zghz330, zCAP_BASE_HI_BIT);
        RECREATE(sail_int)(&zghz331);
        CONVERT_OF(sail_int, mach_int)(&zghz331, zCAP_BASE_MANTISSA_LO_BIT);
        RECREATE(lbits)(&zghz332);
        vector_subrange_lbits(&zghz332, zc, zghz330, zghz331);
        zgaz325 = CONVERT_OF(fbits, lbits)(zghz332, true);
      }
      zgaz326 = (zgaz325 << 3) | UINT64_C(0b000);
    }
    zcbz317 = zgaz326;
    goto cleanup_88;
    /* unreachable after return */
    goto end_cleanup_89;
  cleanup_88: ;
    goto end_function_85;
  end_cleanup_89: ;
  } else {
    __label__ cleanup_86, end_cleanup_87;

    uint64_t zgaz327;
    {
      RECREATE(sail_int)(&zghz327);
      CONVERT_OF(sail_int, mach_int)(&zghz327, zCAP_BASE_HI_BIT);
      RECREATE(sail_int)(&zghz328);
      CONVERT_OF(sail_int, mach_int)(&zghz328, zCAP_BASE_LO_BIT);
      RECREATE(lbits)(&zghz329);
      vector_subrange_lbits(&zghz329, zc, zghz327, zghz328);
      zgaz327 = CONVERT_OF(fbits, lbits)(zghz329, true);
    }
    zcbz317 = zgaz327;
    goto cleanup_86;
    /* unreachable after return */
    goto end_cleanup_87;
  cleanup_86: ;
    goto end_function_85;
  end_cleanup_87: ;
  }

end_function_85: ;
  return zcbz317;
end_block_exception_90: ;

  return UINT64_C(0xdeadc0de);
}



static void finish_sailgen_CapGetBottom(void)
{
  KILL(lbits)(&zghz332);
  KILL(sail_int)(&zghz331);
  KILL(sail_int)(&zghz330);
  KILL(lbits)(&zghz329);
  KILL(sail_int)(&zghz328);
  KILL(sail_int)(&zghz327);
}

static int64_t zCAP_LIMIT_HI_BIT;
static void create_letbind_21(void) {


  int64_t zgsz325;
  zgsz325 = INT64_C(93);
  zCAP_LIMIT_HI_BIT = zgsz325;

let_end_91: ;
}
static void kill_letbind_21(void) {
}

static int64_t zCAP_LIMIT_MANTISSA_LO_BIT;
static void create_letbind_22(void) {


  int64_t zgsz326;
  zgsz326 = INT64_C(83);
  zCAP_LIMIT_MANTISSA_LO_BIT = zgsz326;

let_end_92: ;
}
static void kill_letbind_22(void) {
}

static bool sailgen_CapUnsignedLessThan(lbits, lbits);

sail_int zghz333;
sail_int zghz334;

static void startup_sailgen_CapUnsignedLessThan(void)
{
  CREATE(sail_int)(&zghz333);
  CREATE(sail_int)(&zghz334);
}

static bool sailgen_CapUnsignedLessThan(lbits za, lbits zb)
{
  __label__ cleanup_95, end_cleanup_96, end_function_94, end_block_exception_97;

  bool zcbz318;
  {
    RECREATE(sail_int)(&zghz333);
    sail_unsigned(&zghz333, za);
    RECREATE(sail_int)(&zghz334);
    sail_unsigned(&zghz334, zb);
    zcbz318 = lt(zghz333, zghz334);
  }
  goto cleanup_95;
  /* unreachable after return */
  goto end_cleanup_96;
cleanup_95: ;
  goto end_function_94;
end_cleanup_96: ;
end_function_94: ;
  return zcbz318;
end_block_exception_97: ;

  return false;
}



static void finish_sailgen_CapUnsignedLessThan(void)
{
  KILL(sail_int)(&zghz334);
  KILL(sail_int)(&zghz333);
}

static uint64_t sailgen_CapGetTop(lbits);

lbits zghz335;
sail_int zghz336;
sail_int zghz337;
lbits zghz338;
lbits zghz339;
sail_int zghz340;
sail_int zghz341;
sail_int zghz342;
sail_int zghz343;
sail_int zghz344;
sail_int zghz345;
sail_int zghz346;
sail_int zghz347;
sail_int zghz348;
sail_int zghz349;
sail_int zghz350;
sail_int zghz351;
lbits zghz352;
lbits zghz353;
sail_int zghz354;
sail_int zghz355;
sail_int zghz356;
sail_int zghz357;
sail_int zghz358;
sail_int zghz359;
sail_int zghz360;
sail_int zghz361;
lbits zghz362;
sail_int zghz363;
sail_int zghz364;
lbits zghz365;

static void startup_sailgen_CapGetTop(void)
{
  CREATE(lbits)(&zghz335);
  CREATE(sail_int)(&zghz336);
  CREATE(sail_int)(&zghz337);
  CREATE(lbits)(&zghz338);
  CREATE(lbits)(&zghz339);
  CREATE(sail_int)(&zghz340);
  CREATE(sail_int)(&zghz341);
  CREATE(sail_int)(&zghz342);
  CREATE(sail_int)(&zghz343);
  CREATE(sail_int)(&zghz344);
  CREATE(sail_int)(&zghz345);
  CREATE(sail_int)(&zghz346);
  CREATE(sail_int)(&zghz347);
  CREATE(sail_int)(&zghz348);
  CREATE(sail_int)(&zghz349);
  CREATE(sail_int)(&zghz350);
  CREATE(sail_int)(&zghz351);
  CREATE(lbits)(&zghz352);
  CREATE(lbits)(&zghz353);
  CREATE(sail_int)(&zghz354);
  CREATE(sail_int)(&zghz355);
  CREATE(sail_int)(&zghz356);
  CREATE(sail_int)(&zghz357);
  CREATE(sail_int)(&zghz358);
  CREATE(sail_int)(&zghz359);
  CREATE(sail_int)(&zghz360);
  CREATE(sail_int)(&zghz361);
  CREATE(lbits)(&zghz362);
  CREATE(sail_int)(&zghz363);
  CREATE(sail_int)(&zghz364);
  CREATE(lbits)(&zghz365);
}

static uint64_t sailgen_CapGetTop(lbits zc)
{
  __label__ cleanup_100, end_cleanup_101, end_function_99, end_block_exception_102;

  uint64_t zcbz319;
  uint64_t zlmsb;
  zlmsb = UINT64_C(0b00);
  uint64_t zlcarry;
  zlcarry = UINT64_C(0b00);
  uint64_t zb;
  zb = sailgen_CapGetBottom(zc);
  uint64_t zt;
  zt = UINT64_C(0x0000);
  {
    bool zgaz331;
    zgaz331 = sailgen_CapIsInternalExponent(zc);
    unit zgsz329;
    if (zgaz331) {
      {
        zlmsb = UINT64_C(0b01);
        unit zgsz330;
        zgsz330 = UNIT;
      }
      uint64_t zgaz333;
      {
        uint64_t zgaz332;
        {
          RECREATE(sail_int)(&zghz363);
          CONVERT_OF(sail_int, mach_int)(&zghz363, zCAP_LIMIT_HI_BIT);
          RECREATE(sail_int)(&zghz364);
          CONVERT_OF(sail_int, mach_int)(&zghz364, zCAP_LIMIT_MANTISSA_LO_BIT);
          RECREATE(lbits)(&zghz365);
          vector_subrange_lbits(&zghz365, zc, zghz363, zghz364);
          zgaz332 = CONVERT_OF(fbits, lbits)(zghz365, true);
        }
        zgaz333 = (UINT64_C(0b00) << 11) | zgaz332;
      }
      zt = (zgaz333 << 3) | UINT64_C(0b000);
      zgsz329 = UNIT;
    } else {
      uint64_t zgaz334;
      {
        RECREATE(sail_int)(&zghz360);
        CONVERT_OF(sail_int, mach_int)(&zghz360, zCAP_LIMIT_HI_BIT);
        RECREATE(sail_int)(&zghz361);
        CONVERT_OF(sail_int, mach_int)(&zghz361, zCAP_LIMIT_LO_BIT);
        RECREATE(lbits)(&zghz362);
        vector_subrange_lbits(&zghz362, zc, zghz360, zghz361);
        zgaz334 = CONVERT_OF(fbits, lbits)(zghz362, true);
      }
      zt = (UINT64_C(0b00) << 14) | zgaz334;
      zgsz329 = UNIT;
    }
  }
  {
    bool zgaz339;
    {
      uint64_t zgaz337;
      {
        int64_t zgaz335;
        {
          RECREATE(sail_int)(&zghz357);
          CONVERT_OF(sail_int, mach_int)(&zghz357, zCAP_MW);
          RECREATE(sail_int)(&zghz358);
          CONVERT_OF(sail_int, mach_int)(&zghz358, INT64_C(3));
          RECREATE(sail_int)(&zghz359);
          sub_int(&zghz359, zghz357, zghz358);
          zgaz335 = CONVERT_OF(mach_int, sail_int)(zghz359);
        }
        zgaz337 = (safe_rshift(UINT64_MAX, 64 - 14) & (zt >> INT64_C(0)));
      }
      uint64_t zgaz338;
      {
        int64_t zgaz336;
        {
          RECREATE(sail_int)(&zghz354);
          CONVERT_OF(sail_int, mach_int)(&zghz354, zCAP_MW);
          RECREATE(sail_int)(&zghz355);
          CONVERT_OF(sail_int, mach_int)(&zghz355, INT64_C(3));
          RECREATE(sail_int)(&zghz356);
          sub_int(&zghz356, zghz354, zghz355);
          zgaz336 = CONVERT_OF(mach_int, sail_int)(zghz356);
        }
        zgaz338 = (safe_rshift(UINT64_MAX, 64 - 14) & (zb >> INT64_C(0)));
      }
      {
        RECREATE(lbits)(&zghz352);
        CONVERT_OF(lbits, fbits)(&zghz352, zgaz337, UINT64_C(14) , true);
        RECREATE(lbits)(&zghz353);
        CONVERT_OF(lbits, fbits)(&zghz353, zgaz338, UINT64_C(14) , true);
        zgaz339 = sailgen_CapUnsignedLessThan(zghz352, zghz353);
      }
    }
    unit zgsz328;
    if (zgaz339) {
      zlcarry = UINT64_C(0b01);
      zgsz328 = UNIT;
    } else {  zgsz328 = UNIT;  }
  }
  {
    int64_t zgaz344;
    {
      RECREATE(sail_int)(&zghz349);
      CONVERT_OF(sail_int, mach_int)(&zghz349, zCAP_MW);
      RECREATE(sail_int)(&zghz350);
      CONVERT_OF(sail_int, mach_int)(&zghz350, INT64_C(1));
      RECREATE(sail_int)(&zghz351);
      sub_int(&zghz351, zghz349, zghz350);
      zgaz344 = CONVERT_OF(mach_int, sail_int)(zghz351);
    }
    int64_t zgaz345;
    {
      RECREATE(sail_int)(&zghz346);
      CONVERT_OF(sail_int, mach_int)(&zghz346, zCAP_MW);
      RECREATE(sail_int)(&zghz347);
      CONVERT_OF(sail_int, mach_int)(&zghz347, INT64_C(2));
      RECREATE(sail_int)(&zghz348);
      sub_int(&zghz348, zghz346, zghz347);
      zgaz345 = CONVERT_OF(mach_int, sail_int)(zghz348);
    }
    uint64_t zgaz346;
    {
      uint64_t zgaz343;
      {
        uint64_t zgaz342;
        {
          int64_t zgaz340;
          {
            RECREATE(sail_int)(&zghz343);
            CONVERT_OF(sail_int, mach_int)(&zghz343, zCAP_MW);
            RECREATE(sail_int)(&zghz344);
            CONVERT_OF(sail_int, mach_int)(&zghz344, INT64_C(1));
            RECREATE(sail_int)(&zghz345);
            sub_int(&zghz345, zghz343, zghz344);
            zgaz340 = CONVERT_OF(mach_int, sail_int)(zghz345);
          }
          int64_t zgaz341;
          {
            RECREATE(sail_int)(&zghz340);
            CONVERT_OF(sail_int, mach_int)(&zghz340, zCAP_MW);
            RECREATE(sail_int)(&zghz341);
            CONVERT_OF(sail_int, mach_int)(&zghz341, INT64_C(2));
            RECREATE(sail_int)(&zghz342);
            sub_int(&zghz342, zghz340, zghz341);
            zgaz341 = CONVERT_OF(mach_int, sail_int)(zghz342);
          }
          zgaz342 = (safe_rshift(UINT64_MAX, 64 - 2) & (zb >> zgaz341));
        }
        zgaz343 = ((zgaz342 + zlmsb) & UINT64_C(0b11));
      }
      zgaz346 = ((zgaz343 + zlcarry) & UINT64_C(0b11));
    }
    {
      RECREATE(lbits)(&zghz335);
      CONVERT_OF(lbits, fbits)(&zghz335, zt, UINT64_C(16) , true);
      RECREATE(sail_int)(&zghz336);
      CONVERT_OF(sail_int, mach_int)(&zghz336, zgaz344);
      RECREATE(sail_int)(&zghz337);
      CONVERT_OF(sail_int, mach_int)(&zghz337, zgaz345);
      RECREATE(lbits)(&zghz338);
      CONVERT_OF(lbits, fbits)(&zghz338, zgaz346, UINT64_C(2) , true);
      RECREATE(lbits)(&zghz339);
      vector_update_subrange_lbits(&zghz339, zghz335, zghz336, zghz337, zghz338);
      zt = CONVERT_OF(fbits, lbits)(zghz339, true);
    }
    unit zgsz327;
    zgsz327 = UNIT;
  }
  zcbz319 = zt;
  goto cleanup_100;
  /* unreachable after return */




  goto end_cleanup_101;
cleanup_100: ;




  goto end_function_99;
end_cleanup_101: ;
end_function_99: ;
  return zcbz319;
end_block_exception_102: ;

  return UINT64_C(0xdeadc0de);
}



static void finish_sailgen_CapGetTop(void)
{
  KILL(lbits)(&zghz365);
  KILL(sail_int)(&zghz364);
  KILL(sail_int)(&zghz363);
  KILL(lbits)(&zghz362);
  KILL(sail_int)(&zghz361);
  KILL(sail_int)(&zghz360);
  KILL(sail_int)(&zghz359);
  KILL(sail_int)(&zghz358);
  KILL(sail_int)(&zghz357);
  KILL(sail_int)(&zghz356);
  KILL(sail_int)(&zghz355);
  KILL(sail_int)(&zghz354);
  KILL(lbits)(&zghz353);
  KILL(lbits)(&zghz352);
  KILL(sail_int)(&zghz351);
  KILL(sail_int)(&zghz350);
  KILL(sail_int)(&zghz349);
  KILL(sail_int)(&zghz348);
  KILL(sail_int)(&zghz347);
  KILL(sail_int)(&zghz346);
  KILL(sail_int)(&zghz345);
  KILL(sail_int)(&zghz344);
  KILL(sail_int)(&zghz343);
  KILL(sail_int)(&zghz342);
  KILL(sail_int)(&zghz341);
  KILL(sail_int)(&zghz340);
  KILL(lbits)(&zghz339);
  KILL(lbits)(&zghz338);
  KILL(sail_int)(&zghz337);
  KILL(sail_int)(&zghz336);
  KILL(lbits)(&zghz335);
}

static bool sailgen_CapIsExponentOutOfRange(lbits);

static bool sailgen_CapIsExponentOutOfRange(lbits zc)
{
  __label__ case_105, finish_match_104, end_function_106, end_block_exception_109;

  bool zcbz320;
  int64_t zgaz347;
  zgaz347 = sailgen_CapGetExponent(zc);
  {
    __label__ cleanup_107, end_cleanup_108;

    int64_t zexp;
    zexp = zgaz347;
    bool zgaz349;
    {
      bool zgaz348;
      zgaz348 = (zexp > zCAP_MAX_EXPONENT);
      bool zgsz332;
      if (zgaz348) {  zgsz332 = (zexp < zCAP_MAX_ENCODEABLE_EXPONENT);  } else {  zgsz332 = false;  }
      zgaz349 = zgsz332;
    }
    zcbz320 = zgaz349;
    goto cleanup_107;
    /* unreachable after return */
    goto finish_match_104;
    goto end_cleanup_108;
  cleanup_107: ;
    goto end_function_106;
  end_cleanup_108: ;
  }
case_105: ;
  sail_match_failure("CapIsExponentOutOfRange");
finish_match_104: ;

end_function_106: ;
  return zcbz320;
end_block_exception_109: ;

  return false;
}

static bool sailgen_CapUnsignedGreaterThan(lbits, lbits);

sail_int zghz366;
sail_int zghz367;

static void startup_sailgen_CapUnsignedGreaterThan(void)
{
  CREATE(sail_int)(&zghz366);
  CREATE(sail_int)(&zghz367);
}

static bool sailgen_CapUnsignedGreaterThan(lbits za, lbits zb)
{
  __label__ cleanup_112, end_cleanup_113, end_function_111, end_block_exception_114;

  bool zcbz321;
  {
    RECREATE(sail_int)(&zghz366);
    sail_unsigned(&zghz366, za);
    RECREATE(sail_int)(&zghz367);
    sail_unsigned(&zghz367, zb);
    zcbz321 = gt(zghz366, zghz367);
  }
  goto cleanup_112;
  /* unreachable after return */
  goto end_cleanup_113;
cleanup_112: ;
  goto end_function_111;
end_cleanup_113: ;
end_function_111: ;
  return zcbz321;
end_block_exception_114: ;

  return false;
}



static void finish_sailgen_CapUnsignedGreaterThan(void)
{
  KILL(sail_int)(&zghz367);
  KILL(sail_int)(&zghz366);
}

// struct tuple_(%bv, %bv, %bool)
struct ztuple_z8z5bvzCz0z5bvzCz0z5boolz9 {
  lbits ztup0;
  lbits ztup1;
  bool ztup2;
};

static void COPY(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(struct ztuple_z8z5bvzCz0z5bvzCz0z5boolz9 *rop, const struct ztuple_z8z5bvzCz0z5bvzCz0z5boolz9 op) {
  COPY(lbits)(&rop->ztup0, op.ztup0);
  COPY(lbits)(&rop->ztup1, op.ztup1);
  rop->ztup2 = op.ztup2;
}

static void CREATE(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(struct ztuple_z8z5bvzCz0z5bvzCz0z5boolz9 *op) {
  CREATE(lbits)(&op->ztup0);
  CREATE(lbits)(&op->ztup1);
}

static void RECREATE(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(struct ztuple_z8z5bvzCz0z5bvzCz0z5boolz9 *op) {
  RECREATE(lbits)(&op->ztup0);
  RECREATE(lbits)(&op->ztup1);
}

static void KILL(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(struct ztuple_z8z5bvzCz0z5bvzCz0z5boolz9 *op) {
  KILL(lbits)(&op->ztup0);
  KILL(lbits)(&op->ztup1);
}

static bool EQUAL(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(struct ztuple_z8z5bvzCz0z5bvzCz0z5boolz9 op1, struct ztuple_z8z5bvzCz0z5bvzCz0z5boolz9 op2) {
  return EQUAL(lbits)(op1.ztup0, op2.ztup0) && EQUAL(lbits)(op1.ztup1, op2.ztup1) && EQUAL(bool)(op1.ztup2, op2.ztup2);
}

static void sailgen_CapGetBounds(struct ztuple_z8z5bvzCz0z5bvzCz0z5boolz9 *rop, lbits);

lbits zghz368;
lbits zghz369;
lbits zghz370;
lbits zghz371;
lbits zghz372;
sail_int zghz373;
sail_int zghz374;
lbits zghz375;
lbits zghz376;
sail_int zghz377;
sail_int zghz378;
lbits zghz379;
sail_int zghz380;
sail_int zghz381;
lbits zghz382;
lbits zghz383;
sail_int zghz384;
sail_int zghz385;
sail_int zghz386;
sail_int zghz387;
sail_int zghz388;
sail_int zghz389;
lbits zghz390;
sail_int zghz391;
sail_int zghz392;
lbits zghz393;
lbits zghz394;
sail_int zghz395;
lbits zghz396;
sail_int zghz397;
sail_int zghz398;
lbits zghz399;
lbits zghz3100;
sail_int zghz3101;
lbits zghz3102;
sail_int zghz3103;
sail_int zghz3104;
lbits zghz3105;
sail_int zghz3106;
sail_int zghz3107;
sail_int zghz3108;
sail_int zghz3109;
sail_int zghz3110;
sail_int zghz3111;
lbits zghz3112;
lbits zghz3113;
lbits zghz3114;
lbits zghz3115;
lbits zghz3116;
lbits zghz3117;
sail_int zghz3118;
sail_int zghz3119;
sail_int zghz3120;
sail_int zghz3121;
sail_int zghz3122;
sail_int zghz3123;
sail_int zghz3124;
sail_int zghz3125;
sail_int zghz3126;
sail_int zghz3127;
sail_int zghz3128;
sail_int zghz3129;
sail_int zghz3130;
sail_int zghz3131;
lbits zghz3132;
sail_int zghz3133;
sail_int zghz3134;
sail_int zghz3135;
sail_int zghz3136;
sail_int zghz3137;
sail_int zghz3138;
lbits zghz3139;
lbits zghz3140;
sail_int zghz3141;
sail_int zghz3142;
lbits zghz3143;
sail_int zghz3144;
sail_int zghz3145;
sail_int zghz3146;
sail_int zghz3147;
sail_int zghz3148;
lbits zghz3149;
sail_int zghz3150;
sail_int zghz3151;
sail_int zghz3152;
sail_int zghz3153;
sail_int zghz3154;
sail_int zghz3155;
sail_int zghz3156;
sail_int zghz3157;
sail_int zghz3158;
sail_int zghz3159;
sail_int zghz3160;
lbits zghz3161;
sail_int zghz3162;
lbits zghz3163;
sail_int zghz3164;
sail_int zghz3165;
sail_int zghz3166;
lbits zghz3167;
sail_int zghz3168;
lbits zghz3169;
lbits zghz3170;
lbits zghz3171;

static void startup_sailgen_CapGetBounds(void)
{
  CREATE(lbits)(&zghz368);
  CREATE(lbits)(&zghz369);
  CREATE(lbits)(&zghz370);
  CREATE(lbits)(&zghz371);
  CREATE(lbits)(&zghz372);
  CREATE(sail_int)(&zghz373);
  CREATE(sail_int)(&zghz374);
  CREATE(lbits)(&zghz375);
  CREATE(lbits)(&zghz376);
  CREATE(sail_int)(&zghz377);
  CREATE(sail_int)(&zghz378);
  CREATE(lbits)(&zghz379);
  CREATE(sail_int)(&zghz380);
  CREATE(sail_int)(&zghz381);
  CREATE(lbits)(&zghz382);
  CREATE(lbits)(&zghz383);
  CREATE(sail_int)(&zghz384);
  CREATE(sail_int)(&zghz385);
  CREATE(sail_int)(&zghz386);
  CREATE(sail_int)(&zghz387);
  CREATE(sail_int)(&zghz388);
  CREATE(sail_int)(&zghz389);
  CREATE(lbits)(&zghz390);
  CREATE(sail_int)(&zghz391);
  CREATE(sail_int)(&zghz392);
  CREATE(lbits)(&zghz393);
  CREATE(lbits)(&zghz394);
  CREATE(sail_int)(&zghz395);
  CREATE(lbits)(&zghz396);
  CREATE(sail_int)(&zghz397);
  CREATE(sail_int)(&zghz398);
  CREATE(lbits)(&zghz399);
  CREATE(lbits)(&zghz3100);
  CREATE(sail_int)(&zghz3101);
  CREATE(lbits)(&zghz3102);
  CREATE(sail_int)(&zghz3103);
  CREATE(sail_int)(&zghz3104);
  CREATE(lbits)(&zghz3105);
  CREATE(sail_int)(&zghz3106);
  CREATE(sail_int)(&zghz3107);
  CREATE(sail_int)(&zghz3108);
  CREATE(sail_int)(&zghz3109);
  CREATE(sail_int)(&zghz3110);
  CREATE(sail_int)(&zghz3111);
  CREATE(lbits)(&zghz3112);
  CREATE(lbits)(&zghz3113);
  CREATE(lbits)(&zghz3114);
  CREATE(lbits)(&zghz3115);
  CREATE(lbits)(&zghz3116);
  CREATE(lbits)(&zghz3117);
  CREATE(sail_int)(&zghz3118);
  CREATE(sail_int)(&zghz3119);
  CREATE(sail_int)(&zghz3120);
  CREATE(sail_int)(&zghz3121);
  CREATE(sail_int)(&zghz3122);
  CREATE(sail_int)(&zghz3123);
  CREATE(sail_int)(&zghz3124);
  CREATE(sail_int)(&zghz3125);
  CREATE(sail_int)(&zghz3126);
  CREATE(sail_int)(&zghz3127);
  CREATE(sail_int)(&zghz3128);
  CREATE(sail_int)(&zghz3129);
  CREATE(sail_int)(&zghz3130);
  CREATE(sail_int)(&zghz3131);
  CREATE(lbits)(&zghz3132);
  CREATE(sail_int)(&zghz3133);
  CREATE(sail_int)(&zghz3134);
  CREATE(sail_int)(&zghz3135);
  CREATE(sail_int)(&zghz3136);
  CREATE(sail_int)(&zghz3137);
  CREATE(sail_int)(&zghz3138);
  CREATE(lbits)(&zghz3139);
  CREATE(lbits)(&zghz3140);
  CREATE(sail_int)(&zghz3141);
  CREATE(sail_int)(&zghz3142);
  CREATE(lbits)(&zghz3143);
  CREATE(sail_int)(&zghz3144);
  CREATE(sail_int)(&zghz3145);
  CREATE(sail_int)(&zghz3146);
  CREATE(sail_int)(&zghz3147);
  CREATE(sail_int)(&zghz3148);
  CREATE(lbits)(&zghz3149);
  CREATE(sail_int)(&zghz3150);
  CREATE(sail_int)(&zghz3151);
  CREATE(sail_int)(&zghz3152);
  CREATE(sail_int)(&zghz3153);
  CREATE(sail_int)(&zghz3154);
  CREATE(sail_int)(&zghz3155);
  CREATE(sail_int)(&zghz3156);
  CREATE(sail_int)(&zghz3157);
  CREATE(sail_int)(&zghz3158);
  CREATE(sail_int)(&zghz3159);
  CREATE(sail_int)(&zghz3160);
  CREATE(lbits)(&zghz3161);
  CREATE(sail_int)(&zghz3162);
  CREATE(lbits)(&zghz3163);
  CREATE(sail_int)(&zghz3164);
  CREATE(sail_int)(&zghz3165);
  CREATE(sail_int)(&zghz3166);
  CREATE(lbits)(&zghz3167);
  CREATE(sail_int)(&zghz3168);
  CREATE(lbits)(&zghz3169);
  CREATE(lbits)(&zghz3170);
  CREATE(lbits)(&zghz3171);
}

static void sailgen_CapGetBounds(struct ztuple_z8z5bvzCz0z5bvzCz0z5boolz9 *zcbz322, lbits zc)
{
  __label__ case_117, finish_match_116, end_function_128, end_block_exception_135, end_function_281;

  int64_t zgaz353;
  zgaz353 = sailgen_CapGetExponent(zc);
  struct ztuple_z8z5bvzCz0z5bvzCz0z5boolz9 zgsz334;
  CREATE(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgsz334);
  {
    __label__ case_119, finish_match_118;

    int64_t zexp;
    zexp = zgaz353;
    {
      bool zgaz354;
      zgaz354 = (zexp == zCAP_MAX_ENCODEABLE_EXPONENT);
      unit zgsz337;
      if (zgaz354) {
        __label__ cleanup_133, end_cleanup_134;

        struct ztuple_z8z5bvzCz0z5bvzCz0z5boolz9 zgsz338;
        CREATE(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgsz338);
        COPY(lbits)(&((&zgsz338)->ztup0), zCAP_BOUND_MIN);
        COPY(lbits)(&((&zgsz338)->ztup1), zCAP_BOUND_MAX);
        zgsz338.ztup2 = true;
        COPY(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)((*(&zcbz322)), zgsz338);
        goto cleanup_133;
        /* unreachable after return */
        goto end_cleanup_134;
      cleanup_133: ;
        KILL(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgsz334);
        KILL(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgsz338);
        goto end_function_128;
      end_cleanup_134: ;
      } else {  zgsz337 = UNIT;  }
    }
    {
      bool zgaz355;
      zgaz355 = sailgen_CapIsExponentOutOfRange(zc);
      unit zgsz335;
      if (zgaz355) {
        __label__ cleanup_131, end_cleanup_132;

        struct ztuple_z8z5bvzCz0z5bvzCz0z5boolz9 zgsz336;
        CREATE(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgsz336);
        COPY(lbits)(&((&zgsz336)->ztup0), zCAP_BOUND_MIN);
        COPY(lbits)(&((&zgsz336)->ztup1), zCAP_BOUND_MAX);
        zgsz336.ztup2 = false;
        COPY(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)((*(&zcbz322)), zgsz336);
        goto cleanup_131;
        /* unreachable after return */
        goto end_cleanup_132;
      cleanup_131: ;
        KILL(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgsz334);
        KILL(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgsz336);
        goto end_function_128;
      end_cleanup_132: ;
      } else {  zgsz335 = UNIT;  }
    }
    RECREATE(lbits)(&zghz368);
    {
      RECREATE(lbits)(&zghz3171);
      CONVERT_OF(lbits, fbits)(&zghz3171, UINT64_C(0b00), UINT64_C(2) , true);
      append_64(&zghz3171, zghz3171, UINT64_C(0x0000000000000000));
      COPY(lbits)(&zghz368, zghz3171);
    }
    RECREATE(lbits)(&zghz369);
    {
      RECREATE(lbits)(&zghz3170);
      CONVERT_OF(lbits, fbits)(&zghz3170, UINT64_C(0b00), UINT64_C(2) , true);
      append_64(&zghz3170, zghz3170, UINT64_C(0x0000000000000000));
      COPY(lbits)(&zghz369, zghz3170);
    }
    uint64_t zbottom;
    zbottom = sailgen_CapGetBottom(zc);
    uint64_t ztop;
    ztop = sailgen_CapGetTop(zc);
    {
      sbits zgaz356;
      {
        RECREATE(sail_int)(&zghz3168);
        CONVERT_OF(sail_int, mach_int)(&zghz3168, zexp);
        RECREATE(lbits)(&zghz3169);
        sailgen_Zeros(&zghz3169, zghz3168);
        zgaz356 = CONVERT_OF(sbits, lbits)(zghz3169, true);
      }
      {
        RECREATE(sail_int)(&zghz3164);
        CONVERT_OF(sail_int, mach_int)(&zghz3164, INT64_C(66));
        RECREATE(sail_int)(&zghz3165);
        CONVERT_OF(sail_int, mach_int)(&zghz3165, zexp);
        RECREATE(sail_int)(&zghz3166);
        CONVERT_OF(sail_int, mach_int)(&zghz3166, INT64_C(0));
        RECREATE(lbits)(&zghz3167);
        CONVERT_OF(lbits, sbits)(&zghz3167, zgaz356, true);
        set_slice(&zghz368, zghz3164, zghz3165, zghz368, zghz3166, zghz3167);
      }
      unit zgsz345;
      zgsz345 = UNIT;
    }
    {
      sbits zgaz357;
      {
        RECREATE(sail_int)(&zghz3162);
        CONVERT_OF(sail_int, mach_int)(&zghz3162, zexp);
        RECREATE(lbits)(&zghz3163);
        sailgen_Zeros(&zghz3163, zghz3162);
        zgaz357 = CONVERT_OF(sbits, lbits)(zghz3163, true);
      }
      {
        RECREATE(sail_int)(&zghz3158);
        CONVERT_OF(sail_int, mach_int)(&zghz3158, INT64_C(66));
        RECREATE(sail_int)(&zghz3159);
        CONVERT_OF(sail_int, mach_int)(&zghz3159, zexp);
        RECREATE(sail_int)(&zghz3160);
        CONVERT_OF(sail_int, mach_int)(&zghz3160, INT64_C(0));
        RECREATE(lbits)(&zghz3161);
        CONVERT_OF(lbits, sbits)(&zghz3161, zgaz357, true);
        set_slice(&zghz369, zghz3158, zghz3159, zghz369, zghz3160, zghz3161);
      }
      unit zgsz344;
      zgsz344 = UNIT;
    }
    {
      bool zgaz361;
      {
        int64_t zgaz360;
        {
          int64_t zgaz359;
          {
            int64_t zgaz358;
            {
              RECREATE(sail_int)(&zghz3156);
              CONVERT_OF(sail_int, mach_int)(&zghz3156, zexp);
              RECREATE(sail_int)(&zghz3157);
              sailgen___id(&zghz3157, zghz3156);
              zgaz358 = CONVERT_OF(mach_int, sail_int)(zghz3157);
            }
            zgaz359 = (zgaz358 + INT64_C(16));
          }
          {
            RECREATE(sail_int)(&zghz3153);
            CONVERT_OF(sail_int, mach_int)(&zghz3153, zgaz359);
            RECREATE(sail_int)(&zghz3154);
            CONVERT_OF(sail_int, mach_int)(&zghz3154, INT64_C(1));
            RECREATE(sail_int)(&zghz3155);
            sub_int(&zghz3155, zghz3153, zghz3154);
            zgaz360 = CONVERT_OF(mach_int, sail_int)(zghz3155);
          }
        }
        zgaz361 = (zgaz360 < INT64_C(66));
      }
      unit zgsz343;
      zgsz343 = sail_assert(zgaz361, "v8_base.sail:1391.41-1391.42");
    }
    {
      int64_t zgaz363;
      {
        int64_t zgaz362;
        zgaz362 = (zexp + zCAP_MW);
        {
          RECREATE(sail_int)(&zghz3150);
          CONVERT_OF(sail_int, mach_int)(&zghz3150, zgaz362);
          RECREATE(sail_int)(&zghz3151);
          CONVERT_OF(sail_int, mach_int)(&zghz3151, INT64_C(1));
          RECREATE(sail_int)(&zghz3152);
          sub_int(&zghz3152, zghz3150, zghz3151);
          zgaz363 = CONVERT_OF(mach_int, sail_int)(zghz3152);
        }
      }
      {
        RECREATE(sail_int)(&zghz3147);
        CONVERT_OF(sail_int, mach_int)(&zghz3147, zgaz363);
        RECREATE(sail_int)(&zghz3148);
        CONVERT_OF(sail_int, mach_int)(&zghz3148, zexp);
        RECREATE(lbits)(&zghz3149);
        CONVERT_OF(lbits, fbits)(&zghz3149, zbottom, UINT64_C(16) , true);
        vector_update_subrange_lbits(&zghz368, zghz368, zghz3147, zghz3148, zghz3149);
      }
      unit zgsz342;
      zgsz342 = UNIT;
    }
    {
      int64_t zgaz365;
      {
        int64_t zgaz364;
        zgaz364 = (zexp + zCAP_MW);
        {
          RECREATE(sail_int)(&zghz3144);
          CONVERT_OF(sail_int, mach_int)(&zghz3144, zgaz364);
          RECREATE(sail_int)(&zghz3145);
          CONVERT_OF(sail_int, mach_int)(&zghz3145, INT64_C(1));
          RECREATE(sail_int)(&zghz3146);
          sub_int(&zghz3146, zghz3144, zghz3145);
          zgaz365 = CONVERT_OF(mach_int, sail_int)(zghz3146);
        }
      }
      {
        RECREATE(sail_int)(&zghz3141);
        CONVERT_OF(sail_int, mach_int)(&zghz3141, zgaz365);
        RECREATE(sail_int)(&zghz3142);
        CONVERT_OF(sail_int, mach_int)(&zghz3142, zexp);
        RECREATE(lbits)(&zghz3143);
        CONVERT_OF(lbits, fbits)(&zghz3143, ztop, UINT64_C(16) , true);
        vector_update_subrange_lbits(&zghz369, zghz369, zghz3141, zghz3142, zghz3143);
      }
      unit zgsz341;
      zgsz341 = UNIT;
    }
    RECREATE(lbits)(&zghz370);
    {
      uint64_t zgaz3102;
      {
        uint64_t zgaz3101;
        zgaz3101 = sailgen_CapGetValue(zc);
        zgaz3102 = sailgen_CapBoundsAddress(zgaz3101);
      }
      {
        RECREATE(lbits)(&zghz3139);
        CONVERT_OF(lbits, fbits)(&zghz3139, UINT64_C(0b00), UINT64_C(2) , true);
        RECREATE(lbits)(&zghz3140);
        CONVERT_OF(lbits, fbits)(&zghz3140, zgaz3102, UINT64_C(64) , true);
        append(&zghz370, zghz3139, zghz3140);
      }
    }
    uint64_t zA3;
    {
      int64_t zgaz399;
      {
        int64_t zgaz397;
        zgaz397 = (zexp + zCAP_MW);
        {
          RECREATE(sail_int)(&zghz3136);
          CONVERT_OF(sail_int, mach_int)(&zghz3136, zgaz397);
          RECREATE(sail_int)(&zghz3137);
          CONVERT_OF(sail_int, mach_int)(&zghz3137, INT64_C(1));
          RECREATE(sail_int)(&zghz3138);
          sub_int(&zghz3138, zghz3136, zghz3137);
          zgaz399 = CONVERT_OF(mach_int, sail_int)(zghz3138);
        }
      }
      int64_t zgaz3100;
      {
        int64_t zgaz398;
        zgaz398 = (zexp + zCAP_MW);
        {
          RECREATE(sail_int)(&zghz3133);
          CONVERT_OF(sail_int, mach_int)(&zghz3133, zgaz398);
          RECREATE(sail_int)(&zghz3134);
          CONVERT_OF(sail_int, mach_int)(&zghz3134, INT64_C(3));
          RECREATE(sail_int)(&zghz3135);
          sub_int(&zghz3135, zghz3133, zghz3134);
          zgaz3100 = CONVERT_OF(mach_int, sail_int)(zghz3135);
        }
      }
      {
        RECREATE(sail_int)(&zghz3130);
        CONVERT_OF(sail_int, mach_int)(&zghz3130, zgaz399);
        RECREATE(sail_int)(&zghz3131);
        CONVERT_OF(sail_int, mach_int)(&zghz3131, zgaz3100);
        RECREATE(lbits)(&zghz3132);
        vector_subrange_lbits(&zghz3132, zghz370, zghz3130, zghz3131);
        zA3 = CONVERT_OF(fbits, lbits)(zghz3132, true);
      }
    }
    uint64_t zB3;
    {
      int64_t zgaz395;
      {
        RECREATE(sail_int)(&zghz3127);
        CONVERT_OF(sail_int, mach_int)(&zghz3127, zCAP_MW);
        RECREATE(sail_int)(&zghz3128);
        CONVERT_OF(sail_int, mach_int)(&zghz3128, INT64_C(1));
        RECREATE(sail_int)(&zghz3129);
        sub_int(&zghz3129, zghz3127, zghz3128);
        zgaz395 = CONVERT_OF(mach_int, sail_int)(zghz3129);
      }
      int64_t zgaz396;
      {
        RECREATE(sail_int)(&zghz3124);
        CONVERT_OF(sail_int, mach_int)(&zghz3124, zCAP_MW);
        RECREATE(sail_int)(&zghz3125);
        CONVERT_OF(sail_int, mach_int)(&zghz3125, INT64_C(3));
        RECREATE(sail_int)(&zghz3126);
        sub_int(&zghz3126, zghz3124, zghz3125);
        zgaz396 = CONVERT_OF(mach_int, sail_int)(zghz3126);
      }
      zB3 = (safe_rshift(UINT64_MAX, 64 - 3) & (zbottom >> zgaz396));
    }
    uint64_t zT3;
    {
      int64_t zgaz393;
      {
        RECREATE(sail_int)(&zghz3121);
        CONVERT_OF(sail_int, mach_int)(&zghz3121, zCAP_MW);
        RECREATE(sail_int)(&zghz3122);
        CONVERT_OF(sail_int, mach_int)(&zghz3122, INT64_C(1));
        RECREATE(sail_int)(&zghz3123);
        sub_int(&zghz3123, zghz3121, zghz3122);
        zgaz393 = CONVERT_OF(mach_int, sail_int)(zghz3123);
      }
      int64_t zgaz394;
      {
        RECREATE(sail_int)(&zghz3118);
        CONVERT_OF(sail_int, mach_int)(&zghz3118, zCAP_MW);
        RECREATE(sail_int)(&zghz3119);
        CONVERT_OF(sail_int, mach_int)(&zghz3119, INT64_C(3));
        RECREATE(sail_int)(&zghz3120);
        sub_int(&zghz3120, zghz3118, zghz3119);
        zgaz394 = CONVERT_OF(mach_int, sail_int)(zghz3120);
      }
      zT3 = (safe_rshift(UINT64_MAX, 64 - 3) & (ztop >> zgaz394));
    }
    uint64_t zR3;
    zR3 = ((zB3 - UINT64_C(0b001)) & UINT64_C(0b111));
    int64_t zaHi;
    zaHi = INT64_C(0);
    {
      bool zgaz366;
      {
        RECREATE(lbits)(&zghz3116);
        CONVERT_OF(lbits, fbits)(&zghz3116, zA3, UINT64_C(3) , true);
        RECREATE(lbits)(&zghz3117);
        CONVERT_OF(lbits, fbits)(&zghz3117, zR3, UINT64_C(3) , true);
        zgaz366 = sailgen_CapUnsignedLessThan(zghz3116, zghz3117);
      }
      unit zgsz346;
      if (zgaz366) {
        zaHi = INT64_C(1);
        zgsz346 = UNIT;
      } else {
        zaHi = INT64_C(0);
        zgsz346 = UNIT;
      }
    }
    {
      __label__ case_121, finish_match_120;

      int64_t zaHishadowz30;
      zaHishadowz30 = zaHi;
      int64_t zbHi;
      zbHi = INT64_C(0);
      {
        bool zgaz367;
        {
          RECREATE(lbits)(&zghz3114);
          CONVERT_OF(lbits, fbits)(&zghz3114, zB3, UINT64_C(3) , true);
          RECREATE(lbits)(&zghz3115);
          CONVERT_OF(lbits, fbits)(&zghz3115, zR3, UINT64_C(3) , true);
          zgaz367 = sailgen_CapUnsignedLessThan(zghz3114, zghz3115);
        }
        unit zgsz348;
        if (zgaz367) {
          zbHi = INT64_C(1);
          zgsz348 = UNIT;
        } else {
          zbHi = INT64_C(0);
          zgsz348 = UNIT;
        }
      }
      struct ztuple_z8z5bvzCz0z5bvzCz0z5boolz9 zgsz349;
      CREATE(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgsz349);
      {
        __label__ case_123, finish_match_122;

        int64_t zbHishadowz31;
        zbHishadowz31 = zbHi;
        int64_t ztHi;
        ztHi = INT64_C(0);
        {
          bool zgaz368;
          {
            RECREATE(lbits)(&zghz3112);
            CONVERT_OF(lbits, fbits)(&zghz3112, zT3, UINT64_C(3) , true);
            RECREATE(lbits)(&zghz3113);
            CONVERT_OF(lbits, fbits)(&zghz3113, zR3, UINT64_C(3) , true);
            zgaz368 = sailgen_CapUnsignedLessThan(zghz3112, zghz3113);
          }
          unit zgsz350;
          if (zgaz368) {
            ztHi = INT64_C(1);
            zgsz350 = UNIT;
          } else {
            ztHi = INT64_C(0);
            zgsz350 = UNIT;
          }
        }
        {
          __label__ case_125, finish_match_124;

          int64_t ztHishadowz32;
          ztHishadowz32 = ztHi;
          int64_t zgaz369;
          {
            RECREATE(sail_int)(&zghz3109);
            CONVERT_OF(sail_int, mach_int)(&zghz3109, zbHishadowz31);
            RECREATE(sail_int)(&zghz3110);
            CONVERT_OF(sail_int, mach_int)(&zghz3110, zaHishadowz30);
            RECREATE(sail_int)(&zghz3111);
            sub_int(&zghz3111, zghz3109, zghz3110);
            zgaz369 = CONVERT_OF(mach_int, sail_int)(zghz3111);
          }
          struct ztuple_z8z5bvzCz0z5bvzCz0z5boolz9 zgsz352;
          CREATE(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgsz352);
          {
            __label__ case_127, finish_match_126;

            int64_t zcorrection_base;
            zcorrection_base = zgaz369;
            int64_t zgaz370;
            {
              RECREATE(sail_int)(&zghz3106);
              CONVERT_OF(sail_int, mach_int)(&zghz3106, ztHishadowz32);
              RECREATE(sail_int)(&zghz3107);
              CONVERT_OF(sail_int, mach_int)(&zghz3107, zaHishadowz30);
              RECREATE(sail_int)(&zghz3108);
              sub_int(&zghz3108, zghz3106, zghz3107);
              zgaz370 = CONVERT_OF(mach_int, sail_int)(zghz3108);
            }
            {
              __label__ cleanup_129, end_cleanup_130;

              int64_t zcorrection_limit;
              zcorrection_limit = zgaz370;
              {
                bool zgaz373;
                {
                  int64_t zgaz371;
                  zgaz371 = (zexp + zCAP_MW);
                  int64_t zgaz372;
                  zgaz372 = (zCAP_MAX_EXPONENT + zCAP_MW);
                  zgaz373 = (zgaz371 < zgaz372);
                }
                unit zgsz354;
                if (zgaz373) {
                  sbits zatop;
                  {
                    int64_t zgaz378;
                    zgaz378 = (zexp + zCAP_MW);
                    {
                      RECREATE(sail_int)(&zghz3103);
                      CONVERT_OF(sail_int, mach_int)(&zghz3103, INT64_C(65));
                      RECREATE(sail_int)(&zghz3104);
                      CONVERT_OF(sail_int, mach_int)(&zghz3104, zgaz378);
                      RECREATE(lbits)(&zghz3105);
                      vector_subrange_lbits(&zghz3105, zghz370, zghz3103, zghz3104);
                      zatop = CONVERT_OF(sbits, lbits)(zghz3105, true);
                    }
                  }
                  {
                    int64_t zgaz374;
                    zgaz374 = (zexp + zCAP_MW);
                    sbits zgaz375;
                    {
                      RECREATE(lbits)(&zghz3100);
                      CONVERT_OF(lbits, sbits)(&zghz3100, zatop, true);
                      RECREATE(sail_int)(&zghz3101);
                      CONVERT_OF(sail_int, mach_int)(&zghz3101, zcorrection_base);
                      RECREATE(lbits)(&zghz3102);
                      add_bits_int(&zghz3102, zghz3100, zghz3101);
                      zgaz375 = CONVERT_OF(sbits, lbits)(zghz3102, true);
                    }
                    {
                      RECREATE(sail_int)(&zghz397);
                      CONVERT_OF(sail_int, mach_int)(&zghz397, INT64_C(65));
                      RECREATE(sail_int)(&zghz398);
                      CONVERT_OF(sail_int, mach_int)(&zghz398, zgaz374);
                      RECREATE(lbits)(&zghz399);
                      CONVERT_OF(lbits, sbits)(&zghz399, zgaz375, true);
                      vector_update_subrange_lbits(&zghz368, zghz368, zghz397, zghz398, zghz399);
                    }
                    unit zgsz355;
                    zgsz355 = UNIT;
                  }
                  int64_t zgaz376;
                  zgaz376 = (zexp + zCAP_MW);
                  sbits zgaz377;
                  {
                    RECREATE(lbits)(&zghz394);
                    CONVERT_OF(lbits, sbits)(&zghz394, zatop, true);
                    RECREATE(sail_int)(&zghz395);
                    CONVERT_OF(sail_int, mach_int)(&zghz395, zcorrection_limit);
                    RECREATE(lbits)(&zghz396);
                    add_bits_int(&zghz396, zghz394, zghz395);
                    zgaz377 = CONVERT_OF(sbits, lbits)(zghz396, true);
                  }
                  {
                    RECREATE(sail_int)(&zghz391);
                    CONVERT_OF(sail_int, mach_int)(&zghz391, INT64_C(65));
                    RECREATE(sail_int)(&zghz392);
                    CONVERT_OF(sail_int, mach_int)(&zghz392, zgaz376);
                    RECREATE(lbits)(&zghz393);
                    CONVERT_OF(lbits, sbits)(&zghz393, zgaz377, true);
                    vector_update_subrange_lbits(&zghz369, zghz369, zghz391, zghz392, zghz393);
                  }
                  zgsz354 = UNIT;
                } else {  zgsz354 = UNIT;  }
              }
              uint64_t zl2;
              {
                RECREATE(sail_int)(&zghz388);
                CONVERT_OF(sail_int, mach_int)(&zghz388, INT64_C(64));
                RECREATE(sail_int)(&zghz389);
                CONVERT_OF(sail_int, mach_int)(&zghz389, INT64_C(63));
                RECREATE(lbits)(&zghz390);
                vector_subrange_lbits(&zghz390, zghz369, zghz388, zghz389);
                zl2 = CONVERT_OF(fbits, lbits)(zghz390, true);
              }
              uint64_t zb2;
              {
                uint64_t zgaz392;
                {
                  fbits zgaz391;
                  {
                    RECREATE(sail_int)(&zghz387);
                    CONVERT_OF(sail_int, mach_int)(&zghz387, INT64_C(63));
                    zgaz391 = bitvector_access(zghz368, zghz387);
                  }
                  zgaz392 = UINT64_C(0b0);
                  zgaz392 = update_fbits(zgaz392, INT64_C(0), zgaz391);
                }
                zb2 = (UINT64_C(0b0) << 1) | zgaz392;
              }
              {
                bool zgaz382;
                {
                  bool zgaz381;
                  {
                    int64_t zgaz379;
                    {
                      RECREATE(sail_int)(&zghz384);
                      CONVERT_OF(sail_int, mach_int)(&zghz384, zCAP_MAX_EXPONENT);
                      RECREATE(sail_int)(&zghz385);
                      CONVERT_OF(sail_int, mach_int)(&zghz385, INT64_C(1));
                      RECREATE(sail_int)(&zghz386);
                      sub_int(&zghz386, zghz384, zghz385);
                      zgaz379 = CONVERT_OF(mach_int, sail_int)(zghz386);
                    }
                    zgaz381 = (zexp < zgaz379);
                  }
                  bool zgsz357;
                  if (zgaz381) {
                    uint64_t zgaz380;
                    zgaz380 = ((zl2 - zb2) & UINT64_C(0b11));
                    {
                      RECREATE(lbits)(&zghz382);
                      CONVERT_OF(lbits, fbits)(&zghz382, zgaz380, UINT64_C(2) , true);
                      RECREATE(lbits)(&zghz383);
                      CONVERT_OF(lbits, fbits)(&zghz383, UINT64_C(0b01), UINT64_C(2) , true);
                      zgsz357 = sailgen_CapUnsignedGreaterThan(zghz382, zghz383);
                    }
                  } else {  zgsz357 = false;  }
                  zgaz382 = zgsz357;
                }
                unit zgsz358;
                if (zgaz382) {
                  fbits zgaz386;
                  {
                    uint64_t zgaz385;
                    {
                      uint64_t zgaz384;
                      {
                        fbits zgaz383;
                        {
                          RECREATE(sail_int)(&zghz381);
                          CONVERT_OF(sail_int, mach_int)(&zghz381, INT64_C(64));
                          zgaz383 = bitvector_access(zghz369, zghz381);
                        }
                        zgaz384 = UINT64_C(0b0);
                        zgaz384 = update_fbits(zgaz384, INT64_C(0), zgaz383);
                      }
                      zgaz385 = (~(zgaz384) & UINT64_C(0b1));
                    }
                    zgaz386 = sailgen_Bit(zgaz385);
                  }
                  {
                    RECREATE(sail_int)(&zghz380);
                    CONVERT_OF(sail_int, mach_int)(&zghz380, INT64_C(64));
                    update_lbits(&zghz369, zghz369, zghz380, zgaz386);
                  }
                  zgsz358 = UNIT;
                } else {  zgsz358 = UNIT;  }
              }
              struct ztuple_z8z5bvzCz0z5bvzCz0z5boolz9 zgaz390;
              CREATE(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgaz390);
              {
                RECREATE(lbits)(&zghz371);
                {
                  uint64_t zgaz387;
                  {
                    RECREATE(sail_int)(&zghz377);
                    CONVERT_OF(sail_int, mach_int)(&zghz377, INT64_C(63));
                    RECREATE(sail_int)(&zghz378);
                    CONVERT_OF(sail_int, mach_int)(&zghz378, INT64_C(0));
                    RECREATE(lbits)(&zghz379);
                    vector_subrange_lbits(&zghz379, zghz368, zghz377, zghz378);
                    zgaz387 = CONVERT_OF(fbits, lbits)(zghz379, true);
                  }
                  {
                    RECREATE(lbits)(&zghz375);
                    CONVERT_OF(lbits, fbits)(&zghz375, UINT64_C(0b0), UINT64_C(1) , true);
                    RECREATE(lbits)(&zghz376);
                    CONVERT_OF(lbits, fbits)(&zghz376, zgaz387, UINT64_C(64) , true);
                    append(&zghz371, zghz375, zghz376);
                  }
                }
                RECREATE(lbits)(&zghz372);
                {
                  RECREATE(sail_int)(&zghz373);
                  CONVERT_OF(sail_int, mach_int)(&zghz373, INT64_C(64));
                  RECREATE(sail_int)(&zghz374);
                  CONVERT_OF(sail_int, mach_int)(&zghz374, INT64_C(0));
                  vector_subrange_lbits(&zghz372, zghz369, zghz373, zghz374);
                }
                COPY(lbits)(&((&zgaz390)->ztup0), zghz371);
                COPY(lbits)(&((&zgaz390)->ztup1), zghz372);
                zgaz390.ztup2 = true;
              }
              COPY(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)((*(&zcbz322)), zgaz390);
              goto cleanup_129;
              /* unreachable after return */
              KILL(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgaz390);
              goto finish_match_126;
              goto end_cleanup_130;
            cleanup_129: ;
              KILL(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgsz334);
              KILL(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgsz349);
              KILL(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgsz352);
              KILL(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgaz390);
              goto end_function_128;
            end_cleanup_130: ;
            }
          case_127: ;
            sail_match_failure("CapGetBounds");
          finish_match_126: ;
            goto finish_match_124;
          }
        case_125: ;
          sail_match_failure("CapGetBounds");
        finish_match_124: ;
          COPY(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgsz349, zgsz352);
          KILL(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgsz352);
          goto finish_match_122;
        }
      case_123: ;
        sail_match_failure("CapGetBounds");
      finish_match_122: ;
        goto finish_match_120;
      }
    case_121: ;
      sail_match_failure("CapGetBounds");
    finish_match_120: ;
      COPY(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgsz334, zgsz349);
      KILL(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgsz349);
      goto finish_match_118;
    }
  case_119: ;
    sail_match_failure("CapGetBounds");
  finish_match_118: ;
    goto finish_match_116;
  }
case_117: ;
  sail_match_failure("CapGetBounds");
finish_match_116: ;
  COPY(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)((*(&zcbz322)), zgsz334);
  KILL(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgsz334);

end_function_128: ;
  goto end_function_281;
end_block_exception_135: ;
  goto end_function_281;
end_function_281: ;
}



static void finish_sailgen_CapGetBounds(void)
{
  KILL(lbits)(&zghz3171);
  KILL(lbits)(&zghz3170);
  KILL(lbits)(&zghz3169);
  KILL(sail_int)(&zghz3168);
  KILL(lbits)(&zghz3167);
  KILL(sail_int)(&zghz3166);
  KILL(sail_int)(&zghz3165);
  KILL(sail_int)(&zghz3164);
  KILL(lbits)(&zghz3163);
  KILL(sail_int)(&zghz3162);
  KILL(lbits)(&zghz3161);
  KILL(sail_int)(&zghz3160);
  KILL(sail_int)(&zghz3159);
  KILL(sail_int)(&zghz3158);
  KILL(sail_int)(&zghz3157);
  KILL(sail_int)(&zghz3156);
  KILL(sail_int)(&zghz3155);
  KILL(sail_int)(&zghz3154);
  KILL(sail_int)(&zghz3153);
  KILL(sail_int)(&zghz3152);
  KILL(sail_int)(&zghz3151);
  KILL(sail_int)(&zghz3150);
  KILL(lbits)(&zghz3149);
  KILL(sail_int)(&zghz3148);
  KILL(sail_int)(&zghz3147);
  KILL(sail_int)(&zghz3146);
  KILL(sail_int)(&zghz3145);
  KILL(sail_int)(&zghz3144);
  KILL(lbits)(&zghz3143);
  KILL(sail_int)(&zghz3142);
  KILL(sail_int)(&zghz3141);
  KILL(lbits)(&zghz3140);
  KILL(lbits)(&zghz3139);
  KILL(sail_int)(&zghz3138);
  KILL(sail_int)(&zghz3137);
  KILL(sail_int)(&zghz3136);
  KILL(sail_int)(&zghz3135);
  KILL(sail_int)(&zghz3134);
  KILL(sail_int)(&zghz3133);
  KILL(lbits)(&zghz3132);
  KILL(sail_int)(&zghz3131);
  KILL(sail_int)(&zghz3130);
  KILL(sail_int)(&zghz3129);
  KILL(sail_int)(&zghz3128);
  KILL(sail_int)(&zghz3127);
  KILL(sail_int)(&zghz3126);
  KILL(sail_int)(&zghz3125);
  KILL(sail_int)(&zghz3124);
  KILL(sail_int)(&zghz3123);
  KILL(sail_int)(&zghz3122);
  KILL(sail_int)(&zghz3121);
  KILL(sail_int)(&zghz3120);
  KILL(sail_int)(&zghz3119);
  KILL(sail_int)(&zghz3118);
  KILL(lbits)(&zghz3117);
  KILL(lbits)(&zghz3116);
  KILL(lbits)(&zghz3115);
  KILL(lbits)(&zghz3114);
  KILL(lbits)(&zghz3113);
  KILL(lbits)(&zghz3112);
  KILL(sail_int)(&zghz3111);
  KILL(sail_int)(&zghz3110);
  KILL(sail_int)(&zghz3109);
  KILL(sail_int)(&zghz3108);
  KILL(sail_int)(&zghz3107);
  KILL(sail_int)(&zghz3106);
  KILL(lbits)(&zghz3105);
  KILL(sail_int)(&zghz3104);
  KILL(sail_int)(&zghz3103);
  KILL(lbits)(&zghz3102);
  KILL(sail_int)(&zghz3101);
  KILL(lbits)(&zghz3100);
  KILL(lbits)(&zghz399);
  KILL(sail_int)(&zghz398);
  KILL(sail_int)(&zghz397);
  KILL(lbits)(&zghz396);
  KILL(sail_int)(&zghz395);
  KILL(lbits)(&zghz394);
  KILL(lbits)(&zghz393);
  KILL(sail_int)(&zghz392);
  KILL(sail_int)(&zghz391);
  KILL(lbits)(&zghz390);
  KILL(sail_int)(&zghz389);
  KILL(sail_int)(&zghz388);
  KILL(sail_int)(&zghz387);
  KILL(sail_int)(&zghz386);
  KILL(sail_int)(&zghz385);
  KILL(sail_int)(&zghz384);
  KILL(lbits)(&zghz383);
  KILL(lbits)(&zghz382);
  KILL(sail_int)(&zghz381);
  KILL(sail_int)(&zghz380);
  KILL(lbits)(&zghz379);
  KILL(sail_int)(&zghz378);
  KILL(sail_int)(&zghz377);
  KILL(lbits)(&zghz376);
  KILL(lbits)(&zghz375);
  KILL(sail_int)(&zghz374);
  KILL(sail_int)(&zghz373);
  KILL(lbits)(&zghz372);
  KILL(lbits)(&zghz371);
  KILL(lbits)(&zghz370);
  KILL(lbits)(&zghz369);
  KILL(lbits)(&zghz368);
}

static bool sailgen_CapBoundsEqual(lbits, lbits);

lbits zghz3172;
lbits zghz3173;
lbits zghz3174;
lbits zghz3175;
lbits zghz3176;
lbits zghz3177;
lbits zghz3178;
lbits zghz3179;
lbits zghz3180;
lbits zghz3181;
lbits zghz3182;
lbits zghz3183;

static void startup_sailgen_CapBoundsEqual(void)
{
  CREATE(lbits)(&zghz3172);
  CREATE(lbits)(&zghz3173);
  CREATE(lbits)(&zghz3174);
  CREATE(lbits)(&zghz3175);
  CREATE(lbits)(&zghz3176);
  CREATE(lbits)(&zghz3177);
  CREATE(lbits)(&zghz3178);
  CREATE(lbits)(&zghz3179);
  CREATE(lbits)(&zghz3180);
  CREATE(lbits)(&zghz3181);
  CREATE(lbits)(&zghz3182);
  CREATE(lbits)(&zghz3183);
}

static bool sailgen_CapBoundsEqual(lbits za, lbits zb)
{
  __label__ cleanup_142, end_cleanup_143, end_function_141, end_block_exception_144;

  bool zcbz323;
  RECREATE(lbits)(&zghz3172);
  {
    RECREATE(lbits)(&zghz3183);
    CONVERT_OF(lbits, fbits)(&zghz3183, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3183, zghz3183, UINT64_C(0x0000000000000000));
    COPY(lbits)(&zghz3172, zghz3183);
  }
  RECREATE(lbits)(&zghz3173);
  {
    RECREATE(lbits)(&zghz3182);
    CONVERT_OF(lbits, fbits)(&zghz3182, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3182, zghz3182, UINT64_C(0x0000000000000000));
    COPY(lbits)(&zghz3173, zghz3182);
  }
  bool zavalid;
  zavalid = false;
  RECREATE(lbits)(&zghz3174);
  {
    RECREATE(lbits)(&zghz3181);
    CONVERT_OF(lbits, fbits)(&zghz3181, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3181, zghz3181, UINT64_C(0x0000000000000000));
    COPY(lbits)(&zghz3174, zghz3181);
  }
  RECREATE(lbits)(&zghz3175);
  {
    RECREATE(lbits)(&zghz3180);
    CONVERT_OF(lbits, fbits)(&zghz3180, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3180, zghz3180, UINT64_C(0x0000000000000000));
    COPY(lbits)(&zghz3175, zghz3180);
  }
  bool zbvalid;
  zbvalid = false;
  {
    __label__ case_138, finish_match_137;

    struct ztuple_z8z5bvzCz0z5bvzCz0z5boolz9 zgaz3103;
    CREATE(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgaz3103);
    sailgen_CapGetBounds(&zgaz3103, za);
    unit zgsz371;
    {
      RECREATE(lbits)(&zghz3178);
      COPY(lbits)(&zghz3178, zgaz3103.ztup0);
      RECREATE(lbits)(&zghz3179);
      COPY(lbits)(&zghz3179, zgaz3103.ztup1);
      bool ztup__2;
      ztup__2 = zgaz3103.ztup2;
      {
        COPY(lbits)(&zghz3172, zghz3178);
        unit zgsz373;
        zgsz373 = UNIT;
      }
      {
        COPY(lbits)(&zghz3173, zghz3179);
        unit zgsz372;
        zgsz372 = UNIT;
      }
      zavalid = ztup__2;
      zgsz371 = UNIT;
      goto finish_match_137;
    }
  case_138: ;
    sail_match_failure("CapBoundsEqual");
  finish_match_137: ;
    unit zgsz380;
    zgsz380 = zgsz371;
    KILL(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgaz3103);
  }
  {
    __label__ case_140, finish_match_139;

    struct ztuple_z8z5bvzCz0z5bvzCz0z5boolz9 zgaz3104;
    CREATE(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgaz3104);
    sailgen_CapGetBounds(&zgaz3104, zb);
    unit zgsz375;
    {
      RECREATE(lbits)(&zghz3176);
      COPY(lbits)(&zghz3176, zgaz3104.ztup0);
      RECREATE(lbits)(&zghz3177);
      COPY(lbits)(&zghz3177, zgaz3104.ztup1);
      bool zuz32;
      zuz32 = zgaz3104.ztup2;
      {
        COPY(lbits)(&zghz3174, zghz3176);
        unit zgsz377;
        zgsz377 = UNIT;
      }
      {
        COPY(lbits)(&zghz3175, zghz3177);
        unit zgsz376;
        zgsz376 = UNIT;
      }
      zbvalid = zuz32;
      zgsz375 = UNIT;
      goto finish_match_139;
    }
  case_140: ;
    sail_match_failure("CapBoundsEqual");
  finish_match_139: ;
    unit zgsz379;
    zgsz379 = zgsz375;
    KILL(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgaz3104);
  }
  bool zgaz3108;
  {
    bool zgaz3107;
    {
      bool zgaz3106;
      {
        bool zgaz3105;
        zgaz3105 = eq_bits(zghz3172, zghz3174);
        bool zgsz381;
        if (zgaz3105) {  zgsz381 = eq_bits(zghz3173, zghz3175);  } else {  zgsz381 = false;  }
        zgaz3106 = zgsz381;
      }
      bool zgsz382;
      if (zgaz3106) {  zgsz382 = zavalid;  } else {  zgsz382 = false;  }
      zgaz3107 = zgsz382;
    }
    bool zgsz383;
    if (zgaz3107) {  zgsz383 = zbvalid;  } else {  zgsz383 = false;  }
    zgaz3108 = zgsz383;
  }
  zcbz323 = zgaz3108;
  goto cleanup_142;
  /* unreachable after return */



  goto end_cleanup_143;
cleanup_142: ;



  goto end_function_141;
end_cleanup_143: ;
end_function_141: ;
  return zcbz323;
end_block_exception_144: ;

  return false;
}



static void finish_sailgen_CapBoundsEqual(void)
{
  KILL(lbits)(&zghz3183);
  KILL(lbits)(&zghz3182);
  KILL(lbits)(&zghz3181);
  KILL(lbits)(&zghz3180);
  KILL(lbits)(&zghz3179);
  KILL(lbits)(&zghz3178);
  KILL(lbits)(&zghz3177);
  KILL(lbits)(&zghz3176);
  KILL(lbits)(&zghz3175);
  KILL(lbits)(&zghz3174);
  KILL(lbits)(&zghz3173);
  KILL(lbits)(&zghz3172);
}

static bool sailgen_CapIsRepresentable(lbits, uint64_t);

lbits zghz3184;
sail_int zghz3185;
sail_int zghz3186;
lbits zghz3187;

static void startup_sailgen_CapIsRepresentable(void)
{
  CREATE(lbits)(&zghz3184);
  CREATE(sail_int)(&zghz3185);
  CREATE(sail_int)(&zghz3186);
  CREATE(lbits)(&zghz3187);
}

static bool sailgen_CapIsRepresentable(lbits zc, uint64_t zaddress)
{
  __label__ cleanup_147, end_cleanup_148, end_function_146, end_block_exception_149;

  bool zcbz324;
  RECREATE(lbits)(&zghz3184);
  COPY(lbits)(&zghz3184, zc);
  {
    {
      RECREATE(sail_int)(&zghz3185);
      CONVERT_OF(sail_int, mach_int)(&zghz3185, zCAP_VALUE_HI_BIT);
      RECREATE(sail_int)(&zghz3186);
      CONVERT_OF(sail_int, mach_int)(&zghz3186, zCAP_VALUE_LO_BIT);
      RECREATE(lbits)(&zghz3187);
      CONVERT_OF(lbits, fbits)(&zghz3187, zaddress, UINT64_C(64) , true);
      vector_update_subrange_lbits(&zghz3184, zghz3184, zghz3185, zghz3186, zghz3187);
    }
    unit zgsz384;
    zgsz384 = UNIT;
  }
  zcbz324 = sailgen_CapBoundsEqual(zc, zghz3184);
  goto cleanup_147;
  /* unreachable after return */
  goto end_cleanup_148;
cleanup_147: ;
  goto end_function_146;
end_cleanup_148: ;
end_function_146: ;
  return zcbz324;
end_block_exception_149: ;

  return false;
}



static void finish_sailgen_CapIsRepresentable(void)
{
  KILL(lbits)(&zghz3187);
  KILL(sail_int)(&zghz3186);
  KILL(sail_int)(&zghz3185);
  KILL(lbits)(&zghz3184);
}

static int64_t zCAP_TAG_BIT;
static void create_letbind_23(void) {


  int64_t zgsz385;
  zgsz385 = INT64_C(128);
  zCAP_TAG_BIT = zgsz385;

let_end_150: ;
}
static void kill_letbind_23(void) {
}

static void sailgen_CapSetTag(lbits *rop, lbits, uint64_t);

lbits zghz3188;
sail_int zghz3189;

static void startup_sailgen_CapSetTag(void)
{
  CREATE(lbits)(&zghz3188);
  CREATE(sail_int)(&zghz3189);
}

static void sailgen_CapSetTag(lbits *zcbz325, lbits zc, uint64_t zt)
{
  __label__ cleanup_153, end_cleanup_154, end_function_152, end_block_exception_155, end_function_280;

  RECREATE(lbits)(&zghz3188);
  COPY(lbits)(&zghz3188, zc);
  {
    fbits zgaz3112;
    {
      uint64_t zgaz3111;
      {
        fbits zgaz3110;
        zgaz3110 = (UINT64_C(1) & (zt >> INT64_C(0)));
        zgaz3111 = UINT64_C(0b0);
        zgaz3111 = update_fbits(zgaz3111, INT64_C(0), zgaz3110);
      }
      zgaz3112 = sailgen_Bit(zgaz3111);
    }
    {
      RECREATE(sail_int)(&zghz3189);
      CONVERT_OF(sail_int, mach_int)(&zghz3189, zCAP_TAG_BIT);
      update_lbits(&zghz3188, zghz3188, zghz3189, zgaz3112);
    }
    unit zgsz387;
    zgsz387 = UNIT;
  }
  COPY(lbits)((*(&zcbz325)), zghz3188);
  goto cleanup_153;
  /* unreachable after return */
  goto end_cleanup_154;
cleanup_153: ;
  goto end_function_152;
end_cleanup_154: ;
end_function_152: ;
  goto end_function_280;
end_block_exception_155: ;
  goto end_function_280;
end_function_280: ;
}



static void finish_sailgen_CapSetTag(void)
{
  KILL(sail_int)(&zghz3189);
  KILL(lbits)(&zghz3188);
}

static void sailgen_CapWithTagClear(lbits *rop, lbits);

lbits zghz3190;

static void startup_sailgen_CapWithTagClear(void)
{    CREATE(lbits)(&zghz3190);
}

static void sailgen_CapWithTagClear(lbits *zcbz326, lbits zc)
{
  __label__ cleanup_158, end_cleanup_159, end_function_157, end_block_exception_160, end_function_279;

  RECREATE(lbits)(&zghz3190);
  sailgen_CapSetTag(&zghz3190, zc, UINT64_C(0x0000000000000000));
  COPY(lbits)((*(&zcbz326)), zghz3190);
  goto cleanup_158;
  /* unreachable after return */
  goto end_cleanup_159;
cleanup_158: ;
  goto end_function_157;
end_cleanup_159: ;
end_function_157: ;
  goto end_function_279;
end_block_exception_160: ;
  goto end_function_279;
end_function_279: ;
}



static void finish_sailgen_CapWithTagClear(void)
{    KILL(lbits)(&zghz3190);
}

// register _PC
static uint64_t z_PC;

// register __BranchTaken
static bool z__BranchTaken;

static int64_t zCAP_OTYPE_HI_BIT;
static void create_letbind_24(void) {


  int64_t zgsz388;
  zgsz388 = INT64_C(109);
  zCAP_OTYPE_HI_BIT = zgsz388;

let_end_161: ;
}
static void kill_letbind_24(void) {
}

static int64_t zCAP_OTYPE_LO_BIT;
static void create_letbind_25(void) {


  int64_t zgsz389;
  zgsz389 = INT64_C(95);
  zCAP_OTYPE_LO_BIT = zgsz389;

let_end_162: ;
}
static void kill_letbind_25(void) {
}

static uint64_t sailgen_CapGetObjectType(lbits);

lbits zghz3191;
sail_int zghz3192;
lbits zghz3193;
sail_int zghz3194;
sail_int zghz3195;
lbits zghz3196;

static void startup_sailgen_CapGetObjectType(void)
{
  CREATE(lbits)(&zghz3191);
  CREATE(sail_int)(&zghz3192);
  CREATE(lbits)(&zghz3193);
  CREATE(sail_int)(&zghz3194);
  CREATE(sail_int)(&zghz3195);
  CREATE(lbits)(&zghz3196);
}

static uint64_t sailgen_CapGetObjectType(lbits zc)
{
  __label__ cleanup_165, end_cleanup_166, end_function_164, end_block_exception_167;

  uint64_t zcbz327;
  {
    uint64_t zgaz3114;
    {
      RECREATE(sail_int)(&zghz3194);
      CONVERT_OF(sail_int, mach_int)(&zghz3194, zCAP_OTYPE_HI_BIT);
      RECREATE(sail_int)(&zghz3195);
      CONVERT_OF(sail_int, mach_int)(&zghz3195, zCAP_OTYPE_LO_BIT);
      RECREATE(lbits)(&zghz3196);
      vector_subrange_lbits(&zghz3196, zc, zghz3194, zghz3195);
      zgaz3114 = CONVERT_OF(fbits, lbits)(zghz3196, true);
    }
    {
      RECREATE(lbits)(&zghz3191);
      CONVERT_OF(lbits, fbits)(&zghz3191, zgaz3114, UINT64_C(15) , true);
      RECREATE(sail_int)(&zghz3192);
      CONVERT_OF(sail_int, mach_int)(&zghz3192, zCAP_VALUE_NUM_BITS);
      RECREATE(lbits)(&zghz3193);
      zero_extend(&zghz3193, zghz3191, zghz3192);
      zcbz327 = CONVERT_OF(fbits, lbits)(zghz3193, true);
    }
  }
  goto cleanup_165;
  /* unreachable after return */
  goto end_cleanup_166;
cleanup_165: ;
  goto end_function_164;
end_cleanup_166: ;
end_function_164: ;
  return zcbz327;
end_block_exception_167: ;

  return UINT64_C(0xdeadc0de);
}



static void finish_sailgen_CapGetObjectType(void)
{
  KILL(lbits)(&zghz3196);
  KILL(sail_int)(&zghz3195);
  KILL(sail_int)(&zghz3194);
  KILL(lbits)(&zghz3193);
  KILL(sail_int)(&zghz3192);
  KILL(lbits)(&zghz3191);
}

static bool sailgen_CapIsSealed(lbits);

sail_int zghz3197;
lbits zghz3198;

static void startup_sailgen_CapIsSealed(void)
{
  CREATE(sail_int)(&zghz3197);
  CREATE(lbits)(&zghz3198);
}

static bool sailgen_CapIsSealed(lbits zc)
{
  __label__ cleanup_170, end_cleanup_171, end_function_169, end_block_exception_172;

  bool zcbz328;
  {
    uint64_t zgaz3116;
    zgaz3116 = sailgen_CapGetObjectType(zc);
    uint64_t zgaz3117;
    {
      RECREATE(sail_int)(&zghz3197);
      CONVERT_OF(sail_int, mach_int)(&zghz3197, zCAP_VALUE_NUM_BITS);
      RECREATE(lbits)(&zghz3198);
      sailgen_Zeros(&zghz3198, zghz3197);
      zgaz3117 = CONVERT_OF(fbits, lbits)(zghz3198, true);
    }
    zcbz328 = (zgaz3116 != zgaz3117);
  }
  goto cleanup_170;
  /* unreachable after return */
  goto end_cleanup_171;
cleanup_170: ;
  goto end_function_169;
end_cleanup_171: ;
end_function_169: ;
  return zcbz328;
end_block_exception_172: ;

  return false;
}



static void finish_sailgen_CapIsSealed(void)
{
  KILL(lbits)(&zghz3198);
  KILL(sail_int)(&zghz3197);
}

static int64_t zCAP_FLAGS_HI_BIT;
static void create_letbind_26(void) {


  int64_t zgsz390;
  zgsz390 = INT64_C(63);
  zCAP_FLAGS_HI_BIT = zgsz390;

let_end_173: ;
}
static void kill_letbind_26(void) {
}

// register CCTLR_EL0
static uint64_t zCCTLR_EL0;

// register CCTLR_EL1
static uint64_t zCCTLR_EL1;

// register CCTLR_EL2
static uint64_t zCCTLR_EL2;

// register CCTLR_EL3
static uint64_t zCCTLR_EL3;

// register ELR_EL1
static lbits zELR_EL1;

// register ELR_EL2
static lbits zELR_EL2;

// register ELR_EL3
static lbits zELR_EL3;

// register VBAR_EL1
static lbits zVBAR_EL1;

// register VBAR_EL2
static lbits zVBAR_EL2;

// register VBAR_EL3
static lbits zVBAR_EL3;

// register CPACR_EL1
static uint64_t zCPACR_EL1;

// register CPTR_EL2
static uint64_t zCPTR_EL2;

// register CPTR_EL3
static uint64_t zCPTR_EL3;

static uint64_t zM32_Monitor;
static void create_letbind_27(void) {


  uint64_t zgsz391;
  zgsz391 = UINT64_C(0b10110);
  zM32_Monitor = zgsz391;

let_end_174: ;
}
static void kill_letbind_27(void) {
}

static uint64_t zCAP_PERM_EXECUTIVE;
static void create_letbind_28(void) {


  uint64_t zgsz392;
  zgsz392 = UINT64_C(0x0000000000000002);
  zCAP_PERM_EXECUTIVE = zgsz392;

let_end_175: ;
}
static void kill_letbind_28(void) {
}

static int64_t zCAP_PERMS_HI_BIT;
static void create_letbind_29(void) {


  int64_t zgsz393;
  zgsz393 = INT64_C(127);
  zCAP_PERMS_HI_BIT = zgsz393;

let_end_176: ;
}
static void kill_letbind_29(void) {
}

static int64_t zCAP_PERMS_LO_BIT;
static void create_letbind_30(void) {


  int64_t zgsz394;
  zgsz394 = INT64_C(110);
  zCAP_PERMS_LO_BIT = zgsz394;

let_end_177: ;
}
static void kill_letbind_30(void) {
}

static int64_t zCAP_PERMS_NUM_BITS;
static void create_letbind_31(void) {


  int64_t zgsz395;
  int64_t zgaz3119;
  {
    sail_int zgsz3851;
    CREATE(sail_int)(&zgsz3851);
    CONVERT_OF(sail_int, mach_int)(&zgsz3851, zCAP_PERMS_HI_BIT);
    sail_int zgsz3852;
    CREATE(sail_int)(&zgsz3852);
    CONVERT_OF(sail_int, mach_int)(&zgsz3852, zCAP_PERMS_LO_BIT);
    sail_int zgsz3853;
    CREATE(sail_int)(&zgsz3853);
    sub_int(&zgsz3853, zgsz3851, zgsz3852);
    zgaz3119 = CONVERT_OF(mach_int, sail_int)(zgsz3853);
    KILL(sail_int)(&zgsz3853);
    KILL(sail_int)(&zgsz3852);
    KILL(sail_int)(&zgsz3851);
  }
  zgsz395 = (zgaz3119 + INT64_C(1));

  zCAP_PERMS_NUM_BITS = zgsz395;

let_end_178: ;
}
static void kill_letbind_31(void) {
}

static uint64_t sailgen_CapGetPermissions(lbits);

sail_int zghz3199;
sail_int zghz3200;
lbits zghz3201;

static void startup_sailgen_CapGetPermissions(void)
{
  CREATE(sail_int)(&zghz3199);
  CREATE(sail_int)(&zghz3200);
  CREATE(lbits)(&zghz3201);
}

static uint64_t sailgen_CapGetPermissions(lbits zc)
{
  __label__ cleanup_181, end_cleanup_182, end_function_180, end_block_exception_183;

  uint64_t zcbz329;
  {
    RECREATE(sail_int)(&zghz3199);
    CONVERT_OF(sail_int, mach_int)(&zghz3199, zCAP_PERMS_HI_BIT);
    RECREATE(sail_int)(&zghz3200);
    CONVERT_OF(sail_int, mach_int)(&zghz3200, zCAP_PERMS_LO_BIT);
    RECREATE(lbits)(&zghz3201);
    vector_subrange_lbits(&zghz3201, zc, zghz3199, zghz3200);
    zcbz329 = CONVERT_OF(fbits, lbits)(zghz3201, true);
  }
  goto cleanup_181;
  /* unreachable after return */
  goto end_cleanup_182;
cleanup_181: ;
  goto end_function_180;
end_cleanup_182: ;
end_function_180: ;
  return zcbz329;
end_block_exception_183: ;

  return UINT64_C(0xdeadc0de);
}



static void finish_sailgen_CapGetPermissions(void)
{
  KILL(lbits)(&zghz3201);
  KILL(sail_int)(&zghz3200);
  KILL(sail_int)(&zghz3199);
}

// register EDSCR
static uint64_t zEDSCR;

// register SCTLR_EL1
static uint64_t zSCTLR_EL1;

// register SCTLR_EL2
static uint64_t zSCTLR_EL2;

// register SCTLR_EL3
static uint64_t zSCTLR_EL3;

// register SPSR_EL1
static uint64_t zSPSR_EL1;

// register SPSR_EL2
static uint64_t zSPSR_EL2;

// register SPSR_EL3
static uint64_t zSPSR_EL3;

// register ACTLR_EL1
static uint64_t zACTLR_EL1;

static uint64_t zCAP_PERM_EXECUTE;
static void create_letbind_32(void) {


  uint64_t zgsz396;
  zgsz396 = UINT64_C(0x0000000000008000);
  zCAP_PERM_EXECUTE = zgsz396;

let_end_184: ;
}
static void kill_letbind_32(void) {
}

static uint64_t zCAP_PERM_SYSTEM;
static void create_letbind_33(void) {


  uint64_t zgsz397;
  zgsz397 = UINT64_C(0x0000000000000200);
  zCAP_PERM_SYSTEM = zgsz397;

let_end_185: ;
}
static void kill_letbind_33(void) {
}

// register ACTLR_EL2
static uint64_t zACTLR_EL2;

// register ACTLR_EL3
static uint64_t zACTLR_EL3;

// register AFSR0_EL1
static uint64_t zAFSR0_EL1;

// register AFSR0_EL2
static uint64_t zAFSR0_EL2;

// register AFSR0_EL3
static uint64_t zAFSR0_EL3;

// register AFSR1_EL1
static uint64_t zAFSR1_EL1;

// register AFSR1_EL2
static uint64_t zAFSR1_EL2;

// register AFSR1_EL3
static uint64_t zAFSR1_EL3;

// register AIDR_EL1
static uint64_t zAIDR_EL1;

// register AMAIR_EL1
static uint64_t zAMAIR_EL1;

// register AMAIR_EL2
static uint64_t zAMAIR_EL2;

// register AMAIR_EL3
static uint64_t zAMAIR_EL3;

// register CCSIDR_EL1
static uint64_t zCCSIDR_EL1;

// register CHCR_EL2
static uint64_t zCHCR_EL2;

// register CLIDR_EL1
static uint64_t zCLIDR_EL1;

// register CNTFRQ_EL0
static uint64_t zCNTFRQ_EL0;

// register CNTHCTL_EL2
static uint64_t zCNTHCTL_EL2;

// register CNTKCTL_EL1
static uint64_t zCNTKCTL_EL1;

// register CNTHP_CTL_EL2
static uint64_t zCNTHP_CTL_EL2;

// register CNTHP_CVAL_EL2
static uint64_t zCNTHP_CVAL_EL2;

// register CNTHP_TVAL_EL2
static uint64_t zCNTHP_TVAL_EL2;

// register CNTHV_CTL_EL2
static uint64_t zCNTHV_CTL_EL2;

// register CNTHV_CVAL_EL2
static uint64_t zCNTHV_CVAL_EL2;

// register CNTHV_TVAL_EL2
static uint64_t zCNTHV_TVAL_EL2;

// register CNTPCT_EL0
static uint64_t zCNTPCT_EL0;

// register CNTPS_CTL_EL1
static uint64_t zCNTPS_CTL_EL1;

// register CNTPS_CVAL_EL1
static uint64_t zCNTPS_CVAL_EL1;

// register CNTPS_TVAL_EL1
static uint64_t zCNTPS_TVAL_EL1;

// register CNTP_CTL_EL0
static uint64_t zCNTP_CTL_EL0;

// register CNTP_CVAL_EL0
static uint64_t zCNTP_CVAL_EL0;

// register CNTP_TVAL_EL0
static uint64_t zCNTP_TVAL_EL0;

// register CNTVCT_EL0
static uint64_t zCNTVCT_EL0;

// register CNTVOFF_EL2
static uint64_t zCNTVOFF_EL2;

// register CNTV_CTL_EL0
static uint64_t zCNTV_CTL_EL0;

// register CNTV_CVAL_EL0
static uint64_t zCNTV_CVAL_EL0;

// register CNTV_TVAL_EL0
static uint64_t zCNTV_TVAL_EL0;

// register CONTEXTIDR_EL1
static uint64_t zCONTEXTIDR_EL1;

// register CONTEXTIDR_EL2
static uint64_t zCONTEXTIDR_EL2;

// register CSCR_EL3
static uint64_t zCSCR_EL3;

// register CSSELR_EL1
static uint64_t zCSSELR_EL1;

// register CTR_EL0
static uint64_t zCTR_EL0;

// register DACR32_EL2
static uint64_t zDACR32_EL2;

// register DBGAUTHSTATUS_EL1
static uint64_t zDBGAUTHSTATUS_EL1;

// register MDCR_EL2
static uint64_t zMDCR_EL2;

// register MDCR_EL3
static uint64_t zMDCR_EL3;

struct zz5vecz8z5bv32z9 {
  size_t len;
  uint64_t *data;
};
typedef struct zz5vecz8z5bv32z9 zz5vecz8z5bv32z9;

static void CREATE(zz5vecz8z5bv32z9)(zz5vecz8z5bv32z9 *rop) {
  rop->len = 0;
  rop->data = NULL;
}

static void KILL(zz5vecz8z5bv32z9)(zz5vecz8z5bv32z9 *rop) {
  if (rop->data != NULL) sail_free(rop->data);
}

static void vector_init_zz5vecz8z5bv32z9(zz5vecz8z5bv32z9 *vec, sail_int n, uint64_t elem) {
  KILL(zz5vecz8z5bv32z9)(vec);
  size_t m = (size_t)sail_int_get_ui(n);
  vec->len = m;
  vec->data = sail_new_array(uint64_t, m);
  for (size_t i = 0; i < m; i++) {
    (vec->data)[i] = elem;
  }
}

static void RECREATE(zz5vecz8z5bv32z9)(zz5vecz8z5bv32z9 *rop) {
  KILL(zz5vecz8z5bv32z9)(rop);
  CREATE(zz5vecz8z5bv32z9)(rop);
}

static void undefined_vector_zz5vecz8z5bv32z9(zz5vecz8z5bv32z9 *rop, sail_int len, uint64_t elem) {
  rop->len = sail_int_get_ui(len);
  rop->data = sail_new_array(uint64_t, rop->len);
  for (int i = 0; i < (rop->len); i++) {
    (rop->data)[i] = elem;
  }
}

static uint64_t vector_access_zz5vecz8z5bv32z9(zz5vecz8z5bv32z9 op, sail_int n) {
  int m = sail_int_get_ui(n);
  return op.data[m];
}

static void COPY(zz5vecz8z5bv32z9)(zz5vecz8z5bv32z9 *rop, zz5vecz8z5bv32z9 op) {
  KILL(zz5vecz8z5bv32z9)(rop);
  rop->len = op.len;
  rop->data = sail_new_array(uint64_t, rop->len);
  for (int i = 0; i < op.len; i++) {
    (rop->data)[i] = op.data[i];
  }
}

static void vector_update_zz5vecz8z5bv32z9(zz5vecz8z5bv32z9 *rop, zz5vecz8z5bv32z9 op, sail_int n, uint64_t elem) {
  int m = sail_int_get_ui(n);
  if (rop->data == op.data) {
    rop->data[m] = elem;
  } else {
    COPY(zz5vecz8z5bv32z9)(rop, op);
    rop->data[m] = elem;
  }
}

static bool EQUAL(zz5vecz8z5bv32z9)(const zz5vecz8z5bv32z9 op1, const zz5vecz8z5bv32z9 op2) {
  if (op1.len != op2.len) return false;
  bool result = true;
  for (int i = 0; i < op1.len; i++) {
    result &= EQUAL(fbits)(op1.data[i], op2.data[i]);
  }
  return result;
}

static void length_zz5vecz8z5bv32z9(sail_int *rop, zz5vecz8z5bv32z9 op) {
  mpz_set_ui(*rop, (unsigned long int)(op.len));
}

static void internal_vector_update_zz5vecz8z5bv32z9(zz5vecz8z5bv32z9 *rop, zz5vecz8z5bv32z9 op, const int64_t n, uint64_t elem) {
  rop->data[n] = elem;
}

static void internal_vector_init_zz5vecz8z5bv32z9(zz5vecz8z5bv32z9 *rop, const int64_t len) {
  rop->len = len;
  rop->data = sail_new_array(uint64_t, len);
}

// register DBGBCR_EL1
static zz5vecz8z5bv32z9 zDBGBCR_EL1;

static uint64_t zDebugHalt_SoftwareAccess;
static void create_letbind_34(void) {


  uint64_t zgsz398;
  zgsz398 = UINT64_C(0b110011);
  zDebugHalt_SoftwareAccess = zgsz398;

let_end_186: ;
}
static void kill_letbind_34(void) {
}

// register CDLR_EL0
static lbits zCDLR_EL0;

// register DSPSR_EL0
static uint64_t zDSPSR_EL0;

// register DBGEN
static enum zsignal zDBGEN;

// register SPIDEN
static enum zsignal zSPIDEN;

// register DBGPRCR_EL1
static uint64_t zDBGPRCR_EL1;

// register OSDLR_EL1
static uint64_t zOSDLR_EL1;

// register OSLSR_EL1
static uint64_t zOSLSR_EL1;

struct zz5vecz8z5bv64z9 {
  size_t len;
  uint64_t *data;
};
typedef struct zz5vecz8z5bv64z9 zz5vecz8z5bv64z9;

static void CREATE(zz5vecz8z5bv64z9)(zz5vecz8z5bv64z9 *rop) {
  rop->len = 0;
  rop->data = NULL;
}

static void KILL(zz5vecz8z5bv64z9)(zz5vecz8z5bv64z9 *rop) {
  if (rop->data != NULL) sail_free(rop->data);
}

static void vector_init_zz5vecz8z5bv64z9(zz5vecz8z5bv64z9 *vec, sail_int n, uint64_t elem) {
  KILL(zz5vecz8z5bv64z9)(vec);
  size_t m = (size_t)sail_int_get_ui(n);
  vec->len = m;
  vec->data = sail_new_array(uint64_t, m);
  for (size_t i = 0; i < m; i++) {
    (vec->data)[i] = elem;
  }
}

static void RECREATE(zz5vecz8z5bv64z9)(zz5vecz8z5bv64z9 *rop) {
  KILL(zz5vecz8z5bv64z9)(rop);
  CREATE(zz5vecz8z5bv64z9)(rop);
}

static void undefined_vector_zz5vecz8z5bv64z9(zz5vecz8z5bv64z9 *rop, sail_int len, uint64_t elem) {
  rop->len = sail_int_get_ui(len);
  rop->data = sail_new_array(uint64_t, rop->len);
  for (int i = 0; i < (rop->len); i++) {
    (rop->data)[i] = elem;
  }
}

static uint64_t vector_access_zz5vecz8z5bv64z9(zz5vecz8z5bv64z9 op, sail_int n) {
  int m = sail_int_get_ui(n);
  return op.data[m];
}

static void COPY(zz5vecz8z5bv64z9)(zz5vecz8z5bv64z9 *rop, zz5vecz8z5bv64z9 op) {
  KILL(zz5vecz8z5bv64z9)(rop);
  rop->len = op.len;
  rop->data = sail_new_array(uint64_t, rop->len);
  for (int i = 0; i < op.len; i++) {
    (rop->data)[i] = op.data[i];
  }
}

static void vector_update_zz5vecz8z5bv64z9(zz5vecz8z5bv64z9 *rop, zz5vecz8z5bv64z9 op, sail_int n, uint64_t elem) {
  int m = sail_int_get_ui(n);
  if (rop->data == op.data) {
    rop->data[m] = elem;
  } else {
    COPY(zz5vecz8z5bv64z9)(rop, op);
    rop->data[m] = elem;
  }
}

static bool EQUAL(zz5vecz8z5bv64z9)(const zz5vecz8z5bv64z9 op1, const zz5vecz8z5bv64z9 op2) {
  if (op1.len != op2.len) return false;
  bool result = true;
  for (int i = 0; i < op1.len; i++) {
    result &= EQUAL(fbits)(op1.data[i], op2.data[i]);
  }
  return result;
}

static void length_zz5vecz8z5bv64z9(sail_int *rop, zz5vecz8z5bv64z9 op) {
  mpz_set_ui(*rop, (unsigned long int)(op.len));
}

static void internal_vector_update_zz5vecz8z5bv64z9(zz5vecz8z5bv64z9 *rop, zz5vecz8z5bv64z9 op, const int64_t n, uint64_t elem) {
  rop->data[n] = elem;
}

static void internal_vector_init_zz5vecz8z5bv64z9(zz5vecz8z5bv64z9 *rop, const int64_t len) {
  rop->len = len;
  rop->data = sail_new_array(uint64_t, len);
}

// register DBGBVR_EL1
static zz5vecz8z5bv64z9 zDBGBVR_EL1;

// register DBGCLAIMCLR_EL1
static uint64_t zDBGCLAIMCLR_EL1;

// register DBGCLAIMSET_EL1
static uint64_t zDBGCLAIMSET_EL1;

// register DBGDTRRX_EL0
static uint64_t zDBGDTRRX_EL0;

// register MDSCR_EL1
static uint64_t zMDSCR_EL1;

// register CDBGDTR_EL0
static lbits zCDBGDTR_EL0;

// register DBGVCR32_EL2
static uint64_t zDBGVCR32_EL2;

// register DBGWCR_EL1
static zz5vecz8z5bv32z9 zDBGWCR_EL1;

// register DBGWVR_EL1
static zz5vecz8z5bv64z9 zDBGWVR_EL1;

// register DCZID_EL0
static uint64_t zDCZID_EL0;

// register DISR_EL1
static uint64_t zDISR_EL1;

// register VDISR_EL2
static uint64_t zVDISR_EL2;

// register ERRIDR_EL1
static uint64_t zERRIDR_EL1;

// register ERRSELR_EL1
static uint64_t zERRSELR_EL1;

// register ERXADDR_EL1
static uint64_t zERXADDR_EL1;

// register ERXCTLR_EL1
static uint64_t zERXCTLR_EL1;

// register ERXFR_EL1
static uint64_t zERXFR_EL1;

// register ERXMISC0_EL1
static uint64_t zERXMISC0_EL1;

// register ERXMISC1_EL1
static uint64_t zERXMISC1_EL1;

// register ERXSTATUS_EL1
static uint64_t zERXSTATUS_EL1;

// register FPCR
static uint64_t zFPCR;

// register FPEXC32_EL2
static uint64_t zFPEXC32_EL2;

// register FPSR
static uint64_t zFPSR;

// register HACR_EL2
static uint64_t zHACR_EL2;

// register HSTR_EL2
static uint64_t zHSTR_EL2;

// register ICC_AP0R_EL1
static zz5vecz8z5bv32z9 zICC_AP0R_EL1;

// register ICC_SRE_EL1_NS
static uint64_t zICC_SRE_EL1_NS;

// register ICC_SRE_EL1_S
static uint64_t zICC_SRE_EL1_S;

// register ICC_SRE_EL2
static uint64_t zICC_SRE_EL2;

// register ICC_SRE_EL3
static uint64_t zICC_SRE_EL3;

// register ICH_HCR_EL2
static uint64_t zICH_HCR_EL2;

// register ICV_AP0R_EL1
static zz5vecz8z5bv32z9 zICV_AP0R_EL1;

// register ICC_AP1R_EL1
static zz5vecz8z5bv32z9 zICC_AP1R_EL1;

// register ICC_AP1R_EL1_NS
static zz5vecz8z5bv32z9 zICC_AP1R_EL1_NS;

// register ICC_AP1R_EL1_S
static zz5vecz8z5bv32z9 zICC_AP1R_EL1_S;

// register ICV_AP1R_EL1
static zz5vecz8z5bv32z9 zICV_AP1R_EL1;

// register ICC_BPR0_EL1
static uint64_t zICC_BPR0_EL1;

// register ICV_BPR0_EL1
static uint64_t zICV_BPR0_EL1;

// register ICC_BPR1_EL1_NS
static uint64_t zICC_BPR1_EL1_NS;

// register ICC_BPR1_EL1_S
static uint64_t zICC_BPR1_EL1_S;

// register ICV_BPR1_EL1
static uint64_t zICV_BPR1_EL1;

// register ICC_CTLR_EL1_NS
static uint64_t zICC_CTLR_EL1_NS;

// register ICC_CTLR_EL1_S
static uint64_t zICC_CTLR_EL1_S;

// register ICV_CTLR_EL1
static uint64_t zICV_CTLR_EL1;

// register ICC_CTLR_EL3
static uint64_t zICC_CTLR_EL3;

// register ICC_HPPIR0_EL1
static uint64_t zICC_HPPIR0_EL1;

// register ICV_HPPIR0_EL1
static uint64_t zICV_HPPIR0_EL1;

// register ICC_HPPIR1_EL1
static uint64_t zICC_HPPIR1_EL1;

// register ICV_HPPIR1_EL1
static uint64_t zICV_HPPIR1_EL1;

// register ICC_IAR0_EL1
static uint64_t zICC_IAR0_EL1;

// register ICV_IAR0_EL1
static uint64_t zICV_IAR0_EL1;

// register ICC_IAR1_EL1
static uint64_t zICC_IAR1_EL1;

// register ICV_IAR1_EL1
static uint64_t zICV_IAR1_EL1;

// register ICC_IGRPEN0_EL1
static uint64_t zICC_IGRPEN0_EL1;

// register ICV_IGRPEN0_EL1
static uint64_t zICV_IGRPEN0_EL1;

// register ICC_IGRPEN1_EL1_NS
static uint64_t zICC_IGRPEN1_EL1_NS;

// register ICC_IGRPEN1_EL1_S
static uint64_t zICC_IGRPEN1_EL1_S;

// register ICV_IGRPEN1_EL1
static uint64_t zICV_IGRPEN1_EL1;

// register ICC_IGRPEN1_EL3
static uint64_t zICC_IGRPEN1_EL3;

// register ICC_PMR_EL1
static uint64_t zICC_PMR_EL1;

// register ICV_PMR_EL1
static uint64_t zICV_PMR_EL1;

// register ICC_RPR_EL1
static uint64_t zICC_RPR_EL1;

// register ICV_RPR_EL1
static uint64_t zICV_RPR_EL1;

// register ICH_AP0R_EL2
static zz5vecz8z5bv32z9 zICH_AP0R_EL2;

// register ICH_AP1R_EL2
static zz5vecz8z5bv32z9 zICH_AP1R_EL2;

// register ICH_EISR_EL2
static uint64_t zICH_EISR_EL2;

// register ICH_ELRSR_EL2
static uint64_t zICH_ELRSR_EL2;

// register ICH_LR_EL2
static zz5vecz8z5bv64z9 zICH_LR_EL2;

// register ICH_MISR_EL2
static uint64_t zICH_MISR_EL2;

// register ICH_VMCR_EL2
static uint64_t zICH_VMCR_EL2;

// register ICH_VTR_EL2
static uint64_t zICH_VTR_EL2;

// register ID_AA64AFR0_EL1
static uint64_t zID_AA64AFR0_EL1;

// register ID_AA64AFR1_EL1
static uint64_t zID_AA64AFR1_EL1;

// register ID_AA64DFR0_EL1
static uint64_t zID_AA64DFR0_EL1;

// register ID_AA64DFR1_EL1
static uint64_t zID_AA64DFR1_EL1;

// register ID_AA64ISAR0_EL1
static uint64_t zID_AA64ISAR0_EL1;

// register ID_AA64ISAR1_EL1
static uint64_t zID_AA64ISAR1_EL1;

// register ID_AA64MMFR0_EL1
static uint64_t zID_AA64MMFR0_EL1;

// register ID_AA64MMFR1_EL1
static uint64_t zID_AA64MMFR1_EL1;

// register ID_AA64MMFR2_EL1
static uint64_t zID_AA64MMFR2_EL1;

// register ID_AA64PFR0_EL1
static uint64_t zID_AA64PFR0_EL1;

// register ID_AA64PFR1_EL1
static uint64_t zID_AA64PFR1_EL1;

// register ID_AA64ZFR0_EL1
static uint64_t zID_AA64ZFR0_EL1;

// register ID_AFR0_EL1
static uint64_t zID_AFR0_EL1;

// register ID_DFR0_EL1
static uint64_t zID_DFR0_EL1;

// register ID_ISAR0_EL1
static uint64_t zID_ISAR0_EL1;

// register ID_ISAR1_EL1
static uint64_t zID_ISAR1_EL1;

// register ID_ISAR2_EL1
static uint64_t zID_ISAR2_EL1;

// register ID_ISAR3_EL1
static uint64_t zID_ISAR3_EL1;

// register ID_ISAR4_EL1
static uint64_t zID_ISAR4_EL1;

// register ID_ISAR5_EL1
static uint64_t zID_ISAR5_EL1;

// register ID_ISAR6_EL1
static uint64_t zID_ISAR6_EL1;

// register ID_MMFR0_EL1
static uint64_t zID_MMFR0_EL1;

// register ID_MMFR1_EL1
static uint64_t zID_MMFR1_EL1;

// register ID_MMFR2_EL1
static uint64_t zID_MMFR2_EL1;

// register ID_MMFR3_EL1
static uint64_t zID_MMFR3_EL1;

// register ID_MMFR4_EL1
static uint64_t zID_MMFR4_EL1;

// register ID_MMFR5_EL1
static uint64_t zID_MMFR5_EL1;

// register ID_PFR0_EL1
static uint64_t zID_PFR0_EL1;

// register ID_PFR1_EL1
static uint64_t zID_PFR1_EL1;

// register ID_PFR2_EL1
static uint64_t zID_PFR2_EL1;

// register IFSR32_EL2
static uint64_t zIFSR32_EL2;

// register ISR_EL1
static uint64_t zISR_EL1;

// register LORC_EL1
static uint64_t zLORC_EL1;

// register LOREA_EL1
static uint64_t zLOREA_EL1;

// register LORID_EL1
static uint64_t zLORID_EL1;

// register LORN_EL1
static uint64_t zLORN_EL1;

// register LORSA_EL1
static uint64_t zLORSA_EL1;

// register MAIR_EL1
static uint64_t zMAIR_EL1;

// register MAIR_EL2
static uint64_t zMAIR_EL2;

// register MAIR_EL3
static uint64_t zMAIR_EL3;

// register MDCCINT_EL1
static uint64_t zMDCCINT_EL1;

// register MDCCSR_EL0
static uint64_t zMDCCSR_EL0;

// register MDRAR_EL1
static uint64_t zMDRAR_EL1;

// register MIDR_EL1
static uint64_t zMIDR_EL1;

// register VPIDR_EL2
static uint64_t zVPIDR_EL2;

// register MPAM0_EL1
static uint64_t zMPAM0_EL1;

// register MPAM3_EL3
static uint64_t zMPAM3_EL3;

// register _MPAM2_EL2_0_62
static uint64_t z_MPAM2_EL2_0_62;

// register _MPAM1_EL1_0_62
static uint64_t z_MPAM1_EL1_0_62;

// register MPAMHCR_EL2
static uint64_t zMPAMHCR_EL2;

// register MPAMIDR_EL1
static uint64_t zMPAMIDR_EL1;

// register MPAMVPM0_EL2
static uint64_t zMPAMVPM0_EL2;

// register MPAMVPM1_EL2
static uint64_t zMPAMVPM1_EL2;

// register MPAMVPM2_EL2
static uint64_t zMPAMVPM2_EL2;

// register MPAMVPM3_EL2
static uint64_t zMPAMVPM3_EL2;

// register MPAMVPM4_EL2
static uint64_t zMPAMVPM4_EL2;

// register MPAMVPM5_EL2
static uint64_t zMPAMVPM5_EL2;

// register MPAMVPM6_EL2
static uint64_t zMPAMVPM6_EL2;

// register MPAMVPM7_EL2
static uint64_t zMPAMVPM7_EL2;

// register MPAMVPMV_EL2
static uint64_t zMPAMVPMV_EL2;

// register MPIDR_EL1
static uint64_t zMPIDR_EL1;

// register VMPIDR_EL2
static uint64_t zVMPIDR_EL2;

// register MVFR0_EL1
static uint64_t zMVFR0_EL1;

// register MVFR1_EL1
static uint64_t zMVFR1_EL1;

// register MVFR2_EL1
static uint64_t zMVFR2_EL1;

// register OSDTRRX_EL1
static uint64_t zOSDTRRX_EL1;

// register OSDTRTX_EL1
static uint64_t zOSDTRTX_EL1;

// register OSECCR_EL1
static uint64_t zOSECCR_EL1;

// register PAR_EL1
static uint64_t zPAR_EL1;

// register PMBIDR_EL1
static uint64_t zPMBIDR_EL1;

// register PMBLIMITR_EL1
static uint64_t zPMBLIMITR_EL1;

// register PMBPTR_EL1
static uint64_t zPMBPTR_EL1;

// register PMBSR_EL1
static uint64_t zPMBSR_EL1;

// register PMCCFILTR_EL0
static uint64_t zPMCCFILTR_EL0;

// register PMUSERENR_EL0
static uint64_t zPMUSERENR_EL0;

// register PMCCNTR_EL0
static uint64_t zPMCCNTR_EL0;

// register PMCEID0_EL0
static uint64_t zPMCEID0_EL0;

// register PMCEID1_EL0
static uint64_t zPMCEID1_EL0;

// register PMCNTENCLR_EL0
static uint64_t zPMCNTENCLR_EL0;

// register PMCNTENSET_EL0
static uint64_t zPMCNTENSET_EL0;

// register PMCR_EL0
static uint64_t zPMCR_EL0;

// register PMEVCNTR_EL0
static zz5vecz8z5bv32z9 zPMEVCNTR_EL0;

// register PMEVTYPER_EL0
static zz5vecz8z5bv32z9 zPMEVTYPER_EL0;

// register PMINTENCLR_EL1
static uint64_t zPMINTENCLR_EL1;

// register PMINTENSET_EL1
static uint64_t zPMINTENSET_EL1;

// register PMOVSCLR_EL0
static uint64_t zPMOVSCLR_EL0;

// register PMOVSSET_EL0
static uint64_t zPMOVSSET_EL0;

// register PMSCR_EL1
static uint64_t zPMSCR_EL1;

// register PMSCR_EL2
static uint64_t zPMSCR_EL2;

// register PMSELR_EL0
static uint64_t zPMSELR_EL0;

// register PMSEVFR_EL1
static uint64_t zPMSEVFR_EL1;

// register PMSFCR_EL1
static uint64_t zPMSFCR_EL1;

// register PMSICR_EL1
static uint64_t zPMSICR_EL1;

// register PMSIDR_EL1
static uint64_t zPMSIDR_EL1;

// register PMSIRR_EL1
static uint64_t zPMSIRR_EL1;

// register PMSLATFR_EL1
static uint64_t zPMSLATFR_EL1;

// register PMXEVCNTR_EL0
static uint64_t zPMXEVCNTR_EL0;

// register PMXEVTYPER_EL0
static uint64_t zPMXEVTYPER_EL0;

// register REVIDR_EL1
static uint64_t zREVIDR_EL1;

// register RMR_EL1
static uint64_t zRMR_EL1;

// register RMR_EL2
static uint64_t zRMR_EL2;

// register RMR_EL3
static uint64_t zRMR_EL3;

// register RSP_EL0
static lbits zRSP_EL0;

// register RTPIDR_EL0
static lbits zRTPIDR_EL0;

// register RVBAR_EL1
static uint64_t zRVBAR_EL1;

// register RVBAR_EL2
static uint64_t zRVBAR_EL2;

// register RVBAR_EL3
static uint64_t zRVBAR_EL3;

// register S3_op1_Cn_Cm_op2
static uint64_t zS3_op1_Cn_Cm_op2;

// register CID_EL0
static lbits zCID_EL0;

// register SCXTNUM_EL1
static uint64_t zSCXTNUM_EL1;

// register SCXTNUM_EL2
static uint64_t zSCXTNUM_EL2;

// register SCXTNUM_EL3
static uint64_t zSCXTNUM_EL3;

// register SDER32_EL3
static uint64_t zSDER32_EL3;

// register SPSR_abt
static uint64_t zSPSR_abt;

// register SPSR_fiq
static uint64_t zSPSR_fiq;

// register SPSR_irq
static uint64_t zSPSR_irq;

// register SPSR_und
static uint64_t zSPSR_und;

// register SP_EL0
static lbits zSP_EL0;

// register SP_EL1
static lbits zSP_EL1;

// register SP_EL2
static lbits zSP_EL2;

// register TPIDRRO_EL0
static lbits zTPIDRRO_EL0;

// register TPIDR_EL0
static lbits zTPIDR_EL0;

// register TPIDR_EL1
static lbits zTPIDR_EL1;

// register TPIDR_EL2
static lbits zTPIDR_EL2;

// register TPIDR_EL3
static lbits zTPIDR_EL3;

// register TTBR0_EL1
static uint64_t zTTBR0_EL1;

// register TTBR0_EL2
static uint64_t zTTBR0_EL2;

// register TTBR0_EL3
static uint64_t zTTBR0_EL3;

// register TTBR1_EL1
static uint64_t zTTBR1_EL1;

// register TTBR1_EL2
static uint64_t zTTBR1_EL2;

// register VSESR_EL2
static uint64_t zVSESR_EL2;

// register VTCR_EL2
static uint64_t zVTCR_EL2;

// register VTTBR_EL2
static uint64_t zVTTBR_EL2;

// register DDC_EL0
static lbits zDDC_EL0;

// register DDC_EL1
static lbits zDDC_EL1;

// register DDC_EL2
static lbits zDDC_EL2;

// register DDC_EL3
static lbits zDDC_EL3;

// register RDDC_EL0
static lbits zRDDC_EL0;

// register DBGDTRTX_EL0
static uint64_t zDBGDTRTX_EL0;

// register ICC_ASGI1R_EL1
static uint64_t zICC_ASGI1R_EL1;

// register ICC_DIR_EL1
static uint64_t zICC_DIR_EL1;

// register ICV_DIR_EL1
static uint64_t zICV_DIR_EL1;

// register ICC_EOIR0_EL1
static uint64_t zICC_EOIR0_EL1;

// register ICV_EOIR0_EL1
static uint64_t zICV_EOIR0_EL1;

// register ICC_EOIR1_EL1
static uint64_t zICC_EOIR1_EL1;

// register ICV_EOIR1_EL1
static uint64_t zICV_EOIR1_EL1;

// register ICC_SGI0R_EL1
static uint64_t zICC_SGI0R_EL1;

// register ICC_SGI1R_EL1
static uint64_t zICC_SGI1R_EL1;

// register OSLAR_EL1
static uint64_t zOSLAR_EL1;

// register PMSWINC_EL0
static uint64_t zPMSWINC_EL0;

static uint64_t zMemAttr_NC;
static void create_letbind_35(void) {


  uint64_t zgsz399;
  zgsz399 = UINT64_C(0b00);
  zMemAttr_NC = zgsz399;

let_end_187: ;
}
static void kill_letbind_35(void) {
}

static uint64_t zMemAttr_WT;
static void create_letbind_36(void) {


  uint64_t zgsz3100;
  zgsz3100 = UINT64_C(0b10);
  zMemAttr_WT = zgsz3100;

let_end_188: ;
}
static void kill_letbind_36(void) {
}

static uint64_t zMemAttr_WB;
static void create_letbind_37(void) {


  uint64_t zgsz3101;
  zgsz3101 = UINT64_C(0b11);
  zMemAttr_WB = zgsz3101;

let_end_189: ;
}
static void kill_letbind_37(void) {
}

static uint64_t zMemHint_No;
static void create_letbind_38(void) {


  uint64_t zgsz3102;
  zgsz3102 = UINT64_C(0b00);
  zMemHint_No = zgsz3102;

let_end_190: ;
}
static void kill_letbind_38(void) {
}

static uint64_t zMemHint_RA;
static void create_letbind_39(void) {


  uint64_t zgsz3103;
  zgsz3103 = UINT64_C(0b10);
  zMemHint_RA = zgsz3103;

let_end_191: ;
}
static void kill_letbind_39(void) {
}

static uint64_t zMemHint_RWA;
static void create_letbind_40(void) {


  uint64_t zgsz3104;
  zgsz3104 = UINT64_C(0b11);
  zMemHint_RWA = zgsz3104;

let_end_192: ;
}
static void kill_letbind_40(void) {
}

// register EventRegister
static uint64_t zEventRegister;

struct zz5vecz8z5bvz9 {
  size_t len;
  lbits *data;
};
typedef struct zz5vecz8z5bvz9 zz5vecz8z5bvz9;

static void CREATE(zz5vecz8z5bvz9)(zz5vecz8z5bvz9 *rop) {
  rop->len = 0;
  rop->data = NULL;
}

static void KILL(zz5vecz8z5bvz9)(zz5vecz8z5bvz9 *rop) {
  for (int i = 0; i < (rop->len); i++) {
    KILL(lbits)((rop->data) + i);
  }
  if (rop->data != NULL) sail_free(rop->data);
}

static void vector_init_zz5vecz8z5bvz9(zz5vecz8z5bvz9 *vec, sail_int n, lbits elem) {
  KILL(zz5vecz8z5bvz9)(vec);
  size_t m = (size_t)sail_int_get_ui(n);
  vec->len = m;
  vec->data = sail_new_array(lbits, m);
  for (size_t i = 0; i < m; i++) {
    CREATE(lbits)((vec->data) + i);
    COPY(lbits)((vec->data) + i, elem);
  }
}

static void RECREATE(zz5vecz8z5bvz9)(zz5vecz8z5bvz9 *rop) {
  KILL(zz5vecz8z5bvz9)(rop);
  CREATE(zz5vecz8z5bvz9)(rop);
}

static void undefined_vector_zz5vecz8z5bvz9(zz5vecz8z5bvz9 *rop, sail_int len, lbits elem) {
  rop->len = sail_int_get_ui(len);
  rop->data = sail_new_array(lbits, rop->len);
  for (int i = 0; i < (rop->len); i++) {
    CREATE(lbits)((rop->data) + i);
    COPY(lbits)((rop->data) + i, elem);
  }
}

static void vector_access_zz5vecz8z5bvz9(lbits *rop, zz5vecz8z5bvz9 op, sail_int n) {
  int m = sail_int_get_ui(n);
  COPY(lbits)(rop, op.data[m]);
}

static void COPY(zz5vecz8z5bvz9)(zz5vecz8z5bvz9 *rop, zz5vecz8z5bvz9 op) {
  KILL(zz5vecz8z5bvz9)(rop);
  rop->len = op.len;
  rop->data = sail_new_array(lbits, rop->len);
  for (int i = 0; i < op.len; i++) {
    CREATE(lbits)((rop->data) + i);
    COPY(lbits)((rop->data) + i, op.data[i]);
  }
}

static void vector_update_zz5vecz8z5bvz9(zz5vecz8z5bvz9 *rop, zz5vecz8z5bvz9 op, sail_int n, lbits elem) {
  int m = sail_int_get_ui(n);
  if (rop->data == op.data) {
    COPY(lbits)((rop->data) + m, elem);
  } else {
    COPY(zz5vecz8z5bvz9)(rop, op);
    COPY(lbits)((rop->data) + m, elem);
  }
}

static bool EQUAL(zz5vecz8z5bvz9)(const zz5vecz8z5bvz9 op1, const zz5vecz8z5bvz9 op2) {
  if (op1.len != op2.len) return false;
  bool result = true;
  for (int i = 0; i < op1.len; i++) {
    result &= EQUAL(lbits)(op1.data[i], op2.data[i]);
  }
  return result;
}

static void length_zz5vecz8z5bvz9(sail_int *rop, zz5vecz8z5bvz9 op) {
  mpz_set_ui(*rop, (unsigned long int)(op.len));
}

static void internal_vector_update_zz5vecz8z5bvz9(zz5vecz8z5bvz9 *rop, zz5vecz8z5bvz9 op, const int64_t n, lbits elem) {
  COPY(lbits)((rop->data) + n, elem);
}

static void internal_vector_init_zz5vecz8z5bvz9(zz5vecz8z5bvz9 *rop, const int64_t len) {
  rop->len = len;
  rop->data = sail_new_array(lbits, len);
  for (int i = 0; i < len; i++) {
    CREATE(lbits)((rop->data) + i);
  }
}

// register _V
static zz5vecz8z5bvz9 z_V;

static uint64_t zDefaultPARTID;
static void create_letbind_41(void) {


  uint64_t zgsz3105;
  zgsz3105 = UINT64_C(0x0000);
  zDefaultPARTID = zgsz3105;

let_end_193: ;
}
static void kill_letbind_41(void) {
}

static uint64_t zDefaultPMG;
static void create_letbind_42(void) {


  uint64_t zgsz3106;
  zgsz3106 = UINT64_C(0x00);
  zDefaultPMG = zgsz3106;

let_end_194: ;
}
static void kill_letbind_42(void) {
}

static int64_t zCAPABILITY_DBYTES;
static void create_letbind_43(void) {


  int64_t zgsz3107;
  zgsz3107 = INT64_C(16);
  zCAPABILITY_DBYTES = zgsz3107;

let_end_195: ;
}
static void kill_letbind_43(void) {
}

static int64_t zLOG2_CAPABILITY_DBYTES;
static void create_letbind_44(void) {


  int64_t zgsz3108;
  zgsz3108 = INT64_C(4);
  zLOG2_CAPABILITY_DBYTES = zgsz3108;

let_end_196: ;
}
static void kill_letbind_44(void) {
}

static uint64_t zDebugHalt_Breakpoint;
static void create_letbind_45(void) {


  uint64_t zgsz3109;
  zgsz3109 = UINT64_C(0b000111);
  zDebugHalt_Breakpoint = zgsz3109;

let_end_197: ;
}
static void kill_letbind_45(void) {
}

static uint64_t zDebugHalt_Watchpoint;
static void create_letbind_46(void) {


  uint64_t zgsz3110;
  zgsz3110 = UINT64_C(0b101011);
  zDebugHalt_Watchpoint = zgsz3110;

let_end_198: ;
}
static void kill_letbind_46(void) {
}

static uint64_t zDebugHalt_HaltInstruction;
static void create_letbind_47(void) {


  uint64_t zgsz3111;
  zgsz3111 = UINT64_C(0b101111);
  zDebugHalt_HaltInstruction = zgsz3111;

let_end_199: ;
}
static void kill_letbind_47(void) {
}

static uint64_t zCAP_NO_SEALING;
static void create_letbind_48(void) {


  uint64_t zgsz3112;
  zgsz3112 = UINT64_C(0xFFFFFFFFFFFFFFFF);
  zCAP_NO_SEALING = zgsz3112;

let_end_200: ;
}
static void kill_letbind_48(void) {
}

static uint64_t zCAP_SEAL_TYPE_RB;
static void create_letbind_49(void) {


  uint64_t zgsz3113;
  zgsz3113 = UINT64_C(0x0000000000000001);
  zCAP_SEAL_TYPE_RB = zgsz3113;

let_end_201: ;
}
static void kill_letbind_49(void) {
}

static uint64_t zCAP_SEAL_TYPE_LPB;
static void create_letbind_50(void) {


  uint64_t zgsz3114;
  zgsz3114 = UINT64_C(0x0000000000000002);
  zCAP_SEAL_TYPE_LPB = zgsz3114;

let_end_202: ;
}
static void kill_letbind_50(void) {
}

static uint64_t zCAP_SEAL_TYPE_LB;
static void create_letbind_51(void) {


  uint64_t zgsz3115;
  zgsz3115 = UINT64_C(0x0000000000000003);
  zCAP_SEAL_TYPE_LB = zgsz3115;

let_end_203: ;
}
static void kill_letbind_51(void) {
}

static int64_t zCAP_MAX_FIXED_SEAL_TYPE;
static void create_letbind_52(void) {


  int64_t zgsz3116;
  zgsz3116 = INT64_C(3);
  zCAP_MAX_FIXED_SEAL_TYPE = zgsz3116;

let_end_204: ;
}
static void kill_letbind_52(void) {
}

static uint64_t zCAP_PERM_LOAD;
static void create_letbind_53(void) {


  uint64_t zgsz3117;
  zgsz3117 = UINT64_C(0x0000000000020000);
  zCAP_PERM_LOAD = zgsz3117;

let_end_205: ;
}
static void kill_letbind_53(void) {
}

static uint64_t zCAP_PERM_STORE;
static void create_letbind_54(void) {


  uint64_t zgsz3118;
  zgsz3118 = UINT64_C(0x0000000000010000);
  zCAP_PERM_STORE = zgsz3118;

let_end_206: ;
}
static void kill_letbind_54(void) {
}

static uint64_t zCAP_PERM_LOAD_CAP;
static void create_letbind_55(void) {


  uint64_t zgsz3119;
  zgsz3119 = UINT64_C(0x0000000000004000);
  zCAP_PERM_LOAD_CAP = zgsz3119;

let_end_207: ;
}
static void kill_letbind_55(void) {
}

static uint64_t zCAP_PERM_STORE_CAP;
static void create_letbind_56(void) {


  uint64_t zgsz3120;
  zgsz3120 = UINT64_C(0x0000000000002000);
  zCAP_PERM_STORE_CAP = zgsz3120;

let_end_208: ;
}
static void kill_letbind_56(void) {
}

static uint64_t zCAP_PERM_STORE_LOCAL;
static void create_letbind_57(void) {


  uint64_t zgsz3121;
  zgsz3121 = UINT64_C(0x0000000000001000);
  zCAP_PERM_STORE_LOCAL = zgsz3121;

let_end_209: ;
}
static void kill_letbind_57(void) {
}

static uint64_t zCAP_PERM_SEAL;
static void create_letbind_58(void) {


  uint64_t zgsz3122;
  zgsz3122 = UINT64_C(0x0000000000000800);
  zCAP_PERM_SEAL = zgsz3122;

let_end_210: ;
}
static void kill_letbind_58(void) {
}

static uint64_t zCAP_PERM_UNSEAL;
static void create_letbind_59(void) {


  uint64_t zgsz3123;
  zgsz3123 = UINT64_C(0x0000000000000400);
  zCAP_PERM_UNSEAL = zgsz3123;

let_end_211: ;
}
static void kill_letbind_59(void) {
}

static uint64_t zCAP_PERM_BRANCH_SEALED_PAIR;
static void create_letbind_60(void) {


  uint64_t zgsz3124;
  zgsz3124 = UINT64_C(0x0000000000000100);
  zCAP_PERM_BRANCH_SEALED_PAIR = zgsz3124;

let_end_212: ;
}
static void kill_letbind_60(void) {
}

static uint64_t zCAP_PERM_MUTABLE_LOAD;
static void create_letbind_61(void) {


  uint64_t zgsz3125;
  zgsz3125 = UINT64_C(0x0000000000000040);
  zCAP_PERM_MUTABLE_LOAD = zgsz3125;

let_end_213: ;
}
static void kill_letbind_61(void) {
}

static uint64_t zCAP_PERM_GLOBAL;
static void create_letbind_62(void) {


  uint64_t zgsz3126;
  zgsz3126 = UINT64_C(0x0000000000000001);
  zCAP_PERM_GLOBAL = zgsz3126;

let_end_214: ;
}
static void kill_letbind_62(void) {
}

static uint64_t zCAP_PERM_NONE;
static void create_letbind_63(void) {


  uint64_t zgsz3127;
  zgsz3127 = UINT64_C(0x0000000000000000);
  zCAP_PERM_NONE = zgsz3127;

let_end_215: ;
}
static void kill_letbind_63(void) {
}

static int64_t zCAP_OTYPE_NUM_BITS;
static void create_letbind_64(void) {


  int64_t zgsz3128;
  int64_t zgaz3121;
  {
    sail_int zgsz3857;
    CREATE(sail_int)(&zgsz3857);
    CONVERT_OF(sail_int, mach_int)(&zgsz3857, zCAP_OTYPE_HI_BIT);
    sail_int zgsz3858;
    CREATE(sail_int)(&zgsz3858);
    CONVERT_OF(sail_int, mach_int)(&zgsz3858, zCAP_OTYPE_LO_BIT);
    sail_int zgsz3859;
    CREATE(sail_int)(&zgsz3859);
    sub_int(&zgsz3859, zgsz3857, zgsz3858);
    zgaz3121 = CONVERT_OF(mach_int, sail_int)(zgsz3859);
    KILL(sail_int)(&zgsz3859);
    KILL(sail_int)(&zgsz3858);
    KILL(sail_int)(&zgsz3857);
  }
  zgsz3128 = (zgaz3121 + INT64_C(1));

  zCAP_OTYPE_NUM_BITS = zgsz3128;

let_end_216: ;
}
static void kill_letbind_64(void) {
}

static int64_t zCAP_LENGTH_NUM_BITS;
static void create_letbind_65(void) {


  int64_t zgsz3129;
  zgsz3129 = (zCAP_VALUE_NUM_BITS + INT64_C(1));
  zCAP_LENGTH_NUM_BITS = zgsz3129;

let_end_217: ;
}
static void kill_letbind_65(void) {
}

static sail_int zCAP_MAX_OBJECT_TYPE;
static void create_letbind_66(void) {    CREATE(sail_int)(&zCAP_MAX_OBJECT_TYPE);


  sail_int zgsz3130;
  CREATE(sail_int)(&zgsz3130);
  sail_int zgaz3122;
  CREATE(sail_int)(&zgaz3122);
  {
    sail_int zgsz3861;
    CREATE(sail_int)(&zgsz3861);
    CONVERT_OF(sail_int, mach_int)(&zgsz3861, INT64_C(1));
    sail_int zgsz3862;
    CREATE(sail_int)(&zgsz3862);
    CONVERT_OF(sail_int, mach_int)(&zgsz3862, zCAP_OTYPE_NUM_BITS);
    shl_int(&zgaz3122, zgsz3861, zgsz3862);
    KILL(sail_int)(&zgsz3862);
    KILL(sail_int)(&zgsz3861);
  }
  {
    sail_int zgsz3860;
    CREATE(sail_int)(&zgsz3860);
    CONVERT_OF(sail_int, mach_int)(&zgsz3860, INT64_C(1));
    sub_int(&zgsz3130, zgaz3122, zgsz3860);
    KILL(sail_int)(&zgsz3860);
  }
  KILL(sail_int)(&zgaz3122);
  COPY(sail_int)(&zCAP_MAX_OBJECT_TYPE, zgsz3130);
  KILL(sail_int)(&zgsz3130);
let_end_218: ;
}
static void kill_letbind_66(void) {    KILL(sail_int)(&zCAP_MAX_OBJECT_TYPE);
}

// register __saved_exception_level
static uint64_t z__saved_exception_level;

// register __CNTControlBase
static uint64_t z__CNTControlBase;

// register __highest_el_aarch32
static bool z__highest_el_aarch32;

static uint64_t z__exclusive_granule_sizze;
static void create_letbind_67(void) {


  uint64_t zgsz3131;
  zgsz3131 = UINT64_C(0x4);
  z__exclusive_granule_sizze = zgsz3131;

let_end_219: ;
}
static void kill_letbind_67(void) {
}

static uint64_t zCFG_ID_AA64PFR0_EL1_MPAM;
static void create_letbind_68(void) {


  uint64_t zgsz3132;
  zgsz3132 = UINT64_C(0x1);
  zCFG_ID_AA64PFR0_EL1_MPAM = zgsz3132;

let_end_220: ;
}
static void kill_letbind_68(void) {
}

static bool sailgen_CapUnsignedGreaterThanOrEqual(lbits, lbits);

sail_int zghz3202;
sail_int zghz3203;

static void startup_sailgen_CapUnsignedGreaterThanOrEqual(void)
{
  CREATE(sail_int)(&zghz3202);
  CREATE(sail_int)(&zghz3203);
}

static bool sailgen_CapUnsignedGreaterThanOrEqual(lbits za, lbits zb)
{
  __label__ cleanup_223, end_cleanup_224, end_function_222, end_block_exception_225;

  bool zcbz330;
  {
    RECREATE(sail_int)(&zghz3202);
    sail_unsigned(&zghz3202, za);
    RECREATE(sail_int)(&zghz3203);
    sail_unsigned(&zghz3203, zb);
    zcbz330 = gteq(zghz3202, zghz3203);
  }
  goto cleanup_223;
  /* unreachable after return */
  goto end_cleanup_224;
cleanup_223: ;
  goto end_function_222;
end_cleanup_224: ;
end_function_222: ;
  return zcbz330;
end_block_exception_225: ;

  return false;
}



static void finish_sailgen_CapUnsignedGreaterThanOrEqual(void)
{
  KILL(sail_int)(&zghz3203);
  KILL(sail_int)(&zghz3202);
}

static bool sailgen_CapIsRepresentableFast(lbits, uint64_t);

lbits zghz3204;
lbits zghz3205;
sail_int zghz3206;
lbits zghz3207;
lbits zghz3208;
lbits zghz3209;
lbits zghz3210;
sail_int zghz3211;
lbits zghz3212;
sail_int zghz3213;
lbits zghz3214;
sail_int zghz3215;
lbits zghz3216;
sail_int zghz3217;
sail_int zghz3218;
sail_int zghz3219;
sail_int zghz3220;
sail_int zghz3221;
sail_int zghz3222;
sail_int zghz3223;
sail_int zghz3224;
sail_int zghz3225;
sail_int zghz3226;
sail_int zghz3227;
sail_int zghz3228;
lbits zghz3229;
sail_int zghz3230;
lbits zghz3231;
sail_int zghz3232;
sail_int zghz3233;
sail_int zghz3234;
lbits zghz3235;
sail_int zghz3236;
lbits zghz3237;
lbits zghz3238;
sail_int zghz3239;
lbits zghz3240;
sail_int zghz3241;
sail_int zghz3242;
sail_int zghz3243;

static void startup_sailgen_CapIsRepresentableFast(void)
{
  CREATE(lbits)(&zghz3204);
  CREATE(lbits)(&zghz3205);
  CREATE(sail_int)(&zghz3206);
  CREATE(lbits)(&zghz3207);
  CREATE(lbits)(&zghz3208);
  CREATE(lbits)(&zghz3209);
  CREATE(lbits)(&zghz3210);
  CREATE(sail_int)(&zghz3211);
  CREATE(lbits)(&zghz3212);
  CREATE(sail_int)(&zghz3213);
  CREATE(lbits)(&zghz3214);
  CREATE(sail_int)(&zghz3215);
  CREATE(lbits)(&zghz3216);
  CREATE(sail_int)(&zghz3217);
  CREATE(sail_int)(&zghz3218);
  CREATE(sail_int)(&zghz3219);
  CREATE(sail_int)(&zghz3220);
  CREATE(sail_int)(&zghz3221);
  CREATE(sail_int)(&zghz3222);
  CREATE(sail_int)(&zghz3223);
  CREATE(sail_int)(&zghz3224);
  CREATE(sail_int)(&zghz3225);
  CREATE(sail_int)(&zghz3226);
  CREATE(sail_int)(&zghz3227);
  CREATE(sail_int)(&zghz3228);
  CREATE(lbits)(&zghz3229);
  CREATE(sail_int)(&zghz3230);
  CREATE(lbits)(&zghz3231);
  CREATE(sail_int)(&zghz3232);
  CREATE(sail_int)(&zghz3233);
  CREATE(sail_int)(&zghz3234);
  CREATE(lbits)(&zghz3235);
  CREATE(sail_int)(&zghz3236);
  CREATE(lbits)(&zghz3237);
  CREATE(lbits)(&zghz3238);
  CREATE(sail_int)(&zghz3239);
  CREATE(lbits)(&zghz3240);
  CREATE(sail_int)(&zghz3241);
  CREATE(sail_int)(&zghz3242);
  CREATE(sail_int)(&zghz3243);
}

static bool sailgen_CapIsRepresentableFast(lbits zc, uint64_t zincrement_name__arg)
{
  __label__ case_228, finish_match_227, end_function_229, end_block_exception_238;

  bool zcbz331;
  uint64_t zincrement_name;
  zincrement_name = zincrement_name__arg;
  uint64_t zB3;
  zB3 = UINT64_C(0b000);
  uint64_t zR;
  zR = UINT64_C(0x0000);
  uint64_t zR3;
  zR3 = UINT64_C(0b000);
  uint64_t za_mid;
  za_mid = UINT64_C(0x0000);
  uint64_t zdiff;
  zdiff = UINT64_C(0x0000);
  uint64_t zdiff1;
  zdiff1 = UINT64_C(0x0000);
  uint64_t zi_mid;
  zi_mid = UINT64_C(0x0000);
  uint64_t zi_top;
  zi_top = UINT64_C(0x0000000000000000);
  int64_t zgaz3126;
  zgaz3126 = sailgen_CapGetExponent(zc);
  {
    int64_t zexp;
    zexp = zgaz3126;
    bool zgaz3128;
    {
      int64_t zgaz3127;
      {
        RECREATE(sail_int)(&zghz3241);
        CONVERT_OF(sail_int, mach_int)(&zghz3241, zCAP_MAX_EXPONENT);
        RECREATE(sail_int)(&zghz3242);
        CONVERT_OF(sail_int, mach_int)(&zghz3242, INT64_C(2));
        RECREATE(sail_int)(&zghz3243);
        sub_int(&zghz3243, zghz3241, zghz3242);
        zgaz3127 = CONVERT_OF(mach_int, sail_int)(zghz3243);
      }
      zgaz3128 = (zexp >= zgaz3127);
    }
    if (zgaz3128) {
      __label__ cleanup_236, end_cleanup_237;

      zcbz331 = true;
      goto cleanup_236;
      /* unreachable after return */
      goto end_cleanup_237;
    cleanup_236: ;
      goto end_function_229;
    end_cleanup_237: ;
    } else {
      uint64_t za;
      za = sailgen_CapGetValue(zc);
      uint64_t zashadowz33;
      zashadowz33 = sailgen_CapBoundsAddress(za);
      uint64_t zincrement_nameshadowz34;
      zincrement_nameshadowz34 = sailgen_CapBoundsAddress(zincrement_name);
      uint64_t zi_topshadowz35;
      {
        int64_t zgaz3144;
        zgaz3144 = (zexp + zCAP_MW);
        {
          RECREATE(lbits)(&zghz3238);
          CONVERT_OF(lbits, fbits)(&zghz3238, zincrement_nameshadowz34, UINT64_C(64) , true);
          RECREATE(sail_int)(&zghz3239);
          CONVERT_OF(sail_int, mach_int)(&zghz3239, zgaz3144);
          RECREATE(lbits)(&zghz3240);
          arith_shiftr(&zghz3240, zghz3238, zghz3239);
          zi_topshadowz35 = CONVERT_OF(fbits, lbits)(zghz3240, true);
        }
      }
      uint64_t zi_midshadowz36;
      {
        uint64_t zgaz3142;
        {
          RECREATE(lbits)(&zghz3235);
          CONVERT_OF(lbits, fbits)(&zghz3235, zincrement_nameshadowz34, UINT64_C(64) , true);
          RECREATE(sail_int)(&zghz3236);
          CONVERT_OF(sail_int, mach_int)(&zghz3236, zexp);
          RECREATE(lbits)(&zghz3237);
          shiftr(&zghz3237, zghz3235, zghz3236);
          zgaz3142 = CONVERT_OF(fbits, lbits)(zghz3237, true);
        }
        int64_t zgaz3143;
        {
          RECREATE(sail_int)(&zghz3232);
          CONVERT_OF(sail_int, mach_int)(&zghz3232, zCAP_MW);
          RECREATE(sail_int)(&zghz3233);
          CONVERT_OF(sail_int, mach_int)(&zghz3233, INT64_C(1));
          RECREATE(sail_int)(&zghz3234);
          sub_int(&zghz3234, zghz3232, zghz3233);
          zgaz3143 = CONVERT_OF(mach_int, sail_int)(zghz3234);
        }
        zi_midshadowz36 = (safe_rshift(UINT64_MAX, 64 - 16) & (zgaz3142 >> INT64_C(0)));
      }
      uint64_t za_midshadowz37;
      {
        uint64_t zgaz3140;
        {
          RECREATE(lbits)(&zghz3229);
          CONVERT_OF(lbits, fbits)(&zghz3229, zashadowz33, UINT64_C(64) , true);
          RECREATE(sail_int)(&zghz3230);
          CONVERT_OF(sail_int, mach_int)(&zghz3230, zexp);
          RECREATE(lbits)(&zghz3231);
          shiftr(&zghz3231, zghz3229, zghz3230);
          zgaz3140 = CONVERT_OF(fbits, lbits)(zghz3231, true);
        }
        int64_t zgaz3141;
        {
          RECREATE(sail_int)(&zghz3226);
          CONVERT_OF(sail_int, mach_int)(&zghz3226, zCAP_MW);
          RECREATE(sail_int)(&zghz3227);
          CONVERT_OF(sail_int, mach_int)(&zghz3227, INT64_C(1));
          RECREATE(sail_int)(&zghz3228);
          sub_int(&zghz3228, zghz3226, zghz3227);
          zgaz3141 = CONVERT_OF(mach_int, sail_int)(zghz3228);
        }
        za_midshadowz37 = (safe_rshift(UINT64_MAX, 64 - 16) & (zgaz3140 >> INT64_C(0)));
      }
      uint64_t zB3shadowz38;
      {
        uint64_t zgaz3137;
        zgaz3137 = sailgen_CapGetBottom(zc);
        int64_t zgaz3138;
        {
          RECREATE(sail_int)(&zghz3223);
          CONVERT_OF(sail_int, mach_int)(&zghz3223, zCAP_MW);
          RECREATE(sail_int)(&zghz3224);
          CONVERT_OF(sail_int, mach_int)(&zghz3224, INT64_C(1));
          RECREATE(sail_int)(&zghz3225);
          sub_int(&zghz3225, zghz3223, zghz3224);
          zgaz3138 = CONVERT_OF(mach_int, sail_int)(zghz3225);
        }
        int64_t zgaz3139;
        {
          RECREATE(sail_int)(&zghz3220);
          CONVERT_OF(sail_int, mach_int)(&zghz3220, zCAP_MW);
          RECREATE(sail_int)(&zghz3221);
          CONVERT_OF(sail_int, mach_int)(&zghz3221, INT64_C(3));
          RECREATE(sail_int)(&zghz3222);
          sub_int(&zghz3222, zghz3220, zghz3221);
          zgaz3139 = CONVERT_OF(mach_int, sail_int)(zghz3222);
        }
        zB3shadowz38 = (safe_rshift(UINT64_MAX, 64 - 3) & (zgaz3137 >> zgaz3139));
      }
      uint64_t zR3shadowz39;
      zR3shadowz39 = ((zB3shadowz38 - UINT64_C(0b001)) & UINT64_C(0b111));
      uint64_t zRshadowz310;
      {
        uint64_t zgaz3136;
        {
          int64_t zgaz3135;
          {
            RECREATE(sail_int)(&zghz3217);
            CONVERT_OF(sail_int, mach_int)(&zghz3217, zCAP_MW);
            RECREATE(sail_int)(&zghz3218);
            CONVERT_OF(sail_int, mach_int)(&zghz3218, INT64_C(3));
            RECREATE(sail_int)(&zghz3219);
            sub_int(&zghz3219, zghz3217, zghz3218);
            zgaz3135 = CONVERT_OF(mach_int, sail_int)(zghz3219);
          }
          {
            RECREATE(sail_int)(&zghz3215);
            CONVERT_OF(sail_int, mach_int)(&zghz3215, zgaz3135);
            RECREATE(lbits)(&zghz3216);
            sailgen_Zeros(&zghz3216, zghz3215);
            zgaz3136 = CONVERT_OF(fbits, lbits)(zghz3216, true);
          }
        }
        zRshadowz310 = (zR3shadowz39 << 13) | zgaz3136;
      }
      uint64_t zdiffshadowz311;
      zdiffshadowz311 = ((zRshadowz310 - za_midshadowz37) & UINT64_C(0xFFFF));
      uint64_t zdiff1shadowz312;
      {
        RECREATE(lbits)(&zghz3212);
        CONVERT_OF(lbits, fbits)(&zghz3212, zdiffshadowz311, UINT64_C(16) , true);
        RECREATE(sail_int)(&zghz3213);
        CONVERT_OF(sail_int, mach_int)(&zghz3213, INT64_C(1));
        RECREATE(lbits)(&zghz3214);
        sub_bits_int(&zghz3214, zghz3212, zghz3213);
        zdiff1shadowz312 = CONVERT_OF(fbits, lbits)(zghz3214, true);
      }
      bool zgaz3129;
      {
        RECREATE(lbits)(&zghz3210);
        CONVERT_OF(lbits, fbits)(&zghz3210, zi_topshadowz35, UINT64_C(64) , true);
        RECREATE(sail_int)(&zghz3211);
        CONVERT_OF(sail_int, mach_int)(&zghz3211, INT64_C(0));
        zgaz3129 = sailgen_eq_bits_int(zghz3210, zghz3211);
      }
      if (zgaz3129) {
        __label__ cleanup_234, end_cleanup_235;

        bool zgaz3130;
        {
          RECREATE(lbits)(&zghz3208);
          CONVERT_OF(lbits, fbits)(&zghz3208, zi_midshadowz36, UINT64_C(16) , true);
          RECREATE(lbits)(&zghz3209);
          CONVERT_OF(lbits, fbits)(&zghz3209, zdiff1shadowz312, UINT64_C(16) , true);
          zgaz3130 = sailgen_CapUnsignedLessThan(zghz3208, zghz3209);
        }
        zcbz331 = zgaz3130;
        goto cleanup_234;
        /* unreachable after return */
        goto end_cleanup_235;
      cleanup_234: ;
        goto end_function_229;
      end_cleanup_235: ;
      } else {
        bool zgaz3132;
        {
          uint64_t zgaz3131;
          {
            RECREATE(sail_int)(&zghz3206);
            CONVERT_OF(sail_int, mach_int)(&zghz3206, zCAP_VALUE_NUM_BITS);
            RECREATE(lbits)(&zghz3207);
            sailgen_Ones(&zghz3207, zghz3206);
            zgaz3131 = CONVERT_OF(fbits, lbits)(zghz3207, true);
          }
          zgaz3132 = (zi_topshadowz35 == zgaz3131);
        }
        if (zgaz3132) {
          __label__ cleanup_232, end_cleanup_233;

          bool zgaz3134;
          {
            bool zgaz3133;
            {
              RECREATE(lbits)(&zghz3204);
              CONVERT_OF(lbits, fbits)(&zghz3204, zi_midshadowz36, UINT64_C(16) , true);
              RECREATE(lbits)(&zghz3205);
              CONVERT_OF(lbits, fbits)(&zghz3205, zdiffshadowz311, UINT64_C(16) , true);
              zgaz3133 = sailgen_CapUnsignedGreaterThanOrEqual(zghz3204, zghz3205);
            }
            bool zgsz3135;
            if (zgaz3133) {  zgsz3135 = (zRshadowz310 != za_midshadowz37);  } else {  zgsz3135 = false;  }
            zgaz3134 = zgsz3135;
          }
          zcbz331 = zgaz3134;
          goto cleanup_232;
          /* unreachable after return */
          goto end_cleanup_233;
        cleanup_232: ;
          goto end_function_229;
        end_cleanup_233: ;
        } else {
          __label__ cleanup_230, end_cleanup_231;

          zcbz331 = false;
          goto cleanup_230;
          /* unreachable after return */
          goto end_cleanup_231;
        cleanup_230: ;
          goto end_function_229;
        end_cleanup_231: ;
        }
      }
    }
    goto finish_match_227;
  }
case_228: ;
  sail_match_failure("CapIsRepresentableFast");
finish_match_227: ;










end_function_229: ;
  return zcbz331;
end_block_exception_238: ;

  return false;
}



static void finish_sailgen_CapIsRepresentableFast(void)
{
  KILL(sail_int)(&zghz3243);
  KILL(sail_int)(&zghz3242);
  KILL(sail_int)(&zghz3241);
  KILL(lbits)(&zghz3240);
  KILL(sail_int)(&zghz3239);
  KILL(lbits)(&zghz3238);
  KILL(lbits)(&zghz3237);
  KILL(sail_int)(&zghz3236);
  KILL(lbits)(&zghz3235);
  KILL(sail_int)(&zghz3234);
  KILL(sail_int)(&zghz3233);
  KILL(sail_int)(&zghz3232);
  KILL(lbits)(&zghz3231);
  KILL(sail_int)(&zghz3230);
  KILL(lbits)(&zghz3229);
  KILL(sail_int)(&zghz3228);
  KILL(sail_int)(&zghz3227);
  KILL(sail_int)(&zghz3226);
  KILL(sail_int)(&zghz3225);
  KILL(sail_int)(&zghz3224);
  KILL(sail_int)(&zghz3223);
  KILL(sail_int)(&zghz3222);
  KILL(sail_int)(&zghz3221);
  KILL(sail_int)(&zghz3220);
  KILL(sail_int)(&zghz3219);
  KILL(sail_int)(&zghz3218);
  KILL(sail_int)(&zghz3217);
  KILL(lbits)(&zghz3216);
  KILL(sail_int)(&zghz3215);
  KILL(lbits)(&zghz3214);
  KILL(sail_int)(&zghz3213);
  KILL(lbits)(&zghz3212);
  KILL(sail_int)(&zghz3211);
  KILL(lbits)(&zghz3210);
  KILL(lbits)(&zghz3209);
  KILL(lbits)(&zghz3208);
  KILL(lbits)(&zghz3207);
  KILL(sail_int)(&zghz3206);
  KILL(lbits)(&zghz3205);
  KILL(lbits)(&zghz3204);
}

// register SP_EL3
static lbits zSP_EL3;

static int64_t zCAPABILITY_DBITS;
static void create_letbind_69(void) {


  int64_t zgsz3136;
  {
    sail_int zgsz3903;
    CREATE(sail_int)(&zgsz3903);
    CONVERT_OF(sail_int, mach_int)(&zgsz3903, zCAPABILITY_DBYTES);
    sail_int zgsz3904;
    CREATE(sail_int)(&zgsz3904);
    CONVERT_OF(sail_int, mach_int)(&zgsz3904, INT64_C(8));
    sail_int zgsz3905;
    CREATE(sail_int)(&zgsz3905);
    mult_int(&zgsz3905, zgsz3903, zgsz3904);
    zgsz3136 = CONVERT_OF(mach_int, sail_int)(zgsz3905);
    KILL(sail_int)(&zgsz3905);
    KILL(sail_int)(&zgsz3904);
    KILL(sail_int)(&zgsz3903);
  }
  zCAPABILITY_DBITS = zgsz3136;

let_end_239: ;
}
static void kill_letbind_69(void) {
}

static bool sailgen_CapUnsignedLessThanOrEqual(lbits, lbits);

sail_int zghz3244;
sail_int zghz3245;

static void startup_sailgen_CapUnsignedLessThanOrEqual(void)
{
  CREATE(sail_int)(&zghz3244);
  CREATE(sail_int)(&zghz3245);
}

static bool sailgen_CapUnsignedLessThanOrEqual(lbits za, lbits zb)
{
  __label__ cleanup_242, end_cleanup_243, end_function_241, end_block_exception_244;

  bool zcbz332;
  {
    RECREATE(sail_int)(&zghz3244);
    sail_unsigned(&zghz3244, za);
    RECREATE(sail_int)(&zghz3245);
    sail_unsigned(&zghz3245, zb);
    zcbz332 = lteq(zghz3244, zghz3245);
  }
  goto cleanup_242;
  /* unreachable after return */
  goto end_cleanup_243;
cleanup_242: ;
  goto end_function_241;
end_cleanup_243: ;
end_function_241: ;
  return zcbz332;
end_block_exception_244: ;

  return false;
}



static void finish_sailgen_CapUnsignedLessThanOrEqual(void)
{
  KILL(sail_int)(&zghz3245);
  KILL(sail_int)(&zghz3244);
}

static uint64_t sailgen_CapGetTag(lbits);

lbits zghz3246;
sail_int zghz3247;
lbits zghz3248;
sail_int zghz3249;

static void startup_sailgen_CapGetTag(void)
{
  CREATE(lbits)(&zghz3246);
  CREATE(sail_int)(&zghz3247);
  CREATE(lbits)(&zghz3248);
  CREATE(sail_int)(&zghz3249);
}

static uint64_t sailgen_CapGetTag(lbits zc)
{
  __label__ cleanup_247, end_cleanup_248, end_function_246, end_block_exception_249;

  uint64_t zcbz333;
  {
    uint64_t zgaz3149;
    {
      fbits zgaz3148;
      {
        RECREATE(sail_int)(&zghz3249);
        CONVERT_OF(sail_int, mach_int)(&zghz3249, zCAP_TAG_BIT);
        zgaz3148 = bitvector_access(zc, zghz3249);
      }
      zgaz3149 = UINT64_C(0b0);
      zgaz3149 = update_fbits(zgaz3149, INT64_C(0), zgaz3148);
    }
    {
      RECREATE(lbits)(&zghz3246);
      CONVERT_OF(lbits, fbits)(&zghz3246, zgaz3149, UINT64_C(1) , true);
      RECREATE(sail_int)(&zghz3247);
      CONVERT_OF(sail_int, mach_int)(&zghz3247, INT64_C(64));
      RECREATE(lbits)(&zghz3248);
      zero_extend(&zghz3248, zghz3246, zghz3247);
      zcbz333 = CONVERT_OF(fbits, lbits)(zghz3248, true);
    }
  }
  goto cleanup_247;
  /* unreachable after return */
  goto end_cleanup_248;
cleanup_247: ;
  goto end_function_246;
end_cleanup_248: ;
end_function_246: ;
  return zcbz333;
end_block_exception_249: ;

  return UINT64_C(0xdeadc0de);
}



static void finish_sailgen_CapGetTag(void)
{
  KILL(sail_int)(&zghz3249);
  KILL(lbits)(&zghz3248);
  KILL(sail_int)(&zghz3247);
  KILL(lbits)(&zghz3246);
}

static void sailgen_CapSetBounds(lbits *rop, lbits, lbits, bool);

lbits zghz3250;
lbits zghz3251;
sail_int zghz3252;
lbits zghz3253;
lbits zghz3254;
lbits zghz3255;
sail_int zghz3256;
lbits zghz3257;
sail_int zghz3258;
sail_int zghz3259;
lbits zghz3260;
sail_int zghz3261;
lbits zghz3262;
sail_int zghz3263;
sail_int zghz3264;
sail_int zghz3265;
sail_int zghz3266;
sail_int zghz3267;
sail_int zghz3268;
lbits zghz3269;
sail_int zghz3270;
sail_int zghz3271;
sail_int zghz3272;
sail_int zghz3273;
sail_int zghz3274;
lbits zghz3275;
sail_int zghz3276;
sail_int zghz3277;
sail_int zghz3278;
sail_int zghz3279;
sail_int zghz3280;
lbits zghz3281;
sail_int zghz3282;
sail_int zghz3283;
lbits zghz3284;
sail_int zghz3285;
sail_int zghz3286;
sail_int zghz3287;
lbits zghz3288;
sail_int zghz3289;
sail_int zghz3290;
lbits zghz3291;
sail_int zghz3292;
sail_int zghz3293;
lbits zghz3294;
sail_int zghz3295;
sail_int zghz3296;
lbits zghz3297;
sail_int zghz3298;
sail_int zghz3299;
lbits zghz3300;
sail_int zghz3301;
sail_int zghz3302;
lbits zghz3303;
sail_int zghz3304;
sail_int zghz3305;
lbits zghz3306;
lbits zghz3307;
lbits zghz3308;
sail_int zghz3309;
sail_int zghz3310;
lbits zghz3311;
sail_int zghz3312;
lbits zghz3313;
sail_int zghz3314;
sail_int zghz3315;
lbits zghz3316;
sail_int zghz3317;
sail_int zghz3318;
sail_int zghz3319;
sail_int zghz3320;
sail_int zghz3321;
sail_int zghz3322;
lbits zghz3323;
sail_int zghz3324;
sail_int zghz3325;
sail_int zghz3326;
sail_int zghz3327;
sail_int zghz3328;
sail_int zghz3329;
sail_int zghz3330;
sail_int zghz3331;
sail_int zghz3332;
sail_int zghz3333;
sail_int zghz3334;
sail_int zghz3335;
lbits zghz3336;
sail_int zghz3337;
lbits zghz3338;
lbits zghz3339;
lbits zghz3340;
sail_int zghz3341;
lbits zghz3342;
lbits zghz3343;
sail_int zghz3344;
sail_int zghz3345;
lbits zghz3346;
sail_int zghz3347;
sail_int zghz3348;
sail_int zghz3349;
sail_int zghz3350;
sail_int zghz3351;
sail_int zghz3352;
sail_int zghz3353;
sail_int zghz3354;
sail_int zghz3355;
sail_int zghz3356;
sail_int zghz3357;
sail_int zghz3358;
sail_int zghz3359;
sail_int zghz3360;
sail_int zghz3361;
lbits zghz3362;
sail_int zghz3363;
sail_int zghz3364;
sail_int zghz3365;
sail_int zghz3366;
sail_int zghz3367;
sail_int zghz3368;
sail_int zghz3369;
sail_int zghz3370;
sail_int zghz3371;
sail_int zghz3372;
sail_int zghz3373;
sail_int zghz3374;
sail_int zghz3375;
sail_int zghz3376;
sail_int zghz3377;
sail_int zghz3378;
sail_int zghz3379;
sail_int zghz3380;
sail_int zghz3381;
sail_int zghz3382;
lbits zghz3383;
sail_int zghz3384;
sail_int zghz3385;
sail_int zghz3386;
sail_int zghz3387;
sail_int zghz3388;
sail_int zghz3389;
sail_int zghz3390;
sail_int zghz3391;
sail_int zghz3392;
sail_int zghz3393;
sail_int zghz3394;
sail_int zghz3395;
sail_int zghz3396;
sail_int zghz3397;
sail_int zghz3398;
sail_int zghz3399;
sail_int zghz3400;
sail_int zghz3401;
sail_int zghz3402;
lbits zghz3403;
sail_int zghz3404;
sail_int zghz3405;
sail_int zghz3406;
sail_int zghz3407;
sail_int zghz3408;
lbits zghz3409;
sail_int zghz3410;
sail_int zghz3411;
sail_int zghz3412;
lbits zghz3413;
lbits zghz3414;
lbits zghz3415;
lbits zghz3416;
sail_int zghz3417;
sail_int zghz3418;
sail_int zghz3419;
sail_int zghz3420;
sail_int zghz3421;
sail_int zghz3422;
sail_int zghz3423;
sail_int zghz3424;
lbits zghz3425;
sail_int zghz3426;
sail_int zghz3427;
sail_int zghz3428;
lbits zghz3429;
sail_int zghz3430;
sail_int zghz3431;
sail_int zghz3432;
lbits zghz3433;
lbits zghz3434;

static void startup_sailgen_CapSetBounds(void)
{
  CREATE(lbits)(&zghz3250);
  CREATE(lbits)(&zghz3251);
  CREATE(sail_int)(&zghz3252);
  CREATE(lbits)(&zghz3253);
  CREATE(lbits)(&zghz3254);
  CREATE(lbits)(&zghz3255);
  CREATE(sail_int)(&zghz3256);
  CREATE(lbits)(&zghz3257);
  CREATE(sail_int)(&zghz3258);
  CREATE(sail_int)(&zghz3259);
  CREATE(lbits)(&zghz3260);
  CREATE(sail_int)(&zghz3261);
  CREATE(lbits)(&zghz3262);
  CREATE(sail_int)(&zghz3263);
  CREATE(sail_int)(&zghz3264);
  CREATE(sail_int)(&zghz3265);
  CREATE(sail_int)(&zghz3266);
  CREATE(sail_int)(&zghz3267);
  CREATE(sail_int)(&zghz3268);
  CREATE(lbits)(&zghz3269);
  CREATE(sail_int)(&zghz3270);
  CREATE(sail_int)(&zghz3271);
  CREATE(sail_int)(&zghz3272);
  CREATE(sail_int)(&zghz3273);
  CREATE(sail_int)(&zghz3274);
  CREATE(lbits)(&zghz3275);
  CREATE(sail_int)(&zghz3276);
  CREATE(sail_int)(&zghz3277);
  CREATE(sail_int)(&zghz3278);
  CREATE(sail_int)(&zghz3279);
  CREATE(sail_int)(&zghz3280);
  CREATE(lbits)(&zghz3281);
  CREATE(sail_int)(&zghz3282);
  CREATE(sail_int)(&zghz3283);
  CREATE(lbits)(&zghz3284);
  CREATE(sail_int)(&zghz3285);
  CREATE(sail_int)(&zghz3286);
  CREATE(sail_int)(&zghz3287);
  CREATE(lbits)(&zghz3288);
  CREATE(sail_int)(&zghz3289);
  CREATE(sail_int)(&zghz3290);
  CREATE(lbits)(&zghz3291);
  CREATE(sail_int)(&zghz3292);
  CREATE(sail_int)(&zghz3293);
  CREATE(lbits)(&zghz3294);
  CREATE(sail_int)(&zghz3295);
  CREATE(sail_int)(&zghz3296);
  CREATE(lbits)(&zghz3297);
  CREATE(sail_int)(&zghz3298);
  CREATE(sail_int)(&zghz3299);
  CREATE(lbits)(&zghz3300);
  CREATE(sail_int)(&zghz3301);
  CREATE(sail_int)(&zghz3302);
  CREATE(lbits)(&zghz3303);
  CREATE(sail_int)(&zghz3304);
  CREATE(sail_int)(&zghz3305);
  CREATE(lbits)(&zghz3306);
  CREATE(lbits)(&zghz3307);
  CREATE(lbits)(&zghz3308);
  CREATE(sail_int)(&zghz3309);
  CREATE(sail_int)(&zghz3310);
  CREATE(lbits)(&zghz3311);
  CREATE(sail_int)(&zghz3312);
  CREATE(lbits)(&zghz3313);
  CREATE(sail_int)(&zghz3314);
  CREATE(sail_int)(&zghz3315);
  CREATE(lbits)(&zghz3316);
  CREATE(sail_int)(&zghz3317);
  CREATE(sail_int)(&zghz3318);
  CREATE(sail_int)(&zghz3319);
  CREATE(sail_int)(&zghz3320);
  CREATE(sail_int)(&zghz3321);
  CREATE(sail_int)(&zghz3322);
  CREATE(lbits)(&zghz3323);
  CREATE(sail_int)(&zghz3324);
  CREATE(sail_int)(&zghz3325);
  CREATE(sail_int)(&zghz3326);
  CREATE(sail_int)(&zghz3327);
  CREATE(sail_int)(&zghz3328);
  CREATE(sail_int)(&zghz3329);
  CREATE(sail_int)(&zghz3330);
  CREATE(sail_int)(&zghz3331);
  CREATE(sail_int)(&zghz3332);
  CREATE(sail_int)(&zghz3333);
  CREATE(sail_int)(&zghz3334);
  CREATE(sail_int)(&zghz3335);
  CREATE(lbits)(&zghz3336);
  CREATE(sail_int)(&zghz3337);
  CREATE(lbits)(&zghz3338);
  CREATE(lbits)(&zghz3339);
  CREATE(lbits)(&zghz3340);
  CREATE(sail_int)(&zghz3341);
  CREATE(lbits)(&zghz3342);
  CREATE(lbits)(&zghz3343);
  CREATE(sail_int)(&zghz3344);
  CREATE(sail_int)(&zghz3345);
  CREATE(lbits)(&zghz3346);
  CREATE(sail_int)(&zghz3347);
  CREATE(sail_int)(&zghz3348);
  CREATE(sail_int)(&zghz3349);
  CREATE(sail_int)(&zghz3350);
  CREATE(sail_int)(&zghz3351);
  CREATE(sail_int)(&zghz3352);
  CREATE(sail_int)(&zghz3353);
  CREATE(sail_int)(&zghz3354);
  CREATE(sail_int)(&zghz3355);
  CREATE(sail_int)(&zghz3356);
  CREATE(sail_int)(&zghz3357);
  CREATE(sail_int)(&zghz3358);
  CREATE(sail_int)(&zghz3359);
  CREATE(sail_int)(&zghz3360);
  CREATE(sail_int)(&zghz3361);
  CREATE(lbits)(&zghz3362);
  CREATE(sail_int)(&zghz3363);
  CREATE(sail_int)(&zghz3364);
  CREATE(sail_int)(&zghz3365);
  CREATE(sail_int)(&zghz3366);
  CREATE(sail_int)(&zghz3367);
  CREATE(sail_int)(&zghz3368);
  CREATE(sail_int)(&zghz3369);
  CREATE(sail_int)(&zghz3370);
  CREATE(sail_int)(&zghz3371);
  CREATE(sail_int)(&zghz3372);
  CREATE(sail_int)(&zghz3373);
  CREATE(sail_int)(&zghz3374);
  CREATE(sail_int)(&zghz3375);
  CREATE(sail_int)(&zghz3376);
  CREATE(sail_int)(&zghz3377);
  CREATE(sail_int)(&zghz3378);
  CREATE(sail_int)(&zghz3379);
  CREATE(sail_int)(&zghz3380);
  CREATE(sail_int)(&zghz3381);
  CREATE(sail_int)(&zghz3382);
  CREATE(lbits)(&zghz3383);
  CREATE(sail_int)(&zghz3384);
  CREATE(sail_int)(&zghz3385);
  CREATE(sail_int)(&zghz3386);
  CREATE(sail_int)(&zghz3387);
  CREATE(sail_int)(&zghz3388);
  CREATE(sail_int)(&zghz3389);
  CREATE(sail_int)(&zghz3390);
  CREATE(sail_int)(&zghz3391);
  CREATE(sail_int)(&zghz3392);
  CREATE(sail_int)(&zghz3393);
  CREATE(sail_int)(&zghz3394);
  CREATE(sail_int)(&zghz3395);
  CREATE(sail_int)(&zghz3396);
  CREATE(sail_int)(&zghz3397);
  CREATE(sail_int)(&zghz3398);
  CREATE(sail_int)(&zghz3399);
  CREATE(sail_int)(&zghz3400);
  CREATE(sail_int)(&zghz3401);
  CREATE(sail_int)(&zghz3402);
  CREATE(lbits)(&zghz3403);
  CREATE(sail_int)(&zghz3404);
  CREATE(sail_int)(&zghz3405);
  CREATE(sail_int)(&zghz3406);
  CREATE(sail_int)(&zghz3407);
  CREATE(sail_int)(&zghz3408);
  CREATE(lbits)(&zghz3409);
  CREATE(sail_int)(&zghz3410);
  CREATE(sail_int)(&zghz3411);
  CREATE(sail_int)(&zghz3412);
  CREATE(lbits)(&zghz3413);
  CREATE(lbits)(&zghz3414);
  CREATE(lbits)(&zghz3415);
  CREATE(lbits)(&zghz3416);
  CREATE(sail_int)(&zghz3417);
  CREATE(sail_int)(&zghz3418);
  CREATE(sail_int)(&zghz3419);
  CREATE(sail_int)(&zghz3420);
  CREATE(sail_int)(&zghz3421);
  CREATE(sail_int)(&zghz3422);
  CREATE(sail_int)(&zghz3423);
  CREATE(sail_int)(&zghz3424);
  CREATE(lbits)(&zghz3425);
  CREATE(sail_int)(&zghz3426);
  CREATE(sail_int)(&zghz3427);
  CREATE(sail_int)(&zghz3428);
  CREATE(lbits)(&zghz3429);
  CREATE(sail_int)(&zghz3430);
  CREATE(sail_int)(&zghz3431);
  CREATE(sail_int)(&zghz3432);
  CREATE(lbits)(&zghz3433);
  CREATE(lbits)(&zghz3434);
}

static void sailgen_CapSetBounds(lbits *zcbz334, lbits zc, lbits zreq_len, bool zexact)
{
  __label__ case_260, finish_match_259, end_function_263, end_block_exception_266, end_function_278;

  uint64_t zL_ie;
  zL_ie = UINT64_C(0b0000000000000);
  RECREATE(lbits)(&zghz3250);
  {
    RECREATE(lbits)(&zghz3434);
    CONVERT_OF(lbits, fbits)(&zghz3434, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3434, zghz3434, UINT64_C(0x0000000000000000));
    COPY(lbits)(&zghz3250, zghz3434);
  }
  RECREATE(lbits)(&zghz3251);
  {
    RECREATE(lbits)(&zghz3433);
    CONVERT_OF(lbits, fbits)(&zghz3433, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3433, zghz3433, UINT64_C(0x0000000000000000));
    COPY(lbits)(&zghz3251, zghz3433);
  }
  bool zovalid;
  zovalid = false;
  {
    bool zgaz3151;
    zgaz3151 = sailgen_CapUnsignedLessThanOrEqual(zreq_len, zCAP_BOUND_MAX);
    unit zgsz3140;
    zgsz3140 = sail_assert(zgaz3151, "v8_base.sail:45083.61-45083.62");
  }
  RECREATE(sail_int)(&zghz3252);
  {
    int64_t zgaz3241;
    {
      uint64_t zgaz3240;
      {
        int64_t zgaz3239;
        {
          RECREATE(sail_int)(&zghz3430);
          CONVERT_OF(sail_int, mach_int)(&zghz3430, zCAP_MW);
          RECREATE(sail_int)(&zghz3431);
          CONVERT_OF(sail_int, mach_int)(&zghz3431, INT64_C(1));
          RECREATE(sail_int)(&zghz3432);
          sub_int(&zghz3432, zghz3430, zghz3431);
          zgaz3239 = CONVERT_OF(mach_int, sail_int)(zghz3432);
        }
        {
          RECREATE(sail_int)(&zghz3427);
          CONVERT_OF(sail_int, mach_int)(&zghz3427, zCAP_VALUE_NUM_BITS);
          RECREATE(sail_int)(&zghz3428);
          CONVERT_OF(sail_int, mach_int)(&zghz3428, zgaz3239);
          RECREATE(lbits)(&zghz3429);
          vector_subrange_lbits(&zghz3429, zreq_len, zghz3427, zghz3428);
          zgaz3240 = CONVERT_OF(fbits, lbits)(zghz3429, true);
        }
      }
      {
        RECREATE(lbits)(&zghz3425);
        CONVERT_OF(lbits, fbits)(&zghz3425, zgaz3240, UINT64_C(50) , true);
        RECREATE(sail_int)(&zghz3426);
        count_leading_zeros(&zghz3426, zghz3425);
        zgaz3241 = CONVERT_OF(mach_int, sail_int)(zghz3426);
      }
    }
    {
      RECREATE(sail_int)(&zghz3423);
      CONVERT_OF(sail_int, mach_int)(&zghz3423, zCAP_MAX_EXPONENT);
      RECREATE(sail_int)(&zghz3424);
      CONVERT_OF(sail_int, mach_int)(&zghz3424, zgaz3241);
      sub_int(&zghz3252, zghz3423, zghz3424);
    }
  }
  bool zie;
  {
    bool zgaz3238;
    {
      RECREATE(sail_int)(&zghz3422);
      CONVERT_OF(sail_int, mach_int)(&zghz3422, INT64_C(0));
      zgaz3238 = sailgen_neq_int(zghz3252, zghz3422);
    }
    bool zgsz3142;
    if (zgaz3238) {  zgsz3142 = true;  } else {
      uint64_t zgaz3237;
      {
        fbits zgaz3236;
        {
          int64_t zgaz3235;
          {
            RECREATE(sail_int)(&zghz3419);
            CONVERT_OF(sail_int, mach_int)(&zghz3419, zCAP_MW);
            RECREATE(sail_int)(&zghz3420);
            CONVERT_OF(sail_int, mach_int)(&zghz3420, INT64_C(2));
            RECREATE(sail_int)(&zghz3421);
            sub_int(&zghz3421, zghz3419, zghz3420);
            zgaz3235 = CONVERT_OF(mach_int, sail_int)(zghz3421);
          }
          {
            RECREATE(sail_int)(&zghz3418);
            CONVERT_OF(sail_int, mach_int)(&zghz3418, zgaz3235);
            zgaz3236 = bitvector_access(zreq_len, zghz3418);
          }
        }
        zgaz3237 = UINT64_C(0b0);
        zgaz3237 = update_fbits(zgaz3237, INT64_C(0), zgaz3236);
      }
      zgsz3142 = (zgaz3237 == UINT64_C(0b1));
    }
    zie = zgsz3142;
  }
  uint64_t zbase;
  zbase = sailgen_CapGetValue(zc);
  uint64_t zabase;
  {
    bool zgaz3234;
    {
      int64_t zgaz3233;
      zgaz3233 = sailgen_CapGetExponent(zc);
      {
        RECREATE(sail_int)(&zghz3417);
        CONVERT_OF(sail_int, mach_int)(&zghz3417, zgaz3233);
        zgaz3234 = sailgen_CapBoundsUsesValue(zghz3417);
      }
    }
    if (zgaz3234) {  zabase = sailgen_CapBoundsAddress(zbase);  } else {  zabase = zbase;  }
  }
  RECREATE(lbits)(&zghz3253);
  {
    RECREATE(lbits)(&zghz3415);
    CONVERT_OF(lbits, fbits)(&zghz3415, UINT64_C(0b00), UINT64_C(2) , true);
    RECREATE(lbits)(&zghz3416);
    CONVERT_OF(lbits, fbits)(&zghz3416, zabase, UINT64_C(64) , true);
    append(&zghz3253, zghz3415, zghz3416);
  }
  RECREATE(lbits)(&zghz3254);
  {
    RECREATE(lbits)(&zghz3413);
    {
      RECREATE(lbits)(&zghz3414);
      CONVERT_OF(lbits, fbits)(&zghz3414, UINT64_C(0b0), UINT64_C(1) , true);
      append(&zghz3413, zghz3414, zreq_len);
    }
    add_bits(&zghz3254, zghz3253, zghz3413);
  }
  uint64_t zBbits;
  {
    int64_t zgaz3231;
    {
      RECREATE(sail_int)(&zghz3410);
      CONVERT_OF(sail_int, mach_int)(&zghz3410, zCAP_MW);
      RECREATE(sail_int)(&zghz3411);
      CONVERT_OF(sail_int, mach_int)(&zghz3411, INT64_C(1));
      RECREATE(sail_int)(&zghz3412);
      sub_int(&zghz3412, zghz3410, zghz3411);
      zgaz3231 = CONVERT_OF(mach_int, sail_int)(zghz3412);
    }
    {
      RECREATE(sail_int)(&zghz3407);
      CONVERT_OF(sail_int, mach_int)(&zghz3407, zgaz3231);
      RECREATE(sail_int)(&zghz3408);
      CONVERT_OF(sail_int, mach_int)(&zghz3408, INT64_C(0));
      RECREATE(lbits)(&zghz3409);
      vector_subrange_lbits(&zghz3409, zghz3253, zghz3407, zghz3408);
      zBbits = CONVERT_OF(fbits, lbits)(zghz3409, true);
    }
  }
  uint64_t zTBits;
  {
    int64_t zgaz3230;
    {
      RECREATE(sail_int)(&zghz3404);
      CONVERT_OF(sail_int, mach_int)(&zghz3404, zCAP_MW);
      RECREATE(sail_int)(&zghz3405);
      CONVERT_OF(sail_int, mach_int)(&zghz3405, INT64_C(1));
      RECREATE(sail_int)(&zghz3406);
      sub_int(&zghz3406, zghz3404, zghz3405);
      zgaz3230 = CONVERT_OF(mach_int, sail_int)(zghz3406);
    }
    {
      RECREATE(sail_int)(&zghz3401);
      CONVERT_OF(sail_int, mach_int)(&zghz3401, zgaz3230);
      RECREATE(sail_int)(&zghz3402);
      CONVERT_OF(sail_int, mach_int)(&zghz3402, INT64_C(0));
      RECREATE(lbits)(&zghz3403);
      vector_subrange_lbits(&zghz3403, zghz3254, zghz3401, zghz3402);
      zTBits = CONVERT_OF(fbits, lbits)(zghz3403, true);
    }
  }
  bool zlostTop;
  zlostTop = false;
  bool zlostBottom;
  zlostBottom = false;
  bool zincrementE_name;
  zincrementE_name = false;
  {
    unit zgsz3143;
    if (zie) {
      uint64_t zB_ie;
      {
        __label__ case_252, finish_match_251;

        {
          RECREATE(sail_int)(&zghz3380);
          COPY(sail_int)(&zghz3380, zghz3252);
          {
            bool zgaz3200;
            {
              bool zgaz3199;
              {
                RECREATE(sail_int)(&zghz3397);
                {
                  RECREATE(sail_int)(&zghz3399);
                  sailgen___id(&zghz3399, zghz3380);
                  {
                    RECREATE(sail_int)(&zghz3400);
                    CONVERT_OF(sail_int, mach_int)(&zghz3400, INT64_C(3));
                    add_int(&zghz3397, zghz3399, zghz3400);
                  }
                }
                {
                  RECREATE(sail_int)(&zghz3398);
                  CONVERT_OF(sail_int, mach_int)(&zghz3398, INT64_C(0));
                  zgaz3199 = lteq(zghz3398, zghz3397);
                }
              }
              bool zgsz3145;
              if (zgaz3199) {
                RECREATE(sail_int)(&zghz3391);
                {
                  RECREATE(sail_int)(&zghz3393);
                  {
                    RECREATE(sail_int)(&zghz3395);
                    sailgen___id(&zghz3395, zghz3380);
                    {
                      RECREATE(sail_int)(&zghz3396);
                      CONVERT_OF(sail_int, mach_int)(&zghz3396, INT64_C(16));
                      add_int(&zghz3393, zghz3395, zghz3396);
                    }
                  }
                  {
                    RECREATE(sail_int)(&zghz3394);
                    CONVERT_OF(sail_int, mach_int)(&zghz3394, INT64_C(1));
                    sub_int(&zghz3391, zghz3393, zghz3394);
                  }
                }
                {
                  RECREATE(sail_int)(&zghz3392);
                  CONVERT_OF(sail_int, mach_int)(&zghz3392, INT64_C(66));
                  zgsz3145 = lt(zghz3391, zghz3392);
                }
              } else {  zgsz3145 = false;  }
              zgaz3200 = zgsz3145;
            }
            unit zgsz3146;
            zgsz3146 = sail_assert(zgaz3200, "v8_base.sail:45098.69-45098.70");
          }
          int64_t zgaz3202;
          {
            int64_t zgaz3201;
            {
              RECREATE(sail_int)(&zghz3389);
              CONVERT_OF(sail_int, mach_int)(&zghz3389, zCAP_MW);
              RECREATE(sail_int)(&zghz3390);
              add_int(&zghz3390, zghz3380, zghz3389);
              zgaz3201 = CONVERT_OF(mach_int, sail_int)(zghz3390);
            }
            {
              RECREATE(sail_int)(&zghz3386);
              CONVERT_OF(sail_int, mach_int)(&zghz3386, zgaz3201);
              RECREATE(sail_int)(&zghz3387);
              CONVERT_OF(sail_int, mach_int)(&zghz3387, INT64_C(1));
              RECREATE(sail_int)(&zghz3388);
              sub_int(&zghz3388, zghz3386, zghz3387);
              zgaz3202 = CONVERT_OF(mach_int, sail_int)(zghz3388);
            }
          }
          int64_t zgaz3203;
          {
            RECREATE(sail_int)(&zghz3384);
            CONVERT_OF(sail_int, mach_int)(&zghz3384, INT64_C(3));
            RECREATE(sail_int)(&zghz3385);
            add_int(&zghz3385, zghz3380, zghz3384);
            zgaz3203 = CONVERT_OF(mach_int, sail_int)(zghz3385);
          }
          {
            RECREATE(sail_int)(&zghz3381);
            CONVERT_OF(sail_int, mach_int)(&zghz3381, zgaz3202);
            RECREATE(sail_int)(&zghz3382);
            CONVERT_OF(sail_int, mach_int)(&zghz3382, zgaz3203);
            RECREATE(lbits)(&zghz3383);
            vector_subrange_lbits(&zghz3383, zghz3253, zghz3381, zghz3382);
            zB_ie = CONVERT_OF(fbits, lbits)(zghz3383, true);
          }
          goto finish_match_251;
        }
      case_252: ;
        sail_match_failure("CapSetBounds");
      finish_match_251: ;
      }
      uint64_t zT_ie;
      {
        __label__ case_254, finish_match_253;

        {
          RECREATE(sail_int)(&zghz3359);
          COPY(sail_int)(&zghz3359, zghz3252);
          {
            bool zgaz3190;
            {
              bool zgaz3189;
              {
                RECREATE(sail_int)(&zghz3376);
                {
                  RECREATE(sail_int)(&zghz3378);
                  sailgen___id(&zghz3378, zghz3359);
                  {
                    RECREATE(sail_int)(&zghz3379);
                    CONVERT_OF(sail_int, mach_int)(&zghz3379, INT64_C(3));
                    add_int(&zghz3376, zghz3378, zghz3379);
                  }
                }
                {
                  RECREATE(sail_int)(&zghz3377);
                  CONVERT_OF(sail_int, mach_int)(&zghz3377, INT64_C(0));
                  zgaz3189 = lteq(zghz3377, zghz3376);
                }
              }
              bool zgsz3149;
              if (zgaz3189) {
                RECREATE(sail_int)(&zghz3370);
                {
                  RECREATE(sail_int)(&zghz3372);
                  {
                    RECREATE(sail_int)(&zghz3374);
                    sailgen___id(&zghz3374, zghz3359);
                    {
                      RECREATE(sail_int)(&zghz3375);
                      CONVERT_OF(sail_int, mach_int)(&zghz3375, INT64_C(16));
                      add_int(&zghz3372, zghz3374, zghz3375);
                    }
                  }
                  {
                    RECREATE(sail_int)(&zghz3373);
                    CONVERT_OF(sail_int, mach_int)(&zghz3373, INT64_C(1));
                    sub_int(&zghz3370, zghz3372, zghz3373);
                  }
                }
                {
                  RECREATE(sail_int)(&zghz3371);
                  CONVERT_OF(sail_int, mach_int)(&zghz3371, INT64_C(66));
                  zgsz3149 = lt(zghz3370, zghz3371);
                }
              } else {  zgsz3149 = false;  }
              zgaz3190 = zgsz3149;
            }
            unit zgsz3150;
            zgsz3150 = sail_assert(zgaz3190, "v8_base.sail:45103.69-45103.70");
          }
          int64_t zgaz3192;
          {
            int64_t zgaz3191;
            {
              RECREATE(sail_int)(&zghz3368);
              CONVERT_OF(sail_int, mach_int)(&zghz3368, zCAP_MW);
              RECREATE(sail_int)(&zghz3369);
              add_int(&zghz3369, zghz3359, zghz3368);
              zgaz3191 = CONVERT_OF(mach_int, sail_int)(zghz3369);
            }
            {
              RECREATE(sail_int)(&zghz3365);
              CONVERT_OF(sail_int, mach_int)(&zghz3365, zgaz3191);
              RECREATE(sail_int)(&zghz3366);
              CONVERT_OF(sail_int, mach_int)(&zghz3366, INT64_C(1));
              RECREATE(sail_int)(&zghz3367);
              sub_int(&zghz3367, zghz3365, zghz3366);
              zgaz3192 = CONVERT_OF(mach_int, sail_int)(zghz3367);
            }
          }
          int64_t zgaz3193;
          {
            RECREATE(sail_int)(&zghz3363);
            CONVERT_OF(sail_int, mach_int)(&zghz3363, INT64_C(3));
            RECREATE(sail_int)(&zghz3364);
            add_int(&zghz3364, zghz3359, zghz3363);
            zgaz3193 = CONVERT_OF(mach_int, sail_int)(zghz3364);
          }
          {
            RECREATE(sail_int)(&zghz3360);
            CONVERT_OF(sail_int, mach_int)(&zghz3360, zgaz3192);
            RECREATE(sail_int)(&zghz3361);
            CONVERT_OF(sail_int, mach_int)(&zghz3361, zgaz3193);
            RECREATE(lbits)(&zghz3362);
            vector_subrange_lbits(&zghz3362, zghz3254, zghz3360, zghz3361);
            zT_ie = CONVERT_OF(fbits, lbits)(zghz3362, true);
          }
          goto finish_match_253;
        }
      case_254: ;
        sail_match_failure("CapSetBounds");
      finish_match_253: ;
      }
      RECREATE(lbits)(&zghz3308);
      {
        __label__ case_256, finish_match_255;

        {
          RECREATE(sail_int)(&zghz3345);
          COPY(sail_int)(&zghz3345, zghz3252);
          {
            bool zgaz3177;
            {
              RECREATE(sail_int)(&zghz3355);
              {
                RECREATE(sail_int)(&zghz3357);
                sailgen___id(&zghz3357, zghz3345);
                {
                  RECREATE(sail_int)(&zghz3358);
                  CONVERT_OF(sail_int, mach_int)(&zghz3358, INT64_C(3));
                  add_int(&zghz3355, zghz3357, zghz3358);
                }
              }
              {
                RECREATE(sail_int)(&zghz3356);
                CONVERT_OF(sail_int, mach_int)(&zghz3356, INT64_C(0));
                zgaz3177 = gteq(zghz3355, zghz3356);
              }
            }
            unit zgsz3154;
            zgsz3154 = sail_assert(zgaz3177, "v8_base.sail:45108.46-45108.47");
          }
          {
            bool zgaz3180;
            {
              RECREATE(sail_int)(&zghz3351);
              {
                RECREATE(sail_int)(&zghz3353);
                sailgen___id(&zghz3353, zghz3345);
                {
                  RECREATE(sail_int)(&zghz3354);
                  CONVERT_OF(sail_int, mach_int)(&zghz3354, INT64_C(3));
                  add_int(&zghz3351, zghz3353, zghz3354);
                }
              }
              {
                RECREATE(sail_int)(&zghz3352);
                CONVERT_OF(sail_int, mach_int)(&zghz3352, INT64_C(66));
                zgaz3180 = gteq(zghz3352, zghz3351);
              }
            }
            unit zgsz3153;
            zgsz3153 = sail_assert(zgaz3180, "v8_base.sail:45109.47-45109.48");
          }
          RECREATE(lbits)(&zghz3346);
          {
            int64_t zgaz3181;
            {
              RECREATE(sail_int)(&zghz3349);
              CONVERT_OF(sail_int, mach_int)(&zghz3349, INT64_C(3));
              RECREATE(sail_int)(&zghz3350);
              add_int(&zghz3350, zghz3345, zghz3349);
              zgaz3181 = CONVERT_OF(mach_int, sail_int)(zghz3350);
            }
            {
              RECREATE(sail_int)(&zghz3348);
              CONVERT_OF(sail_int, mach_int)(&zghz3348, zgaz3181);
              sailgen_Ones(&zghz3346, zghz3348);
            }
          }
          int64_t zgaz3183;
          zgaz3183 = (zCAP_VALUE_NUM_BITS + INT64_C(2));
          {
            RECREATE(sail_int)(&zghz3347);
            CONVERT_OF(sail_int, mach_int)(&zghz3347, zgaz3183);
            zero_extend(&zghz3308, zghz3346, zghz3347);
          }
          goto finish_match_255;
        }
      case_256: ;
        sail_match_failure("CapSetBounds");
      finish_match_255: ;
      }
      {
        RECREATE(lbits)(&zghz3342);
        and_bits(&zghz3342, zghz3253, zghz3308);
        RECREATE(lbits)(&zghz3343);
        {
          int64_t zgaz3152;
          zgaz3152 = (zCAP_VALUE_NUM_BITS + INT64_C(2));
          {
            RECREATE(sail_int)(&zghz3344);
            CONVERT_OF(sail_int, mach_int)(&zghz3344, zgaz3152);
            sailgen_Zeros(&zghz3343, zghz3344);
          }
        }
        zlostBottom = neq_bits(zghz3342, zghz3343);
        unit zgsz3158;
        zgsz3158 = UNIT;
      }
      {
        RECREATE(lbits)(&zghz3339);
        and_bits(&zghz3339, zghz3254, zghz3308);
        RECREATE(lbits)(&zghz3340);
        {
          int64_t zgaz3155;
          zgaz3155 = (zCAP_VALUE_NUM_BITS + INT64_C(2));
          {
            RECREATE(sail_int)(&zghz3341);
            CONVERT_OF(sail_int, mach_int)(&zghz3341, zgaz3155);
            sailgen_Zeros(&zghz3340, zghz3341);
          }
        }
        zlostTop = neq_bits(zghz3339, zghz3340);
        unit zgsz3157;
        zgsz3157 = UNIT;
      }
      {
        unit zgsz3156;
        if (zlostTop) {
          {
            RECREATE(lbits)(&zghz3336);
            CONVERT_OF(lbits, fbits)(&zghz3336, zT_ie, UINT64_C(13) , true);
            RECREATE(sail_int)(&zghz3337);
            CONVERT_OF(sail_int, mach_int)(&zghz3337, INT64_C(1));
            RECREATE(lbits)(&zghz3338);
            add_bits_int(&zghz3338, zghz3336, zghz3337);
            zT_ie = CONVERT_OF(fbits, lbits)(zghz3338, true);
          }
          zgsz3156 = UNIT;
        } else {  zgsz3156 = UNIT;  }
      }
      uint64_t zL_ieshadowz314;
      zL_ieshadowz314 = ((zT_ie - zB_ie) & UINT64_C(0b1111111111111));
      {
        __label__ case_258, finish_match_257;

        unit zgsz3159;
        {
          RECREATE(sail_int)(&zghz3310);
          COPY(sail_int)(&zghz3310, zghz3252);
          bool zgaz3161;
          {
            uint64_t zgaz3160;
            {
              fbits zgaz3159;
              {
                int64_t zgaz3158;
                {
                  RECREATE(sail_int)(&zghz3333);
                  CONVERT_OF(sail_int, mach_int)(&zghz3333, zCAP_MW);
                  RECREATE(sail_int)(&zghz3334);
                  CONVERT_OF(sail_int, mach_int)(&zghz3334, INT64_C(4));
                  RECREATE(sail_int)(&zghz3335);
                  sub_int(&zghz3335, zghz3333, zghz3334);
                  zgaz3158 = CONVERT_OF(mach_int, sail_int)(zghz3335);
                }
                zgaz3159 = (UINT64_C(1) & (zL_ieshadowz314 >> zgaz3158));
              }
              zgaz3160 = UINT64_C(0b0);
              zgaz3160 = update_fbits(zgaz3160, INT64_C(0), zgaz3159);
            }
            zgaz3161 = (zgaz3160 == UINT64_C(0b1));
          }
          if (zgaz3161) {
            {
              zincrementE_name = true;
              unit zgsz3172;
              zgsz3172 = UNIT;
            }
            {
              bool zgsz3163;
              if (zlostBottom) {  zgsz3163 = true;  } else {
                uint64_t zgaz3163;
                {
                  fbits zgaz3162;
                  zgaz3162 = (UINT64_C(1) & (zB_ie >> INT64_C(0)));
                  zgaz3163 = UINT64_C(0b0);
                  zgaz3163 = update_fbits(zgaz3163, INT64_C(0), zgaz3162);
                }
                zgsz3163 = (zgaz3163 == UINT64_C(0b1));
              }
              zlostBottom = zgsz3163;
              unit zgsz3171;
              zgsz3171 = UNIT;
            }
            {
              bool zgsz3165;
              if (zlostTop) {  zgsz3165 = true;  } else {
                uint64_t zgaz3165;
                {
                  fbits zgaz3164;
                  zgaz3164 = (UINT64_C(1) & (zT_ie >> INT64_C(0)));
                  zgaz3165 = UINT64_C(0b0);
                  zgaz3165 = update_fbits(zgaz3165, INT64_C(0), zgaz3164);
                }
                zgsz3165 = (zgaz3165 == UINT64_C(0b1));
              }
              zlostTop = zgsz3165;
              unit zgsz3170;
              zgsz3170 = UNIT;
            }
            {
              bool zgaz3166;
              {
                RECREATE(sail_int)(&zghz3332);
                CONVERT_OF(sail_int, mach_int)(&zghz3332, zCAP_MAX_EXPONENT);
                zgaz3166 = lt(zghz3310, zghz3332);
              }
              unit zgsz3169;
              zgsz3169 = sail_assert(zgaz3166, "v8_base.sail:45124.47-45124.48");
            }
            {
              bool zgaz3169;
              {
                RECREATE(sail_int)(&zghz3328);
                {
                  RECREATE(sail_int)(&zghz3330);
                  sailgen___id(&zghz3330, zghz3310);
                  {
                    RECREATE(sail_int)(&zghz3331);
                    CONVERT_OF(sail_int, mach_int)(&zghz3331, INT64_C(4));
                    add_int(&zghz3328, zghz3330, zghz3331);
                  }
                }
                {
                  RECREATE(sail_int)(&zghz3329);
                  CONVERT_OF(sail_int, mach_int)(&zghz3329, INT64_C(0));
                  zgaz3169 = lteq(zghz3329, zghz3328);
                }
              }
              unit zgsz3168;
              zgsz3168 = sail_assert(zgaz3169, "v8_base.sail:45125.50-45125.51");
            }
            {
              int64_t zgaz3170;
              {
                RECREATE(sail_int)(&zghz3326);
                CONVERT_OF(sail_int, mach_int)(&zghz3326, zCAP_MW);
                RECREATE(sail_int)(&zghz3327);
                add_int(&zghz3327, zghz3310, zghz3326);
                zgaz3170 = CONVERT_OF(mach_int, sail_int)(zghz3327);
              }
              int64_t zgaz3171;
              {
                RECREATE(sail_int)(&zghz3324);
                CONVERT_OF(sail_int, mach_int)(&zghz3324, INT64_C(4));
                RECREATE(sail_int)(&zghz3325);
                add_int(&zghz3325, zghz3310, zghz3324);
                zgaz3171 = CONVERT_OF(mach_int, sail_int)(zghz3325);
              }
              {
                RECREATE(sail_int)(&zghz3321);
                CONVERT_OF(sail_int, mach_int)(&zghz3321, zgaz3170);
                RECREATE(sail_int)(&zghz3322);
                CONVERT_OF(sail_int, mach_int)(&zghz3322, zgaz3171);
                RECREATE(lbits)(&zghz3323);
                vector_subrange_lbits(&zghz3323, zghz3253, zghz3321, zghz3322);
                zB_ie = CONVERT_OF(fbits, lbits)(zghz3323, true);
              }
              unit zgsz3167;
              zgsz3167 = UNIT;
            }
            {
              int64_t zgaz3172;
              {
                RECREATE(sail_int)(&zghz3319);
                CONVERT_OF(sail_int, mach_int)(&zghz3319, zCAP_MW);
                RECREATE(sail_int)(&zghz3320);
                add_int(&zghz3320, zghz3310, zghz3319);
                zgaz3172 = CONVERT_OF(mach_int, sail_int)(zghz3320);
              }
              int64_t zgaz3173;
              {
                RECREATE(sail_int)(&zghz3317);
                CONVERT_OF(sail_int, mach_int)(&zghz3317, INT64_C(4));
                RECREATE(sail_int)(&zghz3318);
                add_int(&zghz3318, zghz3310, zghz3317);
                zgaz3173 = CONVERT_OF(mach_int, sail_int)(zghz3318);
              }
              {
                RECREATE(sail_int)(&zghz3314);
                CONVERT_OF(sail_int, mach_int)(&zghz3314, zgaz3172);
                RECREATE(sail_int)(&zghz3315);
                CONVERT_OF(sail_int, mach_int)(&zghz3315, zgaz3173);
                RECREATE(lbits)(&zghz3316);
                vector_subrange_lbits(&zghz3316, zghz3254, zghz3314, zghz3315);
                zT_ie = CONVERT_OF(fbits, lbits)(zghz3316, true);
              }
              unit zgsz3166;
              zgsz3166 = UNIT;
            }
            if (zlostTop) {
              {
                RECREATE(lbits)(&zghz3311);
                CONVERT_OF(lbits, fbits)(&zghz3311, zT_ie, UINT64_C(13) , true);
                RECREATE(sail_int)(&zghz3312);
                CONVERT_OF(sail_int, mach_int)(&zghz3312, INT64_C(1));
                RECREATE(lbits)(&zghz3313);
                add_bits_int(&zghz3313, zghz3311, zghz3312);
                zT_ie = CONVERT_OF(fbits, lbits)(zghz3313, true);
              }
              zgsz3159 = UNIT;
            } else {  zgsz3159 = UNIT;  }
          } else {  zgsz3159 = UNIT;  }
          goto finish_match_257;
        }
      case_258: ;
        sail_match_failure("CapSetBounds");
      finish_match_257: ;
        unit zgsz3175;
        zgsz3175 = zgsz3159;
      }
      {
        bool zgaz3174;
        zgaz3174 = eq_bool(zincrementE_name, true);
        unit zgsz3174;
        if (zgaz3174) {
          {
            RECREATE(sail_int)(&zghz3309);
            CONVERT_OF(sail_int, mach_int)(&zghz3309, INT64_C(1));
            add_int(&zghz3252, zghz3252, zghz3309);
          }
          zgsz3174 = UNIT;
        } else {  zgsz3174 = UNIT;  }
      }
      {
        zBbits = (zB_ie << 3) | UINT64_C(0b000);
        unit zgsz3173;
        zgsz3173 = UNIT;
      }
      zTBits = (zT_ie << 3) | UINT64_C(0b000);
      zgsz3143 = UNIT;
    } else {  zgsz3143 = UNIT;  }
  }
  RECREATE(lbits)(&zghz3255);
  {
    __label__ cleanup_264, end_cleanup_265;

    RECREATE(sail_int)(&zghz3256);
    COPY(sail_int)(&zghz3256, zghz3252);
    RECREATE(lbits)(&zghz3257);
    COPY(lbits)(&zghz3257, zc);
    {
      __label__ case_262, finish_match_261;

      struct ztuple_z8z5bvzCz0z5bvzCz0z5boolz9 zgaz3204;
      CREATE(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgaz3204);
      sailgen_CapGetBounds(&zgaz3204, zc);
      unit zgsz3177;
      {
        RECREATE(lbits)(&zghz3306);
        COPY(lbits)(&zghz3306, zgaz3204.ztup0);
        RECREATE(lbits)(&zghz3307);
        COPY(lbits)(&zghz3307, zgaz3204.ztup1);
        bool ztup__2;
        ztup__2 = zgaz3204.ztup2;
        {
          COPY(lbits)(&zghz3250, zghz3306);
          unit zgsz3179;
          zgsz3179 = UNIT;
        }
        {
          COPY(lbits)(&zghz3251, zghz3307);
          unit zgsz3178;
          zgsz3178 = UNIT;
        }
        zovalid = ztup__2;
        zgsz3177 = UNIT;
        goto finish_match_261;
      }
    case_262: ;
      sail_match_failure("CapSetBounds");
    finish_match_261: ;
      unit zgsz3191;
      zgsz3191 = zgsz3177;
      KILL(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgaz3204);
    }
    {
      bool zgaz3211;
      {
        bool zgaz3210;
        {
          bool zgaz3209;
          {
            bool zgaz3206;
            {
              RECREATE(lbits)(&zghz3303);
              {
                RECREATE(sail_int)(&zghz3304);
                CONVERT_OF(sail_int, mach_int)(&zghz3304, INT64_C(0));
                RECREATE(sail_int)(&zghz3305);
                CONVERT_OF(sail_int, mach_int)(&zghz3305, zCAP_BOUND_NUM_BITS);
                slice(&zghz3303, zghz3253, zghz3304, zghz3305);
              }
              zgaz3206 = sailgen_CapUnsignedGreaterThanOrEqual(zghz3303, zghz3250);
            }
            zgaz3209 = not(zgaz3206);
          }
          bool zgsz3181;
          if (zgaz3209) {  zgsz3181 = true;  } else {
            bool zgaz3208;
            {
              RECREATE(lbits)(&zghz3300);
              {
                RECREATE(sail_int)(&zghz3301);
                CONVERT_OF(sail_int, mach_int)(&zghz3301, INT64_C(0));
                RECREATE(sail_int)(&zghz3302);
                CONVERT_OF(sail_int, mach_int)(&zghz3302, zCAP_BOUND_NUM_BITS);
                slice(&zghz3300, zghz3254, zghz3301, zghz3302);
              }
              zgaz3208 = sailgen_CapUnsignedLessThanOrEqual(zghz3300, zghz3251);
            }
            zgsz3181 = not(zgaz3208);
          }
          zgaz3210 = zgsz3181;
        }
        bool zgsz3182;
        if (zgaz3210) {  zgsz3182 = true;  } else {  zgsz3182 = not(zovalid);  }
        zgaz3211 = zgsz3182;
      }
      unit zgsz3190;
      if (zgaz3211) {
        {
          RECREATE(sail_int)(&zghz3299);
          CONVERT_OF(sail_int, mach_int)(&zghz3299, zCAP_TAG_BIT);
          update_lbits(&zghz3257, zghz3257, zghz3299, UINT64_C(0));
        }
        zgsz3190 = UNIT;
      } else {  zgsz3190 = UNIT;  }
    }
    {
      unit zgsz3185;
      if (zie) {
        {
          {
            RECREATE(sail_int)(&zghz3298);
            CONVERT_OF(sail_int, mach_int)(&zghz3298, zCAP_IE_BIT);
            update_lbits(&zghz3257, zghz3257, zghz3298, UINT64_C(0));
          }
          unit zgsz3189;
          zgsz3189 = UNIT;
        }
        {
          uint64_t zgaz3213;
          {
            uint64_t zgaz3212;
            {
              RECREATE(sail_int)(&zghz3295);
              CONVERT_OF(sail_int, mach_int)(&zghz3295, INT64_C(2));
              RECREATE(sail_int)(&zghz3296);
              CONVERT_OF(sail_int, mach_int)(&zghz3296, INT64_C(0));
              RECREATE(lbits)(&zghz3297);
              sailgen_integer_subrange(&zghz3297, zghz3256, zghz3295, zghz3296);
              zgaz3212 = CONVERT_OF(fbits, lbits)(zghz3297, true);
            }
            zgaz3213 = (~(zgaz3212) & UINT64_C(0b111));
          }
          {
            RECREATE(sail_int)(&zghz3292);
            CONVERT_OF(sail_int, mach_int)(&zghz3292, zCAP_BASE_EXP_HI_BIT);
            RECREATE(sail_int)(&zghz3293);
            CONVERT_OF(sail_int, mach_int)(&zghz3293, zCAP_BASE_LO_BIT);
            RECREATE(lbits)(&zghz3294);
            CONVERT_OF(lbits, fbits)(&zghz3294, zgaz3213, UINT64_C(3) , true);
            vector_update_subrange_lbits(&zghz3257, zghz3257, zghz3292, zghz3293, zghz3294);
          }
          unit zgsz3188;
          zgsz3188 = UNIT;
        }
        uint64_t zgaz3215;
        {
          uint64_t zgaz3214;
          {
            RECREATE(sail_int)(&zghz3289);
            CONVERT_OF(sail_int, mach_int)(&zghz3289, INT64_C(5));
            RECREATE(sail_int)(&zghz3290);
            CONVERT_OF(sail_int, mach_int)(&zghz3290, INT64_C(3));
            RECREATE(lbits)(&zghz3291);
            sailgen_integer_subrange(&zghz3291, zghz3256, zghz3289, zghz3290);
            zgaz3214 = CONVERT_OF(fbits, lbits)(zghz3291, true);
          }
          zgaz3215 = (~(zgaz3214) & UINT64_C(0b111));
        }
        {
          RECREATE(sail_int)(&zghz3286);
          CONVERT_OF(sail_int, mach_int)(&zghz3286, zCAP_LIMIT_EXP_HI_BIT);
          RECREATE(sail_int)(&zghz3287);
          CONVERT_OF(sail_int, mach_int)(&zghz3287, zCAP_LIMIT_LO_BIT);
          RECREATE(lbits)(&zghz3288);
          CONVERT_OF(lbits, fbits)(&zghz3288, zgaz3215, UINT64_C(3) , true);
          vector_update_subrange_lbits(&zghz3257, zghz3257, zghz3286, zghz3287, zghz3288);
        }
        zgsz3185 = UNIT;
      } else {
        {
          {
            RECREATE(sail_int)(&zghz3285);
            CONVERT_OF(sail_int, mach_int)(&zghz3285, zCAP_IE_BIT);
            update_lbits(&zghz3257, zghz3257, zghz3285, UINT64_C(1));
          }
          unit zgsz3187;
          zgsz3187 = UNIT;
        }
        {
          uint64_t zgaz3216;
          zgaz3216 = (safe_rshift(UINT64_MAX, 64 - 3) & (zBbits >> INT64_C(0)));
          {
            RECREATE(sail_int)(&zghz3282);
            CONVERT_OF(sail_int, mach_int)(&zghz3282, zCAP_BASE_EXP_HI_BIT);
            RECREATE(sail_int)(&zghz3283);
            CONVERT_OF(sail_int, mach_int)(&zghz3283, zCAP_BASE_LO_BIT);
            RECREATE(lbits)(&zghz3284);
            CONVERT_OF(lbits, fbits)(&zghz3284, zgaz3216, UINT64_C(3) , true);
            vector_update_subrange_lbits(&zghz3257, zghz3257, zghz3282, zghz3283, zghz3284);
          }
          unit zgsz3186;
          zgsz3186 = UNIT;
        }
        uint64_t zgaz3217;
        zgaz3217 = (safe_rshift(UINT64_MAX, 64 - 3) & (zTBits >> INT64_C(0)));
        {
          RECREATE(sail_int)(&zghz3279);
          CONVERT_OF(sail_int, mach_int)(&zghz3279, zCAP_LIMIT_EXP_HI_BIT);
          RECREATE(sail_int)(&zghz3280);
          CONVERT_OF(sail_int, mach_int)(&zghz3280, zCAP_LIMIT_LO_BIT);
          RECREATE(lbits)(&zghz3281);
          CONVERT_OF(lbits, fbits)(&zghz3281, zgaz3217, UINT64_C(3) , true);
          vector_update_subrange_lbits(&zghz3257, zghz3257, zghz3279, zghz3280, zghz3281);
        }
        zgsz3185 = UNIT;
      }
    }
    {
      uint64_t zgaz3219;
      {
        int64_t zgaz3218;
        {
          RECREATE(sail_int)(&zghz3276);
          CONVERT_OF(sail_int, mach_int)(&zghz3276, zCAP_MW);
          RECREATE(sail_int)(&zghz3277);
          CONVERT_OF(sail_int, mach_int)(&zghz3277, INT64_C(1));
          RECREATE(sail_int)(&zghz3278);
          sub_int(&zghz3278, zghz3276, zghz3277);
          zgaz3218 = CONVERT_OF(mach_int, sail_int)(zghz3278);
        }
        zgaz3219 = (safe_rshift(UINT64_MAX, 64 - 13) & (zBbits >> INT64_C(3)));
      }
      {
        RECREATE(sail_int)(&zghz3273);
        CONVERT_OF(sail_int, mach_int)(&zghz3273, zCAP_BASE_HI_BIT);
        RECREATE(sail_int)(&zghz3274);
        CONVERT_OF(sail_int, mach_int)(&zghz3274, zCAP_BASE_MANTISSA_LO_BIT);
        RECREATE(lbits)(&zghz3275);
        CONVERT_OF(lbits, fbits)(&zghz3275, zgaz3219, UINT64_C(13) , true);
        vector_update_subrange_lbits(&zghz3257, zghz3257, zghz3273, zghz3274, zghz3275);
      }
      unit zgsz3184;
      zgsz3184 = UNIT;
    }
    {
      uint64_t zgaz3221;
      {
        int64_t zgaz3220;
        {
          RECREATE(sail_int)(&zghz3270);
          CONVERT_OF(sail_int, mach_int)(&zghz3270, zCAP_MW);
          RECREATE(sail_int)(&zghz3271);
          CONVERT_OF(sail_int, mach_int)(&zghz3271, INT64_C(3));
          RECREATE(sail_int)(&zghz3272);
          sub_int(&zghz3272, zghz3270, zghz3271);
          zgaz3220 = CONVERT_OF(mach_int, sail_int)(zghz3272);
        }
        zgaz3221 = (safe_rshift(UINT64_MAX, 64 - 11) & (zTBits >> INT64_C(3)));
      }
      {
        RECREATE(sail_int)(&zghz3267);
        CONVERT_OF(sail_int, mach_int)(&zghz3267, zCAP_LIMIT_HI_BIT);
        RECREATE(sail_int)(&zghz3268);
        CONVERT_OF(sail_int, mach_int)(&zghz3268, zCAP_LIMIT_MANTISSA_LO_BIT);
        RECREATE(lbits)(&zghz3269);
        CONVERT_OF(lbits, fbits)(&zghz3269, zgaz3221, UINT64_C(11) , true);
        vector_update_subrange_lbits(&zghz3257, zghz3257, zghz3267, zghz3268, zghz3269);
      }
      unit zgsz3183;
      zgsz3183 = UNIT;
    }
    bool zfrom_large;
    {
      bool zgaz3229;
      {
        int64_t zgaz3228;
        zgaz3228 = sailgen_CapGetExponent(zc);
        {
          RECREATE(sail_int)(&zghz3266);
          CONVERT_OF(sail_int, mach_int)(&zghz3266, zgaz3228);
          zgaz3229 = sailgen_CapBoundsUsesValue(zghz3266);
        }
      }
      zfrom_large = not(zgaz3229);
    }
    bool zto_small;
    zto_small = sailgen_CapBoundsUsesValue(zghz3256);
    {
      bool zgaz3226;
      {
        bool zgaz3225;
        {    if (zfrom_large) {  zgaz3225 = zto_small;  } else {  zgaz3225 = false;  }
        }
        bool zgsz3193;
        if (zgaz3225) {
          uint64_t zgaz3224;
          {
            uint64_t zgaz3223;
            {
              int64_t zgaz3222;
              {
                RECREATE(sail_int)(&zghz3263);
                CONVERT_OF(sail_int, mach_int)(&zghz3263, zCAP_FLAGS_LO_BIT);
                RECREATE(sail_int)(&zghz3264);
                CONVERT_OF(sail_int, mach_int)(&zghz3264, INT64_C(1));
                RECREATE(sail_int)(&zghz3265);
                sub_int(&zghz3265, zghz3263, zghz3264);
                zgaz3222 = CONVERT_OF(mach_int, sail_int)(zghz3265);
              }
              zgaz3223 = (safe_rshift(UINT64_MAX, 64 - 56) & (zbase >> INT64_C(0)));
            }
            {
              RECREATE(lbits)(&zghz3260);
              CONVERT_OF(lbits, fbits)(&zghz3260, zgaz3223, UINT64_C(56) , true);
              RECREATE(sail_int)(&zghz3261);
              CONVERT_OF(sail_int, mach_int)(&zghz3261, INT64_C(64));
              RECREATE(lbits)(&zghz3262);
              sign_extend(&zghz3262, zghz3260, zghz3261);
              zgaz3224 = CONVERT_OF(fbits, lbits)(zghz3262, true);
            }
          }
          zgsz3193 = (zgaz3224 != zbase);
        } else {  zgsz3193 = false;  }
        zgaz3226 = zgsz3193;
      }
      unit zgsz3197;
      if (zgaz3226) {
        {
          RECREATE(sail_int)(&zghz3259);
          CONVERT_OF(sail_int, mach_int)(&zghz3259, zCAP_TAG_BIT);
          update_lbits(&zghz3257, zghz3257, zghz3259, UINT64_C(0));
        }
        zgsz3197 = UNIT;
      } else {  zgsz3197 = UNIT;  }
    }
    {
      bool zgaz3227;
      {
        if (zexact) {
          bool zgsz3194;
          if (zlostBottom) {  zgsz3194 = true;  } else {  zgsz3194 = zlostTop;  }
          zgaz3227 = zgsz3194;
        } else {  zgaz3227 = false;  }
      }
      unit zgsz3196;
      if (zgaz3227) {
        {
          RECREATE(sail_int)(&zghz3258);
          CONVERT_OF(sail_int, mach_int)(&zghz3258, zCAP_TAG_BIT);
          update_lbits(&zghz3257, zghz3257, zghz3258, UINT64_C(0));
        }
        zgsz3196 = UNIT;
      } else {  zgsz3196 = UNIT;  }
    }
    COPY(lbits)((*(&zcbz334)), zghz3257);
    goto cleanup_264;
    /* unreachable after return */
    goto finish_match_259;
    goto end_cleanup_265;
  cleanup_264: ;
    goto end_function_263;
  end_cleanup_265: ;
  }
case_260: ;
  sail_match_failure("CapSetBounds");
finish_match_259: ;
  COPY(lbits)((*(&zcbz334)), zghz3255);










end_function_263: ;
  goto end_function_278;
end_block_exception_266: ;
  goto end_function_278;
end_function_278: ;
}



static void finish_sailgen_CapSetBounds(void)
{
  KILL(lbits)(&zghz3434);
  KILL(lbits)(&zghz3433);
  KILL(sail_int)(&zghz3432);
  KILL(sail_int)(&zghz3431);
  KILL(sail_int)(&zghz3430);
  KILL(lbits)(&zghz3429);
  KILL(sail_int)(&zghz3428);
  KILL(sail_int)(&zghz3427);
  KILL(sail_int)(&zghz3426);
  KILL(lbits)(&zghz3425);
  KILL(sail_int)(&zghz3424);
  KILL(sail_int)(&zghz3423);
  KILL(sail_int)(&zghz3422);
  KILL(sail_int)(&zghz3421);
  KILL(sail_int)(&zghz3420);
  KILL(sail_int)(&zghz3419);
  KILL(sail_int)(&zghz3418);
  KILL(sail_int)(&zghz3417);
  KILL(lbits)(&zghz3416);
  KILL(lbits)(&zghz3415);
  KILL(lbits)(&zghz3414);
  KILL(lbits)(&zghz3413);
  KILL(sail_int)(&zghz3412);
  KILL(sail_int)(&zghz3411);
  KILL(sail_int)(&zghz3410);
  KILL(lbits)(&zghz3409);
  KILL(sail_int)(&zghz3408);
  KILL(sail_int)(&zghz3407);
  KILL(sail_int)(&zghz3406);
  KILL(sail_int)(&zghz3405);
  KILL(sail_int)(&zghz3404);
  KILL(lbits)(&zghz3403);
  KILL(sail_int)(&zghz3402);
  KILL(sail_int)(&zghz3401);
  KILL(sail_int)(&zghz3400);
  KILL(sail_int)(&zghz3399);
  KILL(sail_int)(&zghz3398);
  KILL(sail_int)(&zghz3397);
  KILL(sail_int)(&zghz3396);
  KILL(sail_int)(&zghz3395);
  KILL(sail_int)(&zghz3394);
  KILL(sail_int)(&zghz3393);
  KILL(sail_int)(&zghz3392);
  KILL(sail_int)(&zghz3391);
  KILL(sail_int)(&zghz3390);
  KILL(sail_int)(&zghz3389);
  KILL(sail_int)(&zghz3388);
  KILL(sail_int)(&zghz3387);
  KILL(sail_int)(&zghz3386);
  KILL(sail_int)(&zghz3385);
  KILL(sail_int)(&zghz3384);
  KILL(lbits)(&zghz3383);
  KILL(sail_int)(&zghz3382);
  KILL(sail_int)(&zghz3381);
  KILL(sail_int)(&zghz3380);
  KILL(sail_int)(&zghz3379);
  KILL(sail_int)(&zghz3378);
  KILL(sail_int)(&zghz3377);
  KILL(sail_int)(&zghz3376);
  KILL(sail_int)(&zghz3375);
  KILL(sail_int)(&zghz3374);
  KILL(sail_int)(&zghz3373);
  KILL(sail_int)(&zghz3372);
  KILL(sail_int)(&zghz3371);
  KILL(sail_int)(&zghz3370);
  KILL(sail_int)(&zghz3369);
  KILL(sail_int)(&zghz3368);
  KILL(sail_int)(&zghz3367);
  KILL(sail_int)(&zghz3366);
  KILL(sail_int)(&zghz3365);
  KILL(sail_int)(&zghz3364);
  KILL(sail_int)(&zghz3363);
  KILL(lbits)(&zghz3362);
  KILL(sail_int)(&zghz3361);
  KILL(sail_int)(&zghz3360);
  KILL(sail_int)(&zghz3359);
  KILL(sail_int)(&zghz3358);
  KILL(sail_int)(&zghz3357);
  KILL(sail_int)(&zghz3356);
  KILL(sail_int)(&zghz3355);
  KILL(sail_int)(&zghz3354);
  KILL(sail_int)(&zghz3353);
  KILL(sail_int)(&zghz3352);
  KILL(sail_int)(&zghz3351);
  KILL(sail_int)(&zghz3350);
  KILL(sail_int)(&zghz3349);
  KILL(sail_int)(&zghz3348);
  KILL(sail_int)(&zghz3347);
  KILL(lbits)(&zghz3346);
  KILL(sail_int)(&zghz3345);
  KILL(sail_int)(&zghz3344);
  KILL(lbits)(&zghz3343);
  KILL(lbits)(&zghz3342);
  KILL(sail_int)(&zghz3341);
  KILL(lbits)(&zghz3340);
  KILL(lbits)(&zghz3339);
  KILL(lbits)(&zghz3338);
  KILL(sail_int)(&zghz3337);
  KILL(lbits)(&zghz3336);
  KILL(sail_int)(&zghz3335);
  KILL(sail_int)(&zghz3334);
  KILL(sail_int)(&zghz3333);
  KILL(sail_int)(&zghz3332);
  KILL(sail_int)(&zghz3331);
  KILL(sail_int)(&zghz3330);
  KILL(sail_int)(&zghz3329);
  KILL(sail_int)(&zghz3328);
  KILL(sail_int)(&zghz3327);
  KILL(sail_int)(&zghz3326);
  KILL(sail_int)(&zghz3325);
  KILL(sail_int)(&zghz3324);
  KILL(lbits)(&zghz3323);
  KILL(sail_int)(&zghz3322);
  KILL(sail_int)(&zghz3321);
  KILL(sail_int)(&zghz3320);
  KILL(sail_int)(&zghz3319);
  KILL(sail_int)(&zghz3318);
  KILL(sail_int)(&zghz3317);
  KILL(lbits)(&zghz3316);
  KILL(sail_int)(&zghz3315);
  KILL(sail_int)(&zghz3314);
  KILL(lbits)(&zghz3313);
  KILL(sail_int)(&zghz3312);
  KILL(lbits)(&zghz3311);
  KILL(sail_int)(&zghz3310);
  KILL(sail_int)(&zghz3309);
  KILL(lbits)(&zghz3308);
  KILL(lbits)(&zghz3307);
  KILL(lbits)(&zghz3306);
  KILL(sail_int)(&zghz3305);
  KILL(sail_int)(&zghz3304);
  KILL(lbits)(&zghz3303);
  KILL(sail_int)(&zghz3302);
  KILL(sail_int)(&zghz3301);
  KILL(lbits)(&zghz3300);
  KILL(sail_int)(&zghz3299);
  KILL(sail_int)(&zghz3298);
  KILL(lbits)(&zghz3297);
  KILL(sail_int)(&zghz3296);
  KILL(sail_int)(&zghz3295);
  KILL(lbits)(&zghz3294);
  KILL(sail_int)(&zghz3293);
  KILL(sail_int)(&zghz3292);
  KILL(lbits)(&zghz3291);
  KILL(sail_int)(&zghz3290);
  KILL(sail_int)(&zghz3289);
  KILL(lbits)(&zghz3288);
  KILL(sail_int)(&zghz3287);
  KILL(sail_int)(&zghz3286);
  KILL(sail_int)(&zghz3285);
  KILL(lbits)(&zghz3284);
  KILL(sail_int)(&zghz3283);
  KILL(sail_int)(&zghz3282);
  KILL(lbits)(&zghz3281);
  KILL(sail_int)(&zghz3280);
  KILL(sail_int)(&zghz3279);
  KILL(sail_int)(&zghz3278);
  KILL(sail_int)(&zghz3277);
  KILL(sail_int)(&zghz3276);
  KILL(lbits)(&zghz3275);
  KILL(sail_int)(&zghz3274);
  KILL(sail_int)(&zghz3273);
  KILL(sail_int)(&zghz3272);
  KILL(sail_int)(&zghz3271);
  KILL(sail_int)(&zghz3270);
  KILL(lbits)(&zghz3269);
  KILL(sail_int)(&zghz3268);
  KILL(sail_int)(&zghz3267);
  KILL(sail_int)(&zghz3266);
  KILL(sail_int)(&zghz3265);
  KILL(sail_int)(&zghz3264);
  KILL(sail_int)(&zghz3263);
  KILL(lbits)(&zghz3262);
  KILL(sail_int)(&zghz3261);
  KILL(lbits)(&zghz3260);
  KILL(sail_int)(&zghz3259);
  KILL(sail_int)(&zghz3258);
  KILL(lbits)(&zghz3257);
  KILL(sail_int)(&zghz3256);
  KILL(lbits)(&zghz3255);
  KILL(lbits)(&zghz3254);
  KILL(lbits)(&zghz3253);
  KILL(sail_int)(&zghz3252);
  KILL(lbits)(&zghz3251);
  KILL(lbits)(&zghz3250);
}

static lbits zCMAX;
static void create_letbind_70(void) {    CREATE(lbits)(&zCMAX);


  lbits zgsz3200;
  CREATE(lbits)(&zgsz3200);
  lbits zgsz3199;
  CREATE(lbits)(&zgsz3199);
  CONVERT_OF(lbits, fbits)(&zgsz3199, UINT64_C(0b1), UINT64_C(1) , true);
  append_64(&zgsz3199, zgsz3199, UINT64_C(0xFFFFC00000010005));
  append_64(&zgsz3199, zgsz3199, UINT64_C(0x0000000000000000));
  COPY(lbits)(&zgsz3200, zgsz3199);
  KILL(lbits)(&zgsz3199);
  COPY(lbits)(&zCMAX, zgsz3200);
  KILL(lbits)(&zgsz3200);
let_end_267: ;
}
static void kill_letbind_70(void) {    KILL(lbits)(&zCMAX);
}

// register TestCap
static lbits zTestCap;

static void sailgen_doCSetBounds(lbits *rop, lbits, lbits);

lbits zghz3435;

static void startup_sailgen_doCSetBounds(void)
{    CREATE(lbits)(&zghz3435);
}

static void sailgen_doCSetBounds(lbits *zcbz335, lbits zinput, lbits zreq_len)
{
  __label__ end_function_269, end_block_exception_270, end_function_277;

  RECREATE(lbits)(&zghz3435);
  sailgen_CapSetBounds(&zghz3435, zinput, zreq_len, false);
  bool zgaz3242;
  zgaz3242 = sailgen_CapIsSealed(zinput);
  if (zgaz3242) {  sailgen_CapWithTagClear((*(&zcbz335)), zghz3435);  } else {  COPY(lbits)((*(&zcbz335)), zghz3435);  }

end_function_269: ;
  goto end_function_277;
end_block_exception_270: ;
  goto end_function_277;
end_function_277: ;
}



static void finish_sailgen_doCSetBounds(void)
{    KILL(lbits)(&zghz3435);
}

static unit sailgen_main(unit);

lbits zghz3436;
lbits zghz3437;
lbits zghz3438;
lbits zghz3439;

static void startup_sailgen_main(void)
{
  CREATE(lbits)(&zghz3436);
  CREATE(lbits)(&zghz3437);
  CREATE(lbits)(&zghz3438);
  CREATE(lbits)(&zghz3439);
}

static unit sailgen_main(unit zgsz3201)
{
  __label__ end_function_272, end_block_exception_273;

  unit zcbz336;
  {
    RECREATE(lbits)(&zghz3439);
    CONVERT_OF(lbits, fbits)(&zghz3439, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3439, zghz3439, UINT64_C(0x0000000000000000));
    append_64(&zghz3439, zghz3439, UINT64_C(0x0000000000000000));
    COPY(lbits)(&zTestCap, zghz3439);
    unit zgsz3203;
    zgsz3203 = UNIT;
  }
  RECREATE(lbits)(&zghz3436);
  COPY(lbits)(&zghz3436, zCMAX);
  uint64_t zbits1;
  zbits1 = sailgen_CapGetValue(zTestCap);
  uint64_t zbits2;
  zbits2 = sailgen_CapGetPermissions(zTestCap);
  uint64_t zbits3;
  zbits3 = sailgen_CapGetObjectType(zTestCap);
  uint64_t zbits4;
  zbits4 = sailgen_CapGetTag(zTestCap);
  RECREATE(lbits)(&zghz3437);
  {
    RECREATE(lbits)(&zghz3438);
    CONVERT_OF(lbits, fbits)(&zghz3438, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3438, zghz3438, UINT64_C(0x0000000000000000));
    sailgen_doCSetBounds(&zghz3437, zTestCap, zghz3438);
  }
  struct ztuple_z8z5bvzCz0z5bvzCz0z5boolz9 zbits6;
  CREATE(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zbits6);
  sailgen_CapGetBounds(&zbits6, zTestCap);
  int64_t zexp;
  zexp = sailgen_CapGetExponent(zTestCap);
  bool zrep;
  zrep = sailgen_CapIsRepresentableFast(zTestCap, UINT64_C(0x0000000000000000));
  zcbz336 = UNIT;


  KILL(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zbits6);




end_function_272: ;
  return zcbz336;
end_block_exception_273: ;

  return UNIT;
}



static void finish_sailgen_main(void)
{
  KILL(lbits)(&zghz3439);
  KILL(lbits)(&zghz3438);
  KILL(lbits)(&zghz3437);
  KILL(lbits)(&zghz3436);
}

static unit sailgen_initializze_registers(unit);

lbits zghz3440;
lbits zghz3441;
lbits zghz3442;
lbits zghz3443;
lbits zghz3444;
lbits zghz3445;
lbits zghz3446;
lbits zghz3447;
lbits zghz3448;
lbits zghz3449;
lbits zghz3450;
lbits zghz3451;
lbits zghz3452;
lbits zghz3453;
lbits zghz3454;
lbits zghz3455;
lbits zghz3456;
lbits zghz3457;
lbits zghz3458;
lbits zghz3459;
lbits zghz3460;
lbits zghz3461;
lbits zghz3462;
lbits zghz3463;
lbits zghz3464;
lbits zghz3465;
lbits zghz3466;
lbits zghz3467;
lbits zghz3468;
lbits zghz3469;
lbits zghz3470;
lbits zghz3471;
lbits zghz3472;
lbits zghz3473;
lbits zghz3474;
lbits zghz3475;
lbits zghz3476;
lbits zghz3477;
lbits zghz3478;
lbits zghz3479;
lbits zghz3480;
lbits zghz3481;
lbits zghz3482;
lbits zghz3483;
lbits zghz3484;
lbits zghz3485;
lbits zghz3486;
lbits zghz3487;
lbits zghz3488;
lbits zghz3489;
lbits zghz3490;
lbits zghz3491;
lbits zghz3492;
lbits zghz3493;
lbits zghz3494;
lbits zghz3495;
lbits zghz3496;
lbits zghz3497;
lbits zghz3498;
lbits zghz3499;
lbits zghz3500;
lbits zghz3501;
lbits zghz3502;
lbits zghz3503;
lbits zghz3504;
lbits zghz3505;
lbits zghz3506;
lbits zghz3507;
lbits zghz3508;
lbits zghz3509;
lbits zghz3510;
lbits zghz3511;
lbits zghz3512;
lbits zghz3513;
lbits zghz3514;
lbits zghz3515;
lbits zghz3516;
lbits zghz3517;
lbits zghz3518;
lbits zghz3519;
lbits zghz3520;
lbits zghz3521;
lbits zghz3522;
lbits zghz3523;
lbits zghz3524;
lbits zghz3525;
lbits zghz3526;
lbits zghz3527;
lbits zghz3528;
struct zProcState zghz3529;
lbits zghz3530;

static void startup_sailgen_initializze_registers(void)
{
  CREATE(lbits)(&zghz3440);
  CREATE(lbits)(&zghz3441);
  CREATE(lbits)(&zghz3442);
  CREATE(lbits)(&zghz3443);
  CREATE(lbits)(&zghz3444);
  CREATE(lbits)(&zghz3445);
  CREATE(lbits)(&zghz3446);
  CREATE(lbits)(&zghz3447);
  CREATE(lbits)(&zghz3448);
  CREATE(lbits)(&zghz3449);
  CREATE(lbits)(&zghz3450);
  CREATE(lbits)(&zghz3451);
  CREATE(lbits)(&zghz3452);
  CREATE(lbits)(&zghz3453);
  CREATE(lbits)(&zghz3454);
  CREATE(lbits)(&zghz3455);
  CREATE(lbits)(&zghz3456);
  CREATE(lbits)(&zghz3457);
  CREATE(lbits)(&zghz3458);
  CREATE(lbits)(&zghz3459);
  CREATE(lbits)(&zghz3460);
  CREATE(lbits)(&zghz3461);
  CREATE(lbits)(&zghz3462);
  CREATE(lbits)(&zghz3463);
  CREATE(lbits)(&zghz3464);
  CREATE(lbits)(&zghz3465);
  CREATE(lbits)(&zghz3466);
  CREATE(lbits)(&zghz3467);
  CREATE(lbits)(&zghz3468);
  CREATE(lbits)(&zghz3469);
  CREATE(lbits)(&zghz3470);
  CREATE(lbits)(&zghz3471);
  CREATE(lbits)(&zghz3472);
  CREATE(lbits)(&zghz3473);
  CREATE(lbits)(&zghz3474);
  CREATE(lbits)(&zghz3475);
  CREATE(lbits)(&zghz3476);
  CREATE(lbits)(&zghz3477);
  CREATE(lbits)(&zghz3478);
  CREATE(lbits)(&zghz3479);
  CREATE(lbits)(&zghz3480);
  CREATE(lbits)(&zghz3481);
  CREATE(lbits)(&zghz3482);
  CREATE(lbits)(&zghz3483);
  CREATE(lbits)(&zghz3484);
  CREATE(lbits)(&zghz3485);
  CREATE(lbits)(&zghz3486);
  CREATE(lbits)(&zghz3487);
  CREATE(lbits)(&zghz3488);
  CREATE(lbits)(&zghz3489);
  CREATE(lbits)(&zghz3490);
  CREATE(lbits)(&zghz3491);
  CREATE(lbits)(&zghz3492);
  CREATE(lbits)(&zghz3493);
  CREATE(lbits)(&zghz3494);
  CREATE(lbits)(&zghz3495);
  CREATE(lbits)(&zghz3496);
  CREATE(lbits)(&zghz3497);
  CREATE(lbits)(&zghz3498);
  CREATE(lbits)(&zghz3499);
  CREATE(lbits)(&zghz3500);
  CREATE(lbits)(&zghz3501);
  CREATE(lbits)(&zghz3502);
  CREATE(lbits)(&zghz3503);
  CREATE(lbits)(&zghz3504);
  CREATE(lbits)(&zghz3505);
  CREATE(lbits)(&zghz3506);
  CREATE(lbits)(&zghz3507);
  CREATE(lbits)(&zghz3508);
  CREATE(lbits)(&zghz3509);
  CREATE(lbits)(&zghz3510);
  CREATE(lbits)(&zghz3511);
  CREATE(lbits)(&zghz3512);
  CREATE(lbits)(&zghz3513);
  CREATE(lbits)(&zghz3514);
  CREATE(lbits)(&zghz3515);
  CREATE(lbits)(&zghz3516);
  CREATE(lbits)(&zghz3517);
  CREATE(lbits)(&zghz3518);
  CREATE(lbits)(&zghz3519);
  CREATE(lbits)(&zghz3520);
  CREATE(lbits)(&zghz3521);
  CREATE(lbits)(&zghz3522);
  CREATE(lbits)(&zghz3523);
  CREATE(lbits)(&zghz3524);
  CREATE(lbits)(&zghz3525);
  CREATE(lbits)(&zghz3526);
  CREATE(lbits)(&zghz3527);
  CREATE(lbits)(&zghz3528);

  CREATE(lbits)(&zghz3530);
}








static unit sailgen_initializze_registers(unit zgsz3205)
{
  __label__ end_function_275, end_block_exception_276;

  unit zcbz337;
  {
    CONVERT_OF(sail_int, mach_int)(&zSEE, INT64_C(0));
    unit zgsz3666;
    zgsz3666 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3530);
    CONVERT_OF(lbits, fbits)(&zghz3530, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3530, zghz3530, UINT64_C(0x0000000000000000));
    append_64(&zghz3530, zghz3530, UINT64_C(0x0000000000000000));
    COPY(lbits)(&zPCC, zghz3530);
    unit zgsz3665;
    zgsz3665 = UNIT;
  }
  {
    zCNTHPS_CTL_EL2 = UINT64_C(0x00000000);
    unit zgsz3664;
    zgsz3664 = UNIT;
  }
  {
    zCNTHPS_CVAL_EL2 = UINT64_C(0x0000000000000000);
    unit zgsz3663;
    zgsz3663 = UNIT;
  }
  {
    zCNTHPS_TVAL_EL2 = UINT64_C(0x00000000);
    unit zgsz3662;
    zgsz3662 = UNIT;
  }
  {
    zCNTHVS_CTL_EL2 = UINT64_C(0x00000000);
    unit zgsz3661;
    zgsz3661 = UNIT;
  }
  {
    zCNTHVS_CVAL_EL2 = UINT64_C(0x0000000000000000);
    unit zgsz3660;
    zgsz3660 = UNIT;
  }
  {
    zCNTHVS_TVAL_EL2 = UINT64_C(0x00000000);
    unit zgsz3659;
    zgsz3659 = UNIT;
  }
  {
    zSCR_EL3 = UINT64_C(0x00000000);
    unit zgsz3658;
    zgsz3658 = UNIT;
  }
  {
    zHCR_EL2 = UINT64_C(0x0000000000000000);
    unit zgsz3657;
    zgsz3657 = UNIT;
  }
  {
    struct zProcState zghz3529;
    zghz3529.zA = UINT64_C(0b0);
    zghz3529.zC = UINT64_C(0b0);
    zghz3529.zC64 = UINT64_C(0b0);
    zghz3529.zD = UINT64_C(0b0);
    zghz3529.zDIT = UINT64_C(0b0);
    zghz3529.zE = UINT64_C(0b0);
    zghz3529.zEL = UINT64_C(0b00);
    zghz3529.zF = UINT64_C(0b0);
    zghz3529.zGE = UINT64_C(0x0);
    zghz3529.zI = UINT64_C(0b0);
    zghz3529.zIL = UINT64_C(0b0);
    zghz3529.zIT = UINT64_C(0x00);
    zghz3529.zJ = UINT64_C(0b0);
    zghz3529.zM = UINT64_C(0b00000);
    zghz3529.zN = UINT64_C(0b0);
    zghz3529.zPAN = UINT64_C(0b0);
    zghz3529.zQ = UINT64_C(0b0);
    zghz3529.zSP = UINT64_C(0b0);
    zghz3529.zSS = UINT64_C(0b0);
    zghz3529.zSSBS = UINT64_C(0b0);
    zghz3529.zT = UINT64_C(0b0);
    zghz3529.zTCO = UINT64_C(0b0);
    zghz3529.zUAO = UINT64_C(0b0);
    zghz3529.zV = UINT64_C(0b0);
    zghz3529.zZ = UINT64_C(0b0);
    zghz3529.znRW = UINT64_C(0b0);
    zPSTATE = zghz3529;
    unit zgsz3656;
    zgsz3656 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3528);
    CONVERT_OF(lbits, fbits)(&zghz3528, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3528, zghz3528, UINT64_C(0x0000000000000000));
    append_64(&zghz3528, zghz3528, UINT64_C(0x0000000000000000));
    COPY(lbits)(&z_R00, zghz3528);
    unit zgsz3655;
    zgsz3655 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3527);
    CONVERT_OF(lbits, fbits)(&zghz3527, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3527, zghz3527, UINT64_C(0x0000000000000000));
    append_64(&zghz3527, zghz3527, UINT64_C(0x0000000000000000));
    COPY(lbits)(&z_R01, zghz3527);
    unit zgsz3654;
    zgsz3654 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3526);
    CONVERT_OF(lbits, fbits)(&zghz3526, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3526, zghz3526, UINT64_C(0x0000000000000000));
    append_64(&zghz3526, zghz3526, UINT64_C(0x0000000000000000));
    COPY(lbits)(&z_R02, zghz3526);
    unit zgsz3653;
    zgsz3653 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3525);
    CONVERT_OF(lbits, fbits)(&zghz3525, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3525, zghz3525, UINT64_C(0x0000000000000000));
    append_64(&zghz3525, zghz3525, UINT64_C(0x0000000000000000));
    COPY(lbits)(&z_R03, zghz3525);
    unit zgsz3652;
    zgsz3652 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3524);
    CONVERT_OF(lbits, fbits)(&zghz3524, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3524, zghz3524, UINT64_C(0x0000000000000000));
    append_64(&zghz3524, zghz3524, UINT64_C(0x0000000000000000));
    COPY(lbits)(&z_R04, zghz3524);
    unit zgsz3651;
    zgsz3651 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3523);
    CONVERT_OF(lbits, fbits)(&zghz3523, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3523, zghz3523, UINT64_C(0x0000000000000000));
    append_64(&zghz3523, zghz3523, UINT64_C(0x0000000000000000));
    COPY(lbits)(&z_R05, zghz3523);
    unit zgsz3650;
    zgsz3650 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3522);
    CONVERT_OF(lbits, fbits)(&zghz3522, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3522, zghz3522, UINT64_C(0x0000000000000000));
    append_64(&zghz3522, zghz3522, UINT64_C(0x0000000000000000));
    COPY(lbits)(&z_R06, zghz3522);
    unit zgsz3649;
    zgsz3649 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3521);
    CONVERT_OF(lbits, fbits)(&zghz3521, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3521, zghz3521, UINT64_C(0x0000000000000000));
    append_64(&zghz3521, zghz3521, UINT64_C(0x0000000000000000));
    COPY(lbits)(&z_R07, zghz3521);
    unit zgsz3648;
    zgsz3648 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3520);
    CONVERT_OF(lbits, fbits)(&zghz3520, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3520, zghz3520, UINT64_C(0x0000000000000000));
    append_64(&zghz3520, zghz3520, UINT64_C(0x0000000000000000));
    COPY(lbits)(&z_R08, zghz3520);
    unit zgsz3647;
    zgsz3647 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3519);
    CONVERT_OF(lbits, fbits)(&zghz3519, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3519, zghz3519, UINT64_C(0x0000000000000000));
    append_64(&zghz3519, zghz3519, UINT64_C(0x0000000000000000));
    COPY(lbits)(&z_R09, zghz3519);
    unit zgsz3646;
    zgsz3646 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3518);
    CONVERT_OF(lbits, fbits)(&zghz3518, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3518, zghz3518, UINT64_C(0x0000000000000000));
    append_64(&zghz3518, zghz3518, UINT64_C(0x0000000000000000));
    COPY(lbits)(&z_R10, zghz3518);
    unit zgsz3645;
    zgsz3645 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3517);
    CONVERT_OF(lbits, fbits)(&zghz3517, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3517, zghz3517, UINT64_C(0x0000000000000000));
    append_64(&zghz3517, zghz3517, UINT64_C(0x0000000000000000));
    COPY(lbits)(&z_R11, zghz3517);
    unit zgsz3644;
    zgsz3644 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3516);
    CONVERT_OF(lbits, fbits)(&zghz3516, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3516, zghz3516, UINT64_C(0x0000000000000000));
    append_64(&zghz3516, zghz3516, UINT64_C(0x0000000000000000));
    COPY(lbits)(&z_R12, zghz3516);
    unit zgsz3643;
    zgsz3643 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3515);
    CONVERT_OF(lbits, fbits)(&zghz3515, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3515, zghz3515, UINT64_C(0x0000000000000000));
    append_64(&zghz3515, zghz3515, UINT64_C(0x0000000000000000));
    COPY(lbits)(&z_R13, zghz3515);
    unit zgsz3642;
    zgsz3642 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3514);
    CONVERT_OF(lbits, fbits)(&zghz3514, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3514, zghz3514, UINT64_C(0x0000000000000000));
    append_64(&zghz3514, zghz3514, UINT64_C(0x0000000000000000));
    COPY(lbits)(&z_R14, zghz3514);
    unit zgsz3641;
    zgsz3641 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3513);
    CONVERT_OF(lbits, fbits)(&zghz3513, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3513, zghz3513, UINT64_C(0x0000000000000000));
    append_64(&zghz3513, zghz3513, UINT64_C(0x0000000000000000));
    COPY(lbits)(&z_R15, zghz3513);
    unit zgsz3640;
    zgsz3640 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3512);
    CONVERT_OF(lbits, fbits)(&zghz3512, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3512, zghz3512, UINT64_C(0x0000000000000000));
    append_64(&zghz3512, zghz3512, UINT64_C(0x0000000000000000));
    COPY(lbits)(&z_R16, zghz3512);
    unit zgsz3639;
    zgsz3639 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3511);
    CONVERT_OF(lbits, fbits)(&zghz3511, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3511, zghz3511, UINT64_C(0x0000000000000000));
    append_64(&zghz3511, zghz3511, UINT64_C(0x0000000000000000));
    COPY(lbits)(&z_R17, zghz3511);
    unit zgsz3638;
    zgsz3638 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3510);
    CONVERT_OF(lbits, fbits)(&zghz3510, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3510, zghz3510, UINT64_C(0x0000000000000000));
    append_64(&zghz3510, zghz3510, UINT64_C(0x0000000000000000));
    COPY(lbits)(&z_R18, zghz3510);
    unit zgsz3637;
    zgsz3637 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3509);
    CONVERT_OF(lbits, fbits)(&zghz3509, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3509, zghz3509, UINT64_C(0x0000000000000000));
    append_64(&zghz3509, zghz3509, UINT64_C(0x0000000000000000));
    COPY(lbits)(&z_R19, zghz3509);
    unit zgsz3636;
    zgsz3636 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3508);
    CONVERT_OF(lbits, fbits)(&zghz3508, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3508, zghz3508, UINT64_C(0x0000000000000000));
    append_64(&zghz3508, zghz3508, UINT64_C(0x0000000000000000));
    COPY(lbits)(&z_R20, zghz3508);
    unit zgsz3635;
    zgsz3635 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3507);
    CONVERT_OF(lbits, fbits)(&zghz3507, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3507, zghz3507, UINT64_C(0x0000000000000000));
    append_64(&zghz3507, zghz3507, UINT64_C(0x0000000000000000));
    COPY(lbits)(&z_R21, zghz3507);
    unit zgsz3634;
    zgsz3634 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3506);
    CONVERT_OF(lbits, fbits)(&zghz3506, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3506, zghz3506, UINT64_C(0x0000000000000000));
    append_64(&zghz3506, zghz3506, UINT64_C(0x0000000000000000));
    COPY(lbits)(&z_R22, zghz3506);
    unit zgsz3633;
    zgsz3633 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3505);
    CONVERT_OF(lbits, fbits)(&zghz3505, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3505, zghz3505, UINT64_C(0x0000000000000000));
    append_64(&zghz3505, zghz3505, UINT64_C(0x0000000000000000));
    COPY(lbits)(&z_R23, zghz3505);
    unit zgsz3632;
    zgsz3632 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3504);
    CONVERT_OF(lbits, fbits)(&zghz3504, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3504, zghz3504, UINT64_C(0x0000000000000000));
    append_64(&zghz3504, zghz3504, UINT64_C(0x0000000000000000));
    COPY(lbits)(&z_R24, zghz3504);
    unit zgsz3631;
    zgsz3631 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3503);
    CONVERT_OF(lbits, fbits)(&zghz3503, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3503, zghz3503, UINT64_C(0x0000000000000000));
    append_64(&zghz3503, zghz3503, UINT64_C(0x0000000000000000));
    COPY(lbits)(&z_R25, zghz3503);
    unit zgsz3630;
    zgsz3630 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3502);
    CONVERT_OF(lbits, fbits)(&zghz3502, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3502, zghz3502, UINT64_C(0x0000000000000000));
    append_64(&zghz3502, zghz3502, UINT64_C(0x0000000000000000));
    COPY(lbits)(&z_R26, zghz3502);
    unit zgsz3629;
    zgsz3629 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3501);
    CONVERT_OF(lbits, fbits)(&zghz3501, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3501, zghz3501, UINT64_C(0x0000000000000000));
    append_64(&zghz3501, zghz3501, UINT64_C(0x0000000000000000));
    COPY(lbits)(&z_R27, zghz3501);
    unit zgsz3628;
    zgsz3628 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3500);
    CONVERT_OF(lbits, fbits)(&zghz3500, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3500, zghz3500, UINT64_C(0x0000000000000000));
    append_64(&zghz3500, zghz3500, UINT64_C(0x0000000000000000));
    COPY(lbits)(&z_R28, zghz3500);
    unit zgsz3627;
    zgsz3627 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3499);
    CONVERT_OF(lbits, fbits)(&zghz3499, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3499, zghz3499, UINT64_C(0x0000000000000000));
    append_64(&zghz3499, zghz3499, UINT64_C(0x0000000000000000));
    COPY(lbits)(&z_R29, zghz3499);
    unit zgsz3626;
    zgsz3626 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3498);
    CONVERT_OF(lbits, fbits)(&zghz3498, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3498, zghz3498, UINT64_C(0x0000000000000000));
    append_64(&zghz3498, zghz3498, UINT64_C(0x0000000000000000));
    COPY(lbits)(&z_R30, zghz3498);
    unit zgsz3625;
    zgsz3625 = UNIT;
  }
  {
    zESR_EL1 = UINT64_C(0x00000000);
    unit zgsz3624;
    zgsz3624 = UNIT;
  }
  {
    zESR_EL2 = UINT64_C(0x00000000);
    unit zgsz3623;
    zgsz3623 = UNIT;
  }
  {
    zESR_EL3 = UINT64_C(0x00000000);
    unit zgsz3622;
    zgsz3622 = UNIT;
  }
  {
    zFAR_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3621;
    zgsz3621 = UNIT;
  }
  {
    zFAR_EL2 = UINT64_C(0x0000000000000000);
    unit zgsz3620;
    zgsz3620 = UNIT;
  }
  {
    zFAR_EL3 = UINT64_C(0x0000000000000000);
    unit zgsz3619;
    zgsz3619 = UNIT;
  }
  {
    zHPFAR_EL2 = UINT64_C(0x0000000000000000);
    unit zgsz3618;
    zgsz3618 = UNIT;
  }
  {
    zTCR_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3617;
    zgsz3617 = UNIT;
  }
  {
    zTCR_EL2 = UINT64_C(0x0000000000000000);
    unit zgsz3616;
    zgsz3616 = UNIT;
  }
  {
    zTCR_EL3 = UINT64_C(0x00000000);
    unit zgsz3615;
    zgsz3615 = UNIT;
  }
  {
    z_PC = UINT64_C(0x0000000000000000);
    unit zgsz3614;
    zgsz3614 = UNIT;
  }
  {
    z__BranchTaken = false;
    unit zgsz3613;
    zgsz3613 = UNIT;
  }
  {
    zCCTLR_EL0 = UINT64_C(0x00000000);
    unit zgsz3612;
    zgsz3612 = UNIT;
  }
  {
    zCCTLR_EL1 = UINT64_C(0x00000000);
    unit zgsz3611;
    zgsz3611 = UNIT;
  }
  {
    zCCTLR_EL2 = UINT64_C(0x00000000);
    unit zgsz3610;
    zgsz3610 = UNIT;
  }
  {
    zCCTLR_EL3 = UINT64_C(0x00000000);
    unit zgsz3609;
    zgsz3609 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3497);
    CONVERT_OF(lbits, fbits)(&zghz3497, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3497, zghz3497, UINT64_C(0x0000000000000000));
    append_64(&zghz3497, zghz3497, UINT64_C(0x0000000000000000));
    COPY(lbits)(&zELR_EL1, zghz3497);
    unit zgsz3608;
    zgsz3608 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3496);
    CONVERT_OF(lbits, fbits)(&zghz3496, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3496, zghz3496, UINT64_C(0x0000000000000000));
    append_64(&zghz3496, zghz3496, UINT64_C(0x0000000000000000));
    COPY(lbits)(&zELR_EL2, zghz3496);
    unit zgsz3607;
    zgsz3607 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3495);
    CONVERT_OF(lbits, fbits)(&zghz3495, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3495, zghz3495, UINT64_C(0x0000000000000000));
    append_64(&zghz3495, zghz3495, UINT64_C(0x0000000000000000));
    COPY(lbits)(&zELR_EL3, zghz3495);
    unit zgsz3606;
    zgsz3606 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3494);
    CONVERT_OF(lbits, fbits)(&zghz3494, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3494, zghz3494, UINT64_C(0x0000000000000000));
    append_64(&zghz3494, zghz3494, UINT64_C(0x0000000000000000));
    COPY(lbits)(&zVBAR_EL1, zghz3494);
    unit zgsz3605;
    zgsz3605 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3493);
    CONVERT_OF(lbits, fbits)(&zghz3493, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3493, zghz3493, UINT64_C(0x0000000000000000));
    append_64(&zghz3493, zghz3493, UINT64_C(0x0000000000000000));
    COPY(lbits)(&zVBAR_EL2, zghz3493);
    unit zgsz3604;
    zgsz3604 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3492);
    CONVERT_OF(lbits, fbits)(&zghz3492, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3492, zghz3492, UINT64_C(0x0000000000000000));
    append_64(&zghz3492, zghz3492, UINT64_C(0x0000000000000000));
    COPY(lbits)(&zVBAR_EL3, zghz3492);
    unit zgsz3603;
    zgsz3603 = UNIT;
  }
  {
    zCPACR_EL1 = UINT64_C(0x00000000);
    unit zgsz3602;
    zgsz3602 = UNIT;
  }
  {
    zCPTR_EL2 = UINT64_C(0x00000000);
    unit zgsz3601;
    zgsz3601 = UNIT;
  }
  {
    zCPTR_EL3 = UINT64_C(0x00000000);
    unit zgsz3600;
    zgsz3600 = UNIT;
  }
  {
    zEDSCR = UINT64_C(0x00000000);
    unit zgsz3599;
    zgsz3599 = UNIT;
  }
  {
    zSCTLR_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3598;
    zgsz3598 = UNIT;
  }
  {
    zSCTLR_EL2 = UINT64_C(0x0000000000000000);
    unit zgsz3597;
    zgsz3597 = UNIT;
  }
  {
    zSCTLR_EL3 = UINT64_C(0x0000000000000000);
    unit zgsz3596;
    zgsz3596 = UNIT;
  }
  {
    zSPSR_EL1 = UINT64_C(0x00000000);
    unit zgsz3595;
    zgsz3595 = UNIT;
  }
  {
    zSPSR_EL2 = UINT64_C(0x00000000);
    unit zgsz3594;
    zgsz3594 = UNIT;
  }
  {
    zSPSR_EL3 = UINT64_C(0x00000000);
    unit zgsz3593;
    zgsz3593 = UNIT;
  }
  {
    zACTLR_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3592;
    zgsz3592 = UNIT;
  }
  {
    zACTLR_EL2 = UINT64_C(0x0000000000000000);
    unit zgsz3591;
    zgsz3591 = UNIT;
  }
  {
    zACTLR_EL3 = UINT64_C(0x0000000000000000);
    unit zgsz3590;
    zgsz3590 = UNIT;
  }
  {
    zAFSR0_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3589;
    zgsz3589 = UNIT;
  }
  {
    zAFSR0_EL2 = UINT64_C(0x0000000000000000);
    unit zgsz3588;
    zgsz3588 = UNIT;
  }
  {
    zAFSR0_EL3 = UINT64_C(0x0000000000000000);
    unit zgsz3587;
    zgsz3587 = UNIT;
  }
  {
    zAFSR1_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3586;
    zgsz3586 = UNIT;
  }
  {
    zAFSR1_EL2 = UINT64_C(0x0000000000000000);
    unit zgsz3585;
    zgsz3585 = UNIT;
  }
  {
    zAFSR1_EL3 = UINT64_C(0x0000000000000000);
    unit zgsz3584;
    zgsz3584 = UNIT;
  }
  {
    zAIDR_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3583;
    zgsz3583 = UNIT;
  }
  {
    zAMAIR_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3582;
    zgsz3582 = UNIT;
  }
  {
    zAMAIR_EL2 = UINT64_C(0x0000000000000000);
    unit zgsz3581;
    zgsz3581 = UNIT;
  }
  {
    zAMAIR_EL3 = UINT64_C(0x0000000000000000);
    unit zgsz3580;
    zgsz3580 = UNIT;
  }
  {
    zCCSIDR_EL1 = UINT64_C(0x00000000);
    unit zgsz3579;
    zgsz3579 = UNIT;
  }
  {
    zCHCR_EL2 = UINT64_C(0x00000000);
    unit zgsz3578;
    zgsz3578 = UNIT;
  }
  {
    zCLIDR_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3577;
    zgsz3577 = UNIT;
  }
  {
    zCNTFRQ_EL0 = UINT64_C(0x00000000);
    unit zgsz3576;
    zgsz3576 = UNIT;
  }
  {
    zCNTHCTL_EL2 = UINT64_C(0x00000000);
    unit zgsz3575;
    zgsz3575 = UNIT;
  }
  {
    zCNTKCTL_EL1 = UINT64_C(0x00000000);
    unit zgsz3574;
    zgsz3574 = UNIT;
  }
  {
    zCNTHP_CTL_EL2 = UINT64_C(0x00000000);
    unit zgsz3573;
    zgsz3573 = UNIT;
  }
  {
    zCNTHP_CVAL_EL2 = UINT64_C(0x0000000000000000);
    unit zgsz3572;
    zgsz3572 = UNIT;
  }
  {
    zCNTHP_TVAL_EL2 = UINT64_C(0x00000000);
    unit zgsz3571;
    zgsz3571 = UNIT;
  }
  {
    zCNTHV_CTL_EL2 = UINT64_C(0x00000000);
    unit zgsz3570;
    zgsz3570 = UNIT;
  }
  {
    zCNTHV_CVAL_EL2 = UINT64_C(0x0000000000000000);
    unit zgsz3569;
    zgsz3569 = UNIT;
  }
  {
    zCNTHV_TVAL_EL2 = UINT64_C(0x00000000);
    unit zgsz3568;
    zgsz3568 = UNIT;
  }
  {
    zCNTPCT_EL0 = UINT64_C(0x0000000000000000);
    unit zgsz3567;
    zgsz3567 = UNIT;
  }
  {
    zCNTPS_CTL_EL1 = UINT64_C(0x00000000);
    unit zgsz3566;
    zgsz3566 = UNIT;
  }
  {
    zCNTPS_CVAL_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3565;
    zgsz3565 = UNIT;
  }
  {
    zCNTPS_TVAL_EL1 = UINT64_C(0x00000000);
    unit zgsz3564;
    zgsz3564 = UNIT;
  }
  {
    zCNTP_CTL_EL0 = UINT64_C(0x00000000);
    unit zgsz3563;
    zgsz3563 = UNIT;
  }
  {
    zCNTP_CVAL_EL0 = UINT64_C(0x0000000000000000);
    unit zgsz3562;
    zgsz3562 = UNIT;
  }
  {
    zCNTP_TVAL_EL0 = UINT64_C(0x00000000);
    unit zgsz3561;
    zgsz3561 = UNIT;
  }
  {
    zCNTVCT_EL0 = UINT64_C(0x0000000000000000);
    unit zgsz3560;
    zgsz3560 = UNIT;
  }
  {
    zCNTVOFF_EL2 = UINT64_C(0x0000000000000000);
    unit zgsz3559;
    zgsz3559 = UNIT;
  }
  {
    zCNTV_CTL_EL0 = UINT64_C(0x00000000);
    unit zgsz3558;
    zgsz3558 = UNIT;
  }
  {
    zCNTV_CVAL_EL0 = UINT64_C(0x0000000000000000);
    unit zgsz3557;
    zgsz3557 = UNIT;
  }
  {
    zCNTV_TVAL_EL0 = UINT64_C(0x00000000);
    unit zgsz3556;
    zgsz3556 = UNIT;
  }
  {
    zCONTEXTIDR_EL1 = UINT64_C(0x00000000);
    unit zgsz3555;
    zgsz3555 = UNIT;
  }
  {
    zCONTEXTIDR_EL2 = UINT64_C(0x00000000);
    unit zgsz3554;
    zgsz3554 = UNIT;
  }
  {
    zCSCR_EL3 = UINT64_C(0x00000000);
    unit zgsz3553;
    zgsz3553 = UNIT;
  }
  {
    zCSSELR_EL1 = UINT64_C(0x00000000);
    unit zgsz3552;
    zgsz3552 = UNIT;
  }
  {
    zCTR_EL0 = UINT64_C(0x00000000);
    unit zgsz3551;
    zgsz3551 = UNIT;
  }
  {
    zDACR32_EL2 = UINT64_C(0x00000000);
    unit zgsz3550;
    zgsz3550 = UNIT;
  }
  {
    zDBGAUTHSTATUS_EL1 = UINT64_C(0x00000000);
    unit zgsz3549;
    zgsz3549 = UNIT;
  }
  {
    zMDCR_EL2 = UINT64_C(0x00000000);
    unit zgsz3548;
    zgsz3548 = UNIT;
  }
  {
    zMDCR_EL3 = UINT64_C(0x00000000);
    unit zgsz3547;
    zgsz3547 = UNIT;
  }
  {
    zz5vecz8z5bv32z9 zgsz3245;
    CREATE(zz5vecz8z5bv32z9)(&zgsz3245);
    internal_vector_init_zz5vecz8z5bv32z9(&zgsz3245, INT64_C(16));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3245, zgsz3245, INT64_C(0), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3245, zgsz3245, INT64_C(1), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3245, zgsz3245, INT64_C(2), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3245, zgsz3245, INT64_C(3), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3245, zgsz3245, INT64_C(4), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3245, zgsz3245, INT64_C(5), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3245, zgsz3245, INT64_C(6), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3245, zgsz3245, INT64_C(7), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3245, zgsz3245, INT64_C(8), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3245, zgsz3245, INT64_C(9), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3245, zgsz3245, INT64_C(10), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3245, zgsz3245, INT64_C(11), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3245, zgsz3245, INT64_C(12), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3245, zgsz3245, INT64_C(13), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3245, zgsz3245, INT64_C(14), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3245, zgsz3245, INT64_C(15), UINT64_C(0x00000000));
    KILL(zz5vecz8z5bv32z9)(&zDBGBCR_EL1);
    internal_vector_init_zz5vecz8z5bv32z9(&zDBGBCR_EL1, zgsz3245.len);
    for (int zcbz339 = 0; zcbz339 < zgsz3245.len; zcbz339++) {
      uint64_t zcbz340 = zgsz3245.data[zcbz339];
      uint64_t zcbz341;
      zcbz341 = zcbz340;
      zDBGBCR_EL1.data[zcbz339] = zcbz341;
    }
    unit zgsz3546;
    zgsz3546 = UNIT;
    KILL(zz5vecz8z5bv32z9)(&zgsz3245);
  }
  {
    RECREATE(lbits)(&zghz3491);
    CONVERT_OF(lbits, fbits)(&zghz3491, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3491, zghz3491, UINT64_C(0x0000000000000000));
    append_64(&zghz3491, zghz3491, UINT64_C(0x0000000000000000));
    COPY(lbits)(&zCDLR_EL0, zghz3491);
    unit zgsz3545;
    zgsz3545 = UNIT;
  }
  {
    zDSPSR_EL0 = UINT64_C(0x00000000);
    unit zgsz3544;
    zgsz3544 = UNIT;
  }
  {
    zDBGEN = sailgen_undefined_signal(UNIT);
    unit zgsz3543;
    zgsz3543 = UNIT;
  }
  {
    zSPIDEN = sailgen_undefined_signal(UNIT);
    unit zgsz3542;
    zgsz3542 = UNIT;
  }
  {
    zDBGPRCR_EL1 = UINT64_C(0x00000000);
    unit zgsz3541;
    zgsz3541 = UNIT;
  }
  {
    zOSDLR_EL1 = UINT64_C(0x00000000);
    unit zgsz3540;
    zgsz3540 = UNIT;
  }
  {
    zOSLSR_EL1 = UINT64_C(0x00000000);
    unit zgsz3539;
    zgsz3539 = UNIT;
  }
  {
    zz5vecz8z5bv64z9 zgsz3247;
    CREATE(zz5vecz8z5bv64z9)(&zgsz3247);
    internal_vector_init_zz5vecz8z5bv64z9(&zgsz3247, INT64_C(16));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3247, zgsz3247, INT64_C(0), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3247, zgsz3247, INT64_C(1), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3247, zgsz3247, INT64_C(2), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3247, zgsz3247, INT64_C(3), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3247, zgsz3247, INT64_C(4), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3247, zgsz3247, INT64_C(5), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3247, zgsz3247, INT64_C(6), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3247, zgsz3247, INT64_C(7), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3247, zgsz3247, INT64_C(8), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3247, zgsz3247, INT64_C(9), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3247, zgsz3247, INT64_C(10), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3247, zgsz3247, INT64_C(11), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3247, zgsz3247, INT64_C(12), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3247, zgsz3247, INT64_C(13), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3247, zgsz3247, INT64_C(14), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3247, zgsz3247, INT64_C(15), UINT64_C(0x0000000000000000));
    KILL(zz5vecz8z5bv64z9)(&zDBGBVR_EL1);
    internal_vector_init_zz5vecz8z5bv64z9(&zDBGBVR_EL1, zgsz3247.len);
    for (int zcbz342 = 0; zcbz342 < zgsz3247.len; zcbz342++) {
      uint64_t zcbz343 = zgsz3247.data[zcbz342];
      uint64_t zcbz344;
      zcbz344 = zcbz343;
      zDBGBVR_EL1.data[zcbz342] = zcbz344;
    }
    unit zgsz3538;
    zgsz3538 = UNIT;
    KILL(zz5vecz8z5bv64z9)(&zgsz3247);
  }
  {
    zDBGCLAIMCLR_EL1 = UINT64_C(0x00000000);
    unit zgsz3537;
    zgsz3537 = UNIT;
  }
  {
    zDBGCLAIMSET_EL1 = UINT64_C(0x00000000);
    unit zgsz3536;
    zgsz3536 = UNIT;
  }
  {
    zDBGDTRRX_EL0 = UINT64_C(0x00000000);
    unit zgsz3535;
    zgsz3535 = UNIT;
  }
  {
    zMDSCR_EL1 = UINT64_C(0x00000000);
    unit zgsz3534;
    zgsz3534 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3490);
    CONVERT_OF(lbits, fbits)(&zghz3490, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3490, zghz3490, UINT64_C(0x0000000000000000));
    append_64(&zghz3490, zghz3490, UINT64_C(0x0000000000000000));
    COPY(lbits)(&zCDBGDTR_EL0, zghz3490);
    unit zgsz3533;
    zgsz3533 = UNIT;
  }
  {
    zDBGVCR32_EL2 = UINT64_C(0x00000000);
    unit zgsz3532;
    zgsz3532 = UNIT;
  }
  {
    zz5vecz8z5bv32z9 zgsz3249;
    CREATE(zz5vecz8z5bv32z9)(&zgsz3249);
    internal_vector_init_zz5vecz8z5bv32z9(&zgsz3249, INT64_C(16));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3249, zgsz3249, INT64_C(0), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3249, zgsz3249, INT64_C(1), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3249, zgsz3249, INT64_C(2), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3249, zgsz3249, INT64_C(3), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3249, zgsz3249, INT64_C(4), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3249, zgsz3249, INT64_C(5), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3249, zgsz3249, INT64_C(6), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3249, zgsz3249, INT64_C(7), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3249, zgsz3249, INT64_C(8), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3249, zgsz3249, INT64_C(9), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3249, zgsz3249, INT64_C(10), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3249, zgsz3249, INT64_C(11), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3249, zgsz3249, INT64_C(12), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3249, zgsz3249, INT64_C(13), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3249, zgsz3249, INT64_C(14), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3249, zgsz3249, INT64_C(15), UINT64_C(0x00000000));
    KILL(zz5vecz8z5bv32z9)(&zDBGWCR_EL1);
    internal_vector_init_zz5vecz8z5bv32z9(&zDBGWCR_EL1, zgsz3249.len);
    for (int zcbz345 = 0; zcbz345 < zgsz3249.len; zcbz345++) {
      uint64_t zcbz346 = zgsz3249.data[zcbz345];
      uint64_t zcbz347;
      zcbz347 = zcbz346;
      zDBGWCR_EL1.data[zcbz345] = zcbz347;
    }
    unit zgsz3531;
    zgsz3531 = UNIT;
    KILL(zz5vecz8z5bv32z9)(&zgsz3249);
  }
  {
    zz5vecz8z5bv64z9 zgsz3250;
    CREATE(zz5vecz8z5bv64z9)(&zgsz3250);
    internal_vector_init_zz5vecz8z5bv64z9(&zgsz3250, INT64_C(16));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3250, zgsz3250, INT64_C(0), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3250, zgsz3250, INT64_C(1), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3250, zgsz3250, INT64_C(2), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3250, zgsz3250, INT64_C(3), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3250, zgsz3250, INT64_C(4), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3250, zgsz3250, INT64_C(5), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3250, zgsz3250, INT64_C(6), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3250, zgsz3250, INT64_C(7), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3250, zgsz3250, INT64_C(8), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3250, zgsz3250, INT64_C(9), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3250, zgsz3250, INT64_C(10), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3250, zgsz3250, INT64_C(11), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3250, zgsz3250, INT64_C(12), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3250, zgsz3250, INT64_C(13), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3250, zgsz3250, INT64_C(14), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3250, zgsz3250, INT64_C(15), UINT64_C(0x0000000000000000));
    KILL(zz5vecz8z5bv64z9)(&zDBGWVR_EL1);
    internal_vector_init_zz5vecz8z5bv64z9(&zDBGWVR_EL1, zgsz3250.len);
    for (int zcbz348 = 0; zcbz348 < zgsz3250.len; zcbz348++) {
      uint64_t zcbz349 = zgsz3250.data[zcbz348];
      uint64_t zcbz350;
      zcbz350 = zcbz349;
      zDBGWVR_EL1.data[zcbz348] = zcbz350;
    }
    unit zgsz3530;
    zgsz3530 = UNIT;
    KILL(zz5vecz8z5bv64z9)(&zgsz3250);
  }
  {
    zDCZID_EL0 = UINT64_C(0x00000000);
    unit zgsz3529;
    zgsz3529 = UNIT;
  }
  {
    zDISR_EL1 = UINT64_C(0x00000000);
    unit zgsz3528;
    zgsz3528 = UNIT;
  }
  {
    zVDISR_EL2 = UINT64_C(0x00000000);
    unit zgsz3527;
    zgsz3527 = UNIT;
  }
  {
    zERRIDR_EL1 = UINT64_C(0x00000000);
    unit zgsz3526;
    zgsz3526 = UNIT;
  }
  {
    zERRSELR_EL1 = UINT64_C(0x00000000);
    unit zgsz3525;
    zgsz3525 = UNIT;
  }
  {
    zERXADDR_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3524;
    zgsz3524 = UNIT;
  }
  {
    zERXCTLR_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3523;
    zgsz3523 = UNIT;
  }
  {
    zERXFR_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3522;
    zgsz3522 = UNIT;
  }
  {
    zERXMISC0_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3521;
    zgsz3521 = UNIT;
  }
  {
    zERXMISC1_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3520;
    zgsz3520 = UNIT;
  }
  {
    zERXSTATUS_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3519;
    zgsz3519 = UNIT;
  }
  {
    zFPCR = UINT64_C(0x00000000);
    unit zgsz3518;
    zgsz3518 = UNIT;
  }
  {
    zFPEXC32_EL2 = UINT64_C(0x00000000);
    unit zgsz3517;
    zgsz3517 = UNIT;
  }
  {
    zFPSR = UINT64_C(0x00000000);
    unit zgsz3516;
    zgsz3516 = UNIT;
  }
  {
    zHACR_EL2 = UINT64_C(0x0000000000000000);
    unit zgsz3515;
    zgsz3515 = UNIT;
  }
  {
    zHSTR_EL2 = UINT64_C(0x00000000);
    unit zgsz3514;
    zgsz3514 = UNIT;
  }
  {
    zz5vecz8z5bv32z9 zgsz3251;
    CREATE(zz5vecz8z5bv32z9)(&zgsz3251);
    internal_vector_init_zz5vecz8z5bv32z9(&zgsz3251, INT64_C(4));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3251, zgsz3251, INT64_C(0), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3251, zgsz3251, INT64_C(1), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3251, zgsz3251, INT64_C(2), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3251, zgsz3251, INT64_C(3), UINT64_C(0x00000000));
    KILL(zz5vecz8z5bv32z9)(&zICC_AP0R_EL1);
    internal_vector_init_zz5vecz8z5bv32z9(&zICC_AP0R_EL1, zgsz3251.len);
    for (int zcbz351 = 0; zcbz351 < zgsz3251.len; zcbz351++) {
      uint64_t zcbz352 = zgsz3251.data[zcbz351];
      uint64_t zcbz353;
      zcbz353 = zcbz352;
      zICC_AP0R_EL1.data[zcbz351] = zcbz353;
    }
    unit zgsz3513;
    zgsz3513 = UNIT;
    KILL(zz5vecz8z5bv32z9)(&zgsz3251);
  }
  {
    zICC_SRE_EL1_NS = UINT64_C(0x00000000);
    unit zgsz3512;
    zgsz3512 = UNIT;
  }
  {
    zICC_SRE_EL1_S = UINT64_C(0x00000000);
    unit zgsz3511;
    zgsz3511 = UNIT;
  }
  {
    zICC_SRE_EL2 = UINT64_C(0x00000000);
    unit zgsz3510;
    zgsz3510 = UNIT;
  }
  {
    zICC_SRE_EL3 = UINT64_C(0x00000000);
    unit zgsz3509;
    zgsz3509 = UNIT;
  }
  {
    zICH_HCR_EL2 = UINT64_C(0x00000000);
    unit zgsz3508;
    zgsz3508 = UNIT;
  }
  {
    zz5vecz8z5bv32z9 zgsz3252;
    CREATE(zz5vecz8z5bv32z9)(&zgsz3252);
    internal_vector_init_zz5vecz8z5bv32z9(&zgsz3252, INT64_C(4));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3252, zgsz3252, INT64_C(0), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3252, zgsz3252, INT64_C(1), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3252, zgsz3252, INT64_C(2), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3252, zgsz3252, INT64_C(3), UINT64_C(0x00000000));
    KILL(zz5vecz8z5bv32z9)(&zICV_AP0R_EL1);
    internal_vector_init_zz5vecz8z5bv32z9(&zICV_AP0R_EL1, zgsz3252.len);
    for (int zcbz354 = 0; zcbz354 < zgsz3252.len; zcbz354++) {
      uint64_t zcbz355 = zgsz3252.data[zcbz354];
      uint64_t zcbz356;
      zcbz356 = zcbz355;
      zICV_AP0R_EL1.data[zcbz354] = zcbz356;
    }
    unit zgsz3507;
    zgsz3507 = UNIT;
    KILL(zz5vecz8z5bv32z9)(&zgsz3252);
  }
  {
    zz5vecz8z5bv32z9 zgsz3253;
    CREATE(zz5vecz8z5bv32z9)(&zgsz3253);
    internal_vector_init_zz5vecz8z5bv32z9(&zgsz3253, INT64_C(4));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3253, zgsz3253, INT64_C(0), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3253, zgsz3253, INT64_C(1), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3253, zgsz3253, INT64_C(2), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3253, zgsz3253, INT64_C(3), UINT64_C(0x00000000));
    KILL(zz5vecz8z5bv32z9)(&zICC_AP1R_EL1);
    internal_vector_init_zz5vecz8z5bv32z9(&zICC_AP1R_EL1, zgsz3253.len);
    for (int zcbz357 = 0; zcbz357 < zgsz3253.len; zcbz357++) {
      uint64_t zcbz358 = zgsz3253.data[zcbz357];
      uint64_t zcbz359;
      zcbz359 = zcbz358;
      zICC_AP1R_EL1.data[zcbz357] = zcbz359;
    }
    unit zgsz3506;
    zgsz3506 = UNIT;
    KILL(zz5vecz8z5bv32z9)(&zgsz3253);
  }
  {
    zz5vecz8z5bv32z9 zgsz3254;
    CREATE(zz5vecz8z5bv32z9)(&zgsz3254);
    internal_vector_init_zz5vecz8z5bv32z9(&zgsz3254, INT64_C(4));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3254, zgsz3254, INT64_C(0), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3254, zgsz3254, INT64_C(1), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3254, zgsz3254, INT64_C(2), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3254, zgsz3254, INT64_C(3), UINT64_C(0x00000000));
    KILL(zz5vecz8z5bv32z9)(&zICC_AP1R_EL1_NS);
    internal_vector_init_zz5vecz8z5bv32z9(&zICC_AP1R_EL1_NS, zgsz3254.len);
    for (int zcbz360 = 0; zcbz360 < zgsz3254.len; zcbz360++) {
      uint64_t zcbz361 = zgsz3254.data[zcbz360];
      uint64_t zcbz362;
      zcbz362 = zcbz361;
      zICC_AP1R_EL1_NS.data[zcbz360] = zcbz362;
    }
    unit zgsz3505;
    zgsz3505 = UNIT;
    KILL(zz5vecz8z5bv32z9)(&zgsz3254);
  }
  {
    zz5vecz8z5bv32z9 zgsz3255;
    CREATE(zz5vecz8z5bv32z9)(&zgsz3255);
    internal_vector_init_zz5vecz8z5bv32z9(&zgsz3255, INT64_C(4));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3255, zgsz3255, INT64_C(0), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3255, zgsz3255, INT64_C(1), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3255, zgsz3255, INT64_C(2), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3255, zgsz3255, INT64_C(3), UINT64_C(0x00000000));
    KILL(zz5vecz8z5bv32z9)(&zICC_AP1R_EL1_S);
    internal_vector_init_zz5vecz8z5bv32z9(&zICC_AP1R_EL1_S, zgsz3255.len);
    for (int zcbz363 = 0; zcbz363 < zgsz3255.len; zcbz363++) {
      uint64_t zcbz364 = zgsz3255.data[zcbz363];
      uint64_t zcbz365;
      zcbz365 = zcbz364;
      zICC_AP1R_EL1_S.data[zcbz363] = zcbz365;
    }
    unit zgsz3504;
    zgsz3504 = UNIT;
    KILL(zz5vecz8z5bv32z9)(&zgsz3255);
  }
  {
    zz5vecz8z5bv32z9 zgsz3256;
    CREATE(zz5vecz8z5bv32z9)(&zgsz3256);
    internal_vector_init_zz5vecz8z5bv32z9(&zgsz3256, INT64_C(4));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3256, zgsz3256, INT64_C(0), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3256, zgsz3256, INT64_C(1), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3256, zgsz3256, INT64_C(2), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3256, zgsz3256, INT64_C(3), UINT64_C(0x00000000));
    KILL(zz5vecz8z5bv32z9)(&zICV_AP1R_EL1);
    internal_vector_init_zz5vecz8z5bv32z9(&zICV_AP1R_EL1, zgsz3256.len);
    for (int zcbz366 = 0; zcbz366 < zgsz3256.len; zcbz366++) {
      uint64_t zcbz367 = zgsz3256.data[zcbz366];
      uint64_t zcbz368;
      zcbz368 = zcbz367;
      zICV_AP1R_EL1.data[zcbz366] = zcbz368;
    }
    unit zgsz3503;
    zgsz3503 = UNIT;
    KILL(zz5vecz8z5bv32z9)(&zgsz3256);
  }
  {
    zICC_BPR0_EL1 = UINT64_C(0x00000000);
    unit zgsz3502;
    zgsz3502 = UNIT;
  }
  {
    zICV_BPR0_EL1 = UINT64_C(0x00000000);
    unit zgsz3501;
    zgsz3501 = UNIT;
  }
  {
    zICC_BPR1_EL1_NS = UINT64_C(0x00000000);
    unit zgsz3500;
    zgsz3500 = UNIT;
  }
  {
    zICC_BPR1_EL1_S = UINT64_C(0x00000000);
    unit zgsz3499;
    zgsz3499 = UNIT;
  }
  {
    zICV_BPR1_EL1 = UINT64_C(0x00000000);
    unit zgsz3498;
    zgsz3498 = UNIT;
  }
  {
    zICC_CTLR_EL1_NS = UINT64_C(0x00000000);
    unit zgsz3497;
    zgsz3497 = UNIT;
  }
  {
    zICC_CTLR_EL1_S = UINT64_C(0x00000000);
    unit zgsz3496;
    zgsz3496 = UNIT;
  }
  {
    zICV_CTLR_EL1 = UINT64_C(0x00000000);
    unit zgsz3495;
    zgsz3495 = UNIT;
  }
  {
    zICC_CTLR_EL3 = UINT64_C(0x00000000);
    unit zgsz3494;
    zgsz3494 = UNIT;
  }
  {
    zICC_HPPIR0_EL1 = UINT64_C(0x00000000);
    unit zgsz3493;
    zgsz3493 = UNIT;
  }
  {
    zICV_HPPIR0_EL1 = UINT64_C(0x00000000);
    unit zgsz3492;
    zgsz3492 = UNIT;
  }
  {
    zICC_HPPIR1_EL1 = UINT64_C(0x00000000);
    unit zgsz3491;
    zgsz3491 = UNIT;
  }
  {
    zICV_HPPIR1_EL1 = UINT64_C(0x00000000);
    unit zgsz3490;
    zgsz3490 = UNIT;
  }
  {
    zICC_IAR0_EL1 = UINT64_C(0x00000000);
    unit zgsz3489;
    zgsz3489 = UNIT;
  }
  {
    zICV_IAR0_EL1 = UINT64_C(0x00000000);
    unit zgsz3488;
    zgsz3488 = UNIT;
  }
  {
    zICC_IAR1_EL1 = UINT64_C(0x00000000);
    unit zgsz3487;
    zgsz3487 = UNIT;
  }
  {
    zICV_IAR1_EL1 = UINT64_C(0x00000000);
    unit zgsz3486;
    zgsz3486 = UNIT;
  }
  {
    zICC_IGRPEN0_EL1 = UINT64_C(0x00000000);
    unit zgsz3485;
    zgsz3485 = UNIT;
  }
  {
    zICV_IGRPEN0_EL1 = UINT64_C(0x00000000);
    unit zgsz3484;
    zgsz3484 = UNIT;
  }
  {
    zICC_IGRPEN1_EL1_NS = UINT64_C(0x00000000);
    unit zgsz3483;
    zgsz3483 = UNIT;
  }
  {
    zICC_IGRPEN1_EL1_S = UINT64_C(0x00000000);
    unit zgsz3482;
    zgsz3482 = UNIT;
  }
  {
    zICV_IGRPEN1_EL1 = UINT64_C(0x00000000);
    unit zgsz3481;
    zgsz3481 = UNIT;
  }
  {
    zICC_IGRPEN1_EL3 = UINT64_C(0x00000000);
    unit zgsz3480;
    zgsz3480 = UNIT;
  }
  {
    zICC_PMR_EL1 = UINT64_C(0x00000000);
    unit zgsz3479;
    zgsz3479 = UNIT;
  }
  {
    zICV_PMR_EL1 = UINT64_C(0x00000000);
    unit zgsz3478;
    zgsz3478 = UNIT;
  }
  {
    zICC_RPR_EL1 = UINT64_C(0x00000000);
    unit zgsz3477;
    zgsz3477 = UNIT;
  }
  {
    zICV_RPR_EL1 = UINT64_C(0x00000000);
    unit zgsz3476;
    zgsz3476 = UNIT;
  }
  {
    zz5vecz8z5bv32z9 zgsz3257;
    CREATE(zz5vecz8z5bv32z9)(&zgsz3257);
    internal_vector_init_zz5vecz8z5bv32z9(&zgsz3257, INT64_C(4));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3257, zgsz3257, INT64_C(0), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3257, zgsz3257, INT64_C(1), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3257, zgsz3257, INT64_C(2), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3257, zgsz3257, INT64_C(3), UINT64_C(0x00000000));
    KILL(zz5vecz8z5bv32z9)(&zICH_AP0R_EL2);
    internal_vector_init_zz5vecz8z5bv32z9(&zICH_AP0R_EL2, zgsz3257.len);
    for (int zcbz369 = 0; zcbz369 < zgsz3257.len; zcbz369++) {
      uint64_t zcbz370 = zgsz3257.data[zcbz369];
      uint64_t zcbz371;
      zcbz371 = zcbz370;
      zICH_AP0R_EL2.data[zcbz369] = zcbz371;
    }
    unit zgsz3475;
    zgsz3475 = UNIT;
    KILL(zz5vecz8z5bv32z9)(&zgsz3257);
  }
  {
    zz5vecz8z5bv32z9 zgsz3258;
    CREATE(zz5vecz8z5bv32z9)(&zgsz3258);
    internal_vector_init_zz5vecz8z5bv32z9(&zgsz3258, INT64_C(4));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3258, zgsz3258, INT64_C(0), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3258, zgsz3258, INT64_C(1), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3258, zgsz3258, INT64_C(2), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3258, zgsz3258, INT64_C(3), UINT64_C(0x00000000));
    KILL(zz5vecz8z5bv32z9)(&zICH_AP1R_EL2);
    internal_vector_init_zz5vecz8z5bv32z9(&zICH_AP1R_EL2, zgsz3258.len);
    for (int zcbz372 = 0; zcbz372 < zgsz3258.len; zcbz372++) {
      uint64_t zcbz373 = zgsz3258.data[zcbz372];
      uint64_t zcbz374;
      zcbz374 = zcbz373;
      zICH_AP1R_EL2.data[zcbz372] = zcbz374;
    }
    unit zgsz3474;
    zgsz3474 = UNIT;
    KILL(zz5vecz8z5bv32z9)(&zgsz3258);
  }
  {
    zICH_EISR_EL2 = UINT64_C(0x00000000);
    unit zgsz3473;
    zgsz3473 = UNIT;
  }
  {
    zICH_ELRSR_EL2 = UINT64_C(0x00000000);
    unit zgsz3472;
    zgsz3472 = UNIT;
  }
  {
    zz5vecz8z5bv64z9 zgsz3259;
    CREATE(zz5vecz8z5bv64z9)(&zgsz3259);
    internal_vector_init_zz5vecz8z5bv64z9(&zgsz3259, INT64_C(16));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3259, zgsz3259, INT64_C(0), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3259, zgsz3259, INT64_C(1), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3259, zgsz3259, INT64_C(2), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3259, zgsz3259, INT64_C(3), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3259, zgsz3259, INT64_C(4), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3259, zgsz3259, INT64_C(5), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3259, zgsz3259, INT64_C(6), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3259, zgsz3259, INT64_C(7), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3259, zgsz3259, INT64_C(8), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3259, zgsz3259, INT64_C(9), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3259, zgsz3259, INT64_C(10), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3259, zgsz3259, INT64_C(11), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3259, zgsz3259, INT64_C(12), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3259, zgsz3259, INT64_C(13), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3259, zgsz3259, INT64_C(14), UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bv64z9(&zgsz3259, zgsz3259, INT64_C(15), UINT64_C(0x0000000000000000));
    KILL(zz5vecz8z5bv64z9)(&zICH_LR_EL2);
    internal_vector_init_zz5vecz8z5bv64z9(&zICH_LR_EL2, zgsz3259.len);
    for (int zcbz375 = 0; zcbz375 < zgsz3259.len; zcbz375++) {
      uint64_t zcbz376 = zgsz3259.data[zcbz375];
      uint64_t zcbz377;
      zcbz377 = zcbz376;
      zICH_LR_EL2.data[zcbz375] = zcbz377;
    }
    unit zgsz3471;
    zgsz3471 = UNIT;
    KILL(zz5vecz8z5bv64z9)(&zgsz3259);
  }
  {
    zICH_MISR_EL2 = UINT64_C(0x00000000);
    unit zgsz3470;
    zgsz3470 = UNIT;
  }
  {
    zICH_VMCR_EL2 = UINT64_C(0x00000000);
    unit zgsz3469;
    zgsz3469 = UNIT;
  }
  {
    zICH_VTR_EL2 = UINT64_C(0x00000000);
    unit zgsz3468;
    zgsz3468 = UNIT;
  }
  {
    zID_AA64AFR0_EL1 = UINT64_C(0x00000000);
    unit zgsz3467;
    zgsz3467 = UNIT;
  }
  {
    zID_AA64AFR1_EL1 = UINT64_C(0x00000000);
    unit zgsz3466;
    zgsz3466 = UNIT;
  }
  {
    zID_AA64DFR0_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3465;
    zgsz3465 = UNIT;
  }
  {
    zID_AA64DFR1_EL1 = UINT64_C(0x00000000);
    unit zgsz3464;
    zgsz3464 = UNIT;
  }
  {
    zID_AA64ISAR0_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3463;
    zgsz3463 = UNIT;
  }
  {
    zID_AA64ISAR1_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3462;
    zgsz3462 = UNIT;
  }
  {
    zID_AA64MMFR0_EL1 = UINT64_C(0x00000000);
    unit zgsz3461;
    zgsz3461 = UNIT;
  }
  {
    zID_AA64MMFR1_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3460;
    zgsz3460 = UNIT;
  }
  {
    zID_AA64MMFR2_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3459;
    zgsz3459 = UNIT;
  }
  {
    zID_AA64PFR0_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3458;
    zgsz3458 = UNIT;
  }
  {
    zID_AA64PFR1_EL1 = UINT64_C(0x00000000);
    unit zgsz3457;
    zgsz3457 = UNIT;
  }
  {
    zID_AA64ZFR0_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3456;
    zgsz3456 = UNIT;
  }
  {
    zID_AFR0_EL1 = UINT64_C(0x00000000);
    unit zgsz3455;
    zgsz3455 = UNIT;
  }
  {
    zID_DFR0_EL1 = UINT64_C(0x00000000);
    unit zgsz3454;
    zgsz3454 = UNIT;
  }
  {
    zID_ISAR0_EL1 = UINT64_C(0x00000000);
    unit zgsz3453;
    zgsz3453 = UNIT;
  }
  {
    zID_ISAR1_EL1 = UINT64_C(0x00000000);
    unit zgsz3452;
    zgsz3452 = UNIT;
  }
  {
    zID_ISAR2_EL1 = UINT64_C(0x00000000);
    unit zgsz3451;
    zgsz3451 = UNIT;
  }
  {
    zID_ISAR3_EL1 = UINT64_C(0x00000000);
    unit zgsz3450;
    zgsz3450 = UNIT;
  }
  {
    zID_ISAR4_EL1 = UINT64_C(0x00000000);
    unit zgsz3449;
    zgsz3449 = UNIT;
  }
  {
    zID_ISAR5_EL1 = UINT64_C(0x00000000);
    unit zgsz3448;
    zgsz3448 = UNIT;
  }
  {
    zID_ISAR6_EL1 = UINT64_C(0x00000000);
    unit zgsz3447;
    zgsz3447 = UNIT;
  }
  {
    zID_MMFR0_EL1 = UINT64_C(0x00000000);
    unit zgsz3446;
    zgsz3446 = UNIT;
  }
  {
    zID_MMFR1_EL1 = UINT64_C(0x00000000);
    unit zgsz3445;
    zgsz3445 = UNIT;
  }
  {
    zID_MMFR2_EL1 = UINT64_C(0x00000000);
    unit zgsz3444;
    zgsz3444 = UNIT;
  }
  {
    zID_MMFR3_EL1 = UINT64_C(0x00000000);
    unit zgsz3443;
    zgsz3443 = UNIT;
  }
  {
    zID_MMFR4_EL1 = UINT64_C(0x00000000);
    unit zgsz3442;
    zgsz3442 = UNIT;
  }
  {
    zID_MMFR5_EL1 = UINT64_C(0x00000000);
    unit zgsz3441;
    zgsz3441 = UNIT;
  }
  {
    zID_PFR0_EL1 = UINT64_C(0x00000000);
    unit zgsz3440;
    zgsz3440 = UNIT;
  }
  {
    zID_PFR1_EL1 = UINT64_C(0x00000000);
    unit zgsz3439;
    zgsz3439 = UNIT;
  }
  {
    zID_PFR2_EL1 = UINT64_C(0x00000000);
    unit zgsz3438;
    zgsz3438 = UNIT;
  }
  {
    zIFSR32_EL2 = UINT64_C(0x00000000);
    unit zgsz3437;
    zgsz3437 = UNIT;
  }
  {
    zISR_EL1 = UINT64_C(0x00000000);
    unit zgsz3436;
    zgsz3436 = UNIT;
  }
  {
    zLORC_EL1 = UINT64_C(0x00000000);
    unit zgsz3435;
    zgsz3435 = UNIT;
  }
  {
    zLOREA_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3434;
    zgsz3434 = UNIT;
  }
  {
    zLORID_EL1 = UINT64_C(0x00000000);
    unit zgsz3433;
    zgsz3433 = UNIT;
  }
  {
    zLORN_EL1 = UINT64_C(0x00000000);
    unit zgsz3432;
    zgsz3432 = UNIT;
  }
  {
    zLORSA_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3431;
    zgsz3431 = UNIT;
  }
  {
    zMAIR_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3430;
    zgsz3430 = UNIT;
  }
  {
    zMAIR_EL2 = UINT64_C(0x0000000000000000);
    unit zgsz3429;
    zgsz3429 = UNIT;
  }
  {
    zMAIR_EL3 = UINT64_C(0x0000000000000000);
    unit zgsz3428;
    zgsz3428 = UNIT;
  }
  {
    zMDCCINT_EL1 = UINT64_C(0x00000000);
    unit zgsz3427;
    zgsz3427 = UNIT;
  }
  {
    zMDCCSR_EL0 = UINT64_C(0x00000000);
    unit zgsz3426;
    zgsz3426 = UNIT;
  }
  {
    zMDRAR_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3425;
    zgsz3425 = UNIT;
  }
  {
    zMIDR_EL1 = UINT64_C(0x00000000);
    unit zgsz3424;
    zgsz3424 = UNIT;
  }
  {
    zVPIDR_EL2 = UINT64_C(0x00000000);
    unit zgsz3423;
    zgsz3423 = UNIT;
  }
  {
    zMPAM0_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3422;
    zgsz3422 = UNIT;
  }
  {
    zMPAM3_EL3 = UINT64_C(0x0000000000000000);
    unit zgsz3421;
    zgsz3421 = UNIT;
  }
  {
    z_MPAM2_EL2_0_62 = UINT64_C(0b000000000000000000000000000000000000000000000000000000000000000);
    unit zgsz3420;
    zgsz3420 = UNIT;
  }
  {
    z_MPAM1_EL1_0_62 = UINT64_C(0b000000000000000000000000000000000000000000000000000000000000000);
    unit zgsz3419;
    zgsz3419 = UNIT;
  }
  {
    zMPAMHCR_EL2 = UINT64_C(0x00000000);
    unit zgsz3418;
    zgsz3418 = UNIT;
  }
  {
    zMPAMIDR_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3417;
    zgsz3417 = UNIT;
  }
  {
    zMPAMVPM0_EL2 = UINT64_C(0x0000000000000000);
    unit zgsz3416;
    zgsz3416 = UNIT;
  }
  {
    zMPAMVPM1_EL2 = UINT64_C(0x0000000000000000);
    unit zgsz3415;
    zgsz3415 = UNIT;
  }
  {
    zMPAMVPM2_EL2 = UINT64_C(0x0000000000000000);
    unit zgsz3414;
    zgsz3414 = UNIT;
  }
  {
    zMPAMVPM3_EL2 = UINT64_C(0x0000000000000000);
    unit zgsz3413;
    zgsz3413 = UNIT;
  }
  {
    zMPAMVPM4_EL2 = UINT64_C(0x0000000000000000);
    unit zgsz3412;
    zgsz3412 = UNIT;
  }
  {
    zMPAMVPM5_EL2 = UINT64_C(0x0000000000000000);
    unit zgsz3411;
    zgsz3411 = UNIT;
  }
  {
    zMPAMVPM6_EL2 = UINT64_C(0x0000000000000000);
    unit zgsz3410;
    zgsz3410 = UNIT;
  }
  {
    zMPAMVPM7_EL2 = UINT64_C(0x0000000000000000);
    unit zgsz3409;
    zgsz3409 = UNIT;
  }
  {
    zMPAMVPMV_EL2 = UINT64_C(0x00000000);
    unit zgsz3408;
    zgsz3408 = UNIT;
  }
  {
    zMPIDR_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3407;
    zgsz3407 = UNIT;
  }
  {
    zVMPIDR_EL2 = UINT64_C(0x0000000000000000);
    unit zgsz3406;
    zgsz3406 = UNIT;
  }
  {
    zMVFR0_EL1 = UINT64_C(0x00000000);
    unit zgsz3405;
    zgsz3405 = UNIT;
  }
  {
    zMVFR1_EL1 = UINT64_C(0x00000000);
    unit zgsz3404;
    zgsz3404 = UNIT;
  }
  {
    zMVFR2_EL1 = UINT64_C(0x00000000);
    unit zgsz3403;
    zgsz3403 = UNIT;
  }
  {
    zOSDTRRX_EL1 = UINT64_C(0x00000000);
    unit zgsz3402;
    zgsz3402 = UNIT;
  }
  {
    zOSDTRTX_EL1 = UINT64_C(0x00000000);
    unit zgsz3401;
    zgsz3401 = UNIT;
  }
  {
    zOSECCR_EL1 = UINT64_C(0x00000000);
    unit zgsz3400;
    zgsz3400 = UNIT;
  }
  {
    zPAR_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3399;
    zgsz3399 = UNIT;
  }
  {
    zPMBIDR_EL1 = UINT64_C(0x00000000);
    unit zgsz3398;
    zgsz3398 = UNIT;
  }
  {
    zPMBLIMITR_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3397;
    zgsz3397 = UNIT;
  }
  {
    zPMBPTR_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3396;
    zgsz3396 = UNIT;
  }
  {
    zPMBSR_EL1 = UINT64_C(0x00000000);
    unit zgsz3395;
    zgsz3395 = UNIT;
  }
  {
    zPMCCFILTR_EL0 = UINT64_C(0x00000000);
    unit zgsz3394;
    zgsz3394 = UNIT;
  }
  {
    zPMUSERENR_EL0 = UINT64_C(0x00000000);
    unit zgsz3393;
    zgsz3393 = UNIT;
  }
  {
    zPMCCNTR_EL0 = UINT64_C(0x0000000000000000);
    unit zgsz3392;
    zgsz3392 = UNIT;
  }
  {
    zPMCEID0_EL0 = UINT64_C(0x0000000000000000);
    unit zgsz3391;
    zgsz3391 = UNIT;
  }
  {
    zPMCEID1_EL0 = UINT64_C(0x0000000000000000);
    unit zgsz3390;
    zgsz3390 = UNIT;
  }
  {
    zPMCNTENCLR_EL0 = UINT64_C(0x00000000);
    unit zgsz3389;
    zgsz3389 = UNIT;
  }
  {
    zPMCNTENSET_EL0 = UINT64_C(0x00000000);
    unit zgsz3388;
    zgsz3388 = UNIT;
  }
  {
    zPMCR_EL0 = UINT64_C(0x00000000);
    unit zgsz3387;
    zgsz3387 = UNIT;
  }
  {
    zz5vecz8z5bv32z9 zgsz3260;
    CREATE(zz5vecz8z5bv32z9)(&zgsz3260);
    internal_vector_init_zz5vecz8z5bv32z9(&zgsz3260, INT64_C(31));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3260, zgsz3260, INT64_C(0), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3260, zgsz3260, INT64_C(1), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3260, zgsz3260, INT64_C(2), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3260, zgsz3260, INT64_C(3), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3260, zgsz3260, INT64_C(4), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3260, zgsz3260, INT64_C(5), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3260, zgsz3260, INT64_C(6), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3260, zgsz3260, INT64_C(7), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3260, zgsz3260, INT64_C(8), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3260, zgsz3260, INT64_C(9), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3260, zgsz3260, INT64_C(10), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3260, zgsz3260, INT64_C(11), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3260, zgsz3260, INT64_C(12), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3260, zgsz3260, INT64_C(13), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3260, zgsz3260, INT64_C(14), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3260, zgsz3260, INT64_C(15), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3260, zgsz3260, INT64_C(16), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3260, zgsz3260, INT64_C(17), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3260, zgsz3260, INT64_C(18), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3260, zgsz3260, INT64_C(19), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3260, zgsz3260, INT64_C(20), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3260, zgsz3260, INT64_C(21), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3260, zgsz3260, INT64_C(22), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3260, zgsz3260, INT64_C(23), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3260, zgsz3260, INT64_C(24), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3260, zgsz3260, INT64_C(25), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3260, zgsz3260, INT64_C(26), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3260, zgsz3260, INT64_C(27), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3260, zgsz3260, INT64_C(28), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3260, zgsz3260, INT64_C(29), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3260, zgsz3260, INT64_C(30), UINT64_C(0x00000000));
    KILL(zz5vecz8z5bv32z9)(&zPMEVCNTR_EL0);
    internal_vector_init_zz5vecz8z5bv32z9(&zPMEVCNTR_EL0, zgsz3260.len);
    for (int zcbz378 = 0; zcbz378 < zgsz3260.len; zcbz378++) {
      uint64_t zcbz379 = zgsz3260.data[zcbz378];
      uint64_t zcbz380;
      zcbz380 = zcbz379;
      zPMEVCNTR_EL0.data[zcbz378] = zcbz380;
    }
    unit zgsz3386;
    zgsz3386 = UNIT;
    KILL(zz5vecz8z5bv32z9)(&zgsz3260);
  }
  {
    zz5vecz8z5bv32z9 zgsz3261;
    CREATE(zz5vecz8z5bv32z9)(&zgsz3261);
    internal_vector_init_zz5vecz8z5bv32z9(&zgsz3261, INT64_C(31));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3261, zgsz3261, INT64_C(0), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3261, zgsz3261, INT64_C(1), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3261, zgsz3261, INT64_C(2), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3261, zgsz3261, INT64_C(3), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3261, zgsz3261, INT64_C(4), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3261, zgsz3261, INT64_C(5), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3261, zgsz3261, INT64_C(6), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3261, zgsz3261, INT64_C(7), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3261, zgsz3261, INT64_C(8), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3261, zgsz3261, INT64_C(9), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3261, zgsz3261, INT64_C(10), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3261, zgsz3261, INT64_C(11), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3261, zgsz3261, INT64_C(12), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3261, zgsz3261, INT64_C(13), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3261, zgsz3261, INT64_C(14), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3261, zgsz3261, INT64_C(15), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3261, zgsz3261, INT64_C(16), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3261, zgsz3261, INT64_C(17), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3261, zgsz3261, INT64_C(18), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3261, zgsz3261, INT64_C(19), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3261, zgsz3261, INT64_C(20), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3261, zgsz3261, INT64_C(21), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3261, zgsz3261, INT64_C(22), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3261, zgsz3261, INT64_C(23), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3261, zgsz3261, INT64_C(24), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3261, zgsz3261, INT64_C(25), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3261, zgsz3261, INT64_C(26), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3261, zgsz3261, INT64_C(27), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3261, zgsz3261, INT64_C(28), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3261, zgsz3261, INT64_C(29), UINT64_C(0x00000000));
    internal_vector_update_zz5vecz8z5bv32z9(&zgsz3261, zgsz3261, INT64_C(30), UINT64_C(0x00000000));
    KILL(zz5vecz8z5bv32z9)(&zPMEVTYPER_EL0);
    internal_vector_init_zz5vecz8z5bv32z9(&zPMEVTYPER_EL0, zgsz3261.len);
    for (int zcbz381 = 0; zcbz381 < zgsz3261.len; zcbz381++) {
      uint64_t zcbz382 = zgsz3261.data[zcbz381];
      uint64_t zcbz383;
      zcbz383 = zcbz382;
      zPMEVTYPER_EL0.data[zcbz381] = zcbz383;
    }
    unit zgsz3385;
    zgsz3385 = UNIT;
    KILL(zz5vecz8z5bv32z9)(&zgsz3261);
  }
  {
    zPMINTENCLR_EL1 = UINT64_C(0x00000000);
    unit zgsz3384;
    zgsz3384 = UNIT;
  }
  {
    zPMINTENSET_EL1 = UINT64_C(0x00000000);
    unit zgsz3383;
    zgsz3383 = UNIT;
  }
  {
    zPMOVSCLR_EL0 = UINT64_C(0x00000000);
    unit zgsz3382;
    zgsz3382 = UNIT;
  }
  {
    zPMOVSSET_EL0 = UINT64_C(0x00000000);
    unit zgsz3381;
    zgsz3381 = UNIT;
  }
  {
    zPMSCR_EL1 = UINT64_C(0x00000000);
    unit zgsz3380;
    zgsz3380 = UNIT;
  }
  {
    zPMSCR_EL2 = UINT64_C(0x00000000);
    unit zgsz3379;
    zgsz3379 = UNIT;
  }
  {
    zPMSELR_EL0 = UINT64_C(0x00000000);
    unit zgsz3378;
    zgsz3378 = UNIT;
  }
  {
    zPMSEVFR_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3377;
    zgsz3377 = UNIT;
  }
  {
    zPMSFCR_EL1 = UINT64_C(0x00000000);
    unit zgsz3376;
    zgsz3376 = UNIT;
  }
  {
    zPMSICR_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3375;
    zgsz3375 = UNIT;
  }
  {
    zPMSIDR_EL1 = UINT64_C(0x00000000);
    unit zgsz3374;
    zgsz3374 = UNIT;
  }
  {
    zPMSIRR_EL1 = UINT64_C(0x00000000);
    unit zgsz3373;
    zgsz3373 = UNIT;
  }
  {
    zPMSLATFR_EL1 = UINT64_C(0x00000000);
    unit zgsz3372;
    zgsz3372 = UNIT;
  }
  {
    zPMXEVCNTR_EL0 = UINT64_C(0x00000000);
    unit zgsz3371;
    zgsz3371 = UNIT;
  }
  {
    zPMXEVTYPER_EL0 = UINT64_C(0x00000000);
    unit zgsz3370;
    zgsz3370 = UNIT;
  }
  {
    zREVIDR_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3369;
    zgsz3369 = UNIT;
  }
  {
    zRMR_EL1 = UINT64_C(0x00000000);
    unit zgsz3368;
    zgsz3368 = UNIT;
  }
  {
    zRMR_EL2 = UINT64_C(0x00000000);
    unit zgsz3367;
    zgsz3367 = UNIT;
  }
  {
    zRMR_EL3 = UINT64_C(0x00000000);
    unit zgsz3366;
    zgsz3366 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3489);
    CONVERT_OF(lbits, fbits)(&zghz3489, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3489, zghz3489, UINT64_C(0x0000000000000000));
    append_64(&zghz3489, zghz3489, UINT64_C(0x0000000000000000));
    COPY(lbits)(&zRSP_EL0, zghz3489);
    unit zgsz3365;
    zgsz3365 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3488);
    CONVERT_OF(lbits, fbits)(&zghz3488, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3488, zghz3488, UINT64_C(0x0000000000000000));
    append_64(&zghz3488, zghz3488, UINT64_C(0x0000000000000000));
    COPY(lbits)(&zRTPIDR_EL0, zghz3488);
    unit zgsz3364;
    zgsz3364 = UNIT;
  }
  {
    zRVBAR_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3363;
    zgsz3363 = UNIT;
  }
  {
    zRVBAR_EL2 = UINT64_C(0x0000000000000000);
    unit zgsz3362;
    zgsz3362 = UNIT;
  }
  {
    zRVBAR_EL3 = UINT64_C(0x0000000000000000);
    unit zgsz3361;
    zgsz3361 = UNIT;
  }
  {
    zS3_op1_Cn_Cm_op2 = UINT64_C(0x0000000000000000);
    unit zgsz3360;
    zgsz3360 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3487);
    CONVERT_OF(lbits, fbits)(&zghz3487, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3487, zghz3487, UINT64_C(0x0000000000000000));
    append_64(&zghz3487, zghz3487, UINT64_C(0x0000000000000000));
    COPY(lbits)(&zCID_EL0, zghz3487);
    unit zgsz3359;
    zgsz3359 = UNIT;
  }
  {
    zSCXTNUM_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3358;
    zgsz3358 = UNIT;
  }
  {
    zSCXTNUM_EL2 = UINT64_C(0x0000000000000000);
    unit zgsz3357;
    zgsz3357 = UNIT;
  }
  {
    zSCXTNUM_EL3 = UINT64_C(0x0000000000000000);
    unit zgsz3356;
    zgsz3356 = UNIT;
  }
  {
    zSDER32_EL3 = UINT64_C(0x00000000);
    unit zgsz3355;
    zgsz3355 = UNIT;
  }
  {
    zSPSR_abt = UINT64_C(0x00000000);
    unit zgsz3354;
    zgsz3354 = UNIT;
  }
  {
    zSPSR_fiq = UINT64_C(0x00000000);
    unit zgsz3353;
    zgsz3353 = UNIT;
  }
  {
    zSPSR_irq = UINT64_C(0x00000000);
    unit zgsz3352;
    zgsz3352 = UNIT;
  }
  {
    zSPSR_und = UINT64_C(0x00000000);
    unit zgsz3351;
    zgsz3351 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3486);
    CONVERT_OF(lbits, fbits)(&zghz3486, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3486, zghz3486, UINT64_C(0x0000000000000000));
    append_64(&zghz3486, zghz3486, UINT64_C(0x0000000000000000));
    COPY(lbits)(&zSP_EL0, zghz3486);
    unit zgsz3350;
    zgsz3350 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3485);
    CONVERT_OF(lbits, fbits)(&zghz3485, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3485, zghz3485, UINT64_C(0x0000000000000000));
    append_64(&zghz3485, zghz3485, UINT64_C(0x0000000000000000));
    COPY(lbits)(&zSP_EL1, zghz3485);
    unit zgsz3349;
    zgsz3349 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3484);
    CONVERT_OF(lbits, fbits)(&zghz3484, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3484, zghz3484, UINT64_C(0x0000000000000000));
    append_64(&zghz3484, zghz3484, UINT64_C(0x0000000000000000));
    COPY(lbits)(&zSP_EL2, zghz3484);
    unit zgsz3348;
    zgsz3348 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3483);
    CONVERT_OF(lbits, fbits)(&zghz3483, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3483, zghz3483, UINT64_C(0x0000000000000000));
    append_64(&zghz3483, zghz3483, UINT64_C(0x0000000000000000));
    COPY(lbits)(&zTPIDRRO_EL0, zghz3483);
    unit zgsz3347;
    zgsz3347 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3482);
    CONVERT_OF(lbits, fbits)(&zghz3482, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3482, zghz3482, UINT64_C(0x0000000000000000));
    append_64(&zghz3482, zghz3482, UINT64_C(0x0000000000000000));
    COPY(lbits)(&zTPIDR_EL0, zghz3482);
    unit zgsz3346;
    zgsz3346 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3481);
    CONVERT_OF(lbits, fbits)(&zghz3481, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3481, zghz3481, UINT64_C(0x0000000000000000));
    append_64(&zghz3481, zghz3481, UINT64_C(0x0000000000000000));
    COPY(lbits)(&zTPIDR_EL1, zghz3481);
    unit zgsz3345;
    zgsz3345 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3480);
    CONVERT_OF(lbits, fbits)(&zghz3480, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3480, zghz3480, UINT64_C(0x0000000000000000));
    append_64(&zghz3480, zghz3480, UINT64_C(0x0000000000000000));
    COPY(lbits)(&zTPIDR_EL2, zghz3480);
    unit zgsz3344;
    zgsz3344 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3479);
    CONVERT_OF(lbits, fbits)(&zghz3479, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3479, zghz3479, UINT64_C(0x0000000000000000));
    append_64(&zghz3479, zghz3479, UINT64_C(0x0000000000000000));
    COPY(lbits)(&zTPIDR_EL3, zghz3479);
    unit zgsz3343;
    zgsz3343 = UNIT;
  }
  {
    zTTBR0_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3342;
    zgsz3342 = UNIT;
  }
  {
    zTTBR0_EL2 = UINT64_C(0x0000000000000000);
    unit zgsz3341;
    zgsz3341 = UNIT;
  }
  {
    zTTBR0_EL3 = UINT64_C(0x0000000000000000);
    unit zgsz3340;
    zgsz3340 = UNIT;
  }
  {
    zTTBR1_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3339;
    zgsz3339 = UNIT;
  }
  {
    zTTBR1_EL2 = UINT64_C(0x0000000000000000);
    unit zgsz3338;
    zgsz3338 = UNIT;
  }
  {
    zVSESR_EL2 = UINT64_C(0x00000000);
    unit zgsz3337;
    zgsz3337 = UNIT;
  }
  {
    zVTCR_EL2 = UINT64_C(0x00000000);
    unit zgsz3336;
    zgsz3336 = UNIT;
  }
  {
    zVTTBR_EL2 = UINT64_C(0x0000000000000000);
    unit zgsz3335;
    zgsz3335 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3478);
    CONVERT_OF(lbits, fbits)(&zghz3478, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3478, zghz3478, UINT64_C(0x0000000000000000));
    append_64(&zghz3478, zghz3478, UINT64_C(0x0000000000000000));
    COPY(lbits)(&zDDC_EL0, zghz3478);
    unit zgsz3334;
    zgsz3334 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3477);
    CONVERT_OF(lbits, fbits)(&zghz3477, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3477, zghz3477, UINT64_C(0x0000000000000000));
    append_64(&zghz3477, zghz3477, UINT64_C(0x0000000000000000));
    COPY(lbits)(&zDDC_EL1, zghz3477);
    unit zgsz3333;
    zgsz3333 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3476);
    CONVERT_OF(lbits, fbits)(&zghz3476, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3476, zghz3476, UINT64_C(0x0000000000000000));
    append_64(&zghz3476, zghz3476, UINT64_C(0x0000000000000000));
    COPY(lbits)(&zDDC_EL2, zghz3476);
    unit zgsz3332;
    zgsz3332 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3475);
    CONVERT_OF(lbits, fbits)(&zghz3475, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3475, zghz3475, UINT64_C(0x0000000000000000));
    append_64(&zghz3475, zghz3475, UINT64_C(0x0000000000000000));
    COPY(lbits)(&zDDC_EL3, zghz3475);
    unit zgsz3331;
    zgsz3331 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3474);
    CONVERT_OF(lbits, fbits)(&zghz3474, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3474, zghz3474, UINT64_C(0x0000000000000000));
    append_64(&zghz3474, zghz3474, UINT64_C(0x0000000000000000));
    COPY(lbits)(&zRDDC_EL0, zghz3474);
    unit zgsz3330;
    zgsz3330 = UNIT;
  }
  {
    zDBGDTRTX_EL0 = UINT64_C(0x00000000);
    unit zgsz3329;
    zgsz3329 = UNIT;
  }
  {
    zICC_ASGI1R_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3328;
    zgsz3328 = UNIT;
  }
  {
    zICC_DIR_EL1 = UINT64_C(0x00000000);
    unit zgsz3327;
    zgsz3327 = UNIT;
  }
  {
    zICV_DIR_EL1 = UINT64_C(0x00000000);
    unit zgsz3326;
    zgsz3326 = UNIT;
  }
  {
    zICC_EOIR0_EL1 = UINT64_C(0x00000000);
    unit zgsz3325;
    zgsz3325 = UNIT;
  }
  {
    zICV_EOIR0_EL1 = UINT64_C(0x00000000);
    unit zgsz3324;
    zgsz3324 = UNIT;
  }
  {
    zICC_EOIR1_EL1 = UINT64_C(0x00000000);
    unit zgsz3323;
    zgsz3323 = UNIT;
  }
  {
    zICV_EOIR1_EL1 = UINT64_C(0x00000000);
    unit zgsz3322;
    zgsz3322 = UNIT;
  }
  {
    zICC_SGI0R_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3321;
    zgsz3321 = UNIT;
  }
  {
    zICC_SGI1R_EL1 = UINT64_C(0x0000000000000000);
    unit zgsz3320;
    zgsz3320 = UNIT;
  }
  {
    zOSLAR_EL1 = UINT64_C(0x00000000);
    unit zgsz3319;
    zgsz3319 = UNIT;
  }
  {
    zPMSWINC_EL0 = UINT64_C(0x00000000);
    unit zgsz3318;
    zgsz3318 = UNIT;
  }
  {
    zEventRegister = UINT64_C(0b0);
    unit zgsz3317;
    zgsz3317 = UNIT;
  }
  {
    zz5vecz8z5bvz9 zgsz3278;
    CREATE(zz5vecz8z5bvz9)(&zgsz3278);
    internal_vector_init_zz5vecz8z5bvz9(&zgsz3278, INT64_C(32));
    RECREATE(lbits)(&zghz3442);
    CONVERT_OF(lbits, fbits)(&zghz3442, UINT64_C(0), UINT64_C(0) , true);
    append_64(&zghz3442, zghz3442, UINT64_C(0x0000000000000000));
    append_64(&zghz3442, zghz3442, UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bvz9(&zgsz3278, zgsz3278, INT64_C(0), zghz3442);
    RECREATE(lbits)(&zghz3443);
    CONVERT_OF(lbits, fbits)(&zghz3443, UINT64_C(0), UINT64_C(0) , true);
    append_64(&zghz3443, zghz3443, UINT64_C(0x0000000000000000));
    append_64(&zghz3443, zghz3443, UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bvz9(&zgsz3278, zgsz3278, INT64_C(1), zghz3443);
    RECREATE(lbits)(&zghz3444);
    CONVERT_OF(lbits, fbits)(&zghz3444, UINT64_C(0), UINT64_C(0) , true);
    append_64(&zghz3444, zghz3444, UINT64_C(0x0000000000000000));
    append_64(&zghz3444, zghz3444, UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bvz9(&zgsz3278, zgsz3278, INT64_C(2), zghz3444);
    RECREATE(lbits)(&zghz3445);
    CONVERT_OF(lbits, fbits)(&zghz3445, UINT64_C(0), UINT64_C(0) , true);
    append_64(&zghz3445, zghz3445, UINT64_C(0x0000000000000000));
    append_64(&zghz3445, zghz3445, UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bvz9(&zgsz3278, zgsz3278, INT64_C(3), zghz3445);
    RECREATE(lbits)(&zghz3446);
    CONVERT_OF(lbits, fbits)(&zghz3446, UINT64_C(0), UINT64_C(0) , true);
    append_64(&zghz3446, zghz3446, UINT64_C(0x0000000000000000));
    append_64(&zghz3446, zghz3446, UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bvz9(&zgsz3278, zgsz3278, INT64_C(4), zghz3446);
    RECREATE(lbits)(&zghz3447);
    CONVERT_OF(lbits, fbits)(&zghz3447, UINT64_C(0), UINT64_C(0) , true);
    append_64(&zghz3447, zghz3447, UINT64_C(0x0000000000000000));
    append_64(&zghz3447, zghz3447, UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bvz9(&zgsz3278, zgsz3278, INT64_C(5), zghz3447);
    RECREATE(lbits)(&zghz3448);
    CONVERT_OF(lbits, fbits)(&zghz3448, UINT64_C(0), UINT64_C(0) , true);
    append_64(&zghz3448, zghz3448, UINT64_C(0x0000000000000000));
    append_64(&zghz3448, zghz3448, UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bvz9(&zgsz3278, zgsz3278, INT64_C(6), zghz3448);
    RECREATE(lbits)(&zghz3449);
    CONVERT_OF(lbits, fbits)(&zghz3449, UINT64_C(0), UINT64_C(0) , true);
    append_64(&zghz3449, zghz3449, UINT64_C(0x0000000000000000));
    append_64(&zghz3449, zghz3449, UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bvz9(&zgsz3278, zgsz3278, INT64_C(7), zghz3449);
    RECREATE(lbits)(&zghz3450);
    CONVERT_OF(lbits, fbits)(&zghz3450, UINT64_C(0), UINT64_C(0) , true);
    append_64(&zghz3450, zghz3450, UINT64_C(0x0000000000000000));
    append_64(&zghz3450, zghz3450, UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bvz9(&zgsz3278, zgsz3278, INT64_C(8), zghz3450);
    RECREATE(lbits)(&zghz3451);
    CONVERT_OF(lbits, fbits)(&zghz3451, UINT64_C(0), UINT64_C(0) , true);
    append_64(&zghz3451, zghz3451, UINT64_C(0x0000000000000000));
    append_64(&zghz3451, zghz3451, UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bvz9(&zgsz3278, zgsz3278, INT64_C(9), zghz3451);
    RECREATE(lbits)(&zghz3452);
    CONVERT_OF(lbits, fbits)(&zghz3452, UINT64_C(0), UINT64_C(0) , true);
    append_64(&zghz3452, zghz3452, UINT64_C(0x0000000000000000));
    append_64(&zghz3452, zghz3452, UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bvz9(&zgsz3278, zgsz3278, INT64_C(10), zghz3452);
    RECREATE(lbits)(&zghz3453);
    CONVERT_OF(lbits, fbits)(&zghz3453, UINT64_C(0), UINT64_C(0) , true);
    append_64(&zghz3453, zghz3453, UINT64_C(0x0000000000000000));
    append_64(&zghz3453, zghz3453, UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bvz9(&zgsz3278, zgsz3278, INT64_C(11), zghz3453);
    RECREATE(lbits)(&zghz3454);
    CONVERT_OF(lbits, fbits)(&zghz3454, UINT64_C(0), UINT64_C(0) , true);
    append_64(&zghz3454, zghz3454, UINT64_C(0x0000000000000000));
    append_64(&zghz3454, zghz3454, UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bvz9(&zgsz3278, zgsz3278, INT64_C(12), zghz3454);
    RECREATE(lbits)(&zghz3455);
    CONVERT_OF(lbits, fbits)(&zghz3455, UINT64_C(0), UINT64_C(0) , true);
    append_64(&zghz3455, zghz3455, UINT64_C(0x0000000000000000));
    append_64(&zghz3455, zghz3455, UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bvz9(&zgsz3278, zgsz3278, INT64_C(13), zghz3455);
    RECREATE(lbits)(&zghz3456);
    CONVERT_OF(lbits, fbits)(&zghz3456, UINT64_C(0), UINT64_C(0) , true);
    append_64(&zghz3456, zghz3456, UINT64_C(0x0000000000000000));
    append_64(&zghz3456, zghz3456, UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bvz9(&zgsz3278, zgsz3278, INT64_C(14), zghz3456);
    RECREATE(lbits)(&zghz3457);
    CONVERT_OF(lbits, fbits)(&zghz3457, UINT64_C(0), UINT64_C(0) , true);
    append_64(&zghz3457, zghz3457, UINT64_C(0x0000000000000000));
    append_64(&zghz3457, zghz3457, UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bvz9(&zgsz3278, zgsz3278, INT64_C(15), zghz3457);
    RECREATE(lbits)(&zghz3458);
    CONVERT_OF(lbits, fbits)(&zghz3458, UINT64_C(0), UINT64_C(0) , true);
    append_64(&zghz3458, zghz3458, UINT64_C(0x0000000000000000));
    append_64(&zghz3458, zghz3458, UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bvz9(&zgsz3278, zgsz3278, INT64_C(16), zghz3458);
    RECREATE(lbits)(&zghz3459);
    CONVERT_OF(lbits, fbits)(&zghz3459, UINT64_C(0), UINT64_C(0) , true);
    append_64(&zghz3459, zghz3459, UINT64_C(0x0000000000000000));
    append_64(&zghz3459, zghz3459, UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bvz9(&zgsz3278, zgsz3278, INT64_C(17), zghz3459);
    RECREATE(lbits)(&zghz3460);
    CONVERT_OF(lbits, fbits)(&zghz3460, UINT64_C(0), UINT64_C(0) , true);
    append_64(&zghz3460, zghz3460, UINT64_C(0x0000000000000000));
    append_64(&zghz3460, zghz3460, UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bvz9(&zgsz3278, zgsz3278, INT64_C(18), zghz3460);
    RECREATE(lbits)(&zghz3461);
    CONVERT_OF(lbits, fbits)(&zghz3461, UINT64_C(0), UINT64_C(0) , true);
    append_64(&zghz3461, zghz3461, UINT64_C(0x0000000000000000));
    append_64(&zghz3461, zghz3461, UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bvz9(&zgsz3278, zgsz3278, INT64_C(19), zghz3461);
    RECREATE(lbits)(&zghz3462);
    CONVERT_OF(lbits, fbits)(&zghz3462, UINT64_C(0), UINT64_C(0) , true);
    append_64(&zghz3462, zghz3462, UINT64_C(0x0000000000000000));
    append_64(&zghz3462, zghz3462, UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bvz9(&zgsz3278, zgsz3278, INT64_C(20), zghz3462);
    RECREATE(lbits)(&zghz3463);
    CONVERT_OF(lbits, fbits)(&zghz3463, UINT64_C(0), UINT64_C(0) , true);
    append_64(&zghz3463, zghz3463, UINT64_C(0x0000000000000000));
    append_64(&zghz3463, zghz3463, UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bvz9(&zgsz3278, zgsz3278, INT64_C(21), zghz3463);
    RECREATE(lbits)(&zghz3464);
    CONVERT_OF(lbits, fbits)(&zghz3464, UINT64_C(0), UINT64_C(0) , true);
    append_64(&zghz3464, zghz3464, UINT64_C(0x0000000000000000));
    append_64(&zghz3464, zghz3464, UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bvz9(&zgsz3278, zgsz3278, INT64_C(22), zghz3464);
    RECREATE(lbits)(&zghz3465);
    CONVERT_OF(lbits, fbits)(&zghz3465, UINT64_C(0), UINT64_C(0) , true);
    append_64(&zghz3465, zghz3465, UINT64_C(0x0000000000000000));
    append_64(&zghz3465, zghz3465, UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bvz9(&zgsz3278, zgsz3278, INT64_C(23), zghz3465);
    RECREATE(lbits)(&zghz3466);
    CONVERT_OF(lbits, fbits)(&zghz3466, UINT64_C(0), UINT64_C(0) , true);
    append_64(&zghz3466, zghz3466, UINT64_C(0x0000000000000000));
    append_64(&zghz3466, zghz3466, UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bvz9(&zgsz3278, zgsz3278, INT64_C(24), zghz3466);
    RECREATE(lbits)(&zghz3467);
    CONVERT_OF(lbits, fbits)(&zghz3467, UINT64_C(0), UINT64_C(0) , true);
    append_64(&zghz3467, zghz3467, UINT64_C(0x0000000000000000));
    append_64(&zghz3467, zghz3467, UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bvz9(&zgsz3278, zgsz3278, INT64_C(25), zghz3467);
    RECREATE(lbits)(&zghz3468);
    CONVERT_OF(lbits, fbits)(&zghz3468, UINT64_C(0), UINT64_C(0) , true);
    append_64(&zghz3468, zghz3468, UINT64_C(0x0000000000000000));
    append_64(&zghz3468, zghz3468, UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bvz9(&zgsz3278, zgsz3278, INT64_C(26), zghz3468);
    RECREATE(lbits)(&zghz3469);
    CONVERT_OF(lbits, fbits)(&zghz3469, UINT64_C(0), UINT64_C(0) , true);
    append_64(&zghz3469, zghz3469, UINT64_C(0x0000000000000000));
    append_64(&zghz3469, zghz3469, UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bvz9(&zgsz3278, zgsz3278, INT64_C(27), zghz3469);
    RECREATE(lbits)(&zghz3470);
    CONVERT_OF(lbits, fbits)(&zghz3470, UINT64_C(0), UINT64_C(0) , true);
    append_64(&zghz3470, zghz3470, UINT64_C(0x0000000000000000));
    append_64(&zghz3470, zghz3470, UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bvz9(&zgsz3278, zgsz3278, INT64_C(28), zghz3470);
    RECREATE(lbits)(&zghz3471);
    CONVERT_OF(lbits, fbits)(&zghz3471, UINT64_C(0), UINT64_C(0) , true);
    append_64(&zghz3471, zghz3471, UINT64_C(0x0000000000000000));
    append_64(&zghz3471, zghz3471, UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bvz9(&zgsz3278, zgsz3278, INT64_C(29), zghz3471);
    RECREATE(lbits)(&zghz3472);
    CONVERT_OF(lbits, fbits)(&zghz3472, UINT64_C(0), UINT64_C(0) , true);
    append_64(&zghz3472, zghz3472, UINT64_C(0x0000000000000000));
    append_64(&zghz3472, zghz3472, UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bvz9(&zgsz3278, zgsz3278, INT64_C(30), zghz3472);
    RECREATE(lbits)(&zghz3473);
    CONVERT_OF(lbits, fbits)(&zghz3473, UINT64_C(0), UINT64_C(0) , true);
    append_64(&zghz3473, zghz3473, UINT64_C(0x0000000000000000));
    append_64(&zghz3473, zghz3473, UINT64_C(0x0000000000000000));
    internal_vector_update_zz5vecz8z5bvz9(&zgsz3278, zgsz3278, INT64_C(31), zghz3473);
    KILL(zz5vecz8z5bvz9)(&z_V);
    internal_vector_init_zz5vecz8z5bvz9(&z_V, zgsz3278.len);
    for (int zcbz384 = 0; zcbz384 < zgsz3278.len; zcbz384++) {
      lbits zcbz385;
      CREATE(lbits)(&zcbz385);
      COPY(lbits)(&zcbz385, zgsz3278.data[zcbz384]);
      lbits zcbz386;
      CREATE(lbits)(&zcbz386);
      COPY(lbits)(&zcbz386, zcbz385);
      COPY(lbits)(&((&z_V)->data[zcbz384]), zcbz386);
      KILL(lbits)(&zcbz386);
      KILL(lbits)(&zcbz385);
    }
    unit zgsz3316;
    zgsz3316 = UNIT;
    KILL(zz5vecz8z5bvz9)(&zgsz3278);
  }
  {
    z__saved_exception_level = UINT64_C(0b00);
    unit zgsz3315;
    zgsz3315 = UNIT;
  }
  {
    z__CNTControlBase = UINT64_C(0x000000000000);
    unit zgsz3314;
    zgsz3314 = UNIT;
  }
  {
    z__highest_el_aarch32 = false;
    unit zgsz3313;
    zgsz3313 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3441);
    CONVERT_OF(lbits, fbits)(&zghz3441, UINT64_C(0b0), UINT64_C(1) , true);
    append_64(&zghz3441, zghz3441, UINT64_C(0x0000000000000000));
    append_64(&zghz3441, zghz3441, UINT64_C(0x0000000000000000));
    COPY(lbits)(&zSP_EL3, zghz3441);
    unit zgsz3312;
    zgsz3312 = UNIT;
  }
  RECREATE(lbits)(&zghz3440);
  CONVERT_OF(lbits, fbits)(&zghz3440, UINT64_C(0b0), UINT64_C(1) , true);
  append_64(&zghz3440, zghz3440, UINT64_C(0x0000000000000000));
  append_64(&zghz3440, zghz3440, UINT64_C(0x0000000000000000));
  COPY(lbits)(&zTestCap, zghz3440);
  zcbz337 = UNIT;
end_function_275: ;
  return zcbz337;
end_block_exception_276: ;

  return UNIT;
}



static void finish_sailgen_initializze_registers(void)
{
  KILL(lbits)(&zghz3530);

  KILL(lbits)(&zghz3528);
  KILL(lbits)(&zghz3527);
  KILL(lbits)(&zghz3526);
  KILL(lbits)(&zghz3525);
  KILL(lbits)(&zghz3524);
  KILL(lbits)(&zghz3523);
  KILL(lbits)(&zghz3522);
  KILL(lbits)(&zghz3521);
  KILL(lbits)(&zghz3520);
  KILL(lbits)(&zghz3519);
  KILL(lbits)(&zghz3518);
  KILL(lbits)(&zghz3517);
  KILL(lbits)(&zghz3516);
  KILL(lbits)(&zghz3515);
  KILL(lbits)(&zghz3514);
  KILL(lbits)(&zghz3513);
  KILL(lbits)(&zghz3512);
  KILL(lbits)(&zghz3511);
  KILL(lbits)(&zghz3510);
  KILL(lbits)(&zghz3509);
  KILL(lbits)(&zghz3508);
  KILL(lbits)(&zghz3507);
  KILL(lbits)(&zghz3506);
  KILL(lbits)(&zghz3505);
  KILL(lbits)(&zghz3504);
  KILL(lbits)(&zghz3503);
  KILL(lbits)(&zghz3502);
  KILL(lbits)(&zghz3501);
  KILL(lbits)(&zghz3500);
  KILL(lbits)(&zghz3499);
  KILL(lbits)(&zghz3498);
  KILL(lbits)(&zghz3497);
  KILL(lbits)(&zghz3496);
  KILL(lbits)(&zghz3495);
  KILL(lbits)(&zghz3494);
  KILL(lbits)(&zghz3493);
  KILL(lbits)(&zghz3492);
  KILL(lbits)(&zghz3491);
  KILL(lbits)(&zghz3490);
  KILL(lbits)(&zghz3489);
  KILL(lbits)(&zghz3488);
  KILL(lbits)(&zghz3487);
  KILL(lbits)(&zghz3486);
  KILL(lbits)(&zghz3485);
  KILL(lbits)(&zghz3484);
  KILL(lbits)(&zghz3483);
  KILL(lbits)(&zghz3482);
  KILL(lbits)(&zghz3481);
  KILL(lbits)(&zghz3480);
  KILL(lbits)(&zghz3479);
  KILL(lbits)(&zghz3478);
  KILL(lbits)(&zghz3477);
  KILL(lbits)(&zghz3476);
  KILL(lbits)(&zghz3475);
  KILL(lbits)(&zghz3474);
  KILL(lbits)(&zghz3473);
  KILL(lbits)(&zghz3472);
  KILL(lbits)(&zghz3471);
  KILL(lbits)(&zghz3470);
  KILL(lbits)(&zghz3469);
  KILL(lbits)(&zghz3468);
  KILL(lbits)(&zghz3467);
  KILL(lbits)(&zghz3466);
  KILL(lbits)(&zghz3465);
  KILL(lbits)(&zghz3464);
  KILL(lbits)(&zghz3463);
  KILL(lbits)(&zghz3462);
  KILL(lbits)(&zghz3461);
  KILL(lbits)(&zghz3460);
  KILL(lbits)(&zghz3459);
  KILL(lbits)(&zghz3458);
  KILL(lbits)(&zghz3457);
  KILL(lbits)(&zghz3456);
  KILL(lbits)(&zghz3455);
  KILL(lbits)(&zghz3454);
  KILL(lbits)(&zghz3453);
  KILL(lbits)(&zghz3452);
  KILL(lbits)(&zghz3451);
  KILL(lbits)(&zghz3450);
  KILL(lbits)(&zghz3449);
  KILL(lbits)(&zghz3448);
  KILL(lbits)(&zghz3447);
  KILL(lbits)(&zghz3446);
  KILL(lbits)(&zghz3445);
  KILL(lbits)(&zghz3444);
  KILL(lbits)(&zghz3443);
  KILL(lbits)(&zghz3442);
  KILL(lbits)(&zghz3441);
  KILL(lbits)(&zghz3440);
}

static void model_init(void)
{
  setup_rts();
  current_exception = sail_new(struct zexception);
  CREATE(zexception)(current_exception);
  throw_location = sail_new(sail_string);
  CREATE(sail_string)(throw_location);
  startup_sailgen_sail_ones();
  startup_sailgen_eq_bits_int();
  startup_sailgen_integer_subrange();
  startup_sailgen_undefined_ProcState();
  startup_sailgen_CapBoundsUsesValue();
  startup_sailgen_CapIsInternalExponent();
  startup_sailgen_CapGetExponent();
  startup_sailgen_CapGetValue();
  startup_sailgen_CapBoundsAddress();
  startup_sailgen_CapGetBottom();
  startup_sailgen_CapUnsignedLessThan();
  startup_sailgen_CapGetTop();
  startup_sailgen_CapUnsignedGreaterThan();
  startup_sailgen_CapGetBounds();
  startup_sailgen_CapBoundsEqual();
  startup_sailgen_CapIsRepresentable();
  startup_sailgen_CapSetTag();
  startup_sailgen_CapWithTagClear();
  startup_sailgen_CapGetObjectType();
  startup_sailgen_CapIsSealed();
  startup_sailgen_CapGetPermissions();
  startup_sailgen_CapUnsignedGreaterThanOrEqual();
  startup_sailgen_CapIsRepresentableFast();
  startup_sailgen_CapUnsignedLessThanOrEqual();
  startup_sailgen_CapGetTag();
  startup_sailgen_CapSetBounds();
  startup_sailgen_doCSetBounds();
  startup_sailgen_main();
  startup_sailgen_initializze_registers();
  create_letbind_0();
  create_letbind_1();
  create_letbind_2();
  create_letbind_3();
  create_letbind_4();
  create_letbind_5();
  create_letbind_6();
  create_letbind_7();
  create_letbind_8();
  create_letbind_9();
  create_letbind_10();
  create_letbind_11();
  create_letbind_12();
  create_letbind_13();
  create_letbind_14();
  create_letbind_15();
  create_letbind_16();
  create_letbind_17();
  create_letbind_18();
  create_letbind_19();
  create_letbind_20();
  create_letbind_21();
  create_letbind_22();
  create_letbind_23();
  create_letbind_24();
  create_letbind_25();
  create_letbind_26();
  create_letbind_27();
  create_letbind_28();
  create_letbind_29();
  create_letbind_30();
  create_letbind_31();
  create_letbind_32();
  create_letbind_33();
  create_letbind_34();
  create_letbind_35();
  create_letbind_36();
  create_letbind_37();
  create_letbind_38();
  create_letbind_39();
  create_letbind_40();
  create_letbind_41();
  create_letbind_42();
  create_letbind_43();
  create_letbind_44();
  create_letbind_45();
  create_letbind_46();
  create_letbind_47();
  create_letbind_48();
  create_letbind_49();
  create_letbind_50();
  create_letbind_51();
  create_letbind_52();
  create_letbind_53();
  create_letbind_54();
  create_letbind_55();
  create_letbind_56();
  create_letbind_57();
  create_letbind_58();
  create_letbind_59();
  create_letbind_60();
  create_letbind_61();
  create_letbind_62();
  create_letbind_63();
  create_letbind_64();
  create_letbind_65();
  create_letbind_66();
  create_letbind_67();
  create_letbind_68();
  create_letbind_69();
  create_letbind_70();
  CREATE(sail_int)(&zSEE);
  CREATE(lbits)(&zPCC);
  CREATE(lbits)(&z_R00);
  CREATE(lbits)(&z_R01);
  CREATE(lbits)(&z_R02);
  CREATE(lbits)(&z_R03);
  CREATE(lbits)(&z_R04);
  CREATE(lbits)(&z_R05);
  CREATE(lbits)(&z_R06);
  CREATE(lbits)(&z_R07);
  CREATE(lbits)(&z_R08);
  CREATE(lbits)(&z_R09);
  CREATE(lbits)(&z_R10);
  CREATE(lbits)(&z_R11);
  CREATE(lbits)(&z_R12);
  CREATE(lbits)(&z_R13);
  CREATE(lbits)(&z_R14);
  CREATE(lbits)(&z_R15);
  CREATE(lbits)(&z_R16);
  CREATE(lbits)(&z_R17);
  CREATE(lbits)(&z_R18);
  CREATE(lbits)(&z_R19);
  CREATE(lbits)(&z_R20);
  CREATE(lbits)(&z_R21);
  CREATE(lbits)(&z_R22);
  CREATE(lbits)(&z_R23);
  CREATE(lbits)(&z_R24);
  CREATE(lbits)(&z_R25);
  CREATE(lbits)(&z_R26);
  CREATE(lbits)(&z_R27);
  CREATE(lbits)(&z_R28);
  CREATE(lbits)(&z_R29);
  CREATE(lbits)(&z_R30);
  CREATE(lbits)(&zELR_EL1);
  CREATE(lbits)(&zELR_EL2);
  CREATE(lbits)(&zELR_EL3);
  CREATE(lbits)(&zVBAR_EL1);
  CREATE(lbits)(&zVBAR_EL2);
  CREATE(lbits)(&zVBAR_EL3);
  CREATE(zz5vecz8z5bv32z9)(&zDBGBCR_EL1);
  CREATE(lbits)(&zCDLR_EL0);
  CREATE(zz5vecz8z5bv64z9)(&zDBGBVR_EL1);
  CREATE(lbits)(&zCDBGDTR_EL0);
  CREATE(zz5vecz8z5bv32z9)(&zDBGWCR_EL1);
  CREATE(zz5vecz8z5bv64z9)(&zDBGWVR_EL1);
  CREATE(zz5vecz8z5bv32z9)(&zICC_AP0R_EL1);
  CREATE(zz5vecz8z5bv32z9)(&zICV_AP0R_EL1);
  CREATE(zz5vecz8z5bv32z9)(&zICC_AP1R_EL1);
  CREATE(zz5vecz8z5bv32z9)(&zICC_AP1R_EL1_NS);
  CREATE(zz5vecz8z5bv32z9)(&zICC_AP1R_EL1_S);
  CREATE(zz5vecz8z5bv32z9)(&zICV_AP1R_EL1);
  CREATE(zz5vecz8z5bv32z9)(&zICH_AP0R_EL2);
  CREATE(zz5vecz8z5bv32z9)(&zICH_AP1R_EL2);
  CREATE(zz5vecz8z5bv64z9)(&zICH_LR_EL2);
  CREATE(zz5vecz8z5bv32z9)(&zPMEVCNTR_EL0);
  CREATE(zz5vecz8z5bv32z9)(&zPMEVTYPER_EL0);
  CREATE(lbits)(&zRSP_EL0);
  CREATE(lbits)(&zRTPIDR_EL0);
  CREATE(lbits)(&zCID_EL0);
  CREATE(lbits)(&zSP_EL0);
  CREATE(lbits)(&zSP_EL1);
  CREATE(lbits)(&zSP_EL2);
  CREATE(lbits)(&zTPIDRRO_EL0);
  CREATE(lbits)(&zTPIDR_EL0);
  CREATE(lbits)(&zTPIDR_EL1);
  CREATE(lbits)(&zTPIDR_EL2);
  CREATE(lbits)(&zTPIDR_EL3);
  CREATE(lbits)(&zDDC_EL0);
  CREATE(lbits)(&zDDC_EL1);
  CREATE(lbits)(&zDDC_EL2);
  CREATE(lbits)(&zDDC_EL3);
  CREATE(lbits)(&zRDDC_EL0);
  CREATE(zz5vecz8z5bvz9)(&z_V);
  CREATE(lbits)(&zSP_EL3);
  CREATE(lbits)(&zTestCap);
  sailgen_initializze_registers(UNIT);
}

static void model_fini(void)
{
  kill_letbind_70();
  kill_letbind_69();
  kill_letbind_68();
  kill_letbind_67();
  kill_letbind_66();
  kill_letbind_65();
  kill_letbind_64();
  kill_letbind_63();
  kill_letbind_62();
  kill_letbind_61();
  kill_letbind_60();
  kill_letbind_59();
  kill_letbind_58();
  kill_letbind_57();
  kill_letbind_56();
  kill_letbind_55();
  kill_letbind_54();
  kill_letbind_53();
  kill_letbind_52();
  kill_letbind_51();
  kill_letbind_50();
  kill_letbind_49();
  kill_letbind_48();
  kill_letbind_47();
  kill_letbind_46();
  kill_letbind_45();
  kill_letbind_44();
  kill_letbind_43();
  kill_letbind_42();
  kill_letbind_41();
  kill_letbind_40();
  kill_letbind_39();
  kill_letbind_38();
  kill_letbind_37();
  kill_letbind_36();
  kill_letbind_35();
  kill_letbind_34();
  kill_letbind_33();
  kill_letbind_32();
  kill_letbind_31();
  kill_letbind_30();
  kill_letbind_29();
  kill_letbind_28();
  kill_letbind_27();
  kill_letbind_26();
  kill_letbind_25();
  kill_letbind_24();
  kill_letbind_23();
  kill_letbind_22();
  kill_letbind_21();
  kill_letbind_20();
  kill_letbind_19();
  kill_letbind_18();
  kill_letbind_17();
  kill_letbind_16();
  kill_letbind_15();
  kill_letbind_14();
  kill_letbind_13();
  kill_letbind_12();
  kill_letbind_11();
  kill_letbind_10();
  kill_letbind_9();
  kill_letbind_8();
  kill_letbind_7();
  kill_letbind_6();
  kill_letbind_5();
  kill_letbind_4();
  kill_letbind_3();
  kill_letbind_2();
  kill_letbind_1();
  kill_letbind_0();
  KILL(sail_int)(&zSEE);
  KILL(lbits)(&zPCC);
  KILL(lbits)(&z_R00);
  KILL(lbits)(&z_R01);
  KILL(lbits)(&z_R02);
  KILL(lbits)(&z_R03);
  KILL(lbits)(&z_R04);
  KILL(lbits)(&z_R05);
  KILL(lbits)(&z_R06);
  KILL(lbits)(&z_R07);
  KILL(lbits)(&z_R08);
  KILL(lbits)(&z_R09);
  KILL(lbits)(&z_R10);
  KILL(lbits)(&z_R11);
  KILL(lbits)(&z_R12);
  KILL(lbits)(&z_R13);
  KILL(lbits)(&z_R14);
  KILL(lbits)(&z_R15);
  KILL(lbits)(&z_R16);
  KILL(lbits)(&z_R17);
  KILL(lbits)(&z_R18);
  KILL(lbits)(&z_R19);
  KILL(lbits)(&z_R20);
  KILL(lbits)(&z_R21);
  KILL(lbits)(&z_R22);
  KILL(lbits)(&z_R23);
  KILL(lbits)(&z_R24);
  KILL(lbits)(&z_R25);
  KILL(lbits)(&z_R26);
  KILL(lbits)(&z_R27);
  KILL(lbits)(&z_R28);
  KILL(lbits)(&z_R29);
  KILL(lbits)(&z_R30);
  KILL(lbits)(&zELR_EL1);
  KILL(lbits)(&zELR_EL2);
  KILL(lbits)(&zELR_EL3);
  KILL(lbits)(&zVBAR_EL1);
  KILL(lbits)(&zVBAR_EL2);
  KILL(lbits)(&zVBAR_EL3);
  KILL(zz5vecz8z5bv32z9)(&zDBGBCR_EL1);
  KILL(lbits)(&zCDLR_EL0);
  KILL(zz5vecz8z5bv64z9)(&zDBGBVR_EL1);
  KILL(lbits)(&zCDBGDTR_EL0);
  KILL(zz5vecz8z5bv32z9)(&zDBGWCR_EL1);
  KILL(zz5vecz8z5bv64z9)(&zDBGWVR_EL1);
  KILL(zz5vecz8z5bv32z9)(&zICC_AP0R_EL1);
  KILL(zz5vecz8z5bv32z9)(&zICV_AP0R_EL1);
  KILL(zz5vecz8z5bv32z9)(&zICC_AP1R_EL1);
  KILL(zz5vecz8z5bv32z9)(&zICC_AP1R_EL1_NS);
  KILL(zz5vecz8z5bv32z9)(&zICC_AP1R_EL1_S);
  KILL(zz5vecz8z5bv32z9)(&zICV_AP1R_EL1);
  KILL(zz5vecz8z5bv32z9)(&zICH_AP0R_EL2);
  KILL(zz5vecz8z5bv32z9)(&zICH_AP1R_EL2);
  KILL(zz5vecz8z5bv64z9)(&zICH_LR_EL2);
  KILL(zz5vecz8z5bv32z9)(&zPMEVCNTR_EL0);
  KILL(zz5vecz8z5bv32z9)(&zPMEVTYPER_EL0);
  KILL(lbits)(&zRSP_EL0);
  KILL(lbits)(&zRTPIDR_EL0);
  KILL(lbits)(&zCID_EL0);
  KILL(lbits)(&zSP_EL0);
  KILL(lbits)(&zSP_EL1);
  KILL(lbits)(&zSP_EL2);
  KILL(lbits)(&zTPIDRRO_EL0);
  KILL(lbits)(&zTPIDR_EL0);
  KILL(lbits)(&zTPIDR_EL1);
  KILL(lbits)(&zTPIDR_EL2);
  KILL(lbits)(&zTPIDR_EL3);
  KILL(lbits)(&zDDC_EL0);
  KILL(lbits)(&zDDC_EL1);
  KILL(lbits)(&zDDC_EL2);
  KILL(lbits)(&zDDC_EL3);
  KILL(lbits)(&zRDDC_EL0);
  KILL(zz5vecz8z5bvz9)(&z_V);
  KILL(lbits)(&zSP_EL3);
  KILL(lbits)(&zTestCap);
  finish_sailgen_sail_ones();
  finish_sailgen_eq_bits_int();
  finish_sailgen_integer_subrange();
  finish_sailgen_undefined_ProcState();
  finish_sailgen_CapBoundsUsesValue();
  finish_sailgen_CapIsInternalExponent();
  finish_sailgen_CapGetExponent();
  finish_sailgen_CapGetValue();
  finish_sailgen_CapBoundsAddress();
  finish_sailgen_CapGetBottom();
  finish_sailgen_CapUnsignedLessThan();
  finish_sailgen_CapGetTop();
  finish_sailgen_CapUnsignedGreaterThan();
  finish_sailgen_CapGetBounds();
  finish_sailgen_CapBoundsEqual();
  finish_sailgen_CapIsRepresentable();
  finish_sailgen_CapSetTag();
  finish_sailgen_CapWithTagClear();
  finish_sailgen_CapGetObjectType();
  finish_sailgen_CapIsSealed();
  finish_sailgen_CapGetPermissions();
  finish_sailgen_CapUnsignedGreaterThanOrEqual();
  finish_sailgen_CapIsRepresentableFast();
  finish_sailgen_CapUnsignedLessThanOrEqual();
  finish_sailgen_CapGetTag();
  finish_sailgen_CapSetBounds();
  finish_sailgen_doCSetBounds();
  finish_sailgen_main();
  finish_sailgen_initializze_registers();
  cleanup_rts();
  if (have_exception) {fprintf(stderr, "Exiting due to uncaught exception: %s\n", *throw_location);}
  KILL(zexception)(current_exception);
  sail_free(current_exception);
  KILL(sail_string)(throw_location);
  sail_free(throw_location);
  if (have_exception) {exit(EXIT_FAILURE);}
}

void model_pre_exit()
{
}

static int model_main(int argc, char *argv[])
{
  model_init();
  if (process_arguments(argc, argv)) exit(EXIT_FAILURE);
  sailgen_main(UNIT);
  model_fini();
  model_pre_exit();
  return EXIT_SUCCESS;
}



#ifdef __cplusplus
}
#endif
