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
#include "cheri_tagmem.h"

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

void helper_load_exclusive_cap_via_cap(CPUArchState *env, uint32_t cd,
                                       uint32_t cd2, uint32_t cb,
                                       target_ulong addr)
{
    GET_HOST_RETPC();

    env->exclusive_addr = addr;

    const cap_register_t *cbp = get_capreg_or_special(env, cb);
    uint32_t size = (cd2 == REG_NONE) ? CHERI_CAP_SIZE : (CHERI_CAP_SIZE * 2);

    // Exclusives must be aligned to the entire size of the operation, not just
    // one of the elements
    cap_check_common_reg(perms_for_load(), env, cb, addr, size,
                         _host_return_address, cbp, size, true);

    bool raw_tag;
    bool tag1 = load_cap_from_memory_128_raw_tag(
        env, &env->exclusive_high, &env->exclusive_val, cb, cbp, addr,
        _host_return_address, NULL, &raw_tag);
    env->exclusive_tag = raw_tag;

    if (cd2 != REG_NONE) {
        bool tag2 = load_cap_from_memory_128_raw_tag(
            env, &env->exclusive_high2, &env->exclusive_val2, cb, cbp,
            addr + CHERI_CAP_SIZE, _host_return_address, NULL, &raw_tag);
        env->exclusive_tag2 = raw_tag;
        update_compressed_capreg(env, cd2, env->exclusive_high2, tag2,
                                 env->exclusive_val2);
    }

    // Must do the update AFTER the other load has not trapped
    update_compressed_capreg(env, cd, env->exclusive_high, tag1,
                             env->exclusive_val);
}

void helper_store_exclusive_cap_via_cap(CPUArchState *env, uint32_t rs,
                                        uint32_t cd, uint32_t cd2, uint32_t cb,
                                        target_ulong addr)
{
    GET_HOST_RETPC();

    cap_register_t cbp = *get_capreg_or_special(env, cb);

    uint32_t perms = perms_for_store(env, cd);
    if (cd2 != REG_NONE)
        perms |= perms_for_store(env, cd2);
    uint32_t size = (cd2 == REG_NONE) ? CHERI_CAP_SIZE : (CHERI_CAP_SIZE * 2);

    cap_check_common_reg(perms, env, cb, addr, size, _host_return_address, &cbp,
                         size, true);

    // Should be storing to same address as load exclusive
    bool success = env->exclusive_addr == addr;

    uint64_t pesbt;
    uint64_t cursor;
    bool tag;

    // Fudge permissions so that no fault occur on load
    cbp.cr_perms |= CAP_PERM_LOAD;
    cbp.cr_perms &= ~CAP_PERM_LOAD_CAP;

    // Check first cap value equal
    if (success) {
        load_cap_from_memory_128_raw_tag(env, &pesbt, &cursor, cb, &cbp, addr,
                                         _host_return_address, NULL, &tag);

        if ((cursor != env->exclusive_val) || (pesbt != env->exclusive_high) ||
            (tag != env->exclusive_tag))
            success = false;
    }

    // Check second value (if any)
    if (success && cd2 != REG_NONE) {
        load_cap_from_memory_128_raw_tag(env, &pesbt, &cursor, cb, &cbp,
                                         addr + CHERI_CAP_SIZE,
                                         _host_return_address, NULL, &tag);
        if ((cursor != env->exclusive_val2) ||
            (pesbt != env->exclusive_high2) || (tag != env->exclusive_tag2))
            success = false;
    }

    if (success) {
        store_cap_to_memory(env, cd, addr, _host_return_address);
        if (cd2 != REG_NONE)
            store_cap_to_memory(env, cd2, addr + CHERI_CAP_SIZE,
                                _host_return_address);
    }

    env->exclusive_addr = -1;

    update_capreg_to_intval(env, rs, success ? 0ULL : 1ULL);
}

