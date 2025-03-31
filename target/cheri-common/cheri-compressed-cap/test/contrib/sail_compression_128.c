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
  __label__ end_function_7, end_block_exception_8, end_function_214;

  RECREATE(lbits)(&zghz30);
  zeros(&zghz30, zn);
  not_bits((*(&zcbz32)), zghz30);
end_function_7: ;
  goto end_function_214;
end_block_exception_8: ;
  goto end_function_214;
end_function_214: ;
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
  __label__ end_function_16, end_block_exception_17, end_function_213;

  sign_extend((*(&zcbz35)), zv, zm);
end_function_16: ;
  goto end_function_213;
end_block_exception_17: ;
  goto end_function_213;
end_function_213: ;
}

static void sailgen_zzero_extend(lbits *zcbz36, sail_int zm, lbits zv)
{
  __label__ end_function_19, end_block_exception_20, end_function_212;

  zero_extend((*(&zcbz36)), zv, zm);
end_function_19: ;
  goto end_function_212;
end_block_exception_20: ;
  goto end_function_212;
end_function_212: ;
}

static void sailgen_zzeros_implicit(lbits *rop, sail_int);

static void sailgen_zzeros_implicit(lbits *zcbz37, sail_int zn)
{
  __label__ end_function_22, end_block_exception_23, end_function_211;

  zeros((*(&zcbz37)), zn);
end_function_22: ;
  goto end_function_211;
end_block_exception_23: ;
  goto end_function_211;
end_function_211: ;
}

static void sailgen_ones(lbits *rop, sail_int);

static void sailgen_ones(lbits *zcbz38, sail_int zn)
{
  __label__ end_function_25, end_block_exception_26, end_function_210;

  sailgen_sail_ones((*(&zcbz38)), zn);
end_function_25: ;
  goto end_function_210;
end_block_exception_26: ;
  goto end_function_210;
end_function_210: ;
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
  __label__ end_function_40, end_block_exception_41, end_function_209;

  {
    RECREATE(sail_int)(&zghz31);
    CONVERT_OF(sail_int, mach_int)(&zghz31, INT64_C(0));
    get_slice_int((*(&zcbz312)), zl, zn, zghz31);
  }
end_function_40: ;
  goto end_function_209;
end_block_exception_41: ;
  goto end_function_209;
end_function_209: ;
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
  __label__ end_function_49, end_block_exception_50, end_function_208;

  RECREATE(sail_int)(&zghz36);
  pow2(&zghz36, zn);
  {
    RECREATE(sail_int)(&zghz37);
    CONVERT_OF(sail_int, mach_int)(&zghz37, INT64_C(1));
    sub_int((*(&zcbz315)), zghz36, zghz37);
  }
end_function_49: ;
  goto end_function_208;
end_block_exception_50: ;
  goto end_function_208;
end_function_208: ;
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
  zgsz39 = INT64_C(16);
  zcap_sizze = zgsz39;

let_end_56: ;
}
static void kill_letbind_5(void) {
}

static int64_t zlog2_cap_sizze;
static void create_letbind_6(void) {


  int64_t zgsz310;
  zgsz310 = INT64_C(4);
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
  zgsz312 = INT64_C(4);
  zcap_uperms_width = zgsz312;

let_end_59: ;
}
static void kill_letbind_8(void) {
}

static int64_t zcap_otype_width;
static void create_letbind_9(void) {


  int64_t zgsz313;
  zgsz313 = INT64_C(18);
  zcap_otype_width = zgsz313;

let_end_60: ;
}
static void kill_letbind_9(void) {
}

static int64_t zcap_reserved_width;
static void create_letbind_10(void) {


  int64_t zgsz314;
  zgsz314 = INT64_C(2);
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
  zgsz316 = INT64_C(14);
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
  zgsz318 = INT64_C(64);
  zcap_addr_width = zgsz318;

let_end_65: ;
}
static void kill_letbind_14(void) {
}

static int64_t zcap_len_width;
static void create_letbind_15(void) {


  int64_t zgsz319;
  zgsz319 = INT64_C(65);
  zcap_len_width = zgsz319;

let_end_66: ;
}
static void kill_letbind_15(void) {
}

