/*-
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2020 Lawrence Esswood
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

#ifndef QEMU_ARMTRICKBOX_H
#define QEMU_ARMTRICKBOX_H

/* This is not an official implementation of any arm technology. */

#define ARM_TRICKBOX_ADDRESS 0x13000000
#define ARM_TRICKBOX_SIZE    0x200000

/* Three memory mapped regions */
#define ARM_TRICKBOX_ACK_START      ARM_TRICKBOX_ADDRESS
#define ARM_TRICKBOX_ACK_END        (ARM_TRICKBOX_ADDRESS + 0x2000)
#define ARM_TRICKBOX_PARTNER_START  (ARM_TRICKBOX_ADDRESS + 0x2000)
#define ARM_TRICKBOX_PARTNER_END    (ARM_TRICKBOX_ADDRESS + 0x3000)
#define ARM_TRICKBOX_RESERVED_START (ARM_TRICKBOX_ADDRESS + 0x3000)
#define ARM_TRICKBOX_RESERVED_END   (ARM_TRICKBOX_ADDRESS + 0x200000)

/* Registers */
#define ARM_TRICKBOX_REGISTER_Tube                         0x0000
#define ARM_TRICKBOX_REGISTER_Configuration                0x0004
#define ARM_TRICKBOX_REGISTER_ScheduleFiq                  0x0008
#define ARM_TRICKBOX_REGISTER_ScheduleIRQ                  0x000C
#define ARM_TRICKBOX_REGISTER_ClearFIQ                     0x0010
#define ARM_TRICKBOX_REGISTER_ClearIRQ                     0x0014
#define ARM_TRICKBOX_REGISTER_ScheduleRST                  0x0018
#define ARM_TRICKBOX_REGISTER_Extended_Configuration       0x0028
#define ARM_TRICKBOX_REGISTER_abortrgnlo2                  0x0038
#define ARM_TRICKBOX_REGISTER_abortrgnhi2                  0x003C
#define ARM_TRICKBOX_REGISTER_abortrgnlo1                  0x0040
#define ARM_TRICKBOX_REGISTER_abortrgnhi1                  0x0044
#define ARM_TRICKBOX_REGISTER_DBGControl                   0x0054
#define ARM_TRICKBOX_REGISTER_DBG_PE_Mailbox               0x0068
#define ARM_TRICKBOX_REGISTER_DBG_PE_END                   0x0070
#define ARM_TRICKBOX_REGISTER_DormantControl               0x00A4
#define ARM_TRICKBOX_REGISTER_CORE_GENERATED_IRQ_PIN_VALUE 0x00B0
#define ARM_TRICKBOX_REGISTER_Extended_TargetCpu           0x00F0
#define ARM_TRICKBOX_REGISTER_TargetCPU                    0x00FC
#define ARM_TRICKBOX_REGISTER_AbortRgn64Lo1                0x0100
#define ARM_TRICKBOX_REGISTER_AbortRgn64Lo1_Hi             0x0104
#define ARM_TRICKBOX_REGISTER_AbortRgn64Hi1                0x0108
#define ARM_TRICKBOX_REGISTER_AbortRgn64Hi1_Hi             0x010C
#define ARM_TRICKBOX_REGISTER_AbortRgn64Lo2                0x0110
#define ARM_TRICKBOX_REGISTER_AbortRgn64Lo2_Hi             0x0114
#define ARM_TRICKBOX_REGISTER_AbortRgn64Hi2                0x0118
#define ARM_TRICKBOX_REGISTER_AbortRgn64Hi2_Hi             0x011C
#define ARM_TRICKBOX_REGISTER_PPUNAddr(N)                  (0x0230 + (8 * N))
#define ARM_TRICKBOX_REGISTER_PPUNMask(N)                  (0x0234 + (8 * N))
#define ARM_TRICKBOX_REGISTER_WatchDog                     0x0284
#define ARM_TRICKBOX_REGISTER_AXIAbortCtl                  0x0500
#define ARM_TRICKBOX_REGISTER_SWResetCtl                   0x0520
#define ARM_TRICKBOX_REGISTER_GTE_API                      0x0A00
#define ARM_TRICKBOX_REGISTER_GTE_API_PARAM                0x0A04
#define ARM_TRICKBOX_REGISTER_GTE_API_STATUS               0x0A08
#define ARM_TRICKBOX_REGISTER_GTE_API_STATUS_64            0x0A20
#define ARM_TRICKBOX_REGISTER_GTE_API_STATUS_64_HI         0x0A24
#define ARM_TRICKBOX_REGISTER_GTE_API_PARAM_64             0x0A28
#define ARM_TRICKBOX_REGISTER_GTE_API_PARAM_64_HI          0x0A2C
#define ARM_TRICKBOX_REGISTER_SPI_PIN_INDEX                0x0C00
#define ARM_TRICKBOX_REGISTER_SPI_PIN_VALUE                0x0C04
#define ARM_TRICKBOX_REGISTER_PPI_TARGET_CORE              0x0C20
#define ARM_TRICKBOX_REGISTER_PPI_INTR_VALUE               0x0C24
#define ARM_TRICKBOX_REGISTER_REI_TARGET_CORE              0x0C40
#define ARM_TRICKBOX_REGISTER_PPI_INTR_CONFIG              0x0C2C
#define ARM_TRICKBOX_REGISTER_REI_VALUE                    0x0C44
#define ARM_TRICKBOX_REGISTER_SEI_TARGET_CORE              0x0C60
#define ARM_TRICKBOX_REGISTER_SEI_VALUE                    0x0C64
#define ARM_TRICKBOX_REGISTER_SPI_PPI_SEI_REI_CONTROL      0x0C80
#define ARM_TRICKBOX_REGISTER_GICRdEventStatusIndex        0x0CA0
#define ARM_TRICKBOX_REGISTER_GICRdEventStatusValue        0x0CA4
#define ARM_TRICKBOX_REGISTER_DeviceID                     0x0D00
#define ARM_TRICKBOX_REGISTER_Attributes                   0x0D10
#define ARM_TRICKBOX_REGISTER_Address                      0x0D20
#define ARM_TRICKBOX_REGISTER_Data                         0x0D40
#define ARM_TRICKBOX_REGISTER_Operate                      0x0D80
#define ARM_TRICKBOX_REGISTER_TEST_TO_BFM_CTRL_REG         0x0D88
#define ARM_TRICKBOX_REGISTER_BFM_TO_TEST_CTRL_REGs        0x0D90
#define ARM_TRICKBOX_REGISTER_EO_Return_Mailbox            0xE000
#define ARM_TRICKBOX_REGISTER_EO_Return_Mailbox_END        0xE010
#define ARM_TRICKBOX_REGISTER_Scratch_Pad                  0x1080
#define ARM_TRICKBOX_REGISTER_Scratch_Pad_END              0x10FC
#define ARM_TRICKBOX_REGISTER_ScheduleSError               0x1100
#define ARM_TRICKBOX_REGISTER_ClearSError                  0x1104
#define ARM_TRICKBOX_REGISTER_SYSTEM_PIN_VALUE             0x1110

