//===-- CPEN211InstrInfo.cpp - CPEN211 Instruction Information
//--------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the CPEN211 implementation of the TargetInstrInfo class.
//
//===----------------------------------------------------------------------===//

#include "CPEN211InstrInfo.h"
#include "CPEN211.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

#define GET_INSTRINFO_CTOR_DTOR
#include "CPEN211GenInstrInfo.inc"

// Pin the vtable to this file.
void CPEN211InstrInfo::anchor() {}

CPEN211InstrInfo::CPEN211InstrInfo(CPEN211Subtarget &STI)
    // : CPEN211GenInstrInfo(CPEN211::ADJCALLSTACKDOWN,
    // CPEN211::ADJCALLSTACKUP),
    : RI() {
  llvm_unreachable("This is not yet implemented!");
}

void CPEN211InstrInfo::storeRegToStackSlot(
    MachineBasicBlock &MBB, MachineBasicBlock::iterator MI, Register SrcReg,
    bool isKill, int FrameIdx, const TargetRegisterClass *RC,
    const TargetRegisterInfo *TRI, Register VReg) const {
  // DebugLoc DL;
  // if (MI != MBB.end())
  //   DL = MI->getDebugLoc();
  // MachineFunction &MF = *MBB.getParent();
  // MachineFrameInfo &MFI = MF.getFrameInfo();

  // MachineMemOperand *MMO = MF.getMachineMemOperand(
  //     MachinePointerInfo::getFixedStack(MF, FrameIdx),
  //     MachineMemOperand::MOStore, MFI.getObjectSize(FrameIdx),
  //     MFI.getObjectAlign(FrameIdx));

  // if (RC == &CPEN211::GR16RegClass)
  //   BuildMI(MBB, MI, DL, get(CPEN211::MOV16mr))
  //       .addFrameIndex(FrameIdx)
  //       .addImm(0)
  //       .addReg(SrcReg, getKillRegState(isKill))
  //       .addMemOperand(MMO);
  // else if (RC == &CPEN211::GR8RegClass)
  //   BuildMI(MBB, MI, DL, get(CPEN211::MOV8mr))
  //       .addFrameIndex(FrameIdx)
  //       .addImm(0)
  //       .addReg(SrcReg, getKillRegState(isKill))
  //       .addMemOperand(MMO);
  // else
  // llvm_unreachable("Cannot store this register to stack slot!");
  llvm_unreachable("not yet implemented");
}

void CPEN211InstrInfo::loadRegFromStackSlot(MachineBasicBlock &MBB,
                                            MachineBasicBlock::iterator MI,
                                            Register DestReg, int FrameIdx,
                                            const TargetRegisterClass *RC,
                                            const TargetRegisterInfo *TRI,
                                            Register VReg) const {

  llvm_unreachable("Not yet implemented!");
  // DebugLoc DL;
  // if (MI != MBB.end())
  //   DL = MI->getDebugLoc();
  // MachineFunction &MF = *MBB.getParent();
  // MachineFrameInfo &MFI = MF.getFrameInfo();

  // MachineMemOperand *MMO = MF.getMachineMemOperand(
  //     MachinePointerInfo::getFixedStack(MF, FrameIdx),
  //     MachineMemOperand::MOLoad, MFI.getObjectSize(FrameIdx),
  //     MFI.getObjectAlign(FrameIdx));

  // if (RC == &CPEN211::GR16RegClass)
  //   BuildMI(MBB, MI, DL, get(CPEN211::MOV16rm))
  //       .addReg(DestReg, getDefRegState(true))
  //       .addFrameIndex(FrameIdx)
  //       .addImm(0)
  //       .addMemOperand(MMO);
  // else if (RC == &CPEN211::GR8RegClass)
  //   BuildMI(MBB, MI, DL, get(CPEN211::MOV8rm))
  //       .addReg(DestReg, getDefRegState(true))
  //       .addFrameIndex(FrameIdx)
  //       .addImm(0)
  //       .addMemOperand(MMO);
  // else
  //   llvm_unreachable("Cannot store this register to stack slot!");
}

