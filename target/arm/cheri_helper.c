/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2020 Lawrence Esswood
 *
 * This work was supported by Innovate UK project 105694, "Digital Security
 * by Design (DSbD) Technology Platform Prototype".
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

#include "qemu/osdep.h"
#include "cpu.h"
#include "exec/exec-all.h"
#include "exec/helper-proto.h"
#include "exec/memop.h"

#include "cheri-helper-utils.h"

void helper_load_cap_pair_via_cap(CPUArchState *env, uint32_t cd, uint32_t cd2,
                                  uint32_t cb, target_ulong addr)
{
    GET_HOST_RETPC();

    const cap_register_t *cbp = get_capreg_or_special(env, cb);

    cap_check_common_reg(perms_for_load(), env, cb, addr, CHERI_CAP_SIZE * 2,
                         _host_return_address, cbp, CHERI_CAP_SIZE, true);

    // We avoid using load_cap_from_memory for the first load as it will have
    // side effects on the register file.
    uint64_t pesbt;
    uint64_t cursor;
    bool tag = load_cap_from_memory_128(env, &pesbt, &cursor, cb, cbp, addr,
                                        _host_return_address, NULL);

    load_cap_from_memory(env, cd2, cb, cbp, addr + CHERI_CAP_SIZE,
                         _host_return_address, NULL);
    // Once the second load has finished, we can modify the register file.
    update_compressed_capreg(env, cd, pesbt, tag, cursor);
}

void helper_store_cap_pair_via_cap(CPUArchState *env, uint32_t cd, uint32_t cd2,
                                   uint32_t cb, target_ulong addr)
{
    GET_HOST_RETPC();

    const cap_register_t *cbp = get_capreg_or_special(env, cb);

    cap_check_common_reg(perms_for_store(env, cd) | perms_for_store(env, cd2),
                         env, cb, addr, CHERI_CAP_SIZE * 2,
                         _host_return_address, cbp, CHERI_CAP_SIZE, true);

    store_cap_to_memory(env, cd, addr, _host_return_address);
    store_cap_to_memory(env, cd2, addr + CHERI_CAP_SIZE, _host_return_address);
}
