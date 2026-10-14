/*
 * QEMU RVFI-DII interface implementation
 *
 * Copyright (c) 2020 Alexander Richardson, Alexander.Richardson@cl.cam.ac.uk
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2 or later, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "qemu/osdep.h"
#include "cpu.h"
#include "exec/exec-all.h"
#include "exec/log_instr.h"
#include "qemu/cutils.h"
#include "sysemu/cpus.h"
#include "sysemu/runstate.h"
#include "disas/disas.h"

#ifdef TARGET_CHERI
#include "cheri-lazy-capregs.h"
#endif

static void send_rvfi_dii_packet(const void *data, size_t len)
{
    if (rvfi_debug_output) {
        qemu_hexdump(stderr, "PACKET", data, len);
    }
    ssize_t nbytes = write(rvfi_client_fd, data, len);
    if (nbytes != len) {
        error_report("Failed to write packet to socket: %zd (%s)", nbytes,
                     strerror(errno));
        exit(EXIT_FAILURE);
    }
}

static void rvfi_dii_send_v1_trace(CPURISCVState *env)
{
    struct rvfi_dii_trace_v1 trace;
    /* convert the state saved in env to a legacy v1 trace */
    trace.rvfi_dii_order = env->rvfi_dii_trace.INST.rvfi_order;
    trace.rvfi_dii_pc_rdata = env->rvfi_dii_trace.PC.rvfi_pc_rdata;
    trace.rvfi_dii_pc_wdata = env->rvfi_dii_trace.PC.rvfi_pc_wdata;
    trace.rvfi_dii_insn = env->rvfi_dii_trace.INST.rvfi_insn;
    trace.rvfi_dii_rs1_data = env->rvfi_dii_trace.INTEGER.rvfi_rs1_rdata;
    trace.rvfi_dii_rs2_data = env->rvfi_dii_trace.INTEGER.rvfi_rs2_rdata;
    trace.rvfi_dii_rd_wdata = env->rvfi_dii_trace.INTEGER.rvfi_rd_wdata;
    trace.rvfi_dii_mem_addr = env->rvfi_dii_trace.MEM.rvfi_mem_addr;
    trace.rvfi_dii_mem_rdata = env->rvfi_dii_trace.MEM.rvfi_mem_rdata[0];
    trace.rvfi_dii_mem_wdata = env->rvfi_dii_trace.MEM.rvfi_mem_wdata[0];
    trace.rvfi_dii_mem_rmask = env->rvfi_dii_trace.MEM.rvfi_mem_rmask;
    trace.rvfi_dii_mem_wmask = env->rvfi_dii_trace.MEM.rvfi_mem_wmask;
    trace.rvfi_dii_rs1_addr = env->rvfi_dii_trace.INTEGER.rvfi_rs1_addr;
    trace.rvfi_dii_rs2_addr = env->rvfi_dii_trace.INTEGER.rvfi_rs2_addr;
    trace.rvfi_dii_rd_addr = env->rvfi_dii_trace.INTEGER.rvfi_rd_addr;
    trace.rvfi_dii_trap = env->rvfi_dii_trace.INST.rvfi_trap;
    trace.rvfi_dii_halt = env->rvfi_dii_trace.INST.rvfi_halt;
    trace.rvfi_dii_intr = env->rvfi_dii_trace.INST.rvfi_intr;

    if (rvfi_debug_output) {
        info_report(
            "Sending %jd PCWD: 0x%08jx, RD: %02d, RWD: 0x%08jx, MA: "
            "0x%08jx, MWD: 0x%08jx, MWM: 0x%08x, I: 0x%016jx H:%u",
            (uintmax_t)trace.rvfi_dii_order, (uintmax_t)trace.rvfi_dii_pc_wdata,
            trace.rvfi_dii_rd_addr, (uintmax_t)trace.rvfi_dii_rd_wdata,
            (uintmax_t)trace.rvfi_dii_mem_addr,
            (uintmax_t)trace.rvfi_dii_mem_wdata, trace.rvfi_dii_mem_wmask,
            (uintmax_t)trace.rvfi_dii_insn, (unsigned)trace.rvfi_dii_halt);
    }
    send_rvfi_dii_packet(&trace, sizeof(trace));
}

