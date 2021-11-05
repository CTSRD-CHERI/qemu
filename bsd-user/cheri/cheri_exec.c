/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2020 John Baldwin
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
 * This file is a port of sys/cheri/cheri_exec.c from CTSRD-CHERI/CheriBSD.
 */

#include "qemu.h"

#include "machine/vmparam.h"

#include "cheri/cherireg.h"
#include "cheri/cheri.h"
#include "cheri/cheric.h"

/*
 * Construct a capability stack pointer for the main thread.
 */
cap_register_t *
cheri_exec_stack_pointer(cap_register_t *cap, abi_ulong stack)
{
    vm_offset_t stackbase, stacktop;
    size_t stacklen;
    cap_register_t *csp;

    assert((stack % ABI_PTR_SIZE) == 0);

    /*
     * Restrict the stack capability to the maximum region allowed
     * for this process and adjust csp accordingly.
     */
    stackbase = target_stkbas;
    stacktop = stack;
    assert(stacktop > stackbase);
    stacklen = stacktop - stackbase;

    /*
     * Round the stack down as required to make it representable.
     */
    stacklen = rounddown2(stacklen, CHERI_REPRESENTABLE_ALIGNMENT(stacklen));
    assert(stackbase == CHERI_REPRESENTABLE_BASE(stackbase, stacklen));
    csp = cheri_setaddress(cap, stackbase);
    csp = cheri_setbounds(csp, stacklen);
    return (cheri_incoffset(csp, stacklen));
}

/*
 * Build a capability to describe the MMAP'able space from the end of
 * the program's heap to the bottom of the stack.
 *
 * XXX: We could probably use looser bounds and rely on the VM system
 * to manage the address space via vm_map instead of the more complex
 * calculations here.
 */
void
cheri_set_mmap_capability(cap_register_t *mmapcapp, struct image_info *info,
    const cap_register_t *csp)
{
    vm_offset_t map_base, stack_base, text_end;
    size_t map_length;

    stack_base = cheri_getbase(csp);
    text_end = roundup2(info->end_addr,
        CHERI_SEALABLE_ALIGNMENT(info->end_addr - info->start_addr));

    /*
     * Less confusing rounded up to a page and 256-bit
     * requires no other rounding.
     */
    text_end = roundup2(text_end, PAGE_SIZE);
    assert(text_end <= stack_base);

    map_base = (text_end == stack_base) ?
        CHERI_CAP_USER_MMAP_BASE :
        roundup2(text_end,
        CHERI_REPRESENTABLE_ALIGNMENT(stack_base - text_end));
    assert(map_base < stack_base);
    map_length = stack_base - map_base;
    map_length = rounddown2(map_length,
        CHERI_REPRESENTABLE_ALIGNMENT(map_length));

    /*
     * Use cheri_capability_build_user_rwx so mmap() can return
     * appropriate permissions derived from a single capability.
     */
    (void)cheri_capability_build_user_rwx(
        mmapcapp, CHERI_CAP_USER_MMAP_PERMS, map_base, map_length,
        CHERI_CAP_USER_MMAP_OFFSET);
    assert(cheri_getperm(mmapcapp) & CHERI_PERM_CHERIABI_VMMAP);
}

cap_register_t *
cheri_exec_pcc(cap_register_t *cap, struct image_info *info)
{
    abi_ulong code_start, code_end;
    size_t code_length;

    /*
     * If we are executing a static binary we use end_addr as the
     * end of the text segment. If $pcc is the start of rtld we
     * use interp_end.  If we are executing rtld directly we can
     * use end_addr to find the end of the rtld mapping.
     */
    if (info->interp_end != 0) {
        code_start = info->reloc_base;
        code_end = info->interp_end;
    } else {
        code_start = info->start_addr;
        code_end = info->end_addr;
    }

    /* Ensure CHERI128 representability */
    code_length = code_end - code_start;
    code_start = CHERI_REPRESENTABLE_BASE(code_start, code_length);
    code_length = CHERI_REPRESENTABLE_LENGTH(code_length);
    assert(code_start + code_length >= code_end);
    return (cheri_capability_build_user_code(cap, CHERI_CAP_USER_CODE_PERMS,
        code_start, code_length, info->entry - code_start));
}

cap_register_t *
cheri_sigcode_capability(cap_register_t *cap)
{
    vm_offset_t sigcode_base;

    sigcode_base = TARGET_PS_STRINGS - TARGET_SZSIGCODE;
    assert(sigcode_base != 0);
    return (cheri_sealentry(cheri_capability_build_user_code(cap,
        CHERI_CAP_USER_CODE_PERMS, sigcode_base, TARGET_SZSIGCODE, 0)));
}

cap_register_t *
cheri_auxv_capability(cap_register_t *cap)
{

    return (cheri_setbounds(cheri_setaddress(cap, target_auxents),
        target_auxents_sz));
}
