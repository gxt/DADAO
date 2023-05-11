//===-- DadaoBaseInfo.h - Top level definitions for Dadao MC ----*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains small standalone helper functions and enum definitions for
// the Dadao target useful for the compiler back-end and the MC libraries.
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_DADAO_MCTARGETDESC_DADAOBASEINFO_H
#define LLVM_LIB_TARGET_DADAO_MCTARGETDESC_DADAOBASEINFO_H

#include "DadaoMCTargetDesc.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/Support/DataTypes.h"
#include "llvm/Support/ErrorHandling.h"

namespace llvm {

// DadaoII - This namespace holds all of the target specific flags that
// instruction info tracks.
namespace DadaoII {
// Target Operand Flag enum.
enum TOF {
  //===------------------------------------------------------------------===//
  // Dadao Specific MachineOperand flags.
  MO_NO_FLAG,

  // MO_ABS_HI/LO - Represents the hi or low part of an absolute symbol
  // address.
  MO_ABS_HI,
  MO_ABS_LO,
};
} // namespace DadaoII

static inline unsigned getDadaoRegisterNumbering(unsigned Reg) {
  switch (Reg) {
  case Dadao::R0:
    return 0;
  case Dadao::R1:
    return 1;
  case Dadao::R2:
  case Dadao::PC:
    return 2;
  case Dadao::R3:
    return 3;
  case Dadao::R4:
  case Dadao::SP:
    return 4;
  case Dadao::R5:
  case Dadao::FP:
    return 5;
  case Dadao::R6:
    return 6;
  case Dadao::R7:
    return 7;
  case Dadao::R8:
  case Dadao::RV:
    return 8;
  case Dadao::R9:
    return 9;
  case Dadao::R10:
  case Dadao::RR1:
    return 10;
  case Dadao::R11:
  case Dadao::RR2:
    return 11;
  case Dadao::R12:
    return 12;
  case Dadao::R13:
    return 13;
  case Dadao::R14:
    return 14;
  case Dadao::R15:
  case Dadao::RCA:
    return 15;
  case Dadao::R16:
    return 16;
  case Dadao::R17:
    return 17;
  case Dadao::R18:
    return 18;
  case Dadao::R19:
    return 19;
  case Dadao::R20:
    return 20;
  case Dadao::R21:
    return 21;
  case Dadao::R22:
    return 22;
  case Dadao::R23:
    return 23;
  case Dadao::R24:
    return 24;
  case Dadao::R25:
    return 25;
  case Dadao::R26:
    return 26;
  case Dadao::R27:
    return 27;
  case Dadao::R28:
    return 28;
  case Dadao::R29:
    return 29;
  case Dadao::R30:
    return 30;
  case Dadao::R31:
    return 31;
  default:
    llvm_unreachable("Unknown register number!");
  }
}
} // namespace llvm
#endif // LLVM_LIB_TARGET_DADAO_MCTARGETDESC_DADAOBASEINFO_H
