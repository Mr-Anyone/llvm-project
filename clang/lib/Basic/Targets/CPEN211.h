//===--- CPEN211.h - Declare CPEN211 target feature support -------*- C++
//-*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file declares CPEN211 TargetInfo objects.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_CLANG_LIB_BASIC_TARGETS_CPEN211_H
#define LLVM_CLANG_LIB_BASIC_TARGETS_CPEN211_H

#include "clang/Basic/TargetInfo.h"
#include "clang/Basic/TargetOptions.h"
#include "llvm/Support/Compiler.h"
#include "llvm/TargetParser/Triple.h"

namespace clang {
namespace targets {

class LLVM_LIBRARY_VISIBILITY CPEN211TargetInfo : public TargetInfo {
  static const char *const GCCRegNames[];

public:
  CPEN211TargetInfo(const llvm::Triple &Triple, const TargetOptions &)
      : TargetInfo(Triple) {
    TLSSupported = false;
    IntWidth = 16;
    IntAlign = 16;
    // FIXME: (please fix this!)
    LongWidth = 16;
    LongLongWidth = 16;
    LongAlign = LongLongAlign = 16;
    // FIXME
    FloatWidth = 16;
    FloatAlign = 16;
    DoubleWidth = LongDoubleWidth = 16;
    DoubleAlign = LongDoubleAlign = 16;
    PointerWidth = 16;
    PointerAlign = 16;
    // FIXME
    SuitableAlign = 16;
    SizeType = UnsignedInt;
    IntMaxType = SignedLongLong;
    IntPtrType = SignedInt;
    PtrDiffType = SignedInt;
    SigAtomicType = SignedLong;

    resetDataLayout("e-p:16:16-i16:16");
    // resetDataLayout("e-m:e-p:16:16-i32:16-i64:16-f32:16-f64:16-a:8-n8:16-S16");
  }

  void getTargetDefines(const LangOptions &Opts,
                        MacroBuilder &Builder) const override;

  ArrayRef<Builtin::Info> getTargetBuiltins() const override {
    // FIXME (for Vincent)
    return {};
  }

  bool allowsLargerPreferedTypeAlignment() const override { return false; }

  bool hasFeature(StringRef Feature) const override {
    return Feature == "msp430";
  }

  ArrayRef<const char *> getGCCRegNames() const override;

  ArrayRef<TargetInfo::GCCRegAlias> getGCCRegAliases() const override {
    // Make r0 - r3 be recognized by llc (f.e., in clobber list)
    static const TargetInfo::GCCRegAlias GCCRegAliases[] = {
        {{"r0"}, "pc"},
        {{"r1"}, "sp"},
        {{"r2"}, "sr"},
        {{"r3"}, "cg"},
    };
    return llvm::ArrayRef(GCCRegAliases);
  }

  bool validateAsmConstraint(const char *&Name,
                             TargetInfo::ConstraintInfo &info) const override {
    // FIXME: implement
    // switch (*Name) {
    // case 'K': // the constant 1
    // case 'L': // constant -1^20 .. 1^19
    // case 'M': // constant 1-4:
    //   return true;
    // }
    // No target constraints for now.
    return false;
  }

  std::string_view getClobbers() const override {
    // FIXME: Is this really right?
    return "";
  }

  BuiltinVaListKind getBuiltinVaListKind() const override {
    // FIXME: implement
    return TargetInfo::CharPtrBuiltinVaList;
  }
};

} // namespace targets
} // namespace clang
#endif // LLVM_CLANG_LIB_BASIC_TARGETS_CPEN211_H
