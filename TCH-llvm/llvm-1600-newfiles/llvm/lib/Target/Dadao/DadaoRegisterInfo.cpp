//===-- DadaoRegisterInfo.cpp - Dadao Register Information ------*- C++ -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file contains the Dadao implementation of the TargetRegisterInfo class.
//
//===----------------------------------------------------------------------===//

#include "DadaoRegisterInfo.h"
#include "DadaoCondCode.h"
#include "DadaoFrameLowering.h"
#include "DadaoInstrInfo.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/CodeGen/RegisterScavenging.h"
#include "llvm/CodeGen/TargetFrameLowering.h"
#include "llvm/CodeGen/TargetInstrInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Type.h"
#include "llvm/Support/ErrorHandling.h"

#define GET_REGINFO_TARGET_DESC
#include "DadaoGenRegisterInfo.inc"

using namespace llvm;

DadaoRegisterInfo::DadaoRegisterInfo() : DadaoGenRegisterInfo(Dadao::RBCA) {}

const uint16_t *
DadaoRegisterInfo::getCalleeSavedRegs(const MachineFunction * /*MF*/) const {
  return CSR_SaveList;
}

BitVector DadaoRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());

  Reserved.set(Dadao::RD0, Dadao::RD8);
  Reserved.set(Dadao::RB0, Dadao::RB8);
  Reserved.set(Dadao::RDZERO);
  Reserved.set(Dadao::RBSP);
  Reserved.set(Dadao::RBFP);
  Reserved.set(Dadao::RAVV);
  Reserved.set(Dadao::RASP);
  return Reserved;
}

bool DadaoRegisterInfo::requiresRegisterScavenging(
    const MachineFunction & /*MF*/) const {
  return true;
}

static unsigned getRRRIOpcodeVariant(unsigned Opcode) {
  switch (Opcode) {
  case Dadao::LDBS_RRII: return Dadao::LDMBS_RRRI;
  case Dadao::LDBU_RRII: return Dadao::LDMBU_RRRI;
  case Dadao::LDWS_RRII: return Dadao::LDMWS_RRRI;
  case Dadao::LDWU_RRII: return Dadao::LDMWU_RRRI;
  case Dadao::LDTS_RRII: return Dadao::LDMTS_RRRI;
  case Dadao::LDTU_RRII: return Dadao::LDMTU_RRRI;
  case Dadao::LDO_RRII:  return Dadao::LDMO_RRRI;
  case Dadao::LDRB_RRII: return Dadao::LDMRB_RRRI;
  case Dadao::STB_RRII:  return Dadao::STMB_RRRI;
  case Dadao::STW_RRII:  return Dadao::STMW_RRRI;
  case Dadao::STT_RRII:  return Dadao::STMT_RRRI;
  case Dadao::STO_RRII:  return Dadao::STMO_RRRI;
  case Dadao::STRB_RRII: return Dadao::STMRB_RRRI;
  default:
    llvm_unreachable("Opcode has no RRRI variant");
  }
}

