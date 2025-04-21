//===-- CPEN211FrameLowering.cpp - CPEN211 Frame Information
//----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the CPEN211 implementation of TargetFrameLowering class.
//
//===----------------------------------------------------------------------===//

#include "CPEN211FrameLowering.h"
#include "CPEN211InstrInfo.h"
#include "CPEN211MachineFunctionInfo.h"
#include "CPEN211Subtarget.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/MachineModuleInfo.h"
#include "llvm/Target/TargetOptions.h"

using namespace llvm;

#define DEBUG_TYPE "cpen211-framelowering"

CPEN211FrameLowering::CPEN211FrameLowering(const CPEN211Subtarget &STI)
    // TODO (for Vincent): I don't think this is right?
    // LocalAreaOffset is -2? I am pretty sure this is 0 CHECK THIS!
    : TargetFrameLowering(TargetFrameLowering::StackGrowsDown, Align(2), 0),
      STI(STI), TII(*STI.getInstrInfo()), TRI(STI.getRegisterInfo()) {}

bool CPEN211FrameLowering::hasFPImpl(const MachineFunction &MF) const {
  const MachineFrameInfo &MFI = MF.getFrameInfo();

  return false;
}

bool CPEN211FrameLowering::hasReservedCallFrame(
    const MachineFunction &MF) const {
  // assume infinite stack is a bad idea?
  return false;
}

