//===-- CPEN211TargetMachine.h - Define TargetMachine for MSP430 -*- C++
//-*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares the CPEN211 specific subclass of TargetMachine.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_CPEN211_MSP430TARGETMACHINE_H
#define LLVM_LIB_TARGET_CPEN211_MSP430TARGETMACHINE_H

#include "CPEN211Subtarget.h"
#include "llvm/CodeGen/CodeGenTargetMachineImpl.h"
#include <optional>

namespace llvm {
class StringRef;

/// CPEN211TargetMachine
///
class CPEN211TargetMachine : public CodeGenTargetMachineImpl {
  std::unique_ptr<TargetLoweringObjectFile> TLOF;
  CPEN211Subtarget Subtarget;

public:
  CPEN211TargetMachine(const Target &T, const Triple &TT, StringRef CPU,
                       StringRef FS, const TargetOptions &Options,
                       std::optional<Reloc::Model> RM,
                       std::optional<CodeModel::Model> CM, CodeGenOptLevel OL,
                       bool JIT);
  ~CPEN211TargetMachine() override;

  const CPEN211Subtarget *getSubtargetImpl(const Function &F) const override {
    return &Subtarget;
  }
  TargetPassConfig *createPassConfig(PassManagerBase &PM) override;

  TargetLoweringObjectFile *getObjFileLowering() const override {
    return TLOF.get();
  }

  MachineFunctionInfo *
  createMachineFunctionInfo(BumpPtrAllocator &Allocator, const Function &F,
                            const TargetSubtargetInfo *STI) const override;
}; // CPEN211TargetMachine.

} // end namespace llvm

#endif
