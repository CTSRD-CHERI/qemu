/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2020 Alfredo Mazzinghi
 * All rights reserved.
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory under DARPA/AFRL contract FA8750-10-C-0237
 * ("CTSRD"), as part of the DARPA CRASH research programme.
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

/*
 * All instruction logging helpers shared between targets are defined here.
 * These are automatically made available to all targets when instruction
 * logging support is enabled.
 */

// TODO(am2419): should be TCG_CALL_NO_RWG?
DEF_HELPER_2(qemu_log_instr_start, void, env, tl)
DEF_HELPER_2(qemu_log_instr_stop, void, env, tl)
DEF_HELPER_2(qemu_log_instr_user_start, void, env, tl)
DEF_HELPER_2(qemu_log_instr_user_stop, void, env, tl)
// TODO(am2419): should be TCG_CALL_NO_RWG?
DEF_HELPER_1(qemu_log_instr_commit, void, env)
// TODO(am2419): should be TCG_CALL_NO_RWG?
DEF_HELPER_2(qemu_log_instr, void, env, tl)
DEF_HELPER_FLAGS_4(qemu_log_instr_load64, TCG_CALL_NO_RWG, void, env, cap_checked_ptr, i64, memop)
DEF_HELPER_FLAGS_4(qemu_log_instr_store64, TCG_CALL_NO_RWG, void, env, cap_checked_ptr, i64, memop)
DEF_HELPER_FLAGS_4(qemu_log_instr_load32, TCG_CALL_NO_RWG, void, env, cap_checked_ptr, i32, memop)
DEF_HELPER_FLAGS_4(qemu_log_instr_store32, TCG_CALL_NO_RWG, void, env, cap_checked_ptr, i32, memop)
