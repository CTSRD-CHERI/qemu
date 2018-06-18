/*
 * Atomic helper templates
 * Included from tcg-runtime.c and cputlb.c.
 *
 * Copyright (c) 2016 Red Hat, Inc
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#if DATA_SIZE == 16
# define SUFFIX     o
# define DATA_TYPE  Int128
# define BSWAP      bswap128
#elif DATA_SIZE == 8
# define SUFFIX     q
# define DATA_TYPE  uint64_t
# define SDATA_TYPE int64_t
# define BSWAP      bswap64
#elif DATA_SIZE == 4
# define SUFFIX     l
# define DATA_TYPE  uint32_t
# define SDATA_TYPE int32_t
# define BSWAP      bswap32
#elif DATA_SIZE == 2
# define SUFFIX     w
# define DATA_TYPE  uint16_t
# define SDATA_TYPE int16_t
# define BSWAP      bswap16
#elif DATA_SIZE == 1
# define SUFFIX     b
# define DATA_TYPE  uint8_t
# define SDATA_TYPE int8_t
# define BSWAP
#else
# error unsupported data size
#endif

#if DATA_SIZE >= 4
# define ABI_TYPE  DATA_TYPE
#else
# define ABI_TYPE  uint32_t
#endif

/* Define host-endian atomic operations.  Note that END is used within
   the ATOMIC_NAME macro, and redefined below.  */
#if DATA_SIZE == 1
# define END
#elif defined(HOST_WORDS_BIGENDIAN)
# define END  _be
#else
# define END  _le
#endif

ABI_TYPE ATOMIC_NAME(cmpxchg)(CPUArchState *env, target_ulong addr,
                              ABI_TYPE cmpv, ABI_TYPE newv EXTRA_ARGS)
{
    ATOMIC_MMU_DECLS;
    DATA_TYPE *haddr = ATOMIC_MMU_LOOKUP;
    DATA_TYPE ret = atomic_cmpxchg__nocheck(haddr, cmpv, newv);
    ATOMIC_MMU_CLEANUP;
    return ret;
}

#if DATA_SIZE >= 16
ABI_TYPE ATOMIC_NAME(ld)(CPUArchState *env, target_ulong addr EXTRA_ARGS)
{
    ATOMIC_MMU_DECLS;
    DATA_TYPE val, *haddr = ATOMIC_MMU_LOOKUP;
    __atomic_load(haddr, &val, __ATOMIC_RELAXED);
    ATOMIC_MMU_CLEANUP;
    return val;
}

void ATOMIC_NAME(st)(CPUArchState *env, target_ulong addr,
                     ABI_TYPE val EXTRA_ARGS)
{
    ATOMIC_MMU_DECLS;
    DATA_TYPE *haddr = ATOMIC_MMU_LOOKUP;
    __atomic_store(haddr, &val, __ATOMIC_RELAXED);
    ATOMIC_MMU_CLEANUP;
}
#else
ABI_TYPE ATOMIC_NAME(xchg)(CPUArchState *env, target_ulong addr,
                           ABI_TYPE val EXTRA_ARGS)
{
    ATOMIC_MMU_DECLS;
    DATA_TYPE *haddr = ATOMIC_MMU_LOOKUP;
    DATA_TYPE ret = atomic_xchg__nocheck(haddr, val);
    ATOMIC_MMU_CLEANUP;
    return ret;
}

#define GEN_ATOMIC_HELPER(X)                                        \
ABI_TYPE ATOMIC_NAME(X)(CPUArchState *env, target_ulong addr,       \
                 ABI_TYPE val EXTRA_ARGS)                           \
{                                                                   \
    ATOMIC_MMU_DECLS;                                               \
    DATA_TYPE *haddr = ATOMIC_MMU_LOOKUP;                           \
    DATA_TYPE ret = atomic_##X(haddr, val);                         \
    ATOMIC_MMU_CLEANUP;                                             \
    return ret;                                                     \
}

