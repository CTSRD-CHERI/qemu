#include "sail.h"
#include "rts.h"
#include "elf.h"

// enum write_kind
enum zwrite_kind { zWrite_plain, zWrite_conditional, zWrite_release, zWrite_exclusive, zWrite_exclusive_release, zWrite_RISCV_release, zWrite_RISCV_strong_release, zWrite_RISCV_conditional, zWrite_RISCV_conditional_release, zWrite_RISCV_conditional_strong_release, zWrite_X86_locked };

static bool eq_zwrite_kind(enum zwrite_kind op1, enum zwrite_kind op2) { return op1 == op2; }

static enum zwrite_kind UNDEFINED(zwrite_kind)(unit u) { return zWrite_plain; }

// enum trans_kind
enum ztrans_kind { zTransaction_start, zTransaction_commit, zTransaction_abort };

static bool eq_ztrans_kind(enum ztrans_kind op1, enum ztrans_kind op2) { return op1 == op2; }

static enum ztrans_kind UNDEFINED(ztrans_kind)(unit u) { return zTransaction_start; }

// struct tuple_(%string, %i)
struct ztuple_z8z5stringzCz0z5iz9 {
  sail_string ztup0;
  sail_int ztup1;
};

static void COPY(ztuple_z8z5stringzCz0z5iz9)(struct ztuple_z8z5stringzCz0z5iz9 *rop, const struct ztuple_z8z5stringzCz0z5iz9 op) {
  COPY(sail_string)(&rop->ztup0, op.ztup0);
  COPY(sail_int)(&rop->ztup1, op.ztup1);
}

static void CREATE(ztuple_z8z5stringzCz0z5iz9)(struct ztuple_z8z5stringzCz0z5iz9 *op) {
  CREATE(sail_string)(&op->ztup0);
  CREATE(sail_int)(&op->ztup1);
}

static void RECREATE(ztuple_z8z5stringzCz0z5iz9)(struct ztuple_z8z5stringzCz0z5iz9 *op) {
  RECREATE(sail_string)(&op->ztup0);
  RECREATE(sail_int)(&op->ztup1);
}

static void KILL(ztuple_z8z5stringzCz0z5iz9)(struct ztuple_z8z5stringzCz0z5iz9 *op) {
  KILL(sail_string)(&op->ztup0);
  KILL(sail_int)(&op->ztup1);
}

static bool EQUAL(ztuple_z8z5stringzCz0z5iz9)(struct ztuple_z8z5stringzCz0z5iz9 op1, struct ztuple_z8z5stringzCz0z5iz9 op2) {
  return EQUAL(sail_string)(op1.ztup0, op2.ztup0) && EQUAL(sail_int)(op1.ztup1, op2.ztup1);
}


// struct tuple_(%string, %i, %i)
struct ztuple_z8z5stringzCz0z5izCz0z5iz9 {
  sail_string ztup0;
  sail_int ztup1;
  sail_int ztup2;
};

static void COPY(ztuple_z8z5stringzCz0z5izCz0z5iz9)(struct ztuple_z8z5stringzCz0z5izCz0z5iz9 *rop, const struct ztuple_z8z5stringzCz0z5izCz0z5iz9 op) {
  COPY(sail_string)(&rop->ztup0, op.ztup0);
  COPY(sail_int)(&rop->ztup1, op.ztup1);
  COPY(sail_int)(&rop->ztup2, op.ztup2);
}

static void CREATE(ztuple_z8z5stringzCz0z5izCz0z5iz9)(struct ztuple_z8z5stringzCz0z5izCz0z5iz9 *op) {
  CREATE(sail_string)(&op->ztup0);
  CREATE(sail_int)(&op->ztup1);
  CREATE(sail_int)(&op->ztup2);
}

static void RECREATE(ztuple_z8z5stringzCz0z5izCz0z5iz9)(struct ztuple_z8z5stringzCz0z5izCz0z5iz9 *op) {
  RECREATE(sail_string)(&op->ztup0);
  RECREATE(sail_int)(&op->ztup1);
  RECREATE(sail_int)(&op->ztup2);
}

static void KILL(ztuple_z8z5stringzCz0z5izCz0z5iz9)(struct ztuple_z8z5stringzCz0z5izCz0z5iz9 *op) {
  KILL(sail_string)(&op->ztup0);
  KILL(sail_int)(&op->ztup1);
  KILL(sail_int)(&op->ztup2);
}

static bool EQUAL(ztuple_z8z5stringzCz0z5izCz0z5iz9)(struct ztuple_z8z5stringzCz0z5izCz0z5iz9 op1, struct ztuple_z8z5stringzCz0z5izCz0z5iz9 op2) {
  return EQUAL(sail_string)(op1.ztup0, op2.ztup0) && EQUAL(sail_int)(op1.ztup1, op2.ztup1) && EQUAL(sail_int)(op1.ztup2, op2.ztup2);
}


// struct tuple_(%string, %string)
struct ztuple_z8z5stringzCz0z5stringz9 {
  sail_string ztup0;
  sail_string ztup1;
};

static void COPY(ztuple_z8z5stringzCz0z5stringz9)(struct ztuple_z8z5stringzCz0z5stringz9 *rop, const struct ztuple_z8z5stringzCz0z5stringz9 op) {
  COPY(sail_string)(&rop->ztup0, op.ztup0);
  COPY(sail_string)(&rop->ztup1, op.ztup1);
}

static void CREATE(ztuple_z8z5stringzCz0z5stringz9)(struct ztuple_z8z5stringzCz0z5stringz9 *op) {
  CREATE(sail_string)(&op->ztup0);
  CREATE(sail_string)(&op->ztup1);
}

static void RECREATE(ztuple_z8z5stringzCz0z5stringz9)(struct ztuple_z8z5stringzCz0z5stringz9 *op) {
  RECREATE(sail_string)(&op->ztup0);
  RECREATE(sail_string)(&op->ztup1);
}

static void KILL(ztuple_z8z5stringzCz0z5stringz9)(struct ztuple_z8z5stringzCz0z5stringz9 *op) {
  KILL(sail_string)(&op->ztup0);
  KILL(sail_string)(&op->ztup1);
}

static bool EQUAL(ztuple_z8z5stringzCz0z5stringz9)(struct ztuple_z8z5stringzCz0z5stringz9 op1, struct ztuple_z8z5stringzCz0z5stringz9 op2) {
  return EQUAL(sail_string)(op1.ztup0, op2.ztup0) && EQUAL(sail_string)(op1.ztup1, op2.ztup1);
}

// union regfp
enum kind_zregfp { Kind_zRField, Kind_zRFull, Kind_zRSlice, Kind_zRSliceBit };

struct zregfp {
  enum kind_zregfp kind;
  union {
    struct { struct ztuple_z8z5stringzCz0z5stringz9 zRField; };
    struct { sail_string zRFull; };
    struct { struct ztuple_z8z5stringzCz0z5izCz0z5iz9 zRSlice; };
    struct { struct ztuple_z8z5stringzCz0z5iz9 zRSliceBit; };
  };
};

static void CREATE(zregfp)(struct zregfp *op)
{
  op->kind = Kind_zRField;
  CREATE(ztuple_z8z5stringzCz0z5stringz9)(&op->zRField);
}

static void RECREATE(zregfp)(struct zregfp *op) {}

static void KILL(zregfp)(struct zregfp *op)
{
  if (op->kind == Kind_zRField) {
    KILL(ztuple_z8z5stringzCz0z5stringz9)(&op->zRField);
  } else if (op->kind == Kind_zRFull) {
    KILL(sail_string)(&op->zRFull);
  } else if (op->kind == Kind_zRSlice) {
    KILL(ztuple_z8z5stringzCz0z5izCz0z5iz9)(&op->zRSlice);
  } else if (op->kind == Kind_zRSliceBit){
    KILL(ztuple_z8z5stringzCz0z5iz9)(&op->zRSliceBit);
  };
}

static void COPY(zregfp)(struct zregfp *rop, struct zregfp op)
{
  if (rop->kind == Kind_zRField) {
    KILL(ztuple_z8z5stringzCz0z5stringz9)(&rop->zRField);
  } else if (rop->kind == Kind_zRFull) {
    KILL(sail_string)(&rop->zRFull);
  } else if (rop->kind == Kind_zRSlice) {
    KILL(ztuple_z8z5stringzCz0z5izCz0z5iz9)(&rop->zRSlice);
  } else if (rop->kind == Kind_zRSliceBit){
    KILL(ztuple_z8z5stringzCz0z5iz9)(&rop->zRSliceBit);
  };
  rop->kind = op.kind;
  if (op.kind == Kind_zRField) {
  
  CREATE(ztuple_z8z5stringzCz0z5stringz9)(&rop->zRField); COPY(ztuple_z8z5stringzCz0z5stringz9)(&rop->zRField, op.zRField);
  } else if (op.kind == Kind_zRFull) {
    CREATE(sail_string)(&rop->zRFull); COPY(sail_string)(&rop->zRFull, op.zRFull);
  } else if (op.kind == Kind_zRSlice) {
  
  CREATE(ztuple_z8z5stringzCz0z5izCz0z5iz9)(&rop->zRSlice); COPY(ztuple_z8z5stringzCz0z5izCz0z5iz9)(&rop->zRSlice, op.zRSlice);
  } else if (op.kind == Kind_zRSliceBit){
  
  CREATE(ztuple_z8z5stringzCz0z5iz9)(&rop->zRSliceBit); COPY(ztuple_z8z5stringzCz0z5iz9)(&rop->zRSliceBit, op.zRSliceBit);
  }
}

static bool EQUAL(zregfp)(struct zregfp op1, struct zregfp op2) {
  if (op1.kind == Kind_zRField && op2.kind == Kind_zRField) {
    return EQUAL(ztuple_z8z5stringzCz0z5stringz9)(op1.zRField, op2.zRField);
  } else if (op1.kind == Kind_zRFull && op2.kind == Kind_zRFull) {
    return EQUAL(sail_string)(op1.zRFull, op2.zRFull);
  } else if (op1.kind == Kind_zRSlice && op2.kind == Kind_zRSlice) {
    return EQUAL(ztuple_z8z5stringzCz0z5izCz0z5iz9)(op1.zRSlice, op2.zRSlice);
  } else if (op1.kind == Kind_zRSliceBit && op2.kind == Kind_zRSliceBit) {
    return EQUAL(ztuple_z8z5stringzCz0z5iz9)(op1.zRSliceBit, op2.zRSliceBit);
  } else return false;
}

static void sailgen_RField(struct zregfp *rop, struct ztuple_z8z5stringzCz0z5stringz9 op)
{
  if (rop->kind == Kind_zRField) {
    KILL(ztuple_z8z5stringzCz0z5stringz9)(&rop->zRField);
  } else if (rop->kind == Kind_zRFull) {
    KILL(sail_string)(&rop->zRFull);
  } else if (rop->kind == Kind_zRSlice) {
    KILL(ztuple_z8z5stringzCz0z5izCz0z5iz9)(&rop->zRSlice);
  } else if (rop->kind == Kind_zRSliceBit){
    KILL(ztuple_z8z5stringzCz0z5iz9)(&rop->zRSliceBit);
  }
  rop->kind = Kind_zRField;
  CREATE(ztuple_z8z5stringzCz0z5stringz9)(&rop->zRField);
  COPY(ztuple_z8z5stringzCz0z5stringz9)(&rop->zRField, op);
  
}

static void sailgen_RFull(struct zregfp *rop, sail_string op)
{
  if (rop->kind == Kind_zRField) {
    KILL(ztuple_z8z5stringzCz0z5stringz9)(&rop->zRField);
  } else if (rop->kind == Kind_zRFull) {
    KILL(sail_string)(&rop->zRFull);
  } else if (rop->kind == Kind_zRSlice) {
    KILL(ztuple_z8z5stringzCz0z5izCz0z5iz9)(&rop->zRSlice);
  } else if (rop->kind == Kind_zRSliceBit){
    KILL(ztuple_z8z5stringzCz0z5iz9)(&rop->zRSliceBit);
  }
  rop->kind = Kind_zRFull;
  CREATE(sail_string)(&rop->zRFull);
  COPY(sail_string)(&rop->zRFull, op);
  
}

static void sailgen_RSlice(struct zregfp *rop, struct ztuple_z8z5stringzCz0z5izCz0z5iz9 op)
{
  if (rop->kind == Kind_zRField) {
    KILL(ztuple_z8z5stringzCz0z5stringz9)(&rop->zRField);
  } else if (rop->kind == Kind_zRFull) {
    KILL(sail_string)(&rop->zRFull);
  } else if (rop->kind == Kind_zRSlice) {
    KILL(ztuple_z8z5stringzCz0z5izCz0z5iz9)(&rop->zRSlice);
  } else if (rop->kind == Kind_zRSliceBit){
    KILL(ztuple_z8z5stringzCz0z5iz9)(&rop->zRSliceBit);
  }
  rop->kind = Kind_zRSlice;
  CREATE(ztuple_z8z5stringzCz0z5izCz0z5iz9)(&rop->zRSlice);
  COPY(ztuple_z8z5stringzCz0z5izCz0z5iz9)(&rop->zRSlice, op);
  
}

static void sailgen_RSliceBit(struct zregfp *rop, struct ztuple_z8z5stringzCz0z5iz9 op)
{
  if (rop->kind == Kind_zRField) {
    KILL(ztuple_z8z5stringzCz0z5stringz9)(&rop->zRField);
  } else if (rop->kind == Kind_zRFull) {
    KILL(sail_string)(&rop->zRFull);
  } else if (rop->kind == Kind_zRSlice) {
    KILL(ztuple_z8z5stringzCz0z5izCz0z5iz9)(&rop->zRSlice);
  } else if (rop->kind == Kind_zRSliceBit){
    KILL(ztuple_z8z5stringzCz0z5iz9)(&rop->zRSliceBit);
  }
  rop->kind = Kind_zRSliceBit;
  CREATE(ztuple_z8z5stringzCz0z5iz9)(&rop->zRSliceBit);
  COPY(ztuple_z8z5stringzCz0z5iz9)(&rop->zRSliceBit, op);
  
}

// enum read_kind
enum zread_kind { zRead_plain, zRead_reserve, zRead_acquire, zRead_exclusive, zRead_exclusive_acquire, zRead_stream, zRead_RISCV_acquire, zRead_RISCV_strong_acquire, zRead_RISCV_reserved, zRead_RISCV_reserved_acquire, zRead_RISCV_reserved_strong_acquire, zRead_X86_locked };

static bool eq_zread_kind(enum zread_kind op1, enum zread_kind op2) { return op1 == op2; }

static enum zread_kind UNDEFINED(zread_kind)(unit u) { return zRead_plain; }

// union option
enum kind_zoption { Kind_zNone };

struct zoption {
  enum kind_zoption kind;
  union {struct { unit zNone; };};
};

static void CREATE(zoption)(struct zoption *op)
{
  op->kind = Kind_zNone;
  
}

static void RECREATE(zoption)(struct zoption *op) {}

static void KILL(zoption)(struct zoption *op)
{
  if (op->kind == Kind_zNone){
    /* do nothing */
  };
}

static void COPY(zoption)(struct zoption *rop, struct zoption op)
{
  if (rop->kind == Kind_zNone){
    /* do nothing */
  };
  rop->kind = op.kind;
  if (op.kind == Kind_zNone){
    rop->zNone = op.zNone;
  }
}

static bool EQUAL(zoption)(struct zoption op1, struct zoption op2) {
  if (op1.kind == Kind_zNone && op2.kind == Kind_zNone) {
    return EQUAL(unit)(op1.zNone, op2.zNone);
  } else return false;
}

static void sailgen_None(struct zoption *rop, unit op)
{
  if (rop->kind == Kind_zNone){
    /* do nothing */
  }
  rop->kind = Kind_zNone;
  rop->zNone = op;
}

// union niafp
enum kind_zniafp { Kind_zNIAFP_concrete_address, Kind_zNIAFP_indirect_address, Kind_zNIAFP_successor };

struct zniafp {
  enum kind_zniafp kind;
  union {
    struct { uint64_t zNIAFP_concrete_address; };
    struct { unit zNIAFP_indirect_address; };
    struct { unit zNIAFP_successor; };
  };
};

static void CREATE(zniafp)(struct zniafp *op)
{
  op->kind = Kind_zNIAFP_concrete_address;
  
}

static void RECREATE(zniafp)(struct zniafp *op) {}

static void KILL(zniafp)(struct zniafp *op)
{
  if (op->kind == Kind_zNIAFP_concrete_address) {
    /* do nothing */
  } else if (op->kind == Kind_zNIAFP_indirect_address) {
    /* do nothing */
  } else if (op->kind == Kind_zNIAFP_successor){
    /* do nothing */
  };
}

static void COPY(zniafp)(struct zniafp *rop, struct zniafp op)
{
  if (rop->kind == Kind_zNIAFP_concrete_address) {
    /* do nothing */
  } else if (rop->kind == Kind_zNIAFP_indirect_address) {
    /* do nothing */
  } else if (rop->kind == Kind_zNIAFP_successor){
    /* do nothing */
  };
  rop->kind = op.kind;
  if (op.kind == Kind_zNIAFP_concrete_address) {
    rop->zNIAFP_concrete_address = op.zNIAFP_concrete_address;
  } else if (op.kind == Kind_zNIAFP_indirect_address) {
    rop->zNIAFP_indirect_address = op.zNIAFP_indirect_address;
  } else if (op.kind == Kind_zNIAFP_successor){
    rop->zNIAFP_successor = op.zNIAFP_successor;
  }
}

static bool EQUAL(zniafp)(struct zniafp op1, struct zniafp op2) {
  if (op1.kind == Kind_zNIAFP_concrete_address && op2.kind == Kind_zNIAFP_concrete_address) {
    return EQUAL(fbits)(op1.zNIAFP_concrete_address, op2.zNIAFP_concrete_address);
  } else if (op1.kind == Kind_zNIAFP_indirect_address && op2.kind == Kind_zNIAFP_indirect_address) {
    return EQUAL(unit)(op1.zNIAFP_indirect_address, op2.zNIAFP_indirect_address);
  } else if (op1.kind == Kind_zNIAFP_successor && op2.kind == Kind_zNIAFP_successor) {
    return EQUAL(unit)(op1.zNIAFP_successor, op2.zNIAFP_successor);
  } else return false;
}

static void sailgen_NIAFP_concrete_address(struct zniafp *rop, uint64_t op)
{
  if (rop->kind == Kind_zNIAFP_concrete_address) {
    /* do nothing */
  } else if (rop->kind == Kind_zNIAFP_indirect_address) {
    /* do nothing */
  } else if (rop->kind == Kind_zNIAFP_successor){
    /* do nothing */
  }
  rop->kind = Kind_zNIAFP_concrete_address;
  rop->zNIAFP_concrete_address = op;
}

static void sailgen_NIAFP_indirect_address(struct zniafp *rop, unit op)
{
  if (rop->kind == Kind_zNIAFP_concrete_address) {
    /* do nothing */
  } else if (rop->kind == Kind_zNIAFP_indirect_address) {
    /* do nothing */
  } else if (rop->kind == Kind_zNIAFP_successor){
    /* do nothing */
  }
  rop->kind = Kind_zNIAFP_indirect_address;
  rop->zNIAFP_indirect_address = op;
}

static void sailgen_NIAFP_successor(struct zniafp *rop, unit op)
{
  if (rop->kind == Kind_zNIAFP_concrete_address) {
    /* do nothing */
  } else if (rop->kind == Kind_zNIAFP_indirect_address) {
    /* do nothing */
  } else if (rop->kind == Kind_zNIAFP_successor){
    /* do nothing */
  }
  rop->kind = Kind_zNIAFP_successor;
  rop->zNIAFP_successor = op;
}



// union diafp
enum kind_zdiafp { Kind_zDIAFP_concrete, Kind_zDIAFP_none, Kind_zDIAFP_reg };

struct zdiafp {
  enum kind_zdiafp kind;
  union {
    struct { uint64_t zDIAFP_concrete; };
    struct { unit zDIAFP_none; };
    struct { struct zregfp zDIAFP_reg; };
  };
};

static void CREATE(zdiafp)(struct zdiafp *op)
{
  op->kind = Kind_zDIAFP_concrete;
  
}

static void RECREATE(zdiafp)(struct zdiafp *op) {}

static void KILL(zdiafp)(struct zdiafp *op)
{
  if (op->kind == Kind_zDIAFP_concrete) {
    /* do nothing */
  } else if (op->kind == Kind_zDIAFP_none) {
    /* do nothing */
  } else if (op->kind == Kind_zDIAFP_reg){
    KILL(zregfp)(&op->zDIAFP_reg);
  };
}

static void COPY(zdiafp)(struct zdiafp *rop, struct zdiafp op)
{
  if (rop->kind == Kind_zDIAFP_concrete) {
    /* do nothing */
  } else if (rop->kind == Kind_zDIAFP_none) {
    /* do nothing */
  } else if (rop->kind == Kind_zDIAFP_reg){
    KILL(zregfp)(&rop->zDIAFP_reg);
  };
  rop->kind = op.kind;
  if (op.kind == Kind_zDIAFP_concrete) {
    rop->zDIAFP_concrete = op.zDIAFP_concrete;
  } else if (op.kind == Kind_zDIAFP_none) {
    rop->zDIAFP_none = op.zDIAFP_none;
  } else if (op.kind == Kind_zDIAFP_reg){
    CREATE(zregfp)(&rop->zDIAFP_reg); COPY(zregfp)(&rop->zDIAFP_reg, op.zDIAFP_reg);
  }
}

static bool EQUAL(zdiafp)(struct zdiafp op1, struct zdiafp op2) {
  if (op1.kind == Kind_zDIAFP_concrete && op2.kind == Kind_zDIAFP_concrete) {
    return EQUAL(fbits)(op1.zDIAFP_concrete, op2.zDIAFP_concrete);
  } else if (op1.kind == Kind_zDIAFP_none && op2.kind == Kind_zDIAFP_none) {
    return EQUAL(unit)(op1.zDIAFP_none, op2.zDIAFP_none);
  } else if (op1.kind == Kind_zDIAFP_reg && op2.kind == Kind_zDIAFP_reg) {
    return EQUAL(zregfp)(op1.zDIAFP_reg, op2.zDIAFP_reg);
  } else return false;
}

static void sailgen_DIAFP_concrete(struct zdiafp *rop, uint64_t op)
{
  if (rop->kind == Kind_zDIAFP_concrete) {
    /* do nothing */
  } else if (rop->kind == Kind_zDIAFP_none) {
    /* do nothing */
  } else if (rop->kind == Kind_zDIAFP_reg){
    KILL(zregfp)(&rop->zDIAFP_reg);
  }
  rop->kind = Kind_zDIAFP_concrete;
  rop->zDIAFP_concrete = op;
}

static void sailgen_DIAFP_none(struct zdiafp *rop, unit op)
{
  if (rop->kind == Kind_zDIAFP_concrete) {
    /* do nothing */
  } else if (rop->kind == Kind_zDIAFP_none) {
    /* do nothing */
  } else if (rop->kind == Kind_zDIAFP_reg){
    KILL(zregfp)(&rop->zDIAFP_reg);
  }
  rop->kind = Kind_zDIAFP_none;
  rop->zDIAFP_none = op;
}

static void sailgen_DIAFP_reg(struct zdiafp *rop, struct zregfp op)
{
  if (rop->kind == Kind_zDIAFP_concrete) {
    /* do nothing */
  } else if (rop->kind == Kind_zDIAFP_none) {
    /* do nothing */
  } else if (rop->kind == Kind_zDIAFP_reg){
    KILL(zregfp)(&rop->zDIAFP_reg);
  }
  rop->kind = Kind_zDIAFP_reg;
  CREATE(zregfp)(&rop->zDIAFP_reg);
  COPY(zregfp)(&rop->zDIAFP_reg, op);
  
}

// enum cache_op_kind
enum zcache_op_kind { zCache_op_D_IVAC, zCache_op_D_ISW, zCache_op_D_CSW, zCache_op_D_CISW, zCache_op_D_ZVA, zCache_op_D_CVAC, zCache_op_D_CVAU, zCache_op_D_CIVAC, zCache_op_I_IALLUIS, zCache_op_I_IALLU, zCache_op_I_IVAU };

static bool eq_zcache_op_kind(enum zcache_op_kind op1, enum zcache_op_kind op2) { return op1 == op2; }

static enum zcache_op_kind UNDEFINED(zcache_op_kind)(unit u) { return zCache_op_D_IVAC; }

// enum a64_barrier_type
enum za64_barrier_type { zA64_barrier_all, zA64_barrier_LD, zA64_barrier_ST };

static bool eq_za64_barrier_type(enum za64_barrier_type op1, enum za64_barrier_type op2) { return op1 == op2; }

static enum za64_barrier_type UNDEFINED(za64_barrier_type)(unit u) { return zA64_barrier_all; }

// enum a64_barrier_domain
enum za64_barrier_domain { zA64_FullShare, zA64_InnerShare, zA64_OuterShare, zA64_NonShare };

static bool eq_za64_barrier_domain(enum za64_barrier_domain op1, enum za64_barrier_domain op2) { return op1 == op2; }

static enum za64_barrier_domain UNDEFINED(za64_barrier_domain)(unit u) { return zA64_FullShare; }

// struct tuple_(%enum za64_barrier_domain, %enum za64_barrier_type)
struct ztuple_z8z5enumz0zza64_barrier_domainzCz0z5enumz0zza64_barrier_typez9 {
  enum za64_barrier_domain ztup0;
  enum za64_barrier_type ztup1;
};

static void COPY(ztuple_z8z5enumz0zza64_barrier_domainzCz0z5enumz0zza64_barrier_typez9)(struct ztuple_z8z5enumz0zza64_barrier_domainzCz0z5enumz0zza64_barrier_typez9 *rop, const struct ztuple_z8z5enumz0zza64_barrier_domainzCz0z5enumz0zza64_barrier_typez9 op) {
  rop->ztup0 = op.ztup0;
  rop->ztup1 = op.ztup1;
}

static bool EQUAL(ztuple_z8z5enumz0zza64_barrier_domainzCz0z5enumz0zza64_barrier_typez9)(struct ztuple_z8z5enumz0zza64_barrier_domainzCz0z5enumz0zza64_barrier_typez9 op1, struct ztuple_z8z5enumz0zza64_barrier_domainzCz0z5enumz0zza64_barrier_typez9 op2) {
  return EQUAL(za64_barrier_domain)(op1.ztup0, op2.ztup0) && EQUAL(za64_barrier_type)(op1.ztup1, op2.ztup1);
}

// union barrier_kind
enum kind_zbarrier_kind { Kind_zBarrier_DMB, Kind_zBarrier_DSB, Kind_zBarrier_Eieio, Kind_zBarrier_ISB, Kind_zBarrier_Isync, Kind_zBarrier_LwSync, Kind_zBarrier_MIPS_SYNC, Kind_zBarrier_RISCV_i, Kind_zBarrier_RISCV_r_r, Kind_zBarrier_RISCV_r_rw, Kind_zBarrier_RISCV_r_w, Kind_zBarrier_RISCV_rw_r, Kind_zBarrier_RISCV_rw_rw, Kind_zBarrier_RISCV_rw_w, Kind_zBarrier_RISCV_tso, Kind_zBarrier_RISCV_w_r, Kind_zBarrier_RISCV_w_rw, Kind_zBarrier_RISCV_w_w, Kind_zBarrier_Sync, Kind_zBarrier_x86_MFENCE };

struct zbarrier_kind {
  enum kind_zbarrier_kind kind;
  union {
    struct { struct ztuple_z8z5enumz0zza64_barrier_domainzCz0z5enumz0zza64_barrier_typez9 zBarrier_DMB; };
    struct { struct ztuple_z8z5enumz0zza64_barrier_domainzCz0z5enumz0zza64_barrier_typez9 zBarrier_DSB; };
    struct { unit zBarrier_Eieio; };
    struct { unit zBarrier_ISB; };
    struct { unit zBarrier_Isync; };
    struct { unit zBarrier_LwSync; };
    struct { unit zBarrier_MIPS_SYNC; };
    struct { unit zBarrier_RISCV_i; };
    struct { unit zBarrier_RISCV_r_r; };
    struct { unit zBarrier_RISCV_r_rw; };
    struct { unit zBarrier_RISCV_r_w; };
    struct { unit zBarrier_RISCV_rw_r; };
    struct { unit zBarrier_RISCV_rw_rw; };
    struct { unit zBarrier_RISCV_rw_w; };
    struct { unit zBarrier_RISCV_tso; };
    struct { unit zBarrier_RISCV_w_r; };
    struct { unit zBarrier_RISCV_w_rw; };
    struct { unit zBarrier_RISCV_w_w; };
    struct { unit zBarrier_Sync; };
    struct { unit zBarrier_x86_MFENCE; };
  };
};

static void CREATE(zbarrier_kind)(struct zbarrier_kind *op)
{
  op->kind = Kind_zBarrier_DMB;
  
}

static void RECREATE(zbarrier_kind)(struct zbarrier_kind *op) {}

static void KILL(zbarrier_kind)(struct zbarrier_kind *op)
{
  if (op->kind == Kind_zBarrier_DMB) {
    /* do nothing */
  } else if (op->kind == Kind_zBarrier_DSB) {
    /* do nothing */
  } else if (op->kind == Kind_zBarrier_Eieio) {
    /* do nothing */
  } else if (op->kind == Kind_zBarrier_ISB) {
    /* do nothing */
  } else if (op->kind == Kind_zBarrier_Isync) {
    /* do nothing */
  } else if (op->kind == Kind_zBarrier_LwSync) {
    /* do nothing */
  } else if (op->kind == Kind_zBarrier_MIPS_SYNC) {
    /* do nothing */
  } else if (op->kind == Kind_zBarrier_RISCV_i) {
    /* do nothing */
  } else if (op->kind == Kind_zBarrier_RISCV_r_r) {
    /* do nothing */
  } else if (op->kind == Kind_zBarrier_RISCV_r_rw) {
    /* do nothing */
  } else if (op->kind == Kind_zBarrier_RISCV_r_w) {
    /* do nothing */
  } else if (op->kind == Kind_zBarrier_RISCV_rw_r) {
    /* do nothing */
  } else if (op->kind == Kind_zBarrier_RISCV_rw_rw) {
    /* do nothing */
  } else if (op->kind == Kind_zBarrier_RISCV_rw_w) {
    /* do nothing */
  } else if (op->kind == Kind_zBarrier_RISCV_tso) {
    /* do nothing */
  } else if (op->kind == Kind_zBarrier_RISCV_w_r) {
    /* do nothing */
  } else if (op->kind == Kind_zBarrier_RISCV_w_rw) {
    /* do nothing */
  } else if (op->kind == Kind_zBarrier_RISCV_w_w) {
    /* do nothing */
  } else if (op->kind == Kind_zBarrier_Sync) {
    /* do nothing */
  } else if (op->kind == Kind_zBarrier_x86_MFENCE){
    /* do nothing */
  };
}

static void COPY(zbarrier_kind)(struct zbarrier_kind *rop, struct zbarrier_kind op)
{
  if (rop->kind == Kind_zBarrier_DMB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_DSB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Eieio) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_ISB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Isync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_LwSync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_MIPS_SYNC) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_i) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_tso) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Sync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_x86_MFENCE){
    /* do nothing */
  };
  rop->kind = op.kind;
  if (op.kind == Kind_zBarrier_DMB) {
    rop->zBarrier_DMB = op.zBarrier_DMB;
  } else if (op.kind == Kind_zBarrier_DSB) {
    rop->zBarrier_DSB = op.zBarrier_DSB;
  } else if (op.kind == Kind_zBarrier_Eieio) {
    rop->zBarrier_Eieio = op.zBarrier_Eieio;
  } else if (op.kind == Kind_zBarrier_ISB) {
    rop->zBarrier_ISB = op.zBarrier_ISB;
  } else if (op.kind == Kind_zBarrier_Isync) {
    rop->zBarrier_Isync = op.zBarrier_Isync;
  } else if (op.kind == Kind_zBarrier_LwSync) {
    rop->zBarrier_LwSync = op.zBarrier_LwSync;
  } else if (op.kind == Kind_zBarrier_MIPS_SYNC) {
    rop->zBarrier_MIPS_SYNC = op.zBarrier_MIPS_SYNC;
  } else if (op.kind == Kind_zBarrier_RISCV_i) {
    rop->zBarrier_RISCV_i = op.zBarrier_RISCV_i;
  } else if (op.kind == Kind_zBarrier_RISCV_r_r) {
    rop->zBarrier_RISCV_r_r = op.zBarrier_RISCV_r_r;
  } else if (op.kind == Kind_zBarrier_RISCV_r_rw) {
    rop->zBarrier_RISCV_r_rw = op.zBarrier_RISCV_r_rw;
  } else if (op.kind == Kind_zBarrier_RISCV_r_w) {
    rop->zBarrier_RISCV_r_w = op.zBarrier_RISCV_r_w;
  } else if (op.kind == Kind_zBarrier_RISCV_rw_r) {
    rop->zBarrier_RISCV_rw_r = op.zBarrier_RISCV_rw_r;
  } else if (op.kind == Kind_zBarrier_RISCV_rw_rw) {
    rop->zBarrier_RISCV_rw_rw = op.zBarrier_RISCV_rw_rw;
  } else if (op.kind == Kind_zBarrier_RISCV_rw_w) {
    rop->zBarrier_RISCV_rw_w = op.zBarrier_RISCV_rw_w;
  } else if (op.kind == Kind_zBarrier_RISCV_tso) {
    rop->zBarrier_RISCV_tso = op.zBarrier_RISCV_tso;
  } else if (op.kind == Kind_zBarrier_RISCV_w_r) {
    rop->zBarrier_RISCV_w_r = op.zBarrier_RISCV_w_r;
  } else if (op.kind == Kind_zBarrier_RISCV_w_rw) {
    rop->zBarrier_RISCV_w_rw = op.zBarrier_RISCV_w_rw;
  } else if (op.kind == Kind_zBarrier_RISCV_w_w) {
    rop->zBarrier_RISCV_w_w = op.zBarrier_RISCV_w_w;
  } else if (op.kind == Kind_zBarrier_Sync) {
    rop->zBarrier_Sync = op.zBarrier_Sync;
  } else if (op.kind == Kind_zBarrier_x86_MFENCE){
    rop->zBarrier_x86_MFENCE = op.zBarrier_x86_MFENCE;
  }
}

