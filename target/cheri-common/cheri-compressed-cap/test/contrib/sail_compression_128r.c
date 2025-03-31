#include "sail.h"
#include "rts.h"
#include "elf.h"
void (*sail_rts_set_coverage_file)(const char *) = NULL;
#ifdef __cplusplus
extern "C" {
#endif

// union option<o>
enum kind_zoptionzIozK { Kind_zNonezIozK, Kind_zSomezIozK };

struct zoptionzIozK {
  enum kind_zoptionzIozK kind;
  union {
    struct { unit zNonezIozK; };
    struct { bool zSomezIozK; };
  };
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
    rop->zNonezIozK = op.zNonezIozK;
  } else if (op.kind == Kind_zSomezIozK) {
    rop->zSomezIozK = op.zSomezIozK;
  }
}

static bool EQUAL(zoptionzIozK)(struct zoptionzIozK op1, struct zoptionzIozK op2) {
  if (op1.kind == Kind_zNonezIozK && op2.kind == Kind_zNonezIozK) {
    return EQUAL(unit)(op1.zNonezIozK, op2.zNonezIozK);
  } else if (op1.kind == Kind_zSomezIozK && op2.kind == Kind_zSomezIozK) {
    return EQUAL(bool)(op1.zSomezIozK, op2.zSomezIozK);
  } else return false;
}

static void sailgen_NonezIozK(struct zoptionzIozK *rop, unit op) {
  {}
  rop->kind = Kind_zNonezIozK;
  rop->zNonezIozK = op;
}

static void sailgen_SomezIozK(struct zoptionzIozK *rop, bool op) {
  {}
  rop->kind = Kind_zSomezIozK;
  rop->zSomezIozK = op;
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
  };
};

static void CREATE(zoptionzIz8izCiz9zK)(struct zoptionzIz8izCiz9zK *op) {
  op->kind = Kind_zNonezIz8izCiz9zK;
}

static void RECREATE(zoptionzIz8izCiz9zK)(struct zoptionzIz8izCiz9zK *op) {

}

static void KILL(zoptionzIz8izCiz9zK)(struct zoptionzIz8izCiz9zK *op) {
  if (op->kind == Kind_zSomezIz8izCiz9zK) {
    KILL(ztuple_z8z5izCz0z5iz9)(&op->zSomezIz8izCiz9zK);
  }
}

static void COPY(zoptionzIz8izCiz9zK)(struct zoptionzIz8izCiz9zK *rop, struct zoptionzIz8izCiz9zK op) {
  if (rop->kind == Kind_zSomezIz8izCiz9zK) {
    KILL(ztuple_z8z5izCz0z5iz9)(&rop->zSomezIz8izCiz9zK);
  };
  rop->kind = op.kind;
  if (op.kind == Kind_zNonezIz8izCiz9zK) {
    rop->zNonezIz8izCiz9zK = op.zNonezIz8izCiz9zK;
  } else if (op.kind == Kind_zSomezIz8izCiz9zK) {
    CREATE(ztuple_z8z5izCz0z5iz9)(&rop->zSomezIz8izCiz9zK); COPY(ztuple_z8z5izCz0z5iz9)(&rop->zSomezIz8izCiz9zK, op.zSomezIz8izCiz9zK);
  }
}

static bool EQUAL(zoptionzIz8izCiz9zK)(struct zoptionzIz8izCiz9zK op1, struct zoptionzIz8izCiz9zK op2) {
  if (op1.kind == Kind_zNonezIz8izCiz9zK && op2.kind == Kind_zNonezIz8izCiz9zK) {
    return EQUAL(unit)(op1.zNonezIz8izCiz9zK, op2.zNonezIz8izCiz9zK);
  } else if (op1.kind == Kind_zSomezIz8izCiz9zK && op2.kind == Kind_zSomezIz8izCiz9zK) {
    return EQUAL(ztuple_z8z5izCz0z5iz9)(op1.zSomezIz8izCiz9zK, op2.zSomezIz8izCiz9zK);
  } else return false;
}

static void sailgen_NonezIz8izCiz9zK(struct zoptionzIz8izCiz9zK *rop, unit op) {
  if (rop->kind == Kind_zSomezIz8izCiz9zK) {
    KILL(ztuple_z8z5izCz0z5iz9)(&rop->zSomezIz8izCiz9zK);
  }
  rop->kind = Kind_zNonezIz8izCiz9zK;
  rop->zNonezIz8izCiz9zK = op;
}

static void sailgen_SomezIz8izCiz9zK(struct zoptionzIz8izCiz9zK *rop, struct ztuple_z8z5izCz0z5iz9 op) {
  if (rop->kind == Kind_zSomezIz8izCiz9zK) {
    KILL(ztuple_z8z5izCz0z5iz9)(&rop->zSomezIz8izCiz9zK);
  }
  rop->kind = Kind_zSomezIz8izCiz9zK;
  CREATE(ztuple_z8z5izCz0z5iz9)(&rop->zSomezIz8izCiz9zK);
  COPY(ztuple_z8z5izCz0z5iz9)(&rop->zSomezIz8izCiz9zK, op);
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
  };
};

static void CREATE(zoptionzIz8bzCbz9zK)(struct zoptionzIz8bzCbz9zK *op) {
  op->kind = Kind_zNonezIz8bzCbz9zK;
}

static void RECREATE(zoptionzIz8bzCbz9zK)(struct zoptionzIz8bzCbz9zK *op) {

}

static void KILL(zoptionzIz8bzCbz9zK)(struct zoptionzIz8bzCbz9zK *op) {
  if (op->kind == Kind_zSomezIz8bzCbz9zK) {
    KILL(ztuple_z8z5bvzCz0z5bvz9)(&op->zSomezIz8bzCbz9zK);
  }
}

static void COPY(zoptionzIz8bzCbz9zK)(struct zoptionzIz8bzCbz9zK *rop, struct zoptionzIz8bzCbz9zK op) {
  if (rop->kind == Kind_zSomezIz8bzCbz9zK) {
    KILL(ztuple_z8z5bvzCz0z5bvz9)(&rop->zSomezIz8bzCbz9zK);
  };
  rop->kind = op.kind;
  if (op.kind == Kind_zNonezIz8bzCbz9zK) {
    rop->zNonezIz8bzCbz9zK = op.zNonezIz8bzCbz9zK;
  } else if (op.kind == Kind_zSomezIz8bzCbz9zK) {
    CREATE(ztuple_z8z5bvzCz0z5bvz9)(&rop->zSomezIz8bzCbz9zK); COPY(ztuple_z8z5bvzCz0z5bvz9)(&rop->zSomezIz8bzCbz9zK, op.zSomezIz8bzCbz9zK);
  }
}

static bool EQUAL(zoptionzIz8bzCbz9zK)(struct zoptionzIz8bzCbz9zK op1, struct zoptionzIz8bzCbz9zK op2) {
  if (op1.kind == Kind_zNonezIz8bzCbz9zK && op2.kind == Kind_zNonezIz8bzCbz9zK) {
    return EQUAL(unit)(op1.zNonezIz8bzCbz9zK, op2.zNonezIz8bzCbz9zK);
  } else if (op1.kind == Kind_zSomezIz8bzCbz9zK && op2.kind == Kind_zSomezIz8bzCbz9zK) {
    return EQUAL(ztuple_z8z5bvzCz0z5bvz9)(op1.zSomezIz8bzCbz9zK, op2.zSomezIz8bzCbz9zK);
  } else return false;
}

static void sailgen_NonezIz8bzCbz9zK(struct zoptionzIz8bzCbz9zK *rop, unit op) {
  if (rop->kind == Kind_zSomezIz8bzCbz9zK) {
    KILL(ztuple_z8z5bvzCz0z5bvz9)(&rop->zSomezIz8bzCbz9zK);
  }
  rop->kind = Kind_zNonezIz8bzCbz9zK;
  rop->zNonezIz8bzCbz9zK = op;
}

static void sailgen_SomezIz8bzCbz9zK(struct zoptionzIz8bzCbz9zK *rop, struct ztuple_z8z5bvzCz0z5bvz9 op) {
  if (rop->kind == Kind_zSomezIz8bzCbz9zK) {
    KILL(ztuple_z8z5bvzCz0z5bvz9)(&rop->zSomezIz8bzCbz9zK);
  }
  rop->kind = Kind_zSomezIz8bzCbz9zK;
  CREATE(ztuple_z8z5bvzCz0z5bvz9)(&rop->zSomezIz8bzCbz9zK);
  COPY(ztuple_z8z5bvzCz0z5bvz9)(&rop->zSomezIz8bzCbz9zK, op);
}

// union exception
enum kind_zexception { Kind_z__dummy_exnz3 };

struct zexception {
  enum kind_zexception kind;
  union {struct { unit z__dummy_exnz3; };};
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
    rop->z__dummy_exnz3 = op.z__dummy_exnz3;
  }
}

static bool EQUAL(zexception)(struct zexception op1, struct zexception op2) {
  if (op1.kind == Kind_z__dummy_exnz3 && op2.kind == Kind_z__dummy_exnz3) {
    return EQUAL(unit)(op1.z__dummy_exnz3, op2.z__dummy_exnz3);
  } else return false;
}

static void sailgen___dummy_exnz3(struct zexception *rop, unit op) {
  {}
  rop->kind = Kind_z__dummy_exnz3;
  rop->z__dummy_exnz3 = op;
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
  rop->zinternal_exponent = op.zinternal_exponent;
  rop->zreserved_0 = op.zreserved_0;
  rop->zreserved_1 = op.zreserved_1;
  rop->zsd_perms = op.zsd_perms;
  rop->zsealed = op.zsealed;
  rop->ztag = op.ztag;
}

static bool EQUAL(zCapability)(struct zCapability op1, struct zCapability op2) {
  return EQUAL(fbits)(op1.zB, op2.zB) && EQUAL(mach_int)(op1.zE, op2.zE) && EQUAL(fbits)(op1.zT, op2.zT) && EQUAL(fbits)(op1.zaddress, op2.zaddress) && EQUAL(fbits)(op1.zap_m, op2.zap_m) && EQUAL(bool)(op1.zinternal_exponent, op2.zinternal_exponent) && EQUAL(fbits)(op1.zreserved_0, op2.zreserved_0) && EQUAL(fbits)(op1.zreserved_1, op2.zreserved_1) && EQUAL(fbits)(op1.zsd_perms, op2.zsd_perms) && EQUAL(bool)(op1.zsealed, op2.zsealed) && EQUAL(bool)(op1.ztag, op2.ztag);
}

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
  bool zLM;
  bool zR;
  bool zW;
  bool zX;
};

static void COPY(zArchPerms)(struct zArchPerms *rop, const struct zArchPerms op) {
  rop->zASR = op.zASR;
  rop->zC = op.zC;
  rop->zLM = op.zLM;
  rop->zR = op.zR;
  rop->zW = op.zW;
  rop->zX = op.zX;
}

static bool EQUAL(zArchPerms)(struct zArchPerms op1, struct zArchPerms op2) {
  return EQUAL(bool)(op1.zASR, op2.zASR) && EQUAL(bool)(op1.zC, op2.zC) && EQUAL(bool)(op1.zLM, op2.zLM) && EQUAL(bool)(op1.zR, op2.zR) && EQUAL(bool)(op1.zW, op2.zW) && EQUAL(bool)(op1.zX, op2.zX);
}

// union option<RArchPerms>
enum kind_zoptionzIRArchPermszK { Kind_zNonezIRArchPermszK, Kind_zSomezIRArchPermszK };

struct zoptionzIRArchPermszK {
  enum kind_zoptionzIRArchPermszK kind;
  union {
    struct { unit zNonezIRArchPermszK; };
    struct { struct zArchPerms zSomezIRArchPermszK; };
  };
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
    rop->zNonezIRArchPermszK = op.zNonezIRArchPermszK;
  } else if (op.kind == Kind_zSomezIRArchPermszK) {
    rop->zSomezIRArchPermszK = op.zSomezIRArchPermszK;
  }
}

static bool EQUAL(zoptionzIRArchPermszK)(struct zoptionzIRArchPermszK op1, struct zoptionzIRArchPermszK op2) {
  if (op1.kind == Kind_zNonezIRArchPermszK && op2.kind == Kind_zNonezIRArchPermszK) {
    return EQUAL(unit)(op1.zNonezIRArchPermszK, op2.zNonezIRArchPermszK);
  } else if (op1.kind == Kind_zSomezIRArchPermszK && op2.kind == Kind_zSomezIRArchPermszK) {
    return EQUAL(zArchPerms)(op1.zSomezIRArchPermszK, op2.zSomezIRArchPermszK);
  } else return false;
}

static void sailgen_NonezIRArchPermszK(struct zoptionzIRArchPermszK *rop, unit op) {
  {}
  rop->kind = Kind_zNonezIRArchPermszK;
  rop->zNonezIRArchPermszK = op;
}

static void sailgen_SomezIRArchPermszK(struct zoptionzIRArchPermszK *rop, struct zArchPerms op) {
  {}
  rop->kind = Kind_zSomezIRArchPermszK;
  rop->zSomezIRArchPermszK = op;
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
  };
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
    rop->zNonezIz8RArchPermszCEExecutionModez5z9zK = op.zNonezIz8RArchPermszCEExecutionModez5z9zK;
  } else if (op.kind == Kind_zSomezIz8RArchPermszCEExecutionModez5z9zK) {
    rop->zSomezIz8RArchPermszCEExecutionModez5z9zK = op.zSomezIz8RArchPermszCEExecutionModez5z9zK;
  }
}

static bool EQUAL(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(struct zoptionzIz8RArchPermszCEExecutionModez5z9zK op1, struct zoptionzIz8RArchPermszCEExecutionModez5z9zK op2) {
  if (op1.kind == Kind_zNonezIz8RArchPermszCEExecutionModez5z9zK && op2.kind == Kind_zNonezIz8RArchPermszCEExecutionModez5z9zK) {
    return EQUAL(unit)(op1.zNonezIz8RArchPermszCEExecutionModez5z9zK, op2.zNonezIz8RArchPermszCEExecutionModez5z9zK);
  } else if (op1.kind == Kind_zSomezIz8RArchPermszCEExecutionModez5z9zK && op2.kind == Kind_zSomezIz8RArchPermszCEExecutionModez5z9zK) {
    return EQUAL(ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9)(op1.zSomezIz8RArchPermszCEExecutionModez5z9zK, op2.zSomezIz8RArchPermszCEExecutionModez5z9zK);
  } else return false;
}

static void sailgen_NonezIz8RArchPermszCEExecutionModez5z9zK(struct zoptionzIz8RArchPermszCEExecutionModez5z9zK *rop, unit op) {
  {}
  rop->kind = Kind_zNonezIz8RArchPermszCEExecutionModez5z9zK;
  rop->zNonezIz8RArchPermszCEExecutionModez5z9zK = op;
}

static void sailgen_SomezIz8RArchPermszCEExecutionModez5z9zK(struct zoptionzIz8RArchPermszCEExecutionModez5z9zK *rop, struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 op) {
  {}
  rop->kind = Kind_zSomezIz8RArchPermszCEExecutionModez5z9zK;
  rop->zSomezIz8RArchPermszCEExecutionModez5z9zK = op;
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
  __label__ end_function_7, end_block_exception_8, end_function_407;

  RECREATE(lbits)(&zghz30);
  zeros(&zghz30, zn);
  not_bits((*(&zcbz32)), zghz30);
end_function_7: ;
  goto end_function_407;
end_block_exception_8: ;
  goto end_function_407;
end_function_407: ;
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
  zgaz33 = eq_fbits(zx, zy);
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
  __label__ end_function_19, end_block_exception_20, end_function_406;

  zero_extend((*(&zcbz36)), zv, zm);
end_function_19: ;
  goto end_function_406;
end_block_exception_20: ;
  goto end_function_406;
end_function_406: ;
}

static void sailgen_zzeros_implicit(lbits *rop, sail_int);

static void sailgen_zzeros_implicit(lbits *zcbz37, sail_int zn)
{
  __label__ end_function_22, end_block_exception_23, end_function_405;

  zeros((*(&zcbz37)), zn);
end_function_22: ;
  goto end_function_405;
end_block_exception_23: ;
  goto end_function_405;
end_function_405: ;
}

static void sailgen_ones(lbits *rop, sail_int);

static void sailgen_ones(lbits *zcbz38, sail_int zn)
{
  __label__ end_function_25, end_block_exception_26, end_function_404;

  sailgen_sail_ones((*(&zcbz38)), zn);
end_function_25: ;
  goto end_function_404;
end_block_exception_26: ;
  goto end_function_404;
end_function_404: ;
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
    if (!(zgsz32)) {

      goto case_30;
    }
    zcbz39 = UINT64_C(0b1);
    goto finish_match_28;
  }
case_30: ;
  {
    bool zuz30;
    zuz30 = zargz3;
    bool zgsz31;
    zgsz31 = eq_bool(zuz30, false);
    if (!(zgsz31)) {

      goto case_29;
    }
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
    if (!(zgsz35)) {

      goto case_35;
    }
    zcbz310 = UINT64_C(0b1);
    goto finish_match_33;
  }
case_35: ;
  {
    bool zuz31;
    zuz31 = zargz3;
    bool zgsz34;
    zgsz34 = eq_bool(zuz31, false);
    if (!(zgsz34)) {

      goto case_34;
    }
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
    if (!(zgsz38)) {

      goto case_41;
    }
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
    if (!(zgsz311)) {

      goto case_46;
    }
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
    if (!(zgsz315)) {

      goto case_53;
    }
    zcbz313 = true;
    goto finish_match_50;
  }
case_53: ;
  {
    uint64_t zb__1;
    zb__1 = zargz3;
    bool zgsz314;
    zgsz314 = (zb__1 == UINT64_C(0b0));
    if (!(zgsz314)) {

      goto case_52;
    }
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
  __label__ end_function_57, end_block_exception_58, end_function_403;

  {
    RECREATE(sail_int)(&zghz31);
    CONVERT_OF(sail_int, mach_int)(&zghz31, INT64_C(0));
    get_slice_int((*(&zcbz314)), zl, zn, zghz31);
  }
end_function_57: ;
  goto end_function_403;
end_block_exception_58: ;
  goto end_function_403;
end_function_403: ;
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



static int64_t zlog2_xlen_bytes;
static void create_letbind_0(void) {


  int64_t zgsz316;
  zgsz316 = INT64_C(3);
  zlog2_xlen_bytes = zgsz316;

let_end_65: ;
}
static void kill_letbind_0(void) {
}

static int64_t zflen_bytes;
static void create_letbind_1(void) {


  int64_t zgsz317;
  zgsz317 = INT64_C(8);
  zflen_bytes = zgsz317;

let_end_66: ;
}
static void kill_letbind_1(void) {
}

static int64_t zflen;
static void create_letbind_2(void) {


  int64_t zgsz318;
  zgsz318 = INT64_C(64);
  zflen = zgsz318;

let_end_67: ;
}
static void kill_letbind_2(void) {
}

static int64_t zxlen_bytes;
static void create_letbind_3(void) {


  int64_t zgsz319;
  zgsz319 = INT64_C(8);
  zxlen_bytes = zgsz319;

let_end_68: ;
}
static void kill_letbind_3(void) {
}

static int64_t zxlen;
static void create_letbind_4(void) {


  int64_t zgsz320;
  zgsz320 = INT64_C(64);
  zxlen = zgsz320;

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
  __label__ end_function_71, end_block_exception_72, end_function_402;

  RECREATE(sail_int)(&zghz36);
  pow2(&zghz36, zn);
  {
    RECREATE(sail_int)(&zghz37);
    CONVERT_OF(sail_int, mach_int)(&zghz37, INT64_C(1));
    sub_int((*(&zcbz317)), zghz36, zghz37);
  }
end_function_71: ;
  goto end_function_402;
end_block_exception_72: ;
  goto end_function_402;
end_function_402: ;
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
    if (!(zgsz329)) {

      goto case_87;
    }
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
    if (!(zgsz332)) {

      goto case_92;
    }
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
    if (!(zgsz336)) {

      goto case_99;
    }
    zcbz322 = true;
    goto finish_match_96;
  }
case_99: ;
  {
    uint64_t zb__1;
    zb__1 = zargz3;
    bool zgsz335;
    zgsz335 = (zb__1 == UINT64_C(0b0));
    if (!(zgsz335)) {

      goto case_98;
    }
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
  zgsz337.zLM = false;
  zgsz337.zR = false;
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
  zgsz342 = INT64_C(16);
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
  struct zEncMetadata zcbz383 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz383;
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
  struct zEncMetadata zcbz384 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz384;
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
  struct zEncMetadata zcbz385 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz385;
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
  struct zEncMetadata zcbz386 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz386;
}



static void finish_sailgen__update_EncMetadata_Bm(void)
{
  KILL(lbits)(&zghz326);
  KILL(lbits)(&zghz325);
  KILL(sail_int)(&zghz324);
  KILL(sail_int)(&zghz323);
  KILL(lbits)(&zghz322);

}

static uint64_t sailgen__get_EncMetadata_CT(struct zEncMetadata);

static uint64_t sailgen__get_EncMetadata_CT(struct zEncMetadata zv)
{
  __label__ end_function_133, end_block_exception_134;

  uint64_t zcbz330;
  uint64_t zgaz319;
  zgaz319 = zv.zbits;
  zcbz330 = (safe_rshift(UINT64_MAX, 64 - 1) & (zgaz319 >> INT64_C(27)));

end_function_133: ;
  return zcbz330;
end_block_exception_134: ;

  return UINT64_C(0xdeadc0de);
}

static struct zEncMetadata sailgen__update_EncMetadata_CT(struct zEncMetadata, uint64_t);

struct zEncMetadata zghz327;
lbits zghz328;
sail_int zghz329;
sail_int zghz330;
lbits zghz331;
lbits zghz332;

static void startup_sailgen__update_EncMetadata_CT(void)
{

  CREATE(lbits)(&zghz328);
  CREATE(sail_int)(&zghz329);
  CREATE(sail_int)(&zghz330);
  CREATE(lbits)(&zghz331);
  CREATE(lbits)(&zghz332);
}

static struct zEncMetadata sailgen__update_EncMetadata_CT(struct zEncMetadata zv, uint64_t zx)
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
      CONVERT_OF(sail_int, mach_int)(&zghz329, INT64_C(27));
      RECREATE(sail_int)(&zghz330);
      CONVERT_OF(sail_int, mach_int)(&zghz330, INT64_C(27));
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
  struct zEncMetadata zcbz387 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz387;
}



static void finish_sailgen__update_EncMetadata_CT(void)
{
  KILL(lbits)(&zghz332);
  KILL(lbits)(&zghz331);
  KILL(sail_int)(&zghz330);
  KILL(sail_int)(&zghz329);
  KILL(lbits)(&zghz328);

}

static uint64_t sailgen__get_EncMetadata_EF(struct zEncMetadata);

static uint64_t sailgen__get_EncMetadata_EF(struct zEncMetadata zv)
{
  __label__ end_function_139, end_block_exception_140;

  uint64_t zcbz332;
  uint64_t zgaz322;
  zgaz322 = zv.zbits;
  zcbz332 = (safe_rshift(UINT64_MAX, 64 - 1) & (zgaz322 >> INT64_C(26)));

end_function_139: ;
  return zcbz332;
end_block_exception_140: ;

  return UINT64_C(0xdeadc0de);
}

static struct zEncMetadata sailgen__update_EncMetadata_EF(struct zEncMetadata, uint64_t);

struct zEncMetadata zghz333;
lbits zghz334;
sail_int zghz335;
sail_int zghz336;
lbits zghz337;
lbits zghz338;

static void startup_sailgen__update_EncMetadata_EF(void)
{

  CREATE(lbits)(&zghz334);
  CREATE(sail_int)(&zghz335);
  CREATE(sail_int)(&zghz336);
  CREATE(lbits)(&zghz337);
  CREATE(lbits)(&zghz338);
}

static struct zEncMetadata sailgen__update_EncMetadata_EF(struct zEncMetadata zv, uint64_t zx)
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
      CONVERT_OF(sail_int, mach_int)(&zghz335, INT64_C(26));
      RECREATE(sail_int)(&zghz336);
      CONVERT_OF(sail_int, mach_int)(&zghz336, INT64_C(26));
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
  struct zEncMetadata zcbz388 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz388;
}



static void finish_sailgen__update_EncMetadata_EF(void)
{
  KILL(lbits)(&zghz338);
  KILL(lbits)(&zghz337);
  KILL(sail_int)(&zghz336);
  KILL(sail_int)(&zghz335);
  KILL(lbits)(&zghz334);

}

static uint64_t sailgen__get_EncMetadata_SDP(struct zEncMetadata);

static uint64_t sailgen__get_EncMetadata_SDP(struct zEncMetadata zv)
{
  __label__ end_function_145, end_block_exception_146;

  uint64_t zcbz334;
  uint64_t zgaz325;
  zgaz325 = zv.zbits;
  zcbz334 = (safe_rshift(UINT64_MAX, 64 - 4) & (zgaz325 >> INT64_C(53)));

end_function_145: ;
  return zcbz334;
end_block_exception_146: ;

  return UINT64_C(0xdeadc0de);
}

static struct zEncMetadata sailgen__update_EncMetadata_SDP(struct zEncMetadata, uint64_t);

struct zEncMetadata zghz339;
lbits zghz340;
sail_int zghz341;
sail_int zghz342;
lbits zghz343;
lbits zghz344;

static void startup_sailgen__update_EncMetadata_SDP(void)
{

  CREATE(lbits)(&zghz340);
  CREATE(sail_int)(&zghz341);
  CREATE(sail_int)(&zghz342);
  CREATE(lbits)(&zghz343);
  CREATE(lbits)(&zghz344);
}

static struct zEncMetadata sailgen__update_EncMetadata_SDP(struct zEncMetadata zv, uint64_t zx)
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
      CONVERT_OF(sail_int, mach_int)(&zghz341, INT64_C(56));
      RECREATE(sail_int)(&zghz342);
      CONVERT_OF(sail_int, mach_int)(&zghz342, INT64_C(53));
      RECREATE(lbits)(&zghz343);
      CONVERT_OF(lbits, fbits)(&zghz343, zx, UINT64_C(4) , true);
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
  struct zEncMetadata zcbz389 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz389;
}



static void finish_sailgen__update_EncMetadata_SDP(void)
{
  KILL(lbits)(&zghz344);
  KILL(lbits)(&zghz343);
  KILL(sail_int)(&zghz342);
  KILL(sail_int)(&zghz341);
  KILL(lbits)(&zghz340);

}

static uint64_t sailgen__get_EncMetadata_TE(struct zEncMetadata);

static uint64_t sailgen__get_EncMetadata_TE(struct zEncMetadata zv)
{
  __label__ end_function_151, end_block_exception_152;

  uint64_t zcbz336;
  uint64_t zgaz328;
  zgaz328 = zv.zbits;
  zcbz336 = (safe_rshift(UINT64_MAX, 64 - 3) & (zgaz328 >> INT64_C(14)));

end_function_151: ;
  return zcbz336;
end_block_exception_152: ;

  return UINT64_C(0xdeadc0de);
}

static struct zEncMetadata sailgen__update_EncMetadata_TE(struct zEncMetadata, uint64_t);

struct zEncMetadata zghz345;
lbits zghz346;
sail_int zghz347;
sail_int zghz348;
lbits zghz349;
lbits zghz350;

static void startup_sailgen__update_EncMetadata_TE(void)
{

  CREATE(lbits)(&zghz346);
  CREATE(sail_int)(&zghz347);
  CREATE(sail_int)(&zghz348);
  CREATE(lbits)(&zghz349);
  CREATE(lbits)(&zghz350);
}

static struct zEncMetadata sailgen__update_EncMetadata_TE(struct zEncMetadata zv, uint64_t zx)
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
      CONVERT_OF(sail_int, mach_int)(&zghz347, INT64_C(16));
      RECREATE(sail_int)(&zghz348);
      CONVERT_OF(sail_int, mach_int)(&zghz348, INT64_C(14));
      RECREATE(lbits)(&zghz349);
      CONVERT_OF(lbits, fbits)(&zghz349, zx, UINT64_C(3) , true);
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
  struct zEncMetadata zcbz390 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz390;
}



static void finish_sailgen__update_EncMetadata_TE(void)
{
  KILL(lbits)(&zghz350);
  KILL(lbits)(&zghz349);
  KILL(sail_int)(&zghz348);
  KILL(sail_int)(&zghz347);
  KILL(lbits)(&zghz346);

}

static uint64_t sailgen__get_EncMetadata_Tm(struct zEncMetadata);

static uint64_t sailgen__get_EncMetadata_Tm(struct zEncMetadata zv)
{
  __label__ end_function_157, end_block_exception_158;

  uint64_t zcbz338;
  uint64_t zgaz331;
  zgaz331 = zv.zbits;
  zcbz338 = (safe_rshift(UINT64_MAX, 64 - 9) & (zgaz331 >> INT64_C(17)));

end_function_157: ;
  return zcbz338;
end_block_exception_158: ;

  return UINT64_C(0xdeadc0de);
}

static struct zEncMetadata sailgen__update_EncMetadata_Tm(struct zEncMetadata, uint64_t);

struct zEncMetadata zghz351;
lbits zghz352;
sail_int zghz353;
sail_int zghz354;
lbits zghz355;
lbits zghz356;

static void startup_sailgen__update_EncMetadata_Tm(void)
{

  CREATE(lbits)(&zghz352);
  CREATE(sail_int)(&zghz353);
  CREATE(sail_int)(&zghz354);
  CREATE(lbits)(&zghz355);
  CREATE(lbits)(&zghz356);
}

static struct zEncMetadata sailgen__update_EncMetadata_Tm(struct zEncMetadata zv, uint64_t zx)
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
      CONVERT_OF(sail_int, mach_int)(&zghz353, INT64_C(25));
      RECREATE(sail_int)(&zghz354);
      CONVERT_OF(sail_int, mach_int)(&zghz354, INT64_C(17));
      RECREATE(lbits)(&zghz355);
      CONVERT_OF(lbits, fbits)(&zghz355, zx, UINT64_C(9) , true);
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
  struct zEncMetadata zcbz391 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz391;
}



static void finish_sailgen__update_EncMetadata_Tm(void)
{
  KILL(lbits)(&zghz356);
  KILL(lbits)(&zghz355);
  KILL(sail_int)(&zghz354);
  KILL(sail_int)(&zghz353);
  KILL(lbits)(&zghz352);

}

