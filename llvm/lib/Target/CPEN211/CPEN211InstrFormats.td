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

class CPEN211Inst<dag outs, dag ins, int size, string asmstr> : Instruction {
  field bits<16> Inst;
  field bits<64> SoftFail = 0;

  let Namespace = "CPEN211";

  dag OutOperandList = outs;
  dag InOperandList  = ins;

  let AsmString = asmstr;
  let Size = size;
}

class BFormReg<bits<2> op, dag outs, dag ins, int size, string asmstr, list<dag> pattern> 
    : CPEN211Inst<outs, ins, size, asmstr>{
    let Pattern = pattern; 

    bits<3> Rn; 
    bits<3> Rd; 
    let Inst{15-13} = 0b010;
    let Inst{12-11} = op;
    let Inst{10-8}  = Rn;
    let Inst{7-5}   = Rd;
    let Inst{4-0}   = 0b00000;
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
  bits<2> shift;

  let Inst{15-13} = opcode;
  let Inst{12-11} = op;
  let Inst{10-8}  = Rn;
  let Inst{7-5}   = Rd;
  let Inst{4-3}   = shift;
  let Inst{2-0}   = Rm; 
}

// IFormImm 
// only for MOV16ri
class IFormImm<int size, dag outs, 
            dag ins, string asmstr, list<dag> pattern>
    : CPEN211Inst<outs, ins, size, asmstr> {
  let Pattern = pattern; 

  bits<3> Rn;
  bits<8> value;

  let Inst{15-13} = 0b110;
  let Inst{12-11} = 0b10;
  let Inst{10-8}  = Rn;
  let Inst{7-0}   = value;
}

// B Form instruction
// See Table for cond, like BEQ, BNE, BLT, BLE, and B
class BForm<int size, dag outs, 
            dag ins, string asmstr, list<dag> pattern>
    : CPEN211Inst<outs, ins, size, asmstr> {
  let Pattern = pattern;
  

  let Inst{15-13} = 0b001;
  let Inst{12-11} = 0b00;

  bits<3> cond;
  bits<8> dst;
  let Inst{10-8}  = cond;
  let Inst{7-0}   = dst;
}

// LDR and STR instructions
// R[Rd] = M[R[Rn] + sx(imm5)]  
// how are we going to deal with offset?
class MForm<bits<3> opcode, int size, dag outs, 
            dag ins, string asmstr, list<dag> pattern>
    : CPEN211Inst<outs, ins, size, asmstr> {
  let Pattern = pattern;

  // the low 3 bits will be register number
  // , the high 5 bits will be intermediate
  bits<8> RnAndImm;
  bits<3> Rd;

  let Inst{15-13} = opcode;
  let Inst{12-11} = 0b00;
  let Inst{10-8}  = RnAndImm{2...0};
  let Inst{7-5}   = Rd;
  let Inst{4-0}   = RnAndImm{7...3};
}

// Pseudo instructions 
class Pseudo<dag outs, dag ins, string asmstr, list<dag> pattern>
  : CPEN211Inst<outs, ins, 0, asmstr> {
  let Pattern = pattern;
}

