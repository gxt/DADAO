//===-- DadaoTargetInfo.cpp - Dadao Target Implementation -----------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

#include "TargetInfo/DadaoTargetInfo.h"
#include "llvm/MC/TargetRegistry.h"

using namespace llvm;

Target &llvm::getTheDadaoTarget() {
  static Target TheDadaoTarget;
  return TheDadaoTarget;
}

extern "C" LLVM_EXTERNAL_VISIBILITY void LLVMInitializeDadaoTargetInfo() {
  RegisterTarget<Triple::dadao> X(getTheDadaoTarget(), "dadao", "Dadao",
                                  "Dadao");
}