bool DadaoRegisterInfo::eliminateFrameIndex(MachineBasicBlock::iterator II,
                                            int SPAdj, unsigned FIOperandNum,
                                            RegScavenger *RS) const {
  assert(SPAdj == 0 && "Unexpected");

  MachineInstr &MI = *II;
  MachineFunction &MF = *MI.getParent()->getParent();
  const TargetInstrInfo *TII = MF.getSubtarget().getInstrInfo();
  const TargetFrameLowering *TFI = MF.getSubtarget().getFrameLowering();
  bool HasFP = TFI->hasFP(MF);
  DebugLoc DL = MI.getDebugLoc();

  int FrameIndex = MI.getOperand(FIOperandNum).getIndex();

  int Offset = MF.getFrameInfo().getObjectOffset(FrameIndex) +
               MI.getOperand(FIOperandNum + 1).getImm();

  // Addressable stack objects are addressed using neg. offsets from rbfp
  // or pos. offsets from rbsp/basepointer
  if (!HasFP || (hasStackRealignment(MF) && FrameIndex >= 0))
    Offset += MF.getFrameInfo().getStackSize();

  Register FrameReg = getFrameRegister(MF);
  if (FrameIndex >= 0) {
    if (hasBasePointer(MF))
      FrameReg = getBaseRegister();
    else if (hasStackRealignment(MF))
      FrameReg = Dadao::RBSP;
  }

  // Replace frame index with a frame pointer reference.
  // If the offset is small enough to fit in the immediate field, directly
  // encode it.
  // Otherwise scavenge a register and encode it into a MOVHI, ORW_RWII_W0 sequence.
  if (!isInt<12>(Offset)) {
    assert(RS && "Register scavenging must be on");
    Register Reg = RS->FindUnusedReg(&Dadao::GPRDRegClass);
    if (!Reg)
      Reg = RS->scavengeRegister(&Dadao::GPRDRegClass, II, SPAdj);
    assert(Reg && "Register scavenger failed");

    bool HasNegOffset = false;
    // ALU ops have unsigned immediate values. If the Offset is negative, we
    // negate it here and reverse the opcode later.
    if (Offset < 0) {
      HasNegOffset = true;
      Offset = -Offset;
    }

    if (!isInt<16>(Offset)) {
      // Reg = hi(offset) | lo(offset)
      BuildMI(*MI.getParent(), II, DL, TII->get(Dadao::SETZW_RWII_W1), Reg)
          .addImm(static_cast<uint32_t>(Offset) >> 16);
      BuildMI(*MI.getParent(), II, DL, TII->get(Dadao::ORW_RWII_W0), Reg)
          .addReg(Reg)
          .addImm(Offset & 0xffffU);
    } else {
      // Reg = mov(offset)
      BuildMI(*MI.getParent(), II, DL, TII->get(Dadao::ADDI_RB_RRII), Reg)
          .addImm(0)
          .addImm(Offset);
    }
    // Reg = FrameReg OP Reg
    if (MI.getOpcode() == Dadao::ADDI_RRII) {
      BuildMI(*MI.getParent(), II, DL,
              HasNegOffset ? TII->get(Dadao::SUB_RB_ORRR) : TII->get(Dadao::ADD_RB_ORRR),
              MI.getOperand(0).getReg())
          .addReg(FrameReg)
          .addReg(Reg);
      MI.eraseFromParent();
      return true;
    }
    if (isMemRRIIOpcode(MI.getOpcode())) {
      MI.setDesc(TII->get(getRRRIOpcodeVariant(MI.getOpcode())));
    } else
      llvm_unreachable("Unexpected opcode in frame index operation");

    MI.getOperand(FIOperandNum).ChangeToRegister(FrameReg, /*isDef=*/false);
    MI.getOperand(FIOperandNum + 1)
        .ChangeToRegister(Reg, /*isDef=*/false, /*isImp=*/false,
                          /*isKill=*/true);
    return false;
  }

  MI.getOperand(FIOperandNum).ChangeToRegister(FrameReg, /*isDef=*/false);
  MI.getOperand(FIOperandNum + 1).ChangeToImmediate(Offset);

  return false;
}

bool DadaoRegisterInfo::hasBasePointer(const MachineFunction &MF) const {
  const MachineFrameInfo &MFI = MF.getFrameInfo();
  // When we need stack realignment and there are dynamic allocas, we can't
  // reference off of the stack pointer, so we reserve a base pointer.
  if (hasStackRealignment(MF) && MFI.hasVarSizedObjects())
    return true;

  return false;
}

unsigned DadaoRegisterInfo::getRARegister() const { return Dadao::RBCA; }

Register
DadaoRegisterInfo::getFrameRegister(const MachineFunction & /*MF*/) const {
  return Dadao::RBFP;
}

// TODO: use reserved rb6 reg
Register DadaoRegisterInfo::getBaseRegister() const { return Dadao::RB6; }

const uint32_t *
DadaoRegisterInfo::getCallPreservedMask(const MachineFunction & /*MF*/,
                                        CallingConv::ID /*CC*/) const {
  return CSR_RegMask;
}
