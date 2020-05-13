#include "sail.h"
#include "rts.h"
#include "elf.h"

// enum write_kind
enum zwrite_kind { zWrite_plain, zWrite_conditional, zWrite_release, zWrite_exclusive, zWrite_exclusive_release, zWrite_RISCV_release, zWrite_RISCV_strong_release, zWrite_RISCV_conditional, zWrite_RISCV_conditional_release, zWrite_RISCV_conditional_strong_release, zWrite_X86_locked };

static bool eq_zwrite_kind(enum zwrite_kind op1, enum zwrite_kind op2) { return op1 == op2; }

enum zwrite_kind UNDEFINED(zwrite_kind)(unit u) { return zWrite_plain; }

// enum trans_kind
enum ztrans_kind { zTransaction_start, zTransaction_commit, zTransaction_abort };

static bool eq_ztrans_kind(enum ztrans_kind op1, enum ztrans_kind op2) { return op1 == op2; }

enum ztrans_kind UNDEFINED(ztrans_kind)(unit u) { return zTransaction_start; }

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

enum zread_kind UNDEFINED(zread_kind)(unit u) { return zRead_plain; }

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

enum zcache_op_kind UNDEFINED(zcache_op_kind)(unit u) { return zCache_op_D_IVAC; }

// enum a64_barrier_type
enum za64_barrier_type { zA64_barrier_all, zA64_barrier_LD, zA64_barrier_ST };

static bool eq_za64_barrier_type(enum za64_barrier_type op1, enum za64_barrier_type op2) { return op1 == op2; }

enum za64_barrier_type UNDEFINED(za64_barrier_type)(unit u) { return zA64_barrier_all; }

// enum a64_barrier_domain
enum za64_barrier_domain { zA64_FullShare, zA64_InnerShare, zA64_OuterShare, zA64_NonShare };

static bool eq_za64_barrier_domain(enum za64_barrier_domain op1, enum za64_barrier_domain op2) { return op1 == op2; }

enum za64_barrier_domain UNDEFINED(za64_barrier_domain)(unit u) { return zA64_FullShare; }

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

enum zClearRegSet UNDEFINED(zClearRegSet)(unit u) { return zGPRegs; }

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

enum zCapEx UNDEFINED(zCapEx)(unit u) { return zCapEx_None; }

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

enum zCPtrCmpOp UNDEFINED(zCPtrCmpOp)(unit u) { return zCEQ; }

















bool sailgen_zzz3zzzz7mzzJzzK0z3neq_int(sail_int, int64_t);

bool sailgen_zzz3zzzz7mzzJzzK0z3neq_int(sail_int zx, int64_t zy)
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



















bool sailgen_neq_bits(lbits zx, lbits zy)
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

































uint64_t sailgen_zzzz7nzzJzzK4z3sail_ones(int64_t);

uint64_t sailgen_zzzz7nzzJzzK31z3sail_ones(int64_t);

void sailgen_zzz3zzz3sail_ones(lbits *rop, sail_int);

uint64_t sailgen_zzzz7nzzJzzK4z3sail_ones(int64_t zn)
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

uint64_t sailgen_zzzz7nzzJzzK31z3sail_ones(int64_t zn)
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

void sailgen_zzz3zzz3sail_ones(lbits *zcbz34, sail_int zn)
{
  __label__ end_function_13, end_block_exception_14, end_function_235;

  lbits zgaz34;
  CREATE(lbits)(&zgaz34);
  zeros(&zgaz34, zn);
  not_bits((*(&zcbz34)), zgaz34);
  KILL(lbits)(&zgaz34);
end_function_13: ;
  goto end_function_235;
end_block_exception_14: ;
  goto end_function_235;
end_function_235: ;
}











fbits sailgen_not_bit(fbits);

fbits sailgen_not_bit(fbits zb)
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













uint64_t sailgen_zzzz7mzzJzzK64zzCzz0zz7nzzJzzK18z3EXTS(int64_t, uint64_t);

void sailgen_zzz3zzz3EXTZ(lbits *rop, sail_int, lbits);

uint64_t sailgen_zzzz7mzzJzzK15zzCzz0zz7nzzJzzK12z3EXTZ(int64_t, uint64_t);

uint64_t sailgen_zzz3zzzz7mzzJzzK31z3EXTZ(int64_t, sbits);

uint64_t sailgen_zzzz7mzzJzzK64zzCzz0zz7nzzJzzK18z3EXTZ(int64_t, uint64_t);

uint64_t sailgen_zzzz7mzzJzzK64zzCzz0zz7nzzJzzK18z3EXTS(int64_t zm, uint64_t zv)
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

void sailgen_zzz3zzz3EXTZ(lbits *zcbz37, sail_int zm, lbits zv)
{
  __label__ end_function_22, end_block_exception_23, end_function_234;

  zero_extend((*(&zcbz37)), zv, zm);
end_function_22: ;
  goto end_function_234;
end_block_exception_23: ;
  goto end_function_234;
end_function_234: ;
}

uint64_t sailgen_zzzz7mzzJzzK15zzCzz0zz7nzzJzzK12z3EXTZ(int64_t zm, uint64_t zv)
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

uint64_t sailgen_zzz3zzzz7mzzJzzK31z3EXTZ(int64_t zm, sbits zv)
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

uint64_t sailgen_zzzz7mzzJzzK64zzCzz0zz7nzzJzzK18z3EXTZ(int64_t zm, uint64_t zv)
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

uint64_t sailgen_zzzz7nzzJzzK64z3zzeros_implicit(int64_t);

uint64_t sailgen_zzzz7nzzJzzK14z3zzeros_implicit(int64_t);

uint64_t sailgen_zzzz7nzzJzzK2z3zzeros_implicit(int64_t);

uint64_t sailgen_zzzz7nzzJzzK4z3zzeros_implicit(int64_t);

uint64_t sailgen_zzzz7nzzJzzK6z3zzeros_implicit(int64_t);

uint64_t sailgen_zzzz7nzzJzzK12z3zzeros_implicit(int64_t);

void sailgen_zzz3zzz3zzeros_implicit(lbits *rop, sail_int);

uint64_t sailgen_zzzz7nzzJzzK11z3zzeros_implicit(int64_t);

uint64_t sailgen_zzzz7nzzJzzK64z3zzeros_implicit(int64_t zn)
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

uint64_t sailgen_zzzz7nzzJzzK14z3zzeros_implicit(int64_t zn)
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

uint64_t sailgen_zzzz7nzzJzzK2z3zzeros_implicit(int64_t zn)
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

uint64_t sailgen_zzzz7nzzJzzK4z3zzeros_implicit(int64_t zn)
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

uint64_t sailgen_zzzz7nzzJzzK6z3zzeros_implicit(int64_t zn)
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

uint64_t sailgen_zzzz7nzzJzzK12z3zzeros_implicit(int64_t zn)
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

void sailgen_zzz3zzz3zzeros_implicit(lbits *zcbz317, sail_int zn)
{
  __label__ end_function_52, end_block_exception_53, end_function_233;

  zeros((*(&zcbz317)), zn);
end_function_52: ;
  goto end_function_233;
end_block_exception_53: ;
  goto end_function_233;
end_function_233: ;
}

uint64_t sailgen_zzzz7nzzJzzK11z3zzeros_implicit(int64_t zn)
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

uint64_t sailgen_zzzz7nzzJzzK4z3ones(int64_t);

void sailgen_zzz3zzz3ones(lbits *rop, sail_int);

uint64_t sailgen_zzzz7nzzJzzK31z3ones(int64_t);

uint64_t sailgen_zzzz7nzzJzzK4z3ones(int64_t zn)
{
  __label__ end_function_58, end_block_exception_59;

  uint64_t zcbz319;
  zcbz319 = sailgen_zzzz7nzzJzzK4z3sail_ones(zn);
end_function_58: ;
  return zcbz319;
end_block_exception_59: ;

  return UINT64_C(0xdeadc0de);
}

void sailgen_zzz3zzz3ones(lbits *zcbz320, sail_int zn)
{
  __label__ end_function_61, end_block_exception_62, end_function_232;

  sailgen_zzz3zzz3sail_ones((*(&zcbz320)), zn);
end_function_61: ;
  goto end_function_232;
end_block_exception_62: ;
  goto end_function_232;
end_function_232: ;
}

uint64_t sailgen_zzzz7nzzJzzK31z3ones(int64_t zn)
{
  __label__ end_function_64, end_block_exception_65;

  uint64_t zcbz321;
  zcbz321 = sailgen_zzzz7nzzJzzK31z3sail_ones(zn);
end_function_64: ;
  return zcbz321;
end_block_exception_65: ;

  return UINT64_C(0xdeadc0de);
}

uint64_t sailgen_bool_to_bits(bool);

uint64_t sailgen_bool_to_bits(bool zx)
{
  __label__ end_function_67, end_block_exception_68;

  uint64_t zcbz322;
  if (zx) {  zcbz322 = UINT64_C(0b1);  } else {  zcbz322 = UINT64_C(0b0);  }
end_function_67: ;
  return zcbz322;
end_block_exception_68: ;

  return UINT64_C(0xdeadc0de);
}

bool sailgen_bit_to_bool(fbits);

bool sailgen_bit_to_bool(fbits zb)
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

void sailgen_to_bits(lbits *rop, sail_int, sail_int);

uint64_t sailgen_zzzz7lzzJzzK6z3to_bits(int64_t, sail_int);

void sailgen_zzzz7lzzJzzK68z3to_bits(lbits *rop, int64_t, sail_int);

void sailgen_to_bits(lbits *zcbz324, sail_int zl, sail_int zn)
{
  __label__ end_function_76, end_block_exception_77, end_function_231;

  {
    sail_int zgsz338;
    CREATE(sail_int)(&zgsz338);
    CONVERT_OF(sail_int, mach_int)(&zgsz338, 0l);
    get_slice_int((*(&zcbz324)), zl, zn, zgsz338);
    KILL(sail_int)(&zgsz338);
  }
end_function_76: ;
  goto end_function_231;
end_block_exception_77: ;
  goto end_function_231;
end_function_231: ;
}

uint64_t sailgen_zzzz7lzzJzzK6z3to_bits(int64_t zl, sail_int zn)
{
  __label__ end_function_79, end_block_exception_80;

  uint64_t zcbz325;
  {
    sail_int zgsz339;
    CREATE(sail_int)(&zgsz339);
    CONVERT_OF(sail_int, mach_int)(&zgsz339, zl);
    sail_int zgsz340;
    CREATE(sail_int)(&zgsz340);
    CONVERT_OF(sail_int, mach_int)(&zgsz340, 0l);
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

void sailgen_zzzz7lzzJzzK68z3to_bits(lbits *zcbz326, int64_t zl, sail_int zn)
{
  __label__ end_function_82, end_block_exception_83, end_function_230;

  {
    sail_int zgsz343;
    CREATE(sail_int)(&zgsz343);
    CONVERT_OF(sail_int, mach_int)(&zgsz343, 0l);
    sail_int zgsz342;
    CREATE(sail_int)(&zgsz342);
    CONVERT_OF(sail_int, mach_int)(&zgsz342, zl);
    get_slice_int((*(&zcbz326)), zgsz342, zn, zgsz343);
    KILL(sail_int)(&zgsz343);
    KILL(sail_int)(&zgsz342);
  }
end_function_82: ;
  goto end_function_230;
end_block_exception_83: ;
  goto end_function_230;
end_function_230: ;
}

bool sailgen_z8operatorz0zzzz7nzzJzzK12z3zI_uz9(uint64_t, uint64_t);

bool sailgen_z8operatorz0zzzz7nzzJzzK3z3zI_uz9(uint64_t, uint64_t);

bool sailgen_z8operatorz0zzzz7nzzJzzK14z3zI_uz9(uint64_t, uint64_t);

bool sailgen_z8operatorz0zzzz7nzzJzzK14z3zKzJ_uz9(uint64_t, uint64_t);

bool sailgen_z8operatorz0zzzz7nzzJzzK12z3zI_uz9(uint64_t zx, uint64_t zy)
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

bool sailgen_z8operatorz0zzzz7nzzJzzK3z3zI_uz9(uint64_t zx, uint64_t zy)
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

bool sailgen_z8operatorz0zzzz7nzzJzzK14z3zI_uz9(uint64_t zx, uint64_t zy)
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

bool sailgen_z8operatorz0zzzz7nzzJzzK14z3zKzJ_uz9(uint64_t zx, uint64_t zy)
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



void sailgen_n_leading_spaces(sail_int *rop, sail_string);

void sailgen_n_leading_spaces(sail_int *zcbz331, sail_string zs)
{
  __label__ case_98, case_99, finish_match_97, end_function_103, end_block_exception_104, end_function_229;

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
    CONVERT_OF(sail_int, mach_int)(&zgsz352, 0l);
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
      CONVERT_OF(sail_int, mach_int)(&zgsz354, 1l);
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
          CONVERT_OF(sail_int, mach_int)(&zgsz356, 1l);
          string_drop(&zgaz315, zs, zgsz356);
          KILL(sail_int)(&zgsz356);
        }
        sailgen_n_leading_spaces(&zgaz316, zgaz315);
        KILL(sail_string)(&zgaz315);
      }
      {
        sail_int zgsz358;
        CREATE(sail_int)(&zgsz358);
        CONVERT_OF(sail_int, mach_int)(&zgsz358, 1l);
        add_int(&zgsz355, zgsz358, zgaz316);
        KILL(sail_int)(&zgsz358);
      }
      KILL(sail_int)(&zgaz316);
      KILL(sail_string)(&zp0z3);
      goto finish_match_100;
    }
  case_101: ;
    {
      CONVERT_OF(sail_int, mach_int)(&zgsz355, 0l);
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
  goto end_function_229;
end_block_exception_104: ;
  goto end_function_229;
end_function_229: ;
}

void sailgen_zzzz7nzzJzzK64z3MAX(sail_int *rop, int64_t);

int64_t sailgen_zzzz7nzzJzzK18z3MAX(int64_t);

void sailgen_zzzz7nzzJzzK64z3MAX(sail_int *zcbz332, int64_t zn)
{
  __label__ end_function_106, end_block_exception_107, end_function_228;

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
    CONVERT_OF(sail_int, mach_int)(&zgsz362, 1l);
    sub_int((*(&zcbz332)), zgaz317, zgsz362);
    KILL(sail_int)(&zgsz362);
  }
  KILL(sail_int)(&zgaz317);
end_function_106: ;
  goto end_function_228;
end_block_exception_107: ;
  goto end_function_228;
end_function_228: ;
}

int64_t sailgen_zzzz7nzzJzzK18z3MAX(int64_t zn)
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
    CONVERT_OF(sail_int, mach_int)(&zgsz366, 1l);
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



int64_t znum_flags;
static void create_letbind_0(void) {  


  int64_t zgsz368;
  zgsz368 = 1l;
  znum_flags = zgsz368;

let_end_111: ;
}
static void kill_letbind_0(void) {  
}

int64_t zreserved_otypes;
static void create_letbind_1(void) {  


  int64_t zgsz369;
  zgsz369 = 4l;
  zreserved_otypes = zgsz369;

let_end_112: ;
}
static void kill_letbind_1(void) {  
}

int64_t zotype_unsealed;
static void create_letbind_2(void) {  


  int64_t zgsz370;
  zgsz370 = -1l;
  zotype_unsealed = zgsz370;

let_end_113: ;
}
static void kill_letbind_2(void) {  
}

uint64_t zPCC_IDX;
static void create_letbind_3(void) {  


  uint64_t zgsz371;
  zgsz371 = UINT64_C(0b100000);
  zPCC_IDX = zgsz371;

let_end_114: ;
}
static void kill_letbind_3(void) {  
}

uint64_t zDDC_IDX;
static void create_letbind_4(void) {  


  uint64_t zgsz372;
  zgsz372 = UINT64_C(0b100001);
  zDDC_IDX = zgsz372;

let_end_115: ;
}
static void kill_letbind_4(void) {  
}

int64_t zcap_sizze;
static void create_letbind_5(void) {  


  int64_t zgsz373;
  zgsz373 = 16l;
  zcap_sizze = zgsz373;

let_end_116: ;
}
static void kill_letbind_5(void) {  
}

int64_t zlog2_cap_sizze;
static void create_letbind_6(void) {  


  int64_t zgsz374;
  zgsz374 = 4l;
  zlog2_cap_sizze = zgsz374;

let_end_117: ;
}
static void kill_letbind_6(void) {  
}

int64_t zotype_width;
static void create_letbind_7(void) {  


  int64_t zgsz375;
  zgsz375 = 18l;
  zotype_width = zgsz375;

let_end_118: ;
}
static void kill_letbind_7(void) {  
}

int64_t zuperms_width;
static void create_letbind_8(void) {  


  int64_t zgsz376;
  zgsz376 = 4l;
  zuperms_width = zgsz376;

let_end_119: ;
}
static void kill_letbind_8(void) {  
}

int64_t zcap_addr_width;
static void create_letbind_9(void) {  


  int64_t zgsz377;
  zgsz377 = 64l;
  zcap_addr_width = zgsz377;

let_end_120: ;
}
static void kill_letbind_9(void) {  
}

