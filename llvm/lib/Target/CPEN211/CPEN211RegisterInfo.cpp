//===-- CPEN211RegisterInfo.cpp - CPEN211 Register Information
//--------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the CPEN211 implementation of the TargetRegisterInfo
// class.
//
//===----------------------------------------------------------------------===//

#include "CPEN211RegisterInfo.h"
#include "CPEN211TargetMachine.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/IR/Function.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Target/TargetOptions.h"
#include <cstdlib>

using namespace llvm;

#define DEBUG_TYPE "msp430-reg-info"

#define GET_REGINFO_TARGET_DESC
#include "CPEN211GenRegisterInfo.inc"

// FIXME: Provide proper call frame setup / destroy opcodes.
CPEN211RegisterInfo::CPEN211RegisterInfo()
    // TODO (for Vincent): is this the stack pointer or is this the link
    // or is this the link register
    : CPEN211GenRegisterInfo(CPEN211::SP) {
  // llvm_unreachable("Please decide a PC on top! ");
}

const MCPhysReg *
CPEN211RegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  const CPEN211FrameLowering *TFI = getFrameLowering(*MF);
  const Function *F = &MF->getFunction();
  static const MCPhysReg CalleeSavedRegs[] = {CPEN211::R5, CPEN211::SP,
                                              CPEN211::LR};

  // TODO (for Vincent): this may changes depending on
  // MF Calling Convention
  return CalleeSavedRegs;
}

BitVector
CPEN211RegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());
  // 
  Reserved.set(CPEN211::SP);
  Reserved.set(CPEN211::LR);
  Reserved.set(CPEN211::SR); // status register

  return Reserved;
}

const TargetRegisterClass *
CPEN211RegisterInfo::getPointerRegClass(const MachineFunction &MF,
                                        unsigned Kind) const {
  return &CPEN211::GR16RegClass;
}

bool CPEN211RegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                              int SPAdj, unsigned FIOperandNum,
                                              RegScavenger *RS) const {
  // llvm_unreachable("this is not yet implemented");
  assert(SPAdj == 0 && "Unexpected");

  MachineInstr &MI = *II;
  MachineBasicBlock &MBB = *MI.getParent();
  MachineFunction &MF = *MBB.getParent();
  const CPEN211FrameLowering *TFI = getFrameLowering(MF);
  DebugLoc dl = MI.getDebugLoc();
  int FrameIndex = MI.getOperand(FIOperandNum).getIndex();

  assert(!TFI->hasFP(MF) && "unexpected");
  unsigned BasePtr = CPEN211::SP;
  // offset have to be

  int Offset = MF.getFrameInfo().getObjectOffset(FrameIndex);

  // TODO (for Vincent): what even is this offset?
  // check this offset!
  if (!TFI->hasFP(MF))
    Offset += MF.getFrameInfo().getStackSize();
  else
    Offset += 2; // Skip the saved FP

  // Fold imm into offset
  Offset += MI.getOperand(FIOperandNum + 1).getImm();
  assert(Offset % 2 == 0 && "Offset mus be divisible by two!");
  // TODO (for Vincent): is this even correct?
  Offset = Offset /
           2; // this is because of the weirdness of the CPEN211 memory model

  MI.getOperand(FIOperandNum).ChangeToRegister(BasePtr, false);
  MI.getOperand(FIOperandNum + 1).ChangeToImmediate(Offset);
  return false;
}

Register
CPEN211RegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  const CPEN211FrameLowering *TFI = getFrameLowering(MF);
  // llvm_unreachable("this is not yet implemented!");
  // make the frame pointer the stack pointer
  // this may be janky?
  assert(!TFI->hasFP(MF) && "where is the frame pointer?");
  return CPEN211::SP;
}