void CPEN211FrameLowering::emitPrologue(MachineFunction &MF,
                                        MachineBasicBlock &MBB) const {

  assert(&MF.front() == &MBB && "Shrink-wrapping not yet supported");
  MachineFrameInfo &MFI = MF.getFrameInfo();
  CPEN211MachineFunctionInfo *CPEN211FI =
      MF.getInfo<CPEN211MachineFunctionInfo>();
  const CPEN211InstrInfo &TII =
      *static_cast<const CPEN211InstrInfo *>(MF.getSubtarget().getInstrInfo());

  // this is another problem for future me
  CPEN211FI->getCalleeSavedFrameSize();

  // assert(CPEN211FI->getCalleeSavedFrameSize() == 0 &&
  // "this is another future problem for me!future");

  MachineBasicBlock::iterator MBBI = MBB.begin();
  DebugLoc DL = MBBI != MBB.end() ? MBBI->getDebugLoc() : DebugLoc();

  // Get the number of bytes to allocate from the FrameInfo.
  uint64_t StackSize = MFI.getStackSize();
  LLVM_DEBUG(dbgs() << "Framesize: " << CPEN211FI->getCalleeSavedFrameSize()
                    << " Stack Size: " << StackSize << "\n");

  // TODO: we also have to save link register?
  assert(StackSize % 2 == 0);
  uint64_t SubtractSize = StackSize / 2;

  // SUB R1, R2, #10
  // converts into the following:
  // MOV R4, #-10
  // ADD R1, R2, R4
  BuildMI(MBB, MBBI, DL, TII.get(CPEN211::MOV16ri), CPEN211::R4)
      .addImm(SubtractSize * -1);

  BuildMI(MBB, MBBI, DL, TII.get(CPEN211::ADD16rr), CPEN211::SP)
      .addReg(CPEN211::SP)
      .addReg(CPEN211::R4);

  // BuildMI(MBB, MBBI, DL, TII.get(CPEN211::SUB16ri), CPEN211::SP)
  //     .addReg(CPEN211::SP)
  //     .addImm(SubtractSize)
  //     .setMIFlag(MachineInstr::FrameSetup);

  // assert(StackSize % 2 == 0 && "must be divisible by two!");
  // llvm_unreachable("error!");

  // uint64_t NumBytes = 0;
  // if (hasFP(MF)) {
  //   // Calculate required stack adjustment
  //   uint64_t FrameSize = StackSize - 2;
  //   NumBytes = FrameSize - CPEN211FI->getCalleeSavedFrameSize();

  //   // Get the offset of the stack slot for the EBP register... which is
  //   // guaranteed to be the last slot by processFunctionBeforeFrameFinalized.
  //   // Update the frame offset adjustment.
  //   MFI.setOffsetAdjustment(-NumBytes);

  //   // Save FP into the appropriate stack slot...
  //   BuildMI(MBB, MBBI, DL, TII.get(CPEN211::PUSH16r))
  //       .addReg(CPEN211::R4, RegState::Kill)
  //       .setMIFlag(MachineInstr::FrameSetup);

  //   // Mark the place where FP was saved.
  //   // Define the current CFA rule to use the provided offset.
  //   BuildCFI(MBB, MBBI, DL,
  //            MCCFIInstruction::cfiDefCfaOffset(nullptr, -2 * stackGrowth),
  //            MachineInstr::FrameSetup);

  //   // Change the rule for the FramePtr to be an "offset" rule.
  //   unsigned DwarfFramePtr = TRI->getDwarfRegNum(CPEN211::R4, true);
  //   BuildCFI(
  //       MBB, MBBI, DL,
  //       MCCFIInstruction::createOffset(nullptr, DwarfFramePtr, 2 *
  //       stackGrowth), MachineInstr::FrameSetup);

  //   // Update FP with the new base value...
  //   BuildMI(MBB, MBBI, DL, TII.get(CPEN211::MOV16rr), CPEN211::R4)
  //       .addReg(CPEN211::SP)
  //       .setMIFlag(MachineInstr::FrameSetup);

  //   // Mark effective beginning of when frame pointer becomes valid.
  //   // Define the current CFA to use the FP register.
  //   BuildCFI(MBB, MBBI, DL,
  //            MCCFIInstruction::createDefCfaRegister(nullptr, DwarfFramePtr),
  //            MachineInstr::FrameSetup);

  //   // Mark the FramePtr as live-in in every block except the entry.
  //   for (MachineBasicBlock &MBBJ : llvm::drop_begin(MF))
  //     MBBJ.addLiveIn(CPEN211::R4);
  // } else
  //   NumBytes = StackSize - CPEN211FI->getCalleeSavedFrameSize();

  // // Skip the callee-saved push instructions.
  // int StackOffset = 2 * stackGrowth;
  // while (MBBI != MBB.end() && MBBI->getFlag(MachineInstr::FrameSetup) &&
  //        (MBBI->getOpcode() == CPEN211::PUSH16r)) {
  //   ++MBBI;

  //   if (!hasFP(MF)) {
  //     // Mark callee-saved push instruction.
  //     // Define the current CFA rule to use the provided offset.
  //     assert(StackSize && "Expected stack frame");
  //     BuildCFI(MBB, MBBI, DL,
  //              MCCFIInstruction::cfiDefCfaOffset(nullptr, -StackOffset),
  //              MachineInstr::FrameSetup);
  //     StackOffset += stackGrowth;
  //   }
  // }

  // if (MBBI != MBB.end())
  //   DL = MBBI->getDebugLoc();

  // if (NumBytes) { // adjust stack pointer: SP -= numbytes
  //   // If there is an SUB16ri of SP immediately before this instruction,
  //   merge
  //   // the two.
  //   // NumBytes -= mergeSPUpdates(MBB, MBBI, true);
  //   // If there is an ADD16ri or SUB16ri of SP immediately after this
  //   // instruction, merge the two instructions.
  //   // mergeSPUpdatesDown(MBB, MBBI, &NumBytes);

  //   if (NumBytes) {
  //     MachineInstr *MI =
  //         BuildMI(MBB, MBBI, DL, TII.get(CPEN211::SUB16ri), CPEN211::SP)
  //             .addReg(CPEN211::SP)
  //             .addImm(NumBytes)
  //             .setMIFlag(MachineInstr::FrameSetup);
  //     // The SRW implicit def is dead.
  //     MI->getOperand(3).setIsDead();
  //   }
  //   if (!hasFP(MF)) {
  //     // Adjust the previous CFA value if CFA was not redefined by FP
  //     BuildCFI(
  //         MBB, MBBI, DL,
  //         MCCFIInstruction::cfiDefCfaOffset(nullptr, StackSize -
  //         stackGrowth), MachineInstr::FrameSetup);
  //   }
  // }

  // emitCalleeSavedFrameMoves(MBB, MBBI, DL, true);
}

