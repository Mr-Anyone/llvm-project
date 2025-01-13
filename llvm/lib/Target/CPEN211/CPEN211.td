//===-- CPEN211.td - Describe the MSP430 Target Machine -----*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
// This is the top level entry point for the CPEN211 target.
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Target-independent interfaces
//===----------------------------------------------------------------------===//

include "llvm/Target/Target.td"

//===----------------------------------------------------------------------===//
// Subtarget Features. 
//===----------------------------------------------------------------------===//
def FeatureX
 : SubtargetFeature<"ext", "ExtendedInsts", "true",
                    "Enable CPEN211-X extensions">;

def FeatureHWMult16
 : SubtargetFeature<"hwmult16", "HWMultMode", "HWMult16",
                    "Enable 16-bit hardware multiplier">;

def FeatureHWMult32
 : SubtargetFeature<"hwmult32", "HWMultMode", "HWMult32",
                    "Enable 32-bit hardware multiplier">;

def FeatureHWMultF5
 : SubtargetFeature<"hwmultf5", "HWMultMode", "HWMultF5",
                    "Enable F5 series hardware multiplier">;

//===----------------------------------------------------------------------===//
// CPEN211 supported processors.
//===----------------------------------------------------------------------===//
class Proc<string Name, list<SubtargetFeature> Features>
 : Processor<Name, NoItineraries, Features>;

def : Proc<"generic",         []>;
def : Proc<"msp430",          []>;
def : Proc<"msp430x",         [FeatureX]>;

//===----------------------------------------------------------------------===//
// Register File Description
//===----------------------------------------------------------------------===//

include "CPEN211RegisterInfo.td"

//===----------------------------------------------------------------------===//
// Calling Convention Description
//===----------------------------------------------------------------------===//

include "CPEN211CallingConv.td"

//===----------------------------------------------------------------------===//
// Instruction Descriptions
//===----------------------------------------------------------------------===//

include "CPEN211InstrInfo.td"

def CPEN211InstrInfo : InstrInfo;

//===---------------------------------------------------------------------===//
// Assembly Printers
//===---------------------------------------------------------------------===//

def CPEN211AsmWriter : AsmWriter {
  string AsmWriterClassName = "InstPrinter";
}

//===---------------------------------------------------------------------===//
// Assembly Parsers
//===---------------------------------------------------------------------===//

def CPEN211AsmParser : AsmParser {
  let AllowDuplicateRegisterNames = 1;
  let ShouldEmitMatchRegisterAltName = 1;
}

//===----------------------------------------------------------------------===//
// Target Declaration
//===----------------------------------------------------------------------===//

def CPEN211 : Target {
  let InstructionSet = CPEN211InstrInfo;
  let AssemblyParsers = [CPEN211AsmParser];
}

