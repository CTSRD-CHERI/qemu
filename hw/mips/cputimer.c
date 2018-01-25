/*
 * QEMU MIPS timer support
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "qemu/osdep.h"
#include "hw/hw.h"
#include "hw/mips/cpudevs.h"
#include "qemu/timer.h"
#include "sysemu/kvm.h"

#define CLOCK_PERIOD 10 /* 10 ns period for 100 Mhz frequency */
#define CYCLES_PER_CNT  2
#define TIMER_PERIOD    (CYCLES_PER_CNT * CLOCK_PERIOD)

#if 0
#define DEBUG_COUNT_REGISTER(fmt, ...) \
    fprintf(stderr, "%s: " fmt "\n", __func__, __VA_ARGS__)
#else
#define DEBUG_COUNT_REGISTER(fmt, ...)
#endif


/* XXX: do not use a global */
uint32_t cpu_mips_get_random (CPUMIPSState *env)
{
    static uint32_t seed = 1;
    static uint32_t prev_idx = 0;
    uint32_t idx;
    uint32_t nb_rand_tlb = env->tlb->nb_tlb - env->CP0_Wired;

    if (nb_rand_tlb == 1) {
        return env->tlb->nb_tlb - 1;
    }

    /* Don't return same value twice, so get another value */
    do {
        /* Use a simple algorithm of Linear Congruential Generator
         * from ISO/IEC 9899 standard. */
        seed = 1103515245 * seed + 12345;
        idx = (seed >> 16) % nb_rand_tlb + env->CP0_Wired;
    } while (idx == prev_idx);
    prev_idx = idx;
    return idx;
}

/* MIPS R4K timer */
static void cpu_mips_timer_update(CPUMIPSState *env)
{
    uint64_t now, next;
    uint32_t wait;

    now = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
    /*
     * XXXAR: Since (now / TIMER_PERIOD) is added to CP0_Count during the read,
     * the upstream code assumes it can just subtract it again here. However,
     * this can cause the value of wait to become negative (i.e. a large
     * unsigned number) and therefore it causes a long timer delay instead
     * of the timer triggering immediately.
     */
    wait = env->CP0_Compare - env->CP0_Count;
    if ((int32_t)wait > 0) {
        /*
         * If we have a valid waiting period (Compare > Count) we need to ensure
         * that subtracting the already elapsed time doesn't make the value wrap
         * around and cause a long delay instead.
         */
        uint32_t already_elapsed = (uint32_t)(now / TIMER_PERIOD);
        DEBUG_COUNT_REGISTER("wait is %d and %d ticks have already elapsed.",
                              wait, already_elapsed);
        wait = already_elapsed >= wait ? 0 : wait - already_elapsed;
        DEBUG_COUNT_REGISTER("Actual wait is %d", wait);
    }
    DEBUG_COUNT_REGISTER("wait=0x%x (%d), now = %llx, CP0_Count=0x%x, CP0_Compare=0x%x",
                         wait, wait, now, env->CP0_Count, env->CP0_Compare);
    next = now + (uint64_t)wait * TIMER_PERIOD;
    DEBUG_COUNT_REGISTER("Next timeout is in %gms", (next - now) / 1000000.0);
    timer_mod(env->timer, next);
}

/* Expire the timer.  */
static void cpu_mips_timer_expire(CPUMIPSState *env)
{
    DEBUG_COUNT_REGISTER("CP0_Count=0x%x, CP0_Compare=0x%x", env->CP0_Count,
                         env->CP0_Compare);
    cpu_mips_timer_update(env);
    if (env->insn_flags & ISA_MIPS32R2) {
        env->CP0_Cause |= 1 << CP0Ca_TI;
    }
    qemu_irq_raise(env->irq[(env->CP0_IntCtl >> CP0IntCtl_IPTI) & 0x7]);
}

uint32_t cpu_mips_get_count (CPUMIPSState *env)
{
    if (env->CP0_Cause & (1 << CP0Ca_DC)) {
        DEBUG_COUNT_REGISTER("CP0_Count=0x%x, CP0_Compare=0x%x",
                             env->CP0_Count, env->CP0_Compare);
        return env->CP0_Count;
    } else {
        uint64_t now;

        now = qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL);
        if (timer_pending(env->timer)
            && timer_expired(env->timer, now)) {
            /* The timer has already expired.  */
            cpu_mips_timer_expire(env);
        }
        DEBUG_COUNT_REGISTER("returning count + 0x%x: CP0_Count=0x%x,"
                             " result=0x%x, CP0_Compare=0x%x",
                             (uint32_t)(now / TIMER_PERIOD), env->CP0_Count,
                             env->CP0_Count + (uint32_t)(now / TIMER_PERIOD),
                             env->CP0_Compare);
        return env->CP0_Count + (uint32_t)(now / TIMER_PERIOD);;
    }
}