GEN_ATOMIC_HELPER(fetch_add)
GEN_ATOMIC_HELPER(fetch_and)
GEN_ATOMIC_HELPER(fetch_or)
GEN_ATOMIC_HELPER(fetch_xor)
GEN_ATOMIC_HELPER(add_fetch)
GEN_ATOMIC_HELPER(and_fetch)
GEN_ATOMIC_HELPER(or_fetch)
GEN_ATOMIC_HELPER(xor_fetch)

#undef GEN_ATOMIC_HELPER

/* These helpers are, as a whole, full barriers.  Within the helper,
 * the leading barrier is explicit and the trailing barrier is within
 * cmpxchg primitive.
 */
#define GEN_ATOMIC_HELPER_FN(X, FN, XDATA_TYPE, RET)                \
ABI_TYPE ATOMIC_NAME(X)(CPUArchState *env, target_ulong addr,       \
                        ABI_TYPE xval EXTRA_ARGS)                   \
{                                                                   \
    ATOMIC_MMU_DECLS;                                               \
    XDATA_TYPE *haddr = ATOMIC_MMU_LOOKUP;                          \
    XDATA_TYPE cmp, old, new, val = xval;                           \
    smp_mb();                                                       \
    cmp = atomic_read__nocheck(haddr);                              \
    do {                                                            \
        old = cmp; new = FN(old, val);                              \
        cmp = atomic_cmpxchg__nocheck(haddr, old, new);             \
    } while (cmp != old);                                           \
    ATOMIC_MMU_CLEANUP;                                             \
    return RET;                                                     \
}

GEN_ATOMIC_HELPER_FN(fetch_smin, MIN, SDATA_TYPE, old)
GEN_ATOMIC_HELPER_FN(fetch_umin, MIN,  DATA_TYPE, old)
GEN_ATOMIC_HELPER_FN(fetch_smax, MAX, SDATA_TYPE, old)
GEN_ATOMIC_HELPER_FN(fetch_umax, MAX,  DATA_TYPE, old)

GEN_ATOMIC_HELPER_FN(smin_fetch, MIN, SDATA_TYPE, new)
GEN_ATOMIC_HELPER_FN(umin_fetch, MIN,  DATA_TYPE, new)
GEN_ATOMIC_HELPER_FN(smax_fetch, MAX, SDATA_TYPE, new)
GEN_ATOMIC_HELPER_FN(umax_fetch, MAX,  DATA_TYPE, new)

#undef GEN_ATOMIC_HELPER_FN
#endif /* DATA SIZE >= 16 */

#undef END

#if DATA_SIZE > 1

/* Define reverse-host-endian atomic operations.  Note that END is used
   within the ATOMIC_NAME macro.  */
#ifdef HOST_WORDS_BIGENDIAN
# define END  _le
#else
# define END  _be
#endif

ABI_TYPE ATOMIC_NAME(cmpxchg)(CPUArchState *env, target_ulong addr,
                              ABI_TYPE cmpv, ABI_TYPE newv EXTRA_ARGS)
{
    ATOMIC_MMU_DECLS;
    DATA_TYPE *haddr = ATOMIC_MMU_LOOKUP;
    DATA_TYPE ret = atomic_cmpxchg__nocheck(haddr, BSWAP(cmpv), BSWAP(newv));
    ATOMIC_MMU_CLEANUP;
    return BSWAP(ret);
}

#if DATA_SIZE >= 16
ABI_TYPE ATOMIC_NAME(ld)(CPUArchState *env, target_ulong addr EXTRA_ARGS)
{
    ATOMIC_MMU_DECLS;
    DATA_TYPE val, *haddr = ATOMIC_MMU_LOOKUP;
    __atomic_load(haddr, &val, __ATOMIC_RELAXED);
    ATOMIC_MMU_CLEANUP;
    return BSWAP(val);
}

void ATOMIC_NAME(st)(CPUArchState *env, target_ulong addr,
                     ABI_TYPE val EXTRA_ARGS)
{
    ATOMIC_MMU_DECLS;
    DATA_TYPE *haddr = ATOMIC_MMU_LOOKUP;
    val = BSWAP(val);
    __atomic_store(haddr, &val, __ATOMIC_RELAXED);
    ATOMIC_MMU_CLEANUP;
}
#else
ABI_TYPE ATOMIC_NAME(xchg)(CPUArchState *env, target_ulong addr,
                           ABI_TYPE val EXTRA_ARGS)
{
    ATOMIC_MMU_DECLS;
    DATA_TYPE *haddr = ATOMIC_MMU_LOOKUP;
    ABI_TYPE ret = atomic_xchg__nocheck(haddr, BSWAP(val));
    ATOMIC_MMU_CLEANUP;
    return BSWAP(ret);
}

