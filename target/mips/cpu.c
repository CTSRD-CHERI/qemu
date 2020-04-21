/*
 * QEMU MIPS CPU
 *
 * Copyright (c) 2012 SUSE LINUX Products GmbH
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see
 * <http://www.gnu.org/licenses/lgpl-2.1.html>
 */

#include "qemu/osdep.h"
#include "qapi/error.h"
#include "cpu.h"
#include "internal.h"
#ifdef TARGET_CHERI
#include "cheri_utils.h"
#endif
#include "kvm_mips.h"
#include "qemu/module.h"
#include "sysemu/kvm.h"
#include "exec/gdbstub.h"
#include "exec/exec-all.h"
#include "qemu/error-report.h"

#ifndef TARGET_MIPS64
const char mips_gp_regnames[32][5] = {
    {"zero"}, {"at"},   {"v0"},   {"v1"},   {"a0"},   {"a1"},   {"a2"},   {"a3"},
    {"t0"},   {"t1"},   {"t2"},   {"t3"},   {"t4"},   {"t5"},   {"t6"},   {"t7"},
    {"s0"},   {"s1"},   {"s2"},   {"s3"},   {"s4"},   {"s5"},   {"s6"},   {"s7"},
    {"t8"},   {"t9"},   {"k0"},   {"k1"},   {"gp"},   {"sp"},   {"s8"},   {"ra"
};
#else
// Use n64 register names
const char mips_gp_regnames[32][5] = {
    {"zero"}, {"at"},   {"v0"},   {"v1"},   {"a0"},   {"a1"},   {"a2"},   {"a3"},
    {"a4"},   {"a5"},   {"a6"},   {"a7"},   {"t0"},   {"t1"},   {"t2"},   {"t3"},
    {"s0"},   {"s1"},   {"s2"},   {"s3"},   {"s4"},   {"s5"},   {"s6"},   {"s7"},
    {"t8"},   {"t9"},   {"k0"},   {"k1"},   {"gp"},   {"sp"},   {"s8"},   {"ra"},
};
#endif

const char mips_regnames_HI[4][4] = {
    {"HI0"}, {"HI1"}, {"HI2"}, {"HI3"},
};

const char mips_regnames_LO[4][4] = {
    {"LO0"}, {"LO1"}, {"LO2"}, {"LO3"},
};

const char mips_fregnames[32][4] = {
    {"f0"},  {"f1"},  {"f2"},  {"f3"},  {"f4"},  {"f5"},  {"f6"},  {"f7"},
    {"f8"},  {"f9"},  {"f10"}, {"f11"}, {"f12"}, {"f13"}, {"f14"}, {"f15"},
    {"f16"}, {"f17"}, {"f18"}, {"f19"}, {"f20"}, {"f21"}, {"f22"}, {"f23"},
    {"f24"}, {"f25"}, {"f26"}, {"f27"}, {"f28"}, {"f29"}, {"f30"}, {"f31"},
};

const char mips_msaregnames[64][7] = {
    {"w0.d0"},  {"w0.d1"},  {"w1.d0"},  {"w1.d1"},
    {"w2.d0"},  {"w2.d1"},  {"w3.d0"},  {"w3.d1"},
    {"w4.d0"},  {"w4.d1"},  {"w5.d0"},  {"w5.d1"},
    {"w6.d0"},  {"w6.d1"},  {"w7.d0"},  {"w7.d1"},
    {"w8.d0"},  {"w8.d1"},  {"w9.d0"},  {"w9.d1"},
    {"w10.d0"}, {"w10.d1"}, {"w11.d0"}, {"w11.d1"},
    {"w12.d0"}, {"w12.d1"}, {"w13.d0"}, {"w13.d1"},
    {"w14.d0"}, {"w14.d1"}, {"w15.d0"}, {"w15.d1"},
    {"w16.d0"}, {"w16.d1"}, {"w17.d0"}, {"w17.d1"},
    {"w18.d0"}, {"w18.d1"}, {"w19.d0"}, {"w19.d1"},
    {"w20.d0"}, {"w20.d1"}, {"w21.d0"}, {"w21.d1"},
    {"w22.d0"}, {"w22.d1"}, {"w23.d0"}, {"w23.d1"},
    {"w24.d0"}, {"w24.d1"}, {"w25.d0"}, {"w25.d1"},
    {"w26.d0"}, {"w26.d1"}, {"w27.d0"}, {"w27.d1"},
    {"w28.d0"}, {"w28.d1"}, {"w29.d0"}, {"w29.d1"},
    {"w30.d0"}, {"w30.d1"}, {"w31.d0"}, {"w31.d1"},
};

#if !defined(TARGET_MIPS64)
const char * const mips_mxuregnames[16][7] = {
    {"XR1"},  {"XR2"},  {"XR3"},  {"XR4"},  {"XR5"},  {"XR6"},  {"XR7"},  {"XR8"},
    {"XR9"},  {"XR10"}, {"XR11"}, {"XR12"}, {"XR13"}, {"XR14"}, {"XR15"}, {"MXU_CR"},
};
#endif

/*
 * Names of coprocessor 0 registers.
 */
const char mips_cop0_regnames[32*8][32] = {
/*0*/   {"Index"},        {"MVPControl"},   {"MVPConf0"},     {"MVPConf1"},
        {0},              {0},              {0},              {0},
/*1*/   {"Random"},       {"VPEControl"},   {"VPEConf0"},     {"VPEConf1"},
        {"YQMask"},       {"VPESchedule"},  {"VPEScheFBack"}, {"VPEOpt"},
/*2*/   {"EntryLo0"},     {"TCStatus"},     {"TCBind"},       {"TCRestart"},
        {"TCHalt"},       {"TCContext"},    {"TCSchedule"},   {"TCScheFBack"},
/*3*/   {"EntryLo1"},     {0},              {0},              {0},
        {0},              {0},              {0},              {"TCOpt"},
/*4*/   {"Context"},      {"ContextConfig"}, {"UserLocal"},    {"XContextConfig"},
        {0},              {0},              {0},              {0},
/*5*/   {"PageMask"},     {"PageGrain"},    {"SegCtl0"},      {"SegCtl1"},
        {"SegCtl2"},      {0},              {0},              {0},
/*6*/   {"Wired"},        {"SRSConf0"},     {"SRSConf1"},     {"SRSConf2"},
        {"SRSConf3"},     {"SRSConf4"},     {0},              {0},
/*7*/   {"HWREna"},       {0},              {0},              {0},
        {0},              {0},              {0},              {0},
/*8*/   {"BadVAddr"},     {"BadInstr"},     {"BadInstrP"},    {0},
        {0},              {0},              {0},              {0},
/*9*/   {"Count"},        {0},              {0},              {0},
        {0},              {0},              {0},              {0},
/*10*/  {"EntryHi"},      {0},              {0},              {0},
        {0},              {"MSAAccess"},    {"MSASave"},      {"MSARequest"},
/*11*/  {"Compare"},      {0},              {0},              {0},
        {0},              {0},              {0},              {0},
/*12*/  {"Status"},       {"IntCtl"},       {"SRSCtl"},       {"SRSMap"},
        {"ViewIPL"},      {"SRSMap2"},      {0},              {0},
/*13*/  {"Cause"},        {0},              {0},              {0},
        {"ViewRIPL"},     {"NestedExc"},    {0},              {0},
/*14*/  {"EPC"},          {0},              {"NestedEPC"},    {0},
        {0},              {0},              {0},              {0},
/*15*/  {"PRId"},         {"EBase"},        {"CDMMBase"},     {"CMGCRBase"},
        {0},              {0},              {0},              {0},
/*16*/  {"Config"},       {"Config1"},      {"Config2"},      {"Config3"},
        {"Config4"},      {"Config5"},      {"Config6"},      {"Config7"},
/*17*/  {"LLAddr"},       {"internal_lladdr (virtual)"}, {"internal_llval"}, {"internal_linkedflag"},
        {0},              {0},              {0},              {0},
/*18*/  {"WatchLo"},      {"WatchLo1"},     {"WatchLo2"},     {"WatchLo3"},
        {"WatchLo4"},     {"WatchLo5"},     {"WatchLo6"},     {"WatchLo7"},
/*19*/  {"WatchHi"},      {"WatchHi1"},     {"WatchHi2"},     {"WatchHi3"},
        {"WatchHi4"},     {"WatchHi5"},     {"WatchHi6"},     {"WatchHi7"},
/*20*/  {"XContext"},     {0},              {0},              {0},
        {0},              {0},              {0},              {0},
/*21*/  {0},              {0},              {0},              {0},
        {0},              {0},              {0},              {0},
/*22*/  {0},              {0},              {0},              {0},
        {0},              {0},              {0},              {0},
/*23*/  {"Debug"},        {"TraceControl"}, {"TraceControl2"}, {"UserTraceData"},
        {"TraceIBPC"},    {"TraceDBPC"},    {"Debug2"},       {0},
/*24*/  {"DEPC"},         {0},              {"TraceControl3"}, {"UserTraceData2"},
        {0},              {0},              {0},              {0},
/*25*/  {"PerfCnt"},      {"PerfCnt1"},     {"PerfCnt2"},     {"PerfCnt3"},
        {"PerfCnt4"},     {"PerfCnt5"},     {"PerfCnt6"},     {"PerfCnt7"},
/*26*/  {"ErrCtl"},       {0},              {0},              {0},
        {0},              {0},              {0},              {0},
/*27*/  {"CacheErr"},     {0},              {0},              {0},
        {0},              {0},              {0},              {0},
/*28*/  {"ITagLo"},       {"IDataLo"},      {"DTagLo"},       {"DDataLo"},
        {"L23TagLo"},     {"L23DataLo"},    {0},              {0},
/*29*/  {"ITagHi"},       {"IDataHi"},      {"DTagHi"},       {0},
        {0},              {"L23DataHi"},    {0},              {0},
/*30*/  {"ErrorEPC"},     {0},              {0},              {0},
        {0},              {0},              {0},              {0},
/*31*/  {"DESAVE"},       {0},              {"KScratch1"},    {"KScratch2"},
        {"KScratch3"},    {"KScratch4"},    {"KScratch5"},    {"KScratch6"},
};

#ifdef TARGET_CHERI
const char cheri_gp_regnames[32][4] = {
    {"C00"}, {"C01"}, {"C02"}, {"C03"}, {"C04"}, {"C05"}, {"C06"}, {"C07"},
    {"C08"}, {"C09"}, {"C10"}, {"C11"}, {"C12"}, {"C13"}, {"C14"}, {"C15"},
    {"C16"}, {"C17"}, {"C18"}, {"C19"}, {"C20"}, {"C21"}, {"C22"}, {"C23"},
    {"C24"}, {"C25"}, {"C26"}, {"C27"}, {"C28"}, {"C29"}, {"C30"}, {"C31"},
};

const char mips_cheri_hw_regnames[32][10] = {
    {"DDC"},       {"UserTLS"}, {0},      {0},
    {0},           {0},         {0},      {0},
    {"PrivTLS"},   {0},         {0},      {0},
    {0},           {0},         {0},      {0},
    {0},           {0},         {0},      {0},
    {0},           {0},         {"KR1C"}, {"KR2C"},
    {0},           {0},         {0},      {0},
    {"ErrorEPCC"}, {"KCC"},     {"KDC"},  {"EPCC"},
};
#endif

static void mips_cpu_set_pc(CPUState *cs, vaddr value)
{
    MIPSCPU *cpu = MIPS_CPU(cs);
    CPUMIPSState *env = &cpu->env;

    mips_update_pc(env, value & ~(target_ulong)1, /*can_be_unrepresentable=*/false);
    if (value & 1) {
        env->hflags |= MIPS_HFLAG_M16;
    } else {
        env->hflags &= ~(MIPS_HFLAG_M16);
    }
}

static void mips_cpu_synchronize_from_tb(CPUState *cs, TranslationBlock *tb)
{
    MIPSCPU *cpu = MIPS_CPU(cs);
    CPUMIPSState *env = &cpu->env;

    mips_update_pc(env, tb->pc, /*can_be_unrepresentable=*/false);
    env->hflags &= ~MIPS_HFLAG_BMASK;
    env->hflags |= tb->flags & MIPS_HFLAG_BMASK;
}

static bool mips_cpu_has_work(CPUState *cs)
{
    MIPSCPU *cpu = MIPS_CPU(cs);
    CPUMIPSState *env = &cpu->env;
    bool has_work = false;

    /*
     * Prior to MIPS Release 6 it is implementation dependent if non-enabled
     * interrupts wake-up the CPU, however most of the implementations only
     * check for interrupts that can be taken.
     */
    if ((cs->interrupt_request & CPU_INTERRUPT_HARD) &&
        cpu_mips_hw_interrupts_pending(env)) {
        if (cpu_mips_hw_interrupts_enabled(env) ||
            (env->insn_flags & ISA_MIPS32R6)) {
            has_work = true;
        }
    }

    /* MIPS-MT has the ability to halt the CPU.  */
    if (env->CP0_Config3 & (1 << CP0C3_MT)) {
        /*
         * The QEMU model will issue an _WAKE request whenever the CPUs
         * should be woken up.
         */
        if (cs->interrupt_request & CPU_INTERRUPT_WAKE) {
            has_work = true;
        }

        if (!mips_vpe_active(env)) {
            has_work = false;
        }
    }
    /* MIPS Release 6 has the ability to halt the CPU.  */
    if (env->CP0_Config5 & (1 << CP0C5_VP)) {
        if (cs->interrupt_request & CPU_INTERRUPT_WAKE) {
            has_work = true;
        }
        if (!mips_vp_active(env)) {
            has_work = false;
        }
    }
    return has_work;
}

static void mips_cpu_reset(DeviceState *dev)
{
    CPUState *s = CPU(dev);
    MIPSCPU *cpu = MIPS_CPU(s);
    MIPSCPUClass *mcc = MIPS_CPU_GET_CLASS(cpu);
    CPUMIPSState *env = &cpu->env;

    mcc->parent_reset(dev);

    memset(env, 0, offsetof(CPUMIPSState, end_reset_fields));
#ifdef CONFIG_DEBUG_TCG
    env->active_tc._pc_is_current = true;
#endif

    cpu_state_reset(env);

#ifndef CONFIG_USER_ONLY
    if (kvm_enabled()) {
        kvm_mips_reset_vcpu(cpu);
    }
#endif
}

static void mips_cpu_disas_set_info(CPUState *s, disassemble_info *info)
{
    MIPSCPU *cpu = MIPS_CPU(s);
    CPUMIPSState *env = &cpu->env;

    if (!(env->insn_flags & ISA_NANOMIPS32)) {
#ifdef TARGET_WORDS_BIGENDIAN
        info->print_insn = print_insn_big_mips;
#else
        info->print_insn = print_insn_little_mips;
#endif
    } else {
#if defined(CONFIG_NANOMIPS_DIS)
        info->print_insn = print_insn_nanomips;
#endif
    }
#ifdef TARGET_MIPS64
    // See disas/mips.c
#define bfd_mach_mipsisa64r2           65
    info->mach = bfd_mach_mipsisa64r2;
#endif

}

static void mips_cpu_realizefn(DeviceState *dev, Error **errp)
{
    CPUState *cs = CPU(dev);
    MIPSCPU *cpu = MIPS_CPU(dev);
    MIPSCPUClass *mcc = MIPS_CPU_GET_CLASS(dev);
    Error *local_err = NULL;

    cpu_exec_realizefn(cs, &local_err);
    if (local_err != NULL) {
        error_propagate(errp, local_err);
        return;
    }

#ifdef TARGET_MIPS64
    gdb_register_coprocessor(cs, NULL, NULL, 0, "mips64-cp0.xml", 0);
    gdb_register_coprocessor(cs, NULL, NULL, 0, "mips64-fpu.xml", 0);
    gdb_register_coprocessor(cs, mips_gdb_get_sys_reg, mips_gdb_set_sys_reg,
        1, "mips64-sys.xml", 0);
#else
    gdb_register_coprocessor(cs, NULL, NULL, 0, "mips-cp0.xml", 0);
    gdb_register_coprocessor(cs, NULL, NULL, 0, "mips-fpu.xml", 0);
    gdb_register_coprocessor(cs, mips_gdb_get_sys_reg, mips_gdb_set_sys_reg,
        1, "mips-sys.xml", 0);
#endif
#if defined(TARGET_CHERI)
    gdb_register_coprocessor(cs, mips_gdb_get_cheri_reg,
        mips_gdb_set_cheri_reg, 44,
#if defined(CHERI_MAGIC128) || defined(CHERI_128)
        "mips64-cheri-c128.xml",
#else
        "mips64-cheri-c256.xml",
#endif
        0);
#endif

    cpu_mips_realize_env(&cpu->env);

    cpu_reset(cs);
    qemu_init_vcpu(cs);

    mcc->parent_realize(dev, errp);
}

static void mips_cpu_initfn(Object *obj)
{
    MIPSCPU *cpu = MIPS_CPU(obj);
    CPUMIPSState *env = &cpu->env;
    MIPSCPUClass *mcc = MIPS_CPU_GET_CLASS(obj);

    cpu_set_cpustate_pointers(cpu);
    env->cpu_model = mcc->cpu_def;
}

static char *mips_cpu_type_name(const char *cpu_model)
{
    return g_strdup_printf(MIPS_CPU_TYPE_NAME("%s"), cpu_model);
}

static ObjectClass *mips_cpu_class_by_name(const char *cpu_model)
{
    ObjectClass *oc;
    char *typename;

    typename = mips_cpu_type_name(cpu_model);
    oc = object_class_by_name(typename);
    g_free(typename);
    return oc;
}

#if defined(TARGET_CHERI)
static uint64_t start_ns = 0;
static void dump_cpu_ips_on_exit(void) {
    assert(start_ns != 0);
    CPUState *cpu;
    CPU_FOREACH(cpu) {
        CPUMIPSState *env = cpu->env_ptr;
        double duration_s = (get_clock() - start_ns) / 1000000000.0;
        uint64_t inst_total = env->statcounters_icount_kernel + env->statcounters_icount_user;
        info_report("CPU%d executed instructions: %jd (%jd user, %jd kernel) in %.2fs KIPS: %.2f\r\n", cpu->cpu_index,
                    (uintmax_t)inst_total, (uintmax_t)env->statcounters_icount_user,
                    (uintmax_t)env->statcounters_icount_kernel, duration_s,
                    (double)(inst_total / duration_s) / 1000.0);
    }
}
#if defined(DO_CHERI_STATISTICS)
static void dump_stats_on_exit(void)
{
    if (qemu_log_enabled() && qemu_loglevel_mask(CPU_LOG_INSTR | CPU_LOG_CHERI_BOUNDS)) {
        FILE* logf = qemu_log_lock();
        cheri_cpu_dump_statistics_f(NULL, logf, 0);
        qemu_log_unlock(logf);
    } else
        cheri_cpu_dump_statistics_f(NULL, stderr, 0);
}
#endif
#endif

static void mips_cpu_class_init(ObjectClass *c, void *data)
{
    MIPSCPUClass *mcc = MIPS_CPU_CLASS(c);
    CPUClass *cc = CPU_CLASS(c);
    DeviceClass *dc = DEVICE_CLASS(c);

    device_class_set_parent_realize(dc, mips_cpu_realizefn,
                                    &mcc->parent_realize);
    device_class_set_parent_reset(dc, mips_cpu_reset, &mcc->parent_reset);

    cc->class_by_name = mips_cpu_class_by_name;
    cc->has_work = mips_cpu_has_work;
    cc->do_interrupt = mips_cpu_do_interrupt;
    cc->cpu_exec_interrupt = mips_cpu_exec_interrupt;
    cc->dump_state = mips_cpu_dump_state;
    cc->set_pc = mips_cpu_set_pc;
    cc->synchronize_from_tb = mips_cpu_synchronize_from_tb;
    cc->gdb_read_register = mips_cpu_gdb_read_register;
    cc->gdb_write_register = mips_cpu_gdb_write_register;
#ifndef CONFIG_USER_ONLY
    cc->do_transaction_failed = mips_cpu_do_transaction_failed;
    cc->do_unaligned_access = mips_cpu_do_unaligned_access;
    cc->get_phys_page_debug = mips_cpu_get_phys_page_debug;
    cc->vmsd = &vmstate_mips_cpu;
#endif
    cc->disas_set_info = mips_cpu_disas_set_info;
#ifdef CONFIG_TCG
    cc->tcg_initialize = mips_tcg_init;
    cc->tlb_fill = mips_cpu_tlb_fill;
#endif

#if defined(TARGET_MIPS64)
    cc->gdb_core_xml_file = "mips64-cpu.xml";
#else
    cc->gdb_core_xml_file = "mips-cpu.xml";
#endif
    cc->gdb_num_core_regs = 72;
    cc->gdb_stop_before_watchpoint = true;
#if defined(TARGET_CHERI)
    cc->dump_statistics = cheri_cpu_dump_statistics;
    start_ns = get_clock();
    atexit(dump_cpu_ips_on_exit);
#if defined(DO_CHERI_STATISTICS)
    atexit(dump_stats_on_exit);
#endif
#endif

}

static const TypeInfo mips_cpu_type_info = {
    .name = TYPE_MIPS_CPU,
    .parent = TYPE_CPU,
    .instance_size = sizeof(MIPSCPU),
    .instance_init = mips_cpu_initfn,
    .abstract = true,
    .class_size = sizeof(MIPSCPUClass),
    .class_init = mips_cpu_class_init,
};

static void mips_cpu_cpudef_class_init(ObjectClass *oc, void *data)
{
    MIPSCPUClass *mcc = MIPS_CPU_CLASS(oc);
    mcc->cpu_def = data;
}

static void mips_register_cpudef_type(const struct mips_def_t *def)
{
    char *typename = mips_cpu_type_name(def->name);
    TypeInfo ti = {
        .name = typename,
        .parent = TYPE_MIPS_CPU,
        .class_init = mips_cpu_cpudef_class_init,
        .class_data = (void *)def,
    };

    type_register(&ti);
    g_free(typename);
}

static void mips_cpu_register_types(void)
{
    int i;

    type_register_static(&mips_cpu_type_info);
    for (i = 0; i < mips_defs_number; i++) {
        mips_register_cpudef_type(&mips_defs[i]);
    }
}

#ifdef TARGET_CHERI
static inline void set_epc_or_error_epc(CPUMIPSState *env, cap_register_t* epc_or_error_epc, target_ulong new_cursor)
{

    // Setting EPC should clear EPCC.tag if EPCC is sealed or becomes unrepresentable.
    // This will cause exception on instruction fetch following subsequent eret
    if (!cap_is_unsealed(epc_or_error_epc)) {
        error_report("Attempting to modify sealed EPCC/ErrorEPCC: " PRINT_CAP_FMTSTR "\r", PRINT_CAP_ARGS(epc_or_error_epc));
        qemu_log("Attempting to modify sealed EPCC/ErrorEPCC: " PRINT_CAP_FMTSTR "\r", PRINT_CAP_ARGS(epc_or_error_epc));
        // Clear the tag bit and update the cursor:
        cap_mark_unrepresentable(new_cursor, epc_or_error_epc);
    } else if (!is_representable_cap_with_addr(epc_or_error_epc, new_cursor)) {
        error_report("Attempting to set unrepresentable cursor(0x" TARGET_FMT_lx
                    ") on EPCC/ErrorEPCC: " PRINT_CAP_FMTSTR "\r", new_cursor, PRINT_CAP_ARGS(epc_or_error_epc));
        cap_mark_unrepresentable(new_cursor, epc_or_error_epc);
    } else {
        epc_or_error_epc->_cr_cursor = new_cursor;
    }
}
#endif

void set_CP0_EPC(CPUMIPSState *env, target_ulong arg)
{
#ifdef TARGET_CHERI
    set_epc_or_error_epc(env, &env->active_tc.CHWR.EPCC, arg);
#else
    env->CP0_EPC = arg;
#endif
}
void set_CP0_ErrorEPC(CPUMIPSState *env, target_ulong arg)
{
#ifdef TARGET_CHERI
    set_epc_or_error_epc(env, &env->active_tc.CHWR.ErrorEPCC, arg);
#else
    env->CP0_ErrorEPC = arg;
#endif
}

type_init(mips_cpu_register_types)
