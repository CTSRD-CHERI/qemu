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
  __label__ end_function_7, end_block_exception_8, end_function_177;

  lbits zgaz32;
  CREATE(lbits)(&zgaz32);
  zeros(&zgaz32, zn);
  not_bits((*(&zcbz32)), zgaz32);
  KILL(lbits)(&zgaz32);
end_function_7: ;
  goto end_function_177;
end_block_exception_8: ;
  goto end_function_177;
end_function_177: ;
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
  __label__ end_function_13, end_block_exception_14, end_function_176;

  sign_extend((*(&zcbz34)), zv, zm);
end_function_13: ;
  goto end_function_176;
end_block_exception_14: ;
  goto end_function_176;
end_function_176: ;
}

static void sailgen_EXTZ(lbits *zcbz35, sail_int zm, lbits zv)
{
  __label__ end_function_16, end_block_exception_17, end_function_175;

  zero_extend((*(&zcbz35)), zv, zm);
end_function_16: ;
  goto end_function_175;
end_block_exception_17: ;
  goto end_function_175;
end_function_175: ;
}

static void sailgen_zzeros_implicit(lbits *rop, sail_int);

static void sailgen_zzeros_implicit(lbits *zcbz36, sail_int zn)
{
  __label__ end_function_19, end_block_exception_20, end_function_174;

  zeros((*(&zcbz36)), zn);
end_function_19: ;
  goto end_function_174;
end_block_exception_20: ;
  goto end_function_174;
end_function_174: ;
}

static void sailgen_ones(lbits *rop, sail_int);

static void sailgen_ones(lbits *zcbz37, sail_int zn)
{
  __label__ end_function_22, end_block_exception_23, end_function_173;

  sailgen_sail_ones((*(&zcbz37)), zn);
end_function_22: ;
  goto end_function_173;
end_block_exception_23: ;
  goto end_function_173;
end_function_173: ;
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
  __label__ end_function_34, end_block_exception_35, end_function_172;

  {
    sail_int zgsz389;
    CREATE(sail_int)(&zgsz389);
    CONVERT_OF(sail_int, mach_int)(&zgsz389, INT64_C(0));
    get_slice_int((*(&zcbz310)), zl, zn, zgsz389);
    KILL(sail_int)(&zgsz389);
  }
end_function_34: ;
  goto end_function_172;
end_block_exception_35: ;
  goto end_function_172;
end_function_172: ;
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
  __label__ end_function_43, end_block_exception_44, end_function_171;

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
  goto end_function_171;
end_block_exception_44: ;
  goto end_function_171;
end_function_171: ;
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
  zgsz38 = INT64_C(8);
  zcap_sizze = zgsz38;

let_end_50: ;
}
static void kill_letbind_5(void) {
}

static int64_t zlog2_cap_sizze;
static void create_letbind_6(void) {


  int64_t zgsz39;
  zgsz39 = INT64_C(3);
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
  zgsz311 = INT64_C(0);
  zcap_uperms_width = zgsz311;

let_end_53: ;
}
static void kill_letbind_8(void) {
}

static int64_t zcap_otype_width;
static void create_letbind_9(void) {


  int64_t zgsz312;
  zgsz312 = INT64_C(4);
  zcap_otype_width = zgsz312;

let_end_54: ;
}
static void kill_letbind_9(void) {
}

