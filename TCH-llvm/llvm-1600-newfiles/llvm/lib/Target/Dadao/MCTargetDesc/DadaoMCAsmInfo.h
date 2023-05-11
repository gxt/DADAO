//=====-- DadaoMCAsmInfo.h - Dadao asm properties -----------*- C++ -*--====//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the declaration of the DadaoMCAsmInfo class.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_DADAO_MCTARGETDESC_DADAOMCASMINFO_H
#define LLVM_LIB_TARGET_DADAO_MCTARGETDESC_DADAOMCASMINFO_H

#include "llvm/MC/MCAsmInfoELF.h"

namespace llvm {
class Triple;

class DadaoMCAsmInfo : public MCAsmInfoELF {
  void anchor() override;

public:
  explicit DadaoMCAsmInfo(const Triple &TheTriple,
                          const MCTargetOptions &Options);
};

} // namespace llvm

#endif // LLVM_LIB_TARGET_DADAO_MCTARGETDESC_DADAOMCASMINFO_H
