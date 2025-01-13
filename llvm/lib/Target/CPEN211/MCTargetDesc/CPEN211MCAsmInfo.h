//===-- CPEN211MCAsmInfo.h - MSP430 asm properties --------------*- C++
//-*--===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the CPEN211MCAsmInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_CPEN211_MCTARGETDESC_MSP430MCASMINFO_H
#define LLVM_LIB_TARGET_CPEN211_MCTARGETDESC_MSP430MCASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {
class Triple;

class CPEN211MCAsmInfo : public MCAsmInfoELF {
  void anchor() override;

public:
  explicit CPEN211MCAsmInfo(const Triple &TT);
};

} // namespace llvm

#endif
