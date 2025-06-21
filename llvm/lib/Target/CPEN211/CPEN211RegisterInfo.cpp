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
  static const MCPhysReg CalleeSavedRegs[] = {CPEN211::R3, CPEN211::R5,
                                              CPEN211::LR};

  // TODO (for Vincent): this may changes depending on
  // MF Calling Convention
  return CalleeSavedRegs;
}

BitVector
CPEN211RegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());

  Reserved.set(CPEN211::R4); // reserved for constant folding magic
  Reserved.set(CPEN211::R5); // frame pointer
  Reserved.set(CPEN211::SP); // stack pointer
  Reserved.set(CPEN211::LR); // link register
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

  // We have select AddFrame instruction, so
  // This is the case where we need to calculate the frame index instead!
  if (MI.getOpcode() == CPEN211::AddFrame) {
    // This is actually "load effective address" of the stack slot
    // instruction. We have only two-address instructions, thus we need to
    // expand it into mov + add
    assert(MI.getOperand(FIOperandNum + 1).getImm() == 0 &&
           "offset must be zero");

    const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();
    MI.setDesc(TII.get(CPEN211::MOV16rr));
    MI.getOperand(FIOperandNum).ChangeToRegister(CPEN211::SP, false);
    MI.removeOperand(FIOperandNum + 1); // remove the offset, which

    assert(Offset > 0 && "offset must be gerater than 0!");

    Register DstReg = MI.getOperand(FIOperandNum - 1).getReg();

    BuildMI(MBB, std::next(II), dl, TII.get(CPEN211::MOV16ri), CPEN211::R4)
        .addImm(Offset);
    II = std::next(II);
    if (Offset > 0)
      BuildMI(MBB, std::next(II), dl, TII.get(CPEN211::ADD16rr), DstReg)
          .addReg(CPEN211::R4)
          .addReg(CPEN211::SP);

    return false;
  }

  // for the typical normal case, if the offset is way too big
  // STR or LDR requires the offset to be in between -16 <= Offset <= 15
  // trying moving into a intermediate register and adding it up
  if (Offset > 15 || Offset < -16) {
    const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();
    // Address calculation STR R0, [Base, offset] 
    BuildMI(MBB, II, dl, TII.get(CPEN211::MOV16ri), CPEN211::R4)
        .addImm(Offset);

    BuildMI(MBB, II, dl,  TII.get(CPEN211::ADD16rr), CPEN211::R4)
        .addReg(CPEN211::R4)
        .addReg(BasePtr);

    MI.getOperand(FIOperandNum).ChangeToRegister(CPEN211::R4, false);
    MI.getOperand(FIOperandNum + 1).ChangeToImmediate(0);
    return false;
  }

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
