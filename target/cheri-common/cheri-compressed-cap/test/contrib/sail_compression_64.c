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
  __label__ end_function_7, end_block_exception_8, end_function_189;

  lbits zgaz32;
  CREATE(lbits)(&zgaz32);
  zeros(&zgaz32, zn);
  not_bits((*(&zcbz32)), zgaz32);
  KILL(lbits)(&zgaz32);
end_function_7: ;
  goto end_function_189;
end_block_exception_8: ;
  goto end_function_189;
end_function_189: ;
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
  __label__ end_function_13, end_block_exception_14, end_function_188;

  sign_extend((*(&zcbz34)), zv, zm);
end_function_13: ;
  goto end_function_188;
end_block_exception_14: ;
  goto end_function_188;
end_function_188: ;
}

static void sailgen_EXTZ(lbits *zcbz35, sail_int zm, lbits zv)
{
  __label__ end_function_16, end_block_exception_17, end_function_187;

  zero_extend((*(&zcbz35)), zv, zm);
end_function_16: ;
  goto end_function_187;
end_block_exception_17: ;
  goto end_function_187;
end_function_187: ;
}

static void sailgen_zzeros_implicit(lbits *rop, sail_int);

static void sailgen_zzeros_implicit(lbits *zcbz36, sail_int zn)
{
  __label__ end_function_19, end_block_exception_20, end_function_186;

  zeros((*(&zcbz36)), zn);
end_function_19: ;
  goto end_function_186;
end_block_exception_20: ;
  goto end_function_186;
end_function_186: ;
}

static void sailgen_ones(lbits *rop, sail_int);

