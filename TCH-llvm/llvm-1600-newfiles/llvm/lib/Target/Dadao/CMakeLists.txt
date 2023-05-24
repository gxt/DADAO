add_llvm_component_group(Dadao)

set(LLVM_TARGET_DEFINITIONS Dadao.td)

tablegen(LLVM DadaoGenAsmMatcher.inc -gen-asm-matcher)
tablegen(LLVM DadaoGenAsmWriter.inc -gen-asm-writer)
tablegen(LLVM DadaoGenCallingConv.inc -gen-callingconv)
tablegen(LLVM DadaoGenDAGISel.inc -gen-dag-isel)
tablegen(LLVM DadaoGenDisassemblerTables.inc -gen-disassembler)
tablegen(LLVM DadaoGenInstrInfo.inc -gen-instr-info)
tablegen(LLVM DadaoGenMCCodeEmitter.inc -gen-emitter)
tablegen(LLVM DadaoGenRegisterInfo.inc -gen-register-info)
tablegen(LLVM DadaoGenSubtargetInfo.inc -gen-subtarget)

add_public_tablegen_target(DadaoCommonTableGen)

add_llvm_target(DadaoCodeGen
  DadaoAsmPrinter.cpp
  DadaoFrameLowering.cpp
  DadaoInstrInfo.cpp
  DadaoISelDAGToDAG.cpp
  DadaoISelLowering.cpp
  DadaoMachineFunctionInfo.cpp
  DadaoMCInstLower.cpp
  DadaoMemAluCombiner.cpp
  DadaoRegisterInfo.cpp
  DadaoSelectionDAGInfo.cpp
  DadaoSubtarget.cpp
  DadaoTargetMachine.cpp
  DadaoTargetObjectFile.cpp

  LINK_COMPONENTS
  Analysis
  AsmPrinter
  CodeGen
  Core
  DadaoAsmParser
  DadaoDesc
  DadaoInfo
  MC
  SelectionDAG
  Support
  Target
  TargetParser
  TransformUtils

  ADD_TO_COMPONENT
  Dadao
)

add_subdirectory(AsmParser)
add_subdirectory(Disassembler)
add_subdirectory(MCTargetDesc)
add_subdirectory(TargetInfo)
