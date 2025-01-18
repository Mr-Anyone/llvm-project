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

using namespace llvm;

#define DEBUG_TYPE "msp430-reg-info"

#define GET_REGINFO_TARGET_DESC
#include "CPEN211GenRegisterInfo.inc"

// FIXME: Provide proper call frame setup / destroy opcodes.
CPEN211RegisterInfo::CPEN211RegisterInfo()
    // TODO
    : CPEN211GenRegisterInfo(CPEN211::R1) {
  // llvm_unreachable("Please decide a PC on top! ");
}

const MCPhysReg *
CPEN211RegisterInfo::getCalleeSavedRegs(const MachineFunction *MF) const {
  const CPEN211FrameLowering *TFI = getFrameLowering(*MF);
  const Function *F = &MF->getFunction();
  static const MCPhysReg CalleeSavedRegs[] = {CPEN211::R4, CPEN211::R5,
                                              CPEN211::R6, CPEN211::R7, 0};

  static const MCPhysReg CalleeSavedRegsFP[] = {CPEN211::R5, CPEN211::R6,
                                                CPEN211::R7, 0};
  static const MCPhysReg CalleeSavedRegsIntr[] = {CPEN211::R4, CPEN211::R5,
                                                  CPEN211::R6, CPEN211::R7, 0};

  static const MCPhysReg CalleeSavedRegsIntrFP[] = {CPEN211::R5, CPEN211::R6,
                                                    CPEN211::R7};

  // TODO (for Vincent): fix the following
  // we have to define a custom calling convention I guess
  if (TFI->hasFP(*MF))
    return (F->getCallingConv() == CallingConv::MSP430_INTR
                ? CalleeSavedRegsIntrFP
                : CalleeSavedRegsFP);
  else
    return (F->getCallingConv() == CallingConv::MSP430_INTR
                ? CalleeSavedRegsIntr
                : CalleeSavedRegs);
}

BitVector
CPEN211RegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());
  const CPEN211FrameLowering *TFI = getFrameLowering(MF);

  // Mark 4 special registers with subregisters as reserved.
  // Reserved.set(CPEN211::PCB);
  // Reserved.set(CPEN211::SPB);
  // Reserved.set(CPEN211::SRB);
  // Reserved.set(CPEN211::CGB);
  // Reserved.set(CPEN211::PC);
  // Reserved.set(CPEN211::SP);
  // Reserved.set(CPEN211::SR);
  // Reserved.set(CPEN211::CG);

  // Mark frame pointer as reserved if needed.
  // if (TFI->hasFP(MF)) {
  //   Reserved.set(CPEN211::R4B);
  //   Reserved.set(CPEN211::R4);
  // }

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
  llvm_unreachable("this is not yet implemented");
  // assert(SPAdj == 0 && "Unexpected");

  // MachineInstr &MI = *II;
  // MachineBasicBlock &MBB = *MI.getParent();
  // MachineFunction &MF = *MBB.getParent();
  // const CPEN211FrameLowering *TFI = getFrameLowering(MF);
  // DebugLoc dl = MI.getDebugLoc();
  // int FrameIndex = MI.getOperand(FIOperandNum).getIndex();

  // unsigned BasePtr = (TFI->hasFP(MF) ? CPEN211::R4 : CPEN211::SP);
  // int Offset = MF.getFrameInfo().getObjectOffset(FrameIndex);

  // // Skip the saved PC
  // Offset += 2;

  // if (!TFI->hasFP(MF))
  //   Offset += MF.getFrameInfo().getStackSize();
  // else
  //   Offset += 2; // Skip the saved FP

  // // Fold imm into offset
  // Offset += MI.getOperand(FIOperandNum + 1).getImm();

  // if (MI.getOpcode() == CPEN211::ADDframe) {
  //   // This is actually "load effective address" of the stack slot
  //   // instruction. We have only two-address instructions, thus we need to
  //   // expand it into mov + add
  //   const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();

  //   MI.setDesc(TII.get(CPEN211::MOV16rr));
  //   MI.getOperand(FIOperandNum).ChangeToRegister(BasePtr, false);

  //   // Remove the now unused Offset operand.
  //   MI.removeOperand(FIOperandNum + 1);

  //   if (Offset == 0)
  //     return false;

  //   // We need to materialize the offset via add instruction.
  //   Register DstReg = MI.getOperand(0).getReg();
  //   if (Offset < 0)
  //     BuildMI(MBB, std::next(II), dl, TII.get(CPEN211::SUB16ri), DstReg)
  //         .addReg(DstReg)
  //         .addImm(-Offset);
  //   else
  //     BuildMI(MBB, std::next(II), dl, TII.get(CPEN211::ADD16ri), DstReg)
  //         .addReg(DstReg)
  //         .addImm(Offset);

  //   return false;
  // }

  // MI.getOperand(FIOperandNum).ChangeToRegister(BasePtr, false);
  // MI.getOperand(FIOperandNum + 1).ChangeToImmediate(Offset);
  // return false;
}

Register
CPEN211RegisterInfo::getFrameRegister(const MachineFunction &MF) const {
  const CPEN211FrameLowering *TFI = getFrameLowering(MF);

  llvm_unreachable("this is not yet implemented!");
  // return TFI->hasFP(MF) ? CPEN211::R4 : CPEN211::SP;
  return CPEN211::R4;
}