static uint64_t sailgen__get_EncMetadata_reserved_0(struct zEncMetadata);

static uint64_t sailgen__get_EncMetadata_reserved_0(struct zEncMetadata zv)
{
  __label__ end_function_163, end_block_exception_164;

  uint64_t zcbz340;
  uint64_t zgaz334;
  zgaz334 = zv.zbits;
  zcbz340 = (safe_rshift(UINT64_MAX, 64 - 16) & (zgaz334 >> INT64_C(28)));

end_function_163: ;
  return zcbz340;
end_block_exception_164: ;

  return UINT64_C(0xdeadc0de);
}

static struct zEncMetadata sailgen__update_EncMetadata_reserved_0(struct zEncMetadata, uint64_t);

struct zEncMetadata zghz357;
lbits zghz358;
sail_int zghz359;
sail_int zghz360;
lbits zghz361;
lbits zghz362;

static void startup_sailgen__update_EncMetadata_reserved_0(void)
{

  CREATE(lbits)(&zghz358);
  CREATE(sail_int)(&zghz359);
  CREATE(sail_int)(&zghz360);
  CREATE(lbits)(&zghz361);
  CREATE(lbits)(&zghz362);
}

static struct zEncMetadata sailgen__update_EncMetadata_reserved_0(struct zEncMetadata zv, uint64_t zx)
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
      CONVERT_OF(sail_int, mach_int)(&zghz359, INT64_C(43));
      RECREATE(sail_int)(&zghz360);
      CONVERT_OF(sail_int, mach_int)(&zghz360, INT64_C(28));
      RECREATE(lbits)(&zghz361);
      CONVERT_OF(lbits, fbits)(&zghz361, zx, UINT64_C(16) , true);
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
  struct zEncMetadata zcbz392 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz392;
}



static void finish_sailgen__update_EncMetadata_reserved_0(void)
{
  KILL(lbits)(&zghz362);
  KILL(lbits)(&zghz361);
  KILL(sail_int)(&zghz360);
  KILL(sail_int)(&zghz359);
  KILL(lbits)(&zghz358);

}

static uint64_t sailgen__get_EncMetadata_reserved_1(struct zEncMetadata);

static uint64_t sailgen__get_EncMetadata_reserved_1(struct zEncMetadata zv)
{
  __label__ end_function_169, end_block_exception_170;

  uint64_t zcbz342;
  uint64_t zgaz337;
  zgaz337 = zv.zbits;
  zcbz342 = (safe_rshift(UINT64_MAX, 64 - 7) & (zgaz337 >> INT64_C(57)));

end_function_169: ;
  return zcbz342;
end_block_exception_170: ;

  return UINT64_C(0xdeadc0de);
}

static struct zEncMetadata sailgen__update_EncMetadata_reserved_1(struct zEncMetadata, uint64_t);

struct zEncMetadata zghz363;
lbits zghz364;
sail_int zghz365;
sail_int zghz366;
lbits zghz367;
lbits zghz368;

static void startup_sailgen__update_EncMetadata_reserved_1(void)
{

  CREATE(lbits)(&zghz364);
  CREATE(sail_int)(&zghz365);
  CREATE(sail_int)(&zghz366);
  CREATE(lbits)(&zghz367);
  CREATE(lbits)(&zghz368);
}

static struct zEncMetadata sailgen__update_EncMetadata_reserved_1(struct zEncMetadata zv, uint64_t zx)
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
      CONVERT_OF(sail_int, mach_int)(&zghz365, INT64_C(63));
      RECREATE(sail_int)(&zghz366);
      CONVERT_OF(sail_int, mach_int)(&zghz366, INT64_C(57));
      RECREATE(lbits)(&zghz367);
      CONVERT_OF(lbits, fbits)(&zghz367, zx, UINT64_C(7) , true);
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
  struct zEncMetadata zcbz393 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz393;
}



static void finish_sailgen__update_EncMetadata_reserved_1(void)
{
  KILL(lbits)(&zghz368);
  KILL(lbits)(&zghz367);
  KILL(sail_int)(&zghz366);
  KILL(sail_int)(&zghz365);
  KILL(lbits)(&zghz364);

}

static struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 sailgen_ap_m_encdec_forwards(uint64_t);

static uint64_t sailgen_ap_m_encdec_backwards(struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9);

static bool sailgen_ap_m_encdec_forwards_matches(uint64_t);

struct zArchPerms zghz369;

static void startup_sailgen_ap_m_encdec_forwards(void)
{
}



// struct tuple_(%enum zExecutionMode, %bool, %bool, %bool, %bool, %bool, %bool)
struct ztuple_z8z5enumz0zzExecutionModezCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolz9 {
  enum zExecutionMode ztup0;
  bool ztup1;
  bool ztup2;
  bool ztup3;
  bool ztup4;
  bool ztup5;
  bool ztup6;
};

static void COPY(ztuple_z8z5enumz0zzExecutionModezCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolz9)(struct ztuple_z8z5enumz0zzExecutionModezCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolz9 *rop, const struct ztuple_z8z5enumz0zzExecutionModezCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolz9 op) {
  rop->ztup0 = op.ztup0;
  rop->ztup1 = op.ztup1;
  rop->ztup2 = op.ztup2;
  rop->ztup3 = op.ztup3;
  rop->ztup4 = op.ztup4;
  rop->ztup5 = op.ztup5;
  rop->ztup6 = op.ztup6;
}

static bool EQUAL(ztuple_z8z5enumz0zzExecutionModezCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolz9)(struct ztuple_z8z5enumz0zzExecutionModezCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolz9 op1, struct ztuple_z8z5enumz0zzExecutionModezCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolz9 op2) {
  return EQUAL(zExecutionMode)(op1.ztup0, op2.ztup0) && EQUAL(bool)(op1.ztup1, op2.ztup1) && EQUAL(bool)(op1.ztup2, op2.ztup2) && EQUAL(bool)(op1.ztup3, op2.ztup3) && EQUAL(bool)(op1.ztup4, op2.ztup4) && EQUAL(bool)(op1.ztup5, op2.ztup5) && EQUAL(bool)(op1.ztup6, op2.ztup6);
}

static struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 sailgen_ap_m_encdec_forwards(uint64_t zargz3)
{
  __label__ case_182, finish_match_181, end_function_183, end_block_exception_184;

  struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zcbz344;
  uint64_t zhead_expz3;
  zhead_expz3 = zargz3;
  struct zoptionzIz8RArchPermszCEExecutionModez5z9zK zgaz364;
  CREATE(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zgaz364);
  {
    __label__ case_177, case_176, finish_match_175;

    {
      __label__ case_180, case_179, finish_match_178;

      uint64_t zv__0;
      zv__0 = zhead_expz3;
      bool zgaz363;
      {
        uint64_t zmapping0z3;
        zmapping0z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(8)));
        uint64_t zmapping6z3;
        zmapping6z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(0)));
        uint64_t zmapping5z3;
        zmapping5z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(1)));
        uint64_t zmapping4z3;
        zmapping4z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(2)));
        uint64_t zmapping3z3;
        zmapping3z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(3)));
        uint64_t zmapping2z3;
        zmapping2z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(4)));
        uint64_t zmapping1z3;
        zmapping1z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(5)));
        uint64_t zmapping0z3shadowz30;
        zmapping0z3shadowz30 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(8)));
        bool zgaz361;
        zgaz361 = sailgen_execution_mode_encdec_backwards_matches(zmapping0z3shadowz30);
        bool zgsz365;
        if (zgaz361) {
          bool zgaz360;
          zgaz360 = sailgen_bool_bits_backwards_matches(zmapping1z3);
          bool zgsz364;
          if (zgaz360) {
            bool zgaz359;
            zgaz359 = sailgen_bool_bits_backwards_matches(zmapping2z3);
            bool zgsz363;
            if (zgaz359) {
              bool zgaz358;
              zgaz358 = sailgen_bool_bits_backwards_matches(zmapping3z3);
              bool zgsz362;
              if (zgaz358) {
                bool zgaz357;
                zgaz357 = sailgen_bool_bits_backwards_matches(zmapping4z3);
                bool zgsz361;
                if (zgaz357) {
                  bool zgaz356;
                  zgaz356 = sailgen_bool_bits_backwards_matches(zmapping5z3);
                  bool zgsz360;
                  if (zgaz356) {  zgsz360 = sailgen_bool_bits_backwards_matches(zmapping6z3);  } else {
                    zgsz360 = false;
                  }
                  zgsz361 = zgsz360;
                } else {  zgsz361 = false;  }
                zgsz362 = zgsz361;
              } else {  zgsz362 = false;  }
              zgsz363 = zgsz362;
            } else {  zgsz363 = false;  }
            zgsz364 = zgsz363;
          } else {  zgsz364 = false;  }
          zgsz365 = zgsz364;
        } else {  zgsz365 = false;  }
        zgaz363 = zgsz365;
      }
      bool zgsz366;
      if (zgaz363) {
        uint64_t zgaz362;
        zgaz362 = (safe_rshift(UINT64_MAX, 64 - 2) & (zv__0 >> INT64_C(6)));
        zgsz366 = (zgaz362 == UINT64_C(0b00));
      } else {  zgsz366 = false;  }
      bool zgsz381;
      zgsz381 = zgsz366;
      if (!(zgsz381)) {

        goto case_177;
      }
      uint64_t zmapping0z3shadowz31;
      zmapping0z3shadowz31 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(8)));
      uint64_t zmapping6z3shadowz32;
      zmapping6z3shadowz32 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(0)));
      uint64_t zmapping5z3shadowz33;
      zmapping5z3shadowz33 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(1)));
      uint64_t zmapping4z3shadowz34;
      zmapping4z3shadowz34 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(2)));
      uint64_t zmapping3z3shadowz35;
      zmapping3z3shadowz35 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(3)));
      uint64_t zmapping2z3shadowz36;
      zmapping2z3shadowz36 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(4)));
      uint64_t zmapping1z3shadowz37;
      zmapping1z3shadowz37 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(5)));
      uint64_t zmapping0z3shadowz38;
      zmapping0z3shadowz38 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(8)));
      struct ztuple_z8z5enumz0zzExecutionModezCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolz9 zgaz347;
      {
        enum zExecutionMode zgaz340;
        zgaz340 = sailgen_execution_mode_encdec_backwards_infallible(zmapping0z3shadowz38);
        bool zgaz341;
        zgaz341 = sailgen_bool_bits_backwards_infallible(zmapping1z3shadowz37);
        bool zgaz342;
        zgaz342 = sailgen_bool_bits_backwards_infallible(zmapping2z3shadowz36);
        bool zgaz343;
        zgaz343 = sailgen_bool_bits_backwards_infallible(zmapping3z3shadowz35);
        bool zgaz344;
        zgaz344 = sailgen_bool_bits_backwards_infallible(zmapping4z3shadowz34);
        bool zgaz345;
        zgaz345 = sailgen_bool_bits_backwards_infallible(zmapping5z3shadowz33);
        bool zgaz346;
        zgaz346 = sailgen_bool_bits_backwards_infallible(zmapping6z3shadowz32);
        zgaz347.ztup0 = zgaz340;
        zgaz347.ztup1 = zgaz341;
        zgaz347.ztup2 = zgaz342;
        zgaz347.ztup3 = zgaz343;
        zgaz347.ztup4 = zgaz344;
        zgaz347.ztup5 = zgaz345;
        zgaz347.ztup6 = zgaz346;
      }
      struct zoptionzIz8RArchPermszCEExecutionModez5z9zK zgsz368;
      CREATE(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zgsz368);
      {
        enum zExecutionMode zM;
        zM = zgaz347.ztup0;
        bool zLM;
        zLM = zgaz347.ztup1;
        bool zASR;
        zASR = zgaz347.ztup2;
        bool zX;
        zX = zgaz347.ztup3;
        bool zR;
        zR = zgaz347.ztup4;
        bool zW;
        zW = zgaz347.ztup5;
        bool zC;
        zC = zgaz347.ztup6;
        bool zgaz355;
        {
          bool zgaz348;
          zgaz348 = not(zASR);
          bool zgsz370;
          if (zgaz348) {  zgsz370 = true;  } else {  zgsz370 = zX;  }
          zgaz355 = zgsz370;
        }
        bool zgsz377;
        if (zgaz355) {
          bool zgaz354;
          {
            bool zgaz349;
            zgaz349 = not(zC);
            bool zgsz372;
            if (zgaz349) {  zgsz372 = true;  } else {
              bool zgsz371;
              if (zR) {  zgsz371 = true;  } else {  zgsz371 = zW;  }
              zgsz372 = zgsz371;
            }
            zgaz354 = zgsz372;
          }
          bool zgsz376;
          if (zgaz354) {
            bool zgaz353;
            {
              bool zgaz351;
              {
                enum zExecutionMode zgaz350;
                zgaz350 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
                zgaz351 = eq_zExecutionMode(zM, zgaz350);
              }
              bool zgsz373;
              if (zgaz351) {  zgsz373 = true;  } else {  zgsz373 = zX;  }
              zgaz353 = zgsz373;
            }
            bool zgsz375;
            if (zgaz353) {
              bool zgaz352;
              zgaz352 = not(zLM);
              bool zgsz374;
              if (zgaz352) {  zgsz374 = true;  } else {  zgsz374 = zC;  }
              zgsz375 = zgsz374;
            } else {  zgsz375 = false;  }
            zgsz376 = zgsz375;
          } else {  zgsz376 = false;  }
          zgsz377 = zgsz376;
        } else {  zgsz377 = false;  }
        bool zgsz380;
        zgsz380 = zgsz377;
        if (!(zgsz380)) {






          goto case_180;
        }
        struct zArchPerms zghz369;
        zghz369.zASR = zASR;
        zghz369.zC = zC;
        zghz369.zLM = zLM;
        zghz369.zR = zR;
        zghz369.zW = zW;
        zghz369.zX = zX;
        struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz379;
        zgsz379.ztup0 = zghz369;
        zgsz379.ztup1 = zM;
        sailgen_SomezIz8RArchPermszCEExecutionModez5z9zK(&zgsz368, zgsz379);
        goto finish_match_178;
      }
    case_180: ;
      {
        sailgen_NonezIz8RArchPermszCEExecutionModez5z9zK(&zgsz368, UNIT);
        goto finish_match_178;
      }
    case_179: ;
    finish_match_178: ;
      COPY(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zgaz364, zgsz368);
      KILL(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zgsz368);
      goto finish_match_175;
    }
  case_177: ;
    {
      sailgen_NonezIz8RArchPermszCEExecutionModez5z9zK(&zgaz364, UNIT);
      goto finish_match_175;
    }
  case_176: ;
  finish_match_175: ;
  }
  struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz382;
  {
    if (zgaz364.kind != Kind_zSomezIz8RArchPermszCEExecutionModez5z9zK) goto case_182;
    zgsz382 = zgaz364.zSomezIz8RArchPermszCEExecutionModez5z9zK;
    goto finish_match_181;
  }
case_182: ;
  sail_match_failure("ap_m_encdec_forwards");
finish_match_181: ;
  zcbz344 = zgsz382;

  KILL(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zgaz364);

end_function_183: ;
  return zcbz344;
end_block_exception_184: ;
  struct zArchPerms zcbz395 = { .zASR = false, .zC = false, .zLM = false, .zR = false, .zW = false, .zX = false };
  struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zcbz394 = { .ztup0 = zcbz395, .ztup1 = zCapPtrMode };
  return zcbz394;
}



static void finish_sailgen_ap_m_encdec_forwards(void)
{
}

static struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 sailgen_ap_m_encdec_forwards_infallible(uint64_t);

struct zArchPerms zghz370;

static void startup_sailgen_ap_m_encdec_forwards_infallible(void)
{
}



static struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 sailgen_ap_m_encdec_forwards_infallible(uint64_t zargz3)
{
  __label__ case_192, finish_match_191, end_function_193, end_block_exception_194;

  struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zcbz345;
  uint64_t zhead_expz3;
  zhead_expz3 = zargz3;
  struct zoptionzIz8RArchPermszCEExecutionModez5z9zK zgaz364;
  CREATE(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zgaz364);
  {
    __label__ case_187, case_186, finish_match_185;

    {
      __label__ case_190, case_189, finish_match_188;

      uint64_t zv__0;
      zv__0 = zhead_expz3;
      bool zgaz363;
      {
        uint64_t zmapping0z3;
        zmapping0z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(8)));
        uint64_t zmapping6z3;
        zmapping6z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(0)));
        uint64_t zmapping5z3;
        zmapping5z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(1)));
        uint64_t zmapping4z3;
        zmapping4z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(2)));
        uint64_t zmapping3z3;
        zmapping3z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(3)));
        uint64_t zmapping2z3;
        zmapping2z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(4)));
        uint64_t zmapping1z3;
        zmapping1z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(5)));
        uint64_t zmapping0z3shadowz30;
        zmapping0z3shadowz30 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(8)));
        bool zgaz361;
        zgaz361 = sailgen_execution_mode_encdec_backwards_matches(zmapping0z3shadowz30);
        bool zgsz391;
        if (zgaz361) {
          bool zgaz360;
          zgaz360 = sailgen_bool_bits_backwards_matches(zmapping1z3);
          bool zgsz390;
          if (zgaz360) {
            bool zgaz359;
            zgaz359 = sailgen_bool_bits_backwards_matches(zmapping2z3);
            bool zgsz389;
            if (zgaz359) {
              bool zgaz358;
              zgaz358 = sailgen_bool_bits_backwards_matches(zmapping3z3);
              bool zgsz388;
              if (zgaz358) {
                bool zgaz357;
                zgaz357 = sailgen_bool_bits_backwards_matches(zmapping4z3);
                bool zgsz387;
                if (zgaz357) {
                  bool zgaz356;
                  zgaz356 = sailgen_bool_bits_backwards_matches(zmapping5z3);
                  bool zgsz386;
                  if (zgaz356) {  zgsz386 = sailgen_bool_bits_backwards_matches(zmapping6z3);  } else {
                    zgsz386 = false;
                  }
                  zgsz387 = zgsz386;
                } else {  zgsz387 = false;  }
                zgsz388 = zgsz387;
              } else {  zgsz388 = false;  }
              zgsz389 = zgsz388;
            } else {  zgsz389 = false;  }
            zgsz390 = zgsz389;
          } else {  zgsz390 = false;  }
          zgsz391 = zgsz390;
        } else {  zgsz391 = false;  }
        zgaz363 = zgsz391;
      }
      bool zgsz392;
      if (zgaz363) {
        uint64_t zgaz362;
        zgaz362 = (safe_rshift(UINT64_MAX, 64 - 2) & (zv__0 >> INT64_C(6)));
        zgsz392 = (zgaz362 == UINT64_C(0b00));
      } else {  zgsz392 = false;  }
      bool zgsz3107;
      zgsz3107 = zgsz392;
      if (!(zgsz3107)) {

        goto case_187;
      }
      uint64_t zmapping0z3shadowz31;
      zmapping0z3shadowz31 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(8)));
      uint64_t zmapping6z3shadowz32;
      zmapping6z3shadowz32 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(0)));
      uint64_t zmapping5z3shadowz33;
      zmapping5z3shadowz33 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(1)));
      uint64_t zmapping4z3shadowz34;
      zmapping4z3shadowz34 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(2)));
      uint64_t zmapping3z3shadowz35;
      zmapping3z3shadowz35 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(3)));
      uint64_t zmapping2z3shadowz36;
      zmapping2z3shadowz36 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(4)));
      uint64_t zmapping1z3shadowz37;
      zmapping1z3shadowz37 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(5)));
      uint64_t zmapping0z3shadowz38;
      zmapping0z3shadowz38 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__0 >> INT64_C(8)));
      struct ztuple_z8z5enumz0zzExecutionModezCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolz9 zgaz347;
      {
        enum zExecutionMode zgaz340;
        zgaz340 = sailgen_execution_mode_encdec_backwards_infallible(zmapping0z3shadowz38);
        bool zgaz341;
        zgaz341 = sailgen_bool_bits_backwards_infallible(zmapping1z3shadowz37);
        bool zgaz342;
        zgaz342 = sailgen_bool_bits_backwards_infallible(zmapping2z3shadowz36);
        bool zgaz343;
        zgaz343 = sailgen_bool_bits_backwards_infallible(zmapping3z3shadowz35);
        bool zgaz344;
        zgaz344 = sailgen_bool_bits_backwards_infallible(zmapping4z3shadowz34);
        bool zgaz345;
        zgaz345 = sailgen_bool_bits_backwards_infallible(zmapping5z3shadowz33);
        bool zgaz346;
        zgaz346 = sailgen_bool_bits_backwards_infallible(zmapping6z3shadowz32);
        zgaz347.ztup0 = zgaz340;
        zgaz347.ztup1 = zgaz341;
        zgaz347.ztup2 = zgaz342;
        zgaz347.ztup3 = zgaz343;
        zgaz347.ztup4 = zgaz344;
        zgaz347.ztup5 = zgaz345;
        zgaz347.ztup6 = zgaz346;
      }
      struct zoptionzIz8RArchPermszCEExecutionModez5z9zK zgsz394;
      CREATE(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zgsz394);
      {
        enum zExecutionMode zM;
        zM = zgaz347.ztup0;
        bool zLM;
        zLM = zgaz347.ztup1;
        bool zASR;
        zASR = zgaz347.ztup2;
        bool zX;
        zX = zgaz347.ztup3;
        bool zR;
        zR = zgaz347.ztup4;
        bool zW;
        zW = zgaz347.ztup5;
        bool zC;
        zC = zgaz347.ztup6;
        bool zgaz355;
        {
          bool zgaz348;
          zgaz348 = not(zASR);
          bool zgsz396;
          if (zgaz348) {  zgsz396 = true;  } else {  zgsz396 = zX;  }
          zgaz355 = zgsz396;
        }
        bool zgsz3103;
        if (zgaz355) {
          bool zgaz354;
          {
            bool zgaz349;
            zgaz349 = not(zC);
            bool zgsz398;
            if (zgaz349) {  zgsz398 = true;  } else {
              bool zgsz397;
              if (zR) {  zgsz397 = true;  } else {  zgsz397 = zW;  }
              zgsz398 = zgsz397;
            }
            zgaz354 = zgsz398;
          }
          bool zgsz3102;
          if (zgaz354) {
            bool zgaz353;
            {
              bool zgaz351;
              {
                enum zExecutionMode zgaz350;
                zgaz350 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
                zgaz351 = eq_zExecutionMode(zM, zgaz350);
              }
              bool zgsz399;
              if (zgaz351) {  zgsz399 = true;  } else {  zgsz399 = zX;  }
              zgaz353 = zgsz399;
            }
            bool zgsz3101;
            if (zgaz353) {
              bool zgaz352;
              zgaz352 = not(zLM);
              bool zgsz3100;
              if (zgaz352) {  zgsz3100 = true;  } else {  zgsz3100 = zC;  }
              zgsz3101 = zgsz3100;
            } else {  zgsz3101 = false;  }
            zgsz3102 = zgsz3101;
          } else {  zgsz3102 = false;  }
          zgsz3103 = zgsz3102;
        } else {  zgsz3103 = false;  }
        bool zgsz3106;
        zgsz3106 = zgsz3103;
        if (!(zgsz3106)) {






          goto case_190;
        }
        struct zArchPerms zghz370;
        zghz370.zASR = zASR;
        zghz370.zC = zC;
        zghz370.zLM = zLM;
        zghz370.zR = zR;
        zghz370.zW = zW;
        zghz370.zX = zX;
        struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz3105;
        zgsz3105.ztup0 = zghz370;
        zgsz3105.ztup1 = zM;
        sailgen_SomezIz8RArchPermszCEExecutionModez5z9zK(&zgsz394, zgsz3105);
        goto finish_match_188;
      }
    case_190: ;
      {
        sailgen_NonezIz8RArchPermszCEExecutionModez5z9zK(&zgsz394, UNIT);
        goto finish_match_188;
      }
    case_189: ;
    finish_match_188: ;
      COPY(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zgaz364, zgsz394);
      KILL(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zgsz394);
      goto finish_match_185;
    }
  case_187: ;
    {
      sailgen_NonezIz8RArchPermszCEExecutionModez5z9zK(&zgaz364, UNIT);
      goto finish_match_185;
    }
  case_186: ;
  finish_match_185: ;
  }
  struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz3108;
  {
    if (zgaz364.kind != Kind_zSomezIz8RArchPermszCEExecutionModez5z9zK) goto case_192;
    zgsz3108 = zgaz364.zSomezIz8RArchPermszCEExecutionModez5z9zK;
    goto finish_match_191;
  }
case_192: ;
  sail_match_failure("ap_m_encdec_forwards_infallible");
finish_match_191: ;
  zcbz345 = zgsz3108;

  KILL(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zgaz364);

end_function_193: ;
  return zcbz345;
end_block_exception_194: ;
  struct zArchPerms zcbz397 = { .zASR = false, .zC = false, .zLM = false, .zR = false, .zW = false, .zX = false };
  struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zcbz396 = { .ztup0 = zcbz397, .ztup1 = zCapPtrMode };
  return zcbz396;
}



static void finish_sailgen_ap_m_encdec_forwards_infallible(void)
{
}

static uint64_t sailgen_ap_m_encdec_backwards(struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zargz3)
{
  __label__ case_197, finish_match_196, end_function_198, end_block_exception_199;

  uint64_t zcbz346;
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
    bool zASR;
    zASR = zargz3.ztup0.zASR;
    enum zExecutionMode zM;
    zM = zargz3.ztup1;
    bool zgaz385;
    {
      bool zgaz378;
      zgaz378 = not(zASR);
      bool zgsz3111;
      if (zgaz378) {  zgsz3111 = true;  } else {  zgsz3111 = zX;  }
      zgaz385 = zgsz3111;
    }
    bool zgsz3118;
    if (zgaz385) {
      bool zgaz384;
      {
        bool zgaz379;
        zgaz379 = not(zC);
        bool zgsz3113;
        if (zgaz379) {  zgsz3113 = true;  } else {
          bool zgsz3112;
          if (zR) {  zgsz3112 = true;  } else {  zgsz3112 = zW;  }
          zgsz3113 = zgsz3112;
        }
        zgaz384 = zgsz3113;
      }
      bool zgsz3117;
      if (zgaz384) {
        bool zgaz383;
        {
          bool zgaz381;
          {
            enum zExecutionMode zgaz380;
            zgaz380 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
            zgaz381 = eq_zExecutionMode(zM, zgaz380);
          }
          bool zgsz3114;
          if (zgaz381) {  zgsz3114 = true;  } else {  zgsz3114 = zX;  }
          zgaz383 = zgsz3114;
        }
        bool zgsz3116;
        if (zgaz383) {
          bool zgaz382;
          zgaz382 = not(zLM);
          bool zgsz3115;
          if (zgaz382) {  zgsz3115 = true;  } else {  zgsz3115 = zC;  }
          zgsz3116 = zgsz3115;
        } else {  zgsz3116 = false;  }
        zgsz3117 = zgsz3116;
      } else {  zgsz3117 = false;  }
      zgsz3118 = zgsz3117;
    } else {  zgsz3118 = false;  }
    bool zgsz3119;
    zgsz3119 = zgsz3118;
    if (!(zgsz3119)) {






      goto case_197;
    }
    uint64_t zgaz376;
    zgaz376 = sailgen_execution_mode_encdec_forwards(zM);
    uint64_t zgaz377;
    {
      uint64_t zgaz375;
      {
        uint64_t zgaz373;
        zgaz373 = sailgen_bool_bits_forwards(zLM);
        uint64_t zgaz374;
        {
          uint64_t zgaz371;
          zgaz371 = sailgen_bool_bits_forwards(zASR);
          uint64_t zgaz372;
          {
            uint64_t zgaz369;
            zgaz369 = sailgen_bool_bits_forwards(zX);
            uint64_t zgaz370;
            {
              uint64_t zgaz367;
              zgaz367 = sailgen_bool_bits_forwards(zR);
              uint64_t zgaz368;
              {
                uint64_t zgaz365;
                zgaz365 = sailgen_bool_bits_forwards(zW);
                uint64_t zgaz366;
                zgaz366 = sailgen_bool_bits_forwards(zC);
                zgaz368 = (zgaz365 << 1) | zgaz366;
              }
              zgaz370 = (zgaz367 << 2) | zgaz368;
            }
            zgaz372 = (zgaz369 << 3) | zgaz370;
          }
          zgaz374 = (zgaz371 << 4) | zgaz372;
        }
        zgaz375 = (zgaz373 << 5) | zgaz374;
      }
      zgaz377 = (UINT64_C(0b00) << 6) | zgaz375;
    }
    zcbz346 = (zgaz376 << 8) | zgaz377;
    goto finish_match_196;
  }
case_197: ;
  sail_match_failure("ap_m_encdec_backwards");
finish_match_196: ;
end_function_198: ;
  return zcbz346;
end_block_exception_199: ;

  return UINT64_C(0xdeadc0de);
}

static uint64_t sailgen_ap_m_encdec_backwards_infallible(struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9);

