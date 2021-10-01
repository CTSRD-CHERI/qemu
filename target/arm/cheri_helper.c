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

void helper_load_cap_via_cap_mmu_idx(CPUArchState *env, uint32_t cd,
                                     uint32_t cb, target_ulong addr,
                                     uint32_t mmu_idx)
{
    GET_HOST_RETPC();
    const cap_register_t *cbp = get_capreg_or_special(env, cb);

    cap_check_common_reg(perms_for_load(), env, cb, addr, CHERI_CAP_SIZE,
                         _host_return_address, cbp, CHERI_CAP_SIZE,
                         raise_unaligned_load_exception);

    target_ulong pesbt;
    target_ulong cursor;
    bool tag = load_cap_from_memory_raw_tag_mmu_idx(
        env, &pesbt, &cursor, cb, cbp, addr, _host_return_address, NULL, NULL,
        mmu_idx);
    update_compressed_capreg(env, cd, pesbt, tag, cursor);
}

void helper_store_cap_via_cap_mmu_idx(CPUArchState *env, uint32_t cd,
                                      uint32_t cb, target_ulong addr,
                                      uint32_t mmu_idx)
{
    GET_HOST_RETPC();

    const cap_register_t *cbp = get_capreg_or_special(env, cb);

    cap_check_common_reg(perms_for_store(env, cd), env, cb, addr,
                         CHERI_CAP_SIZE, _host_return_address, cbp,
                         CHERI_CAP_SIZE, raise_unaligned_store_exception);

    store_cap_to_memory_mmu_index(env, cd, addr, _host_return_address, mmu_idx);
}

