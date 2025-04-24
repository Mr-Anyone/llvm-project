//===-- CPEN211MCCodeEmitter.cpp - Convert MSP430 code to machine code
//-----===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the CPEN211MCCodeEmitter class.
//
//===----------------------------------------------------------------------===//

#include "CPEN211.h"
#include "MCTargetDesc/CPEN211FixupKinds.h"
#include "MCTargetDesc/CPEN211MCTargetDesc.h"

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/EndianStream.h"

#define DEBUG_TYPE "mccodeemitter"

namespace llvm {

class CPEN211MCCodeEmitter : public MCCodeEmitter {
  MCContext &Ctx;
  MCInstrInfo const &MCII;

  /// TableGen'erated function for getting the binary encoding for an
  /// instruction.
  uint64_t getBinaryCodeForInstr(const MCInst &MI,
                                 SmallVectorImpl<MCFixup> &Fixups,
                                 const MCSubtargetInfo &STI) const;

  /// Returns the binary encoding of operands.
  ///
  /// If an operand requires relocation, the relocation is recorded
  /// and zero is returned.
  unsigned getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                             SmallVectorImpl<MCFixup> &Fixups,
                             const MCSubtargetInfo &STI) const;

  unsigned getMemOpValue(const MCInst &MI, unsigned Op,
                         SmallVectorImpl<MCFixup> &Fixups,
                         const MCSubtargetInfo &STI) const;

  unsigned getPCRelImmOpValue(const MCInst &MI, unsigned Op,
                              SmallVectorImpl<MCFixup> &Fixups,
                              const MCSubtargetInfo &STI) const;

  unsigned getCGImmOpValue(const MCInst &MI, unsigned Op,
                           SmallVectorImpl<MCFixup> &Fixups,
                           const MCSubtargetInfo &STI) const;

  unsigned getCCOpValue(const MCInst &MI, unsigned Op,
                        SmallVectorImpl<MCFixup> &Fixups,
                        const MCSubtargetInfo &STI) const;

public:
  CPEN211MCCodeEmitter(MCContext &ctx, MCInstrInfo const &MCII)
      : Ctx(ctx), MCII(MCII) {}

  void encodeInstruction(const MCInst &MI, SmallVectorImpl<char> &CB,
                         SmallVectorImpl<MCFixup> &Fixups,
                         const MCSubtargetInfo &STI) const override;
};

void CPEN211MCCodeEmitter::encodeInstruction(const MCInst &MI,
                                             SmallVectorImpl<char> &CB,
                                             SmallVectorImpl<MCFixup> &Fixups,
                                             const MCSubtargetInfo &STI) const {
  const MCInstrDesc &Desc = MCII.get(MI.getOpcode());
  // Get byte count of instruction.
  unsigned Size = Desc.getSize();

  // Initialize fixup offset
  assert(Size = 2);
  uint64_t BinaryOpCode = getBinaryCodeForInstr(MI, Fixups, STI);
  size_t WordCount = Size / 2;

  while (WordCount--) {
    support::endian::write(CB, (uint16_t)BinaryOpCode,
                           llvm::endianness::little);
    BinaryOpCode >>= 16;
  }
}

unsigned
CPEN211MCCodeEmitter::getMachineOpValue(const MCInst &MI, const MCOperand &MO,
                                        SmallVectorImpl<MCFixup> &Fixups,
                                        const MCSubtargetInfo &STI) const {
  if (MO.isReg())
    return Ctx.getRegisterInfo()->getEncodingValue(MO.getReg());

  if (MO.isImm()) {
    return MO.getImm();
  }

  assert(MO.isExpr() && "Expected expr operand");
  return 0;
}

unsigned CPEN211MCCodeEmitter::getMemOpValue(const MCInst &MI, unsigned Op,
                                             SmallVectorImpl<MCFixup> &Fixups,
                                             const MCSubtargetInfo &STI) const {

  // assert(false && "think more about this later");
  const MCOperand &MO1 = MI.getOperand(Op);
  assert(MO1.isReg() && "Register operand expected");
  unsigned Reg = Ctx.getRegisterInfo()->getEncodingValue(MO1.getReg());

  return Reg;
  // const MCOperand &MO2 = MI.getOperand(Op + 1);
  // if (MO2.isImm()) {
  //   Offset += 2;
  //   return ((unsigned)MO2.getImm() << 4) | Reg;
  // }

  // assert(MO2.isExpr() && "Expr operand expected");
  // CPEN211::Fixups FixupKind;
  // switch (Reg) {
  // case 0:
  //   FixupKind = CPEN211::fixup_16_pcrel_byte;
  //   break;
  // case 2:
  //   FixupKind = CPEN211::fixup_16_byte;
  //   break;
  // default:
  //   FixupKind = CPEN211::fixup_16_byte;
  //   break;
  // }
  //// Fixups.push_back(MCFixup::create(
  ////     Offset, MO2.getExpr(), static_cast<MCFixupKind>(FixupKind),
  ////     MI.getLoc()));
  // Offset += 2;
  // return Reg;
}

unsigned
CPEN211MCCodeEmitter::getPCRelImmOpValue(const MCInst &MI, unsigned Op,
                                         SmallVectorImpl<MCFixup> &Fixups,
                                         const MCSubtargetInfo &STI) const {
  const MCOperand &MO = MI.getOperand(Op);
  // MI.dump();
  MO.getExpr()->dump();
  assert(MO.isExpr() && "Expr operand expected");
  // if (MO.isImm())
  //   return MO.getImm();

  Fixups.push_back(MCFixup::create(0, MO.getExpr(), FK_PCRel_1, MI.getLoc()));
  return 0;
}

unsigned CPEN211MCCodeEmitter::getCCOpValue(const MCInst &MI, unsigned Op,
                                            SmallVectorImpl<MCFixup> &Fixups,
                                            const MCSubtargetInfo &STI) const {
  const MCOperand &MO = MI.getOperand(Op);
  assert(MO.isImm() && "Immediate operand expected");
  switch (MO.getImm()) {
  case CPEN211CC::COND_EQ:
    return 0b001;
  case CPEN211CC::COND_NE:
    return 0b010;
  case CPEN211CC::COND_LT:
    return 0b011;
  case CPEN211CC::COND_LE:
    return 0b100;
  default:
    llvm_unreachable("Unknown condition code");
  }
}

MCCodeEmitter *createCPEN211MCCodeEmitter(const MCInstrInfo &MCII,
                                          MCContext &Ctx) {
  return new CPEN211MCCodeEmitter(Ctx, MCII);
}

#include "CPEN211GenMCCodeEmitter.inc"

} // end of namespace llvm
