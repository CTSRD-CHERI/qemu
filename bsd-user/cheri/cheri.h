/*-
 * Copyright (c) 2011-2017 Robert N. M. Watson
 * Copyright (c) 2015 SRI International
 * All rights reserved.
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory under DARPA/AFRL contract (FA8750-10-C-0237)
 * ("CTSRD"), as part of the DARPA CRASH research programme.
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
 * This file is a port of sys/cheri/cheri.h from CTSRD-CHERI/CheriBSD.
 */

#ifndef _CHERI_CHERI_H_
#define	_CHERI_CHERI_H_

/*
 * Functions to construct userspace capabilities.
 */
cap_register_t *cheri_capability_build_user_code(cap_register_t *cap,
    uint32_t perms, abi_ulong basep, size_t length, off_t off);
cap_register_t *cheri_capability_build_user_rwx(cap_register_t *cap,
    uint32_t perms, abi_ulong basep, size_t length, off_t off);

/*
 * Global capabilities used to construct other capabilities.
 */

/* Root of all unsealed userspace capabilities. */
extern cap_register_t userspace_cap;
extern cap_register_t userspace_sealcap;

/*
 * Functions to create capabilities used in exec.
 */
cap_register_t *cheri_auxv_capability(cap_register_t *cap);
cap_register_t *cheri_exec_pcc(cap_register_t *cap, struct image_info *info);
cap_register_t *cheri_exec_stack_pointer(cap_register_t *cap, abi_ulong stack);
void cheri_set_mmap_capability(cap_register_t *mmapcapp,
    struct image_info *info, const cap_register_t *csp);
cap_register_t *cheri_sigcode_capability(cap_register_t *cap,
    struct image_info *info);

#endif /* _CHERI_CHERI_H_ */