static uint64_t sailgen_ap_m_encdec_backwards_infallible(struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zargz3)
{
  __label__ case_201, finish_match_200, end_function_202, end_block_exception_203;

  uint64_t zcbz347;
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
    bool zASR;
    zASR = zargz3.ztup0.zASR;
    enum zExecutionMode zM;
    zM = zargz3.ztup1;
    bool zgaz385;
    {
      bool zgaz378;
      zgaz378 = not(zASR);
      bool zgsz3121;
      if (zgaz378) {  zgsz3121 = true;  } else {  zgsz3121 = zX;  }
      zgaz385 = zgsz3121;
    }
    bool zgsz3128;
    if (zgaz385) {
      bool zgaz384;
      {
        bool zgaz379;
        zgaz379 = not(zC);
        bool zgsz3123;
        if (zgaz379) {  zgsz3123 = true;  } else {
          bool zgsz3122;
          if (zR) {  zgsz3122 = true;  } else {  zgsz3122 = zW;  }
          zgsz3123 = zgsz3122;
        }
        zgaz384 = zgsz3123;
      }
      bool zgsz3127;
      if (zgaz384) {
        bool zgaz383;
        {
          bool zgaz381;
          {
            enum zExecutionMode zgaz380;
            zgaz380 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
            zgaz381 = eq_zExecutionMode(zM, zgaz380);
          }
          bool zgsz3124;
          if (zgaz381) {  zgsz3124 = true;  } else {  zgsz3124 = zX;  }
          zgaz383 = zgsz3124;
        }
        bool zgsz3126;
        if (zgaz383) {
          bool zgaz382;
          zgaz382 = not(zLM);
          bool zgsz3125;
          if (zgaz382) {  zgsz3125 = true;  } else {  zgsz3125 = zC;  }
          zgsz3126 = zgsz3125;
        } else {  zgsz3126 = false;  }
        zgsz3127 = zgsz3126;
      } else {  zgsz3127 = false;  }
      zgsz3128 = zgsz3127;
    } else {  zgsz3128 = false;  }
    bool zgsz3129;
    zgsz3129 = zgsz3128;
    if (!(zgsz3129)) {






      goto case_201;
    }
    uint64_t zgaz376;
    zgaz376 = sailgen_execution_mode_encdec_forwards(zM);
    uint64_t zgaz377;
    {
      uint64_t zgaz375;
      {
        uint64_t zgaz373;
        zgaz373 = sailgen_bool_bits_forwards(zLM);
        uint64_t zgaz374;
        {
          uint64_t zgaz371;
          zgaz371 = sailgen_bool_bits_forwards(zASR);
          uint64_t zgaz372;
          {
            uint64_t zgaz369;
            zgaz369 = sailgen_bool_bits_forwards(zX);
            uint64_t zgaz370;
            {
              uint64_t zgaz367;
              zgaz367 = sailgen_bool_bits_forwards(zR);
              uint64_t zgaz368;
              {
                uint64_t zgaz365;
                zgaz365 = sailgen_bool_bits_forwards(zW);
                uint64_t zgaz366;
                zgaz366 = sailgen_bool_bits_forwards(zC);
                zgaz368 = (zgaz365 << 1) | zgaz366;
              }
              zgaz370 = (zgaz367 << 2) | zgaz368;
            }
            zgaz372 = (zgaz369 << 3) | zgaz370;
          }
          zgaz374 = (zgaz371 << 4) | zgaz372;
        }
        zgaz375 = (zgaz373 << 5) | zgaz374;
      }
      zgaz377 = (UINT64_C(0b00) << 6) | zgaz375;
    }
    zcbz347 = (zgaz376 << 8) | zgaz377;
    goto finish_match_200;
  }
case_201: ;
finish_match_200: ;
end_function_202: ;
  return zcbz347;
end_block_exception_203: ;

  return UINT64_C(0xdeadc0de);
}

static bool sailgen_ap_m_encdec_forwards_matches(uint64_t zargz3)
{
  __label__ case_215, case_212, finish_match_211, end_function_216, end_block_exception_217;

  bool zcbz348;
  uint64_t zhead_expz3;
  zhead_expz3 = zargz3;
  struct zoptionzIozK zgaz3110;
  CREATE(zoptionzIozK)(&zgaz3110);
  {
    __label__ case_207, case_206, finish_match_205;

    {
      __label__ case_210, case_209, finish_match_208;

      uint64_t zv__2;
      zv__2 = zhead_expz3;
      bool zgaz3109;
      {
        uint64_t zmapping0z3;
        zmapping0z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__2 >> INT64_C(8)));
        uint64_t zmapping6z3;
        zmapping6z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__2 >> INT64_C(0)));
        uint64_t zmapping5z3;
        zmapping5z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__2 >> INT64_C(1)));
        uint64_t zmapping4z3;
        zmapping4z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__2 >> INT64_C(2)));
        uint64_t zmapping3z3;
        zmapping3z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__2 >> INT64_C(3)));
        uint64_t zmapping2z3;
        zmapping2z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__2 >> INT64_C(4)));
        uint64_t zmapping1z3;
        zmapping1z3 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__2 >> INT64_C(5)));
        uint64_t zmapping0z3shadowz39;
        zmapping0z3shadowz39 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__2 >> INT64_C(8)));
        bool zgaz3107;
        zgaz3107 = sailgen_execution_mode_encdec_backwards_matches(zmapping0z3shadowz39);
        bool zgsz3137;
        if (zgaz3107) {
          bool zgaz3106;
          zgaz3106 = sailgen_bool_bits_backwards_matches(zmapping1z3);
          bool zgsz3136;
          if (zgaz3106) {
            bool zgaz3105;
            zgaz3105 = sailgen_bool_bits_backwards_matches(zmapping2z3);
            bool zgsz3135;
            if (zgaz3105) {
              bool zgaz3104;
              zgaz3104 = sailgen_bool_bits_backwards_matches(zmapping3z3);
              bool zgsz3134;
              if (zgaz3104) {
                bool zgaz3103;
                zgaz3103 = sailgen_bool_bits_backwards_matches(zmapping4z3);
                bool zgsz3133;
                if (zgaz3103) {
                  bool zgaz3102;
                  zgaz3102 = sailgen_bool_bits_backwards_matches(zmapping5z3);
                  bool zgsz3132;
                  if (zgaz3102) {  zgsz3132 = sailgen_bool_bits_backwards_matches(zmapping6z3);  } else {
                    zgsz3132 = false;
                  }
                  zgsz3133 = zgsz3132;
                } else {  zgsz3133 = false;  }
                zgsz3134 = zgsz3133;
              } else {  zgsz3134 = false;  }
              zgsz3135 = zgsz3134;
            } else {  zgsz3135 = false;  }
            zgsz3136 = zgsz3135;
          } else {  zgsz3136 = false;  }
          zgsz3137 = zgsz3136;
        } else {  zgsz3137 = false;  }
        zgaz3109 = zgsz3137;
      }
      bool zgsz3138;
      if (zgaz3109) {
        uint64_t zgaz3108;
        zgaz3108 = (safe_rshift(UINT64_MAX, 64 - 2) & (zv__2 >> INT64_C(6)));
        zgsz3138 = (zgaz3108 == UINT64_C(0b00));
      } else {  zgsz3138 = false;  }
      bool zgsz3151;
      zgsz3151 = zgsz3138;
      if (!(zgsz3151)) {

        goto case_207;
      }
      uint64_t zmapping0z3shadowz310;
      zmapping0z3shadowz310 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__2 >> INT64_C(8)));
      uint64_t zmapping6z3shadowz311;
      zmapping6z3shadowz311 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__2 >> INT64_C(0)));
      uint64_t zmapping5z3shadowz312;
      zmapping5z3shadowz312 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__2 >> INT64_C(1)));
      uint64_t zmapping4z3shadowz313;
      zmapping4z3shadowz313 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__2 >> INT64_C(2)));
      uint64_t zmapping3z3shadowz314;
      zmapping3z3shadowz314 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__2 >> INT64_C(3)));
      uint64_t zmapping2z3shadowz315;
      zmapping2z3shadowz315 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__2 >> INT64_C(4)));
      uint64_t zmapping1z3shadowz316;
      zmapping1z3shadowz316 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__2 >> INT64_C(5)));
      uint64_t zmapping0z3shadowz317;
      zmapping0z3shadowz317 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__2 >> INT64_C(8)));
      struct ztuple_z8z5enumz0zzExecutionModezCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolzCz0z5boolz9 zgaz393;
      {
        enum zExecutionMode zgaz386;
        zgaz386 = sailgen_execution_mode_encdec_backwards_infallible(zmapping0z3shadowz317);
        bool zgaz387;
        zgaz387 = sailgen_bool_bits_backwards_infallible(zmapping1z3shadowz316);
        bool zgaz388;
        zgaz388 = sailgen_bool_bits_backwards_infallible(zmapping2z3shadowz315);
        bool zgaz389;
        zgaz389 = sailgen_bool_bits_backwards_infallible(zmapping3z3shadowz314);
        bool zgaz390;
        zgaz390 = sailgen_bool_bits_backwards_infallible(zmapping4z3shadowz313);
        bool zgaz391;
        zgaz391 = sailgen_bool_bits_backwards_infallible(zmapping5z3shadowz312);
        bool zgaz392;
        zgaz392 = sailgen_bool_bits_backwards_infallible(zmapping6z3shadowz311);
        zgaz393.ztup0 = zgaz386;
        zgaz393.ztup1 = zgaz387;
        zgaz393.ztup2 = zgaz388;
        zgaz393.ztup3 = zgaz389;
        zgaz393.ztup4 = zgaz390;
        zgaz393.ztup5 = zgaz391;
        zgaz393.ztup6 = zgaz392;
      }
      struct zoptionzIozK zgsz3140;
      CREATE(zoptionzIozK)(&zgsz3140);
      {
        enum zExecutionMode zM;
        zM = zgaz393.ztup0;
        bool zLM;
        zLM = zgaz393.ztup1;
        bool zASR;
        zASR = zgaz393.ztup2;
        bool zX;
        zX = zgaz393.ztup3;
        bool zR;
        zR = zgaz393.ztup4;
        bool zW;
        zW = zgaz393.ztup5;
        bool zC;
        zC = zgaz393.ztup6;
        bool zgaz3101;
        {
          bool zgaz394;
          zgaz394 = not(zASR);
          bool zgsz3142;
          if (zgaz394) {  zgsz3142 = true;  } else {  zgsz3142 = zX;  }
          zgaz3101 = zgsz3142;
        }
        bool zgsz3149;
        if (zgaz3101) {
          bool zgaz3100;
          {
            bool zgaz395;
            zgaz395 = not(zC);
            bool zgsz3144;
            if (zgaz395) {  zgsz3144 = true;  } else {
              bool zgsz3143;
              if (zR) {  zgsz3143 = true;  } else {  zgsz3143 = zW;  }
              zgsz3144 = zgsz3143;
            }
            zgaz3100 = zgsz3144;
          }
          bool zgsz3148;
          if (zgaz3100) {
            bool zgaz399;
            {
              bool zgaz397;
              {
                enum zExecutionMode zgaz396;
                zgaz396 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
                zgaz397 = eq_zExecutionMode(zM, zgaz396);
              }
              bool zgsz3145;
              if (zgaz397) {  zgsz3145 = true;  } else {  zgsz3145 = zX;  }
              zgaz399 = zgsz3145;
            }
            bool zgsz3147;
            if (zgaz399) {
              bool zgaz398;
              zgaz398 = not(zLM);
              bool zgsz3146;
              if (zgaz398) {  zgsz3146 = true;  } else {  zgsz3146 = zC;  }
              zgsz3147 = zgsz3146;
            } else {  zgsz3147 = false;  }
            zgsz3148 = zgsz3147;
          } else {  zgsz3148 = false;  }
          zgsz3149 = zgsz3148;
        } else {  zgsz3149 = false;  }
        bool zgsz3150;
        zgsz3150 = zgsz3149;
        if (!(zgsz3150)) {






          goto case_210;
        }
        sailgen_SomezIozK(&zgsz3140, true);
        goto finish_match_208;
      }
    case_210: ;
      {
        sailgen_NonezIozK(&zgsz3140, UNIT);
        goto finish_match_208;
      }
    case_209: ;
    finish_match_208: ;
      COPY(zoptionzIozK)(&zgaz3110, zgsz3140);
      KILL(zoptionzIozK)(&zgsz3140);
      goto finish_match_205;
    }
  case_207: ;
    {
      sailgen_NonezIozK(&zgaz3110, UNIT);
      goto finish_match_205;
    }
  case_206: ;
  finish_match_205: ;
  }
  bool zgsz3152;
  {
    if (zgaz3110.kind != Kind_zSomezIozK) goto case_215;
    zgsz3152 = zgaz3110.zSomezIozK;
    goto finish_match_211;
  }
case_215: ;
  {
    __label__ case_214, finish_match_213;

    /* complete */
    bool zgsz3153;
    {
      zgsz3153 = false;
      goto finish_match_213;
    }
  case_214: ;
  finish_match_213: ;
    zgsz3152 = zgsz3153;
    goto finish_match_211;
  }
case_212: ;
finish_match_211: ;
  zcbz348 = zgsz3152;

  KILL(zoptionzIozK)(&zgaz3110);

end_function_216: ;
  return zcbz348;
end_block_exception_217: ;

  return false;
}

static uint64_t znull_ap_m;
static void create_letbind_14(void) {


  uint64_t zgsz3157;
  zgsz3157 = UINT64_C(0b000000000);
  znull_ap_m = zgsz3157;

let_end_218: ;
}
static void kill_letbind_14(void) {
}

static uint64_t zinfinite_ap_m;
static void create_letbind_15(void) {


  uint64_t zgsz3158;
  zgsz3158 = UINT64_C(0b100111111);
  zinfinite_ap_m = zgsz3158;

let_end_219: ;
}
static void kill_letbind_15(void) {
}

static struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 sailgen_legalizzePermsAndMode(struct zArchPerms, enum zExecutionMode);

struct zArchPerms zghz371;
struct zArchPerms zghz372;
struct zArchPerms zghz373;
struct zArchPerms zghz374;
struct zArchPerms zghz375;
struct zArchPerms zghz376;

static void startup_sailgen_legalizzePermsAndMode(void)
{






}

static struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 sailgen_legalizzePermsAndMode(struct zArchPerms zperms, enum zExecutionMode zmode)
{
  __label__ end_function_221, end_block_exception_222;

  struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zcbz349;
  struct zArchPerms zghz371;
  {
    bool zgaz3115;
    zgaz3115 = zperms.zX;
    if (zgaz3115) {  zghz371 = zperms;  } else {
      struct zArchPerms zghz376;
      zghz376 = zperms;
      zghz376.zASR = false;
      zghz371 = zghz376;
    }
  }
  struct zArchPerms zghz372;
  {
    bool zgaz3114;
    {
      bool zgaz3113;
      zgaz3113 = zghz371.zR;
      bool zgsz3160;
      if (zgaz3113) {  zgsz3160 = true;  } else {  zgsz3160 = zghz371.zW;  }
      zgaz3114 = zgsz3160;
    }
    if (zgaz3114) {  zghz372 = zghz371;  } else {
      struct zArchPerms zghz375;
      zghz375 = zghz371;
      zghz375.zC = false;
      zghz372 = zghz375;
    }
  }
  enum zExecutionMode zmodeshadowz320;
  {
    bool zgaz3112;
    zgaz3112 = zghz372.zX;
    if (zgaz3112) {  zmodeshadowz320 = zmode;  } else {
      zmodeshadowz320 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    }
  }
  struct zArchPerms zghz373;
  {
    bool zgaz3111;
    zgaz3111 = zghz372.zC;
    if (zgaz3111) {  zghz373 = zghz372;  } else {
      struct zArchPerms zghz374;
      zghz374 = zghz372;
      zghz374.zLM = false;
      zghz373 = zghz374;
    }
  }
  zcbz349.ztup0 = zghz373;
  zcbz349.ztup1 = zmodeshadowz320;

end_function_221: ;
  return zcbz349;
end_block_exception_222: ;
  struct zArchPerms zcbz399 = { .zASR = false, .zC = false, .zLM = false, .zR = false, .zW = false, .zX = false };
  struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zcbz398 = { .ztup0 = zcbz399, .ztup1 = zCapPtrMode };
  return zcbz398;
}



static void finish_sailgen_legalizzePermsAndMode(void)
{






}

static uint64_t sailgen_getMetadataL8(struct zEncMetadata);

static uint64_t sailgen_getMetadataL8(struct zEncMetadata zmeta)
{
  __label__ end_function_224, end_block_exception_225;

  uint64_t zcbz350;
  zcbz350 = UINT64_C(0b0);
end_function_224: ;
  return zcbz350;
end_block_exception_225: ;

  return UINT64_C(0xdeadc0de);
}

static struct zEncMetadata sailgen_setMetadataL8(struct zEncMetadata, uint64_t);

struct zEncMetadata zghz377;

static void startup_sailgen_setMetadataL8(void)
{
}

static struct zEncMetadata sailgen_setMetadataL8(struct zEncMetadata zmeta, uint64_t zL8)
{
  __label__ end_function_227, end_block_exception_228;

  struct zEncMetadata zghz377;
  zghz377 = zmeta;
end_function_227: ;
  return zghz377;
end_block_exception_228: ;
  struct zEncMetadata zcbz3100 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz3100;
}



static void finish_sailgen_setMetadataL8(void)
{
}

static uint64_t sailgen_getMetadataInternalExponent(struct zEncMetadata);

static uint64_t sailgen_getMetadataInternalExponent(struct zEncMetadata zmeta)
{
  __label__ end_function_230, end_block_exception_231;

  uint64_t zcbz352;
  uint64_t zgaz3116;
  zgaz3116 = sailgen__get_EncMetadata_TE(zmeta);
  uint64_t zgaz3117;
  zgaz3117 = sailgen__get_EncMetadata_BE(zmeta);
  zcbz352 = (zgaz3116 << 3) | zgaz3117;


end_function_230: ;
  return zcbz352;
end_block_exception_231: ;

  return UINT64_C(0xdeadc0de);
}

static int64_t zcap_sizze;
static void create_letbind_16(void) {


  int64_t zgsz3164;
  zgsz3164 = INT64_C(16);
  zcap_sizze = zgsz3164;

let_end_232: ;
}
static void kill_letbind_16(void) {
}

static int64_t zcap_addr_width;
static void create_letbind_17(void) {


  int64_t zgsz3165;
  zgsz3165 = INT64_C(64);
  zcap_addr_width = zgsz3165;

let_end_233: ;
}
static void kill_letbind_17(void) {
}

static int64_t zcap_len_width;
static void create_letbind_18(void) {


  int64_t zgsz3166;
  zgsz3166 = INT64_C(65);
  zcap_len_width = zgsz3166;

let_end_234: ;
}
static void kill_letbind_18(void) {
}

static sail_int zcap_max_addr;
static void create_letbind_19(void) {    CREATE(sail_int)(&zcap_max_addr);


  sail_int zgsz3167;
  CREATE(sail_int)(&zgsz3167);
  {
    sail_int zgsz3368;
    CREATE(sail_int)(&zgsz3368);
    CONVERT_OF(sail_int, mach_int)(&zgsz3368, zcap_addr_width);
    sailgen_MAX(&zgsz3167, zgsz3368);
    KILL(sail_int)(&zgsz3368);
  }
  COPY(sail_int)(&zcap_max_addr, zgsz3167);
  KILL(sail_int)(&zgsz3167);
let_end_235: ;
}
static void kill_letbind_19(void) {    KILL(sail_int)(&zcap_max_addr);
}

static int64_t zcap_xe_width;
static void create_letbind_20(void) {


  int64_t zgsz3168;
  zgsz3168 = INT64_C(3);
  zcap_xe_width = zgsz3168;

let_end_236: ;
}
static void kill_letbind_20(void) {
}

static int64_t zcap_max_E;
static void create_letbind_21(void) {


  int64_t zgsz3169;
  zgsz3169 = INT64_C(52);
  zcap_max_E = zgsz3169;

let_end_237: ;
}
static void kill_letbind_21(void) {
}

static int64_t zcap_min_E;
static void create_letbind_22(void) {


  int64_t zgsz3170;
  zgsz3170 = INT64_C(-11);
  zcap_min_E = zgsz3170;

let_end_238: ;
}
static void kill_letbind_22(void) {
}

static uint64_t sailgen_internal_exponent_encdec_forwards(bool);

static bool sailgen_internal_exponent_encdec_backwards(uint64_t);

static uint64_t sailgen_internal_exponent_encdec_forwards(bool zargz3)
{
  __label__ case_242, case_241, finish_match_240, end_function_243, end_block_exception_244;

  uint64_t zcbz353;
  {
    bool zp0z3;
    zp0z3 = zargz3;
    bool zgsz3173;
    zgsz3173 = eq_bool(zp0z3, false);
    if (!(zgsz3173)) {

      goto case_242;
    }
    zcbz353 = UINT64_C(0b1);
    goto finish_match_240;
  }
case_242: ;
  {
    bool zuz32;
    zuz32 = zargz3;
    bool zgsz3172;
    zgsz3172 = eq_bool(zuz32, true);
    if (!(zgsz3172)) {

      goto case_241;
    }
    zcbz353 = UINT64_C(0b0);
    goto finish_match_240;
  }
case_241: ;
finish_match_240: ;
end_function_243: ;
  return zcbz353;
end_block_exception_244: ;

  return UINT64_C(0xdeadc0de);
}

static uint64_t sailgen_internal_exponent_encdec_forwards_infallible(bool);

static uint64_t sailgen_internal_exponent_encdec_forwards_infallible(bool zargz3)
{
  __label__ case_247, case_246, finish_match_245, end_function_248, end_block_exception_249;

  uint64_t zcbz354;
  {
    bool zp0z3;
    zp0z3 = zargz3;
    bool zgsz3176;
    zgsz3176 = eq_bool(zp0z3, false);
    if (!(zgsz3176)) {

      goto case_247;
    }
    zcbz354 = UINT64_C(0b1);
    goto finish_match_245;
  }
case_247: ;
  {
    bool zuz33;
    zuz33 = zargz3;
    bool zgsz3175;
    zgsz3175 = eq_bool(zuz33, true);
    if (!(zgsz3175)) {

      goto case_246;
    }
    zcbz354 = UINT64_C(0b0);
    goto finish_match_245;
  }
case_246: ;
finish_match_245: ;
end_function_248: ;
  return zcbz354;
end_block_exception_249: ;

  return UINT64_C(0xdeadc0de);
}

static bool sailgen_internal_exponent_encdec_backwards(uint64_t zargz3)
{
  __label__ case_253, case_252, finish_match_251, end_function_254, end_block_exception_255;

  bool zcbz355;
  {
    uint64_t zb__0;
    zb__0 = zargz3;
    bool zgsz3179;
    zgsz3179 = (zb__0 == UINT64_C(0b1));
    if (!(zgsz3179)) {

      goto case_253;
    }
    zcbz355 = false;
    goto finish_match_251;
  }
case_253: ;
  {
    zcbz355 = true;
    goto finish_match_251;
  }
case_252: ;
finish_match_251: ;
end_function_254: ;
  return zcbz355;
end_block_exception_255: ;

  return false;
}

static bool sailgen_internal_exponent_encdec_backwards_infallible(uint64_t);

static bool sailgen_internal_exponent_encdec_backwards_infallible(uint64_t zargz3)
{
  __label__ case_258, case_257, finish_match_256, end_function_259, end_block_exception_260;

  bool zcbz356;
  {
    uint64_t zb__0;
    zb__0 = zargz3;
    bool zgsz3182;
    zgsz3182 = (zb__0 == UINT64_C(0b1));
    if (!(zgsz3182)) {

      goto case_258;
    }
    zcbz356 = false;
    goto finish_match_256;
  }
case_258: ;
  {
    zcbz356 = true;
    goto finish_match_256;
  }
case_257: ;
finish_match_256: ;
end_function_259: ;
  return zcbz356;
end_block_exception_260: ;

  return false;
}

static struct zCapability sailgen_undefined_Capability(unit);

struct zCapability zghz378;
sail_int zghz379;

static void startup_sailgen_undefined_Capability(void)
{

  CREATE(sail_int)(&zghz379);
}

static struct zCapability sailgen_undefined_Capability(unit zgsz3183)
{
  __label__ end_function_262, end_block_exception_263;

  struct zCapability zghz378;
  zghz378.zB = UINT64_C(0b00000000000000);
  RECREATE(sail_int)(&zghz379);
  CONVERT_OF(sail_int, mach_int)(&zghz379, INT64_C(-11));
  zghz378.zE = CONVERT_OF(mach_int, sail_int)(zghz379);
  zghz378.zT = UINT64_C(0b00000000000000);
  zghz378.zaddress = UINT64_C(0x0000000000000000);
  zghz378.zap_m = UINT64_C(0b000000000);
  zghz378.zinternal_exponent = false;
  zghz378.zreserved_0 = UINT64_C(0x0000);
  zghz378.zreserved_1 = UINT64_C(0b0000000);
  zghz378.zsd_perms = UINT64_C(0x0);
  zghz378.zsealed = false;
  zghz378.ztag = false;
end_function_262: ;
  return zghz378;
end_block_exception_263: ;
  struct zCapability zcbz3101 = { .zB = UINT64_C(0xdeadc0de), .zE = INT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zap_m = UINT64_C(0xdeadc0de), .zinternal_exponent = false, .zreserved_0 = UINT64_C(0xdeadc0de), .zreserved_1 = UINT64_C(0xdeadc0de), .zsd_perms = UINT64_C(0xdeadc0de), .zsealed = false, .ztag = false };
  return zcbz3101;
}



static void finish_sailgen_undefined_Capability(void)
{
  KILL(sail_int)(&zghz379);

}

static struct zCapability sailgen_bitsToCap(bool, lbits);

struct zCapability zghz380;
struct zEncMetadata zghz381;
struct zCapability zghz382;
sail_int zghz383;
sail_int zghz384;
lbits zghz385;
sail_int zghz386;
sail_int zghz387;
sail_int zghz388;
lbits zghz389;
lbits zghz390;
lbits zghz391;
lbits zghz392;
lbits zghz393;
lbits zghz394;
lbits zghz395;
sail_int zghz396;
sail_int zghz397;
lbits zghz398;
sail_int zghz399;
sail_int zghz3100;
sail_int zghz3101;
sail_int zghz3102;
sail_int zghz3103;
sail_int zghz3104;
lbits zghz3105;
lbits zghz3106;
lbits zghz3107;
sail_int zghz3108;
sail_int zghz3109;
lbits zghz3110;
sail_int zghz3111;
sail_int zghz3112;
sail_int zghz3113;
sail_int zghz3114;
sail_int zghz3115;
sail_int zghz3116;
lbits zghz3117;
sail_int zghz3118;
sail_int zghz3119;
sail_int zghz3120;
lbits zghz3121;
sail_int zghz3122;
sail_int zghz3123;
sail_int zghz3124;
sail_int zghz3125;
sail_int zghz3126;
sail_int zghz3127;

static void startup_sailgen_bitsToCap(void)
{



  CREATE(sail_int)(&zghz383);
  CREATE(sail_int)(&zghz384);
  CREATE(lbits)(&zghz385);
  CREATE(sail_int)(&zghz386);
  CREATE(sail_int)(&zghz387);
  CREATE(sail_int)(&zghz388);
  CREATE(lbits)(&zghz389);
  CREATE(lbits)(&zghz390);
  CREATE(lbits)(&zghz391);
  CREATE(lbits)(&zghz392);
  CREATE(lbits)(&zghz393);
  CREATE(lbits)(&zghz394);
  CREATE(lbits)(&zghz395);
  CREATE(sail_int)(&zghz396);
  CREATE(sail_int)(&zghz397);
  CREATE(lbits)(&zghz398);
  CREATE(sail_int)(&zghz399);
  CREATE(sail_int)(&zghz3100);
  CREATE(sail_int)(&zghz3101);
  CREATE(sail_int)(&zghz3102);
  CREATE(sail_int)(&zghz3103);
  CREATE(sail_int)(&zghz3104);
  CREATE(lbits)(&zghz3105);
  CREATE(lbits)(&zghz3106);
  CREATE(lbits)(&zghz3107);
  CREATE(sail_int)(&zghz3108);
  CREATE(sail_int)(&zghz3109);
  CREATE(lbits)(&zghz3110);
  CREATE(sail_int)(&zghz3111);
  CREATE(sail_int)(&zghz3112);
  CREATE(sail_int)(&zghz3113);
  CREATE(sail_int)(&zghz3114);
  CREATE(sail_int)(&zghz3115);
  CREATE(sail_int)(&zghz3116);
  CREATE(lbits)(&zghz3117);
  CREATE(sail_int)(&zghz3118);
  CREATE(sail_int)(&zghz3119);
  CREATE(sail_int)(&zghz3120);
  CREATE(lbits)(&zghz3121);
  CREATE(sail_int)(&zghz3122);
  CREATE(sail_int)(&zghz3123);
  CREATE(sail_int)(&zghz3124);
  CREATE(sail_int)(&zghz3125);
  CREATE(sail_int)(&zghz3126);
  CREATE(sail_int)(&zghz3127);
}

static struct zCapability sailgen_bitsToCap(bool ztag, lbits zcap_bits)
{
  __label__ case_266, finish_match_265, end_function_267, end_block_exception_268;

