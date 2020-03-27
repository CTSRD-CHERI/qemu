/*
 * Generic intermediate code generation.
 *
 * Copyright (C) 2016-2017 Lluís Vilanova <vilanova@ac.upc.edu>
 *
 * This work is licensed under the terms of the GNU GPL, version 2 or later.
 * See the COPYING file in the top-level directory.
 */

#include "qemu/osdep.h"
#include "qemu/error-report.h"
#include "cpu.h"
#include "tcg/tcg.h"
#include "tcg/tcg-op.h"
#include "exec/exec-all.h"
#include "exec/gen-icount.h"
#include "exec/log.h"
#include "exec/translator.h"
#include "exec/plugin-gen.h"

/* Pairs with tcg_clear_temp_count.
   To be called by #TranslatorOps.{translate_insn,tb_stop} if
   (1) the target is sufficiently clean to support reporting,
   (2) as and when all temporaries are known to be consumed.
   For most targets, (2) is at the end of translate_insn.  */
void translator_loop_temp_check(DisasContextBase *db)
{
    if (tcg_check_temp_count()) {
        qemu_log("warning: TCG temporary leaks before "
                 TARGET_FMT_lx "\n", db->pc_next);
    }
}

#ifdef CONFIG_MIPS_LOG_INSTR
extern int cl_default_trace_format;
#endif
#define DEBUG_INSTR_LOGGING 0

