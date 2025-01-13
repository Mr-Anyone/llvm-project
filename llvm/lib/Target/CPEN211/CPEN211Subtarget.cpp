//===-- CPEN211Subtarget.cpp - MSP430 Subtarget Information
//----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the CPEN211 specific subclass of TargetSubtargetInfo.
//
//===----------------------------------------------------------------------===//

#include "CPEN211Subtarget.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

#define DEBUG_TYPE "msp430-subtarget"

// static cl::opt<CPEN211Subtarget::HWMultEnum> HWMultModeOption(
//     "mhwmult", cl::Hidden, cl::desc("Hardware multiplier use mode for
//     CPEN211"), cl::init(CPEN211Subtarget::NoHWMult),
//     cl::values(clEnumValN(CPEN211Subtarget::NoHWMult, "none",
//                           "Do not use hardware multiplier"),
//                clEnumValN(CPEN211Subtarget::HWMult16, "16bit",
//                           "Use 16-bit hardware multiplier"),
//                clEnumValN(CPEN211Subtarget::HWMult32, "32bit",
//                           "Use 32-bit hardware multiplier"),
//                clEnumValN(CPEN211Subtarget::HWMultF5, "f5series",
//                           "Use F5 series hardware multiplier")));

#define GET_SUBTARGETINFO_TARGET_DESC
#define GET_SUBTARGETINFO_CTOR
#include "CPEN211GenSubtargetInfo.inc"

void CPEN211Subtarget::anchor() {}

CPEN211Subtarget &
CPEN211Subtarget::initializeSubtargetDependencies(StringRef CPU, StringRef FS) {
  ExtendedInsts = false;
  HWMultMode = NoHWMult;

  StringRef CPUName = CPU;
  if (CPUName.empty())
    CPUName = "cpen211";

  ParseSubtargetFeatures(CPUName, /*TuneCPU*/ CPUName, FS);

  HWMultMode = CPEN211Subtarget::NoHWMult;

  return *this;
}

CPEN211Subtarget::CPEN211Subtarget(const Triple &TT, const std::string &CPU,
                                   const std::string &FS,
                                   const TargetMachine &TM)
    : CPEN211GenSubtargetInfo(TT, CPU, /*TuneCPU*/ CPU, FS),
      InstrInfo(initializeSubtargetDependencies(CPU, FS)), TLInfo(TM, *this),
      FrameLowering(*this) {}