static void sailgen_ones(lbits *zcbz37, sail_int zn)
{
  __label__ end_function_22, end_block_exception_23, end_function_185;

  sailgen_sail_ones((*(&zcbz37)), zn);
end_function_22: ;
  goto end_function_185;
end_block_exception_23: ;
  goto end_function_185;
end_function_185: ;
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
  __label__ end_function_34, end_block_exception_35, end_function_184;

  {
    sail_int zgsz398;
    CREATE(sail_int)(&zgsz398);
    CONVERT_OF(sail_int, mach_int)(&zgsz398, INT64_C(0));
    get_slice_int((*(&zcbz310)), zl, zn, zgsz398);
    KILL(sail_int)(&zgsz398);
  }
end_function_34: ;
  goto end_function_184;
end_block_exception_35: ;
  goto end_function_184;
end_function_184: ;
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
  __label__ end_function_43, end_block_exception_44, end_function_183;

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
  goto end_function_183;
end_block_exception_44: ;
  goto end_function_183;
end_function_183: ;
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
    sail_int zgsz3100;
    CREATE(sail_int)(&zgsz3100);
    CONVERT_OF(sail_int, mach_int)(&zgsz3100, INT64_C(32));
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
    lbits zgsz3125;
    CREATE(lbits)(&zgsz3125);
    CONVERT_OF(lbits, fbits)(&zgsz3125, zc, UINT64_C(64) , true);
    sail_int zgsz3126;
    CREATE(sail_int)(&zgsz3126);
    CONVERT_OF(sail_int, mach_int)(&zgsz3126, INT64_C(63));
    sail_int zgsz3127;
    CREATE(sail_int)(&zgsz3127);
    CONVERT_OF(sail_int, mach_int)(&zgsz3127, INT64_C(52));
    lbits zgsz3128;
    CREATE(lbits)(&zgsz3128);
    vector_subrange_lbits(&zgsz3128, zgsz3125, zgsz3126, zgsz3127);
    zgaz39 = CONVERT_OF(fbits, lbits)(zgsz3128, true);
    KILL(lbits)(&zgsz3128);
    KILL(sail_int)(&zgsz3127);
    KILL(sail_int)(&zgsz3126);
    KILL(lbits)(&zgsz3125);
  }
  uint64_t zgaz310;
  {
    lbits zgsz3121;
    CREATE(lbits)(&zgsz3121);
    CONVERT_OF(lbits, fbits)(&zgsz3121, zc, UINT64_C(64) , true);
    sail_int zgsz3122;
    CREATE(sail_int)(&zgsz3122);
    CONVERT_OF(sail_int, mach_int)(&zgsz3122, INT64_C(51));
    sail_int zgsz3123;
    CREATE(sail_int)(&zgsz3123);
    CONVERT_OF(sail_int, mach_int)(&zgsz3123, INT64_C(51));
    lbits zgsz3124;
    CREATE(lbits)(&zgsz3124);
    vector_subrange_lbits(&zgsz3124, zgsz3121, zgsz3122, zgsz3123);
    zgaz310 = CONVERT_OF(fbits, lbits)(zgsz3124, true);
    KILL(lbits)(&zgsz3124);
    KILL(sail_int)(&zgsz3123);
    KILL(sail_int)(&zgsz3122);
    KILL(lbits)(&zgsz3121);
  }
  uint64_t zgaz311;
  {
    lbits zgsz3117;
    CREATE(lbits)(&zgsz3117);
    CONVERT_OF(lbits, fbits)(&zgsz3117, zc, UINT64_C(64) , true);
    sail_int zgsz3118;
    CREATE(sail_int)(&zgsz3118);
    CONVERT_OF(sail_int, mach_int)(&zgsz3118, INT64_C(50));
    sail_int zgsz3119;
    CREATE(sail_int)(&zgsz3119);
    CONVERT_OF(sail_int, mach_int)(&zgsz3119, INT64_C(47));
    lbits zgsz3120;
    CREATE(lbits)(&zgsz3120);
    vector_subrange_lbits(&zgsz3120, zgsz3117, zgsz3118, zgsz3119);
    zgaz311 = CONVERT_OF(fbits, lbits)(zgsz3120, true);
    KILL(lbits)(&zgsz3120);
    KILL(sail_int)(&zgsz3119);
    KILL(sail_int)(&zgsz3118);
    KILL(lbits)(&zgsz3117);
  }
  fbits zgaz312;
  {
    lbits zgsz3115;
    CREATE(lbits)(&zgsz3115);
    CONVERT_OF(lbits, fbits)(&zgsz3115, zc, UINT64_C(64) , true);
    sail_int zgsz3116;
    CREATE(sail_int)(&zgsz3116);
    CONVERT_OF(sail_int, mach_int)(&zgsz3116, INT64_C(46));
    zgaz312 = bitvector_access(zgsz3115, zgsz3116);
    KILL(sail_int)(&zgsz3116);
    KILL(lbits)(&zgsz3115);
  }
  uint64_t zgaz313;
  {
    lbits zgsz3111;
    CREATE(lbits)(&zgsz3111);
    CONVERT_OF(lbits, fbits)(&zgsz3111, zc, UINT64_C(64) , true);
    sail_int zgsz3112;
    CREATE(sail_int)(&zgsz3112);
    CONVERT_OF(sail_int, mach_int)(&zgsz3112, INT64_C(45));
    sail_int zgsz3113;
    CREATE(sail_int)(&zgsz3113);
    CONVERT_OF(sail_int, mach_int)(&zgsz3113, INT64_C(40));
    lbits zgsz3114;
    CREATE(lbits)(&zgsz3114);
    vector_subrange_lbits(&zgsz3114, zgsz3111, zgsz3112, zgsz3113);
    zgaz313 = CONVERT_OF(fbits, lbits)(zgsz3114, true);
    KILL(lbits)(&zgsz3114);
    KILL(sail_int)(&zgsz3113);
    KILL(sail_int)(&zgsz3112);
    KILL(lbits)(&zgsz3111);
  }
  uint64_t zgaz314;
  {
    lbits zgsz3107;
    CREATE(lbits)(&zgsz3107);
    CONVERT_OF(lbits, fbits)(&zgsz3107, zc, UINT64_C(64) , true);
    sail_int zgsz3108;
    CREATE(sail_int)(&zgsz3108);
    CONVERT_OF(sail_int, mach_int)(&zgsz3108, INT64_C(39));
    sail_int zgsz3109;
    CREATE(sail_int)(&zgsz3109);
    CONVERT_OF(sail_int, mach_int)(&zgsz3109, INT64_C(32));
    lbits zgsz3110;
    CREATE(lbits)(&zgsz3110);
    vector_subrange_lbits(&zgsz3110, zgsz3107, zgsz3108, zgsz3109);
    zgaz314 = CONVERT_OF(fbits, lbits)(zgsz3110, true);
    KILL(lbits)(&zgsz3110);
    KILL(sail_int)(&zgsz3109);
    KILL(sail_int)(&zgsz3108);
    KILL(lbits)(&zgsz3107);
  }
  uint64_t zgaz315;
  {
    lbits zgsz3103;
    CREATE(lbits)(&zgsz3103);
    CONVERT_OF(lbits, fbits)(&zgsz3103, zc, UINT64_C(64) , true);
    sail_int zgsz3104;
    CREATE(sail_int)(&zgsz3104);
    CONVERT_OF(sail_int, mach_int)(&zgsz3104, INT64_C(31));
    sail_int zgsz3105;
    CREATE(sail_int)(&zgsz3105);
    CONVERT_OF(sail_int, mach_int)(&zgsz3105, INT64_C(0));
    lbits zgsz3106;
    CREATE(lbits)(&zgsz3106);
    vector_subrange_lbits(&zgsz3106, zgsz3103, zgsz3104, zgsz3105);
    zgaz315 = CONVERT_OF(fbits, lbits)(zgsz3106, true);
    KILL(lbits)(&zgsz3106);
    KILL(sail_int)(&zgsz3105);
    KILL(sail_int)(&zgsz3104);
    KILL(lbits)(&zgsz3103);
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
  struct zEncCapability zcbz343 = { .zreserved = UINT64_C(0xdeadc0de), .zperms = UINT64_C(0xdeadc0de), .zotype = UINT64_C(0xdeadc0de), .zinternal_E = UINT64_C(0), .zflags = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  return zcbz343;
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
                lbits zgsz3129;
                CREATE(lbits)(&zgsz3129);
                CONVERT_OF(lbits, fbits)(&zgsz3129, zgaz317, UINT64_C(8) , true);
                lbits zgsz3130;
                CREATE(lbits)(&zgsz3130);
                CONVERT_OF(lbits, fbits)(&zgsz3130, zgaz318, UINT64_C(32) , true);
                lbits zgsz3131;
                CREATE(lbits)(&zgsz3131);
                append(&zgsz3131, zgsz3129, zgsz3130);
                zgaz320 = CONVERT_OF(fbits, lbits)(zgsz3131, true);
                KILL(lbits)(&zgsz3131);
                KILL(lbits)(&zgsz3130);
                KILL(lbits)(&zgsz3129);
              }
            }
            {
              lbits zgsz3132;
              CREATE(lbits)(&zgsz3132);
              CONVERT_OF(lbits, fbits)(&zgsz3132, zgaz319, UINT64_C(6) , true);
              lbits zgsz3133;
              CREATE(lbits)(&zgsz3133);
              CONVERT_OF(lbits, fbits)(&zgsz3133, zgaz320, UINT64_C(40) , true);
              lbits zgsz3134;
              CREATE(lbits)(&zgsz3134);
              append(&zgsz3134, zgsz3132, zgsz3133);
              zgaz322 = CONVERT_OF(fbits, lbits)(zgsz3134, true);
              KILL(lbits)(&zgsz3134);
              KILL(lbits)(&zgsz3133);
              KILL(lbits)(&zgsz3132);
            }
          }
          {
            lbits zgsz3135;
            CREATE(lbits)(&zgsz3135);
            CONVERT_OF(lbits, fbits)(&zgsz3135, zgaz321, UINT64_C(1) , true);
            lbits zgsz3136;
            CREATE(lbits)(&zgsz3136);
            CONVERT_OF(lbits, fbits)(&zgsz3136, zgaz322, UINT64_C(46) , true);
            lbits zgsz3137;
            CREATE(lbits)(&zgsz3137);
            append(&zgsz3137, zgsz3135, zgsz3136);
            zgaz324 = CONVERT_OF(fbits, lbits)(zgsz3137, true);
            KILL(lbits)(&zgsz3137);
            KILL(lbits)(&zgsz3136);
            KILL(lbits)(&zgsz3135);
          }
        }
        {
          lbits zgsz3138;
          CREATE(lbits)(&zgsz3138);
          CONVERT_OF(lbits, fbits)(&zgsz3138, zgaz323, UINT64_C(4) , true);
          lbits zgsz3139;
          CREATE(lbits)(&zgsz3139);
          CONVERT_OF(lbits, fbits)(&zgsz3139, zgaz324, UINT64_C(47) , true);
          lbits zgsz3140;
          CREATE(lbits)(&zgsz3140);
          append(&zgsz3140, zgsz3138, zgsz3139);
          zgaz326 = CONVERT_OF(fbits, lbits)(zgsz3140, true);
          KILL(lbits)(&zgsz3140);
          KILL(lbits)(&zgsz3139);
          KILL(lbits)(&zgsz3138);
        }
      }
      {
        lbits zgsz3141;
        CREATE(lbits)(&zgsz3141);
        CONVERT_OF(lbits, fbits)(&zgsz3141, zgaz325, UINT64_C(1) , true);
        lbits zgsz3142;
        CREATE(lbits)(&zgsz3142);
        CONVERT_OF(lbits, fbits)(&zgsz3142, zgaz326, UINT64_C(51) , true);
        lbits zgsz3143;
        CREATE(lbits)(&zgsz3143);
        append(&zgsz3143, zgsz3141, zgsz3142);
        zgaz328 = CONVERT_OF(fbits, lbits)(zgsz3143, true);
        KILL(lbits)(&zgsz3143);
        KILL(lbits)(&zgsz3142);
        KILL(lbits)(&zgsz3141);
      }
    }
    {
      lbits zgsz3144;
      CREATE(lbits)(&zgsz3144);
      CONVERT_OF(lbits, fbits)(&zgsz3144, zgaz327, UINT64_C(0) , true);
      lbits zgsz3145;
      CREATE(lbits)(&zgsz3145);
      CONVERT_OF(lbits, fbits)(&zgsz3145, zgaz328, UINT64_C(52) , true);
      lbits zgsz3146;
      CREATE(lbits)(&zgsz3146);
      append(&zgsz3146, zgsz3144, zgsz3145);
      zgaz330 = CONVERT_OF(fbits, lbits)(zgsz3146, true);
      KILL(lbits)(&zgsz3146);
      KILL(lbits)(&zgsz3145);
      KILL(lbits)(&zgsz3144);
    }
  }
  {
    lbits zgsz3147;
    CREATE(lbits)(&zgsz3147);
    CONVERT_OF(lbits, fbits)(&zgsz3147, zgaz329, UINT64_C(12) , true);
    lbits zgsz3148;
    CREATE(lbits)(&zgsz3148);
    CONVERT_OF(lbits, fbits)(&zgsz3148, zgaz330, UINT64_C(52) , true);
    lbits zgsz3149;
    CREATE(lbits)(&zgsz3149);
    append(&zgsz3149, zgsz3147, zgsz3148);
    zcbz315 = CONVERT_OF(fbits, lbits)(zgsz3149, true);
    KILL(lbits)(&zgsz3149);
    KILL(lbits)(&zgsz3148);
    KILL(lbits)(&zgsz3147);
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
    sail_int zgsz3150;
    CREATE(sail_int)(&zgsz3150);
    CONVERT_OF(sail_int, mach_int)(&zgsz3150, zcap_addr_width);
    sail_int zgsz3151;
    CREATE(sail_int)(&zgsz3151);
    sailgen_MAX(&zgsz3151, zgsz3150);
    zgsz323 = CONVERT_OF(mach_int, sail_int)(zgsz3151);
    KILL(sail_int)(&zgsz3151);
    KILL(sail_int)(&zgsz3150);
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
    sail_int zgsz3155;
    CREATE(sail_int)(&zgsz3155);
    CONVERT_OF(sail_int, mach_int)(&zgsz3155, zcap_otype_width);
    sail_int zgsz3156;
    CREATE(sail_int)(&zgsz3156);
    sailgen_MAX(&zgsz3156, zgsz3155);
    zgaz331 = CONVERT_OF(mach_int, sail_int)(zgsz3156);
    KILL(sail_int)(&zgsz3156);
    KILL(sail_int)(&zgsz3155);
  }
  {
    sail_int zgsz3152;
    CREATE(sail_int)(&zgsz3152);
    CONVERT_OF(sail_int, mach_int)(&zgsz3152, zgaz331);
    sail_int zgsz3153;
    CREATE(sail_int)(&zgsz3153);
    CONVERT_OF(sail_int, mach_int)(&zgsz3153, zreserved_otypes);
    sail_int zgsz3154;
    CREATE(sail_int)(&zgsz3154);
    sub_int(&zgsz3154, zgsz3152, zgsz3153);
    zgsz324 = CONVERT_OF(mach_int, sail_int)(zgsz3154);
    KILL(sail_int)(&zgsz3154);
    KILL(sail_int)(&zgsz3153);
    KILL(sail_int)(&zgsz3152);
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
    sail_int zgsz3157;
    CREATE(sail_int)(&zgsz3157);
    CONVERT_OF(sail_int, mach_int)(&zgsz3157, INT64_C(15));
    sail_int zgsz3158;
    CREATE(sail_int)(&zgsz3158);
    CONVERT_OF(sail_int, mach_int)(&zgsz3158, INT64_C(0));
    sail_int zgsz3159;
    CREATE(sail_int)(&zgsz3159);
    add_int(&zgsz3159, zgsz3157, zgsz3158);
    zgsz326 = CONVERT_OF(mach_int, sail_int)(zgsz3159);
    KILL(sail_int)(&zgsz3159);
    KILL(sail_int)(&zgsz3158);
    KILL(sail_int)(&zgsz3157);
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
    sail_int zgsz3163;
    CREATE(sail_int)(&zgsz3163);
    CONVERT_OF(sail_int, mach_int)(&zgsz3163, zcap_len_width);
    sail_int zgsz3164;
    CREATE(sail_int)(&zgsz3164);
    CONVERT_OF(sail_int, mach_int)(&zgsz3164, zcap_mantissa_width);
    sail_int zgsz3165;
    CREATE(sail_int)(&zgsz3165);
    sub_int(&zgsz3165, zgsz3163, zgsz3164);
    zgaz332 = CONVERT_OF(mach_int, sail_int)(zgsz3165);
    KILL(sail_int)(&zgsz3165);
    KILL(sail_int)(&zgsz3164);
    KILL(sail_int)(&zgsz3163);
  }
  {
    sail_int zgsz3160;
    CREATE(sail_int)(&zgsz3160);
    CONVERT_OF(sail_int, mach_int)(&zgsz3160, zgaz332);
    sail_int zgsz3161;
    CREATE(sail_int)(&zgsz3161);
    CONVERT_OF(sail_int, mach_int)(&zgsz3161, INT64_C(1));
    sail_int zgsz3162;
    CREATE(sail_int)(&zgsz3162);
    add_int(&zgsz3162, zgsz3160, zgsz3161);
    zgsz327 = CONVERT_OF(mach_int, sail_int)(zgsz3162);
    KILL(sail_int)(&zgsz3162);
    KILL(sail_int)(&zgsz3161);
    KILL(sail_int)(&zgsz3160);
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
    sail_int zgsz3166;
    CREATE(sail_int)(&zgsz3166);
    CONVERT_OF(sail_int, mach_int)(&zgsz3166, zcap_E_width);
    sail_int zgsz3167;
    CREATE(sail_int)(&zgsz3167);
    CONVERT_OF(sail_int, mach_int)(&zgsz3167, zcap_max_E);
    lbits zgsz3168;
    CREATE(lbits)(&zgsz3168);
    sailgen_to_bits(&zgsz3168, zgsz3166, zgsz3167);
    zgsz328 = CONVERT_OF(fbits, lbits)(zgsz3168, true);
    KILL(lbits)(&zgsz3168);
    KILL(sail_int)(&zgsz3167);
    KILL(sail_int)(&zgsz3166);
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
      sail_int zgsz3171;
      CREATE(sail_int)(&zgsz3171);
      CONVERT_OF(sail_int, mach_int)(&zgsz3171, zcap_mantissa_width);
      sail_int zgsz3172;
      CREATE(sail_int)(&zgsz3172);
      CONVERT_OF(sail_int, mach_int)(&zgsz3172, INT64_C(2));
      sail_int zgsz3173;
      CREATE(sail_int)(&zgsz3173);
      sub_int(&zgsz3173, zgsz3171, zgsz3172);
      zgaz333 = CONVERT_OF(mach_int, sail_int)(zgsz3173);
      KILL(sail_int)(&zgsz3173);
      KILL(sail_int)(&zgsz3172);
      KILL(sail_int)(&zgsz3171);
    }
    {
      sail_int zgsz3169;
      CREATE(sail_int)(&zgsz3169);
      CONVERT_OF(sail_int, mach_int)(&zgsz3169, zgaz333);
      lbits zgsz3170;
      CREATE(lbits)(&zgsz3170);
      sailgen_zzeros_implicit(&zgsz3170, zgsz3169);
      zgaz334 = CONVERT_OF(fbits, lbits)(zgsz3170, true);
      KILL(lbits)(&zgsz3170);
      KILL(sail_int)(&zgsz3169);
    }
  }
  {
    lbits zgsz3174;
    CREATE(lbits)(&zgsz3174);
    CONVERT_OF(lbits, fbits)(&zgsz3174, UINT64_C(0b01), UINT64_C(2) , true);
    lbits zgsz3175;
    CREATE(lbits)(&zgsz3175);
    CONVERT_OF(lbits, fbits)(&zgsz3175, zgaz334, UINT64_C(6) , true);
    lbits zgsz3176;
    CREATE(lbits)(&zgsz3176);
    append(&zgsz3176, zgsz3174, zgsz3175);
    zgsz329 = CONVERT_OF(fbits, lbits)(zgsz3176, true);
    KILL(lbits)(&zgsz3176);
    KILL(lbits)(&zgsz3175);
    KILL(lbits)(&zgsz3174);
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
    sail_int zgsz3189;
    CREATE(sail_int)(&zgsz3189);
    CONVERT_OF(sail_int, mach_int)(&zgsz3189, INT64_C(0));
    lbits zgsz3190;
    CREATE(lbits)(&zgsz3190);
    UNDEFINED(lbits)(&zgsz3190, zgsz3189);
    zgaz336 = CONVERT_OF(fbits, lbits)(zgsz3190, true);
    KILL(lbits)(&zgsz3190);
    KILL(sail_int)(&zgsz3189);
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
    sail_int zgsz3187;
    CREATE(sail_int)(&zgsz3187);
    CONVERT_OF(sail_int, mach_int)(&zgsz3187, INT64_C(0));
    lbits zgsz3188;
    CREATE(lbits)(&zgsz3188);
    UNDEFINED(lbits)(&zgsz3188, zgsz3187);
    zgaz349 = CONVERT_OF(fbits, lbits)(zgsz3188, true);
    KILL(lbits)(&zgsz3188);
    KILL(sail_int)(&zgsz3187);
  }
  bool zgaz350;
  zgaz350 = undefined_bool(UNIT);
  bool zgaz351;
  zgaz351 = undefined_bool(UNIT);
  uint64_t zgaz352;
  {
    sail_int zgsz3185;
    CREATE(sail_int)(&zgsz3185);
    CONVERT_OF(sail_int, mach_int)(&zgsz3185, INT64_C(6));
    lbits zgsz3186;
    CREATE(lbits)(&zgsz3186);
    UNDEFINED(lbits)(&zgsz3186, zgsz3185);
    zgaz352 = CONVERT_OF(fbits, lbits)(zgsz3186, true);
    KILL(lbits)(&zgsz3186);
    KILL(sail_int)(&zgsz3185);
  }
  uint64_t zgaz353;
  {
    sail_int zgsz3183;
    CREATE(sail_int)(&zgsz3183);
    CONVERT_OF(sail_int, mach_int)(&zgsz3183, INT64_C(8));
    lbits zgsz3184;
    CREATE(lbits)(&zgsz3184);
    UNDEFINED(lbits)(&zgsz3184, zgsz3183);
    zgaz353 = CONVERT_OF(fbits, lbits)(zgsz3184, true);
    KILL(lbits)(&zgsz3184);
    KILL(sail_int)(&zgsz3183);
  }
  uint64_t zgaz354;
  {
    sail_int zgsz3181;
    CREATE(sail_int)(&zgsz3181);
    CONVERT_OF(sail_int, mach_int)(&zgsz3181, INT64_C(8));
    lbits zgsz3182;
    CREATE(lbits)(&zgsz3182);
    UNDEFINED(lbits)(&zgsz3182, zgsz3181);
    zgaz354 = CONVERT_OF(fbits, lbits)(zgsz3182, true);
    KILL(lbits)(&zgsz3182);
    KILL(sail_int)(&zgsz3181);
  }
  uint64_t zgaz355;
  {
    sail_int zgsz3179;
    CREATE(sail_int)(&zgsz3179);
    CONVERT_OF(sail_int, mach_int)(&zgsz3179, INT64_C(4));
    lbits zgsz3180;
    CREATE(lbits)(&zgsz3180);
    UNDEFINED(lbits)(&zgsz3180, zgsz3179);
    zgaz355 = CONVERT_OF(fbits, lbits)(zgsz3180, true);
    KILL(lbits)(&zgsz3180);
    KILL(sail_int)(&zgsz3179);
  }
  uint64_t zgaz356;
  {
    sail_int zgsz3177;
    CREATE(sail_int)(&zgsz3177);
    CONVERT_OF(sail_int, mach_int)(&zgsz3177, INT64_C(32));
    lbits zgsz3178;
    CREATE(lbits)(&zgsz3178);
    UNDEFINED(lbits)(&zgsz3178, zgsz3177);
    zgaz356 = CONVERT_OF(fbits, lbits)(zgsz3178, true);
    KILL(lbits)(&zgsz3178);
    KILL(sail_int)(&zgsz3177);
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
  struct zCapability zcbz344 = { .zuperms = UINT64_C(0xdeadc0de), .ztag = false, .zreserved = UINT64_C(0xdeadc0de), .zpermit_unseal = false, .zpermit_store_local_cap = false, .zpermit_store_cap = false, .zpermit_store = false, .zpermit_set_CID = false, .zpermit_seal = false, .zpermit_load_cap = false, .zpermit_load = false, .zpermit_execute = false, .zpermit_cinvoke = false, .zotype = UINT64_C(0xdeadc0de), .zinternal_E = false, .zglobal = false, .zflag_cap_mode = false, .zaddress = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zT = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  return zcbz344;
}

static struct zCapability znull_cap;
static void create_letbind_25(void) {


  struct zCapability zgsz333;
  uint64_t zgaz357;
  {
    sail_int zgsz3200;
    CREATE(sail_int)(&zgsz3200);
    CONVERT_OF(sail_int, mach_int)(&zgsz3200, INT64_C(0));
    lbits zgsz3201;
    CREATE(lbits)(&zgsz3201);
    sailgen_zzeros_implicit(&zgsz3201, zgsz3200);
    zgaz357 = CONVERT_OF(fbits, lbits)(zgsz3201, true);
    KILL(lbits)(&zgsz3201);
    KILL(sail_int)(&zgsz3200);
  }
  uint64_t zgaz358;
  {
    sail_int zgsz3198;
    CREATE(sail_int)(&zgsz3198);
    CONVERT_OF(sail_int, mach_int)(&zgsz3198, INT64_C(0));
    lbits zgsz3199;
    CREATE(lbits)(&zgsz3199);
    sailgen_zzeros_implicit(&zgsz3199, zgsz3198);
    zgaz358 = CONVERT_OF(fbits, lbits)(zgsz3199, true);
    KILL(lbits)(&zgsz3199);
    KILL(sail_int)(&zgsz3198);
  }
  uint64_t zgaz359;
  {
    sail_int zgsz3196;
    CREATE(sail_int)(&zgsz3196);
    CONVERT_OF(sail_int, mach_int)(&zgsz3196, INT64_C(8));
    lbits zgsz3197;
    CREATE(lbits)(&zgsz3197);
    sailgen_zzeros_implicit(&zgsz3197, zgsz3196);
    zgaz359 = CONVERT_OF(fbits, lbits)(zgsz3197, true);
    KILL(lbits)(&zgsz3197);
    KILL(sail_int)(&zgsz3196);
  }
  uint64_t zgaz360;
  {
    sail_int zgsz3193;
    CREATE(sail_int)(&zgsz3193);
    CONVERT_OF(sail_int, mach_int)(&zgsz3193, zcap_otype_width);
    sail_int zgsz3194;
    CREATE(sail_int)(&zgsz3194);
    CONVERT_OF(sail_int, mach_int)(&zgsz3194, zotype_unsealed);
    lbits zgsz3195;
    CREATE(lbits)(&zgsz3195);
    sailgen_to_bits(&zgsz3195, zgsz3193, zgsz3194);
    zgaz360 = CONVERT_OF(fbits, lbits)(zgsz3195, true);
    KILL(lbits)(&zgsz3195);
    KILL(sail_int)(&zgsz3194);
    KILL(sail_int)(&zgsz3193);
  }
  uint64_t zgaz361;
  {
    sail_int zgsz3191;
    CREATE(sail_int)(&zgsz3191);
    CONVERT_OF(sail_int, mach_int)(&zgsz3191, INT64_C(32));
    lbits zgsz3192;
    CREATE(lbits)(&zgsz3192);
    sailgen_zzeros_implicit(&zgsz3192, zgsz3191);
    zgaz361 = CONVERT_OF(fbits, lbits)(zgsz3192, true);
    KILL(lbits)(&zgsz3192);
    KILL(sail_int)(&zgsz3191);
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
    sail_int zgsz3211;
    CREATE(sail_int)(&zgsz3211);
    CONVERT_OF(sail_int, mach_int)(&zgsz3211, INT64_C(0));
    lbits zgsz3212;
    CREATE(lbits)(&zgsz3212);
    sailgen_ones(&zgsz3212, zgsz3211);
    zgaz362 = CONVERT_OF(fbits, lbits)(zgsz3212, true);
    KILL(lbits)(&zgsz3212);
    KILL(sail_int)(&zgsz3211);
  }
  uint64_t zgaz363;
  {
    sail_int zgsz3209;
    CREATE(sail_int)(&zgsz3209);
    CONVERT_OF(sail_int, mach_int)(&zgsz3209, INT64_C(0));
    lbits zgsz3210;
    CREATE(lbits)(&zgsz3210);
    sailgen_zzeros_implicit(&zgsz3210, zgsz3209);
    zgaz363 = CONVERT_OF(fbits, lbits)(zgsz3210, true);
    KILL(lbits)(&zgsz3210);
    KILL(sail_int)(&zgsz3209);
  }
  uint64_t zgaz364;
  {
    sail_int zgsz3207;
    CREATE(sail_int)(&zgsz3207);
    CONVERT_OF(sail_int, mach_int)(&zgsz3207, INT64_C(8));
    lbits zgsz3208;
    CREATE(lbits)(&zgsz3208);
    sailgen_zzeros_implicit(&zgsz3208, zgsz3207);
    zgaz364 = CONVERT_OF(fbits, lbits)(zgsz3208, true);
    KILL(lbits)(&zgsz3208);
    KILL(sail_int)(&zgsz3207);
  }
  uint64_t zgaz365;
  {
    sail_int zgsz3204;
    CREATE(sail_int)(&zgsz3204);
    CONVERT_OF(sail_int, mach_int)(&zgsz3204, zcap_otype_width);
    sail_int zgsz3205;
    CREATE(sail_int)(&zgsz3205);
    CONVERT_OF(sail_int, mach_int)(&zgsz3205, zotype_unsealed);
    lbits zgsz3206;
    CREATE(lbits)(&zgsz3206);
    sailgen_to_bits(&zgsz3206, zgsz3204, zgsz3205);
    zgaz365 = CONVERT_OF(fbits, lbits)(zgsz3206, true);
    KILL(lbits)(&zgsz3206);
    KILL(sail_int)(&zgsz3205);
    KILL(sail_int)(&zgsz3204);
  }
  uint64_t zgaz366;
  {
    sail_int zgsz3202;
    CREATE(sail_int)(&zgsz3202);
    CONVERT_OF(sail_int, mach_int)(&zgsz3202, INT64_C(32));
    lbits zgsz3203;
    CREATE(lbits)(&zgsz3203);
    sailgen_zzeros_implicit(&zgsz3203, zgsz3202);
    zgaz366 = CONVERT_OF(fbits, lbits)(zgsz3203, true);
    KILL(lbits)(&zgsz3203);
    KILL(sail_int)(&zgsz3202);
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
                        lbits zgsz3213;
                        CREATE(lbits)(&zgsz3213);
                        CONVERT_OF(lbits, fbits)(&zgsz3213, zgaz379, UINT64_C(1) , true);
                        lbits zgsz3214;
                        CREATE(lbits)(&zgsz3214);
                        CONVERT_OF(lbits, fbits)(&zgsz3214, zgaz380, UINT64_C(1) , true);
                        lbits zgsz3215;
                        CREATE(lbits)(&zgsz3215);
                        append(&zgsz3215, zgsz3213, zgsz3214);
                        zgaz382 = CONVERT_OF(fbits, lbits)(zgsz3215, true);
                        KILL(lbits)(&zgsz3215);
                        KILL(lbits)(&zgsz3214);
                        KILL(lbits)(&zgsz3213);
                      }
                    }
                    {
                      lbits zgsz3216;
                      CREATE(lbits)(&zgsz3216);
                      CONVERT_OF(lbits, fbits)(&zgsz3216, zgaz381, UINT64_C(1) , true);
                      lbits zgsz3217;
                      CREATE(lbits)(&zgsz3217);
                      CONVERT_OF(lbits, fbits)(&zgsz3217, zgaz382, UINT64_C(2) , true);
                      lbits zgsz3218;
                      CREATE(lbits)(&zgsz3218);
                      append(&zgsz3218, zgsz3216, zgsz3217);
                      zgaz384 = CONVERT_OF(fbits, lbits)(zgsz3218, true);
                      KILL(lbits)(&zgsz3218);
                      KILL(lbits)(&zgsz3217);
                      KILL(lbits)(&zgsz3216);
                    }
                  }
                  {
                    lbits zgsz3219;
                    CREATE(lbits)(&zgsz3219);
                    CONVERT_OF(lbits, fbits)(&zgsz3219, zgaz383, UINT64_C(1) , true);
                    lbits zgsz3220;
                    CREATE(lbits)(&zgsz3220);
                    CONVERT_OF(lbits, fbits)(&zgsz3220, zgaz384, UINT64_C(3) , true);
                    lbits zgsz3221;
                    CREATE(lbits)(&zgsz3221);
                    append(&zgsz3221, zgsz3219, zgsz3220);
                    zgaz386 = CONVERT_OF(fbits, lbits)(zgsz3221, true);
                    KILL(lbits)(&zgsz3221);
                    KILL(lbits)(&zgsz3220);
                    KILL(lbits)(&zgsz3219);
                  }
                }
                {
                  lbits zgsz3222;
                  CREATE(lbits)(&zgsz3222);
                  CONVERT_OF(lbits, fbits)(&zgsz3222, zgaz385, UINT64_C(1) , true);
                  lbits zgsz3223;
                  CREATE(lbits)(&zgsz3223);
                  CONVERT_OF(lbits, fbits)(&zgsz3223, zgaz386, UINT64_C(4) , true);
                  lbits zgsz3224;
                  CREATE(lbits)(&zgsz3224);
                  append(&zgsz3224, zgsz3222, zgsz3223);
                  zgaz388 = CONVERT_OF(fbits, lbits)(zgsz3224, true);
                  KILL(lbits)(&zgsz3224);
                  KILL(lbits)(&zgsz3223);
                  KILL(lbits)(&zgsz3222);
                }
              }
              {
                lbits zgsz3225;
                CREATE(lbits)(&zgsz3225);
                CONVERT_OF(lbits, fbits)(&zgsz3225, zgaz387, UINT64_C(1) , true);
                lbits zgsz3226;
                CREATE(lbits)(&zgsz3226);
                CONVERT_OF(lbits, fbits)(&zgsz3226, zgaz388, UINT64_C(5) , true);
                lbits zgsz3227;
                CREATE(lbits)(&zgsz3227);
                append(&zgsz3227, zgsz3225, zgsz3226);
                zgaz390 = CONVERT_OF(fbits, lbits)(zgsz3227, true);
                KILL(lbits)(&zgsz3227);
                KILL(lbits)(&zgsz3226);
                KILL(lbits)(&zgsz3225);
              }
            }
            {
              lbits zgsz3228;
              CREATE(lbits)(&zgsz3228);
              CONVERT_OF(lbits, fbits)(&zgsz3228, zgaz389, UINT64_C(1) , true);
              lbits zgsz3229;
              CREATE(lbits)(&zgsz3229);
              CONVERT_OF(lbits, fbits)(&zgsz3229, zgaz390, UINT64_C(6) , true);
              lbits zgsz3230;
              CREATE(lbits)(&zgsz3230);
              append(&zgsz3230, zgsz3228, zgsz3229);
              zgaz392 = CONVERT_OF(fbits, lbits)(zgsz3230, true);
              KILL(lbits)(&zgsz3230);
              KILL(lbits)(&zgsz3229);
              KILL(lbits)(&zgsz3228);
            }
          }
          {
            lbits zgsz3231;
            CREATE(lbits)(&zgsz3231);
            CONVERT_OF(lbits, fbits)(&zgsz3231, zgaz391, UINT64_C(1) , true);
            lbits zgsz3232;
            CREATE(lbits)(&zgsz3232);
            CONVERT_OF(lbits, fbits)(&zgsz3232, zgaz392, UINT64_C(7) , true);
            lbits zgsz3233;
            CREATE(lbits)(&zgsz3233);
            append(&zgsz3233, zgsz3231, zgsz3232);
            zgaz394 = CONVERT_OF(fbits, lbits)(zgsz3233, true);
            KILL(lbits)(&zgsz3233);
            KILL(lbits)(&zgsz3232);
            KILL(lbits)(&zgsz3231);
          }
        }
        {
          lbits zgsz3234;
          CREATE(lbits)(&zgsz3234);
          CONVERT_OF(lbits, fbits)(&zgsz3234, zgaz393, UINT64_C(1) , true);
          lbits zgsz3235;
          CREATE(lbits)(&zgsz3235);
          CONVERT_OF(lbits, fbits)(&zgsz3235, zgaz394, UINT64_C(8) , true);
          lbits zgsz3236;
          CREATE(lbits)(&zgsz3236);
          append(&zgsz3236, zgsz3234, zgsz3235);
          zgaz396 = CONVERT_OF(fbits, lbits)(zgsz3236, true);
          KILL(lbits)(&zgsz3236);
          KILL(lbits)(&zgsz3235);
          KILL(lbits)(&zgsz3234);
        }
      }
      {
        lbits zgsz3237;
        CREATE(lbits)(&zgsz3237);
        CONVERT_OF(lbits, fbits)(&zgsz3237, zgaz395, UINT64_C(1) , true);
        lbits zgsz3238;
        CREATE(lbits)(&zgsz3238);
        CONVERT_OF(lbits, fbits)(&zgsz3238, zgaz396, UINT64_C(9) , true);
        lbits zgsz3239;
        CREATE(lbits)(&zgsz3239);
        append(&zgsz3239, zgsz3237, zgsz3238);
        zgaz398 = CONVERT_OF(fbits, lbits)(zgsz3239, true);
        KILL(lbits)(&zgsz3239);
        KILL(lbits)(&zgsz3238);
        KILL(lbits)(&zgsz3237);
      }
    }
    {
      lbits zgsz3240;
      CREATE(lbits)(&zgsz3240);
      CONVERT_OF(lbits, fbits)(&zgsz3240, zgaz397, UINT64_C(1) , true);
      lbits zgsz3241;
      CREATE(lbits)(&zgsz3241);
      CONVERT_OF(lbits, fbits)(&zgsz3241, zgaz398, UINT64_C(10) , true);
      lbits zgsz3242;
      CREATE(lbits)(&zgsz3242);
      append(&zgsz3242, zgsz3240, zgsz3241);
      zgaz3100 = CONVERT_OF(fbits, lbits)(zgsz3242, true);
      KILL(lbits)(&zgsz3242);
      KILL(lbits)(&zgsz3241);
      KILL(lbits)(&zgsz3240);
    }
  }
  {
    lbits zgsz3243;
    CREATE(lbits)(&zgsz3243);
    CONVERT_OF(lbits, fbits)(&zgsz3243, zgaz399, UINT64_C(1) , true);
    lbits zgsz3244;
    CREATE(lbits)(&zgsz3244);
    CONVERT_OF(lbits, fbits)(&zgsz3244, zgaz3100, UINT64_C(11) , true);
    lbits zgsz3245;
    CREATE(lbits)(&zgsz3245);
    append(&zgsz3245, zgsz3243, zgsz3244);
    zcbz317 = CONVERT_OF(fbits, lbits)(zgsz3245, true);
    KILL(lbits)(&zgsz3245);
    KILL(lbits)(&zgsz3244);
    KILL(lbits)(&zgsz3243);
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
    sail_int zgsz3352;
    CREATE(sail_int)(&zgsz3352);
    CONVERT_OF(sail_int, mach_int)(&zgsz3352, INT64_C(6));
    lbits zgsz3353;
    CREATE(lbits)(&zgsz3353);
    sailgen_zzeros_implicit(&zgsz3353, zgsz3352);
    zE = CONVERT_OF(fbits, lbits)(zgsz3353, true);
    KILL(lbits)(&zgsz3353);
    KILL(sail_int)(&zgsz3352);
  }
  uint64_t zBs;
  {
    sail_int zgsz3350;
    CREATE(sail_int)(&zgsz3350);
    CONVERT_OF(sail_int, mach_int)(&zgsz3350, INT64_C(8));
    lbits zgsz3351;
    CREATE(lbits)(&zgsz3351);
    sailgen_zzeros_implicit(&zgsz3351, zgsz3350);
    zBs = CONVERT_OF(fbits, lbits)(zgsz3351, true);
    KILL(lbits)(&zgsz3351);
    KILL(sail_int)(&zgsz3350);
  }
  uint64_t zT;
  {
    int64_t zgaz3171;
    {
      sail_int zgsz3248;
      CREATE(sail_int)(&zgsz3248);
      CONVERT_OF(sail_int, mach_int)(&zgsz3248, INT64_C(8));
      sail_int zgsz3249;
      CREATE(sail_int)(&zgsz3249);
      CONVERT_OF(sail_int, mach_int)(&zgsz3249, INT64_C(2));
      sail_int zgsz3250;
      CREATE(sail_int)(&zgsz3250);
      sub_int(&zgsz3250, zgsz3248, zgsz3249);
      zgaz3171 = CONVERT_OF(mach_int, sail_int)(zgsz3250);
      KILL(sail_int)(&zgsz3250);
      KILL(sail_int)(&zgsz3249);
      KILL(sail_int)(&zgsz3248);
    }
    {
      sail_int zgsz3246;
      CREATE(sail_int)(&zgsz3246);
      CONVERT_OF(sail_int, mach_int)(&zgsz3246, zgaz3171);
      lbits zgsz3247;
      CREATE(lbits)(&zgsz3247);
      sailgen_zzeros_implicit(&zgsz3247, zgsz3246);
      zT = CONVERT_OF(fbits, lbits)(zgsz3247, true);
      KILL(lbits)(&zgsz3247);
      KILL(sail_int)(&zgsz3246);
    }
  }
  uint64_t zlenMSBs;
  {
    sail_int zgsz3348;
    CREATE(sail_int)(&zgsz3348);
    CONVERT_OF(sail_int, mach_int)(&zgsz3348, INT64_C(2));
    lbits zgsz3349;
    CREATE(lbits)(&zgsz3349);
    sailgen_zzeros_implicit(&zgsz3349, zgsz3348);
    zlenMSBs = CONVERT_OF(fbits, lbits)(zgsz3349, true);
    KILL(lbits)(&zgsz3349);
    KILL(sail_int)(&zgsz3348);
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
            sail_int zgsz3255;
            CREATE(sail_int)(&zgsz3255);
            CONVERT_OF(sail_int, mach_int)(&zgsz3255, zinternal_E_take_bits);
            sail_int zgsz3256;
            CREATE(sail_int)(&zgsz3256);
            CONVERT_OF(sail_int, mach_int)(&zgsz3256, INT64_C(1));
            sail_int zgsz3257;
            CREATE(sail_int)(&zgsz3257);
            sub_int(&zgsz3257, zgsz3255, zgsz3256);
            zgaz3102 = CONVERT_OF(mach_int, sail_int)(zgsz3257);
            KILL(sail_int)(&zgsz3257);
            KILL(sail_int)(&zgsz3256);
            KILL(sail_int)(&zgsz3255);
          }
          {
            lbits zgsz3251;
            CREATE(lbits)(&zgsz3251);
            CONVERT_OF(lbits, fbits)(&zgsz3251, zgaz3101, UINT64_C(6) , true);
            sail_int zgsz3252;
            CREATE(sail_int)(&zgsz3252);
            CONVERT_OF(sail_int, mach_int)(&zgsz3252, zgaz3102);
            sail_int zgsz3253;
            CREATE(sail_int)(&zgsz3253);
            CONVERT_OF(sail_int, mach_int)(&zgsz3253, INT64_C(0));
            lbits zgsz3254;
            CREATE(lbits)(&zgsz3254);
            vector_subrange_lbits(&zgsz3254, zgsz3251, zgsz3252, zgsz3253);
            zgaz3105 = CONVERT_OF(fbits, lbits)(zgsz3254, true);
            KILL(lbits)(&zgsz3254);
            KILL(sail_int)(&zgsz3253);
            KILL(sail_int)(&zgsz3252);
            KILL(lbits)(&zgsz3251);
          }
        }
        uint64_t zgaz3106;
        {
          uint64_t zgaz3103;
          zgaz3103 = zc.zB;
          int64_t zgaz3104;
          {
            sail_int zgsz3262;
            CREATE(sail_int)(&zgsz3262);
            CONVERT_OF(sail_int, mach_int)(&zgsz3262, zinternal_E_take_bits);
            sail_int zgsz3263;
            CREATE(sail_int)(&zgsz3263);
            CONVERT_OF(sail_int, mach_int)(&zgsz3263, INT64_C(1));
            sail_int zgsz3264;
            CREATE(sail_int)(&zgsz3264);
            sub_int(&zgsz3264, zgsz3262, zgsz3263);
            zgaz3104 = CONVERT_OF(mach_int, sail_int)(zgsz3264);
            KILL(sail_int)(&zgsz3264);
            KILL(sail_int)(&zgsz3263);
            KILL(sail_int)(&zgsz3262);
          }
          {
            lbits zgsz3258;
            CREATE(lbits)(&zgsz3258);
            CONVERT_OF(lbits, fbits)(&zgsz3258, zgaz3103, UINT64_C(8) , true);
            sail_int zgsz3259;
            CREATE(sail_int)(&zgsz3259);
            CONVERT_OF(sail_int, mach_int)(&zgsz3259, zgaz3104);
            sail_int zgsz3260;
            CREATE(sail_int)(&zgsz3260);
            CONVERT_OF(sail_int, mach_int)(&zgsz3260, INT64_C(0));
            lbits zgsz3261;
            CREATE(lbits)(&zgsz3261);
            vector_subrange_lbits(&zgsz3261, zgsz3258, zgsz3259, zgsz3260);
            zgaz3106 = CONVERT_OF(fbits, lbits)(zgsz3261, true);
            KILL(lbits)(&zgsz3261);
            KILL(sail_int)(&zgsz3260);
            KILL(sail_int)(&zgsz3259);
            KILL(lbits)(&zgsz3258);
          }
        }
        {
          lbits zgsz3265;
          CREATE(lbits)(&zgsz3265);
          CONVERT_OF(lbits, fbits)(&zgsz3265, zgaz3105, UINT64_C(3) , true);
          lbits zgsz3266;
          CREATE(lbits)(&zgsz3266);
          CONVERT_OF(lbits, fbits)(&zgsz3266, zgaz3106, UINT64_C(3) , true);
          lbits zgsz3267;
          CREATE(lbits)(&zgsz3267);
          append(&zgsz3267, zgsz3265, zgsz3266);
          zE = CONVERT_OF(fbits, lbits)(zgsz3267, true);
          KILL(lbits)(&zgsz3267);
          KILL(lbits)(&zgsz3266);
          KILL(lbits)(&zgsz3265);
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
            sail_int zgsz3272;
            CREATE(sail_int)(&zgsz3272);
            CONVERT_OF(sail_int, mach_int)(&zgsz3272, zcap_mantissa_width);
            sail_int zgsz3273;
            CREATE(sail_int)(&zgsz3273);
            CONVERT_OF(sail_int, mach_int)(&zgsz3273, INT64_C(3));
            sail_int zgsz3274;
            CREATE(sail_int)(&zgsz3274);
            sub_int(&zgsz3274, zgsz3272, zgsz3273);
            zgaz3108 = CONVERT_OF(mach_int, sail_int)(zgsz3274);
            KILL(sail_int)(&zgsz3274);
            KILL(sail_int)(&zgsz3273);
            KILL(sail_int)(&zgsz3272);
          }
          {
            lbits zgsz3268;
            CREATE(lbits)(&zgsz3268);
            CONVERT_OF(lbits, fbits)(&zgsz3268, zgaz3107, UINT64_C(6) , true);
            sail_int zgsz3269;
            CREATE(sail_int)(&zgsz3269);
            CONVERT_OF(sail_int, mach_int)(&zgsz3269, zgaz3108);
            sail_int zgsz3270;
            CREATE(sail_int)(&zgsz3270);
            CONVERT_OF(sail_int, mach_int)(&zgsz3270, INT64_C(3));
            lbits zgsz3271;
            CREATE(lbits)(&zgsz3271);
            vector_subrange_lbits(&zgsz3271, zgsz3268, zgsz3269, zgsz3270);
            zgaz3109 = CONVERT_OF(fbits, lbits)(zgsz3271, true);
            KILL(lbits)(&zgsz3271);
            KILL(sail_int)(&zgsz3270);
            KILL(sail_int)(&zgsz3269);
            KILL(lbits)(&zgsz3268);
          }
        }
        uint64_t zgaz3110;
        {
          sail_int zgsz3278;
          CREATE(sail_int)(&zgsz3278);
          CONVERT_OF(sail_int, mach_int)(&zgsz3278, zinternal_E_take_bits);
          lbits zgsz3279;
          CREATE(lbits)(&zgsz3279);
          sailgen_zzeros_implicit(&zgsz3279, zgsz3278);
          zgaz3110 = CONVERT_OF(fbits, lbits)(zgsz3279, true);
          KILL(lbits)(&zgsz3279);
          KILL(sail_int)(&zgsz3278);
        }
        {
          lbits zgsz3275;
          CREATE(lbits)(&zgsz3275);
          CONVERT_OF(lbits, fbits)(&zgsz3275, zgaz3109, UINT64_C(3) , true);
          lbits zgsz3276;
          CREATE(lbits)(&zgsz3276);
          CONVERT_OF(lbits, fbits)(&zgsz3276, zgaz3110, UINT64_C(3) , true);
          lbits zgsz3277;
          CREATE(lbits)(&zgsz3277);
          append(&zgsz3277, zgsz3275, zgsz3276);
          zT = CONVERT_OF(fbits, lbits)(zgsz3277, true);
          KILL(lbits)(&zgsz3277);
          KILL(lbits)(&zgsz3276);
          KILL(lbits)(&zgsz3275);
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
          sail_int zgsz3284;
          CREATE(sail_int)(&zgsz3284);
          CONVERT_OF(sail_int, mach_int)(&zgsz3284, zcap_mantissa_width);
          sail_int zgsz3285;
          CREATE(sail_int)(&zgsz3285);
          CONVERT_OF(sail_int, mach_int)(&zgsz3285, INT64_C(1));
          sail_int zgsz3286;
          CREATE(sail_int)(&zgsz3286);
          sub_int(&zgsz3286, zgsz3284, zgsz3285);
          zgaz3112 = CONVERT_OF(mach_int, sail_int)(zgsz3286);
          KILL(sail_int)(&zgsz3286);
          KILL(sail_int)(&zgsz3285);
          KILL(sail_int)(&zgsz3284);
        }
        {
          lbits zgsz3280;
          CREATE(lbits)(&zgsz3280);
          CONVERT_OF(lbits, fbits)(&zgsz3280, zgaz3111, UINT64_C(8) , true);
          sail_int zgsz3281;
          CREATE(sail_int)(&zgsz3281);
          CONVERT_OF(sail_int, mach_int)(&zgsz3281, zgaz3112);
          sail_int zgsz3282;
          CREATE(sail_int)(&zgsz3282);
          CONVERT_OF(sail_int, mach_int)(&zgsz3282, INT64_C(3));
          lbits zgsz3283;
          CREATE(lbits)(&zgsz3283);
          vector_subrange_lbits(&zgsz3283, zgsz3280, zgsz3281, zgsz3282);
          zgaz3113 = CONVERT_OF(fbits, lbits)(zgsz3283, true);
          KILL(lbits)(&zgsz3283);
          KILL(sail_int)(&zgsz3282);
          KILL(sail_int)(&zgsz3281);
          KILL(lbits)(&zgsz3280);
        }
      }
      uint64_t zgaz3114;
      {
        sail_int zgsz3290;
        CREATE(sail_int)(&zgsz3290);
        CONVERT_OF(sail_int, mach_int)(&zgsz3290, zinternal_E_take_bits);
        lbits zgsz3291;
        CREATE(lbits)(&zgsz3291);
        sailgen_zzeros_implicit(&zgsz3291, zgsz3290);
        zgaz3114 = CONVERT_OF(fbits, lbits)(zgsz3291, true);
        KILL(lbits)(&zgsz3291);
        KILL(sail_int)(&zgsz3290);
      }
      {
        lbits zgsz3287;
        CREATE(lbits)(&zgsz3287);
        CONVERT_OF(lbits, fbits)(&zgsz3287, zgaz3113, UINT64_C(5) , true);
        lbits zgsz3288;
        CREATE(lbits)(&zgsz3288);
        CONVERT_OF(lbits, fbits)(&zgsz3288, zgaz3114, UINT64_C(3) , true);
        lbits zgsz3289;
        CREATE(lbits)(&zgsz3289);
        append(&zgsz3289, zgsz3287, zgsz3288);
        zBs = CONVERT_OF(fbits, lbits)(zgsz3289, true);
        KILL(lbits)(&zgsz3289);
        KILL(lbits)(&zgsz3288);
        KILL(lbits)(&zgsz3287);
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
          sail_int zgsz3296;
          CREATE(sail_int)(&zgsz3296);
          CONVERT_OF(sail_int, mach_int)(&zgsz3296, zcap_mantissa_width);
          sail_int zgsz3297;
          CREATE(sail_int)(&zgsz3297);
          CONVERT_OF(sail_int, mach_int)(&zgsz3297, INT64_C(3));
          sail_int zgsz3298;
          CREATE(sail_int)(&zgsz3298);
          sub_int(&zgsz3298, zgsz3296, zgsz3297);
          zgaz3168 = CONVERT_OF(mach_int, sail_int)(zgsz3298);
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
          CONVERT_OF(sail_int, mach_int)(&zgsz3293, zgaz3168);
          sail_int zgsz3294;
          CREATE(sail_int)(&zgsz3294);
          CONVERT_OF(sail_int, mach_int)(&zgsz3294, INT64_C(0));
          lbits zgsz3295;
          CREATE(lbits)(&zgsz3295);
          vector_subrange_lbits(&zgsz3295, zgsz3292, zgsz3293, zgsz3294);
          zgaz3169 = CONVERT_OF(fbits, lbits)(zgsz3295, true);
          KILL(lbits)(&zgsz3295);
          KILL(sail_int)(&zgsz3294);
          KILL(sail_int)(&zgsz3293);
          KILL(lbits)(&zgsz3292);
        }
      }
      {
        lbits zgsz3299;
        CREATE(lbits)(&zgsz3299);
        CONVERT_OF(lbits, fbits)(&zgsz3299, zT, UINT64_C(6) , true);
        lbits zgsz3300;
        CREATE(lbits)(&zgsz3300);
        CONVERT_OF(lbits, fbits)(&zgsz3300, zgaz3169, UINT64_C(6) , true);
        zgaz3170 = sailgen_z8operatorz0zI_uz9(zgsz3299, zgsz3300);
        KILL(lbits)(&zgsz3300);
        KILL(lbits)(&zgsz3299);
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
          sail_int zgsz3308;
          CREATE(sail_int)(&zgsz3308);
          CONVERT_OF(sail_int, mach_int)(&zgsz3308, zcap_mantissa_width);
          sail_int zgsz3309;
          CREATE(sail_int)(&zgsz3309);
          CONVERT_OF(sail_int, mach_int)(&zgsz3309, INT64_C(1));
          sail_int zgsz3310;
          CREATE(sail_int)(&zgsz3310);
          sub_int(&zgsz3310, zgsz3308, zgsz3309);
          zgaz3164 = CONVERT_OF(mach_int, sail_int)(zgsz3310);
          KILL(sail_int)(&zgsz3310);
          KILL(sail_int)(&zgsz3309);
          KILL(sail_int)(&zgsz3308);
        }
        int64_t zgaz3165;
        {
          sail_int zgsz3305;
          CREATE(sail_int)(&zgsz3305);
          CONVERT_OF(sail_int, mach_int)(&zgsz3305, zcap_mantissa_width);
          sail_int zgsz3306;
          CREATE(sail_int)(&zgsz3306);
          CONVERT_OF(sail_int, mach_int)(&zgsz3306, INT64_C(2));
          sail_int zgsz3307;
          CREATE(sail_int)(&zgsz3307);
          sub_int(&zgsz3307, zgsz3305, zgsz3306);
          zgaz3165 = CONVERT_OF(mach_int, sail_int)(zgsz3307);
          KILL(sail_int)(&zgsz3307);
          KILL(sail_int)(&zgsz3306);
          KILL(sail_int)(&zgsz3305);
        }
        {
          lbits zgsz3301;
          CREATE(lbits)(&zgsz3301);
          CONVERT_OF(lbits, fbits)(&zgsz3301, zBs, UINT64_C(8) , true);
          sail_int zgsz3302;
          CREATE(sail_int)(&zgsz3302);
          CONVERT_OF(sail_int, mach_int)(&zgsz3302, zgaz3164);
          sail_int zgsz3303;
          CREATE(sail_int)(&zgsz3303);
          CONVERT_OF(sail_int, mach_int)(&zgsz3303, zgaz3165);
          lbits zgsz3304;
          CREATE(lbits)(&zgsz3304);
          vector_subrange_lbits(&zgsz3304, zgsz3301, zgsz3302, zgsz3303);
          zgaz3166 = CONVERT_OF(fbits, lbits)(zgsz3304, true);
          KILL(lbits)(&zgsz3304);
          KILL(sail_int)(&zgsz3303);
          KILL(sail_int)(&zgsz3302);
          KILL(lbits)(&zgsz3301);
        }
      }
      {
        lbits zgsz3311;
        CREATE(lbits)(&zgsz3311);
        CONVERT_OF(lbits, fbits)(&zgsz3311, zgaz3166, UINT64_C(2) , true);
        lbits zgsz3312;
        CREATE(lbits)(&zgsz3312);
        CONVERT_OF(lbits, fbits)(&zgsz3312, zlenMSBs, UINT64_C(2) , true);
        lbits zgsz3313;
        CREATE(lbits)(&zgsz3313);
        add_bits(&zgsz3313, zgsz3311, zgsz3312);
        zgaz3167 = CONVERT_OF(fbits, lbits)(zgsz3313, true);
        KILL(lbits)(&zgsz3313);
        KILL(lbits)(&zgsz3312);
        KILL(lbits)(&zgsz3311);
      }
    }
    {
      lbits zgsz3314;
      CREATE(lbits)(&zgsz3314);
      CONVERT_OF(lbits, fbits)(&zgsz3314, zgaz3167, UINT64_C(2) , true);
      lbits zgsz3315;
      CREATE(lbits)(&zgsz3315);
      CONVERT_OF(lbits, fbits)(&zgsz3315, zcarry_out, UINT64_C(2) , true);
      lbits zgsz3316;
      CREATE(lbits)(&zgsz3316);
      add_bits(&zgsz3316, zgsz3314, zgsz3315);
      zTtop2 = CONVERT_OF(fbits, lbits)(zgsz3316, true);
      KILL(lbits)(&zgsz3316);
      KILL(lbits)(&zgsz3315);
      KILL(lbits)(&zgsz3314);
    }
  }
  struct zCapability zgaz3163;
  {
    uint64_t zgaz3119;
    {
      bool zgaz3115;
      {
        sail_int zgsz3317;
        CREATE(sail_int)(&zgsz3317);
        CONVERT_OF(sail_int, mach_int)(&zgsz3317, zcap_uperms_width);
        sail_int zgsz3318;
        CREATE(sail_int)(&zgsz3318);
        CONVERT_OF(sail_int, mach_int)(&zgsz3318, INT64_C(0));
        zgaz3115 = gt(zgsz3317, zgsz3318);
        KILL(sail_int)(&zgsz3318);
        KILL(sail_int)(&zgsz3317);
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
          lbits zgsz3319;
          CREATE(lbits)(&zgsz3319);
          CONVERT_OF(lbits, fbits)(&zgsz3319, zgaz3120, UINT64_C(12) , true);
          sail_int zgsz3320;
          CREATE(sail_int)(&zgsz3320);
          CONVERT_OF(sail_int, mach_int)(&zgsz3320, INT64_C(11));
          zgaz3121 = bitvector_access(zgsz3319, zgsz3320);
          KILL(sail_int)(&zgsz3320);
          KILL(lbits)(&zgsz3319);
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
          lbits zgsz3321;
          CREATE(lbits)(&zgsz3321);
          CONVERT_OF(lbits, fbits)(&zgsz3321, zgaz3123, UINT64_C(12) , true);
          sail_int zgsz3322;
          CREATE(sail_int)(&zgsz3322);
          CONVERT_OF(sail_int, mach_int)(&zgsz3322, INT64_C(10));
          zgaz3124 = bitvector_access(zgsz3321, zgsz3322);
          KILL(sail_int)(&zgsz3322);
          KILL(lbits)(&zgsz3321);
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
          lbits zgsz3323;
          CREATE(lbits)(&zgsz3323);
          CONVERT_OF(lbits, fbits)(&zgsz3323, zgaz3126, UINT64_C(12) , true);
          sail_int zgsz3324;
          CREATE(sail_int)(&zgsz3324);
          CONVERT_OF(sail_int, mach_int)(&zgsz3324, INT64_C(9));
          zgaz3127 = bitvector_access(zgsz3323, zgsz3324);
          KILL(sail_int)(&zgsz3324);
          KILL(lbits)(&zgsz3323);
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
          lbits zgsz3325;
          CREATE(lbits)(&zgsz3325);
          CONVERT_OF(lbits, fbits)(&zgsz3325, zgaz3129, UINT64_C(12) , true);
          sail_int zgsz3326;
          CREATE(sail_int)(&zgsz3326);
          CONVERT_OF(sail_int, mach_int)(&zgsz3326, INT64_C(8));
          zgaz3130 = bitvector_access(zgsz3325, zgsz3326);
          KILL(sail_int)(&zgsz3326);
          KILL(lbits)(&zgsz3325);
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
          lbits zgsz3327;
          CREATE(lbits)(&zgsz3327);
          CONVERT_OF(lbits, fbits)(&zgsz3327, zgaz3132, UINT64_C(12) , true);
          sail_int zgsz3328;
          CREATE(sail_int)(&zgsz3328);
          CONVERT_OF(sail_int, mach_int)(&zgsz3328, INT64_C(7));
          zgaz3133 = bitvector_access(zgsz3327, zgsz3328);
          KILL(sail_int)(&zgsz3328);
          KILL(lbits)(&zgsz3327);
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
          lbits zgsz3329;
          CREATE(lbits)(&zgsz3329);
          CONVERT_OF(lbits, fbits)(&zgsz3329, zgaz3135, UINT64_C(12) , true);
          sail_int zgsz3330;
          CREATE(sail_int)(&zgsz3330);
          CONVERT_OF(sail_int, mach_int)(&zgsz3330, INT64_C(6));
          zgaz3136 = bitvector_access(zgsz3329, zgsz3330);
          KILL(sail_int)(&zgsz3330);
          KILL(lbits)(&zgsz3329);
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
          lbits zgsz3331;
          CREATE(lbits)(&zgsz3331);
          CONVERT_OF(lbits, fbits)(&zgsz3331, zgaz3138, UINT64_C(12) , true);
          sail_int zgsz3332;
          CREATE(sail_int)(&zgsz3332);
          CONVERT_OF(sail_int, mach_int)(&zgsz3332, INT64_C(5));
          zgaz3139 = bitvector_access(zgsz3331, zgsz3332);
          KILL(sail_int)(&zgsz3332);
          KILL(lbits)(&zgsz3331);
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
          lbits zgsz3333;
          CREATE(lbits)(&zgsz3333);
          CONVERT_OF(lbits, fbits)(&zgsz3333, zgaz3141, UINT64_C(12) , true);
          sail_int zgsz3334;
          CREATE(sail_int)(&zgsz3334);
          CONVERT_OF(sail_int, mach_int)(&zgsz3334, INT64_C(4));
          zgaz3142 = bitvector_access(zgsz3333, zgsz3334);
          KILL(sail_int)(&zgsz3334);
          KILL(lbits)(&zgsz3333);
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
          lbits zgsz3335;
          CREATE(lbits)(&zgsz3335);
          CONVERT_OF(lbits, fbits)(&zgsz3335, zgaz3144, UINT64_C(12) , true);
          sail_int zgsz3336;
          CREATE(sail_int)(&zgsz3336);
          CONVERT_OF(sail_int, mach_int)(&zgsz3336, INT64_C(3));
          zgaz3145 = bitvector_access(zgsz3335, zgsz3336);
          KILL(sail_int)(&zgsz3336);
          KILL(lbits)(&zgsz3335);
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
          lbits zgsz3337;
          CREATE(lbits)(&zgsz3337);
          CONVERT_OF(lbits, fbits)(&zgsz3337, zgaz3147, UINT64_C(12) , true);
          sail_int zgsz3338;
          CREATE(sail_int)(&zgsz3338);
          CONVERT_OF(sail_int, mach_int)(&zgsz3338, INT64_C(2));
          zgaz3148 = bitvector_access(zgsz3337, zgsz3338);
          KILL(sail_int)(&zgsz3338);
          KILL(lbits)(&zgsz3337);
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
          lbits zgsz3339;
          CREATE(lbits)(&zgsz3339);
          CONVERT_OF(lbits, fbits)(&zgsz3339, zgaz3150, UINT64_C(12) , true);
          sail_int zgsz3340;
          CREATE(sail_int)(&zgsz3340);
          CONVERT_OF(sail_int, mach_int)(&zgsz3340, INT64_C(1));
          zgaz3151 = bitvector_access(zgsz3339, zgsz3340);
          KILL(sail_int)(&zgsz3340);
          KILL(lbits)(&zgsz3339);
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
          lbits zgsz3341;
          CREATE(lbits)(&zgsz3341);
          CONVERT_OF(lbits, fbits)(&zgsz3341, zgaz3153, UINT64_C(12) , true);
          sail_int zgsz3342;
          CREATE(sail_int)(&zgsz3342);
          CONVERT_OF(sail_int, mach_int)(&zgsz3342, INT64_C(0));
          zgaz3154 = bitvector_access(zgsz3341, zgsz3342);
          KILL(sail_int)(&zgsz3342);
          KILL(lbits)(&zgsz3341);
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
          lbits zgsz3343;
          CREATE(lbits)(&zgsz3343);
          CONVERT_OF(lbits, fbits)(&zgsz3343, zgaz3157, UINT64_C(1) , true);
          sail_int zgsz3344;
          CREATE(sail_int)(&zgsz3344);
          CONVERT_OF(sail_int, mach_int)(&zgsz3344, INT64_C(0));
          zgaz3158 = bitvector_access(zgsz3343, zgsz3344);
          KILL(sail_int)(&zgsz3344);
          KILL(lbits)(&zgsz3343);
        }
      }
      zgaz3159 = sailgen_bit_to_bool(zgaz3158);
    }
    uint64_t zgaz3160;
    {
      lbits zgsz3345;
      CREATE(lbits)(&zgsz3345);
      CONVERT_OF(lbits, fbits)(&zgsz3345, zTtop2, UINT64_C(2) , true);
      lbits zgsz3346;
      CREATE(lbits)(&zgsz3346);
      CONVERT_OF(lbits, fbits)(&zgsz3346, zT, UINT64_C(6) , true);
      lbits zgsz3347;
      CREATE(lbits)(&zgsz3347);
      append(&zgsz3347, zgsz3345, zgsz3346);
      zgaz3160 = CONVERT_OF(fbits, lbits)(zgsz3347, true);
      KILL(lbits)(&zgsz3347);
      KILL(lbits)(&zgsz3346);
      KILL(lbits)(&zgsz3345);
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
  struct zCapability zcbz345 = { .zuperms = UINT64_C(0xdeadc0de), .ztag = false, .zreserved = UINT64_C(0xdeadc0de), .zpermit_unseal = false, .zpermit_store_local_cap = false, .zpermit_store_cap = false, .zpermit_store = false, .zpermit_set_CID = false, .zpermit_seal = false, .zpermit_load_cap = false, .zpermit_load = false, .zpermit_execute = false, .zpermit_cinvoke = false, .zotype = UINT64_C(0xdeadc0de), .zinternal_E = false, .zglobal = false, .zflag_cap_mode = false, .zaddress = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zT = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  return zcbz345;
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
    uint64_t zgaz3200;
    zgaz3200 = zcap.zT;
    int64_t zgaz3201;
    {
      sail_int zgsz3358;
      CREATE(sail_int)(&zgsz3358);
      CONVERT_OF(sail_int, mach_int)(&zgsz3358, zcap_mantissa_width);
      sail_int zgsz3359;
      CREATE(sail_int)(&zgsz3359);
      CONVERT_OF(sail_int, mach_int)(&zgsz3359, INT64_C(3));
      sail_int zgsz3360;
      CREATE(sail_int)(&zgsz3360);
      sub_int(&zgsz3360, zgsz3358, zgsz3359);
      zgaz3201 = CONVERT_OF(mach_int, sail_int)(zgsz3360);
      KILL(sail_int)(&zgsz3360);
      KILL(sail_int)(&zgsz3359);
      KILL(sail_int)(&zgsz3358);
    }
    {
      lbits zgsz3354;
      CREATE(lbits)(&zgsz3354);
      CONVERT_OF(lbits, fbits)(&zgsz3354, zgaz3200, UINT64_C(8) , true);
      sail_int zgsz3355;
      CREATE(sail_int)(&zgsz3355);
      CONVERT_OF(sail_int, mach_int)(&zgsz3355, zgaz3201);
      sail_int zgsz3356;
      CREATE(sail_int)(&zgsz3356);
      CONVERT_OF(sail_int, mach_int)(&zgsz3356, zinternal_E_take_bits);
      lbits zgsz3357;
      CREATE(lbits)(&zgsz3357);
      vector_subrange_lbits(&zgsz3357, zgsz3354, zgsz3355, zgsz3356);
      zt_hi = CONVERT_OF(fbits, lbits)(zgsz3357, true);
      KILL(lbits)(&zgsz3357);
      KILL(sail_int)(&zgsz3356);
      KILL(sail_int)(&zgsz3355);
      KILL(lbits)(&zgsz3354);
    }
  }
  uint64_t zt_lo;
  {
    uint64_t zgaz3198;
    zgaz3198 = zcap.zT;
    int64_t zgaz3199;
    {
      sail_int zgsz3365;
      CREATE(sail_int)(&zgsz3365);
      CONVERT_OF(sail_int, mach_int)(&zgsz3365, zinternal_E_take_bits);
      sail_int zgsz3366;
      CREATE(sail_int)(&zgsz3366);
      CONVERT_OF(sail_int, mach_int)(&zgsz3366, INT64_C(1));
      sail_int zgsz3367;
      CREATE(sail_int)(&zgsz3367);
      sub_int(&zgsz3367, zgsz3365, zgsz3366);
      zgaz3199 = CONVERT_OF(mach_int, sail_int)(zgsz3367);
      KILL(sail_int)(&zgsz3367);
      KILL(sail_int)(&zgsz3366);
      KILL(sail_int)(&zgsz3365);
    }
    {
      lbits zgsz3361;
      CREATE(lbits)(&zgsz3361);
      CONVERT_OF(lbits, fbits)(&zgsz3361, zgaz3198, UINT64_C(8) , true);
      sail_int zgsz3362;
      CREATE(sail_int)(&zgsz3362);
      CONVERT_OF(sail_int, mach_int)(&zgsz3362, zgaz3199);
      sail_int zgsz3363;
      CREATE(sail_int)(&zgsz3363);
      CONVERT_OF(sail_int, mach_int)(&zgsz3363, INT64_C(0));
      lbits zgsz3364;
      CREATE(lbits)(&zgsz3364);
      vector_subrange_lbits(&zgsz3364, zgsz3361, zgsz3362, zgsz3363);
      zt_lo = CONVERT_OF(fbits, lbits)(zgsz3364, true);
      KILL(lbits)(&zgsz3364);
      KILL(sail_int)(&zgsz3363);
      KILL(sail_int)(&zgsz3362);
      KILL(lbits)(&zgsz3361);
    }
  }
  uint64_t zb_hi;
  {
    uint64_t zgaz3196;
    zgaz3196 = zcap.zB;
    int64_t zgaz3197;
    {
      sail_int zgsz3372;
      CREATE(sail_int)(&zgsz3372);
      CONVERT_OF(sail_int, mach_int)(&zgsz3372, zcap_mantissa_width);
      sail_int zgsz3373;
      CREATE(sail_int)(&zgsz3373);
      CONVERT_OF(sail_int, mach_int)(&zgsz3373, INT64_C(1));
      sail_int zgsz3374;
      CREATE(sail_int)(&zgsz3374);
      sub_int(&zgsz3374, zgsz3372, zgsz3373);
      zgaz3197 = CONVERT_OF(mach_int, sail_int)(zgsz3374);
      KILL(sail_int)(&zgsz3374);
      KILL(sail_int)(&zgsz3373);
      KILL(sail_int)(&zgsz3372);
    }
    {
      lbits zgsz3368;
      CREATE(lbits)(&zgsz3368);
      CONVERT_OF(lbits, fbits)(&zgsz3368, zgaz3196, UINT64_C(8) , true);
      sail_int zgsz3369;
      CREATE(sail_int)(&zgsz3369);
      CONVERT_OF(sail_int, mach_int)(&zgsz3369, zgaz3197);
      sail_int zgsz3370;
      CREATE(sail_int)(&zgsz3370);
      CONVERT_OF(sail_int, mach_int)(&zgsz3370, zinternal_E_take_bits);
      lbits zgsz3371;
      CREATE(lbits)(&zgsz3371);
      vector_subrange_lbits(&zgsz3371, zgsz3368, zgsz3369, zgsz3370);
      zb_hi = CONVERT_OF(fbits, lbits)(zgsz3371, true);
      KILL(lbits)(&zgsz3371);
      KILL(sail_int)(&zgsz3370);
      KILL(sail_int)(&zgsz3369);
      KILL(lbits)(&zgsz3368);
    }
  }
  uint64_t zb_lo;
  {
    uint64_t zgaz3194;
    zgaz3194 = zcap.zB;
    int64_t zgaz3195;
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
      zgaz3195 = CONVERT_OF(mach_int, sail_int)(zgsz3381);
      KILL(sail_int)(&zgsz3381);
      KILL(sail_int)(&zgsz3380);
      KILL(sail_int)(&zgsz3379);
    }
    {
      lbits zgsz3375;
      CREATE(lbits)(&zgsz3375);
      CONVERT_OF(lbits, fbits)(&zgsz3375, zgaz3194, UINT64_C(8) , true);
      sail_int zgsz3376;
      CREATE(sail_int)(&zgsz3376);
      CONVERT_OF(sail_int, mach_int)(&zgsz3376, zgaz3195);
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
            sail_int zgsz3385;
            CREATE(sail_int)(&zgsz3385);
            CONVERT_OF(sail_int, mach_int)(&zgsz3385, INT64_C(2));
            sail_int zgsz3386;
            CREATE(sail_int)(&zgsz3386);
            CONVERT_OF(sail_int, mach_int)(&zgsz3386, zinternal_E_take_bits);
            sail_int zgsz3387;
            CREATE(sail_int)(&zgsz3387);
            mult_int(&zgsz3387, zgsz3385, zgsz3386);
            zgaz3175 = CONVERT_OF(mach_int, sail_int)(zgsz3387);
            KILL(sail_int)(&zgsz3387);
            KILL(sail_int)(&zgsz3386);
            KILL(sail_int)(&zgsz3385);
          }
          {
            sail_int zgsz3382;
            CREATE(sail_int)(&zgsz3382);
            CONVERT_OF(sail_int, mach_int)(&zgsz3382, zgaz3175);
            sail_int zgsz3383;
            CREATE(sail_int)(&zgsz3383);
            CONVERT_OF(sail_int, mach_int)(&zgsz3383, INT64_C(1));
            sail_int zgsz3384;
            CREATE(sail_int)(&zgsz3384);
            sub_int(&zgsz3384, zgsz3382, zgsz3383);
            zgaz3177 = CONVERT_OF(mach_int, sail_int)(zgsz3384);
            KILL(sail_int)(&zgsz3384);
            KILL(sail_int)(&zgsz3383);
            KILL(sail_int)(&zgsz3382);
          }
        }
        {
          lbits zgsz3388;
          CREATE(lbits)(&zgsz3388);
          CONVERT_OF(lbits, fbits)(&zgsz3388, zgaz3176, UINT64_C(6) , true);
          sail_int zgsz3389;
          CREATE(sail_int)(&zgsz3389);
          CONVERT_OF(sail_int, mach_int)(&zgsz3389, zgaz3177);
          sail_int zgsz3390;
          CREATE(sail_int)(&zgsz3390);
          CONVERT_OF(sail_int, mach_int)(&zgsz3390, zinternal_E_take_bits);
          lbits zgsz3391;
          CREATE(lbits)(&zgsz3391);
          vector_subrange_lbits(&zgsz3391, zgsz3388, zgsz3389, zgsz3390);
          zt_lo = CONVERT_OF(fbits, lbits)(zgsz3391, true);
          KILL(lbits)(&zgsz3391);
          KILL(sail_int)(&zgsz3390);
          KILL(sail_int)(&zgsz3389);
          KILL(lbits)(&zgsz3388);
        }
        unit zgsz344;
        zgsz344 = UNIT;
      }
      uint64_t zgaz3178;
      zgaz3178 = zcap.zE;
      int64_t zgaz3179;
      {
        sail_int zgsz3396;
        CREATE(sail_int)(&zgsz3396);
        CONVERT_OF(sail_int, mach_int)(&zgsz3396, zinternal_E_take_bits);
        sail_int zgsz3397;
        CREATE(sail_int)(&zgsz3397);
        CONVERT_OF(sail_int, mach_int)(&zgsz3397, INT64_C(1));
        sail_int zgsz3398;
        CREATE(sail_int)(&zgsz3398);
        sub_int(&zgsz3398, zgsz3396, zgsz3397);
        zgaz3179 = CONVERT_OF(mach_int, sail_int)(zgsz3398);
        KILL(sail_int)(&zgsz3398);
        KILL(sail_int)(&zgsz3397);
        KILL(sail_int)(&zgsz3396);
      }
      {
        lbits zgsz3392;
        CREATE(lbits)(&zgsz3392);
        CONVERT_OF(lbits, fbits)(&zgsz3392, zgaz3178, UINT64_C(6) , true);
        sail_int zgsz3393;
        CREATE(sail_int)(&zgsz3393);
        CONVERT_OF(sail_int, mach_int)(&zgsz3393, zgaz3179);
        sail_int zgsz3394;
        CREATE(sail_int)(&zgsz3394);
        CONVERT_OF(sail_int, mach_int)(&zgsz3394, INT64_C(0));
        lbits zgsz3395;
        CREATE(lbits)(&zgsz3395);
        vector_subrange_lbits(&zgsz3395, zgsz3392, zgsz3393, zgsz3394);
        zb_lo = CONVERT_OF(fbits, lbits)(zgsz3395, true);
        KILL(lbits)(&zgsz3395);
        KILL(sail_int)(&zgsz3394);
        KILL(sail_int)(&zgsz3393);
        KILL(lbits)(&zgsz3392);
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
        lbits zgsz3399;
        CREATE(lbits)(&zgsz3399);
        CONVERT_OF(lbits, fbits)(&zgsz3399, zgaz3180, UINT64_C(0) , true);
        lbits zgsz3400;
        CREATE(lbits)(&zgsz3400);
        CONVERT_OF(lbits, fbits)(&zgsz3400, zgaz3181, UINT64_C(12) , true);
        lbits zgsz3401;
        CREATE(lbits)(&zgsz3401);
        append(&zgsz3401, zgsz3399, zgsz3400);
        zgaz3182 = CONVERT_OF(fbits, lbits)(zgsz3401, true);
        KILL(lbits)(&zgsz3401);
        KILL(lbits)(&zgsz3400);
        KILL(lbits)(&zgsz3399);
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
        lbits zgsz3402;
        CREATE(lbits)(&zgsz3402);
        CONVERT_OF(lbits, fbits)(&zgsz3402, zgaz3188, UINT64_C(1) , true);
        sail_int zgsz3403;
        CREATE(sail_int)(&zgsz3403);
        CONVERT_OF(sail_int, mach_int)(&zgsz3403, INT64_C(0));
        zgaz3189 = bitvector_access(zgsz3402, zgsz3403);
        KILL(sail_int)(&zgsz3403);
        KILL(lbits)(&zgsz3402);
      }
    }
    uint64_t zgaz3190;
    {
      lbits zgsz3407;
      CREATE(lbits)(&zgsz3407);
      CONVERT_OF(lbits, fbits)(&zgsz3407, zt_hi, UINT64_C(3) , true);
      lbits zgsz3408;
      CREATE(lbits)(&zgsz3408);
      CONVERT_OF(lbits, fbits)(&zgsz3408, zt_lo, UINT64_C(3) , true);
      lbits zgsz3409;
      CREATE(lbits)(&zgsz3409);
      append(&zgsz3409, zgsz3407, zgsz3408);
      zgaz3190 = CONVERT_OF(fbits, lbits)(zgsz3409, true);
      KILL(lbits)(&zgsz3409);
      KILL(lbits)(&zgsz3408);
      KILL(lbits)(&zgsz3407);
    }
    uint64_t zgaz3191;
    {
      lbits zgsz3404;
      CREATE(lbits)(&zgsz3404);
      CONVERT_OF(lbits, fbits)(&zgsz3404, zb_hi, UINT64_C(5) , true);
      lbits zgsz3405;
      CREATE(lbits)(&zgsz3405);
      CONVERT_OF(lbits, fbits)(&zgsz3405, zb_lo, UINT64_C(3) , true);
      lbits zgsz3406;
      CREATE(lbits)(&zgsz3406);
      append(&zgsz3406, zgsz3404, zgsz3405);
      zgaz3191 = CONVERT_OF(fbits, lbits)(zgsz3406, true);
      KILL(lbits)(&zgsz3406);
      KILL(lbits)(&zgsz3405);
      KILL(lbits)(&zgsz3404);
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
  struct zEncCapability zcbz347 = { .zreserved = UINT64_C(0xdeadc0de), .zperms = UINT64_C(0xdeadc0de), .zotype = UINT64_C(0xdeadc0de), .zinternal_E = UINT64_C(0), .zflags = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  return zcbz347;
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
    lbits zgsz3410;
    CREATE(lbits)(&zgsz3410);
    CONVERT_OF(lbits, fbits)(&zgsz3410, zgaz3203, UINT64_C(64) , true);
    lbits zgsz3411;
    CREATE(lbits)(&zgsz3411);
    CONVERT_OF(lbits, fbits)(&zgsz3411, znull_cap_bits, UINT64_C(64) , true);
    lbits zgsz3412;
    CREATE(lbits)(&zgsz3412);
    xor_bits(&zgsz3412, zgsz3410, zgsz3411);
    zcbz322 = CONVERT_OF(fbits, lbits)(zgsz3412, true);
    KILL(lbits)(&zgsz3412);
    KILL(lbits)(&zgsz3411);
    KILL(lbits)(&zgsz3410);
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
    lbits zgsz3413;
    CREATE(lbits)(&zgsz3413);
    CONVERT_OF(lbits, fbits)(&zgsz3413, zb, UINT64_C(64) , true);
    lbits zgsz3414;
    CREATE(lbits)(&zgsz3414);
    CONVERT_OF(lbits, fbits)(&zgsz3414, znull_cap_bits, UINT64_C(64) , true);
    lbits zgsz3415;
    CREATE(lbits)(&zgsz3415);
    xor_bits(&zgsz3415, zgsz3413, zgsz3414);
    zgaz3204 = CONVERT_OF(fbits, lbits)(zgsz3415, true);
    KILL(lbits)(&zgsz3415);
    KILL(lbits)(&zgsz3414);
    KILL(lbits)(&zgsz3413);
  }
  zcbz323 = sailgen_capBitsToCapability(ztag, zgaz3204);

end_function_105: ;
  return zcbz323;
end_block_exception_106: ;
  struct zCapability zcbz348 = { .zuperms = UINT64_C(0xdeadc0de), .ztag = false, .zreserved = UINT64_C(0xdeadc0de), .zpermit_unseal = false, .zpermit_store_local_cap = false, .zpermit_store_cap = false, .zpermit_store = false, .zpermit_set_CID = false, .zpermit_seal = false, .zpermit_load_cap = false, .zpermit_load = false, .zpermit_execute = false, .zpermit_cinvoke = false, .zotype = UINT64_C(0xdeadc0de), .zinternal_E = false, .zglobal = false, .zflag_cap_mode = false, .zaddress = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zT = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  return zcbz348;
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
        lbits zgsz3416;
        CREATE(lbits)(&zgsz3416);
        CONVERT_OF(lbits, fbits)(&zgsz3416, zgaz3237, UINT64_C(6) , true);
        sail_int zgsz3417;
        CREATE(sail_int)(&zgsz3417);
        sail_unsigned(&zgsz3417, zgsz3416);
        zgaz3238 = CONVERT_OF(mach_int, sail_int)(zgsz3417);
        KILL(sail_int)(&zgsz3417);
        KILL(lbits)(&zgsz3416);
      }
    }
    {
      sail_int zgsz3418;
      CREATE(sail_int)(&zgsz3418);
      CONVERT_OF(sail_int, mach_int)(&zgsz3418, zcap_max_E);
      sail_int zgsz3419;
      CREATE(sail_int)(&zgsz3419);
      CONVERT_OF(sail_int, mach_int)(&zgsz3419, zgaz3238);
      sail_int zgsz3420;
      CREATE(sail_int)(&zgsz3420);
      min_int(&zgsz3420, zgsz3418, zgsz3419);
      zE = CONVERT_OF(mach_int, sail_int)(zgsz3420);
      KILL(sail_int)(&zgsz3420);
      KILL(sail_int)(&zgsz3419);
      KILL(sail_int)(&zgsz3418);
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
          sail_int zgsz3424;
          CREATE(sail_int)(&zgsz3424);
          CONVERT_OF(sail_int, mach_int)(&zgsz3424, zE);
          sail_int zgsz3425;
          CREATE(sail_int)(&zgsz3425);
          CONVERT_OF(sail_int, mach_int)(&zgsz3425, zcap_mantissa_width);
          sail_int zgsz3426;
          CREATE(sail_int)(&zgsz3426);
          add_int(&zgsz3426, zgsz3424, zgsz3425);
          zgaz3234 = CONVERT_OF(mach_int, sail_int)(zgsz3426);
          KILL(sail_int)(&zgsz3426);
          KILL(sail_int)(&zgsz3425);
          KILL(sail_int)(&zgsz3424);
        }
        {
          sail_int zgsz3421;
          CREATE(sail_int)(&zgsz3421);
          CONVERT_OF(sail_int, mach_int)(&zgsz3421, zgaz3234);
          sail_int zgsz3422;
          CREATE(sail_int)(&zgsz3422);
          CONVERT_OF(sail_int, mach_int)(&zgsz3422, INT64_C(3));
          sail_int zgsz3423;
          CREATE(sail_int)(&zgsz3423);
          sub_int(&zgsz3423, zgsz3421, zgsz3422);
          zgaz3235 = CONVERT_OF(mach_int, sail_int)(zgsz3423);
          KILL(sail_int)(&zgsz3423);
          KILL(sail_int)(&zgsz3422);
          KILL(sail_int)(&zgsz3421);
        }
      }
      {
        lbits zgsz3427;
        CREATE(lbits)(&zgsz3427);
        CONVERT_OF(lbits, fbits)(&zgsz3427, za, UINT64_C(32) , true);
        sail_int zgsz3428;
        CREATE(sail_int)(&zgsz3428);
        CONVERT_OF(sail_int, mach_int)(&zgsz3428, zgaz3235);
        lbits zgsz3429;
        CREATE(lbits)(&zgsz3429);
        shiftr(&zgsz3429, zgsz3427, zgsz3428);
        zgaz3236 = CONVERT_OF(fbits, lbits)(zgsz3429, true);
        KILL(lbits)(&zgsz3429);
        KILL(sail_int)(&zgsz3428);
        KILL(lbits)(&zgsz3427);
      }
    }
    {
      lbits zgsz3430;
      CREATE(lbits)(&zgsz3430);
      CONVERT_OF(lbits, fbits)(&zgsz3430, zgaz3236, UINT64_C(32) , true);
      sail_int zgsz3431;
      CREATE(sail_int)(&zgsz3431);
      CONVERT_OF(sail_int, mach_int)(&zgsz3431, INT64_C(3));
      lbits zgsz3432;
      CREATE(lbits)(&zgsz3432);
      sail_truncate(&zgsz3432, zgsz3430, zgsz3431);
      za3 = CONVERT_OF(fbits, lbits)(zgsz3432, true);
      KILL(lbits)(&zgsz3432);
      KILL(sail_int)(&zgsz3431);
      KILL(lbits)(&zgsz3430);
    }
  }
  uint64_t zB3;
  {
    uint64_t zgaz3233;
    zgaz3233 = zc.zB;
    {
      lbits zgsz3433;
      CREATE(lbits)(&zgsz3433);
      CONVERT_OF(lbits, fbits)(&zgsz3433, zgaz3233, UINT64_C(8) , true);
      sail_int zgsz3434;
      CREATE(sail_int)(&zgsz3434);
      CONVERT_OF(sail_int, mach_int)(&zgsz3434, INT64_C(3));
      lbits zgsz3435;
      CREATE(lbits)(&zgsz3435);
      sail_truncateLSB(&zgsz3435, zgsz3433, zgsz3434);
      zB3 = CONVERT_OF(fbits, lbits)(zgsz3435, true);
      KILL(lbits)(&zgsz3435);
      KILL(sail_int)(&zgsz3434);
      KILL(lbits)(&zgsz3433);
    }
  }
  uint64_t zT3;
  {
    uint64_t zgaz3232;
    zgaz3232 = zc.zT;
    {
      lbits zgsz3436;
      CREATE(lbits)(&zgsz3436);
      CONVERT_OF(lbits, fbits)(&zgsz3436, zgaz3232, UINT64_C(8) , true);
      sail_int zgsz3437;
      CREATE(sail_int)(&zgsz3437);
      CONVERT_OF(sail_int, mach_int)(&zgsz3437, INT64_C(3));
      lbits zgsz3438;
      CREATE(lbits)(&zgsz3438);
      sail_truncateLSB(&zgsz3438, zgsz3436, zgsz3437);
      zT3 = CONVERT_OF(fbits, lbits)(zgsz3438, true);
      KILL(lbits)(&zgsz3438);
      KILL(sail_int)(&zgsz3437);
      KILL(lbits)(&zgsz3436);
    }
  }
  uint64_t zR3;
  {
    lbits zgsz3520;
    CREATE(lbits)(&zgsz3520);
    CONVERT_OF(lbits, fbits)(&zgsz3520, zB3, UINT64_C(3) , true);
    lbits zgsz3521;
    CREATE(lbits)(&zgsz3521);
    CONVERT_OF(lbits, fbits)(&zgsz3521, UINT64_C(0b001), UINT64_C(3) , true);
    lbits zgsz3522;
    CREATE(lbits)(&zgsz3522);
    sub_bits(&zgsz3522, zgsz3520, zgsz3521);
    zR3 = CONVERT_OF(fbits, lbits)(zgsz3522, true);
    KILL(lbits)(&zgsz3522);
    KILL(lbits)(&zgsz3521);
    KILL(lbits)(&zgsz3520);
  }
  int64_t zaHi;
  {
    bool zgaz3231;
    {
      lbits zgsz3439;
      CREATE(lbits)(&zgsz3439);
      CONVERT_OF(lbits, fbits)(&zgsz3439, za3, UINT64_C(3) , true);
      lbits zgsz3440;
      CREATE(lbits)(&zgsz3440);
      CONVERT_OF(lbits, fbits)(&zgsz3440, zR3, UINT64_C(3) , true);
      zgaz3231 = sailgen_z8operatorz0zI_uz9(zgsz3439, zgsz3440);
      KILL(lbits)(&zgsz3440);
      KILL(lbits)(&zgsz3439);
    }
    if (zgaz3231) {  zaHi = INT64_C(1);  } else {  zaHi = INT64_C(0);  }
  }
  int64_t zbHi;
  {
    bool zgaz3230;
    {
      lbits zgsz3441;
      CREATE(lbits)(&zgsz3441);
      CONVERT_OF(lbits, fbits)(&zgsz3441, zB3, UINT64_C(3) , true);
      lbits zgsz3442;
      CREATE(lbits)(&zgsz3442);
      CONVERT_OF(lbits, fbits)(&zgsz3442, zR3, UINT64_C(3) , true);
      zgaz3230 = sailgen_z8operatorz0zI_uz9(zgsz3441, zgsz3442);
      KILL(lbits)(&zgsz3442);
      KILL(lbits)(&zgsz3441);
    }
    if (zgaz3230) {  zbHi = INT64_C(1);  } else {  zbHi = INT64_C(0);  }
  }
  int64_t ztHi;
  {
    bool zgaz3229;
    {
      lbits zgsz3443;
      CREATE(lbits)(&zgsz3443);
      CONVERT_OF(lbits, fbits)(&zgsz3443, zT3, UINT64_C(3) , true);
      lbits zgsz3444;
      CREATE(lbits)(&zgsz3444);
      CONVERT_OF(lbits, fbits)(&zgsz3444, zR3, UINT64_C(3) , true);
      zgaz3229 = sailgen_z8operatorz0zI_uz9(zgsz3443, zgsz3444);
      KILL(lbits)(&zgsz3444);
      KILL(lbits)(&zgsz3443);
    }
    if (zgaz3229) {  ztHi = INT64_C(1);  } else {  ztHi = INT64_C(0);  }
  }
  int64_t zcorrection_base;
  {
    sail_int zgsz3517;
    CREATE(sail_int)(&zgsz3517);
    CONVERT_OF(sail_int, mach_int)(&zgsz3517, zbHi);
    sail_int zgsz3518;
    CREATE(sail_int)(&zgsz3518);
    CONVERT_OF(sail_int, mach_int)(&zgsz3518, zaHi);
    sail_int zgsz3519;
    CREATE(sail_int)(&zgsz3519);
    sub_int(&zgsz3519, zgsz3517, zgsz3518);
    zcorrection_base = CONVERT_OF(mach_int, sail_int)(zgsz3519);
    KILL(sail_int)(&zgsz3519);
    KILL(sail_int)(&zgsz3518);
    KILL(sail_int)(&zgsz3517);
  }
  int64_t zcorrection_top;
  {
    sail_int zgsz3514;
    CREATE(sail_int)(&zgsz3514);
    CONVERT_OF(sail_int, mach_int)(&zgsz3514, ztHi);
    sail_int zgsz3515;
    CREATE(sail_int)(&zgsz3515);
    CONVERT_OF(sail_int, mach_int)(&zgsz3515, zaHi);
    sail_int zgsz3516;
    CREATE(sail_int)(&zgsz3516);
    sub_int(&zgsz3516, zgsz3514, zgsz3515);
    zcorrection_top = CONVERT_OF(mach_int, sail_int)(zgsz3516);
    KILL(sail_int)(&zgsz3516);
    KILL(sail_int)(&zgsz3515);
    KILL(sail_int)(&zgsz3514);
  }
  uint64_t za_top;
  {
    int64_t zgaz3228;
    {
      sail_int zgsz3448;
      CREATE(sail_int)(&zgsz3448);
      CONVERT_OF(sail_int, mach_int)(&zgsz3448, zE);
      sail_int zgsz3449;
      CREATE(sail_int)(&zgsz3449);
      CONVERT_OF(sail_int, mach_int)(&zgsz3449, zcap_mantissa_width);
      sail_int zgsz3450;
      CREATE(sail_int)(&zgsz3450);
      add_int(&zgsz3450, zgsz3448, zgsz3449);
      zgaz3228 = CONVERT_OF(mach_int, sail_int)(zgsz3450);
      KILL(sail_int)(&zgsz3450);
      KILL(sail_int)(&zgsz3449);
      KILL(sail_int)(&zgsz3448);
    }
    {
      lbits zgsz3445;
      CREATE(lbits)(&zgsz3445);
      CONVERT_OF(lbits, fbits)(&zgsz3445, za, UINT64_C(32) , true);
      sail_int zgsz3446;
      CREATE(sail_int)(&zgsz3446);
      CONVERT_OF(sail_int, mach_int)(&zgsz3446, zgaz3228);
      lbits zgsz3447;
      CREATE(lbits)(&zgsz3447);
      shiftr(&zgsz3447, zgsz3445, zgsz3446);
      za_top = CONVERT_OF(fbits, lbits)(zgsz3447, true);
      KILL(lbits)(&zgsz3447);
      KILL(sail_int)(&zgsz3446);
      KILL(lbits)(&zgsz3445);
    }
  }
  uint64_t zbase;
  {
    lbits zgaz3227;
    CREATE(lbits)(&zgaz3227);
    {
      uint64_t zgaz3225;
      {
        lbits zgsz3458;
        CREATE(lbits)(&zgsz3458);
        CONVERT_OF(lbits, fbits)(&zgsz3458, za_top, UINT64_C(32) , true);
        sail_int zgsz3459;
        CREATE(sail_int)(&zgsz3459);
        CONVERT_OF(sail_int, mach_int)(&zgsz3459, zcorrection_base);
        lbits zgsz3460;
        CREATE(lbits)(&zgsz3460);
        add_bits_int(&zgsz3460, zgsz3458, zgsz3459);
        zgaz3225 = CONVERT_OF(fbits, lbits)(zgsz3460, true);
        KILL(lbits)(&zgsz3460);
        KILL(sail_int)(&zgsz3459);
        KILL(lbits)(&zgsz3458);
      }
      sbits zgaz3226;
      {
        uint64_t zgaz3223;
        zgaz3223 = zc.zB;
        sbits zgaz3224;
        {
          sail_int zgsz3454;
          CREATE(sail_int)(&zgsz3454);
          CONVERT_OF(sail_int, mach_int)(&zgsz3454, zE);
          lbits zgsz3455;
          CREATE(lbits)(&zgsz3455);
          sailgen_zzeros_implicit(&zgsz3455, zgsz3454);
          zgaz3224 = CONVERT_OF(sbits, lbits)(zgsz3455, true);
          KILL(lbits)(&zgsz3455);
          KILL(sail_int)(&zgsz3454);
        }
        {
          lbits zgsz3451;
          CREATE(lbits)(&zgsz3451);
          CONVERT_OF(lbits, fbits)(&zgsz3451, zgaz3223, UINT64_C(8) , true);
          lbits zgsz3452;
          CREATE(lbits)(&zgsz3452);
          CONVERT_OF(lbits, sbits)(&zgsz3452, zgaz3224, true);
          lbits zgsz3453;
          CREATE(lbits)(&zgsz3453);
          append(&zgsz3453, zgsz3451, zgsz3452);
          zgaz3226 = CONVERT_OF(sbits, lbits)(zgsz3453, true);
          KILL(lbits)(&zgsz3453);
          KILL(lbits)(&zgsz3452);
          KILL(lbits)(&zgsz3451);
        }
      }
      {
        lbits zgsz3456;
        CREATE(lbits)(&zgsz3456);
        CONVERT_OF(lbits, fbits)(&zgsz3456, zgaz3225, UINT64_C(32) , true);
        lbits zgsz3457;
        CREATE(lbits)(&zgsz3457);
        CONVERT_OF(lbits, sbits)(&zgsz3457, zgaz3226, true);
        append(&zgaz3227, zgsz3456, zgsz3457);
        KILL(lbits)(&zgsz3457);
        KILL(lbits)(&zgsz3456);
      }
    }
    {
      sail_int zgsz3461;
      CREATE(sail_int)(&zgsz3461);
      CONVERT_OF(sail_int, mach_int)(&zgsz3461, zcap_len_width);
      lbits zgsz3462;
      CREATE(lbits)(&zgsz3462);
      sail_truncate(&zgsz3462, zgaz3227, zgsz3461);
      zbase = CONVERT_OF(fbits, lbits)(zgsz3462, true);
      KILL(lbits)(&zgsz3462);
      KILL(sail_int)(&zgsz3461);
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
        lbits zgsz3470;
        CREATE(lbits)(&zgsz3470);
        CONVERT_OF(lbits, fbits)(&zgsz3470, za_top, UINT64_C(32) , true);
        sail_int zgsz3471;
        CREATE(sail_int)(&zgsz3471);
        CONVERT_OF(sail_int, mach_int)(&zgsz3471, zcorrection_top);
        lbits zgsz3472;
        CREATE(lbits)(&zgsz3472);
        add_bits_int(&zgsz3472, zgsz3470, zgsz3471);
        zgaz3220 = CONVERT_OF(fbits, lbits)(zgsz3472, true);
        KILL(lbits)(&zgsz3472);
        KILL(sail_int)(&zgsz3471);
        KILL(lbits)(&zgsz3470);
      }
      sbits zgaz3221;
      {
        uint64_t zgaz3218;
        zgaz3218 = zc.zT;
        sbits zgaz3219;
        {
          sail_int zgsz3466;
          CREATE(sail_int)(&zgsz3466);
          CONVERT_OF(sail_int, mach_int)(&zgsz3466, zE);
          lbits zgsz3467;
          CREATE(lbits)(&zgsz3467);
          sailgen_zzeros_implicit(&zgsz3467, zgsz3466);
          zgaz3219 = CONVERT_OF(sbits, lbits)(zgsz3467, true);
          KILL(lbits)(&zgsz3467);
          KILL(sail_int)(&zgsz3466);
        }
        {
          lbits zgsz3463;
          CREATE(lbits)(&zgsz3463);
          CONVERT_OF(lbits, fbits)(&zgsz3463, zgaz3218, UINT64_C(8) , true);
          lbits zgsz3464;
          CREATE(lbits)(&zgsz3464);
          CONVERT_OF(lbits, sbits)(&zgsz3464, zgaz3219, true);
          lbits zgsz3465;
          CREATE(lbits)(&zgsz3465);
          append(&zgsz3465, zgsz3463, zgsz3464);
          zgaz3221 = CONVERT_OF(sbits, lbits)(zgsz3465, true);
          KILL(lbits)(&zgsz3465);
          KILL(lbits)(&zgsz3464);
          KILL(lbits)(&zgsz3463);
        }
      }
      {
        lbits zgsz3468;
        CREATE(lbits)(&zgsz3468);
        CONVERT_OF(lbits, fbits)(&zgsz3468, zgaz3220, UINT64_C(32) , true);
        lbits zgsz3469;
        CREATE(lbits)(&zgsz3469);
        CONVERT_OF(lbits, sbits)(&zgsz3469, zgaz3221, true);
        append(&zgaz3222, zgsz3468, zgsz3469);
        KILL(lbits)(&zgsz3469);
        KILL(lbits)(&zgsz3468);
      }
    }
    {
      sail_int zgsz3473;
      CREATE(sail_int)(&zgsz3473);
      CONVERT_OF(sail_int, mach_int)(&zgsz3473, zcap_len_width);
      lbits zgsz3474;
      CREATE(lbits)(&zgsz3474);
      sail_truncate(&zgsz3474, zgaz3222, zgsz3473);
      ztop = CONVERT_OF(fbits, lbits)(zgsz3474, true);
      KILL(lbits)(&zgsz3474);
      KILL(sail_int)(&zgsz3473);
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
          sail_int zgsz3477;
          CREATE(sail_int)(&zgsz3477);
          CONVERT_OF(sail_int, mach_int)(&zgsz3477, zcap_addr_width);
          sail_int zgsz3478;
          CREATE(sail_int)(&zgsz3478);
          CONVERT_OF(sail_int, mach_int)(&zgsz3478, INT64_C(1));
          sail_int zgsz3479;
          CREATE(sail_int)(&zgsz3479);
          sub_int(&zgsz3479, zgsz3477, zgsz3478);
          zgaz3215 = CONVERT_OF(mach_int, sail_int)(zgsz3479);
          KILL(sail_int)(&zgsz3479);
          KILL(sail_int)(&zgsz3478);
          KILL(sail_int)(&zgsz3477);
        }
        {
          lbits zgsz3475;
          CREATE(lbits)(&zgsz3475);
          CONVERT_OF(lbits, fbits)(&zgsz3475, zbase, UINT64_C(33) , true);
          sail_int zgsz3476;
          CREATE(sail_int)(&zgsz3476);
          CONVERT_OF(sail_int, mach_int)(&zgsz3476, zgaz3215);
          zgaz3216 = bitvector_access(zgsz3475, zgsz3476);
          KILL(sail_int)(&zgsz3476);
          KILL(lbits)(&zgsz3475);
        }
      }
      uint64_t zgsz347;
      zgsz347 = UINT64_C(0b0);
      zgsz347 = update_fbits(zgsz347, INT64_C(0), zgaz3216);
      zgaz3217 = zgsz347;
    }
    {
      lbits zgsz3480;
      CREATE(lbits)(&zgsz3480);
      CONVERT_OF(lbits, fbits)(&zgsz3480, UINT64_C(0b0), UINT64_C(1) , true);
      lbits zgsz3481;
      CREATE(lbits)(&zgsz3481);
      CONVERT_OF(lbits, fbits)(&zgsz3481, zgaz3217, UINT64_C(1) , true);
      lbits zgsz3482;
      CREATE(lbits)(&zgsz3482);
      append(&zgsz3482, zgsz3480, zgsz3481);
      zbase2 = CONVERT_OF(fbits, lbits)(zgsz3482, true);
      KILL(lbits)(&zgsz3482);
      KILL(lbits)(&zgsz3481);
      KILL(lbits)(&zgsz3480);
    }
  }
  uint64_t ztop2;
  {
    int64_t zgaz3214;
    {
      sail_int zgsz3487;
      CREATE(sail_int)(&zgsz3487);
      CONVERT_OF(sail_int, mach_int)(&zgsz3487, zcap_addr_width);
      sail_int zgsz3488;
      CREATE(sail_int)(&zgsz3488);
      CONVERT_OF(sail_int, mach_int)(&zgsz3488, INT64_C(1));
      sail_int zgsz3489;
      CREATE(sail_int)(&zgsz3489);
      sub_int(&zgsz3489, zgsz3487, zgsz3488);
      zgaz3214 = CONVERT_OF(mach_int, sail_int)(zgsz3489);
      KILL(sail_int)(&zgsz3489);
      KILL(sail_int)(&zgsz3488);
      KILL(sail_int)(&zgsz3487);
    }
    {
      lbits zgsz3483;
      CREATE(lbits)(&zgsz3483);
      CONVERT_OF(lbits, fbits)(&zgsz3483, ztop, UINT64_C(33) , true);
      sail_int zgsz3484;
      CREATE(sail_int)(&zgsz3484);
      CONVERT_OF(sail_int, mach_int)(&zgsz3484, zcap_addr_width);
      sail_int zgsz3485;
      CREATE(sail_int)(&zgsz3485);
      CONVERT_OF(sail_int, mach_int)(&zgsz3485, zgaz3214);
      lbits zgsz3486;
      CREATE(lbits)(&zgsz3486);
      vector_subrange_lbits(&zgsz3486, zgsz3483, zgsz3484, zgsz3485);
      ztop2 = CONVERT_OF(fbits, lbits)(zgsz3486, true);
      KILL(lbits)(&zgsz3486);
      KILL(sail_int)(&zgsz3485);
      KILL(sail_int)(&zgsz3484);
      KILL(lbits)(&zgsz3483);
    }
  }
  {
    bool zgaz3209;
    {
      bool zgaz3208;
      {
        int64_t zgaz3205;
        {
          sail_int zgsz3492;
          CREATE(sail_int)(&zgsz3492);
          CONVERT_OF(sail_int, mach_int)(&zgsz3492, zcap_max_E);
          sail_int zgsz3493;
          CREATE(sail_int)(&zgsz3493);
          CONVERT_OF(sail_int, mach_int)(&zgsz3493, INT64_C(1));
          sail_int zgsz3494;
          CREATE(sail_int)(&zgsz3494);
          sub_int(&zgsz3494, zgsz3492, zgsz3493);
          zgaz3205 = CONVERT_OF(mach_int, sail_int)(zgsz3494);
          KILL(sail_int)(&zgsz3494);
          KILL(sail_int)(&zgsz3493);
          KILL(sail_int)(&zgsz3492);
        }
        {
          sail_int zgsz3490;
          CREATE(sail_int)(&zgsz3490);
          CONVERT_OF(sail_int, mach_int)(&zgsz3490, zE);
          sail_int zgsz3491;
          CREATE(sail_int)(&zgsz3491);
          CONVERT_OF(sail_int, mach_int)(&zgsz3491, zgaz3205);
          zgaz3208 = lt(zgsz3490, zgsz3491);
          KILL(sail_int)(&zgsz3491);
          KILL(sail_int)(&zgsz3490);
        }
      }
      bool zgsz348;
      if (zgaz3208) {
        int64_t zgaz3207;
        {
          uint64_t zgaz3206;
          {
            lbits zgsz3497;
            CREATE(lbits)(&zgsz3497);
            CONVERT_OF(lbits, fbits)(&zgsz3497, ztop2, UINT64_C(2) , true);
            lbits zgsz3498;
            CREATE(lbits)(&zgsz3498);
            CONVERT_OF(lbits, fbits)(&zgsz3498, zbase2, UINT64_C(2) , true);
            lbits zgsz3499;
            CREATE(lbits)(&zgsz3499);
            sub_bits(&zgsz3499, zgsz3497, zgsz3498);
            zgaz3206 = CONVERT_OF(fbits, lbits)(zgsz3499, true);
            KILL(lbits)(&zgsz3499);
            KILL(lbits)(&zgsz3498);
            KILL(lbits)(&zgsz3497);
          }
          {
            lbits zgsz3495;
            CREATE(lbits)(&zgsz3495);
            CONVERT_OF(lbits, fbits)(&zgsz3495, zgaz3206, UINT64_C(2) , true);
            sail_int zgsz3496;
            CREATE(sail_int)(&zgsz3496);
            sail_unsigned(&zgsz3496, zgsz3495);
            zgaz3207 = CONVERT_OF(mach_int, sail_int)(zgsz3496);
            KILL(sail_int)(&zgsz3496);
            KILL(lbits)(&zgsz3495);
          }
        }
        {
          sail_int zgsz3500;
          CREATE(sail_int)(&zgsz3500);
          CONVERT_OF(sail_int, mach_int)(&zgsz3500, zgaz3207);
          sail_int zgsz3501;
          CREATE(sail_int)(&zgsz3501);
          CONVERT_OF(sail_int, mach_int)(&zgsz3501, INT64_C(1));
          zgsz348 = gt(zgsz3500, zgsz3501);
          KILL(sail_int)(&zgsz3501);
          KILL(sail_int)(&zgsz3500);
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
          lbits zgsz3502;
          CREATE(lbits)(&zgsz3502);
          CONVERT_OF(lbits, fbits)(&zgsz3502, ztop, UINT64_C(33) , true);
          sail_int zgsz3503;
          CREATE(sail_int)(&zgsz3503);
          CONVERT_OF(sail_int, mach_int)(&zgsz3503, zcap_addr_width);
          zgaz3210 = bitvector_access(zgsz3502, zgsz3503);
          KILL(sail_int)(&zgsz3503);
          KILL(lbits)(&zgsz3502);
        }
        zgaz3211 = sailgen_not_bit(zgaz3210);
      }
      {
        lbits zgsz3504;
        CREATE(lbits)(&zgsz3504);
        CONVERT_OF(lbits, fbits)(&zgsz3504, ztop, UINT64_C(33) , true);
        sail_int zgsz3505;
        CREATE(sail_int)(&zgsz3505);
        CONVERT_OF(sail_int, mach_int)(&zgsz3505, zcap_addr_width);
        lbits zgsz3506;
        CREATE(lbits)(&zgsz3506);
        update_lbits(&zgsz3506, zgsz3504, zgsz3505, zgaz3211);
        ztop = CONVERT_OF(fbits, lbits)(zgsz3506, true);
        KILL(lbits)(&zgsz3506);
        KILL(sail_int)(&zgsz3505);
        KILL(lbits)(&zgsz3504);
      }
      zgsz349 = UNIT;
    } else {  zgsz349 = UNIT;  }
  }
  uint64_t zgaz3213;
  {
    int64_t zgaz3212;
    {
      sail_int zgsz3511;
      CREATE(sail_int)(&zgsz3511);
      CONVERT_OF(sail_int, mach_int)(&zgsz3511, zcap_addr_width);
      sail_int zgsz3512;
      CREATE(sail_int)(&zgsz3512);
      CONVERT_OF(sail_int, mach_int)(&zgsz3512, INT64_C(1));
      sail_int zgsz3513;
      CREATE(sail_int)(&zgsz3513);
      sub_int(&zgsz3513, zgsz3511, zgsz3512);
      zgaz3212 = CONVERT_OF(mach_int, sail_int)(zgsz3513);
      KILL(sail_int)(&zgsz3513);
      KILL(sail_int)(&zgsz3512);
      KILL(sail_int)(&zgsz3511);
    }
    {
      lbits zgsz3507;
      CREATE(lbits)(&zgsz3507);
      CONVERT_OF(lbits, fbits)(&zgsz3507, zbase, UINT64_C(33) , true);
      sail_int zgsz3508;
      CREATE(sail_int)(&zgsz3508);
      CONVERT_OF(sail_int, mach_int)(&zgsz3508, zgaz3212);
      sail_int zgsz3509;
      CREATE(sail_int)(&zgsz3509);
      CONVERT_OF(sail_int, mach_int)(&zgsz3509, INT64_C(0));
      lbits zgsz3510;
      CREATE(lbits)(&zgsz3510);
      vector_subrange_lbits(&zgsz3510, zgsz3507, zgsz3508, zgsz3509);
      zgaz3213 = CONVERT_OF(fbits, lbits)(zgsz3510, true);
      KILL(lbits)(&zgsz3510);
      KILL(sail_int)(&zgsz3509);
      KILL(sail_int)(&zgsz3508);
      KILL(lbits)(&zgsz3507);
    }
  }
  struct ztuple_z8z5bv32zCz0z5bv33z9 zgsz350;
  zgsz350.ztup0 = zgaz3213;
  zgsz350.ztup1 = ztop;
  zcbz324 = zgsz350;


















