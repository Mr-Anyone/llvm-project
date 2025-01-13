add_llvm_component_group(CPEN211)

set(LLVM_TARGET_DEFINITIONS CPEN211.td)

tablegen(LLVM CPEN211GenAsmMatcher.inc -gen-asm-matcher)
tablegen(LLVM CPEN211GenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM CPEN211GenCallingConv.inc -gen-callingconv)
tablegen(LLVM CPEN211GenDAGISel.inc -gen-dag-isel)
tablegen(LLVM CPEN211GenDisassemblerTables.inc -gen-disassembler)
tablegen(LLVM CPEN211GenInstrInfo.inc -gen-instr-info)
tablegen(LLVM CPEN211GenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM CPEN211GenRegisterInfo.inc -gen-register-info)
tablegen(LLVM CPEN211GenSubtargetInfo.inc -gen-subtarget)

add_public_tablegen_target(CPEN211CommonTableGen)

add_llvm_target(CPEN211CodeGen
    CPEN211BranchSelector.cpp
    CPEN211ISelDAGToDAG.cpp
    CPEN211ISelLowering.cpp
    CPEN211InstrInfo.cpp
    CPEN211FrameLowering.cpp
    CPEN211MachineFunctionInfo.cpp
    CPEN211RegisterInfo.cpp
    CPEN211Subtarget.cpp
    CPEN211TargetMachine.cpp
    CPEN211AsmPrinter.cpp
    CPEN211MCInstLower.cpp

  LINK_COMPONENTS
  AsmPrinter
  CodeGen
  CodeGenTypes
  Core
  MC
  CPEN211Desc
  CPEn211Info
  SelectionDAG
  Support
  Target

  ADD_TO_COMPONENT
  CPEN211
)

add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
add_subdirectory(AsmParser)
add_subdirectory(Disassembler)
