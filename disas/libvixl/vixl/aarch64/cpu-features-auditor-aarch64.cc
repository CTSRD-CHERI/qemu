// Copyright 2018, VIXL authors
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
//   * Redistributions of source code must retain the above copyright notice,
//     this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright notice,
//     this list of conditions and the following disclaimer in the documentation
//     and/or other materials provided with the distribution.
//   * Neither the name of Arm Limited nor the names of its contributors may be
//     used to endorse or promote products derived from this software without
//     specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS CONTRIBUTORS "AS IS" AND
// ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "vixl/cpu-features.h"
#include "vixl/globals-vixl.h"
#include "vixl/utils-vixl.h"
#include "decoder-aarch64.h"

#include "cpu-features-auditor-aarch64.h"

namespace vixl {
namespace aarch64 {

// Every instruction must update last_instruction_, even if only to clear it,
// and every instruction must also update seen_ once it has been fully handled.
// This scope makes that simple, and allows early returns in the decode logic.
class CPUFeaturesAuditor::RecordInstructionFeaturesScope {
 public:
  explicit RecordInstructionFeaturesScope(CPUFeaturesAuditor* auditor)
      : auditor_(auditor) {
    auditor_->last_instruction_ = CPUFeatures::None();
    if (auditor->GetISA() == ISA::C64) {
      Record(CPUFeatures::kMorello);
    }
  }
  ~RecordInstructionFeaturesScope() {
    auditor_->seen_.Combine(auditor_->last_instruction_);
  }

  void Record(const CPUFeatures& features) {
    auditor_->last_instruction_.Combine(features);
  }

  void Record(CPUFeatures::Feature feature0,
              CPUFeatures::Feature feature1 = CPUFeatures::kNone,
              CPUFeatures::Feature feature2 = CPUFeatures::kNone,
              CPUFeatures::Feature feature3 = CPUFeatures::kNone) {
    auditor_->last_instruction_.Combine(feature0, feature1, feature2, feature3);
  }

  // If exactly one of a or b is known to be available, record it. Otherwise,
  // record both. This is intended for encodings that can be provided by two
  // different features.
  void RecordOneOrBothOf(CPUFeatures::Feature a, CPUFeatures::Feature b) {
    bool hint_a = auditor_->available_.Has(a);
    bool hint_b = auditor_->available_.Has(b);
    if (hint_a && !hint_b) {
      Record(a);
    } else if (hint_b && !hint_a) {
      Record(b);
    } else {
      Record(a, b);
    }
  }