static bool EQUAL(zbarrier_kind)(struct zbarrier_kind op1, struct zbarrier_kind op2) {
  if (op1.kind == Kind_zBarrier_DMB && op2.kind == Kind_zBarrier_DMB) {
  
  return EQUAL(ztuple_z8z5enumz0zza64_barrier_domainzCz0z5enumz0zza64_barrier_typez9)(op1.zBarrier_DMB, op2.zBarrier_DMB);
  } else if (op1.kind == Kind_zBarrier_DSB && op2.kind == Kind_zBarrier_DSB) {
  
  return EQUAL(ztuple_z8z5enumz0zza64_barrier_domainzCz0z5enumz0zza64_barrier_typez9)(op1.zBarrier_DSB, op2.zBarrier_DSB);
  } else if (op1.kind == Kind_zBarrier_Eieio && op2.kind == Kind_zBarrier_Eieio) {
    return EQUAL(unit)(op1.zBarrier_Eieio, op2.zBarrier_Eieio);
  } else if (op1.kind == Kind_zBarrier_ISB && op2.kind == Kind_zBarrier_ISB) {
    return EQUAL(unit)(op1.zBarrier_ISB, op2.zBarrier_ISB);
  } else if (op1.kind == Kind_zBarrier_Isync && op2.kind == Kind_zBarrier_Isync) {
    return EQUAL(unit)(op1.zBarrier_Isync, op2.zBarrier_Isync);
  } else if (op1.kind == Kind_zBarrier_LwSync && op2.kind == Kind_zBarrier_LwSync) {
    return EQUAL(unit)(op1.zBarrier_LwSync, op2.zBarrier_LwSync);
  } else if (op1.kind == Kind_zBarrier_MIPS_SYNC && op2.kind == Kind_zBarrier_MIPS_SYNC) {
    return EQUAL(unit)(op1.zBarrier_MIPS_SYNC, op2.zBarrier_MIPS_SYNC);
  } else if (op1.kind == Kind_zBarrier_RISCV_i && op2.kind == Kind_zBarrier_RISCV_i) {
    return EQUAL(unit)(op1.zBarrier_RISCV_i, op2.zBarrier_RISCV_i);
  } else if (op1.kind == Kind_zBarrier_RISCV_r_r && op2.kind == Kind_zBarrier_RISCV_r_r) {
    return EQUAL(unit)(op1.zBarrier_RISCV_r_r, op2.zBarrier_RISCV_r_r);
  } else if (op1.kind == Kind_zBarrier_RISCV_r_rw && op2.kind == Kind_zBarrier_RISCV_r_rw) {
    return EQUAL(unit)(op1.zBarrier_RISCV_r_rw, op2.zBarrier_RISCV_r_rw);
  } else if (op1.kind == Kind_zBarrier_RISCV_r_w && op2.kind == Kind_zBarrier_RISCV_r_w) {
    return EQUAL(unit)(op1.zBarrier_RISCV_r_w, op2.zBarrier_RISCV_r_w);
  } else if (op1.kind == Kind_zBarrier_RISCV_rw_r && op2.kind == Kind_zBarrier_RISCV_rw_r) {
    return EQUAL(unit)(op1.zBarrier_RISCV_rw_r, op2.zBarrier_RISCV_rw_r);
  } else if (op1.kind == Kind_zBarrier_RISCV_rw_rw && op2.kind == Kind_zBarrier_RISCV_rw_rw) {
    return EQUAL(unit)(op1.zBarrier_RISCV_rw_rw, op2.zBarrier_RISCV_rw_rw);
  } else if (op1.kind == Kind_zBarrier_RISCV_rw_w && op2.kind == Kind_zBarrier_RISCV_rw_w) {
    return EQUAL(unit)(op1.zBarrier_RISCV_rw_w, op2.zBarrier_RISCV_rw_w);
  } else if (op1.kind == Kind_zBarrier_RISCV_tso && op2.kind == Kind_zBarrier_RISCV_tso) {
    return EQUAL(unit)(op1.zBarrier_RISCV_tso, op2.zBarrier_RISCV_tso);
  } else if (op1.kind == Kind_zBarrier_RISCV_w_r && op2.kind == Kind_zBarrier_RISCV_w_r) {
    return EQUAL(unit)(op1.zBarrier_RISCV_w_r, op2.zBarrier_RISCV_w_r);
  } else if (op1.kind == Kind_zBarrier_RISCV_w_rw && op2.kind == Kind_zBarrier_RISCV_w_rw) {
    return EQUAL(unit)(op1.zBarrier_RISCV_w_rw, op2.zBarrier_RISCV_w_rw);
  } else if (op1.kind == Kind_zBarrier_RISCV_w_w && op2.kind == Kind_zBarrier_RISCV_w_w) {
    return EQUAL(unit)(op1.zBarrier_RISCV_w_w, op2.zBarrier_RISCV_w_w);
  } else if (op1.kind == Kind_zBarrier_Sync && op2.kind == Kind_zBarrier_Sync) {
    return EQUAL(unit)(op1.zBarrier_Sync, op2.zBarrier_Sync);
  } else if (op1.kind == Kind_zBarrier_x86_MFENCE && op2.kind == Kind_zBarrier_x86_MFENCE) {
    return EQUAL(unit)(op1.zBarrier_x86_MFENCE, op2.zBarrier_x86_MFENCE);
  } else return false;
}

static void sailgen_Barrier_DMB(struct zbarrier_kind *rop, struct ztuple_z8z5enumz0zza64_barrier_domainzCz0z5enumz0zza64_barrier_typez9 op)
{
  if (rop->kind == Kind_zBarrier_DMB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_DSB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Eieio) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_ISB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Isync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_LwSync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_MIPS_SYNC) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_i) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_tso) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Sync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_x86_MFENCE){
    /* do nothing */
  }
  rop->kind = Kind_zBarrier_DMB;
  rop->zBarrier_DMB = op;
}

static void sailgen_Barrier_DSB(struct zbarrier_kind *rop, struct ztuple_z8z5enumz0zza64_barrier_domainzCz0z5enumz0zza64_barrier_typez9 op)
{
  if (rop->kind == Kind_zBarrier_DMB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_DSB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Eieio) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_ISB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Isync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_LwSync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_MIPS_SYNC) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_i) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_tso) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Sync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_x86_MFENCE){
    /* do nothing */
  }
  rop->kind = Kind_zBarrier_DSB;
  rop->zBarrier_DSB = op;
}

static void sailgen_Barrier_Eieio(struct zbarrier_kind *rop, unit op)
{
  if (rop->kind == Kind_zBarrier_DMB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_DSB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Eieio) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_ISB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Isync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_LwSync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_MIPS_SYNC) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_i) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_tso) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Sync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_x86_MFENCE){
    /* do nothing */
  }
  rop->kind = Kind_zBarrier_Eieio;
  rop->zBarrier_Eieio = op;
}

static void sailgen_Barrier_ISB(struct zbarrier_kind *rop, unit op)
{
  if (rop->kind == Kind_zBarrier_DMB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_DSB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Eieio) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_ISB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Isync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_LwSync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_MIPS_SYNC) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_i) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_tso) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Sync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_x86_MFENCE){
    /* do nothing */
  }
  rop->kind = Kind_zBarrier_ISB;
  rop->zBarrier_ISB = op;
}

static void sailgen_Barrier_Isync(struct zbarrier_kind *rop, unit op)
{
  if (rop->kind == Kind_zBarrier_DMB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_DSB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Eieio) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_ISB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Isync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_LwSync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_MIPS_SYNC) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_i) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_tso) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Sync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_x86_MFENCE){
    /* do nothing */
  }
  rop->kind = Kind_zBarrier_Isync;
  rop->zBarrier_Isync = op;
}

static void sailgen_Barrier_LwSync(struct zbarrier_kind *rop, unit op)
{
  if (rop->kind == Kind_zBarrier_DMB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_DSB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Eieio) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_ISB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Isync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_LwSync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_MIPS_SYNC) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_i) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_tso) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Sync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_x86_MFENCE){
    /* do nothing */
  }
  rop->kind = Kind_zBarrier_LwSync;
  rop->zBarrier_LwSync = op;
}

static void sailgen_Barrier_MIPS_SYNC(struct zbarrier_kind *rop, unit op)
{
  if (rop->kind == Kind_zBarrier_DMB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_DSB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Eieio) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_ISB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Isync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_LwSync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_MIPS_SYNC) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_i) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_tso) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Sync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_x86_MFENCE){
    /* do nothing */
  }
  rop->kind = Kind_zBarrier_MIPS_SYNC;
  rop->zBarrier_MIPS_SYNC = op;
}

static void sailgen_Barrier_RISCV_i(struct zbarrier_kind *rop, unit op)
{
  if (rop->kind == Kind_zBarrier_DMB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_DSB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Eieio) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_ISB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Isync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_LwSync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_MIPS_SYNC) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_i) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_tso) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Sync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_x86_MFENCE){
    /* do nothing */
  }
  rop->kind = Kind_zBarrier_RISCV_i;
  rop->zBarrier_RISCV_i = op;
}

static void sailgen_Barrier_RISCV_r_r(struct zbarrier_kind *rop, unit op)
{
  if (rop->kind == Kind_zBarrier_DMB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_DSB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Eieio) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_ISB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Isync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_LwSync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_MIPS_SYNC) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_i) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_tso) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Sync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_x86_MFENCE){
    /* do nothing */
  }
  rop->kind = Kind_zBarrier_RISCV_r_r;
  rop->zBarrier_RISCV_r_r = op;
}

static void sailgen_Barrier_RISCV_r_rw(struct zbarrier_kind *rop, unit op)
{
  if (rop->kind == Kind_zBarrier_DMB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_DSB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Eieio) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_ISB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Isync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_LwSync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_MIPS_SYNC) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_i) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_tso) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Sync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_x86_MFENCE){
    /* do nothing */
  }
  rop->kind = Kind_zBarrier_RISCV_r_rw;
  rop->zBarrier_RISCV_r_rw = op;
}

static void sailgen_Barrier_RISCV_r_w(struct zbarrier_kind *rop, unit op)
{
  if (rop->kind == Kind_zBarrier_DMB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_DSB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Eieio) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_ISB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Isync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_LwSync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_MIPS_SYNC) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_i) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_tso) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Sync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_x86_MFENCE){
    /* do nothing */
  }
  rop->kind = Kind_zBarrier_RISCV_r_w;
  rop->zBarrier_RISCV_r_w = op;
}

static void sailgen_Barrier_RISCV_rw_r(struct zbarrier_kind *rop, unit op)
{
  if (rop->kind == Kind_zBarrier_DMB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_DSB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Eieio) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_ISB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Isync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_LwSync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_MIPS_SYNC) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_i) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_tso) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Sync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_x86_MFENCE){
    /* do nothing */
  }
  rop->kind = Kind_zBarrier_RISCV_rw_r;
  rop->zBarrier_RISCV_rw_r = op;
}

static void sailgen_Barrier_RISCV_rw_rw(struct zbarrier_kind *rop, unit op)
{
  if (rop->kind == Kind_zBarrier_DMB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_DSB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Eieio) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_ISB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Isync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_LwSync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_MIPS_SYNC) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_i) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_tso) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Sync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_x86_MFENCE){
    /* do nothing */
  }
  rop->kind = Kind_zBarrier_RISCV_rw_rw;
  rop->zBarrier_RISCV_rw_rw = op;
}

static void sailgen_Barrier_RISCV_rw_w(struct zbarrier_kind *rop, unit op)
{
  if (rop->kind == Kind_zBarrier_DMB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_DSB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Eieio) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_ISB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Isync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_LwSync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_MIPS_SYNC) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_i) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_tso) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Sync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_x86_MFENCE){
    /* do nothing */
  }
  rop->kind = Kind_zBarrier_RISCV_rw_w;
  rop->zBarrier_RISCV_rw_w = op;
}

static void sailgen_Barrier_RISCV_tso(struct zbarrier_kind *rop, unit op)
{
  if (rop->kind == Kind_zBarrier_DMB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_DSB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Eieio) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_ISB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Isync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_LwSync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_MIPS_SYNC) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_i) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_tso) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Sync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_x86_MFENCE){
    /* do nothing */
  }
  rop->kind = Kind_zBarrier_RISCV_tso;
  rop->zBarrier_RISCV_tso = op;
}

static void sailgen_Barrier_RISCV_w_r(struct zbarrier_kind *rop, unit op)
{
  if (rop->kind == Kind_zBarrier_DMB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_DSB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Eieio) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_ISB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Isync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_LwSync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_MIPS_SYNC) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_i) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_tso) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Sync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_x86_MFENCE){
    /* do nothing */
  }
  rop->kind = Kind_zBarrier_RISCV_w_r;
  rop->zBarrier_RISCV_w_r = op;
}

static void sailgen_Barrier_RISCV_w_rw(struct zbarrier_kind *rop, unit op)
{
  if (rop->kind == Kind_zBarrier_DMB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_DSB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Eieio) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_ISB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Isync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_LwSync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_MIPS_SYNC) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_i) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_tso) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Sync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_x86_MFENCE){
    /* do nothing */
  }
  rop->kind = Kind_zBarrier_RISCV_w_rw;
  rop->zBarrier_RISCV_w_rw = op;
}

static void sailgen_Barrier_RISCV_w_w(struct zbarrier_kind *rop, unit op)
{
  if (rop->kind == Kind_zBarrier_DMB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_DSB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Eieio) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_ISB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Isync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_LwSync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_MIPS_SYNC) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_i) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_tso) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Sync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_x86_MFENCE){
    /* do nothing */
  }
  rop->kind = Kind_zBarrier_RISCV_w_w;
  rop->zBarrier_RISCV_w_w = op;
}

static void sailgen_Barrier_Sync(struct zbarrier_kind *rop, unit op)
{
  if (rop->kind == Kind_zBarrier_DMB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_DSB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Eieio) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_ISB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Isync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_LwSync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_MIPS_SYNC) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_i) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_tso) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Sync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_x86_MFENCE){
    /* do nothing */
  }
  rop->kind = Kind_zBarrier_Sync;
  rop->zBarrier_Sync = op;
}

static void sailgen_Barrier_x86_MFENCE(struct zbarrier_kind *rop, unit op)
{
  if (rop->kind == Kind_zBarrier_DMB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_DSB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Eieio) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_ISB) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Isync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_LwSync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_MIPS_SYNC) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_i) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_r_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_rw_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_tso) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_r) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_rw) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_RISCV_w_w) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_Sync) {
    /* do nothing */
  } else if (rop->kind == Kind_zBarrier_x86_MFENCE){
    /* do nothing */
  }
  rop->kind = Kind_zBarrier_x86_MFENCE;
  rop->zBarrier_x86_MFENCE = op;
}

// struct tuple_(%enum zread_kind, %enum zwrite_kind)
struct ztuple_z8z5enumz0zzread_kindzCz0z5enumz0zzwrite_kindz9 {
  enum zread_kind ztup0;
  enum zwrite_kind ztup1;
};

static void COPY(ztuple_z8z5enumz0zzread_kindzCz0z5enumz0zzwrite_kindz9)(struct ztuple_z8z5enumz0zzread_kindzCz0z5enumz0zzwrite_kindz9 *rop, const struct ztuple_z8z5enumz0zzread_kindzCz0z5enumz0zzwrite_kindz9 op) {
  rop->ztup0 = op.ztup0;
  rop->ztup1 = op.ztup1;
}

static bool EQUAL(ztuple_z8z5enumz0zzread_kindzCz0z5enumz0zzwrite_kindz9)(struct ztuple_z8z5enumz0zzread_kindzCz0z5enumz0zzwrite_kindz9 op1, struct ztuple_z8z5enumz0zzread_kindzCz0z5enumz0zzwrite_kindz9 op2) {
  return EQUAL(zread_kind)(op1.ztup0, op2.ztup0) && EQUAL(zwrite_kind)(op1.ztup1, op2.ztup1);
}



// union instruction_kind
enum kind_zinstruction_kind { Kind_zIK_barrier, Kind_zIK_branch, Kind_zIK_cache_op, Kind_zIK_mem_read, Kind_zIK_mem_rmw, Kind_zIK_mem_write, Kind_zIK_simple, Kind_zIK_trans };

struct zinstruction_kind {
  enum kind_zinstruction_kind kind;
  union {
    struct { struct zbarrier_kind zIK_barrier; };
    struct { unit zIK_branch; };
    struct { enum zcache_op_kind zIK_cache_op; };
    struct { enum zread_kind zIK_mem_read; };
    struct { struct ztuple_z8z5enumz0zzread_kindzCz0z5enumz0zzwrite_kindz9 zIK_mem_rmw; };
    struct { enum zwrite_kind zIK_mem_write; };
    struct { unit zIK_simple; };
    struct { enum ztrans_kind zIK_trans; };
  };
};

static void CREATE(zinstruction_kind)(struct zinstruction_kind *op)
{
  op->kind = Kind_zIK_barrier;
  CREATE(zbarrier_kind)(&op->zIK_barrier);
}

static void RECREATE(zinstruction_kind)(struct zinstruction_kind *op) {}

static void KILL(zinstruction_kind)(struct zinstruction_kind *op)
{
  if (op->kind == Kind_zIK_barrier) {
    KILL(zbarrier_kind)(&op->zIK_barrier);
  } else if (op->kind == Kind_zIK_branch) {
    /* do nothing */
  } else if (op->kind == Kind_zIK_cache_op) {
    /* do nothing */
  } else if (op->kind == Kind_zIK_mem_read) {
    /* do nothing */
  } else if (op->kind == Kind_zIK_mem_rmw) {
    /* do nothing */
  } else if (op->kind == Kind_zIK_mem_write) {
    /* do nothing */
  } else if (op->kind == Kind_zIK_simple) {
    /* do nothing */
  } else if (op->kind == Kind_zIK_trans){
    /* do nothing */
  };
}

static void COPY(zinstruction_kind)(struct zinstruction_kind *rop, struct zinstruction_kind op)
{
  if (rop->kind == Kind_zIK_barrier) {
    KILL(zbarrier_kind)(&rop->zIK_barrier);
  } else if (rop->kind == Kind_zIK_branch) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_cache_op) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_mem_read) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_mem_rmw) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_mem_write) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_simple) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_trans){
    /* do nothing */
  };
  rop->kind = op.kind;
  if (op.kind == Kind_zIK_barrier) {
    CREATE(zbarrier_kind)(&rop->zIK_barrier); COPY(zbarrier_kind)(&rop->zIK_barrier, op.zIK_barrier);
  } else if (op.kind == Kind_zIK_branch) {
    rop->zIK_branch = op.zIK_branch;
  } else if (op.kind == Kind_zIK_cache_op) {
    rop->zIK_cache_op = op.zIK_cache_op;
  } else if (op.kind == Kind_zIK_mem_read) {
    rop->zIK_mem_read = op.zIK_mem_read;
  } else if (op.kind == Kind_zIK_mem_rmw) {
    rop->zIK_mem_rmw = op.zIK_mem_rmw;
  } else if (op.kind == Kind_zIK_mem_write) {
    rop->zIK_mem_write = op.zIK_mem_write;
  } else if (op.kind == Kind_zIK_simple) {
    rop->zIK_simple = op.zIK_simple;
  } else if (op.kind == Kind_zIK_trans){
    rop->zIK_trans = op.zIK_trans;
  }
}

static bool EQUAL(zinstruction_kind)(struct zinstruction_kind op1, struct zinstruction_kind op2) {
  if (op1.kind == Kind_zIK_barrier && op2.kind == Kind_zIK_barrier) {
    return EQUAL(zbarrier_kind)(op1.zIK_barrier, op2.zIK_barrier);
  } else if (op1.kind == Kind_zIK_branch && op2.kind == Kind_zIK_branch) {
    return EQUAL(unit)(op1.zIK_branch, op2.zIK_branch);
  } else if (op1.kind == Kind_zIK_cache_op && op2.kind == Kind_zIK_cache_op) {
    return EQUAL(zcache_op_kind)(op1.zIK_cache_op, op2.zIK_cache_op);
  } else if (op1.kind == Kind_zIK_mem_read && op2.kind == Kind_zIK_mem_read) {
    return EQUAL(zread_kind)(op1.zIK_mem_read, op2.zIK_mem_read);
  } else if (op1.kind == Kind_zIK_mem_rmw && op2.kind == Kind_zIK_mem_rmw) {
    return EQUAL(ztuple_z8z5enumz0zzread_kindzCz0z5enumz0zzwrite_kindz9)(op1.zIK_mem_rmw, op2.zIK_mem_rmw);
  } else if (op1.kind == Kind_zIK_mem_write && op2.kind == Kind_zIK_mem_write) {
    return EQUAL(zwrite_kind)(op1.zIK_mem_write, op2.zIK_mem_write);
  } else if (op1.kind == Kind_zIK_simple && op2.kind == Kind_zIK_simple) {
    return EQUAL(unit)(op1.zIK_simple, op2.zIK_simple);
  } else if (op1.kind == Kind_zIK_trans && op2.kind == Kind_zIK_trans) {
    return EQUAL(ztrans_kind)(op1.zIK_trans, op2.zIK_trans);
  } else return false;
}

static void sailgen_IK_barrier(struct zinstruction_kind *rop, struct zbarrier_kind op)
{
  if (rop->kind == Kind_zIK_barrier) {
    KILL(zbarrier_kind)(&rop->zIK_barrier);
  } else if (rop->kind == Kind_zIK_branch) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_cache_op) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_mem_read) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_mem_rmw) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_mem_write) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_simple) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_trans){
    /* do nothing */
  }
  rop->kind = Kind_zIK_barrier;
  CREATE(zbarrier_kind)(&rop->zIK_barrier);
  COPY(zbarrier_kind)(&rop->zIK_barrier, op);
  
}

static void sailgen_IK_branch(struct zinstruction_kind *rop, unit op)
{
  if (rop->kind == Kind_zIK_barrier) {
    KILL(zbarrier_kind)(&rop->zIK_barrier);
  } else if (rop->kind == Kind_zIK_branch) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_cache_op) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_mem_read) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_mem_rmw) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_mem_write) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_simple) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_trans){
    /* do nothing */
  }
  rop->kind = Kind_zIK_branch;
  rop->zIK_branch = op;
}

static void sailgen_IK_cache_op(struct zinstruction_kind *rop, enum zcache_op_kind op)
{
  if (rop->kind == Kind_zIK_barrier) {
    KILL(zbarrier_kind)(&rop->zIK_barrier);
  } else if (rop->kind == Kind_zIK_branch) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_cache_op) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_mem_read) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_mem_rmw) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_mem_write) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_simple) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_trans){
    /* do nothing */
  }
  rop->kind = Kind_zIK_cache_op;
  rop->zIK_cache_op = op;
}

static void sailgen_IK_mem_read(struct zinstruction_kind *rop, enum zread_kind op)
{
  if (rop->kind == Kind_zIK_barrier) {
    KILL(zbarrier_kind)(&rop->zIK_barrier);
  } else if (rop->kind == Kind_zIK_branch) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_cache_op) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_mem_read) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_mem_rmw) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_mem_write) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_simple) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_trans){
    /* do nothing */
  }
  rop->kind = Kind_zIK_mem_read;
  rop->zIK_mem_read = op;
}

static void sailgen_IK_mem_rmw(struct zinstruction_kind *rop, struct ztuple_z8z5enumz0zzread_kindzCz0z5enumz0zzwrite_kindz9 op)
{
  if (rop->kind == Kind_zIK_barrier) {
    KILL(zbarrier_kind)(&rop->zIK_barrier);
  } else if (rop->kind == Kind_zIK_branch) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_cache_op) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_mem_read) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_mem_rmw) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_mem_write) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_simple) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_trans){
    /* do nothing */
  }
  rop->kind = Kind_zIK_mem_rmw;
  rop->zIK_mem_rmw = op;
}

static void sailgen_IK_mem_write(struct zinstruction_kind *rop, enum zwrite_kind op)
{
  if (rop->kind == Kind_zIK_barrier) {
    KILL(zbarrier_kind)(&rop->zIK_barrier);
  } else if (rop->kind == Kind_zIK_branch) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_cache_op) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_mem_read) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_mem_rmw) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_mem_write) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_simple) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_trans){
    /* do nothing */
  }
  rop->kind = Kind_zIK_mem_write;
  rop->zIK_mem_write = op;
}

static void sailgen_IK_simple(struct zinstruction_kind *rop, unit op)
{
  if (rop->kind == Kind_zIK_barrier) {
    KILL(zbarrier_kind)(&rop->zIK_barrier);
  } else if (rop->kind == Kind_zIK_branch) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_cache_op) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_mem_read) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_mem_rmw) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_mem_write) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_simple) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_trans){
    /* do nothing */
  }
  rop->kind = Kind_zIK_simple;
  rop->zIK_simple = op;
}

static void sailgen_IK_trans(struct zinstruction_kind *rop, enum ztrans_kind op)
{
  if (rop->kind == Kind_zIK_barrier) {
    KILL(zbarrier_kind)(&rop->zIK_barrier);
  } else if (rop->kind == Kind_zIK_branch) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_cache_op) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_mem_read) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_mem_rmw) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_mem_write) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_simple) {
    /* do nothing */
  } else if (rop->kind == Kind_zIK_trans){
    /* do nothing */
  }
  rop->kind = Kind_zIK_trans;
  rop->zIK_trans = op;
}

// enum ClearRegSet
enum zClearRegSet { zGPRegs, zFPRegs };

static bool eq_zClearRegSet(enum zClearRegSet op1, enum zClearRegSet op2) { return op1 == op2; }

static enum zClearRegSet UNDEFINED(zClearRegSet)(unit u) { return zGPRegs; }

// struct Capability
struct zCapability {
  uint64_t zB;
  uint64_t zE;
  uint64_t zT;
  bool zaccess_system_regs;
  uint64_t zaddress;
  bool zflag_cap_mode;
  bool zglobal;
  bool zinternal_e;
  uint64_t zotype;
  bool zpermit_ccall;
  bool zpermit_execute;
  bool zpermit_load;
  bool zpermit_load_cap;
  bool zpermit_seal;
  bool zpermit_set_CID;
  bool zpermit_store;
  bool zpermit_store_cap;
  bool zpermit_store_local_cap;
  bool zpermit_unseal;
  uint64_t zreserved;
  bool zsealed;
  bool ztag;
  uint64_t zuperms;
};

static void COPY(zCapability)(struct zCapability *rop, const struct zCapability op) {
  rop->zB = op.zB;
  rop->zE = op.zE;
  rop->zT = op.zT;
  rop->zaccess_system_regs = op.zaccess_system_regs;
  rop->zaddress = op.zaddress;
  rop->zflag_cap_mode = op.zflag_cap_mode;
  rop->zglobal = op.zglobal;
  rop->zinternal_e = op.zinternal_e;
  rop->zotype = op.zotype;
  rop->zpermit_ccall = op.zpermit_ccall;
  rop->zpermit_execute = op.zpermit_execute;
  rop->zpermit_load = op.zpermit_load;
  rop->zpermit_load_cap = op.zpermit_load_cap;
  rop->zpermit_seal = op.zpermit_seal;
  rop->zpermit_set_CID = op.zpermit_set_CID;
  rop->zpermit_store = op.zpermit_store;
  rop->zpermit_store_cap = op.zpermit_store_cap;
  rop->zpermit_store_local_cap = op.zpermit_store_local_cap;
  rop->zpermit_unseal = op.zpermit_unseal;
  rop->zreserved = op.zreserved;
  rop->zsealed = op.zsealed;
  rop->ztag = op.ztag;
  rop->zuperms = op.zuperms;
}

static bool EQUAL(zCapability)(struct zCapability op1, struct zCapability op2) {
  return EQUAL(fbits)(op1.zB, op2.zB) && EQUAL(fbits)(op1.zE, op2.zE) && EQUAL(fbits)(op1.zT, op2.zT) && EQUAL(bool)(op1.zaccess_system_regs, op2.zaccess_system_regs) && EQUAL(fbits)(op1.zaddress, op2.zaddress) && EQUAL(bool)(op1.zflag_cap_mode, op2.zflag_cap_mode) && EQUAL(bool)(op1.zglobal, op2.zglobal) && EQUAL(bool)(op1.zinternal_e, op2.zinternal_e) && EQUAL(fbits)(op1.zotype, op2.zotype) && EQUAL(bool)(op1.zpermit_ccall, op2.zpermit_ccall) && EQUAL(bool)(op1.zpermit_execute, op2.zpermit_execute) && EQUAL(bool)(op1.zpermit_load, op2.zpermit_load) && EQUAL(bool)(op1.zpermit_load_cap, op2.zpermit_load_cap) && EQUAL(bool)(op1.zpermit_seal, op2.zpermit_seal) && EQUAL(bool)(op1.zpermit_set_CID, op2.zpermit_set_CID) && EQUAL(bool)(op1.zpermit_store, op2.zpermit_store) && EQUAL(bool)(op1.zpermit_store_cap, op2.zpermit_store_cap) && EQUAL(bool)(op1.zpermit_store_local_cap, op2.zpermit_store_local_cap) && EQUAL(bool)(op1.zpermit_unseal, op2.zpermit_unseal) && EQUAL(fbits)(op1.zreserved, op2.zreserved) && EQUAL(bool)(op1.zsealed, op2.zsealed) && EQUAL(bool)(op1.ztag, op2.ztag) && EQUAL(fbits)(op1.zuperms, op2.zuperms);
}

// enum CapEx
enum zCapEx { zCapEx_None, zCapEx_LengthViolation, zCapEx_TagViolation, zCapEx_SealViolation, zCapEx_TypeViolation, zCapEx_CallTrap, zCapEx_ReturnTrap, zCapEx_TSSUnderFlow, zCapEx_UserDefViolation, zCapEx_TLBNoStoreCap, zCapEx_InexactBounds, zCapEx_UnalignedBase, zCapEx_GlobalViolation, zCapEx_PermitExecuteViolation, zCapEx_PermitLoadViolation, zCapEx_PermitStoreViolation, zCapEx_PermitLoadCapViolation, zCapEx_PermitStoreCapViolation, zCapEx_PermitStoreLocalCapViolation, zCapEx_PermitSealViolation, zCapEx_AccessSystemRegsViolation, zCapEx_PermitCCallViolation, zCapEx_AccessCCallIDCViolation, zCapEx_PermitUnsealViolation, zCapEx_PermitSetCIDViolation };

static bool eq_zCapEx(enum zCapEx op1, enum zCapEx op2) { return op1 == op2; }

static enum zCapEx UNDEFINED(zCapEx)(unit u) { return zCapEx_None; }

// struct cheri_cause
struct zcheri_cause {
  enum zCapEx zcapEx;
  uint64_t zcap_idx;
};

static void COPY(zcheri_cause)(struct zcheri_cause *rop, const struct zcheri_cause op) {
  rop->zcapEx = op.zcapEx;
  rop->zcap_idx = op.zcap_idx;
}

static bool EQUAL(zcheri_cause)(struct zcheri_cause op1, struct zcheri_cause op2) {
  return EQUAL(zCapEx)(op1.zcapEx, op2.zcapEx) && EQUAL(fbits)(op1.zcap_idx, op2.zcap_idx);
}

// enum CPtrCmpOp
enum zCPtrCmpOp { zCEQ, zCNE, zCLT, zCLE, zCLTU, zCLEU, zCEXEQ, zCNEXEQ };

static bool eq_zCPtrCmpOp(enum zCPtrCmpOp op1, enum zCPtrCmpOp op2) { return op1 == op2; }

static enum zCPtrCmpOp UNDEFINED(zCPtrCmpOp)(unit u) { return zCEQ; }

















static bool sailgen_zzz3zzzz7mzzJzzK0z3neq_int(sail_int, int64_t);

