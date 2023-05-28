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
#include "DadaoAluCode.h"
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

DadaoRegisterInfo::DadaoRegisterInfo() : DadaoGenRegisterInfo(Dadao::RCA) {}

const uint16_t *
DadaoRegisterInfo::getCalleeSavedRegs(const MachineFunction * /*MF*/) const {
  return CSR_SaveList;
}

BitVector DadaoRegisterInfo::getReservedRegs(const MachineFunction &MF) const {
  BitVector Reserved(getNumRegs());

  Reserved.set(Dadao::R0);
  Reserved.set(Dadao::R1);
  Reserved.set(Dadao::PC);
  Reserved.set(Dadao::R2);
  Reserved.set(Dadao::SP);
  Reserved.set(Dadao::R4);
  Reserved.set(Dadao::FP);
  Reserved.set(Dadao::R5);
  Reserved.set(Dadao::RR1);
  Reserved.set(Dadao::R10);
  Reserved.set(Dadao::RR2);
  Reserved.set(Dadao::R11);
  Reserved.set(Dadao::RCA);
  Reserved.set(Dadao::R15);
  if (hasBasePointer(MF))
    Reserved.set(getBaseRegister());
  return Reserved;
}

bool DadaoRegisterInfo::requiresRegisterScavenging(
    const MachineFunction & /*MF*/) const {
  return true;
}

static bool isALUArithLoOpcode(unsigned Opcode) {
  switch (Opcode) {
  case Dadao::ADD_I_LO:
  case Dadao::SUB_I_LO:
  case Dadao::ADD_F_I_LO:
  case Dadao::SUB_F_I_LO:
    return true;
  default:
    return false;
  }
}

static unsigned getOppositeALULoOpcode(unsigned Opcode) {
  switch (Opcode) {
  case Dadao::ADD_I_LO:
    return Dadao::SUB_I_LO;
  case Dadao::SUB_I_LO:
    return Dadao::ADD_I_LO;
  case Dadao::ADD_F_I_LO:
    return Dadao::SUB_F_I_LO;
  case Dadao::SUB_F_I_LO:
    return Dadao::ADD_F_I_LO;
  default:
    llvm_unreachable("Invalid ALU lo opcode");
  }
}

static unsigned getRRRIOpcodeVariant(unsigned Opcode) {
  switch (Opcode) {
  case Dadao::LDBs_RI:
    return Dadao::LDMBS_RRRI;
  case Dadao::LDBz_RI:
    return Dadao::LDMBU_RRRI;
  case Dadao::LDHs_RI:
    return Dadao::LDMWS_RRRI;
  case Dadao::LDHz_RI:
    return Dadao::LDMWU_RRRI;
  case Dadao::LDW_RI:
    return Dadao::LDMO_RRRI;
  case Dadao::STB_RI:
    return Dadao::STMB_RRRI;
  case Dadao::STH_RI:
    return Dadao::STMW_RRRI;
  case Dadao::SW_RI:
    return Dadao::STMO_RRRI;
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

  // Addressable stack objects are addressed using neg. offsets from fp
  // or pos. offsets from sp/basepointer
  if (!HasFP || (hasStackRealignment(MF) && FrameIndex >= 0))
    Offset += MF.getFrameInfo().getStackSize();

  Register FrameReg = getFrameRegister(MF);
  if (FrameIndex >= 0) {
    if (hasBasePointer(MF))
      FrameReg = getBaseRegister();
    else if (hasStackRealignment(MF))
      FrameReg = Dadao::SP;
  }

  // Replace frame index with a frame pointer reference.
  // If the offset is small enough to fit in the immediate field, directly
  // encode it.
  // Otherwise scavenge a register and encode it into a MOVHI, OR_I_LO sequence.
  if ((isSPLSOpcode(MI.getOpcode()) && !isInt<10>(Offset)) ||
      !isInt<16>(Offset)) {
    assert(RS && "Register scavenging must be on");
    Register Reg = RS->FindUnusedReg(&Dadao::GPRRegClass);
    if (!Reg)
      Reg = RS->scavengeRegister(&Dadao::GPRRegClass, II, SPAdj);
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
      BuildMI(*MI.getParent(), II, DL, TII->get(Dadao::MOVHI), Reg)
          .addImm(static_cast<uint32_t>(Offset) >> 16);
      BuildMI(*MI.getParent(), II, DL, TII->get(Dadao::OR_I_LO), Reg)
          .addReg(Reg)
          .addImm(Offset & 0xffffU);
    } else {
      // Reg = mov(offset)
      BuildMI(*MI.getParent(), II, DL, TII->get(Dadao::ADD_I_LO), Reg)
          .addImm(0)
          .addImm(Offset);
    }
    // Reg = FrameReg OP Reg
    if (MI.getOpcode() == Dadao::ADD_I_LO) {
      BuildMI(*MI.getParent(), II, DL,
              HasNegOffset ? TII->get(Dadao::SUB_R) : TII->get(Dadao::ADD_R),
              MI.getOperand(0).getReg())
          .addReg(FrameReg)
          .addReg(Reg)
          .addImm(LPCC::ICC_T);
      MI.eraseFromParent();
      return true;
    }
    if (isSPLSOpcode(MI.getOpcode()) || isRMOpcode(MI.getOpcode())) {
      MI.setDesc(TII->get(getRRRIOpcodeVariant(MI.getOpcode())));
      if (HasNegOffset) {
        // Change the ALU op (operand 3) from LPAC::ADD (the default) to
        // LPAC::SUB with the already negated offset.
        assert((MI.getOperand(3).getImm() == LPAC::ADD) &&
               "Unexpected ALU op in RRRI instruction");
        MI.getOperand(3).setImm(LPAC::SUB);
      }
    } else
      llvm_unreachable("Unexpected opcode in frame index operation");

    MI.getOperand(FIOperandNum).ChangeToRegister(FrameReg, /*isDef=*/false);
    MI.getOperand(FIOperandNum + 1)
        .ChangeToRegister(Reg, /*isDef=*/false, /*isImp=*/false,
                          /*isKill=*/true);
    return false;
  }

  // ALU arithmetic ops take unsigned immediates. If the offset is negative,
  // we replace the instruction with one that inverts the opcode and negates
  // the immediate.
  if ((Offset < 0) && isALUArithLoOpcode(MI.getOpcode())) {
    unsigned NewOpcode = getOppositeALULoOpcode(MI.getOpcode());
    // We know this is an ALU op, so we know the operands are as follows:
    // 0: destination register
    // 1: source register (frame register)
    // 2: immediate
    BuildMI(*MI.getParent(), II, DL, TII->get(NewOpcode),
            MI.getOperand(0).getReg())
        .addReg(FrameReg)
        .addImm(-Offset);
    MI.eraseFromParent();
  } else {
    MI.getOperand(FIOperandNum).ChangeToRegister(FrameReg, /*isDef=*/false);
    MI.getOperand(FIOperandNum + 1).ChangeToImmediate(Offset);
  }
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

unsigned DadaoRegisterInfo::getRARegister() const { return Dadao::RCA; }

Register
DadaoRegisterInfo::getFrameRegister(const MachineFunction & /*MF*/) const {
  return Dadao::FP;
}

Register DadaoRegisterInfo::getBaseRegister() const { return Dadao::R14; }

const uint32_t *
DadaoRegisterInfo::getCallPreservedMask(const MachineFunction & /*MF*/,
                                        CallingConv::ID /*CC*/) const {
  return CSR_RegMask;
}
