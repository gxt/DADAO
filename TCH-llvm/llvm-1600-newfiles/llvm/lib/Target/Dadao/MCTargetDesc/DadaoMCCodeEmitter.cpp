//===-- DadaoMCCodeEmitter.cpp - Convert Dadao code to machine code -------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the DadaoMCCodeEmitter class.
//
//===----------------------------------------------------------------------===//

#include "DadaoAluCode.h"
#include "MCTargetDesc/DadaoBaseInfo.h"
#include "MCTargetDesc/DadaoFixupKinds.h"
#include "MCTargetDesc/DadaoMCExpr.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/MC/MCCodeEmitter.h"
#include "llvm/MC/MCExpr.h"
#include "llvm/MC/MCFixup.h"
#include "llvm/MC/MCInst.h"
#include "llvm/MC/MCInstrInfo.h"
#include "llvm/MC/MCRegisterInfo.h"
#include "llvm/MC/MCSubtargetInfo.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/raw_ostream.h"
#include <cassert>
#include <cstdint>

#define DEBUG_TYPE "mccodeemitter"

STATISTIC(MCNumEmitted, "Number of MC instructions emitted");

namespace llvm {

namespace {

class DadaoMCCodeEmitter : public MCCodeEmitter {
public:
  DadaoMCCodeEmitter(const MCInstrInfo &MCII, MCContext &C) {}
  DadaoMCCodeEmitter(const DadaoMCCodeEmitter &) = delete;
  void operator=(const DadaoMCCodeEmitter &) = delete;
  ~DadaoMCCodeEmitter() override = default;

  // The functions below are called by TableGen generated functions for getting
  // the binary encoding of instructions/opereands.

  // getBinaryCodeForInstr - TableGen'erated function for getting the
  // binary encoding for an instruction.
  uint64_t getBinaryCodeForInstr(const MCInst &Inst,
                                 SmallVectorImpl<MCFixup> &Fixups,
                                 const MCSubtargetInfo &SubtargetInfo) const;

  // getMachineOpValue - Return binary encoding of operand. If the machine
  // operand requires relocation, record the relocation and return zero.
  unsigned getMachineOpValue(const MCInst &Inst, const MCOperand &MCOp,
                             SmallVectorImpl<MCFixup> &Fixups,
                             const MCSubtargetInfo &SubtargetInfo) const;

  unsigned getRRIIMemoryOpValue(const MCInst &Inst, unsigned OpNo,
                              SmallVectorImpl<MCFixup> &Fixups,
                              const MCSubtargetInfo &SubtargetInfo) const;

  unsigned getRRRIMemoryOpValue(const MCInst &Inst, unsigned OpNo,
                              SmallVectorImpl<MCFixup> &Fixups,
                              const MCSubtargetInfo &SubtargetInfo) const;

  unsigned getBranchTargetOpValue(const MCInst &Inst, unsigned OpNo,
                                  SmallVectorImpl<MCFixup> &Fixups,
                                  const MCSubtargetInfo &SubtargetInfo) const;

