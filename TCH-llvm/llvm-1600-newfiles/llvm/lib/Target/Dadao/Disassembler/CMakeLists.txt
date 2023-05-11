add_llvm_component_library(LLVMDadaoDisassembler
  DadaoDisassembler.cpp

  LINK_COMPONENTS
  DadaoDesc
  DadaoInfo
  MC
  MCDisassembler
  Support

  ADD_TO_COMPONENT
  Dadao
  )
