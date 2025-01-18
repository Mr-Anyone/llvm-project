//===-- CPEN211MCTargetDesc.cpp - MSP430 Target Descriptions
//---------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file provides CPEN211 specific target descriptions.
//
//===----------------------------------------------------------------------===//

#include "CPEN211MCTargetDesc.h"
#include "CPEN211InstPrinter.h"
#include "CPEN211MCAsmInfo.h"
#include "TargetInfo/CPEN211TargetInfo.h"
#include "llvm/MC/MCDwarf.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

#define GET_INSTRINFO_MC_DESC
#define ENABLE_INSTR_PREDICATE_VERIFIER
#include "CPEN211GenInstrInfo.inc"

#define GET_SUBTARGETINFO_MC_DESC
#include "CPEN211GenSubtargetInfo.inc"

#define GET_REGINFO_MC_DESC
#include "CPEN211GenRegisterInfo.inc"

// static MCInstrInfo *createCPEN211MCInstrInfo() {
//   MCInstrInfo *X = new MCInstrInfo();
//   InitCPEN211MCInstrInfo(X);
//   return X;
// }
//
// static MCRegisterInfo *createCPEN211MCRegisterInfo(const Triple &TT) {
//   MCRegisterInfo *X = new MCRegisterInfo();
//   InitCPEN211MCRegisterInfo(X, CPEN211::PC);
//   return X;
// }
//
// static MCAsmInfo *createCPEN211MCAsmInfo(const MCRegisterInfo &MRI,
//                                          const Triple &TT,
//                                          const MCTargetOptions &Options) {
//   MCAsmInfo *MAI = new CPEN211MCAsmInfo(TT);
//
//   // Initialize initial frame state.
//   int stackGrowth = -2;
//
//   // Initial state of the frame pointer is sp+ptr_size.
//   MCCFIInstruction Inst = MCCFIInstruction::cfiDefCfa(
//       nullptr, MRI.getDwarfRegNum(CPEN211::SP, true), -stackGrowth);
//   MAI->addInitialFrameState(Inst);
//
//   // Add return address to move list
//   MCCFIInstruction Inst2 = MCCFIInstruction::createOffset(
//       nullptr, MRI.getDwarfRegNum(CPEN211::PC, true), stackGrowth);
//   MAI->addInitialFrameState(Inst2);
//
//   return MAI;
// }
//
// static MCSubtargetInfo *
// createCPEN211MCSubtargetInfo(const Triple &TT, StringRef CPU, StringRef FS) {
//   return createCPEN211MCSubtargetInfoImpl(TT, CPU, /*TuneCPU*/ CPU, FS);
// }
//
// static MCInstPrinter *createCPEN211MCInstPrinter(const Triple &T,
//                                                  unsigned SyntaxVariant,
//                                                  const MCAsmInfo &MAI,
//                                                  const MCInstrInfo &MII,
//                                                  const MCRegisterInfo &MRI) {
//   if (SyntaxVariant == 0)
//     return new CPEN211InstPrinter(MAI, MII, MRI);
//   return nullptr;
// }

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeCPEN211TargetMC() {
  Target &T = getTheCPEN211Target();
  // llvm_unreachable("to do: what does the thing below do?");

  // TargetRegistry::RegisterMCAsmInfo(T, createCPEN211MCAsmInfo);
  // TargetRegistry::RegisterMCInstrInfo(T, createCPEN211MCInstrInfo);
  // TargetRegistry::RegisterMCRegInfo(T, createCPEN211MCRegisterInfo);
  // TargetRegistry::RegisterMCSubtargetInfo(T, createCPEN211MCSubtargetInfo);
  // TargetRegistry::RegisterMCInstPrinter(T, createCPEN211MCInstPrinter);
  // TargetRegistry::RegisterMCCodeEmitter(T, createCPEN211MCCodeEmitter);
  // TargetRegistry::RegisterMCAsmBackend(T, createCPEN211MCAsmBackend);
  // TargetRegistry::RegisterObjectTargetStreamer(
  //     T, createCPEN211ObjectTargetStreamer);
}