  struct zCapability zghz380;
  struct zEncMetadata zghz381;
  {
    uint64_t zgaz3144;
    {
      int64_t zgaz3143;
      {
        int64_t zgaz3142;
        {
          RECREATE(sail_int)(&zghz3125);
          CONVERT_OF(sail_int, mach_int)(&zghz3125, zxlen);
          RECREATE(sail_int)(&zghz3126);
          CONVERT_OF(sail_int, mach_int)(&zghz3126, INT64_C(2));
          RECREATE(sail_int)(&zghz3127);
          mult_int(&zghz3127, zghz3125, zghz3126);
          zgaz3142 = CONVERT_OF(mach_int, sail_int)(zghz3127);
        }
        {
          RECREATE(sail_int)(&zghz3122);
          CONVERT_OF(sail_int, mach_int)(&zghz3122, zgaz3142);
          RECREATE(sail_int)(&zghz3123);
          CONVERT_OF(sail_int, mach_int)(&zghz3123, INT64_C(1));
          RECREATE(sail_int)(&zghz3124);
          sub_int(&zghz3124, zghz3122, zghz3123);
          zgaz3143 = CONVERT_OF(mach_int, sail_int)(zghz3124);
        }
      }
      {
        RECREATE(sail_int)(&zghz3119);
        CONVERT_OF(sail_int, mach_int)(&zghz3119, zgaz3143);
        RECREATE(sail_int)(&zghz3120);
        CONVERT_OF(sail_int, mach_int)(&zghz3120, zxlen);
        RECREATE(lbits)(&zghz3121);
        vector_subrange_lbits(&zghz3121, zcap_bits, zghz3119, zghz3120);
        zgaz3144 = CONVERT_OF(fbits, lbits)(zghz3121, true);
      }
    }
    zghz381 = sailgen_Mk_EncMetadata(zgaz3144);
  }
  bool zinternal_exponent;
  {
    uint64_t zgaz3141;
    zgaz3141 = sailgen__get_EncMetadata_EF(zghz381);
    zinternal_exponent = sailgen_internal_exponent_encdec_backwards(zgaz3141);
  }
  {
    int64_t zMW;
    zMW = zcap_mantissa_width;
    int64_t zE;
    if (zinternal_exponent) {
      int64_t zgaz3140;
      {
        uint64_t zgaz3139;
        zgaz3139 = sailgen_getMetadataInternalExponent(zghz381);
        {
          RECREATE(lbits)(&zghz3117);
          CONVERT_OF(lbits, fbits)(&zghz3117, zgaz3139, UINT64_C(6) , true);
          RECREATE(sail_int)(&zghz3118);
          sail_unsigned(&zghz3118, zghz3117);
          zgaz3140 = CONVERT_OF(mach_int, sail_int)(zghz3118);
        }
      }
      {
        RECREATE(sail_int)(&zghz3114);
        CONVERT_OF(sail_int, mach_int)(&zghz3114, zcap_max_E);
        RECREATE(sail_int)(&zghz3115);
        CONVERT_OF(sail_int, mach_int)(&zghz3115, zgaz3140);
        RECREATE(sail_int)(&zghz3116);
        sub_int(&zghz3116, zghz3114, zghz3115);
        zE = CONVERT_OF(mach_int, sail_int)(zghz3116);
      }
    } else {  zE = INT64_C(0);  }
    uint64_t zLMSB;
    if (zinternal_exponent) {  zLMSB = UINT64_C(0b01);  } else {
      uint64_t zgaz3138;
      zgaz3138 = sailgen_getMetadataL8(zghz381);
      zLMSB = (UINT64_C(0b0) << 1) | zgaz3138;
    }
    uint64_t zT_low;
    {
      bool zgaz3137;
      zgaz3137 = not(zinternal_exponent);
      if (zgaz3137) {  zT_low = sailgen__get_EncMetadata_TE(zghz381);  } else {  zT_low = UINT64_C(0b000);  }
    }
    uint64_t zB_low;
    {
      bool zgaz3136;
      zgaz3136 = not(zinternal_exponent);
      if (zgaz3136) {  zB_low = sailgen__get_EncMetadata_BE(zghz381);  } else {  zB_low = UINT64_C(0b000);  }
    }
    sbits zT;
    {
      uint64_t zgaz3135;
      zgaz3135 = sailgen__get_EncMetadata_Tm(zghz381);
      sbits zgsz3187;
      zgsz3187 = CONVERT_OF(sbits, fbits)((zgaz3135 << 3) | zT_low, UINT64_C(12) , true);
      zT = zgsz3187;
    }
    sbits zB;
    {
      uint64_t zgaz3134;
      zgaz3134 = sailgen__get_EncMetadata_Bm(zghz381);
      sbits zgsz3188;
      zgsz3188 = CONVERT_OF(sbits, fbits)((zgaz3134 << 3) | zB_low, UINT64_C(14) , true);
      zB = zgsz3188;
    }
    uint64_t zLCout;
    {
      bool zgaz3133;
      {
        sbits zgaz3132;
        {
          int64_t zgaz3131;
          {
            RECREATE(sail_int)(&zghz3111);
            CONVERT_OF(sail_int, mach_int)(&zghz3111, zMW);
            RECREATE(sail_int)(&zghz3112);
            CONVERT_OF(sail_int, mach_int)(&zghz3112, INT64_C(3));
            RECREATE(sail_int)(&zghz3113);
            sub_int(&zghz3113, zghz3111, zghz3112);
            zgaz3131 = CONVERT_OF(mach_int, sail_int)(zghz3113);
          }
          {
            RECREATE(lbits)(&zghz3107);
            CONVERT_OF(lbits, sbits)(&zghz3107, zB, true);
            RECREATE(sail_int)(&zghz3108);
            CONVERT_OF(sail_int, mach_int)(&zghz3108, zgaz3131);
            RECREATE(sail_int)(&zghz3109);
            CONVERT_OF(sail_int, mach_int)(&zghz3109, INT64_C(0));
            RECREATE(lbits)(&zghz3110);
            vector_subrange_lbits(&zghz3110, zghz3107, zghz3108, zghz3109);
            zgaz3132 = CONVERT_OF(sbits, lbits)(zghz3110, true);
          }
        }
        {
          RECREATE(lbits)(&zghz3105);
          CONVERT_OF(lbits, sbits)(&zghz3105, zT, true);
          RECREATE(lbits)(&zghz3106);
          CONVERT_OF(lbits, sbits)(&zghz3106, zgaz3132, true);
          zgaz3133 = sailgen_z8operatorz0zI_uz9(zghz3105, zghz3106);
        }
      }
      if (zgaz3133) {  zLCout = UINT64_C(0b01);  } else {  zLCout = UINT64_C(0b00);  }
    }
    sbits zTshadowz322;
    {
      sbits zgaz3130;
      {
        sbits zgaz3129;
        {
          sbits zgaz3128;
          {
            int64_t zgaz3126;
            {
              RECREATE(sail_int)(&zghz3102);
              CONVERT_OF(sail_int, mach_int)(&zghz3102, zMW);
              RECREATE(sail_int)(&zghz3103);
              CONVERT_OF(sail_int, mach_int)(&zghz3103, INT64_C(1));
              RECREATE(sail_int)(&zghz3104);
              sub_int(&zghz3104, zghz3102, zghz3103);
              zgaz3126 = CONVERT_OF(mach_int, sail_int)(zghz3104);
            }
            int64_t zgaz3127;
            {
              RECREATE(sail_int)(&zghz399);
              CONVERT_OF(sail_int, mach_int)(&zghz399, zMW);
              RECREATE(sail_int)(&zghz3100);
              CONVERT_OF(sail_int, mach_int)(&zghz3100, INT64_C(2));
              RECREATE(sail_int)(&zghz3101);
              sub_int(&zghz3101, zghz399, zghz3100);
              zgaz3127 = CONVERT_OF(mach_int, sail_int)(zghz3101);
            }
            {
              RECREATE(lbits)(&zghz395);
              CONVERT_OF(lbits, sbits)(&zghz395, zB, true);
              RECREATE(sail_int)(&zghz396);
              CONVERT_OF(sail_int, mach_int)(&zghz396, zgaz3126);
              RECREATE(sail_int)(&zghz397);
              CONVERT_OF(sail_int, mach_int)(&zghz397, zgaz3127);
              RECREATE(lbits)(&zghz398);
              vector_subrange_lbits(&zghz398, zghz395, zghz396, zghz397);
              zgaz3128 = CONVERT_OF(sbits, lbits)(zghz398, true);
            }
          }
          {
            RECREATE(lbits)(&zghz392);
            CONVERT_OF(lbits, sbits)(&zghz392, zgaz3128, true);
            RECREATE(lbits)(&zghz393);
            CONVERT_OF(lbits, fbits)(&zghz393, zLCout, UINT64_C(2) , true);
            RECREATE(lbits)(&zghz394);
            add_bits(&zghz394, zghz392, zghz393);
            zgaz3129 = CONVERT_OF(sbits, lbits)(zghz394, true);
          }
        }
        {
          RECREATE(lbits)(&zghz389);
          CONVERT_OF(lbits, sbits)(&zghz389, zgaz3129, true);
          RECREATE(lbits)(&zghz390);
          CONVERT_OF(lbits, fbits)(&zghz390, zLMSB, UINT64_C(2) , true);
          RECREATE(lbits)(&zghz391);
          add_bits(&zghz391, zghz389, zghz390);
          zgaz3130 = CONVERT_OF(sbits, lbits)(zghz391, true);
        }
      }
      zTshadowz322 = append_ss(zgaz3130, zT);
    }
    uint64_t zgaz3118;
    zgaz3118 = sailgen__get_EncMetadata_SDP(zghz381);
    uint64_t zgaz3119;
    zgaz3119 = sailgen__get_EncMetadata_AP_M(zghz381);
    bool zgaz3121;
    {
      uint64_t zgaz3120;
      zgaz3120 = sailgen__get_EncMetadata_CT(zghz381);
      zgaz3121 = sailgen_bool_bits_backwards(zgaz3120);
    }
    uint64_t zgaz3122;
    zgaz3122 = sailgen__get_EncMetadata_reserved_0(zghz381);
    uint64_t zgaz3123;
    zgaz3123 = sailgen__get_EncMetadata_reserved_1(zghz381);
    uint64_t zgaz3125;
    {
      int64_t zgaz3124;
      {
        RECREATE(sail_int)(&zghz386);
        CONVERT_OF(sail_int, mach_int)(&zghz386, zxlen);
        RECREATE(sail_int)(&zghz387);
        CONVERT_OF(sail_int, mach_int)(&zghz387, INT64_C(1));
        RECREATE(sail_int)(&zghz388);
        sub_int(&zghz388, zghz386, zghz387);
        zgaz3124 = CONVERT_OF(mach_int, sail_int)(zghz388);
      }
      {
        RECREATE(sail_int)(&zghz383);
        CONVERT_OF(sail_int, mach_int)(&zghz383, zgaz3124);
        RECREATE(sail_int)(&zghz384);
        CONVERT_OF(sail_int, mach_int)(&zghz384, INT64_C(0));
        RECREATE(lbits)(&zghz385);
        vector_subrange_lbits(&zghz385, zcap_bits, zghz383, zghz384);
        zgaz3125 = CONVERT_OF(fbits, lbits)(zghz385, true);
      }
    }
    struct zCapability zghz382;
    zghz382.zB = CONVERT_OF(fbits, sbits)(zB, true);
    zghz382.zE = zE;
    zghz382.zT = CONVERT_OF(fbits, sbits)(zTshadowz322, true);
    zghz382.zaddress = zgaz3125;
    zghz382.zap_m = zgaz3119;
    zghz382.zinternal_exponent = zinternal_exponent;
    zghz382.zreserved_0 = zgaz3122;
    zghz382.zreserved_1 = zgaz3123;
    zghz382.zsd_perms = zgaz3118;
    zghz382.zsealed = zgaz3121;
    zghz382.ztag = ztag;
    zghz380 = zghz382;
    goto finish_match_265;
  }
case_266: ;
  sail_match_failure("bitsToCap");
finish_match_265: ;

end_function_267: ;
  return zghz380;
end_block_exception_268: ;
  struct zCapability zcbz3102 = { .zB = UINT64_C(0xdeadc0de), .zE = INT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zap_m = UINT64_C(0xdeadc0de), .zinternal_exponent = false, .zreserved_0 = UINT64_C(0xdeadc0de), .zreserved_1 = UINT64_C(0xdeadc0de), .zsd_perms = UINT64_C(0xdeadc0de), .zsealed = false, .ztag = false };
  return zcbz3102;
}



static void finish_sailgen_bitsToCap(void)
{
  KILL(sail_int)(&zghz3127);
  KILL(sail_int)(&zghz3126);
  KILL(sail_int)(&zghz3125);
  KILL(sail_int)(&zghz3124);
  KILL(sail_int)(&zghz3123);
  KILL(sail_int)(&zghz3122);
  KILL(lbits)(&zghz3121);
  KILL(sail_int)(&zghz3120);
  KILL(sail_int)(&zghz3119);
  KILL(sail_int)(&zghz3118);
  KILL(lbits)(&zghz3117);
  KILL(sail_int)(&zghz3116);
  KILL(sail_int)(&zghz3115);
  KILL(sail_int)(&zghz3114);
  KILL(sail_int)(&zghz3113);
  KILL(sail_int)(&zghz3112);
  KILL(sail_int)(&zghz3111);
  KILL(lbits)(&zghz3110);
  KILL(sail_int)(&zghz3109);
  KILL(sail_int)(&zghz3108);
  KILL(lbits)(&zghz3107);
  KILL(lbits)(&zghz3106);
  KILL(lbits)(&zghz3105);
  KILL(sail_int)(&zghz3104);
  KILL(sail_int)(&zghz3103);
  KILL(sail_int)(&zghz3102);
  KILL(sail_int)(&zghz3101);
  KILL(sail_int)(&zghz3100);
  KILL(sail_int)(&zghz399);
  KILL(lbits)(&zghz398);
  KILL(sail_int)(&zghz397);
  KILL(sail_int)(&zghz396);
  KILL(lbits)(&zghz395);
  KILL(lbits)(&zghz394);
  KILL(lbits)(&zghz393);
  KILL(lbits)(&zghz392);
  KILL(lbits)(&zghz391);
  KILL(lbits)(&zghz390);
  KILL(lbits)(&zghz389);
  KILL(sail_int)(&zghz388);
  KILL(sail_int)(&zghz387);
  KILL(sail_int)(&zghz386);
  KILL(lbits)(&zghz385);
  KILL(sail_int)(&zghz384);
  KILL(sail_int)(&zghz383);



}

static struct zEncMetadata sailgen_capToMetadataBits(struct zCapability);

struct zEncMetadata zghz3128;
struct zEncMetadata zghz3129;
struct zEncMetadata zghz3130;
struct zEncMetadata zghz3131;
struct zEncMetadata zghz3132;
struct zEncMetadata zghz3133;
struct zEncMetadata zghz3134;
sail_int zghz3135;
sail_int zghz3136;
sail_int zghz3137;
struct zEncMetadata zghz3138;
sail_int zghz3139;
sail_int zghz3140;
sail_int zghz3141;
struct zEncMetadata zghz3142;
struct zEncMetadata zghz3143;
struct zEncMetadata zghz3144;
struct zEncMetadata zghz3145;
sail_int zghz3146;
sail_int zghz3147;
sail_int zghz3148;
sail_int zghz3149;
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
sail_int zghz3161;
sail_int zghz3162;
sail_int zghz3163;
sail_int zghz3164;
sail_int zghz3165;
lbits zghz3166;
sail_int zghz3167;
sail_int zghz3168;
sail_int zghz3169;

static void startup_sailgen_capToMetadataBits(void)
{







  CREATE(sail_int)(&zghz3135);
  CREATE(sail_int)(&zghz3136);
  CREATE(sail_int)(&zghz3137);

  CREATE(sail_int)(&zghz3139);
  CREATE(sail_int)(&zghz3140);
  CREATE(sail_int)(&zghz3141);




  CREATE(sail_int)(&zghz3146);
  CREATE(sail_int)(&zghz3147);
  CREATE(sail_int)(&zghz3148);
  CREATE(sail_int)(&zghz3149);
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
  CREATE(sail_int)(&zghz3161);
  CREATE(sail_int)(&zghz3162);
  CREATE(sail_int)(&zghz3163);
  CREATE(sail_int)(&zghz3164);
  CREATE(sail_int)(&zghz3165);
  CREATE(lbits)(&zghz3166);
  CREATE(sail_int)(&zghz3167);
  CREATE(sail_int)(&zghz3168);
  CREATE(sail_int)(&zghz3169);
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
  __label__ case_271, finish_match_270, end_function_272, end_block_exception_273;

  struct zEncMetadata zghz3128;
  uint64_t zE;
  {
    int64_t zgaz3188;
    {
      int64_t zgaz3187;
      zgaz3187 = zcap.zE;
      {
        RECREATE(sail_int)(&zghz3167);
        CONVERT_OF(sail_int, mach_int)(&zghz3167, zcap_max_E);
        RECREATE(sail_int)(&zghz3168);
        CONVERT_OF(sail_int, mach_int)(&zghz3168, zgaz3187);
        RECREATE(sail_int)(&zghz3169);
        sub_int(&zghz3169, zghz3167, zghz3168);
        zgaz3188 = CONVERT_OF(mach_int, sail_int)(zghz3169);
      }
    }
    {
      RECREATE(sail_int)(&zghz3164);
      CONVERT_OF(sail_int, mach_int)(&zghz3164, zcap_E_width);
      RECREATE(sail_int)(&zghz3165);
      CONVERT_OF(sail_int, mach_int)(&zghz3165, zgaz3188);
      RECREATE(lbits)(&zghz3166);
      sailgen_to_bits(&zghz3166, zghz3164, zghz3165);
      zE = CONVERT_OF(fbits, lbits)(zghz3166, true);
    }
  }
  struct ztuple_z8z5bitzCz0z5bv3zCz0z5bv3z9 zgaz3163;
  {
    bool zgaz3145;
    zgaz3145 = zcap.zinternal_exponent;
    if (zgaz3145) {
      fbits zgaz3150;
      {
        int64_t zgaz3146;
        {
          RECREATE(sail_int)(&zghz3161);
          CONVERT_OF(sail_int, mach_int)(&zghz3161, zcap_E_width);
          RECREATE(sail_int)(&zghz3162);
          CONVERT_OF(sail_int, mach_int)(&zghz3162, INT64_C(1));
          RECREATE(sail_int)(&zghz3163);
          sub_int(&zghz3163, zghz3161, zghz3162);
          zgaz3146 = CONVERT_OF(mach_int, sail_int)(zghz3163);
        }
        zgaz3150 = (UINT64_C(1) & (zE >> zgaz3146));
      }
      uint64_t zgaz3151;
      {
        int64_t zgaz3148;
        {
          int64_t zgaz3147;
          {
            RECREATE(sail_int)(&zghz3158);
            CONVERT_OF(sail_int, mach_int)(&zghz3158, zcap_xe_width);
            RECREATE(sail_int)(&zghz3159);
            CONVERT_OF(sail_int, mach_int)(&zghz3159, INT64_C(2));
            RECREATE(sail_int)(&zghz3160);
            mult_int(&zghz3160, zghz3158, zghz3159);
            zgaz3147 = CONVERT_OF(mach_int, sail_int)(zghz3160);
          }
          {
            RECREATE(sail_int)(&zghz3155);
            CONVERT_OF(sail_int, mach_int)(&zghz3155, zgaz3147);
            RECREATE(sail_int)(&zghz3156);
            CONVERT_OF(sail_int, mach_int)(&zghz3156, INT64_C(1));
            RECREATE(sail_int)(&zghz3157);
            sub_int(&zghz3157, zghz3155, zghz3156);
            zgaz3148 = CONVERT_OF(mach_int, sail_int)(zghz3157);
          }
        }
        zgaz3151 = (safe_rshift(UINT64_MAX, 64 - 3) & (zE >> zcap_xe_width));
      }
      uint64_t zgaz3152;
      {
        int64_t zgaz3149;
        {
          RECREATE(sail_int)(&zghz3152);
          CONVERT_OF(sail_int, mach_int)(&zghz3152, zcap_xe_width);
          RECREATE(sail_int)(&zghz3153);
          CONVERT_OF(sail_int, mach_int)(&zghz3153, INT64_C(1));
          RECREATE(sail_int)(&zghz3154);
          sub_int(&zghz3154, zghz3152, zghz3153);
          zgaz3149 = CONVERT_OF(mach_int, sail_int)(zghz3154);
        }
        zgaz3152 = (safe_rshift(UINT64_MAX, 64 - 3) & (zE >> INT64_C(0)));
      }
      struct ztuple_z8z5bitzCz0z5bv3zCz0z5bv3z9 zgsz3192;
      zgsz3192.ztup0 = zgaz3150;
      zgsz3192.ztup1 = zgaz3151;
      zgsz3192.ztup2 = zgaz3152;
      zgaz3163 = zgsz3192;
    } else {
      fbits zgaz3160;
      {
        uint64_t zgaz3155;
        {
          uint64_t zgaz3153;
          zgaz3153 = zcap.zT;
          uint64_t zgaz3154;
          zgaz3154 = zcap.zB;
          zgaz3155 = ((zgaz3153 - zgaz3154) & UINT64_C(0b11111111111111));
        }
        zgaz3160 = (UINT64_C(1) & (zgaz3155 >> INT64_C(8)));
      }
      uint64_t zgaz3161;
      {
        uint64_t zgaz3156;
        zgaz3156 = zcap.zT;
        int64_t zgaz3157;
        {
          RECREATE(sail_int)(&zghz3149);
          CONVERT_OF(sail_int, mach_int)(&zghz3149, zcap_xe_width);
          RECREATE(sail_int)(&zghz3150);
          CONVERT_OF(sail_int, mach_int)(&zghz3150, INT64_C(1));
          RECREATE(sail_int)(&zghz3151);
          sub_int(&zghz3151, zghz3149, zghz3150);
          zgaz3157 = CONVERT_OF(mach_int, sail_int)(zghz3151);
        }
        zgaz3161 = (safe_rshift(UINT64_MAX, 64 - 3) & (zgaz3156 >> INT64_C(0)));
      }
      uint64_t zgaz3162;
      {
        uint64_t zgaz3158;
        zgaz3158 = zcap.zB;
        int64_t zgaz3159;
        {
          RECREATE(sail_int)(&zghz3146);
          CONVERT_OF(sail_int, mach_int)(&zghz3146, zcap_xe_width);
          RECREATE(sail_int)(&zghz3147);
          CONVERT_OF(sail_int, mach_int)(&zghz3147, INT64_C(1));
          RECREATE(sail_int)(&zghz3148);
          sub_int(&zghz3148, zghz3146, zghz3147);
          zgaz3159 = CONVERT_OF(mach_int, sail_int)(zghz3148);
        }
        zgaz3162 = (safe_rshift(UINT64_MAX, 64 - 3) & (zgaz3158 >> INT64_C(0)));
      }
      struct ztuple_z8z5bitzCz0z5bv3zCz0z5bv3z9 zgsz3191;
      zgsz3191.ztup0 = zgaz3160;
      zgsz3191.ztup1 = zgaz3161;
      zgsz3191.ztup2 = zgaz3162;
      zgaz3163 = zgsz3191;
    }
  }
  {
    fbits zL8;
    zL8 = zgaz3163.ztup0;
    uint64_t zTE;
    zTE = zgaz3163.ztup1;
    uint64_t zBE;
    zBE = zgaz3163.ztup2;
    int64_t zMW;
    zMW = zcap_mantissa_width;
    struct zEncMetadata zghz3129;
    {
      struct zEncMetadata zghz3130;
      {
        struct zEncMetadata zghz3131;
        {
          struct zEncMetadata zghz3132;
          {
            struct zEncMetadata zghz3133;
            {
              struct zEncMetadata zghz3134;
              {
                struct zEncMetadata zghz3138;
                {
                  struct zEncMetadata zghz3142;
                  {
                    struct zEncMetadata zghz3143;
                    {
                      struct zEncMetadata zghz3144;
                      {
                        uint64_t zgaz3164;
                        zgaz3164 = zcap.zreserved_0;
                        struct zEncMetadata zghz3145;
                        zghz3145.zbits = UINT64_C(0x0000000000000000);
                        zghz3144 = sailgen__update_EncMetadata_reserved_0(zghz3145, zgaz3164);
                      }
                      uint64_t zgaz3166;
                      zgaz3166 = zcap.zreserved_1;
                      zghz3143 = sailgen__update_EncMetadata_reserved_1(zghz3144, zgaz3166);
                    }
                    zghz3142 = sailgen__update_EncMetadata_BE(zghz3143, zBE);
                  }
                  zghz3138 = sailgen__update_EncMetadata_TE(zghz3142, zTE);
                }
                uint64_t zgaz3172;
                {
                  uint64_t zgaz3169;
                  zgaz3169 = zcap.zB;
                  int64_t zgaz3170;
                  {
                    RECREATE(sail_int)(&zghz3139);
                    CONVERT_OF(sail_int, mach_int)(&zghz3139, zMW);
                    RECREATE(sail_int)(&zghz3140);
                    CONVERT_OF(sail_int, mach_int)(&zghz3140, INT64_C(1));
                    RECREATE(sail_int)(&zghz3141);
                    sub_int(&zghz3141, zghz3139, zghz3140);
                    zgaz3170 = CONVERT_OF(mach_int, sail_int)(zghz3141);
                  }
                  zgaz3172 = (safe_rshift(UINT64_MAX, 64 - 11) & (zgaz3169 >> zcap_xe_width));
                }
                zghz3134 = sailgen__update_EncMetadata_Bm(zghz3138, zgaz3172);
              }
              uint64_t zgaz3176;
              {
                uint64_t zgaz3173;
                zgaz3173 = zcap.zT;
                int64_t zgaz3174;
                {
                  RECREATE(sail_int)(&zghz3135);
                  CONVERT_OF(sail_int, mach_int)(&zghz3135, zMW);
                  RECREATE(sail_int)(&zghz3136);
                  CONVERT_OF(sail_int, mach_int)(&zghz3136, INT64_C(3));
                  RECREATE(sail_int)(&zghz3137);
                  sub_int(&zghz3137, zghz3135, zghz3136);
                  zgaz3174 = CONVERT_OF(mach_int, sail_int)(zghz3137);
                }
                zgaz3176 = (safe_rshift(UINT64_MAX, 64 - 9) & (zgaz3173 >> zcap_xe_width));
              }
              zghz3133 = sailgen__update_EncMetadata_Tm(zghz3134, zgaz3176);
            }
            uint64_t zgaz3179;
            {
              bool zgaz3177;
              zgaz3177 = zcap.zinternal_exponent;
              zgaz3179 = sailgen_internal_exponent_encdec_forwards(zgaz3177);
            }
            zghz3132 = sailgen__update_EncMetadata_EF(zghz3133, zgaz3179);
          }
          uint64_t zgaz3182;
          {
            bool zgaz3180;
            zgaz3180 = zcap.zsealed;
            zgaz3182 = sailgen_bool_bits_forwards(zgaz3180);
          }
          zghz3131 = sailgen__update_EncMetadata_CT(zghz3132, zgaz3182);
        }
        uint64_t zgaz3184;
        zgaz3184 = zcap.zap_m;
        zghz3130 = sailgen__update_EncMetadata_AP_M(zghz3131, zgaz3184);
      }
      uint64_t zgaz3186;
      zgaz3186 = zcap.zsd_perms;
      zghz3129 = sailgen__update_EncMetadata_SDP(zghz3130, zgaz3186);
    }
    uint64_t zgsz3195;
    zgsz3195 = UINT64_C(0b0);
    zgsz3195 = update_fbits(zgsz3195, INT64_C(0), zL8);
    zghz3128 = sailgen_setMetadataL8(zghz3129, zgsz3195);
    goto finish_match_270;
  }
case_271: ;
  sail_match_failure("capToMetadataBits");
finish_match_270: ;


end_function_272: ;
  return zghz3128;
end_block_exception_273: ;
  struct zEncMetadata zcbz3103 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz3103;
}



static void finish_sailgen_capToMetadataBits(void)
{
  KILL(sail_int)(&zghz3169);
  KILL(sail_int)(&zghz3168);
  KILL(sail_int)(&zghz3167);
  KILL(lbits)(&zghz3166);
  KILL(sail_int)(&zghz3165);
  KILL(sail_int)(&zghz3164);
  KILL(sail_int)(&zghz3163);
  KILL(sail_int)(&zghz3162);
  KILL(sail_int)(&zghz3161);
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
  KILL(sail_int)(&zghz3149);
  KILL(sail_int)(&zghz3148);
  KILL(sail_int)(&zghz3147);
  KILL(sail_int)(&zghz3146);




  KILL(sail_int)(&zghz3141);
  KILL(sail_int)(&zghz3140);
  KILL(sail_int)(&zghz3139);

  KILL(sail_int)(&zghz3137);
  KILL(sail_int)(&zghz3136);
  KILL(sail_int)(&zghz3135);







}

static struct zCapability znull_cap;
static void create_letbind_23(void) {


  struct zCapability zgsz3198;
  uint64_t zgaz3191;
  {
    uint64_t zgaz3190;
    {
      int64_t zgaz3189;
      {
        sail_int zgsz3446;
        CREATE(sail_int)(&zgsz3446);
        CONVERT_OF(sail_int, mach_int)(&zgsz3446, zcap_mantissa_width);
        sail_int zgsz3447;
        CREATE(sail_int)(&zgsz3447);
        CONVERT_OF(sail_int, mach_int)(&zgsz3447, INT64_C(2));
        sail_int zgsz3448;
        CREATE(sail_int)(&zgsz3448);
        sub_int(&zgsz3448, zgsz3446, zgsz3447);
        zgaz3189 = CONVERT_OF(mach_int, sail_int)(zgsz3448);
        KILL(sail_int)(&zgsz3448);
        KILL(sail_int)(&zgsz3447);
        KILL(sail_int)(&zgsz3446);
      }
      {
        sail_int zgsz3444;
        CREATE(sail_int)(&zgsz3444);
        CONVERT_OF(sail_int, mach_int)(&zgsz3444, zgaz3189);
        lbits zgsz3445;
        CREATE(lbits)(&zgsz3445);
        sailgen_zzeros_implicit(&zgsz3445, zgsz3444);
        zgaz3190 = CONVERT_OF(fbits, lbits)(zgsz3445, true);
        KILL(lbits)(&zgsz3445);
        KILL(sail_int)(&zgsz3444);
      }
    }
    zgaz3191 = (UINT64_C(0b01) << 12) | zgaz3190;
  }
  struct zCapability zgsz3197;
  zgsz3197.zB = UINT64_C(0b00000000000000);
  zgsz3197.zE = zcap_max_E;
  zgsz3197.zT = zgaz3191;
  zgsz3197.zaddress = UINT64_C(0x0000000000000000);
  zgsz3197.zap_m = znull_ap_m;
  zgsz3197.zinternal_exponent = true;
  zgsz3197.zreserved_0 = UINT64_C(0x0000);
  zgsz3197.zreserved_1 = UINT64_C(0b0000000);
  zgsz3197.zsd_perms = UINT64_C(0x0);
  zgsz3197.zsealed = false;
  zgsz3197.ztag = false;
  zgsz3198 = zgsz3197;


  znull_cap = zgsz3198;

let_end_274: ;
}
static void kill_letbind_23(void) {
}

static struct zCapability zinfinite_cap;
static void create_letbind_24(void) {


  struct zCapability zgsz3200;
  struct zCapability zgsz3199;
  zgsz3199 = znull_cap;
  zgsz3199.zap_m = zinfinite_ap_m;
  zgsz3199.zsd_perms = UINT64_C(0xF);
  zgsz3199.ztag = true;
  zgsz3200 = zgsz3199;

  zinfinite_cap = zgsz3200;

let_end_275: ;
}
static void kill_letbind_24(void) {
}

static bool sailgen_boundsMalformed(struct zCapability);

sail_int zghz3170;
sail_int zghz3171;
sail_int zghz3172;
sail_int zghz3173;
sail_int zghz3174;
sail_int zghz3175;
sail_int zghz3176;
lbits zghz3177;

static void startup_sailgen_boundsMalformed(void)
{
  CREATE(sail_int)(&zghz3170);
  CREATE(sail_int)(&zghz3171);
  CREATE(sail_int)(&zghz3172);
  CREATE(sail_int)(&zghz3173);
  CREATE(sail_int)(&zghz3174);
  CREATE(sail_int)(&zghz3175);
  CREATE(sail_int)(&zghz3176);
  CREATE(lbits)(&zghz3177);
}

