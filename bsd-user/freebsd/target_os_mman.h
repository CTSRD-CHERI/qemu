/*-
 * SPDX-License-Identifier: BSD-3-Clause
 *
 * Copyright (c) 1982, 1986, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)mman.h	8.2 (Berkeley) 1/9/95
 * $FreeBSD$
 */

/*
 * This file is a port of sys/sys/mman.h from CTSRD-CHERI/CheriBSD.
 */

#ifndef _TARGET_OS_MMAN_H_
#define _TARGET_OS_MMAN_H_

#define _PROT_ALL           (PROT_READ | PROT_WRITE | PROT_EXEC)
#define PROT_EXTRACT(prot) ((prot) & _PROT_ALL)

#define _PROT_MAX_SHIFT         16
#define PROT_MAX(prot)         ((prot) << _PROT_MAX_SHIFT)
#define PROT_MAX_EXTRACT(prot) (((prot) >> _PROT_MAX_SHIFT) & _PROT_ALL)

/*
 * CHERI specific flags and alignment constraints.
 *
 * MAP_CHERI_NOSETBOUNDS (requires MAP_FIXED) returns addr unchanged
 * after successful mapping.  CheriABI only.  Reuses MAP_RESERVED0020.
 *
 * MAP_ALIGNED_CHERI returns memory aligned appropriately for the requested
 * length or fails.  Passing an under-rounded length fails.
 *
 * MAP_ALIGNED_CHERI_SEAL returns memory aligned to allow sealing given the
 * requested length or fails.  Passing an under-rounded length fails.
 */
#define MAP_CHERI_NOSETBOUNDS  0x0020         /* Don't alter addr */
#define MAP_ALIGNED_CHERI      MAP_ALIGNED(2) /* align for CHERI data */
#define MAP_ALIGNED_CHERI_SEAL MAP_ALIGNED(3) /* align for sealing on CHERI */

#endif /* !_TARGET_OS_MMAN_H_ */
