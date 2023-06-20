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
  __label__ end_function_7, end_block_exception_8, end_function_195;

  lbits zgaz32;
  CREATE(lbits)(&zgaz32);
  zeros(&zgaz32, zn);
  not_bits((*(&zcbz32)), zgaz32);
  KILL(lbits)(&zgaz32);
end_function_7: ;
  goto end_function_195;
end_block_exception_8: ;
  goto end_function_195;
end_function_195: ;
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
  __label__ end_function_13, end_block_exception_14, end_function_194;

  sign_extend((*(&zcbz34)), zv, zm);
end_function_13: ;
  goto end_function_194;
end_block_exception_14: ;
  goto end_function_194;
end_function_194: ;
}

static void sailgen_EXTZ(lbits *zcbz35, sail_int zm, lbits zv)
{
  __label__ end_function_16, end_block_exception_17, end_function_193;

  zero_extend((*(&zcbz35)), zv, zm);
end_function_16: ;
  goto end_function_193;
end_block_exception_17: ;
  goto end_function_193;
end_function_193: ;
}

static void sailgen_zzeros_implicit(lbits *rop, sail_int);

static void sailgen_zzeros_implicit(lbits *zcbz36, sail_int zn)
{
  __label__ end_function_19, end_block_exception_20, end_function_192;

  zeros((*(&zcbz36)), zn);
end_function_19: ;
  goto end_function_192;
end_block_exception_20: ;
  goto end_function_192;
end_function_192: ;
}

static void sailgen_ones(lbits *rop, sail_int);

static void sailgen_ones(lbits *zcbz37, sail_int zn)
{
  __label__ end_function_22, end_block_exception_23, end_function_191;

  sailgen_sail_ones((*(&zcbz37)), zn);
end_function_22: ;
  goto end_function_191;
end_block_exception_23: ;
  goto end_function_191;
end_function_191: ;
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
  __label__ end_function_34, end_block_exception_35, end_function_190;

  {
    sail_int zgsz398;
    CREATE(sail_int)(&zgsz398);
    CONVERT_OF(sail_int, mach_int)(&zgsz398, INT64_C(0));
    get_slice_int((*(&zcbz310)), zl, zn, zgsz398);
    KILL(sail_int)(&zgsz398);
  }
end_function_34: ;
  goto end_function_190;
end_block_exception_35: ;
  goto end_function_190;
end_function_190: ;
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
  __label__ end_function_43, end_block_exception_44, end_function_189;

  sail_int zgaz38;
  CREATE(sail_int)(&zgaz38);
  pow2(&zgaz38, zn);
  {
    sail_int zgsz399;
    CREATE(sail_int)(&zgsz399);
    CONVERT_OF(sail_int, mach_int)(&zgsz399, INT64_C(1));
    sub_int((*(&zcbz313)), zgaz38, zgsz399);
    KILL(sail_int)(&zgsz399);
  }
  KILL(sail_int)(&zgaz38);
end_function_43: ;
  goto end_function_189;
end_block_exception_44: ;
  goto end_function_189;
end_function_189: ;
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
    sail_int zgsz3100;
    CREATE(sail_int)(&zgsz3100);
    CONVERT_OF(sail_int, mach_int)(&zgsz3100, INT64_C(64));
    sail_int zgsz3101;
    CREATE(sail_int)(&zgsz3101);
    CONVERT_OF(sail_int, mach_int)(&zgsz3101, INT64_C(1));
    sail_int zgsz3102;
    CREATE(sail_int)(&zgsz3102);
    add_int(&zgsz3102, zgsz3100, zgsz3101);
    zgsz318 = CONVERT_OF(mach_int, sail_int)(zgsz3102);
    KILL(sail_int)(&zgsz3102);
    KILL(sail_int)(&zgsz3101);
    KILL(sail_int)(&zgsz3100);
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
    sail_int zgsz3122;
    CREATE(sail_int)(&zgsz3122);
    CONVERT_OF(sail_int, mach_int)(&zgsz3122, INT64_C(127));
    sail_int zgsz3123;
    CREATE(sail_int)(&zgsz3123);
    CONVERT_OF(sail_int, mach_int)(&zgsz3123, INT64_C(112));
    lbits zgsz3124;
    CREATE(lbits)(&zgsz3124);
    vector_subrange_lbits(&zgsz3124, zc, zgsz3122, zgsz3123);
    zgaz39 = CONVERT_OF(fbits, lbits)(zgsz3124, true);
    KILL(lbits)(&zgsz3124);
    KILL(sail_int)(&zgsz3123);
    KILL(sail_int)(&zgsz3122);
  }
  uint64_t zgaz310;
  {
    sail_int zgsz3119;
    CREATE(sail_int)(&zgsz3119);
    CONVERT_OF(sail_int, mach_int)(&zgsz3119, INT64_C(111));
    sail_int zgsz3120;
    CREATE(sail_int)(&zgsz3120);
    CONVERT_OF(sail_int, mach_int)(&zgsz3120, INT64_C(110));
    lbits zgsz3121;
    CREATE(lbits)(&zgsz3121);
    vector_subrange_lbits(&zgsz3121, zc, zgsz3119, zgsz3120);
    zgaz310 = CONVERT_OF(fbits, lbits)(zgsz3121, true);
    KILL(lbits)(&zgsz3121);
    KILL(sail_int)(&zgsz3120);
    KILL(sail_int)(&zgsz3119);
  }
  uint64_t zgaz311;
  {
    sail_int zgsz3116;
    CREATE(sail_int)(&zgsz3116);
    CONVERT_OF(sail_int, mach_int)(&zgsz3116, INT64_C(109));
    sail_int zgsz3117;
    CREATE(sail_int)(&zgsz3117);
    CONVERT_OF(sail_int, mach_int)(&zgsz3117, INT64_C(109));
    lbits zgsz3118;
    CREATE(lbits)(&zgsz3118);
    vector_subrange_lbits(&zgsz3118, zc, zgsz3116, zgsz3117);
    zgaz311 = CONVERT_OF(fbits, lbits)(zgsz3118, true);
    KILL(lbits)(&zgsz3118);
    KILL(sail_int)(&zgsz3117);
    KILL(sail_int)(&zgsz3116);
  }
  uint64_t zgaz312;
  {
    sail_int zgsz3113;
    CREATE(sail_int)(&zgsz3113);
    CONVERT_OF(sail_int, mach_int)(&zgsz3113, INT64_C(108));
    sail_int zgsz3114;
    CREATE(sail_int)(&zgsz3114);
    CONVERT_OF(sail_int, mach_int)(&zgsz3114, INT64_C(91));
    lbits zgsz3115;
    CREATE(lbits)(&zgsz3115);
    vector_subrange_lbits(&zgsz3115, zc, zgsz3113, zgsz3114);
    zgaz312 = CONVERT_OF(fbits, lbits)(zgsz3115, true);
    KILL(lbits)(&zgsz3115);
    KILL(sail_int)(&zgsz3114);
    KILL(sail_int)(&zgsz3113);
  }
  fbits zgaz313;
  {
    sail_int zgsz3112;
    CREATE(sail_int)(&zgsz3112);
    CONVERT_OF(sail_int, mach_int)(&zgsz3112, INT64_C(90));
    zgaz313 = bitvector_access(zc, zgsz3112);
    KILL(sail_int)(&zgsz3112);
  }
  uint64_t zgaz314;
  {
    sail_int zgsz3109;
    CREATE(sail_int)(&zgsz3109);
    CONVERT_OF(sail_int, mach_int)(&zgsz3109, INT64_C(89));
    sail_int zgsz3110;
    CREATE(sail_int)(&zgsz3110);
    CONVERT_OF(sail_int, mach_int)(&zgsz3110, INT64_C(78));
    lbits zgsz3111;
    CREATE(lbits)(&zgsz3111);
    vector_subrange_lbits(&zgsz3111, zc, zgsz3109, zgsz3110);
    zgaz314 = CONVERT_OF(fbits, lbits)(zgsz3111, true);
    KILL(lbits)(&zgsz3111);
    KILL(sail_int)(&zgsz3110);
    KILL(sail_int)(&zgsz3109);
  }
  uint64_t zgaz315;
  {
    sail_int zgsz3106;
    CREATE(sail_int)(&zgsz3106);
    CONVERT_OF(sail_int, mach_int)(&zgsz3106, INT64_C(77));
    sail_int zgsz3107;
    CREATE(sail_int)(&zgsz3107);
    CONVERT_OF(sail_int, mach_int)(&zgsz3107, INT64_C(64));
    lbits zgsz3108;
    CREATE(lbits)(&zgsz3108);
    vector_subrange_lbits(&zgsz3108, zc, zgsz3106, zgsz3107);
    zgaz315 = CONVERT_OF(fbits, lbits)(zgsz3108, true);
    KILL(lbits)(&zgsz3108);
    KILL(sail_int)(&zgsz3107);
    KILL(sail_int)(&zgsz3106);
  }
  uint64_t zgaz316;
  {
    sail_int zgsz3103;
    CREATE(sail_int)(&zgsz3103);
    CONVERT_OF(sail_int, mach_int)(&zgsz3103, INT64_C(63));
    sail_int zgsz3104;
    CREATE(sail_int)(&zgsz3104);
    CONVERT_OF(sail_int, mach_int)(&zgsz3104, INT64_C(0));
    lbits zgsz3105;
    CREATE(lbits)(&zgsz3105);
    vector_subrange_lbits(&zgsz3105, zc, zgsz3103, zgsz3104);
    zgaz316 = CONVERT_OF(fbits, lbits)(zgsz3105, true);
    KILL(lbits)(&zgsz3105);
    KILL(sail_int)(&zgsz3104);
    KILL(sail_int)(&zgsz3103);
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
  struct zEncCapability zcbz343 = { .zreserved = UINT64_C(0xdeadc0de), .zperms = UINT64_C(0xdeadc0de), .zotype = UINT64_C(0xdeadc0de), .zinternal_E = UINT64_C(0), .zflags = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  return zcbz343;
}

static void sailgen_encCapToBits(lbits *rop, struct zEncCapability);

static void sailgen_encCapToBits(lbits *zcbz315, struct zEncCapability zcap)
{
  __label__ end_function_67, end_block_exception_68, end_function_188;

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
                lbits zgsz3125;
                CREATE(lbits)(&zgsz3125);
                CONVERT_OF(lbits, fbits)(&zgsz3125, zgaz318, UINT64_C(14) , true);
                lbits zgsz3126;
                CREATE(lbits)(&zgsz3126);
                CONVERT_OF(lbits, fbits)(&zgsz3126, zgaz319, UINT64_C(64) , true);
                append(&zgaz321, zgsz3125, zgsz3126);
                KILL(lbits)(&zgsz3126);
                KILL(lbits)(&zgsz3125);
              }
            }
            {
              lbits zgsz3127;
              CREATE(lbits)(&zgsz3127);
              CONVERT_OF(lbits, fbits)(&zgsz3127, zgaz320, UINT64_C(12) , true);
              append(&zgaz323, zgsz3127, zgaz321);
              KILL(lbits)(&zgsz3127);
            }
            KILL(lbits)(&zgaz321);
          }
          {
            lbits zgsz3128;
            CREATE(lbits)(&zgsz3128);
            CONVERT_OF(lbits, fbits)(&zgsz3128, zgaz322, UINT64_C(1) , true);
            append(&zgaz325, zgsz3128, zgaz323);
            KILL(lbits)(&zgsz3128);
          }
          KILL(lbits)(&zgaz323);
        }
        {
          lbits zgsz3129;
          CREATE(lbits)(&zgsz3129);
          CONVERT_OF(lbits, fbits)(&zgsz3129, zgaz324, UINT64_C(18) , true);
          append(&zgaz327, zgsz3129, zgaz325);
          KILL(lbits)(&zgsz3129);
        }
        KILL(lbits)(&zgaz325);
      }
      {
        lbits zgsz3130;
        CREATE(lbits)(&zgsz3130);
        CONVERT_OF(lbits, fbits)(&zgsz3130, zgaz326, UINT64_C(1) , true);
        append(&zgaz329, zgsz3130, zgaz327);
        KILL(lbits)(&zgsz3130);
      }
      KILL(lbits)(&zgaz327);
    }
    {
      lbits zgsz3131;
      CREATE(lbits)(&zgsz3131);
      CONVERT_OF(lbits, fbits)(&zgsz3131, zgaz328, UINT64_C(2) , true);
      append(&zgaz331, zgsz3131, zgaz329);
      KILL(lbits)(&zgsz3131);
    }
    KILL(lbits)(&zgaz329);
  }
  {
    lbits zgsz3132;
    CREATE(lbits)(&zgsz3132);
    CONVERT_OF(lbits, fbits)(&zgsz3132, zgaz330, UINT64_C(16) , true);
    append((*(&zcbz315)), zgsz3132, zgaz331);
    KILL(lbits)(&zgsz3132);
  }
  KILL(lbits)(&zgaz331);

end_function_67: ;
  goto end_function_188;
end_block_exception_68: ;
  goto end_function_188;
end_function_188: ;
}

