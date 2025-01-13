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
def SrcIndReg   : SourceMode<2>; // n
def SrcPostInc  : SourceMode<3>; // p
def SrcImm      : SourceMode<3>; // i

class DestMode<bit val> {
  bit Value = val;
}

def DstReg      : DestMode<0>;   // r
def DstMem      : DestMode<1>;   // m

// Pseudo instructions
class Pseudo<dag outs, dag ins, string asmstr, list<dag> pattern>
  : CPEN211Inst<outs, ins, 0, asmstr> {
  let Pattern = pattern;
}
