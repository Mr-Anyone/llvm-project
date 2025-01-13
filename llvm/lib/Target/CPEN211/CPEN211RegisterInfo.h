//===-- CPEN211RegisterInfo.h - MSP430 Register Information Impl -*- C++
//-*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the CPEN211 implementation of the MRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_CPEN211_MSP430REGISTERINFO_H
#define LLVM_LIB_TARGET_CPEN211_MSP430REGISTERINFO_H

#include "llvm/CodeGen/TargetRegisterInfo.h"

#define GET_REGINFO_HEADER
#include "CPEN211GenRegisterInfo.inc"

namespace llvm {

class CPEN211RegisterInfo : public CPEN211GenRegisterInfo {
public:
  CPEN211RegisterInfo();

  /// Code Generation virtual methods...
  const MCPhysReg *getCalleeSavedRegs(const MachineFunction *MF) const override;

  BitVector getReservedRegs(const MachineFunction &MF) const override;
  const TargetRegisterClass *
  getPointerRegClass(const MachineFunction &MF,
                     unsigned Kind = 0) const override;

  bool eliminateFrameIndex(MachineBasicBlock::iterator II, int SPAdj,
                           unsigned FIOperandNum,
                           RegScavenger *RS = nullptr) const override;

  // Debug information queries.
  Register getFrameRegister(const MachineFunction &MF) const override;
};

} // end namespace llvm

#endif