static void swap_cap_via_cap_impl(CPUArchState *env, uint32_t cd, uint32_t cs,
                                  uint32_t cb, target_ulong addr, bool compare)
{
    GET_HOST_RETPC();

    const cap_register_t *cbp = get_capreg_or_special(env, cb);

    uint32_t perms = perms_for_store(env, cd) | perms_for_load();

    // Capability checks for both the load and store
    cap_check_common_reg(perms, env, cb, addr, CHERI_CAP_SIZE,
                         _host_return_address, cbp, CHERI_CAP_SIZE, true);

    // load (without modifying cs as we will need it for the comparison)

    uint64_t pesbt;
    uint64_t cursor;
    bool tag = load_cap_from_memory_128(env, &pesbt, &cursor, cb, cbp, addr,
                                        _host_return_address, NULL);

    bool do_store = !compare;

    if (compare) {
        do_store = (cursor == get_without_decompress_cursor(env, cs)) &&
                   (pesbt == get_without_decompress_pesbt(env, cs)) &&
                   (tag == get_without_decompress_tag(env, cs));
    }

    // Store
    if (do_store) {
        store_cap_to_memory(env, cd, addr, _host_return_address);
    }

    // Write back to cs
    update_compressed_capreg(env, cs, pesbt, tag, cursor);
}

void helper_swap_cap_via_cap(CPUArchState *env, uint32_t cd, uint32_t cs,
                             uint32_t cb, target_ulong addr)
{
    // Yes, I really meant to swap cs/cd here.
    swap_cap_via_cap_impl(env, cs, cd, cb, addr, false);
}

void helper_compare_swap_cap_via_cap(CPUArchState *env, uint32_t cd,
                                     uint32_t cs, uint32_t cb,
                                     target_ulong addr)
{
    swap_cap_via_cap_impl(env, cd, cs, cb, addr, true);
}

// (possibly) unseal cn, load pair (data,target), branch to target, put data in
// ct, link in link. Can only unseal cn if type LPB and ct ==
// CINVOKE_DATA_REGNUM
void helper_load_pair_and_branch_and_link(CPUArchState *env, uint32_t cn,
                                          uint32_t ct, uint32_t link,
                                          target_ulong link_pc, uint32_t flags)
{

    GET_HOST_RETPC();

    cap_register_t base = *get_capreg_or_special(env, cn);

    target_ulong addr = base._cr_cursor;

    if (base.cr_tag && ct == CINVOKE_DATA_REGNUM &&
        base.cr_otype == CC128_OTYPE_LOAD_PAIR_BRANCH) {
        base.cr_otype = CAP_OTYPE_UNSEALED;
    }

    cap_check_common_reg(perms_for_load(), env, cn, addr, CHERI_CAP_SIZE * 2,
                         _host_return_address, &base, CHERI_CAP_SIZE, true);

    uint64_t target_pesbt, target_cursor;
    bool target_tag = load_cap_from_memory_128(
        env, &target_pesbt, &target_cursor, cn, &base, addr + CHERI_CAP_SIZE,
        _host_return_address, NULL);

    cap_register_t target;
    target.cached_pesbt = target_pesbt;
    cc128_decompress_raw(target_pesbt, target_cursor, target_tag, &target);

    // We do this load SECOND as it has a register-file side effect.
    load_cap_from_memory(env, ct, cn, &base, addr, _host_return_address, NULL);

    cheri_jump_and_link(env, &target, link, link_pc, flags);
}

