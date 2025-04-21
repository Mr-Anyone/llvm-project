
//===--- CPEN211.cpp - Implement MSP430 target feature support
//-------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements CPEN211 TargetInfo objects.
//
//===----------------------------------------------------------------------===//

#include "CPEN211.h"
#include "clang/Basic/MacroBuilder.h"

using namespace clang;
using namespace clang::targets;

const char *const CPEN211TargetInfo::GCCRegNames[] = {"R0", "R1", "R2", "R3",
                                                      "R4", "R5", "R6", "R7"};

ArrayRef<const char *> CPEN211TargetInfo::getGCCRegNames() const {
  return llvm::ArrayRef(GCCRegNames);
}

void CPEN211TargetInfo::getTargetDefines(const LangOptions &Opts,
                                         MacroBuilder &Builder) const {
  Builder.defineMacro("cpen211");
  Builder.defineMacro("__cpen211__");
  // FIXME: defines for different 'flavours' of MCU
}
