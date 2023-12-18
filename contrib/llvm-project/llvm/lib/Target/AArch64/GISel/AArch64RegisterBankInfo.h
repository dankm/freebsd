//===- AArch64RegisterBankInfo -----------------------------------*- C++ -*-==//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
/// \file
/// This file declares the targeting of the RegisterBankInfo class for AArch64.
/// \todo This should be generated by TableGen.
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_AARCH64_AARCH64REGISTERBANKINFO_H
#define LLVM_LIB_TARGET_AARCH64_AARCH64REGISTERBANKINFO_H

#include "llvm/CodeGen/GlobalISel/GenericMachineInstrs.h"
#include "llvm/CodeGen/RegisterBankInfo.h"

#define GET_REGBANK_DECLARATIONS
#include "AArch64GenRegisterBank.inc"

namespace llvm {

class TargetRegisterInfo;

class AArch64GenRegisterBankInfo : public RegisterBankInfo {
protected:
  enum PartialMappingIdx {
    PMI_None = -1,
    PMI_FPR16 = 1,
    PMI_FPR32,
    PMI_FPR64,
    PMI_FPR128,
    PMI_FPR256,
    PMI_FPR512,
    PMI_GPR32,
    PMI_GPR64,
    PMI_GPR128,
    PMI_FirstGPR = PMI_GPR32,
    PMI_LastGPR = PMI_GPR128,
    PMI_FirstFPR = PMI_FPR16,
    PMI_LastFPR = PMI_FPR512,
    PMI_Min = PMI_FirstFPR,
  };

  static const RegisterBankInfo::PartialMapping PartMappings[];
  static const RegisterBankInfo::ValueMapping ValMappings[];
  static const PartialMappingIdx BankIDToCopyMapIdx[];

  enum ValueMappingIdx {
    InvalidIdx = 0,
    First3OpsIdx = 1,
    Last3OpsIdx = 25,
    DistanceBetweenRegBanks = 3,
    FirstCrossRegCpyIdx = 28,
    LastCrossRegCpyIdx = 42,
    DistanceBetweenCrossRegCpy = 2,
    FPExt16To32Idx = 44,
    FPExt16To64Idx = 46,
    FPExt32To64Idx = 48,
    FPExt64To128Idx = 50,
    Shift64Imm = 52,
  };

  static bool checkPartialMap(unsigned Idx, unsigned ValStartIdx,
                              unsigned ValLength, const RegisterBank &RB);
  static bool checkValueMapImpl(unsigned Idx, unsigned FirstInBank,
                                unsigned Size, unsigned Offset);
  static bool checkPartialMappingIdx(PartialMappingIdx FirstAlias,
                                     PartialMappingIdx LastAlias,
                                     ArrayRef<PartialMappingIdx> Order);

  static unsigned getRegBankBaseIdxOffset(unsigned RBIdx, unsigned Size);

  /// Get the pointer to the ValueMapping representing the RegisterBank
  /// at \p RBIdx with a size of \p Size.
  ///
  /// The returned mapping works for instructions with the same kind of
  /// operands for up to 3 operands.
  ///
  /// \pre \p RBIdx != PartialMappingIdx::None
  static const RegisterBankInfo::ValueMapping *
  getValueMapping(PartialMappingIdx RBIdx, unsigned Size);

  /// Get the pointer to the ValueMapping of the operands of a copy
  /// instruction from the \p SrcBankID register bank to the \p DstBankID
  /// register bank with a size of \p Size.
  static const RegisterBankInfo::ValueMapping *
  getCopyMapping(unsigned DstBankID, unsigned SrcBankID, unsigned Size);

  /// Get the instruction mapping for G_FPEXT.
  ///
  /// \pre (DstSize, SrcSize) pair is one of the following:
  ///      (32, 16), (64, 16), (64, 32), (128, 64)
  ///
  /// \return An InstructionMapping with statically allocated OperandsMapping.
  static const RegisterBankInfo::ValueMapping *
  getFPExtMapping(unsigned DstSize, unsigned SrcSize);

#define GET_TARGET_REGBANK_CLASS
#include "AArch64GenRegisterBank.inc"
};

/// This class provides the information for the target register banks.
class AArch64RegisterBankInfo final : public AArch64GenRegisterBankInfo {
  /// See RegisterBankInfo::applyMapping.
  void applyMappingImpl(MachineIRBuilder &Builder,
                        const OperandsMapper &OpdMapper) const override;

  /// Get an instruction mapping where all the operands map to
  /// the same register bank and have similar size.
  ///
  /// \pre MI.getNumOperands() <= 3
  ///
  /// \return An InstructionMappings with a statically allocated
  /// OperandsMapping.
  const InstructionMapping &
  getSameKindOfOperandsMapping(const MachineInstr &MI) const;

  /// Maximum recursion depth for hasFPConstraints.
  const unsigned MaxFPRSearchDepth = 2;

  /// \returns true if \p MI only uses and defines FPRs.
  bool hasFPConstraints(const MachineInstr &MI, const MachineRegisterInfo &MRI,
                     const TargetRegisterInfo &TRI, unsigned Depth = 0) const;

  /// \returns true if \p MI only uses FPRs.
  bool onlyUsesFP(const MachineInstr &MI, const MachineRegisterInfo &MRI,
                  const TargetRegisterInfo &TRI, unsigned Depth = 0) const;

  /// \returns true if \p MI only defines FPRs.
  bool onlyDefinesFP(const MachineInstr &MI, const MachineRegisterInfo &MRI,
                     const TargetRegisterInfo &TRI, unsigned Depth = 0) const;

  /// \returns true if the load \p MI is likely loading from a floating-point
  /// type.
  bool isLoadFromFPType(const MachineInstr &MI) const;

public:
  AArch64RegisterBankInfo(const TargetRegisterInfo &TRI);

  unsigned copyCost(const RegisterBank &A, const RegisterBank &B,
                    TypeSize Size) const override;

  const RegisterBank &getRegBankFromRegClass(const TargetRegisterClass &RC,
                                             LLT) const override;

  InstructionMappings
  getInstrAlternativeMappings(const MachineInstr &MI) const override;

  const InstructionMapping &
  getInstrMapping(const MachineInstr &MI) const override;
};
} // End llvm namespace.
#endif