void CPEN211FrameLowering::emitEpilogue(MachineFunction &MF,
                                        MachineBasicBlock &MBB) const {

  // llvm_unreachable("this is a impossible place to reach!!!");
  const MachineFrameInfo &MFI = MF.getFrameInfo();
  CPEN211MachineFunctionInfo *CPEN211FI =
      MF.getInfo<CPEN211MachineFunctionInfo>();
  const CPEN211InstrInfo &TII =
      *static_cast<const CPEN211InstrInfo *>(MF.getSubtarget().getInstrInfo());

  MachineBasicBlock::iterator MBBI = MBB.getLastNonDebugInstr();
  unsigned RetOpcode = MBBI->getOpcode();
  DebugLoc DL = MBBI->getDebugLoc();

  switch (RetOpcode) {
  case CPEN211::RET:
    break; // These are ok
  default:
    llvm_unreachable("Can only insert epilog into returning blocks");
  }

  // Get the number of bytes to allocate from the FrameInfo
  uint64_t StackSize = MFI.getStackSize();
  unsigned CSSize = CPEN211FI->getCalleeSavedFrameSize();
  uint64_t NumBytes = 0;

  BuildMI(MBB, MBBI, DL, TII.get(CPEN211::MOV16ri), CPEN211::R4)
      .addImm(StackSize);

  BuildMI(MBB, MBBI, DL, TII.get(CPEN211::ADD16rr), CPEN211::SP)
      .addReg(CPEN211::SP)
      .addReg(CPEN211::R4)
      .setMIFlags(MachineInstr::FrameDestroy);

  // assert(CSSize == 0 && "no idea how to restore the stack for now!");

  // MachineBasicBlock::iterator AfterPop = MBBI;
  // if (hasFP(MF)) {
  //   // Calculate required stack adjustment
  //   uint64_t FrameSize = StackSize - 2;
  //   NumBytes = FrameSize - CSSize;

  //   // pop FP.
  //   BuildMI(MBB, MBBI, DL, TII.get(CPEN211::POP16r), CPEN211::R4)
  //       .setMIFlag(MachineInstr::FrameDestroy);
  //   unsigned DwarfStackPtr = TRI->getDwarfRegNum(CPEN211::SP, true);
  //   BuildCFI(MBB, MBBI, DL,
  //            MCCFIInstruction::cfiDefCfa(nullptr, DwarfStackPtr, 2),
  //            MachineInstr::FrameDestroy);
  //   --MBBI;
  //   if (!MBB.succ_empty() && !MBB.isReturnBlock()) {
  //     unsigned DwarfFramePtr = TRI->getDwarfRegNum(CPEN211::R4, true);
  //     BuildCFI(MBB, AfterPop, DL,
  //              MCCFIInstruction::createRestore(nullptr, DwarfFramePtr),
  //              MachineInstr::FrameDestroy);
  //     --MBBI;
  //     --AfterPop;
  //   }
  // } else
  //   NumBytes = StackSize - CSSize;

  // // Skip the callee-saved pop instructions.
  // MachineBasicBlock::iterator FirstCSPop = MBBI;
  // while (MBBI != MBB.begin()) {
  //   MachineBasicBlock::iterator PI = std::prev(MBBI);
  //   unsigned Opc = PI->getOpcode();
  //   if ((Opc != CPEN211::POP16r || !PI->getFlag(MachineInstr::FrameDestroy))
  //   &&
  //       !PI->isTerminator())
  //     break;
  //   FirstCSPop = PI;
  //   --MBBI;
  // }
  // MBBI = FirstCSPop;

  // DL = MBBI->getDebugLoc();

  // // If there is an ADD16ri or SUB16ri of SP immediately before this
  // // instruction, merge the two instructions.
  // // if (NumBytes || MFI.hasVarSizedObjects())
  // //  mergeSPUpdatesUp(MBB, MBBI, StackPtr, &NumBytes);

  // if (MFI.hasVarSizedObjects()) {
  //   BuildMI(MBB, MBBI, DL, TII.get(CPEN211::MOV16rr), CPEN211::SP)
  //       .addReg(CPEN211::R4)
  //       .setMIFlag(MachineInstr::FrameDestroy);
  //   if (CSSize) {
  //     MachineInstr *MI =
  //         BuildMI(MBB, MBBI, DL, TII.get(CPEN211::SUB16ri), CPEN211::SP)
  //             .addReg(CPEN211::SP)
  //             .addImm(CSSize)
  //             .setMIFlag(MachineInstr::FrameDestroy);
  //     // The SRW implicit def is dead.
  //     MI->getOperand(3).setIsDead();
  //   }
  // } else {
  //   // adjust stack pointer back: SP += numbytes
  //   if (NumBytes) {
  //     MachineInstr *MI =
  //         BuildMI(MBB, MBBI, DL, TII.get(CPEN211::ADD16ri), CPEN211::SP)
  //             .addReg(CPEN211::SP)
  //             .addImm(NumBytes)
  //             .setMIFlag(MachineInstr::FrameDestroy);
  //     // The SRW implicit def is dead.
  //     MI->getOperand(3).setIsDead();

  //     if (!hasFP(MF)) {
  //       // Adjust CFA value if it was defined by SP
  //       BuildCFI(MBB, MBBI, DL,
  //                MCCFIInstruction::cfiDefCfaOffset(nullptr, CSSize + 2),
  //                MachineInstr::FrameDestroy);
  //     }
  //   }
  // }

  // if (!hasFP(MF)) {
  //   MBBI = FirstCSPop;
  //   int64_t Offset = -(int64_t)CSSize - 2;
  //   // Mark callee-saved pop instruction.
  //   // Define the current CFA rule to use the provided offset.
  //   while (MBBI != MBB.end()) {
  //     MachineBasicBlock::iterator PI = MBBI;
  //     unsigned Opc = PI->getOpcode();
  //     ++MBBI;
  //     if (Opc == CPEN211::POP16r) {
  //       Offset += 2;
  //       BuildCFI(MBB, MBBI, DL,
  //                MCCFIInstruction::cfiDefCfaOffset(nullptr, -Offset),
  //                MachineInstr::FrameDestroy);
  //     }
  //   }
  // }
  // emitCalleeSavedFrameMoves(MBB, AfterPop, DL, false);
}

