//===-- CPEN211InstrFormats.td - MSP430 Instruction Formats -*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//  Describe CPEN211 instructions format here
//

class SourceMode<bits<2> val> {
  bits<2> Value = val;
}

def SrcReg      : SourceMode<0>; // r
def SrcMem      : SourceMode<1>; // m
def SrcImm      : SourceMode<2>; // i
// def SrcIndReg   : SourceMode<2>; // n
// def SrcPostInc  : SourceMode<3>; // p

class DestMode<bit val> {
  bit Value = val;
}

def DstReg      : DestMode<0>;   // r
def DstMem      : DestMode<1>;   // m

// Generic CPEN211 Format
class CPEN211Inst<dag outs, dag ins, int size, string asmstr> : Instruction {
  field bits<16> Inst;
  field bits<64> SoftFail = 0;

  let Namespace = "CPEN211";

  dag OutOperandList = outs;
  dag InOperandList  = ins;

  let AsmString = asmstr;
  let Size = size;
}

// Table 1 type instruction
// Basically all the alu instruction plus the MOV Rd, Rm instruction
// This is true minus the cmp instruction
class IForm<bits<3> opcode, bits<2> op, int size, dag outs, 
            dag ins, string asmstr, list<dag> pattern>
    : CPEN211Inst<outs, ins, size, asmstr> {
  let Pattern = pattern; 

  bits<3> Rm;
  bits<3> Rn;
  bits<3> Rd;

  let Inst{15-13} = opcode;
  let Inst{12-11} = op;
  let Inst{10-8}  = Rn;
  let Inst{7-5}   = Rd;
  let Inst{4-3}   = 0b00; // TODO (for Vincent): shift is not necessary 0
  let Inst{2-0}   = Rm; 
}

// Basic B types instruction
// See Table for cond, like BEQ, BNE, BLT, BLE, and B
class BForm<bits<3> cond, int size, dag outs, 
            dag ins, string asmstr, list<dag> pattern>
    : CPEN211Inst<outs, ins, size, asmstr> {
  let Pattern = pattern;

  let Inst{15-13} = 0b001;
  let Inst{12-11} = 0b00;
  let Inst{10-8}  = cond;
  // what is this?
  // let Inst{7-0}  = imm;
}

// Pseudo instructions 
class Pseudo<dag outs, dag ins, string asmstr, list<dag> pattern>
  : CPEN211Inst<outs, ins, 0, asmstr> {
  let Pattern = pattern;
}
