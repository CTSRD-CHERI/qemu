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

// enum read_kind
enum zread_kind { zRead_plain, zRead_reserve, zRead_acquire, zRead_exclusive, zRead_exclusive_acquire, zRead_stream, zRead_ifetch, zRead_RISCV_acquire, zRead_RISCV_strong_acquire, zRead_RISCV_reserved, zRead_RISCV_reserved_acquire, zRead_RISCV_reserved_strong_acquire, zRead_X86_locked };

static bool eq_zread_kind(enum zread_kind op1, enum zread_kind op2) { return op1 == op2; }

static enum zread_kind UNDEFINED(zread_kind)(unit u) { return zRead_plain; }

// union exception
enum kind_zexception { Kind_z__dummy_exnz3 };

struct zexception {
  enum kind_zexception kind;
  union {struct { unit z__dummy_exnz3; };};
};

static void CREATE(zexception)(struct zexception *op)
{
  op->kind = Kind_z__dummy_exnz3;

}

static void RECREATE(zexception)(struct zexception *op) {}

static void KILL(zexception)(struct zexception *op)
{
  if (op->kind == Kind_z__dummy_exnz3){
    /* do nothing */
  };
}

static void COPY(zexception)(struct zexception *rop, struct zexception op)
{
  if (rop->kind == Kind_z__dummy_exnz3){
    /* do nothing */
  };
  rop->kind = op.kind;
  if (op.kind == Kind_z__dummy_exnz3){
    rop->z__dummy_exnz3 = op.z__dummy_exnz3;
  }
}

static bool EQUAL(zexception)(struct zexception op1, struct zexception op2) {
  if (op1.kind == Kind_z__dummy_exnz3 && op2.kind == Kind_z__dummy_exnz3) {
    return EQUAL(unit)(op1.z__dummy_exnz3, op2.z__dummy_exnz3);
  } else return false;
}

