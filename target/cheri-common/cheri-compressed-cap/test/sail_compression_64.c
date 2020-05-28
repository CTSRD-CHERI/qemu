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

































static uint64_t sailgen_zzzz7nzzJzzK0z3sail_ones(int64_t);

static uint64_t sailgen_zzzz7nzzJzzK31z3sail_ones(int64_t);

static void sailgen_zzz3zzz3sail_ones(lbits *rop, sail_int);

static uint64_t sailgen_zzzz7nzzJzzK0z3sail_ones(int64_t zn)
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
    CONVERT_OF(lbits, fbits)(&zgsz33, zgaz32, UINT64_C(0) , true);
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
  __label__ end_function_13, end_block_exception_14, end_function_209;

  lbits zgaz34;
  CREATE(lbits)(&zgaz34);
  zeros(&zgaz34, zn);
  not_bits((*(&zcbz34)), zgaz34);
  KILL(lbits)(&zgaz34);
end_function_13: ;
  goto end_function_209;
end_block_exception_14: ;
  goto end_function_209;
end_function_209: ;
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













static uint64_t sailgen_zzzz7mzzJzzK32zzCzz0zz7nzzJzzK4z3EXTS(int64_t, uint64_t);

static void sailgen_zzz3zzz3EXTZ(lbits *rop, sail_int, lbits);

static uint64_t sailgen_zzzz7mzzJzzK15zzCzz0zz7nzzJzzK12z3EXTZ(int64_t, uint64_t);

static uint64_t sailgen_zzz3zzzz7mzzJzzK31z3EXTZ(int64_t, sbits);

static uint64_t sailgen_zzzz7mzzJzzK32zzCzz0zz7nzzJzzK4z3EXTZ(int64_t, uint64_t);

