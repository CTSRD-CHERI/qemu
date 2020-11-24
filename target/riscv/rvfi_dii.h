/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2018 Hesham Almatary <Hesham.Almatary@cl.cam.ac.uk>
 * Copyright (c) 2018 Jack Deely
 * Copyright (c) 2018 Jonathan Woodruff
 *
 * All rights reserved.
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory (Department of Computer Science and
 * Technology) under DARPA contract HR0011-18-C-0016 ("ECATS"), as part of the
 * DARPA SSITH research programme.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#ifndef _RISCV_RVFI_DII_H
#define _RISCV_RVFI_DII_H

#include <stdint.h>

/// The old trace packet for backwards compatibility with older TestRIG versions:
struct rvfi_dii_trace_v1 {
    // uint64_t        rvfi_dii_valid;     // Valid signal:                        Instruction was committed properly.
    uint64_t rvfi_dii_order;    // [000 - 063] Instruction number:      INSTRET value after completion.
    uint64_t rvfi_dii_pc_rdata; // [237 - 300] PC before instr:         PC for current instruction
    uint64_t rvfi_dii_pc_wdata; // [301 - 364] PC after instr:          Following PC - either PC + 4 or jump target.
    uint64_t rvfi_dii_insn;     // [067 - 098] Instruction word:        32-bit command value.

    uint64_t rvfi_dii_rs1_data; // [109 - 172] Read register values:    Values as read from registers named
    uint64_t rvfi_dii_rs2_data; // [173 - 236]                          above. Must be 0 if register ID is 0.
    uint64_t rvfi_dii_rd_wdata; // [434 - 497] Write register value:    MUST be 0 if rd_ is 0.

    uint64_t rvfi_dii_mem_addr; // [498 - 561] Memory access addr:      Points to byte address (aligned if define
                                //                                      is set). *Should* be straightforward.

    // XXX: SC writes something other than read value, but the value that would be read is unimportant.
    // Unsure what the point of this is, it's only relevant when the value is going to be in rd anyway.
    uint64_t rvfi_dii_mem_rdata; // [578 - 641] Read data:               Data read from mem_addr (i.e. before write)
    // PROBLEM: LR/SC, if SC fails then value is not written. Indicate as wmask = 0.
    uint64_t rvfi_dii_mem_wdata; // [365 - 428] Write data:              Data written to memory by this command.

    // Not explicitly given, but calculable from opcode/funct3 from ISA_Decls.
    uint8_t rvfi_dii_mem_rmask; // [562 - 569] Read mask:               Indicates valid bytes read. 0 if unused.
    uint8_t rvfi_dii_mem_wmask; // [570 - 577] Write mask:              Indicates valid bytes written. 0 if unused.

    uint8_t rvfi_dii_rs1_addr; // [099 - 103] Read register addresses: Can be arbitrary when not used,
    uint8_t rvfi_dii_rs2_addr; // [104 - 108]                          otherwise set as decoded.

    // Found in ALU if used, not 0'd if not used. Check opcode/op_stage2.
    // PROBLEM: LD/AMO - then found in stage 2.
    uint8_t rvfi_dii_rd_addr; // [429 - 433] Write register address:  MUST be 0 if not used.

    uint8_t rvfi_dii_trap; // [064 - 064] Trap indicator:          Invalid decode, misaligned access or
                           //                                      jump command to misaligned address.

    uint8_t rvfi_dii_halt; // [065 - 065] Halt indicator:          Marks the last instruction retired
                           //                                      before halting execution.

    uint8_t rvfi_dii_intr; // [066 - 066] Trap handler:            Set for first instruction in trap handler.
} QEMU_PACKED;        // 88 Bytes