  void encodeInstruction(const MCInst &Inst, raw_ostream &Ostream,
                         SmallVectorImpl<MCFixup> &Fixups,
                         const MCSubtargetInfo &SubtargetInfo) const override;
};

} // end anonymous namespace

static Dadao::Fixups FixupKind(const MCExpr *Expr) {
  if (isa<MCSymbolRefExpr>(Expr))
    return Dadao::FIXUP_DADAO_21;
  if (const DadaoMCExpr *McExpr = dyn_cast<DadaoMCExpr>(Expr)) {
    DadaoMCExpr::VariantKind ExprKind = McExpr->getKind();
    switch (ExprKind) {
    case DadaoMCExpr::VK_Dadao_None:
      return Dadao::FIXUP_DADAO_21;
    case DadaoMCExpr::VK_Dadao_ABS_HI:
      return Dadao::FIXUP_DADAO_HI16;
    case DadaoMCExpr::VK_Dadao_ABS_LO:
      return Dadao::FIXUP_DADAO_LO16;
    }
  }
  return Dadao::Fixups(0);
}

// getMachineOpValue - Return binary encoding of operand. If the machine
// operand requires relocation, record the relocation and return zero.
unsigned DadaoMCCodeEmitter::getMachineOpValue(
    const MCInst &Inst, const MCOperand &MCOp, SmallVectorImpl<MCFixup> &Fixups,
    const MCSubtargetInfo &SubtargetInfo) const {
  if (MCOp.isReg())
    return getDadaoRegisterNumbering(MCOp.getReg());
  if (MCOp.isImm())
    return static_cast<unsigned>(MCOp.getImm());

  // MCOp must be an expression
  assert(MCOp.isExpr());
  const MCExpr *Expr = MCOp.getExpr();

  // Extract the symbolic reference side of a binary expression.
  if (Expr->getKind() == MCExpr::Binary) {
    const MCBinaryExpr *BinaryExpr = static_cast<const MCBinaryExpr *>(Expr);
    Expr = BinaryExpr->getLHS();
  }

  assert(isa<DadaoMCExpr>(Expr) || Expr->getKind() == MCExpr::SymbolRef);
  // Push fixup (all info is contained within)
  Fixups.push_back(
      MCFixup::create(0, MCOp.getExpr(), MCFixupKind(FixupKind(Expr))));
  return 0;
}

void DadaoMCCodeEmitter::encodeInstruction(
    const MCInst &Inst, raw_ostream &Ostream, SmallVectorImpl<MCFixup> &Fixups,
    const MCSubtargetInfo &SubtargetInfo) const {
  // Get instruction encoding and emit it
  unsigned Value = getBinaryCodeForInstr(Inst, Fixups, SubtargetInfo);
  ++MCNumEmitted; // Keep track of the number of emitted insns.

  // Emit bytes in big-endian
  for (int i = (4 - 1) * 8; i >= 0; i -= 8)
    Ostream << static_cast<char>((Value >> i) & 0xff);
}

// Encode Dadao Memory Operand
unsigned DadaoMCCodeEmitter::getRRIIMemoryOpValue(
    const MCInst &Inst, unsigned OpNo, SmallVectorImpl<MCFixup> &Fixups,
    const MCSubtargetInfo &SubtargetInfo) const {
  unsigned Encoding;
  const MCOperand Op1 = Inst.getOperand(OpNo + 0);
  const MCOperand Op2 = Inst.getOperand(OpNo + 1);

  assert(Op1.isReg() && "First operand is not register.");
  assert((Op2.isImm() || Op2.isExpr()) &&
         "Second operand is neither an immediate nor an expression.");

  Encoding = (getDadaoRegisterNumbering(Op1.getReg()) << 12);
  if (Op2.isImm()) {
    assert(isInt<12>(Op2.getImm()) &&
           "Constant value truncated (limited to 12-bit)");

    Encoding |= (Op2.getImm() & 0xfff);
  } else
    getMachineOpValue(Inst, Op2, Fixups, SubtargetInfo);

  return Encoding;
}

unsigned DadaoMCCodeEmitter::getRRRIMemoryOpValue(
    const MCInst &Inst, unsigned OpNo, SmallVectorImpl<MCFixup> &Fixups,
    const MCSubtargetInfo &SubtargetInfo) const {
  unsigned Encoding;
  const MCOperand Op1 = Inst.getOperand(OpNo + 0);
  const MCOperand Op2 = Inst.getOperand(OpNo + 1);
  const MCOperand AluMCOp = Inst.getOperand(OpNo + 2);

  assert(Op1.isReg() && "First operand is not RB register.");
  Encoding = (getDadaoRegisterNumbering(Op1.getReg()) << 12);
  assert(Op2.isReg() && "Second operand is not RD register.");
  Encoding |= (getDadaoRegisterNumbering(Op2.getReg()) << 6);

  assert(AluMCOp.isImm() && "Third operator is not immediate.");
  // Set Imm6
  Encoding |= AluMCOp.getImm();

  return Encoding;
}

unsigned DadaoMCCodeEmitter::getBranchTargetOpValue(
    const MCInst &Inst, unsigned OpNo, SmallVectorImpl<MCFixup> &Fixups,
    const MCSubtargetInfo &SubtargetInfo) const {
  const MCOperand &MCOp = Inst.getOperand(OpNo);
  if (MCOp.isReg() || MCOp.isImm())
    return getMachineOpValue(Inst, MCOp, Fixups, SubtargetInfo);

  Fixups.push_back(MCFixup::create(
      0, MCOp.getExpr(), static_cast<MCFixupKind>(Dadao::FIXUP_DADAO_25)));

  return 0;
}

#include "DadaoGenMCCodeEmitter.inc"

} // end namespace llvm

llvm::MCCodeEmitter *
llvm::createDadaoMCCodeEmitter(const MCInstrInfo &InstrInfo,
                               MCContext &context) {
  return new DadaoMCCodeEmitter(InstrInfo, context);
}