static uint64_t sailgen_zzzz7mzzJzzK32zzCzz0zz7nzzJzzK4z3EXTS(int64_t zm, uint64_t zv)
{
  __label__ end_function_19, end_block_exception_20;

  uint64_t zcbz36;
  {
    lbits zgsz39;
    CREATE(lbits)(&zgsz39);
    CONVERT_OF(lbits, fbits)(&zgsz39, zv, UINT64_C(4) , true);
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
  __label__ end_function_22, end_block_exception_23, end_function_208;

  zero_extend((*(&zcbz37)), zv, zm);
end_function_22: ;
  goto end_function_208;
end_block_exception_23: ;
  goto end_function_208;
end_function_208: ;
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

static uint64_t sailgen_zzzz7mzzJzzK32zzCzz0zz7nzzJzzK4z3EXTZ(int64_t zm, uint64_t zv)
{
  __label__ end_function_31, end_block_exception_32;

  uint64_t zcbz310;
  {
    lbits zgsz318;
    CREATE(lbits)(&zgsz318);
    CONVERT_OF(lbits, fbits)(&zgsz318, zv, UINT64_C(4) , true);
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

static uint64_t sailgen_zzzz7nzzJzzK32z3zzeros_implicit(int64_t);

static uint64_t sailgen_zzzz7nzzJzzK8z3zzeros_implicit(int64_t);

static uint64_t sailgen_zzzz7nzzJzzK0z3zzeros_implicit(int64_t);

static uint64_t sailgen_zzzz7nzzJzzK6z3zzeros_implicit(int64_t);

static uint64_t sailgen_zzzz7nzzJzzK2z3zzeros_implicit(int64_t);

static void sailgen_zzz3zzz3zzeros_implicit(lbits *rop, sail_int);

static uint64_t sailgen_zzzz7nzzJzzK5z3zzeros_implicit(int64_t);

static uint64_t sailgen_zzzz7nzzJzzK32z3zzeros_implicit(int64_t zn)
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

static uint64_t sailgen_zzzz7nzzJzzK8z3zzeros_implicit(int64_t zn)
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

static uint64_t sailgen_zzzz7nzzJzzK0z3zzeros_implicit(int64_t zn)
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

static uint64_t sailgen_zzzz7nzzJzzK6z3zzeros_implicit(int64_t zn)
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

static uint64_t sailgen_zzzz7nzzJzzK2z3zzeros_implicit(int64_t zn)
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

static void sailgen_zzz3zzz3zzeros_implicit(lbits *zcbz316, sail_int zn)
{
  __label__ end_function_49, end_block_exception_50, end_function_207;

  zeros((*(&zcbz316)), zn);
end_function_49: ;
  goto end_function_207;
end_block_exception_50: ;
  goto end_function_207;
end_function_207: ;
}

static uint64_t sailgen_zzzz7nzzJzzK5z3zzeros_implicit(int64_t zn)
{
  __label__ end_function_52, end_block_exception_53;

  uint64_t zcbz317;
  {
    sail_int zgsz331;
    CREATE(sail_int)(&zgsz331);
    CONVERT_OF(sail_int, mach_int)(&zgsz331, zn);
    lbits zgsz332;
    CREATE(lbits)(&zgsz332);
    zeros(&zgsz332, zgsz331);
    zcbz317 = CONVERT_OF(fbits, lbits)(zgsz332, true);
    KILL(lbits)(&zgsz332);
    KILL(sail_int)(&zgsz331);
  }
end_function_52: ;
  return zcbz317;
end_block_exception_53: ;

  return UINT64_C(0xdeadc0de);
}

static uint64_t sailgen_zzzz7nzzJzzK0z3ones(int64_t);

static void sailgen_zzz3zzz3ones(lbits *rop, sail_int);

static uint64_t sailgen_zzzz7nzzJzzK31z3ones(int64_t);

static uint64_t sailgen_zzzz7nzzJzzK0z3ones(int64_t zn)
{
  __label__ end_function_55, end_block_exception_56;

  uint64_t zcbz318;
  zcbz318 = sailgen_zzzz7nzzJzzK0z3sail_ones(zn);
end_function_55: ;
  return zcbz318;
end_block_exception_56: ;

  return UINT64_C(0xdeadc0de);
}

static void sailgen_zzz3zzz3ones(lbits *zcbz319, sail_int zn)
{
  __label__ end_function_58, end_block_exception_59, end_function_206;

  sailgen_zzz3zzz3sail_ones((*(&zcbz319)), zn);
end_function_58: ;
  goto end_function_206;
end_block_exception_59: ;
  goto end_function_206;
end_function_206: ;
}

static uint64_t sailgen_zzzz7nzzJzzK31z3ones(int64_t zn)
{
  __label__ end_function_61, end_block_exception_62;

  uint64_t zcbz320;
  zcbz320 = sailgen_zzzz7nzzJzzK31z3sail_ones(zn);
end_function_61: ;
  return zcbz320;
end_block_exception_62: ;

  return UINT64_C(0xdeadc0de);
}

static uint64_t sailgen_bool_to_bits(bool);

static uint64_t sailgen_bool_to_bits(bool zx)
{
  __label__ end_function_64, end_block_exception_65;

  uint64_t zcbz321;
  if (zx) {  zcbz321 = UINT64_C(0b1);  } else {  zcbz321 = UINT64_C(0b0);  }
end_function_64: ;
  return zcbz321;
end_block_exception_65: ;

  return UINT64_C(0xdeadc0de);
}

static bool sailgen_bit_to_bool(fbits);

static bool sailgen_bit_to_bool(fbits zb)
{
  __label__ case_68, case_69, finish_match_67, end_function_70, end_block_exception_71;

  bool zcbz322;
  bool zgsz333;
  {
    fbits zp0z3;
    zp0z3 = zb;
    bool zgsz334;
    zgsz334 = eq_bit(zp0z3, UINT64_C(1));
    if (!(zgsz334)) {
  
    goto case_68;
    }
    zgsz333 = true;
  
    goto finish_match_67;
  }
case_68: ;
  {
    fbits zp0z3;
    zp0z3 = zb;
    bool zgsz335;
    zgsz335 = eq_bit(zp0z3, UINT64_C(0));
    if (!(zgsz335)) {
  
    goto case_69;
    }
    zgsz333 = false;
  
    goto finish_match_67;
  }
case_69: ;
  sail_match_failure("bit_to_bool");
finish_match_67: ;
  zcbz322 = zgsz333;

end_function_70: ;
  return zcbz322;
end_block_exception_71: ;

  return false;
}

static void sailgen_to_bits(lbits *rop, sail_int, sail_int);

static uint64_t sailgen_zzzz7lzzJzzK6z3to_bits(int64_t, sail_int);

static uint64_t sailgen_zzzz7lzzJzzK36z3to_bits(int64_t, sail_int);

static void sailgen_to_bits(lbits *zcbz323, sail_int zl, sail_int zn)
{
  __label__ end_function_73, end_block_exception_74, end_function_205;

  {
    sail_int zgsz336;
    CREATE(sail_int)(&zgsz336);
    CONVERT_OF(sail_int, mach_int)(&zgsz336, INT64_C(0));
    get_slice_int((*(&zcbz323)), zl, zn, zgsz336);
    KILL(sail_int)(&zgsz336);
  }
end_function_73: ;
  goto end_function_205;
end_block_exception_74: ;
  goto end_function_205;
end_function_205: ;
}

static uint64_t sailgen_zzzz7lzzJzzK6z3to_bits(int64_t zl, sail_int zn)
{
  __label__ end_function_76, end_block_exception_77;

  uint64_t zcbz324;
  {
    sail_int zgsz337;
    CREATE(sail_int)(&zgsz337);
    CONVERT_OF(sail_int, mach_int)(&zgsz337, zl);
    sail_int zgsz338;
    CREATE(sail_int)(&zgsz338);
    CONVERT_OF(sail_int, mach_int)(&zgsz338, INT64_C(0));
    lbits zgsz339;
    CREATE(lbits)(&zgsz339);
    get_slice_int(&zgsz339, zgsz337, zn, zgsz338);
    zcbz324 = CONVERT_OF(fbits, lbits)(zgsz339, true);
    KILL(lbits)(&zgsz339);
    KILL(sail_int)(&zgsz338);
    KILL(sail_int)(&zgsz337);
  }
end_function_76: ;
  return zcbz324;
end_block_exception_77: ;

  return UINT64_C(0xdeadc0de);
}

static uint64_t sailgen_zzzz7lzzJzzK36z3to_bits(int64_t zl, sail_int zn)
{
  __label__ end_function_79, end_block_exception_80;

  uint64_t zcbz325;
  {
    sail_int zgsz340;
    CREATE(sail_int)(&zgsz340);
    CONVERT_OF(sail_int, mach_int)(&zgsz340, zl);
    sail_int zgsz341;
    CREATE(sail_int)(&zgsz341);
    CONVERT_OF(sail_int, mach_int)(&zgsz341, INT64_C(0));
    lbits zgsz342;
    CREATE(lbits)(&zgsz342);
    get_slice_int(&zgsz342, zgsz340, zn, zgsz341);
    zcbz325 = CONVERT_OF(fbits, lbits)(zgsz342, true);
    KILL(lbits)(&zgsz342);
    KILL(sail_int)(&zgsz341);
    KILL(sail_int)(&zgsz340);
  }
end_function_79: ;
  return zcbz325;
end_block_exception_80: ;

  return UINT64_C(0xdeadc0de);
}

static bool sailgen_z8operatorz0zzzz7nzzJzzK6z3zI_uz9(uint64_t, uint64_t);

static bool sailgen_z8operatorz0zzzz7nzzJzzK3z3zI_uz9(uint64_t, uint64_t);

static bool sailgen_z8operatorz0zzzz7nzzJzzK8z3zI_uz9(uint64_t, uint64_t);

static bool sailgen_z8operatorz0zzzz7nzzJzzK8z3zKzJ_uz9(uint64_t, uint64_t);

static bool sailgen_z8operatorz0zzzz7nzzJzzK6z3zI_uz9(uint64_t zx, uint64_t zy)
{
  __label__ end_function_82, end_block_exception_83;

  bool zcbz326;
  int64_t zgaz36;
  zgaz36 = ((mach_int) zx);
  int64_t zgaz37;
  zgaz37 = ((mach_int) zy);
  {
    sail_int zgsz344;
    CREATE(sail_int)(&zgsz344);
    CONVERT_OF(sail_int, mach_int)(&zgsz344, zgaz37);
    sail_int zgsz343;
    CREATE(sail_int)(&zgsz343);
    CONVERT_OF(sail_int, mach_int)(&zgsz343, zgaz36);
    zcbz326 = lt(zgsz343, zgsz344);
    KILL(sail_int)(&zgsz344);
    KILL(sail_int)(&zgsz343);
  }


end_function_82: ;
  return zcbz326;
end_block_exception_83: ;

  return false;
}

static bool sailgen_z8operatorz0zzzz7nzzJzzK3z3zI_uz9(uint64_t zx, uint64_t zy)
{
  __label__ end_function_85, end_block_exception_86;

  bool zcbz327;
  int64_t zgaz38;
  zgaz38 = ((mach_int) zx);
  int64_t zgaz39;
  zgaz39 = ((mach_int) zy);
  {
    sail_int zgsz346;
    CREATE(sail_int)(&zgsz346);
    CONVERT_OF(sail_int, mach_int)(&zgsz346, zgaz39);
    sail_int zgsz345;
    CREATE(sail_int)(&zgsz345);
    CONVERT_OF(sail_int, mach_int)(&zgsz345, zgaz38);
    zcbz327 = lt(zgsz345, zgsz346);
    KILL(sail_int)(&zgsz346);
    KILL(sail_int)(&zgsz345);
  }


end_function_85: ;
  return zcbz327;
end_block_exception_86: ;

  return false;
}

static bool sailgen_z8operatorz0zzzz7nzzJzzK8z3zI_uz9(uint64_t zx, uint64_t zy)
{
  __label__ end_function_88, end_block_exception_89;

  bool zcbz328;
  int64_t zgaz310;
  zgaz310 = ((mach_int) zx);
  int64_t zgaz311;
  zgaz311 = ((mach_int) zy);
  {
    sail_int zgsz348;
    CREATE(sail_int)(&zgsz348);
    CONVERT_OF(sail_int, mach_int)(&zgsz348, zgaz311);
    sail_int zgsz347;
    CREATE(sail_int)(&zgsz347);
    CONVERT_OF(sail_int, mach_int)(&zgsz347, zgaz310);
    zcbz328 = lt(zgsz347, zgsz348);
    KILL(sail_int)(&zgsz348);
    KILL(sail_int)(&zgsz347);
  }


end_function_88: ;
  return zcbz328;
end_block_exception_89: ;

  return false;
}

static bool sailgen_z8operatorz0zzzz7nzzJzzK8z3zKzJ_uz9(uint64_t zx, uint64_t zy)
{
  __label__ end_function_91, end_block_exception_92;

  bool zcbz329;
  int64_t zgaz312;
  zgaz312 = ((mach_int) zx);
  int64_t zgaz313;
  zgaz313 = ((mach_int) zy);
  {
    sail_int zgsz350;
    CREATE(sail_int)(&zgsz350);
    CONVERT_OF(sail_int, mach_int)(&zgsz350, zgaz313);
    sail_int zgsz349;
    CREATE(sail_int)(&zgsz349);
    CONVERT_OF(sail_int, mach_int)(&zgsz349, zgaz312);
    zcbz329 = gteq(zgsz349, zgsz350);
    KILL(sail_int)(&zgsz350);
    KILL(sail_int)(&zgsz349);
  }


end_function_91: ;
  return zcbz329;
end_block_exception_92: ;

  return false;
}



static void sailgen_n_leading_spaces(sail_int *rop, sail_string);

static void sailgen_n_leading_spaces(sail_int *zcbz330, sail_string zs)
{
  __label__ case_95, case_96, finish_match_94, end_function_100, end_block_exception_101, end_function_204;

  sail_int zgsz351;
  CREATE(sail_int)(&zgsz351);
  {
    sail_string zp0z3;
    CREATE(sail_string)(&zp0z3);
    COPY(sail_string)(&zp0z3, zs);
    bool zgsz352;
    zgsz352 = eq_string(zp0z3, "");
    if (!(zgsz352)) {
    KILL(sail_string)(&zp0z3);
    goto case_95;
    }
    CONVERT_OF(sail_int, mach_int)(&zgsz351, INT64_C(0));
    KILL(sail_string)(&zp0z3);
    goto finish_match_94;
  }
case_95: ;
  {
    __label__ case_98, case_99, finish_match_97;

    sail_string zgaz314;
    CREATE(sail_string)(&zgaz314);
    {
      sail_int zgsz353;
      CREATE(sail_int)(&zgsz353);
      CONVERT_OF(sail_int, mach_int)(&zgsz353, INT64_C(1));
      string_take(&zgaz314, zs, zgsz353);
      KILL(sail_int)(&zgsz353);
    }
    sail_int zgsz354;
    CREATE(sail_int)(&zgsz354);
    {
      sail_string zp0z3;
      CREATE(sail_string)(&zp0z3);
      COPY(sail_string)(&zp0z3, zgaz314);
      bool zgsz356;
      zgsz356 = eq_string(zp0z3, " ");
      if (!(zgsz356)) {
      KILL(sail_string)(&zp0z3);
      goto case_98;
      }
      sail_int zgaz316;
      CREATE(sail_int)(&zgaz316);
      {
        sail_string zgaz315;
        CREATE(sail_string)(&zgaz315);
        {
          sail_int zgsz355;
          CREATE(sail_int)(&zgsz355);
          CONVERT_OF(sail_int, mach_int)(&zgsz355, INT64_C(1));
          string_drop(&zgaz315, zs, zgsz355);
          KILL(sail_int)(&zgsz355);
        }
        sailgen_n_leading_spaces(&zgaz316, zgaz315);
        KILL(sail_string)(&zgaz315);
      }
      {
        sail_int zgsz357;
        CREATE(sail_int)(&zgsz357);
        CONVERT_OF(sail_int, mach_int)(&zgsz357, INT64_C(1));
        add_int(&zgsz354, zgsz357, zgaz316);
        KILL(sail_int)(&zgsz357);
      }
      KILL(sail_int)(&zgaz316);
      KILL(sail_string)(&zp0z3);
      goto finish_match_97;
    }
  case_98: ;
    {
      CONVERT_OF(sail_int, mach_int)(&zgsz354, INT64_C(0));
      goto finish_match_97;
    }
  case_99: ;
    sail_match_failure("n_leading_spaces");
  finish_match_97: ;
    COPY(sail_int)(&zgsz351, zgsz354);
    KILL(sail_int)(&zgsz354);
    KILL(sail_string)(&zgaz314);
    goto finish_match_94;
  }
case_96: ;
  sail_match_failure("n_leading_spaces");
finish_match_94: ;
  COPY(sail_int)((*(&zcbz330)), zgsz351);
  KILL(sail_int)(&zgsz351);
end_function_100: ;
  goto end_function_204;
end_block_exception_101: ;
  goto end_function_204;
end_function_204: ;
}

static int64_t sailgen_zzzz7nzzJzzK32z3MAX(int64_t);

static int64_t sailgen_zzzz7nzzJzzK4z3MAX(int64_t);

static int64_t sailgen_zzzz7nzzJzzK32z3MAX(int64_t zn)
{
  __label__ end_function_103, end_block_exception_104;

  int64_t zcbz331;
  int64_t zgaz317;
  {
    sail_int zgsz360;
    CREATE(sail_int)(&zgsz360);
    CONVERT_OF(sail_int, mach_int)(&zgsz360, zn);
    sail_int zgsz361;
    CREATE(sail_int)(&zgsz361);
    pow2(&zgsz361, zgsz360);
    zgaz317 = CONVERT_OF(mach_int, sail_int)(zgsz361);
    KILL(sail_int)(&zgsz361);
    KILL(sail_int)(&zgsz360);
  }
  {
    sail_int zgsz362;
    CREATE(sail_int)(&zgsz362);
    CONVERT_OF(sail_int, mach_int)(&zgsz362, zgaz317);
    sail_int zgsz363;
    CREATE(sail_int)(&zgsz363);
    CONVERT_OF(sail_int, mach_int)(&zgsz363, INT64_C(1));
    sail_int zgsz364;
    CREATE(sail_int)(&zgsz364);
    sub_int(&zgsz364, zgsz362, zgsz363);
    zcbz331 = CONVERT_OF(mach_int, sail_int)(zgsz364);
    KILL(sail_int)(&zgsz364);
    KILL(sail_int)(&zgsz363);
    KILL(sail_int)(&zgsz362);
  }

end_function_103: ;
  return zcbz331;
end_block_exception_104: ;

  return INT64_C(0xdeadc0de);
}

static int64_t sailgen_zzzz7nzzJzzK4z3MAX(int64_t zn)
{
  __label__ end_function_106, end_block_exception_107;

  int64_t zcbz332;
  int64_t zgaz318;
  {
    sail_int zgsz365;
    CREATE(sail_int)(&zgsz365);
    CONVERT_OF(sail_int, mach_int)(&zgsz365, zn);
    sail_int zgsz366;
    CREATE(sail_int)(&zgsz366);
    pow2(&zgsz366, zgsz365);
    zgaz318 = CONVERT_OF(mach_int, sail_int)(zgsz366);
    KILL(sail_int)(&zgsz366);
    KILL(sail_int)(&zgsz365);
  }
  {
    sail_int zgsz367;
    CREATE(sail_int)(&zgsz367);
    CONVERT_OF(sail_int, mach_int)(&zgsz367, zgaz318);
    sail_int zgsz368;
    CREATE(sail_int)(&zgsz368);
    CONVERT_OF(sail_int, mach_int)(&zgsz368, INT64_C(1));
    sail_int zgsz369;
    CREATE(sail_int)(&zgsz369);
    sub_int(&zgsz369, zgsz367, zgsz368);
    zcbz332 = CONVERT_OF(mach_int, sail_int)(zgsz369);
    KILL(sail_int)(&zgsz369);
    KILL(sail_int)(&zgsz368);
    KILL(sail_int)(&zgsz367);
  }

end_function_106: ;
  return zcbz332;
end_block_exception_107: ;

  return INT64_C(0xdeadc0de);
}



static int64_t znum_flags;
static void create_letbind_0(void) {  


  int64_t zgsz370;
  zgsz370 = INT64_C(1);
  znum_flags = zgsz370;

let_end_108: ;
}
static void kill_letbind_0(void) {  
}

static int64_t zreserved_otypes;
static void create_letbind_1(void) {  


  int64_t zgsz371;
  zgsz371 = INT64_C(4);
  zreserved_otypes = zgsz371;

let_end_109: ;
}
static void kill_letbind_1(void) {  
}

static int64_t zotype_unsealed;
static void create_letbind_2(void) {  


  int64_t zgsz372;
  zgsz372 = INT64_C(-1);
  zotype_unsealed = zgsz372;

let_end_110: ;
}
static void kill_letbind_2(void) {  
}

static uint64_t zPCC_IDX;
static void create_letbind_3(void) {  


  uint64_t zgsz373;
  zgsz373 = UINT64_C(0b100000);
  zPCC_IDX = zgsz373;

let_end_111: ;
}
static void kill_letbind_3(void) {  
}

static uint64_t zDDC_IDX;
static void create_letbind_4(void) {  


  uint64_t zgsz374;
  zgsz374 = UINT64_C(0b100001);
  zDDC_IDX = zgsz374;

let_end_112: ;
}
static void kill_letbind_4(void) {  
}

static int64_t zcap_sizze;
static void create_letbind_5(void) {  


  int64_t zgsz375;
  zgsz375 = INT64_C(8);
  zcap_sizze = zgsz375;

let_end_113: ;
}
static void kill_letbind_5(void) {  
}

static int64_t zlog2_cap_sizze;
static void create_letbind_6(void) {  


  int64_t zgsz376;
  zgsz376 = INT64_C(3);
  zlog2_cap_sizze = zgsz376;

let_end_114: ;
}
static void kill_letbind_6(void) {  
}

static int64_t zotype_width;
static void create_letbind_7(void) {  


  int64_t zgsz377;
  zgsz377 = INT64_C(4);
  zotype_width = zgsz377;

let_end_115: ;
}
static void kill_letbind_7(void) {  
}

static int64_t zuperms_width;
static void create_letbind_8(void) {  


  int64_t zgsz378;
  zgsz378 = INT64_C(0);
  zuperms_width = zgsz378;

let_end_116: ;
}
static void kill_letbind_8(void) {  
}

static int64_t zcap_addr_width;
static void create_letbind_9(void) {  


  int64_t zgsz379;
  zgsz379 = INT64_C(32);
  zcap_addr_width = zgsz379;

let_end_117: ;
}
static void kill_letbind_9(void) {  
}

static int64_t zcap_len_width;
static void create_letbind_10(void) {  


  int64_t zgsz380;
  {
    sail_int zgsz381;
    CREATE(sail_int)(&zgsz381);
    CONVERT_OF(sail_int, mach_int)(&zgsz381, INT64_C(32));
    sail_int zgsz382;
    CREATE(sail_int)(&zgsz382);
    CONVERT_OF(sail_int, mach_int)(&zgsz382, INT64_C(1));
    sail_int zgsz383;
    CREATE(sail_int)(&zgsz383);
    add_int(&zgsz383, zgsz381, zgsz382);
    zgsz380 = CONVERT_OF(mach_int, sail_int)(zgsz383);
    KILL(sail_int)(&zgsz383);
    KILL(sail_int)(&zgsz382);
    KILL(sail_int)(&zgsz381);
  }
  zcap_len_width = zgsz380;

let_end_118: ;
}
static void kill_letbind_10(void) {  
}

static int64_t zmantissa_width;
static void create_letbind_11(void) {  


  int64_t zgsz384;
  zgsz384 = INT64_C(8);
  zmantissa_width = zgsz384;

let_end_119: ;
}
static void kill_letbind_11(void) {  
}

static int64_t zMAX_ADDR;
static void create_letbind_12(void) {  


  int64_t zgsz385;
  zgsz385 = sailgen_zzzz7nzzJzzK32z3MAX(zcap_addr_width);
  zMAX_ADDR = zgsz385;

let_end_120: ;
}
static void kill_letbind_12(void) {  
}

static struct zCapability sailgen_undefined_Capability(unit);

static struct zCapability sailgen_undefined_Capability(unit zgsz386)
{
  __label__ end_function_122, end_block_exception_123;

  struct zCapability zcbz333;
  bool zgaz319;
  zgaz319 = undefined_bool(UNIT);
  uint64_t zgaz320;
  {
    sail_int zgsz387;
    CREATE(sail_int)(&zgsz387);
    CONVERT_OF(sail_int, mach_int)(&zgsz387, INT64_C(0));
    lbits zgsz388;
    CREATE(lbits)(&zgsz388);
    UNDEFINED(lbits)(&zgsz388, zgsz387);
    zgaz320 = CONVERT_OF(fbits, lbits)(zgsz388, true);
    KILL(lbits)(&zgsz388);
    KILL(sail_int)(&zgsz387);
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
    sail_int zgsz389;
    CREATE(sail_int)(&zgsz389);
    CONVERT_OF(sail_int, mach_int)(&zgsz389, INT64_C(0));
    lbits zgsz390;
    CREATE(lbits)(&zgsz390);
    UNDEFINED(lbits)(&zgsz390, zgsz389);
    zgaz333 = CONVERT_OF(fbits, lbits)(zgsz390, true);
    KILL(lbits)(&zgsz390);
    KILL(sail_int)(&zgsz389);
  }
  bool zgaz334;
  zgaz334 = undefined_bool(UNIT);
  bool zgaz335;
  zgaz335 = undefined_bool(UNIT);
  uint64_t zgaz336;
  {
    sail_int zgsz391;
    CREATE(sail_int)(&zgsz391);
    CONVERT_OF(sail_int, mach_int)(&zgsz391, INT64_C(6));
    lbits zgsz392;
    CREATE(lbits)(&zgsz392);
    UNDEFINED(lbits)(&zgsz392, zgsz391);
    zgaz336 = CONVERT_OF(fbits, lbits)(zgsz392, true);
    KILL(lbits)(&zgsz392);
    KILL(sail_int)(&zgsz391);
  }
  bool zgaz337;
  zgaz337 = undefined_bool(UNIT);
  uint64_t zgaz338;
  {
    sail_int zgsz393;
    CREATE(sail_int)(&zgsz393);
    CONVERT_OF(sail_int, mach_int)(&zgsz393, INT64_C(8));
    lbits zgsz394;
    CREATE(lbits)(&zgsz394);
    UNDEFINED(lbits)(&zgsz394, zgsz393);
    zgaz338 = CONVERT_OF(fbits, lbits)(zgsz394, true);
    KILL(lbits)(&zgsz394);
    KILL(sail_int)(&zgsz393);
  }
  uint64_t zgaz339;
  {
    sail_int zgsz395;
    CREATE(sail_int)(&zgsz395);
    CONVERT_OF(sail_int, mach_int)(&zgsz395, INT64_C(8));
    lbits zgsz396;
    CREATE(lbits)(&zgsz396);
    UNDEFINED(lbits)(&zgsz396, zgsz395);
    zgaz339 = CONVERT_OF(fbits, lbits)(zgsz396, true);
    KILL(lbits)(&zgsz396);
    KILL(sail_int)(&zgsz395);
  }
  uint64_t zgaz340;
  {
    sail_int zgsz397;
    CREATE(sail_int)(&zgsz397);
    CONVERT_OF(sail_int, mach_int)(&zgsz397, INT64_C(4));
    lbits zgsz398;
    CREATE(lbits)(&zgsz398);
    UNDEFINED(lbits)(&zgsz398, zgsz397);
    zgaz340 = CONVERT_OF(fbits, lbits)(zgsz398, true);
    KILL(lbits)(&zgsz398);
    KILL(sail_int)(&zgsz397);
  }
  uint64_t zgaz341;
  {
    sail_int zgsz399;
    CREATE(sail_int)(&zgsz399);
    CONVERT_OF(sail_int, mach_int)(&zgsz399, INT64_C(32));
    lbits zgsz3100;
    CREATE(lbits)(&zgsz3100);
    UNDEFINED(lbits)(&zgsz3100, zgsz399);
    zgaz341 = CONVERT_OF(fbits, lbits)(zgsz3100, true);
    KILL(lbits)(&zgsz3100);
    KILL(sail_int)(&zgsz399);
  }
  struct zCapability zgsz3101;
  zgsz3101.zB = zgaz338;
  zgsz3101.zE = zgaz336;
  zgsz3101.zT = zgaz339;
  zgsz3101.zaccess_system_regs = zgaz322;
  zgsz3101.zaddress = zgaz341;
  zgsz3101.zflag_cap_mode = zgaz334;
  zgsz3101.zglobal = zgaz332;
  zgsz3101.zinternal_e = zgaz335;
  zgsz3101.zotype = zgaz340;
  zgsz3101.zpermit_ccall = zgaz324;
  zgsz3101.zpermit_execute = zgaz331;
  zgsz3101.zpermit_load = zgaz330;
  zgsz3101.zpermit_load_cap = zgaz328;
  zgsz3101.zpermit_seal = zgaz325;
  zgsz3101.zpermit_set_CID = zgaz321;
  zgsz3101.zpermit_store = zgaz329;
  zgsz3101.zpermit_store_cap = zgaz327;
  zgsz3101.zpermit_store_local_cap = zgaz326;
  zgsz3101.zpermit_unseal = zgaz323;
  zgsz3101.zreserved = zgaz333;
  zgsz3101.zsealed = zgaz337;
  zgsz3101.ztag = zgaz319;
  zgsz3101.zuperms = zgaz320;
  zcbz333 = zgsz3101;
























end_function_122: ;
  return zcbz333;
end_block_exception_123: ;
  struct zCapability zcbz353 = { .zuperms = UINT64_C(0xdeadc0de), .ztag = false, .zsealed = false, .zreserved = UINT64_C(0xdeadc0de), .zpermit_unseal = false, .zpermit_store_local_cap = false, .zpermit_store_cap = false, .zpermit_store = false, .zpermit_set_CID = false, .zpermit_seal = false, .zpermit_load_cap = false, .zpermit_load = false, .zpermit_execute = false, .zpermit_ccall = false, .zotype = UINT64_C(0xdeadc0de), .zinternal_e = false, .zglobal = false, .zflag_cap_mode = false, .zaddress = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zT = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  return zcbz353;
}

static int64_t zmaxE;
static void create_letbind_13(void) {  


  int64_t zgsz3102;
  zgsz3102 = INT64_C(26);
  zmaxE = zgsz3102;

let_end_124: ;
}
static void kill_letbind_13(void) {  
}

static int64_t zinternal_exponent_take_bits;
static void create_letbind_14(void) {  


  int64_t zgsz3103;
  zgsz3103 = INT64_C(3);
  zinternal_exponent_take_bits = zgsz3103;

let_end_125: ;
}
static void kill_letbind_14(void) {  
}

static uint64_t zresetE;
static void create_letbind_15(void) {  


  uint64_t zgsz3104;
  {
    sail_int zgsz3105;
    CREATE(sail_int)(&zgsz3105);
    CONVERT_OF(sail_int, mach_int)(&zgsz3105, INT64_C(6));
    sail_int zgsz3106;
    CREATE(sail_int)(&zgsz3106);
    CONVERT_OF(sail_int, mach_int)(&zgsz3106, zmaxE);
    lbits zgsz3107;
    CREATE(lbits)(&zgsz3107);
    sailgen_to_bits(&zgsz3107, zgsz3105, zgsz3106);
    zgsz3104 = CONVERT_OF(fbits, lbits)(zgsz3107, true);
    KILL(lbits)(&zgsz3107);
    KILL(sail_int)(&zgsz3106);
    KILL(sail_int)(&zgsz3105);
  }
  zresetE = zgsz3104;

let_end_126: ;
}
static void kill_letbind_15(void) {  
}

static uint64_t zresetT;
static void create_letbind_16(void) {  


  uint64_t zgsz3108;
  zgsz3108 = UINT64_C(0x40);
  zresetT = zgsz3108;

let_end_127: ;
}
static void kill_letbind_16(void) {  
}

static struct zCapability znull_cap;
static void create_letbind_17(void) {  


  struct zCapability zgsz3113;
  uint64_t zgaz342;
  zgaz342 = sailgen_zzzz7nzzJzzK0z3zzeros_implicit(INT64_C(0));
  uint64_t zgaz343;
  zgaz343 = sailgen_zzzz7nzzJzzK0z3zzeros_implicit(INT64_C(0));
  uint64_t zgaz344;
  zgaz344 = sailgen_zzzz7nzzJzzK8z3zzeros_implicit(INT64_C(8));
  uint64_t zgaz345;
  {
    sail_int zgsz3109;
    CREATE(sail_int)(&zgsz3109);
    CONVERT_OF(sail_int, mach_int)(&zgsz3109, zotype_width);
    sail_int zgsz3110;
    CREATE(sail_int)(&zgsz3110);
    CONVERT_OF(sail_int, mach_int)(&zgsz3110, zotype_unsealed);
    lbits zgsz3111;
    CREATE(lbits)(&zgsz3111);
    sailgen_to_bits(&zgsz3111, zgsz3109, zgsz3110);
    zgaz345 = CONVERT_OF(fbits, lbits)(zgsz3111, true);
    KILL(lbits)(&zgsz3111);
    KILL(sail_int)(&zgsz3110);
    KILL(sail_int)(&zgsz3109);
  }
  uint64_t zgaz346;
  zgaz346 = sailgen_zzzz7nzzJzzK32z3zzeros_implicit(INT64_C(32));
  struct zCapability zgsz3112;
  zgsz3112.zB = zgaz344;
  zgsz3112.zE = zresetE;
  zgsz3112.zT = zresetT;
  zgsz3112.zaccess_system_regs = false;
  zgsz3112.zaddress = zgaz346;
  zgsz3112.zflag_cap_mode = false;
  zgsz3112.zglobal = false;
  zgsz3112.zinternal_e = true;
  zgsz3112.zotype = zgaz345;
  zgsz3112.zpermit_ccall = false;
  zgsz3112.zpermit_execute = false;
  zgsz3112.zpermit_load = false;
  zgsz3112.zpermit_load_cap = false;
  zgsz3112.zpermit_seal = false;
  zgsz3112.zpermit_set_CID = false;
  zgsz3112.zpermit_store = false;
  zgsz3112.zpermit_store_cap = false;
  zgsz3112.zpermit_store_local_cap = false;
  zgsz3112.zpermit_unseal = false;
  zgsz3112.zreserved = zgaz343;
  zgsz3112.zsealed = false;
  zgsz3112.ztag = false;
  zgsz3112.zuperms = zgaz342;
  zgsz3113 = zgsz3112;






  znull_cap = zgsz3113;

let_end_128: ;
}
static void kill_letbind_17(void) {  
}

static struct zCapability zdefault_cap;
static void create_letbind_18(void) {  


  struct zCapability zgsz3118;
  uint64_t zgaz347;
  zgaz347 = sailgen_zzzz7nzzJzzK0z3ones(INT64_C(0));
  uint64_t zgaz348;
  zgaz348 = sailgen_zzzz7nzzJzzK0z3zzeros_implicit(INT64_C(0));
  uint64_t zgaz349;
  zgaz349 = sailgen_zzzz7nzzJzzK8z3zzeros_implicit(INT64_C(8));
  uint64_t zgaz350;
  {
    sail_int zgsz3114;
    CREATE(sail_int)(&zgsz3114);
    CONVERT_OF(sail_int, mach_int)(&zgsz3114, zotype_width);
    sail_int zgsz3115;
    CREATE(sail_int)(&zgsz3115);
    CONVERT_OF(sail_int, mach_int)(&zgsz3115, zotype_unsealed);
    lbits zgsz3116;
    CREATE(lbits)(&zgsz3116);
    sailgen_to_bits(&zgsz3116, zgsz3114, zgsz3115);
    zgaz350 = CONVERT_OF(fbits, lbits)(zgsz3116, true);
    KILL(lbits)(&zgsz3116);
    KILL(sail_int)(&zgsz3115);
    KILL(sail_int)(&zgsz3114);
  }
  uint64_t zgaz351;
  zgaz351 = sailgen_zzzz7nzzJzzK32z3zzeros_implicit(INT64_C(32));
  struct zCapability zgsz3117;
  zgsz3117.zB = zgaz349;
  zgsz3117.zE = zresetE;
  zgsz3117.zT = zresetT;
  zgsz3117.zaccess_system_regs = true;
  zgsz3117.zaddress = zgaz351;
  zgsz3117.zflag_cap_mode = false;
  zgsz3117.zglobal = true;
  zgsz3117.zinternal_e = true;
  zgsz3117.zotype = zgaz350;
  zgsz3117.zpermit_ccall = true;
  zgsz3117.zpermit_execute = true;
  zgsz3117.zpermit_load = true;
  zgsz3117.zpermit_load_cap = true;
  zgsz3117.zpermit_seal = true;
  zgsz3117.zpermit_set_CID = true;
  zgsz3117.zpermit_store = true;
  zgsz3117.zpermit_store_cap = true;
  zgsz3117.zpermit_store_local_cap = true;
  zgsz3117.zpermit_unseal = true;
  zgsz3117.zreserved = zgaz348;
  zgsz3117.zsealed = false;
  zgsz3117.ztag = true;
  zgsz3117.zuperms = zgaz347;
  zgsz3118 = zgsz3117;






  zdefault_cap = zgsz3118;

let_end_129: ;
}
static void kill_letbind_18(void) {  
}

static struct zCapability sailgen_capBitsToCapability(bool, uint64_t);

static struct zCapability sailgen_capBitsToCapability(bool zt, uint64_t zc)
{
  __label__ cleanup_132, end_cleanup_133, end_function_131, end_block_exception_134;

  struct zCapability zcbz334;
  bool zinternal_exponent;
  {
    fbits zgaz390;
    {
      sail_int zgsz3120;
      CREATE(sail_int)(&zgsz3120);
      CONVERT_OF(sail_int, mach_int)(&zgsz3120, INT64_C(46));
      lbits zgsz3119;
      CREATE(lbits)(&zgsz3119);
      CONVERT_OF(lbits, fbits)(&zgsz3119, zc, UINT64_C(64) , true);
      zgaz390 = bitvector_access(zgsz3119, zgsz3120);
      KILL(sail_int)(&zgsz3120);
      KILL(lbits)(&zgsz3119);
    }
    zinternal_exponent = sailgen_bit_to_bool(zgaz390);
  
  }
  uint64_t zotype;
  {
    lbits zgsz3121;
    CREATE(lbits)(&zgsz3121);
    CONVERT_OF(lbits, fbits)(&zgsz3121, zc, UINT64_C(64) , true);
    sail_int zgsz3122;
    CREATE(sail_int)(&zgsz3122);
    CONVERT_OF(sail_int, mach_int)(&zgsz3122, INT64_C(50));
    sail_int zgsz3123;
    CREATE(sail_int)(&zgsz3123);
    CONVERT_OF(sail_int, mach_int)(&zgsz3123, INT64_C(47));
    lbits zgsz3124;
    CREATE(lbits)(&zgsz3124);
    vector_subrange_lbits(&zgsz3124, zgsz3121, zgsz3122, zgsz3123);
    zotype = CONVERT_OF(fbits, lbits)(zgsz3124, true);
    KILL(lbits)(&zgsz3124);
    KILL(sail_int)(&zgsz3123);
    KILL(sail_int)(&zgsz3122);
    KILL(lbits)(&zgsz3121);
  }
  bool zsealed;
  {
    uint64_t zgaz389;
    {
      sail_int zgsz3125;
      CREATE(sail_int)(&zgsz3125);
      CONVERT_OF(sail_int, mach_int)(&zgsz3125, zotype_width);
      sail_int zgsz3126;
      CREATE(sail_int)(&zgsz3126);
      CONVERT_OF(sail_int, mach_int)(&zgsz3126, zotype_unsealed);
      lbits zgsz3127;
      CREATE(lbits)(&zgsz3127);
      sailgen_to_bits(&zgsz3127, zgsz3125, zgsz3126);
      zgaz389 = CONVERT_OF(fbits, lbits)(zgsz3127, true);
      KILL(lbits)(&zgsz3127);
      KILL(sail_int)(&zgsz3126);
      KILL(sail_int)(&zgsz3125);
    }
    {
      lbits zgsz3129;
      CREATE(lbits)(&zgsz3129);
      CONVERT_OF(lbits, fbits)(&zgsz3129, zgaz389, UINT64_C(4) , true);
      lbits zgsz3128;
      CREATE(lbits)(&zgsz3128);
      CONVERT_OF(lbits, fbits)(&zgsz3128, zotype, UINT64_C(4) , true);
      zsealed = neq_bits(zgsz3128, zgsz3129);
      KILL(lbits)(&zgsz3129);
      KILL(lbits)(&zgsz3128);
    }
  
  }
  uint64_t zE;
  zE = sailgen_zzzz7nzzJzzK6z3zzeros_implicit(INT64_C(6));
  uint64_t zBs;
  zBs = sailgen_zzzz7nzzJzzK8z3zzeros_implicit(INT64_C(8));
  uint64_t zT;
  zT = sailgen_zzzz7nzzJzzK6z3zzeros_implicit(INT64_C(6));
  uint64_t zlenMSBs;
  zlenMSBs = sailgen_zzzz7nzzJzzK2z3zzeros_implicit(INT64_C(2));
  {
    unit zgsz3130;
    if (zinternal_exponent) {
    {
      uint64_t zgaz352;
      {
        lbits zgsz3141;
        CREATE(lbits)(&zgsz3141);
        CONVERT_OF(lbits, fbits)(&zgsz3141, zc, UINT64_C(64) , true);
        sail_int zgsz3142;
        CREATE(sail_int)(&zgsz3142);
        CONVERT_OF(sail_int, mach_int)(&zgsz3142, INT64_C(42));
        sail_int zgsz3143;
        CREATE(sail_int)(&zgsz3143);
        CONVERT_OF(sail_int, mach_int)(&zgsz3143, INT64_C(40));
        lbits zgsz3144;
        CREATE(lbits)(&zgsz3144);
        vector_subrange_lbits(&zgsz3144, zgsz3141, zgsz3142, zgsz3143);
        zgaz352 = CONVERT_OF(fbits, lbits)(zgsz3144, true);
        KILL(lbits)(&zgsz3144);
        KILL(sail_int)(&zgsz3143);
        KILL(sail_int)(&zgsz3142);
        KILL(lbits)(&zgsz3141);
      }
      uint64_t zgaz353;
      {
        lbits zgsz3145;
        CREATE(lbits)(&zgsz3145);
        CONVERT_OF(lbits, fbits)(&zgsz3145, zc, UINT64_C(64) , true);
        sail_int zgsz3146;
        CREATE(sail_int)(&zgsz3146);
        CONVERT_OF(sail_int, mach_int)(&zgsz3146, INT64_C(34));
        sail_int zgsz3147;
        CREATE(sail_int)(&zgsz3147);
        CONVERT_OF(sail_int, mach_int)(&zgsz3147, INT64_C(32));
        lbits zgsz3148;
        CREATE(lbits)(&zgsz3148);
        vector_subrange_lbits(&zgsz3148, zgsz3145, zgsz3146, zgsz3147);
        zgaz353 = CONVERT_OF(fbits, lbits)(zgsz3148, true);
        KILL(lbits)(&zgsz3148);
        KILL(sail_int)(&zgsz3147);
        KILL(sail_int)(&zgsz3146);
        KILL(lbits)(&zgsz3145);
      }
      {
        lbits zgsz3149;
        CREATE(lbits)(&zgsz3149);
        CONVERT_OF(lbits, fbits)(&zgsz3149, zgaz352, UINT64_C(3) , true);
        lbits zgsz3150;
        CREATE(lbits)(&zgsz3150);
        CONVERT_OF(lbits, fbits)(&zgsz3150, zgaz353, UINT64_C(3) , true);
        lbits zgsz3151;
        CREATE(lbits)(&zgsz3151);
        append(&zgsz3151, zgsz3149, zgsz3150);
        zE = CONVERT_OF(fbits, lbits)(zgsz3151, true);
        KILL(lbits)(&zgsz3151);
        KILL(lbits)(&zgsz3150);
        KILL(lbits)(&zgsz3149);
      }
      unit zgsz3161;
      zgsz3161 = UNIT;
    
    
    }
    {
      zlenMSBs = UINT64_C(0b01);
      unit zgsz3160;
      zgsz3160 = UNIT;
    }
    {
      uint64_t zgaz354;
      {
        lbits zgsz3152;
        CREATE(lbits)(&zgsz3152);
        CONVERT_OF(lbits, fbits)(&zgsz3152, zc, UINT64_C(64) , true);
        sail_int zgsz3153;
        CREATE(sail_int)(&zgsz3153);
        CONVERT_OF(sail_int, mach_int)(&zgsz3153, INT64_C(45));
        sail_int zgsz3154;
        CREATE(sail_int)(&zgsz3154);
        CONVERT_OF(sail_int, mach_int)(&zgsz3154, INT64_C(43));
        lbits zgsz3155;
        CREATE(lbits)(&zgsz3155);
        vector_subrange_lbits(&zgsz3155, zgsz3152, zgsz3153, zgsz3154);
        zgaz354 = CONVERT_OF(fbits, lbits)(zgsz3155, true);
        KILL(lbits)(&zgsz3155);
        KILL(sail_int)(&zgsz3154);
        KILL(sail_int)(&zgsz3153);
        KILL(lbits)(&zgsz3152);
      }
      {
        lbits zgsz3156;
        CREATE(lbits)(&zgsz3156);
        CONVERT_OF(lbits, fbits)(&zgsz3156, zgaz354, UINT64_C(3) , true);
        lbits zgsz3157;
        CREATE(lbits)(&zgsz3157);
        CONVERT_OF(lbits, fbits)(&zgsz3157, UINT64_C(0b000), UINT64_C(3) , true);
        lbits zgsz3158;
        CREATE(lbits)(&zgsz3158);
        append(&zgsz3158, zgsz3156, zgsz3157);
        zT = CONVERT_OF(fbits, lbits)(zgsz3158, true);
        KILL(lbits)(&zgsz3158);
        KILL(lbits)(&zgsz3157);
        KILL(lbits)(&zgsz3156);
      }
      unit zgsz3159;
      zgsz3159 = UNIT;
    
    }
    uint64_t zgaz355;
    {
      lbits zgsz3162;
      CREATE(lbits)(&zgsz3162);
      CONVERT_OF(lbits, fbits)(&zgsz3162, zc, UINT64_C(64) , true);
      sail_int zgsz3163;
      CREATE(sail_int)(&zgsz3163);
      CONVERT_OF(sail_int, mach_int)(&zgsz3163, INT64_C(39));
      sail_int zgsz3164;
      CREATE(sail_int)(&zgsz3164);
      CONVERT_OF(sail_int, mach_int)(&zgsz3164, INT64_C(35));
      lbits zgsz3165;
      CREATE(lbits)(&zgsz3165);
      vector_subrange_lbits(&zgsz3165, zgsz3162, zgsz3163, zgsz3164);
      zgaz355 = CONVERT_OF(fbits, lbits)(zgsz3165, true);
      KILL(lbits)(&zgsz3165);
      KILL(sail_int)(&zgsz3164);
      KILL(sail_int)(&zgsz3163);
      KILL(lbits)(&zgsz3162);
    }
    {
      lbits zgsz3166;
      CREATE(lbits)(&zgsz3166);
      CONVERT_OF(lbits, fbits)(&zgsz3166, zgaz355, UINT64_C(5) , true);
      lbits zgsz3167;
      CREATE(lbits)(&zgsz3167);
      CONVERT_OF(lbits, fbits)(&zgsz3167, UINT64_C(0b000), UINT64_C(3) , true);
      lbits zgsz3168;
      CREATE(lbits)(&zgsz3168);
      append(&zgsz3168, zgsz3166, zgsz3167);
      zBs = CONVERT_OF(fbits, lbits)(zgsz3168, true);
      KILL(lbits)(&zgsz3168);
      KILL(lbits)(&zgsz3167);
      KILL(lbits)(&zgsz3166);
    }
    zgsz3130 = UNIT;
  
    } else {
    {
      zlenMSBs = UINT64_C(0b00);
      unit zgsz3136;
      zgsz3136 = UNIT;
    }
    {
      {
        lbits zgsz3131;
        CREATE(lbits)(&zgsz3131);
        CONVERT_OF(lbits, fbits)(&zgsz3131, zc, UINT64_C(64) , true);
        sail_int zgsz3132;
        CREATE(sail_int)(&zgsz3132);
        CONVERT_OF(sail_int, mach_int)(&zgsz3132, INT64_C(45));
        sail_int zgsz3133;
        CREATE(sail_int)(&zgsz3133);
        CONVERT_OF(sail_int, mach_int)(&zgsz3133, INT64_C(40));
        lbits zgsz3134;
        CREATE(lbits)(&zgsz3134);
        vector_subrange_lbits(&zgsz3134, zgsz3131, zgsz3132, zgsz3133);
        zT = CONVERT_OF(fbits, lbits)(zgsz3134, true);
        KILL(lbits)(&zgsz3134);
        KILL(sail_int)(&zgsz3133);
        KILL(sail_int)(&zgsz3132);
        KILL(lbits)(&zgsz3131);
      }
      unit zgsz3135;
      zgsz3135 = UNIT;
    }
    {
      lbits zgsz3137;
      CREATE(lbits)(&zgsz3137);
      CONVERT_OF(lbits, fbits)(&zgsz3137, zc, UINT64_C(64) , true);
      sail_int zgsz3138;
      CREATE(sail_int)(&zgsz3138);
      CONVERT_OF(sail_int, mach_int)(&zgsz3138, INT64_C(39));
      sail_int zgsz3139;
      CREATE(sail_int)(&zgsz3139);
      CONVERT_OF(sail_int, mach_int)(&zgsz3139, INT64_C(32));
      lbits zgsz3140;
      CREATE(lbits)(&zgsz3140);
      vector_subrange_lbits(&zgsz3140, zgsz3137, zgsz3138, zgsz3139);
      zBs = CONVERT_OF(fbits, lbits)(zgsz3140, true);
      KILL(lbits)(&zgsz3140);
      KILL(sail_int)(&zgsz3139);
      KILL(sail_int)(&zgsz3138);
      KILL(lbits)(&zgsz3137);
    }
    zgsz3130 = UNIT;
    }
  }
  uint64_t zcarry_out;
  {
    bool zgaz388;
    {
      uint64_t zgaz387;
      {
        lbits zgsz3169;
        CREATE(lbits)(&zgsz3169);
        CONVERT_OF(lbits, fbits)(&zgsz3169, zBs, UINT64_C(8) , true);
        sail_int zgsz3170;
        CREATE(sail_int)(&zgsz3170);
        CONVERT_OF(sail_int, mach_int)(&zgsz3170, INT64_C(5));
        sail_int zgsz3171;
        CREATE(sail_int)(&zgsz3171);
        CONVERT_OF(sail_int, mach_int)(&zgsz3171, INT64_C(0));
        lbits zgsz3172;
        CREATE(lbits)(&zgsz3172);
        vector_subrange_lbits(&zgsz3172, zgsz3169, zgsz3170, zgsz3171);
        zgaz387 = CONVERT_OF(fbits, lbits)(zgsz3172, true);
        KILL(lbits)(&zgsz3172);
        KILL(sail_int)(&zgsz3171);
        KILL(sail_int)(&zgsz3170);
        KILL(lbits)(&zgsz3169);
      }
      zgaz388 = sailgen_z8operatorz0zzzz7nzzJzzK6z3zI_uz9(zT, zgaz387);
    
    }
    if (zgaz388) {  zcarry_out = UINT64_C(0b01);  } else {  zcarry_out = UINT64_C(0b00);  }
  
  }
  uint64_t zTtop2;
  {
    uint64_t zgaz386;
    {
      uint64_t zgaz385;
      {
        lbits zgsz3173;
        CREATE(lbits)(&zgsz3173);
        CONVERT_OF(lbits, fbits)(&zgsz3173, zBs, UINT64_C(8) , true);
        sail_int zgsz3174;
        CREATE(sail_int)(&zgsz3174);
        CONVERT_OF(sail_int, mach_int)(&zgsz3174, INT64_C(7));
        sail_int zgsz3175;
        CREATE(sail_int)(&zgsz3175);
        CONVERT_OF(sail_int, mach_int)(&zgsz3175, INT64_C(6));
        lbits zgsz3176;
        CREATE(lbits)(&zgsz3176);
        vector_subrange_lbits(&zgsz3176, zgsz3173, zgsz3174, zgsz3175);
        zgaz385 = CONVERT_OF(fbits, lbits)(zgsz3176, true);
        KILL(lbits)(&zgsz3176);
        KILL(sail_int)(&zgsz3175);
        KILL(sail_int)(&zgsz3174);
        KILL(lbits)(&zgsz3173);
      }
      {
        lbits zgsz3177;
        CREATE(lbits)(&zgsz3177);
        CONVERT_OF(lbits, fbits)(&zgsz3177, zgaz385, UINT64_C(2) , true);
        lbits zgsz3178;
        CREATE(lbits)(&zgsz3178);
        CONVERT_OF(lbits, fbits)(&zgsz3178, zlenMSBs, UINT64_C(2) , true);
        lbits zgsz3179;
        CREATE(lbits)(&zgsz3179);
        add_bits(&zgsz3179, zgsz3177, zgsz3178);
        zgaz386 = CONVERT_OF(fbits, lbits)(zgsz3179, true);
        KILL(lbits)(&zgsz3179);
        KILL(lbits)(&zgsz3178);
        KILL(lbits)(&zgsz3177);
      }
    
    }
    {
      lbits zgsz3180;
      CREATE(lbits)(&zgsz3180);
      CONVERT_OF(lbits, fbits)(&zgsz3180, zgaz386, UINT64_C(2) , true);
      lbits zgsz3181;
      CREATE(lbits)(&zgsz3181);
      CONVERT_OF(lbits, fbits)(&zgsz3181, zcarry_out, UINT64_C(2) , true);
      lbits zgsz3182;
      CREATE(lbits)(&zgsz3182);
      add_bits(&zgsz3182, zgsz3180, zgsz3181);
      zTtop2 = CONVERT_OF(fbits, lbits)(zgsz3182, true);
      KILL(lbits)(&zgsz3182);
      KILL(lbits)(&zgsz3181);
      KILL(lbits)(&zgsz3180);
    }
  
  }
  struct zCapability zgaz384;
  {
    bool zgaz357;
    {
      fbits zgaz356;
      {
        sail_int zgsz3184;
        CREATE(sail_int)(&zgsz3184);
        CONVERT_OF(sail_int, mach_int)(&zgsz3184, INT64_C(63));
        lbits zgsz3183;
        CREATE(lbits)(&zgsz3183);
        CONVERT_OF(lbits, fbits)(&zgsz3183, zc, UINT64_C(64) , true);
        zgaz356 = bitvector_access(zgsz3183, zgsz3184);
        KILL(sail_int)(&zgsz3184);
        KILL(lbits)(&zgsz3183);
      }
      zgaz357 = sailgen_bit_to_bool(zgaz356);
    
    }
    bool zgaz359;
    {
      fbits zgaz358;
      {
        sail_int zgsz3186;
        CREATE(sail_int)(&zgsz3186);
        CONVERT_OF(sail_int, mach_int)(&zgsz3186, INT64_C(62));
        lbits zgsz3185;
        CREATE(lbits)(&zgsz3185);
        CONVERT_OF(lbits, fbits)(&zgsz3185, zc, UINT64_C(64) , true);
        zgaz358 = bitvector_access(zgsz3185, zgsz3186);
        KILL(sail_int)(&zgsz3186);
        KILL(lbits)(&zgsz3185);
      }
      zgaz359 = sailgen_bit_to_bool(zgaz358);
    
    }
    bool zgaz361;
    {
      fbits zgaz360;
      {
        sail_int zgsz3188;
        CREATE(sail_int)(&zgsz3188);
        CONVERT_OF(sail_int, mach_int)(&zgsz3188, INT64_C(61));
        lbits zgsz3187;
        CREATE(lbits)(&zgsz3187);
        CONVERT_OF(lbits, fbits)(&zgsz3187, zc, UINT64_C(64) , true);
        zgaz360 = bitvector_access(zgsz3187, zgsz3188);
        KILL(sail_int)(&zgsz3188);
        KILL(lbits)(&zgsz3187);
      }
      zgaz361 = sailgen_bit_to_bool(zgaz360);
    
    }
    bool zgaz363;
    {
      fbits zgaz362;
      {
        sail_int zgsz3190;
        CREATE(sail_int)(&zgsz3190);
        CONVERT_OF(sail_int, mach_int)(&zgsz3190, INT64_C(60));
        lbits zgsz3189;
        CREATE(lbits)(&zgsz3189);
        CONVERT_OF(lbits, fbits)(&zgsz3189, zc, UINT64_C(64) , true);
        zgaz362 = bitvector_access(zgsz3189, zgsz3190);
        KILL(sail_int)(&zgsz3190);
        KILL(lbits)(&zgsz3189);
      }
      zgaz363 = sailgen_bit_to_bool(zgaz362);
    
    }
    bool zgaz365;
    {
      fbits zgaz364;
      {
        sail_int zgsz3192;
        CREATE(sail_int)(&zgsz3192);
        CONVERT_OF(sail_int, mach_int)(&zgsz3192, INT64_C(59));
        lbits zgsz3191;
        CREATE(lbits)(&zgsz3191);
        CONVERT_OF(lbits, fbits)(&zgsz3191, zc, UINT64_C(64) , true);
        zgaz364 = bitvector_access(zgsz3191, zgsz3192);
        KILL(sail_int)(&zgsz3192);
        KILL(lbits)(&zgsz3191);
      }
      zgaz365 = sailgen_bit_to_bool(zgaz364);
    
    }
    bool zgaz367;
    {
      fbits zgaz366;
      {
        sail_int zgsz3194;
        CREATE(sail_int)(&zgsz3194);
        CONVERT_OF(sail_int, mach_int)(&zgsz3194, INT64_C(58));
        lbits zgsz3193;
        CREATE(lbits)(&zgsz3193);
        CONVERT_OF(lbits, fbits)(&zgsz3193, zc, UINT64_C(64) , true);
        zgaz366 = bitvector_access(zgsz3193, zgsz3194);
        KILL(sail_int)(&zgsz3194);
        KILL(lbits)(&zgsz3193);
      }
      zgaz367 = sailgen_bit_to_bool(zgaz366);
    
    }
    bool zgaz369;
    {
      fbits zgaz368;
      {
        sail_int zgsz3196;
        CREATE(sail_int)(&zgsz3196);
        CONVERT_OF(sail_int, mach_int)(&zgsz3196, INT64_C(57));
        lbits zgsz3195;
        CREATE(lbits)(&zgsz3195);
        CONVERT_OF(lbits, fbits)(&zgsz3195, zc, UINT64_C(64) , true);
        zgaz368 = bitvector_access(zgsz3195, zgsz3196);
        KILL(sail_int)(&zgsz3196);
        KILL(lbits)(&zgsz3195);
      }
      zgaz369 = sailgen_bit_to_bool(zgaz368);
    
    }
    bool zgaz371;
    {
      fbits zgaz370;
      {
        sail_int zgsz3198;
        CREATE(sail_int)(&zgsz3198);
        CONVERT_OF(sail_int, mach_int)(&zgsz3198, INT64_C(56));
        lbits zgsz3197;
        CREATE(lbits)(&zgsz3197);
        CONVERT_OF(lbits, fbits)(&zgsz3197, zc, UINT64_C(64) , true);
        zgaz370 = bitvector_access(zgsz3197, zgsz3198);
        KILL(sail_int)(&zgsz3198);
        KILL(lbits)(&zgsz3197);
      }
      zgaz371 = sailgen_bit_to_bool(zgaz370);
    
    }
    bool zgaz373;
    {
      fbits zgaz372;
      {
        sail_int zgsz3200;
        CREATE(sail_int)(&zgsz3200);
        CONVERT_OF(sail_int, mach_int)(&zgsz3200, INT64_C(55));
        lbits zgsz3199;
        CREATE(lbits)(&zgsz3199);
        CONVERT_OF(lbits, fbits)(&zgsz3199, zc, UINT64_C(64) , true);
        zgaz372 = bitvector_access(zgsz3199, zgsz3200);
        KILL(sail_int)(&zgsz3200);
        KILL(lbits)(&zgsz3199);
      }
      zgaz373 = sailgen_bit_to_bool(zgaz372);
    
    }
    bool zgaz375;
    {
      fbits zgaz374;
      {
        sail_int zgsz3202;
        CREATE(sail_int)(&zgsz3202);
        CONVERT_OF(sail_int, mach_int)(&zgsz3202, INT64_C(54));
        lbits zgsz3201;
        CREATE(lbits)(&zgsz3201);
        CONVERT_OF(lbits, fbits)(&zgsz3201, zc, UINT64_C(64) , true);
        zgaz374 = bitvector_access(zgsz3201, zgsz3202);
        KILL(sail_int)(&zgsz3202);
        KILL(lbits)(&zgsz3201);
      }
      zgaz375 = sailgen_bit_to_bool(zgaz374);
    
    }
    bool zgaz377;
    {
      fbits zgaz376;
      {
        sail_int zgsz3204;
        CREATE(sail_int)(&zgsz3204);
        CONVERT_OF(sail_int, mach_int)(&zgsz3204, INT64_C(53));
        lbits zgsz3203;
        CREATE(lbits)(&zgsz3203);
        CONVERT_OF(lbits, fbits)(&zgsz3203, zc, UINT64_C(64) , true);
        zgaz376 = bitvector_access(zgsz3203, zgsz3204);
        KILL(sail_int)(&zgsz3204);
        KILL(lbits)(&zgsz3203);
      }
      zgaz377 = sailgen_bit_to_bool(zgaz376);
    
    }
    bool zgaz379;
    {
      fbits zgaz378;
      {
        sail_int zgsz3206;
        CREATE(sail_int)(&zgsz3206);
        CONVERT_OF(sail_int, mach_int)(&zgsz3206, INT64_C(52));
        lbits zgsz3205;
        CREATE(lbits)(&zgsz3205);
        CONVERT_OF(lbits, fbits)(&zgsz3205, zc, UINT64_C(64) , true);
        zgaz378 = bitvector_access(zgsz3205, zgsz3206);
        KILL(sail_int)(&zgsz3206);
        KILL(lbits)(&zgsz3205);
      }
      zgaz379 = sailgen_bit_to_bool(zgaz378);
    
    }
    bool zgaz381;
    {
      fbits zgaz380;
      {
        sail_int zgsz3208;
        CREATE(sail_int)(&zgsz3208);
        CONVERT_OF(sail_int, mach_int)(&zgsz3208, INT64_C(51));
        lbits zgsz3207;
        CREATE(lbits)(&zgsz3207);
        CONVERT_OF(lbits, fbits)(&zgsz3207, zc, UINT64_C(64) , true);
        zgaz380 = bitvector_access(zgsz3207, zgsz3208);
        KILL(sail_int)(&zgsz3208);
        KILL(lbits)(&zgsz3207);
      }
      zgaz381 = sailgen_bit_to_bool(zgaz380);
    
    }
    uint64_t zgaz382;
    {
      lbits zgsz3209;
      CREATE(lbits)(&zgsz3209);
      CONVERT_OF(lbits, fbits)(&zgsz3209, zTtop2, UINT64_C(2) , true);
      lbits zgsz3210;
      CREATE(lbits)(&zgsz3210);
      CONVERT_OF(lbits, fbits)(&zgsz3210, zT, UINT64_C(6) , true);
      lbits zgsz3211;
      CREATE(lbits)(&zgsz3211);
      append(&zgsz3211, zgsz3209, zgsz3210);
      zgaz382 = CONVERT_OF(fbits, lbits)(zgsz3211, true);
      KILL(lbits)(&zgsz3211);
      KILL(lbits)(&zgsz3210);
      KILL(lbits)(&zgsz3209);
    }
    uint64_t zgaz383;
    {
      lbits zgsz3212;
      CREATE(lbits)(&zgsz3212);
      CONVERT_OF(lbits, fbits)(&zgsz3212, zc, UINT64_C(64) , true);
      sail_int zgsz3213;
      CREATE(sail_int)(&zgsz3213);
      CONVERT_OF(sail_int, mach_int)(&zgsz3213, INT64_C(31));
      sail_int zgsz3214;
      CREATE(sail_int)(&zgsz3214);
      CONVERT_OF(sail_int, mach_int)(&zgsz3214, INT64_C(0));
      lbits zgsz3215;
      CREATE(lbits)(&zgsz3215);
      vector_subrange_lbits(&zgsz3215, zgsz3212, zgsz3213, zgsz3214);
      zgaz383 = CONVERT_OF(fbits, lbits)(zgsz3215, true);
      KILL(lbits)(&zgsz3215);
      KILL(sail_int)(&zgsz3214);
      KILL(sail_int)(&zgsz3213);
      KILL(lbits)(&zgsz3212);
    }
    struct zCapability zgsz3216;
    zgsz3216.zB = zBs;
    zgsz3216.zE = zE;
    zgsz3216.zT = zgaz382;
    zgsz3216.zaccess_system_regs = zgaz359;
    zgsz3216.zaddress = zgaz383;
    zgsz3216.zflag_cap_mode = zgaz381;
    zgsz3216.zglobal = zgaz379;
    zgsz3216.zinternal_e = zinternal_exponent;
    zgsz3216.zotype = zotype;
    zgsz3216.zpermit_ccall = zgaz363;
    zgsz3216.zpermit_execute = zgaz377;
    zgsz3216.zpermit_load = zgaz375;
    zgsz3216.zpermit_load_cap = zgaz371;
    zgsz3216.zpermit_seal = zgaz365;
    zgsz3216.zpermit_set_CID = zgaz357;
    zgsz3216.zpermit_store = zgaz373;
    zgsz3216.zpermit_store_cap = zgaz369;
    zgsz3216.zpermit_store_local_cap = zgaz367;
    zgsz3216.zpermit_unseal = zgaz361;
    zgsz3216.zreserved = UINT64_C(0);
    zgsz3216.zsealed = zsealed;
    zgsz3216.ztag = zt;
    zgsz3216.zuperms = UINT64_C(0);
    zgaz384 = zgsz3216;
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  
  }
  zcbz334 = zgaz384;
  goto cleanup_132;
  /* unreachable after return */










  goto end_cleanup_133;
cleanup_132: ;










  goto end_function_131;
end_cleanup_133: ;
end_function_131: ;
  return zcbz334;
end_block_exception_134: ;
  struct zCapability zcbz354 = { .zuperms = UINT64_C(0xdeadc0de), .ztag = false, .zsealed = false, .zreserved = UINT64_C(0xdeadc0de), .zpermit_unseal = false, .zpermit_store_local_cap = false, .zpermit_store_cap = false, .zpermit_store = false, .zpermit_set_CID = false, .zpermit_seal = false, .zpermit_load_cap = false, .zpermit_load = false, .zpermit_execute = false, .zpermit_ccall = false, .zotype = UINT64_C(0xdeadc0de), .zinternal_e = false, .zglobal = false, .zflag_cap_mode = false, .zaddress = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zT = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  return zcbz354;
}

static int64_t zmax_otype;
static void create_letbind_19(void) {  


  int64_t zgsz3217;
  int64_t zgaz391;
  zgaz391 = sailgen_zzzz7nzzJzzK4z3MAX(zotype_width);
  {
    sail_int zgsz3218;
    CREATE(sail_int)(&zgsz3218);
    CONVERT_OF(sail_int, mach_int)(&zgsz3218, zgaz391);
    sail_int zgsz3219;
    CREATE(sail_int)(&zgsz3219);
    CONVERT_OF(sail_int, mach_int)(&zgsz3219, zreserved_otypes);
    sail_int zgsz3220;
    CREATE(sail_int)(&zgsz3220);
    sub_int(&zgsz3220, zgsz3218, zgsz3219);
    zgsz3217 = CONVERT_OF(mach_int, sail_int)(zgsz3220);
    KILL(sail_int)(&zgsz3220);
    KILL(sail_int)(&zgsz3219);
    KILL(sail_int)(&zgsz3218);
  }

  zmax_otype = zgsz3217;

let_end_135: ;
}
static void kill_letbind_19(void) {  
}

static uint64_t sailgen_getCapHardPerms(struct zCapability);

static uint64_t sailgen_getCapHardPerms(struct zCapability zcap)
{
  __label__ end_function_137, end_block_exception_138;

  uint64_t zcbz335;
  uint64_t zgaz3124;
  {
    bool zgaz392;
    zgaz392 = zcap.zpermit_set_CID;
    zgaz3124 = sailgen_bool_to_bits(zgaz392);
  
  }
  uint64_t zgaz3125;
  {
    uint64_t zgaz3122;
    {
      bool zgaz393;
      zgaz393 = zcap.zaccess_system_regs;
      zgaz3122 = sailgen_bool_to_bits(zgaz393);
    
    }
    uint64_t zgaz3123;
    {
      uint64_t zgaz3120;
      {
        bool zgaz394;
        zgaz394 = zcap.zpermit_unseal;
        zgaz3120 = sailgen_bool_to_bits(zgaz394);
      
      }
      uint64_t zgaz3121;
      {
        uint64_t zgaz3118;
        {
          bool zgaz395;
          zgaz395 = zcap.zpermit_ccall;
          zgaz3118 = sailgen_bool_to_bits(zgaz395);
        
        }
        uint64_t zgaz3119;
        {
          uint64_t zgaz3116;
          {
            bool zgaz396;
            zgaz396 = zcap.zpermit_seal;
            zgaz3116 = sailgen_bool_to_bits(zgaz396);
          
          }
          uint64_t zgaz3117;
          {
            uint64_t zgaz3114;
            {
              bool zgaz397;
              zgaz397 = zcap.zpermit_store_local_cap;
              zgaz3114 = sailgen_bool_to_bits(zgaz397);
            
            }
            uint64_t zgaz3115;
            {
              uint64_t zgaz3112;
              {
                bool zgaz398;
                zgaz398 = zcap.zpermit_store_cap;
                zgaz3112 = sailgen_bool_to_bits(zgaz398);
              
              }
              uint64_t zgaz3113;
              {
                uint64_t zgaz3110;
                {
                  bool zgaz399;
                  zgaz399 = zcap.zpermit_load_cap;
                  zgaz3110 = sailgen_bool_to_bits(zgaz399);
                
                }
                uint64_t zgaz3111;
                {
                  uint64_t zgaz3108;
                  {
                    bool zgaz3100;
                    zgaz3100 = zcap.zpermit_store;
                    zgaz3108 = sailgen_bool_to_bits(zgaz3100);
                  
                  }
                  uint64_t zgaz3109;
                  {
                    uint64_t zgaz3106;
                    {
                      bool zgaz3101;
                      zgaz3101 = zcap.zpermit_load;
                      zgaz3106 = sailgen_bool_to_bits(zgaz3101);
                    
                    }
                    uint64_t zgaz3107;
                    {
                      uint64_t zgaz3104;
                      {
                        bool zgaz3102;
                        zgaz3102 = zcap.zpermit_execute;
                        zgaz3104 = sailgen_bool_to_bits(zgaz3102);
                      
                      }
                      uint64_t zgaz3105;
                      {
                        bool zgaz3103;
                        zgaz3103 = zcap.zglobal;
                        zgaz3105 = sailgen_bool_to_bits(zgaz3103);
                      
                      }
                      {
                        lbits zgsz3221;
                        CREATE(lbits)(&zgsz3221);
                        CONVERT_OF(lbits, fbits)(&zgsz3221, zgaz3104, UINT64_C(1) , true);
                        lbits zgsz3222;
                        CREATE(lbits)(&zgsz3222);
                        CONVERT_OF(lbits, fbits)(&zgsz3222, zgaz3105, UINT64_C(1) , true);
                        lbits zgsz3223;
                        CREATE(lbits)(&zgsz3223);
                        append(&zgsz3223, zgsz3221, zgsz3222);
                        zgaz3107 = CONVERT_OF(fbits, lbits)(zgsz3223, true);
                        KILL(lbits)(&zgsz3223);
                        KILL(lbits)(&zgsz3222);
                        KILL(lbits)(&zgsz3221);
                      }
                    
                    
                    }
                    {
                      lbits zgsz3224;
                      CREATE(lbits)(&zgsz3224);
                      CONVERT_OF(lbits, fbits)(&zgsz3224, zgaz3106, UINT64_C(1) , true);
                      lbits zgsz3225;
                      CREATE(lbits)(&zgsz3225);
                      CONVERT_OF(lbits, fbits)(&zgsz3225, zgaz3107, UINT64_C(2) , true);
                      lbits zgsz3226;
                      CREATE(lbits)(&zgsz3226);
                      append(&zgsz3226, zgsz3224, zgsz3225);
                      zgaz3109 = CONVERT_OF(fbits, lbits)(zgsz3226, true);
                      KILL(lbits)(&zgsz3226);
                      KILL(lbits)(&zgsz3225);
                      KILL(lbits)(&zgsz3224);
                    }
                  
                  
                  }
                  {
                    lbits zgsz3227;
                    CREATE(lbits)(&zgsz3227);
                    CONVERT_OF(lbits, fbits)(&zgsz3227, zgaz3108, UINT64_C(1) , true);
                    lbits zgsz3228;
                    CREATE(lbits)(&zgsz3228);
                    CONVERT_OF(lbits, fbits)(&zgsz3228, zgaz3109, UINT64_C(3) , true);
                    lbits zgsz3229;
                    CREATE(lbits)(&zgsz3229);
                    append(&zgsz3229, zgsz3227, zgsz3228);
                    zgaz3111 = CONVERT_OF(fbits, lbits)(zgsz3229, true);
                    KILL(lbits)(&zgsz3229);
                    KILL(lbits)(&zgsz3228);
                    KILL(lbits)(&zgsz3227);
                  }
                
                
                }
                {
                  lbits zgsz3230;
                  CREATE(lbits)(&zgsz3230);
                  CONVERT_OF(lbits, fbits)(&zgsz3230, zgaz3110, UINT64_C(1) , true);
                  lbits zgsz3231;
                  CREATE(lbits)(&zgsz3231);
                  CONVERT_OF(lbits, fbits)(&zgsz3231, zgaz3111, UINT64_C(4) , true);
                  lbits zgsz3232;
                  CREATE(lbits)(&zgsz3232);
                  append(&zgsz3232, zgsz3230, zgsz3231);
                  zgaz3113 = CONVERT_OF(fbits, lbits)(zgsz3232, true);
                  KILL(lbits)(&zgsz3232);
                  KILL(lbits)(&zgsz3231);
                  KILL(lbits)(&zgsz3230);
                }
              
              
              }
              {
                lbits zgsz3233;
                CREATE(lbits)(&zgsz3233);
                CONVERT_OF(lbits, fbits)(&zgsz3233, zgaz3112, UINT64_C(1) , true);
                lbits zgsz3234;
                CREATE(lbits)(&zgsz3234);
                CONVERT_OF(lbits, fbits)(&zgsz3234, zgaz3113, UINT64_C(5) , true);
                lbits zgsz3235;
                CREATE(lbits)(&zgsz3235);
                append(&zgsz3235, zgsz3233, zgsz3234);
                zgaz3115 = CONVERT_OF(fbits, lbits)(zgsz3235, true);
                KILL(lbits)(&zgsz3235);
                KILL(lbits)(&zgsz3234);
                KILL(lbits)(&zgsz3233);
              }
            
            
            }
            {
              lbits zgsz3236;
              CREATE(lbits)(&zgsz3236);
              CONVERT_OF(lbits, fbits)(&zgsz3236, zgaz3114, UINT64_C(1) , true);
              lbits zgsz3237;
              CREATE(lbits)(&zgsz3237);
              CONVERT_OF(lbits, fbits)(&zgsz3237, zgaz3115, UINT64_C(6) , true);
              lbits zgsz3238;
              CREATE(lbits)(&zgsz3238);
              append(&zgsz3238, zgsz3236, zgsz3237);
              zgaz3117 = CONVERT_OF(fbits, lbits)(zgsz3238, true);
              KILL(lbits)(&zgsz3238);
              KILL(lbits)(&zgsz3237);
              KILL(lbits)(&zgsz3236);
            }
          
          
          }
          {
            lbits zgsz3239;
            CREATE(lbits)(&zgsz3239);
            CONVERT_OF(lbits, fbits)(&zgsz3239, zgaz3116, UINT64_C(1) , true);
            lbits zgsz3240;
            CREATE(lbits)(&zgsz3240);
            CONVERT_OF(lbits, fbits)(&zgsz3240, zgaz3117, UINT64_C(7) , true);
            lbits zgsz3241;
            CREATE(lbits)(&zgsz3241);
            append(&zgsz3241, zgsz3239, zgsz3240);
            zgaz3119 = CONVERT_OF(fbits, lbits)(zgsz3241, true);
            KILL(lbits)(&zgsz3241);
            KILL(lbits)(&zgsz3240);
            KILL(lbits)(&zgsz3239);
          }
        
        
        }
        {
          lbits zgsz3242;
          CREATE(lbits)(&zgsz3242);
          CONVERT_OF(lbits, fbits)(&zgsz3242, zgaz3118, UINT64_C(1) , true);
          lbits zgsz3243;
          CREATE(lbits)(&zgsz3243);
          CONVERT_OF(lbits, fbits)(&zgsz3243, zgaz3119, UINT64_C(8) , true);
          lbits zgsz3244;
          CREATE(lbits)(&zgsz3244);
          append(&zgsz3244, zgsz3242, zgsz3243);
          zgaz3121 = CONVERT_OF(fbits, lbits)(zgsz3244, true);
          KILL(lbits)(&zgsz3244);
          KILL(lbits)(&zgsz3243);
          KILL(lbits)(&zgsz3242);
        }
      
      
      }
      {
        lbits zgsz3245;
        CREATE(lbits)(&zgsz3245);
        CONVERT_OF(lbits, fbits)(&zgsz3245, zgaz3120, UINT64_C(1) , true);
        lbits zgsz3246;
        CREATE(lbits)(&zgsz3246);
        CONVERT_OF(lbits, fbits)(&zgsz3246, zgaz3121, UINT64_C(9) , true);
        lbits zgsz3247;
        CREATE(lbits)(&zgsz3247);
        append(&zgsz3247, zgsz3245, zgsz3246);
        zgaz3123 = CONVERT_OF(fbits, lbits)(zgsz3247, true);
        KILL(lbits)(&zgsz3247);
        KILL(lbits)(&zgsz3246);
        KILL(lbits)(&zgsz3245);
      }
    
    
    }
    {
      lbits zgsz3248;
      CREATE(lbits)(&zgsz3248);
      CONVERT_OF(lbits, fbits)(&zgsz3248, zgaz3122, UINT64_C(1) , true);
      lbits zgsz3249;
      CREATE(lbits)(&zgsz3249);
      CONVERT_OF(lbits, fbits)(&zgsz3249, zgaz3123, UINT64_C(10) , true);
      lbits zgsz3250;
      CREATE(lbits)(&zgsz3250);
      append(&zgsz3250, zgsz3248, zgsz3249);
      zgaz3125 = CONVERT_OF(fbits, lbits)(zgsz3250, true);
      KILL(lbits)(&zgsz3250);
      KILL(lbits)(&zgsz3249);
      KILL(lbits)(&zgsz3248);
    }
  
  
  }
  {
    lbits zgsz3251;
    CREATE(lbits)(&zgsz3251);
    CONVERT_OF(lbits, fbits)(&zgsz3251, zgaz3124, UINT64_C(1) , true);
    lbits zgsz3252;
    CREATE(lbits)(&zgsz3252);
    CONVERT_OF(lbits, fbits)(&zgsz3252, zgaz3125, UINT64_C(11) , true);
    lbits zgsz3253;
    CREATE(lbits)(&zgsz3253);
    append(&zgsz3253, zgsz3251, zgsz3252);
    zcbz335 = CONVERT_OF(fbits, lbits)(zgsz3253, true);
    KILL(lbits)(&zgsz3253);
    KILL(lbits)(&zgsz3252);
    KILL(lbits)(&zgsz3251);
  }


end_function_137: ;
  return zcbz335;
end_block_exception_138: ;

  return UINT64_C(0xdeadc0de);
}

static uint64_t sailgen_capToBits(struct zCapability);

static uint64_t sailgen_capToBits(struct zCapability zcap)
{
  __label__ cleanup_141, end_cleanup_142, end_function_140, end_block_exception_143;

  uint64_t zcbz336;
  uint64_t zt_hi;
  {
    uint64_t zgaz3152;
    zgaz3152 = zcap.zT;
    int64_t zgaz3153;
    {
      sail_int zgsz3254;
      CREATE(sail_int)(&zgsz3254);
      CONVERT_OF(sail_int, mach_int)(&zgsz3254, zmantissa_width);
      sail_int zgsz3255;
      CREATE(sail_int)(&zgsz3255);
      CONVERT_OF(sail_int, mach_int)(&zgsz3255, INT64_C(3));
      sail_int zgsz3256;
      CREATE(sail_int)(&zgsz3256);
      sub_int(&zgsz3256, zgsz3254, zgsz3255);
      zgaz3153 = CONVERT_OF(mach_int, sail_int)(zgsz3256);
      KILL(sail_int)(&zgsz3256);
      KILL(sail_int)(&zgsz3255);
      KILL(sail_int)(&zgsz3254);
    }
    {
      lbits zgsz3257;
      CREATE(lbits)(&zgsz3257);
      CONVERT_OF(lbits, fbits)(&zgsz3257, zgaz3152, UINT64_C(8) , true);
      sail_int zgsz3258;
      CREATE(sail_int)(&zgsz3258);
      CONVERT_OF(sail_int, mach_int)(&zgsz3258, zgaz3153);
      sail_int zgsz3259;
      CREATE(sail_int)(&zgsz3259);
      CONVERT_OF(sail_int, mach_int)(&zgsz3259, INT64_C(3));
      lbits zgsz3260;
      CREATE(lbits)(&zgsz3260);
      vector_subrange_lbits(&zgsz3260, zgsz3257, zgsz3258, zgsz3259);
      zt_hi = CONVERT_OF(fbits, lbits)(zgsz3260, true);
      KILL(lbits)(&zgsz3260);
      KILL(sail_int)(&zgsz3259);
      KILL(sail_int)(&zgsz3258);
      KILL(lbits)(&zgsz3257);
    }
  
  
  }
  uint64_t zt_lo;
  {
    uint64_t zgaz3151;
    zgaz3151 = zcap.zT;
    {
      lbits zgsz3261;
      CREATE(lbits)(&zgsz3261);
      CONVERT_OF(lbits, fbits)(&zgsz3261, zgaz3151, UINT64_C(8) , true);
      sail_int zgsz3262;
      CREATE(sail_int)(&zgsz3262);
      CONVERT_OF(sail_int, mach_int)(&zgsz3262, INT64_C(2));
      sail_int zgsz3263;
      CREATE(sail_int)(&zgsz3263);
      CONVERT_OF(sail_int, mach_int)(&zgsz3263, INT64_C(0));
      lbits zgsz3264;
      CREATE(lbits)(&zgsz3264);
      vector_subrange_lbits(&zgsz3264, zgsz3261, zgsz3262, zgsz3263);
      zt_lo = CONVERT_OF(fbits, lbits)(zgsz3264, true);
      KILL(lbits)(&zgsz3264);
      KILL(sail_int)(&zgsz3263);
      KILL(sail_int)(&zgsz3262);
      KILL(lbits)(&zgsz3261);
    }
  
  }
  uint64_t zb_hi;
  {
    uint64_t zgaz3149;
    zgaz3149 = zcap.zB;
    int64_t zgaz3150;
    {
      sail_int zgsz3265;
      CREATE(sail_int)(&zgsz3265);
      CONVERT_OF(sail_int, mach_int)(&zgsz3265, zmantissa_width);
      sail_int zgsz3266;
      CREATE(sail_int)(&zgsz3266);
      CONVERT_OF(sail_int, mach_int)(&zgsz3266, INT64_C(1));
      sail_int zgsz3267;
      CREATE(sail_int)(&zgsz3267);
      sub_int(&zgsz3267, zgsz3265, zgsz3266);
      zgaz3150 = CONVERT_OF(mach_int, sail_int)(zgsz3267);
      KILL(sail_int)(&zgsz3267);
      KILL(sail_int)(&zgsz3266);
      KILL(sail_int)(&zgsz3265);
    }
    {
      lbits zgsz3268;
      CREATE(lbits)(&zgsz3268);
      CONVERT_OF(lbits, fbits)(&zgsz3268, zgaz3149, UINT64_C(8) , true);
      sail_int zgsz3269;
      CREATE(sail_int)(&zgsz3269);
      CONVERT_OF(sail_int, mach_int)(&zgsz3269, zgaz3150);
      sail_int zgsz3270;
      CREATE(sail_int)(&zgsz3270);
      CONVERT_OF(sail_int, mach_int)(&zgsz3270, INT64_C(3));
      lbits zgsz3271;
      CREATE(lbits)(&zgsz3271);
      vector_subrange_lbits(&zgsz3271, zgsz3268, zgsz3269, zgsz3270);
      zb_hi = CONVERT_OF(fbits, lbits)(zgsz3271, true);
      KILL(lbits)(&zgsz3271);
      KILL(sail_int)(&zgsz3270);
      KILL(sail_int)(&zgsz3269);
      KILL(lbits)(&zgsz3268);
    }
  
  
  }
  uint64_t zb_lo;
  {
    uint64_t zgaz3148;
    zgaz3148 = zcap.zB;
    {
      lbits zgsz3272;
      CREATE(lbits)(&zgsz3272);
      CONVERT_OF(lbits, fbits)(&zgsz3272, zgaz3148, UINT64_C(8) , true);
      sail_int zgsz3273;
      CREATE(sail_int)(&zgsz3273);
      CONVERT_OF(sail_int, mach_int)(&zgsz3273, INT64_C(2));
      sail_int zgsz3274;
      CREATE(sail_int)(&zgsz3274);
      CONVERT_OF(sail_int, mach_int)(&zgsz3274, INT64_C(0));
      lbits zgsz3275;
      CREATE(lbits)(&zgsz3275);
      vector_subrange_lbits(&zgsz3275, zgsz3272, zgsz3273, zgsz3274);
      zb_lo = CONVERT_OF(fbits, lbits)(zgsz3275, true);
      KILL(lbits)(&zgsz3275);
      KILL(sail_int)(&zgsz3274);
      KILL(sail_int)(&zgsz3273);
      KILL(lbits)(&zgsz3272);
    }
  
  }
  {
    bool zgaz3126;
    zgaz3126 = zcap.zinternal_e;
    unit zgsz3276;
    if (zgaz3126) {
    {
      uint64_t zgaz3127;
      zgaz3127 = zcap.zE;
      {
        lbits zgsz3277;
        CREATE(lbits)(&zgsz3277);
        CONVERT_OF(lbits, fbits)(&zgsz3277, zgaz3127, UINT64_C(6) , true);
        sail_int zgsz3278;
        CREATE(sail_int)(&zgsz3278);
        CONVERT_OF(sail_int, mach_int)(&zgsz3278, INT64_C(5));
        sail_int zgsz3279;
        CREATE(sail_int)(&zgsz3279);
        CONVERT_OF(sail_int, mach_int)(&zgsz3279, INT64_C(3));
        lbits zgsz3280;
        CREATE(lbits)(&zgsz3280);
        vector_subrange_lbits(&zgsz3280, zgsz3277, zgsz3278, zgsz3279);
        zt_lo = CONVERT_OF(fbits, lbits)(zgsz3280, true);
        KILL(lbits)(&zgsz3280);
        KILL(sail_int)(&zgsz3279);
        KILL(sail_int)(&zgsz3278);
        KILL(lbits)(&zgsz3277);
      }
      unit zgsz3281;
      zgsz3281 = UNIT;
    
    }
    uint64_t zgaz3128;
    zgaz3128 = zcap.zE;
    {
      lbits zgsz3282;
      CREATE(lbits)(&zgsz3282);
      CONVERT_OF(lbits, fbits)(&zgsz3282, zgaz3128, UINT64_C(6) , true);
      sail_int zgsz3283;
      CREATE(sail_int)(&zgsz3283);
      CONVERT_OF(sail_int, mach_int)(&zgsz3283, INT64_C(2));
      sail_int zgsz3284;
      CREATE(sail_int)(&zgsz3284);
      CONVERT_OF(sail_int, mach_int)(&zgsz3284, INT64_C(0));
      lbits zgsz3285;
      CREATE(lbits)(&zgsz3285);
      vector_subrange_lbits(&zgsz3285, zgsz3282, zgsz3283, zgsz3284);
      zb_lo = CONVERT_OF(fbits, lbits)(zgsz3285, true);
      KILL(lbits)(&zgsz3285);
      KILL(sail_int)(&zgsz3284);
      KILL(sail_int)(&zgsz3283);
      KILL(lbits)(&zgsz3282);
    }
    zgsz3276 = UNIT;
  
    } else {  zgsz3276 = UNIT;  }
  
  }
  uint64_t zgaz3147;
  {
    uint64_t zgaz3145;
    zgaz3145 = zcap.zuperms;
    uint64_t zgaz3146;
    {
      uint64_t zgaz3143;
      zgaz3143 = sailgen_getCapHardPerms(zcap);
      uint64_t zgaz3144;
      {
        uint64_t zgaz3141;
        zgaz3141 = zcap.zreserved;
        uint64_t zgaz3142;
        {
          uint64_t zgaz3139;
          {
            bool zgaz3129;
            zgaz3129 = zcap.zflag_cap_mode;
            zgaz3139 = sailgen_bool_to_bits(zgaz3129);
          
          }
          uint64_t zgaz3140;
          {
            uint64_t zgaz3137;
            zgaz3137 = zcap.zotype;
            uint64_t zgaz3138;
            {
              uint64_t zgaz3135;
              {
                bool zgaz3130;
                zgaz3130 = zcap.zinternal_e;
                zgaz3135 = sailgen_bool_to_bits(zgaz3130);
              
              }
              uint64_t zgaz3136;
              {
                uint64_t zgaz3134;
                {
                  uint64_t zgaz3133;
                  {
                    uint64_t zgaz3132;
                    {
                      uint64_t zgaz3131;
                      zgaz3131 = zcap.zaddress;
                      {
                        lbits zgsz3286;
                        CREATE(lbits)(&zgsz3286);
                        CONVERT_OF(lbits, fbits)(&zgsz3286, zb_lo, UINT64_C(3) , true);
                        lbits zgsz3287;
                        CREATE(lbits)(&zgsz3287);
                        CONVERT_OF(lbits, fbits)(&zgsz3287, zgaz3131, UINT64_C(32) , true);
                        lbits zgsz3288;
                        CREATE(lbits)(&zgsz3288);
                        append(&zgsz3288, zgsz3286, zgsz3287);
                        zgaz3132 = CONVERT_OF(fbits, lbits)(zgsz3288, true);
                        KILL(lbits)(&zgsz3288);
                        KILL(lbits)(&zgsz3287);
                        KILL(lbits)(&zgsz3286);
                      }
                    
                    }
                    {
                      lbits zgsz3289;
                      CREATE(lbits)(&zgsz3289);
                      CONVERT_OF(lbits, fbits)(&zgsz3289, zb_hi, UINT64_C(5) , true);
                      lbits zgsz3290;
                      CREATE(lbits)(&zgsz3290);
                      CONVERT_OF(lbits, fbits)(&zgsz3290, zgaz3132, UINT64_C(35) , true);
                      lbits zgsz3291;
                      CREATE(lbits)(&zgsz3291);
                      append(&zgsz3291, zgsz3289, zgsz3290);
                      zgaz3133 = CONVERT_OF(fbits, lbits)(zgsz3291, true);
                      KILL(lbits)(&zgsz3291);
                      KILL(lbits)(&zgsz3290);
                      KILL(lbits)(&zgsz3289);
                    }
                  
                  }
                  {
                    lbits zgsz3292;
                    CREATE(lbits)(&zgsz3292);
                    CONVERT_OF(lbits, fbits)(&zgsz3292, zt_lo, UINT64_C(3) , true);
                    lbits zgsz3293;
                    CREATE(lbits)(&zgsz3293);
                    CONVERT_OF(lbits, fbits)(&zgsz3293, zgaz3133, UINT64_C(40) , true);
                    lbits zgsz3294;
                    CREATE(lbits)(&zgsz3294);
                    append(&zgsz3294, zgsz3292, zgsz3293);
                    zgaz3134 = CONVERT_OF(fbits, lbits)(zgsz3294, true);
                    KILL(lbits)(&zgsz3294);
                    KILL(lbits)(&zgsz3293);
                    KILL(lbits)(&zgsz3292);
                  }
                
                }
                {
                  lbits zgsz3295;
                  CREATE(lbits)(&zgsz3295);
                  CONVERT_OF(lbits, fbits)(&zgsz3295, zt_hi, UINT64_C(3) , true);
                  lbits zgsz3296;
                  CREATE(lbits)(&zgsz3296);
                  CONVERT_OF(lbits, fbits)(&zgsz3296, zgaz3134, UINT64_C(43) , true);
                  lbits zgsz3297;
                  CREATE(lbits)(&zgsz3297);
                  append(&zgsz3297, zgsz3295, zgsz3296);
                  zgaz3136 = CONVERT_OF(fbits, lbits)(zgsz3297, true);
                  KILL(lbits)(&zgsz3297);
                  KILL(lbits)(&zgsz3296);
                  KILL(lbits)(&zgsz3295);
                }
              
              }
              {
                lbits zgsz3298;
                CREATE(lbits)(&zgsz3298);
                CONVERT_OF(lbits, fbits)(&zgsz3298, zgaz3135, UINT64_C(1) , true);
                lbits zgsz3299;
                CREATE(lbits)(&zgsz3299);
                CONVERT_OF(lbits, fbits)(&zgsz3299, zgaz3136, UINT64_C(46) , true);
                lbits zgsz3300;
                CREATE(lbits)(&zgsz3300);
                append(&zgsz3300, zgsz3298, zgsz3299);
                zgaz3138 = CONVERT_OF(fbits, lbits)(zgsz3300, true);
                KILL(lbits)(&zgsz3300);
                KILL(lbits)(&zgsz3299);
                KILL(lbits)(&zgsz3298);
              }
            
            
            }
            {
              lbits zgsz3301;
              CREATE(lbits)(&zgsz3301);
              CONVERT_OF(lbits, fbits)(&zgsz3301, zgaz3137, UINT64_C(4) , true);
              lbits zgsz3302;
              CREATE(lbits)(&zgsz3302);
              CONVERT_OF(lbits, fbits)(&zgsz3302, zgaz3138, UINT64_C(47) , true);
              lbits zgsz3303;
              CREATE(lbits)(&zgsz3303);
              append(&zgsz3303, zgsz3301, zgsz3302);
              zgaz3140 = CONVERT_OF(fbits, lbits)(zgsz3303, true);
              KILL(lbits)(&zgsz3303);
              KILL(lbits)(&zgsz3302);
              KILL(lbits)(&zgsz3301);
            }
          
          
          }
          {
            lbits zgsz3304;
            CREATE(lbits)(&zgsz3304);
            CONVERT_OF(lbits, fbits)(&zgsz3304, zgaz3139, UINT64_C(1) , true);
            lbits zgsz3305;
            CREATE(lbits)(&zgsz3305);
            CONVERT_OF(lbits, fbits)(&zgsz3305, zgaz3140, UINT64_C(51) , true);
            lbits zgsz3306;
            CREATE(lbits)(&zgsz3306);
            append(&zgsz3306, zgsz3304, zgsz3305);
            zgaz3142 = CONVERT_OF(fbits, lbits)(zgsz3306, true);
            KILL(lbits)(&zgsz3306);
            KILL(lbits)(&zgsz3305);
            KILL(lbits)(&zgsz3304);
          }
        
        
        }
        {
          lbits zgsz3307;
          CREATE(lbits)(&zgsz3307);
          CONVERT_OF(lbits, fbits)(&zgsz3307, zgaz3141, UINT64_C(0) , true);
          lbits zgsz3308;
          CREATE(lbits)(&zgsz3308);
          CONVERT_OF(lbits, fbits)(&zgsz3308, zgaz3142, UINT64_C(52) , true);
          lbits zgsz3309;
          CREATE(lbits)(&zgsz3309);
          append(&zgsz3309, zgsz3307, zgsz3308);
          zgaz3144 = CONVERT_OF(fbits, lbits)(zgsz3309, true);
          KILL(lbits)(&zgsz3309);
          KILL(lbits)(&zgsz3308);
          KILL(lbits)(&zgsz3307);
        }
      
      
      }
      {
        lbits zgsz3310;
        CREATE(lbits)(&zgsz3310);
        CONVERT_OF(lbits, fbits)(&zgsz3310, zgaz3143, UINT64_C(12) , true);
        lbits zgsz3311;
        CREATE(lbits)(&zgsz3311);
        CONVERT_OF(lbits, fbits)(&zgsz3311, zgaz3144, UINT64_C(52) , true);
        lbits zgsz3312;
        CREATE(lbits)(&zgsz3312);
        append(&zgsz3312, zgsz3310, zgsz3311);
        zgaz3146 = CONVERT_OF(fbits, lbits)(zgsz3312, true);
        KILL(lbits)(&zgsz3312);
        KILL(lbits)(&zgsz3311);
        KILL(lbits)(&zgsz3310);
      }
    
    
    }
    {
      lbits zgsz3313;
      CREATE(lbits)(&zgsz3313);
      CONVERT_OF(lbits, fbits)(&zgsz3313, zgaz3145, UINT64_C(0) , true);
      lbits zgsz3314;
      CREATE(lbits)(&zgsz3314);
      CONVERT_OF(lbits, fbits)(&zgsz3314, zgaz3146, UINT64_C(64) , true);
      lbits zgsz3315;
      CREATE(lbits)(&zgsz3315);
      append(&zgsz3315, zgsz3313, zgsz3314);
      zgaz3147 = CONVERT_OF(fbits, lbits)(zgsz3315, true);
      KILL(lbits)(&zgsz3315);
      KILL(lbits)(&zgsz3314);
      KILL(lbits)(&zgsz3313);
    }
  
  
  }
  zcbz336 = zgaz3147;
  goto cleanup_141;
  /* unreachable after return */





  goto end_cleanup_142;
cleanup_141: ;





  goto end_function_140;
end_cleanup_142: ;
end_function_140: ;
  return zcbz336;
end_block_exception_143: ;

  return UINT64_C(0xdeadc0de);
}

static uint64_t znull_cap_bits;
static void create_letbind_20(void) {  


  uint64_t zgsz3316;
  zgsz3316 = sailgen_capToBits(znull_cap);
  znull_cap_bits = zgsz3316;

let_end_144: ;
}
static void kill_letbind_20(void) {  
}

static uint64_t sailgen_capToMemBits(struct zCapability);

static uint64_t sailgen_capToMemBits(struct zCapability zcap)
{
  __label__ end_function_146, end_block_exception_147;

  uint64_t zcbz337;
  uint64_t zgaz3154;
  zgaz3154 = sailgen_capToBits(zcap);
  {
    lbits zgsz3317;
    CREATE(lbits)(&zgsz3317);
    CONVERT_OF(lbits, fbits)(&zgsz3317, zgaz3154, UINT64_C(64) , true);
    lbits zgsz3318;
    CREATE(lbits)(&zgsz3318);
    CONVERT_OF(lbits, fbits)(&zgsz3318, znull_cap_bits, UINT64_C(64) , true);
    lbits zgsz3319;
    CREATE(lbits)(&zgsz3319);
    xor_bits(&zgsz3319, zgsz3317, zgsz3318);
    zcbz337 = CONVERT_OF(fbits, lbits)(zgsz3319, true);
    KILL(lbits)(&zgsz3319);
    KILL(lbits)(&zgsz3318);
    KILL(lbits)(&zgsz3317);
  }

end_function_146: ;
  return zcbz337;
end_block_exception_147: ;

  return UINT64_C(0xdeadc0de);
}

static struct zCapability sailgen_memBitsToCapability(bool, uint64_t);

static struct zCapability sailgen_memBitsToCapability(bool ztag, uint64_t zb)
{
  __label__ end_function_149, end_block_exception_150;

  struct zCapability zcbz338;
  uint64_t zgaz3155;
  {
    lbits zgsz3320;
    CREATE(lbits)(&zgsz3320);
    CONVERT_OF(lbits, fbits)(&zgsz3320, zb, UINT64_C(64) , true);
    lbits zgsz3321;
    CREATE(lbits)(&zgsz3321);
    CONVERT_OF(lbits, fbits)(&zgsz3321, znull_cap_bits, UINT64_C(64) , true);
    lbits zgsz3322;
    CREATE(lbits)(&zgsz3322);
    xor_bits(&zgsz3322, zgsz3320, zgsz3321);
    zgaz3155 = CONVERT_OF(fbits, lbits)(zgsz3322, true);
    KILL(lbits)(&zgsz3322);
    KILL(lbits)(&zgsz3321);
    KILL(lbits)(&zgsz3320);
  }
  zcbz338 = sailgen_capBitsToCapability(ztag, zgaz3155);

end_function_149: ;
  return zcbz338;
end_block_exception_150: ;
  struct zCapability zcbz355 = { .zuperms = UINT64_C(0xdeadc0de), .ztag = false, .zsealed = false, .zreserved = UINT64_C(0xdeadc0de), .zpermit_unseal = false, .zpermit_store_local_cap = false, .zpermit_store_cap = false, .zpermit_store = false, .zpermit_set_CID = false, .zpermit_seal = false, .zpermit_load_cap = false, .zpermit_load = false, .zpermit_execute = false, .zpermit_ccall = false, .zotype = UINT64_C(0xdeadc0de), .zinternal_e = false, .zglobal = false, .zflag_cap_mode = false, .zaddress = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zT = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  return zcbz355;
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

static struct ztuple_z8z5bv32zCz0z5bv33z9 sailgen_getCapBoundsBits(struct zCapability);

static struct ztuple_z8z5bv32zCz0z5bv33z9 sailgen_getCapBoundsBits(struct zCapability zc)
{
  __label__ end_function_152, end_block_exception_153;

  struct ztuple_z8z5bv32zCz0z5bv33z9 zcbz339;
  int64_t zE;
  {
    int64_t zgaz3189;
    {
      uint64_t zgaz3188;
      zgaz3188 = zc.zE;
      zgaz3189 = ((mach_int) zgaz3188);
    
    }
    {
      sail_int zgsz3323;
      CREATE(sail_int)(&zgsz3323);
      CONVERT_OF(sail_int, mach_int)(&zgsz3323, zmaxE);
      sail_int zgsz3324;
      CREATE(sail_int)(&zgsz3324);
      CONVERT_OF(sail_int, mach_int)(&zgsz3324, zgaz3189);
      sail_int zgsz3325;
      CREATE(sail_int)(&zgsz3325);
      min_int(&zgsz3325, zgsz3323, zgsz3324);
      zE = CONVERT_OF(mach_int, sail_int)(zgsz3325);
      KILL(sail_int)(&zgsz3325);
      KILL(sail_int)(&zgsz3324);
      KILL(sail_int)(&zgsz3323);
    }
  
  }
  uint64_t za;
  za = zc.zaddress;
  uint64_t za3;
  {
    uint64_t zgaz3187;
    {
      int64_t zgaz3186;
      {
        int64_t zgaz3185;
        {
          sail_int zgsz3326;
          CREATE(sail_int)(&zgsz3326);
          CONVERT_OF(sail_int, mach_int)(&zgsz3326, zE);
          sail_int zgsz3327;
          CREATE(sail_int)(&zgsz3327);
          CONVERT_OF(sail_int, mach_int)(&zgsz3327, zmantissa_width);
          sail_int zgsz3328;
          CREATE(sail_int)(&zgsz3328);
          add_int(&zgsz3328, zgsz3326, zgsz3327);
          zgaz3185 = CONVERT_OF(mach_int, sail_int)(zgsz3328);
          KILL(sail_int)(&zgsz3328);
          KILL(sail_int)(&zgsz3327);
          KILL(sail_int)(&zgsz3326);
        }
        {
          sail_int zgsz3329;
          CREATE(sail_int)(&zgsz3329);
          CONVERT_OF(sail_int, mach_int)(&zgsz3329, zgaz3185);
          sail_int zgsz3330;
          CREATE(sail_int)(&zgsz3330);
          CONVERT_OF(sail_int, mach_int)(&zgsz3330, INT64_C(3));
          sail_int zgsz3331;
          CREATE(sail_int)(&zgsz3331);
          sub_int(&zgsz3331, zgsz3329, zgsz3330);
          zgaz3186 = CONVERT_OF(mach_int, sail_int)(zgsz3331);
          KILL(sail_int)(&zgsz3331);
          KILL(sail_int)(&zgsz3330);
          KILL(sail_int)(&zgsz3329);
        }
      
      }
      {
        lbits zgsz3332;
        CREATE(lbits)(&zgsz3332);
        CONVERT_OF(lbits, fbits)(&zgsz3332, za, UINT64_C(32) , true);
        sail_int zgsz3333;
        CREATE(sail_int)(&zgsz3333);
        CONVERT_OF(sail_int, mach_int)(&zgsz3333, zgaz3186);
        lbits zgsz3334;
        CREATE(lbits)(&zgsz3334);
        shiftr(&zgsz3334, zgsz3332, zgsz3333);
        zgaz3187 = CONVERT_OF(fbits, lbits)(zgsz3334, true);
        KILL(lbits)(&zgsz3334);
        KILL(sail_int)(&zgsz3333);
        KILL(lbits)(&zgsz3332);
      }
    
    }
    {
      lbits zgsz3335;
      CREATE(lbits)(&zgsz3335);
      CONVERT_OF(lbits, fbits)(&zgsz3335, zgaz3187, UINT64_C(32) , true);
      sail_int zgsz3336;
      CREATE(sail_int)(&zgsz3336);
      CONVERT_OF(sail_int, mach_int)(&zgsz3336, INT64_C(3));
      lbits zgsz3337;
      CREATE(lbits)(&zgsz3337);
      sail_truncate(&zgsz3337, zgsz3335, zgsz3336);
      za3 = CONVERT_OF(fbits, lbits)(zgsz3337, true);
      KILL(lbits)(&zgsz3337);
      KILL(sail_int)(&zgsz3336);
      KILL(lbits)(&zgsz3335);
    }
  
  }
  uint64_t zB3;
  {
    uint64_t zgaz3184;
    zgaz3184 = zc.zB;
    {
      lbits zgsz3338;
      CREATE(lbits)(&zgsz3338);
      CONVERT_OF(lbits, fbits)(&zgsz3338, zgaz3184, UINT64_C(8) , true);
      sail_int zgsz3339;
      CREATE(sail_int)(&zgsz3339);
      CONVERT_OF(sail_int, mach_int)(&zgsz3339, INT64_C(3));
      lbits zgsz3340;
      CREATE(lbits)(&zgsz3340);
      sail_truncateLSB(&zgsz3340, zgsz3338, zgsz3339);
      zB3 = CONVERT_OF(fbits, lbits)(zgsz3340, true);
      KILL(lbits)(&zgsz3340);
      KILL(sail_int)(&zgsz3339);
      KILL(lbits)(&zgsz3338);
    }
  
  }
  uint64_t zT3;
  {
    uint64_t zgaz3183;
    zgaz3183 = zc.zT;
    {
      lbits zgsz3341;
      CREATE(lbits)(&zgsz3341);
      CONVERT_OF(lbits, fbits)(&zgsz3341, zgaz3183, UINT64_C(8) , true);
      sail_int zgsz3342;
      CREATE(sail_int)(&zgsz3342);
      CONVERT_OF(sail_int, mach_int)(&zgsz3342, INT64_C(3));
      lbits zgsz3343;
      CREATE(lbits)(&zgsz3343);
      sail_truncateLSB(&zgsz3343, zgsz3341, zgsz3342);
      zT3 = CONVERT_OF(fbits, lbits)(zgsz3343, true);
      KILL(lbits)(&zgsz3343);
      KILL(sail_int)(&zgsz3342);
      KILL(lbits)(&zgsz3341);
    }
  
  }
  uint64_t zR3;
  {
    lbits zgsz3344;
    CREATE(lbits)(&zgsz3344);
    CONVERT_OF(lbits, fbits)(&zgsz3344, zB3, UINT64_C(3) , true);
    lbits zgsz3345;
    CREATE(lbits)(&zgsz3345);
    CONVERT_OF(lbits, fbits)(&zgsz3345, UINT64_C(0b001), UINT64_C(3) , true);
    lbits zgsz3346;
    CREATE(lbits)(&zgsz3346);
    sub_bits(&zgsz3346, zgsz3344, zgsz3345);
    zR3 = CONVERT_OF(fbits, lbits)(zgsz3346, true);
    KILL(lbits)(&zgsz3346);
    KILL(lbits)(&zgsz3345);
    KILL(lbits)(&zgsz3344);
  }
  int64_t zaHi;
  {
    bool zgaz3182;
    zgaz3182 = sailgen_z8operatorz0zzzz7nzzJzzK3z3zI_uz9(za3, zR3);
    if (zgaz3182) {  zaHi = INT64_C(1);  } else {  zaHi = INT64_C(0);  }
  
  }
  int64_t zbHi;
  {
    bool zgaz3181;
    zgaz3181 = sailgen_z8operatorz0zzzz7nzzJzzK3z3zI_uz9(zB3, zR3);
    if (zgaz3181) {  zbHi = INT64_C(1);  } else {  zbHi = INT64_C(0);  }
  
  }
  int64_t ztHi;
  {
    bool zgaz3180;
    zgaz3180 = sailgen_z8operatorz0zzzz7nzzJzzK3z3zI_uz9(zT3, zR3);
    if (zgaz3180) {  ztHi = INT64_C(1);  } else {  ztHi = INT64_C(0);  }
  
  }
  int64_t zcorrection_base;
  {
    sail_int zgsz3347;
    CREATE(sail_int)(&zgsz3347);
    CONVERT_OF(sail_int, mach_int)(&zgsz3347, zbHi);
    sail_int zgsz3348;
    CREATE(sail_int)(&zgsz3348);
    CONVERT_OF(sail_int, mach_int)(&zgsz3348, zaHi);
    sail_int zgsz3349;
    CREATE(sail_int)(&zgsz3349);
    sub_int(&zgsz3349, zgsz3347, zgsz3348);
    zcorrection_base = CONVERT_OF(mach_int, sail_int)(zgsz3349);
    KILL(sail_int)(&zgsz3349);
    KILL(sail_int)(&zgsz3348);
    KILL(sail_int)(&zgsz3347);
  }
  int64_t zcorrection_top;
  {
    sail_int zgsz3350;
    CREATE(sail_int)(&zgsz3350);
    CONVERT_OF(sail_int, mach_int)(&zgsz3350, ztHi);
    sail_int zgsz3351;
    CREATE(sail_int)(&zgsz3351);
    CONVERT_OF(sail_int, mach_int)(&zgsz3351, zaHi);
    sail_int zgsz3352;
    CREATE(sail_int)(&zgsz3352);
    sub_int(&zgsz3352, zgsz3350, zgsz3351);
    zcorrection_top = CONVERT_OF(mach_int, sail_int)(zgsz3352);
    KILL(sail_int)(&zgsz3352);
    KILL(sail_int)(&zgsz3351);
    KILL(sail_int)(&zgsz3350);
  }
  uint64_t za_top;
  {
    int64_t zgaz3179;
    {
      sail_int zgsz3353;
      CREATE(sail_int)(&zgsz3353);
      CONVERT_OF(sail_int, mach_int)(&zgsz3353, zE);
      sail_int zgsz3354;
      CREATE(sail_int)(&zgsz3354);
      CONVERT_OF(sail_int, mach_int)(&zgsz3354, zmantissa_width);
      sail_int zgsz3355;
      CREATE(sail_int)(&zgsz3355);
      add_int(&zgsz3355, zgsz3353, zgsz3354);
      zgaz3179 = CONVERT_OF(mach_int, sail_int)(zgsz3355);
      KILL(sail_int)(&zgsz3355);
      KILL(sail_int)(&zgsz3354);
      KILL(sail_int)(&zgsz3353);
    }
    {
      lbits zgsz3356;
      CREATE(lbits)(&zgsz3356);
      CONVERT_OF(lbits, fbits)(&zgsz3356, za, UINT64_C(32) , true);
      sail_int zgsz3357;
      CREATE(sail_int)(&zgsz3357);
      CONVERT_OF(sail_int, mach_int)(&zgsz3357, zgaz3179);
      lbits zgsz3358;
      CREATE(lbits)(&zgsz3358);
      shiftr(&zgsz3358, zgsz3356, zgsz3357);
      za_top = CONVERT_OF(fbits, lbits)(zgsz3358, true);
      KILL(lbits)(&zgsz3358);
      KILL(sail_int)(&zgsz3357);
      KILL(lbits)(&zgsz3356);
    }
  
  }
  uint64_t zbase;
  {
    lbits zgaz3178;
    CREATE(lbits)(&zgaz3178);
    {
      uint64_t zgaz3176;
      {
        lbits zgsz3359;
        CREATE(lbits)(&zgsz3359);
        CONVERT_OF(lbits, fbits)(&zgsz3359, za_top, UINT64_C(32) , true);
        sail_int zgsz3360;
        CREATE(sail_int)(&zgsz3360);
        CONVERT_OF(sail_int, mach_int)(&zgsz3360, zcorrection_base);
        lbits zgsz3361;
        CREATE(lbits)(&zgsz3361);
        add_bits_int(&zgsz3361, zgsz3359, zgsz3360);
        zgaz3176 = CONVERT_OF(fbits, lbits)(zgsz3361, true);
        KILL(lbits)(&zgsz3361);
        KILL(sail_int)(&zgsz3360);
        KILL(lbits)(&zgsz3359);
      }
      sbits zgaz3177;
      {
        uint64_t zgaz3174;
        zgaz3174 = zc.zB;
        sbits zgaz3175;
        {
          sail_int zgsz3362;
          CREATE(sail_int)(&zgsz3362);
          CONVERT_OF(sail_int, mach_int)(&zgsz3362, zE);
          lbits zgsz3363;
          CREATE(lbits)(&zgsz3363);
          sailgen_zzz3zzz3zzeros_implicit(&zgsz3363, zgsz3362);
          zgaz3175 = CONVERT_OF(sbits, lbits)(zgsz3363, true);
          KILL(lbits)(&zgsz3363);
          KILL(sail_int)(&zgsz3362);
        }
        {
          lbits zgsz3364;
          CREATE(lbits)(&zgsz3364);
          CONVERT_OF(lbits, fbits)(&zgsz3364, zgaz3174, UINT64_C(8) , true);
          lbits zgsz3365;
          CREATE(lbits)(&zgsz3365);
          CONVERT_OF(lbits, sbits)(&zgsz3365, zgaz3175, true);
          lbits zgsz3366;
          CREATE(lbits)(&zgsz3366);
          append(&zgsz3366, zgsz3364, zgsz3365);
          zgaz3177 = CONVERT_OF(sbits, lbits)(zgsz3366, true);
          KILL(lbits)(&zgsz3366);
          KILL(lbits)(&zgsz3365);
          KILL(lbits)(&zgsz3364);
        }
      
      
      }
      {
        lbits zgsz3368;
        CREATE(lbits)(&zgsz3368);
        CONVERT_OF(lbits, sbits)(&zgsz3368, zgaz3177, true);
        lbits zgsz3367;
        CREATE(lbits)(&zgsz3367);
        CONVERT_OF(lbits, fbits)(&zgsz3367, zgaz3176, UINT64_C(32) , true);
        append(&zgaz3178, zgsz3367, zgsz3368);
        KILL(lbits)(&zgsz3368);
        KILL(lbits)(&zgsz3367);
      }
    
    
    }
    {
      sail_int zgsz3369;
      CREATE(sail_int)(&zgsz3369);
      CONVERT_OF(sail_int, mach_int)(&zgsz3369, zcap_len_width);
      lbits zgsz3370;
      CREATE(lbits)(&zgsz3370);
      sail_truncate(&zgsz3370, zgaz3178, zgsz3369);
      zbase = CONVERT_OF(fbits, lbits)(zgsz3370, true);
      KILL(lbits)(&zgsz3370);
      KILL(sail_int)(&zgsz3369);
    }
    KILL(lbits)(&zgaz3178);
  }
  uint64_t ztop;
  {
    lbits zgaz3173;
    CREATE(lbits)(&zgaz3173);
    {
      uint64_t zgaz3171;
      {
        lbits zgsz3371;
        CREATE(lbits)(&zgsz3371);
        CONVERT_OF(lbits, fbits)(&zgsz3371, za_top, UINT64_C(32) , true);
        sail_int zgsz3372;
        CREATE(sail_int)(&zgsz3372);
        CONVERT_OF(sail_int, mach_int)(&zgsz3372, zcorrection_top);
        lbits zgsz3373;
        CREATE(lbits)(&zgsz3373);
        add_bits_int(&zgsz3373, zgsz3371, zgsz3372);
        zgaz3171 = CONVERT_OF(fbits, lbits)(zgsz3373, true);
        KILL(lbits)(&zgsz3373);
        KILL(sail_int)(&zgsz3372);
        KILL(lbits)(&zgsz3371);
      }
      sbits zgaz3172;
      {
        uint64_t zgaz3169;
        zgaz3169 = zc.zT;
        sbits zgaz3170;
        {
          sail_int zgsz3374;
          CREATE(sail_int)(&zgsz3374);
          CONVERT_OF(sail_int, mach_int)(&zgsz3374, zE);
          lbits zgsz3375;
          CREATE(lbits)(&zgsz3375);
          sailgen_zzz3zzz3zzeros_implicit(&zgsz3375, zgsz3374);
          zgaz3170 = CONVERT_OF(sbits, lbits)(zgsz3375, true);
          KILL(lbits)(&zgsz3375);
          KILL(sail_int)(&zgsz3374);
        }
        {
          lbits zgsz3376;
          CREATE(lbits)(&zgsz3376);
          CONVERT_OF(lbits, fbits)(&zgsz3376, zgaz3169, UINT64_C(8) , true);
          lbits zgsz3377;
          CREATE(lbits)(&zgsz3377);
          CONVERT_OF(lbits, sbits)(&zgsz3377, zgaz3170, true);
          lbits zgsz3378;
          CREATE(lbits)(&zgsz3378);
          append(&zgsz3378, zgsz3376, zgsz3377);
          zgaz3172 = CONVERT_OF(sbits, lbits)(zgsz3378, true);
          KILL(lbits)(&zgsz3378);
          KILL(lbits)(&zgsz3377);
          KILL(lbits)(&zgsz3376);
        }
      
      
      }
      {
        lbits zgsz3380;
        CREATE(lbits)(&zgsz3380);
        CONVERT_OF(lbits, sbits)(&zgsz3380, zgaz3172, true);
        lbits zgsz3379;
        CREATE(lbits)(&zgsz3379);
        CONVERT_OF(lbits, fbits)(&zgsz3379, zgaz3171, UINT64_C(32) , true);
        append(&zgaz3173, zgsz3379, zgsz3380);
        KILL(lbits)(&zgsz3380);
        KILL(lbits)(&zgsz3379);
      }
    
    
    }
    {
      sail_int zgsz3381;
      CREATE(sail_int)(&zgsz3381);
      CONVERT_OF(sail_int, mach_int)(&zgsz3381, zcap_len_width);
      lbits zgsz3382;
      CREATE(lbits)(&zgsz3382);
      sail_truncate(&zgsz3382, zgaz3173, zgsz3381);
      ztop = CONVERT_OF(fbits, lbits)(zgsz3382, true);
      KILL(lbits)(&zgsz3382);
      KILL(sail_int)(&zgsz3381);
    }
    KILL(lbits)(&zgaz3173);
  }
  uint64_t zbase2;
  {
    uint64_t zgaz3168;
    {
      fbits zgaz3167;
      {
        int64_t zgaz3166;
        {
          sail_int zgsz3383;
          CREATE(sail_int)(&zgsz3383);
          CONVERT_OF(sail_int, mach_int)(&zgsz3383, zcap_addr_width);
          sail_int zgsz3384;
          CREATE(sail_int)(&zgsz3384);
          CONVERT_OF(sail_int, mach_int)(&zgsz3384, INT64_C(1));
          sail_int zgsz3385;
          CREATE(sail_int)(&zgsz3385);
          sub_int(&zgsz3385, zgsz3383, zgsz3384);
          zgaz3166 = CONVERT_OF(mach_int, sail_int)(zgsz3385);
          KILL(sail_int)(&zgsz3385);
          KILL(sail_int)(&zgsz3384);
          KILL(sail_int)(&zgsz3383);
        }
        {
          sail_int zgsz3387;
          CREATE(sail_int)(&zgsz3387);
          CONVERT_OF(sail_int, mach_int)(&zgsz3387, zgaz3166);
          lbits zgsz3386;
          CREATE(lbits)(&zgsz3386);
          CONVERT_OF(lbits, fbits)(&zgsz3386, zbase, UINT64_C(33) , true);
          zgaz3167 = bitvector_access(zgsz3386, zgsz3387);
          KILL(sail_int)(&zgsz3387);
          KILL(lbits)(&zgsz3386);
        }
      
      }
      uint64_t zgsz3388;
      zgsz3388 = UINT64_C(0b0);
      zgsz3388 = update_fbits(zgsz3388, INT64_C(0), zgaz3167);
      zgaz3168 = zgsz3388;
    
    }
    {
      lbits zgsz3389;
      CREATE(lbits)(&zgsz3389);
      CONVERT_OF(lbits, fbits)(&zgsz3389, UINT64_C(0b0), UINT64_C(1) , true);
      lbits zgsz3390;
      CREATE(lbits)(&zgsz3390);
      CONVERT_OF(lbits, fbits)(&zgsz3390, zgaz3168, UINT64_C(1) , true);
      lbits zgsz3391;
      CREATE(lbits)(&zgsz3391);
      append(&zgsz3391, zgsz3389, zgsz3390);
      zbase2 = CONVERT_OF(fbits, lbits)(zgsz3391, true);
      KILL(lbits)(&zgsz3391);
      KILL(lbits)(&zgsz3390);
      KILL(lbits)(&zgsz3389);
    }
  
  }
  uint64_t ztop2;
  {
    int64_t zgaz3165;
    {
      sail_int zgsz3392;
      CREATE(sail_int)(&zgsz3392);
      CONVERT_OF(sail_int, mach_int)(&zgsz3392, zcap_addr_width);
      sail_int zgsz3393;
      CREATE(sail_int)(&zgsz3393);
      CONVERT_OF(sail_int, mach_int)(&zgsz3393, INT64_C(1));
      sail_int zgsz3394;
      CREATE(sail_int)(&zgsz3394);
      sub_int(&zgsz3394, zgsz3392, zgsz3393);
      zgaz3165 = CONVERT_OF(mach_int, sail_int)(zgsz3394);
      KILL(sail_int)(&zgsz3394);
      KILL(sail_int)(&zgsz3393);
      KILL(sail_int)(&zgsz3392);
    }
    {
      lbits zgsz3395;
      CREATE(lbits)(&zgsz3395);
      CONVERT_OF(lbits, fbits)(&zgsz3395, ztop, UINT64_C(33) , true);
      sail_int zgsz3396;
      CREATE(sail_int)(&zgsz3396);
      CONVERT_OF(sail_int, mach_int)(&zgsz3396, zcap_addr_width);
      sail_int zgsz3397;
      CREATE(sail_int)(&zgsz3397);
      CONVERT_OF(sail_int, mach_int)(&zgsz3397, zgaz3165);
      lbits zgsz3398;
      CREATE(lbits)(&zgsz3398);
      vector_subrange_lbits(&zgsz3398, zgsz3395, zgsz3396, zgsz3397);
      ztop2 = CONVERT_OF(fbits, lbits)(zgsz3398, true);
      KILL(lbits)(&zgsz3398);
      KILL(sail_int)(&zgsz3397);
      KILL(sail_int)(&zgsz3396);
      KILL(lbits)(&zgsz3395);
    }
  
  }
  {
    bool zgaz3160;
    {
      bool zgaz3159;
      {
        int64_t zgaz3156;
        {
          sail_int zgsz3399;
          CREATE(sail_int)(&zgsz3399);
          CONVERT_OF(sail_int, mach_int)(&zgsz3399, zmaxE);
          sail_int zgsz3400;
          CREATE(sail_int)(&zgsz3400);
          CONVERT_OF(sail_int, mach_int)(&zgsz3400, INT64_C(1));
          sail_int zgsz3401;
          CREATE(sail_int)(&zgsz3401);
          sub_int(&zgsz3401, zgsz3399, zgsz3400);
          zgaz3156 = CONVERT_OF(mach_int, sail_int)(zgsz3401);
          KILL(sail_int)(&zgsz3401);
          KILL(sail_int)(&zgsz3400);
          KILL(sail_int)(&zgsz3399);
        }
        {
          sail_int zgsz3403;
          CREATE(sail_int)(&zgsz3403);
          CONVERT_OF(sail_int, mach_int)(&zgsz3403, zgaz3156);
          sail_int zgsz3402;
          CREATE(sail_int)(&zgsz3402);
          CONVERT_OF(sail_int, mach_int)(&zgsz3402, zE);
          zgaz3159 = lt(zgsz3402, zgsz3403);
          KILL(sail_int)(&zgsz3403);
          KILL(sail_int)(&zgsz3402);
        }
      
      }
      bool zgsz3407;
      if (zgaz3159) {
      int64_t zgaz3158;
      {
        uint64_t zgaz3157;
        {
          lbits zgsz3404;
          CREATE(lbits)(&zgsz3404);
          CONVERT_OF(lbits, fbits)(&zgsz3404, ztop2, UINT64_C(2) , true);
          lbits zgsz3405;
          CREATE(lbits)(&zgsz3405);
          CONVERT_OF(lbits, fbits)(&zgsz3405, zbase2, UINT64_C(2) , true);
          lbits zgsz3406;
          CREATE(lbits)(&zgsz3406);
          sub_bits(&zgsz3406, zgsz3404, zgsz3405);
          zgaz3157 = CONVERT_OF(fbits, lbits)(zgsz3406, true);
          KILL(lbits)(&zgsz3406);
          KILL(lbits)(&zgsz3405);
          KILL(lbits)(&zgsz3404);
        }
        zgaz3158 = ((mach_int) zgaz3157);
      
      }
      {
        sail_int zgsz3409;
        CREATE(sail_int)(&zgsz3409);
        CONVERT_OF(sail_int, mach_int)(&zgsz3409, INT64_C(1));
        sail_int zgsz3408;
        CREATE(sail_int)(&zgsz3408);
        CONVERT_OF(sail_int, mach_int)(&zgsz3408, zgaz3158);
        zgsz3407 = gt(zgsz3408, zgsz3409);
        KILL(sail_int)(&zgsz3409);
        KILL(sail_int)(&zgsz3408);
      }
    
      } else {  zgsz3407 = false;  }
      zgaz3160 = zgsz3407;
    
    }
    unit zgsz3410;
    if (zgaz3160) {
    fbits zgaz3162;
    {
      fbits zgaz3161;
      {
        sail_int zgsz3412;
        CREATE(sail_int)(&zgsz3412);
        CONVERT_OF(sail_int, mach_int)(&zgsz3412, zcap_addr_width);
        lbits zgsz3411;
        CREATE(lbits)(&zgsz3411);
        CONVERT_OF(lbits, fbits)(&zgsz3411, ztop, UINT64_C(33) , true);
        zgaz3161 = bitvector_access(zgsz3411, zgsz3412);
        KILL(sail_int)(&zgsz3412);
        KILL(lbits)(&zgsz3411);
      }
      zgaz3162 = sailgen_not_bit(zgaz3161);
    
    }
    {
      lbits zgsz3413;
      CREATE(lbits)(&zgsz3413);
      CONVERT_OF(lbits, fbits)(&zgsz3413, ztop, UINT64_C(33) , true);
      sail_int zgsz3414;
      CREATE(sail_int)(&zgsz3414);
      CONVERT_OF(sail_int, mach_int)(&zgsz3414, zcap_addr_width);
      lbits zgsz3415;
      CREATE(lbits)(&zgsz3415);
      update_lbits(&zgsz3415, zgsz3413, zgsz3414, zgaz3162);
      ztop = CONVERT_OF(fbits, lbits)(zgsz3415, true);
      KILL(lbits)(&zgsz3415);
      KILL(sail_int)(&zgsz3414);
      KILL(lbits)(&zgsz3413);
    }
    zgsz3410 = UNIT;
  
    } else {  zgsz3410 = UNIT;  }
  
  }
  uint64_t zgaz3164;
  {
    int64_t zgaz3163;
    {
      sail_int zgsz3416;
      CREATE(sail_int)(&zgsz3416);
      CONVERT_OF(sail_int, mach_int)(&zgsz3416, zcap_addr_width);
      sail_int zgsz3417;
      CREATE(sail_int)(&zgsz3417);
      CONVERT_OF(sail_int, mach_int)(&zgsz3417, INT64_C(1));
      sail_int zgsz3418;
      CREATE(sail_int)(&zgsz3418);
      sub_int(&zgsz3418, zgsz3416, zgsz3417);
      zgaz3163 = CONVERT_OF(mach_int, sail_int)(zgsz3418);
      KILL(sail_int)(&zgsz3418);
      KILL(sail_int)(&zgsz3417);
      KILL(sail_int)(&zgsz3416);
    }
    {
      lbits zgsz3419;
      CREATE(lbits)(&zgsz3419);
      CONVERT_OF(lbits, fbits)(&zgsz3419, zbase, UINT64_C(33) , true);
      sail_int zgsz3420;
      CREATE(sail_int)(&zgsz3420);
      CONVERT_OF(sail_int, mach_int)(&zgsz3420, zgaz3163);
      sail_int zgsz3421;
      CREATE(sail_int)(&zgsz3421);
      CONVERT_OF(sail_int, mach_int)(&zgsz3421, INT64_C(0));
      lbits zgsz3422;
      CREATE(lbits)(&zgsz3422);
      vector_subrange_lbits(&zgsz3422, zgsz3419, zgsz3420, zgsz3421);
      zgaz3164 = CONVERT_OF(fbits, lbits)(zgsz3422, true);
      KILL(lbits)(&zgsz3422);
      KILL(sail_int)(&zgsz3421);
      KILL(sail_int)(&zgsz3420);
      KILL(lbits)(&zgsz3419);
    }
  
  }
  struct ztuple_z8z5bv32zCz0z5bv33z9 zgsz3423;
  zgsz3423.ztup0 = zgaz3164;
  zgsz3423.ztup1 = ztop;
  zcbz339 = zgsz3423;


















end_function_152: ;
  return zcbz339;
end_block_exception_153: ;
  struct ztuple_z8z5bv32zCz0z5bv33z9 zcbz356 = { .ztup1 = UINT64_C(0xdeadc0de), .ztup0 = UINT64_C(0xdeadc0de) };
  return zcbz356;
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

static struct ztuple_z8z5i64zCz0z5i64z9 sailgen_getCapBounds(struct zCapability);


static struct ztuple_z8z5i64zCz0z5i64z9 sailgen_getCapBounds(struct zCapability zcap)
{
  __label__ case_156, finish_match_155, end_function_157, end_block_exception_158;

  struct ztuple_z8z5i64zCz0z5i64z9 zcbz340;
  struct ztuple_z8z5bv32zCz0z5bv33z9 zgaz3190;
  zgaz3190 = sailgen_getCapBoundsBits(zcap);
  struct ztuple_z8z5i64zCz0z5i64z9 zgsz3424;
  {
    uint64_t zbase;
    zbase = zgaz3190.ztup0;
    uint64_t ztop;
    ztop = zgaz3190.ztup1;
    int64_t zgaz3191;
    zgaz3191 = ((mach_int) zbase);
    int64_t zgaz3192;
    zgaz3192 = ((mach_int) ztop);
    struct ztuple_z8z5i64zCz0z5i64z9 zgsz3425;
    zgsz3425.ztup0 = zgaz3191;
    zgsz3425.ztup1 = zgaz3192;
    zgsz3424 = zgsz3425;
  
  
  
  
  
    goto finish_match_155;
  }
case_156: ;
  sail_match_failure("getCapBounds");
finish_match_155: ;
  zcbz340 = zgsz3424;


end_function_157: ;
  return zcbz340;
end_block_exception_158: ;
  struct ztuple_z8z5i64zCz0z5i64z9 zcbz357 = { .ztup1 = INT64_C(0xdeadc0de), .ztup0 = INT64_C(0xdeadc0de) };
  return zcbz357;
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

static struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 sailgen_setCapBounds(struct zCapability zcap, uint64_t zbase, uint64_t ztop)
{
  __label__ end_function_160, end_block_exception_161;

  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz341;
  uint64_t zext_base;
  {
    lbits zgsz3427;
    CREATE(lbits)(&zgsz3427);
    CONVERT_OF(lbits, fbits)(&zgsz3427, UINT64_C(0b0), UINT64_C(1) , true);
    lbits zgsz3428;
    CREATE(lbits)(&zgsz3428);
    CONVERT_OF(lbits, fbits)(&zgsz3428, zbase, UINT64_C(32) , true);
    lbits zgsz3429;
    CREATE(lbits)(&zgsz3429);
    append(&zgsz3429, zgsz3427, zgsz3428);
    zext_base = CONVERT_OF(fbits, lbits)(zgsz3429, true);
    KILL(lbits)(&zgsz3429);
    KILL(lbits)(&zgsz3428);
    KILL(lbits)(&zgsz3427);
  }
  uint64_t zlength;
  {
    lbits zgsz3430;
    CREATE(lbits)(&zgsz3430);
    CONVERT_OF(lbits, fbits)(&zgsz3430, ztop, UINT64_C(33) , true);
    lbits zgsz3431;
    CREATE(lbits)(&zgsz3431);
    CONVERT_OF(lbits, fbits)(&zgsz3431, zext_base, UINT64_C(33) , true);
    lbits zgsz3432;
    CREATE(lbits)(&zgsz3432);
    sub_bits(&zgsz3432, zgsz3430, zgsz3431);
    zlength = CONVERT_OF(fbits, lbits)(zgsz3432, true);
    KILL(lbits)(&zgsz3432);
    KILL(lbits)(&zgsz3431);
    KILL(lbits)(&zgsz3430);
  }
  int64_t ze;
  {
    int64_t zgaz3226;
    {
      uint64_t zgaz3225;
      {
        int64_t zgaz3224;
        {
          sail_int zgsz3433;
          CREATE(sail_int)(&zgsz3433);
          CONVERT_OF(sail_int, mach_int)(&zgsz3433, zmantissa_width);
          sail_int zgsz3434;
          CREATE(sail_int)(&zgsz3434);
          CONVERT_OF(sail_int, mach_int)(&zgsz3434, INT64_C(1));
          sail_int zgsz3435;
          CREATE(sail_int)(&zgsz3435);
          sub_int(&zgsz3435, zgsz3433, zgsz3434);
          zgaz3224 = CONVERT_OF(mach_int, sail_int)(zgsz3435);
          KILL(sail_int)(&zgsz3435);
          KILL(sail_int)(&zgsz3434);
          KILL(sail_int)(&zgsz3433);
        }
        {
          lbits zgsz3436;
          CREATE(lbits)(&zgsz3436);
          CONVERT_OF(lbits, fbits)(&zgsz3436, zlength, UINT64_C(33) , true);
          sail_int zgsz3437;
          CREATE(sail_int)(&zgsz3437);
          CONVERT_OF(sail_int, mach_int)(&zgsz3437, zcap_addr_width);
          sail_int zgsz3438;
          CREATE(sail_int)(&zgsz3438);
          CONVERT_OF(sail_int, mach_int)(&zgsz3438, zgaz3224);
          lbits zgsz3439;
          CREATE(lbits)(&zgsz3439);
          vector_subrange_lbits(&zgsz3439, zgsz3436, zgsz3437, zgsz3438);
          zgaz3225 = CONVERT_OF(fbits, lbits)(zgsz3439, true);
          KILL(lbits)(&zgsz3439);
          KILL(sail_int)(&zgsz3438);
          KILL(sail_int)(&zgsz3437);
          KILL(lbits)(&zgsz3436);
        }
      
      }
      {
        lbits zgsz3440;
        CREATE(lbits)(&zgsz3440);
        CONVERT_OF(lbits, fbits)(&zgsz3440, zgaz3225, UINT64_C(26) , true);
        sail_int zgsz3441;
        CREATE(sail_int)(&zgsz3441);
        count_leading_zeros(&zgsz3441, zgsz3440);
        zgaz3226 = CONVERT_OF(mach_int, sail_int)(zgsz3441);
        KILL(sail_int)(&zgsz3441);
        KILL(lbits)(&zgsz3440);
      }
    
    }
    {
      sail_int zgsz3442;
      CREATE(sail_int)(&zgsz3442);
      CONVERT_OF(sail_int, mach_int)(&zgsz3442, zmaxE);
      sail_int zgsz3443;
      CREATE(sail_int)(&zgsz3443);
      CONVERT_OF(sail_int, mach_int)(&zgsz3443, zgaz3226);
      sail_int zgsz3444;
      CREATE(sail_int)(&zgsz3444);
      sub_int(&zgsz3444, zgsz3442, zgsz3443);
      ze = CONVERT_OF(mach_int, sail_int)(zgsz3444);
      KILL(sail_int)(&zgsz3444);
      KILL(sail_int)(&zgsz3443);
      KILL(sail_int)(&zgsz3442);
    }
  
  }
  bool zie;
  {
    bool zgaz3223;
    {
      sail_int zgsz3445;
      CREATE(sail_int)(&zgsz3445);
      CONVERT_OF(sail_int, mach_int)(&zgsz3445, ze);
      zgaz3223 = sailgen_zzz3zzzz7mzzJzzK0z3neq_int(zgsz3445, INT64_C(0));
      KILL(sail_int)(&zgsz3445);
    }
    bool zgsz3451;
    if (zgaz3223) {  zgsz3451 = true;  } else {
    fbits zgaz3222;
    {
      int64_t zgaz3221;
      {
        sail_int zgsz3446;
        CREATE(sail_int)(&zgsz3446);
        CONVERT_OF(sail_int, mach_int)(&zgsz3446, zmantissa_width);
        sail_int zgsz3447;
        CREATE(sail_int)(&zgsz3447);
        CONVERT_OF(sail_int, mach_int)(&zgsz3447, INT64_C(2));
        sail_int zgsz3448;
        CREATE(sail_int)(&zgsz3448);
        sub_int(&zgsz3448, zgsz3446, zgsz3447);
        zgaz3221 = CONVERT_OF(mach_int, sail_int)(zgsz3448);
        KILL(sail_int)(&zgsz3448);
        KILL(sail_int)(&zgsz3447);
        KILL(sail_int)(&zgsz3446);
      }
      {
        sail_int zgsz3450;
        CREATE(sail_int)(&zgsz3450);
        CONVERT_OF(sail_int, mach_int)(&zgsz3450, zgaz3221);
        lbits zgsz3449;
        CREATE(lbits)(&zgsz3449);
        CONVERT_OF(lbits, fbits)(&zgsz3449, zlength, UINT64_C(33) , true);
        zgaz3222 = bitvector_access(zgsz3449, zgsz3450);
        KILL(sail_int)(&zgsz3450);
        KILL(lbits)(&zgsz3449);
      }
    
    }
    zgsz3451 = eq_bit(zgaz3222, UINT64_C(1));
  
    }
    zie = zgsz3451;
  
  }
  uint64_t zBbits;
  {
    lbits zgsz3452;
    CREATE(lbits)(&zgsz3452);
    CONVERT_OF(lbits, fbits)(&zgsz3452, zbase, UINT64_C(32) , true);
    sail_int zgsz3453;
    CREATE(sail_int)(&zgsz3453);
    CONVERT_OF(sail_int, mach_int)(&zgsz3453, zmantissa_width);
    lbits zgsz3454;
    CREATE(lbits)(&zgsz3454);
    sail_truncate(&zgsz3454, zgsz3452, zgsz3453);
    zBbits = CONVERT_OF(fbits, lbits)(zgsz3454, true);
    KILL(lbits)(&zgsz3454);
    KILL(sail_int)(&zgsz3453);
    KILL(lbits)(&zgsz3452);
  }
  uint64_t zTbits;
  {
    lbits zgsz3455;
    CREATE(lbits)(&zgsz3455);
    CONVERT_OF(lbits, fbits)(&zgsz3455, ztop, UINT64_C(33) , true);
    sail_int zgsz3456;
    CREATE(sail_int)(&zgsz3456);
    CONVERT_OF(sail_int, mach_int)(&zgsz3456, zmantissa_width);
    lbits zgsz3457;
    CREATE(lbits)(&zgsz3457);
    sail_truncate(&zgsz3457, zgsz3455, zgsz3456);
    zTbits = CONVERT_OF(fbits, lbits)(zgsz3457, true);
    KILL(lbits)(&zgsz3457);
    KILL(sail_int)(&zgsz3456);
    KILL(lbits)(&zgsz3455);
  }
  bool zlostSignificantTop;
  zlostSignificantTop = false;
  bool zlostSignificantBase;
  zlostSignificantBase = false;
  bool zincE;
  zincE = false;
  {
    unit zgsz3458;
    if (zie) {
    uint64_t zB_ie;
    {
      uint64_t zgaz3216;
      {
        int64_t zgaz3215;
        {
          sail_int zgsz3459;
          CREATE(sail_int)(&zgsz3459);
          CONVERT_OF(sail_int, mach_int)(&zgsz3459, ze);
          sail_int zgsz3460;
          CREATE(sail_int)(&zgsz3460);
          CONVERT_OF(sail_int, mach_int)(&zgsz3460, INT64_C(3));
          sail_int zgsz3461;
          CREATE(sail_int)(&zgsz3461);
          add_int(&zgsz3461, zgsz3459, zgsz3460);
          zgaz3215 = CONVERT_OF(mach_int, sail_int)(zgsz3461);
          KILL(sail_int)(&zgsz3461);
          KILL(sail_int)(&zgsz3460);
          KILL(sail_int)(&zgsz3459);
        }
        {
          lbits zgsz3462;
          CREATE(lbits)(&zgsz3462);
          CONVERT_OF(lbits, fbits)(&zgsz3462, zbase, UINT64_C(32) , true);
          sail_int zgsz3463;
          CREATE(sail_int)(&zgsz3463);
          CONVERT_OF(sail_int, mach_int)(&zgsz3463, zgaz3215);
          lbits zgsz3464;
          CREATE(lbits)(&zgsz3464);
          shiftr(&zgsz3464, zgsz3462, zgsz3463);
          zgaz3216 = CONVERT_OF(fbits, lbits)(zgsz3464, true);
          KILL(lbits)(&zgsz3464);
          KILL(sail_int)(&zgsz3463);
          KILL(lbits)(&zgsz3462);
        }
      
      }
      int64_t zgaz3217;
      {
        sail_int zgsz3465;
        CREATE(sail_int)(&zgsz3465);
        CONVERT_OF(sail_int, mach_int)(&zgsz3465, zmantissa_width);
        sail_int zgsz3466;
        CREATE(sail_int)(&zgsz3466);
        CONVERT_OF(sail_int, mach_int)(&zgsz3466, INT64_C(3));
        sail_int zgsz3467;
        CREATE(sail_int)(&zgsz3467);
        sub_int(&zgsz3467, zgsz3465, zgsz3466);
        zgaz3217 = CONVERT_OF(mach_int, sail_int)(zgsz3467);
        KILL(sail_int)(&zgsz3467);
        KILL(sail_int)(&zgsz3466);
        KILL(sail_int)(&zgsz3465);
      }
      {
        lbits zgsz3468;
        CREATE(lbits)(&zgsz3468);
        CONVERT_OF(lbits, fbits)(&zgsz3468, zgaz3216, UINT64_C(32) , true);
        sail_int zgsz3469;
        CREATE(sail_int)(&zgsz3469);
        CONVERT_OF(sail_int, mach_int)(&zgsz3469, zgaz3217);
        lbits zgsz3470;
        CREATE(lbits)(&zgsz3470);
        sail_truncate(&zgsz3470, zgsz3468, zgsz3469);
        zB_ie = CONVERT_OF(fbits, lbits)(zgsz3470, true);
        KILL(lbits)(&zgsz3470);
        KILL(sail_int)(&zgsz3469);
        KILL(lbits)(&zgsz3468);
      }
    
    
    }
    uint64_t zT_ie;
    {
      uint64_t zgaz3213;
      {
        int64_t zgaz3212;
        {
          sail_int zgsz3471;
          CREATE(sail_int)(&zgsz3471);
          CONVERT_OF(sail_int, mach_int)(&zgsz3471, ze);
          sail_int zgsz3472;
          CREATE(sail_int)(&zgsz3472);
          CONVERT_OF(sail_int, mach_int)(&zgsz3472, INT64_C(3));
          sail_int zgsz3473;
          CREATE(sail_int)(&zgsz3473);
          add_int(&zgsz3473, zgsz3471, zgsz3472);
          zgaz3212 = CONVERT_OF(mach_int, sail_int)(zgsz3473);
          KILL(sail_int)(&zgsz3473);
          KILL(sail_int)(&zgsz3472);
          KILL(sail_int)(&zgsz3471);
        }
        {
          lbits zgsz3474;
          CREATE(lbits)(&zgsz3474);
          CONVERT_OF(lbits, fbits)(&zgsz3474, ztop, UINT64_C(33) , true);
          sail_int zgsz3475;
          CREATE(sail_int)(&zgsz3475);
          CONVERT_OF(sail_int, mach_int)(&zgsz3475, zgaz3212);
          lbits zgsz3476;
          CREATE(lbits)(&zgsz3476);
          shiftr(&zgsz3476, zgsz3474, zgsz3475);
          zgaz3213 = CONVERT_OF(fbits, lbits)(zgsz3476, true);
          KILL(lbits)(&zgsz3476);
          KILL(sail_int)(&zgsz3475);
          KILL(lbits)(&zgsz3474);
        }
      
      }
      int64_t zgaz3214;
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
        zgaz3214 = CONVERT_OF(mach_int, sail_int)(zgsz3479);
        KILL(sail_int)(&zgsz3479);
        KILL(sail_int)(&zgsz3478);
        KILL(sail_int)(&zgsz3477);
      }
      {
        lbits zgsz3480;
        CREATE(lbits)(&zgsz3480);
        CONVERT_OF(lbits, fbits)(&zgsz3480, zgaz3213, UINT64_C(33) , true);
        sail_int zgsz3481;
        CREATE(sail_int)(&zgsz3481);
        CONVERT_OF(sail_int, mach_int)(&zgsz3481, zgaz3214);
        lbits zgsz3482;
        CREATE(lbits)(&zgsz3482);
        sail_truncate(&zgsz3482, zgsz3480, zgsz3481);
        zT_ie = CONVERT_OF(fbits, lbits)(zgsz3482, true);
        KILL(lbits)(&zgsz3482);
        KILL(sail_int)(&zgsz3481);
        KILL(lbits)(&zgsz3480);
      }
    
    
    }
    uint64_t zmaskLo;
    {
      int64_t zgaz3210;
      {
        sail_int zgsz3483;
        CREATE(sail_int)(&zgsz3483);
        CONVERT_OF(sail_int, mach_int)(&zgsz3483, INT64_C(32));
        sail_int zgsz3484;
        CREATE(sail_int)(&zgsz3484);
        CONVERT_OF(sail_int, mach_int)(&zgsz3484, INT64_C(1));
        sail_int zgsz3485;
        CREATE(sail_int)(&zgsz3485);
        add_int(&zgsz3485, zgsz3483, zgsz3484);
        zgaz3210 = CONVERT_OF(mach_int, sail_int)(zgsz3485);
        KILL(sail_int)(&zgsz3485);
        KILL(sail_int)(&zgsz3484);
        KILL(sail_int)(&zgsz3483);
      }
      sbits zgaz3211;
      {
        int64_t zgaz3209;
        {
          sail_int zgsz3486;
          CREATE(sail_int)(&zgsz3486);
          CONVERT_OF(sail_int, mach_int)(&zgsz3486, ze);
          sail_int zgsz3487;
          CREATE(sail_int)(&zgsz3487);
          CONVERT_OF(sail_int, mach_int)(&zgsz3487, INT64_C(3));
          sail_int zgsz3488;
          CREATE(sail_int)(&zgsz3488);
          add_int(&zgsz3488, zgsz3486, zgsz3487);
          zgaz3209 = CONVERT_OF(mach_int, sail_int)(zgsz3488);
          KILL(sail_int)(&zgsz3488);
          KILL(sail_int)(&zgsz3487);
          KILL(sail_int)(&zgsz3486);
        }
        {
          sail_int zgsz3489;
          CREATE(sail_int)(&zgsz3489);
          CONVERT_OF(sail_int, mach_int)(&zgsz3489, zgaz3209);
          lbits zgsz3490;
          CREATE(lbits)(&zgsz3490);
          sailgen_zzz3zzz3ones(&zgsz3490, zgsz3489);
          zgaz3211 = CONVERT_OF(sbits, lbits)(zgsz3490, true);
          KILL(lbits)(&zgsz3490);
          KILL(sail_int)(&zgsz3489);
        }
      
      }
      {
        sail_int zgsz3491;
        CREATE(sail_int)(&zgsz3491);
        CONVERT_OF(sail_int, mach_int)(&zgsz3491, zgaz3210);
        lbits zgsz3492;
        CREATE(lbits)(&zgsz3492);
        CONVERT_OF(lbits, sbits)(&zgsz3492, zgaz3211, true);
        lbits zgsz3493;
        CREATE(lbits)(&zgsz3493);
        sailgen_zzz3zzz3EXTZ(&zgsz3493, zgsz3491, zgsz3492);
        zmaskLo = CONVERT_OF(fbits, lbits)(zgsz3493, true);
        KILL(lbits)(&zgsz3493);
        KILL(lbits)(&zgsz3492);
        KILL(sail_int)(&zgsz3491);
      }
    
    
    }
    {
      int64_t zgaz3194;
      {
        uint64_t zgaz3193;
        {
          lbits zgsz3494;
          CREATE(lbits)(&zgsz3494);
          CONVERT_OF(lbits, fbits)(&zgsz3494, zext_base, UINT64_C(33) , true);
          lbits zgsz3495;
          CREATE(lbits)(&zgsz3495);
          CONVERT_OF(lbits, fbits)(&zgsz3495, zmaskLo, UINT64_C(33) , true);
          lbits zgsz3496;
          CREATE(lbits)(&zgsz3496);
          and_bits(&zgsz3496, zgsz3494, zgsz3495);
          zgaz3193 = CONVERT_OF(fbits, lbits)(zgsz3496, true);
          KILL(lbits)(&zgsz3496);
          KILL(lbits)(&zgsz3495);
          KILL(lbits)(&zgsz3494);
        }
        zgaz3194 = ((mach_int) zgaz3193);
      
      }
      {
        sail_int zgsz3497;
        CREATE(sail_int)(&zgsz3497);
        CONVERT_OF(sail_int, mach_int)(&zgsz3497, zgaz3194);
        zlostSignificantBase = sailgen_zzz3zzzz7mzzJzzK0z3neq_int(zgsz3497, INT64_C(0));
        KILL(sail_int)(&zgsz3497);
      }
      unit zgsz3507;
      zgsz3507 = UNIT;
    
    }
    {
      int64_t zgaz3196;
      {
        uint64_t zgaz3195;
        {
          lbits zgsz3498;
          CREATE(lbits)(&zgsz3498);
          CONVERT_OF(lbits, fbits)(&zgsz3498, ztop, UINT64_C(33) , true);
          lbits zgsz3499;
          CREATE(lbits)(&zgsz3499);
          CONVERT_OF(lbits, fbits)(&zgsz3499, zmaskLo, UINT64_C(33) , true);
          lbits zgsz3500;
          CREATE(lbits)(&zgsz3500);
          and_bits(&zgsz3500, zgsz3498, zgsz3499);
          zgaz3195 = CONVERT_OF(fbits, lbits)(zgsz3500, true);
          KILL(lbits)(&zgsz3500);
          KILL(lbits)(&zgsz3499);
          KILL(lbits)(&zgsz3498);
        }
        zgaz3196 = ((mach_int) zgaz3195);
      
      }
      {
        sail_int zgsz3501;
        CREATE(sail_int)(&zgsz3501);
        CONVERT_OF(sail_int, mach_int)(&zgsz3501, zgaz3196);
        zlostSignificantTop = sailgen_zzz3zzzz7mzzJzzK0z3neq_int(zgsz3501, INT64_C(0));
        KILL(sail_int)(&zgsz3501);
      }
      unit zgsz3506;
      zgsz3506 = UNIT;
    
    }
    {
      unit zgsz3502;
      if (zlostSignificantTop) {
      {
        lbits zgsz3503;
        CREATE(lbits)(&zgsz3503);
        CONVERT_OF(lbits, fbits)(&zgsz3503, zT_ie, UINT64_C(5) , true);
        sail_int zgsz3504;
        CREATE(sail_int)(&zgsz3504);
        CONVERT_OF(sail_int, mach_int)(&zgsz3504, INT64_C(1));
        lbits zgsz3505;
        CREATE(lbits)(&zgsz3505);
        add_bits_int(&zgsz3505, zgsz3503, zgsz3504);
        zT_ie = CONVERT_OF(fbits, lbits)(zgsz3505, true);
        KILL(lbits)(&zgsz3505);
        KILL(sail_int)(&zgsz3504);
        KILL(lbits)(&zgsz3503);
      }
      zgsz3502 = UNIT;
      } else {  zgsz3502 = UNIT;  }
    }
    uint64_t zlen_ie;
    {
      lbits zgsz3508;
      CREATE(lbits)(&zgsz3508);
      CONVERT_OF(lbits, fbits)(&zgsz3508, zT_ie, UINT64_C(5) , true);
      lbits zgsz3509;
      CREATE(lbits)(&zgsz3509);
      CONVERT_OF(lbits, fbits)(&zgsz3509, zB_ie, UINT64_C(5) , true);
      lbits zgsz3510;
      CREATE(lbits)(&zgsz3510);
      sub_bits(&zgsz3510, zgsz3508, zgsz3509);
      zlen_ie = CONVERT_OF(fbits, lbits)(zgsz3510, true);
      KILL(lbits)(&zgsz3510);
      KILL(lbits)(&zgsz3509);
      KILL(lbits)(&zgsz3508);
    }
    {
      bool zgaz3199;
      {
        fbits zgaz3198;
        {
          int64_t zgaz3197;
          {
            sail_int zgsz3511;
            CREATE(sail_int)(&zgsz3511);
            CONVERT_OF(sail_int, mach_int)(&zgsz3511, zmantissa_width);
            sail_int zgsz3512;
            CREATE(sail_int)(&zgsz3512);
            CONVERT_OF(sail_int, mach_int)(&zgsz3512, INT64_C(4));
            sail_int zgsz3513;
            CREATE(sail_int)(&zgsz3513);
            sub_int(&zgsz3513, zgsz3511, zgsz3512);
            zgaz3197 = CONVERT_OF(mach_int, sail_int)(zgsz3513);
            KILL(sail_int)(&zgsz3513);
            KILL(sail_int)(&zgsz3512);
            KILL(sail_int)(&zgsz3511);
          }
          {
            sail_int zgsz3515;
            CREATE(sail_int)(&zgsz3515);
            CONVERT_OF(sail_int, mach_int)(&zgsz3515, zgaz3197);
            lbits zgsz3514;
            CREATE(lbits)(&zgsz3514);
            CONVERT_OF(lbits, fbits)(&zgsz3514, zlen_ie, UINT64_C(5) , true);
            zgaz3198 = bitvector_access(zgsz3514, zgsz3515);
            KILL(sail_int)(&zgsz3515);
            KILL(lbits)(&zgsz3514);
          }
        
        }
        zgaz3199 = eq_bit(zgaz3198, UINT64_C(1));
      
      }
      unit zgsz3520;
      if (zgaz3199) {
      {
        zincE = true;
        unit zgsz3542;
        zgsz3542 = UNIT;
      }
      {
        bool zgsz3523;
        if (zlostSignificantBase) {  zgsz3523 = true;  } else {
        fbits zgaz3200;
        {
          sail_int zgsz3522;
          CREATE(sail_int)(&zgsz3522);
          CONVERT_OF(sail_int, mach_int)(&zgsz3522, INT64_C(0));
          lbits zgsz3521;
          CREATE(lbits)(&zgsz3521);
          CONVERT_OF(lbits, fbits)(&zgsz3521, zB_ie, UINT64_C(5) , true);
          zgaz3200 = bitvector_access(zgsz3521, zgsz3522);
          KILL(sail_int)(&zgsz3522);
          KILL(lbits)(&zgsz3521);
        }
        zgsz3523 = eq_bit(zgaz3200, UINT64_C(1));
      
        }
        zlostSignificantBase = zgsz3523;
        unit zgsz3541;
        zgsz3541 = UNIT;
      }
      {
        bool zgsz3526;
        if (zlostSignificantTop) {  zgsz3526 = true;  } else {
        fbits zgaz3201;
        {
          sail_int zgsz3525;
          CREATE(sail_int)(&zgsz3525);
          CONVERT_OF(sail_int, mach_int)(&zgsz3525, INT64_C(0));
          lbits zgsz3524;
          CREATE(lbits)(&zgsz3524);
          CONVERT_OF(lbits, fbits)(&zgsz3524, zT_ie, UINT64_C(5) , true);
          zgaz3201 = bitvector_access(zgsz3524, zgsz3525);
          KILL(sail_int)(&zgsz3525);
          KILL(lbits)(&zgsz3524);
        }
        zgsz3526 = eq_bit(zgaz3201, UINT64_C(1));
      
        }
        zlostSignificantTop = zgsz3526;
        unit zgsz3540;
        zgsz3540 = UNIT;
      }
      {
        uint64_t zgaz3203;
        {
          int64_t zgaz3202;
          {
            sail_int zgsz3527;
            CREATE(sail_int)(&zgsz3527);
            CONVERT_OF(sail_int, mach_int)(&zgsz3527, ze);
            sail_int zgsz3528;
            CREATE(sail_int)(&zgsz3528);
            CONVERT_OF(sail_int, mach_int)(&zgsz3528, INT64_C(4));
            sail_int zgsz3529;
            CREATE(sail_int)(&zgsz3529);
            add_int(&zgsz3529, zgsz3527, zgsz3528);
            zgaz3202 = CONVERT_OF(mach_int, sail_int)(zgsz3529);
            KILL(sail_int)(&zgsz3529);
            KILL(sail_int)(&zgsz3528);
            KILL(sail_int)(&zgsz3527);
          }
          {
            lbits zgsz3530;
            CREATE(lbits)(&zgsz3530);
            CONVERT_OF(lbits, fbits)(&zgsz3530, zbase, UINT64_C(32) , true);
            sail_int zgsz3531;
            CREATE(sail_int)(&zgsz3531);
            CONVERT_OF(sail_int, mach_int)(&zgsz3531, zgaz3202);
            lbits zgsz3532;
            CREATE(lbits)(&zgsz3532);
            shiftr(&zgsz3532, zgsz3530, zgsz3531);
            zgaz3203 = CONVERT_OF(fbits, lbits)(zgsz3532, true);
            KILL(lbits)(&zgsz3532);
            KILL(sail_int)(&zgsz3531);
            KILL(lbits)(&zgsz3530);
          }
        
        }
        int64_t zgaz3204;
        {
          sail_int zgsz3533;
          CREATE(sail_int)(&zgsz3533);
          CONVERT_OF(sail_int, mach_int)(&zgsz3533, zmantissa_width);
          sail_int zgsz3534;
          CREATE(sail_int)(&zgsz3534);
          CONVERT_OF(sail_int, mach_int)(&zgsz3534, INT64_C(3));
          sail_int zgsz3535;
          CREATE(sail_int)(&zgsz3535);
          sub_int(&zgsz3535, zgsz3533, zgsz3534);
          zgaz3204 = CONVERT_OF(mach_int, sail_int)(zgsz3535);
          KILL(sail_int)(&zgsz3535);
          KILL(sail_int)(&zgsz3534);
          KILL(sail_int)(&zgsz3533);
        }
        {
          lbits zgsz3536;
          CREATE(lbits)(&zgsz3536);
          CONVERT_OF(lbits, fbits)(&zgsz3536, zgaz3203, UINT64_C(32) , true);
          sail_int zgsz3537;
          CREATE(sail_int)(&zgsz3537);
          CONVERT_OF(sail_int, mach_int)(&zgsz3537, zgaz3204);
          lbits zgsz3538;
          CREATE(lbits)(&zgsz3538);
          sail_truncate(&zgsz3538, zgsz3536, zgsz3537);
          zB_ie = CONVERT_OF(fbits, lbits)(zgsz3538, true);
          KILL(lbits)(&zgsz3538);
          KILL(sail_int)(&zgsz3537);
          KILL(lbits)(&zgsz3536);
        }
        unit zgsz3539;
        zgsz3539 = UNIT;
      
      
      }
      int64_t zincT;
      if (zlostSignificantTop) {  zincT = INT64_C(1);  } else {  zincT = INT64_C(0);  }
      uint64_t zgaz3208;
      {
        uint64_t zgaz3206;
        {
          int64_t zgaz3205;
          {
            sail_int zgsz3543;
            CREATE(sail_int)(&zgsz3543);
            CONVERT_OF(sail_int, mach_int)(&zgsz3543, ze);
            sail_int zgsz3544;
            CREATE(sail_int)(&zgsz3544);
            CONVERT_OF(sail_int, mach_int)(&zgsz3544, INT64_C(4));
            sail_int zgsz3545;
            CREATE(sail_int)(&zgsz3545);
            add_int(&zgsz3545, zgsz3543, zgsz3544);
            zgaz3205 = CONVERT_OF(mach_int, sail_int)(zgsz3545);
            KILL(sail_int)(&zgsz3545);
            KILL(sail_int)(&zgsz3544);
            KILL(sail_int)(&zgsz3543);
          }
          {
            lbits zgsz3546;
            CREATE(lbits)(&zgsz3546);
            CONVERT_OF(lbits, fbits)(&zgsz3546, ztop, UINT64_C(33) , true);
            sail_int zgsz3547;
            CREATE(sail_int)(&zgsz3547);
            CONVERT_OF(sail_int, mach_int)(&zgsz3547, zgaz3205);
            lbits zgsz3548;
            CREATE(lbits)(&zgsz3548);
            shiftr(&zgsz3548, zgsz3546, zgsz3547);
            zgaz3206 = CONVERT_OF(fbits, lbits)(zgsz3548, true);
            KILL(lbits)(&zgsz3548);
            KILL(sail_int)(&zgsz3547);
            KILL(lbits)(&zgsz3546);
          }
        
        }
        int64_t zgaz3207;
        {
          sail_int zgsz3549;
          CREATE(sail_int)(&zgsz3549);
          CONVERT_OF(sail_int, mach_int)(&zgsz3549, zmantissa_width);
          sail_int zgsz3550;
          CREATE(sail_int)(&zgsz3550);
          CONVERT_OF(sail_int, mach_int)(&zgsz3550, INT64_C(3));
          sail_int zgsz3551;
          CREATE(sail_int)(&zgsz3551);
          sub_int(&zgsz3551, zgsz3549, zgsz3550);
          zgaz3207 = CONVERT_OF(mach_int, sail_int)(zgsz3551);
          KILL(sail_int)(&zgsz3551);
          KILL(sail_int)(&zgsz3550);
          KILL(sail_int)(&zgsz3549);
        }
        {
          lbits zgsz3552;
          CREATE(lbits)(&zgsz3552);
          CONVERT_OF(lbits, fbits)(&zgsz3552, zgaz3206, UINT64_C(33) , true);
          sail_int zgsz3553;
          CREATE(sail_int)(&zgsz3553);
          CONVERT_OF(sail_int, mach_int)(&zgsz3553, zgaz3207);
          lbits zgsz3554;
          CREATE(lbits)(&zgsz3554);
          sail_truncate(&zgsz3554, zgsz3552, zgsz3553);
          zgaz3208 = CONVERT_OF(fbits, lbits)(zgsz3554, true);
          KILL(lbits)(&zgsz3554);
          KILL(sail_int)(&zgsz3553);
          KILL(lbits)(&zgsz3552);
        }
      
      
      }
      {
        lbits zgsz3555;
        CREATE(lbits)(&zgsz3555);
        CONVERT_OF(lbits, fbits)(&zgsz3555, zgaz3208, UINT64_C(5) , true);
        sail_int zgsz3556;
        CREATE(sail_int)(&zgsz3556);
        CONVERT_OF(sail_int, mach_int)(&zgsz3556, zincT);
        lbits zgsz3557;
        CREATE(lbits)(&zgsz3557);
        add_bits_int(&zgsz3557, zgsz3555, zgsz3556);
        zT_ie = CONVERT_OF(fbits, lbits)(zgsz3557, true);
        KILL(lbits)(&zgsz3557);
        KILL(sail_int)(&zgsz3556);
        KILL(lbits)(&zgsz3555);
      }
      zgsz3520 = UNIT;
    
    
      } else {  zgsz3520 = UNIT;  }
    
    }
    {
      {
        lbits zgsz3516;
        CREATE(lbits)(&zgsz3516);
        CONVERT_OF(lbits, fbits)(&zgsz3516, zB_ie, UINT64_C(5) , true);
        lbits zgsz3517;
        CREATE(lbits)(&zgsz3517);
        CONVERT_OF(lbits, fbits)(&zgsz3517, UINT64_C(0b000), UINT64_C(3) , true);
        lbits zgsz3518;
        CREATE(lbits)(&zgsz3518);
        append(&zgsz3518, zgsz3516, zgsz3517);
        zBbits = CONVERT_OF(fbits, lbits)(zgsz3518, true);
        KILL(lbits)(&zgsz3518);
        KILL(lbits)(&zgsz3517);
        KILL(lbits)(&zgsz3516);
      }
      unit zgsz3519;
      zgsz3519 = UNIT;
    }
    {
      lbits zgsz3558;
      CREATE(lbits)(&zgsz3558);
      CONVERT_OF(lbits, fbits)(&zgsz3558, zT_ie, UINT64_C(5) , true);
      lbits zgsz3559;
      CREATE(lbits)(&zgsz3559);
      CONVERT_OF(lbits, fbits)(&zgsz3559, UINT64_C(0b000), UINT64_C(3) , true);
      lbits zgsz3560;
      CREATE(lbits)(&zgsz3560);
      append(&zgsz3560, zgsz3558, zgsz3559);
      zTbits = CONVERT_OF(fbits, lbits)(zgsz3560, true);
      KILL(lbits)(&zgsz3560);
      KILL(lbits)(&zgsz3559);
      KILL(lbits)(&zgsz3558);
    }
    zgsz3458 = UNIT;
  
  
  
  
    } else {  zgsz3458 = UNIT;  }
  }
  struct zCapability znewCap;
  {
    uint64_t zgaz3220;
    {
      int64_t zgaz3219;
      if (zincE) {
      {
        sail_int zgsz3561;
        CREATE(sail_int)(&zgsz3561);
        CONVERT_OF(sail_int, mach_int)(&zgsz3561, ze);
        sail_int zgsz3562;
        CREATE(sail_int)(&zgsz3562);
        CONVERT_OF(sail_int, mach_int)(&zgsz3562, INT64_C(1));
        sail_int zgsz3563;
        CREATE(sail_int)(&zgsz3563);
        add_int(&zgsz3563, zgsz3561, zgsz3562);
        zgaz3219 = CONVERT_OF(mach_int, sail_int)(zgsz3563);
        KILL(sail_int)(&zgsz3563);
        KILL(sail_int)(&zgsz3562);
        KILL(sail_int)(&zgsz3561);
      }
      } else {  zgaz3219 = ze;  }
      {
        sail_int zgsz3564;
        CREATE(sail_int)(&zgsz3564);
        CONVERT_OF(sail_int, mach_int)(&zgsz3564, zgaz3219);
        zgaz3220 = sailgen_zzzz7lzzJzzK6z3to_bits(INT64_C(6), zgsz3564);
        KILL(sail_int)(&zgsz3564);
      }
    
    }
    struct zCapability zgsz3565;
    zgsz3565 = zcap;
    zgsz3565.zB = zBbits;
    zgsz3565.zE = zgaz3220;
    zgsz3565.zT = zTbits;
    zgsz3565.zaddress = zbase;
    zgsz3565.zinternal_e = zie;
    znewCap = zgsz3565;
  
  
  }
  bool zexact;
  {
    bool zgaz3218;
    {
      bool zgsz3566;
      if (zlostSignificantBase) {  zgsz3566 = true;  } else {  zgsz3566 = zlostSignificantTop;  }
      zgaz3218 = zgsz3566;
    }
    zexact = not(zgaz3218);
  
  }
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgsz3567;
  zgsz3567.ztup0 = zexact;
  zgsz3567.ztup1 = znewCap;
  zcbz341 = zgsz3567;












end_function_160: ;
  return zcbz341;
end_block_exception_161: ;
  struct zCapability zcbz359 = { .zuperms = UINT64_C(0xdeadc0de), .ztag = false, .zsealed = false, .zreserved = UINT64_C(0xdeadc0de), .zpermit_unseal = false, .zpermit_store_local_cap = false, .zpermit_store_cap = false, .zpermit_store = false, .zpermit_set_CID = false, .zpermit_seal = false, .zpermit_load_cap = false, .zpermit_load = false, .zpermit_execute = false, .zpermit_ccall = false, .zotype = UINT64_C(0xdeadc0de), .zinternal_e = false, .zglobal = false, .zflag_cap_mode = false, .zaddress = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zT = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz358 = { .ztup1 = zcbz359, .ztup0 = false };
  return zcbz358;
}

static uint64_t sailgen_getCapPerms(struct zCapability);

static uint64_t sailgen_getCapPerms(struct zCapability zcap)
{
  __label__ end_function_163, end_block_exception_164;

  uint64_t zcbz342;
  uint64_t zperms;
  {
    uint64_t zgaz3229;
    zgaz3229 = sailgen_getCapHardPerms(zcap);
    zperms = sailgen_zzzz7mzzJzzK15zzCzz0zz7nzzJzzK12z3EXTZ(INT64_C(15), zgaz3229);
  
  }
  uint64_t zgaz3228;
  {
    uint64_t zgaz3227;
    zgaz3227 = zcap.zuperms;
    {
      lbits zgsz3568;
      CREATE(lbits)(&zgsz3568);
      CONVERT_OF(lbits, fbits)(&zgsz3568, zgaz3227, UINT64_C(0) , true);
      lbits zgsz3569;
      CREATE(lbits)(&zgsz3569);
      CONVERT_OF(lbits, fbits)(&zgsz3569, zperms, UINT64_C(15) , true);
      lbits zgsz3570;
      CREATE(lbits)(&zgsz3570);
      append(&zgsz3570, zgsz3568, zgsz3569);
      zgaz3228 = CONVERT_OF(fbits, lbits)(zgsz3570, true);
      KILL(lbits)(&zgsz3570);
      KILL(lbits)(&zgsz3569);
      KILL(lbits)(&zgsz3568);
    }
  
  }
  {
    sbits zgsz3571;
    zgsz3571 = CONVERT_OF(sbits, fbits)(zgaz3228, UINT64_C(15) , true);
    zcbz342 = sailgen_zzz3zzzz7mzzJzzK31z3EXTZ(INT64_C(31), zgsz3571);
  
  }


end_function_163: ;
  return zcbz342;
end_block_exception_164: ;

  return UINT64_C(0xdeadc0de);
}

static struct zCapability sailgen_setCapPerms(struct zCapability, uint64_t);

static struct zCapability sailgen_setCapPerms(struct zCapability zcap, uint64_t zperms)
{
  __label__ end_function_166, end_block_exception_167;

  struct zCapability zcbz343;
  uint64_t zgaz3231;
  {
    uint64_t zgaz3230;
    {
      lbits zgsz3572;
      CREATE(lbits)(&zgsz3572);
      CONVERT_OF(lbits, fbits)(&zgsz3572, zperms, UINT64_C(31) , true);
      sail_int zgsz3573;
      CREATE(sail_int)(&zgsz3573);
      CONVERT_OF(sail_int, mach_int)(&zgsz3573, INT64_C(30));
      sail_int zgsz3574;
      CREATE(sail_int)(&zgsz3574);
      CONVERT_OF(sail_int, mach_int)(&zgsz3574, INT64_C(15));
      lbits zgsz3575;
      CREATE(lbits)(&zgsz3575);
      vector_subrange_lbits(&zgsz3575, zgsz3572, zgsz3573, zgsz3574);
      zgaz3230 = CONVERT_OF(fbits, lbits)(zgsz3575, true);
      KILL(lbits)(&zgsz3575);
      KILL(sail_int)(&zgsz3574);
      KILL(sail_int)(&zgsz3573);
      KILL(lbits)(&zgsz3572);
    }
    {
      lbits zgsz3576;
      CREATE(lbits)(&zgsz3576);
      CONVERT_OF(lbits, fbits)(&zgsz3576, zgaz3230, UINT64_C(16) , true);
      sail_int zgsz3577;
      CREATE(sail_int)(&zgsz3577);
      CONVERT_OF(sail_int, mach_int)(&zgsz3577, zuperms_width);
      lbits zgsz3578;
      CREATE(lbits)(&zgsz3578);
      sail_truncate(&zgsz3578, zgsz3576, zgsz3577);
      zgaz3231 = CONVERT_OF(fbits, lbits)(zgsz3578, true);
      KILL(lbits)(&zgsz3578);
      KILL(sail_int)(&zgsz3577);
      KILL(lbits)(&zgsz3576);
    }
  
  }
  bool zgaz3233;
  {
    fbits zgaz3232;
    {
      sail_int zgsz3580;
      CREATE(sail_int)(&zgsz3580);
      CONVERT_OF(sail_int, mach_int)(&zgsz3580, INT64_C(11));
      lbits zgsz3579;
      CREATE(lbits)(&zgsz3579);
      CONVERT_OF(lbits, fbits)(&zgsz3579, zperms, UINT64_C(31) , true);
      zgaz3232 = bitvector_access(zgsz3579, zgsz3580);
      KILL(sail_int)(&zgsz3580);
      KILL(lbits)(&zgsz3579);
    }
    zgaz3233 = sailgen_bit_to_bool(zgaz3232);
  
  }
  bool zgaz3235;
  {
    fbits zgaz3234;
    {
      sail_int zgsz3582;
      CREATE(sail_int)(&zgsz3582);
      CONVERT_OF(sail_int, mach_int)(&zgsz3582, INT64_C(10));
      lbits zgsz3581;
      CREATE(lbits)(&zgsz3581);
      CONVERT_OF(lbits, fbits)(&zgsz3581, zperms, UINT64_C(31) , true);
      zgaz3234 = bitvector_access(zgsz3581, zgsz3582);
      KILL(sail_int)(&zgsz3582);
      KILL(lbits)(&zgsz3581);
    }
    zgaz3235 = sailgen_bit_to_bool(zgaz3234);
  
  }
  bool zgaz3237;
  {
    fbits zgaz3236;
    {
      sail_int zgsz3584;
      CREATE(sail_int)(&zgsz3584);
      CONVERT_OF(sail_int, mach_int)(&zgsz3584, INT64_C(9));
      lbits zgsz3583;
      CREATE(lbits)(&zgsz3583);
      CONVERT_OF(lbits, fbits)(&zgsz3583, zperms, UINT64_C(31) , true);
      zgaz3236 = bitvector_access(zgsz3583, zgsz3584);
      KILL(sail_int)(&zgsz3584);
      KILL(lbits)(&zgsz3583);
    }
    zgaz3237 = sailgen_bit_to_bool(zgaz3236);
  
  }
  bool zgaz3239;
  {
    fbits zgaz3238;
    {
      sail_int zgsz3586;
      CREATE(sail_int)(&zgsz3586);
      CONVERT_OF(sail_int, mach_int)(&zgsz3586, INT64_C(8));
      lbits zgsz3585;
      CREATE(lbits)(&zgsz3585);
      CONVERT_OF(lbits, fbits)(&zgsz3585, zperms, UINT64_C(31) , true);
      zgaz3238 = bitvector_access(zgsz3585, zgsz3586);
      KILL(sail_int)(&zgsz3586);
      KILL(lbits)(&zgsz3585);
    }
    zgaz3239 = sailgen_bit_to_bool(zgaz3238);
  
  }
  bool zgaz3241;
  {
    fbits zgaz3240;
    {
      sail_int zgsz3588;
      CREATE(sail_int)(&zgsz3588);
      CONVERT_OF(sail_int, mach_int)(&zgsz3588, INT64_C(7));
      lbits zgsz3587;
      CREATE(lbits)(&zgsz3587);
      CONVERT_OF(lbits, fbits)(&zgsz3587, zperms, UINT64_C(31) , true);
      zgaz3240 = bitvector_access(zgsz3587, zgsz3588);
      KILL(sail_int)(&zgsz3588);
      KILL(lbits)(&zgsz3587);
    }
    zgaz3241 = sailgen_bit_to_bool(zgaz3240);
  
  }
  bool zgaz3243;
  {
    fbits zgaz3242;
    {
      sail_int zgsz3590;
      CREATE(sail_int)(&zgsz3590);
      CONVERT_OF(sail_int, mach_int)(&zgsz3590, INT64_C(6));
      lbits zgsz3589;
      CREATE(lbits)(&zgsz3589);
      CONVERT_OF(lbits, fbits)(&zgsz3589, zperms, UINT64_C(31) , true);
      zgaz3242 = bitvector_access(zgsz3589, zgsz3590);
      KILL(sail_int)(&zgsz3590);
      KILL(lbits)(&zgsz3589);
    }
    zgaz3243 = sailgen_bit_to_bool(zgaz3242);
  
  }
  bool zgaz3245;
  {
    fbits zgaz3244;
    {
      sail_int zgsz3592;
      CREATE(sail_int)(&zgsz3592);
      CONVERT_OF(sail_int, mach_int)(&zgsz3592, INT64_C(5));
      lbits zgsz3591;
      CREATE(lbits)(&zgsz3591);
      CONVERT_OF(lbits, fbits)(&zgsz3591, zperms, UINT64_C(31) , true);
      zgaz3244 = bitvector_access(zgsz3591, zgsz3592);
      KILL(sail_int)(&zgsz3592);
      KILL(lbits)(&zgsz3591);
    }
    zgaz3245 = sailgen_bit_to_bool(zgaz3244);
  
  }
  bool zgaz3247;
  {
    fbits zgaz3246;
    {
      sail_int zgsz3594;
      CREATE(sail_int)(&zgsz3594);
      CONVERT_OF(sail_int, mach_int)(&zgsz3594, INT64_C(4));
      lbits zgsz3593;
      CREATE(lbits)(&zgsz3593);
      CONVERT_OF(lbits, fbits)(&zgsz3593, zperms, UINT64_C(31) , true);
      zgaz3246 = bitvector_access(zgsz3593, zgsz3594);
      KILL(sail_int)(&zgsz3594);
      KILL(lbits)(&zgsz3593);
    }
    zgaz3247 = sailgen_bit_to_bool(zgaz3246);
  
  }
  bool zgaz3249;
  {
    fbits zgaz3248;
    {
      sail_int zgsz3596;
      CREATE(sail_int)(&zgsz3596);
      CONVERT_OF(sail_int, mach_int)(&zgsz3596, INT64_C(3));
      lbits zgsz3595;
      CREATE(lbits)(&zgsz3595);
      CONVERT_OF(lbits, fbits)(&zgsz3595, zperms, UINT64_C(31) , true);
      zgaz3248 = bitvector_access(zgsz3595, zgsz3596);
      KILL(sail_int)(&zgsz3596);
      KILL(lbits)(&zgsz3595);
    }
    zgaz3249 = sailgen_bit_to_bool(zgaz3248);
  
  }
  bool zgaz3251;
  {
    fbits zgaz3250;
    {
      sail_int zgsz3598;
      CREATE(sail_int)(&zgsz3598);
      CONVERT_OF(sail_int, mach_int)(&zgsz3598, INT64_C(2));
      lbits zgsz3597;
      CREATE(lbits)(&zgsz3597);
      CONVERT_OF(lbits, fbits)(&zgsz3597, zperms, UINT64_C(31) , true);
      zgaz3250 = bitvector_access(zgsz3597, zgsz3598);
      KILL(sail_int)(&zgsz3598);
      KILL(lbits)(&zgsz3597);
    }
    zgaz3251 = sailgen_bit_to_bool(zgaz3250);
  
  }
  bool zgaz3253;
  {
    fbits zgaz3252;
    {
      sail_int zgsz3600;
      CREATE(sail_int)(&zgsz3600);
      CONVERT_OF(sail_int, mach_int)(&zgsz3600, INT64_C(1));
      lbits zgsz3599;
      CREATE(lbits)(&zgsz3599);
      CONVERT_OF(lbits, fbits)(&zgsz3599, zperms, UINT64_C(31) , true);
      zgaz3252 = bitvector_access(zgsz3599, zgsz3600);
      KILL(sail_int)(&zgsz3600);
      KILL(lbits)(&zgsz3599);
    }
    zgaz3253 = sailgen_bit_to_bool(zgaz3252);
  
  }
  bool zgaz3255;
  {
    fbits zgaz3254;
    {
      sail_int zgsz3602;
      CREATE(sail_int)(&zgsz3602);
      CONVERT_OF(sail_int, mach_int)(&zgsz3602, INT64_C(0));
      lbits zgsz3601;
      CREATE(lbits)(&zgsz3601);
      CONVERT_OF(lbits, fbits)(&zgsz3601, zperms, UINT64_C(31) , true);
      zgaz3254 = bitvector_access(zgsz3601, zgsz3602);
      KILL(sail_int)(&zgsz3602);
      KILL(lbits)(&zgsz3601);
    }
    zgaz3255 = sailgen_bit_to_bool(zgaz3254);
  
  }
  struct zCapability zgsz3603;
  zgsz3603 = zcap;
  zgsz3603.zaccess_system_regs = zgaz3235;
  zgsz3603.zglobal = zgaz3255;
  zgsz3603.zpermit_ccall = zgaz3239;
  zgsz3603.zpermit_execute = zgaz3253;
  zgsz3603.zpermit_load = zgaz3251;
  zgsz3603.zpermit_load_cap = zgaz3247;
  zgsz3603.zpermit_seal = zgaz3241;
  zgsz3603.zpermit_set_CID = zgaz3233;
  zgsz3603.zpermit_store = zgaz3249;
  zgsz3603.zpermit_store_cap = zgaz3245;
  zgsz3603.zpermit_store_local_cap = zgaz3243;
  zgsz3603.zpermit_unseal = zgaz3237;
  zgsz3603.zuperms = zgaz3231;
  zcbz343 = zgsz3603;














end_function_166: ;
  return zcbz343;
end_block_exception_167: ;
  struct zCapability zcbz360 = { .zuperms = UINT64_C(0xdeadc0de), .ztag = false, .zsealed = false, .zreserved = UINT64_C(0xdeadc0de), .zpermit_unseal = false, .zpermit_store_local_cap = false, .zpermit_store_cap = false, .zpermit_store = false, .zpermit_set_CID = false, .zpermit_seal = false, .zpermit_load_cap = false, .zpermit_load = false, .zpermit_execute = false, .zpermit_ccall = false, .zotype = UINT64_C(0xdeadc0de), .zinternal_e = false, .zglobal = false, .zflag_cap_mode = false, .zaddress = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zT = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  return zcbz360;
}

static uint64_t sailgen_getCapFlags(struct zCapability);

static uint64_t sailgen_getCapFlags(struct zCapability zcap)
{
  __label__ end_function_169, end_block_exception_170;

  uint64_t zcbz344;
  bool zgaz3256;
  zgaz3256 = zcap.zflag_cap_mode;
  zcbz344 = sailgen_bool_to_bits(zgaz3256);

end_function_169: ;
  return zcbz344;
end_block_exception_170: ;

  return UINT64_C(0xdeadc0de);
}

static bool sailgen_hasReservedOType(struct zCapability);

static bool sailgen_hasReservedOType(struct zCapability zcap)
{
  __label__ end_function_172, end_block_exception_173;

  bool zcbz345;
  int64_t zgaz3258;
  {
    uint64_t zgaz3257;
    zgaz3257 = zcap.zotype;
    zgaz3258 = ((mach_int) zgaz3257);
  
  }
  {
    sail_int zgsz3605;
    CREATE(sail_int)(&zgsz3605);
    CONVERT_OF(sail_int, mach_int)(&zgsz3605, zmax_otype);
    sail_int zgsz3604;
    CREATE(sail_int)(&zgsz3604);
    CONVERT_OF(sail_int, mach_int)(&zgsz3604, zgaz3258);
    zcbz345 = gt(zgsz3604, zgsz3605);
    KILL(sail_int)(&zgsz3605);
    KILL(sail_int)(&zgsz3604);
  }

end_function_172: ;
  return zcbz345;
end_block_exception_173: ;

  return false;
}

static uint64_t sailgen_getCapBaseBits(struct zCapability);

static uint64_t sailgen_getCapBaseBits(struct zCapability zc)
{
  __label__ case_176, finish_match_175, end_function_177, end_block_exception_178;

  uint64_t zcbz346;
  struct ztuple_z8z5bv32zCz0z5bv33z9 zgaz3259;
  zgaz3259 = sailgen_getCapBoundsBits(zc);
  uint64_t zgsz3606;
  {
    uint64_t zbase;
    zbase = zgaz3259.ztup0;
    zgsz3606 = zbase;
  
    goto finish_match_175;
  }
case_176: ;
  sail_match_failure("getCapBaseBits");
finish_match_175: ;
  zcbz346 = zgsz3606;


end_function_177: ;
  return zcbz346;
end_block_exception_178: ;

  return UINT64_C(0xdeadc0de);
}

static uint64_t sailgen_getCapOffsetBits(struct zCapability);

static uint64_t sailgen_getCapOffsetBits(struct zCapability zc)
{
  __label__ end_function_180, end_block_exception_181;

  uint64_t zcbz347;
  uint64_t zbase;
  zbase = sailgen_getCapBaseBits(zc);
  uint64_t zgaz3260;
  zgaz3260 = zc.zaddress;
  {
    lbits zgsz3608;
    CREATE(lbits)(&zgsz3608);
    CONVERT_OF(lbits, fbits)(&zgsz3608, zgaz3260, UINT64_C(32) , true);
    lbits zgsz3609;
    CREATE(lbits)(&zgsz3609);
    CONVERT_OF(lbits, fbits)(&zgsz3609, zbase, UINT64_C(32) , true);
    lbits zgsz3610;
    CREATE(lbits)(&zgsz3610);
    sub_bits(&zgsz3610, zgsz3608, zgsz3609);
    zcbz347 = CONVERT_OF(fbits, lbits)(zgsz3610, true);
    KILL(lbits)(&zgsz3610);
    KILL(lbits)(&zgsz3609);
    KILL(lbits)(&zgsz3608);
  }


end_function_180: ;
  return zcbz347;
end_block_exception_181: ;

  return UINT64_C(0xdeadc0de);
}

static int64_t sailgen_getCapLength(struct zCapability);

static int64_t sailgen_getCapLength(struct zCapability zc)
{
  __label__ case_184, finish_match_183, end_function_185, end_block_exception_186;

  int64_t zcbz348;
  struct ztuple_z8z5i64zCz0z5i64z9 zgaz3261;
  zgaz3261 = sailgen_getCapBounds(zc);
  int64_t zgsz3611;
  {
    int64_t zbase;
    zbase = zgaz3261.ztup0;
    int64_t ztop;
    ztop = zgaz3261.ztup1;
    {
      bool zgaz3264;
      {
        bool zgaz3263;
        {
          bool zgaz3262;
          zgaz3262 = zc.ztag;
          zgaz3263 = not(zgaz3262);
        
        }
        bool zgsz3612;
        if (zgaz3263) {  zgsz3612 = true;  } else {
        {
          sail_int zgsz3614;
          CREATE(sail_int)(&zgsz3614);
          CONVERT_OF(sail_int, mach_int)(&zgsz3614, zbase);
          sail_int zgsz3613;
          CREATE(sail_int)(&zgsz3613);
          CONVERT_OF(sail_int, mach_int)(&zgsz3613, ztop);
          zgsz3612 = gteq(zgsz3613, zgsz3614);
          KILL(sail_int)(&zgsz3614);
          KILL(sail_int)(&zgsz3613);
        }
        }
        zgaz3264 = zgsz3612;
      
      }
      unit zgsz3615;
      zgsz3615 = sail_assert(zgaz3264, "src/cheri_cap_common.sail 271:40 - 271:41");
    
    }
    int64_t zgaz3265;
    {
      sail_int zgsz3616;
      CREATE(sail_int)(&zgsz3616);
      CONVERT_OF(sail_int, mach_int)(&zgsz3616, ztop);
      sail_int zgsz3617;
      CREATE(sail_int)(&zgsz3617);
      CONVERT_OF(sail_int, mach_int)(&zgsz3617, zbase);
      sail_int zgsz3618;
      CREATE(sail_int)(&zgsz3618);
      sub_int(&zgsz3618, zgsz3616, zgsz3617);
      zgaz3265 = CONVERT_OF(mach_int, sail_int)(zgsz3618);
      KILL(sail_int)(&zgsz3618);
      KILL(sail_int)(&zgsz3617);
      KILL(sail_int)(&zgsz3616);
    }
    int64_t zgaz3266;
    {
      sail_int zgsz3619;
      CREATE(sail_int)(&zgsz3619);
      CONVERT_OF(sail_int, mach_int)(&zgsz3619, zcap_len_width);
      sail_int zgsz3620;
      CREATE(sail_int)(&zgsz3620);
      pow2(&zgsz3620, zgsz3619);
      zgaz3266 = CONVERT_OF(mach_int, sail_int)(zgsz3620);
      KILL(sail_int)(&zgsz3620);
      KILL(sail_int)(&zgsz3619);
    }
    {
      sail_int zgsz3622;
      CREATE(sail_int)(&zgsz3622);
      CONVERT_OF(sail_int, mach_int)(&zgsz3622, zgaz3265);
      sail_int zgsz3623;
      CREATE(sail_int)(&zgsz3623);
      CONVERT_OF(sail_int, mach_int)(&zgsz3623, zgaz3266);
      sail_int zgsz3624;
      CREATE(sail_int)(&zgsz3624);
      emod_int(&zgsz3624, zgsz3622, zgsz3623);
      zgsz3611 = CONVERT_OF(mach_int, sail_int)(zgsz3624);
      KILL(sail_int)(&zgsz3624);
      KILL(sail_int)(&zgsz3623);
      KILL(sail_int)(&zgsz3622);
    }
  
  
  
  
    goto finish_match_183;
  }
case_184: ;
  sail_match_failure("getCapLength");
finish_match_183: ;
  zcbz348 = zgsz3611;


end_function_185: ;
  return zcbz348;
end_block_exception_186: ;

  return INT64_C(0xdeadc0de);
}

static bool sailgen_fastRepCheck(struct zCapability, uint64_t);

static bool sailgen_fastRepCheck(struct zCapability zc, uint64_t zi)
{
  __label__ end_function_188, end_block_exception_189;

  bool zcbz349;
  int64_t zE;
  {
    uint64_t zgaz3280;
    zgaz3280 = zc.zE;
    zE = ((mach_int) zgaz3280);
  
  }
  bool zgaz3268;
  {
    int64_t zgaz3267;
    {
      sail_int zgsz3625;
      CREATE(sail_int)(&zgsz3625);
      CONVERT_OF(sail_int, mach_int)(&zgsz3625, zmaxE);
      sail_int zgsz3626;
      CREATE(sail_int)(&zgsz3626);
      CONVERT_OF(sail_int, mach_int)(&zgsz3626, INT64_C(2));
      sail_int zgsz3627;
      CREATE(sail_int)(&zgsz3627);
      sub_int(&zgsz3627, zgsz3625, zgsz3626);
      zgaz3267 = CONVERT_OF(mach_int, sail_int)(zgsz3627);
      KILL(sail_int)(&zgsz3627);
      KILL(sail_int)(&zgsz3626);
      KILL(sail_int)(&zgsz3625);
    }
    {
      sail_int zgsz3629;
      CREATE(sail_int)(&zgsz3629);
      CONVERT_OF(sail_int, mach_int)(&zgsz3629, zgaz3267);
      sail_int zgsz3628;
      CREATE(sail_int)(&zgsz3628);
      CONVERT_OF(sail_int, mach_int)(&zgsz3628, zE);
      zgaz3268 = gteq(zgsz3628, zgsz3629);
      KILL(sail_int)(&zgsz3629);
      KILL(sail_int)(&zgsz3628);
    }
  
  }
  if (zgaz3268) {  zcbz349 = true;  } else {
  int64_t zi_top;
  {
    uint64_t zgaz3279;
    {
      int64_t zgaz3278;
      {
        sail_int zgsz3630;
        CREATE(sail_int)(&zgsz3630);
        CONVERT_OF(sail_int, mach_int)(&zgsz3630, zE);
        sail_int zgsz3631;
        CREATE(sail_int)(&zgsz3631);
        CONVERT_OF(sail_int, mach_int)(&zgsz3631, zmantissa_width);
        sail_int zgsz3632;
        CREATE(sail_int)(&zgsz3632);
        add_int(&zgsz3632, zgsz3630, zgsz3631);
        zgaz3278 = CONVERT_OF(mach_int, sail_int)(zgsz3632);
        KILL(sail_int)(&zgsz3632);
        KILL(sail_int)(&zgsz3631);
        KILL(sail_int)(&zgsz3630);
      }
      {
        lbits zgsz3633;
        CREATE(lbits)(&zgsz3633);
        CONVERT_OF(lbits, fbits)(&zgsz3633, zi, UINT64_C(32) , true);
        sail_int zgsz3634;
        CREATE(sail_int)(&zgsz3634);
        CONVERT_OF(sail_int, mach_int)(&zgsz3634, zgaz3278);
        lbits zgsz3635;
        CREATE(lbits)(&zgsz3635);
        arith_shiftr(&zgsz3635, zgsz3633, zgsz3634);
        zgaz3279 = CONVERT_OF(fbits, lbits)(zgsz3635, true);
        KILL(lbits)(&zgsz3635);
        KILL(sail_int)(&zgsz3634);
        KILL(lbits)(&zgsz3633);
      }
    
    }
    zi_top = fast_signed(zgaz3279, 32);
  
  }
  uint64_t zi_mid;
  {
    uint64_t zgaz3277;
    {
      lbits zgsz3636;
      CREATE(lbits)(&zgsz3636);
      CONVERT_OF(lbits, fbits)(&zgsz3636, zi, UINT64_C(32) , true);
      sail_int zgsz3637;
      CREATE(sail_int)(&zgsz3637);
      CONVERT_OF(sail_int, mach_int)(&zgsz3637, zE);
      lbits zgsz3638;
      CREATE(lbits)(&zgsz3638);
      shiftr(&zgsz3638, zgsz3636, zgsz3637);
      zgaz3277 = CONVERT_OF(fbits, lbits)(zgsz3638, true);
      KILL(lbits)(&zgsz3638);
      KILL(sail_int)(&zgsz3637);
      KILL(lbits)(&zgsz3636);
    }
    {
      lbits zgsz3639;
      CREATE(lbits)(&zgsz3639);
      CONVERT_OF(lbits, fbits)(&zgsz3639, zgaz3277, UINT64_C(32) , true);
      sail_int zgsz3640;
      CREATE(sail_int)(&zgsz3640);
      CONVERT_OF(sail_int, mach_int)(&zgsz3640, zmantissa_width);
      lbits zgsz3641;
      CREATE(lbits)(&zgsz3641);
      sail_truncate(&zgsz3641, zgsz3639, zgsz3640);
      zi_mid = CONVERT_OF(fbits, lbits)(zgsz3641, true);
      KILL(lbits)(&zgsz3641);
      KILL(sail_int)(&zgsz3640);
      KILL(lbits)(&zgsz3639);
    }
  
  }
  uint64_t za_mid;
  {
    uint64_t zgaz3276;
    {
      uint64_t zgaz3275;
      zgaz3275 = zc.zaddress;
      {
        lbits zgsz3642;
        CREATE(lbits)(&zgsz3642);
        CONVERT_OF(lbits, fbits)(&zgsz3642, zgaz3275, UINT64_C(32) , true);
        sail_int zgsz3643;
        CREATE(sail_int)(&zgsz3643);
        CONVERT_OF(sail_int, mach_int)(&zgsz3643, zE);
        lbits zgsz3644;
        CREATE(lbits)(&zgsz3644);
        shiftr(&zgsz3644, zgsz3642, zgsz3643);
        zgaz3276 = CONVERT_OF(fbits, lbits)(zgsz3644, true);
        KILL(lbits)(&zgsz3644);
        KILL(sail_int)(&zgsz3643);
        KILL(lbits)(&zgsz3642);
      }
    
    }
    {
      lbits zgsz3645;
      CREATE(lbits)(&zgsz3645);
      CONVERT_OF(lbits, fbits)(&zgsz3645, zgaz3276, UINT64_C(32) , true);
      sail_int zgsz3646;
      CREATE(sail_int)(&zgsz3646);
      CONVERT_OF(sail_int, mach_int)(&zgsz3646, zmantissa_width);
      lbits zgsz3647;
      CREATE(lbits)(&zgsz3647);
      sail_truncate(&zgsz3647, zgsz3645, zgsz3646);
      za_mid = CONVERT_OF(fbits, lbits)(zgsz3647, true);
      KILL(lbits)(&zgsz3647);
      KILL(sail_int)(&zgsz3646);
      KILL(lbits)(&zgsz3645);
    }
  
  }
  uint64_t zB3;
  {
    uint64_t zgaz3274;
    zgaz3274 = zc.zB;
    {
      lbits zgsz3648;
      CREATE(lbits)(&zgsz3648);
      CONVERT_OF(lbits, fbits)(&zgsz3648, zgaz3274, UINT64_C(8) , true);
      sail_int zgsz3649;
      CREATE(sail_int)(&zgsz3649);
      CONVERT_OF(sail_int, mach_int)(&zgsz3649, INT64_C(3));
      lbits zgsz3650;
      CREATE(lbits)(&zgsz3650);
      sail_truncateLSB(&zgsz3650, zgsz3648, zgsz3649);
      zB3 = CONVERT_OF(fbits, lbits)(zgsz3650, true);
      KILL(lbits)(&zgsz3650);
      KILL(sail_int)(&zgsz3649);
      KILL(lbits)(&zgsz3648);
    }
  
  }
  uint64_t zR3;
  {
    lbits zgsz3651;
    CREATE(lbits)(&zgsz3651);
    CONVERT_OF(lbits, fbits)(&zgsz3651, zB3, UINT64_C(3) , true);
    lbits zgsz3652;
    CREATE(lbits)(&zgsz3652);
    CONVERT_OF(lbits, fbits)(&zgsz3652, UINT64_C(0b001), UINT64_C(3) , true);
    lbits zgsz3653;
    CREATE(lbits)(&zgsz3653);
    sub_bits(&zgsz3653, zgsz3651, zgsz3652);
    zR3 = CONVERT_OF(fbits, lbits)(zgsz3653, true);
    KILL(lbits)(&zgsz3653);
    KILL(lbits)(&zgsz3652);
    KILL(lbits)(&zgsz3651);
  }
  uint64_t zR;
  {
    uint64_t zgaz3273;
    {
      int64_t zgaz3272;
      {
        sail_int zgsz3654;
        CREATE(sail_int)(&zgsz3654);
        CONVERT_OF(sail_int, mach_int)(&zgsz3654, zmantissa_width);
        sail_int zgsz3655;
        CREATE(sail_int)(&zgsz3655);
        CONVERT_OF(sail_int, mach_int)(&zgsz3655, INT64_C(3));
        sail_int zgsz3656;
        CREATE(sail_int)(&zgsz3656);
        sub_int(&zgsz3656, zgsz3654, zgsz3655);
        zgaz3272 = CONVERT_OF(mach_int, sail_int)(zgsz3656);
        KILL(sail_int)(&zgsz3656);
        KILL(sail_int)(&zgsz3655);
        KILL(sail_int)(&zgsz3654);
      }
      zgaz3273 = sailgen_zzzz7nzzJzzK5z3zzeros_implicit(zgaz3272);
    
    }
    {
      lbits zgsz3657;
      CREATE(lbits)(&zgsz3657);
      CONVERT_OF(lbits, fbits)(&zgsz3657, zR3, UINT64_C(3) , true);
      lbits zgsz3658;
      CREATE(lbits)(&zgsz3658);
      CONVERT_OF(lbits, fbits)(&zgsz3658, zgaz3273, UINT64_C(5) , true);
      lbits zgsz3659;
      CREATE(lbits)(&zgsz3659);
      append(&zgsz3659, zgsz3657, zgsz3658);
      zR = CONVERT_OF(fbits, lbits)(zgsz3659, true);
      KILL(lbits)(&zgsz3659);
      KILL(lbits)(&zgsz3658);
      KILL(lbits)(&zgsz3657);
    }
  
  }
  uint64_t zdiff;
  {
    lbits zgsz3660;
    CREATE(lbits)(&zgsz3660);
    CONVERT_OF(lbits, fbits)(&zgsz3660, zR, UINT64_C(8) , true);
    lbits zgsz3661;
    CREATE(lbits)(&zgsz3661);
    CONVERT_OF(lbits, fbits)(&zgsz3661, za_mid, UINT64_C(8) , true);
    lbits zgsz3662;
    CREATE(lbits)(&zgsz3662);
    sub_bits(&zgsz3662, zgsz3660, zgsz3661);
    zdiff = CONVERT_OF(fbits, lbits)(zgsz3662, true);
    KILL(lbits)(&zgsz3662);
    KILL(lbits)(&zgsz3661);
    KILL(lbits)(&zgsz3660);
  }
  uint64_t zdiff1;
  {
    lbits zgsz3663;
    CREATE(lbits)(&zgsz3663);
    CONVERT_OF(lbits, fbits)(&zgsz3663, zdiff, UINT64_C(8) , true);
    sail_int zgsz3664;
    CREATE(sail_int)(&zgsz3664);
    CONVERT_OF(sail_int, mach_int)(&zgsz3664, INT64_C(1));
    lbits zgsz3665;
    CREATE(lbits)(&zgsz3665);
    sub_bits_int(&zgsz3665, zgsz3663, zgsz3664);
    zdiff1 = CONVERT_OF(fbits, lbits)(zgsz3665, true);
    KILL(lbits)(&zgsz3665);
    KILL(sail_int)(&zgsz3664);
    KILL(lbits)(&zgsz3663);
  }
  bool zgaz3269;
  {
    sail_int zgsz3667;
    CREATE(sail_int)(&zgsz3667);
    CONVERT_OF(sail_int, mach_int)(&zgsz3667, INT64_C(0));
    sail_int zgsz3666;
    CREATE(sail_int)(&zgsz3666);
    CONVERT_OF(sail_int, mach_int)(&zgsz3666, zi_top);
    zgaz3269 = eq_int(zgsz3666, zgsz3667);
    KILL(sail_int)(&zgsz3667);
    KILL(sail_int)(&zgsz3666);
  }
  if (zgaz3269) {  zcbz349 = sailgen_z8operatorz0zzzz7nzzJzzK8z3zI_uz9(zi_mid, zdiff1);  } else {
  bool zgaz3270;
  {
    sail_int zgsz3669;
    CREATE(sail_int)(&zgsz3669);
    CONVERT_OF(sail_int, mach_int)(&zgsz3669, INT64_C(-1));
    sail_int zgsz3668;
    CREATE(sail_int)(&zgsz3668);
    CONVERT_OF(sail_int, mach_int)(&zgsz3668, zi_top);
    zgaz3270 = eq_int(zgsz3668, zgsz3669);
    KILL(sail_int)(&zgsz3669);
    KILL(sail_int)(&zgsz3668);
  }
  if (zgaz3270) {
  bool zgaz3271;
  zgaz3271 = sailgen_z8operatorz0zzzz7nzzJzzK8z3zKzJ_uz9(zi_mid, zdiff);
  bool zgsz3670;
  if (zgaz3271) {
  {
    lbits zgsz3672;
    CREATE(lbits)(&zgsz3672);
    CONVERT_OF(lbits, fbits)(&zgsz3672, za_mid, UINT64_C(8) , true);
    lbits zgsz3671;
    CREATE(lbits)(&zgsz3671);
    CONVERT_OF(lbits, fbits)(&zgsz3671, zR, UINT64_C(8) , true);
    zgsz3670 = neq_bits(zgsz3671, zgsz3672);
    KILL(lbits)(&zgsz3672);
    KILL(lbits)(&zgsz3671);
  }
  } else {  zgsz3670 = false;  }
  zcbz349 = zgsz3670;

  } else {  zcbz349 = false;  }

  }









  }


end_function_188: ;
  return zcbz349;
end_block_exception_189: ;

  return false;
}

static void sailgen_capToString(sail_string *rop, struct zCapability);

static void sailgen_capToString(sail_string *zcbz350, struct zCapability zcap)
{
  __label__ end_function_191, end_block_exception_192, end_function_203;

  int64_t zlen;
  zlen = sailgen_getCapLength(zcap);
  if (have_exception) {

  goto end_block_exception_192;
  }
  sail_string zlen_str;
  CREATE(sail_string)(&zlen_str);
  {
    uint64_t zgaz3309;
    {
      int64_t zgaz3308;
      {
        sail_int zgsz3673;
        CREATE(sail_int)(&zgsz3673);
        CONVERT_OF(sail_int, mach_int)(&zgsz3673, zcap_len_width);
        sail_int zgsz3674;
        CREATE(sail_int)(&zgsz3674);
        CONVERT_OF(sail_int, mach_int)(&zgsz3674, INT64_C(3));
        sail_int zgsz3675;
        CREATE(sail_int)(&zgsz3675);
        add_int(&zgsz3675, zgsz3673, zgsz3674);
        zgaz3308 = CONVERT_OF(mach_int, sail_int)(zgsz3675);
        KILL(sail_int)(&zgsz3675);
        KILL(sail_int)(&zgsz3674);
        KILL(sail_int)(&zgsz3673);
      }
      {
        sail_int zgsz3676;
        CREATE(sail_int)(&zgsz3676);
        CONVERT_OF(sail_int, mach_int)(&zgsz3676, zlen);
        zgaz3309 = sailgen_zzzz7lzzJzzK36z3to_bits(zgaz3308, zgsz3676);
        KILL(sail_int)(&zgsz3676);
      }
    
    }
    {
      lbits zgsz3677;
      CREATE(lbits)(&zgsz3677);
      CONVERT_OF(lbits, fbits)(&zgsz3677, zgaz3309, UINT64_C(36) , true);
      string_of_lbits(&zlen_str, zgsz3677);
      KILL(lbits)(&zgsz3677);
    }
  
  }
  uint64_t zotype64;
  {
    bool zgaz3305;
    zgaz3305 = sailgen_hasReservedOType(zcap);
    if (zgaz3305) {
    uint64_t zgaz3306;
    zgaz3306 = zcap.zotype;
    zotype64 = sailgen_zzzz7mzzJzzK32zzCzz0zz7nzzJzzK4z3EXTS(INT64_C(32), zgaz3306);
  
    } else {
    uint64_t zgaz3307;
    zgaz3307 = zcap.zotype;
    zotype64 = sailgen_zzzz7mzzJzzK32zzCzz0zz7nzzJzzK4z3EXTZ(INT64_C(32), zgaz3307);
  
    }
  
  }
  sail_string zgaz3304;
  CREATE(sail_string)(&zgaz3304);
  {
    sail_string zgaz3302;
    CREATE(sail_string)(&zgaz3302);
    {
      bool zgaz3281;
      zgaz3281 = zcap.ztag;
      if (zgaz3281) {  COPY(sail_string)(&zgaz3302, "1");  } else {  COPY(sail_string)(&zgaz3302, "0");  }
    
    }
    sail_string zgaz3303;
    CREATE(sail_string)(&zgaz3303);
    {
      sail_string zgaz3301;
      CREATE(sail_string)(&zgaz3301);
      {
        sail_string zgaz3299;
        CREATE(sail_string)(&zgaz3299);
        {
          bool zgaz3282;
          zgaz3282 = zcap.zsealed;
          if (zgaz3282) {  COPY(sail_string)(&zgaz3299, "1");  } else {  COPY(sail_string)(&zgaz3299, "0");  }
        
        }
        sail_string zgaz3300;
        CREATE(sail_string)(&zgaz3300);
        {
          sail_string zgaz3298;
          CREATE(sail_string)(&zgaz3298);
          {
            sail_string zgaz3296;
            CREATE(sail_string)(&zgaz3296);
            {
              uint64_t zgaz3284;
              {
                uint64_t zgaz3283;
                zgaz3283 = sailgen_getCapPerms(zcap);
                {
                  lbits zgsz3678;
                  CREATE(lbits)(&zgsz3678);
                  CONVERT_OF(lbits, fbits)(&zgsz3678, UINT64_C(0b0), UINT64_C(1) , true);
                  lbits zgsz3679;
                  CREATE(lbits)(&zgsz3679);
                  CONVERT_OF(lbits, fbits)(&zgsz3679, zgaz3283, UINT64_C(31) , true);
                  lbits zgsz3680;
                  CREATE(lbits)(&zgsz3680);
                  append(&zgsz3680, zgsz3678, zgsz3679);
                  zgaz3284 = CONVERT_OF(fbits, lbits)(zgsz3680, true);
                  KILL(lbits)(&zgsz3680);
                  KILL(lbits)(&zgsz3679);
                  KILL(lbits)(&zgsz3678);
                }
              
              }
              {
                lbits zgsz3681;
                CREATE(lbits)(&zgsz3681);
                CONVERT_OF(lbits, fbits)(&zgsz3681, zgaz3284, UINT64_C(32) , true);
                string_of_lbits(&zgaz3296, zgsz3681);
                KILL(lbits)(&zgsz3681);
              }
            
            }
            sail_string zgaz3297;
            CREATE(sail_string)(&zgaz3297);
            {
              sail_string zgaz3295;
              CREATE(sail_string)(&zgaz3295);
              {
                sail_string zgaz3293;
                CREATE(sail_string)(&zgaz3293);
                {
                  lbits zgsz3682;
                  CREATE(lbits)(&zgsz3682);
                  CONVERT_OF(lbits, fbits)(&zgsz3682, zotype64, UINT64_C(32) , true);
                  string_of_lbits(&zgaz3293, zgsz3682);
                  KILL(lbits)(&zgsz3682);
                }
                sail_string zgaz3294;
                CREATE(sail_string)(&zgaz3294);
                {
                  sail_string zgaz3292;
                  CREATE(sail_string)(&zgaz3292);
                  {
                    sail_string zgaz3290;
                    CREATE(sail_string)(&zgaz3290);
                    {
                      uint64_t zgaz3285;
                      zgaz3285 = sailgen_getCapOffsetBits(zcap);
                      {
                        lbits zgsz3683;
                        CREATE(lbits)(&zgsz3683);
                        CONVERT_OF(lbits, fbits)(&zgsz3683, zgaz3285, UINT64_C(32) , true);
                        string_of_lbits(&zgaz3290, zgsz3683);
                        KILL(lbits)(&zgsz3683);
                      }
                    
                    }
                    sail_string zgaz3291;
                    CREATE(sail_string)(&zgaz3291);
                    {
                      sail_string zgaz3289;
                      CREATE(sail_string)(&zgaz3289);
                      {
                        sail_string zgaz3287;
                        CREATE(sail_string)(&zgaz3287);
                        {
                          uint64_t zgaz3286;
                          zgaz3286 = sailgen_getCapBaseBits(zcap);
                          {
                            lbits zgsz3684;
                            CREATE(lbits)(&zgsz3684);
                            CONVERT_OF(lbits, fbits)(&zgsz3684, zgaz3286, UINT64_C(32) , true);
                            string_of_lbits(&zgaz3287, zgsz3684);
                            KILL(lbits)(&zgsz3684);
                          }
                        
                        }
                        sail_string zgaz3288;
                        CREATE(sail_string)(&zgaz3288);
                        concat_str(&zgaz3288, " length:", zlen_str);
                        concat_str(&zgaz3289, zgaz3287, zgaz3288);
                        KILL(sail_string)(&zgaz3288);
                        KILL(sail_string)(&zgaz3287);
                      }
                      concat_str(&zgaz3291, " base:", zgaz3289);
                      KILL(sail_string)(&zgaz3289);
                    }
                    concat_str(&zgaz3292, zgaz3290, zgaz3291);
                    KILL(sail_string)(&zgaz3291);
                    KILL(sail_string)(&zgaz3290);
                  }
                  concat_str(&zgaz3294, " offset:", zgaz3292);
                  KILL(sail_string)(&zgaz3292);
                }
                concat_str(&zgaz3295, zgaz3293, zgaz3294);
                KILL(sail_string)(&zgaz3294);
                KILL(sail_string)(&zgaz3293);
              }
              concat_str(&zgaz3297, " type:", zgaz3295);
              KILL(sail_string)(&zgaz3295);
            }
            concat_str(&zgaz3298, zgaz3296, zgaz3297);
            KILL(sail_string)(&zgaz3297);
            KILL(sail_string)(&zgaz3296);
          }
          concat_str(&zgaz3300, " perms:", zgaz3298);
          KILL(sail_string)(&zgaz3298);
        }
        concat_str(&zgaz3301, zgaz3299, zgaz3300);
        KILL(sail_string)(&zgaz3300);
        KILL(sail_string)(&zgaz3299);
      }
      concat_str(&zgaz3303, " s:", zgaz3301);
      KILL(sail_string)(&zgaz3301);
    }
    concat_str(&zgaz3304, zgaz3302, zgaz3303);
    KILL(sail_string)(&zgaz3303);
    KILL(sail_string)(&zgaz3302);
  }
  concat_str((*(&zcbz350)), " t:", zgaz3304);
  KILL(sail_string)(&zgaz3304);

  KILL(sail_string)(&zlen_str);

end_function_191: ;
  goto end_function_203;
end_block_exception_192: ;
  goto end_function_203;
end_function_203: ;
}

// register TestCap
static struct zCapability zTestCap;

// register TestAddr
static uint64_t zTestAddr;

// register TestLen
static uint64_t zTestLen;

static unit sailgen_main(unit);


static unit sailgen_main(unit zgsz3685)
{
  __label__ end_function_198, end_block_exception_199;

  unit zcbz351;
  {
    zTestCap = znull_cap;
    unit zgsz3686;
    zgsz3686 = UNIT;
  }
  uint64_t zbits1;
  zbits1 = sailgen_capToBits(zTestCap);
  uint64_t zbits2;
  zbits2 = sailgen_capToMemBits(zTestCap);
  struct zCapability zcap1;
  {
    bool zgaz3314;
    zgaz3314 = zTestCap.ztag;
    zcap1 = sailgen_capBitsToCapability(zgaz3314, zbits1);
  
  }
  struct zCapability zcap2;
  {
    bool zgaz3313;
    zgaz3313 = zTestCap.ztag;
    zcap2 = sailgen_memBitsToCapability(zgaz3313, zbits2);
  
  }
  bool zrep;
  zrep = sailgen_fastRepCheck(zTestCap, zTestAddr);
  {
    __label__ case_195, finish_match_194;

    struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3310;
    zgaz3310 = sailgen_setCapBounds(zTestCap, zTestAddr, zTestLen);
    unit zgsz3687;
    {
      bool ztup__0;
      ztup__0 = zgaz3310.ztup0;
      struct zCapability ztup__1;
      ztup__1 = zgaz3310.ztup1;
      bool zexact;
      zexact = ztup__0;
      struct zCapability zcap;
      zcap = ztup__1;
      zgsz3687 = UNIT;
    
    
    
    
      goto finish_match_194;
    }
  case_195: ;
    sail_match_failure("main");
  finish_match_194: ;
    unit zgsz3692;
    zgsz3692 = zgsz3687;
  
  
  }
  {
    __label__ case_197, finish_match_196;

    struct ztuple_z8z5i64zCz0z5i64z9 zgaz3311;
    zgaz3311 = sailgen_getCapBounds(zTestCap);
    unit zgsz3689;
    {
      int64_t ztup__0;
      ztup__0 = zgaz3311.ztup0;
      int64_t ztup__1;
      ztup__1 = zgaz3311.ztup1;
      int64_t zbase;
      zbase = ztup__0;
      int64_t ztop;
      ztop = ztup__1;
      zgsz3689 = UNIT;
    
    
    
    
      goto finish_match_196;
    }
  case_197: ;
    sail_match_failure("main");
  finish_match_196: ;
    unit zgsz3691;
    zgsz3691 = zgsz3689;
  
  
  }
  uint64_t zflags;
  zflags = sailgen_getCapFlags(zTestCap);
  struct zCapability znew_cap;
  {
    uint64_t zgaz3312;
    zgaz3312 = sailgen_zzzz7nzzJzzK31z3ones(INT64_C(31));
    znew_cap = sailgen_setCapPerms(zTestCap, zgaz3312);
  
  }
  sail_string zcap_str;
  CREATE(sail_string)(&zcap_str);
  sailgen_capToString(&zcap_str, zTestCap);
  if (have_exception) {







  KILL(sail_string)(&zcap_str);
  goto end_block_exception_199;
  }
  zcbz351 = UNIT;
  KILL(sail_string)(&zcap_str);







end_function_198: ;
  return zcbz351;
end_block_exception_199: ;

  return UNIT;
}

static unit sailgen_initializze_registers(unit);

static unit sailgen_initializze_registers(unit zgsz3693)
{
  __label__ end_function_201, end_block_exception_202;

  unit zcbz352;
  {
    zTestCap = sailgen_undefined_Capability(UNIT);
    unit zgsz3697;
    zgsz3697 = UNIT;
  }
  {
    {
      sail_int zgsz3694;
      CREATE(sail_int)(&zgsz3694);
      CONVERT_OF(sail_int, mach_int)(&zgsz3694, INT64_C(32));
      lbits zgsz3695;
      CREATE(lbits)(&zgsz3695);
      UNDEFINED(lbits)(&zgsz3695, zgsz3694);
      zTestAddr = CONVERT_OF(fbits, lbits)(zgsz3695, true);
      KILL(lbits)(&zgsz3695);
      KILL(sail_int)(&zgsz3694);
    }
    unit zgsz3696;
    zgsz3696 = UNIT;
  }
  int64_t zgaz3315;
  {
    sail_int zgsz3698;
    CREATE(sail_int)(&zgsz3698);
    CONVERT_OF(sail_int, mach_int)(&zgsz3698, INT64_C(32));
    sail_int zgsz3699;
    CREATE(sail_int)(&zgsz3699);
    CONVERT_OF(sail_int, mach_int)(&zgsz3699, INT64_C(1));
    sail_int zgsz3700;
    CREATE(sail_int)(&zgsz3700);
    add_int(&zgsz3700, zgsz3698, zgsz3699);
    zgaz3315 = CONVERT_OF(mach_int, sail_int)(zgsz3700);
    KILL(sail_int)(&zgsz3700);
    KILL(sail_int)(&zgsz3699);
    KILL(sail_int)(&zgsz3698);
  }
  {
    sail_int zgsz3701;
    CREATE(sail_int)(&zgsz3701);
    CONVERT_OF(sail_int, mach_int)(&zgsz3701, zgaz3315);
    lbits zgsz3702;
    CREATE(lbits)(&zgsz3702);
    UNDEFINED(lbits)(&zgsz3702, zgsz3701);
    zTestLen = CONVERT_OF(fbits, lbits)(zgsz3702, true);
    KILL(lbits)(&zgsz3702);
    KILL(sail_int)(&zgsz3701);
  }
  zcbz352 = UNIT;

end_function_201: ;
  return zcbz352;
end_block_exception_202: ;

  return UNIT;
}

static void model_init(void)
{
  setup_rts();
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