static sail_int zcap_max_addr;
static void create_letbind_18(void) {    CREATE(sail_int)(&zcap_max_addr);


  sail_int zgsz323;
  CREATE(sail_int)(&zgsz323);
  {
    sail_int zgsz3133;
    CREATE(sail_int)(&zgsz3133);
    CONVERT_OF(sail_int, mach_int)(&zgsz3133, zcap_addr_width);
    sailgen_MAX(&zgsz323, zgsz3133);
    KILL(sail_int)(&zgsz3133);
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
    sail_int zgsz3137;
    CREATE(sail_int)(&zgsz3137);
    CONVERT_OF(sail_int, mach_int)(&zgsz3137, zcap_otype_width);
    sail_int zgsz3138;
    CREATE(sail_int)(&zgsz3138);
    sailgen_MAX(&zgsz3138, zgsz3137);
    zgaz332 = CONVERT_OF(mach_int, sail_int)(zgsz3138);
    KILL(sail_int)(&zgsz3138);
    KILL(sail_int)(&zgsz3137);
  }
  {
    sail_int zgsz3134;
    CREATE(sail_int)(&zgsz3134);
    CONVERT_OF(sail_int, mach_int)(&zgsz3134, zgaz332);
    sail_int zgsz3135;
    CREATE(sail_int)(&zgsz3135);
    CONVERT_OF(sail_int, mach_int)(&zgsz3135, zreserved_otypes);
    sail_int zgsz3136;
    CREATE(sail_int)(&zgsz3136);
    sub_int(&zgsz3136, zgsz3134, zgsz3135);
    zgsz324 = CONVERT_OF(mach_int, sail_int)(zgsz3136);
    KILL(sail_int)(&zgsz3136);
    KILL(sail_int)(&zgsz3135);
    KILL(sail_int)(&zgsz3134);
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
    sail_int zgsz3139;
    CREATE(sail_int)(&zgsz3139);
    CONVERT_OF(sail_int, mach_int)(&zgsz3139, INT64_C(15));
    sail_int zgsz3140;
    CREATE(sail_int)(&zgsz3140);
    CONVERT_OF(sail_int, mach_int)(&zgsz3140, INT64_C(4));
    sail_int zgsz3141;
    CREATE(sail_int)(&zgsz3141);
    add_int(&zgsz3141, zgsz3139, zgsz3140);
    zgsz326 = CONVERT_OF(mach_int, sail_int)(zgsz3141);
    KILL(sail_int)(&zgsz3141);
    KILL(sail_int)(&zgsz3140);
    KILL(sail_int)(&zgsz3139);
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
    sail_int zgsz3145;
    CREATE(sail_int)(&zgsz3145);
    CONVERT_OF(sail_int, mach_int)(&zgsz3145, zcap_len_width);
    sail_int zgsz3146;
    CREATE(sail_int)(&zgsz3146);
    CONVERT_OF(sail_int, mach_int)(&zgsz3146, zcap_mantissa_width);
    sail_int zgsz3147;
    CREATE(sail_int)(&zgsz3147);
    sub_int(&zgsz3147, zgsz3145, zgsz3146);
    zgaz333 = CONVERT_OF(mach_int, sail_int)(zgsz3147);
    KILL(sail_int)(&zgsz3147);
    KILL(sail_int)(&zgsz3146);
    KILL(sail_int)(&zgsz3145);
  }
  {
    sail_int zgsz3142;
    CREATE(sail_int)(&zgsz3142);
    CONVERT_OF(sail_int, mach_int)(&zgsz3142, zgaz333);
    sail_int zgsz3143;
    CREATE(sail_int)(&zgsz3143);
    CONVERT_OF(sail_int, mach_int)(&zgsz3143, INT64_C(1));
    sail_int zgsz3144;
    CREATE(sail_int)(&zgsz3144);
    add_int(&zgsz3144, zgsz3142, zgsz3143);
    zgsz327 = CONVERT_OF(mach_int, sail_int)(zgsz3144);
    KILL(sail_int)(&zgsz3144);
    KILL(sail_int)(&zgsz3143);
    KILL(sail_int)(&zgsz3142);
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
    sail_int zgsz3148;
    CREATE(sail_int)(&zgsz3148);
    CONVERT_OF(sail_int, mach_int)(&zgsz3148, zcap_E_width);
    sail_int zgsz3149;
    CREATE(sail_int)(&zgsz3149);
    CONVERT_OF(sail_int, mach_int)(&zgsz3149, zcap_max_E);
    lbits zgsz3150;
    CREATE(lbits)(&zgsz3150);
    sailgen_to_bits(&zgsz3150, zgsz3148, zgsz3149);
    zgsz328 = CONVERT_OF(fbits, lbits)(zgsz3150, true);
    KILL(lbits)(&zgsz3150);
    KILL(sail_int)(&zgsz3149);
    KILL(sail_int)(&zgsz3148);
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
      sail_int zgsz3153;
      CREATE(sail_int)(&zgsz3153);
      CONVERT_OF(sail_int, mach_int)(&zgsz3153, zcap_mantissa_width);
      sail_int zgsz3154;
      CREATE(sail_int)(&zgsz3154);
      CONVERT_OF(sail_int, mach_int)(&zgsz3154, INT64_C(2));
      sail_int zgsz3155;
      CREATE(sail_int)(&zgsz3155);
      sub_int(&zgsz3155, zgsz3153, zgsz3154);
      zgaz334 = CONVERT_OF(mach_int, sail_int)(zgsz3155);
      KILL(sail_int)(&zgsz3155);
      KILL(sail_int)(&zgsz3154);
      KILL(sail_int)(&zgsz3153);
    }
    {
      sail_int zgsz3151;
      CREATE(sail_int)(&zgsz3151);
      CONVERT_OF(sail_int, mach_int)(&zgsz3151, zgaz334);
      lbits zgsz3152;
      CREATE(lbits)(&zgsz3152);
      sailgen_zzeros_implicit(&zgsz3152, zgsz3151);
      zgaz335 = CONVERT_OF(fbits, lbits)(zgsz3152, true);
      KILL(lbits)(&zgsz3152);
      KILL(sail_int)(&zgsz3151);
    }
  }
  {
    lbits zgsz3156;
    CREATE(lbits)(&zgsz3156);
    CONVERT_OF(lbits, fbits)(&zgsz3156, UINT64_C(0b01), UINT64_C(2) , true);
    lbits zgsz3157;
    CREATE(lbits)(&zgsz3157);
    CONVERT_OF(lbits, fbits)(&zgsz3157, zgaz335, UINT64_C(12) , true);
    lbits zgsz3158;
    CREATE(lbits)(&zgsz3158);
    append(&zgsz3158, zgsz3156, zgsz3157);
    zgsz329 = CONVERT_OF(fbits, lbits)(zgsz3158, true);
    KILL(lbits)(&zgsz3158);
    KILL(lbits)(&zgsz3157);
    KILL(lbits)(&zgsz3156);
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
    sail_int zgsz3171;
    CREATE(sail_int)(&zgsz3171);
    CONVERT_OF(sail_int, mach_int)(&zgsz3171, INT64_C(4));
    lbits zgsz3172;
    CREATE(lbits)(&zgsz3172);
    UNDEFINED(lbits)(&zgsz3172, zgsz3171);
    zgaz337 = CONVERT_OF(fbits, lbits)(zgsz3172, true);
    KILL(lbits)(&zgsz3172);
    KILL(sail_int)(&zgsz3171);
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
    sail_int zgsz3169;
    CREATE(sail_int)(&zgsz3169);
    CONVERT_OF(sail_int, mach_int)(&zgsz3169, INT64_C(2));
    lbits zgsz3170;
    CREATE(lbits)(&zgsz3170);
    UNDEFINED(lbits)(&zgsz3170, zgsz3169);
    zgaz350 = CONVERT_OF(fbits, lbits)(zgsz3170, true);
    KILL(lbits)(&zgsz3170);
    KILL(sail_int)(&zgsz3169);
  }
  bool zgaz351;
  zgaz351 = undefined_bool(UNIT);
  bool zgaz352;
  zgaz352 = undefined_bool(UNIT);
  uint64_t zgaz353;
  {
    sail_int zgsz3167;
    CREATE(sail_int)(&zgsz3167);
    CONVERT_OF(sail_int, mach_int)(&zgsz3167, INT64_C(6));
    lbits zgsz3168;
    CREATE(lbits)(&zgsz3168);
    UNDEFINED(lbits)(&zgsz3168, zgsz3167);
    zgaz353 = CONVERT_OF(fbits, lbits)(zgsz3168, true);
    KILL(lbits)(&zgsz3168);
    KILL(sail_int)(&zgsz3167);
  }
  uint64_t zgaz354;
  {
    sail_int zgsz3165;
    CREATE(sail_int)(&zgsz3165);
    CONVERT_OF(sail_int, mach_int)(&zgsz3165, INT64_C(14));
    lbits zgsz3166;
    CREATE(lbits)(&zgsz3166);
    UNDEFINED(lbits)(&zgsz3166, zgsz3165);
    zgaz354 = CONVERT_OF(fbits, lbits)(zgsz3166, true);
    KILL(lbits)(&zgsz3166);
    KILL(sail_int)(&zgsz3165);
  }
  uint64_t zgaz355;
  {
    sail_int zgsz3163;
    CREATE(sail_int)(&zgsz3163);
    CONVERT_OF(sail_int, mach_int)(&zgsz3163, INT64_C(14));
    lbits zgsz3164;
    CREATE(lbits)(&zgsz3164);
    UNDEFINED(lbits)(&zgsz3164, zgsz3163);
    zgaz355 = CONVERT_OF(fbits, lbits)(zgsz3164, true);
    KILL(lbits)(&zgsz3164);
    KILL(sail_int)(&zgsz3163);
  }
  uint64_t zgaz356;
  {
    sail_int zgsz3161;
    CREATE(sail_int)(&zgsz3161);
    CONVERT_OF(sail_int, mach_int)(&zgsz3161, INT64_C(18));
    lbits zgsz3162;
    CREATE(lbits)(&zgsz3162);
    UNDEFINED(lbits)(&zgsz3162, zgsz3161);
    zgaz356 = CONVERT_OF(fbits, lbits)(zgsz3162, true);
    KILL(lbits)(&zgsz3162);
    KILL(sail_int)(&zgsz3161);
  }
  uint64_t zgaz357;
  {
    sail_int zgsz3159;
    CREATE(sail_int)(&zgsz3159);
    CONVERT_OF(sail_int, mach_int)(&zgsz3159, INT64_C(64));
    lbits zgsz3160;
    CREATE(lbits)(&zgsz3160);
    UNDEFINED(lbits)(&zgsz3160, zgsz3159);
    zgaz357 = CONVERT_OF(fbits, lbits)(zgsz3160, true);
    KILL(lbits)(&zgsz3160);
    KILL(sail_int)(&zgsz3159);
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
  struct zCapability zcbz344 = { .zuperms = UINT64_C(0xdeadc0de), .ztag = false, .zreserved = UINT64_C(0xdeadc0de), .zpermit_unseal = false, .zpermit_store_local_cap = false, .zpermit_store_cap = false, .zpermit_store = false, .zpermit_set_CID = false, .zpermit_seal = false, .zpermit_load_cap = false, .zpermit_load = false, .zpermit_execute = false, .zpermit_cinvoke = false, .zotype = UINT64_C(0xdeadc0de), .zinternal_E = false, .zglobal = false, .zflag_cap_mode = false, .zaddress = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zT = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  return zcbz344;
}

static struct zCapability znull_cap;
static void create_letbind_25(void) {


  struct zCapability zgsz333;
  uint64_t zgaz358;
  {
    sail_int zgsz3182;
    CREATE(sail_int)(&zgsz3182);
    CONVERT_OF(sail_int, mach_int)(&zgsz3182, INT64_C(4));
    lbits zgsz3183;
    CREATE(lbits)(&zgsz3183);
    sailgen_zzeros_implicit(&zgsz3183, zgsz3182);
    zgaz358 = CONVERT_OF(fbits, lbits)(zgsz3183, true);
    KILL(lbits)(&zgsz3183);
    KILL(sail_int)(&zgsz3182);
  }
  uint64_t zgaz359;
  {
    sail_int zgsz3180;
    CREATE(sail_int)(&zgsz3180);
    CONVERT_OF(sail_int, mach_int)(&zgsz3180, INT64_C(2));
    lbits zgsz3181;
    CREATE(lbits)(&zgsz3181);
    sailgen_zzeros_implicit(&zgsz3181, zgsz3180);
    zgaz359 = CONVERT_OF(fbits, lbits)(zgsz3181, true);
    KILL(lbits)(&zgsz3181);
    KILL(sail_int)(&zgsz3180);
  }
  uint64_t zgaz360;
  {
    sail_int zgsz3178;
    CREATE(sail_int)(&zgsz3178);
    CONVERT_OF(sail_int, mach_int)(&zgsz3178, INT64_C(14));
    lbits zgsz3179;
    CREATE(lbits)(&zgsz3179);
    sailgen_zzeros_implicit(&zgsz3179, zgsz3178);
    zgaz360 = CONVERT_OF(fbits, lbits)(zgsz3179, true);
    KILL(lbits)(&zgsz3179);
    KILL(sail_int)(&zgsz3178);
  }
  uint64_t zgaz361;
  {
    sail_int zgsz3175;
    CREATE(sail_int)(&zgsz3175);
    CONVERT_OF(sail_int, mach_int)(&zgsz3175, zcap_otype_width);
    sail_int zgsz3176;
    CREATE(sail_int)(&zgsz3176);
    CONVERT_OF(sail_int, mach_int)(&zgsz3176, zotype_unsealed);
    lbits zgsz3177;
    CREATE(lbits)(&zgsz3177);
    sailgen_to_bits(&zgsz3177, zgsz3175, zgsz3176);
    zgaz361 = CONVERT_OF(fbits, lbits)(zgsz3177, true);
    KILL(lbits)(&zgsz3177);
    KILL(sail_int)(&zgsz3176);
    KILL(sail_int)(&zgsz3175);
  }
  uint64_t zgaz362;
  {
    sail_int zgsz3173;
    CREATE(sail_int)(&zgsz3173);
    CONVERT_OF(sail_int, mach_int)(&zgsz3173, INT64_C(64));
    lbits zgsz3174;
    CREATE(lbits)(&zgsz3174);
    sailgen_zzeros_implicit(&zgsz3174, zgsz3173);
    zgaz362 = CONVERT_OF(fbits, lbits)(zgsz3174, true);
    KILL(lbits)(&zgsz3174);
    KILL(sail_int)(&zgsz3173);
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
    sail_int zgsz3193;
    CREATE(sail_int)(&zgsz3193);
    CONVERT_OF(sail_int, mach_int)(&zgsz3193, INT64_C(4));
    lbits zgsz3194;
    CREATE(lbits)(&zgsz3194);
    sailgen_ones(&zgsz3194, zgsz3193);
    zgaz363 = CONVERT_OF(fbits, lbits)(zgsz3194, true);
    KILL(lbits)(&zgsz3194);
    KILL(sail_int)(&zgsz3193);
  }
  uint64_t zgaz364;
  {
    sail_int zgsz3191;
    CREATE(sail_int)(&zgsz3191);
    CONVERT_OF(sail_int, mach_int)(&zgsz3191, INT64_C(2));
    lbits zgsz3192;
    CREATE(lbits)(&zgsz3192);
    sailgen_zzeros_implicit(&zgsz3192, zgsz3191);
    zgaz364 = CONVERT_OF(fbits, lbits)(zgsz3192, true);
    KILL(lbits)(&zgsz3192);
    KILL(sail_int)(&zgsz3191);
  }
  uint64_t zgaz365;
  {
    sail_int zgsz3189;
    CREATE(sail_int)(&zgsz3189);
    CONVERT_OF(sail_int, mach_int)(&zgsz3189, INT64_C(14));
    lbits zgsz3190;
    CREATE(lbits)(&zgsz3190);
    sailgen_zzeros_implicit(&zgsz3190, zgsz3189);
    zgaz365 = CONVERT_OF(fbits, lbits)(zgsz3190, true);
    KILL(lbits)(&zgsz3190);
    KILL(sail_int)(&zgsz3189);
  }
  uint64_t zgaz366;
  {
    sail_int zgsz3186;
    CREATE(sail_int)(&zgsz3186);
    CONVERT_OF(sail_int, mach_int)(&zgsz3186, zcap_otype_width);
    sail_int zgsz3187;
    CREATE(sail_int)(&zgsz3187);
    CONVERT_OF(sail_int, mach_int)(&zgsz3187, zotype_unsealed);
    lbits zgsz3188;
    CREATE(lbits)(&zgsz3188);
    sailgen_to_bits(&zgsz3188, zgsz3186, zgsz3187);
    zgaz366 = CONVERT_OF(fbits, lbits)(zgsz3188, true);
    KILL(lbits)(&zgsz3188);
    KILL(sail_int)(&zgsz3187);
    KILL(sail_int)(&zgsz3186);
  }
  uint64_t zgaz367;
  {
    sail_int zgsz3184;
    CREATE(sail_int)(&zgsz3184);
    CONVERT_OF(sail_int, mach_int)(&zgsz3184, INT64_C(64));
    lbits zgsz3185;
    CREATE(lbits)(&zgsz3185);
    sailgen_zzeros_implicit(&zgsz3185, zgsz3184);
    zgaz367 = CONVERT_OF(fbits, lbits)(zgsz3185, true);
    KILL(lbits)(&zgsz3185);
    KILL(sail_int)(&zgsz3184);
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
                        lbits zgsz3195;
                        CREATE(lbits)(&zgsz3195);
                        CONVERT_OF(lbits, fbits)(&zgsz3195, zgaz380, UINT64_C(1) , true);
                        lbits zgsz3196;
                        CREATE(lbits)(&zgsz3196);
                        CONVERT_OF(lbits, fbits)(&zgsz3196, zgaz381, UINT64_C(1) , true);
                        lbits zgsz3197;
                        CREATE(lbits)(&zgsz3197);
                        append(&zgsz3197, zgsz3195, zgsz3196);
                        zgaz383 = CONVERT_OF(fbits, lbits)(zgsz3197, true);
                        KILL(lbits)(&zgsz3197);
                        KILL(lbits)(&zgsz3196);
                        KILL(lbits)(&zgsz3195);
                      }
                    }
                    {
                      lbits zgsz3198;
                      CREATE(lbits)(&zgsz3198);
                      CONVERT_OF(lbits, fbits)(&zgsz3198, zgaz382, UINT64_C(1) , true);
                      lbits zgsz3199;
                      CREATE(lbits)(&zgsz3199);
                      CONVERT_OF(lbits, fbits)(&zgsz3199, zgaz383, UINT64_C(2) , true);
                      lbits zgsz3200;
                      CREATE(lbits)(&zgsz3200);
                      append(&zgsz3200, zgsz3198, zgsz3199);
                      zgaz385 = CONVERT_OF(fbits, lbits)(zgsz3200, true);
                      KILL(lbits)(&zgsz3200);
                      KILL(lbits)(&zgsz3199);
                      KILL(lbits)(&zgsz3198);
                    }
                  }
                  {
                    lbits zgsz3201;
                    CREATE(lbits)(&zgsz3201);
                    CONVERT_OF(lbits, fbits)(&zgsz3201, zgaz384, UINT64_C(1) , true);
                    lbits zgsz3202;
                    CREATE(lbits)(&zgsz3202);
                    CONVERT_OF(lbits, fbits)(&zgsz3202, zgaz385, UINT64_C(3) , true);
                    lbits zgsz3203;
                    CREATE(lbits)(&zgsz3203);
                    append(&zgsz3203, zgsz3201, zgsz3202);
                    zgaz387 = CONVERT_OF(fbits, lbits)(zgsz3203, true);
                    KILL(lbits)(&zgsz3203);
                    KILL(lbits)(&zgsz3202);
                    KILL(lbits)(&zgsz3201);
                  }
                }
                {
                  lbits zgsz3204;
                  CREATE(lbits)(&zgsz3204);
                  CONVERT_OF(lbits, fbits)(&zgsz3204, zgaz386, UINT64_C(1) , true);
                  lbits zgsz3205;
                  CREATE(lbits)(&zgsz3205);
                  CONVERT_OF(lbits, fbits)(&zgsz3205, zgaz387, UINT64_C(4) , true);
                  lbits zgsz3206;
                  CREATE(lbits)(&zgsz3206);
                  append(&zgsz3206, zgsz3204, zgsz3205);
                  zgaz389 = CONVERT_OF(fbits, lbits)(zgsz3206, true);
                  KILL(lbits)(&zgsz3206);
                  KILL(lbits)(&zgsz3205);
                  KILL(lbits)(&zgsz3204);
                }
              }
              {
                lbits zgsz3207;
                CREATE(lbits)(&zgsz3207);
                CONVERT_OF(lbits, fbits)(&zgsz3207, zgaz388, UINT64_C(1) , true);
                lbits zgsz3208;
                CREATE(lbits)(&zgsz3208);
                CONVERT_OF(lbits, fbits)(&zgsz3208, zgaz389, UINT64_C(5) , true);
                lbits zgsz3209;
                CREATE(lbits)(&zgsz3209);
                append(&zgsz3209, zgsz3207, zgsz3208);
                zgaz391 = CONVERT_OF(fbits, lbits)(zgsz3209, true);
                KILL(lbits)(&zgsz3209);
                KILL(lbits)(&zgsz3208);
                KILL(lbits)(&zgsz3207);
              }
            }
            {
              lbits zgsz3210;
              CREATE(lbits)(&zgsz3210);
              CONVERT_OF(lbits, fbits)(&zgsz3210, zgaz390, UINT64_C(1) , true);
              lbits zgsz3211;
              CREATE(lbits)(&zgsz3211);
              CONVERT_OF(lbits, fbits)(&zgsz3211, zgaz391, UINT64_C(6) , true);
              lbits zgsz3212;
              CREATE(lbits)(&zgsz3212);
              append(&zgsz3212, zgsz3210, zgsz3211);
              zgaz393 = CONVERT_OF(fbits, lbits)(zgsz3212, true);
              KILL(lbits)(&zgsz3212);
              KILL(lbits)(&zgsz3211);
              KILL(lbits)(&zgsz3210);
            }
          }
          {
            lbits zgsz3213;
            CREATE(lbits)(&zgsz3213);
            CONVERT_OF(lbits, fbits)(&zgsz3213, zgaz392, UINT64_C(1) , true);
            lbits zgsz3214;
            CREATE(lbits)(&zgsz3214);
            CONVERT_OF(lbits, fbits)(&zgsz3214, zgaz393, UINT64_C(7) , true);
            lbits zgsz3215;
            CREATE(lbits)(&zgsz3215);
            append(&zgsz3215, zgsz3213, zgsz3214);
            zgaz395 = CONVERT_OF(fbits, lbits)(zgsz3215, true);
            KILL(lbits)(&zgsz3215);
            KILL(lbits)(&zgsz3214);
            KILL(lbits)(&zgsz3213);
          }
        }
        {
          lbits zgsz3216;
          CREATE(lbits)(&zgsz3216);
          CONVERT_OF(lbits, fbits)(&zgsz3216, zgaz394, UINT64_C(1) , true);
          lbits zgsz3217;
          CREATE(lbits)(&zgsz3217);
          CONVERT_OF(lbits, fbits)(&zgsz3217, zgaz395, UINT64_C(8) , true);
          lbits zgsz3218;
          CREATE(lbits)(&zgsz3218);
          append(&zgsz3218, zgsz3216, zgsz3217);
          zgaz397 = CONVERT_OF(fbits, lbits)(zgsz3218, true);
          KILL(lbits)(&zgsz3218);
          KILL(lbits)(&zgsz3217);
          KILL(lbits)(&zgsz3216);
        }
      }
      {
        lbits zgsz3219;
        CREATE(lbits)(&zgsz3219);
        CONVERT_OF(lbits, fbits)(&zgsz3219, zgaz396, UINT64_C(1) , true);
        lbits zgsz3220;
        CREATE(lbits)(&zgsz3220);
        CONVERT_OF(lbits, fbits)(&zgsz3220, zgaz397, UINT64_C(9) , true);
        lbits zgsz3221;
        CREATE(lbits)(&zgsz3221);
        append(&zgsz3221, zgsz3219, zgsz3220);
        zgaz399 = CONVERT_OF(fbits, lbits)(zgsz3221, true);
        KILL(lbits)(&zgsz3221);
        KILL(lbits)(&zgsz3220);
        KILL(lbits)(&zgsz3219);
      }
    }
    {
      lbits zgsz3222;
      CREATE(lbits)(&zgsz3222);
      CONVERT_OF(lbits, fbits)(&zgsz3222, zgaz398, UINT64_C(1) , true);
      lbits zgsz3223;
      CREATE(lbits)(&zgsz3223);
      CONVERT_OF(lbits, fbits)(&zgsz3223, zgaz399, UINT64_C(10) , true);
      lbits zgsz3224;
      CREATE(lbits)(&zgsz3224);
      append(&zgsz3224, zgsz3222, zgsz3223);
      zgaz3101 = CONVERT_OF(fbits, lbits)(zgsz3224, true);
      KILL(lbits)(&zgsz3224);
      KILL(lbits)(&zgsz3223);
      KILL(lbits)(&zgsz3222);
    }
  }
  {
    lbits zgsz3225;
    CREATE(lbits)(&zgsz3225);
    CONVERT_OF(lbits, fbits)(&zgsz3225, zgaz3100, UINT64_C(1) , true);
    lbits zgsz3226;
    CREATE(lbits)(&zgsz3226);
    CONVERT_OF(lbits, fbits)(&zgsz3226, zgaz3101, UINT64_C(11) , true);
    lbits zgsz3227;
    CREATE(lbits)(&zgsz3227);
    append(&zgsz3227, zgsz3225, zgsz3226);
    zcbz317 = CONVERT_OF(fbits, lbits)(zgsz3227, true);
    KILL(lbits)(&zgsz3227);
    KILL(lbits)(&zgsz3226);
    KILL(lbits)(&zgsz3225);
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
    sail_int zgsz3344;
    CREATE(sail_int)(&zgsz3344);
    CONVERT_OF(sail_int, mach_int)(&zgsz3344, INT64_C(6));
    lbits zgsz3345;
    CREATE(lbits)(&zgsz3345);
    sailgen_zzeros_implicit(&zgsz3345, zgsz3344);
    zE = CONVERT_OF(fbits, lbits)(zgsz3345, true);
    KILL(lbits)(&zgsz3345);
    KILL(sail_int)(&zgsz3344);
  }
  uint64_t zBs;
  {
    sail_int zgsz3342;
    CREATE(sail_int)(&zgsz3342);
    CONVERT_OF(sail_int, mach_int)(&zgsz3342, INT64_C(14));
    lbits zgsz3343;
    CREATE(lbits)(&zgsz3343);
    sailgen_zzeros_implicit(&zgsz3343, zgsz3342);
    zBs = CONVERT_OF(fbits, lbits)(zgsz3343, true);
    KILL(lbits)(&zgsz3343);
    KILL(sail_int)(&zgsz3342);
  }
  uint64_t zT;
  {
    int64_t zgaz3172;
    {
      sail_int zgsz3230;
      CREATE(sail_int)(&zgsz3230);
      CONVERT_OF(sail_int, mach_int)(&zgsz3230, INT64_C(14));
      sail_int zgsz3231;
      CREATE(sail_int)(&zgsz3231);
      CONVERT_OF(sail_int, mach_int)(&zgsz3231, INT64_C(2));
      sail_int zgsz3232;
      CREATE(sail_int)(&zgsz3232);
      sub_int(&zgsz3232, zgsz3230, zgsz3231);
      zgaz3172 = CONVERT_OF(mach_int, sail_int)(zgsz3232);
      KILL(sail_int)(&zgsz3232);
      KILL(sail_int)(&zgsz3231);
      KILL(sail_int)(&zgsz3230);
    }
    {
      sail_int zgsz3228;
      CREATE(sail_int)(&zgsz3228);
      CONVERT_OF(sail_int, mach_int)(&zgsz3228, zgaz3172);
      lbits zgsz3229;
      CREATE(lbits)(&zgsz3229);
      sailgen_zzeros_implicit(&zgsz3229, zgsz3228);
      zT = CONVERT_OF(fbits, lbits)(zgsz3229, true);
      KILL(lbits)(&zgsz3229);
      KILL(sail_int)(&zgsz3228);
    }
  }
  uint64_t zlenMSBs;
  {
    sail_int zgsz3340;
    CREATE(sail_int)(&zgsz3340);
    CONVERT_OF(sail_int, mach_int)(&zgsz3340, INT64_C(2));
    lbits zgsz3341;
    CREATE(lbits)(&zgsz3341);
    sailgen_zzeros_implicit(&zgsz3341, zgsz3340);
    zlenMSBs = CONVERT_OF(fbits, lbits)(zgsz3341, true);
    KILL(lbits)(&zgsz3341);
    KILL(sail_int)(&zgsz3340);
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
            sail_int zgsz3237;
            CREATE(sail_int)(&zgsz3237);
            CONVERT_OF(sail_int, mach_int)(&zgsz3237, zinternal_E_take_bits);
            sail_int zgsz3238;
            CREATE(sail_int)(&zgsz3238);
            CONVERT_OF(sail_int, mach_int)(&zgsz3238, INT64_C(1));
            sail_int zgsz3239;
            CREATE(sail_int)(&zgsz3239);
            sub_int(&zgsz3239, zgsz3237, zgsz3238);
            zgaz3103 = CONVERT_OF(mach_int, sail_int)(zgsz3239);
            KILL(sail_int)(&zgsz3239);
            KILL(sail_int)(&zgsz3238);
            KILL(sail_int)(&zgsz3237);
          }
          {
            lbits zgsz3233;
            CREATE(lbits)(&zgsz3233);
            CONVERT_OF(lbits, fbits)(&zgsz3233, zgaz3102, UINT64_C(12) , true);
            sail_int zgsz3234;
            CREATE(sail_int)(&zgsz3234);
            CONVERT_OF(sail_int, mach_int)(&zgsz3234, zgaz3103);
            sail_int zgsz3235;
            CREATE(sail_int)(&zgsz3235);
            CONVERT_OF(sail_int, mach_int)(&zgsz3235, INT64_C(0));
            lbits zgsz3236;
            CREATE(lbits)(&zgsz3236);
            vector_subrange_lbits(&zgsz3236, zgsz3233, zgsz3234, zgsz3235);
            zgaz3106 = CONVERT_OF(fbits, lbits)(zgsz3236, true);
            KILL(lbits)(&zgsz3236);
            KILL(sail_int)(&zgsz3235);
            KILL(sail_int)(&zgsz3234);
            KILL(lbits)(&zgsz3233);
          }
        }
        uint64_t zgaz3107;
        {
          uint64_t zgaz3104;
          zgaz3104 = zc.zB;
          int64_t zgaz3105;
          {
            sail_int zgsz3244;
            CREATE(sail_int)(&zgsz3244);
            CONVERT_OF(sail_int, mach_int)(&zgsz3244, zinternal_E_take_bits);
            sail_int zgsz3245;
            CREATE(sail_int)(&zgsz3245);
            CONVERT_OF(sail_int, mach_int)(&zgsz3245, INT64_C(1));
            sail_int zgsz3246;
            CREATE(sail_int)(&zgsz3246);
            sub_int(&zgsz3246, zgsz3244, zgsz3245);
            zgaz3105 = CONVERT_OF(mach_int, sail_int)(zgsz3246);
            KILL(sail_int)(&zgsz3246);
            KILL(sail_int)(&zgsz3245);
            KILL(sail_int)(&zgsz3244);
          }
          {
            lbits zgsz3240;
            CREATE(lbits)(&zgsz3240);
            CONVERT_OF(lbits, fbits)(&zgsz3240, zgaz3104, UINT64_C(14) , true);
            sail_int zgsz3241;
            CREATE(sail_int)(&zgsz3241);
            CONVERT_OF(sail_int, mach_int)(&zgsz3241, zgaz3105);
            sail_int zgsz3242;
            CREATE(sail_int)(&zgsz3242);
            CONVERT_OF(sail_int, mach_int)(&zgsz3242, INT64_C(0));
            lbits zgsz3243;
            CREATE(lbits)(&zgsz3243);
            vector_subrange_lbits(&zgsz3243, zgsz3240, zgsz3241, zgsz3242);
            zgaz3107 = CONVERT_OF(fbits, lbits)(zgsz3243, true);
            KILL(lbits)(&zgsz3243);
            KILL(sail_int)(&zgsz3242);
            KILL(sail_int)(&zgsz3241);
            KILL(lbits)(&zgsz3240);
          }
        }
        {
          lbits zgsz3247;
          CREATE(lbits)(&zgsz3247);
          CONVERT_OF(lbits, fbits)(&zgsz3247, zgaz3106, UINT64_C(3) , true);
          lbits zgsz3248;
          CREATE(lbits)(&zgsz3248);
          CONVERT_OF(lbits, fbits)(&zgsz3248, zgaz3107, UINT64_C(3) , true);
          lbits zgsz3249;
          CREATE(lbits)(&zgsz3249);
          append(&zgsz3249, zgsz3247, zgsz3248);
          zE = CONVERT_OF(fbits, lbits)(zgsz3249, true);
          KILL(lbits)(&zgsz3249);
          KILL(lbits)(&zgsz3248);
          KILL(lbits)(&zgsz3247);
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
            sail_int zgsz3254;
            CREATE(sail_int)(&zgsz3254);
            CONVERT_OF(sail_int, mach_int)(&zgsz3254, zcap_mantissa_width);
            sail_int zgsz3255;
            CREATE(sail_int)(&zgsz3255);
            CONVERT_OF(sail_int, mach_int)(&zgsz3255, INT64_C(3));
            sail_int zgsz3256;
            CREATE(sail_int)(&zgsz3256);
            sub_int(&zgsz3256, zgsz3254, zgsz3255);
            zgaz3109 = CONVERT_OF(mach_int, sail_int)(zgsz3256);
            KILL(sail_int)(&zgsz3256);
            KILL(sail_int)(&zgsz3255);
            KILL(sail_int)(&zgsz3254);
          }
          {
            lbits zgsz3250;
            CREATE(lbits)(&zgsz3250);
            CONVERT_OF(lbits, fbits)(&zgsz3250, zgaz3108, UINT64_C(12) , true);
            sail_int zgsz3251;
            CREATE(sail_int)(&zgsz3251);
            CONVERT_OF(sail_int, mach_int)(&zgsz3251, zgaz3109);
            sail_int zgsz3252;
            CREATE(sail_int)(&zgsz3252);
            CONVERT_OF(sail_int, mach_int)(&zgsz3252, INT64_C(3));
            lbits zgsz3253;
            CREATE(lbits)(&zgsz3253);
            vector_subrange_lbits(&zgsz3253, zgsz3250, zgsz3251, zgsz3252);
            zgaz3110 = CONVERT_OF(fbits, lbits)(zgsz3253, true);
            KILL(lbits)(&zgsz3253);
            KILL(sail_int)(&zgsz3252);
            KILL(sail_int)(&zgsz3251);
            KILL(lbits)(&zgsz3250);
          }
        }
        uint64_t zgaz3111;
        {
          sail_int zgsz3260;
          CREATE(sail_int)(&zgsz3260);
          CONVERT_OF(sail_int, mach_int)(&zgsz3260, zinternal_E_take_bits);
          lbits zgsz3261;
          CREATE(lbits)(&zgsz3261);
          sailgen_zzeros_implicit(&zgsz3261, zgsz3260);
          zgaz3111 = CONVERT_OF(fbits, lbits)(zgsz3261, true);
          KILL(lbits)(&zgsz3261);
          KILL(sail_int)(&zgsz3260);
        }
        {
          lbits zgsz3257;
          CREATE(lbits)(&zgsz3257);
          CONVERT_OF(lbits, fbits)(&zgsz3257, zgaz3110, UINT64_C(9) , true);
          lbits zgsz3258;
          CREATE(lbits)(&zgsz3258);
          CONVERT_OF(lbits, fbits)(&zgsz3258, zgaz3111, UINT64_C(3) , true);
          lbits zgsz3259;
          CREATE(lbits)(&zgsz3259);
          append(&zgsz3259, zgsz3257, zgsz3258);
          zT = CONVERT_OF(fbits, lbits)(zgsz3259, true);
          KILL(lbits)(&zgsz3259);
          KILL(lbits)(&zgsz3258);
          KILL(lbits)(&zgsz3257);
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
          sail_int zgsz3266;
          CREATE(sail_int)(&zgsz3266);
          CONVERT_OF(sail_int, mach_int)(&zgsz3266, zcap_mantissa_width);
          sail_int zgsz3267;
          CREATE(sail_int)(&zgsz3267);
          CONVERT_OF(sail_int, mach_int)(&zgsz3267, INT64_C(1));
          sail_int zgsz3268;
          CREATE(sail_int)(&zgsz3268);
          sub_int(&zgsz3268, zgsz3266, zgsz3267);
          zgaz3113 = CONVERT_OF(mach_int, sail_int)(zgsz3268);
          KILL(sail_int)(&zgsz3268);
          KILL(sail_int)(&zgsz3267);
          KILL(sail_int)(&zgsz3266);
        }
        {
          lbits zgsz3262;
          CREATE(lbits)(&zgsz3262);
          CONVERT_OF(lbits, fbits)(&zgsz3262, zgaz3112, UINT64_C(14) , true);
          sail_int zgsz3263;
          CREATE(sail_int)(&zgsz3263);
          CONVERT_OF(sail_int, mach_int)(&zgsz3263, zgaz3113);
          sail_int zgsz3264;
          CREATE(sail_int)(&zgsz3264);
          CONVERT_OF(sail_int, mach_int)(&zgsz3264, INT64_C(3));
          lbits zgsz3265;
          CREATE(lbits)(&zgsz3265);
          vector_subrange_lbits(&zgsz3265, zgsz3262, zgsz3263, zgsz3264);
          zgaz3114 = CONVERT_OF(fbits, lbits)(zgsz3265, true);
          KILL(lbits)(&zgsz3265);
          KILL(sail_int)(&zgsz3264);
          KILL(sail_int)(&zgsz3263);
          KILL(lbits)(&zgsz3262);
        }
      }
      uint64_t zgaz3115;
      {
        sail_int zgsz3272;
        CREATE(sail_int)(&zgsz3272);
        CONVERT_OF(sail_int, mach_int)(&zgsz3272, zinternal_E_take_bits);
        lbits zgsz3273;
        CREATE(lbits)(&zgsz3273);
        sailgen_zzeros_implicit(&zgsz3273, zgsz3272);
        zgaz3115 = CONVERT_OF(fbits, lbits)(zgsz3273, true);
        KILL(lbits)(&zgsz3273);
        KILL(sail_int)(&zgsz3272);
      }
      {
        lbits zgsz3269;
        CREATE(lbits)(&zgsz3269);
        CONVERT_OF(lbits, fbits)(&zgsz3269, zgaz3114, UINT64_C(11) , true);
        lbits zgsz3270;
        CREATE(lbits)(&zgsz3270);
        CONVERT_OF(lbits, fbits)(&zgsz3270, zgaz3115, UINT64_C(3) , true);
        lbits zgsz3271;
        CREATE(lbits)(&zgsz3271);
        append(&zgsz3271, zgsz3269, zgsz3270);
        zBs = CONVERT_OF(fbits, lbits)(zgsz3271, true);
        KILL(lbits)(&zgsz3271);
        KILL(lbits)(&zgsz3270);
        KILL(lbits)(&zgsz3269);
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
          sail_int zgsz3278;
          CREATE(sail_int)(&zgsz3278);
          CONVERT_OF(sail_int, mach_int)(&zgsz3278, zcap_mantissa_width);
          sail_int zgsz3279;
          CREATE(sail_int)(&zgsz3279);
          CONVERT_OF(sail_int, mach_int)(&zgsz3279, INT64_C(3));
          sail_int zgsz3280;
          CREATE(sail_int)(&zgsz3280);
          sub_int(&zgsz3280, zgsz3278, zgsz3279);
          zgaz3169 = CONVERT_OF(mach_int, sail_int)(zgsz3280);
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
          CONVERT_OF(sail_int, mach_int)(&zgsz3275, zgaz3169);
          sail_int zgsz3276;
          CREATE(sail_int)(&zgsz3276);
          CONVERT_OF(sail_int, mach_int)(&zgsz3276, INT64_C(0));
          lbits zgsz3277;
          CREATE(lbits)(&zgsz3277);
          vector_subrange_lbits(&zgsz3277, zgsz3274, zgsz3275, zgsz3276);
          zgaz3170 = CONVERT_OF(fbits, lbits)(zgsz3277, true);
          KILL(lbits)(&zgsz3277);
          KILL(sail_int)(&zgsz3276);
          KILL(sail_int)(&zgsz3275);
          KILL(lbits)(&zgsz3274);
        }
      }
      {
        lbits zgsz3281;
        CREATE(lbits)(&zgsz3281);
        CONVERT_OF(lbits, fbits)(&zgsz3281, zT, UINT64_C(12) , true);
        lbits zgsz3282;
        CREATE(lbits)(&zgsz3282);
        CONVERT_OF(lbits, fbits)(&zgsz3282, zgaz3170, UINT64_C(12) , true);
        zgaz3171 = sailgen_z8operatorz0zI_uz9(zgsz3281, zgsz3282);
        KILL(lbits)(&zgsz3282);
        KILL(lbits)(&zgsz3281);
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
          sail_int zgsz3290;
          CREATE(sail_int)(&zgsz3290);
          CONVERT_OF(sail_int, mach_int)(&zgsz3290, zcap_mantissa_width);
          sail_int zgsz3291;
          CREATE(sail_int)(&zgsz3291);
          CONVERT_OF(sail_int, mach_int)(&zgsz3291, INT64_C(1));
          sail_int zgsz3292;
          CREATE(sail_int)(&zgsz3292);
          sub_int(&zgsz3292, zgsz3290, zgsz3291);
          zgaz3165 = CONVERT_OF(mach_int, sail_int)(zgsz3292);
          KILL(sail_int)(&zgsz3292);
          KILL(sail_int)(&zgsz3291);
          KILL(sail_int)(&zgsz3290);
        }
        int64_t zgaz3166;
        {
          sail_int zgsz3287;
          CREATE(sail_int)(&zgsz3287);
          CONVERT_OF(sail_int, mach_int)(&zgsz3287, zcap_mantissa_width);
          sail_int zgsz3288;
          CREATE(sail_int)(&zgsz3288);
          CONVERT_OF(sail_int, mach_int)(&zgsz3288, INT64_C(2));
          sail_int zgsz3289;
          CREATE(sail_int)(&zgsz3289);
          sub_int(&zgsz3289, zgsz3287, zgsz3288);
          zgaz3166 = CONVERT_OF(mach_int, sail_int)(zgsz3289);
          KILL(sail_int)(&zgsz3289);
          KILL(sail_int)(&zgsz3288);
          KILL(sail_int)(&zgsz3287);
        }
        {
          lbits zgsz3283;
          CREATE(lbits)(&zgsz3283);
          CONVERT_OF(lbits, fbits)(&zgsz3283, zBs, UINT64_C(14) , true);
          sail_int zgsz3284;
          CREATE(sail_int)(&zgsz3284);
          CONVERT_OF(sail_int, mach_int)(&zgsz3284, zgaz3165);
          sail_int zgsz3285;
          CREATE(sail_int)(&zgsz3285);
          CONVERT_OF(sail_int, mach_int)(&zgsz3285, zgaz3166);
          lbits zgsz3286;
          CREATE(lbits)(&zgsz3286);
          vector_subrange_lbits(&zgsz3286, zgsz3283, zgsz3284, zgsz3285);
          zgaz3167 = CONVERT_OF(fbits, lbits)(zgsz3286, true);
          KILL(lbits)(&zgsz3286);
          KILL(sail_int)(&zgsz3285);
          KILL(sail_int)(&zgsz3284);
          KILL(lbits)(&zgsz3283);
        }
      }
      {
        lbits zgsz3293;
        CREATE(lbits)(&zgsz3293);
        CONVERT_OF(lbits, fbits)(&zgsz3293, zgaz3167, UINT64_C(2) , true);
        lbits zgsz3294;
        CREATE(lbits)(&zgsz3294);
        CONVERT_OF(lbits, fbits)(&zgsz3294, zlenMSBs, UINT64_C(2) , true);
        lbits zgsz3295;
        CREATE(lbits)(&zgsz3295);
        add_bits(&zgsz3295, zgsz3293, zgsz3294);
        zgaz3168 = CONVERT_OF(fbits, lbits)(zgsz3295, true);
        KILL(lbits)(&zgsz3295);
        KILL(lbits)(&zgsz3294);
        KILL(lbits)(&zgsz3293);
      }
    }
    {
      lbits zgsz3296;
      CREATE(lbits)(&zgsz3296);
      CONVERT_OF(lbits, fbits)(&zgsz3296, zgaz3168, UINT64_C(2) , true);
      lbits zgsz3297;
      CREATE(lbits)(&zgsz3297);
      CONVERT_OF(lbits, fbits)(&zgsz3297, zcarry_out, UINT64_C(2) , true);
      lbits zgsz3298;
      CREATE(lbits)(&zgsz3298);
      add_bits(&zgsz3298, zgsz3296, zgsz3297);
      zTtop2 = CONVERT_OF(fbits, lbits)(zgsz3298, true);
      KILL(lbits)(&zgsz3298);
      KILL(lbits)(&zgsz3297);
      KILL(lbits)(&zgsz3296);
    }
  }
  struct zCapability zgaz3164;
  {
    uint64_t zgaz3120;
    {
      bool zgaz3116;
      {
        sail_int zgsz3309;
        CREATE(sail_int)(&zgsz3309);
        CONVERT_OF(sail_int, mach_int)(&zgsz3309, zcap_uperms_width);
        sail_int zgsz3310;
        CREATE(sail_int)(&zgsz3310);
        CONVERT_OF(sail_int, mach_int)(&zgsz3310, INT64_C(0));
        zgaz3116 = gt(zgsz3309, zgsz3310);
        KILL(sail_int)(&zgsz3310);
        KILL(sail_int)(&zgsz3309);
      }
      uint64_t zgaz3118;
      zgaz3118 = zc.zperms;
      int64_t zgaz3119;
      {
        int64_t zgaz3117;
        {
          sail_int zgsz3302;
          CREATE(sail_int)(&zgsz3302);
          CONVERT_OF(sail_int, mach_int)(&zgsz3302, zcap_hperms_width);
          sail_int zgsz3303;
          CREATE(sail_int)(&zgsz3303);
          CONVERT_OF(sail_int, mach_int)(&zgsz3303, zcap_uperms_width);
          sail_int zgsz3304;
          CREATE(sail_int)(&zgsz3304);
          add_int(&zgsz3304, zgsz3302, zgsz3303);
          zgaz3117 = CONVERT_OF(mach_int, sail_int)(zgsz3304);
          KILL(sail_int)(&zgsz3304);
          KILL(sail_int)(&zgsz3303);
          KILL(sail_int)(&zgsz3302);
        }
        {
          sail_int zgsz3299;
          CREATE(sail_int)(&zgsz3299);
          CONVERT_OF(sail_int, mach_int)(&zgsz3299, zgaz3117);
          sail_int zgsz3300;
          CREATE(sail_int)(&zgsz3300);
          CONVERT_OF(sail_int, mach_int)(&zgsz3300, INT64_C(1));
          sail_int zgsz3301;
          CREATE(sail_int)(&zgsz3301);
          sub_int(&zgsz3301, zgsz3299, zgsz3300);
          zgaz3119 = CONVERT_OF(mach_int, sail_int)(zgsz3301);
          KILL(sail_int)(&zgsz3301);
          KILL(sail_int)(&zgsz3300);
          KILL(sail_int)(&zgsz3299);
        }
      }
      {
        lbits zgsz3305;
        CREATE(lbits)(&zgsz3305);
        CONVERT_OF(lbits, fbits)(&zgsz3305, zgaz3118, UINT64_C(16) , true);
        sail_int zgsz3306;
        CREATE(sail_int)(&zgsz3306);
        CONVERT_OF(sail_int, mach_int)(&zgsz3306, zgaz3119);
        sail_int zgsz3307;
        CREATE(sail_int)(&zgsz3307);
        CONVERT_OF(sail_int, mach_int)(&zgsz3307, zcap_hperms_width);
        lbits zgsz3308;
        CREATE(lbits)(&zgsz3308);
        vector_subrange_lbits(&zgsz3308, zgsz3305, zgsz3306, zgsz3307);
        zgaz3120 = CONVERT_OF(fbits, lbits)(zgsz3308, true);
        KILL(lbits)(&zgsz3308);
        KILL(sail_int)(&zgsz3307);
        KILL(sail_int)(&zgsz3306);
        KILL(lbits)(&zgsz3305);
      }
    }
    bool zgaz3123;
    {
      fbits zgaz3122;
      {
        uint64_t zgaz3121;
        zgaz3121 = zc.zperms;
        {
          lbits zgsz3311;
          CREATE(lbits)(&zgsz3311);
          CONVERT_OF(lbits, fbits)(&zgsz3311, zgaz3121, UINT64_C(16) , true);
          sail_int zgsz3312;
          CREATE(sail_int)(&zgsz3312);
          CONVERT_OF(sail_int, mach_int)(&zgsz3312, INT64_C(11));
          zgaz3122 = bitvector_access(zgsz3311, zgsz3312);
          KILL(sail_int)(&zgsz3312);
          KILL(lbits)(&zgsz3311);
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
          lbits zgsz3313;
          CREATE(lbits)(&zgsz3313);
          CONVERT_OF(lbits, fbits)(&zgsz3313, zgaz3124, UINT64_C(16) , true);
          sail_int zgsz3314;
          CREATE(sail_int)(&zgsz3314);
          CONVERT_OF(sail_int, mach_int)(&zgsz3314, INT64_C(10));
          zgaz3125 = bitvector_access(zgsz3313, zgsz3314);
          KILL(sail_int)(&zgsz3314);
          KILL(lbits)(&zgsz3313);
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
          lbits zgsz3315;
          CREATE(lbits)(&zgsz3315);
          CONVERT_OF(lbits, fbits)(&zgsz3315, zgaz3127, UINT64_C(16) , true);
          sail_int zgsz3316;
          CREATE(sail_int)(&zgsz3316);
          CONVERT_OF(sail_int, mach_int)(&zgsz3316, INT64_C(9));
          zgaz3128 = bitvector_access(zgsz3315, zgsz3316);
          KILL(sail_int)(&zgsz3316);
          KILL(lbits)(&zgsz3315);
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
          lbits zgsz3317;
          CREATE(lbits)(&zgsz3317);
          CONVERT_OF(lbits, fbits)(&zgsz3317, zgaz3130, UINT64_C(16) , true);
          sail_int zgsz3318;
          CREATE(sail_int)(&zgsz3318);
          CONVERT_OF(sail_int, mach_int)(&zgsz3318, INT64_C(8));
          zgaz3131 = bitvector_access(zgsz3317, zgsz3318);
          KILL(sail_int)(&zgsz3318);
          KILL(lbits)(&zgsz3317);
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
          lbits zgsz3319;
          CREATE(lbits)(&zgsz3319);
          CONVERT_OF(lbits, fbits)(&zgsz3319, zgaz3133, UINT64_C(16) , true);
          sail_int zgsz3320;
          CREATE(sail_int)(&zgsz3320);
          CONVERT_OF(sail_int, mach_int)(&zgsz3320, INT64_C(7));
          zgaz3134 = bitvector_access(zgsz3319, zgsz3320);
          KILL(sail_int)(&zgsz3320);
          KILL(lbits)(&zgsz3319);
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
          lbits zgsz3321;
          CREATE(lbits)(&zgsz3321);
          CONVERT_OF(lbits, fbits)(&zgsz3321, zgaz3136, UINT64_C(16) , true);
          sail_int zgsz3322;
          CREATE(sail_int)(&zgsz3322);
          CONVERT_OF(sail_int, mach_int)(&zgsz3322, INT64_C(6));
          zgaz3137 = bitvector_access(zgsz3321, zgsz3322);
          KILL(sail_int)(&zgsz3322);
          KILL(lbits)(&zgsz3321);
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
          lbits zgsz3323;
          CREATE(lbits)(&zgsz3323);
          CONVERT_OF(lbits, fbits)(&zgsz3323, zgaz3139, UINT64_C(16) , true);
          sail_int zgsz3324;
          CREATE(sail_int)(&zgsz3324);
          CONVERT_OF(sail_int, mach_int)(&zgsz3324, INT64_C(5));
          zgaz3140 = bitvector_access(zgsz3323, zgsz3324);
          KILL(sail_int)(&zgsz3324);
          KILL(lbits)(&zgsz3323);
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
          lbits zgsz3325;
          CREATE(lbits)(&zgsz3325);
          CONVERT_OF(lbits, fbits)(&zgsz3325, zgaz3142, UINT64_C(16) , true);
          sail_int zgsz3326;
          CREATE(sail_int)(&zgsz3326);
          CONVERT_OF(sail_int, mach_int)(&zgsz3326, INT64_C(4));
          zgaz3143 = bitvector_access(zgsz3325, zgsz3326);
          KILL(sail_int)(&zgsz3326);
          KILL(lbits)(&zgsz3325);
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
          lbits zgsz3327;
          CREATE(lbits)(&zgsz3327);
          CONVERT_OF(lbits, fbits)(&zgsz3327, zgaz3145, UINT64_C(16) , true);
          sail_int zgsz3328;
          CREATE(sail_int)(&zgsz3328);
          CONVERT_OF(sail_int, mach_int)(&zgsz3328, INT64_C(3));
          zgaz3146 = bitvector_access(zgsz3327, zgsz3328);
          KILL(sail_int)(&zgsz3328);
          KILL(lbits)(&zgsz3327);
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
          lbits zgsz3329;
          CREATE(lbits)(&zgsz3329);
          CONVERT_OF(lbits, fbits)(&zgsz3329, zgaz3148, UINT64_C(16) , true);
          sail_int zgsz3330;
          CREATE(sail_int)(&zgsz3330);
          CONVERT_OF(sail_int, mach_int)(&zgsz3330, INT64_C(2));
          zgaz3149 = bitvector_access(zgsz3329, zgsz3330);
          KILL(sail_int)(&zgsz3330);
          KILL(lbits)(&zgsz3329);
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
          lbits zgsz3331;
          CREATE(lbits)(&zgsz3331);
          CONVERT_OF(lbits, fbits)(&zgsz3331, zgaz3151, UINT64_C(16) , true);
          sail_int zgsz3332;
          CREATE(sail_int)(&zgsz3332);
          CONVERT_OF(sail_int, mach_int)(&zgsz3332, INT64_C(1));
          zgaz3152 = bitvector_access(zgsz3331, zgsz3332);
          KILL(sail_int)(&zgsz3332);
          KILL(lbits)(&zgsz3331);
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
          lbits zgsz3333;
          CREATE(lbits)(&zgsz3333);
          CONVERT_OF(lbits, fbits)(&zgsz3333, zgaz3154, UINT64_C(16) , true);
          sail_int zgsz3334;
          CREATE(sail_int)(&zgsz3334);
          CONVERT_OF(sail_int, mach_int)(&zgsz3334, INT64_C(0));
          zgaz3155 = bitvector_access(zgsz3333, zgsz3334);
          KILL(sail_int)(&zgsz3334);
          KILL(lbits)(&zgsz3333);
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
          lbits zgsz3335;
          CREATE(lbits)(&zgsz3335);
          CONVERT_OF(lbits, fbits)(&zgsz3335, zgaz3158, UINT64_C(1) , true);
          sail_int zgsz3336;
          CREATE(sail_int)(&zgsz3336);
          CONVERT_OF(sail_int, mach_int)(&zgsz3336, INT64_C(0));
          zgaz3159 = bitvector_access(zgsz3335, zgsz3336);
          KILL(sail_int)(&zgsz3336);
          KILL(lbits)(&zgsz3335);
        }
      }
      zgaz3160 = sailgen_bit_to_bool(zgaz3159);
    }
    uint64_t zgaz3161;
    {
      lbits zgsz3337;
      CREATE(lbits)(&zgsz3337);
      CONVERT_OF(lbits, fbits)(&zgsz3337, zTtop2, UINT64_C(2) , true);
      lbits zgsz3338;
      CREATE(lbits)(&zgsz3338);
      CONVERT_OF(lbits, fbits)(&zgsz3338, zT, UINT64_C(12) , true);
      lbits zgsz3339;
      CREATE(lbits)(&zgsz3339);
      append(&zgsz3339, zgsz3337, zgsz3338);
      zgaz3161 = CONVERT_OF(fbits, lbits)(zgsz3339, true);
      KILL(lbits)(&zgsz3339);
      KILL(lbits)(&zgsz3338);
      KILL(lbits)(&zgsz3337);
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
  struct zCapability zcbz345 = { .zuperms = UINT64_C(0xdeadc0de), .ztag = false, .zreserved = UINT64_C(0xdeadc0de), .zpermit_unseal = false, .zpermit_store_local_cap = false, .zpermit_store_cap = false, .zpermit_store = false, .zpermit_set_CID = false, .zpermit_seal = false, .zpermit_load_cap = false, .zpermit_load = false, .zpermit_execute = false, .zpermit_cinvoke = false, .zotype = UINT64_C(0xdeadc0de), .zinternal_E = false, .zglobal = false, .zflag_cap_mode = false, .zaddress = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zT = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  return zcbz345;
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
  struct zCapability zcbz346 = { .zuperms = UINT64_C(0xdeadc0de), .ztag = false, .zreserved = UINT64_C(0xdeadc0de), .zpermit_unseal = false, .zpermit_store_local_cap = false, .zpermit_store_cap = false, .zpermit_store = false, .zpermit_set_CID = false, .zpermit_seal = false, .zpermit_load_cap = false, .zpermit_load = false, .zpermit_execute = false, .zpermit_cinvoke = false, .zotype = UINT64_C(0xdeadc0de), .zinternal_E = false, .zglobal = false, .zflag_cap_mode = false, .zaddress = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zT = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  return zcbz346;
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
      sail_int zgsz3350;
      CREATE(sail_int)(&zgsz3350);
      CONVERT_OF(sail_int, mach_int)(&zgsz3350, zcap_mantissa_width);
      sail_int zgsz3351;
      CREATE(sail_int)(&zgsz3351);
      CONVERT_OF(sail_int, mach_int)(&zgsz3351, INT64_C(3));
      sail_int zgsz3352;
      CREATE(sail_int)(&zgsz3352);
      sub_int(&zgsz3352, zgsz3350, zgsz3351);
      zgaz3202 = CONVERT_OF(mach_int, sail_int)(zgsz3352);
      KILL(sail_int)(&zgsz3352);
      KILL(sail_int)(&zgsz3351);
      KILL(sail_int)(&zgsz3350);
    }
    {
      lbits zgsz3346;
      CREATE(lbits)(&zgsz3346);
      CONVERT_OF(lbits, fbits)(&zgsz3346, zgaz3201, UINT64_C(14) , true);
      sail_int zgsz3347;
      CREATE(sail_int)(&zgsz3347);
      CONVERT_OF(sail_int, mach_int)(&zgsz3347, zgaz3202);
      sail_int zgsz3348;
      CREATE(sail_int)(&zgsz3348);
      CONVERT_OF(sail_int, mach_int)(&zgsz3348, zinternal_E_take_bits);
      lbits zgsz3349;
      CREATE(lbits)(&zgsz3349);
      vector_subrange_lbits(&zgsz3349, zgsz3346, zgsz3347, zgsz3348);
      zt_hi = CONVERT_OF(fbits, lbits)(zgsz3349, true);
      KILL(lbits)(&zgsz3349);
      KILL(sail_int)(&zgsz3348);
      KILL(sail_int)(&zgsz3347);
      KILL(lbits)(&zgsz3346);
    }
  }
  uint64_t zt_lo;
  {
    uint64_t zgaz3199;
    zgaz3199 = zcap.zT;
    int64_t zgaz3200;
    {
      sail_int zgsz3357;
      CREATE(sail_int)(&zgsz3357);
      CONVERT_OF(sail_int, mach_int)(&zgsz3357, zinternal_E_take_bits);
      sail_int zgsz3358;
      CREATE(sail_int)(&zgsz3358);
      CONVERT_OF(sail_int, mach_int)(&zgsz3358, INT64_C(1));
      sail_int zgsz3359;
      CREATE(sail_int)(&zgsz3359);
      sub_int(&zgsz3359, zgsz3357, zgsz3358);
      zgaz3200 = CONVERT_OF(mach_int, sail_int)(zgsz3359);
      KILL(sail_int)(&zgsz3359);
      KILL(sail_int)(&zgsz3358);
      KILL(sail_int)(&zgsz3357);
    }
    {
      lbits zgsz3353;
      CREATE(lbits)(&zgsz3353);
      CONVERT_OF(lbits, fbits)(&zgsz3353, zgaz3199, UINT64_C(14) , true);
      sail_int zgsz3354;
      CREATE(sail_int)(&zgsz3354);
      CONVERT_OF(sail_int, mach_int)(&zgsz3354, zgaz3200);
      sail_int zgsz3355;
      CREATE(sail_int)(&zgsz3355);
      CONVERT_OF(sail_int, mach_int)(&zgsz3355, INT64_C(0));
      lbits zgsz3356;
      CREATE(lbits)(&zgsz3356);
      vector_subrange_lbits(&zgsz3356, zgsz3353, zgsz3354, zgsz3355);
      zt_lo = CONVERT_OF(fbits, lbits)(zgsz3356, true);
      KILL(lbits)(&zgsz3356);
      KILL(sail_int)(&zgsz3355);
      KILL(sail_int)(&zgsz3354);
      KILL(lbits)(&zgsz3353);
    }
  }
  uint64_t zb_hi;
  {
    uint64_t zgaz3197;
    zgaz3197 = zcap.zB;
    int64_t zgaz3198;
    {
      sail_int zgsz3364;
      CREATE(sail_int)(&zgsz3364);
      CONVERT_OF(sail_int, mach_int)(&zgsz3364, zcap_mantissa_width);
      sail_int zgsz3365;
      CREATE(sail_int)(&zgsz3365);
      CONVERT_OF(sail_int, mach_int)(&zgsz3365, INT64_C(1));
      sail_int zgsz3366;
      CREATE(sail_int)(&zgsz3366);
      sub_int(&zgsz3366, zgsz3364, zgsz3365);
      zgaz3198 = CONVERT_OF(mach_int, sail_int)(zgsz3366);
      KILL(sail_int)(&zgsz3366);
      KILL(sail_int)(&zgsz3365);
      KILL(sail_int)(&zgsz3364);
    }
    {
      lbits zgsz3360;
      CREATE(lbits)(&zgsz3360);
      CONVERT_OF(lbits, fbits)(&zgsz3360, zgaz3197, UINT64_C(14) , true);
      sail_int zgsz3361;
      CREATE(sail_int)(&zgsz3361);
      CONVERT_OF(sail_int, mach_int)(&zgsz3361, zgaz3198);
      sail_int zgsz3362;
      CREATE(sail_int)(&zgsz3362);
      CONVERT_OF(sail_int, mach_int)(&zgsz3362, zinternal_E_take_bits);
      lbits zgsz3363;
      CREATE(lbits)(&zgsz3363);
      vector_subrange_lbits(&zgsz3363, zgsz3360, zgsz3361, zgsz3362);
      zb_hi = CONVERT_OF(fbits, lbits)(zgsz3363, true);
      KILL(lbits)(&zgsz3363);
      KILL(sail_int)(&zgsz3362);
      KILL(sail_int)(&zgsz3361);
      KILL(lbits)(&zgsz3360);
    }
  }
  uint64_t zb_lo;
  {
    uint64_t zgaz3195;
    zgaz3195 = zcap.zB;
    int64_t zgaz3196;
    {
      sail_int zgsz3371;
      CREATE(sail_int)(&zgsz3371);
      CONVERT_OF(sail_int, mach_int)(&zgsz3371, zinternal_E_take_bits);
      sail_int zgsz3372;
      CREATE(sail_int)(&zgsz3372);
      CONVERT_OF(sail_int, mach_int)(&zgsz3372, INT64_C(1));
      sail_int zgsz3373;
      CREATE(sail_int)(&zgsz3373);
      sub_int(&zgsz3373, zgsz3371, zgsz3372);
      zgaz3196 = CONVERT_OF(mach_int, sail_int)(zgsz3373);
      KILL(sail_int)(&zgsz3373);
      KILL(sail_int)(&zgsz3372);
      KILL(sail_int)(&zgsz3371);
    }
    {
      lbits zgsz3367;
      CREATE(lbits)(&zgsz3367);
      CONVERT_OF(lbits, fbits)(&zgsz3367, zgaz3195, UINT64_C(14) , true);
      sail_int zgsz3368;
      CREATE(sail_int)(&zgsz3368);
      CONVERT_OF(sail_int, mach_int)(&zgsz3368, zgaz3196);
      sail_int zgsz3369;
      CREATE(sail_int)(&zgsz3369);
      CONVERT_OF(sail_int, mach_int)(&zgsz3369, INT64_C(0));
      lbits zgsz3370;
      CREATE(lbits)(&zgsz3370);
      vector_subrange_lbits(&zgsz3370, zgsz3367, zgsz3368, zgsz3369);
      zb_lo = CONVERT_OF(fbits, lbits)(zgsz3370, true);
      KILL(lbits)(&zgsz3370);
      KILL(sail_int)(&zgsz3369);
      KILL(sail_int)(&zgsz3368);
      KILL(lbits)(&zgsz3367);
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
            sail_int zgsz3377;
            CREATE(sail_int)(&zgsz3377);
            CONVERT_OF(sail_int, mach_int)(&zgsz3377, INT64_C(2));
            sail_int zgsz3378;
            CREATE(sail_int)(&zgsz3378);
            CONVERT_OF(sail_int, mach_int)(&zgsz3378, zinternal_E_take_bits);
            sail_int zgsz3379;
            CREATE(sail_int)(&zgsz3379);
            mult_int(&zgsz3379, zgsz3377, zgsz3378);
            zgaz3176 = CONVERT_OF(mach_int, sail_int)(zgsz3379);
            KILL(sail_int)(&zgsz3379);
            KILL(sail_int)(&zgsz3378);
            KILL(sail_int)(&zgsz3377);
          }
          {
            sail_int zgsz3374;
            CREATE(sail_int)(&zgsz3374);
            CONVERT_OF(sail_int, mach_int)(&zgsz3374, zgaz3176);
            sail_int zgsz3375;
            CREATE(sail_int)(&zgsz3375);
            CONVERT_OF(sail_int, mach_int)(&zgsz3375, INT64_C(1));
            sail_int zgsz3376;
            CREATE(sail_int)(&zgsz3376);
            sub_int(&zgsz3376, zgsz3374, zgsz3375);
            zgaz3178 = CONVERT_OF(mach_int, sail_int)(zgsz3376);
            KILL(sail_int)(&zgsz3376);
            KILL(sail_int)(&zgsz3375);
            KILL(sail_int)(&zgsz3374);
          }
        }
        {
          lbits zgsz3380;
          CREATE(lbits)(&zgsz3380);
          CONVERT_OF(lbits, fbits)(&zgsz3380, zgaz3177, UINT64_C(6) , true);
          sail_int zgsz3381;
          CREATE(sail_int)(&zgsz3381);
          CONVERT_OF(sail_int, mach_int)(&zgsz3381, zgaz3178);
          sail_int zgsz3382;
          CREATE(sail_int)(&zgsz3382);
          CONVERT_OF(sail_int, mach_int)(&zgsz3382, zinternal_E_take_bits);
          lbits zgsz3383;
          CREATE(lbits)(&zgsz3383);
          vector_subrange_lbits(&zgsz3383, zgsz3380, zgsz3381, zgsz3382);
          zt_lo = CONVERT_OF(fbits, lbits)(zgsz3383, true);
          KILL(lbits)(&zgsz3383);
          KILL(sail_int)(&zgsz3382);
          KILL(sail_int)(&zgsz3381);
          KILL(lbits)(&zgsz3380);
        }
        unit zgsz344;
        zgsz344 = UNIT;
      }
      uint64_t zgaz3179;
      zgaz3179 = zcap.zE;
      int64_t zgaz3180;
      {
        sail_int zgsz3388;
        CREATE(sail_int)(&zgsz3388);
        CONVERT_OF(sail_int, mach_int)(&zgsz3388, zinternal_E_take_bits);
        sail_int zgsz3389;
        CREATE(sail_int)(&zgsz3389);
        CONVERT_OF(sail_int, mach_int)(&zgsz3389, INT64_C(1));
        sail_int zgsz3390;
        CREATE(sail_int)(&zgsz3390);
        sub_int(&zgsz3390, zgsz3388, zgsz3389);
        zgaz3180 = CONVERT_OF(mach_int, sail_int)(zgsz3390);
        KILL(sail_int)(&zgsz3390);
        KILL(sail_int)(&zgsz3389);
        KILL(sail_int)(&zgsz3388);
      }
      {
        lbits zgsz3384;
        CREATE(lbits)(&zgsz3384);
        CONVERT_OF(lbits, fbits)(&zgsz3384, zgaz3179, UINT64_C(6) , true);
        sail_int zgsz3385;
        CREATE(sail_int)(&zgsz3385);
        CONVERT_OF(sail_int, mach_int)(&zgsz3385, zgaz3180);
        sail_int zgsz3386;
        CREATE(sail_int)(&zgsz3386);
        CONVERT_OF(sail_int, mach_int)(&zgsz3386, INT64_C(0));
        lbits zgsz3387;
        CREATE(lbits)(&zgsz3387);
        vector_subrange_lbits(&zgsz3387, zgsz3384, zgsz3385, zgsz3386);
        zb_lo = CONVERT_OF(fbits, lbits)(zgsz3387, true);
        KILL(lbits)(&zgsz3387);
        KILL(sail_int)(&zgsz3386);
        KILL(sail_int)(&zgsz3385);
        KILL(lbits)(&zgsz3384);
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
        lbits zgsz3391;
        CREATE(lbits)(&zgsz3391);
        CONVERT_OF(lbits, fbits)(&zgsz3391, zgaz3181, UINT64_C(4) , true);
        lbits zgsz3392;
        CREATE(lbits)(&zgsz3392);
        CONVERT_OF(lbits, fbits)(&zgsz3392, zgaz3182, UINT64_C(12) , true);
        lbits zgsz3393;
        CREATE(lbits)(&zgsz3393);
        append(&zgsz3393, zgsz3391, zgsz3392);
        zgaz3183 = CONVERT_OF(fbits, lbits)(zgsz3393, true);
        KILL(lbits)(&zgsz3393);
        KILL(lbits)(&zgsz3392);
        KILL(lbits)(&zgsz3391);
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
        lbits zgsz3394;
        CREATE(lbits)(&zgsz3394);
        CONVERT_OF(lbits, fbits)(&zgsz3394, zgaz3189, UINT64_C(1) , true);
        sail_int zgsz3395;
        CREATE(sail_int)(&zgsz3395);
        CONVERT_OF(sail_int, mach_int)(&zgsz3395, INT64_C(0));
        zgaz3190 = bitvector_access(zgsz3394, zgsz3395);
        KILL(sail_int)(&zgsz3395);
        KILL(lbits)(&zgsz3394);
      }
    }
    uint64_t zgaz3191;
    {
      lbits zgsz3399;
      CREATE(lbits)(&zgsz3399);
      CONVERT_OF(lbits, fbits)(&zgsz3399, zt_hi, UINT64_C(9) , true);
      lbits zgsz3400;
      CREATE(lbits)(&zgsz3400);
      CONVERT_OF(lbits, fbits)(&zgsz3400, zt_lo, UINT64_C(3) , true);
      lbits zgsz3401;
      CREATE(lbits)(&zgsz3401);
      append(&zgsz3401, zgsz3399, zgsz3400);
      zgaz3191 = CONVERT_OF(fbits, lbits)(zgsz3401, true);
      KILL(lbits)(&zgsz3401);
      KILL(lbits)(&zgsz3400);
      KILL(lbits)(&zgsz3399);
    }
    uint64_t zgaz3192;
    {
      lbits zgsz3396;
      CREATE(lbits)(&zgsz3396);
      CONVERT_OF(lbits, fbits)(&zgsz3396, zb_hi, UINT64_C(11) , true);
      lbits zgsz3397;
      CREATE(lbits)(&zgsz3397);
      CONVERT_OF(lbits, fbits)(&zgsz3397, zb_lo, UINT64_C(3) , true);
      lbits zgsz3398;
      CREATE(lbits)(&zgsz3398);
      append(&zgsz3398, zgsz3396, zgsz3397);
      zgaz3192 = CONVERT_OF(fbits, lbits)(zgsz3398, true);
      KILL(lbits)(&zgsz3398);
      KILL(lbits)(&zgsz3397);
      KILL(lbits)(&zgsz3396);
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
  struct zEncCapability zcbz347 = { .zreserved = UINT64_C(0xdeadc0de), .zperms = UINT64_C(0xdeadc0de), .zotype = UINT64_C(0xdeadc0de), .zinternal_E = UINT64_C(0), .zflags = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  return zcbz347;
}

static void sailgen_capToBits(lbits *rop, struct zCapability);

static void sailgen_capToBits(lbits *zcbz321, struct zCapability zcap)
{
  __label__ end_function_98, end_block_exception_99, end_function_187;

  struct zEncCapability zgaz3203;
  zgaz3203 = sailgen_capToEncCap(zcap);
  sailgen_encCapToBits((*(&zcbz321)), zgaz3203);

end_function_98: ;
  goto end_function_187;
end_block_exception_99: ;
  goto end_function_187;
end_function_187: ;
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
  __label__ end_function_102, end_block_exception_103, end_function_186;

  lbits zgaz3204;
  CREATE(lbits)(&zgaz3204);
  sailgen_capToBits(&zgaz3204, zcap);
  xor_bits((*(&zcbz322)), zgaz3204, znull_cap_bits);
  KILL(lbits)(&zgaz3204);
end_function_102: ;
  goto end_function_186;
end_block_exception_103: ;
  goto end_function_186;
end_function_186: ;
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
  struct zCapability zcbz348 = { .zuperms = UINT64_C(0xdeadc0de), .ztag = false, .zreserved = UINT64_C(0xdeadc0de), .zpermit_unseal = false, .zpermit_store_local_cap = false, .zpermit_store_cap = false, .zpermit_store = false, .zpermit_set_CID = false, .zpermit_seal = false, .zpermit_load_cap = false, .zpermit_load = false, .zpermit_execute = false, .zpermit_cinvoke = false, .zotype = UINT64_C(0xdeadc0de), .zinternal_E = false, .zglobal = false, .zflag_cap_mode = false, .zaddress = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zT = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  return zcbz348;
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
  __label__ end_function_108, end_block_exception_109, end_function_185;

  int64_t zE;
  {
    int64_t zgaz3239;
    {
      uint64_t zgaz3238;
      zgaz3238 = zc.zE;
      {
        lbits zgsz3402;
        CREATE(lbits)(&zgsz3402);
        CONVERT_OF(lbits, fbits)(&zgsz3402, zgaz3238, UINT64_C(6) , true);
        sail_int zgsz3403;
        CREATE(sail_int)(&zgsz3403);
        sail_unsigned(&zgsz3403, zgsz3402);
        zgaz3239 = CONVERT_OF(mach_int, sail_int)(zgsz3403);
        KILL(sail_int)(&zgsz3403);
        KILL(lbits)(&zgsz3402);
      }
    }
    {
      sail_int zgsz3404;
      CREATE(sail_int)(&zgsz3404);
      CONVERT_OF(sail_int, mach_int)(&zgsz3404, zcap_max_E);
      sail_int zgsz3405;
      CREATE(sail_int)(&zgsz3405);
      CONVERT_OF(sail_int, mach_int)(&zgsz3405, zgaz3239);
      sail_int zgsz3406;
      CREATE(sail_int)(&zgsz3406);
      min_int(&zgsz3406, zgsz3404, zgsz3405);
      zE = CONVERT_OF(mach_int, sail_int)(zgsz3406);
      KILL(sail_int)(&zgsz3406);
      KILL(sail_int)(&zgsz3405);
      KILL(sail_int)(&zgsz3404);
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
          sail_int zgsz3410;
          CREATE(sail_int)(&zgsz3410);
          CONVERT_OF(sail_int, mach_int)(&zgsz3410, zE);
          sail_int zgsz3411;
          CREATE(sail_int)(&zgsz3411);
          CONVERT_OF(sail_int, mach_int)(&zgsz3411, zcap_mantissa_width);
          sail_int zgsz3412;
          CREATE(sail_int)(&zgsz3412);
          add_int(&zgsz3412, zgsz3410, zgsz3411);
          zgaz3235 = CONVERT_OF(mach_int, sail_int)(zgsz3412);
          KILL(sail_int)(&zgsz3412);
          KILL(sail_int)(&zgsz3411);
          KILL(sail_int)(&zgsz3410);
        }
        {
          sail_int zgsz3407;
          CREATE(sail_int)(&zgsz3407);
          CONVERT_OF(sail_int, mach_int)(&zgsz3407, zgaz3235);
          sail_int zgsz3408;
          CREATE(sail_int)(&zgsz3408);
          CONVERT_OF(sail_int, mach_int)(&zgsz3408, INT64_C(3));
          sail_int zgsz3409;
          CREATE(sail_int)(&zgsz3409);
          sub_int(&zgsz3409, zgsz3407, zgsz3408);
          zgaz3236 = CONVERT_OF(mach_int, sail_int)(zgsz3409);
          KILL(sail_int)(&zgsz3409);
          KILL(sail_int)(&zgsz3408);
          KILL(sail_int)(&zgsz3407);
        }
      }
      {
        lbits zgsz3413;
        CREATE(lbits)(&zgsz3413);
        CONVERT_OF(lbits, fbits)(&zgsz3413, za, UINT64_C(64) , true);
        sail_int zgsz3414;
        CREATE(sail_int)(&zgsz3414);
        CONVERT_OF(sail_int, mach_int)(&zgsz3414, zgaz3236);
        lbits zgsz3415;
        CREATE(lbits)(&zgsz3415);
        shiftr(&zgsz3415, zgsz3413, zgsz3414);
        zgaz3237 = CONVERT_OF(fbits, lbits)(zgsz3415, true);
        KILL(lbits)(&zgsz3415);
        KILL(sail_int)(&zgsz3414);
        KILL(lbits)(&zgsz3413);
      }
    }
    {
      lbits zgsz3416;
      CREATE(lbits)(&zgsz3416);
      CONVERT_OF(lbits, fbits)(&zgsz3416, zgaz3237, UINT64_C(64) , true);
      sail_int zgsz3417;
      CREATE(sail_int)(&zgsz3417);
      CONVERT_OF(sail_int, mach_int)(&zgsz3417, INT64_C(3));
      lbits zgsz3418;
      CREATE(lbits)(&zgsz3418);
      sail_truncate(&zgsz3418, zgsz3416, zgsz3417);
      za3 = CONVERT_OF(fbits, lbits)(zgsz3418, true);
      KILL(lbits)(&zgsz3418);
      KILL(sail_int)(&zgsz3417);
      KILL(lbits)(&zgsz3416);
    }
  }
  uint64_t zB3;
  {
    uint64_t zgaz3234;
    zgaz3234 = zc.zB;
    {
      lbits zgsz3419;
      CREATE(lbits)(&zgsz3419);
      CONVERT_OF(lbits, fbits)(&zgsz3419, zgaz3234, UINT64_C(14) , true);
      sail_int zgsz3420;
      CREATE(sail_int)(&zgsz3420);
      CONVERT_OF(sail_int, mach_int)(&zgsz3420, INT64_C(3));
      lbits zgsz3421;
      CREATE(lbits)(&zgsz3421);
      sail_truncateLSB(&zgsz3421, zgsz3419, zgsz3420);
      zB3 = CONVERT_OF(fbits, lbits)(zgsz3421, true);
      KILL(lbits)(&zgsz3421);
      KILL(sail_int)(&zgsz3420);
      KILL(lbits)(&zgsz3419);
    }
  }
  uint64_t zT3;
  {
    uint64_t zgaz3233;
    zgaz3233 = zc.zT;
    {
      lbits zgsz3422;
      CREATE(lbits)(&zgsz3422);
      CONVERT_OF(lbits, fbits)(&zgsz3422, zgaz3233, UINT64_C(14) , true);
      sail_int zgsz3423;
      CREATE(sail_int)(&zgsz3423);
      CONVERT_OF(sail_int, mach_int)(&zgsz3423, INT64_C(3));
      lbits zgsz3424;
      CREATE(lbits)(&zgsz3424);
      sail_truncateLSB(&zgsz3424, zgsz3422, zgsz3423);
      zT3 = CONVERT_OF(fbits, lbits)(zgsz3424, true);
      KILL(lbits)(&zgsz3424);
      KILL(sail_int)(&zgsz3423);
      KILL(lbits)(&zgsz3422);
    }
  }
  uint64_t zR3;
  {
    lbits zgsz3494;
    CREATE(lbits)(&zgsz3494);
    CONVERT_OF(lbits, fbits)(&zgsz3494, zB3, UINT64_C(3) , true);
    lbits zgsz3495;
    CREATE(lbits)(&zgsz3495);
    CONVERT_OF(lbits, fbits)(&zgsz3495, UINT64_C(0b001), UINT64_C(3) , true);
    lbits zgsz3496;
    CREATE(lbits)(&zgsz3496);
    sub_bits(&zgsz3496, zgsz3494, zgsz3495);
    zR3 = CONVERT_OF(fbits, lbits)(zgsz3496, true);
    KILL(lbits)(&zgsz3496);
    KILL(lbits)(&zgsz3495);
    KILL(lbits)(&zgsz3494);
  }
  int64_t zaHi;
  {
    bool zgaz3232;
    {
      lbits zgsz3425;
      CREATE(lbits)(&zgsz3425);
      CONVERT_OF(lbits, fbits)(&zgsz3425, za3, UINT64_C(3) , true);
      lbits zgsz3426;
      CREATE(lbits)(&zgsz3426);
      CONVERT_OF(lbits, fbits)(&zgsz3426, zR3, UINT64_C(3) , true);
      zgaz3232 = sailgen_z8operatorz0zI_uz9(zgsz3425, zgsz3426);
      KILL(lbits)(&zgsz3426);
      KILL(lbits)(&zgsz3425);
    }
    if (zgaz3232) {  zaHi = INT64_C(1);  } else {  zaHi = INT64_C(0);  }
  }
  int64_t zbHi;
  {
    bool zgaz3231;
    {
      lbits zgsz3427;
      CREATE(lbits)(&zgsz3427);
      CONVERT_OF(lbits, fbits)(&zgsz3427, zB3, UINT64_C(3) , true);
      lbits zgsz3428;
      CREATE(lbits)(&zgsz3428);
      CONVERT_OF(lbits, fbits)(&zgsz3428, zR3, UINT64_C(3) , true);
      zgaz3231 = sailgen_z8operatorz0zI_uz9(zgsz3427, zgsz3428);
      KILL(lbits)(&zgsz3428);
      KILL(lbits)(&zgsz3427);
    }
    if (zgaz3231) {  zbHi = INT64_C(1);  } else {  zbHi = INT64_C(0);  }
  }
  int64_t ztHi;
  {
    bool zgaz3230;
    {
      lbits zgsz3429;
      CREATE(lbits)(&zgsz3429);
      CONVERT_OF(lbits, fbits)(&zgsz3429, zT3, UINT64_C(3) , true);
      lbits zgsz3430;
      CREATE(lbits)(&zgsz3430);
      CONVERT_OF(lbits, fbits)(&zgsz3430, zR3, UINT64_C(3) , true);
      zgaz3230 = sailgen_z8operatorz0zI_uz9(zgsz3429, zgsz3430);
      KILL(lbits)(&zgsz3430);
      KILL(lbits)(&zgsz3429);
    }
    if (zgaz3230) {  ztHi = INT64_C(1);  } else {  ztHi = INT64_C(0);  }
  }
  int64_t zcorrection_base;
  {
    sail_int zgsz3491;
    CREATE(sail_int)(&zgsz3491);
    CONVERT_OF(sail_int, mach_int)(&zgsz3491, zbHi);
    sail_int zgsz3492;
    CREATE(sail_int)(&zgsz3492);
    CONVERT_OF(sail_int, mach_int)(&zgsz3492, zaHi);
    sail_int zgsz3493;
    CREATE(sail_int)(&zgsz3493);
    sub_int(&zgsz3493, zgsz3491, zgsz3492);
    zcorrection_base = CONVERT_OF(mach_int, sail_int)(zgsz3493);
    KILL(sail_int)(&zgsz3493);
    KILL(sail_int)(&zgsz3492);
    KILL(sail_int)(&zgsz3491);
  }
  int64_t zcorrection_top;
  {
    sail_int zgsz3488;
    CREATE(sail_int)(&zgsz3488);
    CONVERT_OF(sail_int, mach_int)(&zgsz3488, ztHi);
    sail_int zgsz3489;
    CREATE(sail_int)(&zgsz3489);
    CONVERT_OF(sail_int, mach_int)(&zgsz3489, zaHi);
    sail_int zgsz3490;
    CREATE(sail_int)(&zgsz3490);
    sub_int(&zgsz3490, zgsz3488, zgsz3489);
    zcorrection_top = CONVERT_OF(mach_int, sail_int)(zgsz3490);
    KILL(sail_int)(&zgsz3490);
    KILL(sail_int)(&zgsz3489);
    KILL(sail_int)(&zgsz3488);
  }
  uint64_t za_top;
  {
    int64_t zgaz3229;
    {
      sail_int zgsz3434;
      CREATE(sail_int)(&zgsz3434);
      CONVERT_OF(sail_int, mach_int)(&zgsz3434, zE);
      sail_int zgsz3435;
      CREATE(sail_int)(&zgsz3435);
      CONVERT_OF(sail_int, mach_int)(&zgsz3435, zcap_mantissa_width);
      sail_int zgsz3436;
      CREATE(sail_int)(&zgsz3436);
      add_int(&zgsz3436, zgsz3434, zgsz3435);
      zgaz3229 = CONVERT_OF(mach_int, sail_int)(zgsz3436);
      KILL(sail_int)(&zgsz3436);
      KILL(sail_int)(&zgsz3435);
      KILL(sail_int)(&zgsz3434);
    }
    {
      lbits zgsz3431;
      CREATE(lbits)(&zgsz3431);
      CONVERT_OF(lbits, fbits)(&zgsz3431, za, UINT64_C(64) , true);
      sail_int zgsz3432;
      CREATE(sail_int)(&zgsz3432);
      CONVERT_OF(sail_int, mach_int)(&zgsz3432, zgaz3229);
      lbits zgsz3433;
      CREATE(lbits)(&zgsz3433);
      shiftr(&zgsz3433, zgsz3431, zgsz3432);
      za_top = CONVERT_OF(fbits, lbits)(zgsz3433, true);
      KILL(lbits)(&zgsz3433);
      KILL(sail_int)(&zgsz3432);
      KILL(lbits)(&zgsz3431);
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
        lbits zgsz3442;
        CREATE(lbits)(&zgsz3442);
        CONVERT_OF(lbits, fbits)(&zgsz3442, za_top, UINT64_C(64) , true);
        sail_int zgsz3443;
        CREATE(sail_int)(&zgsz3443);
        CONVERT_OF(sail_int, mach_int)(&zgsz3443, zcorrection_base);
        lbits zgsz3444;
        CREATE(lbits)(&zgsz3444);
        add_bits_int(&zgsz3444, zgsz3442, zgsz3443);
        zgaz3226 = CONVERT_OF(fbits, lbits)(zgsz3444, true);
        KILL(lbits)(&zgsz3444);
        KILL(sail_int)(&zgsz3443);
        KILL(lbits)(&zgsz3442);
      }
      lbits zgaz3227;
      CREATE(lbits)(&zgaz3227);
      {
        uint64_t zgaz3224;
        zgaz3224 = zc.zB;
        sbits zgaz3225;
        {
          sail_int zgsz3439;
          CREATE(sail_int)(&zgsz3439);
          CONVERT_OF(sail_int, mach_int)(&zgsz3439, zE);
          lbits zgsz3440;
          CREATE(lbits)(&zgsz3440);
          sailgen_zzeros_implicit(&zgsz3440, zgsz3439);
          zgaz3225 = CONVERT_OF(sbits, lbits)(zgsz3440, true);
          KILL(lbits)(&zgsz3440);
          KILL(sail_int)(&zgsz3439);
        }
        {
          lbits zgsz3437;
          CREATE(lbits)(&zgsz3437);
          CONVERT_OF(lbits, fbits)(&zgsz3437, zgaz3224, UINT64_C(14) , true);
          lbits zgsz3438;
          CREATE(lbits)(&zgsz3438);
          CONVERT_OF(lbits, sbits)(&zgsz3438, zgaz3225, true);
          append(&zgaz3227, zgsz3437, zgsz3438);
          KILL(lbits)(&zgsz3438);
          KILL(lbits)(&zgsz3437);
        }
      }
      {
        lbits zgsz3441;
        CREATE(lbits)(&zgsz3441);
        CONVERT_OF(lbits, fbits)(&zgsz3441, zgaz3226, UINT64_C(64) , true);
        append(&zgaz3228, zgsz3441, zgaz3227);
        KILL(lbits)(&zgsz3441);
      }
      KILL(lbits)(&zgaz3227);
    }
    {
      sail_int zgsz3445;
      CREATE(sail_int)(&zgsz3445);
      CONVERT_OF(sail_int, mach_int)(&zgsz3445, zcap_len_width);
      sail_truncate(&zbase, zgaz3228, zgsz3445);
      KILL(sail_int)(&zgsz3445);
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
        lbits zgsz3451;
        CREATE(lbits)(&zgsz3451);
        CONVERT_OF(lbits, fbits)(&zgsz3451, za_top, UINT64_C(64) , true);
        sail_int zgsz3452;
        CREATE(sail_int)(&zgsz3452);
        CONVERT_OF(sail_int, mach_int)(&zgsz3452, zcorrection_top);
        lbits zgsz3453;
        CREATE(lbits)(&zgsz3453);
        add_bits_int(&zgsz3453, zgsz3451, zgsz3452);
        zgaz3221 = CONVERT_OF(fbits, lbits)(zgsz3453, true);
        KILL(lbits)(&zgsz3453);
        KILL(sail_int)(&zgsz3452);
        KILL(lbits)(&zgsz3451);
      }
      lbits zgaz3222;
      CREATE(lbits)(&zgaz3222);
      {
        uint64_t zgaz3219;
        zgaz3219 = zc.zT;
        sbits zgaz3220;
        {
          sail_int zgsz3448;
          CREATE(sail_int)(&zgsz3448);
          CONVERT_OF(sail_int, mach_int)(&zgsz3448, zE);
          lbits zgsz3449;
          CREATE(lbits)(&zgsz3449);
          sailgen_zzeros_implicit(&zgsz3449, zgsz3448);
          zgaz3220 = CONVERT_OF(sbits, lbits)(zgsz3449, true);
          KILL(lbits)(&zgsz3449);
          KILL(sail_int)(&zgsz3448);
        }
        {
          lbits zgsz3446;
          CREATE(lbits)(&zgsz3446);
          CONVERT_OF(lbits, fbits)(&zgsz3446, zgaz3219, UINT64_C(14) , true);
          lbits zgsz3447;
          CREATE(lbits)(&zgsz3447);
          CONVERT_OF(lbits, sbits)(&zgsz3447, zgaz3220, true);
          append(&zgaz3222, zgsz3446, zgsz3447);
          KILL(lbits)(&zgsz3447);
          KILL(lbits)(&zgsz3446);
        }
      }
      {
        lbits zgsz3450;
        CREATE(lbits)(&zgsz3450);
        CONVERT_OF(lbits, fbits)(&zgsz3450, zgaz3221, UINT64_C(64) , true);
        append(&zgaz3223, zgsz3450, zgaz3222);
        KILL(lbits)(&zgsz3450);
      }
      KILL(lbits)(&zgaz3222);
    }
    {
      sail_int zgsz3454;
      CREATE(sail_int)(&zgsz3454);
      CONVERT_OF(sail_int, mach_int)(&zgsz3454, zcap_len_width);
      sail_truncate(&ztop, zgaz3223, zgsz3454);
      KILL(sail_int)(&zgsz3454);
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
          sail_int zgsz3456;
          CREATE(sail_int)(&zgsz3456);
          CONVERT_OF(sail_int, mach_int)(&zgsz3456, zcap_addr_width);
          sail_int zgsz3457;
          CREATE(sail_int)(&zgsz3457);
          CONVERT_OF(sail_int, mach_int)(&zgsz3457, INT64_C(1));
          sail_int zgsz3458;
          CREATE(sail_int)(&zgsz3458);
          sub_int(&zgsz3458, zgsz3456, zgsz3457);
          zgaz3216 = CONVERT_OF(mach_int, sail_int)(zgsz3458);
          KILL(sail_int)(&zgsz3458);
          KILL(sail_int)(&zgsz3457);
          KILL(sail_int)(&zgsz3456);
        }
        {
          sail_int zgsz3455;
          CREATE(sail_int)(&zgsz3455);
          CONVERT_OF(sail_int, mach_int)(&zgsz3455, zgaz3216);
          zgaz3217 = bitvector_access(zbase, zgsz3455);
          KILL(sail_int)(&zgsz3455);
        }
      }
      uint64_t zgsz347;
      zgsz347 = UINT64_C(0b0);
      zgsz347 = update_fbits(zgsz347, INT64_C(0), zgaz3217);
      zgaz3218 = zgsz347;
    }
    {
      lbits zgsz3459;
      CREATE(lbits)(&zgsz3459);
      CONVERT_OF(lbits, fbits)(&zgsz3459, UINT64_C(0b0), UINT64_C(1) , true);
      lbits zgsz3460;
      CREATE(lbits)(&zgsz3460);
      CONVERT_OF(lbits, fbits)(&zgsz3460, zgaz3218, UINT64_C(1) , true);
      lbits zgsz3461;
      CREATE(lbits)(&zgsz3461);
      append(&zgsz3461, zgsz3459, zgsz3460);
      zbase2 = CONVERT_OF(fbits, lbits)(zgsz3461, true);
      KILL(lbits)(&zgsz3461);
      KILL(lbits)(&zgsz3460);
      KILL(lbits)(&zgsz3459);
    }
  }
  uint64_t ztop2;
  {
    int64_t zgaz3215;
    {
      sail_int zgsz3465;
      CREATE(sail_int)(&zgsz3465);
      CONVERT_OF(sail_int, mach_int)(&zgsz3465, zcap_addr_width);
      sail_int zgsz3466;
      CREATE(sail_int)(&zgsz3466);
      CONVERT_OF(sail_int, mach_int)(&zgsz3466, INT64_C(1));
      sail_int zgsz3467;
      CREATE(sail_int)(&zgsz3467);
      sub_int(&zgsz3467, zgsz3465, zgsz3466);
      zgaz3215 = CONVERT_OF(mach_int, sail_int)(zgsz3467);
      KILL(sail_int)(&zgsz3467);
      KILL(sail_int)(&zgsz3466);
      KILL(sail_int)(&zgsz3465);
    }
    {
      sail_int zgsz3462;
      CREATE(sail_int)(&zgsz3462);
      CONVERT_OF(sail_int, mach_int)(&zgsz3462, zcap_addr_width);
      sail_int zgsz3463;
      CREATE(sail_int)(&zgsz3463);
      CONVERT_OF(sail_int, mach_int)(&zgsz3463, zgaz3215);
      lbits zgsz3464;
      CREATE(lbits)(&zgsz3464);
      vector_subrange_lbits(&zgsz3464, ztop, zgsz3462, zgsz3463);
      ztop2 = CONVERT_OF(fbits, lbits)(zgsz3464, true);
      KILL(lbits)(&zgsz3464);
      KILL(sail_int)(&zgsz3463);
      KILL(sail_int)(&zgsz3462);
    }
  }
  {
    bool zgaz3210;
    {
      bool zgaz3209;
      {
        int64_t zgaz3206;
        {
          sail_int zgsz3470;
          CREATE(sail_int)(&zgsz3470);
          CONVERT_OF(sail_int, mach_int)(&zgsz3470, zcap_max_E);
          sail_int zgsz3471;
          CREATE(sail_int)(&zgsz3471);
          CONVERT_OF(sail_int, mach_int)(&zgsz3471, INT64_C(1));
          sail_int zgsz3472;
          CREATE(sail_int)(&zgsz3472);
          sub_int(&zgsz3472, zgsz3470, zgsz3471);
          zgaz3206 = CONVERT_OF(mach_int, sail_int)(zgsz3472);
          KILL(sail_int)(&zgsz3472);
          KILL(sail_int)(&zgsz3471);
          KILL(sail_int)(&zgsz3470);
        }
        {
          sail_int zgsz3468;
          CREATE(sail_int)(&zgsz3468);
          CONVERT_OF(sail_int, mach_int)(&zgsz3468, zE);
          sail_int zgsz3469;
          CREATE(sail_int)(&zgsz3469);
          CONVERT_OF(sail_int, mach_int)(&zgsz3469, zgaz3206);
          zgaz3209 = lt(zgsz3468, zgsz3469);
          KILL(sail_int)(&zgsz3469);
          KILL(sail_int)(&zgsz3468);
        }
      }
      bool zgsz348;
      if (zgaz3209) {
        int64_t zgaz3208;
        {
          uint64_t zgaz3207;
          {
            lbits zgsz3475;
            CREATE(lbits)(&zgsz3475);
            CONVERT_OF(lbits, fbits)(&zgsz3475, ztop2, UINT64_C(2) , true);
            lbits zgsz3476;
            CREATE(lbits)(&zgsz3476);
            CONVERT_OF(lbits, fbits)(&zgsz3476, zbase2, UINT64_C(2) , true);
            lbits zgsz3477;
            CREATE(lbits)(&zgsz3477);
            sub_bits(&zgsz3477, zgsz3475, zgsz3476);
            zgaz3207 = CONVERT_OF(fbits, lbits)(zgsz3477, true);
            KILL(lbits)(&zgsz3477);
            KILL(lbits)(&zgsz3476);
            KILL(lbits)(&zgsz3475);
          }
          {
            lbits zgsz3473;
            CREATE(lbits)(&zgsz3473);
            CONVERT_OF(lbits, fbits)(&zgsz3473, zgaz3207, UINT64_C(2) , true);
            sail_int zgsz3474;
            CREATE(sail_int)(&zgsz3474);
            sail_unsigned(&zgsz3474, zgsz3473);
            zgaz3208 = CONVERT_OF(mach_int, sail_int)(zgsz3474);
            KILL(sail_int)(&zgsz3474);
            KILL(lbits)(&zgsz3473);
          }
        }
        {
          sail_int zgsz3478;
          CREATE(sail_int)(&zgsz3478);
          CONVERT_OF(sail_int, mach_int)(&zgsz3478, zgaz3208);
          sail_int zgsz3479;
          CREATE(sail_int)(&zgsz3479);
          CONVERT_OF(sail_int, mach_int)(&zgsz3479, INT64_C(1));
          zgsz348 = gt(zgsz3478, zgsz3479);
          KILL(sail_int)(&zgsz3479);
          KILL(sail_int)(&zgsz3478);
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
          sail_int zgsz3480;
          CREATE(sail_int)(&zgsz3480);
          CONVERT_OF(sail_int, mach_int)(&zgsz3480, zcap_addr_width);
          zgaz3211 = bitvector_access(ztop, zgsz3480);
          KILL(sail_int)(&zgsz3480);
        }
        zgaz3212 = sailgen_not_bit(zgaz3211);
      }
      {
        sail_int zgsz3481;
        CREATE(sail_int)(&zgsz3481);
        CONVERT_OF(sail_int, mach_int)(&zgsz3481, zcap_addr_width);
        update_lbits(&ztop, ztop, zgsz3481, zgaz3212);
        KILL(sail_int)(&zgsz3481);
      }
      zgsz349 = UNIT;
    } else {  zgsz349 = UNIT;  }
  }
  uint64_t zgaz3214;
  {
    int64_t zgaz3213;
    {
      sail_int zgsz3485;
      CREATE(sail_int)(&zgsz3485);
      CONVERT_OF(sail_int, mach_int)(&zgsz3485, zcap_addr_width);
      sail_int zgsz3486;
      CREATE(sail_int)(&zgsz3486);
      CONVERT_OF(sail_int, mach_int)(&zgsz3486, INT64_C(1));
      sail_int zgsz3487;
      CREATE(sail_int)(&zgsz3487);
      sub_int(&zgsz3487, zgsz3485, zgsz3486);
      zgaz3213 = CONVERT_OF(mach_int, sail_int)(zgsz3487);
      KILL(sail_int)(&zgsz3487);
      KILL(sail_int)(&zgsz3486);
      KILL(sail_int)(&zgsz3485);
    }
    {
      sail_int zgsz3482;
      CREATE(sail_int)(&zgsz3482);
      CONVERT_OF(sail_int, mach_int)(&zgsz3482, zgaz3213);
      sail_int zgsz3483;
      CREATE(sail_int)(&zgsz3483);
      CONVERT_OF(sail_int, mach_int)(&zgsz3483, INT64_C(0));
      lbits zgsz3484;
      CREATE(lbits)(&zgsz3484);
      vector_subrange_lbits(&zgsz3484, zbase, zgsz3482, zgsz3483);
      zgaz3214 = CONVERT_OF(fbits, lbits)(zgsz3484, true);
      KILL(lbits)(&zgsz3484);
      KILL(sail_int)(&zgsz3483);
      KILL(sail_int)(&zgsz3482);
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
  goto end_function_185;
end_block_exception_109: ;
  goto end_function_185;
end_function_185: ;
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
  __label__ case_112, finish_match_111, end_function_113, end_block_exception_114, end_function_184;

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
      lbits zgsz3497;
      CREATE(lbits)(&zgsz3497);
      CONVERT_OF(lbits, fbits)(&zgsz3497, zbase, UINT64_C(64) , true);
      sail_unsigned(&zgaz3241, zgsz3497);
      KILL(lbits)(&zgsz3497);
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
  goto end_function_184;
end_block_exception_114: ;
  goto end_function_184;
end_function_184: ;
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
    lbits zgsz3604;
    CREATE(lbits)(&zgsz3604);
    CONVERT_OF(lbits, fbits)(&zgsz3604, UINT64_C(0b0), UINT64_C(1) , true);
    lbits zgsz3605;
    CREATE(lbits)(&zgsz3605);
    CONVERT_OF(lbits, fbits)(&zgsz3605, zbase, UINT64_C(64) , true);
    append(&zext_base, zgsz3604, zgsz3605);
    KILL(lbits)(&zgsz3605);
    KILL(lbits)(&zgsz3604);
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
          sail_int zgsz3501;
          CREATE(sail_int)(&zgsz3501);
          CONVERT_OF(sail_int, mach_int)(&zgsz3501, zcap_mantissa_width);
          sail_int zgsz3502;
          CREATE(sail_int)(&zgsz3502);
          CONVERT_OF(sail_int, mach_int)(&zgsz3502, INT64_C(1));
          sail_int zgsz3503;
          CREATE(sail_int)(&zgsz3503);
          sub_int(&zgsz3503, zgsz3501, zgsz3502);
          zgaz3274 = CONVERT_OF(mach_int, sail_int)(zgsz3503);
          KILL(sail_int)(&zgsz3503);
          KILL(sail_int)(&zgsz3502);
          KILL(sail_int)(&zgsz3501);
        }
        {
          sail_int zgsz3498;
          CREATE(sail_int)(&zgsz3498);
          CONVERT_OF(sail_int, mach_int)(&zgsz3498, zcap_addr_width);
          sail_int zgsz3499;
          CREATE(sail_int)(&zgsz3499);
          CONVERT_OF(sail_int, mach_int)(&zgsz3499, zgaz3274);
          lbits zgsz3500;
          CREATE(lbits)(&zgsz3500);
          vector_subrange_lbits(&zgsz3500, zlength, zgsz3498, zgsz3499);
          zgaz3275 = CONVERT_OF(fbits, lbits)(zgsz3500, true);
          KILL(lbits)(&zgsz3500);
          KILL(sail_int)(&zgsz3499);
          KILL(sail_int)(&zgsz3498);
        }
      }
      {
        lbits zgsz3504;
        CREATE(lbits)(&zgsz3504);
        CONVERT_OF(lbits, fbits)(&zgsz3504, zgaz3275, UINT64_C(52) , true);
        sail_int zgsz3505;
        CREATE(sail_int)(&zgsz3505);
        count_leading_zeros(&zgsz3505, zgsz3504);
        zgaz3276 = CONVERT_OF(mach_int, sail_int)(zgsz3505);
        KILL(sail_int)(&zgsz3505);
        KILL(lbits)(&zgsz3504);
      }
    }
    {
      sail_int zgsz3506;
      CREATE(sail_int)(&zgsz3506);
      CONVERT_OF(sail_int, mach_int)(&zgsz3506, zcap_max_E);
      sail_int zgsz3507;
      CREATE(sail_int)(&zgsz3507);
      CONVERT_OF(sail_int, mach_int)(&zgsz3507, zgaz3276);
      sail_int zgsz3508;
      CREATE(sail_int)(&zgsz3508);
      sub_int(&zgsz3508, zgsz3506, zgsz3507);
      ze = CONVERT_OF(mach_int, sail_int)(zgsz3508);
      KILL(sail_int)(&zgsz3508);
      KILL(sail_int)(&zgsz3507);
      KILL(sail_int)(&zgsz3506);
    }
  }
  bool zie;
  {
    bool zgaz3273;
    {
      sail_int zgsz3513;
      CREATE(sail_int)(&zgsz3513);
      CONVERT_OF(sail_int, mach_int)(&zgsz3513, ze);
      sail_int zgsz3514;
      CREATE(sail_int)(&zgsz3514);
      CONVERT_OF(sail_int, mach_int)(&zgsz3514, INT64_C(0));
      zgaz3273 = sailgen_neq_int(zgsz3513, zgsz3514);
      KILL(sail_int)(&zgsz3514);
      KILL(sail_int)(&zgsz3513);
    }
    bool zgsz354;
    if (zgaz3273) {  zgsz354 = true;  } else {
      fbits zgaz3272;
      {
        int64_t zgaz3271;
        {
          sail_int zgsz3510;
          CREATE(sail_int)(&zgsz3510);
          CONVERT_OF(sail_int, mach_int)(&zgsz3510, zcap_mantissa_width);
          sail_int zgsz3511;
          CREATE(sail_int)(&zgsz3511);
          CONVERT_OF(sail_int, mach_int)(&zgsz3511, INT64_C(2));
          sail_int zgsz3512;
          CREATE(sail_int)(&zgsz3512);
          sub_int(&zgsz3512, zgsz3510, zgsz3511);
          zgaz3271 = CONVERT_OF(mach_int, sail_int)(zgsz3512);
          KILL(sail_int)(&zgsz3512);
          KILL(sail_int)(&zgsz3511);
          KILL(sail_int)(&zgsz3510);
        }
        {
          sail_int zgsz3509;
          CREATE(sail_int)(&zgsz3509);
          CONVERT_OF(sail_int, mach_int)(&zgsz3509, zgaz3271);
          zgaz3272 = bitvector_access(zlength, zgsz3509);
          KILL(sail_int)(&zgsz3509);
        }
      }
      zgsz354 = eq_bit(zgaz3272, UINT64_C(1));
    }
    zie = zgsz354;
  }
  uint64_t zBbits;
  {
    lbits zgsz3601;
    CREATE(lbits)(&zgsz3601);
    CONVERT_OF(lbits, fbits)(&zgsz3601, zbase, UINT64_C(64) , true);
    sail_int zgsz3602;
    CREATE(sail_int)(&zgsz3602);
    CONVERT_OF(sail_int, mach_int)(&zgsz3602, zcap_mantissa_width);
    lbits zgsz3603;
    CREATE(lbits)(&zgsz3603);
    sail_truncate(&zgsz3603, zgsz3601, zgsz3602);
    zBbits = CONVERT_OF(fbits, lbits)(zgsz3603, true);
    KILL(lbits)(&zgsz3603);
    KILL(sail_int)(&zgsz3602);
    KILL(lbits)(&zgsz3601);
  }
  uint64_t zTbits;
  {
    sail_int zgsz3599;
    CREATE(sail_int)(&zgsz3599);
    CONVERT_OF(sail_int, mach_int)(&zgsz3599, zcap_mantissa_width);
    lbits zgsz3600;
    CREATE(lbits)(&zgsz3600);
    sail_truncate(&zgsz3600, ztop, zgsz3599);
    zTbits = CONVERT_OF(fbits, lbits)(zgsz3600, true);
    KILL(lbits)(&zgsz3600);
    KILL(sail_int)(&zgsz3599);
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
            sail_int zgsz3518;
            CREATE(sail_int)(&zgsz3518);
            CONVERT_OF(sail_int, mach_int)(&zgsz3518, ze);
            sail_int zgsz3519;
            CREATE(sail_int)(&zgsz3519);
            CONVERT_OF(sail_int, mach_int)(&zgsz3519, INT64_C(3));
            sail_int zgsz3520;
            CREATE(sail_int)(&zgsz3520);
            add_int(&zgsz3520, zgsz3518, zgsz3519);
            zgaz3265 = CONVERT_OF(mach_int, sail_int)(zgsz3520);
            KILL(sail_int)(&zgsz3520);
            KILL(sail_int)(&zgsz3519);
            KILL(sail_int)(&zgsz3518);
          }
          {
            lbits zgsz3515;
            CREATE(lbits)(&zgsz3515);
            CONVERT_OF(lbits, fbits)(&zgsz3515, zbase, UINT64_C(64) , true);
            sail_int zgsz3516;
            CREATE(sail_int)(&zgsz3516);
            CONVERT_OF(sail_int, mach_int)(&zgsz3516, zgaz3265);
            lbits zgsz3517;
            CREATE(lbits)(&zgsz3517);
            shiftr(&zgsz3517, zgsz3515, zgsz3516);
            zgaz3266 = CONVERT_OF(fbits, lbits)(zgsz3517, true);
            KILL(lbits)(&zgsz3517);
            KILL(sail_int)(&zgsz3516);
            KILL(lbits)(&zgsz3515);
          }
        }
        int64_t zgaz3267;
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
          zgaz3267 = CONVERT_OF(mach_int, sail_int)(zgsz3526);
          KILL(sail_int)(&zgsz3526);
          KILL(sail_int)(&zgsz3525);
          KILL(sail_int)(&zgsz3524);
        }
        {
          lbits zgsz3521;
          CREATE(lbits)(&zgsz3521);
          CONVERT_OF(lbits, fbits)(&zgsz3521, zgaz3266, UINT64_C(64) , true);
          sail_int zgsz3522;
          CREATE(sail_int)(&zgsz3522);
          CONVERT_OF(sail_int, mach_int)(&zgsz3522, zgaz3267);
          lbits zgsz3523;
          CREATE(lbits)(&zgsz3523);
          sail_truncate(&zgsz3523, zgsz3521, zgsz3522);
          zB_ie = CONVERT_OF(fbits, lbits)(zgsz3523, true);
          KILL(lbits)(&zgsz3523);
          KILL(sail_int)(&zgsz3522);
          KILL(lbits)(&zgsz3521);
        }
      }
      uint64_t zT_ie;
      {
        lbits zgaz3263;
        CREATE(lbits)(&zgaz3263);
        {
          int64_t zgaz3262;
          {
            sail_int zgsz3528;
            CREATE(sail_int)(&zgsz3528);
            CONVERT_OF(sail_int, mach_int)(&zgsz3528, ze);
            sail_int zgsz3529;
            CREATE(sail_int)(&zgsz3529);
            CONVERT_OF(sail_int, mach_int)(&zgsz3529, INT64_C(3));
            sail_int zgsz3530;
            CREATE(sail_int)(&zgsz3530);
            add_int(&zgsz3530, zgsz3528, zgsz3529);
            zgaz3262 = CONVERT_OF(mach_int, sail_int)(zgsz3530);
            KILL(sail_int)(&zgsz3530);
            KILL(sail_int)(&zgsz3529);
            KILL(sail_int)(&zgsz3528);
          }
          {
            sail_int zgsz3527;
            CREATE(sail_int)(&zgsz3527);
            CONVERT_OF(sail_int, mach_int)(&zgsz3527, zgaz3262);
            shiftr(&zgaz3263, ztop, zgsz3527);
            KILL(sail_int)(&zgsz3527);
          }
        }
        int64_t zgaz3264;
        {
          sail_int zgsz3533;
          CREATE(sail_int)(&zgsz3533);
          CONVERT_OF(sail_int, mach_int)(&zgsz3533, zcap_mantissa_width);
          sail_int zgsz3534;
          CREATE(sail_int)(&zgsz3534);
          CONVERT_OF(sail_int, mach_int)(&zgsz3534, INT64_C(3));
          sail_int zgsz3535;
          CREATE(sail_int)(&zgsz3535);
          sub_int(&zgsz3535, zgsz3533, zgsz3534);
          zgaz3264 = CONVERT_OF(mach_int, sail_int)(zgsz3535);
          KILL(sail_int)(&zgsz3535);
          KILL(sail_int)(&zgsz3534);
          KILL(sail_int)(&zgsz3533);
        }
        {
          sail_int zgsz3531;
          CREATE(sail_int)(&zgsz3531);
          CONVERT_OF(sail_int, mach_int)(&zgsz3531, zgaz3264);
          lbits zgsz3532;
          CREATE(lbits)(&zgsz3532);
          sail_truncate(&zgsz3532, zgaz3263, zgsz3531);
          zT_ie = CONVERT_OF(fbits, lbits)(zgsz3532, true);
          KILL(lbits)(&zgsz3532);
          KILL(sail_int)(&zgsz3531);
        }
        KILL(lbits)(&zgaz3263);
      }
      lbits zmaskLo;
      CREATE(lbits)(&zmaskLo);
      {
        int64_t zgaz3260;
        {
          sail_int zgsz3543;
          CREATE(sail_int)(&zgsz3543);
          CONVERT_OF(sail_int, mach_int)(&zgsz3543, INT64_C(64));
          sail_int zgsz3544;
          CREATE(sail_int)(&zgsz3544);
          CONVERT_OF(sail_int, mach_int)(&zgsz3544, INT64_C(1));
          sail_int zgsz3545;
          CREATE(sail_int)(&zgsz3545);
          add_int(&zgsz3545, zgsz3543, zgsz3544);
          zgaz3260 = CONVERT_OF(mach_int, sail_int)(zgsz3545);
          KILL(sail_int)(&zgsz3545);
          KILL(sail_int)(&zgsz3544);
          KILL(sail_int)(&zgsz3543);
        }
        sbits zgaz3261;
        {
          int64_t zgaz3259;
          {
            sail_int zgsz3538;
            CREATE(sail_int)(&zgsz3538);
            CONVERT_OF(sail_int, mach_int)(&zgsz3538, ze);
            sail_int zgsz3539;
            CREATE(sail_int)(&zgsz3539);
            CONVERT_OF(sail_int, mach_int)(&zgsz3539, INT64_C(3));
            sail_int zgsz3540;
            CREATE(sail_int)(&zgsz3540);
            add_int(&zgsz3540, zgsz3538, zgsz3539);
            zgaz3259 = CONVERT_OF(mach_int, sail_int)(zgsz3540);
            KILL(sail_int)(&zgsz3540);
            KILL(sail_int)(&zgsz3539);
            KILL(sail_int)(&zgsz3538);
          }
          {
            sail_int zgsz3536;
            CREATE(sail_int)(&zgsz3536);
            CONVERT_OF(sail_int, mach_int)(&zgsz3536, zgaz3259);
            lbits zgsz3537;
            CREATE(lbits)(&zgsz3537);
            sailgen_ones(&zgsz3537, zgsz3536);
            zgaz3261 = CONVERT_OF(sbits, lbits)(zgsz3537, true);
            KILL(lbits)(&zgsz3537);
            KILL(sail_int)(&zgsz3536);
          }
        }
        {
          sail_int zgsz3541;
          CREATE(sail_int)(&zgsz3541);
          CONVERT_OF(sail_int, mach_int)(&zgsz3541, zgaz3260);
          lbits zgsz3542;
          CREATE(lbits)(&zgsz3542);
          CONVERT_OF(lbits, sbits)(&zgsz3542, zgaz3261, true);
          sailgen_EXTZ(&zmaskLo, zgsz3541, zgsz3542);
          KILL(lbits)(&zgsz3542);
          KILL(sail_int)(&zgsz3541);
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
          sail_int zgsz3546;
          CREATE(sail_int)(&zgsz3546);
          CONVERT_OF(sail_int, mach_int)(&zgsz3546, INT64_C(0));
          zlostSignificantBase = sailgen_neq_int(zgaz3244, zgsz3546);
          KILL(sail_int)(&zgsz3546);
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
          sail_int zgsz3547;
          CREATE(sail_int)(&zgsz3547);
          CONVERT_OF(sail_int, mach_int)(&zgsz3547, INT64_C(0));
          zlostSignificantTop = sailgen_neq_int(zgaz3246, zgsz3547);
          KILL(sail_int)(&zgsz3547);
        }
        unit zgsz357;
        zgsz357 = UNIT;
        KILL(sail_int)(&zgaz3246);
      }
      {
        unit zgsz356;
        if (zlostSignificantTop) {
          {
            lbits zgsz3548;
            CREATE(lbits)(&zgsz3548);
            CONVERT_OF(lbits, fbits)(&zgsz3548, zT_ie, UINT64_C(11) , true);
            sail_int zgsz3549;
            CREATE(sail_int)(&zgsz3549);
            CONVERT_OF(sail_int, mach_int)(&zgsz3549, INT64_C(1));
            lbits zgsz3550;
            CREATE(lbits)(&zgsz3550);
            add_bits_int(&zgsz3550, zgsz3548, zgsz3549);
            zT_ie = CONVERT_OF(fbits, lbits)(zgsz3550, true);
            KILL(lbits)(&zgsz3550);
            KILL(sail_int)(&zgsz3549);
            KILL(lbits)(&zgsz3548);
          }
          zgsz356 = UNIT;
        } else {  zgsz356 = UNIT;  }
      }
      uint64_t zlen_ie;
      {
        lbits zgsz3590;
        CREATE(lbits)(&zgsz3590);
        CONVERT_OF(lbits, fbits)(&zgsz3590, zT_ie, UINT64_C(11) , true);
        lbits zgsz3591;
        CREATE(lbits)(&zgsz3591);
        CONVERT_OF(lbits, fbits)(&zgsz3591, zB_ie, UINT64_C(11) , true);
        lbits zgsz3592;
        CREATE(lbits)(&zgsz3592);
        sub_bits(&zgsz3592, zgsz3590, zgsz3591);
        zlen_ie = CONVERT_OF(fbits, lbits)(zgsz3592, true);
        KILL(lbits)(&zgsz3592);
        KILL(lbits)(&zgsz3591);
        KILL(lbits)(&zgsz3590);
      }
      {
        bool zgaz3249;
        {
          fbits zgaz3248;
          {
            int64_t zgaz3247;
            {
              sail_int zgsz3553;
              CREATE(sail_int)(&zgsz3553);
              CONVERT_OF(sail_int, mach_int)(&zgsz3553, zcap_mantissa_width);
              sail_int zgsz3554;
              CREATE(sail_int)(&zgsz3554);
              CONVERT_OF(sail_int, mach_int)(&zgsz3554, INT64_C(4));
              sail_int zgsz3555;
              CREATE(sail_int)(&zgsz3555);
              sub_int(&zgsz3555, zgsz3553, zgsz3554);
              zgaz3247 = CONVERT_OF(mach_int, sail_int)(zgsz3555);
              KILL(sail_int)(&zgsz3555);
              KILL(sail_int)(&zgsz3554);
              KILL(sail_int)(&zgsz3553);
            }
            {
              lbits zgsz3551;
              CREATE(lbits)(&zgsz3551);
              CONVERT_OF(lbits, fbits)(&zgsz3551, zlen_ie, UINT64_C(11) , true);
              sail_int zgsz3552;
              CREATE(sail_int)(&zgsz3552);
              CONVERT_OF(sail_int, mach_int)(&zgsz3552, zgaz3247);
              zgaz3248 = bitvector_access(zgsz3551, zgsz3552);
              KILL(sail_int)(&zgsz3552);
              KILL(lbits)(&zgsz3551);
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
                lbits zgsz3556;
                CREATE(lbits)(&zgsz3556);
                CONVERT_OF(lbits, fbits)(&zgsz3556, zB_ie, UINT64_C(11) , true);
                sail_int zgsz3557;
                CREATE(sail_int)(&zgsz3557);
                CONVERT_OF(sail_int, mach_int)(&zgsz3557, INT64_C(0));
                zgaz3250 = bitvector_access(zgsz3556, zgsz3557);
                KILL(sail_int)(&zgsz3557);
                KILL(lbits)(&zgsz3556);
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
                lbits zgsz3558;
                CREATE(lbits)(&zgsz3558);
                CONVERT_OF(lbits, fbits)(&zgsz3558, zT_ie, UINT64_C(11) , true);
                sail_int zgsz3559;
                CREATE(sail_int)(&zgsz3559);
                CONVERT_OF(sail_int, mach_int)(&zgsz3559, INT64_C(0));
                zgaz3251 = bitvector_access(zgsz3558, zgsz3559);
                KILL(sail_int)(&zgsz3559);
                KILL(lbits)(&zgsz3558);
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
                sail_int zgsz3563;
                CREATE(sail_int)(&zgsz3563);
                CONVERT_OF(sail_int, mach_int)(&zgsz3563, ze);
                sail_int zgsz3564;
                CREATE(sail_int)(&zgsz3564);
                CONVERT_OF(sail_int, mach_int)(&zgsz3564, INT64_C(4));
                sail_int zgsz3565;
                CREATE(sail_int)(&zgsz3565);
                add_int(&zgsz3565, zgsz3563, zgsz3564);
                zgaz3252 = CONVERT_OF(mach_int, sail_int)(zgsz3565);
                KILL(sail_int)(&zgsz3565);
                KILL(sail_int)(&zgsz3564);
                KILL(sail_int)(&zgsz3563);
              }
              {
                lbits zgsz3560;
                CREATE(lbits)(&zgsz3560);
                CONVERT_OF(lbits, fbits)(&zgsz3560, zbase, UINT64_C(64) , true);
                sail_int zgsz3561;
                CREATE(sail_int)(&zgsz3561);
                CONVERT_OF(sail_int, mach_int)(&zgsz3561, zgaz3252);
                lbits zgsz3562;
                CREATE(lbits)(&zgsz3562);
                shiftr(&zgsz3562, zgsz3560, zgsz3561);
                zgaz3253 = CONVERT_OF(fbits, lbits)(zgsz3562, true);
                KILL(lbits)(&zgsz3562);
                KILL(sail_int)(&zgsz3561);
                KILL(lbits)(&zgsz3560);
              }
            }
            int64_t zgaz3254;
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
              zgaz3254 = CONVERT_OF(mach_int, sail_int)(zgsz3571);
              KILL(sail_int)(&zgsz3571);
              KILL(sail_int)(&zgsz3570);
              KILL(sail_int)(&zgsz3569);
            }
            {
              lbits zgsz3566;
              CREATE(lbits)(&zgsz3566);
              CONVERT_OF(lbits, fbits)(&zgsz3566, zgaz3253, UINT64_C(64) , true);
              sail_int zgsz3567;
              CREATE(sail_int)(&zgsz3567);
              CONVERT_OF(sail_int, mach_int)(&zgsz3567, zgaz3254);
              lbits zgsz3568;
              CREATE(lbits)(&zgsz3568);
              sail_truncate(&zgsz3568, zgsz3566, zgsz3567);
              zB_ie = CONVERT_OF(fbits, lbits)(zgsz3568, true);
              KILL(lbits)(&zgsz3568);
              KILL(sail_int)(&zgsz3567);
              KILL(lbits)(&zgsz3566);
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
                sail_int zgsz3573;
                CREATE(sail_int)(&zgsz3573);
                CONVERT_OF(sail_int, mach_int)(&zgsz3573, ze);
                sail_int zgsz3574;
                CREATE(sail_int)(&zgsz3574);
                CONVERT_OF(sail_int, mach_int)(&zgsz3574, INT64_C(4));
                sail_int zgsz3575;
                CREATE(sail_int)(&zgsz3575);
                add_int(&zgsz3575, zgsz3573, zgsz3574);
                zgaz3255 = CONVERT_OF(mach_int, sail_int)(zgsz3575);
                KILL(sail_int)(&zgsz3575);
                KILL(sail_int)(&zgsz3574);
                KILL(sail_int)(&zgsz3573);
              }
              {
                sail_int zgsz3572;
                CREATE(sail_int)(&zgsz3572);
                CONVERT_OF(sail_int, mach_int)(&zgsz3572, zgaz3255);
                shiftr(&zgaz3256, ztop, zgsz3572);
                KILL(sail_int)(&zgsz3572);
              }
            }
            int64_t zgaz3257;
            {
              sail_int zgsz3578;
              CREATE(sail_int)(&zgsz3578);
              CONVERT_OF(sail_int, mach_int)(&zgsz3578, zcap_mantissa_width);
              sail_int zgsz3579;
              CREATE(sail_int)(&zgsz3579);
              CONVERT_OF(sail_int, mach_int)(&zgsz3579, INT64_C(3));
              sail_int zgsz3580;
              CREATE(sail_int)(&zgsz3580);
              sub_int(&zgsz3580, zgsz3578, zgsz3579);
              zgaz3257 = CONVERT_OF(mach_int, sail_int)(zgsz3580);
              KILL(sail_int)(&zgsz3580);
              KILL(sail_int)(&zgsz3579);
              KILL(sail_int)(&zgsz3578);
            }
            {
              sail_int zgsz3576;
              CREATE(sail_int)(&zgsz3576);
              CONVERT_OF(sail_int, mach_int)(&zgsz3576, zgaz3257);
              lbits zgsz3577;
              CREATE(lbits)(&zgsz3577);
              sail_truncate(&zgsz3577, zgaz3256, zgsz3576);
              zgaz3258 = CONVERT_OF(fbits, lbits)(zgsz3577, true);
              KILL(lbits)(&zgsz3577);
              KILL(sail_int)(&zgsz3576);
            }
            KILL(lbits)(&zgaz3256);
          }
          {
            lbits zgsz3581;
            CREATE(lbits)(&zgsz3581);
            CONVERT_OF(lbits, fbits)(&zgsz3581, zgaz3258, UINT64_C(11) , true);
            sail_int zgsz3582;
            CREATE(sail_int)(&zgsz3582);
            CONVERT_OF(sail_int, mach_int)(&zgsz3582, zincT);
            lbits zgsz3583;
            CREATE(lbits)(&zgsz3583);
            add_bits_int(&zgsz3583, zgsz3581, zgsz3582);
            zT_ie = CONVERT_OF(fbits, lbits)(zgsz3583, true);
            KILL(lbits)(&zgsz3583);
            KILL(sail_int)(&zgsz3582);
            KILL(lbits)(&zgsz3581);
          }
          zgsz360 = UNIT;
        } else {  zgsz360 = UNIT;  }
      }
      {
        {
          lbits zgsz3584;
          CREATE(lbits)(&zgsz3584);
          CONVERT_OF(lbits, fbits)(&zgsz3584, zB_ie, UINT64_C(11) , true);
          lbits zgsz3585;
          CREATE(lbits)(&zgsz3585);
          CONVERT_OF(lbits, fbits)(&zgsz3585, UINT64_C(0b000), UINT64_C(3) , true);
          lbits zgsz3586;
          CREATE(lbits)(&zgsz3586);
          append(&zgsz3586, zgsz3584, zgsz3585);
          zBbits = CONVERT_OF(fbits, lbits)(zgsz3586, true);
          KILL(lbits)(&zgsz3586);
          KILL(lbits)(&zgsz3585);
          KILL(lbits)(&zgsz3584);
        }
        unit zgsz359;
        zgsz359 = UNIT;
      }
      {
        lbits zgsz3587;
        CREATE(lbits)(&zgsz3587);
        CONVERT_OF(lbits, fbits)(&zgsz3587, zT_ie, UINT64_C(11) , true);
        lbits zgsz3588;
        CREATE(lbits)(&zgsz3588);
        CONVERT_OF(lbits, fbits)(&zgsz3588, UINT64_C(0b000), UINT64_C(3) , true);
        lbits zgsz3589;
        CREATE(lbits)(&zgsz3589);
        append(&zgsz3589, zgsz3587, zgsz3588);
        zTbits = CONVERT_OF(fbits, lbits)(zgsz3589, true);
        KILL(lbits)(&zgsz3589);
        KILL(lbits)(&zgsz3588);
        KILL(lbits)(&zgsz3587);
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
          sail_int zgsz3593;
          CREATE(sail_int)(&zgsz3593);
          CONVERT_OF(sail_int, mach_int)(&zgsz3593, ze);
          sail_int zgsz3594;
          CREATE(sail_int)(&zgsz3594);
          CONVERT_OF(sail_int, mach_int)(&zgsz3594, INT64_C(1));
          sail_int zgsz3595;
          CREATE(sail_int)(&zgsz3595);
          add_int(&zgsz3595, zgsz3593, zgsz3594);
          zgaz3269 = CONVERT_OF(mach_int, sail_int)(zgsz3595);
          KILL(sail_int)(&zgsz3595);
          KILL(sail_int)(&zgsz3594);
          KILL(sail_int)(&zgsz3593);
        }
      } else {  zgaz3269 = ze;  }
      {
        sail_int zgsz3596;
        CREATE(sail_int)(&zgsz3596);
        CONVERT_OF(sail_int, mach_int)(&zgsz3596, INT64_C(6));
        sail_int zgsz3597;
        CREATE(sail_int)(&zgsz3597);
        CONVERT_OF(sail_int, mach_int)(&zgsz3597, zgaz3269);
        lbits zgsz3598;
        CREATE(lbits)(&zgsz3598);
        sailgen_to_bits(&zgsz3598, zgsz3596, zgsz3597);
        zgaz3270 = CONVERT_OF(fbits, lbits)(zgsz3598, true);
        KILL(lbits)(&zgsz3598);
        KILL(sail_int)(&zgsz3597);
        KILL(sail_int)(&zgsz3596);
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
    uint64_t zgaz3278;
    zgaz3278 = sailgen_getCapHardPerms(zcap);
    {
      sail_int zgsz3606;
      CREATE(sail_int)(&zgsz3606);
      CONVERT_OF(sail_int, mach_int)(&zgsz3606, INT64_C(15));
      lbits zgsz3607;
      CREATE(lbits)(&zgsz3607);
      CONVERT_OF(lbits, fbits)(&zgsz3607, zgaz3278, UINT64_C(12) , true);
      lbits zgsz3608;
      CREATE(lbits)(&zgsz3608);
      sailgen_EXTZ(&zgsz3608, zgsz3606, zgsz3607);
      zperms = CONVERT_OF(fbits, lbits)(zgsz3608, true);
      KILL(lbits)(&zgsz3608);
      KILL(lbits)(&zgsz3607);
      KILL(sail_int)(&zgsz3606);
    }
  }
  uint64_t zgaz3277;
  zgaz3277 = zcap.zuperms;
  {
    lbits zgsz3609;
    CREATE(lbits)(&zgsz3609);
    CONVERT_OF(lbits, fbits)(&zgsz3609, zgaz3277, UINT64_C(4) , true);
    lbits zgsz3610;
    CREATE(lbits)(&zgsz3610);
    CONVERT_OF(lbits, fbits)(&zgsz3610, zperms, UINT64_C(15) , true);
    lbits zgsz3611;
    CREATE(lbits)(&zgsz3611);
    append(&zgsz3611, zgsz3609, zgsz3610);
    zcbz327 = CONVERT_OF(fbits, lbits)(zgsz3611, true);
    KILL(lbits)(&zgsz3611);
    KILL(lbits)(&zgsz3610);
    KILL(lbits)(&zgsz3609);
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
      lbits zgsz3615;
      CREATE(lbits)(&zgsz3615);
      CONVERT_OF(lbits, fbits)(&zgsz3615, zperms, UINT64_C(19) , true);
      sail_int zgsz3616;
      CREATE(sail_int)(&zgsz3616);
      CONVERT_OF(sail_int, mach_int)(&zgsz3616, zcap_uperms_shift);
      lbits zgsz3617;
      CREATE(lbits)(&zgsz3617);
      shiftr(&zgsz3617, zgsz3615, zgsz3616);
      zgaz3279 = CONVERT_OF(fbits, lbits)(zgsz3617, true);
      KILL(lbits)(&zgsz3617);
      KILL(sail_int)(&zgsz3616);
      KILL(lbits)(&zgsz3615);
    }
    {
      lbits zgsz3612;
      CREATE(lbits)(&zgsz3612);
      CONVERT_OF(lbits, fbits)(&zgsz3612, zgaz3279, UINT64_C(19) , true);
      sail_int zgsz3613;
      CREATE(sail_int)(&zgsz3613);
      CONVERT_OF(sail_int, mach_int)(&zgsz3613, zcap_uperms_width);
      lbits zgsz3614;
      CREATE(lbits)(&zgsz3614);
      sail_truncate(&zgsz3614, zgsz3612, zgsz3613);
      zgaz3280 = CONVERT_OF(fbits, lbits)(zgsz3614, true);
      KILL(lbits)(&zgsz3614);
      KILL(sail_int)(&zgsz3613);
      KILL(lbits)(&zgsz3612);
    }
  }
  bool zgaz3282;
  {
    fbits zgaz3281;
    {
      lbits zgsz3618;
      CREATE(lbits)(&zgsz3618);
      CONVERT_OF(lbits, fbits)(&zgsz3618, zperms, UINT64_C(19) , true);
      sail_int zgsz3619;
      CREATE(sail_int)(&zgsz3619);
      CONVERT_OF(sail_int, mach_int)(&zgsz3619, INT64_C(11));
      zgaz3281 = bitvector_access(zgsz3618, zgsz3619);
      KILL(sail_int)(&zgsz3619);
      KILL(lbits)(&zgsz3618);
    }
    zgaz3282 = sailgen_bit_to_bool(zgaz3281);
  }
  bool zgaz3284;
  {
    fbits zgaz3283;
    {
      lbits zgsz3620;
      CREATE(lbits)(&zgsz3620);
      CONVERT_OF(lbits, fbits)(&zgsz3620, zperms, UINT64_C(19) , true);
      sail_int zgsz3621;
      CREATE(sail_int)(&zgsz3621);
      CONVERT_OF(sail_int, mach_int)(&zgsz3621, INT64_C(10));
      zgaz3283 = bitvector_access(zgsz3620, zgsz3621);
      KILL(sail_int)(&zgsz3621);
      KILL(lbits)(&zgsz3620);
    }
    zgaz3284 = sailgen_bit_to_bool(zgaz3283);
  }
  bool zgaz3286;
  {
    fbits zgaz3285;
    {
      lbits zgsz3622;
      CREATE(lbits)(&zgsz3622);
      CONVERT_OF(lbits, fbits)(&zgsz3622, zperms, UINT64_C(19) , true);
      sail_int zgsz3623;
      CREATE(sail_int)(&zgsz3623);
      CONVERT_OF(sail_int, mach_int)(&zgsz3623, INT64_C(9));
      zgaz3285 = bitvector_access(zgsz3622, zgsz3623);
      KILL(sail_int)(&zgsz3623);
      KILL(lbits)(&zgsz3622);
    }
    zgaz3286 = sailgen_bit_to_bool(zgaz3285);
  }
  bool zgaz3288;
  {
    fbits zgaz3287;
    {
      lbits zgsz3624;
      CREATE(lbits)(&zgsz3624);
      CONVERT_OF(lbits, fbits)(&zgsz3624, zperms, UINT64_C(19) , true);
      sail_int zgsz3625;
      CREATE(sail_int)(&zgsz3625);
      CONVERT_OF(sail_int, mach_int)(&zgsz3625, INT64_C(8));
      zgaz3287 = bitvector_access(zgsz3624, zgsz3625);
      KILL(sail_int)(&zgsz3625);
      KILL(lbits)(&zgsz3624);
    }
    zgaz3288 = sailgen_bit_to_bool(zgaz3287);
  }
  bool zgaz3290;
  {
    fbits zgaz3289;
    {
      lbits zgsz3626;
      CREATE(lbits)(&zgsz3626);
      CONVERT_OF(lbits, fbits)(&zgsz3626, zperms, UINT64_C(19) , true);
      sail_int zgsz3627;
      CREATE(sail_int)(&zgsz3627);
      CONVERT_OF(sail_int, mach_int)(&zgsz3627, INT64_C(7));
      zgaz3289 = bitvector_access(zgsz3626, zgsz3627);
      KILL(sail_int)(&zgsz3627);
      KILL(lbits)(&zgsz3626);
    }
    zgaz3290 = sailgen_bit_to_bool(zgaz3289);
  }
  bool zgaz3292;
  {
    fbits zgaz3291;
    {
      lbits zgsz3628;
      CREATE(lbits)(&zgsz3628);
      CONVERT_OF(lbits, fbits)(&zgsz3628, zperms, UINT64_C(19) , true);
      sail_int zgsz3629;
      CREATE(sail_int)(&zgsz3629);
      CONVERT_OF(sail_int, mach_int)(&zgsz3629, INT64_C(6));
      zgaz3291 = bitvector_access(zgsz3628, zgsz3629);
      KILL(sail_int)(&zgsz3629);
      KILL(lbits)(&zgsz3628);
    }
    zgaz3292 = sailgen_bit_to_bool(zgaz3291);
  }
  bool zgaz3294;
  {
    fbits zgaz3293;
    {
      lbits zgsz3630;
      CREATE(lbits)(&zgsz3630);
      CONVERT_OF(lbits, fbits)(&zgsz3630, zperms, UINT64_C(19) , true);
      sail_int zgsz3631;
      CREATE(sail_int)(&zgsz3631);
      CONVERT_OF(sail_int, mach_int)(&zgsz3631, INT64_C(5));
      zgaz3293 = bitvector_access(zgsz3630, zgsz3631);
      KILL(sail_int)(&zgsz3631);
      KILL(lbits)(&zgsz3630);
    }
    zgaz3294 = sailgen_bit_to_bool(zgaz3293);
  }
  bool zgaz3296;
  {
    fbits zgaz3295;
    {
      lbits zgsz3632;
      CREATE(lbits)(&zgsz3632);
      CONVERT_OF(lbits, fbits)(&zgsz3632, zperms, UINT64_C(19) , true);
      sail_int zgsz3633;
      CREATE(sail_int)(&zgsz3633);
      CONVERT_OF(sail_int, mach_int)(&zgsz3633, INT64_C(4));
      zgaz3295 = bitvector_access(zgsz3632, zgsz3633);
      KILL(sail_int)(&zgsz3633);
      KILL(lbits)(&zgsz3632);
    }
    zgaz3296 = sailgen_bit_to_bool(zgaz3295);
  }
  bool zgaz3298;
  {
    fbits zgaz3297;
    {
      lbits zgsz3634;
      CREATE(lbits)(&zgsz3634);
      CONVERT_OF(lbits, fbits)(&zgsz3634, zperms, UINT64_C(19) , true);
      sail_int zgsz3635;
      CREATE(sail_int)(&zgsz3635);
      CONVERT_OF(sail_int, mach_int)(&zgsz3635, INT64_C(3));
      zgaz3297 = bitvector_access(zgsz3634, zgsz3635);
      KILL(sail_int)(&zgsz3635);
      KILL(lbits)(&zgsz3634);
    }
    zgaz3298 = sailgen_bit_to_bool(zgaz3297);
  }
  bool zgaz3300;
  {
    fbits zgaz3299;
    {
      lbits zgsz3636;
      CREATE(lbits)(&zgsz3636);
      CONVERT_OF(lbits, fbits)(&zgsz3636, zperms, UINT64_C(19) , true);
      sail_int zgsz3637;
      CREATE(sail_int)(&zgsz3637);
      CONVERT_OF(sail_int, mach_int)(&zgsz3637, INT64_C(2));
      zgaz3299 = bitvector_access(zgsz3636, zgsz3637);
      KILL(sail_int)(&zgsz3637);
      KILL(lbits)(&zgsz3636);
    }
    zgaz3300 = sailgen_bit_to_bool(zgaz3299);
  }
  bool zgaz3302;
  {
    fbits zgaz3301;
    {
      lbits zgsz3638;
      CREATE(lbits)(&zgsz3638);
      CONVERT_OF(lbits, fbits)(&zgsz3638, zperms, UINT64_C(19) , true);
      sail_int zgsz3639;
      CREATE(sail_int)(&zgsz3639);
      CONVERT_OF(sail_int, mach_int)(&zgsz3639, INT64_C(1));
      zgaz3301 = bitvector_access(zgsz3638, zgsz3639);
      KILL(sail_int)(&zgsz3639);
      KILL(lbits)(&zgsz3638);
    }
    zgaz3302 = sailgen_bit_to_bool(zgaz3301);
  }
  bool zgaz3304;
  {
    fbits zgaz3303;
    {
      lbits zgsz3640;
      CREATE(lbits)(&zgsz3640);
      CONVERT_OF(lbits, fbits)(&zgsz3640, zperms, UINT64_C(19) , true);
      sail_int zgsz3641;
      CREATE(sail_int)(&zgsz3641);
      CONVERT_OF(sail_int, mach_int)(&zgsz3641, INT64_C(0));
      zgaz3303 = bitvector_access(zgsz3640, zgsz3641);
      KILL(sail_int)(&zgsz3641);
      KILL(lbits)(&zgsz3640);
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
  struct zCapability zcbz351 = { .zuperms = UINT64_C(0xdeadc0de), .ztag = false, .zreserved = UINT64_C(0xdeadc0de), .zpermit_unseal = false, .zpermit_store_local_cap = false, .zpermit_store_cap = false, .zpermit_store = false, .zpermit_set_CID = false, .zpermit_seal = false, .zpermit_load_cap = false, .zpermit_load = false, .zpermit_execute = false, .zpermit_cinvoke = false, .zotype = UINT64_C(0xdeadc0de), .zinternal_E = false, .zglobal = false, .zflag_cap_mode = false, .zaddress = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zT = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  return zcbz351;
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
      lbits zgsz3642;
      CREATE(lbits)(&zgsz3642);
      CONVERT_OF(lbits, fbits)(&zgsz3642, zgaz3306, UINT64_C(18) , true);
      sail_int zgsz3643;
      CREATE(sail_int)(&zgsz3643);
      sail_signed(&zgsz3643, zgsz3642);
      zgaz3307 = CONVERT_OF(mach_int, sail_int)(zgsz3643);
      KILL(sail_int)(&zgsz3643);
      KILL(lbits)(&zgsz3642);
    }
  }
  {
    sail_int zgsz3644;
    CREATE(sail_int)(&zgsz3644);
    CONVERT_OF(sail_int, mach_int)(&zgsz3644, zgaz3307);
    sail_int zgsz3645;
    CREATE(sail_int)(&zgsz3645);
    CONVERT_OF(sail_int, mach_int)(&zgsz3645, zotype_unsealed);
    zcbz330 = sailgen_neq_int(zgsz3644, zgsz3645);
    KILL(sail_int)(&zgsz3645);
    KILL(sail_int)(&zgsz3644);
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
      lbits zgsz3646;
      CREATE(lbits)(&zgsz3646);
      CONVERT_OF(lbits, fbits)(&zgsz3646, zgaz3308, UINT64_C(18) , true);
      sail_int zgsz3647;
      CREATE(sail_int)(&zgsz3647);
      sail_unsigned(&zgsz3647, zgsz3646);
      zgaz3309 = CONVERT_OF(mach_int, sail_int)(zgsz3647);
      KILL(sail_int)(&zgsz3647);
      KILL(lbits)(&zgsz3646);
    }
  }
  {
    sail_int zgsz3648;
    CREATE(sail_int)(&zgsz3648);
    CONVERT_OF(sail_int, mach_int)(&zgsz3648, zgaz3309);
    sail_int zgsz3649;
    CREATE(sail_int)(&zgsz3649);
    CONVERT_OF(sail_int, mach_int)(&zgsz3649, zcap_max_otype);
    zcbz331 = gt(zgsz3648, zgsz3649);
    KILL(sail_int)(&zgsz3649);
    KILL(sail_int)(&zgsz3648);
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
    lbits zgsz3650;
    CREATE(lbits)(&zgsz3650);
    CONVERT_OF(lbits, fbits)(&zgsz3650, zgaz3311, UINT64_C(64) , true);
    lbits zgsz3651;
    CREATE(lbits)(&zgsz3651);
    CONVERT_OF(lbits, fbits)(&zgsz3651, zbase, UINT64_C(64) , true);
    lbits zgsz3652;
    CREATE(lbits)(&zgsz3652);
    sub_bits(&zgsz3652, zgsz3650, zgsz3651);
    zcbz333 = CONVERT_OF(fbits, lbits)(zgsz3652, true);
    KILL(lbits)(&zgsz3652);
    KILL(lbits)(&zgsz3651);
    KILL(lbits)(&zgsz3650);
  }


