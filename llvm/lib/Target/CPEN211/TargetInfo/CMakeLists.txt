add_llvm_component_library(
    LLVMCPEN211Info
    #LLVMCPEN211Info
    CPEN211TargetInfo.cpp

  LINK_COMPONENTS
  MC
  Support

  ADD_TO_COMPONENT
  CPEN211
  )
