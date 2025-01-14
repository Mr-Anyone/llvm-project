//===-- CPEN211InstrInfo.td - CPEN211 Instruction defs -------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file describes the CPEN211 instructions in TableGen format.
//
//===----------------------------------------------------------------------===//

include "CPEN211InstrFormats.td"

//===----------------------------------------------------------------------===//
// Type Profiles.
//===----------------------------------------------------------------------===//
def SDT_MSP430Cmp          : SDTypeProfile<0, 2, [SDTCisSameAs<0, 1>]>;
// the output operand is cond (see below), and target which is an i8
def SDT_MSP430BrCC         : SDTypeProfile<0, 2, [SDTCisVT<0, OtherVT>,
                                                  SDTCisVT<1, i8>]>;
//===----------------------------------------------------------------------===//
// CPEN211 Specific Node Definitions.
//===----------------------------------------------------------------------===//
def CPEN211retglue  :   SDNode<"CPEN211ISD::RET_GLUE", SDTNone,
                            [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;
def MSP430cmp     :     SDNode<"CPEN211ISD::CMP",   SDT_MSP430Cmp, [SDNPOutGlue]>;
def MSP430brcc    :     SDNode<"CPEN211ISD::BR_CC", SDT_MSP430BrCC,
                            [SDNPHasChain, SDNPInGlue]>;
//===----------------------------------------------------------------------===//
// CPEN211 Operand Definitions.
//===----------------------------------------------------------------------===//

def jmptarget : Operand<OtherVT> {
  // let PrintMethod = "printPCRelImmOperand";
  // let EncoderMethod = "getPCRelImmOpValue";
}

def cc : Operand<i8>{
  // let PrintMethod = "printCCOperand";
  // let EncoderMethod = "getCCOpValue";
}

//===----------------------------------------------------------------------===//
// Instruction list...
//===----------------------------------------------------------------------===//

// For Reference
// class IForm<bits<3> opcode, bits<2> op, int size, dag outs, 
//             dag ins, string asmstr, list<dag> pattern>

// TODO (for Vincent): we need let llvm know that we can do a shift as well!
//===----------------------------------------------------------------------
//Move Instructions

// TODO (for Vincent): is the size 16?
def MOV16rr : IForm<0b110, 0b00, 2, (outs GR16:$Rd), (ins GR16:$Rm), "MOV\t{$Rd,$Rm}", []>;

//===----------------------------------------------------------------------
// ALU Instructions without cmp
let isCommutable = true in {
    def ADD16rr : IForm<0b101, 0b00, 2, (outs GR16:$Rd)
                        , (ins GR16:$Rm, GR16:$Rn), "ADD\t{$Rd,$Rn,$Rm}", [(set GR16:$Rd, (add GR16:$Rn, GR16:$Rm))]>;

    def AND16rr : IForm<0b101, 0b10, 2, (outs GR16:$Rd)
                        , (ins GR16:$Rm, GR16:$Rn), "ADD\t{$Rd,$Rn,$Rm}", [(set GR16:$Rd, (and GR16:$Rn, GR16:$Rm))]>;

    // TODO (for Vincent): how do you translate this instruction in llvm?
    // def MVN16rr: IForm<0b101, 0b11, 2, (outs GR16:$Rd)
    //                     , (ins GR16:$Rm), "MVN\t{$Rd,$Rm}", []>;
}

//===----------------------------------------------------------------------
// Branch Type Instructions
let isBarrier = 1, cond = 0b000 in {
    // Branch to instruction
    def B   : BForm<2, (outs), (ins jmptarget:$dst)
                    ,"B\t$dst",  [(br bb:$dst)]>;
}


// BCC are BEQ, BNE, BLT, and BLE
def BCC : BForm<2, (outs), (ins jmptarget:$dst, cc:$cond),
               "B$cond\t$dst", [(MSP430brcc bb:$dst, imm:$cond)]>;
