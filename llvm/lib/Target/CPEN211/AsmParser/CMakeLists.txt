add_llvm_component_library(
    LLVMCPEN211AsmParser
    #LLVMCPEN211AsmParser
    CPEN211AsmParser.cpp

  LINK_COMPONENTS
  CodeGenTypes
  MC
  MCParser
  CPEN211Desc
  CPEN211Info
  Support

  ADD_TO_COMPONENT
  CPEN211
  )