#define GEN_ATOMIC_HELPER(X)                                        \
ABI_TYPE ATOMIC_NAME(X)(CPUArchState *env, target_ulong addr,       \
                 ABI_TYPE val EXTRA_ARGS)                           \
{                                                                   \
    ATOMIC_MMU_DECLS;                                               \
    DATA_TYPE *haddr = ATOMIC_MMU_LOOKUP;                           \
    DATA_TYPE ret = atomic_##X(haddr, BSWAP(val));                  \
    ATOMIC_MMU_CLEANUP;                                             \
    return BSWAP(ret);                                              \
}

GEN_ATOMIC_HELPER(fetch_and)
GEN_ATOMIC_HELPER(fetch_or)
GEN_ATOMIC_HELPER(fetch_xor)
GEN_ATOMIC_HELPER(and_fetch)
GEN_ATOMIC_HELPER(or_fetch)
GEN_ATOMIC_HELPER(xor_fetch)

#undef GEN_ATOMIC_HELPER

/* These helpers are, as a whole, full barriers.  Within the helper,
 * the leading barrier is explicit and the trailing barrier is within
 * cmpxchg primitive.
 */
#define GEN_ATOMIC_HELPER_FN(X, FN, XDATA_TYPE, RET)                \
ABI_TYPE ATOMIC_NAME(X)(CPUArchState *env, target_ulong addr,       \
                        ABI_TYPE xval EXTRA_ARGS)                   \
{                                                                   \
    ATOMIC_MMU_DECLS;                                               \
    XDATA_TYPE *haddr = ATOMIC_MMU_LOOKUP;                          \
    XDATA_TYPE ldo, ldn, old, new, val = xval;                      \
    smp_mb();                                                       \
    ldn = atomic_read__nocheck(haddr);                              \
    do {                                                            \
        ldo = ldn; old = BSWAP(ldo); new = FN(old, val);            \
        ldn = atomic_cmpxchg__nocheck(haddr, ldo, BSWAP(new));      \
    } while (ldo != ldn);                                           \
    ATOMIC_MMU_CLEANUP;                                             \
    return RET;                                                     \
}

GEN_ATOMIC_HELPER_FN(fetch_smin, MIN, SDATA_TYPE, old)
GEN_ATOMIC_HELPER_FN(fetch_umin, MIN,  DATA_TYPE, old)
GEN_ATOMIC_HELPER_FN(fetch_smax, MAX, SDATA_TYPE, old)
GEN_ATOMIC_HELPER_FN(fetch_umax, MAX,  DATA_TYPE, old)

GEN_ATOMIC_HELPER_FN(smin_fetch, MIN, SDATA_TYPE, new)
GEN_ATOMIC_HELPER_FN(umin_fetch, MIN,  DATA_TYPE, new)
GEN_ATOMIC_HELPER_FN(smax_fetch, MAX, SDATA_TYPE, new)
GEN_ATOMIC_HELPER_FN(umax_fetch, MAX,  DATA_TYPE, new)

/* Note that for addition, we need to use a separate cmpxchg loop instead
   of bswaps for the reverse-host-endian helpers.  */
#define ADD(X, Y)   (X + Y)
GEN_ATOMIC_HELPER_FN(fetch_add, ADD, DATA_TYPE, old)
GEN_ATOMIC_HELPER_FN(add_fetch, ADD, DATA_TYPE, new)
#undef ADD

#undef GEN_ATOMIC_HELPER_FN
#endif /* DATA_SIZE >= 16 */

#undef END
#endif /* DATA_SIZE > 1 */

#undef BSWAP
#undef ABI_TYPE
#undef DATA_TYPE
#undef SDATA_TYPE
#undef SUFFIX
#undef DATA_SIZE
