add_llvm_component_library(
    LLVMCPEN211Disassembler
    #LLVMCPEN211Disassembler
    CPEN211Disassembler.cpp

  LINK_COMPONENTS
  MCDisassembler
  MC
  CPEN211Info
  Support

  ADD_TO_COMPONENT
  CPEN211
  )
