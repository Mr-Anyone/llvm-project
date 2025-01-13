add_llvm_component_library(
    #LLVMCPEN211Desc
    LLVMCPEN211Desc
    CPEN211AsmBackend.cpp
    CPEN211ELFObjectWriter.cpp
    CPEN211ELFStreamer.cpp
    CPEN211InstPrinter.cpp
    CPEN211MCAsmInfo.cpp
    CPEN211MCCodeEmitter.cpp
    CPEN211MCTargetDesc.cpp

  LINK_COMPONENTS
  MC
  CPEN211Info
  Support

  ADD_TO_COMPONENT
  CPEN211
  )
