#include "sail.h"
#include "rts.h"
#include "elf.h"
void (*sail_rts_set_coverage_file)(const char *) = NULL;
#ifdef __cplusplus
extern "C" {
#endif

// enum write_kind
enum zwrite_kind { zWrite_plain, zWrite_conditional, zWrite_release, zWrite_exclusive, zWrite_exclusive_release, zWrite_RISCV_release, zWrite_RISCV_strong_release, zWrite_RISCV_conditional, zWrite_RISCV_conditional_release, zWrite_RISCV_conditional_strong_release, zWrite_X86_locked };

static bool EQUAL(zwrite_kind)(enum zwrite_kind op1, enum zwrite_kind op2) {
  return op1 == op2;
}

static enum zwrite_kind UNDEFINED(zwrite_kind)(unit u) { return zWrite_plain; }

// enum trans_kind
enum ztrans_kind { zTransaction_start, zTransaction_commit, zTransaction_abort };

static bool EQUAL(ztrans_kind)(enum ztrans_kind op1, enum ztrans_kind op2) {
  return op1 == op2;
}

static enum ztrans_kind UNDEFINED(ztrans_kind)(unit u) { return zTransaction_start; }

// enum read_kind
enum zread_kind { zRead_plain, zRead_reserve, zRead_acquire, zRead_exclusive, zRead_exclusive_acquire, zRead_stream, zRead_ifetch, zRead_RISCV_acquire, zRead_RISCV_strong_acquire, zRead_RISCV_reserved, zRead_RISCV_reserved_acquire, zRead_RISCV_reserved_strong_acquire, zRead_X86_locked };

static bool EQUAL(zread_kind)(enum zread_kind op1, enum zread_kind op2) {
  return op1 == op2;
}

static enum zread_kind UNDEFINED(zread_kind)(unit u) { return zRead_plain; }

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

// enum cache_op_kind
enum zcache_op_kind { zCache_op_D_IVAC, zCache_op_D_ISW, zCache_op_D_CSW, zCache_op_D_CISW, zCache_op_D_ZVA, zCache_op_D_CVAC, zCache_op_D_CVAU, zCache_op_D_CIVAC, zCache_op_I_IALLUIS, zCache_op_I_IALLU, zCache_op_I_IVAU };

static bool EQUAL(zcache_op_kind)(enum zcache_op_kind op1, enum zcache_op_kind op2) {
  return op1 == op2;
}

static enum zcache_op_kind UNDEFINED(zcache_op_kind)(unit u) { return zCache_op_D_IVAC; }

// enum a64_barrier_type
enum za64_barrier_type { zA64_barrier_all, zA64_barrier_LD, zA64_barrier_ST };

static bool EQUAL(za64_barrier_type)(enum za64_barrier_type op1, enum za64_barrier_type op2) {
  return op1 == op2;
}

static enum za64_barrier_type UNDEFINED(za64_barrier_type)(unit u) { return zA64_barrier_all; }

// enum a64_barrier_domain
enum za64_barrier_domain { zA64_FullShare, zA64_InnerShare, zA64_OuterShare, zA64_NonShare };

static bool EQUAL(za64_barrier_domain)(enum za64_barrier_domain op1, enum za64_barrier_domain op2) {
  return op1 == op2;
}

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

static bool EQUAL(zClearRegSet)(enum zClearRegSet op1, enum zClearRegSet op2) {
  return op1 == op2;
}

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

static bool EQUAL(zCapEx)(enum zCapEx op1, enum zCapEx op2) {
  return op1 == op2;
}

static enum zCapEx UNDEFINED(zCapEx)(unit u) { return zCapEx_None; }

// enum CPtrCmpOp
enum zCPtrCmpOp { zCEQ, zCNE, zCLT, zCLE, zCLTU, zCLEU, zCEXEQ, zCNEXEQ };

static bool EQUAL(zCPtrCmpOp)(enum zCPtrCmpOp op1, enum zCPtrCmpOp op2) {
  return op1 == op2;
}

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

lbits zghz30;

static void startup_sailgen_sail_ones(void)
{    CREATE(lbits)(&zghz30);
}

static void sailgen_sail_ones(lbits *zcbz32, sail_int zn)
{
  __label__ end_function_7, end_block_exception_8, end_function_208;

  RECREATE(lbits)(&zghz30);
  zeros(&zghz30, zn);
  not_bits((*(&zcbz32)), zghz30);
end_function_7: ;
  goto end_function_208;
end_block_exception_8: ;
  goto end_function_208;
end_function_208: ;
}



static void finish_sailgen_sail_ones(void)
{    KILL(lbits)(&zghz30);
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

static bool sailgen_not(bool);

static bool sailgen_not(bool zb)
{
  __label__ end_function_13, end_block_exception_14;

  bool zcbz34;
  zcbz34 = not(zb);
end_function_13: ;
  return zcbz34;
end_block_exception_14: ;

  return false;
}









static void sailgen_sign_extend(lbits *rop, sail_int, lbits);

static void sailgen_zzero_extend(lbits *rop, sail_int, lbits);

static void sailgen_sign_extend(lbits *zcbz35, sail_int zm, lbits zv)
{
  __label__ end_function_16, end_block_exception_17, end_function_207;

  sign_extend((*(&zcbz35)), zv, zm);
end_function_16: ;
  goto end_function_207;
end_block_exception_17: ;
  goto end_function_207;
end_function_207: ;
}

static void sailgen_zzero_extend(lbits *zcbz36, sail_int zm, lbits zv)
{
  __label__ end_function_19, end_block_exception_20, end_function_206;

  zero_extend((*(&zcbz36)), zv, zm);
end_function_19: ;
  goto end_function_206;
end_block_exception_20: ;
  goto end_function_206;
end_function_206: ;
}

static void sailgen_zzeros_implicit(lbits *rop, sail_int);

static void sailgen_zzeros_implicit(lbits *zcbz37, sail_int zn)
{
  __label__ end_function_22, end_block_exception_23, end_function_205;

  zeros((*(&zcbz37)), zn);
end_function_22: ;
  goto end_function_205;
end_block_exception_23: ;
  goto end_function_205;
end_function_205: ;
}

static void sailgen_ones(lbits *rop, sail_int);

static void sailgen_ones(lbits *zcbz38, sail_int zn)
{
  __label__ end_function_25, end_block_exception_26, end_function_204;

  sailgen_sail_ones((*(&zcbz38)), zn);
end_function_25: ;
  goto end_function_204;
end_block_exception_26: ;
  goto end_function_204;
end_function_204: ;
}

static fbits sailgen_bool_to_bit(bool);

static fbits sailgen_bool_to_bit(bool zx)
{
  __label__ end_function_28, end_block_exception_29;

  fbits zcbz39;
  if (zx) {  zcbz39 = UINT64_C(1);  } else {  zcbz39 = UINT64_C(0);  }
end_function_28: ;
  return zcbz39;
end_block_exception_29: ;

  return UINT64_C(0);
}

static uint64_t sailgen_bool_to_bits(bool);

static uint64_t sailgen_bool_to_bits(bool zx)
{
  __label__ end_function_31, end_block_exception_32;

  uint64_t zcbz310;
  fbits zgaz34;
  zgaz34 = sailgen_bool_to_bit(zx);
  zcbz310 = UINT64_C(0b0);
  zcbz310 = update_fbits(zcbz310, INT64_C(0), zgaz34);

end_function_31: ;
  return zcbz310;
end_block_exception_32: ;

  return UINT64_C(0xdeadc0de);
}

static bool sailgen_bit_to_bool(fbits);

static bool sailgen_bit_to_bool(fbits zb)
{
  __label__ case_36, case_35, finish_match_34, end_function_37, end_block_exception_38;

  bool zcbz311;
  {
    fbits zp0z3;
    zp0z3 = zb;
    bool zgsz33;
    zgsz33 = eq_bit(zp0z3, UINT64_C(1));
    if (!(zgsz33)) {

      goto case_36;
    }
    zcbz311 = true;
    goto finish_match_34;
  }
case_36: ;
  {
    fbits zuz30;
    zuz30 = zb;
    bool zgsz32;
    zgsz32 = eq_bit(zuz30, UINT64_C(0));
    if (!(zgsz32)) {

      goto case_35;
    }
    zcbz311 = false;
    goto finish_match_34;
  }
case_35: ;
  sail_match_failure("bit_to_bool");
finish_match_34: ;
end_function_37: ;
  return zcbz311;
end_block_exception_38: ;

  return false;
}

static void sailgen_to_bits(lbits *rop, sail_int, sail_int);

sail_int zghz31;

static void startup_sailgen_to_bits(void)
{    CREATE(sail_int)(&zghz31);
}

static void sailgen_to_bits(lbits *zcbz312, sail_int zl, sail_int zn)
{
  __label__ end_function_40, end_block_exception_41, end_function_203;

  {
    RECREATE(sail_int)(&zghz31);
    CONVERT_OF(sail_int, mach_int)(&zghz31, INT64_C(0));
    get_slice_int((*(&zcbz312)), zl, zn, zghz31);
  }
end_function_40: ;
  goto end_function_203;
end_block_exception_41: ;
  goto end_function_203;
end_function_203: ;
}



static void finish_sailgen_to_bits(void)
{    KILL(sail_int)(&zghz31);
}

static bool sailgen_z8operatorz0zI_uz9(lbits, lbits);

static bool sailgen_z8operatorz0zKzJ_uz9(lbits, lbits);

sail_int zghz32;
sail_int zghz33;

static void startup_sailgen_z8operatorz0zI_uz9(void)
{
  CREATE(sail_int)(&zghz32);
  CREATE(sail_int)(&zghz33);
}

static bool sailgen_z8operatorz0zI_uz9(lbits zx, lbits zy)
{
  __label__ end_function_43, end_block_exception_44;

  bool zcbz313;
  RECREATE(sail_int)(&zghz32);
  sail_unsigned(&zghz32, zx);
  RECREATE(sail_int)(&zghz33);
  sail_unsigned(&zghz33, zy);
  zcbz313 = lt(zghz32, zghz33);
end_function_43: ;
  return zcbz313;
end_block_exception_44: ;

  return false;
}



static void finish_sailgen_z8operatorz0zI_uz9(void)
{
  KILL(sail_int)(&zghz33);
  KILL(sail_int)(&zghz32);
}

sail_int zghz34;
sail_int zghz35;

static void startup_sailgen_z8operatorz0zKzJ_uz9(void)
{
  CREATE(sail_int)(&zghz34);
  CREATE(sail_int)(&zghz35);
}

static bool sailgen_z8operatorz0zKzJ_uz9(lbits zx, lbits zy)
{
  __label__ end_function_46, end_block_exception_47;

  bool zcbz314;
  RECREATE(sail_int)(&zghz34);
  sail_unsigned(&zghz34, zx);
  RECREATE(sail_int)(&zghz35);
  sail_unsigned(&zghz35, zy);
  zcbz314 = gteq(zghz34, zghz35);
end_function_46: ;
  return zcbz314;
end_block_exception_47: ;

  return false;
}



static void finish_sailgen_z8operatorz0zKzJ_uz9(void)
{
  KILL(sail_int)(&zghz35);
  KILL(sail_int)(&zghz34);
}



static void sailgen_MAX(sail_int *rop, sail_int);

sail_int zghz36;
sail_int zghz37;

static void startup_sailgen_MAX(void)
{
  CREATE(sail_int)(&zghz36);
  CREATE(sail_int)(&zghz37);
}

static void sailgen_MAX(sail_int *zcbz315, sail_int zn)
{
  __label__ end_function_49, end_block_exception_50, end_function_202;

  RECREATE(sail_int)(&zghz36);
  pow2(&zghz36, zn);
  {
    RECREATE(sail_int)(&zghz37);
    CONVERT_OF(sail_int, mach_int)(&zghz37, INT64_C(1));
    sub_int((*(&zcbz315)), zghz36, zghz37);
  }
end_function_49: ;
  goto end_function_202;
end_block_exception_50: ;
  goto end_function_202;
end_function_202: ;
}



static void finish_sailgen_MAX(void)
{
  KILL(sail_int)(&zghz37);
  KILL(sail_int)(&zghz36);
}

static int64_t zreserved_otypes;
static void create_letbind_0(void) {


  int64_t zgsz34;
  zgsz34 = INT64_C(4);
  zreserved_otypes = zgsz34;

let_end_51: ;
}
static void kill_letbind_0(void) {
}

static int64_t zotype_unsealed;
static void create_letbind_1(void) {


  int64_t zgsz35;
  zgsz35 = INT64_C(-1);
  zotype_unsealed = zgsz35;

let_end_52: ;
}
static void kill_letbind_1(void) {
}

static int64_t zotype_sentry;
static void create_letbind_2(void) {


  int64_t zgsz36;
  zgsz36 = INT64_C(-2);
  zotype_sentry = zgsz36;

let_end_53: ;
}
static void kill_letbind_2(void) {
}

static uint64_t zPCC_IDX;
static void create_letbind_3(void) {


  uint64_t zgsz37;
  zgsz37 = UINT64_C(0b100000);
  zPCC_IDX = zgsz37;

let_end_54: ;
}
static void kill_letbind_3(void) {
}

static uint64_t zDDC_IDX;
static void create_letbind_4(void) {


  uint64_t zgsz38;
  zgsz38 = UINT64_C(0b100001);
  zDDC_IDX = zgsz38;

let_end_55: ;
}
static void kill_letbind_4(void) {
}

static int64_t zcap_sizze;
static void create_letbind_5(void) {


  int64_t zgsz39;
  zgsz39 = INT64_C(8);
  zcap_sizze = zgsz39;

let_end_56: ;
}
static void kill_letbind_5(void) {
}

static int64_t zlog2_cap_sizze;
static void create_letbind_6(void) {


  int64_t zgsz310;
  zgsz310 = INT64_C(3);
  zlog2_cap_sizze = zgsz310;

let_end_57: ;
}
static void kill_letbind_6(void) {
}

static int64_t zcap_hperms_width;
static void create_letbind_7(void) {


  int64_t zgsz311;
  zgsz311 = INT64_C(12);
  zcap_hperms_width = zgsz311;

let_end_58: ;
}
static void kill_letbind_7(void) {
}

static int64_t zcap_uperms_width;
static void create_letbind_8(void) {


  int64_t zgsz312;
  zgsz312 = INT64_C(0);
  zcap_uperms_width = zgsz312;

let_end_59: ;
}
static void kill_letbind_8(void) {
}

static int64_t zcap_otype_width;
static void create_letbind_9(void) {


  int64_t zgsz313;
  zgsz313 = INT64_C(4);
  zcap_otype_width = zgsz313;

let_end_60: ;
}
static void kill_letbind_9(void) {
}

static int64_t zcap_reserved_width;
static void create_letbind_10(void) {


  int64_t zgsz314;
  zgsz314 = INT64_C(0);
  zcap_reserved_width = zgsz314;

let_end_61: ;
}
static void kill_letbind_10(void) {
}

static int64_t zcap_flags_width;
static void create_letbind_11(void) {


  int64_t zgsz315;
  zgsz315 = INT64_C(1);
  zcap_flags_width = zgsz315;

let_end_62: ;
}
static void kill_letbind_11(void) {
}

static int64_t zcap_mantissa_width;
static void create_letbind_12(void) {


  int64_t zgsz316;
  zgsz316 = INT64_C(8);
  zcap_mantissa_width = zgsz316;

let_end_63: ;
}
static void kill_letbind_12(void) {
}

static int64_t zcap_E_width;
static void create_letbind_13(void) {


  int64_t zgsz317;
  zgsz317 = INT64_C(6);
  zcap_E_width = zgsz317;

let_end_64: ;
}
static void kill_letbind_13(void) {
}

static int64_t zcap_addr_width;
static void create_letbind_14(void) {


  int64_t zgsz318;
  zgsz318 = INT64_C(32);
  zcap_addr_width = zgsz318;

let_end_65: ;
}
static void kill_letbind_14(void) {
}

static int64_t zcap_len_width;
static void create_letbind_15(void) {


  int64_t zgsz319;
  zgsz319 = INT64_C(33);
  zcap_len_width = zgsz319;

let_end_66: ;
}
static void kill_letbind_15(void) {
}

static int64_t zcaps_per_cache_line;
static void create_letbind_16(void) {


  int64_t zgsz320;
  zgsz320 = INT64_C(8);
  zcaps_per_cache_line = zgsz320;

let_end_67: ;
}
static void kill_letbind_16(void) {
}

static int64_t zinternal_E_take_bits;
static void create_letbind_17(void) {


  int64_t zgsz321;
  zgsz321 = INT64_C(3);
  zinternal_E_take_bits = zgsz321;

let_end_68: ;
}
static void kill_letbind_17(void) {
}

static struct zEncCapability sailgen_capBitsToEncCapability(uint64_t);

struct zEncCapability zghz38;

static void startup_sailgen_capBitsToEncCapability(void)
{
}

static struct zEncCapability sailgen_capBitsToEncCapability(uint64_t zc)
{
  __label__ end_function_70, end_block_exception_71;

  struct zEncCapability zghz38;
  uint64_t zgaz310;
  zgaz310 = (safe_rshift(UINT64_MAX, 64 - 12) & (zc >> INT64_C(52)));
  uint64_t zgaz311;
  zgaz311 = (safe_rshift(UINT64_MAX, 64 - 1) & (zc >> INT64_C(51)));
  uint64_t zgaz312;
  zgaz312 = (safe_rshift(UINT64_MAX, 64 - 4) & (zc >> INT64_C(47)));
  fbits zgaz313;
  zgaz313 = (UINT64_C(1) & (zc >> INT64_C(46)));
  uint64_t zgaz314;
  zgaz314 = (safe_rshift(UINT64_MAX, 64 - 6) & (zc >> INT64_C(40)));
  uint64_t zgaz315;
  zgaz315 = (safe_rshift(UINT64_MAX, 64 - 8) & (zc >> INT64_C(32)));
  uint64_t zgaz316;
  zgaz316 = (safe_rshift(UINT64_MAX, 64 - 32) & (zc >> INT64_C(0)));
  zghz38.zB = zgaz315;
  zghz38.zT = zgaz314;
  zghz38.zaddress = zgaz316;
  zghz38.zflags = zgaz311;
  zghz38.zinternal_E = zgaz313;
  zghz38.zotype = zgaz312;
  zghz38.zperms = zgaz310;
  zghz38.zreserved = UINT64_C(0);







end_function_70: ;
  return zghz38;
end_block_exception_71: ;
  struct zEncCapability zcbz348 = { .zB = UINT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zflags = UINT64_C(0xdeadc0de), .zinternal_E = UINT64_C(0), .zotype = UINT64_C(0xdeadc0de), .zperms = UINT64_C(0xdeadc0de), .zreserved = UINT64_C(0xdeadc0de) };
  return zcbz348;
}



static void finish_sailgen_capBitsToEncCapability(void)
{
}

static uint64_t sailgen_encCapToBits(struct zEncCapability);

static uint64_t sailgen_encCapToBits(struct zEncCapability zcap)
{
  __label__ end_function_73, end_block_exception_74;

  uint64_t zcbz317;
  uint64_t zgaz330;
  zgaz330 = zcap.zperms;
  uint64_t zgaz331;
  {
    uint64_t zgaz328;
    zgaz328 = zcap.zreserved;
    uint64_t zgaz329;
    {
      uint64_t zgaz326;
      zgaz326 = zcap.zflags;
      uint64_t zgaz327;
      {
        uint64_t zgaz324;
        zgaz324 = zcap.zotype;
        uint64_t zgaz325;
        {
          uint64_t zgaz322;
          {
            fbits zgaz317;
            zgaz317 = zcap.zinternal_E;
            zgaz322 = UINT64_C(0b0);
            zgaz322 = update_fbits(zgaz322, INT64_C(0), zgaz317);
          }
          uint64_t zgaz323;
          {
            uint64_t zgaz320;
            zgaz320 = zcap.zT;
            uint64_t zgaz321;
            {
              uint64_t zgaz318;
              zgaz318 = zcap.zB;
              uint64_t zgaz319;
              zgaz319 = zcap.zaddress;
              zgaz321 = (zgaz318 << 32) | zgaz319;
            }
            zgaz323 = (zgaz320 << 40) | zgaz321;
          }
          zgaz325 = (zgaz322 << 46) | zgaz323;
        }
        zgaz327 = (zgaz324 << 47) | zgaz325;
      }
      zgaz329 = (zgaz326 << 51) | zgaz327;
    }
    zgaz331 = zgaz329;
  }
  zcbz317 = (zgaz330 << 52) | zgaz331;


end_function_73: ;
  return zcbz317;
end_block_exception_74: ;

  return UINT64_C(0xdeadc0de);
}

static int64_t zcap_max_addr;
static void create_letbind_18(void) {


  int64_t zgsz324;
  {
    sail_int zgsz3111;
    CREATE(sail_int)(&zgsz3111);
    CONVERT_OF(sail_int, mach_int)(&zgsz3111, zcap_addr_width);
    sail_int zgsz3112;
    CREATE(sail_int)(&zgsz3112);
    sailgen_MAX(&zgsz3112, zgsz3111);
    zgsz324 = CONVERT_OF(mach_int, sail_int)(zgsz3112);
    KILL(sail_int)(&zgsz3112);
    KILL(sail_int)(&zgsz3111);
  }
  zcap_max_addr = zgsz324;

let_end_75: ;
}
static void kill_letbind_18(void) {
}

static int64_t zcap_max_otype;
static void create_letbind_19(void) {


  int64_t zgsz325;
  int64_t zgaz332;
  {
    sail_int zgsz3116;
    CREATE(sail_int)(&zgsz3116);
    CONVERT_OF(sail_int, mach_int)(&zgsz3116, zcap_otype_width);
    sail_int zgsz3117;
    CREATE(sail_int)(&zgsz3117);
    sailgen_MAX(&zgsz3117, zgsz3116);
    zgaz332 = CONVERT_OF(mach_int, sail_int)(zgsz3117);
    KILL(sail_int)(&zgsz3117);
    KILL(sail_int)(&zgsz3116);
  }
  {
    sail_int zgsz3113;
    CREATE(sail_int)(&zgsz3113);
    CONVERT_OF(sail_int, mach_int)(&zgsz3113, zgaz332);
    sail_int zgsz3114;
    CREATE(sail_int)(&zgsz3114);
    CONVERT_OF(sail_int, mach_int)(&zgsz3114, zreserved_otypes);
    sail_int zgsz3115;
    CREATE(sail_int)(&zgsz3115);
    sub_int(&zgsz3115, zgsz3113, zgsz3114);
    zgsz325 = CONVERT_OF(mach_int, sail_int)(zgsz3115);
    KILL(sail_int)(&zgsz3115);
    KILL(sail_int)(&zgsz3114);
    KILL(sail_int)(&zgsz3113);
  }

  zcap_max_otype = zgsz325;

let_end_76: ;
}
static void kill_letbind_19(void) {
}

static int64_t zcap_uperms_shift;
static void create_letbind_20(void) {


  int64_t zgsz326;
  zgsz326 = INT64_C(15);
  zcap_uperms_shift = zgsz326;

let_end_77: ;
}
static void kill_letbind_20(void) {
}

static int64_t zcap_perms_width;
static void create_letbind_21(void) {


  int64_t zgsz327;
  zgsz327 = INT64_C(15);
  zcap_perms_width = zgsz327;

let_end_78: ;
}
static void kill_letbind_21(void) {
}

static int64_t zcap_max_E;
static void create_letbind_22(void) {


  int64_t zgsz328;
  int64_t zgaz333;
  {
    sail_int zgsz3118;
    CREATE(sail_int)(&zgsz3118);
    CONVERT_OF(sail_int, mach_int)(&zgsz3118, zcap_len_width);
    sail_int zgsz3119;
    CREATE(sail_int)(&zgsz3119);
    CONVERT_OF(sail_int, mach_int)(&zgsz3119, zcap_mantissa_width);
    sail_int zgsz3120;
    CREATE(sail_int)(&zgsz3120);
    sub_int(&zgsz3120, zgsz3118, zgsz3119);
    zgaz333 = CONVERT_OF(mach_int, sail_int)(zgsz3120);
    KILL(sail_int)(&zgsz3120);
    KILL(sail_int)(&zgsz3119);
    KILL(sail_int)(&zgsz3118);
  }
  zgsz328 = (zgaz333 + INT64_C(1));

  zcap_max_E = zgsz328;

let_end_79: ;
}
static void kill_letbind_22(void) {
}

static uint64_t zcap_reset_E;
static void create_letbind_23(void) {


  uint64_t zgsz329;
  {
    sail_int zgsz3121;
    CREATE(sail_int)(&zgsz3121);
    CONVERT_OF(sail_int, mach_int)(&zgsz3121, zcap_E_width);
    sail_int zgsz3122;
    CREATE(sail_int)(&zgsz3122);
    CONVERT_OF(sail_int, mach_int)(&zgsz3122, zcap_max_E);
    lbits zgsz3123;
    CREATE(lbits)(&zgsz3123);
    sailgen_to_bits(&zgsz3123, zgsz3121, zgsz3122);
    zgsz329 = CONVERT_OF(fbits, lbits)(zgsz3123, true);
    KILL(lbits)(&zgsz3123);
    KILL(sail_int)(&zgsz3122);
    KILL(sail_int)(&zgsz3121);
  }
  zcap_reset_E = zgsz329;

let_end_80: ;
}
static void kill_letbind_23(void) {
}

static uint64_t zcap_reset_T;
static void create_letbind_24(void) {


  uint64_t zgsz330;
  uint64_t zgaz335;
  {
    int64_t zgaz334;
    {
      sail_int zgsz3126;
      CREATE(sail_int)(&zgsz3126);
      CONVERT_OF(sail_int, mach_int)(&zgsz3126, zcap_mantissa_width);
      sail_int zgsz3127;
      CREATE(sail_int)(&zgsz3127);
      CONVERT_OF(sail_int, mach_int)(&zgsz3127, INT64_C(2));
      sail_int zgsz3128;
      CREATE(sail_int)(&zgsz3128);
      sub_int(&zgsz3128, zgsz3126, zgsz3127);
      zgaz334 = CONVERT_OF(mach_int, sail_int)(zgsz3128);
      KILL(sail_int)(&zgsz3128);
      KILL(sail_int)(&zgsz3127);
      KILL(sail_int)(&zgsz3126);
    }
    {
      sail_int zgsz3124;
      CREATE(sail_int)(&zgsz3124);
      CONVERT_OF(sail_int, mach_int)(&zgsz3124, zgaz334);
      lbits zgsz3125;
      CREATE(lbits)(&zgsz3125);
      sailgen_zzeros_implicit(&zgsz3125, zgsz3124);
      zgaz335 = CONVERT_OF(fbits, lbits)(zgsz3125, true);
      KILL(lbits)(&zgsz3125);
      KILL(sail_int)(&zgsz3124);
    }
  }
  zgsz330 = (UINT64_C(0b01) << 6) | zgaz335;

  zcap_reset_T = zgsz330;

let_end_81: ;
}
static void kill_letbind_24(void) {
}

static struct zCapability sailgen_undefined_Capability(unit);

struct zCapability zghz39;

static void startup_sailgen_undefined_Capability(void)
{
}

static struct zCapability sailgen_undefined_Capability(unit zgsz331)
{
  __label__ end_function_83, end_block_exception_84;

  struct zCapability zghz39;
  zghz39.zB = UINT64_C(0x00);
  zghz39.zE = UINT64_C(0b000000);
  zghz39.zT = UINT64_C(0x00);
  zghz39.zaccess_system_regs = false;
  zghz39.zaddress = UINT64_C(0x00000000);
  zghz39.zflag_cap_mode = false;
  zghz39.zglobal = false;
  zghz39.zinternal_E = false;
  zghz39.zotype = UINT64_C(0x0);
  zghz39.zpermit_cinvoke = false;
  zghz39.zpermit_execute = false;
  zghz39.zpermit_load = false;
  zghz39.zpermit_load_cap = false;
  zghz39.zpermit_seal = false;
  zghz39.zpermit_set_CID = false;
  zghz39.zpermit_store = false;
  zghz39.zpermit_store_cap = false;
  zghz39.zpermit_store_local_cap = false;
  zghz39.zpermit_unseal = false;
  zghz39.zreserved = UINT64_C(0);
  zghz39.ztag = false;
  zghz39.zuperms = UINT64_C(0);
end_function_83: ;
  return zghz39;
end_block_exception_84: ;
  struct zCapability zcbz349 = { .zB = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zaddress = UINT64_C(0xdeadc0de), .zflag_cap_mode = false, .zglobal = false, .zinternal_E = false, .zotype = UINT64_C(0xdeadc0de), .zpermit_cinvoke = false, .zpermit_execute = false, .zpermit_load = false, .zpermit_load_cap = false, .zpermit_seal = false, .zpermit_set_CID = false, .zpermit_store = false, .zpermit_store_cap = false, .zpermit_store_local_cap = false, .zpermit_unseal = false, .zreserved = UINT64_C(0xdeadc0de), .ztag = false, .zuperms = UINT64_C(0xdeadc0de) };
  return zcbz349;
}



static void finish_sailgen_undefined_Capability(void)
{
}

static struct zCapability znull_cap;
static void create_letbind_25(void) {


  struct zCapability zgsz334;
  uint64_t zgaz336;
  {
    sail_int zgsz3129;
    CREATE(sail_int)(&zgsz3129);
    CONVERT_OF(sail_int, mach_int)(&zgsz3129, zcap_otype_width);
    sail_int zgsz3130;
    CREATE(sail_int)(&zgsz3130);
    CONVERT_OF(sail_int, mach_int)(&zgsz3130, zotype_unsealed);
    lbits zgsz3131;
    CREATE(lbits)(&zgsz3131);
    sailgen_to_bits(&zgsz3131, zgsz3129, zgsz3130);
    zgaz336 = CONVERT_OF(fbits, lbits)(zgsz3131, true);
    KILL(lbits)(&zgsz3131);
    KILL(sail_int)(&zgsz3130);
    KILL(sail_int)(&zgsz3129);
  }
  struct zCapability zgsz333;
  zgsz333.zB = UINT64_C(0x00);
  zgsz333.zE = zcap_reset_E;
  zgsz333.zT = zcap_reset_T;
  zgsz333.zaccess_system_regs = false;
  zgsz333.zaddress = UINT64_C(0x00000000);
  zgsz333.zflag_cap_mode = false;
  zgsz333.zglobal = false;
  zgsz333.zinternal_E = true;
  zgsz333.zotype = zgaz336;
  zgsz333.zpermit_cinvoke = false;
  zgsz333.zpermit_execute = false;
  zgsz333.zpermit_load = false;
  zgsz333.zpermit_load_cap = false;
  zgsz333.zpermit_seal = false;
  zgsz333.zpermit_set_CID = false;
  zgsz333.zpermit_store = false;
  zgsz333.zpermit_store_cap = false;
  zgsz333.zpermit_store_local_cap = false;
  zgsz333.zpermit_unseal = false;
  zgsz333.zreserved = UINT64_C(0);
  zgsz333.ztag = false;
  zgsz333.zuperms = UINT64_C(0);
  zgsz334 = zgsz333;


  znull_cap = zgsz334;

let_end_85: ;
}
static void kill_letbind_25(void) {
}

static struct zCapability zdefault_cap;
static void create_letbind_26(void) {


  struct zCapability zgsz336;
  uint64_t zgaz337;
  {
    sail_int zgsz3132;
    CREATE(sail_int)(&zgsz3132);
    CONVERT_OF(sail_int, mach_int)(&zgsz3132, zcap_otype_width);
    sail_int zgsz3133;
    CREATE(sail_int)(&zgsz3133);
    CONVERT_OF(sail_int, mach_int)(&zgsz3133, zotype_unsealed);
    lbits zgsz3134;
    CREATE(lbits)(&zgsz3134);
    sailgen_to_bits(&zgsz3134, zgsz3132, zgsz3133);
    zgaz337 = CONVERT_OF(fbits, lbits)(zgsz3134, true);
    KILL(lbits)(&zgsz3134);
    KILL(sail_int)(&zgsz3133);
    KILL(sail_int)(&zgsz3132);
  }
  struct zCapability zgsz335;
  zgsz335.zB = UINT64_C(0x00);
  zgsz335.zE = zcap_reset_E;
  zgsz335.zT = zcap_reset_T;
  zgsz335.zaccess_system_regs = true;
  zgsz335.zaddress = UINT64_C(0x00000000);
  zgsz335.zflag_cap_mode = false;
  zgsz335.zglobal = true;
  zgsz335.zinternal_E = true;
  zgsz335.zotype = zgaz337;
  zgsz335.zpermit_cinvoke = true;
  zgsz335.zpermit_execute = true;
  zgsz335.zpermit_load = true;
  zgsz335.zpermit_load_cap = true;
  zgsz335.zpermit_seal = true;
  zgsz335.zpermit_set_CID = true;
  zgsz335.zpermit_store = true;
  zgsz335.zpermit_store_cap = true;
  zgsz335.zpermit_store_local_cap = true;
  zgsz335.zpermit_unseal = true;
  zgsz335.zreserved = UINT64_C(0);
  zgsz335.ztag = true;
  zgsz335.zuperms = UINT64_C(0);
  zgsz336 = zgsz335;


  zdefault_cap = zgsz336;

let_end_86: ;
}
static void kill_letbind_26(void) {
}

static uint64_t sailgen_getCapHardPerms(struct zCapability);

static uint64_t sailgen_getCapHardPerms(struct zCapability zcap)
{
  __label__ end_function_88, end_block_exception_89;

  uint64_t zcbz319;
  uint64_t zgaz370;
  {
    bool zgaz338;
    zgaz338 = zcap.zpermit_set_CID;
    zgaz370 = sailgen_bool_to_bits(zgaz338);
  }
  uint64_t zgaz371;
  {
    uint64_t zgaz368;
    {
      bool zgaz339;
      zgaz339 = zcap.zaccess_system_regs;
      zgaz368 = sailgen_bool_to_bits(zgaz339);
    }
    uint64_t zgaz369;
    {
      uint64_t zgaz366;
      {
        bool zgaz340;
        zgaz340 = zcap.zpermit_unseal;
        zgaz366 = sailgen_bool_to_bits(zgaz340);
      }
      uint64_t zgaz367;
      {
        uint64_t zgaz364;
        {
          bool zgaz341;
          zgaz341 = zcap.zpermit_cinvoke;
          zgaz364 = sailgen_bool_to_bits(zgaz341);
        }
        uint64_t zgaz365;
        {
          uint64_t zgaz362;
          {
            bool zgaz342;
            zgaz342 = zcap.zpermit_seal;
            zgaz362 = sailgen_bool_to_bits(zgaz342);
          }
          uint64_t zgaz363;
          {
            uint64_t zgaz360;
            {
              bool zgaz343;
              zgaz343 = zcap.zpermit_store_local_cap;
              zgaz360 = sailgen_bool_to_bits(zgaz343);
            }
            uint64_t zgaz361;
            {
              uint64_t zgaz358;
              {
                bool zgaz344;
                zgaz344 = zcap.zpermit_store_cap;
                zgaz358 = sailgen_bool_to_bits(zgaz344);
              }
              uint64_t zgaz359;
              {
                uint64_t zgaz356;
                {
                  bool zgaz345;
                  zgaz345 = zcap.zpermit_load_cap;
                  zgaz356 = sailgen_bool_to_bits(zgaz345);
                }
                uint64_t zgaz357;
                {
                  uint64_t zgaz354;
                  {
                    bool zgaz346;
                    zgaz346 = zcap.zpermit_store;
                    zgaz354 = sailgen_bool_to_bits(zgaz346);
                  }
                  uint64_t zgaz355;
                  {
                    uint64_t zgaz352;
                    {
                      bool zgaz347;
                      zgaz347 = zcap.zpermit_load;
                      zgaz352 = sailgen_bool_to_bits(zgaz347);
                    }
                    uint64_t zgaz353;
                    {
                      uint64_t zgaz350;
                      {
                        bool zgaz348;
                        zgaz348 = zcap.zpermit_execute;
                        zgaz350 = sailgen_bool_to_bits(zgaz348);
                      }
                      uint64_t zgaz351;
                      {
                        bool zgaz349;
                        zgaz349 = zcap.zglobal;
                        zgaz351 = sailgen_bool_to_bits(zgaz349);
                      }
                      zgaz353 = (zgaz350 << 1) | zgaz351;
                    }
                    zgaz355 = (zgaz352 << 2) | zgaz353;
                  }
                  zgaz357 = (zgaz354 << 3) | zgaz355;
                }
                zgaz359 = (zgaz356 << 4) | zgaz357;
              }
              zgaz361 = (zgaz358 << 5) | zgaz359;
            }
            zgaz363 = (zgaz360 << 6) | zgaz361;
          }
          zgaz365 = (zgaz362 << 7) | zgaz363;
        }
        zgaz367 = (zgaz364 << 8) | zgaz365;
      }
      zgaz369 = (zgaz366 << 9) | zgaz367;
    }
    zgaz371 = (zgaz368 << 10) | zgaz369;
  }
  zcbz319 = (zgaz370 << 11) | zgaz371;


end_function_88: ;
  return zcbz319;
end_block_exception_89: ;

  return UINT64_C(0xdeadc0de);
}

static struct zCapability sailgen_encCapabilityToCapability(bool, struct zEncCapability);

struct zCapability zghz310;
struct zCapability zghz311;
sail_int zghz312;
sail_int zghz313;
sail_int zghz314;
sail_int zghz315;
sail_int zghz316;
sail_int zghz317;
lbits zghz318;
lbits zghz319;
sail_int zghz320;
sail_int zghz321;
sail_int zghz322;
sail_int zghz323;
lbits zghz324;
sail_int zghz325;
sail_int zghz326;
sail_int zghz327;
sail_int zghz328;
lbits zghz329;
sail_int zghz330;
sail_int zghz331;
sail_int zghz332;
sail_int zghz333;
sail_int zghz334;
sail_int zghz335;
sail_int zghz336;
sail_int zghz337;
sail_int zghz338;

static void startup_sailgen_encCapabilityToCapability(void)
{


  CREATE(sail_int)(&zghz312);
  CREATE(sail_int)(&zghz313);
  CREATE(sail_int)(&zghz314);
  CREATE(sail_int)(&zghz315);
  CREATE(sail_int)(&zghz316);
  CREATE(sail_int)(&zghz317);
  CREATE(lbits)(&zghz318);
  CREATE(lbits)(&zghz319);
  CREATE(sail_int)(&zghz320);
  CREATE(sail_int)(&zghz321);
  CREATE(sail_int)(&zghz322);
  CREATE(sail_int)(&zghz323);
  CREATE(lbits)(&zghz324);
  CREATE(sail_int)(&zghz325);
  CREATE(sail_int)(&zghz326);
  CREATE(sail_int)(&zghz327);
  CREATE(sail_int)(&zghz328);
  CREATE(lbits)(&zghz329);
  CREATE(sail_int)(&zghz330);
  CREATE(sail_int)(&zghz331);
  CREATE(sail_int)(&zghz332);
  CREATE(sail_int)(&zghz333);
  CREATE(sail_int)(&zghz334);
  CREATE(sail_int)(&zghz335);
  CREATE(sail_int)(&zghz336);
  CREATE(sail_int)(&zghz337);
  CREATE(sail_int)(&zghz338);
}

static struct zCapability sailgen_encCapabilityToCapability(bool zt, struct zEncCapability zc)
{
  __label__ cleanup_92, end_cleanup_93, end_function_91, end_block_exception_94;

  struct zCapability zghz310;
  bool zinternal_E;
  {
    fbits zgaz3142;
    zgaz3142 = zc.zinternal_E;
    zinternal_E = sailgen_bit_to_bool(zgaz3142);
  }
  uint64_t zE;
  zE = UINT64_C(0b000000);
  uint64_t zBs;
  zBs = UINT64_C(0x00);
  uint64_t zT;
  zT = UINT64_C(0b000000);
  uint64_t zlenMSBs;
  zlenMSBs = UINT64_C(0b00);
  {
    unit zgsz337;
    if (zinternal_E) {
      {
        uint64_t zgaz376;
        {
          uint64_t zgaz372;
          zgaz372 = zc.zT;
          int64_t zgaz373;
          {
            RECREATE(sail_int)(&zghz336);
            CONVERT_OF(sail_int, mach_int)(&zghz336, zinternal_E_take_bits);
            RECREATE(sail_int)(&zghz337);
            CONVERT_OF(sail_int, mach_int)(&zghz337, INT64_C(1));
            RECREATE(sail_int)(&zghz338);
            sub_int(&zghz338, zghz336, zghz337);
            zgaz373 = CONVERT_OF(mach_int, sail_int)(zghz338);
          }
          zgaz376 = (safe_rshift(UINT64_MAX, 64 - 3) & (zgaz372 >> INT64_C(0)));
        }
        uint64_t zgaz377;
        {
          uint64_t zgaz374;
          zgaz374 = zc.zB;
          int64_t zgaz375;
          {
            RECREATE(sail_int)(&zghz333);
            CONVERT_OF(sail_int, mach_int)(&zghz333, zinternal_E_take_bits);
            RECREATE(sail_int)(&zghz334);
            CONVERT_OF(sail_int, mach_int)(&zghz334, INT64_C(1));
            RECREATE(sail_int)(&zghz335);
            sub_int(&zghz335, zghz333, zghz334);
            zgaz375 = CONVERT_OF(mach_int, sail_int)(zghz335);
          }
          zgaz377 = (safe_rshift(UINT64_MAX, 64 - 3) & (zgaz374 >> INT64_C(0)));
        }
        zE = (zgaz376 << 3) | zgaz377;
        unit zgsz342;
        zgsz342 = UNIT;
      }
      {
        zlenMSBs = UINT64_C(0b01);
        unit zgsz341;
        zgsz341 = UNIT;
      }
      {
        uint64_t zgaz380;
        {
          uint64_t zgaz378;
          zgaz378 = zc.zT;
          int64_t zgaz379;
          {
            RECREATE(sail_int)(&zghz330);
            CONVERT_OF(sail_int, mach_int)(&zghz330, zcap_mantissa_width);
            RECREATE(sail_int)(&zghz331);
            CONVERT_OF(sail_int, mach_int)(&zghz331, INT64_C(3));
            RECREATE(sail_int)(&zghz332);
            sub_int(&zghz332, zghz330, zghz331);
            zgaz379 = CONVERT_OF(mach_int, sail_int)(zghz332);
          }
          zgaz380 = (safe_rshift(UINT64_MAX, 64 - 3) & (zgaz378 >> INT64_C(3)));
        }
        uint64_t zgaz381;
        {
          RECREATE(sail_int)(&zghz328);
          CONVERT_OF(sail_int, mach_int)(&zghz328, zinternal_E_take_bits);
          RECREATE(lbits)(&zghz329);
          sailgen_zzeros_implicit(&zghz329, zghz328);
          zgaz381 = CONVERT_OF(fbits, lbits)(zghz329, true);
        }
        zT = (zgaz380 << 3) | zgaz381;
        unit zgsz340;
        zgsz340 = UNIT;
      }
      uint64_t zgaz384;
      {
        uint64_t zgaz382;
        zgaz382 = zc.zB;
        int64_t zgaz383;
        {
          RECREATE(sail_int)(&zghz325);
          CONVERT_OF(sail_int, mach_int)(&zghz325, zcap_mantissa_width);
          RECREATE(sail_int)(&zghz326);
          CONVERT_OF(sail_int, mach_int)(&zghz326, INT64_C(1));
          RECREATE(sail_int)(&zghz327);
          sub_int(&zghz327, zghz325, zghz326);
          zgaz383 = CONVERT_OF(mach_int, sail_int)(zghz327);
        }
        zgaz384 = (safe_rshift(UINT64_MAX, 64 - 5) & (zgaz382 >> INT64_C(3)));
      }
      uint64_t zgaz385;
      {
        RECREATE(sail_int)(&zghz323);
        CONVERT_OF(sail_int, mach_int)(&zghz323, zinternal_E_take_bits);
        RECREATE(lbits)(&zghz324);
        sailgen_zzeros_implicit(&zghz324, zghz323);
        zgaz385 = CONVERT_OF(fbits, lbits)(zghz324, true);
      }
      zBs = (zgaz384 << 3) | zgaz385;
      zgsz337 = UNIT;
    } else {
      {
        zlenMSBs = UINT64_C(0b00);
        unit zgsz339;
        zgsz339 = UNIT;
      }
      {
        zT = zc.zT;
        unit zgsz338;
        zgsz338 = UNIT;
      }
      zBs = zc.zB;
      zgsz337 = UNIT;
    }
  }
  uint64_t zcarry_out;
  {
    bool zgaz3141;
    {
      uint64_t zgaz3140;
      {
        int64_t zgaz3139;
        {
          RECREATE(sail_int)(&zghz320);
          CONVERT_OF(sail_int, mach_int)(&zghz320, zcap_mantissa_width);
          RECREATE(sail_int)(&zghz321);
          CONVERT_OF(sail_int, mach_int)(&zghz321, INT64_C(3));
          RECREATE(sail_int)(&zghz322);
          sub_int(&zghz322, zghz320, zghz321);
          zgaz3139 = CONVERT_OF(mach_int, sail_int)(zghz322);
        }
        zgaz3140 = (safe_rshift(UINT64_MAX, 64 - 6) & (zBs >> INT64_C(0)));
      }
      {
        RECREATE(lbits)(&zghz318);
        CONVERT_OF(lbits, fbits)(&zghz318, zT, UINT64_C(6) , true);
        RECREATE(lbits)(&zghz319);
        CONVERT_OF(lbits, fbits)(&zghz319, zgaz3140, UINT64_C(6) , true);
        zgaz3141 = sailgen_z8operatorz0zI_uz9(zghz318, zghz319);
      }
    }
    if (zgaz3141) {  zcarry_out = UINT64_C(0b01);  } else {  zcarry_out = UINT64_C(0b00);  }
  }
  uint64_t zTtop2;
  {
    uint64_t zgaz3138;
    {
      uint64_t zgaz3137;
      {
        int64_t zgaz3135;
        {
          RECREATE(sail_int)(&zghz315);
          CONVERT_OF(sail_int, mach_int)(&zghz315, zcap_mantissa_width);
          RECREATE(sail_int)(&zghz316);
          CONVERT_OF(sail_int, mach_int)(&zghz316, INT64_C(1));
          RECREATE(sail_int)(&zghz317);
          sub_int(&zghz317, zghz315, zghz316);
          zgaz3135 = CONVERT_OF(mach_int, sail_int)(zghz317);
        }
        int64_t zgaz3136;
        {
          RECREATE(sail_int)(&zghz312);
          CONVERT_OF(sail_int, mach_int)(&zghz312, zcap_mantissa_width);
          RECREATE(sail_int)(&zghz313);
          CONVERT_OF(sail_int, mach_int)(&zghz313, INT64_C(2));
          RECREATE(sail_int)(&zghz314);
          sub_int(&zghz314, zghz312, zghz313);
          zgaz3136 = CONVERT_OF(mach_int, sail_int)(zghz314);
        }
        zgaz3137 = (safe_rshift(UINT64_MAX, 64 - 2) & (zBs >> zgaz3136));
      }
      zgaz3138 = ((zgaz3137 + zlenMSBs) & UINT64_C(0b11));
    }
    zTtop2 = ((zgaz3138 + zcarry_out) & UINT64_C(0b11));
  }
  {
    uint64_t zgaz390;
    {
      bool zgaz386;
      zgaz386 = (zcap_uperms_width > INT64_C(0));
      zgaz390 = UINT64_C(0);
    }
    bool zgaz393;
    {
      fbits zgaz392;
      {
        uint64_t zgaz391;
        zgaz391 = zc.zperms;
        zgaz392 = (UINT64_C(1) & (zgaz391 >> INT64_C(11)));
      }
      zgaz393 = sailgen_bit_to_bool(zgaz392);
    }
    bool zgaz396;
    {
      fbits zgaz395;
      {
        uint64_t zgaz394;
        zgaz394 = zc.zperms;
        zgaz395 = (UINT64_C(1) & (zgaz394 >> INT64_C(10)));
      }
      zgaz396 = sailgen_bit_to_bool(zgaz395);
    }
    bool zgaz399;
    {
      fbits zgaz398;
      {
        uint64_t zgaz397;
        zgaz397 = zc.zperms;
        zgaz398 = (UINT64_C(1) & (zgaz397 >> INT64_C(9)));
      }
      zgaz399 = sailgen_bit_to_bool(zgaz398);
    }
    bool zgaz3102;
    {
      fbits zgaz3101;
      {
        uint64_t zgaz3100;
        zgaz3100 = zc.zperms;
        zgaz3101 = (UINT64_C(1) & (zgaz3100 >> INT64_C(8)));
      }
      zgaz3102 = sailgen_bit_to_bool(zgaz3101);
    }
    bool zgaz3105;
    {
      fbits zgaz3104;
      {
        uint64_t zgaz3103;
        zgaz3103 = zc.zperms;
        zgaz3104 = (UINT64_C(1) & (zgaz3103 >> INT64_C(7)));
      }
      zgaz3105 = sailgen_bit_to_bool(zgaz3104);
    }
    bool zgaz3108;
    {
      fbits zgaz3107;
      {
        uint64_t zgaz3106;
        zgaz3106 = zc.zperms;
        zgaz3107 = (UINT64_C(1) & (zgaz3106 >> INT64_C(6)));
      }
      zgaz3108 = sailgen_bit_to_bool(zgaz3107);
    }
    bool zgaz3111;
    {
      fbits zgaz3110;
      {
        uint64_t zgaz3109;
        zgaz3109 = zc.zperms;
        zgaz3110 = (UINT64_C(1) & (zgaz3109 >> INT64_C(5)));
      }
      zgaz3111 = sailgen_bit_to_bool(zgaz3110);
    }
    bool zgaz3114;
    {
      fbits zgaz3113;
      {
        uint64_t zgaz3112;
        zgaz3112 = zc.zperms;
        zgaz3113 = (UINT64_C(1) & (zgaz3112 >> INT64_C(4)));
      }
      zgaz3114 = sailgen_bit_to_bool(zgaz3113);
    }
    bool zgaz3117;
    {
      fbits zgaz3116;
      {
        uint64_t zgaz3115;
        zgaz3115 = zc.zperms;
        zgaz3116 = (UINT64_C(1) & (zgaz3115 >> INT64_C(3)));
      }
      zgaz3117 = sailgen_bit_to_bool(zgaz3116);
    }
    bool zgaz3120;
    {
      fbits zgaz3119;
      {
        uint64_t zgaz3118;
        zgaz3118 = zc.zperms;
        zgaz3119 = (UINT64_C(1) & (zgaz3118 >> INT64_C(2)));
      }
      zgaz3120 = sailgen_bit_to_bool(zgaz3119);
    }
    bool zgaz3123;
    {
      fbits zgaz3122;
      {
        uint64_t zgaz3121;
        zgaz3121 = zc.zperms;
        zgaz3122 = (UINT64_C(1) & (zgaz3121 >> INT64_C(1)));
      }
      zgaz3123 = sailgen_bit_to_bool(zgaz3122);
    }
    bool zgaz3126;
    {
      fbits zgaz3125;
      {
        uint64_t zgaz3124;
        zgaz3124 = zc.zperms;
        zgaz3125 = (UINT64_C(1) & (zgaz3124 >> INT64_C(0)));
      }
      zgaz3126 = sailgen_bit_to_bool(zgaz3125);
    }
    uint64_t zgaz3127;
    zgaz3127 = zc.zreserved;
    bool zgaz3130;
    {
      fbits zgaz3129;
      {
        uint64_t zgaz3128;
        zgaz3128 = zc.zflags;
        zgaz3129 = (UINT64_C(1) & (zgaz3128 >> INT64_C(0)));
      }
      zgaz3130 = sailgen_bit_to_bool(zgaz3129);
    }
    uint64_t zgaz3131;
    zgaz3131 = (zTtop2 << 6) | zT;
    uint64_t zgaz3132;
    zgaz3132 = zc.zotype;
    uint64_t zgaz3133;
    zgaz3133 = zc.zaddress;
    struct zCapability zghz311;
    zghz311.zB = zBs;
    zghz311.zE = zE;
    zghz311.zT = zgaz3131;
    zghz311.zaccess_system_regs = zgaz396;
    zghz311.zaddress = zgaz3133;
    zghz311.zflag_cap_mode = zgaz3130;
    zghz311.zglobal = zgaz3126;
    zghz311.zinternal_E = zinternal_E;
    zghz311.zotype = zgaz3132;
    zghz311.zpermit_cinvoke = zgaz3102;
    zghz311.zpermit_execute = zgaz3123;
    zghz311.zpermit_load = zgaz3120;
    zghz311.zpermit_load_cap = zgaz3114;
    zghz311.zpermit_seal = zgaz3105;
    zghz311.zpermit_set_CID = zgaz393;
    zghz311.zpermit_store = zgaz3117;
    zghz311.zpermit_store_cap = zgaz3111;
    zghz311.zpermit_store_local_cap = zgaz3108;
    zghz311.zpermit_unseal = zgaz399;
    zghz311.zreserved = zgaz3127;
    zghz311.ztag = zt;
    zghz311.zuperms = zgaz390;
    zghz310 = zghz311;
  }
  goto cleanup_92;
  /* unreachable after return */







  goto end_cleanup_93;
cleanup_92: ;







  goto end_function_91;
end_cleanup_93: ;
end_function_91: ;
  return zghz310;
end_block_exception_94: ;
  struct zCapability zcbz350 = { .zB = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zaddress = UINT64_C(0xdeadc0de), .zflag_cap_mode = false, .zglobal = false, .zinternal_E = false, .zotype = UINT64_C(0xdeadc0de), .zpermit_cinvoke = false, .zpermit_execute = false, .zpermit_load = false, .zpermit_load_cap = false, .zpermit_seal = false, .zpermit_set_CID = false, .zpermit_store = false, .zpermit_store_cap = false, .zpermit_store_local_cap = false, .zpermit_unseal = false, .zreserved = UINT64_C(0xdeadc0de), .ztag = false, .zuperms = UINT64_C(0xdeadc0de) };
  return zcbz350;
}



static void finish_sailgen_encCapabilityToCapability(void)
{
  KILL(sail_int)(&zghz338);
  KILL(sail_int)(&zghz337);
  KILL(sail_int)(&zghz336);
  KILL(sail_int)(&zghz335);
  KILL(sail_int)(&zghz334);
  KILL(sail_int)(&zghz333);
  KILL(sail_int)(&zghz332);
  KILL(sail_int)(&zghz331);
  KILL(sail_int)(&zghz330);
  KILL(lbits)(&zghz329);
  KILL(sail_int)(&zghz328);
  KILL(sail_int)(&zghz327);
  KILL(sail_int)(&zghz326);
  KILL(sail_int)(&zghz325);
  KILL(lbits)(&zghz324);
  KILL(sail_int)(&zghz323);
  KILL(sail_int)(&zghz322);
  KILL(sail_int)(&zghz321);
  KILL(sail_int)(&zghz320);
  KILL(lbits)(&zghz319);
  KILL(lbits)(&zghz318);
  KILL(sail_int)(&zghz317);
  KILL(sail_int)(&zghz316);
  KILL(sail_int)(&zghz315);
  KILL(sail_int)(&zghz314);
  KILL(sail_int)(&zghz313);
  KILL(sail_int)(&zghz312);


}

static struct zCapability sailgen_capBitsToCapability(bool, uint64_t);

struct zCapability zghz339;
struct zEncCapability zghz340;

static void startup_sailgen_capBitsToCapability(void)
{


}

static struct zCapability sailgen_capBitsToCapability(bool zt, uint64_t zc)
{
  __label__ end_function_96, end_block_exception_97;

  struct zCapability zghz339;
  struct zEncCapability zghz340;
  zghz340 = sailgen_capBitsToEncCapability(zc);
  zghz339 = sailgen_encCapabilityToCapability(zt, zghz340);
end_function_96: ;
  return zghz339;
end_block_exception_97: ;
  struct zCapability zcbz351 = { .zB = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zaddress = UINT64_C(0xdeadc0de), .zflag_cap_mode = false, .zglobal = false, .zinternal_E = false, .zotype = UINT64_C(0xdeadc0de), .zpermit_cinvoke = false, .zpermit_execute = false, .zpermit_load = false, .zpermit_load_cap = false, .zpermit_seal = false, .zpermit_set_CID = false, .zpermit_store = false, .zpermit_store_cap = false, .zpermit_store_local_cap = false, .zpermit_unseal = false, .zreserved = UINT64_C(0xdeadc0de), .ztag = false, .zuperms = UINT64_C(0xdeadc0de) };
  return zcbz351;
}



static void finish_sailgen_capBitsToCapability(void)
{


}

static struct zEncCapability sailgen_capToEncCap(struct zCapability);

struct zEncCapability zghz341;
struct zEncCapability zghz342;
sail_int zghz343;
sail_int zghz344;
sail_int zghz345;
sail_int zghz346;
sail_int zghz347;
sail_int zghz348;
sail_int zghz349;
sail_int zghz350;
sail_int zghz351;
sail_int zghz352;
sail_int zghz353;
sail_int zghz354;
sail_int zghz355;
sail_int zghz356;
sail_int zghz357;
sail_int zghz358;
sail_int zghz359;
sail_int zghz360;
sail_int zghz361;
sail_int zghz362;
sail_int zghz363;

static void startup_sailgen_capToEncCap(void)
{


  CREATE(sail_int)(&zghz343);
  CREATE(sail_int)(&zghz344);
  CREATE(sail_int)(&zghz345);
  CREATE(sail_int)(&zghz346);
  CREATE(sail_int)(&zghz347);
  CREATE(sail_int)(&zghz348);
  CREATE(sail_int)(&zghz349);
  CREATE(sail_int)(&zghz350);
  CREATE(sail_int)(&zghz351);
  CREATE(sail_int)(&zghz352);
  CREATE(sail_int)(&zghz353);
  CREATE(sail_int)(&zghz354);
  CREATE(sail_int)(&zghz355);
  CREATE(sail_int)(&zghz356);
  CREATE(sail_int)(&zghz357);
  CREATE(sail_int)(&zghz358);
  CREATE(sail_int)(&zghz359);
  CREATE(sail_int)(&zghz360);
  CREATE(sail_int)(&zghz361);
  CREATE(sail_int)(&zghz362);
  CREATE(sail_int)(&zghz363);
}

static struct zEncCapability sailgen_capToEncCap(struct zCapability zcap)
{
  __label__ cleanup_100, end_cleanup_101, end_function_99, end_block_exception_102;

  struct zEncCapability zghz341;
  uint64_t zt_hi;
  {
    uint64_t zgaz3170;
    zgaz3170 = zcap.zT;
    int64_t zgaz3171;
    {
      RECREATE(sail_int)(&zghz361);
      CONVERT_OF(sail_int, mach_int)(&zghz361, zcap_mantissa_width);
      RECREATE(sail_int)(&zghz362);
      CONVERT_OF(sail_int, mach_int)(&zghz362, INT64_C(3));
      RECREATE(sail_int)(&zghz363);
      sub_int(&zghz363, zghz361, zghz362);
      zgaz3171 = CONVERT_OF(mach_int, sail_int)(zghz363);
    }
    zt_hi = (safe_rshift(UINT64_MAX, 64 - 3) & (zgaz3170 >> zinternal_E_take_bits));
  }
  uint64_t zt_lo;
  {
    uint64_t zgaz3168;
    zgaz3168 = zcap.zT;
    int64_t zgaz3169;
    {
      RECREATE(sail_int)(&zghz358);
      CONVERT_OF(sail_int, mach_int)(&zghz358, zinternal_E_take_bits);
      RECREATE(sail_int)(&zghz359);
      CONVERT_OF(sail_int, mach_int)(&zghz359, INT64_C(1));
      RECREATE(sail_int)(&zghz360);
      sub_int(&zghz360, zghz358, zghz359);
      zgaz3169 = CONVERT_OF(mach_int, sail_int)(zghz360);
    }
    zt_lo = (safe_rshift(UINT64_MAX, 64 - 3) & (zgaz3168 >> INT64_C(0)));
  }
  uint64_t zb_hi;
  {
    uint64_t zgaz3166;
    zgaz3166 = zcap.zB;
    int64_t zgaz3167;
    {
      RECREATE(sail_int)(&zghz355);
      CONVERT_OF(sail_int, mach_int)(&zghz355, zcap_mantissa_width);
      RECREATE(sail_int)(&zghz356);
      CONVERT_OF(sail_int, mach_int)(&zghz356, INT64_C(1));
      RECREATE(sail_int)(&zghz357);
      sub_int(&zghz357, zghz355, zghz356);
      zgaz3167 = CONVERT_OF(mach_int, sail_int)(zghz357);
    }
    zb_hi = (safe_rshift(UINT64_MAX, 64 - 5) & (zgaz3166 >> zinternal_E_take_bits));
  }
  uint64_t zb_lo;
  {
    uint64_t zgaz3164;
    zgaz3164 = zcap.zB;
    int64_t zgaz3165;
    {
      RECREATE(sail_int)(&zghz352);
      CONVERT_OF(sail_int, mach_int)(&zghz352, zinternal_E_take_bits);
      RECREATE(sail_int)(&zghz353);
      CONVERT_OF(sail_int, mach_int)(&zghz353, INT64_C(1));
      RECREATE(sail_int)(&zghz354);
      sub_int(&zghz354, zghz352, zghz353);
      zgaz3165 = CONVERT_OF(mach_int, sail_int)(zghz354);
    }
    zb_lo = (safe_rshift(UINT64_MAX, 64 - 3) & (zgaz3164 >> INT64_C(0)));
  }
  {
    bool zgaz3144;
    zgaz3144 = zcap.zinternal_E;
    unit zgsz344;
    if (zgaz3144) {
      {
        uint64_t zgaz3146;
        zgaz3146 = zcap.zE;
        int64_t zgaz3147;
        {
          int64_t zgaz3145;
          {
            RECREATE(sail_int)(&zghz349);
            CONVERT_OF(sail_int, mach_int)(&zghz349, INT64_C(2));
            RECREATE(sail_int)(&zghz350);
            CONVERT_OF(sail_int, mach_int)(&zghz350, zinternal_E_take_bits);
            RECREATE(sail_int)(&zghz351);
            mult_int(&zghz351, zghz349, zghz350);
            zgaz3145 = CONVERT_OF(mach_int, sail_int)(zghz351);
          }
          {
            RECREATE(sail_int)(&zghz346);
            CONVERT_OF(sail_int, mach_int)(&zghz346, zgaz3145);
            RECREATE(sail_int)(&zghz347);
            CONVERT_OF(sail_int, mach_int)(&zghz347, INT64_C(1));
            RECREATE(sail_int)(&zghz348);
            sub_int(&zghz348, zghz346, zghz347);
            zgaz3147 = CONVERT_OF(mach_int, sail_int)(zghz348);
          }
        }
        zt_lo = (safe_rshift(UINT64_MAX, 64 - 3) & (zgaz3146 >> zinternal_E_take_bits));
        unit zgsz345;
        zgsz345 = UNIT;
      }
      uint64_t zgaz3148;
      zgaz3148 = zcap.zE;
      int64_t zgaz3149;
      {
        RECREATE(sail_int)(&zghz343);
        CONVERT_OF(sail_int, mach_int)(&zghz343, zinternal_E_take_bits);
        RECREATE(sail_int)(&zghz344);
        CONVERT_OF(sail_int, mach_int)(&zghz344, INT64_C(1));
        RECREATE(sail_int)(&zghz345);
        sub_int(&zghz345, zghz343, zghz344);
        zgaz3149 = CONVERT_OF(mach_int, sail_int)(zghz345);
      }
      zb_lo = (safe_rshift(UINT64_MAX, 64 - 3) & (zgaz3148 >> INT64_C(0)));
      zgsz344 = UNIT;
    } else {  zgsz344 = UNIT;  }
  }
  {
    uint64_t zgaz3152;
    {
      uint64_t zgaz3150;
      zgaz3150 = zcap.zuperms;
      uint64_t zgaz3151;
      zgaz3151 = sailgen_getCapHardPerms(zcap);
      zgaz3152 = zgaz3151;
    }
    uint64_t zgaz3153;
    zgaz3153 = zcap.zreserved;
    uint64_t zgaz3155;
    {
      bool zgaz3154;
      zgaz3154 = zcap.zflag_cap_mode;
      zgaz3155 = sailgen_bool_to_bits(zgaz3154);
    }
    uint64_t zgaz3156;
    zgaz3156 = zcap.zotype;
    fbits zgaz3159;
    {
      uint64_t zgaz3158;
      {
        bool zgaz3157;
        zgaz3157 = zcap.zinternal_E;
        zgaz3158 = sailgen_bool_to_bits(zgaz3157);
      }
      zgaz3159 = (UINT64_C(1) & (zgaz3158 >> INT64_C(0)));
    }
    uint64_t zgaz3160;
    zgaz3160 = (zt_hi << 3) | zt_lo;
    uint64_t zgaz3161;
    zgaz3161 = (zb_hi << 3) | zb_lo;
    uint64_t zgaz3162;
    zgaz3162 = zcap.zaddress;
    struct zEncCapability zghz342;
    zghz342.zB = zgaz3161;
    zghz342.zT = zgaz3160;
    zghz342.zaddress = zgaz3162;
    zghz342.zflags = zgaz3155;
    zghz342.zinternal_E = zgaz3159;
    zghz342.zotype = zgaz3156;
    zghz342.zperms = zgaz3152;
    zghz342.zreserved = zgaz3153;
    zghz341 = zghz342;
  }
  goto cleanup_100;
  /* unreachable after return */




  goto end_cleanup_101;
cleanup_100: ;




  goto end_function_99;
end_cleanup_101: ;
end_function_99: ;
  return zghz341;
end_block_exception_102: ;
  struct zEncCapability zcbz352 = { .zB = UINT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zflags = UINT64_C(0xdeadc0de), .zinternal_E = UINT64_C(0), .zotype = UINT64_C(0xdeadc0de), .zperms = UINT64_C(0xdeadc0de), .zreserved = UINT64_C(0xdeadc0de) };
  return zcbz352;
}



static void finish_sailgen_capToEncCap(void)
{
  KILL(sail_int)(&zghz363);
  KILL(sail_int)(&zghz362);
  KILL(sail_int)(&zghz361);
  KILL(sail_int)(&zghz360);
  KILL(sail_int)(&zghz359);
  KILL(sail_int)(&zghz358);
  KILL(sail_int)(&zghz357);
  KILL(sail_int)(&zghz356);
  KILL(sail_int)(&zghz355);
  KILL(sail_int)(&zghz354);
  KILL(sail_int)(&zghz353);
  KILL(sail_int)(&zghz352);
  KILL(sail_int)(&zghz351);
  KILL(sail_int)(&zghz350);
  KILL(sail_int)(&zghz349);
  KILL(sail_int)(&zghz348);
  KILL(sail_int)(&zghz347);
  KILL(sail_int)(&zghz346);
  KILL(sail_int)(&zghz345);
  KILL(sail_int)(&zghz344);
  KILL(sail_int)(&zghz343);


}

static uint64_t sailgen_capToBits(struct zCapability);

struct zEncCapability zghz364;

static void startup_sailgen_capToBits(void)
{
}

static uint64_t sailgen_capToBits(struct zCapability zcap)
{
  __label__ end_function_104, end_block_exception_105;

  uint64_t zcbz323;
  struct zEncCapability zghz364;
  zghz364 = sailgen_capToEncCap(zcap);
  zcbz323 = sailgen_encCapToBits(zghz364);
end_function_104: ;
  return zcbz323;
end_block_exception_105: ;

  return UINT64_C(0xdeadc0de);
}



static void finish_sailgen_capToBits(void)
{
}

static uint64_t znull_cap_bits;
static void create_letbind_27(void) {


  uint64_t zgsz347;
  zgsz347 = sailgen_capToBits(znull_cap);
  znull_cap_bits = zgsz347;

let_end_106: ;
}
static void kill_letbind_27(void) {
}

static uint64_t sailgen_capToMemBits(struct zCapability);

static uint64_t sailgen_capToMemBits(struct zCapability zcap)
{
  __label__ end_function_108, end_block_exception_109;

  uint64_t zcbz324;
  uint64_t zgaz3173;
  zgaz3173 = sailgen_capToBits(zcap);
  zcbz324 = (zgaz3173 ^ znull_cap_bits);

end_function_108: ;
  return zcbz324;
end_block_exception_109: ;

  return UINT64_C(0xdeadc0de);
}

static struct zCapability sailgen_memBitsToCapability(bool, uint64_t);

struct zCapability zghz365;

static void startup_sailgen_memBitsToCapability(void)
{
}

static struct zCapability sailgen_memBitsToCapability(bool ztag, uint64_t zb)
{
  __label__ end_function_111, end_block_exception_112;

  struct zCapability zghz365;
  uint64_t zgaz3174;
  zgaz3174 = (zb ^ znull_cap_bits);
  zghz365 = sailgen_capBitsToCapability(ztag, zgaz3174);

end_function_111: ;
  return zghz365;
end_block_exception_112: ;
  struct zCapability zcbz353 = { .zB = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zaddress = UINT64_C(0xdeadc0de), .zflag_cap_mode = false, .zglobal = false, .zinternal_E = false, .zotype = UINT64_C(0xdeadc0de), .zpermit_cinvoke = false, .zpermit_execute = false, .zpermit_load = false, .zpermit_load_cap = false, .zpermit_seal = false, .zpermit_set_CID = false, .zpermit_store = false, .zpermit_store_cap = false, .zpermit_store_local_cap = false, .zpermit_unseal = false, .zreserved = UINT64_C(0xdeadc0de), .ztag = false, .zuperms = UINT64_C(0xdeadc0de) };
  return zcbz353;
}



static void finish_sailgen_memBitsToCapability(void)
{
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

sail_int zghz366;
sail_int zghz367;
sail_int zghz368;
lbits zghz369;
sail_int zghz370;
lbits zghz371;
lbits zghz372;
sail_int zghz373;
sail_int zghz374;
sail_int zghz375;
sail_int zghz376;
sail_int zghz377;
sail_int zghz378;
sail_int zghz379;
sail_int zghz380;
sail_int zghz381;
sail_int zghz382;
lbits zghz383;
sail_int zghz384;
lbits zghz385;
lbits zghz386;
lbits zghz387;
sail_int zghz388;
lbits zghz389;
lbits zghz390;
sail_int zghz391;
lbits zghz392;
lbits zghz393;
sail_int zghz394;
lbits zghz395;
lbits zghz396;
lbits zghz397;
sail_int zghz398;
lbits zghz399;
lbits zghz3100;
sail_int zghz3101;
lbits zghz3102;
lbits zghz3103;
sail_int zghz3104;
lbits zghz3105;
sail_int zghz3106;
sail_int zghz3107;
sail_int zghz3108;
sail_int zghz3109;
sail_int zghz3110;
sail_int zghz3111;
lbits zghz3112;
lbits zghz3113;
lbits zghz3114;
lbits zghz3115;
lbits zghz3116;
lbits zghz3117;
lbits zghz3118;
sail_int zghz3119;
lbits zghz3120;
lbits zghz3121;
sail_int zghz3122;
lbits zghz3123;
lbits zghz3124;
sail_int zghz3125;
lbits zghz3126;
lbits zghz3127;
sail_int zghz3128;
lbits zghz3129;
sail_int zghz3130;
sail_int zghz3131;
sail_int zghz3132;
sail_int zghz3133;
sail_int zghz3134;
sail_int zghz3135;
lbits zghz3136;
sail_int zghz3137;

static void startup_sailgen_getCapBoundsBits(void)
{
  CREATE(sail_int)(&zghz366);
  CREATE(sail_int)(&zghz367);
  CREATE(sail_int)(&zghz368);
  CREATE(lbits)(&zghz369);
  CREATE(sail_int)(&zghz370);
  CREATE(lbits)(&zghz371);
  CREATE(lbits)(&zghz372);
  CREATE(sail_int)(&zghz373);
  CREATE(sail_int)(&zghz374);
  CREATE(sail_int)(&zghz375);
  CREATE(sail_int)(&zghz376);
  CREATE(sail_int)(&zghz377);
  CREATE(sail_int)(&zghz378);
  CREATE(sail_int)(&zghz379);
  CREATE(sail_int)(&zghz380);
  CREATE(sail_int)(&zghz381);
  CREATE(sail_int)(&zghz382);
  CREATE(lbits)(&zghz383);
  CREATE(sail_int)(&zghz384);
  CREATE(lbits)(&zghz385);
  CREATE(lbits)(&zghz386);
  CREATE(lbits)(&zghz387);
  CREATE(sail_int)(&zghz388);
  CREATE(lbits)(&zghz389);
  CREATE(lbits)(&zghz390);
  CREATE(sail_int)(&zghz391);
  CREATE(lbits)(&zghz392);
  CREATE(lbits)(&zghz393);
  CREATE(sail_int)(&zghz394);
  CREATE(lbits)(&zghz395);
  CREATE(lbits)(&zghz396);
  CREATE(lbits)(&zghz397);
  CREATE(sail_int)(&zghz398);
  CREATE(lbits)(&zghz399);
  CREATE(lbits)(&zghz3100);
  CREATE(sail_int)(&zghz3101);
  CREATE(lbits)(&zghz3102);
  CREATE(lbits)(&zghz3103);
  CREATE(sail_int)(&zghz3104);
  CREATE(lbits)(&zghz3105);
  CREATE(sail_int)(&zghz3106);
  CREATE(sail_int)(&zghz3107);
  CREATE(sail_int)(&zghz3108);
  CREATE(sail_int)(&zghz3109);
  CREATE(sail_int)(&zghz3110);
  CREATE(sail_int)(&zghz3111);
  CREATE(lbits)(&zghz3112);
  CREATE(lbits)(&zghz3113);
  CREATE(lbits)(&zghz3114);
  CREATE(lbits)(&zghz3115);
  CREATE(lbits)(&zghz3116);
  CREATE(lbits)(&zghz3117);
  CREATE(lbits)(&zghz3118);
  CREATE(sail_int)(&zghz3119);
  CREATE(lbits)(&zghz3120);
  CREATE(lbits)(&zghz3121);
  CREATE(sail_int)(&zghz3122);
  CREATE(lbits)(&zghz3123);
  CREATE(lbits)(&zghz3124);
  CREATE(sail_int)(&zghz3125);
  CREATE(lbits)(&zghz3126);
  CREATE(lbits)(&zghz3127);
  CREATE(sail_int)(&zghz3128);
  CREATE(lbits)(&zghz3129);
  CREATE(sail_int)(&zghz3130);
  CREATE(sail_int)(&zghz3131);
  CREATE(sail_int)(&zghz3132);
  CREATE(sail_int)(&zghz3133);
  CREATE(sail_int)(&zghz3134);
  CREATE(sail_int)(&zghz3135);
  CREATE(lbits)(&zghz3136);
  CREATE(sail_int)(&zghz3137);
}

static struct ztuple_z8z5bv32zCz0z5bv33z9 sailgen_getCapBoundsBits(struct zCapability zc)
{
  __label__ end_function_114, end_block_exception_115;

  struct ztuple_z8z5bv32zCz0z5bv33z9 zcbz326;
  int64_t zE;
  {
    int64_t zgaz3208;
    {
      uint64_t zgaz3207;
      zgaz3207 = zc.zE;
      {
        RECREATE(lbits)(&zghz3136);
        CONVERT_OF(lbits, fbits)(&zghz3136, zgaz3207, UINT64_C(6) , true);
        RECREATE(sail_int)(&zghz3137);
        sail_unsigned(&zghz3137, zghz3136);
        zgaz3208 = CONVERT_OF(mach_int, sail_int)(zghz3137);
      }
    }
    {
      RECREATE(sail_int)(&zghz3133);
      CONVERT_OF(sail_int, mach_int)(&zghz3133, zcap_max_E);
      RECREATE(sail_int)(&zghz3134);
      CONVERT_OF(sail_int, mach_int)(&zghz3134, zgaz3208);
      RECREATE(sail_int)(&zghz3135);
      min_int(&zghz3135, zghz3133, zghz3134);
      zE = CONVERT_OF(mach_int, sail_int)(zghz3135);
    }
  }
  uint64_t za;
  za = zc.zaddress;
  uint64_t za3;
  {
    uint64_t zgaz3206;
    {
      int64_t zgaz3205;
      {
        int64_t zgaz3204;
        zgaz3204 = (zE + zcap_mantissa_width);
        {
          RECREATE(sail_int)(&zghz3130);
          CONVERT_OF(sail_int, mach_int)(&zghz3130, zgaz3204);
          RECREATE(sail_int)(&zghz3131);
          CONVERT_OF(sail_int, mach_int)(&zghz3131, INT64_C(3));
          RECREATE(sail_int)(&zghz3132);
          sub_int(&zghz3132, zghz3130, zghz3131);
          zgaz3205 = CONVERT_OF(mach_int, sail_int)(zghz3132);
        }
      }
      {
        RECREATE(lbits)(&zghz3127);
        CONVERT_OF(lbits, fbits)(&zghz3127, za, UINT64_C(32) , true);
        RECREATE(sail_int)(&zghz3128);
        CONVERT_OF(sail_int, mach_int)(&zghz3128, zgaz3205);
        RECREATE(lbits)(&zghz3129);
        shiftr(&zghz3129, zghz3127, zghz3128);
        zgaz3206 = CONVERT_OF(fbits, lbits)(zghz3129, true);
      }
    }
    {
      RECREATE(lbits)(&zghz3124);
      CONVERT_OF(lbits, fbits)(&zghz3124, zgaz3206, UINT64_C(32) , true);
      RECREATE(sail_int)(&zghz3125);
      CONVERT_OF(sail_int, mach_int)(&zghz3125, INT64_C(3));
      RECREATE(lbits)(&zghz3126);
      sail_truncate(&zghz3126, zghz3124, zghz3125);
      za3 = CONVERT_OF(fbits, lbits)(zghz3126, true);
    }
  }
  uint64_t zB3;
  {
    uint64_t zgaz3203;
    zgaz3203 = zc.zB;
    {
      RECREATE(lbits)(&zghz3121);
      CONVERT_OF(lbits, fbits)(&zghz3121, zgaz3203, UINT64_C(8) , true);
      RECREATE(sail_int)(&zghz3122);
      CONVERT_OF(sail_int, mach_int)(&zghz3122, INT64_C(3));
      RECREATE(lbits)(&zghz3123);
      sail_truncateLSB(&zghz3123, zghz3121, zghz3122);
      zB3 = CONVERT_OF(fbits, lbits)(zghz3123, true);
    }
  }
  uint64_t zT3;
  {
    uint64_t zgaz3202;
    zgaz3202 = zc.zT;
    {
      RECREATE(lbits)(&zghz3118);
      CONVERT_OF(lbits, fbits)(&zghz3118, zgaz3202, UINT64_C(8) , true);
      RECREATE(sail_int)(&zghz3119);
      CONVERT_OF(sail_int, mach_int)(&zghz3119, INT64_C(3));
      RECREATE(lbits)(&zghz3120);
      sail_truncateLSB(&zghz3120, zghz3118, zghz3119);
      zT3 = CONVERT_OF(fbits, lbits)(zghz3120, true);
    }
  }
  uint64_t zR3;
  zR3 = ((zB3 - UINT64_C(0b001)) & UINT64_C(0b111));
  int64_t zaHi;
  {
    bool zgaz3201;
    {
      RECREATE(lbits)(&zghz3116);
      CONVERT_OF(lbits, fbits)(&zghz3116, za3, UINT64_C(3) , true);
      RECREATE(lbits)(&zghz3117);
      CONVERT_OF(lbits, fbits)(&zghz3117, zR3, UINT64_C(3) , true);
      zgaz3201 = sailgen_z8operatorz0zI_uz9(zghz3116, zghz3117);
    }
    if (zgaz3201) {  zaHi = INT64_C(1);  } else {  zaHi = INT64_C(0);  }
  }
  int64_t zbHi;
  {
    bool zgaz3200;
    {
      RECREATE(lbits)(&zghz3114);
      CONVERT_OF(lbits, fbits)(&zghz3114, zB3, UINT64_C(3) , true);
      RECREATE(lbits)(&zghz3115);
      CONVERT_OF(lbits, fbits)(&zghz3115, zR3, UINT64_C(3) , true);
      zgaz3200 = sailgen_z8operatorz0zI_uz9(zghz3114, zghz3115);
    }
    if (zgaz3200) {  zbHi = INT64_C(1);  } else {  zbHi = INT64_C(0);  }
  }
  int64_t ztHi;
  {
    bool zgaz3199;
    {
      RECREATE(lbits)(&zghz3112);
      CONVERT_OF(lbits, fbits)(&zghz3112, zT3, UINT64_C(3) , true);
      RECREATE(lbits)(&zghz3113);
      CONVERT_OF(lbits, fbits)(&zghz3113, zR3, UINT64_C(3) , true);
      zgaz3199 = sailgen_z8operatorz0zI_uz9(zghz3112, zghz3113);
    }
    if (zgaz3199) {  ztHi = INT64_C(1);  } else {  ztHi = INT64_C(0);  }
  }
  int64_t zcorrection_base;
  {
    RECREATE(sail_int)(&zghz3109);
    CONVERT_OF(sail_int, mach_int)(&zghz3109, zbHi);
    RECREATE(sail_int)(&zghz3110);
    CONVERT_OF(sail_int, mach_int)(&zghz3110, zaHi);
    RECREATE(sail_int)(&zghz3111);
    sub_int(&zghz3111, zghz3109, zghz3110);
    zcorrection_base = CONVERT_OF(mach_int, sail_int)(zghz3111);
  }
  int64_t zcorrection_top;
  {
    RECREATE(sail_int)(&zghz3106);
    CONVERT_OF(sail_int, mach_int)(&zghz3106, ztHi);
    RECREATE(sail_int)(&zghz3107);
    CONVERT_OF(sail_int, mach_int)(&zghz3107, zaHi);
    RECREATE(sail_int)(&zghz3108);
    sub_int(&zghz3108, zghz3106, zghz3107);
    zcorrection_top = CONVERT_OF(mach_int, sail_int)(zghz3108);
  }
  uint64_t za_top;
  {
    int64_t zgaz3198;
    zgaz3198 = (zE + zcap_mantissa_width);
    {
      RECREATE(lbits)(&zghz3103);
      CONVERT_OF(lbits, fbits)(&zghz3103, za, UINT64_C(32) , true);
      RECREATE(sail_int)(&zghz3104);
      CONVERT_OF(sail_int, mach_int)(&zghz3104, zgaz3198);
      RECREATE(lbits)(&zghz3105);
      shiftr(&zghz3105, zghz3103, zghz3104);
      za_top = CONVERT_OF(fbits, lbits)(zghz3105, true);
    }
  }
  uint64_t zbase;
  {
    RECREATE(lbits)(&zghz393);
    {
      uint64_t zgaz3195;
      {
        RECREATE(lbits)(&zghz3100);
        CONVERT_OF(lbits, fbits)(&zghz3100, za_top, UINT64_C(32) , true);
        RECREATE(sail_int)(&zghz3101);
        CONVERT_OF(sail_int, mach_int)(&zghz3101, zcorrection_base);
        RECREATE(lbits)(&zghz3102);
        add_bits_int(&zghz3102, zghz3100, zghz3101);
        zgaz3195 = CONVERT_OF(fbits, lbits)(zghz3102, true);
      }
      sbits zgaz3196;
      {
        uint64_t zgaz3193;
        zgaz3193 = zc.zB;
        sbits zgaz3194;
        {
          RECREATE(sail_int)(&zghz398);
          CONVERT_OF(sail_int, mach_int)(&zghz398, zE);
          RECREATE(lbits)(&zghz399);
          sailgen_zzeros_implicit(&zghz399, zghz398);
          zgaz3194 = CONVERT_OF(sbits, lbits)(zghz399, true);
        }
        zgaz3196 = append_fs(zgaz3193, 8, zgaz3194);
      }
      {
        RECREATE(lbits)(&zghz396);
        CONVERT_OF(lbits, fbits)(&zghz396, zgaz3195, UINT64_C(32) , true);
        RECREATE(lbits)(&zghz397);
        CONVERT_OF(lbits, sbits)(&zghz397, zgaz3196, true);
        append(&zghz393, zghz396, zghz397);
      }
    }
    {
      RECREATE(sail_int)(&zghz394);
      CONVERT_OF(sail_int, mach_int)(&zghz394, zcap_len_width);
      RECREATE(lbits)(&zghz395);
      sail_truncate(&zghz395, zghz393, zghz394);
      zbase = CONVERT_OF(fbits, lbits)(zghz395, true);
    }
  }
  uint64_t ztop;
  {
    RECREATE(lbits)(&zghz383);
    {
      uint64_t zgaz3190;
      {
        RECREATE(lbits)(&zghz390);
        CONVERT_OF(lbits, fbits)(&zghz390, za_top, UINT64_C(32) , true);
        RECREATE(sail_int)(&zghz391);
        CONVERT_OF(sail_int, mach_int)(&zghz391, zcorrection_top);
        RECREATE(lbits)(&zghz392);
        add_bits_int(&zghz392, zghz390, zghz391);
        zgaz3190 = CONVERT_OF(fbits, lbits)(zghz392, true);
      }
      sbits zgaz3191;
      {
        uint64_t zgaz3188;
        zgaz3188 = zc.zT;
        sbits zgaz3189;
        {
          RECREATE(sail_int)(&zghz388);
          CONVERT_OF(sail_int, mach_int)(&zghz388, zE);
          RECREATE(lbits)(&zghz389);
          sailgen_zzeros_implicit(&zghz389, zghz388);
          zgaz3189 = CONVERT_OF(sbits, lbits)(zghz389, true);
        }
        zgaz3191 = append_fs(zgaz3188, 8, zgaz3189);
      }
      {
        RECREATE(lbits)(&zghz386);
        CONVERT_OF(lbits, fbits)(&zghz386, zgaz3190, UINT64_C(32) , true);
        RECREATE(lbits)(&zghz387);
        CONVERT_OF(lbits, sbits)(&zghz387, zgaz3191, true);
        append(&zghz383, zghz386, zghz387);
      }
    }
    {
      RECREATE(sail_int)(&zghz384);
      CONVERT_OF(sail_int, mach_int)(&zghz384, zcap_len_width);
      RECREATE(lbits)(&zghz385);
      sail_truncate(&zghz385, zghz383, zghz384);
      ztop = CONVERT_OF(fbits, lbits)(zghz385, true);
    }
  }
  uint64_t zbase2;
  {
    uint64_t zgaz3187;
    {
      fbits zgaz3186;
      {
        int64_t zgaz3185;
        {
          RECREATE(sail_int)(&zghz380);
          CONVERT_OF(sail_int, mach_int)(&zghz380, zcap_addr_width);
          RECREATE(sail_int)(&zghz381);
          CONVERT_OF(sail_int, mach_int)(&zghz381, INT64_C(1));
          RECREATE(sail_int)(&zghz382);
          sub_int(&zghz382, zghz380, zghz381);
          zgaz3185 = CONVERT_OF(mach_int, sail_int)(zghz382);
        }
        zgaz3186 = (UINT64_C(1) & (zbase >> zgaz3185));
      }
      zgaz3187 = UINT64_C(0b0);
      zgaz3187 = update_fbits(zgaz3187, INT64_C(0), zgaz3186);
    }
    zbase2 = (UINT64_C(0b0) << 1) | zgaz3187;
  }
  uint64_t ztop2;
  {
    int64_t zgaz3184;
    {
      RECREATE(sail_int)(&zghz377);
      CONVERT_OF(sail_int, mach_int)(&zghz377, zcap_addr_width);
      RECREATE(sail_int)(&zghz378);
      CONVERT_OF(sail_int, mach_int)(&zghz378, INT64_C(1));
      RECREATE(sail_int)(&zghz379);
      sub_int(&zghz379, zghz377, zghz378);
      zgaz3184 = CONVERT_OF(mach_int, sail_int)(zghz379);
    }
    ztop2 = (safe_rshift(UINT64_MAX, 64 - 2) & (ztop >> zgaz3184));
  }
  {
    bool zgaz3179;
    {
      bool zgaz3178;
      {
        int64_t zgaz3175;
        {
          RECREATE(sail_int)(&zghz374);
          CONVERT_OF(sail_int, mach_int)(&zghz374, zcap_max_E);
          RECREATE(sail_int)(&zghz375);
          CONVERT_OF(sail_int, mach_int)(&zghz375, INT64_C(1));
          RECREATE(sail_int)(&zghz376);
          sub_int(&zghz376, zghz374, zghz375);
          zgaz3175 = CONVERT_OF(mach_int, sail_int)(zghz376);
        }
        zgaz3178 = (zE < zgaz3175);
      }
      bool zgsz349;
      if (zgaz3178) {
        int64_t zgaz3177;
        {
          uint64_t zgaz3176;
          zgaz3176 = ((ztop2 - zbase2) & UINT64_C(0b11));
          {
            RECREATE(lbits)(&zghz372);
            CONVERT_OF(lbits, fbits)(&zghz372, zgaz3176, UINT64_C(2) , true);
            RECREATE(sail_int)(&zghz373);
            sail_unsigned(&zghz373, zghz372);
            zgaz3177 = CONVERT_OF(mach_int, sail_int)(zghz373);
          }
        }
        zgsz349 = (zgaz3177 > INT64_C(1));
      } else {  zgsz349 = false;  }
      zgaz3179 = zgsz349;
    }
    unit zgsz350;
    if (zgaz3179) {
      fbits zgaz3181;
      {
        fbits zgaz3180;
        zgaz3180 = (UINT64_C(1) & (ztop >> zcap_addr_width));
        zgaz3181 = sailgen_not_bit(zgaz3180);
      }
      {
        RECREATE(lbits)(&zghz369);
        CONVERT_OF(lbits, fbits)(&zghz369, ztop, UINT64_C(33) , true);
        RECREATE(sail_int)(&zghz370);
        CONVERT_OF(sail_int, mach_int)(&zghz370, zcap_addr_width);
        RECREATE(lbits)(&zghz371);
        update_lbits(&zghz371, zghz369, zghz370, zgaz3181);
        ztop = CONVERT_OF(fbits, lbits)(zghz371, true);
      }
      zgsz350 = UNIT;
    } else {  zgsz350 = UNIT;  }
  }
  uint64_t zgaz3183;
  {
    int64_t zgaz3182;
    {
      RECREATE(sail_int)(&zghz366);
      CONVERT_OF(sail_int, mach_int)(&zghz366, zcap_addr_width);
      RECREATE(sail_int)(&zghz367);
      CONVERT_OF(sail_int, mach_int)(&zghz367, INT64_C(1));
      RECREATE(sail_int)(&zghz368);
      sub_int(&zghz368, zghz366, zghz367);
      zgaz3182 = CONVERT_OF(mach_int, sail_int)(zghz368);
    }
    zgaz3183 = (safe_rshift(UINT64_MAX, 64 - 32) & (zbase >> INT64_C(0)));
  }
  zcbz326.ztup0 = zgaz3183;
  zcbz326.ztup1 = ztop;

















end_function_114: ;
  return zcbz326;
end_block_exception_115: ;
  struct ztuple_z8z5bv32zCz0z5bv33z9 zcbz354 = { .ztup0 = UINT64_C(0xdeadc0de), .ztup1 = UINT64_C(0xdeadc0de) };
  return zcbz354;
}



static void finish_sailgen_getCapBoundsBits(void)
{
  KILL(sail_int)(&zghz3137);
  KILL(lbits)(&zghz3136);
  KILL(sail_int)(&zghz3135);
  KILL(sail_int)(&zghz3134);
  KILL(sail_int)(&zghz3133);
  KILL(sail_int)(&zghz3132);
  KILL(sail_int)(&zghz3131);
  KILL(sail_int)(&zghz3130);
  KILL(lbits)(&zghz3129);
  KILL(sail_int)(&zghz3128);
  KILL(lbits)(&zghz3127);
  KILL(lbits)(&zghz3126);
  KILL(sail_int)(&zghz3125);
  KILL(lbits)(&zghz3124);
  KILL(lbits)(&zghz3123);
  KILL(sail_int)(&zghz3122);
  KILL(lbits)(&zghz3121);
  KILL(lbits)(&zghz3120);
  KILL(sail_int)(&zghz3119);
  KILL(lbits)(&zghz3118);
  KILL(lbits)(&zghz3117);
  KILL(lbits)(&zghz3116);
  KILL(lbits)(&zghz3115);
  KILL(lbits)(&zghz3114);
  KILL(lbits)(&zghz3113);
  KILL(lbits)(&zghz3112);
  KILL(sail_int)(&zghz3111);
  KILL(sail_int)(&zghz3110);
  KILL(sail_int)(&zghz3109);
  KILL(sail_int)(&zghz3108);
  KILL(sail_int)(&zghz3107);
  KILL(sail_int)(&zghz3106);
  KILL(lbits)(&zghz3105);
  KILL(sail_int)(&zghz3104);
  KILL(lbits)(&zghz3103);
  KILL(lbits)(&zghz3102);
  KILL(sail_int)(&zghz3101);
  KILL(lbits)(&zghz3100);
  KILL(lbits)(&zghz399);
  KILL(sail_int)(&zghz398);
  KILL(lbits)(&zghz397);
  KILL(lbits)(&zghz396);
  KILL(lbits)(&zghz395);
  KILL(sail_int)(&zghz394);
  KILL(lbits)(&zghz393);
  KILL(lbits)(&zghz392);
  KILL(sail_int)(&zghz391);
  KILL(lbits)(&zghz390);
  KILL(lbits)(&zghz389);
  KILL(sail_int)(&zghz388);
  KILL(lbits)(&zghz387);
  KILL(lbits)(&zghz386);
  KILL(lbits)(&zghz385);
  KILL(sail_int)(&zghz384);
  KILL(lbits)(&zghz383);
  KILL(sail_int)(&zghz382);
  KILL(sail_int)(&zghz381);
  KILL(sail_int)(&zghz380);
  KILL(sail_int)(&zghz379);
  KILL(sail_int)(&zghz378);
  KILL(sail_int)(&zghz377);
  KILL(sail_int)(&zghz376);
  KILL(sail_int)(&zghz375);
  KILL(sail_int)(&zghz374);
  KILL(sail_int)(&zghz373);
  KILL(lbits)(&zghz372);
  KILL(lbits)(&zghz371);
  KILL(sail_int)(&zghz370);
  KILL(lbits)(&zghz369);
  KILL(sail_int)(&zghz368);
  KILL(sail_int)(&zghz367);
  KILL(sail_int)(&zghz366);
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

lbits zghz3138;
sail_int zghz3139;
lbits zghz3140;
sail_int zghz3141;

static void startup_sailgen_getCapBounds(void)
{
  CREATE(lbits)(&zghz3138);
  CREATE(sail_int)(&zghz3139);
  CREATE(lbits)(&zghz3140);
  CREATE(sail_int)(&zghz3141);
}


static struct ztuple_z8z5i64zCz0z5i64z9 sailgen_getCapBounds(struct zCapability zcap)
{
  __label__ case_118, finish_match_117, end_function_119, end_block_exception_120;

  struct ztuple_z8z5i64zCz0z5i64z9 zcbz327;
  struct ztuple_z8z5bv32zCz0z5bv33z9 zgaz3209;
  zgaz3209 = sailgen_getCapBoundsBits(zcap);
  {
    uint64_t zbase;
    zbase = zgaz3209.ztup0;
    uint64_t ztop;
    ztop = zgaz3209.ztup1;
    int64_t zgaz3210;
    {
      RECREATE(lbits)(&zghz3140);
      CONVERT_OF(lbits, fbits)(&zghz3140, zbase, UINT64_C(32) , true);
      RECREATE(sail_int)(&zghz3141);
      sail_unsigned(&zghz3141, zghz3140);
      zgaz3210 = CONVERT_OF(mach_int, sail_int)(zghz3141);
    }
    int64_t zgaz3211;
    {
      RECREATE(lbits)(&zghz3138);
      CONVERT_OF(lbits, fbits)(&zghz3138, ztop, UINT64_C(33) , true);
      RECREATE(sail_int)(&zghz3139);
      sail_unsigned(&zghz3139, zghz3138);
      zgaz3211 = CONVERT_OF(mach_int, sail_int)(zghz3139);
    }
    struct ztuple_z8z5i64zCz0z5i64z9 zgsz353;
    zgsz353.ztup0 = zgaz3210;
    zgsz353.ztup1 = zgaz3211;
    zcbz327 = zgsz353;
    goto finish_match_117;
  }
case_118: ;
  sail_match_failure("getCapBounds");
finish_match_117: ;

end_function_119: ;
  return zcbz327;
end_block_exception_120: ;
  struct ztuple_z8z5i64zCz0z5i64z9 zcbz355 = { .ztup0 = INT64_C(0xdeadc0de), .ztup1 = INT64_C(0xdeadc0de) };
  return zcbz355;
}



static void finish_sailgen_getCapBounds(void)
{
  KILL(sail_int)(&zghz3141);
  KILL(lbits)(&zghz3140);
  KILL(sail_int)(&zghz3139);
  KILL(lbits)(&zghz3138);
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

struct zCapability zghz3142;
sail_int zghz3143;
sail_int zghz3144;
lbits zghz3145;
lbits zghz3146;
sail_int zghz3147;
lbits zghz3148;
lbits zghz3149;
sail_int zghz3150;
lbits zghz3151;
sail_int zghz3152;
sail_int zghz3153;
sail_int zghz3154;
lbits zghz3155;
sail_int zghz3156;
lbits zghz3157;
lbits zghz3158;
sail_int zghz3159;
lbits zghz3160;
sail_int zghz3161;
sail_int zghz3162;
sail_int zghz3163;
lbits zghz3164;
sail_int zghz3165;
lbits zghz3166;
sail_int zghz3167;
sail_int zghz3168;
sail_int zghz3169;
lbits zghz3170;
sail_int zghz3171;
lbits zghz3172;
sail_int zghz3173;
sail_int zghz3174;
lbits zghz3175;
sail_int zghz3176;
sail_int zghz3177;
sail_int zghz3178;
lbits zghz3179;
sail_int zghz3180;
sail_int zghz3181;
lbits zghz3182;
lbits zghz3183;
sail_int zghz3184;
lbits zghz3185;
lbits zghz3186;
sail_int zghz3187;
lbits zghz3188;
sail_int zghz3189;
sail_int zghz3190;
sail_int zghz3191;
lbits zghz3192;
sail_int zghz3193;
lbits zghz3194;
lbits zghz3195;
sail_int zghz3196;
lbits zghz3197;
sail_int zghz3198;
sail_int zghz3199;
sail_int zghz3200;
lbits zghz3201;
sail_int zghz3202;
lbits zghz3203;
lbits zghz3204;
sail_int zghz3205;
lbits zghz3206;
lbits zghz3207;
sail_int zghz3208;
lbits zghz3209;
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
sail_int zghz3220;
sail_int zghz3221;
sail_int zghz3222;

static void startup_sailgen_setCapBounds(void)
{

  CREATE(sail_int)(&zghz3143);
  CREATE(sail_int)(&zghz3144);
  CREATE(lbits)(&zghz3145);
  CREATE(lbits)(&zghz3146);
  CREATE(sail_int)(&zghz3147);
  CREATE(lbits)(&zghz3148);
  CREATE(lbits)(&zghz3149);
  CREATE(sail_int)(&zghz3150);
  CREATE(lbits)(&zghz3151);
  CREATE(sail_int)(&zghz3152);
  CREATE(sail_int)(&zghz3153);
  CREATE(sail_int)(&zghz3154);
  CREATE(lbits)(&zghz3155);
  CREATE(sail_int)(&zghz3156);
  CREATE(lbits)(&zghz3157);
  CREATE(lbits)(&zghz3158);
  CREATE(sail_int)(&zghz3159);
  CREATE(lbits)(&zghz3160);
  CREATE(sail_int)(&zghz3161);
  CREATE(sail_int)(&zghz3162);
  CREATE(sail_int)(&zghz3163);
  CREATE(lbits)(&zghz3164);
  CREATE(sail_int)(&zghz3165);
  CREATE(lbits)(&zghz3166);
  CREATE(sail_int)(&zghz3167);
  CREATE(sail_int)(&zghz3168);
  CREATE(sail_int)(&zghz3169);
  CREATE(lbits)(&zghz3170);
  CREATE(sail_int)(&zghz3171);
  CREATE(lbits)(&zghz3172);
  CREATE(sail_int)(&zghz3173);
  CREATE(sail_int)(&zghz3174);
  CREATE(lbits)(&zghz3175);
  CREATE(sail_int)(&zghz3176);
  CREATE(sail_int)(&zghz3177);
  CREATE(sail_int)(&zghz3178);
  CREATE(lbits)(&zghz3179);
  CREATE(sail_int)(&zghz3180);
  CREATE(sail_int)(&zghz3181);
  CREATE(lbits)(&zghz3182);
  CREATE(lbits)(&zghz3183);
  CREATE(sail_int)(&zghz3184);
  CREATE(lbits)(&zghz3185);
  CREATE(lbits)(&zghz3186);
  CREATE(sail_int)(&zghz3187);
  CREATE(lbits)(&zghz3188);
  CREATE(sail_int)(&zghz3189);
  CREATE(sail_int)(&zghz3190);
  CREATE(sail_int)(&zghz3191);
  CREATE(lbits)(&zghz3192);
  CREATE(sail_int)(&zghz3193);
  CREATE(lbits)(&zghz3194);
  CREATE(lbits)(&zghz3195);
  CREATE(sail_int)(&zghz3196);
  CREATE(lbits)(&zghz3197);
  CREATE(sail_int)(&zghz3198);
  CREATE(sail_int)(&zghz3199);
  CREATE(sail_int)(&zghz3200);
  CREATE(lbits)(&zghz3201);
  CREATE(sail_int)(&zghz3202);
  CREATE(lbits)(&zghz3203);
  CREATE(lbits)(&zghz3204);
  CREATE(sail_int)(&zghz3205);
  CREATE(lbits)(&zghz3206);
  CREATE(lbits)(&zghz3207);
  CREATE(sail_int)(&zghz3208);
  CREATE(lbits)(&zghz3209);
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
  CREATE(sail_int)(&zghz3220);
  CREATE(sail_int)(&zghz3221);
  CREATE(sail_int)(&zghz3222);
}

static struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 sailgen_setCapBounds(struct zCapability zcap, uint64_t zbase, uint64_t ztop)
{
  __label__ end_function_122, end_block_exception_123;

  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz328;
  uint64_t zext_base;
  zext_base = (UINT64_C(0b0) << 32) | zbase;
  uint64_t zlength;
  zlength = ((ztop - zext_base) & UINT64_C(0b111111111111111111111111111111111));
  int64_t ze;
  {
    int64_t zgaz3244;
    {
      uint64_t zgaz3243;
      {
        int64_t zgaz3242;
        {
          RECREATE(sail_int)(&zghz3220);
          CONVERT_OF(sail_int, mach_int)(&zghz3220, zcap_mantissa_width);
          RECREATE(sail_int)(&zghz3221);
          CONVERT_OF(sail_int, mach_int)(&zghz3221, INT64_C(1));
          RECREATE(sail_int)(&zghz3222);
          sub_int(&zghz3222, zghz3220, zghz3221);
          zgaz3242 = CONVERT_OF(mach_int, sail_int)(zghz3222);
        }
        zgaz3243 = (safe_rshift(UINT64_MAX, 64 - 26) & (zlength >> zgaz3242));
      }
      {
        RECREATE(lbits)(&zghz3218);
        CONVERT_OF(lbits, fbits)(&zghz3218, zgaz3243, UINT64_C(26) , true);
        RECREATE(sail_int)(&zghz3219);
        count_leading_zeros(&zghz3219, zghz3218);
        zgaz3244 = CONVERT_OF(mach_int, sail_int)(zghz3219);
      }
    }
    {
      RECREATE(sail_int)(&zghz3215);
      CONVERT_OF(sail_int, mach_int)(&zghz3215, zcap_max_E);
      RECREATE(sail_int)(&zghz3216);
      CONVERT_OF(sail_int, mach_int)(&zghz3216, zgaz3244);
      RECREATE(sail_int)(&zghz3217);
      sub_int(&zghz3217, zghz3215, zghz3216);
      ze = CONVERT_OF(mach_int, sail_int)(zghz3217);
    }
  }
  bool zie;
  {
    bool zgaz3241;
    {
      RECREATE(sail_int)(&zghz3213);
      CONVERT_OF(sail_int, mach_int)(&zghz3213, ze);
      RECREATE(sail_int)(&zghz3214);
      CONVERT_OF(sail_int, mach_int)(&zghz3214, INT64_C(0));
      zgaz3241 = sailgen_neq_int(zghz3213, zghz3214);
    }
    bool zgsz355;
    if (zgaz3241) {  zgsz355 = true;  } else {
      fbits zgaz3240;
      {
        int64_t zgaz3239;
        {
          RECREATE(sail_int)(&zghz3210);
          CONVERT_OF(sail_int, mach_int)(&zghz3210, zcap_mantissa_width);
          RECREATE(sail_int)(&zghz3211);
          CONVERT_OF(sail_int, mach_int)(&zghz3211, INT64_C(2));
          RECREATE(sail_int)(&zghz3212);
          sub_int(&zghz3212, zghz3210, zghz3211);
          zgaz3239 = CONVERT_OF(mach_int, sail_int)(zghz3212);
        }
        zgaz3240 = (UINT64_C(1) & (zlength >> zgaz3239));
      }
      zgsz355 = eq_bit(zgaz3240, UINT64_C(1));
    }
    zie = zgsz355;
  }
  uint64_t zBbits;
  {
    RECREATE(lbits)(&zghz3207);
    CONVERT_OF(lbits, fbits)(&zghz3207, zbase, UINT64_C(32) , true);
    RECREATE(sail_int)(&zghz3208);
    CONVERT_OF(sail_int, mach_int)(&zghz3208, zcap_mantissa_width);
    RECREATE(lbits)(&zghz3209);
    sail_truncate(&zghz3209, zghz3207, zghz3208);
    zBbits = CONVERT_OF(fbits, lbits)(zghz3209, true);
  }
  uint64_t zTbits;
  {
    RECREATE(lbits)(&zghz3204);
    CONVERT_OF(lbits, fbits)(&zghz3204, ztop, UINT64_C(33) , true);
    RECREATE(sail_int)(&zghz3205);
    CONVERT_OF(sail_int, mach_int)(&zghz3205, zcap_mantissa_width);
    RECREATE(lbits)(&zghz3206);
    sail_truncate(&zghz3206, zghz3204, zghz3205);
    zTbits = CONVERT_OF(fbits, lbits)(zghz3206, true);
  }
  bool zlostSignificantTop;
  zlostSignificantTop = false;
  bool zlostSignificantBase;
  zlostSignificantBase = false;
  bool zincE;
  zincE = false;
  {
    unit zgsz356;
    if (zie) {
      uint64_t zB_ie;
      {
        uint64_t zgaz3234;
        {
          int64_t zgaz3233;
          zgaz3233 = (ze + INT64_C(3));
          {
            RECREATE(lbits)(&zghz3201);
            CONVERT_OF(lbits, fbits)(&zghz3201, zbase, UINT64_C(32) , true);
            RECREATE(sail_int)(&zghz3202);
            CONVERT_OF(sail_int, mach_int)(&zghz3202, zgaz3233);
            RECREATE(lbits)(&zghz3203);
            shiftr(&zghz3203, zghz3201, zghz3202);
            zgaz3234 = CONVERT_OF(fbits, lbits)(zghz3203, true);
          }
        }
        int64_t zgaz3235;
        {
          RECREATE(sail_int)(&zghz3198);
          CONVERT_OF(sail_int, mach_int)(&zghz3198, zcap_mantissa_width);
          RECREATE(sail_int)(&zghz3199);
          CONVERT_OF(sail_int, mach_int)(&zghz3199, INT64_C(3));
          RECREATE(sail_int)(&zghz3200);
          sub_int(&zghz3200, zghz3198, zghz3199);
          zgaz3235 = CONVERT_OF(mach_int, sail_int)(zghz3200);
        }
        {
          RECREATE(lbits)(&zghz3195);
          CONVERT_OF(lbits, fbits)(&zghz3195, zgaz3234, UINT64_C(32) , true);
          RECREATE(sail_int)(&zghz3196);
          CONVERT_OF(sail_int, mach_int)(&zghz3196, zgaz3235);
          RECREATE(lbits)(&zghz3197);
          sail_truncate(&zghz3197, zghz3195, zghz3196);
          zB_ie = CONVERT_OF(fbits, lbits)(zghz3197, true);
        }
      }
      uint64_t zT_ie;
      {
        uint64_t zgaz3231;
        {
          int64_t zgaz3230;
          zgaz3230 = (ze + INT64_C(3));
          {
            RECREATE(lbits)(&zghz3192);
            CONVERT_OF(lbits, fbits)(&zghz3192, ztop, UINT64_C(33) , true);
            RECREATE(sail_int)(&zghz3193);
            CONVERT_OF(sail_int, mach_int)(&zghz3193, zgaz3230);
            RECREATE(lbits)(&zghz3194);
            shiftr(&zghz3194, zghz3192, zghz3193);
            zgaz3231 = CONVERT_OF(fbits, lbits)(zghz3194, true);
          }
        }
        int64_t zgaz3232;
        {
          RECREATE(sail_int)(&zghz3189);
          CONVERT_OF(sail_int, mach_int)(&zghz3189, zcap_mantissa_width);
          RECREATE(sail_int)(&zghz3190);
          CONVERT_OF(sail_int, mach_int)(&zghz3190, INT64_C(3));
          RECREATE(sail_int)(&zghz3191);
          sub_int(&zghz3191, zghz3189, zghz3190);
          zgaz3232 = CONVERT_OF(mach_int, sail_int)(zghz3191);
        }
        {
          RECREATE(lbits)(&zghz3186);
          CONVERT_OF(lbits, fbits)(&zghz3186, zgaz3231, UINT64_C(33) , true);
          RECREATE(sail_int)(&zghz3187);
          CONVERT_OF(sail_int, mach_int)(&zghz3187, zgaz3232);
          RECREATE(lbits)(&zghz3188);
          sail_truncate(&zghz3188, zghz3186, zghz3187);
          zT_ie = CONVERT_OF(fbits, lbits)(zghz3188, true);
        }
      }
      uint64_t zmaskLo;
      {
        sbits zgaz3229;
        {
          int64_t zgaz3228;
          zgaz3228 = (ze + INT64_C(3));
          {
            RECREATE(sail_int)(&zghz3184);
            CONVERT_OF(sail_int, mach_int)(&zghz3184, zgaz3228);
            RECREATE(lbits)(&zghz3185);
            sailgen_ones(&zghz3185, zghz3184);
            zgaz3229 = CONVERT_OF(sbits, lbits)(zghz3185, true);
          }
        }
        {
          RECREATE(sail_int)(&zghz3181);
          CONVERT_OF(sail_int, mach_int)(&zghz3181, INT64_C(33));
          RECREATE(lbits)(&zghz3182);
          CONVERT_OF(lbits, sbits)(&zghz3182, zgaz3229, true);
          RECREATE(lbits)(&zghz3183);
          sailgen_zzero_extend(&zghz3183, zghz3181, zghz3182);
          zmaskLo = CONVERT_OF(fbits, lbits)(zghz3183, true);
        }
      }
      {
        int64_t zgaz3213;
        {
          uint64_t zgaz3212;
          zgaz3212 = (zext_base & zmaskLo);
          {
            RECREATE(lbits)(&zghz3179);
            CONVERT_OF(lbits, fbits)(&zghz3179, zgaz3212, UINT64_C(33) , true);
            RECREATE(sail_int)(&zghz3180);
            sail_unsigned(&zghz3180, zghz3179);
            zgaz3213 = CONVERT_OF(mach_int, sail_int)(zghz3180);
          }
        }
        {
          RECREATE(sail_int)(&zghz3177);
          CONVERT_OF(sail_int, mach_int)(&zghz3177, zgaz3213);
          RECREATE(sail_int)(&zghz3178);
          CONVERT_OF(sail_int, mach_int)(&zghz3178, INT64_C(0));
          zlostSignificantBase = sailgen_neq_int(zghz3177, zghz3178);
        }
        unit zgsz359;
        zgsz359 = UNIT;
      }
      {
        int64_t zgaz3215;
        {
          uint64_t zgaz3214;
          zgaz3214 = (ztop & zmaskLo);
          {
            RECREATE(lbits)(&zghz3175);
            CONVERT_OF(lbits, fbits)(&zghz3175, zgaz3214, UINT64_C(33) , true);
            RECREATE(sail_int)(&zghz3176);
            sail_unsigned(&zghz3176, zghz3175);
            zgaz3215 = CONVERT_OF(mach_int, sail_int)(zghz3176);
          }
        }
        {
          RECREATE(sail_int)(&zghz3173);
          CONVERT_OF(sail_int, mach_int)(&zghz3173, zgaz3215);
          RECREATE(sail_int)(&zghz3174);
          CONVERT_OF(sail_int, mach_int)(&zghz3174, INT64_C(0));
          zlostSignificantTop = sailgen_neq_int(zghz3173, zghz3174);
        }
        unit zgsz358;
        zgsz358 = UNIT;
      }
      {
        unit zgsz357;
        if (zlostSignificantTop) {
          {
            RECREATE(lbits)(&zghz3170);
            CONVERT_OF(lbits, fbits)(&zghz3170, zT_ie, UINT64_C(5) , true);
            RECREATE(sail_int)(&zghz3171);
            CONVERT_OF(sail_int, mach_int)(&zghz3171, INT64_C(1));
            RECREATE(lbits)(&zghz3172);
            add_bits_int(&zghz3172, zghz3170, zghz3171);
            zT_ie = CONVERT_OF(fbits, lbits)(zghz3172, true);
          }
          zgsz357 = UNIT;
        } else {  zgsz357 = UNIT;  }
      }
      uint64_t zlen_ie;
      zlen_ie = ((zT_ie - zB_ie) & UINT64_C(0b11111));
      {
        bool zgaz3218;
        {
          fbits zgaz3217;
          {
            int64_t zgaz3216;
            {
              RECREATE(sail_int)(&zghz3167);
              CONVERT_OF(sail_int, mach_int)(&zghz3167, zcap_mantissa_width);
              RECREATE(sail_int)(&zghz3168);
              CONVERT_OF(sail_int, mach_int)(&zghz3168, INT64_C(4));
              RECREATE(sail_int)(&zghz3169);
              sub_int(&zghz3169, zghz3167, zghz3168);
              zgaz3216 = CONVERT_OF(mach_int, sail_int)(zghz3169);
            }
            zgaz3217 = (UINT64_C(1) & (zlen_ie >> zgaz3216));
          }
          zgaz3218 = eq_bit(zgaz3217, UINT64_C(1));
        }
        unit zgsz361;
        if (zgaz3218) {
          {
            zincE = true;
            unit zgsz367;
            zgsz367 = UNIT;
          }
          {
            bool zgsz362;
            if (zlostSignificantBase) {  zgsz362 = true;  } else {
              fbits zgaz3219;
              zgaz3219 = (UINT64_C(1) & (zB_ie >> INT64_C(0)));
              zgsz362 = eq_bit(zgaz3219, UINT64_C(1));
            }
            zlostSignificantBase = zgsz362;
            unit zgsz366;
            zgsz366 = UNIT;
          }
          {
            bool zgsz363;
            if (zlostSignificantTop) {  zgsz363 = true;  } else {
              fbits zgaz3220;
              zgaz3220 = (UINT64_C(1) & (zT_ie >> INT64_C(0)));
              zgsz363 = eq_bit(zgaz3220, UINT64_C(1));
            }
            zlostSignificantTop = zgsz363;
            unit zgsz365;
            zgsz365 = UNIT;
          }
          {
            uint64_t zgaz3222;
            {
              int64_t zgaz3221;
              zgaz3221 = (ze + INT64_C(4));
              {
                RECREATE(lbits)(&zghz3164);
                CONVERT_OF(lbits, fbits)(&zghz3164, zbase, UINT64_C(32) , true);
                RECREATE(sail_int)(&zghz3165);
                CONVERT_OF(sail_int, mach_int)(&zghz3165, zgaz3221);
                RECREATE(lbits)(&zghz3166);
                shiftr(&zghz3166, zghz3164, zghz3165);
                zgaz3222 = CONVERT_OF(fbits, lbits)(zghz3166, true);
              }
            }
            int64_t zgaz3223;
            {
              RECREATE(sail_int)(&zghz3161);
              CONVERT_OF(sail_int, mach_int)(&zghz3161, zcap_mantissa_width);
              RECREATE(sail_int)(&zghz3162);
              CONVERT_OF(sail_int, mach_int)(&zghz3162, INT64_C(3));
              RECREATE(sail_int)(&zghz3163);
              sub_int(&zghz3163, zghz3161, zghz3162);
              zgaz3223 = CONVERT_OF(mach_int, sail_int)(zghz3163);
            }
            {
              RECREATE(lbits)(&zghz3158);
              CONVERT_OF(lbits, fbits)(&zghz3158, zgaz3222, UINT64_C(32) , true);
              RECREATE(sail_int)(&zghz3159);
              CONVERT_OF(sail_int, mach_int)(&zghz3159, zgaz3223);
              RECREATE(lbits)(&zghz3160);
              sail_truncate(&zghz3160, zghz3158, zghz3159);
              zB_ie = CONVERT_OF(fbits, lbits)(zghz3160, true);
            }
            unit zgsz364;
            zgsz364 = UNIT;
          }
          int64_t zincT;
          if (zlostSignificantTop) {  zincT = INT64_C(1);  } else {  zincT = INT64_C(0);  }
          uint64_t zgaz3227;
          {
            uint64_t zgaz3225;
            {
              int64_t zgaz3224;
              zgaz3224 = (ze + INT64_C(4));
              {
                RECREATE(lbits)(&zghz3155);
                CONVERT_OF(lbits, fbits)(&zghz3155, ztop, UINT64_C(33) , true);
                RECREATE(sail_int)(&zghz3156);
                CONVERT_OF(sail_int, mach_int)(&zghz3156, zgaz3224);
                RECREATE(lbits)(&zghz3157);
                shiftr(&zghz3157, zghz3155, zghz3156);
                zgaz3225 = CONVERT_OF(fbits, lbits)(zghz3157, true);
              }
            }
            int64_t zgaz3226;
            {
              RECREATE(sail_int)(&zghz3152);
              CONVERT_OF(sail_int, mach_int)(&zghz3152, zcap_mantissa_width);
              RECREATE(sail_int)(&zghz3153);
              CONVERT_OF(sail_int, mach_int)(&zghz3153, INT64_C(3));
              RECREATE(sail_int)(&zghz3154);
              sub_int(&zghz3154, zghz3152, zghz3153);
              zgaz3226 = CONVERT_OF(mach_int, sail_int)(zghz3154);
            }
            {
              RECREATE(lbits)(&zghz3149);
              CONVERT_OF(lbits, fbits)(&zghz3149, zgaz3225, UINT64_C(33) , true);
              RECREATE(sail_int)(&zghz3150);
              CONVERT_OF(sail_int, mach_int)(&zghz3150, zgaz3226);
              RECREATE(lbits)(&zghz3151);
              sail_truncate(&zghz3151, zghz3149, zghz3150);
              zgaz3227 = CONVERT_OF(fbits, lbits)(zghz3151, true);
            }
          }
          {
            RECREATE(lbits)(&zghz3146);
            CONVERT_OF(lbits, fbits)(&zghz3146, zgaz3227, UINT64_C(5) , true);
            RECREATE(sail_int)(&zghz3147);
            CONVERT_OF(sail_int, mach_int)(&zghz3147, zincT);
            RECREATE(lbits)(&zghz3148);
            add_bits_int(&zghz3148, zghz3146, zghz3147);
            zT_ie = CONVERT_OF(fbits, lbits)(zghz3148, true);
          }
          zgsz361 = UNIT;
        } else {  zgsz361 = UNIT;  }
      }
      {
        zBbits = (zB_ie << 3) | UINT64_C(0b000);
        unit zgsz360;
        zgsz360 = UNIT;
      }
      zTbits = (zT_ie << 3) | UINT64_C(0b000);
      zgsz356 = UNIT;
    } else {  zgsz356 = UNIT;  }
  }
  struct zCapability zghz3142;
  {
    uint64_t zgaz3238;
    {
      int64_t zgaz3237;
      if (zincE) {  zgaz3237 = (ze + INT64_C(1));  } else {  zgaz3237 = ze;  }
      {
        RECREATE(sail_int)(&zghz3143);
        CONVERT_OF(sail_int, mach_int)(&zghz3143, INT64_C(6));
        RECREATE(sail_int)(&zghz3144);
        CONVERT_OF(sail_int, mach_int)(&zghz3144, zgaz3237);
        RECREATE(lbits)(&zghz3145);
        sailgen_to_bits(&zghz3145, zghz3143, zghz3144);
        zgaz3238 = CONVERT_OF(fbits, lbits)(zghz3145, true);
      }
    }
    zghz3142 = zcap;
    zghz3142.zB = zBbits;
    zghz3142.zE = zgaz3238;
    zghz3142.zT = zTbits;
    zghz3142.zaddress = zbase;
    zghz3142.zinternal_E = zie;
  }
  bool zexact;
  {
    bool zgaz3236;
    {    if (zlostSignificantBase) {  zgaz3236 = true;  } else {  zgaz3236 = zlostSignificantTop;  }
    }
    zexact = sailgen_not(zgaz3236);
  }
  zcbz328.ztup0 = zexact;
  zcbz328.ztup1 = zghz3142;










end_function_122: ;
  return zcbz328;
end_block_exception_123: ;
  struct zCapability zcbz357 = { .zB = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zaddress = UINT64_C(0xdeadc0de), .zflag_cap_mode = false, .zglobal = false, .zinternal_E = false, .zotype = UINT64_C(0xdeadc0de), .zpermit_cinvoke = false, .zpermit_execute = false, .zpermit_load = false, .zpermit_load_cap = false, .zpermit_seal = false, .zpermit_set_CID = false, .zpermit_store = false, .zpermit_store_cap = false, .zpermit_store_local_cap = false, .zpermit_unseal = false, .zreserved = UINT64_C(0xdeadc0de), .ztag = false, .zuperms = UINT64_C(0xdeadc0de) };
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz356 = { .ztup0 = false, .ztup1 = zcbz357 };
  return zcbz356;
}



static void finish_sailgen_setCapBounds(void)
{
  KILL(sail_int)(&zghz3222);
  KILL(sail_int)(&zghz3221);
  KILL(sail_int)(&zghz3220);
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
  KILL(lbits)(&zghz3209);
  KILL(sail_int)(&zghz3208);
  KILL(lbits)(&zghz3207);
  KILL(lbits)(&zghz3206);
  KILL(sail_int)(&zghz3205);
  KILL(lbits)(&zghz3204);
  KILL(lbits)(&zghz3203);
  KILL(sail_int)(&zghz3202);
  KILL(lbits)(&zghz3201);
  KILL(sail_int)(&zghz3200);
  KILL(sail_int)(&zghz3199);
  KILL(sail_int)(&zghz3198);
  KILL(lbits)(&zghz3197);
  KILL(sail_int)(&zghz3196);
  KILL(lbits)(&zghz3195);
  KILL(lbits)(&zghz3194);
  KILL(sail_int)(&zghz3193);
  KILL(lbits)(&zghz3192);
  KILL(sail_int)(&zghz3191);
  KILL(sail_int)(&zghz3190);
  KILL(sail_int)(&zghz3189);
  KILL(lbits)(&zghz3188);
  KILL(sail_int)(&zghz3187);
  KILL(lbits)(&zghz3186);
  KILL(lbits)(&zghz3185);
  KILL(sail_int)(&zghz3184);
  KILL(lbits)(&zghz3183);
  KILL(lbits)(&zghz3182);
  KILL(sail_int)(&zghz3181);
  KILL(sail_int)(&zghz3180);
  KILL(lbits)(&zghz3179);
  KILL(sail_int)(&zghz3178);
  KILL(sail_int)(&zghz3177);
  KILL(sail_int)(&zghz3176);
  KILL(lbits)(&zghz3175);
  KILL(sail_int)(&zghz3174);
  KILL(sail_int)(&zghz3173);
  KILL(lbits)(&zghz3172);
  KILL(sail_int)(&zghz3171);
  KILL(lbits)(&zghz3170);
  KILL(sail_int)(&zghz3169);
  KILL(sail_int)(&zghz3168);
  KILL(sail_int)(&zghz3167);
  KILL(lbits)(&zghz3166);
  KILL(sail_int)(&zghz3165);
  KILL(lbits)(&zghz3164);
  KILL(sail_int)(&zghz3163);
  KILL(sail_int)(&zghz3162);
  KILL(sail_int)(&zghz3161);
  KILL(lbits)(&zghz3160);
  KILL(sail_int)(&zghz3159);
  KILL(lbits)(&zghz3158);
  KILL(lbits)(&zghz3157);
  KILL(sail_int)(&zghz3156);
  KILL(lbits)(&zghz3155);
  KILL(sail_int)(&zghz3154);
  KILL(sail_int)(&zghz3153);
  KILL(sail_int)(&zghz3152);
  KILL(lbits)(&zghz3151);
  KILL(sail_int)(&zghz3150);
  KILL(lbits)(&zghz3149);
  KILL(lbits)(&zghz3148);
  KILL(sail_int)(&zghz3147);
  KILL(lbits)(&zghz3146);
  KILL(lbits)(&zghz3145);
  KILL(sail_int)(&zghz3144);
  KILL(sail_int)(&zghz3143);

}

static uint64_t sailgen_getCapPerms(struct zCapability);

sail_int zghz3223;
lbits zghz3224;
lbits zghz3225;

static void startup_sailgen_getCapPerms(void)
{
  CREATE(sail_int)(&zghz3223);
  CREATE(lbits)(&zghz3224);
  CREATE(lbits)(&zghz3225);
}

static uint64_t sailgen_getCapPerms(struct zCapability zcap)
{
  __label__ end_function_125, end_block_exception_126;

  uint64_t zcbz329;
  uint64_t zperms;
  {
    uint64_t zgaz3246;
    zgaz3246 = sailgen_getCapHardPerms(zcap);
    {
      RECREATE(sail_int)(&zghz3223);
      CONVERT_OF(sail_int, mach_int)(&zghz3223, INT64_C(15));
      RECREATE(lbits)(&zghz3224);
      CONVERT_OF(lbits, fbits)(&zghz3224, zgaz3246, UINT64_C(12) , true);
      RECREATE(lbits)(&zghz3225);
      sailgen_zzero_extend(&zghz3225, zghz3223, zghz3224);
      zperms = CONVERT_OF(fbits, lbits)(zghz3225, true);
    }
  }
  uint64_t zgaz3245;
  zgaz3245 = zcap.zuperms;
  zcbz329 = zperms;


end_function_125: ;
  return zcbz329;
end_block_exception_126: ;

  return UINT64_C(0xdeadc0de);
}



static void finish_sailgen_getCapPerms(void)
{
  KILL(lbits)(&zghz3225);
  KILL(lbits)(&zghz3224);
  KILL(sail_int)(&zghz3223);
}

static struct zCapability sailgen_setCapPerms(struct zCapability, uint64_t);

struct zCapability zghz3226;
lbits zghz3227;
sail_int zghz3228;
lbits zghz3229;
lbits zghz3230;
sail_int zghz3231;
lbits zghz3232;

static void startup_sailgen_setCapPerms(void)
{

  CREATE(lbits)(&zghz3227);
  CREATE(sail_int)(&zghz3228);
  CREATE(lbits)(&zghz3229);
  CREATE(lbits)(&zghz3230);
  CREATE(sail_int)(&zghz3231);
  CREATE(lbits)(&zghz3232);
}

static struct zCapability sailgen_setCapPerms(struct zCapability zcap, uint64_t zperms)
{
  __label__ end_function_128, end_block_exception_129;

  struct zCapability zghz3226;
  uint64_t zgaz3248;
  {
    uint64_t zgaz3247;
    {
      RECREATE(lbits)(&zghz3230);
      CONVERT_OF(lbits, fbits)(&zghz3230, zperms, UINT64_C(15) , true);
      RECREATE(sail_int)(&zghz3231);
      CONVERT_OF(sail_int, mach_int)(&zghz3231, zcap_uperms_shift);
      RECREATE(lbits)(&zghz3232);
      shiftr(&zghz3232, zghz3230, zghz3231);
      zgaz3247 = CONVERT_OF(fbits, lbits)(zghz3232, true);
    }
    {
      RECREATE(lbits)(&zghz3227);
      CONVERT_OF(lbits, fbits)(&zghz3227, zgaz3247, UINT64_C(15) , true);
      RECREATE(sail_int)(&zghz3228);
      CONVERT_OF(sail_int, mach_int)(&zghz3228, zcap_uperms_width);
      RECREATE(lbits)(&zghz3229);
      sail_truncate(&zghz3229, zghz3227, zghz3228);
      zgaz3248 = CONVERT_OF(fbits, lbits)(zghz3229, true);
    }
  }
  bool zgaz3250;
  {
    fbits zgaz3249;
    zgaz3249 = (UINT64_C(1) & (zperms >> INT64_C(11)));
    zgaz3250 = sailgen_bit_to_bool(zgaz3249);
  }
  bool zgaz3252;
  {
    fbits zgaz3251;
    zgaz3251 = (UINT64_C(1) & (zperms >> INT64_C(10)));
    zgaz3252 = sailgen_bit_to_bool(zgaz3251);
  }
  bool zgaz3254;
  {
    fbits zgaz3253;
    zgaz3253 = (UINT64_C(1) & (zperms >> INT64_C(9)));
    zgaz3254 = sailgen_bit_to_bool(zgaz3253);
  }
  bool zgaz3256;
  {
    fbits zgaz3255;
    zgaz3255 = (UINT64_C(1) & (zperms >> INT64_C(8)));
    zgaz3256 = sailgen_bit_to_bool(zgaz3255);
  }
  bool zgaz3258;
  {
    fbits zgaz3257;
    zgaz3257 = (UINT64_C(1) & (zperms >> INT64_C(7)));
    zgaz3258 = sailgen_bit_to_bool(zgaz3257);
  }
  bool zgaz3260;
  {
    fbits zgaz3259;
    zgaz3259 = (UINT64_C(1) & (zperms >> INT64_C(6)));
    zgaz3260 = sailgen_bit_to_bool(zgaz3259);
  }
  bool zgaz3262;
  {
    fbits zgaz3261;
    zgaz3261 = (UINT64_C(1) & (zperms >> INT64_C(5)));
    zgaz3262 = sailgen_bit_to_bool(zgaz3261);
  }
  bool zgaz3264;
  {
    fbits zgaz3263;
    zgaz3263 = (UINT64_C(1) & (zperms >> INT64_C(4)));
    zgaz3264 = sailgen_bit_to_bool(zgaz3263);
  }
  bool zgaz3266;
  {
    fbits zgaz3265;
    zgaz3265 = (UINT64_C(1) & (zperms >> INT64_C(3)));
    zgaz3266 = sailgen_bit_to_bool(zgaz3265);
  }
  bool zgaz3268;
  {
    fbits zgaz3267;
    zgaz3267 = (UINT64_C(1) & (zperms >> INT64_C(2)));
    zgaz3268 = sailgen_bit_to_bool(zgaz3267);
  }
  bool zgaz3270;
  {
    fbits zgaz3269;
    zgaz3269 = (UINT64_C(1) & (zperms >> INT64_C(1)));
    zgaz3270 = sailgen_bit_to_bool(zgaz3269);
  }
  bool zgaz3272;
  {
    fbits zgaz3271;
    zgaz3271 = (UINT64_C(1) & (zperms >> INT64_C(0)));
    zgaz3272 = sailgen_bit_to_bool(zgaz3271);
  }
  zghz3226 = zcap;
  zghz3226.zaccess_system_regs = zgaz3252;
  zghz3226.zglobal = zgaz3272;
  zghz3226.zpermit_cinvoke = zgaz3256;
  zghz3226.zpermit_execute = zgaz3270;
  zghz3226.zpermit_load = zgaz3268;
  zghz3226.zpermit_load_cap = zgaz3264;
  zghz3226.zpermit_seal = zgaz3258;
  zghz3226.zpermit_set_CID = zgaz3250;
  zghz3226.zpermit_store = zgaz3266;
  zghz3226.zpermit_store_cap = zgaz3262;
  zghz3226.zpermit_store_local_cap = zgaz3260;
  zghz3226.zpermit_unseal = zgaz3254;
  zghz3226.zuperms = zgaz3248;













end_function_128: ;
  return zghz3226;
end_block_exception_129: ;
  struct zCapability zcbz358 = { .zB = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zaddress = UINT64_C(0xdeadc0de), .zflag_cap_mode = false, .zglobal = false, .zinternal_E = false, .zotype = UINT64_C(0xdeadc0de), .zpermit_cinvoke = false, .zpermit_execute = false, .zpermit_load = false, .zpermit_load_cap = false, .zpermit_seal = false, .zpermit_set_CID = false, .zpermit_store = false, .zpermit_store_cap = false, .zpermit_store_local_cap = false, .zpermit_unseal = false, .zreserved = UINT64_C(0xdeadc0de), .ztag = false, .zuperms = UINT64_C(0xdeadc0de) };
  return zcbz358;
}



static void finish_sailgen_setCapPerms(void)
{
  KILL(lbits)(&zghz3232);
  KILL(sail_int)(&zghz3231);
  KILL(lbits)(&zghz3230);
  KILL(lbits)(&zghz3229);
  KILL(sail_int)(&zghz3228);
  KILL(lbits)(&zghz3227);

}

static uint64_t sailgen_getCapFlags(struct zCapability);

static uint64_t sailgen_getCapFlags(struct zCapability zcap)
{
  __label__ end_function_131, end_block_exception_132;

  uint64_t zcbz331;
  bool zgaz3273;
  zgaz3273 = zcap.zflag_cap_mode;
  zcbz331 = sailgen_bool_to_bits(zgaz3273);

end_function_131: ;
  return zcbz331;
end_block_exception_132: ;

  return UINT64_C(0xdeadc0de);
}

static bool sailgen_isCapSealed(struct zCapability);

sail_int zghz3233;
sail_int zghz3234;
lbits zghz3235;
sail_int zghz3236;

static void startup_sailgen_isCapSealed(void)
{
  CREATE(sail_int)(&zghz3233);
  CREATE(sail_int)(&zghz3234);
  CREATE(lbits)(&zghz3235);
  CREATE(sail_int)(&zghz3236);
}

static bool sailgen_isCapSealed(struct zCapability zcap)
{
  __label__ end_function_134, end_block_exception_135;

  bool zcbz332;
  int64_t zgaz3275;
  {
    uint64_t zgaz3274;
    zgaz3274 = zcap.zotype;
    {
      RECREATE(lbits)(&zghz3235);
      CONVERT_OF(lbits, fbits)(&zghz3235, zgaz3274, UINT64_C(4) , true);
      RECREATE(sail_int)(&zghz3236);
      sail_signed(&zghz3236, zghz3235);
      zgaz3275 = CONVERT_OF(mach_int, sail_int)(zghz3236);
    }
  }
  {
    RECREATE(sail_int)(&zghz3233);
    CONVERT_OF(sail_int, mach_int)(&zghz3233, zgaz3275);
    RECREATE(sail_int)(&zghz3234);
    CONVERT_OF(sail_int, mach_int)(&zghz3234, zotype_unsealed);
    zcbz332 = sailgen_neq_int(zghz3233, zghz3234);
  }

end_function_134: ;
  return zcbz332;
end_block_exception_135: ;

  return false;
}



static void finish_sailgen_isCapSealed(void)
{
  KILL(sail_int)(&zghz3236);
  KILL(lbits)(&zghz3235);
  KILL(sail_int)(&zghz3234);
  KILL(sail_int)(&zghz3233);
}

static bool sailgen_hasReservedOType(struct zCapability);

lbits zghz3237;
sail_int zghz3238;

static void startup_sailgen_hasReservedOType(void)
{
  CREATE(lbits)(&zghz3237);
  CREATE(sail_int)(&zghz3238);
}

static bool sailgen_hasReservedOType(struct zCapability zcap)
{
  __label__ end_function_137, end_block_exception_138;

  bool zcbz333;
  int64_t zgaz3277;
  {
    uint64_t zgaz3276;
    zgaz3276 = zcap.zotype;
    {
      RECREATE(lbits)(&zghz3237);
      CONVERT_OF(lbits, fbits)(&zghz3237, zgaz3276, UINT64_C(4) , true);
      RECREATE(sail_int)(&zghz3238);
      sail_unsigned(&zghz3238, zghz3237);
      zgaz3277 = CONVERT_OF(mach_int, sail_int)(zghz3238);
    }
  }
  zcbz333 = (zgaz3277 > zcap_max_otype);

end_function_137: ;
  return zcbz333;
end_block_exception_138: ;

  return false;
}



static void finish_sailgen_hasReservedOType(void)
{
  KILL(sail_int)(&zghz3238);
  KILL(lbits)(&zghz3237);
}

static uint64_t sailgen_getCapBaseBits(struct zCapability);

static uint64_t sailgen_getCapBaseBits(struct zCapability zc)
{
  __label__ case_141, finish_match_140, end_function_142, end_block_exception_143;

  uint64_t zcbz334;
  struct ztuple_z8z5bv32zCz0z5bv33z9 zgaz3278;
  zgaz3278 = sailgen_getCapBoundsBits(zc);
  {
    uint64_t zbase;
    zbase = zgaz3278.ztup0;
    zcbz334 = zbase;
    goto finish_match_140;
  }
case_141: ;
  sail_match_failure("getCapBaseBits");
finish_match_140: ;

end_function_142: ;
  return zcbz334;
end_block_exception_143: ;

  return UINT64_C(0xdeadc0de);
}

static int64_t sailgen_getCapLength(struct zCapability);

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

static void startup_sailgen_getCapLength(void)
{
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
}

static int64_t sailgen_getCapLength(struct zCapability zc)
{
  __label__ case_146, finish_match_145, end_function_147, end_block_exception_148;

  int64_t zcbz335;
  struct ztuple_z8z5i64zCz0z5i64z9 zgaz3279;
  zgaz3279 = sailgen_getCapBounds(zc);
  {
    int64_t zbase;
    zbase = zgaz3279.ztup0;
    int64_t ztop;
    ztop = zgaz3279.ztup1;
    {
      bool zgaz3282;
      {
        bool zgaz3281;
        {
          bool zgaz3280;
          zgaz3280 = zc.ztag;
          zgaz3281 = sailgen_not(zgaz3280);
        }
        bool zgsz375;
        if (zgaz3281) {  zgsz375 = true;  } else {
          {
            RECREATE(sail_int)(&zghz3247);
            CONVERT_OF(sail_int, mach_int)(&zghz3247, ztop);
            RECREATE(sail_int)(&zghz3248);
            CONVERT_OF(sail_int, mach_int)(&zghz3248, zbase);
            zgsz375 = gteq(zghz3247, zghz3248);
          }
        }
        zgaz3282 = zgsz375;
      }
      unit zgsz376;
      zgsz376 = sail_assert(zgaz3282, "src/cheri_cap_common.sail:453.40-453.41");
    }
    int64_t zgaz3283;
    {
      RECREATE(sail_int)(&zghz3244);
      CONVERT_OF(sail_int, mach_int)(&zghz3244, ztop);
      RECREATE(sail_int)(&zghz3245);
      CONVERT_OF(sail_int, mach_int)(&zghz3245, zbase);
      RECREATE(sail_int)(&zghz3246);
      sub_int(&zghz3246, zghz3244, zghz3245);
      zgaz3283 = CONVERT_OF(mach_int, sail_int)(zghz3246);
    }
    int64_t zgaz3284;
    {
      RECREATE(sail_int)(&zghz3242);
      CONVERT_OF(sail_int, mach_int)(&zghz3242, zcap_len_width);
      RECREATE(sail_int)(&zghz3243);
      pow2(&zghz3243, zghz3242);
      zgaz3284 = CONVERT_OF(mach_int, sail_int)(zghz3243);
    }
    {
      RECREATE(sail_int)(&zghz3239);
      CONVERT_OF(sail_int, mach_int)(&zghz3239, zgaz3283);
      RECREATE(sail_int)(&zghz3240);
      CONVERT_OF(sail_int, mach_int)(&zghz3240, zgaz3284);
      RECREATE(sail_int)(&zghz3241);
      emod_int(&zghz3241, zghz3239, zghz3240);
      zcbz335 = CONVERT_OF(mach_int, sail_int)(zghz3241);
    }
    goto finish_match_145;
  }
case_146: ;
  sail_match_failure("getCapLength");
finish_match_145: ;

end_function_147: ;
  return zcbz335;
end_block_exception_148: ;

  return INT64_C(0xdeadc0de);
}



static void finish_sailgen_getCapLength(void)
{
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
}

static bool sailgen_inCapBounds(struct zCapability, uint64_t, int64_t);

sail_int zghz3249;
sail_int zghz3250;
sail_int zghz3251;
sail_int zghz3252;
lbits zghz3253;
sail_int zghz3254;

static void startup_sailgen_inCapBounds(void)
{
  CREATE(sail_int)(&zghz3249);
  CREATE(sail_int)(&zghz3250);
  CREATE(sail_int)(&zghz3251);
  CREATE(sail_int)(&zghz3252);
  CREATE(lbits)(&zghz3253);
  CREATE(sail_int)(&zghz3254);
}

static bool sailgen_inCapBounds(struct zCapability zcap, uint64_t zaddr, int64_t zsizze)
{
  __label__ case_151, finish_match_150, end_function_152, end_block_exception_153;

  bool zcbz336;
  struct ztuple_z8z5i64zCz0z5i64z9 zgaz3285;
  zgaz3285 = sailgen_getCapBounds(zcap);
  {
    int64_t zbase;
    zbase = zgaz3285.ztup0;
    int64_t ztop;
    ztop = zgaz3285.ztup1;
    int64_t za;
    {
      RECREATE(lbits)(&zghz3253);
      CONVERT_OF(lbits, fbits)(&zghz3253, zaddr, UINT64_C(32) , true);
      RECREATE(sail_int)(&zghz3254);
      sail_unsigned(&zghz3254, zghz3253);
      za = CONVERT_OF(mach_int, sail_int)(zghz3254);
    }
    bool zgaz3287;
    {
      RECREATE(sail_int)(&zghz3251);
      CONVERT_OF(sail_int, mach_int)(&zghz3251, za);
      RECREATE(sail_int)(&zghz3252);
      CONVERT_OF(sail_int, mach_int)(&zghz3252, zbase);
      zgaz3287 = gteq(zghz3251, zghz3252);
    }
    bool zgsz379;
    if (zgaz3287) {
      int64_t zgaz3286;
      zgaz3286 = (za + zsizze);
      {
        RECREATE(sail_int)(&zghz3249);
        CONVERT_OF(sail_int, mach_int)(&zghz3249, zgaz3286);
        RECREATE(sail_int)(&zghz3250);
        CONVERT_OF(sail_int, mach_int)(&zghz3250, ztop);
        zgsz379 = lteq(zghz3249, zghz3250);
      }
    } else {  zgsz379 = false;  }
    zcbz336 = zgsz379;
    goto finish_match_150;
  }
case_151: ;
  sail_match_failure("inCapBounds");
finish_match_150: ;

end_function_152: ;
  return zcbz336;
end_block_exception_153: ;

  return false;
}



static void finish_sailgen_inCapBounds(void)
{
  KILL(sail_int)(&zghz3254);
  KILL(lbits)(&zghz3253);
  KILL(sail_int)(&zghz3252);
  KILL(sail_int)(&zghz3251);
  KILL(sail_int)(&zghz3250);
  KILL(sail_int)(&zghz3249);
}

static struct zCapability sailgen_clearTagIf(struct zCapability, bool);

struct zCapability zghz3255;

static void startup_sailgen_clearTagIf(void)
{
}

static struct zCapability sailgen_clearTagIf(struct zCapability zcap, bool zcond)
{
  __label__ end_function_155, end_block_exception_156;

  struct zCapability zghz3255;
  bool zgaz3289;
  {
    bool zgaz3288;
    zgaz3288 = zcap.ztag;
    bool zgsz381;
    if (zgaz3288) {  zgsz381 = sailgen_not(zcond);  } else {  zgsz381 = false;  }
    zgaz3289 = zgsz381;
  }
  zghz3255 = zcap;
  zghz3255.ztag = zgaz3289;

end_function_155: ;
  return zghz3255;
end_block_exception_156: ;
  struct zCapability zcbz359 = { .zB = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zaddress = UINT64_C(0xdeadc0de), .zflag_cap_mode = false, .zglobal = false, .zinternal_E = false, .zotype = UINT64_C(0xdeadc0de), .zpermit_cinvoke = false, .zpermit_execute = false, .zpermit_load = false, .zpermit_load_cap = false, .zpermit_seal = false, .zpermit_set_CID = false, .zpermit_store = false, .zpermit_store_cap = false, .zpermit_store_local_cap = false, .zpermit_unseal = false, .zreserved = UINT64_C(0xdeadc0de), .ztag = false, .zuperms = UINT64_C(0xdeadc0de) };
  return zcbz359;
}



static void finish_sailgen_clearTagIf(void)
{
}

static struct zCapability sailgen_clearTagIfSealed(struct zCapability);

struct zCapability zghz3256;

static void startup_sailgen_clearTagIfSealed(void)
{
}

static struct zCapability sailgen_clearTagIfSealed(struct zCapability zcap)
{
  __label__ end_function_158, end_block_exception_159;

  struct zCapability zghz3256;
  bool zgaz3290;
  zgaz3290 = sailgen_isCapSealed(zcap);
  zghz3256 = sailgen_clearTagIf(zcap, zgaz3290);

end_function_158: ;
  return zghz3256;
end_block_exception_159: ;
  struct zCapability zcbz360 = { .zB = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zaddress = UINT64_C(0xdeadc0de), .zflag_cap_mode = false, .zglobal = false, .zinternal_E = false, .zotype = UINT64_C(0xdeadc0de), .zpermit_cinvoke = false, .zpermit_execute = false, .zpermit_load = false, .zpermit_load_cap = false, .zpermit_seal = false, .zpermit_set_CID = false, .zpermit_store = false, .zpermit_store_cap = false, .zpermit_store_local_cap = false, .zpermit_unseal = false, .zreserved = UINT64_C(0xdeadc0de), .ztag = false, .zuperms = UINT64_C(0xdeadc0de) };
  return zcbz360;
}



static void finish_sailgen_clearTagIfSealed(void)
{
}

static bool sailgen_capBoundsEqual(struct zCapability, struct zCapability);

sail_int zghz3257;
sail_int zghz3258;
sail_int zghz3259;
sail_int zghz3260;

static void startup_sailgen_capBoundsEqual(void)
{
  CREATE(sail_int)(&zghz3257);
  CREATE(sail_int)(&zghz3258);
  CREATE(sail_int)(&zghz3259);
  CREATE(sail_int)(&zghz3260);
}

static bool sailgen_capBoundsEqual(struct zCapability zc1, struct zCapability zc2)
{
  __label__ case_162, finish_match_161, end_function_165, end_block_exception_166;

  bool zcbz339;
  struct ztuple_z8z5i64zCz0z5i64z9 zgaz3291;
  zgaz3291 = sailgen_getCapBounds(zc1);
  {
    __label__ case_164, finish_match_163;

    int64_t zbase1;
    zbase1 = zgaz3291.ztup0;
    int64_t ztop1;
    ztop1 = zgaz3291.ztup1;
    struct ztuple_z8z5i64zCz0z5i64z9 zgaz3292;
    zgaz3292 = sailgen_getCapBounds(zc2);
    bool zgsz384;
    {
      int64_t zbase2;
      zbase2 = zgaz3292.ztup0;
      int64_t ztop2;
      ztop2 = zgaz3292.ztup1;
      bool zgaz3293;
      {
        RECREATE(sail_int)(&zghz3259);
        CONVERT_OF(sail_int, mach_int)(&zghz3259, zbase1);
        RECREATE(sail_int)(&zghz3260);
        CONVERT_OF(sail_int, mach_int)(&zghz3260, zbase2);
        zgaz3293 = eq_int(zghz3259, zghz3260);
      }
      bool zgsz385;
      if (zgaz3293) {
        {
          RECREATE(sail_int)(&zghz3257);
          CONVERT_OF(sail_int, mach_int)(&zghz3257, ztop1);
          RECREATE(sail_int)(&zghz3258);
          CONVERT_OF(sail_int, mach_int)(&zghz3258, ztop2);
          zgsz385 = eq_int(zghz3257, zghz3258);
        }
      } else {  zgsz385 = false;  }
      zgsz384 = zgsz385;
      goto finish_match_163;
    }
  case_164: ;
    sail_match_failure("capBoundsEqual");
  finish_match_163: ;
    zcbz339 = zgsz384;
    goto finish_match_161;
  }
case_162: ;
  sail_match_failure("capBoundsEqual");
finish_match_161: ;

end_function_165: ;
  return zcbz339;
end_block_exception_166: ;

  return false;
}



static void finish_sailgen_capBoundsEqual(void)
{
  KILL(sail_int)(&zghz3260);
  KILL(sail_int)(&zghz3259);
  KILL(sail_int)(&zghz3258);
  KILL(sail_int)(&zghz3257);
}

static struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 sailgen_setCapAddr(struct zCapability, uint64_t);

struct zCapability zghz3261;

static void startup_sailgen_setCapAddr(void)
{
}

static struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 sailgen_setCapAddr(struct zCapability zc, uint64_t zaddr)
{
  __label__ end_function_168, end_block_exception_169;

  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz340;
  struct zCapability zghz3261;
  {
    zghz3261 = zc;
    zghz3261.zaddress = zaddr;
  }
  bool zrepresentable;
  zrepresentable = sailgen_capBoundsEqual(zc, zghz3261);
  zcbz340.ztup0 = zrepresentable;
  zcbz340.ztup1 = zghz3261;

end_function_168: ;
  return zcbz340;
end_block_exception_169: ;
  struct zCapability zcbz362 = { .zB = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zaddress = UINT64_C(0xdeadc0de), .zflag_cap_mode = false, .zglobal = false, .zinternal_E = false, .zotype = UINT64_C(0xdeadc0de), .zpermit_cinvoke = false, .zpermit_execute = false, .zpermit_load = false, .zpermit_load_cap = false, .zpermit_seal = false, .zpermit_set_CID = false, .zpermit_store = false, .zpermit_store_cap = false, .zpermit_store_local_cap = false, .zpermit_unseal = false, .zreserved = UINT64_C(0xdeadc0de), .ztag = false, .zuperms = UINT64_C(0xdeadc0de) };
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz361 = { .ztup0 = false, .ztup1 = zcbz362 };
  return zcbz361;
}



static void finish_sailgen_setCapAddr(void)
{
}

static bool sailgen_fastRepCheck(struct zCapability, uint64_t);

lbits zghz3262;
lbits zghz3263;
lbits zghz3264;
lbits zghz3265;
lbits zghz3266;
sail_int zghz3267;
lbits zghz3268;
sail_int zghz3269;
lbits zghz3270;
sail_int zghz3271;
sail_int zghz3272;
sail_int zghz3273;
lbits zghz3274;
sail_int zghz3275;
lbits zghz3276;
lbits zghz3277;
sail_int zghz3278;
lbits zghz3279;
lbits zghz3280;
sail_int zghz3281;
lbits zghz3282;
lbits zghz3283;
sail_int zghz3284;
lbits zghz3285;
lbits zghz3286;
sail_int zghz3287;
lbits zghz3288;
lbits zghz3289;
sail_int zghz3290;
lbits zghz3291;
sail_int zghz3292;
lbits zghz3293;
sail_int zghz3294;
sail_int zghz3295;
sail_int zghz3296;
lbits zghz3297;
sail_int zghz3298;

static void startup_sailgen_fastRepCheck(void)
{
  CREATE(lbits)(&zghz3262);
  CREATE(lbits)(&zghz3263);
  CREATE(lbits)(&zghz3264);
  CREATE(lbits)(&zghz3265);
  CREATE(lbits)(&zghz3266);
  CREATE(sail_int)(&zghz3267);
  CREATE(lbits)(&zghz3268);
  CREATE(sail_int)(&zghz3269);
  CREATE(lbits)(&zghz3270);
  CREATE(sail_int)(&zghz3271);
  CREATE(sail_int)(&zghz3272);
  CREATE(sail_int)(&zghz3273);
  CREATE(lbits)(&zghz3274);
  CREATE(sail_int)(&zghz3275);
  CREATE(lbits)(&zghz3276);
  CREATE(lbits)(&zghz3277);
  CREATE(sail_int)(&zghz3278);
  CREATE(lbits)(&zghz3279);
  CREATE(lbits)(&zghz3280);
  CREATE(sail_int)(&zghz3281);
  CREATE(lbits)(&zghz3282);
  CREATE(lbits)(&zghz3283);
  CREATE(sail_int)(&zghz3284);
  CREATE(lbits)(&zghz3285);
  CREATE(lbits)(&zghz3286);
  CREATE(sail_int)(&zghz3287);
  CREATE(lbits)(&zghz3288);
  CREATE(lbits)(&zghz3289);
  CREATE(sail_int)(&zghz3290);
  CREATE(lbits)(&zghz3291);
  CREATE(sail_int)(&zghz3292);
  CREATE(lbits)(&zghz3293);
  CREATE(sail_int)(&zghz3294);
  CREATE(sail_int)(&zghz3295);
  CREATE(sail_int)(&zghz3296);
  CREATE(lbits)(&zghz3297);
  CREATE(sail_int)(&zghz3298);
}

static bool sailgen_fastRepCheck(struct zCapability zc, uint64_t zi)
{
  __label__ end_function_171, end_block_exception_172;

  bool zcbz341;
  int64_t zE;
  {
    uint64_t zgaz3307;
    zgaz3307 = zc.zE;
    {
      RECREATE(lbits)(&zghz3297);
      CONVERT_OF(lbits, fbits)(&zghz3297, zgaz3307, UINT64_C(6) , true);
      RECREATE(sail_int)(&zghz3298);
      sail_unsigned(&zghz3298, zghz3297);
      zE = CONVERT_OF(mach_int, sail_int)(zghz3298);
    }
  }
  bool zgaz3295;
  {
    int64_t zgaz3294;
    {
      RECREATE(sail_int)(&zghz3294);
      CONVERT_OF(sail_int, mach_int)(&zghz3294, zcap_max_E);
      RECREATE(sail_int)(&zghz3295);
      CONVERT_OF(sail_int, mach_int)(&zghz3295, INT64_C(2));
      RECREATE(sail_int)(&zghz3296);
      sub_int(&zghz3296, zghz3294, zghz3295);
      zgaz3294 = CONVERT_OF(mach_int, sail_int)(zghz3296);
    }
    zgaz3295 = (zE >= zgaz3294);
  }
  if (zgaz3295) {  zcbz341 = true;  } else {
    int64_t zi_top;
    {
      uint64_t zgaz3306;
      {
        int64_t zgaz3305;
        zgaz3305 = (zE + zcap_mantissa_width);
        {
          RECREATE(lbits)(&zghz3291);
          CONVERT_OF(lbits, fbits)(&zghz3291, zi, UINT64_C(32) , true);
          RECREATE(sail_int)(&zghz3292);
          CONVERT_OF(sail_int, mach_int)(&zghz3292, zgaz3305);
          RECREATE(lbits)(&zghz3293);
          arith_shiftr(&zghz3293, zghz3291, zghz3292);
          zgaz3306 = CONVERT_OF(fbits, lbits)(zghz3293, true);
        }
      }
      {
        RECREATE(lbits)(&zghz3289);
        CONVERT_OF(lbits, fbits)(&zghz3289, zgaz3306, UINT64_C(32) , true);
        RECREATE(sail_int)(&zghz3290);
        sail_signed(&zghz3290, zghz3289);
        zi_top = CONVERT_OF(mach_int, sail_int)(zghz3290);
      }
    }
    uint64_t zi_mid;
    {
      uint64_t zgaz3304;
      {
        RECREATE(lbits)(&zghz3286);
        CONVERT_OF(lbits, fbits)(&zghz3286, zi, UINT64_C(32) , true);
        RECREATE(sail_int)(&zghz3287);
        CONVERT_OF(sail_int, mach_int)(&zghz3287, zE);
        RECREATE(lbits)(&zghz3288);
        shiftr(&zghz3288, zghz3286, zghz3287);
        zgaz3304 = CONVERT_OF(fbits, lbits)(zghz3288, true);
      }
      {
        RECREATE(lbits)(&zghz3283);
        CONVERT_OF(lbits, fbits)(&zghz3283, zgaz3304, UINT64_C(32) , true);
        RECREATE(sail_int)(&zghz3284);
        CONVERT_OF(sail_int, mach_int)(&zghz3284, zcap_mantissa_width);
        RECREATE(lbits)(&zghz3285);
        sail_truncate(&zghz3285, zghz3283, zghz3284);
        zi_mid = CONVERT_OF(fbits, lbits)(zghz3285, true);
      }
    }
    uint64_t za_mid;
    {
      uint64_t zgaz3303;
      {
        uint64_t zgaz3302;
        zgaz3302 = zc.zaddress;
        {
          RECREATE(lbits)(&zghz3280);
          CONVERT_OF(lbits, fbits)(&zghz3280, zgaz3302, UINT64_C(32) , true);
          RECREATE(sail_int)(&zghz3281);
          CONVERT_OF(sail_int, mach_int)(&zghz3281, zE);
          RECREATE(lbits)(&zghz3282);
          shiftr(&zghz3282, zghz3280, zghz3281);
          zgaz3303 = CONVERT_OF(fbits, lbits)(zghz3282, true);
        }
      }
      {
        RECREATE(lbits)(&zghz3277);
        CONVERT_OF(lbits, fbits)(&zghz3277, zgaz3303, UINT64_C(32) , true);
        RECREATE(sail_int)(&zghz3278);
        CONVERT_OF(sail_int, mach_int)(&zghz3278, zcap_mantissa_width);
        RECREATE(lbits)(&zghz3279);
        sail_truncate(&zghz3279, zghz3277, zghz3278);
        za_mid = CONVERT_OF(fbits, lbits)(zghz3279, true);
      }
    }
    uint64_t zB3;
    {
      uint64_t zgaz3301;
      zgaz3301 = zc.zB;
      {
        RECREATE(lbits)(&zghz3274);
        CONVERT_OF(lbits, fbits)(&zghz3274, zgaz3301, UINT64_C(8) , true);
        RECREATE(sail_int)(&zghz3275);
        CONVERT_OF(sail_int, mach_int)(&zghz3275, INT64_C(3));
        RECREATE(lbits)(&zghz3276);
        sail_truncateLSB(&zghz3276, zghz3274, zghz3275);
        zB3 = CONVERT_OF(fbits, lbits)(zghz3276, true);
      }
    }
    uint64_t zR3;
    zR3 = ((zB3 - UINT64_C(0b001)) & UINT64_C(0b111));
    uint64_t zR;
    {
      uint64_t zgaz3300;
      {
        int64_t zgaz3299;
        {
          RECREATE(sail_int)(&zghz3271);
          CONVERT_OF(sail_int, mach_int)(&zghz3271, zcap_mantissa_width);
          RECREATE(sail_int)(&zghz3272);
          CONVERT_OF(sail_int, mach_int)(&zghz3272, INT64_C(3));
          RECREATE(sail_int)(&zghz3273);
          sub_int(&zghz3273, zghz3271, zghz3272);
          zgaz3299 = CONVERT_OF(mach_int, sail_int)(zghz3273);
        }
        {
          RECREATE(sail_int)(&zghz3269);
          CONVERT_OF(sail_int, mach_int)(&zghz3269, zgaz3299);
          RECREATE(lbits)(&zghz3270);
          sailgen_zzeros_implicit(&zghz3270, zghz3269);
          zgaz3300 = CONVERT_OF(fbits, lbits)(zghz3270, true);
        }
      }
      zR = (zR3 << 5) | zgaz3300;
    }
    uint64_t zdiff;
    zdiff = ((zR - za_mid) & UINT64_C(0xFF));
    uint64_t zdiff1;
    {
      RECREATE(lbits)(&zghz3266);
      CONVERT_OF(lbits, fbits)(&zghz3266, zdiff, UINT64_C(8) , true);
      RECREATE(sail_int)(&zghz3267);
      CONVERT_OF(sail_int, mach_int)(&zghz3267, INT64_C(1));
      RECREATE(lbits)(&zghz3268);
      sub_bits_int(&zghz3268, zghz3266, zghz3267);
      zdiff1 = CONVERT_OF(fbits, lbits)(zghz3268, true);
    }
    bool zgaz3296;
    zgaz3296 = (zi_top == INT64_C(0));
    if (zgaz3296) {
      {
        RECREATE(lbits)(&zghz3264);
        CONVERT_OF(lbits, fbits)(&zghz3264, zi_mid, UINT64_C(8) , true);
        RECREATE(lbits)(&zghz3265);
        CONVERT_OF(lbits, fbits)(&zghz3265, zdiff1, UINT64_C(8) , true);
        zcbz341 = sailgen_z8operatorz0zI_uz9(zghz3264, zghz3265);
      }
    } else {
      bool zgaz3297;
      zgaz3297 = (zi_top == INT64_C(-1));
      if (zgaz3297) {
        bool zgaz3298;
        {
          RECREATE(lbits)(&zghz3262);
          CONVERT_OF(lbits, fbits)(&zghz3262, zi_mid, UINT64_C(8) , true);
          RECREATE(lbits)(&zghz3263);
          CONVERT_OF(lbits, fbits)(&zghz3263, zdiff, UINT64_C(8) , true);
          zgaz3298 = sailgen_z8operatorz0zKzJ_uz9(zghz3262, zghz3263);
        }
        bool zgsz390;
        if (zgaz3298) {  zgsz390 = (zR != za_mid);  } else {  zgsz390 = false;  }
        zcbz341 = zgsz390;
      } else {  zcbz341 = false;  }
    }
  }


end_function_171: ;
  return zcbz341;
end_block_exception_172: ;

  return false;
}



static void finish_sailgen_fastRepCheck(void)
{
  KILL(sail_int)(&zghz3298);
  KILL(lbits)(&zghz3297);
  KILL(sail_int)(&zghz3296);
  KILL(sail_int)(&zghz3295);
  KILL(sail_int)(&zghz3294);
  KILL(lbits)(&zghz3293);
  KILL(sail_int)(&zghz3292);
  KILL(lbits)(&zghz3291);
  KILL(sail_int)(&zghz3290);
  KILL(lbits)(&zghz3289);
  KILL(lbits)(&zghz3288);
  KILL(sail_int)(&zghz3287);
  KILL(lbits)(&zghz3286);
  KILL(lbits)(&zghz3285);
  KILL(sail_int)(&zghz3284);
  KILL(lbits)(&zghz3283);
  KILL(lbits)(&zghz3282);
  KILL(sail_int)(&zghz3281);
  KILL(lbits)(&zghz3280);
  KILL(lbits)(&zghz3279);
  KILL(sail_int)(&zghz3278);
  KILL(lbits)(&zghz3277);
  KILL(lbits)(&zghz3276);
  KILL(sail_int)(&zghz3275);
  KILL(lbits)(&zghz3274);
  KILL(sail_int)(&zghz3273);
  KILL(sail_int)(&zghz3272);
  KILL(sail_int)(&zghz3271);
  KILL(lbits)(&zghz3270);
  KILL(sail_int)(&zghz3269);
  KILL(lbits)(&zghz3268);
  KILL(sail_int)(&zghz3267);
  KILL(lbits)(&zghz3266);
  KILL(lbits)(&zghz3265);
  KILL(lbits)(&zghz3264);
  KILL(lbits)(&zghz3263);
  KILL(lbits)(&zghz3262);
}

static void sailgen_capToString(sail_string *rop, struct zCapability);

lbits zghz3299;
lbits zghz3300;
lbits zghz3301;
lbits zghz3302;
sail_int zghz3303;
lbits zghz3304;
lbits zghz3305;
sail_int zghz3306;
lbits zghz3307;
lbits zghz3308;
lbits zghz3309;
sail_int zghz3310;
sail_int zghz3311;
lbits zghz3312;

static void startup_sailgen_capToString(void)
{
  CREATE(lbits)(&zghz3299);
  CREATE(lbits)(&zghz3300);
  CREATE(lbits)(&zghz3301);
  CREATE(lbits)(&zghz3302);
  CREATE(sail_int)(&zghz3303);
  CREATE(lbits)(&zghz3304);
  CREATE(lbits)(&zghz3305);
  CREATE(sail_int)(&zghz3306);
  CREATE(lbits)(&zghz3307);
  CREATE(lbits)(&zghz3308);
  CREATE(lbits)(&zghz3309);
  CREATE(sail_int)(&zghz3310);
  CREATE(sail_int)(&zghz3311);
  CREATE(lbits)(&zghz3312);
}

static void sailgen_capToString(sail_string *zcbz342, struct zCapability zcap)
{
  __label__ end_function_174, end_block_exception_175, end_function_201;

  int64_t zlen;
  zlen = sailgen_getCapLength(zcap);
  sail_string zlen_str;
  CREATE(sail_string)(&zlen_str);
  {
    uint64_t zgaz3336;
    {
      int64_t zgaz3335;
      zgaz3335 = (zcap_len_width + INT64_C(3));
      {
        RECREATE(sail_int)(&zghz3310);
        CONVERT_OF(sail_int, mach_int)(&zghz3310, zgaz3335);
        RECREATE(sail_int)(&zghz3311);
        CONVERT_OF(sail_int, mach_int)(&zghz3311, zlen);
        RECREATE(lbits)(&zghz3312);
        sailgen_to_bits(&zghz3312, zghz3310, zghz3311);
        zgaz3336 = CONVERT_OF(fbits, lbits)(zghz3312, true);
      }
    }
    {
      RECREATE(lbits)(&zghz3309);
      CONVERT_OF(lbits, fbits)(&zghz3309, zgaz3336, UINT64_C(36) , true);
      string_of_lbits(&zlen_str, zghz3309);
    }
  }
  uint64_t zotype64;
  {
    bool zgaz3332;
    zgaz3332 = sailgen_hasReservedOType(zcap);
    if (zgaz3332) {
      uint64_t zgaz3333;
      zgaz3333 = zcap.zotype;
      {
        RECREATE(sail_int)(&zghz3306);
        CONVERT_OF(sail_int, mach_int)(&zghz3306, INT64_C(32));
        RECREATE(lbits)(&zghz3307);
        CONVERT_OF(lbits, fbits)(&zghz3307, zgaz3333, UINT64_C(4) , true);
        RECREATE(lbits)(&zghz3308);
        sailgen_sign_extend(&zghz3308, zghz3306, zghz3307);
        zotype64 = CONVERT_OF(fbits, lbits)(zghz3308, true);
      }
    } else {
      uint64_t zgaz3334;
      zgaz3334 = zcap.zotype;
      {
        RECREATE(sail_int)(&zghz3303);
        CONVERT_OF(sail_int, mach_int)(&zghz3303, INT64_C(32));
        RECREATE(lbits)(&zghz3304);
        CONVERT_OF(lbits, fbits)(&zghz3304, zgaz3334, UINT64_C(4) , true);
        RECREATE(lbits)(&zghz3305);
        sailgen_zzero_extend(&zghz3305, zghz3303, zghz3304);
        zotype64 = CONVERT_OF(fbits, lbits)(zghz3305, true);
      }
    }
  }
  sail_string zgaz3331;
  CREATE(sail_string)(&zgaz3331);
  {
    sail_string zgaz3329;
    CREATE(sail_string)(&zgaz3329);
    {
      bool zgaz3308;
      zgaz3308 = zcap.ztag;
      if (zgaz3308) {  COPY(sail_string)(&zgaz3329, "1");  } else {  COPY(sail_string)(&zgaz3329, "0");  }
    }
    sail_string zgaz3330;
    CREATE(sail_string)(&zgaz3330);
    {
      sail_string zgaz3328;
      CREATE(sail_string)(&zgaz3328);
      {
        sail_string zgaz3326;
        CREATE(sail_string)(&zgaz3326);
        {
          bool zgaz3309;
          zgaz3309 = sailgen_isCapSealed(zcap);
          if (zgaz3309) {  COPY(sail_string)(&zgaz3326, "1");  } else {  COPY(sail_string)(&zgaz3326, "0");  }
        }
        sail_string zgaz3327;
        CREATE(sail_string)(&zgaz3327);
        {
          sail_string zgaz3325;
          CREATE(sail_string)(&zgaz3325);
          {
            sail_string zgaz3323;
            CREATE(sail_string)(&zgaz3323);
            {
              uint64_t zgaz3311;
              {
                uint64_t zgaz3310;
                zgaz3310 = sailgen_getCapPerms(zcap);
                zgaz3311 = (UINT64_C(0b0) << 15) | zgaz3310;
              }
              {
                RECREATE(lbits)(&zghz3302);
                CONVERT_OF(lbits, fbits)(&zghz3302, zgaz3311, UINT64_C(16) , true);
                string_of_lbits(&zgaz3323, zghz3302);
              }
            }
            sail_string zgaz3324;
            CREATE(sail_string)(&zgaz3324);
            {
              sail_string zgaz3322;
              CREATE(sail_string)(&zgaz3322);
              {
                sail_string zgaz3320;
                CREATE(sail_string)(&zgaz3320);
                {
                  RECREATE(lbits)(&zghz3301);
                  CONVERT_OF(lbits, fbits)(&zghz3301, zotype64, UINT64_C(32) , true);
                  string_of_lbits(&zgaz3320, zghz3301);
                }
                sail_string zgaz3321;
                CREATE(sail_string)(&zgaz3321);
                {
                  sail_string zgaz3319;
                  CREATE(sail_string)(&zgaz3319);
                  {
                    sail_string zgaz3317;
                    CREATE(sail_string)(&zgaz3317);
                    {
                      uint64_t zgaz3312;
                      zgaz3312 = zcap.zaddress;
                      {
                        RECREATE(lbits)(&zghz3300);
                        CONVERT_OF(lbits, fbits)(&zghz3300, zgaz3312, UINT64_C(32) , true);
                        string_of_lbits(&zgaz3317, zghz3300);
                      }
                    }
                    sail_string zgaz3318;
                    CREATE(sail_string)(&zgaz3318);
                    {
                      sail_string zgaz3316;
                      CREATE(sail_string)(&zgaz3316);
                      {
                        sail_string zgaz3314;
                        CREATE(sail_string)(&zgaz3314);
                        {
                          uint64_t zgaz3313;
                          zgaz3313 = sailgen_getCapBaseBits(zcap);
                          {
                            RECREATE(lbits)(&zghz3299);
                            CONVERT_OF(lbits, fbits)(&zghz3299, zgaz3313, UINT64_C(32) , true);
                            string_of_lbits(&zgaz3314, zghz3299);
                          }
                        }
                        sail_string zgaz3315;
                        CREATE(sail_string)(&zgaz3315);
                        concat_str(&zgaz3315, " length:", zlen_str);
                        concat_str(&zgaz3316, zgaz3314, zgaz3315);
                        KILL(sail_string)(&zgaz3315);
                        KILL(sail_string)(&zgaz3314);
                      }
                      concat_str(&zgaz3318, " base:", zgaz3316);
                      KILL(sail_string)(&zgaz3316);
                    }
                    concat_str(&zgaz3319, zgaz3317, zgaz3318);
                    KILL(sail_string)(&zgaz3318);
                    KILL(sail_string)(&zgaz3317);
                  }
                  concat_str(&zgaz3321, " address:", zgaz3319);
                  KILL(sail_string)(&zgaz3319);
                }
                concat_str(&zgaz3322, zgaz3320, zgaz3321);
                KILL(sail_string)(&zgaz3321);
                KILL(sail_string)(&zgaz3320);
              }
              concat_str(&zgaz3324, " type:", zgaz3322);
              KILL(sail_string)(&zgaz3322);
            }
            concat_str(&zgaz3325, zgaz3323, zgaz3324);
            KILL(sail_string)(&zgaz3324);
            KILL(sail_string)(&zgaz3323);
          }
          concat_str(&zgaz3327, " perms:", zgaz3325);
          KILL(sail_string)(&zgaz3325);
        }
        concat_str(&zgaz3328, zgaz3326, zgaz3327);
        KILL(sail_string)(&zgaz3327);
        KILL(sail_string)(&zgaz3326);
      }
      concat_str(&zgaz3330, " s:", zgaz3328);
      KILL(sail_string)(&zgaz3328);
    }
    concat_str(&zgaz3331, zgaz3329, zgaz3330);
    KILL(sail_string)(&zgaz3330);
    KILL(sail_string)(&zgaz3329);
  }
  concat_str((*(&zcbz342)), " t:", zgaz3331);
  KILL(sail_string)(&zgaz3331);

  KILL(sail_string)(&zlen_str);

end_function_174: ;
  goto end_function_201;
end_block_exception_175: ;
  goto end_function_201;
end_function_201: ;
}



static void finish_sailgen_capToString(void)
{
  KILL(lbits)(&zghz3312);
  KILL(sail_int)(&zghz3311);
  KILL(sail_int)(&zghz3310);
  KILL(lbits)(&zghz3309);
  KILL(lbits)(&zghz3308);
  KILL(lbits)(&zghz3307);
  KILL(sail_int)(&zghz3306);
  KILL(lbits)(&zghz3305);
  KILL(lbits)(&zghz3304);
  KILL(sail_int)(&zghz3303);
  KILL(lbits)(&zghz3302);
  KILL(lbits)(&zghz3301);
  KILL(lbits)(&zghz3300);
  KILL(lbits)(&zghz3299);
}

static uint64_t sailgen_getRepresentableAlignmentMask(uint64_t);

struct zCapability zghz3313;
sail_int zghz3314;
lbits zghz3315;
sail_int zghz3316;
lbits zghz3317;
sail_int zghz3318;
sail_int zghz3319;
sail_int zghz3320;
sail_int zghz3321;
sail_int zghz3322;
sail_int zghz3323;
lbits zghz3324;
sail_int zghz3325;

static void startup_sailgen_getRepresentableAlignmentMask(void)
{

  CREATE(sail_int)(&zghz3314);
  CREATE(lbits)(&zghz3315);
  CREATE(sail_int)(&zghz3316);
  CREATE(lbits)(&zghz3317);
  CREATE(sail_int)(&zghz3318);
  CREATE(sail_int)(&zghz3319);
  CREATE(sail_int)(&zghz3320);
  CREATE(sail_int)(&zghz3321);
  CREATE(sail_int)(&zghz3322);
  CREATE(sail_int)(&zghz3323);
  CREATE(lbits)(&zghz3324);
  CREATE(sail_int)(&zghz3325);
}

static uint64_t sailgen_getRepresentableAlignmentMask(uint64_t zlen)
{
  __label__ case_178, finish_match_177, end_function_179, end_block_exception_180;

  uint64_t zcbz343;
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3338;
  {
    uint64_t zgaz3337;
    zgaz3337 = (UINT64_C(0b0) << 32) | zlen;
    zgaz3338 = sailgen_setCapBounds(zdefault_cap, UINT64_C(0x00000000), zgaz3337);
  }
  {
    bool zexact;
    zexact = zgaz3338.ztup0;
    struct zCapability zghz3313;
    zghz3313 = zgaz3338.ztup1;
    int64_t ze;
    {
      int64_t zgaz3344;
      {
        uint64_t zgaz3343;
        zgaz3343 = zghz3313.zE;
        {
          RECREATE(lbits)(&zghz3324);
          CONVERT_OF(lbits, fbits)(&zghz3324, zgaz3343, UINT64_C(6) , true);
          RECREATE(sail_int)(&zghz3325);
          sail_unsigned(&zghz3325, zghz3324);
          zgaz3344 = CONVERT_OF(mach_int, sail_int)(zghz3325);
        }
      }
      {
        RECREATE(sail_int)(&zghz3321);
        CONVERT_OF(sail_int, mach_int)(&zghz3321, zgaz3344);
        RECREATE(sail_int)(&zghz3322);
        CONVERT_OF(sail_int, mach_int)(&zghz3322, zcap_max_E);
        RECREATE(sail_int)(&zghz3323);
        min_int(&zghz3323, zghz3321, zghz3322);
        ze = CONVERT_OF(mach_int, sail_int)(zghz3323);
      }
    }
    int64_t zez7;
    {
      bool zgaz3342;
      zgaz3342 = zghz3313.zinternal_E;
      if (zgaz3342) {  zez7 = (ze + zinternal_E_take_bits);  } else {  zez7 = INT64_C(0);  }
    }
    sbits zgaz3340;
    {
      int64_t zgaz3339;
      {
        RECREATE(sail_int)(&zghz3318);
        CONVERT_OF(sail_int, mach_int)(&zghz3318, INT64_C(32));
        RECREATE(sail_int)(&zghz3319);
        CONVERT_OF(sail_int, mach_int)(&zghz3319, zez7);
        RECREATE(sail_int)(&zghz3320);
        sub_int(&zghz3320, zghz3318, zghz3319);
        zgaz3339 = CONVERT_OF(mach_int, sail_int)(zghz3320);
      }
      {
        RECREATE(sail_int)(&zghz3316);
        CONVERT_OF(sail_int, mach_int)(&zghz3316, zgaz3339);
        RECREATE(lbits)(&zghz3317);
        sailgen_ones(&zghz3317, zghz3316);
        zgaz3340 = CONVERT_OF(sbits, lbits)(zghz3317, true);
      }
    }
    sbits zgaz3341;
    {
      RECREATE(sail_int)(&zghz3314);
      CONVERT_OF(sail_int, mach_int)(&zghz3314, zez7);
      RECREATE(lbits)(&zghz3315);
      sailgen_zzeros_implicit(&zghz3315, zghz3314);
      zgaz3341 = CONVERT_OF(sbits, lbits)(zghz3315, true);
    }
    uint64_t zgsz392;
    zgsz392 = CONVERT_OF(fbits, sbits)(append_ss(zgaz3340, zgaz3341), true);
    zcbz343 = zgsz392;
    goto finish_match_177;
  }
case_178: ;
  sail_match_failure("getRepresentableAlignmentMask");
finish_match_177: ;

end_function_179: ;
  return zcbz343;
end_block_exception_180: ;

  return UINT64_C(0xdeadc0de);
}



static void finish_sailgen_getRepresentableAlignmentMask(void)
{
  KILL(sail_int)(&zghz3325);
  KILL(lbits)(&zghz3324);
  KILL(sail_int)(&zghz3323);
  KILL(sail_int)(&zghz3322);
  KILL(sail_int)(&zghz3321);
  KILL(sail_int)(&zghz3320);
  KILL(sail_int)(&zghz3319);
  KILL(sail_int)(&zghz3318);
  KILL(lbits)(&zghz3317);
  KILL(sail_int)(&zghz3316);
  KILL(lbits)(&zghz3315);
  KILL(sail_int)(&zghz3314);

}

static uint64_t sailgen_getRepresentableLength(uint64_t);

static uint64_t sailgen_getRepresentableLength(uint64_t zlen)
{
  __label__ end_function_182, end_block_exception_183;

  uint64_t zcbz344;
  uint64_t zm;
  zm = sailgen_getRepresentableAlignmentMask(zlen);
  uint64_t zgaz3346;
  {
    uint64_t zgaz3345;
    zgaz3345 = (~(zm) & UINT64_C(0xFFFFFFFF));
    zgaz3346 = ((zlen + zgaz3345) & UINT64_C(0xFFFFFFFF));
  }
  zcbz344 = (zgaz3346 & zm);


end_function_182: ;
  return zcbz344;
end_block_exception_183: ;

  return UINT64_C(0xdeadc0de);
}

static struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 sailgen_doCSetBounds(struct zCapability, uint64_t);

struct zCapability zghz3326;
struct zCapability zghz3327;
struct zCapability zghz3328;
lbits zghz3329;
sail_int zghz3330;
sail_int zghz3331;
lbits zghz3332;
lbits zghz3333;

static void startup_sailgen_doCSetBounds(void)
{



  CREATE(lbits)(&zghz3329);
  CREATE(sail_int)(&zghz3330);
  CREATE(sail_int)(&zghz3331);
  CREATE(lbits)(&zghz3332);
  CREATE(lbits)(&zghz3333);
}

static struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 sailgen_doCSetBounds(struct zCapability zinput, uint64_t zreq_len)
{
  __label__ case_186, finish_match_185, end_function_187, end_block_exception_188;

  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz345;
  uint64_t znewBase;
  znewBase = zinput.zaddress;
  uint64_t znewTop;
  {
    uint64_t zgaz3351;
    {
      RECREATE(sail_int)(&zghz3331);
      CONVERT_OF(sail_int, mach_int)(&zghz3331, INT64_C(33));
      RECREATE(lbits)(&zghz3332);
      CONVERT_OF(lbits, fbits)(&zghz3332, znewBase, UINT64_C(32) , true);
      RECREATE(lbits)(&zghz3333);
      sailgen_zzero_extend(&zghz3333, zghz3331, zghz3332);
      zgaz3351 = CONVERT_OF(fbits, lbits)(zghz3333, true);
    }
    znewTop = ((zgaz3351 + zreq_len) & UINT64_C(0b111111111111111111111111111111111));
  }
  bool zinBounds;
  {
    int64_t zgaz3350;
    {
      RECREATE(lbits)(&zghz3329);
      CONVERT_OF(lbits, fbits)(&zghz3329, zreq_len, UINT64_C(33) , true);
      RECREATE(sail_int)(&zghz3330);
      sail_unsigned(&zghz3330, zghz3329);
      zgaz3350 = CONVERT_OF(mach_int, sail_int)(zghz3330);
    }
    zinBounds = sailgen_inCapBounds(zinput, znewBase, zgaz3350);
  }
  struct zCapability zghz3326;
  zghz3326 = sailgen_clearTagIfSealed(zinput);
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3347;
  zgaz3347 = sailgen_setCapBounds(zghz3326, znewBase, znewTop);
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgsz394;
  {
    bool zexact;
    zexact = zgaz3347.ztup0;
    struct zCapability zghz3327;
    zghz3327 = zgaz3347.ztup1;
    struct zCapability zghz3328;
    {
      bool zgaz3348;
      zgaz3348 = sailgen_not(zinBounds);
      zghz3328 = sailgen_clearTagIf(zghz3327, zgaz3348);
    }
    zgsz394.ztup0 = zexact;
    zgsz394.ztup1 = zghz3328;
    goto finish_match_185;
  }
case_186: ;
  sail_match_failure("doCSetBounds");
finish_match_185: ;
  zcbz345 = zgsz394;





end_function_187: ;
  return zcbz345;
end_block_exception_188: ;
  struct zCapability zcbz364 = { .zB = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zaddress = UINT64_C(0xdeadc0de), .zflag_cap_mode = false, .zglobal = false, .zinternal_E = false, .zotype = UINT64_C(0xdeadc0de), .zpermit_cinvoke = false, .zpermit_execute = false, .zpermit_load = false, .zpermit_load_cap = false, .zpermit_seal = false, .zpermit_set_CID = false, .zpermit_store = false, .zpermit_store_cap = false, .zpermit_store_local_cap = false, .zpermit_unseal = false, .zreserved = UINT64_C(0xdeadc0de), .ztag = false, .zuperms = UINT64_C(0xdeadc0de) };
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz363 = { .ztup0 = false, .ztup1 = zcbz364 };
  return zcbz363;
}



static void finish_sailgen_doCSetBounds(void)
{
  KILL(lbits)(&zghz3333);
  KILL(lbits)(&zghz3332);
  KILL(sail_int)(&zghz3331);
  KILL(sail_int)(&zghz3330);
  KILL(lbits)(&zghz3329);



}

// register TestCap
static struct zCapability zTestCap;

// register TestAddr
static uint64_t zTestAddr;

// register TestLen
static uint64_t zTestLen;

static unit sailgen_main(unit);

struct zCapability zghz3334;
struct zCapability zghz3335;
struct zCapability zghz3336;
struct zCapability zghz3337;
struct zCapability zghz3338;

static void startup_sailgen_main(void)
{





}


static unit sailgen_main(unit zgsz397)
{
  __label__ case_191, finish_match_190, end_function_196, end_block_exception_197;

  unit zcbz346;
  {
    zTestCap = znull_cap;
    unit zgsz398;
    zgsz398 = UNIT;
  }
  uint64_t zbits1;
  zbits1 = sailgen_capToBits(zTestCap);
  uint64_t zbits2;
  zbits2 = sailgen_capToMemBits(zTestCap);
  struct zCapability zghz3334;
  {
    bool zgaz3356;
    zgaz3356 = zTestCap.ztag;
    zghz3334 = sailgen_capBitsToCapability(zgaz3356, zbits1);
  }
  struct zCapability zghz3335;
  {
    bool zgaz3355;
    zgaz3355 = zTestCap.ztag;
    zghz3335 = sailgen_memBitsToCapability(zgaz3355, zbits2);
  }
  bool zrep;
  zrep = sailgen_fastRepCheck(zTestCap, zTestAddr);
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3352;
  zgaz3352 = sailgen_doCSetBounds(zTestCap, zTestLen);
  unit zgsz399;
  {
    __label__ case_193, finish_match_192;

    bool zexact;
    zexact = zgaz3352.ztup0;
    struct zCapability zghz3336;
    zghz3336 = zgaz3352.ztup1;
    struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3353;
    zgaz3353 = sailgen_setCapAddr(zTestCap, zTestAddr);
    {
      __label__ case_195, finish_match_194;

      bool zrepresentable;
      zrepresentable = zgaz3353.ztup0;
      struct zCapability zghz3337;
      zghz3337 = zgaz3353.ztup1;
      struct ztuple_z8z5i64zCz0z5i64z9 zgaz3354;
      zgaz3354 = sailgen_getCapBounds(zTestCap);
      unit zgsz3101;
      {
        int64_t zbase;
        zbase = zgaz3354.ztup0;
        int64_t ztop;
        ztop = zgaz3354.ztup1;
        uint64_t zflags;
        zflags = sailgen_getCapFlags(zTestCap);
        struct zCapability zghz3338;
        zghz3338 = sailgen_setCapPerms(zTestCap, UINT64_C(0b111111111111111));
        sail_string zcap_str;
        CREATE(sail_string)(&zcap_str);
        sailgen_capToString(&zcap_str, zTestCap);
        uint64_t zlen;
        zlen = sailgen_getRepresentableLength(zTestAddr);
        uint64_t zmask;
        zmask = sailgen_getRepresentableAlignmentMask(zTestAddr);
        zgsz3101 = UNIT;
        KILL(sail_string)(&zcap_str);
        goto finish_match_194;
      }
    case_195: ;
      sail_match_failure("main");
    finish_match_194: ;
      zgsz399 = zgsz3101;
      goto finish_match_192;
    }
  case_193: ;
    sail_match_failure("main");
  finish_match_192: ;
    goto finish_match_190;
  }
case_191: ;
  sail_match_failure("main");
finish_match_190: ;
  zcbz346 = zgsz399;





end_function_196: ;
  return zcbz346;
end_block_exception_197: ;

  return UNIT;
}



static void finish_sailgen_main(void)
{





}

static unit sailgen_initializze_registers(unit);

struct zCapability zghz3339;

static void startup_sailgen_initializze_registers(void)
{
}

static unit sailgen_initializze_registers(unit zgsz3105)
{
  __label__ end_function_199, end_block_exception_200;

  unit zcbz347;
  {
    struct zCapability zghz3339;
    zghz3339.zB = UINT64_C(0x00);
    zghz3339.zE = UINT64_C(0b000000);
    zghz3339.zT = UINT64_C(0x00);
    zghz3339.zaccess_system_regs = false;
    zghz3339.zaddress = UINT64_C(0x00000000);
    zghz3339.zflag_cap_mode = false;
    zghz3339.zglobal = false;
    zghz3339.zinternal_E = false;
    zghz3339.zotype = UINT64_C(0x0);
    zghz3339.zpermit_cinvoke = false;
    zghz3339.zpermit_execute = false;
    zghz3339.zpermit_load = false;
    zghz3339.zpermit_load_cap = false;
    zghz3339.zpermit_seal = false;
    zghz3339.zpermit_set_CID = false;
    zghz3339.zpermit_store = false;
    zghz3339.zpermit_store_cap = false;
    zghz3339.zpermit_store_local_cap = false;
    zghz3339.zpermit_unseal = false;
    zghz3339.zreserved = UINT64_C(0);
    zghz3339.ztag = false;
    zghz3339.zuperms = UINT64_C(0);
    zTestCap = zghz3339;
    unit zgsz3108;
    zgsz3108 = UNIT;
  }
  {
    zTestAddr = UINT64_C(0x00000000);
    unit zgsz3107;
    zgsz3107 = UNIT;
  }
  zTestLen = UINT64_C(0b000000000000000000000000000000000);
  zcbz347 = UNIT;
end_function_199: ;
  return zcbz347;
end_block_exception_200: ;

  return UNIT;
}



static void finish_sailgen_initializze_registers(void)
{
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
  startup_sailgen_z8operatorz0zKzJ_uz9();
  startup_sailgen_MAX();
  startup_sailgen_capBitsToEncCapability();
  startup_sailgen_undefined_Capability();
  startup_sailgen_encCapabilityToCapability();
  startup_sailgen_capBitsToCapability();
  startup_sailgen_capToEncCap();
  startup_sailgen_capToBits();
  startup_sailgen_memBitsToCapability();
  startup_sailgen_getCapBoundsBits();
  startup_sailgen_getCapBounds();
  startup_sailgen_setCapBounds();
  startup_sailgen_getCapPerms();
  startup_sailgen_setCapPerms();
  startup_sailgen_isCapSealed();
  startup_sailgen_hasReservedOType();
  startup_sailgen_getCapLength();
  startup_sailgen_inCapBounds();
  startup_sailgen_clearTagIf();
  startup_sailgen_clearTagIfSealed();
  startup_sailgen_capBoundsEqual();
  startup_sailgen_setCapAddr();
  startup_sailgen_fastRepCheck();
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
  finish_sailgen_sail_ones();
  finish_sailgen_to_bits();
  finish_sailgen_z8operatorz0zI_uz9();
  finish_sailgen_z8operatorz0zKzJ_uz9();
  finish_sailgen_MAX();
  finish_sailgen_capBitsToEncCapability();
  finish_sailgen_undefined_Capability();
  finish_sailgen_encCapabilityToCapability();
  finish_sailgen_capBitsToCapability();
  finish_sailgen_capToEncCap();
  finish_sailgen_capToBits();
  finish_sailgen_memBitsToCapability();
  finish_sailgen_getCapBoundsBits();
  finish_sailgen_getCapBounds();
  finish_sailgen_setCapBounds();
  finish_sailgen_getCapPerms();
  finish_sailgen_setCapPerms();
  finish_sailgen_isCapSealed();
  finish_sailgen_hasReservedOType();
  finish_sailgen_getCapLength();
  finish_sailgen_inCapBounds();
  finish_sailgen_clearTagIf();
  finish_sailgen_clearTagIfSealed();
  finish_sailgen_capBoundsEqual();
  finish_sailgen_setCapAddr();
  finish_sailgen_fastRepCheck();
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