void CPEN211InstrInfo::copyPhysReg(MachineBasicBlock &MBB,
                                   MachineBasicBlock::iterator I,
                                   const DebugLoc &DL, MCRegister DestReg,
                                   MCRegister SrcReg, bool KillSrc,
                                   bool RenamableDest,
                                   bool RenamableSrc) const {

  llvm_unreachable("Not yet implemented");
  // unsigned Opc;
  // if (CPEN211::GR16RegClass.contains(DestReg, SrcReg))
  //   Opc = CPEN211::MOV16rr;
  // else if (CPEN211::GR8RegClass.contains(DestReg, SrcReg))
  //   Opc = CPEN211::MOV8rr;
  // else
  //   llvm_unreachable("Impossible reg-to-reg copy");

  // BuildMI(MBB, I, DL, get(Opc), DestReg)
  //     .addReg(SrcReg, getKillRegState(KillSrc));
}

unsigned CPEN211InstrInfo::removeBranch(MachineBasicBlock &MBB,
                                        int *BytesRemoved) const {
  llvm_unreachable("not yet implemented");
  // assert(!BytesRemoved && "code size not handled");

  // MachineBasicBlock::iterator I = MBB.end();
  // unsigned Count = 0;

  // while (I != MBB.begin()) {
  //   --I;
  //   if (I->isDebugInstr())
  //     continue;
  //   if (I->getOpcode() != CPEN211::JMP && I->getOpcode() != CPEN211::JCC &&
  //       I->getOpcode() != CPEN211::Bi && I->getOpcode() != CPEN211::Br &&
  //       I->getOpcode() != CPEN211::Bm)
  //     break;
  //   // Remove the branch.
  //   I->eraseFromParent();
  //   I = MBB.end();
  //   ++Count;
  // }

  // return Count;
}

bool CPEN211InstrInfo::reverseBranchCondition(
    SmallVectorImpl<MachineOperand> &Cond) const {
  assert(Cond.size() == 1 && "Invalid Xbranch condition!");

  llvm_unreachable("Not yet implemented");
  return false;

  // CPEN211CC::CondCodes CC =
  // static_cast<CPEN211CC::CondCodes>(Cond[0].getImm());

  // switch (CC) {
  // default:
  // llvm_unreachable("Invalid branch condition!");
  // case CPEN211CC::COND_E:
  //   CC = CPEN211CC::COND_NE;
  //   break;
  // case CPEN211CC::COND_NE:
  //   CC = CPEN211CC::COND_E;
  //   break;
  // case CPEN211CC::COND_L:
  //   CC = CPEN211CC::COND_GE;
  //   break;
  // case CPEN211CC::COND_GE:
  //   CC = CPEN211CC::COND_L;
  //   break;
  // case CPEN211CC::COND_HS:
  //   CC = CPEN211CC::COND_LO;
  //   break;
  // case CPEN211CC::COND_LO:
  //   CC = CPEN211CC::COND_HS;
  //   break;
  //}

  // Cond[0].setImm(CC);
  // return false;
}

