//===-- CPEN211MachineFunctionInfo.cpp - MSP430 machine function info
//------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "CPEN211MachineFunctionInfo.h"

using namespace llvm;

void CPEN211MachineFunctionInfo::anchor() {}

MachineFunctionInfo *CPEN211MachineFunctionInfo::clone(
    BumpPtrAllocator &Allocator, MachineFunction &DestMF,
    const DenseMap<MachineBasicBlock *, MachineBasicBlock *> &Src2DstMBB)
    const {
  return DestMF.cloneInfo<CPEN211MachineFunctionInfo>(*this);
}
