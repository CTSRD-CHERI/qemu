/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2025 Alex Richardson
 */

#pragma once

#define CHERI_TRANSLATE_DDC_RELOCATION(ctx) (!(ctx)->cheri_v9_semantics)
#define CHERI_TRANSLATE_PCC_RELOCATION(ctx) (!(ctx)->cheri_v9_semantics)