bool CPEN211InstrInfo::analyzeBranch(MachineBasicBlock &MBB,
                                     MachineBasicBlock *&TBB,
                                     MachineBasicBlock *&FBB,
                                     SmallVectorImpl<MachineOperand> &Cond,
                                     bool AllowModify) const {
  llvm_unreachable("not implemented");
  // Start from the bottom of the block and work up, examining the
  // terminator instructions.
  // MachineBasicBlock::iterator I = MBB.end();
  // while (I != MBB.begin()) {
  //   --I;
  //   if (I->isDebugInstr())
  //     continue;

  //   // Working from the bottom, when we see a non-terminator
  //   // instruction, we're done.
  //   if (!isUnpredicatedTerminator(*I))
  //     break;

  //   // A terminator that isn't a branch can't easily be handled
  //   // by this analysis.
  //   if (!I->isBranch())
  //     return true;

  //   // Cannot handle indirect branches.
  //   if (I->getOpcode() == CPEN211::Br || I->getOpcode() == CPEN211::Bm)
  //     return true;

  //   // Handle unconditional branches.
  //   if (I->getOpcode() == CPEN211::JMP || I->getOpcode() == CPEN211::Bi) {
  //     if (!AllowModify) {
  //       TBB = I->getOperand(0).getMBB();
  //       continue;
  //     }

  //     // If the block has any instructions after a JMP, delete them.
  //     MBB.erase(std::next(I), MBB.end());
  //     Cond.clear();
  //     FBB = nullptr;

  //     // Delete the JMP if it's equivalent to a fall-through.
  //     if (MBB.isLayoutSuccessor(I->getOperand(0).getMBB())) {
  //       TBB = nullptr;
  //       I->eraseFromParent();
  //       I = MBB.end();
  //       continue;
  //     }

  //     // TBB is used to indicate the unconditinal destination.
  //     TBB = I->getOperand(0).getMBB();
  //     continue;
  //   }

  //   // Handle conditional branches.
  //   assert(I->getOpcode() == CPEN211::JCC && "Invalid conditional branch");
  //   CPEN211CC::CondCodes BranchCode =
  //       static_cast<CPEN211CC::CondCodes>(I->getOperand(1).getImm());
  //   if (BranchCode == CPEN211CC::COND_INVALID)
  //     return true; // Can't handle weird stuff.

  //   // Working from the bottom, handle the first conditional branch.
  //   if (Cond.empty()) {
  //     FBB = TBB;
  //     TBB = I->getOperand(0).getMBB();
  //     Cond.push_back(MachineOperand::CreateImm(BranchCode));
  //     continue;
  //   }

  //   // Handle subsequent conditional branches. Only handle the case where all
  //   // conditional branches branch to the same destination.
  //   assert(Cond.size() == 1);
  //   assert(TBB);

  //   // Only handle the case where all conditional branches branch to
  //   // the same destination.
  //   if (TBB != I->getOperand(0).getMBB())
  //     return true;

  //   CPEN211CC::CondCodes OldBranchCode =
  //   (CPEN211CC::CondCodes)Cond[0].getImm();
  //   // If the conditions are the same, we can leave them alone.
  //   if (OldBranchCode == BranchCode)
  //     continue;

  //   return true;
  // }

  // return false;
}

unsigned CPEN211InstrInfo::insertBranch(
    MachineBasicBlock &MBB, MachineBasicBlock *TBB, MachineBasicBlock *FBB,
    ArrayRef<MachineOperand> Cond, const DebugLoc &DL, int *BytesAdded) const {
  llvm_unreachable("not implemented");

  // Shouldn't be a fall through.
  // assert(TBB && "insertBranch must not be told to insert a fallthrough");
  // assert((Cond.size() == 1 || Cond.size() == 0) &&
  //        "CPEN211 branch conditions have one component!");
  // assert(!BytesAdded && "code size not handled");

  // if (Cond.empty()) {
  //   // Unconditional branch?
  //   assert(!FBB && "Unconditional branch with multiple successors!");
  //   BuildMI(&MBB, DL, get(CPEN211::JMP)).addMBB(TBB);
  //   return 1;
  // }

  // // Conditional branch.
  // unsigned Count = 0;
  // BuildMI(&MBB, DL, get(CPEN211::JCC)).addMBB(TBB).addImm(Cond[0].getImm());
  // ++Count;

  // if (FBB) {
  //   // Two-way Conditional branch. Insert the second branch.
  //   BuildMI(&MBB, DL, get(CPEN211::JMP)).addMBB(FBB);
  //   ++Count;
  // }
  // return Count;
}

/// GetInstSize - Return the number of bytes of code the specified
/// instruction may be.  This returns the maximum number of bytes.
///
unsigned CPEN211InstrInfo::getInstSizeInBytes(const MachineInstr &MI) const {
  const MCInstrDesc &Desc = MI.getDesc();

  llvm_unreachable("not yet implemented!");
  switch (Desc.getOpcode()) {
  case TargetOpcode::CFI_INSTRUCTION:
  case TargetOpcode::EH_LABEL:
  case TargetOpcode::IMPLICIT_DEF:
  case TargetOpcode::KILL:
  case TargetOpcode::DBG_VALUE:
    return 0;
  case TargetOpcode::INLINEASM:
  case TargetOpcode::INLINEASM_BR: {
    const MachineFunction *MF = MI.getParent()->getParent();
    const TargetInstrInfo &TII = *MF->getSubtarget().getInstrInfo();
    return TII.getInlineAsmLength(MI.getOperand(0).getSymbolName(),
                                  *MF->getTarget().getMCAsmInfo());
  }
  }

  return Desc.getSize();
}
