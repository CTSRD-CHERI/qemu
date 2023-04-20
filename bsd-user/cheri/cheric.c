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

#include "qemu.h"

#include "target/cheri-common/cheri_utils.h"

#include "cheri/cheric.h"

/*
 * See crap_impl() from target/cheri-common/op_helper_cheri_common.c.
 */
target_long
__cheri_round_representable_length(target_ulong len)
{
#if CHERI_CAP_BITS == 128
    cap_register_t tmpcap;

    set_max_perms_capability(&tmpcap, 0);
    cc128_setbounds(&tmpcap, 0, len);
    return ((target_ulong)cap_get_length_full(&tmpcap));
#else

    return (len);
#endif
}

cap_register_t *
cheri_nullcap(void)
{
    static cap_register_t cap;

    null_capability(&cap);
    cap_set_state(&cap, CREG_FULLY_DECOMPRESSED);
    return (&cap);
}

cap_register_t *
cheri_zerocap(void)
{
    cap_register_t *cap;

    cap = cheri_nullcap();
    set_max_perms_capability(cap, 0);
    return (cap);
}

cap_register_t *
cheri_ptr_to_unbounded_cap(const void *ptr)
{

    return (cheri_setaddress(cheri_zerocap(), (intptr_t)ptr));
}

cap_register_t *
cheri_ptr(const void *ptr, size_t len)
{

    return (cheri_setbounds(cheri_ptr_to_unbounded_cap(ptr), len));
}

abi_uintcap_t
cheri_uintcap(const cap_register_t *cap)
{
    abi_uintcap_t uintcap;

    cheri_store(&uintcap, cap);
    return (uintcap);
}

#ifdef TARGET_CHERI_PURE_CAPABILITY
abi_uintptr_t
cheri_uintptr(const cap_register_t *cap)
{

    return ((abi_uintptr_t)cheri_uintcap(cap));
}
#else
abi_uintptr_t
cheri_uintptr(const cap_register_t *cap)
{

    return ((abi_uintptr_t)cap_get_cursor(cap));
}
#endif

cap_register_t *
cheri_load(cap_register_t *cap, const abi_uintcap_t *value)
{
    uint64_t cursor, pesbt;

    pesbt = ldq_p((const uint8_t *)value + CHERI_MEM_OFFSET_METADATA) ^
        CC128_NULL_XOR_MASK;
    cursor = ldq_p((const uint8_t *)value + CHERI_MEM_OFFSET_CURSOR);

    cc128_decompress_raw(pesbt, cursor, true, cap);
    cap_set_state(cap, CREG_FULLY_DECOMPRESSED);
    return (cap);
}

void
cheri_store(void *ptr, const cap_register_t *cap)
{

    *(uint64_t *)(ptr + CHERI_MEM_OFFSET_METADATA) = cc128_compress_raw(cap) ^
        CC128_NULL_XOR_MASK;
    *(uint64_t *)((uint8_t *)ptr + CHERI_MEM_OFFSET_CURSOR) =
        cap_get_cursor(cap);
}

abi_ulong
cheri_getlen(const cap_register_t *cap)
{

    return ((target_ulong)cap_get_length_sat(cap));
}

abi_ulong
cheri_getbase(const cap_register_t *cap)
{

    return (cap_get_base(cap));
}

cap_offset_t
cheri_getoffset(const cap_register_t *cap)
{

    return (cap_get_offset(cap));
}

abi_ulong
cheri_getaddress(const cap_register_t *cap)
{

    return (cap_get_cursor(cap));
}

uint32_t
cheri_getperm(const cap_register_t *cap)
{

    return ((cap_get_perms(cap) & CAP_PERMS_ALL) |
        ((cap_get_uperms(cap) & CAP_UPERMS_ALL) << CAP_UPERMS_SHFT));
}

bool
cheri_gettag(const cap_register_t *cap)
{

    return (cap->cr_tag);
}

cap_register_t *
cheri_andperm(cap_register_t *cap, uint32_t perms)
{

    cap_and_perms(cap, perms);
    return (cap);
}

cap_register_t *
cheri_setbounds(cap_register_t *cap, size_t length)
{

    cc128_setbounds(cap, cap_get_cursor(cap),
        (unsigned __int128)cap_get_cursor(cap) + length);
    return (cap);
}

cap_register_t *
cheri_incoffset(cap_register_t *cap, off_t diff)
{

    cap_set_cursor(cap, cap_get_cursor(cap) + diff);
    return (cap);
}

cap_register_t *
cheri_setoffset(cap_register_t *cap, off_t off)
{

    return (cheri_incoffset(cap, off - cheri_getoffset(cap)));
}

cap_register_t *
cheri_setaddress(cap_register_t *cap, target_ulong addr)
{

    return (cheri_incoffset(cap, addr - cap_get_cursor(cap)));
}

cap_register_t *
cheri_setflags(cap_register_t *cap, target_ulong flags)
{

    cap_set_flags(cap, flags);
    return (cap);
}

cap_register_t *
cheri_sealentry(cap_register_t *cap)
{

    cap_make_sealed_entry(cap);
    return (cap);
}

cap_register_t *
cheri_fromddc(const cap_register_t *ddc, abi_ulong ptr)
{
    static cap_register_t cap;

    cap = *ddc;
    return (cheri_setaddress(&cap, (intptr_t)ptr));
}

cap_register_t *
cheri_fromint(abi_int value)
{

    return (cheri_incoffset(cheri_nullcap(), value));
}
