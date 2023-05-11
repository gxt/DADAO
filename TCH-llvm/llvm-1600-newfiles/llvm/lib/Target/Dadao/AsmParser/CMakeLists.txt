include_directories( ${CMAKE_CURRENT_BINARY_DIR}/.. ${CMAKE_CURRENT_SOURCE_DIR}/.. )

add_llvm_component_library(LLVMDadaoAsmParser
  DadaoAsmParser.cpp

  LINK_COMPONENTS
  MC
  MCParser
  Support
  DadaoDesc
  DadaoInfo

  ADD_TO_COMPONENT
  Dadao
  )

add_dependencies( LLVMDadaoAsmParser DadaoCommonTableGen )