static bool sailgen_boundsMalformed(struct zCapability zcap)
{
  __label__ end_function_277, end_block_exception_278;

  bool zcbz360;
  bool zgaz3206;
  zgaz3206 = zcap.zinternal_exponent;
  bool zgsz3207;
  if (zgaz3206) {
    int64_t zMW;
    zMW = zcap_mantissa_width;
    bool zmalformedMSB;
    {
      bool zgaz3205;
      {
        bool zgaz3198;
        {
          int64_t zgaz3195;
          zgaz3195 = zcap.zE;
          zgaz3198 = (zgaz3195 == zcap_max_E);
        }
        bool zgsz3201;
        if (zgaz3198) {
          uint64_t zgaz3196;
          zgaz3196 = zcap.zB;
          uint64_t zgaz3197;
          {
            RECREATE(sail_int)(&zghz3176);
            CONVERT_OF(sail_int, mach_int)(&zghz3176, zMW);
            RECREATE(lbits)(&zghz3177);
            sailgen_zzeros_implicit(&zghz3177, zghz3176);
            zgaz3197 = CONVERT_OF(fbits, lbits)(zghz3177, true);
          }
          zgsz3201 = (zgaz3196 != zgaz3197);
        } else {  zgsz3201 = false;  }
        zgaz3205 = zgsz3201;
      }
      bool zgsz3203;
      if (zgaz3205) {  zgsz3203 = true;  } else {
        bool zgaz3204;
        {
          int64_t zgaz3199;
          zgaz3199 = zcap.zE;
          int64_t zgaz3200;
          {
            RECREATE(sail_int)(&zghz3173);
            CONVERT_OF(sail_int, mach_int)(&zghz3173, zcap_max_E);
            RECREATE(sail_int)(&zghz3174);
            CONVERT_OF(sail_int, mach_int)(&zghz3174, INT64_C(1));
            RECREATE(sail_int)(&zghz3175);
            sub_int(&zghz3175, zghz3173, zghz3174);
            zgaz3200 = CONVERT_OF(mach_int, sail_int)(zghz3175);
          }
          zgaz3204 = (zgaz3199 == zgaz3200);
        }
        bool zgsz3202;
        if (zgaz3204) {
          fbits zgaz3203;
          {
            uint64_t zgaz3201;
            zgaz3201 = zcap.zB;
            int64_t zgaz3202;
            {
              RECREATE(sail_int)(&zghz3170);
              CONVERT_OF(sail_int, mach_int)(&zghz3170, zMW);
              RECREATE(sail_int)(&zghz3171);
              CONVERT_OF(sail_int, mach_int)(&zghz3171, INT64_C(1));
              RECREATE(sail_int)(&zghz3172);
              sub_int(&zghz3172, zghz3170, zghz3171);
              zgaz3202 = CONVERT_OF(mach_int, sail_int)(zghz3172);
            }
            zgaz3203 = (UINT64_C(1) & (zgaz3201 >> zgaz3202));
          }
          zgsz3202 = sailgen_neq_anythingzItzK(zgaz3203, UINT64_C(0));
        } else {  zgsz3202 = false;  }
        zgsz3203 = zgsz3202;
      }
      zmalformedMSB = zgsz3203;
    }
    bool zmalformedLSB;
    {
      bool zgaz3194;
      {
        int64_t zgaz3192;
        zgaz3192 = zcap.zE;
        zgaz3194 = (zgaz3192 < INT64_C(0));
      }
      bool zgsz3205;
      if (zgaz3194) {  zgsz3205 = true;  } else {
        bool zgsz3204;
        if (zcap_use_L8) {
          int64_t zgaz3193;
          zgaz3193 = zcap.zE;
          zgsz3204 = (zgaz3193 == INT64_C(0));
        } else {  zgsz3204 = false;  }
        zgsz3205 = zgsz3204;
      }
      zmalformedLSB = zgsz3205;
    }
    bool zgsz3206;
    if (zmalformedMSB) {  zgsz3206 = true;  } else {  zgsz3206 = zmalformedLSB;  }
    zgsz3207 = zgsz3206;
  } else {  zgsz3207 = false;  }
  zcbz360 = zgsz3207;

end_function_277: ;
  return zcbz360;
end_block_exception_278: ;

  return false;
}



static void finish_sailgen_boundsMalformed(void)
{
  KILL(lbits)(&zghz3177);
  KILL(sail_int)(&zghz3176);
  KILL(sail_int)(&zghz3175);
  KILL(sail_int)(&zghz3174);
  KILL(sail_int)(&zghz3173);
  KILL(sail_int)(&zghz3172);
  KILL(sail_int)(&zghz3171);
  KILL(sail_int)(&zghz3170);
}

static void sailgen_getCapBoundsBits(struct zoptionzIz8bzCbz9zK *rop, struct zCapability);

lbits zghz3178;
lbits zghz3179;
sail_int zghz3180;
sail_int zghz3181;
lbits zghz3182;
sail_int zghz3183;
sail_int zghz3184;
sail_int zghz3185;
sail_int zghz3186;
sail_int zghz3187;
lbits zghz3188;
lbits zghz3189;
sail_int zghz3190;
sail_int zghz3191;
sail_int zghz3192;
sail_int zghz3193;
sail_int zghz3194;
lbits zghz3195;
sail_int zghz3196;
sail_int zghz3197;
sail_int zghz3198;
sail_int zghz3199;
sail_int zghz3200;
sail_int zghz3201;
sail_int zghz3202;
lbits zghz3203;
sail_int zghz3204;
lbits zghz3205;
lbits zghz3206;
lbits zghz3207;
lbits zghz3208;
sail_int zghz3209;
lbits zghz3210;
lbits zghz3211;
sail_int zghz3212;
lbits zghz3213;
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
sail_int zghz3228;
sail_int zghz3229;
sail_int zghz3230;
sail_int zghz3231;
sail_int zghz3232;
sail_int zghz3233;
lbits zghz3234;
lbits zghz3235;
lbits zghz3236;
lbits zghz3237;
lbits zghz3238;
lbits zghz3239;
sail_int zghz3240;
lbits zghz3241;
sail_int zghz3242;
sail_int zghz3243;
sail_int zghz3244;
lbits zghz3245;
sail_int zghz3246;
lbits zghz3247;
lbits zghz3248;
sail_int zghz3249;
lbits zghz3250;