end_function_139: ;
  return zcbz333;
end_block_exception_140: ;

  return UINT64_C(0xdeadc0de);
}

static void sailgen_getCapLength(sail_int *rop, struct zCapability);

static void sailgen_getCapLength(sail_int *zcbz334, struct zCapability zc)
{
  __label__ case_143, finish_match_142, end_function_144, end_block_exception_145, end_function_183;

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
      sail_int zgsz3653;
      CREATE(sail_int)(&zgsz3653);
      CONVERT_OF(sail_int, mach_int)(&zgsz3653, zcap_len_width);
      pow2(&zgaz3317, zgsz3653);
      KILL(sail_int)(&zgsz3653);
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
  goto end_function_183;
end_block_exception_145: ;
  goto end_function_183;
end_function_183: ;
}

static bool sailgen_capBoundsEqual(struct zCapability, struct zCapability);

static bool sailgen_capBoundsEqual(struct zCapability zc1, struct zCapability zc2)
{
  __label__ case_148, finish_match_147, end_function_151, end_block_exception_152;

  bool zcbz335;
  struct ztuple_z8z5izCz0z5iz9 zgaz3318;
  CREATE(ztuple_z8z5izCz0z5iz9)(&zgaz3318);
  sailgen_getCapBounds(&zgaz3318, zc1);
  bool zgsz377;
  {
    __label__ case_150, finish_match_149;

    sail_int zbase1;
    CREATE(sail_int)(&zbase1);
    COPY(sail_int)(&zbase1, zgaz3318.ztup0);
    sail_int ztop1;
    CREATE(sail_int)(&ztop1);
    COPY(sail_int)(&ztop1, zgaz3318.ztup1);
    struct ztuple_z8z5izCz0z5iz9 zgaz3319;
    CREATE(ztuple_z8z5izCz0z5iz9)(&zgaz3319);
    sailgen_getCapBounds(&zgaz3319, zc2);
    bool zgsz378;
    {
      sail_int zbase2;
      CREATE(sail_int)(&zbase2);
      COPY(sail_int)(&zbase2, zgaz3319.ztup0);
      sail_int ztop2;
      CREATE(sail_int)(&ztop2);
      COPY(sail_int)(&ztop2, zgaz3319.ztup1);
      bool zgaz3320;
      zgaz3320 = eq_int(zbase1, zbase2);
      bool zgsz379;
      if (zgaz3320) {  zgsz379 = eq_int(ztop1, ztop2);  } else {  zgsz379 = false;  }
      zgsz378 = zgsz379;
      KILL(sail_int)(&ztop2);
      KILL(sail_int)(&zbase2);
      goto finish_match_149;
    }
  case_150: ;
    sail_match_failure("capBoundsEqual");
  finish_match_149: ;
    zgsz377 = zgsz378;
    KILL(ztuple_z8z5izCz0z5iz9)(&zgaz3319);
    KILL(sail_int)(&ztop1);
    KILL(sail_int)(&zbase1);
    goto finish_match_147;
  }
case_148: ;
  sail_match_failure("capBoundsEqual");
finish_match_147: ;
  zcbz335 = zgsz377;

  KILL(ztuple_z8z5izCz0z5iz9)(&zgaz3318);
end_function_151: ;
  return zcbz335;
end_block_exception_152: ;

  return false;
}

static struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 sailgen_setCapAddr(struct zCapability, uint64_t);

static struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 sailgen_setCapAddr(struct zCapability zc, uint64_t zaddr)
{
  __label__ end_function_154, end_block_exception_155;

  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz336;
  struct zCapability znewCap;
  {
    struct zCapability zgsz382;
    zgsz382 = zc;
    zgsz382.zaddress = zaddr;
    znewCap = zgsz382;
  }
  bool zrepresentable;
  zrepresentable = sailgen_capBoundsEqual(zc, znewCap);
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgsz383;
  zgsz383.ztup0 = zrepresentable;
  zgsz383.ztup1 = znewCap;
  zcbz336 = zgsz383;



end_function_154: ;
  return zcbz336;
end_block_exception_155: ;
  struct zCapability zcbz353 = { .zuperms = UINT64_C(0xdeadc0de), .ztag = false, .zreserved = UINT64_C(0xdeadc0de), .zpermit_unseal = false, .zpermit_store_local_cap = false, .zpermit_store_cap = false, .zpermit_store = false, .zpermit_set_CID = false, .zpermit_seal = false, .zpermit_load_cap = false, .zpermit_load = false, .zpermit_execute = false, .zpermit_cinvoke = false, .zotype = UINT64_C(0xdeadc0de), .zinternal_E = false, .zglobal = false, .zflag_cap_mode = false, .zaddress = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zT = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz352 = { .ztup1 = zcbz353, .ztup0 = false };
  return zcbz352;
}

static bool sailgen_fastRepCheck(struct zCapability, uint64_t);

static bool sailgen_fastRepCheck(struct zCapability zc, uint64_t zi)
{
  __label__ end_function_157, end_block_exception_158;

  bool zcbz337;
  int64_t zE;
  {
    uint64_t zgaz3334;
    zgaz3334 = zc.zE;
    {
      lbits zgsz3654;
      CREATE(lbits)(&zgsz3654);
      CONVERT_OF(lbits, fbits)(&zgsz3654, zgaz3334, UINT64_C(6) , true);
      sail_int zgsz3655;
      CREATE(sail_int)(&zgsz3655);
      sail_unsigned(&zgsz3655, zgsz3654);
      zE = CONVERT_OF(mach_int, sail_int)(zgsz3655);
      KILL(sail_int)(&zgsz3655);
      KILL(lbits)(&zgsz3654);
    }
  }
  bool zgaz3322;
  {
    int64_t zgaz3321;
    {
      sail_int zgsz3658;
      CREATE(sail_int)(&zgsz3658);
      CONVERT_OF(sail_int, mach_int)(&zgsz3658, zcap_max_E);
      sail_int zgsz3659;
      CREATE(sail_int)(&zgsz3659);
      CONVERT_OF(sail_int, mach_int)(&zgsz3659, INT64_C(2));
      sail_int zgsz3660;
      CREATE(sail_int)(&zgsz3660);
      sub_int(&zgsz3660, zgsz3658, zgsz3659);
      zgaz3321 = CONVERT_OF(mach_int, sail_int)(zgsz3660);
      KILL(sail_int)(&zgsz3660);
      KILL(sail_int)(&zgsz3659);
      KILL(sail_int)(&zgsz3658);
    }
    {
      sail_int zgsz3656;
      CREATE(sail_int)(&zgsz3656);
      CONVERT_OF(sail_int, mach_int)(&zgsz3656, zE);
      sail_int zgsz3657;
      CREATE(sail_int)(&zgsz3657);
      CONVERT_OF(sail_int, mach_int)(&zgsz3657, zgaz3321);
      zgaz3322 = gteq(zgsz3656, zgsz3657);
      KILL(sail_int)(&zgsz3657);
      KILL(sail_int)(&zgsz3656);
    }
  }
  if (zgaz3322) {  zcbz337 = true;  } else {
    int64_t zi_top;
    {
      uint64_t zgaz3333;
      {
        int64_t zgaz3332;
        {
          sail_int zgsz3664;
          CREATE(sail_int)(&zgsz3664);
          CONVERT_OF(sail_int, mach_int)(&zgsz3664, zE);
          sail_int zgsz3665;
          CREATE(sail_int)(&zgsz3665);
          CONVERT_OF(sail_int, mach_int)(&zgsz3665, zcap_mantissa_width);
          sail_int zgsz3666;
          CREATE(sail_int)(&zgsz3666);
          add_int(&zgsz3666, zgsz3664, zgsz3665);
          zgaz3332 = CONVERT_OF(mach_int, sail_int)(zgsz3666);
          KILL(sail_int)(&zgsz3666);
          KILL(sail_int)(&zgsz3665);
          KILL(sail_int)(&zgsz3664);
        }
        {
          lbits zgsz3661;
          CREATE(lbits)(&zgsz3661);
          CONVERT_OF(lbits, fbits)(&zgsz3661, zi, UINT64_C(64) , true);
          sail_int zgsz3662;
          CREATE(sail_int)(&zgsz3662);
          CONVERT_OF(sail_int, mach_int)(&zgsz3662, zgaz3332);
          lbits zgsz3663;
          CREATE(lbits)(&zgsz3663);
          arith_shiftr(&zgsz3663, zgsz3661, zgsz3662);
          zgaz3333 = CONVERT_OF(fbits, lbits)(zgsz3663, true);
          KILL(lbits)(&zgsz3663);
          KILL(sail_int)(&zgsz3662);
          KILL(lbits)(&zgsz3661);
        }
      }
      {
        lbits zgsz3667;
        CREATE(lbits)(&zgsz3667);
        CONVERT_OF(lbits, fbits)(&zgsz3667, zgaz3333, UINT64_C(64) , true);
        sail_int zgsz3668;
        CREATE(sail_int)(&zgsz3668);
        sail_signed(&zgsz3668, zgsz3667);
        zi_top = CONVERT_OF(mach_int, sail_int)(zgsz3668);
        KILL(sail_int)(&zgsz3668);
        KILL(lbits)(&zgsz3667);
      }
    }
    uint64_t zi_mid;
    {
      uint64_t zgaz3331;
      {
        lbits zgsz3672;
        CREATE(lbits)(&zgsz3672);
        CONVERT_OF(lbits, fbits)(&zgsz3672, zi, UINT64_C(64) , true);
        sail_int zgsz3673;
        CREATE(sail_int)(&zgsz3673);
        CONVERT_OF(sail_int, mach_int)(&zgsz3673, zE);
        lbits zgsz3674;
        CREATE(lbits)(&zgsz3674);
        shiftr(&zgsz3674, zgsz3672, zgsz3673);
        zgaz3331 = CONVERT_OF(fbits, lbits)(zgsz3674, true);
        KILL(lbits)(&zgsz3674);
        KILL(sail_int)(&zgsz3673);
        KILL(lbits)(&zgsz3672);
      }
      {
        lbits zgsz3669;
        CREATE(lbits)(&zgsz3669);
        CONVERT_OF(lbits, fbits)(&zgsz3669, zgaz3331, UINT64_C(64) , true);
        sail_int zgsz3670;
        CREATE(sail_int)(&zgsz3670);
        CONVERT_OF(sail_int, mach_int)(&zgsz3670, zcap_mantissa_width);
        lbits zgsz3671;
        CREATE(lbits)(&zgsz3671);
        sail_truncate(&zgsz3671, zgsz3669, zgsz3670);
        zi_mid = CONVERT_OF(fbits, lbits)(zgsz3671, true);
        KILL(lbits)(&zgsz3671);
        KILL(sail_int)(&zgsz3670);
        KILL(lbits)(&zgsz3669);
      }
    }
    uint64_t za_mid;
    {
      uint64_t zgaz3330;
      {
        uint64_t zgaz3329;
        zgaz3329 = zc.zaddress;
        {
          lbits zgsz3675;
          CREATE(lbits)(&zgsz3675);
          CONVERT_OF(lbits, fbits)(&zgsz3675, zgaz3329, UINT64_C(64) , true);
          sail_int zgsz3676;
          CREATE(sail_int)(&zgsz3676);
          CONVERT_OF(sail_int, mach_int)(&zgsz3676, zE);
          lbits zgsz3677;
          CREATE(lbits)(&zgsz3677);
          shiftr(&zgsz3677, zgsz3675, zgsz3676);
          zgaz3330 = CONVERT_OF(fbits, lbits)(zgsz3677, true);
          KILL(lbits)(&zgsz3677);
          KILL(sail_int)(&zgsz3676);
          KILL(lbits)(&zgsz3675);
        }
      }
      {
        lbits zgsz3678;
        CREATE(lbits)(&zgsz3678);
        CONVERT_OF(lbits, fbits)(&zgsz3678, zgaz3330, UINT64_C(64) , true);
        sail_int zgsz3679;
        CREATE(sail_int)(&zgsz3679);
        CONVERT_OF(sail_int, mach_int)(&zgsz3679, zcap_mantissa_width);
        lbits zgsz3680;
        CREATE(lbits)(&zgsz3680);
        sail_truncate(&zgsz3680, zgsz3678, zgsz3679);
        za_mid = CONVERT_OF(fbits, lbits)(zgsz3680, true);
        KILL(lbits)(&zgsz3680);
        KILL(sail_int)(&zgsz3679);
        KILL(lbits)(&zgsz3678);
      }
    }
    uint64_t zB3;
    {
      uint64_t zgaz3328;
      zgaz3328 = zc.zB;
      {
        lbits zgsz3681;
        CREATE(lbits)(&zgsz3681);
        CONVERT_OF(lbits, fbits)(&zgsz3681, zgaz3328, UINT64_C(14) , true);
        sail_int zgsz3682;
        CREATE(sail_int)(&zgsz3682);
        CONVERT_OF(sail_int, mach_int)(&zgsz3682, INT64_C(3));
        lbits zgsz3683;
        CREATE(lbits)(&zgsz3683);
        sail_truncateLSB(&zgsz3683, zgsz3681, zgsz3682);
        zB3 = CONVERT_OF(fbits, lbits)(zgsz3683, true);
        KILL(lbits)(&zgsz3683);
        KILL(sail_int)(&zgsz3682);
        KILL(lbits)(&zgsz3681);
      }
    }
    uint64_t zR3;
    {
      lbits zgsz3708;
      CREATE(lbits)(&zgsz3708);
      CONVERT_OF(lbits, fbits)(&zgsz3708, zB3, UINT64_C(3) , true);
      lbits zgsz3709;
      CREATE(lbits)(&zgsz3709);
      CONVERT_OF(lbits, fbits)(&zgsz3709, UINT64_C(0b001), UINT64_C(3) , true);
      lbits zgsz3710;
      CREATE(lbits)(&zgsz3710);
      sub_bits(&zgsz3710, zgsz3708, zgsz3709);
      zR3 = CONVERT_OF(fbits, lbits)(zgsz3710, true);
      KILL(lbits)(&zgsz3710);
      KILL(lbits)(&zgsz3709);
      KILL(lbits)(&zgsz3708);
    }
    uint64_t zR;
    {
      uint64_t zgaz3327;
      {
        int64_t zgaz3326;
        {
          sail_int zgsz3686;
          CREATE(sail_int)(&zgsz3686);
          CONVERT_OF(sail_int, mach_int)(&zgsz3686, zcap_mantissa_width);
          sail_int zgsz3687;
          CREATE(sail_int)(&zgsz3687);
          CONVERT_OF(sail_int, mach_int)(&zgsz3687, INT64_C(3));
          sail_int zgsz3688;
          CREATE(sail_int)(&zgsz3688);
          sub_int(&zgsz3688, zgsz3686, zgsz3687);
          zgaz3326 = CONVERT_OF(mach_int, sail_int)(zgsz3688);
          KILL(sail_int)(&zgsz3688);
          KILL(sail_int)(&zgsz3687);
          KILL(sail_int)(&zgsz3686);
        }
        {
          sail_int zgsz3684;
          CREATE(sail_int)(&zgsz3684);
          CONVERT_OF(sail_int, mach_int)(&zgsz3684, zgaz3326);
          lbits zgsz3685;
          CREATE(lbits)(&zgsz3685);
          sailgen_zzeros_implicit(&zgsz3685, zgsz3684);
          zgaz3327 = CONVERT_OF(fbits, lbits)(zgsz3685, true);
          KILL(lbits)(&zgsz3685);
          KILL(sail_int)(&zgsz3684);
        }
      }
      {
        lbits zgsz3689;
        CREATE(lbits)(&zgsz3689);
        CONVERT_OF(lbits, fbits)(&zgsz3689, zR3, UINT64_C(3) , true);
        lbits zgsz3690;
        CREATE(lbits)(&zgsz3690);
        CONVERT_OF(lbits, fbits)(&zgsz3690, zgaz3327, UINT64_C(11) , true);
        lbits zgsz3691;
        CREATE(lbits)(&zgsz3691);
        append(&zgsz3691, zgsz3689, zgsz3690);
        zR = CONVERT_OF(fbits, lbits)(zgsz3691, true);
        KILL(lbits)(&zgsz3691);
        KILL(lbits)(&zgsz3690);
        KILL(lbits)(&zgsz3689);
      }
    }
    uint64_t zdiff;
    {
      lbits zgsz3705;
      CREATE(lbits)(&zgsz3705);
      CONVERT_OF(lbits, fbits)(&zgsz3705, zR, UINT64_C(14) , true);
      lbits zgsz3706;
      CREATE(lbits)(&zgsz3706);
      CONVERT_OF(lbits, fbits)(&zgsz3706, za_mid, UINT64_C(14) , true);
      lbits zgsz3707;
      CREATE(lbits)(&zgsz3707);
      sub_bits(&zgsz3707, zgsz3705, zgsz3706);
      zdiff = CONVERT_OF(fbits, lbits)(zgsz3707, true);
      KILL(lbits)(&zgsz3707);
      KILL(lbits)(&zgsz3706);
      KILL(lbits)(&zgsz3705);
    }
    uint64_t zdiff1;
    {
      lbits zgsz3702;
      CREATE(lbits)(&zgsz3702);
      CONVERT_OF(lbits, fbits)(&zgsz3702, zdiff, UINT64_C(14) , true);
      sail_int zgsz3703;
      CREATE(sail_int)(&zgsz3703);
      CONVERT_OF(sail_int, mach_int)(&zgsz3703, INT64_C(1));
      lbits zgsz3704;
      CREATE(lbits)(&zgsz3704);
      sub_bits_int(&zgsz3704, zgsz3702, zgsz3703);
      zdiff1 = CONVERT_OF(fbits, lbits)(zgsz3704, true);
      KILL(lbits)(&zgsz3704);
      KILL(sail_int)(&zgsz3703);
      KILL(lbits)(&zgsz3702);
    }
    bool zgaz3323;
    {
      sail_int zgsz3700;
      CREATE(sail_int)(&zgsz3700);
      CONVERT_OF(sail_int, mach_int)(&zgsz3700, zi_top);
      sail_int zgsz3701;
      CREATE(sail_int)(&zgsz3701);
      CONVERT_OF(sail_int, mach_int)(&zgsz3701, INT64_C(0));
      zgaz3323 = eq_int(zgsz3700, zgsz3701);
      KILL(sail_int)(&zgsz3701);
      KILL(sail_int)(&zgsz3700);
    }
    if (zgaz3323) {
      {
        lbits zgsz3698;
        CREATE(lbits)(&zgsz3698);
        CONVERT_OF(lbits, fbits)(&zgsz3698, zi_mid, UINT64_C(14) , true);
        lbits zgsz3699;
        CREATE(lbits)(&zgsz3699);
        CONVERT_OF(lbits, fbits)(&zgsz3699, zdiff1, UINT64_C(14) , true);
        zcbz337 = sailgen_z8operatorz0zI_uz9(zgsz3698, zgsz3699);
        KILL(lbits)(&zgsz3699);
        KILL(lbits)(&zgsz3698);
      }
    } else {
      bool zgaz3324;
      {
        sail_int zgsz3696;
        CREATE(sail_int)(&zgsz3696);
        CONVERT_OF(sail_int, mach_int)(&zgsz3696, zi_top);
        sail_int zgsz3697;
        CREATE(sail_int)(&zgsz3697);
        CONVERT_OF(sail_int, mach_int)(&zgsz3697, INT64_C(-1));
        zgaz3324 = eq_int(zgsz3696, zgsz3697);
        KILL(sail_int)(&zgsz3697);
        KILL(sail_int)(&zgsz3696);
      }
      if (zgaz3324) {
        bool zgaz3325;
        {
          lbits zgsz3694;
          CREATE(lbits)(&zgsz3694);
          CONVERT_OF(lbits, fbits)(&zgsz3694, zi_mid, UINT64_C(14) , true);
          lbits zgsz3695;
          CREATE(lbits)(&zgsz3695);
          CONVERT_OF(lbits, fbits)(&zgsz3695, zdiff, UINT64_C(14) , true);
          zgaz3325 = sailgen_z8operatorz0zKzJ_uz9(zgsz3694, zgsz3695);
          KILL(lbits)(&zgsz3695);
          KILL(lbits)(&zgsz3694);
        }
        bool zgsz384;
        if (zgaz3325) {
          {
            lbits zgsz3692;
            CREATE(lbits)(&zgsz3692);
            CONVERT_OF(lbits, fbits)(&zgsz3692, zR, UINT64_C(14) , true);
            lbits zgsz3693;
            CREATE(lbits)(&zgsz3693);
            CONVERT_OF(lbits, fbits)(&zgsz3693, za_mid, UINT64_C(14) , true);
            zgsz384 = neq_bits(zgsz3692, zgsz3693);
            KILL(lbits)(&zgsz3693);
            KILL(lbits)(&zgsz3692);
          }
        } else {  zgsz384 = false;  }
        zcbz337 = zgsz384;
      } else {  zcbz337 = false;  }
    }
  }


end_function_157: ;
  return zcbz337;
end_block_exception_158: ;

  return false;
}

static void sailgen_capToString(sail_string *rop, struct zCapability);

static void sailgen_capToString(sail_string *zcbz338, struct zCapability zcap)
{
  __label__ end_function_160, end_block_exception_161, end_function_182;

  sail_int zlen;
  CREATE(sail_int)(&zlen);
  sailgen_getCapLength(&zlen, zcap);
  sail_string zlen_str;
  CREATE(sail_string)(&zlen_str);
  {
    lbits zgaz3363;
    CREATE(lbits)(&zgaz3363);
    {
      int64_t zgaz3362;
      {
        sail_int zgsz3712;
        CREATE(sail_int)(&zgsz3712);
        CONVERT_OF(sail_int, mach_int)(&zgsz3712, zcap_len_width);
        sail_int zgsz3713;
        CREATE(sail_int)(&zgsz3713);
        CONVERT_OF(sail_int, mach_int)(&zgsz3713, INT64_C(3));
        sail_int zgsz3714;
        CREATE(sail_int)(&zgsz3714);
        add_int(&zgsz3714, zgsz3712, zgsz3713);
        zgaz3362 = CONVERT_OF(mach_int, sail_int)(zgsz3714);
        KILL(sail_int)(&zgsz3714);
        KILL(sail_int)(&zgsz3713);
        KILL(sail_int)(&zgsz3712);
      }
      {
        sail_int zgsz3711;
        CREATE(sail_int)(&zgsz3711);
        CONVERT_OF(sail_int, mach_int)(&zgsz3711, zgaz3362);
        sailgen_to_bits(&zgaz3363, zgsz3711, zlen);
        KILL(sail_int)(&zgsz3711);
      }
    }
    string_of_lbits(&zlen_str, zgaz3363);
    KILL(lbits)(&zgaz3363);
  }
  uint64_t zotype64;
  {
    bool zgaz3359;
    zgaz3359 = sailgen_hasReservedOType(zcap);
    if (zgaz3359) {
      uint64_t zgaz3360;
      zgaz3360 = zcap.zotype;
      {
        sail_int zgsz3718;
        CREATE(sail_int)(&zgsz3718);
        CONVERT_OF(sail_int, mach_int)(&zgsz3718, INT64_C(64));
        lbits zgsz3719;
        CREATE(lbits)(&zgsz3719);
        CONVERT_OF(lbits, fbits)(&zgsz3719, zgaz3360, UINT64_C(18) , true);
        lbits zgsz3720;
        CREATE(lbits)(&zgsz3720);
        sailgen_EXTS(&zgsz3720, zgsz3718, zgsz3719);
        zotype64 = CONVERT_OF(fbits, lbits)(zgsz3720, true);
        KILL(lbits)(&zgsz3720);
        KILL(lbits)(&zgsz3719);
        KILL(sail_int)(&zgsz3718);
      }
    } else {
      uint64_t zgaz3361;
      zgaz3361 = zcap.zotype;
      {
        sail_int zgsz3715;
        CREATE(sail_int)(&zgsz3715);
        CONVERT_OF(sail_int, mach_int)(&zgsz3715, INT64_C(64));
        lbits zgsz3716;
        CREATE(lbits)(&zgsz3716);
        CONVERT_OF(lbits, fbits)(&zgsz3716, zgaz3361, UINT64_C(18) , true);
        lbits zgsz3717;
        CREATE(lbits)(&zgsz3717);
        sailgen_EXTZ(&zgsz3717, zgsz3715, zgsz3716);
        zotype64 = CONVERT_OF(fbits, lbits)(zgsz3717, true);
        KILL(lbits)(&zgsz3717);
        KILL(lbits)(&zgsz3716);
        KILL(sail_int)(&zgsz3715);
      }
    }
  }
  sail_string zgaz3358;
  CREATE(sail_string)(&zgaz3358);
  {
    sail_string zgaz3356;
    CREATE(sail_string)(&zgaz3356);
    {
      bool zgaz3335;
      zgaz3335 = zcap.ztag;
      if (zgaz3335) {  COPY(sail_string)(&zgaz3356, "1");  } else {  COPY(sail_string)(&zgaz3356, "0");  }
    }
    sail_string zgaz3357;
    CREATE(sail_string)(&zgaz3357);
    {
      sail_string zgaz3355;
      CREATE(sail_string)(&zgaz3355);
      {
        sail_string zgaz3353;
        CREATE(sail_string)(&zgaz3353);
        {
          bool zgaz3336;
          zgaz3336 = sailgen_isCapSealed(zcap);
          if (zgaz3336) {  COPY(sail_string)(&zgaz3353, "1");  } else {  COPY(sail_string)(&zgaz3353, "0");  }
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
              uint64_t zgaz3338;
              {
                uint64_t zgaz3337;
                zgaz3337 = sailgen_getCapPerms(zcap);
                {
                  lbits zgsz3721;
                  CREATE(lbits)(&zgsz3721);
                  CONVERT_OF(lbits, fbits)(&zgsz3721, UINT64_C(0b0), UINT64_C(1) , true);
                  lbits zgsz3722;
                  CREATE(lbits)(&zgsz3722);
                  CONVERT_OF(lbits, fbits)(&zgsz3722, zgaz3337, UINT64_C(19) , true);
                  lbits zgsz3723;
                  CREATE(lbits)(&zgsz3723);
                  append(&zgsz3723, zgsz3721, zgsz3722);
                  zgaz3338 = CONVERT_OF(fbits, lbits)(zgsz3723, true);
                  KILL(lbits)(&zgsz3723);
                  KILL(lbits)(&zgsz3722);
                  KILL(lbits)(&zgsz3721);
                }
              }
              {
                lbits zgsz3724;
                CREATE(lbits)(&zgsz3724);
                CONVERT_OF(lbits, fbits)(&zgsz3724, zgaz3338, UINT64_C(20) , true);
                string_of_lbits(&zgaz3350, zgsz3724);
                KILL(lbits)(&zgsz3724);
              }
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
                  lbits zgsz3727;
                  CREATE(lbits)(&zgsz3727);
                  CONVERT_OF(lbits, fbits)(&zgsz3727, zotype64, UINT64_C(64) , true);
                  string_of_lbits(&zgaz3347, zgsz3727);
                  KILL(lbits)(&zgsz3727);
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
                      uint64_t zgaz3339;
                      zgaz3339 = sailgen_getCapOffsetBits(zcap);
                      {
                        lbits zgsz3725;
                        CREATE(lbits)(&zgsz3725);
                        CONVERT_OF(lbits, fbits)(&zgsz3725, zgaz3339, UINT64_C(64) , true);
                        string_of_lbits(&zgaz3344, zgsz3725);
                        KILL(lbits)(&zgsz3725);
                      }
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
                          uint64_t zgaz3340;
                          zgaz3340 = sailgen_getCapBaseBits(zcap);
                          {
                            lbits zgsz3726;
                            CREATE(lbits)(&zgsz3726);
                            CONVERT_OF(lbits, fbits)(&zgsz3726, zgaz3340, UINT64_C(64) , true);
                            string_of_lbits(&zgaz3341, zgsz3726);
                            KILL(lbits)(&zgsz3726);
                          }
                        }
                        sail_string zgaz3342;
                        CREATE(sail_string)(&zgaz3342);
                        concat_str(&zgaz3342, " length:", zlen_str);
                        concat_str(&zgaz3343, zgaz3341, zgaz3342);
                        KILL(sail_string)(&zgaz3342);
                        KILL(sail_string)(&zgaz3341);
                      }
                      concat_str(&zgaz3345, " base:", zgaz3343);
                      KILL(sail_string)(&zgaz3343);
                    }
                    concat_str(&zgaz3346, zgaz3344, zgaz3345);
                    KILL(sail_string)(&zgaz3345);
                    KILL(sail_string)(&zgaz3344);
                  }
                  concat_str(&zgaz3348, " offset:", zgaz3346);
                  KILL(sail_string)(&zgaz3346);
                }
                concat_str(&zgaz3349, zgaz3347, zgaz3348);
                KILL(sail_string)(&zgaz3348);
                KILL(sail_string)(&zgaz3347);
              }
              concat_str(&zgaz3351, " type:", zgaz3349);
              KILL(sail_string)(&zgaz3349);
            }
            concat_str(&zgaz3352, zgaz3350, zgaz3351);
            KILL(sail_string)(&zgaz3351);
            KILL(sail_string)(&zgaz3350);
          }
          concat_str(&zgaz3354, " perms:", zgaz3352);
          KILL(sail_string)(&zgaz3352);
        }
        concat_str(&zgaz3355, zgaz3353, zgaz3354);
        KILL(sail_string)(&zgaz3354);
        KILL(sail_string)(&zgaz3353);
      }
      concat_str(&zgaz3357, " s:", zgaz3355);
      KILL(sail_string)(&zgaz3355);
    }
    concat_str(&zgaz3358, zgaz3356, zgaz3357);
    KILL(sail_string)(&zgaz3357);
    KILL(sail_string)(&zgaz3356);
  }
  concat_str((*(&zcbz338)), " t:", zgaz3358);
  KILL(sail_string)(&zgaz3358);

  KILL(sail_string)(&zlen_str);
  KILL(sail_int)(&zlen);