// See https://github.com/SymbioticEDA/riscv-formal/blob/master/docs/rvfi.md
struct rvfi_dii_instruction_metadata {
    /// The rvfi_order field must be set to the instruction index. No indices
    /// must be used twice and there must be no gaps. Instructions may be
    /// retired in a reordered fashion, as long as causality is preserved
    /// (register and memory write operations must be retired before the read
    /// operations that depend on them).
    uint64_t rvfi_order; // output [NRET *   64 - 1 : 0] rvfi_order
    /// rvfi_insn is the instruction word for the retired instruction. In case
    /// of an instruction with fewer than ILEN bits, the upper bits of this
    /// output must be all zero. For compressed instructions the compressed
    /// instruction word must be output on this port. For fused instructions the
    /// complete fused instruction sequence must be output.
    uint64_t rvfi_insn; // output [NRET * ILEN - 1 : 0] rvfi_insn
    /// rvfi_trap must be set for an instruction that cannot be decoded as a
    /// legal instruction, such as 0x00000000.
    /// In addition, rvfi_trap must be set for a misaligned memory read or
    /// write in PMAs that don't allow misaligned access, or other memory
    /// access violations. rvfi_trap must also be set for a jump instruction
    /// that jumps to a misaligned instruction.
    uint8_t rvfi_trap; // output [NRET        - 1 : 0] rvfi_trap
    /// The signal rvfi_halt must be set when the instruction is the last
    /// instruction that the core retires before halting execution. It should not
    /// be set for an instruction that triggers a trap condition if the CPU
    /// reacts to the trap by executing a trap handler. This signal enables
    /// verification of liveness properties.
    uint8_t rvfi_halt; // output [NRET        - 1 : 0] rvfi_halt
    /// rvfi_intr must be set for the first instruction that is part of a trap
    /// handler, i.e. an instruction that has a rvfi_pc_rdata that does not
    /// match the rvfi_pc_wdata of the previous instruction.
    uint8_t rvfi_intr; // output [NRET        - 1 : 0] rvfi_intr
    /// rvfi_mode must be set to the current privilege level, using the following
    /// encoding: 0=U-Mode, 1=S-Mode, 2=Reserved, 3=M-Mode
    uint8_t rvfi_mode; // output [NRET * 2    - 1 : 0] rvfi_mode
    /// rvfi_ixl must be set to the value of MXL/SXL/UXL in the current privilege level,
    /// using the following encoding: 1=32, 2=64
    uint8_t rvfi_ixl;  // output [NRET * 2    - 1 : 0] rvfi_ixl

    /// When the core retires an instruction, it asserts the rvfi_valid signal
    /// and uses the signals described below to output the details of the
    /// retired instruction. The signals below are only valid during such a
    /// cycle and can be driven to arbitrary values in a cycle in which
    /// rvfi_valid is not asserted.
    uint8_t rvfi_valid; // output [NRET        - 1 : 0] rvfi_valid
    // Note: since we only send these packets in the valid state, we could
    // omit the valid signal, but we need 3 bytes of padding after ixl anyway
    // so we might as well include it
    uint8_t padding[2];
} QEMU_PACKED;
_Static_assert(sizeof(struct rvfi_dii_instruction_metadata) == 24, "");

struct rvfi_dii_pc_data {
    /// This is the program counter (pc) before (rvfi_pc_rdata) and after
    /// (rvfi_pc_wdata) execution of this instruction. I.e. this is the address
    /// of the retired instruction and the address of the next instruction.
    uint64_t rvfi_pc_rdata; // output [NRET * XLEN - 1 : 0] rvfi_pc_rdata
    uint64_t rvfi_pc_wdata; // output [NRET * XLEN - 1 : 0] rvfi_pc_wdata
} QEMU_PACKED;
_Static_assert(sizeof(struct rvfi_dii_pc_data) == 16, "");

struct rvfi_dii_integer_data {
    /// rvfi_rd_wdata is the value of the x register addressed by rd after
    /// execution of this instruction. This output must be zero when rd is zero.
    uint64_t rvfi_rd_wdata; // output [NRET * XLEN - 1 : 0] rvfi_rd_wdata
    /// rvfi_rs1_rdata/rvfi_rs2_rdata is the value of the x register addressed
    /// by rs1/rs2 before execution of this instruction. This output must be
    /// zero when rs1/rs2 is zero.
    uint64_t rvfi_rs1_rdata; // output [NRET * XLEN - 1 : 0] rvfi_rs1_rdata
    uint64_t rvfi_rs2_rdata; // output [NRET * XLEN - 1 : 0] rvfi_rs2_rdata
    /// rvfi_rd_addr is the decoded rd register address for the retired
    /// instruction. For an instruction that writes no rd register, this output
    /// must always be zero.
    uint8_t rvfi_rd_addr; // output [NRET *    5 - 1 : 0] rvfi_rd_addr
    /// rvfi_rs1_addr and rvfi_rs2_addr are the decoded rs1 and rs1 register
    /// addresses for the retired instruction. For an instruction that reads no
    /// rs1/rs2 register, this output can have an arbitrary value. However, if
    /// this output is nonzero then rvfi_rs1_rdata must carry the value stored
    /// in that register in the pre-state.
    uint8_t rvfi_rs1_addr; // output [NRET *    5 - 1 : 0] rvfi_rs1_addr
    uint8_t rvfi_rs2_addr; // output [NRET *    5 - 1 : 0] rvfi_rs2_addr
    uint8_t pad[5];
} QEMU_PACKED;
_Static_assert(sizeof(struct rvfi_dii_integer_data) == 32, "");