void cpu_mips_store_count (CPUMIPSState *env, uint32_t count)
{
    /*
     * This gets called from cpu_state_reset(), potentially before timer init.
     * So env->timer may be NULL, which is also the case with KVM enabled so
     * treat timer as disabled in that case.
     */
    if (env->CP0_Cause & (1 << CP0Ca_DC) || !env->timer) {
        env->CP0_Count = count;
        DEBUG_COUNT_REGISTER("reset CP0_Count value %x (%d)", env->CP0_Count,
                             env->CP0_Count);
    } else {
        /* Store new count register */
        env->CP0_Count = count -
               (uint32_t)(qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL) / TIMER_PERIOD);
        DEBUG_COUNT_REGISTER("set CP0_Count value %x (%d)", env->CP0_Count,
                             env->CP0_Count);
        /* Update timer timer */
        cpu_mips_timer_update(env);
    }
}

void cpu_mips_store_compare (CPUMIPSState *env, uint32_t value)
{
    env->CP0_Compare = value;
    DEBUG_COUNT_REGISTER("CP0_Count=0x%x, CP0_Compare=0x%x", env->CP0_Count,
                         env->CP0_Compare);
    if (!(env->CP0_Cause & (1 << CP0Ca_DC)))
        cpu_mips_timer_update(env);
    if (env->insn_flags & ISA_MIPS32R2)
        env->CP0_Cause &= ~(1 << CP0Ca_TI);
    qemu_irq_lower(env->irq[(env->CP0_IntCtl >> CP0IntCtl_IPTI) & 0x7]);
}

void cpu_mips_start_count(CPUMIPSState *env)
{
    cpu_mips_store_count(env, env->CP0_Count);
}

void cpu_mips_stop_count(CPUMIPSState *env)
{
    uint32_t add = (uint32_t)(qemu_clock_get_ns(QEMU_CLOCK_VIRTUAL) /
                              TIMER_PERIOD);
    DEBUG_COUNT_REGISTER("adding 0x%x, CP0_Count=0x%x, CP0_Compare=0x%x",
                         add, env->CP0_Count, env->CP0_Compare);
    /* Store the current value */
    env->CP0_Count += add;
    DEBUG_COUNT_REGISTER("new value CP0_Count=0x%x, CP0_Compare=0x%x",
                         env->CP0_Count, env->CP0_Compare);
}

static void mips_timer_cb (void *opaque)
{
    CPUMIPSState *env;

    env = opaque;
#if 0
    qemu_log("%s\n", __func__);
#endif

    if (env->CP0_Cause & (1 << CP0Ca_DC))
        return;

    DEBUG_COUNT_REGISTER("CP0_Count=0x%x, CP0_Compare=0x%x", env->CP0_Count,
                         env->CP0_Compare);

    /* ??? This callback should occur when the counter is exactly equal to
       the comparator value.  Offset the count by one to avoid immediately
       retriggering the callback before any virtual time has passed.  */
    env->CP0_Count = env->CP0_Compare + 1;
    cpu_mips_timer_expire(env);
    env->CP0_Count = env->CP0_Compare;
}

void cpu_mips_clock_init (MIPSCPU *cpu)
{
    CPUMIPSState *env = &cpu->env;

    /*
     * If we're in KVM mode, don't create the periodic timer, that is handled in
     * kernel.
     */
    if (!kvm_enabled()) {
        env->timer = timer_new_ns(QEMU_CLOCK_VIRTUAL, &mips_timer_cb, env);
    }
}

#ifdef TARGET_CHERI
static int64_t rtc_clock_adj = 0l;

uint64_t cpu_mips_get_rtc64 (CPUMIPSState *env)
{
    int64_t now = qemu_clock_get_ns(QEMU_CLOCK_HOST);

    return (uint64_t)(now + rtc_clock_adj);
}

void cpu_mips_set_rtc64 (CPUMIPSState *env, uint64_t time)
{

    rtc_clock_adj = time - qemu_clock_get_ns(QEMU_CLOCK_HOST);
}
#endif /* TARGET_CHERI */
