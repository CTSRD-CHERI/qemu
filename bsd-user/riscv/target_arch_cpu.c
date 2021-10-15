/*
 *  RISC-V CPU related code
 *
 *  Copyright (c) 2019 Mark Corbin <mark.corbin@embecosm.com>
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

#define TP_OFFSET	16

/* Compare with cpu_set_user_tls() in riscv/riscv/vm_machdep.c */
void target_cpu_set_tls(CPURISCVState *env, target_ulong newtls)
{
    gpr_set_int_value(env, xTP, newtls + TP_OFFSET);
}
