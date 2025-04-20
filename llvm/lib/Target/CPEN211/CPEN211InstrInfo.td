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
def SDT_CPEN211Call         : SDTypeProfile<0, -1, [SDTCisVT<0, iPTR>]>;
def SDT_CPEN211CallSeqStart : SDCallSeqStart<[SDTCisVT<0, i16>,
                                             SDTCisVT<1, i16>]>;
def SDT_CPEN211CallSeqEnd   : SDCallSeqEnd<[SDTCisVT<0, i16>, SDTCisVT<1, i16>]>;
def SDT_CPEN211Cmp          : SDTypeProfile<0, 2, [SDTCisSameAs<0, 1>]>;
// TODO: as of current, there are no type constraint
def SDT_CPEN211BrCC         : SDTypeProfile<0, 2, []>;
// {TrueV, FalseV, TargetCC, Flag};, TargetCC is the enum in CPEN211CC:CondCoes
def SDT_CPEN211SelectCC     : SDTypeProfile<1, 3, [SDTCisSameAs<0, 1>,
                                                  SDTCisSameAs<1, 2>,
                                                  SDTCisVT<3, i16>
                                                  ]>;
//===----------------------------------------------------------------------===//
// CPEN211 Specific Node Definitions.
//===----------------------------------------------------------------------===//
def CPEN211retglue  :   SDNode<"CPEN211ISD::RET_GLUE", SDTNone,
                            [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;
def CPEN211cmp     :     SDNode<"CPEN211ISD::CMP",   SDT_CPEN211Cmp, [SDNPOutGlue]>;
// Chain, dest, target cc, flag
def CPEN211brcc    :     SDNode<"CPEN211ISD::BR_CC", SDT_CPEN211BrCC,
                            [SDNPHasChain, SDNPInGlue]>;
def CPEN211selectcc: SDNode<"CPEN211ISD::SELECT_CC", SDT_CPEN211SelectCC,
                            [SDNPInGlue]>;
def CPEN211call    : SDNode<"CPEN211ISD::CALL", SDT_CPEN211Call,
                     [SDNPHasChain, SDNPOutGlue, SDNPOptInGlue, SDNPVariadic]>;
def CPEN211callseq_start :
                 SDNode<"ISD::CALLSEQ_START", SDT_CPEN211CallSeqStart,
                        [SDNPHasChain, SDNPOutGlue]>;
def CPEN211callseq_end :
                 SDNode<"ISD::CALLSEQ_END",   SDT_CPEN211CallSeqEnd,
                        [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue]>;
//===----------------------------------------------------------------------===//
// CPEN211 Operand Definitions.
//===----------------------------------------------------------------------===//

def jmptarget : Operand<OtherVT> {
    let PrintMethod = "printPCRelImmOperand";
    let EncoderMethod = "getPCRelImmOpValue";
}

// TODO (for Vincent): I am not quite sure if this is right or not! is this i16 or i8? we might have to do something about this?
def cc : Operand<i16>{
    let PrintMethod = "printCCOperand";
    let EncoderMethod = "getCCOpValue";
}

def MemAsmOperand : AsmOperandClass {
  let Name = "Mem";
}

// Address operands
def memsrc : Operand<i16> {
  let PrintMethod = "printSrcMemOperand";
  let MIOperandInfo = (ops GR16, i16imm);
  let ParserMatchClass = MemAsmOperand;
  let EncoderMethod = "getMemOpValue";
  let DecoderMethod = "DecodeMemOperand";
}

//===----------------------------------------------------------------------===//
// Instruction list...
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------
//Move Instructions

// TODO (for Vincent): we need let llvm know that we can do a shift as well!
// TODO (for Vincent): is the size 2?
let Rn = 0b000 in {
    def MOV16rr : IForm<0b110, 0b00, 2, (outs GR16:$Rd), (ins GR16:$Rm)
                , "MOV\t{$Rd, $Rm}", []>;

    // TODO (for Vincent): not all imm are valid? What should you do when imm is greater than 
    // signed extended 8 bit
    // TOOD (for Vincent): for greater than 8 bit, we must store this in the stack and 
    // do a Pseudo instruction instead
    // TOOD (for Vincent): find out why doesn't i8imm doesn't work?
    let Rm=0b000 in {
        def MOV16ri : IForm<0b110, 0b10, 2, (outs GR16:$Rd), (ins i16imm:$value)
                , "MOV\t{$Rd, $value}", [(set GR16:$Rd, imm:$value)]>;
    }
} // Rn

//===----------------------------------------------------------------------
// ALU Instructions without cmp
let isCommutable = true in {
    def ADD16rr : IForm<0b101, 0b00, 2, (outs GR16:$Rd)
                        , (ins GR16:$Rm, GR16:$Rn), "ADD\t{$Rd, $Rn, $Rm}", [(set GR16:$Rd, (add GR16:$Rn, GR16:$Rm))]>;

    def AND16rr : IForm<0b101, 0b10, 2, (outs GR16:$Rd)
                        , (ins GR16:$Rm, GR16:$Rn), "AND\t{$Rd , $Rn, $Rm}", [(set GR16:$Rd, (and GR16:$Rn, GR16:$Rm))]>;

    // TODO (for Vincent): how do you translate this instruction in llvm?
    // def MVN16rr: IForm<0b101, 0b11, 2, (outs GR16:$Rd)
    //                     , (ins GR16:$Rm), "MVN\t{$Rd,$Rm}", []>;
}

// CMP16rr is a native instructoin 
// is the size 2?
// FIXME: make a new instruction format?
def CMP16rr : CPEN211Inst<(outs), (ins GR16:$Rn, GR16:$Rm), /*size*/ 2
                        ,"CMP\t{$Rn, $Rm}"> {
    let Pattern = [(CPEN211cmp GR16:$Rn, GR16:$Rm)];
    let Defs = [SR]; // prevent dead code optimization
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

// FIXME: the cond is not correct, a jank hack as of current to speed development
let isCall = 1, cond = 0b110 in {
    def BL    : BForm<2, (outs), (ins i16imm:$imm),
                        "BL\t$imm", []>;
}

def : Pat<(CPEN211call (i16 tglobaladdr:$dst)),
          (BL tglobaladdr:$dst)>;

// complex pattern to select address or not? 
// arguments:
// ValueType, numops, string fn, roots, props
def addr : ComplexPattern<iPTR, 2, "SelectAddr", [], []>;

//===----------------------------------------------------------------------
// Memory Type Instructions
// TODO: check the dag selection pattern matching! This might have to be a 
// complex pattern instead of just GR16, because we can also left shift as well!
def LDR16rm: MForm<0b011, 2, (outs GR16:$Rd), (ins memsrc:$Rn)
                    , "LDR\t{$Rd, [$Rn]}", [(set GR16:$Rd, (load addr:$Rn))]>;
def STR16mr: MForm<0b100, 2, (outs), (ins memsrc:$Rd, GR16:$Rn)
                    , "STR\t{$Rn, [$Rd]}", [(store GR16:$Rn, addr:$Rd)]>;

// TOOD (for Vincent): create a register alias both the stack pointer and the link register  
// TODO (for Vincent): maybe this is also not the play in a sense that it should be created as a instruction alias?
let isReturn = 1, isTerminator = 1, isBarrier = 1 in def RET : Pseudo<(outs), (ins), 
                "BX\tR7", [(CPEN211retglue)]>;

//===----------------------------------------------------------------------
// Special type of instruction
let isCodeGenOnly = 1, Defs = [SP, SP], Uses = [SP] in {
    def ADJCALLSTACKDOWN : Pseudo<(outs), (ins i16imm:$amt1, i16imm:$amt2),
                                  "#ADJCALLSTACKDOWN $amt1 $amt2",
                                  // TODO (for Vincent): this is in corrent!
                                  [(CPEN211callseq_start timm:$amt1, timm:$amt2)]>;
                                  //[]>;
    def ADJCALLSTACKUP   : Pseudo<(outs), (ins i16imm:$amt1, i16imm:$amt2),
                                  "#ADJCALLSTACKUP $amt1 $amt2",
                                  // TODO (for Vincent): this is in corrent!
                                  [(CPEN211callseq_end timm:$amt1, timm:$amt2)]>;
                                  //[]>;
} // isCodeGenOnly = 1

let isCodeGenOnly = 1, usesCustomInserter = 1 in {
    // this is a pseudo because we have to move the intermediate into 
    // another register to perform the compare. We would have to move the 
    // the intermediate to deal with this using a register
    def CMP16ri : Pseudo<(outs), (ins GR16:$Rn, i16imm:$imm), 
                            "#CMP16ri PSEUDO", 
                            [(CPEN211cmp GR16:$Rn, imm:$imm)]>;

    def SelectCC16 : Pseudo<(outs GR16:$dst), (ins GR16:$src, GR16:$src2, i16imm:$cc),
                        "# SelectCC16 PSEUDO",
                        [(set GR16:$dst,
                          // true, false, cond_code
                          (CPEN211selectcc GR16:$src, GR16:$src2, imm:$cc))]>;
    def SUB16ri    : Pseudo<(outs GR16:$dst), (ins GR16:$rn, i16imm:$im), 
                            "# SUB PSEUDO", 
                            [(set GR16:$dst, (add GR16:$rn, imm:$im))]>;
} // isCodeGenOnly = 1
