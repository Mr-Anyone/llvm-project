//===-- CPEN211TargetMachine.cpp - Define TargetMachine for CPEN211
//---------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Top-level implementation for the CPEN211 target.
//
//===----------------------------------------------------------------------===//

#include "CPEN211TargetMachine.h"
#include "CPEN211.h"
#include "CPEN211MachineFunctionInfo.h"
#include "TargetInfo/CPEN211TargetInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/MC/TargetRegistry.h"
#include <optional>
using namespace llvm;

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeCPEN211Target() {
  // Register the target.
  RegisterTargetMachine<CPEN211TargetMachine> X(getTheCPEN211Target());
  PassRegistry &PR = *PassRegistry::getPassRegistry();
  initializeCPEN211DAGToDAGISelLegacyPass(PR);
}

static Reloc::Model getEffectiveRelocModel(std::optional<Reloc::Model> RM) {
  return RM.value_or(Reloc::Static);
}

static std::string computeDataLayout(const Triple &TT, StringRef CPU,
                                     const TargetOptions &Options) {
  return "e-m:e-p:16:16-i32:16-i64:16-f32:16-f64:16-a:8-n8:16-S16";
}

CPEN211TargetMachine::CPEN211TargetMachine(const Target &T, const Triple &TT,
                                           StringRef CPU, StringRef FS,
                                           const TargetOptions &Options,
                                           std::optional<Reloc::Model> RM,
                                           std::optional<CodeModel::Model> CM,
                                           CodeGenOptLevel OL, bool JIT)
    : CodeGenTargetMachineImpl(T, computeDataLayout(TT, CPU, Options), TT, CPU,
                               FS, Options, getEffectiveRelocModel(RM),
                               getEffectiveCodeModel(CM, CodeModel::Small), OL),
      TLOF(std::make_unique<TargetLoweringObjectFileELF>()),
      Subtarget(TT, std::string(CPU), std::string(FS), *this) {
  initAsmInfo();
}

CPEN211TargetMachine::~CPEN211TargetMachine() = default;

namespace {
/// CPEN211 Code Generator Pass Configuration Options.
class CPEN211PassConfig : public TargetPassConfig {
public:
  CPEN211PassConfig(CPEN211TargetMachine &TM, PassManagerBase &PM)
      : TargetPassConfig(TM, PM) {}

  CPEN211TargetMachine &getCPEN211TargetMachine() const {
    return getTM<CPEN211TargetMachine>();
  }

  void addIRPasses() override;
  bool addInstSelector() override;
  void addPreEmitPass() override;
};
} // namespace

TargetPassConfig *CPEN211TargetMachine::createPassConfig(PassManagerBase &PM) {
  return new CPEN211PassConfig(*this, PM);
}

MachineFunctionInfo *CPEN211TargetMachine::createMachineFunctionInfo(
    BumpPtrAllocator &Allocator, const Function &F,
    const TargetSubtargetInfo *STI) const {
  return CPEN211MachineFunctionInfo::create<CPEN211MachineFunctionInfo>(
      Allocator, F, STI);
}

void CPEN211PassConfig::addIRPasses() {
  addPass(createAtomicExpandLegacyPass());

  TargetPassConfig::addIRPasses();
}

bool CPEN211PassConfig::addInstSelector() {
  // Install an instruction selector.
  addPass(createCPEN211ISelDag(getCPEN211TargetMachine(), getOptLevel()));
  return false;
}

void CPEN211PassConfig::addPreEmitPass() {
  // Must run branch selection immediately preceding the asm printer.
  addPass(createCPEN211BranchSelectionPass());
}
