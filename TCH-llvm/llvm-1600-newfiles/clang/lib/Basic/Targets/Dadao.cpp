//===--- Dadao.cpp - Implement Dadao target feature support ---------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements Dadao TargetInfo objects.
//
//===----------------------------------------------------------------------===//

#include "Dadao.h"
#include "clang/Basic/MacroBuilder.h"
#include "llvm/ADT/StringSwitch.h"

using namespace clang;
using namespace clang::targets;

const char *const DadaoTargetInfo::GCCRegNames[] = {
    "r0",  "r1",  "r2",  "r3",  "r4",  "r5",  "r6",  "r7",  "r8",  "r9",  "r10",
    "r11", "r12", "r13", "r14", "r15", "r16", "r17", "r18", "r19", "r20", "r21",
    "r22", "r23", "r24", "r25", "r26", "r27", "r28", "r29", "r30", "r31"
};

ArrayRef<const char *> DadaoTargetInfo::getGCCRegNames() const {
  return llvm::ArrayRef(GCCRegNames);
}

const TargetInfo::GCCRegAlias DadaoTargetInfo::GCCRegAliases[] = {
    {{"pc"}, "r2"},   {{"sp"}, "r4"},   {{"fp"}, "r5"},   {{"rv"}, "r8"},
    {{"rr1"}, "r10"}, {{"rr2"}, "r11"}, {{"rca"}, "r15"},
};

ArrayRef<TargetInfo::GCCRegAlias> DadaoTargetInfo::getGCCRegAliases() const {
  return llvm::ArrayRef(GCCRegAliases);
}

bool DadaoTargetInfo::isValidCPUName(StringRef Name) const {
  return llvm::StringSwitch<bool>(Name).Case("wuming", true).Default(false);
}
void DadaoTargetInfo::fillValidCPUList(
    SmallVectorImpl<StringRef> &Values) const {
  Values.emplace_back("wuming");
}

bool DadaoTargetInfo::setCPU(const std::string &Name) {
  CPU = llvm::StringSwitch<CPUKind>(Name).Case("wuming", DADAO_WUMING).Default(DADAO_NONE);

  return CPU != DADAO_NONE;
}

bool DadaoTargetInfo::hasFeature(StringRef Feature) const {
  return llvm::StringSwitch<bool>(Feature).Case("dadao", true).Default(false);
}

void DadaoTargetInfo::getTargetDefines(const LangOptions &Opts,
                                       MacroBuilder &Builder) const {
  // Define __dadao__ when building for target dadao.
  Builder.defineMacro("__dadao__");

  // Set define for the CPU specified.
  switch (CPU) {
  case DADAO_WUMING:
    Builder.defineMacro("__DADAO_WUMING__");
    break;
  case DADAO_NONE:
    llvm_unreachable("Unhandled target CPU");
  }
}
