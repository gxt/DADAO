//===-- DadaoAluCode.h - ALU operator encoding ----------------------------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// The encoding for ALU operators used in RM and RRM operands
//
//===----------------------------------------------------------------------===//

#ifndef LLVM_LIB_TARGET_DADAO_DADAOALUCODE_H
#define LLVM_LIB_TARGET_DADAO_DADAOALUCODE_H

#include "llvm/ADT/StringSwitch.h"
#include "llvm/CodeGen/ISDOpcodes.h"
#include "llvm/Support/ErrorHandling.h"

namespace llvm {
namespace LPAC {
enum AluCode {
  ADD = 0x00,
  SUB = 0x02,
  OR = 0x05,
  XOR = 0x06,
  SPECIAL = 0x07,

  // Shift instructions are treated as SPECIAL when encoding the machine
  // instruction, but kept distinct until lowering. The constant values are
  // chosen to ease lowering.
  SHL = 0x17,
  SRL = 0x27,
  SRA = 0x37,

  // Indicates an unknown/unsupported operator
  UNKNOWN = 0xFF,
};

inline static unsigned encodeDadaoAluCode(unsigned AluOp) {
  unsigned const OP_ENCODING_MASK = 0x07;
  return AluOp & OP_ENCODING_MASK;
}

inline static unsigned getAluOp(unsigned AluOp) {
  unsigned const ALU_MASK = 0x3F;
  return AluOp & ALU_MASK;
}

inline static const char *dadaoAluCodeToString(unsigned AluOp) {
  switch (getAluOp(AluOp)) {
  case ADD:
    return "add";
  case SUB:
    return "sub";
  case OR:
    return "or";
  case XOR:
    return "xor";
  case SHL:
    return "sh";
  case SRL:
    return "sh";
  case SRA:
    return "sha";
  default:
    llvm_unreachable("Invalid ALU code.");
  }
}

inline static AluCode stringToDadaoAluCode(StringRef S) {
  return StringSwitch<AluCode>(S)
      .Case("add", ADD)
      .Case("sub", SUB)
      .Case("or", OR)
      .Case("xor", XOR)
      .Case("sh", SHL)
      .Case("srl", SRL)
      .Case("sha", SRA)
      .Default(UNKNOWN);
}

inline static AluCode isdToDadaoAluCode(ISD::NodeType Node_type) {
  switch (Node_type) {
  case ISD::ADD:
    return AluCode::ADD;
  case ISD::SUB:
    return AluCode::SUB;
  case ISD::OR:
    return AluCode::OR;
  case ISD::XOR:
    return AluCode::XOR;
  case ISD::SHL:
    return AluCode::SHL;
  case ISD::SRL:
    return AluCode::SRL;
  case ISD::SRA:
    return AluCode::SRA;
  default:
    return AluCode::UNKNOWN;
  }
}
} // namespace LPAC
} // namespace llvm

#endif // LLVM_LIB_TARGET_DADAO_DADAOALUCODE_H
