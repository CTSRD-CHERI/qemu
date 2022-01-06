#ifndef EXEC_USER_ABITYPES_H
#define EXEC_USER_ABITYPES_H

#include "cpu.h"

#ifdef TARGET_ABI32
#define TARGET_ABI_BITS 32
#else
#define TARGET_ABI_BITS TARGET_LONG_BITS
#endif

#ifdef TARGET_CHERI
#define ABI_PTR_SIZE    CHERI_CAP_SIZE
#else
#define ABI_PTR_SIZE    (TARGET_ABI_BITS / 8)
#endif

#ifdef TARGET_M68K
#define ABI_INT_ALIGNMENT 2
#define ABI_LONG_ALIGNMENT 2
#define ABI_LLONG_ALIGNMENT 2
#endif

#if (defined(TARGET_I386) && !defined(TARGET_X86_64)) || defined(TARGET_SH4)
#define ABI_LLONG_ALIGNMENT 4
#endif

#ifndef ABI_SHORT_ALIGNMENT
#define ABI_SHORT_ALIGNMENT 2
#endif
#ifndef ABI_INT_ALIGNMENT
#define ABI_INT_ALIGNMENT 4
#endif
#ifndef ABI_LONG_ALIGNMENT
#define ABI_LONG_ALIGNMENT (TARGET_ABI_BITS / 8)
#endif
#ifndef ABI_LLONG_ALIGNMENT
#define ABI_LLONG_ALIGNMENT 8
#endif

typedef int16_t abi_short __attribute__ ((aligned(ABI_SHORT_ALIGNMENT)));
typedef uint16_t abi_ushort __attribute__((aligned(ABI_SHORT_ALIGNMENT)));
typedef int32_t abi_int __attribute__((aligned(ABI_INT_ALIGNMENT)));
typedef uint32_t abi_uint __attribute__((aligned(ABI_INT_ALIGNMENT)));
typedef int64_t abi_llong __attribute__((aligned(ABI_LLONG_ALIGNMENT)));
typedef uint64_t abi_ullong __attribute__((aligned(ABI_LLONG_ALIGNMENT)));

#ifdef TARGET_ABI32
typedef uint32_t abi_ulong __attribute__((aligned(ABI_LONG_ALIGNMENT)));
typedef int32_t abi_long __attribute__((aligned(ABI_LONG_ALIGNMENT)));
#define TARGET_ABI_FMT_lx "%08x"
#define TARGET_ABI_FMT_ld "%d"
#define TARGET_ABI_FMT_lu "%u"

static inline abi_ulong tswapal(abi_ulong v)
{
    return tswap32(v);
}

#else
typedef target_ulong abi_ulong __attribute__((aligned(ABI_LONG_ALIGNMENT)));
typedef target_long abi_long __attribute__((aligned(ABI_LONG_ALIGNMENT)));
#define TARGET_ABI_FMT_lx TARGET_FMT_lx
#define TARGET_ABI_FMT_ld TARGET_FMT_ld
#define TARGET_ABI_FMT_lu TARGET_FMT_lu
/* for consistency, define ABI32 too */
#if TARGET_ABI_BITS == 32
#define TARGET_ABI32 1
#endif

typedef target_ulong abi_vaddr_t;
#ifdef TARGET_CHERI
/*
 * XXXKW: abi_uintcap_t should reorder fields depending on
 * CHERI_MEM_OFFSET_METADATA and CHERI_MEM_OFFSET_CURSOR.
 */
typedef struct {
    uint64_t cursor;
    uint64_t pesbt;
} abi_uintcap_t __attribute__ ((aligned(CHERI_CAP_SIZE)));
typedef abi_uintcap_t abi_uintptr_t;

#if CHERI_CAP_BITS == 128 && !defined(TARGET_WORDS_BIGENDIAN)
typedef const cap_register_t * abi_syscallarg_t;
typedef cap_register_t * abi_syscallret_t;
#else
#error abi_syscallarg_t and abi_syscallret_t are defined only for little endian CHERI128
#endif
#else /* !TARGET_CHERI */
typedef abi_long abi_uintptr_t __attribute__((aligned(ABI_LONG_ALIGNMENT)));
typedef abi_uintptr_t abi_uintcap_t;

typedef abi_long * abi_syscallarg_t;
typedef abi_long * abi_syscallret_t;
#endif /* TARGET_CHERI */

#ifdef TARGET_CHERI
#define uintptr_vaddr(x) ((abi_vaddr_t)(x).cursor)
#define vaddr_uintptr(x) (cheri_uintptr(cheri_setaddress(cheri_zerocap(), (x))))

/*
 * XXXKW: QEMU uses NULL for a non-existing system call argument.
 */
#define syscallarg_value(sa)   ((abi_long)((sa != NULL) ? (sa)->_cr_cursor : 0))
#define syscallret_value(sa)   ((abi_long)(sa)->_cr_cursor)

#define syscallarg_uintptr(sa) (cheri_uintptr(sa))
#else
#define uintptr_vaddr(x) ((abi_vaddr_t)(x))
#define vaddr_uintptr(x) ((abi_uintptr_t)(x))

#define syscallarg_value(sa) ((abi_long)(sa != NULL) ? *(sa) : 0)
#define syscallret_value(sa) (*(sa))

#define syscallarg_uintptr(sa) ((abi_uintptr_t)(sa))
#endif
#define syscallarg_int(sa)     ((abi_long)syscallarg_value(sa))

static inline abi_ulong tswapal(abi_ulong v)
{
    return tswapl(v);
}

#ifdef TARGET_CHERI
static inline abi_uintptr_t
tswapuintptr(abi_uintptr_t v)
{

    v.cursor = tswapal(v.cursor);
    v.pesbt = tswapal(v.pesbt);

    return (v);
}
#else
#define tswapuintptr tswapal
#endif

#endif
#endif