end_function_108: ;
  return zcbz324;
end_block_exception_109: ;
  struct ztuple_z8z5bv32zCz0z5bv33z9 zcbz349 = { .ztup1 = UINT64_C(0xdeadc0de), .ztup0 = UINT64_C(0xdeadc0de) };
  return zcbz349;
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
      lbits zgsz3525;
      CREATE(lbits)(&zgsz3525);
      CONVERT_OF(lbits, fbits)(&zgsz3525, zbase, UINT64_C(32) , true);
      sail_int zgsz3526;
      CREATE(sail_int)(&zgsz3526);
      sail_unsigned(&zgsz3526, zgsz3525);
      zgaz3240 = CONVERT_OF(mach_int, sail_int)(zgsz3526);
      KILL(sail_int)(&zgsz3526);
      KILL(lbits)(&zgsz3525);
    }
    int64_t zgaz3241;
    {
      lbits zgsz3523;
      CREATE(lbits)(&zgsz3523);
      CONVERT_OF(lbits, fbits)(&zgsz3523, ztop, UINT64_C(33) , true);
      sail_int zgsz3524;
      CREATE(sail_int)(&zgsz3524);
      sail_unsigned(&zgsz3524, zgsz3523);
      zgaz3241 = CONVERT_OF(mach_int, sail_int)(zgsz3524);
      KILL(sail_int)(&zgsz3524);
      KILL(lbits)(&zgsz3523);
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
  struct ztuple_z8z5i64zCz0z5i64z9 zcbz350 = { .ztup1 = INT64_C(0xdeadc0de), .ztup0 = INT64_C(0xdeadc0de) };
  return zcbz350;
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
    lbits zgsz3658;
    CREATE(lbits)(&zgsz3658);
    CONVERT_OF(lbits, fbits)(&zgsz3658, UINT64_C(0b0), UINT64_C(1) , true);
    lbits zgsz3659;
    CREATE(lbits)(&zgsz3659);
    CONVERT_OF(lbits, fbits)(&zgsz3659, zbase, UINT64_C(32) , true);
    lbits zgsz3660;
    CREATE(lbits)(&zgsz3660);
    append(&zgsz3660, zgsz3658, zgsz3659);
    zext_base = CONVERT_OF(fbits, lbits)(zgsz3660, true);
    KILL(lbits)(&zgsz3660);
    KILL(lbits)(&zgsz3659);
    KILL(lbits)(&zgsz3658);
  }
  uint64_t zlength;
  {
    lbits zgsz3655;
    CREATE(lbits)(&zgsz3655);
    CONVERT_OF(lbits, fbits)(&zgsz3655, ztop, UINT64_C(33) , true);
    lbits zgsz3656;
    CREATE(lbits)(&zgsz3656);
    CONVERT_OF(lbits, fbits)(&zgsz3656, zext_base, UINT64_C(33) , true);
    lbits zgsz3657;
    CREATE(lbits)(&zgsz3657);
    sub_bits(&zgsz3657, zgsz3655, zgsz3656);
    zlength = CONVERT_OF(fbits, lbits)(zgsz3657, true);
    KILL(lbits)(&zgsz3657);
    KILL(lbits)(&zgsz3656);
    KILL(lbits)(&zgsz3655);
  }
  int64_t ze;
  {
    int64_t zgaz3275;
    {
      uint64_t zgaz3274;
      {
        int64_t zgaz3273;
        {
          sail_int zgsz3531;
          CREATE(sail_int)(&zgsz3531);
          CONVERT_OF(sail_int, mach_int)(&zgsz3531, zcap_mantissa_width);
          sail_int zgsz3532;
          CREATE(sail_int)(&zgsz3532);
          CONVERT_OF(sail_int, mach_int)(&zgsz3532, INT64_C(1));
          sail_int zgsz3533;
          CREATE(sail_int)(&zgsz3533);
          sub_int(&zgsz3533, zgsz3531, zgsz3532);
          zgaz3273 = CONVERT_OF(mach_int, sail_int)(zgsz3533);
          KILL(sail_int)(&zgsz3533);
          KILL(sail_int)(&zgsz3532);
          KILL(sail_int)(&zgsz3531);
        }
        {
          lbits zgsz3527;
          CREATE(lbits)(&zgsz3527);
          CONVERT_OF(lbits, fbits)(&zgsz3527, zlength, UINT64_C(33) , true);
          sail_int zgsz3528;
          CREATE(sail_int)(&zgsz3528);
          CONVERT_OF(sail_int, mach_int)(&zgsz3528, zcap_addr_width);
          sail_int zgsz3529;
          CREATE(sail_int)(&zgsz3529);
          CONVERT_OF(sail_int, mach_int)(&zgsz3529, zgaz3273);
          lbits zgsz3530;
          CREATE(lbits)(&zgsz3530);
          vector_subrange_lbits(&zgsz3530, zgsz3527, zgsz3528, zgsz3529);
          zgaz3274 = CONVERT_OF(fbits, lbits)(zgsz3530, true);
          KILL(lbits)(&zgsz3530);
          KILL(sail_int)(&zgsz3529);
          KILL(sail_int)(&zgsz3528);
          KILL(lbits)(&zgsz3527);
        }
      }
      {
        lbits zgsz3534;
        CREATE(lbits)(&zgsz3534);
        CONVERT_OF(lbits, fbits)(&zgsz3534, zgaz3274, UINT64_C(26) , true);
        sail_int zgsz3535;
        CREATE(sail_int)(&zgsz3535);
        count_leading_zeros(&zgsz3535, zgsz3534);
        zgaz3275 = CONVERT_OF(mach_int, sail_int)(zgsz3535);
        KILL(sail_int)(&zgsz3535);
        KILL(lbits)(&zgsz3534);
      }
    }
    {
      sail_int zgsz3536;
      CREATE(sail_int)(&zgsz3536);
      CONVERT_OF(sail_int, mach_int)(&zgsz3536, zcap_max_E);
      sail_int zgsz3537;
      CREATE(sail_int)(&zgsz3537);
      CONVERT_OF(sail_int, mach_int)(&zgsz3537, zgaz3275);
      sail_int zgsz3538;
      CREATE(sail_int)(&zgsz3538);
      sub_int(&zgsz3538, zgsz3536, zgsz3537);
      ze = CONVERT_OF(mach_int, sail_int)(zgsz3538);
      KILL(sail_int)(&zgsz3538);
      KILL(sail_int)(&zgsz3537);
      KILL(sail_int)(&zgsz3536);
    }
  }
  bool zie;
  {
    bool zgaz3272;
    {
      sail_int zgsz3544;
      CREATE(sail_int)(&zgsz3544);
      CONVERT_OF(sail_int, mach_int)(&zgsz3544, ze);
      sail_int zgsz3545;
      CREATE(sail_int)(&zgsz3545);
      CONVERT_OF(sail_int, mach_int)(&zgsz3545, INT64_C(0));
      zgaz3272 = sailgen_neq_int(zgsz3544, zgsz3545);
      KILL(sail_int)(&zgsz3545);
      KILL(sail_int)(&zgsz3544);
    }
    bool zgsz354;
    if (zgaz3272) {  zgsz354 = true;  } else {
      fbits zgaz3271;
      {
        int64_t zgaz3270;
        {
          sail_int zgsz3541;
          CREATE(sail_int)(&zgsz3541);
          CONVERT_OF(sail_int, mach_int)(&zgsz3541, zcap_mantissa_width);
          sail_int zgsz3542;
          CREATE(sail_int)(&zgsz3542);
          CONVERT_OF(sail_int, mach_int)(&zgsz3542, INT64_C(2));
          sail_int zgsz3543;
          CREATE(sail_int)(&zgsz3543);
          sub_int(&zgsz3543, zgsz3541, zgsz3542);
          zgaz3270 = CONVERT_OF(mach_int, sail_int)(zgsz3543);
          KILL(sail_int)(&zgsz3543);
          KILL(sail_int)(&zgsz3542);
          KILL(sail_int)(&zgsz3541);
        }
        {
          lbits zgsz3539;
          CREATE(lbits)(&zgsz3539);
          CONVERT_OF(lbits, fbits)(&zgsz3539, zlength, UINT64_C(33) , true);
          sail_int zgsz3540;
          CREATE(sail_int)(&zgsz3540);
          CONVERT_OF(sail_int, mach_int)(&zgsz3540, zgaz3270);
          zgaz3271 = bitvector_access(zgsz3539, zgsz3540);
          KILL(sail_int)(&zgsz3540);
          KILL(lbits)(&zgsz3539);
        }
      }
      zgsz354 = eq_bit(zgaz3271, UINT64_C(1));
    }
    zie = zgsz354;
  }
  uint64_t zBbits;
  {
    lbits zgsz3652;
    CREATE(lbits)(&zgsz3652);
    CONVERT_OF(lbits, fbits)(&zgsz3652, zbase, UINT64_C(32) , true);
    sail_int zgsz3653;
    CREATE(sail_int)(&zgsz3653);
    CONVERT_OF(sail_int, mach_int)(&zgsz3653, zcap_mantissa_width);
    lbits zgsz3654;
    CREATE(lbits)(&zgsz3654);
    sail_truncate(&zgsz3654, zgsz3652, zgsz3653);
    zBbits = CONVERT_OF(fbits, lbits)(zgsz3654, true);
    KILL(lbits)(&zgsz3654);
    KILL(sail_int)(&zgsz3653);
    KILL(lbits)(&zgsz3652);
  }
  uint64_t zTbits;
  {
    lbits zgsz3649;
    CREATE(lbits)(&zgsz3649);
    CONVERT_OF(lbits, fbits)(&zgsz3649, ztop, UINT64_C(33) , true);
    sail_int zgsz3650;
    CREATE(sail_int)(&zgsz3650);
    CONVERT_OF(sail_int, mach_int)(&zgsz3650, zcap_mantissa_width);
    lbits zgsz3651;
    CREATE(lbits)(&zgsz3651);
    sail_truncate(&zgsz3651, zgsz3649, zgsz3650);
    zTbits = CONVERT_OF(fbits, lbits)(zgsz3651, true);
    KILL(lbits)(&zgsz3651);
    KILL(sail_int)(&zgsz3650);
    KILL(lbits)(&zgsz3649);
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
            sail_int zgsz3549;
            CREATE(sail_int)(&zgsz3549);
            CONVERT_OF(sail_int, mach_int)(&zgsz3549, ze);
            sail_int zgsz3550;
            CREATE(sail_int)(&zgsz3550);
            CONVERT_OF(sail_int, mach_int)(&zgsz3550, INT64_C(3));
            sail_int zgsz3551;
            CREATE(sail_int)(&zgsz3551);
            add_int(&zgsz3551, zgsz3549, zgsz3550);
            zgaz3264 = CONVERT_OF(mach_int, sail_int)(zgsz3551);
            KILL(sail_int)(&zgsz3551);
            KILL(sail_int)(&zgsz3550);
            KILL(sail_int)(&zgsz3549);
          }
          {
            lbits zgsz3546;
            CREATE(lbits)(&zgsz3546);
            CONVERT_OF(lbits, fbits)(&zgsz3546, zbase, UINT64_C(32) , true);
            sail_int zgsz3547;
            CREATE(sail_int)(&zgsz3547);
            CONVERT_OF(sail_int, mach_int)(&zgsz3547, zgaz3264);
            lbits zgsz3548;
            CREATE(lbits)(&zgsz3548);
            shiftr(&zgsz3548, zgsz3546, zgsz3547);
            zgaz3265 = CONVERT_OF(fbits, lbits)(zgsz3548, true);
            KILL(lbits)(&zgsz3548);
            KILL(sail_int)(&zgsz3547);
            KILL(lbits)(&zgsz3546);
          }
        }
        int64_t zgaz3266;
        {
          sail_int zgsz3555;
          CREATE(sail_int)(&zgsz3555);
          CONVERT_OF(sail_int, mach_int)(&zgsz3555, zcap_mantissa_width);
          sail_int zgsz3556;
          CREATE(sail_int)(&zgsz3556);
          CONVERT_OF(sail_int, mach_int)(&zgsz3556, INT64_C(3));
          sail_int zgsz3557;
          CREATE(sail_int)(&zgsz3557);
          sub_int(&zgsz3557, zgsz3555, zgsz3556);
          zgaz3266 = CONVERT_OF(mach_int, sail_int)(zgsz3557);
          KILL(sail_int)(&zgsz3557);
          KILL(sail_int)(&zgsz3556);
          KILL(sail_int)(&zgsz3555);
        }
        {
          lbits zgsz3552;
          CREATE(lbits)(&zgsz3552);
          CONVERT_OF(lbits, fbits)(&zgsz3552, zgaz3265, UINT64_C(32) , true);
          sail_int zgsz3553;
          CREATE(sail_int)(&zgsz3553);
          CONVERT_OF(sail_int, mach_int)(&zgsz3553, zgaz3266);
          lbits zgsz3554;
          CREATE(lbits)(&zgsz3554);
          sail_truncate(&zgsz3554, zgsz3552, zgsz3553);
          zB_ie = CONVERT_OF(fbits, lbits)(zgsz3554, true);
          KILL(lbits)(&zgsz3554);
          KILL(sail_int)(&zgsz3553);
          KILL(lbits)(&zgsz3552);
        }
      }
      uint64_t zT_ie;
      {
        uint64_t zgaz3262;
        {
          int64_t zgaz3261;
          {
            sail_int zgsz3561;
            CREATE(sail_int)(&zgsz3561);
            CONVERT_OF(sail_int, mach_int)(&zgsz3561, ze);
            sail_int zgsz3562;
            CREATE(sail_int)(&zgsz3562);
            CONVERT_OF(sail_int, mach_int)(&zgsz3562, INT64_C(3));
            sail_int zgsz3563;
            CREATE(sail_int)(&zgsz3563);
            add_int(&zgsz3563, zgsz3561, zgsz3562);
            zgaz3261 = CONVERT_OF(mach_int, sail_int)(zgsz3563);
            KILL(sail_int)(&zgsz3563);
            KILL(sail_int)(&zgsz3562);
            KILL(sail_int)(&zgsz3561);
          }
          {
            lbits zgsz3558;
            CREATE(lbits)(&zgsz3558);
            CONVERT_OF(lbits, fbits)(&zgsz3558, ztop, UINT64_C(33) , true);
            sail_int zgsz3559;
            CREATE(sail_int)(&zgsz3559);
            CONVERT_OF(sail_int, mach_int)(&zgsz3559, zgaz3261);
            lbits zgsz3560;
            CREATE(lbits)(&zgsz3560);
            shiftr(&zgsz3560, zgsz3558, zgsz3559);
            zgaz3262 = CONVERT_OF(fbits, lbits)(zgsz3560, true);
            KILL(lbits)(&zgsz3560);
            KILL(sail_int)(&zgsz3559);
            KILL(lbits)(&zgsz3558);
          }
        }
        int64_t zgaz3263;
        {
          sail_int zgsz3567;
          CREATE(sail_int)(&zgsz3567);
          CONVERT_OF(sail_int, mach_int)(&zgsz3567, zcap_mantissa_width);
          sail_int zgsz3568;
          CREATE(sail_int)(&zgsz3568);
          CONVERT_OF(sail_int, mach_int)(&zgsz3568, INT64_C(3));
          sail_int zgsz3569;
          CREATE(sail_int)(&zgsz3569);
          sub_int(&zgsz3569, zgsz3567, zgsz3568);
          zgaz3263 = CONVERT_OF(mach_int, sail_int)(zgsz3569);
          KILL(sail_int)(&zgsz3569);
          KILL(sail_int)(&zgsz3568);
          KILL(sail_int)(&zgsz3567);
        }
        {
          lbits zgsz3564;
          CREATE(lbits)(&zgsz3564);
          CONVERT_OF(lbits, fbits)(&zgsz3564, zgaz3262, UINT64_C(33) , true);
          sail_int zgsz3565;
          CREATE(sail_int)(&zgsz3565);
          CONVERT_OF(sail_int, mach_int)(&zgsz3565, zgaz3263);
          lbits zgsz3566;
          CREATE(lbits)(&zgsz3566);
          sail_truncate(&zgsz3566, zgsz3564, zgsz3565);
          zT_ie = CONVERT_OF(fbits, lbits)(zgsz3566, true);
          KILL(lbits)(&zgsz3566);
          KILL(sail_int)(&zgsz3565);
          KILL(lbits)(&zgsz3564);
        }
      }
      uint64_t zmaskLo;
      {
        int64_t zgaz3259;
        {
          sail_int zgsz3578;
          CREATE(sail_int)(&zgsz3578);
          CONVERT_OF(sail_int, mach_int)(&zgsz3578, INT64_C(32));
          sail_int zgsz3579;
          CREATE(sail_int)(&zgsz3579);
          CONVERT_OF(sail_int, mach_int)(&zgsz3579, INT64_C(1));
          sail_int zgsz3580;
          CREATE(sail_int)(&zgsz3580);
          add_int(&zgsz3580, zgsz3578, zgsz3579);
          zgaz3259 = CONVERT_OF(mach_int, sail_int)(zgsz3580);
          KILL(sail_int)(&zgsz3580);
          KILL(sail_int)(&zgsz3579);
          KILL(sail_int)(&zgsz3578);
        }
        sbits zgaz3260;
        {
          int64_t zgaz3258;
          {
            sail_int zgsz3572;
            CREATE(sail_int)(&zgsz3572);
            CONVERT_OF(sail_int, mach_int)(&zgsz3572, ze);
            sail_int zgsz3573;
            CREATE(sail_int)(&zgsz3573);
            CONVERT_OF(sail_int, mach_int)(&zgsz3573, INT64_C(3));
            sail_int zgsz3574;
            CREATE(sail_int)(&zgsz3574);
            add_int(&zgsz3574, zgsz3572, zgsz3573);
            zgaz3258 = CONVERT_OF(mach_int, sail_int)(zgsz3574);
            KILL(sail_int)(&zgsz3574);
            KILL(sail_int)(&zgsz3573);
            KILL(sail_int)(&zgsz3572);
          }
          {
            sail_int zgsz3570;
            CREATE(sail_int)(&zgsz3570);
            CONVERT_OF(sail_int, mach_int)(&zgsz3570, zgaz3258);
            lbits zgsz3571;
            CREATE(lbits)(&zgsz3571);
            sailgen_ones(&zgsz3571, zgsz3570);
            zgaz3260 = CONVERT_OF(sbits, lbits)(zgsz3571, true);
            KILL(lbits)(&zgsz3571);
            KILL(sail_int)(&zgsz3570);
          }
        }
        {
          sail_int zgsz3575;
          CREATE(sail_int)(&zgsz3575);
          CONVERT_OF(sail_int, mach_int)(&zgsz3575, zgaz3259);
          lbits zgsz3576;
          CREATE(lbits)(&zgsz3576);
          CONVERT_OF(lbits, sbits)(&zgsz3576, zgaz3260, true);
          lbits zgsz3577;
          CREATE(lbits)(&zgsz3577);
          sailgen_EXTZ(&zgsz3577, zgsz3575, zgsz3576);
          zmaskLo = CONVERT_OF(fbits, lbits)(zgsz3577, true);
          KILL(lbits)(&zgsz3577);
          KILL(lbits)(&zgsz3576);
          KILL(sail_int)(&zgsz3575);
        }
      }
      {
        int64_t zgaz3243;
        {
          uint64_t zgaz3242;
          {
            lbits zgsz3583;
            CREATE(lbits)(&zgsz3583);
            CONVERT_OF(lbits, fbits)(&zgsz3583, zext_base, UINT64_C(33) , true);
            lbits zgsz3584;
            CREATE(lbits)(&zgsz3584);
            CONVERT_OF(lbits, fbits)(&zgsz3584, zmaskLo, UINT64_C(33) , true);
            lbits zgsz3585;
            CREATE(lbits)(&zgsz3585);
            and_bits(&zgsz3585, zgsz3583, zgsz3584);
            zgaz3242 = CONVERT_OF(fbits, lbits)(zgsz3585, true);
            KILL(lbits)(&zgsz3585);
            KILL(lbits)(&zgsz3584);
            KILL(lbits)(&zgsz3583);
          }
          {
            lbits zgsz3581;
            CREATE(lbits)(&zgsz3581);
            CONVERT_OF(lbits, fbits)(&zgsz3581, zgaz3242, UINT64_C(33) , true);
            sail_int zgsz3582;
            CREATE(sail_int)(&zgsz3582);
            sail_unsigned(&zgsz3582, zgsz3581);
            zgaz3243 = CONVERT_OF(mach_int, sail_int)(zgsz3582);
            KILL(sail_int)(&zgsz3582);
            KILL(lbits)(&zgsz3581);
          }
        }
        {
          sail_int zgsz3586;
          CREATE(sail_int)(&zgsz3586);
          CONVERT_OF(sail_int, mach_int)(&zgsz3586, zgaz3243);
          sail_int zgsz3587;
          CREATE(sail_int)(&zgsz3587);
          CONVERT_OF(sail_int, mach_int)(&zgsz3587, INT64_C(0));
          zlostSignificantBase = sailgen_neq_int(zgsz3586, zgsz3587);
          KILL(sail_int)(&zgsz3587);
          KILL(sail_int)(&zgsz3586);
        }
        unit zgsz358;
        zgsz358 = UNIT;
      }
      {
        int64_t zgaz3245;
        {
          uint64_t zgaz3244;
          {
            lbits zgsz3590;
            CREATE(lbits)(&zgsz3590);
            CONVERT_OF(lbits, fbits)(&zgsz3590, ztop, UINT64_C(33) , true);
            lbits zgsz3591;
            CREATE(lbits)(&zgsz3591);
            CONVERT_OF(lbits, fbits)(&zgsz3591, zmaskLo, UINT64_C(33) , true);
            lbits zgsz3592;
            CREATE(lbits)(&zgsz3592);
            and_bits(&zgsz3592, zgsz3590, zgsz3591);
            zgaz3244 = CONVERT_OF(fbits, lbits)(zgsz3592, true);
            KILL(lbits)(&zgsz3592);
            KILL(lbits)(&zgsz3591);
            KILL(lbits)(&zgsz3590);
          }
          {
            lbits zgsz3588;
            CREATE(lbits)(&zgsz3588);
            CONVERT_OF(lbits, fbits)(&zgsz3588, zgaz3244, UINT64_C(33) , true);
            sail_int zgsz3589;
            CREATE(sail_int)(&zgsz3589);
            sail_unsigned(&zgsz3589, zgsz3588);
            zgaz3245 = CONVERT_OF(mach_int, sail_int)(zgsz3589);
            KILL(sail_int)(&zgsz3589);
            KILL(lbits)(&zgsz3588);
          }
        }
        {
          sail_int zgsz3593;
          CREATE(sail_int)(&zgsz3593);
          CONVERT_OF(sail_int, mach_int)(&zgsz3593, zgaz3245);
          sail_int zgsz3594;
          CREATE(sail_int)(&zgsz3594);
          CONVERT_OF(sail_int, mach_int)(&zgsz3594, INT64_C(0));
          zlostSignificantTop = sailgen_neq_int(zgsz3593, zgsz3594);
          KILL(sail_int)(&zgsz3594);
          KILL(sail_int)(&zgsz3593);
        }
        unit zgsz357;
        zgsz357 = UNIT;
      }
      {
        unit zgsz356;
        if (zlostSignificantTop) {
          {
            lbits zgsz3595;
            CREATE(lbits)(&zgsz3595);
            CONVERT_OF(lbits, fbits)(&zgsz3595, zT_ie, UINT64_C(5) , true);
            sail_int zgsz3596;
            CREATE(sail_int)(&zgsz3596);
            CONVERT_OF(sail_int, mach_int)(&zgsz3596, INT64_C(1));
            lbits zgsz3597;
            CREATE(lbits)(&zgsz3597);
            add_bits_int(&zgsz3597, zgsz3595, zgsz3596);
            zT_ie = CONVERT_OF(fbits, lbits)(zgsz3597, true);
            KILL(lbits)(&zgsz3597);
            KILL(sail_int)(&zgsz3596);
            KILL(lbits)(&zgsz3595);
          }
          zgsz356 = UNIT;
        } else {  zgsz356 = UNIT;  }
      }
      uint64_t zlen_ie;
      {
        lbits zgsz3640;
        CREATE(lbits)(&zgsz3640);
        CONVERT_OF(lbits, fbits)(&zgsz3640, zT_ie, UINT64_C(5) , true);
        lbits zgsz3641;
        CREATE(lbits)(&zgsz3641);
        CONVERT_OF(lbits, fbits)(&zgsz3641, zB_ie, UINT64_C(5) , true);
        lbits zgsz3642;
        CREATE(lbits)(&zgsz3642);
        sub_bits(&zgsz3642, zgsz3640, zgsz3641);
        zlen_ie = CONVERT_OF(fbits, lbits)(zgsz3642, true);
        KILL(lbits)(&zgsz3642);
        KILL(lbits)(&zgsz3641);
        KILL(lbits)(&zgsz3640);
      }
      {
        bool zgaz3248;
        {
          fbits zgaz3247;
          {
            int64_t zgaz3246;
            {
              sail_int zgsz3600;
              CREATE(sail_int)(&zgsz3600);
              CONVERT_OF(sail_int, mach_int)(&zgsz3600, zcap_mantissa_width);
              sail_int zgsz3601;
              CREATE(sail_int)(&zgsz3601);
              CONVERT_OF(sail_int, mach_int)(&zgsz3601, INT64_C(4));
              sail_int zgsz3602;
              CREATE(sail_int)(&zgsz3602);
              sub_int(&zgsz3602, zgsz3600, zgsz3601);
              zgaz3246 = CONVERT_OF(mach_int, sail_int)(zgsz3602);
              KILL(sail_int)(&zgsz3602);
              KILL(sail_int)(&zgsz3601);
              KILL(sail_int)(&zgsz3600);
            }
            {
              lbits zgsz3598;
              CREATE(lbits)(&zgsz3598);
              CONVERT_OF(lbits, fbits)(&zgsz3598, zlen_ie, UINT64_C(5) , true);
              sail_int zgsz3599;
              CREATE(sail_int)(&zgsz3599);
              CONVERT_OF(sail_int, mach_int)(&zgsz3599, zgaz3246);
              zgaz3247 = bitvector_access(zgsz3598, zgsz3599);
              KILL(sail_int)(&zgsz3599);
              KILL(lbits)(&zgsz3598);
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
                lbits zgsz3603;
                CREATE(lbits)(&zgsz3603);
                CONVERT_OF(lbits, fbits)(&zgsz3603, zB_ie, UINT64_C(5) , true);
                sail_int zgsz3604;
                CREATE(sail_int)(&zgsz3604);
                CONVERT_OF(sail_int, mach_int)(&zgsz3604, INT64_C(0));
                zgaz3249 = bitvector_access(zgsz3603, zgsz3604);
                KILL(sail_int)(&zgsz3604);
                KILL(lbits)(&zgsz3603);
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
                lbits zgsz3605;
                CREATE(lbits)(&zgsz3605);
                CONVERT_OF(lbits, fbits)(&zgsz3605, zT_ie, UINT64_C(5) , true);
                sail_int zgsz3606;
                CREATE(sail_int)(&zgsz3606);
                CONVERT_OF(sail_int, mach_int)(&zgsz3606, INT64_C(0));
                zgaz3250 = bitvector_access(zgsz3605, zgsz3606);
                KILL(sail_int)(&zgsz3606);
                KILL(lbits)(&zgsz3605);
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
                sail_int zgsz3610;
                CREATE(sail_int)(&zgsz3610);
                CONVERT_OF(sail_int, mach_int)(&zgsz3610, ze);
                sail_int zgsz3611;
                CREATE(sail_int)(&zgsz3611);
                CONVERT_OF(sail_int, mach_int)(&zgsz3611, INT64_C(4));
                sail_int zgsz3612;
                CREATE(sail_int)(&zgsz3612);
                add_int(&zgsz3612, zgsz3610, zgsz3611);
                zgaz3251 = CONVERT_OF(mach_int, sail_int)(zgsz3612);
                KILL(sail_int)(&zgsz3612);
                KILL(sail_int)(&zgsz3611);
                KILL(sail_int)(&zgsz3610);
              }
              {
                lbits zgsz3607;
                CREATE(lbits)(&zgsz3607);
                CONVERT_OF(lbits, fbits)(&zgsz3607, zbase, UINT64_C(32) , true);
                sail_int zgsz3608;
                CREATE(sail_int)(&zgsz3608);
                CONVERT_OF(sail_int, mach_int)(&zgsz3608, zgaz3251);
                lbits zgsz3609;
                CREATE(lbits)(&zgsz3609);
                shiftr(&zgsz3609, zgsz3607, zgsz3608);
                zgaz3252 = CONVERT_OF(fbits, lbits)(zgsz3609, true);
                KILL(lbits)(&zgsz3609);
                KILL(sail_int)(&zgsz3608);
                KILL(lbits)(&zgsz3607);
              }
            }
            int64_t zgaz3253;
            {
              sail_int zgsz3616;
              CREATE(sail_int)(&zgsz3616);
              CONVERT_OF(sail_int, mach_int)(&zgsz3616, zcap_mantissa_width);
              sail_int zgsz3617;
              CREATE(sail_int)(&zgsz3617);
              CONVERT_OF(sail_int, mach_int)(&zgsz3617, INT64_C(3));
              sail_int zgsz3618;
              CREATE(sail_int)(&zgsz3618);
              sub_int(&zgsz3618, zgsz3616, zgsz3617);
              zgaz3253 = CONVERT_OF(mach_int, sail_int)(zgsz3618);
              KILL(sail_int)(&zgsz3618);
              KILL(sail_int)(&zgsz3617);
              KILL(sail_int)(&zgsz3616);
            }
            {
              lbits zgsz3613;
              CREATE(lbits)(&zgsz3613);
              CONVERT_OF(lbits, fbits)(&zgsz3613, zgaz3252, UINT64_C(32) , true);
              sail_int zgsz3614;
              CREATE(sail_int)(&zgsz3614);
              CONVERT_OF(sail_int, mach_int)(&zgsz3614, zgaz3253);
              lbits zgsz3615;
              CREATE(lbits)(&zgsz3615);
              sail_truncate(&zgsz3615, zgsz3613, zgsz3614);
              zB_ie = CONVERT_OF(fbits, lbits)(zgsz3615, true);
              KILL(lbits)(&zgsz3615);
              KILL(sail_int)(&zgsz3614);
              KILL(lbits)(&zgsz3613);
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
                sail_int zgsz3622;
                CREATE(sail_int)(&zgsz3622);
                CONVERT_OF(sail_int, mach_int)(&zgsz3622, ze);
                sail_int zgsz3623;
                CREATE(sail_int)(&zgsz3623);
                CONVERT_OF(sail_int, mach_int)(&zgsz3623, INT64_C(4));
                sail_int zgsz3624;
                CREATE(sail_int)(&zgsz3624);
                add_int(&zgsz3624, zgsz3622, zgsz3623);
                zgaz3254 = CONVERT_OF(mach_int, sail_int)(zgsz3624);
                KILL(sail_int)(&zgsz3624);
                KILL(sail_int)(&zgsz3623);
                KILL(sail_int)(&zgsz3622);
              }
              {
                lbits zgsz3619;
                CREATE(lbits)(&zgsz3619);
                CONVERT_OF(lbits, fbits)(&zgsz3619, ztop, UINT64_C(33) , true);
                sail_int zgsz3620;
                CREATE(sail_int)(&zgsz3620);
                CONVERT_OF(sail_int, mach_int)(&zgsz3620, zgaz3254);
                lbits zgsz3621;
                CREATE(lbits)(&zgsz3621);
                shiftr(&zgsz3621, zgsz3619, zgsz3620);
                zgaz3255 = CONVERT_OF(fbits, lbits)(zgsz3621, true);
                KILL(lbits)(&zgsz3621);
                KILL(sail_int)(&zgsz3620);
                KILL(lbits)(&zgsz3619);
              }
            }
            int64_t zgaz3256;
            {
              sail_int zgsz3628;
              CREATE(sail_int)(&zgsz3628);
              CONVERT_OF(sail_int, mach_int)(&zgsz3628, zcap_mantissa_width);
              sail_int zgsz3629;
              CREATE(sail_int)(&zgsz3629);
              CONVERT_OF(sail_int, mach_int)(&zgsz3629, INT64_C(3));
              sail_int zgsz3630;
              CREATE(sail_int)(&zgsz3630);
              sub_int(&zgsz3630, zgsz3628, zgsz3629);
              zgaz3256 = CONVERT_OF(mach_int, sail_int)(zgsz3630);
              KILL(sail_int)(&zgsz3630);
              KILL(sail_int)(&zgsz3629);
              KILL(sail_int)(&zgsz3628);
            }
            {
              lbits zgsz3625;
              CREATE(lbits)(&zgsz3625);
              CONVERT_OF(lbits, fbits)(&zgsz3625, zgaz3255, UINT64_C(33) , true);
              sail_int zgsz3626;
              CREATE(sail_int)(&zgsz3626);
              CONVERT_OF(sail_int, mach_int)(&zgsz3626, zgaz3256);
              lbits zgsz3627;
              CREATE(lbits)(&zgsz3627);
              sail_truncate(&zgsz3627, zgsz3625, zgsz3626);
              zgaz3257 = CONVERT_OF(fbits, lbits)(zgsz3627, true);
              KILL(lbits)(&zgsz3627);
              KILL(sail_int)(&zgsz3626);
              KILL(lbits)(&zgsz3625);
            }
          }
          {
            lbits zgsz3631;
            CREATE(lbits)(&zgsz3631);
            CONVERT_OF(lbits, fbits)(&zgsz3631, zgaz3257, UINT64_C(5) , true);
            sail_int zgsz3632;
            CREATE(sail_int)(&zgsz3632);
            CONVERT_OF(sail_int, mach_int)(&zgsz3632, zincT);
            lbits zgsz3633;
            CREATE(lbits)(&zgsz3633);
            add_bits_int(&zgsz3633, zgsz3631, zgsz3632);
            zT_ie = CONVERT_OF(fbits, lbits)(zgsz3633, true);
            KILL(lbits)(&zgsz3633);
            KILL(sail_int)(&zgsz3632);
            KILL(lbits)(&zgsz3631);
          }
          zgsz360 = UNIT;
        } else {  zgsz360 = UNIT;  }
      }
      {
        {
          lbits zgsz3634;
          CREATE(lbits)(&zgsz3634);
          CONVERT_OF(lbits, fbits)(&zgsz3634, zB_ie, UINT64_C(5) , true);
          lbits zgsz3635;
          CREATE(lbits)(&zgsz3635);
          CONVERT_OF(lbits, fbits)(&zgsz3635, UINT64_C(0b000), UINT64_C(3) , true);
          lbits zgsz3636;
          CREATE(lbits)(&zgsz3636);
          append(&zgsz3636, zgsz3634, zgsz3635);
          zBbits = CONVERT_OF(fbits, lbits)(zgsz3636, true);
          KILL(lbits)(&zgsz3636);
          KILL(lbits)(&zgsz3635);
          KILL(lbits)(&zgsz3634);
        }
        unit zgsz359;
        zgsz359 = UNIT;
      }
      {
        lbits zgsz3637;
        CREATE(lbits)(&zgsz3637);
        CONVERT_OF(lbits, fbits)(&zgsz3637, zT_ie, UINT64_C(5) , true);
        lbits zgsz3638;
        CREATE(lbits)(&zgsz3638);
        CONVERT_OF(lbits, fbits)(&zgsz3638, UINT64_C(0b000), UINT64_C(3) , true);
        lbits zgsz3639;
        CREATE(lbits)(&zgsz3639);
        append(&zgsz3639, zgsz3637, zgsz3638);
        zTbits = CONVERT_OF(fbits, lbits)(zgsz3639, true);
        KILL(lbits)(&zgsz3639);
        KILL(lbits)(&zgsz3638);
        KILL(lbits)(&zgsz3637);
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
          sail_int zgsz3643;
          CREATE(sail_int)(&zgsz3643);
          CONVERT_OF(sail_int, mach_int)(&zgsz3643, ze);
          sail_int zgsz3644;
          CREATE(sail_int)(&zgsz3644);
          CONVERT_OF(sail_int, mach_int)(&zgsz3644, INT64_C(1));
          sail_int zgsz3645;
          CREATE(sail_int)(&zgsz3645);
          add_int(&zgsz3645, zgsz3643, zgsz3644);
          zgaz3268 = CONVERT_OF(mach_int, sail_int)(zgsz3645);
          KILL(sail_int)(&zgsz3645);
          KILL(sail_int)(&zgsz3644);
          KILL(sail_int)(&zgsz3643);
        }
      } else {  zgaz3268 = ze;  }
      {
        sail_int zgsz3646;
        CREATE(sail_int)(&zgsz3646);
        CONVERT_OF(sail_int, mach_int)(&zgsz3646, INT64_C(6));
        sail_int zgsz3647;
        CREATE(sail_int)(&zgsz3647);
        CONVERT_OF(sail_int, mach_int)(&zgsz3647, zgaz3268);
        lbits zgsz3648;
        CREATE(lbits)(&zgsz3648);
        sailgen_to_bits(&zgsz3648, zgsz3646, zgsz3647);
        zgaz3269 = CONVERT_OF(fbits, lbits)(zgsz3648, true);
        KILL(lbits)(&zgsz3648);
        KILL(sail_int)(&zgsz3647);
        KILL(sail_int)(&zgsz3646);
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
  struct zCapability zcbz352 = { .zuperms = UINT64_C(0xdeadc0de), .ztag = false, .zreserved = UINT64_C(0xdeadc0de), .zpermit_unseal = false, .zpermit_store_local_cap = false, .zpermit_store_cap = false, .zpermit_store = false, .zpermit_set_CID = false, .zpermit_seal = false, .zpermit_load_cap = false, .zpermit_load = false, .zpermit_execute = false, .zpermit_cinvoke = false, .zotype = UINT64_C(0xdeadc0de), .zinternal_E = false, .zglobal = false, .zflag_cap_mode = false, .zaddress = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zT = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz351 = { .ztup1 = zcbz352, .ztup0 = false };
  return zcbz351;
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
      sail_int zgsz3661;
      CREATE(sail_int)(&zgsz3661);
      CONVERT_OF(sail_int, mach_int)(&zgsz3661, INT64_C(15));
      lbits zgsz3662;
      CREATE(lbits)(&zgsz3662);
      CONVERT_OF(lbits, fbits)(&zgsz3662, zgaz3277, UINT64_C(12) , true);
      lbits zgsz3663;
      CREATE(lbits)(&zgsz3663);
      sailgen_EXTZ(&zgsz3663, zgsz3661, zgsz3662);
      zperms = CONVERT_OF(fbits, lbits)(zgsz3663, true);
      KILL(lbits)(&zgsz3663);
      KILL(lbits)(&zgsz3662);
      KILL(sail_int)(&zgsz3661);
    }
  }
  uint64_t zgaz3276;
  zgaz3276 = zcap.zuperms;
  {
    lbits zgsz3664;
    CREATE(lbits)(&zgsz3664);
    CONVERT_OF(lbits, fbits)(&zgsz3664, zgaz3276, UINT64_C(0) , true);
    lbits zgsz3665;
    CREATE(lbits)(&zgsz3665);
    CONVERT_OF(lbits, fbits)(&zgsz3665, zperms, UINT64_C(15) , true);
    lbits zgsz3666;
    CREATE(lbits)(&zgsz3666);
    append(&zgsz3666, zgsz3664, zgsz3665);
    zcbz327 = CONVERT_OF(fbits, lbits)(zgsz3666, true);
    KILL(lbits)(&zgsz3666);
    KILL(lbits)(&zgsz3665);
    KILL(lbits)(&zgsz3664);
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
      lbits zgsz3670;
      CREATE(lbits)(&zgsz3670);
      CONVERT_OF(lbits, fbits)(&zgsz3670, zperms, UINT64_C(15) , true);
      sail_int zgsz3671;
      CREATE(sail_int)(&zgsz3671);
      CONVERT_OF(sail_int, mach_int)(&zgsz3671, zcap_uperms_shift);
      lbits zgsz3672;
      CREATE(lbits)(&zgsz3672);
      shiftr(&zgsz3672, zgsz3670, zgsz3671);
      zgaz3278 = CONVERT_OF(fbits, lbits)(zgsz3672, true);
      KILL(lbits)(&zgsz3672);
      KILL(sail_int)(&zgsz3671);
      KILL(lbits)(&zgsz3670);
    }
    {
      lbits zgsz3667;
      CREATE(lbits)(&zgsz3667);
      CONVERT_OF(lbits, fbits)(&zgsz3667, zgaz3278, UINT64_C(15) , true);
      sail_int zgsz3668;
      CREATE(sail_int)(&zgsz3668);
      CONVERT_OF(sail_int, mach_int)(&zgsz3668, zcap_uperms_width);
      lbits zgsz3669;
      CREATE(lbits)(&zgsz3669);
      sail_truncate(&zgsz3669, zgsz3667, zgsz3668);
      zgaz3279 = CONVERT_OF(fbits, lbits)(zgsz3669, true);
      KILL(lbits)(&zgsz3669);
      KILL(sail_int)(&zgsz3668);
      KILL(lbits)(&zgsz3667);
    }
  }
  bool zgaz3281;
  {
    fbits zgaz3280;
    {
      lbits zgsz3673;
      CREATE(lbits)(&zgsz3673);
      CONVERT_OF(lbits, fbits)(&zgsz3673, zperms, UINT64_C(15) , true);
      sail_int zgsz3674;
      CREATE(sail_int)(&zgsz3674);
      CONVERT_OF(sail_int, mach_int)(&zgsz3674, INT64_C(11));
      zgaz3280 = bitvector_access(zgsz3673, zgsz3674);
      KILL(sail_int)(&zgsz3674);
      KILL(lbits)(&zgsz3673);
    }
    zgaz3281 = sailgen_bit_to_bool(zgaz3280);
  }
  bool zgaz3283;
  {
    fbits zgaz3282;
    {
      lbits zgsz3675;
      CREATE(lbits)(&zgsz3675);
      CONVERT_OF(lbits, fbits)(&zgsz3675, zperms, UINT64_C(15) , true);
      sail_int zgsz3676;
      CREATE(sail_int)(&zgsz3676);
      CONVERT_OF(sail_int, mach_int)(&zgsz3676, INT64_C(10));
      zgaz3282 = bitvector_access(zgsz3675, zgsz3676);
      KILL(sail_int)(&zgsz3676);
      KILL(lbits)(&zgsz3675);
    }
    zgaz3283 = sailgen_bit_to_bool(zgaz3282);
  }
  bool zgaz3285;
  {
    fbits zgaz3284;
    {
      lbits zgsz3677;
      CREATE(lbits)(&zgsz3677);
      CONVERT_OF(lbits, fbits)(&zgsz3677, zperms, UINT64_C(15) , true);
      sail_int zgsz3678;
      CREATE(sail_int)(&zgsz3678);
      CONVERT_OF(sail_int, mach_int)(&zgsz3678, INT64_C(9));
      zgaz3284 = bitvector_access(zgsz3677, zgsz3678);
      KILL(sail_int)(&zgsz3678);
      KILL(lbits)(&zgsz3677);
    }
    zgaz3285 = sailgen_bit_to_bool(zgaz3284);
  }
  bool zgaz3287;
  {
    fbits zgaz3286;
    {
      lbits zgsz3679;
      CREATE(lbits)(&zgsz3679);
      CONVERT_OF(lbits, fbits)(&zgsz3679, zperms, UINT64_C(15) , true);
      sail_int zgsz3680;
      CREATE(sail_int)(&zgsz3680);
      CONVERT_OF(sail_int, mach_int)(&zgsz3680, INT64_C(8));
      zgaz3286 = bitvector_access(zgsz3679, zgsz3680);
      KILL(sail_int)(&zgsz3680);
      KILL(lbits)(&zgsz3679);
    }
    zgaz3287 = sailgen_bit_to_bool(zgaz3286);
  }
  bool zgaz3289;
  {
    fbits zgaz3288;
    {
      lbits zgsz3681;
      CREATE(lbits)(&zgsz3681);
      CONVERT_OF(lbits, fbits)(&zgsz3681, zperms, UINT64_C(15) , true);
      sail_int zgsz3682;
      CREATE(sail_int)(&zgsz3682);
      CONVERT_OF(sail_int, mach_int)(&zgsz3682, INT64_C(7));
      zgaz3288 = bitvector_access(zgsz3681, zgsz3682);
      KILL(sail_int)(&zgsz3682);
      KILL(lbits)(&zgsz3681);
    }
    zgaz3289 = sailgen_bit_to_bool(zgaz3288);
  }
  bool zgaz3291;
  {
    fbits zgaz3290;
    {
      lbits zgsz3683;
      CREATE(lbits)(&zgsz3683);
      CONVERT_OF(lbits, fbits)(&zgsz3683, zperms, UINT64_C(15) , true);
      sail_int zgsz3684;
      CREATE(sail_int)(&zgsz3684);
      CONVERT_OF(sail_int, mach_int)(&zgsz3684, INT64_C(6));
      zgaz3290 = bitvector_access(zgsz3683, zgsz3684);
      KILL(sail_int)(&zgsz3684);
      KILL(lbits)(&zgsz3683);
    }
    zgaz3291 = sailgen_bit_to_bool(zgaz3290);
  }
  bool zgaz3293;
  {
    fbits zgaz3292;
    {
      lbits zgsz3685;
      CREATE(lbits)(&zgsz3685);
      CONVERT_OF(lbits, fbits)(&zgsz3685, zperms, UINT64_C(15) , true);
      sail_int zgsz3686;
      CREATE(sail_int)(&zgsz3686);
      CONVERT_OF(sail_int, mach_int)(&zgsz3686, INT64_C(5));
      zgaz3292 = bitvector_access(zgsz3685, zgsz3686);
      KILL(sail_int)(&zgsz3686);
      KILL(lbits)(&zgsz3685);
    }
    zgaz3293 = sailgen_bit_to_bool(zgaz3292);
  }
  bool zgaz3295;
  {
    fbits zgaz3294;
    {
      lbits zgsz3687;
      CREATE(lbits)(&zgsz3687);
      CONVERT_OF(lbits, fbits)(&zgsz3687, zperms, UINT64_C(15) , true);
      sail_int zgsz3688;
      CREATE(sail_int)(&zgsz3688);
      CONVERT_OF(sail_int, mach_int)(&zgsz3688, INT64_C(4));
      zgaz3294 = bitvector_access(zgsz3687, zgsz3688);
      KILL(sail_int)(&zgsz3688);
      KILL(lbits)(&zgsz3687);
    }
    zgaz3295 = sailgen_bit_to_bool(zgaz3294);
  }
  bool zgaz3297;
  {
    fbits zgaz3296;
    {
      lbits zgsz3689;
      CREATE(lbits)(&zgsz3689);
      CONVERT_OF(lbits, fbits)(&zgsz3689, zperms, UINT64_C(15) , true);
      sail_int zgsz3690;
      CREATE(sail_int)(&zgsz3690);
      CONVERT_OF(sail_int, mach_int)(&zgsz3690, INT64_C(3));
      zgaz3296 = bitvector_access(zgsz3689, zgsz3690);
      KILL(sail_int)(&zgsz3690);
      KILL(lbits)(&zgsz3689);
    }
    zgaz3297 = sailgen_bit_to_bool(zgaz3296);
  }
  bool zgaz3299;
  {
    fbits zgaz3298;
    {
      lbits zgsz3691;
      CREATE(lbits)(&zgsz3691);
      CONVERT_OF(lbits, fbits)(&zgsz3691, zperms, UINT64_C(15) , true);
      sail_int zgsz3692;
      CREATE(sail_int)(&zgsz3692);
      CONVERT_OF(sail_int, mach_int)(&zgsz3692, INT64_C(2));
      zgaz3298 = bitvector_access(zgsz3691, zgsz3692);
      KILL(sail_int)(&zgsz3692);
      KILL(lbits)(&zgsz3691);
    }
    zgaz3299 = sailgen_bit_to_bool(zgaz3298);
  }
  bool zgaz3301;
  {
    fbits zgaz3300;
    {
      lbits zgsz3693;
      CREATE(lbits)(&zgsz3693);
      CONVERT_OF(lbits, fbits)(&zgsz3693, zperms, UINT64_C(15) , true);
      sail_int zgsz3694;
      CREATE(sail_int)(&zgsz3694);
      CONVERT_OF(sail_int, mach_int)(&zgsz3694, INT64_C(1));
      zgaz3300 = bitvector_access(zgsz3693, zgsz3694);
      KILL(sail_int)(&zgsz3694);
      KILL(lbits)(&zgsz3693);
    }
    zgaz3301 = sailgen_bit_to_bool(zgaz3300);
  }
  bool zgaz3303;
  {
    fbits zgaz3302;
    {
      lbits zgsz3695;
      CREATE(lbits)(&zgsz3695);
      CONVERT_OF(lbits, fbits)(&zgsz3695, zperms, UINT64_C(15) , true);
      sail_int zgsz3696;
      CREATE(sail_int)(&zgsz3696);
      CONVERT_OF(sail_int, mach_int)(&zgsz3696, INT64_C(0));
      zgaz3302 = bitvector_access(zgsz3695, zgsz3696);
      KILL(sail_int)(&zgsz3696);
      KILL(lbits)(&zgsz3695);
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
  struct zCapability zcbz353 = { .zuperms = UINT64_C(0xdeadc0de), .ztag = false, .zreserved = UINT64_C(0xdeadc0de), .zpermit_unseal = false, .zpermit_store_local_cap = false, .zpermit_store_cap = false, .zpermit_store = false, .zpermit_set_CID = false, .zpermit_seal = false, .zpermit_load_cap = false, .zpermit_load = false, .zpermit_execute = false, .zpermit_cinvoke = false, .zotype = UINT64_C(0xdeadc0de), .zinternal_E = false, .zglobal = false, .zflag_cap_mode = false, .zaddress = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zT = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  return zcbz353;
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
      lbits zgsz3697;
      CREATE(lbits)(&zgsz3697);
      CONVERT_OF(lbits, fbits)(&zgsz3697, zgaz3305, UINT64_C(4) , true);
      sail_int zgsz3698;
      CREATE(sail_int)(&zgsz3698);
      sail_signed(&zgsz3698, zgsz3697);
      zgaz3306 = CONVERT_OF(mach_int, sail_int)(zgsz3698);
      KILL(sail_int)(&zgsz3698);
      KILL(lbits)(&zgsz3697);
    }
  }
  {
    sail_int zgsz3699;
    CREATE(sail_int)(&zgsz3699);
    CONVERT_OF(sail_int, mach_int)(&zgsz3699, zgaz3306);
    sail_int zgsz3700;
    CREATE(sail_int)(&zgsz3700);
    CONVERT_OF(sail_int, mach_int)(&zgsz3700, zotype_unsealed);
    zcbz330 = sailgen_neq_int(zgsz3699, zgsz3700);
    KILL(sail_int)(&zgsz3700);
    KILL(sail_int)(&zgsz3699);
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
      lbits zgsz3701;
      CREATE(lbits)(&zgsz3701);
      CONVERT_OF(lbits, fbits)(&zgsz3701, zgaz3307, UINT64_C(4) , true);
      sail_int zgsz3702;
      CREATE(sail_int)(&zgsz3702);
      sail_unsigned(&zgsz3702, zgsz3701);
      zgaz3308 = CONVERT_OF(mach_int, sail_int)(zgsz3702);
      KILL(sail_int)(&zgsz3702);
      KILL(lbits)(&zgsz3701);
    }
  }
  {
    sail_int zgsz3703;
    CREATE(sail_int)(&zgsz3703);
    CONVERT_OF(sail_int, mach_int)(&zgsz3703, zgaz3308);
    sail_int zgsz3704;
    CREATE(sail_int)(&zgsz3704);
    CONVERT_OF(sail_int, mach_int)(&zgsz3704, zcap_max_otype);
    zcbz331 = gt(zgsz3703, zgsz3704);
    KILL(sail_int)(&zgsz3704);
    KILL(sail_int)(&zgsz3703);
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
    lbits zgsz3705;
    CREATE(lbits)(&zgsz3705);
    CONVERT_OF(lbits, fbits)(&zgsz3705, zgaz3310, UINT64_C(32) , true);
    lbits zgsz3706;
    CREATE(lbits)(&zgsz3706);
    CONVERT_OF(lbits, fbits)(&zgsz3706, zbase, UINT64_C(32) , true);
    lbits zgsz3707;
    CREATE(lbits)(&zgsz3707);
    sub_bits(&zgsz3707, zgsz3705, zgsz3706);
    zcbz333 = CONVERT_OF(fbits, lbits)(zgsz3707, true);
    KILL(lbits)(&zgsz3707);
    KILL(lbits)(&zgsz3706);
    KILL(lbits)(&zgsz3705);
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
            sail_int zgsz3708;
            CREATE(sail_int)(&zgsz3708);
            CONVERT_OF(sail_int, mach_int)(&zgsz3708, ztop);
            sail_int zgsz3709;
            CREATE(sail_int)(&zgsz3709);
            CONVERT_OF(sail_int, mach_int)(&zgsz3709, zbase);
            zgsz374 = gteq(zgsz3708, zgsz3709);
            KILL(sail_int)(&zgsz3709);
            KILL(sail_int)(&zgsz3708);
          }
        }
        zgaz3314 = zgsz374;
      }
      unit zgsz375;
      zgsz375 = sail_assert(zgaz3314, "src/cheri_cap_common.sail:453.40-453.41");
    }
    int64_t zgaz3315;
    {
      sail_int zgsz3715;
      CREATE(sail_int)(&zgsz3715);
      CONVERT_OF(sail_int, mach_int)(&zgsz3715, ztop);
      sail_int zgsz3716;
      CREATE(sail_int)(&zgsz3716);
      CONVERT_OF(sail_int, mach_int)(&zgsz3716, zbase);
      sail_int zgsz3717;
      CREATE(sail_int)(&zgsz3717);
      sub_int(&zgsz3717, zgsz3715, zgsz3716);
      zgaz3315 = CONVERT_OF(mach_int, sail_int)(zgsz3717);
      KILL(sail_int)(&zgsz3717);
      KILL(sail_int)(&zgsz3716);
      KILL(sail_int)(&zgsz3715);
    }
    int64_t zgaz3316;
    {
      sail_int zgsz3713;
      CREATE(sail_int)(&zgsz3713);
      CONVERT_OF(sail_int, mach_int)(&zgsz3713, zcap_len_width);
      sail_int zgsz3714;
      CREATE(sail_int)(&zgsz3714);
      pow2(&zgsz3714, zgsz3713);
      zgaz3316 = CONVERT_OF(mach_int, sail_int)(zgsz3714);
      KILL(sail_int)(&zgsz3714);
      KILL(sail_int)(&zgsz3713);
    }
    {
      sail_int zgsz3710;
      CREATE(sail_int)(&zgsz3710);
      CONVERT_OF(sail_int, mach_int)(&zgsz3710, zgaz3315);
      sail_int zgsz3711;
      CREATE(sail_int)(&zgsz3711);
      CONVERT_OF(sail_int, mach_int)(&zgsz3711, zgaz3316);
      sail_int zgsz3712;
      CREATE(sail_int)(&zgsz3712);
      emod_int(&zgsz3712, zgsz3710, zgsz3711);
      zgsz373 = CONVERT_OF(mach_int, sail_int)(zgsz3712);
      KILL(sail_int)(&zgsz3712);
      KILL(sail_int)(&zgsz3711);
      KILL(sail_int)(&zgsz3710);
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

static bool sailgen_capBoundsEqual(struct zCapability, struct zCapability);

static bool sailgen_capBoundsEqual(struct zCapability zc1, struct zCapability zc2)
{
  __label__ case_148, finish_match_147, end_function_151, end_block_exception_152;

  bool zcbz335;
  struct ztuple_z8z5i64zCz0z5i64z9 zgaz3317;
  zgaz3317 = sailgen_getCapBounds(zc1);
  bool zgsz377;
  {
    __label__ case_150, finish_match_149;

    int64_t zbase1;
    zbase1 = zgaz3317.ztup0;
    int64_t ztop1;
    ztop1 = zgaz3317.ztup1;
    struct ztuple_z8z5i64zCz0z5i64z9 zgaz3318;
    zgaz3318 = sailgen_getCapBounds(zc2);
    bool zgsz378;
    {
      int64_t zbase2;
      zbase2 = zgaz3318.ztup0;
      int64_t ztop2;
      ztop2 = zgaz3318.ztup1;
      bool zgaz3319;
      {
        sail_int zgsz3720;
        CREATE(sail_int)(&zgsz3720);
        CONVERT_OF(sail_int, mach_int)(&zgsz3720, zbase1);
        sail_int zgsz3721;
        CREATE(sail_int)(&zgsz3721);
        CONVERT_OF(sail_int, mach_int)(&zgsz3721, zbase2);
        zgaz3319 = eq_int(zgsz3720, zgsz3721);
        KILL(sail_int)(&zgsz3721);
        KILL(sail_int)(&zgsz3720);
      }
      bool zgsz379;
      if (zgaz3319) {
        {
          sail_int zgsz3718;
          CREATE(sail_int)(&zgsz3718);
          CONVERT_OF(sail_int, mach_int)(&zgsz3718, ztop1);
          sail_int zgsz3719;
          CREATE(sail_int)(&zgsz3719);
          CONVERT_OF(sail_int, mach_int)(&zgsz3719, ztop2);
          zgsz379 = eq_int(zgsz3718, zgsz3719);
          KILL(sail_int)(&zgsz3719);
          KILL(sail_int)(&zgsz3718);
        }
      } else {  zgsz379 = false;  }
      zgsz378 = zgsz379;
      goto finish_match_149;
    }
  case_150: ;
    sail_match_failure("capBoundsEqual");
  finish_match_149: ;
    zgsz377 = zgsz378;
    goto finish_match_147;
  }
case_148: ;
  sail_match_failure("capBoundsEqual");
finish_match_147: ;
  zcbz335 = zgsz377;


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
  struct zCapability zcbz355 = { .zuperms = UINT64_C(0xdeadc0de), .ztag = false, .zreserved = UINT64_C(0xdeadc0de), .zpermit_unseal = false, .zpermit_store_local_cap = false, .zpermit_store_cap = false, .zpermit_store = false, .zpermit_set_CID = false, .zpermit_seal = false, .zpermit_load_cap = false, .zpermit_load = false, .zpermit_execute = false, .zpermit_cinvoke = false, .zotype = UINT64_C(0xdeadc0de), .zinternal_E = false, .zglobal = false, .zflag_cap_mode = false, .zaddress = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zT = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zB = UINT64_C(0xdeadc0de) };
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz354 = { .ztup1 = zcbz355, .ztup0 = false };
  return zcbz354;
}

static bool sailgen_fastRepCheck(struct zCapability, uint64_t);

static bool sailgen_fastRepCheck(struct zCapability zc, uint64_t zi)
{
  __label__ end_function_157, end_block_exception_158;

  bool zcbz337;
  int64_t zE;
  {
    uint64_t zgaz3333;
    zgaz3333 = zc.zE;
    {
      lbits zgsz3722;
      CREATE(lbits)(&zgsz3722);
      CONVERT_OF(lbits, fbits)(&zgsz3722, zgaz3333, UINT64_C(6) , true);
      sail_int zgsz3723;
      CREATE(sail_int)(&zgsz3723);
      sail_unsigned(&zgsz3723, zgsz3722);
      zE = CONVERT_OF(mach_int, sail_int)(zgsz3723);
      KILL(sail_int)(&zgsz3723);
      KILL(lbits)(&zgsz3722);
    }
  }
  bool zgaz3321;
  {
    int64_t zgaz3320;
    {
      sail_int zgsz3726;
      CREATE(sail_int)(&zgsz3726);
      CONVERT_OF(sail_int, mach_int)(&zgsz3726, zcap_max_E);
      sail_int zgsz3727;
      CREATE(sail_int)(&zgsz3727);
      CONVERT_OF(sail_int, mach_int)(&zgsz3727, INT64_C(2));
      sail_int zgsz3728;
      CREATE(sail_int)(&zgsz3728);
      sub_int(&zgsz3728, zgsz3726, zgsz3727);
      zgaz3320 = CONVERT_OF(mach_int, sail_int)(zgsz3728);
      KILL(sail_int)(&zgsz3728);
      KILL(sail_int)(&zgsz3727);
      KILL(sail_int)(&zgsz3726);
    }
    {
      sail_int zgsz3724;
      CREATE(sail_int)(&zgsz3724);
      CONVERT_OF(sail_int, mach_int)(&zgsz3724, zE);
      sail_int zgsz3725;
      CREATE(sail_int)(&zgsz3725);
      CONVERT_OF(sail_int, mach_int)(&zgsz3725, zgaz3320);
      zgaz3321 = gteq(zgsz3724, zgsz3725);
      KILL(sail_int)(&zgsz3725);
      KILL(sail_int)(&zgsz3724);
    }
  }
  if (zgaz3321) {  zcbz337 = true;  } else {
    int64_t zi_top;
    {
      uint64_t zgaz3332;
      {
        int64_t zgaz3331;
        {
          sail_int zgsz3732;
          CREATE(sail_int)(&zgsz3732);
          CONVERT_OF(sail_int, mach_int)(&zgsz3732, zE);
          sail_int zgsz3733;
          CREATE(sail_int)(&zgsz3733);
          CONVERT_OF(sail_int, mach_int)(&zgsz3733, zcap_mantissa_width);
          sail_int zgsz3734;
          CREATE(sail_int)(&zgsz3734);
          add_int(&zgsz3734, zgsz3732, zgsz3733);
          zgaz3331 = CONVERT_OF(mach_int, sail_int)(zgsz3734);
          KILL(sail_int)(&zgsz3734);
          KILL(sail_int)(&zgsz3733);
          KILL(sail_int)(&zgsz3732);
        }
        {
          lbits zgsz3729;
          CREATE(lbits)(&zgsz3729);
          CONVERT_OF(lbits, fbits)(&zgsz3729, zi, UINT64_C(32) , true);
          sail_int zgsz3730;
          CREATE(sail_int)(&zgsz3730);
          CONVERT_OF(sail_int, mach_int)(&zgsz3730, zgaz3331);
          lbits zgsz3731;
          CREATE(lbits)(&zgsz3731);
          arith_shiftr(&zgsz3731, zgsz3729, zgsz3730);
          zgaz3332 = CONVERT_OF(fbits, lbits)(zgsz3731, true);
          KILL(lbits)(&zgsz3731);
          KILL(sail_int)(&zgsz3730);
          KILL(lbits)(&zgsz3729);
        }
      }
      {
        lbits zgsz3735;
        CREATE(lbits)(&zgsz3735);
        CONVERT_OF(lbits, fbits)(&zgsz3735, zgaz3332, UINT64_C(32) , true);
        sail_int zgsz3736;
        CREATE(sail_int)(&zgsz3736);
        sail_signed(&zgsz3736, zgsz3735);
        zi_top = CONVERT_OF(mach_int, sail_int)(zgsz3736);
        KILL(sail_int)(&zgsz3736);
        KILL(lbits)(&zgsz3735);
      }
    }
    uint64_t zi_mid;
    {
      uint64_t zgaz3330;
      {
        lbits zgsz3740;
        CREATE(lbits)(&zgsz3740);
        CONVERT_OF(lbits, fbits)(&zgsz3740, zi, UINT64_C(32) , true);
        sail_int zgsz3741;
        CREATE(sail_int)(&zgsz3741);
        CONVERT_OF(sail_int, mach_int)(&zgsz3741, zE);
        lbits zgsz3742;
        CREATE(lbits)(&zgsz3742);
        shiftr(&zgsz3742, zgsz3740, zgsz3741);
        zgaz3330 = CONVERT_OF(fbits, lbits)(zgsz3742, true);
        KILL(lbits)(&zgsz3742);
        KILL(sail_int)(&zgsz3741);
        KILL(lbits)(&zgsz3740);
      }
      {
        lbits zgsz3737;
        CREATE(lbits)(&zgsz3737);
        CONVERT_OF(lbits, fbits)(&zgsz3737, zgaz3330, UINT64_C(32) , true);
        sail_int zgsz3738;
        CREATE(sail_int)(&zgsz3738);
        CONVERT_OF(sail_int, mach_int)(&zgsz3738, zcap_mantissa_width);
        lbits zgsz3739;
        CREATE(lbits)(&zgsz3739);
        sail_truncate(&zgsz3739, zgsz3737, zgsz3738);
        zi_mid = CONVERT_OF(fbits, lbits)(zgsz3739, true);
        KILL(lbits)(&zgsz3739);
        KILL(sail_int)(&zgsz3738);
        KILL(lbits)(&zgsz3737);
      }
    }
    uint64_t za_mid;
    {
      uint64_t zgaz3329;
      {
        uint64_t zgaz3328;
        zgaz3328 = zc.zaddress;
        {
          lbits zgsz3743;
          CREATE(lbits)(&zgsz3743);
          CONVERT_OF(lbits, fbits)(&zgsz3743, zgaz3328, UINT64_C(32) , true);
          sail_int zgsz3744;
          CREATE(sail_int)(&zgsz3744);
          CONVERT_OF(sail_int, mach_int)(&zgsz3744, zE);
          lbits zgsz3745;
          CREATE(lbits)(&zgsz3745);
          shiftr(&zgsz3745, zgsz3743, zgsz3744);
          zgaz3329 = CONVERT_OF(fbits, lbits)(zgsz3745, true);
          KILL(lbits)(&zgsz3745);
          KILL(sail_int)(&zgsz3744);
          KILL(lbits)(&zgsz3743);
        }
      }
      {
        lbits zgsz3746;
        CREATE(lbits)(&zgsz3746);
        CONVERT_OF(lbits, fbits)(&zgsz3746, zgaz3329, UINT64_C(32) , true);
        sail_int zgsz3747;
        CREATE(sail_int)(&zgsz3747);
        CONVERT_OF(sail_int, mach_int)(&zgsz3747, zcap_mantissa_width);
        lbits zgsz3748;
        CREATE(lbits)(&zgsz3748);
        sail_truncate(&zgsz3748, zgsz3746, zgsz3747);
        za_mid = CONVERT_OF(fbits, lbits)(zgsz3748, true);
        KILL(lbits)(&zgsz3748);
        KILL(sail_int)(&zgsz3747);
        KILL(lbits)(&zgsz3746);
      }
    }
    uint64_t zB3;
    {
      uint64_t zgaz3327;
      zgaz3327 = zc.zB;
      {
        lbits zgsz3749;
        CREATE(lbits)(&zgsz3749);
        CONVERT_OF(lbits, fbits)(&zgsz3749, zgaz3327, UINT64_C(8) , true);
        sail_int zgsz3750;
        CREATE(sail_int)(&zgsz3750);
        CONVERT_OF(sail_int, mach_int)(&zgsz3750, INT64_C(3));
        lbits zgsz3751;
        CREATE(lbits)(&zgsz3751);
        sail_truncateLSB(&zgsz3751, zgsz3749, zgsz3750);
        zB3 = CONVERT_OF(fbits, lbits)(zgsz3751, true);
        KILL(lbits)(&zgsz3751);
        KILL(sail_int)(&zgsz3750);
        KILL(lbits)(&zgsz3749);
      }
    }
    uint64_t zR3;
    {
      lbits zgsz3776;
      CREATE(lbits)(&zgsz3776);
      CONVERT_OF(lbits, fbits)(&zgsz3776, zB3, UINT64_C(3) , true);
      lbits zgsz3777;
      CREATE(lbits)(&zgsz3777);
      CONVERT_OF(lbits, fbits)(&zgsz3777, UINT64_C(0b001), UINT64_C(3) , true);
      lbits zgsz3778;
      CREATE(lbits)(&zgsz3778);
      sub_bits(&zgsz3778, zgsz3776, zgsz3777);
      zR3 = CONVERT_OF(fbits, lbits)(zgsz3778, true);
      KILL(lbits)(&zgsz3778);
      KILL(lbits)(&zgsz3777);
      KILL(lbits)(&zgsz3776);
    }
    uint64_t zR;
    {
      uint64_t zgaz3326;
      {
        int64_t zgaz3325;
        {
          sail_int zgsz3754;
          CREATE(sail_int)(&zgsz3754);
          CONVERT_OF(sail_int, mach_int)(&zgsz3754, zcap_mantissa_width);
          sail_int zgsz3755;
          CREATE(sail_int)(&zgsz3755);
          CONVERT_OF(sail_int, mach_int)(&zgsz3755, INT64_C(3));
          sail_int zgsz3756;
          CREATE(sail_int)(&zgsz3756);
          sub_int(&zgsz3756, zgsz3754, zgsz3755);
          zgaz3325 = CONVERT_OF(mach_int, sail_int)(zgsz3756);
          KILL(sail_int)(&zgsz3756);
          KILL(sail_int)(&zgsz3755);
          KILL(sail_int)(&zgsz3754);
        }
        {
          sail_int zgsz3752;
          CREATE(sail_int)(&zgsz3752);
          CONVERT_OF(sail_int, mach_int)(&zgsz3752, zgaz3325);
          lbits zgsz3753;
          CREATE(lbits)(&zgsz3753);
          sailgen_zzeros_implicit(&zgsz3753, zgsz3752);
          zgaz3326 = CONVERT_OF(fbits, lbits)(zgsz3753, true);
          KILL(lbits)(&zgsz3753);
          KILL(sail_int)(&zgsz3752);
        }
      }
      {
        lbits zgsz3757;
        CREATE(lbits)(&zgsz3757);
        CONVERT_OF(lbits, fbits)(&zgsz3757, zR3, UINT64_C(3) , true);
        lbits zgsz3758;
        CREATE(lbits)(&zgsz3758);
        CONVERT_OF(lbits, fbits)(&zgsz3758, zgaz3326, UINT64_C(5) , true);
        lbits zgsz3759;
        CREATE(lbits)(&zgsz3759);
        append(&zgsz3759, zgsz3757, zgsz3758);
        zR = CONVERT_OF(fbits, lbits)(zgsz3759, true);
        KILL(lbits)(&zgsz3759);
        KILL(lbits)(&zgsz3758);
        KILL(lbits)(&zgsz3757);
      }
    }
    uint64_t zdiff;
    {
      lbits zgsz3773;
      CREATE(lbits)(&zgsz3773);
      CONVERT_OF(lbits, fbits)(&zgsz3773, zR, UINT64_C(8) , true);
      lbits zgsz3774;
      CREATE(lbits)(&zgsz3774);
      CONVERT_OF(lbits, fbits)(&zgsz3774, za_mid, UINT64_C(8) , true);
      lbits zgsz3775;
      CREATE(lbits)(&zgsz3775);
      sub_bits(&zgsz3775, zgsz3773, zgsz3774);
      zdiff = CONVERT_OF(fbits, lbits)(zgsz3775, true);
      KILL(lbits)(&zgsz3775);
      KILL(lbits)(&zgsz3774);
      KILL(lbits)(&zgsz3773);
    }
    uint64_t zdiff1;
    {
      lbits zgsz3770;
      CREATE(lbits)(&zgsz3770);
      CONVERT_OF(lbits, fbits)(&zgsz3770, zdiff, UINT64_C(8) , true);
      sail_int zgsz3771;
      CREATE(sail_int)(&zgsz3771);
      CONVERT_OF(sail_int, mach_int)(&zgsz3771, INT64_C(1));
      lbits zgsz3772;
      CREATE(lbits)(&zgsz3772);
      sub_bits_int(&zgsz3772, zgsz3770, zgsz3771);
      zdiff1 = CONVERT_OF(fbits, lbits)(zgsz3772, true);
      KILL(lbits)(&zgsz3772);
      KILL(sail_int)(&zgsz3771);
      KILL(lbits)(&zgsz3770);
    }
    bool zgaz3322;
    {
      sail_int zgsz3768;
      CREATE(sail_int)(&zgsz3768);
      CONVERT_OF(sail_int, mach_int)(&zgsz3768, zi_top);
      sail_int zgsz3769;
      CREATE(sail_int)(&zgsz3769);
      CONVERT_OF(sail_int, mach_int)(&zgsz3769, INT64_C(0));
      zgaz3322 = eq_int(zgsz3768, zgsz3769);
      KILL(sail_int)(&zgsz3769);
      KILL(sail_int)(&zgsz3768);
    }
    if (zgaz3322) {
      {
        lbits zgsz3766;
        CREATE(lbits)(&zgsz3766);
        CONVERT_OF(lbits, fbits)(&zgsz3766, zi_mid, UINT64_C(8) , true);
        lbits zgsz3767;
        CREATE(lbits)(&zgsz3767);
        CONVERT_OF(lbits, fbits)(&zgsz3767, zdiff1, UINT64_C(8) , true);
        zcbz337 = sailgen_z8operatorz0zI_uz9(zgsz3766, zgsz3767);
        KILL(lbits)(&zgsz3767);
        KILL(lbits)(&zgsz3766);
      }
    } else {
      bool zgaz3323;
      {
        sail_int zgsz3764;
        CREATE(sail_int)(&zgsz3764);
        CONVERT_OF(sail_int, mach_int)(&zgsz3764, zi_top);
        sail_int zgsz3765;
        CREATE(sail_int)(&zgsz3765);
        CONVERT_OF(sail_int, mach_int)(&zgsz3765, INT64_C(-1));
        zgaz3323 = eq_int(zgsz3764, zgsz3765);
        KILL(sail_int)(&zgsz3765);
        KILL(sail_int)(&zgsz3764);
      }
      if (zgaz3323) {
        bool zgaz3324;
        {
          lbits zgsz3762;
          CREATE(lbits)(&zgsz3762);
          CONVERT_OF(lbits, fbits)(&zgsz3762, zi_mid, UINT64_C(8) , true);
          lbits zgsz3763;
          CREATE(lbits)(&zgsz3763);
          CONVERT_OF(lbits, fbits)(&zgsz3763, zdiff, UINT64_C(8) , true);
          zgaz3324 = sailgen_z8operatorz0zKzJ_uz9(zgsz3762, zgsz3763);
          KILL(lbits)(&zgsz3763);
          KILL(lbits)(&zgsz3762);
        }
        bool zgsz384;
        if (zgaz3324) {
          {
            lbits zgsz3760;
            CREATE(lbits)(&zgsz3760);
            CONVERT_OF(lbits, fbits)(&zgsz3760, zR, UINT64_C(8) , true);
            lbits zgsz3761;
            CREATE(lbits)(&zgsz3761);
            CONVERT_OF(lbits, fbits)(&zgsz3761, za_mid, UINT64_C(8) , true);
            zgsz384 = neq_bits(zgsz3760, zgsz3761);
            KILL(lbits)(&zgsz3761);
            KILL(lbits)(&zgsz3760);
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

  int64_t zlen;
  zlen = sailgen_getCapLength(zcap);
  sail_string zlen_str;
  CREATE(sail_string)(&zlen_str);
  {
    uint64_t zgaz3362;
    {
      int64_t zgaz3361;
      {
        sail_int zgsz3782;
        CREATE(sail_int)(&zgsz3782);
        CONVERT_OF(sail_int, mach_int)(&zgsz3782, zcap_len_width);
        sail_int zgsz3783;
        CREATE(sail_int)(&zgsz3783);
        CONVERT_OF(sail_int, mach_int)(&zgsz3783, INT64_C(3));
        sail_int zgsz3784;
        CREATE(sail_int)(&zgsz3784);
        add_int(&zgsz3784, zgsz3782, zgsz3783);
        zgaz3361 = CONVERT_OF(mach_int, sail_int)(zgsz3784);
        KILL(sail_int)(&zgsz3784);
        KILL(sail_int)(&zgsz3783);
        KILL(sail_int)(&zgsz3782);
      }
      {
        sail_int zgsz3779;
        CREATE(sail_int)(&zgsz3779);
        CONVERT_OF(sail_int, mach_int)(&zgsz3779, zgaz3361);
        sail_int zgsz3780;
        CREATE(sail_int)(&zgsz3780);
        CONVERT_OF(sail_int, mach_int)(&zgsz3780, zlen);
        lbits zgsz3781;
        CREATE(lbits)(&zgsz3781);
        sailgen_to_bits(&zgsz3781, zgsz3779, zgsz3780);
        zgaz3362 = CONVERT_OF(fbits, lbits)(zgsz3781, true);
        KILL(lbits)(&zgsz3781);
        KILL(sail_int)(&zgsz3780);
        KILL(sail_int)(&zgsz3779);
      }
    }
    {
      lbits zgsz3785;
      CREATE(lbits)(&zgsz3785);
      CONVERT_OF(lbits, fbits)(&zgsz3785, zgaz3362, UINT64_C(36) , true);
      string_of_lbits(&zlen_str, zgsz3785);
      KILL(lbits)(&zgsz3785);
    }
  }
  uint64_t zotype64;
  {
    bool zgaz3358;
    zgaz3358 = sailgen_hasReservedOType(zcap);
    if (zgaz3358) {
      uint64_t zgaz3359;
      zgaz3359 = zcap.zotype;
      {
        sail_int zgsz3789;
        CREATE(sail_int)(&zgsz3789);
        CONVERT_OF(sail_int, mach_int)(&zgsz3789, INT64_C(32));
        lbits zgsz3790;
        CREATE(lbits)(&zgsz3790);
        CONVERT_OF(lbits, fbits)(&zgsz3790, zgaz3359, UINT64_C(4) , true);
        lbits zgsz3791;
        CREATE(lbits)(&zgsz3791);
        sailgen_EXTS(&zgsz3791, zgsz3789, zgsz3790);
        zotype64 = CONVERT_OF(fbits, lbits)(zgsz3791, true);
        KILL(lbits)(&zgsz3791);
        KILL(lbits)(&zgsz3790);
        KILL(sail_int)(&zgsz3789);
      }
    } else {
      uint64_t zgaz3360;
      zgaz3360 = zcap.zotype;
      {
        sail_int zgsz3786;
        CREATE(sail_int)(&zgsz3786);
        CONVERT_OF(sail_int, mach_int)(&zgsz3786, INT64_C(32));
        lbits zgsz3787;
        CREATE(lbits)(&zgsz3787);
        CONVERT_OF(lbits, fbits)(&zgsz3787, zgaz3360, UINT64_C(4) , true);
        lbits zgsz3788;
        CREATE(lbits)(&zgsz3788);
        sailgen_EXTZ(&zgsz3788, zgsz3786, zgsz3787);
        zotype64 = CONVERT_OF(fbits, lbits)(zgsz3788, true);
        KILL(lbits)(&zgsz3788);
        KILL(lbits)(&zgsz3787);
        KILL(sail_int)(&zgsz3786);
      }
    }
  }
  sail_string zgaz3357;
  CREATE(sail_string)(&zgaz3357);
  {
    sail_string zgaz3355;
    CREATE(sail_string)(&zgaz3355);
    {
      bool zgaz3334;
      zgaz3334 = zcap.ztag;
      if (zgaz3334) {  COPY(sail_string)(&zgaz3355, "1");  } else {  COPY(sail_string)(&zgaz3355, "0");  }
    }
    sail_string zgaz3356;
    CREATE(sail_string)(&zgaz3356);
    {
      sail_string zgaz3354;
      CREATE(sail_string)(&zgaz3354);
      {
        sail_string zgaz3352;
        CREATE(sail_string)(&zgaz3352);
        {
          bool zgaz3335;
          zgaz3335 = sailgen_isCapSealed(zcap);
          if (zgaz3335) {  COPY(sail_string)(&zgaz3352, "1");  } else {  COPY(sail_string)(&zgaz3352, "0");  }
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
              uint64_t zgaz3337;
              {
                uint64_t zgaz3336;
                zgaz3336 = sailgen_getCapPerms(zcap);
                {
                  lbits zgsz3792;
                  CREATE(lbits)(&zgsz3792);
                  CONVERT_OF(lbits, fbits)(&zgsz3792, UINT64_C(0b0), UINT64_C(1) , true);
                  lbits zgsz3793;
                  CREATE(lbits)(&zgsz3793);
                  CONVERT_OF(lbits, fbits)(&zgsz3793, zgaz3336, UINT64_C(15) , true);
                  lbits zgsz3794;
                  CREATE(lbits)(&zgsz3794);
                  append(&zgsz3794, zgsz3792, zgsz3793);
                  zgaz3337 = CONVERT_OF(fbits, lbits)(zgsz3794, true);
                  KILL(lbits)(&zgsz3794);
                  KILL(lbits)(&zgsz3793);
                  KILL(lbits)(&zgsz3792);
                }
              }
              {
                lbits zgsz3795;
                CREATE(lbits)(&zgsz3795);
                CONVERT_OF(lbits, fbits)(&zgsz3795, zgaz3337, UINT64_C(16) , true);
                string_of_lbits(&zgaz3349, zgsz3795);
                KILL(lbits)(&zgsz3795);
              }
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
                  lbits zgsz3798;
                  CREATE(lbits)(&zgsz3798);
                  CONVERT_OF(lbits, fbits)(&zgsz3798, zotype64, UINT64_C(32) , true);
                  string_of_lbits(&zgaz3346, zgsz3798);
                  KILL(lbits)(&zgsz3798);
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
                      uint64_t zgaz3338;
                      zgaz3338 = sailgen_getCapOffsetBits(zcap);
                      {
                        lbits zgsz3796;
                        CREATE(lbits)(&zgsz3796);
                        CONVERT_OF(lbits, fbits)(&zgsz3796, zgaz3338, UINT64_C(32) , true);
                        string_of_lbits(&zgaz3343, zgsz3796);
                        KILL(lbits)(&zgsz3796);
                      }
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
                          uint64_t zgaz3339;
                          zgaz3339 = sailgen_getCapBaseBits(zcap);
                          {
                            lbits zgsz3797;
                            CREATE(lbits)(&zgsz3797);
                            CONVERT_OF(lbits, fbits)(&zgsz3797, zgaz3339, UINT64_C(32) , true);
                            string_of_lbits(&zgaz3340, zgsz3797);
                            KILL(lbits)(&zgsz3797);
                          }
                        }
                        sail_string zgaz3341;
                        CREATE(sail_string)(&zgaz3341);
                        concat_str(&zgaz3341, " length:", zlen_str);
                        concat_str(&zgaz3342, zgaz3340, zgaz3341);
                        KILL(sail_string)(&zgaz3341);
                        KILL(sail_string)(&zgaz3340);
                      }
                      concat_str(&zgaz3344, " base:", zgaz3342);
                      KILL(sail_string)(&zgaz3342);
                    }
                    concat_str(&zgaz3345, zgaz3343, zgaz3344);
                    KILL(sail_string)(&zgaz3344);
                    KILL(sail_string)(&zgaz3343);
                  }
                  concat_str(&zgaz3347, " offset:", zgaz3345);
                  KILL(sail_string)(&zgaz3345);
                }
                concat_str(&zgaz3348, zgaz3346, zgaz3347);
                KILL(sail_string)(&zgaz3347);
                KILL(sail_string)(&zgaz3346);
              }
              concat_str(&zgaz3350, " type:", zgaz3348);
              KILL(sail_string)(&zgaz3348);
            }
            concat_str(&zgaz3351, zgaz3349, zgaz3350);
            KILL(sail_string)(&zgaz3350);
            KILL(sail_string)(&zgaz3349);
          }
          concat_str(&zgaz3353, " perms:", zgaz3351);
          KILL(sail_string)(&zgaz3351);
        }
        concat_str(&zgaz3354, zgaz3352, zgaz3353);
        KILL(sail_string)(&zgaz3353);
        KILL(sail_string)(&zgaz3352);
      }
      concat_str(&zgaz3356, " s:", zgaz3354);
      KILL(sail_string)(&zgaz3354);
    }
    concat_str(&zgaz3357, zgaz3355, zgaz3356);
    KILL(sail_string)(&zgaz3356);
    KILL(sail_string)(&zgaz3355);
  }
  concat_str((*(&zcbz338)), " t:", zgaz3357);
  KILL(sail_string)(&zgaz3357);

  KILL(sail_string)(&zlen_str);

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
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3365;
  {
    uint64_t zgaz3363;
    {
      sail_int zgsz3802;
      CREATE(sail_int)(&zgsz3802);
      CONVERT_OF(sail_int, mach_int)(&zgsz3802, INT64_C(32));
      sail_int zgsz3803;
      CREATE(sail_int)(&zgsz3803);
      CONVERT_OF(sail_int, mach_int)(&zgsz3803, INT64_C(0));
      lbits zgsz3804;
      CREATE(lbits)(&zgsz3804);
      sailgen_to_bits(&zgsz3804, zgsz3802, zgsz3803);
      zgaz3363 = CONVERT_OF(fbits, lbits)(zgsz3804, true);
      KILL(lbits)(&zgsz3804);
      KILL(sail_int)(&zgsz3803);
      KILL(sail_int)(&zgsz3802);
    }
    uint64_t zgaz3364;
    {
      lbits zgsz3799;
      CREATE(lbits)(&zgsz3799);
      CONVERT_OF(lbits, fbits)(&zgsz3799, UINT64_C(0b0), UINT64_C(1) , true);
      lbits zgsz3800;
      CREATE(lbits)(&zgsz3800);
      CONVERT_OF(lbits, fbits)(&zgsz3800, zlen, UINT64_C(32) , true);
      lbits zgsz3801;
      CREATE(lbits)(&zgsz3801);
      append(&zgsz3801, zgsz3799, zgsz3800);
      zgaz3364 = CONVERT_OF(fbits, lbits)(zgsz3801, true);
      KILL(lbits)(&zgsz3801);
      KILL(lbits)(&zgsz3800);
      KILL(lbits)(&zgsz3799);
    }
    zgaz3365 = sailgen_setCapBounds(zdefault_cap, zgaz3363, zgaz3364);
  }
  uint64_t zgsz385;
  {
    bool zexact;
    zexact = zgaz3365.ztup0;
    struct zCapability zc;
    zc = zgaz3365.ztup1;
    int64_t ze;
    {
      int64_t zgaz3371;
      {
        uint64_t zgaz3370;
        zgaz3370 = zc.zE;
        {
          lbits zgsz3805;
          CREATE(lbits)(&zgsz3805);
          CONVERT_OF(lbits, fbits)(&zgsz3805, zgaz3370, UINT64_C(6) , true);
          sail_int zgsz3806;
          CREATE(sail_int)(&zgsz3806);
          sail_unsigned(&zgsz3806, zgsz3805);
          zgaz3371 = CONVERT_OF(mach_int, sail_int)(zgsz3806);
          KILL(sail_int)(&zgsz3806);
          KILL(lbits)(&zgsz3805);
        }
      }
      {
        sail_int zgsz3807;
        CREATE(sail_int)(&zgsz3807);
        CONVERT_OF(sail_int, mach_int)(&zgsz3807, zgaz3371);
        sail_int zgsz3808;
        CREATE(sail_int)(&zgsz3808);
        CONVERT_OF(sail_int, mach_int)(&zgsz3808, zcap_max_E);
        sail_int zgsz3809;
        CREATE(sail_int)(&zgsz3809);
        min_int(&zgsz3809, zgsz3807, zgsz3808);
        ze = CONVERT_OF(mach_int, sail_int)(zgsz3809);
        KILL(sail_int)(&zgsz3809);
        KILL(sail_int)(&zgsz3808);
        KILL(sail_int)(&zgsz3807);
      }
    }
    int64_t zez7;
    {
      bool zgaz3369;
      zgaz3369 = zc.zinternal_E;
      if (zgaz3369) {
        {
          sail_int zgsz3810;
          CREATE(sail_int)(&zgsz3810);
          CONVERT_OF(sail_int, mach_int)(&zgsz3810, ze);
          sail_int zgsz3811;
          CREATE(sail_int)(&zgsz3811);
          CONVERT_OF(sail_int, mach_int)(&zgsz3811, zinternal_E_take_bits);
          sail_int zgsz3812;
          CREATE(sail_int)(&zgsz3812);
          add_int(&zgsz3812, zgsz3810, zgsz3811);
          zez7 = CONVERT_OF(mach_int, sail_int)(zgsz3812);
          KILL(sail_int)(&zgsz3812);
          KILL(sail_int)(&zgsz3811);
          KILL(sail_int)(&zgsz3810);
        }
      } else {  zez7 = INT64_C(0);  }
    }
    sbits zgaz3367;
    {
      int64_t zgaz3366;
      {
        sail_int zgsz3815;
        CREATE(sail_int)(&zgsz3815);
        CONVERT_OF(sail_int, mach_int)(&zgsz3815, INT64_C(32));
        sail_int zgsz3816;
        CREATE(sail_int)(&zgsz3816);
        CONVERT_OF(sail_int, mach_int)(&zgsz3816, zez7);
        sail_int zgsz3817;
        CREATE(sail_int)(&zgsz3817);
        sub_int(&zgsz3817, zgsz3815, zgsz3816);
        zgaz3366 = CONVERT_OF(mach_int, sail_int)(zgsz3817);
        KILL(sail_int)(&zgsz3817);
        KILL(sail_int)(&zgsz3816);
        KILL(sail_int)(&zgsz3815);
      }
      {
        sail_int zgsz3813;
        CREATE(sail_int)(&zgsz3813);
        CONVERT_OF(sail_int, mach_int)(&zgsz3813, zgaz3366);
        lbits zgsz3814;
        CREATE(lbits)(&zgsz3814);
        sailgen_ones(&zgsz3814, zgsz3813);
        zgaz3367 = CONVERT_OF(sbits, lbits)(zgsz3814, true);
        KILL(lbits)(&zgsz3814);
        KILL(sail_int)(&zgsz3813);
      }
    }
    sbits zgaz3368;
    {
      sail_int zgsz3821;
      CREATE(sail_int)(&zgsz3821);
      CONVERT_OF(sail_int, mach_int)(&zgsz3821, zez7);
      lbits zgsz3822;
      CREATE(lbits)(&zgsz3822);
      sailgen_zzeros_implicit(&zgsz3822, zgsz3821);
      zgaz3368 = CONVERT_OF(sbits, lbits)(zgsz3822, true);
      KILL(lbits)(&zgsz3822);
      KILL(sail_int)(&zgsz3821);
    }
    {
      lbits zgsz3818;
      CREATE(lbits)(&zgsz3818);
      CONVERT_OF(lbits, sbits)(&zgsz3818, zgaz3367, true);
      lbits zgsz3819;
      CREATE(lbits)(&zgsz3819);
      CONVERT_OF(lbits, sbits)(&zgsz3819, zgaz3368, true);
      lbits zgsz3820;
      CREATE(lbits)(&zgsz3820);
      append(&zgsz3820, zgsz3818, zgsz3819);
      zgsz385 = CONVERT_OF(fbits, lbits)(zgsz3820, true);
      KILL(lbits)(&zgsz3820);
      KILL(lbits)(&zgsz3819);
      KILL(lbits)(&zgsz3818);
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
  uint64_t zgaz3373;
  {
    uint64_t zgaz3372;
    {
      lbits zgsz3826;
      CREATE(lbits)(&zgsz3826);
      CONVERT_OF(lbits, fbits)(&zgsz3826, zm, UINT64_C(32) , true);
      lbits zgsz3827;
      CREATE(lbits)(&zgsz3827);
      not_bits(&zgsz3827, zgsz3826);
      zgaz3372 = CONVERT_OF(fbits, lbits)(zgsz3827, true);
      KILL(lbits)(&zgsz3827);
      KILL(lbits)(&zgsz3826);
    }
    {
      lbits zgsz3823;
      CREATE(lbits)(&zgsz3823);
      CONVERT_OF(lbits, fbits)(&zgsz3823, zlen, UINT64_C(32) , true);
      lbits zgsz3824;
      CREATE(lbits)(&zgsz3824);
      CONVERT_OF(lbits, fbits)(&zgsz3824, zgaz3372, UINT64_C(32) , true);
      lbits zgsz3825;
      CREATE(lbits)(&zgsz3825);
      add_bits(&zgsz3825, zgsz3823, zgsz3824);
      zgaz3373 = CONVERT_OF(fbits, lbits)(zgsz3825, true);
      KILL(lbits)(&zgsz3825);
      KILL(lbits)(&zgsz3824);
      KILL(lbits)(&zgsz3823);
    }
  }
  {
    lbits zgsz3828;
    CREATE(lbits)(&zgsz3828);
    CONVERT_OF(lbits, fbits)(&zgsz3828, zgaz3373, UINT64_C(32) , true);
    lbits zgsz3829;
    CREATE(lbits)(&zgsz3829);
    CONVERT_OF(lbits, fbits)(&zgsz3829, zm, UINT64_C(32) , true);
    lbits zgsz3830;
    CREATE(lbits)(&zgsz3830);
    and_bits(&zgsz3830, zgsz3828, zgsz3829);
    zcbz340 = CONVERT_OF(fbits, lbits)(zgsz3830, true);
    KILL(lbits)(&zgsz3830);
    KILL(lbits)(&zgsz3829);
    KILL(lbits)(&zgsz3828);
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
static uint64_t zTestLen;

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
  uint64_t zbits1;
  zbits1 = sailgen_capToBits(zTestCap);
  uint64_t zbits2;
  zbits2 = sailgen_capToMemBits(zTestCap);
  struct zCapability zcap1;
  {
    bool zgaz3380;
    zgaz3380 = zTestCap.ztag;
    zcap1 = sailgen_capBitsToCapability(zgaz3380, zbits1);
  }
  struct zCapability zcap2;
  {
    bool zgaz3379;
    zgaz3379 = zTestCap.ztag;
    zcap2 = sailgen_memBitsToCapability(zgaz3379, zbits2);
  }
  bool zrep;
  zrep = sailgen_fastRepCheck(zTestCap, zTestAddr);
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3374;
  zgaz3374 = sailgen_setCapBounds(zTestCap, zTestAddr, zTestLen);
  unit zgsz389;
  {
    __label__ case_174, finish_match_173;

    bool zexact;
    zexact = zgaz3374.ztup0;
    struct zCapability zcap;
    zcap = zgaz3374.ztup1;
    struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3375;
    zgaz3375 = sailgen_setCapAddr(zTestCap, zTestAddr);
    unit zgsz390;
    {
      __label__ case_176, finish_match_175;

      bool zrepresentable;
      zrepresentable = zgaz3375.ztup0;
      struct zCapability zcapshadowz30;
      zcapshadowz30 = zgaz3375.ztup1;
      struct ztuple_z8z5i64zCz0z5i64z9 zgaz3376;
      zgaz3376 = sailgen_getCapBounds(zTestCap);
      unit zgsz391;
      {
        int64_t zbase;
        zbase = zgaz3376.ztup0;
        int64_t ztop;
        ztop = zgaz3376.ztup1;
        uint64_t zflags;
        zflags = sailgen_getCapFlags(zTestCap);
        struct zCapability znew_cap;
        {
          uint64_t zgaz3378;
          {
            int64_t zgaz3377;
            {
              sail_int zgsz3833;
              CREATE(sail_int)(&zgsz3833);
              CONVERT_OF(sail_int, mach_int)(&zgsz3833, INT64_C(15));
              sail_int zgsz3834;
              CREATE(sail_int)(&zgsz3834);
              CONVERT_OF(sail_int, mach_int)(&zgsz3834, INT64_C(0));
              sail_int zgsz3835;
              CREATE(sail_int)(&zgsz3835);
              add_int(&zgsz3835, zgsz3833, zgsz3834);
              zgaz3377 = CONVERT_OF(mach_int, sail_int)(zgsz3835);
              KILL(sail_int)(&zgsz3835);
              KILL(sail_int)(&zgsz3834);
              KILL(sail_int)(&zgsz3833);
            }
            {
              sail_int zgsz3831;
              CREATE(sail_int)(&zgsz3831);
              CONVERT_OF(sail_int, mach_int)(&zgsz3831, zgaz3377);
              lbits zgsz3832;
              CREATE(lbits)(&zgsz3832);
              sailgen_ones(&zgsz3832, zgsz3831);
              zgaz3378 = CONVERT_OF(fbits, lbits)(zgsz3832, true);
              KILL(lbits)(&zgsz3832);
              KILL(sail_int)(&zgsz3831);
            }
          }
          znew_cap = sailgen_setCapPerms(zTestCap, zgaz3378);
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
        goto finish_match_175;
      }
    case_176: ;
      sail_match_failure("main");
    finish_match_175: ;
      zgsz390 = zgsz391;
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
      sail_int zgsz3836;
      CREATE(sail_int)(&zgsz3836);
      CONVERT_OF(sail_int, mach_int)(&zgsz3836, INT64_C(32));
      lbits zgsz3837;
      CREATE(lbits)(&zgsz3837);
      UNDEFINED(lbits)(&zgsz3837, zgsz3836);
      zTestAddr = CONVERT_OF(fbits, lbits)(zgsz3837, true);
      KILL(lbits)(&zgsz3837);
      KILL(sail_int)(&zgsz3836);
    }
    unit zgsz396;
    zgsz396 = UNIT;
  }
  int64_t zgaz3381;
  {
    sail_int zgsz3840;
    CREATE(sail_int)(&zgsz3840);
    CONVERT_OF(sail_int, mach_int)(&zgsz3840, INT64_C(32));
    sail_int zgsz3841;
    CREATE(sail_int)(&zgsz3841);
    CONVERT_OF(sail_int, mach_int)(&zgsz3841, INT64_C(1));
    sail_int zgsz3842;
    CREATE(sail_int)(&zgsz3842);
    add_int(&zgsz3842, zgsz3840, zgsz3841);
    zgaz3381 = CONVERT_OF(mach_int, sail_int)(zgsz3842);
    KILL(sail_int)(&zgsz3842);
    KILL(sail_int)(&zgsz3841);
    KILL(sail_int)(&zgsz3840);
  }
  {
    sail_int zgsz3838;
    CREATE(sail_int)(&zgsz3838);
    CONVERT_OF(sail_int, mach_int)(&zgsz3838, zgaz3381);
    lbits zgsz3839;
    CREATE(lbits)(&zgsz3839);
    UNDEFINED(lbits)(&zgsz3839, zgsz3838);
    zTestLen = CONVERT_OF(fbits, lbits)(zgsz3839, true);
    KILL(lbits)(&zgsz3839);
    KILL(sail_int)(&zgsz3838);
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


