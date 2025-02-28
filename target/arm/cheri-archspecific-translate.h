/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2025 Alex Richardson
 */

#pragma once

#ifdef TARGET_CHERI
static inline bool cctlr_set(struct DisasContext *ctx, uint32_t bits);
#define CHERI_TRANSLATE_DDC_RELOCATION(ctx) cctlr_set(ctx, CCTLR_DDCBO)
#define CHERI_TRANSLATE_PCC_RELOCATION(ctx) cctlr_set(ctx, CCTLR_PCCBO)
#endif
