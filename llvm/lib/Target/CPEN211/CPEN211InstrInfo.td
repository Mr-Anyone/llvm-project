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
// {TrueV, FalseV, TargetCC, Flag(Glue)};, TargetCC is the enum in CPEN211CC:CondCoes
def SDT_CPEN211SelectCC     : SDTypeProfile<1, 3, [SDTCisSameAs<0, 1>,
                                                  SDTCisSameAs<1, 2>,
                                                  SDTCisVT<3, i16>
                                                  ]>;
def SDT_CPEN211Wrapper      : SDTypeProfile<1, 1, [SDTCisSameAs<0, 1>,
                                                  SDTCisPtrTy<0>]>;
def SDT_CPEN211shl          : SDTypeProfile<1, 2, [SDTCisSameAs<0, 1>,
                                                  SDTCisVT<0, i16>]>;
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
// for global instruction, points to a globaladdr_t
def CPEN211Wrapper : SDNode<"CPEN211ISD::Wrapper", SDT_CPEN211Wrapper>;
def CPEN211Load    : SDNode<"CPEN211ISD::LOAD", SDTLoad,
                        [SDNPHasChain, SDNPMayLoad, SDNPMemOperand]>;
def CPEN211Store   : SDNode<"CPEN211ISD::STORE", SDTStore,
                        [SDNPHasChain, SDNPMayStore, SDNPMemOperand]>;
def CPEN211shl     : SDNode<"CPEN211ISD::SHL", SDT_CPEN211shl, []>;
def CPEN211srl     : SDNode<"CPEN211ISD::SRL", SDT_CPEN211shl, []>; // notes that shl and shr should 
                                                                    // have the same constraint
def CPEN211sra     : SDNode<"CPEN211ISD::SRA", SDT_CPEN211shl, []>; // ^^ same as above!
//===----------------------------------------------------------------------===//
// CPEN211 Operand Definitions.
//===----------------------------------------------------------------------===//

def bl_target : Operand<i16> {
    let PrintMethod = "printBLTargetOpValue";
    let EncoderMethod = "getBLTargetOpValue";
}

def jmptarget : Operand<OtherVT> {
    let PrintMethod = "printPCRelImmOperand";
    let EncoderMethod = "getPCRelImmOpValue";
}

def cc : Operand<i16>{
    let PrintMethod = "printCCOperand";
    let EncoderMethod = "getCCOpValue";
}