// Memory Access (basic, reports memory accesses up to 64 bits)
struct rvfi_dii_memory_access_data {
    /// rvfi_mem_rdata is the pre-state data read from rvfi_mem_addr.
    /// rvfi_mem_rmask specifies which bytes are valid.
    /// CHERI-extension: widened to 32 bytes to allow reporting 129 bits
    uint64_t rvfi_mem_rdata[4]; // output [NRET * XLEN   - 1 : 0] rvfi_mem_rdata
    /// rvfi_mem_wdata is the post-state data written to rvfi_mem_addr.
    /// rvfi_mem_wmask specifies which bytes are valid.
    /// CHERI-extension: widened to 32 bytes to allow reporting 129 bits
    uint64_t rvfi_mem_wdata[4]; // output [NRET * XLEN   - 1 : 0] rvfi_mem_wdata
    /// rvfi_mem_rmask is a bitmask that specifies which bytes in rvfi_mem_rdata
    /// contain valid read data from rvfi_mem_addr.
    /// CHERI-extension: we extend rmask+wmask to 32 bits to allow reporting the
    /// mask for CHERI/RV128 accesses here.
    uint32_t rvfi_mem_rmask; // output [NRET * XLEN/8 - 1 : 0] rvfi_mem_rmask
    /// rvfi_mem_wmask is a bitmask that specifies which bytes in rvfi_mem_wdata
    /// contain valid data that is written to rvfi_mem_addr.
    uint32_t rvfi_mem_wmask; // output [NRET * XLEN/8 - 1 : 0] rvfi_mem_wmask
    //// For memory operations (rvfi_mem_rmask and/or rvfi_mem_wmask are
    /// non-zero), rvfi_mem_addr holds the accessed memory location.
    uint64_t rvfi_mem_addr; // output [NRET * XLEN   - 1 : 0] rvfi_mem_addr
} QEMU_PACKED;
_Static_assert(sizeof(struct rvfi_dii_memory_access_data) == 80, "");

enum rvfi_trace_v2_available_fields {
    RVFI_INST_DATA = 0, // Must always be present
    RVFI_PC_DATA = 0, // Must always be present
    RVFI_INTEGER_DATA = 1 << 0,
    RVFI_MEM_DATA = 1 << 1,
    RVFI_FP_DATA = 1 << 2,
    RVFI_CSR_DATA = 1 << 3,
    RVFI_CHERI_DATA = 1 << 4,
    RVFI_CHERI_SCR_DATA = 1 << 5,
    RVFI_CHERI_TRAP_DATA = 1 << 6,
};

struct rvfi_dii_trace_v2 {
    char magic[8]; // must be "trace-v2"
    uint64_t trace_size;  // size of the packet, including all additonal fields
    struct rvfi_dii_instruction_metadata basic_info; // always present
    struct rvfi_dii_pc_data pc_data; // program counter information is also always present
    uint64_t available_fields; // of type rvfi_trace_v2_available_fields
    // If (available_fields & RVFI_INTEGER_DATA), then this is followed by struct rvfi_dii_integer_data;
    // If (available_fields & RVFI_MEM_DATA), then this is followed by struct rvfi_dii_memory_access_data;
    // If (available_fields & RVFI_FP_DATA), then this is followed by struct rvfi_dii_fp_data;
    // If (available_fields & RVFI_CSR_DATA), then this is followed by struct rvfi_dii_csr_data
    // If (available_fields & RVFI_CHERI_DATA), then this is followed by struct rvfi_dii_cheri_data;
    // If (available_fields & RVFI_CHERI_SCR_DATA), then this is followed by struct rvfi_dii_cheri_scr_data;
    // If (available_fields & RVFI_TRAP_DATA), then this is followed by struct rvfi_dii_trap_data;
} QEMU_PACKED;
_Static_assert(sizeof(struct rvfi_dii_trace_v2) == 64, "");

typedef struct rvfi_dii_command {
    uint32_t rvfi_dii_insn; // [0 - 31] Instruction word: 32-bit instruction or command. The lower 16-bits
    // may decode to a 16-bit compressed instruction.
    uint16_t rvfi_dii_time; // [62 - 53] Time to inject token.  The difference between this and the previous
    // instruction time gives a delay before injecting this instruction.
    // This can be ignored for models but gives repeatability for implementations
    // while shortening counterexamples.
    uint8_t rvfi_dii_cmd; // [63] This token is a trace command.  For example, reset device under test.
    uint8_t padding;
} QEMU_PACKED rvfi_dii_command_t; // 8 bytes

#endif
