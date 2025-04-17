//===-- CPEN211RegisterInfo.td - MSP430 Register defs -------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
//  Declarations that describe the CPEN211 register file
//===----------------------------------------------------------------------===//

// not 3 because we have an awkward status register 
class CPEN211Reg<bits<3> num, string n, list<string> alt> : Register<n> {
  field bits<3> Num = num;
  let Namespace = "CPEN211";
  let HWEncoding{2-0} = num;
  // I don't think there are alt names
  let AltNames = alt;
}


//===----------------------------------------------------------------------===//
//  Registers
//===----------------------------------------------------------------------===//


def R0  : CPEN211Reg<0,  "R0", []>;
def R1  : CPEN211Reg<1,  "R1", []>;
def R2  : CPEN211Reg<2,  "R2", []>;
def R3  : CPEN211Reg<3,  "R3", []>;
def R4  : CPEN211Reg<4,  "R4", []>;
def R5  : CPEN211Reg<5,  "R5", []>;
def SP  : CPEN211Reg<6,  "R6", []>;
def LR  : CPEN211Reg<7,  "R7", []>;

// status register
// Z - zero
// V - overflow
// N - negative
def SR  : CPEN211Reg<7,  "SR", []>;  // a jank table gen hack to not modify num, because this will never reach codegen! 

def GR16 : RegisterClass<"CPEN211", [i16], 16,
   // Volatile registers
  (add R0,R1,R2,R3,R4,R5,LR,SP,SR)>;
