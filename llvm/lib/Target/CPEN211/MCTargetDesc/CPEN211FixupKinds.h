//===-- CPEN211FixupKinds.h - CPEN211 Specific Fixup Entries ------*- C++
//-*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_CPEN211_MCTARGETDESC_CPEN211FIXUPKINDS_H
#define LLVM_LIB_TARGET_CPEN211_MCTARGETDESC_CPEN211FIXUPKINDS_H

#include "llvm/MC/MCFixup.h"

// #undefCPEN211

namespace llvm {
namespace CPEN211 {

// This table must be in the same order of
// MCFixupKindInfo Infos[CPEN211::NumTargetFixupKinds]
// in CPEN211AsmBackend.cpp.
//
enum Fixups {
  // A 32 bit absolute fixup.
  fixup_32 = FirstTargetFixupKind,
  // A 10 bit PC relative fixup.
  fixup_10_pcrel,
  // A 16 bit absolute fixup.
  fixup_16,
  // A 16 bit PC relative fixup.
  fixup_16_pcrel,
  // A 16 bit absolute fixup for byte operations.
  fixup_16_byte,
  // A 16 bit PC relative fixup for command address.
  fixup_16_pcrel_byte,
  // A 10 bit PC relative fixup for complicated polymorphs.
  fixup_2x_pcrel,
  // A 16 bit relaxable fixup.
  fixup_rl_pcrel,
  // A 8 bit absolute fixup.
  fixup_8,
  // A 32 bit symbol difference fixup.
  fixup_sym_diff,

  // Marker
  LastTargetFixupKind,
  NumTargetFixupKinds = LastTargetFixupKind - FirstTargetFixupKind
};
} // namespace CPEN211
} // end namespace llvm

#endif
