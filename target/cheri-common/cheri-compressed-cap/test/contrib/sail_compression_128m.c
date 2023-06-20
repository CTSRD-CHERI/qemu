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

// enum signal
enum zsignal { zLOW, zHIGH };

static bool eq_zsignal(enum zsignal op1, enum zsignal op2) { return op1 == op2; }

static enum zsignal UNDEFINED(zsignal)(unit u) { return zLOW; }

// enum read_kind
enum zread_kind { zRead_plain, zRead_reserve, zRead_acquire, zRead_exclusive, zRead_exclusive_acquire, zRead_stream, zRead_ifetch, zRead_RISCV_acquire, zRead_RISCV_strong_acquire, zRead_RISCV_reserved, zRead_RISCV_reserved_acquire, zRead_RISCV_reserved_strong_acquire, zRead_X86_locked };

static bool eq_zread_kind(enum zread_kind op1, enum zread_kind op2) { return op1 == op2; }

static enum zread_kind UNDEFINED(zread_kind)(unit u) { return zRead_plain; }

// struct tuple_(%bv1, %bv1, %bv5, %bv5, %bv5)
struct ztuple_z8z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5zCz0z5bv5z9 {
  uint64_t ztup0;
  uint64_t ztup1;
  uint64_t ztup2;
  uint64_t ztup3;
  uint64_t ztup4;
};

static void COPY(ztuple_z8z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5zCz0z5bv5z9)(struct ztuple_z8z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5zCz0z5bv5z9 *rop, const struct ztuple_z8z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5zCz0z5bv5z9 op) {
  rop->ztup0 = op.ztup0;
  rop->ztup1 = op.ztup1;
  rop->ztup2 = op.ztup2;
  rop->ztup3 = op.ztup3;
  rop->ztup4 = op.ztup4;
}

static bool EQUAL(ztuple_z8z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5zCz0z5bv5z9)(struct ztuple_z8z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5zCz0z5bv5z9 op1, struct ztuple_z8z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5zCz0z5bv5z9 op2) {
  return EQUAL(fbits)(op1.ztup0, op2.ztup0) && EQUAL(fbits)(op1.ztup1, op2.ztup1) && EQUAL(fbits)(op1.ztup2, op2.ztup2) && EQUAL(fbits)(op1.ztup3, op2.ztup3) && EQUAL(fbits)(op1.ztup4, op2.ztup4);
}


// struct tuple_(%bv1, %bv12, %bv5, %bv5)
struct ztuple_z8z5bv1zCz0z5bv12zCz0z5bv5zCz0z5bv5z9 {
  uint64_t ztup0;
  uint64_t ztup1;
  uint64_t ztup2;
  uint64_t ztup3;
};

static void COPY(ztuple_z8z5bv1zCz0z5bv12zCz0z5bv5zCz0z5bv5z9)(struct ztuple_z8z5bv1zCz0z5bv12zCz0z5bv5zCz0z5bv5z9 *rop, const struct ztuple_z8z5bv1zCz0z5bv12zCz0z5bv5zCz0z5bv5z9 op) {
  rop->ztup0 = op.ztup0;
  rop->ztup1 = op.ztup1;
  rop->ztup2 = op.ztup2;
  rop->ztup3 = op.ztup3;
}

static bool EQUAL(ztuple_z8z5bv1zCz0z5bv12zCz0z5bv5zCz0z5bv5z9)(struct ztuple_z8z5bv1zCz0z5bv12zCz0z5bv5zCz0z5bv5z9 op1, struct ztuple_z8z5bv1zCz0z5bv12zCz0z5bv5zCz0z5bv5z9 op2) {
  return EQUAL(fbits)(op1.ztup0, op2.ztup0) && EQUAL(fbits)(op1.ztup1, op2.ztup1) && EQUAL(fbits)(op1.ztup2, op2.ztup2) && EQUAL(fbits)(op1.ztup3, op2.ztup3);
}


// struct tuple_(%bv1, %bv5, %bv1, %bv5, %bv5)
struct ztuple_z8z5bv1zCz0z5bv5zCz0z5bv1zCz0z5bv5zCz0z5bv5z9 {
  uint64_t ztup0;
  uint64_t ztup1;
  uint64_t ztup2;
  uint64_t ztup3;
  uint64_t ztup4;
};

static void COPY(ztuple_z8z5bv1zCz0z5bv5zCz0z5bv1zCz0z5bv5zCz0z5bv5z9)(struct ztuple_z8z5bv1zCz0z5bv5zCz0z5bv1zCz0z5bv5zCz0z5bv5z9 *rop, const struct ztuple_z8z5bv1zCz0z5bv5zCz0z5bv1zCz0z5bv5zCz0z5bv5z9 op) {
  rop->ztup0 = op.ztup0;
  rop->ztup1 = op.ztup1;
  rop->ztup2 = op.ztup2;
  rop->ztup3 = op.ztup3;
  rop->ztup4 = op.ztup4;
}

static bool EQUAL(ztuple_z8z5bv1zCz0z5bv5zCz0z5bv1zCz0z5bv5zCz0z5bv5z9)(struct ztuple_z8z5bv1zCz0z5bv5zCz0z5bv1zCz0z5bv5zCz0z5bv5z9 op1, struct ztuple_z8z5bv1zCz0z5bv5zCz0z5bv1zCz0z5bv5zCz0z5bv5z9 op2) {
  return EQUAL(fbits)(op1.ztup0, op2.ztup0) && EQUAL(fbits)(op1.ztup1, op2.ztup1) && EQUAL(fbits)(op1.ztup2, op2.ztup2) && EQUAL(fbits)(op1.ztup3, op2.ztup3) && EQUAL(fbits)(op1.ztup4, op2.ztup4);
}


// struct tuple_(%bv1, %bv5, %bv5)
struct ztuple_z8z5bv1zCz0z5bv5zCz0z5bv5z9 {
  uint64_t ztup0;
  uint64_t ztup1;
  uint64_t ztup2;
};

static void COPY(ztuple_z8z5bv1zCz0z5bv5zCz0z5bv5z9)(struct ztuple_z8z5bv1zCz0z5bv5zCz0z5bv5z9 *rop, const struct ztuple_z8z5bv1zCz0z5bv5zCz0z5bv5z9 op) {
  rop->ztup0 = op.ztup0;
  rop->ztup1 = op.ztup1;
  rop->ztup2 = op.ztup2;
}

static bool EQUAL(ztuple_z8z5bv1zCz0z5bv5zCz0z5bv5z9)(struct ztuple_z8z5bv1zCz0z5bv5zCz0z5bv5z9 op1, struct ztuple_z8z5bv1zCz0z5bv5zCz0z5bv5z9 op2) {
  return EQUAL(fbits)(op1.ztup0, op2.ztup0) && EQUAL(fbits)(op1.ztup1, op2.ztup1) && EQUAL(fbits)(op1.ztup2, op2.ztup2);
}


// struct tuple_(%bv1, %bv5, %bv5, %bv5)
struct ztuple_z8z5bv1zCz0z5bv5zCz0z5bv5zCz0z5bv5z9 {
  uint64_t ztup0;
  uint64_t ztup1;
  uint64_t ztup2;
  uint64_t ztup3;
};

static void COPY(ztuple_z8z5bv1zCz0z5bv5zCz0z5bv5zCz0z5bv5z9)(struct ztuple_z8z5bv1zCz0z5bv5zCz0z5bv5zCz0z5bv5z9 *rop, const struct ztuple_z8z5bv1zCz0z5bv5zCz0z5bv5zCz0z5bv5z9 op) {
  rop->ztup0 = op.ztup0;
  rop->ztup1 = op.ztup1;
  rop->ztup2 = op.ztup2;
  rop->ztup3 = op.ztup3;
}

static bool EQUAL(ztuple_z8z5bv1zCz0z5bv5zCz0z5bv5zCz0z5bv5z9)(struct ztuple_z8z5bv1zCz0z5bv5zCz0z5bv5zCz0z5bv5z9 op1, struct ztuple_z8z5bv1zCz0z5bv5zCz0z5bv5zCz0z5bv5z9 op2) {
  return EQUAL(fbits)(op1.ztup0, op2.ztup0) && EQUAL(fbits)(op1.ztup1, op2.ztup1) && EQUAL(fbits)(op1.ztup2, op2.ztup2) && EQUAL(fbits)(op1.ztup3, op2.ztup3);
}


// struct tuple_(%bv1, %bv7, %bv5, %bv5, %bv5)
struct ztuple_z8z5bv1zCz0z5bv7zCz0z5bv5zCz0z5bv5zCz0z5bv5z9 {
  uint64_t ztup0;
  uint64_t ztup1;
  uint64_t ztup2;
  uint64_t ztup3;
  uint64_t ztup4;
};

static void COPY(ztuple_z8z5bv1zCz0z5bv7zCz0z5bv5zCz0z5bv5zCz0z5bv5z9)(struct ztuple_z8z5bv1zCz0z5bv7zCz0z5bv5zCz0z5bv5zCz0z5bv5z9 *rop, const struct ztuple_z8z5bv1zCz0z5bv7zCz0z5bv5zCz0z5bv5zCz0z5bv5z9 op) {
  rop->ztup0 = op.ztup0;
  rop->ztup1 = op.ztup1;
  rop->ztup2 = op.ztup2;
  rop->ztup3 = op.ztup3;
  rop->ztup4 = op.ztup4;
}

static bool EQUAL(ztuple_z8z5bv1zCz0z5bv7zCz0z5bv5zCz0z5bv5zCz0z5bv5z9)(struct ztuple_z8z5bv1zCz0z5bv7zCz0z5bv5zCz0z5bv5zCz0z5bv5z9 op1, struct ztuple_z8z5bv1zCz0z5bv7zCz0z5bv5zCz0z5bv5zCz0z5bv5z9 op2) {
  return EQUAL(fbits)(op1.ztup0, op2.ztup0) && EQUAL(fbits)(op1.ztup1, op2.ztup1) && EQUAL(fbits)(op1.ztup2, op2.ztup2) && EQUAL(fbits)(op1.ztup3, op2.ztup3) && EQUAL(fbits)(op1.ztup4, op2.ztup4);
}


// struct tuple_(%bv1, %bv9, %bv2, %bv5, %bv5)
struct ztuple_z8z5bv1zCz0z5bv9zCz0z5bv2zCz0z5bv5zCz0z5bv5z9 {
  uint64_t ztup0;
  uint64_t ztup1;
  uint64_t ztup2;
  uint64_t ztup3;
  uint64_t ztup4;
};

static void COPY(ztuple_z8z5bv1zCz0z5bv9zCz0z5bv2zCz0z5bv5zCz0z5bv5z9)(struct ztuple_z8z5bv1zCz0z5bv9zCz0z5bv2zCz0z5bv5zCz0z5bv5z9 *rop, const struct ztuple_z8z5bv1zCz0z5bv9zCz0z5bv2zCz0z5bv5zCz0z5bv5z9 op) {
  rop->ztup0 = op.ztup0;
  rop->ztup1 = op.ztup1;
  rop->ztup2 = op.ztup2;
  rop->ztup3 = op.ztup3;
  rop->ztup4 = op.ztup4;
}

static bool EQUAL(ztuple_z8z5bv1zCz0z5bv9zCz0z5bv2zCz0z5bv5zCz0z5bv5z9)(struct ztuple_z8z5bv1zCz0z5bv9zCz0z5bv2zCz0z5bv5zCz0z5bv5z9 op1, struct ztuple_z8z5bv1zCz0z5bv9zCz0z5bv2zCz0z5bv5zCz0z5bv5z9 op2) {
  return EQUAL(fbits)(op1.ztup0, op2.ztup0) && EQUAL(fbits)(op1.ztup1, op2.ztup1) && EQUAL(fbits)(op1.ztup2, op2.ztup2) && EQUAL(fbits)(op1.ztup3, op2.ztup3) && EQUAL(fbits)(op1.ztup4, op2.ztup4);
}


// struct tuple_(%bv17, %bv5)
struct ztuple_z8z5bv17zCz0z5bv5z9 {
  uint64_t ztup0;
  uint64_t ztup1;
};

static void COPY(ztuple_z8z5bv17zCz0z5bv5z9)(struct ztuple_z8z5bv17zCz0z5bv5z9 *rop, const struct ztuple_z8z5bv17zCz0z5bv5z9 op) {
  rop->ztup0 = op.ztup0;
  rop->ztup1 = op.ztup1;
}

static bool EQUAL(ztuple_z8z5bv17zCz0z5bv5z9)(struct ztuple_z8z5bv17zCz0z5bv5z9 op1, struct ztuple_z8z5bv17zCz0z5bv5z9 op2) {
  return EQUAL(fbits)(op1.ztup0, op2.ztup0) && EQUAL(fbits)(op1.ztup1, op2.ztup1);
}


// struct tuple_(%bv2, %bv1, %bv9, %bv2, %bv5, %bv5)
struct ztuple_z8z5bv2zCz0z5bv1zCz0z5bv9zCz0z5bv2zCz0z5bv5zCz0z5bv5z9 {
  uint64_t ztup0;
  uint64_t ztup1;
  uint64_t ztup2;
  uint64_t ztup3;
  uint64_t ztup4;
  uint64_t ztup5;
};

static void COPY(ztuple_z8z5bv2zCz0z5bv1zCz0z5bv9zCz0z5bv2zCz0z5bv5zCz0z5bv5z9)(struct ztuple_z8z5bv2zCz0z5bv1zCz0z5bv9zCz0z5bv2zCz0z5bv5zCz0z5bv5z9 *rop, const struct ztuple_z8z5bv2zCz0z5bv1zCz0z5bv9zCz0z5bv2zCz0z5bv5zCz0z5bv5z9 op) {
  rop->ztup0 = op.ztup0;
  rop->ztup1 = op.ztup1;
  rop->ztup2 = op.ztup2;
  rop->ztup3 = op.ztup3;
  rop->ztup4 = op.ztup4;
  rop->ztup5 = op.ztup5;
}

static bool EQUAL(ztuple_z8z5bv2zCz0z5bv1zCz0z5bv9zCz0z5bv2zCz0z5bv5zCz0z5bv5z9)(struct ztuple_z8z5bv2zCz0z5bv1zCz0z5bv9zCz0z5bv2zCz0z5bv5zCz0z5bv5z9 op1, struct ztuple_z8z5bv2zCz0z5bv1zCz0z5bv9zCz0z5bv2zCz0z5bv5zCz0z5bv5z9 op2) {
  return EQUAL(fbits)(op1.ztup0, op2.ztup0) && EQUAL(fbits)(op1.ztup1, op2.ztup1) && EQUAL(fbits)(op1.ztup2, op2.ztup2) && EQUAL(fbits)(op1.ztup3, op2.ztup3) && EQUAL(fbits)(op1.ztup4, op2.ztup4) && EQUAL(fbits)(op1.ztup5, op2.ztup5);
}


// struct tuple_(%bv2, %bv5)
struct ztuple_z8z5bv2zCz0z5bv5z9 {
  uint64_t ztup0;
  uint64_t ztup1;
};

static void COPY(ztuple_z8z5bv2zCz0z5bv5z9)(struct ztuple_z8z5bv2zCz0z5bv5z9 *rop, const struct ztuple_z8z5bv2zCz0z5bv5z9 op) {
  rop->ztup0 = op.ztup0;
  rop->ztup1 = op.ztup1;
}

static bool EQUAL(ztuple_z8z5bv2zCz0z5bv5z9)(struct ztuple_z8z5bv2zCz0z5bv5z9 op1, struct ztuple_z8z5bv2zCz0z5bv5z9 op2) {
  return EQUAL(fbits)(op1.ztup0, op2.ztup0) && EQUAL(fbits)(op1.ztup1, op2.ztup1);
}


// struct tuple_(%bv2, %bv5, %bv1, %bv1, %bv1, %bv5, %bv5)
struct ztuple_z8z5bv2zCz0z5bv5zCz0z5bv1zCz0z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5z9 {
  uint64_t ztup0;
  uint64_t ztup1;
  uint64_t ztup2;
  uint64_t ztup3;
  uint64_t ztup4;
  uint64_t ztup5;
  uint64_t ztup6;
};

static void COPY(ztuple_z8z5bv2zCz0z5bv5zCz0z5bv1zCz0z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5z9)(struct ztuple_z8z5bv2zCz0z5bv5zCz0z5bv1zCz0z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5z9 *rop, const struct ztuple_z8z5bv2zCz0z5bv5zCz0z5bv1zCz0z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5z9 op) {
  rop->ztup0 = op.ztup0;
  rop->ztup1 = op.ztup1;
  rop->ztup2 = op.ztup2;
  rop->ztup3 = op.ztup3;
  rop->ztup4 = op.ztup4;
  rop->ztup5 = op.ztup5;
  rop->ztup6 = op.ztup6;
}

static bool EQUAL(ztuple_z8z5bv2zCz0z5bv5zCz0z5bv1zCz0z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5z9)(struct ztuple_z8z5bv2zCz0z5bv5zCz0z5bv1zCz0z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5z9 op1, struct ztuple_z8z5bv2zCz0z5bv5zCz0z5bv1zCz0z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5z9 op2) {
  return EQUAL(fbits)(op1.ztup0, op2.ztup0) && EQUAL(fbits)(op1.ztup1, op2.ztup1) && EQUAL(fbits)(op1.ztup2, op2.ztup2) && EQUAL(fbits)(op1.ztup3, op2.ztup3) && EQUAL(fbits)(op1.ztup4, op2.ztup4) && EQUAL(fbits)(op1.ztup5, op2.ztup5) && EQUAL(fbits)(op1.ztup6, op2.ztup6);
}


// struct tuple_(%bv2, %bv5, %bv5)
struct ztuple_z8z5bv2zCz0z5bv5zCz0z5bv5z9 {
  uint64_t ztup0;
  uint64_t ztup1;
  uint64_t ztup2;
};

static void COPY(ztuple_z8z5bv2zCz0z5bv5zCz0z5bv5z9)(struct ztuple_z8z5bv2zCz0z5bv5zCz0z5bv5z9 *rop, const struct ztuple_z8z5bv2zCz0z5bv5zCz0z5bv5z9 op) {
  rop->ztup0 = op.ztup0;
  rop->ztup1 = op.ztup1;
  rop->ztup2 = op.ztup2;
}

static bool EQUAL(ztuple_z8z5bv2zCz0z5bv5zCz0z5bv5z9)(struct ztuple_z8z5bv2zCz0z5bv5zCz0z5bv5z9 op1, struct ztuple_z8z5bv2zCz0z5bv5zCz0z5bv5z9 op2) {
  return EQUAL(fbits)(op1.ztup0, op2.ztup0) && EQUAL(fbits)(op1.ztup1, op2.ztup1) && EQUAL(fbits)(op1.ztup2, op2.ztup2);
}


// struct tuple_(%bv2, %bv9, %bv5, %bv5)
struct ztuple_z8z5bv2zCz0z5bv9zCz0z5bv5zCz0z5bv5z9 {
  uint64_t ztup0;
  uint64_t ztup1;
  uint64_t ztup2;
  uint64_t ztup3;
};

static void COPY(ztuple_z8z5bv2zCz0z5bv9zCz0z5bv5zCz0z5bv5z9)(struct ztuple_z8z5bv2zCz0z5bv9zCz0z5bv5zCz0z5bv5z9 *rop, const struct ztuple_z8z5bv2zCz0z5bv9zCz0z5bv5zCz0z5bv5z9 op) {
  rop->ztup0 = op.ztup0;
  rop->ztup1 = op.ztup1;
  rop->ztup2 = op.ztup2;
  rop->ztup3 = op.ztup3;
}

static bool EQUAL(ztuple_z8z5bv2zCz0z5bv9zCz0z5bv5zCz0z5bv5z9)(struct ztuple_z8z5bv2zCz0z5bv9zCz0z5bv5zCz0z5bv5z9 op1, struct ztuple_z8z5bv2zCz0z5bv9zCz0z5bv5zCz0z5bv5z9 op2) {
  return EQUAL(fbits)(op1.ztup0, op2.ztup0) && EQUAL(fbits)(op1.ztup1, op2.ztup1) && EQUAL(fbits)(op1.ztup2, op2.ztup2) && EQUAL(fbits)(op1.ztup3, op2.ztup3);
}


// struct tuple_(%bv5, %bv1, %bv1, %bv1, %bv1, %bv5, %bv5)
struct ztuple_z8z5bv5zCz0z5bv1zCz0z5bv1zCz0z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5z9 {
  uint64_t ztup0;
  uint64_t ztup1;
  uint64_t ztup2;
  uint64_t ztup3;
  uint64_t ztup4;
  uint64_t ztup5;
  uint64_t ztup6;
};

static void COPY(ztuple_z8z5bv5zCz0z5bv1zCz0z5bv1zCz0z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5z9)(struct ztuple_z8z5bv5zCz0z5bv1zCz0z5bv1zCz0z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5z9 *rop, const struct ztuple_z8z5bv5zCz0z5bv1zCz0z5bv1zCz0z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5z9 op) {
  rop->ztup0 = op.ztup0;
  rop->ztup1 = op.ztup1;
  rop->ztup2 = op.ztup2;
  rop->ztup3 = op.ztup3;
  rop->ztup4 = op.ztup4;
  rop->ztup5 = op.ztup5;
  rop->ztup6 = op.ztup6;
}

static bool EQUAL(ztuple_z8z5bv5zCz0z5bv1zCz0z5bv1zCz0z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5z9)(struct ztuple_z8z5bv5zCz0z5bv1zCz0z5bv1zCz0z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5z9 op1, struct ztuple_z8z5bv5zCz0z5bv1zCz0z5bv1zCz0z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5z9 op2) {
  return EQUAL(fbits)(op1.ztup0, op2.ztup0) && EQUAL(fbits)(op1.ztup1, op2.ztup1) && EQUAL(fbits)(op1.ztup2, op2.ztup2) && EQUAL(fbits)(op1.ztup3, op2.ztup3) && EQUAL(fbits)(op1.ztup4, op2.ztup4) && EQUAL(fbits)(op1.ztup5, op2.ztup5) && EQUAL(fbits)(op1.ztup6, op2.ztup6);
}


// struct tuple_(%bv5, %bv2, %bv5)
struct ztuple_z8z5bv5zCz0z5bv2zCz0z5bv5z9 {
  uint64_t ztup0;
  uint64_t ztup1;
  uint64_t ztup2;
};

static void COPY(ztuple_z8z5bv5zCz0z5bv2zCz0z5bv5z9)(struct ztuple_z8z5bv5zCz0z5bv2zCz0z5bv5z9 *rop, const struct ztuple_z8z5bv5zCz0z5bv2zCz0z5bv5z9 op) {
  rop->ztup0 = op.ztup0;
  rop->ztup1 = op.ztup1;
  rop->ztup2 = op.ztup2;
}

static bool EQUAL(ztuple_z8z5bv5zCz0z5bv2zCz0z5bv5z9)(struct ztuple_z8z5bv5zCz0z5bv2zCz0z5bv5z9 op1, struct ztuple_z8z5bv5zCz0z5bv2zCz0z5bv5z9 op2) {
  return EQUAL(fbits)(op1.ztup0, op2.ztup0) && EQUAL(fbits)(op1.ztup1, op2.ztup1) && EQUAL(fbits)(op1.ztup2, op2.ztup2);
}


// struct tuple_(%bv5, %bv5)
struct ztuple_z8z5bv5zCz0z5bv5z9 {
  uint64_t ztup0;
  uint64_t ztup1;
};

static void COPY(ztuple_z8z5bv5zCz0z5bv5z9)(struct ztuple_z8z5bv5zCz0z5bv5z9 *rop, const struct ztuple_z8z5bv5zCz0z5bv5z9 op) {
  rop->ztup0 = op.ztup0;
  rop->ztup1 = op.ztup1;
}

static bool EQUAL(ztuple_z8z5bv5zCz0z5bv5z9)(struct ztuple_z8z5bv5zCz0z5bv5z9 op1, struct ztuple_z8z5bv5zCz0z5bv5z9 op2) {
  return EQUAL(fbits)(op1.ztup0, op2.ztup0) && EQUAL(fbits)(op1.ztup1, op2.ztup1);
}


// struct tuple_(%bv7, %bv5)
struct ztuple_z8z5bv7zCz0z5bv5z9 {
  uint64_t ztup0;
  uint64_t ztup1;
};

static void COPY(ztuple_z8z5bv7zCz0z5bv5z9)(struct ztuple_z8z5bv7zCz0z5bv5z9 *rop, const struct ztuple_z8z5bv7zCz0z5bv5z9 op) {
  rop->ztup0 = op.ztup0;
  rop->ztup1 = op.ztup1;
}

static bool EQUAL(ztuple_z8z5bv7zCz0z5bv5z9)(struct ztuple_z8z5bv7zCz0z5bv5z9 op1, struct ztuple_z8z5bv7zCz0z5bv5z9 op2) {
  return EQUAL(fbits)(op1.ztup0, op2.ztup0) && EQUAL(fbits)(op1.ztup1, op2.ztup1);
}

// union instr_ast
enum kind_zinstr_ast { Kind_zInstr_ALDAR_C_R_C, Kind_zInstr_ALDR_C_RRB_C, Kind_zInstr_ALDR_C_RUI_C, Kind_zInstr_ALDUR_C_RI_C, Kind_zInstr_BLRR_C_C, Kind_zInstr_BLRS_C_C, Kind_zInstr_BLRS_C_C_C, Kind_zInstr_BLR_CI_C, Kind_zInstr_BLR_C_C, Kind_zInstr_BRR_C_C, Kind_zInstr_BRS_C_C, Kind_zInstr_BRS_C_C_C, Kind_zInstr_BR_CI_C, Kind_zInstr_BR_C_C, Kind_zInstr_CASAL_C_R_C, Kind_zInstr_CASA_C_R_C, Kind_zInstr_CASL_C_R_C, Kind_zInstr_CAS_C_R_C, Kind_zInstr_LDAPR_C_R_C, Kind_zInstr_LDAR_C_R_C, Kind_zInstr_LDAXP_C_R_C, Kind_zInstr_LDAXR_C_R_C, Kind_zInstr_LDCT_R_R__, Kind_zInstr_LDNP_C_RIB_C, Kind_zInstr_LDPBLR_C_C_C, Kind_zInstr_LDPBR_C_C_C, Kind_zInstr_LDP_CC_RIAW_C, Kind_zInstr_LDP_C_RIBW_C, Kind_zInstr_LDP_C_RIB_C, Kind_zInstr_LDR_C_I_C, Kind_zInstr_LDR_C_RIAW_C, Kind_zInstr_LDR_C_RIBW_C, Kind_zInstr_LDR_C_RRB_C, Kind_zInstr_LDR_C_RUIB_C, Kind_zInstr_LDTR_C_RIB_C, Kind_zInstr_LDUR_C_RI_C, Kind_zInstr_LDXP_C_R_C, Kind_zInstr_LDXR_C_R_C, Kind_zInstr_Other, Kind_zInstr_RETR_C_C, Kind_zInstr_RETS_C_C, Kind_zInstr_RETS_C_C_C, Kind_zInstr_RET_C_C, Kind_zInstr_SWPAL_CC_R_C, Kind_zInstr_SWPA_CC_R_C, Kind_zInstr_SWPL_CC_R_C, Kind_zInstr_SWP_CC_R_C };

struct zinstr_ast {
  enum kind_zinstr_ast kind;
  union {
    struct { struct ztuple_z8z5bv1zCz0z5bv5zCz0z5bv5z9 zInstr_ALDAR_C_R_C; };
    struct { struct ztuple_z8z5bv5zCz0z5bv1zCz0z5bv1zCz0z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5z9 zInstr_ALDR_C_RRB_C; };
    struct { struct ztuple_z8z5bv1zCz0z5bv9zCz0z5bv2zCz0z5bv5zCz0z5bv5z9 zInstr_ALDR_C_RUI_C; };
    struct { struct ztuple_z8z5bv2zCz0z5bv1zCz0z5bv9zCz0z5bv2zCz0z5bv5zCz0z5bv5z9 zInstr_ALDUR_C_RI_C; };
    struct { struct ztuple_z8z5bv2zCz0z5bv5z9 zInstr_BLRR_C_C; };
    struct { struct ztuple_z8z5bv2zCz0z5bv5z9 zInstr_BLRS_C_C; };
    struct { struct ztuple_z8z5bv5zCz0z5bv2zCz0z5bv5z9 zInstr_BLRS_C_C_C; };
    struct { struct ztuple_z8z5bv7zCz0z5bv5z9 zInstr_BLR_CI_C; };
    struct { struct ztuple_z8z5bv2zCz0z5bv5z9 zInstr_BLR_C_C; };
    struct { struct ztuple_z8z5bv2zCz0z5bv5z9 zInstr_BRR_C_C; };
    struct { struct ztuple_z8z5bv2zCz0z5bv5z9 zInstr_BRS_C_C; };
    struct { struct ztuple_z8z5bv5zCz0z5bv2zCz0z5bv5z9 zInstr_BRS_C_C_C; };
    struct { struct ztuple_z8z5bv7zCz0z5bv5z9 zInstr_BR_CI_C; };
    struct { struct ztuple_z8z5bv2zCz0z5bv5z9 zInstr_BR_C_C; };
    struct { struct ztuple_z8z5bv1zCz0z5bv5zCz0z5bv1zCz0z5bv5zCz0z5bv5z9 zInstr_CASAL_C_R_C; };
    struct { struct ztuple_z8z5bv1zCz0z5bv5zCz0z5bv1zCz0z5bv5zCz0z5bv5z9 zInstr_CASA_C_R_C; };
    struct { struct ztuple_z8z5bv1zCz0z5bv5zCz0z5bv1zCz0z5bv5zCz0z5bv5z9 zInstr_CASL_C_R_C; };
    struct { struct ztuple_z8z5bv1zCz0z5bv5zCz0z5bv1zCz0z5bv5zCz0z5bv5z9 zInstr_CAS_C_R_C; };
    struct { struct ztuple_z8z5bv5zCz0z5bv5z9 zInstr_LDAPR_C_R_C; };
    struct { struct ztuple_z8z5bv1zCz0z5bv5zCz0z5bv5z9 zInstr_LDAR_C_R_C; };
    struct { struct ztuple_z8z5bv1zCz0z5bv5zCz0z5bv5zCz0z5bv5z9 zInstr_LDAXP_C_R_C; };
    struct { struct ztuple_z8z5bv1zCz0z5bv5zCz0z5bv5z9 zInstr_LDAXR_C_R_C; };
    struct { struct ztuple_z8z5bv2zCz0z5bv5zCz0z5bv5z9 zInstr_LDCT_R_R__; };
    struct { struct ztuple_z8z5bv1zCz0z5bv7zCz0z5bv5zCz0z5bv5zCz0z5bv5z9 zInstr_LDNP_C_RIB_C; };
    struct { struct ztuple_z8z5bv2zCz0z5bv5zCz0z5bv5z9 zInstr_LDPBLR_C_C_C; };
    struct { struct ztuple_z8z5bv2zCz0z5bv5zCz0z5bv5z9 zInstr_LDPBR_C_C_C; };
    struct { struct ztuple_z8z5bv1zCz0z5bv7zCz0z5bv5zCz0z5bv5zCz0z5bv5z9 zInstr_LDP_CC_RIAW_C; };
    struct { struct ztuple_z8z5bv1zCz0z5bv7zCz0z5bv5zCz0z5bv5zCz0z5bv5z9 zInstr_LDP_C_RIBW_C; };
    struct { struct ztuple_z8z5bv1zCz0z5bv7zCz0z5bv5zCz0z5bv5zCz0z5bv5z9 zInstr_LDP_C_RIB_C; };
    struct { struct ztuple_z8z5bv17zCz0z5bv5z9 zInstr_LDR_C_I_C; };
    struct { struct ztuple_z8z5bv2zCz0z5bv9zCz0z5bv5zCz0z5bv5z9 zInstr_LDR_C_RIAW_C; };
    struct { struct ztuple_z8z5bv2zCz0z5bv9zCz0z5bv5zCz0z5bv5z9 zInstr_LDR_C_RIBW_C; };
    struct { struct ztuple_z8z5bv2zCz0z5bv5zCz0z5bv1zCz0z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5z9 zInstr_LDR_C_RRB_C; };
    struct { struct ztuple_z8z5bv1zCz0z5bv12zCz0z5bv5zCz0z5bv5z9 zInstr_LDR_C_RUIB_C; };
    struct { struct ztuple_z8z5bv2zCz0z5bv9zCz0z5bv5zCz0z5bv5z9 zInstr_LDTR_C_RIB_C; };
    struct { struct ztuple_z8z5bv2zCz0z5bv9zCz0z5bv5zCz0z5bv5z9 zInstr_LDUR_C_RI_C; };
    struct { struct ztuple_z8z5bv1zCz0z5bv5zCz0z5bv5zCz0z5bv5z9 zInstr_LDXP_C_R_C; };
    struct { struct ztuple_z8z5bv1zCz0z5bv5zCz0z5bv5z9 zInstr_LDXR_C_R_C; };
    struct { uint64_t zInstr_Other; };
    struct { struct ztuple_z8z5bv2zCz0z5bv5z9 zInstr_RETR_C_C; };
    struct { struct ztuple_z8z5bv2zCz0z5bv5z9 zInstr_RETS_C_C; };
    struct { struct ztuple_z8z5bv5zCz0z5bv2zCz0z5bv5z9 zInstr_RETS_C_C_C; };
    struct { struct ztuple_z8z5bv2zCz0z5bv5z9 zInstr_RET_C_C; };
    struct { struct ztuple_z8z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5zCz0z5bv5z9 zInstr_SWPAL_CC_R_C; };
    struct { struct ztuple_z8z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5zCz0z5bv5z9 zInstr_SWPA_CC_R_C; };
    struct { struct ztuple_z8z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5zCz0z5bv5z9 zInstr_SWPL_CC_R_C; };
    struct { struct ztuple_z8z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5zCz0z5bv5z9 zInstr_SWP_CC_R_C; };
  };
};

static void CREATE(zinstr_ast)(struct zinstr_ast *op)
{
  op->kind = Kind_zInstr_ALDAR_C_R_C;

}

static void RECREATE(zinstr_ast)(struct zinstr_ast *op) {}

static void KILL(zinstr_ast)(struct zinstr_ast *op)
{
  if (op->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (op->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  };
}

static void COPY(zinstr_ast)(struct zinstr_ast *rop, struct zinstr_ast op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  };
  rop->kind = op.kind;
  if (op.kind == Kind_zInstr_ALDAR_C_R_C) {
    rop->zInstr_ALDAR_C_R_C = op.zInstr_ALDAR_C_R_C;
  } else if (op.kind == Kind_zInstr_ALDR_C_RRB_C) {
    rop->zInstr_ALDR_C_RRB_C = op.zInstr_ALDR_C_RRB_C;
  } else if (op.kind == Kind_zInstr_ALDR_C_RUI_C) {
    rop->zInstr_ALDR_C_RUI_C = op.zInstr_ALDR_C_RUI_C;
  } else if (op.kind == Kind_zInstr_ALDUR_C_RI_C) {
    rop->zInstr_ALDUR_C_RI_C = op.zInstr_ALDUR_C_RI_C;
  } else if (op.kind == Kind_zInstr_BLRR_C_C) {
    rop->zInstr_BLRR_C_C = op.zInstr_BLRR_C_C;
  } else if (op.kind == Kind_zInstr_BLRS_C_C) {
    rop->zInstr_BLRS_C_C = op.zInstr_BLRS_C_C;
  } else if (op.kind == Kind_zInstr_BLRS_C_C_C) {
    rop->zInstr_BLRS_C_C_C = op.zInstr_BLRS_C_C_C;
  } else if (op.kind == Kind_zInstr_BLR_CI_C) {
    rop->zInstr_BLR_CI_C = op.zInstr_BLR_CI_C;
  } else if (op.kind == Kind_zInstr_BLR_C_C) {
    rop->zInstr_BLR_C_C = op.zInstr_BLR_C_C;
  } else if (op.kind == Kind_zInstr_BRR_C_C) {
    rop->zInstr_BRR_C_C = op.zInstr_BRR_C_C;
  } else if (op.kind == Kind_zInstr_BRS_C_C) {
    rop->zInstr_BRS_C_C = op.zInstr_BRS_C_C;
  } else if (op.kind == Kind_zInstr_BRS_C_C_C) {
    rop->zInstr_BRS_C_C_C = op.zInstr_BRS_C_C_C;
  } else if (op.kind == Kind_zInstr_BR_CI_C) {
    rop->zInstr_BR_CI_C = op.zInstr_BR_CI_C;
  } else if (op.kind == Kind_zInstr_BR_C_C) {
    rop->zInstr_BR_C_C = op.zInstr_BR_C_C;
  } else if (op.kind == Kind_zInstr_CASAL_C_R_C) {
    rop->zInstr_CASAL_C_R_C = op.zInstr_CASAL_C_R_C;
  } else if (op.kind == Kind_zInstr_CASA_C_R_C) {
    rop->zInstr_CASA_C_R_C = op.zInstr_CASA_C_R_C;
  } else if (op.kind == Kind_zInstr_CASL_C_R_C) {
    rop->zInstr_CASL_C_R_C = op.zInstr_CASL_C_R_C;
  } else if (op.kind == Kind_zInstr_CAS_C_R_C) {
    rop->zInstr_CAS_C_R_C = op.zInstr_CAS_C_R_C;
  } else if (op.kind == Kind_zInstr_LDAPR_C_R_C) {
    rop->zInstr_LDAPR_C_R_C = op.zInstr_LDAPR_C_R_C;
  } else if (op.kind == Kind_zInstr_LDAR_C_R_C) {
    rop->zInstr_LDAR_C_R_C = op.zInstr_LDAR_C_R_C;
  } else if (op.kind == Kind_zInstr_LDAXP_C_R_C) {
    rop->zInstr_LDAXP_C_R_C = op.zInstr_LDAXP_C_R_C;
  } else if (op.kind == Kind_zInstr_LDAXR_C_R_C) {
    rop->zInstr_LDAXR_C_R_C = op.zInstr_LDAXR_C_R_C;
  } else if (op.kind == Kind_zInstr_LDCT_R_R__) {
    rop->zInstr_LDCT_R_R__ = op.zInstr_LDCT_R_R__;
  } else if (op.kind == Kind_zInstr_LDNP_C_RIB_C) {
    rop->zInstr_LDNP_C_RIB_C = op.zInstr_LDNP_C_RIB_C;
  } else if (op.kind == Kind_zInstr_LDPBLR_C_C_C) {
    rop->zInstr_LDPBLR_C_C_C = op.zInstr_LDPBLR_C_C_C;
  } else if (op.kind == Kind_zInstr_LDPBR_C_C_C) {
    rop->zInstr_LDPBR_C_C_C = op.zInstr_LDPBR_C_C_C;
  } else if (op.kind == Kind_zInstr_LDP_CC_RIAW_C) {
    rop->zInstr_LDP_CC_RIAW_C = op.zInstr_LDP_CC_RIAW_C;
  } else if (op.kind == Kind_zInstr_LDP_C_RIBW_C) {
    rop->zInstr_LDP_C_RIBW_C = op.zInstr_LDP_C_RIBW_C;
  } else if (op.kind == Kind_zInstr_LDP_C_RIB_C) {
    rop->zInstr_LDP_C_RIB_C = op.zInstr_LDP_C_RIB_C;
  } else if (op.kind == Kind_zInstr_LDR_C_I_C) {
    rop->zInstr_LDR_C_I_C = op.zInstr_LDR_C_I_C;
  } else if (op.kind == Kind_zInstr_LDR_C_RIAW_C) {
    rop->zInstr_LDR_C_RIAW_C = op.zInstr_LDR_C_RIAW_C;
  } else if (op.kind == Kind_zInstr_LDR_C_RIBW_C) {
    rop->zInstr_LDR_C_RIBW_C = op.zInstr_LDR_C_RIBW_C;
  } else if (op.kind == Kind_zInstr_LDR_C_RRB_C) {
    rop->zInstr_LDR_C_RRB_C = op.zInstr_LDR_C_RRB_C;
  } else if (op.kind == Kind_zInstr_LDR_C_RUIB_C) {
    rop->zInstr_LDR_C_RUIB_C = op.zInstr_LDR_C_RUIB_C;
  } else if (op.kind == Kind_zInstr_LDTR_C_RIB_C) {
    rop->zInstr_LDTR_C_RIB_C = op.zInstr_LDTR_C_RIB_C;
  } else if (op.kind == Kind_zInstr_LDUR_C_RI_C) {
    rop->zInstr_LDUR_C_RI_C = op.zInstr_LDUR_C_RI_C;
  } else if (op.kind == Kind_zInstr_LDXP_C_R_C) {
    rop->zInstr_LDXP_C_R_C = op.zInstr_LDXP_C_R_C;
  } else if (op.kind == Kind_zInstr_LDXR_C_R_C) {
    rop->zInstr_LDXR_C_R_C = op.zInstr_LDXR_C_R_C;
  } else if (op.kind == Kind_zInstr_Other) {
    rop->zInstr_Other = op.zInstr_Other;
  } else if (op.kind == Kind_zInstr_RETR_C_C) {
    rop->zInstr_RETR_C_C = op.zInstr_RETR_C_C;
  } else if (op.kind == Kind_zInstr_RETS_C_C) {
    rop->zInstr_RETS_C_C = op.zInstr_RETS_C_C;
  } else if (op.kind == Kind_zInstr_RETS_C_C_C) {
    rop->zInstr_RETS_C_C_C = op.zInstr_RETS_C_C_C;
  } else if (op.kind == Kind_zInstr_RET_C_C) {
    rop->zInstr_RET_C_C = op.zInstr_RET_C_C;
  } else if (op.kind == Kind_zInstr_SWPAL_CC_R_C) {
    rop->zInstr_SWPAL_CC_R_C = op.zInstr_SWPAL_CC_R_C;
  } else if (op.kind == Kind_zInstr_SWPA_CC_R_C) {
    rop->zInstr_SWPA_CC_R_C = op.zInstr_SWPA_CC_R_C;
  } else if (op.kind == Kind_zInstr_SWPL_CC_R_C) {
    rop->zInstr_SWPL_CC_R_C = op.zInstr_SWPL_CC_R_C;
  } else if (op.kind == Kind_zInstr_SWP_CC_R_C){
    rop->zInstr_SWP_CC_R_C = op.zInstr_SWP_CC_R_C;
  }
}

static bool EQUAL(zinstr_ast)(struct zinstr_ast op1, struct zinstr_ast op2) {
  if (op1.kind == Kind_zInstr_ALDAR_C_R_C && op2.kind == Kind_zInstr_ALDAR_C_R_C) {
    return EQUAL(ztuple_z8z5bv1zCz0z5bv5zCz0z5bv5z9)(op1.zInstr_ALDAR_C_R_C, op2.zInstr_ALDAR_C_R_C);
  } else if (op1.kind == Kind_zInstr_ALDR_C_RRB_C && op2.kind == Kind_zInstr_ALDR_C_RRB_C) {

  return EQUAL(ztuple_z8z5bv5zCz0z5bv1zCz0z5bv1zCz0z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5z9)(op1.zInstr_ALDR_C_RRB_C, op2.zInstr_ALDR_C_RRB_C);
  } else if (op1.kind == Kind_zInstr_ALDR_C_RUI_C && op2.kind == Kind_zInstr_ALDR_C_RUI_C) {

  return EQUAL(ztuple_z8z5bv1zCz0z5bv9zCz0z5bv2zCz0z5bv5zCz0z5bv5z9)(op1.zInstr_ALDR_C_RUI_C, op2.zInstr_ALDR_C_RUI_C);
  } else if (op1.kind == Kind_zInstr_ALDUR_C_RI_C && op2.kind == Kind_zInstr_ALDUR_C_RI_C) {

  return EQUAL(ztuple_z8z5bv2zCz0z5bv1zCz0z5bv9zCz0z5bv2zCz0z5bv5zCz0z5bv5z9)(op1.zInstr_ALDUR_C_RI_C, op2.zInstr_ALDUR_C_RI_C);
  } else if (op1.kind == Kind_zInstr_BLRR_C_C && op2.kind == Kind_zInstr_BLRR_C_C) {
    return EQUAL(ztuple_z8z5bv2zCz0z5bv5z9)(op1.zInstr_BLRR_C_C, op2.zInstr_BLRR_C_C);
  } else if (op1.kind == Kind_zInstr_BLRS_C_C && op2.kind == Kind_zInstr_BLRS_C_C) {
    return EQUAL(ztuple_z8z5bv2zCz0z5bv5z9)(op1.zInstr_BLRS_C_C, op2.zInstr_BLRS_C_C);
  } else if (op1.kind == Kind_zInstr_BLRS_C_C_C && op2.kind == Kind_zInstr_BLRS_C_C_C) {
    return EQUAL(ztuple_z8z5bv5zCz0z5bv2zCz0z5bv5z9)(op1.zInstr_BLRS_C_C_C, op2.zInstr_BLRS_C_C_C);
  } else if (op1.kind == Kind_zInstr_BLR_CI_C && op2.kind == Kind_zInstr_BLR_CI_C) {
    return EQUAL(ztuple_z8z5bv7zCz0z5bv5z9)(op1.zInstr_BLR_CI_C, op2.zInstr_BLR_CI_C);
  } else if (op1.kind == Kind_zInstr_BLR_C_C && op2.kind == Kind_zInstr_BLR_C_C) {
    return EQUAL(ztuple_z8z5bv2zCz0z5bv5z9)(op1.zInstr_BLR_C_C, op2.zInstr_BLR_C_C);
  } else if (op1.kind == Kind_zInstr_BRR_C_C && op2.kind == Kind_zInstr_BRR_C_C) {
    return EQUAL(ztuple_z8z5bv2zCz0z5bv5z9)(op1.zInstr_BRR_C_C, op2.zInstr_BRR_C_C);
  } else if (op1.kind == Kind_zInstr_BRS_C_C && op2.kind == Kind_zInstr_BRS_C_C) {
    return EQUAL(ztuple_z8z5bv2zCz0z5bv5z9)(op1.zInstr_BRS_C_C, op2.zInstr_BRS_C_C);
  } else if (op1.kind == Kind_zInstr_BRS_C_C_C && op2.kind == Kind_zInstr_BRS_C_C_C) {
    return EQUAL(ztuple_z8z5bv5zCz0z5bv2zCz0z5bv5z9)(op1.zInstr_BRS_C_C_C, op2.zInstr_BRS_C_C_C);
  } else if (op1.kind == Kind_zInstr_BR_CI_C && op2.kind == Kind_zInstr_BR_CI_C) {
    return EQUAL(ztuple_z8z5bv7zCz0z5bv5z9)(op1.zInstr_BR_CI_C, op2.zInstr_BR_CI_C);
  } else if (op1.kind == Kind_zInstr_BR_C_C && op2.kind == Kind_zInstr_BR_C_C) {
    return EQUAL(ztuple_z8z5bv2zCz0z5bv5z9)(op1.zInstr_BR_C_C, op2.zInstr_BR_C_C);
  } else if (op1.kind == Kind_zInstr_CASAL_C_R_C && op2.kind == Kind_zInstr_CASAL_C_R_C) {
    return EQUAL(ztuple_z8z5bv1zCz0z5bv5zCz0z5bv1zCz0z5bv5zCz0z5bv5z9)(op1.zInstr_CASAL_C_R_C, op2.zInstr_CASAL_C_R_C);
  } else if (op1.kind == Kind_zInstr_CASA_C_R_C && op2.kind == Kind_zInstr_CASA_C_R_C) {
    return EQUAL(ztuple_z8z5bv1zCz0z5bv5zCz0z5bv1zCz0z5bv5zCz0z5bv5z9)(op1.zInstr_CASA_C_R_C, op2.zInstr_CASA_C_R_C);
  } else if (op1.kind == Kind_zInstr_CASL_C_R_C && op2.kind == Kind_zInstr_CASL_C_R_C) {
    return EQUAL(ztuple_z8z5bv1zCz0z5bv5zCz0z5bv1zCz0z5bv5zCz0z5bv5z9)(op1.zInstr_CASL_C_R_C, op2.zInstr_CASL_C_R_C);
  } else if (op1.kind == Kind_zInstr_CAS_C_R_C && op2.kind == Kind_zInstr_CAS_C_R_C) {
    return EQUAL(ztuple_z8z5bv1zCz0z5bv5zCz0z5bv1zCz0z5bv5zCz0z5bv5z9)(op1.zInstr_CAS_C_R_C, op2.zInstr_CAS_C_R_C);
  } else if (op1.kind == Kind_zInstr_LDAPR_C_R_C && op2.kind == Kind_zInstr_LDAPR_C_R_C) {
    return EQUAL(ztuple_z8z5bv5zCz0z5bv5z9)(op1.zInstr_LDAPR_C_R_C, op2.zInstr_LDAPR_C_R_C);
  } else if (op1.kind == Kind_zInstr_LDAR_C_R_C && op2.kind == Kind_zInstr_LDAR_C_R_C) {
    return EQUAL(ztuple_z8z5bv1zCz0z5bv5zCz0z5bv5z9)(op1.zInstr_LDAR_C_R_C, op2.zInstr_LDAR_C_R_C);
  } else if (op1.kind == Kind_zInstr_LDAXP_C_R_C && op2.kind == Kind_zInstr_LDAXP_C_R_C) {
    return EQUAL(ztuple_z8z5bv1zCz0z5bv5zCz0z5bv5zCz0z5bv5z9)(op1.zInstr_LDAXP_C_R_C, op2.zInstr_LDAXP_C_R_C);
  } else if (op1.kind == Kind_zInstr_LDAXR_C_R_C && op2.kind == Kind_zInstr_LDAXR_C_R_C) {
    return EQUAL(ztuple_z8z5bv1zCz0z5bv5zCz0z5bv5z9)(op1.zInstr_LDAXR_C_R_C, op2.zInstr_LDAXR_C_R_C);
  } else if (op1.kind == Kind_zInstr_LDCT_R_R__ && op2.kind == Kind_zInstr_LDCT_R_R__) {
    return EQUAL(ztuple_z8z5bv2zCz0z5bv5zCz0z5bv5z9)(op1.zInstr_LDCT_R_R__, op2.zInstr_LDCT_R_R__);
  } else if (op1.kind == Kind_zInstr_LDNP_C_RIB_C && op2.kind == Kind_zInstr_LDNP_C_RIB_C) {

  return EQUAL(ztuple_z8z5bv1zCz0z5bv7zCz0z5bv5zCz0z5bv5zCz0z5bv5z9)(op1.zInstr_LDNP_C_RIB_C, op2.zInstr_LDNP_C_RIB_C);
  } else if (op1.kind == Kind_zInstr_LDPBLR_C_C_C && op2.kind == Kind_zInstr_LDPBLR_C_C_C) {
    return EQUAL(ztuple_z8z5bv2zCz0z5bv5zCz0z5bv5z9)(op1.zInstr_LDPBLR_C_C_C, op2.zInstr_LDPBLR_C_C_C);
  } else if (op1.kind == Kind_zInstr_LDPBR_C_C_C && op2.kind == Kind_zInstr_LDPBR_C_C_C) {
    return EQUAL(ztuple_z8z5bv2zCz0z5bv5zCz0z5bv5z9)(op1.zInstr_LDPBR_C_C_C, op2.zInstr_LDPBR_C_C_C);
  } else if (op1.kind == Kind_zInstr_LDP_CC_RIAW_C && op2.kind == Kind_zInstr_LDP_CC_RIAW_C) {

  return EQUAL(ztuple_z8z5bv1zCz0z5bv7zCz0z5bv5zCz0z5bv5zCz0z5bv5z9)(op1.zInstr_LDP_CC_RIAW_C, op2.zInstr_LDP_CC_RIAW_C);
  } else if (op1.kind == Kind_zInstr_LDP_C_RIBW_C && op2.kind == Kind_zInstr_LDP_C_RIBW_C) {

  return EQUAL(ztuple_z8z5bv1zCz0z5bv7zCz0z5bv5zCz0z5bv5zCz0z5bv5z9)(op1.zInstr_LDP_C_RIBW_C, op2.zInstr_LDP_C_RIBW_C);
  } else if (op1.kind == Kind_zInstr_LDP_C_RIB_C && op2.kind == Kind_zInstr_LDP_C_RIB_C) {
    return EQUAL(ztuple_z8z5bv1zCz0z5bv7zCz0z5bv5zCz0z5bv5zCz0z5bv5z9)(op1.zInstr_LDP_C_RIB_C, op2.zInstr_LDP_C_RIB_C);
  } else if (op1.kind == Kind_zInstr_LDR_C_I_C && op2.kind == Kind_zInstr_LDR_C_I_C) {
    return EQUAL(ztuple_z8z5bv17zCz0z5bv5z9)(op1.zInstr_LDR_C_I_C, op2.zInstr_LDR_C_I_C);
  } else if (op1.kind == Kind_zInstr_LDR_C_RIAW_C && op2.kind == Kind_zInstr_LDR_C_RIAW_C) {
    return EQUAL(ztuple_z8z5bv2zCz0z5bv9zCz0z5bv5zCz0z5bv5z9)(op1.zInstr_LDR_C_RIAW_C, op2.zInstr_LDR_C_RIAW_C);
  } else if (op1.kind == Kind_zInstr_LDR_C_RIBW_C && op2.kind == Kind_zInstr_LDR_C_RIBW_C) {
    return EQUAL(ztuple_z8z5bv2zCz0z5bv9zCz0z5bv5zCz0z5bv5z9)(op1.zInstr_LDR_C_RIBW_C, op2.zInstr_LDR_C_RIBW_C);
  } else if (op1.kind == Kind_zInstr_LDR_C_RRB_C && op2.kind == Kind_zInstr_LDR_C_RRB_C) {

  return EQUAL(ztuple_z8z5bv2zCz0z5bv5zCz0z5bv1zCz0z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5z9)(op1.zInstr_LDR_C_RRB_C, op2.zInstr_LDR_C_RRB_C);
  } else if (op1.kind == Kind_zInstr_LDR_C_RUIB_C && op2.kind == Kind_zInstr_LDR_C_RUIB_C) {
    return EQUAL(ztuple_z8z5bv1zCz0z5bv12zCz0z5bv5zCz0z5bv5z9)(op1.zInstr_LDR_C_RUIB_C, op2.zInstr_LDR_C_RUIB_C);
  } else if (op1.kind == Kind_zInstr_LDTR_C_RIB_C && op2.kind == Kind_zInstr_LDTR_C_RIB_C) {
    return EQUAL(ztuple_z8z5bv2zCz0z5bv9zCz0z5bv5zCz0z5bv5z9)(op1.zInstr_LDTR_C_RIB_C, op2.zInstr_LDTR_C_RIB_C);
  } else if (op1.kind == Kind_zInstr_LDUR_C_RI_C && op2.kind == Kind_zInstr_LDUR_C_RI_C) {
    return EQUAL(ztuple_z8z5bv2zCz0z5bv9zCz0z5bv5zCz0z5bv5z9)(op1.zInstr_LDUR_C_RI_C, op2.zInstr_LDUR_C_RI_C);
  } else if (op1.kind == Kind_zInstr_LDXP_C_R_C && op2.kind == Kind_zInstr_LDXP_C_R_C) {
    return EQUAL(ztuple_z8z5bv1zCz0z5bv5zCz0z5bv5zCz0z5bv5z9)(op1.zInstr_LDXP_C_R_C, op2.zInstr_LDXP_C_R_C);
  } else if (op1.kind == Kind_zInstr_LDXR_C_R_C && op2.kind == Kind_zInstr_LDXR_C_R_C) {
    return EQUAL(ztuple_z8z5bv1zCz0z5bv5zCz0z5bv5z9)(op1.zInstr_LDXR_C_R_C, op2.zInstr_LDXR_C_R_C);
  } else if (op1.kind == Kind_zInstr_Other && op2.kind == Kind_zInstr_Other) {
    return EQUAL(fbits)(op1.zInstr_Other, op2.zInstr_Other);
  } else if (op1.kind == Kind_zInstr_RETR_C_C && op2.kind == Kind_zInstr_RETR_C_C) {
    return EQUAL(ztuple_z8z5bv2zCz0z5bv5z9)(op1.zInstr_RETR_C_C, op2.zInstr_RETR_C_C);
  } else if (op1.kind == Kind_zInstr_RETS_C_C && op2.kind == Kind_zInstr_RETS_C_C) {
    return EQUAL(ztuple_z8z5bv2zCz0z5bv5z9)(op1.zInstr_RETS_C_C, op2.zInstr_RETS_C_C);
  } else if (op1.kind == Kind_zInstr_RETS_C_C_C && op2.kind == Kind_zInstr_RETS_C_C_C) {
    return EQUAL(ztuple_z8z5bv5zCz0z5bv2zCz0z5bv5z9)(op1.zInstr_RETS_C_C_C, op2.zInstr_RETS_C_C_C);
  } else if (op1.kind == Kind_zInstr_RET_C_C && op2.kind == Kind_zInstr_RET_C_C) {
    return EQUAL(ztuple_z8z5bv2zCz0z5bv5z9)(op1.zInstr_RET_C_C, op2.zInstr_RET_C_C);
  } else if (op1.kind == Kind_zInstr_SWPAL_CC_R_C && op2.kind == Kind_zInstr_SWPAL_CC_R_C) {

  return EQUAL(ztuple_z8z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5zCz0z5bv5z9)(op1.zInstr_SWPAL_CC_R_C, op2.zInstr_SWPAL_CC_R_C);
  } else if (op1.kind == Kind_zInstr_SWPA_CC_R_C && op2.kind == Kind_zInstr_SWPA_CC_R_C) {
    return EQUAL(ztuple_z8z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5zCz0z5bv5z9)(op1.zInstr_SWPA_CC_R_C, op2.zInstr_SWPA_CC_R_C);
  } else if (op1.kind == Kind_zInstr_SWPL_CC_R_C && op2.kind == Kind_zInstr_SWPL_CC_R_C) {
    return EQUAL(ztuple_z8z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5zCz0z5bv5z9)(op1.zInstr_SWPL_CC_R_C, op2.zInstr_SWPL_CC_R_C);
  } else if (op1.kind == Kind_zInstr_SWP_CC_R_C && op2.kind == Kind_zInstr_SWP_CC_R_C) {
    return EQUAL(ztuple_z8z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5zCz0z5bv5z9)(op1.zInstr_SWP_CC_R_C, op2.zInstr_SWP_CC_R_C);
  } else return false;
}

static void sailgen_Instr_ALDAR_C_R_C(struct zinstr_ast *rop, struct ztuple_z8z5bv1zCz0z5bv5zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_ALDAR_C_R_C;
  rop->zInstr_ALDAR_C_R_C = op;
}

static void sailgen_Instr_ALDR_C_RRB_C(struct zinstr_ast *rop, struct ztuple_z8z5bv5zCz0z5bv1zCz0z5bv1zCz0z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_ALDR_C_RRB_C;
  rop->zInstr_ALDR_C_RRB_C = op;
}

static void sailgen_Instr_ALDR_C_RUI_C(struct zinstr_ast *rop, struct ztuple_z8z5bv1zCz0z5bv9zCz0z5bv2zCz0z5bv5zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_ALDR_C_RUI_C;
  rop->zInstr_ALDR_C_RUI_C = op;
}

static void sailgen_Instr_ALDUR_C_RI_C(struct zinstr_ast *rop, struct ztuple_z8z5bv2zCz0z5bv1zCz0z5bv9zCz0z5bv2zCz0z5bv5zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_ALDUR_C_RI_C;
  rop->zInstr_ALDUR_C_RI_C = op;
}

static void sailgen_Instr_BLRR_C_C(struct zinstr_ast *rop, struct ztuple_z8z5bv2zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_BLRR_C_C;
  rop->zInstr_BLRR_C_C = op;
}

static void sailgen_Instr_BLRS_C_C(struct zinstr_ast *rop, struct ztuple_z8z5bv2zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_BLRS_C_C;
  rop->zInstr_BLRS_C_C = op;
}

static void sailgen_Instr_BLRS_C_C_C(struct zinstr_ast *rop, struct ztuple_z8z5bv5zCz0z5bv2zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_BLRS_C_C_C;
  rop->zInstr_BLRS_C_C_C = op;
}

static void sailgen_Instr_BLR_CI_C(struct zinstr_ast *rop, struct ztuple_z8z5bv7zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_BLR_CI_C;
  rop->zInstr_BLR_CI_C = op;
}

static void sailgen_Instr_BLR_C_C(struct zinstr_ast *rop, struct ztuple_z8z5bv2zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_BLR_C_C;
  rop->zInstr_BLR_C_C = op;
}

static void sailgen_Instr_BRR_C_C(struct zinstr_ast *rop, struct ztuple_z8z5bv2zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_BRR_C_C;
  rop->zInstr_BRR_C_C = op;
}

static void sailgen_Instr_BRS_C_C(struct zinstr_ast *rop, struct ztuple_z8z5bv2zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_BRS_C_C;
  rop->zInstr_BRS_C_C = op;
}

static void sailgen_Instr_BRS_C_C_C(struct zinstr_ast *rop, struct ztuple_z8z5bv5zCz0z5bv2zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_BRS_C_C_C;
  rop->zInstr_BRS_C_C_C = op;
}

static void sailgen_Instr_BR_CI_C(struct zinstr_ast *rop, struct ztuple_z8z5bv7zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_BR_CI_C;
  rop->zInstr_BR_CI_C = op;
}

static void sailgen_Instr_BR_C_C(struct zinstr_ast *rop, struct ztuple_z8z5bv2zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_BR_C_C;
  rop->zInstr_BR_C_C = op;
}

static void sailgen_Instr_CASAL_C_R_C(struct zinstr_ast *rop, struct ztuple_z8z5bv1zCz0z5bv5zCz0z5bv1zCz0z5bv5zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_CASAL_C_R_C;
  rop->zInstr_CASAL_C_R_C = op;
}

static void sailgen_Instr_CASA_C_R_C(struct zinstr_ast *rop, struct ztuple_z8z5bv1zCz0z5bv5zCz0z5bv1zCz0z5bv5zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_CASA_C_R_C;
  rop->zInstr_CASA_C_R_C = op;
}

static void sailgen_Instr_CASL_C_R_C(struct zinstr_ast *rop, struct ztuple_z8z5bv1zCz0z5bv5zCz0z5bv1zCz0z5bv5zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_CASL_C_R_C;
  rop->zInstr_CASL_C_R_C = op;
}

static void sailgen_Instr_CAS_C_R_C(struct zinstr_ast *rop, struct ztuple_z8z5bv1zCz0z5bv5zCz0z5bv1zCz0z5bv5zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_CAS_C_R_C;
  rop->zInstr_CAS_C_R_C = op;
}

static void sailgen_Instr_LDAPR_C_R_C(struct zinstr_ast *rop, struct ztuple_z8z5bv5zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_LDAPR_C_R_C;
  rop->zInstr_LDAPR_C_R_C = op;
}

static void sailgen_Instr_LDAR_C_R_C(struct zinstr_ast *rop, struct ztuple_z8z5bv1zCz0z5bv5zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_LDAR_C_R_C;
  rop->zInstr_LDAR_C_R_C = op;
}

static void sailgen_Instr_LDAXP_C_R_C(struct zinstr_ast *rop, struct ztuple_z8z5bv1zCz0z5bv5zCz0z5bv5zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_LDAXP_C_R_C;
  rop->zInstr_LDAXP_C_R_C = op;
}

static void sailgen_Instr_LDAXR_C_R_C(struct zinstr_ast *rop, struct ztuple_z8z5bv1zCz0z5bv5zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_LDAXR_C_R_C;
  rop->zInstr_LDAXR_C_R_C = op;
}

static void sailgen_Instr_LDCT_R_R__(struct zinstr_ast *rop, struct ztuple_z8z5bv2zCz0z5bv5zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_LDCT_R_R__;
  rop->zInstr_LDCT_R_R__ = op;
}

static void sailgen_Instr_LDNP_C_RIB_C(struct zinstr_ast *rop, struct ztuple_z8z5bv1zCz0z5bv7zCz0z5bv5zCz0z5bv5zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_LDNP_C_RIB_C;
  rop->zInstr_LDNP_C_RIB_C = op;
}

static void sailgen_Instr_LDPBLR_C_C_C(struct zinstr_ast *rop, struct ztuple_z8z5bv2zCz0z5bv5zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_LDPBLR_C_C_C;
  rop->zInstr_LDPBLR_C_C_C = op;
}

static void sailgen_Instr_LDPBR_C_C_C(struct zinstr_ast *rop, struct ztuple_z8z5bv2zCz0z5bv5zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_LDPBR_C_C_C;
  rop->zInstr_LDPBR_C_C_C = op;
}

static void sailgen_Instr_LDP_CC_RIAW_C(struct zinstr_ast *rop, struct ztuple_z8z5bv1zCz0z5bv7zCz0z5bv5zCz0z5bv5zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_LDP_CC_RIAW_C;
  rop->zInstr_LDP_CC_RIAW_C = op;
}

static void sailgen_Instr_LDP_C_RIBW_C(struct zinstr_ast *rop, struct ztuple_z8z5bv1zCz0z5bv7zCz0z5bv5zCz0z5bv5zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_LDP_C_RIBW_C;
  rop->zInstr_LDP_C_RIBW_C = op;
}

static void sailgen_Instr_LDP_C_RIB_C(struct zinstr_ast *rop, struct ztuple_z8z5bv1zCz0z5bv7zCz0z5bv5zCz0z5bv5zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_LDP_C_RIB_C;
  rop->zInstr_LDP_C_RIB_C = op;
}

static void sailgen_Instr_LDR_C_I_C(struct zinstr_ast *rop, struct ztuple_z8z5bv17zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_LDR_C_I_C;
  rop->zInstr_LDR_C_I_C = op;
}

static void sailgen_Instr_LDR_C_RIAW_C(struct zinstr_ast *rop, struct ztuple_z8z5bv2zCz0z5bv9zCz0z5bv5zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_LDR_C_RIAW_C;
  rop->zInstr_LDR_C_RIAW_C = op;
}

static void sailgen_Instr_LDR_C_RIBW_C(struct zinstr_ast *rop, struct ztuple_z8z5bv2zCz0z5bv9zCz0z5bv5zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_LDR_C_RIBW_C;
  rop->zInstr_LDR_C_RIBW_C = op;
}

static void sailgen_Instr_LDR_C_RRB_C(struct zinstr_ast *rop, struct ztuple_z8z5bv2zCz0z5bv5zCz0z5bv1zCz0z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_LDR_C_RRB_C;
  rop->zInstr_LDR_C_RRB_C = op;
}

static void sailgen_Instr_LDR_C_RUIB_C(struct zinstr_ast *rop, struct ztuple_z8z5bv1zCz0z5bv12zCz0z5bv5zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_LDR_C_RUIB_C;
  rop->zInstr_LDR_C_RUIB_C = op;
}

static void sailgen_Instr_LDTR_C_RIB_C(struct zinstr_ast *rop, struct ztuple_z8z5bv2zCz0z5bv9zCz0z5bv5zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_LDTR_C_RIB_C;
  rop->zInstr_LDTR_C_RIB_C = op;
}

static void sailgen_Instr_LDUR_C_RI_C(struct zinstr_ast *rop, struct ztuple_z8z5bv2zCz0z5bv9zCz0z5bv5zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_LDUR_C_RI_C;
  rop->zInstr_LDUR_C_RI_C = op;
}

static void sailgen_Instr_LDXP_C_R_C(struct zinstr_ast *rop, struct ztuple_z8z5bv1zCz0z5bv5zCz0z5bv5zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_LDXP_C_R_C;
  rop->zInstr_LDXP_C_R_C = op;
}

static void sailgen_Instr_LDXR_C_R_C(struct zinstr_ast *rop, struct ztuple_z8z5bv1zCz0z5bv5zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_LDXR_C_R_C;
  rop->zInstr_LDXR_C_R_C = op;
}

static void sailgen_Instr_Other(struct zinstr_ast *rop, uint64_t op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_Other;
  rop->zInstr_Other = op;
}

static void sailgen_Instr_RETR_C_C(struct zinstr_ast *rop, struct ztuple_z8z5bv2zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_RETR_C_C;
  rop->zInstr_RETR_C_C = op;
}

static void sailgen_Instr_RETS_C_C(struct zinstr_ast *rop, struct ztuple_z8z5bv2zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_RETS_C_C;
  rop->zInstr_RETS_C_C = op;
}

static void sailgen_Instr_RETS_C_C_C(struct zinstr_ast *rop, struct ztuple_z8z5bv5zCz0z5bv2zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_RETS_C_C_C;
  rop->zInstr_RETS_C_C_C = op;
}

static void sailgen_Instr_RET_C_C(struct zinstr_ast *rop, struct ztuple_z8z5bv2zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_RET_C_C;
  rop->zInstr_RET_C_C = op;
}

static void sailgen_Instr_SWPAL_CC_R_C(struct zinstr_ast *rop, struct ztuple_z8z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_SWPAL_CC_R_C;
  rop->zInstr_SWPAL_CC_R_C = op;
}

static void sailgen_Instr_SWPA_CC_R_C(struct zinstr_ast *rop, struct ztuple_z8z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_SWPA_CC_R_C;
  rop->zInstr_SWPA_CC_R_C = op;
}

static void sailgen_Instr_SWPL_CC_R_C(struct zinstr_ast *rop, struct ztuple_z8z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_SWPL_CC_R_C;
  rop->zInstr_SWPL_CC_R_C = op;
}

static void sailgen_Instr_SWP_CC_R_C(struct zinstr_ast *rop, struct ztuple_z8z5bv1zCz0z5bv1zCz0z5bv5zCz0z5bv5zCz0z5bv5z9 op)
{
  if (rop->kind == Kind_zInstr_ALDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDR_C_RUI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_ALDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BLR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BRS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_CI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_BR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASAL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASA_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CASL_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_CAS_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAPR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDAXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDCT_R_R__) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDNP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBLR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDPBR_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_CC_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDP_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_I_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIAW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RIBW_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RRB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDR_C_RUIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDTR_C_RIB_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDUR_C_RI_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXP_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_LDXR_C_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_Other) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETR_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RETS_C_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_RET_C_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPAL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPA_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWPL_CC_R_C) {
    /* do nothing */
  } else if (rop->kind == Kind_zInstr_SWP_CC_R_C){
    /* do nothing */
  }
  rop->kind = Kind_zInstr_SWP_CC_R_C;
  rop->zInstr_SWP_CC_R_C = op;
}

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

static void CREATE(zexception)(struct zexception *op)
{
  op->kind = Kind_zError_ConstrainedUnpredictable;

}

static void RECREATE(zexception)(struct zexception *op) {}

static void KILL(zexception)(struct zexception *op)
{
  if (op->kind == Kind_zError_ConstrainedUnpredictable) {
    /* do nothing */
  } else if (op->kind == Kind_zError_ExceptionTaken) {
    /* do nothing */
  } else if (op->kind == Kind_zError_ImplementationDefined) {
    KILL(sail_string)(&op->zError_ImplementationDefined);
  } else if (op->kind == Kind_zError_ReservedEncoding) {
    /* do nothing */
  } else if (op->kind == Kind_zError_SError) {
    /* do nothing */
  } else if (op->kind == Kind_zError_See) {
    KILL(sail_string)(&op->zError_See);
  } else if (op->kind == Kind_zError_Undefined) {
    /* do nothing */
  } else if (op->kind == Kind_zError_Unpredictable){
    /* do nothing */
  };
}

static void COPY(zexception)(struct zexception *rop, struct zexception op)
{
  if (rop->kind == Kind_zError_ConstrainedUnpredictable) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_ExceptionTaken) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_ImplementationDefined) {
    KILL(sail_string)(&rop->zError_ImplementationDefined);
  } else if (rop->kind == Kind_zError_ReservedEncoding) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_SError) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_See) {
    KILL(sail_string)(&rop->zError_See);
  } else if (rop->kind == Kind_zError_Undefined) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_Unpredictable){
    /* do nothing */
  };
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
  } else if (op.kind == Kind_zError_Unpredictable){
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

static void sailgen_Error_ConstrainedUnpredictable(struct zexception *rop, unit op)
{
  if (rop->kind == Kind_zError_ConstrainedUnpredictable) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_ExceptionTaken) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_ImplementationDefined) {
    KILL(sail_string)(&rop->zError_ImplementationDefined);
  } else if (rop->kind == Kind_zError_ReservedEncoding) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_SError) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_See) {
    KILL(sail_string)(&rop->zError_See);
  } else if (rop->kind == Kind_zError_Undefined) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_Unpredictable){
    /* do nothing */
  }
  rop->kind = Kind_zError_ConstrainedUnpredictable;
  rop->zError_ConstrainedUnpredictable = op;
}

static void sailgen_Error_ExceptionTaken(struct zexception *rop, unit op)
{
  if (rop->kind == Kind_zError_ConstrainedUnpredictable) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_ExceptionTaken) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_ImplementationDefined) {
    KILL(sail_string)(&rop->zError_ImplementationDefined);
  } else if (rop->kind == Kind_zError_ReservedEncoding) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_SError) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_See) {
    KILL(sail_string)(&rop->zError_See);
  } else if (rop->kind == Kind_zError_Undefined) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_Unpredictable){
    /* do nothing */
  }
  rop->kind = Kind_zError_ExceptionTaken;
  rop->zError_ExceptionTaken = op;
}

static void sailgen_Error_ImplementationDefined(struct zexception *rop, sail_string op)
{
  if (rop->kind == Kind_zError_ConstrainedUnpredictable) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_ExceptionTaken) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_ImplementationDefined) {
    KILL(sail_string)(&rop->zError_ImplementationDefined);
  } else if (rop->kind == Kind_zError_ReservedEncoding) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_SError) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_See) {
    KILL(sail_string)(&rop->zError_See);
  } else if (rop->kind == Kind_zError_Undefined) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_Unpredictable){
    /* do nothing */
  }
  rop->kind = Kind_zError_ImplementationDefined;
  CREATE(sail_string)(&rop->zError_ImplementationDefined);
  COPY(sail_string)(&rop->zError_ImplementationDefined, op);

}

static void sailgen_Error_ReservedEncoding(struct zexception *rop, unit op)
{
  if (rop->kind == Kind_zError_ConstrainedUnpredictable) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_ExceptionTaken) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_ImplementationDefined) {
    KILL(sail_string)(&rop->zError_ImplementationDefined);
  } else if (rop->kind == Kind_zError_ReservedEncoding) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_SError) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_See) {
    KILL(sail_string)(&rop->zError_See);
  } else if (rop->kind == Kind_zError_Undefined) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_Unpredictable){
    /* do nothing */
  }
  rop->kind = Kind_zError_ReservedEncoding;
  rop->zError_ReservedEncoding = op;
}

static void sailgen_Error_SError(struct zexception *rop, unit op)
{
  if (rop->kind == Kind_zError_ConstrainedUnpredictable) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_ExceptionTaken) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_ImplementationDefined) {
    KILL(sail_string)(&rop->zError_ImplementationDefined);
  } else if (rop->kind == Kind_zError_ReservedEncoding) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_SError) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_See) {
    KILL(sail_string)(&rop->zError_See);
  } else if (rop->kind == Kind_zError_Undefined) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_Unpredictable){
    /* do nothing */
  }
  rop->kind = Kind_zError_SError;
  rop->zError_SError = op;
}

static void sailgen_Error_See(struct zexception *rop, sail_string op)
{
  if (rop->kind == Kind_zError_ConstrainedUnpredictable) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_ExceptionTaken) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_ImplementationDefined) {
    KILL(sail_string)(&rop->zError_ImplementationDefined);
  } else if (rop->kind == Kind_zError_ReservedEncoding) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_SError) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_See) {
    KILL(sail_string)(&rop->zError_See);
  } else if (rop->kind == Kind_zError_Undefined) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_Unpredictable){
    /* do nothing */
  }
  rop->kind = Kind_zError_See;
  CREATE(sail_string)(&rop->zError_See);
  COPY(sail_string)(&rop->zError_See, op);

}

static void sailgen_Error_Undefined(struct zexception *rop, unit op)
{
  if (rop->kind == Kind_zError_ConstrainedUnpredictable) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_ExceptionTaken) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_ImplementationDefined) {
    KILL(sail_string)(&rop->zError_ImplementationDefined);
  } else if (rop->kind == Kind_zError_ReservedEncoding) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_SError) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_See) {
    KILL(sail_string)(&rop->zError_See);
  } else if (rop->kind == Kind_zError_Undefined) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_Unpredictable){
    /* do nothing */
  }
  rop->kind = Kind_zError_Undefined;
  rop->zError_Undefined = op;
}

static void sailgen_Error_Unpredictable(struct zexception *rop, unit op)
{
  if (rop->kind == Kind_zError_ConstrainedUnpredictable) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_ExceptionTaken) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_ImplementationDefined) {
    KILL(sail_string)(&rop->zError_ImplementationDefined);
  } else if (rop->kind == Kind_zError_ReservedEncoding) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_SError) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_See) {
    KILL(sail_string)(&rop->zError_See);
  } else if (rop->kind == Kind_zError_Undefined) {
    /* do nothing */
  } else if (rop->kind == Kind_zError_Unpredictable){
    /* do nothing */
  }
  rop->kind = Kind_zError_Unpredictable;
  rop->zError_Unpredictable = op;
}

struct zexception *current_exception = NULL;
bool have_exception = false;
sail_string *throw_location = NULL;

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

// enum __InstrEnc
enum z__InstrEnc { z__A64, z__A32, z__T16, z__T32 };

static bool eq_z__InstrEnc(enum z__InstrEnc op1, enum z__InstrEnc op2) { return op1 == op2; }

static enum z__InstrEnc UNDEFINED(z__InstrEnc)(unit u) { return z__A64; }

// enum VirtualAddressType
enum zVirtualAddressType { zVA_Bits64, zVA_Capability };

static bool eq_zVirtualAddressType(enum zVirtualAddressType op1, enum zVirtualAddressType op2) { return op1 == op2; }

static enum zVirtualAddressType UNDEFINED(zVirtualAddressType)(unit u) { return zVA_Bits64; }

// enum VBitOp
enum zVBitOp { zVBitOp_VBIF, zVBitOp_VBIT, zVBitOp_VBSL, zVBitOp_VEOR };

static bool eq_zVBitOp(enum zVBitOp op1, enum zVBitOp op2) { return op1 == op2; }

static enum zVBitOp UNDEFINED(zVBitOp)(unit u) { return zVBitOp_VBIF; }

// enum Unpredictable
enum zUnpredictable { zUnpredictable_WBOVERLAPLD, zUnpredictable_WBOVERLAPST, zUnpredictable_LDPOVERLAP, zUnpredictable_BASEOVERLAP, zUnpredictable_DATAOVERLAP, zUnpredictable_DEVPAGE2, zUnpredictable_INSTRDEVICE, zUnpredictable_RESCPACR, zUnpredictable_RESMAIR, zUnpredictable_RESTEXCB, zUnpredictable_RESPRRR, zUnpredictable_RESDACR, zUnpredictable_RESVTCRS, zUnpredictable_RESTnSZ, zUnpredictable_LARGEIPA, zUnpredictable_ESRCONDPASS, zUnpredictable_ILZEROIT, zUnpredictable_ILZEROT, zUnpredictable_BPVECTORCATCHPRI, zUnpredictable_VCMATCHHALF, zUnpredictable_VCMATCHDAPA, zUnpredictable_WPMASKANDBAS, zUnpredictable_WPBASCONTIGUOUS, zUnpredictable_RESWPMASK, zUnpredictable_WPMASKEDBITS, zUnpredictable_RESBPWPCTRL, zUnpredictable_BPNOTIMPL, zUnpredictable_RESBPTYPE, zUnpredictable_BPNOTCTXCMP, zUnpredictable_BPMATCHHALF, zUnpredictable_BPMISMATCHHALF, zUnpredictable_RESTARTALIGNPC, zUnpredictable_RESTARTZEROUPPERPC, zUnpredictable_ZEROUPPER, zUnpredictable_ERETZEROUPPERPC, zUnpredictable_A32FORCEALIGNPC, zUnpredictable_SMD, zUnpredictable_AFUPDATE, zUnpredictable_IESBinDebug, zUnpredictable_BADPMSFCR, zUnpredictable_LINKTRANSFEROVERLAPLD, zUnpredictable_LINKBASEOVERLAPLD, zUnpredictable_CLEARERRITEZERO };

static bool eq_zUnpredictable(enum zUnpredictable op1, enum zUnpredictable op2) { return op1 == op2; }

static enum zUnpredictable UNDEFINED(zUnpredictable)(unit u) { return zUnpredictable_WBOVERLAPLD; }

// enum SystemHintOp
enum zSystemHintOp { zSystemHintOp_NOP, zSystemHintOp_YIELD, zSystemHintOp_WFE, zSystemHintOp_WFI, zSystemHintOp_SEV, zSystemHintOp_SEVL, zSystemHintOp_ESB, zSystemHintOp_PSB, zSystemHintOp_CSDB };

static bool eq_zSystemHintOp(enum zSystemHintOp op1, enum zSystemHintOp op2) { return op1 == op2; }

static enum zSystemHintOp UNDEFINED(zSystemHintOp)(unit u) { return zSystemHintOp_NOP; }

// enum ShiftType
enum zShiftType { zShiftType_LSL, zShiftType_LSR, zShiftType_ASR, zShiftType_ROR };

static bool eq_zShiftType(enum zShiftType op1, enum zShiftType op2) { return op1 == op2; }

static enum zShiftType UNDEFINED(zShiftType)(unit u) { return zShiftType_LSL; }

// enum ReduceOp
enum zReduceOp { zReduceOp_FMINNUM, zReduceOp_FMAXNUM, zReduceOp_FMIN, zReduceOp_FMAX, zReduceOp_FADD, zReduceOp_ADD };

static bool eq_zReduceOp(enum zReduceOp op1, enum zReduceOp op2) { return op1 == op2; }

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

static bool eq_zPrefetchHint(enum zPrefetchHint op1, enum zPrefetchHint op2) { return op1 == op2; }

static enum zPrefetchHint UNDEFINED(zPrefetchHint)(unit u) { return zPrefetch_READ; }

// enum PSTATEField
enum zPSTATEField { zPSTATEField_DAIFSet, zPSTATEField_DAIFClr, zPSTATEField_PAN, zPSTATEField_UAO, zPSTATEField_SSBS, zPSTATEField_SP };

static bool eq_zPSTATEField(enum zPSTATEField op1, enum zPSTATEField op2) { return op1 == op2; }

static enum zPSTATEField UNDEFINED(zPSTATEField)(unit u) { return zPSTATEField_DAIFSet; }

// enum MoveWideOp
enum zMoveWideOp { zMoveWideOp_N, zMoveWideOp_Z, zMoveWideOp_K };

static bool eq_zMoveWideOp(enum zMoveWideOp op1, enum zMoveWideOp op2) { return op1 == op2; }

static enum zMoveWideOp UNDEFINED(zMoveWideOp)(unit u) { return zMoveWideOp_N; }

// enum MemType
enum zMemType { zMemType_Normal, zMemType_Device };

static bool eq_zMemType(enum zMemType op1, enum zMemType op2) { return op1 == op2; }

static enum zMemType UNDEFINED(zMemType)(unit u) { return zMemType_Normal; }

// enum MemOp
enum zMemOp { zMemOp_LOAD, zMemOp_STORE, zMemOp_PREFETCH };

static bool eq_zMemOp(enum zMemOp op1, enum zMemOp op2) { return op1 == op2; }

static enum zMemOp UNDEFINED(zMemOp)(unit u) { return zMemOp_LOAD; }

// enum MemAtomicOp
enum zMemAtomicOp { zMemAtomicOp_ADD, zMemAtomicOp_BIC, zMemAtomicOp_EOR, zMemAtomicOp_ORR, zMemAtomicOp_SMAX, zMemAtomicOp_SMIN, zMemAtomicOp_UMAX, zMemAtomicOp_UMIN, zMemAtomicOp_SWP };

static bool eq_zMemAtomicOp(enum zMemAtomicOp op1, enum zMemAtomicOp op2) { return op1 == op2; }

static enum zMemAtomicOp UNDEFINED(zMemAtomicOp)(unit u) { return zMemAtomicOp_ADD; }

// enum MBReqTypes
enum zMBReqTypes { zMBReqTypes_Reads, zMBReqTypes_Writes, zMBReqTypes_All };

static bool eq_zMBReqTypes(enum zMBReqTypes op1, enum zMBReqTypes op2) { return op1 == op2; }

static enum zMBReqTypes UNDEFINED(zMBReqTypes)(unit u) { return zMBReqTypes_Reads; }

// enum MBReqDomain
enum zMBReqDomain { zMBReqDomain_Nonshareable, zMBReqDomain_InnerShareable, zMBReqDomain_OuterShareable, zMBReqDomain_FullSystem };

static bool eq_zMBReqDomain(enum zMBReqDomain op1, enum zMBReqDomain op2) { return op1 == op2; }

static enum zMBReqDomain UNDEFINED(zMBReqDomain)(unit u) { return zMBReqDomain_Nonshareable; }

// enum LogicalOp
enum zLogicalOp { zLogicalOp_AND, zLogicalOp_EOR, zLogicalOp_ORR };

static bool eq_zLogicalOp(enum zLogicalOp op1, enum zLogicalOp op2) { return op1 == op2; }

static enum zLogicalOp UNDEFINED(zLogicalOp)(unit u) { return zLogicalOp_AND; }

// enum ImmediateOp
enum zImmediateOp { zImmediateOp_MOVI, zImmediateOp_MVNI, zImmediateOp_ORR, zImmediateOp_BIC };

static bool eq_zImmediateOp(enum zImmediateOp op1, enum zImmediateOp op2) { return op1 == op2; }

static enum zImmediateOp UNDEFINED(zImmediateOp)(unit u) { return zImmediateOp_MOVI; }

// enum Fault
enum zFault { zFault_None, zFault_AccessFlag, zFault_Alignment, zFault_Background, zFault_Domain, zFault_Permission, zFault_Translation, zFault_AddressSizze, zFault_SyncExternal, zFault_SyncExternalOnWalk, zFault_SyncParity, zFault_SyncParityOnWalk, zFault_AsyncParity, zFault_AsyncExternal, zFault_Debug, zFault_TLBConflict, zFault_HWUpdateAccessFlag, zFault_CapTag, zFault_CapSeal, zFault_CapBounds, zFault_CapPerm, zFault_CapPagePerm, zFault_Lockdown, zFault_Exclusive, zFault_ICacheMaint };

static bool eq_zFault(enum zFault op1, enum zFault op2) { return op1 == op2; }

static enum zFault UNDEFINED(zFault)(unit u) { return zFault_None; }

// enum FPUnaryOp
enum zFPUnaryOp { zFPUnaryOp_ABS, zFPUnaryOp_MOV, zFPUnaryOp_NEG, zFPUnaryOp_SQRT };

static bool eq_zFPUnaryOp(enum zFPUnaryOp op1, enum zFPUnaryOp op2) { return op1 == op2; }

static enum zFPUnaryOp UNDEFINED(zFPUnaryOp)(unit u) { return zFPUnaryOp_ABS; }

// enum FPType
enum zFPType { zFPType_Nonzzero, zFPType_Zero, zFPType_Infinity, zFPType_QNaN, zFPType_SNaN };

static bool eq_zFPType(enum zFPType op1, enum zFPType op2) { return op1 == op2; }

static enum zFPType UNDEFINED(zFPType)(unit u) { return zFPType_Nonzzero; }

// enum FPRounding
enum zFPRounding { zFPRounding_TIEEVEN, zFPRounding_POSINF, zFPRounding_NEGINF, zFPRounding_ZERO, zFPRounding_TIEAWAY, zFPRounding_ODD };

static bool eq_zFPRounding(enum zFPRounding op1, enum zFPRounding op2) { return op1 == op2; }

static enum zFPRounding UNDEFINED(zFPRounding)(unit u) { return zFPRounding_TIEEVEN; }

// enum FPMaxMinOp
enum zFPMaxMinOp { zFPMaxMinOp_MAX, zFPMaxMinOp_MIN, zFPMaxMinOp_MAXNUM, zFPMaxMinOp_MINNUM };

static bool eq_zFPMaxMinOp(enum zFPMaxMinOp op1, enum zFPMaxMinOp op2) { return op1 == op2; }

static enum zFPMaxMinOp UNDEFINED(zFPMaxMinOp)(unit u) { return zFPMaxMinOp_MAX; }

// enum FPExc
enum zFPExc { zFPExc_InvalidOp, zFPExc_DivideByZero, zFPExc_Overflow, zFPExc_Underflow, zFPExc_Inexact, zFPExc_InputDenorm };

static bool eq_zFPExc(enum zFPExc op1, enum zFPExc op2) { return op1 == op2; }

static enum zFPExc UNDEFINED(zFPExc)(unit u) { return zFPExc_InvalidOp; }

// enum FPConvOp
enum zFPConvOp { zFPConvOp_CVT_FtoI, zFPConvOp_CVT_ItoF, zFPConvOp_MOV_FtoI, zFPConvOp_MOV_ItoF };

static bool eq_zFPConvOp(enum zFPConvOp op1, enum zFPConvOp op2) { return op1 == op2; }

static enum zFPConvOp UNDEFINED(zFPConvOp)(unit u) { return zFPConvOp_CVT_FtoI; }

// enum ExtendType
enum zExtendType { zExtendType_SXTB, zExtendType_SXTH, zExtendType_SXTW, zExtendType_SXTX, zExtendType_UXTB, zExtendType_UXTH, zExtendType_UXTW, zExtendType_UXTX };

static bool eq_zExtendType(enum zExtendType op1, enum zExtendType op2) { return op1 == op2; }

static enum zExtendType UNDEFINED(zExtendType)(unit u) { return zExtendType_SXTB; }

// enum Exception
enum zException { zException_Uncategorizzed, zException_WFxTrap, zException_CP15RTTrap, zException_CP15RRTTrap, zException_CP14RTTrap, zException_CP14DTTrap, zException_AdvSIMDFPAccessTrap, zException_FPIDTrap, zException_CP14RRTTrap, zException_IllegalState, zException_SupervisorCall, zException_HypervisorCall, zException_MonitorCall, zException_SystemRegisterTrap, zException_InstructionAbort, zException_PCAlignment, zException_DataAbort, zException_SPAlignment, zException_FPTrappedException, zException_SError, zException_Breakpoint, zException_SoftwareStep, zException_Watchpoint, zException_SoftwareBreakpoint, zException_VectorCatch, zException_IRQ, zException_CapabilitySysRegTrap, zException_CapabilityAccess, zException_FIQ };

static bool eq_zException(enum zException op1, enum zException op2) { return op1 == op2; }

static enum zException UNDEFINED(zException)(unit u) { return zException_Uncategorizzed; }

// enum DeviceType
enum zDeviceType { zDeviceType_GRE, zDeviceType_nGRE, zDeviceType_nGnRE, zDeviceType_nGnRnE };

static bool eq_zDeviceType(enum zDeviceType op1, enum zDeviceType op2) { return op1 == op2; }

static enum zDeviceType UNDEFINED(zDeviceType)(unit u) { return zDeviceType_GRE; }

// enum CrossTriggerIn
enum zCrossTriggerIn { zCrossTriggerIn_CrossHalt, zCrossTriggerIn_PMUOverflow, zCrossTriggerIn_RSVD2, zCrossTriggerIn_RSVD3, zCrossTriggerIn_TraceExtOut0, zCrossTriggerIn_TraceExtOut1, zCrossTriggerIn_TraceExtOut2, zCrossTriggerIn_TraceExtOut3 };

static bool eq_zCrossTriggerIn(enum zCrossTriggerIn op1, enum zCrossTriggerIn op2) { return op1 == op2; }

static enum zCrossTriggerIn UNDEFINED(zCrossTriggerIn)(unit u) { return zCrossTriggerIn_CrossHalt; }

// enum CountOp
enum zCountOp { zCountOp_CLZ, zCountOp_CLS, zCountOp_CNT };

static bool eq_zCountOp(enum zCountOp op1, enum zCountOp op2) { return op1 == op2; }

static enum zCountOp UNDEFINED(zCountOp)(unit u) { return zCountOp_CLZ; }

// enum Constraint
enum zConstraint { zConstraint_NONE, zConstraint_UNKNOWN, zConstraint_UNDEF, zConstraint_UNDEFEL0, zConstraint_NOP, zConstraint_TRUE, zConstraint_FALSE, zConstraint_DISABLED, zConstraint_UNCOND, zConstraint_COND, zConstraint_ADDITIONAL_DECODE, zConstraint_WBSUPPRESS, zConstraint_FAULT, zConstraint_FORCE, zConstraint_FORCENOSLCHECK };

static bool eq_zConstraint(enum zConstraint op1, enum zConstraint op2) { return op1 == op2; }

static enum zConstraint UNDEFINED(zConstraint)(unit u) { return zConstraint_NONE; }

// enum CompareOp
enum zCompareOp { zCompareOp_GT, zCompareOp_GE, zCompareOp_EQ, zCompareOp_LE, zCompareOp_LT };

static bool eq_zCompareOp(enum zCompareOp op1, enum zCompareOp op2) { return op1 == op2; }

static enum zCompareOp UNDEFINED(zCompareOp)(unit u) { return zCompareOp_GT; }

// enum BranchType
enum zBranchType { zBranchType_DIRCALL, zBranchType_INDCALL, zBranchType_ERET, zBranchType_DBGEXIT, zBranchType_RET, zBranchType_DIR, zBranchType_INDIR, zBranchType_EXCEPTION, zBranchType_RESET, zBranchType_UNKNOWN };

static bool eq_zBranchType(enum zBranchType op1, enum zBranchType op2) { return op1 == op2; }

static enum zBranchType UNDEFINED(zBranchType)(unit u) { return zBranchType_DIRCALL; }

// enum ArchVersion
enum zArchVersion { zARMv8p0, zARMv8p1, zARMv8p2 };

static bool eq_zArchVersion(enum zArchVersion op1, enum zArchVersion op2) { return op1 == op2; }

static enum zArchVersion UNDEFINED(zArchVersion)(unit u) { return zARMv8p0; }

// enum AccType
enum zAccType { zAccType_NORMAL, zAccType_VEC, zAccType_STREAM, zAccType_VECSTREAM, zAccType_ATOMIC, zAccType_ATOMICRW, zAccType_ORDERED, zAccType_ORDEREDRW, zAccType_ORDEREDATOMIC, zAccType_ORDEREDATOMICRW, zAccType_LIMITEDORDERED, zAccType_UNPRIV, zAccType_IFETCH, zAccType_PTW, zAccType_DC, zAccType_DC_UNPRIV, zAccType_IC, zAccType_DCZVA, zAccType_AT };

static bool eq_zAccType(enum zAccType op1, enum zAccType op2) { return op1 == op2; }

static enum zAccType UNDEFINED(zAccType)(unit u) { return zAccType_NORMAL; }





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

static void RECREATE(zz5vecz8z5bv64z9)(zz5vecz8z5bv64z9 *rop) { KILL(zz5vecz8z5bv64z9)(rop); CREATE(zz5vecz8z5bv64z9)(rop); }

static void undefined_vector_zz5vecz8z5bv64z9(zz5vecz8z5bv64z9 *rop, sail_int len, uint64_t elem) {
  rop->len = sail_int_get_ui(len);
  rop->data = sail_malloc((rop->len) * sizeof(uint64_t));
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
  rop->data = sail_malloc((rop->len) * sizeof(uint64_t));
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
  bool result = true;  for (int i = 0; i < op1.len; i++) {
    result &= EQUAL(fbits)(op1.data[i], op2.data[i]);  }
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
  rop->data = sail_malloc(len * sizeof(uint64_t));
}



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

static void RECREATE(zz5vecz8z5bv32z9)(zz5vecz8z5bv32z9 *rop) { KILL(zz5vecz8z5bv32z9)(rop); CREATE(zz5vecz8z5bv32z9)(rop); }

static void undefined_vector_zz5vecz8z5bv32z9(zz5vecz8z5bv32z9 *rop, sail_int len, uint64_t elem) {
  rop->len = sail_int_get_ui(len);
  rop->data = sail_malloc((rop->len) * sizeof(uint64_t));
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
  rop->data = sail_malloc((rop->len) * sizeof(uint64_t));
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
  bool result = true;  for (int i = 0; i < op1.len; i++) {
    result &= EQUAL(fbits)(op1.data[i], op2.data[i]);  }
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
  rop->data = sail_malloc(len * sizeof(uint64_t));
}



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

static void RECREATE(zz5vecz8z5bvz9)(zz5vecz8z5bvz9 *rop) { KILL(zz5vecz8z5bvz9)(rop); CREATE(zz5vecz8z5bvz9)(rop); }

static void undefined_vector_zz5vecz8z5bvz9(zz5vecz8z5bvz9 *rop, sail_int len, lbits elem) {
  rop->len = sail_int_get_ui(len);
  rop->data = sail_malloc((rop->len) * sizeof(lbits));
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
  rop->data = sail_malloc((rop->len) * sizeof(lbits));
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
  bool result = true;  for (int i = 0; i < op1.len; i++) {
    result &= EQUAL(lbits)(op1.data[i], op2.data[i]);  }
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
  rop->data = sail_malloc(len * sizeof(lbits));
  for (int i = 0; i < len; i++) {
    CREATE(lbits)((rop->data) + i);
  }
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









static void sailgen___id(sail_int *rop, sail_int);

static void sailgen___id(sail_int *zcbz31, sail_int zx)
{
  __label__ end_function_4, end_block_exception_5, end_function_273;

  COPY(sail_int)((*(&zcbz31)), zx);
end_function_4: ;
  goto end_function_273;
end_block_exception_5: ;
  goto end_function_273;
end_function_273: ;
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
  __label__ end_function_10, end_block_exception_11, end_function_272;

  RECREATE(lbits)(&zghz30);
  zeros(&zghz30, zn);
  not_bits((*(&zcbz33)), zghz30);
end_function_10: ;
  goto end_function_272;
end_block_exception_11: ;
  goto end_function_272;
end_function_272: ;
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
  __label__ end_function_16, end_block_exception_17, end_function_271;

  sailgen_sail_ones((*(&zcbz35)), zn);
end_function_16: ;
  goto end_function_271;
end_block_exception_17: ;
  goto end_function_271;
end_function_271: ;
}

static void sailgen_Zeros(lbits *rop, sail_int);

static void sailgen_Zeros(lbits *zcbz36, sail_int zn)
{
  __label__ end_function_19, end_block_exception_20, end_function_270;

  zeros((*(&zcbz36)), zn);
end_function_19: ;
  goto end_function_270;
end_block_exception_20: ;
  goto end_function_270;
end_function_270: ;
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
  __label__ end_function_25, end_block_exception_26, end_function_269;

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
  goto end_function_269;
end_block_exception_26: ;
  goto end_function_269;
end_function_269: ;
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
  __label__ end_function_28, end_block_exception_29, end_function_268;

  get_slice_int((*(&zcbz39)), zn, zi, zl);
end_function_28: ;
  goto end_function_268;
end_block_exception_29: ;
  goto end_function_268;
end_function_268: ;
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

static unit sailgen___SetConfig(sail_string, sail_int);

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
sail_int zghz36;
lbits zghz37;
sail_int zghz38;
lbits zghz39;
sail_int zghz310;
lbits zghz311;
sail_int zghz312;
lbits zghz313;
sail_int zghz314;
lbits zghz315;
sail_int zghz316;
lbits zghz317;
sail_int zghz318;
lbits zghz319;
sail_int zghz320;
lbits zghz321;
sail_int zghz322;
lbits zghz323;
sail_int zghz324;
lbits zghz325;
sail_int zghz326;
lbits zghz327;
sail_int zghz328;
lbits zghz329;
sail_int zghz330;
lbits zghz331;
sail_int zghz332;
lbits zghz333;
sail_int zghz334;
lbits zghz335;
sail_int zghz336;
lbits zghz337;
sail_int zghz338;
lbits zghz339;
sail_int zghz340;
lbits zghz341;
sail_int zghz342;
lbits zghz343;
sail_int zghz344;
lbits zghz345;
sail_int zghz346;
lbits zghz347;
sail_int zghz348;
lbits zghz349;
sail_int zghz350;
lbits zghz351;
sail_int zghz352;
lbits zghz353;
sail_int zghz354;
lbits zghz355;
sail_int zghz356;
lbits zghz357;

static void startup_sailgen_undefined_ProcState(void)
{

  CREATE(sail_int)(&zghz36);
  CREATE(lbits)(&zghz37);
  CREATE(sail_int)(&zghz38);
  CREATE(lbits)(&zghz39);
  CREATE(sail_int)(&zghz310);
  CREATE(lbits)(&zghz311);
  CREATE(sail_int)(&zghz312);
  CREATE(lbits)(&zghz313);
  CREATE(sail_int)(&zghz314);
  CREATE(lbits)(&zghz315);
  CREATE(sail_int)(&zghz316);
  CREATE(lbits)(&zghz317);
  CREATE(sail_int)(&zghz318);
  CREATE(lbits)(&zghz319);
  CREATE(sail_int)(&zghz320);
  CREATE(lbits)(&zghz321);
  CREATE(sail_int)(&zghz322);
  CREATE(lbits)(&zghz323);
  CREATE(sail_int)(&zghz324);
  CREATE(lbits)(&zghz325);
  CREATE(sail_int)(&zghz326);
  CREATE(lbits)(&zghz327);
  CREATE(sail_int)(&zghz328);
  CREATE(lbits)(&zghz329);
  CREATE(sail_int)(&zghz330);
  CREATE(lbits)(&zghz331);
  CREATE(sail_int)(&zghz332);
  CREATE(lbits)(&zghz333);
  CREATE(sail_int)(&zghz334);
  CREATE(lbits)(&zghz335);
  CREATE(sail_int)(&zghz336);
  CREATE(lbits)(&zghz337);
  CREATE(sail_int)(&zghz338);
  CREATE(lbits)(&zghz339);
  CREATE(sail_int)(&zghz340);
  CREATE(lbits)(&zghz341);
  CREATE(sail_int)(&zghz342);
  CREATE(lbits)(&zghz343);
  CREATE(sail_int)(&zghz344);
  CREATE(lbits)(&zghz345);
  CREATE(sail_int)(&zghz346);
  CREATE(lbits)(&zghz347);
  CREATE(sail_int)(&zghz348);
  CREATE(lbits)(&zghz349);
  CREATE(sail_int)(&zghz350);
  CREATE(lbits)(&zghz351);
  CREATE(sail_int)(&zghz352);
  CREATE(lbits)(&zghz353);
  CREATE(sail_int)(&zghz354);
  CREATE(lbits)(&zghz355);
  CREATE(sail_int)(&zghz356);
  CREATE(lbits)(&zghz357);
}

static struct zProcState sailgen_undefined_ProcState(unit zgsz34)
{
  __label__ end_function_37, end_block_exception_38;

  struct zProcState zghz35;
  uint64_t zgaz36;
  {
    RECREATE(sail_int)(&zghz356);
    CONVERT_OF(sail_int, mach_int)(&zghz356, INT64_C(1));
    RECREATE(lbits)(&zghz357);
    UNDEFINED(lbits)(&zghz357, zghz356);
    zgaz36 = CONVERT_OF(fbits, lbits)(zghz357, true);
  }
  uint64_t zgaz37;
  {
    RECREATE(sail_int)(&zghz354);
    CONVERT_OF(sail_int, mach_int)(&zghz354, INT64_C(1));
    RECREATE(lbits)(&zghz355);
    UNDEFINED(lbits)(&zghz355, zghz354);
    zgaz37 = CONVERT_OF(fbits, lbits)(zghz355, true);
  }
  uint64_t zgaz38;
  {
    RECREATE(sail_int)(&zghz352);
    CONVERT_OF(sail_int, mach_int)(&zghz352, INT64_C(1));
    RECREATE(lbits)(&zghz353);
    UNDEFINED(lbits)(&zghz353, zghz352);
    zgaz38 = CONVERT_OF(fbits, lbits)(zghz353, true);
  }
  uint64_t zgaz39;
  {
    RECREATE(sail_int)(&zghz350);
    CONVERT_OF(sail_int, mach_int)(&zghz350, INT64_C(1));
    RECREATE(lbits)(&zghz351);
    UNDEFINED(lbits)(&zghz351, zghz350);
    zgaz39 = CONVERT_OF(fbits, lbits)(zghz351, true);
  }
  uint64_t zgaz310;
  {
    RECREATE(sail_int)(&zghz348);
    CONVERT_OF(sail_int, mach_int)(&zghz348, INT64_C(1));
    RECREATE(lbits)(&zghz349);
    UNDEFINED(lbits)(&zghz349, zghz348);
    zgaz310 = CONVERT_OF(fbits, lbits)(zghz349, true);
  }
  uint64_t zgaz311;
  {
    RECREATE(sail_int)(&zghz346);
    CONVERT_OF(sail_int, mach_int)(&zghz346, INT64_C(1));
    RECREATE(lbits)(&zghz347);
    UNDEFINED(lbits)(&zghz347, zghz346);
    zgaz311 = CONVERT_OF(fbits, lbits)(zghz347, true);
  }
  uint64_t zgaz312;
  {
    RECREATE(sail_int)(&zghz344);
    CONVERT_OF(sail_int, mach_int)(&zghz344, INT64_C(1));
    RECREATE(lbits)(&zghz345);
    UNDEFINED(lbits)(&zghz345, zghz344);
    zgaz312 = CONVERT_OF(fbits, lbits)(zghz345, true);
  }
  uint64_t zgaz313;
  {
    RECREATE(sail_int)(&zghz342);
    CONVERT_OF(sail_int, mach_int)(&zghz342, INT64_C(1));
    RECREATE(lbits)(&zghz343);
    UNDEFINED(lbits)(&zghz343, zghz342);
    zgaz313 = CONVERT_OF(fbits, lbits)(zghz343, true);
  }
  uint64_t zgaz314;
  {
    RECREATE(sail_int)(&zghz340);
    CONVERT_OF(sail_int, mach_int)(&zghz340, INT64_C(1));
    RECREATE(lbits)(&zghz341);
    UNDEFINED(lbits)(&zghz341, zghz340);
    zgaz314 = CONVERT_OF(fbits, lbits)(zghz341, true);
  }
  uint64_t zgaz315;
  {
    RECREATE(sail_int)(&zghz338);
    CONVERT_OF(sail_int, mach_int)(&zghz338, INT64_C(1));
    RECREATE(lbits)(&zghz339);
    UNDEFINED(lbits)(&zghz339, zghz338);
    zgaz315 = CONVERT_OF(fbits, lbits)(zghz339, true);
  }
  uint64_t zgaz316;
  {
    RECREATE(sail_int)(&zghz336);
    CONVERT_OF(sail_int, mach_int)(&zghz336, INT64_C(1));
    RECREATE(lbits)(&zghz337);
    UNDEFINED(lbits)(&zghz337, zghz336);
    zgaz316 = CONVERT_OF(fbits, lbits)(zghz337, true);
  }
  uint64_t zgaz317;
  {
    RECREATE(sail_int)(&zghz334);
    CONVERT_OF(sail_int, mach_int)(&zghz334, INT64_C(1));
    RECREATE(lbits)(&zghz335);
    UNDEFINED(lbits)(&zghz335, zghz334);
    zgaz317 = CONVERT_OF(fbits, lbits)(zghz335, true);
  }
  uint64_t zgaz318;
  {
    RECREATE(sail_int)(&zghz332);
    CONVERT_OF(sail_int, mach_int)(&zghz332, INT64_C(1));
    RECREATE(lbits)(&zghz333);
    UNDEFINED(lbits)(&zghz333, zghz332);
    zgaz318 = CONVERT_OF(fbits, lbits)(zghz333, true);
  }
  uint64_t zgaz319;
  {
    RECREATE(sail_int)(&zghz330);
    CONVERT_OF(sail_int, mach_int)(&zghz330, INT64_C(1));
    RECREATE(lbits)(&zghz331);
    UNDEFINED(lbits)(&zghz331, zghz330);
    zgaz319 = CONVERT_OF(fbits, lbits)(zghz331, true);
  }
  uint64_t zgaz320;
  {
    RECREATE(sail_int)(&zghz328);
    CONVERT_OF(sail_int, mach_int)(&zghz328, INT64_C(1));
    RECREATE(lbits)(&zghz329);
    UNDEFINED(lbits)(&zghz329, zghz328);
    zgaz320 = CONVERT_OF(fbits, lbits)(zghz329, true);
  }
  uint64_t zgaz321;
  {
    RECREATE(sail_int)(&zghz326);
    CONVERT_OF(sail_int, mach_int)(&zghz326, INT64_C(2));
    RECREATE(lbits)(&zghz327);
    UNDEFINED(lbits)(&zghz327, zghz326);
    zgaz321 = CONVERT_OF(fbits, lbits)(zghz327, true);
  }
  uint64_t zgaz322;
  {
    RECREATE(sail_int)(&zghz324);
    CONVERT_OF(sail_int, mach_int)(&zghz324, INT64_C(1));
    RECREATE(lbits)(&zghz325);
    UNDEFINED(lbits)(&zghz325, zghz324);
    zgaz322 = CONVERT_OF(fbits, lbits)(zghz325, true);
  }
  uint64_t zgaz323;
  {
    RECREATE(sail_int)(&zghz322);
    CONVERT_OF(sail_int, mach_int)(&zghz322, INT64_C(1));
    RECREATE(lbits)(&zghz323);
    UNDEFINED(lbits)(&zghz323, zghz322);
    zgaz323 = CONVERT_OF(fbits, lbits)(zghz323, true);
  }
  uint64_t zgaz324;
  {
    RECREATE(sail_int)(&zghz320);
    CONVERT_OF(sail_int, mach_int)(&zghz320, INT64_C(1));
    RECREATE(lbits)(&zghz321);
    UNDEFINED(lbits)(&zghz321, zghz320);
    zgaz324 = CONVERT_OF(fbits, lbits)(zghz321, true);
  }
  uint64_t zgaz325;
  {
    RECREATE(sail_int)(&zghz318);
    CONVERT_OF(sail_int, mach_int)(&zghz318, INT64_C(4));
    RECREATE(lbits)(&zghz319);
    UNDEFINED(lbits)(&zghz319, zghz318);
    zgaz325 = CONVERT_OF(fbits, lbits)(zghz319, true);
  }
  uint64_t zgaz326;
  {
    RECREATE(sail_int)(&zghz316);
    CONVERT_OF(sail_int, mach_int)(&zghz316, INT64_C(1));
    RECREATE(lbits)(&zghz317);
    UNDEFINED(lbits)(&zghz317, zghz316);
    zgaz326 = CONVERT_OF(fbits, lbits)(zghz317, true);
  }
  uint64_t zgaz327;
  {
    RECREATE(sail_int)(&zghz314);
    CONVERT_OF(sail_int, mach_int)(&zghz314, INT64_C(8));
    RECREATE(lbits)(&zghz315);
    UNDEFINED(lbits)(&zghz315, zghz314);
    zgaz327 = CONVERT_OF(fbits, lbits)(zghz315, true);
  }
  uint64_t zgaz328;
  {
    RECREATE(sail_int)(&zghz312);
    CONVERT_OF(sail_int, mach_int)(&zghz312, INT64_C(1));
    RECREATE(lbits)(&zghz313);
    UNDEFINED(lbits)(&zghz313, zghz312);
    zgaz328 = CONVERT_OF(fbits, lbits)(zghz313, true);
  }
  uint64_t zgaz329;
  {
    RECREATE(sail_int)(&zghz310);
    CONVERT_OF(sail_int, mach_int)(&zghz310, INT64_C(1));
    RECREATE(lbits)(&zghz311);
    UNDEFINED(lbits)(&zghz311, zghz310);
    zgaz329 = CONVERT_OF(fbits, lbits)(zghz311, true);
  }
  uint64_t zgaz330;
  {
    RECREATE(sail_int)(&zghz38);
    CONVERT_OF(sail_int, mach_int)(&zghz38, INT64_C(1));
    RECREATE(lbits)(&zghz39);
    UNDEFINED(lbits)(&zghz39, zghz38);
    zgaz330 = CONVERT_OF(fbits, lbits)(zghz39, true);
  }
  uint64_t zgaz331;
  {
    RECREATE(sail_int)(&zghz36);
    CONVERT_OF(sail_int, mach_int)(&zghz36, INT64_C(5));
    RECREATE(lbits)(&zghz37);
    UNDEFINED(lbits)(&zghz37, zghz36);
    zgaz331 = CONVERT_OF(fbits, lbits)(zghz37, true);
  }
  zghz35.zA = zgaz311;
  zghz35.zC = zgaz38;
  zghz35.zC64 = zgaz318;
  zghz35.zD = zgaz310;
  zghz35.zDIT = zgaz316;
  zghz35.zE = zgaz330;
  zghz35.zEL = zgaz321;
  zghz35.zF = zgaz313;
  zghz35.zGE = zgaz325;
  zghz35.zI = zgaz312;
  zghz35.zIL = zgaz320;
  zghz35.zIT = zgaz327;
  zghz35.zJ = zgaz328;
  zghz35.zM = zgaz331;
  zghz35.zN = zgaz36;
  zghz35.zPAN = zgaz314;
  zghz35.zQ = zgaz324;
  zghz35.zSP = zgaz323;
  zghz35.zSS = zgaz319;
  zghz35.zSSBS = zgaz326;
  zghz35.zT = zgaz329;
  zghz35.zTCO = zgaz317;
  zghz35.zUAO = zgaz315;
  zghz35.zV = zgaz39;
  zghz35.zZ = zgaz37;
  zghz35.znRW = zgaz322;


























end_function_37: ;
  return zghz35;
end_block_exception_38: ;
  struct zProcState zcbz336 = { .znRW = UINT64_C(0xdeadc0de), .zZ = UINT64_C(0xdeadc0de), .zV = UINT64_C(0xdeadc0de), .zUAO = UINT64_C(0xdeadc0de), .zTCO = UINT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zSSBS = UINT64_C(0xdeadc0de), .zSS = UINT64_C(0xdeadc0de), .zSP = UINT64_C(0xdeadc0de), .zQ = UINT64_C(0xdeadc0de), .zPAN = UINT64_C(0xdeadc0de), .zN = UINT64_C(0xdeadc0de), .zM = UINT64_C(0xdeadc0de), .zJ = UINT64_C(0xdeadc0de), .zIT = UINT64_C(0xdeadc0de), .zIL = UINT64_C(0xdeadc0de), .zI = UINT64_C(0xdeadc0de), .zGE = UINT64_C(0xdeadc0de), .zF = UINT64_C(0xdeadc0de), .zEL = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zDIT = UINT64_C(0xdeadc0de), .zD = UINT64_C(0xdeadc0de), .zC64 = UINT64_C(0xdeadc0de), .zC = UINT64_C(0xdeadc0de), .zA = UINT64_C(0xdeadc0de) };
  return zcbz336;
}



static void finish_sailgen_undefined_ProcState(void)
{
  KILL(lbits)(&zghz357);
  KILL(sail_int)(&zghz356);
  KILL(lbits)(&zghz355);
  KILL(sail_int)(&zghz354);
  KILL(lbits)(&zghz353);
  KILL(sail_int)(&zghz352);
  KILL(lbits)(&zghz351);
  KILL(sail_int)(&zghz350);
  KILL(lbits)(&zghz349);
  KILL(sail_int)(&zghz348);
  KILL(lbits)(&zghz347);
  KILL(sail_int)(&zghz346);
  KILL(lbits)(&zghz345);
  KILL(sail_int)(&zghz344);
  KILL(lbits)(&zghz343);
  KILL(sail_int)(&zghz342);
  KILL(lbits)(&zghz341);
  KILL(sail_int)(&zghz340);
  KILL(lbits)(&zghz339);
  KILL(sail_int)(&zghz338);
  KILL(lbits)(&zghz337);
  KILL(sail_int)(&zghz336);
  KILL(lbits)(&zghz335);
  KILL(sail_int)(&zghz334);
  KILL(lbits)(&zghz333);
  KILL(sail_int)(&zghz332);
  KILL(lbits)(&zghz331);
  KILL(sail_int)(&zghz330);
  KILL(lbits)(&zghz329);
  KILL(sail_int)(&zghz328);
  KILL(lbits)(&zghz327);
  KILL(sail_int)(&zghz326);
  KILL(lbits)(&zghz325);
  KILL(sail_int)(&zghz324);
  KILL(lbits)(&zghz323);
  KILL(sail_int)(&zghz322);
  KILL(lbits)(&zghz321);
  KILL(sail_int)(&zghz320);
  KILL(lbits)(&zghz319);
  KILL(sail_int)(&zghz318);
  KILL(lbits)(&zghz317);
  KILL(sail_int)(&zghz316);
  KILL(lbits)(&zghz315);
  KILL(sail_int)(&zghz314);
  KILL(lbits)(&zghz313);
  KILL(sail_int)(&zghz312);
  KILL(lbits)(&zghz311);
  KILL(sail_int)(&zghz310);
  KILL(lbits)(&zghz39);
  KILL(sail_int)(&zghz38);
  KILL(lbits)(&zghz37);
  KILL(sail_int)(&zghz36);

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
  int64_t zgaz332;
  {
    sail_int zgsz3608;
    CREATE(sail_int)(&zgsz3608);
    CONVERT_OF(sail_int, mach_int)(&zgsz3608, zCAP_VALUE_HI_BIT);
    sail_int zgsz3609;
    CREATE(sail_int)(&zgsz3609);
    CONVERT_OF(sail_int, mach_int)(&zgsz3609, zCAP_VALUE_LO_BIT);
    sail_int zgsz3610;
    CREATE(sail_int)(&zgsz3610);
    sub_int(&zgsz3610, zgsz3608, zgsz3609);
    zgaz332 = CONVERT_OF(mach_int, sail_int)(zgsz3610);
    KILL(sail_int)(&zgsz3610);
    KILL(sail_int)(&zgsz3609);
    KILL(sail_int)(&zgsz3608);
  }
  zgsz310 = (zgaz332 + INT64_C(1));

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
  int64_t zgaz333;
  {
    sail_int zgsz3611;
    CREATE(sail_int)(&zgsz3611);
    CONVERT_OF(sail_int, mach_int)(&zgsz3611, zCAP_BASE_HI_BIT);
    sail_int zgsz3612;
    CREATE(sail_int)(&zgsz3612);
    CONVERT_OF(sail_int, mach_int)(&zgsz3612, zCAP_BASE_LO_BIT);
    sail_int zgsz3613;
    CREATE(sail_int)(&zgsz3613);
    sub_int(&zgsz3613, zgsz3611, zgsz3612);
    zgaz333 = CONVERT_OF(mach_int, sail_int)(zgsz3613);
    KILL(sail_int)(&zgsz3613);
    KILL(sail_int)(&zgsz3612);
    KILL(sail_int)(&zgsz3611);
  }
  zgsz313 = (zgaz333 + INT64_C(1));

  zCAP_MW = zgsz313;

let_end_46: ;
}
static void kill_letbind_10(void) {
}

static bool sailgen_CapBoundsUsesValue(sail_int);

sail_int zghz358;
sail_int zghz359;
sail_int zghz360;

static void startup_sailgen_CapBoundsUsesValue(void)
{
  CREATE(sail_int)(&zghz358);
  CREATE(sail_int)(&zghz359);
  CREATE(sail_int)(&zghz360);
}

static bool sailgen_CapBoundsUsesValue(sail_int zexp)
{
  __label__ cleanup_49, end_cleanup_50, end_function_48, end_block_exception_51;

  bool zcbz312;
  {
    RECREATE(sail_int)(&zghz358);
    {
      RECREATE(sail_int)(&zghz360);
      CONVERT_OF(sail_int, mach_int)(&zghz360, zCAP_MW);
      add_int(&zghz358, zexp, zghz360);
    }
    {
      RECREATE(sail_int)(&zghz359);
      CONVERT_OF(sail_int, mach_int)(&zghz359, zCAP_VALUE_NUM_BITS);
      zcbz312 = lt(zghz358, zghz359);
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
  KILL(sail_int)(&zghz360);
  KILL(sail_int)(&zghz359);
  KILL(sail_int)(&zghz358);
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

sail_int zghz361;

static void startup_sailgen_CapIsInternalExponent(void)
{    CREATE(sail_int)(&zghz361);
}

static bool sailgen_CapIsInternalExponent(lbits zc)
{
  __label__ cleanup_58, end_cleanup_59, end_function_57, end_block_exception_60;

  bool zcbz313;
  {
    uint64_t zgaz337;
    {
      fbits zgaz336;
      {
        RECREATE(sail_int)(&zghz361);
        CONVERT_OF(sail_int, mach_int)(&zghz361, zCAP_IE_BIT);
        zgaz336 = bitvector_access(zc, zghz361);
      }
      zgaz337 = UINT64_C(0b0);
      zgaz337 = update_fbits(zgaz337, INT64_C(0), zgaz336);
    }
    zcbz313 = (zgaz337 == UINT64_C(0b0));
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
{    KILL(sail_int)(&zghz361);
}

static int64_t sailgen_CapGetExponent(lbits);

lbits zghz362;
sail_int zghz363;
sail_int zghz364;
sail_int zghz365;
lbits zghz366;
sail_int zghz367;
sail_int zghz368;
lbits zghz369;

static void startup_sailgen_CapGetExponent(void)
{
  CREATE(lbits)(&zghz362);
  CREATE(sail_int)(&zghz363);
  CREATE(sail_int)(&zghz364);
  CREATE(sail_int)(&zghz365);
  CREATE(lbits)(&zghz366);
  CREATE(sail_int)(&zghz367);
  CREATE(sail_int)(&zghz368);
  CREATE(lbits)(&zghz369);
}

static int64_t sailgen_CapGetExponent(lbits zc)
{
  __label__ end_function_62, end_block_exception_67;

  int64_t zcbz314;
  bool zgaz339;
  zgaz339 = sailgen_CapIsInternalExponent(zc);
  if (zgaz339) {
    __label__ cleanup_65, end_cleanup_66;

    uint64_t znexp;
    {
      uint64_t zgaz342;
      {
        RECREATE(sail_int)(&zghz367);
        CONVERT_OF(sail_int, mach_int)(&zghz367, zCAP_LIMIT_EXP_HI_BIT);
        RECREATE(sail_int)(&zghz368);
        CONVERT_OF(sail_int, mach_int)(&zghz368, zCAP_LIMIT_LO_BIT);
        RECREATE(lbits)(&zghz369);
        vector_subrange_lbits(&zghz369, zc, zghz367, zghz368);
        zgaz342 = CONVERT_OF(fbits, lbits)(zghz369, true);
      }
      uint64_t zgaz343;
      {
        RECREATE(sail_int)(&zghz364);
        CONVERT_OF(sail_int, mach_int)(&zghz364, zCAP_BASE_EXP_HI_BIT);
        RECREATE(sail_int)(&zghz365);
        CONVERT_OF(sail_int, mach_int)(&zghz365, zCAP_BASE_LO_BIT);
        RECREATE(lbits)(&zghz366);
        vector_subrange_lbits(&zghz366, zc, zghz364, zghz365);
        zgaz343 = CONVERT_OF(fbits, lbits)(zghz366, true);
      }
      znexp = (zgaz342 << 3) | zgaz343;
    }
    int64_t zgaz341;
    {
      uint64_t zgaz340;
      zgaz340 = (~(znexp) & UINT64_C(0b111111));
      {
        RECREATE(lbits)(&zghz362);
        CONVERT_OF(lbits, fbits)(&zghz362, zgaz340, UINT64_C(6) , true);
        RECREATE(sail_int)(&zghz363);
        sail_unsigned(&zghz363, zghz362);
        zgaz341 = CONVERT_OF(mach_int, sail_int)(zghz363);
      }
    }
    zcbz314 = zgaz341;
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
  KILL(lbits)(&zghz369);
  KILL(sail_int)(&zghz368);
  KILL(sail_int)(&zghz367);
  KILL(lbits)(&zghz366);
  KILL(sail_int)(&zghz365);
  KILL(sail_int)(&zghz364);
  KILL(sail_int)(&zghz363);
  KILL(lbits)(&zghz362);
}

static uint64_t sailgen_CapGetValue(lbits);

sail_int zghz370;
sail_int zghz371;
lbits zghz372;

static void startup_sailgen_CapGetValue(void)
{
  CREATE(sail_int)(&zghz370);
  CREATE(sail_int)(&zghz371);
  CREATE(lbits)(&zghz372);
}

static uint64_t sailgen_CapGetValue(lbits zc)
{
  __label__ cleanup_70, end_cleanup_71, end_function_69, end_block_exception_72;

  uint64_t zcbz315;
  {
    RECREATE(sail_int)(&zghz370);
    CONVERT_OF(sail_int, mach_int)(&zghz370, zCAP_VALUE_HI_BIT);
    RECREATE(sail_int)(&zghz371);
    CONVERT_OF(sail_int, mach_int)(&zghz371, zCAP_VALUE_LO_BIT);
    RECREATE(lbits)(&zghz372);
    vector_subrange_lbits(&zghz372, zc, zghz370, zghz371);
    zcbz315 = CONVERT_OF(fbits, lbits)(zghz372, true);
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
  KILL(lbits)(&zghz372);
  KILL(sail_int)(&zghz371);
  KILL(sail_int)(&zghz370);
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
  sail_int zgaz345;
  CREATE(sail_int)(&zgaz345);
  {
    sail_int zgsz3630;
    CREATE(sail_int)(&zgsz3630);
    CONVERT_OF(sail_int, mach_int)(&zgsz3630, INT64_C(1));
    sail_int zgsz3631;
    CREATE(sail_int)(&zgsz3631);
    CONVERT_OF(sail_int, mach_int)(&zgsz3631, zCAP_VALUE_NUM_BITS);
    shl_int(&zgaz345, zgsz3630, zgsz3631);
    KILL(sail_int)(&zgsz3631);
    KILL(sail_int)(&zgsz3630);
  }
  {
    sail_int zgsz3628;
    CREATE(sail_int)(&zgsz3628);
    CONVERT_OF(sail_int, mach_int)(&zgsz3628, INT64_C(0));
    sail_int zgsz3629;
    CREATE(sail_int)(&zgsz3629);
    CONVERT_OF(sail_int, mach_int)(&zgsz3629, zCAP_BOUND_NUM_BITS);
    sailgen_Slice_int(&zgsz320, zgaz345, zgsz3628, zgsz3629);
    KILL(sail_int)(&zgsz3629);
    KILL(sail_int)(&zgsz3628);
  }
  KILL(sail_int)(&zgaz345);
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
  int64_t zgaz346;
  {
    lbits zgsz3635;
    CREATE(lbits)(&zgsz3635);
    CONVERT_OF(lbits, fbits)(&zgsz3635, UINT64_C(0x0), UINT64_C(4) , true);
    sail_int zgsz3636;
    CREATE(sail_int)(&zgsz3636);
    sail_unsigned(&zgsz3636, zgsz3635);
    zgaz346 = CONVERT_OF(mach_int, sail_int)(zgsz3636);
    KILL(sail_int)(&zgsz3636);
    KILL(lbits)(&zgsz3635);
  }
  {
    sail_int zgsz3632;
    CREATE(sail_int)(&zgsz3632);
    CONVERT_OF(sail_int, mach_int)(&zgsz3632, zgaz346);
    sail_int zgsz3633;
    CREATE(sail_int)(&zgsz3633);
    CONVERT_OF(sail_int, mach_int)(&zgsz3633, INT64_C(0));
    sail_int zgsz3634;
    CREATE(sail_int)(&zgsz3634);
    CONVERT_OF(sail_int, mach_int)(&zgsz3634, zCAP_BOUND_NUM_BITS);
    sailgen_Slice_int(&zgsz321, zgsz3632, zgsz3633, zgsz3634);
    KILL(sail_int)(&zgsz3634);
    KILL(sail_int)(&zgsz3633);
    KILL(sail_int)(&zgsz3632);
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
  int64_t zgaz347;
  {
    sail_int zgsz3637;
    CREATE(sail_int)(&zgsz3637);
    CONVERT_OF(sail_int, mach_int)(&zgsz3637, zCAP_VALUE_NUM_BITS);
    sail_int zgsz3638;
    CREATE(sail_int)(&zgsz3638);
    CONVERT_OF(sail_int, mach_int)(&zgsz3638, zCAP_MW);
    sail_int zgsz3639;
    CREATE(sail_int)(&zgsz3639);
    sub_int(&zgsz3639, zgsz3637, zgsz3638);
    zgaz347 = CONVERT_OF(mach_int, sail_int)(zgsz3639);
    KILL(sail_int)(&zgsz3639);
    KILL(sail_int)(&zgsz3638);
    KILL(sail_int)(&zgsz3637);
  }
  zgsz323 = (zgaz347 + INT64_C(2));

  zCAP_MAX_EXPONENT = zgsz323;

let_end_77: ;
}
static void kill_letbind_19(void) {
}

static uint64_t sailgen_CapBoundsAddress(uint64_t);

lbits zghz373;
sail_int zghz374;
lbits zghz375;
sail_int zghz376;
sail_int zghz377;
sail_int zghz378;

static void startup_sailgen_CapBoundsAddress(void)
{
  CREATE(lbits)(&zghz373);
  CREATE(sail_int)(&zghz374);
  CREATE(lbits)(&zghz375);
  CREATE(sail_int)(&zghz376);
  CREATE(sail_int)(&zghz377);
  CREATE(sail_int)(&zghz378);
}

static uint64_t sailgen_CapBoundsAddress(uint64_t zaddress)
{
  __label__ cleanup_80, end_cleanup_81, end_function_79, end_block_exception_82;

  uint64_t zcbz316;
  {
    uint64_t zgaz349;
    {
      int64_t zgaz348;
      {
        RECREATE(sail_int)(&zghz376);
        CONVERT_OF(sail_int, mach_int)(&zghz376, zCAP_FLAGS_LO_BIT);
        RECREATE(sail_int)(&zghz377);
        CONVERT_OF(sail_int, mach_int)(&zghz377, INT64_C(1));
        RECREATE(sail_int)(&zghz378);
        sub_int(&zghz378, zghz376, zghz377);
        zgaz348 = CONVERT_OF(mach_int, sail_int)(zghz378);
      }
      zgaz349 = (safe_rshift(UINT64_MAX, 64 - 56) & (zaddress >> INT64_C(0)));
    }
    {
      RECREATE(lbits)(&zghz373);
      CONVERT_OF(lbits, fbits)(&zghz373, zgaz349, UINT64_C(56) , true);
      RECREATE(sail_int)(&zghz374);
      CONVERT_OF(sail_int, mach_int)(&zghz374, zCAP_VALUE_NUM_BITS);
      RECREATE(lbits)(&zghz375);
      sign_extend(&zghz375, zghz373, zghz374);
      zcbz316 = CONVERT_OF(fbits, lbits)(zghz375, true);
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
  KILL(sail_int)(&zghz378);
  KILL(sail_int)(&zghz377);
  KILL(sail_int)(&zghz376);
  KILL(lbits)(&zghz375);
  KILL(sail_int)(&zghz374);
  KILL(lbits)(&zghz373);
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

sail_int zghz379;
sail_int zghz380;
lbits zghz381;
sail_int zghz382;
sail_int zghz383;
lbits zghz384;

static void startup_sailgen_CapGetBottom(void)
{
  CREATE(sail_int)(&zghz379);
  CREATE(sail_int)(&zghz380);
  CREATE(lbits)(&zghz381);
  CREATE(sail_int)(&zghz382);
  CREATE(sail_int)(&zghz383);
  CREATE(lbits)(&zghz384);
}

static uint64_t sailgen_CapGetBottom(lbits zc)
{
  __label__ end_function_85, end_block_exception_90;

  uint64_t zcbz317;
  bool zgaz351;
  zgaz351 = sailgen_CapIsInternalExponent(zc);
  if (zgaz351) {
    __label__ cleanup_88, end_cleanup_89;

    uint64_t zgaz353;
    {
      uint64_t zgaz352;
      {
        RECREATE(sail_int)(&zghz382);
        CONVERT_OF(sail_int, mach_int)(&zghz382, zCAP_BASE_HI_BIT);
        RECREATE(sail_int)(&zghz383);
        CONVERT_OF(sail_int, mach_int)(&zghz383, zCAP_BASE_MANTISSA_LO_BIT);
        RECREATE(lbits)(&zghz384);
        vector_subrange_lbits(&zghz384, zc, zghz382, zghz383);
        zgaz352 = CONVERT_OF(fbits, lbits)(zghz384, true);
      }
      zgaz353 = (zgaz352 << 3) | UINT64_C(0b000);
    }
    zcbz317 = zgaz353;
    goto cleanup_88;
    /* unreachable after return */
    goto end_cleanup_89;
  cleanup_88: ;
    goto end_function_85;
  end_cleanup_89: ;
  } else {
    __label__ cleanup_86, end_cleanup_87;

    uint64_t zgaz354;
    {
      RECREATE(sail_int)(&zghz379);
      CONVERT_OF(sail_int, mach_int)(&zghz379, zCAP_BASE_HI_BIT);
      RECREATE(sail_int)(&zghz380);
      CONVERT_OF(sail_int, mach_int)(&zghz380, zCAP_BASE_LO_BIT);
      RECREATE(lbits)(&zghz381);
      vector_subrange_lbits(&zghz381, zc, zghz379, zghz380);
      zgaz354 = CONVERT_OF(fbits, lbits)(zghz381, true);
    }
    zcbz317 = zgaz354;
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
  KILL(lbits)(&zghz384);
  KILL(sail_int)(&zghz383);
  KILL(sail_int)(&zghz382);
  KILL(lbits)(&zghz381);
  KILL(sail_int)(&zghz380);
  KILL(sail_int)(&zghz379);
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

sail_int zghz385;
sail_int zghz386;

static void startup_sailgen_CapUnsignedLessThan(void)
{
  CREATE(sail_int)(&zghz385);
  CREATE(sail_int)(&zghz386);
}

static bool sailgen_CapUnsignedLessThan(lbits za, lbits zb)
{
  __label__ cleanup_95, end_cleanup_96, end_function_94, end_block_exception_97;

  bool zcbz318;
  {
    RECREATE(sail_int)(&zghz385);
    sail_unsigned(&zghz385, za);
    RECREATE(sail_int)(&zghz386);
    sail_unsigned(&zghz386, zb);
    zcbz318 = lt(zghz385, zghz386);
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
  KILL(sail_int)(&zghz386);
  KILL(sail_int)(&zghz385);
}

static uint64_t sailgen_CapGetTop(lbits);

lbits zghz387;
sail_int zghz388;
sail_int zghz389;
lbits zghz390;
lbits zghz391;
sail_int zghz392;
sail_int zghz393;
sail_int zghz394;
sail_int zghz395;
sail_int zghz396;
sail_int zghz397;
sail_int zghz398;
sail_int zghz399;
sail_int zghz3100;
sail_int zghz3101;
sail_int zghz3102;
sail_int zghz3103;
lbits zghz3104;
lbits zghz3105;
sail_int zghz3106;
sail_int zghz3107;
sail_int zghz3108;
sail_int zghz3109;
sail_int zghz3110;
sail_int zghz3111;
sail_int zghz3112;
sail_int zghz3113;
lbits zghz3114;
sail_int zghz3115;
sail_int zghz3116;
lbits zghz3117;
sail_int zghz3118;
lbits zghz3119;
sail_int zghz3120;
sail_int zghz3121;
sail_int zghz3122;

static void startup_sailgen_CapGetTop(void)
{
  CREATE(lbits)(&zghz387);
  CREATE(sail_int)(&zghz388);
  CREATE(sail_int)(&zghz389);
  CREATE(lbits)(&zghz390);
  CREATE(lbits)(&zghz391);
  CREATE(sail_int)(&zghz392);
  CREATE(sail_int)(&zghz393);
  CREATE(sail_int)(&zghz394);
  CREATE(sail_int)(&zghz395);
  CREATE(sail_int)(&zghz396);
  CREATE(sail_int)(&zghz397);
  CREATE(sail_int)(&zghz398);
  CREATE(sail_int)(&zghz399);
  CREATE(sail_int)(&zghz3100);
  CREATE(sail_int)(&zghz3101);
  CREATE(sail_int)(&zghz3102);
  CREATE(sail_int)(&zghz3103);
  CREATE(lbits)(&zghz3104);
  CREATE(lbits)(&zghz3105);
  CREATE(sail_int)(&zghz3106);
  CREATE(sail_int)(&zghz3107);
  CREATE(sail_int)(&zghz3108);
  CREATE(sail_int)(&zghz3109);
  CREATE(sail_int)(&zghz3110);
  CREATE(sail_int)(&zghz3111);
  CREATE(sail_int)(&zghz3112);
  CREATE(sail_int)(&zghz3113);
  CREATE(lbits)(&zghz3114);
  CREATE(sail_int)(&zghz3115);
  CREATE(sail_int)(&zghz3116);
  CREATE(lbits)(&zghz3117);
  CREATE(sail_int)(&zghz3118);
  CREATE(lbits)(&zghz3119);
  CREATE(sail_int)(&zghz3120);
  CREATE(sail_int)(&zghz3121);
  CREATE(sail_int)(&zghz3122);
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
  {
    int64_t zgaz375;
    {
      int64_t zgaz374;
      {
        RECREATE(sail_int)(&zghz3120);
        CONVERT_OF(sail_int, mach_int)(&zghz3120, INT64_C(79));
        RECREATE(sail_int)(&zghz3121);
        CONVERT_OF(sail_int, mach_int)(&zghz3121, INT64_C(64));
        RECREATE(sail_int)(&zghz3122);
        sub_int(&zghz3122, zghz3120, zghz3121);
        zgaz374 = CONVERT_OF(mach_int, sail_int)(zghz3122);
      }
      zgaz375 = (zgaz374 + INT64_C(1));
    }
    {
      RECREATE(sail_int)(&zghz3118);
      CONVERT_OF(sail_int, mach_int)(&zghz3118, zgaz375);
      RECREATE(lbits)(&zghz3119);
      UNDEFINED(lbits)(&zghz3119, zghz3118);
      zt = CONVERT_OF(fbits, lbits)(zghz3119, true);
    }
  }
  {
    bool zgaz358;
    zgaz358 = sailgen_CapIsInternalExponent(zc);
    unit zgsz329;
    if (zgaz358) {
      {
        zlmsb = UINT64_C(0b01);
        unit zgsz330;
        zgsz330 = UNIT;
      }
      uint64_t zgaz360;
      {
        uint64_t zgaz359;
        {
          RECREATE(sail_int)(&zghz3115);
          CONVERT_OF(sail_int, mach_int)(&zghz3115, zCAP_LIMIT_HI_BIT);
          RECREATE(sail_int)(&zghz3116);
          CONVERT_OF(sail_int, mach_int)(&zghz3116, zCAP_LIMIT_MANTISSA_LO_BIT);
          RECREATE(lbits)(&zghz3117);
          vector_subrange_lbits(&zghz3117, zc, zghz3115, zghz3116);
          zgaz359 = CONVERT_OF(fbits, lbits)(zghz3117, true);
        }
        zgaz360 = (UINT64_C(0b00) << 11) | zgaz359;
      }
      zt = (zgaz360 << 3) | UINT64_C(0b000);
      zgsz329 = UNIT;
    } else {
      uint64_t zgaz361;
      {
        RECREATE(sail_int)(&zghz3112);
        CONVERT_OF(sail_int, mach_int)(&zghz3112, zCAP_LIMIT_HI_BIT);
        RECREATE(sail_int)(&zghz3113);
        CONVERT_OF(sail_int, mach_int)(&zghz3113, zCAP_LIMIT_LO_BIT);
        RECREATE(lbits)(&zghz3114);
        vector_subrange_lbits(&zghz3114, zc, zghz3112, zghz3113);
        zgaz361 = CONVERT_OF(fbits, lbits)(zghz3114, true);
      }
      zt = (UINT64_C(0b00) << 14) | zgaz361;
      zgsz329 = UNIT;
    }
  }
  {
    bool zgaz366;
    {
      uint64_t zgaz364;
      {
        int64_t zgaz362;
        {
          RECREATE(sail_int)(&zghz3109);
          CONVERT_OF(sail_int, mach_int)(&zghz3109, zCAP_MW);
          RECREATE(sail_int)(&zghz3110);
          CONVERT_OF(sail_int, mach_int)(&zghz3110, INT64_C(3));
          RECREATE(sail_int)(&zghz3111);
          sub_int(&zghz3111, zghz3109, zghz3110);
          zgaz362 = CONVERT_OF(mach_int, sail_int)(zghz3111);
        }
        zgaz364 = (safe_rshift(UINT64_MAX, 64 - 14) & (zt >> INT64_C(0)));
      }
      uint64_t zgaz365;
      {
        int64_t zgaz363;
        {
          RECREATE(sail_int)(&zghz3106);
          CONVERT_OF(sail_int, mach_int)(&zghz3106, zCAP_MW);
          RECREATE(sail_int)(&zghz3107);
          CONVERT_OF(sail_int, mach_int)(&zghz3107, INT64_C(3));
          RECREATE(sail_int)(&zghz3108);
          sub_int(&zghz3108, zghz3106, zghz3107);
          zgaz363 = CONVERT_OF(mach_int, sail_int)(zghz3108);
        }
        zgaz365 = (safe_rshift(UINT64_MAX, 64 - 14) & (zb >> INT64_C(0)));
      }
      {
        RECREATE(lbits)(&zghz3104);
        CONVERT_OF(lbits, fbits)(&zghz3104, zgaz364, UINT64_C(14) , true);
        RECREATE(lbits)(&zghz3105);
        CONVERT_OF(lbits, fbits)(&zghz3105, zgaz365, UINT64_C(14) , true);
        zgaz366 = sailgen_CapUnsignedLessThan(zghz3104, zghz3105);
      }
    }
    unit zgsz328;
    if (zgaz366) {
      zlcarry = UINT64_C(0b01);
      zgsz328 = UNIT;
    } else {  zgsz328 = UNIT;  }
  }
  {
    int64_t zgaz371;
    {
      RECREATE(sail_int)(&zghz3101);
      CONVERT_OF(sail_int, mach_int)(&zghz3101, zCAP_MW);
      RECREATE(sail_int)(&zghz3102);
      CONVERT_OF(sail_int, mach_int)(&zghz3102, INT64_C(1));
      RECREATE(sail_int)(&zghz3103);
      sub_int(&zghz3103, zghz3101, zghz3102);
      zgaz371 = CONVERT_OF(mach_int, sail_int)(zghz3103);
    }
    int64_t zgaz372;
    {
      RECREATE(sail_int)(&zghz398);
      CONVERT_OF(sail_int, mach_int)(&zghz398, zCAP_MW);
      RECREATE(sail_int)(&zghz399);
      CONVERT_OF(sail_int, mach_int)(&zghz399, INT64_C(2));
      RECREATE(sail_int)(&zghz3100);
      sub_int(&zghz3100, zghz398, zghz399);
      zgaz372 = CONVERT_OF(mach_int, sail_int)(zghz3100);
    }
    uint64_t zgaz373;
    {
      uint64_t zgaz370;
      {
        uint64_t zgaz369;
        {
          int64_t zgaz367;
          {
            RECREATE(sail_int)(&zghz395);
            CONVERT_OF(sail_int, mach_int)(&zghz395, zCAP_MW);
            RECREATE(sail_int)(&zghz396);
            CONVERT_OF(sail_int, mach_int)(&zghz396, INT64_C(1));
            RECREATE(sail_int)(&zghz397);
            sub_int(&zghz397, zghz395, zghz396);
            zgaz367 = CONVERT_OF(mach_int, sail_int)(zghz397);
          }
          int64_t zgaz368;
          {
            RECREATE(sail_int)(&zghz392);
            CONVERT_OF(sail_int, mach_int)(&zghz392, zCAP_MW);
            RECREATE(sail_int)(&zghz393);
            CONVERT_OF(sail_int, mach_int)(&zghz393, INT64_C(2));
            RECREATE(sail_int)(&zghz394);
            sub_int(&zghz394, zghz392, zghz393);
            zgaz368 = CONVERT_OF(mach_int, sail_int)(zghz394);
          }
          zgaz369 = (safe_rshift(UINT64_MAX, 64 - 2) & (zb >> zgaz368));
        }
        zgaz370 = ((zgaz369 + zlmsb) & UINT64_C(0b11));
      }
      zgaz373 = ((zgaz370 + zlcarry) & UINT64_C(0b11));
    }
    {
      RECREATE(lbits)(&zghz387);
      CONVERT_OF(lbits, fbits)(&zghz387, zt, UINT64_C(16) , true);
      RECREATE(sail_int)(&zghz388);
      CONVERT_OF(sail_int, mach_int)(&zghz388, zgaz371);
      RECREATE(sail_int)(&zghz389);
      CONVERT_OF(sail_int, mach_int)(&zghz389, zgaz372);
      RECREATE(lbits)(&zghz390);
      CONVERT_OF(lbits, fbits)(&zghz390, zgaz373, UINT64_C(2) , true);
      RECREATE(lbits)(&zghz391);
      vector_update_subrange_lbits(&zghz391, zghz387, zghz388, zghz389, zghz390);
      zt = CONVERT_OF(fbits, lbits)(zghz391, true);
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
  KILL(sail_int)(&zghz3122);
  KILL(sail_int)(&zghz3121);
  KILL(sail_int)(&zghz3120);
  KILL(lbits)(&zghz3119);
  KILL(sail_int)(&zghz3118);
  KILL(lbits)(&zghz3117);
  KILL(sail_int)(&zghz3116);
  KILL(sail_int)(&zghz3115);
  KILL(lbits)(&zghz3114);
  KILL(sail_int)(&zghz3113);
  KILL(sail_int)(&zghz3112);
  KILL(sail_int)(&zghz3111);
  KILL(sail_int)(&zghz3110);
  KILL(sail_int)(&zghz3109);
  KILL(sail_int)(&zghz3108);
  KILL(sail_int)(&zghz3107);
  KILL(sail_int)(&zghz3106);
  KILL(lbits)(&zghz3105);
  KILL(lbits)(&zghz3104);
  KILL(sail_int)(&zghz3103);
  KILL(sail_int)(&zghz3102);
  KILL(sail_int)(&zghz3101);
  KILL(sail_int)(&zghz3100);
  KILL(sail_int)(&zghz399);
  KILL(sail_int)(&zghz398);
  KILL(sail_int)(&zghz397);
  KILL(sail_int)(&zghz396);
  KILL(sail_int)(&zghz395);
  KILL(sail_int)(&zghz394);
  KILL(sail_int)(&zghz393);
  KILL(sail_int)(&zghz392);
  KILL(lbits)(&zghz391);
  KILL(lbits)(&zghz390);
  KILL(sail_int)(&zghz389);
  KILL(sail_int)(&zghz388);
  KILL(lbits)(&zghz387);
}

static bool sailgen_CapIsExponentOutOfRange(lbits);

static bool sailgen_CapIsExponentOutOfRange(lbits zc)
{
  __label__ case_105, finish_match_104, end_function_106, end_block_exception_109;

  bool zcbz320;
  int64_t zgaz376;
  zgaz376 = sailgen_CapGetExponent(zc);
  {
    __label__ cleanup_107, end_cleanup_108;

    int64_t zexp;
    zexp = zgaz376;
    {
      bool zgaz377;
      zgaz377 = (zexp > zCAP_MAX_EXPONENT);
      bool zgsz332;
      if (zgaz377) {  zgsz332 = (zexp < zCAP_MAX_ENCODEABLE_EXPONENT);  } else {  zgsz332 = false;  }
      zcbz320 = zgsz332;
    }
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

sail_int zghz3123;
sail_int zghz3124;

static void startup_sailgen_CapUnsignedGreaterThan(void)
{
  CREATE(sail_int)(&zghz3123);
  CREATE(sail_int)(&zghz3124);
}

static bool sailgen_CapUnsignedGreaterThan(lbits za, lbits zb)
{
  __label__ cleanup_112, end_cleanup_113, end_function_111, end_block_exception_114;

  bool zcbz321;
  {
    RECREATE(sail_int)(&zghz3123);
    sail_unsigned(&zghz3123, za);
    RECREATE(sail_int)(&zghz3124);
    sail_unsigned(&zghz3124, zb);
    zcbz321 = gt(zghz3123, zghz3124);
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
  KILL(sail_int)(&zghz3124);
  KILL(sail_int)(&zghz3123);
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

lbits zghz3125;
lbits zghz3126;
lbits zghz3127;
lbits zghz3128;
lbits zghz3129;
sail_int zghz3130;
sail_int zghz3131;
lbits zghz3132;
lbits zghz3133;
sail_int zghz3134;
sail_int zghz3135;
lbits zghz3136;
sail_int zghz3137;
sail_int zghz3138;
lbits zghz3139;
lbits zghz3140;
sail_int zghz3141;
sail_int zghz3142;
sail_int zghz3143;
sail_int zghz3144;
sail_int zghz3145;
sail_int zghz3146;
lbits zghz3147;
sail_int zghz3148;
sail_int zghz3149;
lbits zghz3150;
lbits zghz3151;
sail_int zghz3152;
lbits zghz3153;
sail_int zghz3154;
sail_int zghz3155;
lbits zghz3156;
lbits zghz3157;
sail_int zghz3158;
lbits zghz3159;
sail_int zghz3160;
sail_int zghz3161;
lbits zghz3162;
sail_int zghz3163;
sail_int zghz3164;
sail_int zghz3165;
sail_int zghz3166;
sail_int zghz3167;
sail_int zghz3168;
lbits zghz3169;
lbits zghz3170;
lbits zghz3171;
lbits zghz3172;
lbits zghz3173;
lbits zghz3174;
sail_int zghz3175;
sail_int zghz3176;
sail_int zghz3177;
sail_int zghz3178;
sail_int zghz3179;
sail_int zghz3180;
sail_int zghz3181;
sail_int zghz3182;
sail_int zghz3183;
sail_int zghz3184;
sail_int zghz3185;
sail_int zghz3186;
sail_int zghz3187;
sail_int zghz3188;
lbits zghz3189;
sail_int zghz3190;
sail_int zghz3191;
sail_int zghz3192;
sail_int zghz3193;
sail_int zghz3194;
sail_int zghz3195;
lbits zghz3196;
lbits zghz3197;
sail_int zghz3198;
sail_int zghz3199;
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
sail_int zghz3214;
sail_int zghz3215;
sail_int zghz3216;
sail_int zghz3217;
lbits zghz3218;
sail_int zghz3219;
lbits zghz3220;
sail_int zghz3221;
sail_int zghz3222;
sail_int zghz3223;
lbits zghz3224;
sail_int zghz3225;
lbits zghz3226;
sail_int zghz3227;
sail_int zghz3228;

static void startup_sailgen_CapGetBounds(void)
{
  CREATE(lbits)(&zghz3125);
  CREATE(lbits)(&zghz3126);
  CREATE(lbits)(&zghz3127);
  CREATE(lbits)(&zghz3128);
  CREATE(lbits)(&zghz3129);
  CREATE(sail_int)(&zghz3130);
  CREATE(sail_int)(&zghz3131);
  CREATE(lbits)(&zghz3132);
  CREATE(lbits)(&zghz3133);
  CREATE(sail_int)(&zghz3134);
  CREATE(sail_int)(&zghz3135);
  CREATE(lbits)(&zghz3136);
  CREATE(sail_int)(&zghz3137);
  CREATE(sail_int)(&zghz3138);
  CREATE(lbits)(&zghz3139);
  CREATE(lbits)(&zghz3140);
  CREATE(sail_int)(&zghz3141);
  CREATE(sail_int)(&zghz3142);
  CREATE(sail_int)(&zghz3143);
  CREATE(sail_int)(&zghz3144);
  CREATE(sail_int)(&zghz3145);
  CREATE(sail_int)(&zghz3146);
  CREATE(lbits)(&zghz3147);
  CREATE(sail_int)(&zghz3148);
  CREATE(sail_int)(&zghz3149);
  CREATE(lbits)(&zghz3150);
  CREATE(lbits)(&zghz3151);
  CREATE(sail_int)(&zghz3152);
  CREATE(lbits)(&zghz3153);
  CREATE(sail_int)(&zghz3154);
  CREATE(sail_int)(&zghz3155);
  CREATE(lbits)(&zghz3156);
  CREATE(lbits)(&zghz3157);
  CREATE(sail_int)(&zghz3158);
  CREATE(lbits)(&zghz3159);
  CREATE(sail_int)(&zghz3160);
  CREATE(sail_int)(&zghz3161);
  CREATE(lbits)(&zghz3162);
  CREATE(sail_int)(&zghz3163);
  CREATE(sail_int)(&zghz3164);
  CREATE(sail_int)(&zghz3165);
  CREATE(sail_int)(&zghz3166);
  CREATE(sail_int)(&zghz3167);
  CREATE(sail_int)(&zghz3168);
  CREATE(lbits)(&zghz3169);
  CREATE(lbits)(&zghz3170);
  CREATE(lbits)(&zghz3171);
  CREATE(lbits)(&zghz3172);
  CREATE(lbits)(&zghz3173);
  CREATE(lbits)(&zghz3174);
  CREATE(sail_int)(&zghz3175);
  CREATE(sail_int)(&zghz3176);
  CREATE(sail_int)(&zghz3177);
  CREATE(sail_int)(&zghz3178);
  CREATE(sail_int)(&zghz3179);
  CREATE(sail_int)(&zghz3180);
  CREATE(sail_int)(&zghz3181);
  CREATE(sail_int)(&zghz3182);
  CREATE(sail_int)(&zghz3183);
  CREATE(sail_int)(&zghz3184);
  CREATE(sail_int)(&zghz3185);
  CREATE(sail_int)(&zghz3186);
  CREATE(sail_int)(&zghz3187);
  CREATE(sail_int)(&zghz3188);
  CREATE(lbits)(&zghz3189);
  CREATE(sail_int)(&zghz3190);
  CREATE(sail_int)(&zghz3191);
  CREATE(sail_int)(&zghz3192);
  CREATE(sail_int)(&zghz3193);
  CREATE(sail_int)(&zghz3194);
  CREATE(sail_int)(&zghz3195);
  CREATE(lbits)(&zghz3196);
  CREATE(lbits)(&zghz3197);
  CREATE(sail_int)(&zghz3198);
  CREATE(sail_int)(&zghz3199);
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
  CREATE(sail_int)(&zghz3214);
  CREATE(sail_int)(&zghz3215);
  CREATE(sail_int)(&zghz3216);
  CREATE(sail_int)(&zghz3217);
  CREATE(lbits)(&zghz3218);
  CREATE(sail_int)(&zghz3219);
  CREATE(lbits)(&zghz3220);
  CREATE(sail_int)(&zghz3221);
  CREATE(sail_int)(&zghz3222);
  CREATE(sail_int)(&zghz3223);
  CREATE(lbits)(&zghz3224);
  CREATE(sail_int)(&zghz3225);
  CREATE(lbits)(&zghz3226);
  CREATE(sail_int)(&zghz3227);
  CREATE(sail_int)(&zghz3228);
}

static void sailgen_CapGetBounds(struct ztuple_z8z5bvzCz0z5bvzCz0z5boolz9 *zcbz322, lbits zc)
{
  __label__ case_117, finish_match_116, end_function_128, end_block_exception_135, end_function_267;

  int64_t zgaz382;
  zgaz382 = sailgen_CapGetExponent(zc);
  struct ztuple_z8z5bvzCz0z5bvzCz0z5boolz9 zgsz334;
  CREATE(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgsz334);
  {
    __label__ case_119, finish_match_118;

    int64_t zexp;
    zexp = zgaz382;
    {
      bool zgaz383;
      zgaz383 = (zexp == zCAP_MAX_ENCODEABLE_EXPONENT);
      unit zgsz337;
      if (zgaz383) {
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
      bool zgaz384;
      zgaz384 = sailgen_CapIsExponentOutOfRange(zc);
      unit zgsz335;
      if (zgaz384) {
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
    RECREATE(lbits)(&zghz3125);
    {
      RECREATE(sail_int)(&zghz3228);
      CONVERT_OF(sail_int, mach_int)(&zghz3228, INT64_C(66));
      UNDEFINED(lbits)(&zghz3125, zghz3228);
    }
    RECREATE(lbits)(&zghz3126);
    {
      RECREATE(sail_int)(&zghz3227);
      CONVERT_OF(sail_int, mach_int)(&zghz3227, INT64_C(66));
      UNDEFINED(lbits)(&zghz3126, zghz3227);
    }
    uint64_t zbottom;
    zbottom = sailgen_CapGetBottom(zc);
    uint64_t ztop;
    ztop = sailgen_CapGetTop(zc);
    {
      sbits zgaz385;
      {
        RECREATE(sail_int)(&zghz3225);
        CONVERT_OF(sail_int, mach_int)(&zghz3225, zexp);
        RECREATE(lbits)(&zghz3226);
        sailgen_Zeros(&zghz3226, zghz3225);
        zgaz385 = CONVERT_OF(sbits, lbits)(zghz3226, true);
      }
      {
        RECREATE(sail_int)(&zghz3221);
        CONVERT_OF(sail_int, mach_int)(&zghz3221, INT64_C(66));
        RECREATE(sail_int)(&zghz3222);
        CONVERT_OF(sail_int, mach_int)(&zghz3222, zexp);
        RECREATE(sail_int)(&zghz3223);
        CONVERT_OF(sail_int, mach_int)(&zghz3223, INT64_C(0));
        RECREATE(lbits)(&zghz3224);
        CONVERT_OF(lbits, sbits)(&zghz3224, zgaz385, true);
        set_slice(&zghz3125, zghz3221, zghz3222, zghz3125, zghz3223, zghz3224);
      }
      unit zgsz343;
      zgsz343 = UNIT;
    }
    {
      sbits zgaz386;
      {
        RECREATE(sail_int)(&zghz3219);
        CONVERT_OF(sail_int, mach_int)(&zghz3219, zexp);
        RECREATE(lbits)(&zghz3220);
        sailgen_Zeros(&zghz3220, zghz3219);
        zgaz386 = CONVERT_OF(sbits, lbits)(zghz3220, true);
      }
      {
        RECREATE(sail_int)(&zghz3215);
        CONVERT_OF(sail_int, mach_int)(&zghz3215, INT64_C(66));
        RECREATE(sail_int)(&zghz3216);
        CONVERT_OF(sail_int, mach_int)(&zghz3216, zexp);
        RECREATE(sail_int)(&zghz3217);
        CONVERT_OF(sail_int, mach_int)(&zghz3217, INT64_C(0));
        RECREATE(lbits)(&zghz3218);
        CONVERT_OF(lbits, sbits)(&zghz3218, zgaz386, true);
        set_slice(&zghz3126, zghz3215, zghz3216, zghz3126, zghz3217, zghz3218);
      }
      unit zgsz342;
      zgsz342 = UNIT;
    }
    {
      bool zgaz390;
      {
        int64_t zgaz389;
        {
          int64_t zgaz388;
          {
            int64_t zgaz387;
            {
              RECREATE(sail_int)(&zghz3213);
              CONVERT_OF(sail_int, mach_int)(&zghz3213, zexp);
              RECREATE(sail_int)(&zghz3214);
              sailgen___id(&zghz3214, zghz3213);
              zgaz387 = CONVERT_OF(mach_int, sail_int)(zghz3214);
            }
            zgaz388 = (zgaz387 + INT64_C(16));
          }
          {
            RECREATE(sail_int)(&zghz3210);
            CONVERT_OF(sail_int, mach_int)(&zghz3210, zgaz388);
            RECREATE(sail_int)(&zghz3211);
            CONVERT_OF(sail_int, mach_int)(&zghz3211, INT64_C(1));
            RECREATE(sail_int)(&zghz3212);
            sub_int(&zghz3212, zghz3210, zghz3211);
            zgaz389 = CONVERT_OF(mach_int, sail_int)(zghz3212);
          }
        }
        zgaz390 = (zgaz389 < INT64_C(66));
      }
      unit zgsz341;
      zgsz341 = sail_assert(zgaz390, "v8_base.sail:1391.41-1391.42");
    }
    {
      int64_t zgaz392;
      {
        int64_t zgaz391;
        zgaz391 = (zexp + zCAP_MW);
        {
          RECREATE(sail_int)(&zghz3207);
          CONVERT_OF(sail_int, mach_int)(&zghz3207, zgaz391);
          RECREATE(sail_int)(&zghz3208);
          CONVERT_OF(sail_int, mach_int)(&zghz3208, INT64_C(1));
          RECREATE(sail_int)(&zghz3209);
          sub_int(&zghz3209, zghz3207, zghz3208);
          zgaz392 = CONVERT_OF(mach_int, sail_int)(zghz3209);
        }
      }
      {
        RECREATE(sail_int)(&zghz3204);
        CONVERT_OF(sail_int, mach_int)(&zghz3204, zgaz392);
        RECREATE(sail_int)(&zghz3205);
        CONVERT_OF(sail_int, mach_int)(&zghz3205, zexp);
        RECREATE(lbits)(&zghz3206);
        CONVERT_OF(lbits, fbits)(&zghz3206, zbottom, UINT64_C(16) , true);
        vector_update_subrange_lbits(&zghz3125, zghz3125, zghz3204, zghz3205, zghz3206);
      }
      unit zgsz340;
      zgsz340 = UNIT;
    }
    {
      int64_t zgaz394;
      {
        int64_t zgaz393;
        zgaz393 = (zexp + zCAP_MW);
        {
          RECREATE(sail_int)(&zghz3201);
          CONVERT_OF(sail_int, mach_int)(&zghz3201, zgaz393);
          RECREATE(sail_int)(&zghz3202);
          CONVERT_OF(sail_int, mach_int)(&zghz3202, INT64_C(1));
          RECREATE(sail_int)(&zghz3203);
          sub_int(&zghz3203, zghz3201, zghz3202);
          zgaz394 = CONVERT_OF(mach_int, sail_int)(zghz3203);
        }
      }
      {
        RECREATE(sail_int)(&zghz3198);
        CONVERT_OF(sail_int, mach_int)(&zghz3198, zgaz394);
        RECREATE(sail_int)(&zghz3199);
        CONVERT_OF(sail_int, mach_int)(&zghz3199, zexp);
        RECREATE(lbits)(&zghz3200);
        CONVERT_OF(lbits, fbits)(&zghz3200, ztop, UINT64_C(16) , true);
        vector_update_subrange_lbits(&zghz3126, zghz3126, zghz3198, zghz3199, zghz3200);
      }
      unit zgsz339;
      zgsz339 = UNIT;
    }
    RECREATE(lbits)(&zghz3127);
    {
      uint64_t zgaz3131;
      {
        uint64_t zgaz3130;
        zgaz3130 = sailgen_CapGetValue(zc);
        zgaz3131 = sailgen_CapBoundsAddress(zgaz3130);
      }
      {
        RECREATE(lbits)(&zghz3196);
        CONVERT_OF(lbits, fbits)(&zghz3196, UINT64_C(0b00), UINT64_C(2) , true);
        RECREATE(lbits)(&zghz3197);
        CONVERT_OF(lbits, fbits)(&zghz3197, zgaz3131, UINT64_C(64) , true);
        append(&zghz3127, zghz3196, zghz3197);
      }
    }
    uint64_t zA3;
    {
      int64_t zgaz3128;
      {
        int64_t zgaz3126;
        zgaz3126 = (zexp + zCAP_MW);
        {
          RECREATE(sail_int)(&zghz3193);
          CONVERT_OF(sail_int, mach_int)(&zghz3193, zgaz3126);
          RECREATE(sail_int)(&zghz3194);
          CONVERT_OF(sail_int, mach_int)(&zghz3194, INT64_C(1));
          RECREATE(sail_int)(&zghz3195);
          sub_int(&zghz3195, zghz3193, zghz3194);
          zgaz3128 = CONVERT_OF(mach_int, sail_int)(zghz3195);
        }
      }
      int64_t zgaz3129;
      {
        int64_t zgaz3127;
        zgaz3127 = (zexp + zCAP_MW);
        {
          RECREATE(sail_int)(&zghz3190);
          CONVERT_OF(sail_int, mach_int)(&zghz3190, zgaz3127);
          RECREATE(sail_int)(&zghz3191);
          CONVERT_OF(sail_int, mach_int)(&zghz3191, INT64_C(3));
          RECREATE(sail_int)(&zghz3192);
          sub_int(&zghz3192, zghz3190, zghz3191);
          zgaz3129 = CONVERT_OF(mach_int, sail_int)(zghz3192);
        }
      }
      {
        RECREATE(sail_int)(&zghz3187);
        CONVERT_OF(sail_int, mach_int)(&zghz3187, zgaz3128);
        RECREATE(sail_int)(&zghz3188);
        CONVERT_OF(sail_int, mach_int)(&zghz3188, zgaz3129);
        RECREATE(lbits)(&zghz3189);
        vector_subrange_lbits(&zghz3189, zghz3127, zghz3187, zghz3188);
        zA3 = CONVERT_OF(fbits, lbits)(zghz3189, true);
      }
    }
    uint64_t zB3;
    {
      int64_t zgaz3124;
      {
        RECREATE(sail_int)(&zghz3184);
        CONVERT_OF(sail_int, mach_int)(&zghz3184, zCAP_MW);
        RECREATE(sail_int)(&zghz3185);
        CONVERT_OF(sail_int, mach_int)(&zghz3185, INT64_C(1));
        RECREATE(sail_int)(&zghz3186);
        sub_int(&zghz3186, zghz3184, zghz3185);
        zgaz3124 = CONVERT_OF(mach_int, sail_int)(zghz3186);
      }
      int64_t zgaz3125;
      {
        RECREATE(sail_int)(&zghz3181);
        CONVERT_OF(sail_int, mach_int)(&zghz3181, zCAP_MW);
        RECREATE(sail_int)(&zghz3182);
        CONVERT_OF(sail_int, mach_int)(&zghz3182, INT64_C(3));
        RECREATE(sail_int)(&zghz3183);
        sub_int(&zghz3183, zghz3181, zghz3182);
        zgaz3125 = CONVERT_OF(mach_int, sail_int)(zghz3183);
      }
      zB3 = (safe_rshift(UINT64_MAX, 64 - 3) & (zbottom >> zgaz3125));
    }
    uint64_t zT3;
    {
      int64_t zgaz3122;
      {
        RECREATE(sail_int)(&zghz3178);
        CONVERT_OF(sail_int, mach_int)(&zghz3178, zCAP_MW);
        RECREATE(sail_int)(&zghz3179);
        CONVERT_OF(sail_int, mach_int)(&zghz3179, INT64_C(1));
        RECREATE(sail_int)(&zghz3180);
        sub_int(&zghz3180, zghz3178, zghz3179);
        zgaz3122 = CONVERT_OF(mach_int, sail_int)(zghz3180);
      }
      int64_t zgaz3123;
      {
        RECREATE(sail_int)(&zghz3175);
        CONVERT_OF(sail_int, mach_int)(&zghz3175, zCAP_MW);
        RECREATE(sail_int)(&zghz3176);
        CONVERT_OF(sail_int, mach_int)(&zghz3176, INT64_C(3));
        RECREATE(sail_int)(&zghz3177);
        sub_int(&zghz3177, zghz3175, zghz3176);
        zgaz3123 = CONVERT_OF(mach_int, sail_int)(zghz3177);
      }
      zT3 = (safe_rshift(UINT64_MAX, 64 - 3) & (ztop >> zgaz3123));
    }
    uint64_t zR3;
    zR3 = ((zB3 - UINT64_C(0b001)) & UINT64_C(0b111));
    int64_t zaHi;
    zaHi = INT64_C(0);
    {
      bool zgaz395;
      {
        RECREATE(lbits)(&zghz3173);
        CONVERT_OF(lbits, fbits)(&zghz3173, zA3, UINT64_C(3) , true);
        RECREATE(lbits)(&zghz3174);
        CONVERT_OF(lbits, fbits)(&zghz3174, zR3, UINT64_C(3) , true);
        zgaz395 = sailgen_CapUnsignedLessThan(zghz3173, zghz3174);
      }
      unit zgsz344;
      if (zgaz395) {
        zaHi = INT64_C(1);
        zgsz344 = UNIT;
      } else {
        zaHi = INT64_C(0);
        zgsz344 = UNIT;
      }
    }
    {
      __label__ case_121, finish_match_120;

      int64_t zaHishadowz30;
      zaHishadowz30 = zaHi;
      int64_t zbHi;
      zbHi = INT64_C(0);
      {
        bool zgaz396;
        {
          RECREATE(lbits)(&zghz3171);
          CONVERT_OF(lbits, fbits)(&zghz3171, zB3, UINT64_C(3) , true);
          RECREATE(lbits)(&zghz3172);
          CONVERT_OF(lbits, fbits)(&zghz3172, zR3, UINT64_C(3) , true);
          zgaz396 = sailgen_CapUnsignedLessThan(zghz3171, zghz3172);
        }
        unit zgsz346;
        if (zgaz396) {
          zbHi = INT64_C(1);
          zgsz346 = UNIT;
        } else {
          zbHi = INT64_C(0);
          zgsz346 = UNIT;
        }
      }
      {
        __label__ case_123, finish_match_122;

        int64_t zbHishadowz31;
        zbHishadowz31 = zbHi;
        int64_t ztHi;
        ztHi = INT64_C(0);
        {
          bool zgaz397;
          {
            RECREATE(lbits)(&zghz3169);
            CONVERT_OF(lbits, fbits)(&zghz3169, zT3, UINT64_C(3) , true);
            RECREATE(lbits)(&zghz3170);
            CONVERT_OF(lbits, fbits)(&zghz3170, zR3, UINT64_C(3) , true);
            zgaz397 = sailgen_CapUnsignedLessThan(zghz3169, zghz3170);
          }
          unit zgsz348;
          if (zgaz397) {
            ztHi = INT64_C(1);
            zgsz348 = UNIT;
          } else {
            ztHi = INT64_C(0);
            zgsz348 = UNIT;
          }
        }
        {
          __label__ case_125, finish_match_124;

          int64_t ztHishadowz32;
          ztHishadowz32 = ztHi;
          int64_t zgaz398;
          {
            RECREATE(sail_int)(&zghz3166);
            CONVERT_OF(sail_int, mach_int)(&zghz3166, zbHishadowz31);
            RECREATE(sail_int)(&zghz3167);
            CONVERT_OF(sail_int, mach_int)(&zghz3167, zaHishadowz30);
            RECREATE(sail_int)(&zghz3168);
            sub_int(&zghz3168, zghz3166, zghz3167);
            zgaz398 = CONVERT_OF(mach_int, sail_int)(zghz3168);
          }
          {
            __label__ case_127, finish_match_126;

            int64_t zcorrection_base;
            zcorrection_base = zgaz398;
            int64_t zgaz399;
            {
              RECREATE(sail_int)(&zghz3163);
              CONVERT_OF(sail_int, mach_int)(&zghz3163, ztHishadowz32);
              RECREATE(sail_int)(&zghz3164);
              CONVERT_OF(sail_int, mach_int)(&zghz3164, zaHishadowz30);
              RECREATE(sail_int)(&zghz3165);
              sub_int(&zghz3165, zghz3163, zghz3164);
              zgaz399 = CONVERT_OF(mach_int, sail_int)(zghz3165);
            }
            {
              __label__ cleanup_129, end_cleanup_130;

              int64_t zcorrection_limit;
              zcorrection_limit = zgaz399;
              {
                bool zgaz3102;
                {
                  int64_t zgaz3100;
                  zgaz3100 = (zexp + zCAP_MW);
                  int64_t zgaz3101;
                  zgaz3101 = (zCAP_MAX_EXPONENT + zCAP_MW);
                  zgaz3102 = (zgaz3100 < zgaz3101);
                }
                unit zgsz352;
                if (zgaz3102) {
                  sbits zatop;
                  {
                    int64_t zgaz3107;
                    zgaz3107 = (zexp + zCAP_MW);
                    {
                      RECREATE(sail_int)(&zghz3160);
                      CONVERT_OF(sail_int, mach_int)(&zghz3160, INT64_C(65));
                      RECREATE(sail_int)(&zghz3161);
                      CONVERT_OF(sail_int, mach_int)(&zghz3161, zgaz3107);
                      RECREATE(lbits)(&zghz3162);
                      vector_subrange_lbits(&zghz3162, zghz3127, zghz3160, zghz3161);
                      zatop = CONVERT_OF(sbits, lbits)(zghz3162, true);
                    }
                  }
                  {
                    int64_t zgaz3103;
                    zgaz3103 = (zexp + zCAP_MW);
                    sbits zgaz3104;
                    {
                      RECREATE(lbits)(&zghz3157);
                      CONVERT_OF(lbits, sbits)(&zghz3157, zatop, true);
                      RECREATE(sail_int)(&zghz3158);
                      CONVERT_OF(sail_int, mach_int)(&zghz3158, zcorrection_base);
                      RECREATE(lbits)(&zghz3159);
                      add_bits_int(&zghz3159, zghz3157, zghz3158);
                      zgaz3104 = CONVERT_OF(sbits, lbits)(zghz3159, true);
                    }
                    {
                      RECREATE(sail_int)(&zghz3154);
                      CONVERT_OF(sail_int, mach_int)(&zghz3154, INT64_C(65));
                      RECREATE(sail_int)(&zghz3155);
                      CONVERT_OF(sail_int, mach_int)(&zghz3155, zgaz3103);
                      RECREATE(lbits)(&zghz3156);
                      CONVERT_OF(lbits, sbits)(&zghz3156, zgaz3104, true);
                      vector_update_subrange_lbits(&zghz3125, zghz3125, zghz3154, zghz3155, zghz3156);
                    }
                    unit zgsz353;
                    zgsz353 = UNIT;
                  }
                  int64_t zgaz3105;
                  zgaz3105 = (zexp + zCAP_MW);
                  sbits zgaz3106;
                  {
                    RECREATE(lbits)(&zghz3151);
                    CONVERT_OF(lbits, sbits)(&zghz3151, zatop, true);
                    RECREATE(sail_int)(&zghz3152);
                    CONVERT_OF(sail_int, mach_int)(&zghz3152, zcorrection_limit);
                    RECREATE(lbits)(&zghz3153);
                    add_bits_int(&zghz3153, zghz3151, zghz3152);
                    zgaz3106 = CONVERT_OF(sbits, lbits)(zghz3153, true);
                  }
                  {
                    RECREATE(sail_int)(&zghz3148);
                    CONVERT_OF(sail_int, mach_int)(&zghz3148, INT64_C(65));
                    RECREATE(sail_int)(&zghz3149);
                    CONVERT_OF(sail_int, mach_int)(&zghz3149, zgaz3105);
                    RECREATE(lbits)(&zghz3150);
                    CONVERT_OF(lbits, sbits)(&zghz3150, zgaz3106, true);
                    vector_update_subrange_lbits(&zghz3126, zghz3126, zghz3148, zghz3149, zghz3150);
                  }
                  zgsz352 = UNIT;
                } else {  zgsz352 = UNIT;  }
              }
              uint64_t zl2;
              {
                RECREATE(sail_int)(&zghz3145);
                CONVERT_OF(sail_int, mach_int)(&zghz3145, INT64_C(64));
                RECREATE(sail_int)(&zghz3146);
                CONVERT_OF(sail_int, mach_int)(&zghz3146, INT64_C(63));
                RECREATE(lbits)(&zghz3147);
                vector_subrange_lbits(&zghz3147, zghz3126, zghz3145, zghz3146);
                zl2 = CONVERT_OF(fbits, lbits)(zghz3147, true);
              }
              uint64_t zb2;
              {
                uint64_t zgaz3121;
                {
                  fbits zgaz3120;
                  {
                    RECREATE(sail_int)(&zghz3144);
                    CONVERT_OF(sail_int, mach_int)(&zghz3144, INT64_C(63));
                    zgaz3120 = bitvector_access(zghz3125, zghz3144);
                  }
                  zgaz3121 = UINT64_C(0b0);
                  zgaz3121 = update_fbits(zgaz3121, INT64_C(0), zgaz3120);
                }
                zb2 = (UINT64_C(0b0) << 1) | zgaz3121;
              }
              {
                bool zgaz3111;
                {
                  bool zgaz3110;
                  {
                    int64_t zgaz3108;
                    {
                      RECREATE(sail_int)(&zghz3141);
                      CONVERT_OF(sail_int, mach_int)(&zghz3141, zCAP_MAX_EXPONENT);
                      RECREATE(sail_int)(&zghz3142);
                      CONVERT_OF(sail_int, mach_int)(&zghz3142, INT64_C(1));
                      RECREATE(sail_int)(&zghz3143);
                      sub_int(&zghz3143, zghz3141, zghz3142);
                      zgaz3108 = CONVERT_OF(mach_int, sail_int)(zghz3143);
                    }
                    zgaz3110 = (zexp < zgaz3108);
                  }
                  bool zgsz355;
                  if (zgaz3110) {
                    uint64_t zgaz3109;
                    zgaz3109 = ((zl2 - zb2) & UINT64_C(0b11));
                    {
                      RECREATE(lbits)(&zghz3139);
                      CONVERT_OF(lbits, fbits)(&zghz3139, zgaz3109, UINT64_C(2) , true);
                      RECREATE(lbits)(&zghz3140);
                      CONVERT_OF(lbits, fbits)(&zghz3140, UINT64_C(0b01), UINT64_C(2) , true);
                      zgsz355 = sailgen_CapUnsignedGreaterThan(zghz3139, zghz3140);
                    }
                  } else {  zgsz355 = false;  }
                  zgaz3111 = zgsz355;
                }
                unit zgsz356;
                if (zgaz3111) {
                  fbits zgaz3115;
                  {
                    uint64_t zgaz3114;
                    {
                      uint64_t zgaz3113;
                      {
                        fbits zgaz3112;
                        {
                          RECREATE(sail_int)(&zghz3138);
                          CONVERT_OF(sail_int, mach_int)(&zghz3138, INT64_C(64));
                          zgaz3112 = bitvector_access(zghz3126, zghz3138);
                        }
                        zgaz3113 = UINT64_C(0b0);
                        zgaz3113 = update_fbits(zgaz3113, INT64_C(0), zgaz3112);
                      }
                      zgaz3114 = (~(zgaz3113) & UINT64_C(0b1));
                    }
                    zgaz3115 = sailgen_Bit(zgaz3114);
                  }
                  {
                    RECREATE(sail_int)(&zghz3137);
                    CONVERT_OF(sail_int, mach_int)(&zghz3137, INT64_C(64));
                    update_lbits(&zghz3126, zghz3126, zghz3137, zgaz3115);
                  }
                  zgsz356 = UNIT;
                } else {  zgsz356 = UNIT;  }
              }
              struct ztuple_z8z5bvzCz0z5bvzCz0z5boolz9 zgaz3119;
              CREATE(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgaz3119);
              {
                RECREATE(lbits)(&zghz3128);
                {
                  uint64_t zgaz3116;
                  {
                    RECREATE(sail_int)(&zghz3134);
                    CONVERT_OF(sail_int, mach_int)(&zghz3134, INT64_C(63));
                    RECREATE(sail_int)(&zghz3135);
                    CONVERT_OF(sail_int, mach_int)(&zghz3135, INT64_C(0));
                    RECREATE(lbits)(&zghz3136);
                    vector_subrange_lbits(&zghz3136, zghz3125, zghz3134, zghz3135);
                    zgaz3116 = CONVERT_OF(fbits, lbits)(zghz3136, true);
                  }
                  {
                    RECREATE(lbits)(&zghz3132);
                    CONVERT_OF(lbits, fbits)(&zghz3132, UINT64_C(0b0), UINT64_C(1) , true);
                    RECREATE(lbits)(&zghz3133);
                    CONVERT_OF(lbits, fbits)(&zghz3133, zgaz3116, UINT64_C(64) , true);
                    append(&zghz3128, zghz3132, zghz3133);
                  }
                }
                RECREATE(lbits)(&zghz3129);
                {
                  RECREATE(sail_int)(&zghz3130);
                  CONVERT_OF(sail_int, mach_int)(&zghz3130, INT64_C(64));
                  RECREATE(sail_int)(&zghz3131);
                  CONVERT_OF(sail_int, mach_int)(&zghz3131, INT64_C(0));
                  vector_subrange_lbits(&zghz3129, zghz3126, zghz3130, zghz3131);
                }
                COPY(lbits)(&((&zgaz3119)->ztup0), zghz3128);
                COPY(lbits)(&((&zgaz3119)->ztup1), zghz3129);
                zgaz3119.ztup2 = true;
              }
              COPY(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)((*(&zcbz322)), zgaz3119);
              goto cleanup_129;
              /* unreachable after return */
              KILL(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgaz3119);
              goto finish_match_126;
              goto end_cleanup_130;
            cleanup_129: ;
              KILL(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgsz334);
              KILL(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgaz3119);
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
  goto end_function_267;
end_block_exception_135: ;
  goto end_function_267;
end_function_267: ;
}



static void finish_sailgen_CapGetBounds(void)
{
  KILL(sail_int)(&zghz3228);
  KILL(sail_int)(&zghz3227);
  KILL(lbits)(&zghz3226);
  KILL(sail_int)(&zghz3225);
  KILL(lbits)(&zghz3224);
  KILL(sail_int)(&zghz3223);
  KILL(sail_int)(&zghz3222);
  KILL(sail_int)(&zghz3221);
  KILL(lbits)(&zghz3220);
  KILL(sail_int)(&zghz3219);
  KILL(lbits)(&zghz3218);
  KILL(sail_int)(&zghz3217);
  KILL(sail_int)(&zghz3216);
  KILL(sail_int)(&zghz3215);
  KILL(sail_int)(&zghz3214);
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
  KILL(sail_int)(&zghz3199);
  KILL(sail_int)(&zghz3198);
  KILL(lbits)(&zghz3197);
  KILL(lbits)(&zghz3196);
  KILL(sail_int)(&zghz3195);
  KILL(sail_int)(&zghz3194);
  KILL(sail_int)(&zghz3193);
  KILL(sail_int)(&zghz3192);
  KILL(sail_int)(&zghz3191);
  KILL(sail_int)(&zghz3190);
  KILL(lbits)(&zghz3189);
  KILL(sail_int)(&zghz3188);
  KILL(sail_int)(&zghz3187);
  KILL(sail_int)(&zghz3186);
  KILL(sail_int)(&zghz3185);
  KILL(sail_int)(&zghz3184);
  KILL(sail_int)(&zghz3183);
  KILL(sail_int)(&zghz3182);
  KILL(sail_int)(&zghz3181);
  KILL(sail_int)(&zghz3180);
  KILL(sail_int)(&zghz3179);
  KILL(sail_int)(&zghz3178);
  KILL(sail_int)(&zghz3177);
  KILL(sail_int)(&zghz3176);
  KILL(sail_int)(&zghz3175);
  KILL(lbits)(&zghz3174);
  KILL(lbits)(&zghz3173);
  KILL(lbits)(&zghz3172);
  KILL(lbits)(&zghz3171);
  KILL(lbits)(&zghz3170);
  KILL(lbits)(&zghz3169);
  KILL(sail_int)(&zghz3168);
  KILL(sail_int)(&zghz3167);
  KILL(sail_int)(&zghz3166);
  KILL(sail_int)(&zghz3165);
  KILL(sail_int)(&zghz3164);
  KILL(sail_int)(&zghz3163);
  KILL(lbits)(&zghz3162);
  KILL(sail_int)(&zghz3161);
  KILL(sail_int)(&zghz3160);
  KILL(lbits)(&zghz3159);
  KILL(sail_int)(&zghz3158);
  KILL(lbits)(&zghz3157);
  KILL(lbits)(&zghz3156);
  KILL(sail_int)(&zghz3155);
  KILL(sail_int)(&zghz3154);
  KILL(lbits)(&zghz3153);
  KILL(sail_int)(&zghz3152);
  KILL(lbits)(&zghz3151);
  KILL(lbits)(&zghz3150);
  KILL(sail_int)(&zghz3149);
  KILL(sail_int)(&zghz3148);
  KILL(lbits)(&zghz3147);
  KILL(sail_int)(&zghz3146);
  KILL(sail_int)(&zghz3145);
  KILL(sail_int)(&zghz3144);
  KILL(sail_int)(&zghz3143);
  KILL(sail_int)(&zghz3142);
  KILL(sail_int)(&zghz3141);
  KILL(lbits)(&zghz3140);
  KILL(lbits)(&zghz3139);
  KILL(sail_int)(&zghz3138);
  KILL(sail_int)(&zghz3137);
  KILL(lbits)(&zghz3136);
  KILL(sail_int)(&zghz3135);
  KILL(sail_int)(&zghz3134);
  KILL(lbits)(&zghz3133);
  KILL(lbits)(&zghz3132);
  KILL(sail_int)(&zghz3131);
  KILL(sail_int)(&zghz3130);
  KILL(lbits)(&zghz3129);
  KILL(lbits)(&zghz3128);
  KILL(lbits)(&zghz3127);
  KILL(lbits)(&zghz3126);
  KILL(lbits)(&zghz3125);
}

static bool sailgen_CapBoundsEqual(lbits, lbits);

lbits zghz3229;
lbits zghz3230;
lbits zghz3231;
lbits zghz3232;
lbits zghz3233;
lbits zghz3234;
lbits zghz3235;
lbits zghz3236;
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
sail_int zghz3249;
sail_int zghz3250;
sail_int zghz3251;
sail_int zghz3252;

static void startup_sailgen_CapBoundsEqual(void)
{
  CREATE(lbits)(&zghz3229);
  CREATE(lbits)(&zghz3230);
  CREATE(lbits)(&zghz3231);
  CREATE(lbits)(&zghz3232);
  CREATE(lbits)(&zghz3233);
  CREATE(lbits)(&zghz3234);
  CREATE(lbits)(&zghz3235);
  CREATE(lbits)(&zghz3236);
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
  CREATE(sail_int)(&zghz3249);
  CREATE(sail_int)(&zghz3250);
  CREATE(sail_int)(&zghz3251);
  CREATE(sail_int)(&zghz3252);
}

static bool sailgen_CapBoundsEqual(lbits za, lbits zb)
{
  __label__ cleanup_142, end_cleanup_143, end_function_141, end_block_exception_144;

  bool zcbz323;
  RECREATE(lbits)(&zghz3229);
  {
    int64_t zgaz3149;
    {
      int64_t zgaz3148;
      {
        int64_t zgaz3147;
        {
          RECREATE(sail_int)(&zghz3250);
          CONVERT_OF(sail_int, mach_int)(&zghz3250, INT64_C(63));
          RECREATE(sail_int)(&zghz3251);
          CONVERT_OF(sail_int, mach_int)(&zghz3251, INT64_C(0));
          RECREATE(sail_int)(&zghz3252);
          sub_int(&zghz3252, zghz3250, zghz3251);
          zgaz3147 = CONVERT_OF(mach_int, sail_int)(zghz3252);
        }
        zgaz3148 = (zgaz3147 + INT64_C(1));
      }
      zgaz3149 = (zgaz3148 + INT64_C(1));
    }
    {
      RECREATE(sail_int)(&zghz3249);
      CONVERT_OF(sail_int, mach_int)(&zghz3249, zgaz3149);
      UNDEFINED(lbits)(&zghz3229, zghz3249);
    }
  }
  RECREATE(lbits)(&zghz3230);
  {
    int64_t zgaz3146;
    {
      int64_t zgaz3145;
      {
        int64_t zgaz3144;
        {
          RECREATE(sail_int)(&zghz3246);
          CONVERT_OF(sail_int, mach_int)(&zghz3246, INT64_C(63));
          RECREATE(sail_int)(&zghz3247);
          CONVERT_OF(sail_int, mach_int)(&zghz3247, INT64_C(0));
          RECREATE(sail_int)(&zghz3248);
          sub_int(&zghz3248, zghz3246, zghz3247);
          zgaz3144 = CONVERT_OF(mach_int, sail_int)(zghz3248);
        }
        zgaz3145 = (zgaz3144 + INT64_C(1));
      }
      zgaz3146 = (zgaz3145 + INT64_C(1));
    }
    {
      RECREATE(sail_int)(&zghz3245);
      CONVERT_OF(sail_int, mach_int)(&zghz3245, zgaz3146);
      UNDEFINED(lbits)(&zghz3230, zghz3245);
    }
  }
  bool zavalid;
  zavalid = false;
  RECREATE(lbits)(&zghz3231);
  {
    int64_t zgaz3143;
    {
      int64_t zgaz3142;
      {
        int64_t zgaz3141;
        {
          RECREATE(sail_int)(&zghz3242);
          CONVERT_OF(sail_int, mach_int)(&zghz3242, INT64_C(63));
          RECREATE(sail_int)(&zghz3243);
          CONVERT_OF(sail_int, mach_int)(&zghz3243, INT64_C(0));
          RECREATE(sail_int)(&zghz3244);
          sub_int(&zghz3244, zghz3242, zghz3243);
          zgaz3141 = CONVERT_OF(mach_int, sail_int)(zghz3244);
        }
        zgaz3142 = (zgaz3141 + INT64_C(1));
      }
      zgaz3143 = (zgaz3142 + INT64_C(1));
    }
    {
      RECREATE(sail_int)(&zghz3241);
      CONVERT_OF(sail_int, mach_int)(&zghz3241, zgaz3143);
      UNDEFINED(lbits)(&zghz3231, zghz3241);
    }
  }
  RECREATE(lbits)(&zghz3232);
  {
    int64_t zgaz3140;
    {
      int64_t zgaz3139;
      {
        int64_t zgaz3138;
        {
          RECREATE(sail_int)(&zghz3238);
          CONVERT_OF(sail_int, mach_int)(&zghz3238, INT64_C(63));
          RECREATE(sail_int)(&zghz3239);
          CONVERT_OF(sail_int, mach_int)(&zghz3239, INT64_C(0));
          RECREATE(sail_int)(&zghz3240);
          sub_int(&zghz3240, zghz3238, zghz3239);
          zgaz3138 = CONVERT_OF(mach_int, sail_int)(zghz3240);
        }
        zgaz3139 = (zgaz3138 + INT64_C(1));
      }
      zgaz3140 = (zgaz3139 + INT64_C(1));
    }
    {
      RECREATE(sail_int)(&zghz3237);
      CONVERT_OF(sail_int, mach_int)(&zghz3237, zgaz3140);
      UNDEFINED(lbits)(&zghz3232, zghz3237);
    }
  }
  bool zbvalid;
  zbvalid = false;
  {
    __label__ case_138, finish_match_137;

    struct ztuple_z8z5bvzCz0z5bvzCz0z5boolz9 zgaz3132;
    CREATE(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgaz3132);
    sailgen_CapGetBounds(&zgaz3132, za);
    unit zgsz365;
    {
      RECREATE(lbits)(&zghz3235);
      COPY(lbits)(&zghz3235, zgaz3132.ztup0);
      RECREATE(lbits)(&zghz3236);
      COPY(lbits)(&zghz3236, zgaz3132.ztup1);
      bool ztup__2;
      ztup__2 = zgaz3132.ztup2;
      {
        COPY(lbits)(&zghz3229, zghz3235);
        unit zgsz367;
        zgsz367 = UNIT;
      }
      {
        COPY(lbits)(&zghz3230, zghz3236);
        unit zgsz366;
        zgsz366 = UNIT;
      }
      zavalid = ztup__2;
      zgsz365 = UNIT;
      goto finish_match_137;
    }
  case_138: ;
    sail_match_failure("CapBoundsEqual");
  finish_match_137: ;
    unit zgsz374;
    zgsz374 = zgsz365;
    KILL(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgaz3132);
  }
  {
    __label__ case_140, finish_match_139;

    struct ztuple_z8z5bvzCz0z5bvzCz0z5boolz9 zgaz3133;
    CREATE(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgaz3133);
    sailgen_CapGetBounds(&zgaz3133, zb);
    unit zgsz369;
    {
      RECREATE(lbits)(&zghz3233);
      COPY(lbits)(&zghz3233, zgaz3133.ztup0);
      RECREATE(lbits)(&zghz3234);
      COPY(lbits)(&zghz3234, zgaz3133.ztup1);
      bool zuz32;
      zuz32 = zgaz3133.ztup2;
      {
        COPY(lbits)(&zghz3231, zghz3233);
        unit zgsz371;
        zgsz371 = UNIT;
      }
      {
        COPY(lbits)(&zghz3232, zghz3234);
        unit zgsz370;
        zgsz370 = UNIT;
      }
      zbvalid = zuz32;
      zgsz369 = UNIT;
      goto finish_match_139;
    }
  case_140: ;
    sail_match_failure("CapBoundsEqual");
  finish_match_139: ;
    unit zgsz373;
    zgsz373 = zgsz369;
    KILL(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgaz3133);
  }
  bool zgaz3137;
  {
    bool zgaz3136;
    {
      bool zgaz3135;
      {
        bool zgaz3134;
        zgaz3134 = eq_bits(zghz3229, zghz3231);
        bool zgsz375;
        if (zgaz3134) {  zgsz375 = eq_bits(zghz3230, zghz3232);  } else {  zgsz375 = false;  }
        zgaz3135 = zgsz375;
      }
      bool zgsz376;
      if (zgaz3135) {  zgsz376 = zavalid;  } else {  zgsz376 = false;  }
      zgaz3136 = zgsz376;
    }
    bool zgsz377;
    if (zgaz3136) {  zgsz377 = zbvalid;  } else {  zgsz377 = false;  }
    zgaz3137 = zgsz377;
  }
  zcbz323 = zgaz3137;
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
  KILL(sail_int)(&zghz3252);
  KILL(sail_int)(&zghz3251);
  KILL(sail_int)(&zghz3250);
  KILL(sail_int)(&zghz3249);
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
  KILL(lbits)(&zghz3236);
  KILL(lbits)(&zghz3235);
  KILL(lbits)(&zghz3234);
  KILL(lbits)(&zghz3233);
  KILL(lbits)(&zghz3232);
  KILL(lbits)(&zghz3231);
  KILL(lbits)(&zghz3230);
  KILL(lbits)(&zghz3229);
}

static bool sailgen_CapIsRepresentable(lbits, uint64_t);

lbits zghz3253;
sail_int zghz3254;
sail_int zghz3255;
lbits zghz3256;

static void startup_sailgen_CapIsRepresentable(void)
{
  CREATE(lbits)(&zghz3253);
  CREATE(sail_int)(&zghz3254);
  CREATE(sail_int)(&zghz3255);
  CREATE(lbits)(&zghz3256);
}

static bool sailgen_CapIsRepresentable(lbits zc, uint64_t zaddress)
{
  __label__ cleanup_147, end_cleanup_148, end_function_146, end_block_exception_149;

  bool zcbz324;
  RECREATE(lbits)(&zghz3253);
  COPY(lbits)(&zghz3253, zc);
  {
    {
      RECREATE(sail_int)(&zghz3254);
      CONVERT_OF(sail_int, mach_int)(&zghz3254, zCAP_VALUE_HI_BIT);
      RECREATE(sail_int)(&zghz3255);
      CONVERT_OF(sail_int, mach_int)(&zghz3255, zCAP_VALUE_LO_BIT);
      RECREATE(lbits)(&zghz3256);
      CONVERT_OF(lbits, fbits)(&zghz3256, zaddress, UINT64_C(64) , true);
      vector_update_subrange_lbits(&zghz3253, zghz3253, zghz3254, zghz3255, zghz3256);
    }
    unit zgsz378;
    zgsz378 = UNIT;
  }
  zcbz324 = sailgen_CapBoundsEqual(zc, zghz3253);
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
  KILL(lbits)(&zghz3256);
  KILL(sail_int)(&zghz3255);
  KILL(sail_int)(&zghz3254);
  KILL(lbits)(&zghz3253);
}

static int64_t zCAP_TAG_BIT;
static void create_letbind_23(void) {


  int64_t zgsz379;
  zgsz379 = INT64_C(128);
  zCAP_TAG_BIT = zgsz379;

let_end_150: ;
}
static void kill_letbind_23(void) {
}

// register _PC
static uint64_t z_PC;

// register __BranchTaken
static bool z__BranchTaken;

static int64_t zCAP_OTYPE_HI_BIT;
static void create_letbind_24(void) {


  int64_t zgsz380;
  zgsz380 = INT64_C(109);
  zCAP_OTYPE_HI_BIT = zgsz380;

let_end_151: ;
}
static void kill_letbind_24(void) {
}

static int64_t zCAP_OTYPE_LO_BIT;
static void create_letbind_25(void) {


  int64_t zgsz381;
  zgsz381 = INT64_C(95);
  zCAP_OTYPE_LO_BIT = zgsz381;

let_end_152: ;
}
static void kill_letbind_25(void) {
}

static uint64_t sailgen_CapGetObjectType(lbits);

lbits zghz3257;
sail_int zghz3258;
lbits zghz3259;
sail_int zghz3260;
sail_int zghz3261;
lbits zghz3262;

static void startup_sailgen_CapGetObjectType(void)
{
  CREATE(lbits)(&zghz3257);
  CREATE(sail_int)(&zghz3258);
  CREATE(lbits)(&zghz3259);
  CREATE(sail_int)(&zghz3260);
  CREATE(sail_int)(&zghz3261);
  CREATE(lbits)(&zghz3262);
}

static uint64_t sailgen_CapGetObjectType(lbits zc)
{
  __label__ cleanup_155, end_cleanup_156, end_function_154, end_block_exception_157;

  uint64_t zcbz325;
  {
    uint64_t zgaz3151;
    {
      RECREATE(sail_int)(&zghz3260);
      CONVERT_OF(sail_int, mach_int)(&zghz3260, zCAP_OTYPE_HI_BIT);
      RECREATE(sail_int)(&zghz3261);
      CONVERT_OF(sail_int, mach_int)(&zghz3261, zCAP_OTYPE_LO_BIT);
      RECREATE(lbits)(&zghz3262);
      vector_subrange_lbits(&zghz3262, zc, zghz3260, zghz3261);
      zgaz3151 = CONVERT_OF(fbits, lbits)(zghz3262, true);
    }
    {
      RECREATE(lbits)(&zghz3257);
      CONVERT_OF(lbits, fbits)(&zghz3257, zgaz3151, UINT64_C(15) , true);
      RECREATE(sail_int)(&zghz3258);
      CONVERT_OF(sail_int, mach_int)(&zghz3258, zCAP_VALUE_NUM_BITS);
      RECREATE(lbits)(&zghz3259);
      zero_extend(&zghz3259, zghz3257, zghz3258);
      zcbz325 = CONVERT_OF(fbits, lbits)(zghz3259, true);
    }
  }
  goto cleanup_155;
  /* unreachable after return */
  goto end_cleanup_156;
cleanup_155: ;
  goto end_function_154;
end_cleanup_156: ;
end_function_154: ;
  return zcbz325;
end_block_exception_157: ;

  return UINT64_C(0xdeadc0de);
}



static void finish_sailgen_CapGetObjectType(void)
{
  KILL(lbits)(&zghz3262);
  KILL(sail_int)(&zghz3261);
  KILL(sail_int)(&zghz3260);
  KILL(lbits)(&zghz3259);
  KILL(sail_int)(&zghz3258);
  KILL(lbits)(&zghz3257);
}

static int64_t zCAP_FLAGS_HI_BIT;
static void create_letbind_26(void) {


  int64_t zgsz382;
  zgsz382 = INT64_C(63);
  zCAP_FLAGS_HI_BIT = zgsz382;

let_end_158: ;
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


  uint64_t zgsz383;
  zgsz383 = UINT64_C(0b10110);
  zM32_Monitor = zgsz383;

let_end_159: ;
}
static void kill_letbind_27(void) {
}

static uint64_t zCAP_PERM_EXECUTIVE;
static void create_letbind_28(void) {


  uint64_t zgsz384;
  int64_t zgaz3153;
  zgaz3153 = shl_mach_int(INT64_C(1), INT64_C(1));
  {
    sail_int zgsz3812;
    CREATE(sail_int)(&zgsz3812);
    CONVERT_OF(sail_int, mach_int)(&zgsz3812, zgaz3153);
    sail_int zgsz3813;
    CREATE(sail_int)(&zgsz3813);
    CONVERT_OF(sail_int, mach_int)(&zgsz3813, INT64_C(63));
    sail_int zgsz3814;
    CREATE(sail_int)(&zgsz3814);
    CONVERT_OF(sail_int, mach_int)(&zgsz3814, INT64_C(0));
    lbits zgsz3815;
    CREATE(lbits)(&zgsz3815);
    sailgen_integer_subrange(&zgsz3815, zgsz3812, zgsz3813, zgsz3814);
    zgsz384 = CONVERT_OF(fbits, lbits)(zgsz3815, true);
    KILL(lbits)(&zgsz3815);
    KILL(sail_int)(&zgsz3814);
    KILL(sail_int)(&zgsz3813);
    KILL(sail_int)(&zgsz3812);
  }

  zCAP_PERM_EXECUTIVE = zgsz384;

let_end_160: ;
}
static void kill_letbind_28(void) {
}

static int64_t zCAP_PERMS_HI_BIT;
static void create_letbind_29(void) {


  int64_t zgsz385;
  zgsz385 = INT64_C(127);
  zCAP_PERMS_HI_BIT = zgsz385;

let_end_161: ;
}
static void kill_letbind_29(void) {
}

static int64_t zCAP_PERMS_LO_BIT;
static void create_letbind_30(void) {


  int64_t zgsz386;
  zgsz386 = INT64_C(110);
  zCAP_PERMS_LO_BIT = zgsz386;

let_end_162: ;
}
static void kill_letbind_30(void) {
}

static int64_t zCAP_PERMS_NUM_BITS;
static void create_letbind_31(void) {


  int64_t zgsz387;
  int64_t zgaz3154;
  {
    sail_int zgsz3816;
    CREATE(sail_int)(&zgsz3816);
    CONVERT_OF(sail_int, mach_int)(&zgsz3816, zCAP_PERMS_HI_BIT);
    sail_int zgsz3817;
    CREATE(sail_int)(&zgsz3817);
    CONVERT_OF(sail_int, mach_int)(&zgsz3817, zCAP_PERMS_LO_BIT);
    sail_int zgsz3818;
    CREATE(sail_int)(&zgsz3818);
    sub_int(&zgsz3818, zgsz3816, zgsz3817);
    zgaz3154 = CONVERT_OF(mach_int, sail_int)(zgsz3818);
    KILL(sail_int)(&zgsz3818);
    KILL(sail_int)(&zgsz3817);
    KILL(sail_int)(&zgsz3816);
  }
  zgsz387 = (zgaz3154 + INT64_C(1));

  zCAP_PERMS_NUM_BITS = zgsz387;

let_end_163: ;
}
static void kill_letbind_31(void) {
}

static uint64_t sailgen_CapGetPermissions(lbits);

sail_int zghz3263;
sail_int zghz3264;
lbits zghz3265;

static void startup_sailgen_CapGetPermissions(void)
{
  CREATE(sail_int)(&zghz3263);
  CREATE(sail_int)(&zghz3264);
  CREATE(lbits)(&zghz3265);
}

static uint64_t sailgen_CapGetPermissions(lbits zc)
{
  __label__ cleanup_166, end_cleanup_167, end_function_165, end_block_exception_168;

  uint64_t zcbz326;
  {
    RECREATE(sail_int)(&zghz3263);
    CONVERT_OF(sail_int, mach_int)(&zghz3263, zCAP_PERMS_HI_BIT);
    RECREATE(sail_int)(&zghz3264);
    CONVERT_OF(sail_int, mach_int)(&zghz3264, zCAP_PERMS_LO_BIT);
    RECREATE(lbits)(&zghz3265);
    vector_subrange_lbits(&zghz3265, zc, zghz3263, zghz3264);
    zcbz326 = CONVERT_OF(fbits, lbits)(zghz3265, true);
  }
  goto cleanup_166;
  /* unreachable after return */
  goto end_cleanup_167;
cleanup_166: ;
  goto end_function_165;
end_cleanup_167: ;
end_function_165: ;
  return zcbz326;
end_block_exception_168: ;

  return UINT64_C(0xdeadc0de);
}



static void finish_sailgen_CapGetPermissions(void)
{
  KILL(lbits)(&zghz3265);
  KILL(sail_int)(&zghz3264);
  KILL(sail_int)(&zghz3263);
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


  uint64_t zgsz388;
  sail_int zgaz3156;
  CREATE(sail_int)(&zgaz3156);
  {
    sail_int zgsz3825;
    CREATE(sail_int)(&zgsz3825);
    CONVERT_OF(sail_int, mach_int)(&zgsz3825, INT64_C(1));
    sail_int zgsz3826;
    CREATE(sail_int)(&zgsz3826);
    CONVERT_OF(sail_int, mach_int)(&zgsz3826, INT64_C(15));
    shl_int(&zgaz3156, zgsz3825, zgsz3826);
    KILL(sail_int)(&zgsz3826);
    KILL(sail_int)(&zgsz3825);
  }
  {
    sail_int zgsz3822;
    CREATE(sail_int)(&zgsz3822);
    CONVERT_OF(sail_int, mach_int)(&zgsz3822, INT64_C(63));
    sail_int zgsz3823;
    CREATE(sail_int)(&zgsz3823);
    CONVERT_OF(sail_int, mach_int)(&zgsz3823, INT64_C(0));
    lbits zgsz3824;
    CREATE(lbits)(&zgsz3824);
    sailgen_integer_subrange(&zgsz3824, zgaz3156, zgsz3822, zgsz3823);
    zgsz388 = CONVERT_OF(fbits, lbits)(zgsz3824, true);
    KILL(lbits)(&zgsz3824);
    KILL(sail_int)(&zgsz3823);
    KILL(sail_int)(&zgsz3822);
  }
  KILL(sail_int)(&zgaz3156);
  zCAP_PERM_EXECUTE = zgsz388;

let_end_169: ;
}
static void kill_letbind_32(void) {
}

static uint64_t zCAP_PERM_SYSTEM;
static void create_letbind_33(void) {


  uint64_t zgsz389;
  sail_int zgaz3157;
  CREATE(sail_int)(&zgaz3157);
  {
    sail_int zgsz3830;
    CREATE(sail_int)(&zgsz3830);
    CONVERT_OF(sail_int, mach_int)(&zgsz3830, INT64_C(1));
    sail_int zgsz3831;
    CREATE(sail_int)(&zgsz3831);
    CONVERT_OF(sail_int, mach_int)(&zgsz3831, INT64_C(9));
    shl_int(&zgaz3157, zgsz3830, zgsz3831);
    KILL(sail_int)(&zgsz3831);
    KILL(sail_int)(&zgsz3830);
  }
  {
    sail_int zgsz3827;
    CREATE(sail_int)(&zgsz3827);
    CONVERT_OF(sail_int, mach_int)(&zgsz3827, INT64_C(63));
    sail_int zgsz3828;
    CREATE(sail_int)(&zgsz3828);
    CONVERT_OF(sail_int, mach_int)(&zgsz3828, INT64_C(0));
    lbits zgsz3829;
    CREATE(lbits)(&zgsz3829);
    sailgen_integer_subrange(&zgsz3829, zgaz3157, zgsz3827, zgsz3828);
    zgsz389 = CONVERT_OF(fbits, lbits)(zgsz3829, true);
    KILL(lbits)(&zgsz3829);
    KILL(sail_int)(&zgsz3828);
    KILL(sail_int)(&zgsz3827);
  }
  KILL(sail_int)(&zgaz3157);
  zCAP_PERM_SYSTEM = zgsz389;

let_end_170: ;
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

// register DBGBCR_EL1
static zz5vecz8z5bv32z9 zDBGBCR_EL1;

static uint64_t zDebugHalt_SoftwareAccess;
static void create_letbind_34(void) {


  uint64_t zgsz390;
  zgsz390 = UINT64_C(0b110011);
  zDebugHalt_SoftwareAccess = zgsz390;

let_end_171: ;
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


  uint64_t zgsz391;
  zgsz391 = UINT64_C(0b00);
  zMemAttr_NC = zgsz391;

let_end_172: ;
}
static void kill_letbind_35(void) {
}

static uint64_t zMemAttr_WT;
static void create_letbind_36(void) {


  uint64_t zgsz392;
  zgsz392 = UINT64_C(0b10);
  zMemAttr_WT = zgsz392;

let_end_173: ;
}
static void kill_letbind_36(void) {
}

static uint64_t zMemAttr_WB;
static void create_letbind_37(void) {


  uint64_t zgsz393;
  zgsz393 = UINT64_C(0b11);
  zMemAttr_WB = zgsz393;

let_end_174: ;
}
static void kill_letbind_37(void) {
}

static uint64_t zMemHint_No;
static void create_letbind_38(void) {


  uint64_t zgsz394;
  zgsz394 = UINT64_C(0b00);
  zMemHint_No = zgsz394;

let_end_175: ;
}
static void kill_letbind_38(void) {
}

static uint64_t zMemHint_RA;
static void create_letbind_39(void) {


  uint64_t zgsz395;
  zgsz395 = UINT64_C(0b10);
  zMemHint_RA = zgsz395;

let_end_176: ;
}
static void kill_letbind_39(void) {
}

static uint64_t zMemHint_RWA;
static void create_letbind_40(void) {


  uint64_t zgsz396;
  zgsz396 = UINT64_C(0b11);
  zMemHint_RWA = zgsz396;

let_end_177: ;
}
static void kill_letbind_40(void) {
}

// register EventRegister
static uint64_t zEventRegister;

// register _V
static zz5vecz8z5bvz9 z_V;

static uint64_t zDefaultPARTID;
static void create_letbind_41(void) {


  uint64_t zgsz397;
  {
    sail_int zgsz3832;
    CREATE(sail_int)(&zgsz3832);
    CONVERT_OF(sail_int, mach_int)(&zgsz3832, INT64_C(0));
    sail_int zgsz3833;
    CREATE(sail_int)(&zgsz3833);
    CONVERT_OF(sail_int, mach_int)(&zgsz3833, INT64_C(15));
    sail_int zgsz3834;
    CREATE(sail_int)(&zgsz3834);
    CONVERT_OF(sail_int, mach_int)(&zgsz3834, INT64_C(0));
    lbits zgsz3835;
    CREATE(lbits)(&zgsz3835);
    sailgen_integer_subrange(&zgsz3835, zgsz3832, zgsz3833, zgsz3834);
    zgsz397 = CONVERT_OF(fbits, lbits)(zgsz3835, true);
    KILL(lbits)(&zgsz3835);
    KILL(sail_int)(&zgsz3834);
    KILL(sail_int)(&zgsz3833);
    KILL(sail_int)(&zgsz3832);
  }
  zDefaultPARTID = zgsz397;

let_end_178: ;
}
static void kill_letbind_41(void) {
}

static uint64_t zDefaultPMG;
static void create_letbind_42(void) {


  uint64_t zgsz398;
  {
    sail_int zgsz3836;
    CREATE(sail_int)(&zgsz3836);
    CONVERT_OF(sail_int, mach_int)(&zgsz3836, INT64_C(0));
    sail_int zgsz3837;
    CREATE(sail_int)(&zgsz3837);
    CONVERT_OF(sail_int, mach_int)(&zgsz3837, INT64_C(7));
    sail_int zgsz3838;
    CREATE(sail_int)(&zgsz3838);
    CONVERT_OF(sail_int, mach_int)(&zgsz3838, INT64_C(0));
    lbits zgsz3839;
    CREATE(lbits)(&zgsz3839);
    sailgen_integer_subrange(&zgsz3839, zgsz3836, zgsz3837, zgsz3838);
    zgsz398 = CONVERT_OF(fbits, lbits)(zgsz3839, true);
    KILL(lbits)(&zgsz3839);
    KILL(sail_int)(&zgsz3838);
    KILL(sail_int)(&zgsz3837);
    KILL(sail_int)(&zgsz3836);
  }
  zDefaultPMG = zgsz398;

let_end_179: ;
}
static void kill_letbind_42(void) {
}

static int64_t zCAPABILITY_DBYTES;
static void create_letbind_43(void) {


  int64_t zgsz399;
  zgsz399 = INT64_C(16);
  zCAPABILITY_DBYTES = zgsz399;

let_end_180: ;
}
static void kill_letbind_43(void) {
}

static int64_t zLOG2_CAPABILITY_DBYTES;
static void create_letbind_44(void) {


  int64_t zgsz3100;
  zgsz3100 = INT64_C(4);
  zLOG2_CAPABILITY_DBYTES = zgsz3100;

let_end_181: ;
}
static void kill_letbind_44(void) {
}

static uint64_t zDebugHalt_Breakpoint;
static void create_letbind_45(void) {


  uint64_t zgsz3101;
  zgsz3101 = UINT64_C(0b000111);
  zDebugHalt_Breakpoint = zgsz3101;

let_end_182: ;
}
static void kill_letbind_45(void) {
}

static uint64_t zDebugHalt_Watchpoint;
static void create_letbind_46(void) {


  uint64_t zgsz3102;
  zgsz3102 = UINT64_C(0b101011);
  zDebugHalt_Watchpoint = zgsz3102;

let_end_183: ;
}
static void kill_letbind_46(void) {
}

static uint64_t zDebugHalt_HaltInstruction;
static void create_letbind_47(void) {


  uint64_t zgsz3103;
  zgsz3103 = UINT64_C(0b101111);
  zDebugHalt_HaltInstruction = zgsz3103;

let_end_184: ;
}
static void kill_letbind_47(void) {
}

static uint64_t zCAP_NO_SEALING;
static void create_letbind_48(void) {


  uint64_t zgsz3104;
  {
    sail_int zgsz3840;
    CREATE(sail_int)(&zgsz3840);
    CONVERT_OF(sail_int, mach_int)(&zgsz3840, INT64_C(64));
    lbits zgsz3841;
    CREATE(lbits)(&zgsz3841);
    sailgen_Ones(&zgsz3841, zgsz3840);
    zgsz3104 = CONVERT_OF(fbits, lbits)(zgsz3841, true);
    KILL(lbits)(&zgsz3841);
    KILL(sail_int)(&zgsz3840);
  }
  zCAP_NO_SEALING = zgsz3104;

let_end_185: ;
}
static void kill_letbind_48(void) {
}

static uint64_t zCAP_SEAL_TYPE_RB;
static void create_letbind_49(void) {


  uint64_t zgsz3105;
  {
    lbits zgsz3842;
    CREATE(lbits)(&zgsz3842);
    CONVERT_OF(lbits, fbits)(&zgsz3842, UINT64_C(0b01), UINT64_C(2) , true);
    sail_int zgsz3843;
    CREATE(sail_int)(&zgsz3843);
    CONVERT_OF(sail_int, mach_int)(&zgsz3843, INT64_C(64));
    lbits zgsz3844;
    CREATE(lbits)(&zgsz3844);
    zero_extend(&zgsz3844, zgsz3842, zgsz3843);
    zgsz3105 = CONVERT_OF(fbits, lbits)(zgsz3844, true);
    KILL(lbits)(&zgsz3844);
    KILL(sail_int)(&zgsz3843);
    KILL(lbits)(&zgsz3842);
  }
  zCAP_SEAL_TYPE_RB = zgsz3105;

let_end_186: ;
}
static void kill_letbind_49(void) {
}

static uint64_t zCAP_SEAL_TYPE_LPB;
static void create_letbind_50(void) {


  uint64_t zgsz3106;
  {
    lbits zgsz3845;
    CREATE(lbits)(&zgsz3845);
    CONVERT_OF(lbits, fbits)(&zgsz3845, UINT64_C(0b10), UINT64_C(2) , true);
    sail_int zgsz3846;
    CREATE(sail_int)(&zgsz3846);
    CONVERT_OF(sail_int, mach_int)(&zgsz3846, INT64_C(64));
    lbits zgsz3847;
    CREATE(lbits)(&zgsz3847);
    zero_extend(&zgsz3847, zgsz3845, zgsz3846);
    zgsz3106 = CONVERT_OF(fbits, lbits)(zgsz3847, true);
    KILL(lbits)(&zgsz3847);
    KILL(sail_int)(&zgsz3846);
    KILL(lbits)(&zgsz3845);
  }
  zCAP_SEAL_TYPE_LPB = zgsz3106;

let_end_187: ;
}
static void kill_letbind_50(void) {
}

static uint64_t zCAP_SEAL_TYPE_LB;
static void create_letbind_51(void) {


  uint64_t zgsz3107;
  {
    lbits zgsz3848;
    CREATE(lbits)(&zgsz3848);
    CONVERT_OF(lbits, fbits)(&zgsz3848, UINT64_C(0b11), UINT64_C(2) , true);
    sail_int zgsz3849;
    CREATE(sail_int)(&zgsz3849);
    CONVERT_OF(sail_int, mach_int)(&zgsz3849, INT64_C(64));
    lbits zgsz3850;
    CREATE(lbits)(&zgsz3850);
    zero_extend(&zgsz3850, zgsz3848, zgsz3849);
    zgsz3107 = CONVERT_OF(fbits, lbits)(zgsz3850, true);
    KILL(lbits)(&zgsz3850);
    KILL(sail_int)(&zgsz3849);
    KILL(lbits)(&zgsz3848);
  }
  zCAP_SEAL_TYPE_LB = zgsz3107;

let_end_188: ;
}
static void kill_letbind_51(void) {
}

static int64_t zCAP_MAX_FIXED_SEAL_TYPE;
static void create_letbind_52(void) {


  int64_t zgsz3108;
  zgsz3108 = INT64_C(3);
  zCAP_MAX_FIXED_SEAL_TYPE = zgsz3108;

let_end_189: ;
}
static void kill_letbind_52(void) {
}

static uint64_t zCAP_PERM_LOAD;
static void create_letbind_53(void) {


  uint64_t zgsz3109;
  sail_int zgaz3158;
  CREATE(sail_int)(&zgaz3158);
  {
    sail_int zgsz3854;
    CREATE(sail_int)(&zgsz3854);
    CONVERT_OF(sail_int, mach_int)(&zgsz3854, INT64_C(1));
    sail_int zgsz3855;
    CREATE(sail_int)(&zgsz3855);
    CONVERT_OF(sail_int, mach_int)(&zgsz3855, INT64_C(17));
    shl_int(&zgaz3158, zgsz3854, zgsz3855);
    KILL(sail_int)(&zgsz3855);
    KILL(sail_int)(&zgsz3854);
  }
  {
    sail_int zgsz3851;
    CREATE(sail_int)(&zgsz3851);
    CONVERT_OF(sail_int, mach_int)(&zgsz3851, INT64_C(63));
    sail_int zgsz3852;
    CREATE(sail_int)(&zgsz3852);
    CONVERT_OF(sail_int, mach_int)(&zgsz3852, INT64_C(0));
    lbits zgsz3853;
    CREATE(lbits)(&zgsz3853);
    sailgen_integer_subrange(&zgsz3853, zgaz3158, zgsz3851, zgsz3852);
    zgsz3109 = CONVERT_OF(fbits, lbits)(zgsz3853, true);
    KILL(lbits)(&zgsz3853);
    KILL(sail_int)(&zgsz3852);
    KILL(sail_int)(&zgsz3851);
  }
  KILL(sail_int)(&zgaz3158);
  zCAP_PERM_LOAD = zgsz3109;

let_end_190: ;
}
static void kill_letbind_53(void) {
}

static uint64_t zCAP_PERM_STORE;
static void create_letbind_54(void) {


  uint64_t zgsz3110;
  sail_int zgaz3159;
  CREATE(sail_int)(&zgaz3159);
  {
    sail_int zgsz3859;
    CREATE(sail_int)(&zgsz3859);
    CONVERT_OF(sail_int, mach_int)(&zgsz3859, INT64_C(1));
    sail_int zgsz3860;
    CREATE(sail_int)(&zgsz3860);
    CONVERT_OF(sail_int, mach_int)(&zgsz3860, INT64_C(16));
    shl_int(&zgaz3159, zgsz3859, zgsz3860);
    KILL(sail_int)(&zgsz3860);
    KILL(sail_int)(&zgsz3859);
  }
  {
    sail_int zgsz3856;
    CREATE(sail_int)(&zgsz3856);
    CONVERT_OF(sail_int, mach_int)(&zgsz3856, INT64_C(63));
    sail_int zgsz3857;
    CREATE(sail_int)(&zgsz3857);
    CONVERT_OF(sail_int, mach_int)(&zgsz3857, INT64_C(0));
    lbits zgsz3858;
    CREATE(lbits)(&zgsz3858);
    sailgen_integer_subrange(&zgsz3858, zgaz3159, zgsz3856, zgsz3857);
    zgsz3110 = CONVERT_OF(fbits, lbits)(zgsz3858, true);
    KILL(lbits)(&zgsz3858);
    KILL(sail_int)(&zgsz3857);
    KILL(sail_int)(&zgsz3856);
  }
  KILL(sail_int)(&zgaz3159);
  zCAP_PERM_STORE = zgsz3110;

let_end_191: ;
}
static void kill_letbind_54(void) {
}

static uint64_t zCAP_PERM_LOAD_CAP;
static void create_letbind_55(void) {


  uint64_t zgsz3111;
  sail_int zgaz3160;
  CREATE(sail_int)(&zgaz3160);
  {
    sail_int zgsz3864;
    CREATE(sail_int)(&zgsz3864);
    CONVERT_OF(sail_int, mach_int)(&zgsz3864, INT64_C(1));
    sail_int zgsz3865;
    CREATE(sail_int)(&zgsz3865);
    CONVERT_OF(sail_int, mach_int)(&zgsz3865, INT64_C(14));
    shl_int(&zgaz3160, zgsz3864, zgsz3865);
    KILL(sail_int)(&zgsz3865);
    KILL(sail_int)(&zgsz3864);
  }
  {
    sail_int zgsz3861;
    CREATE(sail_int)(&zgsz3861);
    CONVERT_OF(sail_int, mach_int)(&zgsz3861, INT64_C(63));
    sail_int zgsz3862;
    CREATE(sail_int)(&zgsz3862);
    CONVERT_OF(sail_int, mach_int)(&zgsz3862, INT64_C(0));
    lbits zgsz3863;
    CREATE(lbits)(&zgsz3863);
    sailgen_integer_subrange(&zgsz3863, zgaz3160, zgsz3861, zgsz3862);
    zgsz3111 = CONVERT_OF(fbits, lbits)(zgsz3863, true);
    KILL(lbits)(&zgsz3863);
    KILL(sail_int)(&zgsz3862);
    KILL(sail_int)(&zgsz3861);
  }
  KILL(sail_int)(&zgaz3160);
  zCAP_PERM_LOAD_CAP = zgsz3111;

let_end_192: ;
}
static void kill_letbind_55(void) {
}

static uint64_t zCAP_PERM_STORE_CAP;
static void create_letbind_56(void) {


  uint64_t zgsz3112;
  sail_int zgaz3161;
  CREATE(sail_int)(&zgaz3161);
  {
    sail_int zgsz3869;
    CREATE(sail_int)(&zgsz3869);
    CONVERT_OF(sail_int, mach_int)(&zgsz3869, INT64_C(1));
    sail_int zgsz3870;
    CREATE(sail_int)(&zgsz3870);
    CONVERT_OF(sail_int, mach_int)(&zgsz3870, INT64_C(13));
    shl_int(&zgaz3161, zgsz3869, zgsz3870);
    KILL(sail_int)(&zgsz3870);
    KILL(sail_int)(&zgsz3869);
  }
  {
    sail_int zgsz3866;
    CREATE(sail_int)(&zgsz3866);
    CONVERT_OF(sail_int, mach_int)(&zgsz3866, INT64_C(63));
    sail_int zgsz3867;
    CREATE(sail_int)(&zgsz3867);
    CONVERT_OF(sail_int, mach_int)(&zgsz3867, INT64_C(0));
    lbits zgsz3868;
    CREATE(lbits)(&zgsz3868);
    sailgen_integer_subrange(&zgsz3868, zgaz3161, zgsz3866, zgsz3867);
    zgsz3112 = CONVERT_OF(fbits, lbits)(zgsz3868, true);
    KILL(lbits)(&zgsz3868);
    KILL(sail_int)(&zgsz3867);
    KILL(sail_int)(&zgsz3866);
  }
  KILL(sail_int)(&zgaz3161);
  zCAP_PERM_STORE_CAP = zgsz3112;

let_end_193: ;
}
static void kill_letbind_56(void) {
}

static uint64_t zCAP_PERM_STORE_LOCAL;
static void create_letbind_57(void) {


  uint64_t zgsz3113;
  sail_int zgaz3162;
  CREATE(sail_int)(&zgaz3162);
  {
    sail_int zgsz3874;
    CREATE(sail_int)(&zgsz3874);
    CONVERT_OF(sail_int, mach_int)(&zgsz3874, INT64_C(1));
    sail_int zgsz3875;
    CREATE(sail_int)(&zgsz3875);
    CONVERT_OF(sail_int, mach_int)(&zgsz3875, INT64_C(12));
    shl_int(&zgaz3162, zgsz3874, zgsz3875);
    KILL(sail_int)(&zgsz3875);
    KILL(sail_int)(&zgsz3874);
  }
  {
    sail_int zgsz3871;
    CREATE(sail_int)(&zgsz3871);
    CONVERT_OF(sail_int, mach_int)(&zgsz3871, INT64_C(63));
    sail_int zgsz3872;
    CREATE(sail_int)(&zgsz3872);
    CONVERT_OF(sail_int, mach_int)(&zgsz3872, INT64_C(0));
    lbits zgsz3873;
    CREATE(lbits)(&zgsz3873);
    sailgen_integer_subrange(&zgsz3873, zgaz3162, zgsz3871, zgsz3872);
    zgsz3113 = CONVERT_OF(fbits, lbits)(zgsz3873, true);
    KILL(lbits)(&zgsz3873);
    KILL(sail_int)(&zgsz3872);
    KILL(sail_int)(&zgsz3871);
  }
  KILL(sail_int)(&zgaz3162);
  zCAP_PERM_STORE_LOCAL = zgsz3113;

let_end_194: ;
}
static void kill_letbind_57(void) {
}

static uint64_t zCAP_PERM_SEAL;
static void create_letbind_58(void) {


  uint64_t zgsz3114;
  sail_int zgaz3163;
  CREATE(sail_int)(&zgaz3163);
  {
    sail_int zgsz3879;
    CREATE(sail_int)(&zgsz3879);
    CONVERT_OF(sail_int, mach_int)(&zgsz3879, INT64_C(1));
    sail_int zgsz3880;
    CREATE(sail_int)(&zgsz3880);
    CONVERT_OF(sail_int, mach_int)(&zgsz3880, INT64_C(11));
    shl_int(&zgaz3163, zgsz3879, zgsz3880);
    KILL(sail_int)(&zgsz3880);
    KILL(sail_int)(&zgsz3879);
  }
  {
    sail_int zgsz3876;
    CREATE(sail_int)(&zgsz3876);
    CONVERT_OF(sail_int, mach_int)(&zgsz3876, INT64_C(63));
    sail_int zgsz3877;
    CREATE(sail_int)(&zgsz3877);
    CONVERT_OF(sail_int, mach_int)(&zgsz3877, INT64_C(0));
    lbits zgsz3878;
    CREATE(lbits)(&zgsz3878);
    sailgen_integer_subrange(&zgsz3878, zgaz3163, zgsz3876, zgsz3877);
    zgsz3114 = CONVERT_OF(fbits, lbits)(zgsz3878, true);
    KILL(lbits)(&zgsz3878);
    KILL(sail_int)(&zgsz3877);
    KILL(sail_int)(&zgsz3876);
  }
  KILL(sail_int)(&zgaz3163);
  zCAP_PERM_SEAL = zgsz3114;

let_end_195: ;
}
static void kill_letbind_58(void) {
}

static uint64_t zCAP_PERM_UNSEAL;
static void create_letbind_59(void) {


  uint64_t zgsz3115;
  sail_int zgaz3164;
  CREATE(sail_int)(&zgaz3164);
  {
    sail_int zgsz3884;
    CREATE(sail_int)(&zgsz3884);
    CONVERT_OF(sail_int, mach_int)(&zgsz3884, INT64_C(1));
    sail_int zgsz3885;
    CREATE(sail_int)(&zgsz3885);
    CONVERT_OF(sail_int, mach_int)(&zgsz3885, INT64_C(10));
    shl_int(&zgaz3164, zgsz3884, zgsz3885);
    KILL(sail_int)(&zgsz3885);
    KILL(sail_int)(&zgsz3884);
  }
  {
    sail_int zgsz3881;
    CREATE(sail_int)(&zgsz3881);
    CONVERT_OF(sail_int, mach_int)(&zgsz3881, INT64_C(63));
    sail_int zgsz3882;
    CREATE(sail_int)(&zgsz3882);
    CONVERT_OF(sail_int, mach_int)(&zgsz3882, INT64_C(0));
    lbits zgsz3883;
    CREATE(lbits)(&zgsz3883);
    sailgen_integer_subrange(&zgsz3883, zgaz3164, zgsz3881, zgsz3882);
    zgsz3115 = CONVERT_OF(fbits, lbits)(zgsz3883, true);
    KILL(lbits)(&zgsz3883);
    KILL(sail_int)(&zgsz3882);
    KILL(sail_int)(&zgsz3881);
  }
  KILL(sail_int)(&zgaz3164);
  zCAP_PERM_UNSEAL = zgsz3115;

let_end_196: ;
}
static void kill_letbind_59(void) {
}

static uint64_t zCAP_PERM_BRANCH_SEALED_PAIR;
static void create_letbind_60(void) {


  uint64_t zgsz3116;
  sail_int zgaz3165;
  CREATE(sail_int)(&zgaz3165);
  {
    sail_int zgsz3889;
    CREATE(sail_int)(&zgsz3889);
    CONVERT_OF(sail_int, mach_int)(&zgsz3889, INT64_C(1));
    sail_int zgsz3890;
    CREATE(sail_int)(&zgsz3890);
    CONVERT_OF(sail_int, mach_int)(&zgsz3890, INT64_C(8));
    shl_int(&zgaz3165, zgsz3889, zgsz3890);
    KILL(sail_int)(&zgsz3890);
    KILL(sail_int)(&zgsz3889);
  }
  {
    sail_int zgsz3886;
    CREATE(sail_int)(&zgsz3886);
    CONVERT_OF(sail_int, mach_int)(&zgsz3886, INT64_C(63));
    sail_int zgsz3887;
    CREATE(sail_int)(&zgsz3887);
    CONVERT_OF(sail_int, mach_int)(&zgsz3887, INT64_C(0));
    lbits zgsz3888;
    CREATE(lbits)(&zgsz3888);
    sailgen_integer_subrange(&zgsz3888, zgaz3165, zgsz3886, zgsz3887);
    zgsz3116 = CONVERT_OF(fbits, lbits)(zgsz3888, true);
    KILL(lbits)(&zgsz3888);
    KILL(sail_int)(&zgsz3887);
    KILL(sail_int)(&zgsz3886);
  }
  KILL(sail_int)(&zgaz3165);
  zCAP_PERM_BRANCH_SEALED_PAIR = zgsz3116;

let_end_197: ;
}
static void kill_letbind_60(void) {
}

static uint64_t zCAP_PERM_MUTABLE_LOAD;
static void create_letbind_61(void) {


  uint64_t zgsz3117;
  sail_int zgaz3166;
  CREATE(sail_int)(&zgaz3166);
  {
    sail_int zgsz3894;
    CREATE(sail_int)(&zgsz3894);
    CONVERT_OF(sail_int, mach_int)(&zgsz3894, INT64_C(1));
    sail_int zgsz3895;
    CREATE(sail_int)(&zgsz3895);
    CONVERT_OF(sail_int, mach_int)(&zgsz3895, INT64_C(6));
    shl_int(&zgaz3166, zgsz3894, zgsz3895);
    KILL(sail_int)(&zgsz3895);
    KILL(sail_int)(&zgsz3894);
  }
  {
    sail_int zgsz3891;
    CREATE(sail_int)(&zgsz3891);
    CONVERT_OF(sail_int, mach_int)(&zgsz3891, INT64_C(63));
    sail_int zgsz3892;
    CREATE(sail_int)(&zgsz3892);
    CONVERT_OF(sail_int, mach_int)(&zgsz3892, INT64_C(0));
    lbits zgsz3893;
    CREATE(lbits)(&zgsz3893);
    sailgen_integer_subrange(&zgsz3893, zgaz3166, zgsz3891, zgsz3892);
    zgsz3117 = CONVERT_OF(fbits, lbits)(zgsz3893, true);
    KILL(lbits)(&zgsz3893);
    KILL(sail_int)(&zgsz3892);
    KILL(sail_int)(&zgsz3891);
  }
  KILL(sail_int)(&zgaz3166);
  zCAP_PERM_MUTABLE_LOAD = zgsz3117;

let_end_198: ;
}
static void kill_letbind_61(void) {
}

static uint64_t zCAP_PERM_GLOBAL;
static void create_letbind_62(void) {


  uint64_t zgsz3118;
  {
    sail_int zgsz3896;
    CREATE(sail_int)(&zgsz3896);
    CONVERT_OF(sail_int, mach_int)(&zgsz3896, INT64_C(1));
    sail_int zgsz3897;
    CREATE(sail_int)(&zgsz3897);
    CONVERT_OF(sail_int, mach_int)(&zgsz3897, INT64_C(63));
    sail_int zgsz3898;
    CREATE(sail_int)(&zgsz3898);
    CONVERT_OF(sail_int, mach_int)(&zgsz3898, INT64_C(0));
    lbits zgsz3899;
    CREATE(lbits)(&zgsz3899);
    sailgen_integer_subrange(&zgsz3899, zgsz3896, zgsz3897, zgsz3898);
    zgsz3118 = CONVERT_OF(fbits, lbits)(zgsz3899, true);
    KILL(lbits)(&zgsz3899);
    KILL(sail_int)(&zgsz3898);
    KILL(sail_int)(&zgsz3897);
    KILL(sail_int)(&zgsz3896);
  }
  zCAP_PERM_GLOBAL = zgsz3118;

let_end_199: ;
}
static void kill_letbind_62(void) {
}

static uint64_t zCAP_PERM_NONE;
static void create_letbind_63(void) {


  uint64_t zgsz3119;
  {
    sail_int zgsz3900;
    CREATE(sail_int)(&zgsz3900);
    CONVERT_OF(sail_int, mach_int)(&zgsz3900, INT64_C(0));
    sail_int zgsz3901;
    CREATE(sail_int)(&zgsz3901);
    CONVERT_OF(sail_int, mach_int)(&zgsz3901, INT64_C(63));
    sail_int zgsz3902;
    CREATE(sail_int)(&zgsz3902);
    CONVERT_OF(sail_int, mach_int)(&zgsz3902, INT64_C(0));
    lbits zgsz3903;
    CREATE(lbits)(&zgsz3903);
    sailgen_integer_subrange(&zgsz3903, zgsz3900, zgsz3901, zgsz3902);
    zgsz3119 = CONVERT_OF(fbits, lbits)(zgsz3903, true);
    KILL(lbits)(&zgsz3903);
    KILL(sail_int)(&zgsz3902);
    KILL(sail_int)(&zgsz3901);
    KILL(sail_int)(&zgsz3900);
  }
  zCAP_PERM_NONE = zgsz3119;

let_end_200: ;
}
static void kill_letbind_63(void) {
}

static int64_t zCAP_OTYPE_NUM_BITS;
static void create_letbind_64(void) {


  int64_t zgsz3120;
  int64_t zgaz3167;
  {
    sail_int zgsz3904;
    CREATE(sail_int)(&zgsz3904);
    CONVERT_OF(sail_int, mach_int)(&zgsz3904, zCAP_OTYPE_HI_BIT);
    sail_int zgsz3905;
    CREATE(sail_int)(&zgsz3905);
    CONVERT_OF(sail_int, mach_int)(&zgsz3905, zCAP_OTYPE_LO_BIT);
    sail_int zgsz3906;
    CREATE(sail_int)(&zgsz3906);
    sub_int(&zgsz3906, zgsz3904, zgsz3905);
    zgaz3167 = CONVERT_OF(mach_int, sail_int)(zgsz3906);
    KILL(sail_int)(&zgsz3906);
    KILL(sail_int)(&zgsz3905);
    KILL(sail_int)(&zgsz3904);
  }
  zgsz3120 = (zgaz3167 + INT64_C(1));

  zCAP_OTYPE_NUM_BITS = zgsz3120;

let_end_201: ;
}
static void kill_letbind_64(void) {
}

static int64_t zCAP_LENGTH_NUM_BITS;
static void create_letbind_65(void) {


  int64_t zgsz3121;
  zgsz3121 = (zCAP_VALUE_NUM_BITS + INT64_C(1));
  zCAP_LENGTH_NUM_BITS = zgsz3121;

let_end_202: ;
}
static void kill_letbind_65(void) {
}

static sail_int zCAP_MAX_OBJECT_TYPE;
static void create_letbind_66(void) {    CREATE(sail_int)(&zCAP_MAX_OBJECT_TYPE);


  sail_int zgsz3122;
  CREATE(sail_int)(&zgsz3122);
  sail_int zgaz3168;
  CREATE(sail_int)(&zgaz3168);
  {
    sail_int zgsz3908;
    CREATE(sail_int)(&zgsz3908);
    CONVERT_OF(sail_int, mach_int)(&zgsz3908, INT64_C(1));
    sail_int zgsz3909;
    CREATE(sail_int)(&zgsz3909);
    CONVERT_OF(sail_int, mach_int)(&zgsz3909, zCAP_OTYPE_NUM_BITS);
    shl_int(&zgaz3168, zgsz3908, zgsz3909);
    KILL(sail_int)(&zgsz3909);
    KILL(sail_int)(&zgsz3908);
  }
  {
    sail_int zgsz3907;
    CREATE(sail_int)(&zgsz3907);
    CONVERT_OF(sail_int, mach_int)(&zgsz3907, INT64_C(1));
    sub_int(&zgsz3122, zgaz3168, zgsz3907);
    KILL(sail_int)(&zgsz3907);
  }
  KILL(sail_int)(&zgaz3168);
  COPY(sail_int)(&zCAP_MAX_OBJECT_TYPE, zgsz3122);
  KILL(sail_int)(&zgsz3122);
let_end_203: ;
}
static void kill_letbind_66(void) {    KILL(sail_int)(&zCAP_MAX_OBJECT_TYPE);
}

// register __saved_exception_level
static uint64_t z__saved_exception_level;

// register __CNTControlBase
static uint64_t z__CNTControlBase;

static enum z__InstrEnc sailgen_undefined___InstrEnc(unit);

static enum z__InstrEnc sailgen_undefined___InstrEnc(unit zgsz3123)
{
  __label__ end_function_205, end_block_exception_206;

  enum z__InstrEnc zcbz327;
  zcbz327 = z__A64;
end_function_205: ;
  return zcbz327;
end_block_exception_206: ;

  return z__A32;
}

// register __ThisInstr
static uint64_t z__ThisInstr;

// register __ThisInstrEnc
static enum z__InstrEnc z__ThisInstrEnc;














































static void sailgen_undefined_instr_ast(struct zinstr_ast *rop, unit);

sail_int zghz3266;
lbits zghz3267;
sail_int zghz3268;
lbits zghz3269;

static void startup_sailgen_undefined_instr_ast(void)
{
  CREATE(sail_int)(&zghz3266);
  CREATE(lbits)(&zghz3267);
  CREATE(sail_int)(&zghz3268);
  CREATE(lbits)(&zghz3269);
}















































static void sailgen_undefined_instr_ast(struct zinstr_ast *zcbz328, unit zgsz3124)
{
  __label__ end_function_208, end_block_exception_209, end_function_266;

  struct ztuple_z8z5bv2zCz0z5bv5z9 zgaz3171;
  {
    uint64_t zgaz3169;
    {
      RECREATE(sail_int)(&zghz3268);
      CONVERT_OF(sail_int, mach_int)(&zghz3268, INT64_C(2));
      RECREATE(lbits)(&zghz3269);
      UNDEFINED(lbits)(&zghz3269, zghz3268);
      zgaz3169 = CONVERT_OF(fbits, lbits)(zghz3269, true);
    }
    uint64_t zgaz3170;
    {
      RECREATE(sail_int)(&zghz3266);
      CONVERT_OF(sail_int, mach_int)(&zghz3266, INT64_C(5));
      RECREATE(lbits)(&zghz3267);
      UNDEFINED(lbits)(&zghz3267, zghz3266);
      zgaz3170 = CONVERT_OF(fbits, lbits)(zghz3267, true);
    }
    zgaz3171.ztup0 = zgaz3169;
    zgaz3171.ztup1 = zgaz3170;
  }
  sailgen_Instr_BLRR_C_C((*(&zcbz328)), zgaz3171);

end_function_208: ;
  goto end_function_266;
end_block_exception_209: ;
  goto end_function_266;
end_function_266: ;
}



static void finish_sailgen_undefined_instr_ast(void)
{
  KILL(lbits)(&zghz3269);
  KILL(sail_int)(&zghz3268);
  KILL(lbits)(&zghz3267);
  KILL(sail_int)(&zghz3266);
}














































// register __ThisInstrAbstract
static struct zinstr_ast z__ThisInstrAbstract;

// register __highest_el_aarch32
static bool z__highest_el_aarch32;

static uint64_t z__exclusive_granule_sizze;
static void create_letbind_67(void) {


  uint64_t zgsz3126;
  zgsz3126 = UINT64_C(0x4);
  z__exclusive_granule_sizze = zgsz3126;

let_end_210: ;
}
static void kill_letbind_67(void) {
}

static uint64_t zCFG_ID_AA64PFR0_EL1_MPAM;
static void create_letbind_68(void) {


  uint64_t zgsz3127;
  zgsz3127 = UINT64_C(0x1);
  zCFG_ID_AA64PFR0_EL1_MPAM = zgsz3127;

let_end_211: ;
}
static void kill_letbind_68(void) {
}

static bool sailgen_CapUnsignedGreaterThanOrEqual(lbits, lbits);

sail_int zghz3270;
sail_int zghz3271;

static void startup_sailgen_CapUnsignedGreaterThanOrEqual(void)
{
  CREATE(sail_int)(&zghz3270);
  CREATE(sail_int)(&zghz3271);
}

static bool sailgen_CapUnsignedGreaterThanOrEqual(lbits za, lbits zb)
{
  __label__ cleanup_214, end_cleanup_215, end_function_213, end_block_exception_216;

  bool zcbz329;
  {
    RECREATE(sail_int)(&zghz3270);
    sail_unsigned(&zghz3270, za);
    RECREATE(sail_int)(&zghz3271);
    sail_unsigned(&zghz3271, zb);
    zcbz329 = gteq(zghz3270, zghz3271);
  }
  goto cleanup_214;
  /* unreachable after return */
  goto end_cleanup_215;
cleanup_214: ;
  goto end_function_213;
end_cleanup_215: ;
end_function_213: ;
  return zcbz329;
end_block_exception_216: ;

  return false;
}



static void finish_sailgen_CapUnsignedGreaterThanOrEqual(void)
{
  KILL(sail_int)(&zghz3271);
  KILL(sail_int)(&zghz3270);
}

static bool sailgen_CapIsRepresentableFast(lbits, uint64_t);

lbits zghz3272;
lbits zghz3273;
sail_int zghz3274;
lbits zghz3275;
lbits zghz3276;
lbits zghz3277;
lbits zghz3278;
sail_int zghz3279;
lbits zghz3280;
sail_int zghz3281;
lbits zghz3282;
sail_int zghz3283;
lbits zghz3284;
sail_int zghz3285;
sail_int zghz3286;
sail_int zghz3287;
sail_int zghz3288;
sail_int zghz3289;
sail_int zghz3290;
sail_int zghz3291;
sail_int zghz3292;
sail_int zghz3293;
sail_int zghz3294;
sail_int zghz3295;
sail_int zghz3296;
lbits zghz3297;
sail_int zghz3298;
lbits zghz3299;
sail_int zghz3300;
sail_int zghz3301;
sail_int zghz3302;
lbits zghz3303;
sail_int zghz3304;
lbits zghz3305;
lbits zghz3306;
sail_int zghz3307;
lbits zghz3308;
sail_int zghz3309;
sail_int zghz3310;
sail_int zghz3311;
sail_int zghz3312;
lbits zghz3313;
sail_int zghz3314;
lbits zghz3315;
sail_int zghz3316;
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
lbits zghz3328;
sail_int zghz3329;
lbits zghz3330;
sail_int zghz3331;
sail_int zghz3332;
sail_int zghz3333;
sail_int zghz3334;
sail_int zghz3335;
sail_int zghz3336;
sail_int zghz3337;
sail_int zghz3338;
sail_int zghz3339;
sail_int zghz3340;
lbits zghz3341;
sail_int zghz3342;
lbits zghz3343;
sail_int zghz3344;
lbits zghz3345;
sail_int zghz3346;
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

static void startup_sailgen_CapIsRepresentableFast(void)
{
  CREATE(lbits)(&zghz3272);
  CREATE(lbits)(&zghz3273);
  CREATE(sail_int)(&zghz3274);
  CREATE(lbits)(&zghz3275);
  CREATE(lbits)(&zghz3276);
  CREATE(lbits)(&zghz3277);
  CREATE(lbits)(&zghz3278);
  CREATE(sail_int)(&zghz3279);
  CREATE(lbits)(&zghz3280);
  CREATE(sail_int)(&zghz3281);
  CREATE(lbits)(&zghz3282);
  CREATE(sail_int)(&zghz3283);
  CREATE(lbits)(&zghz3284);
  CREATE(sail_int)(&zghz3285);
  CREATE(sail_int)(&zghz3286);
  CREATE(sail_int)(&zghz3287);
  CREATE(sail_int)(&zghz3288);
  CREATE(sail_int)(&zghz3289);
  CREATE(sail_int)(&zghz3290);
  CREATE(sail_int)(&zghz3291);
  CREATE(sail_int)(&zghz3292);
  CREATE(sail_int)(&zghz3293);
  CREATE(sail_int)(&zghz3294);
  CREATE(sail_int)(&zghz3295);
  CREATE(sail_int)(&zghz3296);
  CREATE(lbits)(&zghz3297);
  CREATE(sail_int)(&zghz3298);
  CREATE(lbits)(&zghz3299);
  CREATE(sail_int)(&zghz3300);
  CREATE(sail_int)(&zghz3301);
  CREATE(sail_int)(&zghz3302);
  CREATE(lbits)(&zghz3303);
  CREATE(sail_int)(&zghz3304);
  CREATE(lbits)(&zghz3305);
  CREATE(lbits)(&zghz3306);
  CREATE(sail_int)(&zghz3307);
  CREATE(lbits)(&zghz3308);
  CREATE(sail_int)(&zghz3309);
  CREATE(sail_int)(&zghz3310);
  CREATE(sail_int)(&zghz3311);
  CREATE(sail_int)(&zghz3312);
  CREATE(lbits)(&zghz3313);
  CREATE(sail_int)(&zghz3314);
  CREATE(lbits)(&zghz3315);
  CREATE(sail_int)(&zghz3316);
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
  CREATE(lbits)(&zghz3328);
  CREATE(sail_int)(&zghz3329);
  CREATE(lbits)(&zghz3330);
  CREATE(sail_int)(&zghz3331);
  CREATE(sail_int)(&zghz3332);
  CREATE(sail_int)(&zghz3333);
  CREATE(sail_int)(&zghz3334);
  CREATE(sail_int)(&zghz3335);
  CREATE(sail_int)(&zghz3336);
  CREATE(sail_int)(&zghz3337);
  CREATE(sail_int)(&zghz3338);
  CREATE(sail_int)(&zghz3339);
  CREATE(sail_int)(&zghz3340);
  CREATE(lbits)(&zghz3341);
  CREATE(sail_int)(&zghz3342);
  CREATE(lbits)(&zghz3343);
  CREATE(sail_int)(&zghz3344);
  CREATE(lbits)(&zghz3345);
  CREATE(sail_int)(&zghz3346);
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
}

static bool sailgen_CapIsRepresentableFast(lbits zc, uint64_t zincrement_name__arg)
{
  __label__ case_219, finish_match_218, end_function_220, end_block_exception_229;

  bool zcbz330;
  uint64_t zincrement_name;
  zincrement_name = zincrement_name__arg;
  uint64_t zB3;
  {
    int64_t zgaz3214;
    {
      int64_t zgaz3213;
      {
        int64_t zgaz3212;
        {
          int64_t zgaz3210;
          {
            int64_t zgaz3207;
            {
              int64_t zgaz3206;
              {
                RECREATE(sail_int)(&zghz3358);
                CONVERT_OF(sail_int, mach_int)(&zghz3358, INT64_C(79));
                RECREATE(sail_int)(&zghz3359);
                CONVERT_OF(sail_int, mach_int)(&zghz3359, INT64_C(64));
                RECREATE(sail_int)(&zghz3360);
                sub_int(&zghz3360, zghz3358, zghz3359);
                zgaz3206 = CONVERT_OF(mach_int, sail_int)(zghz3360);
              }
              zgaz3207 = (zgaz3206 + INT64_C(1));
            }
            {
              RECREATE(sail_int)(&zghz3355);
              CONVERT_OF(sail_int, mach_int)(&zghz3355, zgaz3207);
              RECREATE(sail_int)(&zghz3356);
              CONVERT_OF(sail_int, mach_int)(&zghz3356, INT64_C(1));
              RECREATE(sail_int)(&zghz3357);
              sub_int(&zghz3357, zghz3355, zghz3356);
              zgaz3210 = CONVERT_OF(mach_int, sail_int)(zghz3357);
            }
          }
          int64_t zgaz3211;
          {
            int64_t zgaz3209;
            {
              int64_t zgaz3208;
              {
                RECREATE(sail_int)(&zghz3352);
                CONVERT_OF(sail_int, mach_int)(&zghz3352, INT64_C(79));
                RECREATE(sail_int)(&zghz3353);
                CONVERT_OF(sail_int, mach_int)(&zghz3353, INT64_C(64));
                RECREATE(sail_int)(&zghz3354);
                sub_int(&zghz3354, zghz3352, zghz3353);
                zgaz3208 = CONVERT_OF(mach_int, sail_int)(zghz3354);
              }
              zgaz3209 = (zgaz3208 + INT64_C(1));
            }
            {
              RECREATE(sail_int)(&zghz3349);
              CONVERT_OF(sail_int, mach_int)(&zghz3349, zgaz3209);
              RECREATE(sail_int)(&zghz3350);
              CONVERT_OF(sail_int, mach_int)(&zghz3350, INT64_C(3));
              RECREATE(sail_int)(&zghz3351);
              sub_int(&zghz3351, zghz3349, zghz3350);
              zgaz3211 = CONVERT_OF(mach_int, sail_int)(zghz3351);
            }
          }
          {
            RECREATE(sail_int)(&zghz3346);
            CONVERT_OF(sail_int, mach_int)(&zghz3346, zgaz3210);
            RECREATE(sail_int)(&zghz3347);
            CONVERT_OF(sail_int, mach_int)(&zghz3347, zgaz3211);
            RECREATE(sail_int)(&zghz3348);
            sub_int(&zghz3348, zghz3346, zghz3347);
            zgaz3212 = CONVERT_OF(mach_int, sail_int)(zghz3348);
          }
        }
        zgaz3213 = (zgaz3212 + INT64_C(1));
      }
      zgaz3214 = (INT64_C(0) + zgaz3213);
    }
    {
      RECREATE(sail_int)(&zghz3344);
      CONVERT_OF(sail_int, mach_int)(&zghz3344, zgaz3214);
      RECREATE(lbits)(&zghz3345);
      UNDEFINED(lbits)(&zghz3345, zghz3344);
      zB3 = CONVERT_OF(fbits, lbits)(zghz3345, true);
    }
  }
  uint64_t zR;
  {
    RECREATE(sail_int)(&zghz3342);
    CONVERT_OF(sail_int, mach_int)(&zghz3342, INT64_C(16));
    RECREATE(lbits)(&zghz3343);
    UNDEFINED(lbits)(&zghz3343, zghz3342);
    zR = CONVERT_OF(fbits, lbits)(zghz3343, true);
  }
  uint64_t zR3;
  {
    RECREATE(sail_int)(&zghz3340);
    CONVERT_OF(sail_int, mach_int)(&zghz3340, INT64_C(3));
    RECREATE(lbits)(&zghz3341);
    UNDEFINED(lbits)(&zghz3341, zghz3340);
    zR3 = CONVERT_OF(fbits, lbits)(zghz3341, true);
  }
  uint64_t za_mid;
  {
    int64_t zgaz3205;
    {
      int64_t zgaz3204;
      {
        int64_t zgaz3203;
        {
          int64_t zgaz3202;
          {
            int64_t zgaz3201;
            {
              int64_t zgaz3200;
              {
                RECREATE(sail_int)(&zghz3337);
                CONVERT_OF(sail_int, mach_int)(&zghz3337, INT64_C(79));
                RECREATE(sail_int)(&zghz3338);
                CONVERT_OF(sail_int, mach_int)(&zghz3338, INT64_C(64));
                RECREATE(sail_int)(&zghz3339);
                sub_int(&zghz3339, zghz3337, zghz3338);
                zgaz3200 = CONVERT_OF(mach_int, sail_int)(zghz3339);
              }
              zgaz3201 = (zgaz3200 + INT64_C(1));
            }
            {
              RECREATE(sail_int)(&zghz3334);
              CONVERT_OF(sail_int, mach_int)(&zghz3334, zgaz3201);
              RECREATE(sail_int)(&zghz3335);
              CONVERT_OF(sail_int, mach_int)(&zghz3335, INT64_C(1));
              RECREATE(sail_int)(&zghz3336);
              sub_int(&zghz3336, zghz3334, zghz3335);
              zgaz3202 = CONVERT_OF(mach_int, sail_int)(zghz3336);
            }
          }
          {
            RECREATE(sail_int)(&zghz3331);
            CONVERT_OF(sail_int, mach_int)(&zghz3331, zgaz3202);
            RECREATE(sail_int)(&zghz3332);
            CONVERT_OF(sail_int, mach_int)(&zghz3332, INT64_C(0));
            RECREATE(sail_int)(&zghz3333);
            sub_int(&zghz3333, zghz3331, zghz3332);
            zgaz3203 = CONVERT_OF(mach_int, sail_int)(zghz3333);
          }
        }
        zgaz3204 = (zgaz3203 + INT64_C(1));
      }
      zgaz3205 = (INT64_C(0) + zgaz3204);
    }
    {
      RECREATE(sail_int)(&zghz3329);
      CONVERT_OF(sail_int, mach_int)(&zghz3329, zgaz3205);
      RECREATE(lbits)(&zghz3330);
      UNDEFINED(lbits)(&zghz3330, zghz3329);
      za_mid = CONVERT_OF(fbits, lbits)(zghz3330, true);
    }
  }
  uint64_t zdiff;
  {
    RECREATE(sail_int)(&zghz3327);
    CONVERT_OF(sail_int, mach_int)(&zghz3327, INT64_C(16));
    RECREATE(lbits)(&zghz3328);
    UNDEFINED(lbits)(&zghz3328, zghz3327);
    zdiff = CONVERT_OF(fbits, lbits)(zghz3328, true);
  }
  uint64_t zdiff1;
  {
    RECREATE(sail_int)(&zghz3325);
    CONVERT_OF(sail_int, mach_int)(&zghz3325, INT64_C(16));
    RECREATE(lbits)(&zghz3326);
    UNDEFINED(lbits)(&zghz3326, zghz3325);
    zdiff1 = CONVERT_OF(fbits, lbits)(zghz3326, true);
  }
  uint64_t zi_mid;
  {
    int64_t zgaz3199;
    {
      int64_t zgaz3198;
      {
        int64_t zgaz3197;
        {
          int64_t zgaz3196;
          {
            int64_t zgaz3195;
            {
              int64_t zgaz3194;
              {
                RECREATE(sail_int)(&zghz3322);
                CONVERT_OF(sail_int, mach_int)(&zghz3322, INT64_C(79));
                RECREATE(sail_int)(&zghz3323);
                CONVERT_OF(sail_int, mach_int)(&zghz3323, INT64_C(64));
                RECREATE(sail_int)(&zghz3324);
                sub_int(&zghz3324, zghz3322, zghz3323);
                zgaz3194 = CONVERT_OF(mach_int, sail_int)(zghz3324);
              }
              zgaz3195 = (zgaz3194 + INT64_C(1));
            }
            {
              RECREATE(sail_int)(&zghz3319);
              CONVERT_OF(sail_int, mach_int)(&zghz3319, zgaz3195);
              RECREATE(sail_int)(&zghz3320);
              CONVERT_OF(sail_int, mach_int)(&zghz3320, INT64_C(1));
              RECREATE(sail_int)(&zghz3321);
              sub_int(&zghz3321, zghz3319, zghz3320);
              zgaz3196 = CONVERT_OF(mach_int, sail_int)(zghz3321);
            }
          }
          {
            RECREATE(sail_int)(&zghz3316);
            CONVERT_OF(sail_int, mach_int)(&zghz3316, zgaz3196);
            RECREATE(sail_int)(&zghz3317);
            CONVERT_OF(sail_int, mach_int)(&zghz3317, INT64_C(0));
            RECREATE(sail_int)(&zghz3318);
            sub_int(&zghz3318, zghz3316, zghz3317);
            zgaz3197 = CONVERT_OF(mach_int, sail_int)(zghz3318);
          }
        }
        zgaz3198 = (zgaz3197 + INT64_C(1));
      }
      zgaz3199 = (INT64_C(0) + zgaz3198);
    }
    {
      RECREATE(sail_int)(&zghz3314);
      CONVERT_OF(sail_int, mach_int)(&zghz3314, zgaz3199);
      RECREATE(lbits)(&zghz3315);
      UNDEFINED(lbits)(&zghz3315, zghz3314);
      zi_mid = CONVERT_OF(fbits, lbits)(zghz3315, true);
    }
  }
  uint64_t zi_top;
  {
    RECREATE(sail_int)(&zghz3312);
    CONVERT_OF(sail_int, mach_int)(&zghz3312, INT64_C(64));
    RECREATE(lbits)(&zghz3313);
    UNDEFINED(lbits)(&zghz3313, zghz3312);
    zi_top = CONVERT_OF(fbits, lbits)(zghz3313, true);
  }
  int64_t zgaz3175;
  zgaz3175 = sailgen_CapGetExponent(zc);
  {
    int64_t zexp;
    zexp = zgaz3175;
    bool zgaz3177;
    {
      int64_t zgaz3176;
      {
        RECREATE(sail_int)(&zghz3309);
        CONVERT_OF(sail_int, mach_int)(&zghz3309, zCAP_MAX_EXPONENT);
        RECREATE(sail_int)(&zghz3310);
        CONVERT_OF(sail_int, mach_int)(&zghz3310, INT64_C(2));
        RECREATE(sail_int)(&zghz3311);
        sub_int(&zghz3311, zghz3309, zghz3310);
        zgaz3176 = CONVERT_OF(mach_int, sail_int)(zghz3311);
      }
      zgaz3177 = (zexp >= zgaz3176);
    }
    if (zgaz3177) {
      __label__ cleanup_227, end_cleanup_228;

      zcbz330 = true;
      goto cleanup_227;
      /* unreachable after return */
      goto end_cleanup_228;
    cleanup_227: ;
      goto end_function_220;
    end_cleanup_228: ;
    } else {
      uint64_t za;
      za = sailgen_CapGetValue(zc);
      uint64_t zashadowz33;
      zashadowz33 = sailgen_CapBoundsAddress(za);
      uint64_t zincrement_nameshadowz34;
      zincrement_nameshadowz34 = sailgen_CapBoundsAddress(zincrement_name);
      uint64_t zi_topshadowz35;
      {
        int64_t zgaz3193;
        zgaz3193 = (zexp + zCAP_MW);
        {
          RECREATE(lbits)(&zghz3306);
          CONVERT_OF(lbits, fbits)(&zghz3306, zincrement_nameshadowz34, UINT64_C(64) , true);
          RECREATE(sail_int)(&zghz3307);
          CONVERT_OF(sail_int, mach_int)(&zghz3307, zgaz3193);
          RECREATE(lbits)(&zghz3308);
          arith_shiftr(&zghz3308, zghz3306, zghz3307);
          zi_topshadowz35 = CONVERT_OF(fbits, lbits)(zghz3308, true);
        }
      }
      uint64_t zi_midshadowz36;
      {
        uint64_t zgaz3191;
        {
          RECREATE(lbits)(&zghz3303);
          CONVERT_OF(lbits, fbits)(&zghz3303, zincrement_nameshadowz34, UINT64_C(64) , true);
          RECREATE(sail_int)(&zghz3304);
          CONVERT_OF(sail_int, mach_int)(&zghz3304, zexp);
          RECREATE(lbits)(&zghz3305);
          shiftr(&zghz3305, zghz3303, zghz3304);
          zgaz3191 = CONVERT_OF(fbits, lbits)(zghz3305, true);
        }
        int64_t zgaz3192;
        {
          RECREATE(sail_int)(&zghz3300);
          CONVERT_OF(sail_int, mach_int)(&zghz3300, zCAP_MW);
          RECREATE(sail_int)(&zghz3301);
          CONVERT_OF(sail_int, mach_int)(&zghz3301, INT64_C(1));
          RECREATE(sail_int)(&zghz3302);
          sub_int(&zghz3302, zghz3300, zghz3301);
          zgaz3192 = CONVERT_OF(mach_int, sail_int)(zghz3302);
        }
        zi_midshadowz36 = (safe_rshift(UINT64_MAX, 64 - 16) & (zgaz3191 >> INT64_C(0)));
      }
      uint64_t za_midshadowz37;
      {
        uint64_t zgaz3189;
        {
          RECREATE(lbits)(&zghz3297);
          CONVERT_OF(lbits, fbits)(&zghz3297, zashadowz33, UINT64_C(64) , true);
          RECREATE(sail_int)(&zghz3298);
          CONVERT_OF(sail_int, mach_int)(&zghz3298, zexp);
          RECREATE(lbits)(&zghz3299);
          shiftr(&zghz3299, zghz3297, zghz3298);
          zgaz3189 = CONVERT_OF(fbits, lbits)(zghz3299, true);
        }
        int64_t zgaz3190;
        {
          RECREATE(sail_int)(&zghz3294);
          CONVERT_OF(sail_int, mach_int)(&zghz3294, zCAP_MW);
          RECREATE(sail_int)(&zghz3295);
          CONVERT_OF(sail_int, mach_int)(&zghz3295, INT64_C(1));
          RECREATE(sail_int)(&zghz3296);
          sub_int(&zghz3296, zghz3294, zghz3295);
          zgaz3190 = CONVERT_OF(mach_int, sail_int)(zghz3296);
        }
        za_midshadowz37 = (safe_rshift(UINT64_MAX, 64 - 16) & (zgaz3189 >> INT64_C(0)));
      }
      uint64_t zB3shadowz38;
      {
        uint64_t zgaz3186;
        zgaz3186 = sailgen_CapGetBottom(zc);
        int64_t zgaz3187;
        {
          RECREATE(sail_int)(&zghz3291);
          CONVERT_OF(sail_int, mach_int)(&zghz3291, zCAP_MW);
          RECREATE(sail_int)(&zghz3292);
          CONVERT_OF(sail_int, mach_int)(&zghz3292, INT64_C(1));
          RECREATE(sail_int)(&zghz3293);
          sub_int(&zghz3293, zghz3291, zghz3292);
          zgaz3187 = CONVERT_OF(mach_int, sail_int)(zghz3293);
        }
        int64_t zgaz3188;
        {
          RECREATE(sail_int)(&zghz3288);
          CONVERT_OF(sail_int, mach_int)(&zghz3288, zCAP_MW);
          RECREATE(sail_int)(&zghz3289);
          CONVERT_OF(sail_int, mach_int)(&zghz3289, INT64_C(3));
          RECREATE(sail_int)(&zghz3290);
          sub_int(&zghz3290, zghz3288, zghz3289);
          zgaz3188 = CONVERT_OF(mach_int, sail_int)(zghz3290);
        }
        zB3shadowz38 = (safe_rshift(UINT64_MAX, 64 - 3) & (zgaz3186 >> zgaz3188));
      }
      uint64_t zR3shadowz39;
      zR3shadowz39 = ((zB3shadowz38 - UINT64_C(0b001)) & UINT64_C(0b111));
      uint64_t zRshadowz310;
      {
        uint64_t zgaz3185;
        {
          int64_t zgaz3184;
          {
            RECREATE(sail_int)(&zghz3285);
            CONVERT_OF(sail_int, mach_int)(&zghz3285, zCAP_MW);
            RECREATE(sail_int)(&zghz3286);
            CONVERT_OF(sail_int, mach_int)(&zghz3286, INT64_C(3));
            RECREATE(sail_int)(&zghz3287);
            sub_int(&zghz3287, zghz3285, zghz3286);
            zgaz3184 = CONVERT_OF(mach_int, sail_int)(zghz3287);
          }
          {
            RECREATE(sail_int)(&zghz3283);
            CONVERT_OF(sail_int, mach_int)(&zghz3283, zgaz3184);
            RECREATE(lbits)(&zghz3284);
            sailgen_Zeros(&zghz3284, zghz3283);
            zgaz3185 = CONVERT_OF(fbits, lbits)(zghz3284, true);
          }
        }
        zRshadowz310 = (zR3shadowz39 << 13) | zgaz3185;
      }
      uint64_t zdiffshadowz311;
      zdiffshadowz311 = ((zRshadowz310 - za_midshadowz37) & UINT64_C(0xFFFF));
      uint64_t zdiff1shadowz312;
      {
        RECREATE(lbits)(&zghz3280);
        CONVERT_OF(lbits, fbits)(&zghz3280, zdiffshadowz311, UINT64_C(16) , true);
        RECREATE(sail_int)(&zghz3281);
        CONVERT_OF(sail_int, mach_int)(&zghz3281, INT64_C(1));
        RECREATE(lbits)(&zghz3282);
        sub_bits_int(&zghz3282, zghz3280, zghz3281);
        zdiff1shadowz312 = CONVERT_OF(fbits, lbits)(zghz3282, true);
      }
      bool zgaz3178;
      {
        RECREATE(lbits)(&zghz3278);
        CONVERT_OF(lbits, fbits)(&zghz3278, zi_topshadowz35, UINT64_C(64) , true);
        RECREATE(sail_int)(&zghz3279);
        CONVERT_OF(sail_int, mach_int)(&zghz3279, INT64_C(0));
        zgaz3178 = sailgen_eq_bits_int(zghz3278, zghz3279);
      }
      if (zgaz3178) {
        __label__ cleanup_225, end_cleanup_226;

        bool zgaz3179;
        {
          RECREATE(lbits)(&zghz3276);
          CONVERT_OF(lbits, fbits)(&zghz3276, zi_midshadowz36, UINT64_C(16) , true);
          RECREATE(lbits)(&zghz3277);
          CONVERT_OF(lbits, fbits)(&zghz3277, zdiff1shadowz312, UINT64_C(16) , true);
          zgaz3179 = sailgen_CapUnsignedLessThan(zghz3276, zghz3277);
        }
        zcbz330 = zgaz3179;
        goto cleanup_225;
        /* unreachable after return */
        goto end_cleanup_226;
      cleanup_225: ;
        goto end_function_220;
      end_cleanup_226: ;
      } else {
        bool zgaz3181;
        {
          uint64_t zgaz3180;
          {
            RECREATE(sail_int)(&zghz3274);
            CONVERT_OF(sail_int, mach_int)(&zghz3274, zCAP_VALUE_NUM_BITS);
            RECREATE(lbits)(&zghz3275);
            sailgen_Ones(&zghz3275, zghz3274);
            zgaz3180 = CONVERT_OF(fbits, lbits)(zghz3275, true);
          }
          zgaz3181 = (zi_topshadowz35 == zgaz3180);
        }
        if (zgaz3181) {
          __label__ cleanup_223, end_cleanup_224;

          bool zgaz3183;
          {
            bool zgaz3182;
            {
              RECREATE(lbits)(&zghz3272);
              CONVERT_OF(lbits, fbits)(&zghz3272, zi_midshadowz36, UINT64_C(16) , true);
              RECREATE(lbits)(&zghz3273);
              CONVERT_OF(lbits, fbits)(&zghz3273, zdiffshadowz311, UINT64_C(16) , true);
              zgaz3182 = sailgen_CapUnsignedGreaterThanOrEqual(zghz3272, zghz3273);
            }
            bool zgsz3130;
            if (zgaz3182) {  zgsz3130 = (zRshadowz310 != za_midshadowz37);  } else {  zgsz3130 = false;  }
            zgaz3183 = zgsz3130;
          }
          zcbz330 = zgaz3183;
          goto cleanup_223;
          /* unreachable after return */
          goto end_cleanup_224;
        cleanup_223: ;
          goto end_function_220;
        end_cleanup_224: ;
        } else {
          __label__ cleanup_221, end_cleanup_222;

          zcbz330 = false;
          goto cleanup_221;
          /* unreachable after return */
          goto end_cleanup_222;
        cleanup_221: ;
          goto end_function_220;
        end_cleanup_222: ;
        }
      }
    }
    goto finish_match_218;
  }
case_219: ;
  sail_match_failure("CapIsRepresentableFast");
finish_match_218: ;










end_function_220: ;
  return zcbz330;
end_block_exception_229: ;

  return false;
}



static void finish_sailgen_CapIsRepresentableFast(void)
{
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
  KILL(sail_int)(&zghz3346);
  KILL(lbits)(&zghz3345);
  KILL(sail_int)(&zghz3344);
  KILL(lbits)(&zghz3343);
  KILL(sail_int)(&zghz3342);
  KILL(lbits)(&zghz3341);
  KILL(sail_int)(&zghz3340);
  KILL(sail_int)(&zghz3339);
  KILL(sail_int)(&zghz3338);
  KILL(sail_int)(&zghz3337);
  KILL(sail_int)(&zghz3336);
  KILL(sail_int)(&zghz3335);
  KILL(sail_int)(&zghz3334);
  KILL(sail_int)(&zghz3333);
  KILL(sail_int)(&zghz3332);
  KILL(sail_int)(&zghz3331);
  KILL(lbits)(&zghz3330);
  KILL(sail_int)(&zghz3329);
  KILL(lbits)(&zghz3328);
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
  KILL(sail_int)(&zghz3316);
  KILL(lbits)(&zghz3315);
  KILL(sail_int)(&zghz3314);
  KILL(lbits)(&zghz3313);
  KILL(sail_int)(&zghz3312);
  KILL(sail_int)(&zghz3311);
  KILL(sail_int)(&zghz3310);
  KILL(sail_int)(&zghz3309);
  KILL(lbits)(&zghz3308);
  KILL(sail_int)(&zghz3307);
  KILL(lbits)(&zghz3306);
  KILL(lbits)(&zghz3305);
  KILL(sail_int)(&zghz3304);
  KILL(lbits)(&zghz3303);
  KILL(sail_int)(&zghz3302);
  KILL(sail_int)(&zghz3301);
  KILL(sail_int)(&zghz3300);
  KILL(lbits)(&zghz3299);
  KILL(sail_int)(&zghz3298);
  KILL(lbits)(&zghz3297);
  KILL(sail_int)(&zghz3296);
  KILL(sail_int)(&zghz3295);
  KILL(sail_int)(&zghz3294);
  KILL(sail_int)(&zghz3293);
  KILL(sail_int)(&zghz3292);
  KILL(sail_int)(&zghz3291);
  KILL(sail_int)(&zghz3290);
  KILL(sail_int)(&zghz3289);
  KILL(sail_int)(&zghz3288);
  KILL(sail_int)(&zghz3287);
  KILL(sail_int)(&zghz3286);
  KILL(sail_int)(&zghz3285);
  KILL(lbits)(&zghz3284);
  KILL(sail_int)(&zghz3283);
  KILL(lbits)(&zghz3282);
  KILL(sail_int)(&zghz3281);
  KILL(lbits)(&zghz3280);
  KILL(sail_int)(&zghz3279);
  KILL(lbits)(&zghz3278);
  KILL(lbits)(&zghz3277);
  KILL(lbits)(&zghz3276);
  KILL(lbits)(&zghz3275);
  KILL(sail_int)(&zghz3274);
  KILL(lbits)(&zghz3273);
  KILL(lbits)(&zghz3272);
}

// register SP_EL3
static lbits zSP_EL3;

static int64_t zCAPABILITY_DBITS;
static void create_letbind_69(void) {


  int64_t zgsz3131;
  {
    sail_int zgsz31003;
    CREATE(sail_int)(&zgsz31003);
    CONVERT_OF(sail_int, mach_int)(&zgsz31003, zCAPABILITY_DBYTES);
    sail_int zgsz31004;
    CREATE(sail_int)(&zgsz31004);
    CONVERT_OF(sail_int, mach_int)(&zgsz31004, INT64_C(8));
    sail_int zgsz31005;
    CREATE(sail_int)(&zgsz31005);
    mult_int(&zgsz31005, zgsz31003, zgsz31004);
    zgsz3131 = CONVERT_OF(mach_int, sail_int)(zgsz31005);
    KILL(sail_int)(&zgsz31005);
    KILL(sail_int)(&zgsz31004);
    KILL(sail_int)(&zgsz31003);
  }
  zCAPABILITY_DBITS = zgsz3131;

let_end_230: ;
}
static void kill_letbind_69(void) {
}

static bool sailgen_CapUnsignedLessThanOrEqual(lbits, lbits);

sail_int zghz3361;
sail_int zghz3362;

static void startup_sailgen_CapUnsignedLessThanOrEqual(void)
{
  CREATE(sail_int)(&zghz3361);
  CREATE(sail_int)(&zghz3362);
}

static bool sailgen_CapUnsignedLessThanOrEqual(lbits za, lbits zb)
{
  __label__ cleanup_233, end_cleanup_234, end_function_232, end_block_exception_235;

  bool zcbz331;
  {
    RECREATE(sail_int)(&zghz3361);
    sail_unsigned(&zghz3361, za);
    RECREATE(sail_int)(&zghz3362);
    sail_unsigned(&zghz3362, zb);
    zcbz331 = lteq(zghz3361, zghz3362);
  }
  goto cleanup_233;
  /* unreachable after return */
  goto end_cleanup_234;
cleanup_233: ;
  goto end_function_232;
end_cleanup_234: ;
end_function_232: ;
  return zcbz331;
end_block_exception_235: ;

  return false;
}



static void finish_sailgen_CapUnsignedLessThanOrEqual(void)
{
  KILL(sail_int)(&zghz3362);
  KILL(sail_int)(&zghz3361);
}

static uint64_t sailgen_CapGetTag(lbits);

lbits zghz3363;
sail_int zghz3364;
lbits zghz3365;
sail_int zghz3366;

static void startup_sailgen_CapGetTag(void)
{
  CREATE(lbits)(&zghz3363);
  CREATE(sail_int)(&zghz3364);
  CREATE(lbits)(&zghz3365);
  CREATE(sail_int)(&zghz3366);
}

static uint64_t sailgen_CapGetTag(lbits zc)
{
  __label__ cleanup_238, end_cleanup_239, end_function_237, end_block_exception_240;

  uint64_t zcbz332;
  {
    uint64_t zgaz3219;
    {
      fbits zgaz3218;
      {
        RECREATE(sail_int)(&zghz3366);
        CONVERT_OF(sail_int, mach_int)(&zghz3366, zCAP_TAG_BIT);
        zgaz3218 = bitvector_access(zc, zghz3366);
      }
      zgaz3219 = UINT64_C(0b0);
      zgaz3219 = update_fbits(zgaz3219, INT64_C(0), zgaz3218);
    }
    {
      RECREATE(lbits)(&zghz3363);
      CONVERT_OF(lbits, fbits)(&zghz3363, zgaz3219, UINT64_C(1) , true);
      RECREATE(sail_int)(&zghz3364);
      CONVERT_OF(sail_int, mach_int)(&zghz3364, INT64_C(64));
      RECREATE(lbits)(&zghz3365);
      zero_extend(&zghz3365, zghz3363, zghz3364);
      zcbz332 = CONVERT_OF(fbits, lbits)(zghz3365, true);
    }
  }
  goto cleanup_238;
  /* unreachable after return */
  goto end_cleanup_239;
cleanup_238: ;
  goto end_function_237;
end_cleanup_239: ;
end_function_237: ;
  return zcbz332;
end_block_exception_240: ;

  return UINT64_C(0xdeadc0de);
}



static void finish_sailgen_CapGetTag(void)
{
  KILL(sail_int)(&zghz3366);
  KILL(lbits)(&zghz3365);
  KILL(sail_int)(&zghz3364);
  KILL(lbits)(&zghz3363);
}

static void sailgen_CapSetBounds(lbits *rop, lbits, lbits, bool);

lbits zghz3367;
lbits zghz3368;
sail_int zghz3369;
lbits zghz3370;
lbits zghz3371;
lbits zghz3372;
sail_int zghz3373;
lbits zghz3374;
sail_int zghz3375;
sail_int zghz3376;
lbits zghz3377;
sail_int zghz3378;
lbits zghz3379;
sail_int zghz3380;
sail_int zghz3381;
sail_int zghz3382;
sail_int zghz3383;
sail_int zghz3384;
sail_int zghz3385;
lbits zghz3386;
sail_int zghz3387;
sail_int zghz3388;
sail_int zghz3389;
sail_int zghz3390;
sail_int zghz3391;
lbits zghz3392;
sail_int zghz3393;
sail_int zghz3394;
sail_int zghz3395;
sail_int zghz3396;
sail_int zghz3397;
lbits zghz3398;
sail_int zghz3399;
sail_int zghz3400;
lbits zghz3401;
sail_int zghz3402;
sail_int zghz3403;
sail_int zghz3404;
lbits zghz3405;
sail_int zghz3406;
sail_int zghz3407;
lbits zghz3408;
sail_int zghz3409;
sail_int zghz3410;
lbits zghz3411;
sail_int zghz3412;
sail_int zghz3413;
lbits zghz3414;
sail_int zghz3415;
sail_int zghz3416;
lbits zghz3417;
sail_int zghz3418;
sail_int zghz3419;
lbits zghz3420;
sail_int zghz3421;
sail_int zghz3422;
lbits zghz3423;
lbits zghz3424;
lbits zghz3425;
sail_int zghz3426;
sail_int zghz3427;
lbits zghz3428;
sail_int zghz3429;
lbits zghz3430;
sail_int zghz3431;
sail_int zghz3432;
lbits zghz3433;
sail_int zghz3434;
sail_int zghz3435;
sail_int zghz3436;
sail_int zghz3437;
sail_int zghz3438;
sail_int zghz3439;
lbits zghz3440;
sail_int zghz3441;
sail_int zghz3442;
sail_int zghz3443;
sail_int zghz3444;
sail_int zghz3445;
sail_int zghz3446;
sail_int zghz3447;
sail_int zghz3448;
sail_int zghz3449;
sail_int zghz3450;
sail_int zghz3451;
sail_int zghz3452;
lbits zghz3453;
sail_int zghz3454;
lbits zghz3455;
lbits zghz3456;
lbits zghz3457;
sail_int zghz3458;
lbits zghz3459;
lbits zghz3460;
sail_int zghz3461;
sail_int zghz3462;
lbits zghz3463;
sail_int zghz3464;
sail_int zghz3465;
sail_int zghz3466;
sail_int zghz3467;
sail_int zghz3468;
sail_int zghz3469;
sail_int zghz3470;
sail_int zghz3471;
sail_int zghz3472;
sail_int zghz3473;
sail_int zghz3474;
sail_int zghz3475;
sail_int zghz3476;
sail_int zghz3477;
sail_int zghz3478;
lbits zghz3479;
sail_int zghz3480;
sail_int zghz3481;
sail_int zghz3482;
sail_int zghz3483;
sail_int zghz3484;
sail_int zghz3485;
sail_int zghz3486;
sail_int zghz3487;
sail_int zghz3488;
sail_int zghz3489;
sail_int zghz3490;
sail_int zghz3491;
sail_int zghz3492;
sail_int zghz3493;
sail_int zghz3494;
sail_int zghz3495;
sail_int zghz3496;
sail_int zghz3497;
sail_int zghz3498;
sail_int zghz3499;
lbits zghz3500;
sail_int zghz3501;
sail_int zghz3502;
sail_int zghz3503;
sail_int zghz3504;
sail_int zghz3505;
sail_int zghz3506;
sail_int zghz3507;
sail_int zghz3508;
sail_int zghz3509;
sail_int zghz3510;
sail_int zghz3511;
sail_int zghz3512;
sail_int zghz3513;
sail_int zghz3514;
sail_int zghz3515;
sail_int zghz3516;
sail_int zghz3517;
sail_int zghz3518;
sail_int zghz3519;
lbits zghz3520;
sail_int zghz3521;
sail_int zghz3522;
sail_int zghz3523;
sail_int zghz3524;
sail_int zghz3525;
lbits zghz3526;
sail_int zghz3527;
sail_int zghz3528;
sail_int zghz3529;
lbits zghz3530;
lbits zghz3531;
lbits zghz3532;
lbits zghz3533;
sail_int zghz3534;
sail_int zghz3535;
sail_int zghz3536;
sail_int zghz3537;
sail_int zghz3538;
sail_int zghz3539;
sail_int zghz3540;
sail_int zghz3541;
lbits zghz3542;
sail_int zghz3543;
sail_int zghz3544;
sail_int zghz3545;
lbits zghz3546;
sail_int zghz3547;
sail_int zghz3548;
sail_int zghz3549;
sail_int zghz3550;
sail_int zghz3551;
sail_int zghz3552;
sail_int zghz3553;
sail_int zghz3554;
sail_int zghz3555;
sail_int zghz3556;
sail_int zghz3557;
sail_int zghz3558;
lbits zghz3559;

static void startup_sailgen_CapSetBounds(void)
{
  CREATE(lbits)(&zghz3367);
  CREATE(lbits)(&zghz3368);
  CREATE(sail_int)(&zghz3369);
  CREATE(lbits)(&zghz3370);
  CREATE(lbits)(&zghz3371);
  CREATE(lbits)(&zghz3372);
  CREATE(sail_int)(&zghz3373);
  CREATE(lbits)(&zghz3374);
  CREATE(sail_int)(&zghz3375);
  CREATE(sail_int)(&zghz3376);
  CREATE(lbits)(&zghz3377);
  CREATE(sail_int)(&zghz3378);
  CREATE(lbits)(&zghz3379);
  CREATE(sail_int)(&zghz3380);
  CREATE(sail_int)(&zghz3381);
  CREATE(sail_int)(&zghz3382);
  CREATE(sail_int)(&zghz3383);
  CREATE(sail_int)(&zghz3384);
  CREATE(sail_int)(&zghz3385);
  CREATE(lbits)(&zghz3386);
  CREATE(sail_int)(&zghz3387);
  CREATE(sail_int)(&zghz3388);
  CREATE(sail_int)(&zghz3389);
  CREATE(sail_int)(&zghz3390);
  CREATE(sail_int)(&zghz3391);
  CREATE(lbits)(&zghz3392);
  CREATE(sail_int)(&zghz3393);
  CREATE(sail_int)(&zghz3394);
  CREATE(sail_int)(&zghz3395);
  CREATE(sail_int)(&zghz3396);
  CREATE(sail_int)(&zghz3397);
  CREATE(lbits)(&zghz3398);
  CREATE(sail_int)(&zghz3399);
  CREATE(sail_int)(&zghz3400);
  CREATE(lbits)(&zghz3401);
  CREATE(sail_int)(&zghz3402);
  CREATE(sail_int)(&zghz3403);
  CREATE(sail_int)(&zghz3404);
  CREATE(lbits)(&zghz3405);
  CREATE(sail_int)(&zghz3406);
  CREATE(sail_int)(&zghz3407);
  CREATE(lbits)(&zghz3408);
  CREATE(sail_int)(&zghz3409);
  CREATE(sail_int)(&zghz3410);
  CREATE(lbits)(&zghz3411);
  CREATE(sail_int)(&zghz3412);
  CREATE(sail_int)(&zghz3413);
  CREATE(lbits)(&zghz3414);
  CREATE(sail_int)(&zghz3415);
  CREATE(sail_int)(&zghz3416);
  CREATE(lbits)(&zghz3417);
  CREATE(sail_int)(&zghz3418);
  CREATE(sail_int)(&zghz3419);
  CREATE(lbits)(&zghz3420);
  CREATE(sail_int)(&zghz3421);
  CREATE(sail_int)(&zghz3422);
  CREATE(lbits)(&zghz3423);
  CREATE(lbits)(&zghz3424);
  CREATE(lbits)(&zghz3425);
  CREATE(sail_int)(&zghz3426);
  CREATE(sail_int)(&zghz3427);
  CREATE(lbits)(&zghz3428);
  CREATE(sail_int)(&zghz3429);
  CREATE(lbits)(&zghz3430);
  CREATE(sail_int)(&zghz3431);
  CREATE(sail_int)(&zghz3432);
  CREATE(lbits)(&zghz3433);
  CREATE(sail_int)(&zghz3434);
  CREATE(sail_int)(&zghz3435);
  CREATE(sail_int)(&zghz3436);
  CREATE(sail_int)(&zghz3437);
  CREATE(sail_int)(&zghz3438);
  CREATE(sail_int)(&zghz3439);
  CREATE(lbits)(&zghz3440);
  CREATE(sail_int)(&zghz3441);
  CREATE(sail_int)(&zghz3442);
  CREATE(sail_int)(&zghz3443);
  CREATE(sail_int)(&zghz3444);
  CREATE(sail_int)(&zghz3445);
  CREATE(sail_int)(&zghz3446);
  CREATE(sail_int)(&zghz3447);
  CREATE(sail_int)(&zghz3448);
  CREATE(sail_int)(&zghz3449);
  CREATE(sail_int)(&zghz3450);
  CREATE(sail_int)(&zghz3451);
  CREATE(sail_int)(&zghz3452);
  CREATE(lbits)(&zghz3453);
  CREATE(sail_int)(&zghz3454);
  CREATE(lbits)(&zghz3455);
  CREATE(lbits)(&zghz3456);
  CREATE(lbits)(&zghz3457);
  CREATE(sail_int)(&zghz3458);
  CREATE(lbits)(&zghz3459);
  CREATE(lbits)(&zghz3460);
  CREATE(sail_int)(&zghz3461);
  CREATE(sail_int)(&zghz3462);
  CREATE(lbits)(&zghz3463);
  CREATE(sail_int)(&zghz3464);
  CREATE(sail_int)(&zghz3465);
  CREATE(sail_int)(&zghz3466);
  CREATE(sail_int)(&zghz3467);
  CREATE(sail_int)(&zghz3468);
  CREATE(sail_int)(&zghz3469);
  CREATE(sail_int)(&zghz3470);
  CREATE(sail_int)(&zghz3471);
  CREATE(sail_int)(&zghz3472);
  CREATE(sail_int)(&zghz3473);
  CREATE(sail_int)(&zghz3474);
  CREATE(sail_int)(&zghz3475);
  CREATE(sail_int)(&zghz3476);
  CREATE(sail_int)(&zghz3477);
  CREATE(sail_int)(&zghz3478);
  CREATE(lbits)(&zghz3479);
  CREATE(sail_int)(&zghz3480);
  CREATE(sail_int)(&zghz3481);
  CREATE(sail_int)(&zghz3482);
  CREATE(sail_int)(&zghz3483);
  CREATE(sail_int)(&zghz3484);
  CREATE(sail_int)(&zghz3485);
  CREATE(sail_int)(&zghz3486);
  CREATE(sail_int)(&zghz3487);
  CREATE(sail_int)(&zghz3488);
  CREATE(sail_int)(&zghz3489);
  CREATE(sail_int)(&zghz3490);
  CREATE(sail_int)(&zghz3491);
  CREATE(sail_int)(&zghz3492);
  CREATE(sail_int)(&zghz3493);
  CREATE(sail_int)(&zghz3494);
  CREATE(sail_int)(&zghz3495);
  CREATE(sail_int)(&zghz3496);
  CREATE(sail_int)(&zghz3497);
  CREATE(sail_int)(&zghz3498);
  CREATE(sail_int)(&zghz3499);
  CREATE(lbits)(&zghz3500);
  CREATE(sail_int)(&zghz3501);
  CREATE(sail_int)(&zghz3502);
  CREATE(sail_int)(&zghz3503);
  CREATE(sail_int)(&zghz3504);
  CREATE(sail_int)(&zghz3505);
  CREATE(sail_int)(&zghz3506);
  CREATE(sail_int)(&zghz3507);
  CREATE(sail_int)(&zghz3508);
  CREATE(sail_int)(&zghz3509);
  CREATE(sail_int)(&zghz3510);
  CREATE(sail_int)(&zghz3511);
  CREATE(sail_int)(&zghz3512);
  CREATE(sail_int)(&zghz3513);
  CREATE(sail_int)(&zghz3514);
  CREATE(sail_int)(&zghz3515);
  CREATE(sail_int)(&zghz3516);
  CREATE(sail_int)(&zghz3517);
  CREATE(sail_int)(&zghz3518);
  CREATE(sail_int)(&zghz3519);
  CREATE(lbits)(&zghz3520);
  CREATE(sail_int)(&zghz3521);
  CREATE(sail_int)(&zghz3522);
  CREATE(sail_int)(&zghz3523);
  CREATE(sail_int)(&zghz3524);
  CREATE(sail_int)(&zghz3525);
  CREATE(lbits)(&zghz3526);
  CREATE(sail_int)(&zghz3527);
  CREATE(sail_int)(&zghz3528);
  CREATE(sail_int)(&zghz3529);
  CREATE(lbits)(&zghz3530);
  CREATE(lbits)(&zghz3531);
  CREATE(lbits)(&zghz3532);
  CREATE(lbits)(&zghz3533);
  CREATE(sail_int)(&zghz3534);
  CREATE(sail_int)(&zghz3535);
  CREATE(sail_int)(&zghz3536);
  CREATE(sail_int)(&zghz3537);
  CREATE(sail_int)(&zghz3538);
  CREATE(sail_int)(&zghz3539);
  CREATE(sail_int)(&zghz3540);
  CREATE(sail_int)(&zghz3541);
  CREATE(lbits)(&zghz3542);
  CREATE(sail_int)(&zghz3543);
  CREATE(sail_int)(&zghz3544);
  CREATE(sail_int)(&zghz3545);
  CREATE(lbits)(&zghz3546);
  CREATE(sail_int)(&zghz3547);
  CREATE(sail_int)(&zghz3548);
  CREATE(sail_int)(&zghz3549);
  CREATE(sail_int)(&zghz3550);
  CREATE(sail_int)(&zghz3551);
  CREATE(sail_int)(&zghz3552);
  CREATE(sail_int)(&zghz3553);
  CREATE(sail_int)(&zghz3554);
  CREATE(sail_int)(&zghz3555);
  CREATE(sail_int)(&zghz3556);
  CREATE(sail_int)(&zghz3557);
  CREATE(sail_int)(&zghz3558);
  CREATE(lbits)(&zghz3559);
}

static void sailgen_CapSetBounds(lbits *zcbz333, lbits zc, lbits zreq_len, bool zexact)
{
  __label__ case_251, finish_match_250, end_function_254, end_block_exception_257, end_function_265;

  uint64_t zL_ie;
  {
    RECREATE(sail_int)(&zghz3558);
    CONVERT_OF(sail_int, mach_int)(&zghz3558, INT64_C(13));
    RECREATE(lbits)(&zghz3559);
    UNDEFINED(lbits)(&zghz3559, zghz3558);
    zL_ie = CONVERT_OF(fbits, lbits)(zghz3559, true);
  }
  RECREATE(lbits)(&zghz3367);
  {
    int64_t zgaz3322;
    {
      int64_t zgaz3321;
      {
        int64_t zgaz3320;
        {
          RECREATE(sail_int)(&zghz3555);
          CONVERT_OF(sail_int, mach_int)(&zghz3555, INT64_C(63));
          RECREATE(sail_int)(&zghz3556);
          CONVERT_OF(sail_int, mach_int)(&zghz3556, INT64_C(0));
          RECREATE(sail_int)(&zghz3557);
          sub_int(&zghz3557, zghz3555, zghz3556);
          zgaz3320 = CONVERT_OF(mach_int, sail_int)(zghz3557);
        }
        zgaz3321 = (zgaz3320 + INT64_C(1));
      }
      zgaz3322 = (zgaz3321 + INT64_C(1));
    }
    {
      RECREATE(sail_int)(&zghz3554);
      CONVERT_OF(sail_int, mach_int)(&zghz3554, zgaz3322);
      UNDEFINED(lbits)(&zghz3367, zghz3554);
    }
  }
  RECREATE(lbits)(&zghz3368);
  {
    int64_t zgaz3319;
    {
      int64_t zgaz3318;
      {
        int64_t zgaz3317;
        {
          RECREATE(sail_int)(&zghz3551);
          CONVERT_OF(sail_int, mach_int)(&zghz3551, INT64_C(63));
          RECREATE(sail_int)(&zghz3552);
          CONVERT_OF(sail_int, mach_int)(&zghz3552, INT64_C(0));
          RECREATE(sail_int)(&zghz3553);
          sub_int(&zghz3553, zghz3551, zghz3552);
          zgaz3317 = CONVERT_OF(mach_int, sail_int)(zghz3553);
        }
        zgaz3318 = (zgaz3317 + INT64_C(1));
      }
      zgaz3319 = (zgaz3318 + INT64_C(1));
    }
    {
      RECREATE(sail_int)(&zghz3550);
      CONVERT_OF(sail_int, mach_int)(&zghz3550, zgaz3319);
      UNDEFINED(lbits)(&zghz3368, zghz3550);
    }
  }
  bool zovalid;
  zovalid = false;
  {
    bool zgaz3221;
    zgaz3221 = sailgen_CapUnsignedLessThanOrEqual(zreq_len, zCAP_BOUND_MAX);
    unit zgsz3133;
    zgsz3133 = sail_assert(zgaz3221, "v8_base.sail:45083.61-45083.62");
  }
  RECREATE(sail_int)(&zghz3369);
  {
    int64_t zgaz3316;
    {
      uint64_t zgaz3315;
      {
        int64_t zgaz3314;
        {
          RECREATE(sail_int)(&zghz3547);
          CONVERT_OF(sail_int, mach_int)(&zghz3547, zCAP_MW);
          RECREATE(sail_int)(&zghz3548);
          CONVERT_OF(sail_int, mach_int)(&zghz3548, INT64_C(1));
          RECREATE(sail_int)(&zghz3549);
          sub_int(&zghz3549, zghz3547, zghz3548);
          zgaz3314 = CONVERT_OF(mach_int, sail_int)(zghz3549);
        }
        {
          RECREATE(sail_int)(&zghz3544);
          CONVERT_OF(sail_int, mach_int)(&zghz3544, zCAP_VALUE_NUM_BITS);
          RECREATE(sail_int)(&zghz3545);
          CONVERT_OF(sail_int, mach_int)(&zghz3545, zgaz3314);
          RECREATE(lbits)(&zghz3546);
          vector_subrange_lbits(&zghz3546, zreq_len, zghz3544, zghz3545);
          zgaz3315 = CONVERT_OF(fbits, lbits)(zghz3546, true);
        }
      }
      {
        RECREATE(lbits)(&zghz3542);
        CONVERT_OF(lbits, fbits)(&zghz3542, zgaz3315, UINT64_C(50) , true);
        RECREATE(sail_int)(&zghz3543);
        count_leading_zeros(&zghz3543, zghz3542);
        zgaz3316 = CONVERT_OF(mach_int, sail_int)(zghz3543);
      }
    }
    {
      RECREATE(sail_int)(&zghz3540);
      CONVERT_OF(sail_int, mach_int)(&zghz3540, zCAP_MAX_EXPONENT);
      RECREATE(sail_int)(&zghz3541);
      CONVERT_OF(sail_int, mach_int)(&zghz3541, zgaz3316);
      sub_int(&zghz3369, zghz3540, zghz3541);
    }
  }
  bool zie;
  {
    bool zgaz3313;
    {
      RECREATE(sail_int)(&zghz3539);
      CONVERT_OF(sail_int, mach_int)(&zghz3539, INT64_C(0));
      zgaz3313 = sailgen_neq_int(zghz3369, zghz3539);
    }
    bool zgsz3135;
    if (zgaz3313) {  zgsz3135 = true;  } else {
      uint64_t zgaz3312;
      {
        fbits zgaz3311;
        {
          int64_t zgaz3310;
          {
            RECREATE(sail_int)(&zghz3536);
            CONVERT_OF(sail_int, mach_int)(&zghz3536, zCAP_MW);
            RECREATE(sail_int)(&zghz3537);
            CONVERT_OF(sail_int, mach_int)(&zghz3537, INT64_C(2));
            RECREATE(sail_int)(&zghz3538);
            sub_int(&zghz3538, zghz3536, zghz3537);
            zgaz3310 = CONVERT_OF(mach_int, sail_int)(zghz3538);
          }
          {
            RECREATE(sail_int)(&zghz3535);
            CONVERT_OF(sail_int, mach_int)(&zghz3535, zgaz3310);
            zgaz3311 = bitvector_access(zreq_len, zghz3535);
          }
        }
        zgaz3312 = UINT64_C(0b0);
        zgaz3312 = update_fbits(zgaz3312, INT64_C(0), zgaz3311);
      }
      zgsz3135 = (zgaz3312 == UINT64_C(0b1));
    }
    zie = zgsz3135;
  }
  uint64_t zbase;
  zbase = sailgen_CapGetValue(zc);
  uint64_t zabase;
  {
    bool zgaz3309;
    {
      int64_t zgaz3308;
      zgaz3308 = sailgen_CapGetExponent(zc);
      {
        RECREATE(sail_int)(&zghz3534);
        CONVERT_OF(sail_int, mach_int)(&zghz3534, zgaz3308);
        zgaz3309 = sailgen_CapBoundsUsesValue(zghz3534);
      }
    }
    if (zgaz3309) {  zabase = sailgen_CapBoundsAddress(zbase);  } else {  zabase = zbase;  }
  }
  RECREATE(lbits)(&zghz3370);
  {
    RECREATE(lbits)(&zghz3532);
    CONVERT_OF(lbits, fbits)(&zghz3532, UINT64_C(0b00), UINT64_C(2) , true);
    RECREATE(lbits)(&zghz3533);
    CONVERT_OF(lbits, fbits)(&zghz3533, zabase, UINT64_C(64) , true);
    append(&zghz3370, zghz3532, zghz3533);
  }
  RECREATE(lbits)(&zghz3371);
  {
    RECREATE(lbits)(&zghz3530);
    {
      RECREATE(lbits)(&zghz3531);
      CONVERT_OF(lbits, fbits)(&zghz3531, UINT64_C(0b0), UINT64_C(1) , true);
      append(&zghz3530, zghz3531, zreq_len);
    }
    add_bits(&zghz3371, zghz3370, zghz3530);
  }
  uint64_t zBbits;
  {
    int64_t zgaz3306;
    {
      RECREATE(sail_int)(&zghz3527);
      CONVERT_OF(sail_int, mach_int)(&zghz3527, zCAP_MW);
      RECREATE(sail_int)(&zghz3528);
      CONVERT_OF(sail_int, mach_int)(&zghz3528, INT64_C(1));
      RECREATE(sail_int)(&zghz3529);
      sub_int(&zghz3529, zghz3527, zghz3528);
      zgaz3306 = CONVERT_OF(mach_int, sail_int)(zghz3529);
    }
    {
      RECREATE(sail_int)(&zghz3524);
      CONVERT_OF(sail_int, mach_int)(&zghz3524, zgaz3306);
      RECREATE(sail_int)(&zghz3525);
      CONVERT_OF(sail_int, mach_int)(&zghz3525, INT64_C(0));
      RECREATE(lbits)(&zghz3526);
      vector_subrange_lbits(&zghz3526, zghz3370, zghz3524, zghz3525);
      zBbits = CONVERT_OF(fbits, lbits)(zghz3526, true);
    }
  }
  uint64_t zTBits;
  {
    int64_t zgaz3305;
    {
      RECREATE(sail_int)(&zghz3521);
      CONVERT_OF(sail_int, mach_int)(&zghz3521, zCAP_MW);
      RECREATE(sail_int)(&zghz3522);
      CONVERT_OF(sail_int, mach_int)(&zghz3522, INT64_C(1));
      RECREATE(sail_int)(&zghz3523);
      sub_int(&zghz3523, zghz3521, zghz3522);
      zgaz3305 = CONVERT_OF(mach_int, sail_int)(zghz3523);
    }
    {
      RECREATE(sail_int)(&zghz3518);
      CONVERT_OF(sail_int, mach_int)(&zghz3518, zgaz3305);
      RECREATE(sail_int)(&zghz3519);
      CONVERT_OF(sail_int, mach_int)(&zghz3519, INT64_C(0));
      RECREATE(lbits)(&zghz3520);
      vector_subrange_lbits(&zghz3520, zghz3371, zghz3518, zghz3519);
      zTBits = CONVERT_OF(fbits, lbits)(zghz3520, true);
    }
  }
  bool zlostTop;
  zlostTop = false;
  bool zlostBottom;
  zlostBottom = false;
  bool zincrementE_name;
  zincrementE_name = false;
  {
    unit zgsz3136;
    if (zie) {
      uint64_t zB_ie;
      {
        __label__ case_243, finish_match_242;

        {
          RECREATE(sail_int)(&zghz3497);
          COPY(sail_int)(&zghz3497, zghz3369);
          {
            bool zgaz3270;
            {
              bool zgaz3269;
              {
                RECREATE(sail_int)(&zghz3514);
                {
                  RECREATE(sail_int)(&zghz3516);
                  sailgen___id(&zghz3516, zghz3497);
                  {
                    RECREATE(sail_int)(&zghz3517);
                    CONVERT_OF(sail_int, mach_int)(&zghz3517, INT64_C(3));
                    add_int(&zghz3514, zghz3516, zghz3517);
                  }
                }
                {
                  RECREATE(sail_int)(&zghz3515);
                  CONVERT_OF(sail_int, mach_int)(&zghz3515, INT64_C(0));
                  zgaz3269 = lteq(zghz3515, zghz3514);
                }
              }
              bool zgsz3138;
              if (zgaz3269) {
                RECREATE(sail_int)(&zghz3508);
                {
                  RECREATE(sail_int)(&zghz3510);
                  {
                    RECREATE(sail_int)(&zghz3512);
                    sailgen___id(&zghz3512, zghz3497);
                    {
                      RECREATE(sail_int)(&zghz3513);
                      CONVERT_OF(sail_int, mach_int)(&zghz3513, INT64_C(16));
                      add_int(&zghz3510, zghz3512, zghz3513);
                    }
                  }
                  {
                    RECREATE(sail_int)(&zghz3511);
                    CONVERT_OF(sail_int, mach_int)(&zghz3511, INT64_C(1));
                    sub_int(&zghz3508, zghz3510, zghz3511);
                  }
                }
                {
                  RECREATE(sail_int)(&zghz3509);
                  CONVERT_OF(sail_int, mach_int)(&zghz3509, INT64_C(66));
                  zgsz3138 = lt(zghz3508, zghz3509);
                }
              } else {  zgsz3138 = false;  }
              zgaz3270 = zgsz3138;
            }
            unit zgsz3139;
            zgsz3139 = sail_assert(zgaz3270, "v8_base.sail:45098.69-45098.70");
          }
          int64_t zgaz3272;
          {
            int64_t zgaz3271;
            {
              RECREATE(sail_int)(&zghz3506);
              CONVERT_OF(sail_int, mach_int)(&zghz3506, zCAP_MW);
              RECREATE(sail_int)(&zghz3507);
              add_int(&zghz3507, zghz3497, zghz3506);
              zgaz3271 = CONVERT_OF(mach_int, sail_int)(zghz3507);
            }
            {
              RECREATE(sail_int)(&zghz3503);
              CONVERT_OF(sail_int, mach_int)(&zghz3503, zgaz3271);
              RECREATE(sail_int)(&zghz3504);
              CONVERT_OF(sail_int, mach_int)(&zghz3504, INT64_C(1));
              RECREATE(sail_int)(&zghz3505);
              sub_int(&zghz3505, zghz3503, zghz3504);
              zgaz3272 = CONVERT_OF(mach_int, sail_int)(zghz3505);
            }
          }
          int64_t zgaz3273;
          {
            RECREATE(sail_int)(&zghz3501);
            CONVERT_OF(sail_int, mach_int)(&zghz3501, INT64_C(3));
            RECREATE(sail_int)(&zghz3502);
            add_int(&zghz3502, zghz3497, zghz3501);
            zgaz3273 = CONVERT_OF(mach_int, sail_int)(zghz3502);
          }
          {
            RECREATE(sail_int)(&zghz3498);
            CONVERT_OF(sail_int, mach_int)(&zghz3498, zgaz3272);
            RECREATE(sail_int)(&zghz3499);
            CONVERT_OF(sail_int, mach_int)(&zghz3499, zgaz3273);
            RECREATE(lbits)(&zghz3500);
            vector_subrange_lbits(&zghz3500, zghz3370, zghz3498, zghz3499);
            zB_ie = CONVERT_OF(fbits, lbits)(zghz3500, true);
          }
          goto finish_match_242;
        }
      case_243: ;
        sail_match_failure("CapSetBounds");
      finish_match_242: ;
      }
      uint64_t zT_ie;
      {
        __label__ case_245, finish_match_244;

        {
          RECREATE(sail_int)(&zghz3476);
          COPY(sail_int)(&zghz3476, zghz3369);
          {
            bool zgaz3260;
            {
              bool zgaz3259;
              {
                RECREATE(sail_int)(&zghz3493);
                {
                  RECREATE(sail_int)(&zghz3495);
                  sailgen___id(&zghz3495, zghz3476);
                  {
                    RECREATE(sail_int)(&zghz3496);
                    CONVERT_OF(sail_int, mach_int)(&zghz3496, INT64_C(3));
                    add_int(&zghz3493, zghz3495, zghz3496);
                  }
                }
                {
                  RECREATE(sail_int)(&zghz3494);
                  CONVERT_OF(sail_int, mach_int)(&zghz3494, INT64_C(0));
                  zgaz3259 = lteq(zghz3494, zghz3493);
                }
              }
              bool zgsz3142;
              if (zgaz3259) {
                RECREATE(sail_int)(&zghz3487);
                {
                  RECREATE(sail_int)(&zghz3489);
                  {
                    RECREATE(sail_int)(&zghz3491);
                    sailgen___id(&zghz3491, zghz3476);
                    {
                      RECREATE(sail_int)(&zghz3492);
                      CONVERT_OF(sail_int, mach_int)(&zghz3492, INT64_C(16));
                      add_int(&zghz3489, zghz3491, zghz3492);
                    }
                  }
                  {
                    RECREATE(sail_int)(&zghz3490);
                    CONVERT_OF(sail_int, mach_int)(&zghz3490, INT64_C(1));
                    sub_int(&zghz3487, zghz3489, zghz3490);
                  }
                }
                {
                  RECREATE(sail_int)(&zghz3488);
                  CONVERT_OF(sail_int, mach_int)(&zghz3488, INT64_C(66));
                  zgsz3142 = lt(zghz3487, zghz3488);
                }
              } else {  zgsz3142 = false;  }
              zgaz3260 = zgsz3142;
            }
            unit zgsz3143;
            zgsz3143 = sail_assert(zgaz3260, "v8_base.sail:45103.69-45103.70");
          }
          int64_t zgaz3262;
          {
            int64_t zgaz3261;
            {
              RECREATE(sail_int)(&zghz3485);
              CONVERT_OF(sail_int, mach_int)(&zghz3485, zCAP_MW);
              RECREATE(sail_int)(&zghz3486);
              add_int(&zghz3486, zghz3476, zghz3485);
              zgaz3261 = CONVERT_OF(mach_int, sail_int)(zghz3486);
            }
            {
              RECREATE(sail_int)(&zghz3482);
              CONVERT_OF(sail_int, mach_int)(&zghz3482, zgaz3261);
              RECREATE(sail_int)(&zghz3483);
              CONVERT_OF(sail_int, mach_int)(&zghz3483, INT64_C(1));
              RECREATE(sail_int)(&zghz3484);
              sub_int(&zghz3484, zghz3482, zghz3483);
              zgaz3262 = CONVERT_OF(mach_int, sail_int)(zghz3484);
            }
          }
          int64_t zgaz3263;
          {
            RECREATE(sail_int)(&zghz3480);
            CONVERT_OF(sail_int, mach_int)(&zghz3480, INT64_C(3));
            RECREATE(sail_int)(&zghz3481);
            add_int(&zghz3481, zghz3476, zghz3480);
            zgaz3263 = CONVERT_OF(mach_int, sail_int)(zghz3481);
          }
          {
            RECREATE(sail_int)(&zghz3477);
            CONVERT_OF(sail_int, mach_int)(&zghz3477, zgaz3262);
            RECREATE(sail_int)(&zghz3478);
            CONVERT_OF(sail_int, mach_int)(&zghz3478, zgaz3263);
            RECREATE(lbits)(&zghz3479);
            vector_subrange_lbits(&zghz3479, zghz3371, zghz3477, zghz3478);
            zT_ie = CONVERT_OF(fbits, lbits)(zghz3479, true);
          }
          goto finish_match_244;
        }
      case_245: ;
        sail_match_failure("CapSetBounds");
      finish_match_244: ;
      }
      RECREATE(lbits)(&zghz3425);
      {
        __label__ case_247, finish_match_246;

        {
          RECREATE(sail_int)(&zghz3462);
          COPY(sail_int)(&zghz3462, zghz3369);
          {
            bool zgaz3247;
            {
              RECREATE(sail_int)(&zghz3472);
              {
                RECREATE(sail_int)(&zghz3474);
                sailgen___id(&zghz3474, zghz3462);
                {
                  RECREATE(sail_int)(&zghz3475);
                  CONVERT_OF(sail_int, mach_int)(&zghz3475, INT64_C(3));
                  add_int(&zghz3472, zghz3474, zghz3475);
                }
              }
              {
                RECREATE(sail_int)(&zghz3473);
                CONVERT_OF(sail_int, mach_int)(&zghz3473, INT64_C(0));
                zgaz3247 = gteq(zghz3472, zghz3473);
              }
            }
            unit zgsz3147;
            zgsz3147 = sail_assert(zgaz3247, "v8_base.sail:45108.46-45108.47");
          }
          {
            bool zgaz3250;
            {
              RECREATE(sail_int)(&zghz3468);
              {
                RECREATE(sail_int)(&zghz3470);
                sailgen___id(&zghz3470, zghz3462);
                {
                  RECREATE(sail_int)(&zghz3471);
                  CONVERT_OF(sail_int, mach_int)(&zghz3471, INT64_C(3));
                  add_int(&zghz3468, zghz3470, zghz3471);
                }
              }
              {
                RECREATE(sail_int)(&zghz3469);
                CONVERT_OF(sail_int, mach_int)(&zghz3469, INT64_C(66));
                zgaz3250 = gteq(zghz3469, zghz3468);
              }
            }
            unit zgsz3146;
            zgsz3146 = sail_assert(zgaz3250, "v8_base.sail:45109.47-45109.48");
          }
          RECREATE(lbits)(&zghz3463);
          {
            int64_t zgaz3251;
            {
              RECREATE(sail_int)(&zghz3466);
              CONVERT_OF(sail_int, mach_int)(&zghz3466, INT64_C(3));
              RECREATE(sail_int)(&zghz3467);
              add_int(&zghz3467, zghz3462, zghz3466);
              zgaz3251 = CONVERT_OF(mach_int, sail_int)(zghz3467);
            }
            {
              RECREATE(sail_int)(&zghz3465);
              CONVERT_OF(sail_int, mach_int)(&zghz3465, zgaz3251);
              sailgen_Ones(&zghz3463, zghz3465);
            }
          }
          int64_t zgaz3253;
          zgaz3253 = (zCAP_VALUE_NUM_BITS + INT64_C(2));
          {
            RECREATE(sail_int)(&zghz3464);
            CONVERT_OF(sail_int, mach_int)(&zghz3464, zgaz3253);
            zero_extend(&zghz3425, zghz3463, zghz3464);
          }
          goto finish_match_246;
        }
      case_247: ;
        sail_match_failure("CapSetBounds");
      finish_match_246: ;
      }
      {
        RECREATE(lbits)(&zghz3459);
        and_bits(&zghz3459, zghz3370, zghz3425);
        RECREATE(lbits)(&zghz3460);
        {
          int64_t zgaz3222;
          zgaz3222 = (zCAP_VALUE_NUM_BITS + INT64_C(2));
          {
            RECREATE(sail_int)(&zghz3461);
            CONVERT_OF(sail_int, mach_int)(&zghz3461, zgaz3222);
            sailgen_Zeros(&zghz3460, zghz3461);
          }
        }
        zlostBottom = neq_bits(zghz3459, zghz3460);
        unit zgsz3151;
        zgsz3151 = UNIT;
      }
      {
        RECREATE(lbits)(&zghz3456);
        and_bits(&zghz3456, zghz3371, zghz3425);
        RECREATE(lbits)(&zghz3457);
        {
          int64_t zgaz3225;
          zgaz3225 = (zCAP_VALUE_NUM_BITS + INT64_C(2));
          {
            RECREATE(sail_int)(&zghz3458);
            CONVERT_OF(sail_int, mach_int)(&zghz3458, zgaz3225);
            sailgen_Zeros(&zghz3457, zghz3458);
          }
        }
        zlostTop = neq_bits(zghz3456, zghz3457);
        unit zgsz3150;
        zgsz3150 = UNIT;
      }
      {
        unit zgsz3149;
        if (zlostTop) {
          {
            RECREATE(lbits)(&zghz3453);
            CONVERT_OF(lbits, fbits)(&zghz3453, zT_ie, UINT64_C(13) , true);
            RECREATE(sail_int)(&zghz3454);
            CONVERT_OF(sail_int, mach_int)(&zghz3454, INT64_C(1));
            RECREATE(lbits)(&zghz3455);
            add_bits_int(&zghz3455, zghz3453, zghz3454);
            zT_ie = CONVERT_OF(fbits, lbits)(zghz3455, true);
          }
          zgsz3149 = UNIT;
        } else {  zgsz3149 = UNIT;  }
      }
      uint64_t zL_ieshadowz314;
      zL_ieshadowz314 = ((zT_ie - zB_ie) & UINT64_C(0b1111111111111));
      {
        __label__ case_249, finish_match_248;

        unit zgsz3152;
        {
          RECREATE(sail_int)(&zghz3427);
          COPY(sail_int)(&zghz3427, zghz3369);
          bool zgaz3231;
          {
            uint64_t zgaz3230;
            {
              fbits zgaz3229;
              {
                int64_t zgaz3228;
                {
                  RECREATE(sail_int)(&zghz3450);
                  CONVERT_OF(sail_int, mach_int)(&zghz3450, zCAP_MW);
                  RECREATE(sail_int)(&zghz3451);
                  CONVERT_OF(sail_int, mach_int)(&zghz3451, INT64_C(4));
                  RECREATE(sail_int)(&zghz3452);
                  sub_int(&zghz3452, zghz3450, zghz3451);
                  zgaz3228 = CONVERT_OF(mach_int, sail_int)(zghz3452);
                }
                zgaz3229 = (UINT64_C(1) & (zL_ieshadowz314 >> zgaz3228));
              }
              zgaz3230 = UINT64_C(0b0);
              zgaz3230 = update_fbits(zgaz3230, INT64_C(0), zgaz3229);
            }
            zgaz3231 = (zgaz3230 == UINT64_C(0b1));
          }
          if (zgaz3231) {
            {
              zincrementE_name = true;
              unit zgsz3165;
              zgsz3165 = UNIT;
            }
            {
              bool zgsz3156;
              if (zlostBottom) {  zgsz3156 = true;  } else {
                uint64_t zgaz3233;
                {
                  fbits zgaz3232;
                  zgaz3232 = (UINT64_C(1) & (zB_ie >> INT64_C(0)));
                  zgaz3233 = UINT64_C(0b0);
                  zgaz3233 = update_fbits(zgaz3233, INT64_C(0), zgaz3232);
                }
                zgsz3156 = (zgaz3233 == UINT64_C(0b1));
              }
              zlostBottom = zgsz3156;
              unit zgsz3164;
              zgsz3164 = UNIT;
            }
            {
              bool zgsz3158;
              if (zlostTop) {  zgsz3158 = true;  } else {
                uint64_t zgaz3235;
                {
                  fbits zgaz3234;
                  zgaz3234 = (UINT64_C(1) & (zT_ie >> INT64_C(0)));
                  zgaz3235 = UINT64_C(0b0);
                  zgaz3235 = update_fbits(zgaz3235, INT64_C(0), zgaz3234);
                }
                zgsz3158 = (zgaz3235 == UINT64_C(0b1));
              }
              zlostTop = zgsz3158;
              unit zgsz3163;
              zgsz3163 = UNIT;
            }
            {
              bool zgaz3236;
              {
                RECREATE(sail_int)(&zghz3449);
                CONVERT_OF(sail_int, mach_int)(&zghz3449, zCAP_MAX_EXPONENT);
                zgaz3236 = lt(zghz3427, zghz3449);
              }
              unit zgsz3162;
              zgsz3162 = sail_assert(zgaz3236, "v8_base.sail:45124.47-45124.48");
            }
            {
              bool zgaz3239;
              {
                RECREATE(sail_int)(&zghz3445);
                {
                  RECREATE(sail_int)(&zghz3447);
                  sailgen___id(&zghz3447, zghz3427);
                  {
                    RECREATE(sail_int)(&zghz3448);
                    CONVERT_OF(sail_int, mach_int)(&zghz3448, INT64_C(4));
                    add_int(&zghz3445, zghz3447, zghz3448);
                  }
                }
                {
                  RECREATE(sail_int)(&zghz3446);
                  CONVERT_OF(sail_int, mach_int)(&zghz3446, INT64_C(0));
                  zgaz3239 = lteq(zghz3446, zghz3445);
                }
              }
              unit zgsz3161;
              zgsz3161 = sail_assert(zgaz3239, "v8_base.sail:45125.50-45125.51");
            }
            {
              int64_t zgaz3240;
              {
                RECREATE(sail_int)(&zghz3443);
                CONVERT_OF(sail_int, mach_int)(&zghz3443, zCAP_MW);
                RECREATE(sail_int)(&zghz3444);
                add_int(&zghz3444, zghz3427, zghz3443);
                zgaz3240 = CONVERT_OF(mach_int, sail_int)(zghz3444);
              }
              int64_t zgaz3241;
              {
                RECREATE(sail_int)(&zghz3441);
                CONVERT_OF(sail_int, mach_int)(&zghz3441, INT64_C(4));
                RECREATE(sail_int)(&zghz3442);
                add_int(&zghz3442, zghz3427, zghz3441);
                zgaz3241 = CONVERT_OF(mach_int, sail_int)(zghz3442);
              }
              {
                RECREATE(sail_int)(&zghz3438);
                CONVERT_OF(sail_int, mach_int)(&zghz3438, zgaz3240);
                RECREATE(sail_int)(&zghz3439);
                CONVERT_OF(sail_int, mach_int)(&zghz3439, zgaz3241);
                RECREATE(lbits)(&zghz3440);
                vector_subrange_lbits(&zghz3440, zghz3370, zghz3438, zghz3439);
                zB_ie = CONVERT_OF(fbits, lbits)(zghz3440, true);
              }
              unit zgsz3160;
              zgsz3160 = UNIT;
            }
            {
              int64_t zgaz3242;
              {
                RECREATE(sail_int)(&zghz3436);
                CONVERT_OF(sail_int, mach_int)(&zghz3436, zCAP_MW);
                RECREATE(sail_int)(&zghz3437);
                add_int(&zghz3437, zghz3427, zghz3436);
                zgaz3242 = CONVERT_OF(mach_int, sail_int)(zghz3437);
              }
              int64_t zgaz3243;
              {
                RECREATE(sail_int)(&zghz3434);
                CONVERT_OF(sail_int, mach_int)(&zghz3434, INT64_C(4));
                RECREATE(sail_int)(&zghz3435);
                add_int(&zghz3435, zghz3427, zghz3434);
                zgaz3243 = CONVERT_OF(mach_int, sail_int)(zghz3435);
              }
              {
                RECREATE(sail_int)(&zghz3431);
                CONVERT_OF(sail_int, mach_int)(&zghz3431, zgaz3242);
                RECREATE(sail_int)(&zghz3432);
                CONVERT_OF(sail_int, mach_int)(&zghz3432, zgaz3243);
                RECREATE(lbits)(&zghz3433);
                vector_subrange_lbits(&zghz3433, zghz3371, zghz3431, zghz3432);
                zT_ie = CONVERT_OF(fbits, lbits)(zghz3433, true);
              }
              unit zgsz3159;
              zgsz3159 = UNIT;
            }
            if (zlostTop) {
              {
                RECREATE(lbits)(&zghz3428);
                CONVERT_OF(lbits, fbits)(&zghz3428, zT_ie, UINT64_C(13) , true);
                RECREATE(sail_int)(&zghz3429);
                CONVERT_OF(sail_int, mach_int)(&zghz3429, INT64_C(1));
                RECREATE(lbits)(&zghz3430);
                add_bits_int(&zghz3430, zghz3428, zghz3429);
                zT_ie = CONVERT_OF(fbits, lbits)(zghz3430, true);
              }
              zgsz3152 = UNIT;
            } else {  zgsz3152 = UNIT;  }
          } else {  zgsz3152 = UNIT;  }
          goto finish_match_248;
        }
      case_249: ;
        sail_match_failure("CapSetBounds");
      finish_match_248: ;
        unit zgsz3168;
        zgsz3168 = zgsz3152;
      }
      {
        bool zgaz3244;
        zgaz3244 = eq_bool(zincrementE_name, true);
        unit zgsz3167;
        if (zgaz3244) {
          {
            RECREATE(sail_int)(&zghz3426);
            CONVERT_OF(sail_int, mach_int)(&zghz3426, INT64_C(1));
            add_int(&zghz3369, zghz3369, zghz3426);
          }
          zgsz3167 = UNIT;
        } else {  zgsz3167 = UNIT;  }
      }
      {
        zBbits = (zB_ie << 3) | UINT64_C(0b000);
        unit zgsz3166;
        zgsz3166 = UNIT;
      }
      zTBits = (zT_ie << 3) | UINT64_C(0b000);
      zgsz3136 = UNIT;
    } else {  zgsz3136 = UNIT;  }
  }
  RECREATE(lbits)(&zghz3372);
  {
    __label__ cleanup_255, end_cleanup_256;

    RECREATE(sail_int)(&zghz3373);
    COPY(sail_int)(&zghz3373, zghz3369);
    RECREATE(lbits)(&zghz3374);
    COPY(lbits)(&zghz3374, zc);
    {
      __label__ case_253, finish_match_252;

      struct ztuple_z8z5bvzCz0z5bvzCz0z5boolz9 zgaz3274;
      CREATE(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgaz3274);
      sailgen_CapGetBounds(&zgaz3274, zc);
      unit zgsz3170;
      {
        RECREATE(lbits)(&zghz3423);
        COPY(lbits)(&zghz3423, zgaz3274.ztup0);
        RECREATE(lbits)(&zghz3424);
        COPY(lbits)(&zghz3424, zgaz3274.ztup1);
        bool ztup__2;
        ztup__2 = zgaz3274.ztup2;
        {
          COPY(lbits)(&zghz3367, zghz3423);
          unit zgsz3172;
          zgsz3172 = UNIT;
        }
        {
          COPY(lbits)(&zghz3368, zghz3424);
          unit zgsz3171;
          zgsz3171 = UNIT;
        }
        zovalid = ztup__2;
        zgsz3170 = UNIT;
        goto finish_match_252;
      }
    case_253: ;
      sail_match_failure("CapSetBounds");
    finish_match_252: ;
      unit zgsz3184;
      zgsz3184 = zgsz3170;
      KILL(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgaz3274);
    }
    {
      bool zgaz3281;
      {
        bool zgaz3280;
        {
          bool zgaz3279;
          {
            bool zgaz3276;
            {
              RECREATE(lbits)(&zghz3420);
              {
                RECREATE(sail_int)(&zghz3421);
                CONVERT_OF(sail_int, mach_int)(&zghz3421, INT64_C(0));
                RECREATE(sail_int)(&zghz3422);
                CONVERT_OF(sail_int, mach_int)(&zghz3422, zCAP_BOUND_NUM_BITS);
                slice(&zghz3420, zghz3370, zghz3421, zghz3422);
              }
              zgaz3276 = sailgen_CapUnsignedGreaterThanOrEqual(zghz3420, zghz3367);
            }
            zgaz3279 = not(zgaz3276);
          }
          bool zgsz3174;
          if (zgaz3279) {  zgsz3174 = true;  } else {
            bool zgaz3278;
            {
              RECREATE(lbits)(&zghz3417);
              {
                RECREATE(sail_int)(&zghz3418);
                CONVERT_OF(sail_int, mach_int)(&zghz3418, INT64_C(0));
                RECREATE(sail_int)(&zghz3419);
                CONVERT_OF(sail_int, mach_int)(&zghz3419, zCAP_BOUND_NUM_BITS);
                slice(&zghz3417, zghz3371, zghz3418, zghz3419);
              }
              zgaz3278 = sailgen_CapUnsignedLessThanOrEqual(zghz3417, zghz3368);
            }
            zgsz3174 = not(zgaz3278);
          }
          zgaz3280 = zgsz3174;
        }
        bool zgsz3175;
        if (zgaz3280) {  zgsz3175 = true;  } else {  zgsz3175 = not(zovalid);  }
        zgaz3281 = zgsz3175;
      }
      unit zgsz3183;
      if (zgaz3281) {
        fbits zgaz3282;
        zgaz3282 = sailgen_Bit(UINT64_C(0b0));
        {
          RECREATE(sail_int)(&zghz3416);
          CONVERT_OF(sail_int, mach_int)(&zghz3416, zCAP_TAG_BIT);
          update_lbits(&zghz3374, zghz3374, zghz3416, zgaz3282);
        }
        zgsz3183 = UNIT;
      } else {  zgsz3183 = UNIT;  }
    }
    {
      unit zgsz3178;
      if (zie) {
        {
          fbits zgaz3283;
          zgaz3283 = sailgen_Bit(UINT64_C(0b0));
          {
            RECREATE(sail_int)(&zghz3415);
            CONVERT_OF(sail_int, mach_int)(&zghz3415, zCAP_IE_BIT);
            update_lbits(&zghz3374, zghz3374, zghz3415, zgaz3283);
          }
          unit zgsz3182;
          zgsz3182 = UNIT;
        }
        {
          uint64_t zgaz3285;
          {
            uint64_t zgaz3284;
            {
              RECREATE(sail_int)(&zghz3412);
              CONVERT_OF(sail_int, mach_int)(&zghz3412, INT64_C(2));
              RECREATE(sail_int)(&zghz3413);
              CONVERT_OF(sail_int, mach_int)(&zghz3413, INT64_C(0));
              RECREATE(lbits)(&zghz3414);
              sailgen_integer_subrange(&zghz3414, zghz3373, zghz3412, zghz3413);
              zgaz3284 = CONVERT_OF(fbits, lbits)(zghz3414, true);
            }
            zgaz3285 = (~(zgaz3284) & UINT64_C(0b111));
          }
          {
            RECREATE(sail_int)(&zghz3409);
            CONVERT_OF(sail_int, mach_int)(&zghz3409, zCAP_BASE_EXP_HI_BIT);
            RECREATE(sail_int)(&zghz3410);
            CONVERT_OF(sail_int, mach_int)(&zghz3410, zCAP_BASE_LO_BIT);
            RECREATE(lbits)(&zghz3411);
            CONVERT_OF(lbits, fbits)(&zghz3411, zgaz3285, UINT64_C(3) , true);
            vector_update_subrange_lbits(&zghz3374, zghz3374, zghz3409, zghz3410, zghz3411);
          }
          unit zgsz3181;
          zgsz3181 = UNIT;
        }
        uint64_t zgaz3287;
        {
          uint64_t zgaz3286;
          {
            RECREATE(sail_int)(&zghz3406);
            CONVERT_OF(sail_int, mach_int)(&zghz3406, INT64_C(5));
            RECREATE(sail_int)(&zghz3407);
            CONVERT_OF(sail_int, mach_int)(&zghz3407, INT64_C(3));
            RECREATE(lbits)(&zghz3408);
            sailgen_integer_subrange(&zghz3408, zghz3373, zghz3406, zghz3407);
            zgaz3286 = CONVERT_OF(fbits, lbits)(zghz3408, true);
          }
          zgaz3287 = (~(zgaz3286) & UINT64_C(0b111));
        }
        {
          RECREATE(sail_int)(&zghz3403);
          CONVERT_OF(sail_int, mach_int)(&zghz3403, zCAP_LIMIT_EXP_HI_BIT);
          RECREATE(sail_int)(&zghz3404);
          CONVERT_OF(sail_int, mach_int)(&zghz3404, zCAP_LIMIT_LO_BIT);
          RECREATE(lbits)(&zghz3405);
          CONVERT_OF(lbits, fbits)(&zghz3405, zgaz3287, UINT64_C(3) , true);
          vector_update_subrange_lbits(&zghz3374, zghz3374, zghz3403, zghz3404, zghz3405);
        }
        zgsz3178 = UNIT;
      } else {
        {
          fbits zgaz3288;
          zgaz3288 = sailgen_Bit(UINT64_C(0b1));
          {
            RECREATE(sail_int)(&zghz3402);
            CONVERT_OF(sail_int, mach_int)(&zghz3402, zCAP_IE_BIT);
            update_lbits(&zghz3374, zghz3374, zghz3402, zgaz3288);
          }
          unit zgsz3180;
          zgsz3180 = UNIT;
        }
        {
          uint64_t zgaz3289;
          zgaz3289 = (safe_rshift(UINT64_MAX, 64 - 3) & (zBbits >> INT64_C(0)));
          {
            RECREATE(sail_int)(&zghz3399);
            CONVERT_OF(sail_int, mach_int)(&zghz3399, zCAP_BASE_EXP_HI_BIT);
            RECREATE(sail_int)(&zghz3400);
            CONVERT_OF(sail_int, mach_int)(&zghz3400, zCAP_BASE_LO_BIT);
            RECREATE(lbits)(&zghz3401);
            CONVERT_OF(lbits, fbits)(&zghz3401, zgaz3289, UINT64_C(3) , true);
            vector_update_subrange_lbits(&zghz3374, zghz3374, zghz3399, zghz3400, zghz3401);
          }
          unit zgsz3179;
          zgsz3179 = UNIT;
        }
        uint64_t zgaz3290;
        zgaz3290 = (safe_rshift(UINT64_MAX, 64 - 3) & (zTBits >> INT64_C(0)));
        {
          RECREATE(sail_int)(&zghz3396);
          CONVERT_OF(sail_int, mach_int)(&zghz3396, zCAP_LIMIT_EXP_HI_BIT);
          RECREATE(sail_int)(&zghz3397);
          CONVERT_OF(sail_int, mach_int)(&zghz3397, zCAP_LIMIT_LO_BIT);
          RECREATE(lbits)(&zghz3398);
          CONVERT_OF(lbits, fbits)(&zghz3398, zgaz3290, UINT64_C(3) , true);
          vector_update_subrange_lbits(&zghz3374, zghz3374, zghz3396, zghz3397, zghz3398);
        }
        zgsz3178 = UNIT;
      }
    }
    {
      uint64_t zgaz3292;
      {
        int64_t zgaz3291;
        {
          RECREATE(sail_int)(&zghz3393);
          CONVERT_OF(sail_int, mach_int)(&zghz3393, zCAP_MW);
          RECREATE(sail_int)(&zghz3394);
          CONVERT_OF(sail_int, mach_int)(&zghz3394, INT64_C(1));
          RECREATE(sail_int)(&zghz3395);
          sub_int(&zghz3395, zghz3393, zghz3394);
          zgaz3291 = CONVERT_OF(mach_int, sail_int)(zghz3395);
        }
        zgaz3292 = (safe_rshift(UINT64_MAX, 64 - 13) & (zBbits >> INT64_C(3)));
      }
      {
        RECREATE(sail_int)(&zghz3390);
        CONVERT_OF(sail_int, mach_int)(&zghz3390, zCAP_BASE_HI_BIT);
        RECREATE(sail_int)(&zghz3391);
        CONVERT_OF(sail_int, mach_int)(&zghz3391, zCAP_BASE_MANTISSA_LO_BIT);
        RECREATE(lbits)(&zghz3392);
        CONVERT_OF(lbits, fbits)(&zghz3392, zgaz3292, UINT64_C(13) , true);
        vector_update_subrange_lbits(&zghz3374, zghz3374, zghz3390, zghz3391, zghz3392);
      }
      unit zgsz3177;
      zgsz3177 = UNIT;
    }
    {
      uint64_t zgaz3294;
      {
        int64_t zgaz3293;
        {
          RECREATE(sail_int)(&zghz3387);
          CONVERT_OF(sail_int, mach_int)(&zghz3387, zCAP_MW);
          RECREATE(sail_int)(&zghz3388);
          CONVERT_OF(sail_int, mach_int)(&zghz3388, INT64_C(3));
          RECREATE(sail_int)(&zghz3389);
          sub_int(&zghz3389, zghz3387, zghz3388);
          zgaz3293 = CONVERT_OF(mach_int, sail_int)(zghz3389);
        }
        zgaz3294 = (safe_rshift(UINT64_MAX, 64 - 11) & (zTBits >> INT64_C(3)));
      }
      {
        RECREATE(sail_int)(&zghz3384);
        CONVERT_OF(sail_int, mach_int)(&zghz3384, zCAP_LIMIT_HI_BIT);
        RECREATE(sail_int)(&zghz3385);
        CONVERT_OF(sail_int, mach_int)(&zghz3385, zCAP_LIMIT_MANTISSA_LO_BIT);
        RECREATE(lbits)(&zghz3386);
        CONVERT_OF(lbits, fbits)(&zghz3386, zgaz3294, UINT64_C(11) , true);
        vector_update_subrange_lbits(&zghz3374, zghz3374, zghz3384, zghz3385, zghz3386);
      }
      unit zgsz3176;
      zgsz3176 = UNIT;
    }
    bool zfrom_large;
    {
      bool zgaz3304;
      {
        int64_t zgaz3303;
        zgaz3303 = sailgen_CapGetExponent(zc);
        {
          RECREATE(sail_int)(&zghz3383);
          CONVERT_OF(sail_int, mach_int)(&zghz3383, zgaz3303);
          zgaz3304 = sailgen_CapBoundsUsesValue(zghz3383);
        }
      }
      zfrom_large = not(zgaz3304);
    }
    bool zto_small;
    zto_small = sailgen_CapBoundsUsesValue(zghz3373);
    {
      bool zgaz3299;
      {
        bool zgaz3298;
        {    if (zfrom_large) {  zgaz3298 = zto_small;  } else {  zgaz3298 = false;  }
        }
        bool zgsz3186;
        if (zgaz3298) {
          uint64_t zgaz3297;
          {
            uint64_t zgaz3296;
            {
              int64_t zgaz3295;
              {
                RECREATE(sail_int)(&zghz3380);
                CONVERT_OF(sail_int, mach_int)(&zghz3380, zCAP_FLAGS_LO_BIT);
                RECREATE(sail_int)(&zghz3381);
                CONVERT_OF(sail_int, mach_int)(&zghz3381, INT64_C(1));
                RECREATE(sail_int)(&zghz3382);
                sub_int(&zghz3382, zghz3380, zghz3381);
                zgaz3295 = CONVERT_OF(mach_int, sail_int)(zghz3382);
              }
              zgaz3296 = (safe_rshift(UINT64_MAX, 64 - 56) & (zbase >> INT64_C(0)));
            }
            {
              RECREATE(lbits)(&zghz3377);
              CONVERT_OF(lbits, fbits)(&zghz3377, zgaz3296, UINT64_C(56) , true);
              RECREATE(sail_int)(&zghz3378);
              CONVERT_OF(sail_int, mach_int)(&zghz3378, INT64_C(64));
              RECREATE(lbits)(&zghz3379);
              sign_extend(&zghz3379, zghz3377, zghz3378);
              zgaz3297 = CONVERT_OF(fbits, lbits)(zghz3379, true);
            }
          }
          zgsz3186 = (zgaz3297 != zbase);
        } else {  zgsz3186 = false;  }
        zgaz3299 = zgsz3186;
      }
      unit zgsz3190;
      if (zgaz3299) {
        fbits zgaz3300;
        zgaz3300 = sailgen_Bit(UINT64_C(0b0));
        {
          RECREATE(sail_int)(&zghz3376);
          CONVERT_OF(sail_int, mach_int)(&zghz3376, zCAP_TAG_BIT);
          update_lbits(&zghz3374, zghz3374, zghz3376, zgaz3300);
        }
        zgsz3190 = UNIT;
      } else {  zgsz3190 = UNIT;  }
    }
    {
      bool zgaz3301;
      {
        if (zexact) {
          bool zgsz3187;
          if (zlostBottom) {  zgsz3187 = true;  } else {  zgsz3187 = zlostTop;  }
          zgaz3301 = zgsz3187;
        } else {  zgaz3301 = false;  }
      }
      unit zgsz3189;
      if (zgaz3301) {
        fbits zgaz3302;
        zgaz3302 = sailgen_Bit(UINT64_C(0b0));
        {
          RECREATE(sail_int)(&zghz3375);
          CONVERT_OF(sail_int, mach_int)(&zghz3375, zCAP_TAG_BIT);
          update_lbits(&zghz3374, zghz3374, zghz3375, zgaz3302);
        }
        zgsz3189 = UNIT;
      } else {  zgsz3189 = UNIT;  }
    }
    COPY(lbits)((*(&zcbz333)), zghz3374);
    goto cleanup_255;
    /* unreachable after return */
    goto finish_match_250;
    goto end_cleanup_256;
  cleanup_255: ;
    goto end_function_254;
  end_cleanup_256: ;
  }
case_251: ;
  sail_match_failure("CapSetBounds");
finish_match_250: ;
  COPY(lbits)((*(&zcbz333)), zghz3372);










end_function_254: ;
  goto end_function_265;
end_block_exception_257: ;
  goto end_function_265;
end_function_265: ;
}



static void finish_sailgen_CapSetBounds(void)
{
  KILL(lbits)(&zghz3559);
  KILL(sail_int)(&zghz3558);
  KILL(sail_int)(&zghz3557);
  KILL(sail_int)(&zghz3556);
  KILL(sail_int)(&zghz3555);
  KILL(sail_int)(&zghz3554);
  KILL(sail_int)(&zghz3553);
  KILL(sail_int)(&zghz3552);
  KILL(sail_int)(&zghz3551);
  KILL(sail_int)(&zghz3550);
  KILL(sail_int)(&zghz3549);
  KILL(sail_int)(&zghz3548);
  KILL(sail_int)(&zghz3547);
  KILL(lbits)(&zghz3546);
  KILL(sail_int)(&zghz3545);
  KILL(sail_int)(&zghz3544);
  KILL(sail_int)(&zghz3543);
  KILL(lbits)(&zghz3542);
  KILL(sail_int)(&zghz3541);
  KILL(sail_int)(&zghz3540);
  KILL(sail_int)(&zghz3539);
  KILL(sail_int)(&zghz3538);
  KILL(sail_int)(&zghz3537);
  KILL(sail_int)(&zghz3536);
  KILL(sail_int)(&zghz3535);
  KILL(sail_int)(&zghz3534);
  KILL(lbits)(&zghz3533);
  KILL(lbits)(&zghz3532);
  KILL(lbits)(&zghz3531);
  KILL(lbits)(&zghz3530);
  KILL(sail_int)(&zghz3529);
  KILL(sail_int)(&zghz3528);
  KILL(sail_int)(&zghz3527);
  KILL(lbits)(&zghz3526);
  KILL(sail_int)(&zghz3525);
  KILL(sail_int)(&zghz3524);
  KILL(sail_int)(&zghz3523);
  KILL(sail_int)(&zghz3522);
  KILL(sail_int)(&zghz3521);
  KILL(lbits)(&zghz3520);
  KILL(sail_int)(&zghz3519);
  KILL(sail_int)(&zghz3518);
  KILL(sail_int)(&zghz3517);
  KILL(sail_int)(&zghz3516);
  KILL(sail_int)(&zghz3515);
  KILL(sail_int)(&zghz3514);
  KILL(sail_int)(&zghz3513);
  KILL(sail_int)(&zghz3512);
  KILL(sail_int)(&zghz3511);
  KILL(sail_int)(&zghz3510);
  KILL(sail_int)(&zghz3509);
  KILL(sail_int)(&zghz3508);
  KILL(sail_int)(&zghz3507);
  KILL(sail_int)(&zghz3506);
  KILL(sail_int)(&zghz3505);
  KILL(sail_int)(&zghz3504);
  KILL(sail_int)(&zghz3503);
  KILL(sail_int)(&zghz3502);
  KILL(sail_int)(&zghz3501);
  KILL(lbits)(&zghz3500);
  KILL(sail_int)(&zghz3499);
  KILL(sail_int)(&zghz3498);
  KILL(sail_int)(&zghz3497);
  KILL(sail_int)(&zghz3496);
  KILL(sail_int)(&zghz3495);
  KILL(sail_int)(&zghz3494);
  KILL(sail_int)(&zghz3493);
  KILL(sail_int)(&zghz3492);
  KILL(sail_int)(&zghz3491);
  KILL(sail_int)(&zghz3490);
  KILL(sail_int)(&zghz3489);
  KILL(sail_int)(&zghz3488);
  KILL(sail_int)(&zghz3487);
  KILL(sail_int)(&zghz3486);
  KILL(sail_int)(&zghz3485);
  KILL(sail_int)(&zghz3484);
  KILL(sail_int)(&zghz3483);
  KILL(sail_int)(&zghz3482);
  KILL(sail_int)(&zghz3481);
  KILL(sail_int)(&zghz3480);
  KILL(lbits)(&zghz3479);
  KILL(sail_int)(&zghz3478);
  KILL(sail_int)(&zghz3477);
  KILL(sail_int)(&zghz3476);
  KILL(sail_int)(&zghz3475);
  KILL(sail_int)(&zghz3474);
  KILL(sail_int)(&zghz3473);
  KILL(sail_int)(&zghz3472);
  KILL(sail_int)(&zghz3471);
  KILL(sail_int)(&zghz3470);
  KILL(sail_int)(&zghz3469);
  KILL(sail_int)(&zghz3468);
  KILL(sail_int)(&zghz3467);
  KILL(sail_int)(&zghz3466);
  KILL(sail_int)(&zghz3465);
  KILL(sail_int)(&zghz3464);
  KILL(lbits)(&zghz3463);
  KILL(sail_int)(&zghz3462);
  KILL(sail_int)(&zghz3461);
  KILL(lbits)(&zghz3460);
  KILL(lbits)(&zghz3459);
  KILL(sail_int)(&zghz3458);
  KILL(lbits)(&zghz3457);
  KILL(lbits)(&zghz3456);
  KILL(lbits)(&zghz3455);
  KILL(sail_int)(&zghz3454);
  KILL(lbits)(&zghz3453);
  KILL(sail_int)(&zghz3452);
  KILL(sail_int)(&zghz3451);
  KILL(sail_int)(&zghz3450);
  KILL(sail_int)(&zghz3449);
  KILL(sail_int)(&zghz3448);
  KILL(sail_int)(&zghz3447);
  KILL(sail_int)(&zghz3446);
  KILL(sail_int)(&zghz3445);
  KILL(sail_int)(&zghz3444);
  KILL(sail_int)(&zghz3443);
  KILL(sail_int)(&zghz3442);
  KILL(sail_int)(&zghz3441);
  KILL(lbits)(&zghz3440);
  KILL(sail_int)(&zghz3439);
  KILL(sail_int)(&zghz3438);
  KILL(sail_int)(&zghz3437);
  KILL(sail_int)(&zghz3436);
  KILL(sail_int)(&zghz3435);
  KILL(sail_int)(&zghz3434);
  KILL(lbits)(&zghz3433);
  KILL(sail_int)(&zghz3432);
  KILL(sail_int)(&zghz3431);
  KILL(lbits)(&zghz3430);
  KILL(sail_int)(&zghz3429);
  KILL(lbits)(&zghz3428);
  KILL(sail_int)(&zghz3427);
  KILL(sail_int)(&zghz3426);
  KILL(lbits)(&zghz3425);
  KILL(lbits)(&zghz3424);
  KILL(lbits)(&zghz3423);
  KILL(sail_int)(&zghz3422);
  KILL(sail_int)(&zghz3421);
  KILL(lbits)(&zghz3420);
  KILL(sail_int)(&zghz3419);
  KILL(sail_int)(&zghz3418);
  KILL(lbits)(&zghz3417);
  KILL(sail_int)(&zghz3416);
  KILL(sail_int)(&zghz3415);
  KILL(lbits)(&zghz3414);
  KILL(sail_int)(&zghz3413);
  KILL(sail_int)(&zghz3412);
  KILL(lbits)(&zghz3411);
  KILL(sail_int)(&zghz3410);
  KILL(sail_int)(&zghz3409);
  KILL(lbits)(&zghz3408);
  KILL(sail_int)(&zghz3407);
  KILL(sail_int)(&zghz3406);
  KILL(lbits)(&zghz3405);
  KILL(sail_int)(&zghz3404);
  KILL(sail_int)(&zghz3403);
  KILL(sail_int)(&zghz3402);
  KILL(lbits)(&zghz3401);
  KILL(sail_int)(&zghz3400);
  KILL(sail_int)(&zghz3399);
  KILL(lbits)(&zghz3398);
  KILL(sail_int)(&zghz3397);
  KILL(sail_int)(&zghz3396);
  KILL(sail_int)(&zghz3395);
  KILL(sail_int)(&zghz3394);
  KILL(sail_int)(&zghz3393);
  KILL(lbits)(&zghz3392);
  KILL(sail_int)(&zghz3391);
  KILL(sail_int)(&zghz3390);
  KILL(sail_int)(&zghz3389);
  KILL(sail_int)(&zghz3388);
  KILL(sail_int)(&zghz3387);
  KILL(lbits)(&zghz3386);
  KILL(sail_int)(&zghz3385);
  KILL(sail_int)(&zghz3384);
  KILL(sail_int)(&zghz3383);
  KILL(sail_int)(&zghz3382);
  KILL(sail_int)(&zghz3381);
  KILL(sail_int)(&zghz3380);
  KILL(lbits)(&zghz3379);
  KILL(sail_int)(&zghz3378);
  KILL(lbits)(&zghz3377);
  KILL(sail_int)(&zghz3376);
  KILL(sail_int)(&zghz3375);
  KILL(lbits)(&zghz3374);
  KILL(sail_int)(&zghz3373);
  KILL(lbits)(&zghz3372);
  KILL(lbits)(&zghz3371);
  KILL(lbits)(&zghz3370);
  KILL(sail_int)(&zghz3369);
  KILL(lbits)(&zghz3368);
  KILL(lbits)(&zghz3367);
}

static lbits zCMAX;
static void create_letbind_70(void) {    CREATE(lbits)(&zCMAX);


  lbits zgsz3193;
  CREATE(lbits)(&zgsz3193);
  lbits zgsz3192;
  CREATE(lbits)(&zgsz3192);
  CONVERT_OF(lbits, fbits)(&zgsz3192, UINT64_C(0b1), UINT64_C(1) , true);
  append_64(&zgsz3192, zgsz3192, UINT64_C(0xFFFFC00000010005));
  append_64(&zgsz3192, zgsz3192, UINT64_C(0x0000000000000000));
  COPY(lbits)(&zgsz3193, zgsz3192);
  KILL(lbits)(&zgsz3192);
  COPY(lbits)(&zCMAX, zgsz3193);
  KILL(lbits)(&zgsz3193);
let_end_258: ;
}
static void kill_letbind_70(void) {    KILL(lbits)(&zCMAX);
}

// register TestCap
static lbits zTestCap;

static unit sailgen_main(unit);

lbits zghz3560;
lbits zghz3561;
sail_int zghz3562;
lbits zghz3563;
lbits zghz3564;
sail_int zghz3565;
sail_int zghz3566;

static void startup_sailgen_main(void)
{
  CREATE(lbits)(&zghz3560);
  CREATE(lbits)(&zghz3561);
  CREATE(sail_int)(&zghz3562);
  CREATE(lbits)(&zghz3563);
  CREATE(lbits)(&zghz3564);
  CREATE(sail_int)(&zghz3565);
  CREATE(sail_int)(&zghz3566);
}

static unit sailgen_main(unit zgsz3194)
{
  __label__ end_function_260, end_block_exception_261;

  unit zcbz334;
  {
    {
      RECREATE(sail_int)(&zghz3566);
      CONVERT_OF(sail_int, mach_int)(&zghz3566, INT64_C(129));
      sailgen_Zeros(&zTestCap, zghz3566);
    }
    unit zgsz3195;
    zgsz3195 = UNIT;
  }
  RECREATE(lbits)(&zghz3560);
  COPY(lbits)(&zghz3560, zCMAX);
  uint64_t zbits1;
  zbits1 = sailgen_CapGetValue(zTestCap);
  uint64_t zbits2;
  zbits2 = sailgen_CapGetPermissions(zTestCap);
  uint64_t zbits3;
  zbits3 = sailgen_CapGetObjectType(zTestCap);
  uint64_t zbits4;
  zbits4 = sailgen_CapGetTag(zTestCap);
  RECREATE(lbits)(&zghz3561);
  {
    RECREATE(lbits)(&zghz3564);
    {
      RECREATE(sail_int)(&zghz3565);
      CONVERT_OF(sail_int, mach_int)(&zghz3565, INT64_C(65));
      sailgen_Zeros(&zghz3564, zghz3565);
    }
    sailgen_CapSetBounds(&zghz3561, zTestCap, zghz3564, true);
  }
  struct ztuple_z8z5bvzCz0z5bvzCz0z5boolz9 zbits6;
  CREATE(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zbits6);
  sailgen_CapGetBounds(&zbits6, zTestCap);
  int64_t zexp;
  zexp = sailgen_CapGetExponent(zTestCap);
  bool zrep;
  {
    uint64_t zgaz3323;
    {
      RECREATE(sail_int)(&zghz3562);
      CONVERT_OF(sail_int, mach_int)(&zghz3562, INT64_C(64));
      RECREATE(lbits)(&zghz3563);
      sailgen_Zeros(&zghz3563, zghz3562);
      zgaz3323 = CONVERT_OF(fbits, lbits)(zghz3563, true);
    }
    zrep = sailgen_CapIsRepresentableFast(zTestCap, zgaz3323);
  }
  zcbz334 = UNIT;


  KILL(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zbits6);




end_function_260: ;
  return zcbz334;
end_block_exception_261: ;

  return UNIT;
}



static void finish_sailgen_main(void)
{
  KILL(sail_int)(&zghz3566);
  KILL(sail_int)(&zghz3565);
  KILL(lbits)(&zghz3564);
  KILL(lbits)(&zghz3563);
  KILL(sail_int)(&zghz3562);
  KILL(lbits)(&zghz3561);
  KILL(lbits)(&zghz3560);
}

static unit sailgen_initializze_registers(unit);

sail_int zghz3567;
sail_int zghz3568;
sail_int zghz3569;
lbits zghz3570;
sail_int zghz3571;
lbits zghz3572;
sail_int zghz3573;
lbits zghz3574;
lbits zghz3575;
sail_int zghz3576;
sail_int zghz3577;
sail_int zghz3578;
lbits zghz3579;
sail_int zghz3580;
lbits zghz3581;
sail_int zghz3582;
lbits zghz3583;
sail_int zghz3584;
lbits zghz3585;
sail_int zghz3586;
lbits zghz3587;
sail_int zghz3588;
lbits zghz3589;
sail_int zghz3590;
lbits zghz3591;
sail_int zghz3592;
lbits zghz3593;
sail_int zghz3594;
lbits zghz3595;
sail_int zghz3596;
lbits zghz3597;
sail_int zghz3598;
lbits zghz3599;
sail_int zghz3600;
lbits zghz3601;
sail_int zghz3602;
lbits zghz3603;
sail_int zghz3604;
sail_int zghz3605;
sail_int zghz3606;
sail_int zghz3607;
sail_int zghz3608;
sail_int zghz3609;
lbits zghz3610;
sail_int zghz3611;
lbits zghz3612;
sail_int zghz3613;
lbits zghz3614;
sail_int zghz3615;
lbits zghz3616;
sail_int zghz3617;
lbits zghz3618;
sail_int zghz3619;
lbits zghz3620;
sail_int zghz3621;
lbits zghz3622;
sail_int zghz3623;
lbits zghz3624;
sail_int zghz3625;
sail_int zghz3626;
sail_int zghz3627;
sail_int zghz3628;
sail_int zghz3629;
sail_int zghz3630;
sail_int zghz3631;
sail_int zghz3632;
sail_int zghz3633;
lbits zghz3634;
sail_int zghz3635;
lbits zghz3636;
sail_int zghz3637;
lbits zghz3638;
sail_int zghz3639;
lbits zghz3640;
sail_int zghz3641;
lbits zghz3642;
sail_int zghz3643;
lbits zghz3644;
sail_int zghz3645;
lbits zghz3646;
sail_int zghz3647;
lbits zghz3648;
sail_int zghz3649;
sail_int zghz3650;
lbits zghz3651;
sail_int zghz3652;
lbits zghz3653;
sail_int zghz3654;
lbits zghz3655;
sail_int zghz3656;
lbits zghz3657;
sail_int zghz3658;
sail_int zghz3659;
sail_int zghz3660;
lbits zghz3661;
sail_int zghz3662;
lbits zghz3663;
sail_int zghz3664;
lbits zghz3665;
sail_int zghz3666;
lbits zghz3667;
sail_int zghz3668;
lbits zghz3669;
sail_int zghz3670;
lbits zghz3671;
sail_int zghz3672;
lbits zghz3673;
sail_int zghz3674;
lbits zghz3675;
sail_int zghz3676;
lbits zghz3677;
sail_int zghz3678;
lbits zghz3679;
sail_int zghz3680;
lbits zghz3681;
sail_int zghz3682;
lbits zghz3683;
sail_int zghz3684;
lbits zghz3685;
sail_int zghz3686;
lbits zghz3687;
sail_int zghz3688;
lbits zghz3689;
sail_int zghz3690;
lbits zghz3691;
sail_int zghz3692;
lbits zghz3693;
sail_int zghz3694;
lbits zghz3695;
sail_int zghz3696;
lbits zghz3697;
sail_int zghz3698;
sail_int zghz3699;
lbits zghz3700;
sail_int zghz3701;
sail_int zghz3702;
lbits zghz3703;
sail_int zghz3704;
lbits zghz3705;
sail_int zghz3706;
lbits zghz3707;
sail_int zghz3708;
lbits zghz3709;
sail_int zghz3710;
lbits zghz3711;
sail_int zghz3712;
lbits zghz3713;
sail_int zghz3714;
lbits zghz3715;
sail_int zghz3716;
lbits zghz3717;
sail_int zghz3718;
lbits zghz3719;
sail_int zghz3720;
lbits zghz3721;
sail_int zghz3722;
lbits zghz3723;
sail_int zghz3724;
lbits zghz3725;
sail_int zghz3726;
lbits zghz3727;
sail_int zghz3728;
lbits zghz3729;
sail_int zghz3730;
lbits zghz3731;
sail_int zghz3732;
lbits zghz3733;
sail_int zghz3734;
lbits zghz3735;
sail_int zghz3736;
lbits zghz3737;
sail_int zghz3738;
lbits zghz3739;
sail_int zghz3740;
lbits zghz3741;
sail_int zghz3742;
lbits zghz3743;
sail_int zghz3744;
lbits zghz3745;
sail_int zghz3746;
lbits zghz3747;
sail_int zghz3748;
lbits zghz3749;
sail_int zghz3750;
lbits zghz3751;
sail_int zghz3752;
lbits zghz3753;
sail_int zghz3754;
lbits zghz3755;
sail_int zghz3756;
lbits zghz3757;
sail_int zghz3758;
lbits zghz3759;
sail_int zghz3760;
lbits zghz3761;
sail_int zghz3762;
lbits zghz3763;
sail_int zghz3764;
lbits zghz3765;
sail_int zghz3766;
lbits zghz3767;
sail_int zghz3768;
lbits zghz3769;
sail_int zghz3770;
lbits zghz3771;
sail_int zghz3772;
lbits zghz3773;
sail_int zghz3774;
lbits zghz3775;
sail_int zghz3776;
lbits zghz3777;
sail_int zghz3778;
lbits zghz3779;
sail_int zghz3780;
lbits zghz3781;
sail_int zghz3782;
lbits zghz3783;
sail_int zghz3784;
lbits zghz3785;
sail_int zghz3786;
lbits zghz3787;
sail_int zghz3788;
lbits zghz3789;
sail_int zghz3790;
lbits zghz3791;
sail_int zghz3792;
lbits zghz3793;
sail_int zghz3794;
lbits zghz3795;
sail_int zghz3796;
lbits zghz3797;
sail_int zghz3798;
lbits zghz3799;
sail_int zghz3800;
lbits zghz3801;
sail_int zghz3802;
lbits zghz3803;
sail_int zghz3804;
lbits zghz3805;
sail_int zghz3806;
lbits zghz3807;
sail_int zghz3808;
lbits zghz3809;
sail_int zghz3810;
lbits zghz3811;
sail_int zghz3812;
lbits zghz3813;
sail_int zghz3814;
lbits zghz3815;
sail_int zghz3816;
lbits zghz3817;
sail_int zghz3818;
lbits zghz3819;
sail_int zghz3820;
lbits zghz3821;
sail_int zghz3822;
lbits zghz3823;
sail_int zghz3824;
lbits zghz3825;
sail_int zghz3826;
lbits zghz3827;
sail_int zghz3828;
lbits zghz3829;
sail_int zghz3830;
lbits zghz3831;
sail_int zghz3832;
lbits zghz3833;
sail_int zghz3834;
lbits zghz3835;
sail_int zghz3836;
lbits zghz3837;
sail_int zghz3838;
lbits zghz3839;
sail_int zghz3840;
lbits zghz3841;
sail_int zghz3842;
lbits zghz3843;
sail_int zghz3844;
lbits zghz3845;
sail_int zghz3846;
lbits zghz3847;
sail_int zghz3848;
lbits zghz3849;
sail_int zghz3850;
lbits zghz3851;
sail_int zghz3852;
lbits zghz3853;
sail_int zghz3854;
lbits zghz3855;
sail_int zghz3856;
lbits zghz3857;
sail_int zghz3858;
lbits zghz3859;
sail_int zghz3860;
lbits zghz3861;
sail_int zghz3862;
lbits zghz3863;
sail_int zghz3864;
lbits zghz3865;
sail_int zghz3866;
lbits zghz3867;
sail_int zghz3868;
lbits zghz3869;
sail_int zghz3870;
lbits zghz3871;
sail_int zghz3872;
sail_int zghz3873;
lbits zghz3874;
sail_int zghz3875;
lbits zghz3876;
sail_int zghz3877;
lbits zghz3878;
sail_int zghz3879;
sail_int zghz3880;
lbits zghz3881;
sail_int zghz3882;
sail_int zghz3883;
lbits zghz3884;
sail_int zghz3885;
lbits zghz3886;
sail_int zghz3887;
lbits zghz3888;
sail_int zghz3889;
lbits zghz3890;
sail_int zghz3891;
lbits zghz3892;
sail_int zghz3893;
lbits zghz3894;
sail_int zghz3895;
lbits zghz3896;
sail_int zghz3897;
lbits zghz3898;
sail_int zghz3899;
lbits zghz3900;
sail_int zghz3901;
lbits zghz3902;
sail_int zghz3903;
lbits zghz3904;
sail_int zghz3905;
lbits zghz3906;
sail_int zghz3907;
lbits zghz3908;
sail_int zghz3909;
lbits zghz3910;
sail_int zghz3911;
lbits zghz3912;
sail_int zghz3913;
lbits zghz3914;
sail_int zghz3915;
lbits zghz3916;
sail_int zghz3917;
lbits zghz3918;
sail_int zghz3919;
lbits zghz3920;
sail_int zghz3921;
lbits zghz3922;
sail_int zghz3923;
lbits zghz3924;
sail_int zghz3925;
lbits zghz3926;
sail_int zghz3927;
lbits zghz3928;
sail_int zghz3929;
lbits zghz3930;
sail_int zghz3931;
lbits zghz3932;
sail_int zghz3933;
lbits zghz3934;
sail_int zghz3935;
lbits zghz3936;
sail_int zghz3937;
lbits zghz3938;
sail_int zghz3939;
sail_int zghz3940;
lbits zghz3941;
sail_int zghz3942;
sail_int zghz3943;
lbits zghz3944;
sail_int zghz3945;
sail_int zghz3946;
lbits zghz3947;
sail_int zghz3948;
sail_int zghz3949;
lbits zghz3950;
sail_int zghz3951;
sail_int zghz3952;
lbits zghz3953;
sail_int zghz3954;
lbits zghz3955;
sail_int zghz3956;
lbits zghz3957;
sail_int zghz3958;
lbits zghz3959;
sail_int zghz3960;
lbits zghz3961;
sail_int zghz3962;
lbits zghz3963;
sail_int zghz3964;
sail_int zghz3965;
lbits zghz3966;
sail_int zghz3967;
lbits zghz3968;
sail_int zghz3969;
lbits zghz3970;
sail_int zghz3971;
lbits zghz3972;
sail_int zghz3973;
lbits zghz3974;
sail_int zghz3975;
lbits zghz3976;
sail_int zghz3977;
lbits zghz3978;
sail_int zghz3979;
lbits zghz3980;
sail_int zghz3981;
lbits zghz3982;
sail_int zghz3983;
lbits zghz3984;
sail_int zghz3985;
lbits zghz3986;
sail_int zghz3987;
lbits zghz3988;
sail_int zghz3989;
lbits zghz3990;
sail_int zghz3991;
lbits zghz3992;
sail_int zghz3993;
lbits zghz3994;
sail_int zghz3995;
lbits zghz3996;
sail_int zghz3997;
lbits zghz3998;
sail_int zghz3999;
sail_int zghz31000;
lbits zghz31001;
sail_int zghz31002;
sail_int zghz31003;
lbits zghz31004;
sail_int zghz31005;
lbits zghz31006;
sail_int zghz31007;
sail_int zghz31008;
lbits zghz31009;
sail_int zghz31010;
lbits zghz31011;
sail_int zghz31012;
lbits zghz31013;
sail_int zghz31014;
lbits zghz31015;
sail_int zghz31016;
sail_int zghz31017;
lbits zghz31018;
sail_int zghz31019;
lbits zghz31020;
sail_int zghz31021;
lbits zghz31022;
sail_int zghz31023;
lbits zghz31024;
sail_int zghz31025;
lbits zghz31026;
sail_int zghz31027;
sail_int zghz31028;
sail_int zghz31029;
lbits zghz31030;
sail_int zghz31031;
lbits zghz31032;
sail_int zghz31033;
lbits zghz31034;
sail_int zghz31035;
lbits zghz31036;
sail_int zghz31037;
lbits zghz31038;
sail_int zghz31039;
lbits zghz31040;
sail_int zghz31041;
lbits zghz31042;
sail_int zghz31043;
lbits zghz31044;
sail_int zghz31045;
lbits zghz31046;
sail_int zghz31047;
lbits zghz31048;
sail_int zghz31049;
lbits zghz31050;
sail_int zghz31051;
lbits zghz31052;
sail_int zghz31053;
lbits zghz31054;
sail_int zghz31055;
lbits zghz31056;
sail_int zghz31057;
lbits zghz31058;
sail_int zghz31059;
lbits zghz31060;
sail_int zghz31061;
lbits zghz31062;
sail_int zghz31063;
lbits zghz31064;
sail_int zghz31065;
lbits zghz31066;
sail_int zghz31067;
lbits zghz31068;
sail_int zghz31069;
lbits zghz31070;
sail_int zghz31071;
lbits zghz31072;
sail_int zghz31073;
lbits zghz31074;
sail_int zghz31075;
lbits zghz31076;
sail_int zghz31077;
lbits zghz31078;
sail_int zghz31079;
lbits zghz31080;
sail_int zghz31081;
lbits zghz31082;
sail_int zghz31083;
lbits zghz31084;
sail_int zghz31085;
lbits zghz31086;
sail_int zghz31087;
lbits zghz31088;
sail_int zghz31089;
lbits zghz31090;
sail_int zghz31091;
lbits zghz31092;
sail_int zghz31093;
lbits zghz31094;
sail_int zghz31095;
lbits zghz31096;
sail_int zghz31097;
lbits zghz31098;
sail_int zghz31099;
lbits zghz31100;
sail_int zghz31101;
lbits zghz31102;
sail_int zghz31103;
lbits zghz31104;
sail_int zghz31105;
lbits zghz31106;
sail_int zghz31107;
lbits zghz31108;
sail_int zghz31109;
lbits zghz31110;
sail_int zghz31111;
lbits zghz31112;
sail_int zghz31113;
lbits zghz31114;
sail_int zghz31115;
lbits zghz31116;
sail_int zghz31117;
lbits zghz31118;
sail_int zghz31119;
lbits zghz31120;
sail_int zghz31121;
lbits zghz31122;
sail_int zghz31123;
lbits zghz31124;
sail_int zghz31125;
lbits zghz31126;
sail_int zghz31127;
lbits zghz31128;
sail_int zghz31129;
lbits zghz31130;
sail_int zghz31131;
lbits zghz31132;
sail_int zghz31133;
lbits zghz31134;
sail_int zghz31135;
lbits zghz31136;
sail_int zghz31137;
lbits zghz31138;
sail_int zghz31139;
lbits zghz31140;
sail_int zghz31141;
lbits zghz31142;
sail_int zghz31143;
sail_int zghz31144;
sail_int zghz31145;
sail_int zghz31146;
sail_int zghz31147;
sail_int zghz31148;
sail_int zghz31149;
lbits zghz31150;
sail_int zghz31151;
lbits zghz31152;
sail_int zghz31153;
lbits zghz31154;
sail_int zghz31155;
lbits zghz31156;
sail_int zghz31157;
lbits zghz31158;
sail_int zghz31159;
lbits zghz31160;
sail_int zghz31161;
lbits zghz31162;
sail_int zghz31163;
lbits zghz31164;
sail_int zghz31165;
lbits zghz31166;
sail_int zghz31167;
lbits zghz31168;
sail_int zghz31169;
lbits zghz31170;
sail_int zghz31171;
lbits zghz31172;
sail_int zghz31173;
lbits zghz31174;
sail_int zghz31175;
lbits zghz31176;
sail_int zghz31177;
lbits zghz31178;
sail_int zghz31179;
sail_int zghz31180;
sail_int zghz31181;
sail_int zghz31182;
sail_int zghz31183;
sail_int zghz31184;
sail_int zghz31185;
sail_int zghz31186;
sail_int zghz31187;
sail_int zghz31188;
sail_int zghz31189;
sail_int zghz31190;
sail_int zghz31191;
sail_int zghz31192;
sail_int zghz31193;
sail_int zghz31194;
sail_int zghz31195;
sail_int zghz31196;
sail_int zghz31197;
sail_int zghz31198;
sail_int zghz31199;
sail_int zghz31200;
sail_int zghz31201;
sail_int zghz31202;
sail_int zghz31203;
sail_int zghz31204;
sail_int zghz31205;
sail_int zghz31206;
sail_int zghz31207;
sail_int zghz31208;
sail_int zghz31209;
sail_int zghz31210;
lbits zghz31211;
sail_int zghz31212;
lbits zghz31213;
sail_int zghz31214;
lbits zghz31215;
sail_int zghz31216;
lbits zghz31217;
sail_int zghz31218;
lbits zghz31219;
sail_int zghz31220;
lbits zghz31221;
sail_int zghz31222;
lbits zghz31223;
sail_int zghz31224;
lbits zghz31225;
sail_int zghz31226;

static void startup_sailgen_initializze_registers(void)
{
  CREATE(sail_int)(&zghz3567);
  CREATE(sail_int)(&zghz3568);
  CREATE(sail_int)(&zghz3569);
  CREATE(lbits)(&zghz3570);
  CREATE(sail_int)(&zghz3571);
  CREATE(lbits)(&zghz3572);
  CREATE(sail_int)(&zghz3573);
  CREATE(lbits)(&zghz3574);
  CREATE(lbits)(&zghz3575);
  CREATE(sail_int)(&zghz3576);
  CREATE(sail_int)(&zghz3577);
  CREATE(sail_int)(&zghz3578);
  CREATE(lbits)(&zghz3579);
  CREATE(sail_int)(&zghz3580);
  CREATE(lbits)(&zghz3581);
  CREATE(sail_int)(&zghz3582);
  CREATE(lbits)(&zghz3583);
  CREATE(sail_int)(&zghz3584);
  CREATE(lbits)(&zghz3585);
  CREATE(sail_int)(&zghz3586);
  CREATE(lbits)(&zghz3587);
  CREATE(sail_int)(&zghz3588);
  CREATE(lbits)(&zghz3589);
  CREATE(sail_int)(&zghz3590);
  CREATE(lbits)(&zghz3591);
  CREATE(sail_int)(&zghz3592);
  CREATE(lbits)(&zghz3593);
  CREATE(sail_int)(&zghz3594);
  CREATE(lbits)(&zghz3595);
  CREATE(sail_int)(&zghz3596);
  CREATE(lbits)(&zghz3597);
  CREATE(sail_int)(&zghz3598);
  CREATE(lbits)(&zghz3599);
  CREATE(sail_int)(&zghz3600);
  CREATE(lbits)(&zghz3601);
  CREATE(sail_int)(&zghz3602);
  CREATE(lbits)(&zghz3603);
  CREATE(sail_int)(&zghz3604);
  CREATE(sail_int)(&zghz3605);
  CREATE(sail_int)(&zghz3606);
  CREATE(sail_int)(&zghz3607);
  CREATE(sail_int)(&zghz3608);
  CREATE(sail_int)(&zghz3609);
  CREATE(lbits)(&zghz3610);
  CREATE(sail_int)(&zghz3611);
  CREATE(lbits)(&zghz3612);
  CREATE(sail_int)(&zghz3613);
  CREATE(lbits)(&zghz3614);
  CREATE(sail_int)(&zghz3615);
  CREATE(lbits)(&zghz3616);
  CREATE(sail_int)(&zghz3617);
  CREATE(lbits)(&zghz3618);
  CREATE(sail_int)(&zghz3619);
  CREATE(lbits)(&zghz3620);
  CREATE(sail_int)(&zghz3621);
  CREATE(lbits)(&zghz3622);
  CREATE(sail_int)(&zghz3623);
  CREATE(lbits)(&zghz3624);
  CREATE(sail_int)(&zghz3625);
  CREATE(sail_int)(&zghz3626);
  CREATE(sail_int)(&zghz3627);
  CREATE(sail_int)(&zghz3628);
  CREATE(sail_int)(&zghz3629);
  CREATE(sail_int)(&zghz3630);
  CREATE(sail_int)(&zghz3631);
  CREATE(sail_int)(&zghz3632);
  CREATE(sail_int)(&zghz3633);
  CREATE(lbits)(&zghz3634);
  CREATE(sail_int)(&zghz3635);
  CREATE(lbits)(&zghz3636);
  CREATE(sail_int)(&zghz3637);
  CREATE(lbits)(&zghz3638);
  CREATE(sail_int)(&zghz3639);
  CREATE(lbits)(&zghz3640);
  CREATE(sail_int)(&zghz3641);
  CREATE(lbits)(&zghz3642);
  CREATE(sail_int)(&zghz3643);
  CREATE(lbits)(&zghz3644);
  CREATE(sail_int)(&zghz3645);
  CREATE(lbits)(&zghz3646);
  CREATE(sail_int)(&zghz3647);
  CREATE(lbits)(&zghz3648);
  CREATE(sail_int)(&zghz3649);
  CREATE(sail_int)(&zghz3650);
  CREATE(lbits)(&zghz3651);
  CREATE(sail_int)(&zghz3652);
  CREATE(lbits)(&zghz3653);
  CREATE(sail_int)(&zghz3654);
  CREATE(lbits)(&zghz3655);
  CREATE(sail_int)(&zghz3656);
  CREATE(lbits)(&zghz3657);
  CREATE(sail_int)(&zghz3658);
  CREATE(sail_int)(&zghz3659);
  CREATE(sail_int)(&zghz3660);
  CREATE(lbits)(&zghz3661);
  CREATE(sail_int)(&zghz3662);
  CREATE(lbits)(&zghz3663);
  CREATE(sail_int)(&zghz3664);
  CREATE(lbits)(&zghz3665);
  CREATE(sail_int)(&zghz3666);
  CREATE(lbits)(&zghz3667);
  CREATE(sail_int)(&zghz3668);
  CREATE(lbits)(&zghz3669);
  CREATE(sail_int)(&zghz3670);
  CREATE(lbits)(&zghz3671);
  CREATE(sail_int)(&zghz3672);
  CREATE(lbits)(&zghz3673);
  CREATE(sail_int)(&zghz3674);
  CREATE(lbits)(&zghz3675);
  CREATE(sail_int)(&zghz3676);
  CREATE(lbits)(&zghz3677);
  CREATE(sail_int)(&zghz3678);
  CREATE(lbits)(&zghz3679);
  CREATE(sail_int)(&zghz3680);
  CREATE(lbits)(&zghz3681);
  CREATE(sail_int)(&zghz3682);
  CREATE(lbits)(&zghz3683);
  CREATE(sail_int)(&zghz3684);
  CREATE(lbits)(&zghz3685);
  CREATE(sail_int)(&zghz3686);
  CREATE(lbits)(&zghz3687);
  CREATE(sail_int)(&zghz3688);
  CREATE(lbits)(&zghz3689);
  CREATE(sail_int)(&zghz3690);
  CREATE(lbits)(&zghz3691);
  CREATE(sail_int)(&zghz3692);
  CREATE(lbits)(&zghz3693);
  CREATE(sail_int)(&zghz3694);
  CREATE(lbits)(&zghz3695);
  CREATE(sail_int)(&zghz3696);
  CREATE(lbits)(&zghz3697);
  CREATE(sail_int)(&zghz3698);
  CREATE(sail_int)(&zghz3699);
  CREATE(lbits)(&zghz3700);
  CREATE(sail_int)(&zghz3701);
  CREATE(sail_int)(&zghz3702);
  CREATE(lbits)(&zghz3703);
  CREATE(sail_int)(&zghz3704);
  CREATE(lbits)(&zghz3705);
  CREATE(sail_int)(&zghz3706);
  CREATE(lbits)(&zghz3707);
  CREATE(sail_int)(&zghz3708);
  CREATE(lbits)(&zghz3709);
  CREATE(sail_int)(&zghz3710);
  CREATE(lbits)(&zghz3711);
  CREATE(sail_int)(&zghz3712);
  CREATE(lbits)(&zghz3713);
  CREATE(sail_int)(&zghz3714);
  CREATE(lbits)(&zghz3715);
  CREATE(sail_int)(&zghz3716);
  CREATE(lbits)(&zghz3717);
  CREATE(sail_int)(&zghz3718);
  CREATE(lbits)(&zghz3719);
  CREATE(sail_int)(&zghz3720);
  CREATE(lbits)(&zghz3721);
  CREATE(sail_int)(&zghz3722);
  CREATE(lbits)(&zghz3723);
  CREATE(sail_int)(&zghz3724);
  CREATE(lbits)(&zghz3725);
  CREATE(sail_int)(&zghz3726);
  CREATE(lbits)(&zghz3727);
  CREATE(sail_int)(&zghz3728);
  CREATE(lbits)(&zghz3729);
  CREATE(sail_int)(&zghz3730);
  CREATE(lbits)(&zghz3731);
  CREATE(sail_int)(&zghz3732);
  CREATE(lbits)(&zghz3733);
  CREATE(sail_int)(&zghz3734);
  CREATE(lbits)(&zghz3735);
  CREATE(sail_int)(&zghz3736);
  CREATE(lbits)(&zghz3737);
  CREATE(sail_int)(&zghz3738);
  CREATE(lbits)(&zghz3739);
  CREATE(sail_int)(&zghz3740);
  CREATE(lbits)(&zghz3741);
  CREATE(sail_int)(&zghz3742);
  CREATE(lbits)(&zghz3743);
  CREATE(sail_int)(&zghz3744);
  CREATE(lbits)(&zghz3745);
  CREATE(sail_int)(&zghz3746);
  CREATE(lbits)(&zghz3747);
  CREATE(sail_int)(&zghz3748);
  CREATE(lbits)(&zghz3749);
  CREATE(sail_int)(&zghz3750);
  CREATE(lbits)(&zghz3751);
  CREATE(sail_int)(&zghz3752);
  CREATE(lbits)(&zghz3753);
  CREATE(sail_int)(&zghz3754);
  CREATE(lbits)(&zghz3755);
  CREATE(sail_int)(&zghz3756);
  CREATE(lbits)(&zghz3757);
  CREATE(sail_int)(&zghz3758);
  CREATE(lbits)(&zghz3759);
  CREATE(sail_int)(&zghz3760);
  CREATE(lbits)(&zghz3761);
  CREATE(sail_int)(&zghz3762);
  CREATE(lbits)(&zghz3763);
  CREATE(sail_int)(&zghz3764);
  CREATE(lbits)(&zghz3765);
  CREATE(sail_int)(&zghz3766);
  CREATE(lbits)(&zghz3767);
  CREATE(sail_int)(&zghz3768);
  CREATE(lbits)(&zghz3769);
  CREATE(sail_int)(&zghz3770);
  CREATE(lbits)(&zghz3771);
  CREATE(sail_int)(&zghz3772);
  CREATE(lbits)(&zghz3773);
  CREATE(sail_int)(&zghz3774);
  CREATE(lbits)(&zghz3775);
  CREATE(sail_int)(&zghz3776);
  CREATE(lbits)(&zghz3777);
  CREATE(sail_int)(&zghz3778);
  CREATE(lbits)(&zghz3779);
  CREATE(sail_int)(&zghz3780);
  CREATE(lbits)(&zghz3781);
  CREATE(sail_int)(&zghz3782);
  CREATE(lbits)(&zghz3783);
  CREATE(sail_int)(&zghz3784);
  CREATE(lbits)(&zghz3785);
  CREATE(sail_int)(&zghz3786);
  CREATE(lbits)(&zghz3787);
  CREATE(sail_int)(&zghz3788);
  CREATE(lbits)(&zghz3789);
  CREATE(sail_int)(&zghz3790);
  CREATE(lbits)(&zghz3791);
  CREATE(sail_int)(&zghz3792);
  CREATE(lbits)(&zghz3793);
  CREATE(sail_int)(&zghz3794);
  CREATE(lbits)(&zghz3795);
  CREATE(sail_int)(&zghz3796);
  CREATE(lbits)(&zghz3797);
  CREATE(sail_int)(&zghz3798);
  CREATE(lbits)(&zghz3799);
  CREATE(sail_int)(&zghz3800);
  CREATE(lbits)(&zghz3801);
  CREATE(sail_int)(&zghz3802);
  CREATE(lbits)(&zghz3803);
  CREATE(sail_int)(&zghz3804);
  CREATE(lbits)(&zghz3805);
  CREATE(sail_int)(&zghz3806);
  CREATE(lbits)(&zghz3807);
  CREATE(sail_int)(&zghz3808);
  CREATE(lbits)(&zghz3809);
  CREATE(sail_int)(&zghz3810);
  CREATE(lbits)(&zghz3811);
  CREATE(sail_int)(&zghz3812);
  CREATE(lbits)(&zghz3813);
  CREATE(sail_int)(&zghz3814);
  CREATE(lbits)(&zghz3815);
  CREATE(sail_int)(&zghz3816);
  CREATE(lbits)(&zghz3817);
  CREATE(sail_int)(&zghz3818);
  CREATE(lbits)(&zghz3819);
  CREATE(sail_int)(&zghz3820);
  CREATE(lbits)(&zghz3821);
  CREATE(sail_int)(&zghz3822);
  CREATE(lbits)(&zghz3823);
  CREATE(sail_int)(&zghz3824);
  CREATE(lbits)(&zghz3825);
  CREATE(sail_int)(&zghz3826);
  CREATE(lbits)(&zghz3827);
  CREATE(sail_int)(&zghz3828);
  CREATE(lbits)(&zghz3829);
  CREATE(sail_int)(&zghz3830);
  CREATE(lbits)(&zghz3831);
  CREATE(sail_int)(&zghz3832);
  CREATE(lbits)(&zghz3833);
  CREATE(sail_int)(&zghz3834);
  CREATE(lbits)(&zghz3835);
  CREATE(sail_int)(&zghz3836);
  CREATE(lbits)(&zghz3837);
  CREATE(sail_int)(&zghz3838);
  CREATE(lbits)(&zghz3839);
  CREATE(sail_int)(&zghz3840);
  CREATE(lbits)(&zghz3841);
  CREATE(sail_int)(&zghz3842);
  CREATE(lbits)(&zghz3843);
  CREATE(sail_int)(&zghz3844);
  CREATE(lbits)(&zghz3845);
  CREATE(sail_int)(&zghz3846);
  CREATE(lbits)(&zghz3847);
  CREATE(sail_int)(&zghz3848);
  CREATE(lbits)(&zghz3849);
  CREATE(sail_int)(&zghz3850);
  CREATE(lbits)(&zghz3851);
  CREATE(sail_int)(&zghz3852);
  CREATE(lbits)(&zghz3853);
  CREATE(sail_int)(&zghz3854);
  CREATE(lbits)(&zghz3855);
  CREATE(sail_int)(&zghz3856);
  CREATE(lbits)(&zghz3857);
  CREATE(sail_int)(&zghz3858);
  CREATE(lbits)(&zghz3859);
  CREATE(sail_int)(&zghz3860);
  CREATE(lbits)(&zghz3861);
  CREATE(sail_int)(&zghz3862);
  CREATE(lbits)(&zghz3863);
  CREATE(sail_int)(&zghz3864);
  CREATE(lbits)(&zghz3865);
  CREATE(sail_int)(&zghz3866);
  CREATE(lbits)(&zghz3867);
  CREATE(sail_int)(&zghz3868);
  CREATE(lbits)(&zghz3869);
  CREATE(sail_int)(&zghz3870);
  CREATE(lbits)(&zghz3871);
  CREATE(sail_int)(&zghz3872);
  CREATE(sail_int)(&zghz3873);
  CREATE(lbits)(&zghz3874);
  CREATE(sail_int)(&zghz3875);
  CREATE(lbits)(&zghz3876);
  CREATE(sail_int)(&zghz3877);
  CREATE(lbits)(&zghz3878);
  CREATE(sail_int)(&zghz3879);
  CREATE(sail_int)(&zghz3880);
  CREATE(lbits)(&zghz3881);
  CREATE(sail_int)(&zghz3882);
  CREATE(sail_int)(&zghz3883);
  CREATE(lbits)(&zghz3884);
  CREATE(sail_int)(&zghz3885);
  CREATE(lbits)(&zghz3886);
  CREATE(sail_int)(&zghz3887);
  CREATE(lbits)(&zghz3888);
  CREATE(sail_int)(&zghz3889);
  CREATE(lbits)(&zghz3890);
  CREATE(sail_int)(&zghz3891);
  CREATE(lbits)(&zghz3892);
  CREATE(sail_int)(&zghz3893);
  CREATE(lbits)(&zghz3894);
  CREATE(sail_int)(&zghz3895);
  CREATE(lbits)(&zghz3896);
  CREATE(sail_int)(&zghz3897);
  CREATE(lbits)(&zghz3898);
  CREATE(sail_int)(&zghz3899);
  CREATE(lbits)(&zghz3900);
  CREATE(sail_int)(&zghz3901);
  CREATE(lbits)(&zghz3902);
  CREATE(sail_int)(&zghz3903);
  CREATE(lbits)(&zghz3904);
  CREATE(sail_int)(&zghz3905);
  CREATE(lbits)(&zghz3906);
  CREATE(sail_int)(&zghz3907);
  CREATE(lbits)(&zghz3908);
  CREATE(sail_int)(&zghz3909);
  CREATE(lbits)(&zghz3910);
  CREATE(sail_int)(&zghz3911);
  CREATE(lbits)(&zghz3912);
  CREATE(sail_int)(&zghz3913);
  CREATE(lbits)(&zghz3914);
  CREATE(sail_int)(&zghz3915);
  CREATE(lbits)(&zghz3916);
  CREATE(sail_int)(&zghz3917);
  CREATE(lbits)(&zghz3918);
  CREATE(sail_int)(&zghz3919);
  CREATE(lbits)(&zghz3920);
  CREATE(sail_int)(&zghz3921);
  CREATE(lbits)(&zghz3922);
  CREATE(sail_int)(&zghz3923);
  CREATE(lbits)(&zghz3924);
  CREATE(sail_int)(&zghz3925);
  CREATE(lbits)(&zghz3926);
  CREATE(sail_int)(&zghz3927);
  CREATE(lbits)(&zghz3928);
  CREATE(sail_int)(&zghz3929);
  CREATE(lbits)(&zghz3930);
  CREATE(sail_int)(&zghz3931);
  CREATE(lbits)(&zghz3932);
  CREATE(sail_int)(&zghz3933);
  CREATE(lbits)(&zghz3934);
  CREATE(sail_int)(&zghz3935);
  CREATE(lbits)(&zghz3936);
  CREATE(sail_int)(&zghz3937);
  CREATE(lbits)(&zghz3938);
  CREATE(sail_int)(&zghz3939);
  CREATE(sail_int)(&zghz3940);
  CREATE(lbits)(&zghz3941);
  CREATE(sail_int)(&zghz3942);
  CREATE(sail_int)(&zghz3943);
  CREATE(lbits)(&zghz3944);
  CREATE(sail_int)(&zghz3945);
  CREATE(sail_int)(&zghz3946);
  CREATE(lbits)(&zghz3947);
  CREATE(sail_int)(&zghz3948);
  CREATE(sail_int)(&zghz3949);
  CREATE(lbits)(&zghz3950);
  CREATE(sail_int)(&zghz3951);
  CREATE(sail_int)(&zghz3952);
  CREATE(lbits)(&zghz3953);
  CREATE(sail_int)(&zghz3954);
  CREATE(lbits)(&zghz3955);
  CREATE(sail_int)(&zghz3956);
  CREATE(lbits)(&zghz3957);
  CREATE(sail_int)(&zghz3958);
  CREATE(lbits)(&zghz3959);
  CREATE(sail_int)(&zghz3960);
  CREATE(lbits)(&zghz3961);
  CREATE(sail_int)(&zghz3962);
  CREATE(lbits)(&zghz3963);
  CREATE(sail_int)(&zghz3964);
  CREATE(sail_int)(&zghz3965);
  CREATE(lbits)(&zghz3966);
  CREATE(sail_int)(&zghz3967);
  CREATE(lbits)(&zghz3968);
  CREATE(sail_int)(&zghz3969);
  CREATE(lbits)(&zghz3970);
  CREATE(sail_int)(&zghz3971);
  CREATE(lbits)(&zghz3972);
  CREATE(sail_int)(&zghz3973);
  CREATE(lbits)(&zghz3974);
  CREATE(sail_int)(&zghz3975);
  CREATE(lbits)(&zghz3976);
  CREATE(sail_int)(&zghz3977);
  CREATE(lbits)(&zghz3978);
  CREATE(sail_int)(&zghz3979);
  CREATE(lbits)(&zghz3980);
  CREATE(sail_int)(&zghz3981);
  CREATE(lbits)(&zghz3982);
  CREATE(sail_int)(&zghz3983);
  CREATE(lbits)(&zghz3984);
  CREATE(sail_int)(&zghz3985);
  CREATE(lbits)(&zghz3986);
  CREATE(sail_int)(&zghz3987);
  CREATE(lbits)(&zghz3988);
  CREATE(sail_int)(&zghz3989);
  CREATE(lbits)(&zghz3990);
  CREATE(sail_int)(&zghz3991);
  CREATE(lbits)(&zghz3992);
  CREATE(sail_int)(&zghz3993);
  CREATE(lbits)(&zghz3994);
  CREATE(sail_int)(&zghz3995);
  CREATE(lbits)(&zghz3996);
  CREATE(sail_int)(&zghz3997);
  CREATE(lbits)(&zghz3998);
  CREATE(sail_int)(&zghz3999);
  CREATE(sail_int)(&zghz31000);
  CREATE(lbits)(&zghz31001);
  CREATE(sail_int)(&zghz31002);
  CREATE(sail_int)(&zghz31003);
  CREATE(lbits)(&zghz31004);
  CREATE(sail_int)(&zghz31005);
  CREATE(lbits)(&zghz31006);
  CREATE(sail_int)(&zghz31007);
  CREATE(sail_int)(&zghz31008);
  CREATE(lbits)(&zghz31009);
  CREATE(sail_int)(&zghz31010);
  CREATE(lbits)(&zghz31011);
  CREATE(sail_int)(&zghz31012);
  CREATE(lbits)(&zghz31013);
  CREATE(sail_int)(&zghz31014);
  CREATE(lbits)(&zghz31015);
  CREATE(sail_int)(&zghz31016);
  CREATE(sail_int)(&zghz31017);
  CREATE(lbits)(&zghz31018);
  CREATE(sail_int)(&zghz31019);
  CREATE(lbits)(&zghz31020);
  CREATE(sail_int)(&zghz31021);
  CREATE(lbits)(&zghz31022);
  CREATE(sail_int)(&zghz31023);
  CREATE(lbits)(&zghz31024);
  CREATE(sail_int)(&zghz31025);
  CREATE(lbits)(&zghz31026);
  CREATE(sail_int)(&zghz31027);
  CREATE(sail_int)(&zghz31028);
  CREATE(sail_int)(&zghz31029);
  CREATE(lbits)(&zghz31030);
  CREATE(sail_int)(&zghz31031);
  CREATE(lbits)(&zghz31032);
  CREATE(sail_int)(&zghz31033);
  CREATE(lbits)(&zghz31034);
  CREATE(sail_int)(&zghz31035);
  CREATE(lbits)(&zghz31036);
  CREATE(sail_int)(&zghz31037);
  CREATE(lbits)(&zghz31038);
  CREATE(sail_int)(&zghz31039);
  CREATE(lbits)(&zghz31040);
  CREATE(sail_int)(&zghz31041);
  CREATE(lbits)(&zghz31042);
  CREATE(sail_int)(&zghz31043);
  CREATE(lbits)(&zghz31044);
  CREATE(sail_int)(&zghz31045);
  CREATE(lbits)(&zghz31046);
  CREATE(sail_int)(&zghz31047);
  CREATE(lbits)(&zghz31048);
  CREATE(sail_int)(&zghz31049);
  CREATE(lbits)(&zghz31050);
  CREATE(sail_int)(&zghz31051);
  CREATE(lbits)(&zghz31052);
  CREATE(sail_int)(&zghz31053);
  CREATE(lbits)(&zghz31054);
  CREATE(sail_int)(&zghz31055);
  CREATE(lbits)(&zghz31056);
  CREATE(sail_int)(&zghz31057);
  CREATE(lbits)(&zghz31058);
  CREATE(sail_int)(&zghz31059);
  CREATE(lbits)(&zghz31060);
  CREATE(sail_int)(&zghz31061);
  CREATE(lbits)(&zghz31062);
  CREATE(sail_int)(&zghz31063);
  CREATE(lbits)(&zghz31064);
  CREATE(sail_int)(&zghz31065);
  CREATE(lbits)(&zghz31066);
  CREATE(sail_int)(&zghz31067);
  CREATE(lbits)(&zghz31068);
  CREATE(sail_int)(&zghz31069);
  CREATE(lbits)(&zghz31070);
  CREATE(sail_int)(&zghz31071);
  CREATE(lbits)(&zghz31072);
  CREATE(sail_int)(&zghz31073);
  CREATE(lbits)(&zghz31074);
  CREATE(sail_int)(&zghz31075);
  CREATE(lbits)(&zghz31076);
  CREATE(sail_int)(&zghz31077);
  CREATE(lbits)(&zghz31078);
  CREATE(sail_int)(&zghz31079);
  CREATE(lbits)(&zghz31080);
  CREATE(sail_int)(&zghz31081);
  CREATE(lbits)(&zghz31082);
  CREATE(sail_int)(&zghz31083);
  CREATE(lbits)(&zghz31084);
  CREATE(sail_int)(&zghz31085);
  CREATE(lbits)(&zghz31086);
  CREATE(sail_int)(&zghz31087);
  CREATE(lbits)(&zghz31088);
  CREATE(sail_int)(&zghz31089);
  CREATE(lbits)(&zghz31090);
  CREATE(sail_int)(&zghz31091);
  CREATE(lbits)(&zghz31092);
  CREATE(sail_int)(&zghz31093);
  CREATE(lbits)(&zghz31094);
  CREATE(sail_int)(&zghz31095);
  CREATE(lbits)(&zghz31096);
  CREATE(sail_int)(&zghz31097);
  CREATE(lbits)(&zghz31098);
  CREATE(sail_int)(&zghz31099);
  CREATE(lbits)(&zghz31100);
  CREATE(sail_int)(&zghz31101);
  CREATE(lbits)(&zghz31102);
  CREATE(sail_int)(&zghz31103);
  CREATE(lbits)(&zghz31104);
  CREATE(sail_int)(&zghz31105);
  CREATE(lbits)(&zghz31106);
  CREATE(sail_int)(&zghz31107);
  CREATE(lbits)(&zghz31108);
  CREATE(sail_int)(&zghz31109);
  CREATE(lbits)(&zghz31110);
  CREATE(sail_int)(&zghz31111);
  CREATE(lbits)(&zghz31112);
  CREATE(sail_int)(&zghz31113);
  CREATE(lbits)(&zghz31114);
  CREATE(sail_int)(&zghz31115);
  CREATE(lbits)(&zghz31116);
  CREATE(sail_int)(&zghz31117);
  CREATE(lbits)(&zghz31118);
  CREATE(sail_int)(&zghz31119);
  CREATE(lbits)(&zghz31120);
  CREATE(sail_int)(&zghz31121);
  CREATE(lbits)(&zghz31122);
  CREATE(sail_int)(&zghz31123);
  CREATE(lbits)(&zghz31124);
  CREATE(sail_int)(&zghz31125);
  CREATE(lbits)(&zghz31126);
  CREATE(sail_int)(&zghz31127);
  CREATE(lbits)(&zghz31128);
  CREATE(sail_int)(&zghz31129);
  CREATE(lbits)(&zghz31130);
  CREATE(sail_int)(&zghz31131);
  CREATE(lbits)(&zghz31132);
  CREATE(sail_int)(&zghz31133);
  CREATE(lbits)(&zghz31134);
  CREATE(sail_int)(&zghz31135);
  CREATE(lbits)(&zghz31136);
  CREATE(sail_int)(&zghz31137);
  CREATE(lbits)(&zghz31138);
  CREATE(sail_int)(&zghz31139);
  CREATE(lbits)(&zghz31140);
  CREATE(sail_int)(&zghz31141);
  CREATE(lbits)(&zghz31142);
  CREATE(sail_int)(&zghz31143);
  CREATE(sail_int)(&zghz31144);
  CREATE(sail_int)(&zghz31145);
  CREATE(sail_int)(&zghz31146);
  CREATE(sail_int)(&zghz31147);
  CREATE(sail_int)(&zghz31148);
  CREATE(sail_int)(&zghz31149);
  CREATE(lbits)(&zghz31150);
  CREATE(sail_int)(&zghz31151);
  CREATE(lbits)(&zghz31152);
  CREATE(sail_int)(&zghz31153);
  CREATE(lbits)(&zghz31154);
  CREATE(sail_int)(&zghz31155);
  CREATE(lbits)(&zghz31156);
  CREATE(sail_int)(&zghz31157);
  CREATE(lbits)(&zghz31158);
  CREATE(sail_int)(&zghz31159);
  CREATE(lbits)(&zghz31160);
  CREATE(sail_int)(&zghz31161);
  CREATE(lbits)(&zghz31162);
  CREATE(sail_int)(&zghz31163);
  CREATE(lbits)(&zghz31164);
  CREATE(sail_int)(&zghz31165);
  CREATE(lbits)(&zghz31166);
  CREATE(sail_int)(&zghz31167);
  CREATE(lbits)(&zghz31168);
  CREATE(sail_int)(&zghz31169);
  CREATE(lbits)(&zghz31170);
  CREATE(sail_int)(&zghz31171);
  CREATE(lbits)(&zghz31172);
  CREATE(sail_int)(&zghz31173);
  CREATE(lbits)(&zghz31174);
  CREATE(sail_int)(&zghz31175);
  CREATE(lbits)(&zghz31176);
  CREATE(sail_int)(&zghz31177);
  CREATE(lbits)(&zghz31178);
  CREATE(sail_int)(&zghz31179);
  CREATE(sail_int)(&zghz31180);
  CREATE(sail_int)(&zghz31181);
  CREATE(sail_int)(&zghz31182);
  CREATE(sail_int)(&zghz31183);
  CREATE(sail_int)(&zghz31184);
  CREATE(sail_int)(&zghz31185);
  CREATE(sail_int)(&zghz31186);
  CREATE(sail_int)(&zghz31187);
  CREATE(sail_int)(&zghz31188);
  CREATE(sail_int)(&zghz31189);
  CREATE(sail_int)(&zghz31190);
  CREATE(sail_int)(&zghz31191);
  CREATE(sail_int)(&zghz31192);
  CREATE(sail_int)(&zghz31193);
  CREATE(sail_int)(&zghz31194);
  CREATE(sail_int)(&zghz31195);
  CREATE(sail_int)(&zghz31196);
  CREATE(sail_int)(&zghz31197);
  CREATE(sail_int)(&zghz31198);
  CREATE(sail_int)(&zghz31199);
  CREATE(sail_int)(&zghz31200);
  CREATE(sail_int)(&zghz31201);
  CREATE(sail_int)(&zghz31202);
  CREATE(sail_int)(&zghz31203);
  CREATE(sail_int)(&zghz31204);
  CREATE(sail_int)(&zghz31205);
  CREATE(sail_int)(&zghz31206);
  CREATE(sail_int)(&zghz31207);
  CREATE(sail_int)(&zghz31208);
  CREATE(sail_int)(&zghz31209);
  CREATE(sail_int)(&zghz31210);
  CREATE(lbits)(&zghz31211);
  CREATE(sail_int)(&zghz31212);
  CREATE(lbits)(&zghz31213);
  CREATE(sail_int)(&zghz31214);
  CREATE(lbits)(&zghz31215);
  CREATE(sail_int)(&zghz31216);
  CREATE(lbits)(&zghz31217);
  CREATE(sail_int)(&zghz31218);
  CREATE(lbits)(&zghz31219);
  CREATE(sail_int)(&zghz31220);
  CREATE(lbits)(&zghz31221);
  CREATE(sail_int)(&zghz31222);
  CREATE(lbits)(&zghz31223);
  CREATE(sail_int)(&zghz31224);
  CREATE(lbits)(&zghz31225);
  CREATE(sail_int)(&zghz31226);
}

















































static unit sailgen_initializze_registers(unit zgsz3196)
{
  __label__ end_function_263, end_block_exception_264;

  unit zcbz335;
  {
    undefined_int(&zSEE, UNIT);
    unit zgsz3554;
    zgsz3554 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31226);
      CONVERT_OF(sail_int, mach_int)(&zghz31226, INT64_C(129));
      UNDEFINED(lbits)(&zPCC, zghz31226);
    }
    unit zgsz3553;
    zgsz3553 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31224);
      CONVERT_OF(sail_int, mach_int)(&zghz31224, INT64_C(32));
      RECREATE(lbits)(&zghz31225);
      UNDEFINED(lbits)(&zghz31225, zghz31224);
      zCNTHPS_CTL_EL2 = CONVERT_OF(fbits, lbits)(zghz31225, true);
    }
    unit zgsz3552;
    zgsz3552 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31222);
      CONVERT_OF(sail_int, mach_int)(&zghz31222, INT64_C(64));
      RECREATE(lbits)(&zghz31223);
      UNDEFINED(lbits)(&zghz31223, zghz31222);
      zCNTHPS_CVAL_EL2 = CONVERT_OF(fbits, lbits)(zghz31223, true);
    }
    unit zgsz3551;
    zgsz3551 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31220);
      CONVERT_OF(sail_int, mach_int)(&zghz31220, INT64_C(32));
      RECREATE(lbits)(&zghz31221);
      UNDEFINED(lbits)(&zghz31221, zghz31220);
      zCNTHPS_TVAL_EL2 = CONVERT_OF(fbits, lbits)(zghz31221, true);
    }
    unit zgsz3550;
    zgsz3550 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31218);
      CONVERT_OF(sail_int, mach_int)(&zghz31218, INT64_C(32));
      RECREATE(lbits)(&zghz31219);
      UNDEFINED(lbits)(&zghz31219, zghz31218);
      zCNTHVS_CTL_EL2 = CONVERT_OF(fbits, lbits)(zghz31219, true);
    }
    unit zgsz3549;
    zgsz3549 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31216);
      CONVERT_OF(sail_int, mach_int)(&zghz31216, INT64_C(64));
      RECREATE(lbits)(&zghz31217);
      UNDEFINED(lbits)(&zghz31217, zghz31216);
      zCNTHVS_CVAL_EL2 = CONVERT_OF(fbits, lbits)(zghz31217, true);
    }
    unit zgsz3548;
    zgsz3548 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31214);
      CONVERT_OF(sail_int, mach_int)(&zghz31214, INT64_C(32));
      RECREATE(lbits)(&zghz31215);
      UNDEFINED(lbits)(&zghz31215, zghz31214);
      zCNTHVS_TVAL_EL2 = CONVERT_OF(fbits, lbits)(zghz31215, true);
    }
    unit zgsz3547;
    zgsz3547 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31212);
      CONVERT_OF(sail_int, mach_int)(&zghz31212, INT64_C(32));
      RECREATE(lbits)(&zghz31213);
      UNDEFINED(lbits)(&zghz31213, zghz31212);
      zSCR_EL3 = CONVERT_OF(fbits, lbits)(zghz31213, true);
    }
    unit zgsz3546;
    zgsz3546 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31210);
      CONVERT_OF(sail_int, mach_int)(&zghz31210, INT64_C(64));
      RECREATE(lbits)(&zghz31211);
      UNDEFINED(lbits)(&zghz31211, zghz31210);
      zHCR_EL2 = CONVERT_OF(fbits, lbits)(zghz31211, true);
    }
    unit zgsz3545;
    zgsz3545 = UNIT;
  }
  {
    zPSTATE = sailgen_undefined_ProcState(UNIT);
    unit zgsz3544;
    zgsz3544 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31209);
      CONVERT_OF(sail_int, mach_int)(&zghz31209, INT64_C(129));
      UNDEFINED(lbits)(&z_R00, zghz31209);
    }
    unit zgsz3543;
    zgsz3543 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31208);
      CONVERT_OF(sail_int, mach_int)(&zghz31208, INT64_C(129));
      UNDEFINED(lbits)(&z_R01, zghz31208);
    }
    unit zgsz3542;
    zgsz3542 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31207);
      CONVERT_OF(sail_int, mach_int)(&zghz31207, INT64_C(129));
      UNDEFINED(lbits)(&z_R02, zghz31207);
    }
    unit zgsz3541;
    zgsz3541 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31206);
      CONVERT_OF(sail_int, mach_int)(&zghz31206, INT64_C(129));
      UNDEFINED(lbits)(&z_R03, zghz31206);
    }
    unit zgsz3540;
    zgsz3540 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31205);
      CONVERT_OF(sail_int, mach_int)(&zghz31205, INT64_C(129));
      UNDEFINED(lbits)(&z_R04, zghz31205);
    }
    unit zgsz3539;
    zgsz3539 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31204);
      CONVERT_OF(sail_int, mach_int)(&zghz31204, INT64_C(129));
      UNDEFINED(lbits)(&z_R05, zghz31204);
    }
    unit zgsz3538;
    zgsz3538 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31203);
      CONVERT_OF(sail_int, mach_int)(&zghz31203, INT64_C(129));
      UNDEFINED(lbits)(&z_R06, zghz31203);
    }
    unit zgsz3537;
    zgsz3537 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31202);
      CONVERT_OF(sail_int, mach_int)(&zghz31202, INT64_C(129));
      UNDEFINED(lbits)(&z_R07, zghz31202);
    }
    unit zgsz3536;
    zgsz3536 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31201);
      CONVERT_OF(sail_int, mach_int)(&zghz31201, INT64_C(129));
      UNDEFINED(lbits)(&z_R08, zghz31201);
    }
    unit zgsz3535;
    zgsz3535 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31200);
      CONVERT_OF(sail_int, mach_int)(&zghz31200, INT64_C(129));
      UNDEFINED(lbits)(&z_R09, zghz31200);
    }
    unit zgsz3534;
    zgsz3534 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31199);
      CONVERT_OF(sail_int, mach_int)(&zghz31199, INT64_C(129));
      UNDEFINED(lbits)(&z_R10, zghz31199);
    }
    unit zgsz3533;
    zgsz3533 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31198);
      CONVERT_OF(sail_int, mach_int)(&zghz31198, INT64_C(129));
      UNDEFINED(lbits)(&z_R11, zghz31198);
    }
    unit zgsz3532;
    zgsz3532 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31197);
      CONVERT_OF(sail_int, mach_int)(&zghz31197, INT64_C(129));
      UNDEFINED(lbits)(&z_R12, zghz31197);
    }
    unit zgsz3531;
    zgsz3531 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31196);
      CONVERT_OF(sail_int, mach_int)(&zghz31196, INT64_C(129));
      UNDEFINED(lbits)(&z_R13, zghz31196);
    }
    unit zgsz3530;
    zgsz3530 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31195);
      CONVERT_OF(sail_int, mach_int)(&zghz31195, INT64_C(129));
      UNDEFINED(lbits)(&z_R14, zghz31195);
    }
    unit zgsz3529;
    zgsz3529 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31194);
      CONVERT_OF(sail_int, mach_int)(&zghz31194, INT64_C(129));
      UNDEFINED(lbits)(&z_R15, zghz31194);
    }
    unit zgsz3528;
    zgsz3528 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31193);
      CONVERT_OF(sail_int, mach_int)(&zghz31193, INT64_C(129));
      UNDEFINED(lbits)(&z_R16, zghz31193);
    }
    unit zgsz3527;
    zgsz3527 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31192);
      CONVERT_OF(sail_int, mach_int)(&zghz31192, INT64_C(129));
      UNDEFINED(lbits)(&z_R17, zghz31192);
    }
    unit zgsz3526;
    zgsz3526 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31191);
      CONVERT_OF(sail_int, mach_int)(&zghz31191, INT64_C(129));
      UNDEFINED(lbits)(&z_R18, zghz31191);
    }
    unit zgsz3525;
    zgsz3525 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31190);
      CONVERT_OF(sail_int, mach_int)(&zghz31190, INT64_C(129));
      UNDEFINED(lbits)(&z_R19, zghz31190);
    }
    unit zgsz3524;
    zgsz3524 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31189);
      CONVERT_OF(sail_int, mach_int)(&zghz31189, INT64_C(129));
      UNDEFINED(lbits)(&z_R20, zghz31189);
    }
    unit zgsz3523;
    zgsz3523 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31188);
      CONVERT_OF(sail_int, mach_int)(&zghz31188, INT64_C(129));
      UNDEFINED(lbits)(&z_R21, zghz31188);
    }
    unit zgsz3522;
    zgsz3522 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31187);
      CONVERT_OF(sail_int, mach_int)(&zghz31187, INT64_C(129));
      UNDEFINED(lbits)(&z_R22, zghz31187);
    }
    unit zgsz3521;
    zgsz3521 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31186);
      CONVERT_OF(sail_int, mach_int)(&zghz31186, INT64_C(129));
      UNDEFINED(lbits)(&z_R23, zghz31186);
    }
    unit zgsz3520;
    zgsz3520 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31185);
      CONVERT_OF(sail_int, mach_int)(&zghz31185, INT64_C(129));
      UNDEFINED(lbits)(&z_R24, zghz31185);
    }
    unit zgsz3519;
    zgsz3519 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31184);
      CONVERT_OF(sail_int, mach_int)(&zghz31184, INT64_C(129));
      UNDEFINED(lbits)(&z_R25, zghz31184);
    }
    unit zgsz3518;
    zgsz3518 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31183);
      CONVERT_OF(sail_int, mach_int)(&zghz31183, INT64_C(129));
      UNDEFINED(lbits)(&z_R26, zghz31183);
    }
    unit zgsz3517;
    zgsz3517 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31182);
      CONVERT_OF(sail_int, mach_int)(&zghz31182, INT64_C(129));
      UNDEFINED(lbits)(&z_R27, zghz31182);
    }
    unit zgsz3516;
    zgsz3516 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31181);
      CONVERT_OF(sail_int, mach_int)(&zghz31181, INT64_C(129));
      UNDEFINED(lbits)(&z_R28, zghz31181);
    }
    unit zgsz3515;
    zgsz3515 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31180);
      CONVERT_OF(sail_int, mach_int)(&zghz31180, INT64_C(129));
      UNDEFINED(lbits)(&z_R29, zghz31180);
    }
    unit zgsz3514;
    zgsz3514 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31179);
      CONVERT_OF(sail_int, mach_int)(&zghz31179, INT64_C(129));
      UNDEFINED(lbits)(&z_R30, zghz31179);
    }
    unit zgsz3513;
    zgsz3513 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31177);
      CONVERT_OF(sail_int, mach_int)(&zghz31177, INT64_C(32));
      RECREATE(lbits)(&zghz31178);
      UNDEFINED(lbits)(&zghz31178, zghz31177);
      zESR_EL1 = CONVERT_OF(fbits, lbits)(zghz31178, true);
    }
    unit zgsz3512;
    zgsz3512 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31175);
      CONVERT_OF(sail_int, mach_int)(&zghz31175, INT64_C(32));
      RECREATE(lbits)(&zghz31176);
      UNDEFINED(lbits)(&zghz31176, zghz31175);
      zESR_EL2 = CONVERT_OF(fbits, lbits)(zghz31176, true);
    }
    unit zgsz3511;
    zgsz3511 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31173);
      CONVERT_OF(sail_int, mach_int)(&zghz31173, INT64_C(32));
      RECREATE(lbits)(&zghz31174);
      UNDEFINED(lbits)(&zghz31174, zghz31173);
      zESR_EL3 = CONVERT_OF(fbits, lbits)(zghz31174, true);
    }
    unit zgsz3510;
    zgsz3510 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31171);
      CONVERT_OF(sail_int, mach_int)(&zghz31171, INT64_C(64));
      RECREATE(lbits)(&zghz31172);
      UNDEFINED(lbits)(&zghz31172, zghz31171);
      zFAR_EL1 = CONVERT_OF(fbits, lbits)(zghz31172, true);
    }
    unit zgsz3509;
    zgsz3509 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31169);
      CONVERT_OF(sail_int, mach_int)(&zghz31169, INT64_C(64));
      RECREATE(lbits)(&zghz31170);
      UNDEFINED(lbits)(&zghz31170, zghz31169);
      zFAR_EL2 = CONVERT_OF(fbits, lbits)(zghz31170, true);
    }
    unit zgsz3508;
    zgsz3508 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31167);
      CONVERT_OF(sail_int, mach_int)(&zghz31167, INT64_C(64));
      RECREATE(lbits)(&zghz31168);
      UNDEFINED(lbits)(&zghz31168, zghz31167);
      zFAR_EL3 = CONVERT_OF(fbits, lbits)(zghz31168, true);
    }
    unit zgsz3507;
    zgsz3507 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31165);
      CONVERT_OF(sail_int, mach_int)(&zghz31165, INT64_C(64));
      RECREATE(lbits)(&zghz31166);
      UNDEFINED(lbits)(&zghz31166, zghz31165);
      zHPFAR_EL2 = CONVERT_OF(fbits, lbits)(zghz31166, true);
    }
    unit zgsz3506;
    zgsz3506 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31163);
      CONVERT_OF(sail_int, mach_int)(&zghz31163, INT64_C(64));
      RECREATE(lbits)(&zghz31164);
      UNDEFINED(lbits)(&zghz31164, zghz31163);
      zTCR_EL1 = CONVERT_OF(fbits, lbits)(zghz31164, true);
    }
    unit zgsz3505;
    zgsz3505 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31161);
      CONVERT_OF(sail_int, mach_int)(&zghz31161, INT64_C(64));
      RECREATE(lbits)(&zghz31162);
      UNDEFINED(lbits)(&zghz31162, zghz31161);
      zTCR_EL2 = CONVERT_OF(fbits, lbits)(zghz31162, true);
    }
    unit zgsz3504;
    zgsz3504 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31159);
      CONVERT_OF(sail_int, mach_int)(&zghz31159, INT64_C(32));
      RECREATE(lbits)(&zghz31160);
      UNDEFINED(lbits)(&zghz31160, zghz31159);
      zTCR_EL3 = CONVERT_OF(fbits, lbits)(zghz31160, true);
    }
    unit zgsz3503;
    zgsz3503 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31157);
      CONVERT_OF(sail_int, mach_int)(&zghz31157, INT64_C(64));
      RECREATE(lbits)(&zghz31158);
      UNDEFINED(lbits)(&zghz31158, zghz31157);
      z_PC = CONVERT_OF(fbits, lbits)(zghz31158, true);
    }
    unit zgsz3502;
    zgsz3502 = UNIT;
  }
  {
    z__BranchTaken = false;
    unit zgsz3501;
    zgsz3501 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31155);
      CONVERT_OF(sail_int, mach_int)(&zghz31155, INT64_C(32));
      RECREATE(lbits)(&zghz31156);
      UNDEFINED(lbits)(&zghz31156, zghz31155);
      zCCTLR_EL0 = CONVERT_OF(fbits, lbits)(zghz31156, true);
    }
    unit zgsz3500;
    zgsz3500 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31153);
      CONVERT_OF(sail_int, mach_int)(&zghz31153, INT64_C(32));
      RECREATE(lbits)(&zghz31154);
      UNDEFINED(lbits)(&zghz31154, zghz31153);
      zCCTLR_EL1 = CONVERT_OF(fbits, lbits)(zghz31154, true);
    }
    unit zgsz3499;
    zgsz3499 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31151);
      CONVERT_OF(sail_int, mach_int)(&zghz31151, INT64_C(32));
      RECREATE(lbits)(&zghz31152);
      UNDEFINED(lbits)(&zghz31152, zghz31151);
      zCCTLR_EL2 = CONVERT_OF(fbits, lbits)(zghz31152, true);
    }
    unit zgsz3498;
    zgsz3498 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31149);
      CONVERT_OF(sail_int, mach_int)(&zghz31149, INT64_C(32));
      RECREATE(lbits)(&zghz31150);
      UNDEFINED(lbits)(&zghz31150, zghz31149);
      zCCTLR_EL3 = CONVERT_OF(fbits, lbits)(zghz31150, true);
    }
    unit zgsz3497;
    zgsz3497 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31148);
      CONVERT_OF(sail_int, mach_int)(&zghz31148, INT64_C(129));
      UNDEFINED(lbits)(&zELR_EL1, zghz31148);
    }
    unit zgsz3496;
    zgsz3496 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31147);
      CONVERT_OF(sail_int, mach_int)(&zghz31147, INT64_C(129));
      UNDEFINED(lbits)(&zELR_EL2, zghz31147);
    }
    unit zgsz3495;
    zgsz3495 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31146);
      CONVERT_OF(sail_int, mach_int)(&zghz31146, INT64_C(129));
      UNDEFINED(lbits)(&zELR_EL3, zghz31146);
    }
    unit zgsz3494;
    zgsz3494 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31145);
      CONVERT_OF(sail_int, mach_int)(&zghz31145, INT64_C(129));
      UNDEFINED(lbits)(&zVBAR_EL1, zghz31145);
    }
    unit zgsz3493;
    zgsz3493 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31144);
      CONVERT_OF(sail_int, mach_int)(&zghz31144, INT64_C(129));
      UNDEFINED(lbits)(&zVBAR_EL2, zghz31144);
    }
    unit zgsz3492;
    zgsz3492 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31143);
      CONVERT_OF(sail_int, mach_int)(&zghz31143, INT64_C(129));
      UNDEFINED(lbits)(&zVBAR_EL3, zghz31143);
    }
    unit zgsz3491;
    zgsz3491 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31141);
      CONVERT_OF(sail_int, mach_int)(&zghz31141, INT64_C(32));
      RECREATE(lbits)(&zghz31142);
      UNDEFINED(lbits)(&zghz31142, zghz31141);
      zCPACR_EL1 = CONVERT_OF(fbits, lbits)(zghz31142, true);
    }
    unit zgsz3490;
    zgsz3490 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31139);
      CONVERT_OF(sail_int, mach_int)(&zghz31139, INT64_C(32));
      RECREATE(lbits)(&zghz31140);
      UNDEFINED(lbits)(&zghz31140, zghz31139);
      zCPTR_EL2 = CONVERT_OF(fbits, lbits)(zghz31140, true);
    }
    unit zgsz3489;
    zgsz3489 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31137);
      CONVERT_OF(sail_int, mach_int)(&zghz31137, INT64_C(32));
      RECREATE(lbits)(&zghz31138);
      UNDEFINED(lbits)(&zghz31138, zghz31137);
      zCPTR_EL3 = CONVERT_OF(fbits, lbits)(zghz31138, true);
    }
    unit zgsz3488;
    zgsz3488 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31135);
      CONVERT_OF(sail_int, mach_int)(&zghz31135, INT64_C(32));
      RECREATE(lbits)(&zghz31136);
      UNDEFINED(lbits)(&zghz31136, zghz31135);
      zEDSCR = CONVERT_OF(fbits, lbits)(zghz31136, true);
    }
    unit zgsz3487;
    zgsz3487 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31133);
      CONVERT_OF(sail_int, mach_int)(&zghz31133, INT64_C(64));
      RECREATE(lbits)(&zghz31134);
      UNDEFINED(lbits)(&zghz31134, zghz31133);
      zSCTLR_EL1 = CONVERT_OF(fbits, lbits)(zghz31134, true);
    }
    unit zgsz3486;
    zgsz3486 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31131);
      CONVERT_OF(sail_int, mach_int)(&zghz31131, INT64_C(64));
      RECREATE(lbits)(&zghz31132);
      UNDEFINED(lbits)(&zghz31132, zghz31131);
      zSCTLR_EL2 = CONVERT_OF(fbits, lbits)(zghz31132, true);
    }
    unit zgsz3485;
    zgsz3485 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31129);
      CONVERT_OF(sail_int, mach_int)(&zghz31129, INT64_C(64));
      RECREATE(lbits)(&zghz31130);
      UNDEFINED(lbits)(&zghz31130, zghz31129);
      zSCTLR_EL3 = CONVERT_OF(fbits, lbits)(zghz31130, true);
    }
    unit zgsz3484;
    zgsz3484 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31127);
      CONVERT_OF(sail_int, mach_int)(&zghz31127, INT64_C(32));
      RECREATE(lbits)(&zghz31128);
      UNDEFINED(lbits)(&zghz31128, zghz31127);
      zSPSR_EL1 = CONVERT_OF(fbits, lbits)(zghz31128, true);
    }
    unit zgsz3483;
    zgsz3483 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31125);
      CONVERT_OF(sail_int, mach_int)(&zghz31125, INT64_C(32));
      RECREATE(lbits)(&zghz31126);
      UNDEFINED(lbits)(&zghz31126, zghz31125);
      zSPSR_EL2 = CONVERT_OF(fbits, lbits)(zghz31126, true);
    }
    unit zgsz3482;
    zgsz3482 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31123);
      CONVERT_OF(sail_int, mach_int)(&zghz31123, INT64_C(32));
      RECREATE(lbits)(&zghz31124);
      UNDEFINED(lbits)(&zghz31124, zghz31123);
      zSPSR_EL3 = CONVERT_OF(fbits, lbits)(zghz31124, true);
    }
    unit zgsz3481;
    zgsz3481 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31121);
      CONVERT_OF(sail_int, mach_int)(&zghz31121, INT64_C(64));
      RECREATE(lbits)(&zghz31122);
      UNDEFINED(lbits)(&zghz31122, zghz31121);
      zACTLR_EL1 = CONVERT_OF(fbits, lbits)(zghz31122, true);
    }
    unit zgsz3480;
    zgsz3480 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31119);
      CONVERT_OF(sail_int, mach_int)(&zghz31119, INT64_C(64));
      RECREATE(lbits)(&zghz31120);
      UNDEFINED(lbits)(&zghz31120, zghz31119);
      zACTLR_EL2 = CONVERT_OF(fbits, lbits)(zghz31120, true);
    }
    unit zgsz3479;
    zgsz3479 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31117);
      CONVERT_OF(sail_int, mach_int)(&zghz31117, INT64_C(64));
      RECREATE(lbits)(&zghz31118);
      UNDEFINED(lbits)(&zghz31118, zghz31117);
      zACTLR_EL3 = CONVERT_OF(fbits, lbits)(zghz31118, true);
    }
    unit zgsz3478;
    zgsz3478 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31115);
      CONVERT_OF(sail_int, mach_int)(&zghz31115, INT64_C(64));
      RECREATE(lbits)(&zghz31116);
      UNDEFINED(lbits)(&zghz31116, zghz31115);
      zAFSR0_EL1 = CONVERT_OF(fbits, lbits)(zghz31116, true);
    }
    unit zgsz3477;
    zgsz3477 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31113);
      CONVERT_OF(sail_int, mach_int)(&zghz31113, INT64_C(64));
      RECREATE(lbits)(&zghz31114);
      UNDEFINED(lbits)(&zghz31114, zghz31113);
      zAFSR0_EL2 = CONVERT_OF(fbits, lbits)(zghz31114, true);
    }
    unit zgsz3476;
    zgsz3476 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31111);
      CONVERT_OF(sail_int, mach_int)(&zghz31111, INT64_C(64));
      RECREATE(lbits)(&zghz31112);
      UNDEFINED(lbits)(&zghz31112, zghz31111);
      zAFSR0_EL3 = CONVERT_OF(fbits, lbits)(zghz31112, true);
    }
    unit zgsz3475;
    zgsz3475 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31109);
      CONVERT_OF(sail_int, mach_int)(&zghz31109, INT64_C(64));
      RECREATE(lbits)(&zghz31110);
      UNDEFINED(lbits)(&zghz31110, zghz31109);
      zAFSR1_EL1 = CONVERT_OF(fbits, lbits)(zghz31110, true);
    }
    unit zgsz3474;
    zgsz3474 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31107);
      CONVERT_OF(sail_int, mach_int)(&zghz31107, INT64_C(64));
      RECREATE(lbits)(&zghz31108);
      UNDEFINED(lbits)(&zghz31108, zghz31107);
      zAFSR1_EL2 = CONVERT_OF(fbits, lbits)(zghz31108, true);
    }
    unit zgsz3473;
    zgsz3473 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31105);
      CONVERT_OF(sail_int, mach_int)(&zghz31105, INT64_C(64));
      RECREATE(lbits)(&zghz31106);
      UNDEFINED(lbits)(&zghz31106, zghz31105);
      zAFSR1_EL3 = CONVERT_OF(fbits, lbits)(zghz31106, true);
    }
    unit zgsz3472;
    zgsz3472 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31103);
      CONVERT_OF(sail_int, mach_int)(&zghz31103, INT64_C(64));
      RECREATE(lbits)(&zghz31104);
      UNDEFINED(lbits)(&zghz31104, zghz31103);
      zAIDR_EL1 = CONVERT_OF(fbits, lbits)(zghz31104, true);
    }
    unit zgsz3471;
    zgsz3471 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31101);
      CONVERT_OF(sail_int, mach_int)(&zghz31101, INT64_C(64));
      RECREATE(lbits)(&zghz31102);
      UNDEFINED(lbits)(&zghz31102, zghz31101);
      zAMAIR_EL1 = CONVERT_OF(fbits, lbits)(zghz31102, true);
    }
    unit zgsz3470;
    zgsz3470 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31099);
      CONVERT_OF(sail_int, mach_int)(&zghz31099, INT64_C(64));
      RECREATE(lbits)(&zghz31100);
      UNDEFINED(lbits)(&zghz31100, zghz31099);
      zAMAIR_EL2 = CONVERT_OF(fbits, lbits)(zghz31100, true);
    }
    unit zgsz3469;
    zgsz3469 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31097);
      CONVERT_OF(sail_int, mach_int)(&zghz31097, INT64_C(64));
      RECREATE(lbits)(&zghz31098);
      UNDEFINED(lbits)(&zghz31098, zghz31097);
      zAMAIR_EL3 = CONVERT_OF(fbits, lbits)(zghz31098, true);
    }
    unit zgsz3468;
    zgsz3468 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31095);
      CONVERT_OF(sail_int, mach_int)(&zghz31095, INT64_C(32));
      RECREATE(lbits)(&zghz31096);
      UNDEFINED(lbits)(&zghz31096, zghz31095);
      zCCSIDR_EL1 = CONVERT_OF(fbits, lbits)(zghz31096, true);
    }
    unit zgsz3467;
    zgsz3467 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31093);
      CONVERT_OF(sail_int, mach_int)(&zghz31093, INT64_C(32));
      RECREATE(lbits)(&zghz31094);
      UNDEFINED(lbits)(&zghz31094, zghz31093);
      zCHCR_EL2 = CONVERT_OF(fbits, lbits)(zghz31094, true);
    }
    unit zgsz3466;
    zgsz3466 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31091);
      CONVERT_OF(sail_int, mach_int)(&zghz31091, INT64_C(64));
      RECREATE(lbits)(&zghz31092);
      UNDEFINED(lbits)(&zghz31092, zghz31091);
      zCLIDR_EL1 = CONVERT_OF(fbits, lbits)(zghz31092, true);
    }
    unit zgsz3465;
    zgsz3465 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31089);
      CONVERT_OF(sail_int, mach_int)(&zghz31089, INT64_C(32));
      RECREATE(lbits)(&zghz31090);
      UNDEFINED(lbits)(&zghz31090, zghz31089);
      zCNTFRQ_EL0 = CONVERT_OF(fbits, lbits)(zghz31090, true);
    }
    unit zgsz3464;
    zgsz3464 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31087);
      CONVERT_OF(sail_int, mach_int)(&zghz31087, INT64_C(32));
      RECREATE(lbits)(&zghz31088);
      UNDEFINED(lbits)(&zghz31088, zghz31087);
      zCNTHCTL_EL2 = CONVERT_OF(fbits, lbits)(zghz31088, true);
    }
    unit zgsz3463;
    zgsz3463 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31085);
      CONVERT_OF(sail_int, mach_int)(&zghz31085, INT64_C(32));
      RECREATE(lbits)(&zghz31086);
      UNDEFINED(lbits)(&zghz31086, zghz31085);
      zCNTKCTL_EL1 = CONVERT_OF(fbits, lbits)(zghz31086, true);
    }
    unit zgsz3462;
    zgsz3462 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31083);
      CONVERT_OF(sail_int, mach_int)(&zghz31083, INT64_C(32));
      RECREATE(lbits)(&zghz31084);
      UNDEFINED(lbits)(&zghz31084, zghz31083);
      zCNTHP_CTL_EL2 = CONVERT_OF(fbits, lbits)(zghz31084, true);
    }
    unit zgsz3461;
    zgsz3461 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31081);
      CONVERT_OF(sail_int, mach_int)(&zghz31081, INT64_C(64));
      RECREATE(lbits)(&zghz31082);
      UNDEFINED(lbits)(&zghz31082, zghz31081);
      zCNTHP_CVAL_EL2 = CONVERT_OF(fbits, lbits)(zghz31082, true);
    }
    unit zgsz3460;
    zgsz3460 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31079);
      CONVERT_OF(sail_int, mach_int)(&zghz31079, INT64_C(32));
      RECREATE(lbits)(&zghz31080);
      UNDEFINED(lbits)(&zghz31080, zghz31079);
      zCNTHP_TVAL_EL2 = CONVERT_OF(fbits, lbits)(zghz31080, true);
    }
    unit zgsz3459;
    zgsz3459 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31077);
      CONVERT_OF(sail_int, mach_int)(&zghz31077, INT64_C(32));
      RECREATE(lbits)(&zghz31078);
      UNDEFINED(lbits)(&zghz31078, zghz31077);
      zCNTHV_CTL_EL2 = CONVERT_OF(fbits, lbits)(zghz31078, true);
    }
    unit zgsz3458;
    zgsz3458 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31075);
      CONVERT_OF(sail_int, mach_int)(&zghz31075, INT64_C(64));
      RECREATE(lbits)(&zghz31076);
      UNDEFINED(lbits)(&zghz31076, zghz31075);
      zCNTHV_CVAL_EL2 = CONVERT_OF(fbits, lbits)(zghz31076, true);
    }
    unit zgsz3457;
    zgsz3457 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31073);
      CONVERT_OF(sail_int, mach_int)(&zghz31073, INT64_C(32));
      RECREATE(lbits)(&zghz31074);
      UNDEFINED(lbits)(&zghz31074, zghz31073);
      zCNTHV_TVAL_EL2 = CONVERT_OF(fbits, lbits)(zghz31074, true);
    }
    unit zgsz3456;
    zgsz3456 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31071);
      CONVERT_OF(sail_int, mach_int)(&zghz31071, INT64_C(64));
      RECREATE(lbits)(&zghz31072);
      UNDEFINED(lbits)(&zghz31072, zghz31071);
      zCNTPCT_EL0 = CONVERT_OF(fbits, lbits)(zghz31072, true);
    }
    unit zgsz3455;
    zgsz3455 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31069);
      CONVERT_OF(sail_int, mach_int)(&zghz31069, INT64_C(32));
      RECREATE(lbits)(&zghz31070);
      UNDEFINED(lbits)(&zghz31070, zghz31069);
      zCNTPS_CTL_EL1 = CONVERT_OF(fbits, lbits)(zghz31070, true);
    }
    unit zgsz3454;
    zgsz3454 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31067);
      CONVERT_OF(sail_int, mach_int)(&zghz31067, INT64_C(64));
      RECREATE(lbits)(&zghz31068);
      UNDEFINED(lbits)(&zghz31068, zghz31067);
      zCNTPS_CVAL_EL1 = CONVERT_OF(fbits, lbits)(zghz31068, true);
    }
    unit zgsz3453;
    zgsz3453 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31065);
      CONVERT_OF(sail_int, mach_int)(&zghz31065, INT64_C(32));
      RECREATE(lbits)(&zghz31066);
      UNDEFINED(lbits)(&zghz31066, zghz31065);
      zCNTPS_TVAL_EL1 = CONVERT_OF(fbits, lbits)(zghz31066, true);
    }
    unit zgsz3452;
    zgsz3452 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31063);
      CONVERT_OF(sail_int, mach_int)(&zghz31063, INT64_C(32));
      RECREATE(lbits)(&zghz31064);
      UNDEFINED(lbits)(&zghz31064, zghz31063);
      zCNTP_CTL_EL0 = CONVERT_OF(fbits, lbits)(zghz31064, true);
    }
    unit zgsz3451;
    zgsz3451 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31061);
      CONVERT_OF(sail_int, mach_int)(&zghz31061, INT64_C(64));
      RECREATE(lbits)(&zghz31062);
      UNDEFINED(lbits)(&zghz31062, zghz31061);
      zCNTP_CVAL_EL0 = CONVERT_OF(fbits, lbits)(zghz31062, true);
    }
    unit zgsz3450;
    zgsz3450 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31059);
      CONVERT_OF(sail_int, mach_int)(&zghz31059, INT64_C(32));
      RECREATE(lbits)(&zghz31060);
      UNDEFINED(lbits)(&zghz31060, zghz31059);
      zCNTP_TVAL_EL0 = CONVERT_OF(fbits, lbits)(zghz31060, true);
    }
    unit zgsz3449;
    zgsz3449 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31057);
      CONVERT_OF(sail_int, mach_int)(&zghz31057, INT64_C(64));
      RECREATE(lbits)(&zghz31058);
      UNDEFINED(lbits)(&zghz31058, zghz31057);
      zCNTVCT_EL0 = CONVERT_OF(fbits, lbits)(zghz31058, true);
    }
    unit zgsz3448;
    zgsz3448 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31055);
      CONVERT_OF(sail_int, mach_int)(&zghz31055, INT64_C(64));
      RECREATE(lbits)(&zghz31056);
      UNDEFINED(lbits)(&zghz31056, zghz31055);
      zCNTVOFF_EL2 = CONVERT_OF(fbits, lbits)(zghz31056, true);
    }
    unit zgsz3447;
    zgsz3447 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31053);
      CONVERT_OF(sail_int, mach_int)(&zghz31053, INT64_C(32));
      RECREATE(lbits)(&zghz31054);
      UNDEFINED(lbits)(&zghz31054, zghz31053);
      zCNTV_CTL_EL0 = CONVERT_OF(fbits, lbits)(zghz31054, true);
    }
    unit zgsz3446;
    zgsz3446 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31051);
      CONVERT_OF(sail_int, mach_int)(&zghz31051, INT64_C(64));
      RECREATE(lbits)(&zghz31052);
      UNDEFINED(lbits)(&zghz31052, zghz31051);
      zCNTV_CVAL_EL0 = CONVERT_OF(fbits, lbits)(zghz31052, true);
    }
    unit zgsz3445;
    zgsz3445 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31049);
      CONVERT_OF(sail_int, mach_int)(&zghz31049, INT64_C(32));
      RECREATE(lbits)(&zghz31050);
      UNDEFINED(lbits)(&zghz31050, zghz31049);
      zCNTV_TVAL_EL0 = CONVERT_OF(fbits, lbits)(zghz31050, true);
    }
    unit zgsz3444;
    zgsz3444 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31047);
      CONVERT_OF(sail_int, mach_int)(&zghz31047, INT64_C(32));
      RECREATE(lbits)(&zghz31048);
      UNDEFINED(lbits)(&zghz31048, zghz31047);
      zCONTEXTIDR_EL1 = CONVERT_OF(fbits, lbits)(zghz31048, true);
    }
    unit zgsz3443;
    zgsz3443 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31045);
      CONVERT_OF(sail_int, mach_int)(&zghz31045, INT64_C(32));
      RECREATE(lbits)(&zghz31046);
      UNDEFINED(lbits)(&zghz31046, zghz31045);
      zCONTEXTIDR_EL2 = CONVERT_OF(fbits, lbits)(zghz31046, true);
    }
    unit zgsz3442;
    zgsz3442 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31043);
      CONVERT_OF(sail_int, mach_int)(&zghz31043, INT64_C(32));
      RECREATE(lbits)(&zghz31044);
      UNDEFINED(lbits)(&zghz31044, zghz31043);
      zCSCR_EL3 = CONVERT_OF(fbits, lbits)(zghz31044, true);
    }
    unit zgsz3441;
    zgsz3441 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31041);
      CONVERT_OF(sail_int, mach_int)(&zghz31041, INT64_C(32));
      RECREATE(lbits)(&zghz31042);
      UNDEFINED(lbits)(&zghz31042, zghz31041);
      zCSSELR_EL1 = CONVERT_OF(fbits, lbits)(zghz31042, true);
    }
    unit zgsz3440;
    zgsz3440 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31039);
      CONVERT_OF(sail_int, mach_int)(&zghz31039, INT64_C(32));
      RECREATE(lbits)(&zghz31040);
      UNDEFINED(lbits)(&zghz31040, zghz31039);
      zCTR_EL0 = CONVERT_OF(fbits, lbits)(zghz31040, true);
    }
    unit zgsz3439;
    zgsz3439 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31037);
      CONVERT_OF(sail_int, mach_int)(&zghz31037, INT64_C(32));
      RECREATE(lbits)(&zghz31038);
      UNDEFINED(lbits)(&zghz31038, zghz31037);
      zDACR32_EL2 = CONVERT_OF(fbits, lbits)(zghz31038, true);
    }
    unit zgsz3438;
    zgsz3438 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31035);
      CONVERT_OF(sail_int, mach_int)(&zghz31035, INT64_C(32));
      RECREATE(lbits)(&zghz31036);
      UNDEFINED(lbits)(&zghz31036, zghz31035);
      zDBGAUTHSTATUS_EL1 = CONVERT_OF(fbits, lbits)(zghz31036, true);
    }
    unit zgsz3437;
    zgsz3437 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31033);
      CONVERT_OF(sail_int, mach_int)(&zghz31033, INT64_C(32));
      RECREATE(lbits)(&zghz31034);
      UNDEFINED(lbits)(&zghz31034, zghz31033);
      zMDCR_EL2 = CONVERT_OF(fbits, lbits)(zghz31034, true);
    }
    unit zgsz3436;
    zgsz3436 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31031);
      CONVERT_OF(sail_int, mach_int)(&zghz31031, INT64_C(32));
      RECREATE(lbits)(&zghz31032);
      UNDEFINED(lbits)(&zghz31032, zghz31031);
      zMDCR_EL3 = CONVERT_OF(fbits, lbits)(zghz31032, true);
    }
    unit zgsz3435;
    zgsz3435 = UNIT;
  }
  {
    uint64_t zgaz3325;
    {
      RECREATE(sail_int)(&zghz31029);
      CONVERT_OF(sail_int, mach_int)(&zghz31029, INT64_C(32));
      RECREATE(lbits)(&zghz31030);
      UNDEFINED(lbits)(&zghz31030, zghz31029);
      zgaz3325 = CONVERT_OF(fbits, lbits)(zghz31030, true);
    }
    {
      RECREATE(sail_int)(&zghz31028);
      CONVERT_OF(sail_int, mach_int)(&zghz31028, INT64_C(16));
      UNDEFINED(vector_zz5vecz8z5bv32z9)(&zDBGBCR_EL1, zghz31028, zgaz3325);
    }
    unit zgsz3434;
    zgsz3434 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31027);
      CONVERT_OF(sail_int, mach_int)(&zghz31027, INT64_C(129));
      UNDEFINED(lbits)(&zCDLR_EL0, zghz31027);
    }
    unit zgsz3433;
    zgsz3433 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31025);
      CONVERT_OF(sail_int, mach_int)(&zghz31025, INT64_C(32));
      RECREATE(lbits)(&zghz31026);
      UNDEFINED(lbits)(&zghz31026, zghz31025);
      zDSPSR_EL0 = CONVERT_OF(fbits, lbits)(zghz31026, true);
    }
    unit zgsz3432;
    zgsz3432 = UNIT;
  }
  {
    zDBGEN = sailgen_undefined_signal(UNIT);
    unit zgsz3431;
    zgsz3431 = UNIT;
  }
  {
    zSPIDEN = sailgen_undefined_signal(UNIT);
    unit zgsz3430;
    zgsz3430 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31023);
      CONVERT_OF(sail_int, mach_int)(&zghz31023, INT64_C(32));
      RECREATE(lbits)(&zghz31024);
      UNDEFINED(lbits)(&zghz31024, zghz31023);
      zDBGPRCR_EL1 = CONVERT_OF(fbits, lbits)(zghz31024, true);
    }
    unit zgsz3429;
    zgsz3429 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31021);
      CONVERT_OF(sail_int, mach_int)(&zghz31021, INT64_C(32));
      RECREATE(lbits)(&zghz31022);
      UNDEFINED(lbits)(&zghz31022, zghz31021);
      zOSDLR_EL1 = CONVERT_OF(fbits, lbits)(zghz31022, true);
    }
    unit zgsz3428;
    zgsz3428 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31019);
      CONVERT_OF(sail_int, mach_int)(&zghz31019, INT64_C(32));
      RECREATE(lbits)(&zghz31020);
      UNDEFINED(lbits)(&zghz31020, zghz31019);
      zOSLSR_EL1 = CONVERT_OF(fbits, lbits)(zghz31020, true);
    }
    unit zgsz3427;
    zgsz3427 = UNIT;
  }
  {
    uint64_t zgaz3326;
    {
      RECREATE(sail_int)(&zghz31017);
      CONVERT_OF(sail_int, mach_int)(&zghz31017, INT64_C(64));
      RECREATE(lbits)(&zghz31018);
      UNDEFINED(lbits)(&zghz31018, zghz31017);
      zgaz3326 = CONVERT_OF(fbits, lbits)(zghz31018, true);
    }
    {
      RECREATE(sail_int)(&zghz31016);
      CONVERT_OF(sail_int, mach_int)(&zghz31016, INT64_C(16));
      UNDEFINED(vector_zz5vecz8z5bv64z9)(&zDBGBVR_EL1, zghz31016, zgaz3326);
    }
    unit zgsz3426;
    zgsz3426 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31014);
      CONVERT_OF(sail_int, mach_int)(&zghz31014, INT64_C(32));
      RECREATE(lbits)(&zghz31015);
      UNDEFINED(lbits)(&zghz31015, zghz31014);
      zDBGCLAIMCLR_EL1 = CONVERT_OF(fbits, lbits)(zghz31015, true);
    }
    unit zgsz3425;
    zgsz3425 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31012);
      CONVERT_OF(sail_int, mach_int)(&zghz31012, INT64_C(32));
      RECREATE(lbits)(&zghz31013);
      UNDEFINED(lbits)(&zghz31013, zghz31012);
      zDBGCLAIMSET_EL1 = CONVERT_OF(fbits, lbits)(zghz31013, true);
    }
    unit zgsz3424;
    zgsz3424 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31010);
      CONVERT_OF(sail_int, mach_int)(&zghz31010, INT64_C(32));
      RECREATE(lbits)(&zghz31011);
      UNDEFINED(lbits)(&zghz31011, zghz31010);
      zDBGDTRRX_EL0 = CONVERT_OF(fbits, lbits)(zghz31011, true);
    }
    unit zgsz3423;
    zgsz3423 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31008);
      CONVERT_OF(sail_int, mach_int)(&zghz31008, INT64_C(32));
      RECREATE(lbits)(&zghz31009);
      UNDEFINED(lbits)(&zghz31009, zghz31008);
      zMDSCR_EL1 = CONVERT_OF(fbits, lbits)(zghz31009, true);
    }
    unit zgsz3422;
    zgsz3422 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31007);
      CONVERT_OF(sail_int, mach_int)(&zghz31007, INT64_C(129));
      UNDEFINED(lbits)(&zCDBGDTR_EL0, zghz31007);
    }
    unit zgsz3421;
    zgsz3421 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31005);
      CONVERT_OF(sail_int, mach_int)(&zghz31005, INT64_C(32));
      RECREATE(lbits)(&zghz31006);
      UNDEFINED(lbits)(&zghz31006, zghz31005);
      zDBGVCR32_EL2 = CONVERT_OF(fbits, lbits)(zghz31006, true);
    }
    unit zgsz3420;
    zgsz3420 = UNIT;
  }
  {
    uint64_t zgaz3327;
    {
      RECREATE(sail_int)(&zghz31003);
      CONVERT_OF(sail_int, mach_int)(&zghz31003, INT64_C(32));
      RECREATE(lbits)(&zghz31004);
      UNDEFINED(lbits)(&zghz31004, zghz31003);
      zgaz3327 = CONVERT_OF(fbits, lbits)(zghz31004, true);
    }
    {
      RECREATE(sail_int)(&zghz31002);
      CONVERT_OF(sail_int, mach_int)(&zghz31002, INT64_C(16));
      UNDEFINED(vector_zz5vecz8z5bv32z9)(&zDBGWCR_EL1, zghz31002, zgaz3327);
    }
    unit zgsz3419;
    zgsz3419 = UNIT;
  }
  {
    uint64_t zgaz3328;
    {
      RECREATE(sail_int)(&zghz31000);
      CONVERT_OF(sail_int, mach_int)(&zghz31000, INT64_C(64));
      RECREATE(lbits)(&zghz31001);
      UNDEFINED(lbits)(&zghz31001, zghz31000);
      zgaz3328 = CONVERT_OF(fbits, lbits)(zghz31001, true);
    }
    {
      RECREATE(sail_int)(&zghz3999);
      CONVERT_OF(sail_int, mach_int)(&zghz3999, INT64_C(16));
      UNDEFINED(vector_zz5vecz8z5bv64z9)(&zDBGWVR_EL1, zghz3999, zgaz3328);
    }
    unit zgsz3418;
    zgsz3418 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3997);
      CONVERT_OF(sail_int, mach_int)(&zghz3997, INT64_C(32));
      RECREATE(lbits)(&zghz3998);
      UNDEFINED(lbits)(&zghz3998, zghz3997);
      zDCZID_EL0 = CONVERT_OF(fbits, lbits)(zghz3998, true);
    }
    unit zgsz3417;
    zgsz3417 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3995);
      CONVERT_OF(sail_int, mach_int)(&zghz3995, INT64_C(32));
      RECREATE(lbits)(&zghz3996);
      UNDEFINED(lbits)(&zghz3996, zghz3995);
      zDISR_EL1 = CONVERT_OF(fbits, lbits)(zghz3996, true);
    }
    unit zgsz3416;
    zgsz3416 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3993);
      CONVERT_OF(sail_int, mach_int)(&zghz3993, INT64_C(32));
      RECREATE(lbits)(&zghz3994);
      UNDEFINED(lbits)(&zghz3994, zghz3993);
      zVDISR_EL2 = CONVERT_OF(fbits, lbits)(zghz3994, true);
    }
    unit zgsz3415;
    zgsz3415 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3991);
      CONVERT_OF(sail_int, mach_int)(&zghz3991, INT64_C(32));
      RECREATE(lbits)(&zghz3992);
      UNDEFINED(lbits)(&zghz3992, zghz3991);
      zERRIDR_EL1 = CONVERT_OF(fbits, lbits)(zghz3992, true);
    }
    unit zgsz3414;
    zgsz3414 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3989);
      CONVERT_OF(sail_int, mach_int)(&zghz3989, INT64_C(32));
      RECREATE(lbits)(&zghz3990);
      UNDEFINED(lbits)(&zghz3990, zghz3989);
      zERRSELR_EL1 = CONVERT_OF(fbits, lbits)(zghz3990, true);
    }
    unit zgsz3413;
    zgsz3413 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3987);
      CONVERT_OF(sail_int, mach_int)(&zghz3987, INT64_C(64));
      RECREATE(lbits)(&zghz3988);
      UNDEFINED(lbits)(&zghz3988, zghz3987);
      zERXADDR_EL1 = CONVERT_OF(fbits, lbits)(zghz3988, true);
    }
    unit zgsz3412;
    zgsz3412 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3985);
      CONVERT_OF(sail_int, mach_int)(&zghz3985, INT64_C(64));
      RECREATE(lbits)(&zghz3986);
      UNDEFINED(lbits)(&zghz3986, zghz3985);
      zERXCTLR_EL1 = CONVERT_OF(fbits, lbits)(zghz3986, true);
    }
    unit zgsz3411;
    zgsz3411 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3983);
      CONVERT_OF(sail_int, mach_int)(&zghz3983, INT64_C(64));
      RECREATE(lbits)(&zghz3984);
      UNDEFINED(lbits)(&zghz3984, zghz3983);
      zERXFR_EL1 = CONVERT_OF(fbits, lbits)(zghz3984, true);
    }
    unit zgsz3410;
    zgsz3410 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3981);
      CONVERT_OF(sail_int, mach_int)(&zghz3981, INT64_C(64));
      RECREATE(lbits)(&zghz3982);
      UNDEFINED(lbits)(&zghz3982, zghz3981);
      zERXMISC0_EL1 = CONVERT_OF(fbits, lbits)(zghz3982, true);
    }
    unit zgsz3409;
    zgsz3409 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3979);
      CONVERT_OF(sail_int, mach_int)(&zghz3979, INT64_C(64));
      RECREATE(lbits)(&zghz3980);
      UNDEFINED(lbits)(&zghz3980, zghz3979);
      zERXMISC1_EL1 = CONVERT_OF(fbits, lbits)(zghz3980, true);
    }
    unit zgsz3408;
    zgsz3408 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3977);
      CONVERT_OF(sail_int, mach_int)(&zghz3977, INT64_C(64));
      RECREATE(lbits)(&zghz3978);
      UNDEFINED(lbits)(&zghz3978, zghz3977);
      zERXSTATUS_EL1 = CONVERT_OF(fbits, lbits)(zghz3978, true);
    }
    unit zgsz3407;
    zgsz3407 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3975);
      CONVERT_OF(sail_int, mach_int)(&zghz3975, INT64_C(32));
      RECREATE(lbits)(&zghz3976);
      UNDEFINED(lbits)(&zghz3976, zghz3975);
      zFPCR = CONVERT_OF(fbits, lbits)(zghz3976, true);
    }
    unit zgsz3406;
    zgsz3406 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3973);
      CONVERT_OF(sail_int, mach_int)(&zghz3973, INT64_C(32));
      RECREATE(lbits)(&zghz3974);
      UNDEFINED(lbits)(&zghz3974, zghz3973);
      zFPEXC32_EL2 = CONVERT_OF(fbits, lbits)(zghz3974, true);
    }
    unit zgsz3405;
    zgsz3405 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3971);
      CONVERT_OF(sail_int, mach_int)(&zghz3971, INT64_C(32));
      RECREATE(lbits)(&zghz3972);
      UNDEFINED(lbits)(&zghz3972, zghz3971);
      zFPSR = CONVERT_OF(fbits, lbits)(zghz3972, true);
    }
    unit zgsz3404;
    zgsz3404 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3969);
      CONVERT_OF(sail_int, mach_int)(&zghz3969, INT64_C(64));
      RECREATE(lbits)(&zghz3970);
      UNDEFINED(lbits)(&zghz3970, zghz3969);
      zHACR_EL2 = CONVERT_OF(fbits, lbits)(zghz3970, true);
    }
    unit zgsz3403;
    zgsz3403 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3967);
      CONVERT_OF(sail_int, mach_int)(&zghz3967, INT64_C(32));
      RECREATE(lbits)(&zghz3968);
      UNDEFINED(lbits)(&zghz3968, zghz3967);
      zHSTR_EL2 = CONVERT_OF(fbits, lbits)(zghz3968, true);
    }
    unit zgsz3402;
    zgsz3402 = UNIT;
  }
  {
    uint64_t zgaz3329;
    {
      RECREATE(sail_int)(&zghz3965);
      CONVERT_OF(sail_int, mach_int)(&zghz3965, INT64_C(32));
      RECREATE(lbits)(&zghz3966);
      UNDEFINED(lbits)(&zghz3966, zghz3965);
      zgaz3329 = CONVERT_OF(fbits, lbits)(zghz3966, true);
    }
    {
      RECREATE(sail_int)(&zghz3964);
      CONVERT_OF(sail_int, mach_int)(&zghz3964, INT64_C(4));
      UNDEFINED(vector_zz5vecz8z5bv32z9)(&zICC_AP0R_EL1, zghz3964, zgaz3329);
    }
    unit zgsz3401;
    zgsz3401 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3962);
      CONVERT_OF(sail_int, mach_int)(&zghz3962, INT64_C(32));
      RECREATE(lbits)(&zghz3963);
      UNDEFINED(lbits)(&zghz3963, zghz3962);
      zICC_SRE_EL1_NS = CONVERT_OF(fbits, lbits)(zghz3963, true);
    }
    unit zgsz3400;
    zgsz3400 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3960);
      CONVERT_OF(sail_int, mach_int)(&zghz3960, INT64_C(32));
      RECREATE(lbits)(&zghz3961);
      UNDEFINED(lbits)(&zghz3961, zghz3960);
      zICC_SRE_EL1_S = CONVERT_OF(fbits, lbits)(zghz3961, true);
    }
    unit zgsz3399;
    zgsz3399 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3958);
      CONVERT_OF(sail_int, mach_int)(&zghz3958, INT64_C(32));
      RECREATE(lbits)(&zghz3959);
      UNDEFINED(lbits)(&zghz3959, zghz3958);
      zICC_SRE_EL2 = CONVERT_OF(fbits, lbits)(zghz3959, true);
    }
    unit zgsz3398;
    zgsz3398 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3956);
      CONVERT_OF(sail_int, mach_int)(&zghz3956, INT64_C(32));
      RECREATE(lbits)(&zghz3957);
      UNDEFINED(lbits)(&zghz3957, zghz3956);
      zICC_SRE_EL3 = CONVERT_OF(fbits, lbits)(zghz3957, true);
    }
    unit zgsz3397;
    zgsz3397 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3954);
      CONVERT_OF(sail_int, mach_int)(&zghz3954, INT64_C(32));
      RECREATE(lbits)(&zghz3955);
      UNDEFINED(lbits)(&zghz3955, zghz3954);
      zICH_HCR_EL2 = CONVERT_OF(fbits, lbits)(zghz3955, true);
    }
    unit zgsz3396;
    zgsz3396 = UNIT;
  }
  {
    uint64_t zgaz3330;
    {
      RECREATE(sail_int)(&zghz3952);
      CONVERT_OF(sail_int, mach_int)(&zghz3952, INT64_C(32));
      RECREATE(lbits)(&zghz3953);
      UNDEFINED(lbits)(&zghz3953, zghz3952);
      zgaz3330 = CONVERT_OF(fbits, lbits)(zghz3953, true);
    }
    {
      RECREATE(sail_int)(&zghz3951);
      CONVERT_OF(sail_int, mach_int)(&zghz3951, INT64_C(4));
      UNDEFINED(vector_zz5vecz8z5bv32z9)(&zICV_AP0R_EL1, zghz3951, zgaz3330);
    }
    unit zgsz3395;
    zgsz3395 = UNIT;
  }
  {
    uint64_t zgaz3331;
    {
      RECREATE(sail_int)(&zghz3949);
      CONVERT_OF(sail_int, mach_int)(&zghz3949, INT64_C(32));
      RECREATE(lbits)(&zghz3950);
      UNDEFINED(lbits)(&zghz3950, zghz3949);
      zgaz3331 = CONVERT_OF(fbits, lbits)(zghz3950, true);
    }
    {
      RECREATE(sail_int)(&zghz3948);
      CONVERT_OF(sail_int, mach_int)(&zghz3948, INT64_C(4));
      UNDEFINED(vector_zz5vecz8z5bv32z9)(&zICC_AP1R_EL1, zghz3948, zgaz3331);
    }
    unit zgsz3394;
    zgsz3394 = UNIT;
  }
  {
    uint64_t zgaz3332;
    {
      RECREATE(sail_int)(&zghz3946);
      CONVERT_OF(sail_int, mach_int)(&zghz3946, INT64_C(32));
      RECREATE(lbits)(&zghz3947);
      UNDEFINED(lbits)(&zghz3947, zghz3946);
      zgaz3332 = CONVERT_OF(fbits, lbits)(zghz3947, true);
    }
    {
      RECREATE(sail_int)(&zghz3945);
      CONVERT_OF(sail_int, mach_int)(&zghz3945, INT64_C(4));
      UNDEFINED(vector_zz5vecz8z5bv32z9)(&zICC_AP1R_EL1_NS, zghz3945, zgaz3332);
    }
    unit zgsz3393;
    zgsz3393 = UNIT;
  }
  {
    uint64_t zgaz3333;
    {
      RECREATE(sail_int)(&zghz3943);
      CONVERT_OF(sail_int, mach_int)(&zghz3943, INT64_C(32));
      RECREATE(lbits)(&zghz3944);
      UNDEFINED(lbits)(&zghz3944, zghz3943);
      zgaz3333 = CONVERT_OF(fbits, lbits)(zghz3944, true);
    }
    {
      RECREATE(sail_int)(&zghz3942);
      CONVERT_OF(sail_int, mach_int)(&zghz3942, INT64_C(4));
      UNDEFINED(vector_zz5vecz8z5bv32z9)(&zICC_AP1R_EL1_S, zghz3942, zgaz3333);
    }
    unit zgsz3392;
    zgsz3392 = UNIT;
  }
  {
    uint64_t zgaz3334;
    {
      RECREATE(sail_int)(&zghz3940);
      CONVERT_OF(sail_int, mach_int)(&zghz3940, INT64_C(32));
      RECREATE(lbits)(&zghz3941);
      UNDEFINED(lbits)(&zghz3941, zghz3940);
      zgaz3334 = CONVERT_OF(fbits, lbits)(zghz3941, true);
    }
    {
      RECREATE(sail_int)(&zghz3939);
      CONVERT_OF(sail_int, mach_int)(&zghz3939, INT64_C(4));
      UNDEFINED(vector_zz5vecz8z5bv32z9)(&zICV_AP1R_EL1, zghz3939, zgaz3334);
    }
    unit zgsz3391;
    zgsz3391 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3937);
      CONVERT_OF(sail_int, mach_int)(&zghz3937, INT64_C(32));
      RECREATE(lbits)(&zghz3938);
      UNDEFINED(lbits)(&zghz3938, zghz3937);
      zICC_BPR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3938, true);
    }
    unit zgsz3390;
    zgsz3390 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3935);
      CONVERT_OF(sail_int, mach_int)(&zghz3935, INT64_C(32));
      RECREATE(lbits)(&zghz3936);
      UNDEFINED(lbits)(&zghz3936, zghz3935);
      zICV_BPR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3936, true);
    }
    unit zgsz3389;
    zgsz3389 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3933);
      CONVERT_OF(sail_int, mach_int)(&zghz3933, INT64_C(32));
      RECREATE(lbits)(&zghz3934);
      UNDEFINED(lbits)(&zghz3934, zghz3933);
      zICC_BPR1_EL1_NS = CONVERT_OF(fbits, lbits)(zghz3934, true);
    }
    unit zgsz3388;
    zgsz3388 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3931);
      CONVERT_OF(sail_int, mach_int)(&zghz3931, INT64_C(32));
      RECREATE(lbits)(&zghz3932);
      UNDEFINED(lbits)(&zghz3932, zghz3931);
      zICC_BPR1_EL1_S = CONVERT_OF(fbits, lbits)(zghz3932, true);
    }
    unit zgsz3387;
    zgsz3387 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3929);
      CONVERT_OF(sail_int, mach_int)(&zghz3929, INT64_C(32));
      RECREATE(lbits)(&zghz3930);
      UNDEFINED(lbits)(&zghz3930, zghz3929);
      zICV_BPR1_EL1 = CONVERT_OF(fbits, lbits)(zghz3930, true);
    }
    unit zgsz3386;
    zgsz3386 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3927);
      CONVERT_OF(sail_int, mach_int)(&zghz3927, INT64_C(32));
      RECREATE(lbits)(&zghz3928);
      UNDEFINED(lbits)(&zghz3928, zghz3927);
      zICC_CTLR_EL1_NS = CONVERT_OF(fbits, lbits)(zghz3928, true);
    }
    unit zgsz3385;
    zgsz3385 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3925);
      CONVERT_OF(sail_int, mach_int)(&zghz3925, INT64_C(32));
      RECREATE(lbits)(&zghz3926);
      UNDEFINED(lbits)(&zghz3926, zghz3925);
      zICC_CTLR_EL1_S = CONVERT_OF(fbits, lbits)(zghz3926, true);
    }
    unit zgsz3384;
    zgsz3384 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3923);
      CONVERT_OF(sail_int, mach_int)(&zghz3923, INT64_C(32));
      RECREATE(lbits)(&zghz3924);
      UNDEFINED(lbits)(&zghz3924, zghz3923);
      zICV_CTLR_EL1 = CONVERT_OF(fbits, lbits)(zghz3924, true);
    }
    unit zgsz3383;
    zgsz3383 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3921);
      CONVERT_OF(sail_int, mach_int)(&zghz3921, INT64_C(32));
      RECREATE(lbits)(&zghz3922);
      UNDEFINED(lbits)(&zghz3922, zghz3921);
      zICC_CTLR_EL3 = CONVERT_OF(fbits, lbits)(zghz3922, true);
    }
    unit zgsz3382;
    zgsz3382 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3919);
      CONVERT_OF(sail_int, mach_int)(&zghz3919, INT64_C(32));
      RECREATE(lbits)(&zghz3920);
      UNDEFINED(lbits)(&zghz3920, zghz3919);
      zICC_HPPIR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3920, true);
    }
    unit zgsz3381;
    zgsz3381 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3917);
      CONVERT_OF(sail_int, mach_int)(&zghz3917, INT64_C(32));
      RECREATE(lbits)(&zghz3918);
      UNDEFINED(lbits)(&zghz3918, zghz3917);
      zICV_HPPIR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3918, true);
    }
    unit zgsz3380;
    zgsz3380 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3915);
      CONVERT_OF(sail_int, mach_int)(&zghz3915, INT64_C(32));
      RECREATE(lbits)(&zghz3916);
      UNDEFINED(lbits)(&zghz3916, zghz3915);
      zICC_HPPIR1_EL1 = CONVERT_OF(fbits, lbits)(zghz3916, true);
    }
    unit zgsz3379;
    zgsz3379 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3913);
      CONVERT_OF(sail_int, mach_int)(&zghz3913, INT64_C(32));
      RECREATE(lbits)(&zghz3914);
      UNDEFINED(lbits)(&zghz3914, zghz3913);
      zICV_HPPIR1_EL1 = CONVERT_OF(fbits, lbits)(zghz3914, true);
    }
    unit zgsz3378;
    zgsz3378 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3911);
      CONVERT_OF(sail_int, mach_int)(&zghz3911, INT64_C(32));
      RECREATE(lbits)(&zghz3912);
      UNDEFINED(lbits)(&zghz3912, zghz3911);
      zICC_IAR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3912, true);
    }
    unit zgsz3377;
    zgsz3377 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3909);
      CONVERT_OF(sail_int, mach_int)(&zghz3909, INT64_C(32));
      RECREATE(lbits)(&zghz3910);
      UNDEFINED(lbits)(&zghz3910, zghz3909);
      zICV_IAR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3910, true);
    }
    unit zgsz3376;
    zgsz3376 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3907);
      CONVERT_OF(sail_int, mach_int)(&zghz3907, INT64_C(32));
      RECREATE(lbits)(&zghz3908);
      UNDEFINED(lbits)(&zghz3908, zghz3907);
      zICC_IAR1_EL1 = CONVERT_OF(fbits, lbits)(zghz3908, true);
    }
    unit zgsz3375;
    zgsz3375 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3905);
      CONVERT_OF(sail_int, mach_int)(&zghz3905, INT64_C(32));
      RECREATE(lbits)(&zghz3906);
      UNDEFINED(lbits)(&zghz3906, zghz3905);
      zICV_IAR1_EL1 = CONVERT_OF(fbits, lbits)(zghz3906, true);
    }
    unit zgsz3374;
    zgsz3374 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3903);
      CONVERT_OF(sail_int, mach_int)(&zghz3903, INT64_C(32));
      RECREATE(lbits)(&zghz3904);
      UNDEFINED(lbits)(&zghz3904, zghz3903);
      zICC_IGRPEN0_EL1 = CONVERT_OF(fbits, lbits)(zghz3904, true);
    }
    unit zgsz3373;
    zgsz3373 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3901);
      CONVERT_OF(sail_int, mach_int)(&zghz3901, INT64_C(32));
      RECREATE(lbits)(&zghz3902);
      UNDEFINED(lbits)(&zghz3902, zghz3901);
      zICV_IGRPEN0_EL1 = CONVERT_OF(fbits, lbits)(zghz3902, true);
    }
    unit zgsz3372;
    zgsz3372 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3899);
      CONVERT_OF(sail_int, mach_int)(&zghz3899, INT64_C(32));
      RECREATE(lbits)(&zghz3900);
      UNDEFINED(lbits)(&zghz3900, zghz3899);
      zICC_IGRPEN1_EL1_NS = CONVERT_OF(fbits, lbits)(zghz3900, true);
    }
    unit zgsz3371;
    zgsz3371 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3897);
      CONVERT_OF(sail_int, mach_int)(&zghz3897, INT64_C(32));
      RECREATE(lbits)(&zghz3898);
      UNDEFINED(lbits)(&zghz3898, zghz3897);
      zICC_IGRPEN1_EL1_S = CONVERT_OF(fbits, lbits)(zghz3898, true);
    }
    unit zgsz3370;
    zgsz3370 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3895);
      CONVERT_OF(sail_int, mach_int)(&zghz3895, INT64_C(32));
      RECREATE(lbits)(&zghz3896);
      UNDEFINED(lbits)(&zghz3896, zghz3895);
      zICV_IGRPEN1_EL1 = CONVERT_OF(fbits, lbits)(zghz3896, true);
    }
    unit zgsz3369;
    zgsz3369 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3893);
      CONVERT_OF(sail_int, mach_int)(&zghz3893, INT64_C(32));
      RECREATE(lbits)(&zghz3894);
      UNDEFINED(lbits)(&zghz3894, zghz3893);
      zICC_IGRPEN1_EL3 = CONVERT_OF(fbits, lbits)(zghz3894, true);
    }
    unit zgsz3368;
    zgsz3368 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3891);
      CONVERT_OF(sail_int, mach_int)(&zghz3891, INT64_C(32));
      RECREATE(lbits)(&zghz3892);
      UNDEFINED(lbits)(&zghz3892, zghz3891);
      zICC_PMR_EL1 = CONVERT_OF(fbits, lbits)(zghz3892, true);
    }
    unit zgsz3367;
    zgsz3367 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3889);
      CONVERT_OF(sail_int, mach_int)(&zghz3889, INT64_C(32));
      RECREATE(lbits)(&zghz3890);
      UNDEFINED(lbits)(&zghz3890, zghz3889);
      zICV_PMR_EL1 = CONVERT_OF(fbits, lbits)(zghz3890, true);
    }
    unit zgsz3366;
    zgsz3366 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3887);
      CONVERT_OF(sail_int, mach_int)(&zghz3887, INT64_C(32));
      RECREATE(lbits)(&zghz3888);
      UNDEFINED(lbits)(&zghz3888, zghz3887);
      zICC_RPR_EL1 = CONVERT_OF(fbits, lbits)(zghz3888, true);
    }
    unit zgsz3365;
    zgsz3365 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3885);
      CONVERT_OF(sail_int, mach_int)(&zghz3885, INT64_C(32));
      RECREATE(lbits)(&zghz3886);
      UNDEFINED(lbits)(&zghz3886, zghz3885);
      zICV_RPR_EL1 = CONVERT_OF(fbits, lbits)(zghz3886, true);
    }
    unit zgsz3364;
    zgsz3364 = UNIT;
  }
  {
    uint64_t zgaz3335;
    {
      RECREATE(sail_int)(&zghz3883);
      CONVERT_OF(sail_int, mach_int)(&zghz3883, INT64_C(32));
      RECREATE(lbits)(&zghz3884);
      UNDEFINED(lbits)(&zghz3884, zghz3883);
      zgaz3335 = CONVERT_OF(fbits, lbits)(zghz3884, true);
    }
    {
      RECREATE(sail_int)(&zghz3882);
      CONVERT_OF(sail_int, mach_int)(&zghz3882, INT64_C(4));
      UNDEFINED(vector_zz5vecz8z5bv32z9)(&zICH_AP0R_EL2, zghz3882, zgaz3335);
    }
    unit zgsz3363;
    zgsz3363 = UNIT;
  }
  {
    uint64_t zgaz3336;
    {
      RECREATE(sail_int)(&zghz3880);
      CONVERT_OF(sail_int, mach_int)(&zghz3880, INT64_C(32));
      RECREATE(lbits)(&zghz3881);
      UNDEFINED(lbits)(&zghz3881, zghz3880);
      zgaz3336 = CONVERT_OF(fbits, lbits)(zghz3881, true);
    }
    {
      RECREATE(sail_int)(&zghz3879);
      CONVERT_OF(sail_int, mach_int)(&zghz3879, INT64_C(4));
      UNDEFINED(vector_zz5vecz8z5bv32z9)(&zICH_AP1R_EL2, zghz3879, zgaz3336);
    }
    unit zgsz3362;
    zgsz3362 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3877);
      CONVERT_OF(sail_int, mach_int)(&zghz3877, INT64_C(32));
      RECREATE(lbits)(&zghz3878);
      UNDEFINED(lbits)(&zghz3878, zghz3877);
      zICH_EISR_EL2 = CONVERT_OF(fbits, lbits)(zghz3878, true);
    }
    unit zgsz3361;
    zgsz3361 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3875);
      CONVERT_OF(sail_int, mach_int)(&zghz3875, INT64_C(32));
      RECREATE(lbits)(&zghz3876);
      UNDEFINED(lbits)(&zghz3876, zghz3875);
      zICH_ELRSR_EL2 = CONVERT_OF(fbits, lbits)(zghz3876, true);
    }
    unit zgsz3360;
    zgsz3360 = UNIT;
  }
  {
    uint64_t zgaz3337;
    {
      RECREATE(sail_int)(&zghz3873);
      CONVERT_OF(sail_int, mach_int)(&zghz3873, INT64_C(64));
      RECREATE(lbits)(&zghz3874);
      UNDEFINED(lbits)(&zghz3874, zghz3873);
      zgaz3337 = CONVERT_OF(fbits, lbits)(zghz3874, true);
    }
    {
      RECREATE(sail_int)(&zghz3872);
      CONVERT_OF(sail_int, mach_int)(&zghz3872, INT64_C(16));
      UNDEFINED(vector_zz5vecz8z5bv64z9)(&zICH_LR_EL2, zghz3872, zgaz3337);
    }
    unit zgsz3359;
    zgsz3359 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3870);
      CONVERT_OF(sail_int, mach_int)(&zghz3870, INT64_C(32));
      RECREATE(lbits)(&zghz3871);
      UNDEFINED(lbits)(&zghz3871, zghz3870);
      zICH_MISR_EL2 = CONVERT_OF(fbits, lbits)(zghz3871, true);
    }
    unit zgsz3358;
    zgsz3358 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3868);
      CONVERT_OF(sail_int, mach_int)(&zghz3868, INT64_C(32));
      RECREATE(lbits)(&zghz3869);
      UNDEFINED(lbits)(&zghz3869, zghz3868);
      zICH_VMCR_EL2 = CONVERT_OF(fbits, lbits)(zghz3869, true);
    }
    unit zgsz3357;
    zgsz3357 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3866);
      CONVERT_OF(sail_int, mach_int)(&zghz3866, INT64_C(32));
      RECREATE(lbits)(&zghz3867);
      UNDEFINED(lbits)(&zghz3867, zghz3866);
      zICH_VTR_EL2 = CONVERT_OF(fbits, lbits)(zghz3867, true);
    }
    unit zgsz3356;
    zgsz3356 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3864);
      CONVERT_OF(sail_int, mach_int)(&zghz3864, INT64_C(32));
      RECREATE(lbits)(&zghz3865);
      UNDEFINED(lbits)(&zghz3865, zghz3864);
      zID_AA64AFR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3865, true);
    }
    unit zgsz3355;
    zgsz3355 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3862);
      CONVERT_OF(sail_int, mach_int)(&zghz3862, INT64_C(32));
      RECREATE(lbits)(&zghz3863);
      UNDEFINED(lbits)(&zghz3863, zghz3862);
      zID_AA64AFR1_EL1 = CONVERT_OF(fbits, lbits)(zghz3863, true);
    }
    unit zgsz3354;
    zgsz3354 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3860);
      CONVERT_OF(sail_int, mach_int)(&zghz3860, INT64_C(64));
      RECREATE(lbits)(&zghz3861);
      UNDEFINED(lbits)(&zghz3861, zghz3860);
      zID_AA64DFR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3861, true);
    }
    unit zgsz3353;
    zgsz3353 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3858);
      CONVERT_OF(sail_int, mach_int)(&zghz3858, INT64_C(32));
      RECREATE(lbits)(&zghz3859);
      UNDEFINED(lbits)(&zghz3859, zghz3858);
      zID_AA64DFR1_EL1 = CONVERT_OF(fbits, lbits)(zghz3859, true);
    }
    unit zgsz3352;
    zgsz3352 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3856);
      CONVERT_OF(sail_int, mach_int)(&zghz3856, INT64_C(64));
      RECREATE(lbits)(&zghz3857);
      UNDEFINED(lbits)(&zghz3857, zghz3856);
      zID_AA64ISAR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3857, true);
    }
    unit zgsz3351;
    zgsz3351 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3854);
      CONVERT_OF(sail_int, mach_int)(&zghz3854, INT64_C(64));
      RECREATE(lbits)(&zghz3855);
      UNDEFINED(lbits)(&zghz3855, zghz3854);
      zID_AA64ISAR1_EL1 = CONVERT_OF(fbits, lbits)(zghz3855, true);
    }
    unit zgsz3350;
    zgsz3350 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3852);
      CONVERT_OF(sail_int, mach_int)(&zghz3852, INT64_C(32));
      RECREATE(lbits)(&zghz3853);
      UNDEFINED(lbits)(&zghz3853, zghz3852);
      zID_AA64MMFR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3853, true);
    }
    unit zgsz3349;
    zgsz3349 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3850);
      CONVERT_OF(sail_int, mach_int)(&zghz3850, INT64_C(64));
      RECREATE(lbits)(&zghz3851);
      UNDEFINED(lbits)(&zghz3851, zghz3850);
      zID_AA64MMFR1_EL1 = CONVERT_OF(fbits, lbits)(zghz3851, true);
    }
    unit zgsz3348;
    zgsz3348 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3848);
      CONVERT_OF(sail_int, mach_int)(&zghz3848, INT64_C(64));
      RECREATE(lbits)(&zghz3849);
      UNDEFINED(lbits)(&zghz3849, zghz3848);
      zID_AA64MMFR2_EL1 = CONVERT_OF(fbits, lbits)(zghz3849, true);
    }
    unit zgsz3347;
    zgsz3347 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3846);
      CONVERT_OF(sail_int, mach_int)(&zghz3846, INT64_C(64));
      RECREATE(lbits)(&zghz3847);
      UNDEFINED(lbits)(&zghz3847, zghz3846);
      zID_AA64PFR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3847, true);
    }
    unit zgsz3346;
    zgsz3346 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3844);
      CONVERT_OF(sail_int, mach_int)(&zghz3844, INT64_C(32));
      RECREATE(lbits)(&zghz3845);
      UNDEFINED(lbits)(&zghz3845, zghz3844);
      zID_AA64PFR1_EL1 = CONVERT_OF(fbits, lbits)(zghz3845, true);
    }
    unit zgsz3345;
    zgsz3345 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3842);
      CONVERT_OF(sail_int, mach_int)(&zghz3842, INT64_C(64));
      RECREATE(lbits)(&zghz3843);
      UNDEFINED(lbits)(&zghz3843, zghz3842);
      zID_AA64ZFR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3843, true);
    }
    unit zgsz3344;
    zgsz3344 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3840);
      CONVERT_OF(sail_int, mach_int)(&zghz3840, INT64_C(32));
      RECREATE(lbits)(&zghz3841);
      UNDEFINED(lbits)(&zghz3841, zghz3840);
      zID_AFR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3841, true);
    }
    unit zgsz3343;
    zgsz3343 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3838);
      CONVERT_OF(sail_int, mach_int)(&zghz3838, INT64_C(32));
      RECREATE(lbits)(&zghz3839);
      UNDEFINED(lbits)(&zghz3839, zghz3838);
      zID_DFR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3839, true);
    }
    unit zgsz3342;
    zgsz3342 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3836);
      CONVERT_OF(sail_int, mach_int)(&zghz3836, INT64_C(32));
      RECREATE(lbits)(&zghz3837);
      UNDEFINED(lbits)(&zghz3837, zghz3836);
      zID_ISAR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3837, true);
    }
    unit zgsz3341;
    zgsz3341 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3834);
      CONVERT_OF(sail_int, mach_int)(&zghz3834, INT64_C(32));
      RECREATE(lbits)(&zghz3835);
      UNDEFINED(lbits)(&zghz3835, zghz3834);
      zID_ISAR1_EL1 = CONVERT_OF(fbits, lbits)(zghz3835, true);
    }
    unit zgsz3340;
    zgsz3340 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3832);
      CONVERT_OF(sail_int, mach_int)(&zghz3832, INT64_C(32));
      RECREATE(lbits)(&zghz3833);
      UNDEFINED(lbits)(&zghz3833, zghz3832);
      zID_ISAR2_EL1 = CONVERT_OF(fbits, lbits)(zghz3833, true);
    }
    unit zgsz3339;
    zgsz3339 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3830);
      CONVERT_OF(sail_int, mach_int)(&zghz3830, INT64_C(32));
      RECREATE(lbits)(&zghz3831);
      UNDEFINED(lbits)(&zghz3831, zghz3830);
      zID_ISAR3_EL1 = CONVERT_OF(fbits, lbits)(zghz3831, true);
    }
    unit zgsz3338;
    zgsz3338 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3828);
      CONVERT_OF(sail_int, mach_int)(&zghz3828, INT64_C(32));
      RECREATE(lbits)(&zghz3829);
      UNDEFINED(lbits)(&zghz3829, zghz3828);
      zID_ISAR4_EL1 = CONVERT_OF(fbits, lbits)(zghz3829, true);
    }
    unit zgsz3337;
    zgsz3337 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3826);
      CONVERT_OF(sail_int, mach_int)(&zghz3826, INT64_C(32));
      RECREATE(lbits)(&zghz3827);
      UNDEFINED(lbits)(&zghz3827, zghz3826);
      zID_ISAR5_EL1 = CONVERT_OF(fbits, lbits)(zghz3827, true);
    }
    unit zgsz3336;
    zgsz3336 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3824);
      CONVERT_OF(sail_int, mach_int)(&zghz3824, INT64_C(32));
      RECREATE(lbits)(&zghz3825);
      UNDEFINED(lbits)(&zghz3825, zghz3824);
      zID_ISAR6_EL1 = CONVERT_OF(fbits, lbits)(zghz3825, true);
    }
    unit zgsz3335;
    zgsz3335 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3822);
      CONVERT_OF(sail_int, mach_int)(&zghz3822, INT64_C(32));
      RECREATE(lbits)(&zghz3823);
      UNDEFINED(lbits)(&zghz3823, zghz3822);
      zID_MMFR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3823, true);
    }
    unit zgsz3334;
    zgsz3334 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3820);
      CONVERT_OF(sail_int, mach_int)(&zghz3820, INT64_C(32));
      RECREATE(lbits)(&zghz3821);
      UNDEFINED(lbits)(&zghz3821, zghz3820);
      zID_MMFR1_EL1 = CONVERT_OF(fbits, lbits)(zghz3821, true);
    }
    unit zgsz3333;
    zgsz3333 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3818);
      CONVERT_OF(sail_int, mach_int)(&zghz3818, INT64_C(32));
      RECREATE(lbits)(&zghz3819);
      UNDEFINED(lbits)(&zghz3819, zghz3818);
      zID_MMFR2_EL1 = CONVERT_OF(fbits, lbits)(zghz3819, true);
    }
    unit zgsz3332;
    zgsz3332 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3816);
      CONVERT_OF(sail_int, mach_int)(&zghz3816, INT64_C(32));
      RECREATE(lbits)(&zghz3817);
      UNDEFINED(lbits)(&zghz3817, zghz3816);
      zID_MMFR3_EL1 = CONVERT_OF(fbits, lbits)(zghz3817, true);
    }
    unit zgsz3331;
    zgsz3331 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3814);
      CONVERT_OF(sail_int, mach_int)(&zghz3814, INT64_C(32));
      RECREATE(lbits)(&zghz3815);
      UNDEFINED(lbits)(&zghz3815, zghz3814);
      zID_MMFR4_EL1 = CONVERT_OF(fbits, lbits)(zghz3815, true);
    }
    unit zgsz3330;
    zgsz3330 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3812);
      CONVERT_OF(sail_int, mach_int)(&zghz3812, INT64_C(32));
      RECREATE(lbits)(&zghz3813);
      UNDEFINED(lbits)(&zghz3813, zghz3812);
      zID_MMFR5_EL1 = CONVERT_OF(fbits, lbits)(zghz3813, true);
    }
    unit zgsz3329;
    zgsz3329 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3810);
      CONVERT_OF(sail_int, mach_int)(&zghz3810, INT64_C(32));
      RECREATE(lbits)(&zghz3811);
      UNDEFINED(lbits)(&zghz3811, zghz3810);
      zID_PFR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3811, true);
    }
    unit zgsz3328;
    zgsz3328 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3808);
      CONVERT_OF(sail_int, mach_int)(&zghz3808, INT64_C(32));
      RECREATE(lbits)(&zghz3809);
      UNDEFINED(lbits)(&zghz3809, zghz3808);
      zID_PFR1_EL1 = CONVERT_OF(fbits, lbits)(zghz3809, true);
    }
    unit zgsz3327;
    zgsz3327 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3806);
      CONVERT_OF(sail_int, mach_int)(&zghz3806, INT64_C(32));
      RECREATE(lbits)(&zghz3807);
      UNDEFINED(lbits)(&zghz3807, zghz3806);
      zID_PFR2_EL1 = CONVERT_OF(fbits, lbits)(zghz3807, true);
    }
    unit zgsz3326;
    zgsz3326 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3804);
      CONVERT_OF(sail_int, mach_int)(&zghz3804, INT64_C(32));
      RECREATE(lbits)(&zghz3805);
      UNDEFINED(lbits)(&zghz3805, zghz3804);
      zIFSR32_EL2 = CONVERT_OF(fbits, lbits)(zghz3805, true);
    }
    unit zgsz3325;
    zgsz3325 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3802);
      CONVERT_OF(sail_int, mach_int)(&zghz3802, INT64_C(32));
      RECREATE(lbits)(&zghz3803);
      UNDEFINED(lbits)(&zghz3803, zghz3802);
      zISR_EL1 = CONVERT_OF(fbits, lbits)(zghz3803, true);
    }
    unit zgsz3324;
    zgsz3324 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3800);
      CONVERT_OF(sail_int, mach_int)(&zghz3800, INT64_C(32));
      RECREATE(lbits)(&zghz3801);
      UNDEFINED(lbits)(&zghz3801, zghz3800);
      zLORC_EL1 = CONVERT_OF(fbits, lbits)(zghz3801, true);
    }
    unit zgsz3323;
    zgsz3323 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3798);
      CONVERT_OF(sail_int, mach_int)(&zghz3798, INT64_C(64));
      RECREATE(lbits)(&zghz3799);
      UNDEFINED(lbits)(&zghz3799, zghz3798);
      zLOREA_EL1 = CONVERT_OF(fbits, lbits)(zghz3799, true);
    }
    unit zgsz3322;
    zgsz3322 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3796);
      CONVERT_OF(sail_int, mach_int)(&zghz3796, INT64_C(32));
      RECREATE(lbits)(&zghz3797);
      UNDEFINED(lbits)(&zghz3797, zghz3796);
      zLORID_EL1 = CONVERT_OF(fbits, lbits)(zghz3797, true);
    }
    unit zgsz3321;
    zgsz3321 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3794);
      CONVERT_OF(sail_int, mach_int)(&zghz3794, INT64_C(32));
      RECREATE(lbits)(&zghz3795);
      UNDEFINED(lbits)(&zghz3795, zghz3794);
      zLORN_EL1 = CONVERT_OF(fbits, lbits)(zghz3795, true);
    }
    unit zgsz3320;
    zgsz3320 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3792);
      CONVERT_OF(sail_int, mach_int)(&zghz3792, INT64_C(64));
      RECREATE(lbits)(&zghz3793);
      UNDEFINED(lbits)(&zghz3793, zghz3792);
      zLORSA_EL1 = CONVERT_OF(fbits, lbits)(zghz3793, true);
    }
    unit zgsz3319;
    zgsz3319 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3790);
      CONVERT_OF(sail_int, mach_int)(&zghz3790, INT64_C(64));
      RECREATE(lbits)(&zghz3791);
      UNDEFINED(lbits)(&zghz3791, zghz3790);
      zMAIR_EL1 = CONVERT_OF(fbits, lbits)(zghz3791, true);
    }
    unit zgsz3318;
    zgsz3318 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3788);
      CONVERT_OF(sail_int, mach_int)(&zghz3788, INT64_C(64));
      RECREATE(lbits)(&zghz3789);
      UNDEFINED(lbits)(&zghz3789, zghz3788);
      zMAIR_EL2 = CONVERT_OF(fbits, lbits)(zghz3789, true);
    }
    unit zgsz3317;
    zgsz3317 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3786);
      CONVERT_OF(sail_int, mach_int)(&zghz3786, INT64_C(64));
      RECREATE(lbits)(&zghz3787);
      UNDEFINED(lbits)(&zghz3787, zghz3786);
      zMAIR_EL3 = CONVERT_OF(fbits, lbits)(zghz3787, true);
    }
    unit zgsz3316;
    zgsz3316 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3784);
      CONVERT_OF(sail_int, mach_int)(&zghz3784, INT64_C(32));
      RECREATE(lbits)(&zghz3785);
      UNDEFINED(lbits)(&zghz3785, zghz3784);
      zMDCCINT_EL1 = CONVERT_OF(fbits, lbits)(zghz3785, true);
    }
    unit zgsz3315;
    zgsz3315 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3782);
      CONVERT_OF(sail_int, mach_int)(&zghz3782, INT64_C(32));
      RECREATE(lbits)(&zghz3783);
      UNDEFINED(lbits)(&zghz3783, zghz3782);
      zMDCCSR_EL0 = CONVERT_OF(fbits, lbits)(zghz3783, true);
    }
    unit zgsz3314;
    zgsz3314 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3780);
      CONVERT_OF(sail_int, mach_int)(&zghz3780, INT64_C(64));
      RECREATE(lbits)(&zghz3781);
      UNDEFINED(lbits)(&zghz3781, zghz3780);
      zMDRAR_EL1 = CONVERT_OF(fbits, lbits)(zghz3781, true);
    }
    unit zgsz3313;
    zgsz3313 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3778);
      CONVERT_OF(sail_int, mach_int)(&zghz3778, INT64_C(32));
      RECREATE(lbits)(&zghz3779);
      UNDEFINED(lbits)(&zghz3779, zghz3778);
      zMIDR_EL1 = CONVERT_OF(fbits, lbits)(zghz3779, true);
    }
    unit zgsz3312;
    zgsz3312 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3776);
      CONVERT_OF(sail_int, mach_int)(&zghz3776, INT64_C(32));
      RECREATE(lbits)(&zghz3777);
      UNDEFINED(lbits)(&zghz3777, zghz3776);
      zVPIDR_EL2 = CONVERT_OF(fbits, lbits)(zghz3777, true);
    }
    unit zgsz3311;
    zgsz3311 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3774);
      CONVERT_OF(sail_int, mach_int)(&zghz3774, INT64_C(64));
      RECREATE(lbits)(&zghz3775);
      UNDEFINED(lbits)(&zghz3775, zghz3774);
      zMPAM0_EL1 = CONVERT_OF(fbits, lbits)(zghz3775, true);
    }
    unit zgsz3310;
    zgsz3310 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3772);
      CONVERT_OF(sail_int, mach_int)(&zghz3772, INT64_C(64));
      RECREATE(lbits)(&zghz3773);
      UNDEFINED(lbits)(&zghz3773, zghz3772);
      zMPAM3_EL3 = CONVERT_OF(fbits, lbits)(zghz3773, true);
    }
    unit zgsz3309;
    zgsz3309 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3770);
      CONVERT_OF(sail_int, mach_int)(&zghz3770, INT64_C(63));
      RECREATE(lbits)(&zghz3771);
      UNDEFINED(lbits)(&zghz3771, zghz3770);
      z_MPAM2_EL2_0_62 = CONVERT_OF(fbits, lbits)(zghz3771, true);
    }
    unit zgsz3308;
    zgsz3308 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3768);
      CONVERT_OF(sail_int, mach_int)(&zghz3768, INT64_C(63));
      RECREATE(lbits)(&zghz3769);
      UNDEFINED(lbits)(&zghz3769, zghz3768);
      z_MPAM1_EL1_0_62 = CONVERT_OF(fbits, lbits)(zghz3769, true);
    }
    unit zgsz3307;
    zgsz3307 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3766);
      CONVERT_OF(sail_int, mach_int)(&zghz3766, INT64_C(32));
      RECREATE(lbits)(&zghz3767);
      UNDEFINED(lbits)(&zghz3767, zghz3766);
      zMPAMHCR_EL2 = CONVERT_OF(fbits, lbits)(zghz3767, true);
    }
    unit zgsz3306;
    zgsz3306 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3764);
      CONVERT_OF(sail_int, mach_int)(&zghz3764, INT64_C(64));
      RECREATE(lbits)(&zghz3765);
      UNDEFINED(lbits)(&zghz3765, zghz3764);
      zMPAMIDR_EL1 = CONVERT_OF(fbits, lbits)(zghz3765, true);
    }
    unit zgsz3305;
    zgsz3305 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3762);
      CONVERT_OF(sail_int, mach_int)(&zghz3762, INT64_C(64));
      RECREATE(lbits)(&zghz3763);
      UNDEFINED(lbits)(&zghz3763, zghz3762);
      zMPAMVPM0_EL2 = CONVERT_OF(fbits, lbits)(zghz3763, true);
    }
    unit zgsz3304;
    zgsz3304 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3760);
      CONVERT_OF(sail_int, mach_int)(&zghz3760, INT64_C(64));
      RECREATE(lbits)(&zghz3761);
      UNDEFINED(lbits)(&zghz3761, zghz3760);
      zMPAMVPM1_EL2 = CONVERT_OF(fbits, lbits)(zghz3761, true);
    }
    unit zgsz3303;
    zgsz3303 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3758);
      CONVERT_OF(sail_int, mach_int)(&zghz3758, INT64_C(64));
      RECREATE(lbits)(&zghz3759);
      UNDEFINED(lbits)(&zghz3759, zghz3758);
      zMPAMVPM2_EL2 = CONVERT_OF(fbits, lbits)(zghz3759, true);
    }
    unit zgsz3302;
    zgsz3302 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3756);
      CONVERT_OF(sail_int, mach_int)(&zghz3756, INT64_C(64));
      RECREATE(lbits)(&zghz3757);
      UNDEFINED(lbits)(&zghz3757, zghz3756);
      zMPAMVPM3_EL2 = CONVERT_OF(fbits, lbits)(zghz3757, true);
    }
    unit zgsz3301;
    zgsz3301 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3754);
      CONVERT_OF(sail_int, mach_int)(&zghz3754, INT64_C(64));
      RECREATE(lbits)(&zghz3755);
      UNDEFINED(lbits)(&zghz3755, zghz3754);
      zMPAMVPM4_EL2 = CONVERT_OF(fbits, lbits)(zghz3755, true);
    }
    unit zgsz3300;
    zgsz3300 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3752);
      CONVERT_OF(sail_int, mach_int)(&zghz3752, INT64_C(64));
      RECREATE(lbits)(&zghz3753);
      UNDEFINED(lbits)(&zghz3753, zghz3752);
      zMPAMVPM5_EL2 = CONVERT_OF(fbits, lbits)(zghz3753, true);
    }
    unit zgsz3299;
    zgsz3299 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3750);
      CONVERT_OF(sail_int, mach_int)(&zghz3750, INT64_C(64));
      RECREATE(lbits)(&zghz3751);
      UNDEFINED(lbits)(&zghz3751, zghz3750);
      zMPAMVPM6_EL2 = CONVERT_OF(fbits, lbits)(zghz3751, true);
    }
    unit zgsz3298;
    zgsz3298 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3748);
      CONVERT_OF(sail_int, mach_int)(&zghz3748, INT64_C(64));
      RECREATE(lbits)(&zghz3749);
      UNDEFINED(lbits)(&zghz3749, zghz3748);
      zMPAMVPM7_EL2 = CONVERT_OF(fbits, lbits)(zghz3749, true);
    }
    unit zgsz3297;
    zgsz3297 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3746);
      CONVERT_OF(sail_int, mach_int)(&zghz3746, INT64_C(32));
      RECREATE(lbits)(&zghz3747);
      UNDEFINED(lbits)(&zghz3747, zghz3746);
      zMPAMVPMV_EL2 = CONVERT_OF(fbits, lbits)(zghz3747, true);
    }
    unit zgsz3296;
    zgsz3296 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3744);
      CONVERT_OF(sail_int, mach_int)(&zghz3744, INT64_C(64));
      RECREATE(lbits)(&zghz3745);
      UNDEFINED(lbits)(&zghz3745, zghz3744);
      zMPIDR_EL1 = CONVERT_OF(fbits, lbits)(zghz3745, true);
    }
    unit zgsz3295;
    zgsz3295 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3742);
      CONVERT_OF(sail_int, mach_int)(&zghz3742, INT64_C(64));
      RECREATE(lbits)(&zghz3743);
      UNDEFINED(lbits)(&zghz3743, zghz3742);
      zVMPIDR_EL2 = CONVERT_OF(fbits, lbits)(zghz3743, true);
    }
    unit zgsz3294;
    zgsz3294 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3740);
      CONVERT_OF(sail_int, mach_int)(&zghz3740, INT64_C(32));
      RECREATE(lbits)(&zghz3741);
      UNDEFINED(lbits)(&zghz3741, zghz3740);
      zMVFR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3741, true);
    }
    unit zgsz3293;
    zgsz3293 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3738);
      CONVERT_OF(sail_int, mach_int)(&zghz3738, INT64_C(32));
      RECREATE(lbits)(&zghz3739);
      UNDEFINED(lbits)(&zghz3739, zghz3738);
      zMVFR1_EL1 = CONVERT_OF(fbits, lbits)(zghz3739, true);
    }
    unit zgsz3292;
    zgsz3292 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3736);
      CONVERT_OF(sail_int, mach_int)(&zghz3736, INT64_C(32));
      RECREATE(lbits)(&zghz3737);
      UNDEFINED(lbits)(&zghz3737, zghz3736);
      zMVFR2_EL1 = CONVERT_OF(fbits, lbits)(zghz3737, true);
    }
    unit zgsz3291;
    zgsz3291 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3734);
      CONVERT_OF(sail_int, mach_int)(&zghz3734, INT64_C(32));
      RECREATE(lbits)(&zghz3735);
      UNDEFINED(lbits)(&zghz3735, zghz3734);
      zOSDTRRX_EL1 = CONVERT_OF(fbits, lbits)(zghz3735, true);
    }
    unit zgsz3290;
    zgsz3290 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3732);
      CONVERT_OF(sail_int, mach_int)(&zghz3732, INT64_C(32));
      RECREATE(lbits)(&zghz3733);
      UNDEFINED(lbits)(&zghz3733, zghz3732);
      zOSDTRTX_EL1 = CONVERT_OF(fbits, lbits)(zghz3733, true);
    }
    unit zgsz3289;
    zgsz3289 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3730);
      CONVERT_OF(sail_int, mach_int)(&zghz3730, INT64_C(32));
      RECREATE(lbits)(&zghz3731);
      UNDEFINED(lbits)(&zghz3731, zghz3730);
      zOSECCR_EL1 = CONVERT_OF(fbits, lbits)(zghz3731, true);
    }
    unit zgsz3288;
    zgsz3288 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3728);
      CONVERT_OF(sail_int, mach_int)(&zghz3728, INT64_C(64));
      RECREATE(lbits)(&zghz3729);
      UNDEFINED(lbits)(&zghz3729, zghz3728);
      zPAR_EL1 = CONVERT_OF(fbits, lbits)(zghz3729, true);
    }
    unit zgsz3287;
    zgsz3287 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3726);
      CONVERT_OF(sail_int, mach_int)(&zghz3726, INT64_C(32));
      RECREATE(lbits)(&zghz3727);
      UNDEFINED(lbits)(&zghz3727, zghz3726);
      zPMBIDR_EL1 = CONVERT_OF(fbits, lbits)(zghz3727, true);
    }
    unit zgsz3286;
    zgsz3286 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3724);
      CONVERT_OF(sail_int, mach_int)(&zghz3724, INT64_C(64));
      RECREATE(lbits)(&zghz3725);
      UNDEFINED(lbits)(&zghz3725, zghz3724);
      zPMBLIMITR_EL1 = CONVERT_OF(fbits, lbits)(zghz3725, true);
    }
    unit zgsz3285;
    zgsz3285 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3722);
      CONVERT_OF(sail_int, mach_int)(&zghz3722, INT64_C(64));
      RECREATE(lbits)(&zghz3723);
      UNDEFINED(lbits)(&zghz3723, zghz3722);
      zPMBPTR_EL1 = CONVERT_OF(fbits, lbits)(zghz3723, true);
    }
    unit zgsz3284;
    zgsz3284 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3720);
      CONVERT_OF(sail_int, mach_int)(&zghz3720, INT64_C(32));
      RECREATE(lbits)(&zghz3721);
      UNDEFINED(lbits)(&zghz3721, zghz3720);
      zPMBSR_EL1 = CONVERT_OF(fbits, lbits)(zghz3721, true);
    }
    unit zgsz3283;
    zgsz3283 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3718);
      CONVERT_OF(sail_int, mach_int)(&zghz3718, INT64_C(32));
      RECREATE(lbits)(&zghz3719);
      UNDEFINED(lbits)(&zghz3719, zghz3718);
      zPMCCFILTR_EL0 = CONVERT_OF(fbits, lbits)(zghz3719, true);
    }
    unit zgsz3282;
    zgsz3282 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3716);
      CONVERT_OF(sail_int, mach_int)(&zghz3716, INT64_C(32));
      RECREATE(lbits)(&zghz3717);
      UNDEFINED(lbits)(&zghz3717, zghz3716);
      zPMUSERENR_EL0 = CONVERT_OF(fbits, lbits)(zghz3717, true);
    }
    unit zgsz3281;
    zgsz3281 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3714);
      CONVERT_OF(sail_int, mach_int)(&zghz3714, INT64_C(64));
      RECREATE(lbits)(&zghz3715);
      UNDEFINED(lbits)(&zghz3715, zghz3714);
      zPMCCNTR_EL0 = CONVERT_OF(fbits, lbits)(zghz3715, true);
    }
    unit zgsz3280;
    zgsz3280 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3712);
      CONVERT_OF(sail_int, mach_int)(&zghz3712, INT64_C(64));
      RECREATE(lbits)(&zghz3713);
      UNDEFINED(lbits)(&zghz3713, zghz3712);
      zPMCEID0_EL0 = CONVERT_OF(fbits, lbits)(zghz3713, true);
    }
    unit zgsz3279;
    zgsz3279 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3710);
      CONVERT_OF(sail_int, mach_int)(&zghz3710, INT64_C(64));
      RECREATE(lbits)(&zghz3711);
      UNDEFINED(lbits)(&zghz3711, zghz3710);
      zPMCEID1_EL0 = CONVERT_OF(fbits, lbits)(zghz3711, true);
    }
    unit zgsz3278;
    zgsz3278 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3708);
      CONVERT_OF(sail_int, mach_int)(&zghz3708, INT64_C(32));
      RECREATE(lbits)(&zghz3709);
      UNDEFINED(lbits)(&zghz3709, zghz3708);
      zPMCNTENCLR_EL0 = CONVERT_OF(fbits, lbits)(zghz3709, true);
    }
    unit zgsz3277;
    zgsz3277 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3706);
      CONVERT_OF(sail_int, mach_int)(&zghz3706, INT64_C(32));
      RECREATE(lbits)(&zghz3707);
      UNDEFINED(lbits)(&zghz3707, zghz3706);
      zPMCNTENSET_EL0 = CONVERT_OF(fbits, lbits)(zghz3707, true);
    }
    unit zgsz3276;
    zgsz3276 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3704);
      CONVERT_OF(sail_int, mach_int)(&zghz3704, INT64_C(32));
      RECREATE(lbits)(&zghz3705);
      UNDEFINED(lbits)(&zghz3705, zghz3704);
      zPMCR_EL0 = CONVERT_OF(fbits, lbits)(zghz3705, true);
    }
    unit zgsz3275;
    zgsz3275 = UNIT;
  }
  {
    uint64_t zgaz3338;
    {
      RECREATE(sail_int)(&zghz3702);
      CONVERT_OF(sail_int, mach_int)(&zghz3702, INT64_C(32));
      RECREATE(lbits)(&zghz3703);
      UNDEFINED(lbits)(&zghz3703, zghz3702);
      zgaz3338 = CONVERT_OF(fbits, lbits)(zghz3703, true);
    }
    {
      RECREATE(sail_int)(&zghz3701);
      CONVERT_OF(sail_int, mach_int)(&zghz3701, INT64_C(31));
      UNDEFINED(vector_zz5vecz8z5bv32z9)(&zPMEVCNTR_EL0, zghz3701, zgaz3338);
    }
    unit zgsz3274;
    zgsz3274 = UNIT;
  }
  {
    uint64_t zgaz3339;
    {
      RECREATE(sail_int)(&zghz3699);
      CONVERT_OF(sail_int, mach_int)(&zghz3699, INT64_C(32));
      RECREATE(lbits)(&zghz3700);
      UNDEFINED(lbits)(&zghz3700, zghz3699);
      zgaz3339 = CONVERT_OF(fbits, lbits)(zghz3700, true);
    }
    {
      RECREATE(sail_int)(&zghz3698);
      CONVERT_OF(sail_int, mach_int)(&zghz3698, INT64_C(31));
      UNDEFINED(vector_zz5vecz8z5bv32z9)(&zPMEVTYPER_EL0, zghz3698, zgaz3339);
    }
    unit zgsz3273;
    zgsz3273 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3696);
      CONVERT_OF(sail_int, mach_int)(&zghz3696, INT64_C(32));
      RECREATE(lbits)(&zghz3697);
      UNDEFINED(lbits)(&zghz3697, zghz3696);
      zPMINTENCLR_EL1 = CONVERT_OF(fbits, lbits)(zghz3697, true);
    }
    unit zgsz3272;
    zgsz3272 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3694);
      CONVERT_OF(sail_int, mach_int)(&zghz3694, INT64_C(32));
      RECREATE(lbits)(&zghz3695);
      UNDEFINED(lbits)(&zghz3695, zghz3694);
      zPMINTENSET_EL1 = CONVERT_OF(fbits, lbits)(zghz3695, true);
    }
    unit zgsz3271;
    zgsz3271 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3692);
      CONVERT_OF(sail_int, mach_int)(&zghz3692, INT64_C(32));
      RECREATE(lbits)(&zghz3693);
      UNDEFINED(lbits)(&zghz3693, zghz3692);
      zPMOVSCLR_EL0 = CONVERT_OF(fbits, lbits)(zghz3693, true);
    }
    unit zgsz3270;
    zgsz3270 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3690);
      CONVERT_OF(sail_int, mach_int)(&zghz3690, INT64_C(32));
      RECREATE(lbits)(&zghz3691);
      UNDEFINED(lbits)(&zghz3691, zghz3690);
      zPMOVSSET_EL0 = CONVERT_OF(fbits, lbits)(zghz3691, true);
    }
    unit zgsz3269;
    zgsz3269 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3688);
      CONVERT_OF(sail_int, mach_int)(&zghz3688, INT64_C(32));
      RECREATE(lbits)(&zghz3689);
      UNDEFINED(lbits)(&zghz3689, zghz3688);
      zPMSCR_EL1 = CONVERT_OF(fbits, lbits)(zghz3689, true);
    }
    unit zgsz3268;
    zgsz3268 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3686);
      CONVERT_OF(sail_int, mach_int)(&zghz3686, INT64_C(32));
      RECREATE(lbits)(&zghz3687);
      UNDEFINED(lbits)(&zghz3687, zghz3686);
      zPMSCR_EL2 = CONVERT_OF(fbits, lbits)(zghz3687, true);
    }
    unit zgsz3267;
    zgsz3267 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3684);
      CONVERT_OF(sail_int, mach_int)(&zghz3684, INT64_C(32));
      RECREATE(lbits)(&zghz3685);
      UNDEFINED(lbits)(&zghz3685, zghz3684);
      zPMSELR_EL0 = CONVERT_OF(fbits, lbits)(zghz3685, true);
    }
    unit zgsz3266;
    zgsz3266 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3682);
      CONVERT_OF(sail_int, mach_int)(&zghz3682, INT64_C(64));
      RECREATE(lbits)(&zghz3683);
      UNDEFINED(lbits)(&zghz3683, zghz3682);
      zPMSEVFR_EL1 = CONVERT_OF(fbits, lbits)(zghz3683, true);
    }
    unit zgsz3265;
    zgsz3265 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3680);
      CONVERT_OF(sail_int, mach_int)(&zghz3680, INT64_C(32));
      RECREATE(lbits)(&zghz3681);
      UNDEFINED(lbits)(&zghz3681, zghz3680);
      zPMSFCR_EL1 = CONVERT_OF(fbits, lbits)(zghz3681, true);
    }
    unit zgsz3264;
    zgsz3264 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3678);
      CONVERT_OF(sail_int, mach_int)(&zghz3678, INT64_C(64));
      RECREATE(lbits)(&zghz3679);
      UNDEFINED(lbits)(&zghz3679, zghz3678);
      zPMSICR_EL1 = CONVERT_OF(fbits, lbits)(zghz3679, true);
    }
    unit zgsz3263;
    zgsz3263 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3676);
      CONVERT_OF(sail_int, mach_int)(&zghz3676, INT64_C(32));
      RECREATE(lbits)(&zghz3677);
      UNDEFINED(lbits)(&zghz3677, zghz3676);
      zPMSIDR_EL1 = CONVERT_OF(fbits, lbits)(zghz3677, true);
    }
    unit zgsz3262;
    zgsz3262 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3674);
      CONVERT_OF(sail_int, mach_int)(&zghz3674, INT64_C(32));
      RECREATE(lbits)(&zghz3675);
      UNDEFINED(lbits)(&zghz3675, zghz3674);
      zPMSIRR_EL1 = CONVERT_OF(fbits, lbits)(zghz3675, true);
    }
    unit zgsz3261;
    zgsz3261 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3672);
      CONVERT_OF(sail_int, mach_int)(&zghz3672, INT64_C(32));
      RECREATE(lbits)(&zghz3673);
      UNDEFINED(lbits)(&zghz3673, zghz3672);
      zPMSLATFR_EL1 = CONVERT_OF(fbits, lbits)(zghz3673, true);
    }
    unit zgsz3260;
    zgsz3260 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3670);
      CONVERT_OF(sail_int, mach_int)(&zghz3670, INT64_C(32));
      RECREATE(lbits)(&zghz3671);
      UNDEFINED(lbits)(&zghz3671, zghz3670);
      zPMXEVCNTR_EL0 = CONVERT_OF(fbits, lbits)(zghz3671, true);
    }
    unit zgsz3259;
    zgsz3259 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3668);
      CONVERT_OF(sail_int, mach_int)(&zghz3668, INT64_C(32));
      RECREATE(lbits)(&zghz3669);
      UNDEFINED(lbits)(&zghz3669, zghz3668);
      zPMXEVTYPER_EL0 = CONVERT_OF(fbits, lbits)(zghz3669, true);
    }
    unit zgsz3258;
    zgsz3258 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3666);
      CONVERT_OF(sail_int, mach_int)(&zghz3666, INT64_C(64));
      RECREATE(lbits)(&zghz3667);
      UNDEFINED(lbits)(&zghz3667, zghz3666);
      zREVIDR_EL1 = CONVERT_OF(fbits, lbits)(zghz3667, true);
    }
    unit zgsz3257;
    zgsz3257 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3664);
      CONVERT_OF(sail_int, mach_int)(&zghz3664, INT64_C(32));
      RECREATE(lbits)(&zghz3665);
      UNDEFINED(lbits)(&zghz3665, zghz3664);
      zRMR_EL1 = CONVERT_OF(fbits, lbits)(zghz3665, true);
    }
    unit zgsz3256;
    zgsz3256 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3662);
      CONVERT_OF(sail_int, mach_int)(&zghz3662, INT64_C(32));
      RECREATE(lbits)(&zghz3663);
      UNDEFINED(lbits)(&zghz3663, zghz3662);
      zRMR_EL2 = CONVERT_OF(fbits, lbits)(zghz3663, true);
    }
    unit zgsz3255;
    zgsz3255 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3660);
      CONVERT_OF(sail_int, mach_int)(&zghz3660, INT64_C(32));
      RECREATE(lbits)(&zghz3661);
      UNDEFINED(lbits)(&zghz3661, zghz3660);
      zRMR_EL3 = CONVERT_OF(fbits, lbits)(zghz3661, true);
    }
    unit zgsz3254;
    zgsz3254 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3659);
      CONVERT_OF(sail_int, mach_int)(&zghz3659, INT64_C(129));
      UNDEFINED(lbits)(&zRSP_EL0, zghz3659);
    }
    unit zgsz3253;
    zgsz3253 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3658);
      CONVERT_OF(sail_int, mach_int)(&zghz3658, INT64_C(129));
      UNDEFINED(lbits)(&zRTPIDR_EL0, zghz3658);
    }
    unit zgsz3252;
    zgsz3252 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3656);
      CONVERT_OF(sail_int, mach_int)(&zghz3656, INT64_C(64));
      RECREATE(lbits)(&zghz3657);
      UNDEFINED(lbits)(&zghz3657, zghz3656);
      zRVBAR_EL1 = CONVERT_OF(fbits, lbits)(zghz3657, true);
    }
    unit zgsz3251;
    zgsz3251 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3654);
      CONVERT_OF(sail_int, mach_int)(&zghz3654, INT64_C(64));
      RECREATE(lbits)(&zghz3655);
      UNDEFINED(lbits)(&zghz3655, zghz3654);
      zRVBAR_EL2 = CONVERT_OF(fbits, lbits)(zghz3655, true);
    }
    unit zgsz3250;
    zgsz3250 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3652);
      CONVERT_OF(sail_int, mach_int)(&zghz3652, INT64_C(64));
      RECREATE(lbits)(&zghz3653);
      UNDEFINED(lbits)(&zghz3653, zghz3652);
      zRVBAR_EL3 = CONVERT_OF(fbits, lbits)(zghz3653, true);
    }
    unit zgsz3249;
    zgsz3249 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3650);
      CONVERT_OF(sail_int, mach_int)(&zghz3650, INT64_C(64));
      RECREATE(lbits)(&zghz3651);
      UNDEFINED(lbits)(&zghz3651, zghz3650);
      zS3_op1_Cn_Cm_op2 = CONVERT_OF(fbits, lbits)(zghz3651, true);
    }
    unit zgsz3248;
    zgsz3248 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3649);
      CONVERT_OF(sail_int, mach_int)(&zghz3649, INT64_C(129));
      UNDEFINED(lbits)(&zCID_EL0, zghz3649);
    }
    unit zgsz3247;
    zgsz3247 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3647);
      CONVERT_OF(sail_int, mach_int)(&zghz3647, INT64_C(64));
      RECREATE(lbits)(&zghz3648);
      UNDEFINED(lbits)(&zghz3648, zghz3647);
      zSCXTNUM_EL1 = CONVERT_OF(fbits, lbits)(zghz3648, true);
    }
    unit zgsz3246;
    zgsz3246 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3645);
      CONVERT_OF(sail_int, mach_int)(&zghz3645, INT64_C(64));
      RECREATE(lbits)(&zghz3646);
      UNDEFINED(lbits)(&zghz3646, zghz3645);
      zSCXTNUM_EL2 = CONVERT_OF(fbits, lbits)(zghz3646, true);
    }
    unit zgsz3245;
    zgsz3245 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3643);
      CONVERT_OF(sail_int, mach_int)(&zghz3643, INT64_C(64));
      RECREATE(lbits)(&zghz3644);
      UNDEFINED(lbits)(&zghz3644, zghz3643);
      zSCXTNUM_EL3 = CONVERT_OF(fbits, lbits)(zghz3644, true);
    }
    unit zgsz3244;
    zgsz3244 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3641);
      CONVERT_OF(sail_int, mach_int)(&zghz3641, INT64_C(32));
      RECREATE(lbits)(&zghz3642);
      UNDEFINED(lbits)(&zghz3642, zghz3641);
      zSDER32_EL3 = CONVERT_OF(fbits, lbits)(zghz3642, true);
    }
    unit zgsz3243;
    zgsz3243 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3639);
      CONVERT_OF(sail_int, mach_int)(&zghz3639, INT64_C(32));
      RECREATE(lbits)(&zghz3640);
      UNDEFINED(lbits)(&zghz3640, zghz3639);
      zSPSR_abt = CONVERT_OF(fbits, lbits)(zghz3640, true);
    }
    unit zgsz3242;
    zgsz3242 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3637);
      CONVERT_OF(sail_int, mach_int)(&zghz3637, INT64_C(32));
      RECREATE(lbits)(&zghz3638);
      UNDEFINED(lbits)(&zghz3638, zghz3637);
      zSPSR_fiq = CONVERT_OF(fbits, lbits)(zghz3638, true);
    }
    unit zgsz3241;
    zgsz3241 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3635);
      CONVERT_OF(sail_int, mach_int)(&zghz3635, INT64_C(32));
      RECREATE(lbits)(&zghz3636);
      UNDEFINED(lbits)(&zghz3636, zghz3635);
      zSPSR_irq = CONVERT_OF(fbits, lbits)(zghz3636, true);
    }
    unit zgsz3240;
    zgsz3240 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3633);
      CONVERT_OF(sail_int, mach_int)(&zghz3633, INT64_C(32));
      RECREATE(lbits)(&zghz3634);
      UNDEFINED(lbits)(&zghz3634, zghz3633);
      zSPSR_und = CONVERT_OF(fbits, lbits)(zghz3634, true);
    }
    unit zgsz3239;
    zgsz3239 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3632);
      CONVERT_OF(sail_int, mach_int)(&zghz3632, INT64_C(129));
      UNDEFINED(lbits)(&zSP_EL0, zghz3632);
    }
    unit zgsz3238;
    zgsz3238 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3631);
      CONVERT_OF(sail_int, mach_int)(&zghz3631, INT64_C(129));
      UNDEFINED(lbits)(&zSP_EL1, zghz3631);
    }
    unit zgsz3237;
    zgsz3237 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3630);
      CONVERT_OF(sail_int, mach_int)(&zghz3630, INT64_C(129));
      UNDEFINED(lbits)(&zSP_EL2, zghz3630);
    }
    unit zgsz3236;
    zgsz3236 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3629);
      CONVERT_OF(sail_int, mach_int)(&zghz3629, INT64_C(129));
      UNDEFINED(lbits)(&zTPIDRRO_EL0, zghz3629);
    }
    unit zgsz3235;
    zgsz3235 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3628);
      CONVERT_OF(sail_int, mach_int)(&zghz3628, INT64_C(129));
      UNDEFINED(lbits)(&zTPIDR_EL0, zghz3628);
    }
    unit zgsz3234;
    zgsz3234 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3627);
      CONVERT_OF(sail_int, mach_int)(&zghz3627, INT64_C(129));
      UNDEFINED(lbits)(&zTPIDR_EL1, zghz3627);
    }
    unit zgsz3233;
    zgsz3233 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3626);
      CONVERT_OF(sail_int, mach_int)(&zghz3626, INT64_C(129));
      UNDEFINED(lbits)(&zTPIDR_EL2, zghz3626);
    }
    unit zgsz3232;
    zgsz3232 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3625);
      CONVERT_OF(sail_int, mach_int)(&zghz3625, INT64_C(129));
      UNDEFINED(lbits)(&zTPIDR_EL3, zghz3625);
    }
    unit zgsz3231;
    zgsz3231 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3623);
      CONVERT_OF(sail_int, mach_int)(&zghz3623, INT64_C(64));
      RECREATE(lbits)(&zghz3624);
      UNDEFINED(lbits)(&zghz3624, zghz3623);
      zTTBR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3624, true);
    }
    unit zgsz3230;
    zgsz3230 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3621);
      CONVERT_OF(sail_int, mach_int)(&zghz3621, INT64_C(64));
      RECREATE(lbits)(&zghz3622);
      UNDEFINED(lbits)(&zghz3622, zghz3621);
      zTTBR0_EL2 = CONVERT_OF(fbits, lbits)(zghz3622, true);
    }
    unit zgsz3229;
    zgsz3229 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3619);
      CONVERT_OF(sail_int, mach_int)(&zghz3619, INT64_C(64));
      RECREATE(lbits)(&zghz3620);
      UNDEFINED(lbits)(&zghz3620, zghz3619);
      zTTBR0_EL3 = CONVERT_OF(fbits, lbits)(zghz3620, true);
    }
    unit zgsz3228;
    zgsz3228 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3617);
      CONVERT_OF(sail_int, mach_int)(&zghz3617, INT64_C(64));
      RECREATE(lbits)(&zghz3618);
      UNDEFINED(lbits)(&zghz3618, zghz3617);
      zTTBR1_EL1 = CONVERT_OF(fbits, lbits)(zghz3618, true);
    }
    unit zgsz3227;
    zgsz3227 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3615);
      CONVERT_OF(sail_int, mach_int)(&zghz3615, INT64_C(64));
      RECREATE(lbits)(&zghz3616);
      UNDEFINED(lbits)(&zghz3616, zghz3615);
      zTTBR1_EL2 = CONVERT_OF(fbits, lbits)(zghz3616, true);
    }
    unit zgsz3226;
    zgsz3226 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3613);
      CONVERT_OF(sail_int, mach_int)(&zghz3613, INT64_C(32));
      RECREATE(lbits)(&zghz3614);
      UNDEFINED(lbits)(&zghz3614, zghz3613);
      zVSESR_EL2 = CONVERT_OF(fbits, lbits)(zghz3614, true);
    }
    unit zgsz3225;
    zgsz3225 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3611);
      CONVERT_OF(sail_int, mach_int)(&zghz3611, INT64_C(32));
      RECREATE(lbits)(&zghz3612);
      UNDEFINED(lbits)(&zghz3612, zghz3611);
      zVTCR_EL2 = CONVERT_OF(fbits, lbits)(zghz3612, true);
    }
    unit zgsz3224;
    zgsz3224 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3609);
      CONVERT_OF(sail_int, mach_int)(&zghz3609, INT64_C(64));
      RECREATE(lbits)(&zghz3610);
      UNDEFINED(lbits)(&zghz3610, zghz3609);
      zVTTBR_EL2 = CONVERT_OF(fbits, lbits)(zghz3610, true);
    }
    unit zgsz3223;
    zgsz3223 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3608);
      CONVERT_OF(sail_int, mach_int)(&zghz3608, INT64_C(129));
      UNDEFINED(lbits)(&zDDC_EL0, zghz3608);
    }
    unit zgsz3222;
    zgsz3222 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3607);
      CONVERT_OF(sail_int, mach_int)(&zghz3607, INT64_C(129));
      UNDEFINED(lbits)(&zDDC_EL1, zghz3607);
    }
    unit zgsz3221;
    zgsz3221 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3606);
      CONVERT_OF(sail_int, mach_int)(&zghz3606, INT64_C(129));
      UNDEFINED(lbits)(&zDDC_EL2, zghz3606);
    }
    unit zgsz3220;
    zgsz3220 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3605);
      CONVERT_OF(sail_int, mach_int)(&zghz3605, INT64_C(129));
      UNDEFINED(lbits)(&zDDC_EL3, zghz3605);
    }
    unit zgsz3219;
    zgsz3219 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3604);
      CONVERT_OF(sail_int, mach_int)(&zghz3604, INT64_C(129));
      UNDEFINED(lbits)(&zRDDC_EL0, zghz3604);
    }
    unit zgsz3218;
    zgsz3218 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3602);
      CONVERT_OF(sail_int, mach_int)(&zghz3602, INT64_C(32));
      RECREATE(lbits)(&zghz3603);
      UNDEFINED(lbits)(&zghz3603, zghz3602);
      zDBGDTRTX_EL0 = CONVERT_OF(fbits, lbits)(zghz3603, true);
    }
    unit zgsz3217;
    zgsz3217 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3600);
      CONVERT_OF(sail_int, mach_int)(&zghz3600, INT64_C(64));
      RECREATE(lbits)(&zghz3601);
      UNDEFINED(lbits)(&zghz3601, zghz3600);
      zICC_ASGI1R_EL1 = CONVERT_OF(fbits, lbits)(zghz3601, true);
    }
    unit zgsz3216;
    zgsz3216 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3598);
      CONVERT_OF(sail_int, mach_int)(&zghz3598, INT64_C(32));
      RECREATE(lbits)(&zghz3599);
      UNDEFINED(lbits)(&zghz3599, zghz3598);
      zICC_DIR_EL1 = CONVERT_OF(fbits, lbits)(zghz3599, true);
    }
    unit zgsz3215;
    zgsz3215 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3596);
      CONVERT_OF(sail_int, mach_int)(&zghz3596, INT64_C(32));
      RECREATE(lbits)(&zghz3597);
      UNDEFINED(lbits)(&zghz3597, zghz3596);
      zICV_DIR_EL1 = CONVERT_OF(fbits, lbits)(zghz3597, true);
    }
    unit zgsz3214;
    zgsz3214 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3594);
      CONVERT_OF(sail_int, mach_int)(&zghz3594, INT64_C(32));
      RECREATE(lbits)(&zghz3595);
      UNDEFINED(lbits)(&zghz3595, zghz3594);
      zICC_EOIR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3595, true);
    }
    unit zgsz3213;
    zgsz3213 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3592);
      CONVERT_OF(sail_int, mach_int)(&zghz3592, INT64_C(32));
      RECREATE(lbits)(&zghz3593);
      UNDEFINED(lbits)(&zghz3593, zghz3592);
      zICV_EOIR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3593, true);
    }
    unit zgsz3212;
    zgsz3212 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3590);
      CONVERT_OF(sail_int, mach_int)(&zghz3590, INT64_C(32));
      RECREATE(lbits)(&zghz3591);
      UNDEFINED(lbits)(&zghz3591, zghz3590);
      zICC_EOIR1_EL1 = CONVERT_OF(fbits, lbits)(zghz3591, true);
    }
    unit zgsz3211;
    zgsz3211 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3588);
      CONVERT_OF(sail_int, mach_int)(&zghz3588, INT64_C(32));
      RECREATE(lbits)(&zghz3589);
      UNDEFINED(lbits)(&zghz3589, zghz3588);
      zICV_EOIR1_EL1 = CONVERT_OF(fbits, lbits)(zghz3589, true);
    }
    unit zgsz3210;
    zgsz3210 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3586);
      CONVERT_OF(sail_int, mach_int)(&zghz3586, INT64_C(64));
      RECREATE(lbits)(&zghz3587);
      UNDEFINED(lbits)(&zghz3587, zghz3586);
      zICC_SGI0R_EL1 = CONVERT_OF(fbits, lbits)(zghz3587, true);
    }
    unit zgsz3209;
    zgsz3209 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3584);
      CONVERT_OF(sail_int, mach_int)(&zghz3584, INT64_C(64));
      RECREATE(lbits)(&zghz3585);
      UNDEFINED(lbits)(&zghz3585, zghz3584);
      zICC_SGI1R_EL1 = CONVERT_OF(fbits, lbits)(zghz3585, true);
    }
    unit zgsz3208;
    zgsz3208 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3582);
      CONVERT_OF(sail_int, mach_int)(&zghz3582, INT64_C(32));
      RECREATE(lbits)(&zghz3583);
      UNDEFINED(lbits)(&zghz3583, zghz3582);
      zOSLAR_EL1 = CONVERT_OF(fbits, lbits)(zghz3583, true);
    }
    unit zgsz3207;
    zgsz3207 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3580);
      CONVERT_OF(sail_int, mach_int)(&zghz3580, INT64_C(32));
      RECREATE(lbits)(&zghz3581);
      UNDEFINED(lbits)(&zghz3581, zghz3580);
      zPMSWINC_EL0 = CONVERT_OF(fbits, lbits)(zghz3581, true);
    }
    unit zgsz3206;
    zgsz3206 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3578);
      CONVERT_OF(sail_int, mach_int)(&zghz3578, INT64_C(1));
      RECREATE(lbits)(&zghz3579);
      UNDEFINED(lbits)(&zghz3579, zghz3578);
      zEventRegister = CONVERT_OF(fbits, lbits)(zghz3579, true);
    }
    unit zgsz3205;
    zgsz3205 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3575);
    {
      RECREATE(sail_int)(&zghz3577);
      CONVERT_OF(sail_int, mach_int)(&zghz3577, INT64_C(128));
      UNDEFINED(lbits)(&zghz3575, zghz3577);
    }
    {
      RECREATE(sail_int)(&zghz3576);
      CONVERT_OF(sail_int, mach_int)(&zghz3576, INT64_C(32));
      UNDEFINED(vector_zz5vecz8z5bvz9)(&z_V, zghz3576, zghz3575);
    }
    unit zgsz3204;
    zgsz3204 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3573);
      CONVERT_OF(sail_int, mach_int)(&zghz3573, INT64_C(2));
      RECREATE(lbits)(&zghz3574);
      UNDEFINED(lbits)(&zghz3574, zghz3573);
      z__saved_exception_level = CONVERT_OF(fbits, lbits)(zghz3574, true);
    }
    unit zgsz3203;
    zgsz3203 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3571);
      CONVERT_OF(sail_int, mach_int)(&zghz3571, INT64_C(48));
      RECREATE(lbits)(&zghz3572);
      UNDEFINED(lbits)(&zghz3572, zghz3571);
      z__CNTControlBase = CONVERT_OF(fbits, lbits)(zghz3572, true);
    }
    unit zgsz3202;
    zgsz3202 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3569);
      CONVERT_OF(sail_int, mach_int)(&zghz3569, INT64_C(32));
      RECREATE(lbits)(&zghz3570);
      UNDEFINED(lbits)(&zghz3570, zghz3569);
      z__ThisInstr = CONVERT_OF(fbits, lbits)(zghz3570, true);
    }
    unit zgsz3201;
    zgsz3201 = UNIT;
  }
  {
    z__ThisInstrEnc = sailgen_undefined___InstrEnc(UNIT);
    unit zgsz3200;
    zgsz3200 = UNIT;
  }
  {
    sailgen_undefined_instr_ast(&z__ThisInstrAbstract, UNIT);
    unit zgsz3199;
    zgsz3199 = UNIT;
  }
  {
    z__highest_el_aarch32 = false;
    unit zgsz3198;
    zgsz3198 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3568);
      CONVERT_OF(sail_int, mach_int)(&zghz3568, INT64_C(129));
      UNDEFINED(lbits)(&zSP_EL3, zghz3568);
    }
    unit zgsz3197;
    zgsz3197 = UNIT;
  }
  {
    RECREATE(sail_int)(&zghz3567);
    CONVERT_OF(sail_int, mach_int)(&zghz3567, INT64_C(129));
    UNDEFINED(lbits)(&zTestCap, zghz3567);
  }
  zcbz335 = UNIT;
end_function_263: ;
  return zcbz335;
end_block_exception_264: ;

  return UNIT;
}



static void finish_sailgen_initializze_registers(void)
{
  KILL(sail_int)(&zghz31226);
  KILL(lbits)(&zghz31225);
  KILL(sail_int)(&zghz31224);
  KILL(lbits)(&zghz31223);
  KILL(sail_int)(&zghz31222);
  KILL(lbits)(&zghz31221);
  KILL(sail_int)(&zghz31220);
  KILL(lbits)(&zghz31219);
  KILL(sail_int)(&zghz31218);
  KILL(lbits)(&zghz31217);
  KILL(sail_int)(&zghz31216);
  KILL(lbits)(&zghz31215);
  KILL(sail_int)(&zghz31214);
  KILL(lbits)(&zghz31213);
  KILL(sail_int)(&zghz31212);
  KILL(lbits)(&zghz31211);
  KILL(sail_int)(&zghz31210);
  KILL(sail_int)(&zghz31209);
  KILL(sail_int)(&zghz31208);
  KILL(sail_int)(&zghz31207);
  KILL(sail_int)(&zghz31206);
  KILL(sail_int)(&zghz31205);
  KILL(sail_int)(&zghz31204);
  KILL(sail_int)(&zghz31203);
  KILL(sail_int)(&zghz31202);
  KILL(sail_int)(&zghz31201);
  KILL(sail_int)(&zghz31200);
  KILL(sail_int)(&zghz31199);
  KILL(sail_int)(&zghz31198);
  KILL(sail_int)(&zghz31197);
  KILL(sail_int)(&zghz31196);
  KILL(sail_int)(&zghz31195);
  KILL(sail_int)(&zghz31194);
  KILL(sail_int)(&zghz31193);
  KILL(sail_int)(&zghz31192);
  KILL(sail_int)(&zghz31191);
  KILL(sail_int)(&zghz31190);
  KILL(sail_int)(&zghz31189);
  KILL(sail_int)(&zghz31188);
  KILL(sail_int)(&zghz31187);
  KILL(sail_int)(&zghz31186);
  KILL(sail_int)(&zghz31185);
  KILL(sail_int)(&zghz31184);
  KILL(sail_int)(&zghz31183);
  KILL(sail_int)(&zghz31182);
  KILL(sail_int)(&zghz31181);
  KILL(sail_int)(&zghz31180);
  KILL(sail_int)(&zghz31179);
  KILL(lbits)(&zghz31178);
  KILL(sail_int)(&zghz31177);
  KILL(lbits)(&zghz31176);
  KILL(sail_int)(&zghz31175);
  KILL(lbits)(&zghz31174);
  KILL(sail_int)(&zghz31173);
  KILL(lbits)(&zghz31172);
  KILL(sail_int)(&zghz31171);
  KILL(lbits)(&zghz31170);
  KILL(sail_int)(&zghz31169);
  KILL(lbits)(&zghz31168);
  KILL(sail_int)(&zghz31167);
  KILL(lbits)(&zghz31166);
  KILL(sail_int)(&zghz31165);
  KILL(lbits)(&zghz31164);
  KILL(sail_int)(&zghz31163);
  KILL(lbits)(&zghz31162);
  KILL(sail_int)(&zghz31161);
  KILL(lbits)(&zghz31160);
  KILL(sail_int)(&zghz31159);
  KILL(lbits)(&zghz31158);
  KILL(sail_int)(&zghz31157);
  KILL(lbits)(&zghz31156);
  KILL(sail_int)(&zghz31155);
  KILL(lbits)(&zghz31154);
  KILL(sail_int)(&zghz31153);
  KILL(lbits)(&zghz31152);
  KILL(sail_int)(&zghz31151);
  KILL(lbits)(&zghz31150);
  KILL(sail_int)(&zghz31149);
  KILL(sail_int)(&zghz31148);
  KILL(sail_int)(&zghz31147);
  KILL(sail_int)(&zghz31146);
  KILL(sail_int)(&zghz31145);
  KILL(sail_int)(&zghz31144);
  KILL(sail_int)(&zghz31143);
  KILL(lbits)(&zghz31142);
  KILL(sail_int)(&zghz31141);
  KILL(lbits)(&zghz31140);
  KILL(sail_int)(&zghz31139);
  KILL(lbits)(&zghz31138);
  KILL(sail_int)(&zghz31137);
  KILL(lbits)(&zghz31136);
  KILL(sail_int)(&zghz31135);
  KILL(lbits)(&zghz31134);
  KILL(sail_int)(&zghz31133);
  KILL(lbits)(&zghz31132);
  KILL(sail_int)(&zghz31131);
  KILL(lbits)(&zghz31130);
  KILL(sail_int)(&zghz31129);
  KILL(lbits)(&zghz31128);
  KILL(sail_int)(&zghz31127);
  KILL(lbits)(&zghz31126);
  KILL(sail_int)(&zghz31125);
  KILL(lbits)(&zghz31124);
  KILL(sail_int)(&zghz31123);
  KILL(lbits)(&zghz31122);
  KILL(sail_int)(&zghz31121);
  KILL(lbits)(&zghz31120);
  KILL(sail_int)(&zghz31119);
  KILL(lbits)(&zghz31118);
  KILL(sail_int)(&zghz31117);
  KILL(lbits)(&zghz31116);
  KILL(sail_int)(&zghz31115);
  KILL(lbits)(&zghz31114);
  KILL(sail_int)(&zghz31113);
  KILL(lbits)(&zghz31112);
  KILL(sail_int)(&zghz31111);
  KILL(lbits)(&zghz31110);
  KILL(sail_int)(&zghz31109);
  KILL(lbits)(&zghz31108);
  KILL(sail_int)(&zghz31107);
  KILL(lbits)(&zghz31106);
  KILL(sail_int)(&zghz31105);
  KILL(lbits)(&zghz31104);
  KILL(sail_int)(&zghz31103);
  KILL(lbits)(&zghz31102);
  KILL(sail_int)(&zghz31101);
  KILL(lbits)(&zghz31100);
  KILL(sail_int)(&zghz31099);
  KILL(lbits)(&zghz31098);
  KILL(sail_int)(&zghz31097);
  KILL(lbits)(&zghz31096);
  KILL(sail_int)(&zghz31095);
  KILL(lbits)(&zghz31094);
  KILL(sail_int)(&zghz31093);
  KILL(lbits)(&zghz31092);
  KILL(sail_int)(&zghz31091);
  KILL(lbits)(&zghz31090);
  KILL(sail_int)(&zghz31089);
  KILL(lbits)(&zghz31088);
  KILL(sail_int)(&zghz31087);
  KILL(lbits)(&zghz31086);
  KILL(sail_int)(&zghz31085);
  KILL(lbits)(&zghz31084);
  KILL(sail_int)(&zghz31083);
  KILL(lbits)(&zghz31082);
  KILL(sail_int)(&zghz31081);
  KILL(lbits)(&zghz31080);
  KILL(sail_int)(&zghz31079);
  KILL(lbits)(&zghz31078);
  KILL(sail_int)(&zghz31077);
  KILL(lbits)(&zghz31076);
  KILL(sail_int)(&zghz31075);
  KILL(lbits)(&zghz31074);
  KILL(sail_int)(&zghz31073);
  KILL(lbits)(&zghz31072);
  KILL(sail_int)(&zghz31071);
  KILL(lbits)(&zghz31070);
  KILL(sail_int)(&zghz31069);
  KILL(lbits)(&zghz31068);
  KILL(sail_int)(&zghz31067);
  KILL(lbits)(&zghz31066);
  KILL(sail_int)(&zghz31065);
  KILL(lbits)(&zghz31064);
  KILL(sail_int)(&zghz31063);
  KILL(lbits)(&zghz31062);
  KILL(sail_int)(&zghz31061);
  KILL(lbits)(&zghz31060);
  KILL(sail_int)(&zghz31059);
  KILL(lbits)(&zghz31058);
  KILL(sail_int)(&zghz31057);
  KILL(lbits)(&zghz31056);
  KILL(sail_int)(&zghz31055);
  KILL(lbits)(&zghz31054);
  KILL(sail_int)(&zghz31053);
  KILL(lbits)(&zghz31052);
  KILL(sail_int)(&zghz31051);
  KILL(lbits)(&zghz31050);
  KILL(sail_int)(&zghz31049);
  KILL(lbits)(&zghz31048);
  KILL(sail_int)(&zghz31047);
  KILL(lbits)(&zghz31046);
  KILL(sail_int)(&zghz31045);
  KILL(lbits)(&zghz31044);
  KILL(sail_int)(&zghz31043);
  KILL(lbits)(&zghz31042);
  KILL(sail_int)(&zghz31041);
  KILL(lbits)(&zghz31040);
  KILL(sail_int)(&zghz31039);
  KILL(lbits)(&zghz31038);
  KILL(sail_int)(&zghz31037);
  KILL(lbits)(&zghz31036);
  KILL(sail_int)(&zghz31035);
  KILL(lbits)(&zghz31034);
  KILL(sail_int)(&zghz31033);
  KILL(lbits)(&zghz31032);
  KILL(sail_int)(&zghz31031);
  KILL(lbits)(&zghz31030);
  KILL(sail_int)(&zghz31029);
  KILL(sail_int)(&zghz31028);
  KILL(sail_int)(&zghz31027);
  KILL(lbits)(&zghz31026);
  KILL(sail_int)(&zghz31025);
  KILL(lbits)(&zghz31024);
  KILL(sail_int)(&zghz31023);
  KILL(lbits)(&zghz31022);
  KILL(sail_int)(&zghz31021);
  KILL(lbits)(&zghz31020);
  KILL(sail_int)(&zghz31019);
  KILL(lbits)(&zghz31018);
  KILL(sail_int)(&zghz31017);
  KILL(sail_int)(&zghz31016);
  KILL(lbits)(&zghz31015);
  KILL(sail_int)(&zghz31014);
  KILL(lbits)(&zghz31013);
  KILL(sail_int)(&zghz31012);
  KILL(lbits)(&zghz31011);
  KILL(sail_int)(&zghz31010);
  KILL(lbits)(&zghz31009);
  KILL(sail_int)(&zghz31008);
  KILL(sail_int)(&zghz31007);
  KILL(lbits)(&zghz31006);
  KILL(sail_int)(&zghz31005);
  KILL(lbits)(&zghz31004);
  KILL(sail_int)(&zghz31003);
  KILL(sail_int)(&zghz31002);
  KILL(lbits)(&zghz31001);
  KILL(sail_int)(&zghz31000);
  KILL(sail_int)(&zghz3999);
  KILL(lbits)(&zghz3998);
  KILL(sail_int)(&zghz3997);
  KILL(lbits)(&zghz3996);
  KILL(sail_int)(&zghz3995);
  KILL(lbits)(&zghz3994);
  KILL(sail_int)(&zghz3993);
  KILL(lbits)(&zghz3992);
  KILL(sail_int)(&zghz3991);
  KILL(lbits)(&zghz3990);
  KILL(sail_int)(&zghz3989);
  KILL(lbits)(&zghz3988);
  KILL(sail_int)(&zghz3987);
  KILL(lbits)(&zghz3986);
  KILL(sail_int)(&zghz3985);
  KILL(lbits)(&zghz3984);
  KILL(sail_int)(&zghz3983);
  KILL(lbits)(&zghz3982);
  KILL(sail_int)(&zghz3981);
  KILL(lbits)(&zghz3980);
  KILL(sail_int)(&zghz3979);
  KILL(lbits)(&zghz3978);
  KILL(sail_int)(&zghz3977);
  KILL(lbits)(&zghz3976);
  KILL(sail_int)(&zghz3975);
  KILL(lbits)(&zghz3974);
  KILL(sail_int)(&zghz3973);
  KILL(lbits)(&zghz3972);
  KILL(sail_int)(&zghz3971);
  KILL(lbits)(&zghz3970);
  KILL(sail_int)(&zghz3969);
  KILL(lbits)(&zghz3968);
  KILL(sail_int)(&zghz3967);
  KILL(lbits)(&zghz3966);
  KILL(sail_int)(&zghz3965);
  KILL(sail_int)(&zghz3964);
  KILL(lbits)(&zghz3963);
  KILL(sail_int)(&zghz3962);
  KILL(lbits)(&zghz3961);
  KILL(sail_int)(&zghz3960);
  KILL(lbits)(&zghz3959);
  KILL(sail_int)(&zghz3958);
  KILL(lbits)(&zghz3957);
  KILL(sail_int)(&zghz3956);
  KILL(lbits)(&zghz3955);
  KILL(sail_int)(&zghz3954);
  KILL(lbits)(&zghz3953);
  KILL(sail_int)(&zghz3952);
  KILL(sail_int)(&zghz3951);
  KILL(lbits)(&zghz3950);
  KILL(sail_int)(&zghz3949);
  KILL(sail_int)(&zghz3948);
  KILL(lbits)(&zghz3947);
  KILL(sail_int)(&zghz3946);
  KILL(sail_int)(&zghz3945);
  KILL(lbits)(&zghz3944);
  KILL(sail_int)(&zghz3943);
  KILL(sail_int)(&zghz3942);
  KILL(lbits)(&zghz3941);
  KILL(sail_int)(&zghz3940);
  KILL(sail_int)(&zghz3939);
  KILL(lbits)(&zghz3938);
  KILL(sail_int)(&zghz3937);
  KILL(lbits)(&zghz3936);
  KILL(sail_int)(&zghz3935);
  KILL(lbits)(&zghz3934);
  KILL(sail_int)(&zghz3933);
  KILL(lbits)(&zghz3932);
  KILL(sail_int)(&zghz3931);
  KILL(lbits)(&zghz3930);
  KILL(sail_int)(&zghz3929);
  KILL(lbits)(&zghz3928);
  KILL(sail_int)(&zghz3927);
  KILL(lbits)(&zghz3926);
  KILL(sail_int)(&zghz3925);
  KILL(lbits)(&zghz3924);
  KILL(sail_int)(&zghz3923);
  KILL(lbits)(&zghz3922);
  KILL(sail_int)(&zghz3921);
  KILL(lbits)(&zghz3920);
  KILL(sail_int)(&zghz3919);
  KILL(lbits)(&zghz3918);
  KILL(sail_int)(&zghz3917);
  KILL(lbits)(&zghz3916);
  KILL(sail_int)(&zghz3915);
  KILL(lbits)(&zghz3914);
  KILL(sail_int)(&zghz3913);
  KILL(lbits)(&zghz3912);
  KILL(sail_int)(&zghz3911);
  KILL(lbits)(&zghz3910);
  KILL(sail_int)(&zghz3909);
  KILL(lbits)(&zghz3908);
  KILL(sail_int)(&zghz3907);
  KILL(lbits)(&zghz3906);
  KILL(sail_int)(&zghz3905);
  KILL(lbits)(&zghz3904);
  KILL(sail_int)(&zghz3903);
  KILL(lbits)(&zghz3902);
  KILL(sail_int)(&zghz3901);
  KILL(lbits)(&zghz3900);
  KILL(sail_int)(&zghz3899);
  KILL(lbits)(&zghz3898);
  KILL(sail_int)(&zghz3897);
  KILL(lbits)(&zghz3896);
  KILL(sail_int)(&zghz3895);
  KILL(lbits)(&zghz3894);
  KILL(sail_int)(&zghz3893);
  KILL(lbits)(&zghz3892);
  KILL(sail_int)(&zghz3891);
  KILL(lbits)(&zghz3890);
  KILL(sail_int)(&zghz3889);
  KILL(lbits)(&zghz3888);
  KILL(sail_int)(&zghz3887);
  KILL(lbits)(&zghz3886);
  KILL(sail_int)(&zghz3885);
  KILL(lbits)(&zghz3884);
  KILL(sail_int)(&zghz3883);
  KILL(sail_int)(&zghz3882);
  KILL(lbits)(&zghz3881);
  KILL(sail_int)(&zghz3880);
  KILL(sail_int)(&zghz3879);
  KILL(lbits)(&zghz3878);
  KILL(sail_int)(&zghz3877);
  KILL(lbits)(&zghz3876);
  KILL(sail_int)(&zghz3875);
  KILL(lbits)(&zghz3874);
  KILL(sail_int)(&zghz3873);
  KILL(sail_int)(&zghz3872);
  KILL(lbits)(&zghz3871);
  KILL(sail_int)(&zghz3870);
  KILL(lbits)(&zghz3869);
  KILL(sail_int)(&zghz3868);
  KILL(lbits)(&zghz3867);
  KILL(sail_int)(&zghz3866);
  KILL(lbits)(&zghz3865);
  KILL(sail_int)(&zghz3864);
  KILL(lbits)(&zghz3863);
  KILL(sail_int)(&zghz3862);
  KILL(lbits)(&zghz3861);
  KILL(sail_int)(&zghz3860);
  KILL(lbits)(&zghz3859);
  KILL(sail_int)(&zghz3858);
  KILL(lbits)(&zghz3857);
  KILL(sail_int)(&zghz3856);
  KILL(lbits)(&zghz3855);
  KILL(sail_int)(&zghz3854);
  KILL(lbits)(&zghz3853);
  KILL(sail_int)(&zghz3852);
  KILL(lbits)(&zghz3851);
  KILL(sail_int)(&zghz3850);
  KILL(lbits)(&zghz3849);
  KILL(sail_int)(&zghz3848);
  KILL(lbits)(&zghz3847);
  KILL(sail_int)(&zghz3846);
  KILL(lbits)(&zghz3845);
  KILL(sail_int)(&zghz3844);
  KILL(lbits)(&zghz3843);
  KILL(sail_int)(&zghz3842);
  KILL(lbits)(&zghz3841);
  KILL(sail_int)(&zghz3840);
  KILL(lbits)(&zghz3839);
  KILL(sail_int)(&zghz3838);
  KILL(lbits)(&zghz3837);
  KILL(sail_int)(&zghz3836);
  KILL(lbits)(&zghz3835);
  KILL(sail_int)(&zghz3834);
  KILL(lbits)(&zghz3833);
  KILL(sail_int)(&zghz3832);
  KILL(lbits)(&zghz3831);
  KILL(sail_int)(&zghz3830);
  KILL(lbits)(&zghz3829);
  KILL(sail_int)(&zghz3828);
  KILL(lbits)(&zghz3827);
  KILL(sail_int)(&zghz3826);
  KILL(lbits)(&zghz3825);
  KILL(sail_int)(&zghz3824);
  KILL(lbits)(&zghz3823);
  KILL(sail_int)(&zghz3822);
  KILL(lbits)(&zghz3821);
  KILL(sail_int)(&zghz3820);
  KILL(lbits)(&zghz3819);
  KILL(sail_int)(&zghz3818);
  KILL(lbits)(&zghz3817);
  KILL(sail_int)(&zghz3816);
  KILL(lbits)(&zghz3815);
  KILL(sail_int)(&zghz3814);
  KILL(lbits)(&zghz3813);
  KILL(sail_int)(&zghz3812);
  KILL(lbits)(&zghz3811);
  KILL(sail_int)(&zghz3810);
  KILL(lbits)(&zghz3809);
  KILL(sail_int)(&zghz3808);
  KILL(lbits)(&zghz3807);
  KILL(sail_int)(&zghz3806);
  KILL(lbits)(&zghz3805);
  KILL(sail_int)(&zghz3804);
  KILL(lbits)(&zghz3803);
  KILL(sail_int)(&zghz3802);
  KILL(lbits)(&zghz3801);
  KILL(sail_int)(&zghz3800);
  KILL(lbits)(&zghz3799);
  KILL(sail_int)(&zghz3798);
  KILL(lbits)(&zghz3797);
  KILL(sail_int)(&zghz3796);
  KILL(lbits)(&zghz3795);
  KILL(sail_int)(&zghz3794);
  KILL(lbits)(&zghz3793);
  KILL(sail_int)(&zghz3792);
  KILL(lbits)(&zghz3791);
  KILL(sail_int)(&zghz3790);
  KILL(lbits)(&zghz3789);
  KILL(sail_int)(&zghz3788);
  KILL(lbits)(&zghz3787);
  KILL(sail_int)(&zghz3786);
  KILL(lbits)(&zghz3785);
  KILL(sail_int)(&zghz3784);
  KILL(lbits)(&zghz3783);
  KILL(sail_int)(&zghz3782);
  KILL(lbits)(&zghz3781);
  KILL(sail_int)(&zghz3780);
  KILL(lbits)(&zghz3779);
  KILL(sail_int)(&zghz3778);
  KILL(lbits)(&zghz3777);
  KILL(sail_int)(&zghz3776);
  KILL(lbits)(&zghz3775);
  KILL(sail_int)(&zghz3774);
  KILL(lbits)(&zghz3773);
  KILL(sail_int)(&zghz3772);
  KILL(lbits)(&zghz3771);
  KILL(sail_int)(&zghz3770);
  KILL(lbits)(&zghz3769);
  KILL(sail_int)(&zghz3768);
  KILL(lbits)(&zghz3767);
  KILL(sail_int)(&zghz3766);
  KILL(lbits)(&zghz3765);
  KILL(sail_int)(&zghz3764);
  KILL(lbits)(&zghz3763);
  KILL(sail_int)(&zghz3762);
  KILL(lbits)(&zghz3761);
  KILL(sail_int)(&zghz3760);
  KILL(lbits)(&zghz3759);
  KILL(sail_int)(&zghz3758);
  KILL(lbits)(&zghz3757);
  KILL(sail_int)(&zghz3756);
  KILL(lbits)(&zghz3755);
  KILL(sail_int)(&zghz3754);
  KILL(lbits)(&zghz3753);
  KILL(sail_int)(&zghz3752);
  KILL(lbits)(&zghz3751);
  KILL(sail_int)(&zghz3750);
  KILL(lbits)(&zghz3749);
  KILL(sail_int)(&zghz3748);
  KILL(lbits)(&zghz3747);
  KILL(sail_int)(&zghz3746);
  KILL(lbits)(&zghz3745);
  KILL(sail_int)(&zghz3744);
  KILL(lbits)(&zghz3743);
  KILL(sail_int)(&zghz3742);
  KILL(lbits)(&zghz3741);
  KILL(sail_int)(&zghz3740);
  KILL(lbits)(&zghz3739);
  KILL(sail_int)(&zghz3738);
  KILL(lbits)(&zghz3737);
  KILL(sail_int)(&zghz3736);
  KILL(lbits)(&zghz3735);
  KILL(sail_int)(&zghz3734);
  KILL(lbits)(&zghz3733);
  KILL(sail_int)(&zghz3732);
  KILL(lbits)(&zghz3731);
  KILL(sail_int)(&zghz3730);
  KILL(lbits)(&zghz3729);
  KILL(sail_int)(&zghz3728);
  KILL(lbits)(&zghz3727);
  KILL(sail_int)(&zghz3726);
  KILL(lbits)(&zghz3725);
  KILL(sail_int)(&zghz3724);
  KILL(lbits)(&zghz3723);
  KILL(sail_int)(&zghz3722);
  KILL(lbits)(&zghz3721);
  KILL(sail_int)(&zghz3720);
  KILL(lbits)(&zghz3719);
  KILL(sail_int)(&zghz3718);
  KILL(lbits)(&zghz3717);
  KILL(sail_int)(&zghz3716);
  KILL(lbits)(&zghz3715);
  KILL(sail_int)(&zghz3714);
  KILL(lbits)(&zghz3713);
  KILL(sail_int)(&zghz3712);
  KILL(lbits)(&zghz3711);
  KILL(sail_int)(&zghz3710);
  KILL(lbits)(&zghz3709);
  KILL(sail_int)(&zghz3708);
  KILL(lbits)(&zghz3707);
  KILL(sail_int)(&zghz3706);
  KILL(lbits)(&zghz3705);
  KILL(sail_int)(&zghz3704);
  KILL(lbits)(&zghz3703);
  KILL(sail_int)(&zghz3702);
  KILL(sail_int)(&zghz3701);
  KILL(lbits)(&zghz3700);
  KILL(sail_int)(&zghz3699);
  KILL(sail_int)(&zghz3698);
  KILL(lbits)(&zghz3697);
  KILL(sail_int)(&zghz3696);
  KILL(lbits)(&zghz3695);
  KILL(sail_int)(&zghz3694);
  KILL(lbits)(&zghz3693);
  KILL(sail_int)(&zghz3692);
  KILL(lbits)(&zghz3691);
  KILL(sail_int)(&zghz3690);
  KILL(lbits)(&zghz3689);
  KILL(sail_int)(&zghz3688);
  KILL(lbits)(&zghz3687);
  KILL(sail_int)(&zghz3686);
  KILL(lbits)(&zghz3685);
  KILL(sail_int)(&zghz3684);
  KILL(lbits)(&zghz3683);
  KILL(sail_int)(&zghz3682);
  KILL(lbits)(&zghz3681);
  KILL(sail_int)(&zghz3680);
  KILL(lbits)(&zghz3679);
  KILL(sail_int)(&zghz3678);
  KILL(lbits)(&zghz3677);
  KILL(sail_int)(&zghz3676);
  KILL(lbits)(&zghz3675);
  KILL(sail_int)(&zghz3674);
  KILL(lbits)(&zghz3673);
  KILL(sail_int)(&zghz3672);
  KILL(lbits)(&zghz3671);
  KILL(sail_int)(&zghz3670);
  KILL(lbits)(&zghz3669);
  KILL(sail_int)(&zghz3668);
  KILL(lbits)(&zghz3667);
  KILL(sail_int)(&zghz3666);
  KILL(lbits)(&zghz3665);
  KILL(sail_int)(&zghz3664);
  KILL(lbits)(&zghz3663);
  KILL(sail_int)(&zghz3662);
  KILL(lbits)(&zghz3661);
  KILL(sail_int)(&zghz3660);
  KILL(sail_int)(&zghz3659);
  KILL(sail_int)(&zghz3658);
  KILL(lbits)(&zghz3657);
  KILL(sail_int)(&zghz3656);
  KILL(lbits)(&zghz3655);
  KILL(sail_int)(&zghz3654);
  KILL(lbits)(&zghz3653);
  KILL(sail_int)(&zghz3652);
  KILL(lbits)(&zghz3651);
  KILL(sail_int)(&zghz3650);
  KILL(sail_int)(&zghz3649);
  KILL(lbits)(&zghz3648);
  KILL(sail_int)(&zghz3647);
  KILL(lbits)(&zghz3646);
  KILL(sail_int)(&zghz3645);
  KILL(lbits)(&zghz3644);
  KILL(sail_int)(&zghz3643);
  KILL(lbits)(&zghz3642);
  KILL(sail_int)(&zghz3641);
  KILL(lbits)(&zghz3640);
  KILL(sail_int)(&zghz3639);
  KILL(lbits)(&zghz3638);
  KILL(sail_int)(&zghz3637);
  KILL(lbits)(&zghz3636);
  KILL(sail_int)(&zghz3635);
  KILL(lbits)(&zghz3634);
  KILL(sail_int)(&zghz3633);
  KILL(sail_int)(&zghz3632);
  KILL(sail_int)(&zghz3631);
  KILL(sail_int)(&zghz3630);
  KILL(sail_int)(&zghz3629);
  KILL(sail_int)(&zghz3628);
  KILL(sail_int)(&zghz3627);
  KILL(sail_int)(&zghz3626);
  KILL(sail_int)(&zghz3625);
  KILL(lbits)(&zghz3624);
  KILL(sail_int)(&zghz3623);
  KILL(lbits)(&zghz3622);
  KILL(sail_int)(&zghz3621);
  KILL(lbits)(&zghz3620);
  KILL(sail_int)(&zghz3619);
  KILL(lbits)(&zghz3618);
  KILL(sail_int)(&zghz3617);
  KILL(lbits)(&zghz3616);
  KILL(sail_int)(&zghz3615);
  KILL(lbits)(&zghz3614);
  KILL(sail_int)(&zghz3613);
  KILL(lbits)(&zghz3612);
  KILL(sail_int)(&zghz3611);
  KILL(lbits)(&zghz3610);
  KILL(sail_int)(&zghz3609);
  KILL(sail_int)(&zghz3608);
  KILL(sail_int)(&zghz3607);
  KILL(sail_int)(&zghz3606);
  KILL(sail_int)(&zghz3605);
  KILL(sail_int)(&zghz3604);
  KILL(lbits)(&zghz3603);
  KILL(sail_int)(&zghz3602);
  KILL(lbits)(&zghz3601);
  KILL(sail_int)(&zghz3600);
  KILL(lbits)(&zghz3599);
  KILL(sail_int)(&zghz3598);
  KILL(lbits)(&zghz3597);
  KILL(sail_int)(&zghz3596);
  KILL(lbits)(&zghz3595);
  KILL(sail_int)(&zghz3594);
  KILL(lbits)(&zghz3593);
  KILL(sail_int)(&zghz3592);
  KILL(lbits)(&zghz3591);
  KILL(sail_int)(&zghz3590);
  KILL(lbits)(&zghz3589);
  KILL(sail_int)(&zghz3588);
  KILL(lbits)(&zghz3587);
  KILL(sail_int)(&zghz3586);
  KILL(lbits)(&zghz3585);
  KILL(sail_int)(&zghz3584);
  KILL(lbits)(&zghz3583);
  KILL(sail_int)(&zghz3582);
  KILL(lbits)(&zghz3581);
  KILL(sail_int)(&zghz3580);
  KILL(lbits)(&zghz3579);
  KILL(sail_int)(&zghz3578);
  KILL(sail_int)(&zghz3577);
  KILL(sail_int)(&zghz3576);
  KILL(lbits)(&zghz3575);
  KILL(lbits)(&zghz3574);
  KILL(sail_int)(&zghz3573);
  KILL(lbits)(&zghz3572);
  KILL(sail_int)(&zghz3571);
  KILL(lbits)(&zghz3570);
  KILL(sail_int)(&zghz3569);
  KILL(sail_int)(&zghz3568);
  KILL(sail_int)(&zghz3567);
}

static void model_init(void)
{
  setup_rts();
  current_exception = sail_malloc(sizeof(struct zexception));
  CREATE(zexception)(current_exception);
  throw_location = sail_malloc(sizeof(sail_string));
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
  startup_sailgen_CapGetObjectType();
  startup_sailgen_CapGetPermissions();
  startup_sailgen_undefined_instr_ast();
  startup_sailgen_CapUnsignedGreaterThanOrEqual();
  startup_sailgen_CapIsRepresentableFast();
  startup_sailgen_CapUnsignedLessThanOrEqual();
  startup_sailgen_CapGetTag();
  startup_sailgen_CapSetBounds();
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
  CREATE(zinstr_ast)(&z__ThisInstrAbstract);
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
  KILL(zinstr_ast)(&z__ThisInstrAbstract);
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
  finish_sailgen_CapGetObjectType();
  finish_sailgen_CapGetPermissions();
  finish_sailgen_undefined_instr_ast();
  finish_sailgen_CapUnsignedGreaterThanOrEqual();
  finish_sailgen_CapIsRepresentableFast();
  finish_sailgen_CapUnsignedLessThanOrEqual();
  finish_sailgen_CapGetTag();
  finish_sailgen_CapSetBounds();
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