static void rvfi_dii_send_v2_trace(CPURISCVState *env)
{

    struct rvfi_dii_trace_v2 trace = {
        .magic = "trace-v2",
        .available_fields = env->rvfi_dii_trace.available_fields,
        .pc_data = env->rvfi_dii_trace.PC,
        .basic_info = env->rvfi_dii_trace.INST,
    };
    GByteArray *buf = g_byte_array_new();
    g_byte_array_append(buf, (const guint8 *)&trace, sizeof(trace));
    if (env->rvfi_dii_trace.available_fields & RVFI_INTEGER_DATA) {
        g_byte_array_append(buf, (const guint8 *)"int-data", 8);
        g_byte_array_append(buf, (const guint8 *)&env->rvfi_dii_trace.INTEGER,
                            sizeof(env->rvfi_dii_trace.INTEGER));
    }
    if (env->rvfi_dii_trace.available_fields & RVFI_MEM_DATA) {
        g_byte_array_append(buf, (const guint8 *)"mem-data", 8);
        g_byte_array_append(buf, (const guint8 *)&env->rvfi_dii_trace.MEM,
                            sizeof(env->rvfi_dii_trace.MEM));
    }
    /* Now that we know the total size, we can update the trace header: */
    ((struct rvfi_dii_trace_v2 *)buf->data)->trace_size = buf->len;
    if (rvfi_debug_output) {
        fprintf(
            stderr,
            "Sending %u bytes: %jd PCWD: 0x%08jx, RD: %02d, RWD: 0x%08jx, MA: "
            "0x%08jx, MWD: 0x%08jx, MWM: 0x%08x, I: 0x%016jx H:%u T:%u\n",
            buf->len, (uintmax_t)env->rvfi_dii_trace.INST.rvfi_order,
            (uintmax_t)env->rvfi_dii_trace.PC.rvfi_pc_wdata,
            env->rvfi_dii_trace.INTEGER.rvfi_rd_addr,
            (uintmax_t)env->rvfi_dii_trace.INTEGER.rvfi_rd_wdata,
            (uintmax_t)env->rvfi_dii_trace.MEM.rvfi_mem_addr,
            (uintmax_t)env->rvfi_dii_trace.MEM.rvfi_mem_wdata[0],
            env->rvfi_dii_trace.MEM.rvfi_mem_wmask,
            (uintmax_t)env->rvfi_dii_trace.INST.rvfi_insn,
            (unsigned)env->rvfi_dii_trace.INST.rvfi_halt,
            (unsigned)env->rvfi_dii_trace.INST.rvfi_trap);
    }
    send_rvfi_dii_packet(buf->data, buf->len);
    g_byte_array_free(buf, true);
}

static void rvfi_dii_send_trace(CPURISCVState *env, unsigned version)
{
    if (version == 1) {
        rvfi_dii_send_v1_trace(env);
    } else if (version == 2) {
        rvfi_dii_send_v2_trace(env);
    } else {
        error_report("Invalid trace version %d", version);
        exit(EXIT_FAILURE);
    }
}

