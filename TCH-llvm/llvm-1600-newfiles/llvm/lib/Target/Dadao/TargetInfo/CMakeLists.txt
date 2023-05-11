add_llvm_component_library(LLVMDadaoInfo
  DadaoTargetInfo.cpp

  LINK_COMPONENTS
  MC
  Support

  ADD_TO_COMPONENT
  Dadao
  )