// FIXME: Can we eleminate these in favour of generic code?
bool CPEN211FrameLowering::spillCalleeSavedRegisters(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator MI,
    ArrayRef<CalleeSavedInfo> CSI, const TargetRegisterInfo *TRI) const {
  if (CSI.empty())
    return false;

  DebugLoc DL;
  if (MI != MBB.end())
    DL = MI->getDebugLoc();

  MachineFunction &MF = *MBB.getParent();
  const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();
  CPEN211MachineFunctionInfo *MFI = MF.getInfo<CPEN211MachineFunctionInfo>();
  MFI->setCalleeSavedFrameSize(CSI.size() * 2);

  for (const CalleeSavedInfo &I : llvm::reverse(CSI)) {
    LLVM_DEBUG(dbgs() << "I have been called?. One Time?");
    const int FrameIdx = I.getFrameIdx();
    const MachineFrameInfo &MFI = MF.getFrameInfo();
    MachineMemOperand *MMO = MF.getMachineMemOperand(
        MachinePointerInfo::getFixedStack(MF, FrameIdx),
        MachineMemOperand::MOStore, MFI.getObjectSize(FrameIdx), Align(2));

    LLVM_DEBUG(dbgs() << "reg is: " << I.getReg().id()
                      << " is physical: " << I.getReg().isPhysical() << "\n");

    BuildMI(MBB, MI, DL, TII.get(CPEN211::STR16mr))
        .addFrameIndex(FrameIdx)
        .addImm(0) // FIXME: is this even right?
        .addReg(I.getReg())
        .addMemOperand(MMO);
  }

  return true;
}