end_function_160: ;
  goto end_function_182;
end_block_exception_161: ;
  goto end_function_182;
end_function_182: ;
}

static uint64_t sailgen_getRepresentableAlignmentMask(uint64_t);

static uint64_t sailgen_getRepresentableAlignmentMask(uint64_t zlen)
{
  __label__ case_164, finish_match_163, end_function_165, end_block_exception_166;

  uint64_t zcbz339;
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3366;
  {
    uint64_t zgaz3364;
    {
      sail_int zgsz3730;
      CREATE(sail_int)(&zgsz3730);
      CONVERT_OF(sail_int, mach_int)(&zgsz3730, INT64_C(64));
      sail_int zgsz3731;
      CREATE(sail_int)(&zgsz3731);
      CONVERT_OF(sail_int, mach_int)(&zgsz3731, INT64_C(0));
      lbits zgsz3732;
      CREATE(lbits)(&zgsz3732);
      sailgen_to_bits(&zgsz3732, zgsz3730, zgsz3731);
      zgaz3364 = CONVERT_OF(fbits, lbits)(zgsz3732, true);
      KILL(lbits)(&zgsz3732);
      KILL(sail_int)(&zgsz3731);
      KILL(sail_int)(&zgsz3730);
    }
    lbits zgaz3365;
    CREATE(lbits)(&zgaz3365);
    {
      lbits zgsz3728;
      CREATE(lbits)(&zgsz3728);
      CONVERT_OF(lbits, fbits)(&zgsz3728, UINT64_C(0b0), UINT64_C(1) , true);
      lbits zgsz3729;
      CREATE(lbits)(&zgsz3729);
      CONVERT_OF(lbits, fbits)(&zgsz3729, zlen, UINT64_C(64) , true);
      append(&zgaz3365, zgsz3728, zgsz3729);
      KILL(lbits)(&zgsz3729);
      KILL(lbits)(&zgsz3728);
    }
    zgaz3366 = sailgen_setCapBounds(zdefault_cap, zgaz3364, zgaz3365);
    KILL(lbits)(&zgaz3365);
  }
  uint64_t zgsz385;
  {
    bool zexact;
    zexact = zgaz3366.ztup0;
    struct zCapability zc;
    zc = zgaz3366.ztup1;
    int64_t ze;
    {
      int64_t zgaz3372;
      {
        uint64_t zgaz3371;
        zgaz3371 = zc.zE;
        {
          lbits zgsz3733;
          CREATE(lbits)(&zgsz3733);
          CONVERT_OF(lbits, fbits)(&zgsz3733, zgaz3371, UINT64_C(6) , true);
          sail_int zgsz3734;
          CREATE(sail_int)(&zgsz3734);
          sail_unsigned(&zgsz3734, zgsz3733);
          zgaz3372 = CONVERT_OF(mach_int, sail_int)(zgsz3734);
          KILL(sail_int)(&zgsz3734);
          KILL(lbits)(&zgsz3733);
        }
      }
      {
        sail_int zgsz3735;
        CREATE(sail_int)(&zgsz3735);
        CONVERT_OF(sail_int, mach_int)(&zgsz3735, zgaz3372);
        sail_int zgsz3736;
        CREATE(sail_int)(&zgsz3736);
        CONVERT_OF(sail_int, mach_int)(&zgsz3736, zcap_max_E);
        sail_int zgsz3737;
        CREATE(sail_int)(&zgsz3737);
        min_int(&zgsz3737, zgsz3735, zgsz3736);
        ze = CONVERT_OF(mach_int, sail_int)(zgsz3737);
        KILL(sail_int)(&zgsz3737);
        KILL(sail_int)(&zgsz3736);
        KILL(sail_int)(&zgsz3735);
      }
    }
    int64_t zez7;
    {
      bool zgaz3370;
      zgaz3370 = zc.zinternal_E;
      if (zgaz3370) {
        {
          sail_int zgsz3738;
          CREATE(sail_int)(&zgsz3738);
          CONVERT_OF(sail_int, mach_int)(&zgsz3738, ze);
          sail_int zgsz3739;
          CREATE(sail_int)(&zgsz3739);
          CONVERT_OF(sail_int, mach_int)(&zgsz3739, zinternal_E_take_bits);
          sail_int zgsz3740;
          CREATE(sail_int)(&zgsz3740);
          add_int(&zgsz3740, zgsz3738, zgsz3739);
          zez7 = CONVERT_OF(mach_int, sail_int)(zgsz3740);
          KILL(sail_int)(&zgsz3740);
          KILL(sail_int)(&zgsz3739);
          KILL(sail_int)(&zgsz3738);
        }
      } else {  zez7 = INT64_C(0);  }
    }
    sbits zgaz3368;
    {
      int64_t zgaz3367;
      {
        sail_int zgsz3743;
        CREATE(sail_int)(&zgsz3743);
        CONVERT_OF(sail_int, mach_int)(&zgsz3743, INT64_C(64));
        sail_int zgsz3744;
        CREATE(sail_int)(&zgsz3744);
        CONVERT_OF(sail_int, mach_int)(&zgsz3744, zez7);
        sail_int zgsz3745;
        CREATE(sail_int)(&zgsz3745);
        sub_int(&zgsz3745, zgsz3743, zgsz3744);
        zgaz3367 = CONVERT_OF(mach_int, sail_int)(zgsz3745);
        KILL(sail_int)(&zgsz3745);
        KILL(sail_int)(&zgsz3744);
        KILL(sail_int)(&zgsz3743);
      }
      {
        sail_int zgsz3741;
        CREATE(sail_int)(&zgsz3741);
        CONVERT_OF(sail_int, mach_int)(&zgsz3741, zgaz3367);
        lbits zgsz3742;
        CREATE(lbits)(&zgsz3742);
        sailgen_ones(&zgsz3742, zgsz3741);
        zgaz3368 = CONVERT_OF(sbits, lbits)(zgsz3742, true);
        KILL(lbits)(&zgsz3742);
        KILL(sail_int)(&zgsz3741);
      }
    }
    sbits zgaz3369;
    {
      sail_int zgsz3749;
      CREATE(sail_int)(&zgsz3749);
      CONVERT_OF(sail_int, mach_int)(&zgsz3749, zez7);
      lbits zgsz3750;
      CREATE(lbits)(&zgsz3750);
      sailgen_zzeros_implicit(&zgsz3750, zgsz3749);
      zgaz3369 = CONVERT_OF(sbits, lbits)(zgsz3750, true);
      KILL(lbits)(&zgsz3750);
      KILL(sail_int)(&zgsz3749);
    }
    {
      lbits zgsz3746;
      CREATE(lbits)(&zgsz3746);
      CONVERT_OF(lbits, sbits)(&zgsz3746, zgaz3368, true);
      lbits zgsz3747;
      CREATE(lbits)(&zgsz3747);
      CONVERT_OF(lbits, sbits)(&zgsz3747, zgaz3369, true);
      lbits zgsz3748;
      CREATE(lbits)(&zgsz3748);
      append(&zgsz3748, zgsz3746, zgsz3747);
      zgsz385 = CONVERT_OF(fbits, lbits)(zgsz3748, true);
      KILL(lbits)(&zgsz3748);
      KILL(lbits)(&zgsz3747);
      KILL(lbits)(&zgsz3746);
    }
    goto finish_match_163;
  }
case_164: ;
  sail_match_failure("getRepresentableAlignmentMask");
finish_match_163: ;
  zcbz339 = zgsz385;


end_function_165: ;
  return zcbz339;
end_block_exception_166: ;

  return UINT64_C(0xdeadc0de);
}