void translator_loop(const TranslatorOps *ops, DisasContextBase *db,
                     CPUState *cpu, TranslationBlock *tb, int max_insns)
{
    int bp_insn = 0;
    bool plugin_enabled;

    /* Initialize DisasContext */
    db->tb = tb;
    db->pc_first = tb->pc;
    db->pc_next = db->pc_first;
    db->is_jmp = DISAS_NEXT;
    db->num_insns = 0;
    db->max_insns = max_insns;
    db->singlestep_enabled = cpu->singlestep_enabled;
#ifdef TARGET_CHERI
    db->pcc_base = tb->cs_base;
    db->pcc_top = tb->cs_top;
    cheri_debug_assert(db->pcc_base ==
                       cap_get_base(cheri_get_recent_pcc(cpu->env_ptr)));
    cheri_debug_assert(db->pcc_top ==
                       cap_get_top(cheri_get_recent_pcc(cpu->env_ptr)));
#endif
    ops->init_disas_context(db, cpu);
    tcg_debug_assert(db->is_jmp == DISAS_NEXT);  /* no early exit */
#ifdef CONFIG_MIPS_LOG_INSTR
    // Note: changes to the instruction logging flags result in a call to
    // tb_flush so we can do the logging checks at translate time.
    bool should_log_instr =
        qemu_loglevel_mask(CPU_LOG_INSTR | CPU_LOG_CVTRACE | CPU_LOG_USER_ONLY);
    if (unlikely(should_log_instr && qemu_loglevel_mask(CPU_LOG_USER_ONLY))) {
        if (!ops->tb_in_user_mode(db, cpu)) {
#if DEBUG_INSTR_LOGGING
            qemu_log("-- Userspace Instruction logging disabled for "
                     "TB@" TARGET_FMT_lx "\n", db->pc_next);
#endif
            should_log_instr = false;
            // FIXME: this does not work properly with multiple threads
            qemu_loglevel &= ~(CPU_LOG_INSTR | CPU_LOG_CVTRACE);
        } else {
            // FIXME: this does not work properly with multiple threads
            qemu_loglevel |= cl_default_trace_format;
        }
    }
#if DEBUG_INSTR_LOGGING
    if (unlikely(should_log_instr)) {
        qemu_log("-- Instruction logging enabled for TB@" TARGET_FMT_lx
            " (usermode=%d)\n", db->pc_next, ops->tb_in_user_mode(db, cpu));
    }
#endif
    db->log_instr = should_log_instr;
#endif

    /* Reset the temp count so that we can identify leaks */
    tcg_clear_temp_count();

    /* Start translating.  */
    gen_tb_start(db->tb);
#ifdef CONFIG_DEBUG_TCG
    // On TB entry pc is up-to-date.
    if (_pc_is_current) {
        tcg_gen_movi_tl(_pc_is_current, 1);
    }
#endif
    ops->tb_start(db, cpu);
#ifdef TARGET_CHERI
    /* Check PCC permissions and tag once on TB entry. */
    gen_helper_ccheck_pcc_on_tb_entry(cpu_env);
#endif
    tcg_debug_assert(db->is_jmp == DISAS_NEXT);  /* no early exit */

    plugin_enabled = plugin_gen_tb_start(cpu, tb);

    while (true) {
        db->num_insns++;
#ifdef CONFIG_DEBUG_TCG
        // For debugging mark the current PCC.cursor as outdated
        if (_pc_is_current) {
            tcg_gen_movi_tl(_pc_is_current, 0);
        }
#endif
        ops->insn_start(db, cpu);
#if defined(CONFIG_MIPS_LOG_INSTR)
        if (unlikely(should_log_instr)) {
            TCGv tpc = tcg_const_tl(db->pc_next);
            gen_helper_log_instruction(cpu_env, tpc);
            tcg_temp_free(tpc);
        }
#endif
        tcg_debug_assert(db->is_jmp == DISAS_NEXT);  /* no early exit */

        if (plugin_enabled) {
            plugin_gen_insn_start(cpu, db);
        }

        /* Pass breakpoint hits to target for further processing */
        if (!db->singlestep_enabled
            && unlikely(!QTAILQ_EMPTY(&cpu->breakpoints))) {
            CPUBreakpoint *bp;
            QTAILQ_FOREACH(bp, &cpu->breakpoints, entry) {
                if (bp->pc == db->pc_next) {
                    if (ops->breakpoint_check(db, cpu, bp)) {
                        bp_insn = 1;
                        break;
                    }
                }
            }
            /* The breakpoint_check hook may use DISAS_TOO_MANY to indicate
               that only one more instruction is to be executed.  Otherwise
               it should use DISAS_NORETURN when generating an exception,
               but may use a DISAS_TARGET_* value for Something Else.  */
            if (db->is_jmp > DISAS_TOO_MANY) {
                break;
            }
        }

        /* Disassemble one instruction.  The translate_insn hook should
           update db->pc_next and db->is_jmp to indicate what should be
           done next -- either exiting this loop or locate the start of
           the next instruction.  */
        if (db->num_insns == db->max_insns
            && (tb_cflags(db->tb) & CF_LAST_IO)) {
            /* Accept I/O on the last instruction.  */
            gen_io_start();
            ops->translate_insn(db, cpu);
        } else {
            ops->translate_insn(db, cpu);
        }

        /* Stop translation if translate_insn so indicated.  */
        if (db->is_jmp != DISAS_NEXT) {
            break;
        }

        /*
         * We can't instrument after instructions that change control
         * flow although this only really affects post-load operations.
         */
        if (plugin_enabled) {
            plugin_gen_insn_end();
        }

        /* Stop translation if the output buffer is full,
           or we have executed all of the allowed instructions.  */
        if (tcg_op_buf_full() || db->num_insns >= db->max_insns) {
            db->is_jmp = DISAS_TOO_MANY;
            break;
        }
    }

    /* Emit code to exit the TB, as indicated by db->is_jmp.  */
    ops->tb_stop(db, cpu);
    gen_tb_end(db->tb, db->num_insns - bp_insn);

    if (plugin_enabled) {
        plugin_gen_tb_end(cpu);
    }

    /* The disas_log hook may use these values rather than recompute.  */
    db->tb->size = db->pc_next - db->pc_first;
    db->tb->icount = db->num_insns;

#ifdef DEBUG_DISAS
    if (qemu_loglevel_mask(CPU_LOG_TB_IN_ASM)
        && qemu_log_in_addr_range(db->pc_first)) {
        FILE *logfile = qemu_log_lock();
        qemu_log("----------------\n");
        ops->disas_log(db, cpu);
        qemu_log("\n");
        qemu_log_unlock(logfile);
    }
#endif
}
