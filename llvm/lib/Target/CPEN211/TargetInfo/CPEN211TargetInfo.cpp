//===-- CPEN211TargetInfo.cpp - MSP430 Target Implementation
//---------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "TargetInfo/CPEN211TargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
using namespace llvm;

Target &llvm::getTheCPEN211Target() {
  static Target TheCPEN211Target;
  return TheCPEN211Target;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeCPEN211TargetInfo() {
  RegisterTarget<Triple::msp430> X(getTheCPEN211Target(), "cpen211",
                                   "CPEN211 [experimental]", "cpen211");
}
