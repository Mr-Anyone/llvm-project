//===-- CPEN211AsmBackend.cpp - MSP430 Assembler Backend
//-------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/CPEN211FixupKinds.h"
#include "MCTargetDesc/CPEN211MCTargetDesc.h"
#include "llvm/ADT/APInt.h"
#include "llvm/MC/MCAsmBackend.h"
#include "llvm/MC/MCAssembler.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCFixupKindInfo.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/MC/MCTargetOptions.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/raw_ostream.h"
#include <bitset>
#include <iostream>

using namespace llvm;

#define DEBUG_TYPE "cpen211-asmbackend"

namespace {
class CPEN211AsmBackend : public MCAsmBackend {
  uint8_t OSABI;

  uint64_t adjustFixupValue(const MCFixup &Fixup, uint64_t Value,
                            MCContext &Ctx) const;

public:
  CPEN211AsmBackend(const MCSubtargetInfo &STI, uint8_t OSABI)
      : MCAsmBackend(llvm::endianness::little), OSABI(OSABI) {}
  ~CPEN211AsmBackend() override = default;

  void applyFixup(const MCAssembler &Asm, const MCFixup &Fixup,
                  const MCValue &Target, MutableArrayRef<char> Data,
                  uint64_t Value, bool IsResolved,
                  const MCSubtargetInfo *STI) const override;

  std::unique_ptr<MCObjectTargetWriter>
  createObjectTargetWriter() const override {
    return createCPEN211ELFObjectWriter(OSABI);
  }

  bool fixupNeedsRelaxationAdvanced(const MCAssembler &Asm,
                                    const MCFixup &Fixup, bool Resolved,
                                    uint64_t Value,
                                    const MCRelaxableFragment *DF,
                                    const bool WasForced) const override {
    return false;
  }

  unsigned getNumFixupKinds() const override {
    return CPEN211::NumTargetFixupKinds;
  }

  const MCFixupKindInfo &getFixupKindInfo(MCFixupKind Kind) const override {
    // llvm_unreachable("gg");
    // assert(false);
    const static MCFixupKindInfo Infos[CPEN211::NumTargetFixupKinds] = {
        // This table must be in the same order of enum in
        // name            offset bits flags
        {"fixup_8", 0, 8, MCFixupKindInfo::FKF_IsPCRel}, // fixup_8
    };

    static_assert((std::size(Infos)) == CPEN211::NumTargetFixupKinds,
                  "Not all fixup kinds added to Infos array");

    if (Kind < FirstTargetFixupKind)
      return MCAsmBackend::getFixupKindInfo(Kind);

    return Infos[Kind - FirstTargetFixupKind];
  }

  bool writeNopData(raw_ostream &OS, uint64_t Count,
                    const MCSubtargetInfo *STI) const override;
};

uint64_t CPEN211AsmBackend::adjustFixupValue(const MCFixup &Fixup,
                                             uint64_t Value,
                                             MCContext &Ctx) const {
  unsigned Kind = Fixup.getKind();
  switch (Kind) {
  case CPEN211::fixup_8: {
    assert(Value % 2 == 0 && "Cannot have bl intermediate being at odd offset");
    int32_t NewValue = (static_cast<int32_t>(Value) - 2) / 2;
    assert(NewValue >= -128 && NewValue <= 127 &&
           "imm8 must be in between -128 and 127");
    return NewValue;
  }
  default:
    llvm_unreachable("not sure what this is?");
    return Value;
  }
}

void CPEN211AsmBackend::applyFixup(const MCAssembler &Asm, const MCFixup &Fixup,
                                   const MCValue &Target,
                                   MutableArrayRef<char> Data, uint64_t Value,
                                   bool IsResolved,
                                   const MCSubtargetInfo *STI) const {
  assert(IsResolved && "Fixup must be resolved as of current! We don't support "
                       "external symbols");
  Value = adjustFixupValue(Fixup, Value, Asm.getContext());
  MCFixupKindInfo Info = getFixupKindInfo(Fixup.getKind());
  LLVM_DEBUG(dbgs() << "Applying Fixup! Value: "
                    << static_cast<signed int>(Value)
                    << "\tData Size: " << Data.size()
                    << "\tOffset is: " << Fixup.getOffset() << "\n");

  // Shift the value into position.
  // FIXME: this is not correct for some other allocation record
  unsigned NumBytes = alignTo(Info.TargetSize + Info.TargetOffset, 8) / 8;
  assert(Info.TargetOffset == 0);
  assert(NumBytes == 1 && "Offset must be one byte at most!");

  Data[Fixup.getOffset()] |= Value;
}

bool CPEN211AsmBackend::writeNopData(raw_ostream &OS, uint64_t Count,
                                     const MCSubtargetInfo *STI) const {
  // llvm_unreachable("implement this");
  if ((Count % 2) != 0)
    return false;

  uint64_t NopCount = Count / 2;
  while (NopCount--)
    OS.write("\x0\x0", 2);

  return true;
}

} // end anonymous namespace

MCAsmBackend *llvm::createCPEN211MCAsmBackend(const Target &T,
                                              const MCSubtargetInfo &STI,
                                              const MCRegisterInfo &MRI,
                                              const MCTargetOptions &Options) {
  return new CPEN211AsmBackend(STI, ELF::ELFOSABI_STANDALONE);
}

#undef DEBUG_TYPE