static void startup_sailgen_getCapBoundsBits(void)
{
  CREATE(lbits)(&zghz3178);
  CREATE(lbits)(&zghz3179);
  CREATE(sail_int)(&zghz3180);
  CREATE(sail_int)(&zghz3181);
  CREATE(lbits)(&zghz3182);
  CREATE(sail_int)(&zghz3183);
  CREATE(sail_int)(&zghz3184);
  CREATE(sail_int)(&zghz3185);
  CREATE(sail_int)(&zghz3186);
  CREATE(sail_int)(&zghz3187);
  CREATE(lbits)(&zghz3188);
  CREATE(lbits)(&zghz3189);
  CREATE(sail_int)(&zghz3190);
  CREATE(sail_int)(&zghz3191);
  CREATE(sail_int)(&zghz3192);
  CREATE(sail_int)(&zghz3193);
  CREATE(sail_int)(&zghz3194);
  CREATE(lbits)(&zghz3195);
  CREATE(sail_int)(&zghz3196);
  CREATE(sail_int)(&zghz3197);
  CREATE(sail_int)(&zghz3198);
  CREATE(sail_int)(&zghz3199);
  CREATE(sail_int)(&zghz3200);
  CREATE(sail_int)(&zghz3201);
  CREATE(sail_int)(&zghz3202);
  CREATE(lbits)(&zghz3203);
  CREATE(sail_int)(&zghz3204);
  CREATE(lbits)(&zghz3205);
  CREATE(lbits)(&zghz3206);
  CREATE(lbits)(&zghz3207);
  CREATE(lbits)(&zghz3208);
  CREATE(sail_int)(&zghz3209);
  CREATE(lbits)(&zghz3210);
  CREATE(lbits)(&zghz3211);
  CREATE(sail_int)(&zghz3212);
  CREATE(lbits)(&zghz3213);
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
  CREATE(sail_int)(&zghz3228);
  CREATE(sail_int)(&zghz3229);
  CREATE(sail_int)(&zghz3230);
  CREATE(sail_int)(&zghz3231);
  CREATE(sail_int)(&zghz3232);
  CREATE(sail_int)(&zghz3233);
  CREATE(lbits)(&zghz3234);
  CREATE(lbits)(&zghz3235);
  CREATE(lbits)(&zghz3236);
  CREATE(lbits)(&zghz3237);
  CREATE(lbits)(&zghz3238);
  CREATE(lbits)(&zghz3239);
  CREATE(sail_int)(&zghz3240);
  CREATE(lbits)(&zghz3241);
  CREATE(sail_int)(&zghz3242);
  CREATE(sail_int)(&zghz3243);
  CREATE(sail_int)(&zghz3244);
  CREATE(lbits)(&zghz3245);
  CREATE(sail_int)(&zghz3246);
  CREATE(lbits)(&zghz3247);
  CREATE(lbits)(&zghz3248);
  CREATE(sail_int)(&zghz3249);
  CREATE(lbits)(&zghz3250);
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

static void sailgen_getCapBoundsBits(struct zoptionzIz8bzCbz9zK *zcbz361, struct zCapability zc)
{
  __label__ end_function_280, end_block_exception_283, end_function_401;

  {
    bool zgaz3207;
    zgaz3207 = sailgen_boundsMalformed(zc);
    unit zgsz3208;
    if (zgaz3207) {
      __label__ cleanup_281, end_cleanup_282;

      struct zoptionzIz8bzCbz9zK zgaz3208;
      CREATE(zoptionzIz8bzCbz9zK)(&zgaz3208);
      sailgen_NonezIz8bzCbz9zK(&zgaz3208, UNIT);
      COPY(zoptionzIz8bzCbz9zK)((*(&zcbz361)), zgaz3208);
      goto cleanup_281;
      /* unreachable after return */
      KILL(zoptionzIz8bzCbz9zK)(&zgaz3208);
      goto end_cleanup_282;
    cleanup_281: ;
      KILL(zoptionzIz8bzCbz9zK)(&zgaz3208);
      goto end_function_280;
    end_cleanup_282: ;
    } else {  zgsz3208 = UNIT;  }
  }
  int64_t zE;
  zE = zc.zE;
  {
    bool zgaz3209;
    zgaz3209 = (zE >= INT64_C(0));
    unit zgsz3209;
    zgsz3209 = sail_assert(zgaz3209, "Cap is malformed if E<0");
  }
  uint64_t za;
  za = zc.zaddress;
  uint64_t za_mid;
  {
    uint64_t zgaz3241;
    {
      RECREATE(lbits)(&zghz3248);
      CONVERT_OF(lbits, fbits)(&zghz3248, za, UINT64_C(64) , true);
      RECREATE(sail_int)(&zghz3249);
      CONVERT_OF(sail_int, mach_int)(&zghz3249, zE);
      RECREATE(lbits)(&zghz3250);
      shiftr(&zghz3250, zghz3248, zghz3249);
      zgaz3241 = CONVERT_OF(fbits, lbits)(zghz3250, true);
    }
    {
      RECREATE(lbits)(&zghz3245);
      CONVERT_OF(lbits, fbits)(&zghz3245, zgaz3241, UINT64_C(64) , true);
      RECREATE(sail_int)(&zghz3246);
      CONVERT_OF(sail_int, mach_int)(&zghz3246, zcap_mantissa_width);
      RECREATE(lbits)(&zghz3247);
      sail_truncate(&zghz3247, zghz3245, zghz3246);
      za_mid = CONVERT_OF(fbits, lbits)(zghz3247, true);
    }
  }
  uint64_t zR;
  {
    uint64_t zgaz3239;
    zgaz3239 = zc.zB;
    uint64_t zgaz3240;
    {
      uint64_t zgaz3238;
      {
        int64_t zgaz3237;
        {
          RECREATE(sail_int)(&zghz3242);
          CONVERT_OF(sail_int, mach_int)(&zghz3242, zcap_mantissa_width);
          RECREATE(sail_int)(&zghz3243);
          CONVERT_OF(sail_int, mach_int)(&zghz3243, INT64_C(2));
          RECREATE(sail_int)(&zghz3244);
          sub_int(&zghz3244, zghz3242, zghz3243);
          zgaz3237 = CONVERT_OF(mach_int, sail_int)(zghz3244);
        }
        {
          RECREATE(sail_int)(&zghz3240);
          CONVERT_OF(sail_int, mach_int)(&zghz3240, zgaz3237);
          RECREATE(lbits)(&zghz3241);
          sailgen_zzeros_implicit(&zghz3241, zghz3240);
          zgaz3238 = CONVERT_OF(fbits, lbits)(zghz3241, true);
        }
      }
      zgaz3240 = (UINT64_C(0b01) << 12) | zgaz3238;
    }
    zR = ((zgaz3239 - zgaz3240) & UINT64_C(0b11111111111111));
  }
  int64_t zaHi;
  {
    bool zgaz3236;
    {
      RECREATE(lbits)(&zghz3238);
      CONVERT_OF(lbits, fbits)(&zghz3238, za_mid, UINT64_C(14) , true);
      RECREATE(lbits)(&zghz3239);
      CONVERT_OF(lbits, fbits)(&zghz3239, zR, UINT64_C(14) , true);
      zgaz3236 = sailgen_z8operatorz0zI_uz9(zghz3238, zghz3239);
    }
    if (zgaz3236) {  zaHi = INT64_C(1);  } else {  zaHi = INT64_C(0);  }
  }
  int64_t zbHi;
  {
    bool zgaz3235;
    {
      uint64_t zgaz3234;
      zgaz3234 = zc.zB;
      {
        RECREATE(lbits)(&zghz3236);
        CONVERT_OF(lbits, fbits)(&zghz3236, zgaz3234, UINT64_C(14) , true);
        RECREATE(lbits)(&zghz3237);
        CONVERT_OF(lbits, fbits)(&zghz3237, zR, UINT64_C(14) , true);
        zgaz3235 = sailgen_z8operatorz0zI_uz9(zghz3236, zghz3237);
      }
    }
    if (zgaz3235) {  zbHi = INT64_C(1);  } else {  zbHi = INT64_C(0);  }
  }
  int64_t ztHi;
  {
    bool zgaz3233;
    {
      uint64_t zgaz3232;
      zgaz3232 = zc.zT;
      {
        RECREATE(lbits)(&zghz3234);
        CONVERT_OF(lbits, fbits)(&zghz3234, zgaz3232, UINT64_C(14) , true);
        RECREATE(lbits)(&zghz3235);
        CONVERT_OF(lbits, fbits)(&zghz3235, zR, UINT64_C(14) , true);
        zgaz3233 = sailgen_z8operatorz0zI_uz9(zghz3234, zghz3235);
      }
    }
    if (zgaz3233) {  ztHi = INT64_C(1);  } else {  ztHi = INT64_C(0);  }
  }
  int64_t zcorrection_base;
  {
    RECREATE(sail_int)(&zghz3231);
    CONVERT_OF(sail_int, mach_int)(&zghz3231, zbHi);
    RECREATE(sail_int)(&zghz3232);
    CONVERT_OF(sail_int, mach_int)(&zghz3232, zaHi);
    RECREATE(sail_int)(&zghz3233);
    sub_int(&zghz3233, zghz3231, zghz3232);
    zcorrection_base = CONVERT_OF(mach_int, sail_int)(zghz3233);
  }
  int64_t zcorrection_top;
  {
    RECREATE(sail_int)(&zghz3228);
    CONVERT_OF(sail_int, mach_int)(&zghz3228, ztHi);
    RECREATE(sail_int)(&zghz3229);
    CONVERT_OF(sail_int, mach_int)(&zghz3229, zaHi);
    RECREATE(sail_int)(&zghz3230);
    sub_int(&zghz3230, zghz3228, zghz3229);
    zcorrection_top = CONVERT_OF(mach_int, sail_int)(zghz3230);
  }
  uint64_t za_top;
  {
    int64_t zgaz3231;
    zgaz3231 = (zE + zcap_mantissa_width);
    {
      RECREATE(lbits)(&zghz3225);
      CONVERT_OF(lbits, fbits)(&zghz3225, za, UINT64_C(64) , true);
      RECREATE(sail_int)(&zghz3226);
      CONVERT_OF(sail_int, mach_int)(&zghz3226, zgaz3231);
      RECREATE(lbits)(&zghz3227);
      shiftr(&zghz3227, zghz3225, zghz3226);
      za_top = CONVERT_OF(fbits, lbits)(zghz3227, true);
    }
  }
  RECREATE(lbits)(&zghz3178);
  {
    RECREATE(lbits)(&zghz3214);
    {
      uint64_t zgaz3228;
      {
        RECREATE(lbits)(&zghz3222);
        CONVERT_OF(lbits, fbits)(&zghz3222, za_top, UINT64_C(64) , true);
        RECREATE(sail_int)(&zghz3223);
        CONVERT_OF(sail_int, mach_int)(&zghz3223, zcorrection_base);
        RECREATE(lbits)(&zghz3224);
        add_bits_int(&zghz3224, zghz3222, zghz3223);
        zgaz3228 = CONVERT_OF(fbits, lbits)(zghz3224, true);
      }
      RECREATE(lbits)(&zghz3216);
      {
        uint64_t zgaz3226;
        zgaz3226 = zc.zB;
        sbits zgaz3227;
        {
          RECREATE(sail_int)(&zghz3220);
          CONVERT_OF(sail_int, mach_int)(&zghz3220, zE);
          RECREATE(lbits)(&zghz3221);
          sailgen_zzeros_implicit(&zghz3221, zghz3220);
          zgaz3227 = CONVERT_OF(sbits, lbits)(zghz3221, true);
        }
        {
          RECREATE(lbits)(&zghz3218);
          CONVERT_OF(lbits, fbits)(&zghz3218, zgaz3226, UINT64_C(14) , true);
          RECREATE(lbits)(&zghz3219);
          CONVERT_OF(lbits, sbits)(&zghz3219, zgaz3227, true);
          append(&zghz3216, zghz3218, zghz3219);
        }
      }
      {
        RECREATE(lbits)(&zghz3217);
        CONVERT_OF(lbits, fbits)(&zghz3217, zgaz3228, UINT64_C(64) , true);
        append(&zghz3214, zghz3217, zghz3216);
      }
    }
    {
      RECREATE(sail_int)(&zghz3215);
      CONVERT_OF(sail_int, mach_int)(&zghz3215, zcap_len_width);
      sail_truncate(&zghz3178, zghz3214, zghz3215);
    }
  }
  RECREATE(lbits)(&zghz3179);
  {
    RECREATE(lbits)(&zghz3203);
    {
      uint64_t zgaz3223;
      {
        RECREATE(lbits)(&zghz3211);
        CONVERT_OF(lbits, fbits)(&zghz3211, za_top, UINT64_C(64) , true);
        RECREATE(sail_int)(&zghz3212);
        CONVERT_OF(sail_int, mach_int)(&zghz3212, zcorrection_top);
        RECREATE(lbits)(&zghz3213);
        add_bits_int(&zghz3213, zghz3211, zghz3212);
        zgaz3223 = CONVERT_OF(fbits, lbits)(zghz3213, true);
      }
      RECREATE(lbits)(&zghz3205);
      {
        uint64_t zgaz3221;
        zgaz3221 = zc.zT;
        sbits zgaz3222;
        {
          RECREATE(sail_int)(&zghz3209);
          CONVERT_OF(sail_int, mach_int)(&zghz3209, zE);
          RECREATE(lbits)(&zghz3210);
          sailgen_zzeros_implicit(&zghz3210, zghz3209);
          zgaz3222 = CONVERT_OF(sbits, lbits)(zghz3210, true);
        }
        {
          RECREATE(lbits)(&zghz3207);
          CONVERT_OF(lbits, fbits)(&zghz3207, zgaz3221, UINT64_C(14) , true);
          RECREATE(lbits)(&zghz3208);
          CONVERT_OF(lbits, sbits)(&zghz3208, zgaz3222, true);
          append(&zghz3205, zghz3207, zghz3208);
        }
      }
      {
        RECREATE(lbits)(&zghz3206);
        CONVERT_OF(lbits, fbits)(&zghz3206, zgaz3223, UINT64_C(64) , true);
        append(&zghz3203, zghz3206, zghz3205);
      }
    }
    {
      RECREATE(sail_int)(&zghz3204);
      CONVERT_OF(sail_int, mach_int)(&zghz3204, zcap_len_width);
      sail_truncate(&zghz3179, zghz3203, zghz3204);
    }
  }
  uint64_t zbase2;
  {
    uint64_t zgaz3220;
    {
      fbits zgaz3219;
      {
        int64_t zgaz3218;
        {
          RECREATE(sail_int)(&zghz3200);
          CONVERT_OF(sail_int, mach_int)(&zghz3200, zcap_addr_width);
          RECREATE(sail_int)(&zghz3201);
          CONVERT_OF(sail_int, mach_int)(&zghz3201, INT64_C(1));
          RECREATE(sail_int)(&zghz3202);
          sub_int(&zghz3202, zghz3200, zghz3201);
          zgaz3218 = CONVERT_OF(mach_int, sail_int)(zghz3202);
        }
        {
          RECREATE(sail_int)(&zghz3199);
          CONVERT_OF(sail_int, mach_int)(&zghz3199, zgaz3218);
          zgaz3219 = bitvector_access(zghz3178, zghz3199);
        }
      }
      zgaz3220 = UINT64_C(0b0);
      zgaz3220 = update_fbits(zgaz3220, INT64_C(0), zgaz3219);
    }
    zbase2 = (UINT64_C(0b0) << 1) | zgaz3220;
  }
  uint64_t ztop2;
  {
    int64_t zgaz3217;
    {
      RECREATE(sail_int)(&zghz3196);
      CONVERT_OF(sail_int, mach_int)(&zghz3196, zcap_addr_width);
      RECREATE(sail_int)(&zghz3197);
      CONVERT_OF(sail_int, mach_int)(&zghz3197, INT64_C(1));
      RECREATE(sail_int)(&zghz3198);
      sub_int(&zghz3198, zghz3196, zghz3197);
      zgaz3217 = CONVERT_OF(mach_int, sail_int)(zghz3198);
    }
    {
      RECREATE(sail_int)(&zghz3193);
      CONVERT_OF(sail_int, mach_int)(&zghz3193, zcap_addr_width);
      RECREATE(sail_int)(&zghz3194);
      CONVERT_OF(sail_int, mach_int)(&zghz3194, zgaz3217);
      RECREATE(lbits)(&zghz3195);
      vector_subrange_lbits(&zghz3195, zghz3179, zghz3193, zghz3194);
      ztop2 = CONVERT_OF(fbits, lbits)(zghz3195, true);
    }
  }
  {
    bool zgaz3213;
    {
      bool zgaz3212;
      {
        int64_t zgaz3210;
        {
          RECREATE(sail_int)(&zghz3190);
          CONVERT_OF(sail_int, mach_int)(&zghz3190, zcap_max_E);
          RECREATE(sail_int)(&zghz3191);
          CONVERT_OF(sail_int, mach_int)(&zghz3191, INT64_C(1));
          RECREATE(sail_int)(&zghz3192);
          sub_int(&zghz3192, zghz3190, zghz3191);
          zgaz3210 = CONVERT_OF(mach_int, sail_int)(zghz3192);
        }
        zgaz3212 = (zE < zgaz3210);
      }
      bool zgsz3211;
      if (zgaz3212) {
        uint64_t zgaz3211;
        zgaz3211 = ((ztop2 - zbase2) & UINT64_C(0b11));
        {
          RECREATE(lbits)(&zghz3188);
          CONVERT_OF(lbits, fbits)(&zghz3188, zgaz3211, UINT64_C(2) , true);
          RECREATE(lbits)(&zghz3189);
          CONVERT_OF(lbits, fbits)(&zghz3189, UINT64_C(0b01), UINT64_C(2) , true);
          zgsz3211 = sailgen_z8operatorz0zK_uz9(zghz3188, zghz3189);
        }
      } else {  zgsz3211 = false;  }
      zgaz3213 = zgsz3211;
    }
    unit zgsz3212;
    if (zgaz3213) {
      fbits zgaz3215;
      {
        fbits zgaz3214;
        {
          RECREATE(sail_int)(&zghz3187);
          CONVERT_OF(sail_int, mach_int)(&zghz3187, zcap_addr_width);
          zgaz3214 = bitvector_access(zghz3179, zghz3187);
        }
        zgaz3215 = sailgen_not_bit(zgaz3214);
      }
      {
        RECREATE(sail_int)(&zghz3186);
        CONVERT_OF(sail_int, mach_int)(&zghz3186, zcap_addr_width);
        update_lbits(&zghz3179, zghz3179, zghz3186, zgaz3215);
      }
      zgsz3212 = UNIT;
    } else {  zgsz3212 = UNIT;  }
  }
  uint64_t zbaseshadowz323;
  {
    int64_t zgaz3216;
    {
      RECREATE(sail_int)(&zghz3183);
      CONVERT_OF(sail_int, mach_int)(&zghz3183, zcap_addr_width);
      RECREATE(sail_int)(&zghz3184);
      CONVERT_OF(sail_int, mach_int)(&zghz3184, INT64_C(1));
      RECREATE(sail_int)(&zghz3185);
      sub_int(&zghz3185, zghz3183, zghz3184);
      zgaz3216 = CONVERT_OF(mach_int, sail_int)(zghz3185);
    }
    {
      RECREATE(sail_int)(&zghz3180);
      CONVERT_OF(sail_int, mach_int)(&zghz3180, zgaz3216);
      RECREATE(sail_int)(&zghz3181);
      CONVERT_OF(sail_int, mach_int)(&zghz3181, INT64_C(0));
      RECREATE(lbits)(&zghz3182);
      vector_subrange_lbits(&zghz3182, zghz3178, zghz3180, zghz3181);
      zbaseshadowz323 = CONVERT_OF(fbits, lbits)(zghz3182, true);
    }
  }
  struct ztuple_z8z5bv64zCz0z5bvz9 zgsz3213;
  CREATE(ztuple_z8z5bv64zCz0z5bvz9)(&zgsz3213);
  zgsz3213.ztup0 = zbaseshadowz323;
  COPY(lbits)(&((&zgsz3213)->ztup1), zghz3179);
  {
    struct ztuple_z8z5bvzCz0z5bvz9 zgsz3518;
    CREATE(ztuple_z8z5bvzCz0z5bvz9)(&zgsz3518);
    /* conversions */
    CONVERT_OF(lbits, fbits)(&((&zgsz3518)->ztup0), zgsz3213.ztup0, UINT64_C(64) , true);
    COPY(lbits)(&((&zgsz3518)->ztup1), zgsz3213.ztup1);
    /* end conversions */
    sailgen_SomezIz8bzCbz9zK((*(&zcbz361)), zgsz3518);
    KILL(ztuple_z8z5bvzCz0z5bvz9)(&zgsz3518);
  }
  KILL(ztuple_z8z5bv64zCz0z5bvz9)(&zgsz3213);













end_function_280: ;
  goto end_function_401;
end_block_exception_283: ;
  goto end_function_401;
end_function_401: ;
}



static void finish_sailgen_getCapBoundsBits(void)
{
  KILL(lbits)(&zghz3250);
  KILL(sail_int)(&zghz3249);
  KILL(lbits)(&zghz3248);
  KILL(lbits)(&zghz3247);
  KILL(sail_int)(&zghz3246);
  KILL(lbits)(&zghz3245);
  KILL(sail_int)(&zghz3244);
  KILL(sail_int)(&zghz3243);
  KILL(sail_int)(&zghz3242);
  KILL(lbits)(&zghz3241);
  KILL(sail_int)(&zghz3240);
  KILL(lbits)(&zghz3239);
  KILL(lbits)(&zghz3238);
  KILL(lbits)(&zghz3237);
  KILL(lbits)(&zghz3236);
  KILL(lbits)(&zghz3235);
  KILL(lbits)(&zghz3234);
  KILL(sail_int)(&zghz3233);
  KILL(sail_int)(&zghz3232);
  KILL(sail_int)(&zghz3231);
  KILL(sail_int)(&zghz3230);
  KILL(sail_int)(&zghz3229);
  KILL(sail_int)(&zghz3228);
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
  KILL(lbits)(&zghz3213);
  KILL(sail_int)(&zghz3212);
  KILL(lbits)(&zghz3211);
  KILL(lbits)(&zghz3210);
  KILL(sail_int)(&zghz3209);
  KILL(lbits)(&zghz3208);
  KILL(lbits)(&zghz3207);
  KILL(lbits)(&zghz3206);
  KILL(lbits)(&zghz3205);
  KILL(sail_int)(&zghz3204);
  KILL(lbits)(&zghz3203);
  KILL(sail_int)(&zghz3202);
  KILL(sail_int)(&zghz3201);
  KILL(sail_int)(&zghz3200);
  KILL(sail_int)(&zghz3199);
  KILL(sail_int)(&zghz3198);
  KILL(sail_int)(&zghz3197);
  KILL(sail_int)(&zghz3196);
  KILL(lbits)(&zghz3195);
  KILL(sail_int)(&zghz3194);
  KILL(sail_int)(&zghz3193);
  KILL(sail_int)(&zghz3192);
  KILL(sail_int)(&zghz3191);
  KILL(sail_int)(&zghz3190);
  KILL(lbits)(&zghz3189);
  KILL(lbits)(&zghz3188);
  KILL(sail_int)(&zghz3187);
  KILL(sail_int)(&zghz3186);
  KILL(sail_int)(&zghz3185);
  KILL(sail_int)(&zghz3184);
  KILL(sail_int)(&zghz3183);
  KILL(lbits)(&zghz3182);
  KILL(sail_int)(&zghz3181);
  KILL(sail_int)(&zghz3180);
  KILL(lbits)(&zghz3179);
  KILL(lbits)(&zghz3178);
}

static void sailgen_getCapBounds(struct zoptionzIz8izCiz9zK *rop, struct zCapability);

lbits zghz3251;
sail_int zghz3252;
sail_int zghz3253;
lbits zghz3254;

static void startup_sailgen_getCapBounds(void)
{
  CREATE(lbits)(&zghz3251);
  CREATE(sail_int)(&zghz3252);
  CREATE(sail_int)(&zghz3253);
  CREATE(lbits)(&zghz3254);
}



static void sailgen_getCapBounds(struct zoptionzIz8izCiz9zK *zcbz362, struct zCapability zcap)
{
  __label__ case_287, case_286, finish_match_285, end_function_288, end_block_exception_289, end_function_400;

  struct zoptionzIz8bzCbz9zK zgaz3242;
  CREATE(zoptionzIz8bzCbz9zK)(&zgaz3242);
  sailgen_getCapBoundsBits(&zgaz3242, zcap);
  struct zoptionzIz8izCiz9zK zgsz3214;
  CREATE(zoptionzIz8izCiz9zK)(&zgsz3214);
  {
    if (zgaz3242.kind != Kind_zNonezIz8bzCbz9zK) goto case_287;
    sailgen_NonezIz8izCiz9zK(&zgsz3214, UNIT);
    goto finish_match_285;
  }
case_287: ;
  {
    /* complete */
    uint64_t zbase;
    zbase = CONVERT_OF(fbits, lbits)(zgaz3242.zSomezIz8bzCbz9zK.ztup0, true);
    RECREATE(lbits)(&zghz3251);
    COPY(lbits)(&zghz3251, zgaz3242.zSomezIz8bzCbz9zK.ztup1);
    struct ztuple_z8z5izCz0z5iz9 zgaz3245;
    CREATE(ztuple_z8z5izCz0z5iz9)(&zgaz3245);
    {
      RECREATE(sail_int)(&zghz3252);
      {
        RECREATE(lbits)(&zghz3254);
        CONVERT_OF(lbits, fbits)(&zghz3254, zbase, UINT64_C(64) , true);
        sail_unsigned(&zghz3252, zghz3254);
      }
      RECREATE(sail_int)(&zghz3253);
      sail_unsigned(&zghz3253, zghz3251);
      COPY(sail_int)(&((&zgaz3245)->ztup0), zghz3252);
      COPY(sail_int)(&((&zgaz3245)->ztup1), zghz3253);
    }
    sailgen_SomezIz8izCiz9zK(&zgsz3214, zgaz3245);
    KILL(ztuple_z8z5izCz0z5iz9)(&zgaz3245);
    goto finish_match_285;
  }
case_286: ;
finish_match_285: ;
  COPY(zoptionzIz8izCiz9zK)((*(&zcbz362)), zgsz3214);
  KILL(zoptionzIz8izCiz9zK)(&zgsz3214);
  KILL(zoptionzIz8bzCbz9zK)(&zgaz3242);
end_function_288: ;
  goto end_function_400;
end_block_exception_289: ;
  goto end_function_400;
end_function_400: ;
}



static void finish_sailgen_getCapBounds(void)
{
  KILL(lbits)(&zghz3254);
  KILL(sail_int)(&zghz3253);
  KILL(sail_int)(&zghz3252);
  KILL(lbits)(&zghz3251);
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

lbits zghz3255;
lbits zghz3256;
struct zCapability zghz3257;
lbits zghz3258;
sail_int zghz3259;
lbits zghz3260;
sail_int zghz3261;
lbits zghz3262;
lbits zghz3263;
sail_int zghz3264;
lbits zghz3265;
lbits zghz3266;
sail_int zghz3267;
lbits zghz3268;
sail_int zghz3269;
sail_int zghz3270;
sail_int zghz3271;
sail_int zghz3272;
lbits zghz3273;
sail_int zghz3274;
lbits zghz3275;
sail_int zghz3276;
sail_int zghz3277;
sail_int zghz3278;
lbits zghz3279;
sail_int zghz3280;
lbits zghz3281;
sail_int zghz3282;
sail_int zghz3283;
sail_int zghz3284;
lbits zghz3285;
sail_int zghz3286;
lbits zghz3287;
lbits zghz3288;
lbits zghz3289;
lbits zghz3290;
lbits zghz3291;
sail_int zghz3292;
lbits zghz3293;
sail_int zghz3294;
lbits zghz3295;
lbits zghz3296;
sail_int zghz3297;
lbits zghz3298;
sail_int zghz3299;
sail_int zghz3300;
sail_int zghz3301;
sail_int zghz3302;
lbits zghz3303;
sail_int zghz3304;
lbits zghz3305;
sail_int zghz3306;
sail_int zghz3307;
sail_int zghz3308;
lbits zghz3309;
sail_int zghz3310;
lbits zghz3311;
sail_int zghz3312;
lbits zghz3313;
lbits zghz3314;
sail_int zghz3315;
lbits zghz3316;
sail_int zghz3317;
sail_int zghz3318;
sail_int zghz3319;
sail_int zghz3320;
sail_int zghz3321;
sail_int zghz3322;
sail_int zghz3323;
sail_int zghz3324;
sail_int zghz3325;
lbits zghz3326;
sail_int zghz3327;
sail_int zghz3328;
sail_int zghz3329;
lbits zghz3330;
sail_int zghz3331;
sail_int zghz3332;
sail_int zghz3333;
lbits zghz3334;
lbits zghz3335;

static void startup_sailgen_setCapBounds(void)
{
  CREATE(lbits)(&zghz3255);
  CREATE(lbits)(&zghz3256);

  CREATE(lbits)(&zghz3258);
  CREATE(sail_int)(&zghz3259);
  CREATE(lbits)(&zghz3260);
  CREATE(sail_int)(&zghz3261);
  CREATE(lbits)(&zghz3262);
  CREATE(lbits)(&zghz3263);
  CREATE(sail_int)(&zghz3264);
  CREATE(lbits)(&zghz3265);
  CREATE(lbits)(&zghz3266);
  CREATE(sail_int)(&zghz3267);
  CREATE(lbits)(&zghz3268);
  CREATE(sail_int)(&zghz3269);
  CREATE(sail_int)(&zghz3270);
  CREATE(sail_int)(&zghz3271);
  CREATE(sail_int)(&zghz3272);
  CREATE(lbits)(&zghz3273);
  CREATE(sail_int)(&zghz3274);
  CREATE(lbits)(&zghz3275);
  CREATE(sail_int)(&zghz3276);
  CREATE(sail_int)(&zghz3277);
  CREATE(sail_int)(&zghz3278);
  CREATE(lbits)(&zghz3279);
  CREATE(sail_int)(&zghz3280);
  CREATE(lbits)(&zghz3281);
  CREATE(sail_int)(&zghz3282);
  CREATE(sail_int)(&zghz3283);
  CREATE(sail_int)(&zghz3284);
  CREATE(lbits)(&zghz3285);
  CREATE(sail_int)(&zghz3286);
  CREATE(lbits)(&zghz3287);
  CREATE(lbits)(&zghz3288);
  CREATE(lbits)(&zghz3289);
  CREATE(lbits)(&zghz3290);
  CREATE(lbits)(&zghz3291);
  CREATE(sail_int)(&zghz3292);
  CREATE(lbits)(&zghz3293);
  CREATE(sail_int)(&zghz3294);
  CREATE(lbits)(&zghz3295);
  CREATE(lbits)(&zghz3296);
  CREATE(sail_int)(&zghz3297);
  CREATE(lbits)(&zghz3298);
  CREATE(sail_int)(&zghz3299);
  CREATE(sail_int)(&zghz3300);
  CREATE(sail_int)(&zghz3301);
  CREATE(sail_int)(&zghz3302);
  CREATE(lbits)(&zghz3303);
  CREATE(sail_int)(&zghz3304);
  CREATE(lbits)(&zghz3305);
  CREATE(sail_int)(&zghz3306);
  CREATE(sail_int)(&zghz3307);
  CREATE(sail_int)(&zghz3308);
  CREATE(lbits)(&zghz3309);
  CREATE(sail_int)(&zghz3310);
  CREATE(lbits)(&zghz3311);
  CREATE(sail_int)(&zghz3312);
  CREATE(lbits)(&zghz3313);
  CREATE(lbits)(&zghz3314);
  CREATE(sail_int)(&zghz3315);
  CREATE(lbits)(&zghz3316);
  CREATE(sail_int)(&zghz3317);
  CREATE(sail_int)(&zghz3318);
  CREATE(sail_int)(&zghz3319);
  CREATE(sail_int)(&zghz3320);
  CREATE(sail_int)(&zghz3321);
  CREATE(sail_int)(&zghz3322);
  CREATE(sail_int)(&zghz3323);
  CREATE(sail_int)(&zghz3324);
  CREATE(sail_int)(&zghz3325);
  CREATE(lbits)(&zghz3326);
  CREATE(sail_int)(&zghz3327);
  CREATE(sail_int)(&zghz3328);
  CREATE(sail_int)(&zghz3329);
  CREATE(lbits)(&zghz3330);
  CREATE(sail_int)(&zghz3331);
  CREATE(sail_int)(&zghz3332);
  CREATE(sail_int)(&zghz3333);
  CREATE(lbits)(&zghz3334);
  CREATE(lbits)(&zghz3335);
}

static struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 sailgen_setCapBounds(struct zCapability zcap, uint64_t zbase, lbits ztop)
{
  __label__ end_function_291, end_block_exception_292;

  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz363;
  RECREATE(lbits)(&zghz3255);
  {
    RECREATE(lbits)(&zghz3334);
    CONVERT_OF(lbits, fbits)(&zghz3334, UINT64_C(0b0), UINT64_C(1) , true);
    RECREATE(lbits)(&zghz3335);
    CONVERT_OF(lbits, fbits)(&zghz3335, zbase, UINT64_C(64) , true);
    append(&zghz3255, zghz3334, zghz3335);
  }
  RECREATE(lbits)(&zghz3256);
  sub_bits(&zghz3256, ztop, zghz3255);
  int64_t ze;
  {
    int64_t zgaz3283;
    {
      uint64_t zgaz3282;
      {
        int64_t zgaz3281;
        {
          RECREATE(sail_int)(&zghz3331);
          CONVERT_OF(sail_int, mach_int)(&zghz3331, zcap_mantissa_width);
          RECREATE(sail_int)(&zghz3332);
          CONVERT_OF(sail_int, mach_int)(&zghz3332, INT64_C(1));
          RECREATE(sail_int)(&zghz3333);
          sub_int(&zghz3333, zghz3331, zghz3332);
          zgaz3281 = CONVERT_OF(mach_int, sail_int)(zghz3333);
        }
        {
          RECREATE(sail_int)(&zghz3328);
          CONVERT_OF(sail_int, mach_int)(&zghz3328, zcap_addr_width);
          RECREATE(sail_int)(&zghz3329);
          CONVERT_OF(sail_int, mach_int)(&zghz3329, zgaz3281);
          RECREATE(lbits)(&zghz3330);
          vector_subrange_lbits(&zghz3330, zghz3256, zghz3328, zghz3329);
          zgaz3282 = CONVERT_OF(fbits, lbits)(zghz3330, true);
        }
      }
      {
        RECREATE(lbits)(&zghz3326);
        CONVERT_OF(lbits, fbits)(&zghz3326, zgaz3282, UINT64_C(52) , true);
        RECREATE(sail_int)(&zghz3327);
        count_leading_zeros(&zghz3327, zghz3326);
        zgaz3283 = CONVERT_OF(mach_int, sail_int)(zghz3327);
      }
    }
    {
      RECREATE(sail_int)(&zghz3323);
      CONVERT_OF(sail_int, mach_int)(&zghz3323, zcap_max_E);
      RECREATE(sail_int)(&zghz3324);
      CONVERT_OF(sail_int, mach_int)(&zghz3324, zgaz3283);
      RECREATE(sail_int)(&zghz3325);
      sub_int(&zghz3325, zghz3323, zghz3324);
      ze = CONVERT_OF(mach_int, sail_int)(zghz3325);
    }
  }
  bool zie;
  {
    bool zgaz3280;
    {
      RECREATE(sail_int)(&zghz3321);
      CONVERT_OF(sail_int, mach_int)(&zghz3321, ze);
      RECREATE(sail_int)(&zghz3322);
      CONVERT_OF(sail_int, mach_int)(&zghz3322, INT64_C(0));
      zgaz3280 = sailgen_neq_int(zghz3321, zghz3322);
    }
    bool zgsz3219;
    if (zgaz3280) {  zgsz3219 = true;  } else {
      bool zgaz3279;
      zgaz3279 = not(zcap_use_L8);
      bool zgsz3218;
      if (zgaz3279) {
        fbits zgaz3278;
        {
          int64_t zgaz3277;
          {
            RECREATE(sail_int)(&zghz3318);
            CONVERT_OF(sail_int, mach_int)(&zghz3318, zcap_mantissa_width);
            RECREATE(sail_int)(&zghz3319);
            CONVERT_OF(sail_int, mach_int)(&zghz3319, INT64_C(2));
            RECREATE(sail_int)(&zghz3320);
            sub_int(&zghz3320, zghz3318, zghz3319);
            zgaz3277 = CONVERT_OF(mach_int, sail_int)(zghz3320);
          }
          {
            RECREATE(sail_int)(&zghz3317);
            CONVERT_OF(sail_int, mach_int)(&zghz3317, zgaz3277);
            zgaz3278 = bitvector_access(zghz3256, zghz3317);
          }
        }
        zgsz3218 = eq_bit(zgaz3278, UINT64_C(1));
      } else {  zgsz3218 = false;  }
      zgsz3219 = zgsz3218;
    }
    zie = zgsz3219;
  }
  uint64_t zBbits;
  {
    RECREATE(lbits)(&zghz3314);
    CONVERT_OF(lbits, fbits)(&zghz3314, zbase, UINT64_C(64) , true);
    RECREATE(sail_int)(&zghz3315);
    CONVERT_OF(sail_int, mach_int)(&zghz3315, zcap_mantissa_width);
    RECREATE(lbits)(&zghz3316);
    sail_truncate(&zghz3316, zghz3314, zghz3315);
    zBbits = CONVERT_OF(fbits, lbits)(zghz3316, true);
  }
  uint64_t zTbits;
  {
    RECREATE(sail_int)(&zghz3312);
    CONVERT_OF(sail_int, mach_int)(&zghz3312, zcap_mantissa_width);
    RECREATE(lbits)(&zghz3313);
    sail_truncate(&zghz3313, ztop, zghz3312);
    zTbits = CONVERT_OF(fbits, lbits)(zghz3313, true);
  }
  bool zlostSignificantTop;
  zlostSignificantTop = false;
  bool zlostSignificantBase;
  zlostSignificantBase = false;
  bool zincE;
  zincE = false;
  {
    unit zgsz3220;
    if (zie) {
      uint64_t zB_ie;
      {
        uint64_t zgaz3271;
        {
          int64_t zgaz3270;
          zgaz3270 = (ze + zcap_xe_width);
          {
            RECREATE(lbits)(&zghz3309);
            CONVERT_OF(lbits, fbits)(&zghz3309, zbase, UINT64_C(64) , true);
            RECREATE(sail_int)(&zghz3310);
            CONVERT_OF(sail_int, mach_int)(&zghz3310, zgaz3270);
            RECREATE(lbits)(&zghz3311);
            shiftr(&zghz3311, zghz3309, zghz3310);
            zgaz3271 = CONVERT_OF(fbits, lbits)(zghz3311, true);
          }
        }
        int64_t zgaz3272;
        {
          RECREATE(sail_int)(&zghz3306);
          CONVERT_OF(sail_int, mach_int)(&zghz3306, zcap_mantissa_width);
          RECREATE(sail_int)(&zghz3307);
          CONVERT_OF(sail_int, mach_int)(&zghz3307, zcap_xe_width);
          RECREATE(sail_int)(&zghz3308);
          sub_int(&zghz3308, zghz3306, zghz3307);
          zgaz3272 = CONVERT_OF(mach_int, sail_int)(zghz3308);
        }
        {
          RECREATE(lbits)(&zghz3303);
          CONVERT_OF(lbits, fbits)(&zghz3303, zgaz3271, UINT64_C(64) , true);
          RECREATE(sail_int)(&zghz3304);
          CONVERT_OF(sail_int, mach_int)(&zghz3304, zgaz3272);
          RECREATE(lbits)(&zghz3305);
          sail_truncate(&zghz3305, zghz3303, zghz3304);
          zB_ie = CONVERT_OF(fbits, lbits)(zghz3305, true);
        }
      }
      uint64_t zT_ie;
      {
        RECREATE(lbits)(&zghz3296);
        {
          int64_t zgaz3267;
          zgaz3267 = (ze + zcap_xe_width);
          {
            RECREATE(sail_int)(&zghz3302);
            CONVERT_OF(sail_int, mach_int)(&zghz3302, zgaz3267);
            shiftr(&zghz3296, ztop, zghz3302);
          }
        }
        int64_t zgaz3269;
        {
          RECREATE(sail_int)(&zghz3299);
          CONVERT_OF(sail_int, mach_int)(&zghz3299, zcap_mantissa_width);
          RECREATE(sail_int)(&zghz3300);
          CONVERT_OF(sail_int, mach_int)(&zghz3300, zcap_xe_width);
          RECREATE(sail_int)(&zghz3301);
          sub_int(&zghz3301, zghz3299, zghz3300);
          zgaz3269 = CONVERT_OF(mach_int, sail_int)(zghz3301);
        }
        {
          RECREATE(sail_int)(&zghz3297);
          CONVERT_OF(sail_int, mach_int)(&zghz3297, zgaz3269);
          RECREATE(lbits)(&zghz3298);
          sail_truncate(&zghz3298, zghz3296, zghz3297);
          zT_ie = CONVERT_OF(fbits, lbits)(zghz3298, true);
        }
      }
      RECREATE(lbits)(&zghz3258);
      {
        sbits zgaz3266;
        {
          int64_t zgaz3265;
          zgaz3265 = (ze + zcap_xe_width);
          {
            RECREATE(sail_int)(&zghz3294);
            CONVERT_OF(sail_int, mach_int)(&zghz3294, zgaz3265);
            RECREATE(lbits)(&zghz3295);
            sailgen_ones(&zghz3295, zghz3294);
            zgaz3266 = CONVERT_OF(sbits, lbits)(zghz3295, true);
          }
        }
        {
          RECREATE(sail_int)(&zghz3292);
          CONVERT_OF(sail_int, mach_int)(&zghz3292, INT64_C(65));
          RECREATE(lbits)(&zghz3293);
          CONVERT_OF(lbits, sbits)(&zghz3293, zgaz3266, true);
          sailgen_zzero_extend(&zghz3258, zghz3292, zghz3293);
        }
      }
      {
        RECREATE(lbits)(&zghz3290);
        and_bits(&zghz3290, zghz3255, zghz3258);
        RECREATE(lbits)(&zghz3291);
        CONVERT_OF(lbits, fbits)(&zghz3291, UINT64_C(0b0), UINT64_C(1) , true);
        append_64(&zghz3291, zghz3291, UINT64_C(0x0000000000000000));
        zlostSignificantBase = neq_bits(zghz3290, zghz3291);
        unit zgsz3225;
        zgsz3225 = UNIT;
      }
      {
        RECREATE(lbits)(&zghz3288);
        and_bits(&zghz3288, ztop, zghz3258);
        RECREATE(lbits)(&zghz3289);
        CONVERT_OF(lbits, fbits)(&zghz3289, UINT64_C(0b0), UINT64_C(1) , true);
        append_64(&zghz3289, zghz3289, UINT64_C(0x0000000000000000));
        zlostSignificantTop = neq_bits(zghz3288, zghz3289);
        unit zgsz3224;
        zgsz3224 = UNIT;
      }
      {
        unit zgsz3223;
        if (zlostSignificantTop) {
          {
            RECREATE(lbits)(&zghz3285);
            CONVERT_OF(lbits, fbits)(&zghz3285, zT_ie, UINT64_C(11) , true);
            RECREATE(sail_int)(&zghz3286);
            CONVERT_OF(sail_int, mach_int)(&zghz3286, INT64_C(1));
            RECREATE(lbits)(&zghz3287);
            add_bits_int(&zghz3287, zghz3285, zghz3286);
            zT_ie = CONVERT_OF(fbits, lbits)(zghz3287, true);
          }
          zgsz3223 = UNIT;
        } else {  zgsz3223 = UNIT;  }
      }
      uint64_t zlen_ie;
      zlen_ie = ((zT_ie - zB_ie) & UINT64_C(0b11111111111));
      {
        bool zgaz3251;
        {
          fbits zgaz3250;
          {
            int64_t zgaz3249;
            {
              int64_t zgaz3248;
              zgaz3248 = (zcap_xe_width + INT64_C(1));
              {
                RECREATE(sail_int)(&zghz3282);
                CONVERT_OF(sail_int, mach_int)(&zghz3282, zcap_mantissa_width);
                RECREATE(sail_int)(&zghz3283);
                CONVERT_OF(sail_int, mach_int)(&zghz3283, zgaz3248);
                RECREATE(sail_int)(&zghz3284);
                sub_int(&zghz3284, zghz3282, zghz3283);
                zgaz3249 = CONVERT_OF(mach_int, sail_int)(zghz3284);
              }
            }
            zgaz3250 = (UINT64_C(1) & (zlen_ie >> zgaz3249));
          }
          zgaz3251 = eq_bit(zgaz3250, UINT64_C(1));
        }
        unit zgsz3227;
        if (zgaz3251) {
          {
            zincE = true;
            unit zgsz3233;
            zgsz3233 = UNIT;
          }
          {
            bool zgsz3228;
            if (zlostSignificantBase) {  zgsz3228 = true;  } else {
              fbits zgaz3252;
              zgaz3252 = (UINT64_C(1) & (zB_ie >> INT64_C(0)));
              zgsz3228 = eq_bit(zgaz3252, UINT64_C(1));
            }
            zlostSignificantBase = zgsz3228;
            unit zgsz3232;
            zgsz3232 = UNIT;
          }
          {
            bool zgsz3229;
            if (zlostSignificantTop) {  zgsz3229 = true;  } else {
              fbits zgaz3253;
              zgaz3253 = (UINT64_C(1) & (zT_ie >> INT64_C(0)));
              zgsz3229 = eq_bit(zgaz3253, UINT64_C(1));
            }
            zlostSignificantTop = zgsz3229;
            unit zgsz3231;
            zgsz3231 = UNIT;
          }
          {
            uint64_t zgaz3256;
            {
              int64_t zgaz3255;
              {
                int64_t zgaz3254;
                zgaz3254 = (ze + zcap_xe_width);
                zgaz3255 = (zgaz3254 + INT64_C(1));
              }
              {
                RECREATE(lbits)(&zghz3279);
                CONVERT_OF(lbits, fbits)(&zghz3279, zbase, UINT64_C(64) , true);
                RECREATE(sail_int)(&zghz3280);
                CONVERT_OF(sail_int, mach_int)(&zghz3280, zgaz3255);
                RECREATE(lbits)(&zghz3281);
                shiftr(&zghz3281, zghz3279, zghz3280);
                zgaz3256 = CONVERT_OF(fbits, lbits)(zghz3281, true);
              }
            }
            int64_t zgaz3257;
            {
              RECREATE(sail_int)(&zghz3276);
              CONVERT_OF(sail_int, mach_int)(&zghz3276, zcap_mantissa_width);
              RECREATE(sail_int)(&zghz3277);
              CONVERT_OF(sail_int, mach_int)(&zghz3277, zcap_xe_width);
              RECREATE(sail_int)(&zghz3278);
              sub_int(&zghz3278, zghz3276, zghz3277);
              zgaz3257 = CONVERT_OF(mach_int, sail_int)(zghz3278);
            }
            {
              RECREATE(lbits)(&zghz3273);
              CONVERT_OF(lbits, fbits)(&zghz3273, zgaz3256, UINT64_C(64) , true);
              RECREATE(sail_int)(&zghz3274);
              CONVERT_OF(sail_int, mach_int)(&zghz3274, zgaz3257);
              RECREATE(lbits)(&zghz3275);
              sail_truncate(&zghz3275, zghz3273, zghz3274);
              zB_ie = CONVERT_OF(fbits, lbits)(zghz3275, true);
            }
            unit zgsz3230;
            zgsz3230 = UNIT;
          }
          int64_t zincT;
          if (zlostSignificantTop) {  zincT = INT64_C(1);  } else {  zincT = INT64_C(0);  }
          uint64_t zgaz3262;
          {
            RECREATE(lbits)(&zghz3266);
            {
              int64_t zgaz3259;
              {
                int64_t zgaz3258;
                zgaz3258 = (ze + zcap_xe_width);
                zgaz3259 = (zgaz3258 + INT64_C(1));
              }
              {
                RECREATE(sail_int)(&zghz3272);
                CONVERT_OF(sail_int, mach_int)(&zghz3272, zgaz3259);
                shiftr(&zghz3266, ztop, zghz3272);
              }
            }
            int64_t zgaz3261;
            {
              RECREATE(sail_int)(&zghz3269);
              CONVERT_OF(sail_int, mach_int)(&zghz3269, zcap_mantissa_width);
              RECREATE(sail_int)(&zghz3270);
              CONVERT_OF(sail_int, mach_int)(&zghz3270, zcap_xe_width);
              RECREATE(sail_int)(&zghz3271);
              sub_int(&zghz3271, zghz3269, zghz3270);
              zgaz3261 = CONVERT_OF(mach_int, sail_int)(zghz3271);
            }
            {
              RECREATE(sail_int)(&zghz3267);
              CONVERT_OF(sail_int, mach_int)(&zghz3267, zgaz3261);
              RECREATE(lbits)(&zghz3268);
              sail_truncate(&zghz3268, zghz3266, zghz3267);
              zgaz3262 = CONVERT_OF(fbits, lbits)(zghz3268, true);
            }
          }
          {
            RECREATE(lbits)(&zghz3263);
            CONVERT_OF(lbits, fbits)(&zghz3263, zgaz3262, UINT64_C(11) , true);
            RECREATE(sail_int)(&zghz3264);
            CONVERT_OF(sail_int, mach_int)(&zghz3264, zincT);
            RECREATE(lbits)(&zghz3265);
            add_bits_int(&zghz3265, zghz3263, zghz3264);
            zT_ie = CONVERT_OF(fbits, lbits)(zghz3265, true);
          }
          zgsz3227 = UNIT;
        } else {  zgsz3227 = UNIT;  }
      }
      {
        uint64_t zgaz3263;
        {
          RECREATE(sail_int)(&zghz3261);
          CONVERT_OF(sail_int, mach_int)(&zghz3261, zcap_xe_width);
          RECREATE(lbits)(&zghz3262);
          sailgen_zzeros_implicit(&zghz3262, zghz3261);
          zgaz3263 = CONVERT_OF(fbits, lbits)(zghz3262, true);
        }
        zBbits = (zB_ie << 3) | zgaz3263;
        unit zgsz3226;
        zgsz3226 = UNIT;
      }
      uint64_t zgaz3264;
      {
        RECREATE(sail_int)(&zghz3259);
        CONVERT_OF(sail_int, mach_int)(&zghz3259, zcap_xe_width);
        RECREATE(lbits)(&zghz3260);
        sailgen_zzeros_implicit(&zghz3260, zghz3259);
        zgaz3264 = CONVERT_OF(fbits, lbits)(zghz3260, true);
      }
      zTbits = (zT_ie << 3) | zgaz3264;
      zgsz3220 = UNIT;
    } else {  zgsz3220 = UNIT;  }
  }
  struct zCapability zghz3257;
  {
    int64_t zgaz3276;
    if (zincE) {
      bool zgaz3275;
      {
        int64_t zgaz3274;
        zgaz3274 = (ze + INT64_C(1));
        zgaz3275 = (zgaz3274 <= zcap_max_E);
      }
      if (zgaz3275) {  zgaz3276 = (ze + INT64_C(1));  } else {  zgaz3276 = zcap_min_E;  }
    } else {  zgaz3276 = ze;  }
    zghz3257 = zcap;
    zghz3257.zB = zBbits;
    zghz3257.zE = zgaz3276;
    zghz3257.zT = zTbits;
    zghz3257.zaddress = zbase;
    zghz3257.zinternal_exponent = zie;
  }
  bool zexact;
  {
    bool zgaz3273;
    {    if (zlostSignificantBase) {  zgaz3273 = true;  } else {  zgaz3273 = zlostSignificantTop;  }
    }
    zexact = not(zgaz3273);
  }
  zcbz363.ztup0 = zexact;
  zcbz363.ztup1 = zghz3257;








end_function_291: ;
  return zcbz363;
end_block_exception_292: ;
  struct zCapability zcbz3105 = { .zB = UINT64_C(0xdeadc0de), .zE = INT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zap_m = UINT64_C(0xdeadc0de), .zinternal_exponent = false, .zreserved_0 = UINT64_C(0xdeadc0de), .zreserved_1 = UINT64_C(0xdeadc0de), .zsd_perms = UINT64_C(0xdeadc0de), .zsealed = false, .ztag = false };
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz3104 = { .ztup0 = false, .ztup1 = zcbz3105 };
  return zcbz3104;
}



static void finish_sailgen_setCapBounds(void)
{
  KILL(lbits)(&zghz3335);
  KILL(lbits)(&zghz3334);
  KILL(sail_int)(&zghz3333);
  KILL(sail_int)(&zghz3332);
  KILL(sail_int)(&zghz3331);
  KILL(lbits)(&zghz3330);
  KILL(sail_int)(&zghz3329);
  KILL(sail_int)(&zghz3328);
  KILL(sail_int)(&zghz3327);
  KILL(lbits)(&zghz3326);
  KILL(sail_int)(&zghz3325);
  KILL(sail_int)(&zghz3324);
  KILL(sail_int)(&zghz3323);
  KILL(sail_int)(&zghz3322);
  KILL(sail_int)(&zghz3321);
  KILL(sail_int)(&zghz3320);
  KILL(sail_int)(&zghz3319);
  KILL(sail_int)(&zghz3318);
  KILL(sail_int)(&zghz3317);
  KILL(lbits)(&zghz3316);
  KILL(sail_int)(&zghz3315);
  KILL(lbits)(&zghz3314);
  KILL(lbits)(&zghz3313);
  KILL(sail_int)(&zghz3312);
  KILL(lbits)(&zghz3311);
  KILL(sail_int)(&zghz3310);
  KILL(lbits)(&zghz3309);
  KILL(sail_int)(&zghz3308);
  KILL(sail_int)(&zghz3307);
  KILL(sail_int)(&zghz3306);
  KILL(lbits)(&zghz3305);
  KILL(sail_int)(&zghz3304);
  KILL(lbits)(&zghz3303);
  KILL(sail_int)(&zghz3302);
  KILL(sail_int)(&zghz3301);
  KILL(sail_int)(&zghz3300);
  KILL(sail_int)(&zghz3299);
  KILL(lbits)(&zghz3298);
  KILL(sail_int)(&zghz3297);
  KILL(lbits)(&zghz3296);
  KILL(lbits)(&zghz3295);
  KILL(sail_int)(&zghz3294);
  KILL(lbits)(&zghz3293);
  KILL(sail_int)(&zghz3292);
  KILL(lbits)(&zghz3291);
  KILL(lbits)(&zghz3290);
  KILL(lbits)(&zghz3289);
  KILL(lbits)(&zghz3288);
  KILL(lbits)(&zghz3287);
  KILL(sail_int)(&zghz3286);
  KILL(lbits)(&zghz3285);
  KILL(sail_int)(&zghz3284);
  KILL(sail_int)(&zghz3283);
  KILL(sail_int)(&zghz3282);
  KILL(lbits)(&zghz3281);
  KILL(sail_int)(&zghz3280);
  KILL(lbits)(&zghz3279);
  KILL(sail_int)(&zghz3278);
  KILL(sail_int)(&zghz3277);
  KILL(sail_int)(&zghz3276);
  KILL(lbits)(&zghz3275);
  KILL(sail_int)(&zghz3274);
  KILL(lbits)(&zghz3273);
  KILL(sail_int)(&zghz3272);
  KILL(sail_int)(&zghz3271);
  KILL(sail_int)(&zghz3270);
  KILL(sail_int)(&zghz3269);
  KILL(lbits)(&zghz3268);
  KILL(sail_int)(&zghz3267);
  KILL(lbits)(&zghz3266);
  KILL(lbits)(&zghz3265);
  KILL(sail_int)(&zghz3264);
  KILL(lbits)(&zghz3263);
  KILL(lbits)(&zghz3262);
  KILL(sail_int)(&zghz3261);
  KILL(lbits)(&zghz3260);
  KILL(sail_int)(&zghz3259);
  KILL(lbits)(&zghz3258);

  KILL(lbits)(&zghz3256);
  KILL(lbits)(&zghz3255);
}

static bool sailgen_capIsSealed(struct zCapability);

static bool sailgen_capIsSealed(struct zCapability zcap)
{
  __label__ end_function_294, end_block_exception_295;

  bool zcbz364;
  zcbz364 = zcap.zsealed;
end_function_294: ;
  return zcbz364;
end_block_exception_295: ;

  return false;
}

static bool sailgen_permsMalformed(struct zCapability);

static bool sailgen_permsMalformed(struct zCapability zcap)
{
  __label__ end_function_297, end_block_exception_298;

  bool zcbz365;
  bool zgaz3285;
  {
    uint64_t zgaz3284;
    zgaz3284 = zcap.zap_m;
    zgaz3285 = sailgen_ap_m_encdec_forwards_matches(zgaz3284);
  }
  zcbz365 = not(zgaz3285);

end_function_297: ;
  return zcbz365;
end_block_exception_298: ;

  return false;
}

static void sailgen_getCapPermsAndMode(struct zoptionzIz8RArchPermszCEExecutionModez5z9zK *rop, struct zCapability);


static void sailgen_getCapPermsAndMode(struct zoptionzIz8RArchPermszCEExecutionModez5z9zK *zcbz366, struct zCapability zcap)
{
  __label__ end_function_300, end_block_exception_301, end_function_399;

  bool zgaz3286;
  zgaz3286 = sailgen_permsMalformed(zcap);
  if (zgaz3286) {  sailgen_NonezIz8RArchPermszCEExecutionModez5z9zK((*(&zcbz366)), UNIT);  } else {
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgaz3288;
    {
      uint64_t zgaz3287;
      zgaz3287 = zcap.zap_m;
      zgaz3288 = sailgen_ap_m_encdec_forwards(zgaz3287);
    }
    sailgen_SomezIz8RArchPermszCEExecutionModez5z9zK((*(&zcbz366)), zgaz3288);
  }

end_function_300: ;
  goto end_function_399;
end_block_exception_301: ;
  goto end_function_399;
end_function_399: ;
}

static struct zCapability sailgen_setCapPermsAndMode(struct zCapability, struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9);

struct zCapability zghz3336;
struct zArchPerms zghz3337;

static void startup_sailgen_setCapPermsAndMode(void)
{


}

static struct zCapability sailgen_setCapPermsAndMode(struct zCapability zcap, struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz3237)
{
  __label__ fundef_fail_302, fundef_body_303, end_function_304, end_block_exception_305;

  struct zCapability zghz3336;
  struct zArchPerms zghz3337;
  zghz3337 = zgsz3237.ztup0;
  enum zExecutionMode zmode;
  zmode = zgsz3237.ztup1;
  goto fundef_body_303;
fundef_fail_302: ;
  sail_match_failure("setCapPermsAndMode");
fundef_body_303: ;
  uint64_t zgaz3290;
  {
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgaz3289;
    zgaz3289 = sailgen_legalizzePermsAndMode(zghz3337, zmode);
    zgaz3290 = sailgen_ap_m_encdec_backwards(zgaz3289);
  }
  zghz3336 = zcap;
  zghz3336.zap_m = zgaz3290;

end_function_304: ;
  return zghz3336;
end_block_exception_305: ;
  struct zCapability zcbz3106 = { .zB = UINT64_C(0xdeadc0de), .zE = INT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zap_m = UINT64_C(0xdeadc0de), .zinternal_exponent = false, .zreserved_0 = UINT64_C(0xdeadc0de), .zreserved_1 = UINT64_C(0xdeadc0de), .zsd_perms = UINT64_C(0xdeadc0de), .zsealed = false, .ztag = false };
  return zcbz3106;
}



static void finish_sailgen_setCapPermsAndMode(void)
{


}

static void sailgen_getArchPerms(struct zoptionzIRArchPermszK *rop, struct zCapability);

struct zArchPerms zghz3338;

static void startup_sailgen_getArchPerms(void)
{
}

static void sailgen_getArchPerms(struct zoptionzIRArchPermszK *zcbz368, struct zCapability zcap)
{
  __label__ case_309, case_308, finish_match_307, end_function_310, end_block_exception_311, end_function_398;

  struct zoptionzIz8RArchPermszCEExecutionModez5z9zK zgaz3291;
  CREATE(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zgaz3291);
  sailgen_getCapPermsAndMode(&zgaz3291, zcap);
  struct zoptionzIRArchPermszK zgsz3239;
  CREATE(zoptionzIRArchPermszK)(&zgsz3239);
  {
    if (zgaz3291.kind != Kind_zNonezIz8RArchPermszCEExecutionModez5z9zK) goto case_309;
    sailgen_NonezIRArchPermszK(&zgsz3239, UNIT);
    goto finish_match_307;
  }
case_309: ;
  {
    /* complete */
    struct zArchPerms zghz3338;
    zghz3338 = zgaz3291.zSomezIz8RArchPermszCEExecutionModez5z9zK.ztup0;
    sailgen_SomezIRArchPermszK(&zgsz3239, zghz3338);
    goto finish_match_307;
  }
case_308: ;
finish_match_307: ;
  COPY(zoptionzIRArchPermszK)((*(&zcbz368)), zgsz3239);
  KILL(zoptionzIRArchPermszK)(&zgsz3239);
  KILL(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zgaz3291);
end_function_310: ;
  goto end_function_398;
end_block_exception_311: ;
  goto end_function_398;
end_function_398: ;
}



static void finish_sailgen_getArchPerms(void)
{
}

static struct zArchPerms sailgen_getArchPermsLegalizzed(struct zCapability);

struct zArchPerms zghz3339;
struct zArchPerms zghz3340;

static void startup_sailgen_getArchPermsLegalizzed(void)
{


}

static struct zArchPerms sailgen_getArchPermsLegalizzed(struct zCapability zcap)
{
  __label__ case_315, case_314, finish_match_313, end_function_316, end_block_exception_317;

  struct zArchPerms zghz3339;
  struct zoptionzIRArchPermszK zgaz3292;
  CREATE(zoptionzIRArchPermszK)(&zgaz3292);
  sailgen_getArchPerms(&zgaz3292, zcap);
  {
    if (zgaz3292.kind != Kind_zNonezIRArchPermszK) goto case_315;
    zghz3339 = znull_perms;
    goto finish_match_313;
  }
case_315: ;
  {
    /* complete */
    struct zArchPerms zghz3340;
    zghz3340 = zgaz3292.zSomezIRArchPermszK;
    zghz3339 = zghz3340;
    goto finish_match_313;
  }
case_314: ;
finish_match_313: ;
  KILL(zoptionzIRArchPermszK)(&zgaz3292);
end_function_316: ;
  return zghz3339;
end_block_exception_317: ;
  struct zArchPerms zcbz3107 = { .zASR = false, .zC = false, .zLM = false, .zR = false, .zW = false, .zX = false };
  return zcbz3107;
}



static void finish_sailgen_getArchPermsLegalizzed(void)
{


}

static enum zExecutionMode sailgen_getCapMode(struct zCapability);

static enum zExecutionMode sailgen_getCapMode(struct zCapability zcap)
{
  __label__ case_321, case_320, finish_match_319, end_function_322, end_block_exception_323;

  enum zExecutionMode zcbz370;
  struct zoptionzIz8RArchPermszCEExecutionModez5z9zK zgaz3293;
  CREATE(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zgaz3293);
  sailgen_getCapPermsAndMode(&zgaz3293, zcap);
  {
    if (zgaz3293.kind != Kind_zNonezIz8RArchPermszCEExecutionModez5z9zK) goto case_321;
    zcbz370 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    goto finish_match_319;
  }
case_321: ;
  {
    /* complete */
    enum zExecutionMode zmode;
    zmode = zgaz3293.zSomezIz8RArchPermszCEExecutionModez5z9zK.ztup1;
    zcbz370 = zmode;
    goto finish_match_319;
  }
case_320: ;
finish_match_319: ;
  KILL(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zgaz3293);
end_function_322: ;
  return zcbz370;
end_block_exception_323: ;

  return zCapPtrMode;
}

static void sailgen_getCapLength(sail_int *rop, struct zCapability);

sail_int zghz3341;
sail_int zghz3342;
sail_int zghz3343;

static void startup_sailgen_getCapLength(void)
{
  CREATE(sail_int)(&zghz3341);
  CREATE(sail_int)(&zghz3342);
  CREATE(sail_int)(&zghz3343);
}

static void sailgen_getCapLength(sail_int *zcbz371, struct zCapability zc)
{
  __label__ case_327, case_326, finish_match_325, end_function_328, end_block_exception_329, end_function_397;

  struct zoptionzIz8izCiz9zK zgaz3294;
  CREATE(zoptionzIz8izCiz9zK)(&zgaz3294);
  sailgen_getCapBounds(&zgaz3294, zc);
  RECREATE(sail_int)(&zghz3341);
  {
    if (zgaz3294.kind != Kind_zNonezIz8izCiz9zK) goto case_327;
    CONVERT_OF(sail_int, mach_int)(&zghz3341, INT64_C(0));
    goto finish_match_325;
  }
case_327: ;
  {
    /* complete */
    RECREATE(sail_int)(&zghz3342);
    COPY(sail_int)(&zghz3342, zgaz3294.zSomezIz8izCiz9zK.ztup0);
    RECREATE(sail_int)(&zghz3343);
    COPY(sail_int)(&zghz3343, zgaz3294.zSomezIz8izCiz9zK.ztup1);
    {
      bool zgaz3295;
      zgaz3295 = gteq(zghz3343, zghz3342);
      unit zgsz3249;
      zgsz3249 = sail_assert(zgaz3295, "src/cheri_cap_common.sail:433.42-433.43");
    }
    sub_int(&zghz3341, zghz3343, zghz3342);
    goto finish_match_325;
  }
case_326: ;
finish_match_325: ;
  COPY(sail_int)((*(&zcbz371)), zghz3341);
  KILL(zoptionzIz8izCiz9zK)(&zgaz3294);
end_function_328: ;
  goto end_function_397;
end_block_exception_329: ;
  goto end_function_397;
end_function_397: ;
}



static void finish_sailgen_getCapLength(void)
{
  KILL(sail_int)(&zghz3343);
  KILL(sail_int)(&zghz3342);
  KILL(sail_int)(&zghz3341);
}

static bool sailgen_inCapBoundsNoWrap(struct zCapability, uint64_t, sail_int);

sail_int zghz3344;
sail_int zghz3345;
sail_int zghz3346;
sail_int zghz3347;
lbits zghz3348;

static void startup_sailgen_inCapBoundsNoWrap(void)
{
  CREATE(sail_int)(&zghz3344);
  CREATE(sail_int)(&zghz3345);
  CREATE(sail_int)(&zghz3346);
  CREATE(sail_int)(&zghz3347);
  CREATE(lbits)(&zghz3348);
}

static bool sailgen_inCapBoundsNoWrap(struct zCapability zcap, uint64_t zaddr, sail_int zsizze)
{
  __label__ case_333, case_332, finish_match_331, end_function_334, end_block_exception_335;

  bool zcbz372;
  RECREATE(sail_int)(&zghz3344);
  {
    RECREATE(lbits)(&zghz3348);
    CONVERT_OF(lbits, fbits)(&zghz3348, zaddr, UINT64_C(64) , true);
    sail_unsigned(&zghz3344, zghz3348);
  }
  struct zoptionzIz8izCiz9zK zgaz3296;
  CREATE(zoptionzIz8izCiz9zK)(&zgaz3296);
  sailgen_getCapBounds(&zgaz3296, zcap);
  {
    if (zgaz3296.kind != Kind_zNonezIz8izCiz9zK) goto case_333;
    zcbz372 = false;
    goto finish_match_331;
  }
case_333: ;
  {
    /* complete */
    RECREATE(sail_int)(&zghz3345);
    COPY(sail_int)(&zghz3345, zgaz3296.zSomezIz8izCiz9zK.ztup0);
    RECREATE(sail_int)(&zghz3346);
    COPY(sail_int)(&zghz3346, zgaz3296.zSomezIz8izCiz9zK.ztup1);
    bool zgaz3298;
    zgaz3298 = gteq(zghz3344, zghz3345);
    bool zgsz3253;
    if (zgaz3298) {
      RECREATE(sail_int)(&zghz3347);
      add_int(&zghz3347, zghz3344, zsizze);
      zgsz3253 = lteq(zghz3347, zghz3346);
    } else {  zgsz3253 = false;  }
    zcbz372 = zgsz3253;
    goto finish_match_331;
  }
case_332: ;
finish_match_331: ;
  KILL(zoptionzIz8izCiz9zK)(&zgaz3296);
end_function_334: ;
  return zcbz372;
end_block_exception_335: ;

  return false;
}



static void finish_sailgen_inCapBoundsNoWrap(void)
{
  KILL(lbits)(&zghz3348);
  KILL(sail_int)(&zghz3347);
  KILL(sail_int)(&zghz3346);
  KILL(sail_int)(&zghz3345);
  KILL(sail_int)(&zghz3344);
}

static bool sailgen_capReservedValid(struct zCapability);

static bool sailgen_capReservedValid(struct zCapability zcap)
{
  __label__ end_function_337, end_block_exception_338;

  bool zcbz373;
  bool zgaz3301;
  {
    uint64_t zgaz3299;
    zgaz3299 = zcap.zreserved_0;
    zgaz3301 = (zgaz3299 == UINT64_C(0x0000));
  }
  bool zgsz3256;
  if (zgaz3301) {
    uint64_t zgaz3300;
    zgaz3300 = zcap.zreserved_1;
    zgsz3256 = (zgaz3300 == UINT64_C(0b0000000));
  } else {  zgsz3256 = false;  }
  zcbz373 = zgsz3256;

end_function_337: ;
  return zcbz373;
end_block_exception_338: ;

  return false;
}

static struct zCapability sailgen_clearTagIf(struct zCapability, bool);

struct zCapability zghz3349;

static void startup_sailgen_clearTagIf(void)
{
}

static struct zCapability sailgen_clearTagIf(struct zCapability zcap, bool zcond)
{
  __label__ end_function_340, end_block_exception_341;

  struct zCapability zghz3349;
  bool zgaz3303;
  {
    bool zgaz3302;
    zgaz3302 = zcap.ztag;
    bool zgsz3257;
    if (zgaz3302) {  zgsz3257 = not(zcond);  } else {  zgsz3257 = false;  }
    zgaz3303 = zgsz3257;
  }
  zghz3349 = zcap;
  zghz3349.ztag = zgaz3303;

end_function_340: ;
  return zghz3349;
end_block_exception_341: ;
  struct zCapability zcbz3108 = { .zB = UINT64_C(0xdeadc0de), .zE = INT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zap_m = UINT64_C(0xdeadc0de), .zinternal_exponent = false, .zreserved_0 = UINT64_C(0xdeadc0de), .zreserved_1 = UINT64_C(0xdeadc0de), .zsd_perms = UINT64_C(0xdeadc0de), .zsealed = false, .ztag = false };
  return zcbz3108;
}



static void finish_sailgen_clearTagIf(void)
{
}

static bool zsys_legalizze_malformed_permission_on_load;
static void create_letbind_25(void) {


  bool zgsz3259;
  zgsz3259 = false;
  zsys_legalizze_malformed_permission_on_load = zgsz3259;

let_end_342: ;
}
static void kill_letbind_25(void) {
}

static bool sailgen_capBoundsEqual(struct zCapability, struct zCapability);

sail_int zghz3350;
sail_int zghz3351;
sail_int zghz3352;
sail_int zghz3353;

static void startup_sailgen_capBoundsEqual(void)
{
  CREATE(sail_int)(&zghz3350);
  CREATE(sail_int)(&zghz3351);
  CREATE(sail_int)(&zghz3352);
  CREATE(sail_int)(&zghz3353);
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
  __label__ case_347, case_346, case_345, finish_match_344, end_function_348, end_block_exception_349;

  bool zcbz375;
  struct ztuple_z8z5unionz0zzoptionzzIzz8izzCizz9zzKzCz0z5unionz0zzoptionzzIzz8izzCizz9zzKz9 zgaz3306;
  CREATE(ztuple_z8z5unionz0zzoptionzzIzz8izzCizz9zzKzCz0z5unionz0zzoptionzzIzz8izzCizz9zzKz9)(&zgaz3306);
  {
    struct zoptionzIz8izCiz9zK zgaz3304;
    CREATE(zoptionzIz8izCiz9zK)(&zgaz3304);
    sailgen_getCapBounds(&zgaz3304, zc1);
    struct zoptionzIz8izCiz9zK zgaz3305;
    CREATE(zoptionzIz8izCiz9zK)(&zgaz3305);
    sailgen_getCapBounds(&zgaz3305, zc2);
    COPY(zoptionzIz8izCiz9zK)(&((&zgaz3306)->ztup0), zgaz3304);
    COPY(zoptionzIz8izCiz9zK)(&((&zgaz3306)->ztup1), zgaz3305);
    KILL(zoptionzIz8izCiz9zK)(&zgaz3305);
    KILL(zoptionzIz8izCiz9zK)(&zgaz3304);
  }
  {
    if (zgaz3306.ztup0.kind != Kind_zNonezIz8izCiz9zK) goto case_347;
    zcbz375 = false;
    goto finish_match_344;
  }
case_347: ;
  {
    if (zgaz3306.ztup1.kind != Kind_zNonezIz8izCiz9zK) goto case_346;
    zcbz375 = false;
    goto finish_match_344;
  }
case_346: ;
  {
    /* complete */
    /* complete */
    RECREATE(sail_int)(&zghz3350);
    COPY(sail_int)(&zghz3350, zgaz3306.ztup0.zSomezIz8izCiz9zK.ztup0);
    RECREATE(sail_int)(&zghz3351);
    COPY(sail_int)(&zghz3351, zgaz3306.ztup0.zSomezIz8izCiz9zK.ztup1);
    RECREATE(sail_int)(&zghz3352);
    COPY(sail_int)(&zghz3352, zgaz3306.ztup1.zSomezIz8izCiz9zK.ztup0);
    RECREATE(sail_int)(&zghz3353);
    COPY(sail_int)(&zghz3353, zgaz3306.ztup1.zSomezIz8izCiz9zK.ztup1);
    bool zgaz3307;
    zgaz3307 = eq_int(zghz3350, zghz3352);
    bool zgsz3262;
    if (zgaz3307) {  zgsz3262 = eq_int(zghz3351, zghz3353);  } else {  zgsz3262 = false;  }
    zcbz375 = zgsz3262;
    goto finish_match_344;
  }
case_345: ;
finish_match_344: ;
  KILL(ztuple_z8z5unionz0zzoptionzzIzz8izzCizz9zzKzCz0z5unionz0zzoptionzzIzz8izzCizz9zzKz9)(&zgaz3306);
end_function_348: ;
  return zcbz375;
end_block_exception_349: ;

  return false;
}



static void finish_sailgen_capBoundsEqual(void)
{
  KILL(sail_int)(&zghz3353);
  KILL(sail_int)(&zghz3352);
  KILL(sail_int)(&zghz3351);
  KILL(sail_int)(&zghz3350);
}

static struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 sailgen_setCapAddr(struct zCapability, uint64_t);

struct zCapability zghz3354;

static void startup_sailgen_setCapAddr(void)
{
}

static struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 sailgen_setCapAddr(struct zCapability zc, uint64_t zaddr)
{
  __label__ end_function_351, end_block_exception_352;

  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz376;
  struct zCapability zghz3354;
  {
    zghz3354 = zc;
    zghz3354.zaddress = zaddr;
  }
  bool zrepresentable;
  zrepresentable = sailgen_capBoundsEqual(zc, zghz3354);
  zcbz376.ztup0 = zrepresentable;
  zcbz376.ztup1 = zghz3354;

end_function_351: ;
  return zcbz376;
end_block_exception_352: ;
  struct zCapability zcbz3110 = { .zB = UINT64_C(0xdeadc0de), .zE = INT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zap_m = UINT64_C(0xdeadc0de), .zinternal_exponent = false, .zreserved_0 = UINT64_C(0xdeadc0de), .zreserved_1 = UINT64_C(0xdeadc0de), .zsd_perms = UINT64_C(0xdeadc0de), .zsealed = false, .ztag = false };
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz3109 = { .ztup0 = false, .ztup1 = zcbz3110 };
  return zcbz3109;
}



static void finish_sailgen_setCapAddr(void)
{
}

static void sailgen_capToString(sail_string *rop, struct zCapability);

lbits zghz3355;
lbits zghz3356;
lbits zghz3357;
struct zEncMetadata zghz3358;
struct zArchPerms zghz3359;
lbits zghz3360;
sail_int zghz3361;
sail_int zghz3362;
lbits zghz3363;
lbits zghz3364;
sail_int zghz3365;
lbits zghz3366;

static void startup_sailgen_capToString(void)
{
  CREATE(lbits)(&zghz3355);
  CREATE(lbits)(&zghz3356);
  CREATE(lbits)(&zghz3357);


  CREATE(lbits)(&zghz3360);
  CREATE(sail_int)(&zghz3361);
  CREATE(sail_int)(&zghz3362);
  CREATE(lbits)(&zghz3363);
  CREATE(lbits)(&zghz3364);
  CREATE(sail_int)(&zghz3365);
  CREATE(lbits)(&zghz3366);
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

static void sailgen_capToString(sail_string *zcbz377, struct zCapability zcap)
{
  __label__ case_358, finish_match_357, end_function_369, end_block_exception_370, end_function_396;

  struct ztuple_z8z5stringzCz0z5stringzCz0z5boolz9 zgaz3313;
  CREATE(ztuple_z8z5stringzCz0z5stringzCz0z5boolz9)(&zgaz3313);
  {
    __label__ case_356, case_355, finish_match_354;

    struct zoptionzIz8bzCbz9zK zgaz3308;
    CREATE(zoptionzIz8bzCbz9zK)(&zgaz3308);
    sailgen_getCapBoundsBits(&zgaz3308, zcap);
    {
      if (zgaz3308.kind != Kind_zNonezIz8bzCbz9zK) goto case_356;
      struct ztuple_z8z5stringzCz0z5stringzCz0z5boolz9 zgsz3271;
      CREATE(ztuple_z8z5stringzCz0z5stringzCz0z5boolz9)(&zgsz3271);
      COPY(sail_string)(&((&zgsz3271)->ztup0), "0 (malformed)");
      COPY(sail_string)(&((&zgsz3271)->ztup1), "0 (malformed)");
      zgsz3271.ztup2 = true;
      COPY(ztuple_z8z5stringzCz0z5stringzCz0z5boolz9)(&zgaz3313, zgsz3271);
      KILL(ztuple_z8z5stringzCz0z5stringzCz0z5boolz9)(&zgsz3271);
      goto finish_match_354;
    }
  case_356: ;
    {
      /* complete */
      uint64_t zbase;
      zbase = CONVERT_OF(fbits, lbits)(zgaz3308.zSomezIz8bzCbz9zK.ztup0, true);
      RECREATE(lbits)(&zghz3363);
      COPY(lbits)(&zghz3363, zgaz3308.zSomezIz8bzCbz9zK.ztup1);
      sail_string zgaz3311;
      CREATE(sail_string)(&zgaz3311);
      {
        RECREATE(lbits)(&zghz3366);
        CONVERT_OF(lbits, fbits)(&zghz3366, zbase, UINT64_C(64) , true);
        string_of_lbits(&zgaz3311, zghz3366);
      }
      sail_string zgaz3312;
      CREATE(sail_string)(&zgaz3312);
      {
        RECREATE(lbits)(&zghz3364);
        {
          int64_t zgaz3309;
          zgaz3309 = (zcap_len_width + INT64_C(3));
          {
            RECREATE(sail_int)(&zghz3365);
            CONVERT_OF(sail_int, mach_int)(&zghz3365, zgaz3309);
            sailgen_zzero_extend(&zghz3364, zghz3365, zghz3363);
          }
        }
        string_of_lbits(&zgaz3312, zghz3364);
      }
      struct ztuple_z8z5stringzCz0z5stringzCz0z5boolz9 zgsz3269;
      CREATE(ztuple_z8z5stringzCz0z5stringzCz0z5boolz9)(&zgsz3269);
      COPY(sail_string)(&((&zgsz3269)->ztup0), zgaz3311);
      COPY(sail_string)(&((&zgsz3269)->ztup1), zgaz3312);
      zgsz3269.ztup2 = false;
      COPY(ztuple_z8z5stringzCz0z5stringzCz0z5boolz9)(&zgaz3313, zgsz3269);
      KILL(ztuple_z8z5stringzCz0z5stringzCz0z5boolz9)(&zgsz3269);
      KILL(sail_string)(&zgaz3312);
      KILL(sail_string)(&zgaz3311);
      goto finish_match_354;
    }
  case_355: ;
  finish_match_354: ;
    KILL(zoptionzIz8bzCbz9zK)(&zgaz3308);
  }
  sail_string zgsz3273;
  CREATE(sail_string)(&zgsz3273);
  {
    __label__ case_363, finish_match_362;

    sail_string zbase_str;
    CREATE(sail_string)(&zbase_str);
    COPY(sail_string)(&zbase_str, zgaz3313.ztup0);
    sail_string ztop_str;
    CREATE(sail_string)(&ztop_str);
    COPY(sail_string)(&ztop_str, zgaz3313.ztup1);
    bool zbounds_malformed;
    zbounds_malformed = zgaz3313.ztup2;
    sail_string zlen_str;
    CREATE(sail_string)(&zlen_str);
    {
      RECREATE(lbits)(&zghz3360);
      {
        int64_t zgaz3367;
        zgaz3367 = (zcap_len_width + INT64_C(3));
        RECREATE(sail_int)(&zghz3361);
        sailgen_getCapLength(&zghz3361, zcap);
        {
          RECREATE(sail_int)(&zghz3362);
          CONVERT_OF(sail_int, mach_int)(&zghz3362, zgaz3367);
          sailgen_to_bits(&zghz3360, zghz3362, zghz3361);
        }
      }
      string_of_lbits(&zlen_str, zghz3360);
    }
    struct ztuple_z8z5stringzCz0z5boolz9 zgaz3332;
    CREATE(ztuple_z8z5stringzCz0z5boolz9)(&zgaz3332);
    {
      __label__ case_361, case_360, finish_match_359;

      struct zoptionzIRArchPermszK zgaz3314;
      CREATE(zoptionzIRArchPermszK)(&zgaz3314);
      sailgen_getArchPerms(&zgaz3314, zcap);
      {
        if (zgaz3314.kind != Kind_zNonezIRArchPermszK) goto case_361;
        struct ztuple_z8z5stringzCz0z5boolz9 zgsz3277;
        CREATE(ztuple_z8z5stringzCz0z5boolz9)(&zgsz3277);
        COPY(sail_string)(&((&zgsz3277)->ztup0), "(malformed)");
        zgsz3277.ztup1 = true;
        COPY(ztuple_z8z5stringzCz0z5boolz9)(&zgaz3332, zgsz3277);
        KILL(ztuple_z8z5stringzCz0z5boolz9)(&zgsz3277);
        goto finish_match_359;
      }
    case_361: ;
      {
        /* complete */
        struct zArchPerms zghz3359;
        zghz3359 = zgaz3314.zSomezIRArchPermszK;
        sail_string zgaz3331;
        CREATE(sail_string)(&zgaz3331);
        {
          sail_string zgaz3329;
          CREATE(sail_string)(&zgaz3329);
          {
            bool zgaz3315;
            zgaz3315 = zghz3359.zR;
            if (zgaz3315) {  COPY(sail_string)(&zgaz3329, "R");  } else {  COPY(sail_string)(&zgaz3329, "");  }
          }
          sail_string zgaz3330;
          CREATE(sail_string)(&zgaz3330);
          {
            sail_string zgaz3327;
            CREATE(sail_string)(&zgaz3327);
            {
              bool zgaz3316;
              zgaz3316 = zghz3359.zW;
              if (zgaz3316) {  COPY(sail_string)(&zgaz3327, "W");  } else {  COPY(sail_string)(&zgaz3327, "");  }
            }
            sail_string zgaz3328;
            CREATE(sail_string)(&zgaz3328);
            {
              sail_string zgaz3325;
              CREATE(sail_string)(&zgaz3325);
              {
                bool zgaz3317;
                zgaz3317 = zghz3359.zC;
                if (zgaz3317) {  COPY(sail_string)(&zgaz3325, "C");  } else {  COPY(sail_string)(&zgaz3325, "");  }
              }
              sail_string zgaz3326;
              CREATE(sail_string)(&zgaz3326);
              {
                sail_string zgaz3323;
                CREATE(sail_string)(&zgaz3323);
                {
                  bool zgaz3318;
                  zgaz3318 = zghz3359.zX;
                  if (zgaz3318) {  COPY(sail_string)(&zgaz3323, "X");  } else {  COPY(sail_string)(&zgaz3323, "");  }
                }
                sail_string zgaz3324;
                CREATE(sail_string)(&zgaz3324);
                {
                  sail_string zgaz3321;
                  CREATE(sail_string)(&zgaz3321);
                  {
                    bool zgaz3319;
                    zgaz3319 = zghz3359.zLM;
                    if (zgaz3319) {  COPY(sail_string)(&zgaz3321, "Lm");  } else {  COPY(sail_string)(&zgaz3321, "");  }
                  }
                  sail_string zgaz3322;
                  CREATE(sail_string)(&zgaz3322);
                  {
                    bool zgaz3320;
                    zgaz3320 = zghz3359.zASR;
                    if (zgaz3320) {  COPY(sail_string)(&zgaz3322, "Asr");  } else {
                      COPY(sail_string)(&zgaz3322, "");
                    }
                  }
                  concat_str(&zgaz3324, zgaz3321, zgaz3322);
                  KILL(sail_string)(&zgaz3322);
                  KILL(sail_string)(&zgaz3321);
                }
                concat_str(&zgaz3326, zgaz3323, zgaz3324);
                KILL(sail_string)(&zgaz3324);
                KILL(sail_string)(&zgaz3323);
              }
              concat_str(&zgaz3328, zgaz3325, zgaz3326);
              KILL(sail_string)(&zgaz3326);
              KILL(sail_string)(&zgaz3325);
            }
            concat_str(&zgaz3330, zgaz3327, zgaz3328);
            KILL(sail_string)(&zgaz3328);
            KILL(sail_string)(&zgaz3327);
          }
          concat_str(&zgaz3331, zgaz3329, zgaz3330);
          KILL(sail_string)(&zgaz3330);
          KILL(sail_string)(&zgaz3329);
        }
        struct ztuple_z8z5stringzCz0z5boolz9 zgsz3275;
        CREATE(ztuple_z8z5stringzCz0z5boolz9)(&zgsz3275);
        COPY(sail_string)(&((&zgsz3275)->ztup0), zgaz3331);
        zgsz3275.ztup1 = false;
        COPY(ztuple_z8z5stringzCz0z5boolz9)(&zgaz3332, zgsz3275);
        KILL(ztuple_z8z5stringzCz0z5boolz9)(&zgsz3275);
        KILL(sail_string)(&zgaz3331);
        goto finish_match_359;
      }
    case_360: ;
    finish_match_359: ;
      KILL(zoptionzIRArchPermszK)(&zgaz3314);
    }
    sail_string zgsz3279;
    CREATE(sail_string)(&zgsz3279);
    {
      __label__ case_365, finish_match_364;

      sail_string zarch_perms_str;
      CREATE(sail_string)(&zarch_perms_str);
      COPY(sail_string)(&zarch_perms_str, zgaz3332.ztup0);
      bool zperms_malformed;
      zperms_malformed = zgaz3332.ztup1;
      struct ztuple_z8z5stringzCz0z5boolz9 zgaz3335;
      CREATE(ztuple_z8z5stringzCz0z5boolz9)(&zgaz3335);
      {
        bool zgaz3334;
        {
          bool zgaz3333;
          zgaz3333 = sailgen_capReservedValid(zcap);
          zgaz3334 = not(zgaz3333);
        }
        if (zgaz3334) {
          struct ztuple_z8z5stringzCz0z5boolz9 zgsz3281;
          CREATE(ztuple_z8z5stringzCz0z5boolz9)(&zgsz3281);
          COPY(sail_string)(&((&zgsz3281)->ztup0), " (has non-zero reserved bits)");
          zgsz3281.ztup1 = true;
          COPY(ztuple_z8z5stringzCz0z5boolz9)(&zgaz3335, zgsz3281);
          KILL(ztuple_z8z5stringzCz0z5boolz9)(&zgsz3281);
        } else {
          struct ztuple_z8z5stringzCz0z5boolz9 zgsz3280;
          CREATE(ztuple_z8z5stringzCz0z5boolz9)(&zgsz3280);
          COPY(sail_string)(&((&zgsz3280)->ztup0), "");
          zgsz3280.ztup1 = false;
          COPY(ztuple_z8z5stringzCz0z5boolz9)(&zgaz3335, zgsz3280);
          KILL(ztuple_z8z5stringzCz0z5boolz9)(&zgsz3280);
        }
      }
      sail_string zgsz3282;
      CREATE(sail_string)(&zgsz3282);
      {
        sail_string zres_str;
        CREATE(sail_string)(&zres_str);
        COPY(sail_string)(&zres_str, zgaz3335.ztup0);
        bool zreserved_malformed;
        zreserved_malformed = zgaz3335.ztup1;
        sail_string zraw_str;
        CREATE(sail_string)(&zraw_str);
        {
          bool zgaz3363;
          {
            if (zbounds_malformed) {  zgaz3363 = true;  } else {
              bool zgsz3283;
              if (zperms_malformed) {  zgsz3283 = true;  } else {  zgsz3283 = zreserved_malformed;  }
              zgaz3363 = zgsz3283;
            }
          }
          if (zgaz3363) {
            sail_string zgaz3366;
            CREATE(sail_string)(&zgaz3366);
            {
              uint64_t zgaz3365;
              {
                struct zEncMetadata zghz3358;
                zghz3358 = sailgen_capToMetadataBits(zcap);
                zgaz3365 = zghz3358.zbits;
              }
              {
                RECREATE(lbits)(&zghz3357);
                CONVERT_OF(lbits, fbits)(&zghz3357, zgaz3365, UINT64_C(64) , true);
                string_of_lbits(&zgaz3366, zghz3357);
              }
            }
            concat_str(&zraw_str, " raw:", zgaz3366);
            KILL(sail_string)(&zgaz3366);
          } else {  COPY(sail_string)(&zraw_str, "");  }
        }
        sail_string zcheri_mode;
        CREATE(sail_string)(&zcheri_mode);
        {
          __label__ case_368, case_367, finish_match_366;

          enum zExecutionMode zgaz3362;
          zgaz3362 = sailgen_getCapMode(zcap);
          {
            if ((zCapPtrMode != zgaz3362)) goto case_368;
            COPY(sail_string)(&zcheri_mode, "CapPtrMode");
            goto finish_match_366;
          }
        case_368: ;
          {
            /* complete */
            COPY(sail_string)(&zcheri_mode, "Integer");
            goto finish_match_366;
          }
        case_367: ;
        finish_match_366: ;
        }
        sail_string zgaz3361;
        CREATE(sail_string)(&zgaz3361);
        {
          sail_string zgaz3359;
          CREATE(sail_string)(&zgaz3359);
          {
            bool zgaz3336;
            zgaz3336 = zcap.ztag;
            if (zgaz3336) {  COPY(sail_string)(&zgaz3359, "1");  } else {  COPY(sail_string)(&zgaz3359, "0");  }
          }
          sail_string zgaz3360;
          CREATE(sail_string)(&zgaz3360);
          {
            sail_string zgaz3358;
            CREATE(sail_string)(&zgaz3358);
            {
              sail_string zgaz3356;
              CREATE(sail_string)(&zgaz3356);
              {
                uint64_t zgaz3337;
                zgaz3337 = zcap.zaddress;
                {
                  RECREATE(lbits)(&zghz3356);
                  CONVERT_OF(lbits, fbits)(&zghz3356, zgaz3337, UINT64_C(64) , true);
                  string_of_lbits(&zgaz3356, zghz3356);
                }
              }
              sail_string zgaz3357;
              CREATE(sail_string)(&zgaz3357);
              {
                sail_string zgaz3355;
                CREATE(sail_string)(&zgaz3355);
                {
                  sail_string zgaz3354;
                  CREATE(sail_string)(&zgaz3354);
                  {
                    sail_string zgaz3353;
                    CREATE(sail_string)(&zgaz3353);
                    {
                      sail_string zgaz3352;
                      CREATE(sail_string)(&zgaz3352);
                      {
                        sail_string zgaz3351;
                        CREATE(sail_string)(&zgaz3351);
                        {
                          sail_string zgaz3350;
                          CREATE(sail_string)(&zgaz3350);
                          {
                            sail_string zgaz3349;
                            CREATE(sail_string)(&zgaz3349);
                            {
                              sail_string zgaz3347;
                              CREATE(sail_string)(&zgaz3347);
                              {
                                bool zgaz3338;
                                zgaz3338 = zcap.zsealed;
                                if (zgaz3338) {  COPY(sail_string)(&zgaz3347, "1");  } else {
                                  COPY(sail_string)(&zgaz3347, "0");
                                }
                              }
                              sail_string zgaz3348;
                              CREATE(sail_string)(&zgaz3348);
                              {
                                sail_string zgaz3346;
                                CREATE(sail_string)(&zgaz3346);
                                {
                                  sail_string zgaz3345;
                                  CREATE(sail_string)(&zgaz3345);
                                  {
                                    sail_string zgaz3344;
                                    CREATE(sail_string)(&zgaz3344);
                                    {
                                      sail_string zgaz3342;
                                      CREATE(sail_string)(&zgaz3342);
                                      {
                                        uint64_t zgaz3339;
                                        zgaz3339 = zcap.zsd_perms;
                                        {
                                          RECREATE(lbits)(&zghz3355);
                                          CONVERT_OF(lbits, fbits)(&zghz3355, zgaz3339, UINT64_C(4) , true);
                                          string_of_lbits(&zgaz3342, zghz3355);
                                        }
                                      }
                                      sail_string zgaz3343;
                                      CREATE(sail_string)(&zgaz3343);
                                      {
                                        sail_string zgaz3341;
                                        CREATE(sail_string)(&zgaz3341);
                                        {
                                          sail_string zgaz3340;
                                          CREATE(sail_string)(&zgaz3340);
                                          concat_str(&zgaz3340, zraw_str, zres_str);
                                          concat_str(&zgaz3341, zcheri_mode, zgaz3340);
                                          KILL(sail_string)(&zgaz3340);
                                        }
                                        concat_str(&zgaz3343, " mode:", zgaz3341);
                                        KILL(sail_string)(&zgaz3341);
                                      }
                                      concat_str(&zgaz3344, zgaz3342, zgaz3343);
                                      KILL(sail_string)(&zgaz3343);
                                      KILL(sail_string)(&zgaz3342);
                                    }
                                    concat_str(&zgaz3345, " sdperms:", zgaz3344);
                                    KILL(sail_string)(&zgaz3344);
                                  }
                                  concat_str(&zgaz3346, zarch_perms_str, zgaz3345);
                                  KILL(sail_string)(&zgaz3345);
                                }
                                concat_str(&zgaz3348, " perms:", zgaz3346);
                                KILL(sail_string)(&zgaz3346);
                              }
                              concat_str(&zgaz3349, zgaz3347, zgaz3348);
                              KILL(sail_string)(&zgaz3348);
                              KILL(sail_string)(&zgaz3347);
                            }
                            concat_str(&zgaz3350, " sealed:", zgaz3349);
                            KILL(sail_string)(&zgaz3349);
                          }
                          concat_str(&zgaz3351, zlen_str, zgaz3350);
                          KILL(sail_string)(&zgaz3350);
                        }
                        concat_str(&zgaz3352, " length:", zgaz3351);
                        KILL(sail_string)(&zgaz3351);
                      }
                      concat_str(&zgaz3353, ztop_str, zgaz3352);
                      KILL(sail_string)(&zgaz3352);
                    }
                    concat_str(&zgaz3354, " top:", zgaz3353);
                    KILL(sail_string)(&zgaz3353);
                  }
                  concat_str(&zgaz3355, zbase_str, zgaz3354);
                  KILL(sail_string)(&zgaz3354);
                }
                concat_str(&zgaz3357, " base:", zgaz3355);
                KILL(sail_string)(&zgaz3355);
              }
              concat_str(&zgaz3358, zgaz3356, zgaz3357);
              KILL(sail_string)(&zgaz3357);
              KILL(sail_string)(&zgaz3356);
            }
            concat_str(&zgaz3360, " address:", zgaz3358);
            KILL(sail_string)(&zgaz3358);
          }
          concat_str(&zgaz3361, zgaz3359, zgaz3360);
          KILL(sail_string)(&zgaz3360);
          KILL(sail_string)(&zgaz3359);
        }
        concat_str(&zgsz3282, "t:", zgaz3361);
        KILL(sail_string)(&zgaz3361);
        KILL(sail_string)(&zcheri_mode);
        KILL(sail_string)(&zraw_str);
        KILL(sail_string)(&zres_str);
        goto finish_match_364;
      }
    case_365: ;
      sail_match_failure("capToString");
    finish_match_364: ;
      COPY(sail_string)(&zgsz3279, zgsz3282);
      KILL(sail_string)(&zgsz3282);
      KILL(ztuple_z8z5stringzCz0z5boolz9)(&zgaz3335);
      KILL(sail_string)(&zarch_perms_str);
      goto finish_match_362;
    }
  case_363: ;
    sail_match_failure("capToString");
  finish_match_362: ;
    COPY(sail_string)(&zgsz3273, zgsz3279);
    KILL(sail_string)(&zgsz3279);
    KILL(ztuple_z8z5stringzCz0z5boolz9)(&zgaz3332);
    KILL(sail_string)(&zlen_str);
    KILL(sail_string)(&ztop_str);
    KILL(sail_string)(&zbase_str);
    goto finish_match_357;
  }
case_358: ;
  sail_match_failure("capToString");
finish_match_357: ;
  COPY(sail_string)((*(&zcbz377)), zgsz3273);
  KILL(sail_string)(&zgsz3273);
  KILL(ztuple_z8z5stringzCz0z5stringzCz0z5boolz9)(&zgaz3313);
end_function_369: ;
  goto end_function_396;
end_block_exception_370: ;
  goto end_function_396;
end_function_396: ;
}



static void finish_sailgen_capToString(void)
{
  KILL(lbits)(&zghz3366);
  KILL(sail_int)(&zghz3365);
  KILL(lbits)(&zghz3364);
  KILL(lbits)(&zghz3363);
  KILL(sail_int)(&zghz3362);
  KILL(sail_int)(&zghz3361);
  KILL(lbits)(&zghz3360);


  KILL(lbits)(&zghz3357);
  KILL(lbits)(&zghz3356);
  KILL(lbits)(&zghz3355);
}

static uint64_t sailgen_getRepresentableAlignmentMask(uint64_t);

struct zCapability zghz3367;
sail_int zghz3368;
lbits zghz3369;
lbits zghz3370;
sail_int zghz3371;
lbits zghz3372;
lbits zghz3373;
lbits zghz3374;
lbits zghz3375;

static void startup_sailgen_getRepresentableAlignmentMask(void)
{

  CREATE(sail_int)(&zghz3368);
  CREATE(lbits)(&zghz3369);
  CREATE(lbits)(&zghz3370);
  CREATE(sail_int)(&zghz3371);
  CREATE(lbits)(&zghz3372);
  CREATE(lbits)(&zghz3373);
  CREATE(lbits)(&zghz3374);
  CREATE(lbits)(&zghz3375);
}

static uint64_t sailgen_getRepresentableAlignmentMask(uint64_t zlen)
{
  __label__ case_373, finish_match_372, end_function_374, end_block_exception_375;

  uint64_t zcbz378;
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3371;
  {
    RECREATE(lbits)(&zghz3373);
    {
      RECREATE(lbits)(&zghz3374);
      CONVERT_OF(lbits, fbits)(&zghz3374, UINT64_C(0b0), UINT64_C(1) , true);
      RECREATE(lbits)(&zghz3375);
      CONVERT_OF(lbits, fbits)(&zghz3375, zlen, UINT64_C(64) , true);
      append(&zghz3373, zghz3374, zghz3375);
    }
    zgaz3371 = sailgen_setCapBounds(zinfinite_cap, UINT64_C(0x0000000000000000), zghz3373);
  }
  {
    bool z_exact;
    z_exact = zgaz3371.ztup0;
    struct zCapability zghz3367;
    zghz3367 = zgaz3371.ztup1;
    bool zgaz3372;
    zgaz3372 = zghz3367.zinternal_exponent;
    if (zgaz3372) {
      int64_t ze;
      ze = zghz3367.zE;
      {
        bool zgaz3373;
        zgaz3373 = (ze >= INT64_C(0));
        unit zgsz3293;
        zgsz3293 = sail_assert(zgaz3373, "src/cheri_cap_common.sail:623.17-623.18");
      }
      uint64_t zgaz3376;
      {
        sbits zgaz3375;
        {
          int64_t zgaz3374;
          zgaz3374 = (ze + zcap_xe_width);
          {
            RECREATE(sail_int)(&zghz3371);
            CONVERT_OF(sail_int, mach_int)(&zghz3371, zgaz3374);
            RECREATE(lbits)(&zghz3372);
            sailgen_ones(&zghz3372, zghz3371);
            zgaz3375 = CONVERT_OF(sbits, lbits)(zghz3372, true);
          }
        }
        {
          RECREATE(sail_int)(&zghz3368);
          CONVERT_OF(sail_int, mach_int)(&zghz3368, INT64_C(64));
          RECREATE(lbits)(&zghz3369);
          CONVERT_OF(lbits, sbits)(&zghz3369, zgaz3375, true);
          RECREATE(lbits)(&zghz3370);
          sailgen_zzero_extend(&zghz3370, zghz3368, zghz3369);
          zgaz3376 = CONVERT_OF(fbits, lbits)(zghz3370, true);
        }
      }
      zcbz378 = (~(zgaz3376) & UINT64_C(0xFFFFFFFFFFFFFFFF));
    } else {  zcbz378 = UINT64_C(0xFFFFFFFFFFFFFFFF);  }
    goto finish_match_372;
  }
case_373: ;
  sail_match_failure("getRepresentableAlignmentMask");
finish_match_372: ;

end_function_374: ;
  return zcbz378;
end_block_exception_375: ;

  return UINT64_C(0xdeadc0de);
}



static void finish_sailgen_getRepresentableAlignmentMask(void)
{
  KILL(lbits)(&zghz3375);
  KILL(lbits)(&zghz3374);
  KILL(lbits)(&zghz3373);
  KILL(lbits)(&zghz3372);
  KILL(sail_int)(&zghz3371);
  KILL(lbits)(&zghz3370);
  KILL(lbits)(&zghz3369);
  KILL(sail_int)(&zghz3368);

}

static uint64_t sailgen_getRepresentableLength(uint64_t);

static uint64_t sailgen_getRepresentableLength(uint64_t zlen)
{
  __label__ end_function_377, end_block_exception_378;

  uint64_t zcbz379;
  uint64_t zm;
  zm = sailgen_getRepresentableAlignmentMask(zlen);
  uint64_t zgaz3378;
  {
    uint64_t zgaz3377;
    zgaz3377 = (~(zm) & UINT64_C(0xFFFFFFFFFFFFFFFF));
    zgaz3378 = ((zlen + zgaz3377) & UINT64_C(0xFFFFFFFFFFFFFFFF));
  }
  zcbz379 = (zgaz3378 & zm);


end_function_377: ;
  return zcbz379;
end_block_exception_378: ;

  return UINT64_C(0xdeadc0de);
}

static struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 sailgen_doCSetBounds(struct zCapability, lbits);

lbits zghz3376;
struct zCapability zghz3377;
struct zCapability zghz3378;
struct zCapability zghz3379;
sail_int zghz3380;
lbits zghz3381;
sail_int zghz3382;
lbits zghz3383;

static void startup_sailgen_doCSetBounds(void)
{
  CREATE(lbits)(&zghz3376);



  CREATE(sail_int)(&zghz3380);
  CREATE(lbits)(&zghz3381);
  CREATE(sail_int)(&zghz3382);
  CREATE(lbits)(&zghz3383);
}

static struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 sailgen_doCSetBounds(struct zCapability zinput, lbits zreq_len)
{
  __label__ case_381, finish_match_380, end_function_384, end_block_exception_385;

  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz380;
  uint64_t znewBase;
  znewBase = zinput.zaddress;
  RECREATE(lbits)(&zghz3376);
  {
    RECREATE(lbits)(&zghz3381);
    {
      RECREATE(sail_int)(&zghz3382);
      CONVERT_OF(sail_int, mach_int)(&zghz3382, INT64_C(65));
      RECREATE(lbits)(&zghz3383);
      CONVERT_OF(lbits, fbits)(&zghz3383, znewBase, UINT64_C(64) , true);
      sailgen_zzero_extend(&zghz3381, zghz3382, zghz3383);
    }
    add_bits(&zghz3376, zghz3381, zreq_len);
  }
  bool zinBounds;
  {
    RECREATE(sail_int)(&zghz3380);
    sail_unsigned(&zghz3380, zreq_len);
    zinBounds = sailgen_inCapBoundsNoWrap(zinput, znewBase, zghz3380);
  }
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3379;
  zgaz3379 = sailgen_setCapBounds(zinput, znewBase, zghz3376);
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgsz3294;
  {
    __label__ case_383, finish_match_382;

    bool zexact;
    zexact = zgaz3379.ztup0;
    struct zCapability zghz3377;
    zghz3377 = zgaz3379.ztup1;
    bool zcond;
    {
      bool zgaz3386;
      {
        bool zgaz3382;
        {    if (zinBounds) {  zgaz3382 = zexact;  } else {  zgaz3382 = false;  }
        }
        zgaz3386 = not(zgaz3382);
      }
      bool zgsz3298;
      if (zgaz3386) {  zgsz3298 = true;  } else {
        bool zgaz3385;
        zgaz3385 = sailgen_boundsMalformed(zghz3377);
        bool zgsz3297;
        if (zgaz3385) {  zgsz3297 = true;  } else {
          bool zgaz3384;
          {
            bool zgaz3383;
            zgaz3383 = sailgen_capReservedValid(zghz3377);
            zgaz3384 = not(zgaz3383);
          }
          bool zgsz3296;
          if (zgaz3384) {  zgsz3296 = true;  } else {  zgsz3296 = sailgen_capIsSealed(zghz3377);  }
          zgsz3297 = zgsz3296;
        }
        zgsz3298 = zgsz3297;
      }
      zcond = zgsz3298;
    }
    struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3380;
    zgaz3380 = sailgen_setCapBounds(zinput, znewBase, zghz3376);
    struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgsz3299;
    {
      bool zexactshadowz324;
      zexactshadowz324 = zgaz3380.ztup0;
      struct zCapability zghz3378;
      zghz3378 = zgaz3380.ztup1;
      struct zCapability zghz3379;
      zghz3379 = sailgen_clearTagIf(zghz3378, zcond);
      zgsz3299.ztup0 = zexactshadowz324;
      zgsz3299.ztup1 = zghz3379;
      goto finish_match_382;
    }
  case_383: ;
    sail_match_failure("doCSetBounds");
  finish_match_382: ;
    zgsz3294 = zgsz3299;
    goto finish_match_380;
  }
case_381: ;
  sail_match_failure("doCSetBounds");
finish_match_380: ;
  zcbz380 = zgsz3294;




end_function_384: ;
  return zcbz380;
end_block_exception_385: ;
  struct zCapability zcbz3112 = { .zB = UINT64_C(0xdeadc0de), .zE = INT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zap_m = UINT64_C(0xdeadc0de), .zinternal_exponent = false, .zreserved_0 = UINT64_C(0xdeadc0de), .zreserved_1 = UINT64_C(0xdeadc0de), .zsd_perms = UINT64_C(0xdeadc0de), .zsealed = false, .ztag = false };
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz3111 = { .ztup0 = false, .ztup1 = zcbz3112 };
  return zcbz3111;
}



static void finish_sailgen_doCSetBounds(void)
{
  KILL(lbits)(&zghz3383);
  KILL(sail_int)(&zghz3382);
  KILL(lbits)(&zghz3381);
  KILL(sail_int)(&zghz3380);



  KILL(lbits)(&zghz3376);
}

// register TestCap
static struct zCapability zTestCap;

// register TestAddr
static uint64_t zTestAddr;

// register TestLen
static lbits zTestLen;

static unit sailgen_main(unit);

struct zCapability zghz3384;
struct zCapability zghz3385;
struct zCapability zghz3386;
struct zCapability zghz3387;
struct zArchPerms zghz3388;
struct zEncMetadata zghz3389;
lbits zghz3390;
lbits zghz3391;
lbits zghz3392;

static void startup_sailgen_main(void)
{






  CREATE(lbits)(&zghz3390);
  CREATE(lbits)(&zghz3391);
  CREATE(lbits)(&zghz3392);
}




static unit sailgen_main(unit zgsz3303)
{
  __label__ case_388, finish_match_387, end_function_391, end_block_exception_392;

  unit zcbz381;
  {
    zTestCap = znull_cap;
    unit zgsz3304;
    zgsz3304 = UNIT;
  }
  struct zCapability zghz3384;
  {
    bool zgaz3395;
    zgaz3395 = zTestCap.ztag;
    RECREATE(lbits)(&zghz3390);
    {
      RECREATE(lbits)(&zghz3391);
      CONVERT_OF(lbits, fbits)(&zghz3391, zTestAddr, UINT64_C(64) , true);
      RECREATE(lbits)(&zghz3392);
      CONVERT_OF(lbits, fbits)(&zghz3392, zTestAddr, UINT64_C(64) , true);
      append(&zghz3390, zghz3391, zghz3392);
    }
    zghz3384 = sailgen_bitsToCap(zgaz3395, zghz3390);
  }
  uint64_t zcap_bits;
  {
    struct zEncMetadata zghz3389;
    zghz3389 = sailgen_capToMetadataBits(zghz3384);
    zcap_bits = zghz3389.zbits;
  }
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3389;
  zgaz3389 = sailgen_doCSetBounds(zTestCap, zTestLen);
  unit zgsz3305;
  {
    __label__ case_390, finish_match_389;

    bool zexact;
    zexact = zgaz3389.ztup0;
    struct zCapability zghz3385;
    zghz3385 = zgaz3389.ztup1;
    struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3390;
    zgaz3390 = sailgen_setCapAddr(zTestCap, zTestAddr);
    {
      bool zrepresentable;
      zrepresentable = zgaz3390.ztup0;
      struct zCapability zghz3386;
      zghz3386 = zgaz3390.ztup1;
      struct zoptionzIz8izCiz9zK zbounds;
      CREATE(zoptionzIz8izCiz9zK)(&zbounds);
      sailgen_getCapBounds(&zbounds, zTestCap);
      enum zExecutionMode zmode;
      zmode = sailgen_getCapMode(zTestCap);
      sail_string zcap_str;
      CREATE(sail_string)(&zcap_str);
      sailgen_capToString(&zcap_str, zTestCap);
      struct zCapability zghz3387;
      {
        struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgaz3393;
        {
          struct zArchPerms zghz3388;
          zghz3388 = sailgen_getArchPermsLegalizzed(zTestCap);
          enum zExecutionMode zgaz3392;
          zgaz3392 = sailgen_getCapMode(zTestCap);
          zgaz3393.ztup0 = zghz3388;
          zgaz3393.ztup1 = zgaz3392;
        }
        zghz3387 = sailgen_setCapPermsAndMode(zTestCap, zgaz3393);
      }
      struct zoptionzIz8RArchPermszCEExecutionModez5z9zK zperms_mode;
      CREATE(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zperms_mode);
      sailgen_getCapPermsAndMode(&zperms_mode, zTestCap);
      uint64_t zlen;
      zlen = sailgen_getRepresentableLength(zTestAddr);
      uint64_t zmask;
      zmask = sailgen_getRepresentableAlignmentMask(zTestAddr);
      zgsz3305 = UNIT;
      KILL(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zperms_mode);
      KILL(sail_string)(&zcap_str);
      KILL(zoptionzIz8izCiz9zK)(&zbounds);
      goto finish_match_389;
    }
  case_390: ;
    sail_match_failure("main");
  finish_match_389: ;
    goto finish_match_387;
  }
case_388: ;
  sail_match_failure("main");
finish_match_387: ;
  zcbz381 = zgsz3305;



end_function_391: ;
  return zcbz381;
end_block_exception_392: ;

  return UNIT;
}



static void finish_sailgen_main(void)
{
  KILL(lbits)(&zghz3392);
  KILL(lbits)(&zghz3391);
  KILL(lbits)(&zghz3390);






}

static unit sailgen_initializze_registers(unit);

lbits zghz3393;
struct zCapability zghz3394;
sail_int zghz3395;

static void startup_sailgen_initializze_registers(void)
{
  CREATE(lbits)(&zghz3393);

  CREATE(sail_int)(&zghz3395);
}

static unit sailgen_initializze_registers(unit zgsz3310)
{
  __label__ end_function_394, end_block_exception_395;

  unit zcbz382;
  {
    struct zCapability zghz3394;
    zghz3394.zB = UINT64_C(0b00000000000000);
    RECREATE(sail_int)(&zghz3395);
    CONVERT_OF(sail_int, mach_int)(&zghz3395, INT64_C(-11));
    zghz3394.zE = CONVERT_OF(mach_int, sail_int)(zghz3395);
    zghz3394.zT = UINT64_C(0b00000000000000);
    zghz3394.zaddress = UINT64_C(0x0000000000000000);
    zghz3394.zap_m = UINT64_C(0b000000000);
    zghz3394.zinternal_exponent = false;
    zghz3394.zreserved_0 = UINT64_C(0x0000);
    zghz3394.zreserved_1 = UINT64_C(0b0000000);
    zghz3394.zsd_perms = UINT64_C(0x0);
    zghz3394.zsealed = false;
    zghz3394.ztag = false;
    zTestCap = zghz3394;
    unit zgsz3314;
    zgsz3314 = UNIT;
  }
  {
    zTestAddr = UINT64_C(0x0000000000000000);
    unit zgsz3313;
    zgsz3313 = UNIT;
  }
  RECREATE(lbits)(&zghz3393);
  CONVERT_OF(lbits, fbits)(&zghz3393, UINT64_C(0b0), UINT64_C(1) , true);
  append_64(&zghz3393, zghz3393, UINT64_C(0x0000000000000000));
  COPY(lbits)(&zTestLen, zghz3393);
  zcbz382 = UNIT;
end_function_394: ;
  return zcbz382;
end_block_exception_395: ;

  return UNIT;
}



static void finish_sailgen_initializze_registers(void)
{
  KILL(sail_int)(&zghz3395);

  KILL(lbits)(&zghz3393);
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
  CREATE(lbits)(&zTestLen);
  sailgen_initializze_registers(UNIT);
}

static void model_fini(void)
{
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
  KILL(lbits)(&zTestLen);
  finish_sailgen_sail_ones();
  finish_sailgen_to_bits();
  finish_sailgen_z8operatorz0zI_uz9();
  finish_sailgen_z8operatorz0zK_uz9();
  finish_sailgen_MAX();
  finish_sailgen_Mk_EncMetadata();
  finish_sailgen__update_EncMetadata_AP_M();
  finish_sailgen__update_EncMetadata_BE();
  finish_sailgen__update_EncMetadata_Bm();
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



#ifdef __cplusplus
}
#endif