/* Constants and fields  */

#define ARM_TRICKBOX_TUBE_EXIT '\004'

#define ARM_TRICKBOX_Configuration_HIVECS           (1 << 0)
#define ARM_TRICKBOX_Configuration_RW               (1 << 1)
#define ARM_TRICKBOX_Configuration_CFGEND           (1 << 2)
#define ARM_TRICKBOX_Configuration_CONFIG64         (1 << 14)
#define ARM_TRICKBOX_Configuration_disableSPIDebug  (1 << 18)
#define ARM_TRICKBOX_Configuration_disableSPNIDebug (1 << 19)
#define ARM_TRICKBOX_Configuration_Cp15SDisable     (1 << 20)
#define ARM_TRICKBOX_Configuration_ExceptInit       (1 << 22)
#define ARM_TRICKBOX_Configuration_CfgNMFI          (1 << 23)
#define ARM_TRICKBOX_Configuration_disableNIDebug   (1 << 24)
#define ARM_TRICKBOX_Configuration_Cp15SDisable2    (1 << 25)

#define ARM_TRICKBOX_AXIAbortCtl_Rgn1_Rd_Disable (1 << 20)
#define ARM_TRICKBOX_AXIAbortCtl_Rgn1_Wr_Disable (1 << 21)
#define ARM_TRICKBOX_AXIAbortCtl_Rgn2_Rd_Disable (1 << 24)
#define ARM_TRICKBOX_AXIAbortCtl_Rgn2_Wr_Disable (1 << 25)

SysBusDevice *arm_trickbox_mm_init_default(void);

#endif /* QEMU_ARMTRICKBOX_H */