static bool sailgen_zzz3zzzz7mzzJzzK0z3neq_int(sail_int zx, int64_t zy)
{
  __label__ end_function_1, end_block_exception_2;

  bool zcbz30;
  bool zgaz30;
  {
    sail_int zgsz30;
    CREATE(sail_int)(&zgsz30);
    CONVERT_OF(sail_int, mach_int)(&zgsz30, zy);
    zgaz30 = eq_int(zx, zgsz30);
    KILL(sail_int)(&zgsz30);
  }
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

































static uint64_t sailgen_zzzz7nzzJzzK4z3sail_ones(int64_t);

static uint64_t sailgen_zzzz7nzzJzzK31z3sail_ones(int64_t);

static void sailgen_zzz3zzz3sail_ones(lbits *rop, sail_int);

static uint64_t sailgen_zzzz7nzzJzzK4z3sail_ones(int64_t zn)
{
  __label__ end_function_7, end_block_exception_8;

  uint64_t zcbz32;
  uint64_t zgaz32;
  {
    sail_int zgsz31;
    CREATE(sail_int)(&zgsz31);
    CONVERT_OF(sail_int, mach_int)(&zgsz31, zn);
    lbits zgsz32;
    CREATE(lbits)(&zgsz32);
    zeros(&zgsz32, zgsz31);
    zgaz32 = CONVERT_OF(fbits, lbits)(zgsz32, true);
    KILL(lbits)(&zgsz32);
    KILL(sail_int)(&zgsz31);
  }
  {
    lbits zgsz33;
    CREATE(lbits)(&zgsz33);
    CONVERT_OF(lbits, fbits)(&zgsz33, zgaz32, UINT64_C(4) , true);
    lbits zgsz34;
    CREATE(lbits)(&zgsz34);
    not_bits(&zgsz34, zgsz33);
    zcbz32 = CONVERT_OF(fbits, lbits)(zgsz34, true);
    KILL(lbits)(&zgsz34);
    KILL(lbits)(&zgsz33);
  }

end_function_7: ;
  return zcbz32;
end_block_exception_8: ;

  return UINT64_C(0xdeadc0de);
}

static uint64_t sailgen_zzzz7nzzJzzK31z3sail_ones(int64_t zn)
{
  __label__ end_function_10, end_block_exception_11;

  uint64_t zcbz33;
  uint64_t zgaz33;
  {
    sail_int zgsz35;
    CREATE(sail_int)(&zgsz35);
    CONVERT_OF(sail_int, mach_int)(&zgsz35, zn);
    lbits zgsz36;
    CREATE(lbits)(&zgsz36);
    zeros(&zgsz36, zgsz35);
    zgaz33 = CONVERT_OF(fbits, lbits)(zgsz36, true);
    KILL(lbits)(&zgsz36);
    KILL(sail_int)(&zgsz35);
  }
  {
    lbits zgsz37;
    CREATE(lbits)(&zgsz37);
    CONVERT_OF(lbits, fbits)(&zgsz37, zgaz33, UINT64_C(31) , true);
    lbits zgsz38;
    CREATE(lbits)(&zgsz38);
    not_bits(&zgsz38, zgsz37);
    zcbz33 = CONVERT_OF(fbits, lbits)(zgsz38, true);
    KILL(lbits)(&zgsz38);
    KILL(lbits)(&zgsz37);
  }

end_function_10: ;
  return zcbz33;
end_block_exception_11: ;

  return UINT64_C(0xdeadc0de);
}

static void sailgen_zzz3zzz3sail_ones(lbits *zcbz34, sail_int zn)
{
  __label__ end_function_13, end_block_exception_14, end_function_219;

  lbits zgaz34;
  CREATE(lbits)(&zgaz34);
  zeros(&zgaz34, zn);
  not_bits((*(&zcbz34)), zgaz34);
  KILL(lbits)(&zgaz34);
end_function_13: ;
  goto end_function_219;
end_block_exception_14: ;
  goto end_function_219;
end_function_219: ;
}











static fbits sailgen_not_bit(fbits);

static fbits sailgen_not_bit(fbits zb)
{
  __label__ end_function_16, end_block_exception_17;

  fbits zcbz35;
  bool zgaz35;
  zgaz35 = eq_bit(zb, UINT64_C(1));
  if (zgaz35) {  zcbz35 = UINT64_C(0);  } else {  zcbz35 = UINT64_C(1);  }

end_function_16: ;
  return zcbz35;
end_block_exception_17: ;

  return UINT64_C(0);
}













static uint64_t sailgen_zzzz7mzzJzzK64zzCzz0zz7nzzJzzK18z3EXTS(int64_t, uint64_t);

static void sailgen_zzz3zzz3EXTZ(lbits *rop, sail_int, lbits);

static uint64_t sailgen_zzzz7mzzJzzK15zzCzz0zz7nzzJzzK12z3EXTZ(int64_t, uint64_t);

static uint64_t sailgen_zzz3zzzz7mzzJzzK31z3EXTZ(int64_t, sbits);

static uint64_t sailgen_zzzz7mzzJzzK64zzCzz0zz7nzzJzzK18z3EXTZ(int64_t, uint64_t);

static uint64_t sailgen_zzzz7mzzJzzK64zzCzz0zz7nzzJzzK18z3EXTS(int64_t zm, uint64_t zv)
{
  __label__ end_function_19, end_block_exception_20;

  uint64_t zcbz36;
  {
    lbits zgsz39;
    CREATE(lbits)(&zgsz39);
    CONVERT_OF(lbits, fbits)(&zgsz39, zv, UINT64_C(18) , true);
    sail_int zgsz310;
    CREATE(sail_int)(&zgsz310);
    CONVERT_OF(sail_int, mach_int)(&zgsz310, zm);
    lbits zgsz311;
    CREATE(lbits)(&zgsz311);
    sign_extend(&zgsz311, zgsz39, zgsz310);
    zcbz36 = CONVERT_OF(fbits, lbits)(zgsz311, true);
    KILL(lbits)(&zgsz311);
    KILL(sail_int)(&zgsz310);
    KILL(lbits)(&zgsz39);
  }
end_function_19: ;
  return zcbz36;
end_block_exception_20: ;

  return UINT64_C(0xdeadc0de);
}

static void sailgen_zzz3zzz3EXTZ(lbits *zcbz37, sail_int zm, lbits zv)
{
  __label__ end_function_22, end_block_exception_23, end_function_218;

  zero_extend((*(&zcbz37)), zv, zm);
end_function_22: ;
  goto end_function_218;
end_block_exception_23: ;
  goto end_function_218;
end_function_218: ;
}

static uint64_t sailgen_zzzz7mzzJzzK15zzCzz0zz7nzzJzzK12z3EXTZ(int64_t zm, uint64_t zv)
{
  __label__ end_function_25, end_block_exception_26;

  uint64_t zcbz38;
  {
    lbits zgsz312;
    CREATE(lbits)(&zgsz312);
    CONVERT_OF(lbits, fbits)(&zgsz312, zv, UINT64_C(12) , true);
    sail_int zgsz313;
    CREATE(sail_int)(&zgsz313);
    CONVERT_OF(sail_int, mach_int)(&zgsz313, zm);
    lbits zgsz314;
    CREATE(lbits)(&zgsz314);
    zero_extend(&zgsz314, zgsz312, zgsz313);
    zcbz38 = CONVERT_OF(fbits, lbits)(zgsz314, true);
    KILL(lbits)(&zgsz314);
    KILL(sail_int)(&zgsz313);
    KILL(lbits)(&zgsz312);
  }
end_function_25: ;
  return zcbz38;
end_block_exception_26: ;

  return UINT64_C(0xdeadc0de);
}

static uint64_t sailgen_zzz3zzzz7mzzJzzK31z3EXTZ(int64_t zm, sbits zv)
{
  __label__ end_function_28, end_block_exception_29;

  uint64_t zcbz39;
  {
    lbits zgsz315;
    CREATE(lbits)(&zgsz315);
    CONVERT_OF(lbits, sbits)(&zgsz315, zv, true);
    sail_int zgsz316;
    CREATE(sail_int)(&zgsz316);
    CONVERT_OF(sail_int, mach_int)(&zgsz316, zm);
    lbits zgsz317;
    CREATE(lbits)(&zgsz317);
    zero_extend(&zgsz317, zgsz315, zgsz316);
    zcbz39 = CONVERT_OF(fbits, lbits)(zgsz317, true);
    KILL(lbits)(&zgsz317);
    KILL(sail_int)(&zgsz316);
    KILL(lbits)(&zgsz315);
  }
end_function_28: ;
  return zcbz39;
end_block_exception_29: ;

  return UINT64_C(0xdeadc0de);
}

static uint64_t sailgen_zzzz7mzzJzzK64zzCzz0zz7nzzJzzK18z3EXTZ(int64_t zm, uint64_t zv)
{
  __label__ end_function_31, end_block_exception_32;

  uint64_t zcbz310;
  {
    lbits zgsz318;
    CREATE(lbits)(&zgsz318);
    CONVERT_OF(lbits, fbits)(&zgsz318, zv, UINT64_C(18) , true);
    sail_int zgsz319;
    CREATE(sail_int)(&zgsz319);
    CONVERT_OF(sail_int, mach_int)(&zgsz319, zm);
    lbits zgsz320;
    CREATE(lbits)(&zgsz320);
    zero_extend(&zgsz320, zgsz318, zgsz319);
    zcbz310 = CONVERT_OF(fbits, lbits)(zgsz320, true);
    KILL(lbits)(&zgsz320);
    KILL(sail_int)(&zgsz319);
    KILL(lbits)(&zgsz318);
  }
end_function_31: ;
  return zcbz310;
end_block_exception_32: ;

  return UINT64_C(0xdeadc0de);
}

static uint64_t sailgen_zzzz7nzzJzzK64z3zzeros_implicit(int64_t);

static uint64_t sailgen_zzzz7nzzJzzK14z3zzeros_implicit(int64_t);

static uint64_t sailgen_zzzz7nzzJzzK2z3zzeros_implicit(int64_t);

static uint64_t sailgen_zzzz7nzzJzzK4z3zzeros_implicit(int64_t);

static uint64_t sailgen_zzzz7nzzJzzK6z3zzeros_implicit(int64_t);

static uint64_t sailgen_zzzz7nzzJzzK12z3zzeros_implicit(int64_t);

static void sailgen_zzz3zzz3zzeros_implicit(lbits *rop, sail_int);

static uint64_t sailgen_zzzz7nzzJzzK11z3zzeros_implicit(int64_t);

static uint64_t sailgen_zzzz7nzzJzzK64z3zzeros_implicit(int64_t zn)
{
  __label__ end_function_34, end_block_exception_35;

  uint64_t zcbz311;
  {
    sail_int zgsz321;
    CREATE(sail_int)(&zgsz321);
    CONVERT_OF(sail_int, mach_int)(&zgsz321, zn);
    lbits zgsz322;
    CREATE(lbits)(&zgsz322);
    zeros(&zgsz322, zgsz321);
    zcbz311 = CONVERT_OF(fbits, lbits)(zgsz322, true);
    KILL(lbits)(&zgsz322);
    KILL(sail_int)(&zgsz321);
  }
end_function_34: ;
  return zcbz311;
end_block_exception_35: ;

  return UINT64_C(0xdeadc0de);
}

static uint64_t sailgen_zzzz7nzzJzzK14z3zzeros_implicit(int64_t zn)
{
  __label__ end_function_37, end_block_exception_38;

  uint64_t zcbz312;
  {
    sail_int zgsz323;
    CREATE(sail_int)(&zgsz323);
    CONVERT_OF(sail_int, mach_int)(&zgsz323, zn);
    lbits zgsz324;
    CREATE(lbits)(&zgsz324);
    zeros(&zgsz324, zgsz323);
    zcbz312 = CONVERT_OF(fbits, lbits)(zgsz324, true);
    KILL(lbits)(&zgsz324);
    KILL(sail_int)(&zgsz323);
  }
end_function_37: ;
  return zcbz312;
end_block_exception_38: ;

  return UINT64_C(0xdeadc0de);
}

static uint64_t sailgen_zzzz7nzzJzzK2z3zzeros_implicit(int64_t zn)
{
  __label__ end_function_40, end_block_exception_41;

  uint64_t zcbz313;
  {
    sail_int zgsz325;
    CREATE(sail_int)(&zgsz325);
    CONVERT_OF(sail_int, mach_int)(&zgsz325, zn);
    lbits zgsz326;
    CREATE(lbits)(&zgsz326);
    zeros(&zgsz326, zgsz325);
    zcbz313 = CONVERT_OF(fbits, lbits)(zgsz326, true);
    KILL(lbits)(&zgsz326);
    KILL(sail_int)(&zgsz325);
  }
end_function_40: ;
  return zcbz313;
end_block_exception_41: ;

  return UINT64_C(0xdeadc0de);
}

static uint64_t sailgen_zzzz7nzzJzzK4z3zzeros_implicit(int64_t zn)
{
  __label__ end_function_43, end_block_exception_44;

  uint64_t zcbz314;
  {
    sail_int zgsz327;
    CREATE(sail_int)(&zgsz327);
    CONVERT_OF(sail_int, mach_int)(&zgsz327, zn);
    lbits zgsz328;
    CREATE(lbits)(&zgsz328);
    zeros(&zgsz328, zgsz327);
    zcbz314 = CONVERT_OF(fbits, lbits)(zgsz328, true);
    KILL(lbits)(&zgsz328);
    KILL(sail_int)(&zgsz327);
  }
end_function_43: ;
  return zcbz314;
end_block_exception_44: ;

  return UINT64_C(0xdeadc0de);
}

static uint64_t sailgen_zzzz7nzzJzzK6z3zzeros_implicit(int64_t zn)
{
  __label__ end_function_46, end_block_exception_47;

  uint64_t zcbz315;
  {
    sail_int zgsz329;
    CREATE(sail_int)(&zgsz329);
    CONVERT_OF(sail_int, mach_int)(&zgsz329, zn);
    lbits zgsz330;
    CREATE(lbits)(&zgsz330);
    zeros(&zgsz330, zgsz329);
    zcbz315 = CONVERT_OF(fbits, lbits)(zgsz330, true);
    KILL(lbits)(&zgsz330);
    KILL(sail_int)(&zgsz329);
  }
end_function_46: ;
  return zcbz315;
end_block_exception_47: ;

  return UINT64_C(0xdeadc0de);
}

static uint64_t sailgen_zzzz7nzzJzzK12z3zzeros_implicit(int64_t zn)
{
  __label__ end_function_49, end_block_exception_50;

  uint64_t zcbz316;
  {
    sail_int zgsz331;
    CREATE(sail_int)(&zgsz331);
    CONVERT_OF(sail_int, mach_int)(&zgsz331, zn);
    lbits zgsz332;
    CREATE(lbits)(&zgsz332);
    zeros(&zgsz332, zgsz331);
    zcbz316 = CONVERT_OF(fbits, lbits)(zgsz332, true);
    KILL(lbits)(&zgsz332);
    KILL(sail_int)(&zgsz331);
  }
end_function_49: ;
  return zcbz316;
end_block_exception_50: ;

  return UINT64_C(0xdeadc0de);
}

static void sailgen_zzz3zzz3zzeros_implicit(lbits *zcbz317, sail_int zn)
{
  __label__ end_function_52, end_block_exception_53, end_function_217;

  zeros((*(&zcbz317)), zn);
end_function_52: ;
  goto end_function_217;
end_block_exception_53: ;
  goto end_function_217;
end_function_217: ;
}

static uint64_t sailgen_zzzz7nzzJzzK11z3zzeros_implicit(int64_t zn)
{
  __label__ end_function_55, end_block_exception_56;

  uint64_t zcbz318;
  {
    sail_int zgsz333;
    CREATE(sail_int)(&zgsz333);
    CONVERT_OF(sail_int, mach_int)(&zgsz333, zn);
    lbits zgsz334;
    CREATE(lbits)(&zgsz334);
    zeros(&zgsz334, zgsz333);
    zcbz318 = CONVERT_OF(fbits, lbits)(zgsz334, true);
    KILL(lbits)(&zgsz334);
    KILL(sail_int)(&zgsz333);
  }
end_function_55: ;
  return zcbz318;
end_block_exception_56: ;

  return UINT64_C(0xdeadc0de);
}

static uint64_t sailgen_zzzz7nzzJzzK4z3ones(int64_t);

static void sailgen_zzz3zzz3ones(lbits *rop, sail_int);

static uint64_t sailgen_zzzz7nzzJzzK31z3ones(int64_t);

static uint64_t sailgen_zzzz7nzzJzzK4z3ones(int64_t zn)
{
  __label__ end_function_58, end_block_exception_59;

  uint64_t zcbz319;
  zcbz319 = sailgen_zzzz7nzzJzzK4z3sail_ones(zn);
end_function_58: ;
  return zcbz319;
end_block_exception_59: ;

  return UINT64_C(0xdeadc0de);
}

static void sailgen_zzz3zzz3ones(lbits *zcbz320, sail_int zn)
{
  __label__ end_function_61, end_block_exception_62, end_function_216;

  sailgen_zzz3zzz3sail_ones((*(&zcbz320)), zn);
end_function_61: ;
  goto end_function_216;
end_block_exception_62: ;
  goto end_function_216;
end_function_216: ;
}

static uint64_t sailgen_zzzz7nzzJzzK31z3ones(int64_t zn)
{
  __label__ end_function_64, end_block_exception_65;

  uint64_t zcbz321;
  zcbz321 = sailgen_zzzz7nzzJzzK31z3sail_ones(zn);
end_function_64: ;
  return zcbz321;
end_block_exception_65: ;

  return UINT64_C(0xdeadc0de);
}

static uint64_t sailgen_bool_to_bits(bool);

static uint64_t sailgen_bool_to_bits(bool zx)
{
  __label__ end_function_67, end_block_exception_68;

  uint64_t zcbz322;
  if (zx) {  zcbz322 = UINT64_C(0b1);  } else {  zcbz322 = UINT64_C(0b0);  }
end_function_67: ;
  return zcbz322;
end_block_exception_68: ;

  return UINT64_C(0xdeadc0de);
}

static bool sailgen_bit_to_bool(fbits);

static bool sailgen_bit_to_bool(fbits zb)
{
  __label__ case_71, case_72, finish_match_70, end_function_73, end_block_exception_74;

  bool zcbz323;
  bool zgsz335;
  {
    fbits zp0z3;
    zp0z3 = zb;
    bool zgsz336;
    zgsz336 = eq_bit(zp0z3, UINT64_C(1));
    if (!(zgsz336)) {
  
    goto case_71;
    }
    zgsz335 = true;
  
    goto finish_match_70;
  }
case_71: ;
  {
    fbits zp0z3;
    zp0z3 = zb;
    bool zgsz337;
    zgsz337 = eq_bit(zp0z3, UINT64_C(0));
    if (!(zgsz337)) {
  
    goto case_72;
    }
    zgsz335 = false;
  
    goto finish_match_70;
  }
case_72: ;
  sail_match_failure("bit_to_bool");
finish_match_70: ;
  zcbz323 = zgsz335;

end_function_73: ;
  return zcbz323;
end_block_exception_74: ;

  return false;
}

static void sailgen_to_bits(lbits *rop, sail_int, sail_int);

static uint64_t sailgen_zzzz7lzzJzzK6z3to_bits(int64_t, sail_int);

static void sailgen_zzzz7lzzJzzK68z3to_bits(lbits *rop, int64_t, sail_int);

static void sailgen_to_bits(lbits *zcbz324, sail_int zl, sail_int zn)
{
  __label__ end_function_76, end_block_exception_77, end_function_215;

  {
    sail_int zgsz338;
    CREATE(sail_int)(&zgsz338);
    CONVERT_OF(sail_int, mach_int)(&zgsz338, INT64_C(0));
    get_slice_int((*(&zcbz324)), zl, zn, zgsz338);
    KILL(sail_int)(&zgsz338);
  }
end_function_76: ;
  goto end_function_215;
end_block_exception_77: ;
  goto end_function_215;
end_function_215: ;
}

static uint64_t sailgen_zzzz7lzzJzzK6z3to_bits(int64_t zl, sail_int zn)
{
  __label__ end_function_79, end_block_exception_80;

  uint64_t zcbz325;
  {
    sail_int zgsz339;
    CREATE(sail_int)(&zgsz339);
    CONVERT_OF(sail_int, mach_int)(&zgsz339, zl);
    sail_int zgsz340;
    CREATE(sail_int)(&zgsz340);
    CONVERT_OF(sail_int, mach_int)(&zgsz340, INT64_C(0));
    lbits zgsz341;
    CREATE(lbits)(&zgsz341);
    get_slice_int(&zgsz341, zgsz339, zn, zgsz340);
    zcbz325 = CONVERT_OF(fbits, lbits)(zgsz341, true);
    KILL(lbits)(&zgsz341);
    KILL(sail_int)(&zgsz340);
    KILL(sail_int)(&zgsz339);
  }
end_function_79: ;
  return zcbz325;
end_block_exception_80: ;

  return UINT64_C(0xdeadc0de);
}

static void sailgen_zzzz7lzzJzzK68z3to_bits(lbits *zcbz326, int64_t zl, sail_int zn)
{
  __label__ end_function_82, end_block_exception_83, end_function_214;

  {
    sail_int zgsz343;
    CREATE(sail_int)(&zgsz343);
    CONVERT_OF(sail_int, mach_int)(&zgsz343, INT64_C(0));
    sail_int zgsz342;
    CREATE(sail_int)(&zgsz342);
    CONVERT_OF(sail_int, mach_int)(&zgsz342, zl);
    get_slice_int((*(&zcbz326)), zgsz342, zn, zgsz343);
    KILL(sail_int)(&zgsz343);
    KILL(sail_int)(&zgsz342);
  }
end_function_82: ;
  goto end_function_214;
end_block_exception_83: ;
  goto end_function_214;
end_function_214: ;
}

static bool sailgen_z8operatorz0zzzz7nzzJzzK12z3zI_uz9(uint64_t, uint64_t);

static bool sailgen_z8operatorz0zzzz7nzzJzzK3z3zI_uz9(uint64_t, uint64_t);

static bool sailgen_z8operatorz0zzzz7nzzJzzK14z3zI_uz9(uint64_t, uint64_t);

static bool sailgen_z8operatorz0zzzz7nzzJzzK14z3zKzJ_uz9(uint64_t, uint64_t);

static bool sailgen_z8operatorz0zzzz7nzzJzzK12z3zI_uz9(uint64_t zx, uint64_t zy)
{
  __label__ end_function_85, end_block_exception_86;

  bool zcbz327;
  int64_t zgaz36;
  zgaz36 = ((mach_int) zx);
  int64_t zgaz37;
  zgaz37 = ((mach_int) zy);
  {
    sail_int zgsz345;
    CREATE(sail_int)(&zgsz345);
    CONVERT_OF(sail_int, mach_int)(&zgsz345, zgaz37);
    sail_int zgsz344;
    CREATE(sail_int)(&zgsz344);
    CONVERT_OF(sail_int, mach_int)(&zgsz344, zgaz36);
    zcbz327 = lt(zgsz344, zgsz345);
    KILL(sail_int)(&zgsz345);
    KILL(sail_int)(&zgsz344);
  }


end_function_85: ;
  return zcbz327;
end_block_exception_86: ;

  return false;
}

static bool sailgen_z8operatorz0zzzz7nzzJzzK3z3zI_uz9(uint64_t zx, uint64_t zy)
{
  __label__ end_function_88, end_block_exception_89;

  bool zcbz328;
  int64_t zgaz38;
  zgaz38 = ((mach_int) zx);
  int64_t zgaz39;
  zgaz39 = ((mach_int) zy);
  {
    sail_int zgsz347;
    CREATE(sail_int)(&zgsz347);
    CONVERT_OF(sail_int, mach_int)(&zgsz347, zgaz39);
    sail_int zgsz346;
    CREATE(sail_int)(&zgsz346);
    CONVERT_OF(sail_int, mach_int)(&zgsz346, zgaz38);
    zcbz328 = lt(zgsz346, zgsz347);
    KILL(sail_int)(&zgsz347);
    KILL(sail_int)(&zgsz346);
  }


end_function_88: ;
  return zcbz328;
end_block_exception_89: ;

  return false;
}

static bool sailgen_z8operatorz0zzzz7nzzJzzK14z3zI_uz9(uint64_t zx, uint64_t zy)
{
  __label__ end_function_91, end_block_exception_92;

  bool zcbz329;
  int64_t zgaz310;
  zgaz310 = ((mach_int) zx);
  int64_t zgaz311;
  zgaz311 = ((mach_int) zy);
  {
    sail_int zgsz349;
    CREATE(sail_int)(&zgsz349);
    CONVERT_OF(sail_int, mach_int)(&zgsz349, zgaz311);
    sail_int zgsz348;
    CREATE(sail_int)(&zgsz348);
    CONVERT_OF(sail_int, mach_int)(&zgsz348, zgaz310);
    zcbz329 = lt(zgsz348, zgsz349);
    KILL(sail_int)(&zgsz349);
    KILL(sail_int)(&zgsz348);
  }


end_function_91: ;
  return zcbz329;
end_block_exception_92: ;

  return false;
}

static bool sailgen_z8operatorz0zzzz7nzzJzzK14z3zKzJ_uz9(uint64_t zx, uint64_t zy)
{
  __label__ end_function_94, end_block_exception_95;

  bool zcbz330;
  int64_t zgaz312;
  zgaz312 = ((mach_int) zx);
  int64_t zgaz313;
  zgaz313 = ((mach_int) zy);
  {
    sail_int zgsz351;
    CREATE(sail_int)(&zgsz351);
    CONVERT_OF(sail_int, mach_int)(&zgsz351, zgaz313);
    sail_int zgsz350;
    CREATE(sail_int)(&zgsz350);
    CONVERT_OF(sail_int, mach_int)(&zgsz350, zgaz312);
    zcbz330 = gteq(zgsz350, zgsz351);
    KILL(sail_int)(&zgsz351);
    KILL(sail_int)(&zgsz350);
  }


end_function_94: ;
  return zcbz330;
end_block_exception_95: ;

  return false;
}



static void sailgen_n_leading_spaces(sail_int *rop, sail_string);

static void sailgen_n_leading_spaces(sail_int *zcbz331, sail_string zs)
{
  __label__ case_98, case_99, finish_match_97, end_function_103, end_block_exception_104, end_function_213;

  sail_int zgsz352;
  CREATE(sail_int)(&zgsz352);
  {
    sail_string zp0z3;
    CREATE(sail_string)(&zp0z3);
    COPY(sail_string)(&zp0z3, zs);
    bool zgsz353;
    zgsz353 = eq_string(zp0z3, "");
    if (!(zgsz353)) {
    KILL(sail_string)(&zp0z3);
    goto case_98;
    }
    CONVERT_OF(sail_int, mach_int)(&zgsz352, INT64_C(0));
    KILL(sail_string)(&zp0z3);
    goto finish_match_97;
  }
case_98: ;
  {
    __label__ case_101, case_102, finish_match_100;

    sail_string zgaz314;
    CREATE(sail_string)(&zgaz314);
    {
      sail_int zgsz354;
      CREATE(sail_int)(&zgsz354);
      CONVERT_OF(sail_int, mach_int)(&zgsz354, INT64_C(1));
      string_take(&zgaz314, zs, zgsz354);
      KILL(sail_int)(&zgsz354);
    }
    sail_int zgsz355;
    CREATE(sail_int)(&zgsz355);
    {
      sail_string zp0z3;
      CREATE(sail_string)(&zp0z3);
      COPY(sail_string)(&zp0z3, zgaz314);
      bool zgsz357;
      zgsz357 = eq_string(zp0z3, " ");
      if (!(zgsz357)) {
      KILL(sail_string)(&zp0z3);
      goto case_101;
      }
      sail_int zgaz316;
      CREATE(sail_int)(&zgaz316);
      {
        sail_string zgaz315;
        CREATE(sail_string)(&zgaz315);
        {
          sail_int zgsz356;
          CREATE(sail_int)(&zgsz356);
          CONVERT_OF(sail_int, mach_int)(&zgsz356, INT64_C(1));
          string_drop(&zgaz315, zs, zgsz356);
          KILL(sail_int)(&zgsz356);
        }
        sailgen_n_leading_spaces(&zgaz316, zgaz315);
        KILL(sail_string)(&zgaz315);
      }
      {
        sail_int zgsz358;
        CREATE(sail_int)(&zgsz358);
        CONVERT_OF(sail_int, mach_int)(&zgsz358, INT64_C(1));
        add_int(&zgsz355, zgsz358, zgaz316);
        KILL(sail_int)(&zgsz358);
      }
      KILL(sail_int)(&zgaz316);
      KILL(sail_string)(&zp0z3);
      goto finish_match_100;
    }
  case_101: ;
    {
      CONVERT_OF(sail_int, mach_int)(&zgsz355, INT64_C(0));
      goto finish_match_100;
    }
  case_102: ;
    sail_match_failure("n_leading_spaces");
  finish_match_100: ;
    COPY(sail_int)(&zgsz352, zgsz355);
    KILL(sail_int)(&zgsz355);
    KILL(sail_string)(&zgaz314);
    goto finish_match_97;
  }
case_99: ;
  sail_match_failure("n_leading_spaces");
finish_match_97: ;
  COPY(sail_int)((*(&zcbz331)), zgsz352);
  KILL(sail_int)(&zgsz352);
end_function_103: ;
  goto end_function_213;
end_block_exception_104: ;
  goto end_function_213;
end_function_213: ;
}

static void sailgen_zzzz7nzzJzzK64z3MAX(sail_int *rop, int64_t);

static int64_t sailgen_zzzz7nzzJzzK18z3MAX(int64_t);

static void sailgen_zzzz7nzzJzzK64z3MAX(sail_int *zcbz332, int64_t zn)
{
  __label__ end_function_106, end_block_exception_107, end_function_212;

  sail_int zgaz317;
  CREATE(sail_int)(&zgaz317);
  {
    sail_int zgsz361;
    CREATE(sail_int)(&zgsz361);
    CONVERT_OF(sail_int, mach_int)(&zgsz361, zn);
    pow2(&zgaz317, zgsz361);
    KILL(sail_int)(&zgsz361);
  }
  {
    sail_int zgsz362;
    CREATE(sail_int)(&zgsz362);
    CONVERT_OF(sail_int, mach_int)(&zgsz362, INT64_C(1));
    sub_int((*(&zcbz332)), zgaz317, zgsz362);
    KILL(sail_int)(&zgsz362);
  }
  KILL(sail_int)(&zgaz317);
end_function_106: ;
  goto end_function_212;
end_block_exception_107: ;
  goto end_function_212;
end_function_212: ;
}

static int64_t sailgen_zzzz7nzzJzzK18z3MAX(int64_t zn)
{
  __label__ end_function_109, end_block_exception_110;

  int64_t zcbz333;
  int64_t zgaz318;
  {
    sail_int zgsz363;
    CREATE(sail_int)(&zgsz363);
    CONVERT_OF(sail_int, mach_int)(&zgsz363, zn);
    sail_int zgsz364;
    CREATE(sail_int)(&zgsz364);
    pow2(&zgsz364, zgsz363);
    zgaz318 = CONVERT_OF(mach_int, sail_int)(zgsz364);
    KILL(sail_int)(&zgsz364);
    KILL(sail_int)(&zgsz363);
  }
  {
    sail_int zgsz365;
    CREATE(sail_int)(&zgsz365);
    CONVERT_OF(sail_int, mach_int)(&zgsz365, zgaz318);
    sail_int zgsz366;
    CREATE(sail_int)(&zgsz366);
    CONVERT_OF(sail_int, mach_int)(&zgsz366, INT64_C(1));
    sail_int zgsz367;
    CREATE(sail_int)(&zgsz367);
    sub_int(&zgsz367, zgsz365, zgsz366);
    zcbz333 = CONVERT_OF(mach_int, sail_int)(zgsz367);
    KILL(sail_int)(&zgsz367);
    KILL(sail_int)(&zgsz366);
    KILL(sail_int)(&zgsz365);
  }

end_function_109: ;
  return zcbz333;
end_block_exception_110: ;

  return INT64_C(0xdeadc0de);
}



static int64_t znum_flags;
static void create_letbind_0(void) {  


  int64_t zgsz368;
  zgsz368 = INT64_C(1);
  znum_flags = zgsz368;

let_end_111: ;
}
static void kill_letbind_0(void) {  
}

static int64_t zreserved_otypes;
static void create_letbind_1(void) {  


  int64_t zgsz369;
  zgsz369 = INT64_C(4);
  zreserved_otypes = zgsz369;

let_end_112: ;
}
static void kill_letbind_1(void) {  
}

static int64_t zotype_unsealed;
static void create_letbind_2(void) {  


  int64_t zgsz370;
  zgsz370 = INT64_C(-1);
  zotype_unsealed = zgsz370;

let_end_113: ;
}
static void kill_letbind_2(void) {  
}

static uint64_t zPCC_IDX;
static void create_letbind_3(void) {  


  uint64_t zgsz371;
  zgsz371 = UINT64_C(0b100000);
  zPCC_IDX = zgsz371;

let_end_114: ;
}
static void kill_letbind_3(void) {  
}

static uint64_t zDDC_IDX;
static void create_letbind_4(void) {  


  uint64_t zgsz372;
  zgsz372 = UINT64_C(0b100001);
  zDDC_IDX = zgsz372;

let_end_115: ;
}
static void kill_letbind_4(void) {  
}

static int64_t zcap_sizze;
static void create_letbind_5(void) {  


  int64_t zgsz373;
  zgsz373 = INT64_C(16);
  zcap_sizze = zgsz373;

let_end_116: ;
}
static void kill_letbind_5(void) {  
}

static int64_t zlog2_cap_sizze;
static void create_letbind_6(void) {  


  int64_t zgsz374;
  zgsz374 = INT64_C(4);
  zlog2_cap_sizze = zgsz374;

let_end_117: ;
}
static void kill_letbind_6(void) {  
}

static int64_t zotype_width;
static void create_letbind_7(void) {  


  int64_t zgsz375;
  zgsz375 = INT64_C(18);
  zotype_width = zgsz375;

let_end_118: ;
}
static void kill_letbind_7(void) {  
}

static int64_t zuperms_width;
static void create_letbind_8(void) {  


  int64_t zgsz376;
  zgsz376 = INT64_C(4);
  zuperms_width = zgsz376;

let_end_119: ;
}
static void kill_letbind_8(void) {  
}

static int64_t zcap_addr_width;
static void create_letbind_9(void) {  


  int64_t zgsz377;
  zgsz377 = INT64_C(64);
  zcap_addr_width = zgsz377;

let_end_120: ;
}
static void kill_letbind_9(void) {  
}

static int64_t zcap_len_width;
static void create_letbind_10(void) {  


  int64_t zgsz378;
  {
    sail_int zgsz379;
    CREATE(sail_int)(&zgsz379);
    CONVERT_OF(sail_int, mach_int)(&zgsz379, INT64_C(64));
    sail_int zgsz380;
    CREATE(sail_int)(&zgsz380);
    CONVERT_OF(sail_int, mach_int)(&zgsz380, INT64_C(1));
    sail_int zgsz381;
    CREATE(sail_int)(&zgsz381);
    add_int(&zgsz381, zgsz379, zgsz380);
    zgsz378 = CONVERT_OF(mach_int, sail_int)(zgsz381);
    KILL(sail_int)(&zgsz381);
    KILL(sail_int)(&zgsz380);
    KILL(sail_int)(&zgsz379);
  }
  zcap_len_width = zgsz378;

let_end_121: ;
}
static void kill_letbind_10(void) {  
}

static int64_t zmantissa_width;
static void create_letbind_11(void) {  


  int64_t zgsz382;
  zgsz382 = INT64_C(14);
  zmantissa_width = zgsz382;

let_end_122: ;
}
static void kill_letbind_11(void) {  
}

static sail_int zMAX_ADDR;
static void create_letbind_12(void) {    CREATE(sail_int)(&zMAX_ADDR);


  sail_int zgsz383;
  CREATE(sail_int)(&zgsz383);
  sailgen_zzzz7nzzJzzK64z3MAX(&zgsz383, zcap_addr_width);
  COPY(sail_int)(&zMAX_ADDR, zgsz383);
  KILL(sail_int)(&zgsz383);
let_end_123: ;
}
static void kill_letbind_12(void) {    KILL(sail_int)(&zMAX_ADDR);
}

static struct zCapability sailgen_undefined_Capability(unit);

static struct zCapability sailgen_undefined_Capability(unit zgsz384)
{
  __label__ end_function_125, end_block_exception_126;

  struct zCapability zcbz334;
  bool zgaz319;
  zgaz319 = undefined_bool(UNIT);
  uint64_t zgaz320;
  {
    sail_int zgsz385;
    CREATE(sail_int)(&zgsz385);
    CONVERT_OF(sail_int, mach_int)(&zgsz385, INT64_C(4));
    lbits zgsz386;
    CREATE(lbits)(&zgsz386);
    UNDEFINED(lbits)(&zgsz386, zgsz385);
    zgaz320 = CONVERT_OF(fbits, lbits)(zgsz386, true);
    KILL(lbits)(&zgsz386);
    KILL(sail_int)(&zgsz385);
  }
  bool zgaz321;
  zgaz321 = undefined_bool(UNIT);
  bool zgaz322;
  zgaz322 = undefined_bool(UNIT);
  bool zgaz323;
  zgaz323 = undefined_bool(UNIT);
  bool zgaz324;
  zgaz324 = undefined_bool(UNIT);
  bool zgaz325;
  zgaz325 = undefined_bool(UNIT);
  bool zgaz326;
  zgaz326 = undefined_bool(UNIT);
  bool zgaz327;
  zgaz327 = undefined_bool(UNIT);
  bool zgaz328;
  zgaz328 = undefined_bool(UNIT);
  bool zgaz329;
  zgaz329 = undefined_bool(UNIT);
  bool zgaz330;
  zgaz330 = undefined_bool(UNIT);
  bool zgaz331;
  zgaz331 = undefined_bool(UNIT);
  bool zgaz332;
  zgaz332 = undefined_bool(UNIT);
  uint64_t zgaz333;
  {
    sail_int zgsz387;
    CREATE(sail_int)(&zgsz387);
    CONVERT_OF(sail_int, mach_int)(&zgsz387, INT64_C(2));
    lbits zgsz388;
    CREATE(lbits)(&zgsz388);
    UNDEFINED(lbits)(&zgsz388, zgsz387);
    zgaz333 = CONVERT_OF(fbits, lbits)(zgsz388, true);
    KILL(lbits)(&zgsz388);
    KILL(sail_int)(&zgsz387);
  }
  bool zgaz334;
  zgaz334 = undefined_bool(UNIT);
  bool zgaz335;
  zgaz335 = undefined_bool(UNIT);
  uint64_t zgaz336;
  {
    sail_int zgsz389;
    CREATE(sail_int)(&zgsz389);
    CONVERT_OF(sail_int, mach_int)(&zgsz389, INT64_C(6));
    lbits zgsz390;
    CREATE(lbits)(&zgsz390);
    UNDEFINED(lbits)(&zgsz390, zgsz389);
    zgaz336 = CONVERT_OF(fbits, lbits)(zgsz390, true);
    KILL(lbits)(&zgsz390);
    KILL(sail_int)(&zgsz389);
  }
  bool zgaz337;
  zgaz337 = undefined_bool(UNIT);
  uint64_t zgaz338;
  {
    sail_int zgsz391;
    CREATE(sail_int)(&zgsz391);
    CONVERT_OF(sail_int, mach_int)(&zgsz391, INT64_C(14));
    lbits zgsz392;
    CREATE(lbits)(&zgsz392);
    UNDEFINED(lbits)(&zgsz392, zgsz391);
    zgaz338 = CONVERT_OF(fbits, lbits)(zgsz392, true);
    KILL(lbits)(&zgsz392);
    KILL(sail_int)(&zgsz391);
  }
  uint64_t zgaz339;
  {
    sail_int zgsz393;
    CREATE(sail_int)(&zgsz393);
    CONVERT_OF(sail_int, mach_int)(&zgsz393, INT64_C(14));
    lbits zgsz394;
    CREATE(lbits)(&zgsz394);
    UNDEFINED(lbits)(&zgsz394, zgsz393);
    zgaz339 = CONVERT_OF(fbits, lbits)(zgsz394, true);
    KILL(lbits)(&zgsz394);
    KILL(sail_int)(&zgsz393);
  }
  uint64_t zgaz340;
  {
    sail_int zgsz395;
    CREATE(sail_int)(&zgsz395);
    CONVERT_OF(sail_int, mach_int)(&zgsz395, INT64_C(18));
    lbits zgsz396;
    CREATE(lbits)(&zgsz396);
    UNDEFINED(lbits)(&zgsz396, zgsz395);
    zgaz340 = CONVERT_OF(fbits, lbits)(zgsz396, true);
    KILL(lbits)(&zgsz396);
    KILL(sail_int)(&zgsz395);
  }
  uint64_t zgaz341;
  {
    sail_int zgsz397;
    CREATE(sail_int)(&zgsz397);
    CONVERT_OF(sail_int, mach_int)(&zgsz397, INT64_C(64));
    lbits zgsz398;
    CREATE(lbits)(&zgsz398);
    UNDEFINED(lbits)(&zgsz398, zgsz397);
    zgaz341 = CONVERT_OF(fbits, lbits)(zgsz398, true);
    KILL(lbits)(&zgsz398);
    KILL(sail_int)(&zgsz397);
  }
  struct zCapability zgsz399;
  zgsz399.zB = zgaz338;
  zgsz399.zE = zgaz336;
  zgsz399.zT = zgaz339;
  zgsz399.zaccess_system_regs = zgaz322;
  zgsz399.zaddress = zgaz341;
  zgsz399.zflag_cap_mode = zgaz334;
  zgsz399.zglobal = zgaz332;
  zgsz399.zinternal_e = zgaz335;
  zgsz399.zotype = zgaz340;
  zgsz399.zpermit_ccall = zgaz324;
  zgsz399.zpermit_execute = zgaz331;
  zgsz399.zpermit_load = zgaz330;
  zgsz399.zpermit_load_cap = zgaz328;
  zgsz399.zpermit_seal = zgaz325;
  zgsz399.zpermit_set_CID = zgaz321;
  zgsz399.zpermit_store = zgaz329;
  zgsz399.zpermit_store_cap = zgaz327;
  zgsz399.zpermit_store_local_cap = zgaz326;
  zgsz399.zpermit_unseal = zgaz323;
  zgsz399.zreserved = zgaz333;
  zgsz399.zsealed = zgaz337;
  zgsz399.ztag = zgaz319;
  zgsz399.zuperms = zgaz320;
  zcbz334 = zgsz399;
























end_function_125: ;
  return zcbz334;
end_block_exception_126: ;
  struct zCapability zcbz354 = { .zuperms = UINT64_C(0xdeadc0de), .ztag = false, .zsealed = false, .zreserved = UINT64_C(0xdeadc0de), .zpermit_unseal = false, .zpermit_store_local_cap = false, .zpermit_store_cap = false, .zpermit_store = false, .zpermit_set_CID = false, .zpermit_seal = false, .zpermit_load_cap = false, .zpermit_load = false, .zpermit_execute = false, .zpermit_ccall = false, .zotype = UINT64_C(0xdeadc0de), .zinternal_e = false, .zglobal = false, .zflag_cap_mode = false, .zaddress = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zT = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  return zcbz354;
}

static int64_t zmaxE;
static void create_letbind_13(void) {  


  int64_t zgsz3100;
  zgsz3100 = INT64_C(52);
  zmaxE = zgsz3100;

let_end_127: ;
}
static void kill_letbind_13(void) {  
}

static int64_t zinternal_exponent_take_bits;
static void create_letbind_14(void) {  


  int64_t zgsz3101;
  zgsz3101 = INT64_C(3);
  zinternal_exponent_take_bits = zgsz3101;

let_end_128: ;
}
static void kill_letbind_14(void) {  
}

static uint64_t zresetE;
static void create_letbind_15(void) {  


  uint64_t zgsz3102;
  {
    sail_int zgsz3103;
    CREATE(sail_int)(&zgsz3103);
    CONVERT_OF(sail_int, mach_int)(&zgsz3103, INT64_C(6));
    sail_int zgsz3104;
    CREATE(sail_int)(&zgsz3104);
    CONVERT_OF(sail_int, mach_int)(&zgsz3104, zmaxE);
    lbits zgsz3105;
    CREATE(lbits)(&zgsz3105);
    sailgen_to_bits(&zgsz3105, zgsz3103, zgsz3104);
    zgsz3102 = CONVERT_OF(fbits, lbits)(zgsz3105, true);
    KILL(lbits)(&zgsz3105);
    KILL(sail_int)(&zgsz3104);
    KILL(sail_int)(&zgsz3103);
  }
  zresetE = zgsz3102;

let_end_129: ;
}
static void kill_letbind_15(void) {  
}

static uint64_t zresetT;
static void create_letbind_16(void) {  


  uint64_t zgsz3106;
  {
    lbits zgsz3107;
    CREATE(lbits)(&zgsz3107);
    CONVERT_OF(lbits, fbits)(&zgsz3107, UINT64_C(0b01), UINT64_C(2) , true);
    lbits zgsz3108;
    CREATE(lbits)(&zgsz3108);
    CONVERT_OF(lbits, fbits)(&zgsz3108, UINT64_C(0x000), UINT64_C(12) , true);
    lbits zgsz3109;
    CREATE(lbits)(&zgsz3109);
    append(&zgsz3109, zgsz3107, zgsz3108);
    zgsz3106 = CONVERT_OF(fbits, lbits)(zgsz3109, true);
    KILL(lbits)(&zgsz3109);
    KILL(lbits)(&zgsz3108);
    KILL(lbits)(&zgsz3107);
  }
  zresetT = zgsz3106;

let_end_130: ;
}
static void kill_letbind_16(void) {  
}

static struct zCapability znull_cap;
static void create_letbind_17(void) {  


  struct zCapability zgsz3114;
  uint64_t zgaz342;
  zgaz342 = sailgen_zzzz7nzzJzzK4z3zzeros_implicit(INT64_C(4));
  uint64_t zgaz343;
  zgaz343 = sailgen_zzzz7nzzJzzK2z3zzeros_implicit(INT64_C(2));
  uint64_t zgaz344;
  zgaz344 = sailgen_zzzz7nzzJzzK14z3zzeros_implicit(INT64_C(14));
  uint64_t zgaz345;
  {
    sail_int zgsz3110;
    CREATE(sail_int)(&zgsz3110);
    CONVERT_OF(sail_int, mach_int)(&zgsz3110, zotype_width);
    sail_int zgsz3111;
    CREATE(sail_int)(&zgsz3111);
    CONVERT_OF(sail_int, mach_int)(&zgsz3111, zotype_unsealed);
    lbits zgsz3112;
    CREATE(lbits)(&zgsz3112);
    sailgen_to_bits(&zgsz3112, zgsz3110, zgsz3111);
    zgaz345 = CONVERT_OF(fbits, lbits)(zgsz3112, true);
    KILL(lbits)(&zgsz3112);
    KILL(sail_int)(&zgsz3111);
    KILL(sail_int)(&zgsz3110);
  }
  uint64_t zgaz346;
  zgaz346 = sailgen_zzzz7nzzJzzK64z3zzeros_implicit(INT64_C(64));
  struct zCapability zgsz3113;
  zgsz3113.zB = zgaz344;
  zgsz3113.zE = zresetE;
  zgsz3113.zT = zresetT;
  zgsz3113.zaccess_system_regs = false;
  zgsz3113.zaddress = zgaz346;
  zgsz3113.zflag_cap_mode = false;
  zgsz3113.zglobal = false;
  zgsz3113.zinternal_e = true;
  zgsz3113.zotype = zgaz345;
  zgsz3113.zpermit_ccall = false;
  zgsz3113.zpermit_execute = false;
  zgsz3113.zpermit_load = false;
  zgsz3113.zpermit_load_cap = false;
  zgsz3113.zpermit_seal = false;
  zgsz3113.zpermit_set_CID = false;
  zgsz3113.zpermit_store = false;
  zgsz3113.zpermit_store_cap = false;
  zgsz3113.zpermit_store_local_cap = false;
  zgsz3113.zpermit_unseal = false;
  zgsz3113.zreserved = zgaz343;
  zgsz3113.zsealed = false;
  zgsz3113.ztag = false;
  zgsz3113.zuperms = zgaz342;
  zgsz3114 = zgsz3113;






  znull_cap = zgsz3114;

let_end_131: ;
}
static void kill_letbind_17(void) {  
}

static struct zCapability zdefault_cap;
static void create_letbind_18(void) {  


  struct zCapability zgsz3119;
  uint64_t zgaz347;
  zgaz347 = sailgen_zzzz7nzzJzzK4z3ones(INT64_C(4));
  uint64_t zgaz348;
  zgaz348 = sailgen_zzzz7nzzJzzK2z3zzeros_implicit(INT64_C(2));
  uint64_t zgaz349;
  zgaz349 = sailgen_zzzz7nzzJzzK14z3zzeros_implicit(INT64_C(14));
  uint64_t zgaz350;
  {
    sail_int zgsz3115;
    CREATE(sail_int)(&zgsz3115);
    CONVERT_OF(sail_int, mach_int)(&zgsz3115, zotype_width);
    sail_int zgsz3116;
    CREATE(sail_int)(&zgsz3116);
    CONVERT_OF(sail_int, mach_int)(&zgsz3116, zotype_unsealed);
    lbits zgsz3117;
    CREATE(lbits)(&zgsz3117);
    sailgen_to_bits(&zgsz3117, zgsz3115, zgsz3116);
    zgaz350 = CONVERT_OF(fbits, lbits)(zgsz3117, true);
    KILL(lbits)(&zgsz3117);
    KILL(sail_int)(&zgsz3116);
    KILL(sail_int)(&zgsz3115);
  }
  uint64_t zgaz351;
  zgaz351 = sailgen_zzzz7nzzJzzK64z3zzeros_implicit(INT64_C(64));
  struct zCapability zgsz3118;
  zgsz3118.zB = zgaz349;
  zgsz3118.zE = zresetE;
  zgsz3118.zT = zresetT;
  zgsz3118.zaccess_system_regs = true;
  zgsz3118.zaddress = zgaz351;
  zgsz3118.zflag_cap_mode = false;
  zgsz3118.zglobal = true;
  zgsz3118.zinternal_e = true;
  zgsz3118.zotype = zgaz350;
  zgsz3118.zpermit_ccall = true;
  zgsz3118.zpermit_execute = true;
  zgsz3118.zpermit_load = true;
  zgsz3118.zpermit_load_cap = true;
  zgsz3118.zpermit_seal = true;
  zgsz3118.zpermit_set_CID = true;
  zgsz3118.zpermit_store = true;
  zgsz3118.zpermit_store_cap = true;
  zgsz3118.zpermit_store_local_cap = true;
  zgsz3118.zpermit_unseal = true;
  zgsz3118.zreserved = zgaz348;
  zgsz3118.zsealed = false;
  zgsz3118.ztag = true;
  zgsz3118.zuperms = zgaz347;
  zgsz3119 = zgsz3118;






  zdefault_cap = zgsz3119;

let_end_132: ;
}
static void kill_letbind_18(void) {  
}

static struct zCapability sailgen_capBitsToCapability(bool, lbits);

static struct zCapability sailgen_capBitsToCapability(bool zt, lbits zc)
{
  __label__ cleanup_135, end_cleanup_136, end_function_134, end_block_exception_137;

  struct zCapability zcbz335;
  bool zinternal_exponent;
  {
    fbits zgaz392;
    {
      sail_int zgsz3120;
      CREATE(sail_int)(&zgsz3120);
      CONVERT_OF(sail_int, mach_int)(&zgsz3120, INT64_C(90));
      zgaz392 = bitvector_access(zc, zgsz3120);
      KILL(sail_int)(&zgsz3120);
    }
    zinternal_exponent = sailgen_bit_to_bool(zgaz392);
  
  }
  uint64_t zotype;
  {
    sail_int zgsz3121;
    CREATE(sail_int)(&zgsz3121);
    CONVERT_OF(sail_int, mach_int)(&zgsz3121, INT64_C(108));
    sail_int zgsz3122;
    CREATE(sail_int)(&zgsz3122);
    CONVERT_OF(sail_int, mach_int)(&zgsz3122, INT64_C(91));
    lbits zgsz3123;
    CREATE(lbits)(&zgsz3123);
    vector_subrange_lbits(&zgsz3123, zc, zgsz3121, zgsz3122);
    zotype = CONVERT_OF(fbits, lbits)(zgsz3123, true);
    KILL(lbits)(&zgsz3123);
    KILL(sail_int)(&zgsz3122);
    KILL(sail_int)(&zgsz3121);
  }
  bool zsealed;
  {
    uint64_t zgaz391;
    {
      sail_int zgsz3124;
      CREATE(sail_int)(&zgsz3124);
      CONVERT_OF(sail_int, mach_int)(&zgsz3124, zotype_width);
      sail_int zgsz3125;
      CREATE(sail_int)(&zgsz3125);
      CONVERT_OF(sail_int, mach_int)(&zgsz3125, zotype_unsealed);
      lbits zgsz3126;
      CREATE(lbits)(&zgsz3126);
      sailgen_to_bits(&zgsz3126, zgsz3124, zgsz3125);
      zgaz391 = CONVERT_OF(fbits, lbits)(zgsz3126, true);
      KILL(lbits)(&zgsz3126);
      KILL(sail_int)(&zgsz3125);
      KILL(sail_int)(&zgsz3124);
    }
    {
      lbits zgsz3128;
      CREATE(lbits)(&zgsz3128);
      CONVERT_OF(lbits, fbits)(&zgsz3128, zgaz391, UINT64_C(18) , true);
      lbits zgsz3127;
      CREATE(lbits)(&zgsz3127);
      CONVERT_OF(lbits, fbits)(&zgsz3127, zotype, UINT64_C(18) , true);
      zsealed = neq_bits(zgsz3127, zgsz3128);
      KILL(lbits)(&zgsz3128);
      KILL(lbits)(&zgsz3127);
    }
  
  }
  uint64_t zE;
  zE = sailgen_zzzz7nzzJzzK6z3zzeros_implicit(INT64_C(6));
  uint64_t zBs;
  zBs = sailgen_zzzz7nzzJzzK14z3zzeros_implicit(INT64_C(14));
  uint64_t zT;
  zT = sailgen_zzzz7nzzJzzK12z3zzeros_implicit(INT64_C(12));
  uint64_t zlenMSBs;
  zlenMSBs = sailgen_zzzz7nzzJzzK2z3zzeros_implicit(INT64_C(2));
  {
    unit zgsz3129;
    if (zinternal_exponent) {
    {
      uint64_t zgaz352;
      {
        sail_int zgsz3138;
        CREATE(sail_int)(&zgsz3138);
        CONVERT_OF(sail_int, mach_int)(&zgsz3138, INT64_C(80));
        sail_int zgsz3139;
        CREATE(sail_int)(&zgsz3139);
        CONVERT_OF(sail_int, mach_int)(&zgsz3139, INT64_C(78));
        lbits zgsz3140;
        CREATE(lbits)(&zgsz3140);
        vector_subrange_lbits(&zgsz3140, zc, zgsz3138, zgsz3139);
        zgaz352 = CONVERT_OF(fbits, lbits)(zgsz3140, true);
        KILL(lbits)(&zgsz3140);
        KILL(sail_int)(&zgsz3139);
        KILL(sail_int)(&zgsz3138);
      }
      uint64_t zgaz353;
      {
        sail_int zgsz3141;
        CREATE(sail_int)(&zgsz3141);
        CONVERT_OF(sail_int, mach_int)(&zgsz3141, INT64_C(66));
        sail_int zgsz3142;
        CREATE(sail_int)(&zgsz3142);
        CONVERT_OF(sail_int, mach_int)(&zgsz3142, INT64_C(64));
        lbits zgsz3143;
        CREATE(lbits)(&zgsz3143);
        vector_subrange_lbits(&zgsz3143, zc, zgsz3141, zgsz3142);
        zgaz353 = CONVERT_OF(fbits, lbits)(zgsz3143, true);
        KILL(lbits)(&zgsz3143);
        KILL(sail_int)(&zgsz3142);
        KILL(sail_int)(&zgsz3141);
      }
      {
        lbits zgsz3144;
        CREATE(lbits)(&zgsz3144);
        CONVERT_OF(lbits, fbits)(&zgsz3144, zgaz352, UINT64_C(3) , true);
        lbits zgsz3145;
        CREATE(lbits)(&zgsz3145);
        CONVERT_OF(lbits, fbits)(&zgsz3145, zgaz353, UINT64_C(3) , true);
        lbits zgsz3146;
        CREATE(lbits)(&zgsz3146);
        append(&zgsz3146, zgsz3144, zgsz3145);
        zE = CONVERT_OF(fbits, lbits)(zgsz3146, true);
        KILL(lbits)(&zgsz3146);
        KILL(lbits)(&zgsz3145);
        KILL(lbits)(&zgsz3144);
      }
      unit zgsz3155;
      zgsz3155 = UNIT;
    
    
    }
    {
      zlenMSBs = UINT64_C(0b01);
      unit zgsz3154;
      zgsz3154 = UNIT;
    }
    {
      uint64_t zgaz354;
      {
        sail_int zgsz3147;
        CREATE(sail_int)(&zgsz3147);
        CONVERT_OF(sail_int, mach_int)(&zgsz3147, INT64_C(89));
        sail_int zgsz3148;
        CREATE(sail_int)(&zgsz3148);
        CONVERT_OF(sail_int, mach_int)(&zgsz3148, INT64_C(81));
        lbits zgsz3149;
        CREATE(lbits)(&zgsz3149);
        vector_subrange_lbits(&zgsz3149, zc, zgsz3147, zgsz3148);
        zgaz354 = CONVERT_OF(fbits, lbits)(zgsz3149, true);
        KILL(lbits)(&zgsz3149);
        KILL(sail_int)(&zgsz3148);
        KILL(sail_int)(&zgsz3147);
      }
      {
        lbits zgsz3150;
        CREATE(lbits)(&zgsz3150);
        CONVERT_OF(lbits, fbits)(&zgsz3150, zgaz354, UINT64_C(9) , true);
        lbits zgsz3151;
        CREATE(lbits)(&zgsz3151);
        CONVERT_OF(lbits, fbits)(&zgsz3151, UINT64_C(0b000), UINT64_C(3) , true);
        lbits zgsz3152;
        CREATE(lbits)(&zgsz3152);
        append(&zgsz3152, zgsz3150, zgsz3151);
        zT = CONVERT_OF(fbits, lbits)(zgsz3152, true);
        KILL(lbits)(&zgsz3152);
        KILL(lbits)(&zgsz3151);
        KILL(lbits)(&zgsz3150);
      }
      unit zgsz3153;
      zgsz3153 = UNIT;
    
    }
    uint64_t zgaz355;
    {
      sail_int zgsz3156;
      CREATE(sail_int)(&zgsz3156);
      CONVERT_OF(sail_int, mach_int)(&zgsz3156, INT64_C(77));
      sail_int zgsz3157;
      CREATE(sail_int)(&zgsz3157);
      CONVERT_OF(sail_int, mach_int)(&zgsz3157, INT64_C(67));
      lbits zgsz3158;
      CREATE(lbits)(&zgsz3158);
      vector_subrange_lbits(&zgsz3158, zc, zgsz3156, zgsz3157);
      zgaz355 = CONVERT_OF(fbits, lbits)(zgsz3158, true);
      KILL(lbits)(&zgsz3158);
      KILL(sail_int)(&zgsz3157);
      KILL(sail_int)(&zgsz3156);
    }
    {
      lbits zgsz3159;
      CREATE(lbits)(&zgsz3159);
      CONVERT_OF(lbits, fbits)(&zgsz3159, zgaz355, UINT64_C(11) , true);
      lbits zgsz3160;
      CREATE(lbits)(&zgsz3160);
      CONVERT_OF(lbits, fbits)(&zgsz3160, UINT64_C(0b000), UINT64_C(3) , true);
      lbits zgsz3161;
      CREATE(lbits)(&zgsz3161);
      append(&zgsz3161, zgsz3159, zgsz3160);
      zBs = CONVERT_OF(fbits, lbits)(zgsz3161, true);
      KILL(lbits)(&zgsz3161);
      KILL(lbits)(&zgsz3160);
      KILL(lbits)(&zgsz3159);
    }
    zgsz3129 = UNIT;
  
    } else {
    {
      zlenMSBs = UINT64_C(0b00);
      unit zgsz3134;
      zgsz3134 = UNIT;
    }
    {
      {
        sail_int zgsz3130;
        CREATE(sail_int)(&zgsz3130);
        CONVERT_OF(sail_int, mach_int)(&zgsz3130, INT64_C(89));
        sail_int zgsz3131;
        CREATE(sail_int)(&zgsz3131);
        CONVERT_OF(sail_int, mach_int)(&zgsz3131, INT64_C(78));
        lbits zgsz3132;
        CREATE(lbits)(&zgsz3132);
        vector_subrange_lbits(&zgsz3132, zc, zgsz3130, zgsz3131);
        zT = CONVERT_OF(fbits, lbits)(zgsz3132, true);
        KILL(lbits)(&zgsz3132);
        KILL(sail_int)(&zgsz3131);
        KILL(sail_int)(&zgsz3130);
      }
      unit zgsz3133;
      zgsz3133 = UNIT;
    }
    {
      sail_int zgsz3135;
      CREATE(sail_int)(&zgsz3135);
      CONVERT_OF(sail_int, mach_int)(&zgsz3135, INT64_C(77));
      sail_int zgsz3136;
      CREATE(sail_int)(&zgsz3136);
      CONVERT_OF(sail_int, mach_int)(&zgsz3136, INT64_C(64));
      lbits zgsz3137;
      CREATE(lbits)(&zgsz3137);
      vector_subrange_lbits(&zgsz3137, zc, zgsz3135, zgsz3136);
      zBs = CONVERT_OF(fbits, lbits)(zgsz3137, true);
      KILL(lbits)(&zgsz3137);
      KILL(sail_int)(&zgsz3136);
      KILL(sail_int)(&zgsz3135);
    }
    zgsz3129 = UNIT;
    }
  }
  uint64_t zcarry_out;
  {
    bool zgaz390;
    {
      uint64_t zgaz389;
      {
        lbits zgsz3162;
        CREATE(lbits)(&zgsz3162);
        CONVERT_OF(lbits, fbits)(&zgsz3162, zBs, UINT64_C(14) , true);
        sail_int zgsz3163;
        CREATE(sail_int)(&zgsz3163);
        CONVERT_OF(sail_int, mach_int)(&zgsz3163, INT64_C(11));
        sail_int zgsz3164;
        CREATE(sail_int)(&zgsz3164);
        CONVERT_OF(sail_int, mach_int)(&zgsz3164, INT64_C(0));
        lbits zgsz3165;
        CREATE(lbits)(&zgsz3165);
        vector_subrange_lbits(&zgsz3165, zgsz3162, zgsz3163, zgsz3164);
        zgaz389 = CONVERT_OF(fbits, lbits)(zgsz3165, true);
        KILL(lbits)(&zgsz3165);
        KILL(sail_int)(&zgsz3164);
        KILL(sail_int)(&zgsz3163);
        KILL(lbits)(&zgsz3162);
      }
      zgaz390 = sailgen_z8operatorz0zzzz7nzzJzzK12z3zI_uz9(zT, zgaz389);
    
    }
    if (zgaz390) {  zcarry_out = UINT64_C(0b01);  } else {  zcarry_out = UINT64_C(0b00);  }
  
  }
  uint64_t zTtop2;
  {
    uint64_t zgaz388;
    {
      uint64_t zgaz387;
      {
        lbits zgsz3166;
        CREATE(lbits)(&zgsz3166);
        CONVERT_OF(lbits, fbits)(&zgsz3166, zBs, UINT64_C(14) , true);
        sail_int zgsz3167;
        CREATE(sail_int)(&zgsz3167);
        CONVERT_OF(sail_int, mach_int)(&zgsz3167, INT64_C(13));
        sail_int zgsz3168;
        CREATE(sail_int)(&zgsz3168);
        CONVERT_OF(sail_int, mach_int)(&zgsz3168, INT64_C(12));
        lbits zgsz3169;
        CREATE(lbits)(&zgsz3169);
        vector_subrange_lbits(&zgsz3169, zgsz3166, zgsz3167, zgsz3168);
        zgaz387 = CONVERT_OF(fbits, lbits)(zgsz3169, true);
        KILL(lbits)(&zgsz3169);
        KILL(sail_int)(&zgsz3168);
        KILL(sail_int)(&zgsz3167);
        KILL(lbits)(&zgsz3166);
      }
      {
        lbits zgsz3170;
        CREATE(lbits)(&zgsz3170);
        CONVERT_OF(lbits, fbits)(&zgsz3170, zgaz387, UINT64_C(2) , true);
        lbits zgsz3171;
        CREATE(lbits)(&zgsz3171);
        CONVERT_OF(lbits, fbits)(&zgsz3171, zlenMSBs, UINT64_C(2) , true);
        lbits zgsz3172;
        CREATE(lbits)(&zgsz3172);
        add_bits(&zgsz3172, zgsz3170, zgsz3171);
        zgaz388 = CONVERT_OF(fbits, lbits)(zgsz3172, true);
        KILL(lbits)(&zgsz3172);
        KILL(lbits)(&zgsz3171);
        KILL(lbits)(&zgsz3170);
      }
    
    }
    {
      lbits zgsz3173;
      CREATE(lbits)(&zgsz3173);
      CONVERT_OF(lbits, fbits)(&zgsz3173, zgaz388, UINT64_C(2) , true);
      lbits zgsz3174;
      CREATE(lbits)(&zgsz3174);
      CONVERT_OF(lbits, fbits)(&zgsz3174, zcarry_out, UINT64_C(2) , true);
      lbits zgsz3175;
      CREATE(lbits)(&zgsz3175);
      add_bits(&zgsz3175, zgsz3173, zgsz3174);
      zTtop2 = CONVERT_OF(fbits, lbits)(zgsz3175, true);
      KILL(lbits)(&zgsz3175);
      KILL(lbits)(&zgsz3174);
      KILL(lbits)(&zgsz3173);
    }
  
  }
  struct zCapability zgaz386;
  {
    uint64_t zgaz356;
    {
      sail_int zgsz3176;
      CREATE(sail_int)(&zgsz3176);
      CONVERT_OF(sail_int, mach_int)(&zgsz3176, INT64_C(127));
      sail_int zgsz3177;
      CREATE(sail_int)(&zgsz3177);
      CONVERT_OF(sail_int, mach_int)(&zgsz3177, INT64_C(124));
      lbits zgsz3178;
      CREATE(lbits)(&zgsz3178);
      vector_subrange_lbits(&zgsz3178, zc, zgsz3176, zgsz3177);
      zgaz356 = CONVERT_OF(fbits, lbits)(zgsz3178, true);
      KILL(lbits)(&zgsz3178);
      KILL(sail_int)(&zgsz3177);
      KILL(sail_int)(&zgsz3176);
    }
    bool zgaz358;
    {
      fbits zgaz357;
      {
        sail_int zgsz3179;
        CREATE(sail_int)(&zgsz3179);
        CONVERT_OF(sail_int, mach_int)(&zgsz3179, INT64_C(123));
        zgaz357 = bitvector_access(zc, zgsz3179);
        KILL(sail_int)(&zgsz3179);
      }
      zgaz358 = sailgen_bit_to_bool(zgaz357);
    
    }
    bool zgaz360;
    {
      fbits zgaz359;
      {
        sail_int zgsz3180;
        CREATE(sail_int)(&zgsz3180);
        CONVERT_OF(sail_int, mach_int)(&zgsz3180, INT64_C(122));
        zgaz359 = bitvector_access(zc, zgsz3180);
        KILL(sail_int)(&zgsz3180);
      }
      zgaz360 = sailgen_bit_to_bool(zgaz359);
    
    }
    bool zgaz362;
    {
      fbits zgaz361;
      {
        sail_int zgsz3181;
        CREATE(sail_int)(&zgsz3181);
        CONVERT_OF(sail_int, mach_int)(&zgsz3181, INT64_C(121));
        zgaz361 = bitvector_access(zc, zgsz3181);
        KILL(sail_int)(&zgsz3181);
      }
      zgaz362 = sailgen_bit_to_bool(zgaz361);
    
    }
    bool zgaz364;
    {
      fbits zgaz363;
      {
        sail_int zgsz3182;
        CREATE(sail_int)(&zgsz3182);
        CONVERT_OF(sail_int, mach_int)(&zgsz3182, INT64_C(120));
        zgaz363 = bitvector_access(zc, zgsz3182);
        KILL(sail_int)(&zgsz3182);
      }
      zgaz364 = sailgen_bit_to_bool(zgaz363);
    
    }
    bool zgaz366;
    {
      fbits zgaz365;
      {
        sail_int zgsz3183;
        CREATE(sail_int)(&zgsz3183);
        CONVERT_OF(sail_int, mach_int)(&zgsz3183, INT64_C(119));
        zgaz365 = bitvector_access(zc, zgsz3183);
        KILL(sail_int)(&zgsz3183);
      }
      zgaz366 = sailgen_bit_to_bool(zgaz365);
    
    }
    bool zgaz368;
    {
      fbits zgaz367;
      {
        sail_int zgsz3184;
        CREATE(sail_int)(&zgsz3184);
        CONVERT_OF(sail_int, mach_int)(&zgsz3184, INT64_C(118));
        zgaz367 = bitvector_access(zc, zgsz3184);
        KILL(sail_int)(&zgsz3184);
      }
      zgaz368 = sailgen_bit_to_bool(zgaz367);
    
    }
    bool zgaz370;
    {
      fbits zgaz369;
      {
        sail_int zgsz3185;
        CREATE(sail_int)(&zgsz3185);
        CONVERT_OF(sail_int, mach_int)(&zgsz3185, INT64_C(117));
        zgaz369 = bitvector_access(zc, zgsz3185);
        KILL(sail_int)(&zgsz3185);
      }
      zgaz370 = sailgen_bit_to_bool(zgaz369);
    
    }
    bool zgaz372;
    {
      fbits zgaz371;
      {
        sail_int zgsz3186;
        CREATE(sail_int)(&zgsz3186);
        CONVERT_OF(sail_int, mach_int)(&zgsz3186, INT64_C(116));
        zgaz371 = bitvector_access(zc, zgsz3186);
        KILL(sail_int)(&zgsz3186);
      }
      zgaz372 = sailgen_bit_to_bool(zgaz371);
    
    }
    bool zgaz374;
    {
      fbits zgaz373;
      {
        sail_int zgsz3187;
        CREATE(sail_int)(&zgsz3187);
        CONVERT_OF(sail_int, mach_int)(&zgsz3187, INT64_C(115));
        zgaz373 = bitvector_access(zc, zgsz3187);
        KILL(sail_int)(&zgsz3187);
      }
      zgaz374 = sailgen_bit_to_bool(zgaz373);
    
    }
    bool zgaz376;
    {
      fbits zgaz375;
      {
        sail_int zgsz3188;
        CREATE(sail_int)(&zgsz3188);
        CONVERT_OF(sail_int, mach_int)(&zgsz3188, INT64_C(114));
        zgaz375 = bitvector_access(zc, zgsz3188);
        KILL(sail_int)(&zgsz3188);
      }
      zgaz376 = sailgen_bit_to_bool(zgaz375);
    
    }
    bool zgaz378;
    {
      fbits zgaz377;
      {
        sail_int zgsz3189;
        CREATE(sail_int)(&zgsz3189);
        CONVERT_OF(sail_int, mach_int)(&zgsz3189, INT64_C(113));
        zgaz377 = bitvector_access(zc, zgsz3189);
        KILL(sail_int)(&zgsz3189);
      }
      zgaz378 = sailgen_bit_to_bool(zgaz377);
    
    }
    bool zgaz380;
    {
      fbits zgaz379;
      {
        sail_int zgsz3190;
        CREATE(sail_int)(&zgsz3190);
        CONVERT_OF(sail_int, mach_int)(&zgsz3190, INT64_C(112));
        zgaz379 = bitvector_access(zc, zgsz3190);
        KILL(sail_int)(&zgsz3190);
      }
      zgaz380 = sailgen_bit_to_bool(zgaz379);
    
    }
    uint64_t zgaz381;
    {
      sail_int zgsz3191;
      CREATE(sail_int)(&zgsz3191);
      CONVERT_OF(sail_int, mach_int)(&zgsz3191, INT64_C(111));
      sail_int zgsz3192;
      CREATE(sail_int)(&zgsz3192);
      CONVERT_OF(sail_int, mach_int)(&zgsz3192, INT64_C(110));
      lbits zgsz3193;
      CREATE(lbits)(&zgsz3193);
      vector_subrange_lbits(&zgsz3193, zc, zgsz3191, zgsz3192);
      zgaz381 = CONVERT_OF(fbits, lbits)(zgsz3193, true);
      KILL(lbits)(&zgsz3193);
      KILL(sail_int)(&zgsz3192);
      KILL(sail_int)(&zgsz3191);
    }
    bool zgaz383;
    {
      fbits zgaz382;
      {
        sail_int zgsz3194;
        CREATE(sail_int)(&zgsz3194);
        CONVERT_OF(sail_int, mach_int)(&zgsz3194, INT64_C(109));
        zgaz382 = bitvector_access(zc, zgsz3194);
        KILL(sail_int)(&zgsz3194);
      }
      zgaz383 = sailgen_bit_to_bool(zgaz382);
    
    }
    uint64_t zgaz384;
    {
      lbits zgsz3195;
      CREATE(lbits)(&zgsz3195);
      CONVERT_OF(lbits, fbits)(&zgsz3195, zTtop2, UINT64_C(2) , true);
      lbits zgsz3196;
      CREATE(lbits)(&zgsz3196);
      CONVERT_OF(lbits, fbits)(&zgsz3196, zT, UINT64_C(12) , true);
      lbits zgsz3197;
      CREATE(lbits)(&zgsz3197);
      append(&zgsz3197, zgsz3195, zgsz3196);
      zgaz384 = CONVERT_OF(fbits, lbits)(zgsz3197, true);
      KILL(lbits)(&zgsz3197);
      KILL(lbits)(&zgsz3196);
      KILL(lbits)(&zgsz3195);
    }
    uint64_t zgaz385;
    {
      sail_int zgsz3198;
      CREATE(sail_int)(&zgsz3198);
      CONVERT_OF(sail_int, mach_int)(&zgsz3198, INT64_C(63));
      sail_int zgsz3199;
      CREATE(sail_int)(&zgsz3199);
      CONVERT_OF(sail_int, mach_int)(&zgsz3199, INT64_C(0));
      lbits zgsz3200;
      CREATE(lbits)(&zgsz3200);
      vector_subrange_lbits(&zgsz3200, zc, zgsz3198, zgsz3199);
      zgaz385 = CONVERT_OF(fbits, lbits)(zgsz3200, true);
      KILL(lbits)(&zgsz3200);
      KILL(sail_int)(&zgsz3199);
      KILL(sail_int)(&zgsz3198);
    }
    struct zCapability zgsz3201;
    zgsz3201.zB = zBs;
    zgsz3201.zE = zE;
    zgsz3201.zT = zgaz384;
    zgsz3201.zaccess_system_regs = zgaz360;
    zgsz3201.zaddress = zgaz385;
    zgsz3201.zflag_cap_mode = zgaz383;
    zgsz3201.zglobal = zgaz380;
    zgsz3201.zinternal_e = zinternal_exponent;
    zgsz3201.zotype = zotype;
    zgsz3201.zpermit_ccall = zgaz364;
    zgsz3201.zpermit_execute = zgaz378;
    zgsz3201.zpermit_load = zgaz376;
    zgsz3201.zpermit_load_cap = zgaz372;
    zgsz3201.zpermit_seal = zgaz366;
    zgsz3201.zpermit_set_CID = zgaz358;
    zgsz3201.zpermit_store = zgaz374;
    zgsz3201.zpermit_store_cap = zgaz370;
    zgsz3201.zpermit_store_local_cap = zgaz368;
    zgsz3201.zpermit_unseal = zgaz362;
    zgsz3201.zreserved = zgaz381;
    zgsz3201.zsealed = zsealed;
    zgsz3201.ztag = zt;
    zgsz3201.zuperms = zgaz356;
    zgaz386 = zgsz3201;
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  }
  zcbz335 = zgaz386;
  goto cleanup_135;
  /* unreachable after return */










  goto end_cleanup_136;
cleanup_135: ;










  goto end_function_134;
end_cleanup_136: ;
end_function_134: ;
  return zcbz335;
end_block_exception_137: ;
  struct zCapability zcbz355 = { .zuperms = UINT64_C(0xdeadc0de), .ztag = false, .zsealed = false, .zreserved = UINT64_C(0xdeadc0de), .zpermit_unseal = false, .zpermit_store_local_cap = false, .zpermit_store_cap = false, .zpermit_store = false, .zpermit_set_CID = false, .zpermit_seal = false, .zpermit_load_cap = false, .zpermit_load = false, .zpermit_execute = false, .zpermit_ccall = false, .zotype = UINT64_C(0xdeadc0de), .zinternal_e = false, .zglobal = false, .zflag_cap_mode = false, .zaddress = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zT = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  return zcbz355;
}

static int64_t zmax_otype;
static void create_letbind_19(void) {  


  int64_t zgsz3202;
  int64_t zgaz393;
  zgaz393 = sailgen_zzzz7nzzJzzK18z3MAX(zotype_width);
  {
    sail_int zgsz3203;
    CREATE(sail_int)(&zgsz3203);
    CONVERT_OF(sail_int, mach_int)(&zgsz3203, zgaz393);
    sail_int zgsz3204;
    CREATE(sail_int)(&zgsz3204);
    CONVERT_OF(sail_int, mach_int)(&zgsz3204, zreserved_otypes);
    sail_int zgsz3205;
    CREATE(sail_int)(&zgsz3205);
    sub_int(&zgsz3205, zgsz3203, zgsz3204);
    zgsz3202 = CONVERT_OF(mach_int, sail_int)(zgsz3205);
    KILL(sail_int)(&zgsz3205);
    KILL(sail_int)(&zgsz3204);
    KILL(sail_int)(&zgsz3203);
  }

  zmax_otype = zgsz3202;

let_end_138: ;
}
static void kill_letbind_19(void) {  
}

static uint64_t sailgen_getCapHardPerms(struct zCapability);

static uint64_t sailgen_getCapHardPerms(struct zCapability zcap)
{
  __label__ end_function_140, end_block_exception_141;

  uint64_t zcbz336;
  uint64_t zgaz3126;
  {
    bool zgaz394;
    zgaz394 = zcap.zpermit_set_CID;
    zgaz3126 = sailgen_bool_to_bits(zgaz394);
  
  }
  uint64_t zgaz3127;
  {
    uint64_t zgaz3124;
    {
      bool zgaz395;
      zgaz395 = zcap.zaccess_system_regs;
      zgaz3124 = sailgen_bool_to_bits(zgaz395);
    
    }
    uint64_t zgaz3125;
    {
      uint64_t zgaz3122;
      {
        bool zgaz396;
        zgaz396 = zcap.zpermit_unseal;
        zgaz3122 = sailgen_bool_to_bits(zgaz396);
      
      }
      uint64_t zgaz3123;
      {
        uint64_t zgaz3120;
        {
          bool zgaz397;
          zgaz397 = zcap.zpermit_ccall;
          zgaz3120 = sailgen_bool_to_bits(zgaz397);
        
        }
        uint64_t zgaz3121;
        {
          uint64_t zgaz3118;
          {
            bool zgaz398;
            zgaz398 = zcap.zpermit_seal;
            zgaz3118 = sailgen_bool_to_bits(zgaz398);
          
          }
          uint64_t zgaz3119;
          {
            uint64_t zgaz3116;
            {
              bool zgaz399;
              zgaz399 = zcap.zpermit_store_local_cap;
              zgaz3116 = sailgen_bool_to_bits(zgaz399);
            
            }
            uint64_t zgaz3117;
            {
              uint64_t zgaz3114;
              {
                bool zgaz3100;
                zgaz3100 = zcap.zpermit_store_cap;
                zgaz3114 = sailgen_bool_to_bits(zgaz3100);
              
              }
              uint64_t zgaz3115;
              {
                uint64_t zgaz3112;
                {
                  bool zgaz3101;
                  zgaz3101 = zcap.zpermit_load_cap;
                  zgaz3112 = sailgen_bool_to_bits(zgaz3101);
                
                }
                uint64_t zgaz3113;
                {
                  uint64_t zgaz3110;
                  {
                    bool zgaz3102;
                    zgaz3102 = zcap.zpermit_store;
                    zgaz3110 = sailgen_bool_to_bits(zgaz3102);
                  
                  }
                  uint64_t zgaz3111;
                  {
                    uint64_t zgaz3108;
                    {
                      bool zgaz3103;
                      zgaz3103 = zcap.zpermit_load;
                      zgaz3108 = sailgen_bool_to_bits(zgaz3103);
                    
                    }
                    uint64_t zgaz3109;
                    {
                      uint64_t zgaz3106;
                      {
                        bool zgaz3104;
                        zgaz3104 = zcap.zpermit_execute;
                        zgaz3106 = sailgen_bool_to_bits(zgaz3104);
                      
                      }
                      uint64_t zgaz3107;
                      {
                        bool zgaz3105;
                        zgaz3105 = zcap.zglobal;
                        zgaz3107 = sailgen_bool_to_bits(zgaz3105);
                      
                      }
                      {
                        lbits zgsz3206;
                        CREATE(lbits)(&zgsz3206);
                        CONVERT_OF(lbits, fbits)(&zgsz3206, zgaz3106, UINT64_C(1) , true);
                        lbits zgsz3207;
                        CREATE(lbits)(&zgsz3207);
                        CONVERT_OF(lbits, fbits)(&zgsz3207, zgaz3107, UINT64_C(1) , true);
                        lbits zgsz3208;
                        CREATE(lbits)(&zgsz3208);
                        append(&zgsz3208, zgsz3206, zgsz3207);
                        zgaz3109 = CONVERT_OF(fbits, lbits)(zgsz3208, true);
                        KILL(lbits)(&zgsz3208);
                        KILL(lbits)(&zgsz3207);
                        KILL(lbits)(&zgsz3206);
                      }
                    
                    
                    }
                    {
                      lbits zgsz3209;
                      CREATE(lbits)(&zgsz3209);
                      CONVERT_OF(lbits, fbits)(&zgsz3209, zgaz3108, UINT64_C(1) , true);
                      lbits zgsz3210;
                      CREATE(lbits)(&zgsz3210);
                      CONVERT_OF(lbits, fbits)(&zgsz3210, zgaz3109, UINT64_C(2) , true);
                      lbits zgsz3211;
                      CREATE(lbits)(&zgsz3211);
                      append(&zgsz3211, zgsz3209, zgsz3210);
                      zgaz3111 = CONVERT_OF(fbits, lbits)(zgsz3211, true);
                      KILL(lbits)(&zgsz3211);
                      KILL(lbits)(&zgsz3210);
                      KILL(lbits)(&zgsz3209);
                    }
                  
                  
                  }
                  {
                    lbits zgsz3212;
                    CREATE(lbits)(&zgsz3212);
                    CONVERT_OF(lbits, fbits)(&zgsz3212, zgaz3110, UINT64_C(1) , true);
                    lbits zgsz3213;
                    CREATE(lbits)(&zgsz3213);
                    CONVERT_OF(lbits, fbits)(&zgsz3213, zgaz3111, UINT64_C(3) , true);
                    lbits zgsz3214;
                    CREATE(lbits)(&zgsz3214);
                    append(&zgsz3214, zgsz3212, zgsz3213);
                    zgaz3113 = CONVERT_OF(fbits, lbits)(zgsz3214, true);
                    KILL(lbits)(&zgsz3214);
                    KILL(lbits)(&zgsz3213);
                    KILL(lbits)(&zgsz3212);
                  }
                
                
                }
                {
                  lbits zgsz3215;
                  CREATE(lbits)(&zgsz3215);
                  CONVERT_OF(lbits, fbits)(&zgsz3215, zgaz3112, UINT64_C(1) , true);
                  lbits zgsz3216;
                  CREATE(lbits)(&zgsz3216);
                  CONVERT_OF(lbits, fbits)(&zgsz3216, zgaz3113, UINT64_C(4) , true);
                  lbits zgsz3217;
                  CREATE(lbits)(&zgsz3217);
                  append(&zgsz3217, zgsz3215, zgsz3216);
                  zgaz3115 = CONVERT_OF(fbits, lbits)(zgsz3217, true);
                  KILL(lbits)(&zgsz3217);
                  KILL(lbits)(&zgsz3216);
                  KILL(lbits)(&zgsz3215);
                }
              
              
              }
              {
                lbits zgsz3218;
                CREATE(lbits)(&zgsz3218);
                CONVERT_OF(lbits, fbits)(&zgsz3218, zgaz3114, UINT64_C(1) , true);
                lbits zgsz3219;
                CREATE(lbits)(&zgsz3219);
                CONVERT_OF(lbits, fbits)(&zgsz3219, zgaz3115, UINT64_C(5) , true);
                lbits zgsz3220;
                CREATE(lbits)(&zgsz3220);
                append(&zgsz3220, zgsz3218, zgsz3219);
                zgaz3117 = CONVERT_OF(fbits, lbits)(zgsz3220, true);
                KILL(lbits)(&zgsz3220);
                KILL(lbits)(&zgsz3219);
                KILL(lbits)(&zgsz3218);
              }
            
            
            }
            {
              lbits zgsz3221;
              CREATE(lbits)(&zgsz3221);
              CONVERT_OF(lbits, fbits)(&zgsz3221, zgaz3116, UINT64_C(1) , true);
              lbits zgsz3222;
              CREATE(lbits)(&zgsz3222);
              CONVERT_OF(lbits, fbits)(&zgsz3222, zgaz3117, UINT64_C(6) , true);
              lbits zgsz3223;
              CREATE(lbits)(&zgsz3223);
              append(&zgsz3223, zgsz3221, zgsz3222);
              zgaz3119 = CONVERT_OF(fbits, lbits)(zgsz3223, true);
              KILL(lbits)(&zgsz3223);
              KILL(lbits)(&zgsz3222);
              KILL(lbits)(&zgsz3221);
            }
          
          
          }
          {
            lbits zgsz3224;
            CREATE(lbits)(&zgsz3224);
            CONVERT_OF(lbits, fbits)(&zgsz3224, zgaz3118, UINT64_C(1) , true);
            lbits zgsz3225;
            CREATE(lbits)(&zgsz3225);
            CONVERT_OF(lbits, fbits)(&zgsz3225, zgaz3119, UINT64_C(7) , true);
            lbits zgsz3226;
            CREATE(lbits)(&zgsz3226);
            append(&zgsz3226, zgsz3224, zgsz3225);
            zgaz3121 = CONVERT_OF(fbits, lbits)(zgsz3226, true);
            KILL(lbits)(&zgsz3226);
            KILL(lbits)(&zgsz3225);
            KILL(lbits)(&zgsz3224);
          }
        
        
        }
        {
          lbits zgsz3227;
          CREATE(lbits)(&zgsz3227);
          CONVERT_OF(lbits, fbits)(&zgsz3227, zgaz3120, UINT64_C(1) , true);
          lbits zgsz3228;
          CREATE(lbits)(&zgsz3228);
          CONVERT_OF(lbits, fbits)(&zgsz3228, zgaz3121, UINT64_C(8) , true);
          lbits zgsz3229;
          CREATE(lbits)(&zgsz3229);
          append(&zgsz3229, zgsz3227, zgsz3228);
          zgaz3123 = CONVERT_OF(fbits, lbits)(zgsz3229, true);
          KILL(lbits)(&zgsz3229);
          KILL(lbits)(&zgsz3228);
          KILL(lbits)(&zgsz3227);
        }
      
      
      }
      {
        lbits zgsz3230;
        CREATE(lbits)(&zgsz3230);
        CONVERT_OF(lbits, fbits)(&zgsz3230, zgaz3122, UINT64_C(1) , true);
        lbits zgsz3231;
        CREATE(lbits)(&zgsz3231);
        CONVERT_OF(lbits, fbits)(&zgsz3231, zgaz3123, UINT64_C(9) , true);
        lbits zgsz3232;
        CREATE(lbits)(&zgsz3232);
        append(&zgsz3232, zgsz3230, zgsz3231);
        zgaz3125 = CONVERT_OF(fbits, lbits)(zgsz3232, true);
        KILL(lbits)(&zgsz3232);
        KILL(lbits)(&zgsz3231);
        KILL(lbits)(&zgsz3230);
      }
    
    
    }
    {
      lbits zgsz3233;
      CREATE(lbits)(&zgsz3233);
      CONVERT_OF(lbits, fbits)(&zgsz3233, zgaz3124, UINT64_C(1) , true);
      lbits zgsz3234;
      CREATE(lbits)(&zgsz3234);
      CONVERT_OF(lbits, fbits)(&zgsz3234, zgaz3125, UINT64_C(10) , true);
      lbits zgsz3235;
      CREATE(lbits)(&zgsz3235);
      append(&zgsz3235, zgsz3233, zgsz3234);
      zgaz3127 = CONVERT_OF(fbits, lbits)(zgsz3235, true);
      KILL(lbits)(&zgsz3235);
      KILL(lbits)(&zgsz3234);
      KILL(lbits)(&zgsz3233);
    }
  
  
  }
  {
    lbits zgsz3236;
    CREATE(lbits)(&zgsz3236);
    CONVERT_OF(lbits, fbits)(&zgsz3236, zgaz3126, UINT64_C(1) , true);
    lbits zgsz3237;
    CREATE(lbits)(&zgsz3237);
    CONVERT_OF(lbits, fbits)(&zgsz3237, zgaz3127, UINT64_C(11) , true);
    lbits zgsz3238;
    CREATE(lbits)(&zgsz3238);
    append(&zgsz3238, zgsz3236, zgsz3237);
    zcbz336 = CONVERT_OF(fbits, lbits)(zgsz3238, true);
    KILL(lbits)(&zgsz3238);
    KILL(lbits)(&zgsz3237);
    KILL(lbits)(&zgsz3236);
  }


end_function_140: ;
  return zcbz336;
end_block_exception_141: ;

  return UINT64_C(0xdeadc0de);
}

static void sailgen_capToBits(lbits *rop, struct zCapability);

static void sailgen_capToBits(lbits *zcbz337, struct zCapability zcap)
{
  __label__ cleanup_144, end_cleanup_145, end_function_143, end_block_exception_146, end_function_211;

  uint64_t zt_hi;
  {
    uint64_t zgaz3154;
    zgaz3154 = zcap.zT;
    int64_t zgaz3155;
    {
      sail_int zgsz3239;
      CREATE(sail_int)(&zgsz3239);
      CONVERT_OF(sail_int, mach_int)(&zgsz3239, zmantissa_width);
      sail_int zgsz3240;
      CREATE(sail_int)(&zgsz3240);
      CONVERT_OF(sail_int, mach_int)(&zgsz3240, INT64_C(3));
      sail_int zgsz3241;
      CREATE(sail_int)(&zgsz3241);
      sub_int(&zgsz3241, zgsz3239, zgsz3240);
      zgaz3155 = CONVERT_OF(mach_int, sail_int)(zgsz3241);
      KILL(sail_int)(&zgsz3241);
      KILL(sail_int)(&zgsz3240);
      KILL(sail_int)(&zgsz3239);
    }
    {
      lbits zgsz3242;
      CREATE(lbits)(&zgsz3242);
      CONVERT_OF(lbits, fbits)(&zgsz3242, zgaz3154, UINT64_C(14) , true);
      sail_int zgsz3243;
      CREATE(sail_int)(&zgsz3243);
      CONVERT_OF(sail_int, mach_int)(&zgsz3243, zgaz3155);
      sail_int zgsz3244;
      CREATE(sail_int)(&zgsz3244);
      CONVERT_OF(sail_int, mach_int)(&zgsz3244, INT64_C(3));
      lbits zgsz3245;
      CREATE(lbits)(&zgsz3245);
      vector_subrange_lbits(&zgsz3245, zgsz3242, zgsz3243, zgsz3244);
      zt_hi = CONVERT_OF(fbits, lbits)(zgsz3245, true);
      KILL(lbits)(&zgsz3245);
      KILL(sail_int)(&zgsz3244);
      KILL(sail_int)(&zgsz3243);
      KILL(lbits)(&zgsz3242);
    }
  
  
  }
  uint64_t zt_lo;
  {
    uint64_t zgaz3153;
    zgaz3153 = zcap.zT;
    {
      lbits zgsz3246;
      CREATE(lbits)(&zgsz3246);
      CONVERT_OF(lbits, fbits)(&zgsz3246, zgaz3153, UINT64_C(14) , true);
      sail_int zgsz3247;
      CREATE(sail_int)(&zgsz3247);
      CONVERT_OF(sail_int, mach_int)(&zgsz3247, INT64_C(2));
      sail_int zgsz3248;
      CREATE(sail_int)(&zgsz3248);
      CONVERT_OF(sail_int, mach_int)(&zgsz3248, INT64_C(0));
      lbits zgsz3249;
      CREATE(lbits)(&zgsz3249);
      vector_subrange_lbits(&zgsz3249, zgsz3246, zgsz3247, zgsz3248);
      zt_lo = CONVERT_OF(fbits, lbits)(zgsz3249, true);
      KILL(lbits)(&zgsz3249);
      KILL(sail_int)(&zgsz3248);
      KILL(sail_int)(&zgsz3247);
      KILL(lbits)(&zgsz3246);
    }
  
  }
  uint64_t zb_hi;
  {
    uint64_t zgaz3151;
    zgaz3151 = zcap.zB;
    int64_t zgaz3152;
    {
      sail_int zgsz3250;
      CREATE(sail_int)(&zgsz3250);
      CONVERT_OF(sail_int, mach_int)(&zgsz3250, zmantissa_width);
      sail_int zgsz3251;
      CREATE(sail_int)(&zgsz3251);
      CONVERT_OF(sail_int, mach_int)(&zgsz3251, INT64_C(1));
      sail_int zgsz3252;
      CREATE(sail_int)(&zgsz3252);
      sub_int(&zgsz3252, zgsz3250, zgsz3251);
      zgaz3152 = CONVERT_OF(mach_int, sail_int)(zgsz3252);
      KILL(sail_int)(&zgsz3252);
      KILL(sail_int)(&zgsz3251);
      KILL(sail_int)(&zgsz3250);
    }
    {
      lbits zgsz3253;
      CREATE(lbits)(&zgsz3253);
      CONVERT_OF(lbits, fbits)(&zgsz3253, zgaz3151, UINT64_C(14) , true);
      sail_int zgsz3254;
      CREATE(sail_int)(&zgsz3254);
      CONVERT_OF(sail_int, mach_int)(&zgsz3254, zgaz3152);
      sail_int zgsz3255;
      CREATE(sail_int)(&zgsz3255);
      CONVERT_OF(sail_int, mach_int)(&zgsz3255, INT64_C(3));
      lbits zgsz3256;
      CREATE(lbits)(&zgsz3256);
      vector_subrange_lbits(&zgsz3256, zgsz3253, zgsz3254, zgsz3255);
      zb_hi = CONVERT_OF(fbits, lbits)(zgsz3256, true);
      KILL(lbits)(&zgsz3256);
      KILL(sail_int)(&zgsz3255);
      KILL(sail_int)(&zgsz3254);
      KILL(lbits)(&zgsz3253);
    }
  
  
  }
  uint64_t zb_lo;
  {
    uint64_t zgaz3150;
    zgaz3150 = zcap.zB;
    {
      lbits zgsz3257;
      CREATE(lbits)(&zgsz3257);
      CONVERT_OF(lbits, fbits)(&zgsz3257, zgaz3150, UINT64_C(14) , true);
      sail_int zgsz3258;
      CREATE(sail_int)(&zgsz3258);
      CONVERT_OF(sail_int, mach_int)(&zgsz3258, INT64_C(2));
      sail_int zgsz3259;
      CREATE(sail_int)(&zgsz3259);
      CONVERT_OF(sail_int, mach_int)(&zgsz3259, INT64_C(0));
      lbits zgsz3260;
      CREATE(lbits)(&zgsz3260);
      vector_subrange_lbits(&zgsz3260, zgsz3257, zgsz3258, zgsz3259);
      zb_lo = CONVERT_OF(fbits, lbits)(zgsz3260, true);
      KILL(lbits)(&zgsz3260);
      KILL(sail_int)(&zgsz3259);
      KILL(sail_int)(&zgsz3258);
      KILL(lbits)(&zgsz3257);
    }
  
  }
  {
    bool zgaz3128;
    zgaz3128 = zcap.zinternal_e;
    unit zgsz3261;
    if (zgaz3128) {
    {
      uint64_t zgaz3129;
      zgaz3129 = zcap.zE;
      {
        lbits zgsz3262;
        CREATE(lbits)(&zgsz3262);
        CONVERT_OF(lbits, fbits)(&zgsz3262, zgaz3129, UINT64_C(6) , true);
        sail_int zgsz3263;
        CREATE(sail_int)(&zgsz3263);
        CONVERT_OF(sail_int, mach_int)(&zgsz3263, INT64_C(5));
        sail_int zgsz3264;
        CREATE(sail_int)(&zgsz3264);
        CONVERT_OF(sail_int, mach_int)(&zgsz3264, INT64_C(3));
        lbits zgsz3265;
        CREATE(lbits)(&zgsz3265);
        vector_subrange_lbits(&zgsz3265, zgsz3262, zgsz3263, zgsz3264);
        zt_lo = CONVERT_OF(fbits, lbits)(zgsz3265, true);
        KILL(lbits)(&zgsz3265);
        KILL(sail_int)(&zgsz3264);
        KILL(sail_int)(&zgsz3263);
        KILL(lbits)(&zgsz3262);
      }
      unit zgsz3266;
      zgsz3266 = UNIT;
    
    }
    uint64_t zgaz3130;
    zgaz3130 = zcap.zE;
    {
      lbits zgsz3267;
      CREATE(lbits)(&zgsz3267);
      CONVERT_OF(lbits, fbits)(&zgsz3267, zgaz3130, UINT64_C(6) , true);
      sail_int zgsz3268;
      CREATE(sail_int)(&zgsz3268);
      CONVERT_OF(sail_int, mach_int)(&zgsz3268, INT64_C(2));
      sail_int zgsz3269;
      CREATE(sail_int)(&zgsz3269);
      CONVERT_OF(sail_int, mach_int)(&zgsz3269, INT64_C(0));
      lbits zgsz3270;
      CREATE(lbits)(&zgsz3270);
      vector_subrange_lbits(&zgsz3270, zgsz3267, zgsz3268, zgsz3269);
      zb_lo = CONVERT_OF(fbits, lbits)(zgsz3270, true);
      KILL(lbits)(&zgsz3270);
      KILL(sail_int)(&zgsz3269);
      KILL(sail_int)(&zgsz3268);
      KILL(lbits)(&zgsz3267);
    }
    zgsz3261 = UNIT;
  
    } else {  zgsz3261 = UNIT;  }
  
  }
  lbits zgaz3149;
  CREATE(lbits)(&zgaz3149);
  {
    uint64_t zgaz3147;
    zgaz3147 = zcap.zuperms;
    lbits zgaz3148;
    CREATE(lbits)(&zgaz3148);
    {
      uint64_t zgaz3145;
      zgaz3145 = sailgen_getCapHardPerms(zcap);
      lbits zgaz3146;
      CREATE(lbits)(&zgaz3146);
      {
        uint64_t zgaz3143;
        zgaz3143 = zcap.zreserved;
        lbits zgaz3144;
        CREATE(lbits)(&zgaz3144);
        {
          uint64_t zgaz3141;
          {
            bool zgaz3131;
            zgaz3131 = zcap.zflag_cap_mode;
            zgaz3141 = sailgen_bool_to_bits(zgaz3131);
          
          }
          lbits zgaz3142;
          CREATE(lbits)(&zgaz3142);
          {
            uint64_t zgaz3139;
            zgaz3139 = zcap.zotype;
            lbits zgaz3140;
            CREATE(lbits)(&zgaz3140);
            {
              uint64_t zgaz3137;
              {
                bool zgaz3132;
                zgaz3132 = zcap.zinternal_e;
                zgaz3137 = sailgen_bool_to_bits(zgaz3132);
              
              }
              lbits zgaz3138;
              CREATE(lbits)(&zgaz3138);
              {
                lbits zgaz3136;
                CREATE(lbits)(&zgaz3136);
                {
                  lbits zgaz3135;
                  CREATE(lbits)(&zgaz3135);
                  {
                    lbits zgaz3134;
                    CREATE(lbits)(&zgaz3134);
                    {
                      uint64_t zgaz3133;
                      zgaz3133 = zcap.zaddress;
                      {
                        lbits zgsz3272;
                        CREATE(lbits)(&zgsz3272);
                        CONVERT_OF(lbits, fbits)(&zgsz3272, zgaz3133, UINT64_C(64) , true);
                        lbits zgsz3271;
                        CREATE(lbits)(&zgsz3271);
                        CONVERT_OF(lbits, fbits)(&zgsz3271, zb_lo, UINT64_C(3) , true);
                        append(&zgaz3134, zgsz3271, zgsz3272);
                        KILL(lbits)(&zgsz3272);
                        KILL(lbits)(&zgsz3271);
                      }
                    
                    }
                    {
                      lbits zgsz3273;
                      CREATE(lbits)(&zgsz3273);
                      CONVERT_OF(lbits, fbits)(&zgsz3273, zb_hi, UINT64_C(11) , true);
                      append(&zgaz3135, zgsz3273, zgaz3134);
                      KILL(lbits)(&zgsz3273);
                    }
                    KILL(lbits)(&zgaz3134);
                  }
                  {
                    lbits zgsz3274;
                    CREATE(lbits)(&zgsz3274);
                    CONVERT_OF(lbits, fbits)(&zgsz3274, zt_lo, UINT64_C(3) , true);
                    append(&zgaz3136, zgsz3274, zgaz3135);
                    KILL(lbits)(&zgsz3274);
                  }
                  KILL(lbits)(&zgaz3135);
                }
                {
                  lbits zgsz3275;
                  CREATE(lbits)(&zgsz3275);
                  CONVERT_OF(lbits, fbits)(&zgsz3275, zt_hi, UINT64_C(9) , true);
                  append(&zgaz3138, zgsz3275, zgaz3136);
                  KILL(lbits)(&zgsz3275);
                }
                KILL(lbits)(&zgaz3136);
              }
              {
                lbits zgsz3276;
                CREATE(lbits)(&zgsz3276);
                CONVERT_OF(lbits, fbits)(&zgsz3276, zgaz3137, UINT64_C(1) , true);
                append(&zgaz3140, zgsz3276, zgaz3138);
                KILL(lbits)(&zgsz3276);
              }
              KILL(lbits)(&zgaz3138);
            
            }
            {
              lbits zgsz3277;
              CREATE(lbits)(&zgsz3277);
              CONVERT_OF(lbits, fbits)(&zgsz3277, zgaz3139, UINT64_C(18) , true);
              append(&zgaz3142, zgsz3277, zgaz3140);
              KILL(lbits)(&zgsz3277);
            }
            KILL(lbits)(&zgaz3140);
          
          }
          {
            lbits zgsz3278;
            CREATE(lbits)(&zgsz3278);
            CONVERT_OF(lbits, fbits)(&zgsz3278, zgaz3141, UINT64_C(1) , true);
            append(&zgaz3144, zgsz3278, zgaz3142);
            KILL(lbits)(&zgsz3278);
          }
          KILL(lbits)(&zgaz3142);
        
        }
        {
          lbits zgsz3279;
          CREATE(lbits)(&zgsz3279);
          CONVERT_OF(lbits, fbits)(&zgsz3279, zgaz3143, UINT64_C(2) , true);
          append(&zgaz3146, zgsz3279, zgaz3144);
          KILL(lbits)(&zgsz3279);
        }
        KILL(lbits)(&zgaz3144);
      
      }
      {
        lbits zgsz3280;
        CREATE(lbits)(&zgsz3280);
        CONVERT_OF(lbits, fbits)(&zgsz3280, zgaz3145, UINT64_C(12) , true);
        append(&zgaz3148, zgsz3280, zgaz3146);
        KILL(lbits)(&zgsz3280);
      }
      KILL(lbits)(&zgaz3146);
    
    }
    {
      lbits zgsz3281;
      CREATE(lbits)(&zgsz3281);
      CONVERT_OF(lbits, fbits)(&zgsz3281, zgaz3147, UINT64_C(4) , true);
      append(&zgaz3149, zgsz3281, zgaz3148);
      KILL(lbits)(&zgsz3281);
    }
    KILL(lbits)(&zgaz3148);
  
  }
  COPY(lbits)((*(&zcbz337)), zgaz3149);
  goto cleanup_144;
  /* unreachable after return */
  KILL(lbits)(&zgaz3149);




  goto end_cleanup_145;
cleanup_144: ;




  KILL(lbits)(&zgaz3149);
  goto end_function_143;
end_cleanup_145: ;
end_function_143: ;
  goto end_function_211;
end_block_exception_146: ;
  goto end_function_211;
end_function_211: ;
}

static lbits znull_cap_bits;
static void create_letbind_20(void) {    CREATE(lbits)(&znull_cap_bits);


  lbits zgsz3282;
  CREATE(lbits)(&zgsz3282);
  sailgen_capToBits(&zgsz3282, znull_cap);
  COPY(lbits)(&znull_cap_bits, zgsz3282);
  KILL(lbits)(&zgsz3282);
let_end_147: ;
}
static void kill_letbind_20(void) {    KILL(lbits)(&znull_cap_bits);
}

static void sailgen_capToMemBits(lbits *rop, struct zCapability);

static void sailgen_capToMemBits(lbits *zcbz338, struct zCapability zcap)
{
  __label__ end_function_149, end_block_exception_150, end_function_210;

  lbits zgaz3156;
  CREATE(lbits)(&zgaz3156);
  sailgen_capToBits(&zgaz3156, zcap);
  xor_bits((*(&zcbz338)), zgaz3156, znull_cap_bits);
  KILL(lbits)(&zgaz3156);
end_function_149: ;
  goto end_function_210;
end_block_exception_150: ;
  goto end_function_210;
end_function_210: ;
}

static struct zCapability sailgen_memBitsToCapability(bool, lbits);

static struct zCapability sailgen_memBitsToCapability(bool ztag, lbits zb)
{
  __label__ end_function_152, end_block_exception_153;

  struct zCapability zcbz339;
  lbits zgaz3157;
  CREATE(lbits)(&zgaz3157);
  xor_bits(&zgaz3157, zb, znull_cap_bits);
  zcbz339 = sailgen_capBitsToCapability(ztag, zgaz3157);
  KILL(lbits)(&zgaz3157);
end_function_152: ;
  return zcbz339;
end_block_exception_153: ;
  struct zCapability zcbz356 = { .zuperms = UINT64_C(0xdeadc0de), .ztag = false, .zsealed = false, .zreserved = UINT64_C(0xdeadc0de), .zpermit_unseal = false, .zpermit_store_local_cap = false, .zpermit_store_cap = false, .zpermit_store = false, .zpermit_set_CID = false, .zpermit_seal = false, .zpermit_load_cap = false, .zpermit_load = false, .zpermit_execute = false, .zpermit_ccall = false, .zotype = UINT64_C(0xdeadc0de), .zinternal_e = false, .zglobal = false, .zflag_cap_mode = false, .zaddress = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zT = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  return zcbz356;
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

static void CREATE(ztuple_z8z5bv64zCz0z5bvz9)(struct ztuple_z8z5bv64zCz0z5bvz9 *op) {CREATE(lbits)(&op->ztup1);}

static void RECREATE(ztuple_z8z5bv64zCz0z5bvz9)(struct ztuple_z8z5bv64zCz0z5bvz9 *op) {RECREATE(lbits)(&op->ztup1);}

static void KILL(ztuple_z8z5bv64zCz0z5bvz9)(struct ztuple_z8z5bv64zCz0z5bvz9 *op) {KILL(lbits)(&op->ztup1);}

static bool EQUAL(ztuple_z8z5bv64zCz0z5bvz9)(struct ztuple_z8z5bv64zCz0z5bvz9 op1, struct ztuple_z8z5bv64zCz0z5bvz9 op2) {
  return EQUAL(fbits)(op1.ztup0, op2.ztup0) && EQUAL(lbits)(op1.ztup1, op2.ztup1);
}

static void sailgen_getCapBoundsBits(struct ztuple_z8z5bv64zCz0z5bvz9 *rop, struct zCapability);

static void sailgen_getCapBoundsBits(struct ztuple_z8z5bv64zCz0z5bvz9 *zcbz340, struct zCapability zc)
{
  __label__ end_function_155, end_block_exception_156, end_function_209;

  int64_t zE;
  {
    int64_t zgaz3191;
    {
      uint64_t zgaz3190;
      zgaz3190 = zc.zE;
      zgaz3191 = ((mach_int) zgaz3190);
    
    }
    {
      sail_int zgsz3283;
      CREATE(sail_int)(&zgsz3283);
      CONVERT_OF(sail_int, mach_int)(&zgsz3283, zmaxE);
      sail_int zgsz3284;
      CREATE(sail_int)(&zgsz3284);
      CONVERT_OF(sail_int, mach_int)(&zgsz3284, zgaz3191);
      sail_int zgsz3285;
      CREATE(sail_int)(&zgsz3285);
      min_int(&zgsz3285, zgsz3283, zgsz3284);
      zE = CONVERT_OF(mach_int, sail_int)(zgsz3285);
      KILL(sail_int)(&zgsz3285);
      KILL(sail_int)(&zgsz3284);
      KILL(sail_int)(&zgsz3283);
    }
  
  }
  uint64_t za;
  za = zc.zaddress;
  uint64_t za3;
  {
    uint64_t zgaz3189;
    {
      int64_t zgaz3188;
      {
        int64_t zgaz3187;
        {
          sail_int zgsz3286;
          CREATE(sail_int)(&zgsz3286);
          CONVERT_OF(sail_int, mach_int)(&zgsz3286, zE);
          sail_int zgsz3287;
          CREATE(sail_int)(&zgsz3287);
          CONVERT_OF(sail_int, mach_int)(&zgsz3287, zmantissa_width);
          sail_int zgsz3288;
          CREATE(sail_int)(&zgsz3288);
          add_int(&zgsz3288, zgsz3286, zgsz3287);
          zgaz3187 = CONVERT_OF(mach_int, sail_int)(zgsz3288);
          KILL(sail_int)(&zgsz3288);
          KILL(sail_int)(&zgsz3287);
          KILL(sail_int)(&zgsz3286);
        }
        {
          sail_int zgsz3289;
          CREATE(sail_int)(&zgsz3289);
          CONVERT_OF(sail_int, mach_int)(&zgsz3289, zgaz3187);
          sail_int zgsz3290;
          CREATE(sail_int)(&zgsz3290);
          CONVERT_OF(sail_int, mach_int)(&zgsz3290, INT64_C(3));
          sail_int zgsz3291;
          CREATE(sail_int)(&zgsz3291);
          sub_int(&zgsz3291, zgsz3289, zgsz3290);
          zgaz3188 = CONVERT_OF(mach_int, sail_int)(zgsz3291);
          KILL(sail_int)(&zgsz3291);
          KILL(sail_int)(&zgsz3290);
          KILL(sail_int)(&zgsz3289);
        }
      
      }
      {
        lbits zgsz3292;
        CREATE(lbits)(&zgsz3292);
        CONVERT_OF(lbits, fbits)(&zgsz3292, za, UINT64_C(64) , true);
        sail_int zgsz3293;
        CREATE(sail_int)(&zgsz3293);
        CONVERT_OF(sail_int, mach_int)(&zgsz3293, zgaz3188);
        lbits zgsz3294;
        CREATE(lbits)(&zgsz3294);
        shiftr(&zgsz3294, zgsz3292, zgsz3293);
        zgaz3189 = CONVERT_OF(fbits, lbits)(zgsz3294, true);
        KILL(lbits)(&zgsz3294);
        KILL(sail_int)(&zgsz3293);
        KILL(lbits)(&zgsz3292);
      }
    
    }
    {
      lbits zgsz3295;
      CREATE(lbits)(&zgsz3295);
      CONVERT_OF(lbits, fbits)(&zgsz3295, zgaz3189, UINT64_C(64) , true);
      sail_int zgsz3296;
      CREATE(sail_int)(&zgsz3296);
      CONVERT_OF(sail_int, mach_int)(&zgsz3296, INT64_C(3));
      lbits zgsz3297;
      CREATE(lbits)(&zgsz3297);
      sail_truncate(&zgsz3297, zgsz3295, zgsz3296);
      za3 = CONVERT_OF(fbits, lbits)(zgsz3297, true);
      KILL(lbits)(&zgsz3297);
      KILL(sail_int)(&zgsz3296);
      KILL(lbits)(&zgsz3295);
    }
  
  }
  uint64_t zB3;
  {
    uint64_t zgaz3186;
    zgaz3186 = zc.zB;
    {
      lbits zgsz3298;
      CREATE(lbits)(&zgsz3298);
      CONVERT_OF(lbits, fbits)(&zgsz3298, zgaz3186, UINT64_C(14) , true);
      sail_int zgsz3299;
      CREATE(sail_int)(&zgsz3299);
      CONVERT_OF(sail_int, mach_int)(&zgsz3299, INT64_C(3));
      lbits zgsz3300;
      CREATE(lbits)(&zgsz3300);
      sail_truncateLSB(&zgsz3300, zgsz3298, zgsz3299);
      zB3 = CONVERT_OF(fbits, lbits)(zgsz3300, true);
      KILL(lbits)(&zgsz3300);
      KILL(sail_int)(&zgsz3299);
      KILL(lbits)(&zgsz3298);
    }
  
  }
  uint64_t zT3;
  {
    uint64_t zgaz3185;
    zgaz3185 = zc.zT;
    {
      lbits zgsz3301;
      CREATE(lbits)(&zgsz3301);
      CONVERT_OF(lbits, fbits)(&zgsz3301, zgaz3185, UINT64_C(14) , true);
      sail_int zgsz3302;
      CREATE(sail_int)(&zgsz3302);
      CONVERT_OF(sail_int, mach_int)(&zgsz3302, INT64_C(3));
      lbits zgsz3303;
      CREATE(lbits)(&zgsz3303);
      sail_truncateLSB(&zgsz3303, zgsz3301, zgsz3302);
      zT3 = CONVERT_OF(fbits, lbits)(zgsz3303, true);
      KILL(lbits)(&zgsz3303);
      KILL(sail_int)(&zgsz3302);
      KILL(lbits)(&zgsz3301);
    }
  
  }
  uint64_t zR3;
  {
    lbits zgsz3304;
    CREATE(lbits)(&zgsz3304);
    CONVERT_OF(lbits, fbits)(&zgsz3304, zB3, UINT64_C(3) , true);
    lbits zgsz3305;
    CREATE(lbits)(&zgsz3305);
    CONVERT_OF(lbits, fbits)(&zgsz3305, UINT64_C(0b001), UINT64_C(3) , true);
    lbits zgsz3306;
    CREATE(lbits)(&zgsz3306);
    sub_bits(&zgsz3306, zgsz3304, zgsz3305);
    zR3 = CONVERT_OF(fbits, lbits)(zgsz3306, true);
    KILL(lbits)(&zgsz3306);
    KILL(lbits)(&zgsz3305);
    KILL(lbits)(&zgsz3304);
  }
  int64_t zaHi;
  {
    bool zgaz3184;
    zgaz3184 = sailgen_z8operatorz0zzzz7nzzJzzK3z3zI_uz9(za3, zR3);
    if (zgaz3184) {  zaHi = INT64_C(1);  } else {  zaHi = INT64_C(0);  }
  
  }
  int64_t zbHi;
  {
    bool zgaz3183;
    zgaz3183 = sailgen_z8operatorz0zzzz7nzzJzzK3z3zI_uz9(zB3, zR3);
    if (zgaz3183) {  zbHi = INT64_C(1);  } else {  zbHi = INT64_C(0);  }
  
  }
  int64_t ztHi;
  {
    bool zgaz3182;
    zgaz3182 = sailgen_z8operatorz0zzzz7nzzJzzK3z3zI_uz9(zT3, zR3);
    if (zgaz3182) {  ztHi = INT64_C(1);  } else {  ztHi = INT64_C(0);  }
  
  }
  int64_t zcorrection_base;
  {
    sail_int zgsz3307;
    CREATE(sail_int)(&zgsz3307);
    CONVERT_OF(sail_int, mach_int)(&zgsz3307, zbHi);
    sail_int zgsz3308;
    CREATE(sail_int)(&zgsz3308);
    CONVERT_OF(sail_int, mach_int)(&zgsz3308, zaHi);
    sail_int zgsz3309;
    CREATE(sail_int)(&zgsz3309);
    sub_int(&zgsz3309, zgsz3307, zgsz3308);
    zcorrection_base = CONVERT_OF(mach_int, sail_int)(zgsz3309);
    KILL(sail_int)(&zgsz3309);
    KILL(sail_int)(&zgsz3308);
    KILL(sail_int)(&zgsz3307);
  }
  int64_t zcorrection_top;
  {
    sail_int zgsz3310;
    CREATE(sail_int)(&zgsz3310);
    CONVERT_OF(sail_int, mach_int)(&zgsz3310, ztHi);
    sail_int zgsz3311;
    CREATE(sail_int)(&zgsz3311);
    CONVERT_OF(sail_int, mach_int)(&zgsz3311, zaHi);
    sail_int zgsz3312;
    CREATE(sail_int)(&zgsz3312);
    sub_int(&zgsz3312, zgsz3310, zgsz3311);
    zcorrection_top = CONVERT_OF(mach_int, sail_int)(zgsz3312);
    KILL(sail_int)(&zgsz3312);
    KILL(sail_int)(&zgsz3311);
    KILL(sail_int)(&zgsz3310);
  }
  uint64_t za_top;
  {
    int64_t zgaz3181;
    {
      sail_int zgsz3313;
      CREATE(sail_int)(&zgsz3313);
      CONVERT_OF(sail_int, mach_int)(&zgsz3313, zE);
      sail_int zgsz3314;
      CREATE(sail_int)(&zgsz3314);
      CONVERT_OF(sail_int, mach_int)(&zgsz3314, zmantissa_width);
      sail_int zgsz3315;
      CREATE(sail_int)(&zgsz3315);
      add_int(&zgsz3315, zgsz3313, zgsz3314);
      zgaz3181 = CONVERT_OF(mach_int, sail_int)(zgsz3315);
      KILL(sail_int)(&zgsz3315);
      KILL(sail_int)(&zgsz3314);
      KILL(sail_int)(&zgsz3313);
    }
    {
      lbits zgsz3316;
      CREATE(lbits)(&zgsz3316);
      CONVERT_OF(lbits, fbits)(&zgsz3316, za, UINT64_C(64) , true);
      sail_int zgsz3317;
      CREATE(sail_int)(&zgsz3317);
      CONVERT_OF(sail_int, mach_int)(&zgsz3317, zgaz3181);
      lbits zgsz3318;
      CREATE(lbits)(&zgsz3318);
      shiftr(&zgsz3318, zgsz3316, zgsz3317);
      za_top = CONVERT_OF(fbits, lbits)(zgsz3318, true);
      KILL(lbits)(&zgsz3318);
      KILL(sail_int)(&zgsz3317);
      KILL(lbits)(&zgsz3316);
    }
  
  }
  lbits zbase;
  CREATE(lbits)(&zbase);
  {
    lbits zgaz3180;
    CREATE(lbits)(&zgaz3180);
    {
      uint64_t zgaz3178;
      {
        lbits zgsz3319;
        CREATE(lbits)(&zgsz3319);
        CONVERT_OF(lbits, fbits)(&zgsz3319, za_top, UINT64_C(64) , true);
        sail_int zgsz3320;
        CREATE(sail_int)(&zgsz3320);
        CONVERT_OF(sail_int, mach_int)(&zgsz3320, zcorrection_base);
        lbits zgsz3321;
        CREATE(lbits)(&zgsz3321);
        add_bits_int(&zgsz3321, zgsz3319, zgsz3320);
        zgaz3178 = CONVERT_OF(fbits, lbits)(zgsz3321, true);
        KILL(lbits)(&zgsz3321);
        KILL(sail_int)(&zgsz3320);
        KILL(lbits)(&zgsz3319);
      }
      lbits zgaz3179;
      CREATE(lbits)(&zgaz3179);
      {
        uint64_t zgaz3176;
        zgaz3176 = zc.zB;
        sbits zgaz3177;
        {
          sail_int zgsz3322;
          CREATE(sail_int)(&zgsz3322);
          CONVERT_OF(sail_int, mach_int)(&zgsz3322, zE);
          lbits zgsz3323;
          CREATE(lbits)(&zgsz3323);
          sailgen_zzz3zzz3zzeros_implicit(&zgsz3323, zgsz3322);
          zgaz3177 = CONVERT_OF(sbits, lbits)(zgsz3323, true);
          KILL(lbits)(&zgsz3323);
          KILL(sail_int)(&zgsz3322);
        }
        {
          lbits zgsz3325;
          CREATE(lbits)(&zgsz3325);
          CONVERT_OF(lbits, sbits)(&zgsz3325, zgaz3177, true);
          lbits zgsz3324;
          CREATE(lbits)(&zgsz3324);
          CONVERT_OF(lbits, fbits)(&zgsz3324, zgaz3176, UINT64_C(14) , true);
          append(&zgaz3179, zgsz3324, zgsz3325);
          KILL(lbits)(&zgsz3325);
          KILL(lbits)(&zgsz3324);
        }
      
      
      }
      {
        lbits zgsz3326;
        CREATE(lbits)(&zgsz3326);
        CONVERT_OF(lbits, fbits)(&zgsz3326, zgaz3178, UINT64_C(64) , true);
        append(&zgaz3180, zgsz3326, zgaz3179);
        KILL(lbits)(&zgsz3326);
      }
      KILL(lbits)(&zgaz3179);
    
    }
    {
      sail_int zgsz3327;
      CREATE(sail_int)(&zgsz3327);
      CONVERT_OF(sail_int, mach_int)(&zgsz3327, zcap_len_width);
      sail_truncate(&zbase, zgaz3180, zgsz3327);
      KILL(sail_int)(&zgsz3327);
    }
    KILL(lbits)(&zgaz3180);
  }
  lbits ztop;
  CREATE(lbits)(&ztop);
  {
    lbits zgaz3175;
    CREATE(lbits)(&zgaz3175);
    {
      uint64_t zgaz3173;
      {
        lbits zgsz3328;
        CREATE(lbits)(&zgsz3328);
        CONVERT_OF(lbits, fbits)(&zgsz3328, za_top, UINT64_C(64) , true);
        sail_int zgsz3329;
        CREATE(sail_int)(&zgsz3329);
        CONVERT_OF(sail_int, mach_int)(&zgsz3329, zcorrection_top);
        lbits zgsz3330;
        CREATE(lbits)(&zgsz3330);
        add_bits_int(&zgsz3330, zgsz3328, zgsz3329);
        zgaz3173 = CONVERT_OF(fbits, lbits)(zgsz3330, true);
        KILL(lbits)(&zgsz3330);
        KILL(sail_int)(&zgsz3329);
        KILL(lbits)(&zgsz3328);
      }
      lbits zgaz3174;
      CREATE(lbits)(&zgaz3174);
      {
        uint64_t zgaz3171;
        zgaz3171 = zc.zT;
        sbits zgaz3172;
        {
          sail_int zgsz3331;
          CREATE(sail_int)(&zgsz3331);
          CONVERT_OF(sail_int, mach_int)(&zgsz3331, zE);
          lbits zgsz3332;
          CREATE(lbits)(&zgsz3332);
          sailgen_zzz3zzz3zzeros_implicit(&zgsz3332, zgsz3331);
          zgaz3172 = CONVERT_OF(sbits, lbits)(zgsz3332, true);
          KILL(lbits)(&zgsz3332);
          KILL(sail_int)(&zgsz3331);
        }
        {
          lbits zgsz3334;
          CREATE(lbits)(&zgsz3334);
          CONVERT_OF(lbits, sbits)(&zgsz3334, zgaz3172, true);
          lbits zgsz3333;
          CREATE(lbits)(&zgsz3333);
          CONVERT_OF(lbits, fbits)(&zgsz3333, zgaz3171, UINT64_C(14) , true);
          append(&zgaz3174, zgsz3333, zgsz3334);
          KILL(lbits)(&zgsz3334);
          KILL(lbits)(&zgsz3333);
        }
      
      
      }
      {
        lbits zgsz3335;
        CREATE(lbits)(&zgsz3335);
        CONVERT_OF(lbits, fbits)(&zgsz3335, zgaz3173, UINT64_C(64) , true);
        append(&zgaz3175, zgsz3335, zgaz3174);
        KILL(lbits)(&zgsz3335);
      }
      KILL(lbits)(&zgaz3174);
    
    }
    {
      sail_int zgsz3336;
      CREATE(sail_int)(&zgsz3336);
      CONVERT_OF(sail_int, mach_int)(&zgsz3336, zcap_len_width);
      sail_truncate(&ztop, zgaz3175, zgsz3336);
      KILL(sail_int)(&zgsz3336);
    }
    KILL(lbits)(&zgaz3175);
  }
  uint64_t zbase2;
  {
    uint64_t zgaz3170;
    {
      fbits zgaz3169;
      {
        int64_t zgaz3168;
        {
          sail_int zgsz3337;
          CREATE(sail_int)(&zgsz3337);
          CONVERT_OF(sail_int, mach_int)(&zgsz3337, zcap_addr_width);
          sail_int zgsz3338;
          CREATE(sail_int)(&zgsz3338);
          CONVERT_OF(sail_int, mach_int)(&zgsz3338, INT64_C(1));
          sail_int zgsz3339;
          CREATE(sail_int)(&zgsz3339);
          sub_int(&zgsz3339, zgsz3337, zgsz3338);
          zgaz3168 = CONVERT_OF(mach_int, sail_int)(zgsz3339);
          KILL(sail_int)(&zgsz3339);
          KILL(sail_int)(&zgsz3338);
          KILL(sail_int)(&zgsz3337);
        }
        {
          sail_int zgsz3340;
          CREATE(sail_int)(&zgsz3340);
          CONVERT_OF(sail_int, mach_int)(&zgsz3340, zgaz3168);
          zgaz3169 = bitvector_access(zbase, zgsz3340);
          KILL(sail_int)(&zgsz3340);
        }
      
      }
      uint64_t zgsz3341;
      zgsz3341 = UINT64_C(0b0);
      zgsz3341 = update_fbits(zgsz3341, INT64_C(0), zgaz3169);
      zgaz3170 = zgsz3341;
    
    }
    {
      lbits zgsz3342;
      CREATE(lbits)(&zgsz3342);
      CONVERT_OF(lbits, fbits)(&zgsz3342, UINT64_C(0b0), UINT64_C(1) , true);
      lbits zgsz3343;
      CREATE(lbits)(&zgsz3343);
      CONVERT_OF(lbits, fbits)(&zgsz3343, zgaz3170, UINT64_C(1) , true);
      lbits zgsz3344;
      CREATE(lbits)(&zgsz3344);
      append(&zgsz3344, zgsz3342, zgsz3343);
      zbase2 = CONVERT_OF(fbits, lbits)(zgsz3344, true);
      KILL(lbits)(&zgsz3344);
      KILL(lbits)(&zgsz3343);
      KILL(lbits)(&zgsz3342);
    }
  
  }
  uint64_t ztop2;
  {
    int64_t zgaz3167;
    {
      sail_int zgsz3345;
      CREATE(sail_int)(&zgsz3345);
      CONVERT_OF(sail_int, mach_int)(&zgsz3345, zcap_addr_width);
      sail_int zgsz3346;
      CREATE(sail_int)(&zgsz3346);
      CONVERT_OF(sail_int, mach_int)(&zgsz3346, INT64_C(1));
      sail_int zgsz3347;
      CREATE(sail_int)(&zgsz3347);
      sub_int(&zgsz3347, zgsz3345, zgsz3346);
      zgaz3167 = CONVERT_OF(mach_int, sail_int)(zgsz3347);
      KILL(sail_int)(&zgsz3347);
      KILL(sail_int)(&zgsz3346);
      KILL(sail_int)(&zgsz3345);
    }
    {
      sail_int zgsz3348;
      CREATE(sail_int)(&zgsz3348);
      CONVERT_OF(sail_int, mach_int)(&zgsz3348, zcap_addr_width);
      sail_int zgsz3349;
      CREATE(sail_int)(&zgsz3349);
      CONVERT_OF(sail_int, mach_int)(&zgsz3349, zgaz3167);
      lbits zgsz3350;
      CREATE(lbits)(&zgsz3350);
      vector_subrange_lbits(&zgsz3350, ztop, zgsz3348, zgsz3349);
      ztop2 = CONVERT_OF(fbits, lbits)(zgsz3350, true);
      KILL(lbits)(&zgsz3350);
      KILL(sail_int)(&zgsz3349);
      KILL(sail_int)(&zgsz3348);
    }
  
  }
  {
    bool zgaz3162;
    {
      bool zgaz3161;
      {
        int64_t zgaz3158;
        {
          sail_int zgsz3351;
          CREATE(sail_int)(&zgsz3351);
          CONVERT_OF(sail_int, mach_int)(&zgsz3351, zmaxE);
          sail_int zgsz3352;
          CREATE(sail_int)(&zgsz3352);
          CONVERT_OF(sail_int, mach_int)(&zgsz3352, INT64_C(1));
          sail_int zgsz3353;
          CREATE(sail_int)(&zgsz3353);
          sub_int(&zgsz3353, zgsz3351, zgsz3352);
          zgaz3158 = CONVERT_OF(mach_int, sail_int)(zgsz3353);
          KILL(sail_int)(&zgsz3353);
          KILL(sail_int)(&zgsz3352);
          KILL(sail_int)(&zgsz3351);
        }
        {
          sail_int zgsz3355;
          CREATE(sail_int)(&zgsz3355);
          CONVERT_OF(sail_int, mach_int)(&zgsz3355, zgaz3158);
          sail_int zgsz3354;
          CREATE(sail_int)(&zgsz3354);
          CONVERT_OF(sail_int, mach_int)(&zgsz3354, zE);
          zgaz3161 = lt(zgsz3354, zgsz3355);
          KILL(sail_int)(&zgsz3355);
          KILL(sail_int)(&zgsz3354);
        }
      
      }
      bool zgsz3359;
      if (zgaz3161) {
      int64_t zgaz3160;
      {
        uint64_t zgaz3159;
        {
          lbits zgsz3356;
          CREATE(lbits)(&zgsz3356);
          CONVERT_OF(lbits, fbits)(&zgsz3356, ztop2, UINT64_C(2) , true);
          lbits zgsz3357;
          CREATE(lbits)(&zgsz3357);
          CONVERT_OF(lbits, fbits)(&zgsz3357, zbase2, UINT64_C(2) , true);
          lbits zgsz3358;
          CREATE(lbits)(&zgsz3358);
          sub_bits(&zgsz3358, zgsz3356, zgsz3357);
          zgaz3159 = CONVERT_OF(fbits, lbits)(zgsz3358, true);
          KILL(lbits)(&zgsz3358);
          KILL(lbits)(&zgsz3357);
          KILL(lbits)(&zgsz3356);
        }
        zgaz3160 = ((mach_int) zgaz3159);
      
      }
      {
        sail_int zgsz3361;
        CREATE(sail_int)(&zgsz3361);
        CONVERT_OF(sail_int, mach_int)(&zgsz3361, INT64_C(1));
        sail_int zgsz3360;
        CREATE(sail_int)(&zgsz3360);
        CONVERT_OF(sail_int, mach_int)(&zgsz3360, zgaz3160);
        zgsz3359 = gt(zgsz3360, zgsz3361);
        KILL(sail_int)(&zgsz3361);
        KILL(sail_int)(&zgsz3360);
      }
    
      } else {  zgsz3359 = false;  }
      zgaz3162 = zgsz3359;
    
    }
    unit zgsz3362;
    if (zgaz3162) {
    fbits zgaz3164;
    {
      fbits zgaz3163;
      {
        sail_int zgsz3363;
        CREATE(sail_int)(&zgsz3363);
        CONVERT_OF(sail_int, mach_int)(&zgsz3363, zcap_addr_width);
        zgaz3163 = bitvector_access(ztop, zgsz3363);
        KILL(sail_int)(&zgsz3363);
      }
      zgaz3164 = sailgen_not_bit(zgaz3163);
    
    }
    {
      sail_int zgsz3364;
      CREATE(sail_int)(&zgsz3364);
      CONVERT_OF(sail_int, mach_int)(&zgsz3364, zcap_addr_width);
      update_lbits(&ztop, ztop, zgsz3364, zgaz3164);
      KILL(sail_int)(&zgsz3364);
    }
    zgsz3362 = UNIT;
  
    } else {  zgsz3362 = UNIT;  }
  
  }
  uint64_t zgaz3166;
  {
    int64_t zgaz3165;
    {
      sail_int zgsz3365;
      CREATE(sail_int)(&zgsz3365);
      CONVERT_OF(sail_int, mach_int)(&zgsz3365, zcap_addr_width);
      sail_int zgsz3366;
      CREATE(sail_int)(&zgsz3366);
      CONVERT_OF(sail_int, mach_int)(&zgsz3366, INT64_C(1));
      sail_int zgsz3367;
      CREATE(sail_int)(&zgsz3367);
      sub_int(&zgsz3367, zgsz3365, zgsz3366);
      zgaz3165 = CONVERT_OF(mach_int, sail_int)(zgsz3367);
      KILL(sail_int)(&zgsz3367);
      KILL(sail_int)(&zgsz3366);
      KILL(sail_int)(&zgsz3365);
    }
    {
      sail_int zgsz3368;
      CREATE(sail_int)(&zgsz3368);
      CONVERT_OF(sail_int, mach_int)(&zgsz3368, zgaz3165);
      sail_int zgsz3369;
      CREATE(sail_int)(&zgsz3369);
      CONVERT_OF(sail_int, mach_int)(&zgsz3369, INT64_C(0));
      lbits zgsz3370;
      CREATE(lbits)(&zgsz3370);
      vector_subrange_lbits(&zgsz3370, zbase, zgsz3368, zgsz3369);
      zgaz3166 = CONVERT_OF(fbits, lbits)(zgsz3370, true);
      KILL(lbits)(&zgsz3370);
      KILL(sail_int)(&zgsz3369);
      KILL(sail_int)(&zgsz3368);
    }
  
  }
  struct ztuple_z8z5bv64zCz0z5bvz9 zgsz3371;
  CREATE(ztuple_z8z5bv64zCz0z5bvz9)(&zgsz3371);
  zgsz3371.ztup0 = zgaz3166;
  COPY(lbits)(&((&zgsz3371)->ztup1), ztop);
  COPY(ztuple_z8z5bv64zCz0z5bvz9)((*(&zcbz340)), zgsz3371);
  KILL(ztuple_z8z5bv64zCz0z5bvz9)(&zgsz3371);



  KILL(lbits)(&ztop);
  KILL(lbits)(&zbase);












end_function_155: ;
  goto end_function_209;
end_block_exception_156: ;
  goto end_function_209;
end_function_209: ;
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

static void sailgen_getCapBounds(struct ztuple_z8z5izCz0z5iz9 *rop, struct zCapability);


static void sailgen_getCapBounds(struct ztuple_z8z5izCz0z5iz9 *zcbz341, struct zCapability zcap)
{
  __label__ case_159, finish_match_158, end_function_160, end_block_exception_161, end_function_208;

  struct ztuple_z8z5bv64zCz0z5bvz9 zgaz3192;
  CREATE(ztuple_z8z5bv64zCz0z5bvz9)(&zgaz3192);
  sailgen_getCapBoundsBits(&zgaz3192, zcap);
  struct ztuple_z8z5izCz0z5iz9 zgsz3372;
  CREATE(ztuple_z8z5izCz0z5iz9)(&zgsz3372);
  {
    uint64_t zbase;
    zbase = zgaz3192.ztup0;
    lbits ztop;
    CREATE(lbits)(&ztop);
    COPY(lbits)(&ztop, zgaz3192.ztup1);
    sail_int zgaz3193;
    CREATE(sail_int)(&zgaz3193);
    {
      lbits zgsz3373;
      CREATE(lbits)(&zgsz3373);
      CONVERT_OF(lbits, fbits)(&zgsz3373, zbase, UINT64_C(64) , true);
      sail_unsigned(&zgaz3193, zgsz3373);
      KILL(lbits)(&zgsz3373);
    }
    sail_int zgaz3194;
    CREATE(sail_int)(&zgaz3194);
    sail_unsigned(&zgaz3194, ztop);
    struct ztuple_z8z5izCz0z5iz9 zgsz3374;
    CREATE(ztuple_z8z5izCz0z5iz9)(&zgsz3374);
    COPY(sail_int)(&((&zgsz3374)->ztup0), zgaz3193);
    COPY(sail_int)(&((&zgsz3374)->ztup1), zgaz3194);
    COPY(ztuple_z8z5izCz0z5iz9)(&zgsz3372, zgsz3374);
    KILL(ztuple_z8z5izCz0z5iz9)(&zgsz3374);
    KILL(sail_int)(&zgaz3194);
    KILL(sail_int)(&zgaz3193);
    KILL(lbits)(&ztop);
  
    goto finish_match_158;
  }
case_159: ;
  sail_match_failure("getCapBounds");
finish_match_158: ;
  COPY(ztuple_z8z5izCz0z5iz9)((*(&zcbz341)), zgsz3372);
  KILL(ztuple_z8z5izCz0z5iz9)(&zgsz3372);
  KILL(ztuple_z8z5bv64zCz0z5bvz9)(&zgaz3192);
end_function_160: ;
  goto end_function_208;
end_block_exception_161: ;
  goto end_function_208;
end_function_208: ;
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

static struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 sailgen_setCapBounds(struct zCapability zcap, uint64_t zbase, lbits ztop)
{
  __label__ end_function_163, end_block_exception_164;

  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz342;
  lbits zext_base;
  CREATE(lbits)(&zext_base);
  {
    lbits zgsz3377;
    CREATE(lbits)(&zgsz3377);
    CONVERT_OF(lbits, fbits)(&zgsz3377, zbase, UINT64_C(64) , true);
    lbits zgsz3376;
    CREATE(lbits)(&zgsz3376);
    CONVERT_OF(lbits, fbits)(&zgsz3376, UINT64_C(0b0), UINT64_C(1) , true);
    append(&zext_base, zgsz3376, zgsz3377);
    KILL(lbits)(&zgsz3377);
    KILL(lbits)(&zgsz3376);
  }
  lbits zlength;
  CREATE(lbits)(&zlength);
  sub_bits(&zlength, ztop, zext_base);
  int64_t ze;
  {
    int64_t zgaz3228;
    {
      uint64_t zgaz3227;
      {
        int64_t zgaz3226;
        {
          sail_int zgsz3378;
          CREATE(sail_int)(&zgsz3378);
          CONVERT_OF(sail_int, mach_int)(&zgsz3378, zmantissa_width);
          sail_int zgsz3379;
          CREATE(sail_int)(&zgsz3379);
          CONVERT_OF(sail_int, mach_int)(&zgsz3379, INT64_C(1));
          sail_int zgsz3380;
          CREATE(sail_int)(&zgsz3380);
          sub_int(&zgsz3380, zgsz3378, zgsz3379);
          zgaz3226 = CONVERT_OF(mach_int, sail_int)(zgsz3380);
          KILL(sail_int)(&zgsz3380);
          KILL(sail_int)(&zgsz3379);
          KILL(sail_int)(&zgsz3378);
        }
        {
          sail_int zgsz3381;
          CREATE(sail_int)(&zgsz3381);
          CONVERT_OF(sail_int, mach_int)(&zgsz3381, zcap_addr_width);
          sail_int zgsz3382;
          CREATE(sail_int)(&zgsz3382);
          CONVERT_OF(sail_int, mach_int)(&zgsz3382, zgaz3226);
          lbits zgsz3383;
          CREATE(lbits)(&zgsz3383);
          vector_subrange_lbits(&zgsz3383, zlength, zgsz3381, zgsz3382);
          zgaz3227 = CONVERT_OF(fbits, lbits)(zgsz3383, true);
          KILL(lbits)(&zgsz3383);
          KILL(sail_int)(&zgsz3382);
          KILL(sail_int)(&zgsz3381);
        }
      
      }
      {
        lbits zgsz3384;
        CREATE(lbits)(&zgsz3384);
        CONVERT_OF(lbits, fbits)(&zgsz3384, zgaz3227, UINT64_C(52) , true);
        sail_int zgsz3385;
        CREATE(sail_int)(&zgsz3385);
        count_leading_zeros(&zgsz3385, zgsz3384);
        zgaz3228 = CONVERT_OF(mach_int, sail_int)(zgsz3385);
        KILL(sail_int)(&zgsz3385);
        KILL(lbits)(&zgsz3384);
      }
    
    }
    {
      sail_int zgsz3386;
      CREATE(sail_int)(&zgsz3386);
      CONVERT_OF(sail_int, mach_int)(&zgsz3386, zmaxE);
      sail_int zgsz3387;
      CREATE(sail_int)(&zgsz3387);
      CONVERT_OF(sail_int, mach_int)(&zgsz3387, zgaz3228);
      sail_int zgsz3388;
      CREATE(sail_int)(&zgsz3388);
      sub_int(&zgsz3388, zgsz3386, zgsz3387);
      ze = CONVERT_OF(mach_int, sail_int)(zgsz3388);
      KILL(sail_int)(&zgsz3388);
      KILL(sail_int)(&zgsz3387);
      KILL(sail_int)(&zgsz3386);
    }
  
  }
  bool zie;
  {
    bool zgaz3225;
    {
      sail_int zgsz3389;
      CREATE(sail_int)(&zgsz3389);
      CONVERT_OF(sail_int, mach_int)(&zgsz3389, ze);
      zgaz3225 = sailgen_zzz3zzzz7mzzJzzK0z3neq_int(zgsz3389, INT64_C(0));
      KILL(sail_int)(&zgsz3389);
    }
    bool zgsz3394;
    if (zgaz3225) {  zgsz3394 = true;  } else {
    fbits zgaz3224;
    {
      int64_t zgaz3223;
      {
        sail_int zgsz3390;
        CREATE(sail_int)(&zgsz3390);
        CONVERT_OF(sail_int, mach_int)(&zgsz3390, zmantissa_width);
        sail_int zgsz3391;
        CREATE(sail_int)(&zgsz3391);
        CONVERT_OF(sail_int, mach_int)(&zgsz3391, INT64_C(2));
        sail_int zgsz3392;
        CREATE(sail_int)(&zgsz3392);
        sub_int(&zgsz3392, zgsz3390, zgsz3391);
        zgaz3223 = CONVERT_OF(mach_int, sail_int)(zgsz3392);
        KILL(sail_int)(&zgsz3392);
        KILL(sail_int)(&zgsz3391);
        KILL(sail_int)(&zgsz3390);
      }
      {
        sail_int zgsz3393;
        CREATE(sail_int)(&zgsz3393);
        CONVERT_OF(sail_int, mach_int)(&zgsz3393, zgaz3223);
        zgaz3224 = bitvector_access(zlength, zgsz3393);
        KILL(sail_int)(&zgsz3393);
      }
    
    }
    zgsz3394 = eq_bit(zgaz3224, UINT64_C(1));
  
    }
    zie = zgsz3394;
  
  }
  uint64_t zBbits;
  {
    lbits zgsz3395;
    CREATE(lbits)(&zgsz3395);
    CONVERT_OF(lbits, fbits)(&zgsz3395, zbase, UINT64_C(64) , true);
    sail_int zgsz3396;
    CREATE(sail_int)(&zgsz3396);
    CONVERT_OF(sail_int, mach_int)(&zgsz3396, zmantissa_width);
    lbits zgsz3397;
    CREATE(lbits)(&zgsz3397);
    sail_truncate(&zgsz3397, zgsz3395, zgsz3396);
    zBbits = CONVERT_OF(fbits, lbits)(zgsz3397, true);
    KILL(lbits)(&zgsz3397);
    KILL(sail_int)(&zgsz3396);
    KILL(lbits)(&zgsz3395);
  }
  uint64_t zTbits;
  {
    sail_int zgsz3398;
    CREATE(sail_int)(&zgsz3398);
    CONVERT_OF(sail_int, mach_int)(&zgsz3398, zmantissa_width);
    lbits zgsz3399;
    CREATE(lbits)(&zgsz3399);
    sail_truncate(&zgsz3399, ztop, zgsz3398);
    zTbits = CONVERT_OF(fbits, lbits)(zgsz3399, true);
    KILL(lbits)(&zgsz3399);
    KILL(sail_int)(&zgsz3398);
  }
  bool zlostSignificantTop;
  zlostSignificantTop = false;
  bool zlostSignificantBase;
  zlostSignificantBase = false;
  bool zincE;
  zincE = false;
  {
    unit zgsz3400;
    if (zie) {
    uint64_t zB_ie;
    {
      uint64_t zgaz3218;
      {
        int64_t zgaz3217;
        {
          sail_int zgsz3401;
          CREATE(sail_int)(&zgsz3401);
          CONVERT_OF(sail_int, mach_int)(&zgsz3401, ze);
          sail_int zgsz3402;
          CREATE(sail_int)(&zgsz3402);
          CONVERT_OF(sail_int, mach_int)(&zgsz3402, INT64_C(3));
          sail_int zgsz3403;
          CREATE(sail_int)(&zgsz3403);
          add_int(&zgsz3403, zgsz3401, zgsz3402);
          zgaz3217 = CONVERT_OF(mach_int, sail_int)(zgsz3403);
          KILL(sail_int)(&zgsz3403);
          KILL(sail_int)(&zgsz3402);
          KILL(sail_int)(&zgsz3401);
        }
        {
          lbits zgsz3404;
          CREATE(lbits)(&zgsz3404);
          CONVERT_OF(lbits, fbits)(&zgsz3404, zbase, UINT64_C(64) , true);
          sail_int zgsz3405;
          CREATE(sail_int)(&zgsz3405);
          CONVERT_OF(sail_int, mach_int)(&zgsz3405, zgaz3217);
          lbits zgsz3406;
          CREATE(lbits)(&zgsz3406);
          shiftr(&zgsz3406, zgsz3404, zgsz3405);
          zgaz3218 = CONVERT_OF(fbits, lbits)(zgsz3406, true);
          KILL(lbits)(&zgsz3406);
          KILL(sail_int)(&zgsz3405);
          KILL(lbits)(&zgsz3404);
        }
      
      }
      int64_t zgaz3219;
      {
        sail_int zgsz3407;
        CREATE(sail_int)(&zgsz3407);
        CONVERT_OF(sail_int, mach_int)(&zgsz3407, zmantissa_width);
        sail_int zgsz3408;
        CREATE(sail_int)(&zgsz3408);
        CONVERT_OF(sail_int, mach_int)(&zgsz3408, INT64_C(3));
        sail_int zgsz3409;
        CREATE(sail_int)(&zgsz3409);
        sub_int(&zgsz3409, zgsz3407, zgsz3408);
        zgaz3219 = CONVERT_OF(mach_int, sail_int)(zgsz3409);
        KILL(sail_int)(&zgsz3409);
        KILL(sail_int)(&zgsz3408);
        KILL(sail_int)(&zgsz3407);
      }
      {
        lbits zgsz3410;
        CREATE(lbits)(&zgsz3410);
        CONVERT_OF(lbits, fbits)(&zgsz3410, zgaz3218, UINT64_C(64) , true);
        sail_int zgsz3411;
        CREATE(sail_int)(&zgsz3411);
        CONVERT_OF(sail_int, mach_int)(&zgsz3411, zgaz3219);
        lbits zgsz3412;
        CREATE(lbits)(&zgsz3412);
        sail_truncate(&zgsz3412, zgsz3410, zgsz3411);
        zB_ie = CONVERT_OF(fbits, lbits)(zgsz3412, true);
        KILL(lbits)(&zgsz3412);
        KILL(sail_int)(&zgsz3411);
        KILL(lbits)(&zgsz3410);
      }
    
    
    }
    uint64_t zT_ie;
    {
      lbits zgaz3215;
      CREATE(lbits)(&zgaz3215);
      {
        int64_t zgaz3214;
        {
          sail_int zgsz3413;
          CREATE(sail_int)(&zgsz3413);
          CONVERT_OF(sail_int, mach_int)(&zgsz3413, ze);
          sail_int zgsz3414;
          CREATE(sail_int)(&zgsz3414);
          CONVERT_OF(sail_int, mach_int)(&zgsz3414, INT64_C(3));
          sail_int zgsz3415;
          CREATE(sail_int)(&zgsz3415);
          add_int(&zgsz3415, zgsz3413, zgsz3414);
          zgaz3214 = CONVERT_OF(mach_int, sail_int)(zgsz3415);
          KILL(sail_int)(&zgsz3415);
          KILL(sail_int)(&zgsz3414);
          KILL(sail_int)(&zgsz3413);
        }
        {
          sail_int zgsz3416;
          CREATE(sail_int)(&zgsz3416);
          CONVERT_OF(sail_int, mach_int)(&zgsz3416, zgaz3214);
          shiftr(&zgaz3215, ztop, zgsz3416);
          KILL(sail_int)(&zgsz3416);
        }
      
      }
      int64_t zgaz3216;
      {
        sail_int zgsz3417;
        CREATE(sail_int)(&zgsz3417);
        CONVERT_OF(sail_int, mach_int)(&zgsz3417, zmantissa_width);
        sail_int zgsz3418;
        CREATE(sail_int)(&zgsz3418);
        CONVERT_OF(sail_int, mach_int)(&zgsz3418, INT64_C(3));
        sail_int zgsz3419;
        CREATE(sail_int)(&zgsz3419);
        sub_int(&zgsz3419, zgsz3417, zgsz3418);
        zgaz3216 = CONVERT_OF(mach_int, sail_int)(zgsz3419);
        KILL(sail_int)(&zgsz3419);
        KILL(sail_int)(&zgsz3418);
        KILL(sail_int)(&zgsz3417);
      }
      {
        sail_int zgsz3420;
        CREATE(sail_int)(&zgsz3420);
        CONVERT_OF(sail_int, mach_int)(&zgsz3420, zgaz3216);
        lbits zgsz3421;
        CREATE(lbits)(&zgsz3421);
        sail_truncate(&zgsz3421, zgaz3215, zgsz3420);
        zT_ie = CONVERT_OF(fbits, lbits)(zgsz3421, true);
        KILL(lbits)(&zgsz3421);
        KILL(sail_int)(&zgsz3420);
      }
    
      KILL(lbits)(&zgaz3215);
    }
    lbits zmaskLo;
    CREATE(lbits)(&zmaskLo);
    {
      int64_t zgaz3212;
      {
        sail_int zgsz3422;
        CREATE(sail_int)(&zgsz3422);
        CONVERT_OF(sail_int, mach_int)(&zgsz3422, INT64_C(64));
        sail_int zgsz3423;
        CREATE(sail_int)(&zgsz3423);
        CONVERT_OF(sail_int, mach_int)(&zgsz3423, INT64_C(1));
        sail_int zgsz3424;
        CREATE(sail_int)(&zgsz3424);
        add_int(&zgsz3424, zgsz3422, zgsz3423);
        zgaz3212 = CONVERT_OF(mach_int, sail_int)(zgsz3424);
        KILL(sail_int)(&zgsz3424);
        KILL(sail_int)(&zgsz3423);
        KILL(sail_int)(&zgsz3422);
      }
      sbits zgaz3213;
      {
        int64_t zgaz3211;
        {
          sail_int zgsz3425;
          CREATE(sail_int)(&zgsz3425);
          CONVERT_OF(sail_int, mach_int)(&zgsz3425, ze);
          sail_int zgsz3426;
          CREATE(sail_int)(&zgsz3426);
          CONVERT_OF(sail_int, mach_int)(&zgsz3426, INT64_C(3));
          sail_int zgsz3427;
          CREATE(sail_int)(&zgsz3427);
          add_int(&zgsz3427, zgsz3425, zgsz3426);
          zgaz3211 = CONVERT_OF(mach_int, sail_int)(zgsz3427);
          KILL(sail_int)(&zgsz3427);
          KILL(sail_int)(&zgsz3426);
          KILL(sail_int)(&zgsz3425);
        }
        {
          sail_int zgsz3428;
          CREATE(sail_int)(&zgsz3428);
          CONVERT_OF(sail_int, mach_int)(&zgsz3428, zgaz3211);
          lbits zgsz3429;
          CREATE(lbits)(&zgsz3429);
          sailgen_zzz3zzz3ones(&zgsz3429, zgsz3428);
          zgaz3213 = CONVERT_OF(sbits, lbits)(zgsz3429, true);
          KILL(lbits)(&zgsz3429);
          KILL(sail_int)(&zgsz3428);
        }
      
      }
      {
        lbits zgsz3431;
        CREATE(lbits)(&zgsz3431);
        CONVERT_OF(lbits, sbits)(&zgsz3431, zgaz3213, true);
        sail_int zgsz3430;
        CREATE(sail_int)(&zgsz3430);
        CONVERT_OF(sail_int, mach_int)(&zgsz3430, zgaz3212);
        sailgen_zzz3zzz3EXTZ(&zmaskLo, zgsz3430, zgsz3431);
        KILL(lbits)(&zgsz3431);
        KILL(sail_int)(&zgsz3430);
      }
    
    
    }
    {
      sail_int zgaz3196;
      CREATE(sail_int)(&zgaz3196);
      {
        lbits zgaz3195;
        CREATE(lbits)(&zgaz3195);
        and_bits(&zgaz3195, zext_base, zmaskLo);
        sail_unsigned(&zgaz3196, zgaz3195);
        KILL(lbits)(&zgaz3195);
      }
      zlostSignificantBase = sailgen_zzz3zzzz7mzzJzzK0z3neq_int(zgaz3196, INT64_C(0));
      unit zgsz3437;
      zgsz3437 = UNIT;
      KILL(sail_int)(&zgaz3196);
    }
    {
      sail_int zgaz3198;
      CREATE(sail_int)(&zgaz3198);
      {
        lbits zgaz3197;
        CREATE(lbits)(&zgaz3197);
        and_bits(&zgaz3197, ztop, zmaskLo);
        sail_unsigned(&zgaz3198, zgaz3197);
        KILL(lbits)(&zgaz3197);
      }
      zlostSignificantTop = sailgen_zzz3zzzz7mzzJzzK0z3neq_int(zgaz3198, INT64_C(0));
      unit zgsz3436;
      zgsz3436 = UNIT;
      KILL(sail_int)(&zgaz3198);
    }
    {
      unit zgsz3432;
      if (zlostSignificantTop) {
      {
        lbits zgsz3433;
        CREATE(lbits)(&zgsz3433);
        CONVERT_OF(lbits, fbits)(&zgsz3433, zT_ie, UINT64_C(11) , true);
        sail_int zgsz3434;
        CREATE(sail_int)(&zgsz3434);
        CONVERT_OF(sail_int, mach_int)(&zgsz3434, INT64_C(1));
        lbits zgsz3435;
        CREATE(lbits)(&zgsz3435);
        add_bits_int(&zgsz3435, zgsz3433, zgsz3434);
        zT_ie = CONVERT_OF(fbits, lbits)(zgsz3435, true);
        KILL(lbits)(&zgsz3435);
        KILL(sail_int)(&zgsz3434);
        KILL(lbits)(&zgsz3433);
      }
      zgsz3432 = UNIT;
      } else {  zgsz3432 = UNIT;  }
    }
    uint64_t zlen_ie;
    {
      lbits zgsz3438;
      CREATE(lbits)(&zgsz3438);
      CONVERT_OF(lbits, fbits)(&zgsz3438, zT_ie, UINT64_C(11) , true);
      lbits zgsz3439;
      CREATE(lbits)(&zgsz3439);
      CONVERT_OF(lbits, fbits)(&zgsz3439, zB_ie, UINT64_C(11) , true);
      lbits zgsz3440;
      CREATE(lbits)(&zgsz3440);
      sub_bits(&zgsz3440, zgsz3438, zgsz3439);
      zlen_ie = CONVERT_OF(fbits, lbits)(zgsz3440, true);
      KILL(lbits)(&zgsz3440);
      KILL(lbits)(&zgsz3439);
      KILL(lbits)(&zgsz3438);
    }
    {
      bool zgaz3201;
      {
        fbits zgaz3200;
        {
          int64_t zgaz3199;
          {
            sail_int zgsz3441;
            CREATE(sail_int)(&zgsz3441);
            CONVERT_OF(sail_int, mach_int)(&zgsz3441, zmantissa_width);
            sail_int zgsz3442;
            CREATE(sail_int)(&zgsz3442);
            CONVERT_OF(sail_int, mach_int)(&zgsz3442, INT64_C(4));
            sail_int zgsz3443;
            CREATE(sail_int)(&zgsz3443);
            sub_int(&zgsz3443, zgsz3441, zgsz3442);
            zgaz3199 = CONVERT_OF(mach_int, sail_int)(zgsz3443);
            KILL(sail_int)(&zgsz3443);
            KILL(sail_int)(&zgsz3442);
            KILL(sail_int)(&zgsz3441);
          }
          {
            sail_int zgsz3445;
            CREATE(sail_int)(&zgsz3445);
            CONVERT_OF(sail_int, mach_int)(&zgsz3445, zgaz3199);
            lbits zgsz3444;
            CREATE(lbits)(&zgsz3444);
            CONVERT_OF(lbits, fbits)(&zgsz3444, zlen_ie, UINT64_C(11) , true);
            zgaz3200 = bitvector_access(zgsz3444, zgsz3445);
            KILL(sail_int)(&zgsz3445);
            KILL(lbits)(&zgsz3444);
          }
        
        }
        zgaz3201 = eq_bit(zgaz3200, UINT64_C(1));
      
      }
      unit zgsz3450;
      if (zgaz3201) {
      {
        zincE = true;
        unit zgsz3472;
        zgsz3472 = UNIT;
      }
      {
        bool zgsz3453;
        if (zlostSignificantBase) {  zgsz3453 = true;  } else {
        fbits zgaz3202;
        {
          sail_int zgsz3452;
          CREATE(sail_int)(&zgsz3452);
          CONVERT_OF(sail_int, mach_int)(&zgsz3452, INT64_C(0));
          lbits zgsz3451;
          CREATE(lbits)(&zgsz3451);
          CONVERT_OF(lbits, fbits)(&zgsz3451, zB_ie, UINT64_C(11) , true);
          zgaz3202 = bitvector_access(zgsz3451, zgsz3452);
          KILL(sail_int)(&zgsz3452);
          KILL(lbits)(&zgsz3451);
        }
        zgsz3453 = eq_bit(zgaz3202, UINT64_C(1));
      
        }
        zlostSignificantBase = zgsz3453;
        unit zgsz3471;
        zgsz3471 = UNIT;
      }
      {
        bool zgsz3456;
        if (zlostSignificantTop) {  zgsz3456 = true;  } else {
        fbits zgaz3203;
        {
          sail_int zgsz3455;
          CREATE(sail_int)(&zgsz3455);
          CONVERT_OF(sail_int, mach_int)(&zgsz3455, INT64_C(0));
          lbits zgsz3454;
          CREATE(lbits)(&zgsz3454);
          CONVERT_OF(lbits, fbits)(&zgsz3454, zT_ie, UINT64_C(11) , true);
          zgaz3203 = bitvector_access(zgsz3454, zgsz3455);
          KILL(sail_int)(&zgsz3455);
          KILL(lbits)(&zgsz3454);
        }
        zgsz3456 = eq_bit(zgaz3203, UINT64_C(1));
      
        }
        zlostSignificantTop = zgsz3456;
        unit zgsz3470;
        zgsz3470 = UNIT;
      }
      {
        uint64_t zgaz3205;
        {
          int64_t zgaz3204;
          {
            sail_int zgsz3457;
            CREATE(sail_int)(&zgsz3457);
            CONVERT_OF(sail_int, mach_int)(&zgsz3457, ze);
            sail_int zgsz3458;
            CREATE(sail_int)(&zgsz3458);
            CONVERT_OF(sail_int, mach_int)(&zgsz3458, INT64_C(4));
            sail_int zgsz3459;
            CREATE(sail_int)(&zgsz3459);
            add_int(&zgsz3459, zgsz3457, zgsz3458);
            zgaz3204 = CONVERT_OF(mach_int, sail_int)(zgsz3459);
            KILL(sail_int)(&zgsz3459);
            KILL(sail_int)(&zgsz3458);
            KILL(sail_int)(&zgsz3457);
          }
          {
            lbits zgsz3460;
            CREATE(lbits)(&zgsz3460);
            CONVERT_OF(lbits, fbits)(&zgsz3460, zbase, UINT64_C(64) , true);
            sail_int zgsz3461;
            CREATE(sail_int)(&zgsz3461);
            CONVERT_OF(sail_int, mach_int)(&zgsz3461, zgaz3204);
            lbits zgsz3462;
            CREATE(lbits)(&zgsz3462);
            shiftr(&zgsz3462, zgsz3460, zgsz3461);
            zgaz3205 = CONVERT_OF(fbits, lbits)(zgsz3462, true);
            KILL(lbits)(&zgsz3462);
            KILL(sail_int)(&zgsz3461);
            KILL(lbits)(&zgsz3460);
          }
        
        }
        int64_t zgaz3206;
        {
          sail_int zgsz3463;
          CREATE(sail_int)(&zgsz3463);
          CONVERT_OF(sail_int, mach_int)(&zgsz3463, zmantissa_width);
          sail_int zgsz3464;
          CREATE(sail_int)(&zgsz3464);
          CONVERT_OF(sail_int, mach_int)(&zgsz3464, INT64_C(3));
          sail_int zgsz3465;
          CREATE(sail_int)(&zgsz3465);
          sub_int(&zgsz3465, zgsz3463, zgsz3464);
          zgaz3206 = CONVERT_OF(mach_int, sail_int)(zgsz3465);
          KILL(sail_int)(&zgsz3465);
          KILL(sail_int)(&zgsz3464);
          KILL(sail_int)(&zgsz3463);
        }
        {
          lbits zgsz3466;
          CREATE(lbits)(&zgsz3466);
          CONVERT_OF(lbits, fbits)(&zgsz3466, zgaz3205, UINT64_C(64) , true);
          sail_int zgsz3467;
          CREATE(sail_int)(&zgsz3467);
          CONVERT_OF(sail_int, mach_int)(&zgsz3467, zgaz3206);
          lbits zgsz3468;
          CREATE(lbits)(&zgsz3468);
          sail_truncate(&zgsz3468, zgsz3466, zgsz3467);
          zB_ie = CONVERT_OF(fbits, lbits)(zgsz3468, true);
          KILL(lbits)(&zgsz3468);
          KILL(sail_int)(&zgsz3467);
          KILL(lbits)(&zgsz3466);
        }
        unit zgsz3469;
        zgsz3469 = UNIT;
      
      
      }
      int64_t zincT;
      if (zlostSignificantTop) {  zincT = INT64_C(1);  } else {  zincT = INT64_C(0);  }
      uint64_t zgaz3210;
      {
        lbits zgaz3208;
        CREATE(lbits)(&zgaz3208);
        {
          int64_t zgaz3207;
          {
            sail_int zgsz3473;
            CREATE(sail_int)(&zgsz3473);
            CONVERT_OF(sail_int, mach_int)(&zgsz3473, ze);
            sail_int zgsz3474;
            CREATE(sail_int)(&zgsz3474);
            CONVERT_OF(sail_int, mach_int)(&zgsz3474, INT64_C(4));
            sail_int zgsz3475;
            CREATE(sail_int)(&zgsz3475);
            add_int(&zgsz3475, zgsz3473, zgsz3474);
            zgaz3207 = CONVERT_OF(mach_int, sail_int)(zgsz3475);
            KILL(sail_int)(&zgsz3475);
            KILL(sail_int)(&zgsz3474);
            KILL(sail_int)(&zgsz3473);
          }
          {
            sail_int zgsz3476;
            CREATE(sail_int)(&zgsz3476);
            CONVERT_OF(sail_int, mach_int)(&zgsz3476, zgaz3207);
            shiftr(&zgaz3208, ztop, zgsz3476);
            KILL(sail_int)(&zgsz3476);
          }
        
        }
        int64_t zgaz3209;
        {
          sail_int zgsz3477;
          CREATE(sail_int)(&zgsz3477);
          CONVERT_OF(sail_int, mach_int)(&zgsz3477, zmantissa_width);
          sail_int zgsz3478;
          CREATE(sail_int)(&zgsz3478);
          CONVERT_OF(sail_int, mach_int)(&zgsz3478, INT64_C(3));
          sail_int zgsz3479;
          CREATE(sail_int)(&zgsz3479);
          sub_int(&zgsz3479, zgsz3477, zgsz3478);
          zgaz3209 = CONVERT_OF(mach_int, sail_int)(zgsz3479);
          KILL(sail_int)(&zgsz3479);
          KILL(sail_int)(&zgsz3478);
          KILL(sail_int)(&zgsz3477);
        }
        {
          sail_int zgsz3480;
          CREATE(sail_int)(&zgsz3480);
          CONVERT_OF(sail_int, mach_int)(&zgsz3480, zgaz3209);
          lbits zgsz3481;
          CREATE(lbits)(&zgsz3481);
          sail_truncate(&zgsz3481, zgaz3208, zgsz3480);
          zgaz3210 = CONVERT_OF(fbits, lbits)(zgsz3481, true);
          KILL(lbits)(&zgsz3481);
          KILL(sail_int)(&zgsz3480);
        }
      
        KILL(lbits)(&zgaz3208);
      }
      {
        lbits zgsz3482;
        CREATE(lbits)(&zgsz3482);
        CONVERT_OF(lbits, fbits)(&zgsz3482, zgaz3210, UINT64_C(11) , true);
        sail_int zgsz3483;
        CREATE(sail_int)(&zgsz3483);
        CONVERT_OF(sail_int, mach_int)(&zgsz3483, zincT);
        lbits zgsz3484;
        CREATE(lbits)(&zgsz3484);
        add_bits_int(&zgsz3484, zgsz3482, zgsz3483);
        zT_ie = CONVERT_OF(fbits, lbits)(zgsz3484, true);
        KILL(lbits)(&zgsz3484);
        KILL(sail_int)(&zgsz3483);
        KILL(lbits)(&zgsz3482);
      }
      zgsz3450 = UNIT;
    
    
      } else {  zgsz3450 = UNIT;  }
    
    }
    {
      {
        lbits zgsz3446;
        CREATE(lbits)(&zgsz3446);
        CONVERT_OF(lbits, fbits)(&zgsz3446, zB_ie, UINT64_C(11) , true);
        lbits zgsz3447;
        CREATE(lbits)(&zgsz3447);
        CONVERT_OF(lbits, fbits)(&zgsz3447, UINT64_C(0b000), UINT64_C(3) , true);
        lbits zgsz3448;
        CREATE(lbits)(&zgsz3448);
        append(&zgsz3448, zgsz3446, zgsz3447);
        zBbits = CONVERT_OF(fbits, lbits)(zgsz3448, true);
        KILL(lbits)(&zgsz3448);
        KILL(lbits)(&zgsz3447);
        KILL(lbits)(&zgsz3446);
      }
      unit zgsz3449;
      zgsz3449 = UNIT;
    }
    {
      lbits zgsz3485;
      CREATE(lbits)(&zgsz3485);
      CONVERT_OF(lbits, fbits)(&zgsz3485, zT_ie, UINT64_C(11) , true);
      lbits zgsz3486;
      CREATE(lbits)(&zgsz3486);
      CONVERT_OF(lbits, fbits)(&zgsz3486, UINT64_C(0b000), UINT64_C(3) , true);
      lbits zgsz3487;
      CREATE(lbits)(&zgsz3487);
      append(&zgsz3487, zgsz3485, zgsz3486);
      zTbits = CONVERT_OF(fbits, lbits)(zgsz3487, true);
      KILL(lbits)(&zgsz3487);
      KILL(lbits)(&zgsz3486);
      KILL(lbits)(&zgsz3485);
    }
    zgsz3400 = UNIT;
  
    KILL(lbits)(&zmaskLo);
  
  
    } else {  zgsz3400 = UNIT;  }
  }
  struct zCapability znewCap;
  {
    uint64_t zgaz3222;
    {
      int64_t zgaz3221;
      if (zincE) {
      {
        sail_int zgsz3488;
        CREATE(sail_int)(&zgsz3488);
        CONVERT_OF(sail_int, mach_int)(&zgsz3488, ze);
        sail_int zgsz3489;
        CREATE(sail_int)(&zgsz3489);
        CONVERT_OF(sail_int, mach_int)(&zgsz3489, INT64_C(1));
        sail_int zgsz3490;
        CREATE(sail_int)(&zgsz3490);
        add_int(&zgsz3490, zgsz3488, zgsz3489);
        zgaz3221 = CONVERT_OF(mach_int, sail_int)(zgsz3490);
        KILL(sail_int)(&zgsz3490);
        KILL(sail_int)(&zgsz3489);
        KILL(sail_int)(&zgsz3488);
      }
      } else {  zgaz3221 = ze;  }
      {
        sail_int zgsz3491;
        CREATE(sail_int)(&zgsz3491);
        CONVERT_OF(sail_int, mach_int)(&zgsz3491, zgaz3221);
        zgaz3222 = sailgen_zzzz7lzzJzzK6z3to_bits(INT64_C(6), zgsz3491);
        KILL(sail_int)(&zgsz3491);
      }
    
    }
    struct zCapability zgsz3492;
    zgsz3492 = zcap;
    zgsz3492.zB = zBbits;
    zgsz3492.zE = zgaz3222;
    zgsz3492.zT = zTbits;
    zgsz3492.zaddress = zbase;
    zgsz3492.zinternal_e = zie;
    znewCap = zgsz3492;
  
  
  }
  bool zexact;
  {
    bool zgaz3220;
    {
      bool zgsz3493;
      if (zlostSignificantBase) {  zgsz3493 = true;  } else {  zgsz3493 = zlostSignificantTop;  }
      zgaz3220 = zgsz3493;
    }
    zexact = not(zgaz3220);
  
  }
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgsz3494;
  zgsz3494.ztup0 = zexact;
  zgsz3494.ztup1 = znewCap;
  zcbz342 = zgsz3494;










  KILL(lbits)(&zlength);
  KILL(lbits)(&zext_base);
end_function_163: ;
  return zcbz342;
end_block_exception_164: ;
  struct zCapability zcbz358 = { .zuperms = UINT64_C(0xdeadc0de), .ztag = false, .zsealed = false, .zreserved = UINT64_C(0xdeadc0de), .zpermit_unseal = false, .zpermit_store_local_cap = false, .zpermit_store_cap = false, .zpermit_store = false, .zpermit_set_CID = false, .zpermit_seal = false, .zpermit_load_cap = false, .zpermit_load = false, .zpermit_execute = false, .zpermit_ccall = false, .zotype = UINT64_C(0xdeadc0de), .zinternal_e = false, .zglobal = false, .zflag_cap_mode = false, .zaddress = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zT = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz357 = { .ztup1 = zcbz358, .ztup0 = false };
  return zcbz357;
}

static uint64_t sailgen_getCapPerms(struct zCapability);

static uint64_t sailgen_getCapPerms(struct zCapability zcap)
{
  __label__ end_function_166, end_block_exception_167;

  uint64_t zcbz343;
  uint64_t zperms;
  {
    uint64_t zgaz3231;
    zgaz3231 = sailgen_getCapHardPerms(zcap);
    zperms = sailgen_zzzz7mzzJzzK15zzCzz0zz7nzzJzzK12z3EXTZ(INT64_C(15), zgaz3231);
  
  }
  uint64_t zgaz3230;
  {
    uint64_t zgaz3229;
    zgaz3229 = zcap.zuperms;
    {
      lbits zgsz3495;
      CREATE(lbits)(&zgsz3495);
      CONVERT_OF(lbits, fbits)(&zgsz3495, zgaz3229, UINT64_C(4) , true);
      lbits zgsz3496;
      CREATE(lbits)(&zgsz3496);
      CONVERT_OF(lbits, fbits)(&zgsz3496, zperms, UINT64_C(15) , true);
      lbits zgsz3497;
      CREATE(lbits)(&zgsz3497);
      append(&zgsz3497, zgsz3495, zgsz3496);
      zgaz3230 = CONVERT_OF(fbits, lbits)(zgsz3497, true);
      KILL(lbits)(&zgsz3497);
      KILL(lbits)(&zgsz3496);
      KILL(lbits)(&zgsz3495);
    }
  
  }
  {
    sbits zgsz3498;
    zgsz3498 = CONVERT_OF(sbits, fbits)(zgaz3230, UINT64_C(19) , true);
    zcbz343 = sailgen_zzz3zzzz7mzzJzzK31z3EXTZ(INT64_C(31), zgsz3498);
  
  }


end_function_166: ;
  return zcbz343;
end_block_exception_167: ;

  return UINT64_C(0xdeadc0de);
}

static struct zCapability sailgen_setCapPerms(struct zCapability, uint64_t);

static struct zCapability sailgen_setCapPerms(struct zCapability zcap, uint64_t zperms)
{
  __label__ end_function_169, end_block_exception_170;

  struct zCapability zcbz344;
  uint64_t zgaz3233;
  {
    uint64_t zgaz3232;
    {
      lbits zgsz3499;
      CREATE(lbits)(&zgsz3499);
      CONVERT_OF(lbits, fbits)(&zgsz3499, zperms, UINT64_C(31) , true);
      sail_int zgsz3500;
      CREATE(sail_int)(&zgsz3500);
      CONVERT_OF(sail_int, mach_int)(&zgsz3500, INT64_C(30));
      sail_int zgsz3501;
      CREATE(sail_int)(&zgsz3501);
      CONVERT_OF(sail_int, mach_int)(&zgsz3501, INT64_C(15));
      lbits zgsz3502;
      CREATE(lbits)(&zgsz3502);
      vector_subrange_lbits(&zgsz3502, zgsz3499, zgsz3500, zgsz3501);
      zgaz3232 = CONVERT_OF(fbits, lbits)(zgsz3502, true);
      KILL(lbits)(&zgsz3502);
      KILL(sail_int)(&zgsz3501);
      KILL(sail_int)(&zgsz3500);
      KILL(lbits)(&zgsz3499);
    }
    {
      lbits zgsz3503;
      CREATE(lbits)(&zgsz3503);
      CONVERT_OF(lbits, fbits)(&zgsz3503, zgaz3232, UINT64_C(16) , true);
      sail_int zgsz3504;
      CREATE(sail_int)(&zgsz3504);
      CONVERT_OF(sail_int, mach_int)(&zgsz3504, zuperms_width);
      lbits zgsz3505;
      CREATE(lbits)(&zgsz3505);
      sail_truncate(&zgsz3505, zgsz3503, zgsz3504);
      zgaz3233 = CONVERT_OF(fbits, lbits)(zgsz3505, true);
      KILL(lbits)(&zgsz3505);
      KILL(sail_int)(&zgsz3504);
      KILL(lbits)(&zgsz3503);
    }
  
  }
  bool zgaz3235;
  {
    fbits zgaz3234;
    {
      sail_int zgsz3507;
      CREATE(sail_int)(&zgsz3507);
      CONVERT_OF(sail_int, mach_int)(&zgsz3507, INT64_C(11));
      lbits zgsz3506;
      CREATE(lbits)(&zgsz3506);
      CONVERT_OF(lbits, fbits)(&zgsz3506, zperms, UINT64_C(31) , true);
      zgaz3234 = bitvector_access(zgsz3506, zgsz3507);
      KILL(sail_int)(&zgsz3507);
      KILL(lbits)(&zgsz3506);
    }
    zgaz3235 = sailgen_bit_to_bool(zgaz3234);
  
  }
  bool zgaz3237;
  {
    fbits zgaz3236;
    {
      sail_int zgsz3509;
      CREATE(sail_int)(&zgsz3509);
      CONVERT_OF(sail_int, mach_int)(&zgsz3509, INT64_C(10));
      lbits zgsz3508;
      CREATE(lbits)(&zgsz3508);
      CONVERT_OF(lbits, fbits)(&zgsz3508, zperms, UINT64_C(31) , true);
      zgaz3236 = bitvector_access(zgsz3508, zgsz3509);
      KILL(sail_int)(&zgsz3509);
      KILL(lbits)(&zgsz3508);
    }
    zgaz3237 = sailgen_bit_to_bool(zgaz3236);
  
  }
  bool zgaz3239;
  {
    fbits zgaz3238;
    {
      sail_int zgsz3511;
      CREATE(sail_int)(&zgsz3511);
      CONVERT_OF(sail_int, mach_int)(&zgsz3511, INT64_C(9));
      lbits zgsz3510;
      CREATE(lbits)(&zgsz3510);
      CONVERT_OF(lbits, fbits)(&zgsz3510, zperms, UINT64_C(31) , true);
      zgaz3238 = bitvector_access(zgsz3510, zgsz3511);
      KILL(sail_int)(&zgsz3511);
      KILL(lbits)(&zgsz3510);
    }
    zgaz3239 = sailgen_bit_to_bool(zgaz3238);
  
  }
  bool zgaz3241;
  {
    fbits zgaz3240;
    {
      sail_int zgsz3513;
      CREATE(sail_int)(&zgsz3513);
      CONVERT_OF(sail_int, mach_int)(&zgsz3513, INT64_C(8));
      lbits zgsz3512;
      CREATE(lbits)(&zgsz3512);
      CONVERT_OF(lbits, fbits)(&zgsz3512, zperms, UINT64_C(31) , true);
      zgaz3240 = bitvector_access(zgsz3512, zgsz3513);
      KILL(sail_int)(&zgsz3513);
      KILL(lbits)(&zgsz3512);
    }
    zgaz3241 = sailgen_bit_to_bool(zgaz3240);
  
  }
  bool zgaz3243;
  {
    fbits zgaz3242;
    {
      sail_int zgsz3515;
      CREATE(sail_int)(&zgsz3515);
      CONVERT_OF(sail_int, mach_int)(&zgsz3515, INT64_C(7));
      lbits zgsz3514;
      CREATE(lbits)(&zgsz3514);
      CONVERT_OF(lbits, fbits)(&zgsz3514, zperms, UINT64_C(31) , true);
      zgaz3242 = bitvector_access(zgsz3514, zgsz3515);
      KILL(sail_int)(&zgsz3515);
      KILL(lbits)(&zgsz3514);
    }
    zgaz3243 = sailgen_bit_to_bool(zgaz3242);
  
  }
  bool zgaz3245;
  {
    fbits zgaz3244;
    {
      sail_int zgsz3517;
      CREATE(sail_int)(&zgsz3517);
      CONVERT_OF(sail_int, mach_int)(&zgsz3517, INT64_C(6));
      lbits zgsz3516;
      CREATE(lbits)(&zgsz3516);
      CONVERT_OF(lbits, fbits)(&zgsz3516, zperms, UINT64_C(31) , true);
      zgaz3244 = bitvector_access(zgsz3516, zgsz3517);
      KILL(sail_int)(&zgsz3517);
      KILL(lbits)(&zgsz3516);
    }
    zgaz3245 = sailgen_bit_to_bool(zgaz3244);
  
  }
  bool zgaz3247;
  {
    fbits zgaz3246;
    {
      sail_int zgsz3519;
      CREATE(sail_int)(&zgsz3519);
      CONVERT_OF(sail_int, mach_int)(&zgsz3519, INT64_C(5));
      lbits zgsz3518;
      CREATE(lbits)(&zgsz3518);
      CONVERT_OF(lbits, fbits)(&zgsz3518, zperms, UINT64_C(31) , true);
      zgaz3246 = bitvector_access(zgsz3518, zgsz3519);
      KILL(sail_int)(&zgsz3519);
      KILL(lbits)(&zgsz3518);
    }
    zgaz3247 = sailgen_bit_to_bool(zgaz3246);
  
  }
  bool zgaz3249;
  {
    fbits zgaz3248;
    {
      sail_int zgsz3521;
      CREATE(sail_int)(&zgsz3521);
      CONVERT_OF(sail_int, mach_int)(&zgsz3521, INT64_C(4));
      lbits zgsz3520;
      CREATE(lbits)(&zgsz3520);
      CONVERT_OF(lbits, fbits)(&zgsz3520, zperms, UINT64_C(31) , true);
      zgaz3248 = bitvector_access(zgsz3520, zgsz3521);
      KILL(sail_int)(&zgsz3521);
      KILL(lbits)(&zgsz3520);
    }
    zgaz3249 = sailgen_bit_to_bool(zgaz3248);
  
  }
  bool zgaz3251;
  {
    fbits zgaz3250;
    {
      sail_int zgsz3523;
      CREATE(sail_int)(&zgsz3523);
      CONVERT_OF(sail_int, mach_int)(&zgsz3523, INT64_C(3));
      lbits zgsz3522;
      CREATE(lbits)(&zgsz3522);
      CONVERT_OF(lbits, fbits)(&zgsz3522, zperms, UINT64_C(31) , true);
      zgaz3250 = bitvector_access(zgsz3522, zgsz3523);
      KILL(sail_int)(&zgsz3523);
      KILL(lbits)(&zgsz3522);
    }
    zgaz3251 = sailgen_bit_to_bool(zgaz3250);
  
  }
  bool zgaz3253;
  {
    fbits zgaz3252;
    {
      sail_int zgsz3525;
      CREATE(sail_int)(&zgsz3525);
      CONVERT_OF(sail_int, mach_int)(&zgsz3525, INT64_C(2));
      lbits zgsz3524;
      CREATE(lbits)(&zgsz3524);
      CONVERT_OF(lbits, fbits)(&zgsz3524, zperms, UINT64_C(31) , true);
      zgaz3252 = bitvector_access(zgsz3524, zgsz3525);
      KILL(sail_int)(&zgsz3525);
      KILL(lbits)(&zgsz3524);
    }
    zgaz3253 = sailgen_bit_to_bool(zgaz3252);
  
  }
  bool zgaz3255;
  {
    fbits zgaz3254;
    {
      sail_int zgsz3527;
      CREATE(sail_int)(&zgsz3527);
      CONVERT_OF(sail_int, mach_int)(&zgsz3527, INT64_C(1));
      lbits zgsz3526;
      CREATE(lbits)(&zgsz3526);
      CONVERT_OF(lbits, fbits)(&zgsz3526, zperms, UINT64_C(31) , true);
      zgaz3254 = bitvector_access(zgsz3526, zgsz3527);
      KILL(sail_int)(&zgsz3527);
      KILL(lbits)(&zgsz3526);
    }
    zgaz3255 = sailgen_bit_to_bool(zgaz3254);
  
  }
  bool zgaz3257;
  {
    fbits zgaz3256;
    {
      sail_int zgsz3529;
      CREATE(sail_int)(&zgsz3529);
      CONVERT_OF(sail_int, mach_int)(&zgsz3529, INT64_C(0));
      lbits zgsz3528;
      CREATE(lbits)(&zgsz3528);
      CONVERT_OF(lbits, fbits)(&zgsz3528, zperms, UINT64_C(31) , true);
      zgaz3256 = bitvector_access(zgsz3528, zgsz3529);
      KILL(sail_int)(&zgsz3529);
      KILL(lbits)(&zgsz3528);
    }
    zgaz3257 = sailgen_bit_to_bool(zgaz3256);
  
  }
  struct zCapability zgsz3530;
  zgsz3530 = zcap;
  zgsz3530.zaccess_system_regs = zgaz3237;
  zgsz3530.zglobal = zgaz3257;
  zgsz3530.zpermit_ccall = zgaz3241;
  zgsz3530.zpermit_execute = zgaz3255;
  zgsz3530.zpermit_load = zgaz3253;
  zgsz3530.zpermit_load_cap = zgaz3249;
  zgsz3530.zpermit_seal = zgaz3243;
  zgsz3530.zpermit_set_CID = zgaz3235;
  zgsz3530.zpermit_store = zgaz3251;
  zgsz3530.zpermit_store_cap = zgaz3247;
  zgsz3530.zpermit_store_local_cap = zgaz3245;
  zgsz3530.zpermit_unseal = zgaz3239;
  zgsz3530.zuperms = zgaz3233;
  zcbz344 = zgsz3530;














end_function_169: ;
  return zcbz344;
end_block_exception_170: ;
  struct zCapability zcbz359 = { .zuperms = UINT64_C(0xdeadc0de), .ztag = false, .zsealed = false, .zreserved = UINT64_C(0xdeadc0de), .zpermit_unseal = false, .zpermit_store_local_cap = false, .zpermit_store_cap = false, .zpermit_store = false, .zpermit_set_CID = false, .zpermit_seal = false, .zpermit_load_cap = false, .zpermit_load = false, .zpermit_execute = false, .zpermit_ccall = false, .zotype = UINT64_C(0xdeadc0de), .zinternal_e = false, .zglobal = false, .zflag_cap_mode = false, .zaddress = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zT = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  return zcbz359;
}

static uint64_t sailgen_getCapFlags(struct zCapability);

static uint64_t sailgen_getCapFlags(struct zCapability zcap)
{
  __label__ end_function_172, end_block_exception_173;

  uint64_t zcbz345;
  bool zgaz3258;
  zgaz3258 = zcap.zflag_cap_mode;
  zcbz345 = sailgen_bool_to_bits(zgaz3258);

end_function_172: ;
  return zcbz345;
end_block_exception_173: ;

  return UINT64_C(0xdeadc0de);
}

static bool sailgen_hasReservedOType(struct zCapability);

static bool sailgen_hasReservedOType(struct zCapability zcap)
{
  __label__ end_function_175, end_block_exception_176;

  bool zcbz346;
  int64_t zgaz3260;
  {
    uint64_t zgaz3259;
    zgaz3259 = zcap.zotype;
    zgaz3260 = ((mach_int) zgaz3259);
  
  }
  {
    sail_int zgsz3532;
    CREATE(sail_int)(&zgsz3532);
    CONVERT_OF(sail_int, mach_int)(&zgsz3532, zmax_otype);
    sail_int zgsz3531;
    CREATE(sail_int)(&zgsz3531);
    CONVERT_OF(sail_int, mach_int)(&zgsz3531, zgaz3260);
    zcbz346 = gt(zgsz3531, zgsz3532);
    KILL(sail_int)(&zgsz3532);
    KILL(sail_int)(&zgsz3531);
  }

end_function_175: ;
  return zcbz346;
end_block_exception_176: ;

  return false;
}

static uint64_t sailgen_getCapBaseBits(struct zCapability);

static uint64_t sailgen_getCapBaseBits(struct zCapability zc)
{
  __label__ case_179, finish_match_178, end_function_180, end_block_exception_181;

  uint64_t zcbz347;
  struct ztuple_z8z5bv64zCz0z5bvz9 zgaz3261;
  CREATE(ztuple_z8z5bv64zCz0z5bvz9)(&zgaz3261);
  sailgen_getCapBoundsBits(&zgaz3261, zc);
  uint64_t zgsz3533;
  {
    uint64_t zbase;
    zbase = zgaz3261.ztup0;
    zgsz3533 = zbase;
  
    goto finish_match_178;
  }
case_179: ;
  sail_match_failure("getCapBaseBits");
finish_match_178: ;
  zcbz347 = zgsz3533;

  KILL(ztuple_z8z5bv64zCz0z5bvz9)(&zgaz3261);
end_function_180: ;
  return zcbz347;
end_block_exception_181: ;

  return UINT64_C(0xdeadc0de);
}

static uint64_t sailgen_getCapOffsetBits(struct zCapability);

static uint64_t sailgen_getCapOffsetBits(struct zCapability zc)
{
  __label__ end_function_183, end_block_exception_184;

  uint64_t zcbz348;
  uint64_t zbase;
  zbase = sailgen_getCapBaseBits(zc);
  uint64_t zgaz3262;
  zgaz3262 = zc.zaddress;
  {
    lbits zgsz3535;
    CREATE(lbits)(&zgsz3535);
    CONVERT_OF(lbits, fbits)(&zgsz3535, zgaz3262, UINT64_C(64) , true);
    lbits zgsz3536;
    CREATE(lbits)(&zgsz3536);
    CONVERT_OF(lbits, fbits)(&zgsz3536, zbase, UINT64_C(64) , true);
    lbits zgsz3537;
    CREATE(lbits)(&zgsz3537);
    sub_bits(&zgsz3537, zgsz3535, zgsz3536);
    zcbz348 = CONVERT_OF(fbits, lbits)(zgsz3537, true);
    KILL(lbits)(&zgsz3537);
    KILL(lbits)(&zgsz3536);
    KILL(lbits)(&zgsz3535);
  }


end_function_183: ;
  return zcbz348;
end_block_exception_184: ;

  return UINT64_C(0xdeadc0de);
}

static void sailgen_getCapLength(sail_int *rop, struct zCapability);

static void sailgen_getCapLength(sail_int *zcbz349, struct zCapability zc)
{
  __label__ case_187, finish_match_186, end_function_188, end_block_exception_189, end_function_207;

  struct ztuple_z8z5izCz0z5iz9 zgaz3263;
  CREATE(ztuple_z8z5izCz0z5iz9)(&zgaz3263);
  sailgen_getCapBounds(&zgaz3263, zc);
  sail_int zgsz3538;
  CREATE(sail_int)(&zgsz3538);
  {
    sail_int zbase;
    CREATE(sail_int)(&zbase);
    COPY(sail_int)(&zbase, zgaz3263.ztup0);
    sail_int ztop;
    CREATE(sail_int)(&ztop);
    COPY(sail_int)(&ztop, zgaz3263.ztup1);
    {
      bool zgaz3266;
      {
        bool zgaz3265;
        {
          bool zgaz3264;
          zgaz3264 = zc.ztag;
          zgaz3265 = not(zgaz3264);
        
        }
        bool zgsz3539;
        if (zgaz3265) {  zgsz3539 = true;  } else {  zgsz3539 = gteq(ztop, zbase);  }
        zgaz3266 = zgsz3539;
      
      }
      unit zgsz3540;
      zgsz3540 = sail_assert(zgaz3266, "src/cheri_cap_common.sail 271:40 - 271:41");
    
    }
    sail_int zgaz3267;
    CREATE(sail_int)(&zgaz3267);
    sub_int(&zgaz3267, ztop, zbase);
    sail_int zgaz3268;
    CREATE(sail_int)(&zgaz3268);
    {
      sail_int zgsz3541;
      CREATE(sail_int)(&zgsz3541);
      CONVERT_OF(sail_int, mach_int)(&zgsz3541, zcap_len_width);
      pow2(&zgaz3268, zgsz3541);
      KILL(sail_int)(&zgsz3541);
    }
    emod_int(&zgsz3538, zgaz3267, zgaz3268);
    KILL(sail_int)(&zgaz3268);
    KILL(sail_int)(&zgaz3267);
    KILL(sail_int)(&ztop);
    KILL(sail_int)(&zbase);
    goto finish_match_186;
  }
case_187: ;
  sail_match_failure("getCapLength");
finish_match_186: ;
  COPY(sail_int)((*(&zcbz349)), zgsz3538);
  KILL(sail_int)(&zgsz3538);
  KILL(ztuple_z8z5izCz0z5iz9)(&zgaz3263);
end_function_188: ;
  goto end_function_207;
end_block_exception_189: ;
  goto end_function_207;
end_function_207: ;
}

static bool sailgen_fastRepCheck(struct zCapability, uint64_t);

static bool sailgen_fastRepCheck(struct zCapability zc, uint64_t zi)
{
  __label__ end_function_191, end_block_exception_192;

  bool zcbz350;
  int64_t zE;
  {
    uint64_t zgaz3282;
    zgaz3282 = zc.zE;
    zE = ((mach_int) zgaz3282);
  
  }
  bool zgaz3270;
  {
    int64_t zgaz3269;
    {
      sail_int zgsz3543;
      CREATE(sail_int)(&zgsz3543);
      CONVERT_OF(sail_int, mach_int)(&zgsz3543, zmaxE);
      sail_int zgsz3544;
      CREATE(sail_int)(&zgsz3544);
      CONVERT_OF(sail_int, mach_int)(&zgsz3544, INT64_C(2));
      sail_int zgsz3545;
      CREATE(sail_int)(&zgsz3545);
      sub_int(&zgsz3545, zgsz3543, zgsz3544);
      zgaz3269 = CONVERT_OF(mach_int, sail_int)(zgsz3545);
      KILL(sail_int)(&zgsz3545);
      KILL(sail_int)(&zgsz3544);
      KILL(sail_int)(&zgsz3543);
    }
    {
      sail_int zgsz3547;
      CREATE(sail_int)(&zgsz3547);
      CONVERT_OF(sail_int, mach_int)(&zgsz3547, zgaz3269);
      sail_int zgsz3546;
      CREATE(sail_int)(&zgsz3546);
      CONVERT_OF(sail_int, mach_int)(&zgsz3546, zE);
      zgaz3270 = gteq(zgsz3546, zgsz3547);
      KILL(sail_int)(&zgsz3547);
      KILL(sail_int)(&zgsz3546);
    }
  
  }
  if (zgaz3270) {  zcbz350 = true;  } else {
  int64_t zi_top;
  {
    uint64_t zgaz3281;
    {
      int64_t zgaz3280;
      {
        sail_int zgsz3548;
        CREATE(sail_int)(&zgsz3548);
        CONVERT_OF(sail_int, mach_int)(&zgsz3548, zE);
        sail_int zgsz3549;
        CREATE(sail_int)(&zgsz3549);
        CONVERT_OF(sail_int, mach_int)(&zgsz3549, zmantissa_width);
        sail_int zgsz3550;
        CREATE(sail_int)(&zgsz3550);
        add_int(&zgsz3550, zgsz3548, zgsz3549);
        zgaz3280 = CONVERT_OF(mach_int, sail_int)(zgsz3550);
        KILL(sail_int)(&zgsz3550);
        KILL(sail_int)(&zgsz3549);
        KILL(sail_int)(&zgsz3548);
      }
      {
        lbits zgsz3551;
        CREATE(lbits)(&zgsz3551);
        CONVERT_OF(lbits, fbits)(&zgsz3551, zi, UINT64_C(64) , true);
        sail_int zgsz3552;
        CREATE(sail_int)(&zgsz3552);
        CONVERT_OF(sail_int, mach_int)(&zgsz3552, zgaz3280);
        lbits zgsz3553;
        CREATE(lbits)(&zgsz3553);
        arith_shiftr(&zgsz3553, zgsz3551, zgsz3552);
        zgaz3281 = CONVERT_OF(fbits, lbits)(zgsz3553, true);
        KILL(lbits)(&zgsz3553);
        KILL(sail_int)(&zgsz3552);
        KILL(lbits)(&zgsz3551);
      }
    
    }
    zi_top = fast_signed(zgaz3281, 64);
  
  }
  uint64_t zi_mid;
  {
    uint64_t zgaz3279;
    {
      lbits zgsz3554;
      CREATE(lbits)(&zgsz3554);
      CONVERT_OF(lbits, fbits)(&zgsz3554, zi, UINT64_C(64) , true);
      sail_int zgsz3555;
      CREATE(sail_int)(&zgsz3555);
      CONVERT_OF(sail_int, mach_int)(&zgsz3555, zE);
      lbits zgsz3556;
      CREATE(lbits)(&zgsz3556);
      shiftr(&zgsz3556, zgsz3554, zgsz3555);
      zgaz3279 = CONVERT_OF(fbits, lbits)(zgsz3556, true);
      KILL(lbits)(&zgsz3556);
      KILL(sail_int)(&zgsz3555);
      KILL(lbits)(&zgsz3554);
    }
    {
      lbits zgsz3557;
      CREATE(lbits)(&zgsz3557);
      CONVERT_OF(lbits, fbits)(&zgsz3557, zgaz3279, UINT64_C(64) , true);
      sail_int zgsz3558;
      CREATE(sail_int)(&zgsz3558);
      CONVERT_OF(sail_int, mach_int)(&zgsz3558, zmantissa_width);
      lbits zgsz3559;
      CREATE(lbits)(&zgsz3559);
      sail_truncate(&zgsz3559, zgsz3557, zgsz3558);
      zi_mid = CONVERT_OF(fbits, lbits)(zgsz3559, true);
      KILL(lbits)(&zgsz3559);
      KILL(sail_int)(&zgsz3558);
      KILL(lbits)(&zgsz3557);
    }
  
  }
  uint64_t za_mid;
  {
    uint64_t zgaz3278;
    {
      uint64_t zgaz3277;
      zgaz3277 = zc.zaddress;
      {
        lbits zgsz3560;
        CREATE(lbits)(&zgsz3560);
        CONVERT_OF(lbits, fbits)(&zgsz3560, zgaz3277, UINT64_C(64) , true);
        sail_int zgsz3561;
        CREATE(sail_int)(&zgsz3561);
        CONVERT_OF(sail_int, mach_int)(&zgsz3561, zE);
        lbits zgsz3562;
        CREATE(lbits)(&zgsz3562);
        shiftr(&zgsz3562, zgsz3560, zgsz3561);
        zgaz3278 = CONVERT_OF(fbits, lbits)(zgsz3562, true);
        KILL(lbits)(&zgsz3562);
        KILL(sail_int)(&zgsz3561);
        KILL(lbits)(&zgsz3560);
      }
    
    }
    {
      lbits zgsz3563;
      CREATE(lbits)(&zgsz3563);
      CONVERT_OF(lbits, fbits)(&zgsz3563, zgaz3278, UINT64_C(64) , true);
      sail_int zgsz3564;
      CREATE(sail_int)(&zgsz3564);
      CONVERT_OF(sail_int, mach_int)(&zgsz3564, zmantissa_width);
      lbits zgsz3565;
      CREATE(lbits)(&zgsz3565);
      sail_truncate(&zgsz3565, zgsz3563, zgsz3564);
      za_mid = CONVERT_OF(fbits, lbits)(zgsz3565, true);
      KILL(lbits)(&zgsz3565);
      KILL(sail_int)(&zgsz3564);
      KILL(lbits)(&zgsz3563);
    }
  
  }
  uint64_t zB3;
  {
    uint64_t zgaz3276;
    zgaz3276 = zc.zB;
    {
      lbits zgsz3566;
      CREATE(lbits)(&zgsz3566);
      CONVERT_OF(lbits, fbits)(&zgsz3566, zgaz3276, UINT64_C(14) , true);
      sail_int zgsz3567;
      CREATE(sail_int)(&zgsz3567);
      CONVERT_OF(sail_int, mach_int)(&zgsz3567, INT64_C(3));
      lbits zgsz3568;
      CREATE(lbits)(&zgsz3568);
      sail_truncateLSB(&zgsz3568, zgsz3566, zgsz3567);
      zB3 = CONVERT_OF(fbits, lbits)(zgsz3568, true);
      KILL(lbits)(&zgsz3568);
      KILL(sail_int)(&zgsz3567);
      KILL(lbits)(&zgsz3566);
    }
  
  }
  uint64_t zR3;
  {
    lbits zgsz3569;
    CREATE(lbits)(&zgsz3569);
    CONVERT_OF(lbits, fbits)(&zgsz3569, zB3, UINT64_C(3) , true);
    lbits zgsz3570;
    CREATE(lbits)(&zgsz3570);
    CONVERT_OF(lbits, fbits)(&zgsz3570, UINT64_C(0b001), UINT64_C(3) , true);
    lbits zgsz3571;
    CREATE(lbits)(&zgsz3571);
    sub_bits(&zgsz3571, zgsz3569, zgsz3570);
    zR3 = CONVERT_OF(fbits, lbits)(zgsz3571, true);
    KILL(lbits)(&zgsz3571);
    KILL(lbits)(&zgsz3570);
    KILL(lbits)(&zgsz3569);
  }
  uint64_t zR;
  {
    uint64_t zgaz3275;
    {
      int64_t zgaz3274;
      {
        sail_int zgsz3572;
        CREATE(sail_int)(&zgsz3572);
        CONVERT_OF(sail_int, mach_int)(&zgsz3572, zmantissa_width);
        sail_int zgsz3573;
        CREATE(sail_int)(&zgsz3573);
        CONVERT_OF(sail_int, mach_int)(&zgsz3573, INT64_C(3));
        sail_int zgsz3574;
        CREATE(sail_int)(&zgsz3574);
        sub_int(&zgsz3574, zgsz3572, zgsz3573);
        zgaz3274 = CONVERT_OF(mach_int, sail_int)(zgsz3574);
        KILL(sail_int)(&zgsz3574);
        KILL(sail_int)(&zgsz3573);
        KILL(sail_int)(&zgsz3572);
      }
      zgaz3275 = sailgen_zzzz7nzzJzzK11z3zzeros_implicit(zgaz3274);
    
    }
    {
      lbits zgsz3575;
      CREATE(lbits)(&zgsz3575);
      CONVERT_OF(lbits, fbits)(&zgsz3575, zR3, UINT64_C(3) , true);
      lbits zgsz3576;
      CREATE(lbits)(&zgsz3576);
      CONVERT_OF(lbits, fbits)(&zgsz3576, zgaz3275, UINT64_C(11) , true);
      lbits zgsz3577;
      CREATE(lbits)(&zgsz3577);
      append(&zgsz3577, zgsz3575, zgsz3576);
      zR = CONVERT_OF(fbits, lbits)(zgsz3577, true);
      KILL(lbits)(&zgsz3577);
      KILL(lbits)(&zgsz3576);
      KILL(lbits)(&zgsz3575);
    }
  
  }
  uint64_t zdiff;
  {
    lbits zgsz3578;
    CREATE(lbits)(&zgsz3578);
    CONVERT_OF(lbits, fbits)(&zgsz3578, zR, UINT64_C(14) , true);
    lbits zgsz3579;
    CREATE(lbits)(&zgsz3579);
    CONVERT_OF(lbits, fbits)(&zgsz3579, za_mid, UINT64_C(14) , true);
    lbits zgsz3580;
    CREATE(lbits)(&zgsz3580);
    sub_bits(&zgsz3580, zgsz3578, zgsz3579);
    zdiff = CONVERT_OF(fbits, lbits)(zgsz3580, true);
    KILL(lbits)(&zgsz3580);
    KILL(lbits)(&zgsz3579);
    KILL(lbits)(&zgsz3578);
  }
  uint64_t zdiff1;
  {
    lbits zgsz3581;
    CREATE(lbits)(&zgsz3581);
    CONVERT_OF(lbits, fbits)(&zgsz3581, zdiff, UINT64_C(14) , true);
    sail_int zgsz3582;
    CREATE(sail_int)(&zgsz3582);
    CONVERT_OF(sail_int, mach_int)(&zgsz3582, INT64_C(1));
    lbits zgsz3583;
    CREATE(lbits)(&zgsz3583);
    sub_bits_int(&zgsz3583, zgsz3581, zgsz3582);
    zdiff1 = CONVERT_OF(fbits, lbits)(zgsz3583, true);
    KILL(lbits)(&zgsz3583);
    KILL(sail_int)(&zgsz3582);
    KILL(lbits)(&zgsz3581);
  }
  bool zgaz3271;
  {
    sail_int zgsz3585;
    CREATE(sail_int)(&zgsz3585);
    CONVERT_OF(sail_int, mach_int)(&zgsz3585, INT64_C(0));
    sail_int zgsz3584;
    CREATE(sail_int)(&zgsz3584);
    CONVERT_OF(sail_int, mach_int)(&zgsz3584, zi_top);
    zgaz3271 = eq_int(zgsz3584, zgsz3585);
    KILL(sail_int)(&zgsz3585);
    KILL(sail_int)(&zgsz3584);
  }
  if (zgaz3271) {  zcbz350 = sailgen_z8operatorz0zzzz7nzzJzzK14z3zI_uz9(zi_mid, zdiff1);  } else {
  bool zgaz3272;
  {
    sail_int zgsz3587;
    CREATE(sail_int)(&zgsz3587);
    CONVERT_OF(sail_int, mach_int)(&zgsz3587, INT64_C(-1));
    sail_int zgsz3586;
    CREATE(sail_int)(&zgsz3586);
    CONVERT_OF(sail_int, mach_int)(&zgsz3586, zi_top);
    zgaz3272 = eq_int(zgsz3586, zgsz3587);
    KILL(sail_int)(&zgsz3587);
    KILL(sail_int)(&zgsz3586);
  }
  if (zgaz3272) {
  bool zgaz3273;
  zgaz3273 = sailgen_z8operatorz0zzzz7nzzJzzK14z3zKzJ_uz9(zi_mid, zdiff);
  bool zgsz3588;
  if (zgaz3273) {
  {
    lbits zgsz3590;
    CREATE(lbits)(&zgsz3590);
    CONVERT_OF(lbits, fbits)(&zgsz3590, za_mid, UINT64_C(14) , true);
    lbits zgsz3589;
    CREATE(lbits)(&zgsz3589);
    CONVERT_OF(lbits, fbits)(&zgsz3589, zR, UINT64_C(14) , true);
    zgsz3588 = neq_bits(zgsz3589, zgsz3590);
    KILL(lbits)(&zgsz3590);
    KILL(lbits)(&zgsz3589);
  }
  } else {  zgsz3588 = false;  }
  zcbz350 = zgsz3588;

  } else {  zcbz350 = false;  }

  }









  }


end_function_191: ;
  return zcbz350;
end_block_exception_192: ;

  return false;
}

static void sailgen_capToString(sail_string *rop, struct zCapability);

static void sailgen_capToString(sail_string *zcbz351, struct zCapability zcap)
{
  __label__ end_function_194, end_block_exception_195, end_function_206;

  sail_int zlen;
  CREATE(sail_int)(&zlen);
  sailgen_getCapLength(&zlen, zcap);
  if (have_exception) {
  KILL(sail_int)(&zlen);
  goto end_block_exception_195;
  }
  sail_string zlen_str;
  CREATE(sail_string)(&zlen_str);
  {
    lbits zgaz3311;
    CREATE(lbits)(&zgaz3311);
    {
      int64_t zgaz3310;
      {
        sail_int zgsz3591;
        CREATE(sail_int)(&zgsz3591);
        CONVERT_OF(sail_int, mach_int)(&zgsz3591, zcap_len_width);
        sail_int zgsz3592;
        CREATE(sail_int)(&zgsz3592);
        CONVERT_OF(sail_int, mach_int)(&zgsz3592, INT64_C(3));
        sail_int zgsz3593;
        CREATE(sail_int)(&zgsz3593);
        add_int(&zgsz3593, zgsz3591, zgsz3592);
        zgaz3310 = CONVERT_OF(mach_int, sail_int)(zgsz3593);
        KILL(sail_int)(&zgsz3593);
        KILL(sail_int)(&zgsz3592);
        KILL(sail_int)(&zgsz3591);
      }
      sailgen_zzzz7lzzJzzK68z3to_bits(&zgaz3311, zgaz3310, zlen);
    
    }
    string_of_lbits(&zlen_str, zgaz3311);
    KILL(lbits)(&zgaz3311);
  }
  uint64_t zotype64;
  {
    bool zgaz3307;
    zgaz3307 = sailgen_hasReservedOType(zcap);
    if (zgaz3307) {
    uint64_t zgaz3308;
    zgaz3308 = zcap.zotype;
    zotype64 = sailgen_zzzz7mzzJzzK64zzCzz0zz7nzzJzzK18z3EXTS(INT64_C(64), zgaz3308);
  
    } else {
    uint64_t zgaz3309;
    zgaz3309 = zcap.zotype;
    zotype64 = sailgen_zzzz7mzzJzzK64zzCzz0zz7nzzJzzK18z3EXTZ(INT64_C(64), zgaz3309);
  
    }
  
  }
  sail_string zgaz3306;
  CREATE(sail_string)(&zgaz3306);
  {
    sail_string zgaz3304;
    CREATE(sail_string)(&zgaz3304);
    {
      bool zgaz3283;
      zgaz3283 = zcap.ztag;
      if (zgaz3283) {  COPY(sail_string)(&zgaz3304, "1");  } else {  COPY(sail_string)(&zgaz3304, "0");  }
    
    }
    sail_string zgaz3305;
    CREATE(sail_string)(&zgaz3305);
    {
      sail_string zgaz3303;
      CREATE(sail_string)(&zgaz3303);
      {
        sail_string zgaz3301;
        CREATE(sail_string)(&zgaz3301);
        {
          bool zgaz3284;
          zgaz3284 = zcap.zsealed;
          if (zgaz3284) {  COPY(sail_string)(&zgaz3301, "1");  } else {  COPY(sail_string)(&zgaz3301, "0");  }
        
        }
        sail_string zgaz3302;
        CREATE(sail_string)(&zgaz3302);
        {
          sail_string zgaz3300;
          CREATE(sail_string)(&zgaz3300);
          {
            sail_string zgaz3298;
            CREATE(sail_string)(&zgaz3298);
            {
              uint64_t zgaz3286;
              {
                uint64_t zgaz3285;
                zgaz3285 = sailgen_getCapPerms(zcap);
                {
                  lbits zgsz3594;
                  CREATE(lbits)(&zgsz3594);
                  CONVERT_OF(lbits, fbits)(&zgsz3594, UINT64_C(0b0), UINT64_C(1) , true);
                  lbits zgsz3595;
                  CREATE(lbits)(&zgsz3595);
                  CONVERT_OF(lbits, fbits)(&zgsz3595, zgaz3285, UINT64_C(31) , true);
                  lbits zgsz3596;
                  CREATE(lbits)(&zgsz3596);
                  append(&zgsz3596, zgsz3594, zgsz3595);
                  zgaz3286 = CONVERT_OF(fbits, lbits)(zgsz3596, true);
                  KILL(lbits)(&zgsz3596);
                  KILL(lbits)(&zgsz3595);
                  KILL(lbits)(&zgsz3594);
                }
              
              }
              {
                lbits zgsz3597;
                CREATE(lbits)(&zgsz3597);
                CONVERT_OF(lbits, fbits)(&zgsz3597, zgaz3286, UINT64_C(32) , true);
                string_of_lbits(&zgaz3298, zgsz3597);
                KILL(lbits)(&zgsz3597);
              }
            
            }
            sail_string zgaz3299;
            CREATE(sail_string)(&zgaz3299);
            {
              sail_string zgaz3297;
              CREATE(sail_string)(&zgaz3297);
              {
                sail_string zgaz3295;
                CREATE(sail_string)(&zgaz3295);
                {
                  lbits zgsz3598;
                  CREATE(lbits)(&zgsz3598);
                  CONVERT_OF(lbits, fbits)(&zgsz3598, zotype64, UINT64_C(64) , true);
                  string_of_lbits(&zgaz3295, zgsz3598);
                  KILL(lbits)(&zgsz3598);
                }
                sail_string zgaz3296;
                CREATE(sail_string)(&zgaz3296);
                {
                  sail_string zgaz3294;
                  CREATE(sail_string)(&zgaz3294);
                  {
                    sail_string zgaz3292;
                    CREATE(sail_string)(&zgaz3292);
                    {
                      uint64_t zgaz3287;
                      zgaz3287 = sailgen_getCapOffsetBits(zcap);
                      {
                        lbits zgsz3599;
                        CREATE(lbits)(&zgsz3599);
                        CONVERT_OF(lbits, fbits)(&zgsz3599, zgaz3287, UINT64_C(64) , true);
                        string_of_lbits(&zgaz3292, zgsz3599);
                        KILL(lbits)(&zgsz3599);
                      }
                    
                    }
                    sail_string zgaz3293;
                    CREATE(sail_string)(&zgaz3293);
                    {
                      sail_string zgaz3291;
                      CREATE(sail_string)(&zgaz3291);
                      {
                        sail_string zgaz3289;
                        CREATE(sail_string)(&zgaz3289);
                        {
                          uint64_t zgaz3288;
                          zgaz3288 = sailgen_getCapBaseBits(zcap);
                          {
                            lbits zgsz3600;
                            CREATE(lbits)(&zgsz3600);
                            CONVERT_OF(lbits, fbits)(&zgsz3600, zgaz3288, UINT64_C(64) , true);
                            string_of_lbits(&zgaz3289, zgsz3600);
                            KILL(lbits)(&zgsz3600);
                          }
                        
                        }
                        sail_string zgaz3290;
                        CREATE(sail_string)(&zgaz3290);
                        concat_str(&zgaz3290, " length:", zlen_str);
                        concat_str(&zgaz3291, zgaz3289, zgaz3290);
                        KILL(sail_string)(&zgaz3290);
                        KILL(sail_string)(&zgaz3289);
                      }
                      concat_str(&zgaz3293, " base:", zgaz3291);
                      KILL(sail_string)(&zgaz3291);
                    }
                    concat_str(&zgaz3294, zgaz3292, zgaz3293);
                    KILL(sail_string)(&zgaz3293);
                    KILL(sail_string)(&zgaz3292);
                  }
                  concat_str(&zgaz3296, " offset:", zgaz3294);
                  KILL(sail_string)(&zgaz3294);
                }
                concat_str(&zgaz3297, zgaz3295, zgaz3296);
                KILL(sail_string)(&zgaz3296);
                KILL(sail_string)(&zgaz3295);
              }
              concat_str(&zgaz3299, " type:", zgaz3297);
              KILL(sail_string)(&zgaz3297);
            }
            concat_str(&zgaz3300, zgaz3298, zgaz3299);
            KILL(sail_string)(&zgaz3299);
            KILL(sail_string)(&zgaz3298);
          }
          concat_str(&zgaz3302, " perms:", zgaz3300);
          KILL(sail_string)(&zgaz3300);
        }
        concat_str(&zgaz3303, zgaz3301, zgaz3302);
        KILL(sail_string)(&zgaz3302);
        KILL(sail_string)(&zgaz3301);
      }
      concat_str(&zgaz3305, " s:", zgaz3303);
      KILL(sail_string)(&zgaz3303);
    }
    concat_str(&zgaz3306, zgaz3304, zgaz3305);
    KILL(sail_string)(&zgaz3305);
    KILL(sail_string)(&zgaz3304);
  }
  concat_str((*(&zcbz351)), " t:", zgaz3306);
  KILL(sail_string)(&zgaz3306);

  KILL(sail_string)(&zlen_str);
  KILL(sail_int)(&zlen);
end_function_194: ;
  goto end_function_206;
end_block_exception_195: ;
  goto end_function_206;
end_function_206: ;
}

// register TestCap
static struct zCapability zTestCap;

// register TestAddr
static uint64_t zTestAddr;

// register TestLen
static lbits zTestLen;

static unit sailgen_main(unit);


static unit sailgen_main(unit zgsz3601)
{
  __label__ end_function_201, end_block_exception_202;

  unit zcbz352;
  {
    zTestCap = znull_cap;
    unit zgsz3602;
    zgsz3602 = UNIT;
  }
  lbits zbits1;
  CREATE(lbits)(&zbits1);
  sailgen_capToBits(&zbits1, zTestCap);
  lbits zbits2;
  CREATE(lbits)(&zbits2);
  sailgen_capToMemBits(&zbits2, zTestCap);
  struct zCapability zcap1;
  {
    bool zgaz3316;
    zgaz3316 = zTestCap.ztag;
    zcap1 = sailgen_capBitsToCapability(zgaz3316, zbits1);
  
  }
  struct zCapability zcap2;
  {
    bool zgaz3315;
    zgaz3315 = zTestCap.ztag;
    zcap2 = sailgen_memBitsToCapability(zgaz3315, zbits2);
  
  }
  bool zrep;
  zrep = sailgen_fastRepCheck(zTestCap, zTestAddr);
  {
    __label__ case_198, finish_match_197;

    struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3312;
    zgaz3312 = sailgen_setCapBounds(zTestCap, zTestAddr, zTestLen);
    unit zgsz3603;
    {
      bool ztup__0;
      ztup__0 = zgaz3312.ztup0;
      struct zCapability ztup__1;
      ztup__1 = zgaz3312.ztup1;
      bool zexact;
      zexact = ztup__0;
      struct zCapability zcap;
      zcap = ztup__1;
      zgsz3603 = UNIT;
    
    
    
    
      goto finish_match_197;
    }
  case_198: ;
    sail_match_failure("main");
  finish_match_197: ;
    unit zgsz3608;
    zgsz3608 = zgsz3603;
  
  
  }
  {
    __label__ case_200, finish_match_199;

    struct ztuple_z8z5izCz0z5iz9 zgaz3313;
    CREATE(ztuple_z8z5izCz0z5iz9)(&zgaz3313);
    sailgen_getCapBounds(&zgaz3313, zTestCap);
    unit zgsz3605;
    {
      sail_int ztup__0;
      CREATE(sail_int)(&ztup__0);
      COPY(sail_int)(&ztup__0, zgaz3313.ztup0);
      sail_int ztup__1;
      CREATE(sail_int)(&ztup__1);
      COPY(sail_int)(&ztup__1, zgaz3313.ztup1);
      sail_int zbase;
      CREATE(sail_int)(&zbase);
      COPY(sail_int)(&zbase, ztup__0);
      sail_int ztop;
      CREATE(sail_int)(&ztop);
      COPY(sail_int)(&ztop, ztup__1);
      zgsz3605 = UNIT;
      KILL(sail_int)(&ztop);
      KILL(sail_int)(&zbase);
      KILL(sail_int)(&ztup__1);
      KILL(sail_int)(&ztup__0);
      goto finish_match_199;
    }
  case_200: ;
    sail_match_failure("main");
  finish_match_199: ;
    unit zgsz3607;
    zgsz3607 = zgsz3605;
  
    KILL(ztuple_z8z5izCz0z5iz9)(&zgaz3313);
  }
  uint64_t zflags;
  zflags = sailgen_getCapFlags(zTestCap);
  struct zCapability znew_cap;
  {
    uint64_t zgaz3314;
    zgaz3314 = sailgen_zzzz7nzzJzzK31z3ones(INT64_C(31));
    znew_cap = sailgen_setCapPerms(zTestCap, zgaz3314);
  
  }
  sail_string zcap_str;
  CREATE(sail_string)(&zcap_str);
  sailgen_capToString(&zcap_str, zTestCap);
  if (have_exception) {
  KILL(lbits)(&zbits1);
  KILL(lbits)(&zbits2);





  KILL(sail_string)(&zcap_str);
  goto end_block_exception_202;
  }
  zcbz352 = UNIT;
  KILL(sail_string)(&zcap_str);





  KILL(lbits)(&zbits2);
  KILL(lbits)(&zbits1);
end_function_201: ;
  return zcbz352;
end_block_exception_202: ;

  return UNIT;
}

static unit sailgen_initializze_registers(unit);

static unit sailgen_initializze_registers(unit zgsz3609)
{
  __label__ end_function_204, end_block_exception_205;

  unit zcbz353;
  {
    zTestCap = sailgen_undefined_Capability(UNIT);
    unit zgsz3613;
    zgsz3613 = UNIT;
  }
  {
    {
      sail_int zgsz3610;
      CREATE(sail_int)(&zgsz3610);
      CONVERT_OF(sail_int, mach_int)(&zgsz3610, INT64_C(64));
      lbits zgsz3611;
      CREATE(lbits)(&zgsz3611);
      UNDEFINED(lbits)(&zgsz3611, zgsz3610);
      zTestAddr = CONVERT_OF(fbits, lbits)(zgsz3611, true);
      KILL(lbits)(&zgsz3611);
      KILL(sail_int)(&zgsz3610);
    }
    unit zgsz3612;
    zgsz3612 = UNIT;
  }
  int64_t zgaz3317;
  {
    sail_int zgsz3614;
    CREATE(sail_int)(&zgsz3614);
    CONVERT_OF(sail_int, mach_int)(&zgsz3614, INT64_C(64));
    sail_int zgsz3615;
    CREATE(sail_int)(&zgsz3615);
    CONVERT_OF(sail_int, mach_int)(&zgsz3615, INT64_C(1));
    sail_int zgsz3616;
    CREATE(sail_int)(&zgsz3616);
    add_int(&zgsz3616, zgsz3614, zgsz3615);
    zgaz3317 = CONVERT_OF(mach_int, sail_int)(zgsz3616);
    KILL(sail_int)(&zgsz3616);
    KILL(sail_int)(&zgsz3615);
    KILL(sail_int)(&zgsz3614);
  }
  {
    sail_int zgsz3617;
    CREATE(sail_int)(&zgsz3617);
    CONVERT_OF(sail_int, mach_int)(&zgsz3617, zgaz3317);
    UNDEFINED(lbits)(&zTestLen, zgsz3617);
    KILL(sail_int)(&zgsz3617);
  }
  zcbz353 = UNIT;

end_function_204: ;
  return zcbz353;
end_block_exception_205: ;

  return UNIT;
}

static void model_init(void)
{
  setup_rts();
  CREATE(lbits)(&zTestLen);
  sailgen_initializze_registers(UNIT);
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
}

static void model_fini(void)
{
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
  cleanup_rts();
}

static int model_main(int argc, char *argv[])
{
  model_init();
  if (process_arguments(argc, argv)) exit(EXIT_FAILURE);
  sailgen_main(UNIT);
  model_fini();
  return EXIT_SUCCESS;
}


