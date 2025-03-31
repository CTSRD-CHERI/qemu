/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2020 Alex Richardson
 *
 * This software was developed by SRI International and the University of
 * Cambridge Computer Laboratory (Department of Computer Science and
 * Technology) under DARPA contract HR0011-18-C-0016 ("ECATS"), as part of the
 * DARPA SSITH research programme.
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
#define _CC_TEST_ENABLE_DEPRECATED // TODO: port away from deprecated API
#include "../cheri_compressed_cap.h"

#define SAIL_WRAPPER_FORMAT_LOWER 64
#include "sail_wrapper_api.inc"
#undef SAIL_WRAPPER_FORMAT_LOWER

#define SAIL_WRAPPER_FORMAT_LOWER 64r
#include "sail_wrapper_api.inc"
#undef SAIL_WRAPPER_FORMAT_LOWER

#define SAIL_WRAPPER_FORMAT_LOWER 128
#include "sail_wrapper_api.inc"
#undef SAIL_WRAPPER_FORMAT_LOWER

#define SAIL_WRAPPER_FORMAT_LOWER 128m
#include "sail_wrapper_api.inc"
#undef SAIL_WRAPPER_FORMAT_LOWER

#define SAIL_WRAPPER_FORMAT_LOWER 128r
#include "sail_wrapper_api.inc"
#undef SAIL_WRAPPER_FORMAT_LOWER

#define _cc_sail(name) _CC_CONCAT(sail_, _CC_CONCAT(name, _CC_CONCAT(_, CC_FORMAT_LOWER)))
#ifdef __cplusplus
#define TestAPICC _CC_CONCAT(TestAPI, CC_FORMAT_LOWER)
#endif