static uint64_t sailgen_getRepresentableLength(uint64_t);

static uint64_t sailgen_getRepresentableLength(uint64_t zlen)
{
  __label__ end_function_168, end_block_exception_169;

  uint64_t zcbz340;
  uint64_t zm;
  zm = sailgen_getRepresentableAlignmentMask(zlen);
  uint64_t zgaz3374;
  {
    uint64_t zgaz3373;
    {
      lbits zgsz3754;
      CREATE(lbits)(&zgsz3754);
      CONVERT_OF(lbits, fbits)(&zgsz3754, zm, UINT64_C(64) , true);
      lbits zgsz3755;
      CREATE(lbits)(&zgsz3755);
      not_bits(&zgsz3755, zgsz3754);
      zgaz3373 = CONVERT_OF(fbits, lbits)(zgsz3755, true);
      KILL(lbits)(&zgsz3755);
      KILL(lbits)(&zgsz3754);
    }
    {
      lbits zgsz3751;
      CREATE(lbits)(&zgsz3751);
      CONVERT_OF(lbits, fbits)(&zgsz3751, zlen, UINT64_C(64) , true);
      lbits zgsz3752;
      CREATE(lbits)(&zgsz3752);
      CONVERT_OF(lbits, fbits)(&zgsz3752, zgaz3373, UINT64_C(64) , true);
      lbits zgsz3753;
      CREATE(lbits)(&zgsz3753);
      add_bits(&zgsz3753, zgsz3751, zgsz3752);
      zgaz3374 = CONVERT_OF(fbits, lbits)(zgsz3753, true);
      KILL(lbits)(&zgsz3753);
      KILL(lbits)(&zgsz3752);
      KILL(lbits)(&zgsz3751);
    }
  }
  {
    lbits zgsz3756;
    CREATE(lbits)(&zgsz3756);
    CONVERT_OF(lbits, fbits)(&zgsz3756, zgaz3374, UINT64_C(64) , true);
    lbits zgsz3757;
    CREATE(lbits)(&zgsz3757);
    CONVERT_OF(lbits, fbits)(&zgsz3757, zm, UINT64_C(64) , true);
    lbits zgsz3758;
    CREATE(lbits)(&zgsz3758);
    and_bits(&zgsz3758, zgsz3756, zgsz3757);
    zcbz340 = CONVERT_OF(fbits, lbits)(zgsz3758, true);
    KILL(lbits)(&zgsz3758);
    KILL(lbits)(&zgsz3757);
    KILL(lbits)(&zgsz3756);
  }


end_function_168: ;
  return zcbz340;
end_block_exception_169: ;

  return UINT64_C(0xdeadc0de);
}

