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

// union option<b>
enum kind_zoptionzIbzK { Kind_zNonezIbzK, Kind_zSomezIbzK };

struct zoptionzIbzK {
  enum kind_zoptionzIbzK kind;
  union {
    struct { unit zNonezIbzK; };
    struct { lbits zSomezIbzK; };
  };
};

static void CREATE(zoptionzIbzK)(struct zoptionzIbzK *op) {
  op->kind = Kind_zNonezIbzK;
}

static void RECREATE(zoptionzIbzK)(struct zoptionzIbzK *op) {

}

static void KILL(zoptionzIbzK)(struct zoptionzIbzK *op) {
  if (op->kind == Kind_zSomezIbzK) {
    KILL(lbits)(&op->zSomezIbzK);
  }
}

static void COPY(zoptionzIbzK)(struct zoptionzIbzK *rop, struct zoptionzIbzK op) {
  if (rop->kind == Kind_zSomezIbzK) {
    KILL(lbits)(&rop->zSomezIbzK);
  };
  rop->kind = op.kind;
  if (op.kind == Kind_zNonezIbzK) {
    rop->zNonezIbzK = op.zNonezIbzK;
  } else if (op.kind == Kind_zSomezIbzK) {
    CREATE(lbits)(&rop->zSomezIbzK); COPY(lbits)(&rop->zSomezIbzK, op.zSomezIbzK);
  }
}

static bool EQUAL(zoptionzIbzK)(struct zoptionzIbzK op1, struct zoptionzIbzK op2) {
  if (op1.kind == Kind_zNonezIbzK && op2.kind == Kind_zNonezIbzK) {
    return EQUAL(unit)(op1.zNonezIbzK, op2.zNonezIbzK);
  } else if (op1.kind == Kind_zSomezIbzK && op2.kind == Kind_zSomezIbzK) {
    return EQUAL(lbits)(op1.zSomezIbzK, op2.zSomezIbzK);
  } else return false;
}

static void sailgen_NonezIbzK(struct zoptionzIbzK *rop, unit op) {
  if (rop->kind == Kind_zSomezIbzK) {
    KILL(lbits)(&rop->zSomezIbzK);
  }
  rop->kind = Kind_zNonezIbzK;
  rop->zNonezIbzK = op;
}

static void sailgen_SomezIbzK(struct zoptionzIbzK *rop, lbits op) {
  if (rop->kind == Kind_zSomezIbzK) {
    KILL(lbits)(&rop->zSomezIbzK);
  }
  rop->kind = Kind_zSomezIbzK;
  CREATE(lbits)(&rop->zSomezIbzK);
  COPY(lbits)(&rop->zSomezIbzK, op);
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
  __label__ end_function_7, end_block_exception_8, end_function_713;

  RECREATE(lbits)(&zghz30);
  zeros(&zghz30, zn);
  not_bits((*(&zcbz32)), zghz30);
end_function_7: ;
  goto end_function_713;
end_block_exception_8: ;
  goto end_function_713;
end_function_713: ;
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
  __label__ end_function_19, end_block_exception_20, end_function_712;

  zero_extend((*(&zcbz36)), zv, zm);
end_function_19: ;
  goto end_function_712;
end_block_exception_20: ;
  goto end_function_712;
end_function_712: ;
}

static void sailgen_zzeros_implicit(lbits *rop, sail_int);

static void sailgen_zzeros_implicit(lbits *zcbz37, sail_int zn)
{
  __label__ end_function_22, end_block_exception_23, end_function_711;

  zeros((*(&zcbz37)), zn);
end_function_22: ;
  goto end_function_711;
end_block_exception_23: ;
  goto end_function_711;
end_function_711: ;
}

static void sailgen_ones(lbits *rop, sail_int);

static void sailgen_ones(lbits *zcbz38, sail_int zn)
{
  __label__ end_function_25, end_block_exception_26, end_function_710;

  sailgen_sail_ones((*(&zcbz38)), zn);
end_function_25: ;
  goto end_function_710;
end_block_exception_26: ;
  goto end_function_710;
end_function_710: ;
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

static void sailgen_to_bits(lbits *rop, sail_int, sail_int);

sail_int zghz31;

static void startup_sailgen_to_bits(void)
{    CREATE(sail_int)(&zghz31);
}

static void sailgen_to_bits(lbits *zcbz313, sail_int zl, sail_int zn)
{
  __label__ end_function_50, end_block_exception_51, end_function_709;

  {
    RECREATE(sail_int)(&zghz31);
    CONVERT_OF(sail_int, mach_int)(&zghz31, INT64_C(0));
    get_slice_int((*(&zcbz313)), zl, zn, zghz31);
  }
end_function_50: ;
  goto end_function_709;
end_block_exception_51: ;
  goto end_function_709;
end_function_709: ;
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



static int64_t zlog2_xlen_bytes;
static void create_letbind_0(void) {


  int64_t zgsz312;
  zgsz312 = INT64_C(2);
  zlog2_xlen_bytes = zgsz312;

let_end_58: ;
}
static void kill_letbind_0(void) {
}

static int64_t zflen_bytes;
static void create_letbind_1(void) {


  int64_t zgsz313;
  zgsz313 = INT64_C(4);
  zflen_bytes = zgsz313;

let_end_59: ;
}
static void kill_letbind_1(void) {
}

static int64_t zflen;
static void create_letbind_2(void) {


  int64_t zgsz314;
  zgsz314 = INT64_C(32);
  zflen = zgsz314;

let_end_60: ;
}
static void kill_letbind_2(void) {
}

static int64_t zxlen_bytes;
static void create_letbind_3(void) {


  int64_t zgsz315;
  zgsz315 = INT64_C(4);
  zxlen_bytes = zgsz315;

let_end_61: ;
}
static void kill_letbind_3(void) {
}

static int64_t zxlen;
static void create_letbind_4(void) {


  int64_t zgsz316;
  zgsz316 = INT64_C(32);
  zxlen = zgsz316;

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
  __label__ end_function_64, end_block_exception_65, end_function_708;

  RECREATE(sail_int)(&zghz36);
  pow2(&zghz36, zn);
  {
    RECREATE(sail_int)(&zghz37);
    CONVERT_OF(sail_int, mach_int)(&zghz37, INT64_C(1));
    sub_int((*(&zcbz316)), zghz36, zghz37);
  }
end_function_64: ;
  goto end_function_708;
end_block_exception_65: ;
  goto end_function_708;
end_function_708: ;
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
    if (!(zgsz325)) {

      goto case_80;
    }
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
    if (!(zgsz328)) {

      goto case_85;
    }
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
  zgsz333.zLM = false;
  zgsz333.zR = false;
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
  zgsz338 = INT64_C(3);
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
  struct zEncMetadata zcbz385 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz385;
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

static uint64_t sailgen__get_EncMetadata_CT(struct zEncMetadata);

static uint64_t sailgen__get_EncMetadata_CT(struct zEncMetadata zv)
{
  __label__ end_function_126, end_block_exception_127;

  uint64_t zcbz329;
  uint64_t zgaz319;
  zgaz319 = zv.zbits;
  zcbz329 = (safe_rshift(UINT64_MAX, 64 - 1) & (zgaz319 >> INT64_C(20)));

end_function_126: ;
  return zcbz329;
end_block_exception_127: ;

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
      CONVERT_OF(sail_int, mach_int)(&zghz329, INT64_C(20));
      RECREATE(sail_int)(&zghz330);
      CONVERT_OF(sail_int, mach_int)(&zghz330, INT64_C(20));
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
  struct zEncMetadata zcbz389 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz389;
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
  __label__ end_function_132, end_block_exception_133;

  uint64_t zcbz331;
  uint64_t zgaz322;
  zgaz322 = zv.zbits;
  zcbz331 = (safe_rshift(UINT64_MAX, 64 - 1) & (zgaz322 >> INT64_C(19)));

end_function_132: ;
  return zcbz331;
end_block_exception_133: ;

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
      CONVERT_OF(sail_int, mach_int)(&zghz335, INT64_C(19));
      RECREATE(sail_int)(&zghz336);
      CONVERT_OF(sail_int, mach_int)(&zghz336, INT64_C(19));
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
  struct zEncMetadata zcbz390 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz390;
}



static void finish_sailgen__update_EncMetadata_EF(void)
{
  KILL(lbits)(&zghz338);
  KILL(lbits)(&zghz337);
  KILL(sail_int)(&zghz336);
  KILL(sail_int)(&zghz335);
  KILL(lbits)(&zghz334);

}

static uint64_t sailgen__get_EncMetadata_L8(struct zEncMetadata);

static uint64_t sailgen__get_EncMetadata_L8(struct zEncMetadata zv)
{
  __label__ end_function_138, end_block_exception_139;

  uint64_t zcbz333;
  uint64_t zgaz325;
  zgaz325 = zv.zbits;
  zcbz333 = (safe_rshift(UINT64_MAX, 64 - 1) & (zgaz325 >> INT64_C(18)));

end_function_138: ;
  return zcbz333;
end_block_exception_139: ;

  return UINT64_C(0xdeadc0de);
}

static struct zEncMetadata sailgen__update_EncMetadata_L8(struct zEncMetadata, uint64_t);

struct zEncMetadata zghz339;
lbits zghz340;
sail_int zghz341;
sail_int zghz342;
lbits zghz343;
lbits zghz344;

static void startup_sailgen__update_EncMetadata_L8(void)
{

  CREATE(lbits)(&zghz340);
  CREATE(sail_int)(&zghz341);
  CREATE(sail_int)(&zghz342);
  CREATE(lbits)(&zghz343);
  CREATE(lbits)(&zghz344);
}

static struct zEncMetadata sailgen__update_EncMetadata_L8(struct zEncMetadata zv, uint64_t zx)
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
      CONVERT_OF(sail_int, mach_int)(&zghz341, INT64_C(18));
      RECREATE(sail_int)(&zghz342);
      CONVERT_OF(sail_int, mach_int)(&zghz342, INT64_C(18));
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
  struct zEncMetadata zcbz391 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz391;
}



static void finish_sailgen__update_EncMetadata_L8(void)
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
  __label__ end_function_144, end_block_exception_145;

  uint64_t zcbz335;
  uint64_t zgaz328;
  zgaz328 = zv.zbits;
  zcbz335 = (safe_rshift(UINT64_MAX, 64 - 2) & (zgaz328 >> INT64_C(30)));

end_function_144: ;
  return zcbz335;
end_block_exception_145: ;

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
      CONVERT_OF(sail_int, mach_int)(&zghz347, INT64_C(31));
      RECREATE(sail_int)(&zghz348);
      CONVERT_OF(sail_int, mach_int)(&zghz348, INT64_C(30));
      RECREATE(lbits)(&zghz349);
      CONVERT_OF(lbits, fbits)(&zghz349, zx, UINT64_C(2) , true);
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
  __label__ end_function_150, end_block_exception_151;

  uint64_t zcbz337;
  uint64_t zgaz331;
  zgaz331 = zv.zbits;
  zcbz337 = (safe_rshift(UINT64_MAX, 64 - 2) & (zgaz331 >> INT64_C(10)));

end_function_150: ;
  return zcbz337;
end_block_exception_151: ;

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
      CONVERT_OF(sail_int, mach_int)(&zghz353, INT64_C(11));
      RECREATE(sail_int)(&zghz354);
      CONVERT_OF(sail_int, mach_int)(&zghz354, INT64_C(10));
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
  __label__ end_function_156, end_block_exception_157;

  uint64_t zcbz339;
  uint64_t zgaz334;
  zgaz334 = zv.zbits;
  zcbz339 = (safe_rshift(UINT64_MAX, 64 - 6) & (zgaz334 >> INT64_C(12)));

end_function_156: ;
  return zcbz339;
end_block_exception_157: ;

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
      CONVERT_OF(sail_int, mach_int)(&zghz359, INT64_C(17));
      RECREATE(sail_int)(&zghz360);
      CONVERT_OF(sail_int, mach_int)(&zghz360, INT64_C(12));
      RECREATE(lbits)(&zghz361);
      CONVERT_OF(lbits, fbits)(&zghz361, zx, UINT64_C(6) , true);
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
  __label__ end_function_162, end_block_exception_163;

  uint64_t zcbz341;
  uint64_t zgaz337;
  zgaz337 = zv.zbits;
  zcbz341 = (safe_rshift(UINT64_MAX, 64 - 3) & (zgaz337 >> INT64_C(21)));

end_function_162: ;
  return zcbz341;
end_block_exception_163: ;

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
      CONVERT_OF(sail_int, mach_int)(&zghz365, INT64_C(23));
      RECREATE(sail_int)(&zghz366);
      CONVERT_OF(sail_int, mach_int)(&zghz366, INT64_C(21));
      RECREATE(lbits)(&zghz367);
      CONVERT_OF(lbits, fbits)(&zghz367, zx, UINT64_C(3) , true);
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
  __label__ end_function_168, end_block_exception_169;

  uint64_t zcbz343;
  uint64_t zgaz340;
  zgaz340 = zv.zbits;
  zcbz343 = (safe_rshift(UINT64_MAX, 64 - 1) & (zgaz340 >> INT64_C(24)));

end_function_168: ;
  return zcbz343;
end_block_exception_169: ;

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
      CONVERT_OF(sail_int, mach_int)(&zghz371, INT64_C(24));
      RECREATE(sail_int)(&zghz372);
      CONVERT_OF(sail_int, mach_int)(&zghz372, INT64_C(24));
      RECREATE(lbits)(&zghz373);
      CONVERT_OF(lbits, fbits)(&zghz373, zx, UINT64_C(1) , true);
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

static bool sailgen_ap_m_encdec_backwards_matches(struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9);

struct zArchPerms zghz375;
struct zArchPerms zghz376;
struct zArchPerms zghz377;
struct zArchPerms zghz378;
struct zArchPerms zghz379;
struct zArchPerms zghz380;
struct zArchPerms zghz381;
struct zArchPerms zghz382;
struct zArchPerms zghz383;
struct zArchPerms zghz384;
struct zArchPerms zghz385;

static void startup_sailgen_ap_m_encdec_forwards(void)
{











}

static struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 sailgen_ap_m_encdec_forwards(uint64_t zargz3)
{
  __label__ case_185, case_184, case_183, case_182, case_181, case_180, case_179, case_178, case_177, case_176, case_175, finish_match_174, end_function_186, end_block_exception_187;

  struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zcbz345;
  {
    uint64_t zv__0;
    zv__0 = zargz3;
    bool zgsz388;
    zgsz388 = (zv__0 == UINT64_C(0b00000));
    if (!(zgsz388)) {

      goto case_185;
    }
    enum zExecutionMode zgaz343;
    zgaz343 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    struct zArchPerms zghz385;
    zghz385.zASR = false;
    zghz385.zC = false;
    zghz385.zLM = false;
    zghz385.zR = false;
    zghz385.zW = false;
    zghz385.zX = false;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz387;
    zgsz387.ztup0 = zghz385;
    zgsz387.ztup1 = zgaz343;
    zcbz345 = zgsz387;
    goto finish_match_174;
  }
case_185: ;
  {
    uint64_t zv__3;
    zv__3 = zargz3;
    bool zgsz385;
    zgsz385 = (zv__3 == UINT64_C(0b00001));
    if (!(zgsz385)) {

      goto case_184;
    }
    enum zExecutionMode zgaz344;
    zgaz344 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    struct zArchPerms zghz384;
    zghz384.zASR = false;
    zghz384.zC = false;
    zghz384.zLM = false;
    zghz384.zR = true;
    zghz384.zW = false;
    zghz384.zX = false;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz384;
    zgsz384.ztup0 = zghz384;
    zgsz384.ztup1 = zgaz344;
    zcbz345 = zgsz384;
    goto finish_match_174;
  }
case_184: ;
  {
    uint64_t zv__6;
    zv__6 = zargz3;
    bool zgsz382;
    zgsz382 = (zv__6 == UINT64_C(0b00100));
    if (!(zgsz382)) {

      goto case_183;
    }
    enum zExecutionMode zgaz345;
    zgaz345 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    struct zArchPerms zghz383;
    zghz383.zASR = false;
    zghz383.zC = false;
    zghz383.zLM = false;
    zghz383.zR = false;
    zghz383.zW = true;
    zghz383.zX = false;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz381;
    zgsz381.ztup0 = zghz383;
    zgsz381.ztup1 = zgaz345;
    zcbz345 = zgsz381;
    goto finish_match_174;
  }
case_183: ;
  {
    uint64_t zv__9;
    zv__9 = zargz3;
    bool zgsz379;
    zgsz379 = (zv__9 == UINT64_C(0b00101));
    if (!(zgsz379)) {

      goto case_182;
    }
    enum zExecutionMode zgaz346;
    zgaz346 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    struct zArchPerms zghz382;
    zghz382.zASR = false;
    zghz382.zC = false;
    zghz382.zLM = false;
    zghz382.zR = true;
    zghz382.zW = true;
    zghz382.zX = false;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz378;
    zgsz378.ztup0 = zghz382;
    zgsz378.ztup1 = zgaz346;
    zcbz345 = zgsz378;
    goto finish_match_174;
  }
case_182: ;
  {
    uint64_t zv__12;
    zv__12 = zargz3;
    uint64_t zgaz348;
    zgaz348 = (safe_rshift(UINT64_MAX, 64 - 4) & (zv__12 >> INT64_C(1)));
    bool zgsz376;
    zgsz376 = (zgaz348 == UINT64_C(0x4));
    if (!(zgsz376)) {

      goto case_181;
    }
    uint64_t zm;
    zm = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__12 >> INT64_C(0)));
    enum zExecutionMode zgaz347;
    zgaz347 = sailgen_execution_mode_encdec_backwards(zm);
    struct zArchPerms zghz381;
    zghz381.zASR = true;
    zghz381.zC = true;
    zghz381.zLM = true;
    zghz381.zR = true;
    zghz381.zW = true;
    zghz381.zX = true;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz375;
    zgsz375.ztup0 = zghz381;
    zgsz375.ztup1 = zgaz347;
    zcbz345 = zgsz375;
    goto finish_match_174;
  }
case_181: ;
  {
    uint64_t zv__15;
    zv__15 = zargz3;
    uint64_t zgaz350;
    zgaz350 = (safe_rshift(UINT64_MAX, 64 - 4) & (zv__15 >> INT64_C(1)));
    bool zgsz373;
    zgsz373 = (zgaz350 == UINT64_C(0x5));
    if (!(zgsz373)) {

      goto case_180;
    }
    uint64_t zuz32;
    zuz32 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__15 >> INT64_C(0)));
    enum zExecutionMode zgaz349;
    zgaz349 = sailgen_execution_mode_encdec_backwards(zuz32);
    struct zArchPerms zghz380;
    zghz380.zASR = false;
    zghz380.zC = true;
    zghz380.zLM = true;
    zghz380.zR = true;
    zghz380.zW = false;
    zghz380.zX = true;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz372;
    zgsz372.ztup0 = zghz380;
    zgsz372.ztup1 = zgaz349;
    zcbz345 = zgsz372;
    goto finish_match_174;
  }
case_180: ;
  {
    uint64_t zv__18;
    zv__18 = zargz3;
    uint64_t zgaz352;
    zgaz352 = (safe_rshift(UINT64_MAX, 64 - 4) & (zv__18 >> INT64_C(1)));
    bool zgsz370;
    zgsz370 = (zgaz352 == UINT64_C(0x6));
    if (!(zgsz370)) {

      goto case_179;
    }
    uint64_t zuz33;
    zuz33 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__18 >> INT64_C(0)));
    enum zExecutionMode zgaz351;
    zgaz351 = sailgen_execution_mode_encdec_backwards(zuz33);
    struct zArchPerms zghz379;
    zghz379.zASR = false;
    zghz379.zC = true;
    zghz379.zLM = true;
    zghz379.zR = true;
    zghz379.zW = true;
    zghz379.zX = true;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz369;
    zgsz369.ztup0 = zghz379;
    zgsz369.ztup1 = zgaz351;
    zcbz345 = zgsz369;
    goto finish_match_174;
  }
case_179: ;
  {
    uint64_t zv__21;
    zv__21 = zargz3;
    uint64_t zgaz354;
    zgaz354 = (safe_rshift(UINT64_MAX, 64 - 4) & (zv__21 >> INT64_C(1)));
    bool zgsz367;
    zgsz367 = (zgaz354 == UINT64_C(0x7));
    if (!(zgsz367)) {

      goto case_178;
    }
    uint64_t zuz34;
    zuz34 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__21 >> INT64_C(0)));
    enum zExecutionMode zgaz353;
    zgaz353 = sailgen_execution_mode_encdec_backwards(zuz34);
    struct zArchPerms zghz378;
    zghz378.zASR = false;
    zghz378.zC = false;
    zghz378.zLM = false;
    zghz378.zR = true;
    zghz378.zW = true;
    zghz378.zX = true;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz366;
    zgsz366.ztup0 = zghz378;
    zgsz366.ztup1 = zgaz353;
    zcbz345 = zgsz366;
    goto finish_match_174;
  }
case_178: ;
  {
    uint64_t zv__24;
    zv__24 = zargz3;
    bool zgsz364;
    zgsz364 = (zv__24 == UINT64_C(0b10011));
    if (!(zgsz364)) {

      goto case_177;
    }
    enum zExecutionMode zgaz355;
    zgaz355 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    struct zArchPerms zghz377;
    zghz377.zASR = false;
    zghz377.zC = true;
    zghz377.zLM = false;
    zghz377.zR = true;
    zghz377.zW = false;
    zghz377.zX = false;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz363;
    zgsz363.ztup0 = zghz377;
    zgsz363.ztup1 = zgaz355;
    zcbz345 = zgsz363;
    goto finish_match_174;
  }
case_177: ;
  {
    uint64_t zv__27;
    zv__27 = zargz3;
    bool zgsz361;
    zgsz361 = (zv__27 == UINT64_C(0b11011));
    if (!(zgsz361)) {

      goto case_176;
    }
    enum zExecutionMode zgaz356;
    zgaz356 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    struct zArchPerms zghz376;
    zghz376.zASR = false;
    zghz376.zC = true;
    zghz376.zLM = true;
    zghz376.zR = true;
    zghz376.zW = false;
    zghz376.zX = false;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz360;
    zgsz360.ztup0 = zghz376;
    zgsz360.ztup1 = zgaz356;
    zcbz345 = zgsz360;
    goto finish_match_174;
  }
case_176: ;
  {
    uint64_t zv__30;
    zv__30 = zargz3;
    bool zgsz358;
    zgsz358 = (zv__30 == UINT64_C(0b11111));
    if (!(zgsz358)) {

      goto case_175;
    }
    enum zExecutionMode zgaz357;
    zgaz357 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    struct zArchPerms zghz375;
    zghz375.zASR = false;
    zghz375.zC = true;
    zghz375.zLM = true;
    zghz375.zR = true;
    zghz375.zW = true;
    zghz375.zX = false;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz357;
    zgsz357.ztup0 = zghz375;
    zgsz357.ztup1 = zgaz357;
    zcbz345 = zgsz357;
    goto finish_match_174;
  }
case_175: ;
  sail_match_failure("ap_m_encdec_forwards");
finish_match_174: ;
end_function_186: ;
  return zcbz345;
end_block_exception_187: ;
  struct zArchPerms zcbz398 = { .zASR = false, .zC = false, .zLM = false, .zR = false, .zW = false, .zX = false };
  struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zcbz397 = { .ztup0 = zcbz398, .ztup1 = zCapPtrMode };
  return zcbz397;
}



static void finish_sailgen_ap_m_encdec_forwards(void)
{











}

static struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 sailgen_ap_m_encdec_forwards_infallible(uint64_t);

struct zArchPerms zghz386;
struct zArchPerms zghz387;
struct zArchPerms zghz388;
struct zArchPerms zghz389;
struct zArchPerms zghz390;
struct zArchPerms zghz391;
struct zArchPerms zghz392;
struct zArchPerms zghz393;
struct zArchPerms zghz394;
struct zArchPerms zghz395;
struct zArchPerms zghz396;

static void startup_sailgen_ap_m_encdec_forwards_infallible(void)
{











}

static struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 sailgen_ap_m_encdec_forwards_infallible(uint64_t zargz3)
{
  __label__ case_199, case_198, case_197, case_196, case_195, case_194, case_193, case_192, case_191, case_190, case_189, finish_match_188, end_function_200, end_block_exception_201;

  struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zcbz346;
  {
    uint64_t zv__0;
    zv__0 = zargz3;
    bool zgsz3122;
    zgsz3122 = (zv__0 == UINT64_C(0b00000));
    if (!(zgsz3122)) {

      goto case_199;
    }
    enum zExecutionMode zgaz343;
    zgaz343 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    struct zArchPerms zghz396;
    zghz396.zASR = false;
    zghz396.zC = false;
    zghz396.zLM = false;
    zghz396.zR = false;
    zghz396.zW = false;
    zghz396.zX = false;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz3121;
    zgsz3121.ztup0 = zghz396;
    zgsz3121.ztup1 = zgaz343;
    zcbz346 = zgsz3121;
    goto finish_match_188;
  }
case_199: ;
  {
    uint64_t zv__3;
    zv__3 = zargz3;
    bool zgsz3119;
    zgsz3119 = (zv__3 == UINT64_C(0b00001));
    if (!(zgsz3119)) {

      goto case_198;
    }
    enum zExecutionMode zgaz344;
    zgaz344 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    struct zArchPerms zghz395;
    zghz395.zASR = false;
    zghz395.zC = false;
    zghz395.zLM = false;
    zghz395.zR = true;
    zghz395.zW = false;
    zghz395.zX = false;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz3118;
    zgsz3118.ztup0 = zghz395;
    zgsz3118.ztup1 = zgaz344;
    zcbz346 = zgsz3118;
    goto finish_match_188;
  }
case_198: ;
  {
    uint64_t zv__6;
    zv__6 = zargz3;
    bool zgsz3116;
    zgsz3116 = (zv__6 == UINT64_C(0b00100));
    if (!(zgsz3116)) {

      goto case_197;
    }
    enum zExecutionMode zgaz345;
    zgaz345 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    struct zArchPerms zghz394;
    zghz394.zASR = false;
    zghz394.zC = false;
    zghz394.zLM = false;
    zghz394.zR = false;
    zghz394.zW = true;
    zghz394.zX = false;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz3115;
    zgsz3115.ztup0 = zghz394;
    zgsz3115.ztup1 = zgaz345;
    zcbz346 = zgsz3115;
    goto finish_match_188;
  }
case_197: ;
  {
    uint64_t zv__9;
    zv__9 = zargz3;
    bool zgsz3113;
    zgsz3113 = (zv__9 == UINT64_C(0b00101));
    if (!(zgsz3113)) {

      goto case_196;
    }
    enum zExecutionMode zgaz346;
    zgaz346 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    struct zArchPerms zghz393;
    zghz393.zASR = false;
    zghz393.zC = false;
    zghz393.zLM = false;
    zghz393.zR = true;
    zghz393.zW = true;
    zghz393.zX = false;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz3112;
    zgsz3112.ztup0 = zghz393;
    zgsz3112.ztup1 = zgaz346;
    zcbz346 = zgsz3112;
    goto finish_match_188;
  }
case_196: ;
  {
    uint64_t zv__12;
    zv__12 = zargz3;
    uint64_t zgaz348;
    zgaz348 = (safe_rshift(UINT64_MAX, 64 - 4) & (zv__12 >> INT64_C(1)));
    bool zgsz3110;
    zgsz3110 = (zgaz348 == UINT64_C(0x4));
    if (!(zgsz3110)) {

      goto case_195;
    }
    uint64_t zm;
    zm = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__12 >> INT64_C(0)));
    enum zExecutionMode zgaz347;
    zgaz347 = sailgen_execution_mode_encdec_backwards(zm);
    struct zArchPerms zghz392;
    zghz392.zASR = true;
    zghz392.zC = true;
    zghz392.zLM = true;
    zghz392.zR = true;
    zghz392.zW = true;
    zghz392.zX = true;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz3109;
    zgsz3109.ztup0 = zghz392;
    zgsz3109.ztup1 = zgaz347;
    zcbz346 = zgsz3109;
    goto finish_match_188;
  }
case_195: ;
  {
    uint64_t zv__15;
    zv__15 = zargz3;
    uint64_t zgaz350;
    zgaz350 = (safe_rshift(UINT64_MAX, 64 - 4) & (zv__15 >> INT64_C(1)));
    bool zgsz3107;
    zgsz3107 = (zgaz350 == UINT64_C(0x5));
    if (!(zgsz3107)) {

      goto case_194;
    }
    uint64_t zuz35;
    zuz35 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__15 >> INT64_C(0)));
    enum zExecutionMode zgaz349;
    zgaz349 = sailgen_execution_mode_encdec_backwards(zuz35);
    struct zArchPerms zghz391;
    zghz391.zASR = false;
    zghz391.zC = true;
    zghz391.zLM = true;
    zghz391.zR = true;
    zghz391.zW = false;
    zghz391.zX = true;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz3106;
    zgsz3106.ztup0 = zghz391;
    zgsz3106.ztup1 = zgaz349;
    zcbz346 = zgsz3106;
    goto finish_match_188;
  }
case_194: ;
  {
    uint64_t zv__18;
    zv__18 = zargz3;
    uint64_t zgaz352;
    zgaz352 = (safe_rshift(UINT64_MAX, 64 - 4) & (zv__18 >> INT64_C(1)));
    bool zgsz3104;
    zgsz3104 = (zgaz352 == UINT64_C(0x6));
    if (!(zgsz3104)) {

      goto case_193;
    }
    uint64_t zuz36;
    zuz36 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__18 >> INT64_C(0)));
    enum zExecutionMode zgaz351;
    zgaz351 = sailgen_execution_mode_encdec_backwards(zuz36);
    struct zArchPerms zghz390;
    zghz390.zASR = false;
    zghz390.zC = true;
    zghz390.zLM = true;
    zghz390.zR = true;
    zghz390.zW = true;
    zghz390.zX = true;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz3103;
    zgsz3103.ztup0 = zghz390;
    zgsz3103.ztup1 = zgaz351;
    zcbz346 = zgsz3103;
    goto finish_match_188;
  }
case_193: ;
  {
    uint64_t zv__21;
    zv__21 = zargz3;
    uint64_t zgaz354;
    zgaz354 = (safe_rshift(UINT64_MAX, 64 - 4) & (zv__21 >> INT64_C(1)));
    bool zgsz3101;
    zgsz3101 = (zgaz354 == UINT64_C(0x7));
    if (!(zgsz3101)) {

      goto case_192;
    }
    uint64_t zuz37;
    zuz37 = (safe_rshift(UINT64_MAX, 64 - 1) & (zv__21 >> INT64_C(0)));
    enum zExecutionMode zgaz353;
    zgaz353 = sailgen_execution_mode_encdec_backwards(zuz37);
    struct zArchPerms zghz389;
    zghz389.zASR = false;
    zghz389.zC = false;
    zghz389.zLM = false;
    zghz389.zR = true;
    zghz389.zW = true;
    zghz389.zX = true;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz3100;
    zgsz3100.ztup0 = zghz389;
    zgsz3100.ztup1 = zgaz353;
    zcbz346 = zgsz3100;
    goto finish_match_188;
  }
case_192: ;
  {
    uint64_t zv__24;
    zv__24 = zargz3;
    bool zgsz398;
    zgsz398 = (zv__24 == UINT64_C(0b10011));
    if (!(zgsz398)) {

      goto case_191;
    }
    enum zExecutionMode zgaz355;
    zgaz355 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    struct zArchPerms zghz388;
    zghz388.zASR = false;
    zghz388.zC = true;
    zghz388.zLM = false;
    zghz388.zR = true;
    zghz388.zW = false;
    zghz388.zX = false;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz397;
    zgsz397.ztup0 = zghz388;
    zgsz397.ztup1 = zgaz355;
    zcbz346 = zgsz397;
    goto finish_match_188;
  }
case_191: ;
  {
    uint64_t zv__27;
    zv__27 = zargz3;
    bool zgsz395;
    zgsz395 = (zv__27 == UINT64_C(0b11011));
    if (!(zgsz395)) {

      goto case_190;
    }
    enum zExecutionMode zgaz356;
    zgaz356 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    struct zArchPerms zghz387;
    zghz387.zASR = false;
    zghz387.zC = true;
    zghz387.zLM = true;
    zghz387.zR = true;
    zghz387.zW = false;
    zghz387.zX = false;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz394;
    zgsz394.ztup0 = zghz387;
    zgsz394.ztup1 = zgaz356;
    zcbz346 = zgsz394;
    goto finish_match_188;
  }
case_190: ;
  {
    uint64_t zv__30;
    zv__30 = zargz3;
    bool zgsz392;
    zgsz392 = (zv__30 == UINT64_C(0b11111));
    if (!(zgsz392)) {

      goto case_189;
    }
    enum zExecutionMode zgaz357;
    zgaz357 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    struct zArchPerms zghz386;
    zghz386.zASR = false;
    zghz386.zC = true;
    zghz386.zLM = true;
    zghz386.zR = true;
    zghz386.zW = true;
    zghz386.zX = false;
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz391;
    zgsz391.ztup0 = zghz386;
    zgsz391.ztup1 = zgaz357;
    zcbz346 = zgsz391;
    goto finish_match_188;
  }
case_189: ;
finish_match_188: ;
end_function_200: ;
  return zcbz346;
end_block_exception_201: ;
  struct zArchPerms zcbz3100 = { .zASR = false, .zC = false, .zLM = false, .zR = false, .zW = false, .zX = false };
  struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zcbz399 = { .ztup0 = zcbz3100, .ztup1 = zCapPtrMode };
  return zcbz399;
}



static void finish_sailgen_ap_m_encdec_forwards_infallible(void)
{











}

lbits zghz397;
lbits zghz398;
lbits zghz399;
lbits zghz3100;
lbits zghz3101;
lbits zghz3102;
lbits zghz3103;
lbits zghz3104;
lbits zghz3105;
lbits zghz3106;
lbits zghz3107;

static void startup_sailgen_ap_m_encdec_backwards(void)
{
  CREATE(lbits)(&zghz397);
  CREATE(lbits)(&zghz398);
  CREATE(lbits)(&zghz399);
  CREATE(lbits)(&zghz3100);
  CREATE(lbits)(&zghz3101);
  CREATE(lbits)(&zghz3102);
  CREATE(lbits)(&zghz3103);
  CREATE(lbits)(&zghz3104);
  CREATE(lbits)(&zghz3105);
  CREATE(lbits)(&zghz3106);
  CREATE(lbits)(&zghz3107);
}

static uint64_t sailgen_ap_m_encdec_backwards(struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zargz3)
{
  __label__ case_300, case_210, finish_match_209, end_function_301, end_block_exception_302;

  uint64_t zcbz347;
  struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zhead_expz3;
  zhead_expz3 = zargz3;
  struct zoptionzIbzK zgaz365;
  CREATE(zoptionzIbzK)(&zgaz365);
  {
    __label__ case_205, case_204, finish_match_203;

    {
      __label__ case_208, case_207, finish_match_206;

      bool zp0z3;
      zp0z3 = zhead_expz3.ztup0.zR;
      bool zp1z3;
      zp1z3 = zhead_expz3.ztup0.zW;
      bool zp2z3;
      zp2z3 = zhead_expz3.ztup0.zC;
      bool zp3z3;
      zp3z3 = zhead_expz3.ztup0.zLM;
      bool zp4z3;
      zp4z3 = zhead_expz3.ztup0.zX;
      bool zp5z3;
      zp5z3 = zhead_expz3.ztup0.zASR;
      enum zExecutionMode zmapping0z3;
      zmapping0z3 = zhead_expz3.ztup1;
      bool zgaz364;
      {
        bool zgaz363;
        {
          bool zgaz362;
          {
            bool zgaz361;
            {
              bool zgaz360;
              {
                bool zgaz359;
                zgaz359 = sailgen_execution_mode_encdec_forwards_matches(zmapping0z3);
                bool zgsz3125;
                if (zgaz359) {  zgsz3125 = eq_bool(zp5z3, false);  } else {  zgsz3125 = false;  }
                zgaz360 = zgsz3125;
              }
              bool zgsz3126;
              if (zgaz360) {  zgsz3126 = eq_bool(zp4z3, false);  } else {  zgsz3126 = false;  }
              zgaz361 = zgsz3126;
            }
            bool zgsz3127;
            if (zgaz361) {  zgsz3127 = eq_bool(zp3z3, false);  } else {  zgsz3127 = false;  }
            zgaz362 = zgsz3127;
          }
          bool zgsz3128;
          if (zgaz362) {  zgsz3128 = eq_bool(zp2z3, false);  } else {  zgsz3128 = false;  }
          zgaz363 = zgsz3128;
        }
        bool zgsz3129;
        if (zgaz363) {  zgsz3129 = eq_bool(zp1z3, false);  } else {  zgsz3129 = false;  }
        zgaz364 = zgsz3129;
      }
      bool zgsz3130;
      if (zgaz364) {  zgsz3130 = eq_bool(zp0z3, false);  } else {  zgsz3130 = false;  }
      bool zgsz3134;
      zgsz3134 = zgsz3130;
      if (!(zgsz3134)) {






        goto case_205;
      }
      uint64_t zgaz358;
      zgaz358 = sailgen_execution_mode_encdec_forwards_infallible(zmapping0z3);
      struct zoptionzIbzK zgsz3131;
      CREATE(zoptionzIbzK)(&zgsz3131);
      {
        uint64_t zb__6;
        zb__6 = zgaz358;
        bool zgsz3133;
        zgsz3133 = (zb__6 == UINT64_C(0b0));
        if (!(zgsz3133)) {

          goto case_208;
        }
        {
          RECREATE(lbits)(&zghz3107);
          CONVERT_OF(lbits, fbits)(&zghz3107, UINT64_C(0b00000), UINT64_C(5) , true);
          sailgen_SomezIbzK(&zgsz3131, zghz3107);
        }
        goto finish_match_206;
      }
    case_208: ;
      {
        sailgen_NonezIbzK(&zgsz3131, UNIT);
        goto finish_match_206;
      }
    case_207: ;
    finish_match_206: ;
      COPY(zoptionzIbzK)(&zgaz365, zgsz3131);
      KILL(zoptionzIbzK)(&zgsz3131);
      goto finish_match_203;
    }
  case_205: ;
    {
      sailgen_NonezIbzK(&zgaz365, UNIT);
      goto finish_match_203;
    }
  case_204: ;
  finish_match_203: ;
  }
  {
    if (zgaz365.kind != Kind_zSomezIbzK) goto case_300;
    uint64_t zresult;
    zresult = CONVERT_OF(fbits, lbits)(zgaz365.zSomezIbzK, true);
    zcbz347 = zresult;
    goto finish_match_209;
  }
case_300: ;
  {
    __label__ case_299, case_218, finish_match_217;

    /* complete */
    struct zoptionzIbzK zgaz373;
    CREATE(zoptionzIbzK)(&zgaz373);
    {
      __label__ case_213, case_212, finish_match_211;

      {
        __label__ case_216, case_215, finish_match_214;

        bool zuz38;
        zuz38 = zhead_expz3.ztup0.zR;
        bool zuz39;
        zuz39 = zhead_expz3.ztup0.zW;
        bool zuz310;
        zuz310 = zhead_expz3.ztup0.zC;
        bool zuz311;
        zuz311 = zhead_expz3.ztup0.zLM;
        bool zuz312;
        zuz312 = zhead_expz3.ztup0.zX;
        bool zuz313;
        zuz313 = zhead_expz3.ztup0.zASR;
        enum zExecutionMode zmapping1z3;
        zmapping1z3 = zhead_expz3.ztup1;
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
                  zgaz367 = sailgen_execution_mode_encdec_forwards_matches(zmapping1z3);
                  bool zgsz3138;
                  if (zgaz367) {  zgsz3138 = eq_bool(zuz313, false);  } else {  zgsz3138 = false;  }
                  zgaz368 = zgsz3138;
                }
                bool zgsz3139;
                if (zgaz368) {  zgsz3139 = eq_bool(zuz312, false);  } else {  zgsz3139 = false;  }
                zgaz369 = zgsz3139;
              }
              bool zgsz3140;
              if (zgaz369) {  zgsz3140 = eq_bool(zuz311, false);  } else {  zgsz3140 = false;  }
              zgaz370 = zgsz3140;
            }
            bool zgsz3141;
            if (zgaz370) {  zgsz3141 = eq_bool(zuz310, false);  } else {  zgsz3141 = false;  }
            zgaz371 = zgsz3141;
          }
          bool zgsz3142;
          if (zgaz371) {  zgsz3142 = eq_bool(zuz39, false);  } else {  zgsz3142 = false;  }
          zgaz372 = zgsz3142;
        }
        bool zgsz3143;
        if (zgaz372) {  zgsz3143 = eq_bool(zuz38, true);  } else {  zgsz3143 = false;  }
        bool zgsz3147;
        zgsz3147 = zgsz3143;
        if (!(zgsz3147)) {






          goto case_213;
        }
        uint64_t zgaz366;
        zgaz366 = sailgen_execution_mode_encdec_forwards_infallible(zmapping1z3);
        struct zoptionzIbzK zgsz3144;
        CREATE(zoptionzIbzK)(&zgsz3144);
        {
          uint64_t zb__5;
          zb__5 = zgaz366;
          bool zgsz3146;
          zgsz3146 = (zb__5 == UINT64_C(0b0));
          if (!(zgsz3146)) {

            goto case_216;
          }
          {
            RECREATE(lbits)(&zghz3106);
            CONVERT_OF(lbits, fbits)(&zghz3106, UINT64_C(0b00001), UINT64_C(5) , true);
            sailgen_SomezIbzK(&zgsz3144, zghz3106);
          }
          goto finish_match_214;
        }
      case_216: ;
        {
          sailgen_NonezIbzK(&zgsz3144, UNIT);
          goto finish_match_214;
        }
      case_215: ;
      finish_match_214: ;
        COPY(zoptionzIbzK)(&zgaz373, zgsz3144);
        KILL(zoptionzIbzK)(&zgsz3144);
        goto finish_match_211;
      }
    case_213: ;
      {
        sailgen_NonezIbzK(&zgaz373, UNIT);
        goto finish_match_211;
      }
    case_212: ;
    finish_match_211: ;
    }
    uint64_t zgsz3148;
    {
      if (zgaz373.kind != Kind_zSomezIbzK) goto case_299;
      zgsz3148 = CONVERT_OF(fbits, lbits)(zgaz373.zSomezIbzK, true);
      goto finish_match_217;
    }
  case_299: ;
    {
      __label__ case_298, case_226, finish_match_225;

      /* complete */
      struct zoptionzIbzK zgaz381;
      CREATE(zoptionzIbzK)(&zgaz381);
      {
        __label__ case_221, case_220, finish_match_219;

        {
          __label__ case_224, case_223, finish_match_222;

          bool zuz315;
          zuz315 = zhead_expz3.ztup0.zR;
          bool zuz316;
          zuz316 = zhead_expz3.ztup0.zW;
          bool zuz317;
          zuz317 = zhead_expz3.ztup0.zC;
          bool zuz318;
          zuz318 = zhead_expz3.ztup0.zLM;
          bool zuz319;
          zuz319 = zhead_expz3.ztup0.zX;
          bool zuz320;
          zuz320 = zhead_expz3.ztup0.zASR;
          enum zExecutionMode zmapping2z3;
          zmapping2z3 = zhead_expz3.ztup1;
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
                    zgaz375 = sailgen_execution_mode_encdec_forwards_matches(zmapping2z3);
                    bool zgsz3151;
                    if (zgaz375) {  zgsz3151 = eq_bool(zuz320, false);  } else {  zgsz3151 = false;  }
                    zgaz376 = zgsz3151;
                  }
                  bool zgsz3152;
                  if (zgaz376) {  zgsz3152 = eq_bool(zuz319, false);  } else {  zgsz3152 = false;  }
                  zgaz377 = zgsz3152;
                }
                bool zgsz3153;
                if (zgaz377) {  zgsz3153 = eq_bool(zuz318, false);  } else {  zgsz3153 = false;  }
                zgaz378 = zgsz3153;
              }
              bool zgsz3154;
              if (zgaz378) {  zgsz3154 = eq_bool(zuz317, false);  } else {  zgsz3154 = false;  }
              zgaz379 = zgsz3154;
            }
            bool zgsz3155;
            if (zgaz379) {  zgsz3155 = eq_bool(zuz316, true);  } else {  zgsz3155 = false;  }
            zgaz380 = zgsz3155;
          }
          bool zgsz3156;
          if (zgaz380) {  zgsz3156 = eq_bool(zuz315, false);  } else {  zgsz3156 = false;  }
          bool zgsz3160;
          zgsz3160 = zgsz3156;
          if (!(zgsz3160)) {






            goto case_221;
          }
          uint64_t zgaz374;
          zgaz374 = sailgen_execution_mode_encdec_forwards_infallible(zmapping2z3);
          struct zoptionzIbzK zgsz3157;
          CREATE(zoptionzIbzK)(&zgsz3157);
          {
            uint64_t zb__4;
            zb__4 = zgaz374;
            bool zgsz3159;
            zgsz3159 = (zb__4 == UINT64_C(0b0));
            if (!(zgsz3159)) {

              goto case_224;
            }
            {
              RECREATE(lbits)(&zghz3105);
              CONVERT_OF(lbits, fbits)(&zghz3105, UINT64_C(0b00100), UINT64_C(5) , true);
              sailgen_SomezIbzK(&zgsz3157, zghz3105);
            }
            goto finish_match_222;
          }
        case_224: ;
          {
            sailgen_NonezIbzK(&zgsz3157, UNIT);
            goto finish_match_222;
          }
        case_223: ;
        finish_match_222: ;
          COPY(zoptionzIbzK)(&zgaz381, zgsz3157);
          KILL(zoptionzIbzK)(&zgsz3157);
          goto finish_match_219;
        }
      case_221: ;
        {
          sailgen_NonezIbzK(&zgaz381, UNIT);
          goto finish_match_219;
        }
      case_220: ;
      finish_match_219: ;
      }
      uint64_t zgsz3161;
      {
        if (zgaz381.kind != Kind_zSomezIbzK) goto case_298;
        zgsz3161 = CONVERT_OF(fbits, lbits)(zgaz381.zSomezIbzK, true);
        goto finish_match_225;
      }
    case_298: ;
      {
        __label__ case_297, case_234, finish_match_233;

        /* complete */
        struct zoptionzIbzK zgaz389;
        CREATE(zoptionzIbzK)(&zgaz389);
        {
          __label__ case_229, case_228, finish_match_227;

          {
            __label__ case_232, case_231, finish_match_230;

            bool zuz322;
            zuz322 = zhead_expz3.ztup0.zR;
            bool zuz323;
            zuz323 = zhead_expz3.ztup0.zW;
            bool zuz324;
            zuz324 = zhead_expz3.ztup0.zC;
            bool zuz325;
            zuz325 = zhead_expz3.ztup0.zLM;
            bool zuz326;
            zuz326 = zhead_expz3.ztup0.zX;
            bool zuz327;
            zuz327 = zhead_expz3.ztup0.zASR;
            enum zExecutionMode zmapping3z3;
            zmapping3z3 = zhead_expz3.ztup1;
            bool zgaz388;
            {
              bool zgaz387;
              {
                bool zgaz386;
                {
                  bool zgaz385;
                  {
                    bool zgaz384;
                    {
                      bool zgaz383;
                      zgaz383 = sailgen_execution_mode_encdec_forwards_matches(zmapping3z3);
                      bool zgsz3164;
                      if (zgaz383) {  zgsz3164 = eq_bool(zuz327, false);  } else {  zgsz3164 = false;  }
                      zgaz384 = zgsz3164;
                    }
                    bool zgsz3165;
                    if (zgaz384) {  zgsz3165 = eq_bool(zuz326, false);  } else {  zgsz3165 = false;  }
                    zgaz385 = zgsz3165;
                  }
                  bool zgsz3166;
                  if (zgaz385) {  zgsz3166 = eq_bool(zuz325, false);  } else {  zgsz3166 = false;  }
                  zgaz386 = zgsz3166;
                }
                bool zgsz3167;
                if (zgaz386) {  zgsz3167 = eq_bool(zuz324, false);  } else {  zgsz3167 = false;  }
                zgaz387 = zgsz3167;
              }
              bool zgsz3168;
              if (zgaz387) {  zgsz3168 = eq_bool(zuz323, true);  } else {  zgsz3168 = false;  }
              zgaz388 = zgsz3168;
            }
            bool zgsz3169;
            if (zgaz388) {  zgsz3169 = eq_bool(zuz322, true);  } else {  zgsz3169 = false;  }
            bool zgsz3173;
            zgsz3173 = zgsz3169;
            if (!(zgsz3173)) {






              goto case_229;
            }
            uint64_t zgaz382;
            zgaz382 = sailgen_execution_mode_encdec_forwards_infallible(zmapping3z3);
            struct zoptionzIbzK zgsz3170;
            CREATE(zoptionzIbzK)(&zgsz3170);
            {
              uint64_t zb__3;
              zb__3 = zgaz382;
              bool zgsz3172;
              zgsz3172 = (zb__3 == UINT64_C(0b0));
              if (!(zgsz3172)) {

                goto case_232;
              }
              {
                RECREATE(lbits)(&zghz3104);
                CONVERT_OF(lbits, fbits)(&zghz3104, UINT64_C(0b00101), UINT64_C(5) , true);
                sailgen_SomezIbzK(&zgsz3170, zghz3104);
              }
              goto finish_match_230;
            }
          case_232: ;
            {
              sailgen_NonezIbzK(&zgsz3170, UNIT);
              goto finish_match_230;
            }
          case_231: ;
          finish_match_230: ;
            COPY(zoptionzIbzK)(&zgaz389, zgsz3170);
            KILL(zoptionzIbzK)(&zgsz3170);
            goto finish_match_227;
          }
        case_229: ;
          {
            sailgen_NonezIbzK(&zgaz389, UNIT);
            goto finish_match_227;
          }
        case_228: ;
        finish_match_227: ;
        }
        uint64_t zgsz3174;
        {
          if (zgaz389.kind != Kind_zSomezIbzK) goto case_297;
          zgsz3174 = CONVERT_OF(fbits, lbits)(zgaz389.zSomezIbzK, true);
          goto finish_match_233;
        }
      case_297: ;
        {
          __label__ case_296, case_242, finish_match_241;

          /* complete */
          struct zoptionzIbzK zgaz3100;
          CREATE(zoptionzIbzK)(&zgaz3100);
          {
            __label__ case_237, case_236, finish_match_235;

            {
              __label__ case_240, case_239, finish_match_238;

              bool zuz329;
              zuz329 = zhead_expz3.ztup0.zR;
              bool zuz330;
              zuz330 = zhead_expz3.ztup0.zW;
              bool zuz331;
              zuz331 = zhead_expz3.ztup0.zC;
              bool zuz332;
              zuz332 = zhead_expz3.ztup0.zLM;
              bool zuz333;
              zuz333 = zhead_expz3.ztup0.zX;
              bool zuz334;
              zuz334 = zhead_expz3.ztup0.zASR;
              enum zExecutionMode zmapping4z3;
              zmapping4z3 = zhead_expz3.ztup1;
              bool zgaz399;
              {
                bool zgaz398;
                {
                  bool zgaz397;
                  {
                    bool zgaz396;
                    {
                      bool zgaz395;
                      {
                        bool zgaz394;
                        zgaz394 = sailgen_execution_mode_encdec_forwards_matches(zmapping4z3);
                        bool zgsz3177;
                        if (zgaz394) {  zgsz3177 = eq_bool(zuz334, true);  } else {  zgsz3177 = false;  }
                        zgaz395 = zgsz3177;
                      }
                      bool zgsz3178;
                      if (zgaz395) {  zgsz3178 = eq_bool(zuz333, true);  } else {  zgsz3178 = false;  }
                      zgaz396 = zgsz3178;
                    }
                    bool zgsz3179;
                    if (zgaz396) {  zgsz3179 = eq_bool(zuz332, true);  } else {  zgsz3179 = false;  }
                    zgaz397 = zgsz3179;
                  }
                  bool zgsz3180;
                  if (zgaz397) {  zgsz3180 = eq_bool(zuz331, true);  } else {  zgsz3180 = false;  }
                  zgaz398 = zgsz3180;
                }
                bool zgsz3181;
                if (zgaz398) {  zgsz3181 = eq_bool(zuz330, true);  } else {  zgsz3181 = false;  }
                zgaz399 = zgsz3181;
              }
              bool zgsz3182;
              if (zgaz399) {  zgsz3182 = eq_bool(zuz329, true);  } else {  zgsz3182 = false;  }
              bool zgsz3186;
              zgsz3186 = zgsz3182;
              if (!(zgsz3186)) {






                goto case_237;
              }
              uint64_t zgaz390;
              zgaz390 = sailgen_execution_mode_encdec_forwards_infallible(zmapping4z3);
              struct zoptionzIbzK zgsz3183;
              CREATE(zoptionzIbzK)(&zgsz3183);
              {
                uint64_t zm;
                zm = zgaz390;
                uint64_t zgaz393;
                {
                  uint64_t zgaz392;
                  {
                    uint64_t zgaz391;
                    zgaz391 = zm;
                    zgaz392 = (UINT64_C(0b00) << 1) | zgaz391;
                  }
                  zgaz393 = (UINT64_C(0b01) << 3) | zgaz392;
                }
                {
                  RECREATE(lbits)(&zghz3103);
                  CONVERT_OF(lbits, fbits)(&zghz3103, zgaz393, UINT64_C(5) , true);
                  sailgen_SomezIbzK(&zgsz3183, zghz3103);
                }
                goto finish_match_238;
              }
            case_240: ;
              {
                sailgen_NonezIbzK(&zgsz3183, UNIT);
                goto finish_match_238;
              }
            case_239: ;
            finish_match_238: ;
              COPY(zoptionzIbzK)(&zgaz3100, zgsz3183);
              KILL(zoptionzIbzK)(&zgsz3183);
              goto finish_match_235;
            }
          case_237: ;
            {
              sailgen_NonezIbzK(&zgaz3100, UNIT);
              goto finish_match_235;
            }
          case_236: ;
          finish_match_235: ;
          }
          uint64_t zgsz3187;
          {
            if (zgaz3100.kind != Kind_zSomezIbzK) goto case_296;
            zgsz3187 = CONVERT_OF(fbits, lbits)(zgaz3100.zSomezIbzK, true);
            goto finish_match_241;
          }
        case_296: ;
          {
            __label__ case_295, case_250, finish_match_249;

            /* complete */
            struct zoptionzIbzK zgaz3111;
            CREATE(zoptionzIbzK)(&zgaz3111);
            {
              __label__ case_245, case_244, finish_match_243;

              {
                __label__ case_248, case_247, finish_match_246;

                bool zuz336;
                zuz336 = zhead_expz3.ztup0.zR;
                bool zuz337;
                zuz337 = zhead_expz3.ztup0.zW;
                bool zuz338;
                zuz338 = zhead_expz3.ztup0.zC;
                bool zuz339;
                zuz339 = zhead_expz3.ztup0.zLM;
                bool zuz340;
                zuz340 = zhead_expz3.ztup0.zX;
                bool zuz341;
                zuz341 = zhead_expz3.ztup0.zASR;
                enum zExecutionMode zmapping5z3;
                zmapping5z3 = zhead_expz3.ztup1;
                bool zgaz3110;
                {
                  bool zgaz3109;
                  {
                    bool zgaz3108;
                    {
                      bool zgaz3107;
                      {
                        bool zgaz3106;
                        {
                          bool zgaz3105;
                          zgaz3105 = sailgen_execution_mode_encdec_forwards_matches(zmapping5z3);
                          bool zgsz3190;
                          if (zgaz3105) {  zgsz3190 = eq_bool(zuz341, false);  } else {  zgsz3190 = false;  }
                          zgaz3106 = zgsz3190;
                        }
                        bool zgsz3191;
                        if (zgaz3106) {  zgsz3191 = eq_bool(zuz340, true);  } else {  zgsz3191 = false;  }
                        zgaz3107 = zgsz3191;
                      }
                      bool zgsz3192;
                      if (zgaz3107) {  zgsz3192 = eq_bool(zuz339, true);  } else {  zgsz3192 = false;  }
                      zgaz3108 = zgsz3192;
                    }
                    bool zgsz3193;
                    if (zgaz3108) {  zgsz3193 = eq_bool(zuz338, true);  } else {  zgsz3193 = false;  }
                    zgaz3109 = zgsz3193;
                  }
                  bool zgsz3194;
                  if (zgaz3109) {  zgsz3194 = eq_bool(zuz337, false);  } else {  zgsz3194 = false;  }
                  zgaz3110 = zgsz3194;
                }
                bool zgsz3195;
                if (zgaz3110) {  zgsz3195 = eq_bool(zuz336, true);  } else {  zgsz3195 = false;  }
                bool zgsz3199;
                zgsz3199 = zgsz3195;
                if (!(zgsz3199)) {






                  goto case_245;
                }
                uint64_t zgaz3101;
                zgaz3101 = sailgen_execution_mode_encdec_forwards_infallible(zmapping5z3);
                struct zoptionzIbzK zgsz3196;
                CREATE(zoptionzIbzK)(&zgsz3196);
                {
                  uint64_t zuz342;
                  zuz342 = zgaz3101;
                  uint64_t zgaz3104;
                  {
                    uint64_t zgaz3103;
                    {
                      uint64_t zgaz3102;
                      zgaz3102 = zuz342;
                      zgaz3103 = (UINT64_C(0b01) << 1) | zgaz3102;
                    }
                    zgaz3104 = (UINT64_C(0b01) << 3) | zgaz3103;
                  }
                  {
                    RECREATE(lbits)(&zghz3102);
                    CONVERT_OF(lbits, fbits)(&zghz3102, zgaz3104, UINT64_C(5) , true);
                    sailgen_SomezIbzK(&zgsz3196, zghz3102);
                  }
                  goto finish_match_246;
                }
              case_248: ;
                {
                  sailgen_NonezIbzK(&zgsz3196, UNIT);
                  goto finish_match_246;
                }
              case_247: ;
              finish_match_246: ;
                COPY(zoptionzIbzK)(&zgaz3111, zgsz3196);
                KILL(zoptionzIbzK)(&zgsz3196);
                goto finish_match_243;
              }
            case_245: ;
              {
                sailgen_NonezIbzK(&zgaz3111, UNIT);
                goto finish_match_243;
              }
            case_244: ;
            finish_match_243: ;
            }
            uint64_t zgsz3200;
            {
              if (zgaz3111.kind != Kind_zSomezIbzK) goto case_295;
              zgsz3200 = CONVERT_OF(fbits, lbits)(zgaz3111.zSomezIbzK, true);
              goto finish_match_249;
            }
          case_295: ;
            {
              __label__ case_294, case_258, finish_match_257;

              /* complete */
              struct zoptionzIbzK zgaz3122;
              CREATE(zoptionzIbzK)(&zgaz3122);
              {
                __label__ case_253, case_252, finish_match_251;

                {
                  __label__ case_256, case_255, finish_match_254;

                  bool zuz344;
                  zuz344 = zhead_expz3.ztup0.zR;
                  bool zuz345;
                  zuz345 = zhead_expz3.ztup0.zW;
                  bool zuz346;
                  zuz346 = zhead_expz3.ztup0.zC;
                  bool zuz347;
                  zuz347 = zhead_expz3.ztup0.zLM;
                  bool zuz348;
                  zuz348 = zhead_expz3.ztup0.zX;
                  bool zuz349;
                  zuz349 = zhead_expz3.ztup0.zASR;
                  enum zExecutionMode zmapping6z3;
                  zmapping6z3 = zhead_expz3.ztup1;
                  bool zgaz3121;
                  {
                    bool zgaz3120;
                    {
                      bool zgaz3119;
                      {
                        bool zgaz3118;
                        {
                          bool zgaz3117;
                          {
                            bool zgaz3116;
                            zgaz3116 = sailgen_execution_mode_encdec_forwards_matches(zmapping6z3);
                            bool zgsz3203;
                            if (zgaz3116) {  zgsz3203 = eq_bool(zuz349, false);  } else {  zgsz3203 = false;  }
                            zgaz3117 = zgsz3203;
                          }
                          bool zgsz3204;
                          if (zgaz3117) {  zgsz3204 = eq_bool(zuz348, true);  } else {  zgsz3204 = false;  }
                          zgaz3118 = zgsz3204;
                        }
                        bool zgsz3205;
                        if (zgaz3118) {  zgsz3205 = eq_bool(zuz347, true);  } else {  zgsz3205 = false;  }
                        zgaz3119 = zgsz3205;
                      }
                      bool zgsz3206;
                      if (zgaz3119) {  zgsz3206 = eq_bool(zuz346, true);  } else {  zgsz3206 = false;  }
                      zgaz3120 = zgsz3206;
                    }
                    bool zgsz3207;
                    if (zgaz3120) {  zgsz3207 = eq_bool(zuz345, true);  } else {  zgsz3207 = false;  }
                    zgaz3121 = zgsz3207;
                  }
                  bool zgsz3208;
                  if (zgaz3121) {  zgsz3208 = eq_bool(zuz344, true);  } else {  zgsz3208 = false;  }
                  bool zgsz3212;
                  zgsz3212 = zgsz3208;
                  if (!(zgsz3212)) {






                    goto case_253;
                  }
                  uint64_t zgaz3112;
                  zgaz3112 = sailgen_execution_mode_encdec_forwards_infallible(zmapping6z3);
                  struct zoptionzIbzK zgsz3209;
                  CREATE(zoptionzIbzK)(&zgsz3209);
                  {
                    uint64_t zuz350;
                    zuz350 = zgaz3112;
                    uint64_t zgaz3115;
                    {
                      uint64_t zgaz3114;
                      {
                        uint64_t zgaz3113;
                        zgaz3113 = zuz350;
                        zgaz3114 = (UINT64_C(0b10) << 1) | zgaz3113;
                      }
                      zgaz3115 = (UINT64_C(0b01) << 3) | zgaz3114;
                    }
                    {
                      RECREATE(lbits)(&zghz3101);
                      CONVERT_OF(lbits, fbits)(&zghz3101, zgaz3115, UINT64_C(5) , true);
                      sailgen_SomezIbzK(&zgsz3209, zghz3101);
                    }
                    goto finish_match_254;
                  }
                case_256: ;
                  {
                    sailgen_NonezIbzK(&zgsz3209, UNIT);
                    goto finish_match_254;
                  }
                case_255: ;
                finish_match_254: ;
                  COPY(zoptionzIbzK)(&zgaz3122, zgsz3209);
                  KILL(zoptionzIbzK)(&zgsz3209);
                  goto finish_match_251;
                }
              case_253: ;
                {
                  sailgen_NonezIbzK(&zgaz3122, UNIT);
                  goto finish_match_251;
                }
              case_252: ;
              finish_match_251: ;
              }
              uint64_t zgsz3213;
              {
                if (zgaz3122.kind != Kind_zSomezIbzK) goto case_294;
                zgsz3213 = CONVERT_OF(fbits, lbits)(zgaz3122.zSomezIbzK, true);
                goto finish_match_257;
              }
            case_294: ;
              {
                __label__ case_293, case_266, finish_match_265;

                /* complete */
                struct zoptionzIbzK zgaz3133;
                CREATE(zoptionzIbzK)(&zgaz3133);
                {
                  __label__ case_261, case_260, finish_match_259;

                  {
                    __label__ case_264, case_263, finish_match_262;

                    bool zuz352;
                    zuz352 = zhead_expz3.ztup0.zR;
                    bool zuz353;
                    zuz353 = zhead_expz3.ztup0.zW;
                    bool zuz354;
                    zuz354 = zhead_expz3.ztup0.zC;
                    bool zuz355;
                    zuz355 = zhead_expz3.ztup0.zLM;
                    bool zuz356;
                    zuz356 = zhead_expz3.ztup0.zX;
                    bool zuz357;
                    zuz357 = zhead_expz3.ztup0.zASR;
                    enum zExecutionMode zmapping7z3;
                    zmapping7z3 = zhead_expz3.ztup1;
                    bool zgaz3132;
                    {
                      bool zgaz3131;
                      {
                        bool zgaz3130;
                        {
                          bool zgaz3129;
                          {
                            bool zgaz3128;
                            {
                              bool zgaz3127;
                              zgaz3127 = sailgen_execution_mode_encdec_forwards_matches(zmapping7z3);
                              bool zgsz3216;
                              if (zgaz3127) {  zgsz3216 = eq_bool(zuz357, false);  } else {  zgsz3216 = false;  }
                              zgaz3128 = zgsz3216;
                            }
                            bool zgsz3217;
                            if (zgaz3128) {  zgsz3217 = eq_bool(zuz356, true);  } else {  zgsz3217 = false;  }
                            zgaz3129 = zgsz3217;
                          }
                          bool zgsz3218;
                          if (zgaz3129) {  zgsz3218 = eq_bool(zuz355, false);  } else {  zgsz3218 = false;  }
                          zgaz3130 = zgsz3218;
                        }
                        bool zgsz3219;
                        if (zgaz3130) {  zgsz3219 = eq_bool(zuz354, false);  } else {  zgsz3219 = false;  }
                        zgaz3131 = zgsz3219;
                      }
                      bool zgsz3220;
                      if (zgaz3131) {  zgsz3220 = eq_bool(zuz353, true);  } else {  zgsz3220 = false;  }
                      zgaz3132 = zgsz3220;
                    }
                    bool zgsz3221;
                    if (zgaz3132) {  zgsz3221 = eq_bool(zuz352, true);  } else {  zgsz3221 = false;  }
                    bool zgsz3225;
                    zgsz3225 = zgsz3221;
                    if (!(zgsz3225)) {






                      goto case_261;
                    }
                    uint64_t zgaz3123;
                    zgaz3123 = sailgen_execution_mode_encdec_forwards_infallible(zmapping7z3);
                    struct zoptionzIbzK zgsz3222;
                    CREATE(zoptionzIbzK)(&zgsz3222);
                    {
                      uint64_t zuz358;
                      zuz358 = zgaz3123;
                      uint64_t zgaz3126;
                      {
                        uint64_t zgaz3125;
                        {
                          uint64_t zgaz3124;
                          zgaz3124 = zuz358;
                          zgaz3125 = (UINT64_C(0b11) << 1) | zgaz3124;
                        }
                        zgaz3126 = (UINT64_C(0b01) << 3) | zgaz3125;
                      }
                      {
                        RECREATE(lbits)(&zghz3100);
                        CONVERT_OF(lbits, fbits)(&zghz3100, zgaz3126, UINT64_C(5) , true);
                        sailgen_SomezIbzK(&zgsz3222, zghz3100);
                      }
                      goto finish_match_262;
                    }
                  case_264: ;
                    {
                      sailgen_NonezIbzK(&zgsz3222, UNIT);
                      goto finish_match_262;
                    }
                  case_263: ;
                  finish_match_262: ;
                    COPY(zoptionzIbzK)(&zgaz3133, zgsz3222);
                    KILL(zoptionzIbzK)(&zgsz3222);
                    goto finish_match_259;
                  }
                case_261: ;
                  {
                    sailgen_NonezIbzK(&zgaz3133, UNIT);
                    goto finish_match_259;
                  }
                case_260: ;
                finish_match_259: ;
                }
                uint64_t zgsz3226;
                {
                  if (zgaz3133.kind != Kind_zSomezIbzK) goto case_293;
                  zgsz3226 = CONVERT_OF(fbits, lbits)(zgaz3133.zSomezIbzK, true);
                  goto finish_match_265;
                }
              case_293: ;
                {
                  __label__ case_292, case_274, finish_match_273;

                  /* complete */
                  struct zoptionzIbzK zgaz3141;
                  CREATE(zoptionzIbzK)(&zgaz3141);
                  {
                    __label__ case_269, case_268, finish_match_267;

                    {
                      __label__ case_272, case_271, finish_match_270;

                      bool zuz360;
                      zuz360 = zhead_expz3.ztup0.zR;
                      bool zuz361;
                      zuz361 = zhead_expz3.ztup0.zW;
                      bool zuz362;
                      zuz362 = zhead_expz3.ztup0.zC;
                      bool zuz363;
                      zuz363 = zhead_expz3.ztup0.zLM;
                      bool zuz364;
                      zuz364 = zhead_expz3.ztup0.zX;
                      bool zuz365;
                      zuz365 = zhead_expz3.ztup0.zASR;
                      enum zExecutionMode zmapping8z3;
                      zmapping8z3 = zhead_expz3.ztup1;
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
                                zgaz3135 = sailgen_execution_mode_encdec_forwards_matches(zmapping8z3);
                                bool zgsz3229;
                                if (zgaz3135) {  zgsz3229 = eq_bool(zuz365, false);  } else {  zgsz3229 = false;  }
                                zgaz3136 = zgsz3229;
                              }
                              bool zgsz3230;
                              if (zgaz3136) {  zgsz3230 = eq_bool(zuz364, false);  } else {  zgsz3230 = false;  }
                              zgaz3137 = zgsz3230;
                            }
                            bool zgsz3231;
                            if (zgaz3137) {  zgsz3231 = eq_bool(zuz363, false);  } else {  zgsz3231 = false;  }
                            zgaz3138 = zgsz3231;
                          }
                          bool zgsz3232;
                          if (zgaz3138) {  zgsz3232 = eq_bool(zuz362, true);  } else {  zgsz3232 = false;  }
                          zgaz3139 = zgsz3232;
                        }
                        bool zgsz3233;
                        if (zgaz3139) {  zgsz3233 = eq_bool(zuz361, false);  } else {  zgsz3233 = false;  }
                        zgaz3140 = zgsz3233;
                      }
                      bool zgsz3234;
                      if (zgaz3140) {  zgsz3234 = eq_bool(zuz360, true);  } else {  zgsz3234 = false;  }
                      bool zgsz3238;
                      zgsz3238 = zgsz3234;
                      if (!(zgsz3238)) {






                        goto case_269;
                      }
                      uint64_t zgaz3134;
                      zgaz3134 = sailgen_execution_mode_encdec_forwards_infallible(zmapping8z3);
                      struct zoptionzIbzK zgsz3235;
                      CREATE(zoptionzIbzK)(&zgsz3235);
                      {
                        uint64_t zb__2;
                        zb__2 = zgaz3134;
                        bool zgsz3237;
                        zgsz3237 = (zb__2 == UINT64_C(0b0));
                        if (!(zgsz3237)) {

                          goto case_272;
                        }
                        {
                          RECREATE(lbits)(&zghz399);
                          CONVERT_OF(lbits, fbits)(&zghz399, UINT64_C(0b10011), UINT64_C(5) , true);
                          sailgen_SomezIbzK(&zgsz3235, zghz399);
                        }
                        goto finish_match_270;
                      }
                    case_272: ;
                      {
                        sailgen_NonezIbzK(&zgsz3235, UNIT);
                        goto finish_match_270;
                      }
                    case_271: ;
                    finish_match_270: ;
                      COPY(zoptionzIbzK)(&zgaz3141, zgsz3235);
                      KILL(zoptionzIbzK)(&zgsz3235);
                      goto finish_match_267;
                    }
                  case_269: ;
                    {
                      sailgen_NonezIbzK(&zgaz3141, UNIT);
                      goto finish_match_267;
                    }
                  case_268: ;
                  finish_match_267: ;
                  }
                  uint64_t zgsz3239;
                  {
                    if (zgaz3141.kind != Kind_zSomezIbzK) goto case_292;
                    zgsz3239 = CONVERT_OF(fbits, lbits)(zgaz3141.zSomezIbzK, true);
                    goto finish_match_273;
                  }
                case_292: ;
                  {
                    __label__ case_291, case_282, finish_match_281;

                    /* complete */
                    struct zoptionzIbzK zgaz3149;
                    CREATE(zoptionzIbzK)(&zgaz3149);
                    {
                      __label__ case_277, case_276, finish_match_275;

                      {
                        __label__ case_280, case_279, finish_match_278;

                        bool zuz367;
                        zuz367 = zhead_expz3.ztup0.zR;
                        bool zuz368;
                        zuz368 = zhead_expz3.ztup0.zW;
                        bool zuz369;
                        zuz369 = zhead_expz3.ztup0.zC;
                        bool zuz370;
                        zuz370 = zhead_expz3.ztup0.zLM;
                        bool zuz371;
                        zuz371 = zhead_expz3.ztup0.zX;
                        bool zuz372;
                        zuz372 = zhead_expz3.ztup0.zASR;
                        enum zExecutionMode zmapping9z3;
                        zmapping9z3 = zhead_expz3.ztup1;
                        bool zgaz3148;
                        {
                          bool zgaz3147;
                          {
                            bool zgaz3146;
                            {
                              bool zgaz3145;
                              {
                                bool zgaz3144;
                                {
                                  bool zgaz3143;
                                  zgaz3143 = sailgen_execution_mode_encdec_forwards_matches(zmapping9z3);
                                  bool zgsz3242;
                                  if (zgaz3143) {  zgsz3242 = eq_bool(zuz372, false);  } else {  zgsz3242 = false;  }
                                  zgaz3144 = zgsz3242;
                                }
                                bool zgsz3243;
                                if (zgaz3144) {  zgsz3243 = eq_bool(zuz371, false);  } else {  zgsz3243 = false;  }
                                zgaz3145 = zgsz3243;
                              }
                              bool zgsz3244;
                              if (zgaz3145) {  zgsz3244 = eq_bool(zuz370, true);  } else {  zgsz3244 = false;  }
                              zgaz3146 = zgsz3244;
                            }
                            bool zgsz3245;
                            if (zgaz3146) {  zgsz3245 = eq_bool(zuz369, true);  } else {  zgsz3245 = false;  }
                            zgaz3147 = zgsz3245;
                          }
                          bool zgsz3246;
                          if (zgaz3147) {  zgsz3246 = eq_bool(zuz368, false);  } else {  zgsz3246 = false;  }
                          zgaz3148 = zgsz3246;
                        }
                        bool zgsz3247;
                        if (zgaz3148) {  zgsz3247 = eq_bool(zuz367, true);  } else {  zgsz3247 = false;  }
                        bool zgsz3251;
                        zgsz3251 = zgsz3247;
                        if (!(zgsz3251)) {






                          goto case_277;
                        }
                        uint64_t zgaz3142;
                        zgaz3142 = sailgen_execution_mode_encdec_forwards_infallible(zmapping9z3);
                        struct zoptionzIbzK zgsz3248;
                        CREATE(zoptionzIbzK)(&zgsz3248);
                        {
                          uint64_t zb__1;
                          zb__1 = zgaz3142;
                          bool zgsz3250;
                          zgsz3250 = (zb__1 == UINT64_C(0b0));
                          if (!(zgsz3250)) {

                            goto case_280;
                          }
                          {
                            RECREATE(lbits)(&zghz398);
                            CONVERT_OF(lbits, fbits)(&zghz398, UINT64_C(0b11011), UINT64_C(5) , true);
                            sailgen_SomezIbzK(&zgsz3248, zghz398);
                          }
                          goto finish_match_278;
                        }
                      case_280: ;
                        {
                          sailgen_NonezIbzK(&zgsz3248, UNIT);
                          goto finish_match_278;
                        }
                      case_279: ;
                      finish_match_278: ;
                        COPY(zoptionzIbzK)(&zgaz3149, zgsz3248);
                        KILL(zoptionzIbzK)(&zgsz3248);
                        goto finish_match_275;
                      }
                    case_277: ;
                      {
                        sailgen_NonezIbzK(&zgaz3149, UNIT);
                        goto finish_match_275;
                      }
                    case_276: ;
                    finish_match_275: ;
                    }
                    uint64_t zgsz3252;
                    {
                      if (zgaz3149.kind != Kind_zSomezIbzK) goto case_291;
                      zgsz3252 = CONVERT_OF(fbits, lbits)(zgaz3149.zSomezIbzK, true);
                      goto finish_match_281;
                    }
                  case_291: ;
                    {
                      __label__ case_290, finish_match_289;

                      /* complete */
                      struct zoptionzIbzK zgaz3157;
                      CREATE(zoptionzIbzK)(&zgaz3157);
                      {
                        __label__ case_285, case_284, finish_match_283;

                        {
                          __label__ case_288, case_287, finish_match_286;

                          bool zuz374;
                          zuz374 = zhead_expz3.ztup0.zR;
                          bool zuz375;
                          zuz375 = zhead_expz3.ztup0.zW;
                          bool zuz376;
                          zuz376 = zhead_expz3.ztup0.zC;
                          bool zuz377;
                          zuz377 = zhead_expz3.ztup0.zLM;
                          bool zuz378;
                          zuz378 = zhead_expz3.ztup0.zX;
                          bool zuz379;
                          zuz379 = zhead_expz3.ztup0.zASR;
                          enum zExecutionMode zmapping10z3;
                          zmapping10z3 = zhead_expz3.ztup1;
                          bool zgaz3156;
                          {
                            bool zgaz3155;
                            {
                              bool zgaz3154;
                              {
                                bool zgaz3153;
                                {
                                  bool zgaz3152;
                                  {
                                    bool zgaz3151;
                                    zgaz3151 = sailgen_execution_mode_encdec_forwards_matches(zmapping10z3);
                                    bool zgsz3255;
                                    if (zgaz3151) {  zgsz3255 = eq_bool(zuz379, false);  } else {  zgsz3255 = false;  }
                                    zgaz3152 = zgsz3255;
                                  }
                                  bool zgsz3256;
                                  if (zgaz3152) {  zgsz3256 = eq_bool(zuz378, false);  } else {  zgsz3256 = false;  }
                                  zgaz3153 = zgsz3256;
                                }
                                bool zgsz3257;
                                if (zgaz3153) {  zgsz3257 = eq_bool(zuz377, true);  } else {  zgsz3257 = false;  }
                                zgaz3154 = zgsz3257;
                              }
                              bool zgsz3258;
                              if (zgaz3154) {  zgsz3258 = eq_bool(zuz376, true);  } else {  zgsz3258 = false;  }
                              zgaz3155 = zgsz3258;
                            }
                            bool zgsz3259;
                            if (zgaz3155) {  zgsz3259 = eq_bool(zuz375, true);  } else {  zgsz3259 = false;  }
                            zgaz3156 = zgsz3259;
                          }
                          bool zgsz3260;
                          if (zgaz3156) {  zgsz3260 = eq_bool(zuz374, true);  } else {  zgsz3260 = false;  }
                          bool zgsz3264;
                          zgsz3264 = zgsz3260;
                          if (!(zgsz3264)) {






                            goto case_285;
                          }
                          uint64_t zgaz3150;
                          zgaz3150 = sailgen_execution_mode_encdec_forwards_infallible(zmapping10z3);
                          struct zoptionzIbzK zgsz3261;
                          CREATE(zoptionzIbzK)(&zgsz3261);
                          {
                            uint64_t zb__0;
                            zb__0 = zgaz3150;
                            bool zgsz3263;
                            zgsz3263 = (zb__0 == UINT64_C(0b0));
                            if (!(zgsz3263)) {

                              goto case_288;
                            }
                            {
                              RECREATE(lbits)(&zghz397);
                              CONVERT_OF(lbits, fbits)(&zghz397, UINT64_C(0b11111), UINT64_C(5) , true);
                              sailgen_SomezIbzK(&zgsz3261, zghz397);
                            }
                            goto finish_match_286;
                          }
                        case_288: ;
                          {
                            sailgen_NonezIbzK(&zgsz3261, UNIT);
                            goto finish_match_286;
                          }
                        case_287: ;
                        finish_match_286: ;
                          COPY(zoptionzIbzK)(&zgaz3157, zgsz3261);
                          KILL(zoptionzIbzK)(&zgsz3261);
                          goto finish_match_283;
                        }
                      case_285: ;
                        {
                          sailgen_NonezIbzK(&zgaz3157, UNIT);
                          goto finish_match_283;
                        }
                      case_284: ;
                      finish_match_283: ;
                      }
                      uint64_t zgsz3265;
                      {
                        if (zgaz3157.kind != Kind_zSomezIbzK) goto case_290;
                        zgsz3265 = CONVERT_OF(fbits, lbits)(zgaz3157.zSomezIbzK, true);
                        goto finish_match_289;
                      }
                    case_290: ;
                      sail_match_failure("ap_m_encdec_backwards");
                    finish_match_289: ;
                      zgsz3252 = zgsz3265;
                      KILL(zoptionzIbzK)(&zgaz3157);
                      goto finish_match_281;
                    }
                  case_282: ;
                  finish_match_281: ;
                    zgsz3239 = zgsz3252;
                    KILL(zoptionzIbzK)(&zgaz3149);
                    goto finish_match_273;
                  }
                case_274: ;
                finish_match_273: ;
                  zgsz3226 = zgsz3239;
                  KILL(zoptionzIbzK)(&zgaz3141);
                  goto finish_match_265;
                }
              case_266: ;
              finish_match_265: ;
                zgsz3213 = zgsz3226;
                KILL(zoptionzIbzK)(&zgaz3133);
                goto finish_match_257;
              }
            case_258: ;
            finish_match_257: ;
              zgsz3200 = zgsz3213;
              KILL(zoptionzIbzK)(&zgaz3122);
              goto finish_match_249;
            }
          case_250: ;
          finish_match_249: ;
            zgsz3187 = zgsz3200;
            KILL(zoptionzIbzK)(&zgaz3111);
            goto finish_match_241;
          }
        case_242: ;
        finish_match_241: ;
          zgsz3174 = zgsz3187;
          KILL(zoptionzIbzK)(&zgaz3100);
          goto finish_match_233;
        }
      case_234: ;
      finish_match_233: ;
        zgsz3161 = zgsz3174;
        KILL(zoptionzIbzK)(&zgaz389);
        goto finish_match_225;
      }
    case_226: ;
    finish_match_225: ;
      zgsz3148 = zgsz3161;
      KILL(zoptionzIbzK)(&zgaz381);
      goto finish_match_217;
    }
  case_218: ;
  finish_match_217: ;
    zcbz347 = zgsz3148;
    KILL(zoptionzIbzK)(&zgaz373);
    goto finish_match_209;
  }
case_210: ;
finish_match_209: ;
  KILL(zoptionzIbzK)(&zgaz365);

end_function_301: ;
  return zcbz347;
end_block_exception_302: ;

  return UINT64_C(0xdeadc0de);
}



static void finish_sailgen_ap_m_encdec_backwards(void)
{
  KILL(lbits)(&zghz3107);
  KILL(lbits)(&zghz3106);
  KILL(lbits)(&zghz3105);
  KILL(lbits)(&zghz3104);
  KILL(lbits)(&zghz3103);
  KILL(lbits)(&zghz3102);
  KILL(lbits)(&zghz3101);
  KILL(lbits)(&zghz3100);
  KILL(lbits)(&zghz399);
  KILL(lbits)(&zghz398);
  KILL(lbits)(&zghz397);
}

static uint64_t sailgen_ap_m_encdec_backwards_infallible(struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9);

lbits zghz3108;
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

static void startup_sailgen_ap_m_encdec_backwards_infallible(void)
{
  CREATE(lbits)(&zghz3108);
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
}

static uint64_t sailgen_ap_m_encdec_backwards_infallible(struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zargz3)
{
  __label__ case_400, case_310, finish_match_309, end_function_401, end_block_exception_402;

  uint64_t zcbz348;
  struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zhead_expz3;
  zhead_expz3 = zargz3;
  struct zoptionzIbzK zgaz365;
  CREATE(zoptionzIbzK)(&zgaz365);
  {
    __label__ case_305, case_304, finish_match_303;

    {
      __label__ case_308, case_307, finish_match_306;

      bool zp0z3;
      zp0z3 = zhead_expz3.ztup0.zR;
      bool zp1z3;
      zp1z3 = zhead_expz3.ztup0.zW;
      bool zp2z3;
      zp2z3 = zhead_expz3.ztup0.zC;
      bool zp3z3;
      zp3z3 = zhead_expz3.ztup0.zLM;
      bool zp4z3;
      zp4z3 = zhead_expz3.ztup0.zX;
      bool zp5z3;
      zp5z3 = zhead_expz3.ztup0.zASR;
      enum zExecutionMode zmapping0z3;
      zmapping0z3 = zhead_expz3.ztup1;
      bool zgaz364;
      {
        bool zgaz363;
        {
          bool zgaz362;
          {
            bool zgaz361;
            {
              bool zgaz360;
              {
                bool zgaz359;
                zgaz359 = sailgen_execution_mode_encdec_forwards_matches(zmapping0z3);
                bool zgsz3289;
                if (zgaz359) {  zgsz3289 = eq_bool(zp5z3, false);  } else {  zgsz3289 = false;  }
                zgaz360 = zgsz3289;
              }
              bool zgsz3290;
              if (zgaz360) {  zgsz3290 = eq_bool(zp4z3, false);  } else {  zgsz3290 = false;  }
              zgaz361 = zgsz3290;
            }
            bool zgsz3291;
            if (zgaz361) {  zgsz3291 = eq_bool(zp3z3, false);  } else {  zgsz3291 = false;  }
            zgaz362 = zgsz3291;
          }
          bool zgsz3292;
          if (zgaz362) {  zgsz3292 = eq_bool(zp2z3, false);  } else {  zgsz3292 = false;  }
          zgaz363 = zgsz3292;
        }
        bool zgsz3293;
        if (zgaz363) {  zgsz3293 = eq_bool(zp1z3, false);  } else {  zgsz3293 = false;  }
        zgaz364 = zgsz3293;
      }
      bool zgsz3294;
      if (zgaz364) {  zgsz3294 = eq_bool(zp0z3, false);  } else {  zgsz3294 = false;  }
      bool zgsz3298;
      zgsz3298 = zgsz3294;
      if (!(zgsz3298)) {






        goto case_305;
      }
      uint64_t zgaz358;
      zgaz358 = sailgen_execution_mode_encdec_forwards_infallible(zmapping0z3);
      struct zoptionzIbzK zgsz3295;
      CREATE(zoptionzIbzK)(&zgsz3295);
      {
        uint64_t zb__6;
        zb__6 = zgaz358;
        bool zgsz3297;
        zgsz3297 = (zb__6 == UINT64_C(0b0));
        if (!(zgsz3297)) {

          goto case_308;
        }
        {
          RECREATE(lbits)(&zghz3118);
          CONVERT_OF(lbits, fbits)(&zghz3118, UINT64_C(0b00000), UINT64_C(5) , true);
          sailgen_SomezIbzK(&zgsz3295, zghz3118);
        }
        goto finish_match_306;
      }
    case_308: ;
      {
        sailgen_NonezIbzK(&zgsz3295, UNIT);
        goto finish_match_306;
      }
    case_307: ;
    finish_match_306: ;
      COPY(zoptionzIbzK)(&zgaz365, zgsz3295);
      KILL(zoptionzIbzK)(&zgsz3295);
      goto finish_match_303;
    }
  case_305: ;
    {
      sailgen_NonezIbzK(&zgaz365, UNIT);
      goto finish_match_303;
    }
  case_304: ;
  finish_match_303: ;
  }
  {
    if (zgaz365.kind != Kind_zSomezIbzK) goto case_400;
    uint64_t zresult;
    zresult = CONVERT_OF(fbits, lbits)(zgaz365.zSomezIbzK, true);
    zcbz348 = zresult;
    goto finish_match_309;
  }
case_400: ;
  {
    __label__ case_399, case_318, finish_match_317;

    /* complete */
    struct zoptionzIbzK zgaz373;
    CREATE(zoptionzIbzK)(&zgaz373);
    {
      __label__ case_313, case_312, finish_match_311;

      {
        __label__ case_316, case_315, finish_match_314;

        bool zuz381;
        zuz381 = zhead_expz3.ztup0.zR;
        bool zuz382;
        zuz382 = zhead_expz3.ztup0.zW;
        bool zuz383;
        zuz383 = zhead_expz3.ztup0.zC;
        bool zuz384;
        zuz384 = zhead_expz3.ztup0.zLM;
        bool zuz385;
        zuz385 = zhead_expz3.ztup0.zX;
        bool zuz386;
        zuz386 = zhead_expz3.ztup0.zASR;
        enum zExecutionMode zmapping1z3;
        zmapping1z3 = zhead_expz3.ztup1;
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
                  zgaz367 = sailgen_execution_mode_encdec_forwards_matches(zmapping1z3);
                  bool zgsz3302;
                  if (zgaz367) {  zgsz3302 = eq_bool(zuz386, false);  } else {  zgsz3302 = false;  }
                  zgaz368 = zgsz3302;
                }
                bool zgsz3303;
                if (zgaz368) {  zgsz3303 = eq_bool(zuz385, false);  } else {  zgsz3303 = false;  }
                zgaz369 = zgsz3303;
              }
              bool zgsz3304;
              if (zgaz369) {  zgsz3304 = eq_bool(zuz384, false);  } else {  zgsz3304 = false;  }
              zgaz370 = zgsz3304;
            }
            bool zgsz3305;
            if (zgaz370) {  zgsz3305 = eq_bool(zuz383, false);  } else {  zgsz3305 = false;  }
            zgaz371 = zgsz3305;
          }
          bool zgsz3306;
          if (zgaz371) {  zgsz3306 = eq_bool(zuz382, false);  } else {  zgsz3306 = false;  }
          zgaz372 = zgsz3306;
        }
        bool zgsz3307;
        if (zgaz372) {  zgsz3307 = eq_bool(zuz381, true);  } else {  zgsz3307 = false;  }
        bool zgsz3311;
        zgsz3311 = zgsz3307;
        if (!(zgsz3311)) {






          goto case_313;
        }
        uint64_t zgaz366;
        zgaz366 = sailgen_execution_mode_encdec_forwards_infallible(zmapping1z3);
        struct zoptionzIbzK zgsz3308;
        CREATE(zoptionzIbzK)(&zgsz3308);
        {
          uint64_t zb__5;
          zb__5 = zgaz366;
          bool zgsz3310;
          zgsz3310 = (zb__5 == UINT64_C(0b0));
          if (!(zgsz3310)) {

            goto case_316;
          }
          {
            RECREATE(lbits)(&zghz3117);
            CONVERT_OF(lbits, fbits)(&zghz3117, UINT64_C(0b00001), UINT64_C(5) , true);
            sailgen_SomezIbzK(&zgsz3308, zghz3117);
          }
          goto finish_match_314;
        }
      case_316: ;
        {
          sailgen_NonezIbzK(&zgsz3308, UNIT);
          goto finish_match_314;
        }
      case_315: ;
      finish_match_314: ;
        COPY(zoptionzIbzK)(&zgaz373, zgsz3308);
        KILL(zoptionzIbzK)(&zgsz3308);
        goto finish_match_311;
      }
    case_313: ;
      {
        sailgen_NonezIbzK(&zgaz373, UNIT);
        goto finish_match_311;
      }
    case_312: ;
    finish_match_311: ;
    }
    uint64_t zgsz3312;
    {
      if (zgaz373.kind != Kind_zSomezIbzK) goto case_399;
      zgsz3312 = CONVERT_OF(fbits, lbits)(zgaz373.zSomezIbzK, true);
      goto finish_match_317;
    }
  case_399: ;
    {
      __label__ case_398, case_326, finish_match_325;

      /* complete */
      struct zoptionzIbzK zgaz381;
      CREATE(zoptionzIbzK)(&zgaz381);
      {
        __label__ case_321, case_320, finish_match_319;

        {
          __label__ case_324, case_323, finish_match_322;

          bool zuz388;
          zuz388 = zhead_expz3.ztup0.zR;
          bool zuz389;
          zuz389 = zhead_expz3.ztup0.zW;
          bool zuz390;
          zuz390 = zhead_expz3.ztup0.zC;
          bool zuz391;
          zuz391 = zhead_expz3.ztup0.zLM;
          bool zuz392;
          zuz392 = zhead_expz3.ztup0.zX;
          bool zuz393;
          zuz393 = zhead_expz3.ztup0.zASR;
          enum zExecutionMode zmapping2z3;
          zmapping2z3 = zhead_expz3.ztup1;
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
                    zgaz375 = sailgen_execution_mode_encdec_forwards_matches(zmapping2z3);
                    bool zgsz3315;
                    if (zgaz375) {  zgsz3315 = eq_bool(zuz393, false);  } else {  zgsz3315 = false;  }
                    zgaz376 = zgsz3315;
                  }
                  bool zgsz3316;
                  if (zgaz376) {  zgsz3316 = eq_bool(zuz392, false);  } else {  zgsz3316 = false;  }
                  zgaz377 = zgsz3316;
                }
                bool zgsz3317;
                if (zgaz377) {  zgsz3317 = eq_bool(zuz391, false);  } else {  zgsz3317 = false;  }
                zgaz378 = zgsz3317;
              }
              bool zgsz3318;
              if (zgaz378) {  zgsz3318 = eq_bool(zuz390, false);  } else {  zgsz3318 = false;  }
              zgaz379 = zgsz3318;
            }
            bool zgsz3319;
            if (zgaz379) {  zgsz3319 = eq_bool(zuz389, true);  } else {  zgsz3319 = false;  }
            zgaz380 = zgsz3319;
          }
          bool zgsz3320;
          if (zgaz380) {  zgsz3320 = eq_bool(zuz388, false);  } else {  zgsz3320 = false;  }
          bool zgsz3324;
          zgsz3324 = zgsz3320;
          if (!(zgsz3324)) {






            goto case_321;
          }
          uint64_t zgaz374;
          zgaz374 = sailgen_execution_mode_encdec_forwards_infallible(zmapping2z3);
          struct zoptionzIbzK zgsz3321;
          CREATE(zoptionzIbzK)(&zgsz3321);
          {
            uint64_t zb__4;
            zb__4 = zgaz374;
            bool zgsz3323;
            zgsz3323 = (zb__4 == UINT64_C(0b0));
            if (!(zgsz3323)) {

              goto case_324;
            }
            {
              RECREATE(lbits)(&zghz3116);
              CONVERT_OF(lbits, fbits)(&zghz3116, UINT64_C(0b00100), UINT64_C(5) , true);
              sailgen_SomezIbzK(&zgsz3321, zghz3116);
            }
            goto finish_match_322;
          }
        case_324: ;
          {
            sailgen_NonezIbzK(&zgsz3321, UNIT);
            goto finish_match_322;
          }
        case_323: ;
        finish_match_322: ;
          COPY(zoptionzIbzK)(&zgaz381, zgsz3321);
          KILL(zoptionzIbzK)(&zgsz3321);
          goto finish_match_319;
        }
      case_321: ;
        {
          sailgen_NonezIbzK(&zgaz381, UNIT);
          goto finish_match_319;
        }
      case_320: ;
      finish_match_319: ;
      }
      uint64_t zgsz3325;
      {
        if (zgaz381.kind != Kind_zSomezIbzK) goto case_398;
        zgsz3325 = CONVERT_OF(fbits, lbits)(zgaz381.zSomezIbzK, true);
        goto finish_match_325;
      }
    case_398: ;
      {
        __label__ case_397, case_334, finish_match_333;

        /* complete */
        struct zoptionzIbzK zgaz389;
        CREATE(zoptionzIbzK)(&zgaz389);
        {
          __label__ case_329, case_328, finish_match_327;

          {
            __label__ case_332, case_331, finish_match_330;

            bool zuz395;
            zuz395 = zhead_expz3.ztup0.zR;
            bool zuz396;
            zuz396 = zhead_expz3.ztup0.zW;
            bool zuz397;
            zuz397 = zhead_expz3.ztup0.zC;
            bool zuz398;
            zuz398 = zhead_expz3.ztup0.zLM;
            bool zuz399;
            zuz399 = zhead_expz3.ztup0.zX;
            bool zuz3100;
            zuz3100 = zhead_expz3.ztup0.zASR;
            enum zExecutionMode zmapping3z3;
            zmapping3z3 = zhead_expz3.ztup1;
            bool zgaz388;
            {
              bool zgaz387;
              {
                bool zgaz386;
                {
                  bool zgaz385;
                  {
                    bool zgaz384;
                    {
                      bool zgaz383;
                      zgaz383 = sailgen_execution_mode_encdec_forwards_matches(zmapping3z3);
                      bool zgsz3328;
                      if (zgaz383) {  zgsz3328 = eq_bool(zuz3100, false);  } else {  zgsz3328 = false;  }
                      zgaz384 = zgsz3328;
                    }
                    bool zgsz3329;
                    if (zgaz384) {  zgsz3329 = eq_bool(zuz399, false);  } else {  zgsz3329 = false;  }
                    zgaz385 = zgsz3329;
                  }
                  bool zgsz3330;
                  if (zgaz385) {  zgsz3330 = eq_bool(zuz398, false);  } else {  zgsz3330 = false;  }
                  zgaz386 = zgsz3330;
                }
                bool zgsz3331;
                if (zgaz386) {  zgsz3331 = eq_bool(zuz397, false);  } else {  zgsz3331 = false;  }
                zgaz387 = zgsz3331;
              }
              bool zgsz3332;
              if (zgaz387) {  zgsz3332 = eq_bool(zuz396, true);  } else {  zgsz3332 = false;  }
              zgaz388 = zgsz3332;
            }
            bool zgsz3333;
            if (zgaz388) {  zgsz3333 = eq_bool(zuz395, true);  } else {  zgsz3333 = false;  }
            bool zgsz3337;
            zgsz3337 = zgsz3333;
            if (!(zgsz3337)) {






              goto case_329;
            }
            uint64_t zgaz382;
            zgaz382 = sailgen_execution_mode_encdec_forwards_infallible(zmapping3z3);
            struct zoptionzIbzK zgsz3334;
            CREATE(zoptionzIbzK)(&zgsz3334);
            {
              uint64_t zb__3;
              zb__3 = zgaz382;
              bool zgsz3336;
              zgsz3336 = (zb__3 == UINT64_C(0b0));
              if (!(zgsz3336)) {

                goto case_332;
              }
              {
                RECREATE(lbits)(&zghz3115);
                CONVERT_OF(lbits, fbits)(&zghz3115, UINT64_C(0b00101), UINT64_C(5) , true);
                sailgen_SomezIbzK(&zgsz3334, zghz3115);
              }
              goto finish_match_330;
            }
          case_332: ;
            {
              sailgen_NonezIbzK(&zgsz3334, UNIT);
              goto finish_match_330;
            }
          case_331: ;
          finish_match_330: ;
            COPY(zoptionzIbzK)(&zgaz389, zgsz3334);
            KILL(zoptionzIbzK)(&zgsz3334);
            goto finish_match_327;
          }
        case_329: ;
          {
            sailgen_NonezIbzK(&zgaz389, UNIT);
            goto finish_match_327;
          }
        case_328: ;
        finish_match_327: ;
        }
        uint64_t zgsz3338;
        {
          if (zgaz389.kind != Kind_zSomezIbzK) goto case_397;
          zgsz3338 = CONVERT_OF(fbits, lbits)(zgaz389.zSomezIbzK, true);
          goto finish_match_333;
        }
      case_397: ;
        {
          __label__ case_396, case_342, finish_match_341;

          /* complete */
          struct zoptionzIbzK zgaz3100;
          CREATE(zoptionzIbzK)(&zgaz3100);
          {
            __label__ case_337, case_336, finish_match_335;

            {
              __label__ case_340, case_339, finish_match_338;

              bool zuz3102;
              zuz3102 = zhead_expz3.ztup0.zR;
              bool zuz3103;
              zuz3103 = zhead_expz3.ztup0.zW;
              bool zuz3104;
              zuz3104 = zhead_expz3.ztup0.zC;
              bool zuz3105;
              zuz3105 = zhead_expz3.ztup0.zLM;
              bool zuz3106;
              zuz3106 = zhead_expz3.ztup0.zX;
              bool zuz3107;
              zuz3107 = zhead_expz3.ztup0.zASR;
              enum zExecutionMode zmapping4z3;
              zmapping4z3 = zhead_expz3.ztup1;
              bool zgaz399;
              {
                bool zgaz398;
                {
                  bool zgaz397;
                  {
                    bool zgaz396;
                    {
                      bool zgaz395;
                      {
                        bool zgaz394;
                        zgaz394 = sailgen_execution_mode_encdec_forwards_matches(zmapping4z3);
                        bool zgsz3341;
                        if (zgaz394) {  zgsz3341 = eq_bool(zuz3107, true);  } else {  zgsz3341 = false;  }
                        zgaz395 = zgsz3341;
                      }
                      bool zgsz3342;
                      if (zgaz395) {  zgsz3342 = eq_bool(zuz3106, true);  } else {  zgsz3342 = false;  }
                      zgaz396 = zgsz3342;
                    }
                    bool zgsz3343;
                    if (zgaz396) {  zgsz3343 = eq_bool(zuz3105, true);  } else {  zgsz3343 = false;  }
                    zgaz397 = zgsz3343;
                  }
                  bool zgsz3344;
                  if (zgaz397) {  zgsz3344 = eq_bool(zuz3104, true);  } else {  zgsz3344 = false;  }
                  zgaz398 = zgsz3344;
                }
                bool zgsz3345;
                if (zgaz398) {  zgsz3345 = eq_bool(zuz3103, true);  } else {  zgsz3345 = false;  }
                zgaz399 = zgsz3345;
              }
              bool zgsz3346;
              if (zgaz399) {  zgsz3346 = eq_bool(zuz3102, true);  } else {  zgsz3346 = false;  }
              bool zgsz3350;
              zgsz3350 = zgsz3346;
              if (!(zgsz3350)) {






                goto case_337;
              }
              uint64_t zgaz390;
              zgaz390 = sailgen_execution_mode_encdec_forwards_infallible(zmapping4z3);
              struct zoptionzIbzK zgsz3347;
              CREATE(zoptionzIbzK)(&zgsz3347);
              {
                uint64_t zm;
                zm = zgaz390;
                uint64_t zgaz393;
                {
                  uint64_t zgaz392;
                  {
                    uint64_t zgaz391;
                    zgaz391 = zm;
                    zgaz392 = (UINT64_C(0b00) << 1) | zgaz391;
                  }
                  zgaz393 = (UINT64_C(0b01) << 3) | zgaz392;
                }
                {
                  RECREATE(lbits)(&zghz3114);
                  CONVERT_OF(lbits, fbits)(&zghz3114, zgaz393, UINT64_C(5) , true);
                  sailgen_SomezIbzK(&zgsz3347, zghz3114);
                }
                goto finish_match_338;
              }
            case_340: ;
              {
                sailgen_NonezIbzK(&zgsz3347, UNIT);
                goto finish_match_338;
              }
            case_339: ;
            finish_match_338: ;
              COPY(zoptionzIbzK)(&zgaz3100, zgsz3347);
              KILL(zoptionzIbzK)(&zgsz3347);
              goto finish_match_335;
            }
          case_337: ;
            {
              sailgen_NonezIbzK(&zgaz3100, UNIT);
              goto finish_match_335;
            }
          case_336: ;
          finish_match_335: ;
          }
          uint64_t zgsz3351;
          {
            if (zgaz3100.kind != Kind_zSomezIbzK) goto case_396;
            zgsz3351 = CONVERT_OF(fbits, lbits)(zgaz3100.zSomezIbzK, true);
            goto finish_match_341;
          }
        case_396: ;
          {
            __label__ case_395, case_350, finish_match_349;

            /* complete */
            struct zoptionzIbzK zgaz3111;
            CREATE(zoptionzIbzK)(&zgaz3111);
            {
              __label__ case_345, case_344, finish_match_343;

              {
                __label__ case_348, case_347, finish_match_346;

                bool zuz3109;
                zuz3109 = zhead_expz3.ztup0.zR;
                bool zuz3110;
                zuz3110 = zhead_expz3.ztup0.zW;
                bool zuz3111;
                zuz3111 = zhead_expz3.ztup0.zC;
                bool zuz3112;
                zuz3112 = zhead_expz3.ztup0.zLM;
                bool zuz3113;
                zuz3113 = zhead_expz3.ztup0.zX;
                bool zuz3114;
                zuz3114 = zhead_expz3.ztup0.zASR;
                enum zExecutionMode zmapping5z3;
                zmapping5z3 = zhead_expz3.ztup1;
                bool zgaz3110;
                {
                  bool zgaz3109;
                  {
                    bool zgaz3108;
                    {
                      bool zgaz3107;
                      {
                        bool zgaz3106;
                        {
                          bool zgaz3105;
                          zgaz3105 = sailgen_execution_mode_encdec_forwards_matches(zmapping5z3);
                          bool zgsz3354;
                          if (zgaz3105) {  zgsz3354 = eq_bool(zuz3114, false);  } else {  zgsz3354 = false;  }
                          zgaz3106 = zgsz3354;
                        }
                        bool zgsz3355;
                        if (zgaz3106) {  zgsz3355 = eq_bool(zuz3113, true);  } else {  zgsz3355 = false;  }
                        zgaz3107 = zgsz3355;
                      }
                      bool zgsz3356;
                      if (zgaz3107) {  zgsz3356 = eq_bool(zuz3112, true);  } else {  zgsz3356 = false;  }
                      zgaz3108 = zgsz3356;
                    }
                    bool zgsz3357;
                    if (zgaz3108) {  zgsz3357 = eq_bool(zuz3111, true);  } else {  zgsz3357 = false;  }
                    zgaz3109 = zgsz3357;
                  }
                  bool zgsz3358;
                  if (zgaz3109) {  zgsz3358 = eq_bool(zuz3110, false);  } else {  zgsz3358 = false;  }
                  zgaz3110 = zgsz3358;
                }
                bool zgsz3359;
                if (zgaz3110) {  zgsz3359 = eq_bool(zuz3109, true);  } else {  zgsz3359 = false;  }
                bool zgsz3363;
                zgsz3363 = zgsz3359;
                if (!(zgsz3363)) {






                  goto case_345;
                }
                uint64_t zgaz3101;
                zgaz3101 = sailgen_execution_mode_encdec_forwards_infallible(zmapping5z3);
                struct zoptionzIbzK zgsz3360;
                CREATE(zoptionzIbzK)(&zgsz3360);
                {
                  uint64_t zuz3115;
                  zuz3115 = zgaz3101;
                  uint64_t zgaz3104;
                  {
                    uint64_t zgaz3103;
                    {
                      uint64_t zgaz3102;
                      zgaz3102 = zuz3115;
                      zgaz3103 = (UINT64_C(0b01) << 1) | zgaz3102;
                    }
                    zgaz3104 = (UINT64_C(0b01) << 3) | zgaz3103;
                  }
                  {
                    RECREATE(lbits)(&zghz3113);
                    CONVERT_OF(lbits, fbits)(&zghz3113, zgaz3104, UINT64_C(5) , true);
                    sailgen_SomezIbzK(&zgsz3360, zghz3113);
                  }
                  goto finish_match_346;
                }
              case_348: ;
                {
                  sailgen_NonezIbzK(&zgsz3360, UNIT);
                  goto finish_match_346;
                }
              case_347: ;
              finish_match_346: ;
                COPY(zoptionzIbzK)(&zgaz3111, zgsz3360);
                KILL(zoptionzIbzK)(&zgsz3360);
                goto finish_match_343;
              }
            case_345: ;
              {
                sailgen_NonezIbzK(&zgaz3111, UNIT);
                goto finish_match_343;
              }
            case_344: ;
            finish_match_343: ;
            }
            uint64_t zgsz3364;
            {
              if (zgaz3111.kind != Kind_zSomezIbzK) goto case_395;
              zgsz3364 = CONVERT_OF(fbits, lbits)(zgaz3111.zSomezIbzK, true);
              goto finish_match_349;
            }
          case_395: ;
            {
              __label__ case_394, case_358, finish_match_357;

              /* complete */
              struct zoptionzIbzK zgaz3122;
              CREATE(zoptionzIbzK)(&zgaz3122);
              {
                __label__ case_353, case_352, finish_match_351;

                {
                  __label__ case_356, case_355, finish_match_354;

                  bool zuz3117;
                  zuz3117 = zhead_expz3.ztup0.zR;
                  bool zuz3118;
                  zuz3118 = zhead_expz3.ztup0.zW;
                  bool zuz3119;
                  zuz3119 = zhead_expz3.ztup0.zC;
                  bool zuz3120;
                  zuz3120 = zhead_expz3.ztup0.zLM;
                  bool zuz3121;
                  zuz3121 = zhead_expz3.ztup0.zX;
                  bool zuz3122;
                  zuz3122 = zhead_expz3.ztup0.zASR;
                  enum zExecutionMode zmapping6z3;
                  zmapping6z3 = zhead_expz3.ztup1;
                  bool zgaz3121;
                  {
                    bool zgaz3120;
                    {
                      bool zgaz3119;
                      {
                        bool zgaz3118;
                        {
                          bool zgaz3117;
                          {
                            bool zgaz3116;
                            zgaz3116 = sailgen_execution_mode_encdec_forwards_matches(zmapping6z3);
                            bool zgsz3367;
                            if (zgaz3116) {  zgsz3367 = eq_bool(zuz3122, false);  } else {  zgsz3367 = false;  }
                            zgaz3117 = zgsz3367;
                          }
                          bool zgsz3368;
                          if (zgaz3117) {  zgsz3368 = eq_bool(zuz3121, true);  } else {  zgsz3368 = false;  }
                          zgaz3118 = zgsz3368;
                        }
                        bool zgsz3369;
                        if (zgaz3118) {  zgsz3369 = eq_bool(zuz3120, true);  } else {  zgsz3369 = false;  }
                        zgaz3119 = zgsz3369;
                      }
                      bool zgsz3370;
                      if (zgaz3119) {  zgsz3370 = eq_bool(zuz3119, true);  } else {  zgsz3370 = false;  }
                      zgaz3120 = zgsz3370;
                    }
                    bool zgsz3371;
                    if (zgaz3120) {  zgsz3371 = eq_bool(zuz3118, true);  } else {  zgsz3371 = false;  }
                    zgaz3121 = zgsz3371;
                  }
                  bool zgsz3372;
                  if (zgaz3121) {  zgsz3372 = eq_bool(zuz3117, true);  } else {  zgsz3372 = false;  }
                  bool zgsz3376;
                  zgsz3376 = zgsz3372;
                  if (!(zgsz3376)) {






                    goto case_353;
                  }
                  uint64_t zgaz3112;
                  zgaz3112 = sailgen_execution_mode_encdec_forwards_infallible(zmapping6z3);
                  struct zoptionzIbzK zgsz3373;
                  CREATE(zoptionzIbzK)(&zgsz3373);
                  {
                    uint64_t zuz3123;
                    zuz3123 = zgaz3112;
                    uint64_t zgaz3115;
                    {
                      uint64_t zgaz3114;
                      {
                        uint64_t zgaz3113;
                        zgaz3113 = zuz3123;
                        zgaz3114 = (UINT64_C(0b10) << 1) | zgaz3113;
                      }
                      zgaz3115 = (UINT64_C(0b01) << 3) | zgaz3114;
                    }
                    {
                      RECREATE(lbits)(&zghz3112);
                      CONVERT_OF(lbits, fbits)(&zghz3112, zgaz3115, UINT64_C(5) , true);
                      sailgen_SomezIbzK(&zgsz3373, zghz3112);
                    }
                    goto finish_match_354;
                  }
                case_356: ;
                  {
                    sailgen_NonezIbzK(&zgsz3373, UNIT);
                    goto finish_match_354;
                  }
                case_355: ;
                finish_match_354: ;
                  COPY(zoptionzIbzK)(&zgaz3122, zgsz3373);
                  KILL(zoptionzIbzK)(&zgsz3373);
                  goto finish_match_351;
                }
              case_353: ;
                {
                  sailgen_NonezIbzK(&zgaz3122, UNIT);
                  goto finish_match_351;
                }
              case_352: ;
              finish_match_351: ;
              }
              uint64_t zgsz3377;
              {
                if (zgaz3122.kind != Kind_zSomezIbzK) goto case_394;
                zgsz3377 = CONVERT_OF(fbits, lbits)(zgaz3122.zSomezIbzK, true);
                goto finish_match_357;
              }
            case_394: ;
              {
                __label__ case_393, case_366, finish_match_365;

                /* complete */
                struct zoptionzIbzK zgaz3133;
                CREATE(zoptionzIbzK)(&zgaz3133);
                {
                  __label__ case_361, case_360, finish_match_359;

                  {
                    __label__ case_364, case_363, finish_match_362;

                    bool zuz3125;
                    zuz3125 = zhead_expz3.ztup0.zR;
                    bool zuz3126;
                    zuz3126 = zhead_expz3.ztup0.zW;
                    bool zuz3127;
                    zuz3127 = zhead_expz3.ztup0.zC;
                    bool zuz3128;
                    zuz3128 = zhead_expz3.ztup0.zLM;
                    bool zuz3129;
                    zuz3129 = zhead_expz3.ztup0.zX;
                    bool zuz3130;
                    zuz3130 = zhead_expz3.ztup0.zASR;
                    enum zExecutionMode zmapping7z3;
                    zmapping7z3 = zhead_expz3.ztup1;
                    bool zgaz3132;
                    {
                      bool zgaz3131;
                      {
                        bool zgaz3130;
                        {
                          bool zgaz3129;
                          {
                            bool zgaz3128;
                            {
                              bool zgaz3127;
                              zgaz3127 = sailgen_execution_mode_encdec_forwards_matches(zmapping7z3);
                              bool zgsz3380;
                              if (zgaz3127) {  zgsz3380 = eq_bool(zuz3130, false);  } else {  zgsz3380 = false;  }
                              zgaz3128 = zgsz3380;
                            }
                            bool zgsz3381;
                            if (zgaz3128) {  zgsz3381 = eq_bool(zuz3129, true);  } else {  zgsz3381 = false;  }
                            zgaz3129 = zgsz3381;
                          }
                          bool zgsz3382;
                          if (zgaz3129) {  zgsz3382 = eq_bool(zuz3128, false);  } else {  zgsz3382 = false;  }
                          zgaz3130 = zgsz3382;
                        }
                        bool zgsz3383;
                        if (zgaz3130) {  zgsz3383 = eq_bool(zuz3127, false);  } else {  zgsz3383 = false;  }
                        zgaz3131 = zgsz3383;
                      }
                      bool zgsz3384;
                      if (zgaz3131) {  zgsz3384 = eq_bool(zuz3126, true);  } else {  zgsz3384 = false;  }
                      zgaz3132 = zgsz3384;
                    }
                    bool zgsz3385;
                    if (zgaz3132) {  zgsz3385 = eq_bool(zuz3125, true);  } else {  zgsz3385 = false;  }
                    bool zgsz3389;
                    zgsz3389 = zgsz3385;
                    if (!(zgsz3389)) {






                      goto case_361;
                    }
                    uint64_t zgaz3123;
                    zgaz3123 = sailgen_execution_mode_encdec_forwards_infallible(zmapping7z3);
                    struct zoptionzIbzK zgsz3386;
                    CREATE(zoptionzIbzK)(&zgsz3386);
                    {
                      uint64_t zuz3131;
                      zuz3131 = zgaz3123;
                      uint64_t zgaz3126;
                      {
                        uint64_t zgaz3125;
                        {
                          uint64_t zgaz3124;
                          zgaz3124 = zuz3131;
                          zgaz3125 = (UINT64_C(0b11) << 1) | zgaz3124;
                        }
                        zgaz3126 = (UINT64_C(0b01) << 3) | zgaz3125;
                      }
                      {
                        RECREATE(lbits)(&zghz3111);
                        CONVERT_OF(lbits, fbits)(&zghz3111, zgaz3126, UINT64_C(5) , true);
                        sailgen_SomezIbzK(&zgsz3386, zghz3111);
                      }
                      goto finish_match_362;
                    }
                  case_364: ;
                    {
                      sailgen_NonezIbzK(&zgsz3386, UNIT);
                      goto finish_match_362;
                    }
                  case_363: ;
                  finish_match_362: ;
                    COPY(zoptionzIbzK)(&zgaz3133, zgsz3386);
                    KILL(zoptionzIbzK)(&zgsz3386);
                    goto finish_match_359;
                  }
                case_361: ;
                  {
                    sailgen_NonezIbzK(&zgaz3133, UNIT);
                    goto finish_match_359;
                  }
                case_360: ;
                finish_match_359: ;
                }
                uint64_t zgsz3390;
                {
                  if (zgaz3133.kind != Kind_zSomezIbzK) goto case_393;
                  zgsz3390 = CONVERT_OF(fbits, lbits)(zgaz3133.zSomezIbzK, true);
                  goto finish_match_365;
                }
              case_393: ;
                {
                  __label__ case_392, case_374, finish_match_373;

                  /* complete */
                  struct zoptionzIbzK zgaz3141;
                  CREATE(zoptionzIbzK)(&zgaz3141);
                  {
                    __label__ case_369, case_368, finish_match_367;

                    {
                      __label__ case_372, case_371, finish_match_370;

                      bool zuz3133;
                      zuz3133 = zhead_expz3.ztup0.zR;
                      bool zuz3134;
                      zuz3134 = zhead_expz3.ztup0.zW;
                      bool zuz3135;
                      zuz3135 = zhead_expz3.ztup0.zC;
                      bool zuz3136;
                      zuz3136 = zhead_expz3.ztup0.zLM;
                      bool zuz3137;
                      zuz3137 = zhead_expz3.ztup0.zX;
                      bool zuz3138;
                      zuz3138 = zhead_expz3.ztup0.zASR;
                      enum zExecutionMode zmapping8z3;
                      zmapping8z3 = zhead_expz3.ztup1;
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
                                zgaz3135 = sailgen_execution_mode_encdec_forwards_matches(zmapping8z3);
                                bool zgsz3393;
                                if (zgaz3135) {  zgsz3393 = eq_bool(zuz3138, false);  } else {  zgsz3393 = false;  }
                                zgaz3136 = zgsz3393;
                              }
                              bool zgsz3394;
                              if (zgaz3136) {  zgsz3394 = eq_bool(zuz3137, false);  } else {  zgsz3394 = false;  }
                              zgaz3137 = zgsz3394;
                            }
                            bool zgsz3395;
                            if (zgaz3137) {  zgsz3395 = eq_bool(zuz3136, false);  } else {  zgsz3395 = false;  }
                            zgaz3138 = zgsz3395;
                          }
                          bool zgsz3396;
                          if (zgaz3138) {  zgsz3396 = eq_bool(zuz3135, true);  } else {  zgsz3396 = false;  }
                          zgaz3139 = zgsz3396;
                        }
                        bool zgsz3397;
                        if (zgaz3139) {  zgsz3397 = eq_bool(zuz3134, false);  } else {  zgsz3397 = false;  }
                        zgaz3140 = zgsz3397;
                      }
                      bool zgsz3398;
                      if (zgaz3140) {  zgsz3398 = eq_bool(zuz3133, true);  } else {  zgsz3398 = false;  }
                      bool zgsz3402;
                      zgsz3402 = zgsz3398;
                      if (!(zgsz3402)) {






                        goto case_369;
                      }
                      uint64_t zgaz3134;
                      zgaz3134 = sailgen_execution_mode_encdec_forwards_infallible(zmapping8z3);
                      struct zoptionzIbzK zgsz3399;
                      CREATE(zoptionzIbzK)(&zgsz3399);
                      {
                        uint64_t zb__2;
                        zb__2 = zgaz3134;
                        bool zgsz3401;
                        zgsz3401 = (zb__2 == UINT64_C(0b0));
                        if (!(zgsz3401)) {

                          goto case_372;
                        }
                        {
                          RECREATE(lbits)(&zghz3110);
                          CONVERT_OF(lbits, fbits)(&zghz3110, UINT64_C(0b10011), UINT64_C(5) , true);
                          sailgen_SomezIbzK(&zgsz3399, zghz3110);
                        }
                        goto finish_match_370;
                      }
                    case_372: ;
                      {
                        sailgen_NonezIbzK(&zgsz3399, UNIT);
                        goto finish_match_370;
                      }
                    case_371: ;
                    finish_match_370: ;
                      COPY(zoptionzIbzK)(&zgaz3141, zgsz3399);
                      KILL(zoptionzIbzK)(&zgsz3399);
                      goto finish_match_367;
                    }
                  case_369: ;
                    {
                      sailgen_NonezIbzK(&zgaz3141, UNIT);
                      goto finish_match_367;
                    }
                  case_368: ;
                  finish_match_367: ;
                  }
                  uint64_t zgsz3403;
                  {
                    if (zgaz3141.kind != Kind_zSomezIbzK) goto case_392;
                    zgsz3403 = CONVERT_OF(fbits, lbits)(zgaz3141.zSomezIbzK, true);
                    goto finish_match_373;
                  }
                case_392: ;
                  {
                    __label__ case_391, case_382, finish_match_381;

                    /* complete */
                    struct zoptionzIbzK zgaz3149;
                    CREATE(zoptionzIbzK)(&zgaz3149);
                    {
                      __label__ case_377, case_376, finish_match_375;

                      {
                        __label__ case_380, case_379, finish_match_378;

                        bool zuz3140;
                        zuz3140 = zhead_expz3.ztup0.zR;
                        bool zuz3141;
                        zuz3141 = zhead_expz3.ztup0.zW;
                        bool zuz3142;
                        zuz3142 = zhead_expz3.ztup0.zC;
                        bool zuz3143;
                        zuz3143 = zhead_expz3.ztup0.zLM;
                        bool zuz3144;
                        zuz3144 = zhead_expz3.ztup0.zX;
                        bool zuz3145;
                        zuz3145 = zhead_expz3.ztup0.zASR;
                        enum zExecutionMode zmapping9z3;
                        zmapping9z3 = zhead_expz3.ztup1;
                        bool zgaz3148;
                        {
                          bool zgaz3147;
                          {
                            bool zgaz3146;
                            {
                              bool zgaz3145;
                              {
                                bool zgaz3144;
                                {
                                  bool zgaz3143;
                                  zgaz3143 = sailgen_execution_mode_encdec_forwards_matches(zmapping9z3);
                                  bool zgsz3406;
                                  if (zgaz3143) {  zgsz3406 = eq_bool(zuz3145, false);  } else {  zgsz3406 = false;  }
                                  zgaz3144 = zgsz3406;
                                }
                                bool zgsz3407;
                                if (zgaz3144) {  zgsz3407 = eq_bool(zuz3144, false);  } else {  zgsz3407 = false;  }
                                zgaz3145 = zgsz3407;
                              }
                              bool zgsz3408;
                              if (zgaz3145) {  zgsz3408 = eq_bool(zuz3143, true);  } else {  zgsz3408 = false;  }
                              zgaz3146 = zgsz3408;
                            }
                            bool zgsz3409;
                            if (zgaz3146) {  zgsz3409 = eq_bool(zuz3142, true);  } else {  zgsz3409 = false;  }
                            zgaz3147 = zgsz3409;
                          }
                          bool zgsz3410;
                          if (zgaz3147) {  zgsz3410 = eq_bool(zuz3141, false);  } else {  zgsz3410 = false;  }
                          zgaz3148 = zgsz3410;
                        }
                        bool zgsz3411;
                        if (zgaz3148) {  zgsz3411 = eq_bool(zuz3140, true);  } else {  zgsz3411 = false;  }
                        bool zgsz3415;
                        zgsz3415 = zgsz3411;
                        if (!(zgsz3415)) {






                          goto case_377;
                        }
                        uint64_t zgaz3142;
                        zgaz3142 = sailgen_execution_mode_encdec_forwards_infallible(zmapping9z3);
                        struct zoptionzIbzK zgsz3412;
                        CREATE(zoptionzIbzK)(&zgsz3412);
                        {
                          uint64_t zb__1;
                          zb__1 = zgaz3142;
                          bool zgsz3414;
                          zgsz3414 = (zb__1 == UINT64_C(0b0));
                          if (!(zgsz3414)) {

                            goto case_380;
                          }
                          {
                            RECREATE(lbits)(&zghz3109);
                            CONVERT_OF(lbits, fbits)(&zghz3109, UINT64_C(0b11011), UINT64_C(5) , true);
                            sailgen_SomezIbzK(&zgsz3412, zghz3109);
                          }
                          goto finish_match_378;
                        }
                      case_380: ;
                        {
                          sailgen_NonezIbzK(&zgsz3412, UNIT);
                          goto finish_match_378;
                        }
                      case_379: ;
                      finish_match_378: ;
                        COPY(zoptionzIbzK)(&zgaz3149, zgsz3412);
                        KILL(zoptionzIbzK)(&zgsz3412);
                        goto finish_match_375;
                      }
                    case_377: ;
                      {
                        sailgen_NonezIbzK(&zgaz3149, UNIT);
                        goto finish_match_375;
                      }
                    case_376: ;
                    finish_match_375: ;
                    }
                    uint64_t zgsz3416;
                    {
                      if (zgaz3149.kind != Kind_zSomezIbzK) goto case_391;
                      zgsz3416 = CONVERT_OF(fbits, lbits)(zgaz3149.zSomezIbzK, true);
                      goto finish_match_381;
                    }
                  case_391: ;
                    {
                      __label__ case_390, finish_match_389;

                      /* complete */
                      struct zoptionzIbzK zgaz3157;
                      CREATE(zoptionzIbzK)(&zgaz3157);
                      {
                        __label__ case_385, case_384, finish_match_383;

                        {
                          __label__ case_388, case_387, finish_match_386;

                          bool zuz3147;
                          zuz3147 = zhead_expz3.ztup0.zR;
                          bool zuz3148;
                          zuz3148 = zhead_expz3.ztup0.zW;
                          bool zuz3149;
                          zuz3149 = zhead_expz3.ztup0.zC;
                          bool zuz3150;
                          zuz3150 = zhead_expz3.ztup0.zLM;
                          bool zuz3151;
                          zuz3151 = zhead_expz3.ztup0.zX;
                          bool zuz3152;
                          zuz3152 = zhead_expz3.ztup0.zASR;
                          enum zExecutionMode zmapping10z3;
                          zmapping10z3 = zhead_expz3.ztup1;
                          bool zgaz3156;
                          {
                            bool zgaz3155;
                            {
                              bool zgaz3154;
                              {
                                bool zgaz3153;
                                {
                                  bool zgaz3152;
                                  {
                                    bool zgaz3151;
                                    zgaz3151 = sailgen_execution_mode_encdec_forwards_matches(zmapping10z3);
                                    bool zgsz3419;
                                    if (zgaz3151) {  zgsz3419 = eq_bool(zuz3152, false);  } else {  zgsz3419 = false;  }
                                    zgaz3152 = zgsz3419;
                                  }
                                  bool zgsz3420;
                                  if (zgaz3152) {  zgsz3420 = eq_bool(zuz3151, false);  } else {  zgsz3420 = false;  }
                                  zgaz3153 = zgsz3420;
                                }
                                bool zgsz3421;
                                if (zgaz3153) {  zgsz3421 = eq_bool(zuz3150, true);  } else {  zgsz3421 = false;  }
                                zgaz3154 = zgsz3421;
                              }
                              bool zgsz3422;
                              if (zgaz3154) {  zgsz3422 = eq_bool(zuz3149, true);  } else {  zgsz3422 = false;  }
                              zgaz3155 = zgsz3422;
                            }
                            bool zgsz3423;
                            if (zgaz3155) {  zgsz3423 = eq_bool(zuz3148, true);  } else {  zgsz3423 = false;  }
                            zgaz3156 = zgsz3423;
                          }
                          bool zgsz3424;
                          if (zgaz3156) {  zgsz3424 = eq_bool(zuz3147, true);  } else {  zgsz3424 = false;  }
                          bool zgsz3428;
                          zgsz3428 = zgsz3424;
                          if (!(zgsz3428)) {






                            goto case_385;
                          }
                          uint64_t zgaz3150;
                          zgaz3150 = sailgen_execution_mode_encdec_forwards_infallible(zmapping10z3);
                          struct zoptionzIbzK zgsz3425;
                          CREATE(zoptionzIbzK)(&zgsz3425);
                          {
                            uint64_t zb__0;
                            zb__0 = zgaz3150;
                            bool zgsz3427;
                            zgsz3427 = (zb__0 == UINT64_C(0b0));
                            if (!(zgsz3427)) {

                              goto case_388;
                            }
                            {
                              RECREATE(lbits)(&zghz3108);
                              CONVERT_OF(lbits, fbits)(&zghz3108, UINT64_C(0b11111), UINT64_C(5) , true);
                              sailgen_SomezIbzK(&zgsz3425, zghz3108);
                            }
                            goto finish_match_386;
                          }
                        case_388: ;
                          {
                            sailgen_NonezIbzK(&zgsz3425, UNIT);
                            goto finish_match_386;
                          }
                        case_387: ;
                        finish_match_386: ;
                          COPY(zoptionzIbzK)(&zgaz3157, zgsz3425);
                          KILL(zoptionzIbzK)(&zgsz3425);
                          goto finish_match_383;
                        }
                      case_385: ;
                        {
                          sailgen_NonezIbzK(&zgaz3157, UNIT);
                          goto finish_match_383;
                        }
                      case_384: ;
                      finish_match_383: ;
                      }
                      uint64_t zgsz3429;
                      {
                        if (zgaz3157.kind != Kind_zSomezIbzK) goto case_390;
                        zgsz3429 = CONVERT_OF(fbits, lbits)(zgaz3157.zSomezIbzK, true);
                        goto finish_match_389;
                      }
                    case_390: ;
                      sail_match_failure("ap_m_encdec_backwards_infallible");
                    finish_match_389: ;
                      zgsz3416 = zgsz3429;
                      KILL(zoptionzIbzK)(&zgaz3157);
                      goto finish_match_381;
                    }
                  case_382: ;
                  finish_match_381: ;
                    zgsz3403 = zgsz3416;
                    KILL(zoptionzIbzK)(&zgaz3149);
                    goto finish_match_373;
                  }
                case_374: ;
                finish_match_373: ;
                  zgsz3390 = zgsz3403;
                  KILL(zoptionzIbzK)(&zgaz3141);
                  goto finish_match_365;
                }
              case_366: ;
              finish_match_365: ;
                zgsz3377 = zgsz3390;
                KILL(zoptionzIbzK)(&zgaz3133);
                goto finish_match_357;
              }
            case_358: ;
            finish_match_357: ;
              zgsz3364 = zgsz3377;
              KILL(zoptionzIbzK)(&zgaz3122);
              goto finish_match_349;
            }
          case_350: ;
          finish_match_349: ;
            zgsz3351 = zgsz3364;
            KILL(zoptionzIbzK)(&zgaz3111);
            goto finish_match_341;
          }
        case_342: ;
        finish_match_341: ;
          zgsz3338 = zgsz3351;
          KILL(zoptionzIbzK)(&zgaz3100);
          goto finish_match_333;
        }
      case_334: ;
      finish_match_333: ;
        zgsz3325 = zgsz3338;
        KILL(zoptionzIbzK)(&zgaz389);
        goto finish_match_325;
      }
    case_326: ;
    finish_match_325: ;
      zgsz3312 = zgsz3325;
      KILL(zoptionzIbzK)(&zgaz381);
      goto finish_match_317;
    }
  case_318: ;
  finish_match_317: ;
    zcbz348 = zgsz3312;
    KILL(zoptionzIbzK)(&zgaz373);
    goto finish_match_309;
  }
case_310: ;
finish_match_309: ;
  KILL(zoptionzIbzK)(&zgaz365);

end_function_401: ;
  return zcbz348;
end_block_exception_402: ;

  return UINT64_C(0xdeadc0de);
}



static void finish_sailgen_ap_m_encdec_backwards_infallible(void)
{
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
  KILL(lbits)(&zghz3108);
}

static bool sailgen_ap_m_encdec_forwards_matches(uint64_t zargz3)
{
  __label__ case_416, case_415, case_414, case_413, case_412, case_411, case_410, case_409, case_408, case_407, case_406, case_405, finish_match_404, end_function_417, end_block_exception_418;

  bool zcbz349;
  {
    uint64_t zv__33;
    zv__33 = zargz3;
    bool zgsz3463;
    zgsz3463 = (zv__33 == UINT64_C(0b00000));
    if (!(zgsz3463)) {

      goto case_416;
    }
    zcbz349 = true;
    goto finish_match_404;
  }
case_416: ;
  {
    uint64_t zv__36;
    zv__36 = zargz3;
    bool zgsz3462;
    zgsz3462 = (zv__36 == UINT64_C(0b00001));
    if (!(zgsz3462)) {

      goto case_415;
    }
    zcbz349 = true;
    goto finish_match_404;
  }
case_415: ;
  {
    uint64_t zv__39;
    zv__39 = zargz3;
    bool zgsz3461;
    zgsz3461 = (zv__39 == UINT64_C(0b00100));
    if (!(zgsz3461)) {

      goto case_414;
    }
    zcbz349 = true;
    goto finish_match_404;
  }
case_414: ;
  {
    uint64_t zv__42;
    zv__42 = zargz3;
    bool zgsz3460;
    zgsz3460 = (zv__42 == UINT64_C(0b00101));
    if (!(zgsz3460)) {

      goto case_413;
    }
    zcbz349 = true;
    goto finish_match_404;
  }
case_413: ;
  {
    uint64_t zv__45;
    zv__45 = zargz3;
    uint64_t zgaz3158;
    zgaz3158 = (safe_rshift(UINT64_MAX, 64 - 4) & (zv__45 >> INT64_C(1)));
    bool zgsz3459;
    zgsz3459 = (zgaz3158 == UINT64_C(0x4));
    if (!(zgsz3459)) {

      goto case_412;
    }
    zcbz349 = true;
    goto finish_match_404;
  }
case_412: ;
  {
    uint64_t zv__48;
    zv__48 = zargz3;
    uint64_t zgaz3159;
    zgaz3159 = (safe_rshift(UINT64_MAX, 64 - 4) & (zv__48 >> INT64_C(1)));
    bool zgsz3458;
    zgsz3458 = (zgaz3159 == UINT64_C(0x5));
    if (!(zgsz3458)) {

      goto case_411;
    }
    zcbz349 = true;
    goto finish_match_404;
  }
case_411: ;
  {
    uint64_t zv__51;
    zv__51 = zargz3;
    uint64_t zgaz3160;
    zgaz3160 = (safe_rshift(UINT64_MAX, 64 - 4) & (zv__51 >> INT64_C(1)));
    bool zgsz3457;
    zgsz3457 = (zgaz3160 == UINT64_C(0x6));
    if (!(zgsz3457)) {

      goto case_410;
    }
    zcbz349 = true;
    goto finish_match_404;
  }
case_410: ;
  {
    uint64_t zv__54;
    zv__54 = zargz3;
    uint64_t zgaz3161;
    zgaz3161 = (safe_rshift(UINT64_MAX, 64 - 4) & (zv__54 >> INT64_C(1)));
    bool zgsz3456;
    zgsz3456 = (zgaz3161 == UINT64_C(0x7));
    if (!(zgsz3456)) {

      goto case_409;
    }
    zcbz349 = true;
    goto finish_match_404;
  }
case_409: ;
  {
    uint64_t zv__57;
    zv__57 = zargz3;
    bool zgsz3455;
    zgsz3455 = (zv__57 == UINT64_C(0b10011));
    if (!(zgsz3455)) {

      goto case_408;
    }
    zcbz349 = true;
    goto finish_match_404;
  }
case_408: ;
  {
    uint64_t zv__60;
    zv__60 = zargz3;
    bool zgsz3454;
    zgsz3454 = (zv__60 == UINT64_C(0b11011));
    if (!(zgsz3454)) {

      goto case_407;
    }
    zcbz349 = true;
    goto finish_match_404;
  }
case_407: ;
  {
    uint64_t zv__63;
    zv__63 = zargz3;
    bool zgsz3453;
    zgsz3453 = (zv__63 == UINT64_C(0b11111));
    if (!(zgsz3453)) {

      goto case_406;
    }
    zcbz349 = true;
    goto finish_match_404;
  }
case_406: ;
  {
    zcbz349 = false;
    goto finish_match_404;
  }
case_405: ;
finish_match_404: ;
end_function_417: ;
  return zcbz349;
end_block_exception_418: ;

  return false;
}

static bool sailgen_ap_m_encdec_backwards_matches(struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zargz3)
{
  __label__ case_520, case_427, finish_match_426, end_function_521, end_block_exception_522;

  bool zcbz350;
  struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zhead_expz3;
  zhead_expz3 = zargz3;
  struct zoptionzIozK zgaz3169;
  CREATE(zoptionzIozK)(&zgaz3169);
  {
    __label__ case_422, case_421, finish_match_420;

    {
      __label__ case_425, case_424, finish_match_423;

      bool zp0z3;
      zp0z3 = zhead_expz3.ztup0.zR;
      bool zp1z3;
      zp1z3 = zhead_expz3.ztup0.zW;
      bool zp2z3;
      zp2z3 = zhead_expz3.ztup0.zC;
      bool zp3z3;
      zp3z3 = zhead_expz3.ztup0.zLM;
      bool zp4z3;
      zp4z3 = zhead_expz3.ztup0.zX;
      bool zp5z3;
      zp5z3 = zhead_expz3.ztup0.zASR;
      enum zExecutionMode zmapping0z3;
      zmapping0z3 = zhead_expz3.ztup1;
      bool zgaz3168;
      {
        bool zgaz3167;
        {
          bool zgaz3166;
          {
            bool zgaz3165;
            {
              bool zgaz3164;
              {
                bool zgaz3163;
                zgaz3163 = sailgen_execution_mode_encdec_forwards_matches(zmapping0z3);
                bool zgsz3466;
                if (zgaz3163) {  zgsz3466 = eq_bool(zp5z3, false);  } else {  zgsz3466 = false;  }
                zgaz3164 = zgsz3466;
              }
              bool zgsz3467;
              if (zgaz3164) {  zgsz3467 = eq_bool(zp4z3, false);  } else {  zgsz3467 = false;  }
              zgaz3165 = zgsz3467;
            }
            bool zgsz3468;
            if (zgaz3165) {  zgsz3468 = eq_bool(zp3z3, false);  } else {  zgsz3468 = false;  }
            zgaz3166 = zgsz3468;
          }
          bool zgsz3469;
          if (zgaz3166) {  zgsz3469 = eq_bool(zp2z3, false);  } else {  zgsz3469 = false;  }
          zgaz3167 = zgsz3469;
        }
        bool zgsz3470;
        if (zgaz3167) {  zgsz3470 = eq_bool(zp1z3, false);  } else {  zgsz3470 = false;  }
        zgaz3168 = zgsz3470;
      }
      bool zgsz3471;
      if (zgaz3168) {  zgsz3471 = eq_bool(zp0z3, false);  } else {  zgsz3471 = false;  }
      bool zgsz3475;
      zgsz3475 = zgsz3471;
      if (!(zgsz3475)) {






        goto case_422;
      }
      uint64_t zgaz3162;
      zgaz3162 = sailgen_execution_mode_encdec_forwards_infallible(zmapping0z3);
      struct zoptionzIozK zgsz3472;
      CREATE(zoptionzIozK)(&zgsz3472);
      {
        uint64_t zb__6;
        zb__6 = zgaz3162;
        bool zgsz3474;
        zgsz3474 = (zb__6 == UINT64_C(0b0));
        if (!(zgsz3474)) {

          goto case_425;
        }
        sailgen_SomezIozK(&zgsz3472, true);
        goto finish_match_423;
      }
    case_425: ;
      {
        sailgen_NonezIozK(&zgsz3472, UNIT);
        goto finish_match_423;
      }
    case_424: ;
    finish_match_423: ;
      COPY(zoptionzIozK)(&zgaz3169, zgsz3472);
      KILL(zoptionzIozK)(&zgsz3472);
      goto finish_match_420;
    }
  case_422: ;
    {
      sailgen_NonezIozK(&zgaz3169, UNIT);
      goto finish_match_420;
    }
  case_421: ;
  finish_match_420: ;
  }
  bool zgsz3476;
  {
    if (zgaz3169.kind != Kind_zSomezIozK) goto case_520;
    zgsz3476 = zgaz3169.zSomezIozK;
    goto finish_match_426;
  }
case_520: ;
  {
    __label__ case_519, case_435, finish_match_434;

    /* complete */
    struct zoptionzIozK zgaz3177;
    CREATE(zoptionzIozK)(&zgaz3177);
    {
      __label__ case_430, case_429, finish_match_428;

      {
        __label__ case_433, case_432, finish_match_431;

        bool zuz3154;
        zuz3154 = zhead_expz3.ztup0.zR;
        bool zuz3155;
        zuz3155 = zhead_expz3.ztup0.zW;
        bool zuz3156;
        zuz3156 = zhead_expz3.ztup0.zC;
        bool zuz3157;
        zuz3157 = zhead_expz3.ztup0.zLM;
        bool zuz3158;
        zuz3158 = zhead_expz3.ztup0.zX;
        bool zuz3159;
        zuz3159 = zhead_expz3.ztup0.zASR;
        enum zExecutionMode zmapping1z3;
        zmapping1z3 = zhead_expz3.ztup1;
        bool zgaz3176;
        {
          bool zgaz3175;
          {
            bool zgaz3174;
            {
              bool zgaz3173;
              {
                bool zgaz3172;
                {
                  bool zgaz3171;
                  zgaz3171 = sailgen_execution_mode_encdec_forwards_matches(zmapping1z3);
                  bool zgsz3479;
                  if (zgaz3171) {  zgsz3479 = eq_bool(zuz3159, false);  } else {  zgsz3479 = false;  }
                  zgaz3172 = zgsz3479;
                }
                bool zgsz3480;
                if (zgaz3172) {  zgsz3480 = eq_bool(zuz3158, false);  } else {  zgsz3480 = false;  }
                zgaz3173 = zgsz3480;
              }
              bool zgsz3481;
              if (zgaz3173) {  zgsz3481 = eq_bool(zuz3157, false);  } else {  zgsz3481 = false;  }
              zgaz3174 = zgsz3481;
            }
            bool zgsz3482;
            if (zgaz3174) {  zgsz3482 = eq_bool(zuz3156, false);  } else {  zgsz3482 = false;  }
            zgaz3175 = zgsz3482;
          }
          bool zgsz3483;
          if (zgaz3175) {  zgsz3483 = eq_bool(zuz3155, false);  } else {  zgsz3483 = false;  }
          zgaz3176 = zgsz3483;
        }
        bool zgsz3484;
        if (zgaz3176) {  zgsz3484 = eq_bool(zuz3154, true);  } else {  zgsz3484 = false;  }
        bool zgsz3488;
        zgsz3488 = zgsz3484;
        if (!(zgsz3488)) {






          goto case_430;
        }
        uint64_t zgaz3170;
        zgaz3170 = sailgen_execution_mode_encdec_forwards_infallible(zmapping1z3);
        struct zoptionzIozK zgsz3485;
        CREATE(zoptionzIozK)(&zgsz3485);
        {
          uint64_t zb__5;
          zb__5 = zgaz3170;
          bool zgsz3487;
          zgsz3487 = (zb__5 == UINT64_C(0b0));
          if (!(zgsz3487)) {

            goto case_433;
          }
          sailgen_SomezIozK(&zgsz3485, true);
          goto finish_match_431;
        }
      case_433: ;
        {
          sailgen_NonezIozK(&zgsz3485, UNIT);
          goto finish_match_431;
        }
      case_432: ;
      finish_match_431: ;
        COPY(zoptionzIozK)(&zgaz3177, zgsz3485);
        KILL(zoptionzIozK)(&zgsz3485);
        goto finish_match_428;
      }
    case_430: ;
      {
        sailgen_NonezIozK(&zgaz3177, UNIT);
        goto finish_match_428;
      }
    case_429: ;
    finish_match_428: ;
    }
    bool zgsz3489;
    {
      if (zgaz3177.kind != Kind_zSomezIozK) goto case_519;
      zgsz3489 = zgaz3177.zSomezIozK;
      goto finish_match_434;
    }
  case_519: ;
    {
      __label__ case_518, case_443, finish_match_442;

      /* complete */
      struct zoptionzIozK zgaz3185;
      CREATE(zoptionzIozK)(&zgaz3185);
      {
        __label__ case_438, case_437, finish_match_436;

        {
          __label__ case_441, case_440, finish_match_439;

          bool zuz3161;
          zuz3161 = zhead_expz3.ztup0.zR;
          bool zuz3162;
          zuz3162 = zhead_expz3.ztup0.zW;
          bool zuz3163;
          zuz3163 = zhead_expz3.ztup0.zC;
          bool zuz3164;
          zuz3164 = zhead_expz3.ztup0.zLM;
          bool zuz3165;
          zuz3165 = zhead_expz3.ztup0.zX;
          bool zuz3166;
          zuz3166 = zhead_expz3.ztup0.zASR;
          enum zExecutionMode zmapping2z3;
          zmapping2z3 = zhead_expz3.ztup1;
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
                    zgaz3179 = sailgen_execution_mode_encdec_forwards_matches(zmapping2z3);
                    bool zgsz3492;
                    if (zgaz3179) {  zgsz3492 = eq_bool(zuz3166, false);  } else {  zgsz3492 = false;  }
                    zgaz3180 = zgsz3492;
                  }
                  bool zgsz3493;
                  if (zgaz3180) {  zgsz3493 = eq_bool(zuz3165, false);  } else {  zgsz3493 = false;  }
                  zgaz3181 = zgsz3493;
                }
                bool zgsz3494;
                if (zgaz3181) {  zgsz3494 = eq_bool(zuz3164, false);  } else {  zgsz3494 = false;  }
                zgaz3182 = zgsz3494;
              }
              bool zgsz3495;
              if (zgaz3182) {  zgsz3495 = eq_bool(zuz3163, false);  } else {  zgsz3495 = false;  }
              zgaz3183 = zgsz3495;
            }
            bool zgsz3496;
            if (zgaz3183) {  zgsz3496 = eq_bool(zuz3162, true);  } else {  zgsz3496 = false;  }
            zgaz3184 = zgsz3496;
          }
          bool zgsz3497;
          if (zgaz3184) {  zgsz3497 = eq_bool(zuz3161, false);  } else {  zgsz3497 = false;  }
          bool zgsz3501;
          zgsz3501 = zgsz3497;
          if (!(zgsz3501)) {






            goto case_438;
          }
          uint64_t zgaz3178;
          zgaz3178 = sailgen_execution_mode_encdec_forwards_infallible(zmapping2z3);
          struct zoptionzIozK zgsz3498;
          CREATE(zoptionzIozK)(&zgsz3498);
          {
            uint64_t zb__4;
            zb__4 = zgaz3178;
            bool zgsz3500;
            zgsz3500 = (zb__4 == UINT64_C(0b0));
            if (!(zgsz3500)) {

              goto case_441;
            }
            sailgen_SomezIozK(&zgsz3498, true);
            goto finish_match_439;
          }
        case_441: ;
          {
            sailgen_NonezIozK(&zgsz3498, UNIT);
            goto finish_match_439;
          }
        case_440: ;
        finish_match_439: ;
          COPY(zoptionzIozK)(&zgaz3185, zgsz3498);
          KILL(zoptionzIozK)(&zgsz3498);
          goto finish_match_436;
        }
      case_438: ;
        {
          sailgen_NonezIozK(&zgaz3185, UNIT);
          goto finish_match_436;
        }
      case_437: ;
      finish_match_436: ;
      }
      bool zgsz3502;
      {
        if (zgaz3185.kind != Kind_zSomezIozK) goto case_518;
        zgsz3502 = zgaz3185.zSomezIozK;
        goto finish_match_442;
      }
    case_518: ;
      {
        __label__ case_517, case_451, finish_match_450;

        /* complete */
        struct zoptionzIozK zgaz3193;
        CREATE(zoptionzIozK)(&zgaz3193);
        {
          __label__ case_446, case_445, finish_match_444;

          {
            __label__ case_449, case_448, finish_match_447;

            bool zuz3168;
            zuz3168 = zhead_expz3.ztup0.zR;
            bool zuz3169;
            zuz3169 = zhead_expz3.ztup0.zW;
            bool zuz3170;
            zuz3170 = zhead_expz3.ztup0.zC;
            bool zuz3171;
            zuz3171 = zhead_expz3.ztup0.zLM;
            bool zuz3172;
            zuz3172 = zhead_expz3.ztup0.zX;
            bool zuz3173;
            zuz3173 = zhead_expz3.ztup0.zASR;
            enum zExecutionMode zmapping3z3;
            zmapping3z3 = zhead_expz3.ztup1;
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
                      zgaz3187 = sailgen_execution_mode_encdec_forwards_matches(zmapping3z3);
                      bool zgsz3505;
                      if (zgaz3187) {  zgsz3505 = eq_bool(zuz3173, false);  } else {  zgsz3505 = false;  }
                      zgaz3188 = zgsz3505;
                    }
                    bool zgsz3506;
                    if (zgaz3188) {  zgsz3506 = eq_bool(zuz3172, false);  } else {  zgsz3506 = false;  }
                    zgaz3189 = zgsz3506;
                  }
                  bool zgsz3507;
                  if (zgaz3189) {  zgsz3507 = eq_bool(zuz3171, false);  } else {  zgsz3507 = false;  }
                  zgaz3190 = zgsz3507;
                }
                bool zgsz3508;
                if (zgaz3190) {  zgsz3508 = eq_bool(zuz3170, false);  } else {  zgsz3508 = false;  }
                zgaz3191 = zgsz3508;
              }
              bool zgsz3509;
              if (zgaz3191) {  zgsz3509 = eq_bool(zuz3169, true);  } else {  zgsz3509 = false;  }
              zgaz3192 = zgsz3509;
            }
            bool zgsz3510;
            if (zgaz3192) {  zgsz3510 = eq_bool(zuz3168, true);  } else {  zgsz3510 = false;  }
            bool zgsz3514;
            zgsz3514 = zgsz3510;
            if (!(zgsz3514)) {






              goto case_446;
            }
            uint64_t zgaz3186;
            zgaz3186 = sailgen_execution_mode_encdec_forwards_infallible(zmapping3z3);
            struct zoptionzIozK zgsz3511;
            CREATE(zoptionzIozK)(&zgsz3511);
            {
              uint64_t zb__3;
              zb__3 = zgaz3186;
              bool zgsz3513;
              zgsz3513 = (zb__3 == UINT64_C(0b0));
              if (!(zgsz3513)) {

                goto case_449;
              }
              sailgen_SomezIozK(&zgsz3511, true);
              goto finish_match_447;
            }
          case_449: ;
            {
              sailgen_NonezIozK(&zgsz3511, UNIT);
              goto finish_match_447;
            }
          case_448: ;
          finish_match_447: ;
            COPY(zoptionzIozK)(&zgaz3193, zgsz3511);
            KILL(zoptionzIozK)(&zgsz3511);
            goto finish_match_444;
          }
        case_446: ;
          {
            sailgen_NonezIozK(&zgaz3193, UNIT);
            goto finish_match_444;
          }
        case_445: ;
        finish_match_444: ;
        }
        bool zgsz3515;
        {
          if (zgaz3193.kind != Kind_zSomezIozK) goto case_517;
          zgsz3515 = zgaz3193.zSomezIozK;
          goto finish_match_450;
        }
      case_517: ;
        {
          __label__ case_516, case_459, finish_match_458;

          /* complete */
          struct zoptionzIozK zgaz3201;
          CREATE(zoptionzIozK)(&zgaz3201);
          {
            __label__ case_454, case_453, finish_match_452;

            {
              __label__ case_457, case_456, finish_match_455;

              bool zuz3175;
              zuz3175 = zhead_expz3.ztup0.zR;
              bool zuz3176;
              zuz3176 = zhead_expz3.ztup0.zW;
              bool zuz3177;
              zuz3177 = zhead_expz3.ztup0.zC;
              bool zuz3178;
              zuz3178 = zhead_expz3.ztup0.zLM;
              bool zuz3179;
              zuz3179 = zhead_expz3.ztup0.zX;
              bool zuz3180;
              zuz3180 = zhead_expz3.ztup0.zASR;
              enum zExecutionMode zmapping4z3;
              zmapping4z3 = zhead_expz3.ztup1;
              bool zgaz3200;
              {
                bool zgaz3199;
                {
                  bool zgaz3198;
                  {
                    bool zgaz3197;
                    {
                      bool zgaz3196;
                      {
                        bool zgaz3195;
                        zgaz3195 = sailgen_execution_mode_encdec_forwards_matches(zmapping4z3);
                        bool zgsz3518;
                        if (zgaz3195) {  zgsz3518 = eq_bool(zuz3180, true);  } else {  zgsz3518 = false;  }
                        zgaz3196 = zgsz3518;
                      }
                      bool zgsz3519;
                      if (zgaz3196) {  zgsz3519 = eq_bool(zuz3179, true);  } else {  zgsz3519 = false;  }
                      zgaz3197 = zgsz3519;
                    }
                    bool zgsz3520;
                    if (zgaz3197) {  zgsz3520 = eq_bool(zuz3178, true);  } else {  zgsz3520 = false;  }
                    zgaz3198 = zgsz3520;
                  }
                  bool zgsz3521;
                  if (zgaz3198) {  zgsz3521 = eq_bool(zuz3177, true);  } else {  zgsz3521 = false;  }
                  zgaz3199 = zgsz3521;
                }
                bool zgsz3522;
                if (zgaz3199) {  zgsz3522 = eq_bool(zuz3176, true);  } else {  zgsz3522 = false;  }
                zgaz3200 = zgsz3522;
              }
              bool zgsz3523;
              if (zgaz3200) {  zgsz3523 = eq_bool(zuz3175, true);  } else {  zgsz3523 = false;  }
              bool zgsz3527;
              zgsz3527 = zgsz3523;
              if (!(zgsz3527)) {






                goto case_454;
              }
              uint64_t zgaz3194;
              zgaz3194 = sailgen_execution_mode_encdec_forwards_infallible(zmapping4z3);
              struct zoptionzIozK zgsz3524;
              CREATE(zoptionzIozK)(&zgsz3524);
              {
                uint64_t zm;
                zm = zgaz3194;
                sailgen_SomezIozK(&zgsz3524, true);
                goto finish_match_455;
              }
            case_457: ;
              {
                sailgen_NonezIozK(&zgsz3524, UNIT);
                goto finish_match_455;
              }
            case_456: ;
            finish_match_455: ;
              COPY(zoptionzIozK)(&zgaz3201, zgsz3524);
              KILL(zoptionzIozK)(&zgsz3524);
              goto finish_match_452;
            }
          case_454: ;
            {
              sailgen_NonezIozK(&zgaz3201, UNIT);
              goto finish_match_452;
            }
          case_453: ;
          finish_match_452: ;
          }
          bool zgsz3528;
          {
            if (zgaz3201.kind != Kind_zSomezIozK) goto case_516;
            zgsz3528 = zgaz3201.zSomezIozK;
            goto finish_match_458;
          }
        case_516: ;
          {
            __label__ case_515, case_467, finish_match_466;

            /* complete */
            struct zoptionzIozK zgaz3209;
            CREATE(zoptionzIozK)(&zgaz3209);
            {
              __label__ case_462, case_461, finish_match_460;

              {
                __label__ case_465, case_464, finish_match_463;

                bool zuz3182;
                zuz3182 = zhead_expz3.ztup0.zR;
                bool zuz3183;
                zuz3183 = zhead_expz3.ztup0.zW;
                bool zuz3184;
                zuz3184 = zhead_expz3.ztup0.zC;
                bool zuz3185;
                zuz3185 = zhead_expz3.ztup0.zLM;
                bool zuz3186;
                zuz3186 = zhead_expz3.ztup0.zX;
                bool zuz3187;
                zuz3187 = zhead_expz3.ztup0.zASR;
                enum zExecutionMode zmapping5z3;
                zmapping5z3 = zhead_expz3.ztup1;
                bool zgaz3208;
                {
                  bool zgaz3207;
                  {
                    bool zgaz3206;
                    {
                      bool zgaz3205;
                      {
                        bool zgaz3204;
                        {
                          bool zgaz3203;
                          zgaz3203 = sailgen_execution_mode_encdec_forwards_matches(zmapping5z3);
                          bool zgsz3531;
                          if (zgaz3203) {  zgsz3531 = eq_bool(zuz3187, false);  } else {  zgsz3531 = false;  }
                          zgaz3204 = zgsz3531;
                        }
                        bool zgsz3532;
                        if (zgaz3204) {  zgsz3532 = eq_bool(zuz3186, true);  } else {  zgsz3532 = false;  }
                        zgaz3205 = zgsz3532;
                      }
                      bool zgsz3533;
                      if (zgaz3205) {  zgsz3533 = eq_bool(zuz3185, true);  } else {  zgsz3533 = false;  }
                      zgaz3206 = zgsz3533;
                    }
                    bool zgsz3534;
                    if (zgaz3206) {  zgsz3534 = eq_bool(zuz3184, true);  } else {  zgsz3534 = false;  }
                    zgaz3207 = zgsz3534;
                  }
                  bool zgsz3535;
                  if (zgaz3207) {  zgsz3535 = eq_bool(zuz3183, false);  } else {  zgsz3535 = false;  }
                  zgaz3208 = zgsz3535;
                }
                bool zgsz3536;
                if (zgaz3208) {  zgsz3536 = eq_bool(zuz3182, true);  } else {  zgsz3536 = false;  }
                bool zgsz3540;
                zgsz3540 = zgsz3536;
                if (!(zgsz3540)) {






                  goto case_462;
                }
                uint64_t zgaz3202;
                zgaz3202 = sailgen_execution_mode_encdec_forwards_infallible(zmapping5z3);
                struct zoptionzIozK zgsz3537;
                CREATE(zoptionzIozK)(&zgsz3537);
                {
                  uint64_t zuz3188;
                  zuz3188 = zgaz3202;
                  sailgen_SomezIozK(&zgsz3537, true);
                  goto finish_match_463;
                }
              case_465: ;
                {
                  sailgen_NonezIozK(&zgsz3537, UNIT);
                  goto finish_match_463;
                }
              case_464: ;
              finish_match_463: ;
                COPY(zoptionzIozK)(&zgaz3209, zgsz3537);
                KILL(zoptionzIozK)(&zgsz3537);
                goto finish_match_460;
              }
            case_462: ;
              {
                sailgen_NonezIozK(&zgaz3209, UNIT);
                goto finish_match_460;
              }
            case_461: ;
            finish_match_460: ;
            }
            bool zgsz3541;
            {
              if (zgaz3209.kind != Kind_zSomezIozK) goto case_515;
              zgsz3541 = zgaz3209.zSomezIozK;
              goto finish_match_466;
            }
          case_515: ;
            {
              __label__ case_514, case_475, finish_match_474;

              /* complete */
              struct zoptionzIozK zgaz3217;
              CREATE(zoptionzIozK)(&zgaz3217);
              {
                __label__ case_470, case_469, finish_match_468;

                {
                  __label__ case_473, case_472, finish_match_471;

                  bool zuz3190;
                  zuz3190 = zhead_expz3.ztup0.zR;
                  bool zuz3191;
                  zuz3191 = zhead_expz3.ztup0.zW;
                  bool zuz3192;
                  zuz3192 = zhead_expz3.ztup0.zC;
                  bool zuz3193;
                  zuz3193 = zhead_expz3.ztup0.zLM;
                  bool zuz3194;
                  zuz3194 = zhead_expz3.ztup0.zX;
                  bool zuz3195;
                  zuz3195 = zhead_expz3.ztup0.zASR;
                  enum zExecutionMode zmapping6z3;
                  zmapping6z3 = zhead_expz3.ztup1;
                  bool zgaz3216;
                  {
                    bool zgaz3215;
                    {
                      bool zgaz3214;
                      {
                        bool zgaz3213;
                        {
                          bool zgaz3212;
                          {
                            bool zgaz3211;
                            zgaz3211 = sailgen_execution_mode_encdec_forwards_matches(zmapping6z3);
                            bool zgsz3544;
                            if (zgaz3211) {  zgsz3544 = eq_bool(zuz3195, false);  } else {  zgsz3544 = false;  }
                            zgaz3212 = zgsz3544;
                          }
                          bool zgsz3545;
                          if (zgaz3212) {  zgsz3545 = eq_bool(zuz3194, true);  } else {  zgsz3545 = false;  }
                          zgaz3213 = zgsz3545;
                        }
                        bool zgsz3546;
                        if (zgaz3213) {  zgsz3546 = eq_bool(zuz3193, true);  } else {  zgsz3546 = false;  }
                        zgaz3214 = zgsz3546;
                      }
                      bool zgsz3547;
                      if (zgaz3214) {  zgsz3547 = eq_bool(zuz3192, true);  } else {  zgsz3547 = false;  }
                      zgaz3215 = zgsz3547;
                    }
                    bool zgsz3548;
                    if (zgaz3215) {  zgsz3548 = eq_bool(zuz3191, true);  } else {  zgsz3548 = false;  }
                    zgaz3216 = zgsz3548;
                  }
                  bool zgsz3549;
                  if (zgaz3216) {  zgsz3549 = eq_bool(zuz3190, true);  } else {  zgsz3549 = false;  }
                  bool zgsz3553;
                  zgsz3553 = zgsz3549;
                  if (!(zgsz3553)) {






                    goto case_470;
                  }
                  uint64_t zgaz3210;
                  zgaz3210 = sailgen_execution_mode_encdec_forwards_infallible(zmapping6z3);
                  struct zoptionzIozK zgsz3550;
                  CREATE(zoptionzIozK)(&zgsz3550);
                  {
                    uint64_t zuz3196;
                    zuz3196 = zgaz3210;
                    sailgen_SomezIozK(&zgsz3550, true);
                    goto finish_match_471;
                  }
                case_473: ;
                  {
                    sailgen_NonezIozK(&zgsz3550, UNIT);
                    goto finish_match_471;
                  }
                case_472: ;
                finish_match_471: ;
                  COPY(zoptionzIozK)(&zgaz3217, zgsz3550);
                  KILL(zoptionzIozK)(&zgsz3550);
                  goto finish_match_468;
                }
              case_470: ;
                {
                  sailgen_NonezIozK(&zgaz3217, UNIT);
                  goto finish_match_468;
                }
              case_469: ;
              finish_match_468: ;
              }
              bool zgsz3554;
              {
                if (zgaz3217.kind != Kind_zSomezIozK) goto case_514;
                zgsz3554 = zgaz3217.zSomezIozK;
                goto finish_match_474;
              }
            case_514: ;
              {
                __label__ case_513, case_483, finish_match_482;

                /* complete */
                struct zoptionzIozK zgaz3225;
                CREATE(zoptionzIozK)(&zgaz3225);
                {
                  __label__ case_478, case_477, finish_match_476;

                  {
                    __label__ case_481, case_480, finish_match_479;

                    bool zuz3198;
                    zuz3198 = zhead_expz3.ztup0.zR;
                    bool zuz3199;
                    zuz3199 = zhead_expz3.ztup0.zW;
                    bool zuz3200;
                    zuz3200 = zhead_expz3.ztup0.zC;
                    bool zuz3201;
                    zuz3201 = zhead_expz3.ztup0.zLM;
                    bool zuz3202;
                    zuz3202 = zhead_expz3.ztup0.zX;
                    bool zuz3203;
                    zuz3203 = zhead_expz3.ztup0.zASR;
                    enum zExecutionMode zmapping7z3;
                    zmapping7z3 = zhead_expz3.ztup1;
                    bool zgaz3224;
                    {
                      bool zgaz3223;
                      {
                        bool zgaz3222;
                        {
                          bool zgaz3221;
                          {
                            bool zgaz3220;
                            {
                              bool zgaz3219;
                              zgaz3219 = sailgen_execution_mode_encdec_forwards_matches(zmapping7z3);
                              bool zgsz3557;
                              if (zgaz3219) {  zgsz3557 = eq_bool(zuz3203, false);  } else {  zgsz3557 = false;  }
                              zgaz3220 = zgsz3557;
                            }
                            bool zgsz3558;
                            if (zgaz3220) {  zgsz3558 = eq_bool(zuz3202, true);  } else {  zgsz3558 = false;  }
                            zgaz3221 = zgsz3558;
                          }
                          bool zgsz3559;
                          if (zgaz3221) {  zgsz3559 = eq_bool(zuz3201, false);  } else {  zgsz3559 = false;  }
                          zgaz3222 = zgsz3559;
                        }
                        bool zgsz3560;
                        if (zgaz3222) {  zgsz3560 = eq_bool(zuz3200, false);  } else {  zgsz3560 = false;  }
                        zgaz3223 = zgsz3560;
                      }
                      bool zgsz3561;
                      if (zgaz3223) {  zgsz3561 = eq_bool(zuz3199, true);  } else {  zgsz3561 = false;  }
                      zgaz3224 = zgsz3561;
                    }
                    bool zgsz3562;
                    if (zgaz3224) {  zgsz3562 = eq_bool(zuz3198, true);  } else {  zgsz3562 = false;  }
                    bool zgsz3566;
                    zgsz3566 = zgsz3562;
                    if (!(zgsz3566)) {






                      goto case_478;
                    }
                    uint64_t zgaz3218;
                    zgaz3218 = sailgen_execution_mode_encdec_forwards_infallible(zmapping7z3);
                    struct zoptionzIozK zgsz3563;
                    CREATE(zoptionzIozK)(&zgsz3563);
                    {
                      uint64_t zuz3204;
                      zuz3204 = zgaz3218;
                      sailgen_SomezIozK(&zgsz3563, true);
                      goto finish_match_479;
                    }
                  case_481: ;
                    {
                      sailgen_NonezIozK(&zgsz3563, UNIT);
                      goto finish_match_479;
                    }
                  case_480: ;
                  finish_match_479: ;
                    COPY(zoptionzIozK)(&zgaz3225, zgsz3563);
                    KILL(zoptionzIozK)(&zgsz3563);
                    goto finish_match_476;
                  }
                case_478: ;
                  {
                    sailgen_NonezIozK(&zgaz3225, UNIT);
                    goto finish_match_476;
                  }
                case_477: ;
                finish_match_476: ;
                }
                bool zgsz3567;
                {
                  if (zgaz3225.kind != Kind_zSomezIozK) goto case_513;
                  zgsz3567 = zgaz3225.zSomezIozK;
                  goto finish_match_482;
                }
              case_513: ;
                {
                  __label__ case_512, case_491, finish_match_490;

                  /* complete */
                  struct zoptionzIozK zgaz3233;
                  CREATE(zoptionzIozK)(&zgaz3233);
                  {
                    __label__ case_486, case_485, finish_match_484;

                    {
                      __label__ case_489, case_488, finish_match_487;

                      bool zuz3206;
                      zuz3206 = zhead_expz3.ztup0.zR;
                      bool zuz3207;
                      zuz3207 = zhead_expz3.ztup0.zW;
                      bool zuz3208;
                      zuz3208 = zhead_expz3.ztup0.zC;
                      bool zuz3209;
                      zuz3209 = zhead_expz3.ztup0.zLM;
                      bool zuz3210;
                      zuz3210 = zhead_expz3.ztup0.zX;
                      bool zuz3211;
                      zuz3211 = zhead_expz3.ztup0.zASR;
                      enum zExecutionMode zmapping8z3;
                      zmapping8z3 = zhead_expz3.ztup1;
                      bool zgaz3232;
                      {
                        bool zgaz3231;
                        {
                          bool zgaz3230;
                          {
                            bool zgaz3229;
                            {
                              bool zgaz3228;
                              {
                                bool zgaz3227;
                                zgaz3227 = sailgen_execution_mode_encdec_forwards_matches(zmapping8z3);
                                bool zgsz3570;
                                if (zgaz3227) {  zgsz3570 = eq_bool(zuz3211, false);  } else {  zgsz3570 = false;  }
                                zgaz3228 = zgsz3570;
                              }
                              bool zgsz3571;
                              if (zgaz3228) {  zgsz3571 = eq_bool(zuz3210, false);  } else {  zgsz3571 = false;  }
                              zgaz3229 = zgsz3571;
                            }
                            bool zgsz3572;
                            if (zgaz3229) {  zgsz3572 = eq_bool(zuz3209, false);  } else {  zgsz3572 = false;  }
                            zgaz3230 = zgsz3572;
                          }
                          bool zgsz3573;
                          if (zgaz3230) {  zgsz3573 = eq_bool(zuz3208, true);  } else {  zgsz3573 = false;  }
                          zgaz3231 = zgsz3573;
                        }
                        bool zgsz3574;
                        if (zgaz3231) {  zgsz3574 = eq_bool(zuz3207, false);  } else {  zgsz3574 = false;  }
                        zgaz3232 = zgsz3574;
                      }
                      bool zgsz3575;
                      if (zgaz3232) {  zgsz3575 = eq_bool(zuz3206, true);  } else {  zgsz3575 = false;  }
                      bool zgsz3579;
                      zgsz3579 = zgsz3575;
                      if (!(zgsz3579)) {






                        goto case_486;
                      }
                      uint64_t zgaz3226;
                      zgaz3226 = sailgen_execution_mode_encdec_forwards_infallible(zmapping8z3);
                      struct zoptionzIozK zgsz3576;
                      CREATE(zoptionzIozK)(&zgsz3576);
                      {
                        uint64_t zb__2;
                        zb__2 = zgaz3226;
                        bool zgsz3578;
                        zgsz3578 = (zb__2 == UINT64_C(0b0));
                        if (!(zgsz3578)) {

                          goto case_489;
                        }
                        sailgen_SomezIozK(&zgsz3576, true);
                        goto finish_match_487;
                      }
                    case_489: ;
                      {
                        sailgen_NonezIozK(&zgsz3576, UNIT);
                        goto finish_match_487;
                      }
                    case_488: ;
                    finish_match_487: ;
                      COPY(zoptionzIozK)(&zgaz3233, zgsz3576);
                      KILL(zoptionzIozK)(&zgsz3576);
                      goto finish_match_484;
                    }
                  case_486: ;
                    {
                      sailgen_NonezIozK(&zgaz3233, UNIT);
                      goto finish_match_484;
                    }
                  case_485: ;
                  finish_match_484: ;
                  }
                  bool zgsz3580;
                  {
                    if (zgaz3233.kind != Kind_zSomezIozK) goto case_512;
                    zgsz3580 = zgaz3233.zSomezIozK;
                    goto finish_match_490;
                  }
                case_512: ;
                  {
                    __label__ case_511, case_499, finish_match_498;

                    /* complete */
                    struct zoptionzIozK zgaz3241;
                    CREATE(zoptionzIozK)(&zgaz3241);
                    {
                      __label__ case_494, case_493, finish_match_492;

                      {
                        __label__ case_497, case_496, finish_match_495;

                        bool zuz3213;
                        zuz3213 = zhead_expz3.ztup0.zR;
                        bool zuz3214;
                        zuz3214 = zhead_expz3.ztup0.zW;
                        bool zuz3215;
                        zuz3215 = zhead_expz3.ztup0.zC;
                        bool zuz3216;
                        zuz3216 = zhead_expz3.ztup0.zLM;
                        bool zuz3217;
                        zuz3217 = zhead_expz3.ztup0.zX;
                        bool zuz3218;
                        zuz3218 = zhead_expz3.ztup0.zASR;
                        enum zExecutionMode zmapping9z3;
                        zmapping9z3 = zhead_expz3.ztup1;
                        bool zgaz3240;
                        {
                          bool zgaz3239;
                          {
                            bool zgaz3238;
                            {
                              bool zgaz3237;
                              {
                                bool zgaz3236;
                                {
                                  bool zgaz3235;
                                  zgaz3235 = sailgen_execution_mode_encdec_forwards_matches(zmapping9z3);
                                  bool zgsz3583;
                                  if (zgaz3235) {  zgsz3583 = eq_bool(zuz3218, false);  } else {  zgsz3583 = false;  }
                                  zgaz3236 = zgsz3583;
                                }
                                bool zgsz3584;
                                if (zgaz3236) {  zgsz3584 = eq_bool(zuz3217, false);  } else {  zgsz3584 = false;  }
                                zgaz3237 = zgsz3584;
                              }
                              bool zgsz3585;
                              if (zgaz3237) {  zgsz3585 = eq_bool(zuz3216, true);  } else {  zgsz3585 = false;  }
                              zgaz3238 = zgsz3585;
                            }
                            bool zgsz3586;
                            if (zgaz3238) {  zgsz3586 = eq_bool(zuz3215, true);  } else {  zgsz3586 = false;  }
                            zgaz3239 = zgsz3586;
                          }
                          bool zgsz3587;
                          if (zgaz3239) {  zgsz3587 = eq_bool(zuz3214, false);  } else {  zgsz3587 = false;  }
                          zgaz3240 = zgsz3587;
                        }
                        bool zgsz3588;
                        if (zgaz3240) {  zgsz3588 = eq_bool(zuz3213, true);  } else {  zgsz3588 = false;  }
                        bool zgsz3592;
                        zgsz3592 = zgsz3588;
                        if (!(zgsz3592)) {






                          goto case_494;
                        }
                        uint64_t zgaz3234;
                        zgaz3234 = sailgen_execution_mode_encdec_forwards_infallible(zmapping9z3);
                        struct zoptionzIozK zgsz3589;
                        CREATE(zoptionzIozK)(&zgsz3589);
                        {
                          uint64_t zb__1;
                          zb__1 = zgaz3234;
                          bool zgsz3591;
                          zgsz3591 = (zb__1 == UINT64_C(0b0));
                          if (!(zgsz3591)) {

                            goto case_497;
                          }
                          sailgen_SomezIozK(&zgsz3589, true);
                          goto finish_match_495;
                        }
                      case_497: ;
                        {
                          sailgen_NonezIozK(&zgsz3589, UNIT);
                          goto finish_match_495;
                        }
                      case_496: ;
                      finish_match_495: ;
                        COPY(zoptionzIozK)(&zgaz3241, zgsz3589);
                        KILL(zoptionzIozK)(&zgsz3589);
                        goto finish_match_492;
                      }
                    case_494: ;
                      {
                        sailgen_NonezIozK(&zgaz3241, UNIT);
                        goto finish_match_492;
                      }
                    case_493: ;
                    finish_match_492: ;
                    }
                    bool zgsz3593;
                    {
                      if (zgaz3241.kind != Kind_zSomezIozK) goto case_511;
                      zgsz3593 = zgaz3241.zSomezIozK;
                      goto finish_match_498;
                    }
                  case_511: ;
                    {
                      __label__ case_510, case_507, finish_match_506;

                      /* complete */
                      struct zoptionzIozK zgaz3249;
                      CREATE(zoptionzIozK)(&zgaz3249);
                      {
                        __label__ case_502, case_501, finish_match_500;

                        {
                          __label__ case_505, case_504, finish_match_503;

                          bool zuz3220;
                          zuz3220 = zhead_expz3.ztup0.zR;
                          bool zuz3221;
                          zuz3221 = zhead_expz3.ztup0.zW;
                          bool zuz3222;
                          zuz3222 = zhead_expz3.ztup0.zC;
                          bool zuz3223;
                          zuz3223 = zhead_expz3.ztup0.zLM;
                          bool zuz3224;
                          zuz3224 = zhead_expz3.ztup0.zX;
                          bool zuz3225;
                          zuz3225 = zhead_expz3.ztup0.zASR;
                          enum zExecutionMode zmapping10z3;
                          zmapping10z3 = zhead_expz3.ztup1;
                          bool zgaz3248;
                          {
                            bool zgaz3247;
                            {
                              bool zgaz3246;
                              {
                                bool zgaz3245;
                                {
                                  bool zgaz3244;
                                  {
                                    bool zgaz3243;
                                    zgaz3243 = sailgen_execution_mode_encdec_forwards_matches(zmapping10z3);
                                    bool zgsz3596;
                                    if (zgaz3243) {  zgsz3596 = eq_bool(zuz3225, false);  } else {  zgsz3596 = false;  }
                                    zgaz3244 = zgsz3596;
                                  }
                                  bool zgsz3597;
                                  if (zgaz3244) {  zgsz3597 = eq_bool(zuz3224, false);  } else {  zgsz3597 = false;  }
                                  zgaz3245 = zgsz3597;
                                }
                                bool zgsz3598;
                                if (zgaz3245) {  zgsz3598 = eq_bool(zuz3223, true);  } else {  zgsz3598 = false;  }
                                zgaz3246 = zgsz3598;
                              }
                              bool zgsz3599;
                              if (zgaz3246) {  zgsz3599 = eq_bool(zuz3222, true);  } else {  zgsz3599 = false;  }
                              zgaz3247 = zgsz3599;
                            }
                            bool zgsz3600;
                            if (zgaz3247) {  zgsz3600 = eq_bool(zuz3221, true);  } else {  zgsz3600 = false;  }
                            zgaz3248 = zgsz3600;
                          }
                          bool zgsz3601;
                          if (zgaz3248) {  zgsz3601 = eq_bool(zuz3220, true);  } else {  zgsz3601 = false;  }
                          bool zgsz3605;
                          zgsz3605 = zgsz3601;
                          if (!(zgsz3605)) {






                            goto case_502;
                          }
                          uint64_t zgaz3242;
                          zgaz3242 = sailgen_execution_mode_encdec_forwards_infallible(zmapping10z3);
                          struct zoptionzIozK zgsz3602;
                          CREATE(zoptionzIozK)(&zgsz3602);
                          {
                            uint64_t zb__0;
                            zb__0 = zgaz3242;
                            bool zgsz3604;
                            zgsz3604 = (zb__0 == UINT64_C(0b0));
                            if (!(zgsz3604)) {

                              goto case_505;
                            }
                            sailgen_SomezIozK(&zgsz3602, true);
                            goto finish_match_503;
                          }
                        case_505: ;
                          {
                            sailgen_NonezIozK(&zgsz3602, UNIT);
                            goto finish_match_503;
                          }
                        case_504: ;
                        finish_match_503: ;
                          COPY(zoptionzIozK)(&zgaz3249, zgsz3602);
                          KILL(zoptionzIozK)(&zgsz3602);
                          goto finish_match_500;
                        }
                      case_502: ;
                        {
                          sailgen_NonezIozK(&zgaz3249, UNIT);
                          goto finish_match_500;
                        }
                      case_501: ;
                      finish_match_500: ;
                      }
                      bool zgsz3606;
                      {
                        if (zgaz3249.kind != Kind_zSomezIozK) goto case_510;
                        zgsz3606 = zgaz3249.zSomezIozK;
                        goto finish_match_506;
                      }
                    case_510: ;
                      {
                        __label__ case_509, finish_match_508;

                        /* complete */
                        bool zgsz3607;
                        {
                          zgsz3607 = false;
                          goto finish_match_508;
                        }
                      case_509: ;
                        sail_match_failure("ap_m_encdec_backwards_matches");
                      finish_match_508: ;
                        zgsz3606 = zgsz3607;
                        goto finish_match_506;
                      }
                    case_507: ;
                    finish_match_506: ;
                      zgsz3593 = zgsz3606;
                      KILL(zoptionzIozK)(&zgaz3249);
                      goto finish_match_498;
                    }
                  case_499: ;
                  finish_match_498: ;
                    zgsz3580 = zgsz3593;
                    KILL(zoptionzIozK)(&zgaz3241);
                    goto finish_match_490;
                  }
                case_491: ;
                finish_match_490: ;
                  zgsz3567 = zgsz3580;
                  KILL(zoptionzIozK)(&zgaz3233);
                  goto finish_match_482;
                }
              case_483: ;
              finish_match_482: ;
                zgsz3554 = zgsz3567;
                KILL(zoptionzIozK)(&zgaz3225);
                goto finish_match_474;
              }
            case_475: ;
            finish_match_474: ;
              zgsz3541 = zgsz3554;
              KILL(zoptionzIozK)(&zgaz3217);
              goto finish_match_466;
            }
          case_467: ;
          finish_match_466: ;
            zgsz3528 = zgsz3541;
            KILL(zoptionzIozK)(&zgaz3209);
            goto finish_match_458;
          }
        case_459: ;
        finish_match_458: ;
          zgsz3515 = zgsz3528;
          KILL(zoptionzIozK)(&zgaz3201);
          goto finish_match_450;
        }
      case_451: ;
      finish_match_450: ;
        zgsz3502 = zgsz3515;
        KILL(zoptionzIozK)(&zgaz3193);
        goto finish_match_442;
      }
    case_443: ;
    finish_match_442: ;
      zgsz3489 = zgsz3502;
      KILL(zoptionzIozK)(&zgaz3185);
      goto finish_match_434;
    }
  case_435: ;
  finish_match_434: ;
    zgsz3476 = zgsz3489;
    KILL(zoptionzIozK)(&zgaz3177);
    goto finish_match_426;
  }
case_427: ;
finish_match_426: ;
  zcbz350 = zgsz3476;

  KILL(zoptionzIozK)(&zgaz3169);

end_function_521: ;
  return zcbz350;
end_block_exception_522: ;

  return false;
}

static uint64_t znull_ap_m;
static void create_letbind_14(void) {


  uint64_t zgsz3631;
  zgsz3631 = UINT64_C(0b00000);
  znull_ap_m = zgsz3631;

let_end_523: ;
}
static void kill_letbind_14(void) {
}

static uint64_t zinfinite_ap_m;
static void create_letbind_15(void) {


  uint64_t zgsz3632;
  zgsz3632 = UINT64_C(0b01001);
  zinfinite_ap_m = zgsz3632;

let_end_524: ;
}
static void kill_letbind_15(void) {
}

static struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 sailgen_legalizzePermsAndMode(struct zArchPerms, enum zExecutionMode);

struct zArchPerms zghz3119;
struct zArchPerms zghz3120;
struct zArchPerms zghz3121;
struct zArchPerms zghz3122;
struct zArchPerms zghz3123;
struct zArchPerms zghz3124;
struct zArchPerms zghz3125;
struct zArchPerms zghz3126;
struct zArchPerms zghz3127;
struct zArchPerms zghz3128;
struct zArchPerms zghz3129;
struct zArchPerms zghz3130;

static void startup_sailgen_legalizzePermsAndMode(void)
{












}

static struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 sailgen_legalizzePermsAndMode(struct zArchPerms zperms, enum zExecutionMode zmode)
{
  __label__ case_527, finish_match_526, end_function_528, end_block_exception_529;

  struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zcbz351;
  struct zArchPerms zghz3119;
  {
    bool zgaz3271;
    {
      bool zgaz3270;
      zgaz3270 = zperms.zR;
      bool zgsz3637;
      if (zgaz3270) {
        bool zgaz3269;
        zgaz3269 = zperms.zW;
        bool zgsz3636;
        if (zgaz3269) {
          bool zgaz3268;
          zgaz3268 = zperms.zX;
          bool zgsz3635;
          if (zgaz3268) {
            bool zgaz3267;
            zgaz3267 = zperms.zASR;
            bool zgsz3634;
            if (zgaz3267) {
              bool zgaz3266;
              zgaz3266 = zperms.zC;
              bool zgsz3633;
              if (zgaz3266) {  zgsz3633 = zperms.zLM;  } else {  zgsz3633 = false;  }
              zgsz3634 = zgsz3633;
            } else {  zgsz3634 = false;  }
            zgsz3635 = zgsz3634;
          } else {  zgsz3635 = false;  }
          zgsz3636 = zgsz3635;
        } else {  zgsz3636 = false;  }
        zgsz3637 = zgsz3636;
      } else {  zgsz3637 = false;  }
      zgaz3271 = zgsz3637;
    }
    if (zgaz3271) {  zghz3119 = zperms;  } else {
      struct zArchPerms zghz3130;
      zghz3130 = zperms;
      zghz3130.zASR = false;
      zghz3119 = zghz3130;
    }
  }
  struct zArchPerms zghz3120;
  {
    bool zgaz3265;
    zgaz3265 = zghz3119.zR;
    if (zgaz3265) {  zghz3120 = zghz3119;  } else {
      struct zArchPerms zghz3129;
      zghz3129 = zghz3119;
      zghz3129.zC = false;
      zghz3129.zX = false;
      zghz3120 = zghz3129;
    }
  }
  struct zArchPerms zghz3121;
  {
    bool zgaz3264;
    {
      bool zgaz3263;
      zgaz3263 = zghz3120.zX;
      bool zgsz3642;
      if (zgaz3263) {
        bool zgaz3262;
        zgaz3262 = zghz3120.zR;
        bool zgsz3641;
        if (zgaz3262) {
          bool zgaz3261;
          {
            bool zgaz3259;
            zgaz3259 = zghz3120.zC;
            zgaz3261 = not(zgaz3259);
          }
          bool zgsz3640;
          if (zgaz3261) {
            bool zgaz3260;
            zgaz3260 = zghz3120.zW;
            zgsz3640 = not(zgaz3260);
          } else {  zgsz3640 = false;  }
          zgsz3641 = zgsz3640;
        } else {  zgsz3641 = false;  }
        zgsz3642 = zgsz3641;
      } else {  zgsz3642 = false;  }
      zgaz3264 = zgsz3642;
    }
    if (zgaz3264) {
      struct zArchPerms zghz3128;
      zghz3128 = zghz3120;
      zghz3128.zX = false;
      zghz3121 = zghz3128;
    } else {  zghz3121 = zghz3120;  }
  }
  enum zExecutionMode zmodeshadowz33;
  {
    bool zgaz3258;
    zgaz3258 = zghz3121.zX;
    if (zgaz3258) {  zmodeshadowz33 = zmode;  } else {
      zmodeshadowz33 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    }
  }
  struct zArchPerms zghz3122;
  {
    bool zgaz3257;
    zgaz3257 = zghz3121.zC;
    if (zgaz3257) {  zghz3122 = zghz3121;  } else {
      struct zArchPerms zghz3127;
      zghz3127 = zghz3121;
      zghz3127.zLM = false;
      zghz3122 = zghz3127;
    }
  }
  struct zArchPerms zghz3123;
  {
    bool zgaz3256;
    {
      bool zgaz3255;
      zgaz3255 = zghz3122.zC;
      bool zgsz3645;
      if (zgaz3255) {
        bool zgaz3254;
        zgaz3254 = zghz3122.zLM;
        zgsz3645 = not(zgaz3254);
      } else {  zgsz3645 = false;  }
      zgaz3256 = zgsz3645;
    }
    if (zgaz3256) {
      struct zArchPerms zghz3126;
      zghz3126 = zghz3122;
      zghz3126.zW = false;
      zghz3123 = zghz3126;
    } else {  zghz3123 = zghz3122;  }
  }
  struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgaz3253;
  {
    bool zgaz3250;
    {
      struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz3647;
      zgsz3647.ztup0 = zghz3123;
      zgsz3647.ztup1 = zmodeshadowz33;
      zgaz3250 = sailgen_ap_m_encdec_backwards_matches(zgsz3647);
    }
    if (zgaz3250) {
      struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz3650;
      zgsz3650.ztup0 = zghz3123;
      zgsz3650.ztup1 = zmodeshadowz33;
      zgaz3253 = zgsz3650;
    } else {
      struct zArchPerms zghz3125;
      {
        zghz3125 = znull_perms;
        zghz3125.zC = true;
        zghz3125.zR = true;
      }
      enum zExecutionMode zgaz3252;
      zgaz3252 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
      struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz3649;
      zgsz3649.ztup0 = zghz3125;
      zgsz3649.ztup1 = zgaz3252;
      zgaz3253 = zgsz3649;
    }
  }
  struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz3651;
  {
    struct zArchPerms zghz3124;
    zghz3124 = zgaz3253.ztup0;
    enum zExecutionMode zmodeshadowz36;
    zmodeshadowz36 = zgaz3253.ztup1;
    zgsz3651.ztup0 = zghz3124;
    zgsz3651.ztup1 = zmodeshadowz36;
    goto finish_match_526;
  }
case_527: ;
  sail_match_failure("legalizePermsAndMode");
finish_match_526: ;
  zcbz351 = zgsz3651;



end_function_528: ;
  return zcbz351;
end_block_exception_529: ;
  struct zArchPerms zcbz3102 = { .zASR = false, .zC = false, .zLM = false, .zR = false, .zW = false, .zX = false };
  struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zcbz3101 = { .ztup0 = zcbz3102, .ztup1 = zCapPtrMode };
  return zcbz3101;
}



static void finish_sailgen_legalizzePermsAndMode(void)
{












}

static uint64_t sailgen_getMetadataL8(struct zEncMetadata);

static uint64_t sailgen_getMetadataL8(struct zEncMetadata zmeta)
{
  __label__ end_function_531, end_block_exception_532;

  uint64_t zcbz352;
  zcbz352 = sailgen__get_EncMetadata_L8(zmeta);
end_function_531: ;
  return zcbz352;
end_block_exception_532: ;

  return UINT64_C(0xdeadc0de);
}

static struct zEncMetadata sailgen_setMetadataL8(struct zEncMetadata, uint64_t);

struct zEncMetadata zghz3131;

static void startup_sailgen_setMetadataL8(void)
{
}

static struct zEncMetadata sailgen_setMetadataL8(struct zEncMetadata zmeta, uint64_t zL8)
{
  __label__ end_function_534, end_block_exception_535;

  struct zEncMetadata zghz3131;
  zghz3131 = sailgen__update_EncMetadata_L8(zmeta, zL8);
end_function_534: ;
  return zghz3131;
end_block_exception_535: ;
  struct zEncMetadata zcbz3103 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz3103;
}



static void finish_sailgen_setMetadataL8(void)
{
}

static uint64_t sailgen_getMetadataInternalExponent(struct zEncMetadata);

static uint64_t sailgen_getMetadataInternalExponent(struct zEncMetadata zmeta)
{
  __label__ end_function_537, end_block_exception_538;

  uint64_t zcbz354;
  uint64_t zgaz3274;
  zgaz3274 = sailgen__get_EncMetadata_L8(zmeta);
  uint64_t zgaz3275;
  {
    uint64_t zgaz3272;
    zgaz3272 = sailgen__get_EncMetadata_TE(zmeta);
    uint64_t zgaz3273;
    zgaz3273 = sailgen__get_EncMetadata_BE(zmeta);
    zgaz3275 = (zgaz3272 << 2) | zgaz3273;
  }
  zcbz354 = (zgaz3274 << 4) | zgaz3275;


end_function_537: ;
  return zcbz354;
end_block_exception_538: ;

  return UINT64_C(0xdeadc0de);
}

static int64_t zcap_sizze;
static void create_letbind_16(void) {


  int64_t zgsz3654;
  zgsz3654 = INT64_C(8);
  zcap_sizze = zgsz3654;

let_end_539: ;
}
static void kill_letbind_16(void) {
}

static int64_t zcap_addr_width;
static void create_letbind_17(void) {


  int64_t zgsz3655;
  zgsz3655 = INT64_C(32);
  zcap_addr_width = zgsz3655;

let_end_540: ;
}
static void kill_letbind_17(void) {
}

static int64_t zcap_len_width;
static void create_letbind_18(void) {


  int64_t zgsz3656;
  zgsz3656 = INT64_C(33);
  zcap_len_width = zgsz3656;

let_end_541: ;
}
static void kill_letbind_18(void) {
}

static int64_t zcap_max_addr;
static void create_letbind_19(void) {


  int64_t zgsz3657;
  {
    sail_int zgsz3882;
    CREATE(sail_int)(&zgsz3882);
    CONVERT_OF(sail_int, mach_int)(&zgsz3882, zcap_addr_width);
    sail_int zgsz3883;
    CREATE(sail_int)(&zgsz3883);
    sailgen_MAX(&zgsz3883, zgsz3882);
    zgsz3657 = CONVERT_OF(mach_int, sail_int)(zgsz3883);
    KILL(sail_int)(&zgsz3883);
    KILL(sail_int)(&zgsz3882);
  }
  zcap_max_addr = zgsz3657;

let_end_542: ;
}
static void kill_letbind_19(void) {
}

static int64_t zcap_xe_width;
static void create_letbind_20(void) {


  int64_t zgsz3658;
  zgsz3658 = INT64_C(2);
  zcap_xe_width = zgsz3658;

let_end_543: ;
}
static void kill_letbind_20(void) {
}

static int64_t zcap_max_E;
static void create_letbind_21(void) {


  int64_t zgsz3659;
  zgsz3659 = INT64_C(24);
  zcap_max_E = zgsz3659;

let_end_544: ;
}
static void kill_letbind_21(void) {
}

static int64_t zcap_min_E;
static void create_letbind_22(void) {


  int64_t zgsz3660;
  zgsz3660 = INT64_C(-7);
  zcap_min_E = zgsz3660;

let_end_545: ;
}
static void kill_letbind_22(void) {
}

static uint64_t sailgen_internal_exponent_encdec_forwards(bool);

static bool sailgen_internal_exponent_encdec_backwards(uint64_t);

static uint64_t sailgen_internal_exponent_encdec_forwards(bool zargz3)
{
  __label__ case_549, case_548, finish_match_547, end_function_550, end_block_exception_551;

  uint64_t zcbz355;
  {
    bool zp0z3;
    zp0z3 = zargz3;
    bool zgsz3663;
    zgsz3663 = eq_bool(zp0z3, false);
    if (!(zgsz3663)) {

      goto case_549;
    }
    zcbz355 = UINT64_C(0b1);
    goto finish_match_547;
  }
case_549: ;
  {
    bool zuz3227;
    zuz3227 = zargz3;
    bool zgsz3662;
    zgsz3662 = eq_bool(zuz3227, true);
    if (!(zgsz3662)) {

      goto case_548;
    }
    zcbz355 = UINT64_C(0b0);
    goto finish_match_547;
  }
case_548: ;
finish_match_547: ;
end_function_550: ;
  return zcbz355;
end_block_exception_551: ;

  return UINT64_C(0xdeadc0de);
}

static uint64_t sailgen_internal_exponent_encdec_forwards_infallible(bool);

static uint64_t sailgen_internal_exponent_encdec_forwards_infallible(bool zargz3)
{
  __label__ case_554, case_553, finish_match_552, end_function_555, end_block_exception_556;

  uint64_t zcbz356;
  {
    bool zp0z3;
    zp0z3 = zargz3;
    bool zgsz3666;
    zgsz3666 = eq_bool(zp0z3, false);
    if (!(zgsz3666)) {

      goto case_554;
    }
    zcbz356 = UINT64_C(0b1);
    goto finish_match_552;
  }
case_554: ;
  {
    bool zuz3228;
    zuz3228 = zargz3;
    bool zgsz3665;
    zgsz3665 = eq_bool(zuz3228, true);
    if (!(zgsz3665)) {

      goto case_553;
    }
    zcbz356 = UINT64_C(0b0);
    goto finish_match_552;
  }
case_553: ;
finish_match_552: ;
end_function_555: ;
  return zcbz356;
end_block_exception_556: ;

  return UINT64_C(0xdeadc0de);
}

static bool sailgen_internal_exponent_encdec_backwards(uint64_t zargz3)
{
  __label__ case_560, case_559, finish_match_558, end_function_561, end_block_exception_562;

  bool zcbz357;
  {
    uint64_t zb__0;
    zb__0 = zargz3;
    bool zgsz3669;
    zgsz3669 = (zb__0 == UINT64_C(0b1));
    if (!(zgsz3669)) {

      goto case_560;
    }
    zcbz357 = false;
    goto finish_match_558;
  }
case_560: ;
  {
    zcbz357 = true;
    goto finish_match_558;
  }
case_559: ;
finish_match_558: ;
end_function_561: ;
  return zcbz357;
end_block_exception_562: ;

  return false;
}

static bool sailgen_internal_exponent_encdec_backwards_infallible(uint64_t);

static bool sailgen_internal_exponent_encdec_backwards_infallible(uint64_t zargz3)
{
  __label__ case_565, case_564, finish_match_563, end_function_566, end_block_exception_567;

  bool zcbz358;
  {
    uint64_t zb__0;
    zb__0 = zargz3;
    bool zgsz3672;
    zgsz3672 = (zb__0 == UINT64_C(0b1));
    if (!(zgsz3672)) {

      goto case_565;
    }
    zcbz358 = false;
    goto finish_match_563;
  }
case_565: ;
  {
    zcbz358 = true;
    goto finish_match_563;
  }
case_564: ;
finish_match_563: ;
end_function_566: ;
  return zcbz358;
end_block_exception_567: ;

  return false;
}

static struct zCapability sailgen_undefined_Capability(unit);

struct zCapability zghz3132;
sail_int zghz3133;

static void startup_sailgen_undefined_Capability(void)
{

  CREATE(sail_int)(&zghz3133);
}

static struct zCapability sailgen_undefined_Capability(unit zgsz3673)
{
  __label__ end_function_569, end_block_exception_570;

  struct zCapability zghz3132;
  zghz3132.zB = UINT64_C(0b0000000000);
  RECREATE(sail_int)(&zghz3133);
  CONVERT_OF(sail_int, mach_int)(&zghz3133, INT64_C(-7));
  zghz3132.zE = CONVERT_OF(mach_int, sail_int)(zghz3133);
  zghz3132.zT = UINT64_C(0b0000000000);
  zghz3132.zaddress = UINT64_C(0x00000000);
  zghz3132.zap_m = UINT64_C(0b00000);
  zghz3132.zinternal_exponent = false;
  zghz3132.zreserved_0 = UINT64_C(0b000);
  zghz3132.zreserved_1 = UINT64_C(0b0);
  zghz3132.zsd_perms = UINT64_C(0b00);
  zghz3132.zsealed = false;
  zghz3132.ztag = false;
end_function_569: ;
  return zghz3132;
end_block_exception_570: ;
  struct zCapability zcbz3104 = { .zB = UINT64_C(0xdeadc0de), .zE = INT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zap_m = UINT64_C(0xdeadc0de), .zinternal_exponent = false, .zreserved_0 = UINT64_C(0xdeadc0de), .zreserved_1 = UINT64_C(0xdeadc0de), .zsd_perms = UINT64_C(0xdeadc0de), .zsealed = false, .ztag = false };
  return zcbz3104;
}



static void finish_sailgen_undefined_Capability(void)
{
  KILL(sail_int)(&zghz3133);

}

static struct zCapability sailgen_bitsToCap(bool, uint64_t);

struct zCapability zghz3134;
struct zEncMetadata zghz3135;
struct zCapability zghz3136;
sail_int zghz3137;
sail_int zghz3138;
sail_int zghz3139;
lbits zghz3140;
lbits zghz3141;
lbits zghz3142;
lbits zghz3143;
lbits zghz3144;
lbits zghz3145;
lbits zghz3146;
sail_int zghz3147;
sail_int zghz3148;
lbits zghz3149;
sail_int zghz3150;
sail_int zghz3151;
sail_int zghz3152;
sail_int zghz3153;
sail_int zghz3154;
sail_int zghz3155;
lbits zghz3156;
lbits zghz3157;
lbits zghz3158;
sail_int zghz3159;
sail_int zghz3160;
lbits zghz3161;
sail_int zghz3162;
sail_int zghz3163;
sail_int zghz3164;
sail_int zghz3165;
sail_int zghz3166;
sail_int zghz3167;
lbits zghz3168;
sail_int zghz3169;
sail_int zghz3170;
sail_int zghz3171;
sail_int zghz3172;
sail_int zghz3173;
sail_int zghz3174;
sail_int zghz3175;

static void startup_sailgen_bitsToCap(void)
{



  CREATE(sail_int)(&zghz3137);
  CREATE(sail_int)(&zghz3138);
  CREATE(sail_int)(&zghz3139);
  CREATE(lbits)(&zghz3140);
  CREATE(lbits)(&zghz3141);
  CREATE(lbits)(&zghz3142);
  CREATE(lbits)(&zghz3143);
  CREATE(lbits)(&zghz3144);
  CREATE(lbits)(&zghz3145);
  CREATE(lbits)(&zghz3146);
  CREATE(sail_int)(&zghz3147);
  CREATE(sail_int)(&zghz3148);
  CREATE(lbits)(&zghz3149);
  CREATE(sail_int)(&zghz3150);
  CREATE(sail_int)(&zghz3151);
  CREATE(sail_int)(&zghz3152);
  CREATE(sail_int)(&zghz3153);
  CREATE(sail_int)(&zghz3154);
  CREATE(sail_int)(&zghz3155);
  CREATE(lbits)(&zghz3156);
  CREATE(lbits)(&zghz3157);
  CREATE(lbits)(&zghz3158);
  CREATE(sail_int)(&zghz3159);
  CREATE(sail_int)(&zghz3160);
  CREATE(lbits)(&zghz3161);
  CREATE(sail_int)(&zghz3162);
  CREATE(sail_int)(&zghz3163);
  CREATE(sail_int)(&zghz3164);
  CREATE(sail_int)(&zghz3165);
  CREATE(sail_int)(&zghz3166);
  CREATE(sail_int)(&zghz3167);
  CREATE(lbits)(&zghz3168);
  CREATE(sail_int)(&zghz3169);
  CREATE(sail_int)(&zghz3170);
  CREATE(sail_int)(&zghz3171);
  CREATE(sail_int)(&zghz3172);
  CREATE(sail_int)(&zghz3173);
  CREATE(sail_int)(&zghz3174);
  CREATE(sail_int)(&zghz3175);
}

static struct zCapability sailgen_bitsToCap(bool ztag, uint64_t zcap_bits)
{
  __label__ case_573, finish_match_572, end_function_574, end_block_exception_575;

  struct zCapability zghz3134;
  struct zEncMetadata zghz3135;
  {
    uint64_t zgaz3302;
    {
      int64_t zgaz3301;
      {
        int64_t zgaz3300;
        {
          RECREATE(sail_int)(&zghz3173);
          CONVERT_OF(sail_int, mach_int)(&zghz3173, zxlen);
          RECREATE(sail_int)(&zghz3174);
          CONVERT_OF(sail_int, mach_int)(&zghz3174, INT64_C(2));
          RECREATE(sail_int)(&zghz3175);
          mult_int(&zghz3175, zghz3173, zghz3174);
          zgaz3300 = CONVERT_OF(mach_int, sail_int)(zghz3175);
        }
        {
          RECREATE(sail_int)(&zghz3170);
          CONVERT_OF(sail_int, mach_int)(&zghz3170, zgaz3300);
          RECREATE(sail_int)(&zghz3171);
          CONVERT_OF(sail_int, mach_int)(&zghz3171, INT64_C(1));
          RECREATE(sail_int)(&zghz3172);
          sub_int(&zghz3172, zghz3170, zghz3171);
          zgaz3301 = CONVERT_OF(mach_int, sail_int)(zghz3172);
        }
      }
      zgaz3302 = (safe_rshift(UINT64_MAX, 64 - 32) & (zcap_bits >> zxlen));
    }
    zghz3135 = sailgen_Mk_EncMetadata(zgaz3302);
  }
  bool zinternal_exponent;
  {
    uint64_t zgaz3299;
    zgaz3299 = sailgen__get_EncMetadata_EF(zghz3135);
    zinternal_exponent = sailgen_internal_exponent_encdec_backwards(zgaz3299);
  }
  {
    int64_t zMW;
    zMW = zcap_mantissa_width;
    int64_t zE;
    if (zinternal_exponent) {
      int64_t zgaz3298;
      {
        uint64_t zgaz3297;
        zgaz3297 = sailgen_getMetadataInternalExponent(zghz3135);
        {
          RECREATE(lbits)(&zghz3168);
          CONVERT_OF(lbits, fbits)(&zghz3168, zgaz3297, UINT64_C(5) , true);
          RECREATE(sail_int)(&zghz3169);
          sail_unsigned(&zghz3169, zghz3168);
          zgaz3298 = CONVERT_OF(mach_int, sail_int)(zghz3169);
        }
      }
      {
        RECREATE(sail_int)(&zghz3165);
        CONVERT_OF(sail_int, mach_int)(&zghz3165, zcap_max_E);
        RECREATE(sail_int)(&zghz3166);
        CONVERT_OF(sail_int, mach_int)(&zghz3166, zgaz3298);
        RECREATE(sail_int)(&zghz3167);
        sub_int(&zghz3167, zghz3165, zghz3166);
        zE = CONVERT_OF(mach_int, sail_int)(zghz3167);
      }
    } else {  zE = INT64_C(0);  }
    uint64_t zLMSB;
    if (zinternal_exponent) {  zLMSB = UINT64_C(0b01);  } else {
      uint64_t zgaz3296;
      zgaz3296 = sailgen_getMetadataL8(zghz3135);
      zLMSB = (UINT64_C(0b0) << 1) | zgaz3296;
    }
    uint64_t zT_low;
    {
      bool zgaz3295;
      zgaz3295 = not(zinternal_exponent);
      if (zgaz3295) {  zT_low = sailgen__get_EncMetadata_TE(zghz3135);  } else {  zT_low = UINT64_C(0b00);  }
    }
    uint64_t zB_low;
    {
      bool zgaz3294;
      zgaz3294 = not(zinternal_exponent);
      if (zgaz3294) {  zB_low = sailgen__get_EncMetadata_BE(zghz3135);  } else {  zB_low = UINT64_C(0b00);  }
    }
    sbits zT;
    {
      uint64_t zgaz3293;
      zgaz3293 = sailgen__get_EncMetadata_Tm(zghz3135);
      sbits zgsz3677;
      zgsz3677 = CONVERT_OF(sbits, fbits)((zgaz3293 << 2) | zT_low, UINT64_C(8) , true);
      zT = zgsz3677;
    }
    sbits zB;
    {
      uint64_t zgaz3292;
      zgaz3292 = sailgen__get_EncMetadata_Bm(zghz3135);
      sbits zgsz3678;
      zgsz3678 = CONVERT_OF(sbits, fbits)((zgaz3292 << 2) | zB_low, UINT64_C(10) , true);
      zB = zgsz3678;
    }
    uint64_t zLCout;
    {
      bool zgaz3291;
      {
        sbits zgaz3290;
        {
          int64_t zgaz3289;
          {
            RECREATE(sail_int)(&zghz3162);
            CONVERT_OF(sail_int, mach_int)(&zghz3162, zMW);
            RECREATE(sail_int)(&zghz3163);
            CONVERT_OF(sail_int, mach_int)(&zghz3163, INT64_C(3));
            RECREATE(sail_int)(&zghz3164);
            sub_int(&zghz3164, zghz3162, zghz3163);
            zgaz3289 = CONVERT_OF(mach_int, sail_int)(zghz3164);
          }
          {
            RECREATE(lbits)(&zghz3158);
            CONVERT_OF(lbits, sbits)(&zghz3158, zB, true);
            RECREATE(sail_int)(&zghz3159);
            CONVERT_OF(sail_int, mach_int)(&zghz3159, zgaz3289);
            RECREATE(sail_int)(&zghz3160);
            CONVERT_OF(sail_int, mach_int)(&zghz3160, INT64_C(0));
            RECREATE(lbits)(&zghz3161);
            vector_subrange_lbits(&zghz3161, zghz3158, zghz3159, zghz3160);
            zgaz3290 = CONVERT_OF(sbits, lbits)(zghz3161, true);
          }
        }
        {
          RECREATE(lbits)(&zghz3156);
          CONVERT_OF(lbits, sbits)(&zghz3156, zT, true);
          RECREATE(lbits)(&zghz3157);
          CONVERT_OF(lbits, sbits)(&zghz3157, zgaz3290, true);
          zgaz3291 = sailgen_z8operatorz0zI_uz9(zghz3156, zghz3157);
        }
      }
      if (zgaz3291) {  zLCout = UINT64_C(0b01);  } else {  zLCout = UINT64_C(0b00);  }
    }
    sbits zTshadowz38;
    {
      sbits zgaz3288;
      {
        sbits zgaz3287;
        {
          sbits zgaz3286;
          {
            int64_t zgaz3284;
            {
              RECREATE(sail_int)(&zghz3153);
              CONVERT_OF(sail_int, mach_int)(&zghz3153, zMW);
              RECREATE(sail_int)(&zghz3154);
              CONVERT_OF(sail_int, mach_int)(&zghz3154, INT64_C(1));
              RECREATE(sail_int)(&zghz3155);
              sub_int(&zghz3155, zghz3153, zghz3154);
              zgaz3284 = CONVERT_OF(mach_int, sail_int)(zghz3155);
            }
            int64_t zgaz3285;
            {
              RECREATE(sail_int)(&zghz3150);
              CONVERT_OF(sail_int, mach_int)(&zghz3150, zMW);
              RECREATE(sail_int)(&zghz3151);
              CONVERT_OF(sail_int, mach_int)(&zghz3151, INT64_C(2));
              RECREATE(sail_int)(&zghz3152);
              sub_int(&zghz3152, zghz3150, zghz3151);
              zgaz3285 = CONVERT_OF(mach_int, sail_int)(zghz3152);
            }
            {
              RECREATE(lbits)(&zghz3146);
              CONVERT_OF(lbits, sbits)(&zghz3146, zB, true);
              RECREATE(sail_int)(&zghz3147);
              CONVERT_OF(sail_int, mach_int)(&zghz3147, zgaz3284);
              RECREATE(sail_int)(&zghz3148);
              CONVERT_OF(sail_int, mach_int)(&zghz3148, zgaz3285);
              RECREATE(lbits)(&zghz3149);
              vector_subrange_lbits(&zghz3149, zghz3146, zghz3147, zghz3148);
              zgaz3286 = CONVERT_OF(sbits, lbits)(zghz3149, true);
            }
          }
          {
            RECREATE(lbits)(&zghz3143);
            CONVERT_OF(lbits, sbits)(&zghz3143, zgaz3286, true);
            RECREATE(lbits)(&zghz3144);
            CONVERT_OF(lbits, fbits)(&zghz3144, zLCout, UINT64_C(2) , true);
            RECREATE(lbits)(&zghz3145);
            add_bits(&zghz3145, zghz3143, zghz3144);
            zgaz3287 = CONVERT_OF(sbits, lbits)(zghz3145, true);
          }
        }
        {
          RECREATE(lbits)(&zghz3140);
          CONVERT_OF(lbits, sbits)(&zghz3140, zgaz3287, true);
          RECREATE(lbits)(&zghz3141);
          CONVERT_OF(lbits, fbits)(&zghz3141, zLMSB, UINT64_C(2) , true);
          RECREATE(lbits)(&zghz3142);
          add_bits(&zghz3142, zghz3140, zghz3141);
          zgaz3288 = CONVERT_OF(sbits, lbits)(zghz3142, true);
        }
      }
      zTshadowz38 = append_ss(zgaz3288, zT);
    }
    uint64_t zgaz3276;
    zgaz3276 = sailgen__get_EncMetadata_SDP(zghz3135);
    uint64_t zgaz3277;
    zgaz3277 = sailgen__get_EncMetadata_AP_M(zghz3135);
    bool zgaz3279;
    {
      uint64_t zgaz3278;
      zgaz3278 = sailgen__get_EncMetadata_CT(zghz3135);
      zgaz3279 = sailgen_bool_bits_backwards(zgaz3278);
    }
    uint64_t zgaz3280;
    zgaz3280 = sailgen__get_EncMetadata_reserved_0(zghz3135);
    uint64_t zgaz3281;
    zgaz3281 = sailgen__get_EncMetadata_reserved_1(zghz3135);
    uint64_t zgaz3283;
    {
      int64_t zgaz3282;
      {
        RECREATE(sail_int)(&zghz3137);
        CONVERT_OF(sail_int, mach_int)(&zghz3137, zxlen);
        RECREATE(sail_int)(&zghz3138);
        CONVERT_OF(sail_int, mach_int)(&zghz3138, INT64_C(1));
        RECREATE(sail_int)(&zghz3139);
        sub_int(&zghz3139, zghz3137, zghz3138);
        zgaz3282 = CONVERT_OF(mach_int, sail_int)(zghz3139);
      }
      zgaz3283 = (safe_rshift(UINT64_MAX, 64 - 32) & (zcap_bits >> INT64_C(0)));
    }
    struct zCapability zghz3136;
    zghz3136.zB = CONVERT_OF(fbits, sbits)(zB, true);
    zghz3136.zE = zE;
    zghz3136.zT = CONVERT_OF(fbits, sbits)(zTshadowz38, true);
    zghz3136.zaddress = zgaz3283;
    zghz3136.zap_m = zgaz3277;
    zghz3136.zinternal_exponent = zinternal_exponent;
    zghz3136.zreserved_0 = zgaz3280;
    zghz3136.zreserved_1 = zgaz3281;
    zghz3136.zsd_perms = zgaz3276;
    zghz3136.zsealed = zgaz3279;
    zghz3136.ztag = ztag;
    zghz3134 = zghz3136;
    goto finish_match_572;
  }
case_573: ;
  sail_match_failure("bitsToCap");
finish_match_572: ;

end_function_574: ;
  return zghz3134;
end_block_exception_575: ;
  struct zCapability zcbz3105 = { .zB = UINT64_C(0xdeadc0de), .zE = INT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zap_m = UINT64_C(0xdeadc0de), .zinternal_exponent = false, .zreserved_0 = UINT64_C(0xdeadc0de), .zreserved_1 = UINT64_C(0xdeadc0de), .zsd_perms = UINT64_C(0xdeadc0de), .zsealed = false, .ztag = false };
  return zcbz3105;
}



static void finish_sailgen_bitsToCap(void)
{
  KILL(sail_int)(&zghz3175);
  KILL(sail_int)(&zghz3174);
  KILL(sail_int)(&zghz3173);
  KILL(sail_int)(&zghz3172);
  KILL(sail_int)(&zghz3171);
  KILL(sail_int)(&zghz3170);
  KILL(sail_int)(&zghz3169);
  KILL(lbits)(&zghz3168);
  KILL(sail_int)(&zghz3167);
  KILL(sail_int)(&zghz3166);
  KILL(sail_int)(&zghz3165);
  KILL(sail_int)(&zghz3164);
  KILL(sail_int)(&zghz3163);
  KILL(sail_int)(&zghz3162);
  KILL(lbits)(&zghz3161);
  KILL(sail_int)(&zghz3160);
  KILL(sail_int)(&zghz3159);
  KILL(lbits)(&zghz3158);
  KILL(lbits)(&zghz3157);
  KILL(lbits)(&zghz3156);
  KILL(sail_int)(&zghz3155);
  KILL(sail_int)(&zghz3154);
  KILL(sail_int)(&zghz3153);
  KILL(sail_int)(&zghz3152);
  KILL(sail_int)(&zghz3151);
  KILL(sail_int)(&zghz3150);
  KILL(lbits)(&zghz3149);
  KILL(sail_int)(&zghz3148);
  KILL(sail_int)(&zghz3147);
  KILL(lbits)(&zghz3146);
  KILL(lbits)(&zghz3145);
  KILL(lbits)(&zghz3144);
  KILL(lbits)(&zghz3143);
  KILL(lbits)(&zghz3142);
  KILL(lbits)(&zghz3141);
  KILL(lbits)(&zghz3140);
  KILL(sail_int)(&zghz3139);
  KILL(sail_int)(&zghz3138);
  KILL(sail_int)(&zghz3137);



}

static struct zEncMetadata sailgen_capToMetadataBits(struct zCapability);

struct zEncMetadata zghz3176;
struct zEncMetadata zghz3177;
struct zEncMetadata zghz3178;
struct zEncMetadata zghz3179;
struct zEncMetadata zghz3180;
struct zEncMetadata zghz3181;
struct zEncMetadata zghz3182;
sail_int zghz3183;
sail_int zghz3184;
sail_int zghz3185;
struct zEncMetadata zghz3186;
sail_int zghz3187;
sail_int zghz3188;
sail_int zghz3189;
struct zEncMetadata zghz3190;
struct zEncMetadata zghz3191;
struct zEncMetadata zghz3192;
struct zEncMetadata zghz3193;
sail_int zghz3194;
sail_int zghz3195;
sail_int zghz3196;
sail_int zghz3197;
sail_int zghz3198;
sail_int zghz3199;
sail_int zghz3200;
sail_int zghz3201;
sail_int zghz3202;
sail_int zghz3203;
sail_int zghz3204;
sail_int zghz3205;
sail_int zghz3206;
sail_int zghz3207;
sail_int zghz3208;
sail_int zghz3209;
sail_int zghz3210;
sail_int zghz3211;
sail_int zghz3212;
sail_int zghz3213;
lbits zghz3214;
sail_int zghz3215;
sail_int zghz3216;
sail_int zghz3217;

static void startup_sailgen_capToMetadataBits(void)
{







  CREATE(sail_int)(&zghz3183);
  CREATE(sail_int)(&zghz3184);
  CREATE(sail_int)(&zghz3185);

  CREATE(sail_int)(&zghz3187);
  CREATE(sail_int)(&zghz3188);
  CREATE(sail_int)(&zghz3189);




  CREATE(sail_int)(&zghz3194);
  CREATE(sail_int)(&zghz3195);
  CREATE(sail_int)(&zghz3196);
  CREATE(sail_int)(&zghz3197);
  CREATE(sail_int)(&zghz3198);
  CREATE(sail_int)(&zghz3199);
  CREATE(sail_int)(&zghz3200);
  CREATE(sail_int)(&zghz3201);
  CREATE(sail_int)(&zghz3202);
  CREATE(sail_int)(&zghz3203);
  CREATE(sail_int)(&zghz3204);
  CREATE(sail_int)(&zghz3205);
  CREATE(sail_int)(&zghz3206);
  CREATE(sail_int)(&zghz3207);
  CREATE(sail_int)(&zghz3208);
  CREATE(sail_int)(&zghz3209);
  CREATE(sail_int)(&zghz3210);
  CREATE(sail_int)(&zghz3211);
  CREATE(sail_int)(&zghz3212);
  CREATE(sail_int)(&zghz3213);
  CREATE(lbits)(&zghz3214);
  CREATE(sail_int)(&zghz3215);
  CREATE(sail_int)(&zghz3216);
  CREATE(sail_int)(&zghz3217);
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
  __label__ case_578, finish_match_577, end_function_579, end_block_exception_580;

  struct zEncMetadata zghz3176;
  uint64_t zE;
  {
    int64_t zgaz3346;
    {
      int64_t zgaz3345;
      zgaz3345 = zcap.zE;
      {
        RECREATE(sail_int)(&zghz3215);
        CONVERT_OF(sail_int, mach_int)(&zghz3215, zcap_max_E);
        RECREATE(sail_int)(&zghz3216);
        CONVERT_OF(sail_int, mach_int)(&zghz3216, zgaz3345);
        RECREATE(sail_int)(&zghz3217);
        sub_int(&zghz3217, zghz3215, zghz3216);
        zgaz3346 = CONVERT_OF(mach_int, sail_int)(zghz3217);
      }
    }
    {
      RECREATE(sail_int)(&zghz3212);
      CONVERT_OF(sail_int, mach_int)(&zghz3212, zcap_E_width);
      RECREATE(sail_int)(&zghz3213);
      CONVERT_OF(sail_int, mach_int)(&zghz3213, zgaz3346);
      RECREATE(lbits)(&zghz3214);
      sailgen_to_bits(&zghz3214, zghz3212, zghz3213);
      zE = CONVERT_OF(fbits, lbits)(zghz3214, true);
    }
  }
  struct ztuple_z8z5bitzCz0z5bv2zCz0z5bv2z9 zgaz3321;
  {
    bool zgaz3303;
    zgaz3303 = zcap.zinternal_exponent;
    if (zgaz3303) {
      fbits zgaz3308;
      {
        int64_t zgaz3304;
        {
          RECREATE(sail_int)(&zghz3209);
          CONVERT_OF(sail_int, mach_int)(&zghz3209, zcap_E_width);
          RECREATE(sail_int)(&zghz3210);
          CONVERT_OF(sail_int, mach_int)(&zghz3210, INT64_C(1));
          RECREATE(sail_int)(&zghz3211);
          sub_int(&zghz3211, zghz3209, zghz3210);
          zgaz3304 = CONVERT_OF(mach_int, sail_int)(zghz3211);
        }
        zgaz3308 = (UINT64_C(1) & (zE >> zgaz3304));
      }
      uint64_t zgaz3309;
      {
        int64_t zgaz3306;
        {
          int64_t zgaz3305;
          {
            RECREATE(sail_int)(&zghz3206);
            CONVERT_OF(sail_int, mach_int)(&zghz3206, zcap_xe_width);
            RECREATE(sail_int)(&zghz3207);
            CONVERT_OF(sail_int, mach_int)(&zghz3207, INT64_C(2));
            RECREATE(sail_int)(&zghz3208);
            mult_int(&zghz3208, zghz3206, zghz3207);
            zgaz3305 = CONVERT_OF(mach_int, sail_int)(zghz3208);
          }
          {
            RECREATE(sail_int)(&zghz3203);
            CONVERT_OF(sail_int, mach_int)(&zghz3203, zgaz3305);
            RECREATE(sail_int)(&zghz3204);
            CONVERT_OF(sail_int, mach_int)(&zghz3204, INT64_C(1));
            RECREATE(sail_int)(&zghz3205);
            sub_int(&zghz3205, zghz3203, zghz3204);
            zgaz3306 = CONVERT_OF(mach_int, sail_int)(zghz3205);
          }
        }
        zgaz3309 = (safe_rshift(UINT64_MAX, 64 - 2) & (zE >> zcap_xe_width));
      }
      uint64_t zgaz3310;
      {
        int64_t zgaz3307;
        {
          RECREATE(sail_int)(&zghz3200);
          CONVERT_OF(sail_int, mach_int)(&zghz3200, zcap_xe_width);
          RECREATE(sail_int)(&zghz3201);
          CONVERT_OF(sail_int, mach_int)(&zghz3201, INT64_C(1));
          RECREATE(sail_int)(&zghz3202);
          sub_int(&zghz3202, zghz3200, zghz3201);
          zgaz3307 = CONVERT_OF(mach_int, sail_int)(zghz3202);
        }
        zgaz3310 = (safe_rshift(UINT64_MAX, 64 - 2) & (zE >> INT64_C(0)));
      }
      struct ztuple_z8z5bitzCz0z5bv2zCz0z5bv2z9 zgsz3682;
      zgsz3682.ztup0 = zgaz3308;
      zgsz3682.ztup1 = zgaz3309;
      zgsz3682.ztup2 = zgaz3310;
      zgaz3321 = zgsz3682;
    } else {
      fbits zgaz3318;
      {
        uint64_t zgaz3313;
        {
          uint64_t zgaz3311;
          zgaz3311 = zcap.zT;
          uint64_t zgaz3312;
          zgaz3312 = zcap.zB;
          zgaz3313 = ((zgaz3311 - zgaz3312) & UINT64_C(0b1111111111));
        }
        zgaz3318 = (UINT64_C(1) & (zgaz3313 >> INT64_C(8)));
      }
      uint64_t zgaz3319;
      {
        uint64_t zgaz3314;
        zgaz3314 = zcap.zT;
        int64_t zgaz3315;
        {
          RECREATE(sail_int)(&zghz3197);
          CONVERT_OF(sail_int, mach_int)(&zghz3197, zcap_xe_width);
          RECREATE(sail_int)(&zghz3198);
          CONVERT_OF(sail_int, mach_int)(&zghz3198, INT64_C(1));
          RECREATE(sail_int)(&zghz3199);
          sub_int(&zghz3199, zghz3197, zghz3198);
          zgaz3315 = CONVERT_OF(mach_int, sail_int)(zghz3199);
        }
        zgaz3319 = (safe_rshift(UINT64_MAX, 64 - 2) & (zgaz3314 >> INT64_C(0)));
      }
      uint64_t zgaz3320;
      {
        uint64_t zgaz3316;
        zgaz3316 = zcap.zB;
        int64_t zgaz3317;
        {
          RECREATE(sail_int)(&zghz3194);
          CONVERT_OF(sail_int, mach_int)(&zghz3194, zcap_xe_width);
          RECREATE(sail_int)(&zghz3195);
          CONVERT_OF(sail_int, mach_int)(&zghz3195, INT64_C(1));
          RECREATE(sail_int)(&zghz3196);
          sub_int(&zghz3196, zghz3194, zghz3195);
          zgaz3317 = CONVERT_OF(mach_int, sail_int)(zghz3196);
        }
        zgaz3320 = (safe_rshift(UINT64_MAX, 64 - 2) & (zgaz3316 >> INT64_C(0)));
      }
      struct ztuple_z8z5bitzCz0z5bv2zCz0z5bv2z9 zgsz3681;
      zgsz3681.ztup0 = zgaz3318;
      zgsz3681.ztup1 = zgaz3319;
      zgsz3681.ztup2 = zgaz3320;
      zgaz3321 = zgsz3681;
    }
  }
  {
    fbits zL8;
    zL8 = zgaz3321.ztup0;
    uint64_t zTE;
    zTE = zgaz3321.ztup1;
    uint64_t zBE;
    zBE = zgaz3321.ztup2;
    int64_t zMW;
    zMW = zcap_mantissa_width;
    struct zEncMetadata zghz3177;
    {
      struct zEncMetadata zghz3178;
      {
        struct zEncMetadata zghz3179;
        {
          struct zEncMetadata zghz3180;
          {
            struct zEncMetadata zghz3181;
            {
              struct zEncMetadata zghz3182;
              {
                struct zEncMetadata zghz3186;
                {
                  struct zEncMetadata zghz3190;
                  {
                    struct zEncMetadata zghz3191;
                    {
                      struct zEncMetadata zghz3192;
                      {
                        uint64_t zgaz3322;
                        zgaz3322 = zcap.zreserved_0;
                        struct zEncMetadata zghz3193;
                        zghz3193.zbits = UINT64_C(0x00000000);
                        zghz3192 = sailgen__update_EncMetadata_reserved_0(zghz3193, zgaz3322);
                      }
                      uint64_t zgaz3324;
                      zgaz3324 = zcap.zreserved_1;
                      zghz3191 = sailgen__update_EncMetadata_reserved_1(zghz3192, zgaz3324);
                    }
                    zghz3190 = sailgen__update_EncMetadata_BE(zghz3191, zBE);
                  }
                  zghz3186 = sailgen__update_EncMetadata_TE(zghz3190, zTE);
                }
                uint64_t zgaz3330;
                {
                  uint64_t zgaz3327;
                  zgaz3327 = zcap.zB;
                  int64_t zgaz3328;
                  {
                    RECREATE(sail_int)(&zghz3187);
                    CONVERT_OF(sail_int, mach_int)(&zghz3187, zMW);
                    RECREATE(sail_int)(&zghz3188);
                    CONVERT_OF(sail_int, mach_int)(&zghz3188, INT64_C(1));
                    RECREATE(sail_int)(&zghz3189);
                    sub_int(&zghz3189, zghz3187, zghz3188);
                    zgaz3328 = CONVERT_OF(mach_int, sail_int)(zghz3189);
                  }
                  zgaz3330 = (safe_rshift(UINT64_MAX, 64 - 8) & (zgaz3327 >> zcap_xe_width));
                }
                zghz3182 = sailgen__update_EncMetadata_Bm(zghz3186, zgaz3330);
              }
              uint64_t zgaz3334;
              {
                uint64_t zgaz3331;
                zgaz3331 = zcap.zT;
                int64_t zgaz3332;
                {
                  RECREATE(sail_int)(&zghz3183);
                  CONVERT_OF(sail_int, mach_int)(&zghz3183, zMW);
                  RECREATE(sail_int)(&zghz3184);
                  CONVERT_OF(sail_int, mach_int)(&zghz3184, INT64_C(3));
                  RECREATE(sail_int)(&zghz3185);
                  sub_int(&zghz3185, zghz3183, zghz3184);
                  zgaz3332 = CONVERT_OF(mach_int, sail_int)(zghz3185);
                }
                zgaz3334 = (safe_rshift(UINT64_MAX, 64 - 6) & (zgaz3331 >> zcap_xe_width));
              }
              zghz3181 = sailgen__update_EncMetadata_Tm(zghz3182, zgaz3334);
            }
            uint64_t zgaz3337;
            {
              bool zgaz3335;
              zgaz3335 = zcap.zinternal_exponent;
              zgaz3337 = sailgen_internal_exponent_encdec_forwards(zgaz3335);
            }
            zghz3180 = sailgen__update_EncMetadata_EF(zghz3181, zgaz3337);
          }
          uint64_t zgaz3340;
          {
            bool zgaz3338;
            zgaz3338 = zcap.zsealed;
            zgaz3340 = sailgen_bool_bits_forwards(zgaz3338);
          }
          zghz3179 = sailgen__update_EncMetadata_CT(zghz3180, zgaz3340);
        }
        uint64_t zgaz3342;
        zgaz3342 = zcap.zap_m;
        zghz3178 = sailgen__update_EncMetadata_AP_M(zghz3179, zgaz3342);
      }
      uint64_t zgaz3344;
      zgaz3344 = zcap.zsd_perms;
      zghz3177 = sailgen__update_EncMetadata_SDP(zghz3178, zgaz3344);
    }
    uint64_t zgsz3685;
    zgsz3685 = UINT64_C(0b0);
    zgsz3685 = update_fbits(zgsz3685, INT64_C(0), zL8);
    zghz3176 = sailgen_setMetadataL8(zghz3177, zgsz3685);
    goto finish_match_577;
  }
case_578: ;
  sail_match_failure("capToMetadataBits");
finish_match_577: ;


end_function_579: ;
  return zghz3176;
end_block_exception_580: ;
  struct zEncMetadata zcbz3106 = { .zbits = UINT64_C(0xdeadc0de) };
  return zcbz3106;
}



static void finish_sailgen_capToMetadataBits(void)
{
  KILL(sail_int)(&zghz3217);
  KILL(sail_int)(&zghz3216);
  KILL(sail_int)(&zghz3215);
  KILL(lbits)(&zghz3214);
  KILL(sail_int)(&zghz3213);
  KILL(sail_int)(&zghz3212);
  KILL(sail_int)(&zghz3211);
  KILL(sail_int)(&zghz3210);
  KILL(sail_int)(&zghz3209);
  KILL(sail_int)(&zghz3208);
  KILL(sail_int)(&zghz3207);
  KILL(sail_int)(&zghz3206);
  KILL(sail_int)(&zghz3205);
  KILL(sail_int)(&zghz3204);
  KILL(sail_int)(&zghz3203);
  KILL(sail_int)(&zghz3202);
  KILL(sail_int)(&zghz3201);
  KILL(sail_int)(&zghz3200);
  KILL(sail_int)(&zghz3199);
  KILL(sail_int)(&zghz3198);
  KILL(sail_int)(&zghz3197);
  KILL(sail_int)(&zghz3196);
  KILL(sail_int)(&zghz3195);
  KILL(sail_int)(&zghz3194);




  KILL(sail_int)(&zghz3189);
  KILL(sail_int)(&zghz3188);
  KILL(sail_int)(&zghz3187);

  KILL(sail_int)(&zghz3185);
  KILL(sail_int)(&zghz3184);
  KILL(sail_int)(&zghz3183);







}

static struct zCapability znull_cap;
static void create_letbind_23(void) {


  struct zCapability zgsz3688;
  uint64_t zgaz3349;
  {
    uint64_t zgaz3348;
    {
      int64_t zgaz3347;
      {
        sail_int zgsz3955;
        CREATE(sail_int)(&zgsz3955);
        CONVERT_OF(sail_int, mach_int)(&zgsz3955, zcap_mantissa_width);
        sail_int zgsz3956;
        CREATE(sail_int)(&zgsz3956);
        CONVERT_OF(sail_int, mach_int)(&zgsz3956, INT64_C(2));
        sail_int zgsz3957;
        CREATE(sail_int)(&zgsz3957);
        sub_int(&zgsz3957, zgsz3955, zgsz3956);
        zgaz3347 = CONVERT_OF(mach_int, sail_int)(zgsz3957);
        KILL(sail_int)(&zgsz3957);
        KILL(sail_int)(&zgsz3956);
        KILL(sail_int)(&zgsz3955);
      }
      {
        sail_int zgsz3953;
        CREATE(sail_int)(&zgsz3953);
        CONVERT_OF(sail_int, mach_int)(&zgsz3953, zgaz3347);
        lbits zgsz3954;
        CREATE(lbits)(&zgsz3954);
        sailgen_zzeros_implicit(&zgsz3954, zgsz3953);
        zgaz3348 = CONVERT_OF(fbits, lbits)(zgsz3954, true);
        KILL(lbits)(&zgsz3954);
        KILL(sail_int)(&zgsz3953);
      }
    }
    zgaz3349 = (UINT64_C(0b01) << 8) | zgaz3348;
  }
  struct zCapability zgsz3687;
  zgsz3687.zB = UINT64_C(0b0000000000);
  zgsz3687.zE = zcap_max_E;
  zgsz3687.zT = zgaz3349;
  zgsz3687.zaddress = UINT64_C(0x00000000);
  zgsz3687.zap_m = znull_ap_m;
  zgsz3687.zinternal_exponent = true;
  zgsz3687.zreserved_0 = UINT64_C(0b000);
  zgsz3687.zreserved_1 = UINT64_C(0b0);
  zgsz3687.zsd_perms = UINT64_C(0b00);
  zgsz3687.zsealed = false;
  zgsz3687.ztag = false;
  zgsz3688 = zgsz3687;


  znull_cap = zgsz3688;

let_end_581: ;
}
static void kill_letbind_23(void) {
}

static struct zCapability zinfinite_cap;
static void create_letbind_24(void) {


  struct zCapability zgsz3690;
  struct zCapability zgsz3689;
  zgsz3689 = znull_cap;
  zgsz3689.zap_m = zinfinite_ap_m;
  zgsz3689.zsd_perms = UINT64_C(0b11);
  zgsz3689.ztag = true;
  zgsz3690 = zgsz3689;

  zinfinite_cap = zgsz3690;

let_end_582: ;
}
static void kill_letbind_24(void) {
}

static bool sailgen_boundsMalformed(struct zCapability);

sail_int zghz3218;
sail_int zghz3219;
sail_int zghz3220;
sail_int zghz3221;
sail_int zghz3222;
sail_int zghz3223;
sail_int zghz3224;
lbits zghz3225;

static void startup_sailgen_boundsMalformed(void)
{
  CREATE(sail_int)(&zghz3218);
  CREATE(sail_int)(&zghz3219);
  CREATE(sail_int)(&zghz3220);
  CREATE(sail_int)(&zghz3221);
  CREATE(sail_int)(&zghz3222);
  CREATE(sail_int)(&zghz3223);
  CREATE(sail_int)(&zghz3224);
  CREATE(lbits)(&zghz3225);
}

static bool sailgen_boundsMalformed(struct zCapability zcap)
{
  __label__ end_function_584, end_block_exception_585;

  bool zcbz362;
  bool zgaz3364;
  zgaz3364 = zcap.zinternal_exponent;
  bool zgsz3697;
  if (zgaz3364) {
    int64_t zMW;
    zMW = zcap_mantissa_width;
    bool zmalformedMSB;
    {
      bool zgaz3363;
      {
        bool zgaz3356;
        {
          int64_t zgaz3353;
          zgaz3353 = zcap.zE;
          zgaz3356 = (zgaz3353 == zcap_max_E);
        }
        bool zgsz3691;
        if (zgaz3356) {
          uint64_t zgaz3354;
          zgaz3354 = zcap.zB;
          uint64_t zgaz3355;
          {
            RECREATE(sail_int)(&zghz3224);
            CONVERT_OF(sail_int, mach_int)(&zghz3224, zMW);
            RECREATE(lbits)(&zghz3225);
            sailgen_zzeros_implicit(&zghz3225, zghz3224);
            zgaz3355 = CONVERT_OF(fbits, lbits)(zghz3225, true);
          }
          zgsz3691 = (zgaz3354 != zgaz3355);
        } else {  zgsz3691 = false;  }
        zgaz3363 = zgsz3691;
      }
      bool zgsz3693;
      if (zgaz3363) {  zgsz3693 = true;  } else {
        bool zgaz3362;
        {
          int64_t zgaz3357;
          zgaz3357 = zcap.zE;
          int64_t zgaz3358;
          {
            RECREATE(sail_int)(&zghz3221);
            CONVERT_OF(sail_int, mach_int)(&zghz3221, zcap_max_E);
            RECREATE(sail_int)(&zghz3222);
            CONVERT_OF(sail_int, mach_int)(&zghz3222, INT64_C(1));
            RECREATE(sail_int)(&zghz3223);
            sub_int(&zghz3223, zghz3221, zghz3222);
            zgaz3358 = CONVERT_OF(mach_int, sail_int)(zghz3223);
          }
          zgaz3362 = (zgaz3357 == zgaz3358);
        }
        bool zgsz3692;
        if (zgaz3362) {
          fbits zgaz3361;
          {
            uint64_t zgaz3359;
            zgaz3359 = zcap.zB;
            int64_t zgaz3360;
            {
              RECREATE(sail_int)(&zghz3218);
              CONVERT_OF(sail_int, mach_int)(&zghz3218, zMW);
              RECREATE(sail_int)(&zghz3219);
              CONVERT_OF(sail_int, mach_int)(&zghz3219, INT64_C(1));
              RECREATE(sail_int)(&zghz3220);
              sub_int(&zghz3220, zghz3218, zghz3219);
              zgaz3360 = CONVERT_OF(mach_int, sail_int)(zghz3220);
            }
            zgaz3361 = (UINT64_C(1) & (zgaz3359 >> zgaz3360));
          }
          zgsz3692 = sailgen_neq_anythingzItzK(zgaz3361, UINT64_C(0));
        } else {  zgsz3692 = false;  }
        zgsz3693 = zgsz3692;
      }
      zmalformedMSB = zgsz3693;
    }
    bool zmalformedLSB;
    {
      bool zgaz3352;
      {
        int64_t zgaz3350;
        zgaz3350 = zcap.zE;
        zgaz3352 = (zgaz3350 < INT64_C(0));
      }
      bool zgsz3695;
      if (zgaz3352) {  zgsz3695 = true;  } else {
        bool zgsz3694;
        if (zcap_use_L8) {
          int64_t zgaz3351;
          zgaz3351 = zcap.zE;
          zgsz3694 = (zgaz3351 == INT64_C(0));
        } else {  zgsz3694 = false;  }
        zgsz3695 = zgsz3694;
      }
      zmalformedLSB = zgsz3695;
    }
    bool zgsz3696;
    if (zmalformedMSB) {  zgsz3696 = true;  } else {  zgsz3696 = zmalformedLSB;  }
    zgsz3697 = zgsz3696;
  } else {  zgsz3697 = false;  }
  zcbz362 = zgsz3697;

end_function_584: ;
  return zcbz362;
end_block_exception_585: ;

  return false;
}



static void finish_sailgen_boundsMalformed(void)
{
  KILL(lbits)(&zghz3225);
  KILL(sail_int)(&zghz3224);
  KILL(sail_int)(&zghz3223);
  KILL(sail_int)(&zghz3222);
  KILL(sail_int)(&zghz3221);
  KILL(sail_int)(&zghz3220);
  KILL(sail_int)(&zghz3219);
  KILL(sail_int)(&zghz3218);
}

static void sailgen_getCapBoundsBits(struct zoptionzIz8bzCbz9zK *rop, struct zCapability);

sail_int zghz3226;
sail_int zghz3227;
sail_int zghz3228;
lbits zghz3229;
sail_int zghz3230;
lbits zghz3231;
lbits zghz3232;
lbits zghz3233;
sail_int zghz3234;
sail_int zghz3235;
sail_int zghz3236;
sail_int zghz3237;
sail_int zghz3238;
sail_int zghz3239;
sail_int zghz3240;
sail_int zghz3241;
sail_int zghz3242;
lbits zghz3243;
sail_int zghz3244;
lbits zghz3245;
lbits zghz3246;
lbits zghz3247;
sail_int zghz3248;
lbits zghz3249;
lbits zghz3250;
sail_int zghz3251;
lbits zghz3252;
lbits zghz3253;
sail_int zghz3254;
lbits zghz3255;
lbits zghz3256;
lbits zghz3257;
sail_int zghz3258;
lbits zghz3259;
lbits zghz3260;
sail_int zghz3261;
lbits zghz3262;
lbits zghz3263;
sail_int zghz3264;
lbits zghz3265;
sail_int zghz3266;
sail_int zghz3267;
sail_int zghz3268;
sail_int zghz3269;
sail_int zghz3270;
sail_int zghz3271;
lbits zghz3272;
lbits zghz3273;
lbits zghz3274;
lbits zghz3275;
lbits zghz3276;
lbits zghz3277;
sail_int zghz3278;
lbits zghz3279;
sail_int zghz3280;
sail_int zghz3281;
sail_int zghz3282;
lbits zghz3283;
sail_int zghz3284;
lbits zghz3285;
lbits zghz3286;
sail_int zghz3287;
lbits zghz3288;

static void startup_sailgen_getCapBoundsBits(void)
{
  CREATE(sail_int)(&zghz3226);
  CREATE(sail_int)(&zghz3227);
  CREATE(sail_int)(&zghz3228);
  CREATE(lbits)(&zghz3229);
  CREATE(sail_int)(&zghz3230);
  CREATE(lbits)(&zghz3231);
  CREATE(lbits)(&zghz3232);
  CREATE(lbits)(&zghz3233);
  CREATE(sail_int)(&zghz3234);
  CREATE(sail_int)(&zghz3235);
  CREATE(sail_int)(&zghz3236);
  CREATE(sail_int)(&zghz3237);
  CREATE(sail_int)(&zghz3238);
  CREATE(sail_int)(&zghz3239);
  CREATE(sail_int)(&zghz3240);
  CREATE(sail_int)(&zghz3241);
  CREATE(sail_int)(&zghz3242);
  CREATE(lbits)(&zghz3243);
  CREATE(sail_int)(&zghz3244);
  CREATE(lbits)(&zghz3245);
  CREATE(lbits)(&zghz3246);
  CREATE(lbits)(&zghz3247);
  CREATE(sail_int)(&zghz3248);
  CREATE(lbits)(&zghz3249);
  CREATE(lbits)(&zghz3250);
  CREATE(sail_int)(&zghz3251);
  CREATE(lbits)(&zghz3252);
  CREATE(lbits)(&zghz3253);
  CREATE(sail_int)(&zghz3254);
  CREATE(lbits)(&zghz3255);
  CREATE(lbits)(&zghz3256);
  CREATE(lbits)(&zghz3257);
  CREATE(sail_int)(&zghz3258);
  CREATE(lbits)(&zghz3259);
  CREATE(lbits)(&zghz3260);
  CREATE(sail_int)(&zghz3261);
  CREATE(lbits)(&zghz3262);
  CREATE(lbits)(&zghz3263);
  CREATE(sail_int)(&zghz3264);
  CREATE(lbits)(&zghz3265);
  CREATE(sail_int)(&zghz3266);
  CREATE(sail_int)(&zghz3267);
  CREATE(sail_int)(&zghz3268);
  CREATE(sail_int)(&zghz3269);
  CREATE(sail_int)(&zghz3270);
  CREATE(sail_int)(&zghz3271);
  CREATE(lbits)(&zghz3272);
  CREATE(lbits)(&zghz3273);
  CREATE(lbits)(&zghz3274);
  CREATE(lbits)(&zghz3275);
  CREATE(lbits)(&zghz3276);
  CREATE(lbits)(&zghz3277);
  CREATE(sail_int)(&zghz3278);
  CREATE(lbits)(&zghz3279);
  CREATE(sail_int)(&zghz3280);
  CREATE(sail_int)(&zghz3281);
  CREATE(sail_int)(&zghz3282);
  CREATE(lbits)(&zghz3283);
  CREATE(sail_int)(&zghz3284);
  CREATE(lbits)(&zghz3285);
  CREATE(lbits)(&zghz3286);
  CREATE(sail_int)(&zghz3287);
  CREATE(lbits)(&zghz3288);
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

static void sailgen_getCapBoundsBits(struct zoptionzIz8bzCbz9zK *zcbz363, struct zCapability zc)
{
  __label__ end_function_587, end_block_exception_590, end_function_707;

  {
    bool zgaz3365;
    zgaz3365 = sailgen_boundsMalformed(zc);
    unit zgsz3698;
    if (zgaz3365) {
      __label__ cleanup_588, end_cleanup_589;

      struct zoptionzIz8bzCbz9zK zgaz3366;
      CREATE(zoptionzIz8bzCbz9zK)(&zgaz3366);
      sailgen_NonezIz8bzCbz9zK(&zgaz3366, UNIT);
      COPY(zoptionzIz8bzCbz9zK)((*(&zcbz363)), zgaz3366);
      goto cleanup_588;
      /* unreachable after return */
      KILL(zoptionzIz8bzCbz9zK)(&zgaz3366);
      goto end_cleanup_589;
    cleanup_588: ;
      KILL(zoptionzIz8bzCbz9zK)(&zgaz3366);
      goto end_function_587;
    end_cleanup_589: ;
    } else {  zgsz3698 = UNIT;  }
  }
  int64_t zE;
  zE = zc.zE;
  {
    bool zgaz3367;
    zgaz3367 = (zE >= INT64_C(0));
    unit zgsz3699;
    zgsz3699 = sail_assert(zgaz3367, "Cap is malformed if E<0");
  }
  uint64_t za;
  za = zc.zaddress;
  uint64_t za_mid;
  {
    uint64_t zgaz3399;
    {
      RECREATE(lbits)(&zghz3286);
      CONVERT_OF(lbits, fbits)(&zghz3286, za, UINT64_C(32) , true);
      RECREATE(sail_int)(&zghz3287);
      CONVERT_OF(sail_int, mach_int)(&zghz3287, zE);
      RECREATE(lbits)(&zghz3288);
      shiftr(&zghz3288, zghz3286, zghz3287);
      zgaz3399 = CONVERT_OF(fbits, lbits)(zghz3288, true);
    }
    {
      RECREATE(lbits)(&zghz3283);
      CONVERT_OF(lbits, fbits)(&zghz3283, zgaz3399, UINT64_C(32) , true);
      RECREATE(sail_int)(&zghz3284);
      CONVERT_OF(sail_int, mach_int)(&zghz3284, zcap_mantissa_width);
      RECREATE(lbits)(&zghz3285);
      sail_truncate(&zghz3285, zghz3283, zghz3284);
      za_mid = CONVERT_OF(fbits, lbits)(zghz3285, true);
    }
  }
  uint64_t zR;
  {
    uint64_t zgaz3397;
    zgaz3397 = zc.zB;
    uint64_t zgaz3398;
    {
      uint64_t zgaz3396;
      {
        int64_t zgaz3395;
        {
          RECREATE(sail_int)(&zghz3280);
          CONVERT_OF(sail_int, mach_int)(&zghz3280, zcap_mantissa_width);
          RECREATE(sail_int)(&zghz3281);
          CONVERT_OF(sail_int, mach_int)(&zghz3281, INT64_C(2));
          RECREATE(sail_int)(&zghz3282);
          sub_int(&zghz3282, zghz3280, zghz3281);
          zgaz3395 = CONVERT_OF(mach_int, sail_int)(zghz3282);
        }
        {
          RECREATE(sail_int)(&zghz3278);
          CONVERT_OF(sail_int, mach_int)(&zghz3278, zgaz3395);
          RECREATE(lbits)(&zghz3279);
          sailgen_zzeros_implicit(&zghz3279, zghz3278);
          zgaz3396 = CONVERT_OF(fbits, lbits)(zghz3279, true);
        }
      }
      zgaz3398 = (UINT64_C(0b01) << 8) | zgaz3396;
    }
    zR = ((zgaz3397 - zgaz3398) & UINT64_C(0b1111111111));
  }
  int64_t zaHi;
  {
    bool zgaz3394;
    {
      RECREATE(lbits)(&zghz3276);
      CONVERT_OF(lbits, fbits)(&zghz3276, za_mid, UINT64_C(10) , true);
      RECREATE(lbits)(&zghz3277);
      CONVERT_OF(lbits, fbits)(&zghz3277, zR, UINT64_C(10) , true);
      zgaz3394 = sailgen_z8operatorz0zI_uz9(zghz3276, zghz3277);
    }
    if (zgaz3394) {  zaHi = INT64_C(1);  } else {  zaHi = INT64_C(0);  }
  }
  int64_t zbHi;
  {
    bool zgaz3393;
    {
      uint64_t zgaz3392;
      zgaz3392 = zc.zB;
      {
        RECREATE(lbits)(&zghz3274);
        CONVERT_OF(lbits, fbits)(&zghz3274, zgaz3392, UINT64_C(10) , true);
        RECREATE(lbits)(&zghz3275);
        CONVERT_OF(lbits, fbits)(&zghz3275, zR, UINT64_C(10) , true);
        zgaz3393 = sailgen_z8operatorz0zI_uz9(zghz3274, zghz3275);
      }
    }
    if (zgaz3393) {  zbHi = INT64_C(1);  } else {  zbHi = INT64_C(0);  }
  }
  int64_t ztHi;
  {
    bool zgaz3391;
    {
      uint64_t zgaz3390;
      zgaz3390 = zc.zT;
      {
        RECREATE(lbits)(&zghz3272);
        CONVERT_OF(lbits, fbits)(&zghz3272, zgaz3390, UINT64_C(10) , true);
        RECREATE(lbits)(&zghz3273);
        CONVERT_OF(lbits, fbits)(&zghz3273, zR, UINT64_C(10) , true);
        zgaz3391 = sailgen_z8operatorz0zI_uz9(zghz3272, zghz3273);
      }
    }
    if (zgaz3391) {  ztHi = INT64_C(1);  } else {  ztHi = INT64_C(0);  }
  }
  int64_t zcorrection_base;
  {
    RECREATE(sail_int)(&zghz3269);
    CONVERT_OF(sail_int, mach_int)(&zghz3269, zbHi);
    RECREATE(sail_int)(&zghz3270);
    CONVERT_OF(sail_int, mach_int)(&zghz3270, zaHi);
    RECREATE(sail_int)(&zghz3271);
    sub_int(&zghz3271, zghz3269, zghz3270);
    zcorrection_base = CONVERT_OF(mach_int, sail_int)(zghz3271);
  }
  int64_t zcorrection_top;
  {
    RECREATE(sail_int)(&zghz3266);
    CONVERT_OF(sail_int, mach_int)(&zghz3266, ztHi);
    RECREATE(sail_int)(&zghz3267);
    CONVERT_OF(sail_int, mach_int)(&zghz3267, zaHi);
    RECREATE(sail_int)(&zghz3268);
    sub_int(&zghz3268, zghz3266, zghz3267);
    zcorrection_top = CONVERT_OF(mach_int, sail_int)(zghz3268);
  }
  uint64_t za_top;
  {
    int64_t zgaz3389;
    zgaz3389 = (zE + zcap_mantissa_width);
    {
      RECREATE(lbits)(&zghz3263);
      CONVERT_OF(lbits, fbits)(&zghz3263, za, UINT64_C(32) , true);
      RECREATE(sail_int)(&zghz3264);
      CONVERT_OF(sail_int, mach_int)(&zghz3264, zgaz3389);
      RECREATE(lbits)(&zghz3265);
      shiftr(&zghz3265, zghz3263, zghz3264);
      za_top = CONVERT_OF(fbits, lbits)(zghz3265, true);
    }
  }
  uint64_t zbase;
  {
    RECREATE(lbits)(&zghz3253);
    {
      uint64_t zgaz3386;
      {
        RECREATE(lbits)(&zghz3260);
        CONVERT_OF(lbits, fbits)(&zghz3260, za_top, UINT64_C(32) , true);
        RECREATE(sail_int)(&zghz3261);
        CONVERT_OF(sail_int, mach_int)(&zghz3261, zcorrection_base);
        RECREATE(lbits)(&zghz3262);
        add_bits_int(&zghz3262, zghz3260, zghz3261);
        zgaz3386 = CONVERT_OF(fbits, lbits)(zghz3262, true);
      }
      sbits zgaz3387;
      {
        uint64_t zgaz3384;
        zgaz3384 = zc.zB;
        sbits zgaz3385;
        {
          RECREATE(sail_int)(&zghz3258);
          CONVERT_OF(sail_int, mach_int)(&zghz3258, zE);
          RECREATE(lbits)(&zghz3259);
          sailgen_zzeros_implicit(&zghz3259, zghz3258);
          zgaz3385 = CONVERT_OF(sbits, lbits)(zghz3259, true);
        }
        zgaz3387 = append_fs(zgaz3384, 10, zgaz3385);
      }
      {
        RECREATE(lbits)(&zghz3256);
        CONVERT_OF(lbits, fbits)(&zghz3256, zgaz3386, UINT64_C(32) , true);
        RECREATE(lbits)(&zghz3257);
        CONVERT_OF(lbits, sbits)(&zghz3257, zgaz3387, true);
        append(&zghz3253, zghz3256, zghz3257);
      }
    }
    {
      RECREATE(sail_int)(&zghz3254);
      CONVERT_OF(sail_int, mach_int)(&zghz3254, zcap_len_width);
      RECREATE(lbits)(&zghz3255);
      sail_truncate(&zghz3255, zghz3253, zghz3254);
      zbase = CONVERT_OF(fbits, lbits)(zghz3255, true);
    }
  }
  uint64_t ztop;
  {
    RECREATE(lbits)(&zghz3243);
    {
      uint64_t zgaz3381;
      {
        RECREATE(lbits)(&zghz3250);
        CONVERT_OF(lbits, fbits)(&zghz3250, za_top, UINT64_C(32) , true);
        RECREATE(sail_int)(&zghz3251);
        CONVERT_OF(sail_int, mach_int)(&zghz3251, zcorrection_top);
        RECREATE(lbits)(&zghz3252);
        add_bits_int(&zghz3252, zghz3250, zghz3251);
        zgaz3381 = CONVERT_OF(fbits, lbits)(zghz3252, true);
      }
      sbits zgaz3382;
      {
        uint64_t zgaz3379;
        zgaz3379 = zc.zT;
        sbits zgaz3380;
        {
          RECREATE(sail_int)(&zghz3248);
          CONVERT_OF(sail_int, mach_int)(&zghz3248, zE);
          RECREATE(lbits)(&zghz3249);
          sailgen_zzeros_implicit(&zghz3249, zghz3248);
          zgaz3380 = CONVERT_OF(sbits, lbits)(zghz3249, true);
        }
        zgaz3382 = append_fs(zgaz3379, 10, zgaz3380);
      }
      {
        RECREATE(lbits)(&zghz3246);
        CONVERT_OF(lbits, fbits)(&zghz3246, zgaz3381, UINT64_C(32) , true);
        RECREATE(lbits)(&zghz3247);
        CONVERT_OF(lbits, sbits)(&zghz3247, zgaz3382, true);
        append(&zghz3243, zghz3246, zghz3247);
      }
    }
    {
      RECREATE(sail_int)(&zghz3244);
      CONVERT_OF(sail_int, mach_int)(&zghz3244, zcap_len_width);
      RECREATE(lbits)(&zghz3245);
      sail_truncate(&zghz3245, zghz3243, zghz3244);
      ztop = CONVERT_OF(fbits, lbits)(zghz3245, true);
    }
  }
  uint64_t zbase2;
  {
    uint64_t zgaz3378;
    {
      fbits zgaz3377;
      {
        int64_t zgaz3376;
        {
          RECREATE(sail_int)(&zghz3240);
          CONVERT_OF(sail_int, mach_int)(&zghz3240, zcap_addr_width);
          RECREATE(sail_int)(&zghz3241);
          CONVERT_OF(sail_int, mach_int)(&zghz3241, INT64_C(1));
          RECREATE(sail_int)(&zghz3242);
          sub_int(&zghz3242, zghz3240, zghz3241);
          zgaz3376 = CONVERT_OF(mach_int, sail_int)(zghz3242);
        }
        zgaz3377 = (UINT64_C(1) & (zbase >> zgaz3376));
      }
      zgaz3378 = UINT64_C(0b0);
      zgaz3378 = update_fbits(zgaz3378, INT64_C(0), zgaz3377);
    }
    zbase2 = (UINT64_C(0b0) << 1) | zgaz3378;
  }
  uint64_t ztop2;
  {
    int64_t zgaz3375;
    {
      RECREATE(sail_int)(&zghz3237);
      CONVERT_OF(sail_int, mach_int)(&zghz3237, zcap_addr_width);
      RECREATE(sail_int)(&zghz3238);
      CONVERT_OF(sail_int, mach_int)(&zghz3238, INT64_C(1));
      RECREATE(sail_int)(&zghz3239);
      sub_int(&zghz3239, zghz3237, zghz3238);
      zgaz3375 = CONVERT_OF(mach_int, sail_int)(zghz3239);
    }
    ztop2 = (safe_rshift(UINT64_MAX, 64 - 2) & (ztop >> zgaz3375));
  }
  {
    bool zgaz3371;
    {
      bool zgaz3370;
      {
        int64_t zgaz3368;
        {
          RECREATE(sail_int)(&zghz3234);
          CONVERT_OF(sail_int, mach_int)(&zghz3234, zcap_max_E);
          RECREATE(sail_int)(&zghz3235);
          CONVERT_OF(sail_int, mach_int)(&zghz3235, INT64_C(1));
          RECREATE(sail_int)(&zghz3236);
          sub_int(&zghz3236, zghz3234, zghz3235);
          zgaz3368 = CONVERT_OF(mach_int, sail_int)(zghz3236);
        }
        zgaz3370 = (zE < zgaz3368);
      }
      bool zgsz3701;
      if (zgaz3370) {
        uint64_t zgaz3369;
        zgaz3369 = ((ztop2 - zbase2) & UINT64_C(0b11));
        {
          RECREATE(lbits)(&zghz3232);
          CONVERT_OF(lbits, fbits)(&zghz3232, zgaz3369, UINT64_C(2) , true);
          RECREATE(lbits)(&zghz3233);
          CONVERT_OF(lbits, fbits)(&zghz3233, UINT64_C(0b01), UINT64_C(2) , true);
          zgsz3701 = sailgen_z8operatorz0zK_uz9(zghz3232, zghz3233);
        }
      } else {  zgsz3701 = false;  }
      zgaz3371 = zgsz3701;
    }
    unit zgsz3702;
    if (zgaz3371) {
      fbits zgaz3373;
      {
        fbits zgaz3372;
        zgaz3372 = (UINT64_C(1) & (ztop >> zcap_addr_width));
        zgaz3373 = sailgen_not_bit(zgaz3372);
      }
      {
        RECREATE(lbits)(&zghz3229);
        CONVERT_OF(lbits, fbits)(&zghz3229, ztop, UINT64_C(33) , true);
        RECREATE(sail_int)(&zghz3230);
        CONVERT_OF(sail_int, mach_int)(&zghz3230, zcap_addr_width);
        RECREATE(lbits)(&zghz3231);
        update_lbits(&zghz3231, zghz3229, zghz3230, zgaz3373);
        ztop = CONVERT_OF(fbits, lbits)(zghz3231, true);
      }
      zgsz3702 = UNIT;
    } else {  zgsz3702 = UNIT;  }
  }
  uint64_t zbaseshadowz39;
  {
    int64_t zgaz3374;
    {
      RECREATE(sail_int)(&zghz3226);
      CONVERT_OF(sail_int, mach_int)(&zghz3226, zcap_addr_width);
      RECREATE(sail_int)(&zghz3227);
      CONVERT_OF(sail_int, mach_int)(&zghz3227, INT64_C(1));
      RECREATE(sail_int)(&zghz3228);
      sub_int(&zghz3228, zghz3226, zghz3227);
      zgaz3374 = CONVERT_OF(mach_int, sail_int)(zghz3228);
    }
    zbaseshadowz39 = (safe_rshift(UINT64_MAX, 64 - 32) & (zbase >> INT64_C(0)));
  }
  struct ztuple_z8z5bv32zCz0z5bv33z9 zgsz3703;
  zgsz3703.ztup0 = zbaseshadowz39;
  zgsz3703.ztup1 = ztop;
  {
    struct ztuple_z8z5bvzCz0z5bvz9 zgsz31021;
    CREATE(ztuple_z8z5bvzCz0z5bvz9)(&zgsz31021);
    /* conversions */
    CONVERT_OF(lbits, fbits)(&((&zgsz31021)->ztup0), zgsz3703.ztup0, UINT64_C(32) , true);
    CONVERT_OF(lbits, fbits)(&((&zgsz31021)->ztup1), zgsz3703.ztup1, UINT64_C(33) , true);
    /* end conversions */
    sailgen_SomezIz8bzCbz9zK((*(&zcbz363)), zgsz31021);
    KILL(ztuple_z8z5bvzCz0z5bvz9)(&zgsz31021);
  }
















end_function_587: ;
  goto end_function_707;
end_block_exception_590: ;
  goto end_function_707;
end_function_707: ;
}



static void finish_sailgen_getCapBoundsBits(void)
{
  KILL(lbits)(&zghz3288);
  KILL(sail_int)(&zghz3287);
  KILL(lbits)(&zghz3286);
  KILL(lbits)(&zghz3285);
  KILL(sail_int)(&zghz3284);
  KILL(lbits)(&zghz3283);
  KILL(sail_int)(&zghz3282);
  KILL(sail_int)(&zghz3281);
  KILL(sail_int)(&zghz3280);
  KILL(lbits)(&zghz3279);
  KILL(sail_int)(&zghz3278);
  KILL(lbits)(&zghz3277);
  KILL(lbits)(&zghz3276);
  KILL(lbits)(&zghz3275);
  KILL(lbits)(&zghz3274);
  KILL(lbits)(&zghz3273);
  KILL(lbits)(&zghz3272);
  KILL(sail_int)(&zghz3271);
  KILL(sail_int)(&zghz3270);
  KILL(sail_int)(&zghz3269);
  KILL(sail_int)(&zghz3268);
  KILL(sail_int)(&zghz3267);
  KILL(sail_int)(&zghz3266);
  KILL(lbits)(&zghz3265);
  KILL(sail_int)(&zghz3264);
  KILL(lbits)(&zghz3263);
  KILL(lbits)(&zghz3262);
  KILL(sail_int)(&zghz3261);
  KILL(lbits)(&zghz3260);
  KILL(lbits)(&zghz3259);
  KILL(sail_int)(&zghz3258);
  KILL(lbits)(&zghz3257);
  KILL(lbits)(&zghz3256);
  KILL(lbits)(&zghz3255);
  KILL(sail_int)(&zghz3254);
  KILL(lbits)(&zghz3253);
  KILL(lbits)(&zghz3252);
  KILL(sail_int)(&zghz3251);
  KILL(lbits)(&zghz3250);
  KILL(lbits)(&zghz3249);
  KILL(sail_int)(&zghz3248);
  KILL(lbits)(&zghz3247);
  KILL(lbits)(&zghz3246);
  KILL(lbits)(&zghz3245);
  KILL(sail_int)(&zghz3244);
  KILL(lbits)(&zghz3243);
  KILL(sail_int)(&zghz3242);
  KILL(sail_int)(&zghz3241);
  KILL(sail_int)(&zghz3240);
  KILL(sail_int)(&zghz3239);
  KILL(sail_int)(&zghz3238);
  KILL(sail_int)(&zghz3237);
  KILL(sail_int)(&zghz3236);
  KILL(sail_int)(&zghz3235);
  KILL(sail_int)(&zghz3234);
  KILL(lbits)(&zghz3233);
  KILL(lbits)(&zghz3232);
  KILL(lbits)(&zghz3231);
  KILL(sail_int)(&zghz3230);
  KILL(lbits)(&zghz3229);
  KILL(sail_int)(&zghz3228);
  KILL(sail_int)(&zghz3227);
  KILL(sail_int)(&zghz3226);
}

static void sailgen_getCapBounds(struct zoptionzIz8izCiz9zK *rop, struct zCapability);

lbits zghz3289;
sail_int zghz3290;
lbits zghz3291;
sail_int zghz3292;

static void startup_sailgen_getCapBounds(void)
{
  CREATE(lbits)(&zghz3289);
  CREATE(sail_int)(&zghz3290);
  CREATE(lbits)(&zghz3291);
  CREATE(sail_int)(&zghz3292);
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


static void sailgen_getCapBounds(struct zoptionzIz8izCiz9zK *zcbz364, struct zCapability zcap)
{
  __label__ case_594, case_593, finish_match_592, end_function_595, end_block_exception_596, end_function_706;

  struct zoptionzIz8bzCbz9zK zgaz3400;
  CREATE(zoptionzIz8bzCbz9zK)(&zgaz3400);
  sailgen_getCapBoundsBits(&zgaz3400, zcap);
  struct zoptionzIz8izCiz9zK zgsz3704;
  CREATE(zoptionzIz8izCiz9zK)(&zgsz3704);
  {
    if (zgaz3400.kind != Kind_zNonezIz8bzCbz9zK) goto case_594;
    sailgen_NonezIz8izCiz9zK(&zgsz3704, UNIT);
    goto finish_match_592;
  }
case_594: ;
  {
    /* complete */
    uint64_t zbase;
    zbase = CONVERT_OF(fbits, lbits)(zgaz3400.zSomezIz8bzCbz9zK.ztup0, true);
    uint64_t ztop;
    ztop = CONVERT_OF(fbits, lbits)(zgaz3400.zSomezIz8bzCbz9zK.ztup1, true);
    struct ztuple_z8z5i64zCz0z5i64z9 zgaz3403;
    {
      int64_t zgaz3401;
      {
        RECREATE(lbits)(&zghz3291);
        CONVERT_OF(lbits, fbits)(&zghz3291, zbase, UINT64_C(32) , true);
        RECREATE(sail_int)(&zghz3292);
        sail_unsigned(&zghz3292, zghz3291);
        zgaz3401 = CONVERT_OF(mach_int, sail_int)(zghz3292);
      }
      int64_t zgaz3402;
      {
        RECREATE(lbits)(&zghz3289);
        CONVERT_OF(lbits, fbits)(&zghz3289, ztop, UINT64_C(33) , true);
        RECREATE(sail_int)(&zghz3290);
        sail_unsigned(&zghz3290, zghz3289);
        zgaz3402 = CONVERT_OF(mach_int, sail_int)(zghz3290);
      }
      zgaz3403.ztup0 = zgaz3401;
      zgaz3403.ztup1 = zgaz3402;
    }
    {
      struct ztuple_z8z5izCz0z5iz9 zgsz31032;
      CREATE(ztuple_z8z5izCz0z5iz9)(&zgsz31032);
      /* conversions */
      CONVERT_OF(sail_int, mach_int)(&((&zgsz31032)->ztup0), zgaz3403.ztup0);
      CONVERT_OF(sail_int, mach_int)(&((&zgsz31032)->ztup1), zgaz3403.ztup1);
      /* end conversions */
      sailgen_SomezIz8izCiz9zK(&zgsz3704, zgsz31032);
      KILL(ztuple_z8z5izCz0z5iz9)(&zgsz31032);
    }
    goto finish_match_592;
  }
case_593: ;
finish_match_592: ;
  COPY(zoptionzIz8izCiz9zK)((*(&zcbz364)), zgsz3704);
  KILL(zoptionzIz8izCiz9zK)(&zgsz3704);
  KILL(zoptionzIz8bzCbz9zK)(&zgaz3400);
end_function_595: ;
  goto end_function_706;
end_block_exception_596: ;
  goto end_function_706;
end_function_706: ;
}



static void finish_sailgen_getCapBounds(void)
{
  KILL(sail_int)(&zghz3292);
  KILL(lbits)(&zghz3291);
  KILL(sail_int)(&zghz3290);
  KILL(lbits)(&zghz3289);
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

struct zCapability zghz3293;
sail_int zghz3294;
lbits zghz3295;
sail_int zghz3296;
lbits zghz3297;
lbits zghz3298;
sail_int zghz3299;
lbits zghz3300;
lbits zghz3301;
sail_int zghz3302;
lbits zghz3303;
sail_int zghz3304;
sail_int zghz3305;
sail_int zghz3306;
lbits zghz3307;
sail_int zghz3308;
lbits zghz3309;
lbits zghz3310;
sail_int zghz3311;
lbits zghz3312;
sail_int zghz3313;
sail_int zghz3314;
sail_int zghz3315;
lbits zghz3316;
sail_int zghz3317;
lbits zghz3318;
sail_int zghz3319;
sail_int zghz3320;
sail_int zghz3321;
lbits zghz3322;
sail_int zghz3323;
lbits zghz3324;
sail_int zghz3325;
lbits zghz3326;
lbits zghz3327;
sail_int zghz3328;
lbits zghz3329;
lbits zghz3330;
sail_int zghz3331;
lbits zghz3332;
sail_int zghz3333;
sail_int zghz3334;
sail_int zghz3335;
lbits zghz3336;
sail_int zghz3337;
lbits zghz3338;
lbits zghz3339;
sail_int zghz3340;
lbits zghz3341;
sail_int zghz3342;
sail_int zghz3343;
sail_int zghz3344;
lbits zghz3345;
sail_int zghz3346;
lbits zghz3347;
lbits zghz3348;
sail_int zghz3349;
lbits zghz3350;
lbits zghz3351;
sail_int zghz3352;
lbits zghz3353;
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

static void startup_sailgen_setCapBounds(void)
{

  CREATE(sail_int)(&zghz3294);
  CREATE(lbits)(&zghz3295);
  CREATE(sail_int)(&zghz3296);
  CREATE(lbits)(&zghz3297);
  CREATE(lbits)(&zghz3298);
  CREATE(sail_int)(&zghz3299);
  CREATE(lbits)(&zghz3300);
  CREATE(lbits)(&zghz3301);
  CREATE(sail_int)(&zghz3302);
  CREATE(lbits)(&zghz3303);
  CREATE(sail_int)(&zghz3304);
  CREATE(sail_int)(&zghz3305);
  CREATE(sail_int)(&zghz3306);
  CREATE(lbits)(&zghz3307);
  CREATE(sail_int)(&zghz3308);
  CREATE(lbits)(&zghz3309);
  CREATE(lbits)(&zghz3310);
  CREATE(sail_int)(&zghz3311);
  CREATE(lbits)(&zghz3312);
  CREATE(sail_int)(&zghz3313);
  CREATE(sail_int)(&zghz3314);
  CREATE(sail_int)(&zghz3315);
  CREATE(lbits)(&zghz3316);
  CREATE(sail_int)(&zghz3317);
  CREATE(lbits)(&zghz3318);
  CREATE(sail_int)(&zghz3319);
  CREATE(sail_int)(&zghz3320);
  CREATE(sail_int)(&zghz3321);
  CREATE(lbits)(&zghz3322);
  CREATE(sail_int)(&zghz3323);
  CREATE(lbits)(&zghz3324);
  CREATE(sail_int)(&zghz3325);
  CREATE(lbits)(&zghz3326);
  CREATE(lbits)(&zghz3327);
  CREATE(sail_int)(&zghz3328);
  CREATE(lbits)(&zghz3329);
  CREATE(lbits)(&zghz3330);
  CREATE(sail_int)(&zghz3331);
  CREATE(lbits)(&zghz3332);
  CREATE(sail_int)(&zghz3333);
  CREATE(sail_int)(&zghz3334);
  CREATE(sail_int)(&zghz3335);
  CREATE(lbits)(&zghz3336);
  CREATE(sail_int)(&zghz3337);
  CREATE(lbits)(&zghz3338);
  CREATE(lbits)(&zghz3339);
  CREATE(sail_int)(&zghz3340);
  CREATE(lbits)(&zghz3341);
  CREATE(sail_int)(&zghz3342);
  CREATE(sail_int)(&zghz3343);
  CREATE(sail_int)(&zghz3344);
  CREATE(lbits)(&zghz3345);
  CREATE(sail_int)(&zghz3346);
  CREATE(lbits)(&zghz3347);
  CREATE(lbits)(&zghz3348);
  CREATE(sail_int)(&zghz3349);
  CREATE(lbits)(&zghz3350);
  CREATE(lbits)(&zghz3351);
  CREATE(sail_int)(&zghz3352);
  CREATE(lbits)(&zghz3353);
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
}

static struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 sailgen_setCapBounds(struct zCapability zcap, uint64_t zbase, uint64_t ztop)
{
  __label__ end_function_598, end_block_exception_599;

  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz365;
  uint64_t zext_base;
  zext_base = (UINT64_C(0b0) << 32) | zbase;
  uint64_t zlength;
  zlength = ((ztop - zext_base) & UINT64_C(0b111111111111111111111111111111111));
  int64_t ze;
  {
    int64_t zgaz3441;
    {
      uint64_t zgaz3440;
      {
        int64_t zgaz3439;
        {
          RECREATE(sail_int)(&zghz3364);
          CONVERT_OF(sail_int, mach_int)(&zghz3364, zcap_mantissa_width);
          RECREATE(sail_int)(&zghz3365);
          CONVERT_OF(sail_int, mach_int)(&zghz3365, INT64_C(1));
          RECREATE(sail_int)(&zghz3366);
          sub_int(&zghz3366, zghz3364, zghz3365);
          zgaz3439 = CONVERT_OF(mach_int, sail_int)(zghz3366);
        }
        zgaz3440 = (safe_rshift(UINT64_MAX, 64 - 24) & (zlength >> zgaz3439));
      }
      {
        RECREATE(lbits)(&zghz3362);
        CONVERT_OF(lbits, fbits)(&zghz3362, zgaz3440, UINT64_C(24) , true);
        RECREATE(sail_int)(&zghz3363);
        count_leading_zeros(&zghz3363, zghz3362);
        zgaz3441 = CONVERT_OF(mach_int, sail_int)(zghz3363);
      }
    }
    {
      RECREATE(sail_int)(&zghz3359);
      CONVERT_OF(sail_int, mach_int)(&zghz3359, zcap_max_E);
      RECREATE(sail_int)(&zghz3360);
      CONVERT_OF(sail_int, mach_int)(&zghz3360, zgaz3441);
      RECREATE(sail_int)(&zghz3361);
      sub_int(&zghz3361, zghz3359, zghz3360);
      ze = CONVERT_OF(mach_int, sail_int)(zghz3361);
    }
  }
  bool zie;
  {
    bool zgaz3438;
    {
      RECREATE(sail_int)(&zghz3357);
      CONVERT_OF(sail_int, mach_int)(&zghz3357, ze);
      RECREATE(sail_int)(&zghz3358);
      CONVERT_OF(sail_int, mach_int)(&zghz3358, INT64_C(0));
      zgaz3438 = sailgen_neq_int(zghz3357, zghz3358);
    }
    bool zgsz3709;
    if (zgaz3438) {  zgsz3709 = true;  } else {
      bool zgaz3437;
      zgaz3437 = not(zcap_use_L8);
      bool zgsz3708;
      if (zgaz3437) {
        fbits zgaz3436;
        {
          int64_t zgaz3435;
          {
            RECREATE(sail_int)(&zghz3354);
            CONVERT_OF(sail_int, mach_int)(&zghz3354, zcap_mantissa_width);
            RECREATE(sail_int)(&zghz3355);
            CONVERT_OF(sail_int, mach_int)(&zghz3355, INT64_C(2));
            RECREATE(sail_int)(&zghz3356);
            sub_int(&zghz3356, zghz3354, zghz3355);
            zgaz3435 = CONVERT_OF(mach_int, sail_int)(zghz3356);
          }
          zgaz3436 = (UINT64_C(1) & (zlength >> zgaz3435));
        }
        zgsz3708 = eq_bit(zgaz3436, UINT64_C(1));
      } else {  zgsz3708 = false;  }
      zgsz3709 = zgsz3708;
    }
    zie = zgsz3709;
  }
  uint64_t zBbits;
  {
    RECREATE(lbits)(&zghz3351);
    CONVERT_OF(lbits, fbits)(&zghz3351, zbase, UINT64_C(32) , true);
    RECREATE(sail_int)(&zghz3352);
    CONVERT_OF(sail_int, mach_int)(&zghz3352, zcap_mantissa_width);
    RECREATE(lbits)(&zghz3353);
    sail_truncate(&zghz3353, zghz3351, zghz3352);
    zBbits = CONVERT_OF(fbits, lbits)(zghz3353, true);
  }
  uint64_t zTbits;
  {
    RECREATE(lbits)(&zghz3348);
    CONVERT_OF(lbits, fbits)(&zghz3348, ztop, UINT64_C(33) , true);
    RECREATE(sail_int)(&zghz3349);
    CONVERT_OF(sail_int, mach_int)(&zghz3349, zcap_mantissa_width);
    RECREATE(lbits)(&zghz3350);
    sail_truncate(&zghz3350, zghz3348, zghz3349);
    zTbits = CONVERT_OF(fbits, lbits)(zghz3350, true);
  }
  bool zlostSignificantTop;
  zlostSignificantTop = false;
  bool zlostSignificantBase;
  zlostSignificantBase = false;
  bool zincE;
  zincE = false;
  {
    unit zgsz3710;
    if (zie) {
      uint64_t zB_ie;
      {
        uint64_t zgaz3429;
        {
          int64_t zgaz3428;
          zgaz3428 = (ze + zcap_xe_width);
          {
            RECREATE(lbits)(&zghz3345);
            CONVERT_OF(lbits, fbits)(&zghz3345, zbase, UINT64_C(32) , true);
            RECREATE(sail_int)(&zghz3346);
            CONVERT_OF(sail_int, mach_int)(&zghz3346, zgaz3428);
            RECREATE(lbits)(&zghz3347);
            shiftr(&zghz3347, zghz3345, zghz3346);
            zgaz3429 = CONVERT_OF(fbits, lbits)(zghz3347, true);
          }
        }
        int64_t zgaz3430;
        {
          RECREATE(sail_int)(&zghz3342);
          CONVERT_OF(sail_int, mach_int)(&zghz3342, zcap_mantissa_width);
          RECREATE(sail_int)(&zghz3343);
          CONVERT_OF(sail_int, mach_int)(&zghz3343, zcap_xe_width);
          RECREATE(sail_int)(&zghz3344);
          sub_int(&zghz3344, zghz3342, zghz3343);
          zgaz3430 = CONVERT_OF(mach_int, sail_int)(zghz3344);
        }
        {
          RECREATE(lbits)(&zghz3339);
          CONVERT_OF(lbits, fbits)(&zghz3339, zgaz3429, UINT64_C(32) , true);
          RECREATE(sail_int)(&zghz3340);
          CONVERT_OF(sail_int, mach_int)(&zghz3340, zgaz3430);
          RECREATE(lbits)(&zghz3341);
          sail_truncate(&zghz3341, zghz3339, zghz3340);
          zB_ie = CONVERT_OF(fbits, lbits)(zghz3341, true);
        }
      }
      uint64_t zT_ie;
      {
        uint64_t zgaz3426;
        {
          int64_t zgaz3425;
          zgaz3425 = (ze + zcap_xe_width);
          {
            RECREATE(lbits)(&zghz3336);
            CONVERT_OF(lbits, fbits)(&zghz3336, ztop, UINT64_C(33) , true);
            RECREATE(sail_int)(&zghz3337);
            CONVERT_OF(sail_int, mach_int)(&zghz3337, zgaz3425);
            RECREATE(lbits)(&zghz3338);
            shiftr(&zghz3338, zghz3336, zghz3337);
            zgaz3426 = CONVERT_OF(fbits, lbits)(zghz3338, true);
          }
        }
        int64_t zgaz3427;
        {
          RECREATE(sail_int)(&zghz3333);
          CONVERT_OF(sail_int, mach_int)(&zghz3333, zcap_mantissa_width);
          RECREATE(sail_int)(&zghz3334);
          CONVERT_OF(sail_int, mach_int)(&zghz3334, zcap_xe_width);
          RECREATE(sail_int)(&zghz3335);
          sub_int(&zghz3335, zghz3333, zghz3334);
          zgaz3427 = CONVERT_OF(mach_int, sail_int)(zghz3335);
        }
        {
          RECREATE(lbits)(&zghz3330);
          CONVERT_OF(lbits, fbits)(&zghz3330, zgaz3426, UINT64_C(33) , true);
          RECREATE(sail_int)(&zghz3331);
          CONVERT_OF(sail_int, mach_int)(&zghz3331, zgaz3427);
          RECREATE(lbits)(&zghz3332);
          sail_truncate(&zghz3332, zghz3330, zghz3331);
          zT_ie = CONVERT_OF(fbits, lbits)(zghz3332, true);
        }
      }
      uint64_t zmaskLo;
      {
        sbits zgaz3424;
        {
          int64_t zgaz3423;
          zgaz3423 = (ze + zcap_xe_width);
          {
            RECREATE(sail_int)(&zghz3328);
            CONVERT_OF(sail_int, mach_int)(&zghz3328, zgaz3423);
            RECREATE(lbits)(&zghz3329);
            sailgen_ones(&zghz3329, zghz3328);
            zgaz3424 = CONVERT_OF(sbits, lbits)(zghz3329, true);
          }
        }
        {
          RECREATE(sail_int)(&zghz3325);
          CONVERT_OF(sail_int, mach_int)(&zghz3325, INT64_C(33));
          RECREATE(lbits)(&zghz3326);
          CONVERT_OF(lbits, sbits)(&zghz3326, zgaz3424, true);
          RECREATE(lbits)(&zghz3327);
          sailgen_zzero_extend(&zghz3327, zghz3325, zghz3326);
          zmaskLo = CONVERT_OF(fbits, lbits)(zghz3327, true);
        }
      }
      {
        uint64_t zgaz3404;
        zgaz3404 = (zext_base & zmaskLo);
        zlostSignificantBase = (zgaz3404 != UINT64_C(0b000000000000000000000000000000000));
        unit zgsz3713;
        zgsz3713 = UNIT;
      }
      {
        uint64_t zgaz3405;
        zgaz3405 = (ztop & zmaskLo);
        zlostSignificantTop = (zgaz3405 != UINT64_C(0b000000000000000000000000000000000));
        unit zgsz3712;
        zgsz3712 = UNIT;
      }
      {
        unit zgsz3711;
        if (zlostSignificantTop) {
          {
            RECREATE(lbits)(&zghz3322);
            CONVERT_OF(lbits, fbits)(&zghz3322, zT_ie, UINT64_C(8) , true);
            RECREATE(sail_int)(&zghz3323);
            CONVERT_OF(sail_int, mach_int)(&zghz3323, INT64_C(1));
            RECREATE(lbits)(&zghz3324);
            add_bits_int(&zghz3324, zghz3322, zghz3323);
            zT_ie = CONVERT_OF(fbits, lbits)(zghz3324, true);
          }
          zgsz3711 = UNIT;
        } else {  zgsz3711 = UNIT;  }
      }
      uint64_t zlen_ie;
      zlen_ie = ((zT_ie - zB_ie) & UINT64_C(0xFF));
      {
        bool zgaz3409;
        {
          fbits zgaz3408;
          {
            int64_t zgaz3407;
            {
              int64_t zgaz3406;
              zgaz3406 = (zcap_xe_width + INT64_C(1));
              {
                RECREATE(sail_int)(&zghz3319);
                CONVERT_OF(sail_int, mach_int)(&zghz3319, zcap_mantissa_width);
                RECREATE(sail_int)(&zghz3320);
                CONVERT_OF(sail_int, mach_int)(&zghz3320, zgaz3406);
                RECREATE(sail_int)(&zghz3321);
                sub_int(&zghz3321, zghz3319, zghz3320);
                zgaz3407 = CONVERT_OF(mach_int, sail_int)(zghz3321);
              }
            }
            zgaz3408 = (UINT64_C(1) & (zlen_ie >> zgaz3407));
          }
          zgaz3409 = eq_bit(zgaz3408, UINT64_C(1));
        }
        unit zgsz3715;
        if (zgaz3409) {
          {
            zincE = true;
            unit zgsz3721;
            zgsz3721 = UNIT;
          }
          {
            bool zgsz3716;
            if (zlostSignificantBase) {  zgsz3716 = true;  } else {
              fbits zgaz3410;
              zgaz3410 = (UINT64_C(1) & (zB_ie >> INT64_C(0)));
              zgsz3716 = eq_bit(zgaz3410, UINT64_C(1));
            }
            zlostSignificantBase = zgsz3716;
            unit zgsz3720;
            zgsz3720 = UNIT;
          }
          {
            bool zgsz3717;
            if (zlostSignificantTop) {  zgsz3717 = true;  } else {
              fbits zgaz3411;
              zgaz3411 = (UINT64_C(1) & (zT_ie >> INT64_C(0)));
              zgsz3717 = eq_bit(zgaz3411, UINT64_C(1));
            }
            zlostSignificantTop = zgsz3717;
            unit zgsz3719;
            zgsz3719 = UNIT;
          }
          {
            uint64_t zgaz3414;
            {
              int64_t zgaz3413;
              {
                int64_t zgaz3412;
                zgaz3412 = (ze + zcap_xe_width);
                zgaz3413 = (zgaz3412 + INT64_C(1));
              }
              {
                RECREATE(lbits)(&zghz3316);
                CONVERT_OF(lbits, fbits)(&zghz3316, zbase, UINT64_C(32) , true);
                RECREATE(sail_int)(&zghz3317);
                CONVERT_OF(sail_int, mach_int)(&zghz3317, zgaz3413);
                RECREATE(lbits)(&zghz3318);
                shiftr(&zghz3318, zghz3316, zghz3317);
                zgaz3414 = CONVERT_OF(fbits, lbits)(zghz3318, true);
              }
            }
            int64_t zgaz3415;
            {
              RECREATE(sail_int)(&zghz3313);
              CONVERT_OF(sail_int, mach_int)(&zghz3313, zcap_mantissa_width);
              RECREATE(sail_int)(&zghz3314);
              CONVERT_OF(sail_int, mach_int)(&zghz3314, zcap_xe_width);
              RECREATE(sail_int)(&zghz3315);
              sub_int(&zghz3315, zghz3313, zghz3314);
              zgaz3415 = CONVERT_OF(mach_int, sail_int)(zghz3315);
            }
            {
              RECREATE(lbits)(&zghz3310);
              CONVERT_OF(lbits, fbits)(&zghz3310, zgaz3414, UINT64_C(32) , true);
              RECREATE(sail_int)(&zghz3311);
              CONVERT_OF(sail_int, mach_int)(&zghz3311, zgaz3415);
              RECREATE(lbits)(&zghz3312);
              sail_truncate(&zghz3312, zghz3310, zghz3311);
              zB_ie = CONVERT_OF(fbits, lbits)(zghz3312, true);
            }
            unit zgsz3718;
            zgsz3718 = UNIT;
          }
          int64_t zincT;
          if (zlostSignificantTop) {  zincT = INT64_C(1);  } else {  zincT = INT64_C(0);  }
          uint64_t zgaz3420;
          {
            uint64_t zgaz3418;
            {
              int64_t zgaz3417;
              {
                int64_t zgaz3416;
                zgaz3416 = (ze + zcap_xe_width);
                zgaz3417 = (zgaz3416 + INT64_C(1));
              }
              {
                RECREATE(lbits)(&zghz3307);
                CONVERT_OF(lbits, fbits)(&zghz3307, ztop, UINT64_C(33) , true);
                RECREATE(sail_int)(&zghz3308);
                CONVERT_OF(sail_int, mach_int)(&zghz3308, zgaz3417);
                RECREATE(lbits)(&zghz3309);
                shiftr(&zghz3309, zghz3307, zghz3308);
                zgaz3418 = CONVERT_OF(fbits, lbits)(zghz3309, true);
              }
            }
            int64_t zgaz3419;
            {
              RECREATE(sail_int)(&zghz3304);
              CONVERT_OF(sail_int, mach_int)(&zghz3304, zcap_mantissa_width);
              RECREATE(sail_int)(&zghz3305);
              CONVERT_OF(sail_int, mach_int)(&zghz3305, zcap_xe_width);
              RECREATE(sail_int)(&zghz3306);
              sub_int(&zghz3306, zghz3304, zghz3305);
              zgaz3419 = CONVERT_OF(mach_int, sail_int)(zghz3306);
            }
            {
              RECREATE(lbits)(&zghz3301);
              CONVERT_OF(lbits, fbits)(&zghz3301, zgaz3418, UINT64_C(33) , true);
              RECREATE(sail_int)(&zghz3302);
              CONVERT_OF(sail_int, mach_int)(&zghz3302, zgaz3419);
              RECREATE(lbits)(&zghz3303);
              sail_truncate(&zghz3303, zghz3301, zghz3302);
              zgaz3420 = CONVERT_OF(fbits, lbits)(zghz3303, true);
            }
          }
          {
            RECREATE(lbits)(&zghz3298);
            CONVERT_OF(lbits, fbits)(&zghz3298, zgaz3420, UINT64_C(8) , true);
            RECREATE(sail_int)(&zghz3299);
            CONVERT_OF(sail_int, mach_int)(&zghz3299, zincT);
            RECREATE(lbits)(&zghz3300);
            add_bits_int(&zghz3300, zghz3298, zghz3299);
            zT_ie = CONVERT_OF(fbits, lbits)(zghz3300, true);
          }
          zgsz3715 = UNIT;
        } else {  zgsz3715 = UNIT;  }
      }
      {
        uint64_t zgaz3421;
        {
          RECREATE(sail_int)(&zghz3296);
          CONVERT_OF(sail_int, mach_int)(&zghz3296, zcap_xe_width);
          RECREATE(lbits)(&zghz3297);
          sailgen_zzeros_implicit(&zghz3297, zghz3296);
          zgaz3421 = CONVERT_OF(fbits, lbits)(zghz3297, true);
        }
        zBbits = (zB_ie << 2) | zgaz3421;
        unit zgsz3714;
        zgsz3714 = UNIT;
      }
      uint64_t zgaz3422;
      {
        RECREATE(sail_int)(&zghz3294);
        CONVERT_OF(sail_int, mach_int)(&zghz3294, zcap_xe_width);
        RECREATE(lbits)(&zghz3295);
        sailgen_zzeros_implicit(&zghz3295, zghz3294);
        zgaz3422 = CONVERT_OF(fbits, lbits)(zghz3295, true);
      }
      zTbits = (zT_ie << 2) | zgaz3422;
      zgsz3710 = UNIT;
    } else {  zgsz3710 = UNIT;  }
  }
  struct zCapability zghz3293;
  {
    int64_t zgaz3434;
    if (zincE) {
      bool zgaz3433;
      {
        int64_t zgaz3432;
        zgaz3432 = (ze + INT64_C(1));
        zgaz3433 = (zgaz3432 <= zcap_max_E);
      }
      if (zgaz3433) {  zgaz3434 = (ze + INT64_C(1));  } else {  zgaz3434 = zcap_min_E;  }
    } else {  zgaz3434 = ze;  }
    zghz3293 = zcap;
    zghz3293.zB = zBbits;
    zghz3293.zE = zgaz3434;
    zghz3293.zT = zTbits;
    zghz3293.zaddress = zbase;
    zghz3293.zinternal_exponent = zie;
  }
  bool zexact;
  {
    bool zgaz3431;
    {    if (zlostSignificantBase) {  zgaz3431 = true;  } else {  zgaz3431 = zlostSignificantTop;  }
    }
    zexact = not(zgaz3431);
  }
  zcbz365.ztup0 = zexact;
  zcbz365.ztup1 = zghz3293;










end_function_598: ;
  return zcbz365;
end_block_exception_599: ;
  struct zCapability zcbz3108 = { .zB = UINT64_C(0xdeadc0de), .zE = INT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zap_m = UINT64_C(0xdeadc0de), .zinternal_exponent = false, .zreserved_0 = UINT64_C(0xdeadc0de), .zreserved_1 = UINT64_C(0xdeadc0de), .zsd_perms = UINT64_C(0xdeadc0de), .zsealed = false, .ztag = false };
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz3107 = { .ztup0 = false, .ztup1 = zcbz3108 };
  return zcbz3107;
}



static void finish_sailgen_setCapBounds(void)
{
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
  KILL(lbits)(&zghz3353);
  KILL(sail_int)(&zghz3352);
  KILL(lbits)(&zghz3351);
  KILL(lbits)(&zghz3350);
  KILL(sail_int)(&zghz3349);
  KILL(lbits)(&zghz3348);
  KILL(lbits)(&zghz3347);
  KILL(sail_int)(&zghz3346);
  KILL(lbits)(&zghz3345);
  KILL(sail_int)(&zghz3344);
  KILL(sail_int)(&zghz3343);
  KILL(sail_int)(&zghz3342);
  KILL(lbits)(&zghz3341);
  KILL(sail_int)(&zghz3340);
  KILL(lbits)(&zghz3339);
  KILL(lbits)(&zghz3338);
  KILL(sail_int)(&zghz3337);
  KILL(lbits)(&zghz3336);
  KILL(sail_int)(&zghz3335);
  KILL(sail_int)(&zghz3334);
  KILL(sail_int)(&zghz3333);
  KILL(lbits)(&zghz3332);
  KILL(sail_int)(&zghz3331);
  KILL(lbits)(&zghz3330);
  KILL(lbits)(&zghz3329);
  KILL(sail_int)(&zghz3328);
  KILL(lbits)(&zghz3327);
  KILL(lbits)(&zghz3326);
  KILL(sail_int)(&zghz3325);
  KILL(lbits)(&zghz3324);
  KILL(sail_int)(&zghz3323);
  KILL(lbits)(&zghz3322);
  KILL(sail_int)(&zghz3321);
  KILL(sail_int)(&zghz3320);
  KILL(sail_int)(&zghz3319);
  KILL(lbits)(&zghz3318);
  KILL(sail_int)(&zghz3317);
  KILL(lbits)(&zghz3316);
  KILL(sail_int)(&zghz3315);
  KILL(sail_int)(&zghz3314);
  KILL(sail_int)(&zghz3313);
  KILL(lbits)(&zghz3312);
  KILL(sail_int)(&zghz3311);
  KILL(lbits)(&zghz3310);
  KILL(lbits)(&zghz3309);
  KILL(sail_int)(&zghz3308);
  KILL(lbits)(&zghz3307);
  KILL(sail_int)(&zghz3306);
  KILL(sail_int)(&zghz3305);
  KILL(sail_int)(&zghz3304);
  KILL(lbits)(&zghz3303);
  KILL(sail_int)(&zghz3302);
  KILL(lbits)(&zghz3301);
  KILL(lbits)(&zghz3300);
  KILL(sail_int)(&zghz3299);
  KILL(lbits)(&zghz3298);
  KILL(lbits)(&zghz3297);
  KILL(sail_int)(&zghz3296);
  KILL(lbits)(&zghz3295);
  KILL(sail_int)(&zghz3294);

}

static bool sailgen_capIsSealed(struct zCapability);

static bool sailgen_capIsSealed(struct zCapability zcap)
{
  __label__ end_function_601, end_block_exception_602;

  bool zcbz366;
  zcbz366 = zcap.zsealed;
end_function_601: ;
  return zcbz366;
end_block_exception_602: ;

  return false;
}

static bool sailgen_permsMalformed(struct zCapability);

static bool sailgen_permsMalformed(struct zCapability zcap)
{
  __label__ end_function_604, end_block_exception_605;

  bool zcbz367;
  bool zgaz3443;
  {
    uint64_t zgaz3442;
    zgaz3442 = zcap.zap_m;
    zgaz3443 = sailgen_ap_m_encdec_forwards_matches(zgaz3442);
  }
  zcbz367 = not(zgaz3443);

end_function_604: ;
  return zcbz367;
end_block_exception_605: ;

  return false;
}

static void sailgen_getCapPermsAndMode(struct zoptionzIz8RArchPermszCEExecutionModez5z9zK *rop, struct zCapability);


static void sailgen_getCapPermsAndMode(struct zoptionzIz8RArchPermszCEExecutionModez5z9zK *zcbz368, struct zCapability zcap)
{
  __label__ end_function_607, end_block_exception_608, end_function_705;

  bool zgaz3444;
  zgaz3444 = sailgen_permsMalformed(zcap);
  if (zgaz3444) {  sailgen_NonezIz8RArchPermszCEExecutionModez5z9zK((*(&zcbz368)), UNIT);  } else {
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgaz3446;
    {
      uint64_t zgaz3445;
      zgaz3445 = zcap.zap_m;
      zgaz3446 = sailgen_ap_m_encdec_forwards(zgaz3445);
    }
    sailgen_SomezIz8RArchPermszCEExecutionModez5z9zK((*(&zcbz368)), zgaz3446);
  }

end_function_607: ;
  goto end_function_705;
end_block_exception_608: ;
  goto end_function_705;
end_function_705: ;
}

static struct zCapability sailgen_setCapPermsAndMode(struct zCapability, struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9);

struct zCapability zghz3367;
struct zArchPerms zghz3368;

static void startup_sailgen_setCapPermsAndMode(void)
{


}

static struct zCapability sailgen_setCapPermsAndMode(struct zCapability zcap, struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgsz3725)
{
  __label__ fundef_fail_609, fundef_body_610, end_function_611, end_block_exception_612;

  struct zCapability zghz3367;
  struct zArchPerms zghz3368;
  zghz3368 = zgsz3725.ztup0;
  enum zExecutionMode zmode;
  zmode = zgsz3725.ztup1;
  goto fundef_body_610;
fundef_fail_609: ;
  sail_match_failure("setCapPermsAndMode");
fundef_body_610: ;
  uint64_t zgaz3448;
  {
    struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgaz3447;
    zgaz3447 = sailgen_legalizzePermsAndMode(zghz3368, zmode);
    zgaz3448 = sailgen_ap_m_encdec_backwards(zgaz3447);
  }
  zghz3367 = zcap;
  zghz3367.zap_m = zgaz3448;

end_function_611: ;
  return zghz3367;
end_block_exception_612: ;
  struct zCapability zcbz3109 = { .zB = UINT64_C(0xdeadc0de), .zE = INT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zap_m = UINT64_C(0xdeadc0de), .zinternal_exponent = false, .zreserved_0 = UINT64_C(0xdeadc0de), .zreserved_1 = UINT64_C(0xdeadc0de), .zsd_perms = UINT64_C(0xdeadc0de), .zsealed = false, .ztag = false };
  return zcbz3109;
}



static void finish_sailgen_setCapPermsAndMode(void)
{


}

static void sailgen_getArchPerms(struct zoptionzIRArchPermszK *rop, struct zCapability);

struct zArchPerms zghz3369;

static void startup_sailgen_getArchPerms(void)
{
}

static void sailgen_getArchPerms(struct zoptionzIRArchPermszK *zcbz370, struct zCapability zcap)
{
  __label__ case_616, case_615, finish_match_614, end_function_617, end_block_exception_618, end_function_704;

  struct zoptionzIz8RArchPermszCEExecutionModez5z9zK zgaz3449;
  CREATE(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zgaz3449);
  sailgen_getCapPermsAndMode(&zgaz3449, zcap);
  struct zoptionzIRArchPermszK zgsz3727;
  CREATE(zoptionzIRArchPermszK)(&zgsz3727);
  {
    if (zgaz3449.kind != Kind_zNonezIz8RArchPermszCEExecutionModez5z9zK) goto case_616;
    sailgen_NonezIRArchPermszK(&zgsz3727, UNIT);
    goto finish_match_614;
  }
case_616: ;
  {
    /* complete */
    struct zArchPerms zghz3369;
    zghz3369 = zgaz3449.zSomezIz8RArchPermszCEExecutionModez5z9zK.ztup0;
    sailgen_SomezIRArchPermszK(&zgsz3727, zghz3369);
    goto finish_match_614;
  }
case_615: ;
finish_match_614: ;
  COPY(zoptionzIRArchPermszK)((*(&zcbz370)), zgsz3727);
  KILL(zoptionzIRArchPermszK)(&zgsz3727);
  KILL(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zgaz3449);
end_function_617: ;
  goto end_function_704;
end_block_exception_618: ;
  goto end_function_704;
end_function_704: ;
}



static void finish_sailgen_getArchPerms(void)
{
}

static struct zArchPerms sailgen_getArchPermsLegalizzed(struct zCapability);

struct zArchPerms zghz3370;
struct zArchPerms zghz3371;

static void startup_sailgen_getArchPermsLegalizzed(void)
{


}

static struct zArchPerms sailgen_getArchPermsLegalizzed(struct zCapability zcap)
{
  __label__ case_622, case_621, finish_match_620, end_function_623, end_block_exception_624;

  struct zArchPerms zghz3370;
  struct zoptionzIRArchPermszK zgaz3450;
  CREATE(zoptionzIRArchPermszK)(&zgaz3450);
  sailgen_getArchPerms(&zgaz3450, zcap);
  {
    if (zgaz3450.kind != Kind_zNonezIRArchPermszK) goto case_622;
    zghz3370 = znull_perms;
    goto finish_match_620;
  }
case_622: ;
  {
    /* complete */
    struct zArchPerms zghz3371;
    zghz3371 = zgaz3450.zSomezIRArchPermszK;
    zghz3370 = zghz3371;
    goto finish_match_620;
  }
case_621: ;
finish_match_620: ;
  KILL(zoptionzIRArchPermszK)(&zgaz3450);
end_function_623: ;
  return zghz3370;
end_block_exception_624: ;
  struct zArchPerms zcbz3110 = { .zASR = false, .zC = false, .zLM = false, .zR = false, .zW = false, .zX = false };
  return zcbz3110;
}



static void finish_sailgen_getArchPermsLegalizzed(void)
{


}

static enum zExecutionMode sailgen_getCapMode(struct zCapability);

static enum zExecutionMode sailgen_getCapMode(struct zCapability zcap)
{
  __label__ case_628, case_627, finish_match_626, end_function_629, end_block_exception_630;

  enum zExecutionMode zcbz372;
  struct zoptionzIz8RArchPermszCEExecutionModez5z9zK zgaz3451;
  CREATE(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zgaz3451);
  sailgen_getCapPermsAndMode(&zgaz3451, zcap);
  {
    if (zgaz3451.kind != Kind_zNonezIz8RArchPermszCEExecutionModez5z9zK) goto case_628;
    zcbz372 = sailgen_execution_mode_encdec_backwards(UINT64_C(0b0));
    goto finish_match_626;
  }
case_628: ;
  {
    /* complete */
    enum zExecutionMode zmode;
    zmode = zgaz3451.zSomezIz8RArchPermszCEExecutionModez5z9zK.ztup1;
    zcbz372 = zmode;
    goto finish_match_626;
  }
case_627: ;
finish_match_626: ;
  KILL(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zgaz3451);
end_function_629: ;
  return zcbz372;
end_block_exception_630: ;

  return zCapPtrMode;
}

static int64_t sailgen_getCapLength(struct zCapability);

sail_int zghz3372;
sail_int zghz3373;
sail_int zghz3374;
sail_int zghz3375;
sail_int zghz3376;

static void startup_sailgen_getCapLength(void)
{
  CREATE(sail_int)(&zghz3372);
  CREATE(sail_int)(&zghz3373);
  CREATE(sail_int)(&zghz3374);
  CREATE(sail_int)(&zghz3375);
  CREATE(sail_int)(&zghz3376);
}

static int64_t sailgen_getCapLength(struct zCapability zc)
{
  __label__ case_634, case_633, finish_match_632, end_function_635, end_block_exception_636;

  int64_t zcbz373;
  struct zoptionzIz8izCiz9zK zgaz3452;
  CREATE(zoptionzIz8izCiz9zK)(&zgaz3452);
  sailgen_getCapBounds(&zgaz3452, zc);
  {
    if (zgaz3452.kind != Kind_zNonezIz8izCiz9zK) goto case_634;
    zcbz373 = INT64_C(0);
    goto finish_match_632;
  }
case_634: ;
  {
    /* complete */
    int64_t zbase;
    zbase = CONVERT_OF(mach_int, sail_int)(zgaz3452.zSomezIz8izCiz9zK.ztup0);
    int64_t ztop;
    ztop = CONVERT_OF(mach_int, sail_int)(zgaz3452.zSomezIz8izCiz9zK.ztup1);
    {
      bool zgaz3453;
      {
        RECREATE(sail_int)(&zghz3375);
        CONVERT_OF(sail_int, mach_int)(&zghz3375, ztop);
        RECREATE(sail_int)(&zghz3376);
        CONVERT_OF(sail_int, mach_int)(&zghz3376, zbase);
        zgaz3453 = gteq(zghz3375, zghz3376);
      }
      unit zgsz3737;
      zgsz3737 = sail_assert(zgaz3453, "src/cheri_cap_common.sail:433.42-433.43");
    }
    {
      RECREATE(sail_int)(&zghz3372);
      CONVERT_OF(sail_int, mach_int)(&zghz3372, ztop);
      RECREATE(sail_int)(&zghz3373);
      CONVERT_OF(sail_int, mach_int)(&zghz3373, zbase);
      RECREATE(sail_int)(&zghz3374);
      sub_int(&zghz3374, zghz3372, zghz3373);
      zcbz373 = CONVERT_OF(mach_int, sail_int)(zghz3374);
    }
    goto finish_match_632;
  }
case_633: ;
finish_match_632: ;
  KILL(zoptionzIz8izCiz9zK)(&zgaz3452);
end_function_635: ;
  return zcbz373;
end_block_exception_636: ;

  return INT64_C(0xdeadc0de);
}



static void finish_sailgen_getCapLength(void)
{
  KILL(sail_int)(&zghz3376);
  KILL(sail_int)(&zghz3375);
  KILL(sail_int)(&zghz3374);
  KILL(sail_int)(&zghz3373);
  KILL(sail_int)(&zghz3372);
}

static bool sailgen_inCapBoundsNoWrap(struct zCapability, uint64_t, int64_t);

sail_int zghz3377;
sail_int zghz3378;
sail_int zghz3379;
sail_int zghz3380;
lbits zghz3381;
sail_int zghz3382;

static void startup_sailgen_inCapBoundsNoWrap(void)
{
  CREATE(sail_int)(&zghz3377);
  CREATE(sail_int)(&zghz3378);
  CREATE(sail_int)(&zghz3379);
  CREATE(sail_int)(&zghz3380);
  CREATE(lbits)(&zghz3381);
  CREATE(sail_int)(&zghz3382);
}

static bool sailgen_inCapBoundsNoWrap(struct zCapability zcap, uint64_t zaddr, int64_t zsizze)
{
  __label__ case_640, case_639, finish_match_638, end_function_641, end_block_exception_642;

  bool zcbz374;
  int64_t za;
  {
    RECREATE(lbits)(&zghz3381);
    CONVERT_OF(lbits, fbits)(&zghz3381, zaddr, UINT64_C(32) , true);
    RECREATE(sail_int)(&zghz3382);
    sail_unsigned(&zghz3382, zghz3381);
    za = CONVERT_OF(mach_int, sail_int)(zghz3382);
  }
  struct zoptionzIz8izCiz9zK zgaz3454;
  CREATE(zoptionzIz8izCiz9zK)(&zgaz3454);
  sailgen_getCapBounds(&zgaz3454, zcap);
  {
    if (zgaz3454.kind != Kind_zNonezIz8izCiz9zK) goto case_640;
    zcbz374 = false;
    goto finish_match_638;
  }
case_640: ;
  {
    /* complete */
    int64_t zbase;
    zbase = CONVERT_OF(mach_int, sail_int)(zgaz3454.zSomezIz8izCiz9zK.ztup0);
    int64_t ztop;
    ztop = CONVERT_OF(mach_int, sail_int)(zgaz3454.zSomezIz8izCiz9zK.ztup1);
    bool zgaz3456;
    {
      RECREATE(sail_int)(&zghz3379);
      CONVERT_OF(sail_int, mach_int)(&zghz3379, za);
      RECREATE(sail_int)(&zghz3380);
      CONVERT_OF(sail_int, mach_int)(&zghz3380, zbase);
      zgaz3456 = gteq(zghz3379, zghz3380);
    }
    bool zgsz3741;
    if (zgaz3456) {
      int64_t zgaz3455;
      zgaz3455 = (za + zsizze);
      {
        RECREATE(sail_int)(&zghz3377);
        CONVERT_OF(sail_int, mach_int)(&zghz3377, zgaz3455);
        RECREATE(sail_int)(&zghz3378);
        CONVERT_OF(sail_int, mach_int)(&zghz3378, ztop);
        zgsz3741 = lteq(zghz3377, zghz3378);
      }
    } else {  zgsz3741 = false;  }
    zcbz374 = zgsz3741;
    goto finish_match_638;
  }
case_639: ;
finish_match_638: ;
  KILL(zoptionzIz8izCiz9zK)(&zgaz3454);

end_function_641: ;
  return zcbz374;
end_block_exception_642: ;

  return false;
}



static void finish_sailgen_inCapBoundsNoWrap(void)
{
  KILL(sail_int)(&zghz3382);
  KILL(lbits)(&zghz3381);
  KILL(sail_int)(&zghz3380);
  KILL(sail_int)(&zghz3379);
  KILL(sail_int)(&zghz3378);
  KILL(sail_int)(&zghz3377);
}

static bool sailgen_capReservedValid(struct zCapability);

static bool sailgen_capReservedValid(struct zCapability zcap)
{
  __label__ end_function_644, end_block_exception_645;

  bool zcbz375;
  bool zgaz3459;
  {
    uint64_t zgaz3457;
    zgaz3457 = zcap.zreserved_0;
    zgaz3459 = (zgaz3457 == UINT64_C(0b000));
  }
  bool zgsz3744;
  if (zgaz3459) {
    uint64_t zgaz3458;
    zgaz3458 = zcap.zreserved_1;
    zgsz3744 = (zgaz3458 == UINT64_C(0b0));
  } else {  zgsz3744 = false;  }
  zcbz375 = zgsz3744;

end_function_644: ;
  return zcbz375;
end_block_exception_645: ;

  return false;
}

static struct zCapability sailgen_clearTagIf(struct zCapability, bool);

struct zCapability zghz3383;

static void startup_sailgen_clearTagIf(void)
{
}

static struct zCapability sailgen_clearTagIf(struct zCapability zcap, bool zcond)
{
  __label__ end_function_647, end_block_exception_648;

  struct zCapability zghz3383;
  bool zgaz3461;
  {
    bool zgaz3460;
    zgaz3460 = zcap.ztag;
    bool zgsz3745;
    if (zgaz3460) {  zgsz3745 = not(zcond);  } else {  zgsz3745 = false;  }
    zgaz3461 = zgsz3745;
  }
  zghz3383 = zcap;
  zghz3383.ztag = zgaz3461;

end_function_647: ;
  return zghz3383;
end_block_exception_648: ;
  struct zCapability zcbz3111 = { .zB = UINT64_C(0xdeadc0de), .zE = INT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zap_m = UINT64_C(0xdeadc0de), .zinternal_exponent = false, .zreserved_0 = UINT64_C(0xdeadc0de), .zreserved_1 = UINT64_C(0xdeadc0de), .zsd_perms = UINT64_C(0xdeadc0de), .zsealed = false, .ztag = false };
  return zcbz3111;
}



static void finish_sailgen_clearTagIf(void)
{
}

static bool zsys_legalizze_malformed_permission_on_load;
static void create_letbind_25(void) {


  bool zgsz3747;
  zgsz3747 = false;
  zsys_legalizze_malformed_permission_on_load = zgsz3747;

let_end_649: ;
}
static void kill_letbind_25(void) {
}

static bool sailgen_capBoundsEqual(struct zCapability, struct zCapability);

sail_int zghz3384;
sail_int zghz3385;
sail_int zghz3386;
sail_int zghz3387;

static void startup_sailgen_capBoundsEqual(void)
{
  CREATE(sail_int)(&zghz3384);
  CREATE(sail_int)(&zghz3385);
  CREATE(sail_int)(&zghz3386);
  CREATE(sail_int)(&zghz3387);
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
  __label__ case_654, case_653, case_652, finish_match_651, end_function_655, end_block_exception_656;

  bool zcbz377;
  struct ztuple_z8z5unionz0zzoptionzzIzz8izzCizz9zzKzCz0z5unionz0zzoptionzzIzz8izzCizz9zzKz9 zgaz3464;
  CREATE(ztuple_z8z5unionz0zzoptionzzIzz8izzCizz9zzKzCz0z5unionz0zzoptionzzIzz8izzCizz9zzKz9)(&zgaz3464);
  {
    struct zoptionzIz8izCiz9zK zgaz3462;
    CREATE(zoptionzIz8izCiz9zK)(&zgaz3462);
    sailgen_getCapBounds(&zgaz3462, zc1);
    struct zoptionzIz8izCiz9zK zgaz3463;
    CREATE(zoptionzIz8izCiz9zK)(&zgaz3463);
    sailgen_getCapBounds(&zgaz3463, zc2);
    COPY(zoptionzIz8izCiz9zK)(&((&zgaz3464)->ztup0), zgaz3462);
    COPY(zoptionzIz8izCiz9zK)(&((&zgaz3464)->ztup1), zgaz3463);
    KILL(zoptionzIz8izCiz9zK)(&zgaz3463);
    KILL(zoptionzIz8izCiz9zK)(&zgaz3462);
  }
  {
    if (zgaz3464.ztup0.kind != Kind_zNonezIz8izCiz9zK) goto case_654;
    zcbz377 = false;
    goto finish_match_651;
  }
case_654: ;
  {
    if (zgaz3464.ztup1.kind != Kind_zNonezIz8izCiz9zK) goto case_653;
    zcbz377 = false;
    goto finish_match_651;
  }
case_653: ;
  {
    /* complete */
    /* complete */
    int64_t zbase1;
    zbase1 = CONVERT_OF(mach_int, sail_int)(zgaz3464.ztup0.zSomezIz8izCiz9zK.ztup0);
    int64_t ztop1;
    ztop1 = CONVERT_OF(mach_int, sail_int)(zgaz3464.ztup0.zSomezIz8izCiz9zK.ztup1);
    int64_t zbase2;
    zbase2 = CONVERT_OF(mach_int, sail_int)(zgaz3464.ztup1.zSomezIz8izCiz9zK.ztup0);
    int64_t ztop2;
    ztop2 = CONVERT_OF(mach_int, sail_int)(zgaz3464.ztup1.zSomezIz8izCiz9zK.ztup1);
    bool zgaz3465;
    {
      RECREATE(sail_int)(&zghz3386);
      CONVERT_OF(sail_int, mach_int)(&zghz3386, zbase1);
      RECREATE(sail_int)(&zghz3387);
      CONVERT_OF(sail_int, mach_int)(&zghz3387, zbase2);
      zgaz3465 = eq_int(zghz3386, zghz3387);
    }
    bool zgsz3750;
    if (zgaz3465) {
      {
        RECREATE(sail_int)(&zghz3384);
        CONVERT_OF(sail_int, mach_int)(&zghz3384, ztop1);
        RECREATE(sail_int)(&zghz3385);
        CONVERT_OF(sail_int, mach_int)(&zghz3385, ztop2);
        zgsz3750 = eq_int(zghz3384, zghz3385);
      }
    } else {  zgsz3750 = false;  }
    zcbz377 = zgsz3750;
    goto finish_match_651;
  }
case_652: ;
finish_match_651: ;
  KILL(ztuple_z8z5unionz0zzoptionzzIzz8izzCizz9zzKzCz0z5unionz0zzoptionzzIzz8izzCizz9zzKz9)(&zgaz3464);
end_function_655: ;
  return zcbz377;
end_block_exception_656: ;

  return false;
}



static void finish_sailgen_capBoundsEqual(void)
{
  KILL(sail_int)(&zghz3387);
  KILL(sail_int)(&zghz3386);
  KILL(sail_int)(&zghz3385);
  KILL(sail_int)(&zghz3384);
}

static struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 sailgen_setCapAddr(struct zCapability, uint64_t);

struct zCapability zghz3388;

static void startup_sailgen_setCapAddr(void)
{
}

static struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 sailgen_setCapAddr(struct zCapability zc, uint64_t zaddr)
{
  __label__ end_function_658, end_block_exception_659;

  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz378;
  struct zCapability zghz3388;
  {
    zghz3388 = zc;
    zghz3388.zaddress = zaddr;
  }
  bool zrepresentable;
  zrepresentable = sailgen_capBoundsEqual(zc, zghz3388);
  zcbz378.ztup0 = zrepresentable;
  zcbz378.ztup1 = zghz3388;

end_function_658: ;
  return zcbz378;
end_block_exception_659: ;
  struct zCapability zcbz3113 = { .zB = UINT64_C(0xdeadc0de), .zE = INT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zap_m = UINT64_C(0xdeadc0de), .zinternal_exponent = false, .zreserved_0 = UINT64_C(0xdeadc0de), .zreserved_1 = UINT64_C(0xdeadc0de), .zsd_perms = UINT64_C(0xdeadc0de), .zsealed = false, .ztag = false };
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz3112 = { .ztup0 = false, .ztup1 = zcbz3113 };
  return zcbz3112;
}



static void finish_sailgen_setCapAddr(void)
{
}

static void sailgen_capToString(sail_string *rop, struct zCapability);

lbits zghz3389;
lbits zghz3390;
lbits zghz3391;
struct zEncMetadata zghz3392;
struct zArchPerms zghz3393;
lbits zghz3394;
sail_int zghz3395;
sail_int zghz3396;
lbits zghz3397;
lbits zghz3398;
sail_int zghz3399;
lbits zghz3400;
lbits zghz3401;
lbits zghz3402;

static void startup_sailgen_capToString(void)
{
  CREATE(lbits)(&zghz3389);
  CREATE(lbits)(&zghz3390);
  CREATE(lbits)(&zghz3391);


  CREATE(lbits)(&zghz3394);
  CREATE(sail_int)(&zghz3395);
  CREATE(sail_int)(&zghz3396);
  CREATE(lbits)(&zghz3397);
  CREATE(lbits)(&zghz3398);
  CREATE(sail_int)(&zghz3399);
  CREATE(lbits)(&zghz3400);
  CREATE(lbits)(&zghz3401);
  CREATE(lbits)(&zghz3402);
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
  __label__ case_665, finish_match_664, end_function_676, end_block_exception_677, end_function_703;

  struct ztuple_z8z5stringzCz0z5stringzCz0z5boolz9 zgaz3471;
  CREATE(ztuple_z8z5stringzCz0z5stringzCz0z5boolz9)(&zgaz3471);
  {
    __label__ case_663, case_662, finish_match_661;

    struct zoptionzIz8bzCbz9zK zgaz3466;
    CREATE(zoptionzIz8bzCbz9zK)(&zgaz3466);
    sailgen_getCapBoundsBits(&zgaz3466, zcap);
    {
      if (zgaz3466.kind != Kind_zNonezIz8bzCbz9zK) goto case_663;
      struct ztuple_z8z5stringzCz0z5stringzCz0z5boolz9 zgsz3759;
      CREATE(ztuple_z8z5stringzCz0z5stringzCz0z5boolz9)(&zgsz3759);
      COPY(sail_string)(&((&zgsz3759)->ztup0), "0 (malformed)");
      COPY(sail_string)(&((&zgsz3759)->ztup1), "0 (malformed)");
      zgsz3759.ztup2 = true;
      COPY(ztuple_z8z5stringzCz0z5stringzCz0z5boolz9)(&zgaz3471, zgsz3759);
      KILL(ztuple_z8z5stringzCz0z5stringzCz0z5boolz9)(&zgsz3759);
      goto finish_match_661;
    }
  case_663: ;
    {
      /* complete */
      uint64_t zbase;
      zbase = CONVERT_OF(fbits, lbits)(zgaz3466.zSomezIz8bzCbz9zK.ztup0, true);
      uint64_t ztop;
      ztop = CONVERT_OF(fbits, lbits)(zgaz3466.zSomezIz8bzCbz9zK.ztup1, true);
      sail_string zgaz3469;
      CREATE(sail_string)(&zgaz3469);
      {
        RECREATE(lbits)(&zghz3402);
        CONVERT_OF(lbits, fbits)(&zghz3402, zbase, UINT64_C(32) , true);
        string_of_lbits(&zgaz3469, zghz3402);
      }
      sail_string zgaz3470;
      CREATE(sail_string)(&zgaz3470);
      {
        uint64_t zgaz3468;
        {
          int64_t zgaz3467;
          zgaz3467 = (zcap_len_width + INT64_C(3));
          {
            RECREATE(sail_int)(&zghz3399);
            CONVERT_OF(sail_int, mach_int)(&zghz3399, zgaz3467);
            RECREATE(lbits)(&zghz3400);
            CONVERT_OF(lbits, fbits)(&zghz3400, ztop, UINT64_C(33) , true);
            RECREATE(lbits)(&zghz3401);
            sailgen_zzero_extend(&zghz3401, zghz3399, zghz3400);
            zgaz3468 = CONVERT_OF(fbits, lbits)(zghz3401, true);
          }
        }
        {
          RECREATE(lbits)(&zghz3398);
          CONVERT_OF(lbits, fbits)(&zghz3398, zgaz3468, UINT64_C(36) , true);
          string_of_lbits(&zgaz3470, zghz3398);
        }
      }
      struct ztuple_z8z5stringzCz0z5stringzCz0z5boolz9 zgsz3757;
      CREATE(ztuple_z8z5stringzCz0z5stringzCz0z5boolz9)(&zgsz3757);
      COPY(sail_string)(&((&zgsz3757)->ztup0), zgaz3469);
      COPY(sail_string)(&((&zgsz3757)->ztup1), zgaz3470);
      zgsz3757.ztup2 = false;
      COPY(ztuple_z8z5stringzCz0z5stringzCz0z5boolz9)(&zgaz3471, zgsz3757);
      KILL(ztuple_z8z5stringzCz0z5stringzCz0z5boolz9)(&zgsz3757);
      KILL(sail_string)(&zgaz3470);
      KILL(sail_string)(&zgaz3469);
      goto finish_match_661;
    }
  case_662: ;
  finish_match_661: ;
    KILL(zoptionzIz8bzCbz9zK)(&zgaz3466);
  }
  sail_string zgsz3761;
  CREATE(sail_string)(&zgsz3761);
  {
    __label__ case_670, finish_match_669;

    sail_string zbase_str;
    CREATE(sail_string)(&zbase_str);
    COPY(sail_string)(&zbase_str, zgaz3471.ztup0);
    sail_string ztop_str;
    CREATE(sail_string)(&ztop_str);
    COPY(sail_string)(&ztop_str, zgaz3471.ztup1);
    bool zbounds_malformed;
    zbounds_malformed = zgaz3471.ztup2;
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
          RECREATE(sail_int)(&zghz3395);
          CONVERT_OF(sail_int, mach_int)(&zghz3395, zgaz3525);
          RECREATE(sail_int)(&zghz3396);
          CONVERT_OF(sail_int, mach_int)(&zghz3396, zgaz3526);
          RECREATE(lbits)(&zghz3397);
          sailgen_to_bits(&zghz3397, zghz3395, zghz3396);
          zgaz3527 = CONVERT_OF(fbits, lbits)(zghz3397, true);
        }
      }
      {
        RECREATE(lbits)(&zghz3394);
        CONVERT_OF(lbits, fbits)(&zghz3394, zgaz3527, UINT64_C(36) , true);
        string_of_lbits(&zlen_str, zghz3394);
      }
    }
    struct ztuple_z8z5stringzCz0z5boolz9 zgaz3490;
    CREATE(ztuple_z8z5stringzCz0z5boolz9)(&zgaz3490);
    {
      __label__ case_668, case_667, finish_match_666;

      struct zoptionzIRArchPermszK zgaz3472;
      CREATE(zoptionzIRArchPermszK)(&zgaz3472);
      sailgen_getArchPerms(&zgaz3472, zcap);
      {
        if (zgaz3472.kind != Kind_zNonezIRArchPermszK) goto case_668;
        struct ztuple_z8z5stringzCz0z5boolz9 zgsz3765;
        CREATE(ztuple_z8z5stringzCz0z5boolz9)(&zgsz3765);
        COPY(sail_string)(&((&zgsz3765)->ztup0), "(malformed)");
        zgsz3765.ztup1 = true;
        COPY(ztuple_z8z5stringzCz0z5boolz9)(&zgaz3490, zgsz3765);
        KILL(ztuple_z8z5stringzCz0z5boolz9)(&zgsz3765);
        goto finish_match_666;
      }
    case_668: ;
      {
        /* complete */
        struct zArchPerms zghz3393;
        zghz3393 = zgaz3472.zSomezIRArchPermszK;
        sail_string zgaz3489;
        CREATE(sail_string)(&zgaz3489);
        {
          sail_string zgaz3487;
          CREATE(sail_string)(&zgaz3487);
          {
            bool zgaz3473;
            zgaz3473 = zghz3393.zR;
            if (zgaz3473) {  COPY(sail_string)(&zgaz3487, "R");  } else {  COPY(sail_string)(&zgaz3487, "");  }
          }
          sail_string zgaz3488;
          CREATE(sail_string)(&zgaz3488);
          {
            sail_string zgaz3485;
            CREATE(sail_string)(&zgaz3485);
            {
              bool zgaz3474;
              zgaz3474 = zghz3393.zW;
              if (zgaz3474) {  COPY(sail_string)(&zgaz3485, "W");  } else {  COPY(sail_string)(&zgaz3485, "");  }
            }
            sail_string zgaz3486;
            CREATE(sail_string)(&zgaz3486);
            {
              sail_string zgaz3483;
              CREATE(sail_string)(&zgaz3483);
              {
                bool zgaz3475;
                zgaz3475 = zghz3393.zC;
                if (zgaz3475) {  COPY(sail_string)(&zgaz3483, "C");  } else {  COPY(sail_string)(&zgaz3483, "");  }
              }
              sail_string zgaz3484;
              CREATE(sail_string)(&zgaz3484);
              {
                sail_string zgaz3481;
                CREATE(sail_string)(&zgaz3481);
                {
                  bool zgaz3476;
                  zgaz3476 = zghz3393.zX;
                  if (zgaz3476) {  COPY(sail_string)(&zgaz3481, "X");  } else {  COPY(sail_string)(&zgaz3481, "");  }
                }
                sail_string zgaz3482;
                CREATE(sail_string)(&zgaz3482);
                {
                  sail_string zgaz3479;
                  CREATE(sail_string)(&zgaz3479);
                  {
                    bool zgaz3477;
                    zgaz3477 = zghz3393.zLM;
                    if (zgaz3477) {  COPY(sail_string)(&zgaz3479, "Lm");  } else {  COPY(sail_string)(&zgaz3479, "");  }
                  }
                  sail_string zgaz3480;
                  CREATE(sail_string)(&zgaz3480);
                  {
                    bool zgaz3478;
                    zgaz3478 = zghz3393.zASR;
                    if (zgaz3478) {  COPY(sail_string)(&zgaz3480, "Asr");  } else {
                      COPY(sail_string)(&zgaz3480, "");
                    }
                  }
                  concat_str(&zgaz3482, zgaz3479, zgaz3480);
                  KILL(sail_string)(&zgaz3480);
                  KILL(sail_string)(&zgaz3479);
                }
                concat_str(&zgaz3484, zgaz3481, zgaz3482);
                KILL(sail_string)(&zgaz3482);
                KILL(sail_string)(&zgaz3481);
              }
              concat_str(&zgaz3486, zgaz3483, zgaz3484);
              KILL(sail_string)(&zgaz3484);
              KILL(sail_string)(&zgaz3483);
            }
            concat_str(&zgaz3488, zgaz3485, zgaz3486);
            KILL(sail_string)(&zgaz3486);
            KILL(sail_string)(&zgaz3485);
          }
          concat_str(&zgaz3489, zgaz3487, zgaz3488);
          KILL(sail_string)(&zgaz3488);
          KILL(sail_string)(&zgaz3487);
        }
        struct ztuple_z8z5stringzCz0z5boolz9 zgsz3763;
        CREATE(ztuple_z8z5stringzCz0z5boolz9)(&zgsz3763);
        COPY(sail_string)(&((&zgsz3763)->ztup0), zgaz3489);
        zgsz3763.ztup1 = false;
        COPY(ztuple_z8z5stringzCz0z5boolz9)(&zgaz3490, zgsz3763);
        KILL(ztuple_z8z5stringzCz0z5boolz9)(&zgsz3763);
        KILL(sail_string)(&zgaz3489);
        goto finish_match_666;
      }
    case_667: ;
    finish_match_666: ;
      KILL(zoptionzIRArchPermszK)(&zgaz3472);
    }
    sail_string zgsz3767;
    CREATE(sail_string)(&zgsz3767);
    {
      __label__ case_672, finish_match_671;

      sail_string zarch_perms_str;
      CREATE(sail_string)(&zarch_perms_str);
      COPY(sail_string)(&zarch_perms_str, zgaz3490.ztup0);
      bool zperms_malformed;
      zperms_malformed = zgaz3490.ztup1;
      struct ztuple_z8z5stringzCz0z5boolz9 zgaz3493;
      CREATE(ztuple_z8z5stringzCz0z5boolz9)(&zgaz3493);
      {
        bool zgaz3492;
        {
          bool zgaz3491;
          zgaz3491 = sailgen_capReservedValid(zcap);
          zgaz3492 = not(zgaz3491);
        }
        if (zgaz3492) {
          struct ztuple_z8z5stringzCz0z5boolz9 zgsz3769;
          CREATE(ztuple_z8z5stringzCz0z5boolz9)(&zgsz3769);
          COPY(sail_string)(&((&zgsz3769)->ztup0), " (has non-zero reserved bits)");
          zgsz3769.ztup1 = true;
          COPY(ztuple_z8z5stringzCz0z5boolz9)(&zgaz3493, zgsz3769);
          KILL(ztuple_z8z5stringzCz0z5boolz9)(&zgsz3769);
        } else {
          struct ztuple_z8z5stringzCz0z5boolz9 zgsz3768;
          CREATE(ztuple_z8z5stringzCz0z5boolz9)(&zgsz3768);
          COPY(sail_string)(&((&zgsz3768)->ztup0), "");
          zgsz3768.ztup1 = false;
          COPY(ztuple_z8z5stringzCz0z5boolz9)(&zgaz3493, zgsz3768);
          KILL(ztuple_z8z5stringzCz0z5boolz9)(&zgsz3768);
        }
      }
      sail_string zgsz3770;
      CREATE(sail_string)(&zgsz3770);
      {
        sail_string zres_str;
        CREATE(sail_string)(&zres_str);
        COPY(sail_string)(&zres_str, zgaz3493.ztup0);
        bool zreserved_malformed;
        zreserved_malformed = zgaz3493.ztup1;
        sail_string zraw_str;
        CREATE(sail_string)(&zraw_str);
        {
          bool zgaz3521;
          {
            if (zbounds_malformed) {  zgaz3521 = true;  } else {
              bool zgsz3771;
              if (zperms_malformed) {  zgsz3771 = true;  } else {  zgsz3771 = zreserved_malformed;  }
              zgaz3521 = zgsz3771;
            }
          }
          if (zgaz3521) {
            sail_string zgaz3524;
            CREATE(sail_string)(&zgaz3524);
            {
              uint64_t zgaz3523;
              {
                struct zEncMetadata zghz3392;
                zghz3392 = sailgen_capToMetadataBits(zcap);
                zgaz3523 = zghz3392.zbits;
              }
              {
                RECREATE(lbits)(&zghz3391);
                CONVERT_OF(lbits, fbits)(&zghz3391, zgaz3523, UINT64_C(32) , true);
                string_of_lbits(&zgaz3524, zghz3391);
              }
            }
            concat_str(&zraw_str, " raw:", zgaz3524);
            KILL(sail_string)(&zgaz3524);
          } else {  COPY(sail_string)(&zraw_str, "");  }
        }
        sail_string zcheri_mode;
        CREATE(sail_string)(&zcheri_mode);
        {
          __label__ case_675, case_674, finish_match_673;

          enum zExecutionMode zgaz3520;
          zgaz3520 = sailgen_getCapMode(zcap);
          {
            if ((zCapPtrMode != zgaz3520)) goto case_675;
            COPY(sail_string)(&zcheri_mode, "CapPtrMode");
            goto finish_match_673;
          }
        case_675: ;
          {
            /* complete */
            COPY(sail_string)(&zcheri_mode, "Integer");
            goto finish_match_673;
          }
        case_674: ;
        finish_match_673: ;
        }
        sail_string zgaz3519;
        CREATE(sail_string)(&zgaz3519);
        {
          sail_string zgaz3517;
          CREATE(sail_string)(&zgaz3517);
          {
            bool zgaz3494;
            zgaz3494 = zcap.ztag;
            if (zgaz3494) {  COPY(sail_string)(&zgaz3517, "1");  } else {  COPY(sail_string)(&zgaz3517, "0");  }
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
                uint64_t zgaz3495;
                zgaz3495 = zcap.zaddress;
                {
                  RECREATE(lbits)(&zghz3390);
                  CONVERT_OF(lbits, fbits)(&zghz3390, zgaz3495, UINT64_C(32) , true);
                  string_of_lbits(&zgaz3514, zghz3390);
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
                                bool zgaz3496;
                                zgaz3496 = zcap.zsealed;
                                if (zgaz3496) {  COPY(sail_string)(&zgaz3505, "1");  } else {
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
                                        uint64_t zgaz3497;
                                        zgaz3497 = zcap.zsd_perms;
                                        {
                                          RECREATE(lbits)(&zghz3389);
                                          CONVERT_OF(lbits, fbits)(&zghz3389, zgaz3497, UINT64_C(2) , true);
                                          string_of_lbits(&zgaz3500, zghz3389);
                                        }
                                      }
                                      sail_string zgaz3501;
                                      CREATE(sail_string)(&zgaz3501);
                                      {
                                        sail_string zgaz3499;
                                        CREATE(sail_string)(&zgaz3499);
                                        {
                                          sail_string zgaz3498;
                                          CREATE(sail_string)(&zgaz3498);
                                          concat_str(&zgaz3498, zraw_str, zres_str);
                                          concat_str(&zgaz3499, zcheri_mode, zgaz3498);
                                          KILL(sail_string)(&zgaz3498);
                                        }
                                        concat_str(&zgaz3501, " mode:", zgaz3499);
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
        concat_str(&zgsz3770, "t:", zgaz3519);
        KILL(sail_string)(&zgaz3519);
        KILL(sail_string)(&zcheri_mode);
        KILL(sail_string)(&zraw_str);
        KILL(sail_string)(&zres_str);
        goto finish_match_671;
      }
    case_672: ;
      sail_match_failure("capToString");
    finish_match_671: ;
      COPY(sail_string)(&zgsz3767, zgsz3770);
      KILL(sail_string)(&zgsz3770);
      KILL(ztuple_z8z5stringzCz0z5boolz9)(&zgaz3493);
      KILL(sail_string)(&zarch_perms_str);
      goto finish_match_669;
    }
  case_670: ;
    sail_match_failure("capToString");
  finish_match_669: ;
    COPY(sail_string)(&zgsz3761, zgsz3767);
    KILL(sail_string)(&zgsz3767);
    KILL(ztuple_z8z5stringzCz0z5boolz9)(&zgaz3490);
    KILL(sail_string)(&zlen_str);
    KILL(sail_string)(&ztop_str);
    KILL(sail_string)(&zbase_str);
    goto finish_match_664;
  }
case_665: ;
  sail_match_failure("capToString");
finish_match_664: ;
  COPY(sail_string)((*(&zcbz379)), zgsz3761);
  KILL(sail_string)(&zgsz3761);
  KILL(ztuple_z8z5stringzCz0z5stringzCz0z5boolz9)(&zgaz3471);
end_function_676: ;
  goto end_function_703;
end_block_exception_677: ;
  goto end_function_703;
end_function_703: ;
}



static void finish_sailgen_capToString(void)
{
  KILL(lbits)(&zghz3402);
  KILL(lbits)(&zghz3401);
  KILL(lbits)(&zghz3400);
  KILL(sail_int)(&zghz3399);
  KILL(lbits)(&zghz3398);
  KILL(lbits)(&zghz3397);
  KILL(sail_int)(&zghz3396);
  KILL(sail_int)(&zghz3395);
  KILL(lbits)(&zghz3394);


  KILL(lbits)(&zghz3391);
  KILL(lbits)(&zghz3390);
  KILL(lbits)(&zghz3389);
}

static uint64_t sailgen_getRepresentableAlignmentMask(uint64_t);

struct zCapability zghz3403;
sail_int zghz3404;
lbits zghz3405;
lbits zghz3406;
sail_int zghz3407;
lbits zghz3408;

static void startup_sailgen_getRepresentableAlignmentMask(void)
{

  CREATE(sail_int)(&zghz3404);
  CREATE(lbits)(&zghz3405);
  CREATE(lbits)(&zghz3406);
  CREATE(sail_int)(&zghz3407);
  CREATE(lbits)(&zghz3408);
}

static uint64_t sailgen_getRepresentableAlignmentMask(uint64_t zlen)
{
  __label__ case_680, finish_match_679, end_function_681, end_block_exception_682;

  uint64_t zcbz380;
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3529;
  {
    uint64_t zgaz3528;
    zgaz3528 = (UINT64_C(0b0) << 32) | zlen;
    zgaz3529 = sailgen_setCapBounds(zinfinite_cap, UINT64_C(0x00000000), zgaz3528);
  }
  {
    bool z_exact;
    z_exact = zgaz3529.ztup0;
    struct zCapability zghz3403;
    zghz3403 = zgaz3529.ztup1;
    bool zgaz3530;
    zgaz3530 = zghz3403.zinternal_exponent;
    if (zgaz3530) {
      int64_t ze;
      ze = zghz3403.zE;
      {
        bool zgaz3531;
        zgaz3531 = (ze >= INT64_C(0));
        unit zgsz3781;
        zgsz3781 = sail_assert(zgaz3531, "src/cheri_cap_common.sail:623.17-623.18");
      }
      uint64_t zgaz3534;
      {
        sbits zgaz3533;
        {
          int64_t zgaz3532;
          zgaz3532 = (ze + zcap_xe_width);
          {
            RECREATE(sail_int)(&zghz3407);
            CONVERT_OF(sail_int, mach_int)(&zghz3407, zgaz3532);
            RECREATE(lbits)(&zghz3408);
            sailgen_ones(&zghz3408, zghz3407);
            zgaz3533 = CONVERT_OF(sbits, lbits)(zghz3408, true);
          }
        }
        {
          RECREATE(sail_int)(&zghz3404);
          CONVERT_OF(sail_int, mach_int)(&zghz3404, INT64_C(32));
          RECREATE(lbits)(&zghz3405);
          CONVERT_OF(lbits, sbits)(&zghz3405, zgaz3533, true);
          RECREATE(lbits)(&zghz3406);
          sailgen_zzero_extend(&zghz3406, zghz3404, zghz3405);
          zgaz3534 = CONVERT_OF(fbits, lbits)(zghz3406, true);
        }
      }
      zcbz380 = (~(zgaz3534) & UINT64_C(0xFFFFFFFF));
    } else {  zcbz380 = UINT64_C(0xFFFFFFFF);  }
    goto finish_match_679;
  }
case_680: ;
  sail_match_failure("getRepresentableAlignmentMask");
finish_match_679: ;

end_function_681: ;
  return zcbz380;
end_block_exception_682: ;

  return UINT64_C(0xdeadc0de);
}



static void finish_sailgen_getRepresentableAlignmentMask(void)
{
  KILL(lbits)(&zghz3408);
  KILL(sail_int)(&zghz3407);
  KILL(lbits)(&zghz3406);
  KILL(lbits)(&zghz3405);
  KILL(sail_int)(&zghz3404);

}

static uint64_t sailgen_getRepresentableLength(uint64_t);

static uint64_t sailgen_getRepresentableLength(uint64_t zlen)
{
  __label__ end_function_684, end_block_exception_685;

  uint64_t zcbz381;
  uint64_t zm;
  zm = sailgen_getRepresentableAlignmentMask(zlen);
  uint64_t zgaz3536;
  {
    uint64_t zgaz3535;
    zgaz3535 = (~(zm) & UINT64_C(0xFFFFFFFF));
    zgaz3536 = ((zlen + zgaz3535) & UINT64_C(0xFFFFFFFF));
  }
  zcbz381 = (zgaz3536 & zm);


end_function_684: ;
  return zcbz381;
end_block_exception_685: ;

  return UINT64_C(0xdeadc0de);
}

static struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 sailgen_doCSetBounds(struct zCapability, uint64_t);

struct zCapability zghz3409;
struct zCapability zghz3410;
struct zCapability zghz3411;
lbits zghz3412;
sail_int zghz3413;
sail_int zghz3414;
lbits zghz3415;
lbits zghz3416;

static void startup_sailgen_doCSetBounds(void)
{



  CREATE(lbits)(&zghz3412);
  CREATE(sail_int)(&zghz3413);
  CREATE(sail_int)(&zghz3414);
  CREATE(lbits)(&zghz3415);
  CREATE(lbits)(&zghz3416);
}

static struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 sailgen_doCSetBounds(struct zCapability zinput, uint64_t zreq_len)
{
  __label__ case_688, finish_match_687, end_function_691, end_block_exception_692;

  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz382;
  uint64_t znewBase;
  znewBase = zinput.zaddress;
  uint64_t znewTop;
  {
    uint64_t zgaz3546;
    {
      RECREATE(sail_int)(&zghz3414);
      CONVERT_OF(sail_int, mach_int)(&zghz3414, INT64_C(33));
      RECREATE(lbits)(&zghz3415);
      CONVERT_OF(lbits, fbits)(&zghz3415, znewBase, UINT64_C(32) , true);
      RECREATE(lbits)(&zghz3416);
      sailgen_zzero_extend(&zghz3416, zghz3414, zghz3415);
      zgaz3546 = CONVERT_OF(fbits, lbits)(zghz3416, true);
    }
    znewTop = ((zgaz3546 + zreq_len) & UINT64_C(0b111111111111111111111111111111111));
  }
  bool zinBounds;
  {
    int64_t zgaz3545;
    {
      RECREATE(lbits)(&zghz3412);
      CONVERT_OF(lbits, fbits)(&zghz3412, zreq_len, UINT64_C(33) , true);
      RECREATE(sail_int)(&zghz3413);
      sail_unsigned(&zghz3413, zghz3412);
      zgaz3545 = CONVERT_OF(mach_int, sail_int)(zghz3413);
    }
    zinBounds = sailgen_inCapBoundsNoWrap(zinput, znewBase, zgaz3545);
  }
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3537;
  zgaz3537 = sailgen_setCapBounds(zinput, znewBase, znewTop);
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgsz3782;
  {
    __label__ case_690, finish_match_689;

    bool zexact;
    zexact = zgaz3537.ztup0;
    struct zCapability zghz3409;
    zghz3409 = zgaz3537.ztup1;
    bool zcond;
    {
      bool zgaz3544;
      {
        bool zgaz3540;
        {    if (zinBounds) {  zgaz3540 = zexact;  } else {  zgaz3540 = false;  }
        }
        zgaz3544 = not(zgaz3540);
      }
      bool zgsz3786;
      if (zgaz3544) {  zgsz3786 = true;  } else {
        bool zgaz3543;
        zgaz3543 = sailgen_boundsMalformed(zghz3409);
        bool zgsz3785;
        if (zgaz3543) {  zgsz3785 = true;  } else {
          bool zgaz3542;
          {
            bool zgaz3541;
            zgaz3541 = sailgen_capReservedValid(zghz3409);
            zgaz3542 = not(zgaz3541);
          }
          bool zgsz3784;
          if (zgaz3542) {  zgsz3784 = true;  } else {  zgsz3784 = sailgen_capIsSealed(zghz3409);  }
          zgsz3785 = zgsz3784;
        }
        zgsz3786 = zgsz3785;
      }
      zcond = zgsz3786;
    }
    struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3538;
    zgaz3538 = sailgen_setCapBounds(zinput, znewBase, znewTop);
    struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgsz3787;
    {
      bool zexactshadowz310;
      zexactshadowz310 = zgaz3538.ztup0;
      struct zCapability zghz3410;
      zghz3410 = zgaz3538.ztup1;
      struct zCapability zghz3411;
      zghz3411 = sailgen_clearTagIf(zghz3410, zcond);
      zgsz3787.ztup0 = zexactshadowz310;
      zgsz3787.ztup1 = zghz3411;
      goto finish_match_689;
    }
  case_690: ;
    sail_match_failure("doCSetBounds");
  finish_match_689: ;
    zgsz3782 = zgsz3787;
    goto finish_match_687;
  }
case_688: ;
  sail_match_failure("doCSetBounds");
finish_match_687: ;
  zcbz382 = zgsz3782;





end_function_691: ;
  return zcbz382;
end_block_exception_692: ;
  struct zCapability zcbz3115 = { .zB = UINT64_C(0xdeadc0de), .zE = INT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zap_m = UINT64_C(0xdeadc0de), .zinternal_exponent = false, .zreserved_0 = UINT64_C(0xdeadc0de), .zreserved_1 = UINT64_C(0xdeadc0de), .zsd_perms = UINT64_C(0xdeadc0de), .zsealed = false, .ztag = false };
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz3114 = { .ztup0 = false, .ztup1 = zcbz3115 };
  return zcbz3114;
}



static void finish_sailgen_doCSetBounds(void)
{
  KILL(lbits)(&zghz3416);
  KILL(lbits)(&zghz3415);
  KILL(sail_int)(&zghz3414);
  KILL(sail_int)(&zghz3413);
  KILL(lbits)(&zghz3412);



}

// register TestCap
static struct zCapability zTestCap;

// register TestAddr
static uint64_t zTestAddr;

// register TestLen
static uint64_t zTestLen;

static unit sailgen_main(unit);

struct zCapability zghz3417;
struct zCapability zghz3418;
struct zCapability zghz3419;
struct zCapability zghz3420;
struct zArchPerms zghz3421;
struct zEncMetadata zghz3422;

static void startup_sailgen_main(void)
{






}




static unit sailgen_main(unit zgsz3791)
{
  __label__ case_695, finish_match_694, end_function_698, end_block_exception_699;

  unit zcbz383;
  {
    zTestCap = znull_cap;
    unit zgsz3792;
    zgsz3792 = UNIT;
  }
  struct zCapability zghz3417;
  {
    bool zgaz3553;
    zgaz3553 = zTestCap.ztag;
    uint64_t zgaz3554;
    zgaz3554 = (zTestAddr << 32) | zTestAddr;
    zghz3417 = sailgen_bitsToCap(zgaz3553, zgaz3554);
  }
  uint64_t zcap_bits;
  {
    struct zEncMetadata zghz3422;
    zghz3422 = sailgen_capToMetadataBits(zghz3417);
    zcap_bits = zghz3422.zbits;
  }
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3547;
  zgaz3547 = sailgen_doCSetBounds(zTestCap, zTestLen);
  unit zgsz3793;
  {
    __label__ case_697, finish_match_696;

    bool zexact;
    zexact = zgaz3547.ztup0;
    struct zCapability zghz3418;
    zghz3418 = zgaz3547.ztup1;
    struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3548;
    zgaz3548 = sailgen_setCapAddr(zTestCap, zTestAddr);
    {
      bool zrepresentable;
      zrepresentable = zgaz3548.ztup0;
      struct zCapability zghz3419;
      zghz3419 = zgaz3548.ztup1;
      struct zoptionzIz8izCiz9zK zbounds;
      CREATE(zoptionzIz8izCiz9zK)(&zbounds);
      sailgen_getCapBounds(&zbounds, zTestCap);
      enum zExecutionMode zmode;
      zmode = sailgen_getCapMode(zTestCap);
      sail_string zcap_str;
      CREATE(sail_string)(&zcap_str);
      sailgen_capToString(&zcap_str, zTestCap);
      struct zCapability zghz3420;
      {
        struct ztuple_z8z5structz0zzArchPermszCz0z5enumz0zzExecutionModez9 zgaz3551;
        {
          struct zArchPerms zghz3421;
          zghz3421 = sailgen_getArchPermsLegalizzed(zTestCap);
          enum zExecutionMode zgaz3550;
          zgaz3550 = sailgen_getCapMode(zTestCap);
          zgaz3551.ztup0 = zghz3421;
          zgaz3551.ztup1 = zgaz3550;
        }
        zghz3420 = sailgen_setCapPermsAndMode(zTestCap, zgaz3551);
      }
      struct zoptionzIz8RArchPermszCEExecutionModez5z9zK zperms_mode;
      CREATE(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zperms_mode);
      sailgen_getCapPermsAndMode(&zperms_mode, zTestCap);
      uint64_t zlen;
      zlen = sailgen_getRepresentableLength(zTestAddr);
      uint64_t zmask;
      zmask = sailgen_getRepresentableAlignmentMask(zTestAddr);
      zgsz3793 = UNIT;
      KILL(zoptionzIz8RArchPermszCEExecutionModez5z9zK)(&zperms_mode);
      KILL(sail_string)(&zcap_str);
      KILL(zoptionzIz8izCiz9zK)(&zbounds);
      goto finish_match_696;
    }
  case_697: ;
    sail_match_failure("main");
  finish_match_696: ;
    goto finish_match_694;
  }
case_695: ;
  sail_match_failure("main");
finish_match_694: ;
  zcbz383 = zgsz3793;



end_function_698: ;
  return zcbz383;
end_block_exception_699: ;

  return UNIT;
}



static void finish_sailgen_main(void)
{






}

static unit sailgen_initializze_registers(unit);

struct zCapability zghz3423;
sail_int zghz3424;

static void startup_sailgen_initializze_registers(void)
{

  CREATE(sail_int)(&zghz3424);
}

static unit sailgen_initializze_registers(unit zgsz3798)
{
  __label__ end_function_701, end_block_exception_702;

  unit zcbz384;
  {
    struct zCapability zghz3423;
    zghz3423.zB = UINT64_C(0b0000000000);
    RECREATE(sail_int)(&zghz3424);
    CONVERT_OF(sail_int, mach_int)(&zghz3424, INT64_C(-7));
    zghz3423.zE = CONVERT_OF(mach_int, sail_int)(zghz3424);
    zghz3423.zT = UINT64_C(0b0000000000);
    zghz3423.zaddress = UINT64_C(0x00000000);
    zghz3423.zap_m = UINT64_C(0b00000);
    zghz3423.zinternal_exponent = false;
    zghz3423.zreserved_0 = UINT64_C(0b000);
    zghz3423.zreserved_1 = UINT64_C(0b0);
    zghz3423.zsd_perms = UINT64_C(0b00);
    zghz3423.zsealed = false;
    zghz3423.ztag = false;
    zTestCap = zghz3423;
    unit zgsz3802;
    zgsz3802 = UNIT;
  }
  {
    zTestAddr = UINT64_C(0x00000000);
    unit zgsz3801;
    zgsz3801 = UNIT;
  }
  zTestLen = UINT64_C(0b000000000000000000000000000000000);
  zcbz384 = UNIT;
end_function_701: ;
  return zcbz384;
end_block_exception_702: ;

  return UNIT;
}



static void finish_sailgen_initializze_registers(void)
{
  KILL(sail_int)(&zghz3424);

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



#ifdef __cplusplus
}
#endif