static int64_t zcap_reserved_width;
static void create_letbind_10(void) {


  int64_t zgsz313;
  zgsz313 = INT64_C(0);
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
  zgsz315 = INT64_C(8);
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
  zgsz317 = INT64_C(32);
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
    CONVERT_OF(sail_int, mach_int)(&zgsz391, INT64_C(32));
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
  zgsz319 = INT64_C(8);
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

static struct zEncCapability sailgen_capBitsToEncCapability(uint64_t);

static struct zEncCapability sailgen_capBitsToEncCapability(uint64_t zc)
{
  __label__ end_function_64, end_block_exception_65;

  struct zEncCapability zcbz314;
  uint64_t zgaz39;
  {
    lbits zgsz3116;
    CREATE(lbits)(&zgsz3116);
    CONVERT_OF(lbits, fbits)(&zgsz3116, zc, UINT64_C(64) , true);
    sail_int zgsz3117;
    CREATE(sail_int)(&zgsz3117);
    CONVERT_OF(sail_int, mach_int)(&zgsz3117, INT64_C(63));
    sail_int zgsz3118;
    CREATE(sail_int)(&zgsz3118);
    CONVERT_OF(sail_int, mach_int)(&zgsz3118, INT64_C(52));
    lbits zgsz3119;
    CREATE(lbits)(&zgsz3119);
    vector_subrange_lbits(&zgsz3119, zgsz3116, zgsz3117, zgsz3118);
    zgaz39 = CONVERT_OF(fbits, lbits)(zgsz3119, true);
    KILL(lbits)(&zgsz3119);
    KILL(sail_int)(&zgsz3118);
    KILL(sail_int)(&zgsz3117);
    KILL(lbits)(&zgsz3116);
  }
  uint64_t zgaz310;
  {
    lbits zgsz3112;
    CREATE(lbits)(&zgsz3112);
    CONVERT_OF(lbits, fbits)(&zgsz3112, zc, UINT64_C(64) , true);
    sail_int zgsz3113;
    CREATE(sail_int)(&zgsz3113);
    CONVERT_OF(sail_int, mach_int)(&zgsz3113, INT64_C(51));
    sail_int zgsz3114;
    CREATE(sail_int)(&zgsz3114);
    CONVERT_OF(sail_int, mach_int)(&zgsz3114, INT64_C(51));
    lbits zgsz3115;
    CREATE(lbits)(&zgsz3115);
    vector_subrange_lbits(&zgsz3115, zgsz3112, zgsz3113, zgsz3114);
    zgaz310 = CONVERT_OF(fbits, lbits)(zgsz3115, true);
    KILL(lbits)(&zgsz3115);
    KILL(sail_int)(&zgsz3114);
    KILL(sail_int)(&zgsz3113);
    KILL(lbits)(&zgsz3112);
  }
  uint64_t zgaz311;
  {
    lbits zgsz3108;
    CREATE(lbits)(&zgsz3108);
    CONVERT_OF(lbits, fbits)(&zgsz3108, zc, UINT64_C(64) , true);
    sail_int zgsz3109;
    CREATE(sail_int)(&zgsz3109);
    CONVERT_OF(sail_int, mach_int)(&zgsz3109, INT64_C(50));
    sail_int zgsz3110;
    CREATE(sail_int)(&zgsz3110);
    CONVERT_OF(sail_int, mach_int)(&zgsz3110, INT64_C(47));
    lbits zgsz3111;
    CREATE(lbits)(&zgsz3111);
    vector_subrange_lbits(&zgsz3111, zgsz3108, zgsz3109, zgsz3110);
    zgaz311 = CONVERT_OF(fbits, lbits)(zgsz3111, true);
    KILL(lbits)(&zgsz3111);
    KILL(sail_int)(&zgsz3110);
    KILL(sail_int)(&zgsz3109);
    KILL(lbits)(&zgsz3108);
  }
  fbits zgaz312;
  {
    lbits zgsz3106;
    CREATE(lbits)(&zgsz3106);
    CONVERT_OF(lbits, fbits)(&zgsz3106, zc, UINT64_C(64) , true);
    sail_int zgsz3107;
    CREATE(sail_int)(&zgsz3107);
    CONVERT_OF(sail_int, mach_int)(&zgsz3107, INT64_C(46));
    zgaz312 = bitvector_access(zgsz3106, zgsz3107);
    KILL(sail_int)(&zgsz3107);
    KILL(lbits)(&zgsz3106);
  }
  uint64_t zgaz313;
  {
    lbits zgsz3102;
    CREATE(lbits)(&zgsz3102);
    CONVERT_OF(lbits, fbits)(&zgsz3102, zc, UINT64_C(64) , true);
    sail_int zgsz3103;
    CREATE(sail_int)(&zgsz3103);
    CONVERT_OF(sail_int, mach_int)(&zgsz3103, INT64_C(45));
    sail_int zgsz3104;
    CREATE(sail_int)(&zgsz3104);
    CONVERT_OF(sail_int, mach_int)(&zgsz3104, INT64_C(40));
    lbits zgsz3105;
    CREATE(lbits)(&zgsz3105);
    vector_subrange_lbits(&zgsz3105, zgsz3102, zgsz3103, zgsz3104);
    zgaz313 = CONVERT_OF(fbits, lbits)(zgsz3105, true);
    KILL(lbits)(&zgsz3105);
    KILL(sail_int)(&zgsz3104);
    KILL(sail_int)(&zgsz3103);
    KILL(lbits)(&zgsz3102);
  }
  uint64_t zgaz314;
  {
    lbits zgsz398;
    CREATE(lbits)(&zgsz398);
    CONVERT_OF(lbits, fbits)(&zgsz398, zc, UINT64_C(64) , true);
    sail_int zgsz399;
    CREATE(sail_int)(&zgsz399);
    CONVERT_OF(sail_int, mach_int)(&zgsz399, INT64_C(39));
    sail_int zgsz3100;
    CREATE(sail_int)(&zgsz3100);
    CONVERT_OF(sail_int, mach_int)(&zgsz3100, INT64_C(32));
    lbits zgsz3101;
    CREATE(lbits)(&zgsz3101);
    vector_subrange_lbits(&zgsz3101, zgsz398, zgsz399, zgsz3100);
    zgaz314 = CONVERT_OF(fbits, lbits)(zgsz3101, true);
    KILL(lbits)(&zgsz3101);
    KILL(sail_int)(&zgsz3100);
    KILL(sail_int)(&zgsz399);
    KILL(lbits)(&zgsz398);
  }
  uint64_t zgaz315;
  {
    lbits zgsz394;
    CREATE(lbits)(&zgsz394);
    CONVERT_OF(lbits, fbits)(&zgsz394, zc, UINT64_C(64) , true);
    sail_int zgsz395;
    CREATE(sail_int)(&zgsz395);
    CONVERT_OF(sail_int, mach_int)(&zgsz395, INT64_C(31));
    sail_int zgsz396;
    CREATE(sail_int)(&zgsz396);
    CONVERT_OF(sail_int, mach_int)(&zgsz396, INT64_C(0));
    lbits zgsz397;
    CREATE(lbits)(&zgsz397);
    vector_subrange_lbits(&zgsz397, zgsz394, zgsz395, zgsz396);
    zgaz315 = CONVERT_OF(fbits, lbits)(zgsz397, true);
    KILL(lbits)(&zgsz397);
    KILL(sail_int)(&zgsz396);
    KILL(sail_int)(&zgsz395);
    KILL(lbits)(&zgsz394);
  }
  struct zEncCapability zgsz321;
  zgsz321.zB = zgaz314;
  zgsz321.zT = zgaz313;
  zgsz321.zaddress = zgaz315;
  zgsz321.zflags = zgaz310;
  zgsz321.zinternal_E = zgaz312;
  zgsz321.zotype = zgaz311;
  zgsz321.zperms = zgaz39;
  zgsz321.zreserved = UINT64_C(0);
  zcbz314 = zgsz321;








end_function_64: ;
  return zcbz314;
end_block_exception_65: ;
  struct zEncCapability zcbz341 = { .zreserved = UINT64_C(0xdeadc0de), .zperms = UINT64_C(0xdeadc0de), .zotype = UINT64_C(0xdeadc0de), .zinternal_E = UINT64_C(0), .zflags = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  return zcbz341;
}

static uint64_t sailgen_encCapToBits(struct zEncCapability);

static uint64_t sailgen_encCapToBits(struct zEncCapability zcap)
{
  __label__ end_function_67, end_block_exception_68;

  uint64_t zcbz315;
  uint64_t zgaz329;
  zgaz329 = zcap.zperms;
  uint64_t zgaz330;
  {
    uint64_t zgaz327;
    zgaz327 = zcap.zreserved;
    uint64_t zgaz328;
    {
      uint64_t zgaz325;
      zgaz325 = zcap.zflags;
      uint64_t zgaz326;
      {
        uint64_t zgaz323;
        zgaz323 = zcap.zotype;
        uint64_t zgaz324;
        {
          uint64_t zgaz321;
          {
            fbits zgaz316;
            zgaz316 = zcap.zinternal_E;
            uint64_t zgsz322;
            zgsz322 = UINT64_C(0b0);
            zgsz322 = update_fbits(zgsz322, INT64_C(0), zgaz316);
            zgaz321 = zgsz322;
          }
          uint64_t zgaz322;
          {
            uint64_t zgaz319;
            zgaz319 = zcap.zT;
            uint64_t zgaz320;
            {
              uint64_t zgaz317;
              zgaz317 = zcap.zB;
              uint64_t zgaz318;
              zgaz318 = zcap.zaddress;
              {
                lbits zgsz3120;
                CREATE(lbits)(&zgsz3120);
                CONVERT_OF(lbits, fbits)(&zgsz3120, zgaz317, UINT64_C(8) , true);
                lbits zgsz3121;
                CREATE(lbits)(&zgsz3121);
                CONVERT_OF(lbits, fbits)(&zgsz3121, zgaz318, UINT64_C(32) , true);
                lbits zgsz3122;
                CREATE(lbits)(&zgsz3122);
                append(&zgsz3122, zgsz3120, zgsz3121);
                zgaz320 = CONVERT_OF(fbits, lbits)(zgsz3122, true);
                KILL(lbits)(&zgsz3122);
                KILL(lbits)(&zgsz3121);
                KILL(lbits)(&zgsz3120);
              }
            }
            {
              lbits zgsz3123;
              CREATE(lbits)(&zgsz3123);
              CONVERT_OF(lbits, fbits)(&zgsz3123, zgaz319, UINT64_C(6) , true);
              lbits zgsz3124;
              CREATE(lbits)(&zgsz3124);
              CONVERT_OF(lbits, fbits)(&zgsz3124, zgaz320, UINT64_C(40) , true);
              lbits zgsz3125;
              CREATE(lbits)(&zgsz3125);
              append(&zgsz3125, zgsz3123, zgsz3124);
              zgaz322 = CONVERT_OF(fbits, lbits)(zgsz3125, true);
              KILL(lbits)(&zgsz3125);
              KILL(lbits)(&zgsz3124);
              KILL(lbits)(&zgsz3123);
            }
          }
          {
            lbits zgsz3126;
            CREATE(lbits)(&zgsz3126);
            CONVERT_OF(lbits, fbits)(&zgsz3126, zgaz321, UINT64_C(1) , true);
            lbits zgsz3127;
            CREATE(lbits)(&zgsz3127);
            CONVERT_OF(lbits, fbits)(&zgsz3127, zgaz322, UINT64_C(46) , true);
            lbits zgsz3128;
            CREATE(lbits)(&zgsz3128);
            append(&zgsz3128, zgsz3126, zgsz3127);
            zgaz324 = CONVERT_OF(fbits, lbits)(zgsz3128, true);
            KILL(lbits)(&zgsz3128);
            KILL(lbits)(&zgsz3127);
            KILL(lbits)(&zgsz3126);
          }
        }
        {
          lbits zgsz3129;
          CREATE(lbits)(&zgsz3129);
          CONVERT_OF(lbits, fbits)(&zgsz3129, zgaz323, UINT64_C(4) , true);
          lbits zgsz3130;
          CREATE(lbits)(&zgsz3130);
          CONVERT_OF(lbits, fbits)(&zgsz3130, zgaz324, UINT64_C(47) , true);
          lbits zgsz3131;
          CREATE(lbits)(&zgsz3131);
          append(&zgsz3131, zgsz3129, zgsz3130);
          zgaz326 = CONVERT_OF(fbits, lbits)(zgsz3131, true);
          KILL(lbits)(&zgsz3131);
          KILL(lbits)(&zgsz3130);
          KILL(lbits)(&zgsz3129);
        }
      }
      {
        lbits zgsz3132;
        CREATE(lbits)(&zgsz3132);
        CONVERT_OF(lbits, fbits)(&zgsz3132, zgaz325, UINT64_C(1) , true);
        lbits zgsz3133;
        CREATE(lbits)(&zgsz3133);
        CONVERT_OF(lbits, fbits)(&zgsz3133, zgaz326, UINT64_C(51) , true);
        lbits zgsz3134;
        CREATE(lbits)(&zgsz3134);
        append(&zgsz3134, zgsz3132, zgsz3133);
        zgaz328 = CONVERT_OF(fbits, lbits)(zgsz3134, true);
        KILL(lbits)(&zgsz3134);
        KILL(lbits)(&zgsz3133);
        KILL(lbits)(&zgsz3132);
      }
    }
    {
      lbits zgsz3135;
      CREATE(lbits)(&zgsz3135);
      CONVERT_OF(lbits, fbits)(&zgsz3135, zgaz327, UINT64_C(0) , true);
      lbits zgsz3136;
      CREATE(lbits)(&zgsz3136);
      CONVERT_OF(lbits, fbits)(&zgsz3136, zgaz328, UINT64_C(52) , true);
      lbits zgsz3137;
      CREATE(lbits)(&zgsz3137);
      append(&zgsz3137, zgsz3135, zgsz3136);
      zgaz330 = CONVERT_OF(fbits, lbits)(zgsz3137, true);
      KILL(lbits)(&zgsz3137);
      KILL(lbits)(&zgsz3136);
      KILL(lbits)(&zgsz3135);
    }
  }
  {
    lbits zgsz3138;
    CREATE(lbits)(&zgsz3138);
    CONVERT_OF(lbits, fbits)(&zgsz3138, zgaz329, UINT64_C(12) , true);
    lbits zgsz3139;
    CREATE(lbits)(&zgsz3139);
    CONVERT_OF(lbits, fbits)(&zgsz3139, zgaz330, UINT64_C(52) , true);
    lbits zgsz3140;
    CREATE(lbits)(&zgsz3140);
    append(&zgsz3140, zgsz3138, zgsz3139);
    zcbz315 = CONVERT_OF(fbits, lbits)(zgsz3140, true);
    KILL(lbits)(&zgsz3140);
    KILL(lbits)(&zgsz3139);
    KILL(lbits)(&zgsz3138);
  }


end_function_67: ;
  return zcbz315;
end_block_exception_68: ;

  return UINT64_C(0xdeadc0de);
}

static int64_t zcap_max_addr;
static void create_letbind_18(void) {


  int64_t zgsz323;
  {
    sail_int zgsz3141;
    CREATE(sail_int)(&zgsz3141);
    CONVERT_OF(sail_int, mach_int)(&zgsz3141, zcap_addr_width);
    sail_int zgsz3142;
    CREATE(sail_int)(&zgsz3142);
    sailgen_MAX(&zgsz3142, zgsz3141);
    zgsz323 = CONVERT_OF(mach_int, sail_int)(zgsz3142);
    KILL(sail_int)(&zgsz3142);
    KILL(sail_int)(&zgsz3141);
  }
  zcap_max_addr = zgsz323;

let_end_69: ;
}
static void kill_letbind_18(void) {
}

static int64_t zcap_max_otype;
static void create_letbind_19(void) {


  int64_t zgsz324;
  int64_t zgaz331;
  {
    sail_int zgsz3146;
    CREATE(sail_int)(&zgsz3146);
    CONVERT_OF(sail_int, mach_int)(&zgsz3146, zcap_otype_width);
    sail_int zgsz3147;
    CREATE(sail_int)(&zgsz3147);
    sailgen_MAX(&zgsz3147, zgsz3146);
    zgaz331 = CONVERT_OF(mach_int, sail_int)(zgsz3147);
    KILL(sail_int)(&zgsz3147);
    KILL(sail_int)(&zgsz3146);
  }
  {
    sail_int zgsz3143;
    CREATE(sail_int)(&zgsz3143);
    CONVERT_OF(sail_int, mach_int)(&zgsz3143, zgaz331);
    sail_int zgsz3144;
    CREATE(sail_int)(&zgsz3144);
    CONVERT_OF(sail_int, mach_int)(&zgsz3144, zreserved_otypes);
    sail_int zgsz3145;
    CREATE(sail_int)(&zgsz3145);
    sub_int(&zgsz3145, zgsz3143, zgsz3144);
    zgsz324 = CONVERT_OF(mach_int, sail_int)(zgsz3145);
    KILL(sail_int)(&zgsz3145);
    KILL(sail_int)(&zgsz3144);
    KILL(sail_int)(&zgsz3143);
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
    sail_int zgsz3148;
    CREATE(sail_int)(&zgsz3148);
    CONVERT_OF(sail_int, mach_int)(&zgsz3148, INT64_C(15));
    sail_int zgsz3149;
    CREATE(sail_int)(&zgsz3149);
    CONVERT_OF(sail_int, mach_int)(&zgsz3149, INT64_C(0));
    sail_int zgsz3150;
    CREATE(sail_int)(&zgsz3150);
    add_int(&zgsz3150, zgsz3148, zgsz3149);
    zgsz326 = CONVERT_OF(mach_int, sail_int)(zgsz3150);
    KILL(sail_int)(&zgsz3150);
    KILL(sail_int)(&zgsz3149);
    KILL(sail_int)(&zgsz3148);
  }
  zcap_perms_width = zgsz326;

let_end_72: ;
}
static void kill_letbind_21(void) {
}

static int64_t zcap_max_E;
static void create_letbind_22(void) {


  int64_t zgsz327;
  int64_t zgaz332;
  {
    sail_int zgsz3154;
    CREATE(sail_int)(&zgsz3154);
    CONVERT_OF(sail_int, mach_int)(&zgsz3154, zcap_len_width);
    sail_int zgsz3155;
    CREATE(sail_int)(&zgsz3155);
    CONVERT_OF(sail_int, mach_int)(&zgsz3155, zcap_mantissa_width);
    sail_int zgsz3156;
    CREATE(sail_int)(&zgsz3156);
    sub_int(&zgsz3156, zgsz3154, zgsz3155);
    zgaz332 = CONVERT_OF(mach_int, sail_int)(zgsz3156);
    KILL(sail_int)(&zgsz3156);
    KILL(sail_int)(&zgsz3155);
    KILL(sail_int)(&zgsz3154);
  }
  {
    sail_int zgsz3151;
    CREATE(sail_int)(&zgsz3151);
    CONVERT_OF(sail_int, mach_int)(&zgsz3151, zgaz332);
    sail_int zgsz3152;
    CREATE(sail_int)(&zgsz3152);
    CONVERT_OF(sail_int, mach_int)(&zgsz3152, INT64_C(1));
    sail_int zgsz3153;
    CREATE(sail_int)(&zgsz3153);
    add_int(&zgsz3153, zgsz3151, zgsz3152);
    zgsz327 = CONVERT_OF(mach_int, sail_int)(zgsz3153);
    KILL(sail_int)(&zgsz3153);
    KILL(sail_int)(&zgsz3152);
    KILL(sail_int)(&zgsz3151);
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
    sail_int zgsz3157;
    CREATE(sail_int)(&zgsz3157);
    CONVERT_OF(sail_int, mach_int)(&zgsz3157, zcap_E_width);
    sail_int zgsz3158;
    CREATE(sail_int)(&zgsz3158);
    CONVERT_OF(sail_int, mach_int)(&zgsz3158, zcap_max_E);
    lbits zgsz3159;
    CREATE(lbits)(&zgsz3159);
    sailgen_to_bits(&zgsz3159, zgsz3157, zgsz3158);
    zgsz328 = CONVERT_OF(fbits, lbits)(zgsz3159, true);
    KILL(lbits)(&zgsz3159);
    KILL(sail_int)(&zgsz3158);
    KILL(sail_int)(&zgsz3157);
  }
  zcap_reset_E = zgsz328;

let_end_74: ;
}
static void kill_letbind_23(void) {
}

static uint64_t zcap_reset_T;
static void create_letbind_24(void) {


  uint64_t zgsz329;
  uint64_t zgaz334;
  {
    int64_t zgaz333;
    {
      sail_int zgsz3162;
      CREATE(sail_int)(&zgsz3162);
      CONVERT_OF(sail_int, mach_int)(&zgsz3162, zcap_mantissa_width);
      sail_int zgsz3163;
      CREATE(sail_int)(&zgsz3163);
      CONVERT_OF(sail_int, mach_int)(&zgsz3163, INT64_C(2));
      sail_int zgsz3164;
      CREATE(sail_int)(&zgsz3164);
      sub_int(&zgsz3164, zgsz3162, zgsz3163);
      zgaz333 = CONVERT_OF(mach_int, sail_int)(zgsz3164);
      KILL(sail_int)(&zgsz3164);
      KILL(sail_int)(&zgsz3163);
      KILL(sail_int)(&zgsz3162);
    }
    {
      sail_int zgsz3160;
      CREATE(sail_int)(&zgsz3160);
      CONVERT_OF(sail_int, mach_int)(&zgsz3160, zgaz333);
      lbits zgsz3161;
      CREATE(lbits)(&zgsz3161);
      sailgen_zzeros_implicit(&zgsz3161, zgsz3160);
      zgaz334 = CONVERT_OF(fbits, lbits)(zgsz3161, true);
      KILL(lbits)(&zgsz3161);
      KILL(sail_int)(&zgsz3160);
    }
  }
  {
    lbits zgsz3165;
    CREATE(lbits)(&zgsz3165);
    CONVERT_OF(lbits, fbits)(&zgsz3165, UINT64_C(0b01), UINT64_C(2) , true);
    lbits zgsz3166;
    CREATE(lbits)(&zgsz3166);
    CONVERT_OF(lbits, fbits)(&zgsz3166, zgaz334, UINT64_C(6) , true);
    lbits zgsz3167;
    CREATE(lbits)(&zgsz3167);
    append(&zgsz3167, zgsz3165, zgsz3166);
    zgsz329 = CONVERT_OF(fbits, lbits)(zgsz3167, true);
    KILL(lbits)(&zgsz3167);
    KILL(lbits)(&zgsz3166);
    KILL(lbits)(&zgsz3165);
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
  bool zgaz335;
  zgaz335 = undefined_bool(UNIT);
  uint64_t zgaz336;
  {
    sail_int zgsz3180;
    CREATE(sail_int)(&zgsz3180);
    CONVERT_OF(sail_int, mach_int)(&zgsz3180, INT64_C(0));
    lbits zgsz3181;
    CREATE(lbits)(&zgsz3181);
    UNDEFINED(lbits)(&zgsz3181, zgsz3180);
    zgaz336 = CONVERT_OF(fbits, lbits)(zgsz3181, true);
    KILL(lbits)(&zgsz3181);
    KILL(sail_int)(&zgsz3180);
  }
  bool zgaz337;
  zgaz337 = undefined_bool(UNIT);
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
  uint64_t zgaz349;
  {
    sail_int zgsz3178;
    CREATE(sail_int)(&zgsz3178);
    CONVERT_OF(sail_int, mach_int)(&zgsz3178, INT64_C(0));
    lbits zgsz3179;
    CREATE(lbits)(&zgsz3179);
    UNDEFINED(lbits)(&zgsz3179, zgsz3178);
    zgaz349 = CONVERT_OF(fbits, lbits)(zgsz3179, true);
    KILL(lbits)(&zgsz3179);
    KILL(sail_int)(&zgsz3178);
  }
  bool zgaz350;
  zgaz350 = undefined_bool(UNIT);
  bool zgaz351;
  zgaz351 = undefined_bool(UNIT);
  uint64_t zgaz352;
  {
    sail_int zgsz3176;
    CREATE(sail_int)(&zgsz3176);
    CONVERT_OF(sail_int, mach_int)(&zgsz3176, INT64_C(6));
    lbits zgsz3177;
    CREATE(lbits)(&zgsz3177);
    UNDEFINED(lbits)(&zgsz3177, zgsz3176);
    zgaz352 = CONVERT_OF(fbits, lbits)(zgsz3177, true);
    KILL(lbits)(&zgsz3177);
    KILL(sail_int)(&zgsz3176);
  }
  uint64_t zgaz353;
  {
    sail_int zgsz3174;
    CREATE(sail_int)(&zgsz3174);
    CONVERT_OF(sail_int, mach_int)(&zgsz3174, INT64_C(8));
    lbits zgsz3175;
    CREATE(lbits)(&zgsz3175);
    UNDEFINED(lbits)(&zgsz3175, zgsz3174);
    zgaz353 = CONVERT_OF(fbits, lbits)(zgsz3175, true);
    KILL(lbits)(&zgsz3175);
    KILL(sail_int)(&zgsz3174);
  }
  uint64_t zgaz354;
  {
    sail_int zgsz3172;
    CREATE(sail_int)(&zgsz3172);
    CONVERT_OF(sail_int, mach_int)(&zgsz3172, INT64_C(8));
    lbits zgsz3173;
    CREATE(lbits)(&zgsz3173);
    UNDEFINED(lbits)(&zgsz3173, zgsz3172);
    zgaz354 = CONVERT_OF(fbits, lbits)(zgsz3173, true);
    KILL(lbits)(&zgsz3173);
    KILL(sail_int)(&zgsz3172);
  }
  uint64_t zgaz355;
  {
    sail_int zgsz3170;
    CREATE(sail_int)(&zgsz3170);
    CONVERT_OF(sail_int, mach_int)(&zgsz3170, INT64_C(4));
    lbits zgsz3171;
    CREATE(lbits)(&zgsz3171);
    UNDEFINED(lbits)(&zgsz3171, zgsz3170);
    zgaz355 = CONVERT_OF(fbits, lbits)(zgsz3171, true);
    KILL(lbits)(&zgsz3171);
    KILL(sail_int)(&zgsz3170);
  }
  uint64_t zgaz356;
  {
    sail_int zgsz3168;
    CREATE(sail_int)(&zgsz3168);
    CONVERT_OF(sail_int, mach_int)(&zgsz3168, INT64_C(32));
    lbits zgsz3169;
    CREATE(lbits)(&zgsz3169);
    UNDEFINED(lbits)(&zgsz3169, zgsz3168);
    zgaz356 = CONVERT_OF(fbits, lbits)(zgsz3169, true);
    KILL(lbits)(&zgsz3169);
    KILL(sail_int)(&zgsz3168);
  }
  struct zCapability zgsz331;
  zgsz331.zB = zgaz353;
  zgsz331.zE = zgaz352;
  zgsz331.zT = zgaz354;
  zgsz331.zaccess_system_regs = zgaz338;
  zgsz331.zaddress = zgaz356;
  zgsz331.zflag_cap_mode = zgaz350;
  zgsz331.zglobal = zgaz348;
  zgsz331.zinternal_E = zgaz351;
  zgsz331.zotype = zgaz355;
  zgsz331.zpermit_cinvoke = zgaz340;
  zgsz331.zpermit_execute = zgaz347;
  zgsz331.zpermit_load = zgaz346;
  zgsz331.zpermit_load_cap = zgaz344;
  zgsz331.zpermit_seal = zgaz341;
  zgsz331.zpermit_set_CID = zgaz337;
  zgsz331.zpermit_store = zgaz345;
  zgsz331.zpermit_store_cap = zgaz343;
  zgsz331.zpermit_store_local_cap = zgaz342;
  zgsz331.zpermit_unseal = zgaz339;
  zgsz331.zreserved = zgaz349;
  zgsz331.ztag = zgaz335;
  zgsz331.zuperms = zgaz336;
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
  uint64_t zgaz357;
  {
    sail_int zgsz3191;
    CREATE(sail_int)(&zgsz3191);
    CONVERT_OF(sail_int, mach_int)(&zgsz3191, INT64_C(0));
    lbits zgsz3192;
    CREATE(lbits)(&zgsz3192);
    sailgen_zzeros_implicit(&zgsz3192, zgsz3191);
    zgaz357 = CONVERT_OF(fbits, lbits)(zgsz3192, true);
    KILL(lbits)(&zgsz3192);
    KILL(sail_int)(&zgsz3191);
  }
  uint64_t zgaz358;
  {
    sail_int zgsz3189;
    CREATE(sail_int)(&zgsz3189);
    CONVERT_OF(sail_int, mach_int)(&zgsz3189, INT64_C(0));
    lbits zgsz3190;
    CREATE(lbits)(&zgsz3190);
    sailgen_zzeros_implicit(&zgsz3190, zgsz3189);
    zgaz358 = CONVERT_OF(fbits, lbits)(zgsz3190, true);
    KILL(lbits)(&zgsz3190);
    KILL(sail_int)(&zgsz3189);
  }
  uint64_t zgaz359;
  {
    sail_int zgsz3187;
    CREATE(sail_int)(&zgsz3187);
    CONVERT_OF(sail_int, mach_int)(&zgsz3187, INT64_C(8));
    lbits zgsz3188;
    CREATE(lbits)(&zgsz3188);
    sailgen_zzeros_implicit(&zgsz3188, zgsz3187);
    zgaz359 = CONVERT_OF(fbits, lbits)(zgsz3188, true);
    KILL(lbits)(&zgsz3188);
    KILL(sail_int)(&zgsz3187);
  }
  uint64_t zgaz360;
  {
    sail_int zgsz3184;
    CREATE(sail_int)(&zgsz3184);
    CONVERT_OF(sail_int, mach_int)(&zgsz3184, zcap_otype_width);
    sail_int zgsz3185;
    CREATE(sail_int)(&zgsz3185);
    CONVERT_OF(sail_int, mach_int)(&zgsz3185, zotype_unsealed);
    lbits zgsz3186;
    CREATE(lbits)(&zgsz3186);
    sailgen_to_bits(&zgsz3186, zgsz3184, zgsz3185);
    zgaz360 = CONVERT_OF(fbits, lbits)(zgsz3186, true);
    KILL(lbits)(&zgsz3186);
    KILL(sail_int)(&zgsz3185);
    KILL(sail_int)(&zgsz3184);
  }
  uint64_t zgaz361;
  {
    sail_int zgsz3182;
    CREATE(sail_int)(&zgsz3182);
    CONVERT_OF(sail_int, mach_int)(&zgsz3182, INT64_C(32));
    lbits zgsz3183;
    CREATE(lbits)(&zgsz3183);
    sailgen_zzeros_implicit(&zgsz3183, zgsz3182);
    zgaz361 = CONVERT_OF(fbits, lbits)(zgsz3183, true);
    KILL(lbits)(&zgsz3183);
    KILL(sail_int)(&zgsz3182);
  }
  struct zCapability zgsz332;
  zgsz332.zB = zgaz359;
  zgsz332.zE = zcap_reset_E;
  zgsz332.zT = zcap_reset_T;
  zgsz332.zaccess_system_regs = false;
  zgsz332.zaddress = zgaz361;
  zgsz332.zflag_cap_mode = false;
  zgsz332.zglobal = false;
  zgsz332.zinternal_E = true;
  zgsz332.zotype = zgaz360;
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
  zgsz332.zreserved = zgaz358;
  zgsz332.ztag = false;
  zgsz332.zuperms = zgaz357;
  zgsz333 = zgsz332;






  znull_cap = zgsz333;

let_end_79: ;
}
static void kill_letbind_25(void) {
}

static struct zCapability zdefault_cap;
static void create_letbind_26(void) {


  struct zCapability zgsz335;
  uint64_t zgaz362;
  {
    sail_int zgsz3202;
    CREATE(sail_int)(&zgsz3202);
    CONVERT_OF(sail_int, mach_int)(&zgsz3202, INT64_C(0));
    lbits zgsz3203;
    CREATE(lbits)(&zgsz3203);
    sailgen_ones(&zgsz3203, zgsz3202);
    zgaz362 = CONVERT_OF(fbits, lbits)(zgsz3203, true);
    KILL(lbits)(&zgsz3203);
    KILL(sail_int)(&zgsz3202);
  }
  uint64_t zgaz363;
  {
    sail_int zgsz3200;
    CREATE(sail_int)(&zgsz3200);
    CONVERT_OF(sail_int, mach_int)(&zgsz3200, INT64_C(0));
    lbits zgsz3201;
    CREATE(lbits)(&zgsz3201);
    sailgen_zzeros_implicit(&zgsz3201, zgsz3200);
    zgaz363 = CONVERT_OF(fbits, lbits)(zgsz3201, true);
    KILL(lbits)(&zgsz3201);
    KILL(sail_int)(&zgsz3200);
  }
  uint64_t zgaz364;
  {
    sail_int zgsz3198;
    CREATE(sail_int)(&zgsz3198);
    CONVERT_OF(sail_int, mach_int)(&zgsz3198, INT64_C(8));
    lbits zgsz3199;
    CREATE(lbits)(&zgsz3199);
    sailgen_zzeros_implicit(&zgsz3199, zgsz3198);
    zgaz364 = CONVERT_OF(fbits, lbits)(zgsz3199, true);
    KILL(lbits)(&zgsz3199);
    KILL(sail_int)(&zgsz3198);
  }
  uint64_t zgaz365;
  {
    sail_int zgsz3195;
    CREATE(sail_int)(&zgsz3195);
    CONVERT_OF(sail_int, mach_int)(&zgsz3195, zcap_otype_width);
    sail_int zgsz3196;
    CREATE(sail_int)(&zgsz3196);
    CONVERT_OF(sail_int, mach_int)(&zgsz3196, zotype_unsealed);
    lbits zgsz3197;
    CREATE(lbits)(&zgsz3197);
    sailgen_to_bits(&zgsz3197, zgsz3195, zgsz3196);
    zgaz365 = CONVERT_OF(fbits, lbits)(zgsz3197, true);
    KILL(lbits)(&zgsz3197);
    KILL(sail_int)(&zgsz3196);
    KILL(sail_int)(&zgsz3195);
  }
  uint64_t zgaz366;
  {
    sail_int zgsz3193;
    CREATE(sail_int)(&zgsz3193);
    CONVERT_OF(sail_int, mach_int)(&zgsz3193, INT64_C(32));
    lbits zgsz3194;
    CREATE(lbits)(&zgsz3194);
    sailgen_zzeros_implicit(&zgsz3194, zgsz3193);
    zgaz366 = CONVERT_OF(fbits, lbits)(zgsz3194, true);
    KILL(lbits)(&zgsz3194);
    KILL(sail_int)(&zgsz3193);
  }
  struct zCapability zgsz334;
  zgsz334.zB = zgaz364;
  zgsz334.zE = zcap_reset_E;
  zgsz334.zT = zcap_reset_T;
  zgsz334.zaccess_system_regs = true;
  zgsz334.zaddress = zgaz366;
  zgsz334.zflag_cap_mode = false;
  zgsz334.zglobal = true;
  zgsz334.zinternal_E = true;
  zgsz334.zotype = zgaz365;
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
  zgsz334.zreserved = zgaz363;
  zgsz334.ztag = true;
  zgsz334.zuperms = zgaz362;
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
  uint64_t zgaz399;
  {
    bool zgaz367;
    zgaz367 = zcap.zpermit_set_CID;
    zgaz399 = sailgen_bool_to_bits(zgaz367);
  }
  uint64_t zgaz3100;
  {
    uint64_t zgaz397;
    {
      bool zgaz368;
      zgaz368 = zcap.zaccess_system_regs;
      zgaz397 = sailgen_bool_to_bits(zgaz368);
    }
    uint64_t zgaz398;
    {
      uint64_t zgaz395;
      {
        bool zgaz369;
        zgaz369 = zcap.zpermit_unseal;
        zgaz395 = sailgen_bool_to_bits(zgaz369);
      }
      uint64_t zgaz396;
      {
        uint64_t zgaz393;
        {
          bool zgaz370;
          zgaz370 = zcap.zpermit_cinvoke;
          zgaz393 = sailgen_bool_to_bits(zgaz370);
        }
        uint64_t zgaz394;
        {
          uint64_t zgaz391;
          {
            bool zgaz371;
            zgaz371 = zcap.zpermit_seal;
            zgaz391 = sailgen_bool_to_bits(zgaz371);
          }
          uint64_t zgaz392;
          {
            uint64_t zgaz389;
            {
              bool zgaz372;
              zgaz372 = zcap.zpermit_store_local_cap;
              zgaz389 = sailgen_bool_to_bits(zgaz372);
            }
            uint64_t zgaz390;
            {
              uint64_t zgaz387;
              {
                bool zgaz373;
                zgaz373 = zcap.zpermit_store_cap;
                zgaz387 = sailgen_bool_to_bits(zgaz373);
              }
              uint64_t zgaz388;
              {
                uint64_t zgaz385;
                {
                  bool zgaz374;
                  zgaz374 = zcap.zpermit_load_cap;
                  zgaz385 = sailgen_bool_to_bits(zgaz374);
                }
                uint64_t zgaz386;
                {
                  uint64_t zgaz383;
                  {
                    bool zgaz375;
                    zgaz375 = zcap.zpermit_store;
                    zgaz383 = sailgen_bool_to_bits(zgaz375);
                  }
                  uint64_t zgaz384;
                  {
                    uint64_t zgaz381;
                    {
                      bool zgaz376;
                      zgaz376 = zcap.zpermit_load;
                      zgaz381 = sailgen_bool_to_bits(zgaz376);
                    }
                    uint64_t zgaz382;
                    {
                      uint64_t zgaz379;
                      {
                        bool zgaz377;
                        zgaz377 = zcap.zpermit_execute;
                        zgaz379 = sailgen_bool_to_bits(zgaz377);
                      }
                      uint64_t zgaz380;
                      {
                        bool zgaz378;
                        zgaz378 = zcap.zglobal;
                        zgaz380 = sailgen_bool_to_bits(zgaz378);
                      }
                      {
                        lbits zgsz3204;
                        CREATE(lbits)(&zgsz3204);
                        CONVERT_OF(lbits, fbits)(&zgsz3204, zgaz379, UINT64_C(1) , true);
                        lbits zgsz3205;
                        CREATE(lbits)(&zgsz3205);
                        CONVERT_OF(lbits, fbits)(&zgsz3205, zgaz380, UINT64_C(1) , true);
                        lbits zgsz3206;
                        CREATE(lbits)(&zgsz3206);
                        append(&zgsz3206, zgsz3204, zgsz3205);
                        zgaz382 = CONVERT_OF(fbits, lbits)(zgsz3206, true);
                        KILL(lbits)(&zgsz3206);
                        KILL(lbits)(&zgsz3205);
                        KILL(lbits)(&zgsz3204);
                      }
                    }
                    {
                      lbits zgsz3207;
                      CREATE(lbits)(&zgsz3207);
                      CONVERT_OF(lbits, fbits)(&zgsz3207, zgaz381, UINT64_C(1) , true);
                      lbits zgsz3208;
                      CREATE(lbits)(&zgsz3208);
                      CONVERT_OF(lbits, fbits)(&zgsz3208, zgaz382, UINT64_C(2) , true);
                      lbits zgsz3209;
                      CREATE(lbits)(&zgsz3209);
                      append(&zgsz3209, zgsz3207, zgsz3208);
                      zgaz384 = CONVERT_OF(fbits, lbits)(zgsz3209, true);
                      KILL(lbits)(&zgsz3209);
                      KILL(lbits)(&zgsz3208);
                      KILL(lbits)(&zgsz3207);
                    }
                  }
                  {
                    lbits zgsz3210;
                    CREATE(lbits)(&zgsz3210);
                    CONVERT_OF(lbits, fbits)(&zgsz3210, zgaz383, UINT64_C(1) , true);
                    lbits zgsz3211;
                    CREATE(lbits)(&zgsz3211);
                    CONVERT_OF(lbits, fbits)(&zgsz3211, zgaz384, UINT64_C(3) , true);
                    lbits zgsz3212;
                    CREATE(lbits)(&zgsz3212);
                    append(&zgsz3212, zgsz3210, zgsz3211);
                    zgaz386 = CONVERT_OF(fbits, lbits)(zgsz3212, true);
                    KILL(lbits)(&zgsz3212);
                    KILL(lbits)(&zgsz3211);
                    KILL(lbits)(&zgsz3210);
                  }
                }
                {
                  lbits zgsz3213;
                  CREATE(lbits)(&zgsz3213);
                  CONVERT_OF(lbits, fbits)(&zgsz3213, zgaz385, UINT64_C(1) , true);
                  lbits zgsz3214;
                  CREATE(lbits)(&zgsz3214);
                  CONVERT_OF(lbits, fbits)(&zgsz3214, zgaz386, UINT64_C(4) , true);
                  lbits zgsz3215;
                  CREATE(lbits)(&zgsz3215);
                  append(&zgsz3215, zgsz3213, zgsz3214);
                  zgaz388 = CONVERT_OF(fbits, lbits)(zgsz3215, true);
                  KILL(lbits)(&zgsz3215);
                  KILL(lbits)(&zgsz3214);
                  KILL(lbits)(&zgsz3213);
                }
              }
              {
                lbits zgsz3216;
                CREATE(lbits)(&zgsz3216);
                CONVERT_OF(lbits, fbits)(&zgsz3216, zgaz387, UINT64_C(1) , true);
                lbits zgsz3217;
                CREATE(lbits)(&zgsz3217);
                CONVERT_OF(lbits, fbits)(&zgsz3217, zgaz388, UINT64_C(5) , true);
                lbits zgsz3218;
                CREATE(lbits)(&zgsz3218);
                append(&zgsz3218, zgsz3216, zgsz3217);
                zgaz390 = CONVERT_OF(fbits, lbits)(zgsz3218, true);
                KILL(lbits)(&zgsz3218);
                KILL(lbits)(&zgsz3217);
                KILL(lbits)(&zgsz3216);
              }
            }
            {
              lbits zgsz3219;
              CREATE(lbits)(&zgsz3219);
              CONVERT_OF(lbits, fbits)(&zgsz3219, zgaz389, UINT64_C(1) , true);
              lbits zgsz3220;
              CREATE(lbits)(&zgsz3220);
              CONVERT_OF(lbits, fbits)(&zgsz3220, zgaz390, UINT64_C(6) , true);
              lbits zgsz3221;
              CREATE(lbits)(&zgsz3221);
              append(&zgsz3221, zgsz3219, zgsz3220);
              zgaz392 = CONVERT_OF(fbits, lbits)(zgsz3221, true);
              KILL(lbits)(&zgsz3221);
              KILL(lbits)(&zgsz3220);
              KILL(lbits)(&zgsz3219);
            }
          }
          {
            lbits zgsz3222;
            CREATE(lbits)(&zgsz3222);
            CONVERT_OF(lbits, fbits)(&zgsz3222, zgaz391, UINT64_C(1) , true);
            lbits zgsz3223;
            CREATE(lbits)(&zgsz3223);
            CONVERT_OF(lbits, fbits)(&zgsz3223, zgaz392, UINT64_C(7) , true);
            lbits zgsz3224;
            CREATE(lbits)(&zgsz3224);
            append(&zgsz3224, zgsz3222, zgsz3223);
            zgaz394 = CONVERT_OF(fbits, lbits)(zgsz3224, true);
            KILL(lbits)(&zgsz3224);
            KILL(lbits)(&zgsz3223);
            KILL(lbits)(&zgsz3222);
          }
        }
        {
          lbits zgsz3225;
          CREATE(lbits)(&zgsz3225);
          CONVERT_OF(lbits, fbits)(&zgsz3225, zgaz393, UINT64_C(1) , true);
          lbits zgsz3226;
          CREATE(lbits)(&zgsz3226);
          CONVERT_OF(lbits, fbits)(&zgsz3226, zgaz394, UINT64_C(8) , true);
          lbits zgsz3227;
          CREATE(lbits)(&zgsz3227);
          append(&zgsz3227, zgsz3225, zgsz3226);
          zgaz396 = CONVERT_OF(fbits, lbits)(zgsz3227, true);
          KILL(lbits)(&zgsz3227);
          KILL(lbits)(&zgsz3226);
          KILL(lbits)(&zgsz3225);
        }
      }
      {
        lbits zgsz3228;
        CREATE(lbits)(&zgsz3228);
        CONVERT_OF(lbits, fbits)(&zgsz3228, zgaz395, UINT64_C(1) , true);
        lbits zgsz3229;
        CREATE(lbits)(&zgsz3229);
        CONVERT_OF(lbits, fbits)(&zgsz3229, zgaz396, UINT64_C(9) , true);
        lbits zgsz3230;
        CREATE(lbits)(&zgsz3230);
        append(&zgsz3230, zgsz3228, zgsz3229);
        zgaz398 = CONVERT_OF(fbits, lbits)(zgsz3230, true);
        KILL(lbits)(&zgsz3230);
        KILL(lbits)(&zgsz3229);
        KILL(lbits)(&zgsz3228);
      }
    }
    {
      lbits zgsz3231;
      CREATE(lbits)(&zgsz3231);
      CONVERT_OF(lbits, fbits)(&zgsz3231, zgaz397, UINT64_C(1) , true);
      lbits zgsz3232;
      CREATE(lbits)(&zgsz3232);
      CONVERT_OF(lbits, fbits)(&zgsz3232, zgaz398, UINT64_C(10) , true);
      lbits zgsz3233;
      CREATE(lbits)(&zgsz3233);
      append(&zgsz3233, zgsz3231, zgsz3232);
      zgaz3100 = CONVERT_OF(fbits, lbits)(zgsz3233, true);
      KILL(lbits)(&zgsz3233);
      KILL(lbits)(&zgsz3232);
      KILL(lbits)(&zgsz3231);
    }
  }
  {
    lbits zgsz3234;
    CREATE(lbits)(&zgsz3234);
    CONVERT_OF(lbits, fbits)(&zgsz3234, zgaz399, UINT64_C(1) , true);
    lbits zgsz3235;
    CREATE(lbits)(&zgsz3235);
    CONVERT_OF(lbits, fbits)(&zgsz3235, zgaz3100, UINT64_C(11) , true);
    lbits zgsz3236;
    CREATE(lbits)(&zgsz3236);
    append(&zgsz3236, zgsz3234, zgsz3235);
    zcbz317 = CONVERT_OF(fbits, lbits)(zgsz3236, true);
    KILL(lbits)(&zgsz3236);
    KILL(lbits)(&zgsz3235);
    KILL(lbits)(&zgsz3234);
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
    fbits zgaz3172;
    zgaz3172 = zc.zinternal_E;
    zinternal_E = sailgen_bit_to_bool(zgaz3172);
  }
  uint64_t zE;
  {
    sail_int zgsz3343;
    CREATE(sail_int)(&zgsz3343);
    CONVERT_OF(sail_int, mach_int)(&zgsz3343, INT64_C(6));
    lbits zgsz3344;
    CREATE(lbits)(&zgsz3344);
    sailgen_zzeros_implicit(&zgsz3344, zgsz3343);
    zE = CONVERT_OF(fbits, lbits)(zgsz3344, true);
    KILL(lbits)(&zgsz3344);
    KILL(sail_int)(&zgsz3343);
  }
  uint64_t zBs;
  {
    sail_int zgsz3341;
    CREATE(sail_int)(&zgsz3341);
    CONVERT_OF(sail_int, mach_int)(&zgsz3341, INT64_C(8));
    lbits zgsz3342;
    CREATE(lbits)(&zgsz3342);
    sailgen_zzeros_implicit(&zgsz3342, zgsz3341);
    zBs = CONVERT_OF(fbits, lbits)(zgsz3342, true);
    KILL(lbits)(&zgsz3342);
    KILL(sail_int)(&zgsz3341);
  }
  uint64_t zT;
  {
    int64_t zgaz3171;
    {
      sail_int zgsz3239;
      CREATE(sail_int)(&zgsz3239);
      CONVERT_OF(sail_int, mach_int)(&zgsz3239, INT64_C(8));
      sail_int zgsz3240;
      CREATE(sail_int)(&zgsz3240);
      CONVERT_OF(sail_int, mach_int)(&zgsz3240, INT64_C(2));
      sail_int zgsz3241;
      CREATE(sail_int)(&zgsz3241);
      sub_int(&zgsz3241, zgsz3239, zgsz3240);
      zgaz3171 = CONVERT_OF(mach_int, sail_int)(zgsz3241);
      KILL(sail_int)(&zgsz3241);
      KILL(sail_int)(&zgsz3240);
      KILL(sail_int)(&zgsz3239);
    }
    {
      sail_int zgsz3237;
      CREATE(sail_int)(&zgsz3237);
      CONVERT_OF(sail_int, mach_int)(&zgsz3237, zgaz3171);
      lbits zgsz3238;
      CREATE(lbits)(&zgsz3238);
      sailgen_zzeros_implicit(&zgsz3238, zgsz3237);
      zT = CONVERT_OF(fbits, lbits)(zgsz3238, true);
      KILL(lbits)(&zgsz3238);
      KILL(sail_int)(&zgsz3237);
    }
  }
  uint64_t zlenMSBs;
  {
    sail_int zgsz3339;
    CREATE(sail_int)(&zgsz3339);
    CONVERT_OF(sail_int, mach_int)(&zgsz3339, INT64_C(2));
    lbits zgsz3340;
    CREATE(lbits)(&zgsz3340);
    sailgen_zzeros_implicit(&zgsz3340, zgsz3339);
    zlenMSBs = CONVERT_OF(fbits, lbits)(zgsz3340, true);
    KILL(lbits)(&zgsz3340);
    KILL(sail_int)(&zgsz3339);
  }
  {
    unit zgsz336;
    if (zinternal_E) {
      {
        uint64_t zgaz3105;
        {
          uint64_t zgaz3101;
          zgaz3101 = zc.zT;
          int64_t zgaz3102;
          {
            sail_int zgsz3246;
            CREATE(sail_int)(&zgsz3246);
            CONVERT_OF(sail_int, mach_int)(&zgsz3246, zinternal_E_take_bits);
            sail_int zgsz3247;
            CREATE(sail_int)(&zgsz3247);
            CONVERT_OF(sail_int, mach_int)(&zgsz3247, INT64_C(1));
            sail_int zgsz3248;
            CREATE(sail_int)(&zgsz3248);
            sub_int(&zgsz3248, zgsz3246, zgsz3247);
            zgaz3102 = CONVERT_OF(mach_int, sail_int)(zgsz3248);
            KILL(sail_int)(&zgsz3248);
            KILL(sail_int)(&zgsz3247);
            KILL(sail_int)(&zgsz3246);
          }
          {
            lbits zgsz3242;
            CREATE(lbits)(&zgsz3242);
            CONVERT_OF(lbits, fbits)(&zgsz3242, zgaz3101, UINT64_C(6) , true);
            sail_int zgsz3243;
            CREATE(sail_int)(&zgsz3243);
            CONVERT_OF(sail_int, mach_int)(&zgsz3243, zgaz3102);
            sail_int zgsz3244;
            CREATE(sail_int)(&zgsz3244);
            CONVERT_OF(sail_int, mach_int)(&zgsz3244, INT64_C(0));
            lbits zgsz3245;
            CREATE(lbits)(&zgsz3245);
            vector_subrange_lbits(&zgsz3245, zgsz3242, zgsz3243, zgsz3244);
            zgaz3105 = CONVERT_OF(fbits, lbits)(zgsz3245, true);
            KILL(lbits)(&zgsz3245);
            KILL(sail_int)(&zgsz3244);
            KILL(sail_int)(&zgsz3243);
            KILL(lbits)(&zgsz3242);
          }
        }
        uint64_t zgaz3106;
        {
          uint64_t zgaz3103;
          zgaz3103 = zc.zB;
          int64_t zgaz3104;
          {
            sail_int zgsz3253;
            CREATE(sail_int)(&zgsz3253);
            CONVERT_OF(sail_int, mach_int)(&zgsz3253, zinternal_E_take_bits);
            sail_int zgsz3254;
            CREATE(sail_int)(&zgsz3254);
            CONVERT_OF(sail_int, mach_int)(&zgsz3254, INT64_C(1));
            sail_int zgsz3255;
            CREATE(sail_int)(&zgsz3255);
            sub_int(&zgsz3255, zgsz3253, zgsz3254);
            zgaz3104 = CONVERT_OF(mach_int, sail_int)(zgsz3255);
            KILL(sail_int)(&zgsz3255);
            KILL(sail_int)(&zgsz3254);
            KILL(sail_int)(&zgsz3253);
          }
          {
            lbits zgsz3249;
            CREATE(lbits)(&zgsz3249);
            CONVERT_OF(lbits, fbits)(&zgsz3249, zgaz3103, UINT64_C(8) , true);
            sail_int zgsz3250;
            CREATE(sail_int)(&zgsz3250);
            CONVERT_OF(sail_int, mach_int)(&zgsz3250, zgaz3104);
            sail_int zgsz3251;
            CREATE(sail_int)(&zgsz3251);
            CONVERT_OF(sail_int, mach_int)(&zgsz3251, INT64_C(0));
            lbits zgsz3252;
            CREATE(lbits)(&zgsz3252);
            vector_subrange_lbits(&zgsz3252, zgsz3249, zgsz3250, zgsz3251);
            zgaz3106 = CONVERT_OF(fbits, lbits)(zgsz3252, true);
            KILL(lbits)(&zgsz3252);
            KILL(sail_int)(&zgsz3251);
            KILL(sail_int)(&zgsz3250);
            KILL(lbits)(&zgsz3249);
          }
        }
        {
          lbits zgsz3256;
          CREATE(lbits)(&zgsz3256);
          CONVERT_OF(lbits, fbits)(&zgsz3256, zgaz3105, UINT64_C(3) , true);
          lbits zgsz3257;
          CREATE(lbits)(&zgsz3257);
          CONVERT_OF(lbits, fbits)(&zgsz3257, zgaz3106, UINT64_C(3) , true);
          lbits zgsz3258;
          CREATE(lbits)(&zgsz3258);
          append(&zgsz3258, zgsz3256, zgsz3257);
          zE = CONVERT_OF(fbits, lbits)(zgsz3258, true);
          KILL(lbits)(&zgsz3258);
          KILL(lbits)(&zgsz3257);
          KILL(lbits)(&zgsz3256);
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
        uint64_t zgaz3109;
        {
          uint64_t zgaz3107;
          zgaz3107 = zc.zT;
          int64_t zgaz3108;
          {
            sail_int zgsz3263;
            CREATE(sail_int)(&zgsz3263);
            CONVERT_OF(sail_int, mach_int)(&zgsz3263, zcap_mantissa_width);
            sail_int zgsz3264;
            CREATE(sail_int)(&zgsz3264);
            CONVERT_OF(sail_int, mach_int)(&zgsz3264, INT64_C(3));
            sail_int zgsz3265;
            CREATE(sail_int)(&zgsz3265);
            sub_int(&zgsz3265, zgsz3263, zgsz3264);
            zgaz3108 = CONVERT_OF(mach_int, sail_int)(zgsz3265);
            KILL(sail_int)(&zgsz3265);
            KILL(sail_int)(&zgsz3264);
            KILL(sail_int)(&zgsz3263);
          }
          {
            lbits zgsz3259;
            CREATE(lbits)(&zgsz3259);
            CONVERT_OF(lbits, fbits)(&zgsz3259, zgaz3107, UINT64_C(6) , true);
            sail_int zgsz3260;
            CREATE(sail_int)(&zgsz3260);
            CONVERT_OF(sail_int, mach_int)(&zgsz3260, zgaz3108);
            sail_int zgsz3261;
            CREATE(sail_int)(&zgsz3261);
            CONVERT_OF(sail_int, mach_int)(&zgsz3261, INT64_C(3));
            lbits zgsz3262;
            CREATE(lbits)(&zgsz3262);
            vector_subrange_lbits(&zgsz3262, zgsz3259, zgsz3260, zgsz3261);
            zgaz3109 = CONVERT_OF(fbits, lbits)(zgsz3262, true);
            KILL(lbits)(&zgsz3262);
            KILL(sail_int)(&zgsz3261);
            KILL(sail_int)(&zgsz3260);
            KILL(lbits)(&zgsz3259);
          }
        }
        uint64_t zgaz3110;
        {
          sail_int zgsz3269;
          CREATE(sail_int)(&zgsz3269);
          CONVERT_OF(sail_int, mach_int)(&zgsz3269, zinternal_E_take_bits);
          lbits zgsz3270;
          CREATE(lbits)(&zgsz3270);
          sailgen_zzeros_implicit(&zgsz3270, zgsz3269);
          zgaz3110 = CONVERT_OF(fbits, lbits)(zgsz3270, true);
          KILL(lbits)(&zgsz3270);
          KILL(sail_int)(&zgsz3269);
        }
        {
          lbits zgsz3266;
          CREATE(lbits)(&zgsz3266);
          CONVERT_OF(lbits, fbits)(&zgsz3266, zgaz3109, UINT64_C(3) , true);
          lbits zgsz3267;
          CREATE(lbits)(&zgsz3267);
          CONVERT_OF(lbits, fbits)(&zgsz3267, zgaz3110, UINT64_C(3) , true);
          lbits zgsz3268;
          CREATE(lbits)(&zgsz3268);
          append(&zgsz3268, zgsz3266, zgsz3267);
          zT = CONVERT_OF(fbits, lbits)(zgsz3268, true);
          KILL(lbits)(&zgsz3268);
          KILL(lbits)(&zgsz3267);
          KILL(lbits)(&zgsz3266);
        }
        unit zgsz339;
        zgsz339 = UNIT;
      }
      uint64_t zgaz3113;
      {
        uint64_t zgaz3111;
        zgaz3111 = zc.zB;
        int64_t zgaz3112;
        {
          sail_int zgsz3275;
          CREATE(sail_int)(&zgsz3275);
          CONVERT_OF(sail_int, mach_int)(&zgsz3275, zcap_mantissa_width);
          sail_int zgsz3276;
          CREATE(sail_int)(&zgsz3276);
          CONVERT_OF(sail_int, mach_int)(&zgsz3276, INT64_C(1));
          sail_int zgsz3277;
          CREATE(sail_int)(&zgsz3277);
          sub_int(&zgsz3277, zgsz3275, zgsz3276);
          zgaz3112 = CONVERT_OF(mach_int, sail_int)(zgsz3277);
          KILL(sail_int)(&zgsz3277);
          KILL(sail_int)(&zgsz3276);
          KILL(sail_int)(&zgsz3275);
        }
        {
          lbits zgsz3271;
          CREATE(lbits)(&zgsz3271);
          CONVERT_OF(lbits, fbits)(&zgsz3271, zgaz3111, UINT64_C(8) , true);
          sail_int zgsz3272;
          CREATE(sail_int)(&zgsz3272);
          CONVERT_OF(sail_int, mach_int)(&zgsz3272, zgaz3112);
          sail_int zgsz3273;
          CREATE(sail_int)(&zgsz3273);
          CONVERT_OF(sail_int, mach_int)(&zgsz3273, INT64_C(3));
          lbits zgsz3274;
          CREATE(lbits)(&zgsz3274);
          vector_subrange_lbits(&zgsz3274, zgsz3271, zgsz3272, zgsz3273);
          zgaz3113 = CONVERT_OF(fbits, lbits)(zgsz3274, true);
          KILL(lbits)(&zgsz3274);
          KILL(sail_int)(&zgsz3273);
          KILL(sail_int)(&zgsz3272);
          KILL(lbits)(&zgsz3271);
        }
      }
      uint64_t zgaz3114;
      {
        sail_int zgsz3281;
        CREATE(sail_int)(&zgsz3281);
        CONVERT_OF(sail_int, mach_int)(&zgsz3281, zinternal_E_take_bits);
        lbits zgsz3282;
        CREATE(lbits)(&zgsz3282);
        sailgen_zzeros_implicit(&zgsz3282, zgsz3281);
        zgaz3114 = CONVERT_OF(fbits, lbits)(zgsz3282, true);
        KILL(lbits)(&zgsz3282);
        KILL(sail_int)(&zgsz3281);
      }
      {
        lbits zgsz3278;
        CREATE(lbits)(&zgsz3278);
        CONVERT_OF(lbits, fbits)(&zgsz3278, zgaz3113, UINT64_C(5) , true);
        lbits zgsz3279;
        CREATE(lbits)(&zgsz3279);
        CONVERT_OF(lbits, fbits)(&zgsz3279, zgaz3114, UINT64_C(3) , true);
        lbits zgsz3280;
        CREATE(lbits)(&zgsz3280);
        append(&zgsz3280, zgsz3278, zgsz3279);
        zBs = CONVERT_OF(fbits, lbits)(zgsz3280, true);
        KILL(lbits)(&zgsz3280);
        KILL(lbits)(&zgsz3279);
        KILL(lbits)(&zgsz3278);
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
    bool zgaz3170;
    {
      uint64_t zgaz3169;
      {
        int64_t zgaz3168;
        {
          sail_int zgsz3287;
          CREATE(sail_int)(&zgsz3287);
          CONVERT_OF(sail_int, mach_int)(&zgsz3287, zcap_mantissa_width);
          sail_int zgsz3288;
          CREATE(sail_int)(&zgsz3288);
          CONVERT_OF(sail_int, mach_int)(&zgsz3288, INT64_C(3));
          sail_int zgsz3289;
          CREATE(sail_int)(&zgsz3289);
          sub_int(&zgsz3289, zgsz3287, zgsz3288);
          zgaz3168 = CONVERT_OF(mach_int, sail_int)(zgsz3289);
          KILL(sail_int)(&zgsz3289);
          KILL(sail_int)(&zgsz3288);
          KILL(sail_int)(&zgsz3287);
        }
        {
          lbits zgsz3283;
          CREATE(lbits)(&zgsz3283);
          CONVERT_OF(lbits, fbits)(&zgsz3283, zBs, UINT64_C(8) , true);
          sail_int zgsz3284;
          CREATE(sail_int)(&zgsz3284);
          CONVERT_OF(sail_int, mach_int)(&zgsz3284, zgaz3168);
          sail_int zgsz3285;
          CREATE(sail_int)(&zgsz3285);
          CONVERT_OF(sail_int, mach_int)(&zgsz3285, INT64_C(0));
          lbits zgsz3286;
          CREATE(lbits)(&zgsz3286);
          vector_subrange_lbits(&zgsz3286, zgsz3283, zgsz3284, zgsz3285);
          zgaz3169 = CONVERT_OF(fbits, lbits)(zgsz3286, true);
          KILL(lbits)(&zgsz3286);
          KILL(sail_int)(&zgsz3285);
          KILL(sail_int)(&zgsz3284);
          KILL(lbits)(&zgsz3283);
        }
      }
      {
        lbits zgsz3290;
        CREATE(lbits)(&zgsz3290);
        CONVERT_OF(lbits, fbits)(&zgsz3290, zT, UINT64_C(6) , true);
        lbits zgsz3291;
        CREATE(lbits)(&zgsz3291);
        CONVERT_OF(lbits, fbits)(&zgsz3291, zgaz3169, UINT64_C(6) , true);
        zgaz3170 = sailgen_z8operatorz0zI_uz9(zgsz3290, zgsz3291);
        KILL(lbits)(&zgsz3291);
        KILL(lbits)(&zgsz3290);
      }
    }
    if (zgaz3170) {  zcarry_out = UINT64_C(0b01);  } else {  zcarry_out = UINT64_C(0b00);  }
  }
  uint64_t zTtop2;
  {
    uint64_t zgaz3167;
    {
      uint64_t zgaz3166;
      {
        int64_t zgaz3164;
        {
          sail_int zgsz3299;
          CREATE(sail_int)(&zgsz3299);
          CONVERT_OF(sail_int, mach_int)(&zgsz3299, zcap_mantissa_width);
          sail_int zgsz3300;
          CREATE(sail_int)(&zgsz3300);
          CONVERT_OF(sail_int, mach_int)(&zgsz3300, INT64_C(1));
          sail_int zgsz3301;
          CREATE(sail_int)(&zgsz3301);
          sub_int(&zgsz3301, zgsz3299, zgsz3300);
          zgaz3164 = CONVERT_OF(mach_int, sail_int)(zgsz3301);
          KILL(sail_int)(&zgsz3301);
          KILL(sail_int)(&zgsz3300);
          KILL(sail_int)(&zgsz3299);
        }
        int64_t zgaz3165;
        {
          sail_int zgsz3296;
          CREATE(sail_int)(&zgsz3296);
          CONVERT_OF(sail_int, mach_int)(&zgsz3296, zcap_mantissa_width);
          sail_int zgsz3297;
          CREATE(sail_int)(&zgsz3297);
          CONVERT_OF(sail_int, mach_int)(&zgsz3297, INT64_C(2));
          sail_int zgsz3298;
          CREATE(sail_int)(&zgsz3298);
          sub_int(&zgsz3298, zgsz3296, zgsz3297);
          zgaz3165 = CONVERT_OF(mach_int, sail_int)(zgsz3298);
          KILL(sail_int)(&zgsz3298);
          KILL(sail_int)(&zgsz3297);
          KILL(sail_int)(&zgsz3296);
        }
        {
          lbits zgsz3292;
          CREATE(lbits)(&zgsz3292);
          CONVERT_OF(lbits, fbits)(&zgsz3292, zBs, UINT64_C(8) , true);
          sail_int zgsz3293;
          CREATE(sail_int)(&zgsz3293);
          CONVERT_OF(sail_int, mach_int)(&zgsz3293, zgaz3164);
          sail_int zgsz3294;
          CREATE(sail_int)(&zgsz3294);
          CONVERT_OF(sail_int, mach_int)(&zgsz3294, zgaz3165);
          lbits zgsz3295;
          CREATE(lbits)(&zgsz3295);
          vector_subrange_lbits(&zgsz3295, zgsz3292, zgsz3293, zgsz3294);
          zgaz3166 = CONVERT_OF(fbits, lbits)(zgsz3295, true);
          KILL(lbits)(&zgsz3295);
          KILL(sail_int)(&zgsz3294);
          KILL(sail_int)(&zgsz3293);
          KILL(lbits)(&zgsz3292);
        }
      }
      {
        lbits zgsz3302;
        CREATE(lbits)(&zgsz3302);
        CONVERT_OF(lbits, fbits)(&zgsz3302, zgaz3166, UINT64_C(2) , true);
        lbits zgsz3303;
        CREATE(lbits)(&zgsz3303);
        CONVERT_OF(lbits, fbits)(&zgsz3303, zlenMSBs, UINT64_C(2) , true);
        lbits zgsz3304;
        CREATE(lbits)(&zgsz3304);
        add_bits(&zgsz3304, zgsz3302, zgsz3303);
        zgaz3167 = CONVERT_OF(fbits, lbits)(zgsz3304, true);
        KILL(lbits)(&zgsz3304);
        KILL(lbits)(&zgsz3303);
        KILL(lbits)(&zgsz3302);
      }
    }
    {
      lbits zgsz3305;
      CREATE(lbits)(&zgsz3305);
      CONVERT_OF(lbits, fbits)(&zgsz3305, zgaz3167, UINT64_C(2) , true);
      lbits zgsz3306;
      CREATE(lbits)(&zgsz3306);
      CONVERT_OF(lbits, fbits)(&zgsz3306, zcarry_out, UINT64_C(2) , true);
      lbits zgsz3307;
      CREATE(lbits)(&zgsz3307);
      add_bits(&zgsz3307, zgsz3305, zgsz3306);
      zTtop2 = CONVERT_OF(fbits, lbits)(zgsz3307, true);
      KILL(lbits)(&zgsz3307);
      KILL(lbits)(&zgsz3306);
      KILL(lbits)(&zgsz3305);
    }
  }
  struct zCapability zgaz3163;
  {
    uint64_t zgaz3119;
    {
      bool zgaz3115;
      {
        sail_int zgsz3308;
        CREATE(sail_int)(&zgsz3308);
        CONVERT_OF(sail_int, mach_int)(&zgsz3308, zcap_uperms_width);
        sail_int zgsz3309;
        CREATE(sail_int)(&zgsz3309);
        CONVERT_OF(sail_int, mach_int)(&zgsz3309, INT64_C(0));
        zgaz3115 = gt(zgsz3308, zgsz3309);
        KILL(sail_int)(&zgsz3309);
        KILL(sail_int)(&zgsz3308);
      }
      zgaz3119 = UINT64_C(0);
    }
    bool zgaz3122;
    {
      fbits zgaz3121;
      {
        uint64_t zgaz3120;
        zgaz3120 = zc.zperms;
        {
          lbits zgsz3310;
          CREATE(lbits)(&zgsz3310);
          CONVERT_OF(lbits, fbits)(&zgsz3310, zgaz3120, UINT64_C(12) , true);
          sail_int zgsz3311;
          CREATE(sail_int)(&zgsz3311);
          CONVERT_OF(sail_int, mach_int)(&zgsz3311, INT64_C(11));
          zgaz3121 = bitvector_access(zgsz3310, zgsz3311);
          KILL(sail_int)(&zgsz3311);
          KILL(lbits)(&zgsz3310);
        }
      }
      zgaz3122 = sailgen_bit_to_bool(zgaz3121);
    }
    bool zgaz3125;
    {
      fbits zgaz3124;
      {
        uint64_t zgaz3123;
        zgaz3123 = zc.zperms;
        {
          lbits zgsz3312;
          CREATE(lbits)(&zgsz3312);
          CONVERT_OF(lbits, fbits)(&zgsz3312, zgaz3123, UINT64_C(12) , true);
          sail_int zgsz3313;
          CREATE(sail_int)(&zgsz3313);
          CONVERT_OF(sail_int, mach_int)(&zgsz3313, INT64_C(10));
          zgaz3124 = bitvector_access(zgsz3312, zgsz3313);
          KILL(sail_int)(&zgsz3313);
          KILL(lbits)(&zgsz3312);
        }
      }
      zgaz3125 = sailgen_bit_to_bool(zgaz3124);
    }
    bool zgaz3128;
    {
      fbits zgaz3127;
      {
        uint64_t zgaz3126;
        zgaz3126 = zc.zperms;
        {
          lbits zgsz3314;
          CREATE(lbits)(&zgsz3314);
          CONVERT_OF(lbits, fbits)(&zgsz3314, zgaz3126, UINT64_C(12) , true);
          sail_int zgsz3315;
          CREATE(sail_int)(&zgsz3315);
          CONVERT_OF(sail_int, mach_int)(&zgsz3315, INT64_C(9));
          zgaz3127 = bitvector_access(zgsz3314, zgsz3315);
          KILL(sail_int)(&zgsz3315);
          KILL(lbits)(&zgsz3314);
        }
      }
      zgaz3128 = sailgen_bit_to_bool(zgaz3127);
    }
    bool zgaz3131;
    {
      fbits zgaz3130;
      {
        uint64_t zgaz3129;
        zgaz3129 = zc.zperms;
        {
          lbits zgsz3316;
          CREATE(lbits)(&zgsz3316);
          CONVERT_OF(lbits, fbits)(&zgsz3316, zgaz3129, UINT64_C(12) , true);
          sail_int zgsz3317;
          CREATE(sail_int)(&zgsz3317);
          CONVERT_OF(sail_int, mach_int)(&zgsz3317, INT64_C(8));
          zgaz3130 = bitvector_access(zgsz3316, zgsz3317);
          KILL(sail_int)(&zgsz3317);
          KILL(lbits)(&zgsz3316);
        }
      }
      zgaz3131 = sailgen_bit_to_bool(zgaz3130);
    }
    bool zgaz3134;
    {
      fbits zgaz3133;
      {
        uint64_t zgaz3132;
        zgaz3132 = zc.zperms;
        {
          lbits zgsz3318;
          CREATE(lbits)(&zgsz3318);
          CONVERT_OF(lbits, fbits)(&zgsz3318, zgaz3132, UINT64_C(12) , true);
          sail_int zgsz3319;
          CREATE(sail_int)(&zgsz3319);
          CONVERT_OF(sail_int, mach_int)(&zgsz3319, INT64_C(7));
          zgaz3133 = bitvector_access(zgsz3318, zgsz3319);
          KILL(sail_int)(&zgsz3319);
          KILL(lbits)(&zgsz3318);
        }
      }
      zgaz3134 = sailgen_bit_to_bool(zgaz3133);
    }
    bool zgaz3137;
    {
      fbits zgaz3136;
      {
        uint64_t zgaz3135;
        zgaz3135 = zc.zperms;
        {
          lbits zgsz3320;
          CREATE(lbits)(&zgsz3320);
          CONVERT_OF(lbits, fbits)(&zgsz3320, zgaz3135, UINT64_C(12) , true);
          sail_int zgsz3321;
          CREATE(sail_int)(&zgsz3321);
          CONVERT_OF(sail_int, mach_int)(&zgsz3321, INT64_C(6));
          zgaz3136 = bitvector_access(zgsz3320, zgsz3321);
          KILL(sail_int)(&zgsz3321);
          KILL(lbits)(&zgsz3320);
        }
      }
      zgaz3137 = sailgen_bit_to_bool(zgaz3136);
    }
    bool zgaz3140;
    {
      fbits zgaz3139;
      {
        uint64_t zgaz3138;
        zgaz3138 = zc.zperms;
        {
          lbits zgsz3322;
          CREATE(lbits)(&zgsz3322);
          CONVERT_OF(lbits, fbits)(&zgsz3322, zgaz3138, UINT64_C(12) , true);
          sail_int zgsz3323;
          CREATE(sail_int)(&zgsz3323);
          CONVERT_OF(sail_int, mach_int)(&zgsz3323, INT64_C(5));
          zgaz3139 = bitvector_access(zgsz3322, zgsz3323);
          KILL(sail_int)(&zgsz3323);
          KILL(lbits)(&zgsz3322);
        }
      }
      zgaz3140 = sailgen_bit_to_bool(zgaz3139);
    }
    bool zgaz3143;
    {
      fbits zgaz3142;
      {
        uint64_t zgaz3141;
        zgaz3141 = zc.zperms;
        {
          lbits zgsz3324;
          CREATE(lbits)(&zgsz3324);
          CONVERT_OF(lbits, fbits)(&zgsz3324, zgaz3141, UINT64_C(12) , true);
          sail_int zgsz3325;
          CREATE(sail_int)(&zgsz3325);
          CONVERT_OF(sail_int, mach_int)(&zgsz3325, INT64_C(4));
          zgaz3142 = bitvector_access(zgsz3324, zgsz3325);
          KILL(sail_int)(&zgsz3325);
          KILL(lbits)(&zgsz3324);
        }
      }
      zgaz3143 = sailgen_bit_to_bool(zgaz3142);
    }
    bool zgaz3146;
    {
      fbits zgaz3145;
      {
        uint64_t zgaz3144;
        zgaz3144 = zc.zperms;
        {
          lbits zgsz3326;
          CREATE(lbits)(&zgsz3326);
          CONVERT_OF(lbits, fbits)(&zgsz3326, zgaz3144, UINT64_C(12) , true);
          sail_int zgsz3327;
          CREATE(sail_int)(&zgsz3327);
          CONVERT_OF(sail_int, mach_int)(&zgsz3327, INT64_C(3));
          zgaz3145 = bitvector_access(zgsz3326, zgsz3327);
          KILL(sail_int)(&zgsz3327);
          KILL(lbits)(&zgsz3326);
        }
      }
      zgaz3146 = sailgen_bit_to_bool(zgaz3145);
    }
    bool zgaz3149;
    {
      fbits zgaz3148;
      {
        uint64_t zgaz3147;
        zgaz3147 = zc.zperms;
        {
          lbits zgsz3328;
          CREATE(lbits)(&zgsz3328);
          CONVERT_OF(lbits, fbits)(&zgsz3328, zgaz3147, UINT64_C(12) , true);
          sail_int zgsz3329;
          CREATE(sail_int)(&zgsz3329);
          CONVERT_OF(sail_int, mach_int)(&zgsz3329, INT64_C(2));
          zgaz3148 = bitvector_access(zgsz3328, zgsz3329);
          KILL(sail_int)(&zgsz3329);
          KILL(lbits)(&zgsz3328);
        }
      }
      zgaz3149 = sailgen_bit_to_bool(zgaz3148);
    }
    bool zgaz3152;
    {
      fbits zgaz3151;
      {
        uint64_t zgaz3150;
        zgaz3150 = zc.zperms;
        {
          lbits zgsz3330;
          CREATE(lbits)(&zgsz3330);
          CONVERT_OF(lbits, fbits)(&zgsz3330, zgaz3150, UINT64_C(12) , true);
          sail_int zgsz3331;
          CREATE(sail_int)(&zgsz3331);
          CONVERT_OF(sail_int, mach_int)(&zgsz3331, INT64_C(1));
          zgaz3151 = bitvector_access(zgsz3330, zgsz3331);
          KILL(sail_int)(&zgsz3331);
          KILL(lbits)(&zgsz3330);
        }
      }
      zgaz3152 = sailgen_bit_to_bool(zgaz3151);
    }
    bool zgaz3155;
    {
      fbits zgaz3154;
      {
        uint64_t zgaz3153;
        zgaz3153 = zc.zperms;
        {
          lbits zgsz3332;
          CREATE(lbits)(&zgsz3332);
          CONVERT_OF(lbits, fbits)(&zgsz3332, zgaz3153, UINT64_C(12) , true);
          sail_int zgsz3333;
          CREATE(sail_int)(&zgsz3333);
          CONVERT_OF(sail_int, mach_int)(&zgsz3333, INT64_C(0));
          zgaz3154 = bitvector_access(zgsz3332, zgsz3333);
          KILL(sail_int)(&zgsz3333);
          KILL(lbits)(&zgsz3332);
        }
      }
      zgaz3155 = sailgen_bit_to_bool(zgaz3154);
    }
    uint64_t zgaz3156;
    zgaz3156 = zc.zreserved;
    bool zgaz3159;
    {
      fbits zgaz3158;
      {
        uint64_t zgaz3157;
        zgaz3157 = zc.zflags;
        {
          lbits zgsz3334;
          CREATE(lbits)(&zgsz3334);
          CONVERT_OF(lbits, fbits)(&zgsz3334, zgaz3157, UINT64_C(1) , true);
          sail_int zgsz3335;
          CREATE(sail_int)(&zgsz3335);
          CONVERT_OF(sail_int, mach_int)(&zgsz3335, INT64_C(0));
          zgaz3158 = bitvector_access(zgsz3334, zgsz3335);
          KILL(sail_int)(&zgsz3335);
          KILL(lbits)(&zgsz3334);
        }
      }
      zgaz3159 = sailgen_bit_to_bool(zgaz3158);
    }
    uint64_t zgaz3160;
    {
      lbits zgsz3336;
      CREATE(lbits)(&zgsz3336);
      CONVERT_OF(lbits, fbits)(&zgsz3336, zTtop2, UINT64_C(2) , true);
      lbits zgsz3337;
      CREATE(lbits)(&zgsz3337);
      CONVERT_OF(lbits, fbits)(&zgsz3337, zT, UINT64_C(6) , true);
      lbits zgsz3338;
      CREATE(lbits)(&zgsz3338);
      append(&zgsz3338, zgsz3336, zgsz3337);
      zgaz3160 = CONVERT_OF(fbits, lbits)(zgsz3338, true);
      KILL(lbits)(&zgsz3338);
      KILL(lbits)(&zgsz3337);
      KILL(lbits)(&zgsz3336);
    }
    uint64_t zgaz3161;
    zgaz3161 = zc.zotype;
    uint64_t zgaz3162;
    zgaz3162 = zc.zaddress;
    struct zCapability zgsz342;
    zgsz342.zB = zBs;
    zgsz342.zE = zE;
    zgsz342.zT = zgaz3160;
    zgsz342.zaccess_system_regs = zgaz3125;
    zgsz342.zaddress = zgaz3162;
    zgsz342.zflag_cap_mode = zgaz3159;
    zgsz342.zglobal = zgaz3155;
    zgsz342.zinternal_E = zinternal_E;
    zgsz342.zotype = zgaz3161;
    zgsz342.zpermit_cinvoke = zgaz3131;
    zgsz342.zpermit_execute = zgaz3152;
    zgsz342.zpermit_load = zgaz3149;
    zgsz342.zpermit_load_cap = zgaz3143;
    zgsz342.zpermit_seal = zgaz3134;
    zgsz342.zpermit_set_CID = zgaz3122;
    zgsz342.zpermit_store = zgaz3146;
    zgsz342.zpermit_store_cap = zgaz3140;
    zgsz342.zpermit_store_local_cap = zgaz3137;
    zgsz342.zpermit_unseal = zgaz3128;
    zgsz342.zreserved = zgaz3156;
    zgsz342.ztag = zt;
    zgsz342.zuperms = zgaz3119;
    zgaz3163 = zgsz342;
  }
  zcbz318 = zgaz3163;
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

static struct zCapability sailgen_capBitsToCapability(bool, uint64_t);

static struct zCapability sailgen_capBitsToCapability(bool zt, uint64_t zc)
{
  __label__ end_function_90, end_block_exception_91;

  struct zCapability zcbz319;
  struct zEncCapability zgaz3173;
  zgaz3173 = sailgen_capBitsToEncCapability(zc);
  zcbz319 = sailgen_encCapabilityToCapability(zt, zgaz3173);

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
    uint64_t zgaz3200;
    zgaz3200 = zcap.zT;
    int64_t zgaz3201;
    {
      sail_int zgsz3349;
      CREATE(sail_int)(&zgsz3349);
      CONVERT_OF(sail_int, mach_int)(&zgsz3349, zcap_mantissa_width);
      sail_int zgsz3350;
      CREATE(sail_int)(&zgsz3350);
      CONVERT_OF(sail_int, mach_int)(&zgsz3350, INT64_C(3));
      sail_int zgsz3351;
      CREATE(sail_int)(&zgsz3351);
      sub_int(&zgsz3351, zgsz3349, zgsz3350);
      zgaz3201 = CONVERT_OF(mach_int, sail_int)(zgsz3351);
      KILL(sail_int)(&zgsz3351);
      KILL(sail_int)(&zgsz3350);
      KILL(sail_int)(&zgsz3349);
    }
    {
      lbits zgsz3345;
      CREATE(lbits)(&zgsz3345);
      CONVERT_OF(lbits, fbits)(&zgsz3345, zgaz3200, UINT64_C(8) , true);
      sail_int zgsz3346;
      CREATE(sail_int)(&zgsz3346);
      CONVERT_OF(sail_int, mach_int)(&zgsz3346, zgaz3201);
      sail_int zgsz3347;
      CREATE(sail_int)(&zgsz3347);
      CONVERT_OF(sail_int, mach_int)(&zgsz3347, zinternal_E_take_bits);
      lbits zgsz3348;
      CREATE(lbits)(&zgsz3348);
      vector_subrange_lbits(&zgsz3348, zgsz3345, zgsz3346, zgsz3347);
      zt_hi = CONVERT_OF(fbits, lbits)(zgsz3348, true);
      KILL(lbits)(&zgsz3348);
      KILL(sail_int)(&zgsz3347);
      KILL(sail_int)(&zgsz3346);
      KILL(lbits)(&zgsz3345);
    }
  }
  uint64_t zt_lo;
  {
    uint64_t zgaz3198;
    zgaz3198 = zcap.zT;
    int64_t zgaz3199;
    {
      sail_int zgsz3356;
      CREATE(sail_int)(&zgsz3356);
      CONVERT_OF(sail_int, mach_int)(&zgsz3356, zinternal_E_take_bits);
      sail_int zgsz3357;
      CREATE(sail_int)(&zgsz3357);
      CONVERT_OF(sail_int, mach_int)(&zgsz3357, INT64_C(1));
      sail_int zgsz3358;
      CREATE(sail_int)(&zgsz3358);
      sub_int(&zgsz3358, zgsz3356, zgsz3357);
      zgaz3199 = CONVERT_OF(mach_int, sail_int)(zgsz3358);
      KILL(sail_int)(&zgsz3358);
      KILL(sail_int)(&zgsz3357);
      KILL(sail_int)(&zgsz3356);
    }
    {
      lbits zgsz3352;
      CREATE(lbits)(&zgsz3352);
      CONVERT_OF(lbits, fbits)(&zgsz3352, zgaz3198, UINT64_C(8) , true);
      sail_int zgsz3353;
      CREATE(sail_int)(&zgsz3353);
      CONVERT_OF(sail_int, mach_int)(&zgsz3353, zgaz3199);
      sail_int zgsz3354;
      CREATE(sail_int)(&zgsz3354);
      CONVERT_OF(sail_int, mach_int)(&zgsz3354, INT64_C(0));
      lbits zgsz3355;
      CREATE(lbits)(&zgsz3355);
      vector_subrange_lbits(&zgsz3355, zgsz3352, zgsz3353, zgsz3354);
      zt_lo = CONVERT_OF(fbits, lbits)(zgsz3355, true);
      KILL(lbits)(&zgsz3355);
      KILL(sail_int)(&zgsz3354);
      KILL(sail_int)(&zgsz3353);
      KILL(lbits)(&zgsz3352);
    }
  }
  uint64_t zb_hi;
  {
    uint64_t zgaz3196;
    zgaz3196 = zcap.zB;
    int64_t zgaz3197;
    {
      sail_int zgsz3363;
      CREATE(sail_int)(&zgsz3363);
      CONVERT_OF(sail_int, mach_int)(&zgsz3363, zcap_mantissa_width);
      sail_int zgsz3364;
      CREATE(sail_int)(&zgsz3364);
      CONVERT_OF(sail_int, mach_int)(&zgsz3364, INT64_C(1));
      sail_int zgsz3365;
      CREATE(sail_int)(&zgsz3365);
      sub_int(&zgsz3365, zgsz3363, zgsz3364);
      zgaz3197 = CONVERT_OF(mach_int, sail_int)(zgsz3365);
      KILL(sail_int)(&zgsz3365);
      KILL(sail_int)(&zgsz3364);
      KILL(sail_int)(&zgsz3363);
    }
    {
      lbits zgsz3359;
      CREATE(lbits)(&zgsz3359);
      CONVERT_OF(lbits, fbits)(&zgsz3359, zgaz3196, UINT64_C(8) , true);
      sail_int zgsz3360;
      CREATE(sail_int)(&zgsz3360);
      CONVERT_OF(sail_int, mach_int)(&zgsz3360, zgaz3197);
      sail_int zgsz3361;
      CREATE(sail_int)(&zgsz3361);
      CONVERT_OF(sail_int, mach_int)(&zgsz3361, zinternal_E_take_bits);
      lbits zgsz3362;
      CREATE(lbits)(&zgsz3362);
      vector_subrange_lbits(&zgsz3362, zgsz3359, zgsz3360, zgsz3361);
      zb_hi = CONVERT_OF(fbits, lbits)(zgsz3362, true);
      KILL(lbits)(&zgsz3362);
      KILL(sail_int)(&zgsz3361);
      KILL(sail_int)(&zgsz3360);
      KILL(lbits)(&zgsz3359);
    }
  }
  uint64_t zb_lo;
  {
    uint64_t zgaz3194;
    zgaz3194 = zcap.zB;
    int64_t zgaz3195;
    {
      sail_int zgsz3370;
      CREATE(sail_int)(&zgsz3370);
      CONVERT_OF(sail_int, mach_int)(&zgsz3370, zinternal_E_take_bits);
      sail_int zgsz3371;
      CREATE(sail_int)(&zgsz3371);
      CONVERT_OF(sail_int, mach_int)(&zgsz3371, INT64_C(1));
      sail_int zgsz3372;
      CREATE(sail_int)(&zgsz3372);
      sub_int(&zgsz3372, zgsz3370, zgsz3371);
      zgaz3195 = CONVERT_OF(mach_int, sail_int)(zgsz3372);
      KILL(sail_int)(&zgsz3372);
      KILL(sail_int)(&zgsz3371);
      KILL(sail_int)(&zgsz3370);
    }
    {
      lbits zgsz3366;
      CREATE(lbits)(&zgsz3366);
      CONVERT_OF(lbits, fbits)(&zgsz3366, zgaz3194, UINT64_C(8) , true);
      sail_int zgsz3367;
      CREATE(sail_int)(&zgsz3367);
      CONVERT_OF(sail_int, mach_int)(&zgsz3367, zgaz3195);
      sail_int zgsz3368;
      CREATE(sail_int)(&zgsz3368);
      CONVERT_OF(sail_int, mach_int)(&zgsz3368, INT64_C(0));
      lbits zgsz3369;
      CREATE(lbits)(&zgsz3369);
      vector_subrange_lbits(&zgsz3369, zgsz3366, zgsz3367, zgsz3368);
      zb_lo = CONVERT_OF(fbits, lbits)(zgsz3369, true);
      KILL(lbits)(&zgsz3369);
      KILL(sail_int)(&zgsz3368);
      KILL(sail_int)(&zgsz3367);
      KILL(lbits)(&zgsz3366);
    }
  }
  {
    bool zgaz3174;
    zgaz3174 = zcap.zinternal_E;
    unit zgsz343;
    if (zgaz3174) {
      {
        uint64_t zgaz3176;
        zgaz3176 = zcap.zE;
        int64_t zgaz3177;
        {
          int64_t zgaz3175;
          {
            sail_int zgsz3376;
            CREATE(sail_int)(&zgsz3376);
            CONVERT_OF(sail_int, mach_int)(&zgsz3376, INT64_C(2));
            sail_int zgsz3377;
            CREATE(sail_int)(&zgsz3377);
            CONVERT_OF(sail_int, mach_int)(&zgsz3377, zinternal_E_take_bits);
            sail_int zgsz3378;
            CREATE(sail_int)(&zgsz3378);
            mult_int(&zgsz3378, zgsz3376, zgsz3377);
            zgaz3175 = CONVERT_OF(mach_int, sail_int)(zgsz3378);
            KILL(sail_int)(&zgsz3378);
            KILL(sail_int)(&zgsz3377);
            KILL(sail_int)(&zgsz3376);
          }
          {
            sail_int zgsz3373;
            CREATE(sail_int)(&zgsz3373);
            CONVERT_OF(sail_int, mach_int)(&zgsz3373, zgaz3175);
            sail_int zgsz3374;
            CREATE(sail_int)(&zgsz3374);
            CONVERT_OF(sail_int, mach_int)(&zgsz3374, INT64_C(1));
            sail_int zgsz3375;
            CREATE(sail_int)(&zgsz3375);
            sub_int(&zgsz3375, zgsz3373, zgsz3374);
            zgaz3177 = CONVERT_OF(mach_int, sail_int)(zgsz3375);
            KILL(sail_int)(&zgsz3375);
            KILL(sail_int)(&zgsz3374);
            KILL(sail_int)(&zgsz3373);
          }
        }
        {
          lbits zgsz3379;
          CREATE(lbits)(&zgsz3379);
          CONVERT_OF(lbits, fbits)(&zgsz3379, zgaz3176, UINT64_C(6) , true);
          sail_int zgsz3380;
          CREATE(sail_int)(&zgsz3380);
          CONVERT_OF(sail_int, mach_int)(&zgsz3380, zgaz3177);
          sail_int zgsz3381;
          CREATE(sail_int)(&zgsz3381);
          CONVERT_OF(sail_int, mach_int)(&zgsz3381, zinternal_E_take_bits);
          lbits zgsz3382;
          CREATE(lbits)(&zgsz3382);
          vector_subrange_lbits(&zgsz3382, zgsz3379, zgsz3380, zgsz3381);
          zt_lo = CONVERT_OF(fbits, lbits)(zgsz3382, true);
          KILL(lbits)(&zgsz3382);
          KILL(sail_int)(&zgsz3381);
          KILL(sail_int)(&zgsz3380);
          KILL(lbits)(&zgsz3379);
        }
        unit zgsz344;
        zgsz344 = UNIT;
      }
      uint64_t zgaz3178;
      zgaz3178 = zcap.zE;
      int64_t zgaz3179;
      {
        sail_int zgsz3387;
        CREATE(sail_int)(&zgsz3387);
        CONVERT_OF(sail_int, mach_int)(&zgsz3387, zinternal_E_take_bits);
        sail_int zgsz3388;
        CREATE(sail_int)(&zgsz3388);
        CONVERT_OF(sail_int, mach_int)(&zgsz3388, INT64_C(1));
        sail_int zgsz3389;
        CREATE(sail_int)(&zgsz3389);
        sub_int(&zgsz3389, zgsz3387, zgsz3388);
        zgaz3179 = CONVERT_OF(mach_int, sail_int)(zgsz3389);
        KILL(sail_int)(&zgsz3389);
        KILL(sail_int)(&zgsz3388);
        KILL(sail_int)(&zgsz3387);
      }
      {
        lbits zgsz3383;
        CREATE(lbits)(&zgsz3383);
        CONVERT_OF(lbits, fbits)(&zgsz3383, zgaz3178, UINT64_C(6) , true);
        sail_int zgsz3384;
        CREATE(sail_int)(&zgsz3384);
        CONVERT_OF(sail_int, mach_int)(&zgsz3384, zgaz3179);
        sail_int zgsz3385;
        CREATE(sail_int)(&zgsz3385);
        CONVERT_OF(sail_int, mach_int)(&zgsz3385, INT64_C(0));
        lbits zgsz3386;
        CREATE(lbits)(&zgsz3386);
        vector_subrange_lbits(&zgsz3386, zgsz3383, zgsz3384, zgsz3385);
        zb_lo = CONVERT_OF(fbits, lbits)(zgsz3386, true);
        KILL(lbits)(&zgsz3386);
        KILL(sail_int)(&zgsz3385);
        KILL(sail_int)(&zgsz3384);
        KILL(lbits)(&zgsz3383);
      }
      zgsz343 = UNIT;
    } else {  zgsz343 = UNIT;  }
  }
  struct zEncCapability zgaz3193;
  {
    uint64_t zgaz3182;
    {
      uint64_t zgaz3180;
      zgaz3180 = zcap.zuperms;
      uint64_t zgaz3181;
      zgaz3181 = sailgen_getCapHardPerms(zcap);
      {
        lbits zgsz3390;
        CREATE(lbits)(&zgsz3390);
        CONVERT_OF(lbits, fbits)(&zgsz3390, zgaz3180, UINT64_C(0) , true);
        lbits zgsz3391;
        CREATE(lbits)(&zgsz3391);
        CONVERT_OF(lbits, fbits)(&zgsz3391, zgaz3181, UINT64_C(12) , true);
        lbits zgsz3392;
        CREATE(lbits)(&zgsz3392);
        append(&zgsz3392, zgsz3390, zgsz3391);
        zgaz3182 = CONVERT_OF(fbits, lbits)(zgsz3392, true);
        KILL(lbits)(&zgsz3392);
        KILL(lbits)(&zgsz3391);
        KILL(lbits)(&zgsz3390);
      }
    }
    uint64_t zgaz3183;
    zgaz3183 = zcap.zreserved;
    uint64_t zgaz3185;
    {
      bool zgaz3184;
      zgaz3184 = zcap.zflag_cap_mode;
      zgaz3185 = sailgen_bool_to_bits(zgaz3184);
    }
    uint64_t zgaz3186;
    zgaz3186 = zcap.zotype;
    fbits zgaz3189;
    {
      uint64_t zgaz3188;
      {
        bool zgaz3187;
        zgaz3187 = zcap.zinternal_E;
        zgaz3188 = sailgen_bool_to_bits(zgaz3187);
      }
      {
        lbits zgsz3393;
        CREATE(lbits)(&zgsz3393);
        CONVERT_OF(lbits, fbits)(&zgsz3393, zgaz3188, UINT64_C(1) , true);
        sail_int zgsz3394;
        CREATE(sail_int)(&zgsz3394);
        CONVERT_OF(sail_int, mach_int)(&zgsz3394, INT64_C(0));
        zgaz3189 = bitvector_access(zgsz3393, zgsz3394);
        KILL(sail_int)(&zgsz3394);
        KILL(lbits)(&zgsz3393);
      }
    }
    uint64_t zgaz3190;
    {
      lbits zgsz3398;
      CREATE(lbits)(&zgsz3398);
      CONVERT_OF(lbits, fbits)(&zgsz3398, zt_hi, UINT64_C(3) , true);
      lbits zgsz3399;
      CREATE(lbits)(&zgsz3399);
      CONVERT_OF(lbits, fbits)(&zgsz3399, zt_lo, UINT64_C(3) , true);
      lbits zgsz3400;
      CREATE(lbits)(&zgsz3400);
      append(&zgsz3400, zgsz3398, zgsz3399);
      zgaz3190 = CONVERT_OF(fbits, lbits)(zgsz3400, true);
      KILL(lbits)(&zgsz3400);
      KILL(lbits)(&zgsz3399);
      KILL(lbits)(&zgsz3398);
    }
    uint64_t zgaz3191;
    {
      lbits zgsz3395;
      CREATE(lbits)(&zgsz3395);
      CONVERT_OF(lbits, fbits)(&zgsz3395, zb_hi, UINT64_C(5) , true);
      lbits zgsz3396;
      CREATE(lbits)(&zgsz3396);
      CONVERT_OF(lbits, fbits)(&zgsz3396, zb_lo, UINT64_C(3) , true);
      lbits zgsz3397;
      CREATE(lbits)(&zgsz3397);
      append(&zgsz3397, zgsz3395, zgsz3396);
      zgaz3191 = CONVERT_OF(fbits, lbits)(zgsz3397, true);
      KILL(lbits)(&zgsz3397);
      KILL(lbits)(&zgsz3396);
      KILL(lbits)(&zgsz3395);
    }
    uint64_t zgaz3192;
    zgaz3192 = zcap.zaddress;
    struct zEncCapability zgsz345;
    zgsz345.zB = zgaz3191;
    zgsz345.zT = zgaz3190;
    zgsz345.zaddress = zgaz3192;
    zgsz345.zflags = zgaz3185;
    zgsz345.zinternal_E = zgaz3189;
    zgsz345.zotype = zgaz3186;
    zgsz345.zperms = zgaz3182;
    zgsz345.zreserved = zgaz3183;
    zgaz3193 = zgsz345;
  }
  zcbz320 = zgaz3193;
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

static uint64_t sailgen_capToBits(struct zCapability);

static uint64_t sailgen_capToBits(struct zCapability zcap)
{
  __label__ end_function_98, end_block_exception_99;

  uint64_t zcbz321;
  struct zEncCapability zgaz3202;
  zgaz3202 = sailgen_capToEncCap(zcap);
  zcbz321 = sailgen_encCapToBits(zgaz3202);

end_function_98: ;
  return zcbz321;
end_block_exception_99: ;

  return UINT64_C(0xdeadc0de);
}

static uint64_t znull_cap_bits;
static void create_letbind_27(void) {


  uint64_t zgsz346;
  zgsz346 = sailgen_capToBits(znull_cap);
  znull_cap_bits = zgsz346;

let_end_100: ;
}
static void kill_letbind_27(void) {
}

static uint64_t sailgen_capToMemBits(struct zCapability);

static uint64_t sailgen_capToMemBits(struct zCapability zcap)
{
  __label__ end_function_102, end_block_exception_103;

  uint64_t zcbz322;
  uint64_t zgaz3203;
  zgaz3203 = sailgen_capToBits(zcap);
  {
    lbits zgsz3401;
    CREATE(lbits)(&zgsz3401);
    CONVERT_OF(lbits, fbits)(&zgsz3401, zgaz3203, UINT64_C(64) , true);
    lbits zgsz3402;
    CREATE(lbits)(&zgsz3402);
    CONVERT_OF(lbits, fbits)(&zgsz3402, znull_cap_bits, UINT64_C(64) , true);
    lbits zgsz3403;
    CREATE(lbits)(&zgsz3403);
    xor_bits(&zgsz3403, zgsz3401, zgsz3402);
    zcbz322 = CONVERT_OF(fbits, lbits)(zgsz3403, true);
    KILL(lbits)(&zgsz3403);
    KILL(lbits)(&zgsz3402);
    KILL(lbits)(&zgsz3401);
  }

end_function_102: ;
  return zcbz322;
end_block_exception_103: ;

  return UINT64_C(0xdeadc0de);
}

static struct zCapability sailgen_memBitsToCapability(bool, uint64_t);

static struct zCapability sailgen_memBitsToCapability(bool ztag, uint64_t zb)
{
  __label__ end_function_105, end_block_exception_106;

  struct zCapability zcbz323;
  uint64_t zgaz3204;
  {
    lbits zgsz3404;
    CREATE(lbits)(&zgsz3404);
    CONVERT_OF(lbits, fbits)(&zgsz3404, zb, UINT64_C(64) , true);
    lbits zgsz3405;
    CREATE(lbits)(&zgsz3405);
    CONVERT_OF(lbits, fbits)(&zgsz3405, znull_cap_bits, UINT64_C(64) , true);
    lbits zgsz3406;
    CREATE(lbits)(&zgsz3406);
    xor_bits(&zgsz3406, zgsz3404, zgsz3405);
    zgaz3204 = CONVERT_OF(fbits, lbits)(zgsz3406, true);
    KILL(lbits)(&zgsz3406);
    KILL(lbits)(&zgsz3405);
    KILL(lbits)(&zgsz3404);
  }
  zcbz323 = sailgen_capBitsToCapability(ztag, zgaz3204);

end_function_105: ;
  return zcbz323;
end_block_exception_106: ;
  struct zCapability zcbz346 = { .zuperms = UINT64_C(0xdeadc0de), .ztag = false, .zreserved = UINT64_C(0xdeadc0de), .zpermit_unseal = false, .zpermit_store_local_cap = false, .zpermit_store_cap = false, .zpermit_store = false, .zpermit_set_CID = false, .zpermit_seal = false, .zpermit_load_cap = false, .zpermit_load = false, .zpermit_execute = false, .zpermit_cinvoke = false, .zotype = UINT64_C(0xdeadc0de), .zinternal_E = false, .zglobal = false, .zflag_cap_mode = false, .zaddress = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zT = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  return zcbz346;
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
  __label__ end_function_108, end_block_exception_109;

  struct ztuple_z8z5bv32zCz0z5bv33z9 zcbz324;
  int64_t zE;
  {
    int64_t zgaz3238;
    {
      uint64_t zgaz3237;
      zgaz3237 = zc.zE;
      {
        lbits zgsz3407;
        CREATE(lbits)(&zgsz3407);
        CONVERT_OF(lbits, fbits)(&zgsz3407, zgaz3237, UINT64_C(6) , true);
        sail_int zgsz3408;
        CREATE(sail_int)(&zgsz3408);
        sail_unsigned(&zgsz3408, zgsz3407);
        zgaz3238 = CONVERT_OF(mach_int, sail_int)(zgsz3408);
        KILL(sail_int)(&zgsz3408);
        KILL(lbits)(&zgsz3407);
      }
    }
    {
      sail_int zgsz3409;
      CREATE(sail_int)(&zgsz3409);
      CONVERT_OF(sail_int, mach_int)(&zgsz3409, zcap_max_E);
      sail_int zgsz3410;
      CREATE(sail_int)(&zgsz3410);
      CONVERT_OF(sail_int, mach_int)(&zgsz3410, zgaz3238);
      sail_int zgsz3411;
      CREATE(sail_int)(&zgsz3411);
      min_int(&zgsz3411, zgsz3409, zgsz3410);
      zE = CONVERT_OF(mach_int, sail_int)(zgsz3411);
      KILL(sail_int)(&zgsz3411);
      KILL(sail_int)(&zgsz3410);
      KILL(sail_int)(&zgsz3409);
    }
  }
  uint64_t za;
  za = zc.zaddress;
  uint64_t za3;
  {
    uint64_t zgaz3236;
    {
      int64_t zgaz3235;
      {
        int64_t zgaz3234;
        {
          sail_int zgsz3415;
          CREATE(sail_int)(&zgsz3415);
          CONVERT_OF(sail_int, mach_int)(&zgsz3415, zE);
          sail_int zgsz3416;
          CREATE(sail_int)(&zgsz3416);
          CONVERT_OF(sail_int, mach_int)(&zgsz3416, zcap_mantissa_width);
          sail_int zgsz3417;
          CREATE(sail_int)(&zgsz3417);
          add_int(&zgsz3417, zgsz3415, zgsz3416);
          zgaz3234 = CONVERT_OF(mach_int, sail_int)(zgsz3417);
          KILL(sail_int)(&zgsz3417);
          KILL(sail_int)(&zgsz3416);
          KILL(sail_int)(&zgsz3415);
        }
        {
          sail_int zgsz3412;
          CREATE(sail_int)(&zgsz3412);
          CONVERT_OF(sail_int, mach_int)(&zgsz3412, zgaz3234);
          sail_int zgsz3413;
          CREATE(sail_int)(&zgsz3413);
          CONVERT_OF(sail_int, mach_int)(&zgsz3413, INT64_C(3));
          sail_int zgsz3414;
          CREATE(sail_int)(&zgsz3414);
          sub_int(&zgsz3414, zgsz3412, zgsz3413);
          zgaz3235 = CONVERT_OF(mach_int, sail_int)(zgsz3414);
          KILL(sail_int)(&zgsz3414);
          KILL(sail_int)(&zgsz3413);
          KILL(sail_int)(&zgsz3412);
        }
      }
      {
        lbits zgsz3418;
        CREATE(lbits)(&zgsz3418);
        CONVERT_OF(lbits, fbits)(&zgsz3418, za, UINT64_C(32) , true);
        sail_int zgsz3419;
        CREATE(sail_int)(&zgsz3419);
        CONVERT_OF(sail_int, mach_int)(&zgsz3419, zgaz3235);
        lbits zgsz3420;
        CREATE(lbits)(&zgsz3420);
        shiftr(&zgsz3420, zgsz3418, zgsz3419);
        zgaz3236 = CONVERT_OF(fbits, lbits)(zgsz3420, true);
        KILL(lbits)(&zgsz3420);
        KILL(sail_int)(&zgsz3419);
        KILL(lbits)(&zgsz3418);
      }
    }
    {
      lbits zgsz3421;
      CREATE(lbits)(&zgsz3421);
      CONVERT_OF(lbits, fbits)(&zgsz3421, zgaz3236, UINT64_C(32) , true);
      sail_int zgsz3422;
      CREATE(sail_int)(&zgsz3422);
      CONVERT_OF(sail_int, mach_int)(&zgsz3422, INT64_C(3));
      lbits zgsz3423;
      CREATE(lbits)(&zgsz3423);
      sail_truncate(&zgsz3423, zgsz3421, zgsz3422);
      za3 = CONVERT_OF(fbits, lbits)(zgsz3423, true);
      KILL(lbits)(&zgsz3423);
      KILL(sail_int)(&zgsz3422);
      KILL(lbits)(&zgsz3421);
    }
  }
  uint64_t zB3;
  {
    uint64_t zgaz3233;
    zgaz3233 = zc.zB;
    {
      lbits zgsz3424;
      CREATE(lbits)(&zgsz3424);
      CONVERT_OF(lbits, fbits)(&zgsz3424, zgaz3233, UINT64_C(8) , true);
      sail_int zgsz3425;
      CREATE(sail_int)(&zgsz3425);
      CONVERT_OF(sail_int, mach_int)(&zgsz3425, INT64_C(3));
      lbits zgsz3426;
      CREATE(lbits)(&zgsz3426);
      sail_truncateLSB(&zgsz3426, zgsz3424, zgsz3425);
      zB3 = CONVERT_OF(fbits, lbits)(zgsz3426, true);
      KILL(lbits)(&zgsz3426);
      KILL(sail_int)(&zgsz3425);
      KILL(lbits)(&zgsz3424);
    }
  }
  uint64_t zT3;
  {
    uint64_t zgaz3232;
    zgaz3232 = zc.zT;
    {
      lbits zgsz3427;
      CREATE(lbits)(&zgsz3427);
      CONVERT_OF(lbits, fbits)(&zgsz3427, zgaz3232, UINT64_C(8) , true);
      sail_int zgsz3428;
      CREATE(sail_int)(&zgsz3428);
      CONVERT_OF(sail_int, mach_int)(&zgsz3428, INT64_C(3));
      lbits zgsz3429;
      CREATE(lbits)(&zgsz3429);
      sail_truncateLSB(&zgsz3429, zgsz3427, zgsz3428);
      zT3 = CONVERT_OF(fbits, lbits)(zgsz3429, true);
      KILL(lbits)(&zgsz3429);
      KILL(sail_int)(&zgsz3428);
      KILL(lbits)(&zgsz3427);
    }
  }
  uint64_t zR3;
  {
    lbits zgsz3511;
    CREATE(lbits)(&zgsz3511);
    CONVERT_OF(lbits, fbits)(&zgsz3511, zB3, UINT64_C(3) , true);
    lbits zgsz3512;
    CREATE(lbits)(&zgsz3512);
    CONVERT_OF(lbits, fbits)(&zgsz3512, UINT64_C(0b001), UINT64_C(3) , true);
    lbits zgsz3513;
    CREATE(lbits)(&zgsz3513);
    sub_bits(&zgsz3513, zgsz3511, zgsz3512);
    zR3 = CONVERT_OF(fbits, lbits)(zgsz3513, true);
    KILL(lbits)(&zgsz3513);
    KILL(lbits)(&zgsz3512);
    KILL(lbits)(&zgsz3511);
  }
  int64_t zaHi;
  {
    bool zgaz3231;
    {
      lbits zgsz3430;
      CREATE(lbits)(&zgsz3430);
      CONVERT_OF(lbits, fbits)(&zgsz3430, za3, UINT64_C(3) , true);
      lbits zgsz3431;
      CREATE(lbits)(&zgsz3431);
      CONVERT_OF(lbits, fbits)(&zgsz3431, zR3, UINT64_C(3) , true);
      zgaz3231 = sailgen_z8operatorz0zI_uz9(zgsz3430, zgsz3431);
      KILL(lbits)(&zgsz3431);
      KILL(lbits)(&zgsz3430);
    }
    if (zgaz3231) {  zaHi = INT64_C(1);  } else {  zaHi = INT64_C(0);  }
  }
  int64_t zbHi;
  {
    bool zgaz3230;
    {
      lbits zgsz3432;
      CREATE(lbits)(&zgsz3432);
      CONVERT_OF(lbits, fbits)(&zgsz3432, zB3, UINT64_C(3) , true);
      lbits zgsz3433;
      CREATE(lbits)(&zgsz3433);
      CONVERT_OF(lbits, fbits)(&zgsz3433, zR3, UINT64_C(3) , true);
      zgaz3230 = sailgen_z8operatorz0zI_uz9(zgsz3432, zgsz3433);
      KILL(lbits)(&zgsz3433);
      KILL(lbits)(&zgsz3432);
    }
    if (zgaz3230) {  zbHi = INT64_C(1);  } else {  zbHi = INT64_C(0);  }
  }
  int64_t ztHi;
  {
    bool zgaz3229;
    {
      lbits zgsz3434;
      CREATE(lbits)(&zgsz3434);
      CONVERT_OF(lbits, fbits)(&zgsz3434, zT3, UINT64_C(3) , true);
      lbits zgsz3435;
      CREATE(lbits)(&zgsz3435);
      CONVERT_OF(lbits, fbits)(&zgsz3435, zR3, UINT64_C(3) , true);
      zgaz3229 = sailgen_z8operatorz0zI_uz9(zgsz3434, zgsz3435);
      KILL(lbits)(&zgsz3435);
      KILL(lbits)(&zgsz3434);
    }
    if (zgaz3229) {  ztHi = INT64_C(1);  } else {  ztHi = INT64_C(0);  }
  }
  int64_t zcorrection_base;
  {
    sail_int zgsz3508;
    CREATE(sail_int)(&zgsz3508);
    CONVERT_OF(sail_int, mach_int)(&zgsz3508, zbHi);
    sail_int zgsz3509;
    CREATE(sail_int)(&zgsz3509);
    CONVERT_OF(sail_int, mach_int)(&zgsz3509, zaHi);
    sail_int zgsz3510;
    CREATE(sail_int)(&zgsz3510);
    sub_int(&zgsz3510, zgsz3508, zgsz3509);
    zcorrection_base = CONVERT_OF(mach_int, sail_int)(zgsz3510);
    KILL(sail_int)(&zgsz3510);
    KILL(sail_int)(&zgsz3509);
    KILL(sail_int)(&zgsz3508);
  }
  int64_t zcorrection_top;
  {
    sail_int zgsz3505;
    CREATE(sail_int)(&zgsz3505);
    CONVERT_OF(sail_int, mach_int)(&zgsz3505, ztHi);
    sail_int zgsz3506;
    CREATE(sail_int)(&zgsz3506);
    CONVERT_OF(sail_int, mach_int)(&zgsz3506, zaHi);
    sail_int zgsz3507;
    CREATE(sail_int)(&zgsz3507);
    sub_int(&zgsz3507, zgsz3505, zgsz3506);
    zcorrection_top = CONVERT_OF(mach_int, sail_int)(zgsz3507);
    KILL(sail_int)(&zgsz3507);
    KILL(sail_int)(&zgsz3506);
    KILL(sail_int)(&zgsz3505);
  }
  uint64_t za_top;
  {
    int64_t zgaz3228;
    {
      sail_int zgsz3439;
      CREATE(sail_int)(&zgsz3439);
      CONVERT_OF(sail_int, mach_int)(&zgsz3439, zE);
      sail_int zgsz3440;
      CREATE(sail_int)(&zgsz3440);
      CONVERT_OF(sail_int, mach_int)(&zgsz3440, zcap_mantissa_width);
      sail_int zgsz3441;
      CREATE(sail_int)(&zgsz3441);
      add_int(&zgsz3441, zgsz3439, zgsz3440);
      zgaz3228 = CONVERT_OF(mach_int, sail_int)(zgsz3441);
      KILL(sail_int)(&zgsz3441);
      KILL(sail_int)(&zgsz3440);
      KILL(sail_int)(&zgsz3439);
    }
    {
      lbits zgsz3436;
      CREATE(lbits)(&zgsz3436);
      CONVERT_OF(lbits, fbits)(&zgsz3436, za, UINT64_C(32) , true);
      sail_int zgsz3437;
      CREATE(sail_int)(&zgsz3437);
      CONVERT_OF(sail_int, mach_int)(&zgsz3437, zgaz3228);
      lbits zgsz3438;
      CREATE(lbits)(&zgsz3438);
      shiftr(&zgsz3438, zgsz3436, zgsz3437);
      za_top = CONVERT_OF(fbits, lbits)(zgsz3438, true);
      KILL(lbits)(&zgsz3438);
      KILL(sail_int)(&zgsz3437);
      KILL(lbits)(&zgsz3436);
    }
  }
  uint64_t zbase;
  {
    lbits zgaz3227;
    CREATE(lbits)(&zgaz3227);
    {
      uint64_t zgaz3225;
      {
        lbits zgsz3449;
        CREATE(lbits)(&zgsz3449);
        CONVERT_OF(lbits, fbits)(&zgsz3449, za_top, UINT64_C(32) , true);
        sail_int zgsz3450;
        CREATE(sail_int)(&zgsz3450);
        CONVERT_OF(sail_int, mach_int)(&zgsz3450, zcorrection_base);
        lbits zgsz3451;
        CREATE(lbits)(&zgsz3451);
        add_bits_int(&zgsz3451, zgsz3449, zgsz3450);
        zgaz3225 = CONVERT_OF(fbits, lbits)(zgsz3451, true);
        KILL(lbits)(&zgsz3451);
        KILL(sail_int)(&zgsz3450);
        KILL(lbits)(&zgsz3449);
      }
      sbits zgaz3226;
      {
        uint64_t zgaz3223;
        zgaz3223 = zc.zB;
        sbits zgaz3224;
        {
          sail_int zgsz3445;
          CREATE(sail_int)(&zgsz3445);
          CONVERT_OF(sail_int, mach_int)(&zgsz3445, zE);
          lbits zgsz3446;
          CREATE(lbits)(&zgsz3446);
          sailgen_zzeros_implicit(&zgsz3446, zgsz3445);
          zgaz3224 = CONVERT_OF(sbits, lbits)(zgsz3446, true);
          KILL(lbits)(&zgsz3446);
          KILL(sail_int)(&zgsz3445);
        }
        {
          lbits zgsz3442;
          CREATE(lbits)(&zgsz3442);
          CONVERT_OF(lbits, fbits)(&zgsz3442, zgaz3223, UINT64_C(8) , true);
          lbits zgsz3443;
          CREATE(lbits)(&zgsz3443);
          CONVERT_OF(lbits, sbits)(&zgsz3443, zgaz3224, true);
          lbits zgsz3444;
          CREATE(lbits)(&zgsz3444);
          append(&zgsz3444, zgsz3442, zgsz3443);
          zgaz3226 = CONVERT_OF(sbits, lbits)(zgsz3444, true);
          KILL(lbits)(&zgsz3444);
          KILL(lbits)(&zgsz3443);
          KILL(lbits)(&zgsz3442);
        }
      }
      {
        lbits zgsz3447;
        CREATE(lbits)(&zgsz3447);
        CONVERT_OF(lbits, fbits)(&zgsz3447, zgaz3225, UINT64_C(32) , true);
        lbits zgsz3448;
        CREATE(lbits)(&zgsz3448);
        CONVERT_OF(lbits, sbits)(&zgsz3448, zgaz3226, true);
        append(&zgaz3227, zgsz3447, zgsz3448);
        KILL(lbits)(&zgsz3448);
        KILL(lbits)(&zgsz3447);
      }
    }
    {
      sail_int zgsz3452;
      CREATE(sail_int)(&zgsz3452);
      CONVERT_OF(sail_int, mach_int)(&zgsz3452, zcap_len_width);
      lbits zgsz3453;
      CREATE(lbits)(&zgsz3453);
      sail_truncate(&zgsz3453, zgaz3227, zgsz3452);
      zbase = CONVERT_OF(fbits, lbits)(zgsz3453, true);
      KILL(lbits)(&zgsz3453);
      KILL(sail_int)(&zgsz3452);
    }
    KILL(lbits)(&zgaz3227);
  }
  uint64_t ztop;
  {
    lbits zgaz3222;
    CREATE(lbits)(&zgaz3222);
    {
      uint64_t zgaz3220;
      {
        lbits zgsz3461;
        CREATE(lbits)(&zgsz3461);
        CONVERT_OF(lbits, fbits)(&zgsz3461, za_top, UINT64_C(32) , true);
        sail_int zgsz3462;
        CREATE(sail_int)(&zgsz3462);
        CONVERT_OF(sail_int, mach_int)(&zgsz3462, zcorrection_top);
        lbits zgsz3463;
        CREATE(lbits)(&zgsz3463);
        add_bits_int(&zgsz3463, zgsz3461, zgsz3462);
        zgaz3220 = CONVERT_OF(fbits, lbits)(zgsz3463, true);
        KILL(lbits)(&zgsz3463);
        KILL(sail_int)(&zgsz3462);
        KILL(lbits)(&zgsz3461);
      }
      sbits zgaz3221;
      {
        uint64_t zgaz3218;
        zgaz3218 = zc.zT;
        sbits zgaz3219;
        {
          sail_int zgsz3457;
          CREATE(sail_int)(&zgsz3457);
          CONVERT_OF(sail_int, mach_int)(&zgsz3457, zE);
          lbits zgsz3458;
          CREATE(lbits)(&zgsz3458);
          sailgen_zzeros_implicit(&zgsz3458, zgsz3457);
          zgaz3219 = CONVERT_OF(sbits, lbits)(zgsz3458, true);
          KILL(lbits)(&zgsz3458);
          KILL(sail_int)(&zgsz3457);
        }
        {
          lbits zgsz3454;
          CREATE(lbits)(&zgsz3454);
          CONVERT_OF(lbits, fbits)(&zgsz3454, zgaz3218, UINT64_C(8) , true);
          lbits zgsz3455;
          CREATE(lbits)(&zgsz3455);
          CONVERT_OF(lbits, sbits)(&zgsz3455, zgaz3219, true);
          lbits zgsz3456;
          CREATE(lbits)(&zgsz3456);
          append(&zgsz3456, zgsz3454, zgsz3455);
          zgaz3221 = CONVERT_OF(sbits, lbits)(zgsz3456, true);
          KILL(lbits)(&zgsz3456);
          KILL(lbits)(&zgsz3455);
          KILL(lbits)(&zgsz3454);
        }
      }
      {
        lbits zgsz3459;
        CREATE(lbits)(&zgsz3459);
        CONVERT_OF(lbits, fbits)(&zgsz3459, zgaz3220, UINT64_C(32) , true);
        lbits zgsz3460;
        CREATE(lbits)(&zgsz3460);
        CONVERT_OF(lbits, sbits)(&zgsz3460, zgaz3221, true);
        append(&zgaz3222, zgsz3459, zgsz3460);
        KILL(lbits)(&zgsz3460);
        KILL(lbits)(&zgsz3459);
      }
    }
    {
      sail_int zgsz3464;
      CREATE(sail_int)(&zgsz3464);
      CONVERT_OF(sail_int, mach_int)(&zgsz3464, zcap_len_width);
      lbits zgsz3465;
      CREATE(lbits)(&zgsz3465);
      sail_truncate(&zgsz3465, zgaz3222, zgsz3464);
      ztop = CONVERT_OF(fbits, lbits)(zgsz3465, true);
      KILL(lbits)(&zgsz3465);
      KILL(sail_int)(&zgsz3464);
    }
    KILL(lbits)(&zgaz3222);
  }
  uint64_t zbase2;
  {
    uint64_t zgaz3217;
    {
      fbits zgaz3216;
      {
        int64_t zgaz3215;
        {
          sail_int zgsz3468;
          CREATE(sail_int)(&zgsz3468);
          CONVERT_OF(sail_int, mach_int)(&zgsz3468, zcap_addr_width);
          sail_int zgsz3469;
          CREATE(sail_int)(&zgsz3469);
          CONVERT_OF(sail_int, mach_int)(&zgsz3469, INT64_C(1));
          sail_int zgsz3470;
          CREATE(sail_int)(&zgsz3470);
          sub_int(&zgsz3470, zgsz3468, zgsz3469);
          zgaz3215 = CONVERT_OF(mach_int, sail_int)(zgsz3470);
          KILL(sail_int)(&zgsz3470);
          KILL(sail_int)(&zgsz3469);
          KILL(sail_int)(&zgsz3468);
        }
        {
          lbits zgsz3466;
          CREATE(lbits)(&zgsz3466);
          CONVERT_OF(lbits, fbits)(&zgsz3466, zbase, UINT64_C(33) , true);
          sail_int zgsz3467;
          CREATE(sail_int)(&zgsz3467);
          CONVERT_OF(sail_int, mach_int)(&zgsz3467, zgaz3215);
          zgaz3216 = bitvector_access(zgsz3466, zgsz3467);
          KILL(sail_int)(&zgsz3467);
          KILL(lbits)(&zgsz3466);
        }
      }
      uint64_t zgsz347;
      zgsz347 = UINT64_C(0b0);
      zgsz347 = update_fbits(zgsz347, INT64_C(0), zgaz3216);
      zgaz3217 = zgsz347;
    }
    {
      lbits zgsz3471;
      CREATE(lbits)(&zgsz3471);
      CONVERT_OF(lbits, fbits)(&zgsz3471, UINT64_C(0b0), UINT64_C(1) , true);
      lbits zgsz3472;
      CREATE(lbits)(&zgsz3472);
      CONVERT_OF(lbits, fbits)(&zgsz3472, zgaz3217, UINT64_C(1) , true);
      lbits zgsz3473;
      CREATE(lbits)(&zgsz3473);
      append(&zgsz3473, zgsz3471, zgsz3472);
      zbase2 = CONVERT_OF(fbits, lbits)(zgsz3473, true);
      KILL(lbits)(&zgsz3473);
      KILL(lbits)(&zgsz3472);
      KILL(lbits)(&zgsz3471);
    }
  }
  uint64_t ztop2;
  {
    int64_t zgaz3214;
    {
      sail_int zgsz3478;
      CREATE(sail_int)(&zgsz3478);
      CONVERT_OF(sail_int, mach_int)(&zgsz3478, zcap_addr_width);
      sail_int zgsz3479;
      CREATE(sail_int)(&zgsz3479);
      CONVERT_OF(sail_int, mach_int)(&zgsz3479, INT64_C(1));
      sail_int zgsz3480;
      CREATE(sail_int)(&zgsz3480);
      sub_int(&zgsz3480, zgsz3478, zgsz3479);
      zgaz3214 = CONVERT_OF(mach_int, sail_int)(zgsz3480);
      KILL(sail_int)(&zgsz3480);
      KILL(sail_int)(&zgsz3479);
      KILL(sail_int)(&zgsz3478);
    }
    {
      lbits zgsz3474;
      CREATE(lbits)(&zgsz3474);
      CONVERT_OF(lbits, fbits)(&zgsz3474, ztop, UINT64_C(33) , true);
      sail_int zgsz3475;
      CREATE(sail_int)(&zgsz3475);
      CONVERT_OF(sail_int, mach_int)(&zgsz3475, zcap_addr_width);
      sail_int zgsz3476;
      CREATE(sail_int)(&zgsz3476);
      CONVERT_OF(sail_int, mach_int)(&zgsz3476, zgaz3214);
      lbits zgsz3477;
      CREATE(lbits)(&zgsz3477);
      vector_subrange_lbits(&zgsz3477, zgsz3474, zgsz3475, zgsz3476);
      ztop2 = CONVERT_OF(fbits, lbits)(zgsz3477, true);
      KILL(lbits)(&zgsz3477);
      KILL(sail_int)(&zgsz3476);
      KILL(sail_int)(&zgsz3475);
      KILL(lbits)(&zgsz3474);
    }
  }
  {
    bool zgaz3209;
    {
      bool zgaz3208;
      {
        int64_t zgaz3205;
        {
          sail_int zgsz3483;
          CREATE(sail_int)(&zgsz3483);
          CONVERT_OF(sail_int, mach_int)(&zgsz3483, zcap_max_E);
          sail_int zgsz3484;
          CREATE(sail_int)(&zgsz3484);
          CONVERT_OF(sail_int, mach_int)(&zgsz3484, INT64_C(1));
          sail_int zgsz3485;
          CREATE(sail_int)(&zgsz3485);
          sub_int(&zgsz3485, zgsz3483, zgsz3484);
          zgaz3205 = CONVERT_OF(mach_int, sail_int)(zgsz3485);
          KILL(sail_int)(&zgsz3485);
          KILL(sail_int)(&zgsz3484);
          KILL(sail_int)(&zgsz3483);
        }
        {
          sail_int zgsz3481;
          CREATE(sail_int)(&zgsz3481);
          CONVERT_OF(sail_int, mach_int)(&zgsz3481, zE);
          sail_int zgsz3482;
          CREATE(sail_int)(&zgsz3482);
          CONVERT_OF(sail_int, mach_int)(&zgsz3482, zgaz3205);
          zgaz3208 = lt(zgsz3481, zgsz3482);
          KILL(sail_int)(&zgsz3482);
          KILL(sail_int)(&zgsz3481);
        }
      }
      bool zgsz348;
      if (zgaz3208) {
        int64_t zgaz3207;
        {
          uint64_t zgaz3206;
          {
            lbits zgsz3488;
            CREATE(lbits)(&zgsz3488);
            CONVERT_OF(lbits, fbits)(&zgsz3488, ztop2, UINT64_C(2) , true);
            lbits zgsz3489;
            CREATE(lbits)(&zgsz3489);
            CONVERT_OF(lbits, fbits)(&zgsz3489, zbase2, UINT64_C(2) , true);
            lbits zgsz3490;
            CREATE(lbits)(&zgsz3490);
            sub_bits(&zgsz3490, zgsz3488, zgsz3489);
            zgaz3206 = CONVERT_OF(fbits, lbits)(zgsz3490, true);
            KILL(lbits)(&zgsz3490);
            KILL(lbits)(&zgsz3489);
            KILL(lbits)(&zgsz3488);
          }
          {
            lbits zgsz3486;
            CREATE(lbits)(&zgsz3486);
            CONVERT_OF(lbits, fbits)(&zgsz3486, zgaz3206, UINT64_C(2) , true);
            sail_int zgsz3487;
            CREATE(sail_int)(&zgsz3487);
            sail_unsigned(&zgsz3487, zgsz3486);
            zgaz3207 = CONVERT_OF(mach_int, sail_int)(zgsz3487);
            KILL(sail_int)(&zgsz3487);
            KILL(lbits)(&zgsz3486);
          }
        }
        {
          sail_int zgsz3491;
          CREATE(sail_int)(&zgsz3491);
          CONVERT_OF(sail_int, mach_int)(&zgsz3491, zgaz3207);
          sail_int zgsz3492;
          CREATE(sail_int)(&zgsz3492);
          CONVERT_OF(sail_int, mach_int)(&zgsz3492, INT64_C(1));
          zgsz348 = gt(zgsz3491, zgsz3492);
          KILL(sail_int)(&zgsz3492);
          KILL(sail_int)(&zgsz3491);
        }
      } else {  zgsz348 = false;  }
      zgaz3209 = zgsz348;
    }
    unit zgsz349;
    if (zgaz3209) {
      fbits zgaz3211;
      {
        fbits zgaz3210;
        {
          lbits zgsz3493;
          CREATE(lbits)(&zgsz3493);
          CONVERT_OF(lbits, fbits)(&zgsz3493, ztop, UINT64_C(33) , true);
          sail_int zgsz3494;
          CREATE(sail_int)(&zgsz3494);
          CONVERT_OF(sail_int, mach_int)(&zgsz3494, zcap_addr_width);
          zgaz3210 = bitvector_access(zgsz3493, zgsz3494);
          KILL(sail_int)(&zgsz3494);
          KILL(lbits)(&zgsz3493);
        }
        zgaz3211 = sailgen_not_bit(zgaz3210);
      }
      {
        lbits zgsz3495;
        CREATE(lbits)(&zgsz3495);
        CONVERT_OF(lbits, fbits)(&zgsz3495, ztop, UINT64_C(33) , true);
        sail_int zgsz3496;
        CREATE(sail_int)(&zgsz3496);
        CONVERT_OF(sail_int, mach_int)(&zgsz3496, zcap_addr_width);
        lbits zgsz3497;
        CREATE(lbits)(&zgsz3497);
        update_lbits(&zgsz3497, zgsz3495, zgsz3496, zgaz3211);
        ztop = CONVERT_OF(fbits, lbits)(zgsz3497, true);
        KILL(lbits)(&zgsz3497);
        KILL(sail_int)(&zgsz3496);
        KILL(lbits)(&zgsz3495);
      }
      zgsz349 = UNIT;
    } else {  zgsz349 = UNIT;  }
  }
  uint64_t zgaz3213;
  {
    int64_t zgaz3212;
    {
      sail_int zgsz3502;
      CREATE(sail_int)(&zgsz3502);
      CONVERT_OF(sail_int, mach_int)(&zgsz3502, zcap_addr_width);
      sail_int zgsz3503;
      CREATE(sail_int)(&zgsz3503);
      CONVERT_OF(sail_int, mach_int)(&zgsz3503, INT64_C(1));
      sail_int zgsz3504;
      CREATE(sail_int)(&zgsz3504);
      sub_int(&zgsz3504, zgsz3502, zgsz3503);
      zgaz3212 = CONVERT_OF(mach_int, sail_int)(zgsz3504);
      KILL(sail_int)(&zgsz3504);
      KILL(sail_int)(&zgsz3503);
      KILL(sail_int)(&zgsz3502);
    }
    {
      lbits zgsz3498;
      CREATE(lbits)(&zgsz3498);
      CONVERT_OF(lbits, fbits)(&zgsz3498, zbase, UINT64_C(33) , true);
      sail_int zgsz3499;
      CREATE(sail_int)(&zgsz3499);
      CONVERT_OF(sail_int, mach_int)(&zgsz3499, zgaz3212);
      sail_int zgsz3500;
      CREATE(sail_int)(&zgsz3500);
      CONVERT_OF(sail_int, mach_int)(&zgsz3500, INT64_C(0));
      lbits zgsz3501;
      CREATE(lbits)(&zgsz3501);
      vector_subrange_lbits(&zgsz3501, zgsz3498, zgsz3499, zgsz3500);
      zgaz3213 = CONVERT_OF(fbits, lbits)(zgsz3501, true);
      KILL(lbits)(&zgsz3501);
      KILL(sail_int)(&zgsz3500);
      KILL(sail_int)(&zgsz3499);
      KILL(lbits)(&zgsz3498);
    }
  }
  struct ztuple_z8z5bv32zCz0z5bv33z9 zgsz350;
  zgsz350.ztup0 = zgaz3213;
  zgsz350.ztup1 = ztop;
  zcbz324 = zgsz350;


















end_function_108: ;
  return zcbz324;
end_block_exception_109: ;
  struct ztuple_z8z5bv32zCz0z5bv33z9 zcbz347 = { .ztup1 = UINT64_C(0xdeadc0de), .ztup0 = UINT64_C(0xdeadc0de) };
  return zcbz347;
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
  __label__ case_112, finish_match_111, end_function_113, end_block_exception_114;

  struct ztuple_z8z5i64zCz0z5i64z9 zcbz325;
  struct ztuple_z8z5bv32zCz0z5bv33z9 zgaz3239;
  zgaz3239 = sailgen_getCapBoundsBits(zcap);
  struct ztuple_z8z5i64zCz0z5i64z9 zgsz351;
  {
    uint64_t zbase;
    zbase = zgaz3239.ztup0;
    uint64_t ztop;
    ztop = zgaz3239.ztup1;
    int64_t zgaz3240;
    {
      lbits zgsz3516;
      CREATE(lbits)(&zgsz3516);
      CONVERT_OF(lbits, fbits)(&zgsz3516, zbase, UINT64_C(32) , true);
      sail_int zgsz3517;
      CREATE(sail_int)(&zgsz3517);
      sail_unsigned(&zgsz3517, zgsz3516);
      zgaz3240 = CONVERT_OF(mach_int, sail_int)(zgsz3517);
      KILL(sail_int)(&zgsz3517);
      KILL(lbits)(&zgsz3516);
    }
    int64_t zgaz3241;
    {
      lbits zgsz3514;
      CREATE(lbits)(&zgsz3514);
      CONVERT_OF(lbits, fbits)(&zgsz3514, ztop, UINT64_C(33) , true);
      sail_int zgsz3515;
      CREATE(sail_int)(&zgsz3515);
      sail_unsigned(&zgsz3515, zgsz3514);
      zgaz3241 = CONVERT_OF(mach_int, sail_int)(zgsz3515);
      KILL(sail_int)(&zgsz3515);
      KILL(lbits)(&zgsz3514);
    }
    struct ztuple_z8z5i64zCz0z5i64z9 zgsz352;
    zgsz352.ztup0 = zgaz3240;
    zgsz352.ztup1 = zgaz3241;
    zgsz351 = zgsz352;
    goto finish_match_111;
  }
case_112: ;
  sail_match_failure("getCapBounds");
finish_match_111: ;
  zcbz325 = zgsz351;


end_function_113: ;
  return zcbz325;
end_block_exception_114: ;
  struct ztuple_z8z5i64zCz0z5i64z9 zcbz348 = { .ztup1 = INT64_C(0xdeadc0de), .ztup0 = INT64_C(0xdeadc0de) };
  return zcbz348;
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
  __label__ end_function_116, end_block_exception_117;

  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz326;
  uint64_t zext_base;
  {
    lbits zgsz3649;
    CREATE(lbits)(&zgsz3649);
    CONVERT_OF(lbits, fbits)(&zgsz3649, UINT64_C(0b0), UINT64_C(1) , true);
    lbits zgsz3650;
    CREATE(lbits)(&zgsz3650);
    CONVERT_OF(lbits, fbits)(&zgsz3650, zbase, UINT64_C(32) , true);
    lbits zgsz3651;
    CREATE(lbits)(&zgsz3651);
    append(&zgsz3651, zgsz3649, zgsz3650);
    zext_base = CONVERT_OF(fbits, lbits)(zgsz3651, true);
    KILL(lbits)(&zgsz3651);
    KILL(lbits)(&zgsz3650);
    KILL(lbits)(&zgsz3649);
  }
  uint64_t zlength;
  {
    lbits zgsz3646;
    CREATE(lbits)(&zgsz3646);
    CONVERT_OF(lbits, fbits)(&zgsz3646, ztop, UINT64_C(33) , true);
    lbits zgsz3647;
    CREATE(lbits)(&zgsz3647);
    CONVERT_OF(lbits, fbits)(&zgsz3647, zext_base, UINT64_C(33) , true);
    lbits zgsz3648;
    CREATE(lbits)(&zgsz3648);
    sub_bits(&zgsz3648, zgsz3646, zgsz3647);
    zlength = CONVERT_OF(fbits, lbits)(zgsz3648, true);
    KILL(lbits)(&zgsz3648);
    KILL(lbits)(&zgsz3647);
    KILL(lbits)(&zgsz3646);
  }
  int64_t ze;
  {
    int64_t zgaz3275;
    {
      uint64_t zgaz3274;
      {
        int64_t zgaz3273;
        {
          sail_int zgsz3522;
          CREATE(sail_int)(&zgsz3522);
          CONVERT_OF(sail_int, mach_int)(&zgsz3522, zcap_mantissa_width);
          sail_int zgsz3523;
          CREATE(sail_int)(&zgsz3523);
          CONVERT_OF(sail_int, mach_int)(&zgsz3523, INT64_C(1));
          sail_int zgsz3524;
          CREATE(sail_int)(&zgsz3524);
          sub_int(&zgsz3524, zgsz3522, zgsz3523);
          zgaz3273 = CONVERT_OF(mach_int, sail_int)(zgsz3524);
          KILL(sail_int)(&zgsz3524);
          KILL(sail_int)(&zgsz3523);
          KILL(sail_int)(&zgsz3522);
        }
        {
          lbits zgsz3518;
          CREATE(lbits)(&zgsz3518);
          CONVERT_OF(lbits, fbits)(&zgsz3518, zlength, UINT64_C(33) , true);
          sail_int zgsz3519;
          CREATE(sail_int)(&zgsz3519);
          CONVERT_OF(sail_int, mach_int)(&zgsz3519, zcap_addr_width);
          sail_int zgsz3520;
          CREATE(sail_int)(&zgsz3520);
          CONVERT_OF(sail_int, mach_int)(&zgsz3520, zgaz3273);
          lbits zgsz3521;
          CREATE(lbits)(&zgsz3521);
          vector_subrange_lbits(&zgsz3521, zgsz3518, zgsz3519, zgsz3520);
          zgaz3274 = CONVERT_OF(fbits, lbits)(zgsz3521, true);
          KILL(lbits)(&zgsz3521);
          KILL(sail_int)(&zgsz3520);
          KILL(sail_int)(&zgsz3519);
          KILL(lbits)(&zgsz3518);
        }
      }
      {
        lbits zgsz3525;
        CREATE(lbits)(&zgsz3525);
        CONVERT_OF(lbits, fbits)(&zgsz3525, zgaz3274, UINT64_C(26) , true);
        sail_int zgsz3526;
        CREATE(sail_int)(&zgsz3526);
        count_leading_zeros(&zgsz3526, zgsz3525);
        zgaz3275 = CONVERT_OF(mach_int, sail_int)(zgsz3526);
        KILL(sail_int)(&zgsz3526);
        KILL(lbits)(&zgsz3525);
      }
    }
    {
      sail_int zgsz3527;
      CREATE(sail_int)(&zgsz3527);
      CONVERT_OF(sail_int, mach_int)(&zgsz3527, zcap_max_E);
      sail_int zgsz3528;
      CREATE(sail_int)(&zgsz3528);
      CONVERT_OF(sail_int, mach_int)(&zgsz3528, zgaz3275);
      sail_int zgsz3529;
      CREATE(sail_int)(&zgsz3529);
      sub_int(&zgsz3529, zgsz3527, zgsz3528);
      ze = CONVERT_OF(mach_int, sail_int)(zgsz3529);
      KILL(sail_int)(&zgsz3529);
      KILL(sail_int)(&zgsz3528);
      KILL(sail_int)(&zgsz3527);
    }
  }
  bool zie;
  {
    bool zgaz3272;
    {
      sail_int zgsz3535;
      CREATE(sail_int)(&zgsz3535);
      CONVERT_OF(sail_int, mach_int)(&zgsz3535, ze);
      sail_int zgsz3536;
      CREATE(sail_int)(&zgsz3536);
      CONVERT_OF(sail_int, mach_int)(&zgsz3536, INT64_C(0));
      zgaz3272 = sailgen_neq_int(zgsz3535, zgsz3536);
      KILL(sail_int)(&zgsz3536);
      KILL(sail_int)(&zgsz3535);
    }
    bool zgsz354;
    if (zgaz3272) {  zgsz354 = true;  } else {
      fbits zgaz3271;
      {
        int64_t zgaz3270;
        {
          sail_int zgsz3532;
          CREATE(sail_int)(&zgsz3532);
          CONVERT_OF(sail_int, mach_int)(&zgsz3532, zcap_mantissa_width);
          sail_int zgsz3533;
          CREATE(sail_int)(&zgsz3533);
          CONVERT_OF(sail_int, mach_int)(&zgsz3533, INT64_C(2));
          sail_int zgsz3534;
          CREATE(sail_int)(&zgsz3534);
          sub_int(&zgsz3534, zgsz3532, zgsz3533);
          zgaz3270 = CONVERT_OF(mach_int, sail_int)(zgsz3534);
          KILL(sail_int)(&zgsz3534);
          KILL(sail_int)(&zgsz3533);
          KILL(sail_int)(&zgsz3532);
        }
        {
          lbits zgsz3530;
          CREATE(lbits)(&zgsz3530);
          CONVERT_OF(lbits, fbits)(&zgsz3530, zlength, UINT64_C(33) , true);
          sail_int zgsz3531;
          CREATE(sail_int)(&zgsz3531);
          CONVERT_OF(sail_int, mach_int)(&zgsz3531, zgaz3270);
          zgaz3271 = bitvector_access(zgsz3530, zgsz3531);
          KILL(sail_int)(&zgsz3531);
          KILL(lbits)(&zgsz3530);
        }
      }
      zgsz354 = eq_bit(zgaz3271, UINT64_C(1));
    }
    zie = zgsz354;
  }
  uint64_t zBbits;
  {
    lbits zgsz3643;
    CREATE(lbits)(&zgsz3643);
    CONVERT_OF(lbits, fbits)(&zgsz3643, zbase, UINT64_C(32) , true);
    sail_int zgsz3644;
    CREATE(sail_int)(&zgsz3644);
    CONVERT_OF(sail_int, mach_int)(&zgsz3644, zcap_mantissa_width);
    lbits zgsz3645;
    CREATE(lbits)(&zgsz3645);
    sail_truncate(&zgsz3645, zgsz3643, zgsz3644);
    zBbits = CONVERT_OF(fbits, lbits)(zgsz3645, true);
    KILL(lbits)(&zgsz3645);
    KILL(sail_int)(&zgsz3644);
    KILL(lbits)(&zgsz3643);
  }
  uint64_t zTbits;
  {
    lbits zgsz3640;
    CREATE(lbits)(&zgsz3640);
    CONVERT_OF(lbits, fbits)(&zgsz3640, ztop, UINT64_C(33) , true);
    sail_int zgsz3641;
    CREATE(sail_int)(&zgsz3641);
    CONVERT_OF(sail_int, mach_int)(&zgsz3641, zcap_mantissa_width);
    lbits zgsz3642;
    CREATE(lbits)(&zgsz3642);
    sail_truncate(&zgsz3642, zgsz3640, zgsz3641);
    zTbits = CONVERT_OF(fbits, lbits)(zgsz3642, true);
    KILL(lbits)(&zgsz3642);
    KILL(sail_int)(&zgsz3641);
    KILL(lbits)(&zgsz3640);
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
        uint64_t zgaz3265;
        {
          int64_t zgaz3264;
          {
            sail_int zgsz3540;
            CREATE(sail_int)(&zgsz3540);
            CONVERT_OF(sail_int, mach_int)(&zgsz3540, ze);
            sail_int zgsz3541;
            CREATE(sail_int)(&zgsz3541);
            CONVERT_OF(sail_int, mach_int)(&zgsz3541, INT64_C(3));
            sail_int zgsz3542;
            CREATE(sail_int)(&zgsz3542);
            add_int(&zgsz3542, zgsz3540, zgsz3541);
            zgaz3264 = CONVERT_OF(mach_int, sail_int)(zgsz3542);
            KILL(sail_int)(&zgsz3542);
            KILL(sail_int)(&zgsz3541);
            KILL(sail_int)(&zgsz3540);
          }
          {
            lbits zgsz3537;
            CREATE(lbits)(&zgsz3537);
            CONVERT_OF(lbits, fbits)(&zgsz3537, zbase, UINT64_C(32) , true);
            sail_int zgsz3538;
            CREATE(sail_int)(&zgsz3538);
            CONVERT_OF(sail_int, mach_int)(&zgsz3538, zgaz3264);
            lbits zgsz3539;
            CREATE(lbits)(&zgsz3539);
            shiftr(&zgsz3539, zgsz3537, zgsz3538);
            zgaz3265 = CONVERT_OF(fbits, lbits)(zgsz3539, true);
            KILL(lbits)(&zgsz3539);
            KILL(sail_int)(&zgsz3538);
            KILL(lbits)(&zgsz3537);
          }
        }
        int64_t zgaz3266;
        {
          sail_int zgsz3546;
          CREATE(sail_int)(&zgsz3546);
          CONVERT_OF(sail_int, mach_int)(&zgsz3546, zcap_mantissa_width);
          sail_int zgsz3547;
          CREATE(sail_int)(&zgsz3547);
          CONVERT_OF(sail_int, mach_int)(&zgsz3547, INT64_C(3));
          sail_int zgsz3548;
          CREATE(sail_int)(&zgsz3548);
          sub_int(&zgsz3548, zgsz3546, zgsz3547);
          zgaz3266 = CONVERT_OF(mach_int, sail_int)(zgsz3548);
          KILL(sail_int)(&zgsz3548);
          KILL(sail_int)(&zgsz3547);
          KILL(sail_int)(&zgsz3546);
        }
        {
          lbits zgsz3543;
          CREATE(lbits)(&zgsz3543);
          CONVERT_OF(lbits, fbits)(&zgsz3543, zgaz3265, UINT64_C(32) , true);
          sail_int zgsz3544;
          CREATE(sail_int)(&zgsz3544);
          CONVERT_OF(sail_int, mach_int)(&zgsz3544, zgaz3266);
          lbits zgsz3545;
          CREATE(lbits)(&zgsz3545);
          sail_truncate(&zgsz3545, zgsz3543, zgsz3544);
          zB_ie = CONVERT_OF(fbits, lbits)(zgsz3545, true);
          KILL(lbits)(&zgsz3545);
          KILL(sail_int)(&zgsz3544);
          KILL(lbits)(&zgsz3543);
        }
      }
      uint64_t zT_ie;
      {
        uint64_t zgaz3262;
        {
          int64_t zgaz3261;
          {
            sail_int zgsz3552;
            CREATE(sail_int)(&zgsz3552);
            CONVERT_OF(sail_int, mach_int)(&zgsz3552, ze);
            sail_int zgsz3553;
            CREATE(sail_int)(&zgsz3553);
            CONVERT_OF(sail_int, mach_int)(&zgsz3553, INT64_C(3));
            sail_int zgsz3554;
            CREATE(sail_int)(&zgsz3554);
            add_int(&zgsz3554, zgsz3552, zgsz3553);
            zgaz3261 = CONVERT_OF(mach_int, sail_int)(zgsz3554);
            KILL(sail_int)(&zgsz3554);
            KILL(sail_int)(&zgsz3553);
            KILL(sail_int)(&zgsz3552);
          }
          {
            lbits zgsz3549;
            CREATE(lbits)(&zgsz3549);
            CONVERT_OF(lbits, fbits)(&zgsz3549, ztop, UINT64_C(33) , true);
            sail_int zgsz3550;
            CREATE(sail_int)(&zgsz3550);
            CONVERT_OF(sail_int, mach_int)(&zgsz3550, zgaz3261);
            lbits zgsz3551;
            CREATE(lbits)(&zgsz3551);
            shiftr(&zgsz3551, zgsz3549, zgsz3550);
            zgaz3262 = CONVERT_OF(fbits, lbits)(zgsz3551, true);
            KILL(lbits)(&zgsz3551);
            KILL(sail_int)(&zgsz3550);
            KILL(lbits)(&zgsz3549);
          }
        }
        int64_t zgaz3263;
        {
          sail_int zgsz3558;
          CREATE(sail_int)(&zgsz3558);
          CONVERT_OF(sail_int, mach_int)(&zgsz3558, zcap_mantissa_width);
          sail_int zgsz3559;
          CREATE(sail_int)(&zgsz3559);
          CONVERT_OF(sail_int, mach_int)(&zgsz3559, INT64_C(3));
          sail_int zgsz3560;
          CREATE(sail_int)(&zgsz3560);
          sub_int(&zgsz3560, zgsz3558, zgsz3559);
          zgaz3263 = CONVERT_OF(mach_int, sail_int)(zgsz3560);
          KILL(sail_int)(&zgsz3560);
          KILL(sail_int)(&zgsz3559);
          KILL(sail_int)(&zgsz3558);
        }
        {
          lbits zgsz3555;
          CREATE(lbits)(&zgsz3555);
          CONVERT_OF(lbits, fbits)(&zgsz3555, zgaz3262, UINT64_C(33) , true);
          sail_int zgsz3556;
          CREATE(sail_int)(&zgsz3556);
          CONVERT_OF(sail_int, mach_int)(&zgsz3556, zgaz3263);
          lbits zgsz3557;
          CREATE(lbits)(&zgsz3557);
          sail_truncate(&zgsz3557, zgsz3555, zgsz3556);
          zT_ie = CONVERT_OF(fbits, lbits)(zgsz3557, true);
          KILL(lbits)(&zgsz3557);
          KILL(sail_int)(&zgsz3556);
          KILL(lbits)(&zgsz3555);
        }
      }
      uint64_t zmaskLo;
      {
        int64_t zgaz3259;
        {
          sail_int zgsz3569;
          CREATE(sail_int)(&zgsz3569);
          CONVERT_OF(sail_int, mach_int)(&zgsz3569, INT64_C(32));
          sail_int zgsz3570;
          CREATE(sail_int)(&zgsz3570);
          CONVERT_OF(sail_int, mach_int)(&zgsz3570, INT64_C(1));
          sail_int zgsz3571;
          CREATE(sail_int)(&zgsz3571);
          add_int(&zgsz3571, zgsz3569, zgsz3570);
          zgaz3259 = CONVERT_OF(mach_int, sail_int)(zgsz3571);
          KILL(sail_int)(&zgsz3571);
          KILL(sail_int)(&zgsz3570);
          KILL(sail_int)(&zgsz3569);
        }
        sbits zgaz3260;
        {
          int64_t zgaz3258;
          {
            sail_int zgsz3563;
            CREATE(sail_int)(&zgsz3563);
            CONVERT_OF(sail_int, mach_int)(&zgsz3563, ze);
            sail_int zgsz3564;
            CREATE(sail_int)(&zgsz3564);
            CONVERT_OF(sail_int, mach_int)(&zgsz3564, INT64_C(3));
            sail_int zgsz3565;
            CREATE(sail_int)(&zgsz3565);
            add_int(&zgsz3565, zgsz3563, zgsz3564);
            zgaz3258 = CONVERT_OF(mach_int, sail_int)(zgsz3565);
            KILL(sail_int)(&zgsz3565);
            KILL(sail_int)(&zgsz3564);
            KILL(sail_int)(&zgsz3563);
          }
          {
            sail_int zgsz3561;
            CREATE(sail_int)(&zgsz3561);
            CONVERT_OF(sail_int, mach_int)(&zgsz3561, zgaz3258);
            lbits zgsz3562;
            CREATE(lbits)(&zgsz3562);
            sailgen_ones(&zgsz3562, zgsz3561);
            zgaz3260 = CONVERT_OF(sbits, lbits)(zgsz3562, true);
            KILL(lbits)(&zgsz3562);
            KILL(sail_int)(&zgsz3561);
          }
        }
        {
          sail_int zgsz3566;
          CREATE(sail_int)(&zgsz3566);
          CONVERT_OF(sail_int, mach_int)(&zgsz3566, zgaz3259);
          lbits zgsz3567;
          CREATE(lbits)(&zgsz3567);
          CONVERT_OF(lbits, sbits)(&zgsz3567, zgaz3260, true);
          lbits zgsz3568;
          CREATE(lbits)(&zgsz3568);
          sailgen_EXTZ(&zgsz3568, zgsz3566, zgsz3567);
          zmaskLo = CONVERT_OF(fbits, lbits)(zgsz3568, true);
          KILL(lbits)(&zgsz3568);
          KILL(lbits)(&zgsz3567);
          KILL(sail_int)(&zgsz3566);
        }
      }
      {
        int64_t zgaz3243;
        {
          uint64_t zgaz3242;
          {
            lbits zgsz3574;
            CREATE(lbits)(&zgsz3574);
            CONVERT_OF(lbits, fbits)(&zgsz3574, zext_base, UINT64_C(33) , true);
            lbits zgsz3575;
            CREATE(lbits)(&zgsz3575);
            CONVERT_OF(lbits, fbits)(&zgsz3575, zmaskLo, UINT64_C(33) , true);
            lbits zgsz3576;
            CREATE(lbits)(&zgsz3576);
            and_bits(&zgsz3576, zgsz3574, zgsz3575);
            zgaz3242 = CONVERT_OF(fbits, lbits)(zgsz3576, true);
            KILL(lbits)(&zgsz3576);
            KILL(lbits)(&zgsz3575);
            KILL(lbits)(&zgsz3574);
          }
          {
            lbits zgsz3572;
            CREATE(lbits)(&zgsz3572);
            CONVERT_OF(lbits, fbits)(&zgsz3572, zgaz3242, UINT64_C(33) , true);
            sail_int zgsz3573;
            CREATE(sail_int)(&zgsz3573);
            sail_unsigned(&zgsz3573, zgsz3572);
            zgaz3243 = CONVERT_OF(mach_int, sail_int)(zgsz3573);
            KILL(sail_int)(&zgsz3573);
            KILL(lbits)(&zgsz3572);
          }
        }
        {
          sail_int zgsz3577;
          CREATE(sail_int)(&zgsz3577);
          CONVERT_OF(sail_int, mach_int)(&zgsz3577, zgaz3243);
          sail_int zgsz3578;
          CREATE(sail_int)(&zgsz3578);
          CONVERT_OF(sail_int, mach_int)(&zgsz3578, INT64_C(0));
          zlostSignificantBase = sailgen_neq_int(zgsz3577, zgsz3578);
          KILL(sail_int)(&zgsz3578);
          KILL(sail_int)(&zgsz3577);
        }
        unit zgsz358;
        zgsz358 = UNIT;
      }
      {
        int64_t zgaz3245;
        {
          uint64_t zgaz3244;
          {
            lbits zgsz3581;
            CREATE(lbits)(&zgsz3581);
            CONVERT_OF(lbits, fbits)(&zgsz3581, ztop, UINT64_C(33) , true);
            lbits zgsz3582;
            CREATE(lbits)(&zgsz3582);
            CONVERT_OF(lbits, fbits)(&zgsz3582, zmaskLo, UINT64_C(33) , true);
            lbits zgsz3583;
            CREATE(lbits)(&zgsz3583);
            and_bits(&zgsz3583, zgsz3581, zgsz3582);
            zgaz3244 = CONVERT_OF(fbits, lbits)(zgsz3583, true);
            KILL(lbits)(&zgsz3583);
            KILL(lbits)(&zgsz3582);
            KILL(lbits)(&zgsz3581);
          }
          {
            lbits zgsz3579;
            CREATE(lbits)(&zgsz3579);
            CONVERT_OF(lbits, fbits)(&zgsz3579, zgaz3244, UINT64_C(33) , true);
            sail_int zgsz3580;
            CREATE(sail_int)(&zgsz3580);
            sail_unsigned(&zgsz3580, zgsz3579);
            zgaz3245 = CONVERT_OF(mach_int, sail_int)(zgsz3580);
            KILL(sail_int)(&zgsz3580);
            KILL(lbits)(&zgsz3579);
          }
        }
        {
          sail_int zgsz3584;
          CREATE(sail_int)(&zgsz3584);
          CONVERT_OF(sail_int, mach_int)(&zgsz3584, zgaz3245);
          sail_int zgsz3585;
          CREATE(sail_int)(&zgsz3585);
          CONVERT_OF(sail_int, mach_int)(&zgsz3585, INT64_C(0));
          zlostSignificantTop = sailgen_neq_int(zgsz3584, zgsz3585);
          KILL(sail_int)(&zgsz3585);
          KILL(sail_int)(&zgsz3584);
        }
        unit zgsz357;
        zgsz357 = UNIT;
      }
      {
        unit zgsz356;
        if (zlostSignificantTop) {
          {
            lbits zgsz3586;
            CREATE(lbits)(&zgsz3586);
            CONVERT_OF(lbits, fbits)(&zgsz3586, zT_ie, UINT64_C(5) , true);
            sail_int zgsz3587;
            CREATE(sail_int)(&zgsz3587);
            CONVERT_OF(sail_int, mach_int)(&zgsz3587, INT64_C(1));
            lbits zgsz3588;
            CREATE(lbits)(&zgsz3588);
            add_bits_int(&zgsz3588, zgsz3586, zgsz3587);
            zT_ie = CONVERT_OF(fbits, lbits)(zgsz3588, true);
            KILL(lbits)(&zgsz3588);
            KILL(sail_int)(&zgsz3587);
            KILL(lbits)(&zgsz3586);
          }
          zgsz356 = UNIT;
        } else {  zgsz356 = UNIT;  }
      }
      uint64_t zlen_ie;
      {
        lbits zgsz3631;
        CREATE(lbits)(&zgsz3631);
        CONVERT_OF(lbits, fbits)(&zgsz3631, zT_ie, UINT64_C(5) , true);
        lbits zgsz3632;
        CREATE(lbits)(&zgsz3632);
        CONVERT_OF(lbits, fbits)(&zgsz3632, zB_ie, UINT64_C(5) , true);
        lbits zgsz3633;
        CREATE(lbits)(&zgsz3633);
        sub_bits(&zgsz3633, zgsz3631, zgsz3632);
        zlen_ie = CONVERT_OF(fbits, lbits)(zgsz3633, true);
        KILL(lbits)(&zgsz3633);
        KILL(lbits)(&zgsz3632);
        KILL(lbits)(&zgsz3631);
      }
      {
        bool zgaz3248;
        {
          fbits zgaz3247;
          {
            int64_t zgaz3246;
            {
              sail_int zgsz3591;
              CREATE(sail_int)(&zgsz3591);
              CONVERT_OF(sail_int, mach_int)(&zgsz3591, zcap_mantissa_width);
              sail_int zgsz3592;
              CREATE(sail_int)(&zgsz3592);
              CONVERT_OF(sail_int, mach_int)(&zgsz3592, INT64_C(4));
              sail_int zgsz3593;
              CREATE(sail_int)(&zgsz3593);
              sub_int(&zgsz3593, zgsz3591, zgsz3592);
              zgaz3246 = CONVERT_OF(mach_int, sail_int)(zgsz3593);
              KILL(sail_int)(&zgsz3593);
              KILL(sail_int)(&zgsz3592);
              KILL(sail_int)(&zgsz3591);
            }
            {
              lbits zgsz3589;
              CREATE(lbits)(&zgsz3589);
              CONVERT_OF(lbits, fbits)(&zgsz3589, zlen_ie, UINT64_C(5) , true);
              sail_int zgsz3590;
              CREATE(sail_int)(&zgsz3590);
              CONVERT_OF(sail_int, mach_int)(&zgsz3590, zgaz3246);
              zgaz3247 = bitvector_access(zgsz3589, zgsz3590);
              KILL(sail_int)(&zgsz3590);
              KILL(lbits)(&zgsz3589);
            }
          }
          zgaz3248 = eq_bit(zgaz3247, UINT64_C(1));
        }
        unit zgsz360;
        if (zgaz3248) {
          {
            zincE = true;
            unit zgsz366;
            zgsz366 = UNIT;
          }
          {
            bool zgsz361;
            if (zlostSignificantBase) {  zgsz361 = true;  } else {
              fbits zgaz3249;
              {
                lbits zgsz3594;
                CREATE(lbits)(&zgsz3594);
                CONVERT_OF(lbits, fbits)(&zgsz3594, zB_ie, UINT64_C(5) , true);
                sail_int zgsz3595;
                CREATE(sail_int)(&zgsz3595);
                CONVERT_OF(sail_int, mach_int)(&zgsz3595, INT64_C(0));
                zgaz3249 = bitvector_access(zgsz3594, zgsz3595);
                KILL(sail_int)(&zgsz3595);
                KILL(lbits)(&zgsz3594);
              }
              zgsz361 = eq_bit(zgaz3249, UINT64_C(1));
            }
            zlostSignificantBase = zgsz361;
            unit zgsz365;
            zgsz365 = UNIT;
          }
          {
            bool zgsz362;
            if (zlostSignificantTop) {  zgsz362 = true;  } else {
              fbits zgaz3250;
              {
                lbits zgsz3596;
                CREATE(lbits)(&zgsz3596);
                CONVERT_OF(lbits, fbits)(&zgsz3596, zT_ie, UINT64_C(5) , true);
                sail_int zgsz3597;
                CREATE(sail_int)(&zgsz3597);
                CONVERT_OF(sail_int, mach_int)(&zgsz3597, INT64_C(0));
                zgaz3250 = bitvector_access(zgsz3596, zgsz3597);
                KILL(sail_int)(&zgsz3597);
                KILL(lbits)(&zgsz3596);
              }
              zgsz362 = eq_bit(zgaz3250, UINT64_C(1));
            }
            zlostSignificantTop = zgsz362;
            unit zgsz364;
            zgsz364 = UNIT;
          }
          {
            uint64_t zgaz3252;
            {
              int64_t zgaz3251;
              {
                sail_int zgsz3601;
                CREATE(sail_int)(&zgsz3601);
                CONVERT_OF(sail_int, mach_int)(&zgsz3601, ze);
                sail_int zgsz3602;
                CREATE(sail_int)(&zgsz3602);
                CONVERT_OF(sail_int, mach_int)(&zgsz3602, INT64_C(4));
                sail_int zgsz3603;
                CREATE(sail_int)(&zgsz3603);
                add_int(&zgsz3603, zgsz3601, zgsz3602);
                zgaz3251 = CONVERT_OF(mach_int, sail_int)(zgsz3603);
                KILL(sail_int)(&zgsz3603);
                KILL(sail_int)(&zgsz3602);
                KILL(sail_int)(&zgsz3601);
              }
              {
                lbits zgsz3598;
                CREATE(lbits)(&zgsz3598);
                CONVERT_OF(lbits, fbits)(&zgsz3598, zbase, UINT64_C(32) , true);
                sail_int zgsz3599;
                CREATE(sail_int)(&zgsz3599);
                CONVERT_OF(sail_int, mach_int)(&zgsz3599, zgaz3251);
                lbits zgsz3600;
                CREATE(lbits)(&zgsz3600);
                shiftr(&zgsz3600, zgsz3598, zgsz3599);
                zgaz3252 = CONVERT_OF(fbits, lbits)(zgsz3600, true);
                KILL(lbits)(&zgsz3600);
                KILL(sail_int)(&zgsz3599);
                KILL(lbits)(&zgsz3598);
              }
            }
            int64_t zgaz3253;
            {
              sail_int zgsz3607;
              CREATE(sail_int)(&zgsz3607);
              CONVERT_OF(sail_int, mach_int)(&zgsz3607, zcap_mantissa_width);
              sail_int zgsz3608;
              CREATE(sail_int)(&zgsz3608);
              CONVERT_OF(sail_int, mach_int)(&zgsz3608, INT64_C(3));
              sail_int zgsz3609;
              CREATE(sail_int)(&zgsz3609);
              sub_int(&zgsz3609, zgsz3607, zgsz3608);
              zgaz3253 = CONVERT_OF(mach_int, sail_int)(zgsz3609);
              KILL(sail_int)(&zgsz3609);
              KILL(sail_int)(&zgsz3608);
              KILL(sail_int)(&zgsz3607);
            }
            {
              lbits zgsz3604;
              CREATE(lbits)(&zgsz3604);
              CONVERT_OF(lbits, fbits)(&zgsz3604, zgaz3252, UINT64_C(32) , true);
              sail_int zgsz3605;
              CREATE(sail_int)(&zgsz3605);
              CONVERT_OF(sail_int, mach_int)(&zgsz3605, zgaz3253);
              lbits zgsz3606;
              CREATE(lbits)(&zgsz3606);
              sail_truncate(&zgsz3606, zgsz3604, zgsz3605);
              zB_ie = CONVERT_OF(fbits, lbits)(zgsz3606, true);
              KILL(lbits)(&zgsz3606);
              KILL(sail_int)(&zgsz3605);
              KILL(lbits)(&zgsz3604);
            }
            unit zgsz363;
            zgsz363 = UNIT;
          }
          int64_t zincT;
          if (zlostSignificantTop) {  zincT = INT64_C(1);  } else {  zincT = INT64_C(0);  }
          uint64_t zgaz3257;
          {
            uint64_t zgaz3255;
            {
              int64_t zgaz3254;
              {
                sail_int zgsz3613;
                CREATE(sail_int)(&zgsz3613);
                CONVERT_OF(sail_int, mach_int)(&zgsz3613, ze);
                sail_int zgsz3614;
                CREATE(sail_int)(&zgsz3614);
                CONVERT_OF(sail_int, mach_int)(&zgsz3614, INT64_C(4));
                sail_int zgsz3615;
                CREATE(sail_int)(&zgsz3615);
                add_int(&zgsz3615, zgsz3613, zgsz3614);
                zgaz3254 = CONVERT_OF(mach_int, sail_int)(zgsz3615);
                KILL(sail_int)(&zgsz3615);
                KILL(sail_int)(&zgsz3614);
                KILL(sail_int)(&zgsz3613);
              }
              {
                lbits zgsz3610;
                CREATE(lbits)(&zgsz3610);
                CONVERT_OF(lbits, fbits)(&zgsz3610, ztop, UINT64_C(33) , true);
                sail_int zgsz3611;
                CREATE(sail_int)(&zgsz3611);
                CONVERT_OF(sail_int, mach_int)(&zgsz3611, zgaz3254);
                lbits zgsz3612;
                CREATE(lbits)(&zgsz3612);
                shiftr(&zgsz3612, zgsz3610, zgsz3611);
                zgaz3255 = CONVERT_OF(fbits, lbits)(zgsz3612, true);
                KILL(lbits)(&zgsz3612);
                KILL(sail_int)(&zgsz3611);
                KILL(lbits)(&zgsz3610);
              }
            }
            int64_t zgaz3256;
            {
              sail_int zgsz3619;
              CREATE(sail_int)(&zgsz3619);
              CONVERT_OF(sail_int, mach_int)(&zgsz3619, zcap_mantissa_width);
              sail_int zgsz3620;
              CREATE(sail_int)(&zgsz3620);
              CONVERT_OF(sail_int, mach_int)(&zgsz3620, INT64_C(3));
              sail_int zgsz3621;
              CREATE(sail_int)(&zgsz3621);
              sub_int(&zgsz3621, zgsz3619, zgsz3620);
              zgaz3256 = CONVERT_OF(mach_int, sail_int)(zgsz3621);
              KILL(sail_int)(&zgsz3621);
              KILL(sail_int)(&zgsz3620);
              KILL(sail_int)(&zgsz3619);
            }
            {
              lbits zgsz3616;
              CREATE(lbits)(&zgsz3616);
              CONVERT_OF(lbits, fbits)(&zgsz3616, zgaz3255, UINT64_C(33) , true);
              sail_int zgsz3617;
              CREATE(sail_int)(&zgsz3617);
              CONVERT_OF(sail_int, mach_int)(&zgsz3617, zgaz3256);
              lbits zgsz3618;
              CREATE(lbits)(&zgsz3618);
              sail_truncate(&zgsz3618, zgsz3616, zgsz3617);
              zgaz3257 = CONVERT_OF(fbits, lbits)(zgsz3618, true);
              KILL(lbits)(&zgsz3618);
              KILL(sail_int)(&zgsz3617);
              KILL(lbits)(&zgsz3616);
            }
          }
          {
            lbits zgsz3622;
            CREATE(lbits)(&zgsz3622);
            CONVERT_OF(lbits, fbits)(&zgsz3622, zgaz3257, UINT64_C(5) , true);
            sail_int zgsz3623;
            CREATE(sail_int)(&zgsz3623);
            CONVERT_OF(sail_int, mach_int)(&zgsz3623, zincT);
            lbits zgsz3624;
            CREATE(lbits)(&zgsz3624);
            add_bits_int(&zgsz3624, zgsz3622, zgsz3623);
            zT_ie = CONVERT_OF(fbits, lbits)(zgsz3624, true);
            KILL(lbits)(&zgsz3624);
            KILL(sail_int)(&zgsz3623);
            KILL(lbits)(&zgsz3622);
          }
          zgsz360 = UNIT;
        } else {  zgsz360 = UNIT;  }
      }
      {
        {
          lbits zgsz3625;
          CREATE(lbits)(&zgsz3625);
          CONVERT_OF(lbits, fbits)(&zgsz3625, zB_ie, UINT64_C(5) , true);
          lbits zgsz3626;
          CREATE(lbits)(&zgsz3626);
          CONVERT_OF(lbits, fbits)(&zgsz3626, UINT64_C(0b000), UINT64_C(3) , true);
          lbits zgsz3627;
          CREATE(lbits)(&zgsz3627);
          append(&zgsz3627, zgsz3625, zgsz3626);
          zBbits = CONVERT_OF(fbits, lbits)(zgsz3627, true);
          KILL(lbits)(&zgsz3627);
          KILL(lbits)(&zgsz3626);
          KILL(lbits)(&zgsz3625);
        }
        unit zgsz359;
        zgsz359 = UNIT;
      }
      {
        lbits zgsz3628;
        CREATE(lbits)(&zgsz3628);
        CONVERT_OF(lbits, fbits)(&zgsz3628, zT_ie, UINT64_C(5) , true);
        lbits zgsz3629;
        CREATE(lbits)(&zgsz3629);
        CONVERT_OF(lbits, fbits)(&zgsz3629, UINT64_C(0b000), UINT64_C(3) , true);
        lbits zgsz3630;
        CREATE(lbits)(&zgsz3630);
        append(&zgsz3630, zgsz3628, zgsz3629);
        zTbits = CONVERT_OF(fbits, lbits)(zgsz3630, true);
        KILL(lbits)(&zgsz3630);
        KILL(lbits)(&zgsz3629);
        KILL(lbits)(&zgsz3628);
      }
      zgsz355 = UNIT;
    } else {  zgsz355 = UNIT;  }
  }
  struct zCapability znewCap;
  {
    uint64_t zgaz3269;
    {
      int64_t zgaz3268;
      if (zincE) {
        {
          sail_int zgsz3634;
          CREATE(sail_int)(&zgsz3634);
          CONVERT_OF(sail_int, mach_int)(&zgsz3634, ze);
          sail_int zgsz3635;
          CREATE(sail_int)(&zgsz3635);
          CONVERT_OF(sail_int, mach_int)(&zgsz3635, INT64_C(1));
          sail_int zgsz3636;
          CREATE(sail_int)(&zgsz3636);
          add_int(&zgsz3636, zgsz3634, zgsz3635);
          zgaz3268 = CONVERT_OF(mach_int, sail_int)(zgsz3636);
          KILL(sail_int)(&zgsz3636);
          KILL(sail_int)(&zgsz3635);
          KILL(sail_int)(&zgsz3634);
        }
      } else {  zgaz3268 = ze;  }
      {
        sail_int zgsz3637;
        CREATE(sail_int)(&zgsz3637);
        CONVERT_OF(sail_int, mach_int)(&zgsz3637, INT64_C(6));
        sail_int zgsz3638;
        CREATE(sail_int)(&zgsz3638);
        CONVERT_OF(sail_int, mach_int)(&zgsz3638, zgaz3268);
        lbits zgsz3639;
        CREATE(lbits)(&zgsz3639);
        sailgen_to_bits(&zgsz3639, zgsz3637, zgsz3638);
        zgaz3269 = CONVERT_OF(fbits, lbits)(zgsz3639, true);
        KILL(lbits)(&zgsz3639);
        KILL(sail_int)(&zgsz3638);
        KILL(sail_int)(&zgsz3637);
      }
    }
    struct zCapability zgsz367;
    zgsz367 = zcap;
    zgsz367.zB = zBbits;
    zgsz367.zE = zgaz3269;
    zgsz367.zT = zTbits;
    zgsz367.zaddress = zbase;
    zgsz367.zinternal_E = zie;
    znewCap = zgsz367;
  }
  bool zexact;
  {
    bool zgaz3267;
    {
      bool zgsz368;
      if (zlostSignificantBase) {  zgsz368 = true;  } else {  zgsz368 = zlostSignificantTop;  }
      zgaz3267 = zgsz368;
    }
    zexact = not(zgaz3267);
  }
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgsz369;
  zgsz369.ztup0 = zexact;
  zgsz369.ztup1 = znewCap;
  zcbz326 = zgsz369;












end_function_116: ;
  return zcbz326;
end_block_exception_117: ;
  struct zCapability zcbz350 = { .zuperms = UINT64_C(0xdeadc0de), .ztag = false, .zreserved = UINT64_C(0xdeadc0de), .zpermit_unseal = false, .zpermit_store_local_cap = false, .zpermit_store_cap = false, .zpermit_store = false, .zpermit_set_CID = false, .zpermit_seal = false, .zpermit_load_cap = false, .zpermit_load = false, .zpermit_execute = false, .zpermit_cinvoke = false, .zotype = UINT64_C(0xdeadc0de), .zinternal_E = false, .zglobal = false, .zflag_cap_mode = false, .zaddress = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zT = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz349 = { .ztup1 = zcbz350, .ztup0 = false };
  return zcbz349;
}

static uint64_t sailgen_getCapPerms(struct zCapability);

static uint64_t sailgen_getCapPerms(struct zCapability zcap)
{
  __label__ end_function_119, end_block_exception_120;

  uint64_t zcbz327;
  uint64_t zperms;
  {
    uint64_t zgaz3277;
    zgaz3277 = sailgen_getCapHardPerms(zcap);
    {
      sail_int zgsz3652;
      CREATE(sail_int)(&zgsz3652);
      CONVERT_OF(sail_int, mach_int)(&zgsz3652, INT64_C(15));
      lbits zgsz3653;
      CREATE(lbits)(&zgsz3653);
      CONVERT_OF(lbits, fbits)(&zgsz3653, zgaz3277, UINT64_C(12) , true);
      lbits zgsz3654;
      CREATE(lbits)(&zgsz3654);
      sailgen_EXTZ(&zgsz3654, zgsz3652, zgsz3653);
      zperms = CONVERT_OF(fbits, lbits)(zgsz3654, true);
      KILL(lbits)(&zgsz3654);
      KILL(lbits)(&zgsz3653);
      KILL(sail_int)(&zgsz3652);
    }
  }
  uint64_t zgaz3276;
  zgaz3276 = zcap.zuperms;
  {
    lbits zgsz3655;
    CREATE(lbits)(&zgsz3655);
    CONVERT_OF(lbits, fbits)(&zgsz3655, zgaz3276, UINT64_C(0) , true);
    lbits zgsz3656;
    CREATE(lbits)(&zgsz3656);
    CONVERT_OF(lbits, fbits)(&zgsz3656, zperms, UINT64_C(15) , true);
    lbits zgsz3657;
    CREATE(lbits)(&zgsz3657);
    append(&zgsz3657, zgsz3655, zgsz3656);
    zcbz327 = CONVERT_OF(fbits, lbits)(zgsz3657, true);
    KILL(lbits)(&zgsz3657);
    KILL(lbits)(&zgsz3656);
    KILL(lbits)(&zgsz3655);
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
  uint64_t zgaz3279;
  {
    uint64_t zgaz3278;
    {
      lbits zgsz3661;
      CREATE(lbits)(&zgsz3661);
      CONVERT_OF(lbits, fbits)(&zgsz3661, zperms, UINT64_C(15) , true);
      sail_int zgsz3662;
      CREATE(sail_int)(&zgsz3662);
      CONVERT_OF(sail_int, mach_int)(&zgsz3662, zcap_uperms_shift);
      lbits zgsz3663;
      CREATE(lbits)(&zgsz3663);
      shiftr(&zgsz3663, zgsz3661, zgsz3662);
      zgaz3278 = CONVERT_OF(fbits, lbits)(zgsz3663, true);
      KILL(lbits)(&zgsz3663);
      KILL(sail_int)(&zgsz3662);
      KILL(lbits)(&zgsz3661);
    }
    {
      lbits zgsz3658;
      CREATE(lbits)(&zgsz3658);
      CONVERT_OF(lbits, fbits)(&zgsz3658, zgaz3278, UINT64_C(15) , true);
      sail_int zgsz3659;
      CREATE(sail_int)(&zgsz3659);
      CONVERT_OF(sail_int, mach_int)(&zgsz3659, zcap_uperms_width);
      lbits zgsz3660;
      CREATE(lbits)(&zgsz3660);
      sail_truncate(&zgsz3660, zgsz3658, zgsz3659);
      zgaz3279 = CONVERT_OF(fbits, lbits)(zgsz3660, true);
      KILL(lbits)(&zgsz3660);
      KILL(sail_int)(&zgsz3659);
      KILL(lbits)(&zgsz3658);
    }
  }
  bool zgaz3281;
  {
    fbits zgaz3280;
    {
      lbits zgsz3664;
      CREATE(lbits)(&zgsz3664);
      CONVERT_OF(lbits, fbits)(&zgsz3664, zperms, UINT64_C(15) , true);
      sail_int zgsz3665;
      CREATE(sail_int)(&zgsz3665);
      CONVERT_OF(sail_int, mach_int)(&zgsz3665, INT64_C(11));
      zgaz3280 = bitvector_access(zgsz3664, zgsz3665);
      KILL(sail_int)(&zgsz3665);
      KILL(lbits)(&zgsz3664);
    }
    zgaz3281 = sailgen_bit_to_bool(zgaz3280);
  }
  bool zgaz3283;
  {
    fbits zgaz3282;
    {
      lbits zgsz3666;
      CREATE(lbits)(&zgsz3666);
      CONVERT_OF(lbits, fbits)(&zgsz3666, zperms, UINT64_C(15) , true);
      sail_int zgsz3667;
      CREATE(sail_int)(&zgsz3667);
      CONVERT_OF(sail_int, mach_int)(&zgsz3667, INT64_C(10));
      zgaz3282 = bitvector_access(zgsz3666, zgsz3667);
      KILL(sail_int)(&zgsz3667);
      KILL(lbits)(&zgsz3666);
    }
    zgaz3283 = sailgen_bit_to_bool(zgaz3282);
  }
  bool zgaz3285;
  {
    fbits zgaz3284;
    {
      lbits zgsz3668;
      CREATE(lbits)(&zgsz3668);
      CONVERT_OF(lbits, fbits)(&zgsz3668, zperms, UINT64_C(15) , true);
      sail_int zgsz3669;
      CREATE(sail_int)(&zgsz3669);
      CONVERT_OF(sail_int, mach_int)(&zgsz3669, INT64_C(9));
      zgaz3284 = bitvector_access(zgsz3668, zgsz3669);
      KILL(sail_int)(&zgsz3669);
      KILL(lbits)(&zgsz3668);
    }
    zgaz3285 = sailgen_bit_to_bool(zgaz3284);
  }
  bool zgaz3287;
  {
    fbits zgaz3286;
    {
      lbits zgsz3670;
      CREATE(lbits)(&zgsz3670);
      CONVERT_OF(lbits, fbits)(&zgsz3670, zperms, UINT64_C(15) , true);
      sail_int zgsz3671;
      CREATE(sail_int)(&zgsz3671);
      CONVERT_OF(sail_int, mach_int)(&zgsz3671, INT64_C(8));
      zgaz3286 = bitvector_access(zgsz3670, zgsz3671);
      KILL(sail_int)(&zgsz3671);
      KILL(lbits)(&zgsz3670);
    }
    zgaz3287 = sailgen_bit_to_bool(zgaz3286);
  }
  bool zgaz3289;
  {
    fbits zgaz3288;
    {
      lbits zgsz3672;
      CREATE(lbits)(&zgsz3672);
      CONVERT_OF(lbits, fbits)(&zgsz3672, zperms, UINT64_C(15) , true);
      sail_int zgsz3673;
      CREATE(sail_int)(&zgsz3673);
      CONVERT_OF(sail_int, mach_int)(&zgsz3673, INT64_C(7));
      zgaz3288 = bitvector_access(zgsz3672, zgsz3673);
      KILL(sail_int)(&zgsz3673);
      KILL(lbits)(&zgsz3672);
    }
    zgaz3289 = sailgen_bit_to_bool(zgaz3288);
  }
  bool zgaz3291;
  {
    fbits zgaz3290;
    {
      lbits zgsz3674;
      CREATE(lbits)(&zgsz3674);
      CONVERT_OF(lbits, fbits)(&zgsz3674, zperms, UINT64_C(15) , true);
      sail_int zgsz3675;
      CREATE(sail_int)(&zgsz3675);
      CONVERT_OF(sail_int, mach_int)(&zgsz3675, INT64_C(6));
      zgaz3290 = bitvector_access(zgsz3674, zgsz3675);
      KILL(sail_int)(&zgsz3675);
      KILL(lbits)(&zgsz3674);
    }
    zgaz3291 = sailgen_bit_to_bool(zgaz3290);
  }
  bool zgaz3293;
  {
    fbits zgaz3292;
    {
      lbits zgsz3676;
      CREATE(lbits)(&zgsz3676);
      CONVERT_OF(lbits, fbits)(&zgsz3676, zperms, UINT64_C(15) , true);
      sail_int zgsz3677;
      CREATE(sail_int)(&zgsz3677);
      CONVERT_OF(sail_int, mach_int)(&zgsz3677, INT64_C(5));
      zgaz3292 = bitvector_access(zgsz3676, zgsz3677);
      KILL(sail_int)(&zgsz3677);
      KILL(lbits)(&zgsz3676);
    }
    zgaz3293 = sailgen_bit_to_bool(zgaz3292);
  }
  bool zgaz3295;
  {
    fbits zgaz3294;
    {
      lbits zgsz3678;
      CREATE(lbits)(&zgsz3678);
      CONVERT_OF(lbits, fbits)(&zgsz3678, zperms, UINT64_C(15) , true);
      sail_int zgsz3679;
      CREATE(sail_int)(&zgsz3679);
      CONVERT_OF(sail_int, mach_int)(&zgsz3679, INT64_C(4));
      zgaz3294 = bitvector_access(zgsz3678, zgsz3679);
      KILL(sail_int)(&zgsz3679);
      KILL(lbits)(&zgsz3678);
    }
    zgaz3295 = sailgen_bit_to_bool(zgaz3294);
  }
  bool zgaz3297;
  {
    fbits zgaz3296;
    {
      lbits zgsz3680;
      CREATE(lbits)(&zgsz3680);
      CONVERT_OF(lbits, fbits)(&zgsz3680, zperms, UINT64_C(15) , true);
      sail_int zgsz3681;
      CREATE(sail_int)(&zgsz3681);
      CONVERT_OF(sail_int, mach_int)(&zgsz3681, INT64_C(3));
      zgaz3296 = bitvector_access(zgsz3680, zgsz3681);
      KILL(sail_int)(&zgsz3681);
      KILL(lbits)(&zgsz3680);
    }
    zgaz3297 = sailgen_bit_to_bool(zgaz3296);
  }
  bool zgaz3299;
  {
    fbits zgaz3298;
    {
      lbits zgsz3682;
      CREATE(lbits)(&zgsz3682);
      CONVERT_OF(lbits, fbits)(&zgsz3682, zperms, UINT64_C(15) , true);
      sail_int zgsz3683;
      CREATE(sail_int)(&zgsz3683);
      CONVERT_OF(sail_int, mach_int)(&zgsz3683, INT64_C(2));
      zgaz3298 = bitvector_access(zgsz3682, zgsz3683);
      KILL(sail_int)(&zgsz3683);
      KILL(lbits)(&zgsz3682);
    }
    zgaz3299 = sailgen_bit_to_bool(zgaz3298);
  }
  bool zgaz3301;
  {
    fbits zgaz3300;
    {
      lbits zgsz3684;
      CREATE(lbits)(&zgsz3684);
      CONVERT_OF(lbits, fbits)(&zgsz3684, zperms, UINT64_C(15) , true);
      sail_int zgsz3685;
      CREATE(sail_int)(&zgsz3685);
      CONVERT_OF(sail_int, mach_int)(&zgsz3685, INT64_C(1));
      zgaz3300 = bitvector_access(zgsz3684, zgsz3685);
      KILL(sail_int)(&zgsz3685);
      KILL(lbits)(&zgsz3684);
    }
    zgaz3301 = sailgen_bit_to_bool(zgaz3300);
  }
  bool zgaz3303;
  {
    fbits zgaz3302;
    {
      lbits zgsz3686;
      CREATE(lbits)(&zgsz3686);
      CONVERT_OF(lbits, fbits)(&zgsz3686, zperms, UINT64_C(15) , true);
      sail_int zgsz3687;
      CREATE(sail_int)(&zgsz3687);
      CONVERT_OF(sail_int, mach_int)(&zgsz3687, INT64_C(0));
      zgaz3302 = bitvector_access(zgsz3686, zgsz3687);
      KILL(sail_int)(&zgsz3687);
      KILL(lbits)(&zgsz3686);
    }
    zgaz3303 = sailgen_bit_to_bool(zgaz3302);
  }
  struct zCapability zgsz370;
  zgsz370 = zcap;
  zgsz370.zaccess_system_regs = zgaz3283;
  zgsz370.zglobal = zgaz3303;
  zgsz370.zpermit_cinvoke = zgaz3287;
  zgsz370.zpermit_execute = zgaz3301;
  zgsz370.zpermit_load = zgaz3299;
  zgsz370.zpermit_load_cap = zgaz3295;
  zgsz370.zpermit_seal = zgaz3289;
  zgsz370.zpermit_set_CID = zgaz3281;
  zgsz370.zpermit_store = zgaz3297;
  zgsz370.zpermit_store_cap = zgaz3293;
  zgsz370.zpermit_store_local_cap = zgaz3291;
  zgsz370.zpermit_unseal = zgaz3285;
  zgsz370.zuperms = zgaz3279;
  zcbz328 = zgsz370;














end_function_122: ;
  return zcbz328;
end_block_exception_123: ;
  struct zCapability zcbz351 = { .zuperms = UINT64_C(0xdeadc0de), .ztag = false, .zreserved = UINT64_C(0xdeadc0de), .zpermit_unseal = false, .zpermit_store_local_cap = false, .zpermit_store_cap = false, .zpermit_store = false, .zpermit_set_CID = false, .zpermit_seal = false, .zpermit_load_cap = false, .zpermit_load = false, .zpermit_execute = false, .zpermit_cinvoke = false, .zotype = UINT64_C(0xdeadc0de), .zinternal_E = false, .zglobal = false, .zflag_cap_mode = false, .zaddress = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zT = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  return zcbz351;
}

static uint64_t sailgen_getCapFlags(struct zCapability);

static uint64_t sailgen_getCapFlags(struct zCapability zcap)
{
  __label__ end_function_125, end_block_exception_126;

  uint64_t zcbz329;
  bool zgaz3304;
  zgaz3304 = zcap.zflag_cap_mode;
  zcbz329 = sailgen_bool_to_bits(zgaz3304);

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
  int64_t zgaz3306;
  {
    uint64_t zgaz3305;
    zgaz3305 = zcap.zotype;
    {
      lbits zgsz3688;
      CREATE(lbits)(&zgsz3688);
      CONVERT_OF(lbits, fbits)(&zgsz3688, zgaz3305, UINT64_C(4) , true);
      sail_int zgsz3689;
      CREATE(sail_int)(&zgsz3689);
      sail_signed(&zgsz3689, zgsz3688);
      zgaz3306 = CONVERT_OF(mach_int, sail_int)(zgsz3689);
      KILL(sail_int)(&zgsz3689);
      KILL(lbits)(&zgsz3688);
    }
  }
  {
    sail_int zgsz3690;
    CREATE(sail_int)(&zgsz3690);
    CONVERT_OF(sail_int, mach_int)(&zgsz3690, zgaz3306);
    sail_int zgsz3691;
    CREATE(sail_int)(&zgsz3691);
    CONVERT_OF(sail_int, mach_int)(&zgsz3691, zotype_unsealed);
    zcbz330 = sailgen_neq_int(zgsz3690, zgsz3691);
    KILL(sail_int)(&zgsz3691);
    KILL(sail_int)(&zgsz3690);
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
  int64_t zgaz3308;
  {
    uint64_t zgaz3307;
    zgaz3307 = zcap.zotype;
    {
      lbits zgsz3692;
      CREATE(lbits)(&zgsz3692);
      CONVERT_OF(lbits, fbits)(&zgsz3692, zgaz3307, UINT64_C(4) , true);
      sail_int zgsz3693;
      CREATE(sail_int)(&zgsz3693);
      sail_unsigned(&zgsz3693, zgsz3692);
      zgaz3308 = CONVERT_OF(mach_int, sail_int)(zgsz3693);
      KILL(sail_int)(&zgsz3693);
      KILL(lbits)(&zgsz3692);
    }
  }
  {
    sail_int zgsz3694;
    CREATE(sail_int)(&zgsz3694);
    CONVERT_OF(sail_int, mach_int)(&zgsz3694, zgaz3308);
    sail_int zgsz3695;
    CREATE(sail_int)(&zgsz3695);
    CONVERT_OF(sail_int, mach_int)(&zgsz3695, zcap_max_otype);
    zcbz331 = gt(zgsz3694, zgsz3695);
    KILL(sail_int)(&zgsz3695);
    KILL(sail_int)(&zgsz3694);
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
  struct ztuple_z8z5bv32zCz0z5bv33z9 zgaz3309;
  zgaz3309 = sailgen_getCapBoundsBits(zc);
  uint64_t zgsz371;
  {
    uint64_t zbase;
    zbase = zgaz3309.ztup0;
    zgsz371 = zbase;
    goto finish_match_134;
  }
case_135: ;
  sail_match_failure("getCapBaseBits");
finish_match_134: ;
  zcbz332 = zgsz371;


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
  uint64_t zgaz3310;
  zgaz3310 = zc.zaddress;
  {
    lbits zgsz3696;
    CREATE(lbits)(&zgsz3696);
    CONVERT_OF(lbits, fbits)(&zgsz3696, zgaz3310, UINT64_C(32) , true);
    lbits zgsz3697;
    CREATE(lbits)(&zgsz3697);
    CONVERT_OF(lbits, fbits)(&zgsz3697, zbase, UINT64_C(32) , true);
    lbits zgsz3698;
    CREATE(lbits)(&zgsz3698);
    sub_bits(&zgsz3698, zgsz3696, zgsz3697);
    zcbz333 = CONVERT_OF(fbits, lbits)(zgsz3698, true);
    KILL(lbits)(&zgsz3698);
    KILL(lbits)(&zgsz3697);
    KILL(lbits)(&zgsz3696);
  }


end_function_139: ;
  return zcbz333;
end_block_exception_140: ;

  return UINT64_C(0xdeadc0de);
}

static int64_t sailgen_getCapLength(struct zCapability);

static int64_t sailgen_getCapLength(struct zCapability zc)
{
  __label__ case_143, finish_match_142, end_function_144, end_block_exception_145;

  int64_t zcbz334;
  struct ztuple_z8z5i64zCz0z5i64z9 zgaz3311;
  zgaz3311 = sailgen_getCapBounds(zc);
  int64_t zgsz373;
  {
    int64_t zbase;
    zbase = zgaz3311.ztup0;
    int64_t ztop;
    ztop = zgaz3311.ztup1;
    {
      bool zgaz3314;
      {
        bool zgaz3313;
        {
          bool zgaz3312;
          zgaz3312 = zc.ztag;
          zgaz3313 = not(zgaz3312);
        }
        bool zgsz374;
        if (zgaz3313) {  zgsz374 = true;  } else {
          {
            sail_int zgsz3699;
            CREATE(sail_int)(&zgsz3699);
            CONVERT_OF(sail_int, mach_int)(&zgsz3699, ztop);
            sail_int zgsz3700;
            CREATE(sail_int)(&zgsz3700);
            CONVERT_OF(sail_int, mach_int)(&zgsz3700, zbase);
            zgsz374 = gteq(zgsz3699, zgsz3700);
            KILL(sail_int)(&zgsz3700);
            KILL(sail_int)(&zgsz3699);
          }
        }
        zgaz3314 = zgsz374;
      }
      unit zgsz375;
      zgsz375 = sail_assert(zgaz3314, "src/cheri_cap_common.sail:453.40-453.41");
    }
    int64_t zgaz3315;
    {
      sail_int zgsz3706;
      CREATE(sail_int)(&zgsz3706);
      CONVERT_OF(sail_int, mach_int)(&zgsz3706, ztop);
      sail_int zgsz3707;
      CREATE(sail_int)(&zgsz3707);
      CONVERT_OF(sail_int, mach_int)(&zgsz3707, zbase);
      sail_int zgsz3708;
      CREATE(sail_int)(&zgsz3708);
      sub_int(&zgsz3708, zgsz3706, zgsz3707);
      zgaz3315 = CONVERT_OF(mach_int, sail_int)(zgsz3708);
      KILL(sail_int)(&zgsz3708);
      KILL(sail_int)(&zgsz3707);
      KILL(sail_int)(&zgsz3706);
    }
    int64_t zgaz3316;
    {
      sail_int zgsz3704;
      CREATE(sail_int)(&zgsz3704);
      CONVERT_OF(sail_int, mach_int)(&zgsz3704, zcap_len_width);
      sail_int zgsz3705;
      CREATE(sail_int)(&zgsz3705);
      pow2(&zgsz3705, zgsz3704);
      zgaz3316 = CONVERT_OF(mach_int, sail_int)(zgsz3705);
      KILL(sail_int)(&zgsz3705);
      KILL(sail_int)(&zgsz3704);
    }
    {
      sail_int zgsz3701;
      CREATE(sail_int)(&zgsz3701);
      CONVERT_OF(sail_int, mach_int)(&zgsz3701, zgaz3315);
      sail_int zgsz3702;
      CREATE(sail_int)(&zgsz3702);
      CONVERT_OF(sail_int, mach_int)(&zgsz3702, zgaz3316);
      sail_int zgsz3703;
      CREATE(sail_int)(&zgsz3703);
      emod_int(&zgsz3703, zgsz3701, zgsz3702);
      zgsz373 = CONVERT_OF(mach_int, sail_int)(zgsz3703);
      KILL(sail_int)(&zgsz3703);
      KILL(sail_int)(&zgsz3702);
      KILL(sail_int)(&zgsz3701);
    }
    goto finish_match_142;
  }
case_143: ;
  sail_match_failure("getCapLength");
finish_match_142: ;
  zcbz334 = zgsz373;


end_function_144: ;
  return zcbz334;
end_block_exception_145: ;

  return INT64_C(0xdeadc0de);
}

static bool sailgen_fastRepCheck(struct zCapability, uint64_t);

static bool sailgen_fastRepCheck(struct zCapability zc, uint64_t zi)
{
  __label__ end_function_147, end_block_exception_148;

  bool zcbz335;
  int64_t zE;
  {
    uint64_t zgaz3330;
    zgaz3330 = zc.zE;
    {
      lbits zgsz3709;
      CREATE(lbits)(&zgsz3709);
      CONVERT_OF(lbits, fbits)(&zgsz3709, zgaz3330, UINT64_C(6) , true);
      sail_int zgsz3710;
      CREATE(sail_int)(&zgsz3710);
      sail_unsigned(&zgsz3710, zgsz3709);
      zE = CONVERT_OF(mach_int, sail_int)(zgsz3710);
      KILL(sail_int)(&zgsz3710);
      KILL(lbits)(&zgsz3709);
    }
  }
  bool zgaz3318;
  {
    int64_t zgaz3317;
    {
      sail_int zgsz3713;
      CREATE(sail_int)(&zgsz3713);
      CONVERT_OF(sail_int, mach_int)(&zgsz3713, zcap_max_E);
      sail_int zgsz3714;
      CREATE(sail_int)(&zgsz3714);
      CONVERT_OF(sail_int, mach_int)(&zgsz3714, INT64_C(2));
      sail_int zgsz3715;
      CREATE(sail_int)(&zgsz3715);
      sub_int(&zgsz3715, zgsz3713, zgsz3714);
      zgaz3317 = CONVERT_OF(mach_int, sail_int)(zgsz3715);
      KILL(sail_int)(&zgsz3715);
      KILL(sail_int)(&zgsz3714);
      KILL(sail_int)(&zgsz3713);
    }
    {
      sail_int zgsz3711;
      CREATE(sail_int)(&zgsz3711);
      CONVERT_OF(sail_int, mach_int)(&zgsz3711, zE);
      sail_int zgsz3712;
      CREATE(sail_int)(&zgsz3712);
      CONVERT_OF(sail_int, mach_int)(&zgsz3712, zgaz3317);
      zgaz3318 = gteq(zgsz3711, zgsz3712);
      KILL(sail_int)(&zgsz3712);
      KILL(sail_int)(&zgsz3711);
    }
  }
  if (zgaz3318) {  zcbz335 = true;  } else {
    int64_t zi_top;
    {
      uint64_t zgaz3329;
      {
        int64_t zgaz3328;
        {
          sail_int zgsz3719;
          CREATE(sail_int)(&zgsz3719);
          CONVERT_OF(sail_int, mach_int)(&zgsz3719, zE);
          sail_int zgsz3720;
          CREATE(sail_int)(&zgsz3720);
          CONVERT_OF(sail_int, mach_int)(&zgsz3720, zcap_mantissa_width);
          sail_int zgsz3721;
          CREATE(sail_int)(&zgsz3721);
          add_int(&zgsz3721, zgsz3719, zgsz3720);
          zgaz3328 = CONVERT_OF(mach_int, sail_int)(zgsz3721);
          KILL(sail_int)(&zgsz3721);
          KILL(sail_int)(&zgsz3720);
          KILL(sail_int)(&zgsz3719);
        }
        {
          lbits zgsz3716;
          CREATE(lbits)(&zgsz3716);
          CONVERT_OF(lbits, fbits)(&zgsz3716, zi, UINT64_C(32) , true);
          sail_int zgsz3717;
          CREATE(sail_int)(&zgsz3717);
          CONVERT_OF(sail_int, mach_int)(&zgsz3717, zgaz3328);
          lbits zgsz3718;
          CREATE(lbits)(&zgsz3718);
          arith_shiftr(&zgsz3718, zgsz3716, zgsz3717);
          zgaz3329 = CONVERT_OF(fbits, lbits)(zgsz3718, true);
          KILL(lbits)(&zgsz3718);
          KILL(sail_int)(&zgsz3717);
          KILL(lbits)(&zgsz3716);
        }
      }
      {
        lbits zgsz3722;
        CREATE(lbits)(&zgsz3722);
        CONVERT_OF(lbits, fbits)(&zgsz3722, zgaz3329, UINT64_C(32) , true);
        sail_int zgsz3723;
        CREATE(sail_int)(&zgsz3723);
        sail_signed(&zgsz3723, zgsz3722);
        zi_top = CONVERT_OF(mach_int, sail_int)(zgsz3723);
        KILL(sail_int)(&zgsz3723);
        KILL(lbits)(&zgsz3722);
      }
    }
    uint64_t zi_mid;
    {
      uint64_t zgaz3327;
      {
        lbits zgsz3727;
        CREATE(lbits)(&zgsz3727);
        CONVERT_OF(lbits, fbits)(&zgsz3727, zi, UINT64_C(32) , true);
        sail_int zgsz3728;
        CREATE(sail_int)(&zgsz3728);
        CONVERT_OF(sail_int, mach_int)(&zgsz3728, zE);
        lbits zgsz3729;
        CREATE(lbits)(&zgsz3729);
        shiftr(&zgsz3729, zgsz3727, zgsz3728);
        zgaz3327 = CONVERT_OF(fbits, lbits)(zgsz3729, true);
        KILL(lbits)(&zgsz3729);
        KILL(sail_int)(&zgsz3728);
        KILL(lbits)(&zgsz3727);
      }
      {
        lbits zgsz3724;
        CREATE(lbits)(&zgsz3724);
        CONVERT_OF(lbits, fbits)(&zgsz3724, zgaz3327, UINT64_C(32) , true);
        sail_int zgsz3725;
        CREATE(sail_int)(&zgsz3725);
        CONVERT_OF(sail_int, mach_int)(&zgsz3725, zcap_mantissa_width);
        lbits zgsz3726;
        CREATE(lbits)(&zgsz3726);
        sail_truncate(&zgsz3726, zgsz3724, zgsz3725);
        zi_mid = CONVERT_OF(fbits, lbits)(zgsz3726, true);
        KILL(lbits)(&zgsz3726);
        KILL(sail_int)(&zgsz3725);
        KILL(lbits)(&zgsz3724);
      }
    }
    uint64_t za_mid;
    {
      uint64_t zgaz3326;
      {
        uint64_t zgaz3325;
        zgaz3325 = zc.zaddress;
        {
          lbits zgsz3730;
          CREATE(lbits)(&zgsz3730);
          CONVERT_OF(lbits, fbits)(&zgsz3730, zgaz3325, UINT64_C(32) , true);
          sail_int zgsz3731;
          CREATE(sail_int)(&zgsz3731);
          CONVERT_OF(sail_int, mach_int)(&zgsz3731, zE);
          lbits zgsz3732;
          CREATE(lbits)(&zgsz3732);
          shiftr(&zgsz3732, zgsz3730, zgsz3731);
          zgaz3326 = CONVERT_OF(fbits, lbits)(zgsz3732, true);
          KILL(lbits)(&zgsz3732);
          KILL(sail_int)(&zgsz3731);
          KILL(lbits)(&zgsz3730);
        }
      }
      {
        lbits zgsz3733;
        CREATE(lbits)(&zgsz3733);
        CONVERT_OF(lbits, fbits)(&zgsz3733, zgaz3326, UINT64_C(32) , true);
        sail_int zgsz3734;
        CREATE(sail_int)(&zgsz3734);
        CONVERT_OF(sail_int, mach_int)(&zgsz3734, zcap_mantissa_width);
        lbits zgsz3735;
        CREATE(lbits)(&zgsz3735);
        sail_truncate(&zgsz3735, zgsz3733, zgsz3734);
        za_mid = CONVERT_OF(fbits, lbits)(zgsz3735, true);
        KILL(lbits)(&zgsz3735);
        KILL(sail_int)(&zgsz3734);
        KILL(lbits)(&zgsz3733);
      }
    }
    uint64_t zB3;
    {
      uint64_t zgaz3324;
      zgaz3324 = zc.zB;
      {
        lbits zgsz3736;
        CREATE(lbits)(&zgsz3736);
        CONVERT_OF(lbits, fbits)(&zgsz3736, zgaz3324, UINT64_C(8) , true);
        sail_int zgsz3737;
        CREATE(sail_int)(&zgsz3737);
        CONVERT_OF(sail_int, mach_int)(&zgsz3737, INT64_C(3));
        lbits zgsz3738;
        CREATE(lbits)(&zgsz3738);
        sail_truncateLSB(&zgsz3738, zgsz3736, zgsz3737);
        zB3 = CONVERT_OF(fbits, lbits)(zgsz3738, true);
        KILL(lbits)(&zgsz3738);
        KILL(sail_int)(&zgsz3737);
        KILL(lbits)(&zgsz3736);
      }
    }
    uint64_t zR3;
    {
      lbits zgsz3763;
      CREATE(lbits)(&zgsz3763);
      CONVERT_OF(lbits, fbits)(&zgsz3763, zB3, UINT64_C(3) , true);
      lbits zgsz3764;
      CREATE(lbits)(&zgsz3764);
      CONVERT_OF(lbits, fbits)(&zgsz3764, UINT64_C(0b001), UINT64_C(3) , true);
      lbits zgsz3765;
      CREATE(lbits)(&zgsz3765);
      sub_bits(&zgsz3765, zgsz3763, zgsz3764);
      zR3 = CONVERT_OF(fbits, lbits)(zgsz3765, true);
      KILL(lbits)(&zgsz3765);
      KILL(lbits)(&zgsz3764);
      KILL(lbits)(&zgsz3763);
    }
    uint64_t zR;
    {
      uint64_t zgaz3323;
      {
        int64_t zgaz3322;
        {
          sail_int zgsz3741;
          CREATE(sail_int)(&zgsz3741);
          CONVERT_OF(sail_int, mach_int)(&zgsz3741, zcap_mantissa_width);
          sail_int zgsz3742;
          CREATE(sail_int)(&zgsz3742);
          CONVERT_OF(sail_int, mach_int)(&zgsz3742, INT64_C(3));
          sail_int zgsz3743;
          CREATE(sail_int)(&zgsz3743);
          sub_int(&zgsz3743, zgsz3741, zgsz3742);
          zgaz3322 = CONVERT_OF(mach_int, sail_int)(zgsz3743);
          KILL(sail_int)(&zgsz3743);
          KILL(sail_int)(&zgsz3742);
          KILL(sail_int)(&zgsz3741);
        }
        {
          sail_int zgsz3739;
          CREATE(sail_int)(&zgsz3739);
          CONVERT_OF(sail_int, mach_int)(&zgsz3739, zgaz3322);
          lbits zgsz3740;
          CREATE(lbits)(&zgsz3740);
          sailgen_zzeros_implicit(&zgsz3740, zgsz3739);
          zgaz3323 = CONVERT_OF(fbits, lbits)(zgsz3740, true);
          KILL(lbits)(&zgsz3740);
          KILL(sail_int)(&zgsz3739);
        }
      }
      {
        lbits zgsz3744;
        CREATE(lbits)(&zgsz3744);
        CONVERT_OF(lbits, fbits)(&zgsz3744, zR3, UINT64_C(3) , true);
        lbits zgsz3745;
        CREATE(lbits)(&zgsz3745);
        CONVERT_OF(lbits, fbits)(&zgsz3745, zgaz3323, UINT64_C(5) , true);
        lbits zgsz3746;
        CREATE(lbits)(&zgsz3746);
        append(&zgsz3746, zgsz3744, zgsz3745);
        zR = CONVERT_OF(fbits, lbits)(zgsz3746, true);
        KILL(lbits)(&zgsz3746);
        KILL(lbits)(&zgsz3745);
        KILL(lbits)(&zgsz3744);
      }
    }
    uint64_t zdiff;
    {
      lbits zgsz3760;
      CREATE(lbits)(&zgsz3760);
      CONVERT_OF(lbits, fbits)(&zgsz3760, zR, UINT64_C(8) , true);
      lbits zgsz3761;
      CREATE(lbits)(&zgsz3761);
      CONVERT_OF(lbits, fbits)(&zgsz3761, za_mid, UINT64_C(8) , true);
      lbits zgsz3762;
      CREATE(lbits)(&zgsz3762);
      sub_bits(&zgsz3762, zgsz3760, zgsz3761);
      zdiff = CONVERT_OF(fbits, lbits)(zgsz3762, true);
      KILL(lbits)(&zgsz3762);
      KILL(lbits)(&zgsz3761);
      KILL(lbits)(&zgsz3760);
    }
    uint64_t zdiff1;
    {
      lbits zgsz3757;
      CREATE(lbits)(&zgsz3757);
      CONVERT_OF(lbits, fbits)(&zgsz3757, zdiff, UINT64_C(8) , true);
      sail_int zgsz3758;
      CREATE(sail_int)(&zgsz3758);
      CONVERT_OF(sail_int, mach_int)(&zgsz3758, INT64_C(1));
      lbits zgsz3759;
      CREATE(lbits)(&zgsz3759);
      sub_bits_int(&zgsz3759, zgsz3757, zgsz3758);
      zdiff1 = CONVERT_OF(fbits, lbits)(zgsz3759, true);
      KILL(lbits)(&zgsz3759);
      KILL(sail_int)(&zgsz3758);
      KILL(lbits)(&zgsz3757);
    }
    bool zgaz3319;
    {
      sail_int zgsz3755;
      CREATE(sail_int)(&zgsz3755);
      CONVERT_OF(sail_int, mach_int)(&zgsz3755, zi_top);
      sail_int zgsz3756;
      CREATE(sail_int)(&zgsz3756);
      CONVERT_OF(sail_int, mach_int)(&zgsz3756, INT64_C(0));
      zgaz3319 = eq_int(zgsz3755, zgsz3756);
      KILL(sail_int)(&zgsz3756);
      KILL(sail_int)(&zgsz3755);
    }
    if (zgaz3319) {
      {
        lbits zgsz3753;
        CREATE(lbits)(&zgsz3753);
        CONVERT_OF(lbits, fbits)(&zgsz3753, zi_mid, UINT64_C(8) , true);
        lbits zgsz3754;
        CREATE(lbits)(&zgsz3754);
        CONVERT_OF(lbits, fbits)(&zgsz3754, zdiff1, UINT64_C(8) , true);
        zcbz335 = sailgen_z8operatorz0zI_uz9(zgsz3753, zgsz3754);
        KILL(lbits)(&zgsz3754);
        KILL(lbits)(&zgsz3753);
      }
    } else {
      bool zgaz3320;
      {
        sail_int zgsz3751;
        CREATE(sail_int)(&zgsz3751);
        CONVERT_OF(sail_int, mach_int)(&zgsz3751, zi_top);
        sail_int zgsz3752;
        CREATE(sail_int)(&zgsz3752);
        CONVERT_OF(sail_int, mach_int)(&zgsz3752, INT64_C(-1));
        zgaz3320 = eq_int(zgsz3751, zgsz3752);
        KILL(sail_int)(&zgsz3752);
        KILL(sail_int)(&zgsz3751);
      }
      if (zgaz3320) {
        bool zgaz3321;
        {
          lbits zgsz3749;
          CREATE(lbits)(&zgsz3749);
          CONVERT_OF(lbits, fbits)(&zgsz3749, zi_mid, UINT64_C(8) , true);
          lbits zgsz3750;
          CREATE(lbits)(&zgsz3750);
          CONVERT_OF(lbits, fbits)(&zgsz3750, zdiff, UINT64_C(8) , true);
          zgaz3321 = sailgen_z8operatorz0zKzJ_uz9(zgsz3749, zgsz3750);
          KILL(lbits)(&zgsz3750);
          KILL(lbits)(&zgsz3749);
        }
        bool zgsz377;
        if (zgaz3321) {
          {
            lbits zgsz3747;
            CREATE(lbits)(&zgsz3747);
            CONVERT_OF(lbits, fbits)(&zgsz3747, zR, UINT64_C(8) , true);
            lbits zgsz3748;
            CREATE(lbits)(&zgsz3748);
            CONVERT_OF(lbits, fbits)(&zgsz3748, za_mid, UINT64_C(8) , true);
            zgsz377 = neq_bits(zgsz3747, zgsz3748);
            KILL(lbits)(&zgsz3748);
            KILL(lbits)(&zgsz3747);
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

  int64_t zlen;
  zlen = sailgen_getCapLength(zcap);
  sail_string zlen_str;
  CREATE(sail_string)(&zlen_str);
  {
    uint64_t zgaz3359;
    {
      int64_t zgaz3358;
      {
        sail_int zgsz3769;
        CREATE(sail_int)(&zgsz3769);
        CONVERT_OF(sail_int, mach_int)(&zgsz3769, zcap_len_width);
        sail_int zgsz3770;
        CREATE(sail_int)(&zgsz3770);
        CONVERT_OF(sail_int, mach_int)(&zgsz3770, INT64_C(3));
        sail_int zgsz3771;
        CREATE(sail_int)(&zgsz3771);
        add_int(&zgsz3771, zgsz3769, zgsz3770);
        zgaz3358 = CONVERT_OF(mach_int, sail_int)(zgsz3771);
        KILL(sail_int)(&zgsz3771);
        KILL(sail_int)(&zgsz3770);
        KILL(sail_int)(&zgsz3769);
      }
      {
        sail_int zgsz3766;
        CREATE(sail_int)(&zgsz3766);
        CONVERT_OF(sail_int, mach_int)(&zgsz3766, zgaz3358);
        sail_int zgsz3767;
        CREATE(sail_int)(&zgsz3767);
        CONVERT_OF(sail_int, mach_int)(&zgsz3767, zlen);
        lbits zgsz3768;
        CREATE(lbits)(&zgsz3768);
        sailgen_to_bits(&zgsz3768, zgsz3766, zgsz3767);
        zgaz3359 = CONVERT_OF(fbits, lbits)(zgsz3768, true);
        KILL(lbits)(&zgsz3768);
        KILL(sail_int)(&zgsz3767);
        KILL(sail_int)(&zgsz3766);
      }
    }
    {
      lbits zgsz3772;
      CREATE(lbits)(&zgsz3772);
      CONVERT_OF(lbits, fbits)(&zgsz3772, zgaz3359, UINT64_C(36) , true);
      string_of_lbits(&zlen_str, zgsz3772);
      KILL(lbits)(&zgsz3772);
    }
  }
  uint64_t zotype64;
  {
    bool zgaz3355;
    zgaz3355 = sailgen_hasReservedOType(zcap);
    if (zgaz3355) {
      uint64_t zgaz3356;
      zgaz3356 = zcap.zotype;
      {
        sail_int zgsz3776;
        CREATE(sail_int)(&zgsz3776);
        CONVERT_OF(sail_int, mach_int)(&zgsz3776, INT64_C(32));
        lbits zgsz3777;
        CREATE(lbits)(&zgsz3777);
        CONVERT_OF(lbits, fbits)(&zgsz3777, zgaz3356, UINT64_C(4) , true);
        lbits zgsz3778;
        CREATE(lbits)(&zgsz3778);
        sailgen_EXTS(&zgsz3778, zgsz3776, zgsz3777);
        zotype64 = CONVERT_OF(fbits, lbits)(zgsz3778, true);
        KILL(lbits)(&zgsz3778);
        KILL(lbits)(&zgsz3777);
        KILL(sail_int)(&zgsz3776);
      }
    } else {
      uint64_t zgaz3357;
      zgaz3357 = zcap.zotype;
      {
        sail_int zgsz3773;
        CREATE(sail_int)(&zgsz3773);
        CONVERT_OF(sail_int, mach_int)(&zgsz3773, INT64_C(32));
        lbits zgsz3774;
        CREATE(lbits)(&zgsz3774);
        CONVERT_OF(lbits, fbits)(&zgsz3774, zgaz3357, UINT64_C(4) , true);
        lbits zgsz3775;
        CREATE(lbits)(&zgsz3775);
        sailgen_EXTZ(&zgsz3775, zgsz3773, zgsz3774);
        zotype64 = CONVERT_OF(fbits, lbits)(zgsz3775, true);
        KILL(lbits)(&zgsz3775);
        KILL(lbits)(&zgsz3774);
        KILL(sail_int)(&zgsz3773);
      }
    }
  }
  sail_string zgaz3354;
  CREATE(sail_string)(&zgaz3354);
  {
    sail_string zgaz3352;
    CREATE(sail_string)(&zgaz3352);
    {
      bool zgaz3331;
      zgaz3331 = zcap.ztag;
      if (zgaz3331) {  COPY(sail_string)(&zgaz3352, "1");  } else {  COPY(sail_string)(&zgaz3352, "0");  }
    }
    sail_string zgaz3353;
    CREATE(sail_string)(&zgaz3353);
    {
      sail_string zgaz3351;
      CREATE(sail_string)(&zgaz3351);
      {
        sail_string zgaz3349;
        CREATE(sail_string)(&zgaz3349);
        {
          bool zgaz3332;
          zgaz3332 = sailgen_isCapSealed(zcap);
          if (zgaz3332) {  COPY(sail_string)(&zgaz3349, "1");  } else {  COPY(sail_string)(&zgaz3349, "0");  }
        }
        sail_string zgaz3350;
        CREATE(sail_string)(&zgaz3350);
        {
          sail_string zgaz3348;
          CREATE(sail_string)(&zgaz3348);
          {
            sail_string zgaz3346;
            CREATE(sail_string)(&zgaz3346);
            {
              uint64_t zgaz3334;
              {
                uint64_t zgaz3333;
                zgaz3333 = sailgen_getCapPerms(zcap);
                {
                  lbits zgsz3779;
                  CREATE(lbits)(&zgsz3779);
                  CONVERT_OF(lbits, fbits)(&zgsz3779, UINT64_C(0b0), UINT64_C(1) , true);
                  lbits zgsz3780;
                  CREATE(lbits)(&zgsz3780);
                  CONVERT_OF(lbits, fbits)(&zgsz3780, zgaz3333, UINT64_C(15) , true);
                  lbits zgsz3781;
                  CREATE(lbits)(&zgsz3781);
                  append(&zgsz3781, zgsz3779, zgsz3780);
                  zgaz3334 = CONVERT_OF(fbits, lbits)(zgsz3781, true);
                  KILL(lbits)(&zgsz3781);
                  KILL(lbits)(&zgsz3780);
                  KILL(lbits)(&zgsz3779);
                }
              }
              {
                lbits zgsz3782;
                CREATE(lbits)(&zgsz3782);
                CONVERT_OF(lbits, fbits)(&zgsz3782, zgaz3334, UINT64_C(16) , true);
                string_of_lbits(&zgaz3346, zgsz3782);
                KILL(lbits)(&zgsz3782);
              }
            }
            sail_string zgaz3347;
            CREATE(sail_string)(&zgaz3347);
            {
              sail_string zgaz3345;
              CREATE(sail_string)(&zgaz3345);
              {
                sail_string zgaz3343;
                CREATE(sail_string)(&zgaz3343);
                {
                  lbits zgsz3785;
                  CREATE(lbits)(&zgsz3785);
                  CONVERT_OF(lbits, fbits)(&zgsz3785, zotype64, UINT64_C(32) , true);
                  string_of_lbits(&zgaz3343, zgsz3785);
                  KILL(lbits)(&zgsz3785);
                }
                sail_string zgaz3344;
                CREATE(sail_string)(&zgaz3344);
                {
                  sail_string zgaz3342;
                  CREATE(sail_string)(&zgaz3342);
                  {
                    sail_string zgaz3340;
                    CREATE(sail_string)(&zgaz3340);
                    {
                      uint64_t zgaz3335;
                      zgaz3335 = sailgen_getCapOffsetBits(zcap);
                      {
                        lbits zgsz3783;
                        CREATE(lbits)(&zgsz3783);
                        CONVERT_OF(lbits, fbits)(&zgsz3783, zgaz3335, UINT64_C(32) , true);
                        string_of_lbits(&zgaz3340, zgsz3783);
                        KILL(lbits)(&zgsz3783);
                      }
                    }
                    sail_string zgaz3341;
                    CREATE(sail_string)(&zgaz3341);
                    {
                      sail_string zgaz3339;
                      CREATE(sail_string)(&zgaz3339);
                      {
                        sail_string zgaz3337;
                        CREATE(sail_string)(&zgaz3337);
                        {
                          uint64_t zgaz3336;
                          zgaz3336 = sailgen_getCapBaseBits(zcap);
                          {
                            lbits zgsz3784;
                            CREATE(lbits)(&zgsz3784);
                            CONVERT_OF(lbits, fbits)(&zgsz3784, zgaz3336, UINT64_C(32) , true);
                            string_of_lbits(&zgaz3337, zgsz3784);
                            KILL(lbits)(&zgsz3784);
                          }
                        }
                        sail_string zgaz3338;
                        CREATE(sail_string)(&zgaz3338);
                        concat_str(&zgaz3338, " length:", zlen_str);
                        concat_str(&zgaz3339, zgaz3337, zgaz3338);
                        KILL(sail_string)(&zgaz3338);
                        KILL(sail_string)(&zgaz3337);
                      }
                      concat_str(&zgaz3341, " base:", zgaz3339);
                      KILL(sail_string)(&zgaz3339);
                    }
                    concat_str(&zgaz3342, zgaz3340, zgaz3341);
                    KILL(sail_string)(&zgaz3341);
                    KILL(sail_string)(&zgaz3340);
                  }
                  concat_str(&zgaz3344, " offset:", zgaz3342);
                  KILL(sail_string)(&zgaz3342);
                }
                concat_str(&zgaz3345, zgaz3343, zgaz3344);
                KILL(sail_string)(&zgaz3344);
                KILL(sail_string)(&zgaz3343);
              }
              concat_str(&zgaz3347, " type:", zgaz3345);
              KILL(sail_string)(&zgaz3345);
            }
            concat_str(&zgaz3348, zgaz3346, zgaz3347);
            KILL(sail_string)(&zgaz3347);
            KILL(sail_string)(&zgaz3346);
          }
          concat_str(&zgaz3350, " perms:", zgaz3348);
          KILL(sail_string)(&zgaz3348);
        }
        concat_str(&zgaz3351, zgaz3349, zgaz3350);
        KILL(sail_string)(&zgaz3350);
        KILL(sail_string)(&zgaz3349);
      }
      concat_str(&zgaz3353, " s:", zgaz3351);
      KILL(sail_string)(&zgaz3351);
    }
    concat_str(&zgaz3354, zgaz3352, zgaz3353);
    KILL(sail_string)(&zgaz3353);
    KILL(sail_string)(&zgaz3352);
  }
  concat_str((*(&zcbz336)), " t:", zgaz3354);
  KILL(sail_string)(&zgaz3354);

  KILL(sail_string)(&zlen_str);

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
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3362;
  {
    uint64_t zgaz3360;
    {
      sail_int zgsz3789;
      CREATE(sail_int)(&zgsz3789);
      CONVERT_OF(sail_int, mach_int)(&zgsz3789, INT64_C(32));
      sail_int zgsz3790;
      CREATE(sail_int)(&zgsz3790);
      CONVERT_OF(sail_int, mach_int)(&zgsz3790, INT64_C(0));
      lbits zgsz3791;
      CREATE(lbits)(&zgsz3791);
      sailgen_to_bits(&zgsz3791, zgsz3789, zgsz3790);
      zgaz3360 = CONVERT_OF(fbits, lbits)(zgsz3791, true);
      KILL(lbits)(&zgsz3791);
      KILL(sail_int)(&zgsz3790);
      KILL(sail_int)(&zgsz3789);
    }
    uint64_t zgaz3361;
    {
      lbits zgsz3786;
      CREATE(lbits)(&zgsz3786);
      CONVERT_OF(lbits, fbits)(&zgsz3786, UINT64_C(0b0), UINT64_C(1) , true);
      lbits zgsz3787;
      CREATE(lbits)(&zgsz3787);
      CONVERT_OF(lbits, fbits)(&zgsz3787, zlen, UINT64_C(32) , true);
      lbits zgsz3788;
      CREATE(lbits)(&zgsz3788);
      append(&zgsz3788, zgsz3786, zgsz3787);
      zgaz3361 = CONVERT_OF(fbits, lbits)(zgsz3788, true);
      KILL(lbits)(&zgsz3788);
      KILL(lbits)(&zgsz3787);
      KILL(lbits)(&zgsz3786);
    }
    zgaz3362 = sailgen_setCapBounds(zdefault_cap, zgaz3360, zgaz3361);
  }
  uint64_t zgsz378;
  {
    bool zexact;
    zexact = zgaz3362.ztup0;
    struct zCapability zc;
    zc = zgaz3362.ztup1;
    int64_t ze;
    {
      int64_t zgaz3368;
      {
        uint64_t zgaz3367;
        zgaz3367 = zc.zE;
        {
          lbits zgsz3792;
          CREATE(lbits)(&zgsz3792);
          CONVERT_OF(lbits, fbits)(&zgsz3792, zgaz3367, UINT64_C(6) , true);
          sail_int zgsz3793;
          CREATE(sail_int)(&zgsz3793);
          sail_unsigned(&zgsz3793, zgsz3792);
          zgaz3368 = CONVERT_OF(mach_int, sail_int)(zgsz3793);
          KILL(sail_int)(&zgsz3793);
          KILL(lbits)(&zgsz3792);
        }
      }
      {
        sail_int zgsz3794;
        CREATE(sail_int)(&zgsz3794);
        CONVERT_OF(sail_int, mach_int)(&zgsz3794, zgaz3368);
        sail_int zgsz3795;
        CREATE(sail_int)(&zgsz3795);
        CONVERT_OF(sail_int, mach_int)(&zgsz3795, zcap_max_E);
        sail_int zgsz3796;
        CREATE(sail_int)(&zgsz3796);
        min_int(&zgsz3796, zgsz3794, zgsz3795);
        ze = CONVERT_OF(mach_int, sail_int)(zgsz3796);
        KILL(sail_int)(&zgsz3796);
        KILL(sail_int)(&zgsz3795);
        KILL(sail_int)(&zgsz3794);
      }
    }
    int64_t zez7;
    {
      bool zgaz3366;
      zgaz3366 = zc.zinternal_E;
      if (zgaz3366) {
        {
          sail_int zgsz3797;
          CREATE(sail_int)(&zgsz3797);
          CONVERT_OF(sail_int, mach_int)(&zgsz3797, ze);
          sail_int zgsz3798;
          CREATE(sail_int)(&zgsz3798);
          CONVERT_OF(sail_int, mach_int)(&zgsz3798, zinternal_E_take_bits);
          sail_int zgsz3799;
          CREATE(sail_int)(&zgsz3799);
          add_int(&zgsz3799, zgsz3797, zgsz3798);
          zez7 = CONVERT_OF(mach_int, sail_int)(zgsz3799);
          KILL(sail_int)(&zgsz3799);
          KILL(sail_int)(&zgsz3798);
          KILL(sail_int)(&zgsz3797);
        }
      } else {  zez7 = INT64_C(0);  }
    }
    sbits zgaz3364;
    {
      int64_t zgaz3363;
      {
        sail_int zgsz3802;
        CREATE(sail_int)(&zgsz3802);
        CONVERT_OF(sail_int, mach_int)(&zgsz3802, INT64_C(32));
        sail_int zgsz3803;
        CREATE(sail_int)(&zgsz3803);
        CONVERT_OF(sail_int, mach_int)(&zgsz3803, zez7);
        sail_int zgsz3804;
        CREATE(sail_int)(&zgsz3804);
        sub_int(&zgsz3804, zgsz3802, zgsz3803);
        zgaz3363 = CONVERT_OF(mach_int, sail_int)(zgsz3804);
        KILL(sail_int)(&zgsz3804);
        KILL(sail_int)(&zgsz3803);
        KILL(sail_int)(&zgsz3802);
      }
      {
        sail_int zgsz3800;
        CREATE(sail_int)(&zgsz3800);
        CONVERT_OF(sail_int, mach_int)(&zgsz3800, zgaz3363);
        lbits zgsz3801;
        CREATE(lbits)(&zgsz3801);
        sailgen_ones(&zgsz3801, zgsz3800);
        zgaz3364 = CONVERT_OF(sbits, lbits)(zgsz3801, true);
        KILL(lbits)(&zgsz3801);
        KILL(sail_int)(&zgsz3800);
      }
    }
    sbits zgaz3365;
    {
      sail_int zgsz3808;
      CREATE(sail_int)(&zgsz3808);
      CONVERT_OF(sail_int, mach_int)(&zgsz3808, zez7);
      lbits zgsz3809;
      CREATE(lbits)(&zgsz3809);
      sailgen_zzeros_implicit(&zgsz3809, zgsz3808);
      zgaz3365 = CONVERT_OF(sbits, lbits)(zgsz3809, true);
      KILL(lbits)(&zgsz3809);
      KILL(sail_int)(&zgsz3808);
    }
    {
      lbits zgsz3805;
      CREATE(lbits)(&zgsz3805);
      CONVERT_OF(lbits, sbits)(&zgsz3805, zgaz3364, true);
      lbits zgsz3806;
      CREATE(lbits)(&zgsz3806);
      CONVERT_OF(lbits, sbits)(&zgsz3806, zgaz3365, true);
      lbits zgsz3807;
      CREATE(lbits)(&zgsz3807);
      append(&zgsz3807, zgsz3805, zgsz3806);
      zgsz378 = CONVERT_OF(fbits, lbits)(zgsz3807, true);
      KILL(lbits)(&zgsz3807);
      KILL(lbits)(&zgsz3806);
      KILL(lbits)(&zgsz3805);
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
  uint64_t zgaz3370;
  {
    uint64_t zgaz3369;
    {
      lbits zgsz3813;
      CREATE(lbits)(&zgsz3813);
      CONVERT_OF(lbits, fbits)(&zgsz3813, zm, UINT64_C(32) , true);
      lbits zgsz3814;
      CREATE(lbits)(&zgsz3814);
      not_bits(&zgsz3814, zgsz3813);
      zgaz3369 = CONVERT_OF(fbits, lbits)(zgsz3814, true);
      KILL(lbits)(&zgsz3814);
      KILL(lbits)(&zgsz3813);
    }
    {
      lbits zgsz3810;
      CREATE(lbits)(&zgsz3810);
      CONVERT_OF(lbits, fbits)(&zgsz3810, zlen, UINT64_C(32) , true);
      lbits zgsz3811;
      CREATE(lbits)(&zgsz3811);
      CONVERT_OF(lbits, fbits)(&zgsz3811, zgaz3369, UINT64_C(32) , true);
      lbits zgsz3812;
      CREATE(lbits)(&zgsz3812);
      add_bits(&zgsz3812, zgsz3810, zgsz3811);
      zgaz3370 = CONVERT_OF(fbits, lbits)(zgsz3812, true);
      KILL(lbits)(&zgsz3812);
      KILL(lbits)(&zgsz3811);
      KILL(lbits)(&zgsz3810);
    }
  }
  {
    lbits zgsz3815;
    CREATE(lbits)(&zgsz3815);
    CONVERT_OF(lbits, fbits)(&zgsz3815, zgaz3370, UINT64_C(32) , true);
    lbits zgsz3816;
    CREATE(lbits)(&zgsz3816);
    CONVERT_OF(lbits, fbits)(&zgsz3816, zm, UINT64_C(32) , true);
    lbits zgsz3817;
    CREATE(lbits)(&zgsz3817);
    and_bits(&zgsz3817, zgsz3815, zgsz3816);
    zcbz338 = CONVERT_OF(fbits, lbits)(zgsz3817, true);
    KILL(lbits)(&zgsz3817);
    KILL(lbits)(&zgsz3816);
    KILL(lbits)(&zgsz3815);
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
static uint64_t zTestLen;

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
  uint64_t zbits1;
  zbits1 = sailgen_capToBits(zTestCap);
  uint64_t zbits2;
  zbits2 = sailgen_capToMemBits(zTestCap);
  struct zCapability zcap1;
  {
    bool zgaz3376;
    zgaz3376 = zTestCap.ztag;
    zcap1 = sailgen_capBitsToCapability(zgaz3376, zbits1);
  }
  struct zCapability zcap2;
  {
    bool zgaz3375;
    zgaz3375 = zTestCap.ztag;
    zcap2 = sailgen_memBitsToCapability(zgaz3375, zbits2);
  }
  bool zrep;
  zrep = sailgen_fastRepCheck(zTestCap, zTestAddr);
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3371;
  zgaz3371 = sailgen_setCapBounds(zTestCap, zTestAddr, zTestLen);
  unit zgsz382;
  {
    __label__ case_164, finish_match_163;

    bool zexact;
    zexact = zgaz3371.ztup0;
    struct zCapability zcap;
    zcap = zgaz3371.ztup1;
    struct ztuple_z8z5i64zCz0z5i64z9 zgaz3372;
    zgaz3372 = sailgen_getCapBounds(zTestCap);
    unit zgsz383;
    {
      int64_t zbase;
      zbase = zgaz3372.ztup0;
      int64_t ztop;
      ztop = zgaz3372.ztup1;
      uint64_t zflags;
      zflags = sailgen_getCapFlags(zTestCap);
      struct zCapability znew_cap;
      {
        uint64_t zgaz3374;
        {
          int64_t zgaz3373;
          {
            sail_int zgsz3820;
            CREATE(sail_int)(&zgsz3820);
            CONVERT_OF(sail_int, mach_int)(&zgsz3820, INT64_C(15));
            sail_int zgsz3821;
            CREATE(sail_int)(&zgsz3821);
            CONVERT_OF(sail_int, mach_int)(&zgsz3821, INT64_C(0));
            sail_int zgsz3822;
            CREATE(sail_int)(&zgsz3822);
            add_int(&zgsz3822, zgsz3820, zgsz3821);
            zgaz3373 = CONVERT_OF(mach_int, sail_int)(zgsz3822);
            KILL(sail_int)(&zgsz3822);
            KILL(sail_int)(&zgsz3821);
            KILL(sail_int)(&zgsz3820);
          }
          {
            sail_int zgsz3818;
            CREATE(sail_int)(&zgsz3818);
            CONVERT_OF(sail_int, mach_int)(&zgsz3818, zgaz3373);
            lbits zgsz3819;
            CREATE(lbits)(&zgsz3819);
            sailgen_ones(&zgsz3819, zgsz3818);
            zgaz3374 = CONVERT_OF(fbits, lbits)(zgsz3819, true);
            KILL(lbits)(&zgsz3819);
            KILL(sail_int)(&zgsz3818);
          }
        }
        znew_cap = sailgen_setCapPerms(zTestCap, zgaz3374);
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
      goto finish_match_163;
    }
  case_164: ;
    sail_match_failure("main");
  finish_match_163: ;
    zgsz382 = zgsz383;
    goto finish_match_161;
  }
case_162: ;
  sail_match_failure("main");
finish_match_161: ;
  zcbz339 = zgsz382;







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
      sail_int zgsz3823;
      CREATE(sail_int)(&zgsz3823);
      CONVERT_OF(sail_int, mach_int)(&zgsz3823, INT64_C(32));
      lbits zgsz3824;
      CREATE(lbits)(&zgsz3824);
      UNDEFINED(lbits)(&zgsz3824, zgsz3823);
      zTestAddr = CONVERT_OF(fbits, lbits)(zgsz3824, true);
      KILL(lbits)(&zgsz3824);
      KILL(sail_int)(&zgsz3823);
    }
    unit zgsz387;
    zgsz387 = UNIT;
  }
  int64_t zgaz3377;
  {
    sail_int zgsz3827;
    CREATE(sail_int)(&zgsz3827);
    CONVERT_OF(sail_int, mach_int)(&zgsz3827, INT64_C(32));
    sail_int zgsz3828;
    CREATE(sail_int)(&zgsz3828);
    CONVERT_OF(sail_int, mach_int)(&zgsz3828, INT64_C(1));
    sail_int zgsz3829;
    CREATE(sail_int)(&zgsz3829);
    add_int(&zgsz3829, zgsz3827, zgsz3828);
    zgaz3377 = CONVERT_OF(mach_int, sail_int)(zgsz3829);
    KILL(sail_int)(&zgsz3829);
    KILL(sail_int)(&zgsz3828);
    KILL(sail_int)(&zgsz3827);
  }
  {
    sail_int zgsz3825;
    CREATE(sail_int)(&zgsz3825);
    CONVERT_OF(sail_int, mach_int)(&zgsz3825, zgaz3377);
    lbits zgsz3826;
    CREATE(lbits)(&zgsz3826);
    UNDEFINED(lbits)(&zgsz3826, zgsz3825);
    zTestLen = CONVERT_OF(fbits, lbits)(zgsz3826, true);
    KILL(lbits)(&zgsz3826);
    KILL(sail_int)(&zgsz3825);
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


