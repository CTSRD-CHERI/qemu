#include "sail.h"
#include "sail_config.h"
#include "rts.h"
#include "elf.h"
#ifdef __cplusplus
extern "C" {
#endif
void (*sail_rts_set_coverage_file)(const char *) = NULL;

// type abbreviation xlenbits
typedef uint64_t zxlenbits;

// union option<o>
enum kind_zoptionzIozK { Kind_zNonezIozK, Kind_zSomezIozK };

struct zoptionzIozK {
  enum kind_zoptionzIozK kind;
  union {
    struct { unit zNonezIozK; };
    struct { bool zSomezIozK; };
  } variants;
};

static void CREATE(zoptionzIozK)(struct zoptionzIozK *op) {
  op->kind = Kind_zNonezIozK;
}

static void RECREATE(zoptionzIozK)(struct zoptionzIozK *op) {

}

static void KILL(zoptionzIozK)(struct zoptionzIozK *op) {
  {}
}

static void COPY(zoptionzIozK)(struct zoptionzIozK *rop, struct zoptionzIozK op) {
  {};
  rop->kind = op.kind;
  if (op.kind == Kind_zNonezIozK) {
    rop->variants.zNonezIozK = op.variants.zNonezIozK;
  } else if (op.kind == Kind_zSomezIozK) {
    rop->variants.zSomezIozK = op.variants.zSomezIozK;
  }
}

static bool EQUAL(zoptionzIozK)(struct zoptionzIozK op1, struct zoptionzIozK op2) {
  if (op1.kind == Kind_zNonezIozK && op2.kind == Kind_zNonezIozK) {
    return EQUAL(unit)(op1.variants.zNonezIozK, op2.variants.zNonezIozK);
  } else if (op1.kind == Kind_zSomezIozK && op2.kind == Kind_zSomezIozK) {
    return EQUAL(bool)(op1.variants.zSomezIozK, op2.variants.zSomezIozK);
  } else return false;
}

static void sailgen_NonezIozK(struct zoptionzIozK *rop, unit op) {
  {}
  rop->kind = Kind_zNonezIozK;
  rop->variants.zNonezIozK = op;
}

static void sailgen_SomezIozK(struct zoptionzIozK *rop, bool op) {
  {}
  rop->kind = Kind_zSomezIozK;
  rop->variants.zSomezIozK = op;
}

// struct tuple_(%i, %i)
struct ztuple_z8z5izCz0z5iz9 {
  sail_int ztup0;
  sail_int ztup1;
};

static void COPY(ztuple_z8z5izCz0z5iz9)(struct ztuple_z8z5izCz0z5iz9 *rop, const struct ztuple_z8z5izCz0z5iz9 op) {
  COPY(sail_int)(&rop->ztup0, op.ztup0);
  COPY(sail_int)(&rop->ztup1, op.ztup1);
}

static void CREATE(ztuple_z8z5izCz0z5iz9)(struct ztuple_z8z5izCz0z5iz9 *op) {
  CREATE(sail_int)(&op->ztup0);
  CREATE(sail_int)(&op->ztup1);
}

static void RECREATE(ztuple_z8z5izCz0z5iz9)(struct ztuple_z8z5izCz0z5iz9 *op) {
  RECREATE(sail_int)(&op->ztup0);
  RECREATE(sail_int)(&op->ztup1);
}

static void KILL(ztuple_z8z5izCz0z5iz9)(struct ztuple_z8z5izCz0z5iz9 *op) {
  KILL(sail_int)(&op->ztup0);
  KILL(sail_int)(&op->ztup1);
}

static bool EQUAL(ztuple_z8z5izCz0z5iz9)(struct ztuple_z8z5izCz0z5iz9 op1, struct ztuple_z8z5izCz0z5iz9 op2) {
  return EQUAL(sail_int)(op1.ztup0, op2.ztup0) && EQUAL(sail_int)(op1.ztup1, op2.ztup1);
}

// union option<(i,i)>
enum kind_zoptionzIz8izCiz9zK { Kind_zNonezIz8izCiz9zK, Kind_zSomezIz8izCiz9zK };

struct zoptionzIz8izCiz9zK {
  enum kind_zoptionzIz8izCiz9zK kind;
  union {
    struct { unit zNonezIz8izCiz9zK; };
    struct { struct ztuple_z8z5izCz0z5iz9 zSomezIz8izCiz9zK; };
  } variants;
};

static void CREATE(zoptionzIz8izCiz9zK)(struct zoptionzIz8izCiz9zK *op) {
  op->kind = Kind_zNonezIz8izCiz9zK;
}

static void RECREATE(zoptionzIz8izCiz9zK)(struct zoptionzIz8izCiz9zK *op) {

}

static void KILL(zoptionzIz8izCiz9zK)(struct zoptionzIz8izCiz9zK *op) {
  if (op->kind == Kind_zSomezIz8izCiz9zK) {
    KILL(ztuple_z8z5izCz0z5iz9)(&op->variants.zSomezIz8izCiz9zK);
  }
}

static void COPY(zoptionzIz8izCiz9zK)(struct zoptionzIz8izCiz9zK *rop, struct zoptionzIz8izCiz9zK op) {
  if (rop->kind == Kind_zSomezIz8izCiz9zK) {
    KILL(ztuple_z8z5izCz0z5iz9)(&rop->variants.zSomezIz8izCiz9zK);
  };
  rop->kind = op.kind;
  if (op.kind == Kind_zNonezIz8izCiz9zK) {
    rop->variants.zNonezIz8izCiz9zK = op.variants.zNonezIz8izCiz9zK;
  } else if (op.kind == Kind_zSomezIz8izCiz9zK) {
    CREATE(ztuple_z8z5izCz0z5iz9)(&rop->variants.zSomezIz8izCiz9zK); COPY(ztuple_z8z5izCz0z5iz9)(&rop->variants.zSomezIz8izCiz9zK, op.variants.zSomezIz8izCiz9zK);
  }
}

static bool EQUAL(zoptionzIz8izCiz9zK)(struct zoptionzIz8izCiz9zK op1, struct zoptionzIz8izCiz9zK op2) {
  if (op1.kind == Kind_zNonezIz8izCiz9zK && op2.kind == Kind_zNonezIz8izCiz9zK) {
    return EQUAL(unit)(op1.variants.zNonezIz8izCiz9zK, op2.variants.zNonezIz8izCiz9zK);
  } else if (op1.kind == Kind_zSomezIz8izCiz9zK && op2.kind == Kind_zSomezIz8izCiz9zK) {
    return EQUAL(ztuple_z8z5izCz0z5iz9)(op1.variants.zSomezIz8izCiz9zK, op2.variants.zSomezIz8izCiz9zK);
  } else return false;
}

static void sailgen_NonezIz8izCiz9zK(struct zoptionzIz8izCiz9zK *rop, unit op) {
  if (rop->kind == Kind_zSomezIz8izCiz9zK) {
    KILL(ztuple_z8z5izCz0z5iz9)(&rop->variants.zSomezIz8izCiz9zK);
  }
  rop->kind = Kind_zNonezIz8izCiz9zK;
  rop->variants.zNonezIz8izCiz9zK = op;
}

static void sailgen_SomezIz8izCiz9zK(struct zoptionzIz8izCiz9zK *rop, struct ztuple_z8z5izCz0z5iz9 op) {
  if (rop->kind == Kind_zSomezIz8izCiz9zK) {
    KILL(ztuple_z8z5izCz0z5iz9)(&rop->variants.zSomezIz8izCiz9zK);
  }
  rop->kind = Kind_zSomezIz8izCiz9zK;
  CREATE(ztuple_z8z5izCz0z5iz9)(&rop->variants.zSomezIz8izCiz9zK);
  COPY(ztuple_z8z5izCz0z5iz9)(&rop->variants.zSomezIz8izCiz9zK, op);
}

// struct tuple_(%bv, %bv)
struct ztuple_z8z5bvzCz0z5bvz9 {
  lbits ztup0;
  lbits ztup1;
};

static void COPY(ztuple_z8z5bvzCz0z5bvz9)(struct ztuple_z8z5bvzCz0z5bvz9 *rop, const struct ztuple_z8z5bvzCz0z5bvz9 op) {
  COPY(lbits)(&rop->ztup0, op.ztup0);
  COPY(lbits)(&rop->ztup1, op.ztup1);
}

static void CREATE(ztuple_z8z5bvzCz0z5bvz9)(struct ztuple_z8z5bvzCz0z5bvz9 *op) {
  CREATE(lbits)(&op->ztup0);
  CREATE(lbits)(&op->ztup1);
}

static void RECREATE(ztuple_z8z5bvzCz0z5bvz9)(struct ztuple_z8z5bvzCz0z5bvz9 *op) {
  RECREATE(lbits)(&op->ztup0);
  RECREATE(lbits)(&op->ztup1);
}

static void KILL(ztuple_z8z5bvzCz0z5bvz9)(struct ztuple_z8z5bvzCz0z5bvz9 *op) {
  KILL(lbits)(&op->ztup0);
  KILL(lbits)(&op->ztup1);
}

static bool EQUAL(ztuple_z8z5bvzCz0z5bvz9)(struct ztuple_z8z5bvzCz0z5bvz9 op1, struct ztuple_z8z5bvzCz0z5bvz9 op2) {
  return EQUAL(lbits)(op1.ztup0, op2.ztup0) && EQUAL(lbits)(op1.ztup1, op2.ztup1);
}

// union option<(b,b)>
enum kind_zoptionzIz8bzCbz9zK { Kind_zNonezIz8bzCbz9zK, Kind_zSomezIz8bzCbz9zK };

struct zoptionzIz8bzCbz9zK {
  enum kind_zoptionzIz8bzCbz9zK kind;
  union {
    struct { unit zNonezIz8bzCbz9zK; };
    struct { struct ztuple_z8z5bvzCz0z5bvz9 zSomezIz8bzCbz9zK; };
  } variants;
};

static void CREATE(zoptionzIz8bzCbz9zK)(struct zoptionzIz8bzCbz9zK *op) {
  op->kind = Kind_zNonezIz8bzCbz9zK;
}

static void RECREATE(zoptionzIz8bzCbz9zK)(struct zoptionzIz8bzCbz9zK *op) {

}

static void KILL(zoptionzIz8bzCbz9zK)(struct zoptionzIz8bzCbz9zK *op) {
  if (op->kind == Kind_zSomezIz8bzCbz9zK) {
    KILL(ztuple_z8z5bvzCz0z5bvz9)(&op->variants.zSomezIz8bzCbz9zK);
  }
}

static void COPY(zoptionzIz8bzCbz9zK)(struct zoptionzIz8bzCbz9zK *rop, struct zoptionzIz8bzCbz9zK op) {
  if (rop->kind == Kind_zSomezIz8bzCbz9zK) {
    KILL(ztuple_z8z5bvzCz0z5bvz9)(&rop->variants.zSomezIz8bzCbz9zK);
  };
  rop->kind = op.kind;
  if (op.kind == Kind_zNonezIz8bzCbz9zK) {
    rop->variants.zNonezIz8bzCbz9zK = op.variants.zNonezIz8bzCbz9zK;
  } else if (op.kind == Kind_zSomezIz8bzCbz9zK) {
    CREATE(ztuple_z8z5bvzCz0z5bvz9)(&rop->variants.zSomezIz8bzCbz9zK); COPY(ztuple_z8z5bvzCz0z5bvz9)(&rop->variants.zSomezIz8bzCbz9zK, op.variants.zSomezIz8bzCbz9zK);
  }
}

static bool EQUAL(zoptionzIz8bzCbz9zK)(struct zoptionzIz8bzCbz9zK op1, struct zoptionzIz8bzCbz9zK op2) {
  if (op1.kind == Kind_zNonezIz8bzCbz9zK && op2.kind == Kind_zNonezIz8bzCbz9zK) {
    return EQUAL(unit)(op1.variants.zNonezIz8bzCbz9zK, op2.variants.zNonezIz8bzCbz9zK);
  } else if (op1.kind == Kind_zSomezIz8bzCbz9zK && op2.kind == Kind_zSomezIz8bzCbz9zK) {
    return EQUAL(ztuple_z8z5bvzCz0z5bvz9)(op1.variants.zSomezIz8bzCbz9zK, op2.variants.zSomezIz8bzCbz9zK);
  } else return false;
}

static void sailgen_NonezIz8bzCbz9zK(struct zoptionzIz8bzCbz9zK *rop, unit op) {
  if (rop->kind == Kind_zSomezIz8bzCbz9zK) {
    KILL(ztuple_z8z5bvzCz0z5bvz9)(&rop->variants.zSomezIz8bzCbz9zK);
  }
  rop->kind = Kind_zNonezIz8bzCbz9zK;
  rop->variants.zNonezIz8bzCbz9zK = op;
}

static void sailgen_SomezIz8bzCbz9zK(struct zoptionzIz8bzCbz9zK *rop, struct ztuple_z8z5bvzCz0z5bvz9 op) {
  if (rop->kind == Kind_zSomezIz8bzCbz9zK) {
    KILL(ztuple_z8z5bvzCz0z5bvz9)(&rop->variants.zSomezIz8bzCbz9zK);
  }
  rop->kind = Kind_zSomezIz8bzCbz9zK;
  CREATE(ztuple_z8z5bvzCz0z5bvz9)(&rop->variants.zSomezIz8bzCbz9zK);
  COPY(ztuple_z8z5bvzCz0z5bvz9)(&rop->variants.zSomezIz8bzCbz9zK, op);
}

// union exception
enum kind_zexception { Kind_z__dummy_exnz3 };

struct zexception {
  enum kind_zexception kind;
  union {struct { unit z__dummy_exnz3; };} variants;
};

static void CREATE(zexception)(struct zexception *op) {
  op->kind = Kind_z__dummy_exnz3;
}

static void RECREATE(zexception)(struct zexception *op) {

}

static void KILL(zexception)(struct zexception *op) {
  {}
}

static void COPY(zexception)(struct zexception *rop, struct zexception op) {
  {};
  rop->kind = op.kind;
  if (op.kind == Kind_z__dummy_exnz3) {
    rop->variants.z__dummy_exnz3 = op.variants.z__dummy_exnz3;
  }
}

static bool EQUAL(zexception)(struct zexception op1, struct zexception op2) {
  if (op1.kind == Kind_z__dummy_exnz3 && op2.kind == Kind_z__dummy_exnz3) {
    return EQUAL(unit)(op1.variants.z__dummy_exnz3, op2.variants.z__dummy_exnz3);
  } else return false;
}

static void sailgen___dummy_exnz3(struct zexception *rop, unit op) {
  {}
  rop->kind = Kind_z__dummy_exnz3;
  rop->variants.z__dummy_exnz3 = op;
}

struct zexception *current_exception = NULL;

bool have_exception = false;

sail_string *throw_location = NULL;

// enum ExecutionMode
enum zExecutionMode { zIntPtrMode, zCapPtrMode };

static bool EQUAL(zExecutionMode)(enum zExecutionMode op1, enum zExecutionMode op2) {
  return op1 == op2;
}

static enum zExecutionMode UNDEFINED(zExecutionMode)(unit u) { return zIntPtrMode; }

// struct EncMetadata
struct zEncMetadata {uint64_t zbits;};

static void COPY(zEncMetadata)(struct zEncMetadata *rop, const struct zEncMetadata op) {
  rop->zbits = op.zbits;
}

static bool EQUAL(zEncMetadata)(struct zEncMetadata op1, struct zEncMetadata op2) {
  return EQUAL(fbits)(op1.zbits, op2.zbits);
}

// enum ClearRegSet
enum zClearRegSet { zGPRegs, zFPRegs };

static bool EQUAL(zClearRegSet)(enum zClearRegSet op1, enum zClearRegSet op2) {
  return op1 == op2;
}

static enum zClearRegSet UNDEFINED(zClearRegSet)(unit u) { return zGPRegs; }

// struct Capability
struct zCapability {
  uint64_t zB;
  int64_t zE;
  uint64_t zT;
  uint64_t zaddress;
  uint64_t zap_m;
  uint64_t zcl;
  bool zinternal_exponent;
  uint64_t zreserved_0;
  uint64_t zreserved_1;
  uint64_t zsd_perms;
  bool zsealed;
  bool ztag;
};

static void COPY(zCapability)(struct zCapability *rop, const struct zCapability op) {
  rop->zB = op.zB;
  rop->zE = op.zE;
  rop->zT = op.zT;
  rop->zaddress = op.zaddress;
  rop->zap_m = op.zap_m;
  rop->zcl = op.zcl;
  rop->zinternal_exponent = op.zinternal_exponent;
  rop->zreserved_0 = op.zreserved_0;
  rop->zreserved_1 = op.zreserved_1;
  rop->zsd_perms = op.zsd_perms;
  rop->zsealed = op.zsealed;
  rop->ztag = op.ztag;
}

static bool EQUAL(zCapability)(struct zCapability op1, struct zCapability op2) {
  return EQUAL(fbits)(op1.zB, op2.zB) && EQUAL(mach_int)(op1.zE, op2.zE) && EQUAL(fbits)(op1.zT, op2.zT) && EQUAL(fbits)(op1.zaddress, op2.zaddress) && EQUAL(fbits)(op1.zap_m, op2.zap_m) && EQUAL(fbits)(op1.zcl, op2.zcl) && EQUAL(bool)(op1.zinternal_exponent, op2.zinternal_exponent) && EQUAL(fbits)(op1.zreserved_0, op2.zreserved_0) && EQUAL(fbits)(op1.zreserved_1, op2.zreserved_1) && EQUAL(fbits)(op1.zsd_perms, op2.zsd_perms) && EQUAL(bool)(op1.zsealed, op2.zsealed) && EQUAL(bool)(op1.ztag, op2.ztag);
}

// type abbreviation CapLenInt
typedef sail_int zCapLenInt;

// type abbreviation CapLenBits
typedef lbits zCapLenBits;

// type abbreviation CapExponent
typedef int64_t zCapExponent;

// enum CapEx
enum zCapEx { zCapEx_TagViolation, zCapEx_SealViolation, zCapEx_PermissionViolation, zCapEx_InvalidAddressViolation, zCapEx_LengthViolation };

static bool EQUAL(zCapEx)(enum zCapEx op1, enum zCapEx op2) {
  return op1 == op2;
}

static enum zCapEx UNDEFINED(zCapEx)(unit u) { return zCapEx_TagViolation; }

// enum CapCheckType
enum zCapCheckType { zCapCheckType_Inst, zCapCheckType_Data, zCapCheckType_JBr };

static bool EQUAL(zCapCheckType)(enum zCapCheckType op1, enum zCapCheckType op2) {
  return op1 == op2;
}

static enum zCapCheckType UNDEFINED(zCapCheckType)(unit u) { return zCapCheckType_Inst; }

// type abbreviation CapBits
typedef lbits zCapBits;

// type abbreviation CapAddrBits
typedef uint64_t zCapAddrBits;

// enum CPtrCmpOp
enum zCPtrCmpOp { zCEQ, zCNE, zCLT, zCLE, zCLTU, zCLEU, zCEXEQ, zCNEXEQ };

static bool EQUAL(zCPtrCmpOp)(enum zCPtrCmpOp op1, enum zCPtrCmpOp op2) {
  return op1 == op2;
}

static enum zCPtrCmpOp UNDEFINED(zCPtrCmpOp)(unit u) { return zCEQ; }

// struct ArchPerms
struct zArchPerms {
  bool zASR;
  bool zC;
  bool zEL;
  bool zLM;
  bool zR;
  bool zSL;
  bool zW;
  bool zX;
};

static void COPY(zArchPerms)(struct zArchPerms *rop, const struct zArchPerms op) {
  rop->zASR = op.zASR;
  rop->zC = op.zC;
  rop->zEL = op.zEL;
  rop->zLM = op.zLM;
  rop->zR = op.zR;
  rop->zSL = op.zSL;
  rop->zW = op.zW;
  rop->zX = op.zX;
}

static bool EQUAL(zArchPerms)(struct zArchPerms op1, struct zArchPerms op2) {
  return EQUAL(bool)(op1.zASR, op2.zASR) && EQUAL(bool)(op1.zC, op2.zC) && EQUAL(bool)(op1.zEL, op2.zEL) && EQUAL(bool)(op1.zLM, op2.zLM) && EQUAL(bool)(op1.zR, op2.zR) && EQUAL(bool)(op1.zSL, op2.zSL) && EQUAL(bool)(op1.zW, op2.zW) && EQUAL(bool)(op1.zX, op2.zX);
}

// union option<RArchPerms>
enum kind_zoptionzIRArchPermszK { Kind_zNonezIRArchPermszK, Kind_zSomezIRArchPermszK };

struct zoptionzIRArchPermszK {
  enum kind_zoptionzIRArchPermszK kind;
  union {
    struct { unit zNonezIRArchPermszK; };
    struct { struct zArchPerms zSomezIRArchPermszK; };
  } variants;
};

static void CREATE(zoptionzIRArchPermszK)(struct zoptionzIRArchPermszK *op) {
  op->kind = Kind_zNonezIRArchPermszK;
}

static void RECREATE(zoptionzIRArchPermszK)(struct zoptionzIRArchPermszK *op) {

}

static void KILL(zoptionzIRArchPermszK)(struct zoptionzIRArchPermszK *op) {
  {}
}

static void COPY(zoptionzIRArchPermszK)(struct zoptionzIRArchPermszK *rop, struct zoptionzIRArchPermszK op) {
  {};
  rop->kind = op.kind;
  if (op.kind == Kind_zNonezIRArchPermszK) {
    rop->variants.zNonezIRArchPermszK = op.variants.zNonezIRArchPermszK;
  } else if (op.kind == Kind_zSomezIRArchPermszK) {
    rop->variants.zSomezIRArchPermszK = op.variants.zSomezIRArchPermszK;
  }
}

static bool EQUAL(zoptionzIRArchPermszK)(struct zoptionzIRArchPermszK op1, struct zoptionzIRArchPermszK op2) {
  if (op1.kind == Kind_zNonezIRArchPermszK && op2.kind == Kind_zNonezIRArchPermszK) {
    return EQUAL(unit)(op1.variants.zNonezIRArchPermszK, op2.variants.zNonezIRArchPermszK);
  } else if (op1.kind == Kind_zSomezIRArchPermszK && op2.kind == Kind_zSomezIRArchPermszK) {
    return EQUAL(zArchPerms)(op1.variants.zSomezIRArchPermszK, op2.variants.zSomezIRArchPermszK);
  } else return false;
}

static void sailgen_NonezIRArchPermszK(struct zoptionzIRArchPermszK *rop, unit op) {
  {}
  rop->kind = Kind_zNonezIRArchPermszK;
  rop->variants.zNonezIRArchPermszK = op;
}

static void sailgen_SomezIRArchPermszK(struct zoptionzIRArchPermszK *rop, struct zArchPerms op) {
  {}
  rop->kind = Kind_zSomezIRArchPermszK;
  rop->variants.zSomezIRArchPermszK = op;
}

// struct tuple_(%struct zArchPerms, %enum zExecutionMode)
struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 {
  struct zArchPerms ztup0;
  enum zExecutionMode ztup1;
};

static void COPY(ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9)(struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 *rop, const struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 op) {
  rop->ztup0 = op.ztup0;
  rop->ztup1 = op.ztup1;
}

static bool EQUAL(ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9)(struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 op1, struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 op2) {
  return EQUAL(zArchPerms)(op1.ztup0, op2.ztup0) && EQUAL(zExecutionMode)(op1.ztup1, op2.ztup1);
}

// union option<(RArchPerms,EExecutionMode%)>
enum kind_zoptionzIz8RArchPermszCEExecutionModez5z9zK { Kind_zNonezIz8RArchPermszCEExecutionModez5z9zK, Kind_zSomezIz8RArchPermszCEExecutionModez5z9zK };

struct zoptionzIz8RArchPermszCEExecutionModez5z9zK {
  enum kind_zoptionzIz8RArchPermszCEExecutionModez5z9zK kind;
  union {
    struct { unit zNonezIz8RArchPermszCEExecutionModez5z9zK; };
    struct { struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zSomezIz8RArchPermszCEExecutionModez5z9zK; };
  } variants;
};

static void CREATE(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(struct zoptionzIz8RArchPermszCEExecutionModez5z9zK *op) {
  op->kind = Kind_zNonezIz8RArchPermszCEExecutionModez5z9zK;
}

static void RECREATE(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(struct zoptionzIz8RArchPermszCEExecutionModez5z9zK *op) {

}

static void KILL(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(struct zoptionzIz8RArchPermszCEExecutionModez5z9zK *op) {
  {}
}

static void COPY(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(struct zoptionzIz8RArchPermszCEExecutionModez5z9zK *rop, struct zoptionzIz8RArchPermszCEExecutionModez5z9zK op) {
  {};
  rop->kind = op.kind;
  if (op.kind == Kind_zNonezIz8RArchPermszCEExecutionModez5z9zK) {
    rop->variants.zNonezIz8RArchPermszCEExecutionModez5z9zK = op.variants.zNonezIz8RArchPermszCEExecutionModez5z9zK;
  } else if (op.kind == Kind_zSomezIz8RArchPermszCEExecutionModez5z9zK) {
    rop->variants.zSomezIz8RArchPermszCEExecutionModez5z9zK = op.variants.zSomezIz8RArchPermszCEExecutionModez5z9zK;
  }
}

static bool EQUAL(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(struct zoptionzIz8RArchPermszCEExecutionModez5z9zK op1, struct zoptionzIz8RArchPermszCEExecutionModez5z9zK op2) {
  if (op1.kind == Kind_zNonezIz8RArchPermszCEExecutionModez5z9zK && op2.kind == Kind_zNonezIz8RArchPermszCEExecutionModez5z9zK) {
    return EQUAL(unit)(op1.variants.zNonezIz8RArchPermszCEExecutionModez5z9zK, op2.variants.zNonezIz8RArchPermszCEExecutionModez5z9zK);
  } else if (op1.kind == Kind_zSomezIz8RArchPermszCEExecutionModez5z9zK && op2.kind == Kind_zSomezIz8RArchPermszCEExecutionModez5z9zK) {
    return EQUAL(ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9)(op1.variants.zSomezIz8RArchPermszCEExecutionModez5z9zK, op2.variants.zSomezIz8RArchPermszCEExecutionModez5z9zK);
  } else return false;
}

static void sailgen_NonezIz8RArchPermszCEExecutionModez5z9zK(struct zoptionzIz8RArchPermszCEExecutionModez5z9zK *rop, unit op) {
  {}
  rop->kind = Kind_zNonezIz8RArchPermszCEExecutionModez5z9zK;
  rop->variants.zNonezIz8RArchPermszCEExecutionModez5z9zK = op;
}

static void sailgen_SomezIz8RArchPermszCEExecutionModez5z9zK(struct zoptionzIz8RArchPermszCEExecutionModez5z9zK *rop, struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 op) {
  {}
  rop->kind = Kind_zSomezIz8RArchPermszCEExecutionModez5z9zK;
  rop->variants.zSomezIz8RArchPermszCEExecutionModez5z9zK = op;
}

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

static bool sailgen_neq_bits(lbits zx, lbits zy)
{
  __label__ end_function_4, end_block_exception_5;

  bool zcbz31;
  bool zgaz31;
  zgaz31 = eq_bits(zx, zy);
  zcbz31 = not(zgaz31);
end_function_4: ;
  return zcbz31;
end_block_exception_5: ;

  return false;
}

static void sailgen_sail_ones(lbits *rop, sail_int);

lbits zghz30;

static void startup_sailgen_sail_ones(void)
{    CREATE(lbits)(&zghz30);
}

static void sailgen_sail_ones(lbits *zcbz32, sail_int zn)
{
  __label__ end_function_7, end_block_exception_8, end_function_414;

  RECREATE(lbits)(&zghz30);
  zeros(&zghz30, zn);
  not_bits((*(&zcbz32)), zghz30);
end_function_7: ;
  goto end_function_414;
end_block_exception_8: ;
  goto end_function_414;
end_function_414: ;
}



static void finish_sailgen_sail_ones(void)
{    KILL(lbits)(&zghz30);
}

static bool sailgen_neq_anythingzItzK(fbits, fbits);

static bool sailgen_neq_anythingzItzK(fbits zx, fbits zy)
{
  __label__ end_function_10, end_block_exception_11;

  bool zcbz33;
  bool zgaz33;
  zgaz33 = eq_fbits(zy, zx);
  zcbz33 = not(zgaz33);
end_function_10: ;
  return zcbz33;
end_block_exception_11: ;

  return false;
}

static fbits sailgen_not_bit(fbits);

static fbits sailgen_not_bit(fbits zb)
{
  __label__ end_function_13, end_block_exception_14;

  fbits zcbz34;
  bool zgaz34;
  zgaz34 = eq_bit(zb, UINT64_C(1));
  if (zgaz34) {  zcbz34 = UINT64_C(0);  } else {  zcbz34 = UINT64_C(1);  }
end_function_13: ;
  return zcbz34;
end_block_exception_14: ;

  return UINT64_C(0);
}

static bool sailgen_not(bool);

static bool sailgen_not(bool zb)
{
  __label__ end_function_16, end_block_exception_17;

  bool zcbz35;
  zcbz35 = not(zb);
end_function_16: ;
  return zcbz35;
end_block_exception_17: ;

  return false;
}

static void sailgen_zzero_extend(lbits *rop, sail_int, lbits);

static void sailgen_zzero_extend(lbits *zcbz36, sail_int zm, lbits zv)
{
  __label__ end_function_19, end_block_exception_20, end_function_413;

  zero_extend((*(&zcbz36)), zv, zm);
end_function_19: ;
  goto end_function_413;
end_block_exception_20: ;
  goto end_function_413;
end_function_413: ;
}

static void sailgen_zzeros_implicit(lbits *rop, sail_int);

static void sailgen_zzeros_implicit(lbits *zcbz37, sail_int zn)
{
  __label__ end_function_22, end_block_exception_23, end_function_412;

  zeros((*(&zcbz37)), zn);
end_function_22: ;
  goto end_function_412;
end_block_exception_23: ;
  goto end_function_412;
end_function_412: ;
}

static void sailgen_ones(lbits *rop, sail_int);

static void sailgen_ones(lbits *zcbz38, sail_int zn)
{
  __label__ end_function_25, end_block_exception_26, end_function_411;

  sailgen_sail_ones((*(&zcbz38)), zn);
end_function_25: ;
  goto end_function_411;
end_block_exception_26: ;
  goto end_function_411;
end_function_411: ;
}

static uint64_t sailgen_bool_bits_forwards(bool);

static bool sailgen_bool_bits_backwards(uint64_t);

static bool sailgen_bool_bits_backwards_matches(uint64_t);

static uint64_t sailgen_bool_bits_forwards(bool zargz3)
{
  __label__ case_30, case_29, finish_match_28, end_function_31, end_block_exception_32;

  uint64_t zcbz39;
  {
    bool zp0z3;
    zp0z3 = zargz3;
    bool zgsz32;
    zgsz32 = eq_bool(zp0z3, true);
    if (!(zgsz32)) {  goto case_30;  }
    zcbz39 = UINT64_C(0b1);
    goto finish_match_28;
  }
case_30: ;
  {
    bool zuz30;
    zuz30 = zargz3;
    bool zgsz31;
    zgsz31 = eq_bool(zuz30, false);
    if (!(zgsz31)) {  goto case_29;  }
    zcbz39 = UINT64_C(0b0);
    goto finish_match_28;
  }
case_29: ;
finish_match_28: ;
end_function_31: ;
  return zcbz39;
end_block_exception_32: ;

  return UINT64_C(0xdeadc0de);
}

static uint64_t sailgen_bool_bits_forwards_infallible(bool);

static uint64_t sailgen_bool_bits_forwards_infallible(bool zargz3)
{
  __label__ case_35, case_34, finish_match_33, end_function_36, end_block_exception_37;

  uint64_t zcbz310;
  {
    bool zp0z3;
    zp0z3 = zargz3;
    bool zgsz35;
    zgsz35 = eq_bool(zp0z3, true);
    if (!(zgsz35)) {  goto case_35;  }
    zcbz310 = UINT64_C(0b1);
    goto finish_match_33;
  }
case_35: ;
  {
    bool zuz31;
    zuz31 = zargz3;
    bool zgsz34;
    zgsz34 = eq_bool(zuz31, false);
    if (!(zgsz34)) {  goto case_34;  }
    zcbz310 = UINT64_C(0b0);
    goto finish_match_33;
  }
case_34: ;
finish_match_33: ;
end_function_36: ;
  return zcbz310;
end_block_exception_37: ;

  return UINT64_C(0xdeadc0de);
}

static bool sailgen_bool_bits_backwards(uint64_t zargz3)
{
  __label__ case_41, case_40, finish_match_39, end_function_42, end_block_exception_43;

  bool zcbz311;
  {
    uint64_t zb__0;
    zb__0 = zargz3;
    bool zgsz38;
    zgsz38 = (zb__0 == UINT64_C(0b1));
    if (!(zgsz38)) {  goto case_41;  }
    zcbz311 = true;
    goto finish_match_39;
  }
case_41: ;
  {
    zcbz311 = false;
    goto finish_match_39;
  }
case_40: ;
finish_match_39: ;
end_function_42: ;
  return zcbz311;
end_block_exception_43: ;

  return false;
}

static bool sailgen_bool_bits_backwards_infallible(uint64_t);

static bool sailgen_bool_bits_backwards_infallible(uint64_t zargz3)
{
  __label__ case_46, case_45, finish_match_44, end_function_47, end_block_exception_48;

  bool zcbz312;
  {
    uint64_t zb__0;
    zb__0 = zargz3;
    bool zgsz311;
    zgsz311 = (zb__0 == UINT64_C(0b1));
    if (!(zgsz311)) {  goto case_46;  }
    zcbz312 = true;
    goto finish_match_44;
  }
case_46: ;
  {
    zcbz312 = false;
    goto finish_match_44;
  }
case_45: ;
finish_match_44: ;
end_function_47: ;
  return zcbz312;
end_block_exception_48: ;

  return false;
}

static bool sailgen_bool_bits_backwards_matches(uint64_t zargz3)
{
  __label__ case_53, case_52, case_51, finish_match_50, end_function_54, end_block_exception_55;

  bool zcbz313;
  {
    uint64_t zb__0;
    zb__0 = zargz3;
    bool zgsz315;
    zgsz315 = (zb__0 == UINT64_C(0b1));
    if (!(zgsz315)) {  goto case_53;  }
    zcbz313 = true;
    goto finish_match_50;
  }
case_53: ;
  {
    uint64_t zb__1;
    zb__1 = zargz3;
    bool zgsz314;
    zgsz314 = (zb__1 == UINT64_C(0b0));
    if (!(zgsz314)) {  goto case_52;  }
    zcbz313 = true;
    goto finish_match_50;
  }
case_52: ;
  {
    zcbz313 = false;
    goto finish_match_50;
  }
case_51: ;
finish_match_50: ;
end_function_54: ;
  return zcbz313;
end_block_exception_55: ;

  return false;
}

static void sailgen_to_bits(lbits *rop, sail_int, sail_int);

sail_int zghz31;

static void startup_sailgen_to_bits(void)
{    CREATE(sail_int)(&zghz31);
}

static void sailgen_to_bits(lbits *zcbz314, sail_int zl, sail_int zn)
{
  __label__ end_function_57, end_block_exception_58, end_function_410;

  {
    RECREATE(sail_int)(&zghz31);
    CONVERT_OF(sail_int, mach_int)(&zghz31, INT64_C(0));
    get_slice_int((*(&zcbz314)), zl, zn, zghz31);
  }
end_function_57: ;
  goto end_function_410;
end_block_exception_58: ;
  goto end_function_410;
end_function_410: ;
}



static void finish_sailgen_to_bits(void)
{    KILL(sail_int)(&zghz31);
}

static bool sailgen_z8operatorz0zI_uz9(lbits, lbits);

static bool sailgen_z8operatorz0zK_uz9(lbits, lbits);

sail_int zghz32;
sail_int zghz33;

static void startup_sailgen_z8operatorz0zI_uz9(void)
{
  CREATE(sail_int)(&zghz32);
  CREATE(sail_int)(&zghz33);
}

static bool sailgen_z8operatorz0zI_uz9(lbits zx, lbits zy)
{
  __label__ end_function_60, end_block_exception_61;

  bool zcbz315;
  RECREATE(sail_int)(&zghz32);
  sail_unsigned(&zghz32, zx);
  RECREATE(sail_int)(&zghz33);
  sail_unsigned(&zghz33, zy);
  zcbz315 = lt(zghz32, zghz33);
end_function_60: ;
  return zcbz315;
end_block_exception_61: ;

  return false;
}



static void finish_sailgen_z8operatorz0zI_uz9(void)
{
  KILL(sail_int)(&zghz33);
  KILL(sail_int)(&zghz32);
}

sail_int zghz34;
sail_int zghz35;

static void startup_sailgen_z8operatorz0zK_uz9(void)
{
  CREATE(sail_int)(&zghz34);
  CREATE(sail_int)(&zghz35);
}

static bool sailgen_z8operatorz0zK_uz9(lbits zx, lbits zy)
{
  __label__ end_function_63, end_block_exception_64;

  bool zcbz316;
  RECREATE(sail_int)(&zghz34);
  sail_unsigned(&zghz34, zx);
  RECREATE(sail_int)(&zghz35);
  sail_unsigned(&zghz35, zy);
  zcbz316 = gt(zghz34, zghz35);
end_function_63: ;
  return zcbz316;
end_block_exception_64: ;

  return false;
}



static void finish_sailgen_z8operatorz0zK_uz9(void)
{
  KILL(sail_int)(&zghz35);
  KILL(sail_int)(&zghz34);
}

static int64_t zlog2_xlen;
static void create_letbind_0(void) {


  int64_t zgsz316;
  zgsz316 = INT64_C(6);
  zlog2_xlen = zgsz316;
let_end_65: ;
}
static void kill_letbind_0(void) {
}

static int64_t zlog2_xlen_bytes;
static void create_letbind_1(void) {


  int64_t zgsz317;
  zgsz317 = INT64_C(3);
  zlog2_xlen_bytes = zgsz317;
let_end_66: ;
}
static void kill_letbind_1(void) {
}

static int64_t zxlen_bytes;
static void create_letbind_2(void) {


  int64_t zgsz318;
  zgsz318 = INT64_C(8);
  zxlen_bytes = zgsz318;
let_end_67: ;
}
static void kill_letbind_2(void) {
}

static int64_t zxlen;
static void create_letbind_3(void) {


  int64_t zgsz319;
  zgsz319 = INT64_C(64);
  zxlen = zgsz319;
let_end_68: ;
}
static void kill_letbind_3(void) {
}

static int64_t zasidlen;
static void create_letbind_4(void) {


  int64_t zgsz320;
  zgsz320 = INT64_C(16);
  zasidlen = zgsz320;
let_end_69: ;
}
static void kill_letbind_4(void) {
}

static void sailgen_MAX(sail_int *rop, sail_int);

sail_int zghz36;
sail_int zghz37;

static void startup_sailgen_MAX(void)
{
  CREATE(sail_int)(&zghz36);
  CREATE(sail_int)(&zghz37);
}

static void sailgen_MAX(sail_int *zcbz317, sail_int zn)
{
  __label__ end_function_71, end_block_exception_72, end_function_409;

  RECREATE(sail_int)(&zghz36);
  pow2(&zghz36, zn);
  {
    RECREATE(sail_int)(&zghz37);
    CONVERT_OF(sail_int, mach_int)(&zghz37, INT64_C(1));
    sub_int((*(&zcbz317)), zghz36, zghz37);
  }
end_function_71: ;
  goto end_function_409;
end_block_exception_72: ;
  goto end_function_409;
end_function_409: ;
}



static void finish_sailgen_MAX(void)
{
  KILL(sail_int)(&zghz37);
  KILL(sail_int)(&zghz36);
}

static uint64_t sailgen_execution_mode_encdec_forwards(enum zExecutionMode);

static enum zExecutionMode sailgen_execution_mode_encdec_backwards(uint64_t);

static bool sailgen_execution_mode_encdec_backwards_matches(uint64_t);

static uint64_t sailgen_execution_mode_encdec_forwards(enum zExecutionMode zargz3)
{
  __label__ case_76, case_75, finish_match_74, end_function_77, end_block_exception_78;

  uint64_t zcbz318;
  {
    if ((zIntPtrMode != zargz3)) goto case_76;
    zcbz318 = UINT64_C(0b1);
    goto finish_match_74;
  }
case_76: ;
  {
    /* complete */
    zcbz318 = UINT64_C(0b0);
    goto finish_match_74;
  }
case_75: ;
finish_match_74: ;
end_function_77: ;
  return zcbz318;
end_block_exception_78: ;

  return UINT64_C(0xdeadc0de);
}

static uint64_t sailgen_execution_mode_encdec_forwards_infallible(enum zExecutionMode);

static uint64_t sailgen_execution_mode_encdec_forwards_infallible(enum zExecutionMode zargz3)
{
  __label__ case_81, case_80, finish_match_79, end_function_82, end_block_exception_83;

  uint64_t zcbz319;
  {
    if ((zIntPtrMode != zargz3)) goto case_81;
    zcbz319 = UINT64_C(0b1);
    goto finish_match_79;
  }
case_81: ;
  {
    /* complete */
    zcbz319 = UINT64_C(0b0);
    goto finish_match_79;
  }
case_80: ;
finish_match_79: ;
end_function_82: ;
  return zcbz319;
end_block_exception_83: ;

  return UINT64_C(0xdeadc0de);
}

static enum zExecutionMode sailgen_execution_mode_encdec_backwards(uint64_t zargz3)
{
  __label__ case_87, case_86, finish_match_85, end_function_88, end_block_exception_89;

  enum zExecutionMode zcbz320;
  {
    uint64_t zb__0;
    zb__0 = zargz3;
    bool zgsz329;
    zgsz329 = (zb__0 == UINT64_C(0b1));
    if (!(zgsz329)) {  goto case_87;  }
    zcbz320 = zIntPtrMode;
    goto finish_match_85;
  }
case_87: ;
  {
    zcbz320 = zCapPtrMode;
    goto finish_match_85;
  }
case_86: ;
finish_match_85: ;
end_function_88: ;
  return zcbz320;
end_block_exception_89: ;

  return zCapPtrMode;
}

static enum zExecutionMode sailgen_execution_mode_encdec_backwards_infallible(uint64_t);

static enum zExecutionMode sailgen_execution_mode_encdec_backwards_infallible(uint64_t zargz3)
{
  __label__ case_92, case_91, finish_match_90, end_function_93, end_block_exception_94;

  enum zExecutionMode zcbz321;
  {
    uint64_t zb__0;
    zb__0 = zargz3;
    bool zgsz332;
    zgsz332 = (zb__0 == UINT64_C(0b1));
    if (!(zgsz332)) {  goto case_92;  }
    zcbz321 = zIntPtrMode;
    goto finish_match_90;
  }
case_92: ;
  {
    zcbz321 = zCapPtrMode;
    goto finish_match_90;
  }
case_91: ;
finish_match_90: ;
end_function_93: ;
  return zcbz321;
end_block_exception_94: ;

  return zCapPtrMode;
}

static bool sailgen_execution_mode_encdec_backwards_matches(uint64_t zargz3)
{
  __label__ case_99, case_98, case_97, finish_match_96, end_function_100, end_block_exception_101;

  bool zcbz322;
  {
    uint64_t zb__0;
    zb__0 = zargz3;
    bool zgsz336;
    zgsz336 = (zb__0 == UINT64_C(0b1));
    if (!(zgsz336)) {  goto case_99;  }
    zcbz322 = true;
    goto finish_match_96;
  }
case_99: ;
  {
    uint64_t zb__1;
    zb__1 = zargz3;
    bool zgsz335;
    zgsz335 = (zb__1 == UINT64_C(0b0));
    if (!(zgsz335)) {  goto case_98;  }
    zcbz322 = true;
    goto finish_match_96;
  }
case_98: ;
  {
    zcbz322 = false;
    goto finish_match_96;
  }
case_97: ;
finish_match_96: ;
end_function_100: ;
  return zcbz322;
end_block_exception_101: ;

  return false;
}

static struct zArchPerms znull_perms;
static void create_letbind_5(void) {


  struct zArchPerms zgsz338;
  struct zArchPerms zgsz337;
  zgsz337.zASR = false;
  zgsz337.zC = false;
  zgsz337.zEL = false;
  zgsz337.zLM = false;
  zgsz337.zR = false;
  zgsz337.zSL = false;
  zgsz337.zW = false;
  zgsz337.zX = false;
  zgsz338 = zgsz337;
  znull_perms = zgsz338;
let_end_102: ;
}
static void kill_letbind_5(void) {
}

static int64_t zlog2_cap_sizze;
static void create_letbind_6(void) {


  int64_t zgsz339;
  zgsz339 = INT64_C(4);
  zlog2_cap_sizze = zgsz339;
let_end_103: ;
}
static void kill_letbind_6(void) {
}

static int64_t zcap_sd_perms_width;
static void create_letbind_7(void) {


  int64_t zgsz340;
  zgsz340 = INT64_C(4);
  zcap_sd_perms_width = zgsz340;
let_end_104: ;
}
static void kill_letbind_7(void) {
}

static int64_t zcap_reserved_1_width;
static void create_letbind_8(void) {


  int64_t zgsz341;
  zgsz341 = INT64_C(7);
  zcap_reserved_1_width = zgsz341;
let_end_105: ;
}
static void kill_letbind_8(void) {
}

static int64_t zcap_reserved_0_width;
static void create_letbind_9(void) {


  int64_t zgsz342;
  zgsz342 = INT64_C(15);
  zcap_reserved_0_width = zgsz342;
let_end_106: ;
}
static void kill_letbind_9(void) {
}

static int64_t zcap_mantissa_width;
static void create_letbind_10(void) {


  int64_t zgsz343;
  zgsz343 = INT64_C(14);
  zcap_mantissa_width = zgsz343;
let_end_107: ;
}
static void kill_letbind_10(void) {
}

static int64_t zcap_E_width;
static void create_letbind_11(void) {


  int64_t zgsz344;
  zgsz344 = INT64_C(6);
  zcap_E_width = zgsz344;
let_end_108: ;
}
static void kill_letbind_11(void) {
}

static int64_t zcap_AP_M_width;
static void create_letbind_12(void) {


  int64_t zgsz345;
  zgsz345 = INT64_C(9);
  zcap_AP_M_width = zgsz345;
let_end_109: ;
}
static void kill_letbind_12(void) {
}

static bool zcap_use_L8;
static void create_letbind_13(void) {


  bool zgsz346;
  zgsz346 = false;
  zcap_use_L8 = zgsz346;
let_end_110: ;
}
static void kill_letbind_13(void) {
}

static struct zEncMetadata sailgen_Mk_EncMetadata(uint64_t);

struct zEncMetadata zghz38;

static void startup_sailgen_Mk_EncMetadata(void)
{
}

static struct zEncMetadata sailgen_Mk_EncMetadata(uint64_t zv)
{
  __label__ end_function_112, end_block_exception_113;

  struct zEncMetadata zghz38;
  zghz38.zbits = zv;
end_function_112: ;
  return zghz38;
end_block_exception_113: ;
  struct zEncMetadata zcbz385 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz385;
}



static void finish_sailgen_Mk_EncMetadata(void)
{
}

static uint64_t sailgen__get_EncMetadata_AP_M(struct zEncMetadata);

static uint64_t sailgen__get_EncMetadata_AP_M(struct zEncMetadata zv)
{
  __label__ end_function_115, end_block_exception_116;

  uint64_t zcbz324;
  uint64_t zgaz310;
  zgaz310 = zv.zbits;
  zcbz324 = (safe_rshift(UINT64_MAX, 64 - 9) & (zgaz310 >> INT64_C(44)));
end_function_115: ;
  return zcbz324;
end_block_exception_116: ;

  return UINT64_C(0xdeadc0de);
}

static struct zEncMetadata sailgen__update_EncMetadata_AP_M(struct zEncMetadata, uint64_t);

struct zEncMetadata zghz39;
lbits zghz310;
sail_int zghz311;
sail_int zghz312;
lbits zghz313;
lbits zghz314;

static void startup_sailgen__update_EncMetadata_AP_M(void)
{

  CREATE(lbits)(&zghz310);
  CREATE(sail_int)(&zghz311);
  CREATE(sail_int)(&zghz312);
  CREATE(lbits)(&zghz313);
  CREATE(lbits)(&zghz314);
}

static struct zEncMetadata sailgen__update_EncMetadata_AP_M(struct zEncMetadata zv, uint64_t zx)
{
  __label__ end_function_118, end_block_exception_119;

  struct zEncMetadata zghz39;
  uint64_t zgaz312;
  {
    uint64_t zgaz311;
    zgaz311 = zv.zbits;
    {
      RECREATE(lbits)(&zghz310);
      CONVERT_OF(lbits, fbits)(&zghz310, zgaz311, UINT64_C(64) , true);
      RECREATE(sail_int)(&zghz311);
      CONVERT_OF(sail_int, mach_int)(&zghz311, INT64_C(52));
      RECREATE(sail_int)(&zghz312);
      CONVERT_OF(sail_int, mach_int)(&zghz312, INT64_C(44));
      RECREATE(lbits)(&zghz313);
      CONVERT_OF(lbits, fbits)(&zghz313, zx, UINT64_C(9) , true);
      RECREATE(lbits)(&zghz314);
      vector_update_subrange_lbits(&zghz314, zghz310, zghz311, zghz312, zghz313);
      zgaz312 = CONVERT_OF(fbits, lbits)(zghz314, true);
    }
  }
  zghz39 = zv;
  zghz39.zbits = zgaz312;
end_function_118: ;
  return zghz39;
end_block_exception_119: ;
  struct zEncMetadata zcbz386 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz386;
}



static void finish_sailgen__update_EncMetadata_AP_M(void)
{
  KILL(lbits)(&zghz314);
  KILL(lbits)(&zghz313);
  KILL(sail_int)(&zghz312);
  KILL(sail_int)(&zghz311);
  KILL(lbits)(&zghz310);
}

static uint64_t sailgen__get_EncMetadata_BE(struct zEncMetadata);

static uint64_t sailgen__get_EncMetadata_BE(struct zEncMetadata zv)
{
  __label__ end_function_121, end_block_exception_122;

  uint64_t zcbz326;
  uint64_t zgaz313;
  zgaz313 = zv.zbits;
  zcbz326 = (safe_rshift(UINT64_MAX, 64 - 3) & (zgaz313 >> INT64_C(0)));
end_function_121: ;
  return zcbz326;
end_block_exception_122: ;

  return UINT64_C(0xdeadc0de);
}

static struct zEncMetadata sailgen__update_EncMetadata_BE(struct zEncMetadata, uint64_t);

struct zEncMetadata zghz315;
lbits zghz316;
sail_int zghz317;
sail_int zghz318;
lbits zghz319;
lbits zghz320;

static void startup_sailgen__update_EncMetadata_BE(void)
{

  CREATE(lbits)(&zghz316);
  CREATE(sail_int)(&zghz317);
  CREATE(sail_int)(&zghz318);
  CREATE(lbits)(&zghz319);
  CREATE(lbits)(&zghz320);
}

static struct zEncMetadata sailgen__update_EncMetadata_BE(struct zEncMetadata zv, uint64_t zx)
{
  __label__ end_function_124, end_block_exception_125;

  struct zEncMetadata zghz315;
  uint64_t zgaz315;
  {
    uint64_t zgaz314;
    zgaz314 = zv.zbits;
    {
      RECREATE(lbits)(&zghz316);
      CONVERT_OF(lbits, fbits)(&zghz316, zgaz314, UINT64_C(64) , true);
      RECREATE(sail_int)(&zghz317);
      CONVERT_OF(sail_int, mach_int)(&zghz317, INT64_C(2));
      RECREATE(sail_int)(&zghz318);
      CONVERT_OF(sail_int, mach_int)(&zghz318, INT64_C(0));
      RECREATE(lbits)(&zghz319);
      CONVERT_OF(lbits, fbits)(&zghz319, zx, UINT64_C(3) , true);
      RECREATE(lbits)(&zghz320);
      vector_update_subrange_lbits(&zghz320, zghz316, zghz317, zghz318, zghz319);
      zgaz315 = CONVERT_OF(fbits, lbits)(zghz320, true);
    }
  }
  zghz315 = zv;
  zghz315.zbits = zgaz315;
end_function_124: ;
  return zghz315;
end_block_exception_125: ;
  struct zEncMetadata zcbz387 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz387;
}



static void finish_sailgen__update_EncMetadata_BE(void)
{
  KILL(lbits)(&zghz320);
  KILL(lbits)(&zghz319);
  KILL(sail_int)(&zghz318);
  KILL(sail_int)(&zghz317);
  KILL(lbits)(&zghz316);
}

static uint64_t sailgen__get_EncMetadata_Bm(struct zEncMetadata);

static uint64_t sailgen__get_EncMetadata_Bm(struct zEncMetadata zv)
{
  __label__ end_function_127, end_block_exception_128;

  uint64_t zcbz328;
  uint64_t zgaz316;
  zgaz316 = zv.zbits;
  zcbz328 = (safe_rshift(UINT64_MAX, 64 - 11) & (zgaz316 >> INT64_C(3)));
end_function_127: ;
  return zcbz328;
end_block_exception_128: ;

  return UINT64_C(0xdeadc0de);
}

static struct zEncMetadata sailgen__update_EncMetadata_Bm(struct zEncMetadata, uint64_t);

struct zEncMetadata zghz321;
lbits zghz322;
sail_int zghz323;
sail_int zghz324;
lbits zghz325;
lbits zghz326;

static void startup_sailgen__update_EncMetadata_Bm(void)
{

  CREATE(lbits)(&zghz322);
  CREATE(sail_int)(&zghz323);
  CREATE(sail_int)(&zghz324);
  CREATE(lbits)(&zghz325);
  CREATE(lbits)(&zghz326);
}

static struct zEncMetadata sailgen__update_EncMetadata_Bm(struct zEncMetadata zv, uint64_t zx)
{
  __label__ end_function_130, end_block_exception_131;

  struct zEncMetadata zghz321;
  uint64_t zgaz318;
  {
    uint64_t zgaz317;
    zgaz317 = zv.zbits;
    {
      RECREATE(lbits)(&zghz322);
      CONVERT_OF(lbits, fbits)(&zghz322, zgaz317, UINT64_C(64) , true);
      RECREATE(sail_int)(&zghz323);
      CONVERT_OF(sail_int, mach_int)(&zghz323, INT64_C(13));
      RECREATE(sail_int)(&zghz324);
      CONVERT_OF(sail_int, mach_int)(&zghz324, INT64_C(3));
      RECREATE(lbits)(&zghz325);
      CONVERT_OF(lbits, fbits)(&zghz325, zx, UINT64_C(11) , true);
      RECREATE(lbits)(&zghz326);
      vector_update_subrange_lbits(&zghz326, zghz322, zghz323, zghz324, zghz325);
      zgaz318 = CONVERT_OF(fbits, lbits)(zghz326, true);
    }
  }
  zghz321 = zv;
  zghz321.zbits = zgaz318;
end_function_130: ;
  return zghz321;
end_block_exception_131: ;
  struct zEncMetadata zcbz388 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz388;
}



static void finish_sailgen__update_EncMetadata_Bm(void)
{
  KILL(lbits)(&zghz326);
  KILL(lbits)(&zghz325);
  KILL(sail_int)(&zghz324);
  KILL(sail_int)(&zghz323);
  KILL(lbits)(&zghz322);
}

static uint64_t sailgen__get_EncMetadata_CL(struct zEncMetadata);

static uint64_t sailgen__get_EncMetadata_CL(struct zEncMetadata zv)
{
  __label__ end_function_133, end_block_exception_134;

  uint64_t zcbz330;
  uint64_t zgaz319;
  zgaz319 = zv.zbits;
  zcbz330 = (safe_rshift(UINT64_MAX, 64 - 1) & (zgaz319 >> INT64_C(43)));
end_function_133: ;
  return zcbz330;
end_block_exception_134: ;

  return UINT64_C(0xdeadc0de);
}

static struct zEncMetadata sailgen__update_EncMetadata_CL(struct zEncMetadata, uint64_t);

struct zEncMetadata zghz327;
lbits zghz328;
sail_int zghz329;
sail_int zghz330;
lbits zghz331;
lbits zghz332;

static void startup_sailgen__update_EncMetadata_CL(void)
{

  CREATE(lbits)(&zghz328);
  CREATE(sail_int)(&zghz329);
  CREATE(sail_int)(&zghz330);
  CREATE(lbits)(&zghz331);
  CREATE(lbits)(&zghz332);
}

static struct zEncMetadata sailgen__update_EncMetadata_CL(struct zEncMetadata zv, uint64_t zx)
{
  __label__ end_function_136, end_block_exception_137;

  struct zEncMetadata zghz327;
  uint64_t zgaz321;
  {
    uint64_t zgaz320;
    zgaz320 = zv.zbits;
    {
      RECREATE(lbits)(&zghz328);
      CONVERT_OF(lbits, fbits)(&zghz328, zgaz320, UINT64_C(64) , true);
      RECREATE(sail_int)(&zghz329);
      CONVERT_OF(sail_int, mach_int)(&zghz329, INT64_C(43));
      RECREATE(sail_int)(&zghz330);
      CONVERT_OF(sail_int, mach_int)(&zghz330, INT64_C(43));
      RECREATE(lbits)(&zghz331);
      CONVERT_OF(lbits, fbits)(&zghz331, zx, UINT64_C(1) , true);
      RECREATE(lbits)(&zghz332);
      vector_update_subrange_lbits(&zghz332, zghz328, zghz329, zghz330, zghz331);
      zgaz321 = CONVERT_OF(fbits, lbits)(zghz332, true);
    }
  }
  zghz327 = zv;
  zghz327.zbits = zgaz321;
end_function_136: ;
  return zghz327;
end_block_exception_137: ;
  struct zEncMetadata zcbz389 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz389;
}



static void finish_sailgen__update_EncMetadata_CL(void)
{
  KILL(lbits)(&zghz332);
  KILL(lbits)(&zghz331);
  KILL(sail_int)(&zghz330);
  KILL(sail_int)(&zghz329);
  KILL(lbits)(&zghz328);
}

static uint64_t sailgen__get_EncMetadata_CT(struct zEncMetadata);

static uint64_t sailgen__get_EncMetadata_CT(struct zEncMetadata zv)
{
  __label__ end_function_139, end_block_exception_140;

  uint64_t zcbz332;
  uint64_t zgaz322;
  zgaz322 = zv.zbits;
  zcbz332 = (safe_rshift(UINT64_MAX, 64 - 1) & (zgaz322 >> INT64_C(27)));
end_function_139: ;
  return zcbz332;
end_block_exception_140: ;

  return UINT64_C(0xdeadc0de);
}

static struct zEncMetadata sailgen__update_EncMetadata_CT(struct zEncMetadata, uint64_t);

struct zEncMetadata zghz333;
lbits zghz334;
sail_int zghz335;
sail_int zghz336;
lbits zghz337;
lbits zghz338;

static void startup_sailgen__update_EncMetadata_CT(void)
{

  CREATE(lbits)(&zghz334);
  CREATE(sail_int)(&zghz335);
  CREATE(sail_int)(&zghz336);
  CREATE(lbits)(&zghz337);
  CREATE(lbits)(&zghz338);
}

static struct zEncMetadata sailgen__update_EncMetadata_CT(struct zEncMetadata zv, uint64_t zx)
{
  __label__ end_function_142, end_block_exception_143;

  struct zEncMetadata zghz333;
  uint64_t zgaz324;
  {
    uint64_t zgaz323;
    zgaz323 = zv.zbits;
    {
      RECREATE(lbits)(&zghz334);
      CONVERT_OF(lbits, fbits)(&zghz334, zgaz323, UINT64_C(64) , true);
      RECREATE(sail_int)(&zghz335);
      CONVERT_OF(sail_int, mach_int)(&zghz335, INT64_C(27));
      RECREATE(sail_int)(&zghz336);
      CONVERT_OF(sail_int, mach_int)(&zghz336, INT64_C(27));
      RECREATE(lbits)(&zghz337);
      CONVERT_OF(lbits, fbits)(&zghz337, zx, UINT64_C(1) , true);
      RECREATE(lbits)(&zghz338);
      vector_update_subrange_lbits(&zghz338, zghz334, zghz335, zghz336, zghz337);
      zgaz324 = CONVERT_OF(fbits, lbits)(zghz338, true);
    }
  }
  zghz333 = zv;
  zghz333.zbits = zgaz324;
end_function_142: ;
  return zghz333;
end_block_exception_143: ;
  struct zEncMetadata zcbz390 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz390;
}



static void finish_sailgen__update_EncMetadata_CT(void)
{
  KILL(lbits)(&zghz338);
  KILL(lbits)(&zghz337);
  KILL(sail_int)(&zghz336);
  KILL(sail_int)(&zghz335);
  KILL(lbits)(&zghz334);
}

static uint64_t sailgen__get_EncMetadata_EF(struct zEncMetadata);

static uint64_t sailgen__get_EncMetadata_EF(struct zEncMetadata zv)
{
  __label__ end_function_145, end_block_exception_146;

  uint64_t zcbz334;
  uint64_t zgaz325;
  zgaz325 = zv.zbits;
  zcbz334 = (safe_rshift(UINT64_MAX, 64 - 1) & (zgaz325 >> INT64_C(26)));
end_function_145: ;
  return zcbz334;
end_block_exception_146: ;

  return UINT64_C(0xdeadc0de);
}

static struct zEncMetadata sailgen__update_EncMetadata_EF(struct zEncMetadata, uint64_t);

struct zEncMetadata zghz339;
lbits zghz340;
sail_int zghz341;
sail_int zghz342;
lbits zghz343;
lbits zghz344;

static void startup_sailgen__update_EncMetadata_EF(void)
{

  CREATE(lbits)(&zghz340);
  CREATE(sail_int)(&zghz341);
  CREATE(sail_int)(&zghz342);
  CREATE(lbits)(&zghz343);
  CREATE(lbits)(&zghz344);
}

static struct zEncMetadata sailgen__update_EncMetadata_EF(struct zEncMetadata zv, uint64_t zx)
{
  __label__ end_function_148, end_block_exception_149;

  struct zEncMetadata zghz339;
  uint64_t zgaz327;
  {
    uint64_t zgaz326;
    zgaz326 = zv.zbits;
    {
      RECREATE(lbits)(&zghz340);
      CONVERT_OF(lbits, fbits)(&zghz340, zgaz326, UINT64_C(64) , true);
      RECREATE(sail_int)(&zghz341);
      CONVERT_OF(sail_int, mach_int)(&zghz341, INT64_C(26));
      RECREATE(sail_int)(&zghz342);
      CONVERT_OF(sail_int, mach_int)(&zghz342, INT64_C(26));
      RECREATE(lbits)(&zghz343);
      CONVERT_OF(lbits, fbits)(&zghz343, zx, UINT64_C(1) , true);
      RECREATE(lbits)(&zghz344);
      vector_update_subrange_lbits(&zghz344, zghz340, zghz341, zghz342, zghz343);
      zgaz327 = CONVERT_OF(fbits, lbits)(zghz344, true);
    }
  }
  zghz339 = zv;
  zghz339.zbits = zgaz327;
end_function_148: ;
  return zghz339;
end_block_exception_149: ;
  struct zEncMetadata zcbz391 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz391;
}



static void finish_sailgen__update_EncMetadata_EF(void)
{
  KILL(lbits)(&zghz344);
  KILL(lbits)(&zghz343);
  KILL(sail_int)(&zghz342);
  KILL(sail_int)(&zghz341);
  KILL(lbits)(&zghz340);
}

static uint64_t sailgen__get_EncMetadata_SDP(struct zEncMetadata);

static uint64_t sailgen__get_EncMetadata_SDP(struct zEncMetadata zv)
{
  __label__ end_function_151, end_block_exception_152;

  uint64_t zcbz336;
  uint64_t zgaz328;
  zgaz328 = zv.zbits;
  zcbz336 = (safe_rshift(UINT64_MAX, 64 - 4) & (zgaz328 >> INT64_C(53)));
end_function_151: ;
  return zcbz336;
end_block_exception_152: ;

  return UINT64_C(0xdeadc0de);
}

static struct zEncMetadata sailgen__update_EncMetadata_SDP(struct zEncMetadata, uint64_t);

struct zEncMetadata zghz345;
lbits zghz346;
sail_int zghz347;
sail_int zghz348;
lbits zghz349;
lbits zghz350;

static void startup_sailgen__update_EncMetadata_SDP(void)
{

  CREATE(lbits)(&zghz346);
  CREATE(sail_int)(&zghz347);
  CREATE(sail_int)(&zghz348);
  CREATE(lbits)(&zghz349);
  CREATE(lbits)(&zghz350);
}

static struct zEncMetadata sailgen__update_EncMetadata_SDP(struct zEncMetadata zv, uint64_t zx)
{
  __label__ end_function_154, end_block_exception_155;

  struct zEncMetadata zghz345;
  uint64_t zgaz330;
  {
    uint64_t zgaz329;
    zgaz329 = zv.zbits;
    {
      RECREATE(lbits)(&zghz346);
      CONVERT_OF(lbits, fbits)(&zghz346, zgaz329, UINT64_C(64) , true);
      RECREATE(sail_int)(&zghz347);
      CONVERT_OF(sail_int, mach_int)(&zghz347, INT64_C(56));
      RECREATE(sail_int)(&zghz348);
      CONVERT_OF(sail_int, mach_int)(&zghz348, INT64_C(53));
      RECREATE(lbits)(&zghz349);
      CONVERT_OF(lbits, fbits)(&zghz349, zx, UINT64_C(4) , true);
      RECREATE(lbits)(&zghz350);
      vector_update_subrange_lbits(&zghz350, zghz346, zghz347, zghz348, zghz349);
      zgaz330 = CONVERT_OF(fbits, lbits)(zghz350, true);
    }
  }
  zghz345 = zv;
  zghz345.zbits = zgaz330;
end_function_154: ;
  return zghz345;
end_block_exception_155: ;
  struct zEncMetadata zcbz392 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz392;
}



static void finish_sailgen__update_EncMetadata_SDP(void)
{
  KILL(lbits)(&zghz350);
  KILL(lbits)(&zghz349);
  KILL(sail_int)(&zghz348);
  KILL(sail_int)(&zghz347);
  KILL(lbits)(&zghz346);
}

static uint64_t sailgen__get_EncMetadata_TE(struct zEncMetadata);

static uint64_t sailgen__get_EncMetadata_TE(struct zEncMetadata zv)
{
  __label__ end_function_157, end_block_exception_158;

  uint64_t zcbz338;
  uint64_t zgaz331;
  zgaz331 = zv.zbits;
  zcbz338 = (safe_rshift(UINT64_MAX, 64 - 3) & (zgaz331 >> INT64_C(14)));
end_function_157: ;
  return zcbz338;
end_block_exception_158: ;

  return UINT64_C(0xdeadc0de);
}

static struct zEncMetadata sailgen__update_EncMetadata_TE(struct zEncMetadata, uint64_t);

struct zEncMetadata zghz351;
lbits zghz352;
sail_int zghz353;
sail_int zghz354;
lbits zghz355;
lbits zghz356;

static void startup_sailgen__update_EncMetadata_TE(void)
{

  CREATE(lbits)(&zghz352);
  CREATE(sail_int)(&zghz353);
  CREATE(sail_int)(&zghz354);
  CREATE(lbits)(&zghz355);
  CREATE(lbits)(&zghz356);
}

static struct zEncMetadata sailgen__update_EncMetadata_TE(struct zEncMetadata zv, uint64_t zx)
{
  __label__ end_function_160, end_block_exception_161;

  struct zEncMetadata zghz351;
  uint64_t zgaz333;
  {
    uint64_t zgaz332;
    zgaz332 = zv.zbits;
    {
      RECREATE(lbits)(&zghz352);
      CONVERT_OF(lbits, fbits)(&zghz352, zgaz332, UINT64_C(64) , true);
      RECREATE(sail_int)(&zghz353);
      CONVERT_OF(sail_int, mach_int)(&zghz353, INT64_C(16));
      RECREATE(sail_int)(&zghz354);
      CONVERT_OF(sail_int, mach_int)(&zghz354, INT64_C(14));
      RECREATE(lbits)(&zghz355);
      CONVERT_OF(lbits, fbits)(&zghz355, zx, UINT64_C(3) , true);
      RECREATE(lbits)(&zghz356);
      vector_update_subrange_lbits(&zghz356, zghz352, zghz353, zghz354, zghz355);
      zgaz333 = CONVERT_OF(fbits, lbits)(zghz356, true);
    }
  }
  zghz351 = zv;
  zghz351.zbits = zgaz333;
end_function_160: ;
  return zghz351;
end_block_exception_161: ;
  struct zEncMetadata zcbz393 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz393;
}



static void finish_sailgen__update_EncMetadata_TE(void)
{
  KILL(lbits)(&zghz356);
  KILL(lbits)(&zghz355);
  KILL(sail_int)(&zghz354);
  KILL(sail_int)(&zghz353);
  KILL(lbits)(&zghz352);
}

static uint64_t sailgen__get_EncMetadata_Tm(struct zEncMetadata);

static uint64_t sailgen__get_EncMetadata_Tm(struct zEncMetadata zv)
{
  __label__ end_function_163, end_block_exception_164;

  uint64_t zcbz340;
  uint64_t zgaz334;
  zgaz334 = zv.zbits;
  zcbz340 = (safe_rshift(UINT64_MAX, 64 - 9) & (zgaz334 >> INT64_C(17)));
end_function_163: ;
  return zcbz340;
end_block_exception_164: ;

  return UINT64_C(0xdeadc0de);
}

static struct zEncMetadata sailgen__update_EncMetadata_Tm(struct zEncMetadata, uint64_t);

struct zEncMetadata zghz357;
lbits zghz358;
sail_int zghz359;
sail_int zghz360;
lbits zghz361;
lbits zghz362;

static void startup_sailgen__update_EncMetadata_Tm(void)
{

  CREATE(lbits)(&zghz358);
  CREATE(sail_int)(&zghz359);
  CREATE(sail_int)(&zghz360);
  CREATE(lbits)(&zghz361);
  CREATE(lbits)(&zghz362);
}

static struct zEncMetadata sailgen__update_EncMetadata_Tm(struct zEncMetadata zv, uint64_t zx)
{
  __label__ end_function_166, end_block_exception_167;

  struct zEncMetadata zghz357;
  uint64_t zgaz336;
  {
    uint64_t zgaz335;
    zgaz335 = zv.zbits;
    {
      RECREATE(lbits)(&zghz358);
      CONVERT_OF(lbits, fbits)(&zghz358, zgaz335, UINT64_C(64) , true);
      RECREATE(sail_int)(&zghz359);
      CONVERT_OF(sail_int, mach_int)(&zghz359, INT64_C(25));
      RECREATE(sail_int)(&zghz360);
      CONVERT_OF(sail_int, mach_int)(&zghz360, INT64_C(17));
      RECREATE(lbits)(&zghz361);
      CONVERT_OF(lbits, fbits)(&zghz361, zx, UINT64_C(9) , true);
      RECREATE(lbits)(&zghz362);
      vector_update_subrange_lbits(&zghz362, zghz358, zghz359, zghz360, zghz361);
      zgaz336 = CONVERT_OF(fbits, lbits)(zghz362, true);
    }
  }
  zghz357 = zv;
  zghz357.zbits = zgaz336;
end_function_166: ;
  return zghz357;
end_block_exception_167: ;
  struct zEncMetadata zcbz394 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz394;
}



static void finish_sailgen__update_EncMetadata_Tm(void)
{
  KILL(lbits)(&zghz362);
  KILL(lbits)(&zghz361);
  KILL(sail_int)(&zghz360);
  KILL(sail_int)(&zghz359);
  KILL(lbits)(&zghz358);
}

static uint64_t sailgen__get_EncMetadata_reserved_0(struct zEncMetadata);

static uint64_t sailgen__get_EncMetadata_reserved_0(struct zEncMetadata zv)
{
  __label__ end_function_169, end_block_exception_170;

  uint64_t zcbz342;
  uint64_t zgaz337;
  zgaz337 = zv.zbits;
  zcbz342 = (safe_rshift(UINT64_MAX, 64 - 15) & (zgaz337 >> INT64_C(28)));
end_function_169: ;
  return zcbz342;
end_block_exception_170: ;

  return UINT64_C(0xdeadc0de);
}

static struct zEncMetadata sailgen__update_EncMetadata_reserved_0(struct zEncMetadata, uint64_t);

struct zEncMetadata zghz363;
lbits zghz364;
sail_int zghz365;
sail_int zghz366;
lbits zghz367;
lbits zghz368;

static void startup_sailgen__update_EncMetadata_reserved_0(void)
{

  CREATE(lbits)(&zghz364);
  CREATE(sail_int)(&zghz365);
  CREATE(sail_int)(&zghz366);
  CREATE(lbits)(&zghz367);
  CREATE(lbits)(&zghz368);
}

static struct zEncMetadata sailgen__update_EncMetadata_reserved_0(struct zEncMetadata zv, uint64_t zx)
{
  __label__ end_function_172, end_block_exception_173;

  struct zEncMetadata zghz363;
  uint64_t zgaz339;
  {
    uint64_t zgaz338;
    zgaz338 = zv.zbits;
    {
      RECREATE(lbits)(&zghz364);
      CONVERT_OF(lbits, fbits)(&zghz364, zgaz338, UINT64_C(64) , true);
      RECREATE(sail_int)(&zghz365);
      CONVERT_OF(sail_int, mach_int)(&zghz365, INT64_C(42));
      RECREATE(sail_int)(&zghz366);
      CONVERT_OF(sail_int, mach_int)(&zghz366, INT64_C(28));
      RECREATE(lbits)(&zghz367);
      CONVERT_OF(lbits, fbits)(&zghz367, zx, UINT64_C(15) , true);
      RECREATE(lbits)(&zghz368);
      vector_update_subrange_lbits(&zghz368, zghz364, zghz365, zghz366, zghz367);
      zgaz339 = CONVERT_OF(fbits, lbits)(zghz368, true);
    }
  }
  zghz363 = zv;
  zghz363.zbits = zgaz339;
end_function_172: ;
  return zghz363;
end_block_exception_173: ;
  struct zEncMetadata zcbz395 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz395;
}



static void finish_sailgen__update_EncMetadata_reserved_0(void)
{
  KILL(lbits)(&zghz368);
  KILL(lbits)(&zghz367);
  KILL(sail_int)(&zghz366);
  KILL(sail_int)(&zghz365);
  KILL(lbits)(&zghz364);
}

static uint64_t sailgen__get_EncMetadata_reserved_1(struct zEncMetadata);

static uint64_t sailgen__get_EncMetadata_reserved_1(struct zEncMetadata zv)
{
  __label__ end_function_175, end_block_exception_176;

  uint64_t zcbz344;
  uint64_t zgaz340;
  zgaz340 = zv.zbits;
  zcbz344 = (safe_rshift(UINT64_MAX, 64 - 7) & (zgaz340 >> INT64_C(57)));
end_function_175: ;
  return zcbz344;
end_block_exception_176: ;

  return UINT64_C(0xdeadc0de);
}

static struct zEncMetadata sailgen__update_EncMetadata_reserved_1(struct zEncMetadata, uint64_t);

struct zEncMetadata zghz369;
lbits zghz370;
sail_int zghz371;
sail_int zghz372;
lbits zghz373;
lbits zghz374;

static void startup_sailgen__update_EncMetadata_reserved_1(void)
{

  CREATE(lbits)(&zghz370);
  CREATE(sail_int)(&zghz371);
  CREATE(sail_int)(&zghz372);
  CREATE(lbits)(&zghz373);
  CREATE(lbits)(&zghz374);
}

static struct zEncMetadata sailgen__update_EncMetadata_reserved_1(struct zEncMetadata zv, uint64_t zx)
{
  __label__ end_function_178, end_block_exception_179;

  struct zEncMetadata zghz369;
  uint64_t zgaz342;
  {
    uint64_t zgaz341;
    zgaz341 = zv.zbits;
    {
      RECREATE(lbits)(&zghz370);
      CONVERT_OF(lbits, fbits)(&zghz370, zgaz341, UINT64_C(64) , true);
      RECREATE(sail_int)(&zghz371);
      CONVERT_OF(sail_int, mach_int)(&zghz371, INT64_C(63));
      RECREATE(sail_int)(&zghz372);
      CONVERT_OF(sail_int, mach_int)(&zghz372, INT64_C(57));
      RECREATE(lbits)(&zghz373);
      CONVERT_OF(lbits, fbits)(&zghz373, zx, UINT64_C(7) , true);
      RECREATE(lbits)(&zghz374);
      vector_update_subrange_lbits(&zghz374, zghz370, zghz371, zghz372, zghz373);
      zgaz342 = CONVERT_OF(fbits, lbits)(zghz374, true);
    }
  }
  zghz369 = zv;
  zghz369.zbits = zgaz342;
end_function_178: ;
  return zghz369;
end_block_exception_179: ;
  struct zEncMetadata zcbz396 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz396;
}



static void finish_sailgen__update_EncMetadata_reserved_1(void)
{
  KILL(lbits)(&zghz374);
  KILL(lbits)(&zghz373);
  KILL(sail_int)(&zghz372);
  KILL(sail_int)(&zghz371);
  KILL(lbits)(&zghz370);
}

static struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 sailgen_ap_m_encdec_forwards(uint64_t);

static uint64_t sailgen_ap_m_encdec_backwards(struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9);

static bool sailgen_ap_m_encdec_forwards_matches(uint64_t);

struct zArchPerms zghz375;

static void startup_sailgen_ap_m_encdec_forwards(void)
{
}

// struct tuple_(%enum zExecutionMode, %bool, %bool, %bool, %bool, %bool, %bool, %bool, %bool)
struct ztuple_z8z5enumz0zzExecutionModezCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolz9 {
  enum zExecutionMode ztup0;
  bool ztup1;
  bool ztup2;
  bool ztup3;
  bool ztup4;
  bool ztup5;
  bool ztup6;
  bool ztup7;
  bool ztup8;
};

static void COPY(ztuple_z8z5enumz0zzExecutionModezCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolz9)(struct ztuple_z8z5enumz0zzExecutionModezCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolz9 *rop, const struct ztuple_z8z5enumz0zzExecutionModezCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolz9 op) {
  rop->ztup0 = op.ztup0;
  rop->ztup1 = op.ztup1;
  rop->ztup2 = op.ztup2;
  rop->ztup3 = op.ztup3;
  rop->ztup4 = op.ztup4;
  rop->ztup5 = op.ztup5;
  rop->ztup6 = op.ztup6;
  rop->ztup7 = op.ztup7;
  rop->ztup8 = op.ztup8;
}

static bool EQUAL(ztuple_z8z5enumz0zzExecutionModezCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolz9)(struct ztuple_z8z5enumz0zzExecutionModezCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolz9 op1, struct ztuple_z8z5enumz0zzExecutionModezCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolz9 op2) {
  return EQUAL(zExecutionMode)(op1.ztup0, op2.ztup0) && EQUAL(bool)(op1.ztup1, op2.ztup1) && EQUAL(bool)(op1.ztup2, op2.ztup2) && EQUAL(bool)(op1.ztup3, op2.ztup3) && EQUAL(bool)(op1.ztup4, op2.ztup4) && EQUAL(bool)(op1.ztup5, op2.ztup5) && EQUAL(bool)(op1.ztup6, op2.ztup6) && EQUAL(bool)(op1.ztup7, op2.ztup7) && EQUAL(bool)(op1.ztup8, op2.ztup8);
}

static struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 sailgen_ap_m_encdec_forwards(uint64_t zargz3)
{
  __label__ case_188, finish_match_187, end_function_189, end_block_exception_190;

  struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zcbz346;
  uint64_t zhead_expz3;
  zhead_expz3 = zargz3;
  struct zoptionzIz8RArchPermszCEExecutionModez5z9zK zgaz372;
  CREATE(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zgaz372);
  {
    __label__ case_183, case_182, finish_match_181;

    {
      __label__ case_186, case_185, finish_match_184;

      uint64_t zv__0;
      zv__0 = zhead_expz3;
      uint64_t zmapping0z3;
      zmapping0z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(8)));
      uint64_t zmapping8z3;
      zmapping8z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(0)));
      uint64_t zmapping7z3;
      zmapping7z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(1)));
      uint64_t zmapping6z3;
      zmapping6z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(2)));
      uint64_t zmapping5z3;
      zmapping5z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(3)));
      uint64_t zmapping4z3;
      zmapping4z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(4)));
      uint64_t zmapping3z3;
      zmapping3z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(5)));
      uint64_t zmapping2z3;
      zmapping2z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(6)));
      uint64_t zmapping1z3;
      zmapping1z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(7)));
      uint64_t zmapping0z3shadowz30;
      zmapping0z3shadowz30 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(8)));
      bool zgaz371;
      zgaz371 = sailgen_execution_mode_encdec_backwards_matches(zmapping0z3shadowz30);
      bool zgsz368;
      if (zgaz371) {
        bool zgaz370;
        zgaz370 = sailgen_bool_bits_backwards_matches(zmapping1z3);
        bool zgsz367;
        if (zgaz370) {
          bool zgaz369;
          zgaz369 = sailgen_bool_bits_backwards_matches(zmapping2z3);
          bool zgsz366;
          if (zgaz369) {
            bool zgaz368;
            zgaz368 = sailgen_bool_bits_backwards_matches(zmapping3z3);
            bool zgsz365;
            if (zgaz368) {
              bool zgaz367;
              zgaz367 = sailgen_bool_bits_backwards_matches(zmapping4z3);
              bool zgsz364;
              if (zgaz367) {
                bool zgaz366;
                zgaz366 = sailgen_bool_bits_backwards_matches(zmapping5z3);
                bool zgsz363;
                if (zgaz366) {
                  bool zgaz365;
                  zgaz365 = sailgen_bool_bits_backwards_matches(zmapping6z3);
                  bool zgsz362;
                  if (zgaz365) {
                    bool zgaz364;
                    zgaz364 = sailgen_bool_bits_backwards_matches(zmapping7z3);
                    bool zgsz361;
                    if (zgaz364) {  zgsz361 = sailgen_bool_bits_backwards_matches(zmapping8z3);  } else {
                      zgsz361 = false;
                    }
                    zgsz362 = zgsz361;
                  } else {  zgsz362 = false;  }
                  zgsz363 = zgsz362;
                } else {  zgsz363 = false;  }
                zgsz364 = zgsz363;
              } else {  zgsz364 = false;  }
              zgsz365 = zgsz364;
            } else {  zgsz365 = false;  }
            zgsz366 = zgsz365;
          } else {  zgsz366 = false;  }
          zgsz367 = zgsz366;
        } else {  zgsz367 = false;  }
        zgsz368 = zgsz367;
      } else {  zgsz368 = false;  }
      bool zgsz387;
      zgsz387 = zgsz368;
      if (!(zgsz387)) {  goto case_183;  }
      uint64_t zmapping0z3shadowz31;
      zmapping0z3shadowz31 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(8)));
      uint64_t zmapping8z3shadowz32;
      zmapping8z3shadowz32 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(0)));
      uint64_t zmapping7z3shadowz33;
      zmapping7z3shadowz33 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(1)));
      uint64_t zmapping6z3shadowz34;
      zmapping6z3shadowz34 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(2)));
      uint64_t zmapping5z3shadowz35;
      zmapping5z3shadowz35 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(3)));
      uint64_t zmapping4z3shadowz36;
      zmapping4z3shadowz36 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(4)));
      uint64_t zmapping3z3shadowz37;
      zmapping3z3shadowz37 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(5)));
      uint64_t zmapping2z3shadowz38;
      zmapping2z3shadowz38 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(6)));
      uint64_t zmapping1z3shadowz39;
      zmapping1z3shadowz39 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(7)));
      uint64_t zmapping0z3shadowz310;
      zmapping0z3shadowz310 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(8)));
      struct ztuple_z8z5enumz0zzExecutionModezCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolz9 zgaz352;
      {
        enum zExecutionMode zgaz343;
        zgaz343 = sailgen_execution_mode_encdec_backwards_infallible(zmapping0z3shadowz310);
        bool zgaz344;
        zgaz344 = sailgen_bool_bits_backwards_infallible(zmapping1z3shadowz39);
        bool zgaz345;
        zgaz345 = sailgen_bool_bits_backwards_infallible(zmapping2z3shadowz38);
        bool zgaz346;
        zgaz346 = sailgen_bool_bits_backwards_infallible(zmapping3z3shadowz37);
        bool zgaz347;
        zgaz347 = sailgen_bool_bits_backwards_infallible(zmapping4z3shadowz36);
        bool zgaz348;
        zgaz348 = sailgen_bool_bits_backwards_infallible(zmapping5z3shadowz35);
        bool zgaz349;
        zgaz349 = sailgen_bool_bits_backwards_infallible(zmapping6z3shadowz34);
        bool zgaz350;
        zgaz350 = sailgen_bool_bits_backwards_infallible(zmapping7z3shadowz33);
        bool zgaz351;
        zgaz351 = sailgen_bool_bits_backwards_infallible(zmapping8z3shadowz32);
        zgaz352.ztup0 = zgaz343;
        zgaz352.ztup1 = zgaz344;
        zgaz352.ztup2 = zgaz345;
        zgaz352.ztup3 = zgaz346;
        zgaz352.ztup4 = zgaz347;
        zgaz352.ztup5 = zgaz348;
        zgaz352.ztup6 = zgaz349;
        zgaz352.ztup7 = zgaz350;
        zgaz352.ztup8 = zgaz351;
      }
      struct zoptionzIz8RArchPermszCEExecutionModez5z9zK zgsz370;
      CREATE(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zgsz370);
      {
        enum zExecutionMode zM;
        zM = zgaz352.ztup0;
        bool zSL;
        zSL = zgaz352.ztup1;
        bool zEL;
        zEL = zgaz352.ztup2;
        bool zLM;
        zLM = zgaz352.ztup3;
        bool zASR;
        zASR = zgaz352.ztup4;
        bool zX;
        zX = zgaz352.ztup5;
        bool zR;
        zR = zgaz352.ztup6;
        bool zW;
        zW = zgaz352.ztup7;
        bool zC;
        zC = zgaz352.ztup8;
        bool zgaz363;
        {
          bool zgaz353;
          zgaz353 = sailgen_not(zASR);
          bool zgsz372;
          if (zgaz353) {  zgsz372 = true;  } else {  zgsz372 = zX;  }
          zgaz363 = zgsz372;
        }
        bool zgsz383;
        if (zgaz363) {
          bool zgaz362;
          {
            bool zgaz354;
            zgaz354 = sailgen_not(zC);
            bool zgsz374;
            if (zgaz354) {  zgsz374 = true;  } else {
              bool zgsz373;
              if (zR) {  zgsz373 = true;  } else {  zgsz373 = zW;  }
              zgsz374 = zgsz373;
            }
            zgaz362 = zgsz374;
          }
          bool zgsz382;
          if (zgaz362) {
            bool zgaz361;
            {
              bool zgaz356;
              {
                enum zExecutionMode zgaz355;
                zgaz355 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
                zgaz356 = eq_zExecutionMode(zM, zgaz355);
              }
              bool zgsz375;
              if (zgaz356) {  zgsz375 = true;  } else {  zgsz375 = zX;  }
              zgaz361 = zgsz375;
            }
            bool zgsz381;
            if (zgaz361) {
              bool zgaz360;
              {
                bool zgaz357;
                zgaz357 = sailgen_not(zSL);
                bool zgsz376;
                if (zgaz357) {  zgsz376 = true;  } else {  zgsz376 = zC;  }
                zgaz360 = zgsz376;
              }
              bool zgsz380;
              if (zgaz360) {
                bool zgaz359;
                {
                  bool zgaz358;
                  {    if (zEL) {  zgaz358 = true;  } else {  zgaz358 = zLM;  }
                  }
                  zgaz359 = sailgen_not(zgaz358);
                }
                bool zgsz379;
                if (zgaz359) {  zgsz379 = true;  } else {
                  bool zgsz378;
                  if (zC) {  zgsz378 = zR;  } else {  zgsz378 = false;  }
                  zgsz379 = zgsz378;
                }
                zgsz380 = zgsz379;
              } else {  zgsz380 = false;  }
              zgsz381 = zgsz380;
            } else {  zgsz381 = false;  }
            zgsz382 = zgsz381;
          } else {  zgsz382 = false;  }
          zgsz383 = zgsz382;
        } else {  zgsz383 = false;  }
        bool zgsz386;
        zgsz386 = zgsz383;
        if (!(zgsz386)) {  goto case_186;  }
        struct zArchPerms zghz375;
        zghz375.zASR = zASR;
        zghz375.zC = zC;
        zghz375.zEL = zEL;
        zghz375.zLM = zLM;
        zghz375.zR = zR;
        zghz375.zSL = zSL;
        zghz375.zW = zW;
        zghz375.zX = zX;
        struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz385;
        zgsz385.ztup0 = zghz375;
        zgsz385.ztup1 = zM;
        sailgen_SomezIz8RArchPermszCEExecutionModez5z9zK(&zgsz370, zgsz385);
        goto finish_match_184;
      }
    case_186: ;
      {
        sailgen_NonezIz8RArchPermszCEExecutionModez5z9zK(&zgsz370, UNIT);
        goto finish_match_184;
      }
    case_185: ;
    finish_match_184: ;
      COPY(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zgaz372, zgsz370);
      KILL(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zgsz370);
      goto finish_match_181;
    }
  case_183: ;
    {
      sailgen_NonezIz8RArchPermszCEExecutionModez5z9zK(&zgaz372, UNIT);
      goto finish_match_181;
    }
  case_182: ;
  finish_match_181: ;
  }
  struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz388;
  {
    if (zgaz372.kind != Kind_zSomezIz8RArchPermszCEExecutionModez5z9zK) goto case_188;
    zgsz388 = zgaz372.variants.zSomezIz8RArchPermszCEExecutionModez5z9zK;
    goto finish_match_187;
  }
case_188: ;
  sail_match_failure("ap_m_encdec_forwards");
finish_match_187: ;
  zcbz346 = zgsz388;
  KILL(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zgaz372);
end_function_189: ;
  return zcbz346;
end_block_exception_190: ;
  struct zArchPerms zcbz398 = { .zASR = false, .zC = false, .zEL = false, .zLM = false, .zR = false, .zSL = false, .zW = false, .zX = false };
  struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zcbz397 = { .ztup0 = zcbz398, .ztup1 = zCapPtrMode };
  return zcbz397;
}



static void finish_sailgen_ap_m_encdec_forwards(void)
{
}

static struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 sailgen_ap_m_encdec_forwards_infallible(uint64_t);

struct zArchPerms zghz376;

static void startup_sailgen_ap_m_encdec_forwards_infallible(void)
{
}

static struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 sailgen_ap_m_encdec_forwards_infallible(uint64_t zargz3)
{
  __label__ case_198, finish_match_197, end_function_199, end_block_exception_200;

  struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zcbz347;
  uint64_t zhead_expz3;
  zhead_expz3 = zargz3;
  struct zoptionzIz8RArchPermszCEExecutionModez5z9zK zgaz372;
  CREATE(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zgaz372);
  {
    __label__ case_193, case_192, finish_match_191;

    {
      __label__ case_196, case_195, finish_match_194;

      uint64_t zv__0;
      zv__0 = zhead_expz3;
      uint64_t zmapping0z3;
      zmapping0z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(8)));
      uint64_t zmapping8z3;
      zmapping8z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(0)));
      uint64_t zmapping7z3;
      zmapping7z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(1)));
      uint64_t zmapping6z3;
      zmapping6z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(2)));
      uint64_t zmapping5z3;
      zmapping5z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(3)));
      uint64_t zmapping4z3;
      zmapping4z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(4)));
      uint64_t zmapping3z3;
      zmapping3z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(5)));
      uint64_t zmapping2z3;
      zmapping2z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(6)));
      uint64_t zmapping1z3;
      zmapping1z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(7)));
      uint64_t zmapping0z3shadowz30;
      zmapping0z3shadowz30 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(8)));
      bool zgaz371;
      zgaz371 = sailgen_execution_mode_encdec_backwards_matches(zmapping0z3shadowz30);
      bool zgsz399;
      if (zgaz371) {
        bool zgaz370;
        zgaz370 = sailgen_bool_bits_backwards_matches(zmapping1z3);
        bool zgsz398;
        if (zgaz370) {
          bool zgaz369;
          zgaz369 = sailgen_bool_bits_backwards_matches(zmapping2z3);
          bool zgsz397;
          if (zgaz369) {
            bool zgaz368;
            zgaz368 = sailgen_bool_bits_backwards_matches(zmapping3z3);
            bool zgsz396;
            if (zgaz368) {
              bool zgaz367;
              zgaz367 = sailgen_bool_bits_backwards_matches(zmapping4z3);
              bool zgsz395;
              if (zgaz367) {
                bool zgaz366;
                zgaz366 = sailgen_bool_bits_backwards_matches(zmapping5z3);
                bool zgsz394;
                if (zgaz366) {
                  bool zgaz365;
                  zgaz365 = sailgen_bool_bits_backwards_matches(zmapping6z3);
                  bool zgsz393;
                  if (zgaz365) {
                    bool zgaz364;
                    zgaz364 = sailgen_bool_bits_backwards_matches(zmapping7z3);
                    bool zgsz392;
                    if (zgaz364) {  zgsz392 = sailgen_bool_bits_backwards_matches(zmapping8z3);  } else {
                      zgsz392 = false;
                    }
                    zgsz393 = zgsz392;
                  } else {  zgsz393 = false;  }
                  zgsz394 = zgsz393;
                } else {  zgsz394 = false;  }
                zgsz395 = zgsz394;
              } else {  zgsz395 = false;  }
              zgsz396 = zgsz395;
            } else {  zgsz396 = false;  }
            zgsz397 = zgsz396;
          } else {  zgsz397 = false;  }
          zgsz398 = zgsz397;
        } else {  zgsz398 = false;  }
        zgsz399 = zgsz398;
      } else {  zgsz399 = false;  }
      bool zgsz3118;
      zgsz3118 = zgsz399;
      if (!(zgsz3118)) {  goto case_193;  }
      uint64_t zmapping0z3shadowz31;
      zmapping0z3shadowz31 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(8)));
      uint64_t zmapping8z3shadowz32;
      zmapping8z3shadowz32 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(0)));
      uint64_t zmapping7z3shadowz33;
      zmapping7z3shadowz33 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(1)));
      uint64_t zmapping6z3shadowz34;
      zmapping6z3shadowz34 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(2)));
      uint64_t zmapping5z3shadowz35;
      zmapping5z3shadowz35 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(3)));
      uint64_t zmapping4z3shadowz36;
      zmapping4z3shadowz36 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(4)));
      uint64_t zmapping3z3shadowz37;
      zmapping3z3shadowz37 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(5)));
      uint64_t zmapping2z3shadowz38;
      zmapping2z3shadowz38 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(6)));
      uint64_t zmapping1z3shadowz39;
      zmapping1z3shadowz39 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(7)));
      uint64_t zmapping0z3shadowz310;
      zmapping0z3shadowz310 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(8)));
      struct ztuple_z8z5enumz0zzExecutionModezCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolz9 zgaz352;
      {
        enum zExecutionMode zgaz343;
        zgaz343 = sailgen_execution_mode_encdec_backwards_infallible(zmapping0z3shadowz310);
        bool zgaz344;
        zgaz344 = sailgen_bool_bits_backwards_infallible(zmapping1z3shadowz39);
        bool zgaz345;
        zgaz345 = sailgen_bool_bits_backwards_infallible(zmapping2z3shadowz38);
        bool zgaz346;
        zgaz346 = sailgen_bool_bits_backwards_infallible(zmapping3z3shadowz37);
        bool zgaz347;
        zgaz347 = sailgen_bool_bits_backwards_infallible(zmapping4z3shadowz36);
        bool zgaz348;
        zgaz348 = sailgen_bool_bits_backwards_infallible(zmapping5z3shadowz35);
        bool zgaz349;
        zgaz349 = sailgen_bool_bits_backwards_infallible(zmapping6z3shadowz34);
        bool zgaz350;
        zgaz350 = sailgen_bool_bits_backwards_infallible(zmapping7z3shadowz33);
        bool zgaz351;
        zgaz351 = sailgen_bool_bits_backwards_infallible(zmapping8z3shadowz32);
        zgaz352.ztup0 = zgaz343;
        zgaz352.ztup1 = zgaz344;
        zgaz352.ztup2 = zgaz345;
        zgaz352.ztup3 = zgaz346;
        zgaz352.ztup4 = zgaz347;
        zgaz352.ztup5 = zgaz348;
        zgaz352.ztup6 = zgaz349;
        zgaz352.ztup7 = zgaz350;
        zgaz352.ztup8 = zgaz351;
      }
      struct zoptionzIz8RArchPermszCEExecutionModez5z9zK zgsz3101;
      CREATE(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zgsz3101);
      {
        enum zExecutionMode zM;
        zM = zgaz352.ztup0;
        bool zSL;
        zSL = zgaz352.ztup1;
        bool zEL;
        zEL = zgaz352.ztup2;
        bool zLM;
        zLM = zgaz352.ztup3;
        bool zASR;
        zASR = zgaz352.ztup4;
        bool zX;
        zX = zgaz352.ztup5;
        bool zR;
        zR = zgaz352.ztup6;
        bool zW;
        zW = zgaz352.ztup7;
        bool zC;
        zC = zgaz352.ztup8;
        bool zgaz363;
        {
          bool zgaz353;
          zgaz353 = sailgen_not(zASR);
          bool zgsz3103;
          if (zgaz353) {  zgsz3103 = true;  } else {  zgsz3103 = zX;  }
          zgaz363 = zgsz3103;
        }
        bool zgsz3114;
        if (zgaz363) {
          bool zgaz362;
          {
            bool zgaz354;
            zgaz354 = sailgen_not(zC);
            bool zgsz3105;
            if (zgaz354) {  zgsz3105 = true;  } else {
              bool zgsz3104;
              if (zR) {  zgsz3104 = true;  } else {  zgsz3104 = zW;  }
              zgsz3105 = zgsz3104;
            }
            zgaz362 = zgsz3105;
          }
          bool zgsz3113;
          if (zgaz362) {
            bool zgaz361;
            {
              bool zgaz356;
              {
                enum zExecutionMode zgaz355;
                zgaz355 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
                zgaz356 = eq_zExecutionMode(zM, zgaz355);
              }
              bool zgsz3106;
              if (zgaz356) {  zgsz3106 = true;  } else {  zgsz3106 = zX;  }
              zgaz361 = zgsz3106;
            }
            bool zgsz3112;
            if (zgaz361) {
              bool zgaz360;
              {
                bool zgaz357;
                zgaz357 = sailgen_not(zSL);
                bool zgsz3107;
                if (zgaz357) {  zgsz3107 = true;  } else {  zgsz3107 = zC;  }
                zgaz360 = zgsz3107;
              }
              bool zgsz3111;
              if (zgaz360) {
                bool zgaz359;
                {
                  bool zgaz358;
                  {    if (zEL) {  zgaz358 = true;  } else {  zgaz358 = zLM;  }
                  }
                  zgaz359 = sailgen_not(zgaz358);
                }
                bool zgsz3110;
                if (zgaz359) {  zgsz3110 = true;  } else {
                  bool zgsz3109;
                  if (zC) {  zgsz3109 = zR;  } else {  zgsz3109 = false;  }
                  zgsz3110 = zgsz3109;
                }
                zgsz3111 = zgsz3110;
              } else {  zgsz3111 = false;  }
              zgsz3112 = zgsz3111;
            } else {  zgsz3112 = false;  }
            zgsz3113 = zgsz3112;
          } else {  zgsz3113 = false;  }
          zgsz3114 = zgsz3113;
        } else {  zgsz3114 = false;  }
        bool zgsz3117;
        zgsz3117 = zgsz3114;
        if (!(zgsz3117)) {  goto case_196;  }
        struct zArchPerms zghz376;
        zghz376.zASR = zASR;
        zghz376.zC = zC;
        zghz376.zEL = zEL;
        zghz376.zLM = zLM;
        zghz376.zR = zR;
        zghz376.zSL = zSL;
        zghz376.zW = zW;
        zghz376.zX = zX;
        struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz3116;
        zgsz3116.ztup0 = zghz376;
        zgsz3116.ztup1 = zM;
        sailgen_SomezIz8RArchPermszCEExecutionModez5z9zK(&zgsz3101, zgsz3116);
        goto finish_match_194;
      }
    case_196: ;
      {
        sailgen_NonezIz8RArchPermszCEExecutionModez5z9zK(&zgsz3101, UNIT);
        goto finish_match_194;
      }
    case_195: ;
    finish_match_194: ;
      COPY(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zgaz372, zgsz3101);
      KILL(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zgsz3101);
      goto finish_match_191;
    }
  case_193: ;
    {
      sailgen_NonezIz8RArchPermszCEExecutionModez5z9zK(&zgaz372, UNIT);
      goto finish_match_191;
    }
  case_192: ;
  finish_match_191: ;
  }
  struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz3119;
  {
    if (zgaz372.kind != Kind_zSomezIz8RArchPermszCEExecutionModez5z9zK) goto case_198;
    zgsz3119 = zgaz372.variants.zSomezIz8RArchPermszCEExecutionModez5z9zK;
    goto finish_match_197;
  }
case_198: ;
  sail_match_failure("ap_m_encdec_forwards_infallible");
finish_match_197: ;
  zcbz347 = zgsz3119;
  KILL(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zgaz372);
end_function_199: ;
  return zcbz347;
end_block_exception_200: ;
  struct zArchPerms zcbz3100 = { .zASR = false, .zC = false, .zEL = false, .zLM = false, .zR = false, .zSL = false, .zW = false, .zX = false };
  struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zcbz399 = { .ztup0 = zcbz3100, .ztup1 = zCapPtrMode };
  return zcbz399;
}



static void finish_sailgen_ap_m_encdec_forwards_infallible(void)
{
}

static uint64_t sailgen_ap_m_encdec_backwards(struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zargz3)
{
  __label__ case_203, finish_match_202, end_function_204, end_block_exception_205;

  uint64_t zcbz348;
  {
    bool zR;
    zR = zargz3.ztup0.zR;
    bool zW;
    zW = zargz3.ztup0.zW;
    bool zC;
    zC = zargz3.ztup0.zC;
    bool zX;
    zX = zargz3.ztup0.zX;
    bool zLM;
    zLM = zargz3.ztup0.zLM;
    bool zSL;
    zSL = zargz3.ztup0.zSL;
    bool zEL;
    zEL = zargz3.ztup0.zEL;
    bool zASR;
    zASR = zargz3.ztup0.zASR;
    enum zExecutionMode zM;
    zM = zargz3.ztup1;
    bool zgaz399;
    {
      bool zgaz389;
      zgaz389 = sailgen_not(zASR);
      bool zgsz3122;
      if (zgaz389) {  zgsz3122 = true;  } else {  zgsz3122 = zX;  }
      zgaz399 = zgsz3122;
    }
    bool zgsz3133;
    if (zgaz399) {
      bool zgaz398;
      {
        bool zgaz390;
        zgaz390 = sailgen_not(zC);
        bool zgsz3124;
        if (zgaz390) {  zgsz3124 = true;  } else {
          bool zgsz3123;
          if (zR) {  zgsz3123 = true;  } else {  zgsz3123 = zW;  }
          zgsz3124 = zgsz3123;
        }
        zgaz398 = zgsz3124;
      }
      bool zgsz3132;
      if (zgaz398) {
        bool zgaz397;
        {
          bool zgaz392;
          {
            enum zExecutionMode zgaz391;
            zgaz391 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
            zgaz392 = eq_zExecutionMode(zM, zgaz391);
          }
          bool zgsz3125;
          if (zgaz392) {  zgsz3125 = true;  } else {  zgsz3125 = zX;  }
          zgaz397 = zgsz3125;
        }
        bool zgsz3131;
        if (zgaz397) {
          bool zgaz396;
          {
            bool zgaz393;
            zgaz393 = sailgen_not(zSL);
            bool zgsz3126;
            if (zgaz393) {  zgsz3126 = true;  } else {  zgsz3126 = zC;  }
            zgaz396 = zgsz3126;
          }
          bool zgsz3130;
          if (zgaz396) {
            bool zgaz395;
            {
              bool zgaz394;
              {    if (zEL) {  zgaz394 = true;  } else {  zgaz394 = zLM;  }
              }
              zgaz395 = sailgen_not(zgaz394);
            }
            bool zgsz3129;
            if (zgaz395) {  zgsz3129 = true;  } else {
              bool zgsz3128;
              if (zC) {  zgsz3128 = zR;  } else {  zgsz3128 = false;  }
              zgsz3129 = zgsz3128;
            }
            zgsz3130 = zgsz3129;
          } else {  zgsz3130 = false;  }
          zgsz3131 = zgsz3130;
        } else {  zgsz3131 = false;  }
        zgsz3132 = zgsz3131;
      } else {  zgsz3132 = false;  }
      zgsz3133 = zgsz3132;
    } else {  zgsz3133 = false;  }
    bool zgsz3134;
    zgsz3134 = zgsz3133;
    if (!(zgsz3134)) {  goto case_203;  }
    uint64_t zgaz387;
    zgaz387 = sailgen_execution_mode_encdec_forwards(zM);
    uint64_t zgaz388;
    {
      uint64_t zgaz385;
      zgaz385 = sailgen_bool_bits_forwards(zSL);
      uint64_t zgaz386;
      {
        uint64_t zgaz383;
        zgaz383 = sailgen_bool_bits_forwards(zEL);
        uint64_t zgaz384;
        {
          uint64_t zgaz381;
          zgaz381 = sailgen_bool_bits_forwards(zLM);
          uint64_t zgaz382;
          {
            uint64_t zgaz379;
            zgaz379 = sailgen_bool_bits_forwards(zASR);
            uint64_t zgaz380;
            {
              uint64_t zgaz377;
              zgaz377 = sailgen_bool_bits_forwards(zX);
              uint64_t zgaz378;
              {
                uint64_t zgaz375;
                zgaz375 = sailgen_bool_bits_forwards(zR);
                uint64_t zgaz376;
                {
                  uint64_t zgaz373;
                  zgaz373 = sailgen_bool_bits_forwards(zW);
                  uint64_t zgaz374;
                  zgaz374 = sailgen_bool_bits_forwards(zC);
                  zgaz376 = (zgaz373 << 1) | zgaz374;
                }
                zgaz378 = (zgaz375 << 2) | zgaz376;
              }
              zgaz380 = (zgaz377 << 3) | zgaz378;
            }
            zgaz382 = (zgaz379 << 4) | zgaz380;
          }
          zgaz384 = (zgaz381 << 5) | zgaz382;
        }
        zgaz386 = (zgaz383 << 6) | zgaz384;
      }
      zgaz388 = (zgaz385 << 7) | zgaz386;
    }
    zcbz348 = (zgaz387 << 8) | zgaz388;
    goto finish_match_202;
  }
case_203: ;
  sail_match_failure("ap_m_encdec_backwards");
finish_match_202: ;
end_function_204: ;
  return zcbz348;
end_block_exception_205: ;

  return UINT64_C(0xdeadc0de);
}

static uint64_t sailgen_ap_m_encdec_backwards_infallible(struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9);

static uint64_t sailgen_ap_m_encdec_backwards_infallible(struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zargz3)
{
  __label__ case_207, finish_match_206, end_function_208, end_block_exception_209;

  uint64_t zcbz349;
  {
    bool zR;
    zR = zargz3.ztup0.zR;
    bool zW;
    zW = zargz3.ztup0.zW;
    bool zC;
    zC = zargz3.ztup0.zC;
    bool zX;
    zX = zargz3.ztup0.zX;
    bool zLM;
    zLM = zargz3.ztup0.zLM;
    bool zSL;
    zSL = zargz3.ztup0.zSL;
    bool zEL;
    zEL = zargz3.ztup0.zEL;
    bool zASR;
    zASR = zargz3.ztup0.zASR;
    enum zExecutionMode zM;
    zM = zargz3.ztup1;
    bool zgaz399;
    {
      bool zgaz389;
      zgaz389 = sailgen_not(zASR);
      bool zgsz3136;
      if (zgaz389) {  zgsz3136 = true;  } else {  zgsz3136 = zX;  }
      zgaz399 = zgsz3136;
    }
    bool zgsz3147;
    if (zgaz399) {
      bool zgaz398;
      {
        bool zgaz390;
        zgaz390 = sailgen_not(zC);
        bool zgsz3138;
        if (zgaz390) {  zgsz3138 = true;  } else {
          bool zgsz3137;
          if (zR) {  zgsz3137 = true;  } else {  zgsz3137 = zW;  }
          zgsz3138 = zgsz3137;
        }
        zgaz398 = zgsz3138;
      }
      bool zgsz3146;
      if (zgaz398) {
        bool zgaz397;
        {
          bool zgaz392;
          {
            enum zExecutionMode zgaz391;
            zgaz391 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
            zgaz392 = eq_zExecutionMode(zM, zgaz391);
          }
          bool zgsz3139;
          if (zgaz392) {  zgsz3139 = true;  } else {  zgsz3139 = zX;  }
          zgaz397 = zgsz3139;
        }
        bool zgsz3145;
        if (zgaz397) {
          bool zgaz396;
          {
            bool zgaz393;
            zgaz393 = sailgen_not(zSL);
            bool zgsz3140;
            if (zgaz393) {  zgsz3140 = true;  } else {  zgsz3140 = zC;  }
            zgaz396 = zgsz3140;
          }
          bool zgsz3144;
          if (zgaz396) {
            bool zgaz395;
            {
              bool zgaz394;
              {    if (zEL) {  zgaz394 = true;  } else {  zgaz394 = zLM;  }
              }
              zgaz395 = sailgen_not(zgaz394);
            }
            bool zgsz3143;
            if (zgaz395) {  zgsz3143 = true;  } else {
              bool zgsz3142;
              if (zC) {  zgsz3142 = zR;  } else {  zgsz3142 = false;  }
              zgsz3143 = zgsz3142;
            }
            zgsz3144 = zgsz3143;
          } else {  zgsz3144 = false;  }
          zgsz3145 = zgsz3144;
        } else {  zgsz3145 = false;  }
        zgsz3146 = zgsz3145;
      } else {  zgsz3146 = false;  }
      zgsz3147 = zgsz3146;
    } else {  zgsz3147 = false;  }
    bool zgsz3148;
    zgsz3148 = zgsz3147;
    if (!(zgsz3148)) {  goto case_207;  }
    uint64_t zgaz387;
    zgaz387 = sailgen_execution_mode_encdec_forwards(zM);
    uint64_t zgaz388;
    {
      uint64_t zgaz385;
      zgaz385 = sailgen_bool_bits_forwards(zSL);
      uint64_t zgaz386;
      {
        uint64_t zgaz383;
        zgaz383 = sailgen_bool_bits_forwards(zEL);
        uint64_t zgaz384;
        {
          uint64_t zgaz381;
          zgaz381 = sailgen_bool_bits_forwards(zLM);
          uint64_t zgaz382;
          {
            uint64_t zgaz379;
            zgaz379 = sailgen_bool_bits_forwards(zASR);
            uint64_t zgaz380;
            {
              uint64_t zgaz377;
              zgaz377 = sailgen_bool_bits_forwards(zX);
              uint64_t zgaz378;
              {
                uint64_t zgaz375;
                zgaz375 = sailgen_bool_bits_forwards(zR);
                uint64_t zgaz376;
                {
                  uint64_t zgaz373;
                  zgaz373 = sailgen_bool_bits_forwards(zW);
                  uint64_t zgaz374;
                  zgaz374 = sailgen_bool_bits_forwards(zC);
                  zgaz376 = (zgaz373 << 1) | zgaz374;
                }
                zgaz378 = (zgaz375 << 2) | zgaz376;
              }
              zgaz380 = (zgaz377 << 3) | zgaz378;
            }
            zgaz382 = (zgaz379 << 4) | zgaz380;
          }
          zgaz384 = (zgaz381 << 5) | zgaz382;
        }
        zgaz386 = (zgaz383 << 6) | zgaz384;
      }
      zgaz388 = (zgaz385 << 7) | zgaz386;
    }
    zcbz349 = (zgaz387 << 8) | zgaz388;
    goto finish_match_206;
  }
case_207: ;
finish_match_206: ;
end_function_208: ;
  return zcbz349;
end_block_exception_209: ;

  return UINT64_C(0xdeadc0de);
}

static bool sailgen_ap_m_encdec_forwards_matches(uint64_t zargz3)
{
  __label__ case_221, case_218, finish_match_217, end_function_222, end_block_exception_223;

  bool zcbz350;
  uint64_t zhead_expz3;
  zhead_expz3 = zargz3;
  struct zoptionzIozK zgaz3129;
  CREATE(zoptionzIozK)(&zgaz3129);
  {
    __label__ case_213, case_212, finish_match_211;

    {
      __label__ case_216, case_215, finish_match_214;

      uint64_t zv__1;
      zv__1 = zhead_expz3;
      uint64_t zmapping0z3;
      zmapping0z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__1 >> INT64_C(8)));
      uint64_t zmapping8z3;
      zmapping8z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__1 >> INT64_C(0)));
      uint64_t zmapping7z3;
      zmapping7z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__1 >> INT64_C(1)));
      uint64_t zmapping6z3;
      zmapping6z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__1 >> INT64_C(2)));
      uint64_t zmapping5z3;
      zmapping5z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__1 >> INT64_C(3)));
      uint64_t zmapping4z3;
      zmapping4z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__1 >> INT64_C(4)));
      uint64_t zmapping3z3;
      zmapping3z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__1 >> INT64_C(5)));
      uint64_t zmapping2z3;
      zmapping2z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__1 >> INT64_C(6)));
      uint64_t zmapping1z3;
      zmapping1z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__1 >> INT64_C(7)));
      uint64_t zmapping0z3shadowz311;
      zmapping0z3shadowz311 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__1 >> INT64_C(8)));
      bool zgaz3128;
      zgaz3128 = sailgen_execution_mode_encdec_backwards_matches(zmapping0z3shadowz311);
      bool zgsz3158;
      if (zgaz3128) {
        bool zgaz3127;
        zgaz3127 = sailgen_bool_bits_backwards_matches(zmapping1z3);
        bool zgsz3157;
        if (zgaz3127) {
          bool zgaz3126;
          zgaz3126 = sailgen_bool_bits_backwards_matches(zmapping2z3);
          bool zgsz3156;
          if (zgaz3126) {
            bool zgaz3125;
            zgaz3125 = sailgen_bool_bits_backwards_matches(zmapping3z3);
            bool zgsz3155;
            if (zgaz3125) {
              bool zgaz3124;
              zgaz3124 = sailgen_bool_bits_backwards_matches(zmapping4z3);
              bool zgsz3154;
              if (zgaz3124) {
                bool zgaz3123;
                zgaz3123 = sailgen_bool_bits_backwards_matches(zmapping5z3);
                bool zgsz3153;
                if (zgaz3123) {
                  bool zgaz3122;
                  zgaz3122 = sailgen_bool_bits_backwards_matches(zmapping6z3);
                  bool zgsz3152;
                  if (zgaz3122) {
                    bool zgaz3121;
                    zgaz3121 = sailgen_bool_bits_backwards_matches(zmapping7z3);
                    bool zgsz3151;
                    if (zgaz3121) {  zgsz3151 = sailgen_bool_bits_backwards_matches(zmapping8z3);  } else {
                      zgsz3151 = false;
                    }
                    zgsz3152 = zgsz3151;
                  } else {  zgsz3152 = false;  }
                  zgsz3153 = zgsz3152;
                } else {  zgsz3153 = false;  }
                zgsz3154 = zgsz3153;
              } else {  zgsz3154 = false;  }
              zgsz3155 = zgsz3154;
            } else {  zgsz3155 = false;  }
            zgsz3156 = zgsz3155;
          } else {  zgsz3156 = false;  }
          zgsz3157 = zgsz3156;
        } else {  zgsz3157 = false;  }
        zgsz3158 = zgsz3157;
      } else {  zgsz3158 = false;  }
      bool zgsz3175;
      zgsz3175 = zgsz3158;
      if (!(zgsz3175)) {  goto case_213;  }
      uint64_t zmapping0z3shadowz312;
      zmapping0z3shadowz312 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__1 >> INT64_C(8)));
      uint64_t zmapping8z3shadowz313;
      zmapping8z3shadowz313 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__1 >> INT64_C(0)));
      uint64_t zmapping7z3shadowz314;
      zmapping7z3shadowz314 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__1 >> INT64_C(1)));
      uint64_t zmapping6z3shadowz315;
      zmapping6z3shadowz315 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__1 >> INT64_C(2)));
      uint64_t zmapping5z3shadowz316;
      zmapping5z3shadowz316 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__1 >> INT64_C(3)));
      uint64_t zmapping4z3shadowz317;
      zmapping4z3shadowz317 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__1 >> INT64_C(4)));
      uint64_t zmapping3z3shadowz318;
      zmapping3z3shadowz318 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__1 >> INT64_C(5)));
      uint64_t zmapping2z3shadowz319;
      zmapping2z3shadowz319 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__1 >> INT64_C(6)));
      uint64_t zmapping1z3shadowz320;
      zmapping1z3shadowz320 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__1 >> INT64_C(7)));
      uint64_t zmapping0z3shadowz321;
      zmapping0z3shadowz321 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__1 >> INT64_C(8)));
      struct ztuple_z8z5enumz0zzExecutionModezCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolz9 zgaz3109;
      {
        enum zExecutionMode zgaz3100;
        zgaz3100 = sailgen_execution_mode_encdec_backwards_infallible(zmapping0z3shadowz321);
        bool zgaz3101;
        zgaz3101 = sailgen_bool_bits_backwards_infallible(zmapping1z3shadowz320);
        bool zgaz3102;
        zgaz3102 = sailgen_bool_bits_backwards_infallible(zmapping2z3shadowz319);
        bool zgaz3103;
        zgaz3103 = sailgen_bool_bits_backwards_infallible(zmapping3z3shadowz318);
        bool zgaz3104;
        zgaz3104 = sailgen_bool_bits_backwards_infallible(zmapping4z3shadowz317);
        bool zgaz3105;
        zgaz3105 = sailgen_bool_bits_backwards_infallible(zmapping5z3shadowz316);
        bool zgaz3106;
        zgaz3106 = sailgen_bool_bits_backwards_infallible(zmapping6z3shadowz315);
        bool zgaz3107;
        zgaz3107 = sailgen_bool_bits_backwards_infallible(zmapping7z3shadowz314);
        bool zgaz3108;
        zgaz3108 = sailgen_bool_bits_backwards_infallible(zmapping8z3shadowz313);
        zgaz3109.ztup0 = zgaz3100;
        zgaz3109.ztup1 = zgaz3101;
        zgaz3109.ztup2 = zgaz3102;
        zgaz3109.ztup3 = zgaz3103;
        zgaz3109.ztup4 = zgaz3104;
        zgaz3109.ztup5 = zgaz3105;
        zgaz3109.ztup6 = zgaz3106;
        zgaz3109.ztup7 = zgaz3107;
        zgaz3109.ztup8 = zgaz3108;
      }
      struct zoptionzIozK zgsz3160;
      CREATE(zoptionzIozK)(&zgsz3160);
      {
        enum zExecutionMode zM;
        zM = zgaz3109.ztup0;
        bool zSL;
        zSL = zgaz3109.ztup1;
        bool zEL;
        zEL = zgaz3109.ztup2;
        bool zLM;
        zLM = zgaz3109.ztup3;
        bool zASR;
        zASR = zgaz3109.ztup4;
        bool zX;
        zX = zgaz3109.ztup5;
        bool zR;
        zR = zgaz3109.ztup6;
        bool zW;
        zW = zgaz3109.ztup7;
        bool zC;
        zC = zgaz3109.ztup8;
        bool zgaz3120;
        {
          bool zgaz3110;
          zgaz3110 = sailgen_not(zASR);
          bool zgsz3162;
          if (zgaz3110) {  zgsz3162 = true;  } else {  zgsz3162 = zX;  }
          zgaz3120 = zgsz3162;
        }
        bool zgsz3173;
        if (zgaz3120) {
          bool zgaz3119;
          {
            bool zgaz3111;
            zgaz3111 = sailgen_not(zC);
            bool zgsz3164;
            if (zgaz3111) {  zgsz3164 = true;  } else {
              bool zgsz3163;
              if (zR) {  zgsz3163 = true;  } else {  zgsz3163 = zW;  }
              zgsz3164 = zgsz3163;
            }
            zgaz3119 = zgsz3164;
          }
          bool zgsz3172;
          if (zgaz3119) {
            bool zgaz3118;
            {
              bool zgaz3113;
              {
                enum zExecutionMode zgaz3112;
                zgaz3112 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
                zgaz3113 = eq_zExecutionMode(zM, zgaz3112);
              }
              bool zgsz3165;
              if (zgaz3113) {  zgsz3165 = true;  } else {  zgsz3165 = zX;  }
              zgaz3118 = zgsz3165;
            }
            bool zgsz3171;
            if (zgaz3118) {
              bool zgaz3117;
              {
                bool zgaz3114;
                zgaz3114 = sailgen_not(zSL);
                bool zgsz3166;
                if (zgaz3114) {  zgsz3166 = true;  } else {  zgsz3166 = zC;  }
                zgaz3117 = zgsz3166;
              }
              bool zgsz3170;
              if (zgaz3117) {
                bool zgaz3116;
                {
                  bool zgaz3115;
                  {    if (zEL) {  zgaz3115 = true;  } else {  zgaz3115 = zLM;  }
                  }
                  zgaz3116 = sailgen_not(zgaz3115);
                }
                bool zgsz3169;
                if (zgaz3116) {  zgsz3169 = true;  } else {
                  bool zgsz3168;
                  if (zC) {  zgsz3168 = zR;  } else {  zgsz3168 = false;  }
                  zgsz3169 = zgsz3168;
                }
                zgsz3170 = zgsz3169;
              } else {  zgsz3170 = false;  }
              zgsz3171 = zgsz3170;
            } else {  zgsz3171 = false;  }
            zgsz3172 = zgsz3171;
          } else {  zgsz3172 = false;  }
          zgsz3173 = zgsz3172;
        } else {  zgsz3173 = false;  }
        bool zgsz3174;
        zgsz3174 = zgsz3173;
        if (!(zgsz3174)) {  goto case_216;  }
        sailgen_SomezIozK(&zgsz3160, true);
        goto finish_match_214;
      }
    case_216: ;
      {
        sailgen_NonezIozK(&zgsz3160, UNIT);
        goto finish_match_214;
      }
    case_215: ;
    finish_match_214: ;
      COPY(zoptionzIozK)(&zgaz3129, zgsz3160);
      KILL(zoptionzIozK)(&zgsz3160);
      goto finish_match_211;
    }
  case_213: ;
    {
      sailgen_NonezIozK(&zgaz3129, UNIT);
      goto finish_match_211;
    }
  case_212: ;
  finish_match_211: ;
  }
  bool zgsz3176;
  {
    if (zgaz3129.kind != Kind_zSomezIozK) goto case_221;
    zgsz3176 = zgaz3129.variants.zSomezIozK;
    goto finish_match_217;
  }
case_221: ;
  {
    __label__ case_220, finish_match_219;

    /* complete */
    bool zgsz3177;
    {
      zgsz3177 = false;
      goto finish_match_219;
    }
  case_220: ;
  finish_match_219: ;
    zgsz3176 = zgsz3177;
    goto finish_match_217;
  }
case_218: ;
finish_match_217: ;
  zcbz350 = zgsz3176;
  KILL(zoptionzIozK)(&zgaz3129);
end_function_222: ;
  return zcbz350;
end_block_exception_223: ;

  return false;
}

static uint64_t znull_ap_m;
static void create_letbind_14(void) {


  uint64_t zgsz3181;
  zgsz3181 = UINT64_C(0b000000000);
  znull_ap_m = zgsz3181;
let_end_224: ;
}
static void kill_letbind_14(void) {
}

static uint64_t zinfinite_ap_m;
static void create_letbind_15(void) {


  uint64_t zgsz3182;
  zgsz3182 = UINT64_C(0b111111111);
  zinfinite_ap_m = zgsz3182;
let_end_225: ;
}
static void kill_letbind_15(void) {
}

static struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 sailgen_legalizzePermsAndMode(struct zArchPerms, enum zExecutionMode);

struct zArchPerms zghz377;
struct zArchPerms zghz378;
struct zArchPerms zghz379;
struct zArchPerms zghz380;
struct zArchPerms zghz381;
struct zArchPerms zghz382;
struct zArchPerms zghz383;
struct zArchPerms zghz384;
struct zArchPerms zghz385;
struct zArchPerms zghz386;

static void startup_sailgen_legalizzePermsAndMode(void)
{










}

static struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 sailgen_legalizzePermsAndMode(struct zArchPerms zperms, enum zExecutionMode zmode)
{
  __label__ end_function_227, end_block_exception_228;

  struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zcbz351;
  struct zArchPerms zghz377;
  {
    bool zgaz3138;
    {
      bool zgaz3137;
      zgaz3137 = zperms.zR;
      bool zgsz3183;
      if (zgaz3137) {  zgsz3183 = true;  } else {  zgsz3183 = zperms.zW;  }
      zgaz3138 = zgsz3183;
    }
    if (zgaz3138) {  zghz377 = zperms;  } else {
      struct zArchPerms zghz386;
      zghz386 = zperms;
      zghz386.zC = false;
      zghz377 = zghz386;
    }
  }
  struct zArchPerms zghz378;
  {
    bool zgaz3136;
    {
      bool zgaz3135;
      zgaz3135 = zghz377.zC;
      bool zgsz3185;
      if (zgaz3135) {  zgsz3185 = zghz377.zR;  } else {  zgsz3185 = false;  }
      zgaz3136 = zgsz3185;
    }
    if (zgaz3136) {  zghz378 = zghz377;  } else {
      struct zArchPerms zghz385;
      zghz385 = zghz377;
      zghz385.zEL = false;
      zghz378 = zghz385;
    }
  }
  struct zArchPerms zghz379;
  {
    bool zgaz3134;
    {
      bool zgaz3133;
      zgaz3133 = zghz378.zC;
      bool zgsz3187;
      if (zgaz3133) {  zgsz3187 = zghz378.zR;  } else {  zgsz3187 = false;  }
      zgaz3134 = zgsz3187;
    }
    if (zgaz3134) {  zghz379 = zghz378;  } else {
      struct zArchPerms zghz384;
      zghz384 = zghz378;
      zghz384.zLM = false;
      zghz379 = zghz384;
    }
  }
  struct zArchPerms zghz380;
  {
    bool zgaz3132;
    zgaz3132 = zghz379.zC;
    if (zgaz3132) {  zghz380 = zghz379;  } else {
      struct zArchPerms zghz383;
      zghz383 = zghz379;
      zghz383.zSL = false;
      zghz380 = zghz383;
    }
  }
  struct zArchPerms zghz381;
  {
    bool zgaz3131;
    zgaz3131 = zghz380.zX;
    if (zgaz3131) {  zghz381 = zghz380;  } else {
      struct zArchPerms zghz382;
      zghz382 = zghz380;
      zghz382.zASR = false;
      zghz381 = zghz382;
    }
  }
  enum zExecutionMode zmodeshadowz327;
  {
    bool zgaz3130;
    zgaz3130 = zghz381.zX;
    if (zgaz3130) {  zmodeshadowz327 = zmode;  } else {
      zmodeshadowz327 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    }
  }
  zcbz351.ztup0 = zghz381;
  zcbz351.ztup1 = zmodeshadowz327;
end_function_227: ;
  return zcbz351;
end_block_exception_228: ;
  struct zArchPerms zcbz3102 = { .zASR = false, .zC = false, .zEL = false, .zLM = false, .zR = false, .zSL = false, .zW = false, .zX = false };
  struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zcbz3101 = { .ztup0 = zcbz3102, .ztup1 = zCapPtrMode };
  return zcbz3101;
}



static void finish_sailgen_legalizzePermsAndMode(void)
{
}

static uint64_t sailgen_getMetadataL8(struct zEncMetadata);

static uint64_t sailgen_getMetadataL8(struct zEncMetadata zmeta)
{
  __label__ end_function_230, end_block_exception_231;

  uint64_t zcbz352;
  zcbz352 = UINT64_C(0b0);
end_function_230: ;
  return zcbz352;
end_block_exception_231: ;

  return UINT64_C(0xdeadc0de);
}

static struct zEncMetadata sailgen_setMetadataL8(struct zEncMetadata, uint64_t);

struct zEncMetadata zghz387;

static void startup_sailgen_setMetadataL8(void)
{
}

static struct zEncMetadata sailgen_setMetadataL8(struct zEncMetadata zmeta, uint64_t zL8)
{
  __label__ end_function_233, end_block_exception_234;

  struct zEncMetadata zghz387;
  zghz387 = zmeta;
end_function_233: ;
  return zghz387;
end_block_exception_234: ;
  struct zEncMetadata zcbz3103 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz3103;
}



static void finish_sailgen_setMetadataL8(void)
{
}

static uint64_t sailgen_getMetadataInternalExponent(struct zEncMetadata);

static uint64_t sailgen_getMetadataInternalExponent(struct zEncMetadata zmeta)
{
  __label__ end_function_236, end_block_exception_237;

  uint64_t zcbz354;
  uint64_t zgaz3139;
  zgaz3139 = sailgen__get_EncMetadata_TE(zmeta);
  uint64_t zgaz3140;
  zgaz3140 = sailgen__get_EncMetadata_BE(zmeta);
  zcbz354 = (zgaz3139 << 3) | zgaz3140;
end_function_236: ;
  return zcbz354;
end_block_exception_237: ;

  return UINT64_C(0xdeadc0de);
}

static int64_t zcap_sizze;
static void create_letbind_16(void) {


  int64_t zgsz3192;
  zgsz3192 = INT64_C(16);
  zcap_sizze = zgsz3192;
let_end_238: ;
}
static void kill_letbind_16(void) {
}

static int64_t zcap_addr_width;
static void create_letbind_17(void) {


  int64_t zgsz3193;
  zgsz3193 = INT64_C(64);
  zcap_addr_width = zgsz3193;
let_end_239: ;
}
static void kill_letbind_17(void) {
}

static int64_t zcap_len_width;
static void create_letbind_18(void) {


  int64_t zgsz3194;
  zgsz3194 = INT64_C(65);
  zcap_len_width = zgsz3194;
let_end_240: ;
}
static void kill_letbind_18(void) {
}

static sail_int zcap_max_addr;
static void create_letbind_19(void) {    CREATE(sail_int)(&zcap_max_addr);


  sail_int zgsz3195;
  CREATE(sail_int)(&zgsz3195);
  {
    sail_int zgsz3400;
    CREATE(sail_int)(&zgsz3400);
    CONVERT_OF(sail_int, mach_int)(&zgsz3400, zcap_addr_width);
    sailgen_MAX(&zgsz3195, zgsz3400);
    KILL(sail_int)(&zgsz3400);
  }
  COPY(sail_int)(&zcap_max_addr, zgsz3195);
  KILL(sail_int)(&zgsz3195);
let_end_241: ;
}
static void kill_letbind_19(void) {    KILL(sail_int)(&zcap_max_addr);
}

static int64_t zcap_xe_width;
static void create_letbind_20(void) {


  int64_t zgsz3196;
  zgsz3196 = INT64_C(3);
  zcap_xe_width = zgsz3196;
let_end_242: ;
}
static void kill_letbind_20(void) {
}

static int64_t zcap_max_E;
static void create_letbind_21(void) {


  int64_t zgsz3197;
  zgsz3197 = INT64_C(52);
  zcap_max_E = zgsz3197;
let_end_243: ;
}
static void kill_letbind_21(void) {
}

static int64_t zcap_min_E;
static void create_letbind_22(void) {


  int64_t zgsz3198;
  zgsz3198 = INT64_C(-11);
  zcap_min_E = zgsz3198;
let_end_244: ;
}
static void kill_letbind_22(void) {
}

static int64_t zlvlbits;
static void create_letbind_23(void) {


  int64_t zgsz3199;
  zgsz3199 = INT64_C(1);
  zlvlbits = zgsz3199;
let_end_245: ;
}
static void kill_letbind_23(void) {
}

static uint64_t sailgen_internal_exponent_encdec_forwards(bool);

static bool sailgen_internal_exponent_encdec_backwards(uint64_t);

static uint64_t sailgen_internal_exponent_encdec_forwards(bool zargz3)
{
  __label__ case_249, case_248, finish_match_247, end_function_250, end_block_exception_251;

  uint64_t zcbz355;
  {
    bool zp0z3;
    zp0z3 = zargz3;
    bool zgsz3202;
    zgsz3202 = eq_bool(zp0z3, false);
    if (!(zgsz3202)) {  goto case_249;  }
    zcbz355 = UINT64_C(0b1);
    goto finish_match_247;
  }
case_249: ;
  {
    bool zuz32;
    zuz32 = zargz3;
    bool zgsz3201;
    zgsz3201 = eq_bool(zuz32, true);
    if (!(zgsz3201)) {  goto case_248;  }
    zcbz355 = UINT64_C(0b0);
    goto finish_match_247;
  }
case_248: ;
finish_match_247: ;
end_function_250: ;
  return zcbz355;
end_block_exception_251: ;

  return UINT64_C(0xdeadc0de);
}

static uint64_t sailgen_internal_exponent_encdec_forwards_infallible(bool);

static uint64_t sailgen_internal_exponent_encdec_forwards_infallible(bool zargz3)
{
  __label__ case_254, case_253, finish_match_252, end_function_255, end_block_exception_256;

  uint64_t zcbz356;
  {
    bool zp0z3;
    zp0z3 = zargz3;
    bool zgsz3205;
    zgsz3205 = eq_bool(zp0z3, false);
    if (!(zgsz3205)) {  goto case_254;  }
    zcbz356 = UINT64_C(0b1);
    goto finish_match_252;
  }
case_254: ;
  {
    bool zuz33;
    zuz33 = zargz3;
    bool zgsz3204;
    zgsz3204 = eq_bool(zuz33, true);
    if (!(zgsz3204)) {  goto case_253;  }
    zcbz356 = UINT64_C(0b0);
    goto finish_match_252;
  }
case_253: ;
finish_match_252: ;
end_function_255: ;
  return zcbz356;
end_block_exception_256: ;

  return UINT64_C(0xdeadc0de);
}

static bool sailgen_internal_exponent_encdec_backwards(uint64_t zargz3)
{
  __label__ case_260, case_259, finish_match_258, end_function_261, end_block_exception_262;

  bool zcbz357;
  {
    uint64_t zb__0;
    zb__0 = zargz3;
    bool zgsz3208;
    zgsz3208 = (zb__0 == UINT64_C(0b1));
    if (!(zgsz3208)) {  goto case_260;  }
    zcbz357 = false;
    goto finish_match_258;
  }
case_260: ;
  {
    zcbz357 = true;
    goto finish_match_258;
  }
case_259: ;
finish_match_258: ;
end_function_261: ;
  return zcbz357;
end_block_exception_262: ;

  return false;
}

static bool sailgen_internal_exponent_encdec_backwards_infallible(uint64_t);

static bool sailgen_internal_exponent_encdec_backwards_infallible(uint64_t zargz3)
{
  __label__ case_265, case_264, finish_match_263, end_function_266, end_block_exception_267;

  bool zcbz358;
  {
    uint64_t zb__0;
    zb__0 = zargz3;
    bool zgsz3211;
    zgsz3211 = (zb__0 == UINT64_C(0b1));
    if (!(zgsz3211)) {  goto case_265;  }
    zcbz358 = false;
    goto finish_match_263;
  }
case_265: ;
  {
    zcbz358 = true;
    goto finish_match_263;
  }
case_264: ;
finish_match_263: ;
end_function_266: ;
  return zcbz358;
end_block_exception_267: ;

  return false;
}

static struct zCapability sailgen_undefined_Capability(unit);

struct zCapability zghz388;
sail_int zghz389;

static void startup_sailgen_undefined_Capability(void)
{

  CREATE(sail_int)(&zghz389);
}

static struct zCapability sailgen_undefined_Capability(unit zgsz3212)
{
  __label__ end_function_269, end_block_exception_270;

  struct zCapability zghz388;
  zghz388.zB = UINT64_C(0b00000000000000);
  RECREATE(sail_int)(&zghz389);
  CONVERT_OF(sail_int, mach_int)(&zghz389, INT64_C(-11));
  zghz388.zE = CONVERT_OF(mach_int, sail_int)(zghz389);
  zghz388.zT = UINT64_C(0b00000000000000);
  zghz388.zaddress = UINT64_C(0x0000000000000000);
  zghz388.zap_m = UINT64_C(0b000000000);
  zghz388.zcl = UINT64_C(0b0);
  zghz388.zinternal_exponent = false;
  zghz388.zreserved_0 = UINT64_C(0b000000000000000);
  zghz388.zreserved_1 = UINT64_C(0b0000000);
  zghz388.zsd_perms = UINT64_C(0x0);
  zghz388.zsealed = false;
  zghz388.ztag = false;
end_function_269: ;
  return zghz388;
end_block_exception_270: ;
  struct zCapability zcbz3104 = { .zB = UINT64_C(0xdeadc0de), .zE = INT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zap_m = UINT64_C(0xdeadc0de), .zcl = UINT64_C(0xdeadc0de), .zinternal_exponent = false, .zreserved_0 = UINT64_C(0xdeadc0de), .zreserved_1 = UINT64_C(0xdeadc0de), .zsd_perms = UINT64_C(0xdeadc0de), .zsealed = false, .ztag = false };
  return zcbz3104;
}



static void finish_sailgen_undefined_Capability(void)
{    KILL(sail_int)(&zghz389);
}

static struct zCapability sailgen_bitsToCap(bool, lbits);

struct zCapability zghz390;
struct zEncMetadata zghz391;
struct zCapability zghz392;
sail_int zghz393;
sail_int zghz394;
lbits zghz395;
sail_int zghz396;
sail_int zghz397;
sail_int zghz398;
lbits zghz399;
lbits zghz3100;
lbits zghz3101;
lbits zghz3102;
lbits zghz3103;
lbits zghz3104;
lbits zghz3105;
sail_int zghz3106;
sail_int zghz3107;
lbits zghz3108;
sail_int zghz3109;
sail_int zghz3110;
sail_int zghz3111;
sail_int zghz3112;
sail_int zghz3113;
sail_int zghz3114;
lbits zghz3115;
lbits zghz3116;
lbits zghz3117;
sail_int zghz3118;
sail_int zghz3119;
lbits zghz3120;
sail_int zghz3121;
sail_int zghz3122;
sail_int zghz3123;
sail_int zghz3124;
sail_int zghz3125;
sail_int zghz3126;
lbits zghz3127;
sail_int zghz3128;
sail_int zghz3129;
sail_int zghz3130;
lbits zghz3131;
sail_int zghz3132;
sail_int zghz3133;
sail_int zghz3134;
sail_int zghz3135;
sail_int zghz3136;
sail_int zghz3137;

static void startup_sailgen_bitsToCap(void)
{



  CREATE(sail_int)(&zghz393);
  CREATE(sail_int)(&zghz394);
  CREATE(lbits)(&zghz395);
  CREATE(sail_int)(&zghz396);
  CREATE(sail_int)(&zghz397);
  CREATE(sail_int)(&zghz398);
  CREATE(lbits)(&zghz399);
  CREATE(lbits)(&zghz3100);
  CREATE(lbits)(&zghz3101);
  CREATE(lbits)(&zghz3102);
  CREATE(lbits)(&zghz3103);
  CREATE(lbits)(&zghz3104);
  CREATE(lbits)(&zghz3105);
  CREATE(sail_int)(&zghz3106);
  CREATE(sail_int)(&zghz3107);
  CREATE(lbits)(&zghz3108);
  CREATE(sail_int)(&zghz3109);
  CREATE(sail_int)(&zghz3110);
  CREATE(sail_int)(&zghz3111);
  CREATE(sail_int)(&zghz3112);
  CREATE(sail_int)(&zghz3113);
  CREATE(sail_int)(&zghz3114);
  CREATE(lbits)(&zghz3115);
  CREATE(lbits)(&zghz3116);
  CREATE(lbits)(&zghz3117);
  CREATE(sail_int)(&zghz3118);
  CREATE(sail_int)(&zghz3119);
  CREATE(lbits)(&zghz3120);
  CREATE(sail_int)(&zghz3121);
  CREATE(sail_int)(&zghz3122);
  CREATE(sail_int)(&zghz3123);
  CREATE(sail_int)(&zghz3124);
  CREATE(sail_int)(&zghz3125);
  CREATE(sail_int)(&zghz3126);
  CREATE(lbits)(&zghz3127);
  CREATE(sail_int)(&zghz3128);
  CREATE(sail_int)(&zghz3129);
  CREATE(sail_int)(&zghz3130);
  CREATE(lbits)(&zghz3131);
  CREATE(sail_int)(&zghz3132);
  CREATE(sail_int)(&zghz3133);
  CREATE(sail_int)(&zghz3134);
  CREATE(sail_int)(&zghz3135);
  CREATE(sail_int)(&zghz3136);
  CREATE(sail_int)(&zghz3137);
}

static struct zCapability sailgen_bitsToCap(bool ztag, lbits zcap_bits)
{
  __label__ case_273, finish_match_272, end_function_274, end_block_exception_275;

  struct zCapability zghz390;
  struct zEncMetadata zghz391;
  {
    uint64_t zgaz3168;
    {
      int64_t zgaz3167;
      {
        int64_t zgaz3166;
        {
          RECREATE(sail_int)(&zghz3135);
          CONVERT_OF(sail_int, mach_int)(&zghz3135, zxlen);
          RECREATE(sail_int)(&zghz3136);
          CONVERT_OF(sail_int, mach_int)(&zghz3136, INT64_C(2));
          RECREATE(sail_int)(&zghz3137);
          mult_int(&zghz3137, zghz3135, zghz3136);
          zgaz3166 = CONVERT_OF(mach_int, sail_int)(zghz3137);
        }
        {
          RECREATE(sail_int)(&zghz3132);
          CONVERT_OF(sail_int, mach_int)(&zghz3132, zgaz3166);
          RECREATE(sail_int)(&zghz3133);
          CONVERT_OF(sail_int, mach_int)(&zghz3133, INT64_C(1));
          RECREATE(sail_int)(&zghz3134);
          sub_int(&zghz3134, zghz3132, zghz3133);
          zgaz3167 = CONVERT_OF(mach_int, sail_int)(zghz3134);
        }
      }
      {
        RECREATE(sail_int)(&zghz3129);
        CONVERT_OF(sail_int, mach_int)(&zghz3129, zgaz3167);
        RECREATE(sail_int)(&zghz3130);
        CONVERT_OF(sail_int, mach_int)(&zghz3130, zxlen);
        RECREATE(lbits)(&zghz3131);
        vector_subrange_lbits(&zghz3131, zcap_bits, zghz3129, zghz3130);
        zgaz3168 = CONVERT_OF(fbits, lbits)(zghz3131, true);
      }
    }
    zghz391 = sailgen_Mk_EncMetadata(zgaz3168);
  }
  bool zinternal_exponent;
  {
    uint64_t zgaz3165;
    zgaz3165 = sailgen__get_EncMetadata_EF(zghz391);
    zinternal_exponent = sailgen_internal_exponent_encdec_backwards(zgaz3165);
  }
  {
    int64_t zMW;
    zMW = zcap_mantissa_width;
    int64_t zE;
    if (zinternal_exponent) {
      int64_t zgaz3164;
      {
        uint64_t zgaz3163;
        zgaz3163 = sailgen_getMetadataInternalExponent(zghz391);
        {
          RECREATE(lbits)(&zghz3127);
          CONVERT_OF(lbits, fbits)(&zghz3127, zgaz3163, UINT64_C(6) , true);
          RECREATE(sail_int)(&zghz3128);
          sail_unsigned(&zghz3128, zghz3127);
          zgaz3164 = CONVERT_OF(mach_int, sail_int)(zghz3128);
        }
      }
      {
        RECREATE(sail_int)(&zghz3124);
        CONVERT_OF(sail_int, mach_int)(&zghz3124, zcap_max_E);
        RECREATE(sail_int)(&zghz3125);
        CONVERT_OF(sail_int, mach_int)(&zghz3125, zgaz3164);
        RECREATE(sail_int)(&zghz3126);
        sub_int(&zghz3126, zghz3124, zghz3125);
        zE = CONVERT_OF(mach_int, sail_int)(zghz3126);
      }
    } else {  zE = INT64_C(0);  }
    uint64_t zLMSB;
    if (zinternal_exponent) {  zLMSB = UINT64_C(0b01);  } else {
      uint64_t zgaz3162;
      zgaz3162 = sailgen_getMetadataL8(zghz391);
      zLMSB = (UINT64_C(0b0) << 1) | zgaz3162;
    }
    uint64_t zT_low;
    {
      bool zgaz3161;
      zgaz3161 = sailgen_not(zinternal_exponent);
      if (zgaz3161) {  zT_low = sailgen__get_EncMetadata_TE(zghz391);  } else {  zT_low = UINT64_C(0b000);  }
    }
    uint64_t zB_low;
    {
      bool zgaz3160;
      zgaz3160 = sailgen_not(zinternal_exponent);
      if (zgaz3160) {  zB_low = sailgen__get_EncMetadata_BE(zghz391);  } else {  zB_low = UINT64_C(0b000);  }
    }
    sbits zT;
    {
      uint64_t zgaz3159;
      zgaz3159 = sailgen__get_EncMetadata_Tm(zghz391);
      zT = CONVERT_OF(sbits, fbits)((zgaz3159 << 3) | zT_low, UINT64_C(12) , true);
    }
    sbits zB;
    {
      uint64_t zgaz3158;
      zgaz3158 = sailgen__get_EncMetadata_Bm(zghz391);
      zB = CONVERT_OF(sbits, fbits)((zgaz3158 << 3) | zB_low, UINT64_C(14) , true);
    }
    uint64_t zLCout;
    {
      bool zgaz3157;
      {
        sbits zgaz3156;
        {
          int64_t zgaz3155;
          {
            RECREATE(sail_int)(&zghz3121);
            CONVERT_OF(sail_int, mach_int)(&zghz3121, zMW);
            RECREATE(sail_int)(&zghz3122);
            CONVERT_OF(sail_int, mach_int)(&zghz3122, INT64_C(3));
            RECREATE(sail_int)(&zghz3123);
            sub_int(&zghz3123, zghz3121, zghz3122);
            zgaz3155 = CONVERT_OF(mach_int, sail_int)(zghz3123);
          }
          {
            RECREATE(lbits)(&zghz3117);
            CONVERT_OF(lbits, sbits)(&zghz3117, zB, true);
            RECREATE(sail_int)(&zghz3118);
            CONVERT_OF(sail_int, mach_int)(&zghz3118, zgaz3155);
            RECREATE(sail_int)(&zghz3119);
            CONVERT_OF(sail_int, mach_int)(&zghz3119, INT64_C(0));
            RECREATE(lbits)(&zghz3120);
            vector_subrange_lbits(&zghz3120, zghz3117, zghz3118, zghz3119);
            zgaz3156 = CONVERT_OF(sbits, lbits)(zghz3120, true);
          }
        }
        {
          RECREATE(lbits)(&zghz3115);
          CONVERT_OF(lbits, sbits)(&zghz3115, zT, true);
          RECREATE(lbits)(&zghz3116);
          CONVERT_OF(lbits, sbits)(&zghz3116, zgaz3156, true);
          zgaz3157 = sailgen_z8operatorz0zI_uz9(zghz3115, zghz3116);
        }
      }
      if (zgaz3157) {  zLCout = UINT64_C(0b01);  } else {  zLCout = UINT64_C(0b00);  }
    }
    sbits zTshadowz328;
    {
      sbits zgaz3154;
      {
        sbits zgaz3153;
        {
          sbits zgaz3152;
          {
            int64_t zgaz3150;
            {
              RECREATE(sail_int)(&zghz3112);
              CONVERT_OF(sail_int, mach_int)(&zghz3112, zMW);
              RECREATE(sail_int)(&zghz3113);
              CONVERT_OF(sail_int, mach_int)(&zghz3113, INT64_C(1));
              RECREATE(sail_int)(&zghz3114);
              sub_int(&zghz3114, zghz3112, zghz3113);
              zgaz3150 = CONVERT_OF(mach_int, sail_int)(zghz3114);
            }
            int64_t zgaz3151;
            {
              RECREATE(sail_int)(&zghz3109);
              CONVERT_OF(sail_int, mach_int)(&zghz3109, zMW);
              RECREATE(sail_int)(&zghz3110);
              CONVERT_OF(sail_int, mach_int)(&zghz3110, INT64_C(2));
              RECREATE(sail_int)(&zghz3111);
              sub_int(&zghz3111, zghz3109, zghz3110);
              zgaz3151 = CONVERT_OF(mach_int, sail_int)(zghz3111);
            }
            {
              RECREATE(lbits)(&zghz3105);
              CONVERT_OF(lbits, sbits)(&zghz3105, zB, true);
              RECREATE(sail_int)(&zghz3106);
              CONVERT_OF(sail_int, mach_int)(&zghz3106, zgaz3150);
              RECREATE(sail_int)(&zghz3107);
              CONVERT_OF(sail_int, mach_int)(&zghz3107, zgaz3151);
              RECREATE(lbits)(&zghz3108);
              vector_subrange_lbits(&zghz3108, zghz3105, zghz3106, zghz3107);
              zgaz3152 = CONVERT_OF(sbits, lbits)(zghz3108, true);
            }
          }
          {
            RECREATE(lbits)(&zghz3102);
            CONVERT_OF(lbits, sbits)(&zghz3102, zgaz3152, true);
            RECREATE(lbits)(&zghz3103);
            CONVERT_OF(lbits, fbits)(&zghz3103, zLCout, UINT64_C(2) , true);
            RECREATE(lbits)(&zghz3104);
            add_bits(&zghz3104, zghz3102, zghz3103);
            zgaz3153 = CONVERT_OF(sbits, lbits)(zghz3104, true);
          }
        }
        {
          RECREATE(lbits)(&zghz399);
          CONVERT_OF(lbits, sbits)(&zghz399, zgaz3153, true);
          RECREATE(lbits)(&zghz3100);
          CONVERT_OF(lbits, fbits)(&zghz3100, zLMSB, UINT64_C(2) , true);
          RECREATE(lbits)(&zghz3101);
          add_bits(&zghz3101, zghz399, zghz3100);
          zgaz3154 = CONVERT_OF(sbits, lbits)(zghz3101, true);
        }
      }
      zTshadowz328 = append_ss(zgaz3154, zT);
    }
    uint64_t zgaz3141;
    zgaz3141 = sailgen__get_EncMetadata_SDP(zghz391);
    uint64_t zgaz3142;
    zgaz3142 = sailgen__get_EncMetadata_AP_M(zghz391);
    uint64_t zgaz3143;
    zgaz3143 = sailgen__get_EncMetadata_CL(zghz391);
    bool zgaz3145;
    {
      uint64_t zgaz3144;
      zgaz3144 = sailgen__get_EncMetadata_CT(zghz391);
      zgaz3145 = sailgen_bool_bits_backwards(zgaz3144);
    }
    uint64_t zgaz3146;
    zgaz3146 = sailgen__get_EncMetadata_reserved_0(zghz391);
    uint64_t zgaz3147;
    zgaz3147 = sailgen__get_EncMetadata_reserved_1(zghz391);
    uint64_t zgaz3149;
    {
      int64_t zgaz3148;
      {
        RECREATE(sail_int)(&zghz396);
        CONVERT_OF(sail_int, mach_int)(&zghz396, zxlen);
        RECREATE(sail_int)(&zghz397);
        CONVERT_OF(sail_int, mach_int)(&zghz397, INT64_C(1));
        RECREATE(sail_int)(&zghz398);
        sub_int(&zghz398, zghz396, zghz397);
        zgaz3148 = CONVERT_OF(mach_int, sail_int)(zghz398);
      }
      {
        RECREATE(sail_int)(&zghz393);
        CONVERT_OF(sail_int, mach_int)(&zghz393, zgaz3148);
        RECREATE(sail_int)(&zghz394);
        CONVERT_OF(sail_int, mach_int)(&zghz394, INT64_C(0));
        RECREATE(lbits)(&zghz395);
        vector_subrange_lbits(&zghz395, zcap_bits, zghz393, zghz394);
        zgaz3149 = CONVERT_OF(fbits, lbits)(zghz395, true);
      }
    }
    struct zCapability zghz392;
    zghz392.zB = CONVERT_OF(fbits, sbits)(zB, true);
    zghz392.zE = zE;
    zghz392.zT = CONVERT_OF(fbits, sbits)(zTshadowz328, true);
    zghz392.zaddress = zgaz3149;
    zghz392.zap_m = zgaz3142;
    zghz392.zcl = zgaz3143;
    zghz392.zinternal_exponent = zinternal_exponent;
    zghz392.zreserved_0 = zgaz3146;
    zghz392.zreserved_1 = zgaz3147;
    zghz392.zsd_perms = zgaz3141;
    zghz392.zsealed = zgaz3145;
    zghz392.ztag = ztag;
    zghz390 = zghz392;
    goto finish_match_272;
  }
case_273: ;
  sail_match_failure("bitsToCap");
finish_match_272: ;
end_function_274: ;
  return zghz390;
end_block_exception_275: ;
  struct zCapability zcbz3105 = { .zB = UINT64_C(0xdeadc0de), .zE = INT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zap_m = UINT64_C(0xdeadc0de), .zcl = UINT64_C(0xdeadc0de), .zinternal_exponent = false, .zreserved_0 = UINT64_C(0xdeadc0de), .zreserved_1 = UINT64_C(0xdeadc0de), .zsd_perms = UINT64_C(0xdeadc0de), .zsealed = false, .ztag = false };
  return zcbz3105;
}



static void finish_sailgen_bitsToCap(void)
{
  KILL(sail_int)(&zghz3137);
  KILL(sail_int)(&zghz3136);
  KILL(sail_int)(&zghz3135);
  KILL(sail_int)(&zghz3134);
  KILL(sail_int)(&zghz3133);
  KILL(sail_int)(&zghz3132);
  KILL(lbits)(&zghz3131);
  KILL(sail_int)(&zghz3130);
  KILL(sail_int)(&zghz3129);
  KILL(sail_int)(&zghz3128);
  KILL(lbits)(&zghz3127);
  KILL(sail_int)(&zghz3126);
  KILL(sail_int)(&zghz3125);
  KILL(sail_int)(&zghz3124);
  KILL(sail_int)(&zghz3123);
  KILL(sail_int)(&zghz3122);
  KILL(sail_int)(&zghz3121);
  KILL(lbits)(&zghz3120);
  KILL(sail_int)(&zghz3119);
  KILL(sail_int)(&zghz3118);
  KILL(lbits)(&zghz3117);
  KILL(lbits)(&zghz3116);
  KILL(lbits)(&zghz3115);
  KILL(sail_int)(&zghz3114);
  KILL(sail_int)(&zghz3113);
  KILL(sail_int)(&zghz3112);
  KILL(sail_int)(&zghz3111);
  KILL(sail_int)(&zghz3110);
  KILL(sail_int)(&zghz3109);
  KILL(lbits)(&zghz3108);
  KILL(sail_int)(&zghz3107);
  KILL(sail_int)(&zghz3106);
  KILL(lbits)(&zghz3105);
  KILL(lbits)(&zghz3104);
  KILL(lbits)(&zghz3103);
  KILL(lbits)(&zghz3102);
  KILL(lbits)(&zghz3101);
  KILL(lbits)(&zghz3100);
  KILL(lbits)(&zghz399);
  KILL(sail_int)(&zghz398);
  KILL(sail_int)(&zghz397);
  KILL(sail_int)(&zghz396);
  KILL(lbits)(&zghz395);
  KILL(sail_int)(&zghz394);
  KILL(sail_int)(&zghz393);
}

static struct zEncMetadata sailgen_capToMetadataBits(struct zCapability);

struct zEncMetadata zghz3138;
struct zEncMetadata zghz3139;
struct zEncMetadata zghz3140;
struct zEncMetadata zghz3141;
struct zEncMetadata zghz3142;
struct zEncMetadata zghz3143;
struct zEncMetadata zghz3144;
sail_int zghz3145;
sail_int zghz3146;
sail_int zghz3147;
struct zEncMetadata zghz3148;
sail_int zghz3149;
sail_int zghz3150;
sail_int zghz3151;
struct zEncMetadata zghz3152;
struct zEncMetadata zghz3153;
struct zEncMetadata zghz3154;
struct zEncMetadata zghz3155;
struct zEncMetadata zghz3156;
sail_int zghz3157;
sail_int zghz3158;
sail_int zghz3159;
sail_int zghz3160;
sail_int zghz3161;
sail_int zghz3162;
sail_int zghz3163;
sail_int zghz3164;
sail_int zghz3165;
sail_int zghz3166;
sail_int zghz3167;
sail_int zghz3168;
sail_int zghz3169;
sail_int zghz3170;
sail_int zghz3171;
sail_int zghz3172;
sail_int zghz3173;
sail_int zghz3174;
sail_int zghz3175;
sail_int zghz3176;
lbits zghz3177;
sail_int zghz3178;
sail_int zghz3179;
sail_int zghz3180;

static void startup_sailgen_capToMetadataBits(void)
{







  CREATE(sail_int)(&zghz3145);
  CREATE(sail_int)(&zghz3146);
  CREATE(sail_int)(&zghz3147);

  CREATE(sail_int)(&zghz3149);
  CREATE(sail_int)(&zghz3150);
  CREATE(sail_int)(&zghz3151);





  CREATE(sail_int)(&zghz3157);
  CREATE(sail_int)(&zghz3158);
  CREATE(sail_int)(&zghz3159);
  CREATE(sail_int)(&zghz3160);
  CREATE(sail_int)(&zghz3161);
  CREATE(sail_int)(&zghz3162);
  CREATE(sail_int)(&zghz3163);
  CREATE(sail_int)(&zghz3164);
  CREATE(sail_int)(&zghz3165);
  CREATE(sail_int)(&zghz3166);
  CREATE(sail_int)(&zghz3167);
  CREATE(sail_int)(&zghz3168);
  CREATE(sail_int)(&zghz3169);
  CREATE(sail_int)(&zghz3170);
  CREATE(sail_int)(&zghz3171);
  CREATE(sail_int)(&zghz3172);
  CREATE(sail_int)(&zghz3173);
  CREATE(sail_int)(&zghz3174);
  CREATE(sail_int)(&zghz3175);
  CREATE(sail_int)(&zghz3176);
  CREATE(lbits)(&zghz3177);
  CREATE(sail_int)(&zghz3178);
  CREATE(sail_int)(&zghz3179);
  CREATE(sail_int)(&zghz3180);
}

// struct tuple_(%bit, %bv3, %bv3)
struct ztuple_z8z5bitzCz0z5bv3zCz0z5bv3z9 {
  fbits ztup0;
  uint64_t ztup1;
  uint64_t ztup2;
};

static void COPY(ztuple_z8z5bitzCz0z5bv3zCz0z5bv3z9)(struct ztuple_z8z5bitzCz0z5bv3zCz0z5bv3z9 *rop, const struct ztuple_z8z5bitzCz0z5bv3zCz0z5bv3z9 op) {
  rop->ztup0 = op.ztup0;
  rop->ztup1 = op.ztup1;
  rop->ztup2 = op.ztup2;
}

static bool EQUAL(ztuple_z8z5bitzCz0z5bv3zCz0z5bv3z9)(struct ztuple_z8z5bitzCz0z5bv3zCz0z5bv3z9 op1, struct ztuple_z8z5bitzCz0z5bv3zCz0z5bv3z9 op2) {
  return EQUAL(fbits)(op1.ztup0, op2.ztup0) && EQUAL(fbits)(op1.ztup1, op2.ztup1) && EQUAL(fbits)(op1.ztup2, op2.ztup2);
}

static struct zEncMetadata sailgen_capToMetadataBits(struct zCapability zcap)
{
  __label__ case_278, finish_match_277, end_function_279, end_block_exception_280;

  struct zEncMetadata zghz3138;
  uint64_t zE;
  {
    int64_t zgaz3214;
    {
      int64_t zgaz3213;
      zgaz3213 = zcap.zE;
      {
        RECREATE(sail_int)(&zghz3178);
        CONVERT_OF(sail_int, mach_int)(&zghz3178, zcap_max_E);
        RECREATE(sail_int)(&zghz3179);
        CONVERT_OF(sail_int, mach_int)(&zghz3179, zgaz3213);
        RECREATE(sail_int)(&zghz3180);
        sub_int(&zghz3180, zghz3178, zghz3179);
        zgaz3214 = CONVERT_OF(mach_int, sail_int)(zghz3180);
      }
    }
    {
      RECREATE(sail_int)(&zghz3175);
      CONVERT_OF(sail_int, mach_int)(&zghz3175, zcap_E_width);
      RECREATE(sail_int)(&zghz3176);
      CONVERT_OF(sail_int, mach_int)(&zghz3176, zgaz3214);
      RECREATE(lbits)(&zghz3177);
      sailgen_to_bits(&zghz3177, zghz3175, zghz3176);
      zE = CONVERT_OF(fbits, lbits)(zghz3177, true);
    }
  }
  struct ztuple_z8z5bitzCz0z5bv3zCz0z5bv3z9 zgaz3187;
  {
    bool zgaz3169;
    zgaz3169 = zcap.zinternal_exponent;
    if (zgaz3169) {
      fbits zgaz3174;
      {
        int64_t zgaz3170;
        {
          RECREATE(sail_int)(&zghz3172);
          CONVERT_OF(sail_int, mach_int)(&zghz3172, zcap_E_width);
          RECREATE(sail_int)(&zghz3173);
          CONVERT_OF(sail_int, mach_int)(&zghz3173, INT64_C(1));
          RECREATE(sail_int)(&zghz3174);
          sub_int(&zghz3174, zghz3172, zghz3173);
          zgaz3170 = CONVERT_OF(mach_int, sail_int)(zghz3174);
        }
        zgaz3174 = (UINT64_C(1) & (zE >> zgaz3170));
      }
      uint64_t zgaz3175;
      {
        int64_t zgaz3172;
        {
          int64_t zgaz3171;
          {
            RECREATE(sail_int)(&zghz3169);
            CONVERT_OF(sail_int, mach_int)(&zghz3169, zcap_xe_width);
            RECREATE(sail_int)(&zghz3170);
            CONVERT_OF(sail_int, mach_int)(&zghz3170, INT64_C(2));
            RECREATE(sail_int)(&zghz3171);
            mult_int(&zghz3171, zghz3169, zghz3170);
            zgaz3171 = CONVERT_OF(mach_int, sail_int)(zghz3171);
          }
          {
            RECREATE(sail_int)(&zghz3166);
            CONVERT_OF(sail_int, mach_int)(&zghz3166, zgaz3171);
            RECREATE(sail_int)(&zghz3167);
            CONVERT_OF(sail_int, mach_int)(&zghz3167, INT64_C(1));
            RECREATE(sail_int)(&zghz3168);
            sub_int(&zghz3168, zghz3166, zghz3167);
            zgaz3172 = CONVERT_OF(mach_int, sail_int)(zghz3168);
          }
        }
        zgaz3175 = (safe_rshift(UINT64_MAX, 64 - 3) & (zE >> zcap_xe_width));
      }
      uint64_t zgaz3176;
      {
        int64_t zgaz3173;
        {
          RECREATE(sail_int)(&zghz3163);
          CONVERT_OF(sail_int, mach_int)(&zghz3163, zcap_xe_width);
          RECREATE(sail_int)(&zghz3164);
          CONVERT_OF(sail_int, mach_int)(&zghz3164, INT64_C(1));
          RECREATE(sail_int)(&zghz3165);
          sub_int(&zghz3165, zghz3163, zghz3164);
          zgaz3173 = CONVERT_OF(mach_int, sail_int)(zghz3165);
        }
        zgaz3176 = (safe_rshift(UINT64_MAX, 64 - 3) & (zE >> INT64_C(0)));
      }
      struct ztuple_z8z5bitzCz0z5bv3zCz0z5bv3z9 zgsz3219;
      zgsz3219.ztup0 = zgaz3174;
      zgsz3219.ztup1 = zgaz3175;
      zgsz3219.ztup2 = zgaz3176;
      zgaz3187 = zgsz3219;
    } else {
      fbits zgaz3184;
      {
        uint64_t zgaz3179;
        {
          uint64_t zgaz3177;
          zgaz3177 = zcap.zT;
          uint64_t zgaz3178;
          zgaz3178 = zcap.zB;
          zgaz3179 = ((zgaz3177 - zgaz3178) & UINT64_C(0b11111111111111));
        }
        zgaz3184 = (UINT64_C(1) & (zgaz3179 >> INT64_C(8)));
      }
      uint64_t zgaz3185;
      {
        uint64_t zgaz3180;
        zgaz3180 = zcap.zT;
        int64_t zgaz3181;
        {
          RECREATE(sail_int)(&zghz3160);
          CONVERT_OF(sail_int, mach_int)(&zghz3160, zcap_xe_width);
          RECREATE(sail_int)(&zghz3161);
          CONVERT_OF(sail_int, mach_int)(&zghz3161, INT64_C(1));
          RECREATE(sail_int)(&zghz3162);
          sub_int(&zghz3162, zghz3160, zghz3161);
          zgaz3181 = CONVERT_OF(mach_int, sail_int)(zghz3162);
        }
        zgaz3185 = (safe_rshift(UINT64_MAX, 64 - 3) & (zgaz3180 >> INT64_C(0)));
      }
      uint64_t zgaz3186;
      {
        uint64_t zgaz3182;
        zgaz3182 = zcap.zB;
        int64_t zgaz3183;
        {
          RECREATE(sail_int)(&zghz3157);
          CONVERT_OF(sail_int, mach_int)(&zghz3157, zcap_xe_width);
          RECREATE(sail_int)(&zghz3158);
          CONVERT_OF(sail_int, mach_int)(&zghz3158, INT64_C(1));
          RECREATE(sail_int)(&zghz3159);
          sub_int(&zghz3159, zghz3157, zghz3158);
          zgaz3183 = CONVERT_OF(mach_int, sail_int)(zghz3159);
        }
        zgaz3186 = (safe_rshift(UINT64_MAX, 64 - 3) & (zgaz3182 >> INT64_C(0)));
      }
      struct ztuple_z8z5bitzCz0z5bv3zCz0z5bv3z9 zgsz3218;
      zgsz3218.ztup0 = zgaz3184;
      zgsz3218.ztup1 = zgaz3185;
      zgsz3218.ztup2 = zgaz3186;
      zgaz3187 = zgsz3218;
    }
  }
  {
    fbits zL8;
    zL8 = zgaz3187.ztup0;
    uint64_t zTE;
    zTE = zgaz3187.ztup1;
    uint64_t zBE;
    zBE = zgaz3187.ztup2;
    int64_t zMW;
    zMW = zcap_mantissa_width;
    struct zEncMetadata zghz3139;
    {
      struct zEncMetadata zghz3140;
      {
        struct zEncMetadata zghz3141;
        {
          struct zEncMetadata zghz3142;
          {
            struct zEncMetadata zghz3143;
            {
              struct zEncMetadata zghz3144;
              {
                struct zEncMetadata zghz3148;
                {
                  struct zEncMetadata zghz3152;
                  {
                    struct zEncMetadata zghz3153;
                    {
                      struct zEncMetadata zghz3154;
                      {
                        struct zEncMetadata zghz3155;
                        {
                          uint64_t zgaz3188;
                          zgaz3188 = zcap.zsd_perms;
                          struct zEncMetadata zghz3156;
                          zghz3156.zbits = UINT64_C(0x0000000000000000);
                          zghz3155 = sailgen__update_EncMetadata_SDP(zghz3156, zgaz3188);
                        }
                        uint64_t zgaz3190;
                        zgaz3190 = zcap.zap_m;
                        zghz3154 = sailgen__update_EncMetadata_AP_M(zghz3155, zgaz3190);
                      }
                      uint64_t zgaz3192;
                      zgaz3192 = zcap.zcl;
                      zghz3153 = sailgen__update_EncMetadata_CL(zghz3154, zgaz3192);
                    }
                    uint64_t zgaz3195;
                    {
                      bool zgaz3193;
                      zgaz3193 = zcap.zsealed;
                      zgaz3195 = sailgen_bool_bits_forwards(zgaz3193);
                    }
                    zghz3152 = sailgen__update_EncMetadata_CT(zghz3153, zgaz3195);
                  }
                  uint64_t zgaz3198;
                  {
                    bool zgaz3196;
                    zgaz3196 = zcap.zinternal_exponent;
                    zgaz3198 = sailgen_internal_exponent_encdec_forwards(zgaz3196);
                  }
                  zghz3148 = sailgen__update_EncMetadata_EF(zghz3152, zgaz3198);
                }
                uint64_t zgaz3202;
                {
                  uint64_t zgaz3199;
                  zgaz3199 = zcap.zT;
                  int64_t zgaz3200;
                  {
                    RECREATE(sail_int)(&zghz3149);
                    CONVERT_OF(sail_int, mach_int)(&zghz3149, zMW);
                    RECREATE(sail_int)(&zghz3150);
                    CONVERT_OF(sail_int, mach_int)(&zghz3150, INT64_C(3));
                    RECREATE(sail_int)(&zghz3151);
                    sub_int(&zghz3151, zghz3149, zghz3150);
                    zgaz3200 = CONVERT_OF(mach_int, sail_int)(zghz3151);
                  }
                  zgaz3202 = (safe_rshift(UINT64_MAX, 64 - 9) & (zgaz3199 >> zcap_xe_width));
                }
                zghz3144 = sailgen__update_EncMetadata_Tm(zghz3148, zgaz3202);
              }
              uint64_t zgaz3206;
              {
                uint64_t zgaz3203;
                zgaz3203 = zcap.zB;
                int64_t zgaz3204;
                {
                  RECREATE(sail_int)(&zghz3145);
                  CONVERT_OF(sail_int, mach_int)(&zghz3145, zMW);
                  RECREATE(sail_int)(&zghz3146);
                  CONVERT_OF(sail_int, mach_int)(&zghz3146, INT64_C(1));
                  RECREATE(sail_int)(&zghz3147);
                  sub_int(&zghz3147, zghz3145, zghz3146);
                  zgaz3204 = CONVERT_OF(mach_int, sail_int)(zghz3147);
                }
                zgaz3206 = (safe_rshift(UINT64_MAX, 64 - 11) & (zgaz3203 >> zcap_xe_width));
              }
              zghz3143 = sailgen__update_EncMetadata_Bm(zghz3144, zgaz3206);
            }
            zghz3142 = sailgen__update_EncMetadata_TE(zghz3143, zTE);
          }
          zghz3141 = sailgen__update_EncMetadata_BE(zghz3142, zBE);
        }
        uint64_t zgaz3210;
        zgaz3210 = zcap.zreserved_1;
        zghz3140 = sailgen__update_EncMetadata_reserved_1(zghz3141, zgaz3210);
      }
      uint64_t zgaz3212;
      zgaz3212 = zcap.zreserved_0;
      zghz3139 = sailgen__update_EncMetadata_reserved_0(zghz3140, zgaz3212);
    }
    uint64_t zgsz3222;
    zgsz3222 = UINT64_C(0b0);
    zgsz3222 = update_fbits(zgsz3222, INT64_C(0), zL8);
    zghz3138 = sailgen_setMetadataL8(zghz3139, zgsz3222);
    goto finish_match_277;
  }
case_278: ;
  sail_match_failure("capToMetadataBits");
finish_match_277: ;
end_function_279: ;
  return zghz3138;
end_block_exception_280: ;
  struct zEncMetadata zcbz3106 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz3106;
}



static void finish_sailgen_capToMetadataBits(void)
{
  KILL(sail_int)(&zghz3180);
  KILL(sail_int)(&zghz3179);
  KILL(sail_int)(&zghz3178);
  KILL(lbits)(&zghz3177);
  KILL(sail_int)(&zghz3176);
  KILL(sail_int)(&zghz3175);
  KILL(sail_int)(&zghz3174);
  KILL(sail_int)(&zghz3173);
  KILL(sail_int)(&zghz3172);
  KILL(sail_int)(&zghz3171);
  KILL(sail_int)(&zghz3170);
  KILL(sail_int)(&zghz3169);
  KILL(sail_int)(&zghz3168);
  KILL(sail_int)(&zghz3167);
  KILL(sail_int)(&zghz3166);
  KILL(sail_int)(&zghz3165);
  KILL(sail_int)(&zghz3164);
  KILL(sail_int)(&zghz3163);
  KILL(sail_int)(&zghz3162);
  KILL(sail_int)(&zghz3161);
  KILL(sail_int)(&zghz3160);
  KILL(sail_int)(&zghz3159);
  KILL(sail_int)(&zghz3158);
  KILL(sail_int)(&zghz3157);
  KILL(sail_int)(&zghz3151);
  KILL(sail_int)(&zghz3150);
  KILL(sail_int)(&zghz3149);
  KILL(sail_int)(&zghz3147);
  KILL(sail_int)(&zghz3146);
  KILL(sail_int)(&zghz3145);
}

static struct zCapability znull_cap;
static void create_letbind_24(void) {


  struct zCapability zgsz3225;
  uint64_t zgaz3217;
  {
    uint64_t zgaz3216;
    {
      int64_t zgaz3215;
      {
        sail_int zgsz3478;
        CREATE(sail_int)(&zgsz3478);
        CONVERT_OF(sail_int, mach_int)(&zgsz3478, zcap_mantissa_width);
        sail_int zgsz3479;
        CREATE(sail_int)(&zgsz3479);
        CONVERT_OF(sail_int, mach_int)(&zgsz3479, INT64_C(2));
        sail_int zgsz3480;
        CREATE(sail_int)(&zgsz3480);
        sub_int(&zgsz3480, zgsz3478, zgsz3479);
        zgaz3215 = CONVERT_OF(mach_int, sail_int)(zgsz3480);
        KILL(sail_int)(&zgsz3480);
        KILL(sail_int)(&zgsz3479);
        KILL(sail_int)(&zgsz3478);
      }
      {
        sail_int zgsz3476;
        CREATE(sail_int)(&zgsz3476);
        CONVERT_OF(sail_int, mach_int)(&zgsz3476, zgaz3215);
        lbits zgsz3477;
        CREATE(lbits)(&zgsz3477);
        sailgen_zzeros_implicit(&zgsz3477, zgsz3476);
        zgaz3216 = CONVERT_OF(fbits, lbits)(zgsz3477, true);
        KILL(lbits)(&zgsz3477);
        KILL(sail_int)(&zgsz3476);
      }
    }
    zgaz3217 = (UINT64_C(0b01) << 12) | zgaz3216;
  }
  struct zCapability zgsz3224;
  zgsz3224.zB = UINT64_C(0b00000000000000);
  zgsz3224.zE = zcap_max_E;
  zgsz3224.zT = zgaz3217;
  zgsz3224.zaddress = UINT64_C(0x0000000000000000);
  zgsz3224.zap_m = znull_ap_m;
  zgsz3224.zcl = UINT64_C(0b0);
  zgsz3224.zinternal_exponent = true;
  zgsz3224.zreserved_0 = UINT64_C(0b000000000000000);
  zgsz3224.zreserved_1 = UINT64_C(0b0000000);
  zgsz3224.zsd_perms = UINT64_C(0x0);
  zgsz3224.zsealed = false;
  zgsz3224.ztag = false;
  zgsz3225 = zgsz3224;
  znull_cap = zgsz3225;
let_end_281: ;
}
static void kill_letbind_24(void) {
}

static struct zCapability zinfinite_cap;
static void create_letbind_25(void) {


  struct zCapability zgsz3227;
  struct zCapability zgsz3226;
  zgsz3226 = znull_cap;
  zgsz3226.zap_m = zinfinite_ap_m;
  zgsz3226.zcl = UINT64_C(0b1);
  zgsz3226.zsd_perms = UINT64_C(0xF);
  zgsz3226.ztag = true;
  zgsz3227 = zgsz3226;
  zinfinite_cap = zgsz3227;
let_end_282: ;
}
static void kill_letbind_25(void) {
}

static bool sailgen_boundsMalformed(struct zCapability);

sail_int zghz3181;
sail_int zghz3182;
sail_int zghz3183;
sail_int zghz3184;
sail_int zghz3185;
sail_int zghz3186;
sail_int zghz3187;
lbits zghz3188;

static void startup_sailgen_boundsMalformed(void)
{
  CREATE(sail_int)(&zghz3181);
  CREATE(sail_int)(&zghz3182);
  CREATE(sail_int)(&zghz3183);
  CREATE(sail_int)(&zghz3184);
  CREATE(sail_int)(&zghz3185);
  CREATE(sail_int)(&zghz3186);
  CREATE(sail_int)(&zghz3187);
  CREATE(lbits)(&zghz3188);
}

static bool sailgen_boundsMalformed(struct zCapability zcap)
{
  __label__ end_function_284, end_block_exception_285;

  bool zcbz362;
  bool zgaz3232;
  zgaz3232 = zcap.zinternal_exponent;
  bool zgsz3234;
  if (zgaz3232) {
    int64_t zMW;
    zMW = zcap_mantissa_width;
    bool zmalformedMSB;
    {
      bool zgaz3231;
      {
        bool zgaz3224;
        {
          int64_t zgaz3221;
          zgaz3221 = zcap.zE;
          zgaz3224 = (zgaz3221 == zcap_max_E);
        }
        bool zgsz3228;
        if (zgaz3224) {
          uint64_t zgaz3222;
          zgaz3222 = zcap.zB;
          uint64_t zgaz3223;
          {
            RECREATE(sail_int)(&zghz3187);
            CONVERT_OF(sail_int, mach_int)(&zghz3187, zMW);
            RECREATE(lbits)(&zghz3188);
            sailgen_zzeros_implicit(&zghz3188, zghz3187);
            zgaz3223 = CONVERT_OF(fbits, lbits)(zghz3188, true);
          }
          zgsz3228 = (zgaz3222 != zgaz3223);
        } else {  zgsz3228 = false;  }
        zgaz3231 = zgsz3228;
      }
      bool zgsz3230;
      if (zgaz3231) {  zgsz3230 = true;  } else {
        bool zgaz3230;
        {
          int64_t zgaz3225;
          zgaz3225 = zcap.zE;
          int64_t zgaz3226;
          {
            RECREATE(sail_int)(&zghz3184);
            CONVERT_OF(sail_int, mach_int)(&zghz3184, zcap_max_E);
            RECREATE(sail_int)(&zghz3185);
            CONVERT_OF(sail_int, mach_int)(&zghz3185, INT64_C(1));
            RECREATE(sail_int)(&zghz3186);
            sub_int(&zghz3186, zghz3184, zghz3185);
            zgaz3226 = CONVERT_OF(mach_int, sail_int)(zghz3186);
          }
          zgaz3230 = (zgaz3225 == zgaz3226);
        }
        bool zgsz3229;
        if (zgaz3230) {
          fbits zgaz3229;
          {
            uint64_t zgaz3227;
            zgaz3227 = zcap.zB;
            int64_t zgaz3228;
            {
              RECREATE(sail_int)(&zghz3181);
              CONVERT_OF(sail_int, mach_int)(&zghz3181, zMW);
              RECREATE(sail_int)(&zghz3182);
              CONVERT_OF(sail_int, mach_int)(&zghz3182, INT64_C(1));
              RECREATE(sail_int)(&zghz3183);
              sub_int(&zghz3183, zghz3181, zghz3182);
              zgaz3228 = CONVERT_OF(mach_int, sail_int)(zghz3183);
            }
            zgaz3229 = (UINT64_C(1) & (zgaz3227 >> zgaz3228));
          }
          zgsz3229 = sailgen_neq_anythingzItzK(zgaz3229, UINT64_C(0));
        } else {  zgsz3229 = false;  }
        zgsz3230 = zgsz3229;
      }
      zmalformedMSB = zgsz3230;
    }
    bool zmalformedLSB;
    {
      bool zgaz3220;
      {
        int64_t zgaz3218;
        zgaz3218 = zcap.zE;
        zgaz3220 = (zgaz3218 < INT64_C(0));
      }
      bool zgsz3232;
      if (zgaz3220) {  zgsz3232 = true;  } else {
        bool zgsz3231;
        if (zcap_use_L8) {
          int64_t zgaz3219;
          zgaz3219 = zcap.zE;
          zgsz3231 = (zgaz3219 == INT64_C(0));
        } else {  zgsz3231 = false;  }
        zgsz3232 = zgsz3231;
      }
      zmalformedLSB = zgsz3232;
    }
    bool zgsz3233;
    if (zmalformedMSB) {  zgsz3233 = true;  } else {  zgsz3233 = zmalformedLSB;  }
    zgsz3234 = zgsz3233;
  } else {  zgsz3234 = false;  }
  zcbz362 = zgsz3234;
end_function_284: ;
  return zcbz362;
end_block_exception_285: ;

  return false;
}



static void finish_sailgen_boundsMalformed(void)
{
  KILL(lbits)(&zghz3188);
  KILL(sail_int)(&zghz3187);
  KILL(sail_int)(&zghz3186);
  KILL(sail_int)(&zghz3185);
  KILL(sail_int)(&zghz3184);
  KILL(sail_int)(&zghz3183);
  KILL(sail_int)(&zghz3182);
  KILL(sail_int)(&zghz3181);
}

static void sailgen_getCapBoundsBits(struct zoptionzIz8bzCbz9zK *rop, struct zCapability);

lbits zghz3189;
lbits zghz3190;
sail_int zghz3191;
sail_int zghz3192;
lbits zghz3193;
sail_int zghz3194;
sail_int zghz3195;
sail_int zghz3196;
sail_int zghz3197;
sail_int zghz3198;
lbits zghz3199;
lbits zghz3200;
sail_int zghz3201;
sail_int zghz3202;
sail_int zghz3203;
sail_int zghz3204;
sail_int zghz3205;
lbits zghz3206;
sail_int zghz3207;
sail_int zghz3208;
sail_int zghz3209;
sail_int zghz3210;
sail_int zghz3211;
sail_int zghz3212;
sail_int zghz3213;
lbits zghz3214;
sail_int zghz3215;
lbits zghz3216;
lbits zghz3217;
lbits zghz3218;
lbits zghz3219;
sail_int zghz3220;
lbits zghz3221;
lbits zghz3222;
sail_int zghz3223;
lbits zghz3224;
lbits zghz3225;
sail_int zghz3226;
lbits zghz3227;
lbits zghz3228;
lbits zghz3229;
lbits zghz3230;
sail_int zghz3231;
lbits zghz3232;
lbits zghz3233;
sail_int zghz3234;
lbits zghz3235;
lbits zghz3236;
sail_int zghz3237;
lbits zghz3238;
sail_int zghz3239;
sail_int zghz3240;
sail_int zghz3241;
sail_int zghz3242;
sail_int zghz3243;
sail_int zghz3244;
lbits zghz3245;
lbits zghz3246;
lbits zghz3247;
lbits zghz3248;
lbits zghz3249;
lbits zghz3250;
sail_int zghz3251;
lbits zghz3252;
sail_int zghz3253;
sail_int zghz3254;
sail_int zghz3255;
lbits zghz3256;
sail_int zghz3257;
lbits zghz3258;
lbits zghz3259;
sail_int zghz3260;
lbits zghz3261;

static void startup_sailgen_getCapBoundsBits(void)
{
  CREATE(lbits)(&zghz3189);
  CREATE(lbits)(&zghz3190);
  CREATE(sail_int)(&zghz3191);
  CREATE(sail_int)(&zghz3192);
  CREATE(lbits)(&zghz3193);
  CREATE(sail_int)(&zghz3194);
  CREATE(sail_int)(&zghz3195);
  CREATE(sail_int)(&zghz3196);
  CREATE(sail_int)(&zghz3197);
  CREATE(sail_int)(&zghz3198);
  CREATE(lbits)(&zghz3199);
  CREATE(lbits)(&zghz3200);
  CREATE(sail_int)(&zghz3201);
  CREATE(sail_int)(&zghz3202);
  CREATE(sail_int)(&zghz3203);
  CREATE(sail_int)(&zghz3204);
  CREATE(sail_int)(&zghz3205);
  CREATE(lbits)(&zghz3206);
  CREATE(sail_int)(&zghz3207);
  CREATE(sail_int)(&zghz3208);
  CREATE(sail_int)(&zghz3209);
  CREATE(sail_int)(&zghz3210);
  CREATE(sail_int)(&zghz3211);
  CREATE(sail_int)(&zghz3212);
  CREATE(sail_int)(&zghz3213);
  CREATE(lbits)(&zghz3214);
  CREATE(sail_int)(&zghz3215);
  CREATE(lbits)(&zghz3216);
  CREATE(lbits)(&zghz3217);
  CREATE(lbits)(&zghz3218);
  CREATE(lbits)(&zghz3219);
  CREATE(sail_int)(&zghz3220);
  CREATE(lbits)(&zghz3221);
  CREATE(lbits)(&zghz3222);
  CREATE(sail_int)(&zghz3223);
  CREATE(lbits)(&zghz3224);
  CREATE(lbits)(&zghz3225);
  CREATE(sail_int)(&zghz3226);
  CREATE(lbits)(&zghz3227);
  CREATE(lbits)(&zghz3228);
  CREATE(lbits)(&zghz3229);
  CREATE(lbits)(&zghz3230);
  CREATE(sail_int)(&zghz3231);
  CREATE(lbits)(&zghz3232);
  CREATE(lbits)(&zghz3233);
  CREATE(sail_int)(&zghz3234);
  CREATE(lbits)(&zghz3235);
  CREATE(lbits)(&zghz3236);
  CREATE(sail_int)(&zghz3237);
  CREATE(lbits)(&zghz3238);
  CREATE(sail_int)(&zghz3239);
  CREATE(sail_int)(&zghz3240);
  CREATE(sail_int)(&zghz3241);
  CREATE(sail_int)(&zghz3242);
  CREATE(sail_int)(&zghz3243);
  CREATE(sail_int)(&zghz3244);
  CREATE(lbits)(&zghz3245);
  CREATE(lbits)(&zghz3246);
  CREATE(lbits)(&zghz3247);
  CREATE(lbits)(&zghz3248);
  CREATE(lbits)(&zghz3249);
  CREATE(lbits)(&zghz3250);
  CREATE(sail_int)(&zghz3251);
  CREATE(lbits)(&zghz3252);
  CREATE(sail_int)(&zghz3253);
  CREATE(sail_int)(&zghz3254);
  CREATE(sail_int)(&zghz3255);
  CREATE(lbits)(&zghz3256);
  CREATE(sail_int)(&zghz3257);
  CREATE(lbits)(&zghz3258);
  CREATE(lbits)(&zghz3259);
  CREATE(sail_int)(&zghz3260);
  CREATE(lbits)(&zghz3261);
}

// struct tuple_(%bv64, %bv)
struct ztuple_z8z5bv64zCz0z5bvz9 {
  uint64_t ztup0;
  lbits ztup1;
};

static void COPY(ztuple_z8z5bv64zCz0z5bvz9)(struct ztuple_z8z5bv64zCz0z5bvz9 *rop, const struct ztuple_z8z5bv64zCz0z5bvz9 op) {
  rop->ztup0 = op.ztup0;
  COPY(lbits)(&rop->ztup1, op.ztup1);
}

static void CREATE(ztuple_z8z5bv64zCz0z5bvz9)(struct ztuple_z8z5bv64zCz0z5bvz9 *op) {
  CREATE(lbits)(&op->ztup1);
}

static void RECREATE(ztuple_z8z5bv64zCz0z5bvz9)(struct ztuple_z8z5bv64zCz0z5bvz9 *op) {
  RECREATE(lbits)(&op->ztup1);
}

static void KILL(ztuple_z8z5bv64zCz0z5bvz9)(struct ztuple_z8z5bv64zCz0z5bvz9 *op) {
  KILL(lbits)(&op->ztup1);
}

static bool EQUAL(ztuple_z8z5bv64zCz0z5bvz9)(struct ztuple_z8z5bv64zCz0z5bvz9 op1, struct ztuple_z8z5bv64zCz0z5bvz9 op2) {
  return EQUAL(fbits)(op1.ztup0, op2.ztup0) && EQUAL(lbits)(op1.ztup1, op2.ztup1);
}

static void sailgen_getCapBoundsBits(struct zoptionzIz8bzCbz9zK *zcbz363, struct zCapability zc)
{
  __label__ end_function_287, end_block_exception_290, end_function_408;

  {
    bool zgaz3233;
    zgaz3233 = sailgen_boundsMalformed(zc);
    unit zgsz3235;
    if (zgaz3233) {
      __label__ cleanup_288, end_cleanup_289;

      struct zoptionzIz8bzCbz9zK zgaz3234;
      CREATE(zoptionzIz8bzCbz9zK)(&zgaz3234);
      sailgen_NonezIz8bzCbz9zK(&zgaz3234, UNIT);
      COPY(zoptionzIz8bzCbz9zK)((*(&zcbz363)), zgaz3234);
      goto cleanup_288;
      /* unreachable after return */
      KILL(zoptionzIz8bzCbz9zK)(&zgaz3234);
      goto end_cleanup_289;
    cleanup_288: ;
      KILL(zoptionzIz8bzCbz9zK)(&zgaz3234);
      goto end_function_287;
    end_cleanup_289: ;
    } else {  zgsz3235 = UNIT;  }
  }
  int64_t zE;
  zE = zc.zE;
  {
    bool zgaz3235;
    zgaz3235 = (zE >= INT64_C(0));
    unit zgsz3236;
    zgsz3236 = sail_assert(zgaz3235, "Cap is malformed if E<0");
  }
  uint64_t za;
  za = zc.zaddress;
  uint64_t za_mid;
  {
    uint64_t zgaz3267;
    {
      RECREATE(lbits)(&zghz3259);
      CONVERT_OF(lbits, fbits)(&zghz3259, za, UINT64_C(64) , true);
      RECREATE(sail_int)(&zghz3260);
      CONVERT_OF(sail_int, mach_int)(&zghz3260, zE);
      RECREATE(lbits)(&zghz3261);
      shiftr(&zghz3261, zghz3259, zghz3260);
      zgaz3267 = CONVERT_OF(fbits, lbits)(zghz3261, true);
    }
    {
      RECREATE(lbits)(&zghz3256);
      CONVERT_OF(lbits, fbits)(&zghz3256, zgaz3267, UINT64_C(64) , true);
      RECREATE(sail_int)(&zghz3257);
      CONVERT_OF(sail_int, mach_int)(&zghz3257, zcap_mantissa_width);
      RECREATE(lbits)(&zghz3258);
      sail_truncate(&zghz3258, zghz3256, zghz3257);
      za_mid = CONVERT_OF(fbits, lbits)(zghz3258, true);
    }
  }
  uint64_t zR;
  {
    uint64_t zgaz3265;
    zgaz3265 = zc.zB;
    uint64_t zgaz3266;
    {
      uint64_t zgaz3264;
      {
        int64_t zgaz3263;
        {
          RECREATE(sail_int)(&zghz3253);
          CONVERT_OF(sail_int, mach_int)(&zghz3253, zcap_mantissa_width);
          RECREATE(sail_int)(&zghz3254);
          CONVERT_OF(sail_int, mach_int)(&zghz3254, INT64_C(2));
          RECREATE(sail_int)(&zghz3255);
          sub_int(&zghz3255, zghz3253, zghz3254);
          zgaz3263 = CONVERT_OF(mach_int, sail_int)(zghz3255);
        }
        {
          RECREATE(sail_int)(&zghz3251);
          CONVERT_OF(sail_int, mach_int)(&zghz3251, zgaz3263);
          RECREATE(lbits)(&zghz3252);
          sailgen_zzeros_implicit(&zghz3252, zghz3251);
          zgaz3264 = CONVERT_OF(fbits, lbits)(zghz3252, true);
        }
      }
      zgaz3266 = (UINT64_C(0b01) << 12) | zgaz3264;
    }
    zR = ((zgaz3265 - zgaz3266) & UINT64_C(0b11111111111111));
  }
  int64_t zaHi;
  {
    bool zgaz3262;
    {
      RECREATE(lbits)(&zghz3249);
      CONVERT_OF(lbits, fbits)(&zghz3249, za_mid, UINT64_C(14) , true);
      RECREATE(lbits)(&zghz3250);
      CONVERT_OF(lbits, fbits)(&zghz3250, zR, UINT64_C(14) , true);
      zgaz3262 = sailgen_z8operatorz0zI_uz9(zghz3249, zghz3250);
    }
    if (zgaz3262) {  zaHi = INT64_C(1);  } else {  zaHi = INT64_C(0);  }
  }
  int64_t zbHi;
  {
    bool zgaz3261;
    {
      uint64_t zgaz3260;
      zgaz3260 = zc.zB;
      {
        RECREATE(lbits)(&zghz3247);
        CONVERT_OF(lbits, fbits)(&zghz3247, zgaz3260, UINT64_C(14) , true);
        RECREATE(lbits)(&zghz3248);
        CONVERT_OF(lbits, fbits)(&zghz3248, zR, UINT64_C(14) , true);
        zgaz3261 = sailgen_z8operatorz0zI_uz9(zghz3247, zghz3248);
      }
    }
    if (zgaz3261) {  zbHi = INT64_C(1);  } else {  zbHi = INT64_C(0);  }
  }
  int64_t ztHi;
  {
    bool zgaz3259;
    {
      uint64_t zgaz3258;
      zgaz3258 = zc.zT;
      {
        RECREATE(lbits)(&zghz3245);
        CONVERT_OF(lbits, fbits)(&zghz3245, zgaz3258, UINT64_C(14) , true);
        RECREATE(lbits)(&zghz3246);
        CONVERT_OF(lbits, fbits)(&zghz3246, zR, UINT64_C(14) , true);
        zgaz3259 = sailgen_z8operatorz0zI_uz9(zghz3245, zghz3246);
      }
    }
    if (zgaz3259) {  ztHi = INT64_C(1);  } else {  ztHi = INT64_C(0);  }
  }
  int64_t zcorrection_base;
  {
    RECREATE(sail_int)(&zghz3242);
    CONVERT_OF(sail_int, mach_int)(&zghz3242, zbHi);
    RECREATE(sail_int)(&zghz3243);
    CONVERT_OF(sail_int, mach_int)(&zghz3243, zaHi);
    RECREATE(sail_int)(&zghz3244);
    sub_int(&zghz3244, zghz3242, zghz3243);
    zcorrection_base = CONVERT_OF(mach_int, sail_int)(zghz3244);
  }
  int64_t zcorrection_top;
  {
    RECREATE(sail_int)(&zghz3239);
    CONVERT_OF(sail_int, mach_int)(&zghz3239, ztHi);
    RECREATE(sail_int)(&zghz3240);
    CONVERT_OF(sail_int, mach_int)(&zghz3240, zaHi);
    RECREATE(sail_int)(&zghz3241);
    sub_int(&zghz3241, zghz3239, zghz3240);
    zcorrection_top = CONVERT_OF(mach_int, sail_int)(zghz3241);
  }
  uint64_t za_top;
  {
    int64_t zgaz3257;
    zgaz3257 = (zE + zcap_mantissa_width);
    {
      RECREATE(lbits)(&zghz3236);
      CONVERT_OF(lbits, fbits)(&zghz3236, za, UINT64_C(64) , true);
      RECREATE(sail_int)(&zghz3237);
      CONVERT_OF(sail_int, mach_int)(&zghz3237, zgaz3257);
      RECREATE(lbits)(&zghz3238);
      shiftr(&zghz3238, zghz3236, zghz3237);
      za_top = CONVERT_OF(fbits, lbits)(zghz3238, true);
    }
  }
  RECREATE(lbits)(&zghz3189);
  {
    RECREATE(lbits)(&zghz3225);
    {
      uint64_t zgaz3254;
      {
        RECREATE(lbits)(&zghz3233);
        CONVERT_OF(lbits, fbits)(&zghz3233, za_top, UINT64_C(64) , true);
        RECREATE(sail_int)(&zghz3234);
        CONVERT_OF(sail_int, mach_int)(&zghz3234, zcorrection_base);
        RECREATE(lbits)(&zghz3235);
        add_bits_int(&zghz3235, zghz3233, zghz3234);
        zgaz3254 = CONVERT_OF(fbits, lbits)(zghz3235, true);
      }
      RECREATE(lbits)(&zghz3227);
      {
        uint64_t zgaz3252;
        zgaz3252 = zc.zB;
        sbits zgaz3253;
        {
          RECREATE(sail_int)(&zghz3231);
          CONVERT_OF(sail_int, mach_int)(&zghz3231, zE);
          RECREATE(lbits)(&zghz3232);
          sailgen_zzeros_implicit(&zghz3232, zghz3231);
          zgaz3253 = CONVERT_OF(sbits, lbits)(zghz3232, true);
        }
        {
          RECREATE(lbits)(&zghz3229);
          CONVERT_OF(lbits, fbits)(&zghz3229, zgaz3252, UINT64_C(14) , true);
          RECREATE(lbits)(&zghz3230);
          CONVERT_OF(lbits, sbits)(&zghz3230, zgaz3253, true);
          append(&zghz3227, zghz3229, zghz3230);
        }
      }
      {
        RECREATE(lbits)(&zghz3228);
        CONVERT_OF(lbits, fbits)(&zghz3228, zgaz3254, UINT64_C(64) , true);
        append(&zghz3225, zghz3228, zghz3227);
      }
    }
    {
      RECREATE(sail_int)(&zghz3226);
      CONVERT_OF(sail_int, mach_int)(&zghz3226, zcap_len_width);
      sail_truncate(&zghz3189, zghz3225, zghz3226);
    }
  }
  RECREATE(lbits)(&zghz3190);
  {
    RECREATE(lbits)(&zghz3214);
    {
      uint64_t zgaz3249;
      {
        RECREATE(lbits)(&zghz3222);
        CONVERT_OF(lbits, fbits)(&zghz3222, za_top, UINT64_C(64) , true);
        RECREATE(sail_int)(&zghz3223);
        CONVERT_OF(sail_int, mach_int)(&zghz3223, zcorrection_top);
        RECREATE(lbits)(&zghz3224);
        add_bits_int(&zghz3224, zghz3222, zghz3223);
        zgaz3249 = CONVERT_OF(fbits, lbits)(zghz3224, true);
      }
      RECREATE(lbits)(&zghz3216);
      {
        uint64_t zgaz3247;
        zgaz3247 = zc.zT;
        sbits zgaz3248;
        {
          RECREATE(sail_int)(&zghz3220);
          CONVERT_OF(sail_int, mach_int)(&zghz3220, zE);
          RECREATE(lbits)(&zghz3221);
          sailgen_zzeros_implicit(&zghz3221, zghz3220);
          zgaz3248 = CONVERT_OF(sbits, lbits)(zghz3221, true);
        }
        {
          RECREATE(lbits)(&zghz3218);
          CONVERT_OF(lbits, fbits)(&zghz3218, zgaz3247, UINT64_C(14) , true);
          RECREATE(lbits)(&zghz3219);
          CONVERT_OF(lbits, sbits)(&zghz3219, zgaz3248, true);
          append(&zghz3216, zghz3218, zghz3219);
        }
      }
      {
        RECREATE(lbits)(&zghz3217);
        CONVERT_OF(lbits, fbits)(&zghz3217, zgaz3249, UINT64_C(64) , true);
        append(&zghz3214, zghz3217, zghz3216);
      }
    }
    {
      RECREATE(sail_int)(&zghz3215);
      CONVERT_OF(sail_int, mach_int)(&zghz3215, zcap_len_width);
      sail_truncate(&zghz3190, zghz3214, zghz3215);
    }
  }
  uint64_t zbase2;
  {
    uint64_t zgaz3246;
    {
      fbits zgaz3245;
      {
        int64_t zgaz3244;
        {
          RECREATE(sail_int)(&zghz3211);
          CONVERT_OF(sail_int, mach_int)(&zghz3211, zcap_addr_width);
          RECREATE(sail_int)(&zghz3212);
          CONVERT_OF(sail_int, mach_int)(&zghz3212, INT64_C(1));
          RECREATE(sail_int)(&zghz3213);
          sub_int(&zghz3213, zghz3211, zghz3212);
          zgaz3244 = CONVERT_OF(mach_int, sail_int)(zghz3213);
        }
        {
          RECREATE(sail_int)(&zghz3210);
          CONVERT_OF(sail_int, mach_int)(&zghz3210, zgaz3244);
          zgaz3245 = bitvector_access(zghz3189, zghz3210);
        }
      }
      zgaz3246 = UINT64_C(0b0);
      zgaz3246 = update_fbits(zgaz3246, INT64_C(0), zgaz3245);
    }
    zbase2 = (UINT64_C(0b0) << 1) | zgaz3246;
  }
  uint64_t ztop2;
  {
    int64_t zgaz3243;
    {
      RECREATE(sail_int)(&zghz3207);
      CONVERT_OF(sail_int, mach_int)(&zghz3207, zcap_addr_width);
      RECREATE(sail_int)(&zghz3208);
      CONVERT_OF(sail_int, mach_int)(&zghz3208, INT64_C(1));
      RECREATE(sail_int)(&zghz3209);
      sub_int(&zghz3209, zghz3207, zghz3208);
      zgaz3243 = CONVERT_OF(mach_int, sail_int)(zghz3209);
    }
    {
      RECREATE(sail_int)(&zghz3204);
      CONVERT_OF(sail_int, mach_int)(&zghz3204, zcap_addr_width);
      RECREATE(sail_int)(&zghz3205);
      CONVERT_OF(sail_int, mach_int)(&zghz3205, zgaz3243);
      RECREATE(lbits)(&zghz3206);
      vector_subrange_lbits(&zghz3206, zghz3190, zghz3204, zghz3205);
      ztop2 = CONVERT_OF(fbits, lbits)(zghz3206, true);
    }
  }
  {
    bool zgaz3239;
    {
      bool zgaz3238;
      {
        int64_t zgaz3236;
        {
          RECREATE(sail_int)(&zghz3201);
          CONVERT_OF(sail_int, mach_int)(&zghz3201, zcap_max_E);
          RECREATE(sail_int)(&zghz3202);
          CONVERT_OF(sail_int, mach_int)(&zghz3202, INT64_C(1));
          RECREATE(sail_int)(&zghz3203);
          sub_int(&zghz3203, zghz3201, zghz3202);
          zgaz3236 = CONVERT_OF(mach_int, sail_int)(zghz3203);
        }
        zgaz3238 = (zE < zgaz3236);
      }
      bool zgsz3238;
      if (zgaz3238) {
        uint64_t zgaz3237;
        zgaz3237 = ((ztop2 - zbase2) & UINT64_C(0b11));
        {
          RECREATE(lbits)(&zghz3199);
          CONVERT_OF(lbits, fbits)(&zghz3199, zgaz3237, UINT64_C(2) , true);
          RECREATE(lbits)(&zghz3200);
          CONVERT_OF(lbits, fbits)(&zghz3200, UINT64_C(0b01), UINT64_C(2) , true);
          zgsz3238 = sailgen_z8operatorz0zK_uz9(zghz3199, zghz3200);
        }
      } else {  zgsz3238 = false;  }
      zgaz3239 = zgsz3238;
    }
    unit zgsz3239;
    if (zgaz3239) {
      fbits zgaz3241;
      {
        fbits zgaz3240;
        {
          RECREATE(sail_int)(&zghz3198);
          CONVERT_OF(sail_int, mach_int)(&zghz3198, zcap_addr_width);
          zgaz3240 = bitvector_access(zghz3190, zghz3198);
        }
        zgaz3241 = sailgen_not_bit(zgaz3240);
      }
      {
        RECREATE(sail_int)(&zghz3197);
        CONVERT_OF(sail_int, mach_int)(&zghz3197, zcap_addr_width);
        update_lbits(&zghz3190, zghz3190, zghz3197, zgaz3241);
      }
      zgsz3239 = UNIT;
    } else {  zgsz3239 = UNIT;  }
  }
  uint64_t zbaseshadowz329;
  {
    int64_t zgaz3242;
    {
      RECREATE(sail_int)(&zghz3194);
      CONVERT_OF(sail_int, mach_int)(&zghz3194, zcap_addr_width);
      RECREATE(sail_int)(&zghz3195);
      CONVERT_OF(sail_int, mach_int)(&zghz3195, INT64_C(1));
      RECREATE(sail_int)(&zghz3196);
      sub_int(&zghz3196, zghz3194, zghz3195);
      zgaz3242 = CONVERT_OF(mach_int, sail_int)(zghz3196);
    }
    {
      RECREATE(sail_int)(&zghz3191);
      CONVERT_OF(sail_int, mach_int)(&zghz3191, zgaz3242);
      RECREATE(sail_int)(&zghz3192);
      CONVERT_OF(sail_int, mach_int)(&zghz3192, INT64_C(0));
      RECREATE(lbits)(&zghz3193);
      vector_subrange_lbits(&zghz3193, zghz3189, zghz3191, zghz3192);
      zbaseshadowz329 = CONVERT_OF(fbits, lbits)(zghz3193, true);
    }
  }
  struct ztuple_z8z5bv64zCz0z5bvz9 zgsz3240;
  CREATE(ztuple_z8z5bv64zCz0z5bvz9)(&zgsz3240);
  zgsz3240.ztup0 = zbaseshadowz329;
  COPY(lbits)(&((&zgsz3240)->ztup1), zghz3190);
  {
    struct ztuple_z8z5bvzCz0z5bvz9 zgsz3550;
    CREATE(ztuple_z8z5bvzCz0z5bvz9)(&zgsz3550);
    /* conversions */
    CONVERT_OF(lbits, fbits)(&((&zgsz3550)->ztup0), zgsz3240.ztup0, UINT64_C(64) , true);
    COPY(lbits)(&((&zgsz3550)->ztup1), zgsz3240.ztup1);
    /* end conversions */
    sailgen_SomezIz8bzCbz9zK((*(&zcbz363)), zgsz3550);
    KILL(ztuple_z8z5bvzCz0z5bvz9)(&zgsz3550);
  }
  KILL(ztuple_z8z5bv64zCz0z5bvz9)(&zgsz3240);
end_function_287: ;
  goto end_function_408;
end_block_exception_290: ;
  goto end_function_408;
end_function_408: ;
}



static void finish_sailgen_getCapBoundsBits(void)
{
  KILL(lbits)(&zghz3261);
  KILL(sail_int)(&zghz3260);
  KILL(lbits)(&zghz3259);
  KILL(lbits)(&zghz3258);
  KILL(sail_int)(&zghz3257);
  KILL(lbits)(&zghz3256);
  KILL(sail_int)(&zghz3255);
  KILL(sail_int)(&zghz3254);
  KILL(sail_int)(&zghz3253);
  KILL(lbits)(&zghz3252);
  KILL(sail_int)(&zghz3251);
  KILL(lbits)(&zghz3250);
  KILL(lbits)(&zghz3249);
  KILL(lbits)(&zghz3248);
  KILL(lbits)(&zghz3247);
  KILL(lbits)(&zghz3246);
  KILL(lbits)(&zghz3245);
  KILL(sail_int)(&zghz3244);
  KILL(sail_int)(&zghz3243);
  KILL(sail_int)(&zghz3242);
  KILL(sail_int)(&zghz3241);
  KILL(sail_int)(&zghz3240);
  KILL(sail_int)(&zghz3239);
  KILL(lbits)(&zghz3238);
  KILL(sail_int)(&zghz3237);
  KILL(lbits)(&zghz3236);
  KILL(lbits)(&zghz3235);
  KILL(sail_int)(&zghz3234);
  KILL(lbits)(&zghz3233);
  KILL(lbits)(&zghz3232);
  KILL(sail_int)(&zghz3231);
  KILL(lbits)(&zghz3230);
  KILL(lbits)(&zghz3229);
  KILL(lbits)(&zghz3228);
  KILL(lbits)(&zghz3227);
  KILL(sail_int)(&zghz3226);
  KILL(lbits)(&zghz3225);
  KILL(lbits)(&zghz3224);
  KILL(sail_int)(&zghz3223);
  KILL(lbits)(&zghz3222);
  KILL(lbits)(&zghz3221);
  KILL(sail_int)(&zghz3220);
  KILL(lbits)(&zghz3219);
  KILL(lbits)(&zghz3218);
  KILL(lbits)(&zghz3217);
  KILL(lbits)(&zghz3216);
  KILL(sail_int)(&zghz3215);
  KILL(lbits)(&zghz3214);
  KILL(sail_int)(&zghz3213);
  KILL(sail_int)(&zghz3212);
  KILL(sail_int)(&zghz3211);
  KILL(sail_int)(&zghz3210);
  KILL(sail_int)(&zghz3209);
  KILL(sail_int)(&zghz3208);
  KILL(sail_int)(&zghz3207);
  KILL(lbits)(&zghz3206);
  KILL(sail_int)(&zghz3205);
  KILL(sail_int)(&zghz3204);
  KILL(sail_int)(&zghz3203);
  KILL(sail_int)(&zghz3202);
  KILL(sail_int)(&zghz3201);
  KILL(lbits)(&zghz3200);
  KILL(lbits)(&zghz3199);
  KILL(sail_int)(&zghz3198);
  KILL(sail_int)(&zghz3197);
  KILL(sail_int)(&zghz3196);
  KILL(sail_int)(&zghz3195);
  KILL(sail_int)(&zghz3194);
  KILL(lbits)(&zghz3193);
  KILL(sail_int)(&zghz3192);
  KILL(sail_int)(&zghz3191);
  KILL(lbits)(&zghz3190);
  KILL(lbits)(&zghz3189);
}

static void sailgen_getCapBounds(struct zoptionzIz8izCiz9zK *rop, struct zCapability);

lbits zghz3262;
sail_int zghz3263;
sail_int zghz3264;
lbits zghz3265;

static void startup_sailgen_getCapBounds(void)
{
  CREATE(lbits)(&zghz3262);
  CREATE(sail_int)(&zghz3263);
  CREATE(sail_int)(&zghz3264);
  CREATE(lbits)(&zghz3265);
}

static void sailgen_getCapBounds(struct zoptionzIz8izCiz9zK *zcbz364, struct zCapability zcap)
{
  __label__ case_294, case_293, finish_match_292, end_function_295, end_block_exception_296, end_function_407;

  struct zoptionzIz8bzCbz9zK zgaz3268;
  CREATE(zoptionzIz8bzCbz9zK)(&zgaz3268);
  sailgen_getCapBoundsBits(&zgaz3268, zcap);
  struct zoptionzIz8izCiz9zK zgsz3241;
  CREATE(zoptionzIz8izCiz9zK)(&zgsz3241);
  {
    if (zgaz3268.kind != Kind_zNonezIz8bzCbz9zK) goto case_294;
    sailgen_NonezIz8izCiz9zK(&zgsz3241, UNIT);
    goto finish_match_292;
  }
case_294: ;
  {
    /* complete */
    uint64_t zbase;
    zbase = CONVERT_OF(fbits, lbits)(zgaz3268.variants.zSomezIz8bzCbz9zK.ztup0, true);
    RECREATE(lbits)(&zghz3262);
    COPY(lbits)(&zghz3262, zgaz3268.variants.zSomezIz8bzCbz9zK.ztup1);
    struct ztuple_z8z5izCz0z5iz9 zgaz3271;
    CREATE(ztuple_z8z5izCz0z5iz9)(&zgaz3271);
    {
      RECREATE(sail_int)(&zghz3263);
      {
        RECREATE(lbits)(&zghz3265);
        CONVERT_OF(lbits, fbits)(&zghz3265, zbase, UINT64_C(64) , true);
        sail_unsigned(&zghz3263, zghz3265);
      }
      RECREATE(sail_int)(&zghz3264);
      sail_unsigned(&zghz3264, zghz3262);
      COPY(sail_int)(&((&zgaz3271)->ztup0), zghz3263);
      COPY(sail_int)(&((&zgaz3271)->ztup1), zghz3264);
    }
    sailgen_SomezIz8izCiz9zK(&zgsz3241, zgaz3271);
    KILL(ztuple_z8z5izCz0z5iz9)(&zgaz3271);
    goto finish_match_292;
  }
case_293: ;
finish_match_292: ;
  COPY(zoptionzIz8izCiz9zK)((*(&zcbz364)), zgsz3241);
  KILL(zoptionzIz8izCiz9zK)(&zgsz3241);
  KILL(zoptionzIz8bzCbz9zK)(&zgaz3268);
end_function_295: ;
  goto end_function_407;
end_block_exception_296: ;
  goto end_function_407;
end_function_407: ;
}



static void finish_sailgen_getCapBounds(void)
{
  KILL(lbits)(&zghz3265);
  KILL(sail_int)(&zghz3264);
  KILL(sail_int)(&zghz3263);
  KILL(lbits)(&zghz3262);
}

// struct tuple_(%bool, %struct zCapability)
struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 {
  bool ztup0;
  struct zCapability ztup1;
};

static void COPY(ztuple_z8z5boolzCz0z5structz0zzCapabilityz9)(struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 *rop, const struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 op) {
  rop->ztup0 = op.ztup0;
  rop->ztup1 = op.ztup1;
}

static bool EQUAL(ztuple_z8z5boolzCz0z5structz0zzCapabilityz9)(struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 op1, struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 op2) {
  return EQUAL(bool)(op1.ztup0, op2.ztup0) && EQUAL(zCapability)(op1.ztup1, op2.ztup1);
}

static struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 sailgen_setCapBounds(struct zCapability, uint64_t, lbits);

lbits zghz3266;
lbits zghz3267;
struct zCapability zghz3268;
lbits zghz3269;
sail_int zghz3270;
lbits zghz3271;
sail_int zghz3272;
lbits zghz3273;
lbits zghz3274;
sail_int zghz3275;
lbits zghz3276;
lbits zghz3277;
sail_int zghz3278;
lbits zghz3279;
sail_int zghz3280;
sail_int zghz3281;
sail_int zghz3282;
sail_int zghz3283;
lbits zghz3284;
sail_int zghz3285;
lbits zghz3286;
sail_int zghz3287;
sail_int zghz3288;
sail_int zghz3289;
lbits zghz3290;
sail_int zghz3291;
lbits zghz3292;
sail_int zghz3293;
sail_int zghz3294;
sail_int zghz3295;
lbits zghz3296;
sail_int zghz3297;
lbits zghz3298;
lbits zghz3299;
lbits zghz3300;
lbits zghz3301;
lbits zghz3302;
sail_int zghz3303;
lbits zghz3304;
sail_int zghz3305;
lbits zghz3306;
lbits zghz3307;
sail_int zghz3308;
lbits zghz3309;
sail_int zghz3310;
sail_int zghz3311;
sail_int zghz3312;
sail_int zghz3313;
lbits zghz3314;
sail_int zghz3315;
lbits zghz3316;
sail_int zghz3317;
sail_int zghz3318;
sail_int zghz3319;
lbits zghz3320;
sail_int zghz3321;
lbits zghz3322;
sail_int zghz3323;
lbits zghz3324;
lbits zghz3325;
sail_int zghz3326;
lbits zghz3327;
sail_int zghz3328;
sail_int zghz3329;
sail_int zghz3330;
sail_int zghz3331;
sail_int zghz3332;
sail_int zghz3333;
sail_int zghz3334;
sail_int zghz3335;
sail_int zghz3336;
lbits zghz3337;
sail_int zghz3338;
sail_int zghz3339;
sail_int zghz3340;
lbits zghz3341;
sail_int zghz3342;
sail_int zghz3343;
sail_int zghz3344;
lbits zghz3345;
lbits zghz3346;

static void startup_sailgen_setCapBounds(void)
{
  CREATE(lbits)(&zghz3266);
  CREATE(lbits)(&zghz3267);

  CREATE(lbits)(&zghz3269);
  CREATE(sail_int)(&zghz3270);
  CREATE(lbits)(&zghz3271);
  CREATE(sail_int)(&zghz3272);
  CREATE(lbits)(&zghz3273);
  CREATE(lbits)(&zghz3274);
  CREATE(sail_int)(&zghz3275);
  CREATE(lbits)(&zghz3276);
  CREATE(lbits)(&zghz3277);
  CREATE(sail_int)(&zghz3278);
  CREATE(lbits)(&zghz3279);
  CREATE(sail_int)(&zghz3280);
  CREATE(sail_int)(&zghz3281);
  CREATE(sail_int)(&zghz3282);
  CREATE(sail_int)(&zghz3283);
  CREATE(lbits)(&zghz3284);
  CREATE(sail_int)(&zghz3285);
  CREATE(lbits)(&zghz3286);
  CREATE(sail_int)(&zghz3287);
  CREATE(sail_int)(&zghz3288);
  CREATE(sail_int)(&zghz3289);
  CREATE(lbits)(&zghz3290);
  CREATE(sail_int)(&zghz3291);
  CREATE(lbits)(&zghz3292);
  CREATE(sail_int)(&zghz3293);
  CREATE(sail_int)(&zghz3294);
  CREATE(sail_int)(&zghz3295);
  CREATE(lbits)(&zghz3296);
  CREATE(sail_int)(&zghz3297);
  CREATE(lbits)(&zghz3298);
  CREATE(lbits)(&zghz3299);
  CREATE(lbits)(&zghz3300);
  CREATE(lbits)(&zghz3301);
  CREATE(lbits)(&zghz3302);
  CREATE(sail_int)(&zghz3303);
  CREATE(lbits)(&zghz3304);
  CREATE(sail_int)(&zghz3305);
  CREATE(lbits)(&zghz3306);
  CREATE(lbits)(&zghz3307);
  CREATE(sail_int)(&zghz3308);
  CREATE(lbits)(&zghz3309);
  CREATE(sail_int)(&zghz3310);
  CREATE(sail_int)(&zghz3311);
  CREATE(sail_int)(&zghz3312);
  CREATE(sail_int)(&zghz3313);
  CREATE(lbits)(&zghz3314);
  CREATE(sail_int)(&zghz3315);
  CREATE(lbits)(&zghz3316);
  CREATE(sail_int)(&zghz3317);
  CREATE(sail_int)(&zghz3318);
  CREATE(sail_int)(&zghz3319);
  CREATE(lbits)(&zghz3320);
  CREATE(sail_int)(&zghz3321);
  CREATE(lbits)(&zghz3322);
  CREATE(sail_int)(&zghz3323);
  CREATE(lbits)(&zghz3324);
  CREATE(lbits)(&zghz3325);
  CREATE(sail_int)(&zghz3326);
  CREATE(lbits)(&zghz3327);
  CREATE(sail_int)(&zghz3328);
  CREATE(sail_int)(&zghz3329);
  CREATE(sail_int)(&zghz3330);
  CREATE(sail_int)(&zghz3331);
  CREATE(sail_int)(&zghz3332);
  CREATE(sail_int)(&zghz3333);
  CREATE(sail_int)(&zghz3334);
  CREATE(sail_int)(&zghz3335);
  CREATE(sail_int)(&zghz3336);
  CREATE(lbits)(&zghz3337);
  CREATE(sail_int)(&zghz3338);
  CREATE(sail_int)(&zghz3339);
  CREATE(sail_int)(&zghz3340);
  CREATE(lbits)(&zghz3341);
  CREATE(sail_int)(&zghz3342);
  CREATE(sail_int)(&zghz3343);
  CREATE(sail_int)(&zghz3344);
  CREATE(lbits)(&zghz3345);
  CREATE(lbits)(&zghz3346);
}

static struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 sailgen_setCapBounds(struct zCapability zcap, uint64_t zbase, lbits ztop)
{
  __label__ end_function_298, end_block_exception_299;

  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz365;
  RECREATE(lbits)(&zghz3266);
  {
    RECREATE(lbits)(&zghz3345);
    CONVERT_OF(lbits, fbits)(&zghz3345, UINT64_C(0b0), UINT64_C(1) , true);
    RECREATE(lbits)(&zghz3346);
    CONVERT_OF(lbits, fbits)(&zghz3346, zbase, UINT64_C(64) , true);
    append(&zghz3266, zghz3345, zghz3346);
  }
  RECREATE(lbits)(&zghz3267);
  sub_bits(&zghz3267, ztop, zghz3266);
  int64_t ze;
  {
    int64_t zgaz3309;
    {
      uint64_t zgaz3308;
      {
        int64_t zgaz3307;
        {
          RECREATE(sail_int)(&zghz3342);
          CONVERT_OF(sail_int, mach_int)(&zghz3342, zcap_mantissa_width);
          RECREATE(sail_int)(&zghz3343);
          CONVERT_OF(sail_int, mach_int)(&zghz3343, INT64_C(1));
          RECREATE(sail_int)(&zghz3344);
          sub_int(&zghz3344, zghz3342, zghz3343);
          zgaz3307 = CONVERT_OF(mach_int, sail_int)(zghz3344);
        }
        {
          RECREATE(sail_int)(&zghz3339);
          CONVERT_OF(sail_int, mach_int)(&zghz3339, zcap_addr_width);
          RECREATE(sail_int)(&zghz3340);
          CONVERT_OF(sail_int, mach_int)(&zghz3340, zgaz3307);
          RECREATE(lbits)(&zghz3341);
          vector_subrange_lbits(&zghz3341, zghz3267, zghz3339, zghz3340);
          zgaz3308 = CONVERT_OF(fbits, lbits)(zghz3341, true);
        }
      }
      {
        RECREATE(lbits)(&zghz3337);
        CONVERT_OF(lbits, fbits)(&zghz3337, zgaz3308, UINT64_C(52) , true);
        RECREATE(sail_int)(&zghz3338);
        count_leading_zeros(&zghz3338, zghz3337);
        zgaz3309 = CONVERT_OF(mach_int, sail_int)(zghz3338);
      }
    }
    {
      RECREATE(sail_int)(&zghz3334);
      CONVERT_OF(sail_int, mach_int)(&zghz3334, zcap_max_E);
      RECREATE(sail_int)(&zghz3335);
      CONVERT_OF(sail_int, mach_int)(&zghz3335, zgaz3309);
      RECREATE(sail_int)(&zghz3336);
      sub_int(&zghz3336, zghz3334, zghz3335);
      ze = CONVERT_OF(mach_int, sail_int)(zghz3336);
    }
  }
  bool zie;
  {
    bool zgaz3306;
    {
      RECREATE(sail_int)(&zghz3332);
      CONVERT_OF(sail_int, mach_int)(&zghz3332, ze);
      RECREATE(sail_int)(&zghz3333);
      CONVERT_OF(sail_int, mach_int)(&zghz3333, INT64_C(0));
      zgaz3306 = sailgen_neq_int(zghz3332, zghz3333);
    }
    bool zgsz3246;
    if (zgaz3306) {  zgsz3246 = true;  } else {
      bool zgaz3305;
      zgaz3305 = sailgen_not(zcap_use_L8);
      bool zgsz3245;
      if (zgaz3305) {
        fbits zgaz3304;
        {
          int64_t zgaz3303;
          {
            RECREATE(sail_int)(&zghz3329);
            CONVERT_OF(sail_int, mach_int)(&zghz3329, zcap_mantissa_width);
            RECREATE(sail_int)(&zghz3330);
            CONVERT_OF(sail_int, mach_int)(&zghz3330, INT64_C(2));
            RECREATE(sail_int)(&zghz3331);
            sub_int(&zghz3331, zghz3329, zghz3330);
            zgaz3303 = CONVERT_OF(mach_int, sail_int)(zghz3331);
          }
          {
            RECREATE(sail_int)(&zghz3328);
            CONVERT_OF(sail_int, mach_int)(&zghz3328, zgaz3303);
            zgaz3304 = bitvector_access(zghz3267, zghz3328);
          }
        }
        zgsz3245 = eq_bit(zgaz3304, UINT64_C(1));
      } else {  zgsz3245 = false;  }
      zgsz3246 = zgsz3245;
    }
    zie = zgsz3246;
  }
  uint64_t zBbits;
  {
    RECREATE(lbits)(&zghz3325);
    CONVERT_OF(lbits, fbits)(&zghz3325, zbase, UINT64_C(64) , true);
    RECREATE(sail_int)(&zghz3326);
    CONVERT_OF(sail_int, mach_int)(&zghz3326, zcap_mantissa_width);
    RECREATE(lbits)(&zghz3327);
    sail_truncate(&zghz3327, zghz3325, zghz3326);
    zBbits = CONVERT_OF(fbits, lbits)(zghz3327, true);
  }
  uint64_t zTbits;
  {
    RECREATE(sail_int)(&zghz3323);
    CONVERT_OF(sail_int, mach_int)(&zghz3323, zcap_mantissa_width);
    RECREATE(lbits)(&zghz3324);
    sail_truncate(&zghz3324, ztop, zghz3323);
    zTbits = CONVERT_OF(fbits, lbits)(zghz3324, true);
  }
  bool zlostSignificantTop;
  zlostSignificantTop = false;
  bool zlostSignificantBase;
  zlostSignificantBase = false;
  bool zincE;
  zincE = false;
  {
    unit zgsz3247;
    if (zie) {
      uint64_t zB_ie;
      {
        uint64_t zgaz3297;
        {
          int64_t zgaz3296;
          zgaz3296 = (ze + zcap_xe_width);
          {
            RECREATE(lbits)(&zghz3320);
            CONVERT_OF(lbits, fbits)(&zghz3320, zbase, UINT64_C(64) , true);
            RECREATE(sail_int)(&zghz3321);
            CONVERT_OF(sail_int, mach_int)(&zghz3321, zgaz3296);
            RECREATE(lbits)(&zghz3322);
            shiftr(&zghz3322, zghz3320, zghz3321);
            zgaz3297 = CONVERT_OF(fbits, lbits)(zghz3322, true);
          }
        }
        int64_t zgaz3298;
        {
          RECREATE(sail_int)(&zghz3317);
          CONVERT_OF(sail_int, mach_int)(&zghz3317, zcap_mantissa_width);
          RECREATE(sail_int)(&zghz3318);
          CONVERT_OF(sail_int, mach_int)(&zghz3318, zcap_xe_width);
          RECREATE(sail_int)(&zghz3319);
          sub_int(&zghz3319, zghz3317, zghz3318);
          zgaz3298 = CONVERT_OF(mach_int, sail_int)(zghz3319);
        }
        {
          RECREATE(lbits)(&zghz3314);
          CONVERT_OF(lbits, fbits)(&zghz3314, zgaz3297, UINT64_C(64) , true);
          RECREATE(sail_int)(&zghz3315);
          CONVERT_OF(sail_int, mach_int)(&zghz3315, zgaz3298);
          RECREATE(lbits)(&zghz3316);
          sail_truncate(&zghz3316, zghz3314, zghz3315);
          zB_ie = CONVERT_OF(fbits, lbits)(zghz3316, true);
        }
      }
      uint64_t zT_ie;
      {
        RECREATE(lbits)(&zghz3307);
        {
          int64_t zgaz3293;
          zgaz3293 = (ze + zcap_xe_width);
          {
            RECREATE(sail_int)(&zghz3313);
            CONVERT_OF(sail_int, mach_int)(&zghz3313, zgaz3293);
            shiftr(&zghz3307, ztop, zghz3313);
          }
        }
        int64_t zgaz3295;
        {
          RECREATE(sail_int)(&zghz3310);
          CONVERT_OF(sail_int, mach_int)(&zghz3310, zcap_mantissa_width);
          RECREATE(sail_int)(&zghz3311);
          CONVERT_OF(sail_int, mach_int)(&zghz3311, zcap_xe_width);
          RECREATE(sail_int)(&zghz3312);
          sub_int(&zghz3312, zghz3310, zghz3311);
          zgaz3295 = CONVERT_OF(mach_int, sail_int)(zghz3312);
        }
        {
          RECREATE(sail_int)(&zghz3308);
          CONVERT_OF(sail_int, mach_int)(&zghz3308, zgaz3295);
          RECREATE(lbits)(&zghz3309);
          sail_truncate(&zghz3309, zghz3307, zghz3308);
          zT_ie = CONVERT_OF(fbits, lbits)(zghz3309, true);
        }
      }
      RECREATE(lbits)(&zghz3269);
      {
        sbits zgaz3292;
        {
          int64_t zgaz3291;
          zgaz3291 = (ze + zcap_xe_width);
          {
            RECREATE(sail_int)(&zghz3305);
            CONVERT_OF(sail_int, mach_int)(&zghz3305, zgaz3291);
            RECREATE(lbits)(&zghz3306);
            sailgen_ones(&zghz3306, zghz3305);
            zgaz3292 = CONVERT_OF(sbits, lbits)(zghz3306, true);
          }
        }
        {
          RECREATE(sail_int)(&zghz3303);
          CONVERT_OF(sail_int, mach_int)(&zghz3303, INT64_C(65));
          RECREATE(lbits)(&zghz3304);
          CONVERT_OF(lbits, sbits)(&zghz3304, zgaz3292, true);
          sailgen_zzero_extend(&zghz3269, zghz3303, zghz3304);
        }
      }
      {
        RECREATE(lbits)(&zghz3301);
        and_bits(&zghz3301, zghz3266, zghz3269);
        RECREATE(lbits)(&zghz3302);
        CONVERT_OF(lbits, fbits)(&zghz3302, UINT64_C(0b0), UINT64_C(1) , true);
        append_64(&zghz3302, zghz3302, UINT64_C(0x0000000000000000));
        zlostSignificantBase = neq_bits(zghz3301, zghz3302);
        unit zgsz3252;
        zgsz3252 = UNIT;
      }
      {
        RECREATE(lbits)(&zghz3299);
        and_bits(&zghz3299, ztop, zghz3269);
        RECREATE(lbits)(&zghz3300);
        CONVERT_OF(lbits, fbits)(&zghz3300, UINT64_C(0b0), UINT64_C(1) , true);
        append_64(&zghz3300, zghz3300, UINT64_C(0x0000000000000000));
        zlostSignificantTop = neq_bits(zghz3299, zghz3300);
        unit zgsz3251;
        zgsz3251 = UNIT;
      }
      {
        unit zgsz3250;
        if (zlostSignificantTop) {
          {
            RECREATE(lbits)(&zghz3296);
            CONVERT_OF(lbits, fbits)(&zghz3296, zT_ie, UINT64_C(11) , true);
            RECREATE(sail_int)(&zghz3297);
            CONVERT_OF(sail_int, mach_int)(&zghz3297, INT64_C(1));
            RECREATE(lbits)(&zghz3298);
            add_bits_int(&zghz3298, zghz3296, zghz3297);
            zT_ie = CONVERT_OF(fbits, lbits)(zghz3298, true);
          }
          zgsz3250 = UNIT;
        } else {  zgsz3250 = UNIT;  }
      }
      uint64_t zlen_ie;
      zlen_ie = ((zT_ie - zB_ie) & UINT64_C(0b11111111111));
      {
        bool zgaz3277;
        {
          fbits zgaz3276;
          {
            int64_t zgaz3275;
            {
              int64_t zgaz3274;
              zgaz3274 = (zcap_xe_width + INT64_C(1));
              {
                RECREATE(sail_int)(&zghz3293);
                CONVERT_OF(sail_int, mach_int)(&zghz3293, zcap_mantissa_width);
                RECREATE(sail_int)(&zghz3294);
                CONVERT_OF(sail_int, mach_int)(&zghz3294, zgaz3274);
                RECREATE(sail_int)(&zghz3295);
                sub_int(&zghz3295, zghz3293, zghz3294);
                zgaz3275 = CONVERT_OF(mach_int, sail_int)(zghz3295);
              }
            }
            zgaz3276 = (UINT64_C(1) & (zlen_ie >> zgaz3275));
          }
          zgaz3277 = eq_bit(zgaz3276, UINT64_C(1));
        }
        unit zgsz3254;
        if (zgaz3277) {
          {
            zincE = true;
            unit zgsz3260;
            zgsz3260 = UNIT;
          }
          {
            bool zgsz3255;
            if (zlostSignificantBase) {  zgsz3255 = true;  } else {
              fbits zgaz3278;
              zgaz3278 = (UINT64_C(1) & (zB_ie >> INT64_C(0)));
              zgsz3255 = eq_bit(zgaz3278, UINT64_C(1));
            }
            zlostSignificantBase = zgsz3255;
            unit zgsz3259;
            zgsz3259 = UNIT;
          }
          {
            bool zgsz3256;
            if (zlostSignificantTop) {  zgsz3256 = true;  } else {
              fbits zgaz3279;
              zgaz3279 = (UINT64_C(1) & (zT_ie >> INT64_C(0)));
              zgsz3256 = eq_bit(zgaz3279, UINT64_C(1));
            }
            zlostSignificantTop = zgsz3256;
            unit zgsz3258;
            zgsz3258 = UNIT;
          }
          {
            uint64_t zgaz3282;
            {
              int64_t zgaz3281;
              {
                int64_t zgaz3280;
                zgaz3280 = (ze + zcap_xe_width);
                zgaz3281 = (zgaz3280 + INT64_C(1));
              }
              {
                RECREATE(lbits)(&zghz3290);
                CONVERT_OF(lbits, fbits)(&zghz3290, zbase, UINT64_C(64) , true);
                RECREATE(sail_int)(&zghz3291);
                CONVERT_OF(sail_int, mach_int)(&zghz3291, zgaz3281);
                RECREATE(lbits)(&zghz3292);
                shiftr(&zghz3292, zghz3290, zghz3291);
                zgaz3282 = CONVERT_OF(fbits, lbits)(zghz3292, true);
              }
            }
            int64_t zgaz3283;
            {
              RECREATE(sail_int)(&zghz3287);
              CONVERT_OF(sail_int, mach_int)(&zghz3287, zcap_mantissa_width);
              RECREATE(sail_int)(&zghz3288);
              CONVERT_OF(sail_int, mach_int)(&zghz3288, zcap_xe_width);
              RECREATE(sail_int)(&zghz3289);
              sub_int(&zghz3289, zghz3287, zghz3288);
              zgaz3283 = CONVERT_OF(mach_int, sail_int)(zghz3289);
            }
            {
              RECREATE(lbits)(&zghz3284);
              CONVERT_OF(lbits, fbits)(&zghz3284, zgaz3282, UINT64_C(64) , true);
              RECREATE(sail_int)(&zghz3285);
              CONVERT_OF(sail_int, mach_int)(&zghz3285, zgaz3283);
              RECREATE(lbits)(&zghz3286);
              sail_truncate(&zghz3286, zghz3284, zghz3285);
              zB_ie = CONVERT_OF(fbits, lbits)(zghz3286, true);
            }
            unit zgsz3257;
            zgsz3257 = UNIT;
          }
          int64_t zincT;
          if (zlostSignificantTop) {  zincT = INT64_C(1);  } else {  zincT = INT64_C(0);  }
          uint64_t zgaz3288;
          {
            RECREATE(lbits)(&zghz3277);
            {
              int64_t zgaz3285;
              {
                int64_t zgaz3284;
                zgaz3284 = (ze + zcap_xe_width);
                zgaz3285 = (zgaz3284 + INT64_C(1));
              }
              {
                RECREATE(sail_int)(&zghz3283);
                CONVERT_OF(sail_int, mach_int)(&zghz3283, zgaz3285);
                shiftr(&zghz3277, ztop, zghz3283);
              }
            }
            int64_t zgaz3287;
            {
              RECREATE(sail_int)(&zghz3280);
              CONVERT_OF(sail_int, mach_int)(&zghz3280, zcap_mantissa_width);
              RECREATE(sail_int)(&zghz3281);
              CONVERT_OF(sail_int, mach_int)(&zghz3281, zcap_xe_width);
              RECREATE(sail_int)(&zghz3282);
              sub_int(&zghz3282, zghz3280, zghz3281);
              zgaz3287 = CONVERT_OF(mach_int, sail_int)(zghz3282);
            }
            {
              RECREATE(sail_int)(&zghz3278);
              CONVERT_OF(sail_int, mach_int)(&zghz3278, zgaz3287);
              RECREATE(lbits)(&zghz3279);
              sail_truncate(&zghz3279, zghz3277, zghz3278);
              zgaz3288 = CONVERT_OF(fbits, lbits)(zghz3279, true);
            }
          }
          {
            RECREATE(lbits)(&zghz3274);
            CONVERT_OF(lbits, fbits)(&zghz3274, zgaz3288, UINT64_C(11) , true);
            RECREATE(sail_int)(&zghz3275);
            CONVERT_OF(sail_int, mach_int)(&zghz3275, zincT);
            RECREATE(lbits)(&zghz3276);
            add_bits_int(&zghz3276, zghz3274, zghz3275);
            zT_ie = CONVERT_OF(fbits, lbits)(zghz3276, true);
          }
          zgsz3254 = UNIT;
        } else {  zgsz3254 = UNIT;  }
      }
      {
        uint64_t zgaz3289;
        {
          RECREATE(sail_int)(&zghz3272);
          CONVERT_OF(sail_int, mach_int)(&zghz3272, zcap_xe_width);
          RECREATE(lbits)(&zghz3273);
          sailgen_zzeros_implicit(&zghz3273, zghz3272);
          zgaz3289 = CONVERT_OF(fbits, lbits)(zghz3273, true);
        }
        zBbits = (zB_ie << 3) | zgaz3289;
        unit zgsz3253;
        zgsz3253 = UNIT;
      }
      uint64_t zgaz3290;
      {
        RECREATE(sail_int)(&zghz3270);
        CONVERT_OF(sail_int, mach_int)(&zghz3270, zcap_xe_width);
        RECREATE(lbits)(&zghz3271);
        sailgen_zzeros_implicit(&zghz3271, zghz3270);
        zgaz3290 = CONVERT_OF(fbits, lbits)(zghz3271, true);
      }
      zTbits = (zT_ie << 3) | zgaz3290;
      zgsz3247 = UNIT;
    } else {  zgsz3247 = UNIT;  }
  }
  struct zCapability zghz3268;
  {
    int64_t zgaz3302;
    if (zincE) {
      bool zgaz3301;
      {
        int64_t zgaz3300;
        zgaz3300 = (ze + INT64_C(1));
        zgaz3301 = (zgaz3300 <= zcap_max_E);
      }
      if (zgaz3301) {  zgaz3302 = (ze + INT64_C(1));  } else {  zgaz3302 = zcap_min_E;  }
    } else {  zgaz3302 = ze;  }
    zghz3268 = zcap;
    zghz3268.zB = zBbits;
    zghz3268.zE = zgaz3302;
    zghz3268.zT = zTbits;
    zghz3268.zaddress = zbase;
    zghz3268.zinternal_exponent = zie;
  }
  bool zexact;
  {
    bool zgaz3299;
    {    if (zlostSignificantBase) {  zgaz3299 = true;  } else {  zgaz3299 = zlostSignificantTop;  }
    }
    zexact = sailgen_not(zgaz3299);
  }
  zcbz365.ztup0 = zexact;
  zcbz365.ztup1 = zghz3268;
end_function_298: ;
  return zcbz365;
end_block_exception_299: ;
  struct zCapability zcbz3108 = { .zB = UINT64_C(0xdeadc0de), .zE = INT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zap_m = UINT64_C(0xdeadc0de), .zcl = UINT64_C(0xdeadc0de), .zinternal_exponent = false, .zreserved_0 = UINT64_C(0xdeadc0de), .zreserved_1 = UINT64_C(0xdeadc0de), .zsd_perms = UINT64_C(0xdeadc0de), .zsealed = false, .ztag = false };
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz3107 = { .ztup0 = false, .ztup1 = zcbz3108 };
  return zcbz3107;
}



static void finish_sailgen_setCapBounds(void)
{
  KILL(lbits)(&zghz3346);
  KILL(lbits)(&zghz3345);
  KILL(sail_int)(&zghz3344);
  KILL(sail_int)(&zghz3343);
  KILL(sail_int)(&zghz3342);
  KILL(lbits)(&zghz3341);
  KILL(sail_int)(&zghz3340);
  KILL(sail_int)(&zghz3339);
  KILL(sail_int)(&zghz3338);
  KILL(lbits)(&zghz3337);
  KILL(sail_int)(&zghz3336);
  KILL(sail_int)(&zghz3335);
  KILL(sail_int)(&zghz3334);
  KILL(sail_int)(&zghz3333);
  KILL(sail_int)(&zghz3332);
  KILL(sail_int)(&zghz3331);
  KILL(sail_int)(&zghz3330);
  KILL(sail_int)(&zghz3329);
  KILL(sail_int)(&zghz3328);
  KILL(lbits)(&zghz3327);
  KILL(sail_int)(&zghz3326);
  KILL(lbits)(&zghz3325);
  KILL(lbits)(&zghz3324);
  KILL(sail_int)(&zghz3323);
  KILL(lbits)(&zghz3322);
  KILL(sail_int)(&zghz3321);
  KILL(lbits)(&zghz3320);
  KILL(sail_int)(&zghz3319);
  KILL(sail_int)(&zghz3318);
  KILL(sail_int)(&zghz3317);
  KILL(lbits)(&zghz3316);
  KILL(sail_int)(&zghz3315);
  KILL(lbits)(&zghz3314);
  KILL(sail_int)(&zghz3313);
  KILL(sail_int)(&zghz3312);
  KILL(sail_int)(&zghz3311);
  KILL(sail_int)(&zghz3310);
  KILL(lbits)(&zghz3309);
  KILL(sail_int)(&zghz3308);
  KILL(lbits)(&zghz3307);
  KILL(lbits)(&zghz3306);
  KILL(sail_int)(&zghz3305);
  KILL(lbits)(&zghz3304);
  KILL(sail_int)(&zghz3303);
  KILL(lbits)(&zghz3302);
  KILL(lbits)(&zghz3301);
  KILL(lbits)(&zghz3300);
  KILL(lbits)(&zghz3299);
  KILL(lbits)(&zghz3298);
  KILL(sail_int)(&zghz3297);
  KILL(lbits)(&zghz3296);
  KILL(sail_int)(&zghz3295);
  KILL(sail_int)(&zghz3294);
  KILL(sail_int)(&zghz3293);
  KILL(lbits)(&zghz3292);
  KILL(sail_int)(&zghz3291);
  KILL(lbits)(&zghz3290);
  KILL(sail_int)(&zghz3289);
  KILL(sail_int)(&zghz3288);
  KILL(sail_int)(&zghz3287);
  KILL(lbits)(&zghz3286);
  KILL(sail_int)(&zghz3285);
  KILL(lbits)(&zghz3284);
  KILL(sail_int)(&zghz3283);
  KILL(sail_int)(&zghz3282);
  KILL(sail_int)(&zghz3281);
  KILL(sail_int)(&zghz3280);
  KILL(lbits)(&zghz3279);
  KILL(sail_int)(&zghz3278);
  KILL(lbits)(&zghz3277);
  KILL(lbits)(&zghz3276);
  KILL(sail_int)(&zghz3275);
  KILL(lbits)(&zghz3274);
  KILL(lbits)(&zghz3273);
  KILL(sail_int)(&zghz3272);
  KILL(lbits)(&zghz3271);
  KILL(sail_int)(&zghz3270);
  KILL(lbits)(&zghz3269);
  KILL(lbits)(&zghz3267);
  KILL(lbits)(&zghz3266);
}

static bool sailgen_capIsSealed(struct zCapability);

static bool sailgen_capIsSealed(struct zCapability zcap)
{
  __label__ end_function_301, end_block_exception_302;

  bool zcbz366;
  zcbz366 = zcap.zsealed;
end_function_301: ;
  return zcbz366;
end_block_exception_302: ;

  return false;
}

static bool sailgen_permsMalformed(struct zCapability);

static bool sailgen_permsMalformed(struct zCapability zcap)
{
  __label__ end_function_304, end_block_exception_305;

  bool zcbz367;
  bool zgaz3311;
  {
    uint64_t zgaz3310;
    zgaz3310 = zcap.zap_m;
    zgaz3311 = sailgen_ap_m_encdec_forwards_matches(zgaz3310);
  }
  zcbz367 = sailgen_not(zgaz3311);
end_function_304: ;
  return zcbz367;
end_block_exception_305: ;

  return false;
}

static void sailgen_getCapPermsAndMode(struct zoptionzIz8RArchPermszCEExecutionModez5z9zK *rop, struct zCapability);

static void sailgen_getCapPermsAndMode(struct zoptionzIz8RArchPermszCEExecutionModez5z9zK *zcbz368, struct zCapability zcap)
{
  __label__ end_function_307, end_block_exception_308, end_function_406;

  bool zgaz3312;
  zgaz3312 = sailgen_permsMalformed(zcap);
  if (zgaz3312) {  sailgen_NonezIz8RArchPermszCEExecutionModez5z9zK((*(&zcbz368)), UNIT);  } else {
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgaz3314;
    {
      uint64_t zgaz3313;
      zgaz3313 = zcap.zap_m;
      zgaz3314 = sailgen_ap_m_encdec_forwards(zgaz3313);
    }
    sailgen_SomezIz8RArchPermszCEExecutionModez5z9zK((*(&zcbz368)), zgaz3314);
  }
end_function_307: ;
  goto end_function_406;
end_block_exception_308: ;
  goto end_function_406;
end_function_406: ;
}

static struct zCapability sailgen_setCapPermsAndMode(struct zCapability, struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9);

struct zCapability zghz3347;
struct zArchPerms zghz3348;

static void startup_sailgen_setCapPermsAndMode(void)
{


}

static struct zCapability sailgen_setCapPermsAndMode(struct zCapability zcap, struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz3264)
{
  __label__ fundef_fail_309, fundef_body_310, end_function_311, end_block_exception_312;

  struct zCapability zghz3347;
  struct zArchPerms zghz3348;
  zghz3348 = zgsz3264.ztup0;
  enum zExecutionMode zmode;
  zmode = zgsz3264.ztup1;
  goto fundef_body_310;
fundef_fail_309: ;
  sail_match_failure("setCapPermsAndMode");
fundef_body_310: ;
  uint64_t zgaz3316;
  {
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgaz3315;
    zgaz3315 = sailgen_legalizzePermsAndMode(zghz3348, zmode);
    zgaz3316 = sailgen_ap_m_encdec_backwards(zgaz3315);
  }
  zghz3347 = zcap;
  zghz3347.zap_m = zgaz3316;
end_function_311: ;
  return zghz3347;
end_block_exception_312: ;
  struct zCapability zcbz3109 = { .zB = UINT64_C(0xdeadc0de), .zE = INT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zap_m = UINT64_C(0xdeadc0de), .zcl = UINT64_C(0xdeadc0de), .zinternal_exponent = false, .zreserved_0 = UINT64_C(0xdeadc0de), .zreserved_1 = UINT64_C(0xdeadc0de), .zsd_perms = UINT64_C(0xdeadc0de), .zsealed = false, .ztag = false };
  return zcbz3109;
}



static void finish_sailgen_setCapPermsAndMode(void)
{
}

static void sailgen_getArchPerms(struct zoptionzIRArchPermszK *rop, struct zCapability);

struct zArchPerms zghz3349;

static void startup_sailgen_getArchPerms(void)
{
}

static void sailgen_getArchPerms(struct zoptionzIRArchPermszK *zcbz370, struct zCapability zcap)
{
  __label__ case_316, case_315, finish_match_314, end_function_317, end_block_exception_318, end_function_405;

  struct zoptionzIz8RArchPermszCEExecutionModez5z9zK zgaz3317;
  CREATE(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zgaz3317);
  sailgen_getCapPermsAndMode(&zgaz3317, zcap);
  struct zoptionzIRArchPermszK zgsz3266;
  CREATE(zoptionzIRArchPermszK)(&zgsz3266);
  {
    if (zgaz3317.kind != Kind_zNonezIz8RArchPermszCEExecutionModez5z9zK) goto case_316;
    sailgen_NonezIRArchPermszK(&zgsz3266, UNIT);
    goto finish_match_314;
  }
case_316: ;
  {
    /* complete */
    struct zArchPerms zghz3349;
    zghz3349 = zgaz3317.variants.zSomezIz8RArchPermszCEExecutionModez5z9zK.ztup0;
    sailgen_SomezIRArchPermszK(&zgsz3266, zghz3349);
    goto finish_match_314;
  }
case_315: ;
finish_match_314: ;
  COPY(zoptionzIRArchPermszK)((*(&zcbz370)), zgsz3266);
  KILL(zoptionzIRArchPermszK)(&zgsz3266);
  KILL(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zgaz3317);
end_function_317: ;
  goto end_function_405;
end_block_exception_318: ;
  goto end_function_405;
end_function_405: ;
}



static void finish_sailgen_getArchPerms(void)
{
}

static struct zArchPerms sailgen_getArchPermsLegalizzed(struct zCapability);

struct zArchPerms zghz3350;
struct zArchPerms zghz3351;

static void startup_sailgen_getArchPermsLegalizzed(void)
{


}

static struct zArchPerms sailgen_getArchPermsLegalizzed(struct zCapability zcap)
{
  __label__ case_322, case_321, finish_match_320, end_function_323, end_block_exception_324;

  struct zArchPerms zghz3350;
  struct zoptionzIRArchPermszK zgaz3318;
  CREATE(zoptionzIRArchPermszK)(&zgaz3318);
  sailgen_getArchPerms(&zgaz3318, zcap);
  {
    if (zgaz3318.kind != Kind_zNonezIRArchPermszK) goto case_322;
    zghz3350 = znull_perms;
    goto finish_match_320;
  }
case_322: ;
  {
    /* complete */
    struct zArchPerms zghz3351;
    zghz3351 = zgaz3318.variants.zSomezIRArchPermszK;
    zghz3350 = zghz3351;
    goto finish_match_320;
  }
case_321: ;
finish_match_320: ;
  KILL(zoptionzIRArchPermszK)(&zgaz3318);
end_function_323: ;
  return zghz3350;
end_block_exception_324: ;
  struct zArchPerms zcbz3110 = { .zASR = false, .zC = false, .zEL = false, .zLM = false, .zR = false, .zSL = false, .zW = false, .zX = false };
  return zcbz3110;
}



static void finish_sailgen_getArchPermsLegalizzed(void)
{
}

static enum zExecutionMode sailgen_getCapMode(struct zCapability);

static enum zExecutionMode sailgen_getCapMode(struct zCapability zcap)
{
  __label__ case_328, case_327, finish_match_326, end_function_329, end_block_exception_330;

  enum zExecutionMode zcbz372;
  struct zoptionzIz8RArchPermszCEExecutionModez5z9zK zgaz3319;
  CREATE(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zgaz3319);
  sailgen_getCapPermsAndMode(&zgaz3319, zcap);
  {
    if (zgaz3319.kind != Kind_zNonezIz8RArchPermszCEExecutionModez5z9zK) goto case_328;
    zcbz372 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    goto finish_match_326;
  }
case_328: ;
  {
    /* complete */
    enum zExecutionMode zmode;
    zmode = zgaz3319.variants.zSomezIz8RArchPermszCEExecutionModez5z9zK.ztup1;
    zcbz372 = zmode;
    goto finish_match_326;
  }
case_327: ;
finish_match_326: ;
  KILL(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zgaz3319);
end_function_329: ;
  return zcbz372;
end_block_exception_330: ;

  return zCapPtrMode;
}

static void sailgen_getCapLength(sail_int *rop, struct zCapability);

sail_int zghz3352;
sail_int zghz3353;
sail_int zghz3354;

static void startup_sailgen_getCapLength(void)
{
  CREATE(sail_int)(&zghz3352);
  CREATE(sail_int)(&zghz3353);
  CREATE(sail_int)(&zghz3354);
}

static void sailgen_getCapLength(sail_int *zcbz373, struct zCapability zc)
{
  __label__ case_334, case_333, finish_match_332, end_function_335, end_block_exception_336, end_function_404;

  struct zoptionzIz8izCiz9zK zgaz3320;
  CREATE(zoptionzIz8izCiz9zK)(&zgaz3320);
  sailgen_getCapBounds(&zgaz3320, zc);
  RECREATE(sail_int)(&zghz3352);
  {
    if (zgaz3320.kind != Kind_zNonezIz8izCiz9zK) goto case_334;
    CONVERT_OF(sail_int, mach_int)(&zghz3352, INT64_C(0));
    goto finish_match_332;
  }
case_334: ;
  {
    /* complete */
    RECREATE(sail_int)(&zghz3353);
    COPY(sail_int)(&zghz3353, zgaz3320.variants.zSomezIz8izCiz9zK.ztup0);
    RECREATE(sail_int)(&zghz3354);
    COPY(sail_int)(&zghz3354, zgaz3320.variants.zSomezIz8izCiz9zK.ztup1);
    {
      bool zgaz3321;
      zgaz3321 = gteq(zghz3354, zghz3353);
      unit zgsz3276;
      zgsz3276 = sail_assert(zgaz3321, "src/cheri_cap_common.sail:448.42-448.43");
    }
    sub_int(&zghz3352, zghz3354, zghz3353);
    goto finish_match_332;
  }
case_333: ;
finish_match_332: ;
  COPY(sail_int)((*(&zcbz373)), zghz3352);
  KILL(zoptionzIz8izCiz9zK)(&zgaz3320);
end_function_335: ;
  goto end_function_404;
end_block_exception_336: ;
  goto end_function_404;
end_function_404: ;
}



static void finish_sailgen_getCapLength(void)
{
  KILL(sail_int)(&zghz3354);
  KILL(sail_int)(&zghz3353);
  KILL(sail_int)(&zghz3352);
}

static bool sailgen_inCapBoundsNoWrap(struct zCapability, uint64_t, sail_int);

sail_int zghz3355;
sail_int zghz3356;
sail_int zghz3357;
sail_int zghz3358;
lbits zghz3359;

static void startup_sailgen_inCapBoundsNoWrap(void)
{
  CREATE(sail_int)(&zghz3355);
  CREATE(sail_int)(&zghz3356);
  CREATE(sail_int)(&zghz3357);
  CREATE(sail_int)(&zghz3358);
  CREATE(lbits)(&zghz3359);
}

static bool sailgen_inCapBoundsNoWrap(struct zCapability zcap, uint64_t zaddr, sail_int zsizze)
{
  __label__ case_340, case_339, finish_match_338, end_function_341, end_block_exception_342;

  bool zcbz374;
  RECREATE(sail_int)(&zghz3355);
  {
    RECREATE(lbits)(&zghz3359);
    CONVERT_OF(lbits, fbits)(&zghz3359, zaddr, UINT64_C(64) , true);
    sail_unsigned(&zghz3355, zghz3359);
  }
  struct zoptionzIz8izCiz9zK zgaz3322;
  CREATE(zoptionzIz8izCiz9zK)(&zgaz3322);
  sailgen_getCapBounds(&zgaz3322, zcap);
  {
    if (zgaz3322.kind != Kind_zNonezIz8izCiz9zK) goto case_340;
    zcbz374 = false;
    goto finish_match_338;
  }
case_340: ;
  {
    /* complete */
    RECREATE(sail_int)(&zghz3356);
    COPY(sail_int)(&zghz3356, zgaz3322.variants.zSomezIz8izCiz9zK.ztup0);
    RECREATE(sail_int)(&zghz3357);
    COPY(sail_int)(&zghz3357, zgaz3322.variants.zSomezIz8izCiz9zK.ztup1);
    bool zgaz3324;
    zgaz3324 = gteq(zghz3355, zghz3356);
    bool zgsz3280;
    if (zgaz3324) {
      RECREATE(sail_int)(&zghz3358);
      add_int(&zghz3358, zghz3355, zsizze);
      zgsz3280 = lteq(zghz3358, zghz3357);
    } else {  zgsz3280 = false;  }
    zcbz374 = zgsz3280;
    goto finish_match_338;
  }
case_339: ;
finish_match_338: ;
  KILL(zoptionzIz8izCiz9zK)(&zgaz3322);
end_function_341: ;
  return zcbz374;
end_block_exception_342: ;

  return false;
}



static void finish_sailgen_inCapBoundsNoWrap(void)
{
  KILL(lbits)(&zghz3359);
  KILL(sail_int)(&zghz3358);
  KILL(sail_int)(&zghz3357);
  KILL(sail_int)(&zghz3356);
  KILL(sail_int)(&zghz3355);
}

static bool sailgen_capReservedValid(struct zCapability);

static bool sailgen_capReservedValid(struct zCapability zcap)
{
  __label__ end_function_344, end_block_exception_345;

  bool zcbz375;
  bool zgaz3327;
  {
    uint64_t zgaz3325;
    zgaz3325 = zcap.zreserved_0;
    zgaz3327 = (zgaz3325 == UINT64_C(0b000000000000000));
  }
  bool zgsz3283;
  if (zgaz3327) {
    uint64_t zgaz3326;
    zgaz3326 = zcap.zreserved_1;
    zgsz3283 = (zgaz3326 == UINT64_C(0b0000000));
  } else {  zgsz3283 = false;  }
  zcbz375 = zgsz3283;
end_function_344: ;
  return zcbz375;
end_block_exception_345: ;

  return false;
}

static struct zCapability sailgen_clearTagIf(struct zCapability, bool);

struct zCapability zghz3360;

static void startup_sailgen_clearTagIf(void)
{
}

static struct zCapability sailgen_clearTagIf(struct zCapability zcap, bool zcond)
{
  __label__ end_function_347, end_block_exception_348;

  struct zCapability zghz3360;
  bool zgaz3329;
  {
    bool zgaz3328;
    zgaz3328 = zcap.ztag;
    bool zgsz3284;
    if (zgaz3328) {  zgsz3284 = sailgen_not(zcond);  } else {  zgsz3284 = false;  }
    zgaz3329 = zgsz3284;
  }
  zghz3360 = zcap;
  zghz3360.ztag = zgaz3329;
end_function_347: ;
  return zghz3360;
end_block_exception_348: ;
  struct zCapability zcbz3111 = { .zB = UINT64_C(0xdeadc0de), .zE = INT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zap_m = UINT64_C(0xdeadc0de), .zcl = UINT64_C(0xdeadc0de), .zinternal_exponent = false, .zreserved_0 = UINT64_C(0xdeadc0de), .zreserved_1 = UINT64_C(0xdeadc0de), .zsd_perms = UINT64_C(0xdeadc0de), .zsealed = false, .ztag = false };
  return zcbz3111;
}



static void finish_sailgen_clearTagIf(void)
{
}

static bool zsys_legalizze_malformed_permission_on_load;
static void create_letbind_26(void) {


  bool zgsz3286;
  zgsz3286 = false;
  zsys_legalizze_malformed_permission_on_load = zgsz3286;
let_end_349: ;
}
static void kill_letbind_26(void) {
}

static bool sailgen_capBoundsEqual(struct zCapability, struct zCapability);

sail_int zghz3361;
sail_int zghz3362;
sail_int zghz3363;
sail_int zghz3364;

static void startup_sailgen_capBoundsEqual(void)
{
  CREATE(sail_int)(&zghz3361);
  CREATE(sail_int)(&zghz3362);
  CREATE(sail_int)(&zghz3363);
  CREATE(sail_int)(&zghz3364);
}

// struct tuple_(%union zoptionzIz8izCiz9zK, %union zoptionzIz8izCiz9zK)
struct ztuple_z8z5unionz0zzoptionzzIzz8izzCizz9zzKzCz0z5unionz0zzoptionzzIzz8izzCizz9zzKz9 {
  struct zoptionzIz8izCiz9zK ztup0;
  struct zoptionzIz8izCiz9zK ztup1;
};

static void COPY(ztuple_z8z5unionz0zzoptionzzIzz8izzCizz9zzKzCz0z5unionz0zzoptionzzIzz8izzCizz9zzKz9)(struct ztuple_z8z5unionz0zzoptionzzIzz8izzCizz9zzKzCz0z5unionz0zzoptionzzIzz8izzCizz9zzKz9 *rop, const struct ztuple_z8z5unionz0zzoptionzzIzz8izzCizz9zzKzCz0z5unionz0zzoptionzzIzz8izzCizz9zzKz9 op) {
  COPY(zoptionzIz8izCiz9zK)(&rop->ztup0, op.ztup0);
  COPY(zoptionzIz8izCiz9zK)(&rop->ztup1, op.ztup1);
}

static void CREATE(ztuple_z8z5unionz0zzoptionzzIzz8izzCizz9zzKzCz0z5unionz0zzoptionzzIzz8izzCizz9zzKz9)(struct ztuple_z8z5unionz0zzoptionzzIzz8izzCizz9zzKzCz0z5unionz0zzoptionzzIzz8izzCizz9zzKz9 *op) {
  CREATE(zoptionzIz8izCiz9zK)(&op->ztup0);
  CREATE(zoptionzIz8izCiz9zK)(&op->ztup1);
}

static void RECREATE(ztuple_z8z5unionz0zzoptionzzIzz8izzCizz9zzKzCz0z5unionz0zzoptionzzIzz8izzCizz9zzKz9)(struct ztuple_z8z5unionz0zzoptionzzIzz8izzCizz9zzKzCz0z5unionz0zzoptionzzIzz8izzCizz9zzKz9 *op) {
  RECREATE(zoptionzIz8izCiz9zK)(&op->ztup0);
  RECREATE(zoptionzIz8izCiz9zK)(&op->ztup1);
}

static void KILL(ztuple_z8z5unionz0zzoptionzzIzz8izzCizz9zzKzCz0z5unionz0zzoptionzzIzz8izzCizz9zzKz9)(struct ztuple_z8z5unionz0zzoptionzzIzz8izzCizz9zzKzCz0z5unionz0zzoptionzzIzz8izzCizz9zzKz9 *op) {
  KILL(zoptionzIz8izCiz9zK)(&op->ztup0);
  KILL(zoptionzIz8izCiz9zK)(&op->ztup1);
}

static bool EQUAL(ztuple_z8z5unionz0zzoptionzzIzz8izzCizz9zzKzCz0z5unionz0zzoptionzzIzz8izzCizz9zzKz9)(struct ztuple_z8z5unionz0zzoptionzzIzz8izzCizz9zzKzCz0z5unionz0zzoptionzzIzz8izzCizz9zzKz9 op1, struct ztuple_z8z5unionz0zzoptionzzIzz8izzCizz9zzKzCz0z5unionz0zzoptionzzIzz8izzCizz9zzKz9 op2) {
  return EQUAL(zoptionzIz8izCiz9zK)(op1.ztup0, op2.ztup0) && EQUAL(zoptionzIz8izCiz9zK)(op1.ztup1, op2.ztup1);
}

static bool sailgen_capBoundsEqual(struct zCapability zc1, struct zCapability zc2)
{
  __label__ case_354, case_353, case_352, finish_match_351, end_function_355, end_block_exception_356;

  bool zcbz377;
  struct ztuple_z8z5unionz0zzoptionzzIzz8izzCizz9zzKzCz0z5unionz0zzoptionzzIzz8izzCizz9zzKz9 zgaz3332;
  CREATE(ztuple_z8z5unionz0zzoptionzzIzz8izzCizz9zzKzCz0z5unionz0zzoptionzzIzz8izzCizz9zzKz9)(&zgaz3332);
  {
    struct zoptionzIz8izCiz9zK zgaz3330;
    CREATE(zoptionzIz8izCiz9zK)(&zgaz3330);
    sailgen_getCapBounds(&zgaz3330, zc1);
    struct zoptionzIz8izCiz9zK zgaz3331;
    CREATE(zoptionzIz8izCiz9zK)(&zgaz3331);
    sailgen_getCapBounds(&zgaz3331, zc2);
    COPY(zoptionzIz8izCiz9zK)(&((&zgaz3332)->ztup0), zgaz3330);
    COPY(zoptionzIz8izCiz9zK)(&((&zgaz3332)->ztup1), zgaz3331);
    KILL(zoptionzIz8izCiz9zK)(&zgaz3331);
    KILL(zoptionzIz8izCiz9zK)(&zgaz3330);
  }
  {
    if (zgaz3332.ztup0.kind != Kind_zNonezIz8izCiz9zK) goto case_354;
    zcbz377 = false;
    goto finish_match_351;
  }
case_354: ;
  {
    if (zgaz3332.ztup1.kind != Kind_zNonezIz8izCiz9zK) goto case_353;
    zcbz377 = false;
    goto finish_match_351;
  }
case_353: ;
  {
    /* complete */
    /* complete */
    RECREATE(sail_int)(&zghz3361);
    COPY(sail_int)(&zghz3361, zgaz3332.ztup0.variants.zSomezIz8izCiz9zK.ztup0);
    RECREATE(sail_int)(&zghz3362);
    COPY(sail_int)(&zghz3362, zgaz3332.ztup0.variants.zSomezIz8izCiz9zK.ztup1);
    RECREATE(sail_int)(&zghz3363);
    COPY(sail_int)(&zghz3363, zgaz3332.ztup1.variants.zSomezIz8izCiz9zK.ztup0);
    RECREATE(sail_int)(&zghz3364);
    COPY(sail_int)(&zghz3364, zgaz3332.ztup1.variants.zSomezIz8izCiz9zK.ztup1);
    bool zgaz3333;
    zgaz3333 = eq_int(zghz3361, zghz3363);
    bool zgsz3289;
    if (zgaz3333) {  zgsz3289 = eq_int(zghz3362, zghz3364);  } else {  zgsz3289 = false;  }
    zcbz377 = zgsz3289;
    goto finish_match_351;
  }
case_352: ;
finish_match_351: ;
  KILL(ztuple_z8z5unionz0zzoptionzzIzz8izzCizz9zzKzCz0z5unionz0zzoptionzzIzz8izzCizz9zzKz9)(&zgaz3332);
end_function_355: ;
  return zcbz377;
end_block_exception_356: ;

  return false;
}



static void finish_sailgen_capBoundsEqual(void)
{
  KILL(sail_int)(&zghz3364);
  KILL(sail_int)(&zghz3363);
  KILL(sail_int)(&zghz3362);
  KILL(sail_int)(&zghz3361);
}

static struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 sailgen_setCapAddr(struct zCapability, uint64_t);

struct zCapability zghz3365;

static void startup_sailgen_setCapAddr(void)
{
}

static struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 sailgen_setCapAddr(struct zCapability zc, uint64_t zaddr)
{
  __label__ end_function_358, end_block_exception_359;

  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz378;
  struct zCapability zghz3365;
  {
    zghz3365 = zc;
    zghz3365.zaddress = zaddr;
  }
  bool zrepresentable;
  zrepresentable = sailgen_capBoundsEqual(zc, zghz3365);
  zcbz378.ztup0 = zrepresentable;
  zcbz378.ztup1 = zghz3365;
end_function_358: ;
  return zcbz378;
end_block_exception_359: ;
  struct zCapability zcbz3113 = { .zB = UINT64_C(0xdeadc0de), .zE = INT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zap_m = UINT64_C(0xdeadc0de), .zcl = UINT64_C(0xdeadc0de), .zinternal_exponent = false, .zreserved_0 = UINT64_C(0xdeadc0de), .zreserved_1 = UINT64_C(0xdeadc0de), .zsd_perms = UINT64_C(0xdeadc0de), .zsealed = false, .ztag = false };
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz3112 = { .ztup0 = false, .ztup1 = zcbz3113 };
  return zcbz3112;
}



static void finish_sailgen_setCapAddr(void)
{
}

static void sailgen_capToString(sail_string *rop, struct zCapability);

lbits zghz3366;
lbits zghz3367;
lbits zghz3368;
lbits zghz3369;
struct zEncMetadata zghz3370;
struct zArchPerms zghz3371;
lbits zghz3372;
sail_int zghz3373;
sail_int zghz3374;
lbits zghz3375;
lbits zghz3376;
sail_int zghz3377;
lbits zghz3378;

static void startup_sailgen_capToString(void)
{
  CREATE(lbits)(&zghz3366);
  CREATE(lbits)(&zghz3367);
  CREATE(lbits)(&zghz3368);
  CREATE(lbits)(&zghz3369);


  CREATE(lbits)(&zghz3372);
  CREATE(sail_int)(&zghz3373);
  CREATE(sail_int)(&zghz3374);
  CREATE(lbits)(&zghz3375);
  CREATE(lbits)(&zghz3376);
  CREATE(sail_int)(&zghz3377);
  CREATE(lbits)(&zghz3378);
}

// struct tuple_(%string, %bool)
struct ztuple_z8z5stringzCz0z5boolz9 {
  sail_string ztup0;
  bool ztup1;
};

static void COPY(ztuple_z8z5stringzCz0z5boolz9)(struct ztuple_z8z5stringzCz0z5boolz9 *rop, const struct ztuple_z8z5stringzCz0z5boolz9 op) {
  COPY(sail_string)(&rop->ztup0, op.ztup0);
  rop->ztup1 = op.ztup1;
}

static void CREATE(ztuple_z8z5stringzCz0z5boolz9)(struct ztuple_z8z5stringzCz0z5boolz9 *op) {
  CREATE(sail_string)(&op->ztup0);
}

static void RECREATE(ztuple_z8z5stringzCz0z5boolz9)(struct ztuple_z8z5stringzCz0z5boolz9 *op) {
  RECREATE(sail_string)(&op->ztup0);
}

static void KILL(ztuple_z8z5stringzCz0z5boolz9)(struct ztuple_z8z5stringzCz0z5boolz9 *op) {
  KILL(sail_string)(&op->ztup0);
}

static bool EQUAL(ztuple_z8z5stringzCz0z5boolz9)(struct ztuple_z8z5stringzCz0z5boolz9 op1, struct ztuple_z8z5stringzCz0z5boolz9 op2) {
  return EQUAL(sail_string)(op1.ztup0, op2.ztup0) && EQUAL(bool)(op1.ztup1, op2.ztup1);
}

// struct tuple_(%string, %string, %bool)
struct ztuple_z8z5stringzCz0z5stringzCz0z5boolz9 {
  sail_string ztup0;
  sail_string ztup1;
  bool ztup2;
};

static void COPY(ztuple_z8z5stringzCz0z5stringzCz0z5boolz9)(struct ztuple_z8z5stringzCz0z5stringzCz0z5boolz9 *rop, const struct ztuple_z8z5stringzCz0z5stringzCz0z5boolz9 op) {
  COPY(sail_string)(&rop->ztup0, op.ztup0);
  COPY(sail_string)(&rop->ztup1, op.ztup1);
  rop->ztup2 = op.ztup2;
}

static void CREATE(ztuple_z8z5stringzCz0z5stringzCz0z5boolz9)(struct ztuple_z8z5stringzCz0z5stringzCz0z5boolz9 *op) {
  CREATE(sail_string)(&op->ztup0);
  CREATE(sail_string)(&op->ztup1);
}

static void RECREATE(ztuple_z8z5stringzCz0z5stringzCz0z5boolz9)(struct ztuple_z8z5stringzCz0z5stringzCz0z5boolz9 *op) {
  RECREATE(sail_string)(&op->ztup0);
  RECREATE(sail_string)(&op->ztup1);
}

static void KILL(ztuple_z8z5stringzCz0z5stringzCz0z5boolz9)(struct ztuple_z8z5stringzCz0z5stringzCz0z5boolz9 *op) {
  KILL(sail_string)(&op->ztup0);
  KILL(sail_string)(&op->ztup1);
}

static bool EQUAL(ztuple_z8z5stringzCz0z5stringzCz0z5boolz9)(struct ztuple_z8z5stringzCz0z5stringzCz0z5boolz9 op1, struct ztuple_z8z5stringzCz0z5stringzCz0z5boolz9 op2) {
  return EQUAL(sail_string)(op1.ztup0, op2.ztup0) && EQUAL(sail_string)(op1.ztup1, op2.ztup1) && EQUAL(bool)(op1.ztup2, op2.ztup2);
}

static void sailgen_capToString(sail_string *zcbz379, struct zCapability zcap)
{
  __label__ case_365, finish_match_364, end_function_376, end_block_exception_377, end_function_403;

  struct ztuple_z8z5stringzCz0z5stringzCz0z5boolz9 zgaz3339;
  CREATE(ztuple_z8z5stringzCz0z5stringzCz0z5boolz9)(&zgaz3339);
  {
    __label__ case_363, case_362, finish_match_361;

    struct zoptionzIz8bzCbz9zK zgaz3334;
    CREATE(zoptionzIz8bzCbz9zK)(&zgaz3334);
    sailgen_getCapBoundsBits(&zgaz3334, zcap);
    {
      if (zgaz3334.kind != Kind_zNonezIz8bzCbz9zK) goto case_363;
      struct ztuple_z8z5stringzCz0z5stringzCz0z5boolz9 zgsz3298;
      CREATE(ztuple_z8z5stringzCz0z5stringzCz0z5boolz9)(&zgsz3298);
      COPY(sail_string)(&((&zgsz3298)->ztup0), "0 (malformed)");
      COPY(sail_string)(&((&zgsz3298)->ztup1), "0 (malformed)");
      zgsz3298.ztup2 = true;
      COPY(ztuple_z8z5stringzCz0z5stringzCz0z5boolz9)(&zgaz3339, zgsz3298);
      KILL(ztuple_z8z5stringzCz0z5stringzCz0z5boolz9)(&zgsz3298);
      goto finish_match_361;
    }
  case_363: ;
    {
      /* complete */
      uint64_t zbase;
      zbase = CONVERT_OF(fbits, lbits)(zgaz3334.variants.zSomezIz8bzCbz9zK.ztup0, true);
      RECREATE(lbits)(&zghz3375);
      COPY(lbits)(&zghz3375, zgaz3334.variants.zSomezIz8bzCbz9zK.ztup1);
      sail_string zgaz3337;
      CREATE(sail_string)(&zgaz3337);
      {
        RECREATE(lbits)(&zghz3378);
        CONVERT_OF(lbits, fbits)(&zghz3378, zbase, UINT64_C(64) , true);
        string_of_lbits(&zgaz3337, zghz3378);
      }
      sail_string zgaz3338;
      CREATE(sail_string)(&zgaz3338);
      {
        RECREATE(lbits)(&zghz3376);
        {
          int64_t zgaz3335;
          zgaz3335 = (zcap_len_width + INT64_C(3));
          {
            RECREATE(sail_int)(&zghz3377);
            CONVERT_OF(sail_int, mach_int)(&zghz3377, zgaz3335);
            sailgen_zzero_extend(&zghz3376, zghz3377, zghz3375);
          }
        }
        string_of_lbits(&zgaz3338, zghz3376);
      }
      struct ztuple_z8z5stringzCz0z5stringzCz0z5boolz9 zgsz3296;
      CREATE(ztuple_z8z5stringzCz0z5stringzCz0z5boolz9)(&zgsz3296);
      COPY(sail_string)(&((&zgsz3296)->ztup0), zgaz3337);
      COPY(sail_string)(&((&zgsz3296)->ztup1), zgaz3338);
      zgsz3296.ztup2 = false;
      COPY(ztuple_z8z5stringzCz0z5stringzCz0z5boolz9)(&zgaz3339, zgsz3296);
      KILL(ztuple_z8z5stringzCz0z5stringzCz0z5boolz9)(&zgsz3296);
      KILL(sail_string)(&zgaz3338);
      KILL(sail_string)(&zgaz3337);
      goto finish_match_361;
    }
  case_362: ;
  finish_match_361: ;
    KILL(zoptionzIz8bzCbz9zK)(&zgaz3334);
  }
  sail_string zgsz3300;
  CREATE(sail_string)(&zgsz3300);
  {
    __label__ case_370, finish_match_369;

    sail_string zbase_str;
    CREATE(sail_string)(&zbase_str);
    COPY(sail_string)(&zbase_str, zgaz3339.ztup0);
    sail_string ztop_str;
    CREATE(sail_string)(&ztop_str);
    COPY(sail_string)(&ztop_str, zgaz3339.ztup1);
    bool zbounds_malformed;
    zbounds_malformed = zgaz3339.ztup2;
    sail_string zlen_str;
    CREATE(sail_string)(&zlen_str);
    {
      RECREATE(lbits)(&zghz3372);
      {
        int64_t zgaz3403;
        zgaz3403 = (zcap_len_width + INT64_C(3));
        RECREATE(sail_int)(&zghz3373);
        sailgen_getCapLength(&zghz3373, zcap);
        {
          RECREATE(sail_int)(&zghz3374);
          CONVERT_OF(sail_int, mach_int)(&zghz3374, zgaz3403);
          sailgen_to_bits(&zghz3372, zghz3374, zghz3373);
        }
      }
      string_of_lbits(&zlen_str, zghz3372);
    }
    struct ztuple_z8z5stringzCz0z5boolz9 zgaz3364;
    CREATE(ztuple_z8z5stringzCz0z5boolz9)(&zgaz3364);
    {
      __label__ case_368, case_367, finish_match_366;

      struct zoptionzIRArchPermszK zgaz3340;
      CREATE(zoptionzIRArchPermszK)(&zgaz3340);
      sailgen_getArchPerms(&zgaz3340, zcap);
      {
        if (zgaz3340.kind != Kind_zNonezIRArchPermszK) goto case_368;
        struct ztuple_z8z5stringzCz0z5boolz9 zgsz3304;
        CREATE(ztuple_z8z5stringzCz0z5boolz9)(&zgsz3304);
        COPY(sail_string)(&((&zgsz3304)->ztup0), "(malformed)");
        zgsz3304.ztup1 = true;
        COPY(ztuple_z8z5stringzCz0z5boolz9)(&zgaz3364, zgsz3304);
        KILL(ztuple_z8z5stringzCz0z5boolz9)(&zgsz3304);
        goto finish_match_366;
      }
    case_368: ;
      {
        /* complete */
        struct zArchPerms zghz3371;
        zghz3371 = zgaz3340.variants.zSomezIRArchPermszK;
        sail_string zgaz3363;
        CREATE(sail_string)(&zgaz3363);
        {
          sail_string zgaz3361;
          CREATE(sail_string)(&zgaz3361);
          {
            bool zgaz3341;
            zgaz3341 = zghz3371.zR;
            if (zgaz3341) {  COPY(sail_string)(&zgaz3361, "R");  } else {  COPY(sail_string)(&zgaz3361, "");  }
          }
          sail_string zgaz3362;
          CREATE(sail_string)(&zgaz3362);
          {
            sail_string zgaz3359;
            CREATE(sail_string)(&zgaz3359);
            {
              bool zgaz3342;
              zgaz3342 = zghz3371.zW;
              if (zgaz3342) {  COPY(sail_string)(&zgaz3359, "W");  } else {  COPY(sail_string)(&zgaz3359, "");  }
            }
            sail_string zgaz3360;
            CREATE(sail_string)(&zgaz3360);
            {
              sail_string zgaz3357;
              CREATE(sail_string)(&zgaz3357);
              {
                bool zgaz3343;
                zgaz3343 = zghz3371.zC;
                if (zgaz3343) {  COPY(sail_string)(&zgaz3357, "C");  } else {  COPY(sail_string)(&zgaz3357, "");  }
              }
              sail_string zgaz3358;
              CREATE(sail_string)(&zgaz3358);
              {
                sail_string zgaz3355;
                CREATE(sail_string)(&zgaz3355);
                {
                  bool zgaz3344;
                  zgaz3344 = zghz3371.zX;
                  if (zgaz3344) {  COPY(sail_string)(&zgaz3355, "X");  } else {  COPY(sail_string)(&zgaz3355, "");  }
                }
                sail_string zgaz3356;
                CREATE(sail_string)(&zgaz3356);
                {
                  sail_string zgaz3353;
                  CREATE(sail_string)(&zgaz3353);
                  {
                    bool zgaz3345;
                    zgaz3345 = zghz3371.zLM;
                    if (zgaz3345) {  COPY(sail_string)(&zgaz3353, "Lm");  } else {  COPY(sail_string)(&zgaz3353, "");  }
                  }
                  sail_string zgaz3354;
                  CREATE(sail_string)(&zgaz3354);
                  {
                    sail_string zgaz3351;
                    CREATE(sail_string)(&zgaz3351);
                    {
                      bool zgaz3346;
                      zgaz3346 = zghz3371.zASR;
                      if (zgaz3346) {  COPY(sail_string)(&zgaz3351, "Asr");  } else {
                        COPY(sail_string)(&zgaz3351, "");
                      }
                    }
                    sail_string zgaz3352;
                    CREATE(sail_string)(&zgaz3352);
                    {
                      sail_string zgaz3349;
                      CREATE(sail_string)(&zgaz3349);
                      {
                        bool zgaz3347;
                        zgaz3347 = zghz3371.zSL;
                        if (zgaz3347) {  COPY(sail_string)(&zgaz3349, "Sl");  } else {
                          COPY(sail_string)(&zgaz3349, "");
                        }
                      }
                      sail_string zgaz3350;
                      CREATE(sail_string)(&zgaz3350);
                      {
                        bool zgaz3348;
                        zgaz3348 = zghz3371.zEL;
                        if (zgaz3348) {  COPY(sail_string)(&zgaz3350, "El");  } else {
                          COPY(sail_string)(&zgaz3350, "");
                        }
                      }
                      concat_str(&zgaz3352, zgaz3349, zgaz3350);
                      KILL(sail_string)(&zgaz3350);
                      KILL(sail_string)(&zgaz3349);
                    }
                    concat_str(&zgaz3354, zgaz3351, zgaz3352);
                    KILL(sail_string)(&zgaz3352);
                    KILL(sail_string)(&zgaz3351);
                  }
                  concat_str(&zgaz3356, zgaz3353, zgaz3354);
                  KILL(sail_string)(&zgaz3354);
                  KILL(sail_string)(&zgaz3353);
                }
                concat_str(&zgaz3358, zgaz3355, zgaz3356);
                KILL(sail_string)(&zgaz3356);
                KILL(sail_string)(&zgaz3355);
              }
              concat_str(&zgaz3360, zgaz3357, zgaz3358);
              KILL(sail_string)(&zgaz3358);
              KILL(sail_string)(&zgaz3357);
            }
            concat_str(&zgaz3362, zgaz3359, zgaz3360);
            KILL(sail_string)(&zgaz3360);
            KILL(sail_string)(&zgaz3359);
          }
          concat_str(&zgaz3363, zgaz3361, zgaz3362);
          KILL(sail_string)(&zgaz3362);
          KILL(sail_string)(&zgaz3361);
        }
        struct ztuple_z8z5stringzCz0z5boolz9 zgsz3302;
        CREATE(ztuple_z8z5stringzCz0z5boolz9)(&zgsz3302);
        COPY(sail_string)(&((&zgsz3302)->ztup0), zgaz3363);
        zgsz3302.ztup1 = false;
        COPY(ztuple_z8z5stringzCz0z5boolz9)(&zgaz3364, zgsz3302);
        KILL(ztuple_z8z5stringzCz0z5boolz9)(&zgsz3302);
        KILL(sail_string)(&zgaz3363);
        goto finish_match_366;
      }
    case_367: ;
    finish_match_366: ;
      KILL(zoptionzIRArchPermszK)(&zgaz3340);
    }
    sail_string zgsz3306;
    CREATE(sail_string)(&zgsz3306);
    {
      __label__ case_372, finish_match_371;

      sail_string zarch_perms_str;
      CREATE(sail_string)(&zarch_perms_str);
      COPY(sail_string)(&zarch_perms_str, zgaz3364.ztup0);
      bool zperms_malformed;
      zperms_malformed = zgaz3364.ztup1;
      struct ztuple_z8z5stringzCz0z5boolz9 zgaz3367;
      CREATE(ztuple_z8z5stringzCz0z5boolz9)(&zgaz3367);
      {
        bool zgaz3366;
        {
          bool zgaz3365;
          zgaz3365 = sailgen_capReservedValid(zcap);
          zgaz3366 = sailgen_not(zgaz3365);
        }
        if (zgaz3366) {
          struct ztuple_z8z5stringzCz0z5boolz9 zgsz3308;
          CREATE(ztuple_z8z5stringzCz0z5boolz9)(&zgsz3308);
          COPY(sail_string)(&((&zgsz3308)->ztup0), " (has non-zero reserved bits)");
          zgsz3308.ztup1 = true;
          COPY(ztuple_z8z5stringzCz0z5boolz9)(&zgaz3367, zgsz3308);
          KILL(ztuple_z8z5stringzCz0z5boolz9)(&zgsz3308);
        } else {
          struct ztuple_z8z5stringzCz0z5boolz9 zgsz3307;
          CREATE(ztuple_z8z5stringzCz0z5boolz9)(&zgsz3307);
          COPY(sail_string)(&((&zgsz3307)->ztup0), "");
          zgsz3307.ztup1 = false;
          COPY(ztuple_z8z5stringzCz0z5boolz9)(&zgaz3367, zgsz3307);
          KILL(ztuple_z8z5stringzCz0z5boolz9)(&zgsz3307);
        }
      }
      sail_string zgsz3309;
      CREATE(sail_string)(&zgsz3309);
      {
        sail_string zres_str;
        CREATE(sail_string)(&zres_str);
        COPY(sail_string)(&zres_str, zgaz3367.ztup0);
        bool zreserved_malformed;
        zreserved_malformed = zgaz3367.ztup1;
        sail_string zraw_str;
        CREATE(sail_string)(&zraw_str);
        {
          bool zgaz3399;
          {
            if (zbounds_malformed) {  zgaz3399 = true;  } else {
              bool zgsz3310;
              if (zperms_malformed) {  zgsz3310 = true;  } else {  zgsz3310 = zreserved_malformed;  }
              zgaz3399 = zgsz3310;
            }
          }
          if (zgaz3399) {
            sail_string zgaz3402;
            CREATE(sail_string)(&zgaz3402);
            {
              uint64_t zgaz3401;
              {
                struct zEncMetadata zghz3370;
                zghz3370 = sailgen_capToMetadataBits(zcap);
                zgaz3401 = zghz3370.zbits;
              }
              {
                RECREATE(lbits)(&zghz3369);
                CONVERT_OF(lbits, fbits)(&zghz3369, zgaz3401, UINT64_C(64) , true);
                string_of_lbits(&zgaz3402, zghz3369);
              }
            }
            concat_str(&zraw_str, " raw:", zgaz3402);
            KILL(sail_string)(&zgaz3402);
          } else {  COPY(sail_string)(&zraw_str, "");  }
        }
        sail_string zcheri_mode;
        CREATE(sail_string)(&zcheri_mode);
        {
          __label__ case_375, case_374, finish_match_373;

          enum zExecutionMode zgaz3398;
          zgaz3398 = sailgen_getCapMode(zcap);
          {
            if ((zCapPtrMode != zgaz3398)) goto case_375;
            COPY(sail_string)(&zcheri_mode, "CapPtrMode");
            goto finish_match_373;
          }
        case_375: ;
          {
            /* complete */
            COPY(sail_string)(&zcheri_mode, "Integer");
            goto finish_match_373;
          }
        case_374: ;
        finish_match_373: ;
        }
        sail_string zgaz3397;
        CREATE(sail_string)(&zgaz3397);
        {
          sail_string zgaz3395;
          CREATE(sail_string)(&zgaz3395);
          {
            bool zgaz3368;
            zgaz3368 = zcap.ztag;
            if (zgaz3368) {  COPY(sail_string)(&zgaz3395, "1");  } else {  COPY(sail_string)(&zgaz3395, "0");  }
          }
          sail_string zgaz3396;
          CREATE(sail_string)(&zgaz3396);
          {
            sail_string zgaz3394;
            CREATE(sail_string)(&zgaz3394);
            {
              sail_string zgaz3392;
              CREATE(sail_string)(&zgaz3392);
              {
                uint64_t zgaz3369;
                zgaz3369 = zcap.zaddress;
                {
                  RECREATE(lbits)(&zghz3368);
                  CONVERT_OF(lbits, fbits)(&zghz3368, zgaz3369, UINT64_C(64) , true);
                  string_of_lbits(&zgaz3392, zghz3368);
                }
              }
              sail_string zgaz3393;
              CREATE(sail_string)(&zgaz3393);
              {
                sail_string zgaz3391;
                CREATE(sail_string)(&zgaz3391);
                {
                  sail_string zgaz3390;
                  CREATE(sail_string)(&zgaz3390);
                  {
                    sail_string zgaz3389;
                    CREATE(sail_string)(&zgaz3389);
                    {
                      sail_string zgaz3388;
                      CREATE(sail_string)(&zgaz3388);
                      {
                        sail_string zgaz3387;
                        CREATE(sail_string)(&zgaz3387);
                        {
                          sail_string zgaz3386;
                          CREATE(sail_string)(&zgaz3386);
                          {
                            sail_string zgaz3385;
                            CREATE(sail_string)(&zgaz3385);
                            {
                              sail_string zgaz3383;
                              CREATE(sail_string)(&zgaz3383);
                              {
                                bool zgaz3370;
                                zgaz3370 = zcap.zsealed;
                                if (zgaz3370) {  COPY(sail_string)(&zgaz3383, "1");  } else {
                                  COPY(sail_string)(&zgaz3383, "0");
                                }
                              }
                              sail_string zgaz3384;
                              CREATE(sail_string)(&zgaz3384);
                              {
                                sail_string zgaz3382;
                                CREATE(sail_string)(&zgaz3382);
                                {
                                  sail_string zgaz3381;
                                  CREATE(sail_string)(&zgaz3381);
                                  {
                                    sail_string zgaz3380;
                                    CREATE(sail_string)(&zgaz3380);
                                    {
                                      sail_string zgaz3378;
                                      CREATE(sail_string)(&zgaz3378);
                                      {
                                        uint64_t zgaz3371;
                                        zgaz3371 = zcap.zsd_perms;
                                        {
                                          RECREATE(lbits)(&zghz3367);
                                          CONVERT_OF(lbits, fbits)(&zghz3367, zgaz3371, UINT64_C(4) , true);
                                          string_of_lbits(&zgaz3378, zghz3367);
                                        }
                                      }
                                      sail_string zgaz3379;
                                      CREATE(sail_string)(&zgaz3379);
                                      {
                                        sail_string zgaz3377;
                                        CREATE(sail_string)(&zgaz3377);
                                        {
                                          sail_string zgaz3375;
                                          CREATE(sail_string)(&zgaz3375);
                                          {
                                            uint64_t zgaz3372;
                                            zgaz3372 = zcap.zcl;
                                            {
                                              RECREATE(lbits)(&zghz3366);
                                              CONVERT_OF(lbits, fbits)(&zghz3366, zgaz3372, UINT64_C(1) , true);
                                              string_of_lbits(&zgaz3375, zghz3366);
                                            }
                                          }
                                          sail_string zgaz3376;
                                          CREATE(sail_string)(&zgaz3376);
                                          {
                                            sail_string zgaz3374;
                                            CREATE(sail_string)(&zgaz3374);
                                            {
                                              sail_string zgaz3373;
                                              CREATE(sail_string)(&zgaz3373);
                                              concat_str(&zgaz3373, zraw_str, zres_str);
                                              concat_str(&zgaz3374, zcheri_mode, zgaz3373);
                                              KILL(sail_string)(&zgaz3373);
                                            }
                                            concat_str(&zgaz3376, " mode:", zgaz3374);
                                            KILL(sail_string)(&zgaz3374);
                                          }
                                          concat_str(&zgaz3377, zgaz3375, zgaz3376);
                                          KILL(sail_string)(&zgaz3376);
                                          KILL(sail_string)(&zgaz3375);
                                        }
                                        concat_str(&zgaz3379, " level:", zgaz3377);
                                        KILL(sail_string)(&zgaz3377);
                                      }
                                      concat_str(&zgaz3380, zgaz3378, zgaz3379);
                                      KILL(sail_string)(&zgaz3379);
                                      KILL(sail_string)(&zgaz3378);
                                    }
                                    concat_str(&zgaz3381, " sdperms:", zgaz3380);
                                    KILL(sail_string)(&zgaz3380);
                                  }
                                  concat_str(&zgaz3382, zarch_perms_str, zgaz3381);
                                  KILL(sail_string)(&zgaz3381);
                                }
                                concat_str(&zgaz3384, " perms:", zgaz3382);
                                KILL(sail_string)(&zgaz3382);
                              }
                              concat_str(&zgaz3385, zgaz3383, zgaz3384);
                              KILL(sail_string)(&zgaz3384);
                              KILL(sail_string)(&zgaz3383);
                            }
                            concat_str(&zgaz3386, " sealed:", zgaz3385);
                            KILL(sail_string)(&zgaz3385);
                          }
                          concat_str(&zgaz3387, zlen_str, zgaz3386);
                          KILL(sail_string)(&zgaz3386);
                        }
                        concat_str(&zgaz3388, " length:", zgaz3387);
                        KILL(sail_string)(&zgaz3387);
                      }
                      concat_str(&zgaz3389, ztop_str, zgaz3388);
                      KILL(sail_string)(&zgaz3388);
                    }
                    concat_str(&zgaz3390, " top:", zgaz3389);
                    KILL(sail_string)(&zgaz3389);
                  }
                  concat_str(&zgaz3391, zbase_str, zgaz3390);
                  KILL(sail_string)(&zgaz3390);
                }
                concat_str(&zgaz3393, " base:", zgaz3391);
                KILL(sail_string)(&zgaz3391);
              }
              concat_str(&zgaz3394, zgaz3392, zgaz3393);
              KILL(sail_string)(&zgaz3393);
              KILL(sail_string)(&zgaz3392);
            }
            concat_str(&zgaz3396, " address:", zgaz3394);
            KILL(sail_string)(&zgaz3394);
          }
          concat_str(&zgaz3397, zgaz3395, zgaz3396);
          KILL(sail_string)(&zgaz3396);
          KILL(sail_string)(&zgaz3395);
        }
        concat_str(&zgsz3309, "t:", zgaz3397);
        KILL(sail_string)(&zgaz3397);
        KILL(sail_string)(&zcheri_mode);
        KILL(sail_string)(&zraw_str);
        KILL(sail_string)(&zres_str);
        goto finish_match_371;
      }
    case_372: ;
      sail_match_failure("capToString");
    finish_match_371: ;
      COPY(sail_string)(&zgsz3306, zgsz3309);
      KILL(sail_string)(&zgsz3309);
      KILL(ztuple_z8z5stringzCz0z5boolz9)(&zgaz3367);
      KILL(sail_string)(&zarch_perms_str);
      goto finish_match_369;
    }
  case_370: ;
    sail_match_failure("capToString");
  finish_match_369: ;
    COPY(sail_string)(&zgsz3300, zgsz3306);
    KILL(sail_string)(&zgsz3306);
    KILL(ztuple_z8z5stringzCz0z5boolz9)(&zgaz3364);
    KILL(sail_string)(&zlen_str);
    KILL(sail_string)(&ztop_str);
    KILL(sail_string)(&zbase_str);
    goto finish_match_364;
  }
case_365: ;
  sail_match_failure("capToString");
finish_match_364: ;
  COPY(sail_string)((*(&zcbz379)), zgsz3300);
  KILL(sail_string)(&zgsz3300);
  KILL(ztuple_z8z5stringzCz0z5stringzCz0z5boolz9)(&zgaz3339);
end_function_376: ;
  goto end_function_403;
end_block_exception_377: ;
  goto end_function_403;
end_function_403: ;
}



static void finish_sailgen_capToString(void)
{
  KILL(lbits)(&zghz3378);
  KILL(sail_int)(&zghz3377);
  KILL(lbits)(&zghz3376);
  KILL(lbits)(&zghz3375);
  KILL(sail_int)(&zghz3374);
  KILL(sail_int)(&zghz3373);
  KILL(lbits)(&zghz3372);
  KILL(lbits)(&zghz3369);
  KILL(lbits)(&zghz3368);
  KILL(lbits)(&zghz3367);
  KILL(lbits)(&zghz3366);
}

static uint64_t sailgen_getRepresentableAlignmentMask(uint64_t);

struct zCapability zghz3379;
sail_int zghz3380;
lbits zghz3381;
lbits zghz3382;
sail_int zghz3383;
lbits zghz3384;
lbits zghz3385;
lbits zghz3386;
lbits zghz3387;

static void startup_sailgen_getRepresentableAlignmentMask(void)
{

  CREATE(sail_int)(&zghz3380);
  CREATE(lbits)(&zghz3381);
  CREATE(lbits)(&zghz3382);
  CREATE(sail_int)(&zghz3383);
  CREATE(lbits)(&zghz3384);
  CREATE(lbits)(&zghz3385);
  CREATE(lbits)(&zghz3386);
  CREATE(lbits)(&zghz3387);
}

static uint64_t sailgen_getRepresentableAlignmentMask(uint64_t zlen)
{
  __label__ case_380, finish_match_379, end_function_381, end_block_exception_382;

  uint64_t zcbz380;
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3407;
  {
    RECREATE(lbits)(&zghz3385);
    {
      RECREATE(lbits)(&zghz3386);
      CONVERT_OF(lbits, fbits)(&zghz3386, UINT64_C(0b0), UINT64_C(1) , true);
      RECREATE(lbits)(&zghz3387);
      CONVERT_OF(lbits, fbits)(&zghz3387, zlen, UINT64_C(64) , true);
      append(&zghz3385, zghz3386, zghz3387);
    }
    zgaz3407 = sailgen_setCapBounds(zinfinite_cap, UINT64_C(0x0000000000000000), zghz3385);
  }
  {
    bool z_exact;
    z_exact = zgaz3407.ztup0;
    struct zCapability zghz3379;
    zghz3379 = zgaz3407.ztup1;
    bool zgaz3408;
    zgaz3408 = zghz3379.zinternal_exponent;
    if (zgaz3408) {
      int64_t ze;
      ze = zghz3379.zE;
      {
        bool zgaz3409;
        zgaz3409 = (ze >= INT64_C(0));
        unit zgsz3320;
        zgsz3320 = sail_assert(zgaz3409, "src/cheri_cap_common.sail:688.17-688.18");
      }
      uint64_t zgaz3412;
      {
        sbits zgaz3411;
        {
          int64_t zgaz3410;
          zgaz3410 = (ze + zcap_xe_width);
          {
            RECREATE(sail_int)(&zghz3383);
            CONVERT_OF(sail_int, mach_int)(&zghz3383, zgaz3410);
            RECREATE(lbits)(&zghz3384);
            sailgen_ones(&zghz3384, zghz3383);
            zgaz3411 = CONVERT_OF(sbits, lbits)(zghz3384, true);
          }
        }
        {
          RECREATE(sail_int)(&zghz3380);
          CONVERT_OF(sail_int, mach_int)(&zghz3380, INT64_C(64));
          RECREATE(lbits)(&zghz3381);
          CONVERT_OF(lbits, sbits)(&zghz3381, zgaz3411, true);
          RECREATE(lbits)(&zghz3382);
          sailgen_zzero_extend(&zghz3382, zghz3380, zghz3381);
          zgaz3412 = CONVERT_OF(fbits, lbits)(zghz3382, true);
        }
      }
      zcbz380 = (~(zgaz3412) & UINT64_C(0xFFFFFFFFFFFFFFFF));
    } else {  zcbz380 = UINT64_C(0xFFFFFFFFFFFFFFFF);  }
    goto finish_match_379;
  }
case_380: ;
  sail_match_failure("getRepresentableAlignmentMask");
finish_match_379: ;
end_function_381: ;
  return zcbz380;
end_block_exception_382: ;

  return UINT64_C(0xdeadc0de);
}



static void finish_sailgen_getRepresentableAlignmentMask(void)
{
  KILL(lbits)(&zghz3387);
  KILL(lbits)(&zghz3386);
  KILL(lbits)(&zghz3385);
  KILL(lbits)(&zghz3384);
  KILL(sail_int)(&zghz3383);
  KILL(lbits)(&zghz3382);
  KILL(lbits)(&zghz3381);
  KILL(sail_int)(&zghz3380);
}

static uint64_t sailgen_getRepresentableLength(uint64_t);

static uint64_t sailgen_getRepresentableLength(uint64_t zlen)
{
  __label__ end_function_384, end_block_exception_385;

  uint64_t zcbz381;
  uint64_t zm;
  zm = sailgen_getRepresentableAlignmentMask(zlen);
  uint64_t zgaz3414;
  {
    uint64_t zgaz3413;
    zgaz3413 = (~(zm) & UINT64_C(0xFFFFFFFFFFFFFFFF));
    zgaz3414 = ((zlen + zgaz3413) & UINT64_C(0xFFFFFFFFFFFFFFFF));
  }
  zcbz381 = (zgaz3414 & zm);
end_function_384: ;
  return zcbz381;
end_block_exception_385: ;

  return UINT64_C(0xdeadc0de);
}

static struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 sailgen_doCSetBounds(struct zCapability, lbits);

lbits zghz3388;
struct zCapability zghz3389;
struct zCapability zghz3390;
struct zCapability zghz3391;
sail_int zghz3392;
lbits zghz3393;
sail_int zghz3394;
lbits zghz3395;

static void startup_sailgen_doCSetBounds(void)
{
  CREATE(lbits)(&zghz3388);



  CREATE(sail_int)(&zghz3392);
  CREATE(lbits)(&zghz3393);
  CREATE(sail_int)(&zghz3394);
  CREATE(lbits)(&zghz3395);
}

static struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 sailgen_doCSetBounds(struct zCapability zinput, lbits zreq_len)
{
  __label__ case_388, finish_match_387, end_function_391, end_block_exception_392;

  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz382;
  uint64_t znewBase;
  znewBase = zinput.zaddress;
  RECREATE(lbits)(&zghz3388);
  {
    RECREATE(lbits)(&zghz3393);
    {
      RECREATE(sail_int)(&zghz3394);
      CONVERT_OF(sail_int, mach_int)(&zghz3394, INT64_C(65));
      RECREATE(lbits)(&zghz3395);
      CONVERT_OF(lbits, fbits)(&zghz3395, znewBase, UINT64_C(64) , true);
      sailgen_zzero_extend(&zghz3393, zghz3394, zghz3395);
    }
    add_bits(&zghz3388, zghz3393, zreq_len);
  }
  bool zinBounds;
  {
    RECREATE(sail_int)(&zghz3392);
    sail_unsigned(&zghz3392, zreq_len);
    zinBounds = sailgen_inCapBoundsNoWrap(zinput, znewBase, zghz3392);
  }
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3415;
  zgaz3415 = sailgen_setCapBounds(zinput, znewBase, zghz3388);
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgsz3321;
  {
    __label__ case_390, finish_match_389;

    bool zexact;
    zexact = zgaz3415.ztup0;
    struct zCapability zghz3389;
    zghz3389 = zgaz3415.ztup1;
    bool zcond;
    {
      bool zgaz3422;
      {
        bool zgaz3418;
        {    if (zinBounds) {  zgaz3418 = zexact;  } else {  zgaz3418 = false;  }
        }
        zgaz3422 = sailgen_not(zgaz3418);
      }
      bool zgsz3325;
      if (zgaz3422) {  zgsz3325 = true;  } else {
        bool zgaz3421;
        zgaz3421 = sailgen_boundsMalformed(zghz3389);
        bool zgsz3324;
        if (zgaz3421) {  zgsz3324 = true;  } else {
          bool zgaz3420;
          {
            bool zgaz3419;
            zgaz3419 = sailgen_capReservedValid(zghz3389);
            zgaz3420 = sailgen_not(zgaz3419);
          }
          bool zgsz3323;
          if (zgaz3420) {  zgsz3323 = true;  } else {  zgsz3323 = sailgen_capIsSealed(zghz3389);  }
          zgsz3324 = zgsz3323;
        }
        zgsz3325 = zgsz3324;
      }
      zcond = zgsz3325;
    }
    struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3416;
    zgaz3416 = sailgen_setCapBounds(zinput, znewBase, zghz3388);
    struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgsz3326;
    {
      bool zexactshadowz330;
      zexactshadowz330 = zgaz3416.ztup0;
      struct zCapability zghz3390;
      zghz3390 = zgaz3416.ztup1;
      struct zCapability zghz3391;
      zghz3391 = sailgen_clearTagIf(zghz3390, zcond);
      zgsz3326.ztup0 = zexactshadowz330;
      zgsz3326.ztup1 = zghz3391;
      goto finish_match_389;
    }
  case_390: ;
    sail_match_failure("doCSetBounds");
  finish_match_389: ;
    zgsz3321 = zgsz3326;
    goto finish_match_387;
  }
case_388: ;
  sail_match_failure("doCSetBounds");
finish_match_387: ;
  zcbz382 = zgsz3321;
end_function_391: ;
  return zcbz382;
end_block_exception_392: ;
  struct zCapability zcbz3115 = { .zB = UINT64_C(0xdeadc0de), .zE = INT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zap_m = UINT64_C(0xdeadc0de), .zcl = UINT64_C(0xdeadc0de), .zinternal_exponent = false, .zreserved_0 = UINT64_C(0xdeadc0de), .zreserved_1 = UINT64_C(0xdeadc0de), .zsd_perms = UINT64_C(0xdeadc0de), .zsealed = false, .ztag = false };
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz3114 = { .ztup0 = false, .ztup1 = zcbz3115 };
  return zcbz3114;
}



static void finish_sailgen_doCSetBounds(void)
{
  KILL(lbits)(&zghz3395);
  KILL(sail_int)(&zghz3394);
  KILL(lbits)(&zghz3393);
  KILL(sail_int)(&zghz3392);
  KILL(lbits)(&zghz3388);
}

// register TestCap
static struct zCapability zTestCap;

// register TestAddr
static uint64_t zTestAddr;

// register TestLen
static lbits zTestLen;

static unit sailgen_main(unit);

struct zCapability zghz3396;
struct zCapability zghz3397;
struct zCapability zghz3398;
struct zCapability zghz3399;
struct zArchPerms zghz3400;
struct zEncMetadata zghz3401;
lbits zghz3402;
lbits zghz3403;
lbits zghz3404;

static void startup_sailgen_main(void)
{






  CREATE(lbits)(&zghz3402);
  CREATE(lbits)(&zghz3403);
  CREATE(lbits)(&zghz3404);
}

static unit sailgen_main(unit zgsz3330)
{
  __label__ case_395, finish_match_394, end_function_398, end_block_exception_399;

  unit zcbz383;
  {
    zTestCap = znull_cap;
    unit zgsz3331;
    zgsz3331 = UNIT;
  }
  struct zCapability zghz3396;
  {
    bool zgaz3431;
    zgaz3431 = zTestCap.ztag;
    RECREATE(lbits)(&zghz3402);
    {
      RECREATE(lbits)(&zghz3403);
      CONVERT_OF(lbits, fbits)(&zghz3403, zTestAddr, UINT64_C(64) , true);
      RECREATE(lbits)(&zghz3404);
      CONVERT_OF(lbits, fbits)(&zghz3404, zTestAddr, UINT64_C(64) , true);
      append(&zghz3402, zghz3403, zghz3404);
    }
    zghz3396 = sailgen_bitsToCap(zgaz3431, zghz3402);
  }
  uint64_t zcap_bits;
  {
    struct zEncMetadata zghz3401;
    zghz3401 = sailgen_capToMetadataBits(zghz3396);
    zcap_bits = zghz3401.zbits;
  }
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3425;
  zgaz3425 = sailgen_doCSetBounds(zTestCap, zTestLen);
  unit zgsz3332;
  {
    __label__ case_397, finish_match_396;

    bool zexact;
    zexact = zgaz3425.ztup0;
    struct zCapability zghz3397;
    zghz3397 = zgaz3425.ztup1;
    struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3426;
    zgaz3426 = sailgen_setCapAddr(zTestCap, zTestAddr);
    {
      bool zrepresentable;
      zrepresentable = zgaz3426.ztup0;
      struct zCapability zghz3398;
      zghz3398 = zgaz3426.ztup1;
      struct zoptionzIz8izCiz9zK zbounds;
      CREATE(zoptionzIz8izCiz9zK)(&zbounds);
      sailgen_getCapBounds(&zbounds, zTestCap);
      enum zExecutionMode zmode;
      zmode = sailgen_getCapMode(zTestCap);
      sail_string zcap_str;
      CREATE(sail_string)(&zcap_str);
      sailgen_capToString(&zcap_str, zTestCap);
      struct zCapability zghz3399;
      {
        struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgaz3429;
        {
          struct zArchPerms zghz3400;
          zghz3400 = sailgen_getArchPermsLegalizzed(zTestCap);
          enum zExecutionMode zgaz3428;
          zgaz3428 = sailgen_getCapMode(zTestCap);
          zgaz3429.ztup0 = zghz3400;
          zgaz3429.ztup1 = zgaz3428;
        }
        zghz3399 = sailgen_setCapPermsAndMode(zTestCap, zgaz3429);
      }
      struct zoptionzIz8RArchPermszCEExecutionModez5z9zK zperms_mode;
      CREATE(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zperms_mode);
      sailgen_getCapPermsAndMode(&zperms_mode, zTestCap);
      uint64_t zlen;
      zlen = sailgen_getRepresentableLength(zTestAddr);
      uint64_t zmask;
      zmask = sailgen_getRepresentableAlignmentMask(zTestAddr);
      zgsz3332 = UNIT;
      KILL(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zperms_mode);
      KILL(sail_string)(&zcap_str);
      KILL(zoptionzIz8izCiz9zK)(&zbounds);
      goto finish_match_396;
    }
  case_397: ;
    sail_match_failure("main");
  finish_match_396: ;
    goto finish_match_394;
  }
case_395: ;
  sail_match_failure("main");
finish_match_394: ;
  zcbz383 = zgsz3332;
end_function_398: ;
  return zcbz383;
end_block_exception_399: ;

  return UNIT;
}



static void finish_sailgen_main(void)
{
  KILL(lbits)(&zghz3404);
  KILL(lbits)(&zghz3403);
  KILL(lbits)(&zghz3402);
}

static unit sailgen_initializze_registers(unit);

lbits zghz3405;
struct zCapability zghz3406;
sail_int zghz3407;

static void startup_sailgen_initializze_registers(void)
{
  CREATE(lbits)(&zghz3405);

  CREATE(sail_int)(&zghz3407);
}

static unit sailgen_initializze_registers(unit zgsz3337)
{
  __label__ end_function_401, end_block_exception_402;

  unit zcbz384;
  {
    struct zCapability zghz3406;
    zghz3406.zB = UINT64_C(0b00000000000000);
    RECREATE(sail_int)(&zghz3407);
    CONVERT_OF(sail_int, mach_int)(&zghz3407, INT64_C(-11));
    zghz3406.zE = CONVERT_OF(mach_int, sail_int)(zghz3407);
    zghz3406.zT = UINT64_C(0b00000000000000);
    zghz3406.zaddress = UINT64_C(0x0000000000000000);
    zghz3406.zap_m = UINT64_C(0b000000000);
    zghz3406.zcl = UINT64_C(0b0);
    zghz3406.zinternal_exponent = false;
    zghz3406.zreserved_0 = UINT64_C(0b000000000000000);
    zghz3406.zreserved_1 = UINT64_C(0b0000000);
    zghz3406.zsd_perms = UINT64_C(0x0);
    zghz3406.zsealed = false;
    zghz3406.ztag = false;
    zTestCap = zghz3406;
    unit zgsz3341;
    zgsz3341 = UNIT;
  }
  {
    zTestAddr = UINT64_C(0x0000000000000000);
    unit zgsz3340;
    zgsz3340 = UNIT;
  }
  RECREATE(lbits)(&zghz3405);
  CONVERT_OF(lbits, fbits)(&zghz3405, UINT64_C(0b0), UINT64_C(1) , true);
  append_64(&zghz3405, zghz3405, UINT64_C(0x0000000000000000));
  COPY(lbits)(&zTestLen, zghz3405);
  zcbz384 = UNIT;
end_function_401: ;
  return zcbz384;
end_block_exception_402: ;

  return UNIT;
}



static void finish_sailgen_initializze_registers(void)
{
  KILL(sail_int)(&zghz3407);
  KILL(lbits)(&zghz3405);
}



static void model_init(void)
{
  setup_rts();
  current_exception = sail_new(struct zexception);
  CREATE(zexception)(current_exception);
  throw_location = sail_new(sail_string);
  CREATE(sail_string)(throw_location);
  startup_sailgen_sail_ones();
  startup_sailgen_to_bits();
  startup_sailgen_z8operatorz0zI_uz9();
  startup_sailgen_z8operatorz0zK_uz9();
  startup_sailgen_MAX();
  startup_sailgen_Mk_EncMetadata();
  startup_sailgen__update_EncMetadata_AP_M();
  startup_sailgen__update_EncMetadata_BE();
  startup_sailgen__update_EncMetadata_Bm();
  startup_sailgen__update_EncMetadata_CL();
  startup_sailgen__update_EncMetadata_CT();
  startup_sailgen__update_EncMetadata_EF();
  startup_sailgen__update_EncMetadata_SDP();
  startup_sailgen__update_EncMetadata_TE();
  startup_sailgen__update_EncMetadata_Tm();
  startup_sailgen__update_EncMetadata_reserved_0();
  startup_sailgen__update_EncMetadata_reserved_1();
  startup_sailgen_ap_m_encdec_forwards();
  startup_sailgen_ap_m_encdec_forwards_infallible();
  startup_sailgen_legalizzePermsAndMode();
  startup_sailgen_setMetadataL8();
  startup_sailgen_undefined_Capability();
  startup_sailgen_bitsToCap();
  startup_sailgen_capToMetadataBits();
  startup_sailgen_boundsMalformed();
  startup_sailgen_getCapBoundsBits();
  startup_sailgen_getCapBounds();
  startup_sailgen_setCapBounds();
  startup_sailgen_setCapPermsAndMode();
  startup_sailgen_getArchPerms();
  startup_sailgen_getArchPermsLegalizzed();
  startup_sailgen_getCapLength();
  startup_sailgen_inCapBoundsNoWrap();
  startup_sailgen_clearTagIf();
  startup_sailgen_capBoundsEqual();
  startup_sailgen_setCapAddr();
  startup_sailgen_capToString();
  startup_sailgen_getRepresentableAlignmentMask();
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
  CREATE(lbits)(&zTestLen);
  sailgen_initializze_registers(UNIT);
}

static void model_fini(void)
{
  KILL(lbits)(&zTestLen);
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
  finish_sailgen_sail_ones();
  finish_sailgen_to_bits();
  finish_sailgen_z8operatorz0zI_uz9();
  finish_sailgen_z8operatorz0zK_uz9();
  finish_sailgen_MAX();
  finish_sailgen_Mk_EncMetadata();
  finish_sailgen__update_EncMetadata_AP_M();
  finish_sailgen__update_EncMetadata_BE();
  finish_sailgen__update_EncMetadata_Bm();
  finish_sailgen__update_EncMetadata_CL();
  finish_sailgen__update_EncMetadata_CT();
  finish_sailgen__update_EncMetadata_EF();
  finish_sailgen__update_EncMetadata_SDP();
  finish_sailgen__update_EncMetadata_TE();
  finish_sailgen__update_EncMetadata_Tm();
  finish_sailgen__update_EncMetadata_reserved_0();
  finish_sailgen__update_EncMetadata_reserved_1();
  finish_sailgen_ap_m_encdec_forwards();
  finish_sailgen_ap_m_encdec_forwards_infallible();
  finish_sailgen_legalizzePermsAndMode();
  finish_sailgen_setMetadataL8();
  finish_sailgen_undefined_Capability();
  finish_sailgen_bitsToCap();
  finish_sailgen_capToMetadataBits();
  finish_sailgen_boundsMalformed();
  finish_sailgen_getCapBoundsBits();
  finish_sailgen_getCapBounds();
  finish_sailgen_setCapBounds();
  finish_sailgen_setCapPermsAndMode();
  finish_sailgen_getArchPerms();
  finish_sailgen_getArchPermsLegalizzed();
  finish_sailgen_getCapLength();
  finish_sailgen_inCapBoundsNoWrap();
  finish_sailgen_clearTagIf();
  finish_sailgen_capBoundsEqual();
  finish_sailgen_setCapAddr();
  finish_sailgen_capToString();
  finish_sailgen_getRepresentableAlignmentMask();
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

unit (*const SAIL_TESTS[])(unit) = {
  NULL
};

const char* const SAIL_TEST_NAMES[] = {
};

static void model_test(void)
{
  for (size_t i = 0; SAIL_TESTS[i] != NULL; ++i) {
    model_init();
    printf("Testing %s\n", SAIL_TEST_NAMES[i]);
    SAIL_TESTS[i](UNIT);
    printf("Pass\n");
    model_fini();
  }
}



#ifdef __cplusplus
}
#endif