void rvfi_dii_communicate(CPUState *cs, CPURISCVState *env, bool was_trap)
{
    /*
     * Needs to be global since this function is called for each instruction
     * that is executed.
     */
    static bool rvfi_dii_started;
    static unsigned rvfi_dii_version = 1;

    /* Single-step completed -> update PC in the trace buffer */
    env->rvfi_dii_trace.PC.rvfi_pc_wdata = GET_SPECIAL_REG_ARCH(env, pc, pcc);
    env->rvfi_dii_trace.INST.rvfi_order++;

    /* TestRIG expects a zero $pc after a trap: */
    if (env->rvfi_dii_trace.INST.rvfi_trap && rvfi_debug_output) {
        info_report("Got trap at " TARGET_FMT_lx, PC_ADDR(env));
    }
    env->rvfi_dii_have_injected_insn = false;
    while (true) {
        assert(cs->singlestep_enabled);
        rvfi_dii_command_t cmd_buf;
        _Static_assert(sizeof(cmd_buf) == 8, "Expected 8 bytes of data");
#ifdef CONFIG_TCG_LOG_INSTR
        /*
         * Print the instruction now and skip the next commit() call that
         * happens when we return to the translator loop.
         */
        qemu_log_instr_commit(env);
        /* Avoid an invalid instruction log */
        qemu_log_instr_drop(env);
#endif
        if (rvfi_dii_started) {
            /* Send previous state */
            rvfi_dii_send_trace(env, rvfi_dii_version);
            /* Zero the output trace for the next test except for instret */
            uint64_t old_instret = env->rvfi_dii_trace.INST.rvfi_order;
            memset(&env->rvfi_dii_trace, 0, sizeof(env->rvfi_dii_trace));
            env->rvfi_dii_trace.INST.rvfi_order = old_instret;
        }
        /* Should be blocking, so we only read fewer bytes on EOF */
        ssize_t nbytes = read(rvfi_client_fd, &cmd_buf, sizeof(cmd_buf));
        if (nbytes != sizeof(cmd_buf)) {
            error_report("GOT EOF/Error reading from socket: %zd (%s)", nbytes,
                         strerror(errno));
            exit(EXIT_FAILURE);
        }
        if (rvfi_debug_output) {
            info_report("Handling RVFI-DII command %d", cmd_buf.rvfi_dii_cmd);
        }
        switch (cmd_buf.rvfi_dii_cmd) {
        case RVFI_DII_CMD_END: {
            rvfi_dii_started = false;
            if (cmd_buf.rvfi_dii_insn ==
                (('V' << 24) | ('E' << 16) | ('R' << 8) | 'S')) {
                /*
                 * Version negotiation request -> send a v1 packet with halt=3
                 * to indicate that we support the v2 protocol
                 */
                env->rvfi_dii_trace.INST.rvfi_halt = 3;
            } else {
                env->rvfi_dii_trace.INST.rvfi_halt = 1;
            }
            env->rvfi_dii_trace.INST.rvfi_order = 0;
            /*
             * Clear all fields that can be zeroes: we want a defined reset
             * state for TestRIG even if the RISC-V ISA does not guarantee it.
             */
            memset(env, 0, offsetof(CPURISCVState, end_testrig_reset_fields));
            /*
             * Overwrite the processor's resetvec as otherwise reset()
             * writes PC with default RSTVECTOR (0x10000)
             */
            env->resetvec = RVFI_DII_RAM_START;
            /* Reset CPUs and devices, resume at resetvec 0x80000000 */
            qemu_system_reset(SHUTDOWN_CAUSE_HOST_SIGNAL);
#ifdef TARGET_CHERI
            /* TestRIG expects all capability registers to be max perms */
            set_max_perms_capregs(env);
#endif
            cs->cflags_next_tb = (curr_cflags(cs) & ~CF_USE_ICOUNT) | 1;
            hwaddr system_ram_addr = cpu_get_phys_page_debug(cs, PC_ADDR(env));
            hwaddr system_ram_size = RVFI_DII_RAM_SIZE;
            void *ram_ptr = cpu_physical_memory_map(
                system_ram_addr, &system_ram_size, /*is_write=*/true);
            assert(system_ram_size == RVFI_DII_RAM_SIZE);
            /*
             * TODO: would be nice to do this lazily instead of writing 8 MiB
             * FIXME: is it safe to do a munmap/mmap? We could also MAP_FIXED
             * over the existing mapping. This should be faster since we rarely
             * use more than one page.
             */
            memset(ram_ptr, 0, system_ram_size);
            /* Unmap: this should invalidate all caches for that region. */
            cpu_physical_memory_unmap(ram_ptr, system_ram_size,
                                      /*is_write=*/true, system_ram_size);
            /* Flush the TCG state: */
            tb_flush(cs);
            tlb_flush(cs); /* Flush the QEMU guest->host tlb */
            rvfi_dii_send_trace(env, rvfi_dii_version);
            memset(&env->rvfi_dii_trace, 0, sizeof(env->rvfi_dii_trace));
            continue;
        }
        case RVFI_DII_CMD_VERSION: { /* Set wire format version */
            if (cmd_buf.rvfi_dii_insn == 1) {
                fprintf(stderr, "Requested trace in legacy format!\n");
            } else if (cmd_buf.rvfi_dii_insn == 2) {
                fprintf(stderr, "Requested trace in v2 format!\n");
            } else {
                fprintf(stderr, "Requested trace in unsupported format %jd!\n",
                        (intmax_t)cmd_buf.rvfi_dii_insn);
                exit(EXIT_FAILURE);
            }
            /* From now on send traces in the requested format */
            rvfi_dii_version = cmd_buf.rvfi_dii_insn;
            struct {
                char msg[8];
                uint64_t version;
            } version_response = { "version=", rvfi_dii_version };
            send_rvfi_dii_packet(&version_response, sizeof(version_response));
            continue;
        }
        case RVFI_DII_CMD_BLINK: {
            fprintf(stderr, "*BLINK*\n");
            info_report("*BLINK*");
            break;
        }
        case RVFI_DII_CMD_QUIT: {
            /* The remote disconnected. */
            fprintf(stderr, "Received a quit command. Quitting.\n");
            info_report("Received a quit command. Quitting.");
            close(rvfi_client_fd);
            rvfi_client_fd = 0;
            exit(EXIT_SUCCESS);
        }
        case RVFI_DII_CMD_INSTR: {
            /*
             * We send the resulting packet on the next call of this function.
             */
            rvfi_dii_started = true;
            cpu_single_step(cs, SSTEP_ENABLE | SSTEP_NOIRQ | SSTEP_NOTIMER);
            if (rvfi_debug_output) {
                char buf[512];
                FILE *tmp = fmemopen(buf, sizeof(buf), "w+");
                target_disas_buf(tmp, cs, &cmd_buf.rvfi_dii_insn,
                                 sizeof(cmd_buf.rvfi_dii_insn), PC_ADDR(env),
                                 1);
                fclose(tmp);
                info_report("injecting instruction %d '0x%08x' at %s",
                            cmd_buf.rvfi_dii_time, cmd_buf.rvfi_dii_insn, buf);
            }
            /*
             * Ideally we would just completely disable caching of translated
             * blocks in RVFI-DII mode, but I can't figure out how to do this.
             * Instead let's just flush the entire TCG cache (which should have
             * the same effect).
             */
            tb_flush(cs); /* flush TCG state */
            env->rvfi_dii_injected_insn = cmd_buf.rvfi_dii_insn;
            env->rvfi_dii_have_injected_insn = true;
            env->rvfi_dii_trace.PC.rvfi_pc_rdata =
                GET_SPECIAL_REG_ARCH(env, pc, pcc);
            env->rvfi_dii_trace.INST.rvfi_mode = env->priv;
            env->rvfi_dii_trace.INST.rvfi_ixl = riscv_cpu_mxl(env);
            resume_all_vcpus();
            cpu_resume(cs);
            /* Will be set after single-step trap */
            env->rvfi_dii_trace.PC.rvfi_pc_wdata = -1;
            cs->cflags_next_tb = (curr_cflags(cs) & ~CF_USE_ICOUNT) | 1;
            /*
             * Continue execution at env->pc.
             * Note that this will also reset cs->exception_index and will
             * avoid dropping into GDB.
             */
            cpu_loop_exit_noexc(cs); /* noreturn -> jumps back to TCG */
        }
        default:
            error_report("rvfi_dii got unsupported command '%c'",
                         cmd_buf.rvfi_dii_cmd);
            exit(EXIT_FAILURE);
        }
        rvfi_dii_started = true;
    }
}
