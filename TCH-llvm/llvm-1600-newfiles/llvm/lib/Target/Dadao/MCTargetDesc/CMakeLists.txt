add_llvm_component_library(LLVMDadaoDesc
  DadaoAsmBackend.cpp
  DadaoELFObjectWriter.cpp
  DadaoInstPrinter.cpp
  DadaoMCAsmInfo.cpp
  DadaoMCCodeEmitter.cpp
  DadaoMCExpr.cpp
  DadaoMCTargetDesc.cpp

  LINK_COMPONENTS
  DadaoInfo
  MC
  MCDisassembler
  Support
  TargetParser

  ADD_TO_COMPONENT
  Dadao
)