bool CPEN211FrameLowering::restoreCalleeSavedRegisters(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator MI,
    MutableArrayRef<CalleeSavedInfo> CSI, const TargetRegisterInfo *TRI) const {

  if (CSI.empty())
    return false;

  DebugLoc DL;
  if (MI != MBB.end())
    DL = MI->getDebugLoc();

  MachineFunction &MF = *MBB.getParent();
  const TargetInstrInfo &TII = *MF.getSubtarget().getInstrInfo();

  // popping the stack in reverse order
  for (const CalleeSavedInfo &I : llvm::reverse(CSI)) {
    assert(!I.isSpilledToReg() && "cannot be spilled to reg!");
    const int FrameIdx = I.getFrameIdx();
    const MachineFrameInfo &MFI = MF.getFrameInfo();
    MachineMemOperand *MMO = MF.getMachineMemOperand(
        MachinePointerInfo::getFixedStack(MF, FrameIdx),
        MachineMemOperand::MOLoad, MFI.getObjectSize(FrameIdx), Align(2));

    BuildMI(MBB, MI, DL, TII.get(CPEN211::LDR16rm), I.getReg())
        .addFrameIndex(FrameIdx)
        .addImm(0) // FIXME: is this even right?
        .addReg(I.getReg())
        .addMemOperand(MMO);
  }

  return true;
}

MachineBasicBlock::iterator CPEN211FrameLowering::eliminateCallFramePseudoInstr(
    MachineFunction &MF, MachineBasicBlock &MBB,
    MachineBasicBlock::iterator I) const {
  // If the stack pointer can be changed after prologue, turn the
  // adjcallstackup instruction into a 'sub SP, <amt>' and the
  // adjcallstackdown instruction into 'add SP, <amt>'
  MachineInstr &Old = *I;
  uint64_t Amount = TII.getFrameSize(Old) / 2;
  if (Amount != 0) {
    // We need to keep the stack aligned properly.  To do this, we round the
    // amount of space needed for the outgoing arguments up to the next
    // alignment boundary.
    assert(getStackAlign() == 2);
    Amount = alignTo(Amount, getStackAlign());

    MachineInstr *New = nullptr;
    if (Old.getOpcode() == TII.getCallFrameSetupOpcode()) {
      New =
          BuildMI(MF, Old.getDebugLoc(), TII.get(CPEN211::SUB16ri), CPEN211::SP)
              .addReg(CPEN211::SP)
              .addImm(Amount);
    } else {
      assert(Old.getOpcode() == TII.getCallFrameDestroyOpcode());
      // factor out the amount the callee already popped.
      Amount -= TII.getFramePoppedByCallee(Old);
      if (Amount)
        New = BuildMI(MF, Old.getDebugLoc(), TII.get(CPEN211::ADD16rr),
                      CPEN211::SP)
                  .addReg(CPEN211::SP)
                  .addImm(Amount);
    }

    if (New) {
      // The SRW implicit def is dead.
      New->getOperand(3).setIsDead();

      // Replace the pseudo instruction with a new instruction...
      MBB.insert(I, New);
    }
  }

  MBB.dump();
  return MBB.erase(I);
}

// do we even need this function call?
void CPEN211FrameLowering::processFunctionBeforeFrameFinalized(
    MachineFunction &MF, RegScavenger *) const {
  // llvm_unreachable("this is a impossible place to reach!!!");
  // // Create a frame entry for the FP register that must be saved.
  // if (hasFP(MF)) {
  //   int FrameIdx = MF.getFrameInfo().CreateFixedObject(2, -4, true);
  //   (void)FrameIdx;
  //   assert(FrameIdx == MF.getFrameInfo().getObjectIndexBegin() &&
  //          "Slot for FP register must be last in order to be found!");
  // }
}

#undef DEBUG_TYPE
