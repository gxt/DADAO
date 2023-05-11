//===-- DadaoSelectionDAGInfo.cpp - Dadao SelectionDAG Info -------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the DadaoSelectionDAGInfo class.
//
//===----------------------------------------------------------------------===//

#include "DadaoSelectionDAGInfo.h"

#include "DadaoTargetMachine.h"

#define DEBUG_TYPE "dadao-selectiondag-info"

namespace llvm {

SDValue DadaoSelectionDAGInfo::EmitTargetCodeForMemcpy(
    SelectionDAG & /*DAG*/, const SDLoc & /*dl*/, SDValue /*Chain*/,
    SDValue /*Dst*/, SDValue /*Src*/, SDValue Size, Align /*Alignment*/,
    bool /*isVolatile*/, bool /*AlwaysInline*/,
    MachinePointerInfo /*DstPtrInfo*/,
    MachinePointerInfo /*SrcPtrInfo*/) const {
  ConstantSDNode *ConstantSize = dyn_cast<ConstantSDNode>(Size);
  if (!ConstantSize)
    return SDValue();

  return SDValue();
}

} // namespace llvm
