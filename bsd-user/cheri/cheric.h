/*-
 * Copyright (c) 2013-2016 Robert N. M. Watson
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
 * This file is a port of sys/cheri/cheric.h from CTSRD-CHERI/CheriBSD.
 */

#ifndef _CHERI_CHERIC_H_
#define	_CHERI_CHERIC_H_

/*
 * Functions emulating CHERI compiler built-in functions.
 */
target_long __cheri_round_representable_length(target_ulong len);

/*
 * Programmer-friendly helpers for C code emulating CHERI-aware code.
 */
cap_register_t    *cheri_zerocap(void);
cap_register_t    *cheri_ptr(const void *ptr, size_t len);

cap_register_t    *cheri_load(cap_register_t *cap, const abi_uintcap_t *value);
void               cheri_store(void *ptr, const cap_register_t *cap);

/*
 * Programmer-friendly functions for C code emulating CHERI-aware code.
 */
abi_ulong          cheri_getlen(const cap_register_t *cap);
abi_ulong          cheri_getbase(const cap_register_t *cap);
cap_offset_t       cheri_getoffset(const cap_register_t *cap);
abi_ulong          cheri_getaddress(const cap_register_t *cap);
uint32_t           cheri_getperm(const cap_register_t *cap);
bool               cheri_gettag(const cap_register_t *cap);

cap_register_t    *cheri_andperm(cap_register_t *cap, uint32_t perms);
cap_register_t    *cheri_setbounds(cap_register_t *cap, size_t length);
cap_register_t    *cheri_incoffset(cap_register_t *cap, off_t diff);
cap_register_t    *cheri_setoffset(cap_register_t *cap, off_t off);
cap_register_t    *cheri_setaddress(cap_register_t *cap, target_ulong addr);
cap_register_t    *cheri_setflags(cap_register_t *cap, target_ulong flags);

cap_register_t    *cheri_sealentry(cap_register_t *cap);

/* Get the top of a capability (i.e. one byte past the last accessible one) */
static inline abi_ulong
cheri_gettop(const cap_register_t *cap)
{
    return (cheri_getbase(cap) + cheri_getlen(cap));
}

#endif /* _CHERI_CHERIC_H_ */