// register TestCap
static struct zCapability zTestCap;

// register TestAddr
static uint64_t zTestAddr;

// register TestLen
static lbits zTestLen;

static unit sailgen_main(unit);


static unit sailgen_main(unit zgsz387)
{
  __label__ case_172, finish_match_171, end_function_177, end_block_exception_178;

  unit zcbz341;
  {
    zTestCap = znull_cap;
    unit zgsz388;
    zgsz388 = UNIT;
  }
  lbits zbits1;
  CREATE(lbits)(&zbits1);
  sailgen_capToBits(&zbits1, zTestCap);
  lbits zbits2;
  CREATE(lbits)(&zbits2);
  sailgen_capToMemBits(&zbits2, zTestCap);
  struct zCapability zcap1;
  {
    bool zgaz3381;
    zgaz3381 = zTestCap.ztag;
    zcap1 = sailgen_capBitsToCapability(zgaz3381, zbits1);
  }
  struct zCapability zcap2;
  {
    bool zgaz3380;
    zgaz3380 = zTestCap.ztag;
    zcap2 = sailgen_memBitsToCapability(zgaz3380, zbits2);
  }
  bool zrep;
  zrep = sailgen_fastRepCheck(zTestCap, zTestAddr);
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3375;
  zgaz3375 = sailgen_setCapBounds(zTestCap, zTestAddr, zTestLen);
  unit zgsz389;
  {
    __label__ case_174, finish_match_173;

    bool zexact;
    zexact = zgaz3375.ztup0;
    struct zCapability zcap;
    zcap = zgaz3375.ztup1;
    struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3376;
    zgaz3376 = sailgen_setCapAddr(zTestCap, zTestAddr);
    unit zgsz390;
    {
      __label__ case_176, finish_match_175;

      bool zrepresentable;
      zrepresentable = zgaz3376.ztup0;
      struct zCapability zcapshadowz30;
      zcapshadowz30 = zgaz3376.ztup1;
      struct ztuple_z8z5izCz0z5iz9 zgaz3377;
      CREATE(ztuple_z8z5izCz0z5iz9)(&zgaz3377);
      sailgen_getCapBounds(&zgaz3377, zTestCap);
      unit zgsz391;
      {
        sail_int zbase;
        CREATE(sail_int)(&zbase);
        COPY(sail_int)(&zbase, zgaz3377.ztup0);
        sail_int ztop;
        CREATE(sail_int)(&ztop);
        COPY(sail_int)(&ztop, zgaz3377.ztup1);
        uint64_t zflags;
        zflags = sailgen_getCapFlags(zTestCap);
        struct zCapability znew_cap;
        {
          uint64_t zgaz3379;
          {
            int64_t zgaz3378;
            {
              sail_int zgsz3761;
              CREATE(sail_int)(&zgsz3761);
              CONVERT_OF(sail_int, mach_int)(&zgsz3761, INT64_C(15));
              sail_int zgsz3762;
              CREATE(sail_int)(&zgsz3762);
              CONVERT_OF(sail_int, mach_int)(&zgsz3762, INT64_C(4));
              sail_int zgsz3763;
              CREATE(sail_int)(&zgsz3763);
              add_int(&zgsz3763, zgsz3761, zgsz3762);
              zgaz3378 = CONVERT_OF(mach_int, sail_int)(zgsz3763);
              KILL(sail_int)(&zgsz3763);
              KILL(sail_int)(&zgsz3762);
              KILL(sail_int)(&zgsz3761);
            }
            {
              sail_int zgsz3759;
              CREATE(sail_int)(&zgsz3759);
              CONVERT_OF(sail_int, mach_int)(&zgsz3759, zgaz3378);
              lbits zgsz3760;
              CREATE(lbits)(&zgsz3760);
              sailgen_ones(&zgsz3760, zgsz3759);
              zgaz3379 = CONVERT_OF(fbits, lbits)(zgsz3760, true);
              KILL(lbits)(&zgsz3760);
              KILL(sail_int)(&zgsz3759);
            }
          }
          znew_cap = sailgen_setCapPerms(zTestCap, zgaz3379);
        }
        sail_string zcap_str;
        CREATE(sail_string)(&zcap_str);
        sailgen_capToString(&zcap_str, zTestCap);
        uint64_t zlen;
        zlen = sailgen_getRepresentableLength(zTestAddr);
        uint64_t zmask;
        zmask = sailgen_getRepresentableAlignmentMask(zTestAddr);
        zgsz391 = UNIT;
        KILL(sail_string)(&zcap_str);
        KILL(sail_int)(&ztop);
        KILL(sail_int)(&zbase);
        goto finish_match_175;
      }
    case_176: ;
      sail_match_failure("main");
    finish_match_175: ;
      zgsz390 = zgsz391;
      KILL(ztuple_z8z5izCz0z5iz9)(&zgaz3377);
      goto finish_match_173;
    }
  case_174: ;
    sail_match_failure("main");
  finish_match_173: ;
    zgsz389 = zgsz390;
    goto finish_match_171;
  }
case_172: ;
  sail_match_failure("main");
finish_match_171: ;
  zcbz341 = zgsz389;





  KILL(lbits)(&zbits2);
  KILL(lbits)(&zbits1);
end_function_177: ;
  return zcbz341;
end_block_exception_178: ;

  return UNIT;
}

