//===- CPEN211.cpp
//---------------------------------------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "Symbols.h"
#include "Target.h"
#include "lld/Common/ErrorHandler.h"
#include "llvm/BinaryFormat/ELF.h"
#include "llvm/Support/Endian.h"

using namespace llvm;
using namespace llvm::object;
using namespace llvm::support::endian;
using namespace llvm::ELF;
using namespace lld;
using namespace lld::elf;

namespace {
class CPEN211 final : public TargetInfo {
public:
  CPEN211(Ctx &);
  RelExpr getRelExpr(RelType type, const Symbol &s,
                     const uint8_t *loc) const override;
  void relocate(uint8_t *loc, const Relocation &rel,
                uint64_t val) const override;
};
} // namespace

CPEN211::CPEN211(Ctx &ctx) : TargetInfo(ctx) {}

RelExpr CPEN211::getRelExpr(RelType type, const Symbol &s,
                            const uint8_t *loc) const {
  switch (type) {
  case R_CPEN211_PCREL8:
    return R_PC;
  default:
    llvm_unreachable("there are not absolute addressing");
    return R_ABS;
  }
}

void CPEN211::relocate(uint8_t *loc, const Relocation &rel,
                       uint64_t val) const {
  switch (rel.type) {
  case R_CPEN211_PCREL8: {
    if (val % 2 != 0)
      Err(ctx) << "all address must be divisible by two. This is likely a LLVM "
                  "MC Layer bug!";

    int64_t write_val = ((int64_t)(val)-2) / 2;
    checkInt(ctx, loc, write_val, 8, rel);
    write16le(loc, (0xff00 & (read16le(loc))) | (((int16_t)write_val) & 0xff));
    break;
  }
  default:
    Err(ctx) << getErrorLoc(ctx, loc) << "unrecognized relocation " << rel.type;
  }
}
void elf::setCPEN211TargetInfo(Ctx &ctx) { ctx.target.reset(new CPEN211(ctx)); }