void helper_load_cap_pair_via_cap(CPUArchState *env, uint32_t cd, uint32_t cd2,
                                  uint32_t cb, target_ulong addr)
{
    GET_HOST_RETPC();

    const cap_register_t *cbp = get_capreg_or_special(env, cb);

    cap_check_common_reg(perms_for_load(), env, cb, addr, CHERI_CAP_SIZE * 2,
                         _host_return_address, cbp, CHERI_CAP_SIZE,
                         raise_unaligned_load_exception);

    // We avoid using load_cap_from_memory for the first load as it will have
    // side effects on the register file.
    uint64_t pesbt;
    uint64_t cursor;
    bool tag = load_cap_from_memory_raw(env, &pesbt, &cursor, cb, cbp, addr,
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

    cap_check_common_reg(perms_for_store(env, cd), env, cb, addr,
                         CHERI_CAP_SIZE, _host_return_address, cbp,
                         CHERI_CAP_SIZE, raise_unaligned_store_exception);
    store_cap_to_memory(env, cd, addr, _host_return_address);
    cap_check_common_reg(perms_for_store(env, cd2), env, cb,
                         addr + CHERI_CAP_SIZE, CHERI_CAP_SIZE,
                         _host_return_address, cbp, CHERI_CAP_SIZE,
                         raise_unaligned_store_exception);
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
                         _host_return_address, cbp, size,
                         raise_unaligned_load_exception);

    bool raw_tag;
    bool tag1 = load_cap_from_memory_raw_tag(
        env, &env->exclusive_high, &env->exclusive_val, cb, cbp, addr,
        _host_return_address, NULL, &raw_tag);
    env->exclusive_tag = raw_tag;

    if (cd2 != REG_NONE) {
        bool tag2 = load_cap_from_memory_raw_tag(
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

    // In order to match hardware when storing two caps we need two cap perms
    // and bounds checks and _then_ an alignment check. So have no alignment
    // requirement if storing two caps, then do the check manually.
    uint32_t size_align = (cd2 == REG_NONE) ? CHERI_CAP_SIZE : 0;

    cap_check_common_reg(perms_for_store(env, cd), env, cb, addr,
                         CHERI_CAP_SIZE, _host_return_address, &cbp, size_align,
                         raise_unaligned_store_exception);
    if (cd2 != REG_NONE) {
        cap_check_common_reg(perms_for_store(env, cd2), env, cb,
                             addr + CHERI_CAP_SIZE, CHERI_CAP_SIZE,
                             _host_return_address, &cbp, 0,
                             raise_unaligned_store_exception);
        if (!QEMU_IS_ALIGNED_P2(addr, (CHERI_CAP_SIZE * 2))) {
            raise_unaligned_store_exception(env, addr, _host_return_address);
        }
    }
    // Should be storing to same address as load exclusive
    bool success = env->exclusive_addr == addr;

    uint64_t pesbt;
    uint64_t cursor;
    bool tag;

    // Fudge permissions so that no fault occur on load
    uint32_t temp_perms = cap_get_perms(&cbp) | CAP_PERM_LOAD;
    temp_perms &= ~CAP_PERM_LOAD_CAP;
    CAP_cc(update_perms)(&cbp, temp_perms);

    // Check first cap value equal
    if (success) {
        load_cap_from_memory_raw_tag(env, &pesbt, &cursor, cb, &cbp, addr,
                                     _host_return_address, NULL, &tag);

        if ((cursor != env->exclusive_val) || (pesbt != env->exclusive_high) ||
            (tag != env->exclusive_tag))
            success = false;
    }

    // Check second value (if any)
    if (success && cd2 != REG_NONE) {
        load_cap_from_memory_raw_tag(env, &pesbt, &cursor, cb, &cbp,
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
                                  uint32_t cb, target_ulong addr, bool compare,
                                  uintptr_t _host_return_address)
{

    const cap_register_t *cbp = get_capreg_or_special(env, cb);

    uint32_t perms = perms_for_store(env, cd) | perms_for_load();

    // Capability checks for both the load and store
    // Note: For Morello, RMW ops raise unaligned load exceptions.
    cap_check_common_reg(perms, env, cb, addr, CHERI_CAP_SIZE,
                         _host_return_address, cbp, CHERI_CAP_SIZE,
                         raise_unaligned_load_exception);

    // Now do a probe early, so that we get the store fault with priority
    // (LC/SC MMU fault priority) is the reverse of cap permissions)
    bool cd_tagged = get_without_decompress_tag(env, cd);
    int mmu_index = cpu_mmu_index(env, false);

    if (cd_tagged)
        probe_cap_write(env, addr, CHERI_CAP_SIZE, mmu_index,
                        _host_return_address);

    // load (without modifying cs as we will need it for the comparison)

    uint64_t pesbt;
    uint64_t cursor;
    bool tag = load_cap_from_memory_raw(env, &pesbt, &cursor, cb, cbp, addr,
                                        _host_return_address, NULL);

    bool do_store;

    if (compare) {
        do_store = (cursor == get_without_decompress_cursor(env, cs)) &&
                   (pesbt == get_without_decompress_pesbt(env, cs)) &&
                   (tag == get_without_decompress_tag(env, cs));
    } else {
        do_store = true;
    }

    // Store
    if (do_store) {
        store_cap_to_memory(env, cd, addr, _host_return_address);
    } else {
        bool cd_tagged = get_without_decompress_tag(env, cd);
        // Even if there is no store, we possibly need an MMU permission fault
        if (!cd_tagged)
            probe_write(env, addr, CHERI_CAP_SIZE, mmu_index,
                        _host_return_address);
    }

    // Write back to cs
    update_compressed_capreg(env, cs, pesbt, tag, cursor);
}

void helper_swap_cap_via_cap(CPUArchState *env, uint32_t cd, uint32_t cs,
                             uint32_t cb, target_ulong addr)
{
    // Yes, I really meant to swap cs/cd here.
    GET_HOST_RETPC();
    swap_cap_via_cap_impl(env, cs, cd, cb, addr, false, _host_return_address);
}

void helper_compare_swap_cap_via_cap(CPUArchState *env, uint32_t cd,
                                     uint32_t cs, uint32_t cb,
                                     target_ulong addr)
{
    GET_HOST_RETPC();
    swap_cap_via_cap_impl(env, cd, cs, cb, addr, true, _host_return_address);
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
        cap_get_otype_unsigned(&base) == CAP_OTYPE_LOAD_PAIR_BRANCH) {
        CAP_cc(update_otype)(&base, CAP_OTYPE_UNSEALED);
    }

    cap_check_common_reg(perms_for_load(), env, cn, addr, CHERI_CAP_SIZE * 2,
                         _host_return_address, &base, CHERI_CAP_SIZE,
                         raise_unaligned_load_exception);

    uint64_t target_pesbt, target_cursor;
    bool target_tag = load_cap_from_memory_raw(
        env, &target_pesbt, &target_cursor, cn, &base, addr + CHERI_CAP_SIZE,
        _host_return_address, NULL);

    cap_register_t target;
    target.cr_pesbt = target_pesbt;
    CAP_cc(decompress_raw)(target_pesbt, target_cursor, target_tag, &target);

    // We do this load SECOND as it has a register-file side effect.
    load_cap_from_memory(env, ct, cn, &base, addr, _host_return_address, NULL);

    cheri_jump_and_link(env, &target, cap_get_cursor(&target), link, link_pc,
                        flags);
}

void helper_load_and_branch_and_link(CPUArchState *env, uint32_t cn,
                                     uint32_t im, uint32_t link,
                                     target_ulong link_pc, uint32_t flags)
{
    GET_HOST_RETPC();

    cap_register_t base = *get_capreg_or_special(env, cn);

    if (base.cr_tag && cn == CINVOKE_DATA_REGNUM &&
        cap_get_otype_unsigned(&base) == CAP_OTYPE_LOAD_BRANCH) {
        CAP_cc(update_otype)(&base, CAP_OTYPE_UNSEALED);
    }

    update_capreg(env, cn, &base);

    uint64_t addr = base._cr_cursor + (int64_t)(int32_t)im;

    cap_check_common_reg(perms_for_load(), env, cn, addr, CHERI_CAP_SIZE,
                         _host_return_address, &base, CHERI_CAP_SIZE,
                         raise_unaligned_load_exception);

    uint64_t target_pesbt, target_cursor;
    bool target_tag =
        load_cap_from_memory_raw(env, &target_pesbt, &target_cursor, cn, &base,
                                 addr, _host_return_address, NULL);

    cap_register_t target;
    target.cr_pesbt = target_pesbt;
    CAP_cc(decompress_raw)(target_pesbt, target_cursor, target_tag, &target);

    cheri_jump_and_link(env, &target, cap_get_cursor(&target), link, link_pc,
                        flags);
}

void helper_branch_sealed_pair(CPUArchState *env, uint32_t cn, uint32_t cm,
                               uint32_t link_reg, target_ulong link_pc,
                               uint32_t flags)
{

    cap_register_t target = *get_capreg_or_special(env, cn);
    cap_register_t data = *get_capreg_or_special(env, cm);

    if (target.cr_tag && data.cr_tag && cap_is_sealed_with_type(&target) &&
        cap_is_sealed_with_type(&data) &&
        (cap_get_otype_unsigned(&target) == cap_get_otype_unsigned(&data)) &&
        cap_has_perms(&target, CAP_PERM_BRANCH_SEALED_PAIR) &&
        cap_has_perms(&data, CAP_PERM_BRANCH_SEALED_PAIR) &&
        cap_has_perms(&target, CAP_PERM_EXECUTE) &&
        !cap_has_perms(&data, CAP_PERM_EXECUTE)) {
        cap_set_unsealed(&target);
        cap_set_unsealed(&data);
    } else {
        target.cr_tag = 0;
    }

    update_capreg(env, CINVOKE_DATA_REGNUM, &data);

    cheri_jump_and_link(env, &target, cap_get_cursor(&target), link_reg,
                        link_pc, flags);
}

#define MORELLO_TAGS_OPS_SIZE (CHERI_CAP_SIZE << CAP_TAG_GET_MANY_SHFT)

uint64_t helper_load_tags(CPUArchState *env, uint32_t cn, target_ulong addr)
{

    GET_HOST_RETPC();

    cap_register_t base = *get_capreg_or_special(env, cn);

    cap_check_common_reg(perms_for_load(), env, cn, addr, MORELLO_TAGS_OPS_SIZE,
                         _host_return_address, &base, MORELLO_TAGS_OPS_SIZE,
                         raise_unaligned_load_exception);

    uint32_t result = cheri_tag_get_many(env, addr, cn, NULL, GETPC());

    return cap_has_perms(&base, CAP_PERM_LOAD_CAP) ? result : 0;
}

void helper_store_tags(CPUArchState *env, uint64_t tags, uint32_t cn,
                       target_ulong addr)
{

    GET_HOST_RETPC();

    cap_register_t base = *get_capreg_or_special(env, cn);

    tags &= (1 << 4) - 1;

    assert(!tags || (cheri_is_system(env) &&
                     !arm_is_tag_setting_disabled(env, arm_current_el(env))));

    uint32_t perms = CAP_PERM_STORE;
    if (tags)
        perms |= CAP_PERM_STORE_CAP;

    cap_check_common_reg(perms, env, cn, addr, MORELLO_TAGS_OPS_SIZE,
                         _host_return_address, &base, MORELLO_TAGS_OPS_SIZE,
                         raise_unaligned_store_exception);

    cheri_tag_set_many(env, tags, addr, cn, NULL, GETPC());
}

void helper_set_pcc(CPUArchState *env, target_ulong addr)
{
    set_aarch_reg_value(&env->pc, addr);
}

void QEMU_NORETURN
helper_check_capabilities_enabled_exception(CPUArchState *env)
{
    int el =
        get_cap_enabled_target_exception_level_el(env, arm_current_el(env));
    assert(el != -1);
    raise_exception(env, EXCP_UDEF, syn_aa64_capability_access(), el);
}

void QEMU_NORETURN helper_sys_not_accessible_exception(CPUArchState *env,
                                                       uint32_t syndrome)
{
    int el = exception_target_el_capability(env);
    raise_exception(env, EXCP_UDEF, syndrome, el);
}
