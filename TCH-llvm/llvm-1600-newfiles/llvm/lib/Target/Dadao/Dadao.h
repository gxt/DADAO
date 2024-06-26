//===-- Dadao.h - Top-level interface for Dadao representation --*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the entry points for global functions defined in the LLVM
// Dadao back-end.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_DADAO_DADAO_H
#define LLVM_LIB_TARGET_DADAO_DADAO_H

#include "llvm/Pass.h"

namespace llvm {
class FunctionPass;
class DadaoTargetMachine;
class PassRegistry;

// createDadaoISelDag - This pass converts a legalized DAG into a
// Dadao-specific DAG, ready for instruction scheduling.
FunctionPass *createDadaoISelDag(DadaoTargetMachine &TM);

void initializeDadaoDAGToDAGISelPass(PassRegistry &);

} // namespace llvm

#endif // LLVM_LIB_TARGET_DADAO_DADAO_H
