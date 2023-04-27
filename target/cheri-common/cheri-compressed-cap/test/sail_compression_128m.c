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
  __label__ end_function_4, end_block_exception_5, end_function_242;

  COPY(sail_int)((*(&zcbz31)), zx);
end_function_4: ;
  goto end_function_242;
end_block_exception_5: ;
  goto end_function_242;
end_function_242: ;
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
  __label__ end_function_10, end_block_exception_11, end_function_241;

  RECREATE(lbits)(&zghz30);
  zeros(&zghz30, zn);
  not_bits((*(&zcbz33)), zghz30);
end_function_10: ;
  goto end_function_241;
end_block_exception_11: ;
  goto end_function_241;
end_function_241: ;
}



static void finish_sailgen_sail_ones(void)
{    KILL(lbits)(&zghz30);
}









static void sailgen_Ones(lbits *rop, sail_int);

static void sailgen_Ones(lbits *zcbz34, sail_int zn)
{
  __label__ end_function_13, end_block_exception_14, end_function_240;

  sailgen_sail_ones((*(&zcbz34)), zn);
end_function_13: ;
  goto end_function_240;
end_block_exception_14: ;
  goto end_function_240;
end_function_240: ;
}

static void sailgen_Zeros(lbits *rop, sail_int);

static void sailgen_Zeros(lbits *zcbz35, sail_int zn)
{
  __label__ end_function_16, end_block_exception_17, end_function_239;

  zeros((*(&zcbz35)), zn);
end_function_16: ;
  goto end_function_239;
end_block_exception_17: ;
  goto end_function_239;
end_function_239: ;
}

static fbits sailgen_Bit(uint64_t);

static fbits sailgen_Bit(uint64_t zb)
{
  __label__ end_function_19, end_block_exception_20;

  fbits zcbz36;
  zcbz36 = (UINT64_C(1) & (zb >> INT64_C(0)));
end_function_19: ;
  return zcbz36;
end_block_exception_20: ;

  return UINT64_C(0);
}

static void sailgen_integer_subrange(lbits *rop, sail_int, sail_int, sail_int);

sail_int zghz31;
sail_int zghz32;
sail_int zghz33;

static void startup_sailgen_integer_subrange(void)
{
  CREATE(sail_int)(&zghz31);
  CREATE(sail_int)(&zghz32);
  CREATE(sail_int)(&zghz33);
}

static void sailgen_integer_subrange(lbits *zcbz37, sail_int zi, sail_int zhi, sail_int zlo)
{
  __label__ end_function_22, end_block_exception_23, end_function_238;

  RECREATE(sail_int)(&zghz31);
  {
    RECREATE(sail_int)(&zghz32);
    sub_int(&zghz32, zhi, zlo);
    {
      RECREATE(sail_int)(&zghz33);
      CONVERT_OF(sail_int, mach_int)(&zghz33, INT64_C(1));
      add_int(&zghz31, zghz32, zghz33);
    }
  }
  get_slice_int((*(&zcbz37)), zghz31, zi, zlo);
end_function_22: ;
  goto end_function_238;
end_block_exception_23: ;
  goto end_function_238;
end_function_238: ;
}



static void finish_sailgen_integer_subrange(void)
{
  KILL(sail_int)(&zghz33);
  KILL(sail_int)(&zghz32);
  KILL(sail_int)(&zghz31);
}





static void sailgen_Slice_int(lbits *rop, sail_int, sail_int, sail_int);

static void sailgen_Slice_int(lbits *zcbz38, sail_int zi, sail_int zl, sail_int zn)
{
  __label__ end_function_25, end_block_exception_26, end_function_237;

  get_slice_int((*(&zcbz38)), zn, zi, zl);
end_function_25: ;
  goto end_function_237;
end_block_exception_26: ;
  goto end_function_237;
end_function_237: ;
}

static enum zsignal sailgen_undefined_signal(unit);

static enum zsignal sailgen_undefined_signal(unit zgsz30)
{
  __label__ end_function_28, end_block_exception_29;

  enum zsignal zcbz39;
  zcbz39 = zLOW;
end_function_28: ;
  return zcbz39;
end_block_exception_29: ;

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

let_end_30: ;
}
static void kill_letbind_0(void) {
}

static uint64_t zEL2;
static void create_letbind_1(void) {


  uint64_t zgsz32;
  zgsz32 = UINT64_C(0b10);
  zEL2 = zgsz32;

let_end_31: ;
}
static void kill_letbind_1(void) {
}

static uint64_t zEL3;
static void create_letbind_2(void) {


  uint64_t zgsz33;
  zgsz33 = UINT64_C(0b11);
  zEL3 = zgsz33;

let_end_32: ;
}
static void kill_letbind_2(void) {
}

// register SCR_EL3
static uint64_t zSCR_EL3;

// register HCR_EL2
static uint64_t zHCR_EL2;

static struct zProcState sailgen_undefined_ProcState(unit);

struct zProcState zghz34;
sail_int zghz35;
lbits zghz36;
sail_int zghz37;
lbits zghz38;
sail_int zghz39;
lbits zghz310;
sail_int zghz311;
lbits zghz312;
sail_int zghz313;
lbits zghz314;
sail_int zghz315;
lbits zghz316;
sail_int zghz317;
lbits zghz318;
sail_int zghz319;
lbits zghz320;
sail_int zghz321;
lbits zghz322;
sail_int zghz323;
lbits zghz324;
sail_int zghz325;
lbits zghz326;
sail_int zghz327;
lbits zghz328;
sail_int zghz329;
lbits zghz330;
sail_int zghz331;
lbits zghz332;
sail_int zghz333;
lbits zghz334;
sail_int zghz335;
lbits zghz336;
sail_int zghz337;
lbits zghz338;
sail_int zghz339;
lbits zghz340;
sail_int zghz341;
lbits zghz342;
sail_int zghz343;
lbits zghz344;
sail_int zghz345;
lbits zghz346;
sail_int zghz347;
lbits zghz348;
sail_int zghz349;
lbits zghz350;
sail_int zghz351;
lbits zghz352;
sail_int zghz353;
lbits zghz354;
sail_int zghz355;
lbits zghz356;

static void startup_sailgen_undefined_ProcState(void)
{

  CREATE(sail_int)(&zghz35);
  CREATE(lbits)(&zghz36);
  CREATE(sail_int)(&zghz37);
  CREATE(lbits)(&zghz38);
  CREATE(sail_int)(&zghz39);
  CREATE(lbits)(&zghz310);
  CREATE(sail_int)(&zghz311);
  CREATE(lbits)(&zghz312);
  CREATE(sail_int)(&zghz313);
  CREATE(lbits)(&zghz314);
  CREATE(sail_int)(&zghz315);
  CREATE(lbits)(&zghz316);
  CREATE(sail_int)(&zghz317);
  CREATE(lbits)(&zghz318);
  CREATE(sail_int)(&zghz319);
  CREATE(lbits)(&zghz320);
  CREATE(sail_int)(&zghz321);
  CREATE(lbits)(&zghz322);
  CREATE(sail_int)(&zghz323);
  CREATE(lbits)(&zghz324);
  CREATE(sail_int)(&zghz325);
  CREATE(lbits)(&zghz326);
  CREATE(sail_int)(&zghz327);
  CREATE(lbits)(&zghz328);
  CREATE(sail_int)(&zghz329);
  CREATE(lbits)(&zghz330);
  CREATE(sail_int)(&zghz331);
  CREATE(lbits)(&zghz332);
  CREATE(sail_int)(&zghz333);
  CREATE(lbits)(&zghz334);
  CREATE(sail_int)(&zghz335);
  CREATE(lbits)(&zghz336);
  CREATE(sail_int)(&zghz337);
  CREATE(lbits)(&zghz338);
  CREATE(sail_int)(&zghz339);
  CREATE(lbits)(&zghz340);
  CREATE(sail_int)(&zghz341);
  CREATE(lbits)(&zghz342);
  CREATE(sail_int)(&zghz343);
  CREATE(lbits)(&zghz344);
  CREATE(sail_int)(&zghz345);
  CREATE(lbits)(&zghz346);
  CREATE(sail_int)(&zghz347);
  CREATE(lbits)(&zghz348);
  CREATE(sail_int)(&zghz349);
  CREATE(lbits)(&zghz350);
  CREATE(sail_int)(&zghz351);
  CREATE(lbits)(&zghz352);
  CREATE(sail_int)(&zghz353);
  CREATE(lbits)(&zghz354);
  CREATE(sail_int)(&zghz355);
  CREATE(lbits)(&zghz356);
}

static struct zProcState sailgen_undefined_ProcState(unit zgsz34)
{
  __label__ end_function_34, end_block_exception_35;

  struct zProcState zghz34;
  uint64_t zgaz35;
  {
    RECREATE(sail_int)(&zghz355);
    CONVERT_OF(sail_int, mach_int)(&zghz355, INT64_C(1));
    RECREATE(lbits)(&zghz356);
    UNDEFINED(lbits)(&zghz356, zghz355);
    zgaz35 = CONVERT_OF(fbits, lbits)(zghz356, true);
  }
  uint64_t zgaz36;
  {
    RECREATE(sail_int)(&zghz353);
    CONVERT_OF(sail_int, mach_int)(&zghz353, INT64_C(1));
    RECREATE(lbits)(&zghz354);
    UNDEFINED(lbits)(&zghz354, zghz353);
    zgaz36 = CONVERT_OF(fbits, lbits)(zghz354, true);
  }
  uint64_t zgaz37;
  {
    RECREATE(sail_int)(&zghz351);
    CONVERT_OF(sail_int, mach_int)(&zghz351, INT64_C(1));
    RECREATE(lbits)(&zghz352);
    UNDEFINED(lbits)(&zghz352, zghz351);
    zgaz37 = CONVERT_OF(fbits, lbits)(zghz352, true);
  }
  uint64_t zgaz38;
  {
    RECREATE(sail_int)(&zghz349);
    CONVERT_OF(sail_int, mach_int)(&zghz349, INT64_C(1));
    RECREATE(lbits)(&zghz350);
    UNDEFINED(lbits)(&zghz350, zghz349);
    zgaz38 = CONVERT_OF(fbits, lbits)(zghz350, true);
  }
  uint64_t zgaz39;
  {
    RECREATE(sail_int)(&zghz347);
    CONVERT_OF(sail_int, mach_int)(&zghz347, INT64_C(1));
    RECREATE(lbits)(&zghz348);
    UNDEFINED(lbits)(&zghz348, zghz347);
    zgaz39 = CONVERT_OF(fbits, lbits)(zghz348, true);
  }
  uint64_t zgaz310;
  {
    RECREATE(sail_int)(&zghz345);
    CONVERT_OF(sail_int, mach_int)(&zghz345, INT64_C(1));
    RECREATE(lbits)(&zghz346);
    UNDEFINED(lbits)(&zghz346, zghz345);
    zgaz310 = CONVERT_OF(fbits, lbits)(zghz346, true);
  }
  uint64_t zgaz311;
  {
    RECREATE(sail_int)(&zghz343);
    CONVERT_OF(sail_int, mach_int)(&zghz343, INT64_C(1));
    RECREATE(lbits)(&zghz344);
    UNDEFINED(lbits)(&zghz344, zghz343);
    zgaz311 = CONVERT_OF(fbits, lbits)(zghz344, true);
  }
  uint64_t zgaz312;
  {
    RECREATE(sail_int)(&zghz341);
    CONVERT_OF(sail_int, mach_int)(&zghz341, INT64_C(1));
    RECREATE(lbits)(&zghz342);
    UNDEFINED(lbits)(&zghz342, zghz341);
    zgaz312 = CONVERT_OF(fbits, lbits)(zghz342, true);
  }
  uint64_t zgaz313;
  {
    RECREATE(sail_int)(&zghz339);
    CONVERT_OF(sail_int, mach_int)(&zghz339, INT64_C(1));
    RECREATE(lbits)(&zghz340);
    UNDEFINED(lbits)(&zghz340, zghz339);
    zgaz313 = CONVERT_OF(fbits, lbits)(zghz340, true);
  }
  uint64_t zgaz314;
  {
    RECREATE(sail_int)(&zghz337);
    CONVERT_OF(sail_int, mach_int)(&zghz337, INT64_C(1));
    RECREATE(lbits)(&zghz338);
    UNDEFINED(lbits)(&zghz338, zghz337);
    zgaz314 = CONVERT_OF(fbits, lbits)(zghz338, true);
  }
  uint64_t zgaz315;
  {
    RECREATE(sail_int)(&zghz335);
    CONVERT_OF(sail_int, mach_int)(&zghz335, INT64_C(1));
    RECREATE(lbits)(&zghz336);
    UNDEFINED(lbits)(&zghz336, zghz335);
    zgaz315 = CONVERT_OF(fbits, lbits)(zghz336, true);
  }
  uint64_t zgaz316;
  {
    RECREATE(sail_int)(&zghz333);
    CONVERT_OF(sail_int, mach_int)(&zghz333, INT64_C(1));
    RECREATE(lbits)(&zghz334);
    UNDEFINED(lbits)(&zghz334, zghz333);
    zgaz316 = CONVERT_OF(fbits, lbits)(zghz334, true);
  }
  uint64_t zgaz317;
  {
    RECREATE(sail_int)(&zghz331);
    CONVERT_OF(sail_int, mach_int)(&zghz331, INT64_C(1));
    RECREATE(lbits)(&zghz332);
    UNDEFINED(lbits)(&zghz332, zghz331);
    zgaz317 = CONVERT_OF(fbits, lbits)(zghz332, true);
  }
  uint64_t zgaz318;
  {
    RECREATE(sail_int)(&zghz329);
    CONVERT_OF(sail_int, mach_int)(&zghz329, INT64_C(1));
    RECREATE(lbits)(&zghz330);
    UNDEFINED(lbits)(&zghz330, zghz329);
    zgaz318 = CONVERT_OF(fbits, lbits)(zghz330, true);
  }
  uint64_t zgaz319;
  {
    RECREATE(sail_int)(&zghz327);
    CONVERT_OF(sail_int, mach_int)(&zghz327, INT64_C(1));
    RECREATE(lbits)(&zghz328);
    UNDEFINED(lbits)(&zghz328, zghz327);
    zgaz319 = CONVERT_OF(fbits, lbits)(zghz328, true);
  }
  uint64_t zgaz320;
  {
    RECREATE(sail_int)(&zghz325);
    CONVERT_OF(sail_int, mach_int)(&zghz325, INT64_C(2));
    RECREATE(lbits)(&zghz326);
    UNDEFINED(lbits)(&zghz326, zghz325);
    zgaz320 = CONVERT_OF(fbits, lbits)(zghz326, true);
  }
  uint64_t zgaz321;
  {
    RECREATE(sail_int)(&zghz323);
    CONVERT_OF(sail_int, mach_int)(&zghz323, INT64_C(1));
    RECREATE(lbits)(&zghz324);
    UNDEFINED(lbits)(&zghz324, zghz323);
    zgaz321 = CONVERT_OF(fbits, lbits)(zghz324, true);
  }
  uint64_t zgaz322;
  {
    RECREATE(sail_int)(&zghz321);
    CONVERT_OF(sail_int, mach_int)(&zghz321, INT64_C(1));
    RECREATE(lbits)(&zghz322);
    UNDEFINED(lbits)(&zghz322, zghz321);
    zgaz322 = CONVERT_OF(fbits, lbits)(zghz322, true);
  }
  uint64_t zgaz323;
  {
    RECREATE(sail_int)(&zghz319);
    CONVERT_OF(sail_int, mach_int)(&zghz319, INT64_C(1));
    RECREATE(lbits)(&zghz320);
    UNDEFINED(lbits)(&zghz320, zghz319);
    zgaz323 = CONVERT_OF(fbits, lbits)(zghz320, true);
  }
  uint64_t zgaz324;
  {
    RECREATE(sail_int)(&zghz317);
    CONVERT_OF(sail_int, mach_int)(&zghz317, INT64_C(4));
    RECREATE(lbits)(&zghz318);
    UNDEFINED(lbits)(&zghz318, zghz317);
    zgaz324 = CONVERT_OF(fbits, lbits)(zghz318, true);
  }
  uint64_t zgaz325;
  {
    RECREATE(sail_int)(&zghz315);
    CONVERT_OF(sail_int, mach_int)(&zghz315, INT64_C(1));
    RECREATE(lbits)(&zghz316);
    UNDEFINED(lbits)(&zghz316, zghz315);
    zgaz325 = CONVERT_OF(fbits, lbits)(zghz316, true);
  }
  uint64_t zgaz326;
  {
    RECREATE(sail_int)(&zghz313);
    CONVERT_OF(sail_int, mach_int)(&zghz313, INT64_C(8));
    RECREATE(lbits)(&zghz314);
    UNDEFINED(lbits)(&zghz314, zghz313);
    zgaz326 = CONVERT_OF(fbits, lbits)(zghz314, true);
  }
  uint64_t zgaz327;
  {
    RECREATE(sail_int)(&zghz311);
    CONVERT_OF(sail_int, mach_int)(&zghz311, INT64_C(1));
    RECREATE(lbits)(&zghz312);
    UNDEFINED(lbits)(&zghz312, zghz311);
    zgaz327 = CONVERT_OF(fbits, lbits)(zghz312, true);
  }
  uint64_t zgaz328;
  {
    RECREATE(sail_int)(&zghz39);
    CONVERT_OF(sail_int, mach_int)(&zghz39, INT64_C(1));
    RECREATE(lbits)(&zghz310);
    UNDEFINED(lbits)(&zghz310, zghz39);
    zgaz328 = CONVERT_OF(fbits, lbits)(zghz310, true);
  }
  uint64_t zgaz329;
  {
    RECREATE(sail_int)(&zghz37);
    CONVERT_OF(sail_int, mach_int)(&zghz37, INT64_C(1));
    RECREATE(lbits)(&zghz38);
    UNDEFINED(lbits)(&zghz38, zghz37);
    zgaz329 = CONVERT_OF(fbits, lbits)(zghz38, true);
  }
  uint64_t zgaz330;
  {
    RECREATE(sail_int)(&zghz35);
    CONVERT_OF(sail_int, mach_int)(&zghz35, INT64_C(5));
    RECREATE(lbits)(&zghz36);
    UNDEFINED(lbits)(&zghz36, zghz35);
    zgaz330 = CONVERT_OF(fbits, lbits)(zghz36, true);
  }
  zghz34.zA = zgaz310;
  zghz34.zC = zgaz37;
  zghz34.zC64 = zgaz317;
  zghz34.zD = zgaz39;
  zghz34.zDIT = zgaz315;
  zghz34.zE = zgaz329;
  zghz34.zEL = zgaz320;
  zghz34.zF = zgaz312;
  zghz34.zGE = zgaz324;
  zghz34.zI = zgaz311;
  zghz34.zIL = zgaz319;
  zghz34.zIT = zgaz326;
  zghz34.zJ = zgaz327;
  zghz34.zM = zgaz330;
  zghz34.zN = zgaz35;
  zghz34.zPAN = zgaz313;
  zghz34.zQ = zgaz323;
  zghz34.zSP = zgaz322;
  zghz34.zSS = zgaz318;
  zghz34.zSSBS = zgaz325;
  zghz34.zT = zgaz328;
  zghz34.zTCO = zgaz316;
  zghz34.zUAO = zgaz314;
  zghz34.zV = zgaz38;
  zghz34.zZ = zgaz36;
  zghz34.znRW = zgaz321;


























end_function_34: ;
  return zghz34;
end_block_exception_35: ;
  struct zProcState zcbz332 = { .znRW = UINT64_C(0xdeadc0de), .zZ = UINT64_C(0xdeadc0de), .zV = UINT64_C(0xdeadc0de), .zUAO = UINT64_C(0xdeadc0de), .zTCO = UINT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zSSBS = UINT64_C(0xdeadc0de), .zSS = UINT64_C(0xdeadc0de), .zSP = UINT64_C(0xdeadc0de), .zQ = UINT64_C(0xdeadc0de), .zPAN = UINT64_C(0xdeadc0de), .zN = UINT64_C(0xdeadc0de), .zM = UINT64_C(0xdeadc0de), .zJ = UINT64_C(0xdeadc0de), .zIT = UINT64_C(0xdeadc0de), .zIL = UINT64_C(0xdeadc0de), .zI = UINT64_C(0xdeadc0de), .zGE = UINT64_C(0xdeadc0de), .zF = UINT64_C(0xdeadc0de), .zEL = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zDIT = UINT64_C(0xdeadc0de), .zD = UINT64_C(0xdeadc0de), .zC64 = UINT64_C(0xdeadc0de), .zC = UINT64_C(0xdeadc0de), .zA = UINT64_C(0xdeadc0de) };
  return zcbz332;
}



static void finish_sailgen_undefined_ProcState(void)
{
  KILL(lbits)(&zghz356);
  KILL(sail_int)(&zghz355);
  KILL(lbits)(&zghz354);
  KILL(sail_int)(&zghz353);
  KILL(lbits)(&zghz352);
  KILL(sail_int)(&zghz351);
  KILL(lbits)(&zghz350);
  KILL(sail_int)(&zghz349);
  KILL(lbits)(&zghz348);
  KILL(sail_int)(&zghz347);
  KILL(lbits)(&zghz346);
  KILL(sail_int)(&zghz345);
  KILL(lbits)(&zghz344);
  KILL(sail_int)(&zghz343);
  KILL(lbits)(&zghz342);
  KILL(sail_int)(&zghz341);
  KILL(lbits)(&zghz340);
  KILL(sail_int)(&zghz339);
  KILL(lbits)(&zghz338);
  KILL(sail_int)(&zghz337);
  KILL(lbits)(&zghz336);
  KILL(sail_int)(&zghz335);
  KILL(lbits)(&zghz334);
  KILL(sail_int)(&zghz333);
  KILL(lbits)(&zghz332);
  KILL(sail_int)(&zghz331);
  KILL(lbits)(&zghz330);
  KILL(sail_int)(&zghz329);
  KILL(lbits)(&zghz328);
  KILL(sail_int)(&zghz327);
  KILL(lbits)(&zghz326);
  KILL(sail_int)(&zghz325);
  KILL(lbits)(&zghz324);
  KILL(sail_int)(&zghz323);
  KILL(lbits)(&zghz322);
  KILL(sail_int)(&zghz321);
  KILL(lbits)(&zghz320);
  KILL(sail_int)(&zghz319);
  KILL(lbits)(&zghz318);
  KILL(sail_int)(&zghz317);
  KILL(lbits)(&zghz316);
  KILL(sail_int)(&zghz315);
  KILL(lbits)(&zghz314);
  KILL(sail_int)(&zghz313);
  KILL(lbits)(&zghz312);
  KILL(sail_int)(&zghz311);
  KILL(lbits)(&zghz310);
  KILL(sail_int)(&zghz39);
  KILL(lbits)(&zghz38);
  KILL(sail_int)(&zghz37);
  KILL(lbits)(&zghz36);
  KILL(sail_int)(&zghz35);

}

// register PSTATE
static struct zProcState zPSTATE;

static uint64_t zEL0;
static void create_letbind_3(void) {


  uint64_t zgsz36;
  zgsz36 = UINT64_C(0b00);
  zEL0 = zgsz36;

let_end_36: ;
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

let_end_37: ;
}
static void kill_letbind_4(void) {
}

static int64_t zCAP_VALUE_HI_BIT;
static void create_letbind_5(void) {


  int64_t zgsz38;
  zgsz38 = INT64_C(63);
  zCAP_VALUE_HI_BIT = zgsz38;

let_end_38: ;
}
static void kill_letbind_5(void) {
}

static int64_t zCAP_VALUE_LO_BIT;
static void create_letbind_6(void) {


  int64_t zgsz39;
  zgsz39 = INT64_C(0);
  zCAP_VALUE_LO_BIT = zgsz39;

let_end_39: ;
}
static void kill_letbind_6(void) {
}

static int64_t zCAP_VALUE_NUM_BITS;
static void create_letbind_7(void) {


  int64_t zgsz310;
  int64_t zgaz331;
  {
    sail_int zgsz3589;
    CREATE(sail_int)(&zgsz3589);
    CONVERT_OF(sail_int, mach_int)(&zgsz3589, zCAP_VALUE_HI_BIT);
    sail_int zgsz3590;
    CREATE(sail_int)(&zgsz3590);
    CONVERT_OF(sail_int, mach_int)(&zgsz3590, zCAP_VALUE_LO_BIT);
    sail_int zgsz3591;
    CREATE(sail_int)(&zgsz3591);
    sub_int(&zgsz3591, zgsz3589, zgsz3590);
    zgaz331 = CONVERT_OF(mach_int, sail_int)(zgsz3591);
    KILL(sail_int)(&zgsz3591);
    KILL(sail_int)(&zgsz3590);
    KILL(sail_int)(&zgsz3589);
  }
  zgsz310 = (zgaz331 + INT64_C(1));

  zCAP_VALUE_NUM_BITS = zgsz310;

let_end_40: ;
}
static void kill_letbind_7(void) {
}

static int64_t zCAP_BASE_HI_BIT;
static void create_letbind_8(void) {


  int64_t zgsz311;
  zgsz311 = INT64_C(79);
  zCAP_BASE_HI_BIT = zgsz311;

let_end_41: ;
}
static void kill_letbind_8(void) {
}

static int64_t zCAP_BASE_LO_BIT;
static void create_letbind_9(void) {


  int64_t zgsz312;
  zgsz312 = INT64_C(64);
  zCAP_BASE_LO_BIT = zgsz312;

let_end_42: ;
}
static void kill_letbind_9(void) {
}

static int64_t zCAP_MW;
static void create_letbind_10(void) {


  int64_t zgsz313;
  int64_t zgaz332;
  {
    sail_int zgsz3592;
    CREATE(sail_int)(&zgsz3592);
    CONVERT_OF(sail_int, mach_int)(&zgsz3592, zCAP_BASE_HI_BIT);
    sail_int zgsz3593;
    CREATE(sail_int)(&zgsz3593);
    CONVERT_OF(sail_int, mach_int)(&zgsz3593, zCAP_BASE_LO_BIT);
    sail_int zgsz3594;
    CREATE(sail_int)(&zgsz3594);
    sub_int(&zgsz3594, zgsz3592, zgsz3593);
    zgaz332 = CONVERT_OF(mach_int, sail_int)(zgsz3594);
    KILL(sail_int)(&zgsz3594);
    KILL(sail_int)(&zgsz3593);
    KILL(sail_int)(&zgsz3592);
  }
  zgsz313 = (zgaz332 + INT64_C(1));

  zCAP_MW = zgsz313;

let_end_43: ;
}
static void kill_letbind_10(void) {
}

static bool sailgen_CapBoundsUsesValue(sail_int);

sail_int zghz357;
sail_int zghz358;
sail_int zghz359;

static void startup_sailgen_CapBoundsUsesValue(void)
{
  CREATE(sail_int)(&zghz357);
  CREATE(sail_int)(&zghz358);
  CREATE(sail_int)(&zghz359);
}

static bool sailgen_CapBoundsUsesValue(sail_int zexp)
{
  __label__ cleanup_46, end_cleanup_47, end_function_45, end_block_exception_48;

  bool zcbz311;
  {
    RECREATE(sail_int)(&zghz357);
    {
      RECREATE(sail_int)(&zghz359);
      CONVERT_OF(sail_int, mach_int)(&zghz359, zCAP_MW);
      add_int(&zghz357, zexp, zghz359);
    }
    {
      RECREATE(sail_int)(&zghz358);
      CONVERT_OF(sail_int, mach_int)(&zghz358, zCAP_VALUE_NUM_BITS);
      zcbz311 = lt(zghz357, zghz358);
    }
  }
  goto cleanup_46;
  /* unreachable after return */
  goto end_cleanup_47;
cleanup_46: ;
  goto end_function_45;
end_cleanup_47: ;
end_function_45: ;
  return zcbz311;
end_block_exception_48: ;

  return false;
}



static void finish_sailgen_CapBoundsUsesValue(void)
{
  KILL(sail_int)(&zghz359);
  KILL(sail_int)(&zghz358);
  KILL(sail_int)(&zghz357);
}

static int64_t zCAP_BASE_EXP_HI_BIT;
static void create_letbind_11(void) {


  int64_t zgsz314;
  zgsz314 = INT64_C(66);
  zCAP_BASE_EXP_HI_BIT = zgsz314;

let_end_49: ;
}
static void kill_letbind_11(void) {
}

static int64_t zCAP_LIMIT_EXP_HI_BIT;
static void create_letbind_12(void) {


  int64_t zgsz315;
  zgsz315 = INT64_C(82);
  zCAP_LIMIT_EXP_HI_BIT = zgsz315;

let_end_50: ;
}
static void kill_letbind_12(void) {
}

static int64_t zCAP_LIMIT_LO_BIT;
static void create_letbind_13(void) {


  int64_t zgsz316;
  zgsz316 = INT64_C(80);
  zCAP_LIMIT_LO_BIT = zgsz316;

let_end_51: ;
}
static void kill_letbind_13(void) {
}

static int64_t zCAP_IE_BIT;
static void create_letbind_14(void) {


  int64_t zgsz317;
  zgsz317 = INT64_C(94);
  zCAP_IE_BIT = zgsz317;

let_end_52: ;
}
static void kill_letbind_14(void) {
}

static bool sailgen_CapIsInternalExponent(lbits);

sail_int zghz360;

static void startup_sailgen_CapIsInternalExponent(void)
{    CREATE(sail_int)(&zghz360);
}

static bool sailgen_CapIsInternalExponent(lbits zc)
{
  __label__ cleanup_55, end_cleanup_56, end_function_54, end_block_exception_57;

  bool zcbz312;
  {
    uint64_t zgaz336;
    {
      fbits zgaz335;
      {
        RECREATE(sail_int)(&zghz360);
        CONVERT_OF(sail_int, mach_int)(&zghz360, zCAP_IE_BIT);
        zgaz335 = bitvector_access(zc, zghz360);
      }
      zgaz336 = UINT64_C(0b0);
      zgaz336 = update_fbits(zgaz336, INT64_C(0), zgaz335);
    }
    zcbz312 = (zgaz336 == UINT64_C(0b0));
  }
  goto cleanup_55;
  /* unreachable after return */
  goto end_cleanup_56;
cleanup_55: ;
  goto end_function_54;
end_cleanup_56: ;
end_function_54: ;
  return zcbz312;
end_block_exception_57: ;

  return false;
}



static void finish_sailgen_CapIsInternalExponent(void)
{    KILL(sail_int)(&zghz360);
}

static int64_t sailgen_CapGetExponent(lbits);

lbits zghz361;
sail_int zghz362;
sail_int zghz363;
sail_int zghz364;
lbits zghz365;
sail_int zghz366;
sail_int zghz367;
lbits zghz368;

static void startup_sailgen_CapGetExponent(void)
{
  CREATE(lbits)(&zghz361);
  CREATE(sail_int)(&zghz362);
  CREATE(sail_int)(&zghz363);
  CREATE(sail_int)(&zghz364);
  CREATE(lbits)(&zghz365);
  CREATE(sail_int)(&zghz366);
  CREATE(sail_int)(&zghz367);
  CREATE(lbits)(&zghz368);
}

static int64_t sailgen_CapGetExponent(lbits zc)
{
  __label__ end_function_59, end_block_exception_64;

  int64_t zcbz313;
  bool zgaz338;
  zgaz338 = sailgen_CapIsInternalExponent(zc);
  if (zgaz338) {
    __label__ cleanup_62, end_cleanup_63;

    uint64_t znexp;
    {
      uint64_t zgaz341;
      {
        RECREATE(sail_int)(&zghz366);
        CONVERT_OF(sail_int, mach_int)(&zghz366, zCAP_LIMIT_EXP_HI_BIT);
        RECREATE(sail_int)(&zghz367);
        CONVERT_OF(sail_int, mach_int)(&zghz367, zCAP_LIMIT_LO_BIT);
        RECREATE(lbits)(&zghz368);
        vector_subrange_lbits(&zghz368, zc, zghz366, zghz367);
        zgaz341 = CONVERT_OF(fbits, lbits)(zghz368, true);
      }
      uint64_t zgaz342;
      {
        RECREATE(sail_int)(&zghz363);
        CONVERT_OF(sail_int, mach_int)(&zghz363, zCAP_BASE_EXP_HI_BIT);
        RECREATE(sail_int)(&zghz364);
        CONVERT_OF(sail_int, mach_int)(&zghz364, zCAP_BASE_LO_BIT);
        RECREATE(lbits)(&zghz365);
        vector_subrange_lbits(&zghz365, zc, zghz363, zghz364);
        zgaz342 = CONVERT_OF(fbits, lbits)(zghz365, true);
      }
      znexp = (zgaz341 << 3) | zgaz342;
    }
    int64_t zgaz340;
    {
      uint64_t zgaz339;
      zgaz339 = (~(znexp) & UINT64_C(0b111111));
      {
        RECREATE(lbits)(&zghz361);
        CONVERT_OF(lbits, fbits)(&zghz361, zgaz339, UINT64_C(6) , true);
        RECREATE(sail_int)(&zghz362);
        sail_unsigned(&zghz362, zghz361);
        zgaz340 = CONVERT_OF(mach_int, sail_int)(zghz362);
      }
    }
    zcbz313 = zgaz340;
    goto cleanup_62;
    /* unreachable after return */
    goto end_cleanup_63;
  cleanup_62: ;
    goto end_function_59;
  end_cleanup_63: ;
  } else {
    __label__ cleanup_60, end_cleanup_61;

    zcbz313 = INT64_C(0);
    goto cleanup_60;
    /* unreachable after return */
    goto end_cleanup_61;
  cleanup_60: ;
    goto end_function_59;
  end_cleanup_61: ;
  }

end_function_59: ;
  return zcbz313;
end_block_exception_64: ;

  return INT64_C(0xdeadc0de);
}



static void finish_sailgen_CapGetExponent(void)
{
  KILL(lbits)(&zghz368);
  KILL(sail_int)(&zghz367);
  KILL(sail_int)(&zghz366);
  KILL(lbits)(&zghz365);
  KILL(sail_int)(&zghz364);
  KILL(sail_int)(&zghz363);
  KILL(sail_int)(&zghz362);
  KILL(lbits)(&zghz361);
}

static uint64_t sailgen_CapGetValue(lbits);

sail_int zghz369;
sail_int zghz370;
lbits zghz371;

static void startup_sailgen_CapGetValue(void)
{
  CREATE(sail_int)(&zghz369);
  CREATE(sail_int)(&zghz370);
  CREATE(lbits)(&zghz371);
}

static uint64_t sailgen_CapGetValue(lbits zc)
{
  __label__ cleanup_67, end_cleanup_68, end_function_66, end_block_exception_69;

  uint64_t zcbz314;
  {
    RECREATE(sail_int)(&zghz369);
    CONVERT_OF(sail_int, mach_int)(&zghz369, zCAP_VALUE_HI_BIT);
    RECREATE(sail_int)(&zghz370);
    CONVERT_OF(sail_int, mach_int)(&zghz370, zCAP_VALUE_LO_BIT);
    RECREATE(lbits)(&zghz371);
    vector_subrange_lbits(&zghz371, zc, zghz369, zghz370);
    zcbz314 = CONVERT_OF(fbits, lbits)(zghz371, true);
  }
  goto cleanup_67;
  /* unreachable after return */
  goto end_cleanup_68;
cleanup_67: ;
  goto end_function_66;
end_cleanup_68: ;
end_function_66: ;
  return zcbz314;
end_block_exception_69: ;

  return UINT64_C(0xdeadc0de);
}



static void finish_sailgen_CapGetValue(void)
{
  KILL(lbits)(&zghz371);
  KILL(sail_int)(&zghz370);
  KILL(sail_int)(&zghz369);
}

static int64_t zCAP_BOUND_NUM_BITS;
static void create_letbind_15(void) {


  int64_t zgsz319;
  zgsz319 = (zCAP_VALUE_NUM_BITS + INT64_C(1));
  zCAP_BOUND_NUM_BITS = zgsz319;

let_end_70: ;
}
static void kill_letbind_15(void) {
}

static lbits zCAP_BOUND_MAX;
static void create_letbind_16(void) {    CREATE(lbits)(&zCAP_BOUND_MAX);


  lbits zgsz320;
  CREATE(lbits)(&zgsz320);
  sail_int zgaz344;
  CREATE(sail_int)(&zgaz344);
  {
    sail_int zgsz3611;
    CREATE(sail_int)(&zgsz3611);
    CONVERT_OF(sail_int, mach_int)(&zgsz3611, INT64_C(1));
    sail_int zgsz3612;
    CREATE(sail_int)(&zgsz3612);
    CONVERT_OF(sail_int, mach_int)(&zgsz3612, zCAP_VALUE_NUM_BITS);
    shl_int(&zgaz344, zgsz3611, zgsz3612);
    KILL(sail_int)(&zgsz3612);
    KILL(sail_int)(&zgsz3611);
  }
  {
    sail_int zgsz3609;
    CREATE(sail_int)(&zgsz3609);
    CONVERT_OF(sail_int, mach_int)(&zgsz3609, INT64_C(0));
    sail_int zgsz3610;
    CREATE(sail_int)(&zgsz3610);
    CONVERT_OF(sail_int, mach_int)(&zgsz3610, zCAP_BOUND_NUM_BITS);
    sailgen_Slice_int(&zgsz320, zgaz344, zgsz3609, zgsz3610);
    KILL(sail_int)(&zgsz3610);
    KILL(sail_int)(&zgsz3609);
  }
  KILL(sail_int)(&zgaz344);
  COPY(lbits)(&zCAP_BOUND_MAX, zgsz320);
  KILL(lbits)(&zgsz320);
let_end_71: ;
}
static void kill_letbind_16(void) {    KILL(lbits)(&zCAP_BOUND_MAX);
}

static lbits zCAP_BOUND_MIN;
static void create_letbind_17(void) {    CREATE(lbits)(&zCAP_BOUND_MIN);


  lbits zgsz321;
  CREATE(lbits)(&zgsz321);
  int64_t zgaz345;
  {
    lbits zgsz3616;
    CREATE(lbits)(&zgsz3616);
    CONVERT_OF(lbits, fbits)(&zgsz3616, UINT64_C(0x0), UINT64_C(4) , true);
    sail_int zgsz3617;
    CREATE(sail_int)(&zgsz3617);
    sail_unsigned(&zgsz3617, zgsz3616);
    zgaz345 = CONVERT_OF(mach_int, sail_int)(zgsz3617);
    KILL(sail_int)(&zgsz3617);
    KILL(lbits)(&zgsz3616);
  }
  {
    sail_int zgsz3613;
    CREATE(sail_int)(&zgsz3613);
    CONVERT_OF(sail_int, mach_int)(&zgsz3613, zgaz345);
    sail_int zgsz3614;
    CREATE(sail_int)(&zgsz3614);
    CONVERT_OF(sail_int, mach_int)(&zgsz3614, INT64_C(0));
    sail_int zgsz3615;
    CREATE(sail_int)(&zgsz3615);
    CONVERT_OF(sail_int, mach_int)(&zgsz3615, zCAP_BOUND_NUM_BITS);
    sailgen_Slice_int(&zgsz321, zgsz3613, zgsz3614, zgsz3615);
    KILL(sail_int)(&zgsz3615);
    KILL(sail_int)(&zgsz3614);
    KILL(sail_int)(&zgsz3613);
  }

  COPY(lbits)(&zCAP_BOUND_MIN, zgsz321);
  KILL(lbits)(&zgsz321);
let_end_72: ;
}
static void kill_letbind_17(void) {    KILL(lbits)(&zCAP_BOUND_MIN);
}

static int64_t zCAP_MAX_ENCODEABLE_EXPONENT;
static void create_letbind_18(void) {


  int64_t zgsz322;
  zgsz322 = INT64_C(63);
  zCAP_MAX_ENCODEABLE_EXPONENT = zgsz322;

let_end_73: ;
}
static void kill_letbind_18(void) {
}

static int64_t zCAP_MAX_EXPONENT;
static void create_letbind_19(void) {


  int64_t zgsz323;
  int64_t zgaz346;
  {
    sail_int zgsz3618;
    CREATE(sail_int)(&zgsz3618);
    CONVERT_OF(sail_int, mach_int)(&zgsz3618, zCAP_VALUE_NUM_BITS);
    sail_int zgsz3619;
    CREATE(sail_int)(&zgsz3619);
    CONVERT_OF(sail_int, mach_int)(&zgsz3619, zCAP_MW);
    sail_int zgsz3620;
    CREATE(sail_int)(&zgsz3620);
    sub_int(&zgsz3620, zgsz3618, zgsz3619);
    zgaz346 = CONVERT_OF(mach_int, sail_int)(zgsz3620);
    KILL(sail_int)(&zgsz3620);
    KILL(sail_int)(&zgsz3619);
    KILL(sail_int)(&zgsz3618);
  }
  zgsz323 = (zgaz346 + INT64_C(2));

  zCAP_MAX_EXPONENT = zgsz323;

let_end_74: ;
}
static void kill_letbind_19(void) {
}

static uint64_t sailgen_CapBoundsAddress(uint64_t);

lbits zghz372;
sail_int zghz373;
lbits zghz374;
sail_int zghz375;
sail_int zghz376;
sail_int zghz377;

static void startup_sailgen_CapBoundsAddress(void)
{
  CREATE(lbits)(&zghz372);
  CREATE(sail_int)(&zghz373);
  CREATE(lbits)(&zghz374);
  CREATE(sail_int)(&zghz375);
  CREATE(sail_int)(&zghz376);
  CREATE(sail_int)(&zghz377);
}

static uint64_t sailgen_CapBoundsAddress(uint64_t zaddress)
{
  __label__ cleanup_77, end_cleanup_78, end_function_76, end_block_exception_79;

  uint64_t zcbz315;
  {
    uint64_t zgaz348;
    {
      int64_t zgaz347;
      {
        RECREATE(sail_int)(&zghz375);
        CONVERT_OF(sail_int, mach_int)(&zghz375, zCAP_FLAGS_LO_BIT);
        RECREATE(sail_int)(&zghz376);
        CONVERT_OF(sail_int, mach_int)(&zghz376, INT64_C(1));
        RECREATE(sail_int)(&zghz377);
        sub_int(&zghz377, zghz375, zghz376);
        zgaz347 = CONVERT_OF(mach_int, sail_int)(zghz377);
      }
      zgaz348 = (safe_rshift(UINT64_MAX, 64 - 56) & (zaddress >> INT64_C(0)));
    }
    {
      RECREATE(lbits)(&zghz372);
      CONVERT_OF(lbits, fbits)(&zghz372, zgaz348, UINT64_C(56) , true);
      RECREATE(sail_int)(&zghz373);
      CONVERT_OF(sail_int, mach_int)(&zghz373, zCAP_VALUE_NUM_BITS);
      RECREATE(lbits)(&zghz374);
      sign_extend(&zghz374, zghz372, zghz373);
      zcbz315 = CONVERT_OF(fbits, lbits)(zghz374, true);
    }
  }
  goto cleanup_77;
  /* unreachable after return */
  goto end_cleanup_78;
cleanup_77: ;
  goto end_function_76;
end_cleanup_78: ;
end_function_76: ;
  return zcbz315;
end_block_exception_79: ;

  return UINT64_C(0xdeadc0de);
}



static void finish_sailgen_CapBoundsAddress(void)
{
  KILL(sail_int)(&zghz377);
  KILL(sail_int)(&zghz376);
  KILL(sail_int)(&zghz375);
  KILL(lbits)(&zghz374);
  KILL(sail_int)(&zghz373);
  KILL(lbits)(&zghz372);
}

static int64_t zCAP_BASE_MANTISSA_LO_BIT;
static void create_letbind_20(void) {


  int64_t zgsz324;
  zgsz324 = INT64_C(67);
  zCAP_BASE_MANTISSA_LO_BIT = zgsz324;

let_end_80: ;
}
static void kill_letbind_20(void) {
}

static uint64_t sailgen_CapGetBottom(lbits);

sail_int zghz378;
sail_int zghz379;
lbits zghz380;
sail_int zghz381;
sail_int zghz382;
lbits zghz383;

static void startup_sailgen_CapGetBottom(void)
{
  CREATE(sail_int)(&zghz378);
  CREATE(sail_int)(&zghz379);
  CREATE(lbits)(&zghz380);
  CREATE(sail_int)(&zghz381);
  CREATE(sail_int)(&zghz382);
  CREATE(lbits)(&zghz383);
}

static uint64_t sailgen_CapGetBottom(lbits zc)
{
  __label__ end_function_82, end_block_exception_87;

  uint64_t zcbz316;
  bool zgaz350;
  zgaz350 = sailgen_CapIsInternalExponent(zc);
  if (zgaz350) {
    __label__ cleanup_85, end_cleanup_86;

    uint64_t zgaz352;
    {
      uint64_t zgaz351;
      {
        RECREATE(sail_int)(&zghz381);
        CONVERT_OF(sail_int, mach_int)(&zghz381, zCAP_BASE_HI_BIT);
        RECREATE(sail_int)(&zghz382);
        CONVERT_OF(sail_int, mach_int)(&zghz382, zCAP_BASE_MANTISSA_LO_BIT);
        RECREATE(lbits)(&zghz383);
        vector_subrange_lbits(&zghz383, zc, zghz381, zghz382);
        zgaz351 = CONVERT_OF(fbits, lbits)(zghz383, true);
      }
      zgaz352 = (zgaz351 << 3) | UINT64_C(0b000);
    }
    zcbz316 = zgaz352;
    goto cleanup_85;
    /* unreachable after return */
    goto end_cleanup_86;
  cleanup_85: ;
    goto end_function_82;
  end_cleanup_86: ;
  } else {
    __label__ cleanup_83, end_cleanup_84;

    uint64_t zgaz353;
    {
      RECREATE(sail_int)(&zghz378);
      CONVERT_OF(sail_int, mach_int)(&zghz378, zCAP_BASE_HI_BIT);
      RECREATE(sail_int)(&zghz379);
      CONVERT_OF(sail_int, mach_int)(&zghz379, zCAP_BASE_LO_BIT);
      RECREATE(lbits)(&zghz380);
      vector_subrange_lbits(&zghz380, zc, zghz378, zghz379);
      zgaz353 = CONVERT_OF(fbits, lbits)(zghz380, true);
    }
    zcbz316 = zgaz353;
    goto cleanup_83;
    /* unreachable after return */
    goto end_cleanup_84;
  cleanup_83: ;
    goto end_function_82;
  end_cleanup_84: ;
  }

end_function_82: ;
  return zcbz316;
end_block_exception_87: ;

  return UINT64_C(0xdeadc0de);
}



static void finish_sailgen_CapGetBottom(void)
{
  KILL(lbits)(&zghz383);
  KILL(sail_int)(&zghz382);
  KILL(sail_int)(&zghz381);
  KILL(lbits)(&zghz380);
  KILL(sail_int)(&zghz379);
  KILL(sail_int)(&zghz378);
}

static int64_t zCAP_LIMIT_HI_BIT;
static void create_letbind_21(void) {


  int64_t zgsz325;
  zgsz325 = INT64_C(93);
  zCAP_LIMIT_HI_BIT = zgsz325;

let_end_88: ;
}
static void kill_letbind_21(void) {
}

static int64_t zCAP_LIMIT_MANTISSA_LO_BIT;
static void create_letbind_22(void) {


  int64_t zgsz326;
  zgsz326 = INT64_C(83);
  zCAP_LIMIT_MANTISSA_LO_BIT = zgsz326;

let_end_89: ;
}
static void kill_letbind_22(void) {
}

static bool sailgen_CapUnsignedLessThan(lbits, lbits);

sail_int zghz384;
sail_int zghz385;

static void startup_sailgen_CapUnsignedLessThan(void)
{
  CREATE(sail_int)(&zghz384);
  CREATE(sail_int)(&zghz385);
}

static bool sailgen_CapUnsignedLessThan(lbits za, lbits zb)
{
  __label__ cleanup_92, end_cleanup_93, end_function_91, end_block_exception_94;

  bool zcbz317;
  {
    RECREATE(sail_int)(&zghz384);
    sail_unsigned(&zghz384, za);
    RECREATE(sail_int)(&zghz385);
    sail_unsigned(&zghz385, zb);
    zcbz317 = lt(zghz384, zghz385);
  }
  goto cleanup_92;
  /* unreachable after return */
  goto end_cleanup_93;
cleanup_92: ;
  goto end_function_91;
end_cleanup_93: ;
end_function_91: ;
  return zcbz317;
end_block_exception_94: ;

  return false;
}



static void finish_sailgen_CapUnsignedLessThan(void)
{
  KILL(sail_int)(&zghz385);
  KILL(sail_int)(&zghz384);
}

static uint64_t sailgen_CapGetTop(lbits);

lbits zghz386;
sail_int zghz387;
sail_int zghz388;
lbits zghz389;
lbits zghz390;
sail_int zghz391;
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
lbits zghz3103;
lbits zghz3104;
sail_int zghz3105;
sail_int zghz3106;
sail_int zghz3107;
sail_int zghz3108;
sail_int zghz3109;
sail_int zghz3110;
sail_int zghz3111;
sail_int zghz3112;
lbits zghz3113;
sail_int zghz3114;
sail_int zghz3115;
lbits zghz3116;
sail_int zghz3117;
lbits zghz3118;
sail_int zghz3119;
sail_int zghz3120;
sail_int zghz3121;

static void startup_sailgen_CapGetTop(void)
{
  CREATE(lbits)(&zghz386);
  CREATE(sail_int)(&zghz387);
  CREATE(sail_int)(&zghz388);
  CREATE(lbits)(&zghz389);
  CREATE(lbits)(&zghz390);
  CREATE(sail_int)(&zghz391);
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
  CREATE(lbits)(&zghz3103);
  CREATE(lbits)(&zghz3104);
  CREATE(sail_int)(&zghz3105);
  CREATE(sail_int)(&zghz3106);
  CREATE(sail_int)(&zghz3107);
  CREATE(sail_int)(&zghz3108);
  CREATE(sail_int)(&zghz3109);
  CREATE(sail_int)(&zghz3110);
  CREATE(sail_int)(&zghz3111);
  CREATE(sail_int)(&zghz3112);
  CREATE(lbits)(&zghz3113);
  CREATE(sail_int)(&zghz3114);
  CREATE(sail_int)(&zghz3115);
  CREATE(lbits)(&zghz3116);
  CREATE(sail_int)(&zghz3117);
  CREATE(lbits)(&zghz3118);
  CREATE(sail_int)(&zghz3119);
  CREATE(sail_int)(&zghz3120);
  CREATE(sail_int)(&zghz3121);
}

static uint64_t sailgen_CapGetTop(lbits zc)
{
  __label__ cleanup_97, end_cleanup_98, end_function_96, end_block_exception_99;

  uint64_t zcbz318;
  uint64_t zlmsb;
  zlmsb = UINT64_C(0b00);
  uint64_t zlcarry;
  zlcarry = UINT64_C(0b00);
  uint64_t zb;
  zb = sailgen_CapGetBottom(zc);
  uint64_t zt;
  {
    int64_t zgaz374;
    {
      int64_t zgaz373;
      {
        RECREATE(sail_int)(&zghz3119);
        CONVERT_OF(sail_int, mach_int)(&zghz3119, INT64_C(79));
        RECREATE(sail_int)(&zghz3120);
        CONVERT_OF(sail_int, mach_int)(&zghz3120, INT64_C(64));
        RECREATE(sail_int)(&zghz3121);
        sub_int(&zghz3121, zghz3119, zghz3120);
        zgaz373 = CONVERT_OF(mach_int, sail_int)(zghz3121);
      }
      zgaz374 = (zgaz373 + INT64_C(1));
    }
    {
      RECREATE(sail_int)(&zghz3117);
      CONVERT_OF(sail_int, mach_int)(&zghz3117, zgaz374);
      RECREATE(lbits)(&zghz3118);
      UNDEFINED(lbits)(&zghz3118, zghz3117);
      zt = CONVERT_OF(fbits, lbits)(zghz3118, true);
    }
  }
  {
    bool zgaz357;
    zgaz357 = sailgen_CapIsInternalExponent(zc);
    unit zgsz329;
    if (zgaz357) {
      {
        zlmsb = UINT64_C(0b01);
        unit zgsz330;
        zgsz330 = UNIT;
      }
      uint64_t zgaz359;
      {
        uint64_t zgaz358;
        {
          RECREATE(sail_int)(&zghz3114);
          CONVERT_OF(sail_int, mach_int)(&zghz3114, zCAP_LIMIT_HI_BIT);
          RECREATE(sail_int)(&zghz3115);
          CONVERT_OF(sail_int, mach_int)(&zghz3115, zCAP_LIMIT_MANTISSA_LO_BIT);
          RECREATE(lbits)(&zghz3116);
          vector_subrange_lbits(&zghz3116, zc, zghz3114, zghz3115);
          zgaz358 = CONVERT_OF(fbits, lbits)(zghz3116, true);
        }
        zgaz359 = (UINT64_C(0b00) << 11) | zgaz358;
      }
      zt = (zgaz359 << 3) | UINT64_C(0b000);
      zgsz329 = UNIT;
    } else {
      uint64_t zgaz360;
      {
        RECREATE(sail_int)(&zghz3111);
        CONVERT_OF(sail_int, mach_int)(&zghz3111, zCAP_LIMIT_HI_BIT);
        RECREATE(sail_int)(&zghz3112);
        CONVERT_OF(sail_int, mach_int)(&zghz3112, zCAP_LIMIT_LO_BIT);
        RECREATE(lbits)(&zghz3113);
        vector_subrange_lbits(&zghz3113, zc, zghz3111, zghz3112);
        zgaz360 = CONVERT_OF(fbits, lbits)(zghz3113, true);
      }
      zt = (UINT64_C(0b00) << 14) | zgaz360;
      zgsz329 = UNIT;
    }
  }
  {
    bool zgaz365;
    {
      uint64_t zgaz363;
      {
        int64_t zgaz361;
        {
          RECREATE(sail_int)(&zghz3108);
          CONVERT_OF(sail_int, mach_int)(&zghz3108, zCAP_MW);
          RECREATE(sail_int)(&zghz3109);
          CONVERT_OF(sail_int, mach_int)(&zghz3109, INT64_C(3));
          RECREATE(sail_int)(&zghz3110);
          sub_int(&zghz3110, zghz3108, zghz3109);
          zgaz361 = CONVERT_OF(mach_int, sail_int)(zghz3110);
        }
        zgaz363 = (safe_rshift(UINT64_MAX, 64 - 14) & (zt >> INT64_C(0)));
      }
      uint64_t zgaz364;
      {
        int64_t zgaz362;
        {
          RECREATE(sail_int)(&zghz3105);
          CONVERT_OF(sail_int, mach_int)(&zghz3105, zCAP_MW);
          RECREATE(sail_int)(&zghz3106);
          CONVERT_OF(sail_int, mach_int)(&zghz3106, INT64_C(3));
          RECREATE(sail_int)(&zghz3107);
          sub_int(&zghz3107, zghz3105, zghz3106);
          zgaz362 = CONVERT_OF(mach_int, sail_int)(zghz3107);
        }
        zgaz364 = (safe_rshift(UINT64_MAX, 64 - 14) & (zb >> INT64_C(0)));
      }
      {
        RECREATE(lbits)(&zghz3103);
        CONVERT_OF(lbits, fbits)(&zghz3103, zgaz363, UINT64_C(14) , true);
        RECREATE(lbits)(&zghz3104);
        CONVERT_OF(lbits, fbits)(&zghz3104, zgaz364, UINT64_C(14) , true);
        zgaz365 = sailgen_CapUnsignedLessThan(zghz3103, zghz3104);
      }
    }
    unit zgsz328;
    if (zgaz365) {
      zlcarry = UINT64_C(0b01);
      zgsz328 = UNIT;
    } else {  zgsz328 = UNIT;  }
  }
  {
    int64_t zgaz370;
    {
      RECREATE(sail_int)(&zghz3100);
      CONVERT_OF(sail_int, mach_int)(&zghz3100, zCAP_MW);
      RECREATE(sail_int)(&zghz3101);
      CONVERT_OF(sail_int, mach_int)(&zghz3101, INT64_C(1));
      RECREATE(sail_int)(&zghz3102);
      sub_int(&zghz3102, zghz3100, zghz3101);
      zgaz370 = CONVERT_OF(mach_int, sail_int)(zghz3102);
    }
    int64_t zgaz371;
    {
      RECREATE(sail_int)(&zghz397);
      CONVERT_OF(sail_int, mach_int)(&zghz397, zCAP_MW);
      RECREATE(sail_int)(&zghz398);
      CONVERT_OF(sail_int, mach_int)(&zghz398, INT64_C(2));
      RECREATE(sail_int)(&zghz399);
      sub_int(&zghz399, zghz397, zghz398);
      zgaz371 = CONVERT_OF(mach_int, sail_int)(zghz399);
    }
    uint64_t zgaz372;
    {
      uint64_t zgaz369;
      {
        uint64_t zgaz368;
        {
          int64_t zgaz366;
          {
            RECREATE(sail_int)(&zghz394);
            CONVERT_OF(sail_int, mach_int)(&zghz394, zCAP_MW);
            RECREATE(sail_int)(&zghz395);
            CONVERT_OF(sail_int, mach_int)(&zghz395, INT64_C(1));
            RECREATE(sail_int)(&zghz396);
            sub_int(&zghz396, zghz394, zghz395);
            zgaz366 = CONVERT_OF(mach_int, sail_int)(zghz396);
          }
          int64_t zgaz367;
          {
            RECREATE(sail_int)(&zghz391);
            CONVERT_OF(sail_int, mach_int)(&zghz391, zCAP_MW);
            RECREATE(sail_int)(&zghz392);
            CONVERT_OF(sail_int, mach_int)(&zghz392, INT64_C(2));
            RECREATE(sail_int)(&zghz393);
            sub_int(&zghz393, zghz391, zghz392);
            zgaz367 = CONVERT_OF(mach_int, sail_int)(zghz393);
          }
          zgaz368 = (safe_rshift(UINT64_MAX, 64 - 2) & (zb >> zgaz367));
        }
        zgaz369 = ((zgaz368 + zlmsb) & UINT64_C(0b11));
      }
      zgaz372 = ((zgaz369 + zlcarry) & UINT64_C(0b11));
    }
    {
      RECREATE(lbits)(&zghz386);
      CONVERT_OF(lbits, fbits)(&zghz386, zt, UINT64_C(16) , true);
      RECREATE(sail_int)(&zghz387);
      CONVERT_OF(sail_int, mach_int)(&zghz387, zgaz370);
      RECREATE(sail_int)(&zghz388);
      CONVERT_OF(sail_int, mach_int)(&zghz388, zgaz371);
      RECREATE(lbits)(&zghz389);
      CONVERT_OF(lbits, fbits)(&zghz389, zgaz372, UINT64_C(2) , true);
      RECREATE(lbits)(&zghz390);
      vector_update_subrange_lbits(&zghz390, zghz386, zghz387, zghz388, zghz389);
      zt = CONVERT_OF(fbits, lbits)(zghz390, true);
    }
    unit zgsz327;
    zgsz327 = UNIT;
  }
  zcbz318 = zt;
  goto cleanup_97;
  /* unreachable after return */




  goto end_cleanup_98;
cleanup_97: ;




  goto end_function_96;
end_cleanup_98: ;
end_function_96: ;
  return zcbz318;
end_block_exception_99: ;

  return UINT64_C(0xdeadc0de);
}



static void finish_sailgen_CapGetTop(void)
{
  KILL(sail_int)(&zghz3121);
  KILL(sail_int)(&zghz3120);
  KILL(sail_int)(&zghz3119);
  KILL(lbits)(&zghz3118);
  KILL(sail_int)(&zghz3117);
  KILL(lbits)(&zghz3116);
  KILL(sail_int)(&zghz3115);
  KILL(sail_int)(&zghz3114);
  KILL(lbits)(&zghz3113);
  KILL(sail_int)(&zghz3112);
  KILL(sail_int)(&zghz3111);
  KILL(sail_int)(&zghz3110);
  KILL(sail_int)(&zghz3109);
  KILL(sail_int)(&zghz3108);
  KILL(sail_int)(&zghz3107);
  KILL(sail_int)(&zghz3106);
  KILL(sail_int)(&zghz3105);
  KILL(lbits)(&zghz3104);
  KILL(lbits)(&zghz3103);
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
  KILL(sail_int)(&zghz391);
  KILL(lbits)(&zghz390);
  KILL(lbits)(&zghz389);
  KILL(sail_int)(&zghz388);
  KILL(sail_int)(&zghz387);
  KILL(lbits)(&zghz386);
}

static bool sailgen_CapIsExponentOutOfRange(lbits);

static bool sailgen_CapIsExponentOutOfRange(lbits zc)
{
  __label__ case_102, finish_match_101, end_function_103, end_block_exception_106;

  bool zcbz319;
  int64_t zgaz375;
  zgaz375 = sailgen_CapGetExponent(zc);
  {
    __label__ cleanup_104, end_cleanup_105;

    int64_t zexp;
    zexp = zgaz375;
    {
      bool zgaz376;
      zgaz376 = (zexp > zCAP_MAX_EXPONENT);
      bool zgsz332;
      if (zgaz376) {  zgsz332 = (zexp < zCAP_MAX_ENCODEABLE_EXPONENT);  } else {  zgsz332 = false;  }
      zcbz319 = zgsz332;
    }
    goto cleanup_104;
    /* unreachable after return */
    goto finish_match_101;
    goto end_cleanup_105;
  cleanup_104: ;
    goto end_function_103;
  end_cleanup_105: ;
  }
case_102: ;
  sail_match_failure("CapIsExponentOutOfRange");
finish_match_101: ;

end_function_103: ;
  return zcbz319;
end_block_exception_106: ;

  return false;
}

static bool sailgen_CapUnsignedGreaterThan(lbits, lbits);

sail_int zghz3122;
sail_int zghz3123;

static void startup_sailgen_CapUnsignedGreaterThan(void)
{
  CREATE(sail_int)(&zghz3122);
  CREATE(sail_int)(&zghz3123);
}

static bool sailgen_CapUnsignedGreaterThan(lbits za, lbits zb)
{
  __label__ cleanup_109, end_cleanup_110, end_function_108, end_block_exception_111;

  bool zcbz320;
  {
    RECREATE(sail_int)(&zghz3122);
    sail_unsigned(&zghz3122, za);
    RECREATE(sail_int)(&zghz3123);
    sail_unsigned(&zghz3123, zb);
    zcbz320 = gt(zghz3122, zghz3123);
  }
  goto cleanup_109;
  /* unreachable after return */
  goto end_cleanup_110;
cleanup_109: ;
  goto end_function_108;
end_cleanup_110: ;
end_function_108: ;
  return zcbz320;
end_block_exception_111: ;

  return false;
}



static void finish_sailgen_CapUnsignedGreaterThan(void)
{
  KILL(sail_int)(&zghz3123);
  KILL(sail_int)(&zghz3122);
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

lbits zghz3124;
lbits zghz3125;
lbits zghz3126;
lbits zghz3127;
lbits zghz3128;
sail_int zghz3129;
sail_int zghz3130;
lbits zghz3131;
lbits zghz3132;
sail_int zghz3133;
sail_int zghz3134;
lbits zghz3135;
sail_int zghz3136;
sail_int zghz3137;
lbits zghz3138;
lbits zghz3139;
sail_int zghz3140;
sail_int zghz3141;
sail_int zghz3142;
sail_int zghz3143;
sail_int zghz3144;
sail_int zghz3145;
lbits zghz3146;
sail_int zghz3147;
sail_int zghz3148;
lbits zghz3149;
lbits zghz3150;
sail_int zghz3151;
lbits zghz3152;
sail_int zghz3153;
sail_int zghz3154;
lbits zghz3155;
lbits zghz3156;
sail_int zghz3157;
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
lbits zghz3169;
lbits zghz3170;
lbits zghz3171;
lbits zghz3172;
lbits zghz3173;
sail_int zghz3174;
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
lbits zghz3188;
sail_int zghz3189;
sail_int zghz3190;
sail_int zghz3191;
sail_int zghz3192;
sail_int zghz3193;
sail_int zghz3194;
lbits zghz3195;
lbits zghz3196;
sail_int zghz3197;
sail_int zghz3198;
lbits zghz3199;
sail_int zghz3200;
sail_int zghz3201;
sail_int zghz3202;
sail_int zghz3203;
sail_int zghz3204;
lbits zghz3205;
sail_int zghz3206;
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
lbits zghz3217;
sail_int zghz3218;
lbits zghz3219;
sail_int zghz3220;
sail_int zghz3221;
sail_int zghz3222;
lbits zghz3223;
sail_int zghz3224;
lbits zghz3225;
sail_int zghz3226;
sail_int zghz3227;

static void startup_sailgen_CapGetBounds(void)
{
  CREATE(lbits)(&zghz3124);
  CREATE(lbits)(&zghz3125);
  CREATE(lbits)(&zghz3126);
  CREATE(lbits)(&zghz3127);
  CREATE(lbits)(&zghz3128);
  CREATE(sail_int)(&zghz3129);
  CREATE(sail_int)(&zghz3130);
  CREATE(lbits)(&zghz3131);
  CREATE(lbits)(&zghz3132);
  CREATE(sail_int)(&zghz3133);
  CREATE(sail_int)(&zghz3134);
  CREATE(lbits)(&zghz3135);
  CREATE(sail_int)(&zghz3136);
  CREATE(sail_int)(&zghz3137);
  CREATE(lbits)(&zghz3138);
  CREATE(lbits)(&zghz3139);
  CREATE(sail_int)(&zghz3140);
  CREATE(sail_int)(&zghz3141);
  CREATE(sail_int)(&zghz3142);
  CREATE(sail_int)(&zghz3143);
  CREATE(sail_int)(&zghz3144);
  CREATE(sail_int)(&zghz3145);
  CREATE(lbits)(&zghz3146);
  CREATE(sail_int)(&zghz3147);
  CREATE(sail_int)(&zghz3148);
  CREATE(lbits)(&zghz3149);
  CREATE(lbits)(&zghz3150);
  CREATE(sail_int)(&zghz3151);
  CREATE(lbits)(&zghz3152);
  CREATE(sail_int)(&zghz3153);
  CREATE(sail_int)(&zghz3154);
  CREATE(lbits)(&zghz3155);
  CREATE(lbits)(&zghz3156);
  CREATE(sail_int)(&zghz3157);
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
  CREATE(lbits)(&zghz3169);
  CREATE(lbits)(&zghz3170);
  CREATE(lbits)(&zghz3171);
  CREATE(lbits)(&zghz3172);
  CREATE(lbits)(&zghz3173);
  CREATE(sail_int)(&zghz3174);
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
  CREATE(lbits)(&zghz3188);
  CREATE(sail_int)(&zghz3189);
  CREATE(sail_int)(&zghz3190);
  CREATE(sail_int)(&zghz3191);
  CREATE(sail_int)(&zghz3192);
  CREATE(sail_int)(&zghz3193);
  CREATE(sail_int)(&zghz3194);
  CREATE(lbits)(&zghz3195);
  CREATE(lbits)(&zghz3196);
  CREATE(sail_int)(&zghz3197);
  CREATE(sail_int)(&zghz3198);
  CREATE(lbits)(&zghz3199);
  CREATE(sail_int)(&zghz3200);
  CREATE(sail_int)(&zghz3201);
  CREATE(sail_int)(&zghz3202);
  CREATE(sail_int)(&zghz3203);
  CREATE(sail_int)(&zghz3204);
  CREATE(lbits)(&zghz3205);
  CREATE(sail_int)(&zghz3206);
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
  CREATE(lbits)(&zghz3217);
  CREATE(sail_int)(&zghz3218);
  CREATE(lbits)(&zghz3219);
  CREATE(sail_int)(&zghz3220);
  CREATE(sail_int)(&zghz3221);
  CREATE(sail_int)(&zghz3222);
  CREATE(lbits)(&zghz3223);
  CREATE(sail_int)(&zghz3224);
  CREATE(lbits)(&zghz3225);
  CREATE(sail_int)(&zghz3226);
  CREATE(sail_int)(&zghz3227);
}

static void sailgen_CapGetBounds(struct ztuple_z8z5bvzCz0z5bvzCz0z5boolz9 *zcbz321, lbits zc)
{
  __label__ case_114, finish_match_113, end_function_125, end_block_exception_132, end_function_236;

  int64_t zgaz381;
  zgaz381 = sailgen_CapGetExponent(zc);
  struct ztuple_z8z5bvzCz0z5bvzCz0z5boolz9 zgsz334;
  CREATE(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgsz334);
  {
    __label__ case_116, finish_match_115;

    int64_t zexp;
    zexp = zgaz381;
    {
      bool zgaz382;
      zgaz382 = (zexp == zCAP_MAX_ENCODEABLE_EXPONENT);
      unit zgsz337;
      if (zgaz382) {
        __label__ cleanup_130, end_cleanup_131;

        struct ztuple_z8z5bvzCz0z5bvzCz0z5boolz9 zgsz338;
        CREATE(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgsz338);
        COPY(lbits)(&((&zgsz338)->ztup0), zCAP_BOUND_MIN);
        COPY(lbits)(&((&zgsz338)->ztup1), zCAP_BOUND_MAX);
        zgsz338.ztup2 = true;
        COPY(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)((*(&zcbz321)), zgsz338);
        goto cleanup_130;
        /* unreachable after return */
        goto end_cleanup_131;
      cleanup_130: ;
        KILL(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgsz334);
        KILL(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgsz338);
        goto end_function_125;
      end_cleanup_131: ;
      } else {  zgsz337 = UNIT;  }
    }
    {
      bool zgaz383;
      zgaz383 = sailgen_CapIsExponentOutOfRange(zc);
      unit zgsz335;
      if (zgaz383) {
        __label__ cleanup_128, end_cleanup_129;

        struct ztuple_z8z5bvzCz0z5bvzCz0z5boolz9 zgsz336;
        CREATE(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgsz336);
        COPY(lbits)(&((&zgsz336)->ztup0), zCAP_BOUND_MIN);
        COPY(lbits)(&((&zgsz336)->ztup1), zCAP_BOUND_MAX);
        zgsz336.ztup2 = false;
        COPY(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)((*(&zcbz321)), zgsz336);
        goto cleanup_128;
        /* unreachable after return */
        goto end_cleanup_129;
      cleanup_128: ;
        KILL(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgsz334);
        KILL(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgsz336);
        goto end_function_125;
      end_cleanup_129: ;
      } else {  zgsz335 = UNIT;  }
    }
    RECREATE(lbits)(&zghz3124);
    {
      RECREATE(sail_int)(&zghz3227);
      CONVERT_OF(sail_int, mach_int)(&zghz3227, INT64_C(66));
      UNDEFINED(lbits)(&zghz3124, zghz3227);
    }
    RECREATE(lbits)(&zghz3125);
    {
      RECREATE(sail_int)(&zghz3226);
      CONVERT_OF(sail_int, mach_int)(&zghz3226, INT64_C(66));
      UNDEFINED(lbits)(&zghz3125, zghz3226);
    }
    uint64_t zbottom;
    zbottom = sailgen_CapGetBottom(zc);
    uint64_t ztop;
    ztop = sailgen_CapGetTop(zc);
    {
      sbits zgaz384;
      {
        RECREATE(sail_int)(&zghz3224);
        CONVERT_OF(sail_int, mach_int)(&zghz3224, zexp);
        RECREATE(lbits)(&zghz3225);
        sailgen_Zeros(&zghz3225, zghz3224);
        zgaz384 = CONVERT_OF(sbits, lbits)(zghz3225, true);
      }
      {
        RECREATE(sail_int)(&zghz3220);
        CONVERT_OF(sail_int, mach_int)(&zghz3220, INT64_C(66));
        RECREATE(sail_int)(&zghz3221);
        CONVERT_OF(sail_int, mach_int)(&zghz3221, zexp);
        RECREATE(sail_int)(&zghz3222);
        CONVERT_OF(sail_int, mach_int)(&zghz3222, INT64_C(0));
        RECREATE(lbits)(&zghz3223);
        CONVERT_OF(lbits, sbits)(&zghz3223, zgaz384, true);
        set_slice(&zghz3124, zghz3220, zghz3221, zghz3124, zghz3222, zghz3223);
      }
      unit zgsz343;
      zgsz343 = UNIT;
    }
    {
      sbits zgaz385;
      {
        RECREATE(sail_int)(&zghz3218);
        CONVERT_OF(sail_int, mach_int)(&zghz3218, zexp);
        RECREATE(lbits)(&zghz3219);
        sailgen_Zeros(&zghz3219, zghz3218);
        zgaz385 = CONVERT_OF(sbits, lbits)(zghz3219, true);
      }
      {
        RECREATE(sail_int)(&zghz3214);
        CONVERT_OF(sail_int, mach_int)(&zghz3214, INT64_C(66));
        RECREATE(sail_int)(&zghz3215);
        CONVERT_OF(sail_int, mach_int)(&zghz3215, zexp);
        RECREATE(sail_int)(&zghz3216);
        CONVERT_OF(sail_int, mach_int)(&zghz3216, INT64_C(0));
        RECREATE(lbits)(&zghz3217);
        CONVERT_OF(lbits, sbits)(&zghz3217, zgaz385, true);
        set_slice(&zghz3125, zghz3214, zghz3215, zghz3125, zghz3216, zghz3217);
      }
      unit zgsz342;
      zgsz342 = UNIT;
    }
    {
      bool zgaz389;
      {
        int64_t zgaz388;
        {
          int64_t zgaz387;
          {
            int64_t zgaz386;
            {
              RECREATE(sail_int)(&zghz3212);
              CONVERT_OF(sail_int, mach_int)(&zghz3212, zexp);
              RECREATE(sail_int)(&zghz3213);
              sailgen___id(&zghz3213, zghz3212);
              zgaz386 = CONVERT_OF(mach_int, sail_int)(zghz3213);
            }
            zgaz387 = (zgaz386 + INT64_C(16));
          }
          {
            RECREATE(sail_int)(&zghz3209);
            CONVERT_OF(sail_int, mach_int)(&zghz3209, zgaz387);
            RECREATE(sail_int)(&zghz3210);
            CONVERT_OF(sail_int, mach_int)(&zghz3210, INT64_C(1));
            RECREATE(sail_int)(&zghz3211);
            sub_int(&zghz3211, zghz3209, zghz3210);
            zgaz388 = CONVERT_OF(mach_int, sail_int)(zghz3211);
          }
        }
        zgaz389 = (zgaz388 < INT64_C(66));
      }
      unit zgsz341;
      zgsz341 = sail_assert(zgaz389, "v8_base.sail:1391.41-1391.42");
    }
    {
      int64_t zgaz391;
      {
        int64_t zgaz390;
        zgaz390 = (zexp + zCAP_MW);
        {
          RECREATE(sail_int)(&zghz3206);
          CONVERT_OF(sail_int, mach_int)(&zghz3206, zgaz390);
          RECREATE(sail_int)(&zghz3207);
          CONVERT_OF(sail_int, mach_int)(&zghz3207, INT64_C(1));
          RECREATE(sail_int)(&zghz3208);
          sub_int(&zghz3208, zghz3206, zghz3207);
          zgaz391 = CONVERT_OF(mach_int, sail_int)(zghz3208);
        }
      }
      {
        RECREATE(sail_int)(&zghz3203);
        CONVERT_OF(sail_int, mach_int)(&zghz3203, zgaz391);
        RECREATE(sail_int)(&zghz3204);
        CONVERT_OF(sail_int, mach_int)(&zghz3204, zexp);
        RECREATE(lbits)(&zghz3205);
        CONVERT_OF(lbits, fbits)(&zghz3205, zbottom, UINT64_C(16) , true);
        vector_update_subrange_lbits(&zghz3124, zghz3124, zghz3203, zghz3204, zghz3205);
      }
      unit zgsz340;
      zgsz340 = UNIT;
    }
    {
      int64_t zgaz393;
      {
        int64_t zgaz392;
        zgaz392 = (zexp + zCAP_MW);
        {
          RECREATE(sail_int)(&zghz3200);
          CONVERT_OF(sail_int, mach_int)(&zghz3200, zgaz392);
          RECREATE(sail_int)(&zghz3201);
          CONVERT_OF(sail_int, mach_int)(&zghz3201, INT64_C(1));
          RECREATE(sail_int)(&zghz3202);
          sub_int(&zghz3202, zghz3200, zghz3201);
          zgaz393 = CONVERT_OF(mach_int, sail_int)(zghz3202);
        }
      }
      {
        RECREATE(sail_int)(&zghz3197);
        CONVERT_OF(sail_int, mach_int)(&zghz3197, zgaz393);
        RECREATE(sail_int)(&zghz3198);
        CONVERT_OF(sail_int, mach_int)(&zghz3198, zexp);
        RECREATE(lbits)(&zghz3199);
        CONVERT_OF(lbits, fbits)(&zghz3199, ztop, UINT64_C(16) , true);
        vector_update_subrange_lbits(&zghz3125, zghz3125, zghz3197, zghz3198, zghz3199);
      }
      unit zgsz339;
      zgsz339 = UNIT;
    }
    RECREATE(lbits)(&zghz3126);
    {
      uint64_t zgaz3130;
      {
        uint64_t zgaz3129;
        zgaz3129 = sailgen_CapGetValue(zc);
        zgaz3130 = sailgen_CapBoundsAddress(zgaz3129);
      }
      {
        RECREATE(lbits)(&zghz3195);
        CONVERT_OF(lbits, fbits)(&zghz3195, UINT64_C(0b00), UINT64_C(2) , true);
        RECREATE(lbits)(&zghz3196);
        CONVERT_OF(lbits, fbits)(&zghz3196, zgaz3130, UINT64_C(64) , true);
        append(&zghz3126, zghz3195, zghz3196);
      }
    }
    uint64_t zA3;
    {
      int64_t zgaz3127;
      {
        int64_t zgaz3125;
        zgaz3125 = (zexp + zCAP_MW);
        {
          RECREATE(sail_int)(&zghz3192);
          CONVERT_OF(sail_int, mach_int)(&zghz3192, zgaz3125);
          RECREATE(sail_int)(&zghz3193);
          CONVERT_OF(sail_int, mach_int)(&zghz3193, INT64_C(1));
          RECREATE(sail_int)(&zghz3194);
          sub_int(&zghz3194, zghz3192, zghz3193);
          zgaz3127 = CONVERT_OF(mach_int, sail_int)(zghz3194);
        }
      }
      int64_t zgaz3128;
      {
        int64_t zgaz3126;
        zgaz3126 = (zexp + zCAP_MW);
        {
          RECREATE(sail_int)(&zghz3189);
          CONVERT_OF(sail_int, mach_int)(&zghz3189, zgaz3126);
          RECREATE(sail_int)(&zghz3190);
          CONVERT_OF(sail_int, mach_int)(&zghz3190, INT64_C(3));
          RECREATE(sail_int)(&zghz3191);
          sub_int(&zghz3191, zghz3189, zghz3190);
          zgaz3128 = CONVERT_OF(mach_int, sail_int)(zghz3191);
        }
      }
      {
        RECREATE(sail_int)(&zghz3186);
        CONVERT_OF(sail_int, mach_int)(&zghz3186, zgaz3127);
        RECREATE(sail_int)(&zghz3187);
        CONVERT_OF(sail_int, mach_int)(&zghz3187, zgaz3128);
        RECREATE(lbits)(&zghz3188);
        vector_subrange_lbits(&zghz3188, zghz3126, zghz3186, zghz3187);
        zA3 = CONVERT_OF(fbits, lbits)(zghz3188, true);
      }
    }
    uint64_t zB3;
    {
      int64_t zgaz3123;
      {
        RECREATE(sail_int)(&zghz3183);
        CONVERT_OF(sail_int, mach_int)(&zghz3183, zCAP_MW);
        RECREATE(sail_int)(&zghz3184);
        CONVERT_OF(sail_int, mach_int)(&zghz3184, INT64_C(1));
        RECREATE(sail_int)(&zghz3185);
        sub_int(&zghz3185, zghz3183, zghz3184);
        zgaz3123 = CONVERT_OF(mach_int, sail_int)(zghz3185);
      }
      int64_t zgaz3124;
      {
        RECREATE(sail_int)(&zghz3180);
        CONVERT_OF(sail_int, mach_int)(&zghz3180, zCAP_MW);
        RECREATE(sail_int)(&zghz3181);
        CONVERT_OF(sail_int, mach_int)(&zghz3181, INT64_C(3));
        RECREATE(sail_int)(&zghz3182);
        sub_int(&zghz3182, zghz3180, zghz3181);
        zgaz3124 = CONVERT_OF(mach_int, sail_int)(zghz3182);
      }
      zB3 = (safe_rshift(UINT64_MAX, 64 - 3) & (zbottom >> zgaz3124));
    }
    uint64_t zT3;
    {
      int64_t zgaz3121;
      {
        RECREATE(sail_int)(&zghz3177);
        CONVERT_OF(sail_int, mach_int)(&zghz3177, zCAP_MW);
        RECREATE(sail_int)(&zghz3178);
        CONVERT_OF(sail_int, mach_int)(&zghz3178, INT64_C(1));
        RECREATE(sail_int)(&zghz3179);
        sub_int(&zghz3179, zghz3177, zghz3178);
        zgaz3121 = CONVERT_OF(mach_int, sail_int)(zghz3179);
      }
      int64_t zgaz3122;
      {
        RECREATE(sail_int)(&zghz3174);
        CONVERT_OF(sail_int, mach_int)(&zghz3174, zCAP_MW);
        RECREATE(sail_int)(&zghz3175);
        CONVERT_OF(sail_int, mach_int)(&zghz3175, INT64_C(3));
        RECREATE(sail_int)(&zghz3176);
        sub_int(&zghz3176, zghz3174, zghz3175);
        zgaz3122 = CONVERT_OF(mach_int, sail_int)(zghz3176);
      }
      zT3 = (safe_rshift(UINT64_MAX, 64 - 3) & (ztop >> zgaz3122));
    }
    uint64_t zR3;
    zR3 = ((zB3 - UINT64_C(0b001)) & UINT64_C(0b111));
    int64_t zaHi;
    zaHi = INT64_C(0);
    {
      bool zgaz394;
      {
        RECREATE(lbits)(&zghz3172);
        CONVERT_OF(lbits, fbits)(&zghz3172, zA3, UINT64_C(3) , true);
        RECREATE(lbits)(&zghz3173);
        CONVERT_OF(lbits, fbits)(&zghz3173, zR3, UINT64_C(3) , true);
        zgaz394 = sailgen_CapUnsignedLessThan(zghz3172, zghz3173);
      }
      unit zgsz344;
      if (zgaz394) {
        zaHi = INT64_C(1);
        zgsz344 = UNIT;
      } else {
        zaHi = INT64_C(0);
        zgsz344 = UNIT;
      }
    }
    {
      __label__ case_118, finish_match_117;

      int64_t zaHishadowz30;
      zaHishadowz30 = zaHi;
      int64_t zbHi;
      zbHi = INT64_C(0);
      {
        bool zgaz395;
        {
          RECREATE(lbits)(&zghz3170);
          CONVERT_OF(lbits, fbits)(&zghz3170, zB3, UINT64_C(3) , true);
          RECREATE(lbits)(&zghz3171);
          CONVERT_OF(lbits, fbits)(&zghz3171, zR3, UINT64_C(3) , true);
          zgaz395 = sailgen_CapUnsignedLessThan(zghz3170, zghz3171);
        }
        unit zgsz346;
        if (zgaz395) {
          zbHi = INT64_C(1);
          zgsz346 = UNIT;
        } else {
          zbHi = INT64_C(0);
          zgsz346 = UNIT;
        }
      }
      {
        __label__ case_120, finish_match_119;

        int64_t zbHishadowz31;
        zbHishadowz31 = zbHi;
        int64_t ztHi;
        ztHi = INT64_C(0);
        {
          bool zgaz396;
          {
            RECREATE(lbits)(&zghz3168);
            CONVERT_OF(lbits, fbits)(&zghz3168, zT3, UINT64_C(3) , true);
            RECREATE(lbits)(&zghz3169);
            CONVERT_OF(lbits, fbits)(&zghz3169, zR3, UINT64_C(3) , true);
            zgaz396 = sailgen_CapUnsignedLessThan(zghz3168, zghz3169);
          }
          unit zgsz348;
          if (zgaz396) {
            ztHi = INT64_C(1);
            zgsz348 = UNIT;
          } else {
            ztHi = INT64_C(0);
            zgsz348 = UNIT;
          }
        }
        {
          __label__ case_122, finish_match_121;

          int64_t ztHishadowz32;
          ztHishadowz32 = ztHi;
          int64_t zgaz397;
          {
            RECREATE(sail_int)(&zghz3165);
            CONVERT_OF(sail_int, mach_int)(&zghz3165, zbHishadowz31);
            RECREATE(sail_int)(&zghz3166);
            CONVERT_OF(sail_int, mach_int)(&zghz3166, zaHishadowz30);
            RECREATE(sail_int)(&zghz3167);
            sub_int(&zghz3167, zghz3165, zghz3166);
            zgaz397 = CONVERT_OF(mach_int, sail_int)(zghz3167);
          }
          {
            __label__ case_124, finish_match_123;

            int64_t zcorrection_base;
            zcorrection_base = zgaz397;
            int64_t zgaz398;
            {
              RECREATE(sail_int)(&zghz3162);
              CONVERT_OF(sail_int, mach_int)(&zghz3162, ztHishadowz32);
              RECREATE(sail_int)(&zghz3163);
              CONVERT_OF(sail_int, mach_int)(&zghz3163, zaHishadowz30);
              RECREATE(sail_int)(&zghz3164);
              sub_int(&zghz3164, zghz3162, zghz3163);
              zgaz398 = CONVERT_OF(mach_int, sail_int)(zghz3164);
            }
            {
              __label__ cleanup_126, end_cleanup_127;

              int64_t zcorrection_limit;
              zcorrection_limit = zgaz398;
              {
                bool zgaz3101;
                {
                  int64_t zgaz399;
                  zgaz399 = (zexp + zCAP_MW);
                  int64_t zgaz3100;
                  zgaz3100 = (zCAP_MAX_EXPONENT + zCAP_MW);
                  zgaz3101 = (zgaz399 < zgaz3100);
                }
                unit zgsz352;
                if (zgaz3101) {
                  sbits zatop;
                  {
                    int64_t zgaz3106;
                    zgaz3106 = (zexp + zCAP_MW);
                    {
                      RECREATE(sail_int)(&zghz3159);
                      CONVERT_OF(sail_int, mach_int)(&zghz3159, INT64_C(65));
                      RECREATE(sail_int)(&zghz3160);
                      CONVERT_OF(sail_int, mach_int)(&zghz3160, zgaz3106);
                      RECREATE(lbits)(&zghz3161);
                      vector_subrange_lbits(&zghz3161, zghz3126, zghz3159, zghz3160);
                      zatop = CONVERT_OF(sbits, lbits)(zghz3161, true);
                    }
                  }
                  {
                    int64_t zgaz3102;
                    zgaz3102 = (zexp + zCAP_MW);
                    sbits zgaz3103;
                    {
                      RECREATE(lbits)(&zghz3156);
                      CONVERT_OF(lbits, sbits)(&zghz3156, zatop, true);
                      RECREATE(sail_int)(&zghz3157);
                      CONVERT_OF(sail_int, mach_int)(&zghz3157, zcorrection_base);
                      RECREATE(lbits)(&zghz3158);
                      add_bits_int(&zghz3158, zghz3156, zghz3157);
                      zgaz3103 = CONVERT_OF(sbits, lbits)(zghz3158, true);
                    }
                    {
                      RECREATE(sail_int)(&zghz3153);
                      CONVERT_OF(sail_int, mach_int)(&zghz3153, INT64_C(65));
                      RECREATE(sail_int)(&zghz3154);
                      CONVERT_OF(sail_int, mach_int)(&zghz3154, zgaz3102);
                      RECREATE(lbits)(&zghz3155);
                      CONVERT_OF(lbits, sbits)(&zghz3155, zgaz3103, true);
                      vector_update_subrange_lbits(&zghz3124, zghz3124, zghz3153, zghz3154, zghz3155);
                    }
                    unit zgsz353;
                    zgsz353 = UNIT;
                  }
                  int64_t zgaz3104;
                  zgaz3104 = (zexp + zCAP_MW);
                  sbits zgaz3105;
                  {
                    RECREATE(lbits)(&zghz3150);
                    CONVERT_OF(lbits, sbits)(&zghz3150, zatop, true);
                    RECREATE(sail_int)(&zghz3151);
                    CONVERT_OF(sail_int, mach_int)(&zghz3151, zcorrection_limit);
                    RECREATE(lbits)(&zghz3152);
                    add_bits_int(&zghz3152, zghz3150, zghz3151);
                    zgaz3105 = CONVERT_OF(sbits, lbits)(zghz3152, true);
                  }
                  {
                    RECREATE(sail_int)(&zghz3147);
                    CONVERT_OF(sail_int, mach_int)(&zghz3147, INT64_C(65));
                    RECREATE(sail_int)(&zghz3148);
                    CONVERT_OF(sail_int, mach_int)(&zghz3148, zgaz3104);
                    RECREATE(lbits)(&zghz3149);
                    CONVERT_OF(lbits, sbits)(&zghz3149, zgaz3105, true);
                    vector_update_subrange_lbits(&zghz3125, zghz3125, zghz3147, zghz3148, zghz3149);
                  }
                  zgsz352 = UNIT;
                } else {  zgsz352 = UNIT;  }
              }
              uint64_t zl2;
              {
                RECREATE(sail_int)(&zghz3144);
                CONVERT_OF(sail_int, mach_int)(&zghz3144, INT64_C(64));
                RECREATE(sail_int)(&zghz3145);
                CONVERT_OF(sail_int, mach_int)(&zghz3145, INT64_C(63));
                RECREATE(lbits)(&zghz3146);
                vector_subrange_lbits(&zghz3146, zghz3125, zghz3144, zghz3145);
                zl2 = CONVERT_OF(fbits, lbits)(zghz3146, true);
              }
              uint64_t zb2;
              {
                uint64_t zgaz3120;
                {
                  fbits zgaz3119;
                  {
                    RECREATE(sail_int)(&zghz3143);
                    CONVERT_OF(sail_int, mach_int)(&zghz3143, INT64_C(63));
                    zgaz3119 = bitvector_access(zghz3124, zghz3143);
                  }
                  zgaz3120 = UINT64_C(0b0);
                  zgaz3120 = update_fbits(zgaz3120, INT64_C(0), zgaz3119);
                }
                zb2 = (UINT64_C(0b0) << 1) | zgaz3120;
              }
              {
                bool zgaz3110;
                {
                  bool zgaz3109;
                  {
                    int64_t zgaz3107;
                    {
                      RECREATE(sail_int)(&zghz3140);
                      CONVERT_OF(sail_int, mach_int)(&zghz3140, zCAP_MAX_EXPONENT);
                      RECREATE(sail_int)(&zghz3141);
                      CONVERT_OF(sail_int, mach_int)(&zghz3141, INT64_C(1));
                      RECREATE(sail_int)(&zghz3142);
                      sub_int(&zghz3142, zghz3140, zghz3141);
                      zgaz3107 = CONVERT_OF(mach_int, sail_int)(zghz3142);
                    }
                    zgaz3109 = (zexp < zgaz3107);
                  }
                  bool zgsz355;
                  if (zgaz3109) {
                    uint64_t zgaz3108;
                    zgaz3108 = ((zl2 - zb2) & UINT64_C(0b11));
                    {
                      RECREATE(lbits)(&zghz3138);
                      CONVERT_OF(lbits, fbits)(&zghz3138, zgaz3108, UINT64_C(2) , true);
                      RECREATE(lbits)(&zghz3139);
                      CONVERT_OF(lbits, fbits)(&zghz3139, UINT64_C(0b01), UINT64_C(2) , true);
                      zgsz355 = sailgen_CapUnsignedGreaterThan(zghz3138, zghz3139);
                    }
                  } else {  zgsz355 = false;  }
                  zgaz3110 = zgsz355;
                }
                unit zgsz356;
                if (zgaz3110) {
                  fbits zgaz3114;
                  {
                    uint64_t zgaz3113;
                    {
                      uint64_t zgaz3112;
                      {
                        fbits zgaz3111;
                        {
                          RECREATE(sail_int)(&zghz3137);
                          CONVERT_OF(sail_int, mach_int)(&zghz3137, INT64_C(64));
                          zgaz3111 = bitvector_access(zghz3125, zghz3137);
                        }
                        zgaz3112 = UINT64_C(0b0);
                        zgaz3112 = update_fbits(zgaz3112, INT64_C(0), zgaz3111);
                      }
                      zgaz3113 = (~(zgaz3112) & UINT64_C(0b1));
                    }
                    zgaz3114 = sailgen_Bit(zgaz3113);
                  }
                  {
                    RECREATE(sail_int)(&zghz3136);
                    CONVERT_OF(sail_int, mach_int)(&zghz3136, INT64_C(64));
                    update_lbits(&zghz3125, zghz3125, zghz3136, zgaz3114);
                  }
                  zgsz356 = UNIT;
                } else {  zgsz356 = UNIT;  }
              }
              struct ztuple_z8z5bvzCz0z5bvzCz0z5boolz9 zgaz3118;
              CREATE(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgaz3118);
              {
                RECREATE(lbits)(&zghz3127);
                {
                  uint64_t zgaz3115;
                  {
                    RECREATE(sail_int)(&zghz3133);
                    CONVERT_OF(sail_int, mach_int)(&zghz3133, INT64_C(63));
                    RECREATE(sail_int)(&zghz3134);
                    CONVERT_OF(sail_int, mach_int)(&zghz3134, INT64_C(0));
                    RECREATE(lbits)(&zghz3135);
                    vector_subrange_lbits(&zghz3135, zghz3124, zghz3133, zghz3134);
                    zgaz3115 = CONVERT_OF(fbits, lbits)(zghz3135, true);
                  }
                  {
                    RECREATE(lbits)(&zghz3131);
                    CONVERT_OF(lbits, fbits)(&zghz3131, UINT64_C(0b0), UINT64_C(1) , true);
                    RECREATE(lbits)(&zghz3132);
                    CONVERT_OF(lbits, fbits)(&zghz3132, zgaz3115, UINT64_C(64) , true);
                    append(&zghz3127, zghz3131, zghz3132);
                  }
                }
                RECREATE(lbits)(&zghz3128);
                {
                  RECREATE(sail_int)(&zghz3129);
                  CONVERT_OF(sail_int, mach_int)(&zghz3129, INT64_C(64));
                  RECREATE(sail_int)(&zghz3130);
                  CONVERT_OF(sail_int, mach_int)(&zghz3130, INT64_C(0));
                  vector_subrange_lbits(&zghz3128, zghz3125, zghz3129, zghz3130);
                }
                COPY(lbits)(&((&zgaz3118)->ztup0), zghz3127);
                COPY(lbits)(&((&zgaz3118)->ztup1), zghz3128);
                zgaz3118.ztup2 = true;
              }
              COPY(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)((*(&zcbz321)), zgaz3118);
              goto cleanup_126;
              /* unreachable after return */
              KILL(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgaz3118);
              goto finish_match_123;
              goto end_cleanup_127;
            cleanup_126: ;
              KILL(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgsz334);
              KILL(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgaz3118);
              goto end_function_125;
            end_cleanup_127: ;
            }
          case_124: ;
            sail_match_failure("CapGetBounds");
          finish_match_123: ;
            goto finish_match_121;
          }
        case_122: ;
          sail_match_failure("CapGetBounds");
        finish_match_121: ;
          goto finish_match_119;
        }
      case_120: ;
        sail_match_failure("CapGetBounds");
      finish_match_119: ;
        goto finish_match_117;
      }
    case_118: ;
      sail_match_failure("CapGetBounds");
    finish_match_117: ;
      goto finish_match_115;
    }
  case_116: ;
    sail_match_failure("CapGetBounds");
  finish_match_115: ;
    goto finish_match_113;
  }
case_114: ;
  sail_match_failure("CapGetBounds");
finish_match_113: ;
  COPY(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)((*(&zcbz321)), zgsz334);
  KILL(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgsz334);

end_function_125: ;
  goto end_function_236;
end_block_exception_132: ;
  goto end_function_236;
end_function_236: ;
}



static void finish_sailgen_CapGetBounds(void)
{
  KILL(sail_int)(&zghz3227);
  KILL(sail_int)(&zghz3226);
  KILL(lbits)(&zghz3225);
  KILL(sail_int)(&zghz3224);
  KILL(lbits)(&zghz3223);
  KILL(sail_int)(&zghz3222);
  KILL(sail_int)(&zghz3221);
  KILL(sail_int)(&zghz3220);
  KILL(lbits)(&zghz3219);
  KILL(sail_int)(&zghz3218);
  KILL(lbits)(&zghz3217);
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
  KILL(sail_int)(&zghz3206);
  KILL(lbits)(&zghz3205);
  KILL(sail_int)(&zghz3204);
  KILL(sail_int)(&zghz3203);
  KILL(sail_int)(&zghz3202);
  KILL(sail_int)(&zghz3201);
  KILL(sail_int)(&zghz3200);
  KILL(lbits)(&zghz3199);
  KILL(sail_int)(&zghz3198);
  KILL(sail_int)(&zghz3197);
  KILL(lbits)(&zghz3196);
  KILL(lbits)(&zghz3195);
  KILL(sail_int)(&zghz3194);
  KILL(sail_int)(&zghz3193);
  KILL(sail_int)(&zghz3192);
  KILL(sail_int)(&zghz3191);
  KILL(sail_int)(&zghz3190);
  KILL(sail_int)(&zghz3189);
  KILL(lbits)(&zghz3188);
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
  KILL(sail_int)(&zghz3174);
  KILL(lbits)(&zghz3173);
  KILL(lbits)(&zghz3172);
  KILL(lbits)(&zghz3171);
  KILL(lbits)(&zghz3170);
  KILL(lbits)(&zghz3169);
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
  KILL(sail_int)(&zghz3157);
  KILL(lbits)(&zghz3156);
  KILL(lbits)(&zghz3155);
  KILL(sail_int)(&zghz3154);
  KILL(sail_int)(&zghz3153);
  KILL(lbits)(&zghz3152);
  KILL(sail_int)(&zghz3151);
  KILL(lbits)(&zghz3150);
  KILL(lbits)(&zghz3149);
  KILL(sail_int)(&zghz3148);
  KILL(sail_int)(&zghz3147);
  KILL(lbits)(&zghz3146);
  KILL(sail_int)(&zghz3145);
  KILL(sail_int)(&zghz3144);
  KILL(sail_int)(&zghz3143);
  KILL(sail_int)(&zghz3142);
  KILL(sail_int)(&zghz3141);
  KILL(sail_int)(&zghz3140);
  KILL(lbits)(&zghz3139);
  KILL(lbits)(&zghz3138);
  KILL(sail_int)(&zghz3137);
  KILL(sail_int)(&zghz3136);
  KILL(lbits)(&zghz3135);
  KILL(sail_int)(&zghz3134);
  KILL(sail_int)(&zghz3133);
  KILL(lbits)(&zghz3132);
  KILL(lbits)(&zghz3131);
  KILL(sail_int)(&zghz3130);
  KILL(sail_int)(&zghz3129);
  KILL(lbits)(&zghz3128);
  KILL(lbits)(&zghz3127);
  KILL(lbits)(&zghz3126);
  KILL(lbits)(&zghz3125);
  KILL(lbits)(&zghz3124);
}

static int64_t zCAP_TAG_BIT;
static void create_letbind_23(void) {


  int64_t zgsz365;
  zgsz365 = INT64_C(128);
  zCAP_TAG_BIT = zgsz365;

let_end_133: ;
}
static void kill_letbind_23(void) {
}

// register _PC
static uint64_t z_PC;

// register __BranchTaken
static bool z__BranchTaken;

static int64_t zCAP_OTYPE_HI_BIT;
static void create_letbind_24(void) {


  int64_t zgsz366;
  zgsz366 = INT64_C(109);
  zCAP_OTYPE_HI_BIT = zgsz366;

let_end_134: ;
}
static void kill_letbind_24(void) {
}

static int64_t zCAP_OTYPE_LO_BIT;
static void create_letbind_25(void) {


  int64_t zgsz367;
  zgsz367 = INT64_C(95);
  zCAP_OTYPE_LO_BIT = zgsz367;

let_end_135: ;
}
static void kill_letbind_25(void) {
}

static uint64_t sailgen_CapGetObjectType(lbits);

lbits zghz3228;
sail_int zghz3229;
lbits zghz3230;
sail_int zghz3231;
sail_int zghz3232;
lbits zghz3233;

static void startup_sailgen_CapGetObjectType(void)
{
  CREATE(lbits)(&zghz3228);
  CREATE(sail_int)(&zghz3229);
  CREATE(lbits)(&zghz3230);
  CREATE(sail_int)(&zghz3231);
  CREATE(sail_int)(&zghz3232);
  CREATE(lbits)(&zghz3233);
}

static uint64_t sailgen_CapGetObjectType(lbits zc)
{
  __label__ cleanup_138, end_cleanup_139, end_function_137, end_block_exception_140;

  uint64_t zcbz322;
  {
    uint64_t zgaz3131;
    {
      RECREATE(sail_int)(&zghz3231);
      CONVERT_OF(sail_int, mach_int)(&zghz3231, zCAP_OTYPE_HI_BIT);
      RECREATE(sail_int)(&zghz3232);
      CONVERT_OF(sail_int, mach_int)(&zghz3232, zCAP_OTYPE_LO_BIT);
      RECREATE(lbits)(&zghz3233);
      vector_subrange_lbits(&zghz3233, zc, zghz3231, zghz3232);
      zgaz3131 = CONVERT_OF(fbits, lbits)(zghz3233, true);
    }
    {
      RECREATE(lbits)(&zghz3228);
      CONVERT_OF(lbits, fbits)(&zghz3228, zgaz3131, UINT64_C(15) , true);
      RECREATE(sail_int)(&zghz3229);
      CONVERT_OF(sail_int, mach_int)(&zghz3229, zCAP_VALUE_NUM_BITS);
      RECREATE(lbits)(&zghz3230);
      zero_extend(&zghz3230, zghz3228, zghz3229);
      zcbz322 = CONVERT_OF(fbits, lbits)(zghz3230, true);
    }
  }
  goto cleanup_138;
  /* unreachable after return */
  goto end_cleanup_139;
cleanup_138: ;
  goto end_function_137;
end_cleanup_139: ;
end_function_137: ;
  return zcbz322;
end_block_exception_140: ;

  return UINT64_C(0xdeadc0de);
}



static void finish_sailgen_CapGetObjectType(void)
{
  KILL(lbits)(&zghz3233);
  KILL(sail_int)(&zghz3232);
  KILL(sail_int)(&zghz3231);
  KILL(lbits)(&zghz3230);
  KILL(sail_int)(&zghz3229);
  KILL(lbits)(&zghz3228);
}

static int64_t zCAP_FLAGS_HI_BIT;
static void create_letbind_26(void) {


  int64_t zgsz368;
  zgsz368 = INT64_C(63);
  zCAP_FLAGS_HI_BIT = zgsz368;

let_end_141: ;
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


  uint64_t zgsz369;
  zgsz369 = UINT64_C(0b10110);
  zM32_Monitor = zgsz369;

let_end_142: ;
}
static void kill_letbind_27(void) {
}

static uint64_t zCAP_PERM_EXECUTIVE;
static void create_letbind_28(void) {


  uint64_t zgsz370;
  int64_t zgaz3133;
  zgaz3133 = shl_mach_int(INT64_C(1), INT64_C(1));
  {
    sail_int zgsz3774;
    CREATE(sail_int)(&zgsz3774);
    CONVERT_OF(sail_int, mach_int)(&zgsz3774, zgaz3133);
    sail_int zgsz3775;
    CREATE(sail_int)(&zgsz3775);
    CONVERT_OF(sail_int, mach_int)(&zgsz3775, INT64_C(63));
    sail_int zgsz3776;
    CREATE(sail_int)(&zgsz3776);
    CONVERT_OF(sail_int, mach_int)(&zgsz3776, INT64_C(0));
    lbits zgsz3777;
    CREATE(lbits)(&zgsz3777);
    sailgen_integer_subrange(&zgsz3777, zgsz3774, zgsz3775, zgsz3776);
    zgsz370 = CONVERT_OF(fbits, lbits)(zgsz3777, true);
    KILL(lbits)(&zgsz3777);
    KILL(sail_int)(&zgsz3776);
    KILL(sail_int)(&zgsz3775);
    KILL(sail_int)(&zgsz3774);
  }

  zCAP_PERM_EXECUTIVE = zgsz370;

let_end_143: ;
}
static void kill_letbind_28(void) {
}

static int64_t zCAP_PERMS_HI_BIT;
static void create_letbind_29(void) {


  int64_t zgsz371;
  zgsz371 = INT64_C(127);
  zCAP_PERMS_HI_BIT = zgsz371;

let_end_144: ;
}
static void kill_letbind_29(void) {
}

static int64_t zCAP_PERMS_LO_BIT;
static void create_letbind_30(void) {


  int64_t zgsz372;
  zgsz372 = INT64_C(110);
  zCAP_PERMS_LO_BIT = zgsz372;

let_end_145: ;
}
static void kill_letbind_30(void) {
}

static int64_t zCAP_PERMS_NUM_BITS;
static void create_letbind_31(void) {


  int64_t zgsz373;
  int64_t zgaz3134;
  {
    sail_int zgsz3778;
    CREATE(sail_int)(&zgsz3778);
    CONVERT_OF(sail_int, mach_int)(&zgsz3778, zCAP_PERMS_HI_BIT);
    sail_int zgsz3779;
    CREATE(sail_int)(&zgsz3779);
    CONVERT_OF(sail_int, mach_int)(&zgsz3779, zCAP_PERMS_LO_BIT);
    sail_int zgsz3780;
    CREATE(sail_int)(&zgsz3780);
    sub_int(&zgsz3780, zgsz3778, zgsz3779);
    zgaz3134 = CONVERT_OF(mach_int, sail_int)(zgsz3780);
    KILL(sail_int)(&zgsz3780);
    KILL(sail_int)(&zgsz3779);
    KILL(sail_int)(&zgsz3778);
  }
  zgsz373 = (zgaz3134 + INT64_C(1));

  zCAP_PERMS_NUM_BITS = zgsz373;

let_end_146: ;
}
static void kill_letbind_31(void) {
}

static uint64_t sailgen_CapGetPermissions(lbits);

sail_int zghz3234;
sail_int zghz3235;
lbits zghz3236;

static void startup_sailgen_CapGetPermissions(void)
{
  CREATE(sail_int)(&zghz3234);
  CREATE(sail_int)(&zghz3235);
  CREATE(lbits)(&zghz3236);
}

static uint64_t sailgen_CapGetPermissions(lbits zc)
{
  __label__ cleanup_149, end_cleanup_150, end_function_148, end_block_exception_151;

  uint64_t zcbz323;
  {
    RECREATE(sail_int)(&zghz3234);
    CONVERT_OF(sail_int, mach_int)(&zghz3234, zCAP_PERMS_HI_BIT);
    RECREATE(sail_int)(&zghz3235);
    CONVERT_OF(sail_int, mach_int)(&zghz3235, zCAP_PERMS_LO_BIT);
    RECREATE(lbits)(&zghz3236);
    vector_subrange_lbits(&zghz3236, zc, zghz3234, zghz3235);
    zcbz323 = CONVERT_OF(fbits, lbits)(zghz3236, true);
  }
  goto cleanup_149;
  /* unreachable after return */
  goto end_cleanup_150;
cleanup_149: ;
  goto end_function_148;
end_cleanup_150: ;
end_function_148: ;
  return zcbz323;
end_block_exception_151: ;

  return UINT64_C(0xdeadc0de);
}



static void finish_sailgen_CapGetPermissions(void)
{
  KILL(lbits)(&zghz3236);
  KILL(sail_int)(&zghz3235);
  KILL(sail_int)(&zghz3234);
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


  uint64_t zgsz374;
  sail_int zgaz3136;
  CREATE(sail_int)(&zgaz3136);
  {
    sail_int zgsz3787;
    CREATE(sail_int)(&zgsz3787);
    CONVERT_OF(sail_int, mach_int)(&zgsz3787, INT64_C(1));
    sail_int zgsz3788;
    CREATE(sail_int)(&zgsz3788);
    CONVERT_OF(sail_int, mach_int)(&zgsz3788, INT64_C(15));
    shl_int(&zgaz3136, zgsz3787, zgsz3788);
    KILL(sail_int)(&zgsz3788);
    KILL(sail_int)(&zgsz3787);
  }
  {
    sail_int zgsz3784;
    CREATE(sail_int)(&zgsz3784);
    CONVERT_OF(sail_int, mach_int)(&zgsz3784, INT64_C(63));
    sail_int zgsz3785;
    CREATE(sail_int)(&zgsz3785);
    CONVERT_OF(sail_int, mach_int)(&zgsz3785, INT64_C(0));
    lbits zgsz3786;
    CREATE(lbits)(&zgsz3786);
    sailgen_integer_subrange(&zgsz3786, zgaz3136, zgsz3784, zgsz3785);
    zgsz374 = CONVERT_OF(fbits, lbits)(zgsz3786, true);
    KILL(lbits)(&zgsz3786);
    KILL(sail_int)(&zgsz3785);
    KILL(sail_int)(&zgsz3784);
  }
  KILL(sail_int)(&zgaz3136);
  zCAP_PERM_EXECUTE = zgsz374;

let_end_152: ;
}
static void kill_letbind_32(void) {
}

static uint64_t zCAP_PERM_SYSTEM;
static void create_letbind_33(void) {


  uint64_t zgsz375;
  sail_int zgaz3137;
  CREATE(sail_int)(&zgaz3137);
  {
    sail_int zgsz3792;
    CREATE(sail_int)(&zgsz3792);
    CONVERT_OF(sail_int, mach_int)(&zgsz3792, INT64_C(1));
    sail_int zgsz3793;
    CREATE(sail_int)(&zgsz3793);
    CONVERT_OF(sail_int, mach_int)(&zgsz3793, INT64_C(9));
    shl_int(&zgaz3137, zgsz3792, zgsz3793);
    KILL(sail_int)(&zgsz3793);
    KILL(sail_int)(&zgsz3792);
  }
  {
    sail_int zgsz3789;
    CREATE(sail_int)(&zgsz3789);
    CONVERT_OF(sail_int, mach_int)(&zgsz3789, INT64_C(63));
    sail_int zgsz3790;
    CREATE(sail_int)(&zgsz3790);
    CONVERT_OF(sail_int, mach_int)(&zgsz3790, INT64_C(0));
    lbits zgsz3791;
    CREATE(lbits)(&zgsz3791);
    sailgen_integer_subrange(&zgsz3791, zgaz3137, zgsz3789, zgsz3790);
    zgsz375 = CONVERT_OF(fbits, lbits)(zgsz3791, true);
    KILL(lbits)(&zgsz3791);
    KILL(sail_int)(&zgsz3790);
    KILL(sail_int)(&zgsz3789);
  }
  KILL(sail_int)(&zgaz3137);
  zCAP_PERM_SYSTEM = zgsz375;

let_end_153: ;
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


  uint64_t zgsz376;
  zgsz376 = UINT64_C(0b110011);
  zDebugHalt_SoftwareAccess = zgsz376;

let_end_154: ;
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


  uint64_t zgsz377;
  zgsz377 = UINT64_C(0b00);
  zMemAttr_NC = zgsz377;

let_end_155: ;
}
static void kill_letbind_35(void) {
}

static uint64_t zMemAttr_WT;
static void create_letbind_36(void) {


  uint64_t zgsz378;
  zgsz378 = UINT64_C(0b10);
  zMemAttr_WT = zgsz378;

let_end_156: ;
}
static void kill_letbind_36(void) {
}

static uint64_t zMemAttr_WB;
static void create_letbind_37(void) {


  uint64_t zgsz379;
  zgsz379 = UINT64_C(0b11);
  zMemAttr_WB = zgsz379;

let_end_157: ;
}
static void kill_letbind_37(void) {
}

static uint64_t zMemHint_No;
static void create_letbind_38(void) {


  uint64_t zgsz380;
  zgsz380 = UINT64_C(0b00);
  zMemHint_No = zgsz380;

let_end_158: ;
}
static void kill_letbind_38(void) {
}

static uint64_t zMemHint_RA;
static void create_letbind_39(void) {


  uint64_t zgsz381;
  zgsz381 = UINT64_C(0b10);
  zMemHint_RA = zgsz381;

let_end_159: ;
}
static void kill_letbind_39(void) {
}

static uint64_t zMemHint_RWA;
static void create_letbind_40(void) {


  uint64_t zgsz382;
  zgsz382 = UINT64_C(0b11);
  zMemHint_RWA = zgsz382;

let_end_160: ;
}
static void kill_letbind_40(void) {
}

// register EventRegister
static uint64_t zEventRegister;

// register _V
static zz5vecz8z5bvz9 z_V;

static uint64_t zDefaultPARTID;
static void create_letbind_41(void) {


  uint64_t zgsz383;
  {
    sail_int zgsz3794;
    CREATE(sail_int)(&zgsz3794);
    CONVERT_OF(sail_int, mach_int)(&zgsz3794, INT64_C(0));
    sail_int zgsz3795;
    CREATE(sail_int)(&zgsz3795);
    CONVERT_OF(sail_int, mach_int)(&zgsz3795, INT64_C(15));
    sail_int zgsz3796;
    CREATE(sail_int)(&zgsz3796);
    CONVERT_OF(sail_int, mach_int)(&zgsz3796, INT64_C(0));
    lbits zgsz3797;
    CREATE(lbits)(&zgsz3797);
    sailgen_integer_subrange(&zgsz3797, zgsz3794, zgsz3795, zgsz3796);
    zgsz383 = CONVERT_OF(fbits, lbits)(zgsz3797, true);
    KILL(lbits)(&zgsz3797);
    KILL(sail_int)(&zgsz3796);
    KILL(sail_int)(&zgsz3795);
    KILL(sail_int)(&zgsz3794);
  }
  zDefaultPARTID = zgsz383;

let_end_161: ;
}
static void kill_letbind_41(void) {
}

static uint64_t zDefaultPMG;
static void create_letbind_42(void) {


  uint64_t zgsz384;
  {
    sail_int zgsz3798;
    CREATE(sail_int)(&zgsz3798);
    CONVERT_OF(sail_int, mach_int)(&zgsz3798, INT64_C(0));
    sail_int zgsz3799;
    CREATE(sail_int)(&zgsz3799);
    CONVERT_OF(sail_int, mach_int)(&zgsz3799, INT64_C(7));
    sail_int zgsz3800;
    CREATE(sail_int)(&zgsz3800);
    CONVERT_OF(sail_int, mach_int)(&zgsz3800, INT64_C(0));
    lbits zgsz3801;
    CREATE(lbits)(&zgsz3801);
    sailgen_integer_subrange(&zgsz3801, zgsz3798, zgsz3799, zgsz3800);
    zgsz384 = CONVERT_OF(fbits, lbits)(zgsz3801, true);
    KILL(lbits)(&zgsz3801);
    KILL(sail_int)(&zgsz3800);
    KILL(sail_int)(&zgsz3799);
    KILL(sail_int)(&zgsz3798);
  }
  zDefaultPMG = zgsz384;

let_end_162: ;
}
static void kill_letbind_42(void) {
}

static int64_t zCAPABILITY_DBYTES;
static void create_letbind_43(void) {


  int64_t zgsz385;
  zgsz385 = INT64_C(16);
  zCAPABILITY_DBYTES = zgsz385;

let_end_163: ;
}
static void kill_letbind_43(void) {
}

static int64_t zLOG2_CAPABILITY_DBYTES;
static void create_letbind_44(void) {


  int64_t zgsz386;
  zgsz386 = INT64_C(4);
  zLOG2_CAPABILITY_DBYTES = zgsz386;

let_end_164: ;
}
static void kill_letbind_44(void) {
}

static uint64_t zDebugHalt_Breakpoint;
static void create_letbind_45(void) {


  uint64_t zgsz387;
  zgsz387 = UINT64_C(0b000111);
  zDebugHalt_Breakpoint = zgsz387;

let_end_165: ;
}
static void kill_letbind_45(void) {
}

static uint64_t zDebugHalt_Watchpoint;
static void create_letbind_46(void) {


  uint64_t zgsz388;
  zgsz388 = UINT64_C(0b101011);
  zDebugHalt_Watchpoint = zgsz388;

let_end_166: ;
}
static void kill_letbind_46(void) {
}

static uint64_t zDebugHalt_HaltInstruction;
static void create_letbind_47(void) {


  uint64_t zgsz389;
  zgsz389 = UINT64_C(0b101111);
  zDebugHalt_HaltInstruction = zgsz389;

let_end_167: ;
}
static void kill_letbind_47(void) {
}

static uint64_t zCAP_NO_SEALING;
static void create_letbind_48(void) {


  uint64_t zgsz390;
  {
    sail_int zgsz3802;
    CREATE(sail_int)(&zgsz3802);
    CONVERT_OF(sail_int, mach_int)(&zgsz3802, INT64_C(64));
    lbits zgsz3803;
    CREATE(lbits)(&zgsz3803);
    sailgen_Ones(&zgsz3803, zgsz3802);
    zgsz390 = CONVERT_OF(fbits, lbits)(zgsz3803, true);
    KILL(lbits)(&zgsz3803);
    KILL(sail_int)(&zgsz3802);
  }
  zCAP_NO_SEALING = zgsz390;

let_end_168: ;
}
static void kill_letbind_48(void) {
}

static uint64_t zCAP_SEAL_TYPE_RB;
static void create_letbind_49(void) {


  uint64_t zgsz391;
  {
    lbits zgsz3804;
    CREATE(lbits)(&zgsz3804);
    CONVERT_OF(lbits, fbits)(&zgsz3804, UINT64_C(0b01), UINT64_C(2) , true);
    sail_int zgsz3805;
    CREATE(sail_int)(&zgsz3805);
    CONVERT_OF(sail_int, mach_int)(&zgsz3805, INT64_C(64));
    lbits zgsz3806;
    CREATE(lbits)(&zgsz3806);
    zero_extend(&zgsz3806, zgsz3804, zgsz3805);
    zgsz391 = CONVERT_OF(fbits, lbits)(zgsz3806, true);
    KILL(lbits)(&zgsz3806);
    KILL(sail_int)(&zgsz3805);
    KILL(lbits)(&zgsz3804);
  }
  zCAP_SEAL_TYPE_RB = zgsz391;

let_end_169: ;
}
static void kill_letbind_49(void) {
}

static uint64_t zCAP_SEAL_TYPE_LPB;
static void create_letbind_50(void) {


  uint64_t zgsz392;
  {
    lbits zgsz3807;
    CREATE(lbits)(&zgsz3807);
    CONVERT_OF(lbits, fbits)(&zgsz3807, UINT64_C(0b10), UINT64_C(2) , true);
    sail_int zgsz3808;
    CREATE(sail_int)(&zgsz3808);
    CONVERT_OF(sail_int, mach_int)(&zgsz3808, INT64_C(64));
    lbits zgsz3809;
    CREATE(lbits)(&zgsz3809);
    zero_extend(&zgsz3809, zgsz3807, zgsz3808);
    zgsz392 = CONVERT_OF(fbits, lbits)(zgsz3809, true);
    KILL(lbits)(&zgsz3809);
    KILL(sail_int)(&zgsz3808);
    KILL(lbits)(&zgsz3807);
  }
  zCAP_SEAL_TYPE_LPB = zgsz392;

let_end_170: ;
}
static void kill_letbind_50(void) {
}

static uint64_t zCAP_SEAL_TYPE_LB;
static void create_letbind_51(void) {


  uint64_t zgsz393;
  {
    lbits zgsz3810;
    CREATE(lbits)(&zgsz3810);
    CONVERT_OF(lbits, fbits)(&zgsz3810, UINT64_C(0b11), UINT64_C(2) , true);
    sail_int zgsz3811;
    CREATE(sail_int)(&zgsz3811);
    CONVERT_OF(sail_int, mach_int)(&zgsz3811, INT64_C(64));
    lbits zgsz3812;
    CREATE(lbits)(&zgsz3812);
    zero_extend(&zgsz3812, zgsz3810, zgsz3811);
    zgsz393 = CONVERT_OF(fbits, lbits)(zgsz3812, true);
    KILL(lbits)(&zgsz3812);
    KILL(sail_int)(&zgsz3811);
    KILL(lbits)(&zgsz3810);
  }
  zCAP_SEAL_TYPE_LB = zgsz393;

let_end_171: ;
}
static void kill_letbind_51(void) {
}

static int64_t zCAP_MAX_FIXED_SEAL_TYPE;
static void create_letbind_52(void) {


  int64_t zgsz394;
  zgsz394 = INT64_C(3);
  zCAP_MAX_FIXED_SEAL_TYPE = zgsz394;

let_end_172: ;
}
static void kill_letbind_52(void) {
}

static uint64_t zCAP_PERM_LOAD;
static void create_letbind_53(void) {


  uint64_t zgsz395;
  sail_int zgaz3138;
  CREATE(sail_int)(&zgaz3138);
  {
    sail_int zgsz3816;
    CREATE(sail_int)(&zgsz3816);
    CONVERT_OF(sail_int, mach_int)(&zgsz3816, INT64_C(1));
    sail_int zgsz3817;
    CREATE(sail_int)(&zgsz3817);
    CONVERT_OF(sail_int, mach_int)(&zgsz3817, INT64_C(17));
    shl_int(&zgaz3138, zgsz3816, zgsz3817);
    KILL(sail_int)(&zgsz3817);
    KILL(sail_int)(&zgsz3816);
  }
  {
    sail_int zgsz3813;
    CREATE(sail_int)(&zgsz3813);
    CONVERT_OF(sail_int, mach_int)(&zgsz3813, INT64_C(63));
    sail_int zgsz3814;
    CREATE(sail_int)(&zgsz3814);
    CONVERT_OF(sail_int, mach_int)(&zgsz3814, INT64_C(0));
    lbits zgsz3815;
    CREATE(lbits)(&zgsz3815);
    sailgen_integer_subrange(&zgsz3815, zgaz3138, zgsz3813, zgsz3814);
    zgsz395 = CONVERT_OF(fbits, lbits)(zgsz3815, true);
    KILL(lbits)(&zgsz3815);
    KILL(sail_int)(&zgsz3814);
    KILL(sail_int)(&zgsz3813);
  }
  KILL(sail_int)(&zgaz3138);
  zCAP_PERM_LOAD = zgsz395;

let_end_173: ;
}
static void kill_letbind_53(void) {
}

static uint64_t zCAP_PERM_STORE;
static void create_letbind_54(void) {


  uint64_t zgsz396;
  sail_int zgaz3139;
  CREATE(sail_int)(&zgaz3139);
  {
    sail_int zgsz3821;
    CREATE(sail_int)(&zgsz3821);
    CONVERT_OF(sail_int, mach_int)(&zgsz3821, INT64_C(1));
    sail_int zgsz3822;
    CREATE(sail_int)(&zgsz3822);
    CONVERT_OF(sail_int, mach_int)(&zgsz3822, INT64_C(16));
    shl_int(&zgaz3139, zgsz3821, zgsz3822);
    KILL(sail_int)(&zgsz3822);
    KILL(sail_int)(&zgsz3821);
  }
  {
    sail_int zgsz3818;
    CREATE(sail_int)(&zgsz3818);
    CONVERT_OF(sail_int, mach_int)(&zgsz3818, INT64_C(63));
    sail_int zgsz3819;
    CREATE(sail_int)(&zgsz3819);
    CONVERT_OF(sail_int, mach_int)(&zgsz3819, INT64_C(0));
    lbits zgsz3820;
    CREATE(lbits)(&zgsz3820);
    sailgen_integer_subrange(&zgsz3820, zgaz3139, zgsz3818, zgsz3819);
    zgsz396 = CONVERT_OF(fbits, lbits)(zgsz3820, true);
    KILL(lbits)(&zgsz3820);
    KILL(sail_int)(&zgsz3819);
    KILL(sail_int)(&zgsz3818);
  }
  KILL(sail_int)(&zgaz3139);
  zCAP_PERM_STORE = zgsz396;

let_end_174: ;
}
static void kill_letbind_54(void) {
}

static uint64_t zCAP_PERM_LOAD_CAP;
static void create_letbind_55(void) {


  uint64_t zgsz397;
  sail_int zgaz3140;
  CREATE(sail_int)(&zgaz3140);
  {
    sail_int zgsz3826;
    CREATE(sail_int)(&zgsz3826);
    CONVERT_OF(sail_int, mach_int)(&zgsz3826, INT64_C(1));
    sail_int zgsz3827;
    CREATE(sail_int)(&zgsz3827);
    CONVERT_OF(sail_int, mach_int)(&zgsz3827, INT64_C(14));
    shl_int(&zgaz3140, zgsz3826, zgsz3827);
    KILL(sail_int)(&zgsz3827);
    KILL(sail_int)(&zgsz3826);
  }
  {
    sail_int zgsz3823;
    CREATE(sail_int)(&zgsz3823);
    CONVERT_OF(sail_int, mach_int)(&zgsz3823, INT64_C(63));
    sail_int zgsz3824;
    CREATE(sail_int)(&zgsz3824);
    CONVERT_OF(sail_int, mach_int)(&zgsz3824, INT64_C(0));
    lbits zgsz3825;
    CREATE(lbits)(&zgsz3825);
    sailgen_integer_subrange(&zgsz3825, zgaz3140, zgsz3823, zgsz3824);
    zgsz397 = CONVERT_OF(fbits, lbits)(zgsz3825, true);
    KILL(lbits)(&zgsz3825);
    KILL(sail_int)(&zgsz3824);
    KILL(sail_int)(&zgsz3823);
  }
  KILL(sail_int)(&zgaz3140);
  zCAP_PERM_LOAD_CAP = zgsz397;

let_end_175: ;
}
static void kill_letbind_55(void) {
}

static uint64_t zCAP_PERM_STORE_CAP;
static void create_letbind_56(void) {


  uint64_t zgsz398;
  sail_int zgaz3141;
  CREATE(sail_int)(&zgaz3141);
  {
    sail_int zgsz3831;
    CREATE(sail_int)(&zgsz3831);
    CONVERT_OF(sail_int, mach_int)(&zgsz3831, INT64_C(1));
    sail_int zgsz3832;
    CREATE(sail_int)(&zgsz3832);
    CONVERT_OF(sail_int, mach_int)(&zgsz3832, INT64_C(13));
    shl_int(&zgaz3141, zgsz3831, zgsz3832);
    KILL(sail_int)(&zgsz3832);
    KILL(sail_int)(&zgsz3831);
  }
  {
    sail_int zgsz3828;
    CREATE(sail_int)(&zgsz3828);
    CONVERT_OF(sail_int, mach_int)(&zgsz3828, INT64_C(63));
    sail_int zgsz3829;
    CREATE(sail_int)(&zgsz3829);
    CONVERT_OF(sail_int, mach_int)(&zgsz3829, INT64_C(0));
    lbits zgsz3830;
    CREATE(lbits)(&zgsz3830);
    sailgen_integer_subrange(&zgsz3830, zgaz3141, zgsz3828, zgsz3829);
    zgsz398 = CONVERT_OF(fbits, lbits)(zgsz3830, true);
    KILL(lbits)(&zgsz3830);
    KILL(sail_int)(&zgsz3829);
    KILL(sail_int)(&zgsz3828);
  }
  KILL(sail_int)(&zgaz3141);
  zCAP_PERM_STORE_CAP = zgsz398;

let_end_176: ;
}
static void kill_letbind_56(void) {
}

static uint64_t zCAP_PERM_STORE_LOCAL;
static void create_letbind_57(void) {


  uint64_t zgsz399;
  sail_int zgaz3142;
  CREATE(sail_int)(&zgaz3142);
  {
    sail_int zgsz3836;
    CREATE(sail_int)(&zgsz3836);
    CONVERT_OF(sail_int, mach_int)(&zgsz3836, INT64_C(1));
    sail_int zgsz3837;
    CREATE(sail_int)(&zgsz3837);
    CONVERT_OF(sail_int, mach_int)(&zgsz3837, INT64_C(12));
    shl_int(&zgaz3142, zgsz3836, zgsz3837);
    KILL(sail_int)(&zgsz3837);
    KILL(sail_int)(&zgsz3836);
  }
  {
    sail_int zgsz3833;
    CREATE(sail_int)(&zgsz3833);
    CONVERT_OF(sail_int, mach_int)(&zgsz3833, INT64_C(63));
    sail_int zgsz3834;
    CREATE(sail_int)(&zgsz3834);
    CONVERT_OF(sail_int, mach_int)(&zgsz3834, INT64_C(0));
    lbits zgsz3835;
    CREATE(lbits)(&zgsz3835);
    sailgen_integer_subrange(&zgsz3835, zgaz3142, zgsz3833, zgsz3834);
    zgsz399 = CONVERT_OF(fbits, lbits)(zgsz3835, true);
    KILL(lbits)(&zgsz3835);
    KILL(sail_int)(&zgsz3834);
    KILL(sail_int)(&zgsz3833);
  }
  KILL(sail_int)(&zgaz3142);
  zCAP_PERM_STORE_LOCAL = zgsz399;

let_end_177: ;
}
static void kill_letbind_57(void) {
}

static uint64_t zCAP_PERM_SEAL;
static void create_letbind_58(void) {


  uint64_t zgsz3100;
  sail_int zgaz3143;
  CREATE(sail_int)(&zgaz3143);
  {
    sail_int zgsz3841;
    CREATE(sail_int)(&zgsz3841);
    CONVERT_OF(sail_int, mach_int)(&zgsz3841, INT64_C(1));
    sail_int zgsz3842;
    CREATE(sail_int)(&zgsz3842);
    CONVERT_OF(sail_int, mach_int)(&zgsz3842, INT64_C(11));
    shl_int(&zgaz3143, zgsz3841, zgsz3842);
    KILL(sail_int)(&zgsz3842);
    KILL(sail_int)(&zgsz3841);
  }
  {
    sail_int zgsz3838;
    CREATE(sail_int)(&zgsz3838);
    CONVERT_OF(sail_int, mach_int)(&zgsz3838, INT64_C(63));
    sail_int zgsz3839;
    CREATE(sail_int)(&zgsz3839);
    CONVERT_OF(sail_int, mach_int)(&zgsz3839, INT64_C(0));
    lbits zgsz3840;
    CREATE(lbits)(&zgsz3840);
    sailgen_integer_subrange(&zgsz3840, zgaz3143, zgsz3838, zgsz3839);
    zgsz3100 = CONVERT_OF(fbits, lbits)(zgsz3840, true);
    KILL(lbits)(&zgsz3840);
    KILL(sail_int)(&zgsz3839);
    KILL(sail_int)(&zgsz3838);
  }
  KILL(sail_int)(&zgaz3143);
  zCAP_PERM_SEAL = zgsz3100;

let_end_178: ;
}
static void kill_letbind_58(void) {
}

static uint64_t zCAP_PERM_UNSEAL;
static void create_letbind_59(void) {


  uint64_t zgsz3101;
  sail_int zgaz3144;
  CREATE(sail_int)(&zgaz3144);
  {
    sail_int zgsz3846;
    CREATE(sail_int)(&zgsz3846);
    CONVERT_OF(sail_int, mach_int)(&zgsz3846, INT64_C(1));
    sail_int zgsz3847;
    CREATE(sail_int)(&zgsz3847);
    CONVERT_OF(sail_int, mach_int)(&zgsz3847, INT64_C(10));
    shl_int(&zgaz3144, zgsz3846, zgsz3847);
    KILL(sail_int)(&zgsz3847);
    KILL(sail_int)(&zgsz3846);
  }
  {
    sail_int zgsz3843;
    CREATE(sail_int)(&zgsz3843);
    CONVERT_OF(sail_int, mach_int)(&zgsz3843, INT64_C(63));
    sail_int zgsz3844;
    CREATE(sail_int)(&zgsz3844);
    CONVERT_OF(sail_int, mach_int)(&zgsz3844, INT64_C(0));
    lbits zgsz3845;
    CREATE(lbits)(&zgsz3845);
    sailgen_integer_subrange(&zgsz3845, zgaz3144, zgsz3843, zgsz3844);
    zgsz3101 = CONVERT_OF(fbits, lbits)(zgsz3845, true);
    KILL(lbits)(&zgsz3845);
    KILL(sail_int)(&zgsz3844);
    KILL(sail_int)(&zgsz3843);
  }
  KILL(sail_int)(&zgaz3144);
  zCAP_PERM_UNSEAL = zgsz3101;

let_end_179: ;
}
static void kill_letbind_59(void) {
}

static uint64_t zCAP_PERM_BRANCH_SEALED_PAIR;
static void create_letbind_60(void) {


  uint64_t zgsz3102;
  sail_int zgaz3145;
  CREATE(sail_int)(&zgaz3145);
  {
    sail_int zgsz3851;
    CREATE(sail_int)(&zgsz3851);
    CONVERT_OF(sail_int, mach_int)(&zgsz3851, INT64_C(1));
    sail_int zgsz3852;
    CREATE(sail_int)(&zgsz3852);
    CONVERT_OF(sail_int, mach_int)(&zgsz3852, INT64_C(8));
    shl_int(&zgaz3145, zgsz3851, zgsz3852);
    KILL(sail_int)(&zgsz3852);
    KILL(sail_int)(&zgsz3851);
  }
  {
    sail_int zgsz3848;
    CREATE(sail_int)(&zgsz3848);
    CONVERT_OF(sail_int, mach_int)(&zgsz3848, INT64_C(63));
    sail_int zgsz3849;
    CREATE(sail_int)(&zgsz3849);
    CONVERT_OF(sail_int, mach_int)(&zgsz3849, INT64_C(0));
    lbits zgsz3850;
    CREATE(lbits)(&zgsz3850);
    sailgen_integer_subrange(&zgsz3850, zgaz3145, zgsz3848, zgsz3849);
    zgsz3102 = CONVERT_OF(fbits, lbits)(zgsz3850, true);
    KILL(lbits)(&zgsz3850);
    KILL(sail_int)(&zgsz3849);
    KILL(sail_int)(&zgsz3848);
  }
  KILL(sail_int)(&zgaz3145);
  zCAP_PERM_BRANCH_SEALED_PAIR = zgsz3102;

let_end_180: ;
}
static void kill_letbind_60(void) {
}

static uint64_t zCAP_PERM_MUTABLE_LOAD;
static void create_letbind_61(void) {


  uint64_t zgsz3103;
  sail_int zgaz3146;
  CREATE(sail_int)(&zgaz3146);
  {
    sail_int zgsz3856;
    CREATE(sail_int)(&zgsz3856);
    CONVERT_OF(sail_int, mach_int)(&zgsz3856, INT64_C(1));
    sail_int zgsz3857;
    CREATE(sail_int)(&zgsz3857);
    CONVERT_OF(sail_int, mach_int)(&zgsz3857, INT64_C(6));
    shl_int(&zgaz3146, zgsz3856, zgsz3857);
    KILL(sail_int)(&zgsz3857);
    KILL(sail_int)(&zgsz3856);
  }
  {
    sail_int zgsz3853;
    CREATE(sail_int)(&zgsz3853);
    CONVERT_OF(sail_int, mach_int)(&zgsz3853, INT64_C(63));
    sail_int zgsz3854;
    CREATE(sail_int)(&zgsz3854);
    CONVERT_OF(sail_int, mach_int)(&zgsz3854, INT64_C(0));
    lbits zgsz3855;
    CREATE(lbits)(&zgsz3855);
    sailgen_integer_subrange(&zgsz3855, zgaz3146, zgsz3853, zgsz3854);
    zgsz3103 = CONVERT_OF(fbits, lbits)(zgsz3855, true);
    KILL(lbits)(&zgsz3855);
    KILL(sail_int)(&zgsz3854);
    KILL(sail_int)(&zgsz3853);
  }
  KILL(sail_int)(&zgaz3146);
  zCAP_PERM_MUTABLE_LOAD = zgsz3103;

let_end_181: ;
}
static void kill_letbind_61(void) {
}

static uint64_t zCAP_PERM_GLOBAL;
static void create_letbind_62(void) {


  uint64_t zgsz3104;
  {
    sail_int zgsz3858;
    CREATE(sail_int)(&zgsz3858);
    CONVERT_OF(sail_int, mach_int)(&zgsz3858, INT64_C(1));
    sail_int zgsz3859;
    CREATE(sail_int)(&zgsz3859);
    CONVERT_OF(sail_int, mach_int)(&zgsz3859, INT64_C(63));
    sail_int zgsz3860;
    CREATE(sail_int)(&zgsz3860);
    CONVERT_OF(sail_int, mach_int)(&zgsz3860, INT64_C(0));
    lbits zgsz3861;
    CREATE(lbits)(&zgsz3861);
    sailgen_integer_subrange(&zgsz3861, zgsz3858, zgsz3859, zgsz3860);
    zgsz3104 = CONVERT_OF(fbits, lbits)(zgsz3861, true);
    KILL(lbits)(&zgsz3861);
    KILL(sail_int)(&zgsz3860);
    KILL(sail_int)(&zgsz3859);
    KILL(sail_int)(&zgsz3858);
  }
  zCAP_PERM_GLOBAL = zgsz3104;

let_end_182: ;
}
static void kill_letbind_62(void) {
}

static uint64_t zCAP_PERM_NONE;
static void create_letbind_63(void) {


  uint64_t zgsz3105;
  {
    sail_int zgsz3862;
    CREATE(sail_int)(&zgsz3862);
    CONVERT_OF(sail_int, mach_int)(&zgsz3862, INT64_C(0));
    sail_int zgsz3863;
    CREATE(sail_int)(&zgsz3863);
    CONVERT_OF(sail_int, mach_int)(&zgsz3863, INT64_C(63));
    sail_int zgsz3864;
    CREATE(sail_int)(&zgsz3864);
    CONVERT_OF(sail_int, mach_int)(&zgsz3864, INT64_C(0));
    lbits zgsz3865;
    CREATE(lbits)(&zgsz3865);
    sailgen_integer_subrange(&zgsz3865, zgsz3862, zgsz3863, zgsz3864);
    zgsz3105 = CONVERT_OF(fbits, lbits)(zgsz3865, true);
    KILL(lbits)(&zgsz3865);
    KILL(sail_int)(&zgsz3864);
    KILL(sail_int)(&zgsz3863);
    KILL(sail_int)(&zgsz3862);
  }
  zCAP_PERM_NONE = zgsz3105;

let_end_183: ;
}
static void kill_letbind_63(void) {
}

static int64_t zCAP_OTYPE_NUM_BITS;
static void create_letbind_64(void) {


  int64_t zgsz3106;
  int64_t zgaz3147;
  {
    sail_int zgsz3866;
    CREATE(sail_int)(&zgsz3866);
    CONVERT_OF(sail_int, mach_int)(&zgsz3866, zCAP_OTYPE_HI_BIT);
    sail_int zgsz3867;
    CREATE(sail_int)(&zgsz3867);
    CONVERT_OF(sail_int, mach_int)(&zgsz3867, zCAP_OTYPE_LO_BIT);
    sail_int zgsz3868;
    CREATE(sail_int)(&zgsz3868);
    sub_int(&zgsz3868, zgsz3866, zgsz3867);
    zgaz3147 = CONVERT_OF(mach_int, sail_int)(zgsz3868);
    KILL(sail_int)(&zgsz3868);
    KILL(sail_int)(&zgsz3867);
    KILL(sail_int)(&zgsz3866);
  }
  zgsz3106 = (zgaz3147 + INT64_C(1));

  zCAP_OTYPE_NUM_BITS = zgsz3106;

let_end_184: ;
}
static void kill_letbind_64(void) {
}

static int64_t zCAP_LENGTH_NUM_BITS;
static void create_letbind_65(void) {


  int64_t zgsz3107;
  zgsz3107 = (zCAP_VALUE_NUM_BITS + INT64_C(1));
  zCAP_LENGTH_NUM_BITS = zgsz3107;

let_end_185: ;
}
static void kill_letbind_65(void) {
}

static sail_int zCAP_MAX_OBJECT_TYPE;
static void create_letbind_66(void) {    CREATE(sail_int)(&zCAP_MAX_OBJECT_TYPE);


  sail_int zgsz3108;
  CREATE(sail_int)(&zgsz3108);
  sail_int zgaz3148;
  CREATE(sail_int)(&zgaz3148);
  {
    sail_int zgsz3870;
    CREATE(sail_int)(&zgsz3870);
    CONVERT_OF(sail_int, mach_int)(&zgsz3870, INT64_C(1));
    sail_int zgsz3871;
    CREATE(sail_int)(&zgsz3871);
    CONVERT_OF(sail_int, mach_int)(&zgsz3871, zCAP_OTYPE_NUM_BITS);
    shl_int(&zgaz3148, zgsz3870, zgsz3871);
    KILL(sail_int)(&zgsz3871);
    KILL(sail_int)(&zgsz3870);
  }
  {
    sail_int zgsz3869;
    CREATE(sail_int)(&zgsz3869);
    CONVERT_OF(sail_int, mach_int)(&zgsz3869, INT64_C(1));
    sub_int(&zgsz3108, zgaz3148, zgsz3869);
    KILL(sail_int)(&zgsz3869);
  }
  KILL(sail_int)(&zgaz3148);
  COPY(sail_int)(&zCAP_MAX_OBJECT_TYPE, zgsz3108);
  KILL(sail_int)(&zgsz3108);
let_end_186: ;
}
static void kill_letbind_66(void) {    KILL(sail_int)(&zCAP_MAX_OBJECT_TYPE);
}

// register __saved_exception_level
static uint64_t z__saved_exception_level;

// register __CNTControlBase
static uint64_t z__CNTControlBase;

static enum z__InstrEnc sailgen_undefined___InstrEnc(unit);

static enum z__InstrEnc sailgen_undefined___InstrEnc(unit zgsz3109)
{
  __label__ end_function_188, end_block_exception_189;

  enum z__InstrEnc zcbz324;
  zcbz324 = z__A64;
end_function_188: ;
  return zcbz324;
end_block_exception_189: ;

  return z__A32;
}

// register __ThisInstr
static uint64_t z__ThisInstr;

// register __ThisInstrEnc
static enum z__InstrEnc z__ThisInstrEnc;














































static void sailgen_undefined_instr_ast(struct zinstr_ast *rop, unit);

sail_int zghz3237;
lbits zghz3238;
sail_int zghz3239;
lbits zghz3240;

static void startup_sailgen_undefined_instr_ast(void)
{
  CREATE(sail_int)(&zghz3237);
  CREATE(lbits)(&zghz3238);
  CREATE(sail_int)(&zghz3239);
  CREATE(lbits)(&zghz3240);
}















































static void sailgen_undefined_instr_ast(struct zinstr_ast *zcbz325, unit zgsz3110)
{
  __label__ end_function_191, end_block_exception_192, end_function_235;

  struct ztuple_z8z5bv2zCz0z5bv5z9 zgaz3151;
  {
    uint64_t zgaz3149;
    {
      RECREATE(sail_int)(&zghz3239);
      CONVERT_OF(sail_int, mach_int)(&zghz3239, INT64_C(2));
      RECREATE(lbits)(&zghz3240);
      UNDEFINED(lbits)(&zghz3240, zghz3239);
      zgaz3149 = CONVERT_OF(fbits, lbits)(zghz3240, true);
    }
    uint64_t zgaz3150;
    {
      RECREATE(sail_int)(&zghz3237);
      CONVERT_OF(sail_int, mach_int)(&zghz3237, INT64_C(5));
      RECREATE(lbits)(&zghz3238);
      UNDEFINED(lbits)(&zghz3238, zghz3237);
      zgaz3150 = CONVERT_OF(fbits, lbits)(zghz3238, true);
    }
    zgaz3151.ztup0 = zgaz3149;
    zgaz3151.ztup1 = zgaz3150;
  }
  sailgen_Instr_BLRR_C_C((*(&zcbz325)), zgaz3151);

end_function_191: ;
  goto end_function_235;
end_block_exception_192: ;
  goto end_function_235;
end_function_235: ;
}



static void finish_sailgen_undefined_instr_ast(void)
{
  KILL(lbits)(&zghz3240);
  KILL(sail_int)(&zghz3239);
  KILL(lbits)(&zghz3238);
  KILL(sail_int)(&zghz3237);
}














































// register __ThisInstrAbstract
static struct zinstr_ast z__ThisInstrAbstract;

// register __highest_el_aarch32
static bool z__highest_el_aarch32;

static uint64_t z__exclusive_granule_sizze;
static void create_letbind_67(void) {


  uint64_t zgsz3112;
  zgsz3112 = UINT64_C(0x4);
  z__exclusive_granule_sizze = zgsz3112;

let_end_193: ;
}
static void kill_letbind_67(void) {
}

static uint64_t zCFG_ID_AA64PFR0_EL1_MPAM;
static void create_letbind_68(void) {


  uint64_t zgsz3113;
  zgsz3113 = UINT64_C(0x1);
  zCFG_ID_AA64PFR0_EL1_MPAM = zgsz3113;

let_end_194: ;
}
static void kill_letbind_68(void) {
}

static bool sailgen_CapUnsignedGreaterThanOrEqual(lbits, lbits);

sail_int zghz3241;
sail_int zghz3242;

static void startup_sailgen_CapUnsignedGreaterThanOrEqual(void)
{
  CREATE(sail_int)(&zghz3241);
  CREATE(sail_int)(&zghz3242);
}

static bool sailgen_CapUnsignedGreaterThanOrEqual(lbits za, lbits zb)
{
  __label__ cleanup_197, end_cleanup_198, end_function_196, end_block_exception_199;

  bool zcbz326;
  {
    RECREATE(sail_int)(&zghz3241);
    sail_unsigned(&zghz3241, za);
    RECREATE(sail_int)(&zghz3242);
    sail_unsigned(&zghz3242, zb);
    zcbz326 = gteq(zghz3241, zghz3242);
  }
  goto cleanup_197;
  /* unreachable after return */
  goto end_cleanup_198;
cleanup_197: ;
  goto end_function_196;
end_cleanup_198: ;
end_function_196: ;
  return zcbz326;
end_block_exception_199: ;

  return false;
}



static void finish_sailgen_CapUnsignedGreaterThanOrEqual(void)
{
  KILL(sail_int)(&zghz3242);
  KILL(sail_int)(&zghz3241);
}

// register SP_EL3
static lbits zSP_EL3;

static int64_t zCAPABILITY_DBITS;
static void create_letbind_69(void) {


  int64_t zgsz3114;
  {
    sail_int zgsz3876;
    CREATE(sail_int)(&zgsz3876);
    CONVERT_OF(sail_int, mach_int)(&zgsz3876, zCAPABILITY_DBYTES);
    sail_int zgsz3877;
    CREATE(sail_int)(&zgsz3877);
    CONVERT_OF(sail_int, mach_int)(&zgsz3877, INT64_C(8));
    sail_int zgsz3878;
    CREATE(sail_int)(&zgsz3878);
    mult_int(&zgsz3878, zgsz3876, zgsz3877);
    zgsz3114 = CONVERT_OF(mach_int, sail_int)(zgsz3878);
    KILL(sail_int)(&zgsz3878);
    KILL(sail_int)(&zgsz3877);
    KILL(sail_int)(&zgsz3876);
  }
  zCAPABILITY_DBITS = zgsz3114;

let_end_200: ;
}
static void kill_letbind_69(void) {
}

static bool sailgen_CapUnsignedLessThanOrEqual(lbits, lbits);

sail_int zghz3243;
sail_int zghz3244;

static void startup_sailgen_CapUnsignedLessThanOrEqual(void)
{
  CREATE(sail_int)(&zghz3243);
  CREATE(sail_int)(&zghz3244);
}

static bool sailgen_CapUnsignedLessThanOrEqual(lbits za, lbits zb)
{
  __label__ cleanup_203, end_cleanup_204, end_function_202, end_block_exception_205;

  bool zcbz327;
  {
    RECREATE(sail_int)(&zghz3243);
    sail_unsigned(&zghz3243, za);
    RECREATE(sail_int)(&zghz3244);
    sail_unsigned(&zghz3244, zb);
    zcbz327 = lteq(zghz3243, zghz3244);
  }
  goto cleanup_203;
  /* unreachable after return */
  goto end_cleanup_204;
cleanup_203: ;
  goto end_function_202;
end_cleanup_204: ;
end_function_202: ;
  return zcbz327;
end_block_exception_205: ;

  return false;
}



static void finish_sailgen_CapUnsignedLessThanOrEqual(void)
{
  KILL(sail_int)(&zghz3244);
  KILL(sail_int)(&zghz3243);
}

static uint64_t sailgen_CapGetTag(lbits);

lbits zghz3245;
sail_int zghz3246;
lbits zghz3247;
sail_int zghz3248;

static void startup_sailgen_CapGetTag(void)
{
  CREATE(lbits)(&zghz3245);
  CREATE(sail_int)(&zghz3246);
  CREATE(lbits)(&zghz3247);
  CREATE(sail_int)(&zghz3248);
}

static uint64_t sailgen_CapGetTag(lbits zc)
{
  __label__ cleanup_208, end_cleanup_209, end_function_207, end_block_exception_210;

  uint64_t zcbz328;
  {
    uint64_t zgaz3159;
    {
      fbits zgaz3158;
      {
        RECREATE(sail_int)(&zghz3248);
        CONVERT_OF(sail_int, mach_int)(&zghz3248, zCAP_TAG_BIT);
        zgaz3158 = bitvector_access(zc, zghz3248);
      }
      zgaz3159 = UINT64_C(0b0);
      zgaz3159 = update_fbits(zgaz3159, INT64_C(0), zgaz3158);
    }
    {
      RECREATE(lbits)(&zghz3245);
      CONVERT_OF(lbits, fbits)(&zghz3245, zgaz3159, UINT64_C(1) , true);
      RECREATE(sail_int)(&zghz3246);
      CONVERT_OF(sail_int, mach_int)(&zghz3246, INT64_C(64));
      RECREATE(lbits)(&zghz3247);
      zero_extend(&zghz3247, zghz3245, zghz3246);
      zcbz328 = CONVERT_OF(fbits, lbits)(zghz3247, true);
    }
  }
  goto cleanup_208;
  /* unreachable after return */
  goto end_cleanup_209;
cleanup_208: ;
  goto end_function_207;
end_cleanup_209: ;
end_function_207: ;
  return zcbz328;
end_block_exception_210: ;

  return UINT64_C(0xdeadc0de);
}



static void finish_sailgen_CapGetTag(void)
{
  KILL(sail_int)(&zghz3248);
  KILL(lbits)(&zghz3247);
  KILL(sail_int)(&zghz3246);
  KILL(lbits)(&zghz3245);
}

static void sailgen_CapSetBounds(lbits *rop, lbits, lbits, bool);

lbits zghz3249;
lbits zghz3250;
sail_int zghz3251;
lbits zghz3252;
lbits zghz3253;
lbits zghz3254;
sail_int zghz3255;
lbits zghz3256;
sail_int zghz3257;
sail_int zghz3258;
lbits zghz3259;
sail_int zghz3260;
lbits zghz3261;
sail_int zghz3262;
sail_int zghz3263;
sail_int zghz3264;
sail_int zghz3265;
sail_int zghz3266;
sail_int zghz3267;
lbits zghz3268;
sail_int zghz3269;
sail_int zghz3270;
sail_int zghz3271;
sail_int zghz3272;
sail_int zghz3273;
lbits zghz3274;
sail_int zghz3275;
sail_int zghz3276;
sail_int zghz3277;
sail_int zghz3278;
sail_int zghz3279;
lbits zghz3280;
sail_int zghz3281;
sail_int zghz3282;
lbits zghz3283;
sail_int zghz3284;
sail_int zghz3285;
sail_int zghz3286;
lbits zghz3287;
sail_int zghz3288;
sail_int zghz3289;
lbits zghz3290;
sail_int zghz3291;
sail_int zghz3292;
lbits zghz3293;
sail_int zghz3294;
sail_int zghz3295;
lbits zghz3296;
sail_int zghz3297;
sail_int zghz3298;
lbits zghz3299;
sail_int zghz3300;
sail_int zghz3301;
lbits zghz3302;
sail_int zghz3303;
sail_int zghz3304;
lbits zghz3305;
lbits zghz3306;
lbits zghz3307;
sail_int zghz3308;
sail_int zghz3309;
lbits zghz3310;
sail_int zghz3311;
lbits zghz3312;
sail_int zghz3313;
sail_int zghz3314;
lbits zghz3315;
sail_int zghz3316;
sail_int zghz3317;
sail_int zghz3318;
sail_int zghz3319;
sail_int zghz3320;
sail_int zghz3321;
lbits zghz3322;
sail_int zghz3323;
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
lbits zghz3335;
sail_int zghz3336;
lbits zghz3337;
lbits zghz3338;
lbits zghz3339;
sail_int zghz3340;
lbits zghz3341;
lbits zghz3342;
sail_int zghz3343;
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
lbits zghz3361;
sail_int zghz3362;
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
lbits zghz3382;
sail_int zghz3383;
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
lbits zghz3402;
sail_int zghz3403;
sail_int zghz3404;
sail_int zghz3405;
sail_int zghz3406;
sail_int zghz3407;
lbits zghz3408;
sail_int zghz3409;
sail_int zghz3410;
sail_int zghz3411;
lbits zghz3412;
lbits zghz3413;
lbits zghz3414;
lbits zghz3415;
sail_int zghz3416;
sail_int zghz3417;
sail_int zghz3418;
sail_int zghz3419;
sail_int zghz3420;
sail_int zghz3421;
sail_int zghz3422;
sail_int zghz3423;
lbits zghz3424;
sail_int zghz3425;
sail_int zghz3426;
sail_int zghz3427;
lbits zghz3428;
sail_int zghz3429;
sail_int zghz3430;
sail_int zghz3431;
sail_int zghz3432;
sail_int zghz3433;
sail_int zghz3434;
sail_int zghz3435;
sail_int zghz3436;
sail_int zghz3437;
sail_int zghz3438;
sail_int zghz3439;
sail_int zghz3440;
lbits zghz3441;

static void startup_sailgen_CapSetBounds(void)
{
  CREATE(lbits)(&zghz3249);
  CREATE(lbits)(&zghz3250);
  CREATE(sail_int)(&zghz3251);
  CREATE(lbits)(&zghz3252);
  CREATE(lbits)(&zghz3253);
  CREATE(lbits)(&zghz3254);
  CREATE(sail_int)(&zghz3255);
  CREATE(lbits)(&zghz3256);
  CREATE(sail_int)(&zghz3257);
  CREATE(sail_int)(&zghz3258);
  CREATE(lbits)(&zghz3259);
  CREATE(sail_int)(&zghz3260);
  CREATE(lbits)(&zghz3261);
  CREATE(sail_int)(&zghz3262);
  CREATE(sail_int)(&zghz3263);
  CREATE(sail_int)(&zghz3264);
  CREATE(sail_int)(&zghz3265);
  CREATE(sail_int)(&zghz3266);
  CREATE(sail_int)(&zghz3267);
  CREATE(lbits)(&zghz3268);
  CREATE(sail_int)(&zghz3269);
  CREATE(sail_int)(&zghz3270);
  CREATE(sail_int)(&zghz3271);
  CREATE(sail_int)(&zghz3272);
  CREATE(sail_int)(&zghz3273);
  CREATE(lbits)(&zghz3274);
  CREATE(sail_int)(&zghz3275);
  CREATE(sail_int)(&zghz3276);
  CREATE(sail_int)(&zghz3277);
  CREATE(sail_int)(&zghz3278);
  CREATE(sail_int)(&zghz3279);
  CREATE(lbits)(&zghz3280);
  CREATE(sail_int)(&zghz3281);
  CREATE(sail_int)(&zghz3282);
  CREATE(lbits)(&zghz3283);
  CREATE(sail_int)(&zghz3284);
  CREATE(sail_int)(&zghz3285);
  CREATE(sail_int)(&zghz3286);
  CREATE(lbits)(&zghz3287);
  CREATE(sail_int)(&zghz3288);
  CREATE(sail_int)(&zghz3289);
  CREATE(lbits)(&zghz3290);
  CREATE(sail_int)(&zghz3291);
  CREATE(sail_int)(&zghz3292);
  CREATE(lbits)(&zghz3293);
  CREATE(sail_int)(&zghz3294);
  CREATE(sail_int)(&zghz3295);
  CREATE(lbits)(&zghz3296);
  CREATE(sail_int)(&zghz3297);
  CREATE(sail_int)(&zghz3298);
  CREATE(lbits)(&zghz3299);
  CREATE(sail_int)(&zghz3300);
  CREATE(sail_int)(&zghz3301);
  CREATE(lbits)(&zghz3302);
  CREATE(sail_int)(&zghz3303);
  CREATE(sail_int)(&zghz3304);
  CREATE(lbits)(&zghz3305);
  CREATE(lbits)(&zghz3306);
  CREATE(lbits)(&zghz3307);
  CREATE(sail_int)(&zghz3308);
  CREATE(sail_int)(&zghz3309);
  CREATE(lbits)(&zghz3310);
  CREATE(sail_int)(&zghz3311);
  CREATE(lbits)(&zghz3312);
  CREATE(sail_int)(&zghz3313);
  CREATE(sail_int)(&zghz3314);
  CREATE(lbits)(&zghz3315);
  CREATE(sail_int)(&zghz3316);
  CREATE(sail_int)(&zghz3317);
  CREATE(sail_int)(&zghz3318);
  CREATE(sail_int)(&zghz3319);
  CREATE(sail_int)(&zghz3320);
  CREATE(sail_int)(&zghz3321);
  CREATE(lbits)(&zghz3322);
  CREATE(sail_int)(&zghz3323);
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
  CREATE(lbits)(&zghz3335);
  CREATE(sail_int)(&zghz3336);
  CREATE(lbits)(&zghz3337);
  CREATE(lbits)(&zghz3338);
  CREATE(lbits)(&zghz3339);
  CREATE(sail_int)(&zghz3340);
  CREATE(lbits)(&zghz3341);
  CREATE(lbits)(&zghz3342);
  CREATE(sail_int)(&zghz3343);
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
  CREATE(lbits)(&zghz3361);
  CREATE(sail_int)(&zghz3362);
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
  CREATE(lbits)(&zghz3382);
  CREATE(sail_int)(&zghz3383);
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
  CREATE(lbits)(&zghz3402);
  CREATE(sail_int)(&zghz3403);
  CREATE(sail_int)(&zghz3404);
  CREATE(sail_int)(&zghz3405);
  CREATE(sail_int)(&zghz3406);
  CREATE(sail_int)(&zghz3407);
  CREATE(lbits)(&zghz3408);
  CREATE(sail_int)(&zghz3409);
  CREATE(sail_int)(&zghz3410);
  CREATE(sail_int)(&zghz3411);
  CREATE(lbits)(&zghz3412);
  CREATE(lbits)(&zghz3413);
  CREATE(lbits)(&zghz3414);
  CREATE(lbits)(&zghz3415);
  CREATE(sail_int)(&zghz3416);
  CREATE(sail_int)(&zghz3417);
  CREATE(sail_int)(&zghz3418);
  CREATE(sail_int)(&zghz3419);
  CREATE(sail_int)(&zghz3420);
  CREATE(sail_int)(&zghz3421);
  CREATE(sail_int)(&zghz3422);
  CREATE(sail_int)(&zghz3423);
  CREATE(lbits)(&zghz3424);
  CREATE(sail_int)(&zghz3425);
  CREATE(sail_int)(&zghz3426);
  CREATE(sail_int)(&zghz3427);
  CREATE(lbits)(&zghz3428);
  CREATE(sail_int)(&zghz3429);
  CREATE(sail_int)(&zghz3430);
  CREATE(sail_int)(&zghz3431);
  CREATE(sail_int)(&zghz3432);
  CREATE(sail_int)(&zghz3433);
  CREATE(sail_int)(&zghz3434);
  CREATE(sail_int)(&zghz3435);
  CREATE(sail_int)(&zghz3436);
  CREATE(sail_int)(&zghz3437);
  CREATE(sail_int)(&zghz3438);
  CREATE(sail_int)(&zghz3439);
  CREATE(sail_int)(&zghz3440);
  CREATE(lbits)(&zghz3441);
}

static void sailgen_CapSetBounds(lbits *zcbz329, lbits zc, lbits zreq_len, bool zexact)
{
  __label__ case_221, finish_match_220, end_function_224, end_block_exception_227, end_function_234;

  uint64_t zL_ie;
  {
    RECREATE(sail_int)(&zghz3440);
    CONVERT_OF(sail_int, mach_int)(&zghz3440, INT64_C(13));
    RECREATE(lbits)(&zghz3441);
    UNDEFINED(lbits)(&zghz3441, zghz3440);
    zL_ie = CONVERT_OF(fbits, lbits)(zghz3441, true);
  }
  RECREATE(lbits)(&zghz3249);
  {
    int64_t zgaz3262;
    {
      int64_t zgaz3261;
      {
        int64_t zgaz3260;
        {
          RECREATE(sail_int)(&zghz3437);
          CONVERT_OF(sail_int, mach_int)(&zghz3437, INT64_C(63));
          RECREATE(sail_int)(&zghz3438);
          CONVERT_OF(sail_int, mach_int)(&zghz3438, INT64_C(0));
          RECREATE(sail_int)(&zghz3439);
          sub_int(&zghz3439, zghz3437, zghz3438);
          zgaz3260 = CONVERT_OF(mach_int, sail_int)(zghz3439);
        }
        zgaz3261 = (zgaz3260 + INT64_C(1));
      }
      zgaz3262 = (zgaz3261 + INT64_C(1));
    }
    {
      RECREATE(sail_int)(&zghz3436);
      CONVERT_OF(sail_int, mach_int)(&zghz3436, zgaz3262);
      UNDEFINED(lbits)(&zghz3249, zghz3436);
    }
  }
  RECREATE(lbits)(&zghz3250);
  {
    int64_t zgaz3259;
    {
      int64_t zgaz3258;
      {
        int64_t zgaz3257;
        {
          RECREATE(sail_int)(&zghz3433);
          CONVERT_OF(sail_int, mach_int)(&zghz3433, INT64_C(63));
          RECREATE(sail_int)(&zghz3434);
          CONVERT_OF(sail_int, mach_int)(&zghz3434, INT64_C(0));
          RECREATE(sail_int)(&zghz3435);
          sub_int(&zghz3435, zghz3433, zghz3434);
          zgaz3257 = CONVERT_OF(mach_int, sail_int)(zghz3435);
        }
        zgaz3258 = (zgaz3257 + INT64_C(1));
      }
      zgaz3259 = (zgaz3258 + INT64_C(1));
    }
    {
      RECREATE(sail_int)(&zghz3432);
      CONVERT_OF(sail_int, mach_int)(&zghz3432, zgaz3259);
      UNDEFINED(lbits)(&zghz3250, zghz3432);
    }
  }
  bool zovalid;
  zovalid = false;
  {
    bool zgaz3161;
    zgaz3161 = sailgen_CapUnsignedLessThanOrEqual(zreq_len, zCAP_BOUND_MAX);
    unit zgsz3116;
    zgsz3116 = sail_assert(zgaz3161, "v8_base.sail:45083.61-45083.62");
  }
  RECREATE(sail_int)(&zghz3251);
  {
    int64_t zgaz3256;
    {
      uint64_t zgaz3255;
      {
        int64_t zgaz3254;
        {
          RECREATE(sail_int)(&zghz3429);
          CONVERT_OF(sail_int, mach_int)(&zghz3429, zCAP_MW);
          RECREATE(sail_int)(&zghz3430);
          CONVERT_OF(sail_int, mach_int)(&zghz3430, INT64_C(1));
          RECREATE(sail_int)(&zghz3431);
          sub_int(&zghz3431, zghz3429, zghz3430);
          zgaz3254 = CONVERT_OF(mach_int, sail_int)(zghz3431);
        }
        {
          RECREATE(sail_int)(&zghz3426);
          CONVERT_OF(sail_int, mach_int)(&zghz3426, zCAP_VALUE_NUM_BITS);
          RECREATE(sail_int)(&zghz3427);
          CONVERT_OF(sail_int, mach_int)(&zghz3427, zgaz3254);
          RECREATE(lbits)(&zghz3428);
          vector_subrange_lbits(&zghz3428, zreq_len, zghz3426, zghz3427);
          zgaz3255 = CONVERT_OF(fbits, lbits)(zghz3428, true);
        }
      }
      {
        RECREATE(lbits)(&zghz3424);
        CONVERT_OF(lbits, fbits)(&zghz3424, zgaz3255, UINT64_C(50) , true);
        RECREATE(sail_int)(&zghz3425);
        count_leading_zeros(&zghz3425, zghz3424);
        zgaz3256 = CONVERT_OF(mach_int, sail_int)(zghz3425);
      }
    }
    {
      RECREATE(sail_int)(&zghz3422);
      CONVERT_OF(sail_int, mach_int)(&zghz3422, zCAP_MAX_EXPONENT);
      RECREATE(sail_int)(&zghz3423);
      CONVERT_OF(sail_int, mach_int)(&zghz3423, zgaz3256);
      sub_int(&zghz3251, zghz3422, zghz3423);
    }
  }
  bool zie;
  {
    bool zgaz3253;
    {
      RECREATE(sail_int)(&zghz3421);
      CONVERT_OF(sail_int, mach_int)(&zghz3421, INT64_C(0));
      zgaz3253 = sailgen_neq_int(zghz3251, zghz3421);
    }
    bool zgsz3118;
    if (zgaz3253) {  zgsz3118 = true;  } else {
      uint64_t zgaz3252;
      {
        fbits zgaz3251;
        {
          int64_t zgaz3250;
          {
            RECREATE(sail_int)(&zghz3418);
            CONVERT_OF(sail_int, mach_int)(&zghz3418, zCAP_MW);
            RECREATE(sail_int)(&zghz3419);
            CONVERT_OF(sail_int, mach_int)(&zghz3419, INT64_C(2));
            RECREATE(sail_int)(&zghz3420);
            sub_int(&zghz3420, zghz3418, zghz3419);
            zgaz3250 = CONVERT_OF(mach_int, sail_int)(zghz3420);
          }
          {
            RECREATE(sail_int)(&zghz3417);
            CONVERT_OF(sail_int, mach_int)(&zghz3417, zgaz3250);
            zgaz3251 = bitvector_access(zreq_len, zghz3417);
          }
        }
        zgaz3252 = UINT64_C(0b0);
        zgaz3252 = update_fbits(zgaz3252, INT64_C(0), zgaz3251);
      }
      zgsz3118 = (zgaz3252 == UINT64_C(0b1));
    }
    zie = zgsz3118;
  }
  uint64_t zbase;
  zbase = sailgen_CapGetValue(zc);
  uint64_t zabase;
  {
    bool zgaz3249;
    {
      int64_t zgaz3248;
      zgaz3248 = sailgen_CapGetExponent(zc);
      {
        RECREATE(sail_int)(&zghz3416);
        CONVERT_OF(sail_int, mach_int)(&zghz3416, zgaz3248);
        zgaz3249 = sailgen_CapBoundsUsesValue(zghz3416);
      }
    }
    if (zgaz3249) {  zabase = sailgen_CapBoundsAddress(zbase);  } else {  zabase = zbase;  }
  }
  RECREATE(lbits)(&zghz3252);
  {
    RECREATE(lbits)(&zghz3414);
    CONVERT_OF(lbits, fbits)(&zghz3414, UINT64_C(0b00), UINT64_C(2) , true);
    RECREATE(lbits)(&zghz3415);
    CONVERT_OF(lbits, fbits)(&zghz3415, zabase, UINT64_C(64) , true);
    append(&zghz3252, zghz3414, zghz3415);
  }
  RECREATE(lbits)(&zghz3253);
  {
    RECREATE(lbits)(&zghz3412);
    {
      RECREATE(lbits)(&zghz3413);
      CONVERT_OF(lbits, fbits)(&zghz3413, UINT64_C(0b0), UINT64_C(1) , true);
      append(&zghz3412, zghz3413, zreq_len);
    }
    add_bits(&zghz3253, zghz3252, zghz3412);
  }
  uint64_t zBbits;
  {
    int64_t zgaz3246;
    {
      RECREATE(sail_int)(&zghz3409);
      CONVERT_OF(sail_int, mach_int)(&zghz3409, zCAP_MW);
      RECREATE(sail_int)(&zghz3410);
      CONVERT_OF(sail_int, mach_int)(&zghz3410, INT64_C(1));
      RECREATE(sail_int)(&zghz3411);
      sub_int(&zghz3411, zghz3409, zghz3410);
      zgaz3246 = CONVERT_OF(mach_int, sail_int)(zghz3411);
    }
    {
      RECREATE(sail_int)(&zghz3406);
      CONVERT_OF(sail_int, mach_int)(&zghz3406, zgaz3246);
      RECREATE(sail_int)(&zghz3407);
      CONVERT_OF(sail_int, mach_int)(&zghz3407, INT64_C(0));
      RECREATE(lbits)(&zghz3408);
      vector_subrange_lbits(&zghz3408, zghz3252, zghz3406, zghz3407);
      zBbits = CONVERT_OF(fbits, lbits)(zghz3408, true);
    }
  }
  uint64_t zTBits;
  {
    int64_t zgaz3245;
    {
      RECREATE(sail_int)(&zghz3403);
      CONVERT_OF(sail_int, mach_int)(&zghz3403, zCAP_MW);
      RECREATE(sail_int)(&zghz3404);
      CONVERT_OF(sail_int, mach_int)(&zghz3404, INT64_C(1));
      RECREATE(sail_int)(&zghz3405);
      sub_int(&zghz3405, zghz3403, zghz3404);
      zgaz3245 = CONVERT_OF(mach_int, sail_int)(zghz3405);
    }
    {
      RECREATE(sail_int)(&zghz3400);
      CONVERT_OF(sail_int, mach_int)(&zghz3400, zgaz3245);
      RECREATE(sail_int)(&zghz3401);
      CONVERT_OF(sail_int, mach_int)(&zghz3401, INT64_C(0));
      RECREATE(lbits)(&zghz3402);
      vector_subrange_lbits(&zghz3402, zghz3253, zghz3400, zghz3401);
      zTBits = CONVERT_OF(fbits, lbits)(zghz3402, true);
    }
  }
  bool zlostTop;
  zlostTop = false;
  bool zlostBottom;
  zlostBottom = false;
  bool zincrementE_name;
  zincrementE_name = false;
  {
    unit zgsz3119;
    if (zie) {
      uint64_t zB_ie;
      {
        __label__ case_213, finish_match_212;

        {
          RECREATE(sail_int)(&zghz3379);
          COPY(sail_int)(&zghz3379, zghz3251);
          {
            bool zgaz3210;
            {
              bool zgaz3209;
              {
                RECREATE(sail_int)(&zghz3396);
                {
                  RECREATE(sail_int)(&zghz3398);
                  sailgen___id(&zghz3398, zghz3379);
                  {
                    RECREATE(sail_int)(&zghz3399);
                    CONVERT_OF(sail_int, mach_int)(&zghz3399, INT64_C(3));
                    add_int(&zghz3396, zghz3398, zghz3399);
                  }
                }
                {
                  RECREATE(sail_int)(&zghz3397);
                  CONVERT_OF(sail_int, mach_int)(&zghz3397, INT64_C(0));
                  zgaz3209 = lteq(zghz3397, zghz3396);
                }
              }
              bool zgsz3121;
              if (zgaz3209) {
                RECREATE(sail_int)(&zghz3390);
                {
                  RECREATE(sail_int)(&zghz3392);
                  {
                    RECREATE(sail_int)(&zghz3394);
                    sailgen___id(&zghz3394, zghz3379);
                    {
                      RECREATE(sail_int)(&zghz3395);
                      CONVERT_OF(sail_int, mach_int)(&zghz3395, INT64_C(16));
                      add_int(&zghz3392, zghz3394, zghz3395);
                    }
                  }
                  {
                    RECREATE(sail_int)(&zghz3393);
                    CONVERT_OF(sail_int, mach_int)(&zghz3393, INT64_C(1));
                    sub_int(&zghz3390, zghz3392, zghz3393);
                  }
                }
                {
                  RECREATE(sail_int)(&zghz3391);
                  CONVERT_OF(sail_int, mach_int)(&zghz3391, INT64_C(66));
                  zgsz3121 = lt(zghz3390, zghz3391);
                }
              } else {  zgsz3121 = false;  }
              zgaz3210 = zgsz3121;
            }
            unit zgsz3122;
            zgsz3122 = sail_assert(zgaz3210, "v8_base.sail:45098.69-45098.70");
          }
          int64_t zgaz3212;
          {
            int64_t zgaz3211;
            {
              RECREATE(sail_int)(&zghz3388);
              CONVERT_OF(sail_int, mach_int)(&zghz3388, zCAP_MW);
              RECREATE(sail_int)(&zghz3389);
              add_int(&zghz3389, zghz3379, zghz3388);
              zgaz3211 = CONVERT_OF(mach_int, sail_int)(zghz3389);
            }
            {
              RECREATE(sail_int)(&zghz3385);
              CONVERT_OF(sail_int, mach_int)(&zghz3385, zgaz3211);
              RECREATE(sail_int)(&zghz3386);
              CONVERT_OF(sail_int, mach_int)(&zghz3386, INT64_C(1));
              RECREATE(sail_int)(&zghz3387);
              sub_int(&zghz3387, zghz3385, zghz3386);
              zgaz3212 = CONVERT_OF(mach_int, sail_int)(zghz3387);
            }
          }
          int64_t zgaz3213;
          {
            RECREATE(sail_int)(&zghz3383);
            CONVERT_OF(sail_int, mach_int)(&zghz3383, INT64_C(3));
            RECREATE(sail_int)(&zghz3384);
            add_int(&zghz3384, zghz3379, zghz3383);
            zgaz3213 = CONVERT_OF(mach_int, sail_int)(zghz3384);
          }
          {
            RECREATE(sail_int)(&zghz3380);
            CONVERT_OF(sail_int, mach_int)(&zghz3380, zgaz3212);
            RECREATE(sail_int)(&zghz3381);
            CONVERT_OF(sail_int, mach_int)(&zghz3381, zgaz3213);
            RECREATE(lbits)(&zghz3382);
            vector_subrange_lbits(&zghz3382, zghz3252, zghz3380, zghz3381);
            zB_ie = CONVERT_OF(fbits, lbits)(zghz3382, true);
          }
          goto finish_match_212;
        }
      case_213: ;
        sail_match_failure("CapSetBounds");
      finish_match_212: ;
      }
      uint64_t zT_ie;
      {
        __label__ case_215, finish_match_214;

        {
          RECREATE(sail_int)(&zghz3358);
          COPY(sail_int)(&zghz3358, zghz3251);
          {
            bool zgaz3200;
            {
              bool zgaz3199;
              {
                RECREATE(sail_int)(&zghz3375);
                {
                  RECREATE(sail_int)(&zghz3377);
                  sailgen___id(&zghz3377, zghz3358);
                  {
                    RECREATE(sail_int)(&zghz3378);
                    CONVERT_OF(sail_int, mach_int)(&zghz3378, INT64_C(3));
                    add_int(&zghz3375, zghz3377, zghz3378);
                  }
                }
                {
                  RECREATE(sail_int)(&zghz3376);
                  CONVERT_OF(sail_int, mach_int)(&zghz3376, INT64_C(0));
                  zgaz3199 = lteq(zghz3376, zghz3375);
                }
              }
              bool zgsz3125;
              if (zgaz3199) {
                RECREATE(sail_int)(&zghz3369);
                {
                  RECREATE(sail_int)(&zghz3371);
                  {
                    RECREATE(sail_int)(&zghz3373);
                    sailgen___id(&zghz3373, zghz3358);
                    {
                      RECREATE(sail_int)(&zghz3374);
                      CONVERT_OF(sail_int, mach_int)(&zghz3374, INT64_C(16));
                      add_int(&zghz3371, zghz3373, zghz3374);
                    }
                  }
                  {
                    RECREATE(sail_int)(&zghz3372);
                    CONVERT_OF(sail_int, mach_int)(&zghz3372, INT64_C(1));
                    sub_int(&zghz3369, zghz3371, zghz3372);
                  }
                }
                {
                  RECREATE(sail_int)(&zghz3370);
                  CONVERT_OF(sail_int, mach_int)(&zghz3370, INT64_C(66));
                  zgsz3125 = lt(zghz3369, zghz3370);
                }
              } else {  zgsz3125 = false;  }
              zgaz3200 = zgsz3125;
            }
            unit zgsz3126;
            zgsz3126 = sail_assert(zgaz3200, "v8_base.sail:45103.69-45103.70");
          }
          int64_t zgaz3202;
          {
            int64_t zgaz3201;
            {
              RECREATE(sail_int)(&zghz3367);
              CONVERT_OF(sail_int, mach_int)(&zghz3367, zCAP_MW);
              RECREATE(sail_int)(&zghz3368);
              add_int(&zghz3368, zghz3358, zghz3367);
              zgaz3201 = CONVERT_OF(mach_int, sail_int)(zghz3368);
            }
            {
              RECREATE(sail_int)(&zghz3364);
              CONVERT_OF(sail_int, mach_int)(&zghz3364, zgaz3201);
              RECREATE(sail_int)(&zghz3365);
              CONVERT_OF(sail_int, mach_int)(&zghz3365, INT64_C(1));
              RECREATE(sail_int)(&zghz3366);
              sub_int(&zghz3366, zghz3364, zghz3365);
              zgaz3202 = CONVERT_OF(mach_int, sail_int)(zghz3366);
            }
          }
          int64_t zgaz3203;
          {
            RECREATE(sail_int)(&zghz3362);
            CONVERT_OF(sail_int, mach_int)(&zghz3362, INT64_C(3));
            RECREATE(sail_int)(&zghz3363);
            add_int(&zghz3363, zghz3358, zghz3362);
            zgaz3203 = CONVERT_OF(mach_int, sail_int)(zghz3363);
          }
          {
            RECREATE(sail_int)(&zghz3359);
            CONVERT_OF(sail_int, mach_int)(&zghz3359, zgaz3202);
            RECREATE(sail_int)(&zghz3360);
            CONVERT_OF(sail_int, mach_int)(&zghz3360, zgaz3203);
            RECREATE(lbits)(&zghz3361);
            vector_subrange_lbits(&zghz3361, zghz3253, zghz3359, zghz3360);
            zT_ie = CONVERT_OF(fbits, lbits)(zghz3361, true);
          }
          goto finish_match_214;
        }
      case_215: ;
        sail_match_failure("CapSetBounds");
      finish_match_214: ;
      }
      RECREATE(lbits)(&zghz3307);
      {
        __label__ case_217, finish_match_216;

        {
          RECREATE(sail_int)(&zghz3344);
          COPY(sail_int)(&zghz3344, zghz3251);
          {
            bool zgaz3187;
            {
              RECREATE(sail_int)(&zghz3354);
              {
                RECREATE(sail_int)(&zghz3356);
                sailgen___id(&zghz3356, zghz3344);
                {
                  RECREATE(sail_int)(&zghz3357);
                  CONVERT_OF(sail_int, mach_int)(&zghz3357, INT64_C(3));
                  add_int(&zghz3354, zghz3356, zghz3357);
                }
              }
              {
                RECREATE(sail_int)(&zghz3355);
                CONVERT_OF(sail_int, mach_int)(&zghz3355, INT64_C(0));
                zgaz3187 = gteq(zghz3354, zghz3355);
              }
            }
            unit zgsz3130;
            zgsz3130 = sail_assert(zgaz3187, "v8_base.sail:45108.46-45108.47");
          }
          {
            bool zgaz3190;
            {
              RECREATE(sail_int)(&zghz3350);
              {
                RECREATE(sail_int)(&zghz3352);
                sailgen___id(&zghz3352, zghz3344);
                {
                  RECREATE(sail_int)(&zghz3353);
                  CONVERT_OF(sail_int, mach_int)(&zghz3353, INT64_C(3));
                  add_int(&zghz3350, zghz3352, zghz3353);
                }
              }
              {
                RECREATE(sail_int)(&zghz3351);
                CONVERT_OF(sail_int, mach_int)(&zghz3351, INT64_C(66));
                zgaz3190 = gteq(zghz3351, zghz3350);
              }
            }
            unit zgsz3129;
            zgsz3129 = sail_assert(zgaz3190, "v8_base.sail:45109.47-45109.48");
          }
          RECREATE(lbits)(&zghz3345);
          {
            int64_t zgaz3191;
            {
              RECREATE(sail_int)(&zghz3348);
              CONVERT_OF(sail_int, mach_int)(&zghz3348, INT64_C(3));
              RECREATE(sail_int)(&zghz3349);
              add_int(&zghz3349, zghz3344, zghz3348);
              zgaz3191 = CONVERT_OF(mach_int, sail_int)(zghz3349);
            }
            {
              RECREATE(sail_int)(&zghz3347);
              CONVERT_OF(sail_int, mach_int)(&zghz3347, zgaz3191);
              sailgen_Ones(&zghz3345, zghz3347);
            }
          }
          int64_t zgaz3193;
          zgaz3193 = (zCAP_VALUE_NUM_BITS + INT64_C(2));
          {
            RECREATE(sail_int)(&zghz3346);
            CONVERT_OF(sail_int, mach_int)(&zghz3346, zgaz3193);
            zero_extend(&zghz3307, zghz3345, zghz3346);
          }
          goto finish_match_216;
        }
      case_217: ;
        sail_match_failure("CapSetBounds");
      finish_match_216: ;
      }
      {
        RECREATE(lbits)(&zghz3341);
        and_bits(&zghz3341, zghz3252, zghz3307);
        RECREATE(lbits)(&zghz3342);
        {
          int64_t zgaz3162;
          zgaz3162 = (zCAP_VALUE_NUM_BITS + INT64_C(2));
          {
            RECREATE(sail_int)(&zghz3343);
            CONVERT_OF(sail_int, mach_int)(&zghz3343, zgaz3162);
            sailgen_Zeros(&zghz3342, zghz3343);
          }
        }
        zlostBottom = neq_bits(zghz3341, zghz3342);
        unit zgsz3134;
        zgsz3134 = UNIT;
      }
      {
        RECREATE(lbits)(&zghz3338);
        and_bits(&zghz3338, zghz3253, zghz3307);
        RECREATE(lbits)(&zghz3339);
        {
          int64_t zgaz3165;
          zgaz3165 = (zCAP_VALUE_NUM_BITS + INT64_C(2));
          {
            RECREATE(sail_int)(&zghz3340);
            CONVERT_OF(sail_int, mach_int)(&zghz3340, zgaz3165);
            sailgen_Zeros(&zghz3339, zghz3340);
          }
        }
        zlostTop = neq_bits(zghz3338, zghz3339);
        unit zgsz3133;
        zgsz3133 = UNIT;
      }
      {
        unit zgsz3132;
        if (zlostTop) {
          {
            RECREATE(lbits)(&zghz3335);
            CONVERT_OF(lbits, fbits)(&zghz3335, zT_ie, UINT64_C(13) , true);
            RECREATE(sail_int)(&zghz3336);
            CONVERT_OF(sail_int, mach_int)(&zghz3336, INT64_C(1));
            RECREATE(lbits)(&zghz3337);
            add_bits_int(&zghz3337, zghz3335, zghz3336);
            zT_ie = CONVERT_OF(fbits, lbits)(zghz3337, true);
          }
          zgsz3132 = UNIT;
        } else {  zgsz3132 = UNIT;  }
      }
      uint64_t zL_ieshadowz34;
      zL_ieshadowz34 = ((zT_ie - zB_ie) & UINT64_C(0b1111111111111));
      {
        __label__ case_219, finish_match_218;

        unit zgsz3135;
        {
          RECREATE(sail_int)(&zghz3309);
          COPY(sail_int)(&zghz3309, zghz3251);
          bool zgaz3171;
          {
            uint64_t zgaz3170;
            {
              fbits zgaz3169;
              {
                int64_t zgaz3168;
                {
                  RECREATE(sail_int)(&zghz3332);
                  CONVERT_OF(sail_int, mach_int)(&zghz3332, zCAP_MW);
                  RECREATE(sail_int)(&zghz3333);
                  CONVERT_OF(sail_int, mach_int)(&zghz3333, INT64_C(4));
                  RECREATE(sail_int)(&zghz3334);
                  sub_int(&zghz3334, zghz3332, zghz3333);
                  zgaz3168 = CONVERT_OF(mach_int, sail_int)(zghz3334);
                }
                zgaz3169 = (UINT64_C(1) & (zL_ieshadowz34 >> zgaz3168));
              }
              zgaz3170 = UINT64_C(0b0);
              zgaz3170 = update_fbits(zgaz3170, INT64_C(0), zgaz3169);
            }
            zgaz3171 = (zgaz3170 == UINT64_C(0b1));
          }
          if (zgaz3171) {
            {
              zincrementE_name = true;
              unit zgsz3148;
              zgsz3148 = UNIT;
            }
            {
              bool zgsz3139;
              if (zlostBottom) {  zgsz3139 = true;  } else {
                uint64_t zgaz3173;
                {
                  fbits zgaz3172;
                  zgaz3172 = (UINT64_C(1) & (zB_ie >> INT64_C(0)));
                  zgaz3173 = UINT64_C(0b0);
                  zgaz3173 = update_fbits(zgaz3173, INT64_C(0), zgaz3172);
                }
                zgsz3139 = (zgaz3173 == UINT64_C(0b1));
              }
              zlostBottom = zgsz3139;
              unit zgsz3147;
              zgsz3147 = UNIT;
            }
            {
              bool zgsz3141;
              if (zlostTop) {  zgsz3141 = true;  } else {
                uint64_t zgaz3175;
                {
                  fbits zgaz3174;
                  zgaz3174 = (UINT64_C(1) & (zT_ie >> INT64_C(0)));
                  zgaz3175 = UINT64_C(0b0);
                  zgaz3175 = update_fbits(zgaz3175, INT64_C(0), zgaz3174);
                }
                zgsz3141 = (zgaz3175 == UINT64_C(0b1));
              }
              zlostTop = zgsz3141;
              unit zgsz3146;
              zgsz3146 = UNIT;
            }
            {
              bool zgaz3176;
              {
                RECREATE(sail_int)(&zghz3331);
                CONVERT_OF(sail_int, mach_int)(&zghz3331, zCAP_MAX_EXPONENT);
                zgaz3176 = lt(zghz3309, zghz3331);
              }
              unit zgsz3145;
              zgsz3145 = sail_assert(zgaz3176, "v8_base.sail:45124.47-45124.48");
            }
            {
              bool zgaz3179;
              {
                RECREATE(sail_int)(&zghz3327);
                {
                  RECREATE(sail_int)(&zghz3329);
                  sailgen___id(&zghz3329, zghz3309);
                  {
                    RECREATE(sail_int)(&zghz3330);
                    CONVERT_OF(sail_int, mach_int)(&zghz3330, INT64_C(4));
                    add_int(&zghz3327, zghz3329, zghz3330);
                  }
                }
                {
                  RECREATE(sail_int)(&zghz3328);
                  CONVERT_OF(sail_int, mach_int)(&zghz3328, INT64_C(0));
                  zgaz3179 = lteq(zghz3328, zghz3327);
                }
              }
              unit zgsz3144;
              zgsz3144 = sail_assert(zgaz3179, "v8_base.sail:45125.50-45125.51");
            }
            {
              int64_t zgaz3180;
              {
                RECREATE(sail_int)(&zghz3325);
                CONVERT_OF(sail_int, mach_int)(&zghz3325, zCAP_MW);
                RECREATE(sail_int)(&zghz3326);
                add_int(&zghz3326, zghz3309, zghz3325);
                zgaz3180 = CONVERT_OF(mach_int, sail_int)(zghz3326);
              }
              int64_t zgaz3181;
              {
                RECREATE(sail_int)(&zghz3323);
                CONVERT_OF(sail_int, mach_int)(&zghz3323, INT64_C(4));
                RECREATE(sail_int)(&zghz3324);
                add_int(&zghz3324, zghz3309, zghz3323);
                zgaz3181 = CONVERT_OF(mach_int, sail_int)(zghz3324);
              }
              {
                RECREATE(sail_int)(&zghz3320);
                CONVERT_OF(sail_int, mach_int)(&zghz3320, zgaz3180);
                RECREATE(sail_int)(&zghz3321);
                CONVERT_OF(sail_int, mach_int)(&zghz3321, zgaz3181);
                RECREATE(lbits)(&zghz3322);
                vector_subrange_lbits(&zghz3322, zghz3252, zghz3320, zghz3321);
                zB_ie = CONVERT_OF(fbits, lbits)(zghz3322, true);
              }
              unit zgsz3143;
              zgsz3143 = UNIT;
            }
            {
              int64_t zgaz3182;
              {
                RECREATE(sail_int)(&zghz3318);
                CONVERT_OF(sail_int, mach_int)(&zghz3318, zCAP_MW);
                RECREATE(sail_int)(&zghz3319);
                add_int(&zghz3319, zghz3309, zghz3318);
                zgaz3182 = CONVERT_OF(mach_int, sail_int)(zghz3319);
              }
              int64_t zgaz3183;
              {
                RECREATE(sail_int)(&zghz3316);
                CONVERT_OF(sail_int, mach_int)(&zghz3316, INT64_C(4));
                RECREATE(sail_int)(&zghz3317);
                add_int(&zghz3317, zghz3309, zghz3316);
                zgaz3183 = CONVERT_OF(mach_int, sail_int)(zghz3317);
              }
              {
                RECREATE(sail_int)(&zghz3313);
                CONVERT_OF(sail_int, mach_int)(&zghz3313, zgaz3182);
                RECREATE(sail_int)(&zghz3314);
                CONVERT_OF(sail_int, mach_int)(&zghz3314, zgaz3183);
                RECREATE(lbits)(&zghz3315);
                vector_subrange_lbits(&zghz3315, zghz3253, zghz3313, zghz3314);
                zT_ie = CONVERT_OF(fbits, lbits)(zghz3315, true);
              }
              unit zgsz3142;
              zgsz3142 = UNIT;
            }
            if (zlostTop) {
              {
                RECREATE(lbits)(&zghz3310);
                CONVERT_OF(lbits, fbits)(&zghz3310, zT_ie, UINT64_C(13) , true);
                RECREATE(sail_int)(&zghz3311);
                CONVERT_OF(sail_int, mach_int)(&zghz3311, INT64_C(1));
                RECREATE(lbits)(&zghz3312);
                add_bits_int(&zghz3312, zghz3310, zghz3311);
                zT_ie = CONVERT_OF(fbits, lbits)(zghz3312, true);
              }
              zgsz3135 = UNIT;
            } else {  zgsz3135 = UNIT;  }
          } else {  zgsz3135 = UNIT;  }
          goto finish_match_218;
        }
      case_219: ;
        sail_match_failure("CapSetBounds");
      finish_match_218: ;
        unit zgsz3151;
        zgsz3151 = zgsz3135;
      }
      {
        bool zgaz3184;
        zgaz3184 = eq_bool(zincrementE_name, true);
        unit zgsz3150;
        if (zgaz3184) {
          {
            RECREATE(sail_int)(&zghz3308);
            CONVERT_OF(sail_int, mach_int)(&zghz3308, INT64_C(1));
            add_int(&zghz3251, zghz3251, zghz3308);
          }
          zgsz3150 = UNIT;
        } else {  zgsz3150 = UNIT;  }
      }
      {
        zBbits = (zB_ie << 3) | UINT64_C(0b000);
        unit zgsz3149;
        zgsz3149 = UNIT;
      }
      zTBits = (zT_ie << 3) | UINT64_C(0b000);
      zgsz3119 = UNIT;
    } else {  zgsz3119 = UNIT;  }
  }
  RECREATE(lbits)(&zghz3254);
  {
    __label__ cleanup_225, end_cleanup_226;

    RECREATE(sail_int)(&zghz3255);
    COPY(sail_int)(&zghz3255, zghz3251);
    RECREATE(lbits)(&zghz3256);
    COPY(lbits)(&zghz3256, zc);
    {
      __label__ case_223, finish_match_222;

      struct ztuple_z8z5bvzCz0z5bvzCz0z5boolz9 zgaz3214;
      CREATE(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgaz3214);
      sailgen_CapGetBounds(&zgaz3214, zc);
      unit zgsz3153;
      {
        RECREATE(lbits)(&zghz3305);
        COPY(lbits)(&zghz3305, zgaz3214.ztup0);
        RECREATE(lbits)(&zghz3306);
        COPY(lbits)(&zghz3306, zgaz3214.ztup1);
        bool ztup__2;
        ztup__2 = zgaz3214.ztup2;
        {
          COPY(lbits)(&zghz3249, zghz3305);
          unit zgsz3155;
          zgsz3155 = UNIT;
        }
        {
          COPY(lbits)(&zghz3250, zghz3306);
          unit zgsz3154;
          zgsz3154 = UNIT;
        }
        zovalid = ztup__2;
        zgsz3153 = UNIT;
        goto finish_match_222;
      }
    case_223: ;
      sail_match_failure("CapSetBounds");
    finish_match_222: ;
      unit zgsz3167;
      zgsz3167 = zgsz3153;
      KILL(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zgaz3214);
    }
    {
      bool zgaz3221;
      {
        bool zgaz3220;
        {
          bool zgaz3219;
          {
            bool zgaz3216;
            {
              RECREATE(lbits)(&zghz3302);
              {
                RECREATE(sail_int)(&zghz3303);
                CONVERT_OF(sail_int, mach_int)(&zghz3303, INT64_C(0));
                RECREATE(sail_int)(&zghz3304);
                CONVERT_OF(sail_int, mach_int)(&zghz3304, zCAP_BOUND_NUM_BITS);
                slice(&zghz3302, zghz3252, zghz3303, zghz3304);
              }
              zgaz3216 = sailgen_CapUnsignedGreaterThanOrEqual(zghz3302, zghz3249);
            }
            zgaz3219 = not(zgaz3216);
          }
          bool zgsz3157;
          if (zgaz3219) {  zgsz3157 = true;  } else {
            bool zgaz3218;
            {
              RECREATE(lbits)(&zghz3299);
              {
                RECREATE(sail_int)(&zghz3300);
                CONVERT_OF(sail_int, mach_int)(&zghz3300, INT64_C(0));
                RECREATE(sail_int)(&zghz3301);
                CONVERT_OF(sail_int, mach_int)(&zghz3301, zCAP_BOUND_NUM_BITS);
                slice(&zghz3299, zghz3253, zghz3300, zghz3301);
              }
              zgaz3218 = sailgen_CapUnsignedLessThanOrEqual(zghz3299, zghz3250);
            }
            zgsz3157 = not(zgaz3218);
          }
          zgaz3220 = zgsz3157;
        }
        bool zgsz3158;
        if (zgaz3220) {  zgsz3158 = true;  } else {  zgsz3158 = not(zovalid);  }
        zgaz3221 = zgsz3158;
      }
      unit zgsz3166;
      if (zgaz3221) {
        fbits zgaz3222;
        zgaz3222 = sailgen_Bit(UINT64_C(0b0));
        {
          RECREATE(sail_int)(&zghz3298);
          CONVERT_OF(sail_int, mach_int)(&zghz3298, zCAP_TAG_BIT);
          update_lbits(&zghz3256, zghz3256, zghz3298, zgaz3222);
        }
        zgsz3166 = UNIT;
      } else {  zgsz3166 = UNIT;  }
    }
    {
      unit zgsz3161;
      if (zie) {
        {
          fbits zgaz3223;
          zgaz3223 = sailgen_Bit(UINT64_C(0b0));
          {
            RECREATE(sail_int)(&zghz3297);
            CONVERT_OF(sail_int, mach_int)(&zghz3297, zCAP_IE_BIT);
            update_lbits(&zghz3256, zghz3256, zghz3297, zgaz3223);
          }
          unit zgsz3165;
          zgsz3165 = UNIT;
        }
        {
          uint64_t zgaz3225;
          {
            uint64_t zgaz3224;
            {
              RECREATE(sail_int)(&zghz3294);
              CONVERT_OF(sail_int, mach_int)(&zghz3294, INT64_C(2));
              RECREATE(sail_int)(&zghz3295);
              CONVERT_OF(sail_int, mach_int)(&zghz3295, INT64_C(0));
              RECREATE(lbits)(&zghz3296);
              sailgen_integer_subrange(&zghz3296, zghz3255, zghz3294, zghz3295);
              zgaz3224 = CONVERT_OF(fbits, lbits)(zghz3296, true);
            }
            zgaz3225 = (~(zgaz3224) & UINT64_C(0b111));
          }
          {
            RECREATE(sail_int)(&zghz3291);
            CONVERT_OF(sail_int, mach_int)(&zghz3291, zCAP_BASE_EXP_HI_BIT);
            RECREATE(sail_int)(&zghz3292);
            CONVERT_OF(sail_int, mach_int)(&zghz3292, zCAP_BASE_LO_BIT);
            RECREATE(lbits)(&zghz3293);
            CONVERT_OF(lbits, fbits)(&zghz3293, zgaz3225, UINT64_C(3) , true);
            vector_update_subrange_lbits(&zghz3256, zghz3256, zghz3291, zghz3292, zghz3293);
          }
          unit zgsz3164;
          zgsz3164 = UNIT;
        }
        uint64_t zgaz3227;
        {
          uint64_t zgaz3226;
          {
            RECREATE(sail_int)(&zghz3288);
            CONVERT_OF(sail_int, mach_int)(&zghz3288, INT64_C(5));
            RECREATE(sail_int)(&zghz3289);
            CONVERT_OF(sail_int, mach_int)(&zghz3289, INT64_C(3));
            RECREATE(lbits)(&zghz3290);
            sailgen_integer_subrange(&zghz3290, zghz3255, zghz3288, zghz3289);
            zgaz3226 = CONVERT_OF(fbits, lbits)(zghz3290, true);
          }
          zgaz3227 = (~(zgaz3226) & UINT64_C(0b111));
        }
        {
          RECREATE(sail_int)(&zghz3285);
          CONVERT_OF(sail_int, mach_int)(&zghz3285, zCAP_LIMIT_EXP_HI_BIT);
          RECREATE(sail_int)(&zghz3286);
          CONVERT_OF(sail_int, mach_int)(&zghz3286, zCAP_LIMIT_LO_BIT);
          RECREATE(lbits)(&zghz3287);
          CONVERT_OF(lbits, fbits)(&zghz3287, zgaz3227, UINT64_C(3) , true);
          vector_update_subrange_lbits(&zghz3256, zghz3256, zghz3285, zghz3286, zghz3287);
        }
        zgsz3161 = UNIT;
      } else {
        {
          fbits zgaz3228;
          zgaz3228 = sailgen_Bit(UINT64_C(0b1));
          {
            RECREATE(sail_int)(&zghz3284);
            CONVERT_OF(sail_int, mach_int)(&zghz3284, zCAP_IE_BIT);
            update_lbits(&zghz3256, zghz3256, zghz3284, zgaz3228);
          }
          unit zgsz3163;
          zgsz3163 = UNIT;
        }
        {
          uint64_t zgaz3229;
          zgaz3229 = (safe_rshift(UINT64_MAX, 64 - 3) & (zBbits >> INT64_C(0)));
          {
            RECREATE(sail_int)(&zghz3281);
            CONVERT_OF(sail_int, mach_int)(&zghz3281, zCAP_BASE_EXP_HI_BIT);
            RECREATE(sail_int)(&zghz3282);
            CONVERT_OF(sail_int, mach_int)(&zghz3282, zCAP_BASE_LO_BIT);
            RECREATE(lbits)(&zghz3283);
            CONVERT_OF(lbits, fbits)(&zghz3283, zgaz3229, UINT64_C(3) , true);
            vector_update_subrange_lbits(&zghz3256, zghz3256, zghz3281, zghz3282, zghz3283);
          }
          unit zgsz3162;
          zgsz3162 = UNIT;
        }
        uint64_t zgaz3230;
        zgaz3230 = (safe_rshift(UINT64_MAX, 64 - 3) & (zTBits >> INT64_C(0)));
        {
          RECREATE(sail_int)(&zghz3278);
          CONVERT_OF(sail_int, mach_int)(&zghz3278, zCAP_LIMIT_EXP_HI_BIT);
          RECREATE(sail_int)(&zghz3279);
          CONVERT_OF(sail_int, mach_int)(&zghz3279, zCAP_LIMIT_LO_BIT);
          RECREATE(lbits)(&zghz3280);
          CONVERT_OF(lbits, fbits)(&zghz3280, zgaz3230, UINT64_C(3) , true);
          vector_update_subrange_lbits(&zghz3256, zghz3256, zghz3278, zghz3279, zghz3280);
        }
        zgsz3161 = UNIT;
      }
    }
    {
      uint64_t zgaz3232;
      {
        int64_t zgaz3231;
        {
          RECREATE(sail_int)(&zghz3275);
          CONVERT_OF(sail_int, mach_int)(&zghz3275, zCAP_MW);
          RECREATE(sail_int)(&zghz3276);
          CONVERT_OF(sail_int, mach_int)(&zghz3276, INT64_C(1));
          RECREATE(sail_int)(&zghz3277);
          sub_int(&zghz3277, zghz3275, zghz3276);
          zgaz3231 = CONVERT_OF(mach_int, sail_int)(zghz3277);
        }
        zgaz3232 = (safe_rshift(UINT64_MAX, 64 - 13) & (zBbits >> INT64_C(3)));
      }
      {
        RECREATE(sail_int)(&zghz3272);
        CONVERT_OF(sail_int, mach_int)(&zghz3272, zCAP_BASE_HI_BIT);
        RECREATE(sail_int)(&zghz3273);
        CONVERT_OF(sail_int, mach_int)(&zghz3273, zCAP_BASE_MANTISSA_LO_BIT);
        RECREATE(lbits)(&zghz3274);
        CONVERT_OF(lbits, fbits)(&zghz3274, zgaz3232, UINT64_C(13) , true);
        vector_update_subrange_lbits(&zghz3256, zghz3256, zghz3272, zghz3273, zghz3274);
      }
      unit zgsz3160;
      zgsz3160 = UNIT;
    }
    {
      uint64_t zgaz3234;
      {
        int64_t zgaz3233;
        {
          RECREATE(sail_int)(&zghz3269);
          CONVERT_OF(sail_int, mach_int)(&zghz3269, zCAP_MW);
          RECREATE(sail_int)(&zghz3270);
          CONVERT_OF(sail_int, mach_int)(&zghz3270, INT64_C(3));
          RECREATE(sail_int)(&zghz3271);
          sub_int(&zghz3271, zghz3269, zghz3270);
          zgaz3233 = CONVERT_OF(mach_int, sail_int)(zghz3271);
        }
        zgaz3234 = (safe_rshift(UINT64_MAX, 64 - 11) & (zTBits >> INT64_C(3)));
      }
      {
        RECREATE(sail_int)(&zghz3266);
        CONVERT_OF(sail_int, mach_int)(&zghz3266, zCAP_LIMIT_HI_BIT);
        RECREATE(sail_int)(&zghz3267);
        CONVERT_OF(sail_int, mach_int)(&zghz3267, zCAP_LIMIT_MANTISSA_LO_BIT);
        RECREATE(lbits)(&zghz3268);
        CONVERT_OF(lbits, fbits)(&zghz3268, zgaz3234, UINT64_C(11) , true);
        vector_update_subrange_lbits(&zghz3256, zghz3256, zghz3266, zghz3267, zghz3268);
      }
      unit zgsz3159;
      zgsz3159 = UNIT;
    }
    bool zfrom_large;
    {
      bool zgaz3244;
      {
        int64_t zgaz3243;
        zgaz3243 = sailgen_CapGetExponent(zc);
        {
          RECREATE(sail_int)(&zghz3265);
          CONVERT_OF(sail_int, mach_int)(&zghz3265, zgaz3243);
          zgaz3244 = sailgen_CapBoundsUsesValue(zghz3265);
        }
      }
      zfrom_large = not(zgaz3244);
    }
    bool zto_small;
    zto_small = sailgen_CapBoundsUsesValue(zghz3255);
    {
      bool zgaz3239;
      {
        bool zgaz3238;
        {    if (zfrom_large) {  zgaz3238 = zto_small;  } else {  zgaz3238 = false;  }
        }
        bool zgsz3169;
        if (zgaz3238) {
          uint64_t zgaz3237;
          {
            uint64_t zgaz3236;
            {
              int64_t zgaz3235;
              {
                RECREATE(sail_int)(&zghz3262);
                CONVERT_OF(sail_int, mach_int)(&zghz3262, zCAP_FLAGS_LO_BIT);
                RECREATE(sail_int)(&zghz3263);
                CONVERT_OF(sail_int, mach_int)(&zghz3263, INT64_C(1));
                RECREATE(sail_int)(&zghz3264);
                sub_int(&zghz3264, zghz3262, zghz3263);
                zgaz3235 = CONVERT_OF(mach_int, sail_int)(zghz3264);
              }
              zgaz3236 = (safe_rshift(UINT64_MAX, 64 - 56) & (zbase >> INT64_C(0)));
            }
            {
              RECREATE(lbits)(&zghz3259);
              CONVERT_OF(lbits, fbits)(&zghz3259, zgaz3236, UINT64_C(56) , true);
              RECREATE(sail_int)(&zghz3260);
              CONVERT_OF(sail_int, mach_int)(&zghz3260, INT64_C(64));
              RECREATE(lbits)(&zghz3261);
              sign_extend(&zghz3261, zghz3259, zghz3260);
              zgaz3237 = CONVERT_OF(fbits, lbits)(zghz3261, true);
            }
          }
          zgsz3169 = (zgaz3237 != zbase);
        } else {  zgsz3169 = false;  }
        zgaz3239 = zgsz3169;
      }
      unit zgsz3173;
      if (zgaz3239) {
        fbits zgaz3240;
        zgaz3240 = sailgen_Bit(UINT64_C(0b0));
        {
          RECREATE(sail_int)(&zghz3258);
          CONVERT_OF(sail_int, mach_int)(&zghz3258, zCAP_TAG_BIT);
          update_lbits(&zghz3256, zghz3256, zghz3258, zgaz3240);
        }
        zgsz3173 = UNIT;
      } else {  zgsz3173 = UNIT;  }
    }
    {
      bool zgaz3241;
      {
        if (zexact) {
          bool zgsz3170;
          if (zlostBottom) {  zgsz3170 = true;  } else {  zgsz3170 = zlostTop;  }
          zgaz3241 = zgsz3170;
        } else {  zgaz3241 = false;  }
      }
      unit zgsz3172;
      if (zgaz3241) {
        fbits zgaz3242;
        zgaz3242 = sailgen_Bit(UINT64_C(0b0));
        {
          RECREATE(sail_int)(&zghz3257);
          CONVERT_OF(sail_int, mach_int)(&zghz3257, zCAP_TAG_BIT);
          update_lbits(&zghz3256, zghz3256, zghz3257, zgaz3242);
        }
        zgsz3172 = UNIT;
      } else {  zgsz3172 = UNIT;  }
    }
    COPY(lbits)((*(&zcbz329)), zghz3256);
    goto cleanup_225;
    /* unreachable after return */
    goto finish_match_220;
    goto end_cleanup_226;
  cleanup_225: ;
    goto end_function_224;
  end_cleanup_226: ;
  }
case_221: ;
  sail_match_failure("CapSetBounds");
finish_match_220: ;
  COPY(lbits)((*(&zcbz329)), zghz3254);










end_function_224: ;
  goto end_function_234;
end_block_exception_227: ;
  goto end_function_234;
end_function_234: ;
}



static void finish_sailgen_CapSetBounds(void)
{
  KILL(lbits)(&zghz3441);
  KILL(sail_int)(&zghz3440);
  KILL(sail_int)(&zghz3439);
  KILL(sail_int)(&zghz3438);
  KILL(sail_int)(&zghz3437);
  KILL(sail_int)(&zghz3436);
  KILL(sail_int)(&zghz3435);
  KILL(sail_int)(&zghz3434);
  KILL(sail_int)(&zghz3433);
  KILL(sail_int)(&zghz3432);
  KILL(sail_int)(&zghz3431);
  KILL(sail_int)(&zghz3430);
  KILL(sail_int)(&zghz3429);
  KILL(lbits)(&zghz3428);
  KILL(sail_int)(&zghz3427);
  KILL(sail_int)(&zghz3426);
  KILL(sail_int)(&zghz3425);
  KILL(lbits)(&zghz3424);
  KILL(sail_int)(&zghz3423);
  KILL(sail_int)(&zghz3422);
  KILL(sail_int)(&zghz3421);
  KILL(sail_int)(&zghz3420);
  KILL(sail_int)(&zghz3419);
  KILL(sail_int)(&zghz3418);
  KILL(sail_int)(&zghz3417);
  KILL(sail_int)(&zghz3416);
  KILL(lbits)(&zghz3415);
  KILL(lbits)(&zghz3414);
  KILL(lbits)(&zghz3413);
  KILL(lbits)(&zghz3412);
  KILL(sail_int)(&zghz3411);
  KILL(sail_int)(&zghz3410);
  KILL(sail_int)(&zghz3409);
  KILL(lbits)(&zghz3408);
  KILL(sail_int)(&zghz3407);
  KILL(sail_int)(&zghz3406);
  KILL(sail_int)(&zghz3405);
  KILL(sail_int)(&zghz3404);
  KILL(sail_int)(&zghz3403);
  KILL(lbits)(&zghz3402);
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
  KILL(sail_int)(&zghz3383);
  KILL(lbits)(&zghz3382);
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
  KILL(sail_int)(&zghz3362);
  KILL(lbits)(&zghz3361);
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
  KILL(sail_int)(&zghz3343);
  KILL(lbits)(&zghz3342);
  KILL(lbits)(&zghz3341);
  KILL(sail_int)(&zghz3340);
  KILL(lbits)(&zghz3339);
  KILL(lbits)(&zghz3338);
  KILL(lbits)(&zghz3337);
  KILL(sail_int)(&zghz3336);
  KILL(lbits)(&zghz3335);
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
  KILL(sail_int)(&zghz3323);
  KILL(lbits)(&zghz3322);
  KILL(sail_int)(&zghz3321);
  KILL(sail_int)(&zghz3320);
  KILL(sail_int)(&zghz3319);
  KILL(sail_int)(&zghz3318);
  KILL(sail_int)(&zghz3317);
  KILL(sail_int)(&zghz3316);
  KILL(lbits)(&zghz3315);
  KILL(sail_int)(&zghz3314);
  KILL(sail_int)(&zghz3313);
  KILL(lbits)(&zghz3312);
  KILL(sail_int)(&zghz3311);
  KILL(lbits)(&zghz3310);
  KILL(sail_int)(&zghz3309);
  KILL(sail_int)(&zghz3308);
  KILL(lbits)(&zghz3307);
  KILL(lbits)(&zghz3306);
  KILL(lbits)(&zghz3305);
  KILL(sail_int)(&zghz3304);
  KILL(sail_int)(&zghz3303);
  KILL(lbits)(&zghz3302);
  KILL(sail_int)(&zghz3301);
  KILL(sail_int)(&zghz3300);
  KILL(lbits)(&zghz3299);
  KILL(sail_int)(&zghz3298);
  KILL(sail_int)(&zghz3297);
  KILL(lbits)(&zghz3296);
  KILL(sail_int)(&zghz3295);
  KILL(sail_int)(&zghz3294);
  KILL(lbits)(&zghz3293);
  KILL(sail_int)(&zghz3292);
  KILL(sail_int)(&zghz3291);
  KILL(lbits)(&zghz3290);
  KILL(sail_int)(&zghz3289);
  KILL(sail_int)(&zghz3288);
  KILL(lbits)(&zghz3287);
  KILL(sail_int)(&zghz3286);
  KILL(sail_int)(&zghz3285);
  KILL(sail_int)(&zghz3284);
  KILL(lbits)(&zghz3283);
  KILL(sail_int)(&zghz3282);
  KILL(sail_int)(&zghz3281);
  KILL(lbits)(&zghz3280);
  KILL(sail_int)(&zghz3279);
  KILL(sail_int)(&zghz3278);
  KILL(sail_int)(&zghz3277);
  KILL(sail_int)(&zghz3276);
  KILL(sail_int)(&zghz3275);
  KILL(lbits)(&zghz3274);
  KILL(sail_int)(&zghz3273);
  KILL(sail_int)(&zghz3272);
  KILL(sail_int)(&zghz3271);
  KILL(sail_int)(&zghz3270);
  KILL(sail_int)(&zghz3269);
  KILL(lbits)(&zghz3268);
  KILL(sail_int)(&zghz3267);
  KILL(sail_int)(&zghz3266);
  KILL(sail_int)(&zghz3265);
  KILL(sail_int)(&zghz3264);
  KILL(sail_int)(&zghz3263);
  KILL(sail_int)(&zghz3262);
  KILL(lbits)(&zghz3261);
  KILL(sail_int)(&zghz3260);
  KILL(lbits)(&zghz3259);
  KILL(sail_int)(&zghz3258);
  KILL(sail_int)(&zghz3257);
  KILL(lbits)(&zghz3256);
  KILL(sail_int)(&zghz3255);
  KILL(lbits)(&zghz3254);
  KILL(lbits)(&zghz3253);
  KILL(lbits)(&zghz3252);
  KILL(sail_int)(&zghz3251);
  KILL(lbits)(&zghz3250);
  KILL(lbits)(&zghz3249);
}

// register TestCap
static lbits zTestCap;

static unit sailgen_main(unit);

lbits zghz3442;
lbits zghz3443;
sail_int zghz3444;
sail_int zghz3445;

static void startup_sailgen_main(void)
{
  CREATE(lbits)(&zghz3442);
  CREATE(lbits)(&zghz3443);
  CREATE(sail_int)(&zghz3444);
  CREATE(sail_int)(&zghz3445);
}

static unit sailgen_main(unit zgsz3175)
{
  __label__ end_function_229, end_block_exception_230;

  unit zcbz330;
  {
    {
      RECREATE(sail_int)(&zghz3445);
      CONVERT_OF(sail_int, mach_int)(&zghz3445, INT64_C(129));
      sailgen_Zeros(&zTestCap, zghz3445);
    }
    unit zgsz3176;
    zgsz3176 = UNIT;
  }
  uint64_t zbits1;
  zbits1 = sailgen_CapGetValue(zTestCap);
  uint64_t zbits2;
  zbits2 = sailgen_CapGetPermissions(zTestCap);
  uint64_t zbits3;
  zbits3 = sailgen_CapGetObjectType(zTestCap);
  uint64_t zbits4;
  zbits4 = sailgen_CapGetTag(zTestCap);
  RECREATE(lbits)(&zghz3442);
  {
    RECREATE(lbits)(&zghz3443);
    {
      RECREATE(sail_int)(&zghz3444);
      CONVERT_OF(sail_int, mach_int)(&zghz3444, INT64_C(65));
      sailgen_Zeros(&zghz3443, zghz3444);
    }
    sailgen_CapSetBounds(&zghz3442, zTestCap, zghz3443, true);
  }
  struct ztuple_z8z5bvzCz0z5bvzCz0z5boolz9 zbits6;
  CREATE(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zbits6);
  sailgen_CapGetBounds(&zbits6, zTestCap);
  zcbz330 = UNIT;
  KILL(ztuple_z8z5bvzCz0z5bvzCz0z5boolz9)(&zbits6);




end_function_229: ;
  return zcbz330;
end_block_exception_230: ;

  return UNIT;
}



static void finish_sailgen_main(void)
{
  KILL(sail_int)(&zghz3445);
  KILL(sail_int)(&zghz3444);
  KILL(lbits)(&zghz3443);
  KILL(lbits)(&zghz3442);
}

static unit sailgen_initializze_registers(unit);

sail_int zghz3446;
sail_int zghz3447;
sail_int zghz3448;
lbits zghz3449;
sail_int zghz3450;
lbits zghz3451;
sail_int zghz3452;
lbits zghz3453;
lbits zghz3454;
sail_int zghz3455;
sail_int zghz3456;
sail_int zghz3457;
lbits zghz3458;
sail_int zghz3459;
lbits zghz3460;
sail_int zghz3461;
lbits zghz3462;
sail_int zghz3463;
lbits zghz3464;
sail_int zghz3465;
lbits zghz3466;
sail_int zghz3467;
lbits zghz3468;
sail_int zghz3469;
lbits zghz3470;
sail_int zghz3471;
lbits zghz3472;
sail_int zghz3473;
lbits zghz3474;
sail_int zghz3475;
lbits zghz3476;
sail_int zghz3477;
lbits zghz3478;
sail_int zghz3479;
lbits zghz3480;
sail_int zghz3481;
lbits zghz3482;
sail_int zghz3483;
sail_int zghz3484;
sail_int zghz3485;
sail_int zghz3486;
sail_int zghz3487;
sail_int zghz3488;
lbits zghz3489;
sail_int zghz3490;
lbits zghz3491;
sail_int zghz3492;
lbits zghz3493;
sail_int zghz3494;
lbits zghz3495;
sail_int zghz3496;
lbits zghz3497;
sail_int zghz3498;
lbits zghz3499;
sail_int zghz3500;
lbits zghz3501;
sail_int zghz3502;
lbits zghz3503;
sail_int zghz3504;
sail_int zghz3505;
sail_int zghz3506;
sail_int zghz3507;
sail_int zghz3508;
sail_int zghz3509;
sail_int zghz3510;
sail_int zghz3511;
sail_int zghz3512;
lbits zghz3513;
sail_int zghz3514;
lbits zghz3515;
sail_int zghz3516;
lbits zghz3517;
sail_int zghz3518;
lbits zghz3519;
sail_int zghz3520;
lbits zghz3521;
sail_int zghz3522;
lbits zghz3523;
sail_int zghz3524;
lbits zghz3525;
sail_int zghz3526;
lbits zghz3527;
sail_int zghz3528;
sail_int zghz3529;
lbits zghz3530;
sail_int zghz3531;
lbits zghz3532;
sail_int zghz3533;
lbits zghz3534;
sail_int zghz3535;
lbits zghz3536;
sail_int zghz3537;
sail_int zghz3538;
sail_int zghz3539;
lbits zghz3540;
sail_int zghz3541;
lbits zghz3542;
sail_int zghz3543;
lbits zghz3544;
sail_int zghz3545;
lbits zghz3546;
sail_int zghz3547;
lbits zghz3548;
sail_int zghz3549;
lbits zghz3550;
sail_int zghz3551;
lbits zghz3552;
sail_int zghz3553;
lbits zghz3554;
sail_int zghz3555;
lbits zghz3556;
sail_int zghz3557;
lbits zghz3558;
sail_int zghz3559;
lbits zghz3560;
sail_int zghz3561;
lbits zghz3562;
sail_int zghz3563;
lbits zghz3564;
sail_int zghz3565;
lbits zghz3566;
sail_int zghz3567;
lbits zghz3568;
sail_int zghz3569;
lbits zghz3570;
sail_int zghz3571;
lbits zghz3572;
sail_int zghz3573;
lbits zghz3574;
sail_int zghz3575;
lbits zghz3576;
sail_int zghz3577;
sail_int zghz3578;
lbits zghz3579;
sail_int zghz3580;
sail_int zghz3581;
lbits zghz3582;
sail_int zghz3583;
lbits zghz3584;
sail_int zghz3585;
lbits zghz3586;
sail_int zghz3587;
lbits zghz3588;
sail_int zghz3589;
lbits zghz3590;
sail_int zghz3591;
lbits zghz3592;
sail_int zghz3593;
lbits zghz3594;
sail_int zghz3595;
lbits zghz3596;
sail_int zghz3597;
lbits zghz3598;
sail_int zghz3599;
lbits zghz3600;
sail_int zghz3601;
lbits zghz3602;
sail_int zghz3603;
lbits zghz3604;
sail_int zghz3605;
lbits zghz3606;
sail_int zghz3607;
lbits zghz3608;
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
lbits zghz3626;
sail_int zghz3627;
lbits zghz3628;
sail_int zghz3629;
lbits zghz3630;
sail_int zghz3631;
lbits zghz3632;
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
lbits zghz3650;
sail_int zghz3651;
lbits zghz3652;
sail_int zghz3653;
lbits zghz3654;
sail_int zghz3655;
lbits zghz3656;
sail_int zghz3657;
lbits zghz3658;
sail_int zghz3659;
lbits zghz3660;
sail_int zghz3661;
lbits zghz3662;
sail_int zghz3663;
lbits zghz3664;
sail_int zghz3665;
lbits zghz3666;
sail_int zghz3667;
lbits zghz3668;
sail_int zghz3669;
lbits zghz3670;
sail_int zghz3671;
lbits zghz3672;
sail_int zghz3673;
lbits zghz3674;
sail_int zghz3675;
lbits zghz3676;
sail_int zghz3677;
lbits zghz3678;
sail_int zghz3679;
lbits zghz3680;
sail_int zghz3681;
lbits zghz3682;
sail_int zghz3683;
lbits zghz3684;
sail_int zghz3685;
lbits zghz3686;
sail_int zghz3687;
lbits zghz3688;
sail_int zghz3689;
lbits zghz3690;
sail_int zghz3691;
lbits zghz3692;
sail_int zghz3693;
lbits zghz3694;
sail_int zghz3695;
lbits zghz3696;
sail_int zghz3697;
lbits zghz3698;
sail_int zghz3699;
lbits zghz3700;
sail_int zghz3701;
lbits zghz3702;
sail_int zghz3703;
lbits zghz3704;
sail_int zghz3705;
lbits zghz3706;
sail_int zghz3707;
lbits zghz3708;
sail_int zghz3709;
lbits zghz3710;
sail_int zghz3711;
lbits zghz3712;
sail_int zghz3713;
lbits zghz3714;
sail_int zghz3715;
lbits zghz3716;
sail_int zghz3717;
lbits zghz3718;
sail_int zghz3719;
lbits zghz3720;
sail_int zghz3721;
lbits zghz3722;
sail_int zghz3723;
lbits zghz3724;
sail_int zghz3725;
lbits zghz3726;
sail_int zghz3727;
lbits zghz3728;
sail_int zghz3729;
lbits zghz3730;
sail_int zghz3731;
lbits zghz3732;
sail_int zghz3733;
lbits zghz3734;
sail_int zghz3735;
lbits zghz3736;
sail_int zghz3737;
lbits zghz3738;
sail_int zghz3739;
lbits zghz3740;
sail_int zghz3741;
lbits zghz3742;
sail_int zghz3743;
lbits zghz3744;
sail_int zghz3745;
lbits zghz3746;
sail_int zghz3747;
lbits zghz3748;
sail_int zghz3749;
lbits zghz3750;
sail_int zghz3751;
sail_int zghz3752;
lbits zghz3753;
sail_int zghz3754;
lbits zghz3755;
sail_int zghz3756;
lbits zghz3757;
sail_int zghz3758;
sail_int zghz3759;
lbits zghz3760;
sail_int zghz3761;
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
sail_int zghz3819;
lbits zghz3820;
sail_int zghz3821;
sail_int zghz3822;
lbits zghz3823;
sail_int zghz3824;
sail_int zghz3825;
lbits zghz3826;
sail_int zghz3827;
sail_int zghz3828;
lbits zghz3829;
sail_int zghz3830;
sail_int zghz3831;
lbits zghz3832;
sail_int zghz3833;
lbits zghz3834;
sail_int zghz3835;
lbits zghz3836;
sail_int zghz3837;
lbits zghz3838;
sail_int zghz3839;
lbits zghz3840;
sail_int zghz3841;
lbits zghz3842;
sail_int zghz3843;
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
lbits zghz3873;
sail_int zghz3874;
lbits zghz3875;
sail_int zghz3876;
lbits zghz3877;
sail_int zghz3878;
sail_int zghz3879;
lbits zghz3880;
sail_int zghz3881;
sail_int zghz3882;
lbits zghz3883;
sail_int zghz3884;
lbits zghz3885;
sail_int zghz3886;
sail_int zghz3887;
lbits zghz3888;
sail_int zghz3889;
lbits zghz3890;
sail_int zghz3891;
lbits zghz3892;
sail_int zghz3893;
lbits zghz3894;
sail_int zghz3895;
sail_int zghz3896;
lbits zghz3897;
sail_int zghz3898;
lbits zghz3899;
sail_int zghz3900;
lbits zghz3901;
sail_int zghz3902;
lbits zghz3903;
sail_int zghz3904;
lbits zghz3905;
sail_int zghz3906;
sail_int zghz3907;
sail_int zghz3908;
lbits zghz3909;
sail_int zghz3910;
lbits zghz3911;
sail_int zghz3912;
lbits zghz3913;
sail_int zghz3914;
lbits zghz3915;
sail_int zghz3916;
lbits zghz3917;
sail_int zghz3918;
lbits zghz3919;
sail_int zghz3920;
lbits zghz3921;
sail_int zghz3922;
lbits zghz3923;
sail_int zghz3924;
lbits zghz3925;
sail_int zghz3926;
lbits zghz3927;
sail_int zghz3928;
lbits zghz3929;
sail_int zghz3930;
lbits zghz3931;
sail_int zghz3932;
lbits zghz3933;
sail_int zghz3934;
lbits zghz3935;
sail_int zghz3936;
lbits zghz3937;
sail_int zghz3938;
lbits zghz3939;
sail_int zghz3940;
lbits zghz3941;
sail_int zghz3942;
lbits zghz3943;
sail_int zghz3944;
lbits zghz3945;
sail_int zghz3946;
lbits zghz3947;
sail_int zghz3948;
lbits zghz3949;
sail_int zghz3950;
lbits zghz3951;
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
lbits zghz3965;
sail_int zghz3966;
lbits zghz3967;
sail_int zghz3968;
lbits zghz3969;
sail_int zghz3970;
lbits zghz3971;
sail_int zghz3972;
lbits zghz3973;
sail_int zghz3974;
lbits zghz3975;
sail_int zghz3976;
lbits zghz3977;
sail_int zghz3978;
lbits zghz3979;
sail_int zghz3980;
lbits zghz3981;
sail_int zghz3982;
lbits zghz3983;
sail_int zghz3984;
lbits zghz3985;
sail_int zghz3986;
lbits zghz3987;
sail_int zghz3988;
lbits zghz3989;
sail_int zghz3990;
lbits zghz3991;
sail_int zghz3992;
lbits zghz3993;
sail_int zghz3994;
lbits zghz3995;
sail_int zghz3996;
lbits zghz3997;
sail_int zghz3998;
lbits zghz3999;
sail_int zghz31000;
lbits zghz31001;
sail_int zghz31002;
lbits zghz31003;
sail_int zghz31004;
lbits zghz31005;
sail_int zghz31006;
lbits zghz31007;
sail_int zghz31008;
lbits zghz31009;
sail_int zghz31010;
lbits zghz31011;
sail_int zghz31012;
lbits zghz31013;
sail_int zghz31014;
lbits zghz31015;
sail_int zghz31016;
lbits zghz31017;
sail_int zghz31018;
lbits zghz31019;
sail_int zghz31020;
lbits zghz31021;
sail_int zghz31022;
sail_int zghz31023;
sail_int zghz31024;
sail_int zghz31025;
sail_int zghz31026;
sail_int zghz31027;
sail_int zghz31028;
lbits zghz31029;
sail_int zghz31030;
lbits zghz31031;
sail_int zghz31032;
lbits zghz31033;
sail_int zghz31034;
lbits zghz31035;
sail_int zghz31036;
lbits zghz31037;
sail_int zghz31038;
lbits zghz31039;
sail_int zghz31040;
lbits zghz31041;
sail_int zghz31042;
lbits zghz31043;
sail_int zghz31044;
lbits zghz31045;
sail_int zghz31046;
lbits zghz31047;
sail_int zghz31048;
lbits zghz31049;
sail_int zghz31050;
lbits zghz31051;
sail_int zghz31052;
lbits zghz31053;
sail_int zghz31054;
lbits zghz31055;
sail_int zghz31056;
lbits zghz31057;
sail_int zghz31058;
sail_int zghz31059;
sail_int zghz31060;
sail_int zghz31061;
sail_int zghz31062;
sail_int zghz31063;
sail_int zghz31064;
sail_int zghz31065;
sail_int zghz31066;
sail_int zghz31067;
sail_int zghz31068;
sail_int zghz31069;
sail_int zghz31070;
sail_int zghz31071;
sail_int zghz31072;
sail_int zghz31073;
sail_int zghz31074;
sail_int zghz31075;
sail_int zghz31076;
sail_int zghz31077;
sail_int zghz31078;
sail_int zghz31079;
sail_int zghz31080;
sail_int zghz31081;
sail_int zghz31082;
sail_int zghz31083;
sail_int zghz31084;
sail_int zghz31085;
sail_int zghz31086;
sail_int zghz31087;
sail_int zghz31088;
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

static void startup_sailgen_initializze_registers(void)
{
  CREATE(sail_int)(&zghz3446);
  CREATE(sail_int)(&zghz3447);
  CREATE(sail_int)(&zghz3448);
  CREATE(lbits)(&zghz3449);
  CREATE(sail_int)(&zghz3450);
  CREATE(lbits)(&zghz3451);
  CREATE(sail_int)(&zghz3452);
  CREATE(lbits)(&zghz3453);
  CREATE(lbits)(&zghz3454);
  CREATE(sail_int)(&zghz3455);
  CREATE(sail_int)(&zghz3456);
  CREATE(sail_int)(&zghz3457);
  CREATE(lbits)(&zghz3458);
  CREATE(sail_int)(&zghz3459);
  CREATE(lbits)(&zghz3460);
  CREATE(sail_int)(&zghz3461);
  CREATE(lbits)(&zghz3462);
  CREATE(sail_int)(&zghz3463);
  CREATE(lbits)(&zghz3464);
  CREATE(sail_int)(&zghz3465);
  CREATE(lbits)(&zghz3466);
  CREATE(sail_int)(&zghz3467);
  CREATE(lbits)(&zghz3468);
  CREATE(sail_int)(&zghz3469);
  CREATE(lbits)(&zghz3470);
  CREATE(sail_int)(&zghz3471);
  CREATE(lbits)(&zghz3472);
  CREATE(sail_int)(&zghz3473);
  CREATE(lbits)(&zghz3474);
  CREATE(sail_int)(&zghz3475);
  CREATE(lbits)(&zghz3476);
  CREATE(sail_int)(&zghz3477);
  CREATE(lbits)(&zghz3478);
  CREATE(sail_int)(&zghz3479);
  CREATE(lbits)(&zghz3480);
  CREATE(sail_int)(&zghz3481);
  CREATE(lbits)(&zghz3482);
  CREATE(sail_int)(&zghz3483);
  CREATE(sail_int)(&zghz3484);
  CREATE(sail_int)(&zghz3485);
  CREATE(sail_int)(&zghz3486);
  CREATE(sail_int)(&zghz3487);
  CREATE(sail_int)(&zghz3488);
  CREATE(lbits)(&zghz3489);
  CREATE(sail_int)(&zghz3490);
  CREATE(lbits)(&zghz3491);
  CREATE(sail_int)(&zghz3492);
  CREATE(lbits)(&zghz3493);
  CREATE(sail_int)(&zghz3494);
  CREATE(lbits)(&zghz3495);
  CREATE(sail_int)(&zghz3496);
  CREATE(lbits)(&zghz3497);
  CREATE(sail_int)(&zghz3498);
  CREATE(lbits)(&zghz3499);
  CREATE(sail_int)(&zghz3500);
  CREATE(lbits)(&zghz3501);
  CREATE(sail_int)(&zghz3502);
  CREATE(lbits)(&zghz3503);
  CREATE(sail_int)(&zghz3504);
  CREATE(sail_int)(&zghz3505);
  CREATE(sail_int)(&zghz3506);
  CREATE(sail_int)(&zghz3507);
  CREATE(sail_int)(&zghz3508);
  CREATE(sail_int)(&zghz3509);
  CREATE(sail_int)(&zghz3510);
  CREATE(sail_int)(&zghz3511);
  CREATE(sail_int)(&zghz3512);
  CREATE(lbits)(&zghz3513);
  CREATE(sail_int)(&zghz3514);
  CREATE(lbits)(&zghz3515);
  CREATE(sail_int)(&zghz3516);
  CREATE(lbits)(&zghz3517);
  CREATE(sail_int)(&zghz3518);
  CREATE(lbits)(&zghz3519);
  CREATE(sail_int)(&zghz3520);
  CREATE(lbits)(&zghz3521);
  CREATE(sail_int)(&zghz3522);
  CREATE(lbits)(&zghz3523);
  CREATE(sail_int)(&zghz3524);
  CREATE(lbits)(&zghz3525);
  CREATE(sail_int)(&zghz3526);
  CREATE(lbits)(&zghz3527);
  CREATE(sail_int)(&zghz3528);
  CREATE(sail_int)(&zghz3529);
  CREATE(lbits)(&zghz3530);
  CREATE(sail_int)(&zghz3531);
  CREATE(lbits)(&zghz3532);
  CREATE(sail_int)(&zghz3533);
  CREATE(lbits)(&zghz3534);
  CREATE(sail_int)(&zghz3535);
  CREATE(lbits)(&zghz3536);
  CREATE(sail_int)(&zghz3537);
  CREATE(sail_int)(&zghz3538);
  CREATE(sail_int)(&zghz3539);
  CREATE(lbits)(&zghz3540);
  CREATE(sail_int)(&zghz3541);
  CREATE(lbits)(&zghz3542);
  CREATE(sail_int)(&zghz3543);
  CREATE(lbits)(&zghz3544);
  CREATE(sail_int)(&zghz3545);
  CREATE(lbits)(&zghz3546);
  CREATE(sail_int)(&zghz3547);
  CREATE(lbits)(&zghz3548);
  CREATE(sail_int)(&zghz3549);
  CREATE(lbits)(&zghz3550);
  CREATE(sail_int)(&zghz3551);
  CREATE(lbits)(&zghz3552);
  CREATE(sail_int)(&zghz3553);
  CREATE(lbits)(&zghz3554);
  CREATE(sail_int)(&zghz3555);
  CREATE(lbits)(&zghz3556);
  CREATE(sail_int)(&zghz3557);
  CREATE(lbits)(&zghz3558);
  CREATE(sail_int)(&zghz3559);
  CREATE(lbits)(&zghz3560);
  CREATE(sail_int)(&zghz3561);
  CREATE(lbits)(&zghz3562);
  CREATE(sail_int)(&zghz3563);
  CREATE(lbits)(&zghz3564);
  CREATE(sail_int)(&zghz3565);
  CREATE(lbits)(&zghz3566);
  CREATE(sail_int)(&zghz3567);
  CREATE(lbits)(&zghz3568);
  CREATE(sail_int)(&zghz3569);
  CREATE(lbits)(&zghz3570);
  CREATE(sail_int)(&zghz3571);
  CREATE(lbits)(&zghz3572);
  CREATE(sail_int)(&zghz3573);
  CREATE(lbits)(&zghz3574);
  CREATE(sail_int)(&zghz3575);
  CREATE(lbits)(&zghz3576);
  CREATE(sail_int)(&zghz3577);
  CREATE(sail_int)(&zghz3578);
  CREATE(lbits)(&zghz3579);
  CREATE(sail_int)(&zghz3580);
  CREATE(sail_int)(&zghz3581);
  CREATE(lbits)(&zghz3582);
  CREATE(sail_int)(&zghz3583);
  CREATE(lbits)(&zghz3584);
  CREATE(sail_int)(&zghz3585);
  CREATE(lbits)(&zghz3586);
  CREATE(sail_int)(&zghz3587);
  CREATE(lbits)(&zghz3588);
  CREATE(sail_int)(&zghz3589);
  CREATE(lbits)(&zghz3590);
  CREATE(sail_int)(&zghz3591);
  CREATE(lbits)(&zghz3592);
  CREATE(sail_int)(&zghz3593);
  CREATE(lbits)(&zghz3594);
  CREATE(sail_int)(&zghz3595);
  CREATE(lbits)(&zghz3596);
  CREATE(sail_int)(&zghz3597);
  CREATE(lbits)(&zghz3598);
  CREATE(sail_int)(&zghz3599);
  CREATE(lbits)(&zghz3600);
  CREATE(sail_int)(&zghz3601);
  CREATE(lbits)(&zghz3602);
  CREATE(sail_int)(&zghz3603);
  CREATE(lbits)(&zghz3604);
  CREATE(sail_int)(&zghz3605);
  CREATE(lbits)(&zghz3606);
  CREATE(sail_int)(&zghz3607);
  CREATE(lbits)(&zghz3608);
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
  CREATE(lbits)(&zghz3626);
  CREATE(sail_int)(&zghz3627);
  CREATE(lbits)(&zghz3628);
  CREATE(sail_int)(&zghz3629);
  CREATE(lbits)(&zghz3630);
  CREATE(sail_int)(&zghz3631);
  CREATE(lbits)(&zghz3632);
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
  CREATE(lbits)(&zghz3650);
  CREATE(sail_int)(&zghz3651);
  CREATE(lbits)(&zghz3652);
  CREATE(sail_int)(&zghz3653);
  CREATE(lbits)(&zghz3654);
  CREATE(sail_int)(&zghz3655);
  CREATE(lbits)(&zghz3656);
  CREATE(sail_int)(&zghz3657);
  CREATE(lbits)(&zghz3658);
  CREATE(sail_int)(&zghz3659);
  CREATE(lbits)(&zghz3660);
  CREATE(sail_int)(&zghz3661);
  CREATE(lbits)(&zghz3662);
  CREATE(sail_int)(&zghz3663);
  CREATE(lbits)(&zghz3664);
  CREATE(sail_int)(&zghz3665);
  CREATE(lbits)(&zghz3666);
  CREATE(sail_int)(&zghz3667);
  CREATE(lbits)(&zghz3668);
  CREATE(sail_int)(&zghz3669);
  CREATE(lbits)(&zghz3670);
  CREATE(sail_int)(&zghz3671);
  CREATE(lbits)(&zghz3672);
  CREATE(sail_int)(&zghz3673);
  CREATE(lbits)(&zghz3674);
  CREATE(sail_int)(&zghz3675);
  CREATE(lbits)(&zghz3676);
  CREATE(sail_int)(&zghz3677);
  CREATE(lbits)(&zghz3678);
  CREATE(sail_int)(&zghz3679);
  CREATE(lbits)(&zghz3680);
  CREATE(sail_int)(&zghz3681);
  CREATE(lbits)(&zghz3682);
  CREATE(sail_int)(&zghz3683);
  CREATE(lbits)(&zghz3684);
  CREATE(sail_int)(&zghz3685);
  CREATE(lbits)(&zghz3686);
  CREATE(sail_int)(&zghz3687);
  CREATE(lbits)(&zghz3688);
  CREATE(sail_int)(&zghz3689);
  CREATE(lbits)(&zghz3690);
  CREATE(sail_int)(&zghz3691);
  CREATE(lbits)(&zghz3692);
  CREATE(sail_int)(&zghz3693);
  CREATE(lbits)(&zghz3694);
  CREATE(sail_int)(&zghz3695);
  CREATE(lbits)(&zghz3696);
  CREATE(sail_int)(&zghz3697);
  CREATE(lbits)(&zghz3698);
  CREATE(sail_int)(&zghz3699);
  CREATE(lbits)(&zghz3700);
  CREATE(sail_int)(&zghz3701);
  CREATE(lbits)(&zghz3702);
  CREATE(sail_int)(&zghz3703);
  CREATE(lbits)(&zghz3704);
  CREATE(sail_int)(&zghz3705);
  CREATE(lbits)(&zghz3706);
  CREATE(sail_int)(&zghz3707);
  CREATE(lbits)(&zghz3708);
  CREATE(sail_int)(&zghz3709);
  CREATE(lbits)(&zghz3710);
  CREATE(sail_int)(&zghz3711);
  CREATE(lbits)(&zghz3712);
  CREATE(sail_int)(&zghz3713);
  CREATE(lbits)(&zghz3714);
  CREATE(sail_int)(&zghz3715);
  CREATE(lbits)(&zghz3716);
  CREATE(sail_int)(&zghz3717);
  CREATE(lbits)(&zghz3718);
  CREATE(sail_int)(&zghz3719);
  CREATE(lbits)(&zghz3720);
  CREATE(sail_int)(&zghz3721);
  CREATE(lbits)(&zghz3722);
  CREATE(sail_int)(&zghz3723);
  CREATE(lbits)(&zghz3724);
  CREATE(sail_int)(&zghz3725);
  CREATE(lbits)(&zghz3726);
  CREATE(sail_int)(&zghz3727);
  CREATE(lbits)(&zghz3728);
  CREATE(sail_int)(&zghz3729);
  CREATE(lbits)(&zghz3730);
  CREATE(sail_int)(&zghz3731);
  CREATE(lbits)(&zghz3732);
  CREATE(sail_int)(&zghz3733);
  CREATE(lbits)(&zghz3734);
  CREATE(sail_int)(&zghz3735);
  CREATE(lbits)(&zghz3736);
  CREATE(sail_int)(&zghz3737);
  CREATE(lbits)(&zghz3738);
  CREATE(sail_int)(&zghz3739);
  CREATE(lbits)(&zghz3740);
  CREATE(sail_int)(&zghz3741);
  CREATE(lbits)(&zghz3742);
  CREATE(sail_int)(&zghz3743);
  CREATE(lbits)(&zghz3744);
  CREATE(sail_int)(&zghz3745);
  CREATE(lbits)(&zghz3746);
  CREATE(sail_int)(&zghz3747);
  CREATE(lbits)(&zghz3748);
  CREATE(sail_int)(&zghz3749);
  CREATE(lbits)(&zghz3750);
  CREATE(sail_int)(&zghz3751);
  CREATE(sail_int)(&zghz3752);
  CREATE(lbits)(&zghz3753);
  CREATE(sail_int)(&zghz3754);
  CREATE(lbits)(&zghz3755);
  CREATE(sail_int)(&zghz3756);
  CREATE(lbits)(&zghz3757);
  CREATE(sail_int)(&zghz3758);
  CREATE(sail_int)(&zghz3759);
  CREATE(lbits)(&zghz3760);
  CREATE(sail_int)(&zghz3761);
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
  CREATE(sail_int)(&zghz3819);
  CREATE(lbits)(&zghz3820);
  CREATE(sail_int)(&zghz3821);
  CREATE(sail_int)(&zghz3822);
  CREATE(lbits)(&zghz3823);
  CREATE(sail_int)(&zghz3824);
  CREATE(sail_int)(&zghz3825);
  CREATE(lbits)(&zghz3826);
  CREATE(sail_int)(&zghz3827);
  CREATE(sail_int)(&zghz3828);
  CREATE(lbits)(&zghz3829);
  CREATE(sail_int)(&zghz3830);
  CREATE(sail_int)(&zghz3831);
  CREATE(lbits)(&zghz3832);
  CREATE(sail_int)(&zghz3833);
  CREATE(lbits)(&zghz3834);
  CREATE(sail_int)(&zghz3835);
  CREATE(lbits)(&zghz3836);
  CREATE(sail_int)(&zghz3837);
  CREATE(lbits)(&zghz3838);
  CREATE(sail_int)(&zghz3839);
  CREATE(lbits)(&zghz3840);
  CREATE(sail_int)(&zghz3841);
  CREATE(lbits)(&zghz3842);
  CREATE(sail_int)(&zghz3843);
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
  CREATE(lbits)(&zghz3873);
  CREATE(sail_int)(&zghz3874);
  CREATE(lbits)(&zghz3875);
  CREATE(sail_int)(&zghz3876);
  CREATE(lbits)(&zghz3877);
  CREATE(sail_int)(&zghz3878);
  CREATE(sail_int)(&zghz3879);
  CREATE(lbits)(&zghz3880);
  CREATE(sail_int)(&zghz3881);
  CREATE(sail_int)(&zghz3882);
  CREATE(lbits)(&zghz3883);
  CREATE(sail_int)(&zghz3884);
  CREATE(lbits)(&zghz3885);
  CREATE(sail_int)(&zghz3886);
  CREATE(sail_int)(&zghz3887);
  CREATE(lbits)(&zghz3888);
  CREATE(sail_int)(&zghz3889);
  CREATE(lbits)(&zghz3890);
  CREATE(sail_int)(&zghz3891);
  CREATE(lbits)(&zghz3892);
  CREATE(sail_int)(&zghz3893);
  CREATE(lbits)(&zghz3894);
  CREATE(sail_int)(&zghz3895);
  CREATE(sail_int)(&zghz3896);
  CREATE(lbits)(&zghz3897);
  CREATE(sail_int)(&zghz3898);
  CREATE(lbits)(&zghz3899);
  CREATE(sail_int)(&zghz3900);
  CREATE(lbits)(&zghz3901);
  CREATE(sail_int)(&zghz3902);
  CREATE(lbits)(&zghz3903);
  CREATE(sail_int)(&zghz3904);
  CREATE(lbits)(&zghz3905);
  CREATE(sail_int)(&zghz3906);
  CREATE(sail_int)(&zghz3907);
  CREATE(sail_int)(&zghz3908);
  CREATE(lbits)(&zghz3909);
  CREATE(sail_int)(&zghz3910);
  CREATE(lbits)(&zghz3911);
  CREATE(sail_int)(&zghz3912);
  CREATE(lbits)(&zghz3913);
  CREATE(sail_int)(&zghz3914);
  CREATE(lbits)(&zghz3915);
  CREATE(sail_int)(&zghz3916);
  CREATE(lbits)(&zghz3917);
  CREATE(sail_int)(&zghz3918);
  CREATE(lbits)(&zghz3919);
  CREATE(sail_int)(&zghz3920);
  CREATE(lbits)(&zghz3921);
  CREATE(sail_int)(&zghz3922);
  CREATE(lbits)(&zghz3923);
  CREATE(sail_int)(&zghz3924);
  CREATE(lbits)(&zghz3925);
  CREATE(sail_int)(&zghz3926);
  CREATE(lbits)(&zghz3927);
  CREATE(sail_int)(&zghz3928);
  CREATE(lbits)(&zghz3929);
  CREATE(sail_int)(&zghz3930);
  CREATE(lbits)(&zghz3931);
  CREATE(sail_int)(&zghz3932);
  CREATE(lbits)(&zghz3933);
  CREATE(sail_int)(&zghz3934);
  CREATE(lbits)(&zghz3935);
  CREATE(sail_int)(&zghz3936);
  CREATE(lbits)(&zghz3937);
  CREATE(sail_int)(&zghz3938);
  CREATE(lbits)(&zghz3939);
  CREATE(sail_int)(&zghz3940);
  CREATE(lbits)(&zghz3941);
  CREATE(sail_int)(&zghz3942);
  CREATE(lbits)(&zghz3943);
  CREATE(sail_int)(&zghz3944);
  CREATE(lbits)(&zghz3945);
  CREATE(sail_int)(&zghz3946);
  CREATE(lbits)(&zghz3947);
  CREATE(sail_int)(&zghz3948);
  CREATE(lbits)(&zghz3949);
  CREATE(sail_int)(&zghz3950);
  CREATE(lbits)(&zghz3951);
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
  CREATE(lbits)(&zghz3965);
  CREATE(sail_int)(&zghz3966);
  CREATE(lbits)(&zghz3967);
  CREATE(sail_int)(&zghz3968);
  CREATE(lbits)(&zghz3969);
  CREATE(sail_int)(&zghz3970);
  CREATE(lbits)(&zghz3971);
  CREATE(sail_int)(&zghz3972);
  CREATE(lbits)(&zghz3973);
  CREATE(sail_int)(&zghz3974);
  CREATE(lbits)(&zghz3975);
  CREATE(sail_int)(&zghz3976);
  CREATE(lbits)(&zghz3977);
  CREATE(sail_int)(&zghz3978);
  CREATE(lbits)(&zghz3979);
  CREATE(sail_int)(&zghz3980);
  CREATE(lbits)(&zghz3981);
  CREATE(sail_int)(&zghz3982);
  CREATE(lbits)(&zghz3983);
  CREATE(sail_int)(&zghz3984);
  CREATE(lbits)(&zghz3985);
  CREATE(sail_int)(&zghz3986);
  CREATE(lbits)(&zghz3987);
  CREATE(sail_int)(&zghz3988);
  CREATE(lbits)(&zghz3989);
  CREATE(sail_int)(&zghz3990);
  CREATE(lbits)(&zghz3991);
  CREATE(sail_int)(&zghz3992);
  CREATE(lbits)(&zghz3993);
  CREATE(sail_int)(&zghz3994);
  CREATE(lbits)(&zghz3995);
  CREATE(sail_int)(&zghz3996);
  CREATE(lbits)(&zghz3997);
  CREATE(sail_int)(&zghz3998);
  CREATE(lbits)(&zghz3999);
  CREATE(sail_int)(&zghz31000);
  CREATE(lbits)(&zghz31001);
  CREATE(sail_int)(&zghz31002);
  CREATE(lbits)(&zghz31003);
  CREATE(sail_int)(&zghz31004);
  CREATE(lbits)(&zghz31005);
  CREATE(sail_int)(&zghz31006);
  CREATE(lbits)(&zghz31007);
  CREATE(sail_int)(&zghz31008);
  CREATE(lbits)(&zghz31009);
  CREATE(sail_int)(&zghz31010);
  CREATE(lbits)(&zghz31011);
  CREATE(sail_int)(&zghz31012);
  CREATE(lbits)(&zghz31013);
  CREATE(sail_int)(&zghz31014);
  CREATE(lbits)(&zghz31015);
  CREATE(sail_int)(&zghz31016);
  CREATE(lbits)(&zghz31017);
  CREATE(sail_int)(&zghz31018);
  CREATE(lbits)(&zghz31019);
  CREATE(sail_int)(&zghz31020);
  CREATE(lbits)(&zghz31021);
  CREATE(sail_int)(&zghz31022);
  CREATE(sail_int)(&zghz31023);
  CREATE(sail_int)(&zghz31024);
  CREATE(sail_int)(&zghz31025);
  CREATE(sail_int)(&zghz31026);
  CREATE(sail_int)(&zghz31027);
  CREATE(sail_int)(&zghz31028);
  CREATE(lbits)(&zghz31029);
  CREATE(sail_int)(&zghz31030);
  CREATE(lbits)(&zghz31031);
  CREATE(sail_int)(&zghz31032);
  CREATE(lbits)(&zghz31033);
  CREATE(sail_int)(&zghz31034);
  CREATE(lbits)(&zghz31035);
  CREATE(sail_int)(&zghz31036);
  CREATE(lbits)(&zghz31037);
  CREATE(sail_int)(&zghz31038);
  CREATE(lbits)(&zghz31039);
  CREATE(sail_int)(&zghz31040);
  CREATE(lbits)(&zghz31041);
  CREATE(sail_int)(&zghz31042);
  CREATE(lbits)(&zghz31043);
  CREATE(sail_int)(&zghz31044);
  CREATE(lbits)(&zghz31045);
  CREATE(sail_int)(&zghz31046);
  CREATE(lbits)(&zghz31047);
  CREATE(sail_int)(&zghz31048);
  CREATE(lbits)(&zghz31049);
  CREATE(sail_int)(&zghz31050);
  CREATE(lbits)(&zghz31051);
  CREATE(sail_int)(&zghz31052);
  CREATE(lbits)(&zghz31053);
  CREATE(sail_int)(&zghz31054);
  CREATE(lbits)(&zghz31055);
  CREATE(sail_int)(&zghz31056);
  CREATE(lbits)(&zghz31057);
  CREATE(sail_int)(&zghz31058);
  CREATE(sail_int)(&zghz31059);
  CREATE(sail_int)(&zghz31060);
  CREATE(sail_int)(&zghz31061);
  CREATE(sail_int)(&zghz31062);
  CREATE(sail_int)(&zghz31063);
  CREATE(sail_int)(&zghz31064);
  CREATE(sail_int)(&zghz31065);
  CREATE(sail_int)(&zghz31066);
  CREATE(sail_int)(&zghz31067);
  CREATE(sail_int)(&zghz31068);
  CREATE(sail_int)(&zghz31069);
  CREATE(sail_int)(&zghz31070);
  CREATE(sail_int)(&zghz31071);
  CREATE(sail_int)(&zghz31072);
  CREATE(sail_int)(&zghz31073);
  CREATE(sail_int)(&zghz31074);
  CREATE(sail_int)(&zghz31075);
  CREATE(sail_int)(&zghz31076);
  CREATE(sail_int)(&zghz31077);
  CREATE(sail_int)(&zghz31078);
  CREATE(sail_int)(&zghz31079);
  CREATE(sail_int)(&zghz31080);
  CREATE(sail_int)(&zghz31081);
  CREATE(sail_int)(&zghz31082);
  CREATE(sail_int)(&zghz31083);
  CREATE(sail_int)(&zghz31084);
  CREATE(sail_int)(&zghz31085);
  CREATE(sail_int)(&zghz31086);
  CREATE(sail_int)(&zghz31087);
  CREATE(sail_int)(&zghz31088);
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
}

















































static unit sailgen_initializze_registers(unit zgsz3177)
{
  __label__ end_function_232, end_block_exception_233;

  unit zcbz331;
  {
    undefined_int(&zSEE, UNIT);
    unit zgsz3535;
    zgsz3535 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31105);
      CONVERT_OF(sail_int, mach_int)(&zghz31105, INT64_C(129));
      UNDEFINED(lbits)(&zPCC, zghz31105);
    }
    unit zgsz3534;
    zgsz3534 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31103);
      CONVERT_OF(sail_int, mach_int)(&zghz31103, INT64_C(32));
      RECREATE(lbits)(&zghz31104);
      UNDEFINED(lbits)(&zghz31104, zghz31103);
      zCNTHPS_CTL_EL2 = CONVERT_OF(fbits, lbits)(zghz31104, true);
    }
    unit zgsz3533;
    zgsz3533 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31101);
      CONVERT_OF(sail_int, mach_int)(&zghz31101, INT64_C(64));
      RECREATE(lbits)(&zghz31102);
      UNDEFINED(lbits)(&zghz31102, zghz31101);
      zCNTHPS_CVAL_EL2 = CONVERT_OF(fbits, lbits)(zghz31102, true);
    }
    unit zgsz3532;
    zgsz3532 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31099);
      CONVERT_OF(sail_int, mach_int)(&zghz31099, INT64_C(32));
      RECREATE(lbits)(&zghz31100);
      UNDEFINED(lbits)(&zghz31100, zghz31099);
      zCNTHPS_TVAL_EL2 = CONVERT_OF(fbits, lbits)(zghz31100, true);
    }
    unit zgsz3531;
    zgsz3531 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31097);
      CONVERT_OF(sail_int, mach_int)(&zghz31097, INT64_C(32));
      RECREATE(lbits)(&zghz31098);
      UNDEFINED(lbits)(&zghz31098, zghz31097);
      zCNTHVS_CTL_EL2 = CONVERT_OF(fbits, lbits)(zghz31098, true);
    }
    unit zgsz3530;
    zgsz3530 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31095);
      CONVERT_OF(sail_int, mach_int)(&zghz31095, INT64_C(64));
      RECREATE(lbits)(&zghz31096);
      UNDEFINED(lbits)(&zghz31096, zghz31095);
      zCNTHVS_CVAL_EL2 = CONVERT_OF(fbits, lbits)(zghz31096, true);
    }
    unit zgsz3529;
    zgsz3529 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31093);
      CONVERT_OF(sail_int, mach_int)(&zghz31093, INT64_C(32));
      RECREATE(lbits)(&zghz31094);
      UNDEFINED(lbits)(&zghz31094, zghz31093);
      zCNTHVS_TVAL_EL2 = CONVERT_OF(fbits, lbits)(zghz31094, true);
    }
    unit zgsz3528;
    zgsz3528 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31091);
      CONVERT_OF(sail_int, mach_int)(&zghz31091, INT64_C(32));
      RECREATE(lbits)(&zghz31092);
      UNDEFINED(lbits)(&zghz31092, zghz31091);
      zSCR_EL3 = CONVERT_OF(fbits, lbits)(zghz31092, true);
    }
    unit zgsz3527;
    zgsz3527 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31089);
      CONVERT_OF(sail_int, mach_int)(&zghz31089, INT64_C(64));
      RECREATE(lbits)(&zghz31090);
      UNDEFINED(lbits)(&zghz31090, zghz31089);
      zHCR_EL2 = CONVERT_OF(fbits, lbits)(zghz31090, true);
    }
    unit zgsz3526;
    zgsz3526 = UNIT;
  }
  {
    zPSTATE = sailgen_undefined_ProcState(UNIT);
    unit zgsz3525;
    zgsz3525 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31088);
      CONVERT_OF(sail_int, mach_int)(&zghz31088, INT64_C(129));
      UNDEFINED(lbits)(&z_R00, zghz31088);
    }
    unit zgsz3524;
    zgsz3524 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31087);
      CONVERT_OF(sail_int, mach_int)(&zghz31087, INT64_C(129));
      UNDEFINED(lbits)(&z_R01, zghz31087);
    }
    unit zgsz3523;
    zgsz3523 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31086);
      CONVERT_OF(sail_int, mach_int)(&zghz31086, INT64_C(129));
      UNDEFINED(lbits)(&z_R02, zghz31086);
    }
    unit zgsz3522;
    zgsz3522 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31085);
      CONVERT_OF(sail_int, mach_int)(&zghz31085, INT64_C(129));
      UNDEFINED(lbits)(&z_R03, zghz31085);
    }
    unit zgsz3521;
    zgsz3521 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31084);
      CONVERT_OF(sail_int, mach_int)(&zghz31084, INT64_C(129));
      UNDEFINED(lbits)(&z_R04, zghz31084);
    }
    unit zgsz3520;
    zgsz3520 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31083);
      CONVERT_OF(sail_int, mach_int)(&zghz31083, INT64_C(129));
      UNDEFINED(lbits)(&z_R05, zghz31083);
    }
    unit zgsz3519;
    zgsz3519 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31082);
      CONVERT_OF(sail_int, mach_int)(&zghz31082, INT64_C(129));
      UNDEFINED(lbits)(&z_R06, zghz31082);
    }
    unit zgsz3518;
    zgsz3518 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31081);
      CONVERT_OF(sail_int, mach_int)(&zghz31081, INT64_C(129));
      UNDEFINED(lbits)(&z_R07, zghz31081);
    }
    unit zgsz3517;
    zgsz3517 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31080);
      CONVERT_OF(sail_int, mach_int)(&zghz31080, INT64_C(129));
      UNDEFINED(lbits)(&z_R08, zghz31080);
    }
    unit zgsz3516;
    zgsz3516 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31079);
      CONVERT_OF(sail_int, mach_int)(&zghz31079, INT64_C(129));
      UNDEFINED(lbits)(&z_R09, zghz31079);
    }
    unit zgsz3515;
    zgsz3515 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31078);
      CONVERT_OF(sail_int, mach_int)(&zghz31078, INT64_C(129));
      UNDEFINED(lbits)(&z_R10, zghz31078);
    }
    unit zgsz3514;
    zgsz3514 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31077);
      CONVERT_OF(sail_int, mach_int)(&zghz31077, INT64_C(129));
      UNDEFINED(lbits)(&z_R11, zghz31077);
    }
    unit zgsz3513;
    zgsz3513 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31076);
      CONVERT_OF(sail_int, mach_int)(&zghz31076, INT64_C(129));
      UNDEFINED(lbits)(&z_R12, zghz31076);
    }
    unit zgsz3512;
    zgsz3512 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31075);
      CONVERT_OF(sail_int, mach_int)(&zghz31075, INT64_C(129));
      UNDEFINED(lbits)(&z_R13, zghz31075);
    }
    unit zgsz3511;
    zgsz3511 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31074);
      CONVERT_OF(sail_int, mach_int)(&zghz31074, INT64_C(129));
      UNDEFINED(lbits)(&z_R14, zghz31074);
    }
    unit zgsz3510;
    zgsz3510 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31073);
      CONVERT_OF(sail_int, mach_int)(&zghz31073, INT64_C(129));
      UNDEFINED(lbits)(&z_R15, zghz31073);
    }
    unit zgsz3509;
    zgsz3509 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31072);
      CONVERT_OF(sail_int, mach_int)(&zghz31072, INT64_C(129));
      UNDEFINED(lbits)(&z_R16, zghz31072);
    }
    unit zgsz3508;
    zgsz3508 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31071);
      CONVERT_OF(sail_int, mach_int)(&zghz31071, INT64_C(129));
      UNDEFINED(lbits)(&z_R17, zghz31071);
    }
    unit zgsz3507;
    zgsz3507 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31070);
      CONVERT_OF(sail_int, mach_int)(&zghz31070, INT64_C(129));
      UNDEFINED(lbits)(&z_R18, zghz31070);
    }
    unit zgsz3506;
    zgsz3506 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31069);
      CONVERT_OF(sail_int, mach_int)(&zghz31069, INT64_C(129));
      UNDEFINED(lbits)(&z_R19, zghz31069);
    }
    unit zgsz3505;
    zgsz3505 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31068);
      CONVERT_OF(sail_int, mach_int)(&zghz31068, INT64_C(129));
      UNDEFINED(lbits)(&z_R20, zghz31068);
    }
    unit zgsz3504;
    zgsz3504 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31067);
      CONVERT_OF(sail_int, mach_int)(&zghz31067, INT64_C(129));
      UNDEFINED(lbits)(&z_R21, zghz31067);
    }
    unit zgsz3503;
    zgsz3503 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31066);
      CONVERT_OF(sail_int, mach_int)(&zghz31066, INT64_C(129));
      UNDEFINED(lbits)(&z_R22, zghz31066);
    }
    unit zgsz3502;
    zgsz3502 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31065);
      CONVERT_OF(sail_int, mach_int)(&zghz31065, INT64_C(129));
      UNDEFINED(lbits)(&z_R23, zghz31065);
    }
    unit zgsz3501;
    zgsz3501 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31064);
      CONVERT_OF(sail_int, mach_int)(&zghz31064, INT64_C(129));
      UNDEFINED(lbits)(&z_R24, zghz31064);
    }
    unit zgsz3500;
    zgsz3500 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31063);
      CONVERT_OF(sail_int, mach_int)(&zghz31063, INT64_C(129));
      UNDEFINED(lbits)(&z_R25, zghz31063);
    }
    unit zgsz3499;
    zgsz3499 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31062);
      CONVERT_OF(sail_int, mach_int)(&zghz31062, INT64_C(129));
      UNDEFINED(lbits)(&z_R26, zghz31062);
    }
    unit zgsz3498;
    zgsz3498 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31061);
      CONVERT_OF(sail_int, mach_int)(&zghz31061, INT64_C(129));
      UNDEFINED(lbits)(&z_R27, zghz31061);
    }
    unit zgsz3497;
    zgsz3497 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31060);
      CONVERT_OF(sail_int, mach_int)(&zghz31060, INT64_C(129));
      UNDEFINED(lbits)(&z_R28, zghz31060);
    }
    unit zgsz3496;
    zgsz3496 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31059);
      CONVERT_OF(sail_int, mach_int)(&zghz31059, INT64_C(129));
      UNDEFINED(lbits)(&z_R29, zghz31059);
    }
    unit zgsz3495;
    zgsz3495 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31058);
      CONVERT_OF(sail_int, mach_int)(&zghz31058, INT64_C(129));
      UNDEFINED(lbits)(&z_R30, zghz31058);
    }
    unit zgsz3494;
    zgsz3494 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31056);
      CONVERT_OF(sail_int, mach_int)(&zghz31056, INT64_C(32));
      RECREATE(lbits)(&zghz31057);
      UNDEFINED(lbits)(&zghz31057, zghz31056);
      zESR_EL1 = CONVERT_OF(fbits, lbits)(zghz31057, true);
    }
    unit zgsz3493;
    zgsz3493 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31054);
      CONVERT_OF(sail_int, mach_int)(&zghz31054, INT64_C(32));
      RECREATE(lbits)(&zghz31055);
      UNDEFINED(lbits)(&zghz31055, zghz31054);
      zESR_EL2 = CONVERT_OF(fbits, lbits)(zghz31055, true);
    }
    unit zgsz3492;
    zgsz3492 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31052);
      CONVERT_OF(sail_int, mach_int)(&zghz31052, INT64_C(32));
      RECREATE(lbits)(&zghz31053);
      UNDEFINED(lbits)(&zghz31053, zghz31052);
      zESR_EL3 = CONVERT_OF(fbits, lbits)(zghz31053, true);
    }
    unit zgsz3491;
    zgsz3491 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31050);
      CONVERT_OF(sail_int, mach_int)(&zghz31050, INT64_C(64));
      RECREATE(lbits)(&zghz31051);
      UNDEFINED(lbits)(&zghz31051, zghz31050);
      zFAR_EL1 = CONVERT_OF(fbits, lbits)(zghz31051, true);
    }
    unit zgsz3490;
    zgsz3490 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31048);
      CONVERT_OF(sail_int, mach_int)(&zghz31048, INT64_C(64));
      RECREATE(lbits)(&zghz31049);
      UNDEFINED(lbits)(&zghz31049, zghz31048);
      zFAR_EL2 = CONVERT_OF(fbits, lbits)(zghz31049, true);
    }
    unit zgsz3489;
    zgsz3489 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31046);
      CONVERT_OF(sail_int, mach_int)(&zghz31046, INT64_C(64));
      RECREATE(lbits)(&zghz31047);
      UNDEFINED(lbits)(&zghz31047, zghz31046);
      zFAR_EL3 = CONVERT_OF(fbits, lbits)(zghz31047, true);
    }
    unit zgsz3488;
    zgsz3488 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31044);
      CONVERT_OF(sail_int, mach_int)(&zghz31044, INT64_C(64));
      RECREATE(lbits)(&zghz31045);
      UNDEFINED(lbits)(&zghz31045, zghz31044);
      zHPFAR_EL2 = CONVERT_OF(fbits, lbits)(zghz31045, true);
    }
    unit zgsz3487;
    zgsz3487 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31042);
      CONVERT_OF(sail_int, mach_int)(&zghz31042, INT64_C(64));
      RECREATE(lbits)(&zghz31043);
      UNDEFINED(lbits)(&zghz31043, zghz31042);
      zTCR_EL1 = CONVERT_OF(fbits, lbits)(zghz31043, true);
    }
    unit zgsz3486;
    zgsz3486 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31040);
      CONVERT_OF(sail_int, mach_int)(&zghz31040, INT64_C(64));
      RECREATE(lbits)(&zghz31041);
      UNDEFINED(lbits)(&zghz31041, zghz31040);
      zTCR_EL2 = CONVERT_OF(fbits, lbits)(zghz31041, true);
    }
    unit zgsz3485;
    zgsz3485 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31038);
      CONVERT_OF(sail_int, mach_int)(&zghz31038, INT64_C(32));
      RECREATE(lbits)(&zghz31039);
      UNDEFINED(lbits)(&zghz31039, zghz31038);
      zTCR_EL3 = CONVERT_OF(fbits, lbits)(zghz31039, true);
    }
    unit zgsz3484;
    zgsz3484 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31036);
      CONVERT_OF(sail_int, mach_int)(&zghz31036, INT64_C(64));
      RECREATE(lbits)(&zghz31037);
      UNDEFINED(lbits)(&zghz31037, zghz31036);
      z_PC = CONVERT_OF(fbits, lbits)(zghz31037, true);
    }
    unit zgsz3483;
    zgsz3483 = UNIT;
  }
  {
    z__BranchTaken = false;
    unit zgsz3482;
    zgsz3482 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31034);
      CONVERT_OF(sail_int, mach_int)(&zghz31034, INT64_C(32));
      RECREATE(lbits)(&zghz31035);
      UNDEFINED(lbits)(&zghz31035, zghz31034);
      zCCTLR_EL0 = CONVERT_OF(fbits, lbits)(zghz31035, true);
    }
    unit zgsz3481;
    zgsz3481 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31032);
      CONVERT_OF(sail_int, mach_int)(&zghz31032, INT64_C(32));
      RECREATE(lbits)(&zghz31033);
      UNDEFINED(lbits)(&zghz31033, zghz31032);
      zCCTLR_EL1 = CONVERT_OF(fbits, lbits)(zghz31033, true);
    }
    unit zgsz3480;
    zgsz3480 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31030);
      CONVERT_OF(sail_int, mach_int)(&zghz31030, INT64_C(32));
      RECREATE(lbits)(&zghz31031);
      UNDEFINED(lbits)(&zghz31031, zghz31030);
      zCCTLR_EL2 = CONVERT_OF(fbits, lbits)(zghz31031, true);
    }
    unit zgsz3479;
    zgsz3479 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31028);
      CONVERT_OF(sail_int, mach_int)(&zghz31028, INT64_C(32));
      RECREATE(lbits)(&zghz31029);
      UNDEFINED(lbits)(&zghz31029, zghz31028);
      zCCTLR_EL3 = CONVERT_OF(fbits, lbits)(zghz31029, true);
    }
    unit zgsz3478;
    zgsz3478 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31027);
      CONVERT_OF(sail_int, mach_int)(&zghz31027, INT64_C(129));
      UNDEFINED(lbits)(&zELR_EL1, zghz31027);
    }
    unit zgsz3477;
    zgsz3477 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31026);
      CONVERT_OF(sail_int, mach_int)(&zghz31026, INT64_C(129));
      UNDEFINED(lbits)(&zELR_EL2, zghz31026);
    }
    unit zgsz3476;
    zgsz3476 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31025);
      CONVERT_OF(sail_int, mach_int)(&zghz31025, INT64_C(129));
      UNDEFINED(lbits)(&zELR_EL3, zghz31025);
    }
    unit zgsz3475;
    zgsz3475 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31024);
      CONVERT_OF(sail_int, mach_int)(&zghz31024, INT64_C(129));
      UNDEFINED(lbits)(&zVBAR_EL1, zghz31024);
    }
    unit zgsz3474;
    zgsz3474 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31023);
      CONVERT_OF(sail_int, mach_int)(&zghz31023, INT64_C(129));
      UNDEFINED(lbits)(&zVBAR_EL2, zghz31023);
    }
    unit zgsz3473;
    zgsz3473 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31022);
      CONVERT_OF(sail_int, mach_int)(&zghz31022, INT64_C(129));
      UNDEFINED(lbits)(&zVBAR_EL3, zghz31022);
    }
    unit zgsz3472;
    zgsz3472 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31020);
      CONVERT_OF(sail_int, mach_int)(&zghz31020, INT64_C(32));
      RECREATE(lbits)(&zghz31021);
      UNDEFINED(lbits)(&zghz31021, zghz31020);
      zCPACR_EL1 = CONVERT_OF(fbits, lbits)(zghz31021, true);
    }
    unit zgsz3471;
    zgsz3471 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31018);
      CONVERT_OF(sail_int, mach_int)(&zghz31018, INT64_C(32));
      RECREATE(lbits)(&zghz31019);
      UNDEFINED(lbits)(&zghz31019, zghz31018);
      zCPTR_EL2 = CONVERT_OF(fbits, lbits)(zghz31019, true);
    }
    unit zgsz3470;
    zgsz3470 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31016);
      CONVERT_OF(sail_int, mach_int)(&zghz31016, INT64_C(32));
      RECREATE(lbits)(&zghz31017);
      UNDEFINED(lbits)(&zghz31017, zghz31016);
      zCPTR_EL3 = CONVERT_OF(fbits, lbits)(zghz31017, true);
    }
    unit zgsz3469;
    zgsz3469 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31014);
      CONVERT_OF(sail_int, mach_int)(&zghz31014, INT64_C(32));
      RECREATE(lbits)(&zghz31015);
      UNDEFINED(lbits)(&zghz31015, zghz31014);
      zEDSCR = CONVERT_OF(fbits, lbits)(zghz31015, true);
    }
    unit zgsz3468;
    zgsz3468 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31012);
      CONVERT_OF(sail_int, mach_int)(&zghz31012, INT64_C(64));
      RECREATE(lbits)(&zghz31013);
      UNDEFINED(lbits)(&zghz31013, zghz31012);
      zSCTLR_EL1 = CONVERT_OF(fbits, lbits)(zghz31013, true);
    }
    unit zgsz3467;
    zgsz3467 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31010);
      CONVERT_OF(sail_int, mach_int)(&zghz31010, INT64_C(64));
      RECREATE(lbits)(&zghz31011);
      UNDEFINED(lbits)(&zghz31011, zghz31010);
      zSCTLR_EL2 = CONVERT_OF(fbits, lbits)(zghz31011, true);
    }
    unit zgsz3466;
    zgsz3466 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31008);
      CONVERT_OF(sail_int, mach_int)(&zghz31008, INT64_C(64));
      RECREATE(lbits)(&zghz31009);
      UNDEFINED(lbits)(&zghz31009, zghz31008);
      zSCTLR_EL3 = CONVERT_OF(fbits, lbits)(zghz31009, true);
    }
    unit zgsz3465;
    zgsz3465 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31006);
      CONVERT_OF(sail_int, mach_int)(&zghz31006, INT64_C(32));
      RECREATE(lbits)(&zghz31007);
      UNDEFINED(lbits)(&zghz31007, zghz31006);
      zSPSR_EL1 = CONVERT_OF(fbits, lbits)(zghz31007, true);
    }
    unit zgsz3464;
    zgsz3464 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31004);
      CONVERT_OF(sail_int, mach_int)(&zghz31004, INT64_C(32));
      RECREATE(lbits)(&zghz31005);
      UNDEFINED(lbits)(&zghz31005, zghz31004);
      zSPSR_EL2 = CONVERT_OF(fbits, lbits)(zghz31005, true);
    }
    unit zgsz3463;
    zgsz3463 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31002);
      CONVERT_OF(sail_int, mach_int)(&zghz31002, INT64_C(32));
      RECREATE(lbits)(&zghz31003);
      UNDEFINED(lbits)(&zghz31003, zghz31002);
      zSPSR_EL3 = CONVERT_OF(fbits, lbits)(zghz31003, true);
    }
    unit zgsz3462;
    zgsz3462 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz31000);
      CONVERT_OF(sail_int, mach_int)(&zghz31000, INT64_C(64));
      RECREATE(lbits)(&zghz31001);
      UNDEFINED(lbits)(&zghz31001, zghz31000);
      zACTLR_EL1 = CONVERT_OF(fbits, lbits)(zghz31001, true);
    }
    unit zgsz3461;
    zgsz3461 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3998);
      CONVERT_OF(sail_int, mach_int)(&zghz3998, INT64_C(64));
      RECREATE(lbits)(&zghz3999);
      UNDEFINED(lbits)(&zghz3999, zghz3998);
      zACTLR_EL2 = CONVERT_OF(fbits, lbits)(zghz3999, true);
    }
    unit zgsz3460;
    zgsz3460 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3996);
      CONVERT_OF(sail_int, mach_int)(&zghz3996, INT64_C(64));
      RECREATE(lbits)(&zghz3997);
      UNDEFINED(lbits)(&zghz3997, zghz3996);
      zACTLR_EL3 = CONVERT_OF(fbits, lbits)(zghz3997, true);
    }
    unit zgsz3459;
    zgsz3459 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3994);
      CONVERT_OF(sail_int, mach_int)(&zghz3994, INT64_C(64));
      RECREATE(lbits)(&zghz3995);
      UNDEFINED(lbits)(&zghz3995, zghz3994);
      zAFSR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3995, true);
    }
    unit zgsz3458;
    zgsz3458 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3992);
      CONVERT_OF(sail_int, mach_int)(&zghz3992, INT64_C(64));
      RECREATE(lbits)(&zghz3993);
      UNDEFINED(lbits)(&zghz3993, zghz3992);
      zAFSR0_EL2 = CONVERT_OF(fbits, lbits)(zghz3993, true);
    }
    unit zgsz3457;
    zgsz3457 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3990);
      CONVERT_OF(sail_int, mach_int)(&zghz3990, INT64_C(64));
      RECREATE(lbits)(&zghz3991);
      UNDEFINED(lbits)(&zghz3991, zghz3990);
      zAFSR0_EL3 = CONVERT_OF(fbits, lbits)(zghz3991, true);
    }
    unit zgsz3456;
    zgsz3456 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3988);
      CONVERT_OF(sail_int, mach_int)(&zghz3988, INT64_C(64));
      RECREATE(lbits)(&zghz3989);
      UNDEFINED(lbits)(&zghz3989, zghz3988);
      zAFSR1_EL1 = CONVERT_OF(fbits, lbits)(zghz3989, true);
    }
    unit zgsz3455;
    zgsz3455 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3986);
      CONVERT_OF(sail_int, mach_int)(&zghz3986, INT64_C(64));
      RECREATE(lbits)(&zghz3987);
      UNDEFINED(lbits)(&zghz3987, zghz3986);
      zAFSR1_EL2 = CONVERT_OF(fbits, lbits)(zghz3987, true);
    }
    unit zgsz3454;
    zgsz3454 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3984);
      CONVERT_OF(sail_int, mach_int)(&zghz3984, INT64_C(64));
      RECREATE(lbits)(&zghz3985);
      UNDEFINED(lbits)(&zghz3985, zghz3984);
      zAFSR1_EL3 = CONVERT_OF(fbits, lbits)(zghz3985, true);
    }
    unit zgsz3453;
    zgsz3453 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3982);
      CONVERT_OF(sail_int, mach_int)(&zghz3982, INT64_C(64));
      RECREATE(lbits)(&zghz3983);
      UNDEFINED(lbits)(&zghz3983, zghz3982);
      zAIDR_EL1 = CONVERT_OF(fbits, lbits)(zghz3983, true);
    }
    unit zgsz3452;
    zgsz3452 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3980);
      CONVERT_OF(sail_int, mach_int)(&zghz3980, INT64_C(64));
      RECREATE(lbits)(&zghz3981);
      UNDEFINED(lbits)(&zghz3981, zghz3980);
      zAMAIR_EL1 = CONVERT_OF(fbits, lbits)(zghz3981, true);
    }
    unit zgsz3451;
    zgsz3451 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3978);
      CONVERT_OF(sail_int, mach_int)(&zghz3978, INT64_C(64));
      RECREATE(lbits)(&zghz3979);
      UNDEFINED(lbits)(&zghz3979, zghz3978);
      zAMAIR_EL2 = CONVERT_OF(fbits, lbits)(zghz3979, true);
    }
    unit zgsz3450;
    zgsz3450 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3976);
      CONVERT_OF(sail_int, mach_int)(&zghz3976, INT64_C(64));
      RECREATE(lbits)(&zghz3977);
      UNDEFINED(lbits)(&zghz3977, zghz3976);
      zAMAIR_EL3 = CONVERT_OF(fbits, lbits)(zghz3977, true);
    }
    unit zgsz3449;
    zgsz3449 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3974);
      CONVERT_OF(sail_int, mach_int)(&zghz3974, INT64_C(32));
      RECREATE(lbits)(&zghz3975);
      UNDEFINED(lbits)(&zghz3975, zghz3974);
      zCCSIDR_EL1 = CONVERT_OF(fbits, lbits)(zghz3975, true);
    }
    unit zgsz3448;
    zgsz3448 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3972);
      CONVERT_OF(sail_int, mach_int)(&zghz3972, INT64_C(32));
      RECREATE(lbits)(&zghz3973);
      UNDEFINED(lbits)(&zghz3973, zghz3972);
      zCHCR_EL2 = CONVERT_OF(fbits, lbits)(zghz3973, true);
    }
    unit zgsz3447;
    zgsz3447 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3970);
      CONVERT_OF(sail_int, mach_int)(&zghz3970, INT64_C(64));
      RECREATE(lbits)(&zghz3971);
      UNDEFINED(lbits)(&zghz3971, zghz3970);
      zCLIDR_EL1 = CONVERT_OF(fbits, lbits)(zghz3971, true);
    }
    unit zgsz3446;
    zgsz3446 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3968);
      CONVERT_OF(sail_int, mach_int)(&zghz3968, INT64_C(32));
      RECREATE(lbits)(&zghz3969);
      UNDEFINED(lbits)(&zghz3969, zghz3968);
      zCNTFRQ_EL0 = CONVERT_OF(fbits, lbits)(zghz3969, true);
    }
    unit zgsz3445;
    zgsz3445 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3966);
      CONVERT_OF(sail_int, mach_int)(&zghz3966, INT64_C(32));
      RECREATE(lbits)(&zghz3967);
      UNDEFINED(lbits)(&zghz3967, zghz3966);
      zCNTHCTL_EL2 = CONVERT_OF(fbits, lbits)(zghz3967, true);
    }
    unit zgsz3444;
    zgsz3444 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3964);
      CONVERT_OF(sail_int, mach_int)(&zghz3964, INT64_C(32));
      RECREATE(lbits)(&zghz3965);
      UNDEFINED(lbits)(&zghz3965, zghz3964);
      zCNTKCTL_EL1 = CONVERT_OF(fbits, lbits)(zghz3965, true);
    }
    unit zgsz3443;
    zgsz3443 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3962);
      CONVERT_OF(sail_int, mach_int)(&zghz3962, INT64_C(32));
      RECREATE(lbits)(&zghz3963);
      UNDEFINED(lbits)(&zghz3963, zghz3962);
      zCNTHP_CTL_EL2 = CONVERT_OF(fbits, lbits)(zghz3963, true);
    }
    unit zgsz3442;
    zgsz3442 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3960);
      CONVERT_OF(sail_int, mach_int)(&zghz3960, INT64_C(64));
      RECREATE(lbits)(&zghz3961);
      UNDEFINED(lbits)(&zghz3961, zghz3960);
      zCNTHP_CVAL_EL2 = CONVERT_OF(fbits, lbits)(zghz3961, true);
    }
    unit zgsz3441;
    zgsz3441 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3958);
      CONVERT_OF(sail_int, mach_int)(&zghz3958, INT64_C(32));
      RECREATE(lbits)(&zghz3959);
      UNDEFINED(lbits)(&zghz3959, zghz3958);
      zCNTHP_TVAL_EL2 = CONVERT_OF(fbits, lbits)(zghz3959, true);
    }
    unit zgsz3440;
    zgsz3440 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3956);
      CONVERT_OF(sail_int, mach_int)(&zghz3956, INT64_C(32));
      RECREATE(lbits)(&zghz3957);
      UNDEFINED(lbits)(&zghz3957, zghz3956);
      zCNTHV_CTL_EL2 = CONVERT_OF(fbits, lbits)(zghz3957, true);
    }
    unit zgsz3439;
    zgsz3439 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3954);
      CONVERT_OF(sail_int, mach_int)(&zghz3954, INT64_C(64));
      RECREATE(lbits)(&zghz3955);
      UNDEFINED(lbits)(&zghz3955, zghz3954);
      zCNTHV_CVAL_EL2 = CONVERT_OF(fbits, lbits)(zghz3955, true);
    }
    unit zgsz3438;
    zgsz3438 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3952);
      CONVERT_OF(sail_int, mach_int)(&zghz3952, INT64_C(32));
      RECREATE(lbits)(&zghz3953);
      UNDEFINED(lbits)(&zghz3953, zghz3952);
      zCNTHV_TVAL_EL2 = CONVERT_OF(fbits, lbits)(zghz3953, true);
    }
    unit zgsz3437;
    zgsz3437 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3950);
      CONVERT_OF(sail_int, mach_int)(&zghz3950, INT64_C(64));
      RECREATE(lbits)(&zghz3951);
      UNDEFINED(lbits)(&zghz3951, zghz3950);
      zCNTPCT_EL0 = CONVERT_OF(fbits, lbits)(zghz3951, true);
    }
    unit zgsz3436;
    zgsz3436 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3948);
      CONVERT_OF(sail_int, mach_int)(&zghz3948, INT64_C(32));
      RECREATE(lbits)(&zghz3949);
      UNDEFINED(lbits)(&zghz3949, zghz3948);
      zCNTPS_CTL_EL1 = CONVERT_OF(fbits, lbits)(zghz3949, true);
    }
    unit zgsz3435;
    zgsz3435 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3946);
      CONVERT_OF(sail_int, mach_int)(&zghz3946, INT64_C(64));
      RECREATE(lbits)(&zghz3947);
      UNDEFINED(lbits)(&zghz3947, zghz3946);
      zCNTPS_CVAL_EL1 = CONVERT_OF(fbits, lbits)(zghz3947, true);
    }
    unit zgsz3434;
    zgsz3434 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3944);
      CONVERT_OF(sail_int, mach_int)(&zghz3944, INT64_C(32));
      RECREATE(lbits)(&zghz3945);
      UNDEFINED(lbits)(&zghz3945, zghz3944);
      zCNTPS_TVAL_EL1 = CONVERT_OF(fbits, lbits)(zghz3945, true);
    }
    unit zgsz3433;
    zgsz3433 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3942);
      CONVERT_OF(sail_int, mach_int)(&zghz3942, INT64_C(32));
      RECREATE(lbits)(&zghz3943);
      UNDEFINED(lbits)(&zghz3943, zghz3942);
      zCNTP_CTL_EL0 = CONVERT_OF(fbits, lbits)(zghz3943, true);
    }
    unit zgsz3432;
    zgsz3432 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3940);
      CONVERT_OF(sail_int, mach_int)(&zghz3940, INT64_C(64));
      RECREATE(lbits)(&zghz3941);
      UNDEFINED(lbits)(&zghz3941, zghz3940);
      zCNTP_CVAL_EL0 = CONVERT_OF(fbits, lbits)(zghz3941, true);
    }
    unit zgsz3431;
    zgsz3431 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3938);
      CONVERT_OF(sail_int, mach_int)(&zghz3938, INT64_C(32));
      RECREATE(lbits)(&zghz3939);
      UNDEFINED(lbits)(&zghz3939, zghz3938);
      zCNTP_TVAL_EL0 = CONVERT_OF(fbits, lbits)(zghz3939, true);
    }
    unit zgsz3430;
    zgsz3430 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3936);
      CONVERT_OF(sail_int, mach_int)(&zghz3936, INT64_C(64));
      RECREATE(lbits)(&zghz3937);
      UNDEFINED(lbits)(&zghz3937, zghz3936);
      zCNTVCT_EL0 = CONVERT_OF(fbits, lbits)(zghz3937, true);
    }
    unit zgsz3429;
    zgsz3429 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3934);
      CONVERT_OF(sail_int, mach_int)(&zghz3934, INT64_C(64));
      RECREATE(lbits)(&zghz3935);
      UNDEFINED(lbits)(&zghz3935, zghz3934);
      zCNTVOFF_EL2 = CONVERT_OF(fbits, lbits)(zghz3935, true);
    }
    unit zgsz3428;
    zgsz3428 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3932);
      CONVERT_OF(sail_int, mach_int)(&zghz3932, INT64_C(32));
      RECREATE(lbits)(&zghz3933);
      UNDEFINED(lbits)(&zghz3933, zghz3932);
      zCNTV_CTL_EL0 = CONVERT_OF(fbits, lbits)(zghz3933, true);
    }
    unit zgsz3427;
    zgsz3427 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3930);
      CONVERT_OF(sail_int, mach_int)(&zghz3930, INT64_C(64));
      RECREATE(lbits)(&zghz3931);
      UNDEFINED(lbits)(&zghz3931, zghz3930);
      zCNTV_CVAL_EL0 = CONVERT_OF(fbits, lbits)(zghz3931, true);
    }
    unit zgsz3426;
    zgsz3426 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3928);
      CONVERT_OF(sail_int, mach_int)(&zghz3928, INT64_C(32));
      RECREATE(lbits)(&zghz3929);
      UNDEFINED(lbits)(&zghz3929, zghz3928);
      zCNTV_TVAL_EL0 = CONVERT_OF(fbits, lbits)(zghz3929, true);
    }
    unit zgsz3425;
    zgsz3425 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3926);
      CONVERT_OF(sail_int, mach_int)(&zghz3926, INT64_C(32));
      RECREATE(lbits)(&zghz3927);
      UNDEFINED(lbits)(&zghz3927, zghz3926);
      zCONTEXTIDR_EL1 = CONVERT_OF(fbits, lbits)(zghz3927, true);
    }
    unit zgsz3424;
    zgsz3424 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3924);
      CONVERT_OF(sail_int, mach_int)(&zghz3924, INT64_C(32));
      RECREATE(lbits)(&zghz3925);
      UNDEFINED(lbits)(&zghz3925, zghz3924);
      zCONTEXTIDR_EL2 = CONVERT_OF(fbits, lbits)(zghz3925, true);
    }
    unit zgsz3423;
    zgsz3423 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3922);
      CONVERT_OF(sail_int, mach_int)(&zghz3922, INT64_C(32));
      RECREATE(lbits)(&zghz3923);
      UNDEFINED(lbits)(&zghz3923, zghz3922);
      zCSCR_EL3 = CONVERT_OF(fbits, lbits)(zghz3923, true);
    }
    unit zgsz3422;
    zgsz3422 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3920);
      CONVERT_OF(sail_int, mach_int)(&zghz3920, INT64_C(32));
      RECREATE(lbits)(&zghz3921);
      UNDEFINED(lbits)(&zghz3921, zghz3920);
      zCSSELR_EL1 = CONVERT_OF(fbits, lbits)(zghz3921, true);
    }
    unit zgsz3421;
    zgsz3421 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3918);
      CONVERT_OF(sail_int, mach_int)(&zghz3918, INT64_C(32));
      RECREATE(lbits)(&zghz3919);
      UNDEFINED(lbits)(&zghz3919, zghz3918);
      zCTR_EL0 = CONVERT_OF(fbits, lbits)(zghz3919, true);
    }
    unit zgsz3420;
    zgsz3420 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3916);
      CONVERT_OF(sail_int, mach_int)(&zghz3916, INT64_C(32));
      RECREATE(lbits)(&zghz3917);
      UNDEFINED(lbits)(&zghz3917, zghz3916);
      zDACR32_EL2 = CONVERT_OF(fbits, lbits)(zghz3917, true);
    }
    unit zgsz3419;
    zgsz3419 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3914);
      CONVERT_OF(sail_int, mach_int)(&zghz3914, INT64_C(32));
      RECREATE(lbits)(&zghz3915);
      UNDEFINED(lbits)(&zghz3915, zghz3914);
      zDBGAUTHSTATUS_EL1 = CONVERT_OF(fbits, lbits)(zghz3915, true);
    }
    unit zgsz3418;
    zgsz3418 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3912);
      CONVERT_OF(sail_int, mach_int)(&zghz3912, INT64_C(32));
      RECREATE(lbits)(&zghz3913);
      UNDEFINED(lbits)(&zghz3913, zghz3912);
      zMDCR_EL2 = CONVERT_OF(fbits, lbits)(zghz3913, true);
    }
    unit zgsz3417;
    zgsz3417 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3910);
      CONVERT_OF(sail_int, mach_int)(&zghz3910, INT64_C(32));
      RECREATE(lbits)(&zghz3911);
      UNDEFINED(lbits)(&zghz3911, zghz3910);
      zMDCR_EL3 = CONVERT_OF(fbits, lbits)(zghz3911, true);
    }
    unit zgsz3416;
    zgsz3416 = UNIT;
  }
  {
    uint64_t zgaz3264;
    {
      RECREATE(sail_int)(&zghz3908);
      CONVERT_OF(sail_int, mach_int)(&zghz3908, INT64_C(32));
      RECREATE(lbits)(&zghz3909);
      UNDEFINED(lbits)(&zghz3909, zghz3908);
      zgaz3264 = CONVERT_OF(fbits, lbits)(zghz3909, true);
    }
    {
      RECREATE(sail_int)(&zghz3907);
      CONVERT_OF(sail_int, mach_int)(&zghz3907, INT64_C(16));
      UNDEFINED(vector_zz5vecz8z5bv32z9)(&zDBGBCR_EL1, zghz3907, zgaz3264);
    }
    unit zgsz3415;
    zgsz3415 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3906);
      CONVERT_OF(sail_int, mach_int)(&zghz3906, INT64_C(129));
      UNDEFINED(lbits)(&zCDLR_EL0, zghz3906);
    }
    unit zgsz3414;
    zgsz3414 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3904);
      CONVERT_OF(sail_int, mach_int)(&zghz3904, INT64_C(32));
      RECREATE(lbits)(&zghz3905);
      UNDEFINED(lbits)(&zghz3905, zghz3904);
      zDSPSR_EL0 = CONVERT_OF(fbits, lbits)(zghz3905, true);
    }
    unit zgsz3413;
    zgsz3413 = UNIT;
  }
  {
    zDBGEN = sailgen_undefined_signal(UNIT);
    unit zgsz3412;
    zgsz3412 = UNIT;
  }
  {
    zSPIDEN = sailgen_undefined_signal(UNIT);
    unit zgsz3411;
    zgsz3411 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3902);
      CONVERT_OF(sail_int, mach_int)(&zghz3902, INT64_C(32));
      RECREATE(lbits)(&zghz3903);
      UNDEFINED(lbits)(&zghz3903, zghz3902);
      zDBGPRCR_EL1 = CONVERT_OF(fbits, lbits)(zghz3903, true);
    }
    unit zgsz3410;
    zgsz3410 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3900);
      CONVERT_OF(sail_int, mach_int)(&zghz3900, INT64_C(32));
      RECREATE(lbits)(&zghz3901);
      UNDEFINED(lbits)(&zghz3901, zghz3900);
      zOSDLR_EL1 = CONVERT_OF(fbits, lbits)(zghz3901, true);
    }
    unit zgsz3409;
    zgsz3409 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3898);
      CONVERT_OF(sail_int, mach_int)(&zghz3898, INT64_C(32));
      RECREATE(lbits)(&zghz3899);
      UNDEFINED(lbits)(&zghz3899, zghz3898);
      zOSLSR_EL1 = CONVERT_OF(fbits, lbits)(zghz3899, true);
    }
    unit zgsz3408;
    zgsz3408 = UNIT;
  }
  {
    uint64_t zgaz3265;
    {
      RECREATE(sail_int)(&zghz3896);
      CONVERT_OF(sail_int, mach_int)(&zghz3896, INT64_C(64));
      RECREATE(lbits)(&zghz3897);
      UNDEFINED(lbits)(&zghz3897, zghz3896);
      zgaz3265 = CONVERT_OF(fbits, lbits)(zghz3897, true);
    }
    {
      RECREATE(sail_int)(&zghz3895);
      CONVERT_OF(sail_int, mach_int)(&zghz3895, INT64_C(16));
      UNDEFINED(vector_zz5vecz8z5bv64z9)(&zDBGBVR_EL1, zghz3895, zgaz3265);
    }
    unit zgsz3407;
    zgsz3407 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3893);
      CONVERT_OF(sail_int, mach_int)(&zghz3893, INT64_C(32));
      RECREATE(lbits)(&zghz3894);
      UNDEFINED(lbits)(&zghz3894, zghz3893);
      zDBGCLAIMCLR_EL1 = CONVERT_OF(fbits, lbits)(zghz3894, true);
    }
    unit zgsz3406;
    zgsz3406 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3891);
      CONVERT_OF(sail_int, mach_int)(&zghz3891, INT64_C(32));
      RECREATE(lbits)(&zghz3892);
      UNDEFINED(lbits)(&zghz3892, zghz3891);
      zDBGCLAIMSET_EL1 = CONVERT_OF(fbits, lbits)(zghz3892, true);
    }
    unit zgsz3405;
    zgsz3405 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3889);
      CONVERT_OF(sail_int, mach_int)(&zghz3889, INT64_C(32));
      RECREATE(lbits)(&zghz3890);
      UNDEFINED(lbits)(&zghz3890, zghz3889);
      zDBGDTRRX_EL0 = CONVERT_OF(fbits, lbits)(zghz3890, true);
    }
    unit zgsz3404;
    zgsz3404 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3887);
      CONVERT_OF(sail_int, mach_int)(&zghz3887, INT64_C(32));
      RECREATE(lbits)(&zghz3888);
      UNDEFINED(lbits)(&zghz3888, zghz3887);
      zMDSCR_EL1 = CONVERT_OF(fbits, lbits)(zghz3888, true);
    }
    unit zgsz3403;
    zgsz3403 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3886);
      CONVERT_OF(sail_int, mach_int)(&zghz3886, INT64_C(129));
      UNDEFINED(lbits)(&zCDBGDTR_EL0, zghz3886);
    }
    unit zgsz3402;
    zgsz3402 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3884);
      CONVERT_OF(sail_int, mach_int)(&zghz3884, INT64_C(32));
      RECREATE(lbits)(&zghz3885);
      UNDEFINED(lbits)(&zghz3885, zghz3884);
      zDBGVCR32_EL2 = CONVERT_OF(fbits, lbits)(zghz3885, true);
    }
    unit zgsz3401;
    zgsz3401 = UNIT;
  }
  {
    uint64_t zgaz3266;
    {
      RECREATE(sail_int)(&zghz3882);
      CONVERT_OF(sail_int, mach_int)(&zghz3882, INT64_C(32));
      RECREATE(lbits)(&zghz3883);
      UNDEFINED(lbits)(&zghz3883, zghz3882);
      zgaz3266 = CONVERT_OF(fbits, lbits)(zghz3883, true);
    }
    {
      RECREATE(sail_int)(&zghz3881);
      CONVERT_OF(sail_int, mach_int)(&zghz3881, INT64_C(16));
      UNDEFINED(vector_zz5vecz8z5bv32z9)(&zDBGWCR_EL1, zghz3881, zgaz3266);
    }
    unit zgsz3400;
    zgsz3400 = UNIT;
  }
  {
    uint64_t zgaz3267;
    {
      RECREATE(sail_int)(&zghz3879);
      CONVERT_OF(sail_int, mach_int)(&zghz3879, INT64_C(64));
      RECREATE(lbits)(&zghz3880);
      UNDEFINED(lbits)(&zghz3880, zghz3879);
      zgaz3267 = CONVERT_OF(fbits, lbits)(zghz3880, true);
    }
    {
      RECREATE(sail_int)(&zghz3878);
      CONVERT_OF(sail_int, mach_int)(&zghz3878, INT64_C(16));
      UNDEFINED(vector_zz5vecz8z5bv64z9)(&zDBGWVR_EL1, zghz3878, zgaz3267);
    }
    unit zgsz3399;
    zgsz3399 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3876);
      CONVERT_OF(sail_int, mach_int)(&zghz3876, INT64_C(32));
      RECREATE(lbits)(&zghz3877);
      UNDEFINED(lbits)(&zghz3877, zghz3876);
      zDCZID_EL0 = CONVERT_OF(fbits, lbits)(zghz3877, true);
    }
    unit zgsz3398;
    zgsz3398 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3874);
      CONVERT_OF(sail_int, mach_int)(&zghz3874, INT64_C(32));
      RECREATE(lbits)(&zghz3875);
      UNDEFINED(lbits)(&zghz3875, zghz3874);
      zDISR_EL1 = CONVERT_OF(fbits, lbits)(zghz3875, true);
    }
    unit zgsz3397;
    zgsz3397 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3872);
      CONVERT_OF(sail_int, mach_int)(&zghz3872, INT64_C(32));
      RECREATE(lbits)(&zghz3873);
      UNDEFINED(lbits)(&zghz3873, zghz3872);
      zVDISR_EL2 = CONVERT_OF(fbits, lbits)(zghz3873, true);
    }
    unit zgsz3396;
    zgsz3396 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3870);
      CONVERT_OF(sail_int, mach_int)(&zghz3870, INT64_C(32));
      RECREATE(lbits)(&zghz3871);
      UNDEFINED(lbits)(&zghz3871, zghz3870);
      zERRIDR_EL1 = CONVERT_OF(fbits, lbits)(zghz3871, true);
    }
    unit zgsz3395;
    zgsz3395 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3868);
      CONVERT_OF(sail_int, mach_int)(&zghz3868, INT64_C(32));
      RECREATE(lbits)(&zghz3869);
      UNDEFINED(lbits)(&zghz3869, zghz3868);
      zERRSELR_EL1 = CONVERT_OF(fbits, lbits)(zghz3869, true);
    }
    unit zgsz3394;
    zgsz3394 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3866);
      CONVERT_OF(sail_int, mach_int)(&zghz3866, INT64_C(64));
      RECREATE(lbits)(&zghz3867);
      UNDEFINED(lbits)(&zghz3867, zghz3866);
      zERXADDR_EL1 = CONVERT_OF(fbits, lbits)(zghz3867, true);
    }
    unit zgsz3393;
    zgsz3393 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3864);
      CONVERT_OF(sail_int, mach_int)(&zghz3864, INT64_C(64));
      RECREATE(lbits)(&zghz3865);
      UNDEFINED(lbits)(&zghz3865, zghz3864);
      zERXCTLR_EL1 = CONVERT_OF(fbits, lbits)(zghz3865, true);
    }
    unit zgsz3392;
    zgsz3392 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3862);
      CONVERT_OF(sail_int, mach_int)(&zghz3862, INT64_C(64));
      RECREATE(lbits)(&zghz3863);
      UNDEFINED(lbits)(&zghz3863, zghz3862);
      zERXFR_EL1 = CONVERT_OF(fbits, lbits)(zghz3863, true);
    }
    unit zgsz3391;
    zgsz3391 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3860);
      CONVERT_OF(sail_int, mach_int)(&zghz3860, INT64_C(64));
      RECREATE(lbits)(&zghz3861);
      UNDEFINED(lbits)(&zghz3861, zghz3860);
      zERXMISC0_EL1 = CONVERT_OF(fbits, lbits)(zghz3861, true);
    }
    unit zgsz3390;
    zgsz3390 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3858);
      CONVERT_OF(sail_int, mach_int)(&zghz3858, INT64_C(64));
      RECREATE(lbits)(&zghz3859);
      UNDEFINED(lbits)(&zghz3859, zghz3858);
      zERXMISC1_EL1 = CONVERT_OF(fbits, lbits)(zghz3859, true);
    }
    unit zgsz3389;
    zgsz3389 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3856);
      CONVERT_OF(sail_int, mach_int)(&zghz3856, INT64_C(64));
      RECREATE(lbits)(&zghz3857);
      UNDEFINED(lbits)(&zghz3857, zghz3856);
      zERXSTATUS_EL1 = CONVERT_OF(fbits, lbits)(zghz3857, true);
    }
    unit zgsz3388;
    zgsz3388 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3854);
      CONVERT_OF(sail_int, mach_int)(&zghz3854, INT64_C(32));
      RECREATE(lbits)(&zghz3855);
      UNDEFINED(lbits)(&zghz3855, zghz3854);
      zFPCR = CONVERT_OF(fbits, lbits)(zghz3855, true);
    }
    unit zgsz3387;
    zgsz3387 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3852);
      CONVERT_OF(sail_int, mach_int)(&zghz3852, INT64_C(32));
      RECREATE(lbits)(&zghz3853);
      UNDEFINED(lbits)(&zghz3853, zghz3852);
      zFPEXC32_EL2 = CONVERT_OF(fbits, lbits)(zghz3853, true);
    }
    unit zgsz3386;
    zgsz3386 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3850);
      CONVERT_OF(sail_int, mach_int)(&zghz3850, INT64_C(32));
      RECREATE(lbits)(&zghz3851);
      UNDEFINED(lbits)(&zghz3851, zghz3850);
      zFPSR = CONVERT_OF(fbits, lbits)(zghz3851, true);
    }
    unit zgsz3385;
    zgsz3385 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3848);
      CONVERT_OF(sail_int, mach_int)(&zghz3848, INT64_C(64));
      RECREATE(lbits)(&zghz3849);
      UNDEFINED(lbits)(&zghz3849, zghz3848);
      zHACR_EL2 = CONVERT_OF(fbits, lbits)(zghz3849, true);
    }
    unit zgsz3384;
    zgsz3384 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3846);
      CONVERT_OF(sail_int, mach_int)(&zghz3846, INT64_C(32));
      RECREATE(lbits)(&zghz3847);
      UNDEFINED(lbits)(&zghz3847, zghz3846);
      zHSTR_EL2 = CONVERT_OF(fbits, lbits)(zghz3847, true);
    }
    unit zgsz3383;
    zgsz3383 = UNIT;
  }
  {
    uint64_t zgaz3268;
    {
      RECREATE(sail_int)(&zghz3844);
      CONVERT_OF(sail_int, mach_int)(&zghz3844, INT64_C(32));
      RECREATE(lbits)(&zghz3845);
      UNDEFINED(lbits)(&zghz3845, zghz3844);
      zgaz3268 = CONVERT_OF(fbits, lbits)(zghz3845, true);
    }
    {
      RECREATE(sail_int)(&zghz3843);
      CONVERT_OF(sail_int, mach_int)(&zghz3843, INT64_C(4));
      UNDEFINED(vector_zz5vecz8z5bv32z9)(&zICC_AP0R_EL1, zghz3843, zgaz3268);
    }
    unit zgsz3382;
    zgsz3382 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3841);
      CONVERT_OF(sail_int, mach_int)(&zghz3841, INT64_C(32));
      RECREATE(lbits)(&zghz3842);
      UNDEFINED(lbits)(&zghz3842, zghz3841);
      zICC_SRE_EL1_NS = CONVERT_OF(fbits, lbits)(zghz3842, true);
    }
    unit zgsz3381;
    zgsz3381 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3839);
      CONVERT_OF(sail_int, mach_int)(&zghz3839, INT64_C(32));
      RECREATE(lbits)(&zghz3840);
      UNDEFINED(lbits)(&zghz3840, zghz3839);
      zICC_SRE_EL1_S = CONVERT_OF(fbits, lbits)(zghz3840, true);
    }
    unit zgsz3380;
    zgsz3380 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3837);
      CONVERT_OF(sail_int, mach_int)(&zghz3837, INT64_C(32));
      RECREATE(lbits)(&zghz3838);
      UNDEFINED(lbits)(&zghz3838, zghz3837);
      zICC_SRE_EL2 = CONVERT_OF(fbits, lbits)(zghz3838, true);
    }
    unit zgsz3379;
    zgsz3379 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3835);
      CONVERT_OF(sail_int, mach_int)(&zghz3835, INT64_C(32));
      RECREATE(lbits)(&zghz3836);
      UNDEFINED(lbits)(&zghz3836, zghz3835);
      zICC_SRE_EL3 = CONVERT_OF(fbits, lbits)(zghz3836, true);
    }
    unit zgsz3378;
    zgsz3378 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3833);
      CONVERT_OF(sail_int, mach_int)(&zghz3833, INT64_C(32));
      RECREATE(lbits)(&zghz3834);
      UNDEFINED(lbits)(&zghz3834, zghz3833);
      zICH_HCR_EL2 = CONVERT_OF(fbits, lbits)(zghz3834, true);
    }
    unit zgsz3377;
    zgsz3377 = UNIT;
  }
  {
    uint64_t zgaz3269;
    {
      RECREATE(sail_int)(&zghz3831);
      CONVERT_OF(sail_int, mach_int)(&zghz3831, INT64_C(32));
      RECREATE(lbits)(&zghz3832);
      UNDEFINED(lbits)(&zghz3832, zghz3831);
      zgaz3269 = CONVERT_OF(fbits, lbits)(zghz3832, true);
    }
    {
      RECREATE(sail_int)(&zghz3830);
      CONVERT_OF(sail_int, mach_int)(&zghz3830, INT64_C(4));
      UNDEFINED(vector_zz5vecz8z5bv32z9)(&zICV_AP0R_EL1, zghz3830, zgaz3269);
    }
    unit zgsz3376;
    zgsz3376 = UNIT;
  }
  {
    uint64_t zgaz3270;
    {
      RECREATE(sail_int)(&zghz3828);
      CONVERT_OF(sail_int, mach_int)(&zghz3828, INT64_C(32));
      RECREATE(lbits)(&zghz3829);
      UNDEFINED(lbits)(&zghz3829, zghz3828);
      zgaz3270 = CONVERT_OF(fbits, lbits)(zghz3829, true);
    }
    {
      RECREATE(sail_int)(&zghz3827);
      CONVERT_OF(sail_int, mach_int)(&zghz3827, INT64_C(4));
      UNDEFINED(vector_zz5vecz8z5bv32z9)(&zICC_AP1R_EL1, zghz3827, zgaz3270);
    }
    unit zgsz3375;
    zgsz3375 = UNIT;
  }
  {
    uint64_t zgaz3271;
    {
      RECREATE(sail_int)(&zghz3825);
      CONVERT_OF(sail_int, mach_int)(&zghz3825, INT64_C(32));
      RECREATE(lbits)(&zghz3826);
      UNDEFINED(lbits)(&zghz3826, zghz3825);
      zgaz3271 = CONVERT_OF(fbits, lbits)(zghz3826, true);
    }
    {
      RECREATE(sail_int)(&zghz3824);
      CONVERT_OF(sail_int, mach_int)(&zghz3824, INT64_C(4));
      UNDEFINED(vector_zz5vecz8z5bv32z9)(&zICC_AP1R_EL1_NS, zghz3824, zgaz3271);
    }
    unit zgsz3374;
    zgsz3374 = UNIT;
  }
  {
    uint64_t zgaz3272;
    {
      RECREATE(sail_int)(&zghz3822);
      CONVERT_OF(sail_int, mach_int)(&zghz3822, INT64_C(32));
      RECREATE(lbits)(&zghz3823);
      UNDEFINED(lbits)(&zghz3823, zghz3822);
      zgaz3272 = CONVERT_OF(fbits, lbits)(zghz3823, true);
    }
    {
      RECREATE(sail_int)(&zghz3821);
      CONVERT_OF(sail_int, mach_int)(&zghz3821, INT64_C(4));
      UNDEFINED(vector_zz5vecz8z5bv32z9)(&zICC_AP1R_EL1_S, zghz3821, zgaz3272);
    }
    unit zgsz3373;
    zgsz3373 = UNIT;
  }
  {
    uint64_t zgaz3273;
    {
      RECREATE(sail_int)(&zghz3819);
      CONVERT_OF(sail_int, mach_int)(&zghz3819, INT64_C(32));
      RECREATE(lbits)(&zghz3820);
      UNDEFINED(lbits)(&zghz3820, zghz3819);
      zgaz3273 = CONVERT_OF(fbits, lbits)(zghz3820, true);
    }
    {
      RECREATE(sail_int)(&zghz3818);
      CONVERT_OF(sail_int, mach_int)(&zghz3818, INT64_C(4));
      UNDEFINED(vector_zz5vecz8z5bv32z9)(&zICV_AP1R_EL1, zghz3818, zgaz3273);
    }
    unit zgsz3372;
    zgsz3372 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3816);
      CONVERT_OF(sail_int, mach_int)(&zghz3816, INT64_C(32));
      RECREATE(lbits)(&zghz3817);
      UNDEFINED(lbits)(&zghz3817, zghz3816);
      zICC_BPR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3817, true);
    }
    unit zgsz3371;
    zgsz3371 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3814);
      CONVERT_OF(sail_int, mach_int)(&zghz3814, INT64_C(32));
      RECREATE(lbits)(&zghz3815);
      UNDEFINED(lbits)(&zghz3815, zghz3814);
      zICV_BPR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3815, true);
    }
    unit zgsz3370;
    zgsz3370 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3812);
      CONVERT_OF(sail_int, mach_int)(&zghz3812, INT64_C(32));
      RECREATE(lbits)(&zghz3813);
      UNDEFINED(lbits)(&zghz3813, zghz3812);
      zICC_BPR1_EL1_NS = CONVERT_OF(fbits, lbits)(zghz3813, true);
    }
    unit zgsz3369;
    zgsz3369 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3810);
      CONVERT_OF(sail_int, mach_int)(&zghz3810, INT64_C(32));
      RECREATE(lbits)(&zghz3811);
      UNDEFINED(lbits)(&zghz3811, zghz3810);
      zICC_BPR1_EL1_S = CONVERT_OF(fbits, lbits)(zghz3811, true);
    }
    unit zgsz3368;
    zgsz3368 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3808);
      CONVERT_OF(sail_int, mach_int)(&zghz3808, INT64_C(32));
      RECREATE(lbits)(&zghz3809);
      UNDEFINED(lbits)(&zghz3809, zghz3808);
      zICV_BPR1_EL1 = CONVERT_OF(fbits, lbits)(zghz3809, true);
    }
    unit zgsz3367;
    zgsz3367 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3806);
      CONVERT_OF(sail_int, mach_int)(&zghz3806, INT64_C(32));
      RECREATE(lbits)(&zghz3807);
      UNDEFINED(lbits)(&zghz3807, zghz3806);
      zICC_CTLR_EL1_NS = CONVERT_OF(fbits, lbits)(zghz3807, true);
    }
    unit zgsz3366;
    zgsz3366 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3804);
      CONVERT_OF(sail_int, mach_int)(&zghz3804, INT64_C(32));
      RECREATE(lbits)(&zghz3805);
      UNDEFINED(lbits)(&zghz3805, zghz3804);
      zICC_CTLR_EL1_S = CONVERT_OF(fbits, lbits)(zghz3805, true);
    }
    unit zgsz3365;
    zgsz3365 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3802);
      CONVERT_OF(sail_int, mach_int)(&zghz3802, INT64_C(32));
      RECREATE(lbits)(&zghz3803);
      UNDEFINED(lbits)(&zghz3803, zghz3802);
      zICV_CTLR_EL1 = CONVERT_OF(fbits, lbits)(zghz3803, true);
    }
    unit zgsz3364;
    zgsz3364 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3800);
      CONVERT_OF(sail_int, mach_int)(&zghz3800, INT64_C(32));
      RECREATE(lbits)(&zghz3801);
      UNDEFINED(lbits)(&zghz3801, zghz3800);
      zICC_CTLR_EL3 = CONVERT_OF(fbits, lbits)(zghz3801, true);
    }
    unit zgsz3363;
    zgsz3363 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3798);
      CONVERT_OF(sail_int, mach_int)(&zghz3798, INT64_C(32));
      RECREATE(lbits)(&zghz3799);
      UNDEFINED(lbits)(&zghz3799, zghz3798);
      zICC_HPPIR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3799, true);
    }
    unit zgsz3362;
    zgsz3362 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3796);
      CONVERT_OF(sail_int, mach_int)(&zghz3796, INT64_C(32));
      RECREATE(lbits)(&zghz3797);
      UNDEFINED(lbits)(&zghz3797, zghz3796);
      zICV_HPPIR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3797, true);
    }
    unit zgsz3361;
    zgsz3361 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3794);
      CONVERT_OF(sail_int, mach_int)(&zghz3794, INT64_C(32));
      RECREATE(lbits)(&zghz3795);
      UNDEFINED(lbits)(&zghz3795, zghz3794);
      zICC_HPPIR1_EL1 = CONVERT_OF(fbits, lbits)(zghz3795, true);
    }
    unit zgsz3360;
    zgsz3360 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3792);
      CONVERT_OF(sail_int, mach_int)(&zghz3792, INT64_C(32));
      RECREATE(lbits)(&zghz3793);
      UNDEFINED(lbits)(&zghz3793, zghz3792);
      zICV_HPPIR1_EL1 = CONVERT_OF(fbits, lbits)(zghz3793, true);
    }
    unit zgsz3359;
    zgsz3359 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3790);
      CONVERT_OF(sail_int, mach_int)(&zghz3790, INT64_C(32));
      RECREATE(lbits)(&zghz3791);
      UNDEFINED(lbits)(&zghz3791, zghz3790);
      zICC_IAR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3791, true);
    }
    unit zgsz3358;
    zgsz3358 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3788);
      CONVERT_OF(sail_int, mach_int)(&zghz3788, INT64_C(32));
      RECREATE(lbits)(&zghz3789);
      UNDEFINED(lbits)(&zghz3789, zghz3788);
      zICV_IAR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3789, true);
    }
    unit zgsz3357;
    zgsz3357 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3786);
      CONVERT_OF(sail_int, mach_int)(&zghz3786, INT64_C(32));
      RECREATE(lbits)(&zghz3787);
      UNDEFINED(lbits)(&zghz3787, zghz3786);
      zICC_IAR1_EL1 = CONVERT_OF(fbits, lbits)(zghz3787, true);
    }
    unit zgsz3356;
    zgsz3356 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3784);
      CONVERT_OF(sail_int, mach_int)(&zghz3784, INT64_C(32));
      RECREATE(lbits)(&zghz3785);
      UNDEFINED(lbits)(&zghz3785, zghz3784);
      zICV_IAR1_EL1 = CONVERT_OF(fbits, lbits)(zghz3785, true);
    }
    unit zgsz3355;
    zgsz3355 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3782);
      CONVERT_OF(sail_int, mach_int)(&zghz3782, INT64_C(32));
      RECREATE(lbits)(&zghz3783);
      UNDEFINED(lbits)(&zghz3783, zghz3782);
      zICC_IGRPEN0_EL1 = CONVERT_OF(fbits, lbits)(zghz3783, true);
    }
    unit zgsz3354;
    zgsz3354 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3780);
      CONVERT_OF(sail_int, mach_int)(&zghz3780, INT64_C(32));
      RECREATE(lbits)(&zghz3781);
      UNDEFINED(lbits)(&zghz3781, zghz3780);
      zICV_IGRPEN0_EL1 = CONVERT_OF(fbits, lbits)(zghz3781, true);
    }
    unit zgsz3353;
    zgsz3353 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3778);
      CONVERT_OF(sail_int, mach_int)(&zghz3778, INT64_C(32));
      RECREATE(lbits)(&zghz3779);
      UNDEFINED(lbits)(&zghz3779, zghz3778);
      zICC_IGRPEN1_EL1_NS = CONVERT_OF(fbits, lbits)(zghz3779, true);
    }
    unit zgsz3352;
    zgsz3352 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3776);
      CONVERT_OF(sail_int, mach_int)(&zghz3776, INT64_C(32));
      RECREATE(lbits)(&zghz3777);
      UNDEFINED(lbits)(&zghz3777, zghz3776);
      zICC_IGRPEN1_EL1_S = CONVERT_OF(fbits, lbits)(zghz3777, true);
    }
    unit zgsz3351;
    zgsz3351 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3774);
      CONVERT_OF(sail_int, mach_int)(&zghz3774, INT64_C(32));
      RECREATE(lbits)(&zghz3775);
      UNDEFINED(lbits)(&zghz3775, zghz3774);
      zICV_IGRPEN1_EL1 = CONVERT_OF(fbits, lbits)(zghz3775, true);
    }
    unit zgsz3350;
    zgsz3350 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3772);
      CONVERT_OF(sail_int, mach_int)(&zghz3772, INT64_C(32));
      RECREATE(lbits)(&zghz3773);
      UNDEFINED(lbits)(&zghz3773, zghz3772);
      zICC_IGRPEN1_EL3 = CONVERT_OF(fbits, lbits)(zghz3773, true);
    }
    unit zgsz3349;
    zgsz3349 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3770);
      CONVERT_OF(sail_int, mach_int)(&zghz3770, INT64_C(32));
      RECREATE(lbits)(&zghz3771);
      UNDEFINED(lbits)(&zghz3771, zghz3770);
      zICC_PMR_EL1 = CONVERT_OF(fbits, lbits)(zghz3771, true);
    }
    unit zgsz3348;
    zgsz3348 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3768);
      CONVERT_OF(sail_int, mach_int)(&zghz3768, INT64_C(32));
      RECREATE(lbits)(&zghz3769);
      UNDEFINED(lbits)(&zghz3769, zghz3768);
      zICV_PMR_EL1 = CONVERT_OF(fbits, lbits)(zghz3769, true);
    }
    unit zgsz3347;
    zgsz3347 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3766);
      CONVERT_OF(sail_int, mach_int)(&zghz3766, INT64_C(32));
      RECREATE(lbits)(&zghz3767);
      UNDEFINED(lbits)(&zghz3767, zghz3766);
      zICC_RPR_EL1 = CONVERT_OF(fbits, lbits)(zghz3767, true);
    }
    unit zgsz3346;
    zgsz3346 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3764);
      CONVERT_OF(sail_int, mach_int)(&zghz3764, INT64_C(32));
      RECREATE(lbits)(&zghz3765);
      UNDEFINED(lbits)(&zghz3765, zghz3764);
      zICV_RPR_EL1 = CONVERT_OF(fbits, lbits)(zghz3765, true);
    }
    unit zgsz3345;
    zgsz3345 = UNIT;
  }
  {
    uint64_t zgaz3274;
    {
      RECREATE(sail_int)(&zghz3762);
      CONVERT_OF(sail_int, mach_int)(&zghz3762, INT64_C(32));
      RECREATE(lbits)(&zghz3763);
      UNDEFINED(lbits)(&zghz3763, zghz3762);
      zgaz3274 = CONVERT_OF(fbits, lbits)(zghz3763, true);
    }
    {
      RECREATE(sail_int)(&zghz3761);
      CONVERT_OF(sail_int, mach_int)(&zghz3761, INT64_C(4));
      UNDEFINED(vector_zz5vecz8z5bv32z9)(&zICH_AP0R_EL2, zghz3761, zgaz3274);
    }
    unit zgsz3344;
    zgsz3344 = UNIT;
  }
  {
    uint64_t zgaz3275;
    {
      RECREATE(sail_int)(&zghz3759);
      CONVERT_OF(sail_int, mach_int)(&zghz3759, INT64_C(32));
      RECREATE(lbits)(&zghz3760);
      UNDEFINED(lbits)(&zghz3760, zghz3759);
      zgaz3275 = CONVERT_OF(fbits, lbits)(zghz3760, true);
    }
    {
      RECREATE(sail_int)(&zghz3758);
      CONVERT_OF(sail_int, mach_int)(&zghz3758, INT64_C(4));
      UNDEFINED(vector_zz5vecz8z5bv32z9)(&zICH_AP1R_EL2, zghz3758, zgaz3275);
    }
    unit zgsz3343;
    zgsz3343 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3756);
      CONVERT_OF(sail_int, mach_int)(&zghz3756, INT64_C(32));
      RECREATE(lbits)(&zghz3757);
      UNDEFINED(lbits)(&zghz3757, zghz3756);
      zICH_EISR_EL2 = CONVERT_OF(fbits, lbits)(zghz3757, true);
    }
    unit zgsz3342;
    zgsz3342 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3754);
      CONVERT_OF(sail_int, mach_int)(&zghz3754, INT64_C(32));
      RECREATE(lbits)(&zghz3755);
      UNDEFINED(lbits)(&zghz3755, zghz3754);
      zICH_ELRSR_EL2 = CONVERT_OF(fbits, lbits)(zghz3755, true);
    }
    unit zgsz3341;
    zgsz3341 = UNIT;
  }
  {
    uint64_t zgaz3276;
    {
      RECREATE(sail_int)(&zghz3752);
      CONVERT_OF(sail_int, mach_int)(&zghz3752, INT64_C(64));
      RECREATE(lbits)(&zghz3753);
      UNDEFINED(lbits)(&zghz3753, zghz3752);
      zgaz3276 = CONVERT_OF(fbits, lbits)(zghz3753, true);
    }
    {
      RECREATE(sail_int)(&zghz3751);
      CONVERT_OF(sail_int, mach_int)(&zghz3751, INT64_C(16));
      UNDEFINED(vector_zz5vecz8z5bv64z9)(&zICH_LR_EL2, zghz3751, zgaz3276);
    }
    unit zgsz3340;
    zgsz3340 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3749);
      CONVERT_OF(sail_int, mach_int)(&zghz3749, INT64_C(32));
      RECREATE(lbits)(&zghz3750);
      UNDEFINED(lbits)(&zghz3750, zghz3749);
      zICH_MISR_EL2 = CONVERT_OF(fbits, lbits)(zghz3750, true);
    }
    unit zgsz3339;
    zgsz3339 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3747);
      CONVERT_OF(sail_int, mach_int)(&zghz3747, INT64_C(32));
      RECREATE(lbits)(&zghz3748);
      UNDEFINED(lbits)(&zghz3748, zghz3747);
      zICH_VMCR_EL2 = CONVERT_OF(fbits, lbits)(zghz3748, true);
    }
    unit zgsz3338;
    zgsz3338 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3745);
      CONVERT_OF(sail_int, mach_int)(&zghz3745, INT64_C(32));
      RECREATE(lbits)(&zghz3746);
      UNDEFINED(lbits)(&zghz3746, zghz3745);
      zICH_VTR_EL2 = CONVERT_OF(fbits, lbits)(zghz3746, true);
    }
    unit zgsz3337;
    zgsz3337 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3743);
      CONVERT_OF(sail_int, mach_int)(&zghz3743, INT64_C(32));
      RECREATE(lbits)(&zghz3744);
      UNDEFINED(lbits)(&zghz3744, zghz3743);
      zID_AA64AFR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3744, true);
    }
    unit zgsz3336;
    zgsz3336 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3741);
      CONVERT_OF(sail_int, mach_int)(&zghz3741, INT64_C(32));
      RECREATE(lbits)(&zghz3742);
      UNDEFINED(lbits)(&zghz3742, zghz3741);
      zID_AA64AFR1_EL1 = CONVERT_OF(fbits, lbits)(zghz3742, true);
    }
    unit zgsz3335;
    zgsz3335 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3739);
      CONVERT_OF(sail_int, mach_int)(&zghz3739, INT64_C(64));
      RECREATE(lbits)(&zghz3740);
      UNDEFINED(lbits)(&zghz3740, zghz3739);
      zID_AA64DFR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3740, true);
    }
    unit zgsz3334;
    zgsz3334 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3737);
      CONVERT_OF(sail_int, mach_int)(&zghz3737, INT64_C(32));
      RECREATE(lbits)(&zghz3738);
      UNDEFINED(lbits)(&zghz3738, zghz3737);
      zID_AA64DFR1_EL1 = CONVERT_OF(fbits, lbits)(zghz3738, true);
    }
    unit zgsz3333;
    zgsz3333 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3735);
      CONVERT_OF(sail_int, mach_int)(&zghz3735, INT64_C(64));
      RECREATE(lbits)(&zghz3736);
      UNDEFINED(lbits)(&zghz3736, zghz3735);
      zID_AA64ISAR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3736, true);
    }
    unit zgsz3332;
    zgsz3332 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3733);
      CONVERT_OF(sail_int, mach_int)(&zghz3733, INT64_C(64));
      RECREATE(lbits)(&zghz3734);
      UNDEFINED(lbits)(&zghz3734, zghz3733);
      zID_AA64ISAR1_EL1 = CONVERT_OF(fbits, lbits)(zghz3734, true);
    }
    unit zgsz3331;
    zgsz3331 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3731);
      CONVERT_OF(sail_int, mach_int)(&zghz3731, INT64_C(32));
      RECREATE(lbits)(&zghz3732);
      UNDEFINED(lbits)(&zghz3732, zghz3731);
      zID_AA64MMFR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3732, true);
    }
    unit zgsz3330;
    zgsz3330 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3729);
      CONVERT_OF(sail_int, mach_int)(&zghz3729, INT64_C(64));
      RECREATE(lbits)(&zghz3730);
      UNDEFINED(lbits)(&zghz3730, zghz3729);
      zID_AA64MMFR1_EL1 = CONVERT_OF(fbits, lbits)(zghz3730, true);
    }
    unit zgsz3329;
    zgsz3329 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3727);
      CONVERT_OF(sail_int, mach_int)(&zghz3727, INT64_C(64));
      RECREATE(lbits)(&zghz3728);
      UNDEFINED(lbits)(&zghz3728, zghz3727);
      zID_AA64MMFR2_EL1 = CONVERT_OF(fbits, lbits)(zghz3728, true);
    }
    unit zgsz3328;
    zgsz3328 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3725);
      CONVERT_OF(sail_int, mach_int)(&zghz3725, INT64_C(64));
      RECREATE(lbits)(&zghz3726);
      UNDEFINED(lbits)(&zghz3726, zghz3725);
      zID_AA64PFR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3726, true);
    }
    unit zgsz3327;
    zgsz3327 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3723);
      CONVERT_OF(sail_int, mach_int)(&zghz3723, INT64_C(32));
      RECREATE(lbits)(&zghz3724);
      UNDEFINED(lbits)(&zghz3724, zghz3723);
      zID_AA64PFR1_EL1 = CONVERT_OF(fbits, lbits)(zghz3724, true);
    }
    unit zgsz3326;
    zgsz3326 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3721);
      CONVERT_OF(sail_int, mach_int)(&zghz3721, INT64_C(64));
      RECREATE(lbits)(&zghz3722);
      UNDEFINED(lbits)(&zghz3722, zghz3721);
      zID_AA64ZFR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3722, true);
    }
    unit zgsz3325;
    zgsz3325 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3719);
      CONVERT_OF(sail_int, mach_int)(&zghz3719, INT64_C(32));
      RECREATE(lbits)(&zghz3720);
      UNDEFINED(lbits)(&zghz3720, zghz3719);
      zID_AFR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3720, true);
    }
    unit zgsz3324;
    zgsz3324 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3717);
      CONVERT_OF(sail_int, mach_int)(&zghz3717, INT64_C(32));
      RECREATE(lbits)(&zghz3718);
      UNDEFINED(lbits)(&zghz3718, zghz3717);
      zID_DFR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3718, true);
    }
    unit zgsz3323;
    zgsz3323 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3715);
      CONVERT_OF(sail_int, mach_int)(&zghz3715, INT64_C(32));
      RECREATE(lbits)(&zghz3716);
      UNDEFINED(lbits)(&zghz3716, zghz3715);
      zID_ISAR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3716, true);
    }
    unit zgsz3322;
    zgsz3322 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3713);
      CONVERT_OF(sail_int, mach_int)(&zghz3713, INT64_C(32));
      RECREATE(lbits)(&zghz3714);
      UNDEFINED(lbits)(&zghz3714, zghz3713);
      zID_ISAR1_EL1 = CONVERT_OF(fbits, lbits)(zghz3714, true);
    }
    unit zgsz3321;
    zgsz3321 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3711);
      CONVERT_OF(sail_int, mach_int)(&zghz3711, INT64_C(32));
      RECREATE(lbits)(&zghz3712);
      UNDEFINED(lbits)(&zghz3712, zghz3711);
      zID_ISAR2_EL1 = CONVERT_OF(fbits, lbits)(zghz3712, true);
    }
    unit zgsz3320;
    zgsz3320 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3709);
      CONVERT_OF(sail_int, mach_int)(&zghz3709, INT64_C(32));
      RECREATE(lbits)(&zghz3710);
      UNDEFINED(lbits)(&zghz3710, zghz3709);
      zID_ISAR3_EL1 = CONVERT_OF(fbits, lbits)(zghz3710, true);
    }
    unit zgsz3319;
    zgsz3319 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3707);
      CONVERT_OF(sail_int, mach_int)(&zghz3707, INT64_C(32));
      RECREATE(lbits)(&zghz3708);
      UNDEFINED(lbits)(&zghz3708, zghz3707);
      zID_ISAR4_EL1 = CONVERT_OF(fbits, lbits)(zghz3708, true);
    }
    unit zgsz3318;
    zgsz3318 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3705);
      CONVERT_OF(sail_int, mach_int)(&zghz3705, INT64_C(32));
      RECREATE(lbits)(&zghz3706);
      UNDEFINED(lbits)(&zghz3706, zghz3705);
      zID_ISAR5_EL1 = CONVERT_OF(fbits, lbits)(zghz3706, true);
    }
    unit zgsz3317;
    zgsz3317 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3703);
      CONVERT_OF(sail_int, mach_int)(&zghz3703, INT64_C(32));
      RECREATE(lbits)(&zghz3704);
      UNDEFINED(lbits)(&zghz3704, zghz3703);
      zID_ISAR6_EL1 = CONVERT_OF(fbits, lbits)(zghz3704, true);
    }
    unit zgsz3316;
    zgsz3316 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3701);
      CONVERT_OF(sail_int, mach_int)(&zghz3701, INT64_C(32));
      RECREATE(lbits)(&zghz3702);
      UNDEFINED(lbits)(&zghz3702, zghz3701);
      zID_MMFR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3702, true);
    }
    unit zgsz3315;
    zgsz3315 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3699);
      CONVERT_OF(sail_int, mach_int)(&zghz3699, INT64_C(32));
      RECREATE(lbits)(&zghz3700);
      UNDEFINED(lbits)(&zghz3700, zghz3699);
      zID_MMFR1_EL1 = CONVERT_OF(fbits, lbits)(zghz3700, true);
    }
    unit zgsz3314;
    zgsz3314 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3697);
      CONVERT_OF(sail_int, mach_int)(&zghz3697, INT64_C(32));
      RECREATE(lbits)(&zghz3698);
      UNDEFINED(lbits)(&zghz3698, zghz3697);
      zID_MMFR2_EL1 = CONVERT_OF(fbits, lbits)(zghz3698, true);
    }
    unit zgsz3313;
    zgsz3313 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3695);
      CONVERT_OF(sail_int, mach_int)(&zghz3695, INT64_C(32));
      RECREATE(lbits)(&zghz3696);
      UNDEFINED(lbits)(&zghz3696, zghz3695);
      zID_MMFR3_EL1 = CONVERT_OF(fbits, lbits)(zghz3696, true);
    }
    unit zgsz3312;
    zgsz3312 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3693);
      CONVERT_OF(sail_int, mach_int)(&zghz3693, INT64_C(32));
      RECREATE(lbits)(&zghz3694);
      UNDEFINED(lbits)(&zghz3694, zghz3693);
      zID_MMFR4_EL1 = CONVERT_OF(fbits, lbits)(zghz3694, true);
    }
    unit zgsz3311;
    zgsz3311 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3691);
      CONVERT_OF(sail_int, mach_int)(&zghz3691, INT64_C(32));
      RECREATE(lbits)(&zghz3692);
      UNDEFINED(lbits)(&zghz3692, zghz3691);
      zID_MMFR5_EL1 = CONVERT_OF(fbits, lbits)(zghz3692, true);
    }
    unit zgsz3310;
    zgsz3310 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3689);
      CONVERT_OF(sail_int, mach_int)(&zghz3689, INT64_C(32));
      RECREATE(lbits)(&zghz3690);
      UNDEFINED(lbits)(&zghz3690, zghz3689);
      zID_PFR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3690, true);
    }
    unit zgsz3309;
    zgsz3309 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3687);
      CONVERT_OF(sail_int, mach_int)(&zghz3687, INT64_C(32));
      RECREATE(lbits)(&zghz3688);
      UNDEFINED(lbits)(&zghz3688, zghz3687);
      zID_PFR1_EL1 = CONVERT_OF(fbits, lbits)(zghz3688, true);
    }
    unit zgsz3308;
    zgsz3308 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3685);
      CONVERT_OF(sail_int, mach_int)(&zghz3685, INT64_C(32));
      RECREATE(lbits)(&zghz3686);
      UNDEFINED(lbits)(&zghz3686, zghz3685);
      zID_PFR2_EL1 = CONVERT_OF(fbits, lbits)(zghz3686, true);
    }
    unit zgsz3307;
    zgsz3307 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3683);
      CONVERT_OF(sail_int, mach_int)(&zghz3683, INT64_C(32));
      RECREATE(lbits)(&zghz3684);
      UNDEFINED(lbits)(&zghz3684, zghz3683);
      zIFSR32_EL2 = CONVERT_OF(fbits, lbits)(zghz3684, true);
    }
    unit zgsz3306;
    zgsz3306 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3681);
      CONVERT_OF(sail_int, mach_int)(&zghz3681, INT64_C(32));
      RECREATE(lbits)(&zghz3682);
      UNDEFINED(lbits)(&zghz3682, zghz3681);
      zISR_EL1 = CONVERT_OF(fbits, lbits)(zghz3682, true);
    }
    unit zgsz3305;
    zgsz3305 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3679);
      CONVERT_OF(sail_int, mach_int)(&zghz3679, INT64_C(32));
      RECREATE(lbits)(&zghz3680);
      UNDEFINED(lbits)(&zghz3680, zghz3679);
      zLORC_EL1 = CONVERT_OF(fbits, lbits)(zghz3680, true);
    }
    unit zgsz3304;
    zgsz3304 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3677);
      CONVERT_OF(sail_int, mach_int)(&zghz3677, INT64_C(64));
      RECREATE(lbits)(&zghz3678);
      UNDEFINED(lbits)(&zghz3678, zghz3677);
      zLOREA_EL1 = CONVERT_OF(fbits, lbits)(zghz3678, true);
    }
    unit zgsz3303;
    zgsz3303 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3675);
      CONVERT_OF(sail_int, mach_int)(&zghz3675, INT64_C(32));
      RECREATE(lbits)(&zghz3676);
      UNDEFINED(lbits)(&zghz3676, zghz3675);
      zLORID_EL1 = CONVERT_OF(fbits, lbits)(zghz3676, true);
    }
    unit zgsz3302;
    zgsz3302 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3673);
      CONVERT_OF(sail_int, mach_int)(&zghz3673, INT64_C(32));
      RECREATE(lbits)(&zghz3674);
      UNDEFINED(lbits)(&zghz3674, zghz3673);
      zLORN_EL1 = CONVERT_OF(fbits, lbits)(zghz3674, true);
    }
    unit zgsz3301;
    zgsz3301 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3671);
      CONVERT_OF(sail_int, mach_int)(&zghz3671, INT64_C(64));
      RECREATE(lbits)(&zghz3672);
      UNDEFINED(lbits)(&zghz3672, zghz3671);
      zLORSA_EL1 = CONVERT_OF(fbits, lbits)(zghz3672, true);
    }
    unit zgsz3300;
    zgsz3300 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3669);
      CONVERT_OF(sail_int, mach_int)(&zghz3669, INT64_C(64));
      RECREATE(lbits)(&zghz3670);
      UNDEFINED(lbits)(&zghz3670, zghz3669);
      zMAIR_EL1 = CONVERT_OF(fbits, lbits)(zghz3670, true);
    }
    unit zgsz3299;
    zgsz3299 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3667);
      CONVERT_OF(sail_int, mach_int)(&zghz3667, INT64_C(64));
      RECREATE(lbits)(&zghz3668);
      UNDEFINED(lbits)(&zghz3668, zghz3667);
      zMAIR_EL2 = CONVERT_OF(fbits, lbits)(zghz3668, true);
    }
    unit zgsz3298;
    zgsz3298 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3665);
      CONVERT_OF(sail_int, mach_int)(&zghz3665, INT64_C(64));
      RECREATE(lbits)(&zghz3666);
      UNDEFINED(lbits)(&zghz3666, zghz3665);
      zMAIR_EL3 = CONVERT_OF(fbits, lbits)(zghz3666, true);
    }
    unit zgsz3297;
    zgsz3297 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3663);
      CONVERT_OF(sail_int, mach_int)(&zghz3663, INT64_C(32));
      RECREATE(lbits)(&zghz3664);
      UNDEFINED(lbits)(&zghz3664, zghz3663);
      zMDCCINT_EL1 = CONVERT_OF(fbits, lbits)(zghz3664, true);
    }
    unit zgsz3296;
    zgsz3296 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3661);
      CONVERT_OF(sail_int, mach_int)(&zghz3661, INT64_C(32));
      RECREATE(lbits)(&zghz3662);
      UNDEFINED(lbits)(&zghz3662, zghz3661);
      zMDCCSR_EL0 = CONVERT_OF(fbits, lbits)(zghz3662, true);
    }
    unit zgsz3295;
    zgsz3295 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3659);
      CONVERT_OF(sail_int, mach_int)(&zghz3659, INT64_C(64));
      RECREATE(lbits)(&zghz3660);
      UNDEFINED(lbits)(&zghz3660, zghz3659);
      zMDRAR_EL1 = CONVERT_OF(fbits, lbits)(zghz3660, true);
    }
    unit zgsz3294;
    zgsz3294 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3657);
      CONVERT_OF(sail_int, mach_int)(&zghz3657, INT64_C(32));
      RECREATE(lbits)(&zghz3658);
      UNDEFINED(lbits)(&zghz3658, zghz3657);
      zMIDR_EL1 = CONVERT_OF(fbits, lbits)(zghz3658, true);
    }
    unit zgsz3293;
    zgsz3293 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3655);
      CONVERT_OF(sail_int, mach_int)(&zghz3655, INT64_C(32));
      RECREATE(lbits)(&zghz3656);
      UNDEFINED(lbits)(&zghz3656, zghz3655);
      zVPIDR_EL2 = CONVERT_OF(fbits, lbits)(zghz3656, true);
    }
    unit zgsz3292;
    zgsz3292 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3653);
      CONVERT_OF(sail_int, mach_int)(&zghz3653, INT64_C(64));
      RECREATE(lbits)(&zghz3654);
      UNDEFINED(lbits)(&zghz3654, zghz3653);
      zMPAM0_EL1 = CONVERT_OF(fbits, lbits)(zghz3654, true);
    }
    unit zgsz3291;
    zgsz3291 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3651);
      CONVERT_OF(sail_int, mach_int)(&zghz3651, INT64_C(64));
      RECREATE(lbits)(&zghz3652);
      UNDEFINED(lbits)(&zghz3652, zghz3651);
      zMPAM3_EL3 = CONVERT_OF(fbits, lbits)(zghz3652, true);
    }
    unit zgsz3290;
    zgsz3290 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3649);
      CONVERT_OF(sail_int, mach_int)(&zghz3649, INT64_C(63));
      RECREATE(lbits)(&zghz3650);
      UNDEFINED(lbits)(&zghz3650, zghz3649);
      z_MPAM2_EL2_0_62 = CONVERT_OF(fbits, lbits)(zghz3650, true);
    }
    unit zgsz3289;
    zgsz3289 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3647);
      CONVERT_OF(sail_int, mach_int)(&zghz3647, INT64_C(63));
      RECREATE(lbits)(&zghz3648);
      UNDEFINED(lbits)(&zghz3648, zghz3647);
      z_MPAM1_EL1_0_62 = CONVERT_OF(fbits, lbits)(zghz3648, true);
    }
    unit zgsz3288;
    zgsz3288 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3645);
      CONVERT_OF(sail_int, mach_int)(&zghz3645, INT64_C(32));
      RECREATE(lbits)(&zghz3646);
      UNDEFINED(lbits)(&zghz3646, zghz3645);
      zMPAMHCR_EL2 = CONVERT_OF(fbits, lbits)(zghz3646, true);
    }
    unit zgsz3287;
    zgsz3287 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3643);
      CONVERT_OF(sail_int, mach_int)(&zghz3643, INT64_C(64));
      RECREATE(lbits)(&zghz3644);
      UNDEFINED(lbits)(&zghz3644, zghz3643);
      zMPAMIDR_EL1 = CONVERT_OF(fbits, lbits)(zghz3644, true);
    }
    unit zgsz3286;
    zgsz3286 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3641);
      CONVERT_OF(sail_int, mach_int)(&zghz3641, INT64_C(64));
      RECREATE(lbits)(&zghz3642);
      UNDEFINED(lbits)(&zghz3642, zghz3641);
      zMPAMVPM0_EL2 = CONVERT_OF(fbits, lbits)(zghz3642, true);
    }
    unit zgsz3285;
    zgsz3285 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3639);
      CONVERT_OF(sail_int, mach_int)(&zghz3639, INT64_C(64));
      RECREATE(lbits)(&zghz3640);
      UNDEFINED(lbits)(&zghz3640, zghz3639);
      zMPAMVPM1_EL2 = CONVERT_OF(fbits, lbits)(zghz3640, true);
    }
    unit zgsz3284;
    zgsz3284 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3637);
      CONVERT_OF(sail_int, mach_int)(&zghz3637, INT64_C(64));
      RECREATE(lbits)(&zghz3638);
      UNDEFINED(lbits)(&zghz3638, zghz3637);
      zMPAMVPM2_EL2 = CONVERT_OF(fbits, lbits)(zghz3638, true);
    }
    unit zgsz3283;
    zgsz3283 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3635);
      CONVERT_OF(sail_int, mach_int)(&zghz3635, INT64_C(64));
      RECREATE(lbits)(&zghz3636);
      UNDEFINED(lbits)(&zghz3636, zghz3635);
      zMPAMVPM3_EL2 = CONVERT_OF(fbits, lbits)(zghz3636, true);
    }
    unit zgsz3282;
    zgsz3282 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3633);
      CONVERT_OF(sail_int, mach_int)(&zghz3633, INT64_C(64));
      RECREATE(lbits)(&zghz3634);
      UNDEFINED(lbits)(&zghz3634, zghz3633);
      zMPAMVPM4_EL2 = CONVERT_OF(fbits, lbits)(zghz3634, true);
    }
    unit zgsz3281;
    zgsz3281 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3631);
      CONVERT_OF(sail_int, mach_int)(&zghz3631, INT64_C(64));
      RECREATE(lbits)(&zghz3632);
      UNDEFINED(lbits)(&zghz3632, zghz3631);
      zMPAMVPM5_EL2 = CONVERT_OF(fbits, lbits)(zghz3632, true);
    }
    unit zgsz3280;
    zgsz3280 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3629);
      CONVERT_OF(sail_int, mach_int)(&zghz3629, INT64_C(64));
      RECREATE(lbits)(&zghz3630);
      UNDEFINED(lbits)(&zghz3630, zghz3629);
      zMPAMVPM6_EL2 = CONVERT_OF(fbits, lbits)(zghz3630, true);
    }
    unit zgsz3279;
    zgsz3279 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3627);
      CONVERT_OF(sail_int, mach_int)(&zghz3627, INT64_C(64));
      RECREATE(lbits)(&zghz3628);
      UNDEFINED(lbits)(&zghz3628, zghz3627);
      zMPAMVPM7_EL2 = CONVERT_OF(fbits, lbits)(zghz3628, true);
    }
    unit zgsz3278;
    zgsz3278 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3625);
      CONVERT_OF(sail_int, mach_int)(&zghz3625, INT64_C(32));
      RECREATE(lbits)(&zghz3626);
      UNDEFINED(lbits)(&zghz3626, zghz3625);
      zMPAMVPMV_EL2 = CONVERT_OF(fbits, lbits)(zghz3626, true);
    }
    unit zgsz3277;
    zgsz3277 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3623);
      CONVERT_OF(sail_int, mach_int)(&zghz3623, INT64_C(64));
      RECREATE(lbits)(&zghz3624);
      UNDEFINED(lbits)(&zghz3624, zghz3623);
      zMPIDR_EL1 = CONVERT_OF(fbits, lbits)(zghz3624, true);
    }
    unit zgsz3276;
    zgsz3276 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3621);
      CONVERT_OF(sail_int, mach_int)(&zghz3621, INT64_C(64));
      RECREATE(lbits)(&zghz3622);
      UNDEFINED(lbits)(&zghz3622, zghz3621);
      zVMPIDR_EL2 = CONVERT_OF(fbits, lbits)(zghz3622, true);
    }
    unit zgsz3275;
    zgsz3275 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3619);
      CONVERT_OF(sail_int, mach_int)(&zghz3619, INT64_C(32));
      RECREATE(lbits)(&zghz3620);
      UNDEFINED(lbits)(&zghz3620, zghz3619);
      zMVFR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3620, true);
    }
    unit zgsz3274;
    zgsz3274 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3617);
      CONVERT_OF(sail_int, mach_int)(&zghz3617, INT64_C(32));
      RECREATE(lbits)(&zghz3618);
      UNDEFINED(lbits)(&zghz3618, zghz3617);
      zMVFR1_EL1 = CONVERT_OF(fbits, lbits)(zghz3618, true);
    }
    unit zgsz3273;
    zgsz3273 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3615);
      CONVERT_OF(sail_int, mach_int)(&zghz3615, INT64_C(32));
      RECREATE(lbits)(&zghz3616);
      UNDEFINED(lbits)(&zghz3616, zghz3615);
      zMVFR2_EL1 = CONVERT_OF(fbits, lbits)(zghz3616, true);
    }
    unit zgsz3272;
    zgsz3272 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3613);
      CONVERT_OF(sail_int, mach_int)(&zghz3613, INT64_C(32));
      RECREATE(lbits)(&zghz3614);
      UNDEFINED(lbits)(&zghz3614, zghz3613);
      zOSDTRRX_EL1 = CONVERT_OF(fbits, lbits)(zghz3614, true);
    }
    unit zgsz3271;
    zgsz3271 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3611);
      CONVERT_OF(sail_int, mach_int)(&zghz3611, INT64_C(32));
      RECREATE(lbits)(&zghz3612);
      UNDEFINED(lbits)(&zghz3612, zghz3611);
      zOSDTRTX_EL1 = CONVERT_OF(fbits, lbits)(zghz3612, true);
    }
    unit zgsz3270;
    zgsz3270 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3609);
      CONVERT_OF(sail_int, mach_int)(&zghz3609, INT64_C(32));
      RECREATE(lbits)(&zghz3610);
      UNDEFINED(lbits)(&zghz3610, zghz3609);
      zOSECCR_EL1 = CONVERT_OF(fbits, lbits)(zghz3610, true);
    }
    unit zgsz3269;
    zgsz3269 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3607);
      CONVERT_OF(sail_int, mach_int)(&zghz3607, INT64_C(64));
      RECREATE(lbits)(&zghz3608);
      UNDEFINED(lbits)(&zghz3608, zghz3607);
      zPAR_EL1 = CONVERT_OF(fbits, lbits)(zghz3608, true);
    }
    unit zgsz3268;
    zgsz3268 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3605);
      CONVERT_OF(sail_int, mach_int)(&zghz3605, INT64_C(32));
      RECREATE(lbits)(&zghz3606);
      UNDEFINED(lbits)(&zghz3606, zghz3605);
      zPMBIDR_EL1 = CONVERT_OF(fbits, lbits)(zghz3606, true);
    }
    unit zgsz3267;
    zgsz3267 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3603);
      CONVERT_OF(sail_int, mach_int)(&zghz3603, INT64_C(64));
      RECREATE(lbits)(&zghz3604);
      UNDEFINED(lbits)(&zghz3604, zghz3603);
      zPMBLIMITR_EL1 = CONVERT_OF(fbits, lbits)(zghz3604, true);
    }
    unit zgsz3266;
    zgsz3266 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3601);
      CONVERT_OF(sail_int, mach_int)(&zghz3601, INT64_C(64));
      RECREATE(lbits)(&zghz3602);
      UNDEFINED(lbits)(&zghz3602, zghz3601);
      zPMBPTR_EL1 = CONVERT_OF(fbits, lbits)(zghz3602, true);
    }
    unit zgsz3265;
    zgsz3265 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3599);
      CONVERT_OF(sail_int, mach_int)(&zghz3599, INT64_C(32));
      RECREATE(lbits)(&zghz3600);
      UNDEFINED(lbits)(&zghz3600, zghz3599);
      zPMBSR_EL1 = CONVERT_OF(fbits, lbits)(zghz3600, true);
    }
    unit zgsz3264;
    zgsz3264 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3597);
      CONVERT_OF(sail_int, mach_int)(&zghz3597, INT64_C(32));
      RECREATE(lbits)(&zghz3598);
      UNDEFINED(lbits)(&zghz3598, zghz3597);
      zPMCCFILTR_EL0 = CONVERT_OF(fbits, lbits)(zghz3598, true);
    }
    unit zgsz3263;
    zgsz3263 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3595);
      CONVERT_OF(sail_int, mach_int)(&zghz3595, INT64_C(32));
      RECREATE(lbits)(&zghz3596);
      UNDEFINED(lbits)(&zghz3596, zghz3595);
      zPMUSERENR_EL0 = CONVERT_OF(fbits, lbits)(zghz3596, true);
    }
    unit zgsz3262;
    zgsz3262 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3593);
      CONVERT_OF(sail_int, mach_int)(&zghz3593, INT64_C(64));
      RECREATE(lbits)(&zghz3594);
      UNDEFINED(lbits)(&zghz3594, zghz3593);
      zPMCCNTR_EL0 = CONVERT_OF(fbits, lbits)(zghz3594, true);
    }
    unit zgsz3261;
    zgsz3261 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3591);
      CONVERT_OF(sail_int, mach_int)(&zghz3591, INT64_C(64));
      RECREATE(lbits)(&zghz3592);
      UNDEFINED(lbits)(&zghz3592, zghz3591);
      zPMCEID0_EL0 = CONVERT_OF(fbits, lbits)(zghz3592, true);
    }
    unit zgsz3260;
    zgsz3260 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3589);
      CONVERT_OF(sail_int, mach_int)(&zghz3589, INT64_C(64));
      RECREATE(lbits)(&zghz3590);
      UNDEFINED(lbits)(&zghz3590, zghz3589);
      zPMCEID1_EL0 = CONVERT_OF(fbits, lbits)(zghz3590, true);
    }
    unit zgsz3259;
    zgsz3259 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3587);
      CONVERT_OF(sail_int, mach_int)(&zghz3587, INT64_C(32));
      RECREATE(lbits)(&zghz3588);
      UNDEFINED(lbits)(&zghz3588, zghz3587);
      zPMCNTENCLR_EL0 = CONVERT_OF(fbits, lbits)(zghz3588, true);
    }
    unit zgsz3258;
    zgsz3258 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3585);
      CONVERT_OF(sail_int, mach_int)(&zghz3585, INT64_C(32));
      RECREATE(lbits)(&zghz3586);
      UNDEFINED(lbits)(&zghz3586, zghz3585);
      zPMCNTENSET_EL0 = CONVERT_OF(fbits, lbits)(zghz3586, true);
    }
    unit zgsz3257;
    zgsz3257 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3583);
      CONVERT_OF(sail_int, mach_int)(&zghz3583, INT64_C(32));
      RECREATE(lbits)(&zghz3584);
      UNDEFINED(lbits)(&zghz3584, zghz3583);
      zPMCR_EL0 = CONVERT_OF(fbits, lbits)(zghz3584, true);
    }
    unit zgsz3256;
    zgsz3256 = UNIT;
  }
  {
    uint64_t zgaz3277;
    {
      RECREATE(sail_int)(&zghz3581);
      CONVERT_OF(sail_int, mach_int)(&zghz3581, INT64_C(32));
      RECREATE(lbits)(&zghz3582);
      UNDEFINED(lbits)(&zghz3582, zghz3581);
      zgaz3277 = CONVERT_OF(fbits, lbits)(zghz3582, true);
    }
    {
      RECREATE(sail_int)(&zghz3580);
      CONVERT_OF(sail_int, mach_int)(&zghz3580, INT64_C(31));
      UNDEFINED(vector_zz5vecz8z5bv32z9)(&zPMEVCNTR_EL0, zghz3580, zgaz3277);
    }
    unit zgsz3255;
    zgsz3255 = UNIT;
  }
  {
    uint64_t zgaz3278;
    {
      RECREATE(sail_int)(&zghz3578);
      CONVERT_OF(sail_int, mach_int)(&zghz3578, INT64_C(32));
      RECREATE(lbits)(&zghz3579);
      UNDEFINED(lbits)(&zghz3579, zghz3578);
      zgaz3278 = CONVERT_OF(fbits, lbits)(zghz3579, true);
    }
    {
      RECREATE(sail_int)(&zghz3577);
      CONVERT_OF(sail_int, mach_int)(&zghz3577, INT64_C(31));
      UNDEFINED(vector_zz5vecz8z5bv32z9)(&zPMEVTYPER_EL0, zghz3577, zgaz3278);
    }
    unit zgsz3254;
    zgsz3254 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3575);
      CONVERT_OF(sail_int, mach_int)(&zghz3575, INT64_C(32));
      RECREATE(lbits)(&zghz3576);
      UNDEFINED(lbits)(&zghz3576, zghz3575);
      zPMINTENCLR_EL1 = CONVERT_OF(fbits, lbits)(zghz3576, true);
    }
    unit zgsz3253;
    zgsz3253 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3573);
      CONVERT_OF(sail_int, mach_int)(&zghz3573, INT64_C(32));
      RECREATE(lbits)(&zghz3574);
      UNDEFINED(lbits)(&zghz3574, zghz3573);
      zPMINTENSET_EL1 = CONVERT_OF(fbits, lbits)(zghz3574, true);
    }
    unit zgsz3252;
    zgsz3252 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3571);
      CONVERT_OF(sail_int, mach_int)(&zghz3571, INT64_C(32));
      RECREATE(lbits)(&zghz3572);
      UNDEFINED(lbits)(&zghz3572, zghz3571);
      zPMOVSCLR_EL0 = CONVERT_OF(fbits, lbits)(zghz3572, true);
    }
    unit zgsz3251;
    zgsz3251 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3569);
      CONVERT_OF(sail_int, mach_int)(&zghz3569, INT64_C(32));
      RECREATE(lbits)(&zghz3570);
      UNDEFINED(lbits)(&zghz3570, zghz3569);
      zPMOVSSET_EL0 = CONVERT_OF(fbits, lbits)(zghz3570, true);
    }
    unit zgsz3250;
    zgsz3250 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3567);
      CONVERT_OF(sail_int, mach_int)(&zghz3567, INT64_C(32));
      RECREATE(lbits)(&zghz3568);
      UNDEFINED(lbits)(&zghz3568, zghz3567);
      zPMSCR_EL1 = CONVERT_OF(fbits, lbits)(zghz3568, true);
    }
    unit zgsz3249;
    zgsz3249 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3565);
      CONVERT_OF(sail_int, mach_int)(&zghz3565, INT64_C(32));
      RECREATE(lbits)(&zghz3566);
      UNDEFINED(lbits)(&zghz3566, zghz3565);
      zPMSCR_EL2 = CONVERT_OF(fbits, lbits)(zghz3566, true);
    }
    unit zgsz3248;
    zgsz3248 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3563);
      CONVERT_OF(sail_int, mach_int)(&zghz3563, INT64_C(32));
      RECREATE(lbits)(&zghz3564);
      UNDEFINED(lbits)(&zghz3564, zghz3563);
      zPMSELR_EL0 = CONVERT_OF(fbits, lbits)(zghz3564, true);
    }
    unit zgsz3247;
    zgsz3247 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3561);
      CONVERT_OF(sail_int, mach_int)(&zghz3561, INT64_C(64));
      RECREATE(lbits)(&zghz3562);
      UNDEFINED(lbits)(&zghz3562, zghz3561);
      zPMSEVFR_EL1 = CONVERT_OF(fbits, lbits)(zghz3562, true);
    }
    unit zgsz3246;
    zgsz3246 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3559);
      CONVERT_OF(sail_int, mach_int)(&zghz3559, INT64_C(32));
      RECREATE(lbits)(&zghz3560);
      UNDEFINED(lbits)(&zghz3560, zghz3559);
      zPMSFCR_EL1 = CONVERT_OF(fbits, lbits)(zghz3560, true);
    }
    unit zgsz3245;
    zgsz3245 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3557);
      CONVERT_OF(sail_int, mach_int)(&zghz3557, INT64_C(64));
      RECREATE(lbits)(&zghz3558);
      UNDEFINED(lbits)(&zghz3558, zghz3557);
      zPMSICR_EL1 = CONVERT_OF(fbits, lbits)(zghz3558, true);
    }
    unit zgsz3244;
    zgsz3244 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3555);
      CONVERT_OF(sail_int, mach_int)(&zghz3555, INT64_C(32));
      RECREATE(lbits)(&zghz3556);
      UNDEFINED(lbits)(&zghz3556, zghz3555);
      zPMSIDR_EL1 = CONVERT_OF(fbits, lbits)(zghz3556, true);
    }
    unit zgsz3243;
    zgsz3243 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3553);
      CONVERT_OF(sail_int, mach_int)(&zghz3553, INT64_C(32));
      RECREATE(lbits)(&zghz3554);
      UNDEFINED(lbits)(&zghz3554, zghz3553);
      zPMSIRR_EL1 = CONVERT_OF(fbits, lbits)(zghz3554, true);
    }
    unit zgsz3242;
    zgsz3242 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3551);
      CONVERT_OF(sail_int, mach_int)(&zghz3551, INT64_C(32));
      RECREATE(lbits)(&zghz3552);
      UNDEFINED(lbits)(&zghz3552, zghz3551);
      zPMSLATFR_EL1 = CONVERT_OF(fbits, lbits)(zghz3552, true);
    }
    unit zgsz3241;
    zgsz3241 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3549);
      CONVERT_OF(sail_int, mach_int)(&zghz3549, INT64_C(32));
      RECREATE(lbits)(&zghz3550);
      UNDEFINED(lbits)(&zghz3550, zghz3549);
      zPMXEVCNTR_EL0 = CONVERT_OF(fbits, lbits)(zghz3550, true);
    }
    unit zgsz3240;
    zgsz3240 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3547);
      CONVERT_OF(sail_int, mach_int)(&zghz3547, INT64_C(32));
      RECREATE(lbits)(&zghz3548);
      UNDEFINED(lbits)(&zghz3548, zghz3547);
      zPMXEVTYPER_EL0 = CONVERT_OF(fbits, lbits)(zghz3548, true);
    }
    unit zgsz3239;
    zgsz3239 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3545);
      CONVERT_OF(sail_int, mach_int)(&zghz3545, INT64_C(64));
      RECREATE(lbits)(&zghz3546);
      UNDEFINED(lbits)(&zghz3546, zghz3545);
      zREVIDR_EL1 = CONVERT_OF(fbits, lbits)(zghz3546, true);
    }
    unit zgsz3238;
    zgsz3238 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3543);
      CONVERT_OF(sail_int, mach_int)(&zghz3543, INT64_C(32));
      RECREATE(lbits)(&zghz3544);
      UNDEFINED(lbits)(&zghz3544, zghz3543);
      zRMR_EL1 = CONVERT_OF(fbits, lbits)(zghz3544, true);
    }
    unit zgsz3237;
    zgsz3237 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3541);
      CONVERT_OF(sail_int, mach_int)(&zghz3541, INT64_C(32));
      RECREATE(lbits)(&zghz3542);
      UNDEFINED(lbits)(&zghz3542, zghz3541);
      zRMR_EL2 = CONVERT_OF(fbits, lbits)(zghz3542, true);
    }
    unit zgsz3236;
    zgsz3236 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3539);
      CONVERT_OF(sail_int, mach_int)(&zghz3539, INT64_C(32));
      RECREATE(lbits)(&zghz3540);
      UNDEFINED(lbits)(&zghz3540, zghz3539);
      zRMR_EL3 = CONVERT_OF(fbits, lbits)(zghz3540, true);
    }
    unit zgsz3235;
    zgsz3235 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3538);
      CONVERT_OF(sail_int, mach_int)(&zghz3538, INT64_C(129));
      UNDEFINED(lbits)(&zRSP_EL0, zghz3538);
    }
    unit zgsz3234;
    zgsz3234 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3537);
      CONVERT_OF(sail_int, mach_int)(&zghz3537, INT64_C(129));
      UNDEFINED(lbits)(&zRTPIDR_EL0, zghz3537);
    }
    unit zgsz3233;
    zgsz3233 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3535);
      CONVERT_OF(sail_int, mach_int)(&zghz3535, INT64_C(64));
      RECREATE(lbits)(&zghz3536);
      UNDEFINED(lbits)(&zghz3536, zghz3535);
      zRVBAR_EL1 = CONVERT_OF(fbits, lbits)(zghz3536, true);
    }
    unit zgsz3232;
    zgsz3232 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3533);
      CONVERT_OF(sail_int, mach_int)(&zghz3533, INT64_C(64));
      RECREATE(lbits)(&zghz3534);
      UNDEFINED(lbits)(&zghz3534, zghz3533);
      zRVBAR_EL2 = CONVERT_OF(fbits, lbits)(zghz3534, true);
    }
    unit zgsz3231;
    zgsz3231 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3531);
      CONVERT_OF(sail_int, mach_int)(&zghz3531, INT64_C(64));
      RECREATE(lbits)(&zghz3532);
      UNDEFINED(lbits)(&zghz3532, zghz3531);
      zRVBAR_EL3 = CONVERT_OF(fbits, lbits)(zghz3532, true);
    }
    unit zgsz3230;
    zgsz3230 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3529);
      CONVERT_OF(sail_int, mach_int)(&zghz3529, INT64_C(64));
      RECREATE(lbits)(&zghz3530);
      UNDEFINED(lbits)(&zghz3530, zghz3529);
      zS3_op1_Cn_Cm_op2 = CONVERT_OF(fbits, lbits)(zghz3530, true);
    }
    unit zgsz3229;
    zgsz3229 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3528);
      CONVERT_OF(sail_int, mach_int)(&zghz3528, INT64_C(129));
      UNDEFINED(lbits)(&zCID_EL0, zghz3528);
    }
    unit zgsz3228;
    zgsz3228 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3526);
      CONVERT_OF(sail_int, mach_int)(&zghz3526, INT64_C(64));
      RECREATE(lbits)(&zghz3527);
      UNDEFINED(lbits)(&zghz3527, zghz3526);
      zSCXTNUM_EL1 = CONVERT_OF(fbits, lbits)(zghz3527, true);
    }
    unit zgsz3227;
    zgsz3227 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3524);
      CONVERT_OF(sail_int, mach_int)(&zghz3524, INT64_C(64));
      RECREATE(lbits)(&zghz3525);
      UNDEFINED(lbits)(&zghz3525, zghz3524);
      zSCXTNUM_EL2 = CONVERT_OF(fbits, lbits)(zghz3525, true);
    }
    unit zgsz3226;
    zgsz3226 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3522);
      CONVERT_OF(sail_int, mach_int)(&zghz3522, INT64_C(64));
      RECREATE(lbits)(&zghz3523);
      UNDEFINED(lbits)(&zghz3523, zghz3522);
      zSCXTNUM_EL3 = CONVERT_OF(fbits, lbits)(zghz3523, true);
    }
    unit zgsz3225;
    zgsz3225 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3520);
      CONVERT_OF(sail_int, mach_int)(&zghz3520, INT64_C(32));
      RECREATE(lbits)(&zghz3521);
      UNDEFINED(lbits)(&zghz3521, zghz3520);
      zSDER32_EL3 = CONVERT_OF(fbits, lbits)(zghz3521, true);
    }
    unit zgsz3224;
    zgsz3224 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3518);
      CONVERT_OF(sail_int, mach_int)(&zghz3518, INT64_C(32));
      RECREATE(lbits)(&zghz3519);
      UNDEFINED(lbits)(&zghz3519, zghz3518);
      zSPSR_abt = CONVERT_OF(fbits, lbits)(zghz3519, true);
    }
    unit zgsz3223;
    zgsz3223 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3516);
      CONVERT_OF(sail_int, mach_int)(&zghz3516, INT64_C(32));
      RECREATE(lbits)(&zghz3517);
      UNDEFINED(lbits)(&zghz3517, zghz3516);
      zSPSR_fiq = CONVERT_OF(fbits, lbits)(zghz3517, true);
    }
    unit zgsz3222;
    zgsz3222 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3514);
      CONVERT_OF(sail_int, mach_int)(&zghz3514, INT64_C(32));
      RECREATE(lbits)(&zghz3515);
      UNDEFINED(lbits)(&zghz3515, zghz3514);
      zSPSR_irq = CONVERT_OF(fbits, lbits)(zghz3515, true);
    }
    unit zgsz3221;
    zgsz3221 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3512);
      CONVERT_OF(sail_int, mach_int)(&zghz3512, INT64_C(32));
      RECREATE(lbits)(&zghz3513);
      UNDEFINED(lbits)(&zghz3513, zghz3512);
      zSPSR_und = CONVERT_OF(fbits, lbits)(zghz3513, true);
    }
    unit zgsz3220;
    zgsz3220 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3511);
      CONVERT_OF(sail_int, mach_int)(&zghz3511, INT64_C(129));
      UNDEFINED(lbits)(&zSP_EL0, zghz3511);
    }
    unit zgsz3219;
    zgsz3219 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3510);
      CONVERT_OF(sail_int, mach_int)(&zghz3510, INT64_C(129));
      UNDEFINED(lbits)(&zSP_EL1, zghz3510);
    }
    unit zgsz3218;
    zgsz3218 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3509);
      CONVERT_OF(sail_int, mach_int)(&zghz3509, INT64_C(129));
      UNDEFINED(lbits)(&zSP_EL2, zghz3509);
    }
    unit zgsz3217;
    zgsz3217 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3508);
      CONVERT_OF(sail_int, mach_int)(&zghz3508, INT64_C(129));
      UNDEFINED(lbits)(&zTPIDRRO_EL0, zghz3508);
    }
    unit zgsz3216;
    zgsz3216 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3507);
      CONVERT_OF(sail_int, mach_int)(&zghz3507, INT64_C(129));
      UNDEFINED(lbits)(&zTPIDR_EL0, zghz3507);
    }
    unit zgsz3215;
    zgsz3215 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3506);
      CONVERT_OF(sail_int, mach_int)(&zghz3506, INT64_C(129));
      UNDEFINED(lbits)(&zTPIDR_EL1, zghz3506);
    }
    unit zgsz3214;
    zgsz3214 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3505);
      CONVERT_OF(sail_int, mach_int)(&zghz3505, INT64_C(129));
      UNDEFINED(lbits)(&zTPIDR_EL2, zghz3505);
    }
    unit zgsz3213;
    zgsz3213 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3504);
      CONVERT_OF(sail_int, mach_int)(&zghz3504, INT64_C(129));
      UNDEFINED(lbits)(&zTPIDR_EL3, zghz3504);
    }
    unit zgsz3212;
    zgsz3212 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3502);
      CONVERT_OF(sail_int, mach_int)(&zghz3502, INT64_C(64));
      RECREATE(lbits)(&zghz3503);
      UNDEFINED(lbits)(&zghz3503, zghz3502);
      zTTBR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3503, true);
    }
    unit zgsz3211;
    zgsz3211 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3500);
      CONVERT_OF(sail_int, mach_int)(&zghz3500, INT64_C(64));
      RECREATE(lbits)(&zghz3501);
      UNDEFINED(lbits)(&zghz3501, zghz3500);
      zTTBR0_EL2 = CONVERT_OF(fbits, lbits)(zghz3501, true);
    }
    unit zgsz3210;
    zgsz3210 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3498);
      CONVERT_OF(sail_int, mach_int)(&zghz3498, INT64_C(64));
      RECREATE(lbits)(&zghz3499);
      UNDEFINED(lbits)(&zghz3499, zghz3498);
      zTTBR0_EL3 = CONVERT_OF(fbits, lbits)(zghz3499, true);
    }
    unit zgsz3209;
    zgsz3209 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3496);
      CONVERT_OF(sail_int, mach_int)(&zghz3496, INT64_C(64));
      RECREATE(lbits)(&zghz3497);
      UNDEFINED(lbits)(&zghz3497, zghz3496);
      zTTBR1_EL1 = CONVERT_OF(fbits, lbits)(zghz3497, true);
    }
    unit zgsz3208;
    zgsz3208 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3494);
      CONVERT_OF(sail_int, mach_int)(&zghz3494, INT64_C(64));
      RECREATE(lbits)(&zghz3495);
      UNDEFINED(lbits)(&zghz3495, zghz3494);
      zTTBR1_EL2 = CONVERT_OF(fbits, lbits)(zghz3495, true);
    }
    unit zgsz3207;
    zgsz3207 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3492);
      CONVERT_OF(sail_int, mach_int)(&zghz3492, INT64_C(32));
      RECREATE(lbits)(&zghz3493);
      UNDEFINED(lbits)(&zghz3493, zghz3492);
      zVSESR_EL2 = CONVERT_OF(fbits, lbits)(zghz3493, true);
    }
    unit zgsz3206;
    zgsz3206 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3490);
      CONVERT_OF(sail_int, mach_int)(&zghz3490, INT64_C(32));
      RECREATE(lbits)(&zghz3491);
      UNDEFINED(lbits)(&zghz3491, zghz3490);
      zVTCR_EL2 = CONVERT_OF(fbits, lbits)(zghz3491, true);
    }
    unit zgsz3205;
    zgsz3205 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3488);
      CONVERT_OF(sail_int, mach_int)(&zghz3488, INT64_C(64));
      RECREATE(lbits)(&zghz3489);
      UNDEFINED(lbits)(&zghz3489, zghz3488);
      zVTTBR_EL2 = CONVERT_OF(fbits, lbits)(zghz3489, true);
    }
    unit zgsz3204;
    zgsz3204 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3487);
      CONVERT_OF(sail_int, mach_int)(&zghz3487, INT64_C(129));
      UNDEFINED(lbits)(&zDDC_EL0, zghz3487);
    }
    unit zgsz3203;
    zgsz3203 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3486);
      CONVERT_OF(sail_int, mach_int)(&zghz3486, INT64_C(129));
      UNDEFINED(lbits)(&zDDC_EL1, zghz3486);
    }
    unit zgsz3202;
    zgsz3202 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3485);
      CONVERT_OF(sail_int, mach_int)(&zghz3485, INT64_C(129));
      UNDEFINED(lbits)(&zDDC_EL2, zghz3485);
    }
    unit zgsz3201;
    zgsz3201 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3484);
      CONVERT_OF(sail_int, mach_int)(&zghz3484, INT64_C(129));
      UNDEFINED(lbits)(&zDDC_EL3, zghz3484);
    }
    unit zgsz3200;
    zgsz3200 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3483);
      CONVERT_OF(sail_int, mach_int)(&zghz3483, INT64_C(129));
      UNDEFINED(lbits)(&zRDDC_EL0, zghz3483);
    }
    unit zgsz3199;
    zgsz3199 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3481);
      CONVERT_OF(sail_int, mach_int)(&zghz3481, INT64_C(32));
      RECREATE(lbits)(&zghz3482);
      UNDEFINED(lbits)(&zghz3482, zghz3481);
      zDBGDTRTX_EL0 = CONVERT_OF(fbits, lbits)(zghz3482, true);
    }
    unit zgsz3198;
    zgsz3198 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3479);
      CONVERT_OF(sail_int, mach_int)(&zghz3479, INT64_C(64));
      RECREATE(lbits)(&zghz3480);
      UNDEFINED(lbits)(&zghz3480, zghz3479);
      zICC_ASGI1R_EL1 = CONVERT_OF(fbits, lbits)(zghz3480, true);
    }
    unit zgsz3197;
    zgsz3197 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3477);
      CONVERT_OF(sail_int, mach_int)(&zghz3477, INT64_C(32));
      RECREATE(lbits)(&zghz3478);
      UNDEFINED(lbits)(&zghz3478, zghz3477);
      zICC_DIR_EL1 = CONVERT_OF(fbits, lbits)(zghz3478, true);
    }
    unit zgsz3196;
    zgsz3196 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3475);
      CONVERT_OF(sail_int, mach_int)(&zghz3475, INT64_C(32));
      RECREATE(lbits)(&zghz3476);
      UNDEFINED(lbits)(&zghz3476, zghz3475);
      zICV_DIR_EL1 = CONVERT_OF(fbits, lbits)(zghz3476, true);
    }
    unit zgsz3195;
    zgsz3195 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3473);
      CONVERT_OF(sail_int, mach_int)(&zghz3473, INT64_C(32));
      RECREATE(lbits)(&zghz3474);
      UNDEFINED(lbits)(&zghz3474, zghz3473);
      zICC_EOIR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3474, true);
    }
    unit zgsz3194;
    zgsz3194 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3471);
      CONVERT_OF(sail_int, mach_int)(&zghz3471, INT64_C(32));
      RECREATE(lbits)(&zghz3472);
      UNDEFINED(lbits)(&zghz3472, zghz3471);
      zICV_EOIR0_EL1 = CONVERT_OF(fbits, lbits)(zghz3472, true);
    }
    unit zgsz3193;
    zgsz3193 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3469);
      CONVERT_OF(sail_int, mach_int)(&zghz3469, INT64_C(32));
      RECREATE(lbits)(&zghz3470);
      UNDEFINED(lbits)(&zghz3470, zghz3469);
      zICC_EOIR1_EL1 = CONVERT_OF(fbits, lbits)(zghz3470, true);
    }
    unit zgsz3192;
    zgsz3192 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3467);
      CONVERT_OF(sail_int, mach_int)(&zghz3467, INT64_C(32));
      RECREATE(lbits)(&zghz3468);
      UNDEFINED(lbits)(&zghz3468, zghz3467);
      zICV_EOIR1_EL1 = CONVERT_OF(fbits, lbits)(zghz3468, true);
    }
    unit zgsz3191;
    zgsz3191 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3465);
      CONVERT_OF(sail_int, mach_int)(&zghz3465, INT64_C(64));
      RECREATE(lbits)(&zghz3466);
      UNDEFINED(lbits)(&zghz3466, zghz3465);
      zICC_SGI0R_EL1 = CONVERT_OF(fbits, lbits)(zghz3466, true);
    }
    unit zgsz3190;
    zgsz3190 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3463);
      CONVERT_OF(sail_int, mach_int)(&zghz3463, INT64_C(64));
      RECREATE(lbits)(&zghz3464);
      UNDEFINED(lbits)(&zghz3464, zghz3463);
      zICC_SGI1R_EL1 = CONVERT_OF(fbits, lbits)(zghz3464, true);
    }
    unit zgsz3189;
    zgsz3189 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3461);
      CONVERT_OF(sail_int, mach_int)(&zghz3461, INT64_C(32));
      RECREATE(lbits)(&zghz3462);
      UNDEFINED(lbits)(&zghz3462, zghz3461);
      zOSLAR_EL1 = CONVERT_OF(fbits, lbits)(zghz3462, true);
    }
    unit zgsz3188;
    zgsz3188 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3459);
      CONVERT_OF(sail_int, mach_int)(&zghz3459, INT64_C(32));
      RECREATE(lbits)(&zghz3460);
      UNDEFINED(lbits)(&zghz3460, zghz3459);
      zPMSWINC_EL0 = CONVERT_OF(fbits, lbits)(zghz3460, true);
    }
    unit zgsz3187;
    zgsz3187 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3457);
      CONVERT_OF(sail_int, mach_int)(&zghz3457, INT64_C(1));
      RECREATE(lbits)(&zghz3458);
      UNDEFINED(lbits)(&zghz3458, zghz3457);
      zEventRegister = CONVERT_OF(fbits, lbits)(zghz3458, true);
    }
    unit zgsz3186;
    zgsz3186 = UNIT;
  }
  {
    RECREATE(lbits)(&zghz3454);
    {
      RECREATE(sail_int)(&zghz3456);
      CONVERT_OF(sail_int, mach_int)(&zghz3456, INT64_C(128));
      UNDEFINED(lbits)(&zghz3454, zghz3456);
    }
    {
      RECREATE(sail_int)(&zghz3455);
      CONVERT_OF(sail_int, mach_int)(&zghz3455, INT64_C(32));
      UNDEFINED(vector_zz5vecz8z5bvz9)(&z_V, zghz3455, zghz3454);
    }
    unit zgsz3185;
    zgsz3185 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3452);
      CONVERT_OF(sail_int, mach_int)(&zghz3452, INT64_C(2));
      RECREATE(lbits)(&zghz3453);
      UNDEFINED(lbits)(&zghz3453, zghz3452);
      z__saved_exception_level = CONVERT_OF(fbits, lbits)(zghz3453, true);
    }
    unit zgsz3184;
    zgsz3184 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3450);
      CONVERT_OF(sail_int, mach_int)(&zghz3450, INT64_C(48));
      RECREATE(lbits)(&zghz3451);
      UNDEFINED(lbits)(&zghz3451, zghz3450);
      z__CNTControlBase = CONVERT_OF(fbits, lbits)(zghz3451, true);
    }
    unit zgsz3183;
    zgsz3183 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3448);
      CONVERT_OF(sail_int, mach_int)(&zghz3448, INT64_C(32));
      RECREATE(lbits)(&zghz3449);
      UNDEFINED(lbits)(&zghz3449, zghz3448);
      z__ThisInstr = CONVERT_OF(fbits, lbits)(zghz3449, true);
    }
    unit zgsz3182;
    zgsz3182 = UNIT;
  }
  {
    z__ThisInstrEnc = sailgen_undefined___InstrEnc(UNIT);
    unit zgsz3181;
    zgsz3181 = UNIT;
  }
  {
    sailgen_undefined_instr_ast(&z__ThisInstrAbstract, UNIT);
    unit zgsz3180;
    zgsz3180 = UNIT;
  }
  {
    z__highest_el_aarch32 = false;
    unit zgsz3179;
    zgsz3179 = UNIT;
  }
  {
    {
      RECREATE(sail_int)(&zghz3447);
      CONVERT_OF(sail_int, mach_int)(&zghz3447, INT64_C(129));
      UNDEFINED(lbits)(&zSP_EL3, zghz3447);
    }
    unit zgsz3178;
    zgsz3178 = UNIT;
  }
  {
    RECREATE(sail_int)(&zghz3446);
    CONVERT_OF(sail_int, mach_int)(&zghz3446, INT64_C(129));
    UNDEFINED(lbits)(&zTestCap, zghz3446);
  }
  zcbz331 = UNIT;
end_function_232: ;
  return zcbz331;
end_block_exception_233: ;

  return UNIT;
}



static void finish_sailgen_initializze_registers(void)
{
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
  KILL(sail_int)(&zghz31088);
  KILL(sail_int)(&zghz31087);
  KILL(sail_int)(&zghz31086);
  KILL(sail_int)(&zghz31085);
  KILL(sail_int)(&zghz31084);
  KILL(sail_int)(&zghz31083);
  KILL(sail_int)(&zghz31082);
  KILL(sail_int)(&zghz31081);
  KILL(sail_int)(&zghz31080);
  KILL(sail_int)(&zghz31079);
  KILL(sail_int)(&zghz31078);
  KILL(sail_int)(&zghz31077);
  KILL(sail_int)(&zghz31076);
  KILL(sail_int)(&zghz31075);
  KILL(sail_int)(&zghz31074);
  KILL(sail_int)(&zghz31073);
  KILL(sail_int)(&zghz31072);
  KILL(sail_int)(&zghz31071);
  KILL(sail_int)(&zghz31070);
  KILL(sail_int)(&zghz31069);
  KILL(sail_int)(&zghz31068);
  KILL(sail_int)(&zghz31067);
  KILL(sail_int)(&zghz31066);
  KILL(sail_int)(&zghz31065);
  KILL(sail_int)(&zghz31064);
  KILL(sail_int)(&zghz31063);
  KILL(sail_int)(&zghz31062);
  KILL(sail_int)(&zghz31061);
  KILL(sail_int)(&zghz31060);
  KILL(sail_int)(&zghz31059);
  KILL(sail_int)(&zghz31058);
  KILL(lbits)(&zghz31057);
  KILL(sail_int)(&zghz31056);
  KILL(lbits)(&zghz31055);
  KILL(sail_int)(&zghz31054);
  KILL(lbits)(&zghz31053);
  KILL(sail_int)(&zghz31052);
  KILL(lbits)(&zghz31051);
  KILL(sail_int)(&zghz31050);
  KILL(lbits)(&zghz31049);
  KILL(sail_int)(&zghz31048);
  KILL(lbits)(&zghz31047);
  KILL(sail_int)(&zghz31046);
  KILL(lbits)(&zghz31045);
  KILL(sail_int)(&zghz31044);
  KILL(lbits)(&zghz31043);
  KILL(sail_int)(&zghz31042);
  KILL(lbits)(&zghz31041);
  KILL(sail_int)(&zghz31040);
  KILL(lbits)(&zghz31039);
  KILL(sail_int)(&zghz31038);
  KILL(lbits)(&zghz31037);
  KILL(sail_int)(&zghz31036);
  KILL(lbits)(&zghz31035);
  KILL(sail_int)(&zghz31034);
  KILL(lbits)(&zghz31033);
  KILL(sail_int)(&zghz31032);
  KILL(lbits)(&zghz31031);
  KILL(sail_int)(&zghz31030);
  KILL(lbits)(&zghz31029);
  KILL(sail_int)(&zghz31028);
  KILL(sail_int)(&zghz31027);
  KILL(sail_int)(&zghz31026);
  KILL(sail_int)(&zghz31025);
  KILL(sail_int)(&zghz31024);
  KILL(sail_int)(&zghz31023);
  KILL(sail_int)(&zghz31022);
  KILL(lbits)(&zghz31021);
  KILL(sail_int)(&zghz31020);
  KILL(lbits)(&zghz31019);
  KILL(sail_int)(&zghz31018);
  KILL(lbits)(&zghz31017);
  KILL(sail_int)(&zghz31016);
  KILL(lbits)(&zghz31015);
  KILL(sail_int)(&zghz31014);
  KILL(lbits)(&zghz31013);
  KILL(sail_int)(&zghz31012);
  KILL(lbits)(&zghz31011);
  KILL(sail_int)(&zghz31010);
  KILL(lbits)(&zghz31009);
  KILL(sail_int)(&zghz31008);
  KILL(lbits)(&zghz31007);
  KILL(sail_int)(&zghz31006);
  KILL(lbits)(&zghz31005);
  KILL(sail_int)(&zghz31004);
  KILL(lbits)(&zghz31003);
  KILL(sail_int)(&zghz31002);
  KILL(lbits)(&zghz31001);
  KILL(sail_int)(&zghz31000);
  KILL(lbits)(&zghz3999);
  KILL(sail_int)(&zghz3998);
  KILL(lbits)(&zghz3997);
  KILL(sail_int)(&zghz3996);
  KILL(lbits)(&zghz3995);
  KILL(sail_int)(&zghz3994);
  KILL(lbits)(&zghz3993);
  KILL(sail_int)(&zghz3992);
  KILL(lbits)(&zghz3991);
  KILL(sail_int)(&zghz3990);
  KILL(lbits)(&zghz3989);
  KILL(sail_int)(&zghz3988);
  KILL(lbits)(&zghz3987);
  KILL(sail_int)(&zghz3986);
  KILL(lbits)(&zghz3985);
  KILL(sail_int)(&zghz3984);
  KILL(lbits)(&zghz3983);
  KILL(sail_int)(&zghz3982);
  KILL(lbits)(&zghz3981);
  KILL(sail_int)(&zghz3980);
  KILL(lbits)(&zghz3979);
  KILL(sail_int)(&zghz3978);
  KILL(lbits)(&zghz3977);
  KILL(sail_int)(&zghz3976);
  KILL(lbits)(&zghz3975);
  KILL(sail_int)(&zghz3974);
  KILL(lbits)(&zghz3973);
  KILL(sail_int)(&zghz3972);
  KILL(lbits)(&zghz3971);
  KILL(sail_int)(&zghz3970);
  KILL(lbits)(&zghz3969);
  KILL(sail_int)(&zghz3968);
  KILL(lbits)(&zghz3967);
  KILL(sail_int)(&zghz3966);
  KILL(lbits)(&zghz3965);
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
  KILL(lbits)(&zghz3951);
  KILL(sail_int)(&zghz3950);
  KILL(lbits)(&zghz3949);
  KILL(sail_int)(&zghz3948);
  KILL(lbits)(&zghz3947);
  KILL(sail_int)(&zghz3946);
  KILL(lbits)(&zghz3945);
  KILL(sail_int)(&zghz3944);
  KILL(lbits)(&zghz3943);
  KILL(sail_int)(&zghz3942);
  KILL(lbits)(&zghz3941);
  KILL(sail_int)(&zghz3940);
  KILL(lbits)(&zghz3939);
  KILL(sail_int)(&zghz3938);
  KILL(lbits)(&zghz3937);
  KILL(sail_int)(&zghz3936);
  KILL(lbits)(&zghz3935);
  KILL(sail_int)(&zghz3934);
  KILL(lbits)(&zghz3933);
  KILL(sail_int)(&zghz3932);
  KILL(lbits)(&zghz3931);
  KILL(sail_int)(&zghz3930);
  KILL(lbits)(&zghz3929);
  KILL(sail_int)(&zghz3928);
  KILL(lbits)(&zghz3927);
  KILL(sail_int)(&zghz3926);
  KILL(lbits)(&zghz3925);
  KILL(sail_int)(&zghz3924);
  KILL(lbits)(&zghz3923);
  KILL(sail_int)(&zghz3922);
  KILL(lbits)(&zghz3921);
  KILL(sail_int)(&zghz3920);
  KILL(lbits)(&zghz3919);
  KILL(sail_int)(&zghz3918);
  KILL(lbits)(&zghz3917);
  KILL(sail_int)(&zghz3916);
  KILL(lbits)(&zghz3915);
  KILL(sail_int)(&zghz3914);
  KILL(lbits)(&zghz3913);
  KILL(sail_int)(&zghz3912);
  KILL(lbits)(&zghz3911);
  KILL(sail_int)(&zghz3910);
  KILL(lbits)(&zghz3909);
  KILL(sail_int)(&zghz3908);
  KILL(sail_int)(&zghz3907);
  KILL(sail_int)(&zghz3906);
  KILL(lbits)(&zghz3905);
  KILL(sail_int)(&zghz3904);
  KILL(lbits)(&zghz3903);
  KILL(sail_int)(&zghz3902);
  KILL(lbits)(&zghz3901);
  KILL(sail_int)(&zghz3900);
  KILL(lbits)(&zghz3899);
  KILL(sail_int)(&zghz3898);
  KILL(lbits)(&zghz3897);
  KILL(sail_int)(&zghz3896);
  KILL(sail_int)(&zghz3895);
  KILL(lbits)(&zghz3894);
  KILL(sail_int)(&zghz3893);
  KILL(lbits)(&zghz3892);
  KILL(sail_int)(&zghz3891);
  KILL(lbits)(&zghz3890);
  KILL(sail_int)(&zghz3889);
  KILL(lbits)(&zghz3888);
  KILL(sail_int)(&zghz3887);
  KILL(sail_int)(&zghz3886);
  KILL(lbits)(&zghz3885);
  KILL(sail_int)(&zghz3884);
  KILL(lbits)(&zghz3883);
  KILL(sail_int)(&zghz3882);
  KILL(sail_int)(&zghz3881);
  KILL(lbits)(&zghz3880);
  KILL(sail_int)(&zghz3879);
  KILL(sail_int)(&zghz3878);
  KILL(lbits)(&zghz3877);
  KILL(sail_int)(&zghz3876);
  KILL(lbits)(&zghz3875);
  KILL(sail_int)(&zghz3874);
  KILL(lbits)(&zghz3873);
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
  KILL(sail_int)(&zghz3843);
  KILL(lbits)(&zghz3842);
  KILL(sail_int)(&zghz3841);
  KILL(lbits)(&zghz3840);
  KILL(sail_int)(&zghz3839);
  KILL(lbits)(&zghz3838);
  KILL(sail_int)(&zghz3837);
  KILL(lbits)(&zghz3836);
  KILL(sail_int)(&zghz3835);
  KILL(lbits)(&zghz3834);
  KILL(sail_int)(&zghz3833);
  KILL(lbits)(&zghz3832);
  KILL(sail_int)(&zghz3831);
  KILL(sail_int)(&zghz3830);
  KILL(lbits)(&zghz3829);
  KILL(sail_int)(&zghz3828);
  KILL(sail_int)(&zghz3827);
  KILL(lbits)(&zghz3826);
  KILL(sail_int)(&zghz3825);
  KILL(sail_int)(&zghz3824);
  KILL(lbits)(&zghz3823);
  KILL(sail_int)(&zghz3822);
  KILL(sail_int)(&zghz3821);
  KILL(lbits)(&zghz3820);
  KILL(sail_int)(&zghz3819);
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
  KILL(sail_int)(&zghz3761);
  KILL(lbits)(&zghz3760);
  KILL(sail_int)(&zghz3759);
  KILL(sail_int)(&zghz3758);
  KILL(lbits)(&zghz3757);
  KILL(sail_int)(&zghz3756);
  KILL(lbits)(&zghz3755);
  KILL(sail_int)(&zghz3754);
  KILL(lbits)(&zghz3753);
  KILL(sail_int)(&zghz3752);
  KILL(sail_int)(&zghz3751);
  KILL(lbits)(&zghz3750);
  KILL(sail_int)(&zghz3749);
  KILL(lbits)(&zghz3748);
  KILL(sail_int)(&zghz3747);
  KILL(lbits)(&zghz3746);
  KILL(sail_int)(&zghz3745);
  KILL(lbits)(&zghz3744);
  KILL(sail_int)(&zghz3743);
  KILL(lbits)(&zghz3742);
  KILL(sail_int)(&zghz3741);
  KILL(lbits)(&zghz3740);
  KILL(sail_int)(&zghz3739);
  KILL(lbits)(&zghz3738);
  KILL(sail_int)(&zghz3737);
  KILL(lbits)(&zghz3736);
  KILL(sail_int)(&zghz3735);
  KILL(lbits)(&zghz3734);
  KILL(sail_int)(&zghz3733);
  KILL(lbits)(&zghz3732);
  KILL(sail_int)(&zghz3731);
  KILL(lbits)(&zghz3730);
  KILL(sail_int)(&zghz3729);
  KILL(lbits)(&zghz3728);
  KILL(sail_int)(&zghz3727);
  KILL(lbits)(&zghz3726);
  KILL(sail_int)(&zghz3725);
  KILL(lbits)(&zghz3724);
  KILL(sail_int)(&zghz3723);
  KILL(lbits)(&zghz3722);
  KILL(sail_int)(&zghz3721);
  KILL(lbits)(&zghz3720);
  KILL(sail_int)(&zghz3719);
  KILL(lbits)(&zghz3718);
  KILL(sail_int)(&zghz3717);
  KILL(lbits)(&zghz3716);
  KILL(sail_int)(&zghz3715);
  KILL(lbits)(&zghz3714);
  KILL(sail_int)(&zghz3713);
  KILL(lbits)(&zghz3712);
  KILL(sail_int)(&zghz3711);
  KILL(lbits)(&zghz3710);
  KILL(sail_int)(&zghz3709);
  KILL(lbits)(&zghz3708);
  KILL(sail_int)(&zghz3707);
  KILL(lbits)(&zghz3706);
  KILL(sail_int)(&zghz3705);
  KILL(lbits)(&zghz3704);
  KILL(sail_int)(&zghz3703);
  KILL(lbits)(&zghz3702);
  KILL(sail_int)(&zghz3701);
  KILL(lbits)(&zghz3700);
  KILL(sail_int)(&zghz3699);
  KILL(lbits)(&zghz3698);
  KILL(sail_int)(&zghz3697);
  KILL(lbits)(&zghz3696);
  KILL(sail_int)(&zghz3695);
  KILL(lbits)(&zghz3694);
  KILL(sail_int)(&zghz3693);
  KILL(lbits)(&zghz3692);
  KILL(sail_int)(&zghz3691);
  KILL(lbits)(&zghz3690);
  KILL(sail_int)(&zghz3689);
  KILL(lbits)(&zghz3688);
  KILL(sail_int)(&zghz3687);
  KILL(lbits)(&zghz3686);
  KILL(sail_int)(&zghz3685);
  KILL(lbits)(&zghz3684);
  KILL(sail_int)(&zghz3683);
  KILL(lbits)(&zghz3682);
  KILL(sail_int)(&zghz3681);
  KILL(lbits)(&zghz3680);
  KILL(sail_int)(&zghz3679);
  KILL(lbits)(&zghz3678);
  KILL(sail_int)(&zghz3677);
  KILL(lbits)(&zghz3676);
  KILL(sail_int)(&zghz3675);
  KILL(lbits)(&zghz3674);
  KILL(sail_int)(&zghz3673);
  KILL(lbits)(&zghz3672);
  KILL(sail_int)(&zghz3671);
  KILL(lbits)(&zghz3670);
  KILL(sail_int)(&zghz3669);
  KILL(lbits)(&zghz3668);
  KILL(sail_int)(&zghz3667);
  KILL(lbits)(&zghz3666);
  KILL(sail_int)(&zghz3665);
  KILL(lbits)(&zghz3664);
  KILL(sail_int)(&zghz3663);
  KILL(lbits)(&zghz3662);
  KILL(sail_int)(&zghz3661);
  KILL(lbits)(&zghz3660);
  KILL(sail_int)(&zghz3659);
  KILL(lbits)(&zghz3658);
  KILL(sail_int)(&zghz3657);
  KILL(lbits)(&zghz3656);
  KILL(sail_int)(&zghz3655);
  KILL(lbits)(&zghz3654);
  KILL(sail_int)(&zghz3653);
  KILL(lbits)(&zghz3652);
  KILL(sail_int)(&zghz3651);
  KILL(lbits)(&zghz3650);
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
  KILL(lbits)(&zghz3632);
  KILL(sail_int)(&zghz3631);
  KILL(lbits)(&zghz3630);
  KILL(sail_int)(&zghz3629);
  KILL(lbits)(&zghz3628);
  KILL(sail_int)(&zghz3627);
  KILL(lbits)(&zghz3626);
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
  KILL(lbits)(&zghz3608);
  KILL(sail_int)(&zghz3607);
  KILL(lbits)(&zghz3606);
  KILL(sail_int)(&zghz3605);
  KILL(lbits)(&zghz3604);
  KILL(sail_int)(&zghz3603);
  KILL(lbits)(&zghz3602);
  KILL(sail_int)(&zghz3601);
  KILL(lbits)(&zghz3600);
  KILL(sail_int)(&zghz3599);
  KILL(lbits)(&zghz3598);
  KILL(sail_int)(&zghz3597);
  KILL(lbits)(&zghz3596);
  KILL(sail_int)(&zghz3595);
  KILL(lbits)(&zghz3594);
  KILL(sail_int)(&zghz3593);
  KILL(lbits)(&zghz3592);
  KILL(sail_int)(&zghz3591);
  KILL(lbits)(&zghz3590);
  KILL(sail_int)(&zghz3589);
  KILL(lbits)(&zghz3588);
  KILL(sail_int)(&zghz3587);
  KILL(lbits)(&zghz3586);
  KILL(sail_int)(&zghz3585);
  KILL(lbits)(&zghz3584);
  KILL(sail_int)(&zghz3583);
  KILL(lbits)(&zghz3582);
  KILL(sail_int)(&zghz3581);
  KILL(sail_int)(&zghz3580);
  KILL(lbits)(&zghz3579);
  KILL(sail_int)(&zghz3578);
  KILL(sail_int)(&zghz3577);
  KILL(lbits)(&zghz3576);
  KILL(sail_int)(&zghz3575);
  KILL(lbits)(&zghz3574);
  KILL(sail_int)(&zghz3573);
  KILL(lbits)(&zghz3572);
  KILL(sail_int)(&zghz3571);
  KILL(lbits)(&zghz3570);
  KILL(sail_int)(&zghz3569);
  KILL(lbits)(&zghz3568);
  KILL(sail_int)(&zghz3567);
  KILL(lbits)(&zghz3566);
  KILL(sail_int)(&zghz3565);
  KILL(lbits)(&zghz3564);
  KILL(sail_int)(&zghz3563);
  KILL(lbits)(&zghz3562);
  KILL(sail_int)(&zghz3561);
  KILL(lbits)(&zghz3560);
  KILL(sail_int)(&zghz3559);
  KILL(lbits)(&zghz3558);
  KILL(sail_int)(&zghz3557);
  KILL(lbits)(&zghz3556);
  KILL(sail_int)(&zghz3555);
  KILL(lbits)(&zghz3554);
  KILL(sail_int)(&zghz3553);
  KILL(lbits)(&zghz3552);
  KILL(sail_int)(&zghz3551);
  KILL(lbits)(&zghz3550);
  KILL(sail_int)(&zghz3549);
  KILL(lbits)(&zghz3548);
  KILL(sail_int)(&zghz3547);
  KILL(lbits)(&zghz3546);
  KILL(sail_int)(&zghz3545);
  KILL(lbits)(&zghz3544);
  KILL(sail_int)(&zghz3543);
  KILL(lbits)(&zghz3542);
  KILL(sail_int)(&zghz3541);
  KILL(lbits)(&zghz3540);
  KILL(sail_int)(&zghz3539);
  KILL(sail_int)(&zghz3538);
  KILL(sail_int)(&zghz3537);
  KILL(lbits)(&zghz3536);
  KILL(sail_int)(&zghz3535);
  KILL(lbits)(&zghz3534);
  KILL(sail_int)(&zghz3533);
  KILL(lbits)(&zghz3532);
  KILL(sail_int)(&zghz3531);
  KILL(lbits)(&zghz3530);
  KILL(sail_int)(&zghz3529);
  KILL(sail_int)(&zghz3528);
  KILL(lbits)(&zghz3527);
  KILL(sail_int)(&zghz3526);
  KILL(lbits)(&zghz3525);
  KILL(sail_int)(&zghz3524);
  KILL(lbits)(&zghz3523);
  KILL(sail_int)(&zghz3522);
  KILL(lbits)(&zghz3521);
  KILL(sail_int)(&zghz3520);
  KILL(lbits)(&zghz3519);
  KILL(sail_int)(&zghz3518);
  KILL(lbits)(&zghz3517);
  KILL(sail_int)(&zghz3516);
  KILL(lbits)(&zghz3515);
  KILL(sail_int)(&zghz3514);
  KILL(lbits)(&zghz3513);
  KILL(sail_int)(&zghz3512);
  KILL(sail_int)(&zghz3511);
  KILL(sail_int)(&zghz3510);
  KILL(sail_int)(&zghz3509);
  KILL(sail_int)(&zghz3508);
  KILL(sail_int)(&zghz3507);
  KILL(sail_int)(&zghz3506);
  KILL(sail_int)(&zghz3505);
  KILL(sail_int)(&zghz3504);
  KILL(lbits)(&zghz3503);
  KILL(sail_int)(&zghz3502);
  KILL(lbits)(&zghz3501);
  KILL(sail_int)(&zghz3500);
  KILL(lbits)(&zghz3499);
  KILL(sail_int)(&zghz3498);
  KILL(lbits)(&zghz3497);
  KILL(sail_int)(&zghz3496);
  KILL(lbits)(&zghz3495);
  KILL(sail_int)(&zghz3494);
  KILL(lbits)(&zghz3493);
  KILL(sail_int)(&zghz3492);
  KILL(lbits)(&zghz3491);
  KILL(sail_int)(&zghz3490);
  KILL(lbits)(&zghz3489);
  KILL(sail_int)(&zghz3488);
  KILL(sail_int)(&zghz3487);
  KILL(sail_int)(&zghz3486);
  KILL(sail_int)(&zghz3485);
  KILL(sail_int)(&zghz3484);
  KILL(sail_int)(&zghz3483);
  KILL(lbits)(&zghz3482);
  KILL(sail_int)(&zghz3481);
  KILL(lbits)(&zghz3480);
  KILL(sail_int)(&zghz3479);
  KILL(lbits)(&zghz3478);
  KILL(sail_int)(&zghz3477);
  KILL(lbits)(&zghz3476);
  KILL(sail_int)(&zghz3475);
  KILL(lbits)(&zghz3474);
  KILL(sail_int)(&zghz3473);
  KILL(lbits)(&zghz3472);
  KILL(sail_int)(&zghz3471);
  KILL(lbits)(&zghz3470);
  KILL(sail_int)(&zghz3469);
  KILL(lbits)(&zghz3468);
  KILL(sail_int)(&zghz3467);
  KILL(lbits)(&zghz3466);
  KILL(sail_int)(&zghz3465);
  KILL(lbits)(&zghz3464);
  KILL(sail_int)(&zghz3463);
  KILL(lbits)(&zghz3462);
  KILL(sail_int)(&zghz3461);
  KILL(lbits)(&zghz3460);
  KILL(sail_int)(&zghz3459);
  KILL(lbits)(&zghz3458);
  KILL(sail_int)(&zghz3457);
  KILL(sail_int)(&zghz3456);
  KILL(sail_int)(&zghz3455);
  KILL(lbits)(&zghz3454);
  KILL(lbits)(&zghz3453);
  KILL(sail_int)(&zghz3452);
  KILL(lbits)(&zghz3451);
  KILL(sail_int)(&zghz3450);
  KILL(lbits)(&zghz3449);
  KILL(sail_int)(&zghz3448);
  KILL(sail_int)(&zghz3447);
  KILL(sail_int)(&zghz3446);
}

static void model_init(void)
{
  setup_rts();
  current_exception = sail_malloc(sizeof(struct zexception));
  CREATE(zexception)(current_exception);
  throw_location = sail_malloc(sizeof(sail_string));
  CREATE(sail_string)(throw_location);
  startup_sailgen_sail_ones();
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
  startup_sailgen_CapGetObjectType();
  startup_sailgen_CapGetPermissions();
  startup_sailgen_undefined_instr_ast();
  startup_sailgen_CapUnsignedGreaterThanOrEqual();
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
  finish_sailgen_CapGetObjectType();
  finish_sailgen_CapGetPermissions();
  finish_sailgen_undefined_instr_ast();
  finish_sailgen_CapUnsignedGreaterThanOrEqual();
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


