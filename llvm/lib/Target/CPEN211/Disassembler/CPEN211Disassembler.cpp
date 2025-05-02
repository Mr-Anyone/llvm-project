//===-- CPEN211Disassembler.cpp - Disassembler for MSP430
//------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the CPEN211Disassembler class.
//
//===----------------------------------------------------------------------===//

#include "CPEN211.h"
#include "MCTargetDesc/CPEN211MCTargetDesc.h"
#include "TargetInfo/CPEN211TargetInfo.h"
#include "llvm/MC/MCContext.h"
#include "llvm/MC/MCDecoderOps.h"
#include "llvm/MC/MCDisassembler/MCDisassembler.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/Endian.h"

using namespace llvm;

#define DEBUG_TYPE "msp430-disassembler"

typedef MCDisassembler::DecodeStatus DecodeStatus;

namespace {
class CPEN211Disassembler : public MCDisassembler {
public:
  CPEN211Disassembler(const MCSubtargetInfo &STI, MCContext &Ctx)
      : MCDisassembler(STI, Ctx) {}

  DecodeStatus getInstruction(MCInst &MI, uint64_t &Size,
                              ArrayRef<uint8_t> Bytes, uint64_t Address,
                              raw_ostream &CStream) const override;
};
} // end anonymous namespace

static MCDisassembler *createCPEN211Disassembler(const Target &T,
                                                 const MCSubtargetInfo &STI,
                                                 MCContext &Ctx) {
  return new CPEN211Disassembler(STI, Ctx);
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeCPEN211Disassembler() {
  TargetRegistry::RegisterMCDisassembler(getTheCPEN211Target(),
                                         createCPEN211Disassembler);
}

static const unsigned GR16DecoderTable[] = {
    CPEN211::R0, CPEN211::R1, CPEN211::R2, CPEN211::R3,
    CPEN211::R4, CPEN211::R5, CPEN211::SP, CPEN211::LR};

static DecodeStatus DecodeGR16RegisterClass(MCInst &MI, uint64_t RegNo,
                                            uint64_t Address,
                                            const MCDisassembler *Decoder) {
  if (RegNo > 7)
    return MCDisassembler::Fail;

  unsigned Reg = GR16DecoderTable[RegNo];
  MI.addOperand(MCOperand::createReg(Reg));
  return MCDisassembler::Success;
}

static DecodeStatus DecodeMemOperand(MCInst &MI, uint64_t Bits,
                                     uint64_t Address,
                                     const MCDisassembler *Decoder);

#include "CPEN211GenDisassemblerTables.inc"

static DecodeStatus DecodeMemOperand(MCInst &MI, uint64_t Bits,
                                     uint64_t Address,
                                     const MCDisassembler *Decoder) {
  uint32_t Reg = Bits & 0b111;          // Register is the low three bits
  int64_t Imm5 = Bits >> 3 & (0b11111); // Immediate is the high three bits

  if (DecodeGR16RegisterClass(MI, Reg, Address, Decoder) !=
      MCDisassembler::Success)
    return MCDisassembler::Fail;

  MI.addOperand(MCOperand::createImm((int16_t)Imm5));
  return MCDisassembler::Success;
}

DecodeStatus CPEN211Disassembler::getInstruction(MCInst &MI, uint64_t &Size,
                                                 ArrayRef<uint8_t> Bytes,
                                                 uint64_t Address,
                                                 raw_ostream &CStream) const {
  if (Bytes.size() < 2) {
    Size = 0;
    return MCDisassembler::Fail;
  }

  Size = 2;

  uint64_t Insn = support::endian::read16le(Bytes.data());
  return decodeInstruction(DecoderTable16, MI, Insn, Address, this, STI);
}