 private:
  CPUFeaturesAuditor* auditor_;
};

void CPUFeaturesAuditor::LoadStoreHelper(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  switch (instr->Mask(LoadStoreMask)) {
    case LDR_b:
    case LDR_q:
    case STR_b:
    case STR_q:
      scope.Record(CPUFeatures::kNEON);
      return;
    case LDR_h:
    case LDR_s:
    case LDR_d:
    case STR_h:
    case STR_s:
    case STR_d:
      scope.RecordOneOrBothOf(CPUFeatures::kFP, CPUFeatures::kNEON);
      return;
    default:
      // No special CPU features.
      return;
  }
}

void CPUFeaturesAuditor::LoadStorePairHelper(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  switch (instr->Mask(LoadStorePairMask)) {
    case LDP_q:
    case STP_q:
      scope.Record(CPUFeatures::kNEON);
      return;
    case LDP_s:
    case LDP_d:
    case STP_s:
    case STP_d: {
      scope.RecordOneOrBothOf(CPUFeatures::kFP, CPUFeatures::kNEON);
      return;
    }
    default:
      // No special CPU features.
      return;
  }
}

void CPUFeaturesAuditor::VisitAddSubExtended(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  USE(instr);
}

void CPUFeaturesAuditor::VisitAddSubImmediate(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  USE(instr);
}

void CPUFeaturesAuditor::VisitAddSubShifted(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  USE(instr);
}

void CPUFeaturesAuditor::VisitAddSubWithCarry(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  USE(instr);
}

void CPUFeaturesAuditor::VisitRotateRightIntoFlags(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  switch (instr->Mask(RotateRightIntoFlagsMask)) {
    case RMIF:
      scope.Record(CPUFeatures::kFlagM);
      return;
  }
}

void CPUFeaturesAuditor::VisitEvaluateIntoFlags(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  switch (instr->Mask(EvaluateIntoFlagsMask)) {
    case SETF8:
    case SETF16:
      scope.Record(CPUFeatures::kFlagM);
      return;
  }
}

void CPUFeaturesAuditor::VisitAtomicMemory(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  switch (instr->Mask(AtomicMemoryMask)) {
    case LDAPRB:
    case LDAPRH:
    case LDAPR_w:
    case LDAPR_x:
      scope.Record(CPUFeatures::kRCpc);
      return;
    default:
      // Everything else belongs to the Atomics extension.
      scope.Record(CPUFeatures::kAtomics);
      return;
  }
}

void CPUFeaturesAuditor::VisitBitfield(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  USE(instr);
}

void CPUFeaturesAuditor::VisitCompareBranch(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  USE(instr);
}

void CPUFeaturesAuditor::VisitConditionalBranch(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  USE(instr);
}

void CPUFeaturesAuditor::VisitConditionalCompareImmediate(
    const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  USE(instr);
}

void CPUFeaturesAuditor::VisitConditionalCompareRegister(
    const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  USE(instr);
}

void CPUFeaturesAuditor::VisitConditionalSelect(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  USE(instr);
}

void CPUFeaturesAuditor::VisitCrypto2RegSHA(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  USE(instr);
}

void CPUFeaturesAuditor::VisitCrypto3RegSHA(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  USE(instr);
}

void CPUFeaturesAuditor::VisitCryptoAES(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  USE(instr);
}

void CPUFeaturesAuditor::VisitDataProcessing1Source(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  switch (instr->Mask(DataProcessing1SourceMask)) {
    case PACIA:
    case PACIB:
    case PACDA:
    case PACDB:
    case AUTIA:
    case AUTIB:
    case AUTDA:
    case AUTDB:
    case PACIZA:
    case PACIZB:
    case PACDZA:
    case PACDZB:
    case AUTIZA:
    case AUTIZB:
    case AUTDZA:
    case AUTDZB:
    case XPACI:
    case XPACD:
      scope.Record(CPUFeatures::kPAuth);
      return;
    default:
      // No special CPU features.
      return;
  }
}

void CPUFeaturesAuditor::VisitDataProcessing2Source(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  switch (instr->Mask(DataProcessing2SourceMask)) {
    case CRC32B:
    case CRC32H:
    case CRC32W:
    case CRC32X:
    case CRC32CB:
    case CRC32CH:
    case CRC32CW:
    case CRC32CX:
      scope.Record(CPUFeatures::kCRC32);
      return;
    case PACGA:
      scope.Record(CPUFeatures::kPAuth, CPUFeatures::kPAuthGeneric);
      return;
    default:
      // No special CPU features.
      return;
  }
}

void CPUFeaturesAuditor::VisitLoadStoreRCpcUnscaledOffset(
    const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  switch (instr->Mask(LoadStoreRCpcUnscaledOffsetMask)) {
    case LDAPURB:
    case LDAPURSB_w:
    case LDAPURSB_x:
    case LDAPURH:
    case LDAPURSH_w:
    case LDAPURSH_x:
    case LDAPUR_w:
    case LDAPURSW:
    case LDAPUR_x:

    // These stores don't actually have RCpc semantics but they're included with
    // the RCpc extensions.
    case STLURB:
    case STLURH:
    case STLUR_w:
    case STLUR_x:
      scope.Record(CPUFeatures::kRCpc, CPUFeatures::kRCpcImm);
      return;
  }
}

void CPUFeaturesAuditor::VisitLoadStorePAC(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  USE(instr);
  scope.Record(CPUFeatures::kPAuth);
}

void CPUFeaturesAuditor::VisitDataProcessing3Source(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  USE(instr);
}

void CPUFeaturesAuditor::VisitException(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  USE(instr);
}

void CPUFeaturesAuditor::VisitExtract(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  USE(instr);
}

void CPUFeaturesAuditor::VisitFPCompare(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  // All of these instructions require FP.
  scope.Record(CPUFeatures::kFP);
  switch (instr->Mask(FPCompareMask)) {
    case FCMP_h:
    case FCMP_h_zero:
    case FCMPE_h:
    case FCMPE_h_zero:
      scope.Record(CPUFeatures::kFPHalf);
      return;
    default:
      // No special CPU features.
      return;
  }
}

void CPUFeaturesAuditor::VisitFPConditionalCompare(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  // All of these instructions require FP.
  scope.Record(CPUFeatures::kFP);
  switch (instr->Mask(FPConditionalCompareMask)) {
    case FCCMP_h:
    case FCCMPE_h:
      scope.Record(CPUFeatures::kFPHalf);
      return;
    default:
      // No special CPU features.
      return;
  }
}

void CPUFeaturesAuditor::VisitFPConditionalSelect(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  // All of these instructions require FP.
  scope.Record(CPUFeatures::kFP);
  if (instr->Mask(FPConditionalSelectMask) == FCSEL_h) {
    scope.Record(CPUFeatures::kFPHalf);
  }
}

void CPUFeaturesAuditor::VisitFPDataProcessing1Source(
    const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  // All of these instructions require FP.
  scope.Record(CPUFeatures::kFP);
  switch (instr->Mask(FPDataProcessing1SourceMask)) {
    case FMOV_h:
    case FABS_h:
    case FNEG_h:
    case FSQRT_h:
    case FRINTN_h:
    case FRINTP_h:
    case FRINTM_h:
    case FRINTZ_h:
    case FRINTA_h:
    case FRINTX_h:
    case FRINTI_h:
      scope.Record(CPUFeatures::kFPHalf);
      return;
    case FRINT32X_s:
    case FRINT32X_d:
    case FRINT32Z_s:
    case FRINT32Z_d:
    case FRINT64X_s:
    case FRINT64X_d:
    case FRINT64Z_s:
    case FRINT64Z_d:
      scope.Record(CPUFeatures::kFrintToFixedSizedInt);
      return;
    default:
      // No special CPU features.
      // This category includes some half-precision FCVT instructions that do
      // not require FPHalf.
      return;
  }
}

void CPUFeaturesAuditor::VisitFPDataProcessing2Source(
    const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  // All of these instructions require FP.
  scope.Record(CPUFeatures::kFP);
  switch (instr->Mask(FPDataProcessing2SourceMask)) {
    case FMUL_h:
    case FDIV_h:
    case FADD_h:
    case FSUB_h:
    case FMAX_h:
    case FMIN_h:
    case FMAXNM_h:
    case FMINNM_h:
    case FNMUL_h:
      scope.Record(CPUFeatures::kFPHalf);
      return;
    default:
      // No special CPU features.
      return;
  }
}

void CPUFeaturesAuditor::VisitFPDataProcessing3Source(
    const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  // All of these instructions require FP.
  scope.Record(CPUFeatures::kFP);
  switch (instr->Mask(FPDataProcessing3SourceMask)) {
    case FMADD_h:
    case FMSUB_h:
    case FNMADD_h:
    case FNMSUB_h:
      scope.Record(CPUFeatures::kFPHalf);
      return;
    default:
      // No special CPU features.
      return;
  }
}

void CPUFeaturesAuditor::VisitFPFixedPointConvert(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  // All of these instructions require FP.
  scope.Record(CPUFeatures::kFP);
  switch (instr->Mask(FPFixedPointConvertMask)) {
    case FCVTZS_wh_fixed:
    case FCVTZS_xh_fixed:
    case FCVTZU_wh_fixed:
    case FCVTZU_xh_fixed:
    case SCVTF_hw_fixed:
    case SCVTF_hx_fixed:
    case UCVTF_hw_fixed:
    case UCVTF_hx_fixed:
      scope.Record(CPUFeatures::kFPHalf);
      return;
    default:
      // No special CPU features.
      return;
  }
}

void CPUFeaturesAuditor::VisitFPImmediate(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  // All of these instructions require FP.
  scope.Record(CPUFeatures::kFP);
  if (instr->Mask(FPImmediateMask) == FMOV_h_imm) {
    scope.Record(CPUFeatures::kFPHalf);
  }
}

void CPUFeaturesAuditor::VisitFPIntegerConvert(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  // All of these instructions require FP.
  scope.Record(CPUFeatures::kFP);
  switch (instr->Mask(FPIntegerConvertMask)) {
    case FCVTAS_wh:
    case FCVTAS_xh:
    case FCVTAU_wh:
    case FCVTAU_xh:
    case FCVTMS_wh:
    case FCVTMS_xh:
    case FCVTMU_wh:
    case FCVTMU_xh:
    case FCVTNS_wh:
    case FCVTNS_xh:
    case FCVTNU_wh:
    case FCVTNU_xh:
    case FCVTPS_wh:
    case FCVTPS_xh:
    case FCVTPU_wh:
    case FCVTPU_xh:
    case FCVTZS_wh:
    case FCVTZS_xh:
    case FCVTZU_wh:
    case FCVTZU_xh:
    case FMOV_hw:
    case FMOV_hx:
    case FMOV_wh:
    case FMOV_xh:
    case SCVTF_hw:
    case SCVTF_hx:
    case UCVTF_hw:
    case UCVTF_hx:
      scope.Record(CPUFeatures::kFPHalf);
      return;
    case FMOV_d1_x:
    case FMOV_x_d1:
      scope.Record(CPUFeatures::kNEON);
      return;
    case FJCVTZS:
      scope.Record(CPUFeatures::kJSCVT);
      return;
    default:
      // No special CPU features.
      return;
  }
}

void CPUFeaturesAuditor::VisitLoadLiteral(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  switch (instr->Mask(LoadLiteralMask)) {
    case LDR_s_lit:
    case LDR_d_lit:
      scope.RecordOneOrBothOf(CPUFeatures::kFP, CPUFeatures::kNEON);
      return;
    case LDR_q_lit:
      scope.Record(CPUFeatures::kNEON);
      return;
    default:
      // No special CPU features.
      return;
  }
}

void CPUFeaturesAuditor::VisitLoadStoreExclusive(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  switch (instr->Mask(LoadStoreExclusiveMask)) {
    case CAS_w:
    case CASA_w:
    case CASL_w:
    case CASAL_w:
    case CAS_x:
    case CASA_x:
    case CASL_x:
    case CASAL_x:
    case CASB:
    case CASAB:
    case CASLB:
    case CASALB:
    case CASH:
    case CASAH:
    case CASLH:
    case CASALH:
    case CASP_w:
    case CASPA_w:
    case CASPL_w:
    case CASPAL_w:
    case CASP_x:
    case CASPA_x:
    case CASPL_x:
    case CASPAL_x:
      scope.Record(CPUFeatures::kAtomics);
      return;
    case STLLRB:
    case LDLARB:
    case STLLRH:
    case LDLARH:
    case STLLR_w:
    case LDLAR_w:
    case STLLR_x:
    case LDLAR_x:
      scope.Record(CPUFeatures::kLORegions);
      return;
    default:
      // No special CPU features.
      return;
  }
}

void CPUFeaturesAuditor::VisitLoadStorePairNonTemporal(
    const Instruction* instr) {
  LoadStorePairHelper(instr);
}

void CPUFeaturesAuditor::VisitLoadStorePairOffset(const Instruction* instr) {
  LoadStorePairHelper(instr);
}

void CPUFeaturesAuditor::VisitLoadStorePairPostIndex(const Instruction* instr) {
  LoadStorePairHelper(instr);
}

void CPUFeaturesAuditor::VisitLoadStorePairPreIndex(const Instruction* instr) {
  LoadStorePairHelper(instr);
}

void CPUFeaturesAuditor::VisitLoadStorePostIndex(const Instruction* instr) {
  LoadStoreHelper(instr);
}

void CPUFeaturesAuditor::VisitLoadStorePreIndex(const Instruction* instr) {
  LoadStoreHelper(instr);
}

void CPUFeaturesAuditor::VisitLoadStoreRegisterOffset(
    const Instruction* instr) {
  LoadStoreHelper(instr);
}

void CPUFeaturesAuditor::VisitLoadStoreUnscaledOffset(
    const Instruction* instr) {
  LoadStoreHelper(instr);
}

void CPUFeaturesAuditor::VisitLoadStoreUnsignedOffset(
    const Instruction* instr) {
  LoadStoreHelper(instr);
}

void CPUFeaturesAuditor::VisitLogicalImmediate(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  USE(instr);
}

void CPUFeaturesAuditor::VisitLogicalShifted(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  USE(instr);
}

// Most Morello visitors require only Morello.
#define VIXL_SIMPLE_MORELLO_VISITOR_LIST(V)   \
  V(Morello1Src1Dst)                          \
  V(Morello2SrcCap)                           \
  V(MorelloADD)                               \
  V(MorelloAddSubCap)                         \
  V(MorelloAlignment)                         \
  V(MorelloBitwise)                           \
  V(MorelloBranch)                            \
  V(MorelloBranchBx)                          \
  V(MorelloBranchRestricted)                  \
  V(MorelloBranchSealedDirect)                \
  V(MorelloBranchSealedIndirect)              \
  V(MorelloBranchToSealed)                    \
  V(MorelloChecks)                            \
  V(MorelloCLRPERMImm)                        \
  V(MorelloConvertToCap)                      \
  V(MorelloConvertToCapWithImplicitOperand)   \
  V(MorelloConvertToPointer)                  \
  V(MorelloCSEL)                              \
  V(MorelloCVT)                               \
  V(MorelloGetField1)                         \
  V(MorelloGetField2)                         \
  V(MorelloGetSetSystemRegister)              \
  V(MorelloImmBounds)                         \
  V(MorelloLDR)                               \
  V(MorelloLoadPairAndBranch)                 \
  V(MorelloLoadStoreAcquireRelease)           \
  V(MorelloLoadExclusive)                     \
  V(MorelloLoadPairExclusive)                 \
  V(MorelloLoadStoreAcquireReleaseAltBase)    \
  V(MorelloLoadStoreAcquireReleaseCapAltBase) \
  V(MorelloLoadStoreCapAltBase)               \
  V(MorelloLoadStoreImmediatePostIndex)       \
  V(MorelloLoadStoreImmediatePreIndex)        \
  V(MorelloLoadStorePair)                     \
  V(MorelloLoadStorePairNonTemporal)          \
  V(MorelloLoadStorePairPostIndex)            \
  V(MorelloLoadStorePairPreIndex)             \
  V(MorelloLoadStoreRegister)                 \
  V(MorelloLoadStoreTags)                     \
  V(MorelloLoadStoreUnscaledImmediate)        \
  V(MorelloLoadStoreUnsignedOffset)           \
  V(MorelloLoadStoreUnsignedOffsetAltBase)    \
  V(MorelloLogicalImm)                        \
  V(MorelloMiscCap0)                          \
  V(MorelloMiscCap1)                          \
  V(MorelloMiscCap2)                          \
  V(MorelloSCFLGS)                            \
  V(MorelloSEAL)                              \
  V(MorelloSUBS)                              \
  V(MorelloSetField1)                         \
  V(MorelloSetField2)                         \
  V(MorelloStoreExclusive)                    \
  V(MorelloStorePairExclusive)

#define VIXL_DEFINE_SIMPLE_MORELLO_VISITOR(NAME)                   \
  void CPUFeaturesAuditor::Visit##NAME(const Instruction* instr) { \
    RecordInstructionFeaturesScope scope(this);                    \
    scope.Record(CPUFeatures::kMorello);                           \
    USE(instr);                                                    \
  }
VIXL_SIMPLE_MORELLO_VISITOR_LIST(VIXL_DEFINE_SIMPLE_MORELLO_VISITOR)
#undef VIXL_DEFINE_SIMPLE_MORELLO_VISITOR
#undef VIXL_SIMPLE_MORELLO_VISITOR_LIST

void CPUFeaturesAuditor::VisitMorelloCompareAndSwap(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  scope.Record(CPUFeatures::kMorello);
  switch (instr->Mask(MorelloCompareAndSwapMask)) {
    case CAS_c_r:
    case CASL_c_r:
    case CASA_c_r:
    case CASAL_c_r:
      scope.Record(CPUFeatures::kAtomics);
      return;
    default:
      // No additional features.
      return;
  }
}

void CPUFeaturesAuditor::VisitMorelloLDAPR(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  scope.Record(CPUFeatures::kMorello);
  switch (instr->Mask(MorelloLDAPRMask)) {
    case LDAPR_c_r:
      scope.Record(CPUFeatures::kRCpc);
      return;
    default:
      // No additional features.
      return;
  }
}

void CPUFeaturesAuditor::VisitMorelloLoadStoreRegisterAltBase(
    const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  scope.Record(CPUFeatures::kMorello);
  switch (instr->Mask(MorelloLoadStoreRegisterAltBaseMask)) {
    case ASTR_v_rrb_d:
    case ASTR_v_rrb_s:
    case ALDR_v_rrb_d:
    case ALDR_v_rrb_s:
      scope.RecordOneOrBothOf(CPUFeatures::kFP, CPUFeatures::kNEON);
      return;
    default:
      // No additional features.
      return;
  }
}

void CPUFeaturesAuditor::VisitMorelloLoadStoreUnscaledImmediateAltBase(
    const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  scope.Record(CPUFeatures::kMorello);
  switch (instr->Mask(MorelloLoadStoreUnscaledImmediateAltBaseMask)) {
    case ASTUR_v_ri_b:
    case ASTUR_v_ri_q:
    case ALDUR_v_ri_b:
    case ALDUR_v_ri_q:
      scope.Record(CPUFeatures::kNEON);
      return;
    case ASTUR_v_ri_h:
    case ASTUR_v_ri_s:
    case ASTUR_v_ri_d:
    case ALDUR_v_ri_h:
    case ALDUR_v_ri_s:
    case ALDUR_v_ri_d:
      scope.RecordOneOrBothOf(CPUFeatures::kFP, CPUFeatures::kNEON);
      return;
    default:
      // No additional features.
      return;
  }
}

void CPUFeaturesAuditor::VisitMorelloSwap(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  scope.Record(CPUFeatures::kMorello);
  switch (instr->Mask(MorelloSwapMask)) {
    case SWP_cc_r:
    case SWPL_cc_r:
    case SWPA_cc_r:
    case SWPAL_cc_r:
      scope.Record(CPUFeatures::kAtomics);
      return;
    default:
      // No additional features.
      return;
  }
}

void CPUFeaturesAuditor::VisitMoveWideImmediate(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  USE(instr);
}

void CPUFeaturesAuditor::VisitNEON2RegMisc(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  // All of these instructions require NEON.
  scope.Record(CPUFeatures::kNEON);
  switch (instr->Mask(NEON2RegMiscFPMask)) {
    case NEON_FABS:
    case NEON_FNEG:
    case NEON_FSQRT:
    case NEON_FCVTL:
    case NEON_FCVTN:
    case NEON_FCVTXN:
    case NEON_FRINTI:
    case NEON_FRINTX:
    case NEON_FRINTA:
    case NEON_FRINTM:
    case NEON_FRINTN:
    case NEON_FRINTP:
    case NEON_FRINTZ:
    case NEON_FCVTNS:
    case NEON_FCVTNU:
    case NEON_FCVTPS:
    case NEON_FCVTPU:
    case NEON_FCVTMS:
    case NEON_FCVTMU:
    case NEON_FCVTZS:
    case NEON_FCVTZU:
    case NEON_FCVTAS:
    case NEON_FCVTAU:
    case NEON_SCVTF:
    case NEON_UCVTF:
    case NEON_FRSQRTE:
    case NEON_FRECPE:
    case NEON_FCMGT_zero:
    case NEON_FCMGE_zero:
    case NEON_FCMEQ_zero:
    case NEON_FCMLE_zero:
    case NEON_FCMLT_zero:
      scope.Record(CPUFeatures::kFP);
      return;
    case NEON_FRINT32X:
    case NEON_FRINT32Z:
    case NEON_FRINT64X:
    case NEON_FRINT64Z:
      scope.Record(CPUFeatures::kFP, CPUFeatures::kFrintToFixedSizedInt);
      return;
    default:
      // No additional features.
      return;
  }
}

void CPUFeaturesAuditor::VisitNEON2RegMiscFP16(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  // All of these instructions require NEONHalf.
  scope.Record(CPUFeatures::kFP, CPUFeatures::kNEON, CPUFeatures::kNEONHalf);
  USE(instr);
}

void CPUFeaturesAuditor::VisitNEON3Different(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  // All of these instructions require NEON.
  scope.Record(CPUFeatures::kNEON);
  USE(instr);
}

void CPUFeaturesAuditor::VisitNEON3Same(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  // All of these instructions require NEON.
  scope.Record(CPUFeatures::kNEON);
  if (instr->Mask(NEON3SameFPFMask) == NEON3SameFPFixed) {
    scope.Record(CPUFeatures::kFP);
  }
  switch (instr->Mask(NEON3SameFHMMask)) {
    case NEON_FMLAL:
    case NEON_FMLAL2:
    case NEON_FMLSL:
    case NEON_FMLSL2:
      scope.Record(CPUFeatures::kFP, CPUFeatures::kNEONHalf, CPUFeatures::kFHM);
      return;
    default:
      // No additional features.
      return;
  }
}

void CPUFeaturesAuditor::VisitNEON3SameExtra(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  // All of these instructions require NEON.
  scope.Record(CPUFeatures::kNEON);
  if ((instr->Mask(NEON3SameExtraFCMLAMask) == NEON_FCMLA) ||
      (instr->Mask(NEON3SameExtraFCADDMask) == NEON_FCADD)) {
    scope.Record(CPUFeatures::kFP, CPUFeatures::kFcma);
    if (instr->GetNEONSize() == 1) scope.Record(CPUFeatures::kNEONHalf);
  } else {
    switch (instr->Mask(NEON3SameExtraMask)) {
      case NEON_SDOT:
      case NEON_UDOT:
        scope.Record(CPUFeatures::kDotProduct);
        return;
      case NEON_SQRDMLAH:
      case NEON_SQRDMLSH:
        scope.Record(CPUFeatures::kRDM);
        return;
      default:
        // No additional features.
        return;
    }
  }
}

void CPUFeaturesAuditor::VisitNEON3SameFP16(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  // All of these instructions require NEON FP16 support.
  scope.Record(CPUFeatures::kFP, CPUFeatures::kNEON, CPUFeatures::kNEONHalf);
  USE(instr);
}

void CPUFeaturesAuditor::VisitNEONAcrossLanes(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  // All of these instructions require NEON.
  scope.Record(CPUFeatures::kNEON);
  if (instr->Mask(NEONAcrossLanesFP16FMask) == NEONAcrossLanesFP16Fixed) {
    // FMAXV_H, FMINV_H, FMAXNMV_H, FMINNMV_H
    scope.Record(CPUFeatures::kFP, CPUFeatures::kNEONHalf);
  } else if (instr->Mask(NEONAcrossLanesFPFMask) == NEONAcrossLanesFPFixed) {
    // FMAXV, FMINV, FMAXNMV, FMINNMV
    scope.Record(CPUFeatures::kFP);
  }
}

void CPUFeaturesAuditor::VisitNEONByIndexedElement(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  // All of these instructions require NEON.
  scope.Record(CPUFeatures::kNEON);
  switch (instr->Mask(NEONByIndexedElementMask)) {
    case NEON_SDOT_byelement:
    case NEON_UDOT_byelement:
      scope.Record(CPUFeatures::kDotProduct);
      return;
    case NEON_SQRDMLAH_byelement:
    case NEON_SQRDMLSH_byelement:
      scope.Record(CPUFeatures::kRDM);
      return;
    default:
      // Fall through to check other instructions.
      break;
  }
  switch (instr->Mask(NEONByIndexedElementFPLongMask)) {
    case NEON_FMLAL_H_byelement:
    case NEON_FMLAL2_H_byelement:
    case NEON_FMLSL_H_byelement:
    case NEON_FMLSL2_H_byelement:
      scope.Record(CPUFeatures::kFP, CPUFeatures::kNEONHalf, CPUFeatures::kFHM);
      return;
    default:
      // Fall through to check other instructions.
      break;
  }
  switch (instr->Mask(NEONByIndexedElementFPMask)) {
    case NEON_FMLA_H_byelement:
    case NEON_FMLS_H_byelement:
    case NEON_FMUL_H_byelement:
    case NEON_FMULX_H_byelement:
      scope.Record(CPUFeatures::kNEONHalf);
      VIXL_FALLTHROUGH();
    case NEON_FMLA_byelement:
    case NEON_FMLS_byelement:
    case NEON_FMUL_byelement:
    case NEON_FMULX_byelement:
      scope.Record(CPUFeatures::kFP);
      return;
    default:
      switch (instr->Mask(NEONByIndexedElementFPComplexMask)) {
        case NEON_FCMLA_byelement:
          scope.Record(CPUFeatures::kFP, CPUFeatures::kFcma);
          if (instr->GetNEONSize() == 1) scope.Record(CPUFeatures::kNEONHalf);
          return;
      }
      // No additional features.
      return;
  }
}

void CPUFeaturesAuditor::VisitNEONCopy(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  // All of these instructions require NEON.
  scope.Record(CPUFeatures::kNEON);
  USE(instr);
}

void CPUFeaturesAuditor::VisitNEONExtract(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  // All of these instructions require NEON.
  scope.Record(CPUFeatures::kNEON);
  USE(instr);
}

void CPUFeaturesAuditor::VisitNEONLoadStoreMultiStruct(
    const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  // All of these instructions require NEON.
  scope.Record(CPUFeatures::kNEON);
  USE(instr);
}

void CPUFeaturesAuditor::VisitNEONLoadStoreMultiStructPostIndex(
    const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  // All of these instructions require NEON.
  scope.Record(CPUFeatures::kNEON);
  USE(instr);
}

void CPUFeaturesAuditor::VisitNEONLoadStoreSingleStruct(
    const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  // All of these instructions require NEON.
  scope.Record(CPUFeatures::kNEON);
  USE(instr);
}

void CPUFeaturesAuditor::VisitNEONLoadStoreSingleStructPostIndex(
    const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  // All of these instructions require NEON.
  scope.Record(CPUFeatures::kNEON);
  USE(instr);
}

void CPUFeaturesAuditor::VisitNEONModifiedImmediate(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  // All of these instructions require NEON.
  scope.Record(CPUFeatures::kNEON);
  if (instr->GetNEONCmode() == 0xf) {
    // FMOV (vector, immediate), double-, single- or half-precision.
    scope.Record(CPUFeatures::kFP);
    if (instr->ExtractBit(11)) scope.Record(CPUFeatures::kNEONHalf);
  }
}

void CPUFeaturesAuditor::VisitNEONPerm(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  // All of these instructions require NEON.
  scope.Record(CPUFeatures::kNEON);
  USE(instr);
}

void CPUFeaturesAuditor::VisitNEONScalar2RegMisc(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  // All of these instructions require NEON.
  scope.Record(CPUFeatures::kNEON);
  switch (instr->Mask(NEONScalar2RegMiscFPMask)) {
    case NEON_FRECPE_scalar:
    case NEON_FRECPX_scalar:
    case NEON_FRSQRTE_scalar:
    case NEON_FCMGT_zero_scalar:
    case NEON_FCMGE_zero_scalar:
    case NEON_FCMEQ_zero_scalar:
    case NEON_FCMLE_zero_scalar:
    case NEON_FCMLT_zero_scalar:
    case NEON_SCVTF_scalar:
    case NEON_UCVTF_scalar:
    case NEON_FCVTNS_scalar:
    case NEON_FCVTNU_scalar:
    case NEON_FCVTPS_scalar:
    case NEON_FCVTPU_scalar:
    case NEON_FCVTMS_scalar:
    case NEON_FCVTMU_scalar:
    case NEON_FCVTZS_scalar:
    case NEON_FCVTZU_scalar:
    case NEON_FCVTAS_scalar:
    case NEON_FCVTAU_scalar:
    case NEON_FCVTXN_scalar:
      scope.Record(CPUFeatures::kFP);
      return;
    default:
      // No additional features.
      return;
  }
}

void CPUFeaturesAuditor::VisitNEONScalar2RegMiscFP16(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  // All of these instructions require NEONHalf.
  scope.Record(CPUFeatures::kFP, CPUFeatures::kNEON, CPUFeatures::kNEONHalf);
  USE(instr);
}

void CPUFeaturesAuditor::VisitNEONScalar3Diff(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  // All of these instructions require NEON.
  scope.Record(CPUFeatures::kNEON);
  USE(instr);
}

void CPUFeaturesAuditor::VisitNEONScalar3Same(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  // All of these instructions require NEON.
  scope.Record(CPUFeatures::kNEON);
  if (instr->Mask(NEONScalar3SameFPFMask) == NEONScalar3SameFPFixed) {
    scope.Record(CPUFeatures::kFP);
  }
}

void CPUFeaturesAuditor::VisitNEONScalar3SameExtra(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  // All of these instructions require NEON and RDM.
  scope.Record(CPUFeatures::kNEON, CPUFeatures::kRDM);
  USE(instr);
}

void CPUFeaturesAuditor::VisitNEONScalar3SameFP16(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  // All of these instructions require NEONHalf.
  scope.Record(CPUFeatures::kFP, CPUFeatures::kNEON, CPUFeatures::kNEONHalf);
  USE(instr);
}

void CPUFeaturesAuditor::VisitNEONScalarByIndexedElement(
    const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  // All of these instructions require NEON.
  scope.Record(CPUFeatures::kNEON);
  switch (instr->Mask(NEONScalarByIndexedElementMask)) {
    case NEON_SQRDMLAH_byelement_scalar:
    case NEON_SQRDMLSH_byelement_scalar:
      scope.Record(CPUFeatures::kRDM);
      return;
    default:
      switch (instr->Mask(NEONScalarByIndexedElementFPMask)) {
        case NEON_FMLA_H_byelement_scalar:
        case NEON_FMLS_H_byelement_scalar:
        case NEON_FMUL_H_byelement_scalar:
        case NEON_FMULX_H_byelement_scalar:
          scope.Record(CPUFeatures::kNEONHalf);
          VIXL_FALLTHROUGH();
        case NEON_FMLA_byelement_scalar:
        case NEON_FMLS_byelement_scalar:
        case NEON_FMUL_byelement_scalar:
        case NEON_FMULX_byelement_scalar:
          scope.Record(CPUFeatures::kFP);
          return;
      }
      // No additional features.
      return;
  }
}

void CPUFeaturesAuditor::VisitNEONScalarCopy(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  // All of these instructions require NEON.
  scope.Record(CPUFeatures::kNEON);
  USE(instr);
}

void CPUFeaturesAuditor::VisitNEONScalarPairwise(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  // All of these instructions require NEON.
  scope.Record(CPUFeatures::kNEON);
  switch (instr->Mask(NEONScalarPairwiseMask)) {
    case NEON_FMAXNMP_h_scalar:
    case NEON_FADDP_h_scalar:
    case NEON_FMAXP_h_scalar:
    case NEON_FMINNMP_h_scalar:
    case NEON_FMINP_h_scalar:
      scope.Record(CPUFeatures::kNEONHalf);
      VIXL_FALLTHROUGH();
    case NEON_FADDP_scalar:
    case NEON_FMAXP_scalar:
    case NEON_FMAXNMP_scalar:
    case NEON_FMINP_scalar:
    case NEON_FMINNMP_scalar:
      scope.Record(CPUFeatures::kFP);
      return;
    default:
      // No additional features.
      return;
  }
}

void CPUFeaturesAuditor::VisitNEONScalarShiftImmediate(
    const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  // All of these instructions require NEON.
  scope.Record(CPUFeatures::kNEON);
  switch (instr->Mask(NEONScalarShiftImmediateMask)) {
    case NEON_FCVTZS_imm_scalar:
    case NEON_FCVTZU_imm_scalar:
    case NEON_SCVTF_imm_scalar:
    case NEON_UCVTF_imm_scalar:
      scope.Record(CPUFeatures::kFP);
      // If immh is 0b001x then the data type is FP16, and requires kNEONHalf.
      if ((instr->GetImmNEONImmh() & 0xe) == 0x2) {
        scope.Record(CPUFeatures::kNEONHalf);
      }
      return;
    default:
      // No additional features.
      return;
  }
}

void CPUFeaturesAuditor::VisitNEONShiftImmediate(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  // All of these instructions require NEON.
  scope.Record(CPUFeatures::kNEON);
  switch (instr->Mask(NEONShiftImmediateMask)) {
    case NEON_SCVTF_imm:
    case NEON_UCVTF_imm:
    case NEON_FCVTZS_imm:
    case NEON_FCVTZU_imm:
      scope.Record(CPUFeatures::kFP);
      // If immh is 0b001x then the data type is FP16, and requires kNEONHalf.
      if ((instr->GetImmNEONImmh() & 0xe) == 0x2) {
        scope.Record(CPUFeatures::kNEONHalf);
      }
      return;
    default:
      // No additional features.
      return;
  }
}

void CPUFeaturesAuditor::VisitNEONTable(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  // All of these instructions require NEON.
  scope.Record(CPUFeatures::kNEON);
  USE(instr);
}

void CPUFeaturesAuditor::VisitPCRelAddressing(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  USE(instr);
}

// Most SVE visitors require only SVE.
#define VIXL_SIMPLE_SVE_VISITOR_LIST(V)                          \
  V(SVE32BitGatherLoad_ScalarPlus32BitUnscaledOffsets)           \
  V(SVE32BitGatherLoad_VectorPlusImm)                            \
  V(SVE32BitGatherLoadHalfwords_ScalarPlus32BitScaledOffsets)    \
  V(SVE32BitGatherLoadWords_ScalarPlus32BitScaledOffsets)        \
  V(SVE32BitGatherPrefetch_ScalarPlus32BitScaledOffsets)         \
  V(SVE32BitGatherPrefetch_VectorPlusImm)                        \
  V(SVE32BitScatterStore_ScalarPlus32BitScaledOffsets)           \
  V(SVE32BitScatterStore_ScalarPlus32BitUnscaledOffsets)         \
  V(SVE32BitScatterStore_VectorPlusImm)                          \
  V(SVE64BitGatherLoad_ScalarPlus32BitUnpackedScaledOffsets)     \
  V(SVE64BitGatherLoad_ScalarPlus64BitScaledOffsets)             \
  V(SVE64BitGatherLoad_ScalarPlus64BitUnscaledOffsets)           \
  V(SVE64BitGatherLoad_ScalarPlusUnpacked32BitUnscaledOffsets)   \
  V(SVE64BitGatherLoad_VectorPlusImm)                            \
  V(SVE64BitGatherPrefetch_ScalarPlus64BitScaledOffsets)         \
  V(SVE64BitGatherPrefetch_ScalarPlusUnpacked32BitScaledOffsets) \
  V(SVE64BitGatherPrefetch_VectorPlusImm)                        \
  V(SVE64BitScatterStore_ScalarPlus64BitScaledOffsets)           \
  V(SVE64BitScatterStore_ScalarPlus64BitUnscaledOffsets)         \
  V(SVE64BitScatterStore_ScalarPlusUnpacked32BitScaledOffsets)   \
  V(SVE64BitScatterStore_ScalarPlusUnpacked32BitUnscaledOffsets) \
  V(SVE64BitScatterStore_VectorPlusImm)                          \
  V(SVEAddressGeneration)                                        \
  V(SVEBitwiseLogicalUnpredicated)                               \
  V(SVEBitwiseShiftUnpredicated)                                 \
  V(SVEFFRInitialise)                                            \
  V(SVEFFRWriteFromPredicate)                                    \
  V(SVEFPAccumulatingReduction)                                  \
  V(SVEFPArithmeticUnpredicated)                                 \
  V(SVEFPCompareVectors)                                         \
  V(SVEFPCompareWithZero)                                        \
  V(SVEFPComplexAddition)                                        \
  V(SVEFPComplexMulAdd)                                          \
  V(SVEFPComplexMulAddIndex)                                     \
  V(SVEFPFastReduction)                                          \
  V(SVEFPMulIndex)                                               \
  V(SVEFPMulAdd)                                                 \
  V(SVEFPMulAddIndex)                                            \
  V(SVEFPUnaryOpUnpredicated)                                    \
  V(SVEIncDecByPredicateCount)                                   \
  V(SVEIndexGeneration)                                          \
  V(SVEIntArithmeticUnpredicated)                                \
  V(SVEIntCompareSignedImm)                                      \
  V(SVEIntCompareUnsignedImm)                                    \
  V(SVEIntCompareVectors)                                        \
  V(SVEIntMulAddPredicated)                                      \
  V(SVEIntMulAddUnpredicated)                                    \
  V(SVEIntReduction)                                             \
  V(SVEIntUnaryArithmeticPredicated)                             \
  V(SVEMovprfx)                                                  \
  V(SVEMulIndex)                                                 \
  V(SVEPermuteVectorExtract)                                     \
  V(SVEPermuteVectorInterleaving)                                \
  V(SVEPredicateCount)                                           \
  V(SVEPredicateLogical)                                         \
  V(SVEPropagateBreak)                                           \
  V(SVEStackFrameAdjustment)                                     \
  V(SVEStackFrameSize)                                           \
  V(SVEVectorSelect)                                             \
  V(SVEBitwiseLogical_Predicated)                                \
  V(SVEBitwiseLogicalWithImm_Unpredicated)                       \
  V(SVEBitwiseShiftByImm_Predicated)                             \
  V(SVEBitwiseShiftByVector_Predicated)                          \
  V(SVEBitwiseShiftByWideElements_Predicated)                    \
  V(SVEBroadcastBitmaskImm)                                      \
  V(SVEBroadcastFPImm_Unpredicated)                              \
  V(SVEBroadcastGeneralRegister)                                 \
  V(SVEBroadcastIndexElement)                                    \
  V(SVEBroadcastIntImm_Unpredicated)                             \
  V(SVECompressActiveElements)                                   \
  V(SVEConditionallyBroadcastElementToVector)                    \
  V(SVEConditionallyExtractElementToSIMDFPScalar)                \
  V(SVEConditionallyExtractElementToGeneralRegister)             \
  V(SVEConditionallyTerminateScalars)                            \
  V(SVEConstructivePrefix_Unpredicated)                          \
  V(SVEContiguousFirstFaultLoad_ScalarPlusScalar)                \
  V(SVEContiguousLoad_ScalarPlusImm)                             \
  V(SVEContiguousLoad_ScalarPlusScalar)                          \
  V(SVEContiguousNonFaultLoad_ScalarPlusImm)                     \
  V(SVEContiguousNonTemporalLoad_ScalarPlusImm)                  \
  V(SVEContiguousNonTemporalLoad_ScalarPlusScalar)               \
  V(SVEContiguousNonTemporalStore_ScalarPlusImm)                 \
  V(SVEContiguousNonTemporalStore_ScalarPlusScalar)              \
  V(SVEContiguousPrefetch_ScalarPlusImm)                         \
  V(SVEContiguousPrefetch_ScalarPlusScalar)                      \
  V(SVEContiguousStore_ScalarPlusImm)                            \
  V(SVEContiguousStore_ScalarPlusScalar)                         \
  V(SVECopySIMDFPScalarRegisterToVector_Predicated)              \
  V(SVECopyFPImm_Predicated)                                     \
  V(SVECopyGeneralRegisterToVector_Predicated)                   \
  V(SVECopyIntImm_Predicated)                                    \
  V(SVEElementCount)                                             \
  V(SVEExtractElementToSIMDFPScalarRegister)                     \
  V(SVEExtractElementToGeneralRegister)                          \
  V(SVEFPArithmetic_Predicated)                                  \
  V(SVEFPArithmeticWithImm_Predicated)                           \
  V(SVEFPConvertPrecision)                                       \
  V(SVEFPConvertToInt)                                           \
  V(SVEFPExponentialAccelerator)                                 \
  V(SVEFPRoundToIntegralValue)                                   \
  V(SVEFPTrigMulAddCoefficient)                                  \
  V(SVEFPTrigSelectCoefficient)                                  \
  V(SVEFPUnaryOp)                                                \
  V(SVEIncDecRegisterByElementCount)                             \
  V(SVEIncDecVectorByElementCount)                               \
  V(SVEInsertSIMDFPScalarRegister)                               \
  V(SVEInsertGeneralRegister)                                    \
  V(SVEIntAddSubtractImm_Unpredicated)                           \
  V(SVEIntAddSubtractVectors_Predicated)                         \
  V(SVEIntCompareScalarCountAndLimit)                            \
  V(SVEIntConvertToFP)                                           \
  V(SVEIntDivideVectors_Predicated)                              \
  V(SVEIntMinMaxImm_Unpredicated)                                \
  V(SVEIntMinMaxDifference_Predicated)                           \
  V(SVEIntMulImm_Unpredicated)                                   \
  V(SVEIntMulVectors_Predicated)                                 \
  V(SVELoadAndBroadcastElement)                                  \
  V(SVELoadAndBroadcastQuadword_ScalarPlusImm)                   \
  V(SVELoadAndBroadcastQuadword_ScalarPlusScalar)                \
  V(SVELoadMultipleStructures_ScalarPlusImm)                     \
  V(SVELoadMultipleStructures_ScalarPlusScalar)                  \
  V(SVELoadPredicateRegister)                                    \
  V(SVELoadVectorRegister)                                       \
  V(SVEPartitionBreakCondition)                                  \
  V(SVEPermutePredicateElements)                                 \
  V(SVEPredicateFirstActive)                                     \
  V(SVEPredicateInitialize)                                      \
  V(SVEPredicateNextActive)                                      \
  V(SVEPredicateReadFromFFR_Predicated)                          \
  V(SVEPredicateReadFromFFR_Unpredicated)                        \
  V(SVEPredicateTest)                                            \
  V(SVEPredicateZero)                                            \
  V(SVEPropagateBreakToNextPartition)                            \
  V(SVEReversePredicateElements)                                 \
  V(SVEReverseVectorElements)                                    \
  V(SVEReverseWithinElements)                                    \
  V(SVESaturatingIncDecRegisterByElementCount)                   \
  V(SVESaturatingIncDecVectorByElementCount)                     \
  V(SVEStoreMultipleStructures_ScalarPlusImm)                    \
  V(SVEStoreMultipleStructures_ScalarPlusScalar)                 \
  V(SVEStorePredicateRegister)                                   \
  V(SVEStoreVectorRegister)                                      \
  V(SVETableLookup)                                              \
  V(SVEUnpackPredicateElements)                                  \
  V(SVEUnpackVectorElements)                                     \
  V(SVEVectorSplice_Destructive)

#define VIXL_DEFINE_SIMPLE_SVE_VISITOR(NAME)                       \
  void CPUFeaturesAuditor::Visit##NAME(const Instruction* instr) { \
    RecordInstructionFeaturesScope scope(this);                    \
    scope.Record(CPUFeatures::kSVE);                               \
    USE(instr);                                                    \
  }
