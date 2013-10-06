/*
 *  sparc cpu related code
 *
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#include <sys/types.h>

#include "qemu/osdep.h"
#include "cpu.h"
#include "qemu.h"

#include "target_arch.h"

/* #define DEBUG_WIN */
/* WARNING: dealing with register windows _is_ complicated. More info
   can be found at http://www.sics.se/~psm/sparcstack.html */
static int get_reg_index(CPUSPARCState *env, int cwp, int index)
{
    index = (index + cwp * 16) % (16 * env->nwindows);
    /* wrap handling : if cwp is on the last window, then we use the
       registers 'after' the end */
    if (index < 8 && env->cwp == env->nwindows - 1) {
        index += 16 * env->nwindows;
    }
    return index;
}

/* save the register window 'cwp1' */
static void save_window_offset(CPUSPARCState *env, int cwp1)
{
    unsigned int i;
    abi_ulong sp_ptr;

    sp_ptr = env->regbase[get_reg_index(env, cwp1, 6)];
#if defined(DEBUG_WIN)
    printf("win_overflow: sp_ptr=0x" TARGET_ABI_FMT_lx " save_cwp=%d\n",
           sp_ptr, cwp1);
#endif
    for (i = 0; i < 16; i++) {
        /* FIXME - what to do if put_user() fails? */
        put_user_ual(env->regbase[get_reg_index(env, cwp1, 8 + i)], sp_ptr);
        sp_ptr += sizeof(abi_ulong);
    }
}

void bsd_sparc_save_window(CPUSPARCState *env)
{
    unsigned int new_wim;

    new_wim = ((env->wim >> 1) | (env->wim << (env->nwindows - 1))) &
        ((1LL << env->nwindows) - 1);
    save_window_offset(env, cpu_cwp_dec(env, env->cwp - 2));
    env->wim = new_wim;
}

void bsd_sparc_restore_window(CPUSPARCState *env)
{
    unsigned int new_wim;
    unsigned int i, cwp1;
    abi_ulong sp_ptr;

    new_wim = ((env->wim << 1) | (env->wim >> (env->nwindows - 1))) &
        ((1LL << env->nwindows) - 1);

    /* restore the invalid window */
    cwp1 = cpu_cwp_inc(env, env->cwp + 1);
    sp_ptr = env->regbase[get_reg_index(env, cwp1, 6)];
#if defined(DEBUG_WIN)
    printf("win_underflow: sp_ptr=0x" TARGET_ABI_FMT_lx " load_cwp=%d\n",
           sp_ptr, cwp1);
#endif
    for (i = 0; i < 16; i++) {
        /* FIXME - what to do if get_user() fails? */
        get_user_ual(env->regbase[get_reg_index(env, cwp1, 8 + i)], sp_ptr);
        sp_ptr += sizeof(abi_ulong);
    }
    env->wim = new_wim;
}

void bsd_sparc_flush_windows(CPUSPARCState *env)
{
    int offset, cwp1;

    offset = 1;
    for (;;) {
        /* if restore would invoke restore_window(), then we can stop */
        cwp1 = cpu_cwp_inc(env, env->cwp + offset);
        if (env->wim & (1 << cwp1)) {
            break;
        }
        save_window_offset(env, cwp1);
        offset++;
    }
    cwp1 = cpu_cwp_inc(env, env->cwp + 1);
    /* set wim so that restore will reload the registers */
    env->wim = 1 << cwp1;
#if defined(DEBUG_WIN)
    printf("bsd_sparc_flush_windows: nb=%d\n", offset - 1);
#endif
}

