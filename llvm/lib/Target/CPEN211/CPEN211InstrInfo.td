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
def SDT_CPEN211Cmp          : SDTypeProfile<0, 2, [SDTCisSameAs<0, 1>]>;
// TODO: as of current, there are no type constraint
def SDT_CPEN211BrCC         : SDTypeProfile<0, 2, []>;
//===----------------------------------------------------------------------===//
// CPEN211 Specific Node Definitions.
//===----------------------------------------------------------------------===//
def CPEN211retglue  :   SDNode<"CPEN211ISD::RET_GLUE", SDTNone,
                            [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;
def CPEN211cmp     :     SDNode<"CPEN211ISD::CMP",   SDT_CPEN211Cmp, [SDNPOutGlue]>;
def CPEN211brcc    :     SDNode<"CPEN211ISD::BR_CC", SDT_CPEN211BrCC,
                            [SDNPHasChain, SDNPInGlue]>;
//===----------------------------------------------------------------------===//
// CPEN211 Operand Definitions.
//===----------------------------------------------------------------------===//

def jmptarget : Operand<OtherVT> {
    let PrintMethod = "printPCRelImmOperand";
    let EncoderMethod = "getPCRelImmOpValue";
}

def cc : Operand<i8>{
    let PrintMethod = "printCCOperand";
    let EncoderMethod = "getCCOpValue";
}

//===----------------------------------------------------------------------===//
// Instruction list...
//===----------------------------------------------------------------------===//
//===----------------------------------------------------------------------
//Move Instructions

// TODO (for Vincent): we need let llvm know that we can do a shift as well!
// TODO (for Vincent): is the size 16?
let Rn = 0b000 in {
    def MOV16rr : IForm<0b110, 0b00, 2, (outs GR16:$Rd), (ins GR16:$Rm), "MOV\t{$Rd,$Rm}", []>;
}
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
let isBranch = 1, isTerminator=1 in {
    let isBarrier = 1, cond= 0b000 in {
        // Branch to instruction
        def B   : BForm<2, (outs), (ins jmptarget:$dst)
                        ,"B\t$dst",  [(br bb:$dst)]>;
    }

    // BCC are BEQ, BNE, BLT, and BLE
    let cond = 0b001 in {
        def BCC : BForm<2, (outs), (ins jmptarget:$dst, cc:$cond),
                   "B$cond\t$dst", [(CPEN211brcc bb:$dst, imm:$cond)]>;
    }
} // isBranch, isTerminator

// TOOD (for Vincent): create a register alias both the stack pointer and the link register  
// TODO (for Vincent): maybe this is also not the play in a sense that it should be created as a instruction alias?
let isReturn=1 in def RET : Pseudo<(outs), (ins), 
                "BX\tR7", [(CPEN211retglue)]>;
