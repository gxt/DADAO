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

  if ((Reg >= Dadao::RD0) && (Reg <= Dadao::RD63))   return (Reg - Dadao::RD0);
  if ((Reg >= Dadao::RB0) && (Reg <= Dadao::RB63))   return (Reg - Dadao::RB0);
  if ((Reg >= Dadao::RA0) && (Reg <= Dadao::RA63))   return (Reg - Dadao::RA0);

  switch (Reg) {
  case Dadao::RDZERO:
  case Dadao::RBIP:
  case Dadao::RAVV:
    return 0;
  case Dadao::RBSP:
    return 1;
  case Dadao::RBFP:
    return 2;
  case Dadao::RBGP:
    return 3;
  case Dadao::RBTP:
    return 4;
  case Dadao::RBCA:
  case Dadao::RDCC:
    return 7;
  case Dadao::RDRV:
    return 15;
  case Dadao::RASP:
    return 63;
  default:
    llvm_unreachable("Unknown register number!");
  }
}
} // namespace llvm
#endif // LLVM_LIB_TARGET_DADAO_MCTARGETDESC_DADAOBASEINFO_H
