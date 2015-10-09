/*
 *  mips cpu related code
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
#include "target_arch.h"

#define TP_OFFSET	0x7008

void target_cpu_set_tls(CPUMIPSState *env, target_ulong newtls)
{
    env->active_tc.CP0_UserLocal = newtls + TP_OFFSET;
}

target_ulong target_cpu_get_tls(CPUMIPSState *env)
{
    return (env->active_tc.CP0_UserLocal - TP_OFFSET);
}
