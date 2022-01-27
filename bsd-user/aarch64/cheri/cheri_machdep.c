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

#include "qemu.h"

#include "cheri-archspecific.h"

#include "machine/vmparam.h"
#include "machine/cheri.h"

#include "cheri/cherireg.h"
#include "cheri/cheri.h"
#include "cheri/cheric.h"

void
cheri_init_capabilities(const cap_register_t *kroot)
{

    userspace_cap = *kroot;
    (void)cheri_setaddress(&userspace_cap, CHERI_CAP_USER_DATA_BASE);
    (void)cheri_setbounds(&userspace_cap, CHERI_CAP_USER_DATA_LENGTH);
    (void)cheri_andperm(&userspace_cap, CHERI_CAP_USER_DATA_PERMS |
        CHERI_CAP_USER_CODE_PERMS | CHERI_PERM_CHERIABI_VMMAP);

    userspace_sealcap = *kroot;
    (void)cheri_setaddress(&userspace_sealcap, CHERI_SEALCAP_USERSPACE_BASE);
    (void)cheri_setbounds(&userspace_sealcap, CHERI_SEALCAP_USERSPACE_LENGTH);
    (void)cheri_andperm(&userspace_sealcap, CHERI_SEALCAP_USERSPACE_PERMS);
}

void
cheri_prepare_pcc(cap_register_t *pcc, CPUARMState *env)
{

    if (env->pstate & PSTATE_C64) {
        /*
         * Encode C64 state as required by TCG.
         */
        pcc->_cr_cursor |= 1;
    }
    if (cap_is_sealed_entry(pcc)) {
        cap_unseal_entry(pcc);
    }
    /*
     * XXXKW: We shouln't have to forcefully rebuild chflags here but for some
     * reason TCG flips PSTATE_C64 which results in a TCG cheriflags mismatch.
     * This needs to be further investigated.
     */
    arm_rebuild_chflags(env);
    update_next_pcc_for_tcg(env, pcc, 0);
}