// In CPEN211, we can only either shift by 
// This is essentially an enum, 
// 0 - no shift
// 1 - left shift
// 2 - logical right shift
// 3 - arithmetic right shift
def shift: Operand<i16>{
    let PrintMethod = "printShiftOperand";
    let EncoderMethod = "encodeShiftOperand";
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

def MOV16rr : IForm<0b110, 0b00, 2, (outs GR16:$Rd), (ins GR16:$Rm)
            , "MOV\t{$Rd, $Rm}", []>{
    let shift= 0b00;
    let Rn = 0b000;
}

let Rn = 0b000 in {
    def MOV16Shiftrr: IForm<0b110, 0b00, 2, (outs GR16:$Rd), (ins GR16:$Rm, shift:$shift)
                ,"MOV\t{$Rd, $Rm, $shift}", []>;
}

// Logical shift left
def : Pat<(CPEN211shl GR16:$Rn, (i16 1)), 
            (MOV16Shiftrr GR16:$Rn, (i16 1))>;
def : Pat<(CPEN211srl GR16:$Rn, (i16 1)), 
            (MOV16Shiftrr GR16:$Rn, (i16 2))>;
def : Pat<(CPEN211sra GR16:$Rn, (i16 1)), 
            (MOV16Shiftrr GR16:$Rn, (i16 3))>;

def mov16_ri_immleaf  : ImmLeaf<i16, [{ return Imm >= -128&& Imm <= 127; }]>;

// TOOD (for Vincent): for greater than 8 bit, we must store this in the stack and 
// do a Pseudo instruction instead
def MOV16ri : IFormImm<2, (outs GR16:$Rn), (ins i16imm:$value)
                        , "MOV\t{$Rn, $value}", [(set GR16:$Rn, mov16_ri_immleaf:$value)]>;

// Global Address,
def : Pat<(i16 (CPEN211Wrapper tglobaladdr:$dst)), (MOV16ri tglobaladdr:$dst)>;

//===----------------------------------------------------------------------
// ALU Instructions without cmp
let isCommutable = true, shift =0b00 in {
    def ADD16rr : IForm<0b101, 0b00, 2, (outs GR16:$Rd)
                        , (ins GR16:$Rm, GR16:$Rn), "ADD\t{$Rd, $Rn, $Rm}", [(set GR16:$Rd, (add GR16:$Rn, GR16:$Rm))]>;

    def AND16rr : IForm<0b101, 0b10, 2, (outs GR16:$Rd)
                        , (ins GR16:$Rm, GR16:$Rn), "AND\t{$Rd , $Rn, $Rm}", [(set GR16:$Rd, (and GR16:$Rn, GR16:$Rm))]>;

}
def MVN16rr: IForm<0b101, 0b11, 2, (outs GR16:$Rd)
                    , (ins GR16:$Rm), "MVN\t{$Rd, $Rm}", []>{
    let Rn = 0b000;
    let shift = 0b00;
}

// A || B  => !(!A && !B)
def : Pat<(or GR16:$Rn, GR16:$Rm), 
            (MVN16rr (AND16rr (MVN16rr GR16:$Rn), (MVN16rr GR16:$Rm)))>;
        
// ~A is A^-1
def : Pat<(xor GR16:$Rn, (i16 -1)), 
            (MVN16rr GR16:$Rn)>;

// https://math.stackexchange.com/questions/38473/is-xor-a-combination-of-and-and-not-operators
// A XOR B = (A NAND (A NAND B)) NAND (B NAND (A NAND B))
def : Pat<(xor GR16:$Rd, GR16: $Rn), 
            (MVN16rr // (A NAND (A NAND B)) NAND (B NAND (A NAND B)) 
                (AND16rr
                    (MVN16rr ( // A NAND (A NAND B)                              
                        AND16rr 
                        GR16:$Rd, 
                        (MVN16rr (AND16rr GR16:$Rd, GR16:$Rn))     // (A NAND B)
                    )),
                    (MVN16rr (// (B NAND (A NAND B))
                        AND16rr GR16:$Rn,  // B
                        (MVN16rr (AND16rr GR16:$Rd, GR16:$Rn))     // (A NAND B)

                    ))))>;

// CMP16rr is a native instructoin 
// is the size 2?
// FIXME: make a new instruction format?
def CMP16rr : CPEN211Inst<(outs), (ins GR16:$Rn, GR16:$Rm), /*size*/ 2
                        ,"CMP\t{$Rn, $Rm}"> {
    let Inst{15-11} =   0b11010;
    let Inst{7-5} =     0b000;

    bits<3> Rn; 
    bits<3> Rm; 
    let Inst{10-8} =    Rn;
    let Inst{2-0} =     Rm;

    let Pattern = [(CPEN211cmp GR16:$Rn, GR16:$Rm)];
    let Defs = [SR]; // prevent dead code optimization
}


//===----------------------------------------------------------------------
// Branch Type Instructions
let isBranch = 1, isTerminator=1 in {
    let isBarrier = 1 in {
        // Branch to instruction
        def B   : BForm<2, (outs), (ins jmptarget:$dst)
                        ,"B\t$dst",  [(br bb:$dst)]>{
            let cond = 0b000;
        }
    }

    // BCC are BEQ, BNE, BLT, and BLE
    def BCC : BForm<2, (outs), (ins jmptarget:$dst, cc:$cond),
               "B$cond\t$dst", [(CPEN211brcc bb:$dst, imm:$cond)]>{
    }

    let Rn = 0b000 in  def BX  : BFormReg<0b00, (outs), (ins GR16:$Rd), 2, 
                                "BX\t{$Rd}", []>;

    let Rn = 0b111 in  def BLX : BFormReg<0b10, (outs), (ins GR16:$Rd), 2, 
                                "BLX\t{$Rd}", []>;
} // isBranch, isTerminator

// FIXME: this is not correct as of current!
// we can only encode i8imm
let isCall = 1, cond = 0b111, Defs=[LR, R1, R2, R3], Uses=[SP] in {
    def BL    : BForm<2, (outs), (ins bl_target:$dst),
                        "BL\t$dst", []>;
}

def : Pat<(CPEN211call (i16 tglobaladdr:$dst)),
          (BL tglobaladdr:$dst)>;

def : Pat<(CPEN211call (i16 texternalsym:$dst)),
          (BL texternalsym:$dst)>;

// two operands: base, and addition
def addr : ComplexPattern<iPTR, 2, "SelectAddr", [], []>;

//===----------------------------------------------------------------------
// Memory Type Instructions
// TODO: check the dag selection pattern matching! This might have to be a 
// complex pattern instead of just GR16, because we can also left shift as well!
def LDR16rm: MForm<0b011, 2, (outs GR16:$Rd), (ins memsrc:$RnAndImm)
                    , "LDR\t{$Rd, [$RnAndImm]}", [(set GR16:$Rd, (CPEN211Load addr:$RnAndImm))]>;
def STR16mr: MForm<0b100, 2, (outs), (ins memsrc:$RnAndImm, GR16:$Rd)
                    , "STR\t{$Rd, [$RnAndImm]}", [(CPEN211Store GR16:$Rd, addr:$RnAndImm)]>;

// TOOD (for Vincent): create a register alias both the stack pointer and the link register  
// TODO (for Vincent): maybe this is also not the play in a sense that it should be created as a instruction alias?
let isReturn = 1, isTerminator = 1, isBarrier = 1 in def RET : Pseudo<(outs), (ins), 
                "BX\tR7", [(CPEN211retglue)]>{
    field bits<16> Inst = {0b0100000011100000}; 
}

//===----------------------------------------------------------------------
// Special type of instruction
let isCodeGenOnly = 1, Defs = [SP], Uses = [SP] in {
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

// From the look of things, sometime FrameIndex will need to be selected some how
// A lot of backend use this feature as a way to select FrameIndex
// Although the offset is always zero, we still need this operand beacuse eliminateFrameIndex 
// requires us to do so!
let isCodeGenOnly  = 1 in def AddFrame : Pseudo<(outs  GR16:$dst), (ins i16imm:$base, i16imm:$offset), "# AddFrame Pseudo", []>;

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
                            "# SUB IMM PSEUDO", 
                            [(set GR16:$dst, (add GR16:$rn, imm:$im))]>;

    def SUB16rr    : Pseudo<(outs GR16:$dst), (ins GR16:$rn, GR16:$rm), 
                            "# SUB RR PSEUDO", 
                            [(set GR16:$dst, (sub GR16:$rn, GR16:$rm))]>;
} // isCodeGenOnly = 1
