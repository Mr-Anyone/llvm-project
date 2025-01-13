//===-- CPEN211ELFObjectWriter.cpp - MSP430 ELF Writer
//---------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "MCTargetDesc/CPEN211FixupKinds.h"
#include "MCTargetDesc/CPEN211MCTargetDesc.h"

#include "llvm/MC/MCELFObjectWriter.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCObjectWriter.h"
#include "llvm/MC/MCValue.h"
#include "llvm/Support/ErrorHandling.h"

using namespace llvm;

namespace {
// TODO: THIS ENTIRE CLASS IS WRONG
// TODO (for Vincent): this is some stuff
class CPEN211ELFObjectWriter : public MCELFObjectTargetWriter {
public:
  CPEN211ELFObjectWriter(uint8_t OSABI)
      // TODO (for Vincent): this is entirely wrong
      : MCELFObjectTargetWriter(false, OSABI, ELF::EM_MSP430,
                                /*HasRelocationAddend*/ true) {}

  ~CPEN211ELFObjectWriter() override = default;

protected:
  unsigned getRelocType(MCContext &Ctx, const MCValue &Target,
                        const MCFixup &Fixup, bool IsPCRel) const override {
    // Translate fixup kind to ELF relocation type.
    switch (Fixup.getTargetKind()) {
    default:
      llvm_unreachable("this is not yet implemented!");
      // case FK_Data_1:
      //   return ELF::R_CPEN211_8;
      // case FK_Data_2:
      //   return ELF::R_CPEN211_16_BYTE;
      // case FK_Data_4:
      //   return ELF::R_CPEN211_32;
      // case CPEN211::fixup_32:
      //   return ELF::R_MSP430_32;
      // case CPEN211::fixup_10_pcrel:
      //   return ELF::R_MSP430_10_PCREL;
      // case CPEN211::fixup_16:
      //   return ELF::R_MSP430_16;
      // case CPEN211::fixup_16_pcrel:
      //   return ELF::R_MSP430_16_PCREL;
      // case CPEN211::fixup_16_byte:
      //   return ELF::R_MSP430_16_BYTE;
      // case CPEN211::fixup_16_pcrel_byte:
      //   return ELF::R_MSP430_16_PCREL_BYTE;
      // case CPEN211::fixup_2x_pcrel:
      //   return ELF::R_MSP430_2X_PCREL;
      // case CPEN211::fixup_rl_pcrel:
      //   return ELF::R_MSP430_RL_PCREL;
      // case CPEN211::fixup_8:
      //   return ELF::R_MSP430_8;
      // case CPEN211::fixup_sym_diff:
      //   return ELF::R_MSP430_SYM_DIFF;
      // default:
      //   llvm_unreachable("Invalid fixup kind");
      // }
    }
  }
};
} // end of anonymous namespace

std::unique_ptr<MCObjectTargetWriter>
llvm::createCPEN211ELFObjectWriter(uint8_t OSABI) {
  return std::make_unique<CPEN211ELFObjectWriter>(OSABI);
}