void helper_load_and_branch_and_link(CPUArchState *env, uint32_t cn,
                                     uint32_t im, uint32_t link,
                                     target_ulong link_pc, uint32_t flags)
{
    GET_HOST_RETPC();

    cap_register_t base = *get_capreg_or_special(env, cn);

    if (base.cr_tag && cn == CINVOKE_DATA_REGNUM &&
        base.cr_otype == CC128_OTYPE_LOAD_BRANCH) {
        base.cr_otype = CAP_OTYPE_UNSEALED;
    }

#if (KEEP_BRANCH_AND_LINK_BUG)
    if (cn != 31)
#endif
        update_capreg(env, cn, &base);

    uint64_t addr = base._cr_cursor + (int64_t)(int32_t)im;

    cap_check_common_reg(perms_for_load(), env, cn, addr, CHERI_CAP_SIZE,
                         _host_return_address, &base, CHERI_CAP_SIZE, true);

    uint64_t target_pesbt, target_cursor;
    bool target_tag =
        load_cap_from_memory_128(env, &target_pesbt, &target_cursor, cn, &base,
                                 addr, _host_return_address, NULL);

    cap_register_t target;
    target.cached_pesbt = target_pesbt;
    cc128_decompress_raw(target_pesbt, target_cursor, target_tag, &target);

    cheri_jump_and_link(env, &target, link, link_pc, flags);
}

void helper_branch_sealed_pair(CPUArchState *env, uint32_t cn, uint32_t cm,
                               uint32_t link_reg, target_ulong link_pc,
                               uint32_t flags)
{

    cap_register_t target = *get_capreg_or_special(env, cn);
    cap_register_t data = *get_capreg_or_special(env, cm);

    if (target.cr_tag && data.cr_tag && cap_is_sealed_with_type(&target) &&
        cap_is_sealed_with_type(&data) &&
        (cap_get_otype(&target) == cap_get_otype(&data)) &&
        cap_has_perms(&target, CC128_PERM_BRANCH_SEALED_PAIR) &&
        cap_has_perms(&data, CC128_PERM_BRANCH_SEALED_PAIR) &&
        cap_has_perms(&target, CAP_PERM_EXECUTE) &&
        !cap_has_perms(&data, CAP_PERM_EXECUTE)) {
        cap_set_unsealed(&target);
        cap_set_unsealed(&data);
    } else {
        target.cr_tag = 0;
    }

    update_capreg(env, CINVOKE_DATA_REGNUM, &data);

    cheri_jump_and_link(env, &target, link_reg, link_pc, flags);
}

#define MORELLO_TAGS_OPS_SIZE (CHERI_CAP_SIZE << CAP_TAG_GET_MANY_SHFT)

uint64_t helper_load_tags(CPUArchState *env, uint32_t cn, target_ulong addr)
{

    GET_HOST_RETPC();

    cap_register_t base = *get_capreg_or_special(env, cn);

    cap_check_common_reg(perms_for_load(), env, cn, addr, MORELLO_TAGS_OPS_SIZE,
                         _host_return_address, &base, MORELLO_TAGS_OPS_SIZE,
                         true);

    uint32_t result = cheri_tag_get_many(env, addr, cn, NULL, GETPC());

    return (base.cr_perms & CAP_PERM_LOAD_CAP) ? result : 0;
}

void helper_store_tags(CPUArchState *env, uint64_t tags, uint32_t cn,
                       target_ulong addr)
{

    GET_HOST_RETPC();

    cap_register_t base = *get_capreg_or_special(env, cn);

    tags &= (1 << 4) - 1;

    uint32_t perms = CAP_PERM_STORE;
    if (tags)
        perms |= CAP_PERM_STORE_CAP;

    cap_check_common_reg(perms, env, cn, addr, MORELLO_TAGS_OPS_SIZE,
                         _host_return_address, &base, MORELLO_TAGS_OPS_SIZE,
                         true);

    cheri_tag_set_many(env, tags, addr, cn, NULL, GETPC());
}

void QEMU_NORETURN
helper_check_capabilities_enabled_exception(CPUArchState *env)
{
    int el =
        get_cap_enabled_target_exception_level_el(env, arm_current_el(env));
    assert(el != -1);
    raise_exception(env, EXCP_UDEF, syn_aa64_capability_access(), el);
}
