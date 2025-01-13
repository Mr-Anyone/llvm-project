//===-- CPEN211MCTargetDesc.h - MSP430 Target Descriptions -------*- C++
//-*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file provides CPEN211 specific target descriptions.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_CPEN211_MCTARGETDESC_MSP430MCTARGETDESC_H
#define LLVM_LIB_TARGET_CPEN211_MCTARGETDESC_MSP430MCTARGETDESC_H

#include "llvm/Support/DataTypes.h"
#include <memory>

namespace llvm {
class Target;
class MCAsmBackend;
class MCCodeEmitter;
class MCInstrInfo;
class MCSubtargetInfo;
class MCRegisterInfo;
class MCContext;
class MCTargetOptions;
class MCObjectTargetWriter;
class MCStreamer;
class MCTargetStreamer;

/// Creates a machine code emitter for CPEN211.
MCCodeEmitter *createCPEN211MCCodeEmitter(const MCInstrInfo &MCII,
                                          MCContext &Ctx);

MCAsmBackend *createCPEN211MCAsmBackend(const Target &T,
                                        const MCSubtargetInfo &STI,
                                        const MCRegisterInfo &MRI,
                                        const MCTargetOptions &Options);

MCTargetStreamer *createCPEN211ObjectTargetStreamer(MCStreamer &S,
                                                    const MCSubtargetInfo &STI);

std::unique_ptr<MCObjectTargetWriter>
createCPEN211ELFObjectWriter(uint8_t OSABI);

} // namespace llvm

// Defines symbolic names for CPEN211 registers.
// This defines a mapping from register name to register number.
#define GET_REGINFO_ENUM
#include "CPEN211GenRegisterInfo.inc"

// Defines symbolic names for the CPEN211 instructions.
#define GET_INSTRINFO_ENUM
#define GET_INSTRINFO_MC_HELPER_DECLS
#include "CPEN211GenInstrInfo.inc"

#define GET_SUBTARGETINFO_ENUM
#include "CPEN211GenSubtargetInfo.inc"

#endif
