//===-- DadaoCallingConv.h - Dadao Custom CC Routines -------------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
///
/// \file
/// This file contains the custom routines for the Dadao Calling Convention
/// that aren't done by tablegen.
///
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_DADAO_DADAOCALLINGCONV_H
#define LLVM_LIB_TARGET_DADAO_DADAOCALLINGCONV_H

#include "MCTargetDesc/DadaoMCTargetDesc.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/Function.h"

namespace llvm {

/// Custom state to propagate llvm type info to register CC assigner
struct DadaoCCState : public CCState {
  ArrayRef<Type *> ArgTypeList;

  DadaoCCState(ArrayRef<Type *> ArgTypes, CallingConv::ID CC, bool IsVarArg,
              MachineFunction &MF, SmallVectorImpl<CCValAssign> &Locs,
              LLVMContext &C)
      : CCState(CC, IsVarArg, MF, Locs, C), ArgTypeList(ArgTypes) {}
};

/// NOTE this function is used to select registers for formal arguments and call
/// FIXME: Handling on pointer arguments is not complete
inline bool CC_Dadao_AssignToReg(unsigned &ValNo, MVT &ValVT, MVT &LocVT,
                                    CCValAssign::LocInfo &LocInfo,
                                    ISD::ArgFlagsTy &ArgFlags, CCState &State) {
  const DadaoCCState &CCInfo = static_cast<DadaoCCState &>(State);

  static const MCPhysReg DataRegList[] = {Dadao::RD16, Dadao::RD17, Dadao::RD18, Dadao::RD19,
                                          Dadao::RD20, Dadao::RD21, Dadao::RD22, Dadao::RD23,
                                          Dadao::RD24, Dadao::RD25, Dadao::RD26, Dadao::RD27,
                                         Dadao::RD28, Dadao::RD29, Dadao::RD30, Dadao::RD31};
  // Address registers have %a register priority
  static const MCPhysReg AddrRegList[] = {Dadao::RB16, Dadao::RB17, Dadao::RB18, Dadao::RB19,
                                          Dadao::RB20, Dadao::RB21, Dadao::RB22, Dadao::RB23,
                                          Dadao::RB24, Dadao::RB25, Dadao::RB26, Dadao::RB27,
                                          Dadao::RB28, Dadao::RB29, Dadao::RB30, Dadao::RB31};

//   const auto &ArgTypes = CCInfo.ArgTypeList;
//   auto I = ArgTypes.begin(), End = ArgTypes.end();
//   int No = ValNo;
//   while (No > 0 && I != End) {
//     No -= (*I)->isIntegerTy(64) ? 2 : 1;
//     ++I;
//   }

  bool IsPtr = CCInfo.ArgTypeList[ValNo]->isPointerTy();

  unsigned Reg =
      IsPtr ? State.AllocateReg(AddrRegList) : State.AllocateReg(DataRegList);

  if (Reg) {
    State.addLoc(CCValAssign::getReg(ValNo, ValVT, Reg, LocVT, LocInfo));
    return true;
  }

  return false;
}

} // namespace llvm

#endif // LLVM_LIB_TARGET_Dadao_DadaoCALLINGCONV_H