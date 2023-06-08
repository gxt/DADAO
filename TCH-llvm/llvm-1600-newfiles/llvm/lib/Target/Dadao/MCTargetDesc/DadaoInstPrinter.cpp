//===-- DadaoInstPrinter.cpp - Convert Dadao MCInst to asm syntax ---------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This class prints an Dadao MCInst to a .s file.
//
//===----------------------------------------------------------------------===//

#include "DadaoInstPrinter.h"
#include "DadaoMCExpr.h"
#include "DadaoCondCode.h"
#include "DadaoWydePosition.h"
#include "MCTargetDesc/DadaoMCTargetDesc.h"
#include "llvm/MC/MCAsmInfo.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSymbol.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/FormattedStream.h"

using namespace llvm;

#define DEBUG_TYPE "asm-printer"

// Include the auto-generated portion of the assembly writer.
#define PRINT_ALIAS_INSTR
#include "DadaoGenAsmWriter.inc"

void DadaoInstPrinter::printRegName(raw_ostream &OS, MCRegister Reg) const {
  OS << StringRef(getRegisterName(Reg)).lower();
}

bool DadaoInstPrinter::printInst(const MCInst *MI, raw_ostream &OS,
                                 StringRef Alias, unsigned OpNo0,
                                 unsigned OpNo1) {
  OS << "\t" << Alias << " ";
  printOperand(MI, OpNo0, OS);
  OS << ", ";
  printOperand(MI, OpNo1, OS);
  return true;
}

void DadaoInstPrinter::printInst(const MCInst *MI, uint64_t Address,
                                 StringRef Annotation,
                                 const MCSubtargetInfo & /*STI*/,
                                 raw_ostream &OS) {
  if (!printAliasInstr(MI, Address, OS))
    printInstruction(MI, Address, OS);
  printAnnotation(OS, Annotation);
}

void DadaoInstPrinter::printOperand(const MCInst *MI, unsigned OpNo,
                                    raw_ostream &OS, const char *Modifier) {
  assert((Modifier == nullptr || Modifier[0] == 0) && "No modifiers supported");
  const MCOperand &Op = MI->getOperand(OpNo);
  if (Op.isReg())
    OS << "%" << getRegisterName(Op.getReg());
  else if (Op.isImm())
    OS << formatHex(Op.getImm());
  else {
    assert(Op.isExpr() && "Expected an expression");
    Op.getExpr()->print(OS, &MAI);
  }
}

void DadaoInstPrinter::printHi16ImmOperand(const MCInst *MI, unsigned OpNo,
                                           raw_ostream &OS) {
  const MCOperand &Op = MI->getOperand(OpNo);
  if (Op.isImm()) {
    OS << formatHex(Op.getImm() << 16);
  } else {
    // Symbolic operand will be lowered to immediate value by linker
    assert(Op.isExpr() && "Expected an expression");
    Op.getExpr()->print(OS, &MAI);
  }
}

void DadaoInstPrinter::printHi16AndImmOperand(const MCInst *MI, unsigned OpNo,
                                              raw_ostream &OS) {
  const MCOperand &Op = MI->getOperand(OpNo);
  if (Op.isImm()) {
    OS << formatHex((Op.getImm() << 16) | 0xffff);
  } else {
    // Symbolic operand will be lowered to immediate value by linker
    assert(Op.isExpr() && "Expected an expression");
    Op.getExpr()->print(OS, &MAI);
  }
}

void DadaoInstPrinter::printLo16AndImmOperand(const MCInst *MI, unsigned OpNo,
                                              raw_ostream &OS) {
  const MCOperand &Op = MI->getOperand(OpNo);
  if (Op.isImm()) {
    OS << formatHex(0xffff0000 | Op.getImm());
  } else {
    // Symbolic operand will be lowered to immediate value by linker
    assert(Op.isExpr() && "Expected an expression");
    Op.getExpr()->print(OS, &MAI);
  }
}

static void printMemoryBaseRegister(raw_ostream &OS, const MCOperand &RegOp) {
  assert(RegOp.isReg() && "Register operand expected");
  OS << "%" << DadaoInstPrinter::getRegisterName(RegOp.getReg());
}

template <unsigned SizeInBits>
static void printMemoryImmediateOffset(const MCAsmInfo &MAI,
                                       const MCOperand &OffsetOp,
                                       raw_ostream &OS) {
  assert((OffsetOp.isImm() || OffsetOp.isExpr()) && "Immediate expected");
  if (OffsetOp.isImm()) {
    assert(isInt<SizeInBits>(OffsetOp.getImm()) && "Constant value truncated");
    OS << OffsetOp.getImm();
  } else
    OffsetOp.getExpr()->print(OS, &MAI);
}

void DadaoInstPrinter::printMemRRIIOperand(const MCInst *MI, int OpNo,
                                         raw_ostream &OS,
                                         const char * /*Modifier*/) {
  const MCOperand &RegOp = MI->getOperand(OpNo);
  const MCOperand &OffsetOp = MI->getOperand(OpNo + 1);

  OS << "[";
  // Register
  printMemoryBaseRegister(OS, RegOp);
  OS << ", ";
  // Offset
  printMemoryImmediateOffset<12>(MAI, OffsetOp, OS);
  OS << "]";
}

void DadaoInstPrinter::printMemRRRIOperand(const MCInst *MI, int OpNo,
                                         raw_ostream &OS,
                                         const char * /*Modifier*/) {
  const MCOperand &RegOp = MI->getOperand(OpNo);
  const MCOperand &OffsetOp = MI->getOperand(OpNo + 1);
  assert(OffsetOp.isReg() && RegOp.isReg() && "Registers expected.");

  // [ Base, Offset ]
  OS << "[";
  OS << "%" << getRegisterName(RegOp.getReg());
  OS << ", ";
  OS << "%" << getRegisterName(OffsetOp.getReg());
  OS << "]";
  OS << ", 0";
}

void DadaoInstPrinter::printCCOperand(const MCInst *MI, int OpNo,
                                      raw_ostream &OS) {
  LPCC::CondCode CC =
      static_cast<LPCC::CondCode>(MI->getOperand(OpNo).getImm());
  // Handle the undefined value here for printing so we don't abort().
  if (CC >= LPCC::UNKNOWN)
    OS << "<und>";
  else
    OS << dadaoCondCodeToString(CC);
}

void DadaoInstPrinter::printWPosOperand(const MCInst *MI, unsigned OpNo,
                                             raw_ostream &OS) {
  DDWP::WydePosition WP =
      static_cast<DDWP::WydePosition>(MI->getOperand(OpNo).getImm());
  // Handle the undefined value here for printing so we don't abort().
  if (WP >= DDWP::BEYOND)
    OS << "<und>";
  else
    OS << dadaoWydePositionToString(WP);
}

void DadaoInstPrinter::printLo12AndImmOperand(const MCInst *MI, unsigned OpNo,
                                              raw_ostream &OS) {
  const MCOperand &Op = MI->getOperand(OpNo);
  if (Op.isImm()) {
    OS << formatHex(~0xFFF | Op.getImm());
  } else {
    // Symbolic operand will be lowered to immediate value by linker
    assert(Op.isExpr() && "Expected an expression");
    Op.getExpr()->print(OS, &MAI);
  }
}