VIXL_SIMPLE_SVE_VISITOR_LIST(VIXL_DEFINE_SIMPLE_SVE_VISITOR)
#undef VIXL_DEFINE_SIMPLE_SVE_VISITOR
#undef VIXL_SIMPLE_SVE_VISITOR_LIST

void CPUFeaturesAuditor::VisitSystem(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  if (instr->Mask(SystemHintFMask) == SystemHintFixed) {
    CPUFeatures required;
    switch (instr->GetInstructionBits()) {
      case PACIA1716:
      case PACIB1716:
      case AUTIA1716:
      case AUTIB1716:
      case PACIAZ:
      case PACIASP:
      case PACIBZ:
      case PACIBSP:
      case AUTIAZ:
      case AUTIASP:
      case AUTIBZ:
      case AUTIBSP:
      case XPACLRI:
        required.Combine(CPUFeatures::kPAuth);
        break;
      default:
        switch (instr->GetImmHint()) {
          case ESB:
            required.Combine(CPUFeatures::kRAS);
            break;
          case BTI:
          case BTI_j:
          case BTI_c:
          case BTI_jc:
            required.Combine(CPUFeatures::kBTI);
            break;
          default:
            break;
        }
        break;
    }

    // These are all HINT instructions, and behave as NOPs if the corresponding
    // features are not implemented, so we record the corresponding features
    // only if they are available.
    if (available_.Has(required)) scope.Record(required);
  } else if (instr->Mask(SystemSysMask) == SYS) {
    switch (instr->GetSysOp()) {
      // DC instruction variants.
      case CVAP:
        scope.Record(CPUFeatures::kDCPoP);
        break;
      case CVADP:
        scope.Record(CPUFeatures::kDCCVADP);
        break;
      case IVAU:
      case CVAC:
      case CVAU:
      case CIVAC:
        // No special CPU features.
        break;
    }
  } else if (instr->Mask(SystemPStateFMask) == SystemPStateFixed) {
    switch (instr->Mask(SystemPStateMask)) {
      case CFINV:
        scope.Record(CPUFeatures::kFlagM);
        break;
      case AXFLAG:
      case XAFLAG:
        scope.Record(CPUFeatures::kAXFlag);
        break;
    }
  } else if (instr->Mask(SystemSysRegFMask) == SystemSysRegFixed) {
    bool mrs = instr->Mask(SystemSysRegMask) == MRS;
    switch (instr->GetImmSystemRegister()) {
      case RSP_EL0:
        scope.Record(CPUFeatures::kMorello);
        break;
      case RNDR:
      case RNDRRS:
        if (mrs) scope.Record(CPUFeatures::kRNG);
        break;
    }
  }
}

void CPUFeaturesAuditor::VisitTestBranch(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  USE(instr);
}

void CPUFeaturesAuditor::VisitUnallocated(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  USE(instr);
}

void CPUFeaturesAuditor::VisitUnconditionalBranch(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  USE(instr);
}

void CPUFeaturesAuditor::VisitUnconditionalBranchToRegister(
    const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  switch (instr->Mask(UnconditionalBranchToRegisterMask)) {
    case BRAAZ:
    case BRABZ:
    case BLRAAZ:
    case BLRABZ:
    case RETAA:
    case RETAB:
    case BRAA:
    case BRAB:
    case BLRAA:
    case BLRAB:
      scope.Record(CPUFeatures::kPAuth);
      return;
    default:
      // No additional features.
      return;
  }
}

void CPUFeaturesAuditor::VisitReserved(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  USE(instr);
}

void CPUFeaturesAuditor::VisitUnimplemented(const Instruction* instr) {
  RecordInstructionFeaturesScope scope(this);
  USE(instr);
}


}  // namespace aarch64
}  // namespace vixl
