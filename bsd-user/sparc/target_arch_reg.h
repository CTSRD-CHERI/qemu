/*
 *  FreeBSD sparc register structures
 *
 *  Copyright (c) 2015 Stacey Son
 *  All rights reserved.
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

#ifndef _TARGET_ARCH_REG_H_
#define _TARGET_ARCH_REG_H_

typedef struct target_reg {
    uint64_t r_global[8];
    uint64_t r_out[8];
    uint64_t r_fprs;
    uint64_t r_fsr;
    uint64_t r_gsr;
    uint64_t r_level;
    uint64_t r_pil;
    uint64_t r_sfar;
    uint64_t r_sfsr;
    uint64_t r_tar;
    uint64_t r_tnpc;
    uint64_t r_tpc;
    uint64_t r_tstate;
    uint64_t r_type;
    uint64_t r_y;
    uint64_t r_wstate;
    uint64_t r_pad[2];
} target_reg_t;

typedef struct target_fpreg {
    uint32_t fr_regs[64];
    int64_t fr_fsr;
    int32_t fr_gsr;
    int32_t fr_pad[1];
} target_fpreg_t;

static inline void target_copy_regs(target_reg_t *regs,
        const CPUSPARCState *env)
{
}

#endif /* !_TARGET_ARCH_REG_H_ */
