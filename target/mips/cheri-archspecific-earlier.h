/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2021 Lawrence Esswood
 *
 * This work was supported by Innovate UK project 105694, "Digital Security
 * by Design (DSbD) Technology Platform Prototype".
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

#ifndef QEMU_CHERI_ARCHSPECIFIC_EARLIER_H
#define QEMU_CHERI_ARCHSPECIFIC_EARLIER_H

#define NUM_LAZY_CAP_REGS 32
#define ZERO_REG_NUM 0
// This needs a refactor. It is helpful to give special purpose registers numbers as then common code
// paths can be used for both general purpose and special purpose registers. For RISCV, the exception number
// was used for this purpose, and so arm followed suit in using CHERI_EXC_REGNUM_DDC. However, on mips,
// the actual exception number for DDC is 0. This is not helpful, as 0 is the zero register in some contexts.
// We should really have a DDC_REGNUM distinct from ZERO_REG_NUM (which is what CHERI_EXC_REGNUM_DDC currently is)
// and CHERI_EXC_REGNUM_DDC which is only used for exceptions and nothing else.
#define CHERI_EXC_REGNUM_DDC 32
#define CHERI_TRUE_EXC_REGNUM_DDC 0
#define CHERI_EXC_REGNUM_PCC 0xff
#define CHERI_REGNUM_IDC  26  /* Invoked Data Capability */
#define CINVOKE_DATA_REGNUM CHERI_REGNUM_IDC

// A micro-architectural register to avoid side effects when decomposing instructions into micro-ops
// NOTE: This needs to be defined, but if not used should just be some value not used by a real register
#define SCRATCH_REG_NUM 0xdd

#endif //QEMU_CHERI_ARCHSPECIFIC_EARLIER_H
