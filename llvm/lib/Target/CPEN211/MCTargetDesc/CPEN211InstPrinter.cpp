//===-- CPEN211InstPrinter.cpp - Convert MSP430 MCInst to assembly syntax
//--===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This class prints an CPEN211 MCInst to a .s file.
//
//===----------------------------------------------------------------------===//

#include "CPEN211InstPrinter.h"
#include "CPEN211.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/Support/ErrorHandling.h"
using namespace llvm;

#define DEBUG_TYPE "asm-printer"

// Include the auto-generated portion of the assembly writer.
#define PRINT_ALIAS_INSTR
#include "CPEN211GenAsmWriter.inc"

void CPEN211InstPrinter::printRegName(raw_ostream &O, MCRegister Reg) {
  O << getRegisterName(Reg);
}

void CPEN211InstPrinter::printInst(const MCInst *MI, uint64_t Address,
                                   StringRef Annot, const MCSubtargetInfo &STI,
                                   raw_ostream &O) {
  if (!printAliasInstr(MI, Address, O))
    printInstruction(MI, Address, O);
  printAnnotation(O, Annot);
}

void CPEN211InstPrinter::printPCRelImmOperand(const MCInst *MI, unsigned OpNo,
                                              raw_ostream &O) {
  const MCOperand &Op = MI->getOperand(OpNo);
  if (Op.isImm()) {
    int64_t Imm = Op.getImm() * 2 + 2;
    O << "$";
    if (Imm >= 0)
      O << '+';
    O << Imm;
  } else {
    assert(Op.isExpr() && "unknown pcrel immediate operand");
    Op.getExpr()->print(O, &MAI);
  }
}

void CPEN211InstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                      raw_ostream &O, const char *Modifier) {
  assert((Modifier == nullptr || Modifier[0] == 0) && "No modifiers supported");
  const MCOperand &Op = MI->getOperand(OpNo);
  if (Op.isReg()) {
    O << getRegisterName(Op.getReg());
  } else if (Op.isImm()) {
    O << '#' << Op.getImm();
  } else {
    assert(Op.isExpr() && "unknown operand kind in printOperand");
    O << '#';
    Op.getExpr()->print(O, &MAI);
  }
}

void CPEN211InstPrinter::printSrcMemOperand(const MCInst *MI, unsigned OpNo,
                                            raw_ostream &O,
                                            const char *Modifier) {
  const MCOperand &Base = MI->getOperand(OpNo);
  const MCOperand &Disp = MI->getOperand(OpNo + 1);

  assert(Base.isReg() && "I am not sure how base is not register?");
  assert(Disp.isImm() && "displacement have to me intermediate");

  const char *RegisterName = getRegisterName(Base.getReg());
  O << RegisterName << ", #" << Disp.getImm();
}

void CPEN211InstPrinter::printIndRegOperand(const MCInst *MI, unsigned OpNo,
                                            raw_ostream &O) {
  const MCOperand &Base = MI->getOperand(OpNo);
  O << "@" << getRegisterName(Base.getReg());
}

void CPEN211InstPrinter::printPostIndRegOperand(const MCInst *MI, unsigned OpNo,
                                                raw_ostream &O) {
  const MCOperand &Base = MI->getOperand(OpNo);
  O << "@" << getRegisterName(Base.getReg()) << "+";
}

void CPEN211InstPrinter::printCCOperand(const MCInst *MI, unsigned OpNo,
                                        raw_ostream &O) {
  unsigned CC = MI->getOperand(OpNo).getImm();

  switch (CC) {
  case CPEN211CC::CondCodes::COND_EQ:
    O << "EQ";
    break;
  case CPEN211CC::CondCodes::COND_LE:
    O << "LE";
    break;
  case CPEN211CC::CondCodes::COND_NE:
    O << "NE";
    break;
  case CPEN211CC::CondCodes::COND_LT:
    O << "LT";
    break;
  default:
    llvm_unreachable(
        "how did we get here? You should have Implemented it all!");
  }
}
