//==-- CPEN211.h - Top-level interface for MSP430 representation --*- C++
//-*-==//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in
// the LLVM CPEN211 backend.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_CPEN211_MSP430_H
#define LLVM_LIB_TARGET_CPEN211_MSP430_H

#include "MCTargetDesc/CPEN211MCTargetDesc.h"
#include "llvm/Target/TargetMachine.h"

namespace CPEN211CC {
// CPEN211 specific condition code.
enum CondCodes {
    TODO_PUT_SOMETHING_HERE,

  //COND_INVALID = -1
};
} // namespace CPEN211CC

namespace llvm {
class FunctionPass;
class CPEN211TargetMachine;
class PassRegistry;

FunctionPass *createCPEN211ISelDag(CPEN211TargetMachine &TM,
                                   CodeGenOptLevel OptLevel);

FunctionPass *createCPEN211BranchSelectionPass();

void initializeCPEN211DAGToDAGISelLegacyPass(PassRegistry &);

} // namespace llvm

#endif
