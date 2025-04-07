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

// union option<b>
enum kind_zoptionzIbzK { Kind_zNonezIbzK, Kind_zSomezIbzK };

struct zoptionzIbzK {
  enum kind_zoptionzIbzK kind;
  union {
    struct { unit zNonezIbzK; };
    struct { lbits zSomezIbzK; };
  } variants;
};

static void CREATE(zoptionzIbzK)(struct zoptionzIbzK *op) {
  op->kind = Kind_zNonezIbzK;
}

static void RECREATE(zoptionzIbzK)(struct zoptionzIbzK *op) {

}

static void KILL(zoptionzIbzK)(struct zoptionzIbzK *op) {
  if (op->kind == Kind_zSomezIbzK) {
    KILL(lbits)(&op->variants.zSomezIbzK);
  }
}

static void COPY(zoptionzIbzK)(struct zoptionzIbzK *rop, struct zoptionzIbzK op) {
  if (rop->kind == Kind_zSomezIbzK) {
    KILL(lbits)(&rop->variants.zSomezIbzK);
  };
  rop->kind = op.kind;
  if (op.kind == Kind_zNonezIbzK) {
    rop->variants.zNonezIbzK = op.variants.zNonezIbzK;
  } else if (op.kind == Kind_zSomezIbzK) {
    CREATE(lbits)(&rop->variants.zSomezIbzK); COPY(lbits)(&rop->variants.zSomezIbzK, op.variants.zSomezIbzK);
  }
}

static bool EQUAL(zoptionzIbzK)(struct zoptionzIbzK op1, struct zoptionzIbzK op2) {
  if (op1.kind == Kind_zNonezIbzK && op2.kind == Kind_zNonezIbzK) {
    return EQUAL(unit)(op1.variants.zNonezIbzK, op2.variants.zNonezIbzK);
  } else if (op1.kind == Kind_zSomezIbzK && op2.kind == Kind_zSomezIbzK) {
    return EQUAL(lbits)(op1.variants.zSomezIbzK, op2.variants.zSomezIbzK);
  } else return false;
}

static void sailgen_NonezIbzK(struct zoptionzIbzK *rop, unit op) {
  if (rop->kind == Kind_zSomezIbzK) {
    KILL(lbits)(&rop->variants.zSomezIbzK);
  }
  rop->kind = Kind_zNonezIbzK;
  rop->variants.zNonezIbzK = op;
}

static void sailgen_SomezIbzK(struct zoptionzIbzK *rop, lbits op) {
  if (rop->kind == Kind_zSomezIbzK) {
    KILL(lbits)(&rop->variants.zSomezIbzK);
  }
  rop->kind = Kind_zSomezIbzK;
  CREATE(lbits)(&rop->variants.zSomezIbzK);
  COPY(lbits)(&rop->variants.zSomezIbzK, op);
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
typedef int64_t zCapLenInt;

// type abbreviation CapLenBits
typedef uint64_t zCapLenBits;

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
typedef uint64_t zCapBits;

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
  __label__ end_function_7, end_block_exception_8, end_function_677;

  RECREATE(lbits)(&zghz30);
  zeros(&zghz30, zn);
  not_bits((*(&zcbz32)), zghz30);
end_function_7: ;
  goto end_function_677;
end_block_exception_8: ;
  goto end_function_677;
end_function_677: ;
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
  __label__ end_function_19, end_block_exception_20, end_function_676;

  zero_extend((*(&zcbz36)), zv, zm);
end_function_19: ;
  goto end_function_676;
end_block_exception_20: ;
  goto end_function_676;
end_function_676: ;
}

static void sailgen_zzeros_implicit(lbits *rop, sail_int);

static void sailgen_zzeros_implicit(lbits *zcbz37, sail_int zn)
{
  __label__ end_function_22, end_block_exception_23, end_function_675;

  zeros((*(&zcbz37)), zn);
end_function_22: ;
  goto end_function_675;
end_block_exception_23: ;
  goto end_function_675;
end_function_675: ;
}

static void sailgen_ones(lbits *rop, sail_int);

static void sailgen_ones(lbits *zcbz38, sail_int zn)
{
  __label__ end_function_25, end_block_exception_26, end_function_674;

  sailgen_sail_ones((*(&zcbz38)), zn);
end_function_25: ;
  goto end_function_674;
end_block_exception_26: ;
  goto end_function_674;
end_function_674: ;
}

static uint64_t sailgen_bool_bits_forwards(bool);

static bool sailgen_bool_bits_backwards(uint64_t);

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

static void sailgen_to_bits(lbits *rop, sail_int, sail_int);

sail_int zghz31;

static void startup_sailgen_to_bits(void)
{    CREATE(sail_int)(&zghz31);
}

static void sailgen_to_bits(lbits *zcbz313, sail_int zl, sail_int zn)
{
  __label__ end_function_50, end_block_exception_51, end_function_673;

  {
    RECREATE(sail_int)(&zghz31);
    CONVERT_OF(sail_int, mach_int)(&zghz31, INT64_C(0));
    get_slice_int((*(&zcbz313)), zl, zn, zghz31);
  }
end_function_50: ;
  goto end_function_673;
end_block_exception_51: ;
  goto end_function_673;
end_function_673: ;
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
  __label__ end_function_53, end_block_exception_54;

  bool zcbz314;
  RECREATE(sail_int)(&zghz32);
  sail_unsigned(&zghz32, zx);
  RECREATE(sail_int)(&zghz33);
  sail_unsigned(&zghz33, zy);
  zcbz314 = lt(zghz32, zghz33);
end_function_53: ;
  return zcbz314;
end_block_exception_54: ;

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
  __label__ end_function_56, end_block_exception_57;

  bool zcbz315;
  RECREATE(sail_int)(&zghz34);
  sail_unsigned(&zghz34, zx);
  RECREATE(sail_int)(&zghz35);
  sail_unsigned(&zghz35, zy);
  zcbz315 = gt(zghz34, zghz35);
end_function_56: ;
  return zcbz315;
end_block_exception_57: ;

  return false;
}



static void finish_sailgen_z8operatorz0zK_uz9(void)
{
  KILL(sail_int)(&zghz35);
  KILL(sail_int)(&zghz34);
}

static int64_t zlog2_xlen;
static void create_letbind_0(void) {


  int64_t zgsz312;
  zgsz312 = INT64_C(5);
  zlog2_xlen = zgsz312;
let_end_58: ;
}
static void kill_letbind_0(void) {
}

static int64_t zlog2_xlen_bytes;
static void create_letbind_1(void) {


  int64_t zgsz313;
  zgsz313 = INT64_C(2);
  zlog2_xlen_bytes = zgsz313;
let_end_59: ;
}
static void kill_letbind_1(void) {
}

static int64_t zxlen_bytes;
static void create_letbind_2(void) {


  int64_t zgsz314;
  zgsz314 = INT64_C(4);
  zxlen_bytes = zgsz314;
let_end_60: ;
}
static void kill_letbind_2(void) {
}

static int64_t zxlen;
static void create_letbind_3(void) {


  int64_t zgsz315;
  zgsz315 = INT64_C(32);
  zxlen = zgsz315;
let_end_61: ;
}
static void kill_letbind_3(void) {
}

static int64_t zasidlen;
static void create_letbind_4(void) {


  int64_t zgsz316;
  zgsz316 = INT64_C(9);
  zasidlen = zgsz316;
let_end_62: ;
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

static void sailgen_MAX(sail_int *zcbz316, sail_int zn)
{
  __label__ end_function_64, end_block_exception_65, end_function_672;

  RECREATE(sail_int)(&zghz36);
  pow2(&zghz36, zn);
  {
    RECREATE(sail_int)(&zghz37);
    CONVERT_OF(sail_int, mach_int)(&zghz37, INT64_C(1));
    sub_int((*(&zcbz316)), zghz36, zghz37);
  }
end_function_64: ;
  goto end_function_672;
end_block_exception_65: ;
  goto end_function_672;
end_function_672: ;
}



static void finish_sailgen_MAX(void)
{
  KILL(sail_int)(&zghz37);
  KILL(sail_int)(&zghz36);
}

static uint64_t sailgen_execution_mode_encdec_forwards(enum zExecutionMode);

static enum zExecutionMode sailgen_execution_mode_encdec_backwards(uint64_t);

static bool sailgen_execution_mode_encdec_forwards_matches(enum zExecutionMode);

static uint64_t sailgen_execution_mode_encdec_forwards(enum zExecutionMode zargz3)
{
  __label__ case_69, case_68, finish_match_67, end_function_70, end_block_exception_71;

  uint64_t zcbz317;
  {
    if ((zIntPtrMode != zargz3)) goto case_69;
    zcbz317 = UINT64_C(0b1);
    goto finish_match_67;
  }
case_69: ;
  {
    /* complete */
    zcbz317 = UINT64_C(0b0);
    goto finish_match_67;
  }
case_68: ;
finish_match_67: ;
end_function_70: ;
  return zcbz317;
end_block_exception_71: ;

  return UINT64_C(0xdeadc0de);
}

static uint64_t sailgen_execution_mode_encdec_forwards_infallible(enum zExecutionMode);

static uint64_t sailgen_execution_mode_encdec_forwards_infallible(enum zExecutionMode zargz3)
{
  __label__ case_74, case_73, finish_match_72, end_function_75, end_block_exception_76;

  uint64_t zcbz318;
  {
    if ((zIntPtrMode != zargz3)) goto case_74;
    zcbz318 = UINT64_C(0b1);
    goto finish_match_72;
  }
case_74: ;
  {
    /* complete */
    zcbz318 = UINT64_C(0b0);
    goto finish_match_72;
  }
case_73: ;
finish_match_72: ;
end_function_75: ;
  return zcbz318;
end_block_exception_76: ;

  return UINT64_C(0xdeadc0de);
}

static enum zExecutionMode sailgen_execution_mode_encdec_backwards(uint64_t zargz3)
{
  __label__ case_80, case_79, finish_match_78, end_function_81, end_block_exception_82;

  enum zExecutionMode zcbz319;
  {
    uint64_t zb__0;
    zb__0 = zargz3;
    bool zgsz325;
    zgsz325 = (zb__0 == UINT64_C(0b1));
    if (!(zgsz325)) {  goto case_80;  }
    zcbz319 = zIntPtrMode;
    goto finish_match_78;
  }
case_80: ;
  {
    zcbz319 = zCapPtrMode;
    goto finish_match_78;
  }
case_79: ;
finish_match_78: ;
end_function_81: ;
  return zcbz319;
end_block_exception_82: ;

  return zCapPtrMode;
}

static enum zExecutionMode sailgen_execution_mode_encdec_backwards_infallible(uint64_t);

static enum zExecutionMode sailgen_execution_mode_encdec_backwards_infallible(uint64_t zargz3)
{
  __label__ case_85, case_84, finish_match_83, end_function_86, end_block_exception_87;

  enum zExecutionMode zcbz320;
  {
    uint64_t zb__0;
    zb__0 = zargz3;
    bool zgsz328;
    zgsz328 = (zb__0 == UINT64_C(0b1));
    if (!(zgsz328)) {  goto case_85;  }
    zcbz320 = zIntPtrMode;
    goto finish_match_83;
  }
case_85: ;
  {
    zcbz320 = zCapPtrMode;
    goto finish_match_83;
  }
case_84: ;
finish_match_83: ;
end_function_86: ;
  return zcbz320;
end_block_exception_87: ;

  return zCapPtrMode;
}

static bool sailgen_execution_mode_encdec_forwards_matches(enum zExecutionMode zargz3)
{
  __label__ case_92, case_91, case_90, finish_match_89, end_function_93, end_block_exception_94;

  bool zcbz321;
  {
    if ((zIntPtrMode != zargz3)) goto case_92;
    zcbz321 = true;
    goto finish_match_89;
  }
case_92: ;
  {
    if ((zCapPtrMode != zargz3)) goto case_91;
    zcbz321 = true;
    goto finish_match_89;
  }
case_91: ;
  {
    zcbz321 = false;
    goto finish_match_89;
  }
case_90: ;
finish_match_89: ;
end_function_93: ;
  return zcbz321;
end_block_exception_94: ;

  return false;
}

static struct zArchPerms znull_perms;
static void create_letbind_5(void) {


  struct zArchPerms zgsz334;
  struct zArchPerms zgsz333;
  zgsz333.zASR = false;
  zgsz333.zC = false;
  zgsz333.zEL = false;
  zgsz333.zLM = false;
  zgsz333.zR = false;
  zgsz333.zSL = false;
  zgsz333.zW = false;
  zgsz333.zX = false;
  zgsz334 = zgsz333;
  znull_perms = zgsz334;
let_end_95: ;
}
static void kill_letbind_5(void) {
}

static int64_t zlog2_cap_sizze;
static void create_letbind_6(void) {


  int64_t zgsz335;
  zgsz335 = INT64_C(3);
  zlog2_cap_sizze = zgsz335;
let_end_96: ;
}
static void kill_letbind_6(void) {
}

static int64_t zcap_sd_perms_width;
static void create_letbind_7(void) {


  int64_t zgsz336;
  zgsz336 = INT64_C(2);
  zcap_sd_perms_width = zgsz336;
let_end_97: ;
}
static void kill_letbind_7(void) {
}

static int64_t zcap_reserved_1_width;
static void create_letbind_8(void) {


  int64_t zgsz337;
  zgsz337 = INT64_C(1);
  zcap_reserved_1_width = zgsz337;
let_end_98: ;
}
static void kill_letbind_8(void) {
}

static int64_t zcap_reserved_0_width;
static void create_letbind_9(void) {


  int64_t zgsz338;
  zgsz338 = INT64_C(2);
  zcap_reserved_0_width = zgsz338;
let_end_99: ;
}
static void kill_letbind_9(void) {
}

static int64_t zcap_mantissa_width;
static void create_letbind_10(void) {


  int64_t zgsz339;
  zgsz339 = INT64_C(10);
  zcap_mantissa_width = zgsz339;
let_end_100: ;
}
static void kill_letbind_10(void) {
}

static int64_t zcap_E_width;
static void create_letbind_11(void) {


  int64_t zgsz340;
  zgsz340 = INT64_C(5);
  zcap_E_width = zgsz340;
let_end_101: ;
}
static void kill_letbind_11(void) {
}

static int64_t zcap_AP_M_width;
static void create_letbind_12(void) {


  int64_t zgsz341;
  zgsz341 = INT64_C(5);
  zcap_AP_M_width = zgsz341;
let_end_102: ;
}
static void kill_letbind_12(void) {
}

static bool zcap_use_L8;
static void create_letbind_13(void) {


  bool zgsz342;
  zgsz342 = true;
  zcap_use_L8 = zgsz342;
let_end_103: ;
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
  __label__ end_function_105, end_block_exception_106;

  struct zEncMetadata zghz38;
  zghz38.zbits = zv;
end_function_105: ;
  return zghz38;
end_block_exception_106: ;
  struct zEncMetadata zcbz386 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz386;
}



static void finish_sailgen_Mk_EncMetadata(void)
{
}

static uint64_t sailgen__get_EncMetadata_AP_M(struct zEncMetadata);

static uint64_t sailgen__get_EncMetadata_AP_M(struct zEncMetadata zv)
{
  __label__ end_function_108, end_block_exception_109;

  uint64_t zcbz323;
  uint64_t zgaz310;
  zgaz310 = zv.zbits;
  zcbz323 = (safe_rshift(UINT64_MAX, 64 - 5) & (zgaz310 >> INT64_C(25)));
end_function_108: ;
  return zcbz323;
end_block_exception_109: ;

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
  __label__ end_function_111, end_block_exception_112;

  struct zEncMetadata zghz39;
  uint64_t zgaz312;
  {
    uint64_t zgaz311;
    zgaz311 = zv.zbits;
    {
      RECREATE(lbits)(&zghz310);
      CONVERT_OF(lbits, fbits)(&zghz310, zgaz311, UINT64_C(32) , true);
      RECREATE(sail_int)(&zghz311);
      CONVERT_OF(sail_int, mach_int)(&zghz311, INT64_C(29));
      RECREATE(sail_int)(&zghz312);
      CONVERT_OF(sail_int, mach_int)(&zghz312, INT64_C(25));
      RECREATE(lbits)(&zghz313);
      CONVERT_OF(lbits, fbits)(&zghz313, zx, UINT64_C(5) , true);
      RECREATE(lbits)(&zghz314);
      vector_update_subrange_lbits(&zghz314, zghz310, zghz311, zghz312, zghz313);
      zgaz312 = CONVERT_OF(fbits, lbits)(zghz314, true);
    }
  }
  zghz39 = zv;
  zghz39.zbits = zgaz312;
end_function_111: ;
  return zghz39;
end_block_exception_112: ;
  struct zEncMetadata zcbz387 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz387;
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
  __label__ end_function_114, end_block_exception_115;

  uint64_t zcbz325;
  uint64_t zgaz313;
  zgaz313 = zv.zbits;
  zcbz325 = (safe_rshift(UINT64_MAX, 64 - 2) & (zgaz313 >> INT64_C(0)));
end_function_114: ;
  return zcbz325;
end_block_exception_115: ;

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
  __label__ end_function_117, end_block_exception_118;

  struct zEncMetadata zghz315;
  uint64_t zgaz315;
  {
    uint64_t zgaz314;
    zgaz314 = zv.zbits;
    {
      RECREATE(lbits)(&zghz316);
      CONVERT_OF(lbits, fbits)(&zghz316, zgaz314, UINT64_C(32) , true);
      RECREATE(sail_int)(&zghz317);
      CONVERT_OF(sail_int, mach_int)(&zghz317, INT64_C(1));
      RECREATE(sail_int)(&zghz318);
      CONVERT_OF(sail_int, mach_int)(&zghz318, INT64_C(0));
      RECREATE(lbits)(&zghz319);
      CONVERT_OF(lbits, fbits)(&zghz319, zx, UINT64_C(2) , true);
      RECREATE(lbits)(&zghz320);
      vector_update_subrange_lbits(&zghz320, zghz316, zghz317, zghz318, zghz319);
      zgaz315 = CONVERT_OF(fbits, lbits)(zghz320, true);
    }
  }
  zghz315 = zv;
  zghz315.zbits = zgaz315;
end_function_117: ;
  return zghz315;
end_block_exception_118: ;
  struct zEncMetadata zcbz388 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz388;
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
  __label__ end_function_120, end_block_exception_121;

  uint64_t zcbz327;
  uint64_t zgaz316;
  zgaz316 = zv.zbits;
  zcbz327 = (safe_rshift(UINT64_MAX, 64 - 8) & (zgaz316 >> INT64_C(2)));
end_function_120: ;
  return zcbz327;
end_block_exception_121: ;

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
  __label__ end_function_123, end_block_exception_124;

  struct zEncMetadata zghz321;
  uint64_t zgaz318;
  {
    uint64_t zgaz317;
    zgaz317 = zv.zbits;
    {
      RECREATE(lbits)(&zghz322);
      CONVERT_OF(lbits, fbits)(&zghz322, zgaz317, UINT64_C(32) , true);
      RECREATE(sail_int)(&zghz323);
      CONVERT_OF(sail_int, mach_int)(&zghz323, INT64_C(9));
      RECREATE(sail_int)(&zghz324);
      CONVERT_OF(sail_int, mach_int)(&zghz324, INT64_C(2));
      RECREATE(lbits)(&zghz325);
      CONVERT_OF(lbits, fbits)(&zghz325, zx, UINT64_C(8) , true);
      RECREATE(lbits)(&zghz326);
      vector_update_subrange_lbits(&zghz326, zghz322, zghz323, zghz324, zghz325);
      zgaz318 = CONVERT_OF(fbits, lbits)(zghz326, true);
    }
  }
  zghz321 = zv;
  zghz321.zbits = zgaz318;
end_function_123: ;
  return zghz321;
end_block_exception_124: ;
  struct zEncMetadata zcbz389 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz389;
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
  __label__ end_function_126, end_block_exception_127;

  uint64_t zcbz329;
  uint64_t zgaz319;
  zgaz319 = zv.zbits;
  zcbz329 = (safe_rshift(UINT64_MAX, 64 - 1) & (zgaz319 >> INT64_C(24)));
end_function_126: ;
  return zcbz329;
end_block_exception_127: ;

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
  __label__ end_function_129, end_block_exception_130;

  struct zEncMetadata zghz327;
  uint64_t zgaz321;
  {
    uint64_t zgaz320;
    zgaz320 = zv.zbits;
    {
      RECREATE(lbits)(&zghz328);
      CONVERT_OF(lbits, fbits)(&zghz328, zgaz320, UINT64_C(32) , true);
      RECREATE(sail_int)(&zghz329);
      CONVERT_OF(sail_int, mach_int)(&zghz329, INT64_C(24));
      RECREATE(sail_int)(&zghz330);
      CONVERT_OF(sail_int, mach_int)(&zghz330, INT64_C(24));
      RECREATE(lbits)(&zghz331);
      CONVERT_OF(lbits, fbits)(&zghz331, zx, UINT64_C(1) , true);
      RECREATE(lbits)(&zghz332);
      vector_update_subrange_lbits(&zghz332, zghz328, zghz329, zghz330, zghz331);
      zgaz321 = CONVERT_OF(fbits, lbits)(zghz332, true);
    }
  }
  zghz327 = zv;
  zghz327.zbits = zgaz321;
end_function_129: ;
  return zghz327;
end_block_exception_130: ;
  struct zEncMetadata zcbz390 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz390;
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
  __label__ end_function_132, end_block_exception_133;

  uint64_t zcbz331;
  uint64_t zgaz322;
  zgaz322 = zv.zbits;
  zcbz331 = (safe_rshift(UINT64_MAX, 64 - 1) & (zgaz322 >> INT64_C(20)));
end_function_132: ;
  return zcbz331;
end_block_exception_133: ;

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
  __label__ end_function_135, end_block_exception_136;

  struct zEncMetadata zghz333;
  uint64_t zgaz324;
  {
    uint64_t zgaz323;
    zgaz323 = zv.zbits;
    {
      RECREATE(lbits)(&zghz334);
      CONVERT_OF(lbits, fbits)(&zghz334, zgaz323, UINT64_C(32) , true);
      RECREATE(sail_int)(&zghz335);
      CONVERT_OF(sail_int, mach_int)(&zghz335, INT64_C(20));
      RECREATE(sail_int)(&zghz336);
      CONVERT_OF(sail_int, mach_int)(&zghz336, INT64_C(20));
      RECREATE(lbits)(&zghz337);
      CONVERT_OF(lbits, fbits)(&zghz337, zx, UINT64_C(1) , true);
      RECREATE(lbits)(&zghz338);
      vector_update_subrange_lbits(&zghz338, zghz334, zghz335, zghz336, zghz337);
      zgaz324 = CONVERT_OF(fbits, lbits)(zghz338, true);
    }
  }
  zghz333 = zv;
  zghz333.zbits = zgaz324;
end_function_135: ;
  return zghz333;
end_block_exception_136: ;
  struct zEncMetadata zcbz391 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz391;
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
  __label__ end_function_138, end_block_exception_139;

  uint64_t zcbz333;
  uint64_t zgaz325;
  zgaz325 = zv.zbits;
  zcbz333 = (safe_rshift(UINT64_MAX, 64 - 1) & (zgaz325 >> INT64_C(19)));
end_function_138: ;
  return zcbz333;
end_block_exception_139: ;

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
  __label__ end_function_141, end_block_exception_142;

  struct zEncMetadata zghz339;
  uint64_t zgaz327;
  {
    uint64_t zgaz326;
    zgaz326 = zv.zbits;
    {
      RECREATE(lbits)(&zghz340);
      CONVERT_OF(lbits, fbits)(&zghz340, zgaz326, UINT64_C(32) , true);
      RECREATE(sail_int)(&zghz341);
      CONVERT_OF(sail_int, mach_int)(&zghz341, INT64_C(19));
      RECREATE(sail_int)(&zghz342);
      CONVERT_OF(sail_int, mach_int)(&zghz342, INT64_C(19));
      RECREATE(lbits)(&zghz343);
      CONVERT_OF(lbits, fbits)(&zghz343, zx, UINT64_C(1) , true);
      RECREATE(lbits)(&zghz344);
      vector_update_subrange_lbits(&zghz344, zghz340, zghz341, zghz342, zghz343);
      zgaz327 = CONVERT_OF(fbits, lbits)(zghz344, true);
    }
  }
  zghz339 = zv;
  zghz339.zbits = zgaz327;
end_function_141: ;
  return zghz339;
end_block_exception_142: ;
  struct zEncMetadata zcbz392 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz392;
}



static void finish_sailgen__update_EncMetadata_EF(void)
{
  KILL(lbits)(&zghz344);
  KILL(lbits)(&zghz343);
  KILL(sail_int)(&zghz342);
  KILL(sail_int)(&zghz341);
  KILL(lbits)(&zghz340);
}

static uint64_t sailgen__get_EncMetadata_L8(struct zEncMetadata);

static uint64_t sailgen__get_EncMetadata_L8(struct zEncMetadata zv)
{
  __label__ end_function_144, end_block_exception_145;

  uint64_t zcbz335;
  uint64_t zgaz328;
  zgaz328 = zv.zbits;
  zcbz335 = (safe_rshift(UINT64_MAX, 64 - 1) & (zgaz328 >> INT64_C(18)));
end_function_144: ;
  return zcbz335;
end_block_exception_145: ;

  return UINT64_C(0xdeadc0de);
}

static struct zEncMetadata sailgen__update_EncMetadata_L8(struct zEncMetadata, uint64_t);

struct zEncMetadata zghz345;
lbits zghz346;
sail_int zghz347;
sail_int zghz348;
lbits zghz349;
lbits zghz350;

static void startup_sailgen__update_EncMetadata_L8(void)
{

  CREATE(lbits)(&zghz346);
  CREATE(sail_int)(&zghz347);
  CREATE(sail_int)(&zghz348);
  CREATE(lbits)(&zghz349);
  CREATE(lbits)(&zghz350);
}

static struct zEncMetadata sailgen__update_EncMetadata_L8(struct zEncMetadata zv, uint64_t zx)
{
  __label__ end_function_147, end_block_exception_148;

  struct zEncMetadata zghz345;
  uint64_t zgaz330;
  {
    uint64_t zgaz329;
    zgaz329 = zv.zbits;
    {
      RECREATE(lbits)(&zghz346);
      CONVERT_OF(lbits, fbits)(&zghz346, zgaz329, UINT64_C(32) , true);
      RECREATE(sail_int)(&zghz347);
      CONVERT_OF(sail_int, mach_int)(&zghz347, INT64_C(18));
      RECREATE(sail_int)(&zghz348);
      CONVERT_OF(sail_int, mach_int)(&zghz348, INT64_C(18));
      RECREATE(lbits)(&zghz349);
      CONVERT_OF(lbits, fbits)(&zghz349, zx, UINT64_C(1) , true);
      RECREATE(lbits)(&zghz350);
      vector_update_subrange_lbits(&zghz350, zghz346, zghz347, zghz348, zghz349);
      zgaz330 = CONVERT_OF(fbits, lbits)(zghz350, true);
    }
  }
  zghz345 = zv;
  zghz345.zbits = zgaz330;
end_function_147: ;
  return zghz345;
end_block_exception_148: ;
  struct zEncMetadata zcbz393 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz393;
}



static void finish_sailgen__update_EncMetadata_L8(void)
{
  KILL(lbits)(&zghz350);
  KILL(lbits)(&zghz349);
  KILL(sail_int)(&zghz348);
  KILL(sail_int)(&zghz347);
  KILL(lbits)(&zghz346);
}

static uint64_t sailgen__get_EncMetadata_SDP(struct zEncMetadata);

static uint64_t sailgen__get_EncMetadata_SDP(struct zEncMetadata zv)
{
  __label__ end_function_150, end_block_exception_151;

  uint64_t zcbz337;
  uint64_t zgaz331;
  zgaz331 = zv.zbits;
  zcbz337 = (safe_rshift(UINT64_MAX, 64 - 2) & (zgaz331 >> INT64_C(30)));
end_function_150: ;
  return zcbz337;
end_block_exception_151: ;

  return UINT64_C(0xdeadc0de);
}

static struct zEncMetadata sailgen__update_EncMetadata_SDP(struct zEncMetadata, uint64_t);

struct zEncMetadata zghz351;
lbits zghz352;
sail_int zghz353;
sail_int zghz354;
lbits zghz355;
lbits zghz356;

static void startup_sailgen__update_EncMetadata_SDP(void)
{

  CREATE(lbits)(&zghz352);
  CREATE(sail_int)(&zghz353);
  CREATE(sail_int)(&zghz354);
  CREATE(lbits)(&zghz355);
  CREATE(lbits)(&zghz356);
}

static struct zEncMetadata sailgen__update_EncMetadata_SDP(struct zEncMetadata zv, uint64_t zx)
{
  __label__ end_function_153, end_block_exception_154;

  struct zEncMetadata zghz351;
  uint64_t zgaz333;
  {
    uint64_t zgaz332;
    zgaz332 = zv.zbits;
    {
      RECREATE(lbits)(&zghz352);
      CONVERT_OF(lbits, fbits)(&zghz352, zgaz332, UINT64_C(32) , true);
      RECREATE(sail_int)(&zghz353);
      CONVERT_OF(sail_int, mach_int)(&zghz353, INT64_C(31));
      RECREATE(sail_int)(&zghz354);
      CONVERT_OF(sail_int, mach_int)(&zghz354, INT64_C(30));
      RECREATE(lbits)(&zghz355);
      CONVERT_OF(lbits, fbits)(&zghz355, zx, UINT64_C(2) , true);
      RECREATE(lbits)(&zghz356);
      vector_update_subrange_lbits(&zghz356, zghz352, zghz353, zghz354, zghz355);
      zgaz333 = CONVERT_OF(fbits, lbits)(zghz356, true);
    }
  }
  zghz351 = zv;
  zghz351.zbits = zgaz333;
end_function_153: ;
  return zghz351;
end_block_exception_154: ;
  struct zEncMetadata zcbz394 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz394;
}



static void finish_sailgen__update_EncMetadata_SDP(void)
{
  KILL(lbits)(&zghz356);
  KILL(lbits)(&zghz355);
  KILL(sail_int)(&zghz354);
  KILL(sail_int)(&zghz353);
  KILL(lbits)(&zghz352);
}

static uint64_t sailgen__get_EncMetadata_TE(struct zEncMetadata);

static uint64_t sailgen__get_EncMetadata_TE(struct zEncMetadata zv)
{
  __label__ end_function_156, end_block_exception_157;

  uint64_t zcbz339;
  uint64_t zgaz334;
  zgaz334 = zv.zbits;
  zcbz339 = (safe_rshift(UINT64_MAX, 64 - 2) & (zgaz334 >> INT64_C(10)));
end_function_156: ;
  return zcbz339;
end_block_exception_157: ;

  return UINT64_C(0xdeadc0de);
}

static struct zEncMetadata sailgen__update_EncMetadata_TE(struct zEncMetadata, uint64_t);

struct zEncMetadata zghz357;
lbits zghz358;
sail_int zghz359;
sail_int zghz360;
lbits zghz361;
lbits zghz362;

static void startup_sailgen__update_EncMetadata_TE(void)
{

  CREATE(lbits)(&zghz358);
  CREATE(sail_int)(&zghz359);
  CREATE(sail_int)(&zghz360);
  CREATE(lbits)(&zghz361);
  CREATE(lbits)(&zghz362);
}

static struct zEncMetadata sailgen__update_EncMetadata_TE(struct zEncMetadata zv, uint64_t zx)
{
  __label__ end_function_159, end_block_exception_160;

  struct zEncMetadata zghz357;
  uint64_t zgaz336;
  {
    uint64_t zgaz335;
    zgaz335 = zv.zbits;
    {
      RECREATE(lbits)(&zghz358);
      CONVERT_OF(lbits, fbits)(&zghz358, zgaz335, UINT64_C(32) , true);
      RECREATE(sail_int)(&zghz359);
      CONVERT_OF(sail_int, mach_int)(&zghz359, INT64_C(11));
      RECREATE(sail_int)(&zghz360);
      CONVERT_OF(sail_int, mach_int)(&zghz360, INT64_C(10));
      RECREATE(lbits)(&zghz361);
      CONVERT_OF(lbits, fbits)(&zghz361, zx, UINT64_C(2) , true);
      RECREATE(lbits)(&zghz362);
      vector_update_subrange_lbits(&zghz362, zghz358, zghz359, zghz360, zghz361);
      zgaz336 = CONVERT_OF(fbits, lbits)(zghz362, true);
    }
  }
  zghz357 = zv;
  zghz357.zbits = zgaz336;
end_function_159: ;
  return zghz357;
end_block_exception_160: ;
  struct zEncMetadata zcbz395 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz395;
}



static void finish_sailgen__update_EncMetadata_TE(void)
{
  KILL(lbits)(&zghz362);
  KILL(lbits)(&zghz361);
  KILL(sail_int)(&zghz360);
  KILL(sail_int)(&zghz359);
  KILL(lbits)(&zghz358);
}

static uint64_t sailgen__get_EncMetadata_Tm(struct zEncMetadata);

static uint64_t sailgen__get_EncMetadata_Tm(struct zEncMetadata zv)
{
  __label__ end_function_162, end_block_exception_163;

  uint64_t zcbz341;
  uint64_t zgaz337;
  zgaz337 = zv.zbits;
  zcbz341 = (safe_rshift(UINT64_MAX, 64 - 6) & (zgaz337 >> INT64_C(12)));
end_function_162: ;
  return zcbz341;
end_block_exception_163: ;

  return UINT64_C(0xdeadc0de);
}

static struct zEncMetadata sailgen__update_EncMetadata_Tm(struct zEncMetadata, uint64_t);

struct zEncMetadata zghz363;
lbits zghz364;
sail_int zghz365;
sail_int zghz366;
lbits zghz367;
lbits zghz368;

static void startup_sailgen__update_EncMetadata_Tm(void)
{

  CREATE(lbits)(&zghz364);
  CREATE(sail_int)(&zghz365);
  CREATE(sail_int)(&zghz366);
  CREATE(lbits)(&zghz367);
  CREATE(lbits)(&zghz368);
}

static struct zEncMetadata sailgen__update_EncMetadata_Tm(struct zEncMetadata zv, uint64_t zx)
{
  __label__ end_function_165, end_block_exception_166;

  struct zEncMetadata zghz363;
  uint64_t zgaz339;
  {
    uint64_t zgaz338;
    zgaz338 = zv.zbits;
    {
      RECREATE(lbits)(&zghz364);
      CONVERT_OF(lbits, fbits)(&zghz364, zgaz338, UINT64_C(32) , true);
      RECREATE(sail_int)(&zghz365);
      CONVERT_OF(sail_int, mach_int)(&zghz365, INT64_C(17));
      RECREATE(sail_int)(&zghz366);
      CONVERT_OF(sail_int, mach_int)(&zghz366, INT64_C(12));
      RECREATE(lbits)(&zghz367);
      CONVERT_OF(lbits, fbits)(&zghz367, zx, UINT64_C(6) , true);
      RECREATE(lbits)(&zghz368);
      vector_update_subrange_lbits(&zghz368, zghz364, zghz365, zghz366, zghz367);
      zgaz339 = CONVERT_OF(fbits, lbits)(zghz368, true);
    }
  }
  zghz363 = zv;
  zghz363.zbits = zgaz339;
end_function_165: ;
  return zghz363;
end_block_exception_166: ;
  struct zEncMetadata zcbz396 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz396;
}



static void finish_sailgen__update_EncMetadata_Tm(void)
{
  KILL(lbits)(&zghz368);
  KILL(lbits)(&zghz367);
  KILL(sail_int)(&zghz366);
  KILL(sail_int)(&zghz365);
  KILL(lbits)(&zghz364);
}

static uint64_t sailgen__get_EncMetadata_reserved_0(struct zEncMetadata);

static uint64_t sailgen__get_EncMetadata_reserved_0(struct zEncMetadata zv)
{
  __label__ end_function_168, end_block_exception_169;

  uint64_t zcbz343;
  uint64_t zgaz340;
  zgaz340 = zv.zbits;
  zcbz343 = (safe_rshift(UINT64_MAX, 64 - 2) & (zgaz340 >> INT64_C(21)));
end_function_168: ;
  return zcbz343;
end_block_exception_169: ;

  return UINT64_C(0xdeadc0de);
}

static struct zEncMetadata sailgen__update_EncMetadata_reserved_0(struct zEncMetadata, uint64_t);

struct zEncMetadata zghz369;
lbits zghz370;
sail_int zghz371;
sail_int zghz372;
lbits zghz373;
lbits zghz374;

static void startup_sailgen__update_EncMetadata_reserved_0(void)
{

  CREATE(lbits)(&zghz370);
  CREATE(sail_int)(&zghz371);
  CREATE(sail_int)(&zghz372);
  CREATE(lbits)(&zghz373);
  CREATE(lbits)(&zghz374);
}

static struct zEncMetadata sailgen__update_EncMetadata_reserved_0(struct zEncMetadata zv, uint64_t zx)
{
  __label__ end_function_171, end_block_exception_172;

  struct zEncMetadata zghz369;
  uint64_t zgaz342;
  {
    uint64_t zgaz341;
    zgaz341 = zv.zbits;
    {
      RECREATE(lbits)(&zghz370);
      CONVERT_OF(lbits, fbits)(&zghz370, zgaz341, UINT64_C(32) , true);
      RECREATE(sail_int)(&zghz371);
      CONVERT_OF(sail_int, mach_int)(&zghz371, INT64_C(22));
      RECREATE(sail_int)(&zghz372);
      CONVERT_OF(sail_int, mach_int)(&zghz372, INT64_C(21));
      RECREATE(lbits)(&zghz373);
      CONVERT_OF(lbits, fbits)(&zghz373, zx, UINT64_C(2) , true);
      RECREATE(lbits)(&zghz374);
      vector_update_subrange_lbits(&zghz374, zghz370, zghz371, zghz372, zghz373);
      zgaz342 = CONVERT_OF(fbits, lbits)(zghz374, true);
    }
  }
  zghz369 = zv;
  zghz369.zbits = zgaz342;
end_function_171: ;
  return zghz369;
end_block_exception_172: ;
  struct zEncMetadata zcbz397 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz397;
}



static void finish_sailgen__update_EncMetadata_reserved_0(void)
{
  KILL(lbits)(&zghz374);
  KILL(lbits)(&zghz373);
  KILL(sail_int)(&zghz372);
  KILL(sail_int)(&zghz371);
  KILL(lbits)(&zghz370);
}

static uint64_t sailgen__get_EncMetadata_reserved_1(struct zEncMetadata);

static uint64_t sailgen__get_EncMetadata_reserved_1(struct zEncMetadata zv)
{
  __label__ end_function_174, end_block_exception_175;

  uint64_t zcbz345;
  uint64_t zgaz343;
  zgaz343 = zv.zbits;
  zcbz345 = (safe_rshift(UINT64_MAX, 64 - 1) & (zgaz343 >> INT64_C(23)));
end_function_174: ;
  return zcbz345;
end_block_exception_175: ;

  return UINT64_C(0xdeadc0de);
}

static struct zEncMetadata sailgen__update_EncMetadata_reserved_1(struct zEncMetadata, uint64_t);

struct zEncMetadata zghz375;
lbits zghz376;
sail_int zghz377;
sail_int zghz378;
lbits zghz379;
lbits zghz380;

static void startup_sailgen__update_EncMetadata_reserved_1(void)
{

  CREATE(lbits)(&zghz376);
  CREATE(sail_int)(&zghz377);
  CREATE(sail_int)(&zghz378);
  CREATE(lbits)(&zghz379);
  CREATE(lbits)(&zghz380);
}

static struct zEncMetadata sailgen__update_EncMetadata_reserved_1(struct zEncMetadata zv, uint64_t zx)
{
  __label__ end_function_177, end_block_exception_178;

  struct zEncMetadata zghz375;
  uint64_t zgaz345;
  {
    uint64_t zgaz344;
    zgaz344 = zv.zbits;
    {
      RECREATE(lbits)(&zghz376);
      CONVERT_OF(lbits, fbits)(&zghz376, zgaz344, UINT64_C(32) , true);
      RECREATE(sail_int)(&zghz377);
      CONVERT_OF(sail_int, mach_int)(&zghz377, INT64_C(23));
      RECREATE(sail_int)(&zghz378);
      CONVERT_OF(sail_int, mach_int)(&zghz378, INT64_C(23));
      RECREATE(lbits)(&zghz379);
      CONVERT_OF(lbits, fbits)(&zghz379, zx, UINT64_C(1) , true);
      RECREATE(lbits)(&zghz380);
      vector_update_subrange_lbits(&zghz380, zghz376, zghz377, zghz378, zghz379);
      zgaz345 = CONVERT_OF(fbits, lbits)(zghz380, true);
    }
  }
  zghz375 = zv;
  zghz375.zbits = zgaz345;
end_function_177: ;
  return zghz375;
end_block_exception_178: ;
  struct zEncMetadata zcbz398 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz398;
}



static void finish_sailgen__update_EncMetadata_reserved_1(void)
{
  KILL(lbits)(&zghz380);
  KILL(lbits)(&zghz379);
  KILL(sail_int)(&zghz378);
  KILL(sail_int)(&zghz377);
  KILL(lbits)(&zghz376);
}

static struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 sailgen_ap_m_encdec_forwards(uint64_t);

static uint64_t sailgen_ap_m_encdec_backwards(struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9);

static bool sailgen_ap_m_encdec_forwards_matches(uint64_t);

struct zArchPerms zghz381;
struct zArchPerms zghz382;
struct zArchPerms zghz383;
struct zArchPerms zghz384;
struct zArchPerms zghz385;
struct zArchPerms zghz386;
struct zArchPerms zghz387;
struct zArchPerms zghz388;
struct zArchPerms zghz389;
struct zArchPerms zghz390;
struct zArchPerms zghz391;
struct zArchPerms zghz392;
struct zArchPerms zghz393;
struct zArchPerms zghz394;

static void startup_sailgen_ap_m_encdec_forwards(void)
{














}

static struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 sailgen_ap_m_encdec_forwards(uint64_t zargz3)
{
  __label__ case_194, case_193, case_192, case_191, case_190, case_189, case_188, case_187, case_186, case_185, case_184, case_183, case_182, case_181, finish_match_180, end_function_195, end_block_exception_196;

  struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zcbz347;
  {
    uint64_t zv__0;
    zv__0 = zargz3;
    bool zgsz398;
    zgsz398 = (zv__0 == UINT64_C(0b00000));
    if (!(zgsz398)) {  goto case_194;  }
    enum zExecutionMode zgaz346;
    zgaz346 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    struct zArchPerms zghz394;
    zghz394.zASR = false;
    zghz394.zC = false;
    zghz394.zEL = false;
    zghz394.zLM = false;
    zghz394.zR = false;
    zghz394.zSL = false;
    zghz394.zW = false;
    zghz394.zX = false;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz397;
    zgsz397.ztup0 = zghz394;
    zgsz397.ztup1 = zgaz346;
    zcbz347 = zgsz397;
    goto finish_match_180;
  }
case_194: ;
  {
    uint64_t zv__3;
    zv__3 = zargz3;
    bool zgsz395;
    zgsz395 = (zv__3 == UINT64_C(0b00001));
    if (!(zgsz395)) {  goto case_193;  }
    enum zExecutionMode zgaz347;
    zgaz347 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    struct zArchPerms zghz393;
    zghz393.zASR = false;
    zghz393.zC = false;
    zghz393.zEL = false;
    zghz393.zLM = false;
    zghz393.zR = true;
    zghz393.zSL = false;
    zghz393.zW = false;
    zghz393.zX = false;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz394;
    zgsz394.ztup0 = zghz393;
    zgsz394.ztup1 = zgaz347;
    zcbz347 = zgsz394;
    goto finish_match_180;
  }
case_193: ;
  {
    uint64_t zv__6;
    zv__6 = zargz3;
    bool zgsz392;
    zgsz392 = (zv__6 == UINT64_C(0b00100));
    if (!(zgsz392)) {  goto case_192;  }
    enum zExecutionMode zgaz348;
    zgaz348 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    struct zArchPerms zghz392;
    zghz392.zASR = false;
    zghz392.zC = false;
    zghz392.zEL = false;
    zghz392.zLM = false;
    zghz392.zR = false;
    zghz392.zSL = false;
    zghz392.zW = true;
    zghz392.zX = false;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz391;
    zgsz391.ztup0 = zghz392;
    zgsz391.ztup1 = zgaz348;
    zcbz347 = zgsz391;
    goto finish_match_180;
  }
case_192: ;
  {
    uint64_t zv__9;
    zv__9 = zargz3;
    bool zgsz389;
    zgsz389 = (zv__9 == UINT64_C(0b00101));
    if (!(zgsz389)) {  goto case_191;  }
    enum zExecutionMode zgaz349;
    zgaz349 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    struct zArchPerms zghz391;
    zghz391.zASR = false;
    zghz391.zC = false;
    zghz391.zEL = false;
    zghz391.zLM = false;
    zghz391.zR = true;
    zghz391.zSL = false;
    zghz391.zW = true;
    zghz391.zX = false;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz388;
    zgsz388.ztup0 = zghz391;
    zgsz388.ztup1 = zgaz349;
    zcbz347 = zgsz388;
    goto finish_match_180;
  }
case_191: ;
  {
    uint64_t zv__12;
    zv__12 = zargz3;
    uint64_t zgaz351;
    zgaz351 = (safe_rshift(UINT64_MAX, 64 - 4) & (zv__12 >> INT64_C(1)));
    bool zgsz386;
    zgsz386 = (zgaz351 == UINT64_C(0x4));
    if (!(zgsz386)) {  goto case_190;  }
    uint64_t zm;
    zm = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__12 >> INT64_C(0)));
    enum zExecutionMode zgaz350;
    zgaz350 = sailgen_execution_mode_encdec_backwards(zm);
    struct zArchPerms zghz390;
    zghz390.zASR = true;
    zghz390.zC = true;
    zghz390.zEL = true;
    zghz390.zLM = true;
    zghz390.zR = true;
    zghz390.zSL = true;
    zghz390.zW = true;
    zghz390.zX = true;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz385;
    zgsz385.ztup0 = zghz390;
    zgsz385.ztup1 = zgaz350;
    zcbz347 = zgsz385;
    goto finish_match_180;
  }
case_190: ;
  {
    uint64_t zv__15;
    zv__15 = zargz3;
    uint64_t zgaz353;
    zgaz353 = (safe_rshift(UINT64_MAX, 64 - 4) & (zv__15 >> INT64_C(1)));
    bool zgsz383;
    zgsz383 = (zgaz353 == UINT64_C(0x5));
    if (!(zgsz383)) {  goto case_189;  }
    uint64_t zuz32;
    zuz32 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__15 >> INT64_C(0)));
    enum zExecutionMode zgaz352;
    zgaz352 = sailgen_execution_mode_encdec_backwards(zuz32);
    struct zArchPerms zghz389;
    zghz389.zASR = false;
    zghz389.zC = true;
    zghz389.zEL = true;
    zghz389.zLM = true;
    zghz389.zR = true;
    zghz389.zSL = true;
    zghz389.zW = false;
    zghz389.zX = true;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz382;
    zgsz382.ztup0 = zghz389;
    zgsz382.ztup1 = zgaz352;
    zcbz347 = zgsz382;
    goto finish_match_180;
  }
case_189: ;
  {
    uint64_t zv__18;
    zv__18 = zargz3;
    uint64_t zgaz355;
    zgaz355 = (safe_rshift(UINT64_MAX, 64 - 4) & (zv__18 >> INT64_C(1)));
    bool zgsz380;
    zgsz380 = (zgaz355 == UINT64_C(0x6));
    if (!(zgsz380)) {  goto case_188;  }
    uint64_t zuz33;
    zuz33 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__18 >> INT64_C(0)));
    enum zExecutionMode zgaz354;
    zgaz354 = sailgen_execution_mode_encdec_backwards(zuz33);
    struct zArchPerms zghz388;
    zghz388.zASR = false;
    zghz388.zC = true;
    zghz388.zEL = true;
    zghz388.zLM = true;
    zghz388.zR = true;
    zghz388.zSL = true;
    zghz388.zW = true;
    zghz388.zX = true;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz379;
    zgsz379.ztup0 = zghz388;
    zgsz379.ztup1 = zgaz354;
    zcbz347 = zgsz379;
    goto finish_match_180;
  }
case_188: ;
  {
    uint64_t zv__21;
    zv__21 = zargz3;
    uint64_t zgaz357;
    zgaz357 = (safe_rshift(UINT64_MAX, 64 - 4) & (zv__21 >> INT64_C(1)));
    bool zgsz377;
    zgsz377 = (zgaz357 == UINT64_C(0x7));
    if (!(zgsz377)) {  goto case_187;  }
    uint64_t zuz34;
    zuz34 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__21 >> INT64_C(0)));
    enum zExecutionMode zgaz356;
    zgaz356 = sailgen_execution_mode_encdec_backwards(zuz34);
    struct zArchPerms zghz387;
    zghz387.zASR = false;
    zghz387.zC = false;
    zghz387.zEL = false;
    zghz387.zLM = false;
    zghz387.zR = true;
    zghz387.zSL = false;
    zghz387.zW = true;
    zghz387.zX = true;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz376;
    zgsz376.ztup0 = zghz387;
    zgsz376.ztup1 = zgaz356;
    zcbz347 = zgsz376;
    goto finish_match_180;
  }
case_187: ;
  {
    uint64_t zv__24;
    zv__24 = zargz3;
    bool zgsz374;
    zgsz374 = (zv__24 == UINT64_C(0b10011));
    if (!(zgsz374)) {  goto case_186;  }
    enum zExecutionMode zgaz358;
    zgaz358 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    struct zArchPerms zghz386;
    zghz386.zASR = false;
    zghz386.zC = true;
    zghz386.zEL = false;
    zghz386.zLM = false;
    zghz386.zR = true;
    zghz386.zSL = false;
    zghz386.zW = false;
    zghz386.zX = false;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz373;
    zgsz373.ztup0 = zghz386;
    zgsz373.ztup1 = zgaz358;
    zcbz347 = zgsz373;
    goto finish_match_180;
  }
case_186: ;
  {
    uint64_t zv__27;
    zv__27 = zargz3;
    bool zgsz371;
    zgsz371 = (zv__27 == UINT64_C(0b10110));
    if (!(zgsz371)) {  goto case_185;  }
    enum zExecutionMode zgaz359;
    zgaz359 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    struct zArchPerms zghz385;
    zghz385.zASR = false;
    zghz385.zC = true;
    zghz385.zEL = false;
    zghz385.zLM = true;
    zghz385.zR = true;
    zghz385.zSL = true;
    zghz385.zW = true;
    zghz385.zX = false;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz370;
    zgsz370.ztup0 = zghz385;
    zgsz370.ztup1 = zgaz359;
    zcbz347 = zgsz370;
    goto finish_match_180;
  }
case_185: ;
  {
    uint64_t zv__30;
    zv__30 = zargz3;
    bool zgsz368;
    zgsz368 = (zv__30 == UINT64_C(0b10111));
    if (!(zgsz368)) {  goto case_184;  }
    enum zExecutionMode zgaz360;
    zgaz360 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    struct zArchPerms zghz384;
    zghz384.zASR = false;
    zghz384.zC = true;
    zghz384.zEL = false;
    zghz384.zLM = true;
    zghz384.zR = true;
    zghz384.zSL = false;
    zghz384.zW = true;
    zghz384.zX = false;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz367;
    zgsz367.ztup0 = zghz384;
    zgsz367.ztup1 = zgaz360;
    zcbz347 = zgsz367;
    goto finish_match_180;
  }
case_184: ;
  {
    uint64_t zv__33;
    zv__33 = zargz3;
    bool zgsz365;
    zgsz365 = (zv__33 == UINT64_C(0b11011));
    if (!(zgsz365)) {  goto case_183;  }
    enum zExecutionMode zgaz361;
    zgaz361 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    struct zArchPerms zghz383;
    zghz383.zASR = false;
    zghz383.zC = true;
    zghz383.zEL = true;
    zghz383.zLM = true;
    zghz383.zR = true;
    zghz383.zSL = false;
    zghz383.zW = false;
    zghz383.zX = false;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz364;
    zgsz364.ztup0 = zghz383;
    zgsz364.ztup1 = zgaz361;
    zcbz347 = zgsz364;
    goto finish_match_180;
  }
case_183: ;
  {
    uint64_t zv__36;
    zv__36 = zargz3;
    bool zgsz362;
    zgsz362 = (zv__36 == UINT64_C(0b11110));
    if (!(zgsz362)) {  goto case_182;  }
    enum zExecutionMode zgaz362;
    zgaz362 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    struct zArchPerms zghz382;
    zghz382.zASR = false;
    zghz382.zC = true;
    zghz382.zEL = true;
    zghz382.zLM = true;
    zghz382.zR = true;
    zghz382.zSL = true;
    zghz382.zW = true;
    zghz382.zX = false;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz361;
    zgsz361.ztup0 = zghz382;
    zgsz361.ztup1 = zgaz362;
    zcbz347 = zgsz361;
    goto finish_match_180;
  }
case_182: ;
  {
    uint64_t zv__39;
    zv__39 = zargz3;
    bool zgsz359;
    zgsz359 = (zv__39 == UINT64_C(0b11111));
    if (!(zgsz359)) {  goto case_181;  }
    enum zExecutionMode zgaz363;
    zgaz363 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    struct zArchPerms zghz381;
    zghz381.zASR = false;
    zghz381.zC = true;
    zghz381.zEL = true;
    zghz381.zLM = true;
    zghz381.zR = true;
    zghz381.zSL = false;
    zghz381.zW = true;
    zghz381.zX = false;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz358;
    zgsz358.ztup0 = zghz381;
    zgsz358.ztup1 = zgaz363;
    zcbz347 = zgsz358;
    goto finish_match_180;
  }
case_181: ;
  sail_match_failure("ap_m_encdec_forwards");
finish_match_180: ;
end_function_195: ;
  return zcbz347;
end_block_exception_196: ;
  struct zArchPerms zcbz3100 = { .zASR = false, .zC = false, .zEL = false, .zLM = false, .zR = false, .zSL = false, .zW = false, .zX = false };
  struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zcbz399 = { .ztup0 = zcbz3100, .ztup1 = zCapPtrMode };
  return zcbz399;
}



static void finish_sailgen_ap_m_encdec_forwards(void)
{
}

static struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 sailgen_ap_m_encdec_forwards_infallible(uint64_t);

struct zArchPerms zghz395;
struct zArchPerms zghz396;
struct zArchPerms zghz397;
struct zArchPerms zghz398;
struct zArchPerms zghz399;
struct zArchPerms zghz3100;
struct zArchPerms zghz3101;
struct zArchPerms zghz3102;
struct zArchPerms zghz3103;
struct zArchPerms zghz3104;
struct zArchPerms zghz3105;
struct zArchPerms zghz3106;
struct zArchPerms zghz3107;
struct zArchPerms zghz3108;

static void startup_sailgen_ap_m_encdec_forwards_infallible(void)
{














}

static struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 sailgen_ap_m_encdec_forwards_infallible(uint64_t zargz3)
{
  __label__ case_211, case_210, case_209, case_208, case_207, case_206, case_205, case_204, case_203, case_202, case_201, case_200, case_199, case_198, finish_match_197, end_function_212, end_block_exception_213;

  struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zcbz348;
  {
    uint64_t zv__0;
    zv__0 = zargz3;
    bool zgsz3141;
    zgsz3141 = (zv__0 == UINT64_C(0b00000));
    if (!(zgsz3141)) {  goto case_211;  }
    enum zExecutionMode zgaz346;
    zgaz346 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    struct zArchPerms zghz3108;
    zghz3108.zASR = false;
    zghz3108.zC = false;
    zghz3108.zEL = false;
    zghz3108.zLM = false;
    zghz3108.zR = false;
    zghz3108.zSL = false;
    zghz3108.zW = false;
    zghz3108.zX = false;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz3140;
    zgsz3140.ztup0 = zghz3108;
    zgsz3140.ztup1 = zgaz346;
    zcbz348 = zgsz3140;
    goto finish_match_197;
  }
case_211: ;
  {
    uint64_t zv__3;
    zv__3 = zargz3;
    bool zgsz3138;
    zgsz3138 = (zv__3 == UINT64_C(0b00001));
    if (!(zgsz3138)) {  goto case_210;  }
    enum zExecutionMode zgaz347;
    zgaz347 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    struct zArchPerms zghz3107;
    zghz3107.zASR = false;
    zghz3107.zC = false;
    zghz3107.zEL = false;
    zghz3107.zLM = false;
    zghz3107.zR = true;
    zghz3107.zSL = false;
    zghz3107.zW = false;
    zghz3107.zX = false;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz3137;
    zgsz3137.ztup0 = zghz3107;
    zgsz3137.ztup1 = zgaz347;
    zcbz348 = zgsz3137;
    goto finish_match_197;
  }
case_210: ;
  {
    uint64_t zv__6;
    zv__6 = zargz3;
    bool zgsz3135;
    zgsz3135 = (zv__6 == UINT64_C(0b00100));
    if (!(zgsz3135)) {  goto case_209;  }
    enum zExecutionMode zgaz348;
    zgaz348 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    struct zArchPerms zghz3106;
    zghz3106.zASR = false;
    zghz3106.zC = false;
    zghz3106.zEL = false;
    zghz3106.zLM = false;
    zghz3106.zR = false;
    zghz3106.zSL = false;
    zghz3106.zW = true;
    zghz3106.zX = false;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz3134;
    zgsz3134.ztup0 = zghz3106;
    zgsz3134.ztup1 = zgaz348;
    zcbz348 = zgsz3134;
    goto finish_match_197;
  }
case_209: ;
  {
    uint64_t zv__9;
    zv__9 = zargz3;
    bool zgsz3132;
    zgsz3132 = (zv__9 == UINT64_C(0b00101));
    if (!(zgsz3132)) {  goto case_208;  }
    enum zExecutionMode zgaz349;
    zgaz349 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    struct zArchPerms zghz3105;
    zghz3105.zASR = false;
    zghz3105.zC = false;
    zghz3105.zEL = false;
    zghz3105.zLM = false;
    zghz3105.zR = true;
    zghz3105.zSL = false;
    zghz3105.zW = true;
    zghz3105.zX = false;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz3131;
    zgsz3131.ztup0 = zghz3105;
    zgsz3131.ztup1 = zgaz349;
    zcbz348 = zgsz3131;
    goto finish_match_197;
  }
case_208: ;
  {
    uint64_t zv__12;
    zv__12 = zargz3;
    uint64_t zgaz351;
    zgaz351 = (safe_rshift(UINT64_MAX, 64 - 4) & (zv__12 >> INT64_C(1)));
    bool zgsz3129;
    zgsz3129 = (zgaz351 == UINT64_C(0x4));
    if (!(zgsz3129)) {  goto case_207;  }
    uint64_t zm;
    zm = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__12 >> INT64_C(0)));
    enum zExecutionMode zgaz350;
    zgaz350 = sailgen_execution_mode_encdec_backwards(zm);
    struct zArchPerms zghz3104;
    zghz3104.zASR = true;
    zghz3104.zC = true;
    zghz3104.zEL = true;
    zghz3104.zLM = true;
    zghz3104.zR = true;
    zghz3104.zSL = true;
    zghz3104.zW = true;
    zghz3104.zX = true;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz3128;
    zgsz3128.ztup0 = zghz3104;
    zgsz3128.ztup1 = zgaz350;
    zcbz348 = zgsz3128;
    goto finish_match_197;
  }
case_207: ;
  {
    uint64_t zv__15;
    zv__15 = zargz3;
    uint64_t zgaz353;
    zgaz353 = (safe_rshift(UINT64_MAX, 64 - 4) & (zv__15 >> INT64_C(1)));
    bool zgsz3126;
    zgsz3126 = (zgaz353 == UINT64_C(0x5));
    if (!(zgsz3126)) {  goto case_206;  }
    uint64_t zuz35;
    zuz35 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__15 >> INT64_C(0)));
    enum zExecutionMode zgaz352;
    zgaz352 = sailgen_execution_mode_encdec_backwards(zuz35);
    struct zArchPerms zghz3103;
    zghz3103.zASR = false;
    zghz3103.zC = true;
    zghz3103.zEL = true;
    zghz3103.zLM = true;
    zghz3103.zR = true;
    zghz3103.zSL = true;
    zghz3103.zW = false;
    zghz3103.zX = true;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz3125;
    zgsz3125.ztup0 = zghz3103;
    zgsz3125.ztup1 = zgaz352;
    zcbz348 = zgsz3125;
    goto finish_match_197;
  }
case_206: ;
  {
    uint64_t zv__18;
    zv__18 = zargz3;
    uint64_t zgaz355;
    zgaz355 = (safe_rshift(UINT64_MAX, 64 - 4) & (zv__18 >> INT64_C(1)));
    bool zgsz3123;
    zgsz3123 = (zgaz355 == UINT64_C(0x6));
    if (!(zgsz3123)) {  goto case_205;  }
    uint64_t zuz36;
    zuz36 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__18 >> INT64_C(0)));
    enum zExecutionMode zgaz354;
    zgaz354 = sailgen_execution_mode_encdec_backwards(zuz36);
    struct zArchPerms zghz3102;
    zghz3102.zASR = false;
    zghz3102.zC = true;
    zghz3102.zEL = true;
    zghz3102.zLM = true;
    zghz3102.zR = true;
    zghz3102.zSL = true;
    zghz3102.zW = true;
    zghz3102.zX = true;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz3122;
    zgsz3122.ztup0 = zghz3102;
    zgsz3122.ztup1 = zgaz354;
    zcbz348 = zgsz3122;
    goto finish_match_197;
  }
case_205: ;
  {
    uint64_t zv__21;
    zv__21 = zargz3;
    uint64_t zgaz357;
    zgaz357 = (safe_rshift(UINT64_MAX, 64 - 4) & (zv__21 >> INT64_C(1)));
    bool zgsz3120;
    zgsz3120 = (zgaz357 == UINT64_C(0x7));
    if (!(zgsz3120)) {  goto case_204;  }
    uint64_t zuz37;
    zuz37 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__21 >> INT64_C(0)));
    enum zExecutionMode zgaz356;
    zgaz356 = sailgen_execution_mode_encdec_backwards(zuz37);
    struct zArchPerms zghz3101;
    zghz3101.zASR = false;
    zghz3101.zC = false;
    zghz3101.zEL = false;
    zghz3101.zLM = false;
    zghz3101.zR = true;
    zghz3101.zSL = false;
    zghz3101.zW = true;
    zghz3101.zX = true;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz3119;
    zgsz3119.ztup0 = zghz3101;
    zgsz3119.ztup1 = zgaz356;
    zcbz348 = zgsz3119;
    goto finish_match_197;
  }
case_204: ;
  {
    uint64_t zv__24;
    zv__24 = zargz3;
    bool zgsz3117;
    zgsz3117 = (zv__24 == UINT64_C(0b10011));
    if (!(zgsz3117)) {  goto case_203;  }
    enum zExecutionMode zgaz358;
    zgaz358 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    struct zArchPerms zghz3100;
    zghz3100.zASR = false;
    zghz3100.zC = true;
    zghz3100.zEL = false;
    zghz3100.zLM = false;
    zghz3100.zR = true;
    zghz3100.zSL = false;
    zghz3100.zW = false;
    zghz3100.zX = false;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz3116;
    zgsz3116.ztup0 = zghz3100;
    zgsz3116.ztup1 = zgaz358;
    zcbz348 = zgsz3116;
    goto finish_match_197;
  }
case_203: ;
  {
    uint64_t zv__27;
    zv__27 = zargz3;
    bool zgsz3114;
    zgsz3114 = (zv__27 == UINT64_C(0b10110));
    if (!(zgsz3114)) {  goto case_202;  }
    enum zExecutionMode zgaz359;
    zgaz359 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    struct zArchPerms zghz399;
    zghz399.zASR = false;
    zghz399.zC = true;
    zghz399.zEL = false;
    zghz399.zLM = true;
    zghz399.zR = true;
    zghz399.zSL = true;
    zghz399.zW = true;
    zghz399.zX = false;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz3113;
    zgsz3113.ztup0 = zghz399;
    zgsz3113.ztup1 = zgaz359;
    zcbz348 = zgsz3113;
    goto finish_match_197;
  }
case_202: ;
  {
    uint64_t zv__30;
    zv__30 = zargz3;
    bool zgsz3111;
    zgsz3111 = (zv__30 == UINT64_C(0b10111));
    if (!(zgsz3111)) {  goto case_201;  }
    enum zExecutionMode zgaz360;
    zgaz360 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    struct zArchPerms zghz398;
    zghz398.zASR = false;
    zghz398.zC = true;
    zghz398.zEL = false;
    zghz398.zLM = true;
    zghz398.zR = true;
    zghz398.zSL = false;
    zghz398.zW = true;
    zghz398.zX = false;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz3110;
    zgsz3110.ztup0 = zghz398;
    zgsz3110.ztup1 = zgaz360;
    zcbz348 = zgsz3110;
    goto finish_match_197;
  }
case_201: ;
  {
    uint64_t zv__33;
    zv__33 = zargz3;
    bool zgsz3108;
    zgsz3108 = (zv__33 == UINT64_C(0b11011));
    if (!(zgsz3108)) {  goto case_200;  }
    enum zExecutionMode zgaz361;
    zgaz361 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    struct zArchPerms zghz397;
    zghz397.zASR = false;
    zghz397.zC = true;
    zghz397.zEL = true;
    zghz397.zLM = true;
    zghz397.zR = true;
    zghz397.zSL = false;
    zghz397.zW = false;
    zghz397.zX = false;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz3107;
    zgsz3107.ztup0 = zghz397;
    zgsz3107.ztup1 = zgaz361;
    zcbz348 = zgsz3107;
    goto finish_match_197;
  }
case_200: ;
  {
    uint64_t zv__36;
    zv__36 = zargz3;
    bool zgsz3105;
    zgsz3105 = (zv__36 == UINT64_C(0b11110));
    if (!(zgsz3105)) {  goto case_199;  }
    enum zExecutionMode zgaz362;
    zgaz362 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    struct zArchPerms zghz396;
    zghz396.zASR = false;
    zghz396.zC = true;
    zghz396.zEL = true;
    zghz396.zLM = true;
    zghz396.zR = true;
    zghz396.zSL = true;
    zghz396.zW = true;
    zghz396.zX = false;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz3104;
    zgsz3104.ztup0 = zghz396;
    zgsz3104.ztup1 = zgaz362;
    zcbz348 = zgsz3104;
    goto finish_match_197;
  }
case_199: ;
  {
    uint64_t zv__39;
    zv__39 = zargz3;
    bool zgsz3102;
    zgsz3102 = (zv__39 == UINT64_C(0b11111));
    if (!(zgsz3102)) {  goto case_198;  }
    enum zExecutionMode zgaz363;
    zgaz363 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    struct zArchPerms zghz395;
    zghz395.zASR = false;
    zghz395.zC = true;
    zghz395.zEL = true;
    zghz395.zLM = true;
    zghz395.zR = true;
    zghz395.zSL = false;
    zghz395.zW = true;
    zghz395.zX = false;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz3101;
    zgsz3101.ztup0 = zghz395;
    zgsz3101.ztup1 = zgaz363;
    zcbz348 = zgsz3101;
    goto finish_match_197;
  }
case_198: ;
finish_match_197: ;
end_function_212: ;
  return zcbz348;
end_block_exception_213: ;
  struct zArchPerms zcbz3102 = { .zASR = false, .zC = false, .zEL = false, .zLM = false, .zR = false, .zSL = false, .zW = false, .zX = false };
  struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zcbz3101 = { .ztup0 = zcbz3102, .ztup1 = zCapPtrMode };
  return zcbz3101;
}



static void finish_sailgen_ap_m_encdec_forwards_infallible(void)
{
}

lbits zghz3109;
lbits zghz3110;
lbits zghz3111;
lbits zghz3112;
lbits zghz3113;
lbits zghz3114;
lbits zghz3115;
lbits zghz3116;
lbits zghz3117;
lbits zghz3118;
lbits zghz3119;
lbits zghz3120;
lbits zghz3121;
lbits zghz3122;

static void startup_sailgen_ap_m_encdec_backwards(void)
{
  CREATE(lbits)(&zghz3109);
  CREATE(lbits)(&zghz3110);
  CREATE(lbits)(&zghz3111);
  CREATE(lbits)(&zghz3112);
  CREATE(lbits)(&zghz3113);
  CREATE(lbits)(&zghz3114);
  CREATE(lbits)(&zghz3115);
  CREATE(lbits)(&zghz3116);
  CREATE(lbits)(&zghz3117);
  CREATE(lbits)(&zghz3118);
  CREATE(lbits)(&zghz3119);
  CREATE(lbits)(&zghz3120);
  CREATE(lbits)(&zghz3121);
  CREATE(lbits)(&zghz3122);
}

static uint64_t sailgen_ap_m_encdec_backwards(struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zargz3)
{
  __label__ case_339, case_222, finish_match_221, end_function_340, end_block_exception_341;

  uint64_t zcbz349;
  struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zhead_expz3;
  zhead_expz3 = zargz3;
  struct zoptionzIbzK zgaz373;
  CREATE(zoptionzIbzK)(&zgaz373);
  {
    __label__ case_217, case_216, finish_match_215;

    {
      __label__ case_220, case_219, finish_match_218;

      bool zp0z3;
      zp0z3 = zhead_expz3.ztup0.zR;
      bool zp1z3;
      zp1z3 = zhead_expz3.ztup0.zW;
      bool zp2z3;
      zp2z3 = zhead_expz3.ztup0.zC;
      bool zp3z3;
      zp3z3 = zhead_expz3.ztup0.zLM;
      bool zp4z3;
      zp4z3 = zhead_expz3.ztup0.zEL;
      bool zp5z3;
      zp5z3 = zhead_expz3.ztup0.zSL;
      bool zp6z3;
      zp6z3 = zhead_expz3.ztup0.zX;
      bool zp7z3;
      zp7z3 = zhead_expz3.ztup0.zASR;
      enum zExecutionMode zmapping0z3;
      zmapping0z3 = zhead_expz3.ztup1;
      bool zgaz372;
      {
        bool zgaz371;
        {
          bool zgaz370;
          {
            bool zgaz369;
            {
              bool zgaz368;
              {
                bool zgaz367;
                {
                  bool zgaz366;
                  {
                    bool zgaz365;
                    zgaz365 = sailgen_execution_mode_encdec_forwards_matches(zmapping0z3);
                    bool zgsz3144;
                    if (zgaz365) {  zgsz3144 = eq_bool(zp7z3, false);  } else {  zgsz3144 = false;  }
                    zgaz366 = zgsz3144;
                  }
                  bool zgsz3145;
                  if (zgaz366) {  zgsz3145 = eq_bool(zp6z3, false);  } else {  zgsz3145 = false;  }
                  zgaz367 = zgsz3145;
                }
                bool zgsz3146;
                if (zgaz367) {  zgsz3146 = eq_bool(zp5z3, false);  } else {  zgsz3146 = false;  }
                zgaz368 = zgsz3146;
              }
              bool zgsz3147;
              if (zgaz368) {  zgsz3147 = eq_bool(zp4z3, false);  } else {  zgsz3147 = false;  }
              zgaz369 = zgsz3147;
            }
            bool zgsz3148;
            if (zgaz369) {  zgsz3148 = eq_bool(zp3z3, false);  } else {  zgsz3148 = false;  }
            zgaz370 = zgsz3148;
          }
          bool zgsz3149;
          if (zgaz370) {  zgsz3149 = eq_bool(zp2z3, false);  } else {  zgsz3149 = false;  }
          zgaz371 = zgsz3149;
        }
        bool zgsz3150;
        if (zgaz371) {  zgsz3150 = eq_bool(zp1z3, false);  } else {  zgsz3150 = false;  }
        zgaz372 = zgsz3150;
      }
      bool zgsz3151;
      if (zgaz372) {  zgsz3151 = eq_bool(zp0z3, false);  } else {  zgsz3151 = false;  }
      bool zgsz3155;
      zgsz3155 = zgsz3151;
      if (!(zgsz3155)) {  goto case_217;  }
      uint64_t zgaz364;
      zgaz364 = sailgen_execution_mode_encdec_forwards_infallible(zmapping0z3);
      struct zoptionzIbzK zgsz3152;
      CREATE(zoptionzIbzK)(&zgsz3152);
      {
        uint64_t zb__9;
        zb__9 = zgaz364;
        bool zgsz3154;
        zgsz3154 = (zb__9 == UINT64_C(0b0));
        if (!(zgsz3154)) {  goto case_220;  }
        {
          RECREATE(lbits)(&zghz3122);
          CONVERT_OF(lbits, fbits)(&zghz3122, UINT64_C(0b00000), UINT64_C(5) , true);
          sailgen_SomezIbzK(&zgsz3152, zghz3122);
        }
        goto finish_match_218;
      }
    case_220: ;
      {
        sailgen_NonezIbzK(&zgsz3152, UNIT);
        goto finish_match_218;
      }
    case_219: ;
    finish_match_218: ;
      COPY(zoptionzIbzK)(&zgaz373, zgsz3152);
      KILL(zoptionzIbzK)(&zgsz3152);
      goto finish_match_215;
    }
  case_217: ;
    {
      sailgen_NonezIbzK(&zgaz373, UNIT);
      goto finish_match_215;
    }
  case_216: ;
  finish_match_215: ;
  }
  {
    if (zgaz373.kind != Kind_zSomezIbzK) goto case_339;
    uint64_t zresult;
    zresult = CONVERT_OF(fbits, lbits)(zgaz373.variants.zSomezIbzK, true);
    zcbz349 = zresult;
    goto finish_match_221;
  }
case_339: ;
  {
    __label__ case_338, case_230, finish_match_229;

    /* complete */
    struct zoptionzIbzK zgaz383;
    CREATE(zoptionzIbzK)(&zgaz383);
    {
      __label__ case_225, case_224, finish_match_223;

      {
        __label__ case_228, case_227, finish_match_226;

        bool zuz38;
        zuz38 = zhead_expz3.ztup0.zR;
        bool zuz39;
        zuz39 = zhead_expz3.ztup0.zW;
        bool zuz310;
        zuz310 = zhead_expz3.ztup0.zC;
        bool zuz311;
        zuz311 = zhead_expz3.ztup0.zLM;
        bool zuz312;
        zuz312 = zhead_expz3.ztup0.zEL;
        bool zuz313;
        zuz313 = zhead_expz3.ztup0.zSL;
        bool zuz314;
        zuz314 = zhead_expz3.ztup0.zX;
        bool zuz315;
        zuz315 = zhead_expz3.ztup0.zASR;
        enum zExecutionMode zmapping1z3;
        zmapping1z3 = zhead_expz3.ztup1;
        bool zgaz382;
        {
          bool zgaz381;
          {
            bool zgaz380;
            {
              bool zgaz379;
              {
                bool zgaz378;
                {
                  bool zgaz377;
                  {
                    bool zgaz376;
                    {
                      bool zgaz375;
                      zgaz375 = sailgen_execution_mode_encdec_forwards_matches(zmapping1z3);
                      bool zgsz3159;
                      if (zgaz375) {  zgsz3159 = eq_bool(zuz315, false);  } else {  zgsz3159 = false;  }
                      zgaz376 = zgsz3159;
                    }
                    bool zgsz3160;
                    if (zgaz376) {  zgsz3160 = eq_bool(zuz314, false);  } else {  zgsz3160 = false;  }
                    zgaz377 = zgsz3160;
                  }
                  bool zgsz3161;
                  if (zgaz377) {  zgsz3161 = eq_bool(zuz313, false);  } else {  zgsz3161 = false;  }
                  zgaz378 = zgsz3161;
                }
                bool zgsz3162;
                if (zgaz378) {  zgsz3162 = eq_bool(zuz312, false);  } else {  zgsz3162 = false;  }
                zgaz379 = zgsz3162;
              }
              bool zgsz3163;
              if (zgaz379) {  zgsz3163 = eq_bool(zuz311, false);  } else {  zgsz3163 = false;  }
              zgaz380 = zgsz3163;
            }
            bool zgsz3164;
            if (zgaz380) {  zgsz3164 = eq_bool(zuz310, false);  } else {  zgsz3164 = false;  }
            zgaz381 = zgsz3164;
          }
          bool zgsz3165;
          if (zgaz381) {  zgsz3165 = eq_bool(zuz39, false);  } else {  zgsz3165 = false;  }
          zgaz382 = zgsz3165;
        }
        bool zgsz3166;
        if (zgaz382) {  zgsz3166 = eq_bool(zuz38, true);  } else {  zgsz3166 = false;  }
        bool zgsz3170;
        zgsz3170 = zgsz3166;
        if (!(zgsz3170)) {  goto case_225;  }
        uint64_t zgaz374;
        zgaz374 = sailgen_execution_mode_encdec_forwards_infallible(zmapping1z3);
        struct zoptionzIbzK zgsz3167;
        CREATE(zoptionzIbzK)(&zgsz3167);
        {
          uint64_t zb__8;
          zb__8 = zgaz374;
          bool zgsz3169;
          zgsz3169 = (zb__8 == UINT64_C(0b0));
          if (!(zgsz3169)) {  goto case_228;  }
          {
            RECREATE(lbits)(&zghz3121);
            CONVERT_OF(lbits, fbits)(&zghz3121, UINT64_C(0b00001), UINT64_C(5) , true);
            sailgen_SomezIbzK(&zgsz3167, zghz3121);
          }
          goto finish_match_226;
        }
      case_228: ;
        {
          sailgen_NonezIbzK(&zgsz3167, UNIT);
          goto finish_match_226;
        }
      case_227: ;
      finish_match_226: ;
        COPY(zoptionzIbzK)(&zgaz383, zgsz3167);
        KILL(zoptionzIbzK)(&zgsz3167);
        goto finish_match_223;
      }
    case_225: ;
      {
        sailgen_NonezIbzK(&zgaz383, UNIT);
        goto finish_match_223;
      }
    case_224: ;
    finish_match_223: ;
    }
    uint64_t zgsz3171;
    {
      if (zgaz383.kind != Kind_zSomezIbzK) goto case_338;
      zgsz3171 = CONVERT_OF(fbits, lbits)(zgaz383.variants.zSomezIbzK, true);
      goto finish_match_229;
    }
  case_338: ;
    {
      __label__ case_337, case_238, finish_match_237;

      /* complete */
      struct zoptionzIbzK zgaz393;
      CREATE(zoptionzIbzK)(&zgaz393);
      {
        __label__ case_233, case_232, finish_match_231;

        {
          __label__ case_236, case_235, finish_match_234;

          bool zuz317;
          zuz317 = zhead_expz3.ztup0.zR;
          bool zuz318;
          zuz318 = zhead_expz3.ztup0.zW;
          bool zuz319;
          zuz319 = zhead_expz3.ztup0.zC;
          bool zuz320;
          zuz320 = zhead_expz3.ztup0.zLM;
          bool zuz321;
          zuz321 = zhead_expz3.ztup0.zEL;
          bool zuz322;
          zuz322 = zhead_expz3.ztup0.zSL;
          bool zuz323;
          zuz323 = zhead_expz3.ztup0.zX;
          bool zuz324;
          zuz324 = zhead_expz3.ztup0.zASR;
          enum zExecutionMode zmapping2z3;
          zmapping2z3 = zhead_expz3.ztup1;
          bool zgaz392;
          {
            bool zgaz391;
            {
              bool zgaz390;
              {
                bool zgaz389;
                {
                  bool zgaz388;
                  {
                    bool zgaz387;
                    {
                      bool zgaz386;
                      {
                        bool zgaz385;
                        zgaz385 = sailgen_execution_mode_encdec_forwards_matches(zmapping2z3);
                        bool zgsz3174;
                        if (zgaz385) {  zgsz3174 = eq_bool(zuz324, false);  } else {  zgsz3174 = false;  }
                        zgaz386 = zgsz3174;
                      }
                      bool zgsz3175;
                      if (zgaz386) {  zgsz3175 = eq_bool(zuz323, false);  } else {  zgsz3175 = false;  }
                      zgaz387 = zgsz3175;
                    }
                    bool zgsz3176;
                    if (zgaz387) {  zgsz3176 = eq_bool(zuz322, false);  } else {  zgsz3176 = false;  }
                    zgaz388 = zgsz3176;
                  }
                  bool zgsz3177;
                  if (zgaz388) {  zgsz3177 = eq_bool(zuz321, false);  } else {  zgsz3177 = false;  }
                  zgaz389 = zgsz3177;
                }
                bool zgsz3178;
                if (zgaz389) {  zgsz3178 = eq_bool(zuz320, false);  } else {  zgsz3178 = false;  }
                zgaz390 = zgsz3178;
              }
              bool zgsz3179;
              if (zgaz390) {  zgsz3179 = eq_bool(zuz319, false);  } else {  zgsz3179 = false;  }
              zgaz391 = zgsz3179;
            }
            bool zgsz3180;
            if (zgaz391) {  zgsz3180 = eq_bool(zuz318, true);  } else {  zgsz3180 = false;  }
            zgaz392 = zgsz3180;
          }
          bool zgsz3181;
          if (zgaz392) {  zgsz3181 = eq_bool(zuz317, false);  } else {  zgsz3181 = false;  }
          bool zgsz3185;
          zgsz3185 = zgsz3181;
          if (!(zgsz3185)) {  goto case_233;  }
          uint64_t zgaz384;
          zgaz384 = sailgen_execution_mode_encdec_forwards_infallible(zmapping2z3);
          struct zoptionzIbzK zgsz3182;
          CREATE(zoptionzIbzK)(&zgsz3182);
          {
            uint64_t zb__7;
            zb__7 = zgaz384;
            bool zgsz3184;
            zgsz3184 = (zb__7 == UINT64_C(0b0));
            if (!(zgsz3184)) {  goto case_236;  }
            {
              RECREATE(lbits)(&zghz3120);
              CONVERT_OF(lbits, fbits)(&zghz3120, UINT64_C(0b00100), UINT64_C(5) , true);
              sailgen_SomezIbzK(&zgsz3182, zghz3120);
            }
            goto finish_match_234;
          }
        case_236: ;
          {
            sailgen_NonezIbzK(&zgsz3182, UNIT);
            goto finish_match_234;
          }
        case_235: ;
        finish_match_234: ;
          COPY(zoptionzIbzK)(&zgaz393, zgsz3182);
          KILL(zoptionzIbzK)(&zgsz3182);
          goto finish_match_231;
        }
      case_233: ;
        {
          sailgen_NonezIbzK(&zgaz393, UNIT);
          goto finish_match_231;
        }
      case_232: ;
      finish_match_231: ;
      }
      uint64_t zgsz3186;
      {
        if (zgaz393.kind != Kind_zSomezIbzK) goto case_337;
        zgsz3186 = CONVERT_OF(fbits, lbits)(zgaz393.variants.zSomezIbzK, true);
        goto finish_match_237;
      }
    case_337: ;
      {
        __label__ case_336, case_246, finish_match_245;

        /* complete */
        struct zoptionzIbzK zgaz3103;
        CREATE(zoptionzIbzK)(&zgaz3103);
        {
          __label__ case_241, case_240, finish_match_239;

          {
            __label__ case_244, case_243, finish_match_242;

            bool zuz326;
            zuz326 = zhead_expz3.ztup0.zR;
            bool zuz327;
            zuz327 = zhead_expz3.ztup0.zW;
            bool zuz328;
            zuz328 = zhead_expz3.ztup0.zC;
            bool zuz329;
            zuz329 = zhead_expz3.ztup0.zLM;
            bool zuz330;
            zuz330 = zhead_expz3.ztup0.zEL;
            bool zuz331;
            zuz331 = zhead_expz3.ztup0.zSL;
            bool zuz332;
            zuz332 = zhead_expz3.ztup0.zX;
            bool zuz333;
            zuz333 = zhead_expz3.ztup0.zASR;
            enum zExecutionMode zmapping3z3;
            zmapping3z3 = zhead_expz3.ztup1;
            bool zgaz3102;
            {
              bool zgaz3101;
              {
                bool zgaz3100;
                {
                  bool zgaz399;
                  {
                    bool zgaz398;
                    {
                      bool zgaz397;
                      {
                        bool zgaz396;
                        {
                          bool zgaz395;
                          zgaz395 = sailgen_execution_mode_encdec_forwards_matches(zmapping3z3);
                          bool zgsz3189;
                          if (zgaz395) {  zgsz3189 = eq_bool(zuz333, false);  } else {  zgsz3189 = false;  }
                          zgaz396 = zgsz3189;
                        }
                        bool zgsz3190;
                        if (zgaz396) {  zgsz3190 = eq_bool(zuz332, false);  } else {  zgsz3190 = false;  }
                        zgaz397 = zgsz3190;
                      }
                      bool zgsz3191;
                      if (zgaz397) {  zgsz3191 = eq_bool(zuz331, false);  } else {  zgsz3191 = false;  }
                      zgaz398 = zgsz3191;
                    }
                    bool zgsz3192;
                    if (zgaz398) {  zgsz3192 = eq_bool(zuz330, false);  } else {  zgsz3192 = false;  }
                    zgaz399 = zgsz3192;
                  }
                  bool zgsz3193;
                  if (zgaz399) {  zgsz3193 = eq_bool(zuz329, false);  } else {  zgsz3193 = false;  }
                  zgaz3100 = zgsz3193;
                }
                bool zgsz3194;
                if (zgaz3100) {  zgsz3194 = eq_bool(zuz328, false);  } else {  zgsz3194 = false;  }
                zgaz3101 = zgsz3194;
              }
              bool zgsz3195;
              if (zgaz3101) {  zgsz3195 = eq_bool(zuz327, true);  } else {  zgsz3195 = false;  }
              zgaz3102 = zgsz3195;
            }
            bool zgsz3196;
            if (zgaz3102) {  zgsz3196 = eq_bool(zuz326, true);  } else {  zgsz3196 = false;  }
            bool zgsz3200;
            zgsz3200 = zgsz3196;
            if (!(zgsz3200)) {  goto case_241;  }
            uint64_t zgaz394;
            zgaz394 = sailgen_execution_mode_encdec_forwards_infallible(zmapping3z3);
            struct zoptionzIbzK zgsz3197;
            CREATE(zoptionzIbzK)(&zgsz3197);
            {
              uint64_t zb__6;
              zb__6 = zgaz394;
              bool zgsz3199;
              zgsz3199 = (zb__6 == UINT64_C(0b0));
              if (!(zgsz3199)) {  goto case_244;  }
              {
                RECREATE(lbits)(&zghz3119);
                CONVERT_OF(lbits, fbits)(&zghz3119, UINT64_C(0b00101), UINT64_C(5) , true);
                sailgen_SomezIbzK(&zgsz3197, zghz3119);
              }
              goto finish_match_242;
            }
          case_244: ;
            {
              sailgen_NonezIbzK(&zgsz3197, UNIT);
              goto finish_match_242;
            }
          case_243: ;
          finish_match_242: ;
            COPY(zoptionzIbzK)(&zgaz3103, zgsz3197);
            KILL(zoptionzIbzK)(&zgsz3197);
            goto finish_match_239;
          }
        case_241: ;
          {
            sailgen_NonezIbzK(&zgaz3103, UNIT);
            goto finish_match_239;
          }
        case_240: ;
        finish_match_239: ;
        }
        uint64_t zgsz3201;
        {
          if (zgaz3103.kind != Kind_zSomezIbzK) goto case_336;
          zgsz3201 = CONVERT_OF(fbits, lbits)(zgaz3103.variants.zSomezIbzK, true);
          goto finish_match_245;
        }
      case_336: ;
        {
          __label__ case_335, case_254, finish_match_253;

          /* complete */
          struct zoptionzIbzK zgaz3116;
          CREATE(zoptionzIbzK)(&zgaz3116);
          {
            __label__ case_249, case_248, finish_match_247;

            {
              __label__ case_252, case_251, finish_match_250;

              bool zuz335;
              zuz335 = zhead_expz3.ztup0.zR;
              bool zuz336;
              zuz336 = zhead_expz3.ztup0.zW;
              bool zuz337;
              zuz337 = zhead_expz3.ztup0.zC;
              bool zuz338;
              zuz338 = zhead_expz3.ztup0.zLM;
              bool zuz339;
              zuz339 = zhead_expz3.ztup0.zEL;
              bool zuz340;
              zuz340 = zhead_expz3.ztup0.zSL;
              bool zuz341;
              zuz341 = zhead_expz3.ztup0.zX;
              bool zuz342;
              zuz342 = zhead_expz3.ztup0.zASR;
              enum zExecutionMode zmapping4z3;
              zmapping4z3 = zhead_expz3.ztup1;
              bool zgaz3115;
              {
                bool zgaz3114;
                {
                  bool zgaz3113;
                  {
                    bool zgaz3112;
                    {
                      bool zgaz3111;
                      {
                        bool zgaz3110;
                        {
                          bool zgaz3109;
                          {
                            bool zgaz3108;
                            zgaz3108 = sailgen_execution_mode_encdec_forwards_matches(zmapping4z3);
                            bool zgsz3204;
                            if (zgaz3108) {  zgsz3204 = eq_bool(zuz342, true);  } else {  zgsz3204 = false;  }
                            zgaz3109 = zgsz3204;
                          }
                          bool zgsz3205;
                          if (zgaz3109) {  zgsz3205 = eq_bool(zuz341, true);  } else {  zgsz3205 = false;  }
                          zgaz3110 = zgsz3205;
                        }
                        bool zgsz3206;
                        if (zgaz3110) {  zgsz3206 = eq_bool(zuz340, true);  } else {  zgsz3206 = false;  }
                        zgaz3111 = zgsz3206;
                      }
                      bool zgsz3207;
                      if (zgaz3111) {  zgsz3207 = eq_bool(zuz339, true);  } else {  zgsz3207 = false;  }
                      zgaz3112 = zgsz3207;
                    }
                    bool zgsz3208;
                    if (zgaz3112) {  zgsz3208 = eq_bool(zuz338, true);  } else {  zgsz3208 = false;  }
                    zgaz3113 = zgsz3208;
                  }
                  bool zgsz3209;
                  if (zgaz3113) {  zgsz3209 = eq_bool(zuz337, true);  } else {  zgsz3209 = false;  }
                  zgaz3114 = zgsz3209;
                }
                bool zgsz3210;
                if (zgaz3114) {  zgsz3210 = eq_bool(zuz336, true);  } else {  zgsz3210 = false;  }
                zgaz3115 = zgsz3210;
              }
              bool zgsz3211;
              if (zgaz3115) {  zgsz3211 = eq_bool(zuz335, true);  } else {  zgsz3211 = false;  }
              bool zgsz3215;
              zgsz3215 = zgsz3211;
              if (!(zgsz3215)) {  goto case_249;  }
              uint64_t zgaz3104;
              zgaz3104 = sailgen_execution_mode_encdec_forwards_infallible(zmapping4z3);
              struct zoptionzIbzK zgsz3212;
              CREATE(zoptionzIbzK)(&zgsz3212);
              {
                uint64_t zm;
                zm = zgaz3104;
                uint64_t zgaz3107;
                {
                  uint64_t zgaz3106;
                  {
                    uint64_t zgaz3105;
                    zgaz3105 = zm;
                    zgaz3106 = (UINT64_C(0b00) << 1) | zgaz3105;
                  }
                  zgaz3107 = (UINT64_C(0b01) << 3) | zgaz3106;
                }
                {
                  RECREATE(lbits)(&zghz3118);
                  CONVERT_OF(lbits, fbits)(&zghz3118, zgaz3107, UINT64_C(5) , true);
                  sailgen_SomezIbzK(&zgsz3212, zghz3118);
                }
                goto finish_match_250;
              }
            case_252: ;
              {
                sailgen_NonezIbzK(&zgsz3212, UNIT);
                goto finish_match_250;
              }
            case_251: ;
            finish_match_250: ;
              COPY(zoptionzIbzK)(&zgaz3116, zgsz3212);
              KILL(zoptionzIbzK)(&zgsz3212);
              goto finish_match_247;
            }
          case_249: ;
            {
              sailgen_NonezIbzK(&zgaz3116, UNIT);
              goto finish_match_247;
            }
          case_248: ;
          finish_match_247: ;
          }
          uint64_t zgsz3216;
          {
            if (zgaz3116.kind != Kind_zSomezIbzK) goto case_335;
            zgsz3216 = CONVERT_OF(fbits, lbits)(zgaz3116.variants.zSomezIbzK, true);
            goto finish_match_253;
          }
        case_335: ;
          {
            __label__ case_334, case_262, finish_match_261;

            /* complete */
            struct zoptionzIbzK zgaz3129;
            CREATE(zoptionzIbzK)(&zgaz3129);
            {
              __label__ case_257, case_256, finish_match_255;

              {
                __label__ case_260, case_259, finish_match_258;

                bool zuz344;
                zuz344 = zhead_expz3.ztup0.zR;
                bool zuz345;
                zuz345 = zhead_expz3.ztup0.zW;
                bool zuz346;
                zuz346 = zhead_expz3.ztup0.zC;
                bool zuz347;
                zuz347 = zhead_expz3.ztup0.zLM;
                bool zuz348;
                zuz348 = zhead_expz3.ztup0.zEL;
                bool zuz349;
                zuz349 = zhead_expz3.ztup0.zSL;
                bool zuz350;
                zuz350 = zhead_expz3.ztup0.zX;
                bool zuz351;
                zuz351 = zhead_expz3.ztup0.zASR;
                enum zExecutionMode zmapping5z3;
                zmapping5z3 = zhead_expz3.ztup1;
                bool zgaz3128;
                {
                  bool zgaz3127;
                  {
                    bool zgaz3126;
                    {
                      bool zgaz3125;
                      {
                        bool zgaz3124;
                        {
                          bool zgaz3123;
                          {
                            bool zgaz3122;
                            {
                              bool zgaz3121;
                              zgaz3121 = sailgen_execution_mode_encdec_forwards_matches(zmapping5z3);
                              bool zgsz3219;
                              if (zgaz3121) {  zgsz3219 = eq_bool(zuz351, false);  } else {  zgsz3219 = false;  }
                              zgaz3122 = zgsz3219;
                            }
                            bool zgsz3220;
                            if (zgaz3122) {  zgsz3220 = eq_bool(zuz350, true);  } else {  zgsz3220 = false;  }
                            zgaz3123 = zgsz3220;
                          }
                          bool zgsz3221;
                          if (zgaz3123) {  zgsz3221 = eq_bool(zuz349, true);  } else {  zgsz3221 = false;  }
                          zgaz3124 = zgsz3221;
                        }
                        bool zgsz3222;
                        if (zgaz3124) {  zgsz3222 = eq_bool(zuz348, true);  } else {  zgsz3222 = false;  }
                        zgaz3125 = zgsz3222;
                      }
                      bool zgsz3223;
                      if (zgaz3125) {  zgsz3223 = eq_bool(zuz347, true);  } else {  zgsz3223 = false;  }
                      zgaz3126 = zgsz3223;
                    }
                    bool zgsz3224;
                    if (zgaz3126) {  zgsz3224 = eq_bool(zuz346, true);  } else {  zgsz3224 = false;  }
                    zgaz3127 = zgsz3224;
                  }
                  bool zgsz3225;
                  if (zgaz3127) {  zgsz3225 = eq_bool(zuz345, false);  } else {  zgsz3225 = false;  }
                  zgaz3128 = zgsz3225;
                }
                bool zgsz3226;
                if (zgaz3128) {  zgsz3226 = eq_bool(zuz344, true);  } else {  zgsz3226 = false;  }
                bool zgsz3230;
                zgsz3230 = zgsz3226;
                if (!(zgsz3230)) {  goto case_257;  }
                uint64_t zgaz3117;
                zgaz3117 = sailgen_execution_mode_encdec_forwards_infallible(zmapping5z3);
                struct zoptionzIbzK zgsz3227;
                CREATE(zoptionzIbzK)(&zgsz3227);
                {
                  uint64_t zuz352;
                  zuz352 = zgaz3117;
                  uint64_t zgaz3120;
                  {
                    uint64_t zgaz3119;
                    {
                      uint64_t zgaz3118;
                      zgaz3118 = zuz352;
                      zgaz3119 = (UINT64_C(0b01) << 1) | zgaz3118;
                    }
                    zgaz3120 = (UINT64_C(0b01) << 3) | zgaz3119;
                  }
                  {
                    RECREATE(lbits)(&zghz3117);
                    CONVERT_OF(lbits, fbits)(&zghz3117, zgaz3120, UINT64_C(5) , true);
                    sailgen_SomezIbzK(&zgsz3227, zghz3117);
                  }
                  goto finish_match_258;
                }
              case_260: ;
                {
                  sailgen_NonezIbzK(&zgsz3227, UNIT);
                  goto finish_match_258;
                }
              case_259: ;
              finish_match_258: ;
                COPY(zoptionzIbzK)(&zgaz3129, zgsz3227);
                KILL(zoptionzIbzK)(&zgsz3227);
                goto finish_match_255;
              }
            case_257: ;
              {
                sailgen_NonezIbzK(&zgaz3129, UNIT);
                goto finish_match_255;
              }
            case_256: ;
            finish_match_255: ;
            }
            uint64_t zgsz3231;
            {
              if (zgaz3129.kind != Kind_zSomezIbzK) goto case_334;
              zgsz3231 = CONVERT_OF(fbits, lbits)(zgaz3129.variants.zSomezIbzK, true);
              goto finish_match_261;
            }
          case_334: ;
            {
              __label__ case_333, case_270, finish_match_269;

              /* complete */
              struct zoptionzIbzK zgaz3142;
              CREATE(zoptionzIbzK)(&zgaz3142);
              {
                __label__ case_265, case_264, finish_match_263;

                {
                  __label__ case_268, case_267, finish_match_266;

                  bool zuz354;
                  zuz354 = zhead_expz3.ztup0.zR;
                  bool zuz355;
                  zuz355 = zhead_expz3.ztup0.zW;
                  bool zuz356;
                  zuz356 = zhead_expz3.ztup0.zC;
                  bool zuz357;
                  zuz357 = zhead_expz3.ztup0.zLM;
                  bool zuz358;
                  zuz358 = zhead_expz3.ztup0.zEL;
                  bool zuz359;
                  zuz359 = zhead_expz3.ztup0.zSL;
                  bool zuz360;
                  zuz360 = zhead_expz3.ztup0.zX;
                  bool zuz361;
                  zuz361 = zhead_expz3.ztup0.zASR;
                  enum zExecutionMode zmapping6z3;
                  zmapping6z3 = zhead_expz3.ztup1;
                  bool zgaz3141;
                  {
                    bool zgaz3140;
                    {
                      bool zgaz3139;
                      {
                        bool zgaz3138;
                        {
                          bool zgaz3137;
                          {
                            bool zgaz3136;
                            {
                              bool zgaz3135;
                              {
                                bool zgaz3134;
                                zgaz3134 = sailgen_execution_mode_encdec_forwards_matches(zmapping6z3);
                                bool zgsz3234;
                                if (zgaz3134) {  zgsz3234 = eq_bool(zuz361, false);  } else {  zgsz3234 = false;  }
                                zgaz3135 = zgsz3234;
                              }
                              bool zgsz3235;
                              if (zgaz3135) {  zgsz3235 = eq_bool(zuz360, true);  } else {  zgsz3235 = false;  }
                              zgaz3136 = zgsz3235;
                            }
                            bool zgsz3236;
                            if (zgaz3136) {  zgsz3236 = eq_bool(zuz359, true);  } else {  zgsz3236 = false;  }
                            zgaz3137 = zgsz3236;
                          }
                          bool zgsz3237;
                          if (zgaz3137) {  zgsz3237 = eq_bool(zuz358, true);  } else {  zgsz3237 = false;  }
                          zgaz3138 = zgsz3237;
                        }
                        bool zgsz3238;
                        if (zgaz3138) {  zgsz3238 = eq_bool(zuz357, true);  } else {  zgsz3238 = false;  }
                        zgaz3139 = zgsz3238;
                      }
                      bool zgsz3239;
                      if (zgaz3139) {  zgsz3239 = eq_bool(zuz356, true);  } else {  zgsz3239 = false;  }
                      zgaz3140 = zgsz3239;
                    }
                    bool zgsz3240;
                    if (zgaz3140) {  zgsz3240 = eq_bool(zuz355, true);  } else {  zgsz3240 = false;  }
                    zgaz3141 = zgsz3240;
                  }
                  bool zgsz3241;
                  if (zgaz3141) {  zgsz3241 = eq_bool(zuz354, true);  } else {  zgsz3241 = false;  }
                  bool zgsz3245;
                  zgsz3245 = zgsz3241;
                  if (!(zgsz3245)) {  goto case_265;  }
                  uint64_t zgaz3130;
                  zgaz3130 = sailgen_execution_mode_encdec_forwards_infallible(zmapping6z3);
                  struct zoptionzIbzK zgsz3242;
                  CREATE(zoptionzIbzK)(&zgsz3242);
                  {
                    uint64_t zuz362;
                    zuz362 = zgaz3130;
                    uint64_t zgaz3133;
                    {
                      uint64_t zgaz3132;
                      {
                        uint64_t zgaz3131;
                        zgaz3131 = zuz362;
                        zgaz3132 = (UINT64_C(0b10) << 1) | zgaz3131;
                      }
                      zgaz3133 = (UINT64_C(0b01) << 3) | zgaz3132;
                    }
                    {
                      RECREATE(lbits)(&zghz3116);
                      CONVERT_OF(lbits, fbits)(&zghz3116, zgaz3133, UINT64_C(5) , true);
                      sailgen_SomezIbzK(&zgsz3242, zghz3116);
                    }
                    goto finish_match_266;
                  }
                case_268: ;
                  {
                    sailgen_NonezIbzK(&zgsz3242, UNIT);
                    goto finish_match_266;
                  }
                case_267: ;
                finish_match_266: ;
                  COPY(zoptionzIbzK)(&zgaz3142, zgsz3242);
                  KILL(zoptionzIbzK)(&zgsz3242);
                  goto finish_match_263;
                }
              case_265: ;
                {
                  sailgen_NonezIbzK(&zgaz3142, UNIT);
                  goto finish_match_263;
                }
              case_264: ;
              finish_match_263: ;
              }
              uint64_t zgsz3246;
              {
                if (zgaz3142.kind != Kind_zSomezIbzK) goto case_333;
                zgsz3246 = CONVERT_OF(fbits, lbits)(zgaz3142.variants.zSomezIbzK, true);
                goto finish_match_269;
              }
            case_333: ;
              {
                __label__ case_332, case_278, finish_match_277;

                /* complete */
                struct zoptionzIbzK zgaz3155;
                CREATE(zoptionzIbzK)(&zgaz3155);
                {
                  __label__ case_273, case_272, finish_match_271;

                  {
                    __label__ case_276, case_275, finish_match_274;

                    bool zuz364;
                    zuz364 = zhead_expz3.ztup0.zR;
                    bool zuz365;
                    zuz365 = zhead_expz3.ztup0.zW;
                    bool zuz366;
                    zuz366 = zhead_expz3.ztup0.zC;
                    bool zuz367;
                    zuz367 = zhead_expz3.ztup0.zLM;
                    bool zuz368;
                    zuz368 = zhead_expz3.ztup0.zEL;
                    bool zuz369;
                    zuz369 = zhead_expz3.ztup0.zSL;
                    bool zuz370;
                    zuz370 = zhead_expz3.ztup0.zX;
                    bool zuz371;
                    zuz371 = zhead_expz3.ztup0.zASR;
                    enum zExecutionMode zmapping7z3;
                    zmapping7z3 = zhead_expz3.ztup1;
                    bool zgaz3154;
                    {
                      bool zgaz3153;
                      {
                        bool zgaz3152;
                        {
                          bool zgaz3151;
                          {
                            bool zgaz3150;
                            {
                              bool zgaz3149;
                              {
                                bool zgaz3148;
                                {
                                  bool zgaz3147;
                                  zgaz3147 = sailgen_execution_mode_encdec_forwards_matches(zmapping7z3);
                                  bool zgsz3249;
                                  if (zgaz3147) {  zgsz3249 = eq_bool(zuz371, false);  } else {  zgsz3249 = false;  }
                                  zgaz3148 = zgsz3249;
                                }
                                bool zgsz3250;
                                if (zgaz3148) {  zgsz3250 = eq_bool(zuz370, true);  } else {  zgsz3250 = false;  }
                                zgaz3149 = zgsz3250;
                              }
                              bool zgsz3251;
                              if (zgaz3149) {  zgsz3251 = eq_bool(zuz369, false);  } else {  zgsz3251 = false;  }
                              zgaz3150 = zgsz3251;
                            }
                            bool zgsz3252;
                            if (zgaz3150) {  zgsz3252 = eq_bool(zuz368, false);  } else {  zgsz3252 = false;  }
                            zgaz3151 = zgsz3252;
                          }
                          bool zgsz3253;
                          if (zgaz3151) {  zgsz3253 = eq_bool(zuz367, false);  } else {  zgsz3253 = false;  }
                          zgaz3152 = zgsz3253;
                        }
                        bool zgsz3254;
                        if (zgaz3152) {  zgsz3254 = eq_bool(zuz366, false);  } else {  zgsz3254 = false;  }
                        zgaz3153 = zgsz3254;
                      }
                      bool zgsz3255;
                      if (zgaz3153) {  zgsz3255 = eq_bool(zuz365, true);  } else {  zgsz3255 = false;  }
                      zgaz3154 = zgsz3255;
                    }
                    bool zgsz3256;
                    if (zgaz3154) {  zgsz3256 = eq_bool(zuz364, true);  } else {  zgsz3256 = false;  }
                    bool zgsz3260;
                    zgsz3260 = zgsz3256;
                    if (!(zgsz3260)) {  goto case_273;  }
                    uint64_t zgaz3143;
                    zgaz3143 = sailgen_execution_mode_encdec_forwards_infallible(zmapping7z3);
                    struct zoptionzIbzK zgsz3257;
                    CREATE(zoptionzIbzK)(&zgsz3257);
                    {
                      uint64_t zuz372;
                      zuz372 = zgaz3143;
                      uint64_t zgaz3146;
                      {
                        uint64_t zgaz3145;
                        {
                          uint64_t zgaz3144;
                          zgaz3144 = zuz372;
                          zgaz3145 = (UINT64_C(0b11) << 1) | zgaz3144;
                        }
                        zgaz3146 = (UINT64_C(0b01) << 3) | zgaz3145;
                      }
                      {
                        RECREATE(lbits)(&zghz3115);
                        CONVERT_OF(lbits, fbits)(&zghz3115, zgaz3146, UINT64_C(5) , true);
                        sailgen_SomezIbzK(&zgsz3257, zghz3115);
                      }
                      goto finish_match_274;
                    }
                  case_276: ;
                    {
                      sailgen_NonezIbzK(&zgsz3257, UNIT);
                      goto finish_match_274;
                    }
                  case_275: ;
                  finish_match_274: ;
                    COPY(zoptionzIbzK)(&zgaz3155, zgsz3257);
                    KILL(zoptionzIbzK)(&zgsz3257);
                    goto finish_match_271;
                  }
                case_273: ;
                  {
                    sailgen_NonezIbzK(&zgaz3155, UNIT);
                    goto finish_match_271;
                  }
                case_272: ;
                finish_match_271: ;
                }
                uint64_t zgsz3261;
                {
                  if (zgaz3155.kind != Kind_zSomezIbzK) goto case_332;
                  zgsz3261 = CONVERT_OF(fbits, lbits)(zgaz3155.variants.zSomezIbzK, true);
                  goto finish_match_277;
                }
              case_332: ;
                {
                  __label__ case_331, case_286, finish_match_285;

                  /* complete */
                  struct zoptionzIbzK zgaz3165;
                  CREATE(zoptionzIbzK)(&zgaz3165);
                  {
                    __label__ case_281, case_280, finish_match_279;

                    {
                      __label__ case_284, case_283, finish_match_282;

                      bool zuz374;
                      zuz374 = zhead_expz3.ztup0.zR;
                      bool zuz375;
                      zuz375 = zhead_expz3.ztup0.zW;
                      bool zuz376;
                      zuz376 = zhead_expz3.ztup0.zC;
                      bool zuz377;
                      zuz377 = zhead_expz3.ztup0.zLM;
                      bool zuz378;
                      zuz378 = zhead_expz3.ztup0.zEL;
                      bool zuz379;
                      zuz379 = zhead_expz3.ztup0.zSL;
                      bool zuz380;
                      zuz380 = zhead_expz3.ztup0.zX;
                      bool zuz381;
                      zuz381 = zhead_expz3.ztup0.zASR;
                      enum zExecutionMode zmapping8z3;
                      zmapping8z3 = zhead_expz3.ztup1;
                      bool zgaz3164;
                      {
                        bool zgaz3163;
                        {
                          bool zgaz3162;
                          {
                            bool zgaz3161;
                            {
                              bool zgaz3160;
                              {
                                bool zgaz3159;
                                {
                                  bool zgaz3158;
                                  {
                                    bool zgaz3157;
                                    zgaz3157 = sailgen_execution_mode_encdec_forwards_matches(zmapping8z3);
                                    bool zgsz3264;
                                    if (zgaz3157) {  zgsz3264 = eq_bool(zuz381, false);  } else {  zgsz3264 = false;  }
                                    zgaz3158 = zgsz3264;
                                  }
                                  bool zgsz3265;
                                  if (zgaz3158) {  zgsz3265 = eq_bool(zuz380, false);  } else {  zgsz3265 = false;  }
                                  zgaz3159 = zgsz3265;
                                }
                                bool zgsz3266;
                                if (zgaz3159) {  zgsz3266 = eq_bool(zuz379, false);  } else {  zgsz3266 = false;  }
                                zgaz3160 = zgsz3266;
                              }
                              bool zgsz3267;
                              if (zgaz3160) {  zgsz3267 = eq_bool(zuz378, false);  } else {  zgsz3267 = false;  }
                              zgaz3161 = zgsz3267;
                            }
                            bool zgsz3268;
                            if (zgaz3161) {  zgsz3268 = eq_bool(zuz377, false);  } else {  zgsz3268 = false;  }
                            zgaz3162 = zgsz3268;
                          }
                          bool zgsz3269;
                          if (zgaz3162) {  zgsz3269 = eq_bool(zuz376, true);  } else {  zgsz3269 = false;  }
                          zgaz3163 = zgsz3269;
                        }
                        bool zgsz3270;
                        if (zgaz3163) {  zgsz3270 = eq_bool(zuz375, false);  } else {  zgsz3270 = false;  }
                        zgaz3164 = zgsz3270;
                      }
                      bool zgsz3271;
                      if (zgaz3164) {  zgsz3271 = eq_bool(zuz374, true);  } else {  zgsz3271 = false;  }
                      bool zgsz3275;
                      zgsz3275 = zgsz3271;
                      if (!(zgsz3275)) {  goto case_281;  }
                      uint64_t zgaz3156;
                      zgaz3156 = sailgen_execution_mode_encdec_forwards_infallible(zmapping8z3);
                      struct zoptionzIbzK zgsz3272;
                      CREATE(zoptionzIbzK)(&zgsz3272);
                      {
                        uint64_t zb__5;
                        zb__5 = zgaz3156;
                        bool zgsz3274;
                        zgsz3274 = (zb__5 == UINT64_C(0b0));
                        if (!(zgsz3274)) {  goto case_284;  }
                        {
                          RECREATE(lbits)(&zghz3114);
                          CONVERT_OF(lbits, fbits)(&zghz3114, UINT64_C(0b10011), UINT64_C(5) , true);
                          sailgen_SomezIbzK(&zgsz3272, zghz3114);
                        }
                        goto finish_match_282;
                      }
                    case_284: ;
                      {
                        sailgen_NonezIbzK(&zgsz3272, UNIT);
                        goto finish_match_282;
                      }
                    case_283: ;
                    finish_match_282: ;
                      COPY(zoptionzIbzK)(&zgaz3165, zgsz3272);
                      KILL(zoptionzIbzK)(&zgsz3272);
                      goto finish_match_279;
                    }
                  case_281: ;
                    {
                      sailgen_NonezIbzK(&zgaz3165, UNIT);
                      goto finish_match_279;
                    }
                  case_280: ;
                  finish_match_279: ;
                  }
                  uint64_t zgsz3276;
                  {
                    if (zgaz3165.kind != Kind_zSomezIbzK) goto case_331;
                    zgsz3276 = CONVERT_OF(fbits, lbits)(zgaz3165.variants.zSomezIbzK, true);
                    goto finish_match_285;
                  }
                case_331: ;
                  {
                    __label__ case_330, case_294, finish_match_293;

                    /* complete */
                    struct zoptionzIbzK zgaz3175;
                    CREATE(zoptionzIbzK)(&zgaz3175);
                    {
                      __label__ case_289, case_288, finish_match_287;

                      {
                        __label__ case_292, case_291, finish_match_290;

                        bool zuz383;
                        zuz383 = zhead_expz3.ztup0.zR;
                        bool zuz384;
                        zuz384 = zhead_expz3.ztup0.zW;
                        bool zuz385;
                        zuz385 = zhead_expz3.ztup0.zC;
                        bool zuz386;
                        zuz386 = zhead_expz3.ztup0.zLM;
                        bool zuz387;
                        zuz387 = zhead_expz3.ztup0.zEL;
                        bool zuz388;
                        zuz388 = zhead_expz3.ztup0.zSL;
                        bool zuz389;
                        zuz389 = zhead_expz3.ztup0.zX;
                        bool zuz390;
                        zuz390 = zhead_expz3.ztup0.zASR;
                        enum zExecutionMode zmapping9z3;
                        zmapping9z3 = zhead_expz3.ztup1;
                        bool zgaz3174;
                        {
                          bool zgaz3173;
                          {
                            bool zgaz3172;
                            {
                              bool zgaz3171;
                              {
                                bool zgaz3170;
                                {
                                  bool zgaz3169;
                                  {
                                    bool zgaz3168;
                                    {
                                      bool zgaz3167;
                                      zgaz3167 = sailgen_execution_mode_encdec_forwards_matches(zmapping9z3);
                                      bool zgsz3279;
                                      if (zgaz3167) {  zgsz3279 = eq_bool(zuz390, false);  } else {
                                        zgsz3279 = false;
                                      }
                                      zgaz3168 = zgsz3279;
                                    }
                                    bool zgsz3280;
                                    if (zgaz3168) {  zgsz3280 = eq_bool(zuz389, false);  } else {  zgsz3280 = false;  }
                                    zgaz3169 = zgsz3280;
                                  }
                                  bool zgsz3281;
                                  if (zgaz3169) {  zgsz3281 = eq_bool(zuz388, true);  } else {  zgsz3281 = false;  }
                                  zgaz3170 = zgsz3281;
                                }
                                bool zgsz3282;
                                if (zgaz3170) {  zgsz3282 = eq_bool(zuz387, false);  } else {  zgsz3282 = false;  }
                                zgaz3171 = zgsz3282;
                              }
                              bool zgsz3283;
                              if (zgaz3171) {  zgsz3283 = eq_bool(zuz386, true);  } else {  zgsz3283 = false;  }
                              zgaz3172 = zgsz3283;
                            }
                            bool zgsz3284;
                            if (zgaz3172) {  zgsz3284 = eq_bool(zuz385, true);  } else {  zgsz3284 = false;  }
                            zgaz3173 = zgsz3284;
                          }
                          bool zgsz3285;
                          if (zgaz3173) {  zgsz3285 = eq_bool(zuz384, true);  } else {  zgsz3285 = false;  }
                          zgaz3174 = zgsz3285;
                        }
                        bool zgsz3286;
                        if (zgaz3174) {  zgsz3286 = eq_bool(zuz383, true);  } else {  zgsz3286 = false;  }
                        bool zgsz3290;
                        zgsz3290 = zgsz3286;
                        if (!(zgsz3290)) {  goto case_289;  }
                        uint64_t zgaz3166;
                        zgaz3166 = sailgen_execution_mode_encdec_forwards_infallible(zmapping9z3);
                        struct zoptionzIbzK zgsz3287;
                        CREATE(zoptionzIbzK)(&zgsz3287);
                        {
                          uint64_t zb__4;
                          zb__4 = zgaz3166;
                          bool zgsz3289;
                          zgsz3289 = (zb__4 == UINT64_C(0b0));
                          if (!(zgsz3289)) {  goto case_292;  }
                          {
                            RECREATE(lbits)(&zghz3113);
                            CONVERT_OF(lbits, fbits)(&zghz3113, UINT64_C(0b10110), UINT64_C(5) , true);
                            sailgen_SomezIbzK(&zgsz3287, zghz3113);
                          }
                          goto finish_match_290;
                        }
                      case_292: ;
                        {
                          sailgen_NonezIbzK(&zgsz3287, UNIT);
                          goto finish_match_290;
                        }
                      case_291: ;
                      finish_match_290: ;
                        COPY(zoptionzIbzK)(&zgaz3175, zgsz3287);
                        KILL(zoptionzIbzK)(&zgsz3287);
                        goto finish_match_287;
                      }
                    case_289: ;
                      {
                        sailgen_NonezIbzK(&zgaz3175, UNIT);
                        goto finish_match_287;
                      }
                    case_288: ;
                    finish_match_287: ;
                    }
                    uint64_t zgsz3291;
                    {
                      if (zgaz3175.kind != Kind_zSomezIbzK) goto case_330;
                      zgsz3291 = CONVERT_OF(fbits, lbits)(zgaz3175.variants.zSomezIbzK, true);
                      goto finish_match_293;
                    }
                  case_330: ;
                    {
                      __label__ case_329, case_302, finish_match_301;

                      /* complete */
                      struct zoptionzIbzK zgaz3185;
                      CREATE(zoptionzIbzK)(&zgaz3185);
                      {
                        __label__ case_297, case_296, finish_match_295;

                        {
                          __label__ case_300, case_299, finish_match_298;

                          bool zuz392;
                          zuz392 = zhead_expz3.ztup0.zR;
                          bool zuz393;
                          zuz393 = zhead_expz3.ztup0.zW;
                          bool zuz394;
                          zuz394 = zhead_expz3.ztup0.zC;
                          bool zuz395;
                          zuz395 = zhead_expz3.ztup0.zLM;
                          bool zuz396;
                          zuz396 = zhead_expz3.ztup0.zEL;
                          bool zuz397;
                          zuz397 = zhead_expz3.ztup0.zSL;
                          bool zuz398;
                          zuz398 = zhead_expz3.ztup0.zX;
                          bool zuz399;
                          zuz399 = zhead_expz3.ztup0.zASR;
                          enum zExecutionMode zmapping10z3;
                          zmapping10z3 = zhead_expz3.ztup1;
                          bool zgaz3184;
                          {
                            bool zgaz3183;
                            {
                              bool zgaz3182;
                              {
                                bool zgaz3181;
                                {
                                  bool zgaz3180;
                                  {
                                    bool zgaz3179;
                                    {
                                      bool zgaz3178;
                                      {
                                        bool zgaz3177;
                                        zgaz3177 = sailgen_execution_mode_encdec_forwards_matches(zmapping10z3);
                                        bool zgsz3294;
                                        if (zgaz3177) {  zgsz3294 = eq_bool(zuz399, false);  } else {
                                          zgsz3294 = false;
                                        }
                                        zgaz3178 = zgsz3294;
                                      }
                                      bool zgsz3295;
                                      if (zgaz3178) {  zgsz3295 = eq_bool(zuz398, false);  } else {
                                        zgsz3295 = false;
                                      }
                                      zgaz3179 = zgsz3295;
                                    }
                                    bool zgsz3296;
                                    if (zgaz3179) {  zgsz3296 = eq_bool(zuz397, false);  } else {  zgsz3296 = false;  }
                                    zgaz3180 = zgsz3296;
                                  }
                                  bool zgsz3297;
                                  if (zgaz3180) {  zgsz3297 = eq_bool(zuz396, false);  } else {  zgsz3297 = false;  }
                                  zgaz3181 = zgsz3297;
                                }
                                bool zgsz3298;
                                if (zgaz3181) {  zgsz3298 = eq_bool(zuz395, true);  } else {  zgsz3298 = false;  }
                                zgaz3182 = zgsz3298;
                              }
                              bool zgsz3299;
                              if (zgaz3182) {  zgsz3299 = eq_bool(zuz394, true);  } else {  zgsz3299 = false;  }
                              zgaz3183 = zgsz3299;
                            }
                            bool zgsz3300;
                            if (zgaz3183) {  zgsz3300 = eq_bool(zuz393, true);  } else {  zgsz3300 = false;  }
                            zgaz3184 = zgsz3300;
                          }
                          bool zgsz3301;
                          if (zgaz3184) {  zgsz3301 = eq_bool(zuz392, true);  } else {  zgsz3301 = false;  }
                          bool zgsz3305;
                          zgsz3305 = zgsz3301;
                          if (!(zgsz3305)) {  goto case_297;  }
                          uint64_t zgaz3176;
                          zgaz3176 = sailgen_execution_mode_encdec_forwards_infallible(zmapping10z3);
                          struct zoptionzIbzK zgsz3302;
                          CREATE(zoptionzIbzK)(&zgsz3302);
                          {
                            uint64_t zb__3;
                            zb__3 = zgaz3176;
                            bool zgsz3304;
                            zgsz3304 = (zb__3 == UINT64_C(0b0));
                            if (!(zgsz3304)) {  goto case_300;  }
                            {
                              RECREATE(lbits)(&zghz3112);
                              CONVERT_OF(lbits, fbits)(&zghz3112, UINT64_C(0b10111), UINT64_C(5) , true);
                              sailgen_SomezIbzK(&zgsz3302, zghz3112);
                            }
                            goto finish_match_298;
                          }
                        case_300: ;
                          {
                            sailgen_NonezIbzK(&zgsz3302, UNIT);
                            goto finish_match_298;
                          }
                        case_299: ;
                        finish_match_298: ;
                          COPY(zoptionzIbzK)(&zgaz3185, zgsz3302);
                          KILL(zoptionzIbzK)(&zgsz3302);
                          goto finish_match_295;
                        }
                      case_297: ;
                        {
                          sailgen_NonezIbzK(&zgaz3185, UNIT);
                          goto finish_match_295;
                        }
                      case_296: ;
                      finish_match_295: ;
                      }
                      uint64_t zgsz3306;
                      {
                        if (zgaz3185.kind != Kind_zSomezIbzK) goto case_329;
                        zgsz3306 = CONVERT_OF(fbits, lbits)(zgaz3185.variants.zSomezIbzK, true);
                        goto finish_match_301;
                      }
                    case_329: ;
                      {
                        __label__ case_328, case_310, finish_match_309;

                        /* complete */
                        struct zoptionzIbzK zgaz3195;
                        CREATE(zoptionzIbzK)(&zgaz3195);
                        {
                          __label__ case_305, case_304, finish_match_303;

                          {
                            __label__ case_308, case_307, finish_match_306;

                            bool zuz3101;
                            zuz3101 = zhead_expz3.ztup0.zR;
                            bool zuz3102;
                            zuz3102 = zhead_expz3.ztup0.zW;
                            bool zuz3103;
                            zuz3103 = zhead_expz3.ztup0.zC;
                            bool zuz3104;
                            zuz3104 = zhead_expz3.ztup0.zLM;
                            bool zuz3105;
                            zuz3105 = zhead_expz3.ztup0.zEL;
                            bool zuz3106;
                            zuz3106 = zhead_expz3.ztup0.zSL;
                            bool zuz3107;
                            zuz3107 = zhead_expz3.ztup0.zX;
                            bool zuz3108;
                            zuz3108 = zhead_expz3.ztup0.zASR;
                            enum zExecutionMode zmapping11z3;
                            zmapping11z3 = zhead_expz3.ztup1;
                            bool zgaz3194;
                            {
                              bool zgaz3193;
                              {
                                bool zgaz3192;
                                {
                                  bool zgaz3191;
                                  {
                                    bool zgaz3190;
                                    {
                                      bool zgaz3189;
                                      {
                                        bool zgaz3188;
                                        {
                                          bool zgaz3187;
                                          zgaz3187 = sailgen_execution_mode_encdec_forwards_matches(zmapping11z3);
                                          bool zgsz3309;
                                          if (zgaz3187) {  zgsz3309 = eq_bool(zuz3108, false);  } else {
                                            zgsz3309 = false;
                                          }
                                          zgaz3188 = zgsz3309;
                                        }
                                        bool zgsz3310;
                                        if (zgaz3188) {  zgsz3310 = eq_bool(zuz3107, false);  } else {
                                          zgsz3310 = false;
                                        }
                                        zgaz3189 = zgsz3310;
                                      }
                                      bool zgsz3311;
                                      if (zgaz3189) {  zgsz3311 = eq_bool(zuz3106, false);  } else {
                                        zgsz3311 = false;
                                      }
                                      zgaz3190 = zgsz3311;
                                    }
                                    bool zgsz3312;
                                    if (zgaz3190) {  zgsz3312 = eq_bool(zuz3105, true);  } else {  zgsz3312 = false;  }
                                    zgaz3191 = zgsz3312;
                                  }
                                  bool zgsz3313;
                                  if (zgaz3191) {  zgsz3313 = eq_bool(zuz3104, true);  } else {  zgsz3313 = false;  }
                                  zgaz3192 = zgsz3313;
                                }
                                bool zgsz3314;
                                if (zgaz3192) {  zgsz3314 = eq_bool(zuz3103, true);  } else {  zgsz3314 = false;  }
                                zgaz3193 = zgsz3314;
                              }
                              bool zgsz3315;
                              if (zgaz3193) {  zgsz3315 = eq_bool(zuz3102, false);  } else {  zgsz3315 = false;  }
                              zgaz3194 = zgsz3315;
                            }
                            bool zgsz3316;
                            if (zgaz3194) {  zgsz3316 = eq_bool(zuz3101, true);  } else {  zgsz3316 = false;  }
                            bool zgsz3320;
                            zgsz3320 = zgsz3316;
                            if (!(zgsz3320)) {  goto case_305;  }
                            uint64_t zgaz3186;
                            zgaz3186 = sailgen_execution_mode_encdec_forwards_infallible(zmapping11z3);
                            struct zoptionzIbzK zgsz3317;
                            CREATE(zoptionzIbzK)(&zgsz3317);
                            {
                              uint64_t zb__2;
                              zb__2 = zgaz3186;
                              bool zgsz3319;
                              zgsz3319 = (zb__2 == UINT64_C(0b0));
                              if (!(zgsz3319)) {  goto case_308;  }
                              {
                                RECREATE(lbits)(&zghz3111);
                                CONVERT_OF(lbits, fbits)(&zghz3111, UINT64_C(0b11011), UINT64_C(5) , true);
                                sailgen_SomezIbzK(&zgsz3317, zghz3111);
                              }
                              goto finish_match_306;
                            }
                          case_308: ;
                            {
                              sailgen_NonezIbzK(&zgsz3317, UNIT);
                              goto finish_match_306;
                            }
                          case_307: ;
                          finish_match_306: ;
                            COPY(zoptionzIbzK)(&zgaz3195, zgsz3317);
                            KILL(zoptionzIbzK)(&zgsz3317);
                            goto finish_match_303;
                          }
                        case_305: ;
                          {
                            sailgen_NonezIbzK(&zgaz3195, UNIT);
                            goto finish_match_303;
                          }
                        case_304: ;
                        finish_match_303: ;
                        }
                        uint64_t zgsz3321;
                        {
                          if (zgaz3195.kind != Kind_zSomezIbzK) goto case_328;
                          zgsz3321 = CONVERT_OF(fbits, lbits)(zgaz3195.variants.zSomezIbzK, true);
                          goto finish_match_309;
                        }
                      case_328: ;
                        {
                          __label__ case_327, case_318, finish_match_317;

                          /* complete */
                          struct zoptionzIbzK zgaz3205;
                          CREATE(zoptionzIbzK)(&zgaz3205);
                          {
                            __label__ case_313, case_312, finish_match_311;

                            {
                              __label__ case_316, case_315, finish_match_314;

                              bool zuz3110;
                              zuz3110 = zhead_expz3.ztup0.zR;
                              bool zuz3111;
                              zuz3111 = zhead_expz3.ztup0.zW;
                              bool zuz3112;
                              zuz3112 = zhead_expz3.ztup0.zC;
                              bool zuz3113;
                              zuz3113 = zhead_expz3.ztup0.zLM;
                              bool zuz3114;
                              zuz3114 = zhead_expz3.ztup0.zEL;
                              bool zuz3115;
                              zuz3115 = zhead_expz3.ztup0.zSL;
                              bool zuz3116;
                              zuz3116 = zhead_expz3.ztup0.zX;
                              bool zuz3117;
                              zuz3117 = zhead_expz3.ztup0.zASR;
                              enum zExecutionMode zmapping12z3;
                              zmapping12z3 = zhead_expz3.ztup1;
                              bool zgaz3204;
                              {
                                bool zgaz3203;
                                {
                                  bool zgaz3202;
                                  {
                                    bool zgaz3201;
                                    {
                                      bool zgaz3200;
                                      {
                                        bool zgaz3199;
                                        {
                                          bool zgaz3198;
                                          {
                                            bool zgaz3197;
                                            zgaz3197 = sailgen_execution_mode_encdec_forwards_matches(zmapping12z3);
                                            bool zgsz3324;
                                            if (zgaz3197) {  zgsz3324 = eq_bool(zuz3117, false);  } else {
                                              zgsz3324 = false;
                                            }
                                            zgaz3198 = zgsz3324;
                                          }
                                          bool zgsz3325;
                                          if (zgaz3198) {  zgsz3325 = eq_bool(zuz3116, false);  } else {
                                            zgsz3325 = false;
                                          }
                                          zgaz3199 = zgsz3325;
                                        }
                                        bool zgsz3326;
                                        if (zgaz3199) {  zgsz3326 = eq_bool(zuz3115, true);  } else {
                                          zgsz3326 = false;
                                        }
                                        zgaz3200 = zgsz3326;
                                      }
                                      bool zgsz3327;
                                      if (zgaz3200) {  zgsz3327 = eq_bool(zuz3114, true);  } else {
                                        zgsz3327 = false;
                                      }
                                      zgaz3201 = zgsz3327;
                                    }
                                    bool zgsz3328;
                                    if (zgaz3201) {  zgsz3328 = eq_bool(zuz3113, true);  } else {  zgsz3328 = false;  }
                                    zgaz3202 = zgsz3328;
                                  }
                                  bool zgsz3329;
                                  if (zgaz3202) {  zgsz3329 = eq_bool(zuz3112, true);  } else {  zgsz3329 = false;  }
                                  zgaz3203 = zgsz3329;
                                }
                                bool zgsz3330;
                                if (zgaz3203) {  zgsz3330 = eq_bool(zuz3111, true);  } else {  zgsz3330 = false;  }
                                zgaz3204 = zgsz3330;
                              }
                              bool zgsz3331;
                              if (zgaz3204) {  zgsz3331 = eq_bool(zuz3110, true);  } else {  zgsz3331 = false;  }
                              bool zgsz3335;
                              zgsz3335 = zgsz3331;
                              if (!(zgsz3335)) {  goto case_313;  }
                              uint64_t zgaz3196;
                              zgaz3196 = sailgen_execution_mode_encdec_forwards_infallible(zmapping12z3);
                              struct zoptionzIbzK zgsz3332;
                              CREATE(zoptionzIbzK)(&zgsz3332);
                              {
                                uint64_t zb__1;
                                zb__1 = zgaz3196;
                                bool zgsz3334;
                                zgsz3334 = (zb__1 == UINT64_C(0b0));
                                if (!(zgsz3334)) {  goto case_316;  }
                                {
                                  RECREATE(lbits)(&zghz3110);
                                  CONVERT_OF(lbits, fbits)(&zghz3110, UINT64_C(0b11110), UINT64_C(5) , true);
                                  sailgen_SomezIbzK(&zgsz3332, zghz3110);
                                }
                                goto finish_match_314;
                              }
                            case_316: ;
                              {
                                sailgen_NonezIbzK(&zgsz3332, UNIT);
                                goto finish_match_314;
                              }
                            case_315: ;
                            finish_match_314: ;
                              COPY(zoptionzIbzK)(&zgaz3205, zgsz3332);
                              KILL(zoptionzIbzK)(&zgsz3332);
                              goto finish_match_311;
                            }
                          case_313: ;
                            {
                              sailgen_NonezIbzK(&zgaz3205, UNIT);
                              goto finish_match_311;
                            }
                          case_312: ;
                          finish_match_311: ;
                          }
                          uint64_t zgsz3336;
                          {
                            if (zgaz3205.kind != Kind_zSomezIbzK) goto case_327;
                            zgsz3336 = CONVERT_OF(fbits, lbits)(zgaz3205.variants.zSomezIbzK, true);
                            goto finish_match_317;
                          }
                        case_327: ;
                          {
                            __label__ case_326, finish_match_325;

                            /* complete */
                            struct zoptionzIbzK zgaz3215;
                            CREATE(zoptionzIbzK)(&zgaz3215);
                            {
                              __label__ case_321, case_320, finish_match_319;

                              {
                                __label__ case_324, case_323, finish_match_322;

                                bool zuz3119;
                                zuz3119 = zhead_expz3.ztup0.zR;
                                bool zuz3120;
                                zuz3120 = zhead_expz3.ztup0.zW;
                                bool zuz3121;
                                zuz3121 = zhead_expz3.ztup0.zC;
                                bool zuz3122;
                                zuz3122 = zhead_expz3.ztup0.zLM;
                                bool zuz3123;
                                zuz3123 = zhead_expz3.ztup0.zEL;
                                bool zuz3124;
                                zuz3124 = zhead_expz3.ztup0.zSL;
                                bool zuz3125;
                                zuz3125 = zhead_expz3.ztup0.zX;
                                bool zuz3126;
                                zuz3126 = zhead_expz3.ztup0.zASR;
                                enum zExecutionMode zmapping13z3;
                                zmapping13z3 = zhead_expz3.ztup1;
                                bool zgaz3214;
                                {
                                  bool zgaz3213;
                                  {
                                    bool zgaz3212;
                                    {
                                      bool zgaz3211;
                                      {
                                        bool zgaz3210;
                                        {
                                          bool zgaz3209;
                                          {
                                            bool zgaz3208;
                                            {
                                              bool zgaz3207;
                                              zgaz3207 = sailgen_execution_mode_encdec_forwards_matches(zmapping13z3);
                                              bool zgsz3339;
                                              if (zgaz3207) {  zgsz3339 = eq_bool(zuz3126, false);  } else {
                                                zgsz3339 = false;
                                              }
                                              zgaz3208 = zgsz3339;
                                            }
                                            bool zgsz3340;
                                            if (zgaz3208) {  zgsz3340 = eq_bool(zuz3125, false);  } else {
                                              zgsz3340 = false;
                                            }
                                            zgaz3209 = zgsz3340;
                                          }
                                          bool zgsz3341;
                                          if (zgaz3209) {  zgsz3341 = eq_bool(zuz3124, false);  } else {
                                            zgsz3341 = false;
                                          }
                                          zgaz3210 = zgsz3341;
                                        }
                                        bool zgsz3342;
                                        if (zgaz3210) {  zgsz3342 = eq_bool(zuz3123, true);  } else {
                                          zgsz3342 = false;
                                        }
                                        zgaz3211 = zgsz3342;
                                      }
                                      bool zgsz3343;
                                      if (zgaz3211) {  zgsz3343 = eq_bool(zuz3122, true);  } else {
                                        zgsz3343 = false;
                                      }
                                      zgaz3212 = zgsz3343;
                                    }
                                    bool zgsz3344;
                                    if (zgaz3212) {  zgsz3344 = eq_bool(zuz3121, true);  } else {  zgsz3344 = false;  }
                                    zgaz3213 = zgsz3344;
                                  }
                                  bool zgsz3345;
                                  if (zgaz3213) {  zgsz3345 = eq_bool(zuz3120, true);  } else {  zgsz3345 = false;  }
                                  zgaz3214 = zgsz3345;
                                }
                                bool zgsz3346;
                                if (zgaz3214) {  zgsz3346 = eq_bool(zuz3119, true);  } else {  zgsz3346 = false;  }
                                bool zgsz3350;
                                zgsz3350 = zgsz3346;
                                if (!(zgsz3350)) {  goto case_321;  }
                                uint64_t zgaz3206;
                                zgaz3206 = sailgen_execution_mode_encdec_forwards_infallible(zmapping13z3);
                                struct zoptionzIbzK zgsz3347;
                                CREATE(zoptionzIbzK)(&zgsz3347);
                                {
                                  uint64_t zb__0;
                                  zb__0 = zgaz3206;
                                  bool zgsz3349;
                                  zgsz3349 = (zb__0 == UINT64_C(0b0));
                                  if (!(zgsz3349)) {  goto case_324;  }
                                  {
                                    RECREATE(lbits)(&zghz3109);
                                    CONVERT_OF(lbits, fbits)(&zghz3109, UINT64_C(0b11111), UINT64_C(5) , true);
                                    sailgen_SomezIbzK(&zgsz3347, zghz3109);
                                  }
                                  goto finish_match_322;
                                }
                              case_324: ;
                                {
                                  sailgen_NonezIbzK(&zgsz3347, UNIT);
                                  goto finish_match_322;
                                }
                              case_323: ;
                              finish_match_322: ;
                                COPY(zoptionzIbzK)(&zgaz3215, zgsz3347);
                                KILL(zoptionzIbzK)(&zgsz3347);
                                goto finish_match_319;
                              }
                            case_321: ;
                              {
                                sailgen_NonezIbzK(&zgaz3215, UNIT);
                                goto finish_match_319;
                              }
                            case_320: ;
                            finish_match_319: ;
                            }
                            uint64_t zgsz3351;
                            {
                              if (zgaz3215.kind != Kind_zSomezIbzK) goto case_326;
                              zgsz3351 = CONVERT_OF(fbits, lbits)(zgaz3215.variants.zSomezIbzK, true);
                              goto finish_match_325;
                            }
                          case_326: ;
                            sail_match_failure("ap_m_encdec_backwards");
                          finish_match_325: ;
                            zgsz3336 = zgsz3351;
                            KILL(zoptionzIbzK)(&zgaz3215);
                            goto finish_match_317;
                          }
                        case_318: ;
                        finish_match_317: ;
                          zgsz3321 = zgsz3336;
                          KILL(zoptionzIbzK)(&zgaz3205);
                          goto finish_match_309;
                        }
                      case_310: ;
                      finish_match_309: ;
                        zgsz3306 = zgsz3321;
                        KILL(zoptionzIbzK)(&zgaz3195);
                        goto finish_match_301;
                      }
                    case_302: ;
                    finish_match_301: ;
                      zgsz3291 = zgsz3306;
                      KILL(zoptionzIbzK)(&zgaz3185);
                      goto finish_match_293;
                    }
                  case_294: ;
                  finish_match_293: ;
                    zgsz3276 = zgsz3291;
                    KILL(zoptionzIbzK)(&zgaz3175);
                    goto finish_match_285;
                  }
                case_286: ;
                finish_match_285: ;
                  zgsz3261 = zgsz3276;
                  KILL(zoptionzIbzK)(&zgaz3165);
                  goto finish_match_277;
                }
              case_278: ;
              finish_match_277: ;
                zgsz3246 = zgsz3261;
                KILL(zoptionzIbzK)(&zgaz3155);
                goto finish_match_269;
              }
            case_270: ;
            finish_match_269: ;
              zgsz3231 = zgsz3246;
              KILL(zoptionzIbzK)(&zgaz3142);
              goto finish_match_261;
            }
          case_262: ;
          finish_match_261: ;
            zgsz3216 = zgsz3231;
            KILL(zoptionzIbzK)(&zgaz3129);
            goto finish_match_253;
          }
        case_254: ;
        finish_match_253: ;
          zgsz3201 = zgsz3216;
          KILL(zoptionzIbzK)(&zgaz3116);
          goto finish_match_245;
        }
      case_246: ;
      finish_match_245: ;
        zgsz3186 = zgsz3201;
        KILL(zoptionzIbzK)(&zgaz3103);
        goto finish_match_237;
      }
    case_238: ;
    finish_match_237: ;
      zgsz3171 = zgsz3186;
      KILL(zoptionzIbzK)(&zgaz393);
      goto finish_match_229;
    }
  case_230: ;
  finish_match_229: ;
    zcbz349 = zgsz3171;
    KILL(zoptionzIbzK)(&zgaz383);
    goto finish_match_221;
  }
case_222: ;
finish_match_221: ;
  KILL(zoptionzIbzK)(&zgaz373);
end_function_340: ;
  return zcbz349;
end_block_exception_341: ;

  return UINT64_C(0xdeadc0de);
}



static void finish_sailgen_ap_m_encdec_backwards(void)
{
  KILL(lbits)(&zghz3122);
  KILL(lbits)(&zghz3121);
  KILL(lbits)(&zghz3120);
  KILL(lbits)(&zghz3119);
  KILL(lbits)(&zghz3118);
  KILL(lbits)(&zghz3117);
  KILL(lbits)(&zghz3116);
  KILL(lbits)(&zghz3115);
  KILL(lbits)(&zghz3114);
  KILL(lbits)(&zghz3113);
  KILL(lbits)(&zghz3112);
  KILL(lbits)(&zghz3111);
  KILL(lbits)(&zghz3110);
  KILL(lbits)(&zghz3109);
}

static uint64_t sailgen_ap_m_encdec_backwards_infallible(struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9);

lbits zghz3123;
lbits zghz3124;
lbits zghz3125;
lbits zghz3126;
lbits zghz3127;
lbits zghz3128;
lbits zghz3129;
lbits zghz3130;
lbits zghz3131;
lbits zghz3132;
lbits zghz3133;
lbits zghz3134;
lbits zghz3135;
lbits zghz3136;

static void startup_sailgen_ap_m_encdec_backwards_infallible(void)
{
  CREATE(lbits)(&zghz3123);
  CREATE(lbits)(&zghz3124);
  CREATE(lbits)(&zghz3125);
  CREATE(lbits)(&zghz3126);
  CREATE(lbits)(&zghz3127);
  CREATE(lbits)(&zghz3128);
  CREATE(lbits)(&zghz3129);
  CREATE(lbits)(&zghz3130);
  CREATE(lbits)(&zghz3131);
  CREATE(lbits)(&zghz3132);
  CREATE(lbits)(&zghz3133);
  CREATE(lbits)(&zghz3134);
  CREATE(lbits)(&zghz3135);
  CREATE(lbits)(&zghz3136);
}

static uint64_t sailgen_ap_m_encdec_backwards_infallible(struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zargz3)
{
  __label__ case_466, case_349, finish_match_348, end_function_467, end_block_exception_468;

  uint64_t zcbz350;
  struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zhead_expz3;
  zhead_expz3 = zargz3;
  struct zoptionzIbzK zgaz373;
  CREATE(zoptionzIbzK)(&zgaz373);
  {
    __label__ case_344, case_343, finish_match_342;

    {
      __label__ case_347, case_346, finish_match_345;

      bool zp0z3;
      zp0z3 = zhead_expz3.ztup0.zR;
      bool zp1z3;
      zp1z3 = zhead_expz3.ztup0.zW;
      bool zp2z3;
      zp2z3 = zhead_expz3.ztup0.zC;
      bool zp3z3;
      zp3z3 = zhead_expz3.ztup0.zLM;
      bool zp4z3;
      zp4z3 = zhead_expz3.ztup0.zEL;
      bool zp5z3;
      zp5z3 = zhead_expz3.ztup0.zSL;
      bool zp6z3;
      zp6z3 = zhead_expz3.ztup0.zX;
      bool zp7z3;
      zp7z3 = zhead_expz3.ztup0.zASR;
      enum zExecutionMode zmapping0z3;
      zmapping0z3 = zhead_expz3.ztup1;
      bool zgaz372;
      {
        bool zgaz371;
        {
          bool zgaz370;
          {
            bool zgaz369;
            {
              bool zgaz368;
              {
                bool zgaz367;
                {
                  bool zgaz366;
                  {
                    bool zgaz365;
                    zgaz365 = sailgen_execution_mode_encdec_forwards_matches(zmapping0z3);
                    bool zgsz3381;
                    if (zgaz365) {  zgsz3381 = eq_bool(zp7z3, false);  } else {  zgsz3381 = false;  }
                    zgaz366 = zgsz3381;
                  }
                  bool zgsz3382;
                  if (zgaz366) {  zgsz3382 = eq_bool(zp6z3, false);  } else {  zgsz3382 = false;  }
                  zgaz367 = zgsz3382;
                }
                bool zgsz3383;
                if (zgaz367) {  zgsz3383 = eq_bool(zp5z3, false);  } else {  zgsz3383 = false;  }
                zgaz368 = zgsz3383;
              }
              bool zgsz3384;
              if (zgaz368) {  zgsz3384 = eq_bool(zp4z3, false);  } else {  zgsz3384 = false;  }
              zgaz369 = zgsz3384;
            }
            bool zgsz3385;
            if (zgaz369) {  zgsz3385 = eq_bool(zp3z3, false);  } else {  zgsz3385 = false;  }
            zgaz370 = zgsz3385;
          }
          bool zgsz3386;
          if (zgaz370) {  zgsz3386 = eq_bool(zp2z3, false);  } else {  zgsz3386 = false;  }
          zgaz371 = zgsz3386;
        }
        bool zgsz3387;
        if (zgaz371) {  zgsz3387 = eq_bool(zp1z3, false);  } else {  zgsz3387 = false;  }
        zgaz372 = zgsz3387;
      }
      bool zgsz3388;
      if (zgaz372) {  zgsz3388 = eq_bool(zp0z3, false);  } else {  zgsz3388 = false;  }
      bool zgsz3392;
      zgsz3392 = zgsz3388;
      if (!(zgsz3392)) {  goto case_344;  }
      uint64_t zgaz364;
      zgaz364 = sailgen_execution_mode_encdec_forwards_infallible(zmapping0z3);
      struct zoptionzIbzK zgsz3389;
      CREATE(zoptionzIbzK)(&zgsz3389);
      {
        uint64_t zb__9;
        zb__9 = zgaz364;
        bool zgsz3391;
        zgsz3391 = (zb__9 == UINT64_C(0b0));
        if (!(zgsz3391)) {  goto case_347;  }
        {
          RECREATE(lbits)(&zghz3136);
          CONVERT_OF(lbits, fbits)(&zghz3136, UINT64_C(0b00000), UINT64_C(5) , true);
          sailgen_SomezIbzK(&zgsz3389, zghz3136);
        }
        goto finish_match_345;
      }
    case_347: ;
      {
        sailgen_NonezIbzK(&zgsz3389, UNIT);
        goto finish_match_345;
      }
    case_346: ;
    finish_match_345: ;
      COPY(zoptionzIbzK)(&zgaz373, zgsz3389);
      KILL(zoptionzIbzK)(&zgsz3389);
      goto finish_match_342;
    }
  case_344: ;
    {
      sailgen_NonezIbzK(&zgaz373, UNIT);
      goto finish_match_342;
    }
  case_343: ;
  finish_match_342: ;
  }
  {
    if (zgaz373.kind != Kind_zSomezIbzK) goto case_466;
    uint64_t zresult;
    zresult = CONVERT_OF(fbits, lbits)(zgaz373.variants.zSomezIbzK, true);
    zcbz350 = zresult;
    goto finish_match_348;
  }
case_466: ;
  {
    __label__ case_465, case_357, finish_match_356;

    /* complete */
    struct zoptionzIbzK zgaz383;
    CREATE(zoptionzIbzK)(&zgaz383);
    {
      __label__ case_352, case_351, finish_match_350;

      {
        __label__ case_355, case_354, finish_match_353;

        bool zuz3128;
        zuz3128 = zhead_expz3.ztup0.zR;
        bool zuz3129;
        zuz3129 = zhead_expz3.ztup0.zW;
        bool zuz3130;
        zuz3130 = zhead_expz3.ztup0.zC;
        bool zuz3131;
        zuz3131 = zhead_expz3.ztup0.zLM;
        bool zuz3132;
        zuz3132 = zhead_expz3.ztup0.zEL;
        bool zuz3133;
        zuz3133 = zhead_expz3.ztup0.zSL;
        bool zuz3134;
        zuz3134 = zhead_expz3.ztup0.zX;
        bool zuz3135;
        zuz3135 = zhead_expz3.ztup0.zASR;
        enum zExecutionMode zmapping1z3;
        zmapping1z3 = zhead_expz3.ztup1;
        bool zgaz382;
        {
          bool zgaz381;
          {
            bool zgaz380;
            {
              bool zgaz379;
              {
                bool zgaz378;
                {
                  bool zgaz377;
                  {
                    bool zgaz376;
                    {
                      bool zgaz375;
                      zgaz375 = sailgen_execution_mode_encdec_forwards_matches(zmapping1z3);
                      bool zgsz3396;
                      if (zgaz375) {  zgsz3396 = eq_bool(zuz3135, false);  } else {  zgsz3396 = false;  }
                      zgaz376 = zgsz3396;
                    }
                    bool zgsz3397;
                    if (zgaz376) {  zgsz3397 = eq_bool(zuz3134, false);  } else {  zgsz3397 = false;  }
                    zgaz377 = zgsz3397;
                  }
                  bool zgsz3398;
                  if (zgaz377) {  zgsz3398 = eq_bool(zuz3133, false);  } else {  zgsz3398 = false;  }
                  zgaz378 = zgsz3398;
                }
                bool zgsz3399;
                if (zgaz378) {  zgsz3399 = eq_bool(zuz3132, false);  } else {  zgsz3399 = false;  }
                zgaz379 = zgsz3399;
              }
              bool zgsz3400;
              if (zgaz379) {  zgsz3400 = eq_bool(zuz3131, false);  } else {  zgsz3400 = false;  }
              zgaz380 = zgsz3400;
            }
            bool zgsz3401;
            if (zgaz380) {  zgsz3401 = eq_bool(zuz3130, false);  } else {  zgsz3401 = false;  }
            zgaz381 = zgsz3401;
          }
          bool zgsz3402;
          if (zgaz381) {  zgsz3402 = eq_bool(zuz3129, false);  } else {  zgsz3402 = false;  }
          zgaz382 = zgsz3402;
        }
        bool zgsz3403;
        if (zgaz382) {  zgsz3403 = eq_bool(zuz3128, true);  } else {  zgsz3403 = false;  }
        bool zgsz3407;
        zgsz3407 = zgsz3403;
        if (!(zgsz3407)) {  goto case_352;  }
        uint64_t zgaz374;
        zgaz374 = sailgen_execution_mode_encdec_forwards_infallible(zmapping1z3);
        struct zoptionzIbzK zgsz3404;
        CREATE(zoptionzIbzK)(&zgsz3404);
        {
          uint64_t zb__8;
          zb__8 = zgaz374;
          bool zgsz3406;
          zgsz3406 = (zb__8 == UINT64_C(0b0));
          if (!(zgsz3406)) {  goto case_355;  }
          {
            RECREATE(lbits)(&zghz3135);
            CONVERT_OF(lbits, fbits)(&zghz3135, UINT64_C(0b00001), UINT64_C(5) , true);
            sailgen_SomezIbzK(&zgsz3404, zghz3135);
          }
          goto finish_match_353;
        }
      case_355: ;
        {
          sailgen_NonezIbzK(&zgsz3404, UNIT);
          goto finish_match_353;
        }
      case_354: ;
      finish_match_353: ;
        COPY(zoptionzIbzK)(&zgaz383, zgsz3404);
        KILL(zoptionzIbzK)(&zgsz3404);
        goto finish_match_350;
      }
    case_352: ;
      {
        sailgen_NonezIbzK(&zgaz383, UNIT);
        goto finish_match_350;
      }
    case_351: ;
    finish_match_350: ;
    }
    uint64_t zgsz3408;
    {
      if (zgaz383.kind != Kind_zSomezIbzK) goto case_465;
      zgsz3408 = CONVERT_OF(fbits, lbits)(zgaz383.variants.zSomezIbzK, true);
      goto finish_match_356;
    }
  case_465: ;
    {
      __label__ case_464, case_365, finish_match_364;

      /* complete */
      struct zoptionzIbzK zgaz393;
      CREATE(zoptionzIbzK)(&zgaz393);
      {
        __label__ case_360, case_359, finish_match_358;

        {
          __label__ case_363, case_362, finish_match_361;

          bool zuz3137;
          zuz3137 = zhead_expz3.ztup0.zR;
          bool zuz3138;
          zuz3138 = zhead_expz3.ztup0.zW;
          bool zuz3139;
          zuz3139 = zhead_expz3.ztup0.zC;
          bool zuz3140;
          zuz3140 = zhead_expz3.ztup0.zLM;
          bool zuz3141;
          zuz3141 = zhead_expz3.ztup0.zEL;
          bool zuz3142;
          zuz3142 = zhead_expz3.ztup0.zSL;
          bool zuz3143;
          zuz3143 = zhead_expz3.ztup0.zX;
          bool zuz3144;
          zuz3144 = zhead_expz3.ztup0.zASR;
          enum zExecutionMode zmapping2z3;
          zmapping2z3 = zhead_expz3.ztup1;
          bool zgaz392;
          {
            bool zgaz391;
            {
              bool zgaz390;
              {
                bool zgaz389;
                {
                  bool zgaz388;
                  {
                    bool zgaz387;
                    {
                      bool zgaz386;
                      {
                        bool zgaz385;
                        zgaz385 = sailgen_execution_mode_encdec_forwards_matches(zmapping2z3);
                        bool zgsz3411;
                        if (zgaz385) {  zgsz3411 = eq_bool(zuz3144, false);  } else {  zgsz3411 = false;  }
                        zgaz386 = zgsz3411;
                      }
                      bool zgsz3412;
                      if (zgaz386) {  zgsz3412 = eq_bool(zuz3143, false);  } else {  zgsz3412 = false;  }
                      zgaz387 = zgsz3412;
                    }
                    bool zgsz3413;
                    if (zgaz387) {  zgsz3413 = eq_bool(zuz3142, false);  } else {  zgsz3413 = false;  }
                    zgaz388 = zgsz3413;
                  }
                  bool zgsz3414;
                  if (zgaz388) {  zgsz3414 = eq_bool(zuz3141, false);  } else {  zgsz3414 = false;  }
                  zgaz389 = zgsz3414;
                }
                bool zgsz3415;
                if (zgaz389) {  zgsz3415 = eq_bool(zuz3140, false);  } else {  zgsz3415 = false;  }
                zgaz390 = zgsz3415;
              }
              bool zgsz3416;
              if (zgaz390) {  zgsz3416 = eq_bool(zuz3139, false);  } else {  zgsz3416 = false;  }
              zgaz391 = zgsz3416;
            }
            bool zgsz3417;
            if (zgaz391) {  zgsz3417 = eq_bool(zuz3138, true);  } else {  zgsz3417 = false;  }
            zgaz392 = zgsz3417;
          }
          bool zgsz3418;
          if (zgaz392) {  zgsz3418 = eq_bool(zuz3137, false);  } else {  zgsz3418 = false;  }
          bool zgsz3422;
          zgsz3422 = zgsz3418;
          if (!(zgsz3422)) {  goto case_360;  }
          uint64_t zgaz384;
          zgaz384 = sailgen_execution_mode_encdec_forwards_infallible(zmapping2z3);
          struct zoptionzIbzK zgsz3419;
          CREATE(zoptionzIbzK)(&zgsz3419);
          {
            uint64_t zb__7;
            zb__7 = zgaz384;
            bool zgsz3421;
            zgsz3421 = (zb__7 == UINT64_C(0b0));
            if (!(zgsz3421)) {  goto case_363;  }
            {
              RECREATE(lbits)(&zghz3134);
              CONVERT_OF(lbits, fbits)(&zghz3134, UINT64_C(0b00100), UINT64_C(5) , true);
              sailgen_SomezIbzK(&zgsz3419, zghz3134);
            }
            goto finish_match_361;
          }
        case_363: ;
          {
            sailgen_NonezIbzK(&zgsz3419, UNIT);
            goto finish_match_361;
          }
        case_362: ;
        finish_match_361: ;
          COPY(zoptionzIbzK)(&zgaz393, zgsz3419);
          KILL(zoptionzIbzK)(&zgsz3419);
          goto finish_match_358;
        }
      case_360: ;
        {
          sailgen_NonezIbzK(&zgaz393, UNIT);
          goto finish_match_358;
        }
      case_359: ;
      finish_match_358: ;
      }
      uint64_t zgsz3423;
      {
        if (zgaz393.kind != Kind_zSomezIbzK) goto case_464;
        zgsz3423 = CONVERT_OF(fbits, lbits)(zgaz393.variants.zSomezIbzK, true);
        goto finish_match_364;
      }
    case_464: ;
      {
        __label__ case_463, case_373, finish_match_372;

        /* complete */
        struct zoptionzIbzK zgaz3103;
        CREATE(zoptionzIbzK)(&zgaz3103);
        {
          __label__ case_368, case_367, finish_match_366;

          {
            __label__ case_371, case_370, finish_match_369;

            bool zuz3146;
            zuz3146 = zhead_expz3.ztup0.zR;
            bool zuz3147;
            zuz3147 = zhead_expz3.ztup0.zW;
            bool zuz3148;
            zuz3148 = zhead_expz3.ztup0.zC;
            bool zuz3149;
            zuz3149 = zhead_expz3.ztup0.zLM;
            bool zuz3150;
            zuz3150 = zhead_expz3.ztup0.zEL;
            bool zuz3151;
            zuz3151 = zhead_expz3.ztup0.zSL;
            bool zuz3152;
            zuz3152 = zhead_expz3.ztup0.zX;
            bool zuz3153;
            zuz3153 = zhead_expz3.ztup0.zASR;
            enum zExecutionMode zmapping3z3;
            zmapping3z3 = zhead_expz3.ztup1;
            bool zgaz3102;
            {
              bool zgaz3101;
              {
                bool zgaz3100;
                {
                  bool zgaz399;
                  {
                    bool zgaz398;
                    {
                      bool zgaz397;
                      {
                        bool zgaz396;
                        {
                          bool zgaz395;
                          zgaz395 = sailgen_execution_mode_encdec_forwards_matches(zmapping3z3);
                          bool zgsz3426;
                          if (zgaz395) {  zgsz3426 = eq_bool(zuz3153, false);  } else {  zgsz3426 = false;  }
                          zgaz396 = zgsz3426;
                        }
                        bool zgsz3427;
                        if (zgaz396) {  zgsz3427 = eq_bool(zuz3152, false);  } else {  zgsz3427 = false;  }
                        zgaz397 = zgsz3427;
                      }
                      bool zgsz3428;
                      if (zgaz397) {  zgsz3428 = eq_bool(zuz3151, false);  } else {  zgsz3428 = false;  }
                      zgaz398 = zgsz3428;
                    }
                    bool zgsz3429;
                    if (zgaz398) {  zgsz3429 = eq_bool(zuz3150, false);  } else {  zgsz3429 = false;  }
                    zgaz399 = zgsz3429;
                  }
                  bool zgsz3430;
                  if (zgaz399) {  zgsz3430 = eq_bool(zuz3149, false);  } else {  zgsz3430 = false;  }
                  zgaz3100 = zgsz3430;
                }
                bool zgsz3431;
                if (zgaz3100) {  zgsz3431 = eq_bool(zuz3148, false);  } else {  zgsz3431 = false;  }
                zgaz3101 = zgsz3431;
              }
              bool zgsz3432;
              if (zgaz3101) {  zgsz3432 = eq_bool(zuz3147, true);  } else {  zgsz3432 = false;  }
              zgaz3102 = zgsz3432;
            }
            bool zgsz3433;
            if (zgaz3102) {  zgsz3433 = eq_bool(zuz3146, true);  } else {  zgsz3433 = false;  }
            bool zgsz3437;
            zgsz3437 = zgsz3433;
            if (!(zgsz3437)) {  goto case_368;  }
            uint64_t zgaz394;
            zgaz394 = sailgen_execution_mode_encdec_forwards_infallible(zmapping3z3);
            struct zoptionzIbzK zgsz3434;
            CREATE(zoptionzIbzK)(&zgsz3434);
            {
              uint64_t zb__6;
              zb__6 = zgaz394;
              bool zgsz3436;
              zgsz3436 = (zb__6 == UINT64_C(0b0));
              if (!(zgsz3436)) {  goto case_371;  }
              {
                RECREATE(lbits)(&zghz3133);
                CONVERT_OF(lbits, fbits)(&zghz3133, UINT64_C(0b00101), UINT64_C(5) , true);
                sailgen_SomezIbzK(&zgsz3434, zghz3133);
              }
              goto finish_match_369;
            }
          case_371: ;
            {
              sailgen_NonezIbzK(&zgsz3434, UNIT);
              goto finish_match_369;
            }
          case_370: ;
          finish_match_369: ;
            COPY(zoptionzIbzK)(&zgaz3103, zgsz3434);
            KILL(zoptionzIbzK)(&zgsz3434);
            goto finish_match_366;
          }
        case_368: ;
          {
            sailgen_NonezIbzK(&zgaz3103, UNIT);
            goto finish_match_366;
          }
        case_367: ;
        finish_match_366: ;
        }
        uint64_t zgsz3438;
        {
          if (zgaz3103.kind != Kind_zSomezIbzK) goto case_463;
          zgsz3438 = CONVERT_OF(fbits, lbits)(zgaz3103.variants.zSomezIbzK, true);
          goto finish_match_372;
        }
      case_463: ;
        {
          __label__ case_462, case_381, finish_match_380;

          /* complete */
          struct zoptionzIbzK zgaz3116;
          CREATE(zoptionzIbzK)(&zgaz3116);
          {
            __label__ case_376, case_375, finish_match_374;

            {
              __label__ case_379, case_378, finish_match_377;

              bool zuz3155;
              zuz3155 = zhead_expz3.ztup0.zR;
              bool zuz3156;
              zuz3156 = zhead_expz3.ztup0.zW;
              bool zuz3157;
              zuz3157 = zhead_expz3.ztup0.zC;
              bool zuz3158;
              zuz3158 = zhead_expz3.ztup0.zLM;
              bool zuz3159;
              zuz3159 = zhead_expz3.ztup0.zEL;
              bool zuz3160;
              zuz3160 = zhead_expz3.ztup0.zSL;
              bool zuz3161;
              zuz3161 = zhead_expz3.ztup0.zX;
              bool zuz3162;
              zuz3162 = zhead_expz3.ztup0.zASR;
              enum zExecutionMode zmapping4z3;
              zmapping4z3 = zhead_expz3.ztup1;
              bool zgaz3115;
              {
                bool zgaz3114;
                {
                  bool zgaz3113;
                  {
                    bool zgaz3112;
                    {
                      bool zgaz3111;
                      {
                        bool zgaz3110;
                        {
                          bool zgaz3109;
                          {
                            bool zgaz3108;
                            zgaz3108 = sailgen_execution_mode_encdec_forwards_matches(zmapping4z3);
                            bool zgsz3441;
                            if (zgaz3108) {  zgsz3441 = eq_bool(zuz3162, true);  } else {  zgsz3441 = false;  }
                            zgaz3109 = zgsz3441;
                          }
                          bool zgsz3442;
                          if (zgaz3109) {  zgsz3442 = eq_bool(zuz3161, true);  } else {  zgsz3442 = false;  }
                          zgaz3110 = zgsz3442;
                        }
                        bool zgsz3443;
                        if (zgaz3110) {  zgsz3443 = eq_bool(zuz3160, true);  } else {  zgsz3443 = false;  }
                        zgaz3111 = zgsz3443;
                      }
                      bool zgsz3444;
                      if (zgaz3111) {  zgsz3444 = eq_bool(zuz3159, true);  } else {  zgsz3444 = false;  }
                      zgaz3112 = zgsz3444;
                    }
                    bool zgsz3445;
                    if (zgaz3112) {  zgsz3445 = eq_bool(zuz3158, true);  } else {  zgsz3445 = false;  }
                    zgaz3113 = zgsz3445;
                  }
                  bool zgsz3446;
                  if (zgaz3113) {  zgsz3446 = eq_bool(zuz3157, true);  } else {  zgsz3446 = false;  }
                  zgaz3114 = zgsz3446;
                }
                bool zgsz3447;
                if (zgaz3114) {  zgsz3447 = eq_bool(zuz3156, true);  } else {  zgsz3447 = false;  }
                zgaz3115 = zgsz3447;
              }
              bool zgsz3448;
              if (zgaz3115) {  zgsz3448 = eq_bool(zuz3155, true);  } else {  zgsz3448 = false;  }
              bool zgsz3452;
              zgsz3452 = zgsz3448;
              if (!(zgsz3452)) {  goto case_376;  }
              uint64_t zgaz3104;
              zgaz3104 = sailgen_execution_mode_encdec_forwards_infallible(zmapping4z3);
              struct zoptionzIbzK zgsz3449;
              CREATE(zoptionzIbzK)(&zgsz3449);
              {
                uint64_t zm;
                zm = zgaz3104;
                uint64_t zgaz3107;
                {
                  uint64_t zgaz3106;
                  {
                    uint64_t zgaz3105;
                    zgaz3105 = zm;
                    zgaz3106 = (UINT64_C(0b00) << 1) | zgaz3105;
                  }
                  zgaz3107 = (UINT64_C(0b01) << 3) | zgaz3106;
                }
                {
                  RECREATE(lbits)(&zghz3132);
                  CONVERT_OF(lbits, fbits)(&zghz3132, zgaz3107, UINT64_C(5) , true);
                  sailgen_SomezIbzK(&zgsz3449, zghz3132);
                }
                goto finish_match_377;
              }
            case_379: ;
              {
                sailgen_NonezIbzK(&zgsz3449, UNIT);
                goto finish_match_377;
              }
            case_378: ;
            finish_match_377: ;
              COPY(zoptionzIbzK)(&zgaz3116, zgsz3449);
              KILL(zoptionzIbzK)(&zgsz3449);
              goto finish_match_374;
            }
          case_376: ;
            {
              sailgen_NonezIbzK(&zgaz3116, UNIT);
              goto finish_match_374;
            }
          case_375: ;
          finish_match_374: ;
          }
          uint64_t zgsz3453;
          {
            if (zgaz3116.kind != Kind_zSomezIbzK) goto case_462;
            zgsz3453 = CONVERT_OF(fbits, lbits)(zgaz3116.variants.zSomezIbzK, true);
            goto finish_match_380;
          }
        case_462: ;
          {
            __label__ case_461, case_389, finish_match_388;

            /* complete */
            struct zoptionzIbzK zgaz3129;
            CREATE(zoptionzIbzK)(&zgaz3129);
            {
              __label__ case_384, case_383, finish_match_382;

              {
                __label__ case_387, case_386, finish_match_385;

                bool zuz3164;
                zuz3164 = zhead_expz3.ztup0.zR;
                bool zuz3165;
                zuz3165 = zhead_expz3.ztup0.zW;
                bool zuz3166;
                zuz3166 = zhead_expz3.ztup0.zC;
                bool zuz3167;
                zuz3167 = zhead_expz3.ztup0.zLM;
                bool zuz3168;
                zuz3168 = zhead_expz3.ztup0.zEL;
                bool zuz3169;
                zuz3169 = zhead_expz3.ztup0.zSL;
                bool zuz3170;
                zuz3170 = zhead_expz3.ztup0.zX;
                bool zuz3171;
                zuz3171 = zhead_expz3.ztup0.zASR;
                enum zExecutionMode zmapping5z3;
                zmapping5z3 = zhead_expz3.ztup1;
                bool zgaz3128;
                {
                  bool zgaz3127;
                  {
                    bool zgaz3126;
                    {
                      bool zgaz3125;
                      {
                        bool zgaz3124;
                        {
                          bool zgaz3123;
                          {
                            bool zgaz3122;
                            {
                              bool zgaz3121;
                              zgaz3121 = sailgen_execution_mode_encdec_forwards_matches(zmapping5z3);
                              bool zgsz3456;
                              if (zgaz3121) {  zgsz3456 = eq_bool(zuz3171, false);  } else {  zgsz3456 = false;  }
                              zgaz3122 = zgsz3456;
                            }
                            bool zgsz3457;
                            if (zgaz3122) {  zgsz3457 = eq_bool(zuz3170, true);  } else {  zgsz3457 = false;  }
                            zgaz3123 = zgsz3457;
                          }
                          bool zgsz3458;
                          if (zgaz3123) {  zgsz3458 = eq_bool(zuz3169, true);  } else {  zgsz3458 = false;  }
                          zgaz3124 = zgsz3458;
                        }
                        bool zgsz3459;
                        if (zgaz3124) {  zgsz3459 = eq_bool(zuz3168, true);  } else {  zgsz3459 = false;  }
                        zgaz3125 = zgsz3459;
                      }
                      bool zgsz3460;
                      if (zgaz3125) {  zgsz3460 = eq_bool(zuz3167, true);  } else {  zgsz3460 = false;  }
                      zgaz3126 = zgsz3460;
                    }
                    bool zgsz3461;
                    if (zgaz3126) {  zgsz3461 = eq_bool(zuz3166, true);  } else {  zgsz3461 = false;  }
                    zgaz3127 = zgsz3461;
                  }
                  bool zgsz3462;
                  if (zgaz3127) {  zgsz3462 = eq_bool(zuz3165, false);  } else {  zgsz3462 = false;  }
                  zgaz3128 = zgsz3462;
                }
                bool zgsz3463;
                if (zgaz3128) {  zgsz3463 = eq_bool(zuz3164, true);  } else {  zgsz3463 = false;  }
                bool zgsz3467;
                zgsz3467 = zgsz3463;
                if (!(zgsz3467)) {  goto case_384;  }
                uint64_t zgaz3117;
                zgaz3117 = sailgen_execution_mode_encdec_forwards_infallible(zmapping5z3);
                struct zoptionzIbzK zgsz3464;
                CREATE(zoptionzIbzK)(&zgsz3464);
                {
                  uint64_t zuz3172;
                  zuz3172 = zgaz3117;
                  uint64_t zgaz3120;
                  {
                    uint64_t zgaz3119;
                    {
                      uint64_t zgaz3118;
                      zgaz3118 = zuz3172;
                      zgaz3119 = (UINT64_C(0b01) << 1) | zgaz3118;
                    }
                    zgaz3120 = (UINT64_C(0b01) << 3) | zgaz3119;
                  }
                  {
                    RECREATE(lbits)(&zghz3131);
                    CONVERT_OF(lbits, fbits)(&zghz3131, zgaz3120, UINT64_C(5) , true);
                    sailgen_SomezIbzK(&zgsz3464, zghz3131);
                  }
                  goto finish_match_385;
                }
              case_387: ;
                {
                  sailgen_NonezIbzK(&zgsz3464, UNIT);
                  goto finish_match_385;
                }
              case_386: ;
              finish_match_385: ;
                COPY(zoptionzIbzK)(&zgaz3129, zgsz3464);
                KILL(zoptionzIbzK)(&zgsz3464);
                goto finish_match_382;
              }
            case_384: ;
              {
                sailgen_NonezIbzK(&zgaz3129, UNIT);
                goto finish_match_382;
              }
            case_383: ;
            finish_match_382: ;
            }
            uint64_t zgsz3468;
            {
              if (zgaz3129.kind != Kind_zSomezIbzK) goto case_461;
              zgsz3468 = CONVERT_OF(fbits, lbits)(zgaz3129.variants.zSomezIbzK, true);
              goto finish_match_388;
            }
          case_461: ;
            {
              __label__ case_460, case_397, finish_match_396;

              /* complete */
              struct zoptionzIbzK zgaz3142;
              CREATE(zoptionzIbzK)(&zgaz3142);
              {
                __label__ case_392, case_391, finish_match_390;

                {
                  __label__ case_395, case_394, finish_match_393;

                  bool zuz3174;
                  zuz3174 = zhead_expz3.ztup0.zR;
                  bool zuz3175;
                  zuz3175 = zhead_expz3.ztup0.zW;
                  bool zuz3176;
                  zuz3176 = zhead_expz3.ztup0.zC;
                  bool zuz3177;
                  zuz3177 = zhead_expz3.ztup0.zLM;
                  bool zuz3178;
                  zuz3178 = zhead_expz3.ztup0.zEL;
                  bool zuz3179;
                  zuz3179 = zhead_expz3.ztup0.zSL;
                  bool zuz3180;
                  zuz3180 = zhead_expz3.ztup0.zX;
                  bool zuz3181;
                  zuz3181 = zhead_expz3.ztup0.zASR;
                  enum zExecutionMode zmapping6z3;
                  zmapping6z3 = zhead_expz3.ztup1;
                  bool zgaz3141;
                  {
                    bool zgaz3140;
                    {
                      bool zgaz3139;
                      {
                        bool zgaz3138;
                        {
                          bool zgaz3137;
                          {
                            bool zgaz3136;
                            {
                              bool zgaz3135;
                              {
                                bool zgaz3134;
                                zgaz3134 = sailgen_execution_mode_encdec_forwards_matches(zmapping6z3);
                                bool zgsz3471;
                                if (zgaz3134) {  zgsz3471 = eq_bool(zuz3181, false);  } else {  zgsz3471 = false;  }
                                zgaz3135 = zgsz3471;
                              }
                              bool zgsz3472;
                              if (zgaz3135) {  zgsz3472 = eq_bool(zuz3180, true);  } else {  zgsz3472 = false;  }
                              zgaz3136 = zgsz3472;
                            }
                            bool zgsz3473;
                            if (zgaz3136) {  zgsz3473 = eq_bool(zuz3179, true);  } else {  zgsz3473 = false;  }
                            zgaz3137 = zgsz3473;
                          }
                          bool zgsz3474;
                          if (zgaz3137) {  zgsz3474 = eq_bool(zuz3178, true);  } else {  zgsz3474 = false;  }
                          zgaz3138 = zgsz3474;
                        }
                        bool zgsz3475;
                        if (zgaz3138) {  zgsz3475 = eq_bool(zuz3177, true);  } else {  zgsz3475 = false;  }
                        zgaz3139 = zgsz3475;
                      }
                      bool zgsz3476;
                      if (zgaz3139) {  zgsz3476 = eq_bool(zuz3176, true);  } else {  zgsz3476 = false;  }
                      zgaz3140 = zgsz3476;
                    }
                    bool zgsz3477;
                    if (zgaz3140) {  zgsz3477 = eq_bool(zuz3175, true);  } else {  zgsz3477 = false;  }
                    zgaz3141 = zgsz3477;
                  }
                  bool zgsz3478;
                  if (zgaz3141) {  zgsz3478 = eq_bool(zuz3174, true);  } else {  zgsz3478 = false;  }
                  bool zgsz3482;
                  zgsz3482 = zgsz3478;
                  if (!(zgsz3482)) {  goto case_392;  }
                  uint64_t zgaz3130;
                  zgaz3130 = sailgen_execution_mode_encdec_forwards_infallible(zmapping6z3);
                  struct zoptionzIbzK zgsz3479;
                  CREATE(zoptionzIbzK)(&zgsz3479);
                  {
                    uint64_t zuz3182;
                    zuz3182 = zgaz3130;
                    uint64_t zgaz3133;
                    {
                      uint64_t zgaz3132;
                      {
                        uint64_t zgaz3131;
                        zgaz3131 = zuz3182;
                        zgaz3132 = (UINT64_C(0b10) << 1) | zgaz3131;
                      }
                      zgaz3133 = (UINT64_C(0b01) << 3) | zgaz3132;
                    }
                    {
                      RECREATE(lbits)(&zghz3130);
                      CONVERT_OF(lbits, fbits)(&zghz3130, zgaz3133, UINT64_C(5) , true);
                      sailgen_SomezIbzK(&zgsz3479, zghz3130);
                    }
                    goto finish_match_393;
                  }
                case_395: ;
                  {
                    sailgen_NonezIbzK(&zgsz3479, UNIT);
                    goto finish_match_393;
                  }
                case_394: ;
                finish_match_393: ;
                  COPY(zoptionzIbzK)(&zgaz3142, zgsz3479);
                  KILL(zoptionzIbzK)(&zgsz3479);
                  goto finish_match_390;
                }
              case_392: ;
                {
                  sailgen_NonezIbzK(&zgaz3142, UNIT);
                  goto finish_match_390;
                }
              case_391: ;
              finish_match_390: ;
              }
              uint64_t zgsz3483;
              {
                if (zgaz3142.kind != Kind_zSomezIbzK) goto case_460;
                zgsz3483 = CONVERT_OF(fbits, lbits)(zgaz3142.variants.zSomezIbzK, true);
                goto finish_match_396;
              }
            case_460: ;
              {
                __label__ case_459, case_405, finish_match_404;

                /* complete */
                struct zoptionzIbzK zgaz3155;
                CREATE(zoptionzIbzK)(&zgaz3155);
                {
                  __label__ case_400, case_399, finish_match_398;

                  {
                    __label__ case_403, case_402, finish_match_401;

                    bool zuz3184;
                    zuz3184 = zhead_expz3.ztup0.zR;
                    bool zuz3185;
                    zuz3185 = zhead_expz3.ztup0.zW;
                    bool zuz3186;
                    zuz3186 = zhead_expz3.ztup0.zC;
                    bool zuz3187;
                    zuz3187 = zhead_expz3.ztup0.zLM;
                    bool zuz3188;
                    zuz3188 = zhead_expz3.ztup0.zEL;
                    bool zuz3189;
                    zuz3189 = zhead_expz3.ztup0.zSL;
                    bool zuz3190;
                    zuz3190 = zhead_expz3.ztup0.zX;
                    bool zuz3191;
                    zuz3191 = zhead_expz3.ztup0.zASR;
                    enum zExecutionMode zmapping7z3;
                    zmapping7z3 = zhead_expz3.ztup1;
                    bool zgaz3154;
                    {
                      bool zgaz3153;
                      {
                        bool zgaz3152;
                        {
                          bool zgaz3151;
                          {
                            bool zgaz3150;
                            {
                              bool zgaz3149;
                              {
                                bool zgaz3148;
                                {
                                  bool zgaz3147;
                                  zgaz3147 = sailgen_execution_mode_encdec_forwards_matches(zmapping7z3);
                                  bool zgsz3486;
                                  if (zgaz3147) {  zgsz3486 = eq_bool(zuz3191, false);  } else {  zgsz3486 = false;  }
                                  zgaz3148 = zgsz3486;
                                }
                                bool zgsz3487;
                                if (zgaz3148) {  zgsz3487 = eq_bool(zuz3190, true);  } else {  zgsz3487 = false;  }
                                zgaz3149 = zgsz3487;
                              }
                              bool zgsz3488;
                              if (zgaz3149) {  zgsz3488 = eq_bool(zuz3189, false);  } else {  zgsz3488 = false;  }
                              zgaz3150 = zgsz3488;
                            }
                            bool zgsz3489;
                            if (zgaz3150) {  zgsz3489 = eq_bool(zuz3188, false);  } else {  zgsz3489 = false;  }
                            zgaz3151 = zgsz3489;
                          }
                          bool zgsz3490;
                          if (zgaz3151) {  zgsz3490 = eq_bool(zuz3187, false);  } else {  zgsz3490 = false;  }
                          zgaz3152 = zgsz3490;
                        }
                        bool zgsz3491;
                        if (zgaz3152) {  zgsz3491 = eq_bool(zuz3186, false);  } else {  zgsz3491 = false;  }
                        zgaz3153 = zgsz3491;
                      }
                      bool zgsz3492;
                      if (zgaz3153) {  zgsz3492 = eq_bool(zuz3185, true);  } else {  zgsz3492 = false;  }
                      zgaz3154 = zgsz3492;
                    }
                    bool zgsz3493;
                    if (zgaz3154) {  zgsz3493 = eq_bool(zuz3184, true);  } else {  zgsz3493 = false;  }
                    bool zgsz3497;
                    zgsz3497 = zgsz3493;
                    if (!(zgsz3497)) {  goto case_400;  }
                    uint64_t zgaz3143;
                    zgaz3143 = sailgen_execution_mode_encdec_forwards_infallible(zmapping7z3);
                    struct zoptionzIbzK zgsz3494;
                    CREATE(zoptionzIbzK)(&zgsz3494);
                    {
                      uint64_t zuz3192;
                      zuz3192 = zgaz3143;
                      uint64_t zgaz3146;
                      {
                        uint64_t zgaz3145;
                        {
                          uint64_t zgaz3144;
                          zgaz3144 = zuz3192;
                          zgaz3145 = (UINT64_C(0b11) << 1) | zgaz3144;
                        }
                        zgaz3146 = (UINT64_C(0b01) << 3) | zgaz3145;
                      }
                      {
                        RECREATE(lbits)(&zghz3129);
                        CONVERT_OF(lbits, fbits)(&zghz3129, zgaz3146, UINT64_C(5) , true);
                        sailgen_SomezIbzK(&zgsz3494, zghz3129);
                      }
                      goto finish_match_401;
                    }
                  case_403: ;
                    {
                      sailgen_NonezIbzK(&zgsz3494, UNIT);
                      goto finish_match_401;
                    }
                  case_402: ;
                  finish_match_401: ;
                    COPY(zoptionzIbzK)(&zgaz3155, zgsz3494);
                    KILL(zoptionzIbzK)(&zgsz3494);
                    goto finish_match_398;
                  }
                case_400: ;
                  {
                    sailgen_NonezIbzK(&zgaz3155, UNIT);
                    goto finish_match_398;
                  }
                case_399: ;
                finish_match_398: ;
                }
                uint64_t zgsz3498;
                {
                  if (zgaz3155.kind != Kind_zSomezIbzK) goto case_459;
                  zgsz3498 = CONVERT_OF(fbits, lbits)(zgaz3155.variants.zSomezIbzK, true);
                  goto finish_match_404;
                }
              case_459: ;
                {
                  __label__ case_458, case_413, finish_match_412;

                  /* complete */
                  struct zoptionzIbzK zgaz3165;
                  CREATE(zoptionzIbzK)(&zgaz3165);
                  {
                    __label__ case_408, case_407, finish_match_406;

                    {
                      __label__ case_411, case_410, finish_match_409;

                      bool zuz3194;
                      zuz3194 = zhead_expz3.ztup0.zR;
                      bool zuz3195;
                      zuz3195 = zhead_expz3.ztup0.zW;
                      bool zuz3196;
                      zuz3196 = zhead_expz3.ztup0.zC;
                      bool zuz3197;
                      zuz3197 = zhead_expz3.ztup0.zLM;
                      bool zuz3198;
                      zuz3198 = zhead_expz3.ztup0.zEL;
                      bool zuz3199;
                      zuz3199 = zhead_expz3.ztup0.zSL;
                      bool zuz3200;
                      zuz3200 = zhead_expz3.ztup0.zX;
                      bool zuz3201;
                      zuz3201 = zhead_expz3.ztup0.zASR;
                      enum zExecutionMode zmapping8z3;
                      zmapping8z3 = zhead_expz3.ztup1;
                      bool zgaz3164;
                      {
                        bool zgaz3163;
                        {
                          bool zgaz3162;
                          {
                            bool zgaz3161;
                            {
                              bool zgaz3160;
                              {
                                bool zgaz3159;
                                {
                                  bool zgaz3158;
                                  {
                                    bool zgaz3157;
                                    zgaz3157 = sailgen_execution_mode_encdec_forwards_matches(zmapping8z3);
                                    bool zgsz3501;
                                    if (zgaz3157) {  zgsz3501 = eq_bool(zuz3201, false);  } else {  zgsz3501 = false;  }
                                    zgaz3158 = zgsz3501;
                                  }
                                  bool zgsz3502;
                                  if (zgaz3158) {  zgsz3502 = eq_bool(zuz3200, false);  } else {  zgsz3502 = false;  }
                                  zgaz3159 = zgsz3502;
                                }
                                bool zgsz3503;
                                if (zgaz3159) {  zgsz3503 = eq_bool(zuz3199, false);  } else {  zgsz3503 = false;  }
                                zgaz3160 = zgsz3503;
                              }
                              bool zgsz3504;
                              if (zgaz3160) {  zgsz3504 = eq_bool(zuz3198, false);  } else {  zgsz3504 = false;  }
                              zgaz3161 = zgsz3504;
                            }
                            bool zgsz3505;
                            if (zgaz3161) {  zgsz3505 = eq_bool(zuz3197, false);  } else {  zgsz3505 = false;  }
                            zgaz3162 = zgsz3505;
                          }
                          bool zgsz3506;
                          if (zgaz3162) {  zgsz3506 = eq_bool(zuz3196, true);  } else {  zgsz3506 = false;  }
                          zgaz3163 = zgsz3506;
                        }
                        bool zgsz3507;
                        if (zgaz3163) {  zgsz3507 = eq_bool(zuz3195, false);  } else {  zgsz3507 = false;  }
                        zgaz3164 = zgsz3507;
                      }
                      bool zgsz3508;
                      if (zgaz3164) {  zgsz3508 = eq_bool(zuz3194, true);  } else {  zgsz3508 = false;  }
                      bool zgsz3512;
                      zgsz3512 = zgsz3508;
                      if (!(zgsz3512)) {  goto case_408;  }
                      uint64_t zgaz3156;
                      zgaz3156 = sailgen_execution_mode_encdec_forwards_infallible(zmapping8z3);
                      struct zoptionzIbzK zgsz3509;
                      CREATE(zoptionzIbzK)(&zgsz3509);
                      {
                        uint64_t zb__5;
                        zb__5 = zgaz3156;
                        bool zgsz3511;
                        zgsz3511 = (zb__5 == UINT64_C(0b0));
                        if (!(zgsz3511)) {  goto case_411;  }
                        {
                          RECREATE(lbits)(&zghz3128);
                          CONVERT_OF(lbits, fbits)(&zghz3128, UINT64_C(0b10011), UINT64_C(5) , true);
                          sailgen_SomezIbzK(&zgsz3509, zghz3128);
                        }
                        goto finish_match_409;
                      }
                    case_411: ;
                      {
                        sailgen_NonezIbzK(&zgsz3509, UNIT);
                        goto finish_match_409;
                      }
                    case_410: ;
                    finish_match_409: ;
                      COPY(zoptionzIbzK)(&zgaz3165, zgsz3509);
                      KILL(zoptionzIbzK)(&zgsz3509);
                      goto finish_match_406;
                    }
                  case_408: ;
                    {
                      sailgen_NonezIbzK(&zgaz3165, UNIT);
                      goto finish_match_406;
                    }
                  case_407: ;
                  finish_match_406: ;
                  }
                  uint64_t zgsz3513;
                  {
                    if (zgaz3165.kind != Kind_zSomezIbzK) goto case_458;
                    zgsz3513 = CONVERT_OF(fbits, lbits)(zgaz3165.variants.zSomezIbzK, true);
                    goto finish_match_412;
                  }
                case_458: ;
                  {
                    __label__ case_457, case_421, finish_match_420;

                    /* complete */
                    struct zoptionzIbzK zgaz3175;
                    CREATE(zoptionzIbzK)(&zgaz3175);
                    {
                      __label__ case_416, case_415, finish_match_414;

                      {
                        __label__ case_419, case_418, finish_match_417;

                        bool zuz3203;
                        zuz3203 = zhead_expz3.ztup0.zR;
                        bool zuz3204;
                        zuz3204 = zhead_expz3.ztup0.zW;
                        bool zuz3205;
                        zuz3205 = zhead_expz3.ztup0.zC;
                        bool zuz3206;
                        zuz3206 = zhead_expz3.ztup0.zLM;
                        bool zuz3207;
                        zuz3207 = zhead_expz3.ztup0.zEL;
                        bool zuz3208;
                        zuz3208 = zhead_expz3.ztup0.zSL;
                        bool zuz3209;
                        zuz3209 = zhead_expz3.ztup0.zX;
                        bool zuz3210;
                        zuz3210 = zhead_expz3.ztup0.zASR;
                        enum zExecutionMode zmapping9z3;
                        zmapping9z3 = zhead_expz3.ztup1;
                        bool zgaz3174;
                        {
                          bool zgaz3173;
                          {
                            bool zgaz3172;
                            {
                              bool zgaz3171;
                              {
                                bool zgaz3170;
                                {
                                  bool zgaz3169;
                                  {
                                    bool zgaz3168;
                                    {
                                      bool zgaz3167;
                                      zgaz3167 = sailgen_execution_mode_encdec_forwards_matches(zmapping9z3);
                                      bool zgsz3516;
                                      if (zgaz3167) {  zgsz3516 = eq_bool(zuz3210, false);  } else {
                                        zgsz3516 = false;
                                      }
                                      zgaz3168 = zgsz3516;
                                    }
                                    bool zgsz3517;
                                    if (zgaz3168) {  zgsz3517 = eq_bool(zuz3209, false);  } else {  zgsz3517 = false;  }
                                    zgaz3169 = zgsz3517;
                                  }
                                  bool zgsz3518;
                                  if (zgaz3169) {  zgsz3518 = eq_bool(zuz3208, true);  } else {  zgsz3518 = false;  }
                                  zgaz3170 = zgsz3518;
                                }
                                bool zgsz3519;
                                if (zgaz3170) {  zgsz3519 = eq_bool(zuz3207, false);  } else {  zgsz3519 = false;  }
                                zgaz3171 = zgsz3519;
                              }
                              bool zgsz3520;
                              if (zgaz3171) {  zgsz3520 = eq_bool(zuz3206, true);  } else {  zgsz3520 = false;  }
                              zgaz3172 = zgsz3520;
                            }
                            bool zgsz3521;
                            if (zgaz3172) {  zgsz3521 = eq_bool(zuz3205, true);  } else {  zgsz3521 = false;  }
                            zgaz3173 = zgsz3521;
                          }
                          bool zgsz3522;
                          if (zgaz3173) {  zgsz3522 = eq_bool(zuz3204, true);  } else {  zgsz3522 = false;  }
                          zgaz3174 = zgsz3522;
                        }
                        bool zgsz3523;
                        if (zgaz3174) {  zgsz3523 = eq_bool(zuz3203, true);  } else {  zgsz3523 = false;  }
                        bool zgsz3527;
                        zgsz3527 = zgsz3523;
                        if (!(zgsz3527)) {  goto case_416;  }
                        uint64_t zgaz3166;
                        zgaz3166 = sailgen_execution_mode_encdec_forwards_infallible(zmapping9z3);
                        struct zoptionzIbzK zgsz3524;
                        CREATE(zoptionzIbzK)(&zgsz3524);
                        {
                          uint64_t zb__4;
                          zb__4 = zgaz3166;
                          bool zgsz3526;
                          zgsz3526 = (zb__4 == UINT64_C(0b0));
                          if (!(zgsz3526)) {  goto case_419;  }
                          {
                            RECREATE(lbits)(&zghz3127);
                            CONVERT_OF(lbits, fbits)(&zghz3127, UINT64_C(0b10110), UINT64_C(5) , true);
                            sailgen_SomezIbzK(&zgsz3524, zghz3127);
                          }
                          goto finish_match_417;
                        }
                      case_419: ;
                        {
                          sailgen_NonezIbzK(&zgsz3524, UNIT);
                          goto finish_match_417;
                        }
                      case_418: ;
                      finish_match_417: ;
                        COPY(zoptionzIbzK)(&zgaz3175, zgsz3524);
                        KILL(zoptionzIbzK)(&zgsz3524);
                        goto finish_match_414;
                      }
                    case_416: ;
                      {
                        sailgen_NonezIbzK(&zgaz3175, UNIT);
                        goto finish_match_414;
                      }
                    case_415: ;
                    finish_match_414: ;
                    }
                    uint64_t zgsz3528;
                    {
                      if (zgaz3175.kind != Kind_zSomezIbzK) goto case_457;
                      zgsz3528 = CONVERT_OF(fbits, lbits)(zgaz3175.variants.zSomezIbzK, true);
                      goto finish_match_420;
                    }
                  case_457: ;
                    {
                      __label__ case_456, case_429, finish_match_428;

                      /* complete */
                      struct zoptionzIbzK zgaz3185;
                      CREATE(zoptionzIbzK)(&zgaz3185);
                      {
                        __label__ case_424, case_423, finish_match_422;

                        {
                          __label__ case_427, case_426, finish_match_425;

                          bool zuz3212;
                          zuz3212 = zhead_expz3.ztup0.zR;
                          bool zuz3213;
                          zuz3213 = zhead_expz3.ztup0.zW;
                          bool zuz3214;
                          zuz3214 = zhead_expz3.ztup0.zC;
                          bool zuz3215;
                          zuz3215 = zhead_expz3.ztup0.zLM;
                          bool zuz3216;
                          zuz3216 = zhead_expz3.ztup0.zEL;
                          bool zuz3217;
                          zuz3217 = zhead_expz3.ztup0.zSL;
                          bool zuz3218;
                          zuz3218 = zhead_expz3.ztup0.zX;
                          bool zuz3219;
                          zuz3219 = zhead_expz3.ztup0.zASR;
                          enum zExecutionMode zmapping10z3;
                          zmapping10z3 = zhead_expz3.ztup1;
                          bool zgaz3184;
                          {
                            bool zgaz3183;
                            {
                              bool zgaz3182;
                              {
                                bool zgaz3181;
                                {
                                  bool zgaz3180;
                                  {
                                    bool zgaz3179;
                                    {
                                      bool zgaz3178;
                                      {
                                        bool zgaz3177;
                                        zgaz3177 = sailgen_execution_mode_encdec_forwards_matches(zmapping10z3);
                                        bool zgsz3531;
                                        if (zgaz3177) {  zgsz3531 = eq_bool(zuz3219, false);  } else {
                                          zgsz3531 = false;
                                        }
                                        zgaz3178 = zgsz3531;
                                      }
                                      bool zgsz3532;
                                      if (zgaz3178) {  zgsz3532 = eq_bool(zuz3218, false);  } else {
                                        zgsz3532 = false;
                                      }
                                      zgaz3179 = zgsz3532;
                                    }
                                    bool zgsz3533;
                                    if (zgaz3179) {  zgsz3533 = eq_bool(zuz3217, false);  } else {  zgsz3533 = false;  }
                                    zgaz3180 = zgsz3533;
                                  }
                                  bool zgsz3534;
                                  if (zgaz3180) {  zgsz3534 = eq_bool(zuz3216, false);  } else {  zgsz3534 = false;  }
                                  zgaz3181 = zgsz3534;
                                }
                                bool zgsz3535;
                                if (zgaz3181) {  zgsz3535 = eq_bool(zuz3215, true);  } else {  zgsz3535 = false;  }
                                zgaz3182 = zgsz3535;
                              }
                              bool zgsz3536;
                              if (zgaz3182) {  zgsz3536 = eq_bool(zuz3214, true);  } else {  zgsz3536 = false;  }
                              zgaz3183 = zgsz3536;
                            }
                            bool zgsz3537;
                            if (zgaz3183) {  zgsz3537 = eq_bool(zuz3213, true);  } else {  zgsz3537 = false;  }
                            zgaz3184 = zgsz3537;
                          }
                          bool zgsz3538;
                          if (zgaz3184) {  zgsz3538 = eq_bool(zuz3212, true);  } else {  zgsz3538 = false;  }
                          bool zgsz3542;
                          zgsz3542 = zgsz3538;
                          if (!(zgsz3542)) {  goto case_424;  }
                          uint64_t zgaz3176;
                          zgaz3176 = sailgen_execution_mode_encdec_forwards_infallible(zmapping10z3);
                          struct zoptionzIbzK zgsz3539;
                          CREATE(zoptionzIbzK)(&zgsz3539);
                          {
                            uint64_t zb__3;
                            zb__3 = zgaz3176;
                            bool zgsz3541;
                            zgsz3541 = (zb__3 == UINT64_C(0b0));
                            if (!(zgsz3541)) {  goto case_427;  }
                            {
                              RECREATE(lbits)(&zghz3126);
                              CONVERT_OF(lbits, fbits)(&zghz3126, UINT64_C(0b10111), UINT64_C(5) , true);
                              sailgen_SomezIbzK(&zgsz3539, zghz3126);
                            }
                            goto finish_match_425;
                          }
                        case_427: ;
                          {
                            sailgen_NonezIbzK(&zgsz3539, UNIT);
                            goto finish_match_425;
                          }
                        case_426: ;
                        finish_match_425: ;
                          COPY(zoptionzIbzK)(&zgaz3185, zgsz3539);
                          KILL(zoptionzIbzK)(&zgsz3539);
                          goto finish_match_422;
                        }
                      case_424: ;
                        {
                          sailgen_NonezIbzK(&zgaz3185, UNIT);
                          goto finish_match_422;
                        }
                      case_423: ;
                      finish_match_422: ;
                      }
                      uint64_t zgsz3543;
                      {
                        if (zgaz3185.kind != Kind_zSomezIbzK) goto case_456;
                        zgsz3543 = CONVERT_OF(fbits, lbits)(zgaz3185.variants.zSomezIbzK, true);
                        goto finish_match_428;
                      }
                    case_456: ;
                      {
                        __label__ case_455, case_437, finish_match_436;

                        /* complete */
                        struct zoptionzIbzK zgaz3195;
                        CREATE(zoptionzIbzK)(&zgaz3195);
                        {
                          __label__ case_432, case_431, finish_match_430;

                          {
                            __label__ case_435, case_434, finish_match_433;

                            bool zuz3221;
                            zuz3221 = zhead_expz3.ztup0.zR;
                            bool zuz3222;
                            zuz3222 = zhead_expz3.ztup0.zW;
                            bool zuz3223;
                            zuz3223 = zhead_expz3.ztup0.zC;
                            bool zuz3224;
                            zuz3224 = zhead_expz3.ztup0.zLM;
                            bool zuz3225;
                            zuz3225 = zhead_expz3.ztup0.zEL;
                            bool zuz3226;
                            zuz3226 = zhead_expz3.ztup0.zSL;
                            bool zuz3227;
                            zuz3227 = zhead_expz3.ztup0.zX;
                            bool zuz3228;
                            zuz3228 = zhead_expz3.ztup0.zASR;
                            enum zExecutionMode zmapping11z3;
                            zmapping11z3 = zhead_expz3.ztup1;
                            bool zgaz3194;
                            {
                              bool zgaz3193;
                              {
                                bool zgaz3192;
                                {
                                  bool zgaz3191;
                                  {
                                    bool zgaz3190;
                                    {
                                      bool zgaz3189;
                                      {
                                        bool zgaz3188;
                                        {
                                          bool zgaz3187;
                                          zgaz3187 = sailgen_execution_mode_encdec_forwards_matches(zmapping11z3);
                                          bool zgsz3546;
                                          if (zgaz3187) {  zgsz3546 = eq_bool(zuz3228, false);  } else {
                                            zgsz3546 = false;
                                          }
                                          zgaz3188 = zgsz3546;
                                        }
                                        bool zgsz3547;
                                        if (zgaz3188) {  zgsz3547 = eq_bool(zuz3227, false);  } else {
                                          zgsz3547 = false;
                                        }
                                        zgaz3189 = zgsz3547;
                                      }
                                      bool zgsz3548;
                                      if (zgaz3189) {  zgsz3548 = eq_bool(zuz3226, false);  } else {
                                        zgsz3548 = false;
                                      }
                                      zgaz3190 = zgsz3548;
                                    }
                                    bool zgsz3549;
                                    if (zgaz3190) {  zgsz3549 = eq_bool(zuz3225, true);  } else {  zgsz3549 = false;  }
                                    zgaz3191 = zgsz3549;
                                  }
                                  bool zgsz3550;
                                  if (zgaz3191) {  zgsz3550 = eq_bool(zuz3224, true);  } else {  zgsz3550 = false;  }
                                  zgaz3192 = zgsz3550;
                                }
                                bool zgsz3551;
                                if (zgaz3192) {  zgsz3551 = eq_bool(zuz3223, true);  } else {  zgsz3551 = false;  }
                                zgaz3193 = zgsz3551;
                              }
                              bool zgsz3552;
                              if (zgaz3193) {  zgsz3552 = eq_bool(zuz3222, false);  } else {  zgsz3552 = false;  }
                              zgaz3194 = zgsz3552;
                            }
                            bool zgsz3553;
                            if (zgaz3194) {  zgsz3553 = eq_bool(zuz3221, true);  } else {  zgsz3553 = false;  }
                            bool zgsz3557;
                            zgsz3557 = zgsz3553;
                            if (!(zgsz3557)) {  goto case_432;  }
                            uint64_t zgaz3186;
                            zgaz3186 = sailgen_execution_mode_encdec_forwards_infallible(zmapping11z3);
                            struct zoptionzIbzK zgsz3554;
                            CREATE(zoptionzIbzK)(&zgsz3554);
                            {
                              uint64_t zb__2;
                              zb__2 = zgaz3186;
                              bool zgsz3556;
                              zgsz3556 = (zb__2 == UINT64_C(0b0));
                              if (!(zgsz3556)) {  goto case_435;  }
                              {
                                RECREATE(lbits)(&zghz3125);
                                CONVERT_OF(lbits, fbits)(&zghz3125, UINT64_C(0b11011), UINT64_C(5) , true);
                                sailgen_SomezIbzK(&zgsz3554, zghz3125);
                              }
                              goto finish_match_433;
                            }
                          case_435: ;
                            {
                              sailgen_NonezIbzK(&zgsz3554, UNIT);
                              goto finish_match_433;
                            }
                          case_434: ;
                          finish_match_433: ;
                            COPY(zoptionzIbzK)(&zgaz3195, zgsz3554);
                            KILL(zoptionzIbzK)(&zgsz3554);
                            goto finish_match_430;
                          }
                        case_432: ;
                          {
                            sailgen_NonezIbzK(&zgaz3195, UNIT);
                            goto finish_match_430;
                          }
                        case_431: ;
                        finish_match_430: ;
                        }
                        uint64_t zgsz3558;
                        {
                          if (zgaz3195.kind != Kind_zSomezIbzK) goto case_455;
                          zgsz3558 = CONVERT_OF(fbits, lbits)(zgaz3195.variants.zSomezIbzK, true);
                          goto finish_match_436;
                        }
                      case_455: ;
                        {
                          __label__ case_454, case_445, finish_match_444;

                          /* complete */
                          struct zoptionzIbzK zgaz3205;
                          CREATE(zoptionzIbzK)(&zgaz3205);
                          {
                            __label__ case_440, case_439, finish_match_438;

                            {
                              __label__ case_443, case_442, finish_match_441;

                              bool zuz3230;
                              zuz3230 = zhead_expz3.ztup0.zR;
                              bool zuz3231;
                              zuz3231 = zhead_expz3.ztup0.zW;
                              bool zuz3232;
                              zuz3232 = zhead_expz3.ztup0.zC;
                              bool zuz3233;
                              zuz3233 = zhead_expz3.ztup0.zLM;
                              bool zuz3234;
                              zuz3234 = zhead_expz3.ztup0.zEL;
                              bool zuz3235;
                              zuz3235 = zhead_expz3.ztup0.zSL;
                              bool zuz3236;
                              zuz3236 = zhead_expz3.ztup0.zX;
                              bool zuz3237;
                              zuz3237 = zhead_expz3.ztup0.zASR;
                              enum zExecutionMode zmapping12z3;
                              zmapping12z3 = zhead_expz3.ztup1;
                              bool zgaz3204;
                              {
                                bool zgaz3203;
                                {
                                  bool zgaz3202;
                                  {
                                    bool zgaz3201;
                                    {
                                      bool zgaz3200;
                                      {
                                        bool zgaz3199;
                                        {
                                          bool zgaz3198;
                                          {
                                            bool zgaz3197;
                                            zgaz3197 = sailgen_execution_mode_encdec_forwards_matches(zmapping12z3);
                                            bool zgsz3561;
                                            if (zgaz3197) {  zgsz3561 = eq_bool(zuz3237, false);  } else {
                                              zgsz3561 = false;
                                            }
                                            zgaz3198 = zgsz3561;
                                          }
                                          bool zgsz3562;
                                          if (zgaz3198) {  zgsz3562 = eq_bool(zuz3236, false);  } else {
                                            zgsz3562 = false;
                                          }
                                          zgaz3199 = zgsz3562;
                                        }
                                        bool zgsz3563;
                                        if (zgaz3199) {  zgsz3563 = eq_bool(zuz3235, true);  } else {
                                          zgsz3563 = false;
                                        }
                                        zgaz3200 = zgsz3563;
                                      }
                                      bool zgsz3564;
                                      if (zgaz3200) {  zgsz3564 = eq_bool(zuz3234, true);  } else {
                                        zgsz3564 = false;
                                      }
                                      zgaz3201 = zgsz3564;
                                    }
                                    bool zgsz3565;
                                    if (zgaz3201) {  zgsz3565 = eq_bool(zuz3233, true);  } else {  zgsz3565 = false;  }
                                    zgaz3202 = zgsz3565;
                                  }
                                  bool zgsz3566;
                                  if (zgaz3202) {  zgsz3566 = eq_bool(zuz3232, true);  } else {  zgsz3566 = false;  }
                                  zgaz3203 = zgsz3566;
                                }
                                bool zgsz3567;
                                if (zgaz3203) {  zgsz3567 = eq_bool(zuz3231, true);  } else {  zgsz3567 = false;  }
                                zgaz3204 = zgsz3567;
                              }
                              bool zgsz3568;
                              if (zgaz3204) {  zgsz3568 = eq_bool(zuz3230, true);  } else {  zgsz3568 = false;  }
                              bool zgsz3572;
                              zgsz3572 = zgsz3568;
                              if (!(zgsz3572)) {  goto case_440;  }
                              uint64_t zgaz3196;
                              zgaz3196 = sailgen_execution_mode_encdec_forwards_infallible(zmapping12z3);
                              struct zoptionzIbzK zgsz3569;
                              CREATE(zoptionzIbzK)(&zgsz3569);
                              {
                                uint64_t zb__1;
                                zb__1 = zgaz3196;
                                bool zgsz3571;
                                zgsz3571 = (zb__1 == UINT64_C(0b0));
                                if (!(zgsz3571)) {  goto case_443;  }
                                {
                                  RECREATE(lbits)(&zghz3124);
                                  CONVERT_OF(lbits, fbits)(&zghz3124, UINT64_C(0b11110), UINT64_C(5) , true);
                                  sailgen_SomezIbzK(&zgsz3569, zghz3124);
                                }
                                goto finish_match_441;
                              }
                            case_443: ;
                              {
                                sailgen_NonezIbzK(&zgsz3569, UNIT);
                                goto finish_match_441;
                              }
                            case_442: ;
                            finish_match_441: ;
                              COPY(zoptionzIbzK)(&zgaz3205, zgsz3569);
                              KILL(zoptionzIbzK)(&zgsz3569);
                              goto finish_match_438;
                            }
                          case_440: ;
                            {
                              sailgen_NonezIbzK(&zgaz3205, UNIT);
                              goto finish_match_438;
                            }
                          case_439: ;
                          finish_match_438: ;
                          }
                          uint64_t zgsz3573;
                          {
                            if (zgaz3205.kind != Kind_zSomezIbzK) goto case_454;
                            zgsz3573 = CONVERT_OF(fbits, lbits)(zgaz3205.variants.zSomezIbzK, true);
                            goto finish_match_444;
                          }
                        case_454: ;
                          {
                            __label__ case_453, finish_match_452;

                            /* complete */
                            struct zoptionzIbzK zgaz3215;
                            CREATE(zoptionzIbzK)(&zgaz3215);
                            {
                              __label__ case_448, case_447, finish_match_446;

                              {
                                __label__ case_451, case_450, finish_match_449;

                                bool zuz3239;
                                zuz3239 = zhead_expz3.ztup0.zR;
                                bool zuz3240;
                                zuz3240 = zhead_expz3.ztup0.zW;
                                bool zuz3241;
                                zuz3241 = zhead_expz3.ztup0.zC;
                                bool zuz3242;
                                zuz3242 = zhead_expz3.ztup0.zLM;
                                bool zuz3243;
                                zuz3243 = zhead_expz3.ztup0.zEL;
                                bool zuz3244;
                                zuz3244 = zhead_expz3.ztup0.zSL;
                                bool zuz3245;
                                zuz3245 = zhead_expz3.ztup0.zX;
                                bool zuz3246;
                                zuz3246 = zhead_expz3.ztup0.zASR;
                                enum zExecutionMode zmapping13z3;
                                zmapping13z3 = zhead_expz3.ztup1;
                                bool zgaz3214;
                                {
                                  bool zgaz3213;
                                  {
                                    bool zgaz3212;
                                    {
                                      bool zgaz3211;
                                      {
                                        bool zgaz3210;
                                        {
                                          bool zgaz3209;
                                          {
                                            bool zgaz3208;
                                            {
                                              bool zgaz3207;
                                              zgaz3207 = sailgen_execution_mode_encdec_forwards_matches(zmapping13z3);
                                              bool zgsz3576;
                                              if (zgaz3207) {  zgsz3576 = eq_bool(zuz3246, false);  } else {
                                                zgsz3576 = false;
                                              }
                                              zgaz3208 = zgsz3576;
                                            }
                                            bool zgsz3577;
                                            if (zgaz3208) {  zgsz3577 = eq_bool(zuz3245, false);  } else {
                                              zgsz3577 = false;
                                            }
                                            zgaz3209 = zgsz3577;
                                          }
                                          bool zgsz3578;
                                          if (zgaz3209) {  zgsz3578 = eq_bool(zuz3244, false);  } else {
                                            zgsz3578 = false;
                                          }
                                          zgaz3210 = zgsz3578;
                                        }
                                        bool zgsz3579;
                                        if (zgaz3210) {  zgsz3579 = eq_bool(zuz3243, true);  } else {
                                          zgsz3579 = false;
                                        }
                                        zgaz3211 = zgsz3579;
                                      }
                                      bool zgsz3580;
                                      if (zgaz3211) {  zgsz3580 = eq_bool(zuz3242, true);  } else {
                                        zgsz3580 = false;
                                      }
                                      zgaz3212 = zgsz3580;
                                    }
                                    bool zgsz3581;
                                    if (zgaz3212) {  zgsz3581 = eq_bool(zuz3241, true);  } else {  zgsz3581 = false;  }
                                    zgaz3213 = zgsz3581;
                                  }
                                  bool zgsz3582;
                                  if (zgaz3213) {  zgsz3582 = eq_bool(zuz3240, true);  } else {  zgsz3582 = false;  }
                                  zgaz3214 = zgsz3582;
                                }
                                bool zgsz3583;
                                if (zgaz3214) {  zgsz3583 = eq_bool(zuz3239, true);  } else {  zgsz3583 = false;  }
                                bool zgsz3587;
                                zgsz3587 = zgsz3583;
                                if (!(zgsz3587)) {  goto case_448;  }
                                uint64_t zgaz3206;
                                zgaz3206 = sailgen_execution_mode_encdec_forwards_infallible(zmapping13z3);
                                struct zoptionzIbzK zgsz3584;
                                CREATE(zoptionzIbzK)(&zgsz3584);
                                {
                                  uint64_t zb__0;
                                  zb__0 = zgaz3206;
                                  bool zgsz3586;
                                  zgsz3586 = (zb__0 == UINT64_C(0b0));
                                  if (!(zgsz3586)) {  goto case_451;  }
                                  {
                                    RECREATE(lbits)(&zghz3123);
                                    CONVERT_OF(lbits, fbits)(&zghz3123, UINT64_C(0b11111), UINT64_C(5) , true);
                                    sailgen_SomezIbzK(&zgsz3584, zghz3123);
                                  }
                                  goto finish_match_449;
                                }
                              case_451: ;
                                {
                                  sailgen_NonezIbzK(&zgsz3584, UNIT);
                                  goto finish_match_449;
                                }
                              case_450: ;
                              finish_match_449: ;
                                COPY(zoptionzIbzK)(&zgaz3215, zgsz3584);
                                KILL(zoptionzIbzK)(&zgsz3584);
                                goto finish_match_446;
                              }
                            case_448: ;
                              {
                                sailgen_NonezIbzK(&zgaz3215, UNIT);
                                goto finish_match_446;
                              }
                            case_447: ;
                            finish_match_446: ;
                            }
                            uint64_t zgsz3588;
                            {
                              if (zgaz3215.kind != Kind_zSomezIbzK) goto case_453;
                              zgsz3588 = CONVERT_OF(fbits, lbits)(zgaz3215.variants.zSomezIbzK, true);
                              goto finish_match_452;
                            }
                          case_453: ;
                            sail_match_failure("ap_m_encdec_backwards_infallible");
                          finish_match_452: ;
                            zgsz3573 = zgsz3588;
                            KILL(zoptionzIbzK)(&zgaz3215);
                            goto finish_match_444;
                          }
                        case_445: ;
                        finish_match_444: ;
                          zgsz3558 = zgsz3573;
                          KILL(zoptionzIbzK)(&zgaz3205);
                          goto finish_match_436;
                        }
                      case_437: ;
                      finish_match_436: ;
                        zgsz3543 = zgsz3558;
                        KILL(zoptionzIbzK)(&zgaz3195);
                        goto finish_match_428;
                      }
                    case_429: ;
                    finish_match_428: ;
                      zgsz3528 = zgsz3543;
                      KILL(zoptionzIbzK)(&zgaz3185);
                      goto finish_match_420;
                    }
                  case_421: ;
                  finish_match_420: ;
                    zgsz3513 = zgsz3528;
                    KILL(zoptionzIbzK)(&zgaz3175);
                    goto finish_match_412;
                  }
                case_413: ;
                finish_match_412: ;
                  zgsz3498 = zgsz3513;
                  KILL(zoptionzIbzK)(&zgaz3165);
                  goto finish_match_404;
                }
              case_405: ;
              finish_match_404: ;
                zgsz3483 = zgsz3498;
                KILL(zoptionzIbzK)(&zgaz3155);
                goto finish_match_396;
              }
            case_397: ;
            finish_match_396: ;
              zgsz3468 = zgsz3483;
              KILL(zoptionzIbzK)(&zgaz3142);
              goto finish_match_388;
            }
          case_389: ;
          finish_match_388: ;
            zgsz3453 = zgsz3468;
            KILL(zoptionzIbzK)(&zgaz3129);
            goto finish_match_380;
          }
        case_381: ;
        finish_match_380: ;
          zgsz3438 = zgsz3453;
          KILL(zoptionzIbzK)(&zgaz3116);
          goto finish_match_372;
        }
      case_373: ;
      finish_match_372: ;
        zgsz3423 = zgsz3438;
        KILL(zoptionzIbzK)(&zgaz3103);
        goto finish_match_364;
      }
    case_365: ;
    finish_match_364: ;
      zgsz3408 = zgsz3423;
      KILL(zoptionzIbzK)(&zgaz393);
      goto finish_match_356;
    }
  case_357: ;
  finish_match_356: ;
    zcbz350 = zgsz3408;
    KILL(zoptionzIbzK)(&zgaz383);
    goto finish_match_348;
  }
case_349: ;
finish_match_348: ;
  KILL(zoptionzIbzK)(&zgaz373);
end_function_467: ;
  return zcbz350;
end_block_exception_468: ;

  return UINT64_C(0xdeadc0de);
}



static void finish_sailgen_ap_m_encdec_backwards_infallible(void)
{
  KILL(lbits)(&zghz3136);
  KILL(lbits)(&zghz3135);
  KILL(lbits)(&zghz3134);
  KILL(lbits)(&zghz3133);
  KILL(lbits)(&zghz3132);
  KILL(lbits)(&zghz3131);
  KILL(lbits)(&zghz3130);
  KILL(lbits)(&zghz3129);
  KILL(lbits)(&zghz3128);
  KILL(lbits)(&zghz3127);
  KILL(lbits)(&zghz3126);
  KILL(lbits)(&zghz3125);
  KILL(lbits)(&zghz3124);
  KILL(lbits)(&zghz3123);
}

static bool sailgen_ap_m_encdec_forwards_matches(uint64_t zargz3)
{
  __label__ case_485, case_484, case_483, case_482, case_481, case_480, case_479, case_478, case_477, case_476, case_475, case_474, case_473, case_472, case_471, finish_match_470, end_function_486, end_block_exception_487;

  bool zcbz351;
  {
    uint64_t zv__42;
    zv__42 = zargz3;
    bool zgsz3631;
    zgsz3631 = (zv__42 == UINT64_C(0b00000));
    if (!(zgsz3631)) {  goto case_485;  }
    zcbz351 = true;
    goto finish_match_470;
  }
case_485: ;
  {
    uint64_t zv__45;
    zv__45 = zargz3;
    bool zgsz3630;
    zgsz3630 = (zv__45 == UINT64_C(0b00001));
    if (!(zgsz3630)) {  goto case_484;  }
    zcbz351 = true;
    goto finish_match_470;
  }
case_484: ;
  {
    uint64_t zv__48;
    zv__48 = zargz3;
    bool zgsz3629;
    zgsz3629 = (zv__48 == UINT64_C(0b00100));
    if (!(zgsz3629)) {  goto case_483;  }
    zcbz351 = true;
    goto finish_match_470;
  }
case_483: ;
  {
    uint64_t zv__51;
    zv__51 = zargz3;
    bool zgsz3628;
    zgsz3628 = (zv__51 == UINT64_C(0b00101));
    if (!(zgsz3628)) {  goto case_482;  }
    zcbz351 = true;
    goto finish_match_470;
  }
case_482: ;
  {
    uint64_t zv__54;
    zv__54 = zargz3;
    uint64_t zgaz3216;
    zgaz3216 = (safe_rshift(UINT64_MAX, 64 - 4) & (zv__54 >> INT64_C(1)));
    bool zgsz3627;
    zgsz3627 = (zgaz3216 == UINT64_C(0x4));
    if (!(zgsz3627)) {  goto case_481;  }
    zcbz351 = true;
    goto finish_match_470;
  }
case_481: ;
  {
    uint64_t zv__57;
    zv__57 = zargz3;
    uint64_t zgaz3217;
    zgaz3217 = (safe_rshift(UINT64_MAX, 64 - 4) & (zv__57 >> INT64_C(1)));
    bool zgsz3626;
    zgsz3626 = (zgaz3217 == UINT64_C(0x5));
    if (!(zgsz3626)) {  goto case_480;  }
    zcbz351 = true;
    goto finish_match_470;
  }
case_480: ;
  {
    uint64_t zv__60;
    zv__60 = zargz3;
    uint64_t zgaz3218;
    zgaz3218 = (safe_rshift(UINT64_MAX, 64 - 4) & (zv__60 >> INT64_C(1)));
    bool zgsz3625;
    zgsz3625 = (zgaz3218 == UINT64_C(0x6));
    if (!(zgsz3625)) {  goto case_479;  }
    zcbz351 = true;
    goto finish_match_470;
  }
case_479: ;
  {
    uint64_t zv__63;
    zv__63 = zargz3;
    uint64_t zgaz3219;
    zgaz3219 = (safe_rshift(UINT64_MAX, 64 - 4) & (zv__63 >> INT64_C(1)));
    bool zgsz3624;
    zgsz3624 = (zgaz3219 == UINT64_C(0x7));
    if (!(zgsz3624)) {  goto case_478;  }
    zcbz351 = true;
    goto finish_match_470;
  }
case_478: ;
  {
    uint64_t zv__66;
    zv__66 = zargz3;
    bool zgsz3623;
    zgsz3623 = (zv__66 == UINT64_C(0b10011));
    if (!(zgsz3623)) {  goto case_477;  }
    zcbz351 = true;
    goto finish_match_470;
  }
case_477: ;
  {
    uint64_t zv__69;
    zv__69 = zargz3;
    bool zgsz3622;
    zgsz3622 = (zv__69 == UINT64_C(0b10110));
    if (!(zgsz3622)) {  goto case_476;  }
    zcbz351 = true;
    goto finish_match_470;
  }
case_476: ;
  {
    uint64_t zv__72;
    zv__72 = zargz3;
    bool zgsz3621;
    zgsz3621 = (zv__72 == UINT64_C(0b10111));
    if (!(zgsz3621)) {  goto case_475;  }
    zcbz351 = true;
    goto finish_match_470;
  }
case_475: ;
  {
    uint64_t zv__75;
    zv__75 = zargz3;
    bool zgsz3620;
    zgsz3620 = (zv__75 == UINT64_C(0b11011));
    if (!(zgsz3620)) {  goto case_474;  }
    zcbz351 = true;
    goto finish_match_470;
  }
case_474: ;
  {
    uint64_t zv__78;
    zv__78 = zargz3;
    bool zgsz3619;
    zgsz3619 = (zv__78 == UINT64_C(0b11110));
    if (!(zgsz3619)) {  goto case_473;  }
    zcbz351 = true;
    goto finish_match_470;
  }
case_473: ;
  {
    uint64_t zv__81;
    zv__81 = zargz3;
    bool zgsz3618;
    zgsz3618 = (zv__81 == UINT64_C(0b11111));
    if (!(zgsz3618)) {  goto case_472;  }
    zcbz351 = true;
    goto finish_match_470;
  }
case_472: ;
  {
    zcbz351 = false;
    goto finish_match_470;
  }
case_471: ;
finish_match_470: ;
end_function_486: ;
  return zcbz351;
end_block_exception_487: ;

  return false;
}

static uint64_t znull_ap_m;
static void create_letbind_14(void) {


  uint64_t zgsz3632;
  zgsz3632 = UINT64_C(0b00000);
  znull_ap_m = zgsz3632;
let_end_488: ;
}
static void kill_letbind_14(void) {
}

static uint64_t zinfinite_ap_m;
static void create_letbind_15(void) {


  uint64_t zgsz3633;
  zgsz3633 = UINT64_C(0b01001);
  zinfinite_ap_m = zgsz3633;
let_end_489: ;
}
static void kill_letbind_15(void) {
}

static struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 sailgen_legalizzePermsAndMode(struct zArchPerms, enum zExecutionMode);

struct zArchPerms zghz3137;
struct zArchPerms zghz3138;
struct zArchPerms zghz3139;
struct zArchPerms zghz3140;
struct zArchPerms zghz3141;
struct zArchPerms zghz3142;
struct zArchPerms zghz3143;
struct zArchPerms zghz3144;
struct zArchPerms zghz3145;
struct zArchPerms zghz3146;
struct zArchPerms zghz3147;
struct zArchPerms zghz3148;
struct zArchPerms zghz3149;
struct zArchPerms zghz3150;
struct zArchPerms zghz3151;
struct zArchPerms zghz3152;
struct zArchPerms zghz3153;
struct zArchPerms zghz3154;
struct zArchPerms zghz3155;
struct zArchPerms zghz3156;
struct zArchPerms zghz3157;
struct zArchPerms zghz3158;
struct zArchPerms zghz3159;
struct zArchPerms zghz3160;
struct zArchPerms zghz3161;
struct zArchPerms zghz3162;
struct zArchPerms zghz3163;
struct zArchPerms zghz3164;

static void startup_sailgen_legalizzePermsAndMode(void)
{




























}

static struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 sailgen_legalizzePermsAndMode(struct zArchPerms zperms, enum zExecutionMode zmode)
{
  __label__ end_function_491, end_block_exception_492;

  struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zcbz352;
  struct zArchPerms zghz3137;
  {
    bool zgaz3258;
    {
      bool zgaz3257;
      zgaz3257 = zperms.zR;
      bool zgsz3640;
      if (zgaz3257) {
        bool zgaz3256;
        zgaz3256 = zperms.zW;
        bool zgsz3639;
        if (zgaz3256) {
          bool zgaz3255;
          zgaz3255 = zperms.zX;
          bool zgsz3638;
          if (zgaz3255) {
            bool zgaz3254;
            zgaz3254 = zperms.zASR;
            bool zgsz3637;
            if (zgaz3254) {
              bool zgaz3253;
              zgaz3253 = zperms.zC;
              bool zgsz3636;
              if (zgaz3253) {
                bool zgaz3252;
                zgaz3252 = zperms.zLM;
                bool zgsz3635;
                if (zgaz3252) {
                  bool zgaz3251;
                  zgaz3251 = zperms.zEL;
                  bool zgsz3634;
                  if (zgaz3251) {  zgsz3634 = zperms.zSL;  } else {  zgsz3634 = false;  }
                  zgsz3635 = zgsz3634;
                } else {  zgsz3635 = false;  }
                zgsz3636 = zgsz3635;
              } else {  zgsz3636 = false;  }
              zgsz3637 = zgsz3636;
            } else {  zgsz3637 = false;  }
            zgsz3638 = zgsz3637;
          } else {  zgsz3638 = false;  }
          zgsz3639 = zgsz3638;
        } else {  zgsz3639 = false;  }
        zgsz3640 = zgsz3639;
      } else {  zgsz3640 = false;  }
      zgaz3258 = zgsz3640;
    }
    if (zgaz3258) {  zghz3137 = zperms;  } else {
      struct zArchPerms zghz3164;
      zghz3164 = zperms;
      zghz3164.zASR = false;
      zghz3137 = zghz3164;
    }
  }
  struct zArchPerms zghz3138;
  {
    bool zgaz3250;
    {
      bool zgaz3249;
      zgaz3249 = zghz3137.zR;
      bool zgsz3642;
      if (zgaz3249) {  zgsz3642 = true;  } else {  zgsz3642 = zghz3137.zW;  }
      zgaz3250 = zgsz3642;
    }
    if (zgaz3250) {  zghz3138 = zghz3137;  } else {
      struct zArchPerms zghz3163;
      zghz3163 = zghz3137;
      zghz3163.zC = false;
      zghz3138 = zghz3163;
    }
  }
  struct zArchPerms zghz3139;
  {
    bool zgaz3248;
    zgaz3248 = zghz3138.zR;
    if (zgaz3248) {  zghz3139 = zghz3138;  } else {
      struct zArchPerms zghz3162;
      zghz3162 = zghz3138;
      zghz3162.zC = false;
      zghz3139 = zghz3162;
    }
  }
  struct zArchPerms zghz3140;
  {
    bool zgaz3247;
    zgaz3247 = zghz3139.zR;
    if (zgaz3247) {  zghz3140 = zghz3139;  } else {
      struct zArchPerms zghz3161;
      zghz3161 = zghz3139;
      zghz3161.zX = false;
      zghz3140 = zghz3161;
    }
  }
  struct zArchPerms zghz3141;
  {
    bool zgaz3246;
    {
      bool zgaz3245;
      {
        bool zgaz3244;
        zgaz3244 = zghz3140.zC;
        zgaz3245 = sailgen_not(zgaz3244);
      }
      bool zgsz3646;
      if (zgaz3245) {  zgsz3646 = true;  } else {  zgsz3646 = zghz3140.zLM;  }
      zgaz3246 = zgsz3646;
    }
    if (zgaz3246) {  zghz3141 = zghz3140;  } else {
      struct zArchPerms zghz3160;
      zghz3160 = zghz3140;
      zghz3160.zW = false;
      zghz3141 = zghz3160;
    }
  }
  struct zArchPerms zghz3142;
  {
    bool zgaz3243;
    {
      bool zgaz3242;
      zgaz3242 = zghz3141.zW;
      bool zgsz3648;
      if (zgaz3242) {  zgsz3648 = true;  } else {  zgsz3648 = zghz3141.zC;  }
      zgaz3243 = zgsz3648;
    }
    if (zgaz3243) {  zghz3142 = zghz3141;  } else {
      struct zArchPerms zghz3159;
      zghz3159 = zghz3141;
      zghz3159.zX = false;
      zghz3142 = zghz3159;
    }
  }
  struct zArchPerms zghz3143;
  {
    bool zgaz3241;
    {
      bool zgaz3240;
      zgaz3240 = zghz3142.zC;
      bool zgsz3650;
      if (zgaz3240) {  zgsz3650 = zghz3142.zR;  } else {  zgsz3650 = false;  }
      zgaz3241 = zgsz3650;
    }
    if (zgaz3241) {  zghz3143 = zghz3142;  } else {
      struct zArchPerms zghz3158;
      zghz3158 = zghz3142;
      zghz3158.zEL = false;
      zghz3143 = zghz3158;
    }
  }
  struct zArchPerms zghz3144;
  {
    bool zgaz3239;
    zgaz3239 = zghz3143.zLM;
    if (zgaz3239) {  zghz3144 = zghz3143;  } else {
      struct zArchPerms zghz3157;
      zghz3157 = zghz3143;
      zghz3157.zEL = false;
      zghz3144 = zghz3157;
    }
  }
  struct zArchPerms zghz3145;
  {
    bool zgaz3238;
    {
      bool zgaz3237;
      zgaz3237 = zghz3144.zC;
      bool zgsz3653;
      if (zgaz3237) {  zgsz3653 = zghz3144.zR;  } else {  zgsz3653 = false;  }
      zgaz3238 = zgsz3653;
    }
    if (zgaz3238) {  zghz3145 = zghz3144;  } else {
      struct zArchPerms zghz3156;
      zghz3156 = zghz3144;
      zghz3156.zLM = false;
      zghz3145 = zghz3156;
    }
  }
  struct zArchPerms zghz3146;
  {
    bool zgaz3236;
    {
      bool zgaz3235;
      zgaz3235 = zghz3145.zW;
      bool zgsz3655;
      if (zgaz3235) {  zgsz3655 = true;  } else {  zgsz3655 = zghz3145.zEL;  }
      zgaz3236 = zgsz3655;
    }
    if (zgaz3236) {  zghz3146 = zghz3145;  } else {
      struct zArchPerms zghz3155;
      zghz3155 = zghz3145;
      zghz3155.zLM = false;
      zghz3146 = zghz3155;
    }
  }
  struct zArchPerms zghz3147;
  {
    bool zgaz3234;
    zgaz3234 = zghz3146.zC;
    if (zgaz3234) {  zghz3147 = zghz3146;  } else {
      struct zArchPerms zghz3154;
      zghz3154 = zghz3146;
      zghz3154.zSL = false;
      zghz3147 = zghz3154;
    }
  }
  struct zArchPerms zghz3148;
  {
    bool zgaz3233;
    {
      bool zgaz3232;
      zgaz3232 = zghz3147.zLM;
      bool zgsz3659;
      if (zgaz3232) {
        bool zgaz3231;
        zgaz3231 = zghz3147.zX;
        bool zgsz3658;
        if (zgaz3231) {  zgsz3658 = true;  } else {  zgsz3658 = zghz3147.zW;  }
        zgsz3659 = zgsz3658;
      } else {  zgsz3659 = false;  }
      zgaz3233 = zgsz3659;
    }
    if (zgaz3233) {  zghz3148 = zghz3147;  } else {
      struct zArchPerms zghz3153;
      zghz3153 = zghz3147;
      zghz3153.zSL = false;
      zghz3148 = zghz3153;
    }
  }
  struct zArchPerms zghz3149;
  {
    bool zgaz3230;
    {
      bool zgaz3229;
      {
        bool zgaz3222;
        zgaz3222 = zghz3148.zC;
        bool zgaz3223;
        zgaz3223 = zghz3148.zLM;
        zgaz3229 = eq_bool(zgaz3222, zgaz3223);
      }
      bool zgsz3662;
      if (zgaz3229) {
        bool zgaz3228;
        {
          bool zgaz3224;
          zgaz3224 = zghz3148.zC;
          bool zgaz3225;
          zgaz3225 = zghz3148.zEL;
          zgaz3228 = eq_bool(zgaz3224, zgaz3225);
        }
        bool zgsz3661;
        if (zgaz3228) {
          bool zgaz3226;
          zgaz3226 = zghz3148.zC;
          bool zgaz3227;
          zgaz3227 = zghz3148.zSL;
          zgsz3661 = eq_bool(zgaz3226, zgaz3227);
        } else {  zgsz3661 = false;  }
        zgsz3662 = zgsz3661;
      } else {  zgsz3662 = false;  }
      zgaz3230 = zgsz3662;
    }
    if (zgaz3230) {  zghz3149 = zghz3148;  } else {
      struct zArchPerms zghz3152;
      zghz3152 = zghz3148;
      zghz3152.zX = false;
      zghz3149 = zghz3152;
    }
  }
  struct zArchPerms zghz3150;
  {
    bool zgaz3221;
    zgaz3221 = zghz3149.zX;
    if (zgaz3221) {  zghz3150 = zghz3149;  } else {
      struct zArchPerms zghz3151;
      zghz3151 = zghz3149;
      zghz3151.zASR = false;
      zghz3150 = zghz3151;
    }
  }
  enum zExecutionMode zmodeshadowz314;
  {
    bool zgaz3220;
    zgaz3220 = zghz3150.zX;
    if (zgaz3220) {  zmodeshadowz314 = zmode;  } else {
      zmodeshadowz314 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    }
  }
  zcbz352.ztup0 = zghz3150;
  zcbz352.ztup1 = zmodeshadowz314;
end_function_491: ;
  return zcbz352;
end_block_exception_492: ;
  struct zArchPerms zcbz3104 = { .zASR = false, .zC = false, .zEL = false, .zLM = false, .zR = false, .zSL = false, .zW = false, .zX = false };
  struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zcbz3103 = { .ztup0 = zcbz3104, .ztup1 = zCapPtrMode };
  return zcbz3103;
}



static void finish_sailgen_legalizzePermsAndMode(void)
{
}

static uint64_t sailgen_getMetadataL8(struct zEncMetadata);

static uint64_t sailgen_getMetadataL8(struct zEncMetadata zmeta)
{
  __label__ end_function_494, end_block_exception_495;

  uint64_t zcbz353;
  zcbz353 = sailgen__get_EncMetadata_L8(zmeta);
end_function_494: ;
  return zcbz353;
end_block_exception_495: ;

  return UINT64_C(0xdeadc0de);
}

static struct zEncMetadata sailgen_setMetadataL8(struct zEncMetadata, uint64_t);

struct zEncMetadata zghz3165;

static void startup_sailgen_setMetadataL8(void)
{
}

static struct zEncMetadata sailgen_setMetadataL8(struct zEncMetadata zmeta, uint64_t zL8)
{
  __label__ end_function_497, end_block_exception_498;

  struct zEncMetadata zghz3165;
  zghz3165 = sailgen__update_EncMetadata_L8(zmeta, zL8);
end_function_497: ;
  return zghz3165;
end_block_exception_498: ;
  struct zEncMetadata zcbz3105 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz3105;
}



static void finish_sailgen_setMetadataL8(void)
{
}

static uint64_t sailgen_getMetadataInternalExponent(struct zEncMetadata);

static uint64_t sailgen_getMetadataInternalExponent(struct zEncMetadata zmeta)
{
  __label__ end_function_500, end_block_exception_501;

  uint64_t zcbz355;
  uint64_t zgaz3261;
  zgaz3261 = sailgen__get_EncMetadata_L8(zmeta);
  uint64_t zgaz3262;
  {
    uint64_t zgaz3259;
    zgaz3259 = sailgen__get_EncMetadata_TE(zmeta);
    uint64_t zgaz3260;
    zgaz3260 = sailgen__get_EncMetadata_BE(zmeta);
    zgaz3262 = (zgaz3259 << 2) | zgaz3260;
  }
  zcbz355 = (zgaz3261 << 4) | zgaz3262;
end_function_500: ;
  return zcbz355;
end_block_exception_501: ;

  return UINT64_C(0xdeadc0de);
}

static int64_t zcap_sizze;
static void create_letbind_16(void) {


  int64_t zgsz3666;
  zgsz3666 = INT64_C(8);
  zcap_sizze = zgsz3666;
let_end_502: ;
}
static void kill_letbind_16(void) {
}

static int64_t zcap_addr_width;
static void create_letbind_17(void) {


  int64_t zgsz3667;
  zgsz3667 = INT64_C(32);
  zcap_addr_width = zgsz3667;
let_end_503: ;
}
static void kill_letbind_17(void) {
}

static int64_t zcap_len_width;
static void create_letbind_18(void) {


  int64_t zgsz3668;
  zgsz3668 = INT64_C(33);
  zcap_len_width = zgsz3668;
let_end_504: ;
}
static void kill_letbind_18(void) {
}

static int64_t zcap_max_addr;
static void create_letbind_19(void) {


  int64_t zgsz3669;
  {
    sail_int zgsz3904;
    CREATE(sail_int)(&zgsz3904);
    CONVERT_OF(sail_int, mach_int)(&zgsz3904, zcap_addr_width);
    sail_int zgsz3905;
    CREATE(sail_int)(&zgsz3905);
    sailgen_MAX(&zgsz3905, zgsz3904);
    zgsz3669 = CONVERT_OF(mach_int, sail_int)(zgsz3905);
    KILL(sail_int)(&zgsz3905);
    KILL(sail_int)(&zgsz3904);
  }
  zcap_max_addr = zgsz3669;
let_end_505: ;
}
static void kill_letbind_19(void) {
}

static int64_t zcap_xe_width;
static void create_letbind_20(void) {


  int64_t zgsz3670;
  zgsz3670 = INT64_C(2);
  zcap_xe_width = zgsz3670;
let_end_506: ;
}
static void kill_letbind_20(void) {
}

static int64_t zcap_max_E;
static void create_letbind_21(void) {


  int64_t zgsz3671;
  zgsz3671 = INT64_C(24);
  zcap_max_E = zgsz3671;
let_end_507: ;
}
static void kill_letbind_21(void) {
}

static int64_t zcap_min_E;
static void create_letbind_22(void) {


  int64_t zgsz3672;
  zgsz3672 = INT64_C(-7);
  zcap_min_E = zgsz3672;
let_end_508: ;
}
static void kill_letbind_22(void) {
}

static int64_t zlvlbits;
static void create_letbind_23(void) {


  int64_t zgsz3673;
  zgsz3673 = INT64_C(1);
  zlvlbits = zgsz3673;
let_end_509: ;
}
static void kill_letbind_23(void) {
}

static uint64_t sailgen_internal_exponent_encdec_forwards(bool);

static bool sailgen_internal_exponent_encdec_backwards(uint64_t);

static uint64_t sailgen_internal_exponent_encdec_forwards(bool zargz3)
{
  __label__ case_513, case_512, finish_match_511, end_function_514, end_block_exception_515;

  uint64_t zcbz356;
  {
    bool zp0z3;
    zp0z3 = zargz3;
    bool zgsz3676;
    zgsz3676 = eq_bool(zp0z3, false);
    if (!(zgsz3676)) {  goto case_513;  }
    zcbz356 = UINT64_C(0b1);
    goto finish_match_511;
  }
case_513: ;
  {
    bool zuz3248;
    zuz3248 = zargz3;
    bool zgsz3675;
    zgsz3675 = eq_bool(zuz3248, true);
    if (!(zgsz3675)) {  goto case_512;  }
    zcbz356 = UINT64_C(0b0);
    goto finish_match_511;
  }
case_512: ;
finish_match_511: ;
end_function_514: ;
  return zcbz356;
end_block_exception_515: ;

  return UINT64_C(0xdeadc0de);
}

static uint64_t sailgen_internal_exponent_encdec_forwards_infallible(bool);

static uint64_t sailgen_internal_exponent_encdec_forwards_infallible(bool zargz3)
{
  __label__ case_518, case_517, finish_match_516, end_function_519, end_block_exception_520;

  uint64_t zcbz357;
  {
    bool zp0z3;
    zp0z3 = zargz3;
    bool zgsz3679;
    zgsz3679 = eq_bool(zp0z3, false);
    if (!(zgsz3679)) {  goto case_518;  }
    zcbz357 = UINT64_C(0b1);
    goto finish_match_516;
  }
case_518: ;
  {
    bool zuz3249;
    zuz3249 = zargz3;
    bool zgsz3678;
    zgsz3678 = eq_bool(zuz3249, true);
    if (!(zgsz3678)) {  goto case_517;  }
    zcbz357 = UINT64_C(0b0);
    goto finish_match_516;
  }
case_517: ;
finish_match_516: ;
end_function_519: ;
  return zcbz357;
end_block_exception_520: ;

  return UINT64_C(0xdeadc0de);
}

static bool sailgen_internal_exponent_encdec_backwards(uint64_t zargz3)
{
  __label__ case_524, case_523, finish_match_522, end_function_525, end_block_exception_526;

  bool zcbz358;
  {
    uint64_t zb__0;
    zb__0 = zargz3;
    bool zgsz3682;
    zgsz3682 = (zb__0 == UINT64_C(0b1));
    if (!(zgsz3682)) {  goto case_524;  }
    zcbz358 = false;
    goto finish_match_522;
  }
case_524: ;
  {
    zcbz358 = true;
    goto finish_match_522;
  }
case_523: ;
finish_match_522: ;
end_function_525: ;
  return zcbz358;
end_block_exception_526: ;

  return false;
}

static bool sailgen_internal_exponent_encdec_backwards_infallible(uint64_t);

static bool sailgen_internal_exponent_encdec_backwards_infallible(uint64_t zargz3)
{
  __label__ case_529, case_528, finish_match_527, end_function_530, end_block_exception_531;

  bool zcbz359;
  {
    uint64_t zb__0;
    zb__0 = zargz3;
    bool zgsz3685;
    zgsz3685 = (zb__0 == UINT64_C(0b1));
    if (!(zgsz3685)) {  goto case_529;  }
    zcbz359 = false;
    goto finish_match_527;
  }
case_529: ;
  {
    zcbz359 = true;
    goto finish_match_527;
  }
case_528: ;
finish_match_527: ;
end_function_530: ;
  return zcbz359;
end_block_exception_531: ;

  return false;
}

static struct zCapability sailgen_undefined_Capability(unit);

struct zCapability zghz3166;
sail_int zghz3167;

static void startup_sailgen_undefined_Capability(void)
{

  CREATE(sail_int)(&zghz3167);
}

static struct zCapability sailgen_undefined_Capability(unit zgsz3686)
{
  __label__ end_function_533, end_block_exception_534;

  struct zCapability zghz3166;
  zghz3166.zB = UINT64_C(0b0000000000);
  RECREATE(sail_int)(&zghz3167);
  CONVERT_OF(sail_int, mach_int)(&zghz3167, INT64_C(-7));
  zghz3166.zE = CONVERT_OF(mach_int, sail_int)(zghz3167);
  zghz3166.zT = UINT64_C(0b0000000000);
  zghz3166.zaddress = UINT64_C(0x00000000);
  zghz3166.zap_m = UINT64_C(0b00000);
  zghz3166.zcl = UINT64_C(0b0);
  zghz3166.zinternal_exponent = false;
  zghz3166.zreserved_0 = UINT64_C(0b00);
  zghz3166.zreserved_1 = UINT64_C(0b0);
  zghz3166.zsd_perms = UINT64_C(0b00);
  zghz3166.zsealed = false;
  zghz3166.ztag = false;
end_function_533: ;
  return zghz3166;
end_block_exception_534: ;
  struct zCapability zcbz3106 = { .zB = UINT64_C(0xdeadc0de), .zE = INT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zap_m = UINT64_C(0xdeadc0de), .zcl = UINT64_C(0xdeadc0de), .zinternal_exponent = false, .zreserved_0 = UINT64_C(0xdeadc0de), .zreserved_1 = UINT64_C(0xdeadc0de), .zsd_perms = UINT64_C(0xdeadc0de), .zsealed = false, .ztag = false };
  return zcbz3106;
}



static void finish_sailgen_undefined_Capability(void)
{    KILL(sail_int)(&zghz3167);
}

static struct zCapability sailgen_bitsToCap(bool, uint64_t);

struct zCapability zghz3168;
struct zEncMetadata zghz3169;
struct zCapability zghz3170;
sail_int zghz3171;
sail_int zghz3172;
sail_int zghz3173;
lbits zghz3174;
lbits zghz3175;
lbits zghz3176;
lbits zghz3177;
lbits zghz3178;
lbits zghz3179;
lbits zghz3180;
sail_int zghz3181;
sail_int zghz3182;
lbits zghz3183;
sail_int zghz3184;
sail_int zghz3185;
sail_int zghz3186;
sail_int zghz3187;
sail_int zghz3188;
sail_int zghz3189;
lbits zghz3190;
lbits zghz3191;
lbits zghz3192;
sail_int zghz3193;
sail_int zghz3194;
lbits zghz3195;
sail_int zghz3196;
sail_int zghz3197;
sail_int zghz3198;
sail_int zghz3199;
sail_int zghz3200;
sail_int zghz3201;
lbits zghz3202;
sail_int zghz3203;
sail_int zghz3204;
sail_int zghz3205;
sail_int zghz3206;
sail_int zghz3207;
sail_int zghz3208;
sail_int zghz3209;

static void startup_sailgen_bitsToCap(void)
{



  CREATE(sail_int)(&zghz3171);
  CREATE(sail_int)(&zghz3172);
  CREATE(sail_int)(&zghz3173);
  CREATE(lbits)(&zghz3174);
  CREATE(lbits)(&zghz3175);
  CREATE(lbits)(&zghz3176);
  CREATE(lbits)(&zghz3177);
  CREATE(lbits)(&zghz3178);
  CREATE(lbits)(&zghz3179);
  CREATE(lbits)(&zghz3180);
  CREATE(sail_int)(&zghz3181);
  CREATE(sail_int)(&zghz3182);
  CREATE(lbits)(&zghz3183);
  CREATE(sail_int)(&zghz3184);
  CREATE(sail_int)(&zghz3185);
  CREATE(sail_int)(&zghz3186);
  CREATE(sail_int)(&zghz3187);
  CREATE(sail_int)(&zghz3188);
  CREATE(sail_int)(&zghz3189);
  CREATE(lbits)(&zghz3190);
  CREATE(lbits)(&zghz3191);
  CREATE(lbits)(&zghz3192);
  CREATE(sail_int)(&zghz3193);
  CREATE(sail_int)(&zghz3194);
  CREATE(lbits)(&zghz3195);
  CREATE(sail_int)(&zghz3196);
  CREATE(sail_int)(&zghz3197);
  CREATE(sail_int)(&zghz3198);
  CREATE(sail_int)(&zghz3199);
  CREATE(sail_int)(&zghz3200);
  CREATE(sail_int)(&zghz3201);
  CREATE(lbits)(&zghz3202);
  CREATE(sail_int)(&zghz3203);
  CREATE(sail_int)(&zghz3204);
  CREATE(sail_int)(&zghz3205);
  CREATE(sail_int)(&zghz3206);
  CREATE(sail_int)(&zghz3207);
  CREATE(sail_int)(&zghz3208);
  CREATE(sail_int)(&zghz3209);
}

static struct zCapability sailgen_bitsToCap(bool ztag, uint64_t zcap_bits)
{
  __label__ case_537, finish_match_536, end_function_538, end_block_exception_539;

  struct zCapability zghz3168;
  struct zEncMetadata zghz3169;
  {
    uint64_t zgaz3290;
    {
      int64_t zgaz3289;
      {
        int64_t zgaz3288;
        {
          RECREATE(sail_int)(&zghz3207);
          CONVERT_OF(sail_int, mach_int)(&zghz3207, zxlen);
          RECREATE(sail_int)(&zghz3208);
          CONVERT_OF(sail_int, mach_int)(&zghz3208, INT64_C(2));
          RECREATE(sail_int)(&zghz3209);
          mult_int(&zghz3209, zghz3207, zghz3208);
          zgaz3288 = CONVERT_OF(mach_int, sail_int)(zghz3209);
        }
        {
          RECREATE(sail_int)(&zghz3204);
          CONVERT_OF(sail_int, mach_int)(&zghz3204, zgaz3288);
          RECREATE(sail_int)(&zghz3205);
          CONVERT_OF(sail_int, mach_int)(&zghz3205, INT64_C(1));
          RECREATE(sail_int)(&zghz3206);
          sub_int(&zghz3206, zghz3204, zghz3205);
          zgaz3289 = CONVERT_OF(mach_int, sail_int)(zghz3206);
        }
      }
      zgaz3290 = (safe_rshift(UINT64_MAX, 64 - 32) & (zcap_bits >> zxlen));
    }
    zghz3169 = sailgen_Mk_EncMetadata(zgaz3290);
  }
  bool zinternal_exponent;
  {
    uint64_t zgaz3287;
    zgaz3287 = sailgen__get_EncMetadata_EF(zghz3169);
    zinternal_exponent = sailgen_internal_exponent_encdec_backwards(zgaz3287);
  }
  {
    int64_t zMW;
    zMW = zcap_mantissa_width;
    int64_t zE;
    if (zinternal_exponent) {
      int64_t zgaz3286;
      {
        uint64_t zgaz3285;
        zgaz3285 = sailgen_getMetadataInternalExponent(zghz3169);
        {
          RECREATE(lbits)(&zghz3202);
          CONVERT_OF(lbits, fbits)(&zghz3202, zgaz3285, UINT64_C(5) , true);
          RECREATE(sail_int)(&zghz3203);
          sail_unsigned(&zghz3203, zghz3202);
          zgaz3286 = CONVERT_OF(mach_int, sail_int)(zghz3203);
        }
      }
      {
        RECREATE(sail_int)(&zghz3199);
        CONVERT_OF(sail_int, mach_int)(&zghz3199, zcap_max_E);
        RECREATE(sail_int)(&zghz3200);
        CONVERT_OF(sail_int, mach_int)(&zghz3200, zgaz3286);
        RECREATE(sail_int)(&zghz3201);
        sub_int(&zghz3201, zghz3199, zghz3200);
        zE = CONVERT_OF(mach_int, sail_int)(zghz3201);
      }
    } else {  zE = INT64_C(0);  }
    uint64_t zLMSB;
    if (zinternal_exponent) {  zLMSB = UINT64_C(0b01);  } else {
      uint64_t zgaz3284;
      zgaz3284 = sailgen_getMetadataL8(zghz3169);
      zLMSB = (UINT64_C(0b0) << 1) | zgaz3284;
    }
    uint64_t zT_low;
    {
      bool zgaz3283;
      zgaz3283 = sailgen_not(zinternal_exponent);
      if (zgaz3283) {  zT_low = sailgen__get_EncMetadata_TE(zghz3169);  } else {  zT_low = UINT64_C(0b00);  }
    }
    uint64_t zB_low;
    {
      bool zgaz3282;
      zgaz3282 = sailgen_not(zinternal_exponent);
      if (zgaz3282) {  zB_low = sailgen__get_EncMetadata_BE(zghz3169);  } else {  zB_low = UINT64_C(0b00);  }
    }
    sbits zT;
    {
      uint64_t zgaz3281;
      zgaz3281 = sailgen__get_EncMetadata_Tm(zghz3169);
      zT = CONVERT_OF(sbits, fbits)((zgaz3281 << 2) | zT_low, UINT64_C(8) , true);
    }
    sbits zB;
    {
      uint64_t zgaz3280;
      zgaz3280 = sailgen__get_EncMetadata_Bm(zghz3169);
      zB = CONVERT_OF(sbits, fbits)((zgaz3280 << 2) | zB_low, UINT64_C(10) , true);
    }
    uint64_t zLCout;
    {
      bool zgaz3279;
      {
        sbits zgaz3278;
        {
          int64_t zgaz3277;
          {
            RECREATE(sail_int)(&zghz3196);
            CONVERT_OF(sail_int, mach_int)(&zghz3196, zMW);
            RECREATE(sail_int)(&zghz3197);
            CONVERT_OF(sail_int, mach_int)(&zghz3197, INT64_C(3));
            RECREATE(sail_int)(&zghz3198);
            sub_int(&zghz3198, zghz3196, zghz3197);
            zgaz3277 = CONVERT_OF(mach_int, sail_int)(zghz3198);
          }
          {
            RECREATE(lbits)(&zghz3192);
            CONVERT_OF(lbits, sbits)(&zghz3192, zB, true);
            RECREATE(sail_int)(&zghz3193);
            CONVERT_OF(sail_int, mach_int)(&zghz3193, zgaz3277);
            RECREATE(sail_int)(&zghz3194);
            CONVERT_OF(sail_int, mach_int)(&zghz3194, INT64_C(0));
            RECREATE(lbits)(&zghz3195);
            vector_subrange_lbits(&zghz3195, zghz3192, zghz3193, zghz3194);
            zgaz3278 = CONVERT_OF(sbits, lbits)(zghz3195, true);
          }
        }
        {
          RECREATE(lbits)(&zghz3190);
          CONVERT_OF(lbits, sbits)(&zghz3190, zT, true);
          RECREATE(lbits)(&zghz3191);
          CONVERT_OF(lbits, sbits)(&zghz3191, zgaz3278, true);
          zgaz3279 = sailgen_z8operatorz0zI_uz9(zghz3190, zghz3191);
        }
      }
      if (zgaz3279) {  zLCout = UINT64_C(0b01);  } else {  zLCout = UINT64_C(0b00);  }
    }
    sbits zTshadowz315;
    {
      sbits zgaz3276;
      {
        sbits zgaz3275;
        {
          sbits zgaz3274;
          {
            int64_t zgaz3272;
            {
              RECREATE(sail_int)(&zghz3187);
              CONVERT_OF(sail_int, mach_int)(&zghz3187, zMW);
              RECREATE(sail_int)(&zghz3188);
              CONVERT_OF(sail_int, mach_int)(&zghz3188, INT64_C(1));
              RECREATE(sail_int)(&zghz3189);
              sub_int(&zghz3189, zghz3187, zghz3188);
              zgaz3272 = CONVERT_OF(mach_int, sail_int)(zghz3189);
            }
            int64_t zgaz3273;
            {
              RECREATE(sail_int)(&zghz3184);
              CONVERT_OF(sail_int, mach_int)(&zghz3184, zMW);
              RECREATE(sail_int)(&zghz3185);
              CONVERT_OF(sail_int, mach_int)(&zghz3185, INT64_C(2));
              RECREATE(sail_int)(&zghz3186);
              sub_int(&zghz3186, zghz3184, zghz3185);
              zgaz3273 = CONVERT_OF(mach_int, sail_int)(zghz3186);
            }
            {
              RECREATE(lbits)(&zghz3180);
              CONVERT_OF(lbits, sbits)(&zghz3180, zB, true);
              RECREATE(sail_int)(&zghz3181);
              CONVERT_OF(sail_int, mach_int)(&zghz3181, zgaz3272);
              RECREATE(sail_int)(&zghz3182);
              CONVERT_OF(sail_int, mach_int)(&zghz3182, zgaz3273);
              RECREATE(lbits)(&zghz3183);
              vector_subrange_lbits(&zghz3183, zghz3180, zghz3181, zghz3182);
              zgaz3274 = CONVERT_OF(sbits, lbits)(zghz3183, true);
            }
          }
          {
            RECREATE(lbits)(&zghz3177);
            CONVERT_OF(lbits, sbits)(&zghz3177, zgaz3274, true);
            RECREATE(lbits)(&zghz3178);
            CONVERT_OF(lbits, fbits)(&zghz3178, zLCout, UINT64_C(2) , true);
            RECREATE(lbits)(&zghz3179);
            add_bits(&zghz3179, zghz3177, zghz3178);
            zgaz3275 = CONVERT_OF(sbits, lbits)(zghz3179, true);
          }
        }
        {
          RECREATE(lbits)(&zghz3174);
          CONVERT_OF(lbits, sbits)(&zghz3174, zgaz3275, true);
          RECREATE(lbits)(&zghz3175);
          CONVERT_OF(lbits, fbits)(&zghz3175, zLMSB, UINT64_C(2) , true);
          RECREATE(lbits)(&zghz3176);
          add_bits(&zghz3176, zghz3174, zghz3175);
          zgaz3276 = CONVERT_OF(sbits, lbits)(zghz3176, true);
        }
      }
      zTshadowz315 = append_ss(zgaz3276, zT);
    }
    uint64_t zgaz3263;
    zgaz3263 = sailgen__get_EncMetadata_SDP(zghz3169);
    uint64_t zgaz3264;
    zgaz3264 = sailgen__get_EncMetadata_AP_M(zghz3169);
    uint64_t zgaz3265;
    zgaz3265 = sailgen__get_EncMetadata_CL(zghz3169);
    bool zgaz3267;
    {
      uint64_t zgaz3266;
      zgaz3266 = sailgen__get_EncMetadata_CT(zghz3169);
      zgaz3267 = sailgen_bool_bits_backwards(zgaz3266);
    }
    uint64_t zgaz3268;
    zgaz3268 = sailgen__get_EncMetadata_reserved_0(zghz3169);
    uint64_t zgaz3269;
    zgaz3269 = sailgen__get_EncMetadata_reserved_1(zghz3169);
    uint64_t zgaz3271;
    {
      int64_t zgaz3270;
      {
        RECREATE(sail_int)(&zghz3171);
        CONVERT_OF(sail_int, mach_int)(&zghz3171, zxlen);
        RECREATE(sail_int)(&zghz3172);
        CONVERT_OF(sail_int, mach_int)(&zghz3172, INT64_C(1));
        RECREATE(sail_int)(&zghz3173);
        sub_int(&zghz3173, zghz3171, zghz3172);
        zgaz3270 = CONVERT_OF(mach_int, sail_int)(zghz3173);
      }
      zgaz3271 = (safe_rshift(UINT64_MAX, 64 - 32) & (zcap_bits >> INT64_C(0)));
    }
    struct zCapability zghz3170;
    zghz3170.zB = CONVERT_OF(fbits, sbits)(zB, true);
    zghz3170.zE = zE;
    zghz3170.zT = CONVERT_OF(fbits, sbits)(zTshadowz315, true);
    zghz3170.zaddress = zgaz3271;
    zghz3170.zap_m = zgaz3264;
    zghz3170.zcl = zgaz3265;
    zghz3170.zinternal_exponent = zinternal_exponent;
    zghz3170.zreserved_0 = zgaz3268;
    zghz3170.zreserved_1 = zgaz3269;
    zghz3170.zsd_perms = zgaz3263;
    zghz3170.zsealed = zgaz3267;
    zghz3170.ztag = ztag;
    zghz3168 = zghz3170;
    goto finish_match_536;
  }
case_537: ;
  sail_match_failure("bitsToCap");
finish_match_536: ;
end_function_538: ;
  return zghz3168;
end_block_exception_539: ;
  struct zCapability zcbz3107 = { .zB = UINT64_C(0xdeadc0de), .zE = INT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zap_m = UINT64_C(0xdeadc0de), .zcl = UINT64_C(0xdeadc0de), .zinternal_exponent = false, .zreserved_0 = UINT64_C(0xdeadc0de), .zreserved_1 = UINT64_C(0xdeadc0de), .zsd_perms = UINT64_C(0xdeadc0de), .zsealed = false, .ztag = false };
  return zcbz3107;
}



static void finish_sailgen_bitsToCap(void)
{
  KILL(sail_int)(&zghz3209);
  KILL(sail_int)(&zghz3208);
  KILL(sail_int)(&zghz3207);
  KILL(sail_int)(&zghz3206);
  KILL(sail_int)(&zghz3205);
  KILL(sail_int)(&zghz3204);
  KILL(sail_int)(&zghz3203);
  KILL(lbits)(&zghz3202);
  KILL(sail_int)(&zghz3201);
  KILL(sail_int)(&zghz3200);
  KILL(sail_int)(&zghz3199);
  KILL(sail_int)(&zghz3198);
  KILL(sail_int)(&zghz3197);
  KILL(sail_int)(&zghz3196);
  KILL(lbits)(&zghz3195);
  KILL(sail_int)(&zghz3194);
  KILL(sail_int)(&zghz3193);
  KILL(lbits)(&zghz3192);
  KILL(lbits)(&zghz3191);
  KILL(lbits)(&zghz3190);
  KILL(sail_int)(&zghz3189);
  KILL(sail_int)(&zghz3188);
  KILL(sail_int)(&zghz3187);
  KILL(sail_int)(&zghz3186);
  KILL(sail_int)(&zghz3185);
  KILL(sail_int)(&zghz3184);
  KILL(lbits)(&zghz3183);
  KILL(sail_int)(&zghz3182);
  KILL(sail_int)(&zghz3181);
  KILL(lbits)(&zghz3180);
  KILL(lbits)(&zghz3179);
  KILL(lbits)(&zghz3178);
  KILL(lbits)(&zghz3177);
  KILL(lbits)(&zghz3176);
  KILL(lbits)(&zghz3175);
  KILL(lbits)(&zghz3174);
  KILL(sail_int)(&zghz3173);
  KILL(sail_int)(&zghz3172);
  KILL(sail_int)(&zghz3171);
}

static struct zEncMetadata sailgen_capToMetadataBits(struct zCapability);

struct zEncMetadata zghz3210;
struct zEncMetadata zghz3211;
struct zEncMetadata zghz3212;
struct zEncMetadata zghz3213;
struct zEncMetadata zghz3214;
struct zEncMetadata zghz3215;
struct zEncMetadata zghz3216;
sail_int zghz3217;
sail_int zghz3218;
sail_int zghz3219;
struct zEncMetadata zghz3220;
sail_int zghz3221;
sail_int zghz3222;
sail_int zghz3223;
struct zEncMetadata zghz3224;
struct zEncMetadata zghz3225;
struct zEncMetadata zghz3226;
struct zEncMetadata zghz3227;
struct zEncMetadata zghz3228;
sail_int zghz3229;
sail_int zghz3230;
sail_int zghz3231;
sail_int zghz3232;
sail_int zghz3233;
sail_int zghz3234;
sail_int zghz3235;
sail_int zghz3236;
sail_int zghz3237;
sail_int zghz3238;
sail_int zghz3239;
sail_int zghz3240;
sail_int zghz3241;
sail_int zghz3242;
sail_int zghz3243;
sail_int zghz3244;
sail_int zghz3245;
sail_int zghz3246;
sail_int zghz3247;
sail_int zghz3248;
lbits zghz3249;
sail_int zghz3250;
sail_int zghz3251;
sail_int zghz3252;

static void startup_sailgen_capToMetadataBits(void)
{







  CREATE(sail_int)(&zghz3217);
  CREATE(sail_int)(&zghz3218);
  CREATE(sail_int)(&zghz3219);

  CREATE(sail_int)(&zghz3221);
  CREATE(sail_int)(&zghz3222);
  CREATE(sail_int)(&zghz3223);





  CREATE(sail_int)(&zghz3229);
  CREATE(sail_int)(&zghz3230);
  CREATE(sail_int)(&zghz3231);
  CREATE(sail_int)(&zghz3232);
  CREATE(sail_int)(&zghz3233);
  CREATE(sail_int)(&zghz3234);
  CREATE(sail_int)(&zghz3235);
  CREATE(sail_int)(&zghz3236);
  CREATE(sail_int)(&zghz3237);
  CREATE(sail_int)(&zghz3238);
  CREATE(sail_int)(&zghz3239);
  CREATE(sail_int)(&zghz3240);
  CREATE(sail_int)(&zghz3241);
  CREATE(sail_int)(&zghz3242);
  CREATE(sail_int)(&zghz3243);
  CREATE(sail_int)(&zghz3244);
  CREATE(sail_int)(&zghz3245);
  CREATE(sail_int)(&zghz3246);
  CREATE(sail_int)(&zghz3247);
  CREATE(sail_int)(&zghz3248);
  CREATE(lbits)(&zghz3249);
  CREATE(sail_int)(&zghz3250);
  CREATE(sail_int)(&zghz3251);
  CREATE(sail_int)(&zghz3252);
}

// struct tuple_(%bit, %bv2, %bv2)
struct ztuple_z8z5bitzCz0z5bv2zCz0z5bv2z9 {
  fbits ztup0;
  uint64_t ztup1;
  uint64_t ztup2;
};

static void COPY(ztuple_z8z5bitzCz0z5bv2zCz0z5bv2z9)(struct ztuple_z8z5bitzCz0z5bv2zCz0z5bv2z9 *rop, const struct ztuple_z8z5bitzCz0z5bv2zCz0z5bv2z9 op) {
  rop->ztup0 = op.ztup0;
  rop->ztup1 = op.ztup1;
  rop->ztup2 = op.ztup2;
}

static bool EQUAL(ztuple_z8z5bitzCz0z5bv2zCz0z5bv2z9)(struct ztuple_z8z5bitzCz0z5bv2zCz0z5bv2z9 op1, struct ztuple_z8z5bitzCz0z5bv2zCz0z5bv2z9 op2) {
  return EQUAL(fbits)(op1.ztup0, op2.ztup0) && EQUAL(fbits)(op1.ztup1, op2.ztup1) && EQUAL(fbits)(op1.ztup2, op2.ztup2);
}

static struct zEncMetadata sailgen_capToMetadataBits(struct zCapability zcap)
{
  __label__ case_542, finish_match_541, end_function_543, end_block_exception_544;

  struct zEncMetadata zghz3210;
  uint64_t zE;
  {
    int64_t zgaz3336;
    {
      int64_t zgaz3335;
      zgaz3335 = zcap.zE;
      {
        RECREATE(sail_int)(&zghz3250);
        CONVERT_OF(sail_int, mach_int)(&zghz3250, zcap_max_E);
        RECREATE(sail_int)(&zghz3251);
        CONVERT_OF(sail_int, mach_int)(&zghz3251, zgaz3335);
        RECREATE(sail_int)(&zghz3252);
        sub_int(&zghz3252, zghz3250, zghz3251);
        zgaz3336 = CONVERT_OF(mach_int, sail_int)(zghz3252);
      }
    }
    {
      RECREATE(sail_int)(&zghz3247);
      CONVERT_OF(sail_int, mach_int)(&zghz3247, zcap_E_width);
      RECREATE(sail_int)(&zghz3248);
      CONVERT_OF(sail_int, mach_int)(&zghz3248, zgaz3336);
      RECREATE(lbits)(&zghz3249);
      sailgen_to_bits(&zghz3249, zghz3247, zghz3248);
      zE = CONVERT_OF(fbits, lbits)(zghz3249, true);
    }
  }
  struct ztuple_z8z5bitzCz0z5bv2zCz0z5bv2z9 zgaz3309;
  {
    bool zgaz3291;
    zgaz3291 = zcap.zinternal_exponent;
    if (zgaz3291) {
      fbits zgaz3296;
      {
        int64_t zgaz3292;
        {
          RECREATE(sail_int)(&zghz3244);
          CONVERT_OF(sail_int, mach_int)(&zghz3244, zcap_E_width);
          RECREATE(sail_int)(&zghz3245);
          CONVERT_OF(sail_int, mach_int)(&zghz3245, INT64_C(1));
          RECREATE(sail_int)(&zghz3246);
          sub_int(&zghz3246, zghz3244, zghz3245);
          zgaz3292 = CONVERT_OF(mach_int, sail_int)(zghz3246);
        }
        zgaz3296 = (UINT64_C(1) & (zE >> zgaz3292));
      }
      uint64_t zgaz3297;
      {
        int64_t zgaz3294;
        {
          int64_t zgaz3293;
          {
            RECREATE(sail_int)(&zghz3241);
            CONVERT_OF(sail_int, mach_int)(&zghz3241, zcap_xe_width);
            RECREATE(sail_int)(&zghz3242);
            CONVERT_OF(sail_int, mach_int)(&zghz3242, INT64_C(2));
            RECREATE(sail_int)(&zghz3243);
            mult_int(&zghz3243, zghz3241, zghz3242);
            zgaz3293 = CONVERT_OF(mach_int, sail_int)(zghz3243);
          }
          {
            RECREATE(sail_int)(&zghz3238);
            CONVERT_OF(sail_int, mach_int)(&zghz3238, zgaz3293);
            RECREATE(sail_int)(&zghz3239);
            CONVERT_OF(sail_int, mach_int)(&zghz3239, INT64_C(1));
            RECREATE(sail_int)(&zghz3240);
            sub_int(&zghz3240, zghz3238, zghz3239);
            zgaz3294 = CONVERT_OF(mach_int, sail_int)(zghz3240);
          }
        }
        zgaz3297 = (safe_rshift(UINT64_MAX, 64 - 2) & (zE >> zcap_xe_width));
      }
      uint64_t zgaz3298;
      {
        int64_t zgaz3295;
        {
          RECREATE(sail_int)(&zghz3235);
          CONVERT_OF(sail_int, mach_int)(&zghz3235, zcap_xe_width);
          RECREATE(sail_int)(&zghz3236);
          CONVERT_OF(sail_int, mach_int)(&zghz3236, INT64_C(1));
          RECREATE(sail_int)(&zghz3237);
          sub_int(&zghz3237, zghz3235, zghz3236);
          zgaz3295 = CONVERT_OF(mach_int, sail_int)(zghz3237);
        }
        zgaz3298 = (safe_rshift(UINT64_MAX, 64 - 2) & (zE >> INT64_C(0)));
      }
      struct ztuple_z8z5bitzCz0z5bv2zCz0z5bv2z9 zgsz3693;
      zgsz3693.ztup0 = zgaz3296;
      zgsz3693.ztup1 = zgaz3297;
      zgsz3693.ztup2 = zgaz3298;
      zgaz3309 = zgsz3693;
    } else {
      fbits zgaz3306;
      {
        uint64_t zgaz3301;
        {
          uint64_t zgaz3299;
          zgaz3299 = zcap.zT;
          uint64_t zgaz3300;
          zgaz3300 = zcap.zB;
          zgaz3301 = ((zgaz3299 - zgaz3300) & UINT64_C(0b1111111111));
        }
        zgaz3306 = (UINT64_C(1) & (zgaz3301 >> INT64_C(8)));
      }
      uint64_t zgaz3307;
      {
        uint64_t zgaz3302;
        zgaz3302 = zcap.zT;
        int64_t zgaz3303;
        {
          RECREATE(sail_int)(&zghz3232);
          CONVERT_OF(sail_int, mach_int)(&zghz3232, zcap_xe_width);
          RECREATE(sail_int)(&zghz3233);
          CONVERT_OF(sail_int, mach_int)(&zghz3233, INT64_C(1));
          RECREATE(sail_int)(&zghz3234);
          sub_int(&zghz3234, zghz3232, zghz3233);
          zgaz3303 = CONVERT_OF(mach_int, sail_int)(zghz3234);
        }
        zgaz3307 = (safe_rshift(UINT64_MAX, 64 - 2) & (zgaz3302 >> INT64_C(0)));
      }
      uint64_t zgaz3308;
      {
        uint64_t zgaz3304;
        zgaz3304 = zcap.zB;
        int64_t zgaz3305;
        {
          RECREATE(sail_int)(&zghz3229);
          CONVERT_OF(sail_int, mach_int)(&zghz3229, zcap_xe_width);
          RECREATE(sail_int)(&zghz3230);
          CONVERT_OF(sail_int, mach_int)(&zghz3230, INT64_C(1));
          RECREATE(sail_int)(&zghz3231);
          sub_int(&zghz3231, zghz3229, zghz3230);
          zgaz3305 = CONVERT_OF(mach_int, sail_int)(zghz3231);
        }
        zgaz3308 = (safe_rshift(UINT64_MAX, 64 - 2) & (zgaz3304 >> INT64_C(0)));
      }
      struct ztuple_z8z5bitzCz0z5bv2zCz0z5bv2z9 zgsz3692;
      zgsz3692.ztup0 = zgaz3306;
      zgsz3692.ztup1 = zgaz3307;
      zgsz3692.ztup2 = zgaz3308;
      zgaz3309 = zgsz3692;
    }
  }
  {
    fbits zL8;
    zL8 = zgaz3309.ztup0;
    uint64_t zTE;
    zTE = zgaz3309.ztup1;
    uint64_t zBE;
    zBE = zgaz3309.ztup2;
    int64_t zMW;
    zMW = zcap_mantissa_width;
    struct zEncMetadata zghz3211;
    {
      struct zEncMetadata zghz3212;
      {
        struct zEncMetadata zghz3213;
        {
          struct zEncMetadata zghz3214;
          {
            struct zEncMetadata zghz3215;
            {
              struct zEncMetadata zghz3216;
              {
                struct zEncMetadata zghz3220;
                {
                  struct zEncMetadata zghz3224;
                  {
                    struct zEncMetadata zghz3225;
                    {
                      struct zEncMetadata zghz3226;
                      {
                        struct zEncMetadata zghz3227;
                        {
                          uint64_t zgaz3310;
                          zgaz3310 = zcap.zsd_perms;
                          struct zEncMetadata zghz3228;
                          zghz3228.zbits = UINT64_C(0x00000000);
                          zghz3227 = sailgen__update_EncMetadata_SDP(zghz3228, zgaz3310);
                        }
                        uint64_t zgaz3312;
                        zgaz3312 = zcap.zap_m;
                        zghz3226 = sailgen__update_EncMetadata_AP_M(zghz3227, zgaz3312);
                      }
                      uint64_t zgaz3314;
                      zgaz3314 = zcap.zcl;
                      zghz3225 = sailgen__update_EncMetadata_CL(zghz3226, zgaz3314);
                    }
                    uint64_t zgaz3317;
                    {
                      bool zgaz3315;
                      zgaz3315 = zcap.zsealed;
                      zgaz3317 = sailgen_bool_bits_forwards(zgaz3315);
                    }
                    zghz3224 = sailgen__update_EncMetadata_CT(zghz3225, zgaz3317);
                  }
                  uint64_t zgaz3320;
                  {
                    bool zgaz3318;
                    zgaz3318 = zcap.zinternal_exponent;
                    zgaz3320 = sailgen_internal_exponent_encdec_forwards(zgaz3318);
                  }
                  zghz3220 = sailgen__update_EncMetadata_EF(zghz3224, zgaz3320);
                }
                uint64_t zgaz3324;
                {
                  uint64_t zgaz3321;
                  zgaz3321 = zcap.zT;
                  int64_t zgaz3322;
                  {
                    RECREATE(sail_int)(&zghz3221);
                    CONVERT_OF(sail_int, mach_int)(&zghz3221, zMW);
                    RECREATE(sail_int)(&zghz3222);
                    CONVERT_OF(sail_int, mach_int)(&zghz3222, INT64_C(3));
                    RECREATE(sail_int)(&zghz3223);
                    sub_int(&zghz3223, zghz3221, zghz3222);
                    zgaz3322 = CONVERT_OF(mach_int, sail_int)(zghz3223);
                  }
                  zgaz3324 = (safe_rshift(UINT64_MAX, 64 - 6) & (zgaz3321 >> zcap_xe_width));
                }
                zghz3216 = sailgen__update_EncMetadata_Tm(zghz3220, zgaz3324);
              }
              uint64_t zgaz3328;
              {
                uint64_t zgaz3325;
                zgaz3325 = zcap.zB;
                int64_t zgaz3326;
                {
                  RECREATE(sail_int)(&zghz3217);
                  CONVERT_OF(sail_int, mach_int)(&zghz3217, zMW);
                  RECREATE(sail_int)(&zghz3218);
                  CONVERT_OF(sail_int, mach_int)(&zghz3218, INT64_C(1));
                  RECREATE(sail_int)(&zghz3219);
                  sub_int(&zghz3219, zghz3217, zghz3218);
                  zgaz3326 = CONVERT_OF(mach_int, sail_int)(zghz3219);
                }
                zgaz3328 = (safe_rshift(UINT64_MAX, 64 - 8) & (zgaz3325 >> zcap_xe_width));
              }
              zghz3215 = sailgen__update_EncMetadata_Bm(zghz3216, zgaz3328);
            }
            zghz3214 = sailgen__update_EncMetadata_TE(zghz3215, zTE);
          }
          zghz3213 = sailgen__update_EncMetadata_BE(zghz3214, zBE);
        }
        uint64_t zgaz3332;
        zgaz3332 = zcap.zreserved_1;
        zghz3212 = sailgen__update_EncMetadata_reserved_1(zghz3213, zgaz3332);
      }
      uint64_t zgaz3334;
      zgaz3334 = zcap.zreserved_0;
      zghz3211 = sailgen__update_EncMetadata_reserved_0(zghz3212, zgaz3334);
    }
    uint64_t zgsz3696;
    zgsz3696 = UINT64_C(0b0);
    zgsz3696 = update_fbits(zgsz3696, INT64_C(0), zL8);
    zghz3210 = sailgen_setMetadataL8(zghz3211, zgsz3696);
    goto finish_match_541;
  }
case_542: ;
  sail_match_failure("capToMetadataBits");
finish_match_541: ;
end_function_543: ;
  return zghz3210;
end_block_exception_544: ;
  struct zEncMetadata zcbz3108 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz3108;
}



static void finish_sailgen_capToMetadataBits(void)
{
  KILL(sail_int)(&zghz3252);
  KILL(sail_int)(&zghz3251);
  KILL(sail_int)(&zghz3250);
  KILL(lbits)(&zghz3249);
  KILL(sail_int)(&zghz3248);
  KILL(sail_int)(&zghz3247);
  KILL(sail_int)(&zghz3246);
  KILL(sail_int)(&zghz3245);
  KILL(sail_int)(&zghz3244);
  KILL(sail_int)(&zghz3243);
  KILL(sail_int)(&zghz3242);
  KILL(sail_int)(&zghz3241);
  KILL(sail_int)(&zghz3240);
  KILL(sail_int)(&zghz3239);
  KILL(sail_int)(&zghz3238);
  KILL(sail_int)(&zghz3237);
  KILL(sail_int)(&zghz3236);
  KILL(sail_int)(&zghz3235);
  KILL(sail_int)(&zghz3234);
  KILL(sail_int)(&zghz3233);
  KILL(sail_int)(&zghz3232);
  KILL(sail_int)(&zghz3231);
  KILL(sail_int)(&zghz3230);
  KILL(sail_int)(&zghz3229);
  KILL(sail_int)(&zghz3223);
  KILL(sail_int)(&zghz3222);
  KILL(sail_int)(&zghz3221);
  KILL(sail_int)(&zghz3219);
  KILL(sail_int)(&zghz3218);
  KILL(sail_int)(&zghz3217);
}

static struct zCapability znull_cap;
static void create_letbind_24(void) {


  struct zCapability zgsz3699;
  uint64_t zgaz3339;
  {
    uint64_t zgaz3338;
    {
      int64_t zgaz3337;
      {
        sail_int zgsz3977;
        CREATE(sail_int)(&zgsz3977);
        CONVERT_OF(sail_int, mach_int)(&zgsz3977, zcap_mantissa_width);
        sail_int zgsz3978;
        CREATE(sail_int)(&zgsz3978);
        CONVERT_OF(sail_int, mach_int)(&zgsz3978, INT64_C(2));
        sail_int zgsz3979;
        CREATE(sail_int)(&zgsz3979);
        sub_int(&zgsz3979, zgsz3977, zgsz3978);
        zgaz3337 = CONVERT_OF(mach_int, sail_int)(zgsz3979);
        KILL(sail_int)(&zgsz3979);
        KILL(sail_int)(&zgsz3978);
        KILL(sail_int)(&zgsz3977);
      }
      {
        sail_int zgsz3975;
        CREATE(sail_int)(&zgsz3975);
        CONVERT_OF(sail_int, mach_int)(&zgsz3975, zgaz3337);
        lbits zgsz3976;
        CREATE(lbits)(&zgsz3976);
        sailgen_zzeros_implicit(&zgsz3976, zgsz3975);
        zgaz3338 = CONVERT_OF(fbits, lbits)(zgsz3976, true);
        KILL(lbits)(&zgsz3976);
        KILL(sail_int)(&zgsz3975);
      }
    }
    zgaz3339 = (UINT64_C(0b01) << 8) | zgaz3338;
  }
  struct zCapability zgsz3698;
  zgsz3698.zB = UINT64_C(0b0000000000);
  zgsz3698.zE = zcap_max_E;
  zgsz3698.zT = zgaz3339;
  zgsz3698.zaddress = UINT64_C(0x00000000);
  zgsz3698.zap_m = znull_ap_m;
  zgsz3698.zcl = UINT64_C(0b0);
  zgsz3698.zinternal_exponent = true;
  zgsz3698.zreserved_0 = UINT64_C(0b00);
  zgsz3698.zreserved_1 = UINT64_C(0b0);
  zgsz3698.zsd_perms = UINT64_C(0b00);
  zgsz3698.zsealed = false;
  zgsz3698.ztag = false;
  zgsz3699 = zgsz3698;
  znull_cap = zgsz3699;
let_end_545: ;
}
static void kill_letbind_24(void) {
}

static struct zCapability zinfinite_cap;
static void create_letbind_25(void) {


  struct zCapability zgsz3701;
  struct zCapability zgsz3700;
  zgsz3700 = znull_cap;
  zgsz3700.zap_m = zinfinite_ap_m;
  zgsz3700.zcl = UINT64_C(0b1);
  zgsz3700.zsd_perms = UINT64_C(0b11);
  zgsz3700.ztag = true;
  zgsz3701 = zgsz3700;
  zinfinite_cap = zgsz3701;
let_end_546: ;
}
static void kill_letbind_25(void) {
}

static bool sailgen_boundsMalformed(struct zCapability);

sail_int zghz3253;
sail_int zghz3254;
sail_int zghz3255;
sail_int zghz3256;
sail_int zghz3257;
sail_int zghz3258;
sail_int zghz3259;
lbits zghz3260;

static void startup_sailgen_boundsMalformed(void)
{
  CREATE(sail_int)(&zghz3253);
  CREATE(sail_int)(&zghz3254);
  CREATE(sail_int)(&zghz3255);
  CREATE(sail_int)(&zghz3256);
  CREATE(sail_int)(&zghz3257);
  CREATE(sail_int)(&zghz3258);
  CREATE(sail_int)(&zghz3259);
  CREATE(lbits)(&zghz3260);
}

static bool sailgen_boundsMalformed(struct zCapability zcap)
{
  __label__ end_function_548, end_block_exception_549;

  bool zcbz363;
  bool zgaz3354;
  zgaz3354 = zcap.zinternal_exponent;
  bool zgsz3708;
  if (zgaz3354) {
    int64_t zMW;
    zMW = zcap_mantissa_width;
    bool zmalformedMSB;
    {
      bool zgaz3353;
      {
        bool zgaz3346;
        {
          int64_t zgaz3343;
          zgaz3343 = zcap.zE;
          zgaz3346 = (zgaz3343 == zcap_max_E);
        }
        bool zgsz3702;
        if (zgaz3346) {
          uint64_t zgaz3344;
          zgaz3344 = zcap.zB;
          uint64_t zgaz3345;
          {
            RECREATE(sail_int)(&zghz3259);
            CONVERT_OF(sail_int, mach_int)(&zghz3259, zMW);
            RECREATE(lbits)(&zghz3260);
            sailgen_zzeros_implicit(&zghz3260, zghz3259);
            zgaz3345 = CONVERT_OF(fbits, lbits)(zghz3260, true);
          }
          zgsz3702 = (zgaz3344 != zgaz3345);
        } else {  zgsz3702 = false;  }
        zgaz3353 = zgsz3702;
      }
      bool zgsz3704;
      if (zgaz3353) {  zgsz3704 = true;  } else {
        bool zgaz3352;
        {
          int64_t zgaz3347;
          zgaz3347 = zcap.zE;
          int64_t zgaz3348;
          {
            RECREATE(sail_int)(&zghz3256);
            CONVERT_OF(sail_int, mach_int)(&zghz3256, zcap_max_E);
            RECREATE(sail_int)(&zghz3257);
            CONVERT_OF(sail_int, mach_int)(&zghz3257, INT64_C(1));
            RECREATE(sail_int)(&zghz3258);
            sub_int(&zghz3258, zghz3256, zghz3257);
            zgaz3348 = CONVERT_OF(mach_int, sail_int)(zghz3258);
          }
          zgaz3352 = (zgaz3347 == zgaz3348);
        }
        bool zgsz3703;
        if (zgaz3352) {
          fbits zgaz3351;
          {
            uint64_t zgaz3349;
            zgaz3349 = zcap.zB;
            int64_t zgaz3350;
            {
              RECREATE(sail_int)(&zghz3253);
              CONVERT_OF(sail_int, mach_int)(&zghz3253, zMW);
              RECREATE(sail_int)(&zghz3254);
              CONVERT_OF(sail_int, mach_int)(&zghz3254, INT64_C(1));
              RECREATE(sail_int)(&zghz3255);
              sub_int(&zghz3255, zghz3253, zghz3254);
              zgaz3350 = CONVERT_OF(mach_int, sail_int)(zghz3255);
            }
            zgaz3351 = (UINT64_C(1) & (zgaz3349 >> zgaz3350));
          }
          zgsz3703 = sailgen_neq_anythingzItzK(zgaz3351, UINT64_C(0));
        } else {  zgsz3703 = false;  }
        zgsz3704 = zgsz3703;
      }
      zmalformedMSB = zgsz3704;
    }
    bool zmalformedLSB;
    {
      bool zgaz3342;
      {
        int64_t zgaz3340;
        zgaz3340 = zcap.zE;
        zgaz3342 = (zgaz3340 < INT64_C(0));
      }
      bool zgsz3706;
      if (zgaz3342) {  zgsz3706 = true;  } else {
        bool zgsz3705;
        if (zcap_use_L8) {
          int64_t zgaz3341;
          zgaz3341 = zcap.zE;
          zgsz3705 = (zgaz3341 == INT64_C(0));
        } else {  zgsz3705 = false;  }
        zgsz3706 = zgsz3705;
      }
      zmalformedLSB = zgsz3706;
    }
    bool zgsz3707;
    if (zmalformedMSB) {  zgsz3707 = true;  } else {  zgsz3707 = zmalformedLSB;  }
    zgsz3708 = zgsz3707;
  } else {  zgsz3708 = false;  }
  zcbz363 = zgsz3708;
end_function_548: ;
  return zcbz363;
end_block_exception_549: ;

  return false;
}



static void finish_sailgen_boundsMalformed(void)
{
  KILL(lbits)(&zghz3260);
  KILL(sail_int)(&zghz3259);
  KILL(sail_int)(&zghz3258);
  KILL(sail_int)(&zghz3257);
  KILL(sail_int)(&zghz3256);
  KILL(sail_int)(&zghz3255);
  KILL(sail_int)(&zghz3254);
  KILL(sail_int)(&zghz3253);
}

static void sailgen_getCapBoundsBits(struct zoptionzIz8bzCbz9zK *rop, struct zCapability);

sail_int zghz3261;
sail_int zghz3262;
sail_int zghz3263;
lbits zghz3264;
sail_int zghz3265;
lbits zghz3266;
lbits zghz3267;
lbits zghz3268;
sail_int zghz3269;
sail_int zghz3270;
sail_int zghz3271;
sail_int zghz3272;
sail_int zghz3273;
sail_int zghz3274;
sail_int zghz3275;
sail_int zghz3276;
sail_int zghz3277;
lbits zghz3278;
sail_int zghz3279;
lbits zghz3280;
lbits zghz3281;
lbits zghz3282;
sail_int zghz3283;
lbits zghz3284;
lbits zghz3285;
sail_int zghz3286;
lbits zghz3287;
lbits zghz3288;
sail_int zghz3289;
lbits zghz3290;
lbits zghz3291;
lbits zghz3292;
sail_int zghz3293;
lbits zghz3294;
lbits zghz3295;
sail_int zghz3296;
lbits zghz3297;
lbits zghz3298;
sail_int zghz3299;
lbits zghz3300;
sail_int zghz3301;
sail_int zghz3302;
sail_int zghz3303;
sail_int zghz3304;
sail_int zghz3305;
sail_int zghz3306;
lbits zghz3307;
lbits zghz3308;
lbits zghz3309;
lbits zghz3310;
lbits zghz3311;
lbits zghz3312;
sail_int zghz3313;
lbits zghz3314;
sail_int zghz3315;
sail_int zghz3316;
sail_int zghz3317;
lbits zghz3318;
sail_int zghz3319;
lbits zghz3320;
lbits zghz3321;
sail_int zghz3322;
lbits zghz3323;

static void startup_sailgen_getCapBoundsBits(void)
{
  CREATE(sail_int)(&zghz3261);
  CREATE(sail_int)(&zghz3262);
  CREATE(sail_int)(&zghz3263);
  CREATE(lbits)(&zghz3264);
  CREATE(sail_int)(&zghz3265);
  CREATE(lbits)(&zghz3266);
  CREATE(lbits)(&zghz3267);
  CREATE(lbits)(&zghz3268);
  CREATE(sail_int)(&zghz3269);
  CREATE(sail_int)(&zghz3270);
  CREATE(sail_int)(&zghz3271);
  CREATE(sail_int)(&zghz3272);
  CREATE(sail_int)(&zghz3273);
  CREATE(sail_int)(&zghz3274);
  CREATE(sail_int)(&zghz3275);
  CREATE(sail_int)(&zghz3276);
  CREATE(sail_int)(&zghz3277);
  CREATE(lbits)(&zghz3278);
  CREATE(sail_int)(&zghz3279);
  CREATE(lbits)(&zghz3280);
  CREATE(lbits)(&zghz3281);
  CREATE(lbits)(&zghz3282);
  CREATE(sail_int)(&zghz3283);
  CREATE(lbits)(&zghz3284);
  CREATE(lbits)(&zghz3285);
  CREATE(sail_int)(&zghz3286);
  CREATE(lbits)(&zghz3287);
  CREATE(lbits)(&zghz3288);
  CREATE(sail_int)(&zghz3289);
  CREATE(lbits)(&zghz3290);
  CREATE(lbits)(&zghz3291);
  CREATE(lbits)(&zghz3292);
  CREATE(sail_int)(&zghz3293);
  CREATE(lbits)(&zghz3294);
  CREATE(lbits)(&zghz3295);
  CREATE(sail_int)(&zghz3296);
  CREATE(lbits)(&zghz3297);
  CREATE(lbits)(&zghz3298);
  CREATE(sail_int)(&zghz3299);
  CREATE(lbits)(&zghz3300);
  CREATE(sail_int)(&zghz3301);
  CREATE(sail_int)(&zghz3302);
  CREATE(sail_int)(&zghz3303);
  CREATE(sail_int)(&zghz3304);
  CREATE(sail_int)(&zghz3305);
  CREATE(sail_int)(&zghz3306);
  CREATE(lbits)(&zghz3307);
  CREATE(lbits)(&zghz3308);
  CREATE(lbits)(&zghz3309);
  CREATE(lbits)(&zghz3310);
  CREATE(lbits)(&zghz3311);
  CREATE(lbits)(&zghz3312);
  CREATE(sail_int)(&zghz3313);
  CREATE(lbits)(&zghz3314);
  CREATE(sail_int)(&zghz3315);
  CREATE(sail_int)(&zghz3316);
  CREATE(sail_int)(&zghz3317);
  CREATE(lbits)(&zghz3318);
  CREATE(sail_int)(&zghz3319);
  CREATE(lbits)(&zghz3320);
  CREATE(lbits)(&zghz3321);
  CREATE(sail_int)(&zghz3322);
  CREATE(lbits)(&zghz3323);
}

// struct tuple_(%bv32, %bv33)
struct ztuple_z8z5bv32zCz0z5bv33z9 {
  uint64_t ztup0;
  uint64_t ztup1;
};

static void COPY(ztuple_z8z5bv32zCz0z5bv33z9)(struct ztuple_z8z5bv32zCz0z5bv33z9 *rop, const struct ztuple_z8z5bv32zCz0z5bv33z9 op) {
  rop->ztup0 = op.ztup0;
  rop->ztup1 = op.ztup1;
}

static bool EQUAL(ztuple_z8z5bv32zCz0z5bv33z9)(struct ztuple_z8z5bv32zCz0z5bv33z9 op1, struct ztuple_z8z5bv32zCz0z5bv33z9 op2) {
  return EQUAL(fbits)(op1.ztup0, op2.ztup0) && EQUAL(fbits)(op1.ztup1, op2.ztup1);
}

static void sailgen_getCapBoundsBits(struct zoptionzIz8bzCbz9zK *zcbz364, struct zCapability zc)
{
  __label__ end_function_551, end_block_exception_554, end_function_671;

  {
    bool zgaz3355;
    zgaz3355 = sailgen_boundsMalformed(zc);
    unit zgsz3709;
    if (zgaz3355) {
      __label__ cleanup_552, end_cleanup_553;

      struct zoptionzIz8bzCbz9zK zgaz3356;
      CREATE(zoptionzIz8bzCbz9zK)(&zgaz3356);
      sailgen_NonezIz8bzCbz9zK(&zgaz3356, UNIT);
      COPY(zoptionzIz8bzCbz9zK)((*(&zcbz364)), zgaz3356);
      goto cleanup_552;
      /* unreachable after return */
      KILL(zoptionzIz8bzCbz9zK)(&zgaz3356);
      goto end_cleanup_553;
    cleanup_552: ;
      KILL(zoptionzIz8bzCbz9zK)(&zgaz3356);
      goto end_function_551;
    end_cleanup_553: ;
    } else {  zgsz3709 = UNIT;  }
  }
  int64_t zE;
  zE = zc.zE;
  {
    bool zgaz3357;
    zgaz3357 = (zE >= INT64_C(0));
    unit zgsz3710;
    zgsz3710 = sail_assert(zgaz3357, "Cap is malformed if E<0");
  }
  uint64_t za;
  za = zc.zaddress;
  uint64_t za_mid;
  {
    uint64_t zgaz3389;
    {
      RECREATE(lbits)(&zghz3321);
      CONVERT_OF(lbits, fbits)(&zghz3321, za, UINT64_C(32) , true);
      RECREATE(sail_int)(&zghz3322);
      CONVERT_OF(sail_int, mach_int)(&zghz3322, zE);
      RECREATE(lbits)(&zghz3323);
      shiftr(&zghz3323, zghz3321, zghz3322);
      zgaz3389 = CONVERT_OF(fbits, lbits)(zghz3323, true);
    }
    {
      RECREATE(lbits)(&zghz3318);
      CONVERT_OF(lbits, fbits)(&zghz3318, zgaz3389, UINT64_C(32) , true);
      RECREATE(sail_int)(&zghz3319);
      CONVERT_OF(sail_int, mach_int)(&zghz3319, zcap_mantissa_width);
      RECREATE(lbits)(&zghz3320);
      sail_truncate(&zghz3320, zghz3318, zghz3319);
      za_mid = CONVERT_OF(fbits, lbits)(zghz3320, true);
    }
  }
  uint64_t zR;
  {
    uint64_t zgaz3387;
    zgaz3387 = zc.zB;
    uint64_t zgaz3388;
    {
      uint64_t zgaz3386;
      {
        int64_t zgaz3385;
        {
          RECREATE(sail_int)(&zghz3315);
          CONVERT_OF(sail_int, mach_int)(&zghz3315, zcap_mantissa_width);
          RECREATE(sail_int)(&zghz3316);
          CONVERT_OF(sail_int, mach_int)(&zghz3316, INT64_C(2));
          RECREATE(sail_int)(&zghz3317);
          sub_int(&zghz3317, zghz3315, zghz3316);
          zgaz3385 = CONVERT_OF(mach_int, sail_int)(zghz3317);
        }
        {
          RECREATE(sail_int)(&zghz3313);
          CONVERT_OF(sail_int, mach_int)(&zghz3313, zgaz3385);
          RECREATE(lbits)(&zghz3314);
          sailgen_zzeros_implicit(&zghz3314, zghz3313);
          zgaz3386 = CONVERT_OF(fbits, lbits)(zghz3314, true);
        }
      }
      zgaz3388 = (UINT64_C(0b01) << 8) | zgaz3386;
    }
    zR = ((zgaz3387 - zgaz3388) & UINT64_C(0b1111111111));
  }
  int64_t zaHi;
  {
    bool zgaz3384;
    {
      RECREATE(lbits)(&zghz3311);
      CONVERT_OF(lbits, fbits)(&zghz3311, za_mid, UINT64_C(10) , true);
      RECREATE(lbits)(&zghz3312);
      CONVERT_OF(lbits, fbits)(&zghz3312, zR, UINT64_C(10) , true);
      zgaz3384 = sailgen_z8operatorz0zI_uz9(zghz3311, zghz3312);
    }
    if (zgaz3384) {  zaHi = INT64_C(1);  } else {  zaHi = INT64_C(0);  }
  }
  int64_t zbHi;
  {
    bool zgaz3383;
    {
      uint64_t zgaz3382;
      zgaz3382 = zc.zB;
      {
        RECREATE(lbits)(&zghz3309);
        CONVERT_OF(lbits, fbits)(&zghz3309, zgaz3382, UINT64_C(10) , true);
        RECREATE(lbits)(&zghz3310);
        CONVERT_OF(lbits, fbits)(&zghz3310, zR, UINT64_C(10) , true);
        zgaz3383 = sailgen_z8operatorz0zI_uz9(zghz3309, zghz3310);
      }
    }
    if (zgaz3383) {  zbHi = INT64_C(1);  } else {  zbHi = INT64_C(0);  }
  }
  int64_t ztHi;
  {
    bool zgaz3381;
    {
      uint64_t zgaz3380;
      zgaz3380 = zc.zT;
      {
        RECREATE(lbits)(&zghz3307);
        CONVERT_OF(lbits, fbits)(&zghz3307, zgaz3380, UINT64_C(10) , true);
        RECREATE(lbits)(&zghz3308);
        CONVERT_OF(lbits, fbits)(&zghz3308, zR, UINT64_C(10) , true);
        zgaz3381 = sailgen_z8operatorz0zI_uz9(zghz3307, zghz3308);
      }
    }
    if (zgaz3381) {  ztHi = INT64_C(1);  } else {  ztHi = INT64_C(0);  }
  }
  int64_t zcorrection_base;
  {
    RECREATE(sail_int)(&zghz3304);
    CONVERT_OF(sail_int, mach_int)(&zghz3304, zbHi);
    RECREATE(sail_int)(&zghz3305);
    CONVERT_OF(sail_int, mach_int)(&zghz3305, zaHi);
    RECREATE(sail_int)(&zghz3306);
    sub_int(&zghz3306, zghz3304, zghz3305);
    zcorrection_base = CONVERT_OF(mach_int, sail_int)(zghz3306);
  }
  int64_t zcorrection_top;
  {
    RECREATE(sail_int)(&zghz3301);
    CONVERT_OF(sail_int, mach_int)(&zghz3301, ztHi);
    RECREATE(sail_int)(&zghz3302);
    CONVERT_OF(sail_int, mach_int)(&zghz3302, zaHi);
    RECREATE(sail_int)(&zghz3303);
    sub_int(&zghz3303, zghz3301, zghz3302);
    zcorrection_top = CONVERT_OF(mach_int, sail_int)(zghz3303);
  }
  uint64_t za_top;
  {
    int64_t zgaz3379;
    zgaz3379 = (zE + zcap_mantissa_width);
    {
      RECREATE(lbits)(&zghz3298);
      CONVERT_OF(lbits, fbits)(&zghz3298, za, UINT64_C(32) , true);
      RECREATE(sail_int)(&zghz3299);
      CONVERT_OF(sail_int, mach_int)(&zghz3299, zgaz3379);
      RECREATE(lbits)(&zghz3300);
      shiftr(&zghz3300, zghz3298, zghz3299);
      za_top = CONVERT_OF(fbits, lbits)(zghz3300, true);
    }
  }
  uint64_t zbase;
  {
    RECREATE(lbits)(&zghz3288);
    {
      uint64_t zgaz3376;
      {
        RECREATE(lbits)(&zghz3295);
        CONVERT_OF(lbits, fbits)(&zghz3295, za_top, UINT64_C(32) , true);
        RECREATE(sail_int)(&zghz3296);
        CONVERT_OF(sail_int, mach_int)(&zghz3296, zcorrection_base);
        RECREATE(lbits)(&zghz3297);
        add_bits_int(&zghz3297, zghz3295, zghz3296);
        zgaz3376 = CONVERT_OF(fbits, lbits)(zghz3297, true);
      }
      sbits zgaz3377;
      {
        uint64_t zgaz3374;
        zgaz3374 = zc.zB;
        sbits zgaz3375;
        {
          RECREATE(sail_int)(&zghz3293);
          CONVERT_OF(sail_int, mach_int)(&zghz3293, zE);
          RECREATE(lbits)(&zghz3294);
          sailgen_zzeros_implicit(&zghz3294, zghz3293);
          zgaz3375 = CONVERT_OF(sbits, lbits)(zghz3294, true);
        }
        zgaz3377 = append_fs(zgaz3374, 10, zgaz3375);
      }
      {
        RECREATE(lbits)(&zghz3291);
        CONVERT_OF(lbits, fbits)(&zghz3291, zgaz3376, UINT64_C(32) , true);
        RECREATE(lbits)(&zghz3292);
        CONVERT_OF(lbits, sbits)(&zghz3292, zgaz3377, true);
        append(&zghz3288, zghz3291, zghz3292);
      }
    }
    {
      RECREATE(sail_int)(&zghz3289);
      CONVERT_OF(sail_int, mach_int)(&zghz3289, zcap_len_width);
      RECREATE(lbits)(&zghz3290);
      sail_truncate(&zghz3290, zghz3288, zghz3289);
      zbase = CONVERT_OF(fbits, lbits)(zghz3290, true);
    }
  }
  uint64_t ztop;
  {
    RECREATE(lbits)(&zghz3278);
    {
      uint64_t zgaz3371;
      {
        RECREATE(lbits)(&zghz3285);
        CONVERT_OF(lbits, fbits)(&zghz3285, za_top, UINT64_C(32) , true);
        RECREATE(sail_int)(&zghz3286);
        CONVERT_OF(sail_int, mach_int)(&zghz3286, zcorrection_top);
        RECREATE(lbits)(&zghz3287);
        add_bits_int(&zghz3287, zghz3285, zghz3286);
        zgaz3371 = CONVERT_OF(fbits, lbits)(zghz3287, true);
      }
      sbits zgaz3372;
      {
        uint64_t zgaz3369;
        zgaz3369 = zc.zT;
        sbits zgaz3370;
        {
          RECREATE(sail_int)(&zghz3283);
          CONVERT_OF(sail_int, mach_int)(&zghz3283, zE);
          RECREATE(lbits)(&zghz3284);
          sailgen_zzeros_implicit(&zghz3284, zghz3283);
          zgaz3370 = CONVERT_OF(sbits, lbits)(zghz3284, true);
        }
        zgaz3372 = append_fs(zgaz3369, 10, zgaz3370);
      }
      {
        RECREATE(lbits)(&zghz3281);
        CONVERT_OF(lbits, fbits)(&zghz3281, zgaz3371, UINT64_C(32) , true);
        RECREATE(lbits)(&zghz3282);
        CONVERT_OF(lbits, sbits)(&zghz3282, zgaz3372, true);
        append(&zghz3278, zghz3281, zghz3282);
      }
    }
    {
      RECREATE(sail_int)(&zghz3279);
      CONVERT_OF(sail_int, mach_int)(&zghz3279, zcap_len_width);
      RECREATE(lbits)(&zghz3280);
      sail_truncate(&zghz3280, zghz3278, zghz3279);
      ztop = CONVERT_OF(fbits, lbits)(zghz3280, true);
    }
  }
  uint64_t zbase2;
  {
    uint64_t zgaz3368;
    {
      fbits zgaz3367;
      {
        int64_t zgaz3366;
        {
          RECREATE(sail_int)(&zghz3275);
          CONVERT_OF(sail_int, mach_int)(&zghz3275, zcap_addr_width);
          RECREATE(sail_int)(&zghz3276);
          CONVERT_OF(sail_int, mach_int)(&zghz3276, INT64_C(1));
          RECREATE(sail_int)(&zghz3277);
          sub_int(&zghz3277, zghz3275, zghz3276);
          zgaz3366 = CONVERT_OF(mach_int, sail_int)(zghz3277);
        }
        zgaz3367 = (UINT64_C(1) & (zbase >> zgaz3366));
      }
      zgaz3368 = UINT64_C(0b0);
      zgaz3368 = update_fbits(zgaz3368, INT64_C(0), zgaz3367);
    }
    zbase2 = (UINT64_C(0b0) << 1) | zgaz3368;
  }
  uint64_t ztop2;
  {
    int64_t zgaz3365;
    {
      RECREATE(sail_int)(&zghz3272);
      CONVERT_OF(sail_int, mach_int)(&zghz3272, zcap_addr_width);
      RECREATE(sail_int)(&zghz3273);
      CONVERT_OF(sail_int, mach_int)(&zghz3273, INT64_C(1));
      RECREATE(sail_int)(&zghz3274);
      sub_int(&zghz3274, zghz3272, zghz3273);
      zgaz3365 = CONVERT_OF(mach_int, sail_int)(zghz3274);
    }
    ztop2 = (safe_rshift(UINT64_MAX, 64 - 2) & (ztop >> zgaz3365));
  }
  {
    bool zgaz3361;
    {
      bool zgaz3360;
      {
        int64_t zgaz3358;
        {
          RECREATE(sail_int)(&zghz3269);
          CONVERT_OF(sail_int, mach_int)(&zghz3269, zcap_max_E);
          RECREATE(sail_int)(&zghz3270);
          CONVERT_OF(sail_int, mach_int)(&zghz3270, INT64_C(1));
          RECREATE(sail_int)(&zghz3271);
          sub_int(&zghz3271, zghz3269, zghz3270);
          zgaz3358 = CONVERT_OF(mach_int, sail_int)(zghz3271);
        }
        zgaz3360 = (zE < zgaz3358);
      }
      bool zgsz3712;
      if (zgaz3360) {
        uint64_t zgaz3359;
        zgaz3359 = ((ztop2 - zbase2) & UINT64_C(0b11));
        {
          RECREATE(lbits)(&zghz3267);
          CONVERT_OF(lbits, fbits)(&zghz3267, zgaz3359, UINT64_C(2) , true);
          RECREATE(lbits)(&zghz3268);
          CONVERT_OF(lbits, fbits)(&zghz3268, UINT64_C(0b01), UINT64_C(2) , true);
          zgsz3712 = sailgen_z8operatorz0zK_uz9(zghz3267, zghz3268);
        }
      } else {  zgsz3712 = false;  }
      zgaz3361 = zgsz3712;
    }
    unit zgsz3713;
    if (zgaz3361) {
      fbits zgaz3363;
      {
        fbits zgaz3362;
        zgaz3362 = (UINT64_C(1) & (ztop >> zcap_addr_width));
        zgaz3363 = sailgen_not_bit(zgaz3362);
      }
      {
        RECREATE(lbits)(&zghz3264);
        CONVERT_OF(lbits, fbits)(&zghz3264, ztop, UINT64_C(33) , true);
        RECREATE(sail_int)(&zghz3265);
        CONVERT_OF(sail_int, mach_int)(&zghz3265, zcap_addr_width);
        RECREATE(lbits)(&zghz3266);
        update_lbits(&zghz3266, zghz3264, zghz3265, zgaz3363);
        ztop = CONVERT_OF(fbits, lbits)(zghz3266, true);
      }
      zgsz3713 = UNIT;
    } else {  zgsz3713 = UNIT;  }
  }
  uint64_t zbaseshadowz316;
  {
    int64_t zgaz3364;
    {
      RECREATE(sail_int)(&zghz3261);
      CONVERT_OF(sail_int, mach_int)(&zghz3261, zcap_addr_width);
      RECREATE(sail_int)(&zghz3262);
      CONVERT_OF(sail_int, mach_int)(&zghz3262, INT64_C(1));
      RECREATE(sail_int)(&zghz3263);
      sub_int(&zghz3263, zghz3261, zghz3262);
      zgaz3364 = CONVERT_OF(mach_int, sail_int)(zghz3263);
    }
    zbaseshadowz316 = (safe_rshift(UINT64_MAX, 64 - 32) & (zbase >> INT64_C(0)));
  }
  struct ztuple_z8z5bv32zCz0z5bv33z9 zgsz3714;
  zgsz3714.ztup0 = zbaseshadowz316;
  zgsz3714.ztup1 = ztop;
  {
    struct ztuple_z8z5bvzCz0z5bvz9 zgsz31043;
    CREATE(ztuple_z8z5bvzCz0z5bvz9)(&zgsz31043);
    /* conversions */
    CONVERT_OF(lbits, fbits)(&((&zgsz31043)->ztup0), zgsz3714.ztup0, UINT64_C(32) , true);
    CONVERT_OF(lbits, fbits)(&((&zgsz31043)->ztup1), zgsz3714.ztup1, UINT64_C(33) , true);
    /* end conversions */
    sailgen_SomezIz8bzCbz9zK((*(&zcbz364)), zgsz31043);
    KILL(ztuple_z8z5bvzCz0z5bvz9)(&zgsz31043);
  }
end_function_551: ;
  goto end_function_671;
end_block_exception_554: ;
  goto end_function_671;
end_function_671: ;
}



static void finish_sailgen_getCapBoundsBits(void)
{
  KILL(lbits)(&zghz3323);
  KILL(sail_int)(&zghz3322);
  KILL(lbits)(&zghz3321);
  KILL(lbits)(&zghz3320);
  KILL(sail_int)(&zghz3319);
  KILL(lbits)(&zghz3318);
  KILL(sail_int)(&zghz3317);
  KILL(sail_int)(&zghz3316);
  KILL(sail_int)(&zghz3315);
  KILL(lbits)(&zghz3314);
  KILL(sail_int)(&zghz3313);
  KILL(lbits)(&zghz3312);
  KILL(lbits)(&zghz3311);
  KILL(lbits)(&zghz3310);
  KILL(lbits)(&zghz3309);
  KILL(lbits)(&zghz3308);
  KILL(lbits)(&zghz3307);
  KILL(sail_int)(&zghz3306);
  KILL(sail_int)(&zghz3305);
  KILL(sail_int)(&zghz3304);
  KILL(sail_int)(&zghz3303);
  KILL(sail_int)(&zghz3302);
  KILL(sail_int)(&zghz3301);
  KILL(lbits)(&zghz3300);
  KILL(sail_int)(&zghz3299);
  KILL(lbits)(&zghz3298);
  KILL(lbits)(&zghz3297);
  KILL(sail_int)(&zghz3296);
  KILL(lbits)(&zghz3295);
  KILL(lbits)(&zghz3294);
  KILL(sail_int)(&zghz3293);
  KILL(lbits)(&zghz3292);
  KILL(lbits)(&zghz3291);
  KILL(lbits)(&zghz3290);
  KILL(sail_int)(&zghz3289);
  KILL(lbits)(&zghz3288);
  KILL(lbits)(&zghz3287);
  KILL(sail_int)(&zghz3286);
  KILL(lbits)(&zghz3285);
  KILL(lbits)(&zghz3284);
  KILL(sail_int)(&zghz3283);
  KILL(lbits)(&zghz3282);
  KILL(lbits)(&zghz3281);
  KILL(lbits)(&zghz3280);
  KILL(sail_int)(&zghz3279);
  KILL(lbits)(&zghz3278);
  KILL(sail_int)(&zghz3277);
  KILL(sail_int)(&zghz3276);
  KILL(sail_int)(&zghz3275);
  KILL(sail_int)(&zghz3274);
  KILL(sail_int)(&zghz3273);
  KILL(sail_int)(&zghz3272);
  KILL(sail_int)(&zghz3271);
  KILL(sail_int)(&zghz3270);
  KILL(sail_int)(&zghz3269);
  KILL(lbits)(&zghz3268);
  KILL(lbits)(&zghz3267);
  KILL(lbits)(&zghz3266);
  KILL(sail_int)(&zghz3265);
  KILL(lbits)(&zghz3264);
  KILL(sail_int)(&zghz3263);
  KILL(sail_int)(&zghz3262);
  KILL(sail_int)(&zghz3261);
}

static void sailgen_getCapBounds(struct zoptionzIz8izCiz9zK *rop, struct zCapability);

lbits zghz3324;
sail_int zghz3325;
lbits zghz3326;
sail_int zghz3327;

static void startup_sailgen_getCapBounds(void)
{
  CREATE(lbits)(&zghz3324);
  CREATE(sail_int)(&zghz3325);
  CREATE(lbits)(&zghz3326);
  CREATE(sail_int)(&zghz3327);
}

// struct tuple_(%i64, %i64)
struct ztuple_z8z5i64zCz0z5i64z9 {
  int64_t ztup0;
  int64_t ztup1;
};

static void COPY(ztuple_z8z5i64zCz0z5i64z9)(struct ztuple_z8z5i64zCz0z5i64z9 *rop, const struct ztuple_z8z5i64zCz0z5i64z9 op) {
  rop->ztup0 = op.ztup0;
  rop->ztup1 = op.ztup1;
}

static bool EQUAL(ztuple_z8z5i64zCz0z5i64z9)(struct ztuple_z8z5i64zCz0z5i64z9 op1, struct ztuple_z8z5i64zCz0z5i64z9 op2) {
  return EQUAL(mach_int)(op1.ztup0, op2.ztup0) && EQUAL(mach_int)(op1.ztup1, op2.ztup1);
}

static void sailgen_getCapBounds(struct zoptionzIz8izCiz9zK *zcbz365, struct zCapability zcap)
{
  __label__ case_558, case_557, finish_match_556, end_function_559, end_block_exception_560, end_function_670;

  struct zoptionzIz8bzCbz9zK zgaz3390;
  CREATE(zoptionzIz8bzCbz9zK)(&zgaz3390);
  sailgen_getCapBoundsBits(&zgaz3390, zcap);
  struct zoptionzIz8izCiz9zK zgsz3715;
  CREATE(zoptionzIz8izCiz9zK)(&zgsz3715);
  {
    if (zgaz3390.kind != Kind_zNonezIz8bzCbz9zK) goto case_558;
    sailgen_NonezIz8izCiz9zK(&zgsz3715, UNIT);
    goto finish_match_556;
  }
case_558: ;
  {
    /* complete */
    uint64_t zbase;
    zbase = CONVERT_OF(fbits, lbits)(zgaz3390.variants.zSomezIz8bzCbz9zK.ztup0, true);
    uint64_t ztop;
    ztop = CONVERT_OF(fbits, lbits)(zgaz3390.variants.zSomezIz8bzCbz9zK.ztup1, true);
    struct ztuple_z8z5i64zCz0z5i64z9 zgaz3393;
    {
      int64_t zgaz3391;
      {
        RECREATE(lbits)(&zghz3326);
        CONVERT_OF(lbits, fbits)(&zghz3326, zbase, UINT64_C(32) , true);
        RECREATE(sail_int)(&zghz3327);
        sail_unsigned(&zghz3327, zghz3326);
        zgaz3391 = CONVERT_OF(mach_int, sail_int)(zghz3327);
      }
      int64_t zgaz3392;
      {
        RECREATE(lbits)(&zghz3324);
        CONVERT_OF(lbits, fbits)(&zghz3324, ztop, UINT64_C(33) , true);
        RECREATE(sail_int)(&zghz3325);
        sail_unsigned(&zghz3325, zghz3324);
        zgaz3392 = CONVERT_OF(mach_int, sail_int)(zghz3325);
      }
      zgaz3393.ztup0 = zgaz3391;
      zgaz3393.ztup1 = zgaz3392;
    }
    {
      struct ztuple_z8z5izCz0z5iz9 zgsz31054;
      CREATE(ztuple_z8z5izCz0z5iz9)(&zgsz31054);
      /* conversions */
      CONVERT_OF(sail_int, mach_int)(&((&zgsz31054)->ztup0), zgaz3393.ztup0);
      CONVERT_OF(sail_int, mach_int)(&((&zgsz31054)->ztup1), zgaz3393.ztup1);
      /* end conversions */
      sailgen_SomezIz8izCiz9zK(&zgsz3715, zgsz31054);
      KILL(ztuple_z8z5izCz0z5iz9)(&zgsz31054);
    }
    goto finish_match_556;
  }
case_557: ;
finish_match_556: ;
  COPY(zoptionzIz8izCiz9zK)((*(&zcbz365)), zgsz3715);
  KILL(zoptionzIz8izCiz9zK)(&zgsz3715);
  KILL(zoptionzIz8bzCbz9zK)(&zgaz3390);
end_function_559: ;
  goto end_function_670;
end_block_exception_560: ;
  goto end_function_670;
end_function_670: ;
}



static void finish_sailgen_getCapBounds(void)
{
  KILL(sail_int)(&zghz3327);
  KILL(lbits)(&zghz3326);
  KILL(sail_int)(&zghz3325);
  KILL(lbits)(&zghz3324);
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

static struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 sailgen_setCapBounds(struct zCapability, uint64_t, uint64_t);

struct zCapability zghz3328;
sail_int zghz3329;
lbits zghz3330;
sail_int zghz3331;
lbits zghz3332;
lbits zghz3333;
sail_int zghz3334;
lbits zghz3335;
lbits zghz3336;
sail_int zghz3337;
lbits zghz3338;
sail_int zghz3339;
sail_int zghz3340;
sail_int zghz3341;
lbits zghz3342;
sail_int zghz3343;
lbits zghz3344;
lbits zghz3345;
sail_int zghz3346;
lbits zghz3347;
sail_int zghz3348;
sail_int zghz3349;
sail_int zghz3350;
lbits zghz3351;
sail_int zghz3352;
lbits zghz3353;
sail_int zghz3354;
sail_int zghz3355;
sail_int zghz3356;
lbits zghz3357;
sail_int zghz3358;
lbits zghz3359;
sail_int zghz3360;
lbits zghz3361;
lbits zghz3362;
sail_int zghz3363;
lbits zghz3364;
lbits zghz3365;
sail_int zghz3366;
lbits zghz3367;
sail_int zghz3368;
sail_int zghz3369;
sail_int zghz3370;
lbits zghz3371;
sail_int zghz3372;
lbits zghz3373;
lbits zghz3374;
sail_int zghz3375;
lbits zghz3376;
sail_int zghz3377;
sail_int zghz3378;
sail_int zghz3379;
lbits zghz3380;
sail_int zghz3381;
lbits zghz3382;
lbits zghz3383;
sail_int zghz3384;
lbits zghz3385;
lbits zghz3386;
sail_int zghz3387;
lbits zghz3388;
sail_int zghz3389;
sail_int zghz3390;
sail_int zghz3391;
sail_int zghz3392;
sail_int zghz3393;
sail_int zghz3394;
sail_int zghz3395;
sail_int zghz3396;
lbits zghz3397;
sail_int zghz3398;
sail_int zghz3399;
sail_int zghz3400;
sail_int zghz3401;

static void startup_sailgen_setCapBounds(void)
{

  CREATE(sail_int)(&zghz3329);
  CREATE(lbits)(&zghz3330);
  CREATE(sail_int)(&zghz3331);
  CREATE(lbits)(&zghz3332);
  CREATE(lbits)(&zghz3333);
  CREATE(sail_int)(&zghz3334);
  CREATE(lbits)(&zghz3335);
  CREATE(lbits)(&zghz3336);
  CREATE(sail_int)(&zghz3337);
  CREATE(lbits)(&zghz3338);
  CREATE(sail_int)(&zghz3339);
  CREATE(sail_int)(&zghz3340);
  CREATE(sail_int)(&zghz3341);
  CREATE(lbits)(&zghz3342);
  CREATE(sail_int)(&zghz3343);
  CREATE(lbits)(&zghz3344);
  CREATE(lbits)(&zghz3345);
  CREATE(sail_int)(&zghz3346);
  CREATE(lbits)(&zghz3347);
  CREATE(sail_int)(&zghz3348);
  CREATE(sail_int)(&zghz3349);
  CREATE(sail_int)(&zghz3350);
  CREATE(lbits)(&zghz3351);
  CREATE(sail_int)(&zghz3352);
  CREATE(lbits)(&zghz3353);
  CREATE(sail_int)(&zghz3354);
  CREATE(sail_int)(&zghz3355);
  CREATE(sail_int)(&zghz3356);
  CREATE(lbits)(&zghz3357);
  CREATE(sail_int)(&zghz3358);
  CREATE(lbits)(&zghz3359);
  CREATE(sail_int)(&zghz3360);
  CREATE(lbits)(&zghz3361);
  CREATE(lbits)(&zghz3362);
  CREATE(sail_int)(&zghz3363);
  CREATE(lbits)(&zghz3364);
  CREATE(lbits)(&zghz3365);
  CREATE(sail_int)(&zghz3366);
  CREATE(lbits)(&zghz3367);
  CREATE(sail_int)(&zghz3368);
  CREATE(sail_int)(&zghz3369);
  CREATE(sail_int)(&zghz3370);
  CREATE(lbits)(&zghz3371);
  CREATE(sail_int)(&zghz3372);
  CREATE(lbits)(&zghz3373);
  CREATE(lbits)(&zghz3374);
  CREATE(sail_int)(&zghz3375);
  CREATE(lbits)(&zghz3376);
  CREATE(sail_int)(&zghz3377);
  CREATE(sail_int)(&zghz3378);
  CREATE(sail_int)(&zghz3379);
  CREATE(lbits)(&zghz3380);
  CREATE(sail_int)(&zghz3381);
  CREATE(lbits)(&zghz3382);
  CREATE(lbits)(&zghz3383);
  CREATE(sail_int)(&zghz3384);
  CREATE(lbits)(&zghz3385);
  CREATE(lbits)(&zghz3386);
  CREATE(sail_int)(&zghz3387);
  CREATE(lbits)(&zghz3388);
  CREATE(sail_int)(&zghz3389);
  CREATE(sail_int)(&zghz3390);
  CREATE(sail_int)(&zghz3391);
  CREATE(sail_int)(&zghz3392);
  CREATE(sail_int)(&zghz3393);
  CREATE(sail_int)(&zghz3394);
  CREATE(sail_int)(&zghz3395);
  CREATE(sail_int)(&zghz3396);
  CREATE(lbits)(&zghz3397);
  CREATE(sail_int)(&zghz3398);
  CREATE(sail_int)(&zghz3399);
  CREATE(sail_int)(&zghz3400);
  CREATE(sail_int)(&zghz3401);
}

static struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 sailgen_setCapBounds(struct zCapability zcap, uint64_t zbase, uint64_t ztop)
{
  __label__ end_function_562, end_block_exception_563;

  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz366;
  uint64_t zext_base;
  zext_base = (UINT64_C(0b0) << 32) | zbase;
  uint64_t zlength;
  zlength = ((ztop - zext_base) & UINT64_C(0b111111111111111111111111111111111));
  int64_t ze;
  {
    int64_t zgaz3431;
    {
      uint64_t zgaz3430;
      {
        int64_t zgaz3429;
        {
          RECREATE(sail_int)(&zghz3399);
          CONVERT_OF(sail_int, mach_int)(&zghz3399, zcap_mantissa_width);
          RECREATE(sail_int)(&zghz3400);
          CONVERT_OF(sail_int, mach_int)(&zghz3400, INT64_C(1));
          RECREATE(sail_int)(&zghz3401);
          sub_int(&zghz3401, zghz3399, zghz3400);
          zgaz3429 = CONVERT_OF(mach_int, sail_int)(zghz3401);
        }
        zgaz3430 = (safe_rshift(UINT64_MAX, 64 - 24) & (zlength >> zgaz3429));
      }
      {
        RECREATE(lbits)(&zghz3397);
        CONVERT_OF(lbits, fbits)(&zghz3397, zgaz3430, UINT64_C(24) , true);
        RECREATE(sail_int)(&zghz3398);
        count_leading_zeros(&zghz3398, zghz3397);
        zgaz3431 = CONVERT_OF(mach_int, sail_int)(zghz3398);
      }
    }
    {
      RECREATE(sail_int)(&zghz3394);
      CONVERT_OF(sail_int, mach_int)(&zghz3394, zcap_max_E);
      RECREATE(sail_int)(&zghz3395);
      CONVERT_OF(sail_int, mach_int)(&zghz3395, zgaz3431);
      RECREATE(sail_int)(&zghz3396);
      sub_int(&zghz3396, zghz3394, zghz3395);
      ze = CONVERT_OF(mach_int, sail_int)(zghz3396);
    }
  }
  bool zie;
  {
    bool zgaz3428;
    {
      RECREATE(sail_int)(&zghz3392);
      CONVERT_OF(sail_int, mach_int)(&zghz3392, ze);
      RECREATE(sail_int)(&zghz3393);
      CONVERT_OF(sail_int, mach_int)(&zghz3393, INT64_C(0));
      zgaz3428 = sailgen_neq_int(zghz3392, zghz3393);
    }
    bool zgsz3720;
    if (zgaz3428) {  zgsz3720 = true;  } else {
      bool zgaz3427;
      zgaz3427 = sailgen_not(zcap_use_L8);
      bool zgsz3719;
      if (zgaz3427) {
        fbits zgaz3426;
        {
          int64_t zgaz3425;
          {
            RECREATE(sail_int)(&zghz3389);
            CONVERT_OF(sail_int, mach_int)(&zghz3389, zcap_mantissa_width);
            RECREATE(sail_int)(&zghz3390);
            CONVERT_OF(sail_int, mach_int)(&zghz3390, INT64_C(2));
            RECREATE(sail_int)(&zghz3391);
            sub_int(&zghz3391, zghz3389, zghz3390);
            zgaz3425 = CONVERT_OF(mach_int, sail_int)(zghz3391);
          }
          zgaz3426 = (UINT64_C(1) & (zlength >> zgaz3425));
        }
        zgsz3719 = eq_bit(zgaz3426, UINT64_C(1));
      } else {  zgsz3719 = false;  }
      zgsz3720 = zgsz3719;
    }
    zie = zgsz3720;
  }
  uint64_t zBbits;
  {
    RECREATE(lbits)(&zghz3386);
    CONVERT_OF(lbits, fbits)(&zghz3386, zbase, UINT64_C(32) , true);
    RECREATE(sail_int)(&zghz3387);
    CONVERT_OF(sail_int, mach_int)(&zghz3387, zcap_mantissa_width);
    RECREATE(lbits)(&zghz3388);
    sail_truncate(&zghz3388, zghz3386, zghz3387);
    zBbits = CONVERT_OF(fbits, lbits)(zghz3388, true);
  }
  uint64_t zTbits;
  {
    RECREATE(lbits)(&zghz3383);
    CONVERT_OF(lbits, fbits)(&zghz3383, ztop, UINT64_C(33) , true);
    RECREATE(sail_int)(&zghz3384);
    CONVERT_OF(sail_int, mach_int)(&zghz3384, zcap_mantissa_width);
    RECREATE(lbits)(&zghz3385);
    sail_truncate(&zghz3385, zghz3383, zghz3384);
    zTbits = CONVERT_OF(fbits, lbits)(zghz3385, true);
  }
  bool zlostSignificantTop;
  zlostSignificantTop = false;
  bool zlostSignificantBase;
  zlostSignificantBase = false;
  bool zincE;
  zincE = false;
  {
    unit zgsz3721;
    if (zie) {
      uint64_t zB_ie;
      {
        uint64_t zgaz3419;
        {
          int64_t zgaz3418;
          zgaz3418 = (ze + zcap_xe_width);
          {
            RECREATE(lbits)(&zghz3380);
            CONVERT_OF(lbits, fbits)(&zghz3380, zbase, UINT64_C(32) , true);
            RECREATE(sail_int)(&zghz3381);
            CONVERT_OF(sail_int, mach_int)(&zghz3381, zgaz3418);
            RECREATE(lbits)(&zghz3382);
            shiftr(&zghz3382, zghz3380, zghz3381);
            zgaz3419 = CONVERT_OF(fbits, lbits)(zghz3382, true);
          }
        }
        int64_t zgaz3420;
        {
          RECREATE(sail_int)(&zghz3377);
          CONVERT_OF(sail_int, mach_int)(&zghz3377, zcap_mantissa_width);
          RECREATE(sail_int)(&zghz3378);
          CONVERT_OF(sail_int, mach_int)(&zghz3378, zcap_xe_width);
          RECREATE(sail_int)(&zghz3379);
          sub_int(&zghz3379, zghz3377, zghz3378);
          zgaz3420 = CONVERT_OF(mach_int, sail_int)(zghz3379);
        }
        {
          RECREATE(lbits)(&zghz3374);
          CONVERT_OF(lbits, fbits)(&zghz3374, zgaz3419, UINT64_C(32) , true);
          RECREATE(sail_int)(&zghz3375);
          CONVERT_OF(sail_int, mach_int)(&zghz3375, zgaz3420);
          RECREATE(lbits)(&zghz3376);
          sail_truncate(&zghz3376, zghz3374, zghz3375);
          zB_ie = CONVERT_OF(fbits, lbits)(zghz3376, true);
        }
      }
      uint64_t zT_ie;
      {
        uint64_t zgaz3416;
        {
          int64_t zgaz3415;
          zgaz3415 = (ze + zcap_xe_width);
          {
            RECREATE(lbits)(&zghz3371);
            CONVERT_OF(lbits, fbits)(&zghz3371, ztop, UINT64_C(33) , true);
            RECREATE(sail_int)(&zghz3372);
            CONVERT_OF(sail_int, mach_int)(&zghz3372, zgaz3415);
            RECREATE(lbits)(&zghz3373);
            shiftr(&zghz3373, zghz3371, zghz3372);
            zgaz3416 = CONVERT_OF(fbits, lbits)(zghz3373, true);
          }
        }
        int64_t zgaz3417;
        {
          RECREATE(sail_int)(&zghz3368);
          CONVERT_OF(sail_int, mach_int)(&zghz3368, zcap_mantissa_width);
          RECREATE(sail_int)(&zghz3369);
          CONVERT_OF(sail_int, mach_int)(&zghz3369, zcap_xe_width);
          RECREATE(sail_int)(&zghz3370);
          sub_int(&zghz3370, zghz3368, zghz3369);
          zgaz3417 = CONVERT_OF(mach_int, sail_int)(zghz3370);
        }
        {
          RECREATE(lbits)(&zghz3365);
          CONVERT_OF(lbits, fbits)(&zghz3365, zgaz3416, UINT64_C(33) , true);
          RECREATE(sail_int)(&zghz3366);
          CONVERT_OF(sail_int, mach_int)(&zghz3366, zgaz3417);
          RECREATE(lbits)(&zghz3367);
          sail_truncate(&zghz3367, zghz3365, zghz3366);
          zT_ie = CONVERT_OF(fbits, lbits)(zghz3367, true);
        }
      }
      uint64_t zmaskLo;
      {
        sbits zgaz3414;
        {
          int64_t zgaz3413;
          zgaz3413 = (ze + zcap_xe_width);
          {
            RECREATE(sail_int)(&zghz3363);
            CONVERT_OF(sail_int, mach_int)(&zghz3363, zgaz3413);
            RECREATE(lbits)(&zghz3364);
            sailgen_ones(&zghz3364, zghz3363);
            zgaz3414 = CONVERT_OF(sbits, lbits)(zghz3364, true);
          }
        }
        {
          RECREATE(sail_int)(&zghz3360);
          CONVERT_OF(sail_int, mach_int)(&zghz3360, INT64_C(33));
          RECREATE(lbits)(&zghz3361);
          CONVERT_OF(lbits, sbits)(&zghz3361, zgaz3414, true);
          RECREATE(lbits)(&zghz3362);
          sailgen_zzero_extend(&zghz3362, zghz3360, zghz3361);
          zmaskLo = CONVERT_OF(fbits, lbits)(zghz3362, true);
        }
      }
      {
        uint64_t zgaz3394;
        zgaz3394 = (zext_base & zmaskLo);
        zlostSignificantBase = (zgaz3394 != UINT64_C(0b000000000000000000000000000000000));
        unit zgsz3724;
        zgsz3724 = UNIT;
      }
      {
        uint64_t zgaz3395;
        zgaz3395 = (ztop & zmaskLo);
        zlostSignificantTop = (zgaz3395 != UINT64_C(0b000000000000000000000000000000000));
        unit zgsz3723;
        zgsz3723 = UNIT;
      }
      {
        unit zgsz3722;
        if (zlostSignificantTop) {
          {
            RECREATE(lbits)(&zghz3357);
            CONVERT_OF(lbits, fbits)(&zghz3357, zT_ie, UINT64_C(8) , true);
            RECREATE(sail_int)(&zghz3358);
            CONVERT_OF(sail_int, mach_int)(&zghz3358, INT64_C(1));
            RECREATE(lbits)(&zghz3359);
            add_bits_int(&zghz3359, zghz3357, zghz3358);
            zT_ie = CONVERT_OF(fbits, lbits)(zghz3359, true);
          }
          zgsz3722 = UNIT;
        } else {  zgsz3722 = UNIT;  }
      }
      uint64_t zlen_ie;
      zlen_ie = ((zT_ie - zB_ie) & UINT64_C(0xFF));
      {
        bool zgaz3399;
        {
          fbits zgaz3398;
          {
            int64_t zgaz3397;
            {
              int64_t zgaz3396;
              zgaz3396 = (zcap_xe_width + INT64_C(1));
              {
                RECREATE(sail_int)(&zghz3354);
                CONVERT_OF(sail_int, mach_int)(&zghz3354, zcap_mantissa_width);
                RECREATE(sail_int)(&zghz3355);
                CONVERT_OF(sail_int, mach_int)(&zghz3355, zgaz3396);
                RECREATE(sail_int)(&zghz3356);
                sub_int(&zghz3356, zghz3354, zghz3355);
                zgaz3397 = CONVERT_OF(mach_int, sail_int)(zghz3356);
              }
            }
            zgaz3398 = (UINT64_C(1) & (zlen_ie >> zgaz3397));
          }
          zgaz3399 = eq_bit(zgaz3398, UINT64_C(1));
        }
        unit zgsz3726;
        if (zgaz3399) {
          {
            zincE = true;
            unit zgsz3732;
            zgsz3732 = UNIT;
          }
          {
            bool zgsz3727;
            if (zlostSignificantBase) {  zgsz3727 = true;  } else {
              fbits zgaz3400;
              zgaz3400 = (UINT64_C(1) & (zB_ie >> INT64_C(0)));
              zgsz3727 = eq_bit(zgaz3400, UINT64_C(1));
            }
            zlostSignificantBase = zgsz3727;
            unit zgsz3731;
            zgsz3731 = UNIT;
          }
          {
            bool zgsz3728;
            if (zlostSignificantTop) {  zgsz3728 = true;  } else {
              fbits zgaz3401;
              zgaz3401 = (UINT64_C(1) & (zT_ie >> INT64_C(0)));
              zgsz3728 = eq_bit(zgaz3401, UINT64_C(1));
            }
            zlostSignificantTop = zgsz3728;
            unit zgsz3730;
            zgsz3730 = UNIT;
          }
          {
            uint64_t zgaz3404;
            {
              int64_t zgaz3403;
              {
                int64_t zgaz3402;
                zgaz3402 = (ze + zcap_xe_width);
                zgaz3403 = (zgaz3402 + INT64_C(1));
              }
              {
                RECREATE(lbits)(&zghz3351);
                CONVERT_OF(lbits, fbits)(&zghz3351, zbase, UINT64_C(32) , true);
                RECREATE(sail_int)(&zghz3352);
                CONVERT_OF(sail_int, mach_int)(&zghz3352, zgaz3403);
                RECREATE(lbits)(&zghz3353);
                shiftr(&zghz3353, zghz3351, zghz3352);
                zgaz3404 = CONVERT_OF(fbits, lbits)(zghz3353, true);
              }
            }
            int64_t zgaz3405;
            {
              RECREATE(sail_int)(&zghz3348);
              CONVERT_OF(sail_int, mach_int)(&zghz3348, zcap_mantissa_width);
              RECREATE(sail_int)(&zghz3349);
              CONVERT_OF(sail_int, mach_int)(&zghz3349, zcap_xe_width);
              RECREATE(sail_int)(&zghz3350);
              sub_int(&zghz3350, zghz3348, zghz3349);
              zgaz3405 = CONVERT_OF(mach_int, sail_int)(zghz3350);
            }
            {
              RECREATE(lbits)(&zghz3345);
              CONVERT_OF(lbits, fbits)(&zghz3345, zgaz3404, UINT64_C(32) , true);
              RECREATE(sail_int)(&zghz3346);
              CONVERT_OF(sail_int, mach_int)(&zghz3346, zgaz3405);
              RECREATE(lbits)(&zghz3347);
              sail_truncate(&zghz3347, zghz3345, zghz3346);
              zB_ie = CONVERT_OF(fbits, lbits)(zghz3347, true);
            }
            unit zgsz3729;
            zgsz3729 = UNIT;
          }
          int64_t zincT;
          if (zlostSignificantTop) {  zincT = INT64_C(1);  } else {  zincT = INT64_C(0);  }
          uint64_t zgaz3410;
          {
            uint64_t zgaz3408;
            {
              int64_t zgaz3407;
              {
                int64_t zgaz3406;
                zgaz3406 = (ze + zcap_xe_width);
                zgaz3407 = (zgaz3406 + INT64_C(1));
              }
              {
                RECREATE(lbits)(&zghz3342);
                CONVERT_OF(lbits, fbits)(&zghz3342, ztop, UINT64_C(33) , true);
                RECREATE(sail_int)(&zghz3343);
                CONVERT_OF(sail_int, mach_int)(&zghz3343, zgaz3407);
                RECREATE(lbits)(&zghz3344);
                shiftr(&zghz3344, zghz3342, zghz3343);
                zgaz3408 = CONVERT_OF(fbits, lbits)(zghz3344, true);
              }
            }
            int64_t zgaz3409;
            {
              RECREATE(sail_int)(&zghz3339);
              CONVERT_OF(sail_int, mach_int)(&zghz3339, zcap_mantissa_width);
              RECREATE(sail_int)(&zghz3340);
              CONVERT_OF(sail_int, mach_int)(&zghz3340, zcap_xe_width);
              RECREATE(sail_int)(&zghz3341);
              sub_int(&zghz3341, zghz3339, zghz3340);
              zgaz3409 = CONVERT_OF(mach_int, sail_int)(zghz3341);
            }
            {
              RECREATE(lbits)(&zghz3336);
              CONVERT_OF(lbits, fbits)(&zghz3336, zgaz3408, UINT64_C(33) , true);
              RECREATE(sail_int)(&zghz3337);
              CONVERT_OF(sail_int, mach_int)(&zghz3337, zgaz3409);
              RECREATE(lbits)(&zghz3338);
              sail_truncate(&zghz3338, zghz3336, zghz3337);
              zgaz3410 = CONVERT_OF(fbits, lbits)(zghz3338, true);
            }
          }
          {
            RECREATE(lbits)(&zghz3333);
            CONVERT_OF(lbits, fbits)(&zghz3333, zgaz3410, UINT64_C(8) , true);
            RECREATE(sail_int)(&zghz3334);
            CONVERT_OF(sail_int, mach_int)(&zghz3334, zincT);
            RECREATE(lbits)(&zghz3335);
            add_bits_int(&zghz3335, zghz3333, zghz3334);
            zT_ie = CONVERT_OF(fbits, lbits)(zghz3335, true);
          }
          zgsz3726 = UNIT;
        } else {  zgsz3726 = UNIT;  }
      }
      {
        uint64_t zgaz3411;
        {
          RECREATE(sail_int)(&zghz3331);
          CONVERT_OF(sail_int, mach_int)(&zghz3331, zcap_xe_width);
          RECREATE(lbits)(&zghz3332);
          sailgen_zzeros_implicit(&zghz3332, zghz3331);
          zgaz3411 = CONVERT_OF(fbits, lbits)(zghz3332, true);
        }
        zBbits = (zB_ie << 2) | zgaz3411;
        unit zgsz3725;
        zgsz3725 = UNIT;
      }
      uint64_t zgaz3412;
      {
        RECREATE(sail_int)(&zghz3329);
        CONVERT_OF(sail_int, mach_int)(&zghz3329, zcap_xe_width);
        RECREATE(lbits)(&zghz3330);
        sailgen_zzeros_implicit(&zghz3330, zghz3329);
        zgaz3412 = CONVERT_OF(fbits, lbits)(zghz3330, true);
      }
      zTbits = (zT_ie << 2) | zgaz3412;
      zgsz3721 = UNIT;
    } else {  zgsz3721 = UNIT;  }
  }
  struct zCapability zghz3328;
  {
    int64_t zgaz3424;
    if (zincE) {
      bool zgaz3423;
      {
        int64_t zgaz3422;
        zgaz3422 = (ze + INT64_C(1));
        zgaz3423 = (zgaz3422 <= zcap_max_E);
      }
      if (zgaz3423) {  zgaz3424 = (ze + INT64_C(1));  } else {  zgaz3424 = zcap_min_E;  }
    } else {  zgaz3424 = ze;  }
    zghz3328 = zcap;
    zghz3328.zB = zBbits;
    zghz3328.zE = zgaz3424;
    zghz3328.zT = zTbits;
    zghz3328.zaddress = zbase;
    zghz3328.zinternal_exponent = zie;
  }
  bool zexact;
  {
    bool zgaz3421;
    {    if (zlostSignificantBase) {  zgaz3421 = true;  } else {  zgaz3421 = zlostSignificantTop;  }
    }
    zexact = sailgen_not(zgaz3421);
  }
  zcbz366.ztup0 = zexact;
  zcbz366.ztup1 = zghz3328;
end_function_562: ;
  return zcbz366;
end_block_exception_563: ;
  struct zCapability zcbz3110 = { .zB = UINT64_C(0xdeadc0de), .zE = INT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zap_m = UINT64_C(0xdeadc0de), .zcl = UINT64_C(0xdeadc0de), .zinternal_exponent = false, .zreserved_0 = UINT64_C(0xdeadc0de), .zreserved_1 = UINT64_C(0xdeadc0de), .zsd_perms = UINT64_C(0xdeadc0de), .zsealed = false, .ztag = false };
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz3109 = { .ztup0 = false, .ztup1 = zcbz3110 };
  return zcbz3109;
}



static void finish_sailgen_setCapBounds(void)
{
  KILL(sail_int)(&zghz3401);
  KILL(sail_int)(&zghz3400);
  KILL(sail_int)(&zghz3399);
  KILL(sail_int)(&zghz3398);
  KILL(lbits)(&zghz3397);
  KILL(sail_int)(&zghz3396);
  KILL(sail_int)(&zghz3395);
  KILL(sail_int)(&zghz3394);
  KILL(sail_int)(&zghz3393);
  KILL(sail_int)(&zghz3392);
  KILL(sail_int)(&zghz3391);
  KILL(sail_int)(&zghz3390);
  KILL(sail_int)(&zghz3389);
  KILL(lbits)(&zghz3388);
  KILL(sail_int)(&zghz3387);
  KILL(lbits)(&zghz3386);
  KILL(lbits)(&zghz3385);
  KILL(sail_int)(&zghz3384);
  KILL(lbits)(&zghz3383);
  KILL(lbits)(&zghz3382);
  KILL(sail_int)(&zghz3381);
  KILL(lbits)(&zghz3380);
  KILL(sail_int)(&zghz3379);
  KILL(sail_int)(&zghz3378);
  KILL(sail_int)(&zghz3377);
  KILL(lbits)(&zghz3376);
  KILL(sail_int)(&zghz3375);
  KILL(lbits)(&zghz3374);
  KILL(lbits)(&zghz3373);
  KILL(sail_int)(&zghz3372);
  KILL(lbits)(&zghz3371);
  KILL(sail_int)(&zghz3370);
  KILL(sail_int)(&zghz3369);
  KILL(sail_int)(&zghz3368);
  KILL(lbits)(&zghz3367);
  KILL(sail_int)(&zghz3366);
  KILL(lbits)(&zghz3365);
  KILL(lbits)(&zghz3364);
  KILL(sail_int)(&zghz3363);
  KILL(lbits)(&zghz3362);
  KILL(lbits)(&zghz3361);
  KILL(sail_int)(&zghz3360);
  KILL(lbits)(&zghz3359);
  KILL(sail_int)(&zghz3358);
  KILL(lbits)(&zghz3357);
  KILL(sail_int)(&zghz3356);
  KILL(sail_int)(&zghz3355);
  KILL(sail_int)(&zghz3354);
  KILL(lbits)(&zghz3353);
  KILL(sail_int)(&zghz3352);
  KILL(lbits)(&zghz3351);
  KILL(sail_int)(&zghz3350);
  KILL(sail_int)(&zghz3349);
  KILL(sail_int)(&zghz3348);
  KILL(lbits)(&zghz3347);
  KILL(sail_int)(&zghz3346);
  KILL(lbits)(&zghz3345);
  KILL(lbits)(&zghz3344);
  KILL(sail_int)(&zghz3343);
  KILL(lbits)(&zghz3342);
  KILL(sail_int)(&zghz3341);
  KILL(sail_int)(&zghz3340);
  KILL(sail_int)(&zghz3339);
  KILL(lbits)(&zghz3338);
  KILL(sail_int)(&zghz3337);
  KILL(lbits)(&zghz3336);
  KILL(lbits)(&zghz3335);
  KILL(sail_int)(&zghz3334);
  KILL(lbits)(&zghz3333);
  KILL(lbits)(&zghz3332);
  KILL(sail_int)(&zghz3331);
  KILL(lbits)(&zghz3330);
  KILL(sail_int)(&zghz3329);
}

static bool sailgen_capIsSealed(struct zCapability);

static bool sailgen_capIsSealed(struct zCapability zcap)
{
  __label__ end_function_565, end_block_exception_566;

  bool zcbz367;
  zcbz367 = zcap.zsealed;
end_function_565: ;
  return zcbz367;
end_block_exception_566: ;

  return false;
}

static bool sailgen_permsMalformed(struct zCapability);

static bool sailgen_permsMalformed(struct zCapability zcap)
{
  __label__ end_function_568, end_block_exception_569;

  bool zcbz368;
  bool zgaz3433;
  {
    uint64_t zgaz3432;
    zgaz3432 = zcap.zap_m;
    zgaz3433 = sailgen_ap_m_encdec_forwards_matches(zgaz3432);
  }
  zcbz368 = sailgen_not(zgaz3433);
end_function_568: ;
  return zcbz368;
end_block_exception_569: ;

  return false;
}

static void sailgen_getCapPermsAndMode(struct zoptionzIz8RArchPermszCEExecutionModez5z9zK *rop, struct zCapability);

static void sailgen_getCapPermsAndMode(struct zoptionzIz8RArchPermszCEExecutionModez5z9zK *zcbz369, struct zCapability zcap)
{
  __label__ end_function_571, end_block_exception_572, end_function_669;

  bool zgaz3434;
  zgaz3434 = sailgen_permsMalformed(zcap);
  if (zgaz3434) {  sailgen_NonezIz8RArchPermszCEExecutionModez5z9zK((*(&zcbz369)), UNIT);  } else {
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgaz3436;
    {
      uint64_t zgaz3435;
      zgaz3435 = zcap.zap_m;
      zgaz3436 = sailgen_ap_m_encdec_forwards(zgaz3435);
    }
    sailgen_SomezIz8RArchPermszCEExecutionModez5z9zK((*(&zcbz369)), zgaz3436);
  }
end_function_571: ;
  goto end_function_669;
end_block_exception_572: ;
  goto end_function_669;
end_function_669: ;
}

static struct zCapability sailgen_setCapPermsAndMode(struct zCapability, struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9);

struct zCapability zghz3402;
struct zArchPerms zghz3403;

static void startup_sailgen_setCapPermsAndMode(void)
{


}

static struct zCapability sailgen_setCapPermsAndMode(struct zCapability zcap, struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz3736)
{
  __label__ fundef_fail_573, fundef_body_574, end_function_575, end_block_exception_576;

  struct zCapability zghz3402;
  struct zArchPerms zghz3403;
  zghz3403 = zgsz3736.ztup0;
  enum zExecutionMode zmode;
  zmode = zgsz3736.ztup1;
  goto fundef_body_574;
fundef_fail_573: ;
  sail_match_failure("setCapPermsAndMode");
fundef_body_574: ;
  uint64_t zgaz3438;
  {
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgaz3437;
    zgaz3437 = sailgen_legalizzePermsAndMode(zghz3403, zmode);
    zgaz3438 = sailgen_ap_m_encdec_backwards(zgaz3437);
  }
  zghz3402 = zcap;
  zghz3402.zap_m = zgaz3438;
end_function_575: ;
  return zghz3402;
end_block_exception_576: ;
  struct zCapability zcbz3111 = { .zB = UINT64_C(0xdeadc0de), .zE = INT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zap_m = UINT64_C(0xdeadc0de), .zcl = UINT64_C(0xdeadc0de), .zinternal_exponent = false, .zreserved_0 = UINT64_C(0xdeadc0de), .zreserved_1 = UINT64_C(0xdeadc0de), .zsd_perms = UINT64_C(0xdeadc0de), .zsealed = false, .ztag = false };
  return zcbz3111;
}



static void finish_sailgen_setCapPermsAndMode(void)
{
}

static void sailgen_getArchPerms(struct zoptionzIRArchPermszK *rop, struct zCapability);

struct zArchPerms zghz3404;

static void startup_sailgen_getArchPerms(void)
{
}

static void sailgen_getArchPerms(struct zoptionzIRArchPermszK *zcbz371, struct zCapability zcap)
{
  __label__ case_580, case_579, finish_match_578, end_function_581, end_block_exception_582, end_function_668;

  struct zoptionzIz8RArchPermszCEExecutionModez5z9zK zgaz3439;
  CREATE(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zgaz3439);
  sailgen_getCapPermsAndMode(&zgaz3439, zcap);
  struct zoptionzIRArchPermszK zgsz3738;
  CREATE(zoptionzIRArchPermszK)(&zgsz3738);
  {
    if (zgaz3439.kind != Kind_zNonezIz8RArchPermszCEExecutionModez5z9zK) goto case_580;
    sailgen_NonezIRArchPermszK(&zgsz3738, UNIT);
    goto finish_match_578;
  }
case_580: ;
  {
    /* complete */
    struct zArchPerms zghz3404;
    zghz3404 = zgaz3439.variants.zSomezIz8RArchPermszCEExecutionModez5z9zK.ztup0;
    sailgen_SomezIRArchPermszK(&zgsz3738, zghz3404);
    goto finish_match_578;
  }
case_579: ;
finish_match_578: ;
  COPY(zoptionzIRArchPermszK)((*(&zcbz371)), zgsz3738);
  KILL(zoptionzIRArchPermszK)(&zgsz3738);
  KILL(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zgaz3439);
end_function_581: ;
  goto end_function_668;
end_block_exception_582: ;
  goto end_function_668;
end_function_668: ;
}



static void finish_sailgen_getArchPerms(void)
{
}

static struct zArchPerms sailgen_getArchPermsLegalizzed(struct zCapability);

struct zArchPerms zghz3405;
struct zArchPerms zghz3406;

static void startup_sailgen_getArchPermsLegalizzed(void)
{


}

static struct zArchPerms sailgen_getArchPermsLegalizzed(struct zCapability zcap)
{
  __label__ case_586, case_585, finish_match_584, end_function_587, end_block_exception_588;

  struct zArchPerms zghz3405;
  struct zoptionzIRArchPermszK zgaz3440;
  CREATE(zoptionzIRArchPermszK)(&zgaz3440);
  sailgen_getArchPerms(&zgaz3440, zcap);
  {
    if (zgaz3440.kind != Kind_zNonezIRArchPermszK) goto case_586;
    zghz3405 = znull_perms;
    goto finish_match_584;
  }
case_586: ;
  {
    /* complete */
    struct zArchPerms zghz3406;
    zghz3406 = zgaz3440.variants.zSomezIRArchPermszK;
    zghz3405 = zghz3406;
    goto finish_match_584;
  }
case_585: ;
finish_match_584: ;
  KILL(zoptionzIRArchPermszK)(&zgaz3440);
end_function_587: ;
  return zghz3405;
end_block_exception_588: ;
  struct zArchPerms zcbz3112 = { .zASR = false, .zC = false, .zEL = false, .zLM = false, .zR = false, .zSL = false, .zW = false, .zX = false };
  return zcbz3112;
}



static void finish_sailgen_getArchPermsLegalizzed(void)
{
}

static enum zExecutionMode sailgen_getCapMode(struct zCapability);

static enum zExecutionMode sailgen_getCapMode(struct zCapability zcap)
{
  __label__ case_592, case_591, finish_match_590, end_function_593, end_block_exception_594;

  enum zExecutionMode zcbz373;
  struct zoptionzIz8RArchPermszCEExecutionModez5z9zK zgaz3441;
  CREATE(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zgaz3441);
  sailgen_getCapPermsAndMode(&zgaz3441, zcap);
  {
    if (zgaz3441.kind != Kind_zNonezIz8RArchPermszCEExecutionModez5z9zK) goto case_592;
    zcbz373 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    goto finish_match_590;
  }
case_592: ;
  {
    /* complete */
    enum zExecutionMode zmode;
    zmode = zgaz3441.variants.zSomezIz8RArchPermszCEExecutionModez5z9zK.ztup1;
    zcbz373 = zmode;
    goto finish_match_590;
  }
case_591: ;
finish_match_590: ;
  KILL(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zgaz3441);
end_function_593: ;
  return zcbz373;
end_block_exception_594: ;

  return zCapPtrMode;
}

static int64_t sailgen_getCapLength(struct zCapability);

sail_int zghz3407;
sail_int zghz3408;
sail_int zghz3409;
sail_int zghz3410;
sail_int zghz3411;

static void startup_sailgen_getCapLength(void)
{
  CREATE(sail_int)(&zghz3407);
  CREATE(sail_int)(&zghz3408);
  CREATE(sail_int)(&zghz3409);
  CREATE(sail_int)(&zghz3410);
  CREATE(sail_int)(&zghz3411);
}

static int64_t sailgen_getCapLength(struct zCapability zc)
{
  __label__ case_598, case_597, finish_match_596, end_function_599, end_block_exception_600;

  int64_t zcbz374;
  struct zoptionzIz8izCiz9zK zgaz3442;
  CREATE(zoptionzIz8izCiz9zK)(&zgaz3442);
  sailgen_getCapBounds(&zgaz3442, zc);
  {
    if (zgaz3442.kind != Kind_zNonezIz8izCiz9zK) goto case_598;
    zcbz374 = INT64_C(0);
    goto finish_match_596;
  }
case_598: ;
  {
    /* complete */
    int64_t zbase;
    zbase = CONVERT_OF(mach_int, sail_int)(zgaz3442.variants.zSomezIz8izCiz9zK.ztup0);
    int64_t ztop;
    ztop = CONVERT_OF(mach_int, sail_int)(zgaz3442.variants.zSomezIz8izCiz9zK.ztup1);
    {
      bool zgaz3443;
      {
        RECREATE(sail_int)(&zghz3410);
        CONVERT_OF(sail_int, mach_int)(&zghz3410, ztop);
        RECREATE(sail_int)(&zghz3411);
        CONVERT_OF(sail_int, mach_int)(&zghz3411, zbase);
        zgaz3443 = gteq(zghz3410, zghz3411);
      }
      unit zgsz3748;
      zgsz3748 = sail_assert(zgaz3443, "src/cheri_cap_common.sail:448.42-448.43");
    }
    {
      RECREATE(sail_int)(&zghz3407);
      CONVERT_OF(sail_int, mach_int)(&zghz3407, ztop);
      RECREATE(sail_int)(&zghz3408);
      CONVERT_OF(sail_int, mach_int)(&zghz3408, zbase);
      RECREATE(sail_int)(&zghz3409);
      sub_int(&zghz3409, zghz3407, zghz3408);
      zcbz374 = CONVERT_OF(mach_int, sail_int)(zghz3409);
    }
    goto finish_match_596;
  }
case_597: ;
finish_match_596: ;
  KILL(zoptionzIz8izCiz9zK)(&zgaz3442);
end_function_599: ;
  return zcbz374;
end_block_exception_600: ;

  return INT64_C(0xdeadc0de);
}



static void finish_sailgen_getCapLength(void)
{
  KILL(sail_int)(&zghz3411);
  KILL(sail_int)(&zghz3410);
  KILL(sail_int)(&zghz3409);
  KILL(sail_int)(&zghz3408);
  KILL(sail_int)(&zghz3407);
}

static bool sailgen_inCapBoundsNoWrap(struct zCapability, uint64_t, int64_t);

sail_int zghz3412;
sail_int zghz3413;
sail_int zghz3414;
sail_int zghz3415;
lbits zghz3416;
sail_int zghz3417;

static void startup_sailgen_inCapBoundsNoWrap(void)
{
  CREATE(sail_int)(&zghz3412);
  CREATE(sail_int)(&zghz3413);
  CREATE(sail_int)(&zghz3414);
  CREATE(sail_int)(&zghz3415);
  CREATE(lbits)(&zghz3416);
  CREATE(sail_int)(&zghz3417);
}

static bool sailgen_inCapBoundsNoWrap(struct zCapability zcap, uint64_t zaddr, int64_t zsizze)
{
  __label__ case_604, case_603, finish_match_602, end_function_605, end_block_exception_606;

  bool zcbz375;
  int64_t za;
  {
    RECREATE(lbits)(&zghz3416);
    CONVERT_OF(lbits, fbits)(&zghz3416, zaddr, UINT64_C(32) , true);
    RECREATE(sail_int)(&zghz3417);
    sail_unsigned(&zghz3417, zghz3416);
    za = CONVERT_OF(mach_int, sail_int)(zghz3417);
  }
  struct zoptionzIz8izCiz9zK zgaz3444;
  CREATE(zoptionzIz8izCiz9zK)(&zgaz3444);
  sailgen_getCapBounds(&zgaz3444, zcap);
  {
    if (zgaz3444.kind != Kind_zNonezIz8izCiz9zK) goto case_604;
    zcbz375 = false;
    goto finish_match_602;
  }
case_604: ;
  {
    /* complete */
    int64_t zbase;
    zbase = CONVERT_OF(mach_int, sail_int)(zgaz3444.variants.zSomezIz8izCiz9zK.ztup0);
    int64_t ztop;
    ztop = CONVERT_OF(mach_int, sail_int)(zgaz3444.variants.zSomezIz8izCiz9zK.ztup1);
    bool zgaz3446;
    {
      RECREATE(sail_int)(&zghz3414);
      CONVERT_OF(sail_int, mach_int)(&zghz3414, za);
      RECREATE(sail_int)(&zghz3415);
      CONVERT_OF(sail_int, mach_int)(&zghz3415, zbase);
      zgaz3446 = gteq(zghz3414, zghz3415);
    }
    bool zgsz3752;
    if (zgaz3446) {
      int64_t zgaz3445;
      zgaz3445 = (za + zsizze);
      {
        RECREATE(sail_int)(&zghz3412);
        CONVERT_OF(sail_int, mach_int)(&zghz3412, zgaz3445);
        RECREATE(sail_int)(&zghz3413);
        CONVERT_OF(sail_int, mach_int)(&zghz3413, ztop);
        zgsz3752 = lteq(zghz3412, zghz3413);
      }
    } else {  zgsz3752 = false;  }
    zcbz375 = zgsz3752;
    goto finish_match_602;
  }
case_603: ;
finish_match_602: ;
  KILL(zoptionzIz8izCiz9zK)(&zgaz3444);
end_function_605: ;
  return zcbz375;
end_block_exception_606: ;

  return false;
}



static void finish_sailgen_inCapBoundsNoWrap(void)
{
  KILL(sail_int)(&zghz3417);
  KILL(lbits)(&zghz3416);
  KILL(sail_int)(&zghz3415);
  KILL(sail_int)(&zghz3414);
  KILL(sail_int)(&zghz3413);
  KILL(sail_int)(&zghz3412);
}

static bool sailgen_capReservedValid(struct zCapability);

static bool sailgen_capReservedValid(struct zCapability zcap)
{
  __label__ end_function_608, end_block_exception_609;

  bool zcbz376;
  bool zgaz3449;
  {
    uint64_t zgaz3447;
    zgaz3447 = zcap.zreserved_0;
    zgaz3449 = (zgaz3447 == UINT64_C(0b00));
  }
  bool zgsz3755;
  if (zgaz3449) {
    uint64_t zgaz3448;
    zgaz3448 = zcap.zreserved_1;
    zgsz3755 = (zgaz3448 == UINT64_C(0b0));
  } else {  zgsz3755 = false;  }
  zcbz376 = zgsz3755;
end_function_608: ;
  return zcbz376;
end_block_exception_609: ;

  return false;
}

static struct zCapability sailgen_clearTagIf(struct zCapability, bool);

struct zCapability zghz3418;

static void startup_sailgen_clearTagIf(void)
{
}

static struct zCapability sailgen_clearTagIf(struct zCapability zcap, bool zcond)
{
  __label__ end_function_611, end_block_exception_612;

  struct zCapability zghz3418;
  bool zgaz3451;
  {
    bool zgaz3450;
    zgaz3450 = zcap.ztag;
    bool zgsz3756;
    if (zgaz3450) {  zgsz3756 = sailgen_not(zcond);  } else {  zgsz3756 = false;  }
    zgaz3451 = zgsz3756;
  }
  zghz3418 = zcap;
  zghz3418.ztag = zgaz3451;
end_function_611: ;
  return zghz3418;
end_block_exception_612: ;
  struct zCapability zcbz3113 = { .zB = UINT64_C(0xdeadc0de), .zE = INT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zap_m = UINT64_C(0xdeadc0de), .zcl = UINT64_C(0xdeadc0de), .zinternal_exponent = false, .zreserved_0 = UINT64_C(0xdeadc0de), .zreserved_1 = UINT64_C(0xdeadc0de), .zsd_perms = UINT64_C(0xdeadc0de), .zsealed = false, .ztag = false };
  return zcbz3113;
}



static void finish_sailgen_clearTagIf(void)
{
}

static bool zsys_legalizze_malformed_permission_on_load;
static void create_letbind_26(void) {


  bool zgsz3758;
  zgsz3758 = false;
  zsys_legalizze_malformed_permission_on_load = zgsz3758;
let_end_613: ;
}
static void kill_letbind_26(void) {
}

static bool sailgen_capBoundsEqual(struct zCapability, struct zCapability);

sail_int zghz3419;
sail_int zghz3420;
sail_int zghz3421;
sail_int zghz3422;

static void startup_sailgen_capBoundsEqual(void)
{
  CREATE(sail_int)(&zghz3419);
  CREATE(sail_int)(&zghz3420);
  CREATE(sail_int)(&zghz3421);
  CREATE(sail_int)(&zghz3422);
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
  __label__ case_618, case_617, case_616, finish_match_615, end_function_619, end_block_exception_620;

  bool zcbz378;
  struct ztuple_z8z5unionz0zzoptionzzIzz8izzCizz9zzKzCz0z5unionz0zzoptionzzIzz8izzCizz9zzKz9 zgaz3454;
  CREATE(ztuple_z8z5unionz0zzoptionzzIzz8izzCizz9zzKzCz0z5unionz0zzoptionzzIzz8izzCizz9zzKz9)(&zgaz3454);
  {
    struct zoptionzIz8izCiz9zK zgaz3452;
    CREATE(zoptionzIz8izCiz9zK)(&zgaz3452);
    sailgen_getCapBounds(&zgaz3452, zc1);
    struct zoptionzIz8izCiz9zK zgaz3453;
    CREATE(zoptionzIz8izCiz9zK)(&zgaz3453);
    sailgen_getCapBounds(&zgaz3453, zc2);
    COPY(zoptionzIz8izCiz9zK)(&((&zgaz3454)->ztup0), zgaz3452);
    COPY(zoptionzIz8izCiz9zK)(&((&zgaz3454)->ztup1), zgaz3453);
    KILL(zoptionzIz8izCiz9zK)(&zgaz3453);
    KILL(zoptionzIz8izCiz9zK)(&zgaz3452);
  }
  {
    if (zgaz3454.ztup0.kind != Kind_zNonezIz8izCiz9zK) goto case_618;
    zcbz378 = false;
    goto finish_match_615;
  }
case_618: ;
  {
    if (zgaz3454.ztup1.kind != Kind_zNonezIz8izCiz9zK) goto case_617;
    zcbz378 = false;
    goto finish_match_615;
  }
case_617: ;
  {
    /* complete */
    /* complete */
    int64_t zbase1;
    zbase1 = CONVERT_OF(mach_int, sail_int)(zgaz3454.ztup0.variants.zSomezIz8izCiz9zK.ztup0);
    int64_t ztop1;
    ztop1 = CONVERT_OF(mach_int, sail_int)(zgaz3454.ztup0.variants.zSomezIz8izCiz9zK.ztup1);
    int64_t zbase2;
    zbase2 = CONVERT_OF(mach_int, sail_int)(zgaz3454.ztup1.variants.zSomezIz8izCiz9zK.ztup0);
    int64_t ztop2;
    ztop2 = CONVERT_OF(mach_int, sail_int)(zgaz3454.ztup1.variants.zSomezIz8izCiz9zK.ztup1);
    bool zgaz3455;
    {
      RECREATE(sail_int)(&zghz3421);
      CONVERT_OF(sail_int, mach_int)(&zghz3421, zbase1);
      RECREATE(sail_int)(&zghz3422);
      CONVERT_OF(sail_int, mach_int)(&zghz3422, zbase2);
      zgaz3455 = eq_int(zghz3421, zghz3422);
    }
    bool zgsz3761;
    if (zgaz3455) {
      {
        RECREATE(sail_int)(&zghz3419);
        CONVERT_OF(sail_int, mach_int)(&zghz3419, ztop1);
        RECREATE(sail_int)(&zghz3420);
        CONVERT_OF(sail_int, mach_int)(&zghz3420, ztop2);
        zgsz3761 = eq_int(zghz3419, zghz3420);
      }
    } else {  zgsz3761 = false;  }
    zcbz378 = zgsz3761;
    goto finish_match_615;
  }
case_616: ;
finish_match_615: ;
  KILL(ztuple_z8z5unionz0zzoptionzzIzz8izzCizz9zzKzCz0z5unionz0zzoptionzzIzz8izzCizz9zzKz9)(&zgaz3454);
end_function_619: ;
  return zcbz378;
end_block_exception_620: ;

  return false;
}



static void finish_sailgen_capBoundsEqual(void)
{
  KILL(sail_int)(&zghz3422);
  KILL(sail_int)(&zghz3421);
  KILL(sail_int)(&zghz3420);
  KILL(sail_int)(&zghz3419);
}

static struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 sailgen_setCapAddr(struct zCapability, uint64_t);

struct zCapability zghz3423;

static void startup_sailgen_setCapAddr(void)
{
}

static struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 sailgen_setCapAddr(struct zCapability zc, uint64_t zaddr)
{
  __label__ end_function_622, end_block_exception_623;

  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz379;
  struct zCapability zghz3423;
  {
    zghz3423 = zc;
    zghz3423.zaddress = zaddr;
  }
  bool zrepresentable;
  zrepresentable = sailgen_capBoundsEqual(zc, zghz3423);
  zcbz379.ztup0 = zrepresentable;
  zcbz379.ztup1 = zghz3423;
end_function_622: ;
  return zcbz379;
end_block_exception_623: ;
  struct zCapability zcbz3115 = { .zB = UINT64_C(0xdeadc0de), .zE = INT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zap_m = UINT64_C(0xdeadc0de), .zcl = UINT64_C(0xdeadc0de), .zinternal_exponent = false, .zreserved_0 = UINT64_C(0xdeadc0de), .zreserved_1 = UINT64_C(0xdeadc0de), .zsd_perms = UINT64_C(0xdeadc0de), .zsealed = false, .ztag = false };
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz3114 = { .ztup0 = false, .ztup1 = zcbz3115 };
  return zcbz3114;
}



static void finish_sailgen_setCapAddr(void)
{
}

static void sailgen_capToString(sail_string *rop, struct zCapability);

lbits zghz3424;
lbits zghz3425;
lbits zghz3426;
lbits zghz3427;
struct zEncMetadata zghz3428;
struct zArchPerms zghz3429;
lbits zghz3430;
sail_int zghz3431;
sail_int zghz3432;
lbits zghz3433;
lbits zghz3434;
sail_int zghz3435;
lbits zghz3436;
lbits zghz3437;
lbits zghz3438;

static void startup_sailgen_capToString(void)
{
  CREATE(lbits)(&zghz3424);
  CREATE(lbits)(&zghz3425);
  CREATE(lbits)(&zghz3426);
  CREATE(lbits)(&zghz3427);


  CREATE(lbits)(&zghz3430);
  CREATE(sail_int)(&zghz3431);
  CREATE(sail_int)(&zghz3432);
  CREATE(lbits)(&zghz3433);
  CREATE(lbits)(&zghz3434);
  CREATE(sail_int)(&zghz3435);
  CREATE(lbits)(&zghz3436);
  CREATE(lbits)(&zghz3437);
  CREATE(lbits)(&zghz3438);
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

static void sailgen_capToString(sail_string *zcbz380, struct zCapability zcap)
{
  __label__ case_629, finish_match_628, end_function_640, end_block_exception_641, end_function_667;

  struct ztuple_z8z5stringzCz0z5stringzCz0z5boolz9 zgaz3461;
  CREATE(ztuple_z8z5stringzCz0z5stringzCz0z5boolz9)(&zgaz3461);
  {
    __label__ case_627, case_626, finish_match_625;

    struct zoptionzIz8bzCbz9zK zgaz3456;
    CREATE(zoptionzIz8bzCbz9zK)(&zgaz3456);
    sailgen_getCapBoundsBits(&zgaz3456, zcap);
    {
      if (zgaz3456.kind != Kind_zNonezIz8bzCbz9zK) goto case_627;
      struct ztuple_z8z5stringzCz0z5stringzCz0z5boolz9 zgsz3770;
      CREATE(ztuple_z8z5stringzCz0z5stringzCz0z5boolz9)(&zgsz3770);
      COPY(sail_string)(&((&zgsz3770)->ztup0), "0 (malformed)");
      COPY(sail_string)(&((&zgsz3770)->ztup1), "0 (malformed)");
      zgsz3770.ztup2 = true;
      COPY(ztuple_z8z5stringzCz0z5stringzCz0z5boolz9)(&zgaz3461, zgsz3770);
      KILL(ztuple_z8z5stringzCz0z5stringzCz0z5boolz9)(&zgsz3770);
      goto finish_match_625;
    }
  case_627: ;
    {
      /* complete */
      uint64_t zbase;
      zbase = CONVERT_OF(fbits, lbits)(zgaz3456.variants.zSomezIz8bzCbz9zK.ztup0, true);
      uint64_t ztop;
      ztop = CONVERT_OF(fbits, lbits)(zgaz3456.variants.zSomezIz8bzCbz9zK.ztup1, true);
      sail_string zgaz3459;
      CREATE(sail_string)(&zgaz3459);
      {
        RECREATE(lbits)(&zghz3438);
        CONVERT_OF(lbits, fbits)(&zghz3438, zbase, UINT64_C(32) , true);
        string_of_lbits(&zgaz3459, zghz3438);
      }
      sail_string zgaz3460;
      CREATE(sail_string)(&zgaz3460);
      {
        uint64_t zgaz3458;
        {
          int64_t zgaz3457;
          zgaz3457 = (zcap_len_width + INT64_C(3));
          {
            RECREATE(sail_int)(&zghz3435);
            CONVERT_OF(sail_int, mach_int)(&zghz3435, zgaz3457);
            RECREATE(lbits)(&zghz3436);
            CONVERT_OF(lbits, fbits)(&zghz3436, ztop, UINT64_C(33) , true);
            RECREATE(lbits)(&zghz3437);
            sailgen_zzero_extend(&zghz3437, zghz3435, zghz3436);
            zgaz3458 = CONVERT_OF(fbits, lbits)(zghz3437, true);
          }
        }
        {
          RECREATE(lbits)(&zghz3434);
          CONVERT_OF(lbits, fbits)(&zghz3434, zgaz3458, UINT64_C(36) , true);
          string_of_lbits(&zgaz3460, zghz3434);
        }
      }
      struct ztuple_z8z5stringzCz0z5stringzCz0z5boolz9 zgsz3768;
      CREATE(ztuple_z8z5stringzCz0z5stringzCz0z5boolz9)(&zgsz3768);
      COPY(sail_string)(&((&zgsz3768)->ztup0), zgaz3459);
      COPY(sail_string)(&((&zgsz3768)->ztup1), zgaz3460);
      zgsz3768.ztup2 = false;
      COPY(ztuple_z8z5stringzCz0z5stringzCz0z5boolz9)(&zgaz3461, zgsz3768);
      KILL(ztuple_z8z5stringzCz0z5stringzCz0z5boolz9)(&zgsz3768);
      KILL(sail_string)(&zgaz3460);
      KILL(sail_string)(&zgaz3459);
      goto finish_match_625;
    }
  case_626: ;
  finish_match_625: ;
    KILL(zoptionzIz8bzCbz9zK)(&zgaz3456);
  }
  sail_string zgsz3772;
  CREATE(sail_string)(&zgsz3772);
  {
    __label__ case_634, finish_match_633;

    sail_string zbase_str;
    CREATE(sail_string)(&zbase_str);
    COPY(sail_string)(&zbase_str, zgaz3461.ztup0);
    sail_string ztop_str;
    CREATE(sail_string)(&ztop_str);
    COPY(sail_string)(&ztop_str, zgaz3461.ztup1);
    bool zbounds_malformed;
    zbounds_malformed = zgaz3461.ztup2;
    sail_string zlen_str;
    CREATE(sail_string)(&zlen_str);
    {
      uint64_t zgaz3527;
      {
        int64_t zgaz3525;
        zgaz3525 = (zcap_len_width + INT64_C(3));
        int64_t zgaz3526;
        zgaz3526 = sailgen_getCapLength(zcap);
        {
          RECREATE(sail_int)(&zghz3431);
          CONVERT_OF(sail_int, mach_int)(&zghz3431, zgaz3525);
          RECREATE(sail_int)(&zghz3432);
          CONVERT_OF(sail_int, mach_int)(&zghz3432, zgaz3526);
          RECREATE(lbits)(&zghz3433);
          sailgen_to_bits(&zghz3433, zghz3431, zghz3432);
          zgaz3527 = CONVERT_OF(fbits, lbits)(zghz3433, true);
        }
      }
      {
        RECREATE(lbits)(&zghz3430);
        CONVERT_OF(lbits, fbits)(&zghz3430, zgaz3527, UINT64_C(36) , true);
        string_of_lbits(&zlen_str, zghz3430);
      }
    }
    struct ztuple_z8z5stringzCz0z5boolz9 zgaz3486;
    CREATE(ztuple_z8z5stringzCz0z5boolz9)(&zgaz3486);
    {
      __label__ case_632, case_631, finish_match_630;

      struct zoptionzIRArchPermszK zgaz3462;
      CREATE(zoptionzIRArchPermszK)(&zgaz3462);
      sailgen_getArchPerms(&zgaz3462, zcap);
      {
        if (zgaz3462.kind != Kind_zNonezIRArchPermszK) goto case_632;
        struct ztuple_z8z5stringzCz0z5boolz9 zgsz3776;
        CREATE(ztuple_z8z5stringzCz0z5boolz9)(&zgsz3776);
        COPY(sail_string)(&((&zgsz3776)->ztup0), "(malformed)");
        zgsz3776.ztup1 = true;
        COPY(ztuple_z8z5stringzCz0z5boolz9)(&zgaz3486, zgsz3776);
        KILL(ztuple_z8z5stringzCz0z5boolz9)(&zgsz3776);
        goto finish_match_630;
      }
    case_632: ;
      {
        /* complete */
        struct zArchPerms zghz3429;
        zghz3429 = zgaz3462.variants.zSomezIRArchPermszK;
        sail_string zgaz3485;
        CREATE(sail_string)(&zgaz3485);
        {
          sail_string zgaz3483;
          CREATE(sail_string)(&zgaz3483);
          {
            bool zgaz3463;
            zgaz3463 = zghz3429.zR;
            if (zgaz3463) {  COPY(sail_string)(&zgaz3483, "R");  } else {  COPY(sail_string)(&zgaz3483, "");  }
          }
          sail_string zgaz3484;
          CREATE(sail_string)(&zgaz3484);
          {
            sail_string zgaz3481;
            CREATE(sail_string)(&zgaz3481);
            {
              bool zgaz3464;
              zgaz3464 = zghz3429.zW;
              if (zgaz3464) {  COPY(sail_string)(&zgaz3481, "W");  } else {  COPY(sail_string)(&zgaz3481, "");  }
            }
            sail_string zgaz3482;
            CREATE(sail_string)(&zgaz3482);
            {
              sail_string zgaz3479;
              CREATE(sail_string)(&zgaz3479);
              {
                bool zgaz3465;
                zgaz3465 = zghz3429.zC;
                if (zgaz3465) {  COPY(sail_string)(&zgaz3479, "C");  } else {  COPY(sail_string)(&zgaz3479, "");  }
              }
              sail_string zgaz3480;
              CREATE(sail_string)(&zgaz3480);
              {
                sail_string zgaz3477;
                CREATE(sail_string)(&zgaz3477);
                {
                  bool zgaz3466;
                  zgaz3466 = zghz3429.zX;
                  if (zgaz3466) {  COPY(sail_string)(&zgaz3477, "X");  } else {  COPY(sail_string)(&zgaz3477, "");  }
                }
                sail_string zgaz3478;
                CREATE(sail_string)(&zgaz3478);
                {
                  sail_string zgaz3475;
                  CREATE(sail_string)(&zgaz3475);
                  {
                    bool zgaz3467;
                    zgaz3467 = zghz3429.zLM;
                    if (zgaz3467) {  COPY(sail_string)(&zgaz3475, "Lm");  } else {  COPY(sail_string)(&zgaz3475, "");  }
                  }
                  sail_string zgaz3476;
                  CREATE(sail_string)(&zgaz3476);
                  {
                    sail_string zgaz3473;
                    CREATE(sail_string)(&zgaz3473);
                    {
                      bool zgaz3468;
                      zgaz3468 = zghz3429.zASR;
                      if (zgaz3468) {  COPY(sail_string)(&zgaz3473, "Asr");  } else {
                        COPY(sail_string)(&zgaz3473, "");
                      }
                    }
                    sail_string zgaz3474;
                    CREATE(sail_string)(&zgaz3474);
                    {
                      sail_string zgaz3471;
                      CREATE(sail_string)(&zgaz3471);
                      {
                        bool zgaz3469;
                        zgaz3469 = zghz3429.zSL;
                        if (zgaz3469) {  COPY(sail_string)(&zgaz3471, "Sl");  } else {
                          COPY(sail_string)(&zgaz3471, "");
                        }
                      }
                      sail_string zgaz3472;
                      CREATE(sail_string)(&zgaz3472);
                      {
                        bool zgaz3470;
                        zgaz3470 = zghz3429.zEL;
                        if (zgaz3470) {  COPY(sail_string)(&zgaz3472, "El");  } else {
                          COPY(sail_string)(&zgaz3472, "");
                        }
                      }
                      concat_str(&zgaz3474, zgaz3471, zgaz3472);
                      KILL(sail_string)(&zgaz3472);
                      KILL(sail_string)(&zgaz3471);
                    }
                    concat_str(&zgaz3476, zgaz3473, zgaz3474);
                    KILL(sail_string)(&zgaz3474);
                    KILL(sail_string)(&zgaz3473);
                  }
                  concat_str(&zgaz3478, zgaz3475, zgaz3476);
                  KILL(sail_string)(&zgaz3476);
                  KILL(sail_string)(&zgaz3475);
                }
                concat_str(&zgaz3480, zgaz3477, zgaz3478);
                KILL(sail_string)(&zgaz3478);
                KILL(sail_string)(&zgaz3477);
              }
              concat_str(&zgaz3482, zgaz3479, zgaz3480);
              KILL(sail_string)(&zgaz3480);
              KILL(sail_string)(&zgaz3479);
            }
            concat_str(&zgaz3484, zgaz3481, zgaz3482);
            KILL(sail_string)(&zgaz3482);
            KILL(sail_string)(&zgaz3481);
          }
          concat_str(&zgaz3485, zgaz3483, zgaz3484);
          KILL(sail_string)(&zgaz3484);
          KILL(sail_string)(&zgaz3483);
        }
        struct ztuple_z8z5stringzCz0z5boolz9 zgsz3774;
        CREATE(ztuple_z8z5stringzCz0z5boolz9)(&zgsz3774);
        COPY(sail_string)(&((&zgsz3774)->ztup0), zgaz3485);
        zgsz3774.ztup1 = false;
        COPY(ztuple_z8z5stringzCz0z5boolz9)(&zgaz3486, zgsz3774);
        KILL(ztuple_z8z5stringzCz0z5boolz9)(&zgsz3774);
        KILL(sail_string)(&zgaz3485);
        goto finish_match_630;
      }
    case_631: ;
    finish_match_630: ;
      KILL(zoptionzIRArchPermszK)(&zgaz3462);
    }
    sail_string zgsz3778;
    CREATE(sail_string)(&zgsz3778);
    {
      __label__ case_636, finish_match_635;

      sail_string zarch_perms_str;
      CREATE(sail_string)(&zarch_perms_str);
      COPY(sail_string)(&zarch_perms_str, zgaz3486.ztup0);
      bool zperms_malformed;
      zperms_malformed = zgaz3486.ztup1;
      struct ztuple_z8z5stringzCz0z5boolz9 zgaz3489;
      CREATE(ztuple_z8z5stringzCz0z5boolz9)(&zgaz3489);
      {
        bool zgaz3488;
        {
          bool zgaz3487;
          zgaz3487 = sailgen_capReservedValid(zcap);
          zgaz3488 = sailgen_not(zgaz3487);
        }
        if (zgaz3488) {
          struct ztuple_z8z5stringzCz0z5boolz9 zgsz3780;
          CREATE(ztuple_z8z5stringzCz0z5boolz9)(&zgsz3780);
          COPY(sail_string)(&((&zgsz3780)->ztup0), " (has non-zero reserved bits)");
          zgsz3780.ztup1 = true;
          COPY(ztuple_z8z5stringzCz0z5boolz9)(&zgaz3489, zgsz3780);
          KILL(ztuple_z8z5stringzCz0z5boolz9)(&zgsz3780);
        } else {
          struct ztuple_z8z5stringzCz0z5boolz9 zgsz3779;
          CREATE(ztuple_z8z5stringzCz0z5boolz9)(&zgsz3779);
          COPY(sail_string)(&((&zgsz3779)->ztup0), "");
          zgsz3779.ztup1 = false;
          COPY(ztuple_z8z5stringzCz0z5boolz9)(&zgaz3489, zgsz3779);
          KILL(ztuple_z8z5stringzCz0z5boolz9)(&zgsz3779);
        }
      }
      sail_string zgsz3781;
      CREATE(sail_string)(&zgsz3781);
      {
        sail_string zres_str;
        CREATE(sail_string)(&zres_str);
        COPY(sail_string)(&zres_str, zgaz3489.ztup0);
        bool zreserved_malformed;
        zreserved_malformed = zgaz3489.ztup1;
        sail_string zraw_str;
        CREATE(sail_string)(&zraw_str);
        {
          bool zgaz3521;
          {
            if (zbounds_malformed) {  zgaz3521 = true;  } else {
              bool zgsz3782;
              if (zperms_malformed) {  zgsz3782 = true;  } else {  zgsz3782 = zreserved_malformed;  }
              zgaz3521 = zgsz3782;
            }
          }
          if (zgaz3521) {
            sail_string zgaz3524;
            CREATE(sail_string)(&zgaz3524);
            {
              uint64_t zgaz3523;
              {
                struct zEncMetadata zghz3428;
                zghz3428 = sailgen_capToMetadataBits(zcap);
                zgaz3523 = zghz3428.zbits;
              }
              {
                RECREATE(lbits)(&zghz3427);
                CONVERT_OF(lbits, fbits)(&zghz3427, zgaz3523, UINT64_C(32) , true);
                string_of_lbits(&zgaz3524, zghz3427);
              }
            }
            concat_str(&zraw_str, " raw:", zgaz3524);
            KILL(sail_string)(&zgaz3524);
          } else {  COPY(sail_string)(&zraw_str, "");  }
        }
        sail_string zcheri_mode;
        CREATE(sail_string)(&zcheri_mode);
        {
          __label__ case_639, case_638, finish_match_637;

          enum zExecutionMode zgaz3520;
          zgaz3520 = sailgen_getCapMode(zcap);
          {
            if ((zCapPtrMode != zgaz3520)) goto case_639;
            COPY(sail_string)(&zcheri_mode, "CapPtrMode");
            goto finish_match_637;
          }
        case_639: ;
          {
            /* complete */
            COPY(sail_string)(&zcheri_mode, "Integer");
            goto finish_match_637;
          }
        case_638: ;
        finish_match_637: ;
        }
        sail_string zgaz3519;
        CREATE(sail_string)(&zgaz3519);
        {
          sail_string zgaz3517;
          CREATE(sail_string)(&zgaz3517);
          {
            bool zgaz3490;
            zgaz3490 = zcap.ztag;
            if (zgaz3490) {  COPY(sail_string)(&zgaz3517, "1");  } else {  COPY(sail_string)(&zgaz3517, "0");  }
          }
          sail_string zgaz3518;
          CREATE(sail_string)(&zgaz3518);
          {
            sail_string zgaz3516;
            CREATE(sail_string)(&zgaz3516);
            {
              sail_string zgaz3514;
              CREATE(sail_string)(&zgaz3514);
              {
                uint64_t zgaz3491;
                zgaz3491 = zcap.zaddress;
                {
                  RECREATE(lbits)(&zghz3426);
                  CONVERT_OF(lbits, fbits)(&zghz3426, zgaz3491, UINT64_C(32) , true);
                  string_of_lbits(&zgaz3514, zghz3426);
                }
              }
              sail_string zgaz3515;
              CREATE(sail_string)(&zgaz3515);
              {
                sail_string zgaz3513;
                CREATE(sail_string)(&zgaz3513);
                {
                  sail_string zgaz3512;
                  CREATE(sail_string)(&zgaz3512);
                  {
                    sail_string zgaz3511;
                    CREATE(sail_string)(&zgaz3511);
                    {
                      sail_string zgaz3510;
                      CREATE(sail_string)(&zgaz3510);
                      {
                        sail_string zgaz3509;
                        CREATE(sail_string)(&zgaz3509);
                        {
                          sail_string zgaz3508;
                          CREATE(sail_string)(&zgaz3508);
                          {
                            sail_string zgaz3507;
                            CREATE(sail_string)(&zgaz3507);
                            {
                              sail_string zgaz3505;
                              CREATE(sail_string)(&zgaz3505);
                              {
                                bool zgaz3492;
                                zgaz3492 = zcap.zsealed;
                                if (zgaz3492) {  COPY(sail_string)(&zgaz3505, "1");  } else {
                                  COPY(sail_string)(&zgaz3505, "0");
                                }
                              }
                              sail_string zgaz3506;
                              CREATE(sail_string)(&zgaz3506);
                              {
                                sail_string zgaz3504;
                                CREATE(sail_string)(&zgaz3504);
                                {
                                  sail_string zgaz3503;
                                  CREATE(sail_string)(&zgaz3503);
                                  {
                                    sail_string zgaz3502;
                                    CREATE(sail_string)(&zgaz3502);
                                    {
                                      sail_string zgaz3500;
                                      CREATE(sail_string)(&zgaz3500);
                                      {
                                        uint64_t zgaz3493;
                                        zgaz3493 = zcap.zsd_perms;
                                        {
                                          RECREATE(lbits)(&zghz3425);
                                          CONVERT_OF(lbits, fbits)(&zghz3425, zgaz3493, UINT64_C(2) , true);
                                          string_of_lbits(&zgaz3500, zghz3425);
                                        }
                                      }
                                      sail_string zgaz3501;
                                      CREATE(sail_string)(&zgaz3501);
                                      {
                                        sail_string zgaz3499;
                                        CREATE(sail_string)(&zgaz3499);
                                        {
                                          sail_string zgaz3497;
                                          CREATE(sail_string)(&zgaz3497);
                                          {
                                            uint64_t zgaz3494;
                                            zgaz3494 = zcap.zcl;
                                            {
                                              RECREATE(lbits)(&zghz3424);
                                              CONVERT_OF(lbits, fbits)(&zghz3424, zgaz3494, UINT64_C(1) , true);
                                              string_of_lbits(&zgaz3497, zghz3424);
                                            }
                                          }
                                          sail_string zgaz3498;
                                          CREATE(sail_string)(&zgaz3498);
                                          {
                                            sail_string zgaz3496;
                                            CREATE(sail_string)(&zgaz3496);
                                            {
                                              sail_string zgaz3495;
                                              CREATE(sail_string)(&zgaz3495);
                                              concat_str(&zgaz3495, zraw_str, zres_str);
                                              concat_str(&zgaz3496, zcheri_mode, zgaz3495);
                                              KILL(sail_string)(&zgaz3495);
                                            }
                                            concat_str(&zgaz3498, " mode:", zgaz3496);
                                            KILL(sail_string)(&zgaz3496);
                                          }
                                          concat_str(&zgaz3499, zgaz3497, zgaz3498);
                                          KILL(sail_string)(&zgaz3498);
                                          KILL(sail_string)(&zgaz3497);
                                        }
                                        concat_str(&zgaz3501, " level:", zgaz3499);
                                        KILL(sail_string)(&zgaz3499);
                                      }
                                      concat_str(&zgaz3502, zgaz3500, zgaz3501);
                                      KILL(sail_string)(&zgaz3501);
                                      KILL(sail_string)(&zgaz3500);
                                    }
                                    concat_str(&zgaz3503, " sdperms:", zgaz3502);
                                    KILL(sail_string)(&zgaz3502);
                                  }
                                  concat_str(&zgaz3504, zarch_perms_str, zgaz3503);
                                  KILL(sail_string)(&zgaz3503);
                                }
                                concat_str(&zgaz3506, " perms:", zgaz3504);
                                KILL(sail_string)(&zgaz3504);
                              }
                              concat_str(&zgaz3507, zgaz3505, zgaz3506);
                              KILL(sail_string)(&zgaz3506);
                              KILL(sail_string)(&zgaz3505);
                            }
                            concat_str(&zgaz3508, " sealed:", zgaz3507);
                            KILL(sail_string)(&zgaz3507);
                          }
                          concat_str(&zgaz3509, zlen_str, zgaz3508);
                          KILL(sail_string)(&zgaz3508);
                        }
                        concat_str(&zgaz3510, " length:", zgaz3509);
                        KILL(sail_string)(&zgaz3509);
                      }
                      concat_str(&zgaz3511, ztop_str, zgaz3510);
                      KILL(sail_string)(&zgaz3510);
                    }
                    concat_str(&zgaz3512, " top:", zgaz3511);
                    KILL(sail_string)(&zgaz3511);
                  }
                  concat_str(&zgaz3513, zbase_str, zgaz3512);
                  KILL(sail_string)(&zgaz3512);
                }
                concat_str(&zgaz3515, " base:", zgaz3513);
                KILL(sail_string)(&zgaz3513);
              }
              concat_str(&zgaz3516, zgaz3514, zgaz3515);
              KILL(sail_string)(&zgaz3515);
              KILL(sail_string)(&zgaz3514);
            }
            concat_str(&zgaz3518, " address:", zgaz3516);
            KILL(sail_string)(&zgaz3516);
          }
          concat_str(&zgaz3519, zgaz3517, zgaz3518);
          KILL(sail_string)(&zgaz3518);
          KILL(sail_string)(&zgaz3517);
        }
        concat_str(&zgsz3781, "t:", zgaz3519);
        KILL(sail_string)(&zgaz3519);
        KILL(sail_string)(&zcheri_mode);
        KILL(sail_string)(&zraw_str);
        KILL(sail_string)(&zres_str);
        goto finish_match_635;
      }
    case_636: ;
      sail_match_failure("capToString");
    finish_match_635: ;
      COPY(sail_string)(&zgsz3778, zgsz3781);
      KILL(sail_string)(&zgsz3781);
      KILL(ztuple_z8z5stringzCz0z5boolz9)(&zgaz3489);
      KILL(sail_string)(&zarch_perms_str);
      goto finish_match_633;
    }
  case_634: ;
    sail_match_failure("capToString");
  finish_match_633: ;
    COPY(sail_string)(&zgsz3772, zgsz3778);
    KILL(sail_string)(&zgsz3778);
    KILL(ztuple_z8z5stringzCz0z5boolz9)(&zgaz3486);
    KILL(sail_string)(&zlen_str);
    KILL(sail_string)(&ztop_str);
    KILL(sail_string)(&zbase_str);
    goto finish_match_628;
  }
case_629: ;
  sail_match_failure("capToString");
finish_match_628: ;
  COPY(sail_string)((*(&zcbz380)), zgsz3772);
  KILL(sail_string)(&zgsz3772);
  KILL(ztuple_z8z5stringzCz0z5stringzCz0z5boolz9)(&zgaz3461);
end_function_640: ;
  goto end_function_667;
end_block_exception_641: ;
  goto end_function_667;
end_function_667: ;
}



static void finish_sailgen_capToString(void)
{
  KILL(lbits)(&zghz3438);
  KILL(lbits)(&zghz3437);
  KILL(lbits)(&zghz3436);
  KILL(sail_int)(&zghz3435);
  KILL(lbits)(&zghz3434);
  KILL(lbits)(&zghz3433);
  KILL(sail_int)(&zghz3432);
  KILL(sail_int)(&zghz3431);
  KILL(lbits)(&zghz3430);
  KILL(lbits)(&zghz3427);
  KILL(lbits)(&zghz3426);
  KILL(lbits)(&zghz3425);
  KILL(lbits)(&zghz3424);
}

static uint64_t sailgen_getRepresentableAlignmentMask(uint64_t);

struct zCapability zghz3439;
sail_int zghz3440;
lbits zghz3441;
lbits zghz3442;
sail_int zghz3443;
lbits zghz3444;

static void startup_sailgen_getRepresentableAlignmentMask(void)
{

  CREATE(sail_int)(&zghz3440);
  CREATE(lbits)(&zghz3441);
  CREATE(lbits)(&zghz3442);
  CREATE(sail_int)(&zghz3443);
  CREATE(lbits)(&zghz3444);
}

static uint64_t sailgen_getRepresentableAlignmentMask(uint64_t zlen)
{
  __label__ case_644, finish_match_643, end_function_645, end_block_exception_646;

  uint64_t zcbz381;
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3529;
  {
    uint64_t zgaz3528;
    zgaz3528 = (UINT64_C(0b0) << 32) | zlen;
    zgaz3529 = sailgen_setCapBounds(zinfinite_cap, UINT64_C(0x00000000), zgaz3528);
  }
  {
    bool z_exact;
    z_exact = zgaz3529.ztup0;
    struct zCapability zghz3439;
    zghz3439 = zgaz3529.ztup1;
    bool zgaz3530;
    zgaz3530 = zghz3439.zinternal_exponent;
    if (zgaz3530) {
      int64_t ze;
      ze = zghz3439.zE;
      {
        bool zgaz3531;
        zgaz3531 = (ze >= INT64_C(0));
        unit zgsz3792;
        zgsz3792 = sail_assert(zgaz3531, "src/cheri_cap_common.sail:688.17-688.18");
      }
      uint64_t zgaz3534;
      {
        sbits zgaz3533;
        {
          int64_t zgaz3532;
          zgaz3532 = (ze + zcap_xe_width);
          {
            RECREATE(sail_int)(&zghz3443);
            CONVERT_OF(sail_int, mach_int)(&zghz3443, zgaz3532);
            RECREATE(lbits)(&zghz3444);
            sailgen_ones(&zghz3444, zghz3443);
            zgaz3533 = CONVERT_OF(sbits, lbits)(zghz3444, true);
          }
        }
        {
          RECREATE(sail_int)(&zghz3440);
          CONVERT_OF(sail_int, mach_int)(&zghz3440, INT64_C(32));
          RECREATE(lbits)(&zghz3441);
          CONVERT_OF(lbits, sbits)(&zghz3441, zgaz3533, true);
          RECREATE(lbits)(&zghz3442);
          sailgen_zzero_extend(&zghz3442, zghz3440, zghz3441);
          zgaz3534 = CONVERT_OF(fbits, lbits)(zghz3442, true);
        }
      }
      zcbz381 = (~(zgaz3534) & UINT64_C(0xFFFFFFFF));
    } else {  zcbz381 = UINT64_C(0xFFFFFFFF);  }
    goto finish_match_643;
  }
case_644: ;
  sail_match_failure("getRepresentableAlignmentMask");
finish_match_643: ;
end_function_645: ;
  return zcbz381;
end_block_exception_646: ;

  return UINT64_C(0xdeadc0de);
}



static void finish_sailgen_getRepresentableAlignmentMask(void)
{
  KILL(lbits)(&zghz3444);
  KILL(sail_int)(&zghz3443);
  KILL(lbits)(&zghz3442);
  KILL(lbits)(&zghz3441);
  KILL(sail_int)(&zghz3440);
}

static uint64_t sailgen_getRepresentableLength(uint64_t);

static uint64_t sailgen_getRepresentableLength(uint64_t zlen)
{
  __label__ end_function_648, end_block_exception_649;

  uint64_t zcbz382;
  uint64_t zm;
  zm = sailgen_getRepresentableAlignmentMask(zlen);
  uint64_t zgaz3536;
  {
    uint64_t zgaz3535;
    zgaz3535 = (~(zm) & UINT64_C(0xFFFFFFFF));
    zgaz3536 = ((zlen + zgaz3535) & UINT64_C(0xFFFFFFFF));
  }
  zcbz382 = (zgaz3536 & zm);
end_function_648: ;
  return zcbz382;
end_block_exception_649: ;

  return UINT64_C(0xdeadc0de);
}

static struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 sailgen_doCSetBounds(struct zCapability, uint64_t);

struct zCapability zghz3445;
struct zCapability zghz3446;
struct zCapability zghz3447;
lbits zghz3448;
sail_int zghz3449;
sail_int zghz3450;
lbits zghz3451;
lbits zghz3452;

static void startup_sailgen_doCSetBounds(void)
{



  CREATE(lbits)(&zghz3448);
  CREATE(sail_int)(&zghz3449);
  CREATE(sail_int)(&zghz3450);
  CREATE(lbits)(&zghz3451);
  CREATE(lbits)(&zghz3452);
}

static struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 sailgen_doCSetBounds(struct zCapability zinput, uint64_t zreq_len)
{
  __label__ case_652, finish_match_651, end_function_655, end_block_exception_656;

  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz383;
  uint64_t znewBase;
  znewBase = zinput.zaddress;
  uint64_t znewTop;
  {
    uint64_t zgaz3546;
    {
      RECREATE(sail_int)(&zghz3450);
      CONVERT_OF(sail_int, mach_int)(&zghz3450, INT64_C(33));
      RECREATE(lbits)(&zghz3451);
      CONVERT_OF(lbits, fbits)(&zghz3451, znewBase, UINT64_C(32) , true);
      RECREATE(lbits)(&zghz3452);
      sailgen_zzero_extend(&zghz3452, zghz3450, zghz3451);
      zgaz3546 = CONVERT_OF(fbits, lbits)(zghz3452, true);
    }
    znewTop = ((zgaz3546 + zreq_len) & UINT64_C(0b111111111111111111111111111111111));
  }
  bool zinBounds;
  {
    int64_t zgaz3545;
    {
      RECREATE(lbits)(&zghz3448);
      CONVERT_OF(lbits, fbits)(&zghz3448, zreq_len, UINT64_C(33) , true);
      RECREATE(sail_int)(&zghz3449);
      sail_unsigned(&zghz3449, zghz3448);
      zgaz3545 = CONVERT_OF(mach_int, sail_int)(zghz3449);
    }
    zinBounds = sailgen_inCapBoundsNoWrap(zinput, znewBase, zgaz3545);
  }
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3537;
  zgaz3537 = sailgen_setCapBounds(zinput, znewBase, znewTop);
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgsz3793;
  {
    __label__ case_654, finish_match_653;

    bool zexact;
    zexact = zgaz3537.ztup0;
    struct zCapability zghz3445;
    zghz3445 = zgaz3537.ztup1;
    bool zcond;
    {
      bool zgaz3544;
      {
        bool zgaz3540;
        {    if (zinBounds) {  zgaz3540 = zexact;  } else {  zgaz3540 = false;  }
        }
        zgaz3544 = sailgen_not(zgaz3540);
      }
      bool zgsz3797;
      if (zgaz3544) {  zgsz3797 = true;  } else {
        bool zgaz3543;
        zgaz3543 = sailgen_boundsMalformed(zghz3445);
        bool zgsz3796;
        if (zgaz3543) {  zgsz3796 = true;  } else {
          bool zgaz3542;
          {
            bool zgaz3541;
            zgaz3541 = sailgen_capReservedValid(zghz3445);
            zgaz3542 = sailgen_not(zgaz3541);
          }
          bool zgsz3795;
          if (zgaz3542) {  zgsz3795 = true;  } else {  zgsz3795 = sailgen_capIsSealed(zghz3445);  }
          zgsz3796 = zgsz3795;
        }
        zgsz3797 = zgsz3796;
      }
      zcond = zgsz3797;
    }
    struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3538;
    zgaz3538 = sailgen_setCapBounds(zinput, znewBase, znewTop);
    struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgsz3798;
    {
      bool zexactshadowz317;
      zexactshadowz317 = zgaz3538.ztup0;
      struct zCapability zghz3446;
      zghz3446 = zgaz3538.ztup1;
      struct zCapability zghz3447;
      zghz3447 = sailgen_clearTagIf(zghz3446, zcond);
      zgsz3798.ztup0 = zexactshadowz317;
      zgsz3798.ztup1 = zghz3447;
      goto finish_match_653;
    }
  case_654: ;
    sail_match_failure("doCSetBounds");
  finish_match_653: ;
    zgsz3793 = zgsz3798;
    goto finish_match_651;
  }
case_652: ;
  sail_match_failure("doCSetBounds");
finish_match_651: ;
  zcbz383 = zgsz3793;
end_function_655: ;
  return zcbz383;
end_block_exception_656: ;
  struct zCapability zcbz3117 = { .zB = UINT64_C(0xdeadc0de), .zE = INT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zap_m = UINT64_C(0xdeadc0de), .zcl = UINT64_C(0xdeadc0de), .zinternal_exponent = false, .zreserved_0 = UINT64_C(0xdeadc0de), .zreserved_1 = UINT64_C(0xdeadc0de), .zsd_perms = UINT64_C(0xdeadc0de), .zsealed = false, .ztag = false };
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz3116 = { .ztup0 = false, .ztup1 = zcbz3117 };
  return zcbz3116;
}



static void finish_sailgen_doCSetBounds(void)
{
  KILL(lbits)(&zghz3452);
  KILL(lbits)(&zghz3451);
  KILL(sail_int)(&zghz3450);
  KILL(sail_int)(&zghz3449);
  KILL(lbits)(&zghz3448);
}

// register TestCap
static struct zCapability zTestCap;

// register TestAddr
static uint64_t zTestAddr;

// register TestLen
static uint64_t zTestLen;

static unit sailgen_main(unit);

struct zCapability zghz3453;
struct zCapability zghz3454;
struct zCapability zghz3455;
struct zCapability zghz3456;
struct zArchPerms zghz3457;
struct zEncMetadata zghz3458;

static void startup_sailgen_main(void)
{






}

static unit sailgen_main(unit zgsz3802)
{
  __label__ case_659, finish_match_658, end_function_662, end_block_exception_663;

  unit zcbz384;
  {
    zTestCap = znull_cap;
    unit zgsz3803;
    zgsz3803 = UNIT;
  }
  struct zCapability zghz3453;
  {
    bool zgaz3553;
    zgaz3553 = zTestCap.ztag;
    uint64_t zgaz3554;
    zgaz3554 = (zTestAddr << 32) | zTestAddr;
    zghz3453 = sailgen_bitsToCap(zgaz3553, zgaz3554);
  }
  uint64_t zcap_bits;
  {
    struct zEncMetadata zghz3458;
    zghz3458 = sailgen_capToMetadataBits(zghz3453);
    zcap_bits = zghz3458.zbits;
  }
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3547;
  zgaz3547 = sailgen_doCSetBounds(zTestCap, zTestLen);
  unit zgsz3804;
  {
    __label__ case_661, finish_match_660;

    bool zexact;
    zexact = zgaz3547.ztup0;
    struct zCapability zghz3454;
    zghz3454 = zgaz3547.ztup1;
    struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3548;
    zgaz3548 = sailgen_setCapAddr(zTestCap, zTestAddr);
    {
      bool zrepresentable;
      zrepresentable = zgaz3548.ztup0;
      struct zCapability zghz3455;
      zghz3455 = zgaz3548.ztup1;
      struct zoptionzIz8izCiz9zK zbounds;
      CREATE(zoptionzIz8izCiz9zK)(&zbounds);
      sailgen_getCapBounds(&zbounds, zTestCap);
      enum zExecutionMode zmode;
      zmode = sailgen_getCapMode(zTestCap);
      sail_string zcap_str;
      CREATE(sail_string)(&zcap_str);
      sailgen_capToString(&zcap_str, zTestCap);
      struct zCapability zghz3456;
      {
        struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgaz3551;
        {
          struct zArchPerms zghz3457;
          zghz3457 = sailgen_getArchPermsLegalizzed(zTestCap);
          enum zExecutionMode zgaz3550;
          zgaz3550 = sailgen_getCapMode(zTestCap);
          zgaz3551.ztup0 = zghz3457;
          zgaz3551.ztup1 = zgaz3550;
        }
        zghz3456 = sailgen_setCapPermsAndMode(zTestCap, zgaz3551);
      }
      struct zoptionzIz8RArchPermszCEExecutionModez5z9zK zperms_mode;
      CREATE(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zperms_mode);
      sailgen_getCapPermsAndMode(&zperms_mode, zTestCap);
      uint64_t zlen;
      zlen = sailgen_getRepresentableLength(zTestAddr);
      uint64_t zmask;
      zmask = sailgen_getRepresentableAlignmentMask(zTestAddr);
      zgsz3804 = UNIT;
      KILL(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zperms_mode);
      KILL(sail_string)(&zcap_str);
      KILL(zoptionzIz8izCiz9zK)(&zbounds);
      goto finish_match_660;
    }
  case_661: ;
    sail_match_failure("main");
  finish_match_660: ;
    goto finish_match_658;
  }
case_659: ;
  sail_match_failure("main");
finish_match_658: ;
  zcbz384 = zgsz3804;
end_function_662: ;
  return zcbz384;
end_block_exception_663: ;

  return UNIT;
}



static void finish_sailgen_main(void)
{
}

static unit sailgen_initializze_registers(unit);

struct zCapability zghz3459;
sail_int zghz3460;

static void startup_sailgen_initializze_registers(void)
{

  CREATE(sail_int)(&zghz3460);
}

static unit sailgen_initializze_registers(unit zgsz3809)
{
  __label__ end_function_665, end_block_exception_666;

  unit zcbz385;
  {
    struct zCapability zghz3459;
    zghz3459.zB = UINT64_C(0b0000000000);
    RECREATE(sail_int)(&zghz3460);
    CONVERT_OF(sail_int, mach_int)(&zghz3460, INT64_C(-7));
    zghz3459.zE = CONVERT_OF(mach_int, sail_int)(zghz3460);
    zghz3459.zT = UINT64_C(0b0000000000);
    zghz3459.zaddress = UINT64_C(0x00000000);
    zghz3459.zap_m = UINT64_C(0b00000);
    zghz3459.zcl = UINT64_C(0b0);
    zghz3459.zinternal_exponent = false;
    zghz3459.zreserved_0 = UINT64_C(0b00);
    zghz3459.zreserved_1 = UINT64_C(0b0);
    zghz3459.zsd_perms = UINT64_C(0b00);
    zghz3459.zsealed = false;
    zghz3459.ztag = false;
    zTestCap = zghz3459;
    unit zgsz3813;
    zgsz3813 = UNIT;
  }
  {
    zTestAddr = UINT64_C(0x00000000);
    unit zgsz3812;
    zgsz3812 = UNIT;
  }
  zTestLen = UINT64_C(0b000000000000000000000000000000000);
  zcbz385 = UNIT;
end_function_665: ;
  return zcbz385;
end_block_exception_666: ;

  return UNIT;
}



static void finish_sailgen_initializze_registers(void)
{    KILL(sail_int)(&zghz3460);
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
  startup_sailgen__update_EncMetadata_L8();
  startup_sailgen__update_EncMetadata_SDP();
  startup_sailgen__update_EncMetadata_TE();
  startup_sailgen__update_EncMetadata_Tm();
  startup_sailgen__update_EncMetadata_reserved_0();
  startup_sailgen__update_EncMetadata_reserved_1();
  startup_sailgen_ap_m_encdec_forwards();
  startup_sailgen_ap_m_encdec_forwards_infallible();
  startup_sailgen_ap_m_encdec_backwards();
  startup_sailgen_ap_m_encdec_backwards_infallible();
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
  sailgen_initializze_registers(UNIT);
}

static void model_fini(void)
{
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
  finish_sailgen__update_EncMetadata_L8();
  finish_sailgen__update_EncMetadata_SDP();
  finish_sailgen__update_EncMetadata_TE();
  finish_sailgen__update_EncMetadata_Tm();
  finish_sailgen__update_EncMetadata_reserved_0();
  finish_sailgen__update_EncMetadata_reserved_1();
  finish_sailgen_ap_m_encdec_forwards();
  finish_sailgen_ap_m_encdec_forwards_infallible();
  finish_sailgen_ap_m_encdec_backwards();
  finish_sailgen_ap_m_encdec_backwards_infallible();
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
