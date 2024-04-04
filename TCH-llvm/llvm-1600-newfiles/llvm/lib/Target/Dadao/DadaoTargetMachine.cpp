//===-- DadaoTargetMachine.cpp - Define TargetMachine for Dadao ---------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// Implements the info about Dadao target spec.
//
//===----------------------------------------------------------------------===//

#include "DadaoTargetMachine.h"

#include "Dadao.h"
#include "DadaoMachineFunctionInfo.h"
#include "DadaoTargetObjectFile.h"
#include "DadaoTargetTransformInfo.h"
#include "TargetInfo/DadaoTargetInfo.h"
#include "llvm/Analysis/TargetTransformInfo.h"
#include "llvm/CodeGen/Passes.h"
#include "llvm/CodeGen/TargetLoweringObjectFileImpl.h"
#include "llvm/CodeGen/TargetPassConfig.h"
#include "llvm/MC/TargetRegistry.h"
#include "llvm/Support/FormattedStream.h"
#include "llvm/Target/TargetOptions.h"
#include <optional>

using namespace llvm;

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeDadaoTarget() {
  // Register the target.
  RegisterTargetMachine<DadaoTargetMachine> registered_target(
      getTheDadaoTarget());
  PassRegistry &PR = *PassRegistry::getPassRegistry();
  initializeDadaoDAGToDAGISelPass(PR);
}

static std::string computeDataLayout() {
  // Data layout (keep in sync with clang/lib/Basic/Targets.cpp)
  return "e"        // Little endian
         "-m:e"     // ELF name manging
         "-p:64:64" // 64-bit pointers, 64 bit aligned
         "-i64:64"  // 64 bit integers, 64 bit aligned
         "-a:0:64"  // 64 bit alignment of objects of aggregate type
         "-n64"     // 64 bit native integer width
         "-S128";   // 128 bit natural stack alignment
}

static Reloc::Model getEffectiveRelocModel(std::optional<Reloc::Model> RM) {
  return RM.value_or(Reloc::PIC_);
}

DadaoTargetMachine::DadaoTargetMachine(
    const Target &T, const Triple &TT, StringRef Cpu, StringRef FeatureString,
    const TargetOptions &Options, std::optional<Reloc::Model> RM,
    std::optional<CodeModel::Model> CodeModel, CodeGenOpt::Level OptLevel,
    bool JIT)
    : LLVMTargetMachine(T, computeDataLayout(), TT, Cpu, FeatureString, Options,
                        getEffectiveRelocModel(RM),
                        getEffectiveCodeModel(CodeModel, CodeModel::Medium),
                        OptLevel),
      Subtarget(TT, Cpu, FeatureString, *this, Options, getCodeModel(),
                OptLevel),
      TLOF(new DadaoTargetObjectFile()) {
  initAsmInfo();
}

TargetTransformInfo
DadaoTargetMachine::getTargetTransformInfo(const Function &F) const {
  return TargetTransformInfo(DadaoTTIImpl(this, F));
}

MachineFunctionInfo *DadaoTargetMachine::createMachineFunctionInfo(
    BumpPtrAllocator &Allocator, const Function &F,
    const TargetSubtargetInfo *STI) const {
  return DadaoMachineFunctionInfo::create<DadaoMachineFunctionInfo>(Allocator,
                                                                    F, STI);
}

namespace {
// Dadao Code Generator Pass Configuration Options.
class DadaoPassConfig : public TargetPassConfig {
public:
  DadaoPassConfig(DadaoTargetMachine &TM, PassManagerBase *PassManager)
      : TargetPassConfig(TM, *PassManager) {}

  DadaoTargetMachine &getDadaoTargetMachine() const {
    return getTM<DadaoTargetMachine>();
  }

  void addIRPasses() override;
  bool addInstSelector() override;
};
} // namespace

TargetPassConfig *
DadaoTargetMachine::createPassConfig(PassManagerBase &PassManager) {
  return new DadaoPassConfig(*this, &PassManager);
}

void DadaoPassConfig::addIRPasses() {
  addPass(createAtomicExpandPass());
  TargetPassConfig::addIRPasses();
}

// Install an instruction selector pass.
bool DadaoPassConfig::addInstSelector() {
  addPass(createDadaoISelDag(getDadaoTargetMachine()));
  return false;
}
