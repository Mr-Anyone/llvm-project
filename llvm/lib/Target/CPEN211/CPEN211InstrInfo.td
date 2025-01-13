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
// CPEN211 Specific Node Definitions.
//===----------------------------------------------------------------------===//
def MSP430retglue  : SDNode<"CPEN211ISD::RET_GLUE", SDTNone,
                       [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

//===----------------------------------------------------------------------===//
// CPEN211 Operand Definitions.
//===----------------------------------------------------------------------===//

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
def MOV16rr : IForm<0b110, 0b00, 16, (outs GR16:$Rd), (ins GR16:$Rm), "MOV\t{$Rd,$Rm}", []>;

//===----------------------------------------------------------------------
// ALU Instructions without cmp
let isCommutable = true in {
    def ADD16rr : IForm<0b101, 0b00, 16, (outs GR16:$Rd)
                        , (ins GR16:$Rm, GR16:$Rn), "ADD\t{$Rd,$Rn,$Rm}", []>;

    def AND16rr : IForm<0b101, 0b10, 16, (outs GR16:$Rd)
                        , (ins GR16:$Rm, GR16:$Rn), "ADD\t{$Rd,$Rn,$Rm}", []>;

    def MVN16rr: IForm<0b101, 0b11, 16, (outs GR16:$Rd)
                        , (ins GR16:$Rm), "MVN\t{$Rd,$Rm}", []>;
}

//===----------------------------------------------------------------------
// Branch Type Instructions
let isBranch = 1, isTerminator = 1 in {
}