static int64_t zcaps_per_cache_line;
static void create_letbind_16(void) {


  int64_t zgsz320;
  zgsz320 = INT64_C(4);
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

static struct zEncCapability sailgen_capBitsToEncCapability(lbits);

struct zEncCapability zghz38;
sail_int zghz39;
sail_int zghz310;
lbits zghz311;
sail_int zghz312;
sail_int zghz313;
lbits zghz314;
sail_int zghz315;
sail_int zghz316;
lbits zghz317;
sail_int zghz318;
sail_int zghz319;
sail_int zghz320;
lbits zghz321;
sail_int zghz322;
sail_int zghz323;
lbits zghz324;
sail_int zghz325;
sail_int zghz326;
lbits zghz327;
sail_int zghz328;
sail_int zghz329;
lbits zghz330;

static void startup_sailgen_capBitsToEncCapability(void)
{

  CREATE(sail_int)(&zghz39);
  CREATE(sail_int)(&zghz310);
  CREATE(lbits)(&zghz311);
  CREATE(sail_int)(&zghz312);
  CREATE(sail_int)(&zghz313);
  CREATE(lbits)(&zghz314);
  CREATE(sail_int)(&zghz315);
  CREATE(sail_int)(&zghz316);
  CREATE(lbits)(&zghz317);
  CREATE(sail_int)(&zghz318);
  CREATE(sail_int)(&zghz319);
  CREATE(sail_int)(&zghz320);
  CREATE(lbits)(&zghz321);
  CREATE(sail_int)(&zghz322);
  CREATE(sail_int)(&zghz323);
  CREATE(lbits)(&zghz324);
  CREATE(sail_int)(&zghz325);
  CREATE(sail_int)(&zghz326);
  CREATE(lbits)(&zghz327);
  CREATE(sail_int)(&zghz328);
  CREATE(sail_int)(&zghz329);
  CREATE(lbits)(&zghz330);
}

static struct zEncCapability sailgen_capBitsToEncCapability(lbits zc)
{
  __label__ end_function_70, end_block_exception_71;

  struct zEncCapability zghz38;
  uint64_t zgaz310;
  {
    RECREATE(sail_int)(&zghz328);
    CONVERT_OF(sail_int, mach_int)(&zghz328, INT64_C(127));
    RECREATE(sail_int)(&zghz329);
    CONVERT_OF(sail_int, mach_int)(&zghz329, INT64_C(112));
    RECREATE(lbits)(&zghz330);
    vector_subrange_lbits(&zghz330, zc, zghz328, zghz329);
    zgaz310 = CONVERT_OF(fbits, lbits)(zghz330, true);
  }
  uint64_t zgaz311;
  {
    RECREATE(sail_int)(&zghz325);
    CONVERT_OF(sail_int, mach_int)(&zghz325, INT64_C(111));
    RECREATE(sail_int)(&zghz326);
    CONVERT_OF(sail_int, mach_int)(&zghz326, INT64_C(110));
    RECREATE(lbits)(&zghz327);
    vector_subrange_lbits(&zghz327, zc, zghz325, zghz326);
    zgaz311 = CONVERT_OF(fbits, lbits)(zghz327, true);
  }
  uint64_t zgaz312;
  {
    RECREATE(sail_int)(&zghz322);
    CONVERT_OF(sail_int, mach_int)(&zghz322, INT64_C(109));
    RECREATE(sail_int)(&zghz323);
    CONVERT_OF(sail_int, mach_int)(&zghz323, INT64_C(109));
    RECREATE(lbits)(&zghz324);
    vector_subrange_lbits(&zghz324, zc, zghz322, zghz323);
    zgaz312 = CONVERT_OF(fbits, lbits)(zghz324, true);
  }
  uint64_t zgaz313;
  {
    RECREATE(sail_int)(&zghz319);
    CONVERT_OF(sail_int, mach_int)(&zghz319, INT64_C(108));
    RECREATE(sail_int)(&zghz320);
    CONVERT_OF(sail_int, mach_int)(&zghz320, INT64_C(91));
    RECREATE(lbits)(&zghz321);
    vector_subrange_lbits(&zghz321, zc, zghz319, zghz320);
    zgaz313 = CONVERT_OF(fbits, lbits)(zghz321, true);
  }
  fbits zgaz314;
  {
    RECREATE(sail_int)(&zghz318);
    CONVERT_OF(sail_int, mach_int)(&zghz318, INT64_C(90));
    zgaz314 = bitvector_access(zc, zghz318);
  }
  uint64_t zgaz315;
  {
    RECREATE(sail_int)(&zghz315);
    CONVERT_OF(sail_int, mach_int)(&zghz315, INT64_C(89));
    RECREATE(sail_int)(&zghz316);
    CONVERT_OF(sail_int, mach_int)(&zghz316, INT64_C(78));
    RECREATE(lbits)(&zghz317);
    vector_subrange_lbits(&zghz317, zc, zghz315, zghz316);
    zgaz315 = CONVERT_OF(fbits, lbits)(zghz317, true);
  }
  uint64_t zgaz316;
  {
    RECREATE(sail_int)(&zghz312);
    CONVERT_OF(sail_int, mach_int)(&zghz312, INT64_C(77));
    RECREATE(sail_int)(&zghz313);
    CONVERT_OF(sail_int, mach_int)(&zghz313, INT64_C(64));
    RECREATE(lbits)(&zghz314);
    vector_subrange_lbits(&zghz314, zc, zghz312, zghz313);
    zgaz316 = CONVERT_OF(fbits, lbits)(zghz314, true);
  }
  uint64_t zgaz317;
  {
    RECREATE(sail_int)(&zghz39);
    CONVERT_OF(sail_int, mach_int)(&zghz39, INT64_C(63));
    RECREATE(sail_int)(&zghz310);
    CONVERT_OF(sail_int, mach_int)(&zghz310, INT64_C(0));
    RECREATE(lbits)(&zghz311);
    vector_subrange_lbits(&zghz311, zc, zghz39, zghz310);
    zgaz317 = CONVERT_OF(fbits, lbits)(zghz311, true);
  }
  zghz38.zB = zgaz316;
  zghz38.zT = zgaz315;
  zghz38.zaddress = zgaz317;
  zghz38.zflags = zgaz312;
  zghz38.zinternal_E = zgaz314;
  zghz38.zotype = zgaz313;
  zghz38.zperms = zgaz310;
  zghz38.zreserved = zgaz311;








end_function_70: ;
  return zghz38;
end_block_exception_71: ;
  struct zEncCapability zcbz348 = { .zB = UINT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zflags = UINT64_C(0xdeadc0de), .zinternal_E = UINT64_C(0), .zotype = UINT64_C(0xdeadc0de), .zperms = UINT64_C(0xdeadc0de), .zreserved = UINT64_C(0xdeadc0de) };
  return zcbz348;
}



static void finish_sailgen_capBitsToEncCapability(void)
{
  KILL(lbits)(&zghz330);
  KILL(sail_int)(&zghz329);
  KILL(sail_int)(&zghz328);
  KILL(lbits)(&zghz327);
  KILL(sail_int)(&zghz326);
  KILL(sail_int)(&zghz325);
  KILL(lbits)(&zghz324);
  KILL(sail_int)(&zghz323);
  KILL(sail_int)(&zghz322);
  KILL(lbits)(&zghz321);
  KILL(sail_int)(&zghz320);
  KILL(sail_int)(&zghz319);
  KILL(sail_int)(&zghz318);
  KILL(lbits)(&zghz317);
  KILL(sail_int)(&zghz316);
  KILL(sail_int)(&zghz315);
  KILL(lbits)(&zghz314);
  KILL(sail_int)(&zghz313);
  KILL(sail_int)(&zghz312);
  KILL(lbits)(&zghz311);
  KILL(sail_int)(&zghz310);
  KILL(sail_int)(&zghz39);

}

static void sailgen_encCapToBits(lbits *rop, struct zEncCapability);

lbits zghz331;
lbits zghz332;
lbits zghz333;
lbits zghz334;
lbits zghz335;
lbits zghz336;
lbits zghz337;
lbits zghz338;
lbits zghz339;
lbits zghz340;
lbits zghz341;
lbits zghz342;
lbits zghz343;
lbits zghz344;

static void startup_sailgen_encCapToBits(void)
{
  CREATE(lbits)(&zghz331);
  CREATE(lbits)(&zghz332);
  CREATE(lbits)(&zghz333);
  CREATE(lbits)(&zghz334);
  CREATE(lbits)(&zghz335);
  CREATE(lbits)(&zghz336);
  CREATE(lbits)(&zghz337);
  CREATE(lbits)(&zghz338);
  CREATE(lbits)(&zghz339);
  CREATE(lbits)(&zghz340);
  CREATE(lbits)(&zghz341);
  CREATE(lbits)(&zghz342);
  CREATE(lbits)(&zghz343);
  CREATE(lbits)(&zghz344);
}

static void sailgen_encCapToBits(lbits *zcbz317, struct zEncCapability zcap)
{
  __label__ end_function_73, end_block_exception_74, end_function_207;

  uint64_t zgaz331;
  zgaz331 = zcap.zperms;
  RECREATE(lbits)(&zghz331);
  {
    uint64_t zgaz329;
    zgaz329 = zcap.zreserved;
    RECREATE(lbits)(&zghz333);
    {
      uint64_t zgaz327;
      zgaz327 = zcap.zflags;
      RECREATE(lbits)(&zghz335);
      {
        uint64_t zgaz325;
        zgaz325 = zcap.zotype;
        RECREATE(lbits)(&zghz337);
        {
          uint64_t zgaz323;
          {
            fbits zgaz318;
            zgaz318 = zcap.zinternal_E;
            zgaz323 = UINT64_C(0b0);
            zgaz323 = update_fbits(zgaz323, INT64_C(0), zgaz318);
          }
          RECREATE(lbits)(&zghz339);
          {
            uint64_t zgaz321;
            zgaz321 = zcap.zT;
            RECREATE(lbits)(&zghz341);
            {
              uint64_t zgaz319;
              zgaz319 = zcap.zB;
              uint64_t zgaz320;
              zgaz320 = zcap.zaddress;
              {
                RECREATE(lbits)(&zghz343);
                CONVERT_OF(lbits, fbits)(&zghz343, zgaz319, UINT64_C(14) , true);
                RECREATE(lbits)(&zghz344);
                CONVERT_OF(lbits, fbits)(&zghz344, zgaz320, UINT64_C(64) , true);
                append(&zghz341, zghz343, zghz344);
              }
            }
            {
              RECREATE(lbits)(&zghz342);
              CONVERT_OF(lbits, fbits)(&zghz342, zgaz321, UINT64_C(12) , true);
              append(&zghz339, zghz342, zghz341);
            }
          }
          {
            RECREATE(lbits)(&zghz340);
            CONVERT_OF(lbits, fbits)(&zghz340, zgaz323, UINT64_C(1) , true);
            append(&zghz337, zghz340, zghz339);
          }
        }
        {
          RECREATE(lbits)(&zghz338);
          CONVERT_OF(lbits, fbits)(&zghz338, zgaz325, UINT64_C(18) , true);
          append(&zghz335, zghz338, zghz337);
        }
      }
      {
        RECREATE(lbits)(&zghz336);
        CONVERT_OF(lbits, fbits)(&zghz336, zgaz327, UINT64_C(1) , true);
        append(&zghz333, zghz336, zghz335);
      }
    }
    {
      RECREATE(lbits)(&zghz334);
      CONVERT_OF(lbits, fbits)(&zghz334, zgaz329, UINT64_C(2) , true);
      append(&zghz331, zghz334, zghz333);
    }
  }
  {
    RECREATE(lbits)(&zghz332);
    CONVERT_OF(lbits, fbits)(&zghz332, zgaz331, UINT64_C(16) , true);
    append((*(&zcbz317)), zghz332, zghz331);
  }

end_function_73: ;
  goto end_function_207;
end_block_exception_74: ;
  goto end_function_207;
end_function_207: ;
}



static void finish_sailgen_encCapToBits(void)
{
  KILL(lbits)(&zghz344);
  KILL(lbits)(&zghz343);
  KILL(lbits)(&zghz342);
  KILL(lbits)(&zghz341);
  KILL(lbits)(&zghz340);
  KILL(lbits)(&zghz339);
  KILL(lbits)(&zghz338);
  KILL(lbits)(&zghz337);
  KILL(lbits)(&zghz336);
  KILL(lbits)(&zghz335);
  KILL(lbits)(&zghz334);
  KILL(lbits)(&zghz333);
  KILL(lbits)(&zghz332);
  KILL(lbits)(&zghz331);
}

static sail_int zcap_max_addr;
static void create_letbind_18(void) {    CREATE(sail_int)(&zcap_max_addr);


  sail_int zgsz324;
  CREATE(sail_int)(&zgsz324);
  {
    sail_int zgsz3142;
    CREATE(sail_int)(&zgsz3142);
    CONVERT_OF(sail_int, mach_int)(&zgsz3142, zcap_addr_width);
    sailgen_MAX(&zgsz324, zgsz3142);
    KILL(sail_int)(&zgsz3142);
  }
  COPY(sail_int)(&zcap_max_addr, zgsz324);
  KILL(sail_int)(&zgsz324);
let_end_75: ;
}
static void kill_letbind_18(void) {    KILL(sail_int)(&zcap_max_addr);
}

static int64_t zcap_max_otype;
static void create_letbind_19(void) {


  int64_t zgsz325;
  int64_t zgaz333;
  {
    sail_int zgsz3146;
    CREATE(sail_int)(&zgsz3146);
    CONVERT_OF(sail_int, mach_int)(&zgsz3146, zcap_otype_width);
    sail_int zgsz3147;
    CREATE(sail_int)(&zgsz3147);
    sailgen_MAX(&zgsz3147, zgsz3146);
    zgaz333 = CONVERT_OF(mach_int, sail_int)(zgsz3147);
    KILL(sail_int)(&zgsz3147);
    KILL(sail_int)(&zgsz3146);
  }
  {
    sail_int zgsz3143;
    CREATE(sail_int)(&zgsz3143);
    CONVERT_OF(sail_int, mach_int)(&zgsz3143, zgaz333);
    sail_int zgsz3144;
    CREATE(sail_int)(&zgsz3144);
    CONVERT_OF(sail_int, mach_int)(&zgsz3144, zreserved_otypes);
    sail_int zgsz3145;
    CREATE(sail_int)(&zgsz3145);
    sub_int(&zgsz3145, zgsz3143, zgsz3144);
    zgsz325 = CONVERT_OF(mach_int, sail_int)(zgsz3145);
    KILL(sail_int)(&zgsz3145);
    KILL(sail_int)(&zgsz3144);
    KILL(sail_int)(&zgsz3143);
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
  zgsz327 = INT64_C(19);
  zcap_perms_width = zgsz327;

let_end_78: ;
}
static void kill_letbind_21(void) {
}

static int64_t zcap_max_E;
static void create_letbind_22(void) {


  int64_t zgsz328;
  int64_t zgaz334;
  {
    sail_int zgsz3148;
    CREATE(sail_int)(&zgsz3148);
    CONVERT_OF(sail_int, mach_int)(&zgsz3148, zcap_len_width);
    sail_int zgsz3149;
    CREATE(sail_int)(&zgsz3149);
    CONVERT_OF(sail_int, mach_int)(&zgsz3149, zcap_mantissa_width);
    sail_int zgsz3150;
    CREATE(sail_int)(&zgsz3150);
    sub_int(&zgsz3150, zgsz3148, zgsz3149);
    zgaz334 = CONVERT_OF(mach_int, sail_int)(zgsz3150);
    KILL(sail_int)(&zgsz3150);
    KILL(sail_int)(&zgsz3149);
    KILL(sail_int)(&zgsz3148);
  }
  zgsz328 = (zgaz334 + INT64_C(1));

  zcap_max_E = zgsz328;

let_end_79: ;
}
static void kill_letbind_22(void) {
}

static uint64_t zcap_reset_E;
static void create_letbind_23(void) {


  uint64_t zgsz329;
  {
    sail_int zgsz3151;
    CREATE(sail_int)(&zgsz3151);
    CONVERT_OF(sail_int, mach_int)(&zgsz3151, zcap_E_width);
    sail_int zgsz3152;
    CREATE(sail_int)(&zgsz3152);
    CONVERT_OF(sail_int, mach_int)(&zgsz3152, zcap_max_E);
    lbits zgsz3153;
    CREATE(lbits)(&zgsz3153);
    sailgen_to_bits(&zgsz3153, zgsz3151, zgsz3152);
    zgsz329 = CONVERT_OF(fbits, lbits)(zgsz3153, true);
    KILL(lbits)(&zgsz3153);
    KILL(sail_int)(&zgsz3152);
    KILL(sail_int)(&zgsz3151);
  }
  zcap_reset_E = zgsz329;

let_end_80: ;
}
static void kill_letbind_23(void) {
}

static uint64_t zcap_reset_T;
static void create_letbind_24(void) {


  uint64_t zgsz330;
  uint64_t zgaz336;
  {
    int64_t zgaz335;
    {
      sail_int zgsz3156;
      CREATE(sail_int)(&zgsz3156);
      CONVERT_OF(sail_int, mach_int)(&zgsz3156, zcap_mantissa_width);
      sail_int zgsz3157;
      CREATE(sail_int)(&zgsz3157);
      CONVERT_OF(sail_int, mach_int)(&zgsz3157, INT64_C(2));
      sail_int zgsz3158;
      CREATE(sail_int)(&zgsz3158);
      sub_int(&zgsz3158, zgsz3156, zgsz3157);
      zgaz335 = CONVERT_OF(mach_int, sail_int)(zgsz3158);
      KILL(sail_int)(&zgsz3158);
      KILL(sail_int)(&zgsz3157);
      KILL(sail_int)(&zgsz3156);
    }
    {
      sail_int zgsz3154;
      CREATE(sail_int)(&zgsz3154);
      CONVERT_OF(sail_int, mach_int)(&zgsz3154, zgaz335);
      lbits zgsz3155;
      CREATE(lbits)(&zgsz3155);
      sailgen_zzeros_implicit(&zgsz3155, zgsz3154);
      zgaz336 = CONVERT_OF(fbits, lbits)(zgsz3155, true);
      KILL(lbits)(&zgsz3155);
      KILL(sail_int)(&zgsz3154);
    }
  }
  zgsz330 = (UINT64_C(0b01) << 12) | zgaz336;

  zcap_reset_T = zgsz330;

let_end_81: ;
}
static void kill_letbind_24(void) {
}

static struct zCapability sailgen_undefined_Capability(unit);

struct zCapability zghz345;

static void startup_sailgen_undefined_Capability(void)
{
}

static struct zCapability sailgen_undefined_Capability(unit zgsz331)
{
  __label__ end_function_83, end_block_exception_84;

  struct zCapability zghz345;
  zghz345.zB = UINT64_C(0b00000000000000);
  zghz345.zE = UINT64_C(0b000000);
  zghz345.zT = UINT64_C(0b00000000000000);
  zghz345.zaccess_system_regs = false;
  zghz345.zaddress = UINT64_C(0x0000000000000000);
  zghz345.zflag_cap_mode = false;
  zghz345.zglobal = false;
  zghz345.zinternal_E = false;
  zghz345.zotype = UINT64_C(0b000000000000000000);
  zghz345.zpermit_cinvoke = false;
  zghz345.zpermit_execute = false;
  zghz345.zpermit_load = false;
  zghz345.zpermit_load_cap = false;
  zghz345.zpermit_seal = false;
  zghz345.zpermit_set_CID = false;
  zghz345.zpermit_store = false;
  zghz345.zpermit_store_cap = false;
  zghz345.zpermit_store_local_cap = false;
  zghz345.zpermit_unseal = false;
  zghz345.zreserved = UINT64_C(0b00);
  zghz345.ztag = false;
  zghz345.zuperms = UINT64_C(0x0);
end_function_83: ;
  return zghz345;
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
  uint64_t zgaz337;
  {
    sail_int zgsz3159;
    CREATE(sail_int)(&zgsz3159);
    CONVERT_OF(sail_int, mach_int)(&zgsz3159, zcap_otype_width);
    sail_int zgsz3160;
    CREATE(sail_int)(&zgsz3160);
    CONVERT_OF(sail_int, mach_int)(&zgsz3160, zotype_unsealed);
    lbits zgsz3161;
    CREATE(lbits)(&zgsz3161);
    sailgen_to_bits(&zgsz3161, zgsz3159, zgsz3160);
    zgaz337 = CONVERT_OF(fbits, lbits)(zgsz3161, true);
    KILL(lbits)(&zgsz3161);
    KILL(sail_int)(&zgsz3160);
    KILL(sail_int)(&zgsz3159);
  }
  struct zCapability zgsz333;
  zgsz333.zB = UINT64_C(0b00000000000000);
  zgsz333.zE = zcap_reset_E;
  zgsz333.zT = zcap_reset_T;
  zgsz333.zaccess_system_regs = false;
  zgsz333.zaddress = UINT64_C(0x0000000000000000);
  zgsz333.zflag_cap_mode = false;
  zgsz333.zglobal = false;
  zgsz333.zinternal_E = true;
  zgsz333.zotype = zgaz337;
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
  zgsz333.zreserved = UINT64_C(0b00);
  zgsz333.ztag = false;
  zgsz333.zuperms = UINT64_C(0x0);
  zgsz334 = zgsz333;


  znull_cap = zgsz334;

let_end_85: ;
}
static void kill_letbind_25(void) {
}

static struct zCapability zdefault_cap;
static void create_letbind_26(void) {


  struct zCapability zgsz336;
  uint64_t zgaz338;
  {
    sail_int zgsz3162;
    CREATE(sail_int)(&zgsz3162);
    CONVERT_OF(sail_int, mach_int)(&zgsz3162, zcap_otype_width);
    sail_int zgsz3163;
    CREATE(sail_int)(&zgsz3163);
    CONVERT_OF(sail_int, mach_int)(&zgsz3163, zotype_unsealed);
    lbits zgsz3164;
    CREATE(lbits)(&zgsz3164);
    sailgen_to_bits(&zgsz3164, zgsz3162, zgsz3163);
    zgaz338 = CONVERT_OF(fbits, lbits)(zgsz3164, true);
    KILL(lbits)(&zgsz3164);
    KILL(sail_int)(&zgsz3163);
    KILL(sail_int)(&zgsz3162);
  }
  struct zCapability zgsz335;
  zgsz335.zB = UINT64_C(0b00000000000000);
  zgsz335.zE = zcap_reset_E;
  zgsz335.zT = zcap_reset_T;
  zgsz335.zaccess_system_regs = true;
  zgsz335.zaddress = UINT64_C(0x0000000000000000);
  zgsz335.zflag_cap_mode = false;
  zgsz335.zglobal = true;
  zgsz335.zinternal_E = true;
  zgsz335.zotype = zgaz338;
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
  zgsz335.zreserved = UINT64_C(0b00);
  zgsz335.ztag = true;
  zgsz335.zuperms = UINT64_C(0xF);
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
  uint64_t zgaz371;
  {
    bool zgaz339;
    zgaz339 = zcap.zpermit_set_CID;
    zgaz371 = sailgen_bool_to_bits(zgaz339);
  }
  uint64_t zgaz372;
  {
    uint64_t zgaz369;
    {
      bool zgaz340;
      zgaz340 = zcap.zaccess_system_regs;
      zgaz369 = sailgen_bool_to_bits(zgaz340);
    }
    uint64_t zgaz370;
    {
      uint64_t zgaz367;
      {
        bool zgaz341;
        zgaz341 = zcap.zpermit_unseal;
        zgaz367 = sailgen_bool_to_bits(zgaz341);
      }
      uint64_t zgaz368;
      {
        uint64_t zgaz365;
        {
          bool zgaz342;
          zgaz342 = zcap.zpermit_cinvoke;
          zgaz365 = sailgen_bool_to_bits(zgaz342);
        }
        uint64_t zgaz366;
        {
          uint64_t zgaz363;
          {
            bool zgaz343;
            zgaz343 = zcap.zpermit_seal;
            zgaz363 = sailgen_bool_to_bits(zgaz343);
          }
          uint64_t zgaz364;
          {
            uint64_t zgaz361;
            {
              bool zgaz344;
              zgaz344 = zcap.zpermit_store_local_cap;
              zgaz361 = sailgen_bool_to_bits(zgaz344);
            }
            uint64_t zgaz362;
            {
              uint64_t zgaz359;
              {
                bool zgaz345;
                zgaz345 = zcap.zpermit_store_cap;
                zgaz359 = sailgen_bool_to_bits(zgaz345);
              }
              uint64_t zgaz360;
              {
                uint64_t zgaz357;
                {
                  bool zgaz346;
                  zgaz346 = zcap.zpermit_load_cap;
                  zgaz357 = sailgen_bool_to_bits(zgaz346);
                }
                uint64_t zgaz358;
                {
                  uint64_t zgaz355;
                  {
                    bool zgaz347;
                    zgaz347 = zcap.zpermit_store;
                    zgaz355 = sailgen_bool_to_bits(zgaz347);
                  }
                  uint64_t zgaz356;
                  {
                    uint64_t zgaz353;
                    {
                      bool zgaz348;
                      zgaz348 = zcap.zpermit_load;
                      zgaz353 = sailgen_bool_to_bits(zgaz348);
                    }
                    uint64_t zgaz354;
                    {
                      uint64_t zgaz351;
                      {
                        bool zgaz349;
                        zgaz349 = zcap.zpermit_execute;
                        zgaz351 = sailgen_bool_to_bits(zgaz349);
                      }
                      uint64_t zgaz352;
                      {
                        bool zgaz350;
                        zgaz350 = zcap.zglobal;
                        zgaz352 = sailgen_bool_to_bits(zgaz350);
                      }
                      zgaz354 = (zgaz351 << 1) | zgaz352;
                    }
                    zgaz356 = (zgaz353 << 2) | zgaz354;
                  }
                  zgaz358 = (zgaz355 << 3) | zgaz356;
                }
                zgaz360 = (zgaz357 << 4) | zgaz358;
              }
              zgaz362 = (zgaz359 << 5) | zgaz360;
            }
            zgaz364 = (zgaz361 << 6) | zgaz362;
          }
          zgaz366 = (zgaz363 << 7) | zgaz364;
        }
        zgaz368 = (zgaz365 << 8) | zgaz366;
      }
      zgaz370 = (zgaz367 << 9) | zgaz368;
    }
    zgaz372 = (zgaz369 << 10) | zgaz370;
  }
  zcbz319 = (zgaz371 << 11) | zgaz372;


end_function_88: ;
  return zcbz319;
end_block_exception_89: ;

  return UINT64_C(0xdeadc0de);
}

static struct zCapability sailgen_encCapabilityToCapability(bool, struct zEncCapability);

struct zCapability zghz346;
struct zCapability zghz347;
sail_int zghz348;
sail_int zghz349;
sail_int zghz350;
sail_int zghz351;
sail_int zghz352;
sail_int zghz353;
sail_int zghz354;
sail_int zghz355;
sail_int zghz356;
lbits zghz357;
lbits zghz358;
sail_int zghz359;
sail_int zghz360;
sail_int zghz361;
sail_int zghz362;
lbits zghz363;
sail_int zghz364;
sail_int zghz365;
sail_int zghz366;
sail_int zghz367;
lbits zghz368;
sail_int zghz369;
sail_int zghz370;
sail_int zghz371;
sail_int zghz372;
sail_int zghz373;
sail_int zghz374;
sail_int zghz375;
sail_int zghz376;
sail_int zghz377;

static void startup_sailgen_encCapabilityToCapability(void)
{


  CREATE(sail_int)(&zghz348);
  CREATE(sail_int)(&zghz349);
  CREATE(sail_int)(&zghz350);
  CREATE(sail_int)(&zghz351);
  CREATE(sail_int)(&zghz352);
  CREATE(sail_int)(&zghz353);
  CREATE(sail_int)(&zghz354);
  CREATE(sail_int)(&zghz355);
  CREATE(sail_int)(&zghz356);
  CREATE(lbits)(&zghz357);
  CREATE(lbits)(&zghz358);
  CREATE(sail_int)(&zghz359);
  CREATE(sail_int)(&zghz360);
  CREATE(sail_int)(&zghz361);
  CREATE(sail_int)(&zghz362);
  CREATE(lbits)(&zghz363);
  CREATE(sail_int)(&zghz364);
  CREATE(sail_int)(&zghz365);
  CREATE(sail_int)(&zghz366);
  CREATE(sail_int)(&zghz367);
  CREATE(lbits)(&zghz368);
  CREATE(sail_int)(&zghz369);
  CREATE(sail_int)(&zghz370);
  CREATE(sail_int)(&zghz371);
  CREATE(sail_int)(&zghz372);
  CREATE(sail_int)(&zghz373);
  CREATE(sail_int)(&zghz374);
  CREATE(sail_int)(&zghz375);
  CREATE(sail_int)(&zghz376);
  CREATE(sail_int)(&zghz377);
}

static struct zCapability sailgen_encCapabilityToCapability(bool zt, struct zEncCapability zc)
{
  __label__ cleanup_92, end_cleanup_93, end_function_91, end_block_exception_94;

  struct zCapability zghz346;
  bool zinternal_E;
  {
    fbits zgaz3143;
    zgaz3143 = zc.zinternal_E;
    zinternal_E = sailgen_bit_to_bool(zgaz3143);
  }
  uint64_t zE;
  zE = UINT64_C(0b000000);
  uint64_t zBs;
  zBs = UINT64_C(0b00000000000000);
  uint64_t zT;
  zT = UINT64_C(0x000);
  uint64_t zlenMSBs;
  zlenMSBs = UINT64_C(0b00);
  {
    unit zgsz337;
    if (zinternal_E) {
      {
        uint64_t zgaz377;
        {
          uint64_t zgaz373;
          zgaz373 = zc.zT;
          int64_t zgaz374;
          {
            RECREATE(sail_int)(&zghz375);
            CONVERT_OF(sail_int, mach_int)(&zghz375, zinternal_E_take_bits);
            RECREATE(sail_int)(&zghz376);
            CONVERT_OF(sail_int, mach_int)(&zghz376, INT64_C(1));
            RECREATE(sail_int)(&zghz377);
            sub_int(&zghz377, zghz375, zghz376);
            zgaz374 = CONVERT_OF(mach_int, sail_int)(zghz377);
          }
          zgaz377 = (safe_rshift(UINT64_MAX, 64 - 3) & (zgaz373 >> INT64_C(0)));
        }
        uint64_t zgaz378;
        {
          uint64_t zgaz375;
          zgaz375 = zc.zB;
          int64_t zgaz376;
          {
            RECREATE(sail_int)(&zghz372);
            CONVERT_OF(sail_int, mach_int)(&zghz372, zinternal_E_take_bits);
            RECREATE(sail_int)(&zghz373);
            CONVERT_OF(sail_int, mach_int)(&zghz373, INT64_C(1));
            RECREATE(sail_int)(&zghz374);
            sub_int(&zghz374, zghz372, zghz373);
            zgaz376 = CONVERT_OF(mach_int, sail_int)(zghz374);
          }
          zgaz378 = (safe_rshift(UINT64_MAX, 64 - 3) & (zgaz375 >> INT64_C(0)));
        }
        zE = (zgaz377 << 3) | zgaz378;
        unit zgsz342;
        zgsz342 = UNIT;
      }
      {
        zlenMSBs = UINT64_C(0b01);
        unit zgsz341;
        zgsz341 = UNIT;
      }
      {
        uint64_t zgaz381;
        {
          uint64_t zgaz379;
          zgaz379 = zc.zT;
          int64_t zgaz380;
          {
            RECREATE(sail_int)(&zghz369);
            CONVERT_OF(sail_int, mach_int)(&zghz369, zcap_mantissa_width);
            RECREATE(sail_int)(&zghz370);
            CONVERT_OF(sail_int, mach_int)(&zghz370, INT64_C(3));
            RECREATE(sail_int)(&zghz371);
            sub_int(&zghz371, zghz369, zghz370);
            zgaz380 = CONVERT_OF(mach_int, sail_int)(zghz371);
          }
          zgaz381 = (safe_rshift(UINT64_MAX, 64 - 9) & (zgaz379 >> INT64_C(3)));
        }
        uint64_t zgaz382;
        {
          RECREATE(sail_int)(&zghz367);
          CONVERT_OF(sail_int, mach_int)(&zghz367, zinternal_E_take_bits);
          RECREATE(lbits)(&zghz368);
          sailgen_zzeros_implicit(&zghz368, zghz367);
          zgaz382 = CONVERT_OF(fbits, lbits)(zghz368, true);
        }
        zT = (zgaz381 << 3) | zgaz382;
        unit zgsz340;
        zgsz340 = UNIT;
      }
      uint64_t zgaz385;
      {
        uint64_t zgaz383;
        zgaz383 = zc.zB;
        int64_t zgaz384;
        {
          RECREATE(sail_int)(&zghz364);
          CONVERT_OF(sail_int, mach_int)(&zghz364, zcap_mantissa_width);
          RECREATE(sail_int)(&zghz365);
          CONVERT_OF(sail_int, mach_int)(&zghz365, INT64_C(1));
          RECREATE(sail_int)(&zghz366);
          sub_int(&zghz366, zghz364, zghz365);
          zgaz384 = CONVERT_OF(mach_int, sail_int)(zghz366);
        }
        zgaz385 = (safe_rshift(UINT64_MAX, 64 - 11) & (zgaz383 >> INT64_C(3)));
      }
      uint64_t zgaz386;
      {
        RECREATE(sail_int)(&zghz362);
        CONVERT_OF(sail_int, mach_int)(&zghz362, zinternal_E_take_bits);
        RECREATE(lbits)(&zghz363);
        sailgen_zzeros_implicit(&zghz363, zghz362);
        zgaz386 = CONVERT_OF(fbits, lbits)(zghz363, true);
      }
      zBs = (zgaz385 << 3) | zgaz386;
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
    bool zgaz3142;
    {
      uint64_t zgaz3141;
      {
        int64_t zgaz3140;
        {
          RECREATE(sail_int)(&zghz359);
          CONVERT_OF(sail_int, mach_int)(&zghz359, zcap_mantissa_width);
          RECREATE(sail_int)(&zghz360);
          CONVERT_OF(sail_int, mach_int)(&zghz360, INT64_C(3));
          RECREATE(sail_int)(&zghz361);
          sub_int(&zghz361, zghz359, zghz360);
          zgaz3140 = CONVERT_OF(mach_int, sail_int)(zghz361);
        }
        zgaz3141 = (safe_rshift(UINT64_MAX, 64 - 12) & (zBs >> INT64_C(0)));
      }
      {
        RECREATE(lbits)(&zghz357);
        CONVERT_OF(lbits, fbits)(&zghz357, zT, UINT64_C(12) , true);
        RECREATE(lbits)(&zghz358);
        CONVERT_OF(lbits, fbits)(&zghz358, zgaz3141, UINT64_C(12) , true);
        zgaz3142 = sailgen_z8operatorz0zI_uz9(zghz357, zghz358);
      }
    }
    if (zgaz3142) {  zcarry_out = UINT64_C(0b01);  } else {  zcarry_out = UINT64_C(0b00);  }
  }
  uint64_t zTtop2;
  {
    uint64_t zgaz3139;
    {
      uint64_t zgaz3138;
      {
        int64_t zgaz3136;
        {
          RECREATE(sail_int)(&zghz354);
          CONVERT_OF(sail_int, mach_int)(&zghz354, zcap_mantissa_width);
          RECREATE(sail_int)(&zghz355);
          CONVERT_OF(sail_int, mach_int)(&zghz355, INT64_C(1));
          RECREATE(sail_int)(&zghz356);
          sub_int(&zghz356, zghz354, zghz355);
          zgaz3136 = CONVERT_OF(mach_int, sail_int)(zghz356);
        }
        int64_t zgaz3137;
        {
          RECREATE(sail_int)(&zghz351);
          CONVERT_OF(sail_int, mach_int)(&zghz351, zcap_mantissa_width);
          RECREATE(sail_int)(&zghz352);
          CONVERT_OF(sail_int, mach_int)(&zghz352, INT64_C(2));
          RECREATE(sail_int)(&zghz353);
          sub_int(&zghz353, zghz351, zghz352);
          zgaz3137 = CONVERT_OF(mach_int, sail_int)(zghz353);
        }
        zgaz3138 = (safe_rshift(UINT64_MAX, 64 - 2) & (zBs >> zgaz3137));
      }
      zgaz3139 = ((zgaz3138 + zlenMSBs) & UINT64_C(0b11));
    }
    zTtop2 = ((zgaz3139 + zcarry_out) & UINT64_C(0b11));
  }
  {
    uint64_t zgaz391;
    {
      bool zgaz387;
      zgaz387 = (zcap_uperms_width > INT64_C(0));
      uint64_t zgaz389;
      zgaz389 = zc.zperms;
      int64_t zgaz390;
      {
        int64_t zgaz388;
        zgaz388 = (zcap_hperms_width + zcap_uperms_width);
        {
          RECREATE(sail_int)(&zghz348);
          CONVERT_OF(sail_int, mach_int)(&zghz348, zgaz388);
          RECREATE(sail_int)(&zghz349);
          CONVERT_OF(sail_int, mach_int)(&zghz349, INT64_C(1));
          RECREATE(sail_int)(&zghz350);
          sub_int(&zghz350, zghz348, zghz349);
          zgaz390 = CONVERT_OF(mach_int, sail_int)(zghz350);
        }
      }
      zgaz391 = (safe_rshift(UINT64_MAX, 64 - 4) & (zgaz389 >> zcap_hperms_width));
    }
    bool zgaz394;
    {
      fbits zgaz393;
      {
        uint64_t zgaz392;
        zgaz392 = zc.zperms;
        zgaz393 = (UINT64_C(1) & (zgaz392 >> INT64_C(11)));
      }
      zgaz394 = sailgen_bit_to_bool(zgaz393);
    }
    bool zgaz397;
    {
      fbits zgaz396;
      {
        uint64_t zgaz395;
        zgaz395 = zc.zperms;
        zgaz396 = (UINT64_C(1) & (zgaz395 >> INT64_C(10)));
      }
      zgaz397 = sailgen_bit_to_bool(zgaz396);
    }
    bool zgaz3100;
    {
      fbits zgaz399;
      {
        uint64_t zgaz398;
        zgaz398 = zc.zperms;
        zgaz399 = (UINT64_C(1) & (zgaz398 >> INT64_C(9)));
      }
      zgaz3100 = sailgen_bit_to_bool(zgaz399);
    }
    bool zgaz3103;
    {
      fbits zgaz3102;
      {
        uint64_t zgaz3101;
        zgaz3101 = zc.zperms;
        zgaz3102 = (UINT64_C(1) & (zgaz3101 >> INT64_C(8)));
      }
      zgaz3103 = sailgen_bit_to_bool(zgaz3102);
    }
    bool zgaz3106;
    {
      fbits zgaz3105;
      {
        uint64_t zgaz3104;
        zgaz3104 = zc.zperms;
        zgaz3105 = (UINT64_C(1) & (zgaz3104 >> INT64_C(7)));
      }
      zgaz3106 = sailgen_bit_to_bool(zgaz3105);
    }
    bool zgaz3109;
    {
      fbits zgaz3108;
      {
        uint64_t zgaz3107;
        zgaz3107 = zc.zperms;
        zgaz3108 = (UINT64_C(1) & (zgaz3107 >> INT64_C(6)));
      }
      zgaz3109 = sailgen_bit_to_bool(zgaz3108);
    }
    bool zgaz3112;
    {
      fbits zgaz3111;
      {
        uint64_t zgaz3110;
        zgaz3110 = zc.zperms;
        zgaz3111 = (UINT64_C(1) & (zgaz3110 >> INT64_C(5)));
      }
      zgaz3112 = sailgen_bit_to_bool(zgaz3111);
    }
    bool zgaz3115;
    {
      fbits zgaz3114;
      {
        uint64_t zgaz3113;
        zgaz3113 = zc.zperms;
        zgaz3114 = (UINT64_C(1) & (zgaz3113 >> INT64_C(4)));
      }
      zgaz3115 = sailgen_bit_to_bool(zgaz3114);
    }
    bool zgaz3118;
    {
      fbits zgaz3117;
      {
        uint64_t zgaz3116;
        zgaz3116 = zc.zperms;
        zgaz3117 = (UINT64_C(1) & (zgaz3116 >> INT64_C(3)));
      }
      zgaz3118 = sailgen_bit_to_bool(zgaz3117);
    }
    bool zgaz3121;
    {
      fbits zgaz3120;
      {
        uint64_t zgaz3119;
        zgaz3119 = zc.zperms;
        zgaz3120 = (UINT64_C(1) & (zgaz3119 >> INT64_C(2)));
      }
      zgaz3121 = sailgen_bit_to_bool(zgaz3120);
    }
    bool zgaz3124;
    {
      fbits zgaz3123;
      {
        uint64_t zgaz3122;
        zgaz3122 = zc.zperms;
        zgaz3123 = (UINT64_C(1) & (zgaz3122 >> INT64_C(1)));
      }
      zgaz3124 = sailgen_bit_to_bool(zgaz3123);
    }
    bool zgaz3127;
    {
      fbits zgaz3126;
      {
        uint64_t zgaz3125;
        zgaz3125 = zc.zperms;
        zgaz3126 = (UINT64_C(1) & (zgaz3125 >> INT64_C(0)));
      }
      zgaz3127 = sailgen_bit_to_bool(zgaz3126);
    }
    uint64_t zgaz3128;
    zgaz3128 = zc.zreserved;
    bool zgaz3131;
    {
      fbits zgaz3130;
      {
        uint64_t zgaz3129;
        zgaz3129 = zc.zflags;
        zgaz3130 = (UINT64_C(1) & (zgaz3129 >> INT64_C(0)));
      }
      zgaz3131 = sailgen_bit_to_bool(zgaz3130);
    }
    uint64_t zgaz3132;
    zgaz3132 = (zTtop2 << 12) | zT;
    uint64_t zgaz3133;
    zgaz3133 = zc.zotype;
    uint64_t zgaz3134;
    zgaz3134 = zc.zaddress;
    struct zCapability zghz347;
    zghz347.zB = zBs;
    zghz347.zE = zE;
    zghz347.zT = zgaz3132;
    zghz347.zaccess_system_regs = zgaz397;
    zghz347.zaddress = zgaz3134;
    zghz347.zflag_cap_mode = zgaz3131;
    zghz347.zglobal = zgaz3127;
    zghz347.zinternal_E = zinternal_E;
    zghz347.zotype = zgaz3133;
    zghz347.zpermit_cinvoke = zgaz3103;
    zghz347.zpermit_execute = zgaz3124;
    zghz347.zpermit_load = zgaz3121;
    zghz347.zpermit_load_cap = zgaz3115;
    zghz347.zpermit_seal = zgaz3106;
    zghz347.zpermit_set_CID = zgaz394;
    zghz347.zpermit_store = zgaz3118;
    zghz347.zpermit_store_cap = zgaz3112;
    zghz347.zpermit_store_local_cap = zgaz3109;
    zghz347.zpermit_unseal = zgaz3100;
    zghz347.zreserved = zgaz3128;
    zghz347.ztag = zt;
    zghz347.zuperms = zgaz391;
    zghz346 = zghz347;
  }
  goto cleanup_92;
  /* unreachable after return */







  goto end_cleanup_93;
cleanup_92: ;







  goto end_function_91;
end_cleanup_93: ;
end_function_91: ;
  return zghz346;
end_block_exception_94: ;
  struct zCapability zcbz350 = { .zB = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zaddress = UINT64_C(0xdeadc0de), .zflag_cap_mode = false, .zglobal = false, .zinternal_E = false, .zotype = UINT64_C(0xdeadc0de), .zpermit_cinvoke = false, .zpermit_execute = false, .zpermit_load = false, .zpermit_load_cap = false, .zpermit_seal = false, .zpermit_set_CID = false, .zpermit_store = false, .zpermit_store_cap = false, .zpermit_store_local_cap = false, .zpermit_unseal = false, .zreserved = UINT64_C(0xdeadc0de), .ztag = false, .zuperms = UINT64_C(0xdeadc0de) };
  return zcbz350;
}



static void finish_sailgen_encCapabilityToCapability(void)
{
  KILL(sail_int)(&zghz377);
  KILL(sail_int)(&zghz376);
  KILL(sail_int)(&zghz375);
  KILL(sail_int)(&zghz374);
  KILL(sail_int)(&zghz373);
  KILL(sail_int)(&zghz372);
  KILL(sail_int)(&zghz371);
  KILL(sail_int)(&zghz370);
  KILL(sail_int)(&zghz369);
  KILL(lbits)(&zghz368);
  KILL(sail_int)(&zghz367);
  KILL(sail_int)(&zghz366);
  KILL(sail_int)(&zghz365);
  KILL(sail_int)(&zghz364);
  KILL(lbits)(&zghz363);
  KILL(sail_int)(&zghz362);
  KILL(sail_int)(&zghz361);
  KILL(sail_int)(&zghz360);
  KILL(sail_int)(&zghz359);
  KILL(lbits)(&zghz358);
  KILL(lbits)(&zghz357);
  KILL(sail_int)(&zghz356);
  KILL(sail_int)(&zghz355);
  KILL(sail_int)(&zghz354);
  KILL(sail_int)(&zghz353);
  KILL(sail_int)(&zghz352);
  KILL(sail_int)(&zghz351);
  KILL(sail_int)(&zghz350);
  KILL(sail_int)(&zghz349);
  KILL(sail_int)(&zghz348);


}

static struct zCapability sailgen_capBitsToCapability(bool, lbits);

struct zCapability zghz378;
struct zEncCapability zghz379;

static void startup_sailgen_capBitsToCapability(void)
{


}

static struct zCapability sailgen_capBitsToCapability(bool zt, lbits zc)
{
  __label__ end_function_96, end_block_exception_97;

  struct zCapability zghz378;
  struct zEncCapability zghz379;
  zghz379 = sailgen_capBitsToEncCapability(zc);
  zghz378 = sailgen_encCapabilityToCapability(zt, zghz379);
end_function_96: ;
  return zghz378;
end_block_exception_97: ;
  struct zCapability zcbz351 = { .zB = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zaddress = UINT64_C(0xdeadc0de), .zflag_cap_mode = false, .zglobal = false, .zinternal_E = false, .zotype = UINT64_C(0xdeadc0de), .zpermit_cinvoke = false, .zpermit_execute = false, .zpermit_load = false, .zpermit_load_cap = false, .zpermit_seal = false, .zpermit_set_CID = false, .zpermit_store = false, .zpermit_store_cap = false, .zpermit_store_local_cap = false, .zpermit_unseal = false, .zreserved = UINT64_C(0xdeadc0de), .ztag = false, .zuperms = UINT64_C(0xdeadc0de) };
  return zcbz351;
}



static void finish_sailgen_capBitsToCapability(void)
{


}

static struct zEncCapability sailgen_capToEncCap(struct zCapability);

struct zEncCapability zghz380;
struct zEncCapability zghz381;
sail_int zghz382;
sail_int zghz383;
sail_int zghz384;
sail_int zghz385;
sail_int zghz386;
sail_int zghz387;
sail_int zghz388;
sail_int zghz389;
sail_int zghz390;
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

static void startup_sailgen_capToEncCap(void)
{


  CREATE(sail_int)(&zghz382);
  CREATE(sail_int)(&zghz383);
  CREATE(sail_int)(&zghz384);
  CREATE(sail_int)(&zghz385);
  CREATE(sail_int)(&zghz386);
  CREATE(sail_int)(&zghz387);
  CREATE(sail_int)(&zghz388);
  CREATE(sail_int)(&zghz389);
  CREATE(sail_int)(&zghz390);
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
}

static struct zEncCapability sailgen_capToEncCap(struct zCapability zcap)
{
  __label__ cleanup_100, end_cleanup_101, end_function_99, end_block_exception_102;

  struct zEncCapability zghz380;
  uint64_t zt_hi;
  {
    uint64_t zgaz3171;
    zgaz3171 = zcap.zT;
    int64_t zgaz3172;
    {
      RECREATE(sail_int)(&zghz3100);
      CONVERT_OF(sail_int, mach_int)(&zghz3100, zcap_mantissa_width);
      RECREATE(sail_int)(&zghz3101);
      CONVERT_OF(sail_int, mach_int)(&zghz3101, INT64_C(3));
      RECREATE(sail_int)(&zghz3102);
      sub_int(&zghz3102, zghz3100, zghz3101);
      zgaz3172 = CONVERT_OF(mach_int, sail_int)(zghz3102);
    }
    zt_hi = (safe_rshift(UINT64_MAX, 64 - 9) & (zgaz3171 >> zinternal_E_take_bits));
  }
  uint64_t zt_lo;
  {
    uint64_t zgaz3169;
    zgaz3169 = zcap.zT;
    int64_t zgaz3170;
    {
      RECREATE(sail_int)(&zghz397);
      CONVERT_OF(sail_int, mach_int)(&zghz397, zinternal_E_take_bits);
      RECREATE(sail_int)(&zghz398);
      CONVERT_OF(sail_int, mach_int)(&zghz398, INT64_C(1));
      RECREATE(sail_int)(&zghz399);
      sub_int(&zghz399, zghz397, zghz398);
      zgaz3170 = CONVERT_OF(mach_int, sail_int)(zghz399);
    }
    zt_lo = (safe_rshift(UINT64_MAX, 64 - 3) & (zgaz3169 >> INT64_C(0)));
  }
  uint64_t zb_hi;
  {
    uint64_t zgaz3167;
    zgaz3167 = zcap.zB;
    int64_t zgaz3168;
    {
      RECREATE(sail_int)(&zghz394);
      CONVERT_OF(sail_int, mach_int)(&zghz394, zcap_mantissa_width);
      RECREATE(sail_int)(&zghz395);
      CONVERT_OF(sail_int, mach_int)(&zghz395, INT64_C(1));
      RECREATE(sail_int)(&zghz396);
      sub_int(&zghz396, zghz394, zghz395);
      zgaz3168 = CONVERT_OF(mach_int, sail_int)(zghz396);
    }
    zb_hi = (safe_rshift(UINT64_MAX, 64 - 11) & (zgaz3167 >> zinternal_E_take_bits));
  }
  uint64_t zb_lo;
  {
    uint64_t zgaz3165;
    zgaz3165 = zcap.zB;
    int64_t zgaz3166;
    {
      RECREATE(sail_int)(&zghz391);
      CONVERT_OF(sail_int, mach_int)(&zghz391, zinternal_E_take_bits);
      RECREATE(sail_int)(&zghz392);
      CONVERT_OF(sail_int, mach_int)(&zghz392, INT64_C(1));
      RECREATE(sail_int)(&zghz393);
      sub_int(&zghz393, zghz391, zghz392);
      zgaz3166 = CONVERT_OF(mach_int, sail_int)(zghz393);
    }
    zb_lo = (safe_rshift(UINT64_MAX, 64 - 3) & (zgaz3165 >> INT64_C(0)));
  }
  {
    bool zgaz3145;
    zgaz3145 = zcap.zinternal_E;
    unit zgsz344;
    if (zgaz3145) {
      {
        uint64_t zgaz3147;
        zgaz3147 = zcap.zE;
        int64_t zgaz3148;
        {
          int64_t zgaz3146;
          {
            RECREATE(sail_int)(&zghz388);
            CONVERT_OF(sail_int, mach_int)(&zghz388, INT64_C(2));
            RECREATE(sail_int)(&zghz389);
            CONVERT_OF(sail_int, mach_int)(&zghz389, zinternal_E_take_bits);
            RECREATE(sail_int)(&zghz390);
            mult_int(&zghz390, zghz388, zghz389);
            zgaz3146 = CONVERT_OF(mach_int, sail_int)(zghz390);
          }
          {
            RECREATE(sail_int)(&zghz385);
            CONVERT_OF(sail_int, mach_int)(&zghz385, zgaz3146);
            RECREATE(sail_int)(&zghz386);
            CONVERT_OF(sail_int, mach_int)(&zghz386, INT64_C(1));
            RECREATE(sail_int)(&zghz387);
            sub_int(&zghz387, zghz385, zghz386);
            zgaz3148 = CONVERT_OF(mach_int, sail_int)(zghz387);
          }
        }
        zt_lo = (safe_rshift(UINT64_MAX, 64 - 3) & (zgaz3147 >> zinternal_E_take_bits));
        unit zgsz345;
        zgsz345 = UNIT;
      }
      uint64_t zgaz3149;
      zgaz3149 = zcap.zE;
      int64_t zgaz3150;
      {
        RECREATE(sail_int)(&zghz382);
        CONVERT_OF(sail_int, mach_int)(&zghz382, zinternal_E_take_bits);
        RECREATE(sail_int)(&zghz383);
        CONVERT_OF(sail_int, mach_int)(&zghz383, INT64_C(1));
        RECREATE(sail_int)(&zghz384);
        sub_int(&zghz384, zghz382, zghz383);
        zgaz3150 = CONVERT_OF(mach_int, sail_int)(zghz384);
      }
      zb_lo = (safe_rshift(UINT64_MAX, 64 - 3) & (zgaz3149 >> INT64_C(0)));
      zgsz344 = UNIT;
    } else {  zgsz344 = UNIT;  }
  }
  {
    uint64_t zgaz3153;
    {
      uint64_t zgaz3151;
      zgaz3151 = zcap.zuperms;
      uint64_t zgaz3152;
      zgaz3152 = sailgen_getCapHardPerms(zcap);
      zgaz3153 = (zgaz3151 << 12) | zgaz3152;
    }
    uint64_t zgaz3154;
    zgaz3154 = zcap.zreserved;
    uint64_t zgaz3156;
    {
      bool zgaz3155;
      zgaz3155 = zcap.zflag_cap_mode;
      zgaz3156 = sailgen_bool_to_bits(zgaz3155);
    }
    uint64_t zgaz3157;
    zgaz3157 = zcap.zotype;
    fbits zgaz3160;
    {
      uint64_t zgaz3159;
      {
        bool zgaz3158;
        zgaz3158 = zcap.zinternal_E;
        zgaz3159 = sailgen_bool_to_bits(zgaz3158);
      }
      zgaz3160 = (UINT64_C(1) & (zgaz3159 >> INT64_C(0)));
    }
    uint64_t zgaz3161;
    zgaz3161 = (zt_hi << 3) | zt_lo;
    uint64_t zgaz3162;
    zgaz3162 = (zb_hi << 3) | zb_lo;
    uint64_t zgaz3163;
    zgaz3163 = zcap.zaddress;
    struct zEncCapability zghz381;
    zghz381.zB = zgaz3162;
    zghz381.zT = zgaz3161;
    zghz381.zaddress = zgaz3163;
    zghz381.zflags = zgaz3156;
    zghz381.zinternal_E = zgaz3160;
    zghz381.zotype = zgaz3157;
    zghz381.zperms = zgaz3153;
    zghz381.zreserved = zgaz3154;
    zghz380 = zghz381;
  }
  goto cleanup_100;
  /* unreachable after return */




  goto end_cleanup_101;
cleanup_100: ;




  goto end_function_99;
end_cleanup_101: ;
end_function_99: ;
  return zghz380;
end_block_exception_102: ;
  struct zEncCapability zcbz352 = { .zB = UINT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaddress = UINT64_C(0xdeadc0de), .zflags = UINT64_C(0xdeadc0de), .zinternal_E = UINT64_C(0), .zotype = UINT64_C(0xdeadc0de), .zperms = UINT64_C(0xdeadc0de), .zreserved = UINT64_C(0xdeadc0de) };
  return zcbz352;
}



static void finish_sailgen_capToEncCap(void)
{
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
  KILL(sail_int)(&zghz390);
  KILL(sail_int)(&zghz389);
  KILL(sail_int)(&zghz388);
  KILL(sail_int)(&zghz387);
  KILL(sail_int)(&zghz386);
  KILL(sail_int)(&zghz385);
  KILL(sail_int)(&zghz384);
  KILL(sail_int)(&zghz383);
  KILL(sail_int)(&zghz382);


}

static void sailgen_capToBits(lbits *rop, struct zCapability);

struct zEncCapability zghz3103;

static void startup_sailgen_capToBits(void)
{
}

static void sailgen_capToBits(lbits *zcbz323, struct zCapability zcap)
{
  __label__ end_function_104, end_block_exception_105, end_function_206;

  struct zEncCapability zghz3103;
  zghz3103 = sailgen_capToEncCap(zcap);
  sailgen_encCapToBits((*(&zcbz323)), zghz3103);
end_function_104: ;
  goto end_function_206;
end_block_exception_105: ;
  goto end_function_206;
end_function_206: ;
}



static void finish_sailgen_capToBits(void)
{
}

static lbits znull_cap_bits;
static void create_letbind_27(void) {    CREATE(lbits)(&znull_cap_bits);


  lbits zgsz347;
  CREATE(lbits)(&zgsz347);
  sailgen_capToBits(&zgsz347, znull_cap);
  COPY(lbits)(&znull_cap_bits, zgsz347);
  KILL(lbits)(&zgsz347);
let_end_106: ;
}
static void kill_letbind_27(void) {    KILL(lbits)(&znull_cap_bits);
}

static void sailgen_capToMemBits(lbits *rop, struct zCapability);

lbits zghz3104;

static void startup_sailgen_capToMemBits(void)
{    CREATE(lbits)(&zghz3104);
}

static void sailgen_capToMemBits(lbits *zcbz324, struct zCapability zcap)
{
  __label__ end_function_108, end_block_exception_109, end_function_205;

  RECREATE(lbits)(&zghz3104);
  sailgen_capToBits(&zghz3104, zcap);
  xor_bits((*(&zcbz324)), zghz3104, znull_cap_bits);
end_function_108: ;
  goto end_function_205;
end_block_exception_109: ;
  goto end_function_205;
end_function_205: ;
}



static void finish_sailgen_capToMemBits(void)
{    KILL(lbits)(&zghz3104);
}

static struct zCapability sailgen_memBitsToCapability(bool, lbits);

struct zCapability zghz3105;
lbits zghz3106;

static void startup_sailgen_memBitsToCapability(void)
{

  CREATE(lbits)(&zghz3106);
}

static struct zCapability sailgen_memBitsToCapability(bool ztag, lbits zb)
{
  __label__ end_function_111, end_block_exception_112;

  struct zCapability zghz3105;
  RECREATE(lbits)(&zghz3106);
  xor_bits(&zghz3106, zb, znull_cap_bits);
  zghz3105 = sailgen_capBitsToCapability(ztag, zghz3106);
end_function_111: ;
  return zghz3105;
end_block_exception_112: ;
  struct zCapability zcbz353 = { .zB = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zaddress = UINT64_C(0xdeadc0de), .zflag_cap_mode = false, .zglobal = false, .zinternal_E = false, .zotype = UINT64_C(0xdeadc0de), .zpermit_cinvoke = false, .zpermit_execute = false, .zpermit_load = false, .zpermit_load_cap = false, .zpermit_seal = false, .zpermit_set_CID = false, .zpermit_store = false, .zpermit_store_cap = false, .zpermit_store_local_cap = false, .zpermit_unseal = false, .zreserved = UINT64_C(0xdeadc0de), .ztag = false, .zuperms = UINT64_C(0xdeadc0de) };
  return zcbz353;
}



static void finish_sailgen_memBitsToCapability(void)
{
  KILL(lbits)(&zghz3106);

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

static void CREATE(ztuple_z8z5bv64zCz0z5bvz9)(struct ztuple_z8z5bv64zCz0z5bvz9 *op) {
  CREATE(lbits)(&op->ztup1);
}

static void RECREATE(ztuple_z8z5bv64zCz0z5bvz9)(struct ztuple_z8z5bv64zCz0z5bvz9 *op) {
  RECREATE(lbits)(&op->ztup1);
}

static void KILL(ztuple_z8z5bv64zCz0z5bvz9)(struct ztuple_z8z5bv64zCz0z5bvz9 *op) {
  KILL(lbits)(&op->ztup1);
}

static bool EQUAL(ztuple_z8z5bv64zCz0z5bvz9)(struct ztuple_z8z5bv64zCz0z5bvz9 op1, struct ztuple_z8z5bv64zCz0z5bvz9 op2) {
  return EQUAL(fbits)(op1.ztup0, op2.ztup0) && EQUAL(lbits)(op1.ztup1, op2.ztup1);
}

static void sailgen_getCapBoundsBits(struct ztuple_z8z5bv64zCz0z5bvz9 *rop, struct zCapability);

lbits zghz3107;
lbits zghz3108;
sail_int zghz3109;
sail_int zghz3110;
lbits zghz3111;
sail_int zghz3112;
sail_int zghz3113;
sail_int zghz3114;
sail_int zghz3115;
sail_int zghz3116;
lbits zghz3117;
sail_int zghz3118;
sail_int zghz3119;
sail_int zghz3120;
sail_int zghz3121;
sail_int zghz3122;
sail_int zghz3123;
lbits zghz3124;
sail_int zghz3125;
sail_int zghz3126;
sail_int zghz3127;
sail_int zghz3128;
sail_int zghz3129;
sail_int zghz3130;
sail_int zghz3131;
lbits zghz3132;
sail_int zghz3133;
lbits zghz3134;
lbits zghz3135;
lbits zghz3136;
lbits zghz3137;
sail_int zghz3138;
lbits zghz3139;
lbits zghz3140;
sail_int zghz3141;
lbits zghz3142;
lbits zghz3143;
sail_int zghz3144;
lbits zghz3145;
lbits zghz3146;
lbits zghz3147;
lbits zghz3148;
sail_int zghz3149;
lbits zghz3150;
lbits zghz3151;
sail_int zghz3152;
lbits zghz3153;
lbits zghz3154;
sail_int zghz3155;
lbits zghz3156;
sail_int zghz3157;
sail_int zghz3158;
sail_int zghz3159;
sail_int zghz3160;
sail_int zghz3161;
sail_int zghz3162;
lbits zghz3163;
lbits zghz3164;
lbits zghz3165;
lbits zghz3166;
lbits zghz3167;
lbits zghz3168;
lbits zghz3169;
sail_int zghz3170;
lbits zghz3171;
lbits zghz3172;
sail_int zghz3173;
lbits zghz3174;
lbits zghz3175;
sail_int zghz3176;
lbits zghz3177;
lbits zghz3178;
sail_int zghz3179;
lbits zghz3180;
sail_int zghz3181;
sail_int zghz3182;
sail_int zghz3183;
sail_int zghz3184;
sail_int zghz3185;
sail_int zghz3186;
lbits zghz3187;
sail_int zghz3188;

static void startup_sailgen_getCapBoundsBits(void)
{
  CREATE(lbits)(&zghz3107);
  CREATE(lbits)(&zghz3108);
  CREATE(sail_int)(&zghz3109);
  CREATE(sail_int)(&zghz3110);
  CREATE(lbits)(&zghz3111);
  CREATE(sail_int)(&zghz3112);
  CREATE(sail_int)(&zghz3113);
  CREATE(sail_int)(&zghz3114);
  CREATE(sail_int)(&zghz3115);
  CREATE(sail_int)(&zghz3116);
  CREATE(lbits)(&zghz3117);
  CREATE(sail_int)(&zghz3118);
  CREATE(sail_int)(&zghz3119);
  CREATE(sail_int)(&zghz3120);
  CREATE(sail_int)(&zghz3121);
  CREATE(sail_int)(&zghz3122);
  CREATE(sail_int)(&zghz3123);
  CREATE(lbits)(&zghz3124);
  CREATE(sail_int)(&zghz3125);
  CREATE(sail_int)(&zghz3126);
  CREATE(sail_int)(&zghz3127);
  CREATE(sail_int)(&zghz3128);
  CREATE(sail_int)(&zghz3129);
  CREATE(sail_int)(&zghz3130);
  CREATE(sail_int)(&zghz3131);
  CREATE(lbits)(&zghz3132);
  CREATE(sail_int)(&zghz3133);
  CREATE(lbits)(&zghz3134);
  CREATE(lbits)(&zghz3135);
  CREATE(lbits)(&zghz3136);
  CREATE(lbits)(&zghz3137);
  CREATE(sail_int)(&zghz3138);
  CREATE(lbits)(&zghz3139);
  CREATE(lbits)(&zghz3140);
  CREATE(sail_int)(&zghz3141);
  CREATE(lbits)(&zghz3142);
  CREATE(lbits)(&zghz3143);
  CREATE(sail_int)(&zghz3144);
  CREATE(lbits)(&zghz3145);
  CREATE(lbits)(&zghz3146);
  CREATE(lbits)(&zghz3147);
  CREATE(lbits)(&zghz3148);
  CREATE(sail_int)(&zghz3149);
  CREATE(lbits)(&zghz3150);
  CREATE(lbits)(&zghz3151);
  CREATE(sail_int)(&zghz3152);
  CREATE(lbits)(&zghz3153);
  CREATE(lbits)(&zghz3154);
  CREATE(sail_int)(&zghz3155);
  CREATE(lbits)(&zghz3156);
  CREATE(sail_int)(&zghz3157);
  CREATE(sail_int)(&zghz3158);
  CREATE(sail_int)(&zghz3159);
  CREATE(sail_int)(&zghz3160);
  CREATE(sail_int)(&zghz3161);
  CREATE(sail_int)(&zghz3162);
  CREATE(lbits)(&zghz3163);
  CREATE(lbits)(&zghz3164);
  CREATE(lbits)(&zghz3165);
  CREATE(lbits)(&zghz3166);
  CREATE(lbits)(&zghz3167);
  CREATE(lbits)(&zghz3168);
  CREATE(lbits)(&zghz3169);
  CREATE(sail_int)(&zghz3170);
  CREATE(lbits)(&zghz3171);
  CREATE(lbits)(&zghz3172);
  CREATE(sail_int)(&zghz3173);
  CREATE(lbits)(&zghz3174);
  CREATE(lbits)(&zghz3175);
  CREATE(sail_int)(&zghz3176);
  CREATE(lbits)(&zghz3177);
  CREATE(lbits)(&zghz3178);
  CREATE(sail_int)(&zghz3179);
  CREATE(lbits)(&zghz3180);
  CREATE(sail_int)(&zghz3181);
  CREATE(sail_int)(&zghz3182);
  CREATE(sail_int)(&zghz3183);
  CREATE(sail_int)(&zghz3184);
  CREATE(sail_int)(&zghz3185);
  CREATE(sail_int)(&zghz3186);
  CREATE(lbits)(&zghz3187);
  CREATE(sail_int)(&zghz3188);
}

static void sailgen_getCapBoundsBits(struct ztuple_z8z5bv64zCz0z5bvz9 *zcbz326, struct zCapability zc)
{
  __label__ end_function_114, end_block_exception_115, end_function_204;

  int64_t zE;
  {
    int64_t zgaz3209;
    {
      uint64_t zgaz3208;
      zgaz3208 = zc.zE;
      {
        RECREATE(lbits)(&zghz3187);
        CONVERT_OF(lbits, fbits)(&zghz3187, zgaz3208, UINT64_C(6) , true);
        RECREATE(sail_int)(&zghz3188);
        sail_unsigned(&zghz3188, zghz3187);
        zgaz3209 = CONVERT_OF(mach_int, sail_int)(zghz3188);
      }
    }
    {
      RECREATE(sail_int)(&zghz3184);
      CONVERT_OF(sail_int, mach_int)(&zghz3184, zcap_max_E);
      RECREATE(sail_int)(&zghz3185);
      CONVERT_OF(sail_int, mach_int)(&zghz3185, zgaz3209);
      RECREATE(sail_int)(&zghz3186);
      min_int(&zghz3186, zghz3184, zghz3185);
      zE = CONVERT_OF(mach_int, sail_int)(zghz3186);
    }
  }
  uint64_t za;
  za = zc.zaddress;
  uint64_t za3;
  {
    uint64_t zgaz3207;
    {
      int64_t zgaz3206;
      {
        int64_t zgaz3205;
        zgaz3205 = (zE + zcap_mantissa_width);
        {
          RECREATE(sail_int)(&zghz3181);
          CONVERT_OF(sail_int, mach_int)(&zghz3181, zgaz3205);
          RECREATE(sail_int)(&zghz3182);
          CONVERT_OF(sail_int, mach_int)(&zghz3182, INT64_C(3));
          RECREATE(sail_int)(&zghz3183);
          sub_int(&zghz3183, zghz3181, zghz3182);
          zgaz3206 = CONVERT_OF(mach_int, sail_int)(zghz3183);
        }
      }
      {
        RECREATE(lbits)(&zghz3178);
        CONVERT_OF(lbits, fbits)(&zghz3178, za, UINT64_C(64) , true);
        RECREATE(sail_int)(&zghz3179);
        CONVERT_OF(sail_int, mach_int)(&zghz3179, zgaz3206);
        RECREATE(lbits)(&zghz3180);
        shiftr(&zghz3180, zghz3178, zghz3179);
        zgaz3207 = CONVERT_OF(fbits, lbits)(zghz3180, true);
      }
    }
    {
      RECREATE(lbits)(&zghz3175);
      CONVERT_OF(lbits, fbits)(&zghz3175, zgaz3207, UINT64_C(64) , true);
      RECREATE(sail_int)(&zghz3176);
      CONVERT_OF(sail_int, mach_int)(&zghz3176, INT64_C(3));
      RECREATE(lbits)(&zghz3177);
      sail_truncate(&zghz3177, zghz3175, zghz3176);
      za3 = CONVERT_OF(fbits, lbits)(zghz3177, true);
    }
  }
  uint64_t zB3;
  {
    uint64_t zgaz3204;
    zgaz3204 = zc.zB;
    {
      RECREATE(lbits)(&zghz3172);
      CONVERT_OF(lbits, fbits)(&zghz3172, zgaz3204, UINT64_C(14) , true);
      RECREATE(sail_int)(&zghz3173);
      CONVERT_OF(sail_int, mach_int)(&zghz3173, INT64_C(3));
      RECREATE(lbits)(&zghz3174);
      sail_truncateLSB(&zghz3174, zghz3172, zghz3173);
      zB3 = CONVERT_OF(fbits, lbits)(zghz3174, true);
    }
  }
  uint64_t zT3;
  {
    uint64_t zgaz3203;
    zgaz3203 = zc.zT;
    {
      RECREATE(lbits)(&zghz3169);
      CONVERT_OF(lbits, fbits)(&zghz3169, zgaz3203, UINT64_C(14) , true);
      RECREATE(sail_int)(&zghz3170);
      CONVERT_OF(sail_int, mach_int)(&zghz3170, INT64_C(3));
      RECREATE(lbits)(&zghz3171);
      sail_truncateLSB(&zghz3171, zghz3169, zghz3170);
      zT3 = CONVERT_OF(fbits, lbits)(zghz3171, true);
    }
  }
  uint64_t zR3;
  zR3 = ((zB3 - UINT64_C(0b001)) & UINT64_C(0b111));
  int64_t zaHi;
  {
    bool zgaz3202;
    {
      RECREATE(lbits)(&zghz3167);
      CONVERT_OF(lbits, fbits)(&zghz3167, za3, UINT64_C(3) , true);
      RECREATE(lbits)(&zghz3168);
      CONVERT_OF(lbits, fbits)(&zghz3168, zR3, UINT64_C(3) , true);
      zgaz3202 = sailgen_z8operatorz0zI_uz9(zghz3167, zghz3168);
    }
    if (zgaz3202) {  zaHi = INT64_C(1);  } else {  zaHi = INT64_C(0);  }
  }
  int64_t zbHi;
  {
    bool zgaz3201;
    {
      RECREATE(lbits)(&zghz3165);
      CONVERT_OF(lbits, fbits)(&zghz3165, zB3, UINT64_C(3) , true);
      RECREATE(lbits)(&zghz3166);
      CONVERT_OF(lbits, fbits)(&zghz3166, zR3, UINT64_C(3) , true);
      zgaz3201 = sailgen_z8operatorz0zI_uz9(zghz3165, zghz3166);
    }
    if (zgaz3201) {  zbHi = INT64_C(1);  } else {  zbHi = INT64_C(0);  }
  }
  int64_t ztHi;
  {
    bool zgaz3200;
    {
      RECREATE(lbits)(&zghz3163);
      CONVERT_OF(lbits, fbits)(&zghz3163, zT3, UINT64_C(3) , true);
      RECREATE(lbits)(&zghz3164);
      CONVERT_OF(lbits, fbits)(&zghz3164, zR3, UINT64_C(3) , true);
      zgaz3200 = sailgen_z8operatorz0zI_uz9(zghz3163, zghz3164);
    }
    if (zgaz3200) {  ztHi = INT64_C(1);  } else {  ztHi = INT64_C(0);  }
  }
  int64_t zcorrection_base;
  {
    RECREATE(sail_int)(&zghz3160);
    CONVERT_OF(sail_int, mach_int)(&zghz3160, zbHi);
    RECREATE(sail_int)(&zghz3161);
    CONVERT_OF(sail_int, mach_int)(&zghz3161, zaHi);
    RECREATE(sail_int)(&zghz3162);
    sub_int(&zghz3162, zghz3160, zghz3161);
    zcorrection_base = CONVERT_OF(mach_int, sail_int)(zghz3162);
  }
  int64_t zcorrection_top;
  {
    RECREATE(sail_int)(&zghz3157);
    CONVERT_OF(sail_int, mach_int)(&zghz3157, ztHi);
    RECREATE(sail_int)(&zghz3158);
    CONVERT_OF(sail_int, mach_int)(&zghz3158, zaHi);
    RECREATE(sail_int)(&zghz3159);
    sub_int(&zghz3159, zghz3157, zghz3158);
    zcorrection_top = CONVERT_OF(mach_int, sail_int)(zghz3159);
  }
  uint64_t za_top;
  {
    int64_t zgaz3199;
    zgaz3199 = (zE + zcap_mantissa_width);
    {
      RECREATE(lbits)(&zghz3154);
      CONVERT_OF(lbits, fbits)(&zghz3154, za, UINT64_C(64) , true);
      RECREATE(sail_int)(&zghz3155);
      CONVERT_OF(sail_int, mach_int)(&zghz3155, zgaz3199);
      RECREATE(lbits)(&zghz3156);
      shiftr(&zghz3156, zghz3154, zghz3155);
      za_top = CONVERT_OF(fbits, lbits)(zghz3156, true);
    }
  }
  RECREATE(lbits)(&zghz3107);
  {
    RECREATE(lbits)(&zghz3143);
    {
      uint64_t zgaz3196;
      {
        RECREATE(lbits)(&zghz3151);
        CONVERT_OF(lbits, fbits)(&zghz3151, za_top, UINT64_C(64) , true);
        RECREATE(sail_int)(&zghz3152);
        CONVERT_OF(sail_int, mach_int)(&zghz3152, zcorrection_base);
        RECREATE(lbits)(&zghz3153);
        add_bits_int(&zghz3153, zghz3151, zghz3152);
        zgaz3196 = CONVERT_OF(fbits, lbits)(zghz3153, true);
      }
      RECREATE(lbits)(&zghz3145);
      {
        uint64_t zgaz3194;
        zgaz3194 = zc.zB;
        sbits zgaz3195;
        {
          RECREATE(sail_int)(&zghz3149);
          CONVERT_OF(sail_int, mach_int)(&zghz3149, zE);
          RECREATE(lbits)(&zghz3150);
          sailgen_zzeros_implicit(&zghz3150, zghz3149);
          zgaz3195 = CONVERT_OF(sbits, lbits)(zghz3150, true);
        }
        {
          RECREATE(lbits)(&zghz3147);
          CONVERT_OF(lbits, fbits)(&zghz3147, zgaz3194, UINT64_C(14) , true);
          RECREATE(lbits)(&zghz3148);
          CONVERT_OF(lbits, sbits)(&zghz3148, zgaz3195, true);
          append(&zghz3145, zghz3147, zghz3148);
        }
      }
      {
        RECREATE(lbits)(&zghz3146);
        CONVERT_OF(lbits, fbits)(&zghz3146, zgaz3196, UINT64_C(64) , true);
        append(&zghz3143, zghz3146, zghz3145);
      }
    }
    {
      RECREATE(sail_int)(&zghz3144);
      CONVERT_OF(sail_int, mach_int)(&zghz3144, zcap_len_width);
      sail_truncate(&zghz3107, zghz3143, zghz3144);
    }
  }
  RECREATE(lbits)(&zghz3108);
  {
    RECREATE(lbits)(&zghz3132);
    {
      uint64_t zgaz3191;
      {
        RECREATE(lbits)(&zghz3140);
        CONVERT_OF(lbits, fbits)(&zghz3140, za_top, UINT64_C(64) , true);
        RECREATE(sail_int)(&zghz3141);
        CONVERT_OF(sail_int, mach_int)(&zghz3141, zcorrection_top);
        RECREATE(lbits)(&zghz3142);
        add_bits_int(&zghz3142, zghz3140, zghz3141);
        zgaz3191 = CONVERT_OF(fbits, lbits)(zghz3142, true);
      }
      RECREATE(lbits)(&zghz3134);
      {
        uint64_t zgaz3189;
        zgaz3189 = zc.zT;
        sbits zgaz3190;
        {
          RECREATE(sail_int)(&zghz3138);
          CONVERT_OF(sail_int, mach_int)(&zghz3138, zE);
          RECREATE(lbits)(&zghz3139);
          sailgen_zzeros_implicit(&zghz3139, zghz3138);
          zgaz3190 = CONVERT_OF(sbits, lbits)(zghz3139, true);
        }
        {
          RECREATE(lbits)(&zghz3136);
          CONVERT_OF(lbits, fbits)(&zghz3136, zgaz3189, UINT64_C(14) , true);
          RECREATE(lbits)(&zghz3137);
          CONVERT_OF(lbits, sbits)(&zghz3137, zgaz3190, true);
          append(&zghz3134, zghz3136, zghz3137);
        }
      }
      {
        RECREATE(lbits)(&zghz3135);
        CONVERT_OF(lbits, fbits)(&zghz3135, zgaz3191, UINT64_C(64) , true);
        append(&zghz3132, zghz3135, zghz3134);
      }
    }
    {
      RECREATE(sail_int)(&zghz3133);
      CONVERT_OF(sail_int, mach_int)(&zghz3133, zcap_len_width);
      sail_truncate(&zghz3108, zghz3132, zghz3133);
    }
  }
  uint64_t zbase2;
  {
    uint64_t zgaz3188;
    {
      fbits zgaz3187;
      {
        int64_t zgaz3186;
        {
          RECREATE(sail_int)(&zghz3129);
          CONVERT_OF(sail_int, mach_int)(&zghz3129, zcap_addr_width);
          RECREATE(sail_int)(&zghz3130);
          CONVERT_OF(sail_int, mach_int)(&zghz3130, INT64_C(1));
          RECREATE(sail_int)(&zghz3131);
          sub_int(&zghz3131, zghz3129, zghz3130);
          zgaz3186 = CONVERT_OF(mach_int, sail_int)(zghz3131);
        }
        {
          RECREATE(sail_int)(&zghz3128);
          CONVERT_OF(sail_int, mach_int)(&zghz3128, zgaz3186);
          zgaz3187 = bitvector_access(zghz3107, zghz3128);
        }
      }
      zgaz3188 = UINT64_C(0b0);
      zgaz3188 = update_fbits(zgaz3188, INT64_C(0), zgaz3187);
    }
    zbase2 = (UINT64_C(0b0) << 1) | zgaz3188;
  }
  uint64_t ztop2;
  {
    int64_t zgaz3185;
    {
      RECREATE(sail_int)(&zghz3125);
      CONVERT_OF(sail_int, mach_int)(&zghz3125, zcap_addr_width);
      RECREATE(sail_int)(&zghz3126);
      CONVERT_OF(sail_int, mach_int)(&zghz3126, INT64_C(1));
      RECREATE(sail_int)(&zghz3127);
      sub_int(&zghz3127, zghz3125, zghz3126);
      zgaz3185 = CONVERT_OF(mach_int, sail_int)(zghz3127);
    }
    {
      RECREATE(sail_int)(&zghz3122);
      CONVERT_OF(sail_int, mach_int)(&zghz3122, zcap_addr_width);
      RECREATE(sail_int)(&zghz3123);
      CONVERT_OF(sail_int, mach_int)(&zghz3123, zgaz3185);
      RECREATE(lbits)(&zghz3124);
      vector_subrange_lbits(&zghz3124, zghz3108, zghz3122, zghz3123);
      ztop2 = CONVERT_OF(fbits, lbits)(zghz3124, true);
    }
  }
  {
    bool zgaz3180;
    {
      bool zgaz3179;
      {
        int64_t zgaz3176;
        {
          RECREATE(sail_int)(&zghz3119);
          CONVERT_OF(sail_int, mach_int)(&zghz3119, zcap_max_E);
          RECREATE(sail_int)(&zghz3120);
          CONVERT_OF(sail_int, mach_int)(&zghz3120, INT64_C(1));
          RECREATE(sail_int)(&zghz3121);
          sub_int(&zghz3121, zghz3119, zghz3120);
          zgaz3176 = CONVERT_OF(mach_int, sail_int)(zghz3121);
        }
        zgaz3179 = (zE < zgaz3176);
      }
      bool zgsz349;
      if (zgaz3179) {
        int64_t zgaz3178;
        {
          uint64_t zgaz3177;
          zgaz3177 = ((ztop2 - zbase2) & UINT64_C(0b11));
          {
            RECREATE(lbits)(&zghz3117);
            CONVERT_OF(lbits, fbits)(&zghz3117, zgaz3177, UINT64_C(2) , true);
            RECREATE(sail_int)(&zghz3118);
            sail_unsigned(&zghz3118, zghz3117);
            zgaz3178 = CONVERT_OF(mach_int, sail_int)(zghz3118);
          }
        }
        zgsz349 = (zgaz3178 > INT64_C(1));
      } else {  zgsz349 = false;  }
      zgaz3180 = zgsz349;
    }
    unit zgsz350;
    if (zgaz3180) {
      fbits zgaz3182;
      {
        fbits zgaz3181;
        {
          RECREATE(sail_int)(&zghz3116);
          CONVERT_OF(sail_int, mach_int)(&zghz3116, zcap_addr_width);
          zgaz3181 = bitvector_access(zghz3108, zghz3116);
        }
        zgaz3182 = sailgen_not_bit(zgaz3181);
      }
      {
        RECREATE(sail_int)(&zghz3115);
        CONVERT_OF(sail_int, mach_int)(&zghz3115, zcap_addr_width);
        update_lbits(&zghz3108, zghz3108, zghz3115, zgaz3182);
      }
      zgsz350 = UNIT;
    } else {  zgsz350 = UNIT;  }
  }
  uint64_t zgaz3184;
  {
    int64_t zgaz3183;
    {
      RECREATE(sail_int)(&zghz3112);
      CONVERT_OF(sail_int, mach_int)(&zghz3112, zcap_addr_width);
      RECREATE(sail_int)(&zghz3113);
      CONVERT_OF(sail_int, mach_int)(&zghz3113, INT64_C(1));
      RECREATE(sail_int)(&zghz3114);
      sub_int(&zghz3114, zghz3112, zghz3113);
      zgaz3183 = CONVERT_OF(mach_int, sail_int)(zghz3114);
    }
    {
      RECREATE(sail_int)(&zghz3109);
      CONVERT_OF(sail_int, mach_int)(&zghz3109, zgaz3183);
      RECREATE(sail_int)(&zghz3110);
      CONVERT_OF(sail_int, mach_int)(&zghz3110, INT64_C(0));
      RECREATE(lbits)(&zghz3111);
      vector_subrange_lbits(&zghz3111, zghz3107, zghz3109, zghz3110);
      zgaz3184 = CONVERT_OF(fbits, lbits)(zghz3111, true);
    }
  }
  struct ztuple_z8z5bv64zCz0z5bvz9 zgsz351;
  CREATE(ztuple_z8z5bv64zCz0z5bvz9)(&zgsz351);
  zgsz351.ztup0 = zgaz3184;
  COPY(lbits)(&((&zgsz351)->ztup1), zghz3108);
  COPY(ztuple_z8z5bv64zCz0z5bvz9)((*(&zcbz326)), zgsz351);
  KILL(ztuple_z8z5bv64zCz0z5bvz9)(&zgsz351);















end_function_114: ;
  goto end_function_204;
end_block_exception_115: ;
  goto end_function_204;
end_function_204: ;
}



static void finish_sailgen_getCapBoundsBits(void)
{
  KILL(sail_int)(&zghz3188);
  KILL(lbits)(&zghz3187);
  KILL(sail_int)(&zghz3186);
  KILL(sail_int)(&zghz3185);
  KILL(sail_int)(&zghz3184);
  KILL(sail_int)(&zghz3183);
  KILL(sail_int)(&zghz3182);
  KILL(sail_int)(&zghz3181);
  KILL(lbits)(&zghz3180);
  KILL(sail_int)(&zghz3179);
  KILL(lbits)(&zghz3178);
  KILL(lbits)(&zghz3177);
  KILL(sail_int)(&zghz3176);
  KILL(lbits)(&zghz3175);
  KILL(lbits)(&zghz3174);
  KILL(sail_int)(&zghz3173);
  KILL(lbits)(&zghz3172);
  KILL(lbits)(&zghz3171);
  KILL(sail_int)(&zghz3170);
  KILL(lbits)(&zghz3169);
  KILL(lbits)(&zghz3168);
  KILL(lbits)(&zghz3167);
  KILL(lbits)(&zghz3166);
  KILL(lbits)(&zghz3165);
  KILL(lbits)(&zghz3164);
  KILL(lbits)(&zghz3163);
  KILL(sail_int)(&zghz3162);
  KILL(sail_int)(&zghz3161);
  KILL(sail_int)(&zghz3160);
  KILL(sail_int)(&zghz3159);
  KILL(sail_int)(&zghz3158);
  KILL(sail_int)(&zghz3157);
  KILL(lbits)(&zghz3156);
  KILL(sail_int)(&zghz3155);
  KILL(lbits)(&zghz3154);
  KILL(lbits)(&zghz3153);
  KILL(sail_int)(&zghz3152);
  KILL(lbits)(&zghz3151);
  KILL(lbits)(&zghz3150);
  KILL(sail_int)(&zghz3149);
  KILL(lbits)(&zghz3148);
  KILL(lbits)(&zghz3147);
  KILL(lbits)(&zghz3146);
  KILL(lbits)(&zghz3145);
  KILL(sail_int)(&zghz3144);
  KILL(lbits)(&zghz3143);
  KILL(lbits)(&zghz3142);
  KILL(sail_int)(&zghz3141);
  KILL(lbits)(&zghz3140);
  KILL(lbits)(&zghz3139);
  KILL(sail_int)(&zghz3138);
  KILL(lbits)(&zghz3137);
  KILL(lbits)(&zghz3136);
  KILL(lbits)(&zghz3135);
  KILL(lbits)(&zghz3134);
  KILL(sail_int)(&zghz3133);
  KILL(lbits)(&zghz3132);
  KILL(sail_int)(&zghz3131);
  KILL(sail_int)(&zghz3130);
  KILL(sail_int)(&zghz3129);
  KILL(sail_int)(&zghz3128);
  KILL(sail_int)(&zghz3127);
  KILL(sail_int)(&zghz3126);
  KILL(sail_int)(&zghz3125);
  KILL(lbits)(&zghz3124);
  KILL(sail_int)(&zghz3123);
  KILL(sail_int)(&zghz3122);
  KILL(sail_int)(&zghz3121);
  KILL(sail_int)(&zghz3120);
  KILL(sail_int)(&zghz3119);
  KILL(sail_int)(&zghz3118);
  KILL(lbits)(&zghz3117);
  KILL(sail_int)(&zghz3116);
  KILL(sail_int)(&zghz3115);
  KILL(sail_int)(&zghz3114);
  KILL(sail_int)(&zghz3113);
  KILL(sail_int)(&zghz3112);
  KILL(lbits)(&zghz3111);
  KILL(sail_int)(&zghz3110);
  KILL(sail_int)(&zghz3109);
  KILL(lbits)(&zghz3108);
  KILL(lbits)(&zghz3107);
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

lbits zghz3189;
sail_int zghz3190;
sail_int zghz3191;
lbits zghz3192;

static void startup_sailgen_getCapBounds(void)
{
  CREATE(lbits)(&zghz3189);
  CREATE(sail_int)(&zghz3190);
  CREATE(sail_int)(&zghz3191);
  CREATE(lbits)(&zghz3192);
}


static void sailgen_getCapBounds(struct ztuple_z8z5izCz0z5iz9 *zcbz327, struct zCapability zcap)
{
  __label__ case_118, finish_match_117, end_function_119, end_block_exception_120, end_function_203;

  struct ztuple_z8z5bv64zCz0z5bvz9 zgaz3210;
  CREATE(ztuple_z8z5bv64zCz0z5bvz9)(&zgaz3210);
  sailgen_getCapBoundsBits(&zgaz3210, zcap);
  struct ztuple_z8z5izCz0z5iz9 zgsz352;
  CREATE(ztuple_z8z5izCz0z5iz9)(&zgsz352);
  {
    uint64_t zbase;
    zbase = zgaz3210.ztup0;
    RECREATE(lbits)(&zghz3189);
    COPY(lbits)(&zghz3189, zgaz3210.ztup1);
    RECREATE(sail_int)(&zghz3190);
    {
      RECREATE(lbits)(&zghz3192);
      CONVERT_OF(lbits, fbits)(&zghz3192, zbase, UINT64_C(64) , true);
      sail_unsigned(&zghz3190, zghz3192);
    }
    RECREATE(sail_int)(&zghz3191);
    sail_unsigned(&zghz3191, zghz3189);
    COPY(sail_int)(&((&zgsz352)->ztup0), zghz3190);
    COPY(sail_int)(&((&zgsz352)->ztup1), zghz3191);
    goto finish_match_117;
  }
case_118: ;
  sail_match_failure("getCapBounds");
finish_match_117: ;
  COPY(ztuple_z8z5izCz0z5iz9)((*(&zcbz327)), zgsz352);
  KILL(ztuple_z8z5izCz0z5iz9)(&zgsz352);
  KILL(ztuple_z8z5bv64zCz0z5bvz9)(&zgaz3210);
end_function_119: ;
  goto end_function_203;
end_block_exception_120: ;
  goto end_function_203;
end_function_203: ;
}



static void finish_sailgen_getCapBounds(void)
{
  KILL(lbits)(&zghz3192);
  KILL(sail_int)(&zghz3191);
  KILL(sail_int)(&zghz3190);
  KILL(lbits)(&zghz3189);
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

lbits zghz3193;
lbits zghz3194;
struct zCapability zghz3195;
sail_int zghz3196;
sail_int zghz3197;
lbits zghz3198;
lbits zghz3199;
lbits zghz3200;
sail_int zghz3201;
lbits zghz3202;
lbits zghz3203;
sail_int zghz3204;
lbits zghz3205;
sail_int zghz3206;
sail_int zghz3207;
sail_int zghz3208;
sail_int zghz3209;
lbits zghz3210;
sail_int zghz3211;
lbits zghz3212;
sail_int zghz3213;
sail_int zghz3214;
sail_int zghz3215;
lbits zghz3216;
sail_int zghz3217;
lbits zghz3218;
sail_int zghz3219;
sail_int zghz3220;
sail_int zghz3221;
lbits zghz3222;
sail_int zghz3223;
lbits zghz3224;
sail_int zghz3225;
sail_int zghz3226;
lbits zghz3227;
sail_int zghz3228;
sail_int zghz3229;
lbits zghz3230;
sail_int zghz3231;
lbits zghz3232;
sail_int zghz3233;
lbits zghz3234;
lbits zghz3235;
sail_int zghz3236;
lbits zghz3237;
sail_int zghz3238;
sail_int zghz3239;
sail_int zghz3240;
sail_int zghz3241;
lbits zghz3242;
sail_int zghz3243;
lbits zghz3244;
sail_int zghz3245;
sail_int zghz3246;
sail_int zghz3247;
lbits zghz3248;
sail_int zghz3249;
lbits zghz3250;
sail_int zghz3251;
lbits zghz3252;
lbits zghz3253;
sail_int zghz3254;
lbits zghz3255;
sail_int zghz3256;
sail_int zghz3257;
sail_int zghz3258;
sail_int zghz3259;
sail_int zghz3260;
sail_int zghz3261;
sail_int zghz3262;
sail_int zghz3263;
sail_int zghz3264;
lbits zghz3265;
sail_int zghz3266;
sail_int zghz3267;
sail_int zghz3268;
lbits zghz3269;
sail_int zghz3270;
sail_int zghz3271;
sail_int zghz3272;
lbits zghz3273;
lbits zghz3274;

static void startup_sailgen_setCapBounds(void)
{
  CREATE(lbits)(&zghz3193);
  CREATE(lbits)(&zghz3194);

  CREATE(sail_int)(&zghz3196);
  CREATE(sail_int)(&zghz3197);
  CREATE(lbits)(&zghz3198);
  CREATE(lbits)(&zghz3199);
  CREATE(lbits)(&zghz3200);
  CREATE(sail_int)(&zghz3201);
  CREATE(lbits)(&zghz3202);
  CREATE(lbits)(&zghz3203);
  CREATE(sail_int)(&zghz3204);
  CREATE(lbits)(&zghz3205);
  CREATE(sail_int)(&zghz3206);
  CREATE(sail_int)(&zghz3207);
  CREATE(sail_int)(&zghz3208);
  CREATE(sail_int)(&zghz3209);
  CREATE(lbits)(&zghz3210);
  CREATE(sail_int)(&zghz3211);
  CREATE(lbits)(&zghz3212);
  CREATE(sail_int)(&zghz3213);
  CREATE(sail_int)(&zghz3214);
  CREATE(sail_int)(&zghz3215);
  CREATE(lbits)(&zghz3216);
  CREATE(sail_int)(&zghz3217);
  CREATE(lbits)(&zghz3218);
  CREATE(sail_int)(&zghz3219);
  CREATE(sail_int)(&zghz3220);
  CREATE(sail_int)(&zghz3221);
  CREATE(lbits)(&zghz3222);
  CREATE(sail_int)(&zghz3223);
  CREATE(lbits)(&zghz3224);
  CREATE(sail_int)(&zghz3225);
  CREATE(sail_int)(&zghz3226);
  CREATE(lbits)(&zghz3227);
  CREATE(sail_int)(&zghz3228);
  CREATE(sail_int)(&zghz3229);
  CREATE(lbits)(&zghz3230);
  CREATE(sail_int)(&zghz3231);
  CREATE(lbits)(&zghz3232);
  CREATE(sail_int)(&zghz3233);
  CREATE(lbits)(&zghz3234);
  CREATE(lbits)(&zghz3235);
  CREATE(sail_int)(&zghz3236);
  CREATE(lbits)(&zghz3237);
  CREATE(sail_int)(&zghz3238);
  CREATE(sail_int)(&zghz3239);
  CREATE(sail_int)(&zghz3240);
  CREATE(sail_int)(&zghz3241);
  CREATE(lbits)(&zghz3242);
  CREATE(sail_int)(&zghz3243);
  CREATE(lbits)(&zghz3244);
  CREATE(sail_int)(&zghz3245);
  CREATE(sail_int)(&zghz3246);
  CREATE(sail_int)(&zghz3247);
  CREATE(lbits)(&zghz3248);
  CREATE(sail_int)(&zghz3249);
  CREATE(lbits)(&zghz3250);
  CREATE(sail_int)(&zghz3251);
  CREATE(lbits)(&zghz3252);
  CREATE(lbits)(&zghz3253);
  CREATE(sail_int)(&zghz3254);
  CREATE(lbits)(&zghz3255);
  CREATE(sail_int)(&zghz3256);
  CREATE(sail_int)(&zghz3257);
  CREATE(sail_int)(&zghz3258);
  CREATE(sail_int)(&zghz3259);
  CREATE(sail_int)(&zghz3260);
  CREATE(sail_int)(&zghz3261);
  CREATE(sail_int)(&zghz3262);
  CREATE(sail_int)(&zghz3263);
  CREATE(sail_int)(&zghz3264);
  CREATE(lbits)(&zghz3265);
  CREATE(sail_int)(&zghz3266);
  CREATE(sail_int)(&zghz3267);
  CREATE(sail_int)(&zghz3268);
  CREATE(lbits)(&zghz3269);
  CREATE(sail_int)(&zghz3270);
  CREATE(sail_int)(&zghz3271);
  CREATE(sail_int)(&zghz3272);
  CREATE(lbits)(&zghz3273);
  CREATE(lbits)(&zghz3274);
}

static struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 sailgen_setCapBounds(struct zCapability zcap, uint64_t zbase, lbits ztop)
{
  __label__ end_function_122, end_block_exception_123;

  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz328;
  RECREATE(lbits)(&zghz3193);
  {
    RECREATE(lbits)(&zghz3273);
    CONVERT_OF(lbits, fbits)(&zghz3273, UINT64_C(0b0), UINT64_C(1) , true);
    RECREATE(lbits)(&zghz3274);
    CONVERT_OF(lbits, fbits)(&zghz3274, zbase, UINT64_C(64) , true);
    append(&zghz3193, zghz3273, zghz3274);
  }
  RECREATE(lbits)(&zghz3194);
  sub_bits(&zghz3194, ztop, zghz3193);
  int64_t ze;
  {
    int64_t zgaz3245;
    {
      uint64_t zgaz3244;
      {
        int64_t zgaz3243;
        {
          RECREATE(sail_int)(&zghz3270);
          CONVERT_OF(sail_int, mach_int)(&zghz3270, zcap_mantissa_width);
          RECREATE(sail_int)(&zghz3271);
          CONVERT_OF(sail_int, mach_int)(&zghz3271, INT64_C(1));
          RECREATE(sail_int)(&zghz3272);
          sub_int(&zghz3272, zghz3270, zghz3271);
          zgaz3243 = CONVERT_OF(mach_int, sail_int)(zghz3272);
        }
        {
          RECREATE(sail_int)(&zghz3267);
          CONVERT_OF(sail_int, mach_int)(&zghz3267, zcap_addr_width);
          RECREATE(sail_int)(&zghz3268);
          CONVERT_OF(sail_int, mach_int)(&zghz3268, zgaz3243);
          RECREATE(lbits)(&zghz3269);
          vector_subrange_lbits(&zghz3269, zghz3194, zghz3267, zghz3268);
          zgaz3244 = CONVERT_OF(fbits, lbits)(zghz3269, true);
        }
      }
      {
        RECREATE(lbits)(&zghz3265);
        CONVERT_OF(lbits, fbits)(&zghz3265, zgaz3244, UINT64_C(52) , true);
        RECREATE(sail_int)(&zghz3266);
        count_leading_zeros(&zghz3266, zghz3265);
        zgaz3245 = CONVERT_OF(mach_int, sail_int)(zghz3266);
      }
    }
    {
      RECREATE(sail_int)(&zghz3262);
      CONVERT_OF(sail_int, mach_int)(&zghz3262, zcap_max_E);
      RECREATE(sail_int)(&zghz3263);
      CONVERT_OF(sail_int, mach_int)(&zghz3263, zgaz3245);
      RECREATE(sail_int)(&zghz3264);
      sub_int(&zghz3264, zghz3262, zghz3263);
      ze = CONVERT_OF(mach_int, sail_int)(zghz3264);
    }
  }
  bool zie;
  {
    bool zgaz3242;
    {
      RECREATE(sail_int)(&zghz3260);
      CONVERT_OF(sail_int, mach_int)(&zghz3260, ze);
      RECREATE(sail_int)(&zghz3261);
      CONVERT_OF(sail_int, mach_int)(&zghz3261, INT64_C(0));
      zgaz3242 = sailgen_neq_int(zghz3260, zghz3261);
    }
    bool zgsz355;
    if (zgaz3242) {  zgsz355 = true;  } else {
      fbits zgaz3241;
      {
        int64_t zgaz3240;
        {
          RECREATE(sail_int)(&zghz3257);
          CONVERT_OF(sail_int, mach_int)(&zghz3257, zcap_mantissa_width);
          RECREATE(sail_int)(&zghz3258);
          CONVERT_OF(sail_int, mach_int)(&zghz3258, INT64_C(2));
          RECREATE(sail_int)(&zghz3259);
          sub_int(&zghz3259, zghz3257, zghz3258);
          zgaz3240 = CONVERT_OF(mach_int, sail_int)(zghz3259);
        }
        {
          RECREATE(sail_int)(&zghz3256);
          CONVERT_OF(sail_int, mach_int)(&zghz3256, zgaz3240);
          zgaz3241 = bitvector_access(zghz3194, zghz3256);
        }
      }
      zgsz355 = eq_bit(zgaz3241, UINT64_C(1));
    }
    zie = zgsz355;
  }
  uint64_t zBbits;
  {
    RECREATE(lbits)(&zghz3253);
    CONVERT_OF(lbits, fbits)(&zghz3253, zbase, UINT64_C(64) , true);
    RECREATE(sail_int)(&zghz3254);
    CONVERT_OF(sail_int, mach_int)(&zghz3254, zcap_mantissa_width);
    RECREATE(lbits)(&zghz3255);
    sail_truncate(&zghz3255, zghz3253, zghz3254);
    zBbits = CONVERT_OF(fbits, lbits)(zghz3255, true);
  }
  uint64_t zTbits;
  {
    RECREATE(sail_int)(&zghz3251);
    CONVERT_OF(sail_int, mach_int)(&zghz3251, zcap_mantissa_width);
    RECREATE(lbits)(&zghz3252);
    sail_truncate(&zghz3252, ztop, zghz3251);
    zTbits = CONVERT_OF(fbits, lbits)(zghz3252, true);
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
        uint64_t zgaz3235;
        {
          int64_t zgaz3234;
          zgaz3234 = (ze + INT64_C(3));
          {
            RECREATE(lbits)(&zghz3248);
            CONVERT_OF(lbits, fbits)(&zghz3248, zbase, UINT64_C(64) , true);
            RECREATE(sail_int)(&zghz3249);
            CONVERT_OF(sail_int, mach_int)(&zghz3249, zgaz3234);
            RECREATE(lbits)(&zghz3250);
            shiftr(&zghz3250, zghz3248, zghz3249);
            zgaz3235 = CONVERT_OF(fbits, lbits)(zghz3250, true);
          }
        }
        int64_t zgaz3236;
        {
          RECREATE(sail_int)(&zghz3245);
          CONVERT_OF(sail_int, mach_int)(&zghz3245, zcap_mantissa_width);
          RECREATE(sail_int)(&zghz3246);
          CONVERT_OF(sail_int, mach_int)(&zghz3246, INT64_C(3));
          RECREATE(sail_int)(&zghz3247);
          sub_int(&zghz3247, zghz3245, zghz3246);
          zgaz3236 = CONVERT_OF(mach_int, sail_int)(zghz3247);
        }
        {
          RECREATE(lbits)(&zghz3242);
          CONVERT_OF(lbits, fbits)(&zghz3242, zgaz3235, UINT64_C(64) , true);
          RECREATE(sail_int)(&zghz3243);
          CONVERT_OF(sail_int, mach_int)(&zghz3243, zgaz3236);
          RECREATE(lbits)(&zghz3244);
          sail_truncate(&zghz3244, zghz3242, zghz3243);
          zB_ie = CONVERT_OF(fbits, lbits)(zghz3244, true);
        }
      }
      uint64_t zT_ie;
      {
        RECREATE(lbits)(&zghz3235);
        {
          int64_t zgaz3231;
          zgaz3231 = (ze + INT64_C(3));
          {
            RECREATE(sail_int)(&zghz3241);
            CONVERT_OF(sail_int, mach_int)(&zghz3241, zgaz3231);
            shiftr(&zghz3235, ztop, zghz3241);
          }
        }
        int64_t zgaz3233;
        {
          RECREATE(sail_int)(&zghz3238);
          CONVERT_OF(sail_int, mach_int)(&zghz3238, zcap_mantissa_width);
          RECREATE(sail_int)(&zghz3239);
          CONVERT_OF(sail_int, mach_int)(&zghz3239, INT64_C(3));
          RECREATE(sail_int)(&zghz3240);
          sub_int(&zghz3240, zghz3238, zghz3239);
          zgaz3233 = CONVERT_OF(mach_int, sail_int)(zghz3240);
        }
        {
          RECREATE(sail_int)(&zghz3236);
          CONVERT_OF(sail_int, mach_int)(&zghz3236, zgaz3233);
          RECREATE(lbits)(&zghz3237);
          sail_truncate(&zghz3237, zghz3235, zghz3236);
          zT_ie = CONVERT_OF(fbits, lbits)(zghz3237, true);
        }
      }
      RECREATE(lbits)(&zghz3199);
      {
        sbits zgaz3230;
        {
          int64_t zgaz3229;
          zgaz3229 = (ze + INT64_C(3));
          {
            RECREATE(sail_int)(&zghz3233);
            CONVERT_OF(sail_int, mach_int)(&zghz3233, zgaz3229);
            RECREATE(lbits)(&zghz3234);
            sailgen_ones(&zghz3234, zghz3233);
            zgaz3230 = CONVERT_OF(sbits, lbits)(zghz3234, true);
          }
        }
        {
          RECREATE(sail_int)(&zghz3231);
          CONVERT_OF(sail_int, mach_int)(&zghz3231, INT64_C(65));
          RECREATE(lbits)(&zghz3232);
          CONVERT_OF(lbits, sbits)(&zghz3232, zgaz3230, true);
          sailgen_zzero_extend(&zghz3199, zghz3231, zghz3232);
        }
      }
      {
        RECREATE(sail_int)(&zghz3228);
        {
          RECREATE(lbits)(&zghz3230);
          and_bits(&zghz3230, zghz3193, zghz3199);
          sail_unsigned(&zghz3228, zghz3230);
        }
        {
          RECREATE(sail_int)(&zghz3229);
          CONVERT_OF(sail_int, mach_int)(&zghz3229, INT64_C(0));
          zlostSignificantBase = sailgen_neq_int(zghz3228, zghz3229);
        }
        unit zgsz359;
        zgsz359 = UNIT;
      }
      {
        RECREATE(sail_int)(&zghz3225);
        {
          RECREATE(lbits)(&zghz3227);
          and_bits(&zghz3227, ztop, zghz3199);
          sail_unsigned(&zghz3225, zghz3227);
        }
        {
          RECREATE(sail_int)(&zghz3226);
          CONVERT_OF(sail_int, mach_int)(&zghz3226, INT64_C(0));
          zlostSignificantTop = sailgen_neq_int(zghz3225, zghz3226);
        }
        unit zgsz358;
        zgsz358 = UNIT;
      }
      {
        unit zgsz357;
        if (zlostSignificantTop) {
          {
            RECREATE(lbits)(&zghz3222);
            CONVERT_OF(lbits, fbits)(&zghz3222, zT_ie, UINT64_C(11) , true);
            RECREATE(sail_int)(&zghz3223);
            CONVERT_OF(sail_int, mach_int)(&zghz3223, INT64_C(1));
            RECREATE(lbits)(&zghz3224);
            add_bits_int(&zghz3224, zghz3222, zghz3223);
            zT_ie = CONVERT_OF(fbits, lbits)(zghz3224, true);
          }
          zgsz357 = UNIT;
        } else {  zgsz357 = UNIT;  }
      }
      uint64_t zlen_ie;
      zlen_ie = ((zT_ie - zB_ie) & UINT64_C(0b11111111111));
      {
        bool zgaz3219;
        {
          fbits zgaz3218;
          {
            int64_t zgaz3217;
            {
              RECREATE(sail_int)(&zghz3219);
              CONVERT_OF(sail_int, mach_int)(&zghz3219, zcap_mantissa_width);
              RECREATE(sail_int)(&zghz3220);
              CONVERT_OF(sail_int, mach_int)(&zghz3220, INT64_C(4));
              RECREATE(sail_int)(&zghz3221);
              sub_int(&zghz3221, zghz3219, zghz3220);
              zgaz3217 = CONVERT_OF(mach_int, sail_int)(zghz3221);
            }
            zgaz3218 = (UINT64_C(1) & (zlen_ie >> zgaz3217));
          }
          zgaz3219 = eq_bit(zgaz3218, UINT64_C(1));
        }
        unit zgsz361;
        if (zgaz3219) {
          {
            zincE = true;
            unit zgsz367;
            zgsz367 = UNIT;
          }
          {
            bool zgsz362;
            if (zlostSignificantBase) {  zgsz362 = true;  } else {
              fbits zgaz3220;
              zgaz3220 = (UINT64_C(1) & (zB_ie >> INT64_C(0)));
              zgsz362 = eq_bit(zgaz3220, UINT64_C(1));
            }
            zlostSignificantBase = zgsz362;
            unit zgsz366;
            zgsz366 = UNIT;
          }
          {
            bool zgsz363;
            if (zlostSignificantTop) {  zgsz363 = true;  } else {
              fbits zgaz3221;
              zgaz3221 = (UINT64_C(1) & (zT_ie >> INT64_C(0)));
              zgsz363 = eq_bit(zgaz3221, UINT64_C(1));
            }
            zlostSignificantTop = zgsz363;
            unit zgsz365;
            zgsz365 = UNIT;
          }
          {
            uint64_t zgaz3223;
            {
              int64_t zgaz3222;
              zgaz3222 = (ze + INT64_C(4));
              {
                RECREATE(lbits)(&zghz3216);
                CONVERT_OF(lbits, fbits)(&zghz3216, zbase, UINT64_C(64) , true);
                RECREATE(sail_int)(&zghz3217);
                CONVERT_OF(sail_int, mach_int)(&zghz3217, zgaz3222);
                RECREATE(lbits)(&zghz3218);
                shiftr(&zghz3218, zghz3216, zghz3217);
                zgaz3223 = CONVERT_OF(fbits, lbits)(zghz3218, true);
              }
            }
            int64_t zgaz3224;
            {
              RECREATE(sail_int)(&zghz3213);
              CONVERT_OF(sail_int, mach_int)(&zghz3213, zcap_mantissa_width);
              RECREATE(sail_int)(&zghz3214);
              CONVERT_OF(sail_int, mach_int)(&zghz3214, INT64_C(3));
              RECREATE(sail_int)(&zghz3215);
              sub_int(&zghz3215, zghz3213, zghz3214);
              zgaz3224 = CONVERT_OF(mach_int, sail_int)(zghz3215);
            }
            {
              RECREATE(lbits)(&zghz3210);
              CONVERT_OF(lbits, fbits)(&zghz3210, zgaz3223, UINT64_C(64) , true);
              RECREATE(sail_int)(&zghz3211);
              CONVERT_OF(sail_int, mach_int)(&zghz3211, zgaz3224);
              RECREATE(lbits)(&zghz3212);
              sail_truncate(&zghz3212, zghz3210, zghz3211);
              zB_ie = CONVERT_OF(fbits, lbits)(zghz3212, true);
            }
            unit zgsz364;
            zgsz364 = UNIT;
          }
          int64_t zincT;
          if (zlostSignificantTop) {  zincT = INT64_C(1);  } else {  zincT = INT64_C(0);  }
          uint64_t zgaz3228;
          {
            RECREATE(lbits)(&zghz3203);
            {
              int64_t zgaz3225;
              zgaz3225 = (ze + INT64_C(4));
              {
                RECREATE(sail_int)(&zghz3209);
                CONVERT_OF(sail_int, mach_int)(&zghz3209, zgaz3225);
                shiftr(&zghz3203, ztop, zghz3209);
              }
            }
            int64_t zgaz3227;
            {
              RECREATE(sail_int)(&zghz3206);
              CONVERT_OF(sail_int, mach_int)(&zghz3206, zcap_mantissa_width);
              RECREATE(sail_int)(&zghz3207);
              CONVERT_OF(sail_int, mach_int)(&zghz3207, INT64_C(3));
              RECREATE(sail_int)(&zghz3208);
              sub_int(&zghz3208, zghz3206, zghz3207);
              zgaz3227 = CONVERT_OF(mach_int, sail_int)(zghz3208);
            }
            {
              RECREATE(sail_int)(&zghz3204);
              CONVERT_OF(sail_int, mach_int)(&zghz3204, zgaz3227);
              RECREATE(lbits)(&zghz3205);
              sail_truncate(&zghz3205, zghz3203, zghz3204);
              zgaz3228 = CONVERT_OF(fbits, lbits)(zghz3205, true);
            }
          }
          {
            RECREATE(lbits)(&zghz3200);
            CONVERT_OF(lbits, fbits)(&zghz3200, zgaz3228, UINT64_C(11) , true);
            RECREATE(sail_int)(&zghz3201);
            CONVERT_OF(sail_int, mach_int)(&zghz3201, zincT);
            RECREATE(lbits)(&zghz3202);
            add_bits_int(&zghz3202, zghz3200, zghz3201);
            zT_ie = CONVERT_OF(fbits, lbits)(zghz3202, true);
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
  struct zCapability zghz3195;
  {
    uint64_t zgaz3239;
    {
      int64_t zgaz3238;
      if (zincE) {  zgaz3238 = (ze + INT64_C(1));  } else {  zgaz3238 = ze;  }
      {
        RECREATE(sail_int)(&zghz3196);
        CONVERT_OF(sail_int, mach_int)(&zghz3196, INT64_C(6));
        RECREATE(sail_int)(&zghz3197);
        CONVERT_OF(sail_int, mach_int)(&zghz3197, zgaz3238);
        RECREATE(lbits)(&zghz3198);
        sailgen_to_bits(&zghz3198, zghz3196, zghz3197);
        zgaz3239 = CONVERT_OF(fbits, lbits)(zghz3198, true);
      }
    }
    zghz3195 = zcap;
    zghz3195.zB = zBbits;
    zghz3195.zE = zgaz3239;
    zghz3195.zT = zTbits;
    zghz3195.zaddress = zbase;
    zghz3195.zinternal_E = zie;
  }
  bool zexact;
  {
    bool zgaz3237;
    {    if (zlostSignificantBase) {  zgaz3237 = true;  } else {  zgaz3237 = zlostSignificantTop;  }
    }
    zexact = sailgen_not(zgaz3237);
  }
  zcbz328.ztup0 = zexact;
  zcbz328.ztup1 = zghz3195;








end_function_122: ;
  return zcbz328;
end_block_exception_123: ;
  struct zCapability zcbz355 = { .zB = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zaddress = UINT64_C(0xdeadc0de), .zflag_cap_mode = false, .zglobal = false, .zinternal_E = false, .zotype = UINT64_C(0xdeadc0de), .zpermit_cinvoke = false, .zpermit_execute = false, .zpermit_load = false, .zpermit_load_cap = false, .zpermit_seal = false, .zpermit_set_CID = false, .zpermit_store = false, .zpermit_store_cap = false, .zpermit_store_local_cap = false, .zpermit_unseal = false, .zreserved = UINT64_C(0xdeadc0de), .ztag = false, .zuperms = UINT64_C(0xdeadc0de) };
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz354 = { .ztup0 = false, .ztup1 = zcbz355 };
  return zcbz354;
}



static void finish_sailgen_setCapBounds(void)
{
  KILL(lbits)(&zghz3274);
  KILL(lbits)(&zghz3273);
  KILL(sail_int)(&zghz3272);
  KILL(sail_int)(&zghz3271);
  KILL(sail_int)(&zghz3270);
  KILL(lbits)(&zghz3269);
  KILL(sail_int)(&zghz3268);
  KILL(sail_int)(&zghz3267);
  KILL(sail_int)(&zghz3266);
  KILL(lbits)(&zghz3265);
  KILL(sail_int)(&zghz3264);
  KILL(sail_int)(&zghz3263);
  KILL(sail_int)(&zghz3262);
  KILL(sail_int)(&zghz3261);
  KILL(sail_int)(&zghz3260);
  KILL(sail_int)(&zghz3259);
  KILL(sail_int)(&zghz3258);
  KILL(sail_int)(&zghz3257);
  KILL(sail_int)(&zghz3256);
  KILL(lbits)(&zghz3255);
  KILL(sail_int)(&zghz3254);
  KILL(lbits)(&zghz3253);
  KILL(lbits)(&zghz3252);
  KILL(sail_int)(&zghz3251);
  KILL(lbits)(&zghz3250);
  KILL(sail_int)(&zghz3249);
  KILL(lbits)(&zghz3248);
  KILL(sail_int)(&zghz3247);
  KILL(sail_int)(&zghz3246);
  KILL(sail_int)(&zghz3245);
  KILL(lbits)(&zghz3244);
  KILL(sail_int)(&zghz3243);
  KILL(lbits)(&zghz3242);
  KILL(sail_int)(&zghz3241);
  KILL(sail_int)(&zghz3240);
  KILL(sail_int)(&zghz3239);
  KILL(sail_int)(&zghz3238);
  KILL(lbits)(&zghz3237);
  KILL(sail_int)(&zghz3236);
  KILL(lbits)(&zghz3235);
  KILL(lbits)(&zghz3234);
  KILL(sail_int)(&zghz3233);
  KILL(lbits)(&zghz3232);
  KILL(sail_int)(&zghz3231);
  KILL(lbits)(&zghz3230);
  KILL(sail_int)(&zghz3229);
  KILL(sail_int)(&zghz3228);
  KILL(lbits)(&zghz3227);
  KILL(sail_int)(&zghz3226);
  KILL(sail_int)(&zghz3225);
  KILL(lbits)(&zghz3224);
  KILL(sail_int)(&zghz3223);
  KILL(lbits)(&zghz3222);
  KILL(sail_int)(&zghz3221);
  KILL(sail_int)(&zghz3220);
  KILL(sail_int)(&zghz3219);
  KILL(lbits)(&zghz3218);
  KILL(sail_int)(&zghz3217);
  KILL(lbits)(&zghz3216);
  KILL(sail_int)(&zghz3215);
  KILL(sail_int)(&zghz3214);
  KILL(sail_int)(&zghz3213);
  KILL(lbits)(&zghz3212);
  KILL(sail_int)(&zghz3211);
  KILL(lbits)(&zghz3210);
  KILL(sail_int)(&zghz3209);
  KILL(sail_int)(&zghz3208);
  KILL(sail_int)(&zghz3207);
  KILL(sail_int)(&zghz3206);
  KILL(lbits)(&zghz3205);
  KILL(sail_int)(&zghz3204);
  KILL(lbits)(&zghz3203);
  KILL(lbits)(&zghz3202);
  KILL(sail_int)(&zghz3201);
  KILL(lbits)(&zghz3200);
  KILL(lbits)(&zghz3199);
  KILL(lbits)(&zghz3198);
  KILL(sail_int)(&zghz3197);
  KILL(sail_int)(&zghz3196);

  KILL(lbits)(&zghz3194);
  KILL(lbits)(&zghz3193);
}

static uint64_t sailgen_getCapPerms(struct zCapability);

sail_int zghz3275;
lbits zghz3276;
lbits zghz3277;

static void startup_sailgen_getCapPerms(void)
{
  CREATE(sail_int)(&zghz3275);
  CREATE(lbits)(&zghz3276);
  CREATE(lbits)(&zghz3277);
}

static uint64_t sailgen_getCapPerms(struct zCapability zcap)
{
  __label__ end_function_125, end_block_exception_126;

  uint64_t zcbz329;
  uint64_t zperms;
  {
    uint64_t zgaz3247;
    zgaz3247 = sailgen_getCapHardPerms(zcap);
    {
      RECREATE(sail_int)(&zghz3275);
      CONVERT_OF(sail_int, mach_int)(&zghz3275, INT64_C(15));
      RECREATE(lbits)(&zghz3276);
      CONVERT_OF(lbits, fbits)(&zghz3276, zgaz3247, UINT64_C(12) , true);
      RECREATE(lbits)(&zghz3277);
      sailgen_zzero_extend(&zghz3277, zghz3275, zghz3276);
      zperms = CONVERT_OF(fbits, lbits)(zghz3277, true);
    }
  }
  uint64_t zgaz3246;
  zgaz3246 = zcap.zuperms;
  zcbz329 = (zgaz3246 << 15) | zperms;


end_function_125: ;
  return zcbz329;
end_block_exception_126: ;

  return UINT64_C(0xdeadc0de);
}



static void finish_sailgen_getCapPerms(void)
{
  KILL(lbits)(&zghz3277);
  KILL(lbits)(&zghz3276);
  KILL(sail_int)(&zghz3275);
}

static struct zCapability sailgen_setCapPerms(struct zCapability, uint64_t);

struct zCapability zghz3278;
lbits zghz3279;
sail_int zghz3280;
lbits zghz3281;
lbits zghz3282;
sail_int zghz3283;
lbits zghz3284;

static void startup_sailgen_setCapPerms(void)
{

  CREATE(lbits)(&zghz3279);
  CREATE(sail_int)(&zghz3280);
  CREATE(lbits)(&zghz3281);
  CREATE(lbits)(&zghz3282);
  CREATE(sail_int)(&zghz3283);
  CREATE(lbits)(&zghz3284);
}

static struct zCapability sailgen_setCapPerms(struct zCapability zcap, uint64_t zperms)
{
  __label__ end_function_128, end_block_exception_129;

  struct zCapability zghz3278;
  uint64_t zgaz3249;
  {
    uint64_t zgaz3248;
    {
      RECREATE(lbits)(&zghz3282);
      CONVERT_OF(lbits, fbits)(&zghz3282, zperms, UINT64_C(19) , true);
      RECREATE(sail_int)(&zghz3283);
      CONVERT_OF(sail_int, mach_int)(&zghz3283, zcap_uperms_shift);
      RECREATE(lbits)(&zghz3284);
      shiftr(&zghz3284, zghz3282, zghz3283);
      zgaz3248 = CONVERT_OF(fbits, lbits)(zghz3284, true);
    }
    {
      RECREATE(lbits)(&zghz3279);
      CONVERT_OF(lbits, fbits)(&zghz3279, zgaz3248, UINT64_C(19) , true);
      RECREATE(sail_int)(&zghz3280);
      CONVERT_OF(sail_int, mach_int)(&zghz3280, zcap_uperms_width);
      RECREATE(lbits)(&zghz3281);
      sail_truncate(&zghz3281, zghz3279, zghz3280);
      zgaz3249 = CONVERT_OF(fbits, lbits)(zghz3281, true);
    }
  }
  bool zgaz3251;
  {
    fbits zgaz3250;
    zgaz3250 = (UINT64_C(1) & (zperms >> INT64_C(11)));
    zgaz3251 = sailgen_bit_to_bool(zgaz3250);
  }
  bool zgaz3253;
  {
    fbits zgaz3252;
    zgaz3252 = (UINT64_C(1) & (zperms >> INT64_C(10)));
    zgaz3253 = sailgen_bit_to_bool(zgaz3252);
  }
  bool zgaz3255;
  {
    fbits zgaz3254;
    zgaz3254 = (UINT64_C(1) & (zperms >> INT64_C(9)));
    zgaz3255 = sailgen_bit_to_bool(zgaz3254);
  }
  bool zgaz3257;
  {
    fbits zgaz3256;
    zgaz3256 = (UINT64_C(1) & (zperms >> INT64_C(8)));
    zgaz3257 = sailgen_bit_to_bool(zgaz3256);
  }
  bool zgaz3259;
  {
    fbits zgaz3258;
    zgaz3258 = (UINT64_C(1) & (zperms >> INT64_C(7)));
    zgaz3259 = sailgen_bit_to_bool(zgaz3258);
  }
  bool zgaz3261;
  {
    fbits zgaz3260;
    zgaz3260 = (UINT64_C(1) & (zperms >> INT64_C(6)));
    zgaz3261 = sailgen_bit_to_bool(zgaz3260);
  }
  bool zgaz3263;
  {
    fbits zgaz3262;
    zgaz3262 = (UINT64_C(1) & (zperms >> INT64_C(5)));
    zgaz3263 = sailgen_bit_to_bool(zgaz3262);
  }
  bool zgaz3265;
  {
    fbits zgaz3264;
    zgaz3264 = (UINT64_C(1) & (zperms >> INT64_C(4)));
    zgaz3265 = sailgen_bit_to_bool(zgaz3264);
  }
  bool zgaz3267;
  {
    fbits zgaz3266;
    zgaz3266 = (UINT64_C(1) & (zperms >> INT64_C(3)));
    zgaz3267 = sailgen_bit_to_bool(zgaz3266);
  }
  bool zgaz3269;
  {
    fbits zgaz3268;
    zgaz3268 = (UINT64_C(1) & (zperms >> INT64_C(2)));
    zgaz3269 = sailgen_bit_to_bool(zgaz3268);
  }
  bool zgaz3271;
  {
    fbits zgaz3270;
    zgaz3270 = (UINT64_C(1) & (zperms >> INT64_C(1)));
    zgaz3271 = sailgen_bit_to_bool(zgaz3270);
  }
  bool zgaz3273;
  {
    fbits zgaz3272;
    zgaz3272 = (UINT64_C(1) & (zperms >> INT64_C(0)));
    zgaz3273 = sailgen_bit_to_bool(zgaz3272);
  }
  zghz3278 = zcap;
  zghz3278.zaccess_system_regs = zgaz3253;
  zghz3278.zglobal = zgaz3273;
  zghz3278.zpermit_cinvoke = zgaz3257;
  zghz3278.zpermit_execute = zgaz3271;
  zghz3278.zpermit_load = zgaz3269;
  zghz3278.zpermit_load_cap = zgaz3265;
  zghz3278.zpermit_seal = zgaz3259;
  zghz3278.zpermit_set_CID = zgaz3251;
  zghz3278.zpermit_store = zgaz3267;
  zghz3278.zpermit_store_cap = zgaz3263;
  zghz3278.zpermit_store_local_cap = zgaz3261;
  zghz3278.zpermit_unseal = zgaz3255;
  zghz3278.zuperms = zgaz3249;













end_function_128: ;
  return zghz3278;
end_block_exception_129: ;
  struct zCapability zcbz356 = { .zB = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zaddress = UINT64_C(0xdeadc0de), .zflag_cap_mode = false, .zglobal = false, .zinternal_E = false, .zotype = UINT64_C(0xdeadc0de), .zpermit_cinvoke = false, .zpermit_execute = false, .zpermit_load = false, .zpermit_load_cap = false, .zpermit_seal = false, .zpermit_set_CID = false, .zpermit_store = false, .zpermit_store_cap = false, .zpermit_store_local_cap = false, .zpermit_unseal = false, .zreserved = UINT64_C(0xdeadc0de), .ztag = false, .zuperms = UINT64_C(0xdeadc0de) };
  return zcbz356;
}



static void finish_sailgen_setCapPerms(void)
{
  KILL(lbits)(&zghz3284);
  KILL(sail_int)(&zghz3283);
  KILL(lbits)(&zghz3282);
  KILL(lbits)(&zghz3281);
  KILL(sail_int)(&zghz3280);
  KILL(lbits)(&zghz3279);

}

static uint64_t sailgen_getCapFlags(struct zCapability);

static uint64_t sailgen_getCapFlags(struct zCapability zcap)
{
  __label__ end_function_131, end_block_exception_132;

  uint64_t zcbz331;
  bool zgaz3274;
  zgaz3274 = zcap.zflag_cap_mode;
  zcbz331 = sailgen_bool_to_bits(zgaz3274);

end_function_131: ;
  return zcbz331;
end_block_exception_132: ;

  return UINT64_C(0xdeadc0de);
}

static bool sailgen_isCapSealed(struct zCapability);

sail_int zghz3285;
sail_int zghz3286;
lbits zghz3287;
sail_int zghz3288;

static void startup_sailgen_isCapSealed(void)
{
  CREATE(sail_int)(&zghz3285);
  CREATE(sail_int)(&zghz3286);
  CREATE(lbits)(&zghz3287);
  CREATE(sail_int)(&zghz3288);
}

static bool sailgen_isCapSealed(struct zCapability zcap)
{
  __label__ end_function_134, end_block_exception_135;

  bool zcbz332;
  int64_t zgaz3276;
  {
    uint64_t zgaz3275;
    zgaz3275 = zcap.zotype;
    {
      RECREATE(lbits)(&zghz3287);
      CONVERT_OF(lbits, fbits)(&zghz3287, zgaz3275, UINT64_C(18) , true);
      RECREATE(sail_int)(&zghz3288);
      sail_signed(&zghz3288, zghz3287);
      zgaz3276 = CONVERT_OF(mach_int, sail_int)(zghz3288);
    }
  }
  {
    RECREATE(sail_int)(&zghz3285);
    CONVERT_OF(sail_int, mach_int)(&zghz3285, zgaz3276);
    RECREATE(sail_int)(&zghz3286);
    CONVERT_OF(sail_int, mach_int)(&zghz3286, zotype_unsealed);
    zcbz332 = sailgen_neq_int(zghz3285, zghz3286);
  }

end_function_134: ;
  return zcbz332;
end_block_exception_135: ;

  return false;
}



static void finish_sailgen_isCapSealed(void)
{
  KILL(sail_int)(&zghz3288);
  KILL(lbits)(&zghz3287);
  KILL(sail_int)(&zghz3286);
  KILL(sail_int)(&zghz3285);
}

static bool sailgen_hasReservedOType(struct zCapability);

lbits zghz3289;
sail_int zghz3290;

static void startup_sailgen_hasReservedOType(void)
{
  CREATE(lbits)(&zghz3289);
  CREATE(sail_int)(&zghz3290);
}

static bool sailgen_hasReservedOType(struct zCapability zcap)
{
  __label__ end_function_137, end_block_exception_138;

  bool zcbz333;
  int64_t zgaz3278;
  {
    uint64_t zgaz3277;
    zgaz3277 = zcap.zotype;
    {
      RECREATE(lbits)(&zghz3289);
      CONVERT_OF(lbits, fbits)(&zghz3289, zgaz3277, UINT64_C(18) , true);
      RECREATE(sail_int)(&zghz3290);
      sail_unsigned(&zghz3290, zghz3289);
      zgaz3278 = CONVERT_OF(mach_int, sail_int)(zghz3290);
    }
  }
  zcbz333 = (zgaz3278 > zcap_max_otype);

end_function_137: ;
  return zcbz333;
end_block_exception_138: ;

  return false;
}



static void finish_sailgen_hasReservedOType(void)
{
  KILL(sail_int)(&zghz3290);
  KILL(lbits)(&zghz3289);
}

static uint64_t sailgen_getCapBaseBits(struct zCapability);

static uint64_t sailgen_getCapBaseBits(struct zCapability zc)
{
  __label__ case_141, finish_match_140, end_function_142, end_block_exception_143;

  uint64_t zcbz334;
  struct ztuple_z8z5bv64zCz0z5bvz9 zgaz3279;
  CREATE(ztuple_z8z5bv64zCz0z5bvz9)(&zgaz3279);
  sailgen_getCapBoundsBits(&zgaz3279, zc);
  {
    uint64_t zbase;
    zbase = zgaz3279.ztup0;
    zcbz334 = zbase;
    goto finish_match_140;
  }
case_141: ;
  sail_match_failure("getCapBaseBits");
finish_match_140: ;
  KILL(ztuple_z8z5bv64zCz0z5bvz9)(&zgaz3279);
end_function_142: ;
  return zcbz334;
end_block_exception_143: ;

  return UINT64_C(0xdeadc0de);
}

static void sailgen_getCapLength(sail_int *rop, struct zCapability);

sail_int zghz3291;
sail_int zghz3292;
sail_int zghz3293;
sail_int zghz3294;
sail_int zghz3295;
sail_int zghz3296;

static void startup_sailgen_getCapLength(void)
{
  CREATE(sail_int)(&zghz3291);
  CREATE(sail_int)(&zghz3292);
  CREATE(sail_int)(&zghz3293);
  CREATE(sail_int)(&zghz3294);
  CREATE(sail_int)(&zghz3295);
  CREATE(sail_int)(&zghz3296);
}

static void sailgen_getCapLength(sail_int *zcbz335, struct zCapability zc)
{
  __label__ case_146, finish_match_145, end_function_147, end_block_exception_148, end_function_202;

  struct ztuple_z8z5izCz0z5iz9 zgaz3280;
  CREATE(ztuple_z8z5izCz0z5iz9)(&zgaz3280);
  sailgen_getCapBounds(&zgaz3280, zc);
  RECREATE(sail_int)(&zghz3291);
  {
    RECREATE(sail_int)(&zghz3292);
    COPY(sail_int)(&zghz3292, zgaz3280.ztup0);
    RECREATE(sail_int)(&zghz3293);
    COPY(sail_int)(&zghz3293, zgaz3280.ztup1);
    {
      bool zgaz3283;
      {
        bool zgaz3282;
        {
          bool zgaz3281;
          zgaz3281 = zc.ztag;
          zgaz3282 = sailgen_not(zgaz3281);
        }
        bool zgsz375;
        if (zgaz3282) {  zgsz375 = true;  } else {  zgsz375 = gteq(zghz3293, zghz3292);  }
        zgaz3283 = zgsz375;
      }
      unit zgsz376;
      zgsz376 = sail_assert(zgaz3283, "src/cheri_cap_common.sail:453.40-453.41");
    }
    RECREATE(sail_int)(&zghz3294);
    sub_int(&zghz3294, zghz3293, zghz3292);
    RECREATE(sail_int)(&zghz3295);
    {
      RECREATE(sail_int)(&zghz3296);
      CONVERT_OF(sail_int, mach_int)(&zghz3296, zcap_len_width);
      pow2(&zghz3295, zghz3296);
    }
    emod_int(&zghz3291, zghz3294, zghz3295);
    goto finish_match_145;
  }
case_146: ;
  sail_match_failure("getCapLength");
finish_match_145: ;
  COPY(sail_int)((*(&zcbz335)), zghz3291);
  KILL(ztuple_z8z5izCz0z5iz9)(&zgaz3280);
end_function_147: ;
  goto end_function_202;
end_block_exception_148: ;
  goto end_function_202;
end_function_202: ;
}



static void finish_sailgen_getCapLength(void)
{
  KILL(sail_int)(&zghz3296);
  KILL(sail_int)(&zghz3295);
  KILL(sail_int)(&zghz3294);
  KILL(sail_int)(&zghz3293);
  KILL(sail_int)(&zghz3292);
  KILL(sail_int)(&zghz3291);
}

static bool sailgen_inCapBounds(struct zCapability, uint64_t, sail_int);

sail_int zghz3297;
sail_int zghz3298;
sail_int zghz3299;
sail_int zghz3300;
lbits zghz3301;

static void startup_sailgen_inCapBounds(void)
{
  CREATE(sail_int)(&zghz3297);
  CREATE(sail_int)(&zghz3298);
  CREATE(sail_int)(&zghz3299);
  CREATE(sail_int)(&zghz3300);
  CREATE(lbits)(&zghz3301);
}

static bool sailgen_inCapBounds(struct zCapability zcap, uint64_t zaddr, sail_int zsizze)
{
  __label__ case_151, finish_match_150, end_function_152, end_block_exception_153;

  bool zcbz336;
  struct ztuple_z8z5izCz0z5iz9 zgaz3286;
  CREATE(ztuple_z8z5izCz0z5iz9)(&zgaz3286);
  sailgen_getCapBounds(&zgaz3286, zcap);
  {
    RECREATE(sail_int)(&zghz3297);
    COPY(sail_int)(&zghz3297, zgaz3286.ztup0);
    RECREATE(sail_int)(&zghz3298);
    COPY(sail_int)(&zghz3298, zgaz3286.ztup1);
    RECREATE(sail_int)(&zghz3299);
    {
      RECREATE(lbits)(&zghz3301);
      CONVERT_OF(lbits, fbits)(&zghz3301, zaddr, UINT64_C(64) , true);
      sail_unsigned(&zghz3299, zghz3301);
    }
    bool zgaz3288;
    zgaz3288 = gteq(zghz3299, zghz3297);
    bool zgsz379;
    if (zgaz3288) {
      RECREATE(sail_int)(&zghz3300);
      add_int(&zghz3300, zghz3299, zsizze);
      zgsz379 = lteq(zghz3300, zghz3298);
    } else {  zgsz379 = false;  }
    zcbz336 = zgsz379;
    goto finish_match_150;
  }
case_151: ;
  sail_match_failure("inCapBounds");
finish_match_150: ;
  KILL(ztuple_z8z5izCz0z5iz9)(&zgaz3286);
end_function_152: ;
  return zcbz336;
end_block_exception_153: ;

  return false;
}



static void finish_sailgen_inCapBounds(void)
{
  KILL(lbits)(&zghz3301);
  KILL(sail_int)(&zghz3300);
  KILL(sail_int)(&zghz3299);
  KILL(sail_int)(&zghz3298);
  KILL(sail_int)(&zghz3297);
}

static struct zCapability sailgen_clearTagIf(struct zCapability, bool);

struct zCapability zghz3302;

static void startup_sailgen_clearTagIf(void)
{
}

static struct zCapability sailgen_clearTagIf(struct zCapability zcap, bool zcond)
{
  __label__ end_function_155, end_block_exception_156;

  struct zCapability zghz3302;
  bool zgaz3290;
  {
    bool zgaz3289;
    zgaz3289 = zcap.ztag;
    bool zgsz381;
    if (zgaz3289) {  zgsz381 = sailgen_not(zcond);  } else {  zgsz381 = false;  }
    zgaz3290 = zgsz381;
  }
  zghz3302 = zcap;
  zghz3302.ztag = zgaz3290;

end_function_155: ;
  return zghz3302;
end_block_exception_156: ;
  struct zCapability zcbz357 = { .zB = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zaddress = UINT64_C(0xdeadc0de), .zflag_cap_mode = false, .zglobal = false, .zinternal_E = false, .zotype = UINT64_C(0xdeadc0de), .zpermit_cinvoke = false, .zpermit_execute = false, .zpermit_load = false, .zpermit_load_cap = false, .zpermit_seal = false, .zpermit_set_CID = false, .zpermit_store = false, .zpermit_store_cap = false, .zpermit_store_local_cap = false, .zpermit_unseal = false, .zreserved = UINT64_C(0xdeadc0de), .ztag = false, .zuperms = UINT64_C(0xdeadc0de) };
  return zcbz357;
}



static void finish_sailgen_clearTagIf(void)
{
}

static struct zCapability sailgen_clearTagIfSealed(struct zCapability);

struct zCapability zghz3303;

static void startup_sailgen_clearTagIfSealed(void)
{
}

static struct zCapability sailgen_clearTagIfSealed(struct zCapability zcap)
{
  __label__ end_function_158, end_block_exception_159;

  struct zCapability zghz3303;
  bool zgaz3291;
  zgaz3291 = sailgen_isCapSealed(zcap);
  zghz3303 = sailgen_clearTagIf(zcap, zgaz3291);

end_function_158: ;
  return zghz3303;
end_block_exception_159: ;
  struct zCapability zcbz358 = { .zB = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zaddress = UINT64_C(0xdeadc0de), .zflag_cap_mode = false, .zglobal = false, .zinternal_E = false, .zotype = UINT64_C(0xdeadc0de), .zpermit_cinvoke = false, .zpermit_execute = false, .zpermit_load = false, .zpermit_load_cap = false, .zpermit_seal = false, .zpermit_set_CID = false, .zpermit_store = false, .zpermit_store_cap = false, .zpermit_store_local_cap = false, .zpermit_unseal = false, .zreserved = UINT64_C(0xdeadc0de), .ztag = false, .zuperms = UINT64_C(0xdeadc0de) };
  return zcbz358;
}



static void finish_sailgen_clearTagIfSealed(void)
{
}

static bool sailgen_capBoundsEqual(struct zCapability, struct zCapability);

sail_int zghz3304;
sail_int zghz3305;
sail_int zghz3306;
sail_int zghz3307;

static void startup_sailgen_capBoundsEqual(void)
{
  CREATE(sail_int)(&zghz3304);
  CREATE(sail_int)(&zghz3305);
  CREATE(sail_int)(&zghz3306);
  CREATE(sail_int)(&zghz3307);
}

static bool sailgen_capBoundsEqual(struct zCapability zc1, struct zCapability zc2)
{
  __label__ case_162, finish_match_161, end_function_165, end_block_exception_166;

  bool zcbz339;
  struct ztuple_z8z5izCz0z5iz9 zgaz3292;
  CREATE(ztuple_z8z5izCz0z5iz9)(&zgaz3292);
  sailgen_getCapBounds(&zgaz3292, zc1);
  {
    __label__ case_164, finish_match_163;

    RECREATE(sail_int)(&zghz3304);
    COPY(sail_int)(&zghz3304, zgaz3292.ztup0);
    RECREATE(sail_int)(&zghz3305);
    COPY(sail_int)(&zghz3305, zgaz3292.ztup1);
    struct ztuple_z8z5izCz0z5iz9 zgaz3293;
    CREATE(ztuple_z8z5izCz0z5iz9)(&zgaz3293);
    sailgen_getCapBounds(&zgaz3293, zc2);
    bool zgsz384;
    {
      RECREATE(sail_int)(&zghz3306);
      COPY(sail_int)(&zghz3306, zgaz3293.ztup0);
      RECREATE(sail_int)(&zghz3307);
      COPY(sail_int)(&zghz3307, zgaz3293.ztup1);
      bool zgaz3294;
      zgaz3294 = eq_int(zghz3304, zghz3306);
      bool zgsz385;
      if (zgaz3294) {  zgsz385 = eq_int(zghz3305, zghz3307);  } else {  zgsz385 = false;  }
      zgsz384 = zgsz385;
      goto finish_match_163;
    }
  case_164: ;
    sail_match_failure("capBoundsEqual");
  finish_match_163: ;
    zcbz339 = zgsz384;
    KILL(ztuple_z8z5izCz0z5iz9)(&zgaz3293);
    goto finish_match_161;
  }
case_162: ;
  sail_match_failure("capBoundsEqual");
finish_match_161: ;
  KILL(ztuple_z8z5izCz0z5iz9)(&zgaz3292);
end_function_165: ;
  return zcbz339;
end_block_exception_166: ;

  return false;
}



static void finish_sailgen_capBoundsEqual(void)
{
  KILL(sail_int)(&zghz3307);
  KILL(sail_int)(&zghz3306);
  KILL(sail_int)(&zghz3305);
  KILL(sail_int)(&zghz3304);
}

static struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 sailgen_setCapAddr(struct zCapability, uint64_t);

struct zCapability zghz3308;

static void startup_sailgen_setCapAddr(void)
{
}

static struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 sailgen_setCapAddr(struct zCapability zc, uint64_t zaddr)
{
  __label__ end_function_168, end_block_exception_169;

  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz340;
  struct zCapability zghz3308;
  {
    zghz3308 = zc;
    zghz3308.zaddress = zaddr;
  }
  bool zrepresentable;
  zrepresentable = sailgen_capBoundsEqual(zc, zghz3308);
  zcbz340.ztup0 = zrepresentable;
  zcbz340.ztup1 = zghz3308;

end_function_168: ;
  return zcbz340;
end_block_exception_169: ;
  struct zCapability zcbz360 = { .zB = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zaddress = UINT64_C(0xdeadc0de), .zflag_cap_mode = false, .zglobal = false, .zinternal_E = false, .zotype = UINT64_C(0xdeadc0de), .zpermit_cinvoke = false, .zpermit_execute = false, .zpermit_load = false, .zpermit_load_cap = false, .zpermit_seal = false, .zpermit_set_CID = false, .zpermit_store = false, .zpermit_store_cap = false, .zpermit_store_local_cap = false, .zpermit_unseal = false, .zreserved = UINT64_C(0xdeadc0de), .ztag = false, .zuperms = UINT64_C(0xdeadc0de) };
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz359 = { .ztup0 = false, .ztup1 = zcbz360 };
  return zcbz359;
}



static void finish_sailgen_setCapAddr(void)
{
}

static bool sailgen_fastRepCheck(struct zCapability, uint64_t);

lbits zghz3309;
lbits zghz3310;
lbits zghz3311;
lbits zghz3312;
lbits zghz3313;
sail_int zghz3314;
lbits zghz3315;
sail_int zghz3316;
lbits zghz3317;
sail_int zghz3318;
sail_int zghz3319;
sail_int zghz3320;
lbits zghz3321;
sail_int zghz3322;
lbits zghz3323;
lbits zghz3324;
sail_int zghz3325;
lbits zghz3326;
lbits zghz3327;
sail_int zghz3328;
lbits zghz3329;
lbits zghz3330;
sail_int zghz3331;
lbits zghz3332;
lbits zghz3333;
sail_int zghz3334;
lbits zghz3335;
lbits zghz3336;
sail_int zghz3337;
lbits zghz3338;
sail_int zghz3339;
lbits zghz3340;
sail_int zghz3341;
sail_int zghz3342;
sail_int zghz3343;
lbits zghz3344;
sail_int zghz3345;

static void startup_sailgen_fastRepCheck(void)
{
  CREATE(lbits)(&zghz3309);
  CREATE(lbits)(&zghz3310);
  CREATE(lbits)(&zghz3311);
  CREATE(lbits)(&zghz3312);
  CREATE(lbits)(&zghz3313);
  CREATE(sail_int)(&zghz3314);
  CREATE(lbits)(&zghz3315);
  CREATE(sail_int)(&zghz3316);
  CREATE(lbits)(&zghz3317);
  CREATE(sail_int)(&zghz3318);
  CREATE(sail_int)(&zghz3319);
  CREATE(sail_int)(&zghz3320);
  CREATE(lbits)(&zghz3321);
  CREATE(sail_int)(&zghz3322);
  CREATE(lbits)(&zghz3323);
  CREATE(lbits)(&zghz3324);
  CREATE(sail_int)(&zghz3325);
  CREATE(lbits)(&zghz3326);
  CREATE(lbits)(&zghz3327);
  CREATE(sail_int)(&zghz3328);
  CREATE(lbits)(&zghz3329);
  CREATE(lbits)(&zghz3330);
  CREATE(sail_int)(&zghz3331);
  CREATE(lbits)(&zghz3332);
  CREATE(lbits)(&zghz3333);
  CREATE(sail_int)(&zghz3334);
  CREATE(lbits)(&zghz3335);
  CREATE(lbits)(&zghz3336);
  CREATE(sail_int)(&zghz3337);
  CREATE(lbits)(&zghz3338);
  CREATE(sail_int)(&zghz3339);
  CREATE(lbits)(&zghz3340);
  CREATE(sail_int)(&zghz3341);
  CREATE(sail_int)(&zghz3342);
  CREATE(sail_int)(&zghz3343);
  CREATE(lbits)(&zghz3344);
  CREATE(sail_int)(&zghz3345);
}

static bool sailgen_fastRepCheck(struct zCapability zc, uint64_t zi)
{
  __label__ end_function_171, end_block_exception_172;

  bool zcbz341;
  int64_t zE;
  {
    uint64_t zgaz3308;
    zgaz3308 = zc.zE;
    {
      RECREATE(lbits)(&zghz3344);
      CONVERT_OF(lbits, fbits)(&zghz3344, zgaz3308, UINT64_C(6) , true);
      RECREATE(sail_int)(&zghz3345);
      sail_unsigned(&zghz3345, zghz3344);
      zE = CONVERT_OF(mach_int, sail_int)(zghz3345);
    }
  }
  bool zgaz3296;
  {
    int64_t zgaz3295;
    {
      RECREATE(sail_int)(&zghz3341);
      CONVERT_OF(sail_int, mach_int)(&zghz3341, zcap_max_E);
      RECREATE(sail_int)(&zghz3342);
      CONVERT_OF(sail_int, mach_int)(&zghz3342, INT64_C(2));
      RECREATE(sail_int)(&zghz3343);
      sub_int(&zghz3343, zghz3341, zghz3342);
      zgaz3295 = CONVERT_OF(mach_int, sail_int)(zghz3343);
    }
    zgaz3296 = (zE >= zgaz3295);
  }
  if (zgaz3296) {  zcbz341 = true;  } else {
    int64_t zi_top;
    {
      uint64_t zgaz3307;
      {
        int64_t zgaz3306;
        zgaz3306 = (zE + zcap_mantissa_width);
        {
          RECREATE(lbits)(&zghz3338);
          CONVERT_OF(lbits, fbits)(&zghz3338, zi, UINT64_C(64) , true);
          RECREATE(sail_int)(&zghz3339);
          CONVERT_OF(sail_int, mach_int)(&zghz3339, zgaz3306);
          RECREATE(lbits)(&zghz3340);
          arith_shiftr(&zghz3340, zghz3338, zghz3339);
          zgaz3307 = CONVERT_OF(fbits, lbits)(zghz3340, true);
        }
      }
      {
        RECREATE(lbits)(&zghz3336);
        CONVERT_OF(lbits, fbits)(&zghz3336, zgaz3307, UINT64_C(64) , true);
        RECREATE(sail_int)(&zghz3337);
        sail_signed(&zghz3337, zghz3336);
        zi_top = CONVERT_OF(mach_int, sail_int)(zghz3337);
      }
    }
    uint64_t zi_mid;
    {
      uint64_t zgaz3305;
      {
        RECREATE(lbits)(&zghz3333);
        CONVERT_OF(lbits, fbits)(&zghz3333, zi, UINT64_C(64) , true);
        RECREATE(sail_int)(&zghz3334);
        CONVERT_OF(sail_int, mach_int)(&zghz3334, zE);
        RECREATE(lbits)(&zghz3335);
        shiftr(&zghz3335, zghz3333, zghz3334);
        zgaz3305 = CONVERT_OF(fbits, lbits)(zghz3335, true);
      }
      {
        RECREATE(lbits)(&zghz3330);
        CONVERT_OF(lbits, fbits)(&zghz3330, zgaz3305, UINT64_C(64) , true);
        RECREATE(sail_int)(&zghz3331);
        CONVERT_OF(sail_int, mach_int)(&zghz3331, zcap_mantissa_width);
        RECREATE(lbits)(&zghz3332);
        sail_truncate(&zghz3332, zghz3330, zghz3331);
        zi_mid = CONVERT_OF(fbits, lbits)(zghz3332, true);
      }
    }
    uint64_t za_mid;
    {
      uint64_t zgaz3304;
      {
        uint64_t zgaz3303;
        zgaz3303 = zc.zaddress;
        {
          RECREATE(lbits)(&zghz3327);
          CONVERT_OF(lbits, fbits)(&zghz3327, zgaz3303, UINT64_C(64) , true);
          RECREATE(sail_int)(&zghz3328);
          CONVERT_OF(sail_int, mach_int)(&zghz3328, zE);
          RECREATE(lbits)(&zghz3329);
          shiftr(&zghz3329, zghz3327, zghz3328);
          zgaz3304 = CONVERT_OF(fbits, lbits)(zghz3329, true);
        }
      }
      {
        RECREATE(lbits)(&zghz3324);
        CONVERT_OF(lbits, fbits)(&zghz3324, zgaz3304, UINT64_C(64) , true);
        RECREATE(sail_int)(&zghz3325);
        CONVERT_OF(sail_int, mach_int)(&zghz3325, zcap_mantissa_width);
        RECREATE(lbits)(&zghz3326);
        sail_truncate(&zghz3326, zghz3324, zghz3325);
        za_mid = CONVERT_OF(fbits, lbits)(zghz3326, true);
      }
    }
    uint64_t zB3;
    {
      uint64_t zgaz3302;
      zgaz3302 = zc.zB;
      {
        RECREATE(lbits)(&zghz3321);
        CONVERT_OF(lbits, fbits)(&zghz3321, zgaz3302, UINT64_C(14) , true);
        RECREATE(sail_int)(&zghz3322);
        CONVERT_OF(sail_int, mach_int)(&zghz3322, INT64_C(3));
        RECREATE(lbits)(&zghz3323);
        sail_truncateLSB(&zghz3323, zghz3321, zghz3322);
        zB3 = CONVERT_OF(fbits, lbits)(zghz3323, true);
      }
    }
    uint64_t zR3;
    zR3 = ((zB3 - UINT64_C(0b001)) & UINT64_C(0b111));
    uint64_t zR;
    {
      uint64_t zgaz3301;
      {
        int64_t zgaz3300;
        {
          RECREATE(sail_int)(&zghz3318);
          CONVERT_OF(sail_int, mach_int)(&zghz3318, zcap_mantissa_width);
          RECREATE(sail_int)(&zghz3319);
          CONVERT_OF(sail_int, mach_int)(&zghz3319, INT64_C(3));
          RECREATE(sail_int)(&zghz3320);
          sub_int(&zghz3320, zghz3318, zghz3319);
          zgaz3300 = CONVERT_OF(mach_int, sail_int)(zghz3320);
        }
        {
          RECREATE(sail_int)(&zghz3316);
          CONVERT_OF(sail_int, mach_int)(&zghz3316, zgaz3300);
          RECREATE(lbits)(&zghz3317);
          sailgen_zzeros_implicit(&zghz3317, zghz3316);
          zgaz3301 = CONVERT_OF(fbits, lbits)(zghz3317, true);
        }
      }
      zR = (zR3 << 11) | zgaz3301;
    }
    uint64_t zdiff;
    zdiff = ((zR - za_mid) & UINT64_C(0b11111111111111));
    uint64_t zdiff1;
    {
      RECREATE(lbits)(&zghz3313);
      CONVERT_OF(lbits, fbits)(&zghz3313, zdiff, UINT64_C(14) , true);
      RECREATE(sail_int)(&zghz3314);
      CONVERT_OF(sail_int, mach_int)(&zghz3314, INT64_C(1));
      RECREATE(lbits)(&zghz3315);
      sub_bits_int(&zghz3315, zghz3313, zghz3314);
      zdiff1 = CONVERT_OF(fbits, lbits)(zghz3315, true);
    }
    bool zgaz3297;
    zgaz3297 = (zi_top == INT64_C(0));
    if (zgaz3297) {
      {
        RECREATE(lbits)(&zghz3311);
        CONVERT_OF(lbits, fbits)(&zghz3311, zi_mid, UINT64_C(14) , true);
        RECREATE(lbits)(&zghz3312);
        CONVERT_OF(lbits, fbits)(&zghz3312, zdiff1, UINT64_C(14) , true);
        zcbz341 = sailgen_z8operatorz0zI_uz9(zghz3311, zghz3312);
      }
    } else {
      bool zgaz3298;
      zgaz3298 = (zi_top == INT64_C(-1));
      if (zgaz3298) {
        bool zgaz3299;
        {
          RECREATE(lbits)(&zghz3309);
          CONVERT_OF(lbits, fbits)(&zghz3309, zi_mid, UINT64_C(14) , true);
          RECREATE(lbits)(&zghz3310);
          CONVERT_OF(lbits, fbits)(&zghz3310, zdiff, UINT64_C(14) , true);
          zgaz3299 = sailgen_z8operatorz0zKzJ_uz9(zghz3309, zghz3310);
        }
        bool zgsz390;
        if (zgaz3299) {  zgsz390 = (zR != za_mid);  } else {  zgsz390 = false;  }
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
  KILL(sail_int)(&zghz3345);
  KILL(lbits)(&zghz3344);
  KILL(sail_int)(&zghz3343);
  KILL(sail_int)(&zghz3342);
  KILL(sail_int)(&zghz3341);
  KILL(lbits)(&zghz3340);
  KILL(sail_int)(&zghz3339);
  KILL(lbits)(&zghz3338);
  KILL(sail_int)(&zghz3337);
  KILL(lbits)(&zghz3336);
  KILL(lbits)(&zghz3335);
  KILL(sail_int)(&zghz3334);
  KILL(lbits)(&zghz3333);
  KILL(lbits)(&zghz3332);
  KILL(sail_int)(&zghz3331);
  KILL(lbits)(&zghz3330);
  KILL(lbits)(&zghz3329);
  KILL(sail_int)(&zghz3328);
  KILL(lbits)(&zghz3327);
  KILL(lbits)(&zghz3326);
  KILL(sail_int)(&zghz3325);
  KILL(lbits)(&zghz3324);
  KILL(lbits)(&zghz3323);
  KILL(sail_int)(&zghz3322);
  KILL(lbits)(&zghz3321);
  KILL(sail_int)(&zghz3320);
  KILL(sail_int)(&zghz3319);
  KILL(sail_int)(&zghz3318);
  KILL(lbits)(&zghz3317);
  KILL(sail_int)(&zghz3316);
  KILL(lbits)(&zghz3315);
  KILL(sail_int)(&zghz3314);
  KILL(lbits)(&zghz3313);
  KILL(lbits)(&zghz3312);
  KILL(lbits)(&zghz3311);
  KILL(lbits)(&zghz3310);
  KILL(lbits)(&zghz3309);
}

static void sailgen_capToString(sail_string *rop, struct zCapability);

sail_int zghz3346;
lbits zghz3347;
lbits zghz3348;
lbits zghz3349;
lbits zghz3350;
sail_int zghz3351;
lbits zghz3352;
lbits zghz3353;
sail_int zghz3354;
lbits zghz3355;
lbits zghz3356;
lbits zghz3357;
sail_int zghz3358;

static void startup_sailgen_capToString(void)
{
  CREATE(sail_int)(&zghz3346);
  CREATE(lbits)(&zghz3347);
  CREATE(lbits)(&zghz3348);
  CREATE(lbits)(&zghz3349);
  CREATE(lbits)(&zghz3350);
  CREATE(sail_int)(&zghz3351);
  CREATE(lbits)(&zghz3352);
  CREATE(lbits)(&zghz3353);
  CREATE(sail_int)(&zghz3354);
  CREATE(lbits)(&zghz3355);
  CREATE(lbits)(&zghz3356);
  CREATE(lbits)(&zghz3357);
  CREATE(sail_int)(&zghz3358);
}

static void sailgen_capToString(sail_string *zcbz342, struct zCapability zcap)
{
  __label__ end_function_174, end_block_exception_175, end_function_201;

  RECREATE(sail_int)(&zghz3346);
  sailgen_getCapLength(&zghz3346, zcap);
  sail_string zlen_str;
  CREATE(sail_string)(&zlen_str);
  {
    RECREATE(lbits)(&zghz3357);
    {
      int64_t zgaz3336;
      zgaz3336 = (zcap_len_width + INT64_C(3));
      {
        RECREATE(sail_int)(&zghz3358);
        CONVERT_OF(sail_int, mach_int)(&zghz3358, zgaz3336);
        sailgen_to_bits(&zghz3357, zghz3358, zghz3346);
      }
    }
    string_of_lbits(&zlen_str, zghz3357);
  }
  uint64_t zotype64;
  {
    bool zgaz3333;
    zgaz3333 = sailgen_hasReservedOType(zcap);
    if (zgaz3333) {
      uint64_t zgaz3334;
      zgaz3334 = zcap.zotype;
      {
        RECREATE(sail_int)(&zghz3354);
        CONVERT_OF(sail_int, mach_int)(&zghz3354, INT64_C(64));
        RECREATE(lbits)(&zghz3355);
        CONVERT_OF(lbits, fbits)(&zghz3355, zgaz3334, UINT64_C(18) , true);
        RECREATE(lbits)(&zghz3356);
        sailgen_sign_extend(&zghz3356, zghz3354, zghz3355);
        zotype64 = CONVERT_OF(fbits, lbits)(zghz3356, true);
      }
    } else {
      uint64_t zgaz3335;
      zgaz3335 = zcap.zotype;
      {
        RECREATE(sail_int)(&zghz3351);
        CONVERT_OF(sail_int, mach_int)(&zghz3351, INT64_C(64));
        RECREATE(lbits)(&zghz3352);
        CONVERT_OF(lbits, fbits)(&zghz3352, zgaz3335, UINT64_C(18) , true);
        RECREATE(lbits)(&zghz3353);
        sailgen_zzero_extend(&zghz3353, zghz3351, zghz3352);
        zotype64 = CONVERT_OF(fbits, lbits)(zghz3353, true);
      }
    }
  }
  sail_string zgaz3332;
  CREATE(sail_string)(&zgaz3332);
  {
    sail_string zgaz3330;
    CREATE(sail_string)(&zgaz3330);
    {
      bool zgaz3309;
      zgaz3309 = zcap.ztag;
      if (zgaz3309) {  COPY(sail_string)(&zgaz3330, "1");  } else {  COPY(sail_string)(&zgaz3330, "0");  }
    }
    sail_string zgaz3331;
    CREATE(sail_string)(&zgaz3331);
    {
      sail_string zgaz3329;
      CREATE(sail_string)(&zgaz3329);
      {
        sail_string zgaz3327;
        CREATE(sail_string)(&zgaz3327);
        {
          bool zgaz3310;
          zgaz3310 = sailgen_isCapSealed(zcap);
          if (zgaz3310) {  COPY(sail_string)(&zgaz3327, "1");  } else {  COPY(sail_string)(&zgaz3327, "0");  }
        }
        sail_string zgaz3328;
        CREATE(sail_string)(&zgaz3328);
        {
          sail_string zgaz3326;
          CREATE(sail_string)(&zgaz3326);
          {
            sail_string zgaz3324;
            CREATE(sail_string)(&zgaz3324);
            {
              uint64_t zgaz3312;
              {
                uint64_t zgaz3311;
                zgaz3311 = sailgen_getCapPerms(zcap);
                zgaz3312 = (UINT64_C(0b0) << 19) | zgaz3311;
              }
              {
                RECREATE(lbits)(&zghz3350);
                CONVERT_OF(lbits, fbits)(&zghz3350, zgaz3312, UINT64_C(20) , true);
                string_of_lbits(&zgaz3324, zghz3350);
              }
            }
            sail_string zgaz3325;
            CREATE(sail_string)(&zgaz3325);
            {
              sail_string zgaz3323;
              CREATE(sail_string)(&zgaz3323);
              {
                sail_string zgaz3321;
                CREATE(sail_string)(&zgaz3321);
                {
                  RECREATE(lbits)(&zghz3349);
                  CONVERT_OF(lbits, fbits)(&zghz3349, zotype64, UINT64_C(64) , true);
                  string_of_lbits(&zgaz3321, zghz3349);
                }
                sail_string zgaz3322;
                CREATE(sail_string)(&zgaz3322);
                {
                  sail_string zgaz3320;
                  CREATE(sail_string)(&zgaz3320);
                  {
                    sail_string zgaz3318;
                    CREATE(sail_string)(&zgaz3318);
                    {
                      uint64_t zgaz3313;
                      zgaz3313 = zcap.zaddress;
                      {
                        RECREATE(lbits)(&zghz3348);
                        CONVERT_OF(lbits, fbits)(&zghz3348, zgaz3313, UINT64_C(64) , true);
                        string_of_lbits(&zgaz3318, zghz3348);
                      }
                    }
                    sail_string zgaz3319;
                    CREATE(sail_string)(&zgaz3319);
                    {
                      sail_string zgaz3317;
                      CREATE(sail_string)(&zgaz3317);
                      {
                        sail_string zgaz3315;
                        CREATE(sail_string)(&zgaz3315);
                        {
                          uint64_t zgaz3314;
                          zgaz3314 = sailgen_getCapBaseBits(zcap);
                          {
                            RECREATE(lbits)(&zghz3347);
                            CONVERT_OF(lbits, fbits)(&zghz3347, zgaz3314, UINT64_C(64) , true);
                            string_of_lbits(&zgaz3315, zghz3347);
                          }
                        }
                        sail_string zgaz3316;
                        CREATE(sail_string)(&zgaz3316);
                        concat_str(&zgaz3316, " length:", zlen_str);
                        concat_str(&zgaz3317, zgaz3315, zgaz3316);
                        KILL(sail_string)(&zgaz3316);
                        KILL(sail_string)(&zgaz3315);
                      }
                      concat_str(&zgaz3319, " base:", zgaz3317);
                      KILL(sail_string)(&zgaz3317);
                    }
                    concat_str(&zgaz3320, zgaz3318, zgaz3319);
                    KILL(sail_string)(&zgaz3319);
                    KILL(sail_string)(&zgaz3318);
                  }
                  concat_str(&zgaz3322, " address:", zgaz3320);
                  KILL(sail_string)(&zgaz3320);
                }
                concat_str(&zgaz3323, zgaz3321, zgaz3322);
                KILL(sail_string)(&zgaz3322);
                KILL(sail_string)(&zgaz3321);
              }
              concat_str(&zgaz3325, " type:", zgaz3323);
              KILL(sail_string)(&zgaz3323);
            }
            concat_str(&zgaz3326, zgaz3324, zgaz3325);
            KILL(sail_string)(&zgaz3325);
            KILL(sail_string)(&zgaz3324);
          }
          concat_str(&zgaz3328, " perms:", zgaz3326);
          KILL(sail_string)(&zgaz3326);
        }
        concat_str(&zgaz3329, zgaz3327, zgaz3328);
        KILL(sail_string)(&zgaz3328);
        KILL(sail_string)(&zgaz3327);
      }
      concat_str(&zgaz3331, " s:", zgaz3329);
      KILL(sail_string)(&zgaz3329);
    }
    concat_str(&zgaz3332, zgaz3330, zgaz3331);
    KILL(sail_string)(&zgaz3331);
    KILL(sail_string)(&zgaz3330);
  }
  concat_str((*(&zcbz342)), " t:", zgaz3332);
  KILL(sail_string)(&zgaz3332);

  KILL(sail_string)(&zlen_str);
end_function_174: ;
  goto end_function_201;
end_block_exception_175: ;
  goto end_function_201;
end_function_201: ;
}



static void finish_sailgen_capToString(void)
{
  KILL(sail_int)(&zghz3358);
  KILL(lbits)(&zghz3357);
  KILL(lbits)(&zghz3356);
  KILL(lbits)(&zghz3355);
  KILL(sail_int)(&zghz3354);
  KILL(lbits)(&zghz3353);
  KILL(lbits)(&zghz3352);
  KILL(sail_int)(&zghz3351);
  KILL(lbits)(&zghz3350);
  KILL(lbits)(&zghz3349);
  KILL(lbits)(&zghz3348);
  KILL(lbits)(&zghz3347);
  KILL(sail_int)(&zghz3346);
}

static uint64_t sailgen_getRepresentableAlignmentMask(uint64_t);

struct zCapability zghz3359;
sail_int zghz3360;
lbits zghz3361;
sail_int zghz3362;
lbits zghz3363;
sail_int zghz3364;
sail_int zghz3365;
sail_int zghz3366;
sail_int zghz3367;
sail_int zghz3368;
sail_int zghz3369;
lbits zghz3370;
sail_int zghz3371;
lbits zghz3372;
lbits zghz3373;
lbits zghz3374;

static void startup_sailgen_getRepresentableAlignmentMask(void)
{

  CREATE(sail_int)(&zghz3360);
  CREATE(lbits)(&zghz3361);
  CREATE(sail_int)(&zghz3362);
  CREATE(lbits)(&zghz3363);
  CREATE(sail_int)(&zghz3364);
  CREATE(sail_int)(&zghz3365);
  CREATE(sail_int)(&zghz3366);
  CREATE(sail_int)(&zghz3367);
  CREATE(sail_int)(&zghz3368);
  CREATE(sail_int)(&zghz3369);
  CREATE(lbits)(&zghz3370);
  CREATE(sail_int)(&zghz3371);
  CREATE(lbits)(&zghz3372);
  CREATE(lbits)(&zghz3373);
  CREATE(lbits)(&zghz3374);
}

static uint64_t sailgen_getRepresentableAlignmentMask(uint64_t zlen)
{
  __label__ case_178, finish_match_177, end_function_179, end_block_exception_180;

  uint64_t zcbz343;
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3339;
  {
    RECREATE(lbits)(&zghz3372);
    {
      RECREATE(lbits)(&zghz3373);
      CONVERT_OF(lbits, fbits)(&zghz3373, UINT64_C(0b0), UINT64_C(1) , true);
      RECREATE(lbits)(&zghz3374);
      CONVERT_OF(lbits, fbits)(&zghz3374, zlen, UINT64_C(64) , true);
      append(&zghz3372, zghz3373, zghz3374);
    }
    zgaz3339 = sailgen_setCapBounds(zdefault_cap, UINT64_C(0x0000000000000000), zghz3372);
  }
  {
    bool zexact;
    zexact = zgaz3339.ztup0;
    struct zCapability zghz3359;
    zghz3359 = zgaz3339.ztup1;
    int64_t ze;
    {
      int64_t zgaz3345;
      {
        uint64_t zgaz3344;
        zgaz3344 = zghz3359.zE;
        {
          RECREATE(lbits)(&zghz3370);
          CONVERT_OF(lbits, fbits)(&zghz3370, zgaz3344, UINT64_C(6) , true);
          RECREATE(sail_int)(&zghz3371);
          sail_unsigned(&zghz3371, zghz3370);
          zgaz3345 = CONVERT_OF(mach_int, sail_int)(zghz3371);
        }
      }
      {
        RECREATE(sail_int)(&zghz3367);
        CONVERT_OF(sail_int, mach_int)(&zghz3367, zgaz3345);
        RECREATE(sail_int)(&zghz3368);
        CONVERT_OF(sail_int, mach_int)(&zghz3368, zcap_max_E);
        RECREATE(sail_int)(&zghz3369);
        min_int(&zghz3369, zghz3367, zghz3368);
        ze = CONVERT_OF(mach_int, sail_int)(zghz3369);
      }
    }
    int64_t zez7;
    {
      bool zgaz3343;
      zgaz3343 = zghz3359.zinternal_E;
      if (zgaz3343) {  zez7 = (ze + zinternal_E_take_bits);  } else {  zez7 = INT64_C(0);  }
    }
    sbits zgaz3341;
    {
      int64_t zgaz3340;
      {
        RECREATE(sail_int)(&zghz3364);
        CONVERT_OF(sail_int, mach_int)(&zghz3364, INT64_C(64));
        RECREATE(sail_int)(&zghz3365);
        CONVERT_OF(sail_int, mach_int)(&zghz3365, zez7);
        RECREATE(sail_int)(&zghz3366);
        sub_int(&zghz3366, zghz3364, zghz3365);
        zgaz3340 = CONVERT_OF(mach_int, sail_int)(zghz3366);
      }
      {
        RECREATE(sail_int)(&zghz3362);
        CONVERT_OF(sail_int, mach_int)(&zghz3362, zgaz3340);
        RECREATE(lbits)(&zghz3363);
        sailgen_ones(&zghz3363, zghz3362);
        zgaz3341 = CONVERT_OF(sbits, lbits)(zghz3363, true);
      }
    }
    sbits zgaz3342;
    {
      RECREATE(sail_int)(&zghz3360);
      CONVERT_OF(sail_int, mach_int)(&zghz3360, zez7);
      RECREATE(lbits)(&zghz3361);
      sailgen_zzeros_implicit(&zghz3361, zghz3360);
      zgaz3342 = CONVERT_OF(sbits, lbits)(zghz3361, true);
    }
    uint64_t zgsz392;
    zgsz392 = CONVERT_OF(fbits, sbits)(append_ss(zgaz3341, zgaz3342), true);
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
  KILL(lbits)(&zghz3374);
  KILL(lbits)(&zghz3373);
  KILL(lbits)(&zghz3372);
  KILL(sail_int)(&zghz3371);
  KILL(lbits)(&zghz3370);
  KILL(sail_int)(&zghz3369);
  KILL(sail_int)(&zghz3368);
  KILL(sail_int)(&zghz3367);
  KILL(sail_int)(&zghz3366);
  KILL(sail_int)(&zghz3365);
  KILL(sail_int)(&zghz3364);
  KILL(lbits)(&zghz3363);
  KILL(sail_int)(&zghz3362);
  KILL(lbits)(&zghz3361);
  KILL(sail_int)(&zghz3360);

}

static uint64_t sailgen_getRepresentableLength(uint64_t);

static uint64_t sailgen_getRepresentableLength(uint64_t zlen)
{
  __label__ end_function_182, end_block_exception_183;

  uint64_t zcbz344;
  uint64_t zm;
  zm = sailgen_getRepresentableAlignmentMask(zlen);
  uint64_t zgaz3347;
  {
    uint64_t zgaz3346;
    zgaz3346 = (~(zm) & UINT64_C(0xFFFFFFFFFFFFFFFF));
    zgaz3347 = ((zlen + zgaz3346) & UINT64_C(0xFFFFFFFFFFFFFFFF));
  }
  zcbz344 = (zgaz3347 & zm);


end_function_182: ;
  return zcbz344;
end_block_exception_183: ;

  return UINT64_C(0xdeadc0de);
}

static struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 sailgen_doCSetBounds(struct zCapability, lbits);

lbits zghz3375;
struct zCapability zghz3376;
struct zCapability zghz3377;
struct zCapability zghz3378;
sail_int zghz3379;
lbits zghz3380;
sail_int zghz3381;
lbits zghz3382;

static void startup_sailgen_doCSetBounds(void)
{
  CREATE(lbits)(&zghz3375);



  CREATE(sail_int)(&zghz3379);
  CREATE(lbits)(&zghz3380);
  CREATE(sail_int)(&zghz3381);
  CREATE(lbits)(&zghz3382);
}

static struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 sailgen_doCSetBounds(struct zCapability zinput, lbits zreq_len)
{
  __label__ case_186, finish_match_185, end_function_187, end_block_exception_188;

  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz345;
  uint64_t znewBase;
  znewBase = zinput.zaddress;
  RECREATE(lbits)(&zghz3375);
  {
    RECREATE(lbits)(&zghz3380);
    {
      RECREATE(sail_int)(&zghz3381);
      CONVERT_OF(sail_int, mach_int)(&zghz3381, INT64_C(65));
      RECREATE(lbits)(&zghz3382);
      CONVERT_OF(lbits, fbits)(&zghz3382, znewBase, UINT64_C(64) , true);
      sailgen_zzero_extend(&zghz3380, zghz3381, zghz3382);
    }
    add_bits(&zghz3375, zghz3380, zreq_len);
  }
  bool zinBounds;
  {
    RECREATE(sail_int)(&zghz3379);
    sail_unsigned(&zghz3379, zreq_len);
    zinBounds = sailgen_inCapBounds(zinput, znewBase, zghz3379);
  }
  struct zCapability zghz3376;
  zghz3376 = sailgen_clearTagIfSealed(zinput);
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3348;
  zgaz3348 = sailgen_setCapBounds(zghz3376, znewBase, zghz3375);
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgsz394;
  {
    bool zexact;
    zexact = zgaz3348.ztup0;
    struct zCapability zghz3377;
    zghz3377 = zgaz3348.ztup1;
    struct zCapability zghz3378;
    {
      bool zgaz3349;
      zgaz3349 = sailgen_not(zinBounds);
      zghz3378 = sailgen_clearTagIf(zghz3377, zgaz3349);
    }
    zgsz394.ztup0 = zexact;
    zgsz394.ztup1 = zghz3378;
    goto finish_match_185;
  }
case_186: ;
  sail_match_failure("doCSetBounds");
finish_match_185: ;
  zcbz345 = zgsz394;




end_function_187: ;
  return zcbz345;
end_block_exception_188: ;
  struct zCapability zcbz362 = { .zB = UINT64_C(0xdeadc0de), .zE = UINT64_C(0xdeadc0de), .zT = UINT64_C(0xdeadc0de), .zaccess_system_regs = false, .zaddress = UINT64_C(0xdeadc0de), .zflag_cap_mode = false, .zglobal = false, .zinternal_E = false, .zotype = UINT64_C(0xdeadc0de), .zpermit_cinvoke = false, .zpermit_execute = false, .zpermit_load = false, .zpermit_load_cap = false, .zpermit_seal = false, .zpermit_set_CID = false, .zpermit_store = false, .zpermit_store_cap = false, .zpermit_store_local_cap = false, .zpermit_unseal = false, .zreserved = UINT64_C(0xdeadc0de), .ztag = false, .zuperms = UINT64_C(0xdeadc0de) };
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zcbz361 = { .ztup0 = false, .ztup1 = zcbz362 };
  return zcbz361;
}



static void finish_sailgen_doCSetBounds(void)
{
  KILL(lbits)(&zghz3382);
  KILL(sail_int)(&zghz3381);
  KILL(lbits)(&zghz3380);
  KILL(sail_int)(&zghz3379);



  KILL(lbits)(&zghz3375);
}

// register TestCap
static struct zCapability zTestCap;

// register TestAddr
static uint64_t zTestAddr;

// register TestLen
static lbits zTestLen;

static unit sailgen_main(unit);

lbits zghz3383;
lbits zghz3384;
struct zCapability zghz3385;
struct zCapability zghz3386;
struct zCapability zghz3387;
struct zCapability zghz3388;
sail_int zghz3389;
sail_int zghz3390;
struct zCapability zghz3391;

static void startup_sailgen_main(void)
{
  CREATE(lbits)(&zghz3383);
  CREATE(lbits)(&zghz3384);




  CREATE(sail_int)(&zghz3389);
  CREATE(sail_int)(&zghz3390);

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
  RECREATE(lbits)(&zghz3383);
  sailgen_capToBits(&zghz3383, zTestCap);
  RECREATE(lbits)(&zghz3384);
  sailgen_capToMemBits(&zghz3384, zTestCap);
  struct zCapability zghz3385;
  {
    bool zgaz3357;
    zgaz3357 = zTestCap.ztag;
    zghz3385 = sailgen_capBitsToCapability(zgaz3357, zghz3383);
  }
  struct zCapability zghz3386;
  {
    bool zgaz3356;
    zgaz3356 = zTestCap.ztag;
    zghz3386 = sailgen_memBitsToCapability(zgaz3356, zghz3384);
  }
  bool zrep;
  zrep = sailgen_fastRepCheck(zTestCap, zTestAddr);
  struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3353;
  zgaz3353 = sailgen_doCSetBounds(zTestCap, zTestLen);
  unit zgsz399;
  {
    __label__ case_193, finish_match_192;

    bool zexact;
    zexact = zgaz3353.ztup0;
    struct zCapability zghz3387;
    zghz3387 = zgaz3353.ztup1;
    struct ztuple_z8z5boolzCz0z5structz0zzCapabilityz9 zgaz3354;
    zgaz3354 = sailgen_setCapAddr(zTestCap, zTestAddr);
    {
      __label__ case_195, finish_match_194;

      bool zrepresentable;
      zrepresentable = zgaz3354.ztup0;
      struct zCapability zghz3388;
      zghz3388 = zgaz3354.ztup1;
      struct ztuple_z8z5izCz0z5iz9 zgaz3355;
      CREATE(ztuple_z8z5izCz0z5iz9)(&zgaz3355);
      sailgen_getCapBounds(&zgaz3355, zTestCap);
      unit zgsz3101;
      {
        RECREATE(sail_int)(&zghz3389);
        COPY(sail_int)(&zghz3389, zgaz3355.ztup0);
        RECREATE(sail_int)(&zghz3390);
        COPY(sail_int)(&zghz3390, zgaz3355.ztup1);
        uint64_t zflags;
        zflags = sailgen_getCapFlags(zTestCap);
        struct zCapability zghz3391;
        zghz3391 = sailgen_setCapPerms(zTestCap, UINT64_C(0b1111111111111111111));
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
      KILL(ztuple_z8z5izCz0z5iz9)(&zgaz3355);
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

  KILL(sail_int)(&zghz3390);
  KILL(sail_int)(&zghz3389);




  KILL(lbits)(&zghz3384);
  KILL(lbits)(&zghz3383);
}

static unit sailgen_initializze_registers(unit);

lbits zghz3392;
struct zCapability zghz3393;

static void startup_sailgen_initializze_registers(void)
{
  CREATE(lbits)(&zghz3392);

}

static unit sailgen_initializze_registers(unit zgsz3105)
{
  __label__ end_function_199, end_block_exception_200;

  unit zcbz347;
  {
    struct zCapability zghz3393;
    zghz3393.zB = UINT64_C(0b00000000000000);
    zghz3393.zE = UINT64_C(0b000000);
    zghz3393.zT = UINT64_C(0b00000000000000);
    zghz3393.zaccess_system_regs = false;
    zghz3393.zaddress = UINT64_C(0x0000000000000000);
    zghz3393.zflag_cap_mode = false;
    zghz3393.zglobal = false;
    zghz3393.zinternal_E = false;
    zghz3393.zotype = UINT64_C(0b000000000000000000);
    zghz3393.zpermit_cinvoke = false;
    zghz3393.zpermit_execute = false;
    zghz3393.zpermit_load = false;
    zghz3393.zpermit_load_cap = false;
    zghz3393.zpermit_seal = false;
    zghz3393.zpermit_set_CID = false;
    zghz3393.zpermit_store = false;
    zghz3393.zpermit_store_cap = false;
    zghz3393.zpermit_store_local_cap = false;
    zghz3393.zpermit_unseal = false;
    zghz3393.zreserved = UINT64_C(0b00);
    zghz3393.ztag = false;
    zghz3393.zuperms = UINT64_C(0x0);
    zTestCap = zghz3393;
    unit zgsz3108;
    zgsz3108 = UNIT;
  }
  {
    zTestAddr = UINT64_C(0x0000000000000000);
    unit zgsz3107;
    zgsz3107 = UNIT;
  }
  RECREATE(lbits)(&zghz3392);
  CONVERT_OF(lbits, fbits)(&zghz3392, UINT64_C(0b0), UINT64_C(1) , true);
  append_64(&zghz3392, zghz3392, UINT64_C(0x0000000000000000));
  COPY(lbits)(&zTestLen, zghz3392);
  zcbz347 = UNIT;
end_function_199: ;
  return zcbz347;
end_block_exception_200: ;

  return UNIT;
}



static void finish_sailgen_initializze_registers(void)
{

  KILL(lbits)(&zghz3392);
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
  startup_sailgen_encCapToBits();
  startup_sailgen_undefined_Capability();
  startup_sailgen_encCapabilityToCapability();
  startup_sailgen_capBitsToCapability();
  startup_sailgen_capToEncCap();
  startup_sailgen_capToBits();
  startup_sailgen_capToMemBits();
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
  finish_sailgen_sail_ones();
  finish_sailgen_to_bits();
  finish_sailgen_z8operatorz0zI_uz9();
  finish_sailgen_z8operatorz0zKzJ_uz9();
  finish_sailgen_MAX();
  finish_sailgen_capBitsToEncCapability();
  finish_sailgen_encCapToBits();
  finish_sailgen_undefined_Capability();
  finish_sailgen_encCapabilityToCapability();
  finish_sailgen_capBitsToCapability();
  finish_sailgen_capToEncCap();
  finish_sailgen_capToBits();
  finish_sailgen_capToMemBits();
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