int64_t zcap_len_width;
static void create_letbind_10(void) {  


  int64_t zgsz378;
  {
    sail_int zgsz379;
    CREATE(sail_int)(&zgsz379);
    CONVERT_OF(sail_int, mach_int)(&zgsz379, 64l);
    sail_int zgsz380;
    CREATE(sail_int)(&zgsz380);
    CONVERT_OF(sail_int, mach_int)(&zgsz380, 1l);
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

int64_t zmantissa_width;
static void create_letbind_11(void) {  


  int64_t zgsz382;
  zgsz382 = 14l;
  zmantissa_width = zgsz382;

let_end_122: ;
}
static void kill_letbind_11(void) {  
}

sail_int zMAX_ADDR;
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

struct zCapability sailgen_undefined_Capability(unit);

struct zCapability sailgen_undefined_Capability(unit zgsz384)
{
  __label__ end_function_125, end_block_exception_126;

  struct zCapability zcbz334;
  bool zgaz319;
  zgaz319 = undefined_bool(UNIT);
  uint64_t zgaz320;
  {
    sail_int zgsz385;
    CREATE(sail_int)(&zgsz385);
    CONVERT_OF(sail_int, mach_int)(&zgsz385, 4l);
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
    CONVERT_OF(sail_int, mach_int)(&zgsz387, 2l);
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
    CONVERT_OF(sail_int, mach_int)(&zgsz389, 6l);
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
    CONVERT_OF(sail_int, mach_int)(&zgsz391, 14l);
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
    CONVERT_OF(sail_int, mach_int)(&zgsz393, 14l);
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
    CONVERT_OF(sail_int, mach_int)(&zgsz395, 18l);
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
    CONVERT_OF(sail_int, mach_int)(&zgsz397, 64l);
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

int64_t zmaxE;
static void create_letbind_13(void) {  


  int64_t zgsz3100;
  zgsz3100 = 52l;
  zmaxE = zgsz3100;

let_end_127: ;
}
static void kill_letbind_13(void) {  
}

int64_t zinternal_exponent_take_bits;
static void create_letbind_14(void) {  


  int64_t zgsz3101;
  zgsz3101 = 3l;
  zinternal_exponent_take_bits = zgsz3101;

let_end_128: ;
}
static void kill_letbind_14(void) {  
}

uint64_t zresetE;
static void create_letbind_15(void) {  


  uint64_t zgsz3102;
  {
    sail_int zgsz3103;
    CREATE(sail_int)(&zgsz3103);
    CONVERT_OF(sail_int, mach_int)(&zgsz3103, 6l);
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

uint64_t zresetT;
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

struct zCapability znull_cap;
static void create_letbind_17(void) {  


  struct zCapability zgsz3114;
  uint64_t zgaz342;
  zgaz342 = sailgen_zzzz7nzzJzzK4z3zzeros_implicit(4l);
  uint64_t zgaz343;
  zgaz343 = sailgen_zzzz7nzzJzzK2z3zzeros_implicit(2l);
  uint64_t zgaz344;
  zgaz344 = sailgen_zzzz7nzzJzzK14z3zzeros_implicit(14l);
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
  zgaz346 = sailgen_zzzz7nzzJzzK64z3zzeros_implicit(64l);
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

struct zCapability zdefault_cap;
static void create_letbind_18(void) {  


  struct zCapability zgsz3119;
  uint64_t zgaz347;
  zgaz347 = sailgen_zzzz7nzzJzzK4z3ones(4l);
  uint64_t zgaz348;
  zgaz348 = sailgen_zzzz7nzzJzzK2z3zzeros_implicit(2l);
  uint64_t zgaz349;
  zgaz349 = sailgen_zzzz7nzzJzzK14z3zzeros_implicit(14l);
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
  zgaz351 = sailgen_zzzz7nzzJzzK64z3zzeros_implicit(64l);
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

struct zCapability sailgen_capBitsToCapability(bool, lbits);

struct zCapability sailgen_capBitsToCapability(bool zt, lbits zc)
{
  __label__ case_135, finish_match_134, end_function_136, end_block_exception_139;

  struct zCapability zcbz335;
  bool zinternal_exponent;
  {
    fbits zgaz393;
    {
      sail_int zgsz3120;
      CREATE(sail_int)(&zgsz3120);
      CONVERT_OF(sail_int, mach_int)(&zgsz3120, 90l);
      zgaz393 = bitvector_access(zc, zgsz3120);
      KILL(sail_int)(&zgsz3120);
    }
    zinternal_exponent = sailgen_bit_to_bool(zgaz393);
  
  }
  uint64_t zotype;
  {
    sail_int zgsz3121;
    CREATE(sail_int)(&zgsz3121);
    CONVERT_OF(sail_int, mach_int)(&zgsz3121, 108l);
    sail_int zgsz3122;
    CREATE(sail_int)(&zgsz3122);
    CONVERT_OF(sail_int, mach_int)(&zgsz3122, 91l);
    lbits zgsz3123;
    CREATE(lbits)(&zgsz3123);
    vector_subrange_lbits(&zgsz3123, zc, zgsz3121, zgsz3122);
    zotype = CONVERT_OF(fbits, lbits)(zgsz3123, true);
    KILL(lbits)(&zgsz3123);
    KILL(sail_int)(&zgsz3122);
    KILL(sail_int)(&zgsz3121);
  }
  bool zgaz353;
  {
    uint64_t zgaz352;
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
      zgaz352 = CONVERT_OF(fbits, lbits)(zgsz3126, true);
      KILL(lbits)(&zgsz3126);
      KILL(sail_int)(&zgsz3125);
      KILL(sail_int)(&zgsz3124);
    }
    {
      lbits zgsz3128;
      CREATE(lbits)(&zgsz3128);
      CONVERT_OF(lbits, fbits)(&zgsz3128, zgaz352, UINT64_C(18) , true);
      lbits zgsz3127;
      CREATE(lbits)(&zgsz3127);
      CONVERT_OF(lbits, fbits)(&zgsz3127, zotype, UINT64_C(18) , true);
      zgaz353 = neq_bits(zgsz3127, zgsz3128);
      KILL(lbits)(&zgsz3128);
      KILL(lbits)(&zgsz3127);
    }
  
  }
  struct zCapability zgsz3129;
  {
    __label__ cleanup_137, end_cleanup_138;

    bool zsealed;
    zsealed = zgaz353;
    uint64_t zE;
    zE = sailgen_zzzz7nzzJzzK6z3zzeros_implicit(6l);
    uint64_t zBs;
    zBs = sailgen_zzzz7nzzJzzK14z3zzeros_implicit(14l);
    uint64_t zT;
    zT = sailgen_zzzz7nzzJzzK12z3zzeros_implicit(12l);
    uint64_t zlenMSBs;
    zlenMSBs = sailgen_zzzz7nzzJzzK2z3zzeros_implicit(2l);
    {
      unit zgsz3130;
      if (zinternal_exponent) {
      {
        uint64_t zgaz354;
        {
          sail_int zgsz3139;
          CREATE(sail_int)(&zgsz3139);
          CONVERT_OF(sail_int, mach_int)(&zgsz3139, 80l);
          sail_int zgsz3140;
          CREATE(sail_int)(&zgsz3140);
          CONVERT_OF(sail_int, mach_int)(&zgsz3140, 78l);
          lbits zgsz3141;
          CREATE(lbits)(&zgsz3141);
          vector_subrange_lbits(&zgsz3141, zc, zgsz3139, zgsz3140);
          zgaz354 = CONVERT_OF(fbits, lbits)(zgsz3141, true);
          KILL(lbits)(&zgsz3141);
          KILL(sail_int)(&zgsz3140);
          KILL(sail_int)(&zgsz3139);
        }
        uint64_t zgaz355;
        {
          sail_int zgsz3142;
          CREATE(sail_int)(&zgsz3142);
          CONVERT_OF(sail_int, mach_int)(&zgsz3142, 66l);
          sail_int zgsz3143;
          CREATE(sail_int)(&zgsz3143);
          CONVERT_OF(sail_int, mach_int)(&zgsz3143, 64l);
          lbits zgsz3144;
          CREATE(lbits)(&zgsz3144);
          vector_subrange_lbits(&zgsz3144, zc, zgsz3142, zgsz3143);
          zgaz355 = CONVERT_OF(fbits, lbits)(zgsz3144, true);
          KILL(lbits)(&zgsz3144);
          KILL(sail_int)(&zgsz3143);
          KILL(sail_int)(&zgsz3142);
        }
        {
          lbits zgsz3145;
          CREATE(lbits)(&zgsz3145);
          CONVERT_OF(lbits, fbits)(&zgsz3145, zgaz354, UINT64_C(3) , true);
          lbits zgsz3146;
          CREATE(lbits)(&zgsz3146);
          CONVERT_OF(lbits, fbits)(&zgsz3146, zgaz355, UINT64_C(3) , true);
          lbits zgsz3147;
          CREATE(lbits)(&zgsz3147);
          append(&zgsz3147, zgsz3145, zgsz3146);
          zE = CONVERT_OF(fbits, lbits)(zgsz3147, true);
          KILL(lbits)(&zgsz3147);
          KILL(lbits)(&zgsz3146);
          KILL(lbits)(&zgsz3145);
        }
        unit zgsz3156;
        zgsz3156 = UNIT;
      
      
      }
      {
        zlenMSBs = UINT64_C(0b01);
        unit zgsz3155;
        zgsz3155 = UNIT;
      }
      {
        uint64_t zgaz356;
        {
          sail_int zgsz3148;
          CREATE(sail_int)(&zgsz3148);
          CONVERT_OF(sail_int, mach_int)(&zgsz3148, 89l);
          sail_int zgsz3149;
          CREATE(sail_int)(&zgsz3149);
          CONVERT_OF(sail_int, mach_int)(&zgsz3149, 81l);
          lbits zgsz3150;
          CREATE(lbits)(&zgsz3150);
          vector_subrange_lbits(&zgsz3150, zc, zgsz3148, zgsz3149);
          zgaz356 = CONVERT_OF(fbits, lbits)(zgsz3150, true);
          KILL(lbits)(&zgsz3150);
          KILL(sail_int)(&zgsz3149);
          KILL(sail_int)(&zgsz3148);
        }
        {
          lbits zgsz3151;
          CREATE(lbits)(&zgsz3151);
          CONVERT_OF(lbits, fbits)(&zgsz3151, zgaz356, UINT64_C(9) , true);
          lbits zgsz3152;
          CREATE(lbits)(&zgsz3152);
          CONVERT_OF(lbits, fbits)(&zgsz3152, UINT64_C(0b000), UINT64_C(3) , true);
          lbits zgsz3153;
          CREATE(lbits)(&zgsz3153);
          append(&zgsz3153, zgsz3151, zgsz3152);
          zT = CONVERT_OF(fbits, lbits)(zgsz3153, true);
          KILL(lbits)(&zgsz3153);
          KILL(lbits)(&zgsz3152);
          KILL(lbits)(&zgsz3151);
        }
        unit zgsz3154;
        zgsz3154 = UNIT;
      
      }
      uint64_t zgaz357;
      {
        sail_int zgsz3157;
        CREATE(sail_int)(&zgsz3157);
        CONVERT_OF(sail_int, mach_int)(&zgsz3157, 77l);
        sail_int zgsz3158;
        CREATE(sail_int)(&zgsz3158);
        CONVERT_OF(sail_int, mach_int)(&zgsz3158, 67l);
        lbits zgsz3159;
        CREATE(lbits)(&zgsz3159);
        vector_subrange_lbits(&zgsz3159, zc, zgsz3157, zgsz3158);
        zgaz357 = CONVERT_OF(fbits, lbits)(zgsz3159, true);
        KILL(lbits)(&zgsz3159);
        KILL(sail_int)(&zgsz3158);
        KILL(sail_int)(&zgsz3157);
      }
      {
        lbits zgsz3160;
        CREATE(lbits)(&zgsz3160);
        CONVERT_OF(lbits, fbits)(&zgsz3160, zgaz357, UINT64_C(11) , true);
        lbits zgsz3161;
        CREATE(lbits)(&zgsz3161);
        CONVERT_OF(lbits, fbits)(&zgsz3161, UINT64_C(0b000), UINT64_C(3) , true);
        lbits zgsz3162;
        CREATE(lbits)(&zgsz3162);
        append(&zgsz3162, zgsz3160, zgsz3161);
        zBs = CONVERT_OF(fbits, lbits)(zgsz3162, true);
        KILL(lbits)(&zgsz3162);
        KILL(lbits)(&zgsz3161);
        KILL(lbits)(&zgsz3160);
      }
      zgsz3130 = UNIT;
    
      } else {
      {
        zlenMSBs = UINT64_C(0b00);
        unit zgsz3135;
        zgsz3135 = UNIT;
      }
      {
        {
          sail_int zgsz3131;
          CREATE(sail_int)(&zgsz3131);
          CONVERT_OF(sail_int, mach_int)(&zgsz3131, 89l);
          sail_int zgsz3132;
          CREATE(sail_int)(&zgsz3132);
          CONVERT_OF(sail_int, mach_int)(&zgsz3132, 78l);
          lbits zgsz3133;
          CREATE(lbits)(&zgsz3133);
          vector_subrange_lbits(&zgsz3133, zc, zgsz3131, zgsz3132);
          zT = CONVERT_OF(fbits, lbits)(zgsz3133, true);
          KILL(lbits)(&zgsz3133);
          KILL(sail_int)(&zgsz3132);
          KILL(sail_int)(&zgsz3131);
        }
        unit zgsz3134;
        zgsz3134 = UNIT;
      }
      {
        sail_int zgsz3136;
        CREATE(sail_int)(&zgsz3136);
        CONVERT_OF(sail_int, mach_int)(&zgsz3136, 77l);
        sail_int zgsz3137;
        CREATE(sail_int)(&zgsz3137);
        CONVERT_OF(sail_int, mach_int)(&zgsz3137, 64l);
        lbits zgsz3138;
        CREATE(lbits)(&zgsz3138);
        vector_subrange_lbits(&zgsz3138, zc, zgsz3136, zgsz3137);
        zBs = CONVERT_OF(fbits, lbits)(zgsz3138, true);
        KILL(lbits)(&zgsz3138);
        KILL(sail_int)(&zgsz3137);
        KILL(sail_int)(&zgsz3136);
      }
      zgsz3130 = UNIT;
      }
    }
    uint64_t zcarry_out;
    {
      bool zgaz392;
      {
        uint64_t zgaz391;
        {
          lbits zgsz3163;
          CREATE(lbits)(&zgsz3163);
          CONVERT_OF(lbits, fbits)(&zgsz3163, zBs, UINT64_C(14) , true);
          sail_int zgsz3164;
          CREATE(sail_int)(&zgsz3164);
          CONVERT_OF(sail_int, mach_int)(&zgsz3164, 11l);
          sail_int zgsz3165;
          CREATE(sail_int)(&zgsz3165);
          CONVERT_OF(sail_int, mach_int)(&zgsz3165, 0l);
          lbits zgsz3166;
          CREATE(lbits)(&zgsz3166);
          vector_subrange_lbits(&zgsz3166, zgsz3163, zgsz3164, zgsz3165);
          zgaz391 = CONVERT_OF(fbits, lbits)(zgsz3166, true);
          KILL(lbits)(&zgsz3166);
          KILL(sail_int)(&zgsz3165);
          KILL(sail_int)(&zgsz3164);
          KILL(lbits)(&zgsz3163);
        }
        zgaz392 = sailgen_z8operatorz0zzzz7nzzJzzK12z3zI_uz9(zT, zgaz391);
      
      }
      if (zgaz392) {  zcarry_out = UINT64_C(0b01);  } else {  zcarry_out = UINT64_C(0b00);  }
    
    }
    uint64_t zTtop2;
    {
      uint64_t zgaz390;
      {
        uint64_t zgaz389;
        {
          lbits zgsz3167;
          CREATE(lbits)(&zgsz3167);
          CONVERT_OF(lbits, fbits)(&zgsz3167, zBs, UINT64_C(14) , true);
          sail_int zgsz3168;
          CREATE(sail_int)(&zgsz3168);
          CONVERT_OF(sail_int, mach_int)(&zgsz3168, 13l);
          sail_int zgsz3169;
          CREATE(sail_int)(&zgsz3169);
          CONVERT_OF(sail_int, mach_int)(&zgsz3169, 12l);
          lbits zgsz3170;
          CREATE(lbits)(&zgsz3170);
          vector_subrange_lbits(&zgsz3170, zgsz3167, zgsz3168, zgsz3169);
          zgaz389 = CONVERT_OF(fbits, lbits)(zgsz3170, true);
          KILL(lbits)(&zgsz3170);
          KILL(sail_int)(&zgsz3169);
          KILL(sail_int)(&zgsz3168);
          KILL(lbits)(&zgsz3167);
        }
        {
          lbits zgsz3171;
          CREATE(lbits)(&zgsz3171);
          CONVERT_OF(lbits, fbits)(&zgsz3171, zgaz389, UINT64_C(2) , true);
          lbits zgsz3172;
          CREATE(lbits)(&zgsz3172);
          CONVERT_OF(lbits, fbits)(&zgsz3172, zlenMSBs, UINT64_C(2) , true);
          lbits zgsz3173;
          CREATE(lbits)(&zgsz3173);
          add_bits(&zgsz3173, zgsz3171, zgsz3172);
          zgaz390 = CONVERT_OF(fbits, lbits)(zgsz3173, true);
          KILL(lbits)(&zgsz3173);
          KILL(lbits)(&zgsz3172);
          KILL(lbits)(&zgsz3171);
        }
      
      }
      {
        lbits zgsz3174;
        CREATE(lbits)(&zgsz3174);
        CONVERT_OF(lbits, fbits)(&zgsz3174, zgaz390, UINT64_C(2) , true);
        lbits zgsz3175;
        CREATE(lbits)(&zgsz3175);
        CONVERT_OF(lbits, fbits)(&zgsz3175, zcarry_out, UINT64_C(2) , true);
        lbits zgsz3176;
        CREATE(lbits)(&zgsz3176);
        add_bits(&zgsz3176, zgsz3174, zgsz3175);
        zTtop2 = CONVERT_OF(fbits, lbits)(zgsz3176, true);
        KILL(lbits)(&zgsz3176);
        KILL(lbits)(&zgsz3175);
        KILL(lbits)(&zgsz3174);
      }
    
    }
    struct zCapability zgaz388;
    {
      uint64_t zgaz358;
      {
        sail_int zgsz3177;
        CREATE(sail_int)(&zgsz3177);
        CONVERT_OF(sail_int, mach_int)(&zgsz3177, 127l);
        sail_int zgsz3178;
        CREATE(sail_int)(&zgsz3178);
        CONVERT_OF(sail_int, mach_int)(&zgsz3178, 124l);
        lbits zgsz3179;
        CREATE(lbits)(&zgsz3179);
        vector_subrange_lbits(&zgsz3179, zc, zgsz3177, zgsz3178);
        zgaz358 = CONVERT_OF(fbits, lbits)(zgsz3179, true);
        KILL(lbits)(&zgsz3179);
        KILL(sail_int)(&zgsz3178);
        KILL(sail_int)(&zgsz3177);
      }
      bool zgaz360;
      {
        fbits zgaz359;
        {
          sail_int zgsz3180;
          CREATE(sail_int)(&zgsz3180);
          CONVERT_OF(sail_int, mach_int)(&zgsz3180, 123l);
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
          CONVERT_OF(sail_int, mach_int)(&zgsz3181, 122l);
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
          CONVERT_OF(sail_int, mach_int)(&zgsz3182, 121l);
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
          CONVERT_OF(sail_int, mach_int)(&zgsz3183, 120l);
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
          CONVERT_OF(sail_int, mach_int)(&zgsz3184, 119l);
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
          CONVERT_OF(sail_int, mach_int)(&zgsz3185, 118l);
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
          CONVERT_OF(sail_int, mach_int)(&zgsz3186, 117l);
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
          CONVERT_OF(sail_int, mach_int)(&zgsz3187, 116l);
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
          CONVERT_OF(sail_int, mach_int)(&zgsz3188, 115l);
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
          CONVERT_OF(sail_int, mach_int)(&zgsz3189, 114l);
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
          CONVERT_OF(sail_int, mach_int)(&zgsz3190, 113l);
          zgaz379 = bitvector_access(zc, zgsz3190);
          KILL(sail_int)(&zgsz3190);
        }
        zgaz380 = sailgen_bit_to_bool(zgaz379);
      
      }
      bool zgaz382;
      {
        fbits zgaz381;
        {
          sail_int zgsz3191;
          CREATE(sail_int)(&zgsz3191);
          CONVERT_OF(sail_int, mach_int)(&zgsz3191, 112l);
          zgaz381 = bitvector_access(zc, zgsz3191);
          KILL(sail_int)(&zgsz3191);
        }
        zgaz382 = sailgen_bit_to_bool(zgaz381);
      
      }
      uint64_t zgaz383;
      {
        sail_int zgsz3192;
        CREATE(sail_int)(&zgsz3192);
        CONVERT_OF(sail_int, mach_int)(&zgsz3192, 111l);
        sail_int zgsz3193;
        CREATE(sail_int)(&zgsz3193);
        CONVERT_OF(sail_int, mach_int)(&zgsz3193, 110l);
        lbits zgsz3194;
        CREATE(lbits)(&zgsz3194);
        vector_subrange_lbits(&zgsz3194, zc, zgsz3192, zgsz3193);
        zgaz383 = CONVERT_OF(fbits, lbits)(zgsz3194, true);
        KILL(lbits)(&zgsz3194);
        KILL(sail_int)(&zgsz3193);
        KILL(sail_int)(&zgsz3192);
      }
      bool zgaz385;
      {
        fbits zgaz384;
        {
          sail_int zgsz3195;
          CREATE(sail_int)(&zgsz3195);
          CONVERT_OF(sail_int, mach_int)(&zgsz3195, 109l);
          zgaz384 = bitvector_access(zc, zgsz3195);
          KILL(sail_int)(&zgsz3195);
        }
        zgaz385 = sailgen_bit_to_bool(zgaz384);
      
      }
      uint64_t zgaz386;
      {
        lbits zgsz3196;
        CREATE(lbits)(&zgsz3196);
        CONVERT_OF(lbits, fbits)(&zgsz3196, zTtop2, UINT64_C(2) , true);
        lbits zgsz3197;
        CREATE(lbits)(&zgsz3197);
        CONVERT_OF(lbits, fbits)(&zgsz3197, zT, UINT64_C(12) , true);
        lbits zgsz3198;
        CREATE(lbits)(&zgsz3198);
        append(&zgsz3198, zgsz3196, zgsz3197);
        zgaz386 = CONVERT_OF(fbits, lbits)(zgsz3198, true);
        KILL(lbits)(&zgsz3198);
        KILL(lbits)(&zgsz3197);
        KILL(lbits)(&zgsz3196);
      }
      uint64_t zgaz387;
      {
        sail_int zgsz3199;
        CREATE(sail_int)(&zgsz3199);
        CONVERT_OF(sail_int, mach_int)(&zgsz3199, 63l);
        sail_int zgsz3200;
        CREATE(sail_int)(&zgsz3200);
        CONVERT_OF(sail_int, mach_int)(&zgsz3200, 0l);
        lbits zgsz3201;
        CREATE(lbits)(&zgsz3201);
        vector_subrange_lbits(&zgsz3201, zc, zgsz3199, zgsz3200);
        zgaz387 = CONVERT_OF(fbits, lbits)(zgsz3201, true);
        KILL(lbits)(&zgsz3201);
        KILL(sail_int)(&zgsz3200);
        KILL(sail_int)(&zgsz3199);
      }
      struct zCapability zgsz3202;
      zgsz3202.zB = zBs;
      zgsz3202.zE = zE;
      zgsz3202.zT = zgaz386;
      zgsz3202.zaccess_system_regs = zgaz362;
      zgsz3202.zaddress = zgaz387;
      zgsz3202.zflag_cap_mode = zgaz385;
      zgsz3202.zglobal = zgaz382;
      zgsz3202.zinternal_e = zinternal_exponent;
      zgsz3202.zotype = zotype;
      zgsz3202.zpermit_ccall = zgaz366;
      zgsz3202.zpermit_execute = zgaz380;
      zgsz3202.zpermit_load = zgaz378;
      zgsz3202.zpermit_load_cap = zgaz374;
      zgsz3202.zpermit_seal = zgaz368;
      zgsz3202.zpermit_set_CID = zgaz360;
      zgsz3202.zpermit_store = zgaz376;
      zgsz3202.zpermit_store_cap = zgaz372;
      zgsz3202.zpermit_store_local_cap = zgaz370;
      zgsz3202.zpermit_unseal = zgaz364;
      zgsz3202.zreserved = zgaz383;
      zgsz3202.zsealed = zsealed;
      zgsz3202.ztag = zt;
      zgsz3202.zuperms = zgaz358;
      zgaz388 = zgsz3202;
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    
    }
    zcbz335 = zgaz388;
    goto cleanup_137;
    /* unreachable after return */
  
  
  
  
  
  
  
  
    goto finish_match_134;
    goto end_cleanup_138;
  cleanup_137: ;
  
  
  
  
  
  
  
  
  
  
  
  
    goto end_function_136;
  end_cleanup_138: ;
  }
case_135: ;
  sail_match_failure("capBitsToCapability");
finish_match_134: ;
  zcbz335 = zgsz3129;




end_function_136: ;
  return zcbz335;
end_block_exception_139: ;
  struct zCapability zcbz355 = { .zuperms = UINT64_C(0xdeadc0de), .ztag = false, .zsealed = false, .zreserved = UINT64_C(0xdeadc0de), .zpermit_unseal = false, .zpermit_store_local_cap = false, .zpermit_store_cap = false, .zpermit_store = false, .zpermit_set_CID = false, .zpermit_seal = false, .zpermit_load_cap = false, .zpermit_load = false, .zpermit_execute = false, .zpermit_ccall = false, .zotype = UINT64_C(0xdeadc0de), .zinternal_e = false, .zglobal = false, .zflag_cap_mode = false, .zaddress = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zT = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  return zcbz355;
}

int64_t zmax_otype;
static void create_letbind_19(void) {  


  int64_t zgsz3204;
  int64_t zgaz394;
  zgaz394 = sailgen_zzzz7nzzJzzK18z3MAX(zotype_width);
  {
    sail_int zgsz3205;
    CREATE(sail_int)(&zgsz3205);
    CONVERT_OF(sail_int, mach_int)(&zgsz3205, zgaz394);
    sail_int zgsz3206;
    CREATE(sail_int)(&zgsz3206);
    CONVERT_OF(sail_int, mach_int)(&zgsz3206, zreserved_otypes);
    sail_int zgsz3207;
    CREATE(sail_int)(&zgsz3207);
    sub_int(&zgsz3207, zgsz3205, zgsz3206);
    zgsz3204 = CONVERT_OF(mach_int, sail_int)(zgsz3207);
    KILL(sail_int)(&zgsz3207);
    KILL(sail_int)(&zgsz3206);
    KILL(sail_int)(&zgsz3205);
  }

  zmax_otype = zgsz3204;

let_end_140: ;
}
static void kill_letbind_19(void) {  
}

uint64_t sailgen_getCapHardPerms(struct zCapability);

uint64_t sailgen_getCapHardPerms(struct zCapability zcap)
{
  __label__ end_function_142, end_block_exception_143;

  uint64_t zcbz336;
  uint64_t zgaz3127;
  {
    bool zgaz395;
    zgaz395 = zcap.zpermit_set_CID;
    zgaz3127 = sailgen_bool_to_bits(zgaz395);
  
  }
  uint64_t zgaz3128;
  {
    uint64_t zgaz3125;
    {
      bool zgaz396;
      zgaz396 = zcap.zaccess_system_regs;
      zgaz3125 = sailgen_bool_to_bits(zgaz396);
    
    }
    uint64_t zgaz3126;
    {
      uint64_t zgaz3123;
      {
        bool zgaz397;
        zgaz397 = zcap.zpermit_unseal;
        zgaz3123 = sailgen_bool_to_bits(zgaz397);
      
      }
      uint64_t zgaz3124;
      {
        uint64_t zgaz3121;
        {
          bool zgaz398;
          zgaz398 = zcap.zpermit_ccall;
          zgaz3121 = sailgen_bool_to_bits(zgaz398);
        
        }
        uint64_t zgaz3122;
        {
          uint64_t zgaz3119;
          {
            bool zgaz399;
            zgaz399 = zcap.zpermit_seal;
            zgaz3119 = sailgen_bool_to_bits(zgaz399);
          
          }
          uint64_t zgaz3120;
          {
            uint64_t zgaz3117;
            {
              bool zgaz3100;
              zgaz3100 = zcap.zpermit_store_local_cap;
              zgaz3117 = sailgen_bool_to_bits(zgaz3100);
            
            }
            uint64_t zgaz3118;
            {
              uint64_t zgaz3115;
              {
                bool zgaz3101;
                zgaz3101 = zcap.zpermit_store_cap;
                zgaz3115 = sailgen_bool_to_bits(zgaz3101);
              
              }
              uint64_t zgaz3116;
              {
                uint64_t zgaz3113;
                {
                  bool zgaz3102;
                  zgaz3102 = zcap.zpermit_load_cap;
                  zgaz3113 = sailgen_bool_to_bits(zgaz3102);
                
                }
                uint64_t zgaz3114;
                {
                  uint64_t zgaz3111;
                  {
                    bool zgaz3103;
                    zgaz3103 = zcap.zpermit_store;
                    zgaz3111 = sailgen_bool_to_bits(zgaz3103);
                  
                  }
                  uint64_t zgaz3112;
                  {
                    uint64_t zgaz3109;
                    {
                      bool zgaz3104;
                      zgaz3104 = zcap.zpermit_load;
                      zgaz3109 = sailgen_bool_to_bits(zgaz3104);
                    
                    }
                    uint64_t zgaz3110;
                    {
                      uint64_t zgaz3107;
                      {
                        bool zgaz3105;
                        zgaz3105 = zcap.zpermit_execute;
                        zgaz3107 = sailgen_bool_to_bits(zgaz3105);
                      
                      }
                      uint64_t zgaz3108;
                      {
                        bool zgaz3106;
                        zgaz3106 = zcap.zglobal;
                        zgaz3108 = sailgen_bool_to_bits(zgaz3106);
                      
                      }
                      {
                        lbits zgsz3208;
                        CREATE(lbits)(&zgsz3208);
                        CONVERT_OF(lbits, fbits)(&zgsz3208, zgaz3107, UINT64_C(1) , true);
                        lbits zgsz3209;
                        CREATE(lbits)(&zgsz3209);
                        CONVERT_OF(lbits, fbits)(&zgsz3209, zgaz3108, UINT64_C(1) , true);
                        lbits zgsz3210;
                        CREATE(lbits)(&zgsz3210);
                        append(&zgsz3210, zgsz3208, zgsz3209);
                        zgaz3110 = CONVERT_OF(fbits, lbits)(zgsz3210, true);
                        KILL(lbits)(&zgsz3210);
                        KILL(lbits)(&zgsz3209);
                        KILL(lbits)(&zgsz3208);
                      }
                    
                    
                    }
                    {
                      lbits zgsz3211;
                      CREATE(lbits)(&zgsz3211);
                      CONVERT_OF(lbits, fbits)(&zgsz3211, zgaz3109, UINT64_C(1) , true);
                      lbits zgsz3212;
                      CREATE(lbits)(&zgsz3212);
                      CONVERT_OF(lbits, fbits)(&zgsz3212, zgaz3110, UINT64_C(2) , true);
                      lbits zgsz3213;
                      CREATE(lbits)(&zgsz3213);
                      append(&zgsz3213, zgsz3211, zgsz3212);
                      zgaz3112 = CONVERT_OF(fbits, lbits)(zgsz3213, true);
                      KILL(lbits)(&zgsz3213);
                      KILL(lbits)(&zgsz3212);
                      KILL(lbits)(&zgsz3211);
                    }
                  
                  
                  }
                  {
                    lbits zgsz3214;
                    CREATE(lbits)(&zgsz3214);
                    CONVERT_OF(lbits, fbits)(&zgsz3214, zgaz3111, UINT64_C(1) , true);
                    lbits zgsz3215;
                    CREATE(lbits)(&zgsz3215);
                    CONVERT_OF(lbits, fbits)(&zgsz3215, zgaz3112, UINT64_C(3) , true);
                    lbits zgsz3216;
                    CREATE(lbits)(&zgsz3216);
                    append(&zgsz3216, zgsz3214, zgsz3215);
                    zgaz3114 = CONVERT_OF(fbits, lbits)(zgsz3216, true);
                    KILL(lbits)(&zgsz3216);
                    KILL(lbits)(&zgsz3215);
                    KILL(lbits)(&zgsz3214);
                  }
                
                
                }
                {
                  lbits zgsz3217;
                  CREATE(lbits)(&zgsz3217);
                  CONVERT_OF(lbits, fbits)(&zgsz3217, zgaz3113, UINT64_C(1) , true);
                  lbits zgsz3218;
                  CREATE(lbits)(&zgsz3218);
                  CONVERT_OF(lbits, fbits)(&zgsz3218, zgaz3114, UINT64_C(4) , true);
                  lbits zgsz3219;
                  CREATE(lbits)(&zgsz3219);
                  append(&zgsz3219, zgsz3217, zgsz3218);
                  zgaz3116 = CONVERT_OF(fbits, lbits)(zgsz3219, true);
                  KILL(lbits)(&zgsz3219);
                  KILL(lbits)(&zgsz3218);
                  KILL(lbits)(&zgsz3217);
                }
              
              
              }
              {
                lbits zgsz3220;
                CREATE(lbits)(&zgsz3220);
                CONVERT_OF(lbits, fbits)(&zgsz3220, zgaz3115, UINT64_C(1) , true);
                lbits zgsz3221;
                CREATE(lbits)(&zgsz3221);
                CONVERT_OF(lbits, fbits)(&zgsz3221, zgaz3116, UINT64_C(5) , true);
                lbits zgsz3222;
                CREATE(lbits)(&zgsz3222);
                append(&zgsz3222, zgsz3220, zgsz3221);
                zgaz3118 = CONVERT_OF(fbits, lbits)(zgsz3222, true);
                KILL(lbits)(&zgsz3222);
                KILL(lbits)(&zgsz3221);
                KILL(lbits)(&zgsz3220);
              }
            
            
            }
            {
              lbits zgsz3223;
              CREATE(lbits)(&zgsz3223);
              CONVERT_OF(lbits, fbits)(&zgsz3223, zgaz3117, UINT64_C(1) , true);
              lbits zgsz3224;
              CREATE(lbits)(&zgsz3224);
              CONVERT_OF(lbits, fbits)(&zgsz3224, zgaz3118, UINT64_C(6) , true);
              lbits zgsz3225;
              CREATE(lbits)(&zgsz3225);
              append(&zgsz3225, zgsz3223, zgsz3224);
              zgaz3120 = CONVERT_OF(fbits, lbits)(zgsz3225, true);
              KILL(lbits)(&zgsz3225);
              KILL(lbits)(&zgsz3224);
              KILL(lbits)(&zgsz3223);
            }
          
          
          }
          {
            lbits zgsz3226;
            CREATE(lbits)(&zgsz3226);
            CONVERT_OF(lbits, fbits)(&zgsz3226, zgaz3119, UINT64_C(1) , true);
            lbits zgsz3227;
            CREATE(lbits)(&zgsz3227);
            CONVERT_OF(lbits, fbits)(&zgsz3227, zgaz3120, UINT64_C(7) , true);
            lbits zgsz3228;
            CREATE(lbits)(&zgsz3228);
            append(&zgsz3228, zgsz3226, zgsz3227);
            zgaz3122 = CONVERT_OF(fbits, lbits)(zgsz3228, true);
            KILL(lbits)(&zgsz3228);
            KILL(lbits)(&zgsz3227);
            KILL(lbits)(&zgsz3226);
          }
        
        
        }
        {
          lbits zgsz3229;
          CREATE(lbits)(&zgsz3229);
          CONVERT_OF(lbits, fbits)(&zgsz3229, zgaz3121, UINT64_C(1) , true);
          lbits zgsz3230;
          CREATE(lbits)(&zgsz3230);
          CONVERT_OF(lbits, fbits)(&zgsz3230, zgaz3122, UINT64_C(8) , true);
          lbits zgsz3231;
          CREATE(lbits)(&zgsz3231);
          append(&zgsz3231, zgsz3229, zgsz3230);
          zgaz3124 = CONVERT_OF(fbits, lbits)(zgsz3231, true);
          KILL(lbits)(&zgsz3231);
          KILL(lbits)(&zgsz3230);
          KILL(lbits)(&zgsz3229);
        }
      
      
      }
      {
        lbits zgsz3232;
        CREATE(lbits)(&zgsz3232);
        CONVERT_OF(lbits, fbits)(&zgsz3232, zgaz3123, UINT64_C(1) , true);
        lbits zgsz3233;
        CREATE(lbits)(&zgsz3233);
        CONVERT_OF(lbits, fbits)(&zgsz3233, zgaz3124, UINT64_C(9) , true);
        lbits zgsz3234;
        CREATE(lbits)(&zgsz3234);
        append(&zgsz3234, zgsz3232, zgsz3233);
        zgaz3126 = CONVERT_OF(fbits, lbits)(zgsz3234, true);
        KILL(lbits)(&zgsz3234);
        KILL(lbits)(&zgsz3233);
        KILL(lbits)(&zgsz3232);
      }
    
    
    }
    {
      lbits zgsz3235;
      CREATE(lbits)(&zgsz3235);
      CONVERT_OF(lbits, fbits)(&zgsz3235, zgaz3125, UINT64_C(1) , true);
      lbits zgsz3236;
      CREATE(lbits)(&zgsz3236);
      CONVERT_OF(lbits, fbits)(&zgsz3236, zgaz3126, UINT64_C(10) , true);
      lbits zgsz3237;
      CREATE(lbits)(&zgsz3237);
      append(&zgsz3237, zgsz3235, zgsz3236);
      zgaz3128 = CONVERT_OF(fbits, lbits)(zgsz3237, true);
      KILL(lbits)(&zgsz3237);
      KILL(lbits)(&zgsz3236);
      KILL(lbits)(&zgsz3235);
    }
  
  
  }
  {
    lbits zgsz3238;
    CREATE(lbits)(&zgsz3238);
    CONVERT_OF(lbits, fbits)(&zgsz3238, zgaz3127, UINT64_C(1) , true);
    lbits zgsz3239;
    CREATE(lbits)(&zgsz3239);
    CONVERT_OF(lbits, fbits)(&zgsz3239, zgaz3128, UINT64_C(11) , true);
    lbits zgsz3240;
    CREATE(lbits)(&zgsz3240);
    append(&zgsz3240, zgsz3238, zgsz3239);
    zcbz336 = CONVERT_OF(fbits, lbits)(zgsz3240, true);
    KILL(lbits)(&zgsz3240);
    KILL(lbits)(&zgsz3239);
    KILL(lbits)(&zgsz3238);
  }


end_function_142: ;
  return zcbz336;
end_block_exception_143: ;

  return UINT64_C(0xdeadc0de);
}

void sailgen_capToBits(lbits *rop, struct zCapability);

void sailgen_capToBits(lbits *zcbz337, struct zCapability zcap)
{
  __label__ cleanup_146, end_cleanup_147, end_function_145, end_block_exception_148, end_function_227;

  uint64_t zt_hi;
  {
    uint64_t zgaz3155;
    zgaz3155 = zcap.zT;
    int64_t zgaz3156;
    {
      sail_int zgsz3241;
      CREATE(sail_int)(&zgsz3241);
      CONVERT_OF(sail_int, mach_int)(&zgsz3241, zmantissa_width);
      sail_int zgsz3242;
      CREATE(sail_int)(&zgsz3242);
      CONVERT_OF(sail_int, mach_int)(&zgsz3242, 3l);
      sail_int zgsz3243;
      CREATE(sail_int)(&zgsz3243);
      sub_int(&zgsz3243, zgsz3241, zgsz3242);
      zgaz3156 = CONVERT_OF(mach_int, sail_int)(zgsz3243);
      KILL(sail_int)(&zgsz3243);
      KILL(sail_int)(&zgsz3242);
      KILL(sail_int)(&zgsz3241);
    }
    {
      lbits zgsz3244;
      CREATE(lbits)(&zgsz3244);
      CONVERT_OF(lbits, fbits)(&zgsz3244, zgaz3155, UINT64_C(14) , true);
      sail_int zgsz3245;
      CREATE(sail_int)(&zgsz3245);
      CONVERT_OF(sail_int, mach_int)(&zgsz3245, zgaz3156);
      sail_int zgsz3246;
      CREATE(sail_int)(&zgsz3246);
      CONVERT_OF(sail_int, mach_int)(&zgsz3246, 3l);
      lbits zgsz3247;
      CREATE(lbits)(&zgsz3247);
      vector_subrange_lbits(&zgsz3247, zgsz3244, zgsz3245, zgsz3246);
      zt_hi = CONVERT_OF(fbits, lbits)(zgsz3247, true);
      KILL(lbits)(&zgsz3247);
      KILL(sail_int)(&zgsz3246);
      KILL(sail_int)(&zgsz3245);
      KILL(lbits)(&zgsz3244);
    }
  
  
  }
  uint64_t zt_lo;
  {
    uint64_t zgaz3154;
    zgaz3154 = zcap.zT;
    {
      lbits zgsz3248;
      CREATE(lbits)(&zgsz3248);
      CONVERT_OF(lbits, fbits)(&zgsz3248, zgaz3154, UINT64_C(14) , true);
      sail_int zgsz3249;
      CREATE(sail_int)(&zgsz3249);
      CONVERT_OF(sail_int, mach_int)(&zgsz3249, 2l);
      sail_int zgsz3250;
      CREATE(sail_int)(&zgsz3250);
      CONVERT_OF(sail_int, mach_int)(&zgsz3250, 0l);
      lbits zgsz3251;
      CREATE(lbits)(&zgsz3251);
      vector_subrange_lbits(&zgsz3251, zgsz3248, zgsz3249, zgsz3250);
      zt_lo = CONVERT_OF(fbits, lbits)(zgsz3251, true);
      KILL(lbits)(&zgsz3251);
      KILL(sail_int)(&zgsz3250);
      KILL(sail_int)(&zgsz3249);
      KILL(lbits)(&zgsz3248);
    }
  
  }
  uint64_t zb_hi;
  {
    uint64_t zgaz3152;
    zgaz3152 = zcap.zB;
    int64_t zgaz3153;
    {
      sail_int zgsz3252;
      CREATE(sail_int)(&zgsz3252);
      CONVERT_OF(sail_int, mach_int)(&zgsz3252, zmantissa_width);
      sail_int zgsz3253;
      CREATE(sail_int)(&zgsz3253);
      CONVERT_OF(sail_int, mach_int)(&zgsz3253, 1l);
      sail_int zgsz3254;
      CREATE(sail_int)(&zgsz3254);
      sub_int(&zgsz3254, zgsz3252, zgsz3253);
      zgaz3153 = CONVERT_OF(mach_int, sail_int)(zgsz3254);
      KILL(sail_int)(&zgsz3254);
      KILL(sail_int)(&zgsz3253);
      KILL(sail_int)(&zgsz3252);
    }
    {
      lbits zgsz3255;
      CREATE(lbits)(&zgsz3255);
      CONVERT_OF(lbits, fbits)(&zgsz3255, zgaz3152, UINT64_C(14) , true);
      sail_int zgsz3256;
      CREATE(sail_int)(&zgsz3256);
      CONVERT_OF(sail_int, mach_int)(&zgsz3256, zgaz3153);
      sail_int zgsz3257;
      CREATE(sail_int)(&zgsz3257);
      CONVERT_OF(sail_int, mach_int)(&zgsz3257, 3l);
      lbits zgsz3258;
      CREATE(lbits)(&zgsz3258);
      vector_subrange_lbits(&zgsz3258, zgsz3255, zgsz3256, zgsz3257);
      zb_hi = CONVERT_OF(fbits, lbits)(zgsz3258, true);
      KILL(lbits)(&zgsz3258);
      KILL(sail_int)(&zgsz3257);
      KILL(sail_int)(&zgsz3256);
      KILL(lbits)(&zgsz3255);
    }
  
  
  }
  uint64_t zb_lo;
  {
    uint64_t zgaz3151;
    zgaz3151 = zcap.zB;
    {
      lbits zgsz3259;
      CREATE(lbits)(&zgsz3259);
      CONVERT_OF(lbits, fbits)(&zgsz3259, zgaz3151, UINT64_C(14) , true);
      sail_int zgsz3260;
      CREATE(sail_int)(&zgsz3260);
      CONVERT_OF(sail_int, mach_int)(&zgsz3260, 2l);
      sail_int zgsz3261;
      CREATE(sail_int)(&zgsz3261);
      CONVERT_OF(sail_int, mach_int)(&zgsz3261, 0l);
      lbits zgsz3262;
      CREATE(lbits)(&zgsz3262);
      vector_subrange_lbits(&zgsz3262, zgsz3259, zgsz3260, zgsz3261);
      zb_lo = CONVERT_OF(fbits, lbits)(zgsz3262, true);
      KILL(lbits)(&zgsz3262);
      KILL(sail_int)(&zgsz3261);
      KILL(sail_int)(&zgsz3260);
      KILL(lbits)(&zgsz3259);
    }
  
  }
  {
    bool zgaz3129;
    zgaz3129 = zcap.zinternal_e;
    unit zgsz3263;
    if (zgaz3129) {
    {
      uint64_t zgaz3130;
      zgaz3130 = zcap.zE;
      {
        lbits zgsz3264;
        CREATE(lbits)(&zgsz3264);
        CONVERT_OF(lbits, fbits)(&zgsz3264, zgaz3130, UINT64_C(6) , true);
        sail_int zgsz3265;
        CREATE(sail_int)(&zgsz3265);
        CONVERT_OF(sail_int, mach_int)(&zgsz3265, 5l);
        sail_int zgsz3266;
        CREATE(sail_int)(&zgsz3266);
        CONVERT_OF(sail_int, mach_int)(&zgsz3266, 3l);
        lbits zgsz3267;
        CREATE(lbits)(&zgsz3267);
        vector_subrange_lbits(&zgsz3267, zgsz3264, zgsz3265, zgsz3266);
        zt_lo = CONVERT_OF(fbits, lbits)(zgsz3267, true);
        KILL(lbits)(&zgsz3267);
        KILL(sail_int)(&zgsz3266);
        KILL(sail_int)(&zgsz3265);
        KILL(lbits)(&zgsz3264);
      }
      unit zgsz3268;
      zgsz3268 = UNIT;
    
    }
    uint64_t zgaz3131;
    zgaz3131 = zcap.zE;
    {
      lbits zgsz3269;
      CREATE(lbits)(&zgsz3269);
      CONVERT_OF(lbits, fbits)(&zgsz3269, zgaz3131, UINT64_C(6) , true);
      sail_int zgsz3270;
      CREATE(sail_int)(&zgsz3270);
      CONVERT_OF(sail_int, mach_int)(&zgsz3270, 2l);
      sail_int zgsz3271;
      CREATE(sail_int)(&zgsz3271);
      CONVERT_OF(sail_int, mach_int)(&zgsz3271, 0l);
      lbits zgsz3272;
      CREATE(lbits)(&zgsz3272);
      vector_subrange_lbits(&zgsz3272, zgsz3269, zgsz3270, zgsz3271);
      zb_lo = CONVERT_OF(fbits, lbits)(zgsz3272, true);
      KILL(lbits)(&zgsz3272);
      KILL(sail_int)(&zgsz3271);
      KILL(sail_int)(&zgsz3270);
      KILL(lbits)(&zgsz3269);
    }
    zgsz3263 = UNIT;
  
    } else {  zgsz3263 = UNIT;  }
  
  }
  lbits zgaz3150;
  CREATE(lbits)(&zgaz3150);
  {
    uint64_t zgaz3148;
    zgaz3148 = zcap.zuperms;
    lbits zgaz3149;
    CREATE(lbits)(&zgaz3149);
    {
      uint64_t zgaz3146;
      zgaz3146 = sailgen_getCapHardPerms(zcap);
      lbits zgaz3147;
      CREATE(lbits)(&zgaz3147);
      {
        uint64_t zgaz3144;
        zgaz3144 = zcap.zreserved;
        lbits zgaz3145;
        CREATE(lbits)(&zgaz3145);
        {
          uint64_t zgaz3142;
          {
            bool zgaz3132;
            zgaz3132 = zcap.zflag_cap_mode;
            zgaz3142 = sailgen_bool_to_bits(zgaz3132);
          
          }
          lbits zgaz3143;
          CREATE(lbits)(&zgaz3143);
          {
            uint64_t zgaz3140;
            zgaz3140 = zcap.zotype;
            lbits zgaz3141;
            CREATE(lbits)(&zgaz3141);
            {
              uint64_t zgaz3138;
              {
                bool zgaz3133;
                zgaz3133 = zcap.zinternal_e;
                zgaz3138 = sailgen_bool_to_bits(zgaz3133);
              
              }
              lbits zgaz3139;
              CREATE(lbits)(&zgaz3139);
              {
                lbits zgaz3137;
                CREATE(lbits)(&zgaz3137);
                {
                  lbits zgaz3136;
                  CREATE(lbits)(&zgaz3136);
                  {
                    lbits zgaz3135;
                    CREATE(lbits)(&zgaz3135);
                    {
                      uint64_t zgaz3134;
                      zgaz3134 = zcap.zaddress;
                      {
                        lbits zgsz3274;
                        CREATE(lbits)(&zgsz3274);
                        CONVERT_OF(lbits, fbits)(&zgsz3274, zgaz3134, UINT64_C(64) , true);
                        lbits zgsz3273;
                        CREATE(lbits)(&zgsz3273);
                        CONVERT_OF(lbits, fbits)(&zgsz3273, zb_lo, UINT64_C(3) , true);
                        append(&zgaz3135, zgsz3273, zgsz3274);
                        KILL(lbits)(&zgsz3274);
                        KILL(lbits)(&zgsz3273);
                      }
                    
                    }
                    {
                      lbits zgsz3275;
                      CREATE(lbits)(&zgsz3275);
                      CONVERT_OF(lbits, fbits)(&zgsz3275, zb_hi, UINT64_C(11) , true);
                      append(&zgaz3136, zgsz3275, zgaz3135);
                      KILL(lbits)(&zgsz3275);
                    }
                    KILL(lbits)(&zgaz3135);
                  }
                  {
                    lbits zgsz3276;
                    CREATE(lbits)(&zgsz3276);
                    CONVERT_OF(lbits, fbits)(&zgsz3276, zt_lo, UINT64_C(3) , true);
                    append(&zgaz3137, zgsz3276, zgaz3136);
                    KILL(lbits)(&zgsz3276);
                  }
                  KILL(lbits)(&zgaz3136);
                }
                {
                  lbits zgsz3277;
                  CREATE(lbits)(&zgsz3277);
                  CONVERT_OF(lbits, fbits)(&zgsz3277, zt_hi, UINT64_C(9) , true);
                  append(&zgaz3139, zgsz3277, zgaz3137);
                  KILL(lbits)(&zgsz3277);
                }
                KILL(lbits)(&zgaz3137);
              }
              {
                lbits zgsz3278;
                CREATE(lbits)(&zgsz3278);
                CONVERT_OF(lbits, fbits)(&zgsz3278, zgaz3138, UINT64_C(1) , true);
                append(&zgaz3141, zgsz3278, zgaz3139);
                KILL(lbits)(&zgsz3278);
              }
              KILL(lbits)(&zgaz3139);
            
            }
            {
              lbits zgsz3279;
              CREATE(lbits)(&zgsz3279);
              CONVERT_OF(lbits, fbits)(&zgsz3279, zgaz3140, UINT64_C(18) , true);
              append(&zgaz3143, zgsz3279, zgaz3141);
              KILL(lbits)(&zgsz3279);
            }
            KILL(lbits)(&zgaz3141);
          
          }
          {
            lbits zgsz3280;
            CREATE(lbits)(&zgsz3280);
            CONVERT_OF(lbits, fbits)(&zgsz3280, zgaz3142, UINT64_C(1) , true);
            append(&zgaz3145, zgsz3280, zgaz3143);
            KILL(lbits)(&zgsz3280);
          }
          KILL(lbits)(&zgaz3143);
        
        }
        {
          lbits zgsz3281;
          CREATE(lbits)(&zgsz3281);
          CONVERT_OF(lbits, fbits)(&zgsz3281, zgaz3144, UINT64_C(2) , true);
          append(&zgaz3147, zgsz3281, zgaz3145);
          KILL(lbits)(&zgsz3281);
        }
        KILL(lbits)(&zgaz3145);
      
      }
      {
        lbits zgsz3282;
        CREATE(lbits)(&zgsz3282);
        CONVERT_OF(lbits, fbits)(&zgsz3282, zgaz3146, UINT64_C(12) , true);
        append(&zgaz3149, zgsz3282, zgaz3147);
        KILL(lbits)(&zgsz3282);
      }
      KILL(lbits)(&zgaz3147);
    
    }
    {
      lbits zgsz3283;
      CREATE(lbits)(&zgsz3283);
      CONVERT_OF(lbits, fbits)(&zgsz3283, zgaz3148, UINT64_C(4) , true);
      append(&zgaz3150, zgsz3283, zgaz3149);
      KILL(lbits)(&zgsz3283);
    }
    KILL(lbits)(&zgaz3149);
  
  }
  COPY(lbits)((*(&zcbz337)), zgaz3150);
  goto cleanup_146;
  /* unreachable after return */
  KILL(lbits)(&zgaz3150);




  goto end_cleanup_147;
cleanup_146: ;




  KILL(lbits)(&zgaz3150);
  goto end_function_145;
end_cleanup_147: ;
end_function_145: ;
  goto end_function_227;
end_block_exception_148: ;
  goto end_function_227;
end_function_227: ;
}

lbits znull_cap_bits;
static void create_letbind_20(void) {    CREATE(lbits)(&znull_cap_bits);


  lbits zgsz3284;
  CREATE(lbits)(&zgsz3284);
  sailgen_capToBits(&zgsz3284, znull_cap);
  COPY(lbits)(&znull_cap_bits, zgsz3284);
  KILL(lbits)(&zgsz3284);
let_end_149: ;
}
static void kill_letbind_20(void) {    KILL(lbits)(&znull_cap_bits);
}

void sailgen_capToMemBits(lbits *rop, struct zCapability);

void sailgen_capToMemBits(lbits *zcbz338, struct zCapability zcap)
{
  __label__ end_function_151, end_block_exception_152, end_function_226;

  lbits zgaz3157;
  CREATE(lbits)(&zgaz3157);
  sailgen_capToBits(&zgaz3157, zcap);
  xor_bits((*(&zcbz338)), zgaz3157, znull_cap_bits);
  KILL(lbits)(&zgaz3157);
end_function_151: ;
  goto end_function_226;
end_block_exception_152: ;
  goto end_function_226;
end_function_226: ;
}

struct zCapability sailgen_memBitsToCapability(bool, lbits);

struct zCapability sailgen_memBitsToCapability(bool ztag, lbits zb)
{
  __label__ end_function_154, end_block_exception_155;

  struct zCapability zcbz339;
  lbits zgaz3158;
  CREATE(lbits)(&zgaz3158);
  xor_bits(&zgaz3158, zb, znull_cap_bits);
  zcbz339 = sailgen_capBitsToCapability(ztag, zgaz3158);
  KILL(lbits)(&zgaz3158);
end_function_154: ;
  return zcbz339;
end_block_exception_155: ;
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

void sailgen_getCapBoundsBits(struct ztuple_z8z5bv64zCz0z5bvz9 *rop, struct zCapability);

void sailgen_getCapBoundsBits(struct ztuple_z8z5bv64zCz0z5bvz9 *zcbz340, struct zCapability zc)
{
  __label__ case_158, finish_match_157, end_function_163, end_block_exception_164, end_function_225;

  int64_t zE;
  {
    int64_t zgaz3195;
    {
      uint64_t zgaz3194;
      zgaz3194 = zc.zE;
      zgaz3195 = ((mach_int) zgaz3194);
    
    }
    {
      sail_int zgsz3285;
      CREATE(sail_int)(&zgsz3285);
      CONVERT_OF(sail_int, mach_int)(&zgsz3285, zmaxE);
      sail_int zgsz3286;
      CREATE(sail_int)(&zgsz3286);
      CONVERT_OF(sail_int, mach_int)(&zgsz3286, zgaz3195);
      sail_int zgsz3287;
      CREATE(sail_int)(&zgsz3287);
      min_int(&zgsz3287, zgsz3285, zgsz3286);
      zE = CONVERT_OF(mach_int, sail_int)(zgsz3287);
      KILL(sail_int)(&zgsz3287);
      KILL(sail_int)(&zgsz3286);
      KILL(sail_int)(&zgsz3285);
    }
  
  }
  uint64_t zgaz3159;
  zgaz3159 = zc.zaddress;
  struct ztuple_z8z5bv64zCz0z5bvz9 zgsz3288;
  CREATE(ztuple_z8z5bv64zCz0z5bvz9)(&zgsz3288);
  {
    __label__ case_160, finish_match_159;

    uint64_t za;
    za = zgaz3159;
    uint64_t za3;
    {
      uint64_t zgaz3193;
      {
        int64_t zgaz3192;
        {
          int64_t zgaz3191;
          {
            sail_int zgsz3289;
            CREATE(sail_int)(&zgsz3289);
            CONVERT_OF(sail_int, mach_int)(&zgsz3289, zE);
            sail_int zgsz3290;
            CREATE(sail_int)(&zgsz3290);
            CONVERT_OF(sail_int, mach_int)(&zgsz3290, zmantissa_width);
            sail_int zgsz3291;
            CREATE(sail_int)(&zgsz3291);
            add_int(&zgsz3291, zgsz3289, zgsz3290);
            zgaz3191 = CONVERT_OF(mach_int, sail_int)(zgsz3291);
            KILL(sail_int)(&zgsz3291);
            KILL(sail_int)(&zgsz3290);
            KILL(sail_int)(&zgsz3289);
          }
          {
            sail_int zgsz3292;
            CREATE(sail_int)(&zgsz3292);
            CONVERT_OF(sail_int, mach_int)(&zgsz3292, zgaz3191);
            sail_int zgsz3293;
            CREATE(sail_int)(&zgsz3293);
            CONVERT_OF(sail_int, mach_int)(&zgsz3293, 3l);
            sail_int zgsz3294;
            CREATE(sail_int)(&zgsz3294);
            sub_int(&zgsz3294, zgsz3292, zgsz3293);
            zgaz3192 = CONVERT_OF(mach_int, sail_int)(zgsz3294);
            KILL(sail_int)(&zgsz3294);
            KILL(sail_int)(&zgsz3293);
            KILL(sail_int)(&zgsz3292);
          }
        
        }
        {
          lbits zgsz3295;
          CREATE(lbits)(&zgsz3295);
          CONVERT_OF(lbits, fbits)(&zgsz3295, za, UINT64_C(64) , true);
          sail_int zgsz3296;
          CREATE(sail_int)(&zgsz3296);
          CONVERT_OF(sail_int, mach_int)(&zgsz3296, zgaz3192);
          lbits zgsz3297;
          CREATE(lbits)(&zgsz3297);
          shiftr(&zgsz3297, zgsz3295, zgsz3296);
          zgaz3193 = CONVERT_OF(fbits, lbits)(zgsz3297, true);
          KILL(lbits)(&zgsz3297);
          KILL(sail_int)(&zgsz3296);
          KILL(lbits)(&zgsz3295);
        }
      
      }
      {
        lbits zgsz3298;
        CREATE(lbits)(&zgsz3298);
        CONVERT_OF(lbits, fbits)(&zgsz3298, zgaz3193, UINT64_C(64) , true);
        sail_int zgsz3299;
        CREATE(sail_int)(&zgsz3299);
        CONVERT_OF(sail_int, mach_int)(&zgsz3299, 3l);
        lbits zgsz3300;
        CREATE(lbits)(&zgsz3300);
        sail_truncate(&zgsz3300, zgsz3298, zgsz3299);
        za3 = CONVERT_OF(fbits, lbits)(zgsz3300, true);
        KILL(lbits)(&zgsz3300);
        KILL(sail_int)(&zgsz3299);
        KILL(lbits)(&zgsz3298);
      }
    
    }
    uint64_t zB3;
    {
      uint64_t zgaz3190;
      zgaz3190 = zc.zB;
      {
        lbits zgsz3301;
        CREATE(lbits)(&zgsz3301);
        CONVERT_OF(lbits, fbits)(&zgsz3301, zgaz3190, UINT64_C(14) , true);
        sail_int zgsz3302;
        CREATE(sail_int)(&zgsz3302);
        CONVERT_OF(sail_int, mach_int)(&zgsz3302, 3l);
        lbits zgsz3303;
        CREATE(lbits)(&zgsz3303);
        sail_truncateLSB(&zgsz3303, zgsz3301, zgsz3302);
        zB3 = CONVERT_OF(fbits, lbits)(zgsz3303, true);
        KILL(lbits)(&zgsz3303);
        KILL(sail_int)(&zgsz3302);
        KILL(lbits)(&zgsz3301);
      }
    
    }
    uint64_t zT3;
    {
      uint64_t zgaz3189;
      zgaz3189 = zc.zT;
      {
        lbits zgsz3304;
        CREATE(lbits)(&zgsz3304);
        CONVERT_OF(lbits, fbits)(&zgsz3304, zgaz3189, UINT64_C(14) , true);
        sail_int zgsz3305;
        CREATE(sail_int)(&zgsz3305);
        CONVERT_OF(sail_int, mach_int)(&zgsz3305, 3l);
        lbits zgsz3306;
        CREATE(lbits)(&zgsz3306);
        sail_truncateLSB(&zgsz3306, zgsz3304, zgsz3305);
        zT3 = CONVERT_OF(fbits, lbits)(zgsz3306, true);
        KILL(lbits)(&zgsz3306);
        KILL(sail_int)(&zgsz3305);
        KILL(lbits)(&zgsz3304);
      }
    
    }
    uint64_t zR3;
    {
      lbits zgsz3307;
      CREATE(lbits)(&zgsz3307);
      CONVERT_OF(lbits, fbits)(&zgsz3307, zB3, UINT64_C(3) , true);
      lbits zgsz3308;
      CREATE(lbits)(&zgsz3308);
      CONVERT_OF(lbits, fbits)(&zgsz3308, UINT64_C(0b001), UINT64_C(3) , true);
      lbits zgsz3309;
      CREATE(lbits)(&zgsz3309);
      sub_bits(&zgsz3309, zgsz3307, zgsz3308);
      zR3 = CONVERT_OF(fbits, lbits)(zgsz3309, true);
      KILL(lbits)(&zgsz3309);
      KILL(lbits)(&zgsz3308);
      KILL(lbits)(&zgsz3307);
    }
    int64_t zaHi;
    {
      bool zgaz3188;
      zgaz3188 = sailgen_z8operatorz0zzzz7nzzJzzK3z3zI_uz9(za3, zR3);
      if (zgaz3188) {  zaHi = 1l;  } else {  zaHi = 0l;  }
    
    }
    int64_t zbHi;
    {
      bool zgaz3187;
      zgaz3187 = sailgen_z8operatorz0zzzz7nzzJzzK3z3zI_uz9(zB3, zR3);
      if (zgaz3187) {  zbHi = 1l;  } else {  zbHi = 0l;  }
    
    }
    int64_t ztHi;
    {
      bool zgaz3186;
      zgaz3186 = sailgen_z8operatorz0zzzz7nzzJzzK3z3zI_uz9(zT3, zR3);
      if (zgaz3186) {  ztHi = 1l;  } else {  ztHi = 0l;  }
    
    }
    int64_t zcorrection_base;
    {
      sail_int zgsz3310;
      CREATE(sail_int)(&zgsz3310);
      CONVERT_OF(sail_int, mach_int)(&zgsz3310, zbHi);
      sail_int zgsz3311;
      CREATE(sail_int)(&zgsz3311);
      CONVERT_OF(sail_int, mach_int)(&zgsz3311, zaHi);
      sail_int zgsz3312;
      CREATE(sail_int)(&zgsz3312);
      sub_int(&zgsz3312, zgsz3310, zgsz3311);
      zcorrection_base = CONVERT_OF(mach_int, sail_int)(zgsz3312);
      KILL(sail_int)(&zgsz3312);
      KILL(sail_int)(&zgsz3311);
      KILL(sail_int)(&zgsz3310);
    }
    int64_t zcorrection_top;
    {
      sail_int zgsz3313;
      CREATE(sail_int)(&zgsz3313);
      CONVERT_OF(sail_int, mach_int)(&zgsz3313, ztHi);
      sail_int zgsz3314;
      CREATE(sail_int)(&zgsz3314);
      CONVERT_OF(sail_int, mach_int)(&zgsz3314, zaHi);
      sail_int zgsz3315;
      CREATE(sail_int)(&zgsz3315);
      sub_int(&zgsz3315, zgsz3313, zgsz3314);
      zcorrection_top = CONVERT_OF(mach_int, sail_int)(zgsz3315);
      KILL(sail_int)(&zgsz3315);
      KILL(sail_int)(&zgsz3314);
      KILL(sail_int)(&zgsz3313);
    }
    uint64_t za_top;
    {
      int64_t zgaz3185;
      {
        sail_int zgsz3316;
        CREATE(sail_int)(&zgsz3316);
        CONVERT_OF(sail_int, mach_int)(&zgsz3316, zE);
        sail_int zgsz3317;
        CREATE(sail_int)(&zgsz3317);
        CONVERT_OF(sail_int, mach_int)(&zgsz3317, zmantissa_width);
        sail_int zgsz3318;
        CREATE(sail_int)(&zgsz3318);
        add_int(&zgsz3318, zgsz3316, zgsz3317);
        zgaz3185 = CONVERT_OF(mach_int, sail_int)(zgsz3318);
        KILL(sail_int)(&zgsz3318);
        KILL(sail_int)(&zgsz3317);
        KILL(sail_int)(&zgsz3316);
      }
      {
        lbits zgsz3319;
        CREATE(lbits)(&zgsz3319);
        CONVERT_OF(lbits, fbits)(&zgsz3319, za, UINT64_C(64) , true);
        sail_int zgsz3320;
        CREATE(sail_int)(&zgsz3320);
        CONVERT_OF(sail_int, mach_int)(&zgsz3320, zgaz3185);
        lbits zgsz3321;
        CREATE(lbits)(&zgsz3321);
        shiftr(&zgsz3321, zgsz3319, zgsz3320);
        za_top = CONVERT_OF(fbits, lbits)(zgsz3321, true);
        KILL(lbits)(&zgsz3321);
        KILL(sail_int)(&zgsz3320);
        KILL(lbits)(&zgsz3319);
      }
    
    }
    lbits zbase;
    CREATE(lbits)(&zbase);
    {
      lbits zgaz3184;
      CREATE(lbits)(&zgaz3184);
      {
        uint64_t zgaz3182;
        {
          lbits zgsz3322;
          CREATE(lbits)(&zgsz3322);
          CONVERT_OF(lbits, fbits)(&zgsz3322, za_top, UINT64_C(64) , true);
          sail_int zgsz3323;
          CREATE(sail_int)(&zgsz3323);
          CONVERT_OF(sail_int, mach_int)(&zgsz3323, zcorrection_base);
          lbits zgsz3324;
          CREATE(lbits)(&zgsz3324);
          add_bits_int(&zgsz3324, zgsz3322, zgsz3323);
          zgaz3182 = CONVERT_OF(fbits, lbits)(zgsz3324, true);
          KILL(lbits)(&zgsz3324);
          KILL(sail_int)(&zgsz3323);
          KILL(lbits)(&zgsz3322);
        }
        lbits zgaz3183;
        CREATE(lbits)(&zgaz3183);
        {
          uint64_t zgaz3180;
          zgaz3180 = zc.zB;
          sbits zgaz3181;
          {
            sail_int zgsz3325;
            CREATE(sail_int)(&zgsz3325);
            CONVERT_OF(sail_int, mach_int)(&zgsz3325, zE);
            lbits zgsz3326;
            CREATE(lbits)(&zgsz3326);
            sailgen_zzz3zzz3zzeros_implicit(&zgsz3326, zgsz3325);
            zgaz3181 = CONVERT_OF(sbits, lbits)(zgsz3326, true);
            KILL(lbits)(&zgsz3326);
            KILL(sail_int)(&zgsz3325);
          }
          {
            lbits zgsz3328;
            CREATE(lbits)(&zgsz3328);
            CONVERT_OF(lbits, sbits)(&zgsz3328, zgaz3181, true);
            lbits zgsz3327;
            CREATE(lbits)(&zgsz3327);
            CONVERT_OF(lbits, fbits)(&zgsz3327, zgaz3180, UINT64_C(14) , true);
            append(&zgaz3183, zgsz3327, zgsz3328);
            KILL(lbits)(&zgsz3328);
            KILL(lbits)(&zgsz3327);
          }
        
        
        }
        {
          lbits zgsz3329;
          CREATE(lbits)(&zgsz3329);
          CONVERT_OF(lbits, fbits)(&zgsz3329, zgaz3182, UINT64_C(64) , true);
          append(&zgaz3184, zgsz3329, zgaz3183);
          KILL(lbits)(&zgsz3329);
        }
        KILL(lbits)(&zgaz3183);
      
      }
      {
        sail_int zgsz3330;
        CREATE(sail_int)(&zgsz3330);
        CONVERT_OF(sail_int, mach_int)(&zgsz3330, zcap_len_width);
        sail_truncate(&zbase, zgaz3184, zgsz3330);
        KILL(sail_int)(&zgsz3330);
      }
      KILL(lbits)(&zgaz3184);
    }
    lbits ztop;
    CREATE(lbits)(&ztop);
    {
      lbits zgaz3179;
      CREATE(lbits)(&zgaz3179);
      {
        uint64_t zgaz3177;
        {
          lbits zgsz3331;
          CREATE(lbits)(&zgsz3331);
          CONVERT_OF(lbits, fbits)(&zgsz3331, za_top, UINT64_C(64) , true);
          sail_int zgsz3332;
          CREATE(sail_int)(&zgsz3332);
          CONVERT_OF(sail_int, mach_int)(&zgsz3332, zcorrection_top);
          lbits zgsz3333;
          CREATE(lbits)(&zgsz3333);
          add_bits_int(&zgsz3333, zgsz3331, zgsz3332);
          zgaz3177 = CONVERT_OF(fbits, lbits)(zgsz3333, true);
          KILL(lbits)(&zgsz3333);
          KILL(sail_int)(&zgsz3332);
          KILL(lbits)(&zgsz3331);
        }
        lbits zgaz3178;
        CREATE(lbits)(&zgaz3178);
        {
          uint64_t zgaz3175;
          zgaz3175 = zc.zT;
          sbits zgaz3176;
          {
            sail_int zgsz3334;
            CREATE(sail_int)(&zgsz3334);
            CONVERT_OF(sail_int, mach_int)(&zgsz3334, zE);
            lbits zgsz3335;
            CREATE(lbits)(&zgsz3335);
            sailgen_zzz3zzz3zzeros_implicit(&zgsz3335, zgsz3334);
            zgaz3176 = CONVERT_OF(sbits, lbits)(zgsz3335, true);
            KILL(lbits)(&zgsz3335);
            KILL(sail_int)(&zgsz3334);
          }
          {
            lbits zgsz3337;
            CREATE(lbits)(&zgsz3337);
            CONVERT_OF(lbits, sbits)(&zgsz3337, zgaz3176, true);
            lbits zgsz3336;
            CREATE(lbits)(&zgsz3336);
            CONVERT_OF(lbits, fbits)(&zgsz3336, zgaz3175, UINT64_C(14) , true);
            append(&zgaz3178, zgsz3336, zgsz3337);
            KILL(lbits)(&zgsz3337);
            KILL(lbits)(&zgsz3336);
          }
        
        
        }
        {
          lbits zgsz3338;
          CREATE(lbits)(&zgsz3338);
          CONVERT_OF(lbits, fbits)(&zgsz3338, zgaz3177, UINT64_C(64) , true);
          append(&zgaz3179, zgsz3338, zgaz3178);
          KILL(lbits)(&zgsz3338);
        }
        KILL(lbits)(&zgaz3178);
      
      }
      {
        sail_int zgsz3339;
        CREATE(sail_int)(&zgsz3339);
        CONVERT_OF(sail_int, mach_int)(&zgsz3339, zcap_len_width);
        sail_truncate(&ztop, zgaz3179, zgsz3339);
        KILL(sail_int)(&zgsz3339);
      }
      KILL(lbits)(&zgaz3179);
    }
    uint64_t zgaz3163;
    {
      uint64_t zgaz3162;
      {
        fbits zgaz3161;
        {
          int64_t zgaz3160;
          {
            sail_int zgsz3340;
            CREATE(sail_int)(&zgsz3340);
            CONVERT_OF(sail_int, mach_int)(&zgsz3340, zcap_addr_width);
            sail_int zgsz3341;
            CREATE(sail_int)(&zgsz3341);
            CONVERT_OF(sail_int, mach_int)(&zgsz3341, 1l);
            sail_int zgsz3342;
            CREATE(sail_int)(&zgsz3342);
            sub_int(&zgsz3342, zgsz3340, zgsz3341);
            zgaz3160 = CONVERT_OF(mach_int, sail_int)(zgsz3342);
            KILL(sail_int)(&zgsz3342);
            KILL(sail_int)(&zgsz3341);
            KILL(sail_int)(&zgsz3340);
          }
          {
            sail_int zgsz3343;
            CREATE(sail_int)(&zgsz3343);
            CONVERT_OF(sail_int, mach_int)(&zgsz3343, zgaz3160);
            zgaz3161 = bitvector_access(zbase, zgsz3343);
            KILL(sail_int)(&zgsz3343);
          }
        
        }
        uint64_t zgsz3344;
        zgsz3344 = UINT64_C(0b0);
        zgsz3344 = update_fbits(zgsz3344, 0l, zgaz3161);
        zgaz3162 = zgsz3344;
      
      }
      {
        lbits zgsz3345;
        CREATE(lbits)(&zgsz3345);
        CONVERT_OF(lbits, fbits)(&zgsz3345, UINT64_C(0b0), UINT64_C(1) , true);
        lbits zgsz3346;
        CREATE(lbits)(&zgsz3346);
        CONVERT_OF(lbits, fbits)(&zgsz3346, zgaz3162, UINT64_C(1) , true);
        lbits zgsz3347;
        CREATE(lbits)(&zgsz3347);
        append(&zgsz3347, zgsz3345, zgsz3346);
        zgaz3163 = CONVERT_OF(fbits, lbits)(zgsz3347, true);
        KILL(lbits)(&zgsz3347);
        KILL(lbits)(&zgsz3346);
        KILL(lbits)(&zgsz3345);
      }
    
    }
    struct ztuple_z8z5bv64zCz0z5bvz9 zgsz3348;
    CREATE(ztuple_z8z5bv64zCz0z5bvz9)(&zgsz3348);
    {
      __label__ case_162, finish_match_161;

      uint64_t zbase2;
      zbase2 = zgaz3163;
      uint64_t zgaz3165;
      {
        int64_t zgaz3164;
        {
          sail_int zgsz3349;
          CREATE(sail_int)(&zgsz3349);
          CONVERT_OF(sail_int, mach_int)(&zgsz3349, zcap_addr_width);
          sail_int zgsz3350;
          CREATE(sail_int)(&zgsz3350);
          CONVERT_OF(sail_int, mach_int)(&zgsz3350, 1l);
          sail_int zgsz3351;
          CREATE(sail_int)(&zgsz3351);
          sub_int(&zgsz3351, zgsz3349, zgsz3350);
          zgaz3164 = CONVERT_OF(mach_int, sail_int)(zgsz3351);
          KILL(sail_int)(&zgsz3351);
          KILL(sail_int)(&zgsz3350);
          KILL(sail_int)(&zgsz3349);
        }
        {
          sail_int zgsz3352;
          CREATE(sail_int)(&zgsz3352);
          CONVERT_OF(sail_int, mach_int)(&zgsz3352, zcap_addr_width);
          sail_int zgsz3353;
          CREATE(sail_int)(&zgsz3353);
          CONVERT_OF(sail_int, mach_int)(&zgsz3353, zgaz3164);
          lbits zgsz3354;
          CREATE(lbits)(&zgsz3354);
          vector_subrange_lbits(&zgsz3354, ztop, zgsz3352, zgsz3353);
          zgaz3165 = CONVERT_OF(fbits, lbits)(zgsz3354, true);
          KILL(lbits)(&zgsz3354);
          KILL(sail_int)(&zgsz3353);
          KILL(sail_int)(&zgsz3352);
        }
      
      }
      struct ztuple_z8z5bv64zCz0z5bvz9 zgsz3355;
      CREATE(ztuple_z8z5bv64zCz0z5bvz9)(&zgsz3355);
      {
        uint64_t ztop2;
        ztop2 = zgaz3165;
        {
          bool zgaz3170;
          {
            bool zgaz3169;
            {
              int64_t zgaz3166;
              {
                sail_int zgsz3356;
                CREATE(sail_int)(&zgsz3356);
                CONVERT_OF(sail_int, mach_int)(&zgsz3356, zmaxE);
                sail_int zgsz3357;
                CREATE(sail_int)(&zgsz3357);
                CONVERT_OF(sail_int, mach_int)(&zgsz3357, 1l);
                sail_int zgsz3358;
                CREATE(sail_int)(&zgsz3358);
                sub_int(&zgsz3358, zgsz3356, zgsz3357);
                zgaz3166 = CONVERT_OF(mach_int, sail_int)(zgsz3358);
                KILL(sail_int)(&zgsz3358);
                KILL(sail_int)(&zgsz3357);
                KILL(sail_int)(&zgsz3356);
              }
              {
                sail_int zgsz3360;
                CREATE(sail_int)(&zgsz3360);
                CONVERT_OF(sail_int, mach_int)(&zgsz3360, zgaz3166);
                sail_int zgsz3359;
                CREATE(sail_int)(&zgsz3359);
                CONVERT_OF(sail_int, mach_int)(&zgsz3359, zE);
                zgaz3169 = lt(zgsz3359, zgsz3360);
                KILL(sail_int)(&zgsz3360);
                KILL(sail_int)(&zgsz3359);
              }
            
            }
            bool zgsz3364;
            if (zgaz3169) {
            int64_t zgaz3168;
            {
              uint64_t zgaz3167;
              {
                lbits zgsz3361;
                CREATE(lbits)(&zgsz3361);
                CONVERT_OF(lbits, fbits)(&zgsz3361, ztop2, UINT64_C(2) , true);
                lbits zgsz3362;
                CREATE(lbits)(&zgsz3362);
                CONVERT_OF(lbits, fbits)(&zgsz3362, zbase2, UINT64_C(2) , true);
                lbits zgsz3363;
                CREATE(lbits)(&zgsz3363);
                sub_bits(&zgsz3363, zgsz3361, zgsz3362);
                zgaz3167 = CONVERT_OF(fbits, lbits)(zgsz3363, true);
                KILL(lbits)(&zgsz3363);
                KILL(lbits)(&zgsz3362);
                KILL(lbits)(&zgsz3361);
              }
              zgaz3168 = ((mach_int) zgaz3167);
            
            }
            {
              sail_int zgsz3366;
              CREATE(sail_int)(&zgsz3366);
              CONVERT_OF(sail_int, mach_int)(&zgsz3366, 1l);
              sail_int zgsz3365;
              CREATE(sail_int)(&zgsz3365);
              CONVERT_OF(sail_int, mach_int)(&zgsz3365, zgaz3168);
              zgsz3364 = gt(zgsz3365, zgsz3366);
              KILL(sail_int)(&zgsz3366);
              KILL(sail_int)(&zgsz3365);
            }
          
            } else {  zgsz3364 = false;  }
            zgaz3170 = zgsz3364;
          
          }
          unit zgsz3367;
          if (zgaz3170) {
          fbits zgaz3172;
          {
            fbits zgaz3171;
            {
              sail_int zgsz3368;
              CREATE(sail_int)(&zgsz3368);
              CONVERT_OF(sail_int, mach_int)(&zgsz3368, zcap_addr_width);
              zgaz3171 = bitvector_access(ztop, zgsz3368);
              KILL(sail_int)(&zgsz3368);
            }
            zgaz3172 = sailgen_not_bit(zgaz3171);
          
          }
          {
            sail_int zgsz3369;
            CREATE(sail_int)(&zgsz3369);
            CONVERT_OF(sail_int, mach_int)(&zgsz3369, zcap_addr_width);
            update_lbits(&ztop, ztop, zgsz3369, zgaz3172);
            KILL(sail_int)(&zgsz3369);
          }
          zgsz3367 = UNIT;
        
          } else {  zgsz3367 = UNIT;  }
        
        }
        uint64_t zgaz3174;
        {
          int64_t zgaz3173;
          {
            sail_int zgsz3370;
            CREATE(sail_int)(&zgsz3370);
            CONVERT_OF(sail_int, mach_int)(&zgsz3370, zcap_addr_width);
            sail_int zgsz3371;
            CREATE(sail_int)(&zgsz3371);
            CONVERT_OF(sail_int, mach_int)(&zgsz3371, 1l);
            sail_int zgsz3372;
            CREATE(sail_int)(&zgsz3372);
            sub_int(&zgsz3372, zgsz3370, zgsz3371);
            zgaz3173 = CONVERT_OF(mach_int, sail_int)(zgsz3372);
            KILL(sail_int)(&zgsz3372);
            KILL(sail_int)(&zgsz3371);
            KILL(sail_int)(&zgsz3370);
          }
          {
            sail_int zgsz3373;
            CREATE(sail_int)(&zgsz3373);
            CONVERT_OF(sail_int, mach_int)(&zgsz3373, zgaz3173);
            sail_int zgsz3374;
            CREATE(sail_int)(&zgsz3374);
            CONVERT_OF(sail_int, mach_int)(&zgsz3374, 0l);
            lbits zgsz3375;
            CREATE(lbits)(&zgsz3375);
            vector_subrange_lbits(&zgsz3375, zbase, zgsz3373, zgsz3374);
            zgaz3174 = CONVERT_OF(fbits, lbits)(zgsz3375, true);
            KILL(lbits)(&zgsz3375);
            KILL(sail_int)(&zgsz3374);
            KILL(sail_int)(&zgsz3373);
          }
        
        }
        struct ztuple_z8z5bv64zCz0z5bvz9 zgsz3376;
        CREATE(ztuple_z8z5bv64zCz0z5bvz9)(&zgsz3376);
        zgsz3376.ztup0 = zgaz3174;
        COPY(lbits)(&((&zgsz3376)->ztup1), ztop);
        COPY(ztuple_z8z5bv64zCz0z5bvz9)(&zgsz3355, zgsz3376);
        KILL(ztuple_z8z5bv64zCz0z5bvz9)(&zgsz3376);
      
      
        goto finish_match_161;
      }
    case_162: ;
      sail_match_failure("getCapBoundsBits");
    finish_match_161: ;
      COPY(ztuple_z8z5bv64zCz0z5bvz9)(&zgsz3348, zgsz3355);
      KILL(ztuple_z8z5bv64zCz0z5bvz9)(&zgsz3355);
    
    
      goto finish_match_159;
    }
  case_160: ;
    sail_match_failure("getCapBoundsBits");
  finish_match_159: ;
    COPY(ztuple_z8z5bv64zCz0z5bvz9)(&zgsz3288, zgsz3348);
    KILL(ztuple_z8z5bv64zCz0z5bvz9)(&zgsz3348);
  
    KILL(lbits)(&ztop);
    KILL(lbits)(&zbase);
  
  
  
  
  
  
  
  
  
  
  
    goto finish_match_157;
  }
case_158: ;
  sail_match_failure("getCapBoundsBits");
finish_match_157: ;
  COPY(ztuple_z8z5bv64zCz0z5bvz9)((*(&zcbz340)), zgsz3288);
  KILL(ztuple_z8z5bv64zCz0z5bvz9)(&zgsz3288);


end_function_163: ;
  goto end_function_225;
end_block_exception_164: ;
  goto end_function_225;
end_function_225: ;
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

void sailgen_getCapBounds(struct ztuple_z8z5izCz0z5iz9 *rop, struct zCapability);


void sailgen_getCapBounds(struct ztuple_z8z5izCz0z5iz9 *zcbz341, struct zCapability zcap)
{
  __label__ case_167, finish_match_166, end_function_168, end_block_exception_169, end_function_224;

  struct ztuple_z8z5bv64zCz0z5bvz9 zgaz3196;
  CREATE(ztuple_z8z5bv64zCz0z5bvz9)(&zgaz3196);
  sailgen_getCapBoundsBits(&zgaz3196, zcap);
  struct ztuple_z8z5izCz0z5iz9 zgsz3380;
  CREATE(ztuple_z8z5izCz0z5iz9)(&zgsz3380);
  {
    uint64_t zbase;
    zbase = zgaz3196.ztup0;
    lbits ztop;
    CREATE(lbits)(&ztop);
    COPY(lbits)(&ztop, zgaz3196.ztup1);
    sail_int zgaz3197;
    CREATE(sail_int)(&zgaz3197);
    {
      lbits zgsz3381;
      CREATE(lbits)(&zgsz3381);
      CONVERT_OF(lbits, fbits)(&zgsz3381, zbase, UINT64_C(64) , true);
      sail_unsigned(&zgaz3197, zgsz3381);
      KILL(lbits)(&zgsz3381);
    }
    sail_int zgaz3198;
    CREATE(sail_int)(&zgaz3198);
    sail_unsigned(&zgaz3198, ztop);
    struct ztuple_z8z5izCz0z5iz9 zgsz3382;
    CREATE(ztuple_z8z5izCz0z5iz9)(&zgsz3382);
    COPY(sail_int)(&((&zgsz3382)->ztup0), zgaz3197);
    COPY(sail_int)(&((&zgsz3382)->ztup1), zgaz3198);
    COPY(ztuple_z8z5izCz0z5iz9)(&zgsz3380, zgsz3382);
    KILL(ztuple_z8z5izCz0z5iz9)(&zgsz3382);
    KILL(sail_int)(&zgaz3198);
    KILL(sail_int)(&zgaz3197);
    KILL(lbits)(&ztop);
  
    goto finish_match_166;
  }
case_167: ;
  sail_match_failure("getCapBounds");
finish_match_166: ;
  COPY(ztuple_z8z5izCz0z5iz9)((*(&zcbz341)), zgsz3380);
  KILL(ztuple_z8z5izCz0z5iz9)(&zgsz3380);
  KILL(ztuple_z8z5bv64zCz0z5bvz9)(&zgaz3196);
end_function_168: ;
  goto end_function_224;
end_block_exception_169: ;
  goto end_function_224;
end_function_224: ;
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

struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 sailgen_setCapBounds(struct zCapability, uint64_t, lbits);

struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 sailgen_setCapBounds(struct zCapability zcap, uint64_t zbase, lbits ztop)
{
  __label__ end_function_173, end_block_exception_174;

  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz342;
  lbits zext_base;
  CREATE(lbits)(&zext_base);
  {
    lbits zgsz3385;
    CREATE(lbits)(&zgsz3385);
    CONVERT_OF(lbits, fbits)(&zgsz3385, zbase, UINT64_C(64) , true);
    lbits zgsz3384;
    CREATE(lbits)(&zgsz3384);
    CONVERT_OF(lbits, fbits)(&zgsz3384, UINT64_C(0b0), UINT64_C(1) , true);
    append(&zext_base, zgsz3384, zgsz3385);
    KILL(lbits)(&zgsz3385);
    KILL(lbits)(&zgsz3384);
  }
  lbits zlength;
  CREATE(lbits)(&zlength);
  sub_bits(&zlength, ztop, zext_base);
  int64_t ze;
  {
    int64_t zgaz3233;
    {
      uint64_t zgaz3232;
      {
        int64_t zgaz3231;
        {
          sail_int zgsz3386;
          CREATE(sail_int)(&zgsz3386);
          CONVERT_OF(sail_int, mach_int)(&zgsz3386, zmantissa_width);
          sail_int zgsz3387;
          CREATE(sail_int)(&zgsz3387);
          CONVERT_OF(sail_int, mach_int)(&zgsz3387, 1l);
          sail_int zgsz3388;
          CREATE(sail_int)(&zgsz3388);
          sub_int(&zgsz3388, zgsz3386, zgsz3387);
          zgaz3231 = CONVERT_OF(mach_int, sail_int)(zgsz3388);
          KILL(sail_int)(&zgsz3388);
          KILL(sail_int)(&zgsz3387);
          KILL(sail_int)(&zgsz3386);
        }
        {
          sail_int zgsz3389;
          CREATE(sail_int)(&zgsz3389);
          CONVERT_OF(sail_int, mach_int)(&zgsz3389, zcap_addr_width);
          sail_int zgsz3390;
          CREATE(sail_int)(&zgsz3390);
          CONVERT_OF(sail_int, mach_int)(&zgsz3390, zgaz3231);
          lbits zgsz3391;
          CREATE(lbits)(&zgsz3391);
          vector_subrange_lbits(&zgsz3391, zlength, zgsz3389, zgsz3390);
          zgaz3232 = CONVERT_OF(fbits, lbits)(zgsz3391, true);
          KILL(lbits)(&zgsz3391);
          KILL(sail_int)(&zgsz3390);
          KILL(sail_int)(&zgsz3389);
        }
      
      }
      {
        lbits zgsz3392;
        CREATE(lbits)(&zgsz3392);
        CONVERT_OF(lbits, fbits)(&zgsz3392, zgaz3232, UINT64_C(52) , true);
        sail_int zgsz3393;
        CREATE(sail_int)(&zgsz3393);
        count_leading_zeros(&zgsz3393, zgsz3392);
        zgaz3233 = CONVERT_OF(mach_int, sail_int)(zgsz3393);
        KILL(sail_int)(&zgsz3393);
        KILL(lbits)(&zgsz3392);
      }
    
    }
    {
      sail_int zgsz3394;
      CREATE(sail_int)(&zgsz3394);
      CONVERT_OF(sail_int, mach_int)(&zgsz3394, zmaxE);
      sail_int zgsz3395;
      CREATE(sail_int)(&zgsz3395);
      CONVERT_OF(sail_int, mach_int)(&zgsz3395, zgaz3233);
      sail_int zgsz3396;
      CREATE(sail_int)(&zgsz3396);
      sub_int(&zgsz3396, zgsz3394, zgsz3395);
      ze = CONVERT_OF(mach_int, sail_int)(zgsz3396);
      KILL(sail_int)(&zgsz3396);
      KILL(sail_int)(&zgsz3395);
      KILL(sail_int)(&zgsz3394);
    }
  
  }
  bool zie;
  {
    bool zgaz3230;
    {
      sail_int zgsz3397;
      CREATE(sail_int)(&zgsz3397);
      CONVERT_OF(sail_int, mach_int)(&zgsz3397, ze);
      zgaz3230 = sailgen_zzz3zzzz7mzzJzzK0z3neq_int(zgsz3397, 0l);
      KILL(sail_int)(&zgsz3397);
    }
    bool zgsz3402;
    if (zgaz3230) {  zgsz3402 = true;  } else {
    fbits zgaz3229;
    {
      int64_t zgaz3228;
      {
        sail_int zgsz3398;
        CREATE(sail_int)(&zgsz3398);
        CONVERT_OF(sail_int, mach_int)(&zgsz3398, zmantissa_width);
        sail_int zgsz3399;
        CREATE(sail_int)(&zgsz3399);
        CONVERT_OF(sail_int, mach_int)(&zgsz3399, 2l);
        sail_int zgsz3400;
        CREATE(sail_int)(&zgsz3400);
        sub_int(&zgsz3400, zgsz3398, zgsz3399);
        zgaz3228 = CONVERT_OF(mach_int, sail_int)(zgsz3400);
        KILL(sail_int)(&zgsz3400);
        KILL(sail_int)(&zgsz3399);
        KILL(sail_int)(&zgsz3398);
      }
      {
        sail_int zgsz3401;
        CREATE(sail_int)(&zgsz3401);
        CONVERT_OF(sail_int, mach_int)(&zgsz3401, zgaz3228);
        zgaz3229 = bitvector_access(zlength, zgsz3401);
        KILL(sail_int)(&zgsz3401);
      }
    
    }
    zgsz3402 = eq_bit(zgaz3229, UINT64_C(1));
  
    }
    zie = zgsz3402;
  
  }
  uint64_t zBbits;
  {
    lbits zgsz3403;
    CREATE(lbits)(&zgsz3403);
    CONVERT_OF(lbits, fbits)(&zgsz3403, zbase, UINT64_C(64) , true);
    sail_int zgsz3404;
    CREATE(sail_int)(&zgsz3404);
    CONVERT_OF(sail_int, mach_int)(&zgsz3404, zmantissa_width);
    lbits zgsz3405;
    CREATE(lbits)(&zgsz3405);
    sail_truncate(&zgsz3405, zgsz3403, zgsz3404);
    zBbits = CONVERT_OF(fbits, lbits)(zgsz3405, true);
    KILL(lbits)(&zgsz3405);
    KILL(sail_int)(&zgsz3404);
    KILL(lbits)(&zgsz3403);
  }
  uint64_t zTbits;
  {
    sail_int zgsz3406;
    CREATE(sail_int)(&zgsz3406);
    CONVERT_OF(sail_int, mach_int)(&zgsz3406, zmantissa_width);
    lbits zgsz3407;
    CREATE(lbits)(&zgsz3407);
    sail_truncate(&zgsz3407, ztop, zgsz3406);
    zTbits = CONVERT_OF(fbits, lbits)(zgsz3407, true);
    KILL(lbits)(&zgsz3407);
    KILL(sail_int)(&zgsz3406);
  }
  bool zlostSignificantTop;
  zlostSignificantTop = false;
  bool zlostSignificantBase;
  zlostSignificantBase = false;
  bool zincE;
  zincE = false;
  {
    unit zgsz3408;
    if (zie) {
    uint64_t zB_ie;
    {
      uint64_t zgaz3223;
      {
        int64_t zgaz3222;
        {
          sail_int zgsz3409;
          CREATE(sail_int)(&zgsz3409);
          CONVERT_OF(sail_int, mach_int)(&zgsz3409, ze);
          sail_int zgsz3410;
          CREATE(sail_int)(&zgsz3410);
          CONVERT_OF(sail_int, mach_int)(&zgsz3410, 3l);
          sail_int zgsz3411;
          CREATE(sail_int)(&zgsz3411);
          add_int(&zgsz3411, zgsz3409, zgsz3410);
          zgaz3222 = CONVERT_OF(mach_int, sail_int)(zgsz3411);
          KILL(sail_int)(&zgsz3411);
          KILL(sail_int)(&zgsz3410);
          KILL(sail_int)(&zgsz3409);
        }
        {
          lbits zgsz3412;
          CREATE(lbits)(&zgsz3412);
          CONVERT_OF(lbits, fbits)(&zgsz3412, zbase, UINT64_C(64) , true);
          sail_int zgsz3413;
          CREATE(sail_int)(&zgsz3413);
          CONVERT_OF(sail_int, mach_int)(&zgsz3413, zgaz3222);
          lbits zgsz3414;
          CREATE(lbits)(&zgsz3414);
          shiftr(&zgsz3414, zgsz3412, zgsz3413);
          zgaz3223 = CONVERT_OF(fbits, lbits)(zgsz3414, true);
          KILL(lbits)(&zgsz3414);
          KILL(sail_int)(&zgsz3413);
          KILL(lbits)(&zgsz3412);
        }
      
      }
      int64_t zgaz3224;
      {
        sail_int zgsz3415;
        CREATE(sail_int)(&zgsz3415);
        CONVERT_OF(sail_int, mach_int)(&zgsz3415, zmantissa_width);
        sail_int zgsz3416;
        CREATE(sail_int)(&zgsz3416);
        CONVERT_OF(sail_int, mach_int)(&zgsz3416, 3l);
        sail_int zgsz3417;
        CREATE(sail_int)(&zgsz3417);
        sub_int(&zgsz3417, zgsz3415, zgsz3416);
        zgaz3224 = CONVERT_OF(mach_int, sail_int)(zgsz3417);
        KILL(sail_int)(&zgsz3417);
        KILL(sail_int)(&zgsz3416);
        KILL(sail_int)(&zgsz3415);
      }
      {
        lbits zgsz3418;
        CREATE(lbits)(&zgsz3418);
        CONVERT_OF(lbits, fbits)(&zgsz3418, zgaz3223, UINT64_C(64) , true);
        sail_int zgsz3419;
        CREATE(sail_int)(&zgsz3419);
        CONVERT_OF(sail_int, mach_int)(&zgsz3419, zgaz3224);
        lbits zgsz3420;
        CREATE(lbits)(&zgsz3420);
        sail_truncate(&zgsz3420, zgsz3418, zgsz3419);
        zB_ie = CONVERT_OF(fbits, lbits)(zgsz3420, true);
        KILL(lbits)(&zgsz3420);
        KILL(sail_int)(&zgsz3419);
        KILL(lbits)(&zgsz3418);
      }
    
    
    }
    uint64_t zT_ie;
    {
      lbits zgaz3220;
      CREATE(lbits)(&zgaz3220);
      {
        int64_t zgaz3219;
        {
          sail_int zgsz3421;
          CREATE(sail_int)(&zgsz3421);
          CONVERT_OF(sail_int, mach_int)(&zgsz3421, ze);
          sail_int zgsz3422;
          CREATE(sail_int)(&zgsz3422);
          CONVERT_OF(sail_int, mach_int)(&zgsz3422, 3l);
          sail_int zgsz3423;
          CREATE(sail_int)(&zgsz3423);
          add_int(&zgsz3423, zgsz3421, zgsz3422);
          zgaz3219 = CONVERT_OF(mach_int, sail_int)(zgsz3423);
          KILL(sail_int)(&zgsz3423);
          KILL(sail_int)(&zgsz3422);
          KILL(sail_int)(&zgsz3421);
        }
        {
          sail_int zgsz3424;
          CREATE(sail_int)(&zgsz3424);
          CONVERT_OF(sail_int, mach_int)(&zgsz3424, zgaz3219);
          shiftr(&zgaz3220, ztop, zgsz3424);
          KILL(sail_int)(&zgsz3424);
        }
      
      }
      int64_t zgaz3221;
      {
        sail_int zgsz3425;
        CREATE(sail_int)(&zgsz3425);
        CONVERT_OF(sail_int, mach_int)(&zgsz3425, zmantissa_width);
        sail_int zgsz3426;
        CREATE(sail_int)(&zgsz3426);
        CONVERT_OF(sail_int, mach_int)(&zgsz3426, 3l);
        sail_int zgsz3427;
        CREATE(sail_int)(&zgsz3427);
        sub_int(&zgsz3427, zgsz3425, zgsz3426);
        zgaz3221 = CONVERT_OF(mach_int, sail_int)(zgsz3427);
        KILL(sail_int)(&zgsz3427);
        KILL(sail_int)(&zgsz3426);
        KILL(sail_int)(&zgsz3425);
      }
      {
        sail_int zgsz3428;
        CREATE(sail_int)(&zgsz3428);
        CONVERT_OF(sail_int, mach_int)(&zgsz3428, zgaz3221);
        lbits zgsz3429;
        CREATE(lbits)(&zgsz3429);
        sail_truncate(&zgsz3429, zgaz3220, zgsz3428);
        zT_ie = CONVERT_OF(fbits, lbits)(zgsz3429, true);
        KILL(lbits)(&zgsz3429);
        KILL(sail_int)(&zgsz3428);
      }
    
      KILL(lbits)(&zgaz3220);
    }
    lbits zmaskLo;
    CREATE(lbits)(&zmaskLo);
    {
      int64_t zgaz3217;
      {
        sail_int zgsz3430;
        CREATE(sail_int)(&zgsz3430);
        CONVERT_OF(sail_int, mach_int)(&zgsz3430, 64l);
        sail_int zgsz3431;
        CREATE(sail_int)(&zgsz3431);
        CONVERT_OF(sail_int, mach_int)(&zgsz3431, 1l);
        sail_int zgsz3432;
        CREATE(sail_int)(&zgsz3432);
        add_int(&zgsz3432, zgsz3430, zgsz3431);
        zgaz3217 = CONVERT_OF(mach_int, sail_int)(zgsz3432);
        KILL(sail_int)(&zgsz3432);
        KILL(sail_int)(&zgsz3431);
        KILL(sail_int)(&zgsz3430);
      }
      sbits zgaz3218;
      {
        int64_t zgaz3216;
        {
          sail_int zgsz3433;
          CREATE(sail_int)(&zgsz3433);
          CONVERT_OF(sail_int, mach_int)(&zgsz3433, ze);
          sail_int zgsz3434;
          CREATE(sail_int)(&zgsz3434);
          CONVERT_OF(sail_int, mach_int)(&zgsz3434, 3l);
          sail_int zgsz3435;
          CREATE(sail_int)(&zgsz3435);
          add_int(&zgsz3435, zgsz3433, zgsz3434);
          zgaz3216 = CONVERT_OF(mach_int, sail_int)(zgsz3435);
          KILL(sail_int)(&zgsz3435);
          KILL(sail_int)(&zgsz3434);
          KILL(sail_int)(&zgsz3433);
        }
        {
          sail_int zgsz3436;
          CREATE(sail_int)(&zgsz3436);
          CONVERT_OF(sail_int, mach_int)(&zgsz3436, zgaz3216);
          lbits zgsz3437;
          CREATE(lbits)(&zgsz3437);
          sailgen_zzz3zzz3ones(&zgsz3437, zgsz3436);
          zgaz3218 = CONVERT_OF(sbits, lbits)(zgsz3437, true);
          KILL(lbits)(&zgsz3437);
          KILL(sail_int)(&zgsz3436);
        }
      
      }
      {
        lbits zgsz3439;
        CREATE(lbits)(&zgsz3439);
        CONVERT_OF(lbits, sbits)(&zgsz3439, zgaz3218, true);
        sail_int zgsz3438;
        CREATE(sail_int)(&zgsz3438);
        CONVERT_OF(sail_int, mach_int)(&zgsz3438, zgaz3217);
        sailgen_zzz3zzz3EXTZ(&zmaskLo, zgsz3438, zgsz3439);
        KILL(lbits)(&zgsz3439);
        KILL(sail_int)(&zgsz3438);
      }
    
    
    }
    {
      sail_int zgaz3200;
      CREATE(sail_int)(&zgaz3200);
      {
        lbits zgaz3199;
        CREATE(lbits)(&zgaz3199);
        and_bits(&zgaz3199, zext_base, zmaskLo);
        sail_unsigned(&zgaz3200, zgaz3199);
        KILL(lbits)(&zgaz3199);
      }
      zlostSignificantBase = sailgen_zzz3zzzz7mzzJzzK0z3neq_int(zgaz3200, 0l);
      unit zgsz3445;
      zgsz3445 = UNIT;
      KILL(sail_int)(&zgaz3200);
    }
    {
      sail_int zgaz3202;
      CREATE(sail_int)(&zgaz3202);
      {
        lbits zgaz3201;
        CREATE(lbits)(&zgaz3201);
        and_bits(&zgaz3201, ztop, zmaskLo);
        sail_unsigned(&zgaz3202, zgaz3201);
        KILL(lbits)(&zgaz3201);
      }
      zlostSignificantTop = sailgen_zzz3zzzz7mzzJzzK0z3neq_int(zgaz3202, 0l);
      unit zgsz3444;
      zgsz3444 = UNIT;
      KILL(sail_int)(&zgaz3202);
    }
    {
      unit zgsz3440;
      if (zlostSignificantTop) {
      {
        lbits zgsz3441;
        CREATE(lbits)(&zgsz3441);
        CONVERT_OF(lbits, fbits)(&zgsz3441, zT_ie, UINT64_C(11) , true);
        sail_int zgsz3442;
        CREATE(sail_int)(&zgsz3442);
        CONVERT_OF(sail_int, mach_int)(&zgsz3442, 1l);
        lbits zgsz3443;
        CREATE(lbits)(&zgsz3443);
        add_bits_int(&zgsz3443, zgsz3441, zgsz3442);
        zT_ie = CONVERT_OF(fbits, lbits)(zgsz3443, true);
        KILL(lbits)(&zgsz3443);
        KILL(sail_int)(&zgsz3442);
        KILL(lbits)(&zgsz3441);
      }
      zgsz3440 = UNIT;
      } else {  zgsz3440 = UNIT;  }
    }
    uint64_t zlen_ie;
    {
      lbits zgsz3446;
      CREATE(lbits)(&zgsz3446);
      CONVERT_OF(lbits, fbits)(&zgsz3446, zT_ie, UINT64_C(11) , true);
      lbits zgsz3447;
      CREATE(lbits)(&zgsz3447);
      CONVERT_OF(lbits, fbits)(&zgsz3447, zB_ie, UINT64_C(11) , true);
      lbits zgsz3448;
      CREATE(lbits)(&zgsz3448);
      sub_bits(&zgsz3448, zgsz3446, zgsz3447);
      zlen_ie = CONVERT_OF(fbits, lbits)(zgsz3448, true);
      KILL(lbits)(&zgsz3448);
      KILL(lbits)(&zgsz3447);
      KILL(lbits)(&zgsz3446);
    }
    {
      bool zgaz3205;
      {
        fbits zgaz3204;
        {
          int64_t zgaz3203;
          {
            sail_int zgsz3449;
            CREATE(sail_int)(&zgsz3449);
            CONVERT_OF(sail_int, mach_int)(&zgsz3449, zmantissa_width);
            sail_int zgsz3450;
            CREATE(sail_int)(&zgsz3450);
            CONVERT_OF(sail_int, mach_int)(&zgsz3450, 4l);
            sail_int zgsz3451;
            CREATE(sail_int)(&zgsz3451);
            sub_int(&zgsz3451, zgsz3449, zgsz3450);
            zgaz3203 = CONVERT_OF(mach_int, sail_int)(zgsz3451);
            KILL(sail_int)(&zgsz3451);
            KILL(sail_int)(&zgsz3450);
            KILL(sail_int)(&zgsz3449);
          }
          {
            sail_int zgsz3453;
            CREATE(sail_int)(&zgsz3453);
            CONVERT_OF(sail_int, mach_int)(&zgsz3453, zgaz3203);
            lbits zgsz3452;
            CREATE(lbits)(&zgsz3452);
            CONVERT_OF(lbits, fbits)(&zgsz3452, zlen_ie, UINT64_C(11) , true);
            zgaz3204 = bitvector_access(zgsz3452, zgsz3453);
            KILL(sail_int)(&zgsz3453);
            KILL(lbits)(&zgsz3452);
          }
        
        }
        zgaz3205 = eq_bit(zgaz3204, UINT64_C(1));
      
      }
      unit zgsz3458;
      if (zgaz3205) {
      __label__ case_172, finish_match_171;

      {
        zincE = true;
        unit zgsz3480;
        zgsz3480 = UNIT;
      }
      {
        bool zgsz3461;
        if (zlostSignificantBase) {  zgsz3461 = true;  } else {
        fbits zgaz3206;
        {
          sail_int zgsz3460;
          CREATE(sail_int)(&zgsz3460);
          CONVERT_OF(sail_int, mach_int)(&zgsz3460, 0l);
          lbits zgsz3459;
          CREATE(lbits)(&zgsz3459);
          CONVERT_OF(lbits, fbits)(&zgsz3459, zB_ie, UINT64_C(11) , true);
          zgaz3206 = bitvector_access(zgsz3459, zgsz3460);
          KILL(sail_int)(&zgsz3460);
          KILL(lbits)(&zgsz3459);
        }
        zgsz3461 = eq_bit(zgaz3206, UINT64_C(1));
      
        }
        zlostSignificantBase = zgsz3461;
        unit zgsz3479;
        zgsz3479 = UNIT;
      }
      {
        bool zgsz3464;
        if (zlostSignificantTop) {  zgsz3464 = true;  } else {
        fbits zgaz3207;
        {
          sail_int zgsz3463;
          CREATE(sail_int)(&zgsz3463);
          CONVERT_OF(sail_int, mach_int)(&zgsz3463, 0l);
          lbits zgsz3462;
          CREATE(lbits)(&zgsz3462);
          CONVERT_OF(lbits, fbits)(&zgsz3462, zT_ie, UINT64_C(11) , true);
          zgaz3207 = bitvector_access(zgsz3462, zgsz3463);
          KILL(sail_int)(&zgsz3463);
          KILL(lbits)(&zgsz3462);
        }
        zgsz3464 = eq_bit(zgaz3207, UINT64_C(1));
      
        }
        zlostSignificantTop = zgsz3464;
        unit zgsz3478;
        zgsz3478 = UNIT;
      }
      {
        uint64_t zgaz3209;
        {
          int64_t zgaz3208;
          {
            sail_int zgsz3465;
            CREATE(sail_int)(&zgsz3465);
            CONVERT_OF(sail_int, mach_int)(&zgsz3465, ze);
            sail_int zgsz3466;
            CREATE(sail_int)(&zgsz3466);
            CONVERT_OF(sail_int, mach_int)(&zgsz3466, 4l);
            sail_int zgsz3467;
            CREATE(sail_int)(&zgsz3467);
            add_int(&zgsz3467, zgsz3465, zgsz3466);
            zgaz3208 = CONVERT_OF(mach_int, sail_int)(zgsz3467);
            KILL(sail_int)(&zgsz3467);
            KILL(sail_int)(&zgsz3466);
            KILL(sail_int)(&zgsz3465);
          }
          {
            lbits zgsz3468;
            CREATE(lbits)(&zgsz3468);
            CONVERT_OF(lbits, fbits)(&zgsz3468, zbase, UINT64_C(64) , true);
            sail_int zgsz3469;
            CREATE(sail_int)(&zgsz3469);
            CONVERT_OF(sail_int, mach_int)(&zgsz3469, zgaz3208);
            lbits zgsz3470;
            CREATE(lbits)(&zgsz3470);
            shiftr(&zgsz3470, zgsz3468, zgsz3469);
            zgaz3209 = CONVERT_OF(fbits, lbits)(zgsz3470, true);
            KILL(lbits)(&zgsz3470);
            KILL(sail_int)(&zgsz3469);
            KILL(lbits)(&zgsz3468);
          }
        
        }
        int64_t zgaz3210;
        {
          sail_int zgsz3471;
          CREATE(sail_int)(&zgsz3471);
          CONVERT_OF(sail_int, mach_int)(&zgsz3471, zmantissa_width);
          sail_int zgsz3472;
          CREATE(sail_int)(&zgsz3472);
          CONVERT_OF(sail_int, mach_int)(&zgsz3472, 3l);
          sail_int zgsz3473;
          CREATE(sail_int)(&zgsz3473);
          sub_int(&zgsz3473, zgsz3471, zgsz3472);
          zgaz3210 = CONVERT_OF(mach_int, sail_int)(zgsz3473);
          KILL(sail_int)(&zgsz3473);
          KILL(sail_int)(&zgsz3472);
          KILL(sail_int)(&zgsz3471);
        }
        {
          lbits zgsz3474;
          CREATE(lbits)(&zgsz3474);
          CONVERT_OF(lbits, fbits)(&zgsz3474, zgaz3209, UINT64_C(64) , true);
          sail_int zgsz3475;
          CREATE(sail_int)(&zgsz3475);
          CONVERT_OF(sail_int, mach_int)(&zgsz3475, zgaz3210);
          lbits zgsz3476;
          CREATE(lbits)(&zgsz3476);
          sail_truncate(&zgsz3476, zgsz3474, zgsz3475);
          zB_ie = CONVERT_OF(fbits, lbits)(zgsz3476, true);
          KILL(lbits)(&zgsz3476);
          KILL(sail_int)(&zgsz3475);
          KILL(lbits)(&zgsz3474);
        }
        unit zgsz3477;
        zgsz3477 = UNIT;
      
      
      }
      int64_t zgaz3211;
      if (zlostSignificantTop) {  zgaz3211 = 1l;  } else {  zgaz3211 = 0l;  }
      unit zgsz3481;
      {
        int64_t zincT;
        zincT = zgaz3211;
        uint64_t zgaz3215;
        {
          lbits zgaz3213;
          CREATE(lbits)(&zgaz3213);
          {
            int64_t zgaz3212;
            {
              sail_int zgsz3482;
              CREATE(sail_int)(&zgsz3482);
              CONVERT_OF(sail_int, mach_int)(&zgsz3482, ze);
              sail_int zgsz3483;
              CREATE(sail_int)(&zgsz3483);
              CONVERT_OF(sail_int, mach_int)(&zgsz3483, 4l);
              sail_int zgsz3484;
              CREATE(sail_int)(&zgsz3484);
              add_int(&zgsz3484, zgsz3482, zgsz3483);
              zgaz3212 = CONVERT_OF(mach_int, sail_int)(zgsz3484);
              KILL(sail_int)(&zgsz3484);
              KILL(sail_int)(&zgsz3483);
              KILL(sail_int)(&zgsz3482);
            }
            {
              sail_int zgsz3485;
              CREATE(sail_int)(&zgsz3485);
              CONVERT_OF(sail_int, mach_int)(&zgsz3485, zgaz3212);
              shiftr(&zgaz3213, ztop, zgsz3485);
              KILL(sail_int)(&zgsz3485);
            }
          
          }
          int64_t zgaz3214;
          {
            sail_int zgsz3486;
            CREATE(sail_int)(&zgsz3486);
            CONVERT_OF(sail_int, mach_int)(&zgsz3486, zmantissa_width);
            sail_int zgsz3487;
            CREATE(sail_int)(&zgsz3487);
            CONVERT_OF(sail_int, mach_int)(&zgsz3487, 3l);
            sail_int zgsz3488;
            CREATE(sail_int)(&zgsz3488);
            sub_int(&zgsz3488, zgsz3486, zgsz3487);
            zgaz3214 = CONVERT_OF(mach_int, sail_int)(zgsz3488);
            KILL(sail_int)(&zgsz3488);
            KILL(sail_int)(&zgsz3487);
            KILL(sail_int)(&zgsz3486);
          }
          {
            sail_int zgsz3489;
            CREATE(sail_int)(&zgsz3489);
            CONVERT_OF(sail_int, mach_int)(&zgsz3489, zgaz3214);
            lbits zgsz3490;
            CREATE(lbits)(&zgsz3490);
            sail_truncate(&zgsz3490, zgaz3213, zgsz3489);
            zgaz3215 = CONVERT_OF(fbits, lbits)(zgsz3490, true);
            KILL(lbits)(&zgsz3490);
            KILL(sail_int)(&zgsz3489);
          }
        
          KILL(lbits)(&zgaz3213);
        }
        {
          lbits zgsz3491;
          CREATE(lbits)(&zgsz3491);
          CONVERT_OF(lbits, fbits)(&zgsz3491, zgaz3215, UINT64_C(11) , true);
          sail_int zgsz3492;
          CREATE(sail_int)(&zgsz3492);
          CONVERT_OF(sail_int, mach_int)(&zgsz3492, zincT);
          lbits zgsz3493;
          CREATE(lbits)(&zgsz3493);
          add_bits_int(&zgsz3493, zgsz3491, zgsz3492);
          zT_ie = CONVERT_OF(fbits, lbits)(zgsz3493, true);
          KILL(lbits)(&zgsz3493);
          KILL(sail_int)(&zgsz3492);
          KILL(lbits)(&zgsz3491);
        }
        zgsz3481 = UNIT;
      
      
        goto finish_match_171;
      }
    case_172: ;
      sail_match_failure("setCapBounds");
    finish_match_171: ;
      zgsz3458 = zgsz3481;
    
    
      } else {  zgsz3458 = UNIT;  }
    
    }
    {
      {
        lbits zgsz3454;
        CREATE(lbits)(&zgsz3454);
        CONVERT_OF(lbits, fbits)(&zgsz3454, zB_ie, UINT64_C(11) , true);
        lbits zgsz3455;
        CREATE(lbits)(&zgsz3455);
        CONVERT_OF(lbits, fbits)(&zgsz3455, UINT64_C(0b000), UINT64_C(3) , true);
        lbits zgsz3456;
        CREATE(lbits)(&zgsz3456);
        append(&zgsz3456, zgsz3454, zgsz3455);
        zBbits = CONVERT_OF(fbits, lbits)(zgsz3456, true);
        KILL(lbits)(&zgsz3456);
        KILL(lbits)(&zgsz3455);
        KILL(lbits)(&zgsz3454);
      }
      unit zgsz3457;
      zgsz3457 = UNIT;
    }
    {
      lbits zgsz3495;
      CREATE(lbits)(&zgsz3495);
      CONVERT_OF(lbits, fbits)(&zgsz3495, zT_ie, UINT64_C(11) , true);
      lbits zgsz3496;
      CREATE(lbits)(&zgsz3496);
      CONVERT_OF(lbits, fbits)(&zgsz3496, UINT64_C(0b000), UINT64_C(3) , true);
      lbits zgsz3497;
      CREATE(lbits)(&zgsz3497);
      append(&zgsz3497, zgsz3495, zgsz3496);
      zTbits = CONVERT_OF(fbits, lbits)(zgsz3497, true);
      KILL(lbits)(&zgsz3497);
      KILL(lbits)(&zgsz3496);
      KILL(lbits)(&zgsz3495);
    }
    zgsz3408 = UNIT;
  
    KILL(lbits)(&zmaskLo);
  
  
    } else {  zgsz3408 = UNIT;  }
  }
  struct zCapability znewCap;
  {
    uint64_t zgaz3227;
    {
      int64_t zgaz3226;
      if (zincE) {
      {
        sail_int zgsz3498;
        CREATE(sail_int)(&zgsz3498);
        CONVERT_OF(sail_int, mach_int)(&zgsz3498, ze);
        sail_int zgsz3499;
        CREATE(sail_int)(&zgsz3499);
        CONVERT_OF(sail_int, mach_int)(&zgsz3499, 1l);
        sail_int zgsz3500;
        CREATE(sail_int)(&zgsz3500);
        add_int(&zgsz3500, zgsz3498, zgsz3499);
        zgaz3226 = CONVERT_OF(mach_int, sail_int)(zgsz3500);
        KILL(sail_int)(&zgsz3500);
        KILL(sail_int)(&zgsz3499);
        KILL(sail_int)(&zgsz3498);
      }
      } else {  zgaz3226 = ze;  }
      {
        sail_int zgsz3501;
        CREATE(sail_int)(&zgsz3501);
        CONVERT_OF(sail_int, mach_int)(&zgsz3501, zgaz3226);
        zgaz3227 = sailgen_zzzz7lzzJzzK6z3to_bits(6l, zgsz3501);
        KILL(sail_int)(&zgsz3501);
      }
    
    }
    struct zCapability zgsz3502;
    zgsz3502 = zcap;
    zgsz3502.zB = zBbits;
    zgsz3502.zE = zgaz3227;
    zgsz3502.zT = zTbits;
    zgsz3502.zaddress = zbase;
    zgsz3502.zinternal_e = zie;
    znewCap = zgsz3502;
  
  
  }
  bool zexact;
  {
    bool zgaz3225;
    {
      bool zgsz3503;
      if (zlostSignificantBase) {  zgsz3503 = true;  } else {  zgsz3503 = zlostSignificantTop;  }
      zgaz3225 = zgsz3503;
    }
    zexact = not(zgaz3225);
  
  }
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgsz3504;
  zgsz3504.ztup0 = zexact;
  zgsz3504.ztup1 = znewCap;
  zcbz342 = zgsz3504;










  KILL(lbits)(&zlength);
  KILL(lbits)(&zext_base);
end_function_173: ;
  return zcbz342;
end_block_exception_174: ;
  struct zCapability zcbz358 = { .zuperms = UINT64_C(0xdeadc0de), .ztag = false, .zsealed = false, .zreserved = UINT64_C(0xdeadc0de), .zpermit_unseal = false, .zpermit_store_local_cap = false, .zpermit_store_cap = false, .zpermit_store = false, .zpermit_set_CID = false, .zpermit_seal = false, .zpermit_load_cap = false, .zpermit_load = false, .zpermit_execute = false, .zpermit_ccall = false, .zotype = UINT64_C(0xdeadc0de), .zinternal_e = false, .zglobal = false, .zflag_cap_mode = false, .zaddress = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zT = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz357 = { .ztup1 = zcbz358, .ztup0 = false };
  return zcbz357;
}

uint64_t sailgen_getCapPerms(struct zCapability);

uint64_t sailgen_getCapPerms(struct zCapability zcap)
{
  __label__ case_177, finish_match_176, end_function_178, end_block_exception_179;

  uint64_t zcbz343;
  uint64_t zgaz3235;
  {
    uint64_t zgaz3234;
    zgaz3234 = sailgen_getCapHardPerms(zcap);
    zgaz3235 = sailgen_zzzz7mzzJzzK15zzCzz0zz7nzzJzzK12z3EXTZ(15l, zgaz3234);
  
  }
  uint64_t zgsz3505;
  {
    uint64_t zperms;
    zperms = zgaz3235;
    uint64_t zgaz3237;
    {
      uint64_t zgaz3236;
      zgaz3236 = zcap.zuperms;
      {
        lbits zgsz3506;
        CREATE(lbits)(&zgsz3506);
        CONVERT_OF(lbits, fbits)(&zgsz3506, zgaz3236, UINT64_C(4) , true);
        lbits zgsz3507;
        CREATE(lbits)(&zgsz3507);
        CONVERT_OF(lbits, fbits)(&zgsz3507, zperms, UINT64_C(15) , true);
        lbits zgsz3508;
        CREATE(lbits)(&zgsz3508);
        append(&zgsz3508, zgsz3506, zgsz3507);
        zgaz3237 = CONVERT_OF(fbits, lbits)(zgsz3508, true);
        KILL(lbits)(&zgsz3508);
        KILL(lbits)(&zgsz3507);
        KILL(lbits)(&zgsz3506);
      }
    
    }
    {
      sbits zgsz3510;
      zgsz3510 = CONVERT_OF(sbits, fbits)(zgaz3237, UINT64_C(19) , true);
      zgsz3505 = sailgen_zzz3zzzz7mzzJzzK31z3EXTZ(31l, zgsz3510);
    
    }
  
  
    goto finish_match_176;
  }
case_177: ;
  sail_match_failure("getCapPerms");
finish_match_176: ;
  zcbz343 = zgsz3505;


end_function_178: ;
  return zcbz343;
end_block_exception_179: ;

  return UINT64_C(0xdeadc0de);
}

struct zCapability sailgen_setCapPerms(struct zCapability, uint64_t);

struct zCapability sailgen_setCapPerms(struct zCapability zcap, uint64_t zperms)
{
  __label__ end_function_181, end_block_exception_182;

  struct zCapability zcbz344;
  uint64_t zgaz3239;
  {
    uint64_t zgaz3238;
    {
      lbits zgsz3511;
      CREATE(lbits)(&zgsz3511);
      CONVERT_OF(lbits, fbits)(&zgsz3511, zperms, UINT64_C(31) , true);
      sail_int zgsz3512;
      CREATE(sail_int)(&zgsz3512);
      CONVERT_OF(sail_int, mach_int)(&zgsz3512, 30l);
      sail_int zgsz3513;
      CREATE(sail_int)(&zgsz3513);
      CONVERT_OF(sail_int, mach_int)(&zgsz3513, 15l);
      lbits zgsz3514;
      CREATE(lbits)(&zgsz3514);
      vector_subrange_lbits(&zgsz3514, zgsz3511, zgsz3512, zgsz3513);
      zgaz3238 = CONVERT_OF(fbits, lbits)(zgsz3514, true);
      KILL(lbits)(&zgsz3514);
      KILL(sail_int)(&zgsz3513);
      KILL(sail_int)(&zgsz3512);
      KILL(lbits)(&zgsz3511);
    }
    {
      lbits zgsz3515;
      CREATE(lbits)(&zgsz3515);
      CONVERT_OF(lbits, fbits)(&zgsz3515, zgaz3238, UINT64_C(16) , true);
      sail_int zgsz3516;
      CREATE(sail_int)(&zgsz3516);
      CONVERT_OF(sail_int, mach_int)(&zgsz3516, zuperms_width);
      lbits zgsz3517;
      CREATE(lbits)(&zgsz3517);
      sail_truncate(&zgsz3517, zgsz3515, zgsz3516);
      zgaz3239 = CONVERT_OF(fbits, lbits)(zgsz3517, true);
      KILL(lbits)(&zgsz3517);
      KILL(sail_int)(&zgsz3516);
      KILL(lbits)(&zgsz3515);
    }
  
  }
  bool zgaz3241;
  {
    fbits zgaz3240;
    {
      sail_int zgsz3519;
      CREATE(sail_int)(&zgsz3519);
      CONVERT_OF(sail_int, mach_int)(&zgsz3519, 11l);
      lbits zgsz3518;
      CREATE(lbits)(&zgsz3518);
      CONVERT_OF(lbits, fbits)(&zgsz3518, zperms, UINT64_C(31) , true);
      zgaz3240 = bitvector_access(zgsz3518, zgsz3519);
      KILL(sail_int)(&zgsz3519);
      KILL(lbits)(&zgsz3518);
    }
    zgaz3241 = sailgen_bit_to_bool(zgaz3240);
  
  }
  bool zgaz3243;
  {
    fbits zgaz3242;
    {
      sail_int zgsz3521;
      CREATE(sail_int)(&zgsz3521);
      CONVERT_OF(sail_int, mach_int)(&zgsz3521, 10l);
      lbits zgsz3520;
      CREATE(lbits)(&zgsz3520);
      CONVERT_OF(lbits, fbits)(&zgsz3520, zperms, UINT64_C(31) , true);
      zgaz3242 = bitvector_access(zgsz3520, zgsz3521);
      KILL(sail_int)(&zgsz3521);
      KILL(lbits)(&zgsz3520);
    }
    zgaz3243 = sailgen_bit_to_bool(zgaz3242);
  
  }
  bool zgaz3245;
  {
    fbits zgaz3244;
    {
      sail_int zgsz3523;
      CREATE(sail_int)(&zgsz3523);
      CONVERT_OF(sail_int, mach_int)(&zgsz3523, 9l);
      lbits zgsz3522;
      CREATE(lbits)(&zgsz3522);
      CONVERT_OF(lbits, fbits)(&zgsz3522, zperms, UINT64_C(31) , true);
      zgaz3244 = bitvector_access(zgsz3522, zgsz3523);
      KILL(sail_int)(&zgsz3523);
      KILL(lbits)(&zgsz3522);
    }
    zgaz3245 = sailgen_bit_to_bool(zgaz3244);
  
  }
  bool zgaz3247;
  {
    fbits zgaz3246;
    {
      sail_int zgsz3525;
      CREATE(sail_int)(&zgsz3525);
      CONVERT_OF(sail_int, mach_int)(&zgsz3525, 8l);
      lbits zgsz3524;
      CREATE(lbits)(&zgsz3524);
      CONVERT_OF(lbits, fbits)(&zgsz3524, zperms, UINT64_C(31) , true);
      zgaz3246 = bitvector_access(zgsz3524, zgsz3525);
      KILL(sail_int)(&zgsz3525);
      KILL(lbits)(&zgsz3524);
    }
    zgaz3247 = sailgen_bit_to_bool(zgaz3246);
  
  }
  bool zgaz3249;
  {
    fbits zgaz3248;
    {
      sail_int zgsz3527;
      CREATE(sail_int)(&zgsz3527);
      CONVERT_OF(sail_int, mach_int)(&zgsz3527, 7l);
      lbits zgsz3526;
      CREATE(lbits)(&zgsz3526);
      CONVERT_OF(lbits, fbits)(&zgsz3526, zperms, UINT64_C(31) , true);
      zgaz3248 = bitvector_access(zgsz3526, zgsz3527);
      KILL(sail_int)(&zgsz3527);
      KILL(lbits)(&zgsz3526);
    }
    zgaz3249 = sailgen_bit_to_bool(zgaz3248);
  
  }
  bool zgaz3251;
  {
    fbits zgaz3250;
    {
      sail_int zgsz3529;
      CREATE(sail_int)(&zgsz3529);
      CONVERT_OF(sail_int, mach_int)(&zgsz3529, 6l);
      lbits zgsz3528;
      CREATE(lbits)(&zgsz3528);
      CONVERT_OF(lbits, fbits)(&zgsz3528, zperms, UINT64_C(31) , true);
      zgaz3250 = bitvector_access(zgsz3528, zgsz3529);
      KILL(sail_int)(&zgsz3529);
      KILL(lbits)(&zgsz3528);
    }
    zgaz3251 = sailgen_bit_to_bool(zgaz3250);
  
  }
  bool zgaz3253;
  {
    fbits zgaz3252;
    {
      sail_int zgsz3531;
      CREATE(sail_int)(&zgsz3531);
      CONVERT_OF(sail_int, mach_int)(&zgsz3531, 5l);
      lbits zgsz3530;
      CREATE(lbits)(&zgsz3530);
      CONVERT_OF(lbits, fbits)(&zgsz3530, zperms, UINT64_C(31) , true);
      zgaz3252 = bitvector_access(zgsz3530, zgsz3531);
      KILL(sail_int)(&zgsz3531);
      KILL(lbits)(&zgsz3530);
    }
    zgaz3253 = sailgen_bit_to_bool(zgaz3252);
  
  }
  bool zgaz3255;
  {
    fbits zgaz3254;
    {
      sail_int zgsz3533;
      CREATE(sail_int)(&zgsz3533);
      CONVERT_OF(sail_int, mach_int)(&zgsz3533, 4l);
      lbits zgsz3532;
      CREATE(lbits)(&zgsz3532);
      CONVERT_OF(lbits, fbits)(&zgsz3532, zperms, UINT64_C(31) , true);
      zgaz3254 = bitvector_access(zgsz3532, zgsz3533);
      KILL(sail_int)(&zgsz3533);
      KILL(lbits)(&zgsz3532);
    }
    zgaz3255 = sailgen_bit_to_bool(zgaz3254);
  
  }
  bool zgaz3257;
  {
    fbits zgaz3256;
    {
      sail_int zgsz3535;
      CREATE(sail_int)(&zgsz3535);
      CONVERT_OF(sail_int, mach_int)(&zgsz3535, 3l);
      lbits zgsz3534;
      CREATE(lbits)(&zgsz3534);
      CONVERT_OF(lbits, fbits)(&zgsz3534, zperms, UINT64_C(31) , true);
      zgaz3256 = bitvector_access(zgsz3534, zgsz3535);
      KILL(sail_int)(&zgsz3535);
      KILL(lbits)(&zgsz3534);
    }
    zgaz3257 = sailgen_bit_to_bool(zgaz3256);
  
  }
  bool zgaz3259;
  {
    fbits zgaz3258;
    {
      sail_int zgsz3537;
      CREATE(sail_int)(&zgsz3537);
      CONVERT_OF(sail_int, mach_int)(&zgsz3537, 2l);
      lbits zgsz3536;
      CREATE(lbits)(&zgsz3536);
      CONVERT_OF(lbits, fbits)(&zgsz3536, zperms, UINT64_C(31) , true);
      zgaz3258 = bitvector_access(zgsz3536, zgsz3537);
      KILL(sail_int)(&zgsz3537);
      KILL(lbits)(&zgsz3536);
    }
    zgaz3259 = sailgen_bit_to_bool(zgaz3258);
  
  }
  bool zgaz3261;
  {
    fbits zgaz3260;
    {
      sail_int zgsz3539;
      CREATE(sail_int)(&zgsz3539);
      CONVERT_OF(sail_int, mach_int)(&zgsz3539, 1l);
      lbits zgsz3538;
      CREATE(lbits)(&zgsz3538);
      CONVERT_OF(lbits, fbits)(&zgsz3538, zperms, UINT64_C(31) , true);
      zgaz3260 = bitvector_access(zgsz3538, zgsz3539);
      KILL(sail_int)(&zgsz3539);
      KILL(lbits)(&zgsz3538);
    }
    zgaz3261 = sailgen_bit_to_bool(zgaz3260);
  
  }
  bool zgaz3263;
  {
    fbits zgaz3262;
    {
      sail_int zgsz3541;
      CREATE(sail_int)(&zgsz3541);
      CONVERT_OF(sail_int, mach_int)(&zgsz3541, 0l);
      lbits zgsz3540;
      CREATE(lbits)(&zgsz3540);
      CONVERT_OF(lbits, fbits)(&zgsz3540, zperms, UINT64_C(31) , true);
      zgaz3262 = bitvector_access(zgsz3540, zgsz3541);
      KILL(sail_int)(&zgsz3541);
      KILL(lbits)(&zgsz3540);
    }
    zgaz3263 = sailgen_bit_to_bool(zgaz3262);
  
  }
  struct zCapability zgsz3542;
  zgsz3542 = zcap;
  zgsz3542.zaccess_system_regs = zgaz3243;
  zgsz3542.zglobal = zgaz3263;
  zgsz3542.zpermit_ccall = zgaz3247;
  zgsz3542.zpermit_execute = zgaz3261;
  zgsz3542.zpermit_load = zgaz3259;
  zgsz3542.zpermit_load_cap = zgaz3255;
  zgsz3542.zpermit_seal = zgaz3249;
  zgsz3542.zpermit_set_CID = zgaz3241;
  zgsz3542.zpermit_store = zgaz3257;
  zgsz3542.zpermit_store_cap = zgaz3253;
  zgsz3542.zpermit_store_local_cap = zgaz3251;
  zgsz3542.zpermit_unseal = zgaz3245;
  zgsz3542.zuperms = zgaz3239;
  zcbz344 = zgsz3542;














end_function_181: ;
  return zcbz344;
end_block_exception_182: ;
  struct zCapability zcbz359 = { .zuperms = UINT64_C(0xdeadc0de), .ztag = false, .zsealed = false, .zreserved = UINT64_C(0xdeadc0de), .zpermit_unseal = false, .zpermit_store_local_cap = false, .zpermit_store_cap = false, .zpermit_store = false, .zpermit_set_CID = false, .zpermit_seal = false, .zpermit_load_cap = false, .zpermit_load = false, .zpermit_execute = false, .zpermit_ccall = false, .zotype = UINT64_C(0xdeadc0de), .zinternal_e = false, .zglobal = false, .zflag_cap_mode = false, .zaddress = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zT = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  return zcbz359;
}

uint64_t sailgen_getCapFlags(struct zCapability);

uint64_t sailgen_getCapFlags(struct zCapability zcap)
{
  __label__ end_function_184, end_block_exception_185;

  uint64_t zcbz345;
  bool zgaz3264;
  zgaz3264 = zcap.zflag_cap_mode;
  zcbz345 = sailgen_bool_to_bits(zgaz3264);

end_function_184: ;
  return zcbz345;
end_block_exception_185: ;

  return UINT64_C(0xdeadc0de);
}

bool sailgen_hasReservedOType(struct zCapability);

bool sailgen_hasReservedOType(struct zCapability zcap)
{
  __label__ end_function_187, end_block_exception_188;

  bool zcbz346;
  int64_t zgaz3266;
  {
    uint64_t zgaz3265;
    zgaz3265 = zcap.zotype;
    zgaz3266 = ((mach_int) zgaz3265);
  
  }
  {
    sail_int zgsz3544;
    CREATE(sail_int)(&zgsz3544);
    CONVERT_OF(sail_int, mach_int)(&zgsz3544, zmax_otype);
    sail_int zgsz3543;
    CREATE(sail_int)(&zgsz3543);
    CONVERT_OF(sail_int, mach_int)(&zgsz3543, zgaz3266);
    zcbz346 = gt(zgsz3543, zgsz3544);
    KILL(sail_int)(&zgsz3544);
    KILL(sail_int)(&zgsz3543);
  }

end_function_187: ;
  return zcbz346;
end_block_exception_188: ;

  return false;
}

uint64_t sailgen_getCapBaseBits(struct zCapability);

uint64_t sailgen_getCapBaseBits(struct zCapability zc)
{
  __label__ case_191, finish_match_190, end_function_192, end_block_exception_193;

  uint64_t zcbz347;
  struct ztuple_z8z5bv64zCz0z5bvz9 zgaz3267;
  CREATE(ztuple_z8z5bv64zCz0z5bvz9)(&zgaz3267);
  sailgen_getCapBoundsBits(&zgaz3267, zc);
  uint64_t zgsz3545;
  {
    uint64_t zbase;
    zbase = zgaz3267.ztup0;
    zgsz3545 = zbase;
  
    goto finish_match_190;
  }
case_191: ;
  sail_match_failure("getCapBaseBits");
finish_match_190: ;
  zcbz347 = zgsz3545;

  KILL(ztuple_z8z5bv64zCz0z5bvz9)(&zgaz3267);
end_function_192: ;
  return zcbz347;
end_block_exception_193: ;

  return UINT64_C(0xdeadc0de);
}

uint64_t sailgen_getCapOffsetBits(struct zCapability);

uint64_t sailgen_getCapOffsetBits(struct zCapability zc)
{
  __label__ case_196, finish_match_195, end_function_197, end_block_exception_198;

  uint64_t zcbz348;
  uint64_t zgaz3268;
  zgaz3268 = sailgen_getCapBaseBits(zc);
  uint64_t zgsz3547;
  {
    uint64_t zbase;
    zbase = zgaz3268;
    uint64_t zgaz3269;
    zgaz3269 = zc.zaddress;
    {
      lbits zgsz3549;
      CREATE(lbits)(&zgsz3549);
      CONVERT_OF(lbits, fbits)(&zgsz3549, zgaz3269, UINT64_C(64) , true);
      lbits zgsz3550;
      CREATE(lbits)(&zgsz3550);
      CONVERT_OF(lbits, fbits)(&zgsz3550, zbase, UINT64_C(64) , true);
      lbits zgsz3551;
      CREATE(lbits)(&zgsz3551);
      sub_bits(&zgsz3551, zgsz3549, zgsz3550);
      zgsz3547 = CONVERT_OF(fbits, lbits)(zgsz3551, true);
      KILL(lbits)(&zgsz3551);
      KILL(lbits)(&zgsz3550);
      KILL(lbits)(&zgsz3549);
    }
  
  
    goto finish_match_195;
  }
case_196: ;
  sail_match_failure("getCapOffsetBits");
finish_match_195: ;
  zcbz348 = zgsz3547;


end_function_197: ;
  return zcbz348;
end_block_exception_198: ;

  return UINT64_C(0xdeadc0de);
}

void sailgen_getCapLength(sail_int *rop, struct zCapability);

void sailgen_getCapLength(sail_int *zcbz349, struct zCapability zc)
{
  __label__ case_201, finish_match_200, end_function_202, end_block_exception_203, end_function_223;

  struct ztuple_z8z5izCz0z5iz9 zgaz3270;
  CREATE(ztuple_z8z5izCz0z5iz9)(&zgaz3270);
  sailgen_getCapBounds(&zgaz3270, zc);
  sail_int zgsz3552;
  CREATE(sail_int)(&zgsz3552);
  {
    sail_int zbase;
    CREATE(sail_int)(&zbase);
    COPY(sail_int)(&zbase, zgaz3270.ztup0);
    sail_int ztop;
    CREATE(sail_int)(&ztop);
    COPY(sail_int)(&ztop, zgaz3270.ztup1);
    {
      bool zgaz3273;
      {
        bool zgaz3272;
        {
          bool zgaz3271;
          zgaz3271 = zc.ztag;
          zgaz3272 = not(zgaz3271);
        
        }
        bool zgsz3553;
        if (zgaz3272) {  zgsz3553 = true;  } else {  zgsz3553 = gteq(ztop, zbase);  }
        zgaz3273 = zgsz3553;
      
      }
      unit zgsz3554;
      zgsz3554 = sail_assert(zgaz3273, "src/cheri_cap_common.sail 271:40 - 271:41");
    
    }
    sail_int zgaz3274;
    CREATE(sail_int)(&zgaz3274);
    sub_int(&zgaz3274, ztop, zbase);
    sail_int zgaz3275;
    CREATE(sail_int)(&zgaz3275);
    {
      sail_int zgsz3555;
      CREATE(sail_int)(&zgsz3555);
      CONVERT_OF(sail_int, mach_int)(&zgsz3555, zcap_len_width);
      pow2(&zgaz3275, zgsz3555);
      KILL(sail_int)(&zgsz3555);
    }
    emod_int(&zgsz3552, zgaz3274, zgaz3275);
    KILL(sail_int)(&zgaz3275);
    KILL(sail_int)(&zgaz3274);
    KILL(sail_int)(&ztop);
    KILL(sail_int)(&zbase);
    goto finish_match_200;
  }
case_201: ;
  sail_match_failure("getCapLength");
finish_match_200: ;
  COPY(sail_int)((*(&zcbz349)), zgsz3552);
  KILL(sail_int)(&zgsz3552);
  KILL(ztuple_z8z5izCz0z5iz9)(&zgaz3270);
end_function_202: ;
  goto end_function_223;
end_block_exception_203: ;
  goto end_function_223;
end_function_223: ;
}

bool sailgen_fastRepCheck(struct zCapability, uint64_t);

bool sailgen_fastRepCheck(struct zCapability zc, uint64_t zi)
{
  __label__ end_function_205, end_block_exception_206;

  bool zcbz350;
  int64_t zE;
  {
    uint64_t zgaz3289;
    zgaz3289 = zc.zE;
    zE = ((mach_int) zgaz3289);
  
  }
  bool zgaz3277;
  {
    int64_t zgaz3276;
    {
      sail_int zgsz3557;
      CREATE(sail_int)(&zgsz3557);
      CONVERT_OF(sail_int, mach_int)(&zgsz3557, zmaxE);
      sail_int zgsz3558;
      CREATE(sail_int)(&zgsz3558);
      CONVERT_OF(sail_int, mach_int)(&zgsz3558, 2l);
      sail_int zgsz3559;
      CREATE(sail_int)(&zgsz3559);
      sub_int(&zgsz3559, zgsz3557, zgsz3558);
      zgaz3276 = CONVERT_OF(mach_int, sail_int)(zgsz3559);
      KILL(sail_int)(&zgsz3559);
      KILL(sail_int)(&zgsz3558);
      KILL(sail_int)(&zgsz3557);
    }
    {
      sail_int zgsz3561;
      CREATE(sail_int)(&zgsz3561);
      CONVERT_OF(sail_int, mach_int)(&zgsz3561, zgaz3276);
      sail_int zgsz3560;
      CREATE(sail_int)(&zgsz3560);
      CONVERT_OF(sail_int, mach_int)(&zgsz3560, zE);
      zgaz3277 = gteq(zgsz3560, zgsz3561);
      KILL(sail_int)(&zgsz3561);
      KILL(sail_int)(&zgsz3560);
    }
  
  }
  if (zgaz3277) {  zcbz350 = true;  } else {
  int64_t zi_top;
  {
    uint64_t zgaz3288;
    {
      int64_t zgaz3287;
      {
        sail_int zgsz3562;
        CREATE(sail_int)(&zgsz3562);
        CONVERT_OF(sail_int, mach_int)(&zgsz3562, zE);
        sail_int zgsz3563;
        CREATE(sail_int)(&zgsz3563);
        CONVERT_OF(sail_int, mach_int)(&zgsz3563, zmantissa_width);
        sail_int zgsz3564;
        CREATE(sail_int)(&zgsz3564);
        add_int(&zgsz3564, zgsz3562, zgsz3563);
        zgaz3287 = CONVERT_OF(mach_int, sail_int)(zgsz3564);
        KILL(sail_int)(&zgsz3564);
        KILL(sail_int)(&zgsz3563);
        KILL(sail_int)(&zgsz3562);
      }
      {
        lbits zgsz3565;
        CREATE(lbits)(&zgsz3565);
        CONVERT_OF(lbits, fbits)(&zgsz3565, zi, UINT64_C(64) , true);
        sail_int zgsz3566;
        CREATE(sail_int)(&zgsz3566);
        CONVERT_OF(sail_int, mach_int)(&zgsz3566, zgaz3287);
        lbits zgsz3567;
        CREATE(lbits)(&zgsz3567);
        arith_shiftr(&zgsz3567, zgsz3565, zgsz3566);
        zgaz3288 = CONVERT_OF(fbits, lbits)(zgsz3567, true);
        KILL(lbits)(&zgsz3567);
        KILL(sail_int)(&zgsz3566);
        KILL(lbits)(&zgsz3565);
      }
    
    }
    zi_top = fast_signed(zgaz3288, 64);
  
  }
  uint64_t zi_mid;
  {
    uint64_t zgaz3286;
    {
      lbits zgsz3568;
      CREATE(lbits)(&zgsz3568);
      CONVERT_OF(lbits, fbits)(&zgsz3568, zi, UINT64_C(64) , true);
      sail_int zgsz3569;
      CREATE(sail_int)(&zgsz3569);
      CONVERT_OF(sail_int, mach_int)(&zgsz3569, zE);
      lbits zgsz3570;
      CREATE(lbits)(&zgsz3570);
      shiftr(&zgsz3570, zgsz3568, zgsz3569);
      zgaz3286 = CONVERT_OF(fbits, lbits)(zgsz3570, true);
      KILL(lbits)(&zgsz3570);
      KILL(sail_int)(&zgsz3569);
      KILL(lbits)(&zgsz3568);
    }
    {
      lbits zgsz3571;
      CREATE(lbits)(&zgsz3571);
      CONVERT_OF(lbits, fbits)(&zgsz3571, zgaz3286, UINT64_C(64) , true);
      sail_int zgsz3572;
      CREATE(sail_int)(&zgsz3572);
      CONVERT_OF(sail_int, mach_int)(&zgsz3572, zmantissa_width);
      lbits zgsz3573;
      CREATE(lbits)(&zgsz3573);
      sail_truncate(&zgsz3573, zgsz3571, zgsz3572);
      zi_mid = CONVERT_OF(fbits, lbits)(zgsz3573, true);
      KILL(lbits)(&zgsz3573);
      KILL(sail_int)(&zgsz3572);
      KILL(lbits)(&zgsz3571);
    }
  
  }
  uint64_t za_mid;
  {
    uint64_t zgaz3285;
    {
      uint64_t zgaz3284;
      zgaz3284 = zc.zaddress;
      {
        lbits zgsz3574;
        CREATE(lbits)(&zgsz3574);
        CONVERT_OF(lbits, fbits)(&zgsz3574, zgaz3284, UINT64_C(64) , true);
        sail_int zgsz3575;
        CREATE(sail_int)(&zgsz3575);
        CONVERT_OF(sail_int, mach_int)(&zgsz3575, zE);
        lbits zgsz3576;
        CREATE(lbits)(&zgsz3576);
        shiftr(&zgsz3576, zgsz3574, zgsz3575);
        zgaz3285 = CONVERT_OF(fbits, lbits)(zgsz3576, true);
        KILL(lbits)(&zgsz3576);
        KILL(sail_int)(&zgsz3575);
        KILL(lbits)(&zgsz3574);
      }
    
    }
    {
      lbits zgsz3577;
      CREATE(lbits)(&zgsz3577);
      CONVERT_OF(lbits, fbits)(&zgsz3577, zgaz3285, UINT64_C(64) , true);
      sail_int zgsz3578;
      CREATE(sail_int)(&zgsz3578);
      CONVERT_OF(sail_int, mach_int)(&zgsz3578, zmantissa_width);
      lbits zgsz3579;
      CREATE(lbits)(&zgsz3579);
      sail_truncate(&zgsz3579, zgsz3577, zgsz3578);
      za_mid = CONVERT_OF(fbits, lbits)(zgsz3579, true);
      KILL(lbits)(&zgsz3579);
      KILL(sail_int)(&zgsz3578);
      KILL(lbits)(&zgsz3577);
    }
  
  }
  uint64_t zB3;
  {
    uint64_t zgaz3283;
    zgaz3283 = zc.zB;
    {
      lbits zgsz3580;
      CREATE(lbits)(&zgsz3580);
      CONVERT_OF(lbits, fbits)(&zgsz3580, zgaz3283, UINT64_C(14) , true);
      sail_int zgsz3581;
      CREATE(sail_int)(&zgsz3581);
      CONVERT_OF(sail_int, mach_int)(&zgsz3581, 3l);
      lbits zgsz3582;
      CREATE(lbits)(&zgsz3582);
      sail_truncateLSB(&zgsz3582, zgsz3580, zgsz3581);
      zB3 = CONVERT_OF(fbits, lbits)(zgsz3582, true);
      KILL(lbits)(&zgsz3582);
      KILL(sail_int)(&zgsz3581);
      KILL(lbits)(&zgsz3580);
    }
  
  }
  uint64_t zR3;
  {
    lbits zgsz3583;
    CREATE(lbits)(&zgsz3583);
    CONVERT_OF(lbits, fbits)(&zgsz3583, zB3, UINT64_C(3) , true);
    lbits zgsz3584;
    CREATE(lbits)(&zgsz3584);
    CONVERT_OF(lbits, fbits)(&zgsz3584, UINT64_C(0b001), UINT64_C(3) , true);
    lbits zgsz3585;
    CREATE(lbits)(&zgsz3585);
    sub_bits(&zgsz3585, zgsz3583, zgsz3584);
    zR3 = CONVERT_OF(fbits, lbits)(zgsz3585, true);
    KILL(lbits)(&zgsz3585);
    KILL(lbits)(&zgsz3584);
    KILL(lbits)(&zgsz3583);
  }
  uint64_t zR;
  {
    uint64_t zgaz3282;
    {
      int64_t zgaz3281;
      {
        sail_int zgsz3586;
        CREATE(sail_int)(&zgsz3586);
        CONVERT_OF(sail_int, mach_int)(&zgsz3586, zmantissa_width);
        sail_int zgsz3587;
        CREATE(sail_int)(&zgsz3587);
        CONVERT_OF(sail_int, mach_int)(&zgsz3587, 3l);
        sail_int zgsz3588;
        CREATE(sail_int)(&zgsz3588);
        sub_int(&zgsz3588, zgsz3586, zgsz3587);
        zgaz3281 = CONVERT_OF(mach_int, sail_int)(zgsz3588);
        KILL(sail_int)(&zgsz3588);
        KILL(sail_int)(&zgsz3587);
        KILL(sail_int)(&zgsz3586);
      }
      zgaz3282 = sailgen_zzzz7nzzJzzK11z3zzeros_implicit(zgaz3281);
    
    }
    {
      lbits zgsz3589;
      CREATE(lbits)(&zgsz3589);
      CONVERT_OF(lbits, fbits)(&zgsz3589, zR3, UINT64_C(3) , true);
      lbits zgsz3590;
      CREATE(lbits)(&zgsz3590);
      CONVERT_OF(lbits, fbits)(&zgsz3590, zgaz3282, UINT64_C(11) , true);
      lbits zgsz3591;
      CREATE(lbits)(&zgsz3591);
      append(&zgsz3591, zgsz3589, zgsz3590);
      zR = CONVERT_OF(fbits, lbits)(zgsz3591, true);
      KILL(lbits)(&zgsz3591);
      KILL(lbits)(&zgsz3590);
      KILL(lbits)(&zgsz3589);
    }
  
  }
  uint64_t zdiff;
  {
    lbits zgsz3592;
    CREATE(lbits)(&zgsz3592);
    CONVERT_OF(lbits, fbits)(&zgsz3592, zR, UINT64_C(14) , true);
    lbits zgsz3593;
    CREATE(lbits)(&zgsz3593);
    CONVERT_OF(lbits, fbits)(&zgsz3593, za_mid, UINT64_C(14) , true);
    lbits zgsz3594;
    CREATE(lbits)(&zgsz3594);
    sub_bits(&zgsz3594, zgsz3592, zgsz3593);
    zdiff = CONVERT_OF(fbits, lbits)(zgsz3594, true);
    KILL(lbits)(&zgsz3594);
    KILL(lbits)(&zgsz3593);
    KILL(lbits)(&zgsz3592);
  }
  uint64_t zdiff1;
  {
    lbits zgsz3595;
    CREATE(lbits)(&zgsz3595);
    CONVERT_OF(lbits, fbits)(&zgsz3595, zdiff, UINT64_C(14) , true);
    sail_int zgsz3596;
    CREATE(sail_int)(&zgsz3596);
    CONVERT_OF(sail_int, mach_int)(&zgsz3596, 1l);
    lbits zgsz3597;
    CREATE(lbits)(&zgsz3597);
    sub_bits_int(&zgsz3597, zgsz3595, zgsz3596);
    zdiff1 = CONVERT_OF(fbits, lbits)(zgsz3597, true);
    KILL(lbits)(&zgsz3597);
    KILL(sail_int)(&zgsz3596);
    KILL(lbits)(&zgsz3595);
  }
  bool zgaz3278;
  {
    sail_int zgsz3599;
    CREATE(sail_int)(&zgsz3599);
    CONVERT_OF(sail_int, mach_int)(&zgsz3599, 0l);
    sail_int zgsz3598;
    CREATE(sail_int)(&zgsz3598);
    CONVERT_OF(sail_int, mach_int)(&zgsz3598, zi_top);
    zgaz3278 = eq_int(zgsz3598, zgsz3599);
    KILL(sail_int)(&zgsz3599);
    KILL(sail_int)(&zgsz3598);
  }
  if (zgaz3278) {  zcbz350 = sailgen_z8operatorz0zzzz7nzzJzzK14z3zI_uz9(zi_mid, zdiff1);  } else {
  bool zgaz3279;
  {
    sail_int zgsz3601;
    CREATE(sail_int)(&zgsz3601);
    CONVERT_OF(sail_int, mach_int)(&zgsz3601, -1l);
    sail_int zgsz3600;
    CREATE(sail_int)(&zgsz3600);
    CONVERT_OF(sail_int, mach_int)(&zgsz3600, zi_top);
    zgaz3279 = eq_int(zgsz3600, zgsz3601);
    KILL(sail_int)(&zgsz3601);
    KILL(sail_int)(&zgsz3600);
  }
  if (zgaz3279) {
  bool zgaz3280;
  zgaz3280 = sailgen_z8operatorz0zzzz7nzzJzzK14z3zKzJ_uz9(zi_mid, zdiff);
  bool zgsz3602;
  if (zgaz3280) {
  {
    lbits zgsz3604;
    CREATE(lbits)(&zgsz3604);
    CONVERT_OF(lbits, fbits)(&zgsz3604, za_mid, UINT64_C(14) , true);
    lbits zgsz3603;
    CREATE(lbits)(&zgsz3603);
    CONVERT_OF(lbits, fbits)(&zgsz3603, zR, UINT64_C(14) , true);
    zgsz3602 = neq_bits(zgsz3603, zgsz3604);
    KILL(lbits)(&zgsz3604);
    KILL(lbits)(&zgsz3603);
  }
  } else {  zgsz3602 = false;  }
  zcbz350 = zgsz3602;

  } else {  zcbz350 = false;  }

  }









  }


end_function_205: ;
  return zcbz350;
end_block_exception_206: ;

  return false;
}

void sailgen_capToString(sail_string *rop, struct zCapability);

void sailgen_capToString(sail_string *zcbz351, struct zCapability zcap)
{
  __label__ case_209, finish_match_208, end_function_210, end_block_exception_211, end_function_222;

  sail_int zlen;
  CREATE(sail_int)(&zlen);
  sailgen_getCapLength(&zlen, zcap);
  if (have_exception) {
  KILL(sail_int)(&zlen);
  goto end_block_exception_211;
  }
  sail_string zlen_str;
  CREATE(sail_string)(&zlen_str);
  {
    lbits zgaz3319;
    CREATE(lbits)(&zgaz3319);
    {
      int64_t zgaz3318;
      {
        sail_int zgsz3605;
        CREATE(sail_int)(&zgsz3605);
        CONVERT_OF(sail_int, mach_int)(&zgsz3605, zcap_len_width);
        sail_int zgsz3606;
        CREATE(sail_int)(&zgsz3606);
        CONVERT_OF(sail_int, mach_int)(&zgsz3606, 3l);
        sail_int zgsz3607;
        CREATE(sail_int)(&zgsz3607);
        add_int(&zgsz3607, zgsz3605, zgsz3606);
        zgaz3318 = CONVERT_OF(mach_int, sail_int)(zgsz3607);
        KILL(sail_int)(&zgsz3607);
        KILL(sail_int)(&zgsz3606);
        KILL(sail_int)(&zgsz3605);
      }
      sailgen_zzzz7lzzJzzK68z3to_bits(&zgaz3319, zgaz3318, zlen);
    
    }
    string_of_lbits(&zlen_str, zgaz3319);
    KILL(lbits)(&zgaz3319);
  }
  uint64_t zgaz3293;
  {
    bool zgaz3290;
    zgaz3290 = sailgen_hasReservedOType(zcap);
    if (zgaz3290) {
    uint64_t zgaz3291;
    zgaz3291 = zcap.zotype;
    zgaz3293 = sailgen_zzzz7mzzJzzK64zzCzz0zz7nzzJzzK18z3EXTS(64l, zgaz3291);
  
    } else {
    uint64_t zgaz3292;
    zgaz3292 = zcap.zotype;
    zgaz3293 = sailgen_zzzz7mzzJzzK64zzCzz0zz7nzzJzzK18z3EXTZ(64l, zgaz3292);
  
    }
  
  }
  sail_string zgsz3608;
  CREATE(sail_string)(&zgsz3608);
  {
    uint64_t zotype64;
    zotype64 = zgaz3293;
    sail_string zgaz3317;
    CREATE(sail_string)(&zgaz3317);
    {
      sail_string zgaz3315;
      CREATE(sail_string)(&zgaz3315);
      {
        bool zgaz3294;
        zgaz3294 = zcap.ztag;
        if (zgaz3294) {  COPY(sail_string)(&zgaz3315, "1");  } else {  COPY(sail_string)(&zgaz3315, "0");  }
      
      }
      sail_string zgaz3316;
      CREATE(sail_string)(&zgaz3316);
      {
        sail_string zgaz3314;
        CREATE(sail_string)(&zgaz3314);
        {
          sail_string zgaz3312;
          CREATE(sail_string)(&zgaz3312);
          {
            bool zgaz3295;
            zgaz3295 = zcap.zsealed;
            if (zgaz3295) {  COPY(sail_string)(&zgaz3312, "1");  } else {  COPY(sail_string)(&zgaz3312, "0");  }
          
          }
          sail_string zgaz3313;
          CREATE(sail_string)(&zgaz3313);
          {
            sail_string zgaz3311;
            CREATE(sail_string)(&zgaz3311);
            {
              sail_string zgaz3309;
              CREATE(sail_string)(&zgaz3309);
              {
                uint64_t zgaz3297;
                {
                  uint64_t zgaz3296;
                  zgaz3296 = sailgen_getCapPerms(zcap);
                  {
                    lbits zgsz3609;
                    CREATE(lbits)(&zgsz3609);
                    CONVERT_OF(lbits, fbits)(&zgsz3609, UINT64_C(0b0), UINT64_C(1) , true);
                    lbits zgsz3610;
                    CREATE(lbits)(&zgsz3610);
                    CONVERT_OF(lbits, fbits)(&zgsz3610, zgaz3296, UINT64_C(31) , true);
                    lbits zgsz3611;
                    CREATE(lbits)(&zgsz3611);
                    append(&zgsz3611, zgsz3609, zgsz3610);
                    zgaz3297 = CONVERT_OF(fbits, lbits)(zgsz3611, true);
                    KILL(lbits)(&zgsz3611);
                    KILL(lbits)(&zgsz3610);
                    KILL(lbits)(&zgsz3609);
                  }
                
                }
                {
                  lbits zgsz3612;
                  CREATE(lbits)(&zgsz3612);
                  CONVERT_OF(lbits, fbits)(&zgsz3612, zgaz3297, UINT64_C(32) , true);
                  string_of_lbits(&zgaz3309, zgsz3612);
                  KILL(lbits)(&zgsz3612);
                }
              
              }
              sail_string zgaz3310;
              CREATE(sail_string)(&zgaz3310);
              {
                sail_string zgaz3308;
                CREATE(sail_string)(&zgaz3308);
                {
                  sail_string zgaz3306;
                  CREATE(sail_string)(&zgaz3306);
                  {
                    lbits zgsz3613;
                    CREATE(lbits)(&zgsz3613);
                    CONVERT_OF(lbits, fbits)(&zgsz3613, zotype64, UINT64_C(64) , true);
                    string_of_lbits(&zgaz3306, zgsz3613);
                    KILL(lbits)(&zgsz3613);
                  }
                  sail_string zgaz3307;
                  CREATE(sail_string)(&zgaz3307);
                  {
                    sail_string zgaz3305;
                    CREATE(sail_string)(&zgaz3305);
                    {
                      sail_string zgaz3303;
                      CREATE(sail_string)(&zgaz3303);
                      {
                        uint64_t zgaz3298;
                        zgaz3298 = sailgen_getCapOffsetBits(zcap);
                        {
                          lbits zgsz3614;
                          CREATE(lbits)(&zgsz3614);
                          CONVERT_OF(lbits, fbits)(&zgsz3614, zgaz3298, UINT64_C(64) , true);
                          string_of_lbits(&zgaz3303, zgsz3614);
                          KILL(lbits)(&zgsz3614);
                        }
                      
                      }
                      sail_string zgaz3304;
                      CREATE(sail_string)(&zgaz3304);
                      {
                        sail_string zgaz3302;
                        CREATE(sail_string)(&zgaz3302);
                        {
                          sail_string zgaz3300;
                          CREATE(sail_string)(&zgaz3300);
                          {
                            uint64_t zgaz3299;
                            zgaz3299 = sailgen_getCapBaseBits(zcap);
                            {
                              lbits zgsz3615;
                              CREATE(lbits)(&zgsz3615);
                              CONVERT_OF(lbits, fbits)(&zgsz3615, zgaz3299, UINT64_C(64) , true);
                              string_of_lbits(&zgaz3300, zgsz3615);
                              KILL(lbits)(&zgsz3615);
                            }
                          
                          }
                          sail_string zgaz3301;
                          CREATE(sail_string)(&zgaz3301);
                          concat_str(&zgaz3301, " length:", zlen_str);
                          concat_str(&zgaz3302, zgaz3300, zgaz3301);
                          KILL(sail_string)(&zgaz3301);
                          KILL(sail_string)(&zgaz3300);
                        }
                        concat_str(&zgaz3304, " base:", zgaz3302);
                        KILL(sail_string)(&zgaz3302);
                      }
                      concat_str(&zgaz3305, zgaz3303, zgaz3304);
                      KILL(sail_string)(&zgaz3304);
                      KILL(sail_string)(&zgaz3303);
                    }
                    concat_str(&zgaz3307, " offset:", zgaz3305);
                    KILL(sail_string)(&zgaz3305);
                  }
                  concat_str(&zgaz3308, zgaz3306, zgaz3307);
                  KILL(sail_string)(&zgaz3307);
                  KILL(sail_string)(&zgaz3306);
                }
                concat_str(&zgaz3310, " type:", zgaz3308);
                KILL(sail_string)(&zgaz3308);
              }
              concat_str(&zgaz3311, zgaz3309, zgaz3310);
              KILL(sail_string)(&zgaz3310);
              KILL(sail_string)(&zgaz3309);
            }
            concat_str(&zgaz3313, " perms:", zgaz3311);
            KILL(sail_string)(&zgaz3311);
          }
          concat_str(&zgaz3314, zgaz3312, zgaz3313);
          KILL(sail_string)(&zgaz3313);
          KILL(sail_string)(&zgaz3312);
        }
        concat_str(&zgaz3316, " s:", zgaz3314);
        KILL(sail_string)(&zgaz3314);
      }
      concat_str(&zgaz3317, zgaz3315, zgaz3316);
      KILL(sail_string)(&zgaz3316);
      KILL(sail_string)(&zgaz3315);
    }
    concat_str(&zgsz3608, " t:", zgaz3317);
    KILL(sail_string)(&zgaz3317);
  
    goto finish_match_208;
  }
case_209: ;
  sail_match_failure("capToString");
finish_match_208: ;
  COPY(sail_string)((*(&zcbz351)), zgsz3608);
  KILL(sail_string)(&zgsz3608);

  KILL(sail_string)(&zlen_str);
  KILL(sail_int)(&zlen);
end_function_210: ;
  goto end_function_222;
end_block_exception_211: ;
  goto end_function_222;
end_function_222: ;
}

// register TestCap
struct zCapability zTestCap;

// register TestAddr
uint64_t zTestAddr;

// register TestLen
lbits zTestLen;

unit sailgen_main(unit);


unit sailgen_main(unit zgsz3617)
{
  __label__ end_function_217, end_block_exception_218;

  unit zcbz352;
  {
    zTestCap = znull_cap;
    unit zgsz3618;
    zgsz3618 = UNIT;
  }
  lbits zbits1;
  CREATE(lbits)(&zbits1);
  sailgen_capToBits(&zbits1, zTestCap);
  lbits zbits2;
  CREATE(lbits)(&zbits2);
  sailgen_capToMemBits(&zbits2, zTestCap);
  struct zCapability zcap1;
  {
    bool zgaz3324;
    zgaz3324 = zTestCap.ztag;
    zcap1 = sailgen_capBitsToCapability(zgaz3324, zbits1);
  
  }
  struct zCapability zcap2;
  {
    bool zgaz3323;
    zgaz3323 = zTestCap.ztag;
    zcap2 = sailgen_memBitsToCapability(zgaz3323, zbits2);
  
  }
  bool zrep;
  zrep = sailgen_fastRepCheck(zTestCap, zTestAddr);
  {
    __label__ case_214, finish_match_213;

    struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3320;
    zgaz3320 = sailgen_setCapBounds(zTestCap, zTestAddr, zTestLen);
    unit zgsz3619;
    {
      bool ztup__0;
      ztup__0 = zgaz3320.ztup0;
      struct zCapability ztup__1;
      ztup__1 = zgaz3320.ztup1;
      bool zexact;
      zexact = ztup__0;
      struct zCapability zcap;
      zcap = ztup__1;
      zgsz3619 = UNIT;
    
    
    
    
      goto finish_match_213;
    }
  case_214: ;
    sail_match_failure("main");
  finish_match_213: ;
    unit zgsz3624;
    zgsz3624 = zgsz3619;
  
  
  }
  {
    __label__ case_216, finish_match_215;

    struct ztuple_z8z5izCz0z5iz9 zgaz3321;
    CREATE(ztuple_z8z5izCz0z5iz9)(&zgaz3321);
    sailgen_getCapBounds(&zgaz3321, zTestCap);
    unit zgsz3621;
    {
      sail_int ztup__0;
      CREATE(sail_int)(&ztup__0);
      COPY(sail_int)(&ztup__0, zgaz3321.ztup0);
      sail_int ztup__1;
      CREATE(sail_int)(&ztup__1);
      COPY(sail_int)(&ztup__1, zgaz3321.ztup1);
      sail_int zbase;
      CREATE(sail_int)(&zbase);
      COPY(sail_int)(&zbase, ztup__0);
      sail_int ztop;
      CREATE(sail_int)(&ztop);
      COPY(sail_int)(&ztop, ztup__1);
      zgsz3621 = UNIT;
      KILL(sail_int)(&ztop);
      KILL(sail_int)(&zbase);
      KILL(sail_int)(&ztup__1);
      KILL(sail_int)(&ztup__0);
      goto finish_match_215;
    }
  case_216: ;
    sail_match_failure("main");
  finish_match_215: ;
    unit zgsz3623;
    zgsz3623 = zgsz3621;
  
    KILL(ztuple_z8z5izCz0z5iz9)(&zgaz3321);
  }
  uint64_t zflags;
  zflags = sailgen_getCapFlags(zTestCap);
  struct zCapability znew_cap;
  {
    uint64_t zgaz3322;
    zgaz3322 = sailgen_zzzz7nzzJzzK31z3ones(31l);
    znew_cap = sailgen_setCapPerms(zTestCap, zgaz3322);
  
  }
  sail_string zcap_str;
  CREATE(sail_string)(&zcap_str);
  sailgen_capToString(&zcap_str, zTestCap);
  if (have_exception) {
  KILL(lbits)(&zbits1);
  KILL(lbits)(&zbits2);





  KILL(sail_string)(&zcap_str);
  goto end_block_exception_218;
  }
  zcbz352 = UNIT;
  KILL(sail_string)(&zcap_str);





  KILL(lbits)(&zbits2);
  KILL(lbits)(&zbits1);
end_function_217: ;
  return zcbz352;
end_block_exception_218: ;

  return UNIT;
}

unit sailgen_initializze_registers(unit);

unit sailgen_initializze_registers(unit zgsz3625)
{
  __label__ end_function_220, end_block_exception_221;

  unit zcbz353;
  {
    zTestCap = sailgen_undefined_Capability(UNIT);
    unit zgsz3629;
    zgsz3629 = UNIT;
  }
  {
    {
      sail_int zgsz3626;
      CREATE(sail_int)(&zgsz3626);
      CONVERT_OF(sail_int, mach_int)(&zgsz3626, 64l);
      lbits zgsz3627;
      CREATE(lbits)(&zgsz3627);
      UNDEFINED(lbits)(&zgsz3627, zgsz3626);
      zTestAddr = CONVERT_OF(fbits, lbits)(zgsz3627, true);
      KILL(lbits)(&zgsz3627);
      KILL(sail_int)(&zgsz3626);
    }
    unit zgsz3628;
    zgsz3628 = UNIT;
  }
  int64_t zgaz3325;
  {
    sail_int zgsz3630;
    CREATE(sail_int)(&zgsz3630);
    CONVERT_OF(sail_int, mach_int)(&zgsz3630, 64l);
    sail_int zgsz3631;
    CREATE(sail_int)(&zgsz3631);
    CONVERT_OF(sail_int, mach_int)(&zgsz3631, 1l);
    sail_int zgsz3632;
    CREATE(sail_int)(&zgsz3632);
    add_int(&zgsz3632, zgsz3630, zgsz3631);
    zgaz3325 = CONVERT_OF(mach_int, sail_int)(zgsz3632);
    KILL(sail_int)(&zgsz3632);
    KILL(sail_int)(&zgsz3631);
    KILL(sail_int)(&zgsz3630);
  }
  {
    sail_int zgsz3633;
    CREATE(sail_int)(&zgsz3633);
    CONVERT_OF(sail_int, mach_int)(&zgsz3633, zgaz3325);
    UNDEFINED(lbits)(&zTestLen, zgsz3633);
    KILL(sail_int)(&zgsz3633);
  }
  zcbz353 = UNIT;

end_function_220: ;
  return zcbz353;
end_block_exception_221: ;

  return UNIT;
}

void model_init(void)
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

void model_fini(void)
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

int model_main(int argc, char *argv[])
{
  model_init();
  if (process_arguments(argc, argv)) exit(EXIT_FAILURE);
  sailgen_main(UNIT);
  model_fini();
  return EXIT_SUCCESS;
}