static void sailgen___dummy_exnz3(struct zexception *rop, unit op)
{
  if (rop->kind == Kind_z__dummy_exnz3){
    /* do nothing */
  }
  rop->kind = Kind_z__dummy_exnz3;
  rop->z__dummy_exnz3 = op;
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

// struct EncCapability
struct zEncCapability {
  uint64_t zB;
  uint64_t zT;
  uint64_t zaddress;
  uint64_t zflags;
  fbits zinternal_E;
  uint64_t zotype;
  uint64_t zperms;
  uint64_t zreserved;
};

static void COPY(zEncCapability)(struct zEncCapability *rop, const struct zEncCapability op) {
  rop->zB = op.zB;
  rop->zT = op.zT;
  rop->zaddress = op.zaddress;
  rop->zflags = op.zflags;
  rop->zinternal_E = op.zinternal_E;
  rop->zotype = op.zotype;
  rop->zperms = op.zperms;
  rop->zreserved = op.zreserved;
}

static bool EQUAL(zEncCapability)(struct zEncCapability op1, struct zEncCapability op2) {
  return EQUAL(fbits)(op1.zB, op2.zB) && EQUAL(fbits)(op1.zT, op2.zT) && EQUAL(fbits)(op1.zaddress, op2.zaddress) && EQUAL(fbits)(op1.zflags, op2.zflags) && EQUAL(fbits)(op1.zinternal_E, op2.zinternal_E) && EQUAL(fbits)(op1.zotype, op2.zotype) && EQUAL(fbits)(op1.zperms, op2.zperms) && EQUAL(fbits)(op1.zreserved, op2.zreserved);
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
  bool zinternal_E;
  uint64_t zotype;
  bool zpermit_cinvoke;
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
  rop->zinternal_E = op.zinternal_E;
  rop->zotype = op.zotype;
  rop->zpermit_cinvoke = op.zpermit_cinvoke;
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
  rop->ztag = op.ztag;
  rop->zuperms = op.zuperms;
}

static bool EQUAL(zCapability)(struct zCapability op1, struct zCapability op2) {
  return EQUAL(fbits)(op1.zB, op2.zB) && EQUAL(fbits)(op1.zE, op2.zE) && EQUAL(fbits)(op1.zT, op2.zT) && EQUAL(bool)(op1.zaccess_system_regs, op2.zaccess_system_regs) && EQUAL(fbits)(op1.zaddress, op2.zaddress) && EQUAL(bool)(op1.zflag_cap_mode, op2.zflag_cap_mode) && EQUAL(bool)(op1.zglobal, op2.zglobal) && EQUAL(bool)(op1.zinternal_E, op2.zinternal_E) && EQUAL(fbits)(op1.zotype, op2.zotype) && EQUAL(bool)(op1.zpermit_cinvoke, op2.zpermit_cinvoke) && EQUAL(bool)(op1.zpermit_execute, op2.zpermit_execute) && EQUAL(bool)(op1.zpermit_load, op2.zpermit_load) && EQUAL(bool)(op1.zpermit_load_cap, op2.zpermit_load_cap) && EQUAL(bool)(op1.zpermit_seal, op2.zpermit_seal) && EQUAL(bool)(op1.zpermit_set_CID, op2.zpermit_set_CID) && EQUAL(bool)(op1.zpermit_store, op2.zpermit_store) && EQUAL(bool)(op1.zpermit_store_cap, op2.zpermit_store_cap) && EQUAL(bool)(op1.zpermit_store_local_cap, op2.zpermit_store_local_cap) && EQUAL(bool)(op1.zpermit_unseal, op2.zpermit_unseal) && EQUAL(fbits)(op1.zreserved, op2.zreserved) && EQUAL(bool)(op1.ztag, op2.ztag) && EQUAL(fbits)(op1.zuperms, op2.zuperms);
}

// enum CapEx
enum zCapEx { zCapEx_None, zCapEx_LengthViolation, zCapEx_TagViolation, zCapEx_SealViolation, zCapEx_TypeViolation, zCapEx_UserDefViolation, zCapEx_UnalignedBase, zCapEx_GlobalViolation, zCapEx_PermitExecuteViolation, zCapEx_PermitLoadViolation, zCapEx_PermitStoreViolation, zCapEx_PermitLoadCapViolation, zCapEx_PermitStoreCapViolation, zCapEx_PermitStoreLocalCapViolation, zCapEx_AccessSystemRegsViolation, zCapEx_PermitCInvokeViolation, zCapEx_PermitSetCIDViolation };

static bool eq_zCapEx(enum zCapEx op1, enum zCapEx op2) { return op1 == op2; }

static enum zCapEx UNDEFINED(zCapEx)(unit u) { return zCapEx_None; }

// enum CPtrCmpOp
enum zCPtrCmpOp { zCEQ, zCNE, zCLT, zCLE, zCLTU, zCLEU, zCEXEQ, zCNEXEQ };

static bool eq_zCPtrCmpOp(enum zCPtrCmpOp op1, enum zCPtrCmpOp op2) { return op1 == op2; }

static enum zCPtrCmpOp UNDEFINED(zCPtrCmpOp)(unit u) { return zCEQ; }

















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

static void sailgen_sail_ones(lbits *zcbz32, sail_int zn)
{
  __label__ end_function_7, end_block_exception_8, end_function_183;

  lbits zgaz32;
  CREATE(lbits)(&zgaz32);
  zeros(&zgaz32, zn);
  not_bits((*(&zcbz32)), zgaz32);
  KILL(lbits)(&zgaz32);
end_function_7: ;
  goto end_function_183;
end_block_exception_8: ;
  goto end_function_183;
end_function_183: ;
}







static fbits sailgen_not_bit(fbits);

static fbits sailgen_not_bit(fbits zb)
{
  __label__ end_function_10, end_block_exception_11;

  fbits zcbz33;
  bool zgaz33;
  zgaz33 = eq_bit(zb, UINT64_C(1));
  if (zgaz33) {  zcbz33 = UINT64_C(0);  } else {  zcbz33 = UINT64_C(1);  }

end_function_10: ;
  return zcbz33;
end_block_exception_11: ;

  return UINT64_C(0);
}













static void sailgen_EXTS(lbits *rop, sail_int, lbits);

static void sailgen_EXTZ(lbits *rop, sail_int, lbits);

static void sailgen_EXTS(lbits *zcbz34, sail_int zm, lbits zv)
{
  __label__ end_function_13, end_block_exception_14, end_function_182;

  sign_extend((*(&zcbz34)), zv, zm);
end_function_13: ;
  goto end_function_182;
end_block_exception_14: ;
  goto end_function_182;
end_function_182: ;
}

static void sailgen_EXTZ(lbits *zcbz35, sail_int zm, lbits zv)
{
  __label__ end_function_16, end_block_exception_17, end_function_181;

  zero_extend((*(&zcbz35)), zv, zm);
end_function_16: ;
  goto end_function_181;
end_block_exception_17: ;
  goto end_function_181;
end_function_181: ;
}

static void sailgen_zzeros_implicit(lbits *rop, sail_int);

static void sailgen_zzeros_implicit(lbits *zcbz36, sail_int zn)
{
  __label__ end_function_19, end_block_exception_20, end_function_180;

  zeros((*(&zcbz36)), zn);
end_function_19: ;
  goto end_function_180;
end_block_exception_20: ;
  goto end_function_180;
end_function_180: ;
}

static void sailgen_ones(lbits *rop, sail_int);

static void sailgen_ones(lbits *zcbz37, sail_int zn)
{
  __label__ end_function_22, end_block_exception_23, end_function_179;

  sailgen_sail_ones((*(&zcbz37)), zn);
end_function_22: ;
  goto end_function_179;
end_block_exception_23: ;
  goto end_function_179;
end_function_179: ;
}

static uint64_t sailgen_bool_to_bits(bool);

static uint64_t sailgen_bool_to_bits(bool zx)
{
  __label__ end_function_25, end_block_exception_26;

  uint64_t zcbz38;
  if (zx) {  zcbz38 = UINT64_C(0b1);  } else {  zcbz38 = UINT64_C(0b0);  }
end_function_25: ;
  return zcbz38;
end_block_exception_26: ;

  return UINT64_C(0xdeadc0de);
}

static bool sailgen_bit_to_bool(fbits);

static bool sailgen_bit_to_bool(fbits zb)
{
  __label__ case_29, case_30, finish_match_28, end_function_31, end_block_exception_32;

  bool zcbz39;
  bool zgsz30;
  {
    fbits zp0z3;
    zp0z3 = zb;
    bool zgsz31;
    zgsz31 = eq_bit(zp0z3, UINT64_C(1));
    if (!(zgsz31)) {

      goto case_29;
    }
    zgsz30 = true;
    goto finish_match_28;
  }
case_29: ;
  {
    fbits zuz30;
    zuz30 = zb;
    bool zgsz32;
    zgsz32 = eq_bit(zuz30, UINT64_C(0));
    if (!(zgsz32)) {

      goto case_30;
    }
    zgsz30 = false;
    goto finish_match_28;
  }
case_30: ;
  sail_match_failure("bit_to_bool");
finish_match_28: ;
  zcbz39 = zgsz30;

end_function_31: ;
  return zcbz39;
end_block_exception_32: ;

  return false;
}

static void sailgen_to_bits(lbits *rop, sail_int, sail_int);

static void sailgen_to_bits(lbits *zcbz310, sail_int zl, sail_int zn)
{
  __label__ end_function_34, end_block_exception_35, end_function_178;

  {
    sail_int zgsz389;
    CREATE(sail_int)(&zgsz389);
    CONVERT_OF(sail_int, mach_int)(&zgsz389, INT64_C(0));
    get_slice_int((*(&zcbz310)), zl, zn, zgsz389);
    KILL(sail_int)(&zgsz389);
  }
end_function_34: ;
  goto end_function_178;
end_block_exception_35: ;
  goto end_function_178;
end_function_178: ;
}

static bool sailgen_z8operatorz0zI_uz9(lbits, lbits);

static bool sailgen_z8operatorz0zKzJ_uz9(lbits, lbits);

static bool sailgen_z8operatorz0zI_uz9(lbits zx, lbits zy)
{
  __label__ end_function_37, end_block_exception_38;

  bool zcbz311;
  sail_int zgaz34;
  CREATE(sail_int)(&zgaz34);
  sail_unsigned(&zgaz34, zx);
  sail_int zgaz35;
  CREATE(sail_int)(&zgaz35);
  sail_unsigned(&zgaz35, zy);
  zcbz311 = lt(zgaz34, zgaz35);
  KILL(sail_int)(&zgaz35);
  KILL(sail_int)(&zgaz34);
end_function_37: ;
  return zcbz311;
end_block_exception_38: ;

  return false;
}

static bool sailgen_z8operatorz0zKzJ_uz9(lbits zx, lbits zy)
{
  __label__ end_function_40, end_block_exception_41;

  bool zcbz312;
  sail_int zgaz36;
  CREATE(sail_int)(&zgaz36);
  sail_unsigned(&zgaz36, zx);
  sail_int zgaz37;
  CREATE(sail_int)(&zgaz37);
  sail_unsigned(&zgaz37, zy);
  zcbz312 = gteq(zgaz36, zgaz37);
  KILL(sail_int)(&zgaz37);
  KILL(sail_int)(&zgaz36);
end_function_40: ;
  return zcbz312;
end_block_exception_41: ;

  return false;
}



static void sailgen_MAX(sail_int *rop, sail_int);

static void sailgen_MAX(sail_int *zcbz313, sail_int zn)
{
  __label__ end_function_43, end_block_exception_44, end_function_177;

  sail_int zgaz38;
  CREATE(sail_int)(&zgaz38);
  pow2(&zgaz38, zn);
  {
    sail_int zgsz390;
    CREATE(sail_int)(&zgsz390);
    CONVERT_OF(sail_int, mach_int)(&zgsz390, INT64_C(1));
    sub_int((*(&zcbz313)), zgaz38, zgsz390);
    KILL(sail_int)(&zgsz390);
  }
  KILL(sail_int)(&zgaz38);
end_function_43: ;
  goto end_function_177;
end_block_exception_44: ;
  goto end_function_177;
end_function_177: ;
}



static int64_t zreserved_otypes;
static void create_letbind_0(void) {


  int64_t zgsz33;
  zgsz33 = INT64_C(4);
  zreserved_otypes = zgsz33;

let_end_45: ;
}
static void kill_letbind_0(void) {
}

static int64_t zotype_unsealed;
static void create_letbind_1(void) {


  int64_t zgsz34;
  zgsz34 = INT64_C(-1);
  zotype_unsealed = zgsz34;

let_end_46: ;
}
static void kill_letbind_1(void) {
}

static int64_t zotype_sentry;
static void create_letbind_2(void) {


  int64_t zgsz35;
  zgsz35 = INT64_C(-2);
  zotype_sentry = zgsz35;

let_end_47: ;
}
static void kill_letbind_2(void) {
}

static uint64_t zPCC_IDX;
static void create_letbind_3(void) {


  uint64_t zgsz36;
  zgsz36 = UINT64_C(0b100000);
  zPCC_IDX = zgsz36;

let_end_48: ;
}
static void kill_letbind_3(void) {
}

static uint64_t zDDC_IDX;
static void create_letbind_4(void) {


  uint64_t zgsz37;
  zgsz37 = UINT64_C(0b100001);
  zDDC_IDX = zgsz37;

let_end_49: ;
}
static void kill_letbind_4(void) {
}

static int64_t zcap_sizze;
static void create_letbind_5(void) {


  int64_t zgsz38;
  zgsz38 = INT64_C(16);
  zcap_sizze = zgsz38;

let_end_50: ;
}
static void kill_letbind_5(void) {
}

static int64_t zlog2_cap_sizze;
static void create_letbind_6(void) {


  int64_t zgsz39;
  zgsz39 = INT64_C(4);
  zlog2_cap_sizze = zgsz39;

let_end_51: ;
}
static void kill_letbind_6(void) {
}

static int64_t zcap_hperms_width;
static void create_letbind_7(void) {


  int64_t zgsz310;
  zgsz310 = INT64_C(12);
  zcap_hperms_width = zgsz310;

let_end_52: ;
}
static void kill_letbind_7(void) {
}

static int64_t zcap_uperms_width;
static void create_letbind_8(void) {


  int64_t zgsz311;
  zgsz311 = INT64_C(4);
  zcap_uperms_width = zgsz311;

let_end_53: ;
}
static void kill_letbind_8(void) {
}

static int64_t zcap_otype_width;
static void create_letbind_9(void) {


  int64_t zgsz312;
  zgsz312 = INT64_C(18);
  zcap_otype_width = zgsz312;

let_end_54: ;
}
static void kill_letbind_9(void) {
}

static int64_t zcap_reserved_width;
static void create_letbind_10(void) {


  int64_t zgsz313;
  zgsz313 = INT64_C(2);
  zcap_reserved_width = zgsz313;

let_end_55: ;
}
static void kill_letbind_10(void) {
}

static int64_t zcap_flags_width;
static void create_letbind_11(void) {


  int64_t zgsz314;
  zgsz314 = INT64_C(1);
  zcap_flags_width = zgsz314;

let_end_56: ;
}
static void kill_letbind_11(void) {
}

static int64_t zcap_mantissa_width;
static void create_letbind_12(void) {


  int64_t zgsz315;
  zgsz315 = INT64_C(14);
  zcap_mantissa_width = zgsz315;

let_end_57: ;
}
static void kill_letbind_12(void) {
}

static int64_t zcap_E_width;
static void create_letbind_13(void) {


  int64_t zgsz316;
  zgsz316 = INT64_C(6);
  zcap_E_width = zgsz316;

let_end_58: ;
}
static void kill_letbind_13(void) {
}

static int64_t zcap_addr_width;
static void create_letbind_14(void) {


  int64_t zgsz317;
  zgsz317 = INT64_C(64);
  zcap_addr_width = zgsz317;

let_end_59: ;
}
static void kill_letbind_14(void) {
}

static int64_t zcap_len_width;
static void create_letbind_15(void) {


  int64_t zgsz318;
  {
    sail_int zgsz391;
    CREATE(sail_int)(&zgsz391);
    CONVERT_OF(sail_int, mach_int)(&zgsz391, INT64_C(64));
    sail_int zgsz392;
    CREATE(sail_int)(&zgsz392);
    CONVERT_OF(sail_int, mach_int)(&zgsz392, INT64_C(1));
    sail_int zgsz393;
    CREATE(sail_int)(&zgsz393);
    add_int(&zgsz393, zgsz391, zgsz392);
    zgsz318 = CONVERT_OF(mach_int, sail_int)(zgsz393);
    KILL(sail_int)(&zgsz393);
    KILL(sail_int)(&zgsz392);
    KILL(sail_int)(&zgsz391);
  }
  zcap_len_width = zgsz318;

let_end_60: ;
}
static void kill_letbind_15(void) {
}

static int64_t zcaps_per_cache_line;
static void create_letbind_16(void) {


  int64_t zgsz319;
  zgsz319 = INT64_C(4);
  zcaps_per_cache_line = zgsz319;

let_end_61: ;
}
static void kill_letbind_16(void) {
}

static int64_t zinternal_E_take_bits;
static void create_letbind_17(void) {


  int64_t zgsz320;
  zgsz320 = INT64_C(3);
  zinternal_E_take_bits = zgsz320;

let_end_62: ;
}
static void kill_letbind_17(void) {
}

static struct zEncCapability sailgen_capBitsToEncCapability(lbits);

static struct zEncCapability sailgen_capBitsToEncCapability(lbits zc)
{
  __label__ end_function_64, end_block_exception_65;

  struct zEncCapability zcbz314;
  uint64_t zgaz39;
  {
    sail_int zgsz3113;
    CREATE(sail_int)(&zgsz3113);
    CONVERT_OF(sail_int, mach_int)(&zgsz3113, INT64_C(127));
    sail_int zgsz3114;
    CREATE(sail_int)(&zgsz3114);
    CONVERT_OF(sail_int, mach_int)(&zgsz3114, INT64_C(112));
    lbits zgsz3115;
    CREATE(lbits)(&zgsz3115);
    vector_subrange_lbits(&zgsz3115, zc, zgsz3113, zgsz3114);
    zgaz39 = CONVERT_OF(fbits, lbits)(zgsz3115, true);
    KILL(lbits)(&zgsz3115);
    KILL(sail_int)(&zgsz3114);
    KILL(sail_int)(&zgsz3113);
  }
  uint64_t zgaz310;
  {
    sail_int zgsz3110;
    CREATE(sail_int)(&zgsz3110);
    CONVERT_OF(sail_int, mach_int)(&zgsz3110, INT64_C(111));
    sail_int zgsz3111;
    CREATE(sail_int)(&zgsz3111);
    CONVERT_OF(sail_int, mach_int)(&zgsz3111, INT64_C(110));
    lbits zgsz3112;
    CREATE(lbits)(&zgsz3112);
    vector_subrange_lbits(&zgsz3112, zc, zgsz3110, zgsz3111);
    zgaz310 = CONVERT_OF(fbits, lbits)(zgsz3112, true);
    KILL(lbits)(&zgsz3112);
    KILL(sail_int)(&zgsz3111);
    KILL(sail_int)(&zgsz3110);
  }
  uint64_t zgaz311;
  {
    sail_int zgsz3107;
    CREATE(sail_int)(&zgsz3107);
    CONVERT_OF(sail_int, mach_int)(&zgsz3107, INT64_C(109));
    sail_int zgsz3108;
    CREATE(sail_int)(&zgsz3108);
    CONVERT_OF(sail_int, mach_int)(&zgsz3108, INT64_C(109));
    lbits zgsz3109;
    CREATE(lbits)(&zgsz3109);
    vector_subrange_lbits(&zgsz3109, zc, zgsz3107, zgsz3108);
    zgaz311 = CONVERT_OF(fbits, lbits)(zgsz3109, true);
    KILL(lbits)(&zgsz3109);
    KILL(sail_int)(&zgsz3108);
    KILL(sail_int)(&zgsz3107);
  }
  uint64_t zgaz312;
  {
    sail_int zgsz3104;
    CREATE(sail_int)(&zgsz3104);
    CONVERT_OF(sail_int, mach_int)(&zgsz3104, INT64_C(108));
    sail_int zgsz3105;
    CREATE(sail_int)(&zgsz3105);
    CONVERT_OF(sail_int, mach_int)(&zgsz3105, INT64_C(91));
    lbits zgsz3106;
    CREATE(lbits)(&zgsz3106);
    vector_subrange_lbits(&zgsz3106, zc, zgsz3104, zgsz3105);
    zgaz312 = CONVERT_OF(fbits, lbits)(zgsz3106, true);
    KILL(lbits)(&zgsz3106);
    KILL(sail_int)(&zgsz3105);
    KILL(sail_int)(&zgsz3104);
  }
  fbits zgaz313;
  {
    sail_int zgsz3103;
    CREATE(sail_int)(&zgsz3103);
    CONVERT_OF(sail_int, mach_int)(&zgsz3103, INT64_C(90));
    zgaz313 = bitvector_access(zc, zgsz3103);
    KILL(sail_int)(&zgsz3103);
  }
  uint64_t zgaz314;
  {
    sail_int zgsz3100;
    CREATE(sail_int)(&zgsz3100);
    CONVERT_OF(sail_int, mach_int)(&zgsz3100, INT64_C(89));
    sail_int zgsz3101;
    CREATE(sail_int)(&zgsz3101);
    CONVERT_OF(sail_int, mach_int)(&zgsz3101, INT64_C(78));
    lbits zgsz3102;
    CREATE(lbits)(&zgsz3102);
    vector_subrange_lbits(&zgsz3102, zc, zgsz3100, zgsz3101);
    zgaz314 = CONVERT_OF(fbits, lbits)(zgsz3102, true);
    KILL(lbits)(&zgsz3102);
    KILL(sail_int)(&zgsz3101);
    KILL(sail_int)(&zgsz3100);
  }
  uint64_t zgaz315;
  {
    sail_int zgsz397;
    CREATE(sail_int)(&zgsz397);
    CONVERT_OF(sail_int, mach_int)(&zgsz397, INT64_C(77));
    sail_int zgsz398;
    CREATE(sail_int)(&zgsz398);
    CONVERT_OF(sail_int, mach_int)(&zgsz398, INT64_C(64));
    lbits zgsz399;
    CREATE(lbits)(&zgsz399);
    vector_subrange_lbits(&zgsz399, zc, zgsz397, zgsz398);
    zgaz315 = CONVERT_OF(fbits, lbits)(zgsz399, true);
    KILL(lbits)(&zgsz399);
    KILL(sail_int)(&zgsz398);
    KILL(sail_int)(&zgsz397);
  }
  uint64_t zgaz316;
  {
    sail_int zgsz394;
    CREATE(sail_int)(&zgsz394);
    CONVERT_OF(sail_int, mach_int)(&zgsz394, INT64_C(63));
    sail_int zgsz395;
    CREATE(sail_int)(&zgsz395);
    CONVERT_OF(sail_int, mach_int)(&zgsz395, INT64_C(0));
    lbits zgsz396;
    CREATE(lbits)(&zgsz396);
    vector_subrange_lbits(&zgsz396, zc, zgsz394, zgsz395);
    zgaz316 = CONVERT_OF(fbits, lbits)(zgsz396, true);
    KILL(lbits)(&zgsz396);
    KILL(sail_int)(&zgsz395);
    KILL(sail_int)(&zgsz394);
  }
  struct zEncCapability zgsz321;
  zgsz321.zB = zgaz315;
  zgsz321.zT = zgaz314;
  zgsz321.zaddress = zgaz316;
  zgsz321.zflags = zgaz311;
  zgsz321.zinternal_E = zgaz313;
  zgsz321.zotype = zgaz312;
  zgsz321.zperms = zgaz39;
  zgsz321.zreserved = zgaz310;
  zcbz314 = zgsz321;









end_function_64: ;
  return zcbz314;
end_block_exception_65: ;
  struct zEncCapability zcbz341 = { .zreserved = UINT64_C(0xdeadc0de), .zperms = UINT64_C(0xdeadc0de), .zotype = UINT64_C(0xdeadc0de), .zinternal_E = UINT64_C(0), .zflags = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  return zcbz341;
}

static void sailgen_encCapToBits(lbits *rop, struct zEncCapability);

static void sailgen_encCapToBits(lbits *zcbz315, struct zEncCapability zcap)
{
  __label__ end_function_67, end_block_exception_68, end_function_176;

  uint64_t zgaz330;
  zgaz330 = zcap.zperms;
  lbits zgaz331;
  CREATE(lbits)(&zgaz331);
  {
    uint64_t zgaz328;
    zgaz328 = zcap.zreserved;
    lbits zgaz329;
    CREATE(lbits)(&zgaz329);
    {
      uint64_t zgaz326;
      zgaz326 = zcap.zflags;
      lbits zgaz327;
      CREATE(lbits)(&zgaz327);
      {
        uint64_t zgaz324;
        zgaz324 = zcap.zotype;
        lbits zgaz325;
        CREATE(lbits)(&zgaz325);
        {
          uint64_t zgaz322;
          {
            fbits zgaz317;
            zgaz317 = zcap.zinternal_E;
            uint64_t zgsz322;
            zgsz322 = UINT64_C(0b0);
            zgsz322 = update_fbits(zgsz322, INT64_C(0), zgaz317);
            zgaz322 = zgsz322;
          }
          lbits zgaz323;
          CREATE(lbits)(&zgaz323);
          {
            uint64_t zgaz320;
            zgaz320 = zcap.zT;
            lbits zgaz321;
            CREATE(lbits)(&zgaz321);
            {
              uint64_t zgaz318;
              zgaz318 = zcap.zB;
              uint64_t zgaz319;
              zgaz319 = zcap.zaddress;
              {
                lbits zgsz3116;
                CREATE(lbits)(&zgsz3116);
                CONVERT_OF(lbits, fbits)(&zgsz3116, zgaz318, UINT64_C(14) , true);
                lbits zgsz3117;
                CREATE(lbits)(&zgsz3117);
                CONVERT_OF(lbits, fbits)(&zgsz3117, zgaz319, UINT64_C(64) , true);
                append(&zgaz321, zgsz3116, zgsz3117);
                KILL(lbits)(&zgsz3117);
                KILL(lbits)(&zgsz3116);
              }
            }
            {
              lbits zgsz3118;
              CREATE(lbits)(&zgsz3118);
              CONVERT_OF(lbits, fbits)(&zgsz3118, zgaz320, UINT64_C(12) , true);
              append(&zgaz323, zgsz3118, zgaz321);
              KILL(lbits)(&zgsz3118);
            }
            KILL(lbits)(&zgaz321);
          }
          {
            lbits zgsz3119;
            CREATE(lbits)(&zgsz3119);
            CONVERT_OF(lbits, fbits)(&zgsz3119, zgaz322, UINT64_C(1) , true);
            append(&zgaz325, zgsz3119, zgaz323);
            KILL(lbits)(&zgsz3119);
          }
          KILL(lbits)(&zgaz323);
        }
        {
          lbits zgsz3120;
          CREATE(lbits)(&zgsz3120);
          CONVERT_OF(lbits, fbits)(&zgsz3120, zgaz324, UINT64_C(18) , true);
          append(&zgaz327, zgsz3120, zgaz325);
          KILL(lbits)(&zgsz3120);
        }
        KILL(lbits)(&zgaz325);
      }
      {
        lbits zgsz3121;
        CREATE(lbits)(&zgsz3121);
        CONVERT_OF(lbits, fbits)(&zgsz3121, zgaz326, UINT64_C(1) , true);
        append(&zgaz329, zgsz3121, zgaz327);
        KILL(lbits)(&zgsz3121);
      }
      KILL(lbits)(&zgaz327);
    }
    {
      lbits zgsz3122;
      CREATE(lbits)(&zgsz3122);
      CONVERT_OF(lbits, fbits)(&zgsz3122, zgaz328, UINT64_C(2) , true);
      append(&zgaz331, zgsz3122, zgaz329);
      KILL(lbits)(&zgsz3122);
    }
    KILL(lbits)(&zgaz329);
  }
  {
    lbits zgsz3123;
    CREATE(lbits)(&zgsz3123);
    CONVERT_OF(lbits, fbits)(&zgsz3123, zgaz330, UINT64_C(16) , true);
    append((*(&zcbz315)), zgsz3123, zgaz331);
    KILL(lbits)(&zgsz3123);
  }
  KILL(lbits)(&zgaz331);

end_function_67: ;
  goto end_function_176;
end_block_exception_68: ;
  goto end_function_176;
end_function_176: ;
}

static sail_int zcap_max_addr;
static void create_letbind_18(void) {    CREATE(sail_int)(&zcap_max_addr);


  sail_int zgsz323;
  CREATE(sail_int)(&zgsz323);
  {
    sail_int zgsz3124;
    CREATE(sail_int)(&zgsz3124);
    CONVERT_OF(sail_int, mach_int)(&zgsz3124, zcap_addr_width);
    sailgen_MAX(&zgsz323, zgsz3124);
    KILL(sail_int)(&zgsz3124);
  }
  COPY(sail_int)(&zcap_max_addr, zgsz323);
  KILL(sail_int)(&zgsz323);
let_end_69: ;
}
static void kill_letbind_18(void) {    KILL(sail_int)(&zcap_max_addr);
}

static int64_t zcap_max_otype;
static void create_letbind_19(void) {


  int64_t zgsz324;
  int64_t zgaz332;
  {
    sail_int zgsz3128;
    CREATE(sail_int)(&zgsz3128);
    CONVERT_OF(sail_int, mach_int)(&zgsz3128, zcap_otype_width);
    sail_int zgsz3129;
    CREATE(sail_int)(&zgsz3129);
    sailgen_MAX(&zgsz3129, zgsz3128);
    zgaz332 = CONVERT_OF(mach_int, sail_int)(zgsz3129);
    KILL(sail_int)(&zgsz3129);
    KILL(sail_int)(&zgsz3128);
  }
  {
    sail_int zgsz3125;
    CREATE(sail_int)(&zgsz3125);
    CONVERT_OF(sail_int, mach_int)(&zgsz3125, zgaz332);
    sail_int zgsz3126;
    CREATE(sail_int)(&zgsz3126);
    CONVERT_OF(sail_int, mach_int)(&zgsz3126, zreserved_otypes);
    sail_int zgsz3127;
    CREATE(sail_int)(&zgsz3127);
    sub_int(&zgsz3127, zgsz3125, zgsz3126);
    zgsz324 = CONVERT_OF(mach_int, sail_int)(zgsz3127);
    KILL(sail_int)(&zgsz3127);
    KILL(sail_int)(&zgsz3126);
    KILL(sail_int)(&zgsz3125);
  }

  zcap_max_otype = zgsz324;

let_end_70: ;
}
static void kill_letbind_19(void) {
}

static int64_t zcap_uperms_shift;
static void create_letbind_20(void) {


  int64_t zgsz325;
  zgsz325 = INT64_C(15);
  zcap_uperms_shift = zgsz325;

let_end_71: ;
}
static void kill_letbind_20(void) {
}

static int64_t zcap_perms_width;
static void create_letbind_21(void) {


  int64_t zgsz326;
  {
    sail_int zgsz3130;
    CREATE(sail_int)(&zgsz3130);
    CONVERT_OF(sail_int, mach_int)(&zgsz3130, INT64_C(15));
    sail_int zgsz3131;
    CREATE(sail_int)(&zgsz3131);
    CONVERT_OF(sail_int, mach_int)(&zgsz3131, INT64_C(4));
    sail_int zgsz3132;
    CREATE(sail_int)(&zgsz3132);
    add_int(&zgsz3132, zgsz3130, zgsz3131);
    zgsz326 = CONVERT_OF(mach_int, sail_int)(zgsz3132);
    KILL(sail_int)(&zgsz3132);
    KILL(sail_int)(&zgsz3131);
    KILL(sail_int)(&zgsz3130);
  }
  zcap_perms_width = zgsz326;

let_end_72: ;
}
static void kill_letbind_21(void) {
}

static int64_t zcap_max_E;
static void create_letbind_22(void) {


  int64_t zgsz327;
  int64_t zgaz333;
  {
    sail_int zgsz3136;
    CREATE(sail_int)(&zgsz3136);
    CONVERT_OF(sail_int, mach_int)(&zgsz3136, zcap_len_width);
    sail_int zgsz3137;
    CREATE(sail_int)(&zgsz3137);
    CONVERT_OF(sail_int, mach_int)(&zgsz3137, zcap_mantissa_width);
    sail_int zgsz3138;
    CREATE(sail_int)(&zgsz3138);
    sub_int(&zgsz3138, zgsz3136, zgsz3137);
    zgaz333 = CONVERT_OF(mach_int, sail_int)(zgsz3138);
    KILL(sail_int)(&zgsz3138);
    KILL(sail_int)(&zgsz3137);
    KILL(sail_int)(&zgsz3136);
  }
  {
    sail_int zgsz3133;
    CREATE(sail_int)(&zgsz3133);
    CONVERT_OF(sail_int, mach_int)(&zgsz3133, zgaz333);
    sail_int zgsz3134;
    CREATE(sail_int)(&zgsz3134);
    CONVERT_OF(sail_int, mach_int)(&zgsz3134, INT64_C(1));
    sail_int zgsz3135;
    CREATE(sail_int)(&zgsz3135);
    add_int(&zgsz3135, zgsz3133, zgsz3134);
    zgsz327 = CONVERT_OF(mach_int, sail_int)(zgsz3135);
    KILL(sail_int)(&zgsz3135);
    KILL(sail_int)(&zgsz3134);
    KILL(sail_int)(&zgsz3133);
  }

  zcap_max_E = zgsz327;

let_end_73: ;
}
static void kill_letbind_22(void) {
}

static uint64_t zcap_reset_E;
static void create_letbind_23(void) {


  uint64_t zgsz328;
  {
    sail_int zgsz3139;
    CREATE(sail_int)(&zgsz3139);
    CONVERT_OF(sail_int, mach_int)(&zgsz3139, zcap_E_width);
    sail_int zgsz3140;
    CREATE(sail_int)(&zgsz3140);
    CONVERT_OF(sail_int, mach_int)(&zgsz3140, zcap_max_E);
    lbits zgsz3141;
    CREATE(lbits)(&zgsz3141);
    sailgen_to_bits(&zgsz3141, zgsz3139, zgsz3140);
    zgsz328 = CONVERT_OF(fbits, lbits)(zgsz3141, true);
    KILL(lbits)(&zgsz3141);
    KILL(sail_int)(&zgsz3140);
    KILL(sail_int)(&zgsz3139);
  }
  zcap_reset_E = zgsz328;

let_end_74: ;
}
static void kill_letbind_23(void) {
}

static uint64_t zcap_reset_T;
static void create_letbind_24(void) {


  uint64_t zgsz329;
  uint64_t zgaz335;
  {
    int64_t zgaz334;
    {
      sail_int zgsz3144;
      CREATE(sail_int)(&zgsz3144);
      CONVERT_OF(sail_int, mach_int)(&zgsz3144, zcap_mantissa_width);
      sail_int zgsz3145;
      CREATE(sail_int)(&zgsz3145);
      CONVERT_OF(sail_int, mach_int)(&zgsz3145, INT64_C(2));
      sail_int zgsz3146;
      CREATE(sail_int)(&zgsz3146);
      sub_int(&zgsz3146, zgsz3144, zgsz3145);
      zgaz334 = CONVERT_OF(mach_int, sail_int)(zgsz3146);
      KILL(sail_int)(&zgsz3146);
      KILL(sail_int)(&zgsz3145);
      KILL(sail_int)(&zgsz3144);
    }
    {
      sail_int zgsz3142;
      CREATE(sail_int)(&zgsz3142);
      CONVERT_OF(sail_int, mach_int)(&zgsz3142, zgaz334);
      lbits zgsz3143;
      CREATE(lbits)(&zgsz3143);
      sailgen_zzeros_implicit(&zgsz3143, zgsz3142);
      zgaz335 = CONVERT_OF(fbits, lbits)(zgsz3143, true);
      KILL(lbits)(&zgsz3143);
      KILL(sail_int)(&zgsz3142);
    }
  }
  {
    lbits zgsz3147;
    CREATE(lbits)(&zgsz3147);
    CONVERT_OF(lbits, fbits)(&zgsz3147, UINT64_C(0b01), UINT64_C(2) , true);
    lbits zgsz3148;
    CREATE(lbits)(&zgsz3148);
    CONVERT_OF(lbits, fbits)(&zgsz3148, zgaz335, UINT64_C(12) , true);
    lbits zgsz3149;
    CREATE(lbits)(&zgsz3149);
    append(&zgsz3149, zgsz3147, zgsz3148);
    zgsz329 = CONVERT_OF(fbits, lbits)(zgsz3149, true);
    KILL(lbits)(&zgsz3149);
    KILL(lbits)(&zgsz3148);
    KILL(lbits)(&zgsz3147);
  }

  zcap_reset_T = zgsz329;

let_end_75: ;
}
static void kill_letbind_24(void) {
}

static struct zCapability sailgen_undefined_Capability(unit);

static struct zCapability sailgen_undefined_Capability(unit zgsz330)
{
  __label__ end_function_77, end_block_exception_78;

  struct zCapability zcbz316;
  bool zgaz336;
  zgaz336 = undefined_bool(UNIT);
  uint64_t zgaz337;
  {
    sail_int zgsz3162;
    CREATE(sail_int)(&zgsz3162);
    CONVERT_OF(sail_int, mach_int)(&zgsz3162, INT64_C(4));
    lbits zgsz3163;
    CREATE(lbits)(&zgsz3163);
    UNDEFINED(lbits)(&zgsz3163, zgsz3162);
    zgaz337 = CONVERT_OF(fbits, lbits)(zgsz3163, true);
    KILL(lbits)(&zgsz3163);
    KILL(sail_int)(&zgsz3162);
  }
  bool zgaz338;
  zgaz338 = undefined_bool(UNIT);
  bool zgaz339;
  zgaz339 = undefined_bool(UNIT);
  bool zgaz340;
  zgaz340 = undefined_bool(UNIT);
  bool zgaz341;
  zgaz341 = undefined_bool(UNIT);
  bool zgaz342;
  zgaz342 = undefined_bool(UNIT);
  bool zgaz343;
  zgaz343 = undefined_bool(UNIT);
  bool zgaz344;
  zgaz344 = undefined_bool(UNIT);
  bool zgaz345;
  zgaz345 = undefined_bool(UNIT);
  bool zgaz346;
  zgaz346 = undefined_bool(UNIT);
  bool zgaz347;
  zgaz347 = undefined_bool(UNIT);
  bool zgaz348;
  zgaz348 = undefined_bool(UNIT);
  bool zgaz349;
  zgaz349 = undefined_bool(UNIT);
  uint64_t zgaz350;
  {
    sail_int zgsz3160;
    CREATE(sail_int)(&zgsz3160);
    CONVERT_OF(sail_int, mach_int)(&zgsz3160, INT64_C(2));
    lbits zgsz3161;
    CREATE(lbits)(&zgsz3161);
    UNDEFINED(lbits)(&zgsz3161, zgsz3160);
    zgaz350 = CONVERT_OF(fbits, lbits)(zgsz3161, true);
    KILL(lbits)(&zgsz3161);
    KILL(sail_int)(&zgsz3160);
  }
  bool zgaz351;
  zgaz351 = undefined_bool(UNIT);
  bool zgaz352;
  zgaz352 = undefined_bool(UNIT);
  uint64_t zgaz353;
  {
    sail_int zgsz3158;
    CREATE(sail_int)(&zgsz3158);
    CONVERT_OF(sail_int, mach_int)(&zgsz3158, INT64_C(6));
    lbits zgsz3159;
    CREATE(lbits)(&zgsz3159);
    UNDEFINED(lbits)(&zgsz3159, zgsz3158);
    zgaz353 = CONVERT_OF(fbits, lbits)(zgsz3159, true);
    KILL(lbits)(&zgsz3159);
    KILL(sail_int)(&zgsz3158);
  }
  uint64_t zgaz354;
  {
    sail_int zgsz3156;
    CREATE(sail_int)(&zgsz3156);
    CONVERT_OF(sail_int, mach_int)(&zgsz3156, INT64_C(14));
    lbits zgsz3157;
    CREATE(lbits)(&zgsz3157);
    UNDEFINED(lbits)(&zgsz3157, zgsz3156);
    zgaz354 = CONVERT_OF(fbits, lbits)(zgsz3157, true);
    KILL(lbits)(&zgsz3157);
    KILL(sail_int)(&zgsz3156);
  }
  uint64_t zgaz355;
  {
    sail_int zgsz3154;
    CREATE(sail_int)(&zgsz3154);
    CONVERT_OF(sail_int, mach_int)(&zgsz3154, INT64_C(14));
    lbits zgsz3155;
    CREATE(lbits)(&zgsz3155);
    UNDEFINED(lbits)(&zgsz3155, zgsz3154);
    zgaz355 = CONVERT_OF(fbits, lbits)(zgsz3155, true);
    KILL(lbits)(&zgsz3155);
    KILL(sail_int)(&zgsz3154);
  }
  uint64_t zgaz356;
  {
    sail_int zgsz3152;
    CREATE(sail_int)(&zgsz3152);
    CONVERT_OF(sail_int, mach_int)(&zgsz3152, INT64_C(18));
    lbits zgsz3153;
    CREATE(lbits)(&zgsz3153);
    UNDEFINED(lbits)(&zgsz3153, zgsz3152);
    zgaz356 = CONVERT_OF(fbits, lbits)(zgsz3153, true);
    KILL(lbits)(&zgsz3153);
    KILL(sail_int)(&zgsz3152);
  }
  uint64_t zgaz357;
  {
    sail_int zgsz3150;
    CREATE(sail_int)(&zgsz3150);
    CONVERT_OF(sail_int, mach_int)(&zgsz3150, INT64_C(64));
    lbits zgsz3151;
    CREATE(lbits)(&zgsz3151);
    UNDEFINED(lbits)(&zgsz3151, zgsz3150);
    zgaz357 = CONVERT_OF(fbits, lbits)(zgsz3151, true);
    KILL(lbits)(&zgsz3151);
    KILL(sail_int)(&zgsz3150);
  }
  struct zCapability zgsz331;
  zgsz331.zB = zgaz354;
  zgsz331.zE = zgaz353;
  zgsz331.zT = zgaz355;
  zgsz331.zaccess_system_regs = zgaz339;
  zgsz331.zaddress = zgaz357;
  zgsz331.zflag_cap_mode = zgaz351;
  zgsz331.zglobal = zgaz349;
  zgsz331.zinternal_E = zgaz352;
  zgsz331.zotype = zgaz356;
  zgsz331.zpermit_cinvoke = zgaz341;
  zgsz331.zpermit_execute = zgaz348;
  zgsz331.zpermit_load = zgaz347;
  zgsz331.zpermit_load_cap = zgaz345;
  zgsz331.zpermit_seal = zgaz342;
  zgsz331.zpermit_set_CID = zgaz338;
  zgsz331.zpermit_store = zgaz346;
  zgsz331.zpermit_store_cap = zgaz344;
  zgsz331.zpermit_store_local_cap = zgaz343;
  zgsz331.zpermit_unseal = zgaz340;
  zgsz331.zreserved = zgaz350;
  zgsz331.ztag = zgaz336;
  zgsz331.zuperms = zgaz337;
  zcbz316 = zgsz331;























end_function_77: ;
  return zcbz316;
end_block_exception_78: ;
  struct zCapability zcbz342 = { .zuperms = UINT64_C(0xdeadc0de), .ztag = false, .zreserved = UINT64_C(0xdeadc0de), .zpermit_unseal = false, .zpermit_store_local_cap = false, .zpermit_store_cap = false, .zpermit_store = false, .zpermit_set_CID = false, .zpermit_seal = false, .zpermit_load_cap = false, .zpermit_load = false, .zpermit_execute = false, .zpermit_cinvoke = false, .zotype = UINT64_C(0xdeadc0de), .zinternal_E = false, .zglobal = false, .zflag_cap_mode = false, .zaddress = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zT = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  return zcbz342;
}

static struct zCapability znull_cap;
static void create_letbind_25(void) {


  struct zCapability zgsz333;
  uint64_t zgaz358;
  {
    sail_int zgsz3173;
    CREATE(sail_int)(&zgsz3173);
    CONVERT_OF(sail_int, mach_int)(&zgsz3173, INT64_C(4));
    lbits zgsz3174;
    CREATE(lbits)(&zgsz3174);
    sailgen_zzeros_implicit(&zgsz3174, zgsz3173);
    zgaz358 = CONVERT_OF(fbits, lbits)(zgsz3174, true);
    KILL(lbits)(&zgsz3174);
    KILL(sail_int)(&zgsz3173);
  }
  uint64_t zgaz359;
  {
    sail_int zgsz3171;
    CREATE(sail_int)(&zgsz3171);
    CONVERT_OF(sail_int, mach_int)(&zgsz3171, INT64_C(2));
    lbits zgsz3172;
    CREATE(lbits)(&zgsz3172);
    sailgen_zzeros_implicit(&zgsz3172, zgsz3171);
    zgaz359 = CONVERT_OF(fbits, lbits)(zgsz3172, true);
    KILL(lbits)(&zgsz3172);
    KILL(sail_int)(&zgsz3171);
  }
  uint64_t zgaz360;
  {
    sail_int zgsz3169;
    CREATE(sail_int)(&zgsz3169);
    CONVERT_OF(sail_int, mach_int)(&zgsz3169, INT64_C(14));
    lbits zgsz3170;
    CREATE(lbits)(&zgsz3170);
    sailgen_zzeros_implicit(&zgsz3170, zgsz3169);
    zgaz360 = CONVERT_OF(fbits, lbits)(zgsz3170, true);
    KILL(lbits)(&zgsz3170);
    KILL(sail_int)(&zgsz3169);
  }
  uint64_t zgaz361;
  {
    sail_int zgsz3166;
    CREATE(sail_int)(&zgsz3166);
    CONVERT_OF(sail_int, mach_int)(&zgsz3166, zcap_otype_width);
    sail_int zgsz3167;
    CREATE(sail_int)(&zgsz3167);
    CONVERT_OF(sail_int, mach_int)(&zgsz3167, zotype_unsealed);
    lbits zgsz3168;
    CREATE(lbits)(&zgsz3168);
    sailgen_to_bits(&zgsz3168, zgsz3166, zgsz3167);
    zgaz361 = CONVERT_OF(fbits, lbits)(zgsz3168, true);
    KILL(lbits)(&zgsz3168);
    KILL(sail_int)(&zgsz3167);
    KILL(sail_int)(&zgsz3166);
  }
  uint64_t zgaz362;
  {
    sail_int zgsz3164;
    CREATE(sail_int)(&zgsz3164);
    CONVERT_OF(sail_int, mach_int)(&zgsz3164, INT64_C(64));
    lbits zgsz3165;
    CREATE(lbits)(&zgsz3165);
    sailgen_zzeros_implicit(&zgsz3165, zgsz3164);
    zgaz362 = CONVERT_OF(fbits, lbits)(zgsz3165, true);
    KILL(lbits)(&zgsz3165);
    KILL(sail_int)(&zgsz3164);
  }
  struct zCapability zgsz332;
  zgsz332.zB = zgaz360;
  zgsz332.zE = zcap_reset_E;
  zgsz332.zT = zcap_reset_T;
  zgsz332.zaccess_system_regs = false;
  zgsz332.zaddress = zgaz362;
  zgsz332.zflag_cap_mode = false;
  zgsz332.zglobal = false;
  zgsz332.zinternal_E = true;
  zgsz332.zotype = zgaz361;
  zgsz332.zpermit_cinvoke = false;
  zgsz332.zpermit_execute = false;
  zgsz332.zpermit_load = false;
  zgsz332.zpermit_load_cap = false;
  zgsz332.zpermit_seal = false;
  zgsz332.zpermit_set_CID = false;
  zgsz332.zpermit_store = false;
  zgsz332.zpermit_store_cap = false;
  zgsz332.zpermit_store_local_cap = false;
  zgsz332.zpermit_unseal = false;
  zgsz332.zreserved = zgaz359;
  zgsz332.ztag = false;
  zgsz332.zuperms = zgaz358;
  zgsz333 = zgsz332;






  znull_cap = zgsz333;

let_end_79: ;
}
static void kill_letbind_25(void) {
}

static struct zCapability zdefault_cap;
static void create_letbind_26(void) {


  struct zCapability zgsz335;
  uint64_t zgaz363;
  {
    sail_int zgsz3184;
    CREATE(sail_int)(&zgsz3184);
    CONVERT_OF(sail_int, mach_int)(&zgsz3184, INT64_C(4));
    lbits zgsz3185;
    CREATE(lbits)(&zgsz3185);
    sailgen_ones(&zgsz3185, zgsz3184);
    zgaz363 = CONVERT_OF(fbits, lbits)(zgsz3185, true);
    KILL(lbits)(&zgsz3185);
    KILL(sail_int)(&zgsz3184);
  }
  uint64_t zgaz364;
  {
    sail_int zgsz3182;
    CREATE(sail_int)(&zgsz3182);
    CONVERT_OF(sail_int, mach_int)(&zgsz3182, INT64_C(2));
    lbits zgsz3183;
    CREATE(lbits)(&zgsz3183);
    sailgen_zzeros_implicit(&zgsz3183, zgsz3182);
    zgaz364 = CONVERT_OF(fbits, lbits)(zgsz3183, true);
    KILL(lbits)(&zgsz3183);
    KILL(sail_int)(&zgsz3182);
  }
  uint64_t zgaz365;
  {
    sail_int zgsz3180;
    CREATE(sail_int)(&zgsz3180);
    CONVERT_OF(sail_int, mach_int)(&zgsz3180, INT64_C(14));
    lbits zgsz3181;
    CREATE(lbits)(&zgsz3181);
    sailgen_zzeros_implicit(&zgsz3181, zgsz3180);
    zgaz365 = CONVERT_OF(fbits, lbits)(zgsz3181, true);
    KILL(lbits)(&zgsz3181);
    KILL(sail_int)(&zgsz3180);
  }
  uint64_t zgaz366;
  {
    sail_int zgsz3177;
    CREATE(sail_int)(&zgsz3177);
    CONVERT_OF(sail_int, mach_int)(&zgsz3177, zcap_otype_width);
    sail_int zgsz3178;
    CREATE(sail_int)(&zgsz3178);
    CONVERT_OF(sail_int, mach_int)(&zgsz3178, zotype_unsealed);
    lbits zgsz3179;
    CREATE(lbits)(&zgsz3179);
    sailgen_to_bits(&zgsz3179, zgsz3177, zgsz3178);
    zgaz366 = CONVERT_OF(fbits, lbits)(zgsz3179, true);
    KILL(lbits)(&zgsz3179);
    KILL(sail_int)(&zgsz3178);
    KILL(sail_int)(&zgsz3177);
  }
  uint64_t zgaz367;
  {
    sail_int zgsz3175;
    CREATE(sail_int)(&zgsz3175);
    CONVERT_OF(sail_int, mach_int)(&zgsz3175, INT64_C(64));
    lbits zgsz3176;
    CREATE(lbits)(&zgsz3176);
    sailgen_zzeros_implicit(&zgsz3176, zgsz3175);
    zgaz367 = CONVERT_OF(fbits, lbits)(zgsz3176, true);
    KILL(lbits)(&zgsz3176);
    KILL(sail_int)(&zgsz3175);
  }
  struct zCapability zgsz334;
  zgsz334.zB = zgaz365;
  zgsz334.zE = zcap_reset_E;
  zgsz334.zT = zcap_reset_T;
  zgsz334.zaccess_system_regs = true;
  zgsz334.zaddress = zgaz367;
  zgsz334.zflag_cap_mode = false;
  zgsz334.zglobal = true;
  zgsz334.zinternal_E = true;
  zgsz334.zotype = zgaz366;
  zgsz334.zpermit_cinvoke = true;
  zgsz334.zpermit_execute = true;
  zgsz334.zpermit_load = true;
  zgsz334.zpermit_load_cap = true;
  zgsz334.zpermit_seal = true;
  zgsz334.zpermit_set_CID = true;
  zgsz334.zpermit_store = true;
  zgsz334.zpermit_store_cap = true;
  zgsz334.zpermit_store_local_cap = true;
  zgsz334.zpermit_unseal = true;
  zgsz334.zreserved = zgaz364;
  zgsz334.ztag = true;
  zgsz334.zuperms = zgaz363;
  zgsz335 = zgsz334;






  zdefault_cap = zgsz335;

let_end_80: ;
}
static void kill_letbind_26(void) {
}

static uint64_t sailgen_getCapHardPerms(struct zCapability);

static uint64_t sailgen_getCapHardPerms(struct zCapability zcap)
{
  __label__ end_function_82, end_block_exception_83;

  uint64_t zcbz317;
  uint64_t zgaz3100;
  {
    bool zgaz368;
    zgaz368 = zcap.zpermit_set_CID;
    zgaz3100 = sailgen_bool_to_bits(zgaz368);
  }
  uint64_t zgaz3101;
  {
    uint64_t zgaz398;
    {
      bool zgaz369;
      zgaz369 = zcap.zaccess_system_regs;
      zgaz398 = sailgen_bool_to_bits(zgaz369);
    }
    uint64_t zgaz399;
    {
      uint64_t zgaz396;
      {
        bool zgaz370;
        zgaz370 = zcap.zpermit_unseal;
        zgaz396 = sailgen_bool_to_bits(zgaz370);
      }
      uint64_t zgaz397;
      {
        uint64_t zgaz394;
        {
          bool zgaz371;
          zgaz371 = zcap.zpermit_cinvoke;
          zgaz394 = sailgen_bool_to_bits(zgaz371);
        }
        uint64_t zgaz395;
        {
          uint64_t zgaz392;
          {
            bool zgaz372;
            zgaz372 = zcap.zpermit_seal;
            zgaz392 = sailgen_bool_to_bits(zgaz372);
          }
          uint64_t zgaz393;
          {
            uint64_t zgaz390;
            {
              bool zgaz373;
              zgaz373 = zcap.zpermit_store_local_cap;
              zgaz390 = sailgen_bool_to_bits(zgaz373);
            }
            uint64_t zgaz391;
            {
              uint64_t zgaz388;
              {
                bool zgaz374;
                zgaz374 = zcap.zpermit_store_cap;
                zgaz388 = sailgen_bool_to_bits(zgaz374);
              }
              uint64_t zgaz389;
              {
                uint64_t zgaz386;
                {
                  bool zgaz375;
                  zgaz375 = zcap.zpermit_load_cap;
                  zgaz386 = sailgen_bool_to_bits(zgaz375);
                }
                uint64_t zgaz387;
                {
                  uint64_t zgaz384;
                  {
                    bool zgaz376;
                    zgaz376 = zcap.zpermit_store;
                    zgaz384 = sailgen_bool_to_bits(zgaz376);
                  }
                  uint64_t zgaz385;
                  {
                    uint64_t zgaz382;
                    {
                      bool zgaz377;
                      zgaz377 = zcap.zpermit_load;
                      zgaz382 = sailgen_bool_to_bits(zgaz377);
                    }
                    uint64_t zgaz383;
                    {
                      uint64_t zgaz380;
                      {
                        bool zgaz378;
                        zgaz378 = zcap.zpermit_execute;
                        zgaz380 = sailgen_bool_to_bits(zgaz378);
                      }
                      uint64_t zgaz381;
                      {
                        bool zgaz379;
                        zgaz379 = zcap.zglobal;
                        zgaz381 = sailgen_bool_to_bits(zgaz379);
                      }
                      {
                        lbits zgsz3186;
                        CREATE(lbits)(&zgsz3186);
                        CONVERT_OF(lbits, fbits)(&zgsz3186, zgaz380, UINT64_C(1) , true);
                        lbits zgsz3187;
                        CREATE(lbits)(&zgsz3187);
                        CONVERT_OF(lbits, fbits)(&zgsz3187, zgaz381, UINT64_C(1) , true);
                        lbits zgsz3188;
                        CREATE(lbits)(&zgsz3188);
                        append(&zgsz3188, zgsz3186, zgsz3187);
                        zgaz383 = CONVERT_OF(fbits, lbits)(zgsz3188, true);
                        KILL(lbits)(&zgsz3188);
                        KILL(lbits)(&zgsz3187);
                        KILL(lbits)(&zgsz3186);
                      }
                    }
                    {
                      lbits zgsz3189;
                      CREATE(lbits)(&zgsz3189);
                      CONVERT_OF(lbits, fbits)(&zgsz3189, zgaz382, UINT64_C(1) , true);
                      lbits zgsz3190;
                      CREATE(lbits)(&zgsz3190);
                      CONVERT_OF(lbits, fbits)(&zgsz3190, zgaz383, UINT64_C(2) , true);
                      lbits zgsz3191;
                      CREATE(lbits)(&zgsz3191);
                      append(&zgsz3191, zgsz3189, zgsz3190);
                      zgaz385 = CONVERT_OF(fbits, lbits)(zgsz3191, true);
                      KILL(lbits)(&zgsz3191);
                      KILL(lbits)(&zgsz3190);
                      KILL(lbits)(&zgsz3189);
                    }
                  }
                  {
                    lbits zgsz3192;
                    CREATE(lbits)(&zgsz3192);
                    CONVERT_OF(lbits, fbits)(&zgsz3192, zgaz384, UINT64_C(1) , true);
                    lbits zgsz3193;
                    CREATE(lbits)(&zgsz3193);
                    CONVERT_OF(lbits, fbits)(&zgsz3193, zgaz385, UINT64_C(3) , true);
                    lbits zgsz3194;
                    CREATE(lbits)(&zgsz3194);
                    append(&zgsz3194, zgsz3192, zgsz3193);
                    zgaz387 = CONVERT_OF(fbits, lbits)(zgsz3194, true);
                    KILL(lbits)(&zgsz3194);
                    KILL(lbits)(&zgsz3193);
                    KILL(lbits)(&zgsz3192);
                  }
                }
                {
                  lbits zgsz3195;
                  CREATE(lbits)(&zgsz3195);
                  CONVERT_OF(lbits, fbits)(&zgsz3195, zgaz386, UINT64_C(1) , true);
                  lbits zgsz3196;
                  CREATE(lbits)(&zgsz3196);
                  CONVERT_OF(lbits, fbits)(&zgsz3196, zgaz387, UINT64_C(4) , true);
                  lbits zgsz3197;
                  CREATE(lbits)(&zgsz3197);
                  append(&zgsz3197, zgsz3195, zgsz3196);
                  zgaz389 = CONVERT_OF(fbits, lbits)(zgsz3197, true);
                  KILL(lbits)(&zgsz3197);
                  KILL(lbits)(&zgsz3196);
                  KILL(lbits)(&zgsz3195);
                }
              }
              {
                lbits zgsz3198;
                CREATE(lbits)(&zgsz3198);
                CONVERT_OF(lbits, fbits)(&zgsz3198, zgaz388, UINT64_C(1) , true);
                lbits zgsz3199;
                CREATE(lbits)(&zgsz3199);
                CONVERT_OF(lbits, fbits)(&zgsz3199, zgaz389, UINT64_C(5) , true);
                lbits zgsz3200;
                CREATE(lbits)(&zgsz3200);
                append(&zgsz3200, zgsz3198, zgsz3199);
                zgaz391 = CONVERT_OF(fbits, lbits)(zgsz3200, true);
                KILL(lbits)(&zgsz3200);
                KILL(lbits)(&zgsz3199);
                KILL(lbits)(&zgsz3198);
              }
            }
            {
              lbits zgsz3201;
              CREATE(lbits)(&zgsz3201);
              CONVERT_OF(lbits, fbits)(&zgsz3201, zgaz390, UINT64_C(1) , true);
              lbits zgsz3202;
              CREATE(lbits)(&zgsz3202);
              CONVERT_OF(lbits, fbits)(&zgsz3202, zgaz391, UINT64_C(6) , true);
              lbits zgsz3203;
              CREATE(lbits)(&zgsz3203);
              append(&zgsz3203, zgsz3201, zgsz3202);
              zgaz393 = CONVERT_OF(fbits, lbits)(zgsz3203, true);
              KILL(lbits)(&zgsz3203);
              KILL(lbits)(&zgsz3202);
              KILL(lbits)(&zgsz3201);
            }
          }
          {
            lbits zgsz3204;
            CREATE(lbits)(&zgsz3204);
            CONVERT_OF(lbits, fbits)(&zgsz3204, zgaz392, UINT64_C(1) , true);
            lbits zgsz3205;
            CREATE(lbits)(&zgsz3205);
            CONVERT_OF(lbits, fbits)(&zgsz3205, zgaz393, UINT64_C(7) , true);
            lbits zgsz3206;
            CREATE(lbits)(&zgsz3206);
            append(&zgsz3206, zgsz3204, zgsz3205);
            zgaz395 = CONVERT_OF(fbits, lbits)(zgsz3206, true);
            KILL(lbits)(&zgsz3206);
            KILL(lbits)(&zgsz3205);
            KILL(lbits)(&zgsz3204);
          }
        }
        {
          lbits zgsz3207;
          CREATE(lbits)(&zgsz3207);
          CONVERT_OF(lbits, fbits)(&zgsz3207, zgaz394, UINT64_C(1) , true);
          lbits zgsz3208;
          CREATE(lbits)(&zgsz3208);
          CONVERT_OF(lbits, fbits)(&zgsz3208, zgaz395, UINT64_C(8) , true);
          lbits zgsz3209;
          CREATE(lbits)(&zgsz3209);
          append(&zgsz3209, zgsz3207, zgsz3208);
          zgaz397 = CONVERT_OF(fbits, lbits)(zgsz3209, true);
          KILL(lbits)(&zgsz3209);
          KILL(lbits)(&zgsz3208);
          KILL(lbits)(&zgsz3207);
        }
      }
      {
        lbits zgsz3210;
        CREATE(lbits)(&zgsz3210);
        CONVERT_OF(lbits, fbits)(&zgsz3210, zgaz396, UINT64_C(1) , true);
        lbits zgsz3211;
        CREATE(lbits)(&zgsz3211);
        CONVERT_OF(lbits, fbits)(&zgsz3211, zgaz397, UINT64_C(9) , true);
        lbits zgsz3212;
        CREATE(lbits)(&zgsz3212);
        append(&zgsz3212, zgsz3210, zgsz3211);
        zgaz399 = CONVERT_OF(fbits, lbits)(zgsz3212, true);
        KILL(lbits)(&zgsz3212);
        KILL(lbits)(&zgsz3211);
        KILL(lbits)(&zgsz3210);
      }
    }
    {
      lbits zgsz3213;
      CREATE(lbits)(&zgsz3213);
      CONVERT_OF(lbits, fbits)(&zgsz3213, zgaz398, UINT64_C(1) , true);
      lbits zgsz3214;
      CREATE(lbits)(&zgsz3214);
      CONVERT_OF(lbits, fbits)(&zgsz3214, zgaz399, UINT64_C(10) , true);
      lbits zgsz3215;
      CREATE(lbits)(&zgsz3215);
      append(&zgsz3215, zgsz3213, zgsz3214);
      zgaz3101 = CONVERT_OF(fbits, lbits)(zgsz3215, true);
      KILL(lbits)(&zgsz3215);
      KILL(lbits)(&zgsz3214);
      KILL(lbits)(&zgsz3213);
    }
  }
  {
    lbits zgsz3216;
    CREATE(lbits)(&zgsz3216);
    CONVERT_OF(lbits, fbits)(&zgsz3216, zgaz3100, UINT64_C(1) , true);
    lbits zgsz3217;
    CREATE(lbits)(&zgsz3217);
    CONVERT_OF(lbits, fbits)(&zgsz3217, zgaz3101, UINT64_C(11) , true);
    lbits zgsz3218;
    CREATE(lbits)(&zgsz3218);
    append(&zgsz3218, zgsz3216, zgsz3217);
    zcbz317 = CONVERT_OF(fbits, lbits)(zgsz3218, true);
    KILL(lbits)(&zgsz3218);
    KILL(lbits)(&zgsz3217);
    KILL(lbits)(&zgsz3216);
  }


end_function_82: ;
  return zcbz317;
end_block_exception_83: ;

  return UINT64_C(0xdeadc0de);
}

static struct zCapability sailgen_encCapabilityToCapability(bool, struct zEncCapability);

static struct zCapability sailgen_encCapabilityToCapability(bool zt, struct zEncCapability zc)
{
  __label__ cleanup_86, end_cleanup_87, end_function_85, end_block_exception_88;

  struct zCapability zcbz318;
  bool zinternal_E;
  {
    fbits zgaz3173;
    zgaz3173 = zc.zinternal_E;
    zinternal_E = sailgen_bit_to_bool(zgaz3173);
  }
  uint64_t zE;
  {
    sail_int zgsz3335;
    CREATE(sail_int)(&zgsz3335);
    CONVERT_OF(sail_int, mach_int)(&zgsz3335, INT64_C(6));
    lbits zgsz3336;
    CREATE(lbits)(&zgsz3336);
    sailgen_zzeros_implicit(&zgsz3336, zgsz3335);
    zE = CONVERT_OF(fbits, lbits)(zgsz3336, true);
    KILL(lbits)(&zgsz3336);
    KILL(sail_int)(&zgsz3335);
  }
  uint64_t zBs;
  {
    sail_int zgsz3333;
    CREATE(sail_int)(&zgsz3333);
    CONVERT_OF(sail_int, mach_int)(&zgsz3333, INT64_C(14));
    lbits zgsz3334;
    CREATE(lbits)(&zgsz3334);
    sailgen_zzeros_implicit(&zgsz3334, zgsz3333);
    zBs = CONVERT_OF(fbits, lbits)(zgsz3334, true);
    KILL(lbits)(&zgsz3334);
    KILL(sail_int)(&zgsz3333);
  }
  uint64_t zT;
  {
    int64_t zgaz3172;
    {
      sail_int zgsz3221;
      CREATE(sail_int)(&zgsz3221);
      CONVERT_OF(sail_int, mach_int)(&zgsz3221, INT64_C(14));
      sail_int zgsz3222;
      CREATE(sail_int)(&zgsz3222);
      CONVERT_OF(sail_int, mach_int)(&zgsz3222, INT64_C(2));
      sail_int zgsz3223;
      CREATE(sail_int)(&zgsz3223);
      sub_int(&zgsz3223, zgsz3221, zgsz3222);
      zgaz3172 = CONVERT_OF(mach_int, sail_int)(zgsz3223);
      KILL(sail_int)(&zgsz3223);
      KILL(sail_int)(&zgsz3222);
      KILL(sail_int)(&zgsz3221);
    }
    {
      sail_int zgsz3219;
      CREATE(sail_int)(&zgsz3219);
      CONVERT_OF(sail_int, mach_int)(&zgsz3219, zgaz3172);
      lbits zgsz3220;
      CREATE(lbits)(&zgsz3220);
      sailgen_zzeros_implicit(&zgsz3220, zgsz3219);
      zT = CONVERT_OF(fbits, lbits)(zgsz3220, true);
      KILL(lbits)(&zgsz3220);
      KILL(sail_int)(&zgsz3219);
    }
  }
  uint64_t zlenMSBs;
  {
    sail_int zgsz3331;
    CREATE(sail_int)(&zgsz3331);
    CONVERT_OF(sail_int, mach_int)(&zgsz3331, INT64_C(2));
    lbits zgsz3332;
    CREATE(lbits)(&zgsz3332);
    sailgen_zzeros_implicit(&zgsz3332, zgsz3331);
    zlenMSBs = CONVERT_OF(fbits, lbits)(zgsz3332, true);
    KILL(lbits)(&zgsz3332);
    KILL(sail_int)(&zgsz3331);
  }
  {
    unit zgsz336;
    if (zinternal_E) {
      {
        uint64_t zgaz3106;
        {
          uint64_t zgaz3102;
          zgaz3102 = zc.zT;
          int64_t zgaz3103;
          {
            sail_int zgsz3228;
            CREATE(sail_int)(&zgsz3228);
            CONVERT_OF(sail_int, mach_int)(&zgsz3228, zinternal_E_take_bits);
            sail_int zgsz3229;
            CREATE(sail_int)(&zgsz3229);
            CONVERT_OF(sail_int, mach_int)(&zgsz3229, INT64_C(1));
            sail_int zgsz3230;
            CREATE(sail_int)(&zgsz3230);
            sub_int(&zgsz3230, zgsz3228, zgsz3229);
            zgaz3103 = CONVERT_OF(mach_int, sail_int)(zgsz3230);
            KILL(sail_int)(&zgsz3230);
            KILL(sail_int)(&zgsz3229);
            KILL(sail_int)(&zgsz3228);
          }
          {
            lbits zgsz3224;
            CREATE(lbits)(&zgsz3224);
            CONVERT_OF(lbits, fbits)(&zgsz3224, zgaz3102, UINT64_C(12) , true);
            sail_int zgsz3225;
            CREATE(sail_int)(&zgsz3225);
            CONVERT_OF(sail_int, mach_int)(&zgsz3225, zgaz3103);
            sail_int zgsz3226;
            CREATE(sail_int)(&zgsz3226);
            CONVERT_OF(sail_int, mach_int)(&zgsz3226, INT64_C(0));
            lbits zgsz3227;
            CREATE(lbits)(&zgsz3227);
            vector_subrange_lbits(&zgsz3227, zgsz3224, zgsz3225, zgsz3226);
            zgaz3106 = CONVERT_OF(fbits, lbits)(zgsz3227, true);
            KILL(lbits)(&zgsz3227);
            KILL(sail_int)(&zgsz3226);
            KILL(sail_int)(&zgsz3225);
            KILL(lbits)(&zgsz3224);
          }
        }
        uint64_t zgaz3107;
        {
          uint64_t zgaz3104;
          zgaz3104 = zc.zB;
          int64_t zgaz3105;
          {
            sail_int zgsz3235;
            CREATE(sail_int)(&zgsz3235);
            CONVERT_OF(sail_int, mach_int)(&zgsz3235, zinternal_E_take_bits);
            sail_int zgsz3236;
            CREATE(sail_int)(&zgsz3236);
            CONVERT_OF(sail_int, mach_int)(&zgsz3236, INT64_C(1));
            sail_int zgsz3237;
            CREATE(sail_int)(&zgsz3237);
            sub_int(&zgsz3237, zgsz3235, zgsz3236);
            zgaz3105 = CONVERT_OF(mach_int, sail_int)(zgsz3237);
            KILL(sail_int)(&zgsz3237);
            KILL(sail_int)(&zgsz3236);
            KILL(sail_int)(&zgsz3235);
          }
          {
            lbits zgsz3231;
            CREATE(lbits)(&zgsz3231);
            CONVERT_OF(lbits, fbits)(&zgsz3231, zgaz3104, UINT64_C(14) , true);
            sail_int zgsz3232;
            CREATE(sail_int)(&zgsz3232);
            CONVERT_OF(sail_int, mach_int)(&zgsz3232, zgaz3105);
            sail_int zgsz3233;
            CREATE(sail_int)(&zgsz3233);
            CONVERT_OF(sail_int, mach_int)(&zgsz3233, INT64_C(0));
            lbits zgsz3234;
            CREATE(lbits)(&zgsz3234);
            vector_subrange_lbits(&zgsz3234, zgsz3231, zgsz3232, zgsz3233);
            zgaz3107 = CONVERT_OF(fbits, lbits)(zgsz3234, true);
            KILL(lbits)(&zgsz3234);
            KILL(sail_int)(&zgsz3233);
            KILL(sail_int)(&zgsz3232);
            KILL(lbits)(&zgsz3231);
          }
        }
        {
          lbits zgsz3238;
          CREATE(lbits)(&zgsz3238);
          CONVERT_OF(lbits, fbits)(&zgsz3238, zgaz3106, UINT64_C(3) , true);
          lbits zgsz3239;
          CREATE(lbits)(&zgsz3239);
          CONVERT_OF(lbits, fbits)(&zgsz3239, zgaz3107, UINT64_C(3) , true);
          lbits zgsz3240;
          CREATE(lbits)(&zgsz3240);
          append(&zgsz3240, zgsz3238, zgsz3239);
          zE = CONVERT_OF(fbits, lbits)(zgsz3240, true);
          KILL(lbits)(&zgsz3240);
          KILL(lbits)(&zgsz3239);
          KILL(lbits)(&zgsz3238);
        }
        unit zgsz341;
        zgsz341 = UNIT;
      }
      {
        zlenMSBs = UINT64_C(0b01);
        unit zgsz340;
        zgsz340 = UNIT;
      }
      {
        uint64_t zgaz3110;
        {
          uint64_t zgaz3108;
          zgaz3108 = zc.zT;
          int64_t zgaz3109;
          {
            sail_int zgsz3245;
            CREATE(sail_int)(&zgsz3245);
            CONVERT_OF(sail_int, mach_int)(&zgsz3245, zcap_mantissa_width);
            sail_int zgsz3246;
            CREATE(sail_int)(&zgsz3246);
            CONVERT_OF(sail_int, mach_int)(&zgsz3246, INT64_C(3));
            sail_int zgsz3247;
            CREATE(sail_int)(&zgsz3247);
            sub_int(&zgsz3247, zgsz3245, zgsz3246);
            zgaz3109 = CONVERT_OF(mach_int, sail_int)(zgsz3247);
            KILL(sail_int)(&zgsz3247);
            KILL(sail_int)(&zgsz3246);
            KILL(sail_int)(&zgsz3245);
          }
          {
            lbits zgsz3241;
            CREATE(lbits)(&zgsz3241);
            CONVERT_OF(lbits, fbits)(&zgsz3241, zgaz3108, UINT64_C(12) , true);
            sail_int zgsz3242;
            CREATE(sail_int)(&zgsz3242);
            CONVERT_OF(sail_int, mach_int)(&zgsz3242, zgaz3109);
            sail_int zgsz3243;
            CREATE(sail_int)(&zgsz3243);
            CONVERT_OF(sail_int, mach_int)(&zgsz3243, INT64_C(3));
            lbits zgsz3244;
            CREATE(lbits)(&zgsz3244);
            vector_subrange_lbits(&zgsz3244, zgsz3241, zgsz3242, zgsz3243);
            zgaz3110 = CONVERT_OF(fbits, lbits)(zgsz3244, true);
            KILL(lbits)(&zgsz3244);
            KILL(sail_int)(&zgsz3243);
            KILL(sail_int)(&zgsz3242);
            KILL(lbits)(&zgsz3241);
          }
        }
        uint64_t zgaz3111;
        {
          sail_int zgsz3251;
          CREATE(sail_int)(&zgsz3251);
          CONVERT_OF(sail_int, mach_int)(&zgsz3251, zinternal_E_take_bits);
          lbits zgsz3252;
          CREATE(lbits)(&zgsz3252);
          sailgen_zzeros_implicit(&zgsz3252, zgsz3251);
          zgaz3111 = CONVERT_OF(fbits, lbits)(zgsz3252, true);
          KILL(lbits)(&zgsz3252);
          KILL(sail_int)(&zgsz3251);
        }
        {
          lbits zgsz3248;
          CREATE(lbits)(&zgsz3248);
          CONVERT_OF(lbits, fbits)(&zgsz3248, zgaz3110, UINT64_C(9) , true);
          lbits zgsz3249;
          CREATE(lbits)(&zgsz3249);
          CONVERT_OF(lbits, fbits)(&zgsz3249, zgaz3111, UINT64_C(3) , true);
          lbits zgsz3250;
          CREATE(lbits)(&zgsz3250);
          append(&zgsz3250, zgsz3248, zgsz3249);
          zT = CONVERT_OF(fbits, lbits)(zgsz3250, true);
          KILL(lbits)(&zgsz3250);
          KILL(lbits)(&zgsz3249);
          KILL(lbits)(&zgsz3248);
        }
        unit zgsz339;
        zgsz339 = UNIT;
      }
      uint64_t zgaz3114;
      {
        uint64_t zgaz3112;
        zgaz3112 = zc.zB;
        int64_t zgaz3113;
        {
          sail_int zgsz3257;
          CREATE(sail_int)(&zgsz3257);
          CONVERT_OF(sail_int, mach_int)(&zgsz3257, zcap_mantissa_width);
          sail_int zgsz3258;
          CREATE(sail_int)(&zgsz3258);
          CONVERT_OF(sail_int, mach_int)(&zgsz3258, INT64_C(1));
          sail_int zgsz3259;
          CREATE(sail_int)(&zgsz3259);
          sub_int(&zgsz3259, zgsz3257, zgsz3258);
          zgaz3113 = CONVERT_OF(mach_int, sail_int)(zgsz3259);
          KILL(sail_int)(&zgsz3259);
          KILL(sail_int)(&zgsz3258);
          KILL(sail_int)(&zgsz3257);
        }
        {
          lbits zgsz3253;
          CREATE(lbits)(&zgsz3253);
          CONVERT_OF(lbits, fbits)(&zgsz3253, zgaz3112, UINT64_C(14) , true);
          sail_int zgsz3254;
          CREATE(sail_int)(&zgsz3254);
          CONVERT_OF(sail_int, mach_int)(&zgsz3254, zgaz3113);
          sail_int zgsz3255;
          CREATE(sail_int)(&zgsz3255);
          CONVERT_OF(sail_int, mach_int)(&zgsz3255, INT64_C(3));
          lbits zgsz3256;
          CREATE(lbits)(&zgsz3256);
          vector_subrange_lbits(&zgsz3256, zgsz3253, zgsz3254, zgsz3255);
          zgaz3114 = CONVERT_OF(fbits, lbits)(zgsz3256, true);
          KILL(lbits)(&zgsz3256);
          KILL(sail_int)(&zgsz3255);
          KILL(sail_int)(&zgsz3254);
          KILL(lbits)(&zgsz3253);
        }
      }
      uint64_t zgaz3115;
      {
        sail_int zgsz3263;
        CREATE(sail_int)(&zgsz3263);
        CONVERT_OF(sail_int, mach_int)(&zgsz3263, zinternal_E_take_bits);
        lbits zgsz3264;
        CREATE(lbits)(&zgsz3264);
        sailgen_zzeros_implicit(&zgsz3264, zgsz3263);
        zgaz3115 = CONVERT_OF(fbits, lbits)(zgsz3264, true);
        KILL(lbits)(&zgsz3264);
        KILL(sail_int)(&zgsz3263);
      }
      {
        lbits zgsz3260;
        CREATE(lbits)(&zgsz3260);
        CONVERT_OF(lbits, fbits)(&zgsz3260, zgaz3114, UINT64_C(11) , true);
        lbits zgsz3261;
        CREATE(lbits)(&zgsz3261);
        CONVERT_OF(lbits, fbits)(&zgsz3261, zgaz3115, UINT64_C(3) , true);
        lbits zgsz3262;
        CREATE(lbits)(&zgsz3262);
        append(&zgsz3262, zgsz3260, zgsz3261);
        zBs = CONVERT_OF(fbits, lbits)(zgsz3262, true);
        KILL(lbits)(&zgsz3262);
        KILL(lbits)(&zgsz3261);
        KILL(lbits)(&zgsz3260);
      }
      zgsz336 = UNIT;
    } else {
      {
        zlenMSBs = UINT64_C(0b00);
        unit zgsz338;
        zgsz338 = UNIT;
      }
      {
        zT = zc.zT;
        unit zgsz337;
        zgsz337 = UNIT;
      }
      zBs = zc.zB;
      zgsz336 = UNIT;
    }
  }
  uint64_t zcarry_out;
  {
    bool zgaz3171;
    {
      uint64_t zgaz3170;
      {
        int64_t zgaz3169;
        {
          sail_int zgsz3269;
          CREATE(sail_int)(&zgsz3269);
          CONVERT_OF(sail_int, mach_int)(&zgsz3269, zcap_mantissa_width);
          sail_int zgsz3270;
          CREATE(sail_int)(&zgsz3270);
          CONVERT_OF(sail_int, mach_int)(&zgsz3270, INT64_C(3));
          sail_int zgsz3271;
          CREATE(sail_int)(&zgsz3271);
          sub_int(&zgsz3271, zgsz3269, zgsz3270);
          zgaz3169 = CONVERT_OF(mach_int, sail_int)(zgsz3271);
          KILL(sail_int)(&zgsz3271);
          KILL(sail_int)(&zgsz3270);
          KILL(sail_int)(&zgsz3269);
        }
        {
          lbits zgsz3265;
          CREATE(lbits)(&zgsz3265);
          CONVERT_OF(lbits, fbits)(&zgsz3265, zBs, UINT64_C(14) , true);
          sail_int zgsz3266;
          CREATE(sail_int)(&zgsz3266);
          CONVERT_OF(sail_int, mach_int)(&zgsz3266, zgaz3169);
          sail_int zgsz3267;
          CREATE(sail_int)(&zgsz3267);
          CONVERT_OF(sail_int, mach_int)(&zgsz3267, INT64_C(0));
          lbits zgsz3268;
          CREATE(lbits)(&zgsz3268);
          vector_subrange_lbits(&zgsz3268, zgsz3265, zgsz3266, zgsz3267);
          zgaz3170 = CONVERT_OF(fbits, lbits)(zgsz3268, true);
          KILL(lbits)(&zgsz3268);
          KILL(sail_int)(&zgsz3267);
          KILL(sail_int)(&zgsz3266);
          KILL(lbits)(&zgsz3265);
        }
      }
      {
        lbits zgsz3272;
        CREATE(lbits)(&zgsz3272);
        CONVERT_OF(lbits, fbits)(&zgsz3272, zT, UINT64_C(12) , true);
        lbits zgsz3273;
        CREATE(lbits)(&zgsz3273);
        CONVERT_OF(lbits, fbits)(&zgsz3273, zgaz3170, UINT64_C(12) , true);
        zgaz3171 = sailgen_z8operatorz0zI_uz9(zgsz3272, zgsz3273);
        KILL(lbits)(&zgsz3273);
        KILL(lbits)(&zgsz3272);
      }
    }
    if (zgaz3171) {  zcarry_out = UINT64_C(0b01);  } else {  zcarry_out = UINT64_C(0b00);  }
  }
  uint64_t zTtop2;
  {
    uint64_t zgaz3168;
    {
      uint64_t zgaz3167;
      {
        int64_t zgaz3165;
        {
          sail_int zgsz3281;
          CREATE(sail_int)(&zgsz3281);
          CONVERT_OF(sail_int, mach_int)(&zgsz3281, zcap_mantissa_width);
          sail_int zgsz3282;
          CREATE(sail_int)(&zgsz3282);
          CONVERT_OF(sail_int, mach_int)(&zgsz3282, INT64_C(1));
          sail_int zgsz3283;
          CREATE(sail_int)(&zgsz3283);
          sub_int(&zgsz3283, zgsz3281, zgsz3282);
          zgaz3165 = CONVERT_OF(mach_int, sail_int)(zgsz3283);
          KILL(sail_int)(&zgsz3283);
          KILL(sail_int)(&zgsz3282);
          KILL(sail_int)(&zgsz3281);
        }
        int64_t zgaz3166;
        {
          sail_int zgsz3278;
          CREATE(sail_int)(&zgsz3278);
          CONVERT_OF(sail_int, mach_int)(&zgsz3278, zcap_mantissa_width);
          sail_int zgsz3279;
          CREATE(sail_int)(&zgsz3279);
          CONVERT_OF(sail_int, mach_int)(&zgsz3279, INT64_C(2));
          sail_int zgsz3280;
          CREATE(sail_int)(&zgsz3280);
          sub_int(&zgsz3280, zgsz3278, zgsz3279);
          zgaz3166 = CONVERT_OF(mach_int, sail_int)(zgsz3280);
          KILL(sail_int)(&zgsz3280);
          KILL(sail_int)(&zgsz3279);
          KILL(sail_int)(&zgsz3278);
        }
        {
          lbits zgsz3274;
          CREATE(lbits)(&zgsz3274);
          CONVERT_OF(lbits, fbits)(&zgsz3274, zBs, UINT64_C(14) , true);
          sail_int zgsz3275;
          CREATE(sail_int)(&zgsz3275);
          CONVERT_OF(sail_int, mach_int)(&zgsz3275, zgaz3165);
          sail_int zgsz3276;
          CREATE(sail_int)(&zgsz3276);
          CONVERT_OF(sail_int, mach_int)(&zgsz3276, zgaz3166);
          lbits zgsz3277;
          CREATE(lbits)(&zgsz3277);
          vector_subrange_lbits(&zgsz3277, zgsz3274, zgsz3275, zgsz3276);
          zgaz3167 = CONVERT_OF(fbits, lbits)(zgsz3277, true);
          KILL(lbits)(&zgsz3277);
          KILL(sail_int)(&zgsz3276);
          KILL(sail_int)(&zgsz3275);
          KILL(lbits)(&zgsz3274);
        }
      }
      {
        lbits zgsz3284;
        CREATE(lbits)(&zgsz3284);
        CONVERT_OF(lbits, fbits)(&zgsz3284, zgaz3167, UINT64_C(2) , true);
        lbits zgsz3285;
        CREATE(lbits)(&zgsz3285);
        CONVERT_OF(lbits, fbits)(&zgsz3285, zlenMSBs, UINT64_C(2) , true);
        lbits zgsz3286;
        CREATE(lbits)(&zgsz3286);
        add_bits(&zgsz3286, zgsz3284, zgsz3285);
        zgaz3168 = CONVERT_OF(fbits, lbits)(zgsz3286, true);
        KILL(lbits)(&zgsz3286);
        KILL(lbits)(&zgsz3285);
        KILL(lbits)(&zgsz3284);
      }
    }
    {
      lbits zgsz3287;
      CREATE(lbits)(&zgsz3287);
      CONVERT_OF(lbits, fbits)(&zgsz3287, zgaz3168, UINT64_C(2) , true);
      lbits zgsz3288;
      CREATE(lbits)(&zgsz3288);
      CONVERT_OF(lbits, fbits)(&zgsz3288, zcarry_out, UINT64_C(2) , true);
      lbits zgsz3289;
      CREATE(lbits)(&zgsz3289);
      add_bits(&zgsz3289, zgsz3287, zgsz3288);
      zTtop2 = CONVERT_OF(fbits, lbits)(zgsz3289, true);
      KILL(lbits)(&zgsz3289);
      KILL(lbits)(&zgsz3288);
      KILL(lbits)(&zgsz3287);
    }
  }
  struct zCapability zgaz3164;
  {
    uint64_t zgaz3120;
    {
      bool zgaz3116;
      {
        sail_int zgsz3300;
        CREATE(sail_int)(&zgsz3300);
        CONVERT_OF(sail_int, mach_int)(&zgsz3300, zcap_uperms_width);
        sail_int zgsz3301;
        CREATE(sail_int)(&zgsz3301);
        CONVERT_OF(sail_int, mach_int)(&zgsz3301, INT64_C(0));
        zgaz3116 = gt(zgsz3300, zgsz3301);
        KILL(sail_int)(&zgsz3301);
        KILL(sail_int)(&zgsz3300);
      }
      uint64_t zgaz3118;
      zgaz3118 = zc.zperms;
      int64_t zgaz3119;
      {
        int64_t zgaz3117;
        {
          sail_int zgsz3293;
          CREATE(sail_int)(&zgsz3293);
          CONVERT_OF(sail_int, mach_int)(&zgsz3293, zcap_hperms_width);
          sail_int zgsz3294;
          CREATE(sail_int)(&zgsz3294);
          CONVERT_OF(sail_int, mach_int)(&zgsz3294, zcap_uperms_width);
          sail_int zgsz3295;
          CREATE(sail_int)(&zgsz3295);
          add_int(&zgsz3295, zgsz3293, zgsz3294);
          zgaz3117 = CONVERT_OF(mach_int, sail_int)(zgsz3295);
          KILL(sail_int)(&zgsz3295);
          KILL(sail_int)(&zgsz3294);
          KILL(sail_int)(&zgsz3293);
        }
        {
          sail_int zgsz3290;
          CREATE(sail_int)(&zgsz3290);
          CONVERT_OF(sail_int, mach_int)(&zgsz3290, zgaz3117);
          sail_int zgsz3291;
          CREATE(sail_int)(&zgsz3291);
          CONVERT_OF(sail_int, mach_int)(&zgsz3291, INT64_C(1));
          sail_int zgsz3292;
          CREATE(sail_int)(&zgsz3292);
          sub_int(&zgsz3292, zgsz3290, zgsz3291);
          zgaz3119 = CONVERT_OF(mach_int, sail_int)(zgsz3292);
          KILL(sail_int)(&zgsz3292);
          KILL(sail_int)(&zgsz3291);
          KILL(sail_int)(&zgsz3290);
        }
      }
      {
        lbits zgsz3296;
        CREATE(lbits)(&zgsz3296);
        CONVERT_OF(lbits, fbits)(&zgsz3296, zgaz3118, UINT64_C(16) , true);
        sail_int zgsz3297;
        CREATE(sail_int)(&zgsz3297);
        CONVERT_OF(sail_int, mach_int)(&zgsz3297, zgaz3119);
        sail_int zgsz3298;
        CREATE(sail_int)(&zgsz3298);
        CONVERT_OF(sail_int, mach_int)(&zgsz3298, zcap_hperms_width);
        lbits zgsz3299;
        CREATE(lbits)(&zgsz3299);
        vector_subrange_lbits(&zgsz3299, zgsz3296, zgsz3297, zgsz3298);
        zgaz3120 = CONVERT_OF(fbits, lbits)(zgsz3299, true);
        KILL(lbits)(&zgsz3299);
        KILL(sail_int)(&zgsz3298);
        KILL(sail_int)(&zgsz3297);
        KILL(lbits)(&zgsz3296);
      }
    }
    bool zgaz3123;
    {
      fbits zgaz3122;
      {
        uint64_t zgaz3121;
        zgaz3121 = zc.zperms;
        {
          lbits zgsz3302;
          CREATE(lbits)(&zgsz3302);
          CONVERT_OF(lbits, fbits)(&zgsz3302, zgaz3121, UINT64_C(16) , true);
          sail_int zgsz3303;
          CREATE(sail_int)(&zgsz3303);
          CONVERT_OF(sail_int, mach_int)(&zgsz3303, INT64_C(11));
          zgaz3122 = bitvector_access(zgsz3302, zgsz3303);
          KILL(sail_int)(&zgsz3303);
          KILL(lbits)(&zgsz3302);
        }
      }
      zgaz3123 = sailgen_bit_to_bool(zgaz3122);
    }
    bool zgaz3126;
    {
      fbits zgaz3125;
      {
        uint64_t zgaz3124;
        zgaz3124 = zc.zperms;
        {
          lbits zgsz3304;
          CREATE(lbits)(&zgsz3304);
          CONVERT_OF(lbits, fbits)(&zgsz3304, zgaz3124, UINT64_C(16) , true);
          sail_int zgsz3305;
          CREATE(sail_int)(&zgsz3305);
          CONVERT_OF(sail_int, mach_int)(&zgsz3305, INT64_C(10));
          zgaz3125 = bitvector_access(zgsz3304, zgsz3305);
          KILL(sail_int)(&zgsz3305);
          KILL(lbits)(&zgsz3304);
        }
      }
      zgaz3126 = sailgen_bit_to_bool(zgaz3125);
    }
    bool zgaz3129;
    {
      fbits zgaz3128;
      {
        uint64_t zgaz3127;
        zgaz3127 = zc.zperms;
        {
          lbits zgsz3306;
          CREATE(lbits)(&zgsz3306);
          CONVERT_OF(lbits, fbits)(&zgsz3306, zgaz3127, UINT64_C(16) , true);
          sail_int zgsz3307;
          CREATE(sail_int)(&zgsz3307);
          CONVERT_OF(sail_int, mach_int)(&zgsz3307, INT64_C(9));
          zgaz3128 = bitvector_access(zgsz3306, zgsz3307);
          KILL(sail_int)(&zgsz3307);
          KILL(lbits)(&zgsz3306);
        }
      }
      zgaz3129 = sailgen_bit_to_bool(zgaz3128);
    }
    bool zgaz3132;
    {
      fbits zgaz3131;
      {
        uint64_t zgaz3130;
        zgaz3130 = zc.zperms;
        {
          lbits zgsz3308;
          CREATE(lbits)(&zgsz3308);
          CONVERT_OF(lbits, fbits)(&zgsz3308, zgaz3130, UINT64_C(16) , true);
          sail_int zgsz3309;
          CREATE(sail_int)(&zgsz3309);
          CONVERT_OF(sail_int, mach_int)(&zgsz3309, INT64_C(8));
          zgaz3131 = bitvector_access(zgsz3308, zgsz3309);
          KILL(sail_int)(&zgsz3309);
          KILL(lbits)(&zgsz3308);
        }
      }
      zgaz3132 = sailgen_bit_to_bool(zgaz3131);
    }
    bool zgaz3135;
    {
      fbits zgaz3134;
      {
        uint64_t zgaz3133;
        zgaz3133 = zc.zperms;
        {
          lbits zgsz3310;
          CREATE(lbits)(&zgsz3310);
          CONVERT_OF(lbits, fbits)(&zgsz3310, zgaz3133, UINT64_C(16) , true);
          sail_int zgsz3311;
          CREATE(sail_int)(&zgsz3311);
          CONVERT_OF(sail_int, mach_int)(&zgsz3311, INT64_C(7));
          zgaz3134 = bitvector_access(zgsz3310, zgsz3311);
          KILL(sail_int)(&zgsz3311);
          KILL(lbits)(&zgsz3310);
        }
      }
      zgaz3135 = sailgen_bit_to_bool(zgaz3134);
    }
    bool zgaz3138;
    {
      fbits zgaz3137;
      {
        uint64_t zgaz3136;
        zgaz3136 = zc.zperms;
        {
          lbits zgsz3312;
          CREATE(lbits)(&zgsz3312);
          CONVERT_OF(lbits, fbits)(&zgsz3312, zgaz3136, UINT64_C(16) , true);
          sail_int zgsz3313;
          CREATE(sail_int)(&zgsz3313);
          CONVERT_OF(sail_int, mach_int)(&zgsz3313, INT64_C(6));
          zgaz3137 = bitvector_access(zgsz3312, zgsz3313);
          KILL(sail_int)(&zgsz3313);
          KILL(lbits)(&zgsz3312);
        }
      }
      zgaz3138 = sailgen_bit_to_bool(zgaz3137);
    }
    bool zgaz3141;
    {
      fbits zgaz3140;
      {
        uint64_t zgaz3139;
        zgaz3139 = zc.zperms;
        {
          lbits zgsz3314;
          CREATE(lbits)(&zgsz3314);
          CONVERT_OF(lbits, fbits)(&zgsz3314, zgaz3139, UINT64_C(16) , true);
          sail_int zgsz3315;
          CREATE(sail_int)(&zgsz3315);
          CONVERT_OF(sail_int, mach_int)(&zgsz3315, INT64_C(5));
          zgaz3140 = bitvector_access(zgsz3314, zgsz3315);
          KILL(sail_int)(&zgsz3315);
          KILL(lbits)(&zgsz3314);
        }
      }
      zgaz3141 = sailgen_bit_to_bool(zgaz3140);
    }
    bool zgaz3144;
    {
      fbits zgaz3143;
      {
        uint64_t zgaz3142;
        zgaz3142 = zc.zperms;
        {
          lbits zgsz3316;
          CREATE(lbits)(&zgsz3316);
          CONVERT_OF(lbits, fbits)(&zgsz3316, zgaz3142, UINT64_C(16) , true);
          sail_int zgsz3317;
          CREATE(sail_int)(&zgsz3317);
          CONVERT_OF(sail_int, mach_int)(&zgsz3317, INT64_C(4));
          zgaz3143 = bitvector_access(zgsz3316, zgsz3317);
          KILL(sail_int)(&zgsz3317);
          KILL(lbits)(&zgsz3316);
        }
      }
      zgaz3144 = sailgen_bit_to_bool(zgaz3143);
    }
    bool zgaz3147;
    {
      fbits zgaz3146;
      {
        uint64_t zgaz3145;
        zgaz3145 = zc.zperms;
        {
          lbits zgsz3318;
          CREATE(lbits)(&zgsz3318);
          CONVERT_OF(lbits, fbits)(&zgsz3318, zgaz3145, UINT64_C(16) , true);
          sail_int zgsz3319;
          CREATE(sail_int)(&zgsz3319);
          CONVERT_OF(sail_int, mach_int)(&zgsz3319, INT64_C(3));
          zgaz3146 = bitvector_access(zgsz3318, zgsz3319);
          KILL(sail_int)(&zgsz3319);
          KILL(lbits)(&zgsz3318);
        }
      }
      zgaz3147 = sailgen_bit_to_bool(zgaz3146);
    }
    bool zgaz3150;
    {
      fbits zgaz3149;
      {
        uint64_t zgaz3148;
        zgaz3148 = zc.zperms;
        {
          lbits zgsz3320;
          CREATE(lbits)(&zgsz3320);
          CONVERT_OF(lbits, fbits)(&zgsz3320, zgaz3148, UINT64_C(16) , true);
          sail_int zgsz3321;
          CREATE(sail_int)(&zgsz3321);
          CONVERT_OF(sail_int, mach_int)(&zgsz3321, INT64_C(2));
          zgaz3149 = bitvector_access(zgsz3320, zgsz3321);
          KILL(sail_int)(&zgsz3321);
          KILL(lbits)(&zgsz3320);
        }
      }
      zgaz3150 = sailgen_bit_to_bool(zgaz3149);
    }
    bool zgaz3153;
    {
      fbits zgaz3152;
      {
        uint64_t zgaz3151;
        zgaz3151 = zc.zperms;
        {
          lbits zgsz3322;
          CREATE(lbits)(&zgsz3322);
          CONVERT_OF(lbits, fbits)(&zgsz3322, zgaz3151, UINT64_C(16) , true);
          sail_int zgsz3323;
          CREATE(sail_int)(&zgsz3323);
          CONVERT_OF(sail_int, mach_int)(&zgsz3323, INT64_C(1));
          zgaz3152 = bitvector_access(zgsz3322, zgsz3323);
          KILL(sail_int)(&zgsz3323);
          KILL(lbits)(&zgsz3322);
        }
      }
      zgaz3153 = sailgen_bit_to_bool(zgaz3152);
    }
    bool zgaz3156;
    {
      fbits zgaz3155;
      {
        uint64_t zgaz3154;
        zgaz3154 = zc.zperms;
        {
          lbits zgsz3324;
          CREATE(lbits)(&zgsz3324);
          CONVERT_OF(lbits, fbits)(&zgsz3324, zgaz3154, UINT64_C(16) , true);
          sail_int zgsz3325;
          CREATE(sail_int)(&zgsz3325);
          CONVERT_OF(sail_int, mach_int)(&zgsz3325, INT64_C(0));
          zgaz3155 = bitvector_access(zgsz3324, zgsz3325);
          KILL(sail_int)(&zgsz3325);
          KILL(lbits)(&zgsz3324);
        }
      }
      zgaz3156 = sailgen_bit_to_bool(zgaz3155);
    }
    uint64_t zgaz3157;
    zgaz3157 = zc.zreserved;
    bool zgaz3160;
    {
      fbits zgaz3159;
      {
        uint64_t zgaz3158;
        zgaz3158 = zc.zflags;
        {
          lbits zgsz3326;
          CREATE(lbits)(&zgsz3326);
          CONVERT_OF(lbits, fbits)(&zgsz3326, zgaz3158, UINT64_C(1) , true);
          sail_int zgsz3327;
          CREATE(sail_int)(&zgsz3327);
          CONVERT_OF(sail_int, mach_int)(&zgsz3327, INT64_C(0));
          zgaz3159 = bitvector_access(zgsz3326, zgsz3327);
          KILL(sail_int)(&zgsz3327);
          KILL(lbits)(&zgsz3326);
        }
      }
      zgaz3160 = sailgen_bit_to_bool(zgaz3159);
    }
    uint64_t zgaz3161;
    {
      lbits zgsz3328;
      CREATE(lbits)(&zgsz3328);
      CONVERT_OF(lbits, fbits)(&zgsz3328, zTtop2, UINT64_C(2) , true);
      lbits zgsz3329;
      CREATE(lbits)(&zgsz3329);
      CONVERT_OF(lbits, fbits)(&zgsz3329, zT, UINT64_C(12) , true);
      lbits zgsz3330;
      CREATE(lbits)(&zgsz3330);
      append(&zgsz3330, zgsz3328, zgsz3329);
      zgaz3161 = CONVERT_OF(fbits, lbits)(zgsz3330, true);
      KILL(lbits)(&zgsz3330);
      KILL(lbits)(&zgsz3329);
      KILL(lbits)(&zgsz3328);
    }
    uint64_t zgaz3162;
    zgaz3162 = zc.zotype;
    uint64_t zgaz3163;
    zgaz3163 = zc.zaddress;
    struct zCapability zgsz342;
    zgsz342.zB = zBs;
    zgsz342.zE = zE;
    zgsz342.zT = zgaz3161;
    zgsz342.zaccess_system_regs = zgaz3126;
    zgsz342.zaddress = zgaz3163;
    zgsz342.zflag_cap_mode = zgaz3160;
    zgsz342.zglobal = zgaz3156;
    zgsz342.zinternal_E = zinternal_E;
    zgsz342.zotype = zgaz3162;
    zgsz342.zpermit_cinvoke = zgaz3132;
    zgsz342.zpermit_execute = zgaz3153;
    zgsz342.zpermit_load = zgaz3150;
    zgsz342.zpermit_load_cap = zgaz3144;
    zgsz342.zpermit_seal = zgaz3135;
    zgsz342.zpermit_set_CID = zgaz3123;
    zgsz342.zpermit_store = zgaz3147;
    zgsz342.zpermit_store_cap = zgaz3141;
    zgsz342.zpermit_store_local_cap = zgaz3138;
    zgsz342.zpermit_unseal = zgaz3129;
    zgsz342.zreserved = zgaz3157;
    zgsz342.ztag = zt;
    zgsz342.zuperms = zgaz3120;
    zgaz3164 = zgsz342;
  }
  zcbz318 = zgaz3164;
  goto cleanup_86;
  /* unreachable after return */








  goto end_cleanup_87;
cleanup_86: ;








  goto end_function_85;
end_cleanup_87: ;
end_function_85: ;
  return zcbz318;
end_block_exception_88: ;
  struct zCapability zcbz343 = { .zuperms = UINT64_C(0xdeadc0de), .ztag = false, .zreserved = UINT64_C(0xdeadc0de), .zpermit_unseal = false, .zpermit_store_local_cap = false, .zpermit_store_cap = false, .zpermit_store = false, .zpermit_set_CID = false, .zpermit_seal = false, .zpermit_load_cap = false, .zpermit_load = false, .zpermit_execute = false, .zpermit_cinvoke = false, .zotype = UINT64_C(0xdeadc0de), .zinternal_E = false, .zglobal = false, .zflag_cap_mode = false, .zaddress = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zT = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  return zcbz343;
}

static struct zCapability sailgen_capBitsToCapability(bool, lbits);

static struct zCapability sailgen_capBitsToCapability(bool zt, lbits zc)
{
  __label__ end_function_90, end_block_exception_91;

  struct zCapability zcbz319;
  struct zEncCapability zgaz3174;
  zgaz3174 = sailgen_capBitsToEncCapability(zc);
  zcbz319 = sailgen_encCapabilityToCapability(zt, zgaz3174);

end_function_90: ;
  return zcbz319;
end_block_exception_91: ;
  struct zCapability zcbz344 = { .zuperms = UINT64_C(0xdeadc0de), .ztag = false, .zreserved = UINT64_C(0xdeadc0de), .zpermit_unseal = false, .zpermit_store_local_cap = false, .zpermit_store_cap = false, .zpermit_store = false, .zpermit_set_CID = false, .zpermit_seal = false, .zpermit_load_cap = false, .zpermit_load = false, .zpermit_execute = false, .zpermit_cinvoke = false, .zotype = UINT64_C(0xdeadc0de), .zinternal_E = false, .zglobal = false, .zflag_cap_mode = false, .zaddress = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zT = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  return zcbz344;
}

static struct zEncCapability sailgen_capToEncCap(struct zCapability);

static struct zEncCapability sailgen_capToEncCap(struct zCapability zcap)
{
  __label__ cleanup_94, end_cleanup_95, end_function_93, end_block_exception_96;

  struct zEncCapability zcbz320;
  uint64_t zt_hi;
  {
    uint64_t zgaz3201;
    zgaz3201 = zcap.zT;
    int64_t zgaz3202;
    {
      sail_int zgsz3341;
      CREATE(sail_int)(&zgsz3341);
      CONVERT_OF(sail_int, mach_int)(&zgsz3341, zcap_mantissa_width);
      sail_int zgsz3342;
      CREATE(sail_int)(&zgsz3342);
      CONVERT_OF(sail_int, mach_int)(&zgsz3342, INT64_C(3));
      sail_int zgsz3343;
      CREATE(sail_int)(&zgsz3343);
      sub_int(&zgsz3343, zgsz3341, zgsz3342);
      zgaz3202 = CONVERT_OF(mach_int, sail_int)(zgsz3343);
      KILL(sail_int)(&zgsz3343);
      KILL(sail_int)(&zgsz3342);
      KILL(sail_int)(&zgsz3341);
    }
    {
      lbits zgsz3337;
      CREATE(lbits)(&zgsz3337);
      CONVERT_OF(lbits, fbits)(&zgsz3337, zgaz3201, UINT64_C(14) , true);
      sail_int zgsz3338;
      CREATE(sail_int)(&zgsz3338);
      CONVERT_OF(sail_int, mach_int)(&zgsz3338, zgaz3202);
      sail_int zgsz3339;
      CREATE(sail_int)(&zgsz3339);
      CONVERT_OF(sail_int, mach_int)(&zgsz3339, zinternal_E_take_bits);
      lbits zgsz3340;
      CREATE(lbits)(&zgsz3340);
      vector_subrange_lbits(&zgsz3340, zgsz3337, zgsz3338, zgsz3339);
      zt_hi = CONVERT_OF(fbits, lbits)(zgsz3340, true);
      KILL(lbits)(&zgsz3340);
      KILL(sail_int)(&zgsz3339);
      KILL(sail_int)(&zgsz3338);
      KILL(lbits)(&zgsz3337);
    }
  }
  uint64_t zt_lo;
  {
    uint64_t zgaz3199;
    zgaz3199 = zcap.zT;
    int64_t zgaz3200;
    {
      sail_int zgsz3348;
      CREATE(sail_int)(&zgsz3348);
      CONVERT_OF(sail_int, mach_int)(&zgsz3348, zinternal_E_take_bits);
      sail_int zgsz3349;
      CREATE(sail_int)(&zgsz3349);
      CONVERT_OF(sail_int, mach_int)(&zgsz3349, INT64_C(1));
      sail_int zgsz3350;
      CREATE(sail_int)(&zgsz3350);
      sub_int(&zgsz3350, zgsz3348, zgsz3349);
      zgaz3200 = CONVERT_OF(mach_int, sail_int)(zgsz3350);
      KILL(sail_int)(&zgsz3350);
      KILL(sail_int)(&zgsz3349);
      KILL(sail_int)(&zgsz3348);
    }
    {
      lbits zgsz3344;
      CREATE(lbits)(&zgsz3344);
      CONVERT_OF(lbits, fbits)(&zgsz3344, zgaz3199, UINT64_C(14) , true);
      sail_int zgsz3345;
      CREATE(sail_int)(&zgsz3345);
      CONVERT_OF(sail_int, mach_int)(&zgsz3345, zgaz3200);
      sail_int zgsz3346;
      CREATE(sail_int)(&zgsz3346);
      CONVERT_OF(sail_int, mach_int)(&zgsz3346, INT64_C(0));
      lbits zgsz3347;
      CREATE(lbits)(&zgsz3347);
      vector_subrange_lbits(&zgsz3347, zgsz3344, zgsz3345, zgsz3346);
      zt_lo = CONVERT_OF(fbits, lbits)(zgsz3347, true);
      KILL(lbits)(&zgsz3347);
      KILL(sail_int)(&zgsz3346);
      KILL(sail_int)(&zgsz3345);
      KILL(lbits)(&zgsz3344);
    }
  }
  uint64_t zb_hi;
  {
    uint64_t zgaz3197;
    zgaz3197 = zcap.zB;
    int64_t zgaz3198;
    {
      sail_int zgsz3355;
      CREATE(sail_int)(&zgsz3355);
      CONVERT_OF(sail_int, mach_int)(&zgsz3355, zcap_mantissa_width);
      sail_int zgsz3356;
      CREATE(sail_int)(&zgsz3356);
      CONVERT_OF(sail_int, mach_int)(&zgsz3356, INT64_C(1));
      sail_int zgsz3357;
      CREATE(sail_int)(&zgsz3357);
      sub_int(&zgsz3357, zgsz3355, zgsz3356);
      zgaz3198 = CONVERT_OF(mach_int, sail_int)(zgsz3357);
      KILL(sail_int)(&zgsz3357);
      KILL(sail_int)(&zgsz3356);
      KILL(sail_int)(&zgsz3355);
    }
    {
      lbits zgsz3351;
      CREATE(lbits)(&zgsz3351);
      CONVERT_OF(lbits, fbits)(&zgsz3351, zgaz3197, UINT64_C(14) , true);
      sail_int zgsz3352;
      CREATE(sail_int)(&zgsz3352);
      CONVERT_OF(sail_int, mach_int)(&zgsz3352, zgaz3198);
      sail_int zgsz3353;
      CREATE(sail_int)(&zgsz3353);
      CONVERT_OF(sail_int, mach_int)(&zgsz3353, zinternal_E_take_bits);
      lbits zgsz3354;
      CREATE(lbits)(&zgsz3354);
      vector_subrange_lbits(&zgsz3354, zgsz3351, zgsz3352, zgsz3353);
      zb_hi = CONVERT_OF(fbits, lbits)(zgsz3354, true);
      KILL(lbits)(&zgsz3354);
      KILL(sail_int)(&zgsz3353);
      KILL(sail_int)(&zgsz3352);
      KILL(lbits)(&zgsz3351);
    }
  }
  uint64_t zb_lo;
  {
    uint64_t zgaz3195;
    zgaz3195 = zcap.zB;
    int64_t zgaz3196;
    {
      sail_int zgsz3362;
      CREATE(sail_int)(&zgsz3362);
      CONVERT_OF(sail_int, mach_int)(&zgsz3362, zinternal_E_take_bits);
      sail_int zgsz3363;
      CREATE(sail_int)(&zgsz3363);
      CONVERT_OF(sail_int, mach_int)(&zgsz3363, INT64_C(1));
      sail_int zgsz3364;
      CREATE(sail_int)(&zgsz3364);
      sub_int(&zgsz3364, zgsz3362, zgsz3363);
      zgaz3196 = CONVERT_OF(mach_int, sail_int)(zgsz3364);
      KILL(sail_int)(&zgsz3364);
      KILL(sail_int)(&zgsz3363);
      KILL(sail_int)(&zgsz3362);
    }
    {
      lbits zgsz3358;
      CREATE(lbits)(&zgsz3358);
      CONVERT_OF(lbits, fbits)(&zgsz3358, zgaz3195, UINT64_C(14) , true);
      sail_int zgsz3359;
      CREATE(sail_int)(&zgsz3359);
      CONVERT_OF(sail_int, mach_int)(&zgsz3359, zgaz3196);
      sail_int zgsz3360;
      CREATE(sail_int)(&zgsz3360);
      CONVERT_OF(sail_int, mach_int)(&zgsz3360, INT64_C(0));
      lbits zgsz3361;
      CREATE(lbits)(&zgsz3361);
      vector_subrange_lbits(&zgsz3361, zgsz3358, zgsz3359, zgsz3360);
      zb_lo = CONVERT_OF(fbits, lbits)(zgsz3361, true);
      KILL(lbits)(&zgsz3361);
      KILL(sail_int)(&zgsz3360);
      KILL(sail_int)(&zgsz3359);
      KILL(lbits)(&zgsz3358);
    }
  }
  {
    bool zgaz3175;
    zgaz3175 = zcap.zinternal_E;
    unit zgsz343;
    if (zgaz3175) {
      {
        uint64_t zgaz3177;
        zgaz3177 = zcap.zE;
        int64_t zgaz3178;
        {
          int64_t zgaz3176;
          {
            sail_int zgsz3368;
            CREATE(sail_int)(&zgsz3368);
            CONVERT_OF(sail_int, mach_int)(&zgsz3368, INT64_C(2));
            sail_int zgsz3369;
            CREATE(sail_int)(&zgsz3369);
            CONVERT_OF(sail_int, mach_int)(&zgsz3369, zinternal_E_take_bits);
            sail_int zgsz3370;
            CREATE(sail_int)(&zgsz3370);
            mult_int(&zgsz3370, zgsz3368, zgsz3369);
            zgaz3176 = CONVERT_OF(mach_int, sail_int)(zgsz3370);
            KILL(sail_int)(&zgsz3370);
            KILL(sail_int)(&zgsz3369);
            KILL(sail_int)(&zgsz3368);
          }
          {
            sail_int zgsz3365;
            CREATE(sail_int)(&zgsz3365);
            CONVERT_OF(sail_int, mach_int)(&zgsz3365, zgaz3176);
            sail_int zgsz3366;
            CREATE(sail_int)(&zgsz3366);
            CONVERT_OF(sail_int, mach_int)(&zgsz3366, INT64_C(1));
            sail_int zgsz3367;
            CREATE(sail_int)(&zgsz3367);
            sub_int(&zgsz3367, zgsz3365, zgsz3366);
            zgaz3178 = CONVERT_OF(mach_int, sail_int)(zgsz3367);
            KILL(sail_int)(&zgsz3367);
            KILL(sail_int)(&zgsz3366);
            KILL(sail_int)(&zgsz3365);
          }
        }
        {
          lbits zgsz3371;
          CREATE(lbits)(&zgsz3371);
          CONVERT_OF(lbits, fbits)(&zgsz3371, zgaz3177, UINT64_C(6) , true);
          sail_int zgsz3372;
          CREATE(sail_int)(&zgsz3372);
          CONVERT_OF(sail_int, mach_int)(&zgsz3372, zgaz3178);
          sail_int zgsz3373;
          CREATE(sail_int)(&zgsz3373);
          CONVERT_OF(sail_int, mach_int)(&zgsz3373, zinternal_E_take_bits);
          lbits zgsz3374;
          CREATE(lbits)(&zgsz3374);
          vector_subrange_lbits(&zgsz3374, zgsz3371, zgsz3372, zgsz3373);
          zt_lo = CONVERT_OF(fbits, lbits)(zgsz3374, true);
          KILL(lbits)(&zgsz3374);
          KILL(sail_int)(&zgsz3373);
          KILL(sail_int)(&zgsz3372);
          KILL(lbits)(&zgsz3371);
        }
        unit zgsz344;
        zgsz344 = UNIT;
      }
      uint64_t zgaz3179;
      zgaz3179 = zcap.zE;
      int64_t zgaz3180;
      {
        sail_int zgsz3379;
        CREATE(sail_int)(&zgsz3379);
        CONVERT_OF(sail_int, mach_int)(&zgsz3379, zinternal_E_take_bits);
        sail_int zgsz3380;
        CREATE(sail_int)(&zgsz3380);
        CONVERT_OF(sail_int, mach_int)(&zgsz3380, INT64_C(1));
        sail_int zgsz3381;
        CREATE(sail_int)(&zgsz3381);
        sub_int(&zgsz3381, zgsz3379, zgsz3380);
        zgaz3180 = CONVERT_OF(mach_int, sail_int)(zgsz3381);
        KILL(sail_int)(&zgsz3381);
        KILL(sail_int)(&zgsz3380);
        KILL(sail_int)(&zgsz3379);
      }
      {
        lbits zgsz3375;
        CREATE(lbits)(&zgsz3375);
        CONVERT_OF(lbits, fbits)(&zgsz3375, zgaz3179, UINT64_C(6) , true);
        sail_int zgsz3376;
        CREATE(sail_int)(&zgsz3376);
        CONVERT_OF(sail_int, mach_int)(&zgsz3376, zgaz3180);
        sail_int zgsz3377;
        CREATE(sail_int)(&zgsz3377);
        CONVERT_OF(sail_int, mach_int)(&zgsz3377, INT64_C(0));
        lbits zgsz3378;
        CREATE(lbits)(&zgsz3378);
        vector_subrange_lbits(&zgsz3378, zgsz3375, zgsz3376, zgsz3377);
        zb_lo = CONVERT_OF(fbits, lbits)(zgsz3378, true);
        KILL(lbits)(&zgsz3378);
        KILL(sail_int)(&zgsz3377);
        KILL(sail_int)(&zgsz3376);
        KILL(lbits)(&zgsz3375);
      }
      zgsz343 = UNIT;
    } else {  zgsz343 = UNIT;  }
  }
  struct zEncCapability zgaz3194;
  {
    uint64_t zgaz3183;
    {
      uint64_t zgaz3181;
      zgaz3181 = zcap.zuperms;
      uint64_t zgaz3182;
      zgaz3182 = sailgen_getCapHardPerms(zcap);
      {
        lbits zgsz3382;
        CREATE(lbits)(&zgsz3382);
        CONVERT_OF(lbits, fbits)(&zgsz3382, zgaz3181, UINT64_C(4) , true);
        lbits zgsz3383;
        CREATE(lbits)(&zgsz3383);
        CONVERT_OF(lbits, fbits)(&zgsz3383, zgaz3182, UINT64_C(12) , true);
        lbits zgsz3384;
        CREATE(lbits)(&zgsz3384);
        append(&zgsz3384, zgsz3382, zgsz3383);
        zgaz3183 = CONVERT_OF(fbits, lbits)(zgsz3384, true);
        KILL(lbits)(&zgsz3384);
        KILL(lbits)(&zgsz3383);
        KILL(lbits)(&zgsz3382);
      }
    }
    uint64_t zgaz3184;
    zgaz3184 = zcap.zreserved;
    uint64_t zgaz3186;
    {
      bool zgaz3185;
      zgaz3185 = zcap.zflag_cap_mode;
      zgaz3186 = sailgen_bool_to_bits(zgaz3185);
    }
    uint64_t zgaz3187;
    zgaz3187 = zcap.zotype;
    fbits zgaz3190;
    {
      uint64_t zgaz3189;
      {
        bool zgaz3188;
        zgaz3188 = zcap.zinternal_E;
        zgaz3189 = sailgen_bool_to_bits(zgaz3188);
      }
      {
        lbits zgsz3385;
        CREATE(lbits)(&zgsz3385);
        CONVERT_OF(lbits, fbits)(&zgsz3385, zgaz3189, UINT64_C(1) , true);
        sail_int zgsz3386;
        CREATE(sail_int)(&zgsz3386);
        CONVERT_OF(sail_int, mach_int)(&zgsz3386, INT64_C(0));
        zgaz3190 = bitvector_access(zgsz3385, zgsz3386);
        KILL(sail_int)(&zgsz3386);
        KILL(lbits)(&zgsz3385);
      }
    }
    uint64_t zgaz3191;
    {
      lbits zgsz3390;
      CREATE(lbits)(&zgsz3390);
      CONVERT_OF(lbits, fbits)(&zgsz3390, zt_hi, UINT64_C(9) , true);
      lbits zgsz3391;
      CREATE(lbits)(&zgsz3391);
      CONVERT_OF(lbits, fbits)(&zgsz3391, zt_lo, UINT64_C(3) , true);
      lbits zgsz3392;
      CREATE(lbits)(&zgsz3392);
      append(&zgsz3392, zgsz3390, zgsz3391);
      zgaz3191 = CONVERT_OF(fbits, lbits)(zgsz3392, true);
      KILL(lbits)(&zgsz3392);
      KILL(lbits)(&zgsz3391);
      KILL(lbits)(&zgsz3390);
    }
    uint64_t zgaz3192;
    {
      lbits zgsz3387;
      CREATE(lbits)(&zgsz3387);
      CONVERT_OF(lbits, fbits)(&zgsz3387, zb_hi, UINT64_C(11) , true);
      lbits zgsz3388;
      CREATE(lbits)(&zgsz3388);
      CONVERT_OF(lbits, fbits)(&zgsz3388, zb_lo, UINT64_C(3) , true);
      lbits zgsz3389;
      CREATE(lbits)(&zgsz3389);
      append(&zgsz3389, zgsz3387, zgsz3388);
      zgaz3192 = CONVERT_OF(fbits, lbits)(zgsz3389, true);
      KILL(lbits)(&zgsz3389);
      KILL(lbits)(&zgsz3388);
      KILL(lbits)(&zgsz3387);
    }
    uint64_t zgaz3193;
    zgaz3193 = zcap.zaddress;
    struct zEncCapability zgsz345;
    zgsz345.zB = zgaz3192;
    zgsz345.zT = zgaz3191;
    zgsz345.zaddress = zgaz3193;
    zgsz345.zflags = zgaz3186;
    zgsz345.zinternal_E = zgaz3190;
    zgsz345.zotype = zgaz3187;
    zgsz345.zperms = zgaz3183;
    zgsz345.zreserved = zgaz3184;
    zgaz3194 = zgsz345;
  }
  zcbz320 = zgaz3194;
  goto cleanup_94;
  /* unreachable after return */





  goto end_cleanup_95;
cleanup_94: ;





  goto end_function_93;
end_cleanup_95: ;
end_function_93: ;
  return zcbz320;
end_block_exception_96: ;
  struct zEncCapability zcbz345 = { .zreserved = UINT64_C(0xdeadc0de), .zperms = UINT64_C(0xdeadc0de), .zotype = UINT64_C(0xdeadc0de), .zinternal_E = UINT64_C(0), .zflags = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  return zcbz345;
}

static void sailgen_capToBits(lbits *rop, struct zCapability);

static void sailgen_capToBits(lbits *zcbz321, struct zCapability zcap)
{
  __label__ end_function_98, end_block_exception_99, end_function_175;

  struct zEncCapability zgaz3203;
  zgaz3203 = sailgen_capToEncCap(zcap);
  sailgen_encCapToBits((*(&zcbz321)), zgaz3203);

end_function_98: ;
  goto end_function_175;
end_block_exception_99: ;
  goto end_function_175;
end_function_175: ;
}

static lbits znull_cap_bits;
static void create_letbind_27(void) {    CREATE(lbits)(&znull_cap_bits);


  lbits zgsz346;
  CREATE(lbits)(&zgsz346);
  sailgen_capToBits(&zgsz346, znull_cap);
  COPY(lbits)(&znull_cap_bits, zgsz346);
  KILL(lbits)(&zgsz346);
let_end_100: ;
}
static void kill_letbind_27(void) {    KILL(lbits)(&znull_cap_bits);
}

static void sailgen_capToMemBits(lbits *rop, struct zCapability);

static void sailgen_capToMemBits(lbits *zcbz322, struct zCapability zcap)
{
  __label__ end_function_102, end_block_exception_103, end_function_174;

  lbits zgaz3204;
  CREATE(lbits)(&zgaz3204);
  sailgen_capToBits(&zgaz3204, zcap);
  xor_bits((*(&zcbz322)), zgaz3204, znull_cap_bits);
  KILL(lbits)(&zgaz3204);
end_function_102: ;
  goto end_function_174;
end_block_exception_103: ;
  goto end_function_174;
end_function_174: ;
}

static struct zCapability sailgen_memBitsToCapability(bool, lbits);

static struct zCapability sailgen_memBitsToCapability(bool ztag, lbits zb)
{
  __label__ end_function_105, end_block_exception_106;

  struct zCapability zcbz323;
  lbits zgaz3205;
  CREATE(lbits)(&zgaz3205);
  xor_bits(&zgaz3205, zb, znull_cap_bits);
  zcbz323 = sailgen_capBitsToCapability(ztag, zgaz3205);
  KILL(lbits)(&zgaz3205);
end_function_105: ;
  return zcbz323;
end_block_exception_106: ;
  struct zCapability zcbz346 = { .zuperms = UINT64_C(0xdeadc0de), .ztag = false, .zreserved = UINT64_C(0xdeadc0de), .zpermit_unseal = false, .zpermit_store_local_cap = false, .zpermit_store_cap = false, .zpermit_store = false, .zpermit_set_CID = false, .zpermit_seal = false, .zpermit_load_cap = false, .zpermit_load = false, .zpermit_execute = false, .zpermit_cinvoke = false, .zotype = UINT64_C(0xdeadc0de), .zinternal_E = false, .zglobal = false, .zflag_cap_mode = false, .zaddress = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zT = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  return zcbz346;
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

static void sailgen_getCapBoundsBits(struct ztuple_z8z5bv64zCz0z5bvz9 *zcbz324, struct zCapability zc)
{
  __label__ end_function_108, end_block_exception_109, end_function_173;

  int64_t zE;
  {
    int64_t zgaz3239;
    {
      uint64_t zgaz3238;
      zgaz3238 = zc.zE;
      {
        lbits zgsz3393;
        CREATE(lbits)(&zgsz3393);
        CONVERT_OF(lbits, fbits)(&zgsz3393, zgaz3238, UINT64_C(6) , true);
        sail_int zgsz3394;
        CREATE(sail_int)(&zgsz3394);
        sail_unsigned(&zgsz3394, zgsz3393);
        zgaz3239 = CONVERT_OF(mach_int, sail_int)(zgsz3394);
        KILL(sail_int)(&zgsz3394);
        KILL(lbits)(&zgsz3393);
      }
    }
    {
      sail_int zgsz3395;
      CREATE(sail_int)(&zgsz3395);
      CONVERT_OF(sail_int, mach_int)(&zgsz3395, zcap_max_E);
      sail_int zgsz3396;
      CREATE(sail_int)(&zgsz3396);
      CONVERT_OF(sail_int, mach_int)(&zgsz3396, zgaz3239);
      sail_int zgsz3397;
      CREATE(sail_int)(&zgsz3397);
      min_int(&zgsz3397, zgsz3395, zgsz3396);
      zE = CONVERT_OF(mach_int, sail_int)(zgsz3397);
      KILL(sail_int)(&zgsz3397);
      KILL(sail_int)(&zgsz3396);
      KILL(sail_int)(&zgsz3395);
    }
  }
  uint64_t za;
  za = zc.zaddress;
  uint64_t za3;
  {
    uint64_t zgaz3237;
    {
      int64_t zgaz3236;
      {
        int64_t zgaz3235;
        {
          sail_int zgsz3401;
          CREATE(sail_int)(&zgsz3401);
          CONVERT_OF(sail_int, mach_int)(&zgsz3401, zE);
          sail_int zgsz3402;
          CREATE(sail_int)(&zgsz3402);
          CONVERT_OF(sail_int, mach_int)(&zgsz3402, zcap_mantissa_width);
          sail_int zgsz3403;
          CREATE(sail_int)(&zgsz3403);
          add_int(&zgsz3403, zgsz3401, zgsz3402);
          zgaz3235 = CONVERT_OF(mach_int, sail_int)(zgsz3403);
          KILL(sail_int)(&zgsz3403);
          KILL(sail_int)(&zgsz3402);
          KILL(sail_int)(&zgsz3401);
        }
        {
          sail_int zgsz3398;
          CREATE(sail_int)(&zgsz3398);
          CONVERT_OF(sail_int, mach_int)(&zgsz3398, zgaz3235);
          sail_int zgsz3399;
          CREATE(sail_int)(&zgsz3399);
          CONVERT_OF(sail_int, mach_int)(&zgsz3399, INT64_C(3));
          sail_int zgsz3400;
          CREATE(sail_int)(&zgsz3400);
          sub_int(&zgsz3400, zgsz3398, zgsz3399);
          zgaz3236 = CONVERT_OF(mach_int, sail_int)(zgsz3400);
          KILL(sail_int)(&zgsz3400);
          KILL(sail_int)(&zgsz3399);
          KILL(sail_int)(&zgsz3398);
        }
      }
      {
        lbits zgsz3404;
        CREATE(lbits)(&zgsz3404);
        CONVERT_OF(lbits, fbits)(&zgsz3404, za, UINT64_C(64) , true);
        sail_int zgsz3405;
        CREATE(sail_int)(&zgsz3405);
        CONVERT_OF(sail_int, mach_int)(&zgsz3405, zgaz3236);
        lbits zgsz3406;
        CREATE(lbits)(&zgsz3406);
        shiftr(&zgsz3406, zgsz3404, zgsz3405);
        zgaz3237 = CONVERT_OF(fbits, lbits)(zgsz3406, true);
        KILL(lbits)(&zgsz3406);
        KILL(sail_int)(&zgsz3405);
        KILL(lbits)(&zgsz3404);
      }
    }
    {
      lbits zgsz3407;
      CREATE(lbits)(&zgsz3407);
      CONVERT_OF(lbits, fbits)(&zgsz3407, zgaz3237, UINT64_C(64) , true);
      sail_int zgsz3408;
      CREATE(sail_int)(&zgsz3408);
      CONVERT_OF(sail_int, mach_int)(&zgsz3408, INT64_C(3));
      lbits zgsz3409;
      CREATE(lbits)(&zgsz3409);
      sail_truncate(&zgsz3409, zgsz3407, zgsz3408);
      za3 = CONVERT_OF(fbits, lbits)(zgsz3409, true);
      KILL(lbits)(&zgsz3409);
      KILL(sail_int)(&zgsz3408);
      KILL(lbits)(&zgsz3407);
    }
  }
  uint64_t zB3;
  {
    uint64_t zgaz3234;
    zgaz3234 = zc.zB;
    {
      lbits zgsz3410;
      CREATE(lbits)(&zgsz3410);
      CONVERT_OF(lbits, fbits)(&zgsz3410, zgaz3234, UINT64_C(14) , true);
      sail_int zgsz3411;
      CREATE(sail_int)(&zgsz3411);
      CONVERT_OF(sail_int, mach_int)(&zgsz3411, INT64_C(3));
      lbits zgsz3412;
      CREATE(lbits)(&zgsz3412);
      sail_truncateLSB(&zgsz3412, zgsz3410, zgsz3411);
      zB3 = CONVERT_OF(fbits, lbits)(zgsz3412, true);
      KILL(lbits)(&zgsz3412);
      KILL(sail_int)(&zgsz3411);
      KILL(lbits)(&zgsz3410);
    }
  }
  uint64_t zT3;
  {
    uint64_t zgaz3233;
    zgaz3233 = zc.zT;
    {
      lbits zgsz3413;
      CREATE(lbits)(&zgsz3413);
      CONVERT_OF(lbits, fbits)(&zgsz3413, zgaz3233, UINT64_C(14) , true);
      sail_int zgsz3414;
      CREATE(sail_int)(&zgsz3414);
      CONVERT_OF(sail_int, mach_int)(&zgsz3414, INT64_C(3));
      lbits zgsz3415;
      CREATE(lbits)(&zgsz3415);
      sail_truncateLSB(&zgsz3415, zgsz3413, zgsz3414);
      zT3 = CONVERT_OF(fbits, lbits)(zgsz3415, true);
      KILL(lbits)(&zgsz3415);
      KILL(sail_int)(&zgsz3414);
      KILL(lbits)(&zgsz3413);
    }
  }
  uint64_t zR3;
  {
    lbits zgsz3485;
    CREATE(lbits)(&zgsz3485);
    CONVERT_OF(lbits, fbits)(&zgsz3485, zB3, UINT64_C(3) , true);
    lbits zgsz3486;
    CREATE(lbits)(&zgsz3486);
    CONVERT_OF(lbits, fbits)(&zgsz3486, UINT64_C(0b001), UINT64_C(3) , true);
    lbits zgsz3487;
    CREATE(lbits)(&zgsz3487);
    sub_bits(&zgsz3487, zgsz3485, zgsz3486);
    zR3 = CONVERT_OF(fbits, lbits)(zgsz3487, true);
    KILL(lbits)(&zgsz3487);
    KILL(lbits)(&zgsz3486);
    KILL(lbits)(&zgsz3485);
  }
  int64_t zaHi;
  {
    bool zgaz3232;
    {
      lbits zgsz3416;
      CREATE(lbits)(&zgsz3416);
      CONVERT_OF(lbits, fbits)(&zgsz3416, za3, UINT64_C(3) , true);
      lbits zgsz3417;
      CREATE(lbits)(&zgsz3417);
      CONVERT_OF(lbits, fbits)(&zgsz3417, zR3, UINT64_C(3) , true);
      zgaz3232 = sailgen_z8operatorz0zI_uz9(zgsz3416, zgsz3417);
      KILL(lbits)(&zgsz3417);
      KILL(lbits)(&zgsz3416);
    }
    if (zgaz3232) {  zaHi = INT64_C(1);  } else {  zaHi = INT64_C(0);  }
  }
  int64_t zbHi;
  {
    bool zgaz3231;
    {
      lbits zgsz3418;
      CREATE(lbits)(&zgsz3418);
      CONVERT_OF(lbits, fbits)(&zgsz3418, zB3, UINT64_C(3) , true);
      lbits zgsz3419;
      CREATE(lbits)(&zgsz3419);
      CONVERT_OF(lbits, fbits)(&zgsz3419, zR3, UINT64_C(3) , true);
      zgaz3231 = sailgen_z8operatorz0zI_uz9(zgsz3418, zgsz3419);
      KILL(lbits)(&zgsz3419);
      KILL(lbits)(&zgsz3418);
    }
    if (zgaz3231) {  zbHi = INT64_C(1);  } else {  zbHi = INT64_C(0);  }
  }
  int64_t ztHi;
  {
    bool zgaz3230;
    {
      lbits zgsz3420;
      CREATE(lbits)(&zgsz3420);
      CONVERT_OF(lbits, fbits)(&zgsz3420, zT3, UINT64_C(3) , true);
      lbits zgsz3421;
      CREATE(lbits)(&zgsz3421);
      CONVERT_OF(lbits, fbits)(&zgsz3421, zR3, UINT64_C(3) , true);
      zgaz3230 = sailgen_z8operatorz0zI_uz9(zgsz3420, zgsz3421);
      KILL(lbits)(&zgsz3421);
      KILL(lbits)(&zgsz3420);
    }
    if (zgaz3230) {  ztHi = INT64_C(1);  } else {  ztHi = INT64_C(0);  }
  }
  int64_t zcorrection_base;
  {
    sail_int zgsz3482;
    CREATE(sail_int)(&zgsz3482);
    CONVERT_OF(sail_int, mach_int)(&zgsz3482, zbHi);
    sail_int zgsz3483;
    CREATE(sail_int)(&zgsz3483);
    CONVERT_OF(sail_int, mach_int)(&zgsz3483, zaHi);
    sail_int zgsz3484;
    CREATE(sail_int)(&zgsz3484);
    sub_int(&zgsz3484, zgsz3482, zgsz3483);
    zcorrection_base = CONVERT_OF(mach_int, sail_int)(zgsz3484);
    KILL(sail_int)(&zgsz3484);
    KILL(sail_int)(&zgsz3483);
    KILL(sail_int)(&zgsz3482);
  }
  int64_t zcorrection_top;
  {
    sail_int zgsz3479;
    CREATE(sail_int)(&zgsz3479);
    CONVERT_OF(sail_int, mach_int)(&zgsz3479, ztHi);
    sail_int zgsz3480;
    CREATE(sail_int)(&zgsz3480);
    CONVERT_OF(sail_int, mach_int)(&zgsz3480, zaHi);
    sail_int zgsz3481;
    CREATE(sail_int)(&zgsz3481);
    sub_int(&zgsz3481, zgsz3479, zgsz3480);
    zcorrection_top = CONVERT_OF(mach_int, sail_int)(zgsz3481);
    KILL(sail_int)(&zgsz3481);
    KILL(sail_int)(&zgsz3480);
    KILL(sail_int)(&zgsz3479);
  }
  uint64_t za_top;
  {
    int64_t zgaz3229;
    {
      sail_int zgsz3425;
      CREATE(sail_int)(&zgsz3425);
      CONVERT_OF(sail_int, mach_int)(&zgsz3425, zE);
      sail_int zgsz3426;
      CREATE(sail_int)(&zgsz3426);
      CONVERT_OF(sail_int, mach_int)(&zgsz3426, zcap_mantissa_width);
      sail_int zgsz3427;
      CREATE(sail_int)(&zgsz3427);
      add_int(&zgsz3427, zgsz3425, zgsz3426);
      zgaz3229 = CONVERT_OF(mach_int, sail_int)(zgsz3427);
      KILL(sail_int)(&zgsz3427);
      KILL(sail_int)(&zgsz3426);
      KILL(sail_int)(&zgsz3425);
    }
    {
      lbits zgsz3422;
      CREATE(lbits)(&zgsz3422);
      CONVERT_OF(lbits, fbits)(&zgsz3422, za, UINT64_C(64) , true);
      sail_int zgsz3423;
      CREATE(sail_int)(&zgsz3423);
      CONVERT_OF(sail_int, mach_int)(&zgsz3423, zgaz3229);
      lbits zgsz3424;
      CREATE(lbits)(&zgsz3424);
      shiftr(&zgsz3424, zgsz3422, zgsz3423);
      za_top = CONVERT_OF(fbits, lbits)(zgsz3424, true);
      KILL(lbits)(&zgsz3424);
      KILL(sail_int)(&zgsz3423);
      KILL(lbits)(&zgsz3422);
    }
  }
  lbits zbase;
  CREATE(lbits)(&zbase);
  {
    lbits zgaz3228;
    CREATE(lbits)(&zgaz3228);
    {
      uint64_t zgaz3226;
      {
        lbits zgsz3433;
        CREATE(lbits)(&zgsz3433);
        CONVERT_OF(lbits, fbits)(&zgsz3433, za_top, UINT64_C(64) , true);
        sail_int zgsz3434;
        CREATE(sail_int)(&zgsz3434);
        CONVERT_OF(sail_int, mach_int)(&zgsz3434, zcorrection_base);
        lbits zgsz3435;
        CREATE(lbits)(&zgsz3435);
        add_bits_int(&zgsz3435, zgsz3433, zgsz3434);
        zgaz3226 = CONVERT_OF(fbits, lbits)(zgsz3435, true);
        KILL(lbits)(&zgsz3435);
        KILL(sail_int)(&zgsz3434);
        KILL(lbits)(&zgsz3433);
      }
      lbits zgaz3227;
      CREATE(lbits)(&zgaz3227);
      {
        uint64_t zgaz3224;
        zgaz3224 = zc.zB;
        sbits zgaz3225;
        {
          sail_int zgsz3430;
          CREATE(sail_int)(&zgsz3430);
          CONVERT_OF(sail_int, mach_int)(&zgsz3430, zE);
          lbits zgsz3431;
          CREATE(lbits)(&zgsz3431);
          sailgen_zzeros_implicit(&zgsz3431, zgsz3430);
          zgaz3225 = CONVERT_OF(sbits, lbits)(zgsz3431, true);
          KILL(lbits)(&zgsz3431);
          KILL(sail_int)(&zgsz3430);
        }
        {
          lbits zgsz3428;
          CREATE(lbits)(&zgsz3428);
          CONVERT_OF(lbits, fbits)(&zgsz3428, zgaz3224, UINT64_C(14) , true);
          lbits zgsz3429;
          CREATE(lbits)(&zgsz3429);
          CONVERT_OF(lbits, sbits)(&zgsz3429, zgaz3225, true);
          append(&zgaz3227, zgsz3428, zgsz3429);
          KILL(lbits)(&zgsz3429);
          KILL(lbits)(&zgsz3428);
        }
      }
      {
        lbits zgsz3432;
        CREATE(lbits)(&zgsz3432);
        CONVERT_OF(lbits, fbits)(&zgsz3432, zgaz3226, UINT64_C(64) , true);
        append(&zgaz3228, zgsz3432, zgaz3227);
        KILL(lbits)(&zgsz3432);
      }
      KILL(lbits)(&zgaz3227);
    }
    {
      sail_int zgsz3436;
      CREATE(sail_int)(&zgsz3436);
      CONVERT_OF(sail_int, mach_int)(&zgsz3436, zcap_len_width);
      sail_truncate(&zbase, zgaz3228, zgsz3436);
      KILL(sail_int)(&zgsz3436);
    }
    KILL(lbits)(&zgaz3228);
  }
  lbits ztop;
  CREATE(lbits)(&ztop);
  {
    lbits zgaz3223;
    CREATE(lbits)(&zgaz3223);
    {
      uint64_t zgaz3221;
      {
        lbits zgsz3442;
        CREATE(lbits)(&zgsz3442);
        CONVERT_OF(lbits, fbits)(&zgsz3442, za_top, UINT64_C(64) , true);
        sail_int zgsz3443;
        CREATE(sail_int)(&zgsz3443);
        CONVERT_OF(sail_int, mach_int)(&zgsz3443, zcorrection_top);
        lbits zgsz3444;
        CREATE(lbits)(&zgsz3444);
        add_bits_int(&zgsz3444, zgsz3442, zgsz3443);
        zgaz3221 = CONVERT_OF(fbits, lbits)(zgsz3444, true);
        KILL(lbits)(&zgsz3444);
        KILL(sail_int)(&zgsz3443);
        KILL(lbits)(&zgsz3442);
      }
      lbits zgaz3222;
      CREATE(lbits)(&zgaz3222);
      {
        uint64_t zgaz3219;
        zgaz3219 = zc.zT;
        sbits zgaz3220;
        {
          sail_int zgsz3439;
          CREATE(sail_int)(&zgsz3439);
          CONVERT_OF(sail_int, mach_int)(&zgsz3439, zE);
          lbits zgsz3440;
          CREATE(lbits)(&zgsz3440);
          sailgen_zzeros_implicit(&zgsz3440, zgsz3439);
          zgaz3220 = CONVERT_OF(sbits, lbits)(zgsz3440, true);
          KILL(lbits)(&zgsz3440);
          KILL(sail_int)(&zgsz3439);
        }
        {
          lbits zgsz3437;
          CREATE(lbits)(&zgsz3437);
          CONVERT_OF(lbits, fbits)(&zgsz3437, zgaz3219, UINT64_C(14) , true);
          lbits zgsz3438;
          CREATE(lbits)(&zgsz3438);
          CONVERT_OF(lbits, sbits)(&zgsz3438, zgaz3220, true);
          append(&zgaz3222, zgsz3437, zgsz3438);
          KILL(lbits)(&zgsz3438);
          KILL(lbits)(&zgsz3437);
        }
      }
      {
        lbits zgsz3441;
        CREATE(lbits)(&zgsz3441);
        CONVERT_OF(lbits, fbits)(&zgsz3441, zgaz3221, UINT64_C(64) , true);
        append(&zgaz3223, zgsz3441, zgaz3222);
        KILL(lbits)(&zgsz3441);
      }
      KILL(lbits)(&zgaz3222);
    }
    {
      sail_int zgsz3445;
      CREATE(sail_int)(&zgsz3445);
      CONVERT_OF(sail_int, mach_int)(&zgsz3445, zcap_len_width);
      sail_truncate(&ztop, zgaz3223, zgsz3445);
      KILL(sail_int)(&zgsz3445);
    }
    KILL(lbits)(&zgaz3223);
  }
  uint64_t zbase2;
  {
    uint64_t zgaz3218;
    {
      fbits zgaz3217;
      {
        int64_t zgaz3216;
        {
          sail_int zgsz3447;
          CREATE(sail_int)(&zgsz3447);
          CONVERT_OF(sail_int, mach_int)(&zgsz3447, zcap_addr_width);
          sail_int zgsz3448;
          CREATE(sail_int)(&zgsz3448);
          CONVERT_OF(sail_int, mach_int)(&zgsz3448, INT64_C(1));
          sail_int zgsz3449;
          CREATE(sail_int)(&zgsz3449);
          sub_int(&zgsz3449, zgsz3447, zgsz3448);
          zgaz3216 = CONVERT_OF(mach_int, sail_int)(zgsz3449);
          KILL(sail_int)(&zgsz3449);
          KILL(sail_int)(&zgsz3448);
          KILL(sail_int)(&zgsz3447);
        }
        {
          sail_int zgsz3446;
          CREATE(sail_int)(&zgsz3446);
          CONVERT_OF(sail_int, mach_int)(&zgsz3446, zgaz3216);
          zgaz3217 = bitvector_access(zbase, zgsz3446);
          KILL(sail_int)(&zgsz3446);
        }
      }
      uint64_t zgsz347;
      zgsz347 = UINT64_C(0b0);
      zgsz347 = update_fbits(zgsz347, INT64_C(0), zgaz3217);
      zgaz3218 = zgsz347;
    }
    {
      lbits zgsz3450;
      CREATE(lbits)(&zgsz3450);
      CONVERT_OF(lbits, fbits)(&zgsz3450, UINT64_C(0b0), UINT64_C(1) , true);
      lbits zgsz3451;
      CREATE(lbits)(&zgsz3451);
      CONVERT_OF(lbits, fbits)(&zgsz3451, zgaz3218, UINT64_C(1) , true);
      lbits zgsz3452;
      CREATE(lbits)(&zgsz3452);
      append(&zgsz3452, zgsz3450, zgsz3451);
      zbase2 = CONVERT_OF(fbits, lbits)(zgsz3452, true);
      KILL(lbits)(&zgsz3452);
      KILL(lbits)(&zgsz3451);
      KILL(lbits)(&zgsz3450);
    }
  }
  uint64_t ztop2;
  {
    int64_t zgaz3215;
    {
      sail_int zgsz3456;
      CREATE(sail_int)(&zgsz3456);
      CONVERT_OF(sail_int, mach_int)(&zgsz3456, zcap_addr_width);
      sail_int zgsz3457;
      CREATE(sail_int)(&zgsz3457);
      CONVERT_OF(sail_int, mach_int)(&zgsz3457, INT64_C(1));
      sail_int zgsz3458;
      CREATE(sail_int)(&zgsz3458);
      sub_int(&zgsz3458, zgsz3456, zgsz3457);
      zgaz3215 = CONVERT_OF(mach_int, sail_int)(zgsz3458);
      KILL(sail_int)(&zgsz3458);
      KILL(sail_int)(&zgsz3457);
      KILL(sail_int)(&zgsz3456);
    }
    {
      sail_int zgsz3453;
      CREATE(sail_int)(&zgsz3453);
      CONVERT_OF(sail_int, mach_int)(&zgsz3453, zcap_addr_width);
      sail_int zgsz3454;
      CREATE(sail_int)(&zgsz3454);
      CONVERT_OF(sail_int, mach_int)(&zgsz3454, zgaz3215);
      lbits zgsz3455;
      CREATE(lbits)(&zgsz3455);
      vector_subrange_lbits(&zgsz3455, ztop, zgsz3453, zgsz3454);
      ztop2 = CONVERT_OF(fbits, lbits)(zgsz3455, true);
      KILL(lbits)(&zgsz3455);
      KILL(sail_int)(&zgsz3454);
      KILL(sail_int)(&zgsz3453);
    }
  }
  {
    bool zgaz3210;
    {
      bool zgaz3209;
      {
        int64_t zgaz3206;
        {
          sail_int zgsz3461;
          CREATE(sail_int)(&zgsz3461);
          CONVERT_OF(sail_int, mach_int)(&zgsz3461, zcap_max_E);
          sail_int zgsz3462;
          CREATE(sail_int)(&zgsz3462);
          CONVERT_OF(sail_int, mach_int)(&zgsz3462, INT64_C(1));
          sail_int zgsz3463;
          CREATE(sail_int)(&zgsz3463);
          sub_int(&zgsz3463, zgsz3461, zgsz3462);
          zgaz3206 = CONVERT_OF(mach_int, sail_int)(zgsz3463);
          KILL(sail_int)(&zgsz3463);
          KILL(sail_int)(&zgsz3462);
          KILL(sail_int)(&zgsz3461);
        }
        {
          sail_int zgsz3459;
          CREATE(sail_int)(&zgsz3459);
          CONVERT_OF(sail_int, mach_int)(&zgsz3459, zE);
          sail_int zgsz3460;
          CREATE(sail_int)(&zgsz3460);
          CONVERT_OF(sail_int, mach_int)(&zgsz3460, zgaz3206);
          zgaz3209 = lt(zgsz3459, zgsz3460);
          KILL(sail_int)(&zgsz3460);
          KILL(sail_int)(&zgsz3459);
        }
      }
      bool zgsz348;
      if (zgaz3209) {
        int64_t zgaz3208;
        {
          uint64_t zgaz3207;
          {
            lbits zgsz3466;
            CREATE(lbits)(&zgsz3466);
            CONVERT_OF(lbits, fbits)(&zgsz3466, ztop2, UINT64_C(2) , true);
            lbits zgsz3467;
            CREATE(lbits)(&zgsz3467);
            CONVERT_OF(lbits, fbits)(&zgsz3467, zbase2, UINT64_C(2) , true);
            lbits zgsz3468;
            CREATE(lbits)(&zgsz3468);
            sub_bits(&zgsz3468, zgsz3466, zgsz3467);
            zgaz3207 = CONVERT_OF(fbits, lbits)(zgsz3468, true);
            KILL(lbits)(&zgsz3468);
            KILL(lbits)(&zgsz3467);
            KILL(lbits)(&zgsz3466);
          }
          {
            lbits zgsz3464;
            CREATE(lbits)(&zgsz3464);
            CONVERT_OF(lbits, fbits)(&zgsz3464, zgaz3207, UINT64_C(2) , true);
            sail_int zgsz3465;
            CREATE(sail_int)(&zgsz3465);
            sail_unsigned(&zgsz3465, zgsz3464);
            zgaz3208 = CONVERT_OF(mach_int, sail_int)(zgsz3465);
            KILL(sail_int)(&zgsz3465);
            KILL(lbits)(&zgsz3464);
          }
        }
        {
          sail_int zgsz3469;
          CREATE(sail_int)(&zgsz3469);
          CONVERT_OF(sail_int, mach_int)(&zgsz3469, zgaz3208);
          sail_int zgsz3470;
          CREATE(sail_int)(&zgsz3470);
          CONVERT_OF(sail_int, mach_int)(&zgsz3470, INT64_C(1));
          zgsz348 = gt(zgsz3469, zgsz3470);
          KILL(sail_int)(&zgsz3470);
          KILL(sail_int)(&zgsz3469);
        }
      } else {  zgsz348 = false;  }
      zgaz3210 = zgsz348;
    }
    unit zgsz349;
    if (zgaz3210) {
      fbits zgaz3212;
      {
        fbits zgaz3211;
        {
          sail_int zgsz3471;
          CREATE(sail_int)(&zgsz3471);
          CONVERT_OF(sail_int, mach_int)(&zgsz3471, zcap_addr_width);
          zgaz3211 = bitvector_access(ztop, zgsz3471);
          KILL(sail_int)(&zgsz3471);
        }
        zgaz3212 = sailgen_not_bit(zgaz3211);
      }
      {
        sail_int zgsz3472;
        CREATE(sail_int)(&zgsz3472);
        CONVERT_OF(sail_int, mach_int)(&zgsz3472, zcap_addr_width);
        update_lbits(&ztop, ztop, zgsz3472, zgaz3212);
        KILL(sail_int)(&zgsz3472);
      }
      zgsz349 = UNIT;
    } else {  zgsz349 = UNIT;  }
  }
  uint64_t zgaz3214;
  {
    int64_t zgaz3213;
    {
      sail_int zgsz3476;
      CREATE(sail_int)(&zgsz3476);
      CONVERT_OF(sail_int, mach_int)(&zgsz3476, zcap_addr_width);
      sail_int zgsz3477;
      CREATE(sail_int)(&zgsz3477);
      CONVERT_OF(sail_int, mach_int)(&zgsz3477, INT64_C(1));
      sail_int zgsz3478;
      CREATE(sail_int)(&zgsz3478);
      sub_int(&zgsz3478, zgsz3476, zgsz3477);
      zgaz3213 = CONVERT_OF(mach_int, sail_int)(zgsz3478);
      KILL(sail_int)(&zgsz3478);
      KILL(sail_int)(&zgsz3477);
      KILL(sail_int)(&zgsz3476);
    }
    {
      sail_int zgsz3473;
      CREATE(sail_int)(&zgsz3473);
      CONVERT_OF(sail_int, mach_int)(&zgsz3473, zgaz3213);
      sail_int zgsz3474;
      CREATE(sail_int)(&zgsz3474);
      CONVERT_OF(sail_int, mach_int)(&zgsz3474, INT64_C(0));
      lbits zgsz3475;
      CREATE(lbits)(&zgsz3475);
      vector_subrange_lbits(&zgsz3475, zbase, zgsz3473, zgsz3474);
      zgaz3214 = CONVERT_OF(fbits, lbits)(zgsz3475, true);
      KILL(lbits)(&zgsz3475);
      KILL(sail_int)(&zgsz3474);
      KILL(sail_int)(&zgsz3473);
    }
  }
  struct ztuple_z8z5bv64zCz0z5bvz9 zgsz350;
  CREATE(ztuple_z8z5bv64zCz0z5bvz9)(&zgsz350);
  zgsz350.ztup0 = zgaz3214;
  COPY(lbits)(&((&zgsz350)->ztup1), ztop);
  COPY(ztuple_z8z5bv64zCz0z5bvz9)((*(&zcbz324)), zgsz350);
  KILL(ztuple_z8z5bv64zCz0z5bvz9)(&zgsz350);



  KILL(lbits)(&ztop);
  KILL(lbits)(&zbase);












end_function_108: ;
  goto end_function_173;
end_block_exception_109: ;
  goto end_function_173;
end_function_173: ;
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


static void sailgen_getCapBounds(struct ztuple_z8z5izCz0z5iz9 *zcbz325, struct zCapability zcap)
{
  __label__ case_112, finish_match_111, end_function_113, end_block_exception_114, end_function_172;

  struct ztuple_z8z5bv64zCz0z5bvz9 zgaz3240;
  CREATE(ztuple_z8z5bv64zCz0z5bvz9)(&zgaz3240);
  sailgen_getCapBoundsBits(&zgaz3240, zcap);
  struct ztuple_z8z5izCz0z5iz9 zgsz351;
  CREATE(ztuple_z8z5izCz0z5iz9)(&zgsz351);
  {
    uint64_t zbase;
    zbase = zgaz3240.ztup0;
    lbits ztop;
    CREATE(lbits)(&ztop);
    COPY(lbits)(&ztop, zgaz3240.ztup1);
    sail_int zgaz3241;
    CREATE(sail_int)(&zgaz3241);
    {
      lbits zgsz3488;
      CREATE(lbits)(&zgsz3488);
      CONVERT_OF(lbits, fbits)(&zgsz3488, zbase, UINT64_C(64) , true);
      sail_unsigned(&zgaz3241, zgsz3488);
      KILL(lbits)(&zgsz3488);
    }
    sail_int zgaz3242;
    CREATE(sail_int)(&zgaz3242);
    sail_unsigned(&zgaz3242, ztop);
    struct ztuple_z8z5izCz0z5iz9 zgsz352;
    CREATE(ztuple_z8z5izCz0z5iz9)(&zgsz352);
    COPY(sail_int)(&((&zgsz352)->ztup0), zgaz3241);
    COPY(sail_int)(&((&zgsz352)->ztup1), zgaz3242);
    COPY(ztuple_z8z5izCz0z5iz9)(&zgsz351, zgsz352);
    KILL(ztuple_z8z5izCz0z5iz9)(&zgsz352);
    KILL(sail_int)(&zgaz3242);
    KILL(sail_int)(&zgaz3241);
    KILL(lbits)(&ztop);
    goto finish_match_111;
  }
case_112: ;
  sail_match_failure("getCapBounds");
finish_match_111: ;
  COPY(ztuple_z8z5izCz0z5iz9)((*(&zcbz325)), zgsz351);
  KILL(ztuple_z8z5izCz0z5iz9)(&zgsz351);
  KILL(ztuple_z8z5bv64zCz0z5bvz9)(&zgaz3240);
end_function_113: ;
  goto end_function_172;
end_block_exception_114: ;
  goto end_function_172;
end_function_172: ;
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
  __label__ end_function_116, end_block_exception_117;

  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz326;
  lbits zext_base;
  CREATE(lbits)(&zext_base);
  {
    lbits zgsz3595;
    CREATE(lbits)(&zgsz3595);
    CONVERT_OF(lbits, fbits)(&zgsz3595, UINT64_C(0b0), UINT64_C(1) , true);
    lbits zgsz3596;
    CREATE(lbits)(&zgsz3596);
    CONVERT_OF(lbits, fbits)(&zgsz3596, zbase, UINT64_C(64) , true);
    append(&zext_base, zgsz3595, zgsz3596);
    KILL(lbits)(&zgsz3596);
    KILL(lbits)(&zgsz3595);
  }
  lbits zlength;
  CREATE(lbits)(&zlength);
  sub_bits(&zlength, ztop, zext_base);
  int64_t ze;
  {
    int64_t zgaz3276;
    {
      uint64_t zgaz3275;
      {
        int64_t zgaz3274;
        {
          sail_int zgsz3492;
          CREATE(sail_int)(&zgsz3492);
          CONVERT_OF(sail_int, mach_int)(&zgsz3492, zcap_mantissa_width);
          sail_int zgsz3493;
          CREATE(sail_int)(&zgsz3493);
          CONVERT_OF(sail_int, mach_int)(&zgsz3493, INT64_C(1));
          sail_int zgsz3494;
          CREATE(sail_int)(&zgsz3494);
          sub_int(&zgsz3494, zgsz3492, zgsz3493);
          zgaz3274 = CONVERT_OF(mach_int, sail_int)(zgsz3494);
          KILL(sail_int)(&zgsz3494);
          KILL(sail_int)(&zgsz3493);
          KILL(sail_int)(&zgsz3492);
        }
        {
          sail_int zgsz3489;
          CREATE(sail_int)(&zgsz3489);
          CONVERT_OF(sail_int, mach_int)(&zgsz3489, zcap_addr_width);
          sail_int zgsz3490;
          CREATE(sail_int)(&zgsz3490);
          CONVERT_OF(sail_int, mach_int)(&zgsz3490, zgaz3274);
          lbits zgsz3491;
          CREATE(lbits)(&zgsz3491);
          vector_subrange_lbits(&zgsz3491, zlength, zgsz3489, zgsz3490);
          zgaz3275 = CONVERT_OF(fbits, lbits)(zgsz3491, true);
          KILL(lbits)(&zgsz3491);
          KILL(sail_int)(&zgsz3490);
          KILL(sail_int)(&zgsz3489);
        }
      }
      {
        lbits zgsz3495;
        CREATE(lbits)(&zgsz3495);
        CONVERT_OF(lbits, fbits)(&zgsz3495, zgaz3275, UINT64_C(52) , true);
        sail_int zgsz3496;
        CREATE(sail_int)(&zgsz3496);
        count_leading_zeros(&zgsz3496, zgsz3495);
        zgaz3276 = CONVERT_OF(mach_int, sail_int)(zgsz3496);
        KILL(sail_int)(&zgsz3496);
        KILL(lbits)(&zgsz3495);
      }
    }
    {
      sail_int zgsz3497;
      CREATE(sail_int)(&zgsz3497);
      CONVERT_OF(sail_int, mach_int)(&zgsz3497, zcap_max_E);
      sail_int zgsz3498;
      CREATE(sail_int)(&zgsz3498);
      CONVERT_OF(sail_int, mach_int)(&zgsz3498, zgaz3276);
      sail_int zgsz3499;
      CREATE(sail_int)(&zgsz3499);
      sub_int(&zgsz3499, zgsz3497, zgsz3498);
      ze = CONVERT_OF(mach_int, sail_int)(zgsz3499);
      KILL(sail_int)(&zgsz3499);
      KILL(sail_int)(&zgsz3498);
      KILL(sail_int)(&zgsz3497);
    }
  }
  bool zie;
  {
    bool zgaz3273;
    {
      sail_int zgsz3504;
      CREATE(sail_int)(&zgsz3504);
      CONVERT_OF(sail_int, mach_int)(&zgsz3504, ze);
      sail_int zgsz3505;
      CREATE(sail_int)(&zgsz3505);
      CONVERT_OF(sail_int, mach_int)(&zgsz3505, INT64_C(0));
      zgaz3273 = sailgen_neq_int(zgsz3504, zgsz3505);
      KILL(sail_int)(&zgsz3505);
      KILL(sail_int)(&zgsz3504);
    }
    bool zgsz354;
    if (zgaz3273) {  zgsz354 = true;  } else {
      fbits zgaz3272;
      {
        int64_t zgaz3271;
        {
          sail_int zgsz3501;
          CREATE(sail_int)(&zgsz3501);
          CONVERT_OF(sail_int, mach_int)(&zgsz3501, zcap_mantissa_width);
          sail_int zgsz3502;
          CREATE(sail_int)(&zgsz3502);
          CONVERT_OF(sail_int, mach_int)(&zgsz3502, INT64_C(2));
          sail_int zgsz3503;
          CREATE(sail_int)(&zgsz3503);
          sub_int(&zgsz3503, zgsz3501, zgsz3502);
          zgaz3271 = CONVERT_OF(mach_int, sail_int)(zgsz3503);
          KILL(sail_int)(&zgsz3503);
          KILL(sail_int)(&zgsz3502);
          KILL(sail_int)(&zgsz3501);
        }
        {
          sail_int zgsz3500;
          CREATE(sail_int)(&zgsz3500);
          CONVERT_OF(sail_int, mach_int)(&zgsz3500, zgaz3271);
          zgaz3272 = bitvector_access(zlength, zgsz3500);
          KILL(sail_int)(&zgsz3500);
        }
      }
      zgsz354 = eq_bit(zgaz3272, UINT64_C(1));
    }
    zie = zgsz354;
  }
  uint64_t zBbits;
  {
    lbits zgsz3592;
    CREATE(lbits)(&zgsz3592);
    CONVERT_OF(lbits, fbits)(&zgsz3592, zbase, UINT64_C(64) , true);
    sail_int zgsz3593;
    CREATE(sail_int)(&zgsz3593);
    CONVERT_OF(sail_int, mach_int)(&zgsz3593, zcap_mantissa_width);
    lbits zgsz3594;
    CREATE(lbits)(&zgsz3594);
    sail_truncate(&zgsz3594, zgsz3592, zgsz3593);
    zBbits = CONVERT_OF(fbits, lbits)(zgsz3594, true);
    KILL(lbits)(&zgsz3594);
    KILL(sail_int)(&zgsz3593);
    KILL(lbits)(&zgsz3592);
  }
  uint64_t zTbits;
  {
    sail_int zgsz3590;
    CREATE(sail_int)(&zgsz3590);
    CONVERT_OF(sail_int, mach_int)(&zgsz3590, zcap_mantissa_width);
    lbits zgsz3591;
    CREATE(lbits)(&zgsz3591);
    sail_truncate(&zgsz3591, ztop, zgsz3590);
    zTbits = CONVERT_OF(fbits, lbits)(zgsz3591, true);
    KILL(lbits)(&zgsz3591);
    KILL(sail_int)(&zgsz3590);
  }
  bool zlostSignificantTop;
  zlostSignificantTop = false;
  bool zlostSignificantBase;
  zlostSignificantBase = false;
  bool zincE;
  zincE = false;
  {
    unit zgsz355;
    if (zie) {
      uint64_t zB_ie;
      {
        uint64_t zgaz3266;
        {
          int64_t zgaz3265;
          {
            sail_int zgsz3509;
            CREATE(sail_int)(&zgsz3509);
            CONVERT_OF(sail_int, mach_int)(&zgsz3509, ze);
            sail_int zgsz3510;
            CREATE(sail_int)(&zgsz3510);
            CONVERT_OF(sail_int, mach_int)(&zgsz3510, INT64_C(3));
            sail_int zgsz3511;
            CREATE(sail_int)(&zgsz3511);
            add_int(&zgsz3511, zgsz3509, zgsz3510);
            zgaz3265 = CONVERT_OF(mach_int, sail_int)(zgsz3511);
            KILL(sail_int)(&zgsz3511);
            KILL(sail_int)(&zgsz3510);
            KILL(sail_int)(&zgsz3509);
          }
          {
            lbits zgsz3506;
            CREATE(lbits)(&zgsz3506);
            CONVERT_OF(lbits, fbits)(&zgsz3506, zbase, UINT64_C(64) , true);
            sail_int zgsz3507;
            CREATE(sail_int)(&zgsz3507);
            CONVERT_OF(sail_int, mach_int)(&zgsz3507, zgaz3265);
            lbits zgsz3508;
            CREATE(lbits)(&zgsz3508);
            shiftr(&zgsz3508, zgsz3506, zgsz3507);
            zgaz3266 = CONVERT_OF(fbits, lbits)(zgsz3508, true);
            KILL(lbits)(&zgsz3508);
            KILL(sail_int)(&zgsz3507);
            KILL(lbits)(&zgsz3506);
          }
        }
        int64_t zgaz3267;
        {
          sail_int zgsz3515;
          CREATE(sail_int)(&zgsz3515);
          CONVERT_OF(sail_int, mach_int)(&zgsz3515, zcap_mantissa_width);
          sail_int zgsz3516;
          CREATE(sail_int)(&zgsz3516);
          CONVERT_OF(sail_int, mach_int)(&zgsz3516, INT64_C(3));
          sail_int zgsz3517;
          CREATE(sail_int)(&zgsz3517);
          sub_int(&zgsz3517, zgsz3515, zgsz3516);
          zgaz3267 = CONVERT_OF(mach_int, sail_int)(zgsz3517);
          KILL(sail_int)(&zgsz3517);
          KILL(sail_int)(&zgsz3516);
          KILL(sail_int)(&zgsz3515);
        }
        {
          lbits zgsz3512;
          CREATE(lbits)(&zgsz3512);
          CONVERT_OF(lbits, fbits)(&zgsz3512, zgaz3266, UINT64_C(64) , true);
          sail_int zgsz3513;
          CREATE(sail_int)(&zgsz3513);
          CONVERT_OF(sail_int, mach_int)(&zgsz3513, zgaz3267);
          lbits zgsz3514;
          CREATE(lbits)(&zgsz3514);
          sail_truncate(&zgsz3514, zgsz3512, zgsz3513);
          zB_ie = CONVERT_OF(fbits, lbits)(zgsz3514, true);
          KILL(lbits)(&zgsz3514);
          KILL(sail_int)(&zgsz3513);
          KILL(lbits)(&zgsz3512);
        }
      }
      uint64_t zT_ie;
      {
        lbits zgaz3263;
        CREATE(lbits)(&zgaz3263);
        {
          int64_t zgaz3262;
          {
            sail_int zgsz3519;
            CREATE(sail_int)(&zgsz3519);
            CONVERT_OF(sail_int, mach_int)(&zgsz3519, ze);
            sail_int zgsz3520;
            CREATE(sail_int)(&zgsz3520);
            CONVERT_OF(sail_int, mach_int)(&zgsz3520, INT64_C(3));
            sail_int zgsz3521;
            CREATE(sail_int)(&zgsz3521);
            add_int(&zgsz3521, zgsz3519, zgsz3520);
            zgaz3262 = CONVERT_OF(mach_int, sail_int)(zgsz3521);
            KILL(sail_int)(&zgsz3521);
            KILL(sail_int)(&zgsz3520);
            KILL(sail_int)(&zgsz3519);
          }
          {
            sail_int zgsz3518;
            CREATE(sail_int)(&zgsz3518);
            CONVERT_OF(sail_int, mach_int)(&zgsz3518, zgaz3262);
            shiftr(&zgaz3263, ztop, zgsz3518);
            KILL(sail_int)(&zgsz3518);
          }
        }
        int64_t zgaz3264;
        {
          sail_int zgsz3524;
          CREATE(sail_int)(&zgsz3524);
          CONVERT_OF(sail_int, mach_int)(&zgsz3524, zcap_mantissa_width);
          sail_int zgsz3525;
          CREATE(sail_int)(&zgsz3525);
          CONVERT_OF(sail_int, mach_int)(&zgsz3525, INT64_C(3));
          sail_int zgsz3526;
          CREATE(sail_int)(&zgsz3526);
          sub_int(&zgsz3526, zgsz3524, zgsz3525);
          zgaz3264 = CONVERT_OF(mach_int, sail_int)(zgsz3526);
          KILL(sail_int)(&zgsz3526);
          KILL(sail_int)(&zgsz3525);
          KILL(sail_int)(&zgsz3524);
        }
        {
          sail_int zgsz3522;
          CREATE(sail_int)(&zgsz3522);
          CONVERT_OF(sail_int, mach_int)(&zgsz3522, zgaz3264);
          lbits zgsz3523;
          CREATE(lbits)(&zgsz3523);
          sail_truncate(&zgsz3523, zgaz3263, zgsz3522);
          zT_ie = CONVERT_OF(fbits, lbits)(zgsz3523, true);
          KILL(lbits)(&zgsz3523);
          KILL(sail_int)(&zgsz3522);
        }
        KILL(lbits)(&zgaz3263);
      }
      lbits zmaskLo;
      CREATE(lbits)(&zmaskLo);
      {
        int64_t zgaz3260;
        {
          sail_int zgsz3534;
          CREATE(sail_int)(&zgsz3534);
          CONVERT_OF(sail_int, mach_int)(&zgsz3534, INT64_C(64));
          sail_int zgsz3535;
          CREATE(sail_int)(&zgsz3535);
          CONVERT_OF(sail_int, mach_int)(&zgsz3535, INT64_C(1));
          sail_int zgsz3536;
          CREATE(sail_int)(&zgsz3536);
          add_int(&zgsz3536, zgsz3534, zgsz3535);
          zgaz3260 = CONVERT_OF(mach_int, sail_int)(zgsz3536);
          KILL(sail_int)(&zgsz3536);
          KILL(sail_int)(&zgsz3535);
          KILL(sail_int)(&zgsz3534);
        }
        sbits zgaz3261;
        {
          int64_t zgaz3259;
          {
            sail_int zgsz3529;
            CREATE(sail_int)(&zgsz3529);
            CONVERT_OF(sail_int, mach_int)(&zgsz3529, ze);
            sail_int zgsz3530;
            CREATE(sail_int)(&zgsz3530);
            CONVERT_OF(sail_int, mach_int)(&zgsz3530, INT64_C(3));
            sail_int zgsz3531;
            CREATE(sail_int)(&zgsz3531);
            add_int(&zgsz3531, zgsz3529, zgsz3530);
            zgaz3259 = CONVERT_OF(mach_int, sail_int)(zgsz3531);
            KILL(sail_int)(&zgsz3531);
            KILL(sail_int)(&zgsz3530);
            KILL(sail_int)(&zgsz3529);
          }
          {
            sail_int zgsz3527;
            CREATE(sail_int)(&zgsz3527);
            CONVERT_OF(sail_int, mach_int)(&zgsz3527, zgaz3259);
            lbits zgsz3528;
            CREATE(lbits)(&zgsz3528);
            sailgen_ones(&zgsz3528, zgsz3527);
            zgaz3261 = CONVERT_OF(sbits, lbits)(zgsz3528, true);
            KILL(lbits)(&zgsz3528);
            KILL(sail_int)(&zgsz3527);
          }
        }
        {
          sail_int zgsz3532;
          CREATE(sail_int)(&zgsz3532);
          CONVERT_OF(sail_int, mach_int)(&zgsz3532, zgaz3260);
          lbits zgsz3533;
          CREATE(lbits)(&zgsz3533);
          CONVERT_OF(lbits, sbits)(&zgsz3533, zgaz3261, true);
          sailgen_EXTZ(&zmaskLo, zgsz3532, zgsz3533);
          KILL(lbits)(&zgsz3533);
          KILL(sail_int)(&zgsz3532);
        }
      }
      {
        sail_int zgaz3244;
        CREATE(sail_int)(&zgaz3244);
        {
          lbits zgaz3243;
          CREATE(lbits)(&zgaz3243);
          and_bits(&zgaz3243, zext_base, zmaskLo);
          sail_unsigned(&zgaz3244, zgaz3243);
          KILL(lbits)(&zgaz3243);
        }
        {
          sail_int zgsz3537;
          CREATE(sail_int)(&zgsz3537);
          CONVERT_OF(sail_int, mach_int)(&zgsz3537, INT64_C(0));
          zlostSignificantBase = sailgen_neq_int(zgaz3244, zgsz3537);
          KILL(sail_int)(&zgsz3537);
        }
        unit zgsz358;
        zgsz358 = UNIT;
        KILL(sail_int)(&zgaz3244);
      }
      {
        sail_int zgaz3246;
        CREATE(sail_int)(&zgaz3246);
        {
          lbits zgaz3245;
          CREATE(lbits)(&zgaz3245);
          and_bits(&zgaz3245, ztop, zmaskLo);
          sail_unsigned(&zgaz3246, zgaz3245);
          KILL(lbits)(&zgaz3245);
        }
        {
          sail_int zgsz3538;
          CREATE(sail_int)(&zgsz3538);
          CONVERT_OF(sail_int, mach_int)(&zgsz3538, INT64_C(0));
          zlostSignificantTop = sailgen_neq_int(zgaz3246, zgsz3538);
          KILL(sail_int)(&zgsz3538);
        }
        unit zgsz357;
        zgsz357 = UNIT;
        KILL(sail_int)(&zgaz3246);
      }
      {
        unit zgsz356;
        if (zlostSignificantTop) {
          {
            lbits zgsz3539;
            CREATE(lbits)(&zgsz3539);
            CONVERT_OF(lbits, fbits)(&zgsz3539, zT_ie, UINT64_C(11) , true);
            sail_int zgsz3540;
            CREATE(sail_int)(&zgsz3540);
            CONVERT_OF(sail_int, mach_int)(&zgsz3540, INT64_C(1));
            lbits zgsz3541;
            CREATE(lbits)(&zgsz3541);
            add_bits_int(&zgsz3541, zgsz3539, zgsz3540);
            zT_ie = CONVERT_OF(fbits, lbits)(zgsz3541, true);
            KILL(lbits)(&zgsz3541);
            KILL(sail_int)(&zgsz3540);
            KILL(lbits)(&zgsz3539);
          }
          zgsz356 = UNIT;
        } else {  zgsz356 = UNIT;  }
      }
      uint64_t zlen_ie;
      {
        lbits zgsz3581;
        CREATE(lbits)(&zgsz3581);
        CONVERT_OF(lbits, fbits)(&zgsz3581, zT_ie, UINT64_C(11) , true);
        lbits zgsz3582;
        CREATE(lbits)(&zgsz3582);
        CONVERT_OF(lbits, fbits)(&zgsz3582, zB_ie, UINT64_C(11) , true);
        lbits zgsz3583;
        CREATE(lbits)(&zgsz3583);
        sub_bits(&zgsz3583, zgsz3581, zgsz3582);
        zlen_ie = CONVERT_OF(fbits, lbits)(zgsz3583, true);
        KILL(lbits)(&zgsz3583);
        KILL(lbits)(&zgsz3582);
        KILL(lbits)(&zgsz3581);
      }
      {
        bool zgaz3249;
        {
          fbits zgaz3248;
          {
            int64_t zgaz3247;
            {
              sail_int zgsz3544;
              CREATE(sail_int)(&zgsz3544);
              CONVERT_OF(sail_int, mach_int)(&zgsz3544, zcap_mantissa_width);
              sail_int zgsz3545;
              CREATE(sail_int)(&zgsz3545);
              CONVERT_OF(sail_int, mach_int)(&zgsz3545, INT64_C(4));
              sail_int zgsz3546;
              CREATE(sail_int)(&zgsz3546);
              sub_int(&zgsz3546, zgsz3544, zgsz3545);
              zgaz3247 = CONVERT_OF(mach_int, sail_int)(zgsz3546);
              KILL(sail_int)(&zgsz3546);
              KILL(sail_int)(&zgsz3545);
              KILL(sail_int)(&zgsz3544);
            }
            {
              lbits zgsz3542;
              CREATE(lbits)(&zgsz3542);
              CONVERT_OF(lbits, fbits)(&zgsz3542, zlen_ie, UINT64_C(11) , true);
              sail_int zgsz3543;
              CREATE(sail_int)(&zgsz3543);
              CONVERT_OF(sail_int, mach_int)(&zgsz3543, zgaz3247);
              zgaz3248 = bitvector_access(zgsz3542, zgsz3543);
              KILL(sail_int)(&zgsz3543);
              KILL(lbits)(&zgsz3542);
            }
          }
          zgaz3249 = eq_bit(zgaz3248, UINT64_C(1));
        }
        unit zgsz360;
        if (zgaz3249) {
          {
            zincE = true;
            unit zgsz366;
            zgsz366 = UNIT;
          }
          {
            bool zgsz361;
            if (zlostSignificantBase) {  zgsz361 = true;  } else {
              fbits zgaz3250;
              {
                lbits zgsz3547;
                CREATE(lbits)(&zgsz3547);
                CONVERT_OF(lbits, fbits)(&zgsz3547, zB_ie, UINT64_C(11) , true);
                sail_int zgsz3548;
                CREATE(sail_int)(&zgsz3548);
                CONVERT_OF(sail_int, mach_int)(&zgsz3548, INT64_C(0));
                zgaz3250 = bitvector_access(zgsz3547, zgsz3548);
                KILL(sail_int)(&zgsz3548);
                KILL(lbits)(&zgsz3547);
              }
              zgsz361 = eq_bit(zgaz3250, UINT64_C(1));
            }
            zlostSignificantBase = zgsz361;
            unit zgsz365;
            zgsz365 = UNIT;
          }
          {
            bool zgsz362;
            if (zlostSignificantTop) {  zgsz362 = true;  } else {
              fbits zgaz3251;
              {
                lbits zgsz3549;
                CREATE(lbits)(&zgsz3549);
                CONVERT_OF(lbits, fbits)(&zgsz3549, zT_ie, UINT64_C(11) , true);
                sail_int zgsz3550;
                CREATE(sail_int)(&zgsz3550);
                CONVERT_OF(sail_int, mach_int)(&zgsz3550, INT64_C(0));
                zgaz3251 = bitvector_access(zgsz3549, zgsz3550);
                KILL(sail_int)(&zgsz3550);
                KILL(lbits)(&zgsz3549);
              }
              zgsz362 = eq_bit(zgaz3251, UINT64_C(1));
            }
            zlostSignificantTop = zgsz362;
            unit zgsz364;
            zgsz364 = UNIT;
          }
          {
            uint64_t zgaz3253;
            {
              int64_t zgaz3252;
              {
                sail_int zgsz3554;
                CREATE(sail_int)(&zgsz3554);
                CONVERT_OF(sail_int, mach_int)(&zgsz3554, ze);
                sail_int zgsz3555;
                CREATE(sail_int)(&zgsz3555);
                CONVERT_OF(sail_int, mach_int)(&zgsz3555, INT64_C(4));
                sail_int zgsz3556;
                CREATE(sail_int)(&zgsz3556);
                add_int(&zgsz3556, zgsz3554, zgsz3555);
                zgaz3252 = CONVERT_OF(mach_int, sail_int)(zgsz3556);
                KILL(sail_int)(&zgsz3556);
                KILL(sail_int)(&zgsz3555);
                KILL(sail_int)(&zgsz3554);
              }
              {
                lbits zgsz3551;
                CREATE(lbits)(&zgsz3551);
                CONVERT_OF(lbits, fbits)(&zgsz3551, zbase, UINT64_C(64) , true);
                sail_int zgsz3552;
                CREATE(sail_int)(&zgsz3552);
                CONVERT_OF(sail_int, mach_int)(&zgsz3552, zgaz3252);
                lbits zgsz3553;
                CREATE(lbits)(&zgsz3553);
                shiftr(&zgsz3553, zgsz3551, zgsz3552);
                zgaz3253 = CONVERT_OF(fbits, lbits)(zgsz3553, true);
                KILL(lbits)(&zgsz3553);
                KILL(sail_int)(&zgsz3552);
                KILL(lbits)(&zgsz3551);
              }
            }
            int64_t zgaz3254;
            {
              sail_int zgsz3560;
              CREATE(sail_int)(&zgsz3560);
              CONVERT_OF(sail_int, mach_int)(&zgsz3560, zcap_mantissa_width);
              sail_int zgsz3561;
              CREATE(sail_int)(&zgsz3561);
              CONVERT_OF(sail_int, mach_int)(&zgsz3561, INT64_C(3));
              sail_int zgsz3562;
              CREATE(sail_int)(&zgsz3562);
              sub_int(&zgsz3562, zgsz3560, zgsz3561);
              zgaz3254 = CONVERT_OF(mach_int, sail_int)(zgsz3562);
              KILL(sail_int)(&zgsz3562);
              KILL(sail_int)(&zgsz3561);
              KILL(sail_int)(&zgsz3560);
            }
            {
              lbits zgsz3557;
              CREATE(lbits)(&zgsz3557);
              CONVERT_OF(lbits, fbits)(&zgsz3557, zgaz3253, UINT64_C(64) , true);
              sail_int zgsz3558;
              CREATE(sail_int)(&zgsz3558);
              CONVERT_OF(sail_int, mach_int)(&zgsz3558, zgaz3254);
              lbits zgsz3559;
              CREATE(lbits)(&zgsz3559);
              sail_truncate(&zgsz3559, zgsz3557, zgsz3558);
              zB_ie = CONVERT_OF(fbits, lbits)(zgsz3559, true);
              KILL(lbits)(&zgsz3559);
              KILL(sail_int)(&zgsz3558);
              KILL(lbits)(&zgsz3557);
            }
            unit zgsz363;
            zgsz363 = UNIT;
          }
          int64_t zincT;
          if (zlostSignificantTop) {  zincT = INT64_C(1);  } else {  zincT = INT64_C(0);  }
          uint64_t zgaz3258;
          {
            lbits zgaz3256;
            CREATE(lbits)(&zgaz3256);
            {
              int64_t zgaz3255;
              {
                sail_int zgsz3564;
                CREATE(sail_int)(&zgsz3564);
                CONVERT_OF(sail_int, mach_int)(&zgsz3564, ze);
                sail_int zgsz3565;
                CREATE(sail_int)(&zgsz3565);
                CONVERT_OF(sail_int, mach_int)(&zgsz3565, INT64_C(4));
                sail_int zgsz3566;
                CREATE(sail_int)(&zgsz3566);
                add_int(&zgsz3566, zgsz3564, zgsz3565);
                zgaz3255 = CONVERT_OF(mach_int, sail_int)(zgsz3566);
                KILL(sail_int)(&zgsz3566);
                KILL(sail_int)(&zgsz3565);
                KILL(sail_int)(&zgsz3564);
              }
              {
                sail_int zgsz3563;
                CREATE(sail_int)(&zgsz3563);
                CONVERT_OF(sail_int, mach_int)(&zgsz3563, zgaz3255);
                shiftr(&zgaz3256, ztop, zgsz3563);
                KILL(sail_int)(&zgsz3563);
              }
            }
            int64_t zgaz3257;
            {
              sail_int zgsz3569;
              CREATE(sail_int)(&zgsz3569);
              CONVERT_OF(sail_int, mach_int)(&zgsz3569, zcap_mantissa_width);
              sail_int zgsz3570;
              CREATE(sail_int)(&zgsz3570);
              CONVERT_OF(sail_int, mach_int)(&zgsz3570, INT64_C(3));
              sail_int zgsz3571;
              CREATE(sail_int)(&zgsz3571);
              sub_int(&zgsz3571, zgsz3569, zgsz3570);
              zgaz3257 = CONVERT_OF(mach_int, sail_int)(zgsz3571);
              KILL(sail_int)(&zgsz3571);
              KILL(sail_int)(&zgsz3570);
              KILL(sail_int)(&zgsz3569);
            }
            {
              sail_int zgsz3567;
              CREATE(sail_int)(&zgsz3567);
              CONVERT_OF(sail_int, mach_int)(&zgsz3567, zgaz3257);
              lbits zgsz3568;
              CREATE(lbits)(&zgsz3568);
              sail_truncate(&zgsz3568, zgaz3256, zgsz3567);
              zgaz3258 = CONVERT_OF(fbits, lbits)(zgsz3568, true);
              KILL(lbits)(&zgsz3568);
              KILL(sail_int)(&zgsz3567);
            }
            KILL(lbits)(&zgaz3256);
          }
          {
            lbits zgsz3572;
            CREATE(lbits)(&zgsz3572);
            CONVERT_OF(lbits, fbits)(&zgsz3572, zgaz3258, UINT64_C(11) , true);
            sail_int zgsz3573;
            CREATE(sail_int)(&zgsz3573);
            CONVERT_OF(sail_int, mach_int)(&zgsz3573, zincT);
            lbits zgsz3574;
            CREATE(lbits)(&zgsz3574);
            add_bits_int(&zgsz3574, zgsz3572, zgsz3573);
            zT_ie = CONVERT_OF(fbits, lbits)(zgsz3574, true);
            KILL(lbits)(&zgsz3574);
            KILL(sail_int)(&zgsz3573);
            KILL(lbits)(&zgsz3572);
          }
          zgsz360 = UNIT;
        } else {  zgsz360 = UNIT;  }
      }
      {
        {
          lbits zgsz3575;
          CREATE(lbits)(&zgsz3575);
          CONVERT_OF(lbits, fbits)(&zgsz3575, zB_ie, UINT64_C(11) , true);
          lbits zgsz3576;
          CREATE(lbits)(&zgsz3576);
          CONVERT_OF(lbits, fbits)(&zgsz3576, UINT64_C(0b000), UINT64_C(3) , true);
          lbits zgsz3577;
          CREATE(lbits)(&zgsz3577);
          append(&zgsz3577, zgsz3575, zgsz3576);
          zBbits = CONVERT_OF(fbits, lbits)(zgsz3577, true);
          KILL(lbits)(&zgsz3577);
          KILL(lbits)(&zgsz3576);
          KILL(lbits)(&zgsz3575);
        }
        unit zgsz359;
        zgsz359 = UNIT;
      }
      {
        lbits zgsz3578;
        CREATE(lbits)(&zgsz3578);
        CONVERT_OF(lbits, fbits)(&zgsz3578, zT_ie, UINT64_C(11) , true);
        lbits zgsz3579;
        CREATE(lbits)(&zgsz3579);
        CONVERT_OF(lbits, fbits)(&zgsz3579, UINT64_C(0b000), UINT64_C(3) , true);
        lbits zgsz3580;
        CREATE(lbits)(&zgsz3580);
        append(&zgsz3580, zgsz3578, zgsz3579);
        zTbits = CONVERT_OF(fbits, lbits)(zgsz3580, true);
        KILL(lbits)(&zgsz3580);
        KILL(lbits)(&zgsz3579);
        KILL(lbits)(&zgsz3578);
      }
      zgsz355 = UNIT;
      KILL(lbits)(&zmaskLo);
    } else {  zgsz355 = UNIT;  }
  }
  struct zCapability znewCap;
  {
    uint64_t zgaz3270;
    {
      int64_t zgaz3269;
      if (zincE) {
        {
          sail_int zgsz3584;
          CREATE(sail_int)(&zgsz3584);
          CONVERT_OF(sail_int, mach_int)(&zgsz3584, ze);
          sail_int zgsz3585;
          CREATE(sail_int)(&zgsz3585);
          CONVERT_OF(sail_int, mach_int)(&zgsz3585, INT64_C(1));
          sail_int zgsz3586;
          CREATE(sail_int)(&zgsz3586);
          add_int(&zgsz3586, zgsz3584, zgsz3585);
          zgaz3269 = CONVERT_OF(mach_int, sail_int)(zgsz3586);
          KILL(sail_int)(&zgsz3586);
          KILL(sail_int)(&zgsz3585);
          KILL(sail_int)(&zgsz3584);
        }
      } else {  zgaz3269 = ze;  }
      {
        sail_int zgsz3587;
        CREATE(sail_int)(&zgsz3587);
        CONVERT_OF(sail_int, mach_int)(&zgsz3587, INT64_C(6));
        sail_int zgsz3588;
        CREATE(sail_int)(&zgsz3588);
        CONVERT_OF(sail_int, mach_int)(&zgsz3588, zgaz3269);
        lbits zgsz3589;
        CREATE(lbits)(&zgsz3589);
        sailgen_to_bits(&zgsz3589, zgsz3587, zgsz3588);
        zgaz3270 = CONVERT_OF(fbits, lbits)(zgsz3589, true);
        KILL(lbits)(&zgsz3589);
        KILL(sail_int)(&zgsz3588);
        KILL(sail_int)(&zgsz3587);
      }
    }
    struct zCapability zgsz367;
    zgsz367 = zcap;
    zgsz367.zB = zBbits;
    zgsz367.zE = zgaz3270;
    zgsz367.zT = zTbits;
    zgsz367.zaddress = zbase;
    zgsz367.zinternal_E = zie;
    znewCap = zgsz367;
  }
  bool zexact;
  {
    bool zgaz3268;
    {
      bool zgsz368;
      if (zlostSignificantBase) {  zgsz368 = true;  } else {  zgsz368 = zlostSignificantTop;  }
      zgaz3268 = zgsz368;
    }
    zexact = not(zgaz3268);
  }
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgsz369;
  zgsz369.ztup0 = zexact;
  zgsz369.ztup1 = znewCap;
  zcbz326 = zgsz369;










  KILL(lbits)(&zlength);
  KILL(lbits)(&zext_base);
end_function_116: ;
  return zcbz326;
end_block_exception_117: ;
  struct zCapability zcbz348 = { .zuperms = UINT64_C(0xdeadc0de), .ztag = false, .zreserved = UINT64_C(0xdeadc0de), .zpermit_unseal = false, .zpermit_store_local_cap = false, .zpermit_store_cap = false, .zpermit_store = false, .zpermit_set_CID = false, .zpermit_seal = false, .zpermit_load_cap = false, .zpermit_load = false, .zpermit_execute = false, .zpermit_cinvoke = false, .zotype = UINT64_C(0xdeadc0de), .zinternal_E = false, .zglobal = false, .zflag_cap_mode = false, .zaddress = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zT = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz347 = { .ztup1 = zcbz348, .ztup0 = false };
  return zcbz347;
}

static uint64_t sailgen_getCapPerms(struct zCapability);

static uint64_t sailgen_getCapPerms(struct zCapability zcap)
{
  __label__ end_function_119, end_block_exception_120;

  uint64_t zcbz327;
  uint64_t zperms;
  {
    uint64_t zgaz3278;
    zgaz3278 = sailgen_getCapHardPerms(zcap);
    {
      sail_int zgsz3597;
      CREATE(sail_int)(&zgsz3597);
      CONVERT_OF(sail_int, mach_int)(&zgsz3597, INT64_C(15));
      lbits zgsz3598;
      CREATE(lbits)(&zgsz3598);
      CONVERT_OF(lbits, fbits)(&zgsz3598, zgaz3278, UINT64_C(12) , true);
      lbits zgsz3599;
      CREATE(lbits)(&zgsz3599);
      sailgen_EXTZ(&zgsz3599, zgsz3597, zgsz3598);
      zperms = CONVERT_OF(fbits, lbits)(zgsz3599, true);
      KILL(lbits)(&zgsz3599);
      KILL(lbits)(&zgsz3598);
      KILL(sail_int)(&zgsz3597);
    }
  }
  uint64_t zgaz3277;
  zgaz3277 = zcap.zuperms;
  {
    lbits zgsz3600;
    CREATE(lbits)(&zgsz3600);
    CONVERT_OF(lbits, fbits)(&zgsz3600, zgaz3277, UINT64_C(4) , true);
    lbits zgsz3601;
    CREATE(lbits)(&zgsz3601);
    CONVERT_OF(lbits, fbits)(&zgsz3601, zperms, UINT64_C(15) , true);
    lbits zgsz3602;
    CREATE(lbits)(&zgsz3602);
    append(&zgsz3602, zgsz3600, zgsz3601);
    zcbz327 = CONVERT_OF(fbits, lbits)(zgsz3602, true);
    KILL(lbits)(&zgsz3602);
    KILL(lbits)(&zgsz3601);
    KILL(lbits)(&zgsz3600);
  }


end_function_119: ;
  return zcbz327;
end_block_exception_120: ;

  return UINT64_C(0xdeadc0de);
}

static struct zCapability sailgen_setCapPerms(struct zCapability, uint64_t);

static struct zCapability sailgen_setCapPerms(struct zCapability zcap, uint64_t zperms)
{
  __label__ end_function_122, end_block_exception_123;

  struct zCapability zcbz328;
  uint64_t zgaz3280;
  {
    uint64_t zgaz3279;
    {
      lbits zgsz3606;
      CREATE(lbits)(&zgsz3606);
      CONVERT_OF(lbits, fbits)(&zgsz3606, zperms, UINT64_C(19) , true);
      sail_int zgsz3607;
      CREATE(sail_int)(&zgsz3607);
      CONVERT_OF(sail_int, mach_int)(&zgsz3607, zcap_uperms_shift);
      lbits zgsz3608;
      CREATE(lbits)(&zgsz3608);
      shiftr(&zgsz3608, zgsz3606, zgsz3607);
      zgaz3279 = CONVERT_OF(fbits, lbits)(zgsz3608, true);
      KILL(lbits)(&zgsz3608);
      KILL(sail_int)(&zgsz3607);
      KILL(lbits)(&zgsz3606);
    }
    {
      lbits zgsz3603;
      CREATE(lbits)(&zgsz3603);
      CONVERT_OF(lbits, fbits)(&zgsz3603, zgaz3279, UINT64_C(19) , true);
      sail_int zgsz3604;
      CREATE(sail_int)(&zgsz3604);
      CONVERT_OF(sail_int, mach_int)(&zgsz3604, zcap_uperms_width);
      lbits zgsz3605;
      CREATE(lbits)(&zgsz3605);
      sail_truncate(&zgsz3605, zgsz3603, zgsz3604);
      zgaz3280 = CONVERT_OF(fbits, lbits)(zgsz3605, true);
      KILL(lbits)(&zgsz3605);
      KILL(sail_int)(&zgsz3604);
      KILL(lbits)(&zgsz3603);
    }
  }
  bool zgaz3282;
  {
    fbits zgaz3281;
    {
      lbits zgsz3609;
      CREATE(lbits)(&zgsz3609);
      CONVERT_OF(lbits, fbits)(&zgsz3609, zperms, UINT64_C(19) , true);
      sail_int zgsz3610;
      CREATE(sail_int)(&zgsz3610);
      CONVERT_OF(sail_int, mach_int)(&zgsz3610, INT64_C(11));
      zgaz3281 = bitvector_access(zgsz3609, zgsz3610);
      KILL(sail_int)(&zgsz3610);
      KILL(lbits)(&zgsz3609);
    }
    zgaz3282 = sailgen_bit_to_bool(zgaz3281);
  }
  bool zgaz3284;
  {
    fbits zgaz3283;
    {
      lbits zgsz3611;
      CREATE(lbits)(&zgsz3611);
      CONVERT_OF(lbits, fbits)(&zgsz3611, zperms, UINT64_C(19) , true);
      sail_int zgsz3612;
      CREATE(sail_int)(&zgsz3612);
      CONVERT_OF(sail_int, mach_int)(&zgsz3612, INT64_C(10));
      zgaz3283 = bitvector_access(zgsz3611, zgsz3612);
      KILL(sail_int)(&zgsz3612);
      KILL(lbits)(&zgsz3611);
    }
    zgaz3284 = sailgen_bit_to_bool(zgaz3283);
  }
  bool zgaz3286;
  {
    fbits zgaz3285;
    {
      lbits zgsz3613;
      CREATE(lbits)(&zgsz3613);
      CONVERT_OF(lbits, fbits)(&zgsz3613, zperms, UINT64_C(19) , true);
      sail_int zgsz3614;
      CREATE(sail_int)(&zgsz3614);
      CONVERT_OF(sail_int, mach_int)(&zgsz3614, INT64_C(9));
      zgaz3285 = bitvector_access(zgsz3613, zgsz3614);
      KILL(sail_int)(&zgsz3614);
      KILL(lbits)(&zgsz3613);
    }
    zgaz3286 = sailgen_bit_to_bool(zgaz3285);
  }
  bool zgaz3288;
  {
    fbits zgaz3287;
    {
      lbits zgsz3615;
      CREATE(lbits)(&zgsz3615);
      CONVERT_OF(lbits, fbits)(&zgsz3615, zperms, UINT64_C(19) , true);
      sail_int zgsz3616;
      CREATE(sail_int)(&zgsz3616);
      CONVERT_OF(sail_int, mach_int)(&zgsz3616, INT64_C(8));
      zgaz3287 = bitvector_access(zgsz3615, zgsz3616);
      KILL(sail_int)(&zgsz3616);
      KILL(lbits)(&zgsz3615);
    }
    zgaz3288 = sailgen_bit_to_bool(zgaz3287);
  }
  bool zgaz3290;
  {
    fbits zgaz3289;
    {
      lbits zgsz3617;
      CREATE(lbits)(&zgsz3617);
      CONVERT_OF(lbits, fbits)(&zgsz3617, zperms, UINT64_C(19) , true);
      sail_int zgsz3618;
      CREATE(sail_int)(&zgsz3618);
      CONVERT_OF(sail_int, mach_int)(&zgsz3618, INT64_C(7));
      zgaz3289 = bitvector_access(zgsz3617, zgsz3618);
      KILL(sail_int)(&zgsz3618);
      KILL(lbits)(&zgsz3617);
    }
    zgaz3290 = sailgen_bit_to_bool(zgaz3289);
  }
  bool zgaz3292;
  {
    fbits zgaz3291;
    {
      lbits zgsz3619;
      CREATE(lbits)(&zgsz3619);
      CONVERT_OF(lbits, fbits)(&zgsz3619, zperms, UINT64_C(19) , true);
      sail_int zgsz3620;
      CREATE(sail_int)(&zgsz3620);
      CONVERT_OF(sail_int, mach_int)(&zgsz3620, INT64_C(6));
      zgaz3291 = bitvector_access(zgsz3619, zgsz3620);
      KILL(sail_int)(&zgsz3620);
      KILL(lbits)(&zgsz3619);
    }
    zgaz3292 = sailgen_bit_to_bool(zgaz3291);
  }
  bool zgaz3294;
  {
    fbits zgaz3293;
    {
      lbits zgsz3621;
      CREATE(lbits)(&zgsz3621);
      CONVERT_OF(lbits, fbits)(&zgsz3621, zperms, UINT64_C(19) , true);
      sail_int zgsz3622;
      CREATE(sail_int)(&zgsz3622);
      CONVERT_OF(sail_int, mach_int)(&zgsz3622, INT64_C(5));
      zgaz3293 = bitvector_access(zgsz3621, zgsz3622);
      KILL(sail_int)(&zgsz3622);
      KILL(lbits)(&zgsz3621);
    }
    zgaz3294 = sailgen_bit_to_bool(zgaz3293);
  }
  bool zgaz3296;
  {
    fbits zgaz3295;
    {
      lbits zgsz3623;
      CREATE(lbits)(&zgsz3623);
      CONVERT_OF(lbits, fbits)(&zgsz3623, zperms, UINT64_C(19) , true);
      sail_int zgsz3624;
      CREATE(sail_int)(&zgsz3624);
      CONVERT_OF(sail_int, mach_int)(&zgsz3624, INT64_C(4));
      zgaz3295 = bitvector_access(zgsz3623, zgsz3624);
      KILL(sail_int)(&zgsz3624);
      KILL(lbits)(&zgsz3623);
    }
    zgaz3296 = sailgen_bit_to_bool(zgaz3295);
  }
  bool zgaz3298;
  {
    fbits zgaz3297;
    {
      lbits zgsz3625;
      CREATE(lbits)(&zgsz3625);
      CONVERT_OF(lbits, fbits)(&zgsz3625, zperms, UINT64_C(19) , true);
      sail_int zgsz3626;
      CREATE(sail_int)(&zgsz3626);
      CONVERT_OF(sail_int, mach_int)(&zgsz3626, INT64_C(3));
      zgaz3297 = bitvector_access(zgsz3625, zgsz3626);
      KILL(sail_int)(&zgsz3626);
      KILL(lbits)(&zgsz3625);
    }
    zgaz3298 = sailgen_bit_to_bool(zgaz3297);
  }
  bool zgaz3300;
  {
    fbits zgaz3299;
    {
      lbits zgsz3627;
      CREATE(lbits)(&zgsz3627);
      CONVERT_OF(lbits, fbits)(&zgsz3627, zperms, UINT64_C(19) , true);
      sail_int zgsz3628;
      CREATE(sail_int)(&zgsz3628);
      CONVERT_OF(sail_int, mach_int)(&zgsz3628, INT64_C(2));
      zgaz3299 = bitvector_access(zgsz3627, zgsz3628);
      KILL(sail_int)(&zgsz3628);
      KILL(lbits)(&zgsz3627);
    }
    zgaz3300 = sailgen_bit_to_bool(zgaz3299);
  }
  bool zgaz3302;
  {
    fbits zgaz3301;
    {
      lbits zgsz3629;
      CREATE(lbits)(&zgsz3629);
      CONVERT_OF(lbits, fbits)(&zgsz3629, zperms, UINT64_C(19) , true);
      sail_int zgsz3630;
      CREATE(sail_int)(&zgsz3630);
      CONVERT_OF(sail_int, mach_int)(&zgsz3630, INT64_C(1));
      zgaz3301 = bitvector_access(zgsz3629, zgsz3630);
      KILL(sail_int)(&zgsz3630);
      KILL(lbits)(&zgsz3629);
    }
    zgaz3302 = sailgen_bit_to_bool(zgaz3301);
  }
  bool zgaz3304;
  {
    fbits zgaz3303;
    {
      lbits zgsz3631;
      CREATE(lbits)(&zgsz3631);
      CONVERT_OF(lbits, fbits)(&zgsz3631, zperms, UINT64_C(19) , true);
      sail_int zgsz3632;
      CREATE(sail_int)(&zgsz3632);
      CONVERT_OF(sail_int, mach_int)(&zgsz3632, INT64_C(0));
      zgaz3303 = bitvector_access(zgsz3631, zgsz3632);
      KILL(sail_int)(&zgsz3632);
      KILL(lbits)(&zgsz3631);
    }
    zgaz3304 = sailgen_bit_to_bool(zgaz3303);
  }
  struct zCapability zgsz370;
  zgsz370 = zcap;
  zgsz370.zaccess_system_regs = zgaz3284;
  zgsz370.zglobal = zgaz3304;
  zgsz370.zpermit_cinvoke = zgaz3288;
  zgsz370.zpermit_execute = zgaz3302;
  zgsz370.zpermit_load = zgaz3300;
  zgsz370.zpermit_load_cap = zgaz3296;
  zgsz370.zpermit_seal = zgaz3290;
  zgsz370.zpermit_set_CID = zgaz3282;
  zgsz370.zpermit_store = zgaz3298;
  zgsz370.zpermit_store_cap = zgaz3294;
  zgsz370.zpermit_store_local_cap = zgaz3292;
  zgsz370.zpermit_unseal = zgaz3286;
  zgsz370.zuperms = zgaz3280;
  zcbz328 = zgsz370;














end_function_122: ;
  return zcbz328;
end_block_exception_123: ;
  struct zCapability zcbz349 = { .zuperms = UINT64_C(0xdeadc0de), .ztag = false, .zreserved = UINT64_C(0xdeadc0de), .zpermit_unseal = false, .zpermit_store_local_cap = false, .zpermit_store_cap = false, .zpermit_store = false, .zpermit_set_CID = false, .zpermit_seal = false, .zpermit_load_cap = false, .zpermit_load = false, .zpermit_execute = false, .zpermit_cinvoke = false, .zotype = UINT64_C(0xdeadc0de), .zinternal_E = false, .zglobal = false, .zflag_cap_mode = false, .zaddress = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zT = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  return zcbz349;
}

static uint64_t sailgen_getCapFlags(struct zCapability);

static uint64_t sailgen_getCapFlags(struct zCapability zcap)
{
  __label__ end_function_125, end_block_exception_126;

  uint64_t zcbz329;
  bool zgaz3305;
  zgaz3305 = zcap.zflag_cap_mode;
  zcbz329 = sailgen_bool_to_bits(zgaz3305);

end_function_125: ;
  return zcbz329;
end_block_exception_126: ;

  return UINT64_C(0xdeadc0de);
}

static bool sailgen_isCapSealed(struct zCapability);

static bool sailgen_isCapSealed(struct zCapability zcap)
{
  __label__ end_function_128, end_block_exception_129;

  bool zcbz330;
  int64_t zgaz3307;
  {
    uint64_t zgaz3306;
    zgaz3306 = zcap.zotype;
    {
      lbits zgsz3633;
      CREATE(lbits)(&zgsz3633);
      CONVERT_OF(lbits, fbits)(&zgsz3633, zgaz3306, UINT64_C(18) , true);
      sail_int zgsz3634;
      CREATE(sail_int)(&zgsz3634);
      sail_signed(&zgsz3634, zgsz3633);
      zgaz3307 = CONVERT_OF(mach_int, sail_int)(zgsz3634);
      KILL(sail_int)(&zgsz3634);
      KILL(lbits)(&zgsz3633);
    }
  }
  {
    sail_int zgsz3635;
    CREATE(sail_int)(&zgsz3635);
    CONVERT_OF(sail_int, mach_int)(&zgsz3635, zgaz3307);
    sail_int zgsz3636;
    CREATE(sail_int)(&zgsz3636);
    CONVERT_OF(sail_int, mach_int)(&zgsz3636, zotype_unsealed);
    zcbz330 = sailgen_neq_int(zgsz3635, zgsz3636);
    KILL(sail_int)(&zgsz3636);
    KILL(sail_int)(&zgsz3635);
  }

end_function_128: ;
  return zcbz330;
end_block_exception_129: ;

  return false;
}

static bool sailgen_hasReservedOType(struct zCapability);

static bool sailgen_hasReservedOType(struct zCapability zcap)
{
  __label__ end_function_131, end_block_exception_132;

  bool zcbz331;
  int64_t zgaz3309;
  {
    uint64_t zgaz3308;
    zgaz3308 = zcap.zotype;
    {
      lbits zgsz3637;
      CREATE(lbits)(&zgsz3637);
      CONVERT_OF(lbits, fbits)(&zgsz3637, zgaz3308, UINT64_C(18) , true);
      sail_int zgsz3638;
      CREATE(sail_int)(&zgsz3638);
      sail_unsigned(&zgsz3638, zgsz3637);
      zgaz3309 = CONVERT_OF(mach_int, sail_int)(zgsz3638);
      KILL(sail_int)(&zgsz3638);
      KILL(lbits)(&zgsz3637);
    }
  }
  {
    sail_int zgsz3639;
    CREATE(sail_int)(&zgsz3639);
    CONVERT_OF(sail_int, mach_int)(&zgsz3639, zgaz3309);
    sail_int zgsz3640;
    CREATE(sail_int)(&zgsz3640);
    CONVERT_OF(sail_int, mach_int)(&zgsz3640, zcap_max_otype);
    zcbz331 = gt(zgsz3639, zgsz3640);
    KILL(sail_int)(&zgsz3640);
    KILL(sail_int)(&zgsz3639);
  }

end_function_131: ;
  return zcbz331;
end_block_exception_132: ;

  return false;
}

static uint64_t sailgen_getCapBaseBits(struct zCapability);

static uint64_t sailgen_getCapBaseBits(struct zCapability zc)
{
  __label__ case_135, finish_match_134, end_function_136, end_block_exception_137;

  uint64_t zcbz332;
  struct ztuple_z8z5bv64zCz0z5bvz9 zgaz3310;
  CREATE(ztuple_z8z5bv64zCz0z5bvz9)(&zgaz3310);
  sailgen_getCapBoundsBits(&zgaz3310, zc);
  uint64_t zgsz371;
  {
    uint64_t zbase;
    zbase = zgaz3310.ztup0;
    zgsz371 = zbase;
    goto finish_match_134;
  }
case_135: ;
  sail_match_failure("getCapBaseBits");
finish_match_134: ;
  zcbz332 = zgsz371;

  KILL(ztuple_z8z5bv64zCz0z5bvz9)(&zgaz3310);
end_function_136: ;
  return zcbz332;
end_block_exception_137: ;

  return UINT64_C(0xdeadc0de);
}

static uint64_t sailgen_getCapOffsetBits(struct zCapability);

static uint64_t sailgen_getCapOffsetBits(struct zCapability zc)
{
  __label__ end_function_139, end_block_exception_140;

  uint64_t zcbz333;
  uint64_t zbase;
  zbase = sailgen_getCapBaseBits(zc);
  uint64_t zgaz3311;
  zgaz3311 = zc.zaddress;
  {
    lbits zgsz3641;
    CREATE(lbits)(&zgsz3641);
    CONVERT_OF(lbits, fbits)(&zgsz3641, zgaz3311, UINT64_C(64) , true);
    lbits zgsz3642;
    CREATE(lbits)(&zgsz3642);
    CONVERT_OF(lbits, fbits)(&zgsz3642, zbase, UINT64_C(64) , true);
    lbits zgsz3643;
    CREATE(lbits)(&zgsz3643);
    sub_bits(&zgsz3643, zgsz3641, zgsz3642);
    zcbz333 = CONVERT_OF(fbits, lbits)(zgsz3643, true);
    KILL(lbits)(&zgsz3643);
    KILL(lbits)(&zgsz3642);
    KILL(lbits)(&zgsz3641);
  }


end_function_139: ;
  return zcbz333;
end_block_exception_140: ;

  return UINT64_C(0xdeadc0de);
}

static void sailgen_getCapLength(sail_int *rop, struct zCapability);

static void sailgen_getCapLength(sail_int *zcbz334, struct zCapability zc)
{
  __label__ case_143, finish_match_142, end_function_144, end_block_exception_145, end_function_171;

  struct ztuple_z8z5izCz0z5iz9 zgaz3312;
  CREATE(ztuple_z8z5izCz0z5iz9)(&zgaz3312);
  sailgen_getCapBounds(&zgaz3312, zc);
  sail_int zgsz373;
  CREATE(sail_int)(&zgsz373);
  {
    sail_int zbase;
    CREATE(sail_int)(&zbase);
    COPY(sail_int)(&zbase, zgaz3312.ztup0);
    sail_int ztop;
    CREATE(sail_int)(&ztop);
    COPY(sail_int)(&ztop, zgaz3312.ztup1);
    {
      bool zgaz3315;
      {
        bool zgaz3314;
        {
          bool zgaz3313;
          zgaz3313 = zc.ztag;
          zgaz3314 = not(zgaz3313);
        }
        bool zgsz374;
        if (zgaz3314) {  zgsz374 = true;  } else {  zgsz374 = gteq(ztop, zbase);  }
        zgaz3315 = zgsz374;
      }
      unit zgsz375;
      zgsz375 = sail_assert(zgaz3315, "src/cheri_cap_common.sail:453.40-453.41");
    }
    sail_int zgaz3316;
    CREATE(sail_int)(&zgaz3316);
    sub_int(&zgaz3316, ztop, zbase);
    sail_int zgaz3317;
    CREATE(sail_int)(&zgaz3317);
    {
      sail_int zgsz3644;
      CREATE(sail_int)(&zgsz3644);
      CONVERT_OF(sail_int, mach_int)(&zgsz3644, zcap_len_width);
      pow2(&zgaz3317, zgsz3644);
      KILL(sail_int)(&zgsz3644);
    }
    emod_int(&zgsz373, zgaz3316, zgaz3317);
    KILL(sail_int)(&zgaz3317);
    KILL(sail_int)(&zgaz3316);
    KILL(sail_int)(&ztop);
    KILL(sail_int)(&zbase);
    goto finish_match_142;
  }
case_143: ;
  sail_match_failure("getCapLength");
finish_match_142: ;
  COPY(sail_int)((*(&zcbz334)), zgsz373);
  KILL(sail_int)(&zgsz373);
  KILL(ztuple_z8z5izCz0z5iz9)(&zgaz3312);
end_function_144: ;
  goto end_function_171;
end_block_exception_145: ;
  goto end_function_171;
end_function_171: ;
}

static bool sailgen_fastRepCheck(struct zCapability, uint64_t);

static bool sailgen_fastRepCheck(struct zCapability zc, uint64_t zi)
{
  __label__ end_function_147, end_block_exception_148;

  bool zcbz335;
  int64_t zE;
  {
    uint64_t zgaz3331;
    zgaz3331 = zc.zE;
    {
      lbits zgsz3645;
      CREATE(lbits)(&zgsz3645);
      CONVERT_OF(lbits, fbits)(&zgsz3645, zgaz3331, UINT64_C(6) , true);
      sail_int zgsz3646;
      CREATE(sail_int)(&zgsz3646);
      sail_unsigned(&zgsz3646, zgsz3645);
      zE = CONVERT_OF(mach_int, sail_int)(zgsz3646);
      KILL(sail_int)(&zgsz3646);
      KILL(lbits)(&zgsz3645);
    }
  }
  bool zgaz3319;
  {
    int64_t zgaz3318;
    {
      sail_int zgsz3649;
      CREATE(sail_int)(&zgsz3649);
      CONVERT_OF(sail_int, mach_int)(&zgsz3649, zcap_max_E);
      sail_int zgsz3650;
      CREATE(sail_int)(&zgsz3650);
      CONVERT_OF(sail_int, mach_int)(&zgsz3650, INT64_C(2));
      sail_int zgsz3651;
      CREATE(sail_int)(&zgsz3651);
      sub_int(&zgsz3651, zgsz3649, zgsz3650);
      zgaz3318 = CONVERT_OF(mach_int, sail_int)(zgsz3651);
      KILL(sail_int)(&zgsz3651);
      KILL(sail_int)(&zgsz3650);
      KILL(sail_int)(&zgsz3649);
    }
    {
      sail_int zgsz3647;
      CREATE(sail_int)(&zgsz3647);
      CONVERT_OF(sail_int, mach_int)(&zgsz3647, zE);
      sail_int zgsz3648;
      CREATE(sail_int)(&zgsz3648);
      CONVERT_OF(sail_int, mach_int)(&zgsz3648, zgaz3318);
      zgaz3319 = gteq(zgsz3647, zgsz3648);
      KILL(sail_int)(&zgsz3648);
      KILL(sail_int)(&zgsz3647);
    }
  }
  if (zgaz3319) {  zcbz335 = true;  } else {
    int64_t zi_top;
    {
      uint64_t zgaz3330;
      {
        int64_t zgaz3329;
        {
          sail_int zgsz3655;
          CREATE(sail_int)(&zgsz3655);
          CONVERT_OF(sail_int, mach_int)(&zgsz3655, zE);
          sail_int zgsz3656;
          CREATE(sail_int)(&zgsz3656);
          CONVERT_OF(sail_int, mach_int)(&zgsz3656, zcap_mantissa_width);
          sail_int zgsz3657;
          CREATE(sail_int)(&zgsz3657);
          add_int(&zgsz3657, zgsz3655, zgsz3656);
          zgaz3329 = CONVERT_OF(mach_int, sail_int)(zgsz3657);
          KILL(sail_int)(&zgsz3657);
          KILL(sail_int)(&zgsz3656);
          KILL(sail_int)(&zgsz3655);
        }
        {
          lbits zgsz3652;
          CREATE(lbits)(&zgsz3652);
          CONVERT_OF(lbits, fbits)(&zgsz3652, zi, UINT64_C(64) , true);
          sail_int zgsz3653;
          CREATE(sail_int)(&zgsz3653);
          CONVERT_OF(sail_int, mach_int)(&zgsz3653, zgaz3329);
          lbits zgsz3654;
          CREATE(lbits)(&zgsz3654);
          arith_shiftr(&zgsz3654, zgsz3652, zgsz3653);
          zgaz3330 = CONVERT_OF(fbits, lbits)(zgsz3654, true);
          KILL(lbits)(&zgsz3654);
          KILL(sail_int)(&zgsz3653);
          KILL(lbits)(&zgsz3652);
        }
      }
      {
        lbits zgsz3658;
        CREATE(lbits)(&zgsz3658);
        CONVERT_OF(lbits, fbits)(&zgsz3658, zgaz3330, UINT64_C(64) , true);
        sail_int zgsz3659;
        CREATE(sail_int)(&zgsz3659);
        sail_signed(&zgsz3659, zgsz3658);
        zi_top = CONVERT_OF(mach_int, sail_int)(zgsz3659);
        KILL(sail_int)(&zgsz3659);
        KILL(lbits)(&zgsz3658);
      }
    }
    uint64_t zi_mid;
    {
      uint64_t zgaz3328;
      {
        lbits zgsz3663;
        CREATE(lbits)(&zgsz3663);
        CONVERT_OF(lbits, fbits)(&zgsz3663, zi, UINT64_C(64) , true);
        sail_int zgsz3664;
        CREATE(sail_int)(&zgsz3664);
        CONVERT_OF(sail_int, mach_int)(&zgsz3664, zE);
        lbits zgsz3665;
        CREATE(lbits)(&zgsz3665);
        shiftr(&zgsz3665, zgsz3663, zgsz3664);
        zgaz3328 = CONVERT_OF(fbits, lbits)(zgsz3665, true);
        KILL(lbits)(&zgsz3665);
        KILL(sail_int)(&zgsz3664);
        KILL(lbits)(&zgsz3663);
      }
      {
        lbits zgsz3660;
        CREATE(lbits)(&zgsz3660);
        CONVERT_OF(lbits, fbits)(&zgsz3660, zgaz3328, UINT64_C(64) , true);
        sail_int zgsz3661;
        CREATE(sail_int)(&zgsz3661);
        CONVERT_OF(sail_int, mach_int)(&zgsz3661, zcap_mantissa_width);
        lbits zgsz3662;
        CREATE(lbits)(&zgsz3662);
        sail_truncate(&zgsz3662, zgsz3660, zgsz3661);
        zi_mid = CONVERT_OF(fbits, lbits)(zgsz3662, true);
        KILL(lbits)(&zgsz3662);
        KILL(sail_int)(&zgsz3661);
        KILL(lbits)(&zgsz3660);
      }
    }
    uint64_t za_mid;
    {
      uint64_t zgaz3327;
      {
        uint64_t zgaz3326;
        zgaz3326 = zc.zaddress;
        {
          lbits zgsz3666;
          CREATE(lbits)(&zgsz3666);
          CONVERT_OF(lbits, fbits)(&zgsz3666, zgaz3326, UINT64_C(64) , true);
          sail_int zgsz3667;
          CREATE(sail_int)(&zgsz3667);
          CONVERT_OF(sail_int, mach_int)(&zgsz3667, zE);
          lbits zgsz3668;
          CREATE(lbits)(&zgsz3668);
          shiftr(&zgsz3668, zgsz3666, zgsz3667);
          zgaz3327 = CONVERT_OF(fbits, lbits)(zgsz3668, true);
          KILL(lbits)(&zgsz3668);
          KILL(sail_int)(&zgsz3667);
          KILL(lbits)(&zgsz3666);
        }
      }
      {
        lbits zgsz3669;
        CREATE(lbits)(&zgsz3669);
        CONVERT_OF(lbits, fbits)(&zgsz3669, zgaz3327, UINT64_C(64) , true);
        sail_int zgsz3670;
        CREATE(sail_int)(&zgsz3670);
        CONVERT_OF(sail_int, mach_int)(&zgsz3670, zcap_mantissa_width);
        lbits zgsz3671;
        CREATE(lbits)(&zgsz3671);
        sail_truncate(&zgsz3671, zgsz3669, zgsz3670);
        za_mid = CONVERT_OF(fbits, lbits)(zgsz3671, true);
        KILL(lbits)(&zgsz3671);
        KILL(sail_int)(&zgsz3670);
        KILL(lbits)(&zgsz3669);
      }
    }
    uint64_t zB3;
    {
      uint64_t zgaz3325;
      zgaz3325 = zc.zB;
      {
        lbits zgsz3672;
        CREATE(lbits)(&zgsz3672);
        CONVERT_OF(lbits, fbits)(&zgsz3672, zgaz3325, UINT64_C(14) , true);
        sail_int zgsz3673;
        CREATE(sail_int)(&zgsz3673);
        CONVERT_OF(sail_int, mach_int)(&zgsz3673, INT64_C(3));
        lbits zgsz3674;
        CREATE(lbits)(&zgsz3674);
        sail_truncateLSB(&zgsz3674, zgsz3672, zgsz3673);
        zB3 = CONVERT_OF(fbits, lbits)(zgsz3674, true);
        KILL(lbits)(&zgsz3674);
        KILL(sail_int)(&zgsz3673);
        KILL(lbits)(&zgsz3672);
      }
    }
    uint64_t zR3;
    {
      lbits zgsz3699;
      CREATE(lbits)(&zgsz3699);
      CONVERT_OF(lbits, fbits)(&zgsz3699, zB3, UINT64_C(3) , true);
      lbits zgsz3700;
      CREATE(lbits)(&zgsz3700);
      CONVERT_OF(lbits, fbits)(&zgsz3700, UINT64_C(0b001), UINT64_C(3) , true);
      lbits zgsz3701;
      CREATE(lbits)(&zgsz3701);
      sub_bits(&zgsz3701, zgsz3699, zgsz3700);
      zR3 = CONVERT_OF(fbits, lbits)(zgsz3701, true);
      KILL(lbits)(&zgsz3701);
      KILL(lbits)(&zgsz3700);
      KILL(lbits)(&zgsz3699);
    }
    uint64_t zR;
    {
      uint64_t zgaz3324;
      {
        int64_t zgaz3323;
        {
          sail_int zgsz3677;
          CREATE(sail_int)(&zgsz3677);
          CONVERT_OF(sail_int, mach_int)(&zgsz3677, zcap_mantissa_width);
          sail_int zgsz3678;
          CREATE(sail_int)(&zgsz3678);
          CONVERT_OF(sail_int, mach_int)(&zgsz3678, INT64_C(3));
          sail_int zgsz3679;
          CREATE(sail_int)(&zgsz3679);
          sub_int(&zgsz3679, zgsz3677, zgsz3678);
          zgaz3323 = CONVERT_OF(mach_int, sail_int)(zgsz3679);
          KILL(sail_int)(&zgsz3679);
          KILL(sail_int)(&zgsz3678);
          KILL(sail_int)(&zgsz3677);
        }
        {
          sail_int zgsz3675;
          CREATE(sail_int)(&zgsz3675);
          CONVERT_OF(sail_int, mach_int)(&zgsz3675, zgaz3323);
          lbits zgsz3676;
          CREATE(lbits)(&zgsz3676);
          sailgen_zzeros_implicit(&zgsz3676, zgsz3675);
          zgaz3324 = CONVERT_OF(fbits, lbits)(zgsz3676, true);
          KILL(lbits)(&zgsz3676);
          KILL(sail_int)(&zgsz3675);
        }
      }
      {
        lbits zgsz3680;
        CREATE(lbits)(&zgsz3680);
        CONVERT_OF(lbits, fbits)(&zgsz3680, zR3, UINT64_C(3) , true);
        lbits zgsz3681;
        CREATE(lbits)(&zgsz3681);
        CONVERT_OF(lbits, fbits)(&zgsz3681, zgaz3324, UINT64_C(11) , true);
        lbits zgsz3682;
        CREATE(lbits)(&zgsz3682);
        append(&zgsz3682, zgsz3680, zgsz3681);
        zR = CONVERT_OF(fbits, lbits)(zgsz3682, true);
        KILL(lbits)(&zgsz3682);
        KILL(lbits)(&zgsz3681);
        KILL(lbits)(&zgsz3680);
      }
    }
    uint64_t zdiff;
    {
      lbits zgsz3696;
      CREATE(lbits)(&zgsz3696);
      CONVERT_OF(lbits, fbits)(&zgsz3696, zR, UINT64_C(14) , true);
      lbits zgsz3697;
      CREATE(lbits)(&zgsz3697);
      CONVERT_OF(lbits, fbits)(&zgsz3697, za_mid, UINT64_C(14) , true);
      lbits zgsz3698;
      CREATE(lbits)(&zgsz3698);
      sub_bits(&zgsz3698, zgsz3696, zgsz3697);
      zdiff = CONVERT_OF(fbits, lbits)(zgsz3698, true);
      KILL(lbits)(&zgsz3698);
      KILL(lbits)(&zgsz3697);
      KILL(lbits)(&zgsz3696);
    }
    uint64_t zdiff1;
    {
      lbits zgsz3693;
      CREATE(lbits)(&zgsz3693);
      CONVERT_OF(lbits, fbits)(&zgsz3693, zdiff, UINT64_C(14) , true);
      sail_int zgsz3694;
      CREATE(sail_int)(&zgsz3694);
      CONVERT_OF(sail_int, mach_int)(&zgsz3694, INT64_C(1));
      lbits zgsz3695;
      CREATE(lbits)(&zgsz3695);
      sub_bits_int(&zgsz3695, zgsz3693, zgsz3694);
      zdiff1 = CONVERT_OF(fbits, lbits)(zgsz3695, true);
      KILL(lbits)(&zgsz3695);
      KILL(sail_int)(&zgsz3694);
      KILL(lbits)(&zgsz3693);
    }
    bool zgaz3320;
    {
      sail_int zgsz3691;
      CREATE(sail_int)(&zgsz3691);
      CONVERT_OF(sail_int, mach_int)(&zgsz3691, zi_top);
      sail_int zgsz3692;
      CREATE(sail_int)(&zgsz3692);
      CONVERT_OF(sail_int, mach_int)(&zgsz3692, INT64_C(0));
      zgaz3320 = eq_int(zgsz3691, zgsz3692);
      KILL(sail_int)(&zgsz3692);
      KILL(sail_int)(&zgsz3691);
    }
    if (zgaz3320) {
      {
        lbits zgsz3689;
        CREATE(lbits)(&zgsz3689);
        CONVERT_OF(lbits, fbits)(&zgsz3689, zi_mid, UINT64_C(14) , true);
        lbits zgsz3690;
        CREATE(lbits)(&zgsz3690);
        CONVERT_OF(lbits, fbits)(&zgsz3690, zdiff1, UINT64_C(14) , true);
        zcbz335 = sailgen_z8operatorz0zI_uz9(zgsz3689, zgsz3690);
        KILL(lbits)(&zgsz3690);
        KILL(lbits)(&zgsz3689);
      }
    } else {
      bool zgaz3321;
      {
        sail_int zgsz3687;
        CREATE(sail_int)(&zgsz3687);
        CONVERT_OF(sail_int, mach_int)(&zgsz3687, zi_top);
        sail_int zgsz3688;
        CREATE(sail_int)(&zgsz3688);
        CONVERT_OF(sail_int, mach_int)(&zgsz3688, INT64_C(-1));
        zgaz3321 = eq_int(zgsz3687, zgsz3688);
        KILL(sail_int)(&zgsz3688);
        KILL(sail_int)(&zgsz3687);
      }
      if (zgaz3321) {
        bool zgaz3322;
        {
          lbits zgsz3685;
          CREATE(lbits)(&zgsz3685);
          CONVERT_OF(lbits, fbits)(&zgsz3685, zi_mid, UINT64_C(14) , true);
          lbits zgsz3686;
          CREATE(lbits)(&zgsz3686);
          CONVERT_OF(lbits, fbits)(&zgsz3686, zdiff, UINT64_C(14) , true);
          zgaz3322 = sailgen_z8operatorz0zKzJ_uz9(zgsz3685, zgsz3686);
          KILL(lbits)(&zgsz3686);
          KILL(lbits)(&zgsz3685);
        }
        bool zgsz377;
        if (zgaz3322) {
          {
            lbits zgsz3683;
            CREATE(lbits)(&zgsz3683);
            CONVERT_OF(lbits, fbits)(&zgsz3683, zR, UINT64_C(14) , true);
            lbits zgsz3684;
            CREATE(lbits)(&zgsz3684);
            CONVERT_OF(lbits, fbits)(&zgsz3684, za_mid, UINT64_C(14) , true);
            zgsz377 = neq_bits(zgsz3683, zgsz3684);
            KILL(lbits)(&zgsz3684);
            KILL(lbits)(&zgsz3683);
          }
        } else {  zgsz377 = false;  }
        zcbz335 = zgsz377;
      } else {  zcbz335 = false;  }
    }
  }


end_function_147: ;
  return zcbz335;
end_block_exception_148: ;

  return false;
}

static void sailgen_capToString(sail_string *rop, struct zCapability);

static void sailgen_capToString(sail_string *zcbz336, struct zCapability zcap)
{
  __label__ end_function_150, end_block_exception_151, end_function_170;

  sail_int zlen;
  CREATE(sail_int)(&zlen);
  sailgen_getCapLength(&zlen, zcap);
  sail_string zlen_str;
  CREATE(sail_string)(&zlen_str);
  {
    lbits zgaz3360;
    CREATE(lbits)(&zgaz3360);
    {
      int64_t zgaz3359;
      {
        sail_int zgsz3703;
        CREATE(sail_int)(&zgsz3703);
        CONVERT_OF(sail_int, mach_int)(&zgsz3703, zcap_len_width);
        sail_int zgsz3704;
        CREATE(sail_int)(&zgsz3704);
        CONVERT_OF(sail_int, mach_int)(&zgsz3704, INT64_C(3));
        sail_int zgsz3705;
        CREATE(sail_int)(&zgsz3705);
        add_int(&zgsz3705, zgsz3703, zgsz3704);
        zgaz3359 = CONVERT_OF(mach_int, sail_int)(zgsz3705);
        KILL(sail_int)(&zgsz3705);
        KILL(sail_int)(&zgsz3704);
        KILL(sail_int)(&zgsz3703);
      }
      {
        sail_int zgsz3702;
        CREATE(sail_int)(&zgsz3702);
        CONVERT_OF(sail_int, mach_int)(&zgsz3702, zgaz3359);
        sailgen_to_bits(&zgaz3360, zgsz3702, zlen);
        KILL(sail_int)(&zgsz3702);
      }
    }
    string_of_lbits(&zlen_str, zgaz3360);
    KILL(lbits)(&zgaz3360);
  }
  uint64_t zotype64;
  {
    bool zgaz3356;
    zgaz3356 = sailgen_hasReservedOType(zcap);
    if (zgaz3356) {
      uint64_t zgaz3357;
      zgaz3357 = zcap.zotype;
      {
        sail_int zgsz3709;
        CREATE(sail_int)(&zgsz3709);
        CONVERT_OF(sail_int, mach_int)(&zgsz3709, INT64_C(64));
        lbits zgsz3710;
        CREATE(lbits)(&zgsz3710);
        CONVERT_OF(lbits, fbits)(&zgsz3710, zgaz3357, UINT64_C(18) , true);
        lbits zgsz3711;
        CREATE(lbits)(&zgsz3711);
        sailgen_EXTS(&zgsz3711, zgsz3709, zgsz3710);
        zotype64 = CONVERT_OF(fbits, lbits)(zgsz3711, true);
        KILL(lbits)(&zgsz3711);
        KILL(lbits)(&zgsz3710);
        KILL(sail_int)(&zgsz3709);
      }
    } else {
      uint64_t zgaz3358;
      zgaz3358 = zcap.zotype;
      {
        sail_int zgsz3706;
        CREATE(sail_int)(&zgsz3706);
        CONVERT_OF(sail_int, mach_int)(&zgsz3706, INT64_C(64));
        lbits zgsz3707;
        CREATE(lbits)(&zgsz3707);
        CONVERT_OF(lbits, fbits)(&zgsz3707, zgaz3358, UINT64_C(18) , true);
        lbits zgsz3708;
        CREATE(lbits)(&zgsz3708);
        sailgen_EXTZ(&zgsz3708, zgsz3706, zgsz3707);
        zotype64 = CONVERT_OF(fbits, lbits)(zgsz3708, true);
        KILL(lbits)(&zgsz3708);
        KILL(lbits)(&zgsz3707);
        KILL(sail_int)(&zgsz3706);
      }
    }
  }
  sail_string zgaz3355;
  CREATE(sail_string)(&zgaz3355);
  {
    sail_string zgaz3353;
    CREATE(sail_string)(&zgaz3353);
    {
      bool zgaz3332;
      zgaz3332 = zcap.ztag;
      if (zgaz3332) {  COPY(sail_string)(&zgaz3353, "1");  } else {  COPY(sail_string)(&zgaz3353, "0");  }
    }
    sail_string zgaz3354;
    CREATE(sail_string)(&zgaz3354);
    {
      sail_string zgaz3352;
      CREATE(sail_string)(&zgaz3352);
      {
        sail_string zgaz3350;
        CREATE(sail_string)(&zgaz3350);
        {
          bool zgaz3333;
          zgaz3333 = sailgen_isCapSealed(zcap);
          if (zgaz3333) {  COPY(sail_string)(&zgaz3350, "1");  } else {  COPY(sail_string)(&zgaz3350, "0");  }
        }
        sail_string zgaz3351;
        CREATE(sail_string)(&zgaz3351);
        {
          sail_string zgaz3349;
          CREATE(sail_string)(&zgaz3349);
          {
            sail_string zgaz3347;
            CREATE(sail_string)(&zgaz3347);
            {
              uint64_t zgaz3335;
              {
                uint64_t zgaz3334;
                zgaz3334 = sailgen_getCapPerms(zcap);
                {
                  lbits zgsz3712;
                  CREATE(lbits)(&zgsz3712);
                  CONVERT_OF(lbits, fbits)(&zgsz3712, UINT64_C(0b0), UINT64_C(1) , true);
                  lbits zgsz3713;
                  CREATE(lbits)(&zgsz3713);
                  CONVERT_OF(lbits, fbits)(&zgsz3713, zgaz3334, UINT64_C(19) , true);
                  lbits zgsz3714;
                  CREATE(lbits)(&zgsz3714);
                  append(&zgsz3714, zgsz3712, zgsz3713);
                  zgaz3335 = CONVERT_OF(fbits, lbits)(zgsz3714, true);
                  KILL(lbits)(&zgsz3714);
                  KILL(lbits)(&zgsz3713);
                  KILL(lbits)(&zgsz3712);
                }
              }
              {
                lbits zgsz3715;
                CREATE(lbits)(&zgsz3715);
                CONVERT_OF(lbits, fbits)(&zgsz3715, zgaz3335, UINT64_C(20) , true);
                string_of_lbits(&zgaz3347, zgsz3715);
                KILL(lbits)(&zgsz3715);
              }
            }
            sail_string zgaz3348;
            CREATE(sail_string)(&zgaz3348);
            {
              sail_string zgaz3346;
              CREATE(sail_string)(&zgaz3346);
              {
                sail_string zgaz3344;
                CREATE(sail_string)(&zgaz3344);
                {
                  lbits zgsz3718;
                  CREATE(lbits)(&zgsz3718);
                  CONVERT_OF(lbits, fbits)(&zgsz3718, zotype64, UINT64_C(64) , true);
                  string_of_lbits(&zgaz3344, zgsz3718);
                  KILL(lbits)(&zgsz3718);
                }
                sail_string zgaz3345;
                CREATE(sail_string)(&zgaz3345);
                {
                  sail_string zgaz3343;
                  CREATE(sail_string)(&zgaz3343);
                  {
                    sail_string zgaz3341;
                    CREATE(sail_string)(&zgaz3341);
                    {
                      uint64_t zgaz3336;
                      zgaz3336 = sailgen_getCapOffsetBits(zcap);
                      {
                        lbits zgsz3716;
                        CREATE(lbits)(&zgsz3716);
                        CONVERT_OF(lbits, fbits)(&zgsz3716, zgaz3336, UINT64_C(64) , true);
                        string_of_lbits(&zgaz3341, zgsz3716);
                        KILL(lbits)(&zgsz3716);
                      }
                    }
                    sail_string zgaz3342;
                    CREATE(sail_string)(&zgaz3342);
                    {
                      sail_string zgaz3340;
                      CREATE(sail_string)(&zgaz3340);
                      {
                        sail_string zgaz3338;
                        CREATE(sail_string)(&zgaz3338);
                        {
                          uint64_t zgaz3337;
                          zgaz3337 = sailgen_getCapBaseBits(zcap);
                          {
                            lbits zgsz3717;
                            CREATE(lbits)(&zgsz3717);
                            CONVERT_OF(lbits, fbits)(&zgsz3717, zgaz3337, UINT64_C(64) , true);
                            string_of_lbits(&zgaz3338, zgsz3717);
                            KILL(lbits)(&zgsz3717);
                          }
                        }
                        sail_string zgaz3339;
                        CREATE(sail_string)(&zgaz3339);
                        concat_str(&zgaz3339, " length:", zlen_str);
                        concat_str(&zgaz3340, zgaz3338, zgaz3339);
                        KILL(sail_string)(&zgaz3339);
                        KILL(sail_string)(&zgaz3338);
                      }
                      concat_str(&zgaz3342, " base:", zgaz3340);
                      KILL(sail_string)(&zgaz3340);
                    }
                    concat_str(&zgaz3343, zgaz3341, zgaz3342);
                    KILL(sail_string)(&zgaz3342);
                    KILL(sail_string)(&zgaz3341);
                  }
                  concat_str(&zgaz3345, " offset:", zgaz3343);
                  KILL(sail_string)(&zgaz3343);
                }
                concat_str(&zgaz3346, zgaz3344, zgaz3345);
                KILL(sail_string)(&zgaz3345);
                KILL(sail_string)(&zgaz3344);
              }
              concat_str(&zgaz3348, " type:", zgaz3346);
              KILL(sail_string)(&zgaz3346);
            }
            concat_str(&zgaz3349, zgaz3347, zgaz3348);
            KILL(sail_string)(&zgaz3348);
            KILL(sail_string)(&zgaz3347);
          }
          concat_str(&zgaz3351, " perms:", zgaz3349);
          KILL(sail_string)(&zgaz3349);
        }
        concat_str(&zgaz3352, zgaz3350, zgaz3351);
        KILL(sail_string)(&zgaz3351);
        KILL(sail_string)(&zgaz3350);
      }
      concat_str(&zgaz3354, " s:", zgaz3352);
      KILL(sail_string)(&zgaz3352);
    }
    concat_str(&zgaz3355, zgaz3353, zgaz3354);
    KILL(sail_string)(&zgaz3354);
    KILL(sail_string)(&zgaz3353);
  }
  concat_str((*(&zcbz336)), " t:", zgaz3355);
  KILL(sail_string)(&zgaz3355);

  KILL(sail_string)(&zlen_str);
  KILL(sail_int)(&zlen);
end_function_150: ;
  goto end_function_170;
end_block_exception_151: ;
  goto end_function_170;
end_function_170: ;
}

static uint64_t sailgen_getRepresentableAlignmentMask(uint64_t);

static uint64_t sailgen_getRepresentableAlignmentMask(uint64_t zlen)
{
  __label__ case_154, finish_match_153, end_function_155, end_block_exception_156;

  uint64_t zcbz337;
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3363;
  {
    uint64_t zgaz3361;
    {
      sail_int zgsz3721;
      CREATE(sail_int)(&zgsz3721);
      CONVERT_OF(sail_int, mach_int)(&zgsz3721, INT64_C(64));
      sail_int zgsz3722;
      CREATE(sail_int)(&zgsz3722);
      CONVERT_OF(sail_int, mach_int)(&zgsz3722, INT64_C(0));
      lbits zgsz3723;
      CREATE(lbits)(&zgsz3723);
      sailgen_to_bits(&zgsz3723, zgsz3721, zgsz3722);
      zgaz3361 = CONVERT_OF(fbits, lbits)(zgsz3723, true);
      KILL(lbits)(&zgsz3723);
      KILL(sail_int)(&zgsz3722);
      KILL(sail_int)(&zgsz3721);
    }
    lbits zgaz3362;
    CREATE(lbits)(&zgaz3362);
    {
      lbits zgsz3719;
      CREATE(lbits)(&zgsz3719);
      CONVERT_OF(lbits, fbits)(&zgsz3719, UINT64_C(0b0), UINT64_C(1) , true);
      lbits zgsz3720;
      CREATE(lbits)(&zgsz3720);
      CONVERT_OF(lbits, fbits)(&zgsz3720, zlen, UINT64_C(64) , true);
      append(&zgaz3362, zgsz3719, zgsz3720);
      KILL(lbits)(&zgsz3720);
      KILL(lbits)(&zgsz3719);
    }
    zgaz3363 = sailgen_setCapBounds(zdefault_cap, zgaz3361, zgaz3362);
    KILL(lbits)(&zgaz3362);
  }
  uint64_t zgsz378;
  {
    bool zexact;
    zexact = zgaz3363.ztup0;
    struct zCapability zc;
    zc = zgaz3363.ztup1;
    int64_t ze;
    {
      int64_t zgaz3369;
      {
        uint64_t zgaz3368;
        zgaz3368 = zc.zE;
        {
          lbits zgsz3724;
          CREATE(lbits)(&zgsz3724);
          CONVERT_OF(lbits, fbits)(&zgsz3724, zgaz3368, UINT64_C(6) , true);
          sail_int zgsz3725;
          CREATE(sail_int)(&zgsz3725);
          sail_unsigned(&zgsz3725, zgsz3724);
          zgaz3369 = CONVERT_OF(mach_int, sail_int)(zgsz3725);
          KILL(sail_int)(&zgsz3725);
          KILL(lbits)(&zgsz3724);
        }
      }
      {
        sail_int zgsz3726;
        CREATE(sail_int)(&zgsz3726);
        CONVERT_OF(sail_int, mach_int)(&zgsz3726, zgaz3369);
        sail_int zgsz3727;
        CREATE(sail_int)(&zgsz3727);
        CONVERT_OF(sail_int, mach_int)(&zgsz3727, zcap_max_E);
        sail_int zgsz3728;
        CREATE(sail_int)(&zgsz3728);
        min_int(&zgsz3728, zgsz3726, zgsz3727);
        ze = CONVERT_OF(mach_int, sail_int)(zgsz3728);
        KILL(sail_int)(&zgsz3728);
        KILL(sail_int)(&zgsz3727);
        KILL(sail_int)(&zgsz3726);
      }
    }
    int64_t zez7;
    {
      bool zgaz3367;
      zgaz3367 = zc.zinternal_E;
      if (zgaz3367) {
        {
          sail_int zgsz3729;
          CREATE(sail_int)(&zgsz3729);
          CONVERT_OF(sail_int, mach_int)(&zgsz3729, ze);
          sail_int zgsz3730;
          CREATE(sail_int)(&zgsz3730);
          CONVERT_OF(sail_int, mach_int)(&zgsz3730, zinternal_E_take_bits);
          sail_int zgsz3731;
          CREATE(sail_int)(&zgsz3731);
          add_int(&zgsz3731, zgsz3729, zgsz3730);
          zez7 = CONVERT_OF(mach_int, sail_int)(zgsz3731);
          KILL(sail_int)(&zgsz3731);
          KILL(sail_int)(&zgsz3730);
          KILL(sail_int)(&zgsz3729);
        }
      } else {  zez7 = INT64_C(0);  }
    }
    sbits zgaz3365;
    {
      int64_t zgaz3364;
      {
        sail_int zgsz3734;
        CREATE(sail_int)(&zgsz3734);
        CONVERT_OF(sail_int, mach_int)(&zgsz3734, INT64_C(64));
        sail_int zgsz3735;
        CREATE(sail_int)(&zgsz3735);
        CONVERT_OF(sail_int, mach_int)(&zgsz3735, zez7);
        sail_int zgsz3736;
        CREATE(sail_int)(&zgsz3736);
        sub_int(&zgsz3736, zgsz3734, zgsz3735);
        zgaz3364 = CONVERT_OF(mach_int, sail_int)(zgsz3736);
        KILL(sail_int)(&zgsz3736);
        KILL(sail_int)(&zgsz3735);
        KILL(sail_int)(&zgsz3734);
      }
      {
        sail_int zgsz3732;
        CREATE(sail_int)(&zgsz3732);
        CONVERT_OF(sail_int, mach_int)(&zgsz3732, zgaz3364);
        lbits zgsz3733;
        CREATE(lbits)(&zgsz3733);
        sailgen_ones(&zgsz3733, zgsz3732);
        zgaz3365 = CONVERT_OF(sbits, lbits)(zgsz3733, true);
        KILL(lbits)(&zgsz3733);
        KILL(sail_int)(&zgsz3732);
      }
    }
    sbits zgaz3366;
    {
      sail_int zgsz3740;
      CREATE(sail_int)(&zgsz3740);
      CONVERT_OF(sail_int, mach_int)(&zgsz3740, zez7);
      lbits zgsz3741;
      CREATE(lbits)(&zgsz3741);
      sailgen_zzeros_implicit(&zgsz3741, zgsz3740);
      zgaz3366 = CONVERT_OF(sbits, lbits)(zgsz3741, true);
      KILL(lbits)(&zgsz3741);
      KILL(sail_int)(&zgsz3740);
    }
    {
      lbits zgsz3737;
      CREATE(lbits)(&zgsz3737);
      CONVERT_OF(lbits, sbits)(&zgsz3737, zgaz3365, true);
      lbits zgsz3738;
      CREATE(lbits)(&zgsz3738);
      CONVERT_OF(lbits, sbits)(&zgsz3738, zgaz3366, true);
      lbits zgsz3739;
      CREATE(lbits)(&zgsz3739);
      append(&zgsz3739, zgsz3737, zgsz3738);
      zgsz378 = CONVERT_OF(fbits, lbits)(zgsz3739, true);
      KILL(lbits)(&zgsz3739);
      KILL(lbits)(&zgsz3738);
      KILL(lbits)(&zgsz3737);
    }
    goto finish_match_153;
  }
case_154: ;
  sail_match_failure("getRepresentableAlignmentMask");
finish_match_153: ;
  zcbz337 = zgsz378;


end_function_155: ;
  return zcbz337;
end_block_exception_156: ;

  return UINT64_C(0xdeadc0de);
}

static uint64_t sailgen_getRepresentableLength(uint64_t);

static uint64_t sailgen_getRepresentableLength(uint64_t zlen)
{
  __label__ end_function_158, end_block_exception_159;

  uint64_t zcbz338;
  uint64_t zm;
  zm = sailgen_getRepresentableAlignmentMask(zlen);
  uint64_t zgaz3371;
  {
    uint64_t zgaz3370;
    {
      lbits zgsz3745;
      CREATE(lbits)(&zgsz3745);
      CONVERT_OF(lbits, fbits)(&zgsz3745, zm, UINT64_C(64) , true);
      lbits zgsz3746;
      CREATE(lbits)(&zgsz3746);
      not_bits(&zgsz3746, zgsz3745);
      zgaz3370 = CONVERT_OF(fbits, lbits)(zgsz3746, true);
      KILL(lbits)(&zgsz3746);
      KILL(lbits)(&zgsz3745);
    }
    {
      lbits zgsz3742;
      CREATE(lbits)(&zgsz3742);
      CONVERT_OF(lbits, fbits)(&zgsz3742, zlen, UINT64_C(64) , true);
      lbits zgsz3743;
      CREATE(lbits)(&zgsz3743);
      CONVERT_OF(lbits, fbits)(&zgsz3743, zgaz3370, UINT64_C(64) , true);
      lbits zgsz3744;
      CREATE(lbits)(&zgsz3744);
      add_bits(&zgsz3744, zgsz3742, zgsz3743);
      zgaz3371 = CONVERT_OF(fbits, lbits)(zgsz3744, true);
      KILL(lbits)(&zgsz3744);
      KILL(lbits)(&zgsz3743);
      KILL(lbits)(&zgsz3742);
    }
  }
  {
    lbits zgsz3747;
    CREATE(lbits)(&zgsz3747);
    CONVERT_OF(lbits, fbits)(&zgsz3747, zgaz3371, UINT64_C(64) , true);
    lbits zgsz3748;
    CREATE(lbits)(&zgsz3748);
    CONVERT_OF(lbits, fbits)(&zgsz3748, zm, UINT64_C(64) , true);
    lbits zgsz3749;
    CREATE(lbits)(&zgsz3749);
    and_bits(&zgsz3749, zgsz3747, zgsz3748);
    zcbz338 = CONVERT_OF(fbits, lbits)(zgsz3749, true);
    KILL(lbits)(&zgsz3749);
    KILL(lbits)(&zgsz3748);
    KILL(lbits)(&zgsz3747);
  }


end_function_158: ;
  return zcbz338;
end_block_exception_159: ;

  return UINT64_C(0xdeadc0de);
}

// register TestCap
static struct zCapability zTestCap;

// register TestAddr
static uint64_t zTestAddr;

// register TestLen
static lbits zTestLen;

static unit sailgen_main(unit);


static unit sailgen_main(unit zgsz380)
{
  __label__ case_162, finish_match_161, end_function_165, end_block_exception_166;

  unit zcbz339;
  {
    zTestCap = znull_cap;
    unit zgsz381;
    zgsz381 = UNIT;
  }
  lbits zbits1;
  CREATE(lbits)(&zbits1);
  sailgen_capToBits(&zbits1, zTestCap);
  lbits zbits2;
  CREATE(lbits)(&zbits2);
  sailgen_capToMemBits(&zbits2, zTestCap);
  struct zCapability zcap1;
  {
    bool zgaz3377;
    zgaz3377 = zTestCap.ztag;
    zcap1 = sailgen_capBitsToCapability(zgaz3377, zbits1);
  }
  struct zCapability zcap2;
  {
    bool zgaz3376;
    zgaz3376 = zTestCap.ztag;
    zcap2 = sailgen_memBitsToCapability(zgaz3376, zbits2);
  }
  bool zrep;
  zrep = sailgen_fastRepCheck(zTestCap, zTestAddr);
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3372;
  zgaz3372 = sailgen_setCapBounds(zTestCap, zTestAddr, zTestLen);
  unit zgsz382;
  {
    __label__ case_164, finish_match_163;

    bool zexact;
    zexact = zgaz3372.ztup0;
    struct zCapability zcap;
    zcap = zgaz3372.ztup1;
    struct ztuple_z8z5izCz0z5iz9 zgaz3373;
    CREATE(ztuple_z8z5izCz0z5iz9)(&zgaz3373);
    sailgen_getCapBounds(&zgaz3373, zTestCap);
    unit zgsz383;
    {
      sail_int zbase;
      CREATE(sail_int)(&zbase);
      COPY(sail_int)(&zbase, zgaz3373.ztup0);
      sail_int ztop;
      CREATE(sail_int)(&ztop);
      COPY(sail_int)(&ztop, zgaz3373.ztup1);
      uint64_t zflags;
      zflags = sailgen_getCapFlags(zTestCap);
      struct zCapability znew_cap;
      {
        uint64_t zgaz3375;
        {
          int64_t zgaz3374;
          {
            sail_int zgsz3752;
            CREATE(sail_int)(&zgsz3752);
            CONVERT_OF(sail_int, mach_int)(&zgsz3752, INT64_C(15));
            sail_int zgsz3753;
            CREATE(sail_int)(&zgsz3753);
            CONVERT_OF(sail_int, mach_int)(&zgsz3753, INT64_C(4));
            sail_int zgsz3754;
            CREATE(sail_int)(&zgsz3754);
            add_int(&zgsz3754, zgsz3752, zgsz3753);
            zgaz3374 = CONVERT_OF(mach_int, sail_int)(zgsz3754);
            KILL(sail_int)(&zgsz3754);
            KILL(sail_int)(&zgsz3753);
            KILL(sail_int)(&zgsz3752);
          }
          {
            sail_int zgsz3750;
            CREATE(sail_int)(&zgsz3750);
            CONVERT_OF(sail_int, mach_int)(&zgsz3750, zgaz3374);
            lbits zgsz3751;
            CREATE(lbits)(&zgsz3751);
            sailgen_ones(&zgsz3751, zgsz3750);
            zgaz3375 = CONVERT_OF(fbits, lbits)(zgsz3751, true);
            KILL(lbits)(&zgsz3751);
            KILL(sail_int)(&zgsz3750);
          }
        }
        znew_cap = sailgen_setCapPerms(zTestCap, zgaz3375);
      }
      sail_string zcap_str;
      CREATE(sail_string)(&zcap_str);
      sailgen_capToString(&zcap_str, zTestCap);
      uint64_t zlen;
      zlen = sailgen_getRepresentableLength(zTestAddr);
      uint64_t zmask;
      zmask = sailgen_getRepresentableAlignmentMask(zTestAddr);
      zgsz383 = UNIT;
      KILL(sail_string)(&zcap_str);
      KILL(sail_int)(&ztop);
      KILL(sail_int)(&zbase);
      goto finish_match_163;
    }
  case_164: ;
    sail_match_failure("main");
  finish_match_163: ;
    zgsz382 = zgsz383;
    KILL(ztuple_z8z5izCz0z5iz9)(&zgaz3373);
    goto finish_match_161;
  }
case_162: ;
  sail_match_failure("main");
finish_match_161: ;
  zcbz339 = zgsz382;





  KILL(lbits)(&zbits2);
  KILL(lbits)(&zbits1);
end_function_165: ;
  return zcbz339;
end_block_exception_166: ;

  return UNIT;
}

static unit sailgen_initializze_registers(unit);

static unit sailgen_initializze_registers(unit zgsz386)
{
  __label__ end_function_168, end_block_exception_169;

  unit zcbz340;
  {
    zTestCap = sailgen_undefined_Capability(UNIT);
    unit zgsz388;
    zgsz388 = UNIT;
  }
  {
    {
      sail_int zgsz3755;
      CREATE(sail_int)(&zgsz3755);
      CONVERT_OF(sail_int, mach_int)(&zgsz3755, INT64_C(64));
      lbits zgsz3756;
      CREATE(lbits)(&zgsz3756);
      UNDEFINED(lbits)(&zgsz3756, zgsz3755);
      zTestAddr = CONVERT_OF(fbits, lbits)(zgsz3756, true);
      KILL(lbits)(&zgsz3756);
      KILL(sail_int)(&zgsz3755);
    }
    unit zgsz387;
    zgsz387 = UNIT;
  }
  int64_t zgaz3378;
  {
    sail_int zgsz3758;
    CREATE(sail_int)(&zgsz3758);
    CONVERT_OF(sail_int, mach_int)(&zgsz3758, INT64_C(64));
    sail_int zgsz3759;
    CREATE(sail_int)(&zgsz3759);
    CONVERT_OF(sail_int, mach_int)(&zgsz3759, INT64_C(1));
    sail_int zgsz3760;
    CREATE(sail_int)(&zgsz3760);
    add_int(&zgsz3760, zgsz3758, zgsz3759);
    zgaz3378 = CONVERT_OF(mach_int, sail_int)(zgsz3760);
    KILL(sail_int)(&zgsz3760);
    KILL(sail_int)(&zgsz3759);
    KILL(sail_int)(&zgsz3758);
  }
  {
    sail_int zgsz3757;
    CREATE(sail_int)(&zgsz3757);
    CONVERT_OF(sail_int, mach_int)(&zgsz3757, zgaz3378);
    UNDEFINED(lbits)(&zTestLen, zgsz3757);
    KILL(sail_int)(&zgsz3757);
  }
  zcbz340 = UNIT;

end_function_168: ;
  return zcbz340;
end_block_exception_169: ;

  return UNIT;
}

static void model_init(void)
{
  setup_rts();
  current_exception = sail_malloc(sizeof(struct zexception));
  CREATE(zexception)(current_exception);
  throw_location = sail_malloc(sizeof(sail_string));
  CREATE(sail_string)(throw_location);
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
  CREATE(lbits)(&zTestLen);
  sailgen_initializze_registers(UNIT);
}

static void model_fini(void)
{
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
  KILL(lbits)(&zTestLen);
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