static unit sailgen_initializze_registers(unit);

static unit sailgen_initializze_registers(unit zgsz395)
{
  __label__ end_function_180, end_block_exception_181;

  unit zcbz342;
  {
    zTestCap = sailgen_undefined_Capability(UNIT);
    unit zgsz397;
    zgsz397 = UNIT;
  }
  {
    {
      sail_int zgsz3764;
      CREATE(sail_int)(&zgsz3764);
      CONVERT_OF(sail_int, mach_int)(&zgsz3764, INT64_C(64));
      lbits zgsz3765;
      CREATE(lbits)(&zgsz3765);
      UNDEFINED(lbits)(&zgsz3765, zgsz3764);
      zTestAddr = CONVERT_OF(fbits, lbits)(zgsz3765, true);
      KILL(lbits)(&zgsz3765);
      KILL(sail_int)(&zgsz3764);
    }
    unit zgsz396;
    zgsz396 = UNIT;
  }
  int64_t zgaz3382;
  {
    sail_int zgsz3767;
    CREATE(sail_int)(&zgsz3767);
    CONVERT_OF(sail_int, mach_int)(&zgsz3767, INT64_C(64));
    sail_int zgsz3768;
    CREATE(sail_int)(&zgsz3768);
    CONVERT_OF(sail_int, mach_int)(&zgsz3768, INT64_C(1));
    sail_int zgsz3769;
    CREATE(sail_int)(&zgsz3769);
    add_int(&zgsz3769, zgsz3767, zgsz3768);
    zgaz3382 = CONVERT_OF(mach_int, sail_int)(zgsz3769);
    KILL(sail_int)(&zgsz3769);
    KILL(sail_int)(&zgsz3768);
    KILL(sail_int)(&zgsz3767);
  }
  {
    sail_int zgsz3766;
    CREATE(sail_int)(&zgsz3766);
    CONVERT_OF(sail_int, mach_int)(&zgsz3766, zgaz3382);
    UNDEFINED(lbits)(&zTestLen, zgsz3766);
    KILL(sail_int)(&zgsz3766);
  }
  zcbz342 = UNIT;

end_function_180: ;
  return zcbz342;
end_block_exception_181: ;

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


