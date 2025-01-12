add_llvm_component_library(
    LLVMCPEN211Info
    #LLVMMSP430Info
  MSP430TargetInfo.cpp

  LINK_COMPONENTS
  MC
  Support

  ADD_TO_COMPONENT
  CPEN211
  )
