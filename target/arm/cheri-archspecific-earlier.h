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

#define NUM_LAZY_CAP_REGS 34

// Register 32 will be zero, leaving 31 to be SP. Translation should covert 31
// to 32 where appropriate
#define ZERO_REG_NUM 32
// A register to indicate "no register" where it is inconvenient to have
// different prototypes
#define REG_NONE 0x77
// The register that is meant to be used for the data argument
#define CINVOKE_DATA_REGNUM 29
// A micro-architectural register to avoid side effects when decomposing
// instructions into micro-ops
#define SCRATCH_REG_NUM 33

#define CHERI_EXC_REGNUM_PCC 0xfe
#define CHERI_EXC_REGNUM_DDC 0xff

#endif // QEMU_CHERI_ARCHSPECIFIC_EARLIER_H
