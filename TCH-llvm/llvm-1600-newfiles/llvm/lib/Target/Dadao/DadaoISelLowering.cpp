//===-- DadaoISelLowering.cpp - Dadao DAG Lowering Implementation ---------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file implements the DadaoTargetLowering class.
//
//===----------------------------------------------------------------------===//

#include "DadaoISelLowering.h"
#include "Dadao.h"
#include "DadaoCondCode.h"
#include "DadaoCallingConv.h"
#include "DadaoMachineFunctionInfo.h"
#include "DadaoSubtarget.h"
#include "DadaoTargetObjectFile.h"
#include "MCTargetDesc/DadaoBaseInfo.h"
#include "MCTargetDesc/DadaoMCTargetDesc.h"
#include "llvm/ADT/APInt.h"
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/SmallVector.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/ADT/StringSwitch.h"
#include "llvm/CodeGen/CallingConvLower.h"
#include "llvm/CodeGen/MachineFrameInfo.h"
#include "llvm/CodeGen/MachineFunction.h"
#include "llvm/CodeGen/MachineMemOperand.h"
#include "llvm/CodeGen/MachineRegisterInfo.h"
#include "llvm/CodeGen/RuntimeLibcalls.h"
#include "llvm/CodeGen/SelectionDAG.h"
#include "llvm/CodeGen/SelectionDAGNodes.h"
#include "llvm/CodeGen/TargetCallingConv.h"
#include "llvm/CodeGen/ValueTypes.h"
#include "llvm/IR/CallingConv.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/GlobalValue.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/CodeGen.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/ErrorHandling.h"
#include "llvm/Support/KnownBits.h"
#include "llvm/Support/MachineValueType.h"
#include "llvm/Support/MathExtras.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Target/TargetMachine.h"
#include <cassert>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <utility>

#define DEBUG_TYPE "dadao-lower"

using namespace llvm;
static const MCPhysReg ArgGPRs[] = {Dadao::RD16, Dadao::RD17, Dadao::RD18, Dadao::RD19,
                                          Dadao::RD20, Dadao::RD21, Dadao::RD22, Dadao::RD23,
                                          Dadao::RD24, Dadao::RD25, Dadao::RD26, Dadao::RD27,
                                         Dadao::RD28, Dadao::RD29, Dadao::RD30, Dadao::RD31};
// Limit on number of instructions the lowered multiplication may have before a
// call to the library function should be generated instead. The threshold is
// currently set to 14 as this was the smallest threshold that resulted in all
// constant multiplications being lowered. A threshold of 5 covered all cases
// except for one multiplication which required 14. mulsi3 requires 16
// instructions (including the prologue and epilogue but excluding instructions
// at call site). Until we can inline mulsi3, generating at most 14 instructions
// will be faster than invoking mulsi3.
static cl::opt<int> DadaoLowerConstantMulThreshold(
    "dadao-constant-mul-threshold", cl::Hidden,
    cl::desc("Maximum number of instruction to generate when lowering constant "
             "multiplication instead of calling library function [default=14]"),
    cl::init(14));

DadaoTargetLowering::DadaoTargetLowering(const TargetMachine &TM,
                                         const DadaoSubtarget &STI)
    : TargetLowering(TM) {
  // Set up the register classes.
  addRegisterClass(MVT::i64, &Dadao::GPRBRegClass);
  addRegisterClass(MVT::i64, &Dadao::GPRDRegClass);

  // Compute derived properties from the register classes
  TRI = STI.getRegisterInfo();
  computeRegisterProperties(TRI);

  setStackPointerRegisterToSaveRestore(Dadao::RBSP);

  setOperationAction(ISD::BR_CC, MVT::i64, Custom);
  setOperationAction(ISD::BR_JT, MVT::Other, Expand);
  setOperationAction(ISD::BRCOND, MVT::Other, Expand);
  setOperationAction(ISD::SETCC, MVT::i64, Custom);
  setOperationAction(ISD::SELECT, MVT::i64, Expand);
  setOperationAction(ISD::SELECT_CC, MVT::i64, Custom);

  setOperationAction(ISD::GlobalAddress, MVT::i64, Legal);
  setOperationAction(ISD::BlockAddress, MVT::i64, Custom);
  setOperationAction(ISD::JumpTable, MVT::i64, Custom);
  setOperationAction(ISD::ConstantPool, MVT::i64, Legal);

  setOperationAction(ISD::DYNAMIC_STACKALLOC, MVT::i64, Custom);
  setOperationAction(ISD::STACKSAVE, MVT::Other, Expand);
  setOperationAction(ISD::STACKRESTORE, MVT::Other, Expand);

  setOperationAction(ISD::VASTART, MVT::Other, Custom);
  setOperationAction(ISD::VAARG, MVT::Other, Expand);
  setOperationAction(ISD::VACOPY, MVT::Other, Expand);
  setOperationAction(ISD::VAEND, MVT::Other, Expand);

  setOperationAction(ISD::SDIVREM, MVT::i64, Expand);
  setOperationAction(ISD::UDIVREM, MVT::i64, Expand);

  setOperationAction(ISD::MULHU, MVT::i64, Expand);
  setOperationAction(ISD::MULHS, MVT::i64, Expand);
  setOperationAction(ISD::UMUL_LOHI, MVT::i64, Expand);
  setOperationAction(ISD::SMUL_LOHI, MVT::i64, Expand);

  setOperationAction(ISD::ROTR, MVT::i64, Expand);
  setOperationAction(ISD::ROTL, MVT::i64, Expand);
  setOperationAction(ISD::SHL_PARTS, MVT::i64, Custom);
  setOperationAction(ISD::SRL_PARTS, MVT::i64, Custom);
  setOperationAction(ISD::SRA_PARTS, MVT::i64, Expand);

  setOperationAction(ISD::BSWAP, MVT::i64, Expand);
  setOperationAction(ISD::CTPOP, MVT::i64, Expand);
  setOperationAction(ISD::CTLZ, MVT::i64, Expand);
  setOperationAction(ISD::CTTZ, MVT::i64, Expand);

  setOperationAction(ISD::SIGN_EXTEND_INREG, MVT::i1, Expand);
  setOperationAction(ISD::SIGN_EXTEND_INREG, MVT::i8, Expand);
  setOperationAction(ISD::SIGN_EXTEND_INREG, MVT::i16, Expand);
  setOperationAction(ISD::SIGN_EXTEND_INREG, MVT::i32, Expand);

  // Extended load operations for i1 types must be promoted
  for (MVT VT : MVT::integer_valuetypes()) {
    setLoadExtAction(ISD::EXTLOAD, VT, MVT::i1, Promote);
    setLoadExtAction(ISD::ZEXTLOAD, VT, MVT::i1, Promote);
    setLoadExtAction(ISD::SEXTLOAD, VT, MVT::i1, Promote);
  }

  setTargetDAGCombine({ISD::ADD, ISD::SUB, ISD::AND, ISD::OR, ISD::XOR});

  // Function alignments
  setMinFunctionAlignment(Align(4));
  setPrefFunctionAlignment(Align(4));

  setJumpIsExpensive(true);

  // TODO: Setting the minimum jump table entries needed before a
  // switch is transformed to a jump table to 100 to avoid creating jump tables
  // as this was causing bad performance compared to a large group of if
  // statements. Re-evaluate this on new benchmarks.
  setMinimumJumpTableEntries(100);

  // Use fast calling convention for library functions.
  for (int I = 0; I < RTLIB::UNKNOWN_LIBCALL; ++I) {
    setLibcallCallingConv(static_cast<RTLIB::Libcall>(I), CallingConv::Fast);
  }

  MaxStoresPerMemset = 16; // For @llvm.memset -> sequence of stores
  MaxStoresPerMemsetOptSize = 8;
  MaxStoresPerMemcpy = 16; // For @llvm.memcpy -> sequence of stores
  MaxStoresPerMemcpyOptSize = 8;
  MaxStoresPerMemmove = 16; // For @llvm.memmove -> sequence of stores
  MaxStoresPerMemmoveOptSize = 8;

  // Booleans always contain 0 or 1.
  setBooleanContents(ZeroOrOneBooleanContent);
}

SDValue DadaoTargetLowering::LowerOperation(SDValue Op,
                                            SelectionDAG &DAG) const {
  switch (Op.getOpcode()) {
  case ISD::BR_CC:
    return LowerBR_CC(Op, DAG);
  case ISD::BlockAddress:
    return LowerBlockAddress(Op, DAG);
  case ISD::JumpTable:
    return LowerJumpTable(Op, DAG);
  case ISD::SELECT_CC:
    return LowerSELECT_CC(Op, DAG);
  case ISD::SETCC:
    return LowerSETCC(Op, DAG);
  case ISD::SHL_PARTS:
    return LowerSHL_PARTS(Op, DAG);
  case ISD::SRL_PARTS:
    return LowerSRL_PARTS(Op, DAG);
  case ISD::VASTART:
    return LowerVASTART(Op, DAG);
  case ISD::DYNAMIC_STACKALLOC:
    return LowerDYNAMIC_STACKALLOC(Op, DAG);
  case ISD::RETURNADDR:
    return LowerRETURNADDR(Op, DAG);
  case ISD::FRAMEADDR:
    return LowerFRAMEADDR(Op, DAG);
  default:
    llvm_unreachable("unimplemented operand");
  }
}

//===----------------------------------------------------------------------===//
//                       Dadao Inline Assembly Support
//===----------------------------------------------------------------------===//

Register DadaoTargetLowering::getRegisterByName(
  const char *RegName, LLT /*VT*/,
  const MachineFunction & /*MF*/) const {
  // Only unallocatable registers should be matched here.
  Register Reg = StringSwitch<unsigned>(RegName)
                     .Case("rbip", Dadao::RBIP)
                     .Case("rbsp", Dadao::RBSP)
                     .Case("rbfp", Dadao::RBFP)
                     .Case("rbca", Dadao::RBCA)
                     .Default(0);

  if (Reg)
    return Reg;
  report_fatal_error("Invalid register name global variable");
}

std::pair<unsigned, const TargetRegisterClass *>
DadaoTargetLowering::getRegForInlineAsmConstraint(const TargetRegisterInfo *TRI,
                                                  StringRef Constraint,
                                                  MVT VT) const {
  if (Constraint.size() == 1)
    // GCC Constraint Letters
    switch (Constraint[0]) {
    case 'r': // GENERAL_REGS
      switch (Constraint[1]) {
      case 'd': // RD regs
        return std::make_pair(0U, &Dadao::GPRDRegClass);
      case 'b': // RB regs
        return std::make_pair(0U, &Dadao::GPRBRegClass);
      }
    default:
      break;
    }

  return TargetLowering::getRegForInlineAsmConstraint(TRI, Constraint, VT);
}

// Examine constraint type and operand type and determine a weight value.
// This object must already have been set up with the operand type
// and the current alternative constraint selected.
TargetLowering::ConstraintWeight
DadaoTargetLowering::getSingleConstraintMatchWeight(
    AsmOperandInfo &Info, const char *Constraint) const {
  ConstraintWeight Weight = CW_Invalid;
  Value *CallOperandVal = Info.CallOperandVal;
  // If we don't have a value, we can't do a match,
  // but allow it at the lowest weight.
  if (CallOperandVal == nullptr)
    return CW_Default;
  // Look at the constraint type.
  switch (*Constraint) {
  case 'I': // signed 16 bit immediate
  case 'J': // integer zero
  case 'K': // unsigned 16 bit immediate
  case 'L': // immediate in the range 0 to 31
  case 'M': // signed 32 bit immediate where lower 16 bits are 0
  case 'N': // signed 26 bit immediate
  case 'O': // integer zero
    if (isa<ConstantInt>(CallOperandVal))
      Weight = CW_Constant;
    break;
  default:
    Weight = TargetLowering::getSingleConstraintMatchWeight(Info, Constraint);
    break;
  }
  return Weight;
}

// LowerAsmOperandForConstraint - Lower the specified operand into the Ops
// vector.  If it is invalid, don't add anything to Ops.
void DadaoTargetLowering::LowerAsmOperandForConstraint(
    SDValue Op, std::string &Constraint, std::vector<SDValue> &Ops,
    SelectionDAG &DAG) const {
  SDValue Result;

  // Only support length 1 constraints for now.
  if (Constraint.length() > 1)
    return;

  char ConstraintLetter = Constraint[0];
  switch (ConstraintLetter) {
  case 'I': // Signed 16 bit constant
    // If this fails, the parent routine will give an error
    if (ConstantSDNode *C = dyn_cast<ConstantSDNode>(Op)) {
      if (isInt<16>(C->getSExtValue())) {
        Result = DAG.getTargetConstant(C->getSExtValue(), SDLoc(C),
                                       Op.getValueType());
        break;
      }
    }
    return;
  case 'J': // integer zero
  case 'O':
    if (ConstantSDNode *C = dyn_cast<ConstantSDNode>(Op)) {
      if (C->getZExtValue() == 0) {
        Result = DAG.getTargetConstant(0, SDLoc(C), Op.getValueType());
        break;
      }
    }
    return;
  case 'K': // unsigned 16 bit immediate
    if (ConstantSDNode *C = dyn_cast<ConstantSDNode>(Op)) {
      if (isUInt<16>(C->getZExtValue())) {
        Result = DAG.getTargetConstant(C->getSExtValue(), SDLoc(C),
                                       Op.getValueType());
        break;
      }
    }
    return;
  case 'L': // immediate in the range 0 to 31
    if (ConstantSDNode *C = dyn_cast<ConstantSDNode>(Op)) {
      if (C->getZExtValue() <= 31) {
        Result = DAG.getTargetConstant(C->getZExtValue(), SDLoc(C),
                                       Op.getValueType());
        break;
      }
    }
    return;
  case 'M': // signed 32 bit immediate where lower 16 bits are 0
    if (ConstantSDNode *C = dyn_cast<ConstantSDNode>(Op)) {
      int64_t Val = C->getSExtValue();
      if ((isInt<32>(Val)) && ((Val & 0xffff) == 0)) {
        Result = DAG.getTargetConstant(Val, SDLoc(C), Op.getValueType());
        break;
      }
    }
    return;
  case 'N': // signed 26 bit immediate
    if (ConstantSDNode *C = dyn_cast<ConstantSDNode>(Op)) {
      int64_t Val = C->getSExtValue();
      if ((Val >= -33554432) && (Val <= 33554431)) {
        Result = DAG.getTargetConstant(Val, SDLoc(C), Op.getValueType());
        break;
      }
    }
    return;
  default:
    break; // This will fall through to the generic implementation
  }

  if (Result.getNode()) {
    Ops.push_back(Result);
    return;
  }

  TargetLowering::LowerAsmOperandForConstraint(Op, Constraint, Ops, DAG);
}

//===----------------------------------------------------------------------===//
//                      Calling Convention Implementation
//===----------------------------------------------------------------------===//

#include "DadaoGenCallingConv.inc"

static unsigned NumFixedArgs;
static bool CC_Dadao32_VarArg(unsigned ValNo, MVT ValVT, MVT LocVT,
                              CCValAssign::LocInfo LocInfo,
                              ISD::ArgFlagsTy ArgFlags, CCState &State) {
  // Handle fixed arguments with default CC.
  // Note: Both the default and fast CC handle VarArg the same and hence the
  // calling convention of the function is not considered here.
  if (ValNo < NumFixedArgs) {
    return CC_Dadao32(ValNo, ValVT, LocVT, LocInfo, ArgFlags, State);
  }

  // Promote i8/i16/i32 args to i64
  if (LocVT == MVT::i8 || LocVT == MVT::i16 || LocVT == MVT::i32) {
    LocVT = MVT::i64;
    if (ArgFlags.isSExt())
      LocInfo = CCValAssign::SExt;
    else if (ArgFlags.isZExt())
      LocInfo = CCValAssign::ZExt;
    else
      LocInfo = CCValAssign::AExt;
  }
  unsigned Offset2 = State.AllocateStack(8, Align(8));
  State.addLoc(CCValAssign::getMem(ValNo, ValVT, Offset2, LocVT, LocInfo));
  return false;

  return true; // CC didn't match.
}

SDValue DadaoTargetLowering::LowerFormalArguments(
    SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc &DL,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const {
  switch (CallConv) {
  case CallingConv::C:
  case CallingConv::Fast:
    return LowerCCCArguments(Chain, CallConv, IsVarArg, Ins, DL, DAG, InVals);
  default:
    report_fatal_error("Unsupported calling convention");
  }
}

SDValue DadaoTargetLowering::LowerCall(TargetLowering::CallLoweringInfo &CLI,
                                       SmallVectorImpl<SDValue> &InVals) const {
  SelectionDAG &DAG = CLI.DAG;
  SDLoc &DL = CLI.DL;
  SmallVectorImpl<ISD::OutputArg> &Outs = CLI.Outs;
  SmallVectorImpl<SDValue> &OutVals = CLI.OutVals;
  SmallVectorImpl<ISD::InputArg> &Ins = CLI.Ins;
  SDValue Chain = CLI.Chain;
  SDValue Callee = CLI.Callee;
  bool &IsTailCall = CLI.IsTailCall;
  CallingConv::ID CallConv = CLI.CallConv;
  bool IsVarArg = CLI.IsVarArg;
  ArgListTy &Args = CLI.Args;

  // Dadao target does not yet support tail call optimization.
  IsTailCall = false;

  for (auto &out : CLI.Outs) {
    out.Flags.setInReg();
  }

  switch (CallConv) {
  case CallingConv::Fast:
  case CallingConv::C:
    return LowerCCCCallTo(Chain, Callee, CallConv, IsVarArg, IsTailCall, Outs,
                          OutVals, Ins, DL, DAG, InVals, Args);
  default:
    report_fatal_error("Unsupported calling convention");
  }
}

// LowerCCCArguments - transform physical registers into virtual registers and
// generate load operations for arguments places on the stack.
SDValue DadaoTargetLowering::LowerCCCArguments(
    SDValue Chain, CallingConv::ID CallConv, bool IsVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc &DL,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const {
  MachineFunction &MF = DAG.getMachineFunction();
  MachineFrameInfo &MFI = MF.getFrameInfo();
  MachineRegisterInfo &RegInfo = MF.getRegInfo();
  DadaoMachineFunctionInfo *DadaoMFI = MF.getInfo<DadaoMachineFunctionInfo>();

  // Assign locations to all of the incoming arguments.
  SmallVector<CCValAssign, 16> ArgLocs;
  // Assign locations to all of the incoming arguments.
  SmallVector<Type *, 4> ArgTypes;
  for (const Argument &Arg : MF.getFunction().args())
    ArgTypes.emplace_back(Arg.getType());
  DadaoCCState CCInfo(ArgTypes, CallConv, IsVarArg, MF, ArgLocs, *DAG.getContext());
  
  if (CallConv == CallingConv::Fast) {
    CCInfo.AnalyzeFormalArguments(Ins, CC_Dadao32_Fast);
  } else {
    CCInfo.AnalyzeFormalArguments(Ins, CC_Dadao32);
  }

  for (unsigned i = 0, e = ArgLocs.size(); i != e; ++i) {
    CCValAssign &VA = ArgLocs[i];
    if (VA.isRegLoc()) {
      // Arguments passed in registers
      EVT RegVT = VA.getLocVT();
      switch (RegVT.getSimpleVT().SimpleTy) {
      case MVT::i64: {
        Register VReg = RegInfo.createVirtualRegister(&Dadao::GPRDRegClass);
        RegInfo.addLiveIn(VA.getLocReg(), VReg);
        SDValue ArgValue = DAG.getCopyFromReg(Chain, DL, VReg, RegVT);

        // If this is an 8/16-bit value, it is really passed promoted to 32
        // bits. Insert an assert[sz]ext to capture this, then truncate to the
        // right size.
        if (VA.getLocInfo() == CCValAssign::SExt)
          ArgValue = DAG.getNode(ISD::AssertSext, DL, RegVT, ArgValue,
                                 DAG.getValueType(VA.getValVT()));
        else if (VA.getLocInfo() == CCValAssign::ZExt)
          ArgValue = DAG.getNode(ISD::AssertZext, DL, RegVT, ArgValue,
                                 DAG.getValueType(VA.getValVT()));

        if (VA.getLocInfo() != CCValAssign::Full)
          ArgValue = DAG.getNode(ISD::TRUNCATE, DL, VA.getValVT(), ArgValue);

        InVals.push_back(ArgValue);
        break;
      }
      default:
        LLVM_DEBUG(dbgs() << "LowerFormalArguments Unhandled argument type: "
                          << RegVT.getEVTString() << "\n");
        llvm_unreachable("unhandled argument type");
      }
    } else {
      // Only arguments passed on the stack should make it here.
      assert(VA.isMemLoc());
      // Load the argument to a virtual register
      unsigned ObjSize = VA.getLocVT().getSizeInBits() / 8;
      // Check that the argument fits in stack slot
      if (ObjSize > 4) {
        errs() << "LowerFormalArguments Unhandled argument type: "
               << EVT(VA.getLocVT()).getEVTString() << "\n";
      }
      // Create the frame index object for this incoming parameter...
      int FI = MFI.CreateFixedObject(ObjSize, VA.getLocMemOffset(), true);

      // Create the SelectionDAG nodes corresponding to a load
      // from this parameter
      SDValue FIN = DAG.getFrameIndex(FI, MVT::i64);
      InVals.push_back(DAG.getLoad(
          VA.getLocVT(), DL, Chain, FIN,
          MachinePointerInfo::getFixedStack(DAG.getMachineFunction(), FI)));
    }
  }

  // The Dadao ABI for returning structs by value requires that we copy
  // the sret argument into rdrv for the return. Save the argument into
  // a virtual register so that we can access it from the return points.
  if (MF.getFunction().hasStructRetAttr()) {
    Register Reg = DadaoMFI->getSRetReturnReg();
    if (!Reg) {
      Reg = MF.getRegInfo().createVirtualRegister(getRegClassFor(MVT::i64));
      DadaoMFI->setSRetReturnReg(Reg);
    }
    SDValue Copy = DAG.getCopyToReg(DAG.getEntryNode(), DL, Reg, InVals[0]);
    Chain = DAG.getNode(ISD::TokenFactor, DL, MVT::Other, Copy, Chain);
  }

  std::vector<SDValue> OutChains;
  if (IsVarArg) {
    int XLen = 64;
    int XLenInBytes = 8;
    MVT XLenVT = MVT::i64;
    ArrayRef<MCPhysReg> ArgRegs = ArrayRef(ArgGPRs);
    unsigned Idx = CCInfo.getFirstUnallocated(ArgRegs);
    Idx = ArgRegs.size();
    const TargetRegisterClass *RC = &Dadao::GPRDRegClass;

    // Offset of the first variable argument from stack pointer, and size of
    // the vararg save area. For now, the varargs save area is either zero or
    // large enough to hold rd16-rd31.
    int VaArgOffset, VarArgsSaveSize;

    // If all registers are allocated, then all varargs must be passed on the
    // stack and we don't need to save any argregs.
    if (ArgRegs.size() == Idx) {
      VaArgOffset = CCInfo.getNextStackOffset();
      VarArgsSaveSize = 0;
    } else {
      VarArgsSaveSize = XLenInBytes * (ArgRegs.size() - Idx);
      VaArgOffset = -VarArgsSaveSize;
    }

    // Record the frame index of the first variable argument
    // which is a value necessary to VASTART.
    int FI = MFI.CreateFixedObject(XLenInBytes, VaArgOffset, true);
    DadaoMFI->setVarArgsFrameIndex(FI);

    // Copy the integer registers that may have been used for passing varargs
    // to the vararg save area.
    for (unsigned I = Idx; I < ArgRegs.size();
         ++I, VaArgOffset += XLenInBytes) {
      const Register Reg = RegInfo.createVirtualRegister(RC);
      RegInfo.addLiveIn(ArgRegs[I], Reg);
      SDValue ArgValue = DAG.getCopyFromReg(Chain, DL, Reg, XLenVT);
      FI = MFI.CreateFixedObject(XLenInBytes, VaArgOffset, true);
      SDValue PtrOff = DAG.getFrameIndex(FI, getPointerTy(DAG.getDataLayout()));
      SDValue Store = DAG.getStore(Chain, DL, ArgValue, PtrOff,
                                   MachinePointerInfo::getFixedStack(MF, FI));
      cast<StoreSDNode>(Store.getNode())
          ->getMemOperand()
          ->setValue((Value *)nullptr);
      OutChains.push_back(Store);
    }
    DadaoMFI->setVarArgsSaveSize(VarArgsSaveSize);
  }
  
  if (!OutChains.empty()) {
    OutChains.push_back(Chain);
    Chain = DAG.getNode(ISD::TokenFactor, DL, MVT::Other, OutChains);
  }

  return Chain;
}

bool DadaoTargetLowering::CanLowerReturn(
    CallingConv::ID CallConv, MachineFunction &MF, bool IsVarArg,
    const SmallVectorImpl<ISD::OutputArg> &Outs, LLVMContext &Context) const {
  SmallVector<CCValAssign, 16> RVLocs;
  CCState CCInfo(CallConv, IsVarArg, MF, RVLocs, Context);

  return CCInfo.CheckReturn(Outs, RetCC_Dadao32);
}

SDValue
DadaoTargetLowering::LowerReturn(SDValue Chain, CallingConv::ID CallConv,
                                 bool IsVarArg,
                                 const SmallVectorImpl<ISD::OutputArg> &Outs,
                                 const SmallVectorImpl<SDValue> &OutVals,
                                 const SDLoc &DL, SelectionDAG &DAG) const {
  // CCValAssign - represent the assignment of the return value to a location
  SmallVector<CCValAssign, 16> RVLocs;

  // CCState - Info about the registers and stack slot.
  CCState CCInfo(CallConv, IsVarArg, DAG.getMachineFunction(), RVLocs,
                 *DAG.getContext());

  // Analize return values.
  CCInfo.AnalyzeReturn(Outs, RetCC_Dadao32);

  SDValue Flag;
  SmallVector<SDValue, 4> RetOps(1, Chain);

  // Copy the result values into the output registers.
  for (unsigned i = 0; i != RVLocs.size(); ++i) {
    CCValAssign &VA = RVLocs[i];
    assert(VA.isRegLoc() && "Can only return in registers!");

    Chain = DAG.getCopyToReg(Chain, DL, VA.getLocReg(), OutVals[i], Flag);

    // Guarantee that all emitted copies are stuck together with flags.
    Flag = Chain.getValue(1);
    RetOps.push_back(DAG.getRegister(VA.getLocReg(), VA.getLocVT()));
  }

  // The Dadao ABI for returning structs by value requires that we copy
  // the sret argument into rdrv for the return. We saved the argument into
  // a virtual register in the entry block, so now we copy the value out
  // and into rdrv.
  if (DAG.getMachineFunction().getFunction().hasStructRetAttr()) {
    MachineFunction &MF = DAG.getMachineFunction();
    DadaoMachineFunctionInfo *DadaoMFI = MF.getInfo<DadaoMachineFunctionInfo>();
    Register Reg = DadaoMFI->getSRetReturnReg();
    assert(Reg &&
           "SRetReturnReg should have been set in LowerFormalArguments().");
    SDValue Val =
        DAG.getCopyFromReg(Chain, DL, Reg, getPointerTy(DAG.getDataLayout()));

    Chain = DAG.getCopyToReg(Chain, DL, Dadao::RDRV, Val, Flag);
    Flag = Chain.getValue(1);
    RetOps.push_back(
        DAG.getRegister(Dadao::RDRV, getPointerTy(DAG.getDataLayout())));
  }

  RetOps[0] = Chain; // Update chain

  unsigned Opc = DadaoISD::RET_FLAG;
  if (Flag.getNode())
    RetOps.push_back(Flag);

  // Return Void
  return DAG.getNode(Opc, DL, MVT::Other,
                     ArrayRef<SDValue>(&RetOps[0], RetOps.size()));
}

// LowerCCCCallTo - functions arguments are copied from virtual regs to
// (physical regs)/(stack frame), CALLSEQ_START and CALLSEQ_END are emitted.
SDValue DadaoTargetLowering::LowerCCCCallTo(
    SDValue Chain, SDValue Callee, CallingConv::ID CallConv, bool IsVarArg,
    bool /*IsTailCall*/, const SmallVectorImpl<ISD::OutputArg> &Outs,
    const SmallVectorImpl<SDValue> &OutVals,
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc &DL,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals, ArgListTy &Args) const {
  MachineFunction &MF = DAG.getMachineFunction();
  MachineFrameInfo &MFI = MF.getFrameInfo();

  // Assign locations to all of the incoming arguments.
  SmallVector<CCValAssign, 16> ArgLocs;
  // Assign locations to all of the incoming arguments.
  SmallVector<Type *, 4> ArgTypes;
  for (const auto &Arg : Args)
    ArgTypes.emplace_back(Arg.Ty);
  DadaoCCState CCInfo(ArgTypes, CallConv, IsVarArg, MF, ArgLocs, *DAG.getContext());
  // Analyze operands of the call, assigning locations to each operand.
  GlobalAddressSDNode *G = dyn_cast<GlobalAddressSDNode>(Callee);

  NumFixedArgs = 0;
  bool isPrintf = false;
  if (IsVarArg && G) {
    const Function *CalleeFn = dyn_cast<Function>(G->getGlobal());
    StringRef Name = CalleeFn->getName();
    StringRef PrintfName = StringRef("printf", 6);
    if (Name.equals(PrintfName)) {
      isPrintf = true;
    }
    if (CalleeFn)
      NumFixedArgs = CalleeFn->getFunctionType()->getNumParams();
  }
  if (NumFixedArgs && !isPrintf)
    CCInfo.AnalyzeCallOperands(Outs, CC_Dadao32_VarArg);
  else {
    if (CallConv == CallingConv::Fast)
      CCInfo.AnalyzeCallOperands(Outs, CC_Dadao32_Fast);
    else
      CCInfo.AnalyzeCallOperands(Outs, CC_Dadao32);
  }

  // Get a count of how many bytes are to be pushed on the stack.
  unsigned NumBytes = CCInfo.getNextStackOffset();

  // Create local copies for byval args.
  SmallVector<SDValue, 8> ByValArgs;
  for (unsigned I = 0, E = Outs.size(); I != E; ++I) {
    ISD::ArgFlagsTy Flags = Outs[I].Flags;
    if (!Flags.isByVal())
      continue;

    SDValue Arg = OutVals[I];
    unsigned Size = Flags.getByValSize();
    Align Alignment = Flags.getNonZeroByValAlign();

    int FI = MFI.CreateStackObject(Size, Alignment, false);
    SDValue FIPtr = DAG.getFrameIndex(FI, getPointerTy(DAG.getDataLayout()));
    SDValue SizeNode = DAG.getConstant(Size, DL, MVT::i64);

    Chain = DAG.getMemcpy(Chain, DL, FIPtr, Arg, SizeNode, Alignment,
                          /*IsVolatile=*/false,
                          /*AlwaysInline=*/false,
                          /*isTailCall=*/false, MachinePointerInfo(),
                          MachinePointerInfo());
    ByValArgs.push_back(FIPtr);
  }

  Chain = DAG.getCALLSEQ_START(Chain, NumBytes, 0, DL);

  SmallVector<std::pair<unsigned, SDValue>, 4> RegsToPass;
  SmallVector<SDValue, 12> MemOpChains;
  SDValue StackPtr;

  // Walk the register/memloc assignments, inserting copies/loads.
  for (unsigned I = 0, J = 0, E = ArgLocs.size(); I != E; ++I) {
    CCValAssign &VA = ArgLocs[I];
    SDValue Arg = OutVals[I];
    ISD::ArgFlagsTy Flags = Outs[I].Flags;

    // Promote the value if needed.
    switch (VA.getLocInfo()) {
    case CCValAssign::Full:
      break;
    case CCValAssign::SExt:
      Arg = DAG.getNode(ISD::SIGN_EXTEND, DL, VA.getLocVT(), Arg);
      break;
    case CCValAssign::ZExt:
      Arg = DAG.getNode(ISD::ZERO_EXTEND, DL, VA.getLocVT(), Arg);
      break;
    case CCValAssign::AExt:
      Arg = DAG.getNode(ISD::ANY_EXTEND, DL, VA.getLocVT(), Arg);
      break;
    default:
      llvm_unreachable("Unknown loc info!");
    }

    // Use local copy if it is a byval arg.
    if (Flags.isByVal())
      Arg = ByValArgs[J++];

    // Arguments that can be passed on register must be kept at RegsToPass
    // vector
    if (VA.isRegLoc()) {
      RegsToPass.push_back(std::make_pair(VA.getLocReg(), Arg));
    } else {
      assert(VA.isMemLoc());

      if (StackPtr.getNode() == nullptr)
        StackPtr = DAG.getCopyFromReg(Chain, DL, Dadao::RBSP,
                                      getPointerTy(DAG.getDataLayout()));

      SDValue PtrOff =
          DAG.getNode(ISD::ADD, DL, getPointerTy(DAG.getDataLayout()), StackPtr,
                      DAG.getIntPtrConstant(VA.getLocMemOffset(), DL));

      MemOpChains.push_back(
          DAG.getStore(Chain, DL, Arg, PtrOff, MachinePointerInfo()));
    }
  }

  // Transform all store nodes into one single node because all store nodes are
  // independent of each other.
  if (!MemOpChains.empty())
    Chain = DAG.getNode(ISD::TokenFactor, DL, MVT::Other,
                        ArrayRef<SDValue>(&MemOpChains[0], MemOpChains.size()));

  SDValue InFlag;

  // Build a sequence of copy-to-reg nodes chained together with token chain and
  // flag operands which copy the outgoing args into registers.  The InFlag in
  // necessary since all emitted instructions must be stuck together.
  for (unsigned I = 0, E = RegsToPass.size(); I != E; ++I) {
    Chain = DAG.getCopyToReg(Chain, DL, RegsToPass[I].first,
                             RegsToPass[I].second, InFlag);
    InFlag = Chain.getValue(1);
  }

  // If the callee is a GlobalAddress node (quite common, every direct call is)
  // turn it into a TargetGlobalAddress node so that legalize doesn't hack it.
  // Likewise ExternalSymbol -> TargetExternalSymbol.
  uint8_t OpFlag = DadaoII::MO_NO_FLAG;
  if (G) {
    Callee = DAG.getTargetGlobalAddress(
        G->getGlobal(), DL, getPointerTy(DAG.getDataLayout()), 0, OpFlag);
  } else if (ExternalSymbolSDNode *E = dyn_cast<ExternalSymbolSDNode>(Callee)) {
    Callee = DAG.getTargetExternalSymbol(
        E->getSymbol(), getPointerTy(DAG.getDataLayout()), OpFlag);
  }

  // Returns a chain & a flag for retval copy to use.
  SDVTList NodeTys = DAG.getVTList(MVT::Other, MVT::Glue);
  SmallVector<SDValue, 8> Ops;
  Ops.push_back(Chain);
  Ops.push_back(Callee);

  // Add a register mask operand representing the call-preserved registers.
  // TODO: Should return-twice functions be handled?
  const uint32_t *Mask =
      TRI->getCallPreservedMask(DAG.getMachineFunction(), CallConv);
  assert(Mask && "Missing call preserved mask for calling convention");
  Ops.push_back(DAG.getRegisterMask(Mask));

  // Add argument registers to the end of the list so that they are
  // known live into the call.
  for (unsigned I = 0, E = RegsToPass.size(); I != E; ++I)
    Ops.push_back(DAG.getRegister(RegsToPass[I].first,
                                  RegsToPass[I].second.getValueType()));

  if (InFlag.getNode())
    Ops.push_back(InFlag);

  Chain = DAG.getNode(DadaoISD::CALL, DL, NodeTys,
                      ArrayRef<SDValue>(&Ops[0], Ops.size()));
  InFlag = Chain.getValue(1);

  // Create the CALLSEQ_END node.
  Chain = DAG.getCALLSEQ_END(Chain, NumBytes, 0, InFlag, DL);
  InFlag = Chain.getValue(1);

  // Handle result values, copying them out of physregs into vregs that we
  // return.
  return LowerCallResult(Chain, InFlag, CallConv, IsVarArg, Ins, DL, DAG,
                         InVals);
}

// LowerCallResult - Lower the result values of a call into the
// appropriate copies out of appropriate physical registers.
SDValue DadaoTargetLowering::LowerCallResult(
    SDValue Chain, SDValue InFlag, CallingConv::ID CallConv, bool IsVarArg,
    const SmallVectorImpl<ISD::InputArg> &Ins, const SDLoc &DL,
    SelectionDAG &DAG, SmallVectorImpl<SDValue> &InVals) const {
  // Assign locations to each value returned by this call.
  SmallVector<CCValAssign, 16> RVLocs;
  CCState CCInfo(CallConv, IsVarArg, DAG.getMachineFunction(), RVLocs,
                 *DAG.getContext());

  CCInfo.AnalyzeCallResult(Ins, RetCC_Dadao32);

  // Copy all of the result registers out of their specified physreg.
  for (unsigned I = 0; I != RVLocs.size(); ++I) {
    Chain = DAG.getCopyFromReg(Chain, DL, RVLocs[I].getLocReg(),
                               RVLocs[I].getValVT(), InFlag)
                .getValue(1);
    InFlag = Chain.getValue(2);
    InVals.push_back(Chain.getValue(0));
  }

  return Chain;
}

//===----------------------------------------------------------------------===//
//                      Custom Lowerings
//===----------------------------------------------------------------------===//

static LPCC::CondCode IntCondCCodeToICC(SDValue CC, const SDLoc &DL,
                                        SDValue &RHS, SelectionDAG &DAG) {
  ISD::CondCode SetCCOpcode = cast<CondCodeSDNode>(CC)->get();

  // For integer, only the SETEQ, SETNE, SETLT, SETLE, SETGT, SETGE, SETULT,
  // SETULE, SETUGT, and SETUGE opcodes are used (see CodeGen/ISDOpcodes.h)
  // and Dadao only supports integer comparisons, so only provide definitions
  // for them.
  switch (SetCCOpcode) {
  case ISD::SETEQ:
    return LPCC::ICC_EQ;
  case ISD::SETGT:
    if (ConstantSDNode *RHSC = dyn_cast<ConstantSDNode>(RHS))
      if (RHSC->getZExtValue() == 0xFFFFFFFF) {
        // X > -1 -> X >= 0 -> is_plus(X)
        RHS = DAG.getConstant(0, DL, RHS.getValueType());
        return LPCC::ICC_PL;
      }
    return LPCC::ICC_GT;
  case ISD::SETUGT:
    return LPCC::ICC_UGT;
  case ISD::SETLT:
    if (ConstantSDNode *RHSC = dyn_cast<ConstantSDNode>(RHS))
      if (RHSC->getZExtValue() == 0)
        // X < 0 -> is_minus(X)
        return LPCC::ICC_MI;
    return LPCC::ICC_LT;
  case ISD::SETULT:
    return LPCC::ICC_ULT;
  case ISD::SETLE:
    if (ConstantSDNode *RHSC = dyn_cast<ConstantSDNode>(RHS))
      if (RHSC->getZExtValue() == 0xFFFFFFFF) {
        // X <= -1 -> X < 0 -> is_minus(X)
        RHS = DAG.getConstant(0, DL, RHS.getValueType());
        return LPCC::ICC_MI;
      }
    return LPCC::ICC_LE;
  case ISD::SETULE:
    return LPCC::ICC_ULE;
  case ISD::SETGE:
    if (ConstantSDNode *RHSC = dyn_cast<ConstantSDNode>(RHS))
      if (RHSC->getZExtValue() == 0)
        // X >= 0 -> is_plus(X)
        return LPCC::ICC_PL;
    return LPCC::ICC_GE;
  case ISD::SETUGE:
    return LPCC::ICC_UGE;
  case ISD::SETNE:
    return LPCC::ICC_NE;
  case ISD::SETONE:
  case ISD::SETUNE:
  case ISD::SETOGE:
  case ISD::SETOLE:
  case ISD::SETOLT:
  case ISD::SETOGT:
  case ISD::SETOEQ:
  case ISD::SETUEQ:
  case ISD::SETO:
  case ISD::SETUO:
    llvm_unreachable("Unsupported comparison.");
  default:
    llvm_unreachable("Unknown integer condition code!");
  }
}

SDValue DadaoTargetLowering::LowerBR_CC(SDValue Op, SelectionDAG &DAG) const {
  SDValue Chain = Op.getOperand(0);
  SDValue Cond = Op.getOperand(1);
  SDValue LHS = Op.getOperand(2);
  SDValue RHS = Op.getOperand(3);
  SDValue Dest = Op.getOperand(4);
  SDLoc DL(Op);

  LPCC::CondCode CC = IntCondCCodeToICC(Cond, DL, RHS, DAG);
  SDValue TargetCC = DAG.getConstant(CC, DL, MVT::i64);

  return DAG.getNode(DadaoISD::BR_CC, DL, Op.getValueType(), Chain, TargetCC,
                     LHS, RHS, Dest);
}

SDValue DadaoTargetLowering::LowerSETCC(SDValue Op, SelectionDAG &DAG) const {
  SDValue LHS = Op.getOperand(0);
  SDValue RHS = Op.getOperand(1);
  SDValue Cond = Op.getOperand(2);
  SDLoc DL(Op);

  LPCC::CondCode CC = IntCondCCodeToICC(Cond, DL, RHS, DAG);
  SDValue TargetCC = DAG.getConstant(CC, DL, MVT::i64);

  return DAG.getNode(DadaoISD::SETCC, DL, Op.getValueType(), LHS, RHS, TargetCC);
}

SDValue DadaoTargetLowering::LowerSELECT_CC(SDValue Op,
                                            SelectionDAG &DAG) const {
  SDValue LHS = Op.getOperand(0);
  SDValue RHS = Op.getOperand(1);
  SDValue TrueV = Op.getOperand(2);
  SDValue FalseV = Op.getOperand(3);
  SDValue Cond = Op.getOperand(4);
  SDLoc DL(Op);

  LPCC::CondCode CC = IntCondCCodeToICC(Cond, DL, RHS, DAG);
  SDValue TargetCC = DAG.getConstant(CC, DL, MVT::i64);
  return DAG.getNode(DadaoISD::SELECT_CC, DL, Op.getValueType(), LHS, RHS, TrueV, FalseV, TargetCC);
}

SDValue DadaoTargetLowering::LowerVASTART(SDValue Op, SelectionDAG &DAG) const {
  MachineFunction &MF = DAG.getMachineFunction();
  DadaoMachineFunctionInfo *FuncInfo = MF.getInfo<DadaoMachineFunctionInfo>();

  SDLoc DL(Op);
  SDValue FI = DAG.getFrameIndex(FuncInfo->getVarArgsFrameIndex(),
                                 getPointerTy(DAG.getDataLayout()));

  // vastart just stores the address of the VarArgsFrameIndex slot into the
  // memory location argument.
  const Value *SV = cast<SrcValueSDNode>(Op.getOperand(2))->getValue();
  return DAG.getStore(Op.getOperand(0), DL, FI, Op.getOperand(1),
                      MachinePointerInfo(SV));
}

SDValue DadaoTargetLowering::LowerDYNAMIC_STACKALLOC(SDValue Op,
                                                     SelectionDAG &DAG) const {
  SDValue Chain = Op.getOperand(0);
  SDValue Size = Op.getOperand(1);
  SDLoc DL(Op);

  Register SPReg = getStackPointerRegisterToSaveRestore();

  // Get a reference to the stack pointer.
  SDValue StackPointer = DAG.getCopyFromReg(Chain, DL, SPReg, MVT::i64);

  // Subtract the dynamic size from the actual stack size to
  // obtain the new stack size.
  SDValue Sub = DAG.getNode(ISD::SUB, DL, MVT::i64, StackPointer, Size);

  // For Dadao, the outgoing memory arguments area should be on top of the
  // alloca area on the stack i.e., the outgoing memory arguments should be
  // at a lower address than the alloca area. Move the alloca area down the
  // stack by adding back the space reserved for outgoing arguments to RBSP
  // here.
  //
  // We do not know what the size of the outgoing args is at this point.
  // So, we add a pseudo instruction ADJDYNALLOC that will adjust the
  // stack pointer. We replace this instruction with on that has the correct,
  // known offset in emitPrologue().
  SDValue ArgAdjust = DAG.getNode(DadaoISD::ADJDYNALLOC, DL, MVT::i64, Sub);

  // The Sub result contains the new stack start address, so it
  // must be placed in the stack pointer register.
  SDValue CopyChain = DAG.getCopyToReg(Chain, DL, SPReg, Sub);

  SDValue Ops[2] = {ArgAdjust, CopyChain};
  return DAG.getMergeValues(Ops, DL);
}

SDValue DadaoTargetLowering::LowerRETURNADDR(SDValue Op,
                                             SelectionDAG &DAG) const {
  MachineFunction &MF = DAG.getMachineFunction();
  MachineFrameInfo &MFI = MF.getFrameInfo();
  MFI.setReturnAddressIsTaken(true);

  EVT VT = Op.getValueType();
  SDLoc DL(Op);
  unsigned Depth = cast<ConstantSDNode>(Op.getOperand(0))->getZExtValue();
  if (Depth) {
    SDValue FrameAddr = LowerFRAMEADDR(Op, DAG);
    const unsigned Offset = -4;
    SDValue Ptr = DAG.getNode(ISD::ADD, DL, VT, FrameAddr,
                              DAG.getIntPtrConstant(Offset, DL));
    return DAG.getLoad(VT, DL, DAG.getEntryNode(), Ptr, MachinePointerInfo());
  }

  // Return the link register, which contains the return address.
  // Mark it an implicit live-in.
  Register Reg = MF.addLiveIn(TRI->getRARegister(), getRegClassFor(MVT::i64));
  return DAG.getCopyFromReg(DAG.getEntryNode(), DL, Reg, VT);
}

SDValue DadaoTargetLowering::LowerFRAMEADDR(SDValue Op,
                                            SelectionDAG &DAG) const {
  MachineFrameInfo &MFI = DAG.getMachineFunction().getFrameInfo();
  MFI.setFrameAddressIsTaken(true);

  EVT VT = Op.getValueType();
  SDLoc DL(Op);
  SDValue FrameAddr = DAG.getCopyFromReg(DAG.getEntryNode(), DL, Dadao::RBFP, VT);
  unsigned Depth = cast<ConstantSDNode>(Op.getOperand(0))->getZExtValue();
  while (Depth--) {
    const unsigned Offset = -8;
    SDValue Ptr = DAG.getNode(ISD::ADD, DL, VT, FrameAddr,
                              DAG.getIntPtrConstant(Offset, DL));
    FrameAddr =
        DAG.getLoad(VT, DL, DAG.getEntryNode(), Ptr, MachinePointerInfo());
  }
  return FrameAddr;
}

const char *DadaoTargetLowering::getTargetNodeName(unsigned Opcode) const {
  switch (Opcode) {
  case DadaoISD::ADJDYNALLOC:
    return "DadaoISD::ADJDYNALLOC";
  case DadaoISD::RET_FLAG:
    return "DadaoISD::RET_FLAG";
  case DadaoISD::CALL:
    return "DadaoISD::CALL";
  case DadaoISD::SELECT_CC:
    return "DadaoISD::SELECT_CC";
  case DadaoISD::SETCC:
    return "DadaoISD::SETCC";
  case DadaoISD::SUBBF:
    return "DadaoISD::SUBBF";
  case DadaoISD::BR_CC:
    return "DadaoISD::BR_CC";
  case DadaoISD::Wrapper:
    return "DadaoISD::Wrapper";
  case DadaoISD::HI:
    return "DadaoISD::HI";
  case DadaoISD::LO:
    return "DadaoISD::LO";
  default:
    return nullptr;
  }
}

SDValue DadaoTargetLowering::LowerBlockAddress(SDValue Op,
                                               SelectionDAG &DAG) const {
  SDLoc DL(Op);
  const BlockAddress *BA = cast<BlockAddressSDNode>(Op)->getBlockAddress();

  uint8_t OpFlagHi = DadaoII::MO_ABS_HI;
  uint8_t OpFlagLo = DadaoII::MO_ABS_LO;

  SDValue Hi = DAG.getBlockAddress(BA, MVT::i64, true, OpFlagHi);
  SDValue Lo = DAG.getBlockAddress(BA, MVT::i64, true, OpFlagLo);
  Hi = DAG.getNode(DadaoISD::HI, DL, MVT::i64, Hi);
  Lo = DAG.getNode(DadaoISD::LO, DL, MVT::i64, Lo);
  SDValue Result = DAG.getNode(ISD::OR, DL, MVT::i64, Hi, Lo);
  return Result;
}

SDValue DadaoTargetLowering::LowerJumpTable(SDValue Op,
                                            SelectionDAG &DAG) const {
  SDLoc DL(Op);
  JumpTableSDNode *JT = cast<JumpTableSDNode>(Op);

  uint8_t OpFlagHi = DadaoII::MO_ABS_HI;
  uint8_t OpFlagLo = DadaoII::MO_ABS_LO;

  SDValue Hi = DAG.getTargetJumpTable(
      JT->getIndex(), getPointerTy(DAG.getDataLayout()), OpFlagHi);
  SDValue Lo = DAG.getTargetJumpTable(
      JT->getIndex(), getPointerTy(DAG.getDataLayout()), OpFlagLo);
  Hi = DAG.getNode(DadaoISD::HI, DL, MVT::i64, Hi);
  Lo = DAG.getNode(DadaoISD::LO, DL, MVT::i64, Lo);
  SDValue Result = DAG.getNode(ISD::OR, DL, MVT::i64, Hi, Lo);
  return Result;
}

SDValue DadaoTargetLowering::LowerSHL_PARTS(SDValue Op,
                                            SelectionDAG &DAG) const {
  EVT VT = Op.getValueType();
  unsigned VTBits = VT.getSizeInBits();
  SDLoc dl(Op);
  assert(Op.getNumOperands() == 3 && "Unexpected SHL!");
  SDValue ShOpLo = Op.getOperand(0);
  SDValue ShOpHi = Op.getOperand(1);
  SDValue ShAmt = Op.getOperand(2);

  // Performs the following for (ShOpLo + (ShOpHi << 32)) << ShAmt:
  //   LoBitsForHi = (ShAmt == 0) ? 0 : (ShOpLo >> (32-ShAmt))
  //   HiBitsForHi = ShOpHi << ShAmt
  //   Hi = (ShAmt >= 32) ? (ShOpLo << (ShAmt-32)) : (LoBitsForHi | HiBitsForHi)
  //   Lo = (ShAmt >= 32) ? 0 : (ShOpLo << ShAmt)
  //   return (Hi << 32) | Lo;

  SDValue RevShAmt = DAG.getNode(ISD::SUB, dl, MVT::i64,
                                 DAG.getConstant(VTBits, dl, MVT::i64), ShAmt);
  SDValue LoBitsForHi = DAG.getNode(ISD::SRL, dl, VT, ShOpLo, RevShAmt);

  // If ShAmt == 0, we just calculated "(SRL ShOpLo, 32)" which is "undef". We
  // wanted 0, so CSEL it directly.
  SDValue Zero = DAG.getConstant(0, dl, MVT::i64);
  SDValue SetCC = DAG.getSetCC(dl, MVT::i64, ShAmt, Zero, ISD::SETEQ);
  LoBitsForHi = DAG.getSelect(dl, MVT::i64, SetCC, Zero, LoBitsForHi);

  SDValue ExtraShAmt = DAG.getNode(ISD::SUB, dl, MVT::i64, ShAmt,
                                   DAG.getConstant(VTBits, dl, MVT::i64));
  SDValue HiBitsForHi = DAG.getNode(ISD::SHL, dl, VT, ShOpHi, ShAmt);
  SDValue HiForNormalShift =
      DAG.getNode(ISD::OR, dl, VT, LoBitsForHi, HiBitsForHi);

  SDValue HiForBigShift = DAG.getNode(ISD::SHL, dl, VT, ShOpLo, ExtraShAmt);

  SetCC = DAG.getSetCC(dl, MVT::i64, ExtraShAmt, Zero, ISD::SETGE);
  SDValue Hi =
      DAG.getSelect(dl, MVT::i64, SetCC, HiForBigShift, HiForNormalShift);

  // Dadao shifts of larger than register sizes are wrapped rather than
  // clamped, so we can't just emit "lo << b" if b is too big.
  SDValue LoForNormalShift = DAG.getNode(ISD::SHL, dl, VT, ShOpLo, ShAmt);
  SDValue Lo = DAG.getSelect(
      dl, MVT::i64, SetCC, DAG.getConstant(0, dl, MVT::i64), LoForNormalShift);

  SDValue Ops[2] = {Lo, Hi};
  return DAG.getMergeValues(Ops, dl);
}

SDValue DadaoTargetLowering::LowerSRL_PARTS(SDValue Op,
                                            SelectionDAG &DAG) const {
  MVT VT = Op.getSimpleValueType();
  unsigned VTBits = VT.getSizeInBits();
  SDLoc dl(Op);
  SDValue ShOpLo = Op.getOperand(0);
  SDValue ShOpHi = Op.getOperand(1);
  SDValue ShAmt = Op.getOperand(2);

  // Performs the following for a >> b:
  //   unsigned r_high = a_high >> b;
  //   r_high = (32 - b <= 0) ? 0 : r_high;
  //
  //   unsigned r_low = a_low >> b;
  //   r_low = (32 - b <= 0) ? r_high : r_low;
  //   r_low = (b == 0) ? r_low : r_low | (a_high << (32 - b));
  //   return (unsigned long long)r_high << 32 | r_low;
  // Note: This takes advantage of Dadao's shift behavior to avoid needing to
  // mask the shift amount.

  SDValue Zero = DAG.getConstant(0, dl, MVT::i64);
  SDValue NegatedPlus32 = DAG.getNode(
      ISD::SUB, dl, MVT::i64, DAG.getConstant(VTBits, dl, MVT::i64), ShAmt);
  SDValue SetCC = DAG.getSetCC(dl, MVT::i64, NegatedPlus32, Zero, ISD::SETLE);

  SDValue Hi = DAG.getNode(ISD::SRL, dl, MVT::i64, ShOpHi, ShAmt);
  Hi = DAG.getSelect(dl, MVT::i64, SetCC, Zero, Hi);

  SDValue Lo = DAG.getNode(ISD::SRL, dl, MVT::i64, ShOpLo, ShAmt);
  Lo = DAG.getSelect(dl, MVT::i64, SetCC, Hi, Lo);
  SDValue CarryBits =
      DAG.getNode(ISD::SHL, dl, MVT::i64, ShOpHi, NegatedPlus32);
  SDValue ShiftIsZero = DAG.getSetCC(dl, MVT::i64, ShAmt, Zero, ISD::SETEQ);
  Lo = DAG.getSelect(dl, MVT::i64, ShiftIsZero, Lo,
                     DAG.getNode(ISD::OR, dl, MVT::i64, Lo, CarryBits));

  SDValue Ops[2] = {Lo, Hi};
  return DAG.getMergeValues(Ops, dl);
}

// Helper function that checks if N is a null or all ones constant.
static inline bool isZeroOrAllOnes(SDValue N, bool AllOnes) {
  return AllOnes ? isAllOnesConstant(N) : isNullConstant(N);
}

// Return true if N is conditionally 0 or all ones.
// Detects these expressions where cc is an i1 value:
//
//   (select cc 0, y)   [AllOnes=0]
//   (select cc y, 0)   [AllOnes=0]
//   (zext cc)          [AllOnes=0]
//   (sext cc)          [AllOnes=0/1]
//   (select cc -1, y)  [AllOnes=1]
//   (select cc y, -1)  [AllOnes=1]
//
// * AllOnes determines whether to check for an all zero (AllOnes false) or an
//   all ones operand (AllOnes true).
// * Invert is set when N is the all zero/ones constant when CC is false.
// * OtherOp is set to the alternative value of N.
//
// For example, for (select cc X, Y) and AllOnes = 0 if:
// * X = 0, Invert = False and OtherOp = Y
// * Y = 0, Invert = True and OtherOp = X
static bool isConditionalZeroOrAllOnes(SDNode *N, bool AllOnes, SDValue &CC,
                                       bool &Invert, SDValue &OtherOp,
                                       SelectionDAG &DAG) {
  switch (N->getOpcode()) {
  default:
    return false;
  case ISD::SELECT: {
    CC = N->getOperand(0);
    SDValue N1 = N->getOperand(1);
    SDValue N2 = N->getOperand(2);
    if (isZeroOrAllOnes(N1, AllOnes)) {
      Invert = false;
      OtherOp = N2;
      return true;
    }
    if (isZeroOrAllOnes(N2, AllOnes)) {
      Invert = true;
      OtherOp = N1;
      return true;
    }
    return false;
  }
  case ISD::ZERO_EXTEND: {
    // (zext cc) can never be the all ones value.
    if (AllOnes)
      return false;
    CC = N->getOperand(0);
    if (CC.getValueType() != MVT::i1)
      return false;
    SDLoc dl(N);
    EVT VT = N->getValueType(0);
    OtherOp = DAG.getConstant(1, dl, VT);
    Invert = true;
    return true;
  }
  case ISD::SIGN_EXTEND: {
    CC = N->getOperand(0);
    if (CC.getValueType() != MVT::i1)
      return false;
    SDLoc dl(N);
    EVT VT = N->getValueType(0);
    Invert = !AllOnes;
    if (AllOnes)
      // When looking for an AllOnes constant, N is an sext, and the 'other'
      // value is 0.
      OtherOp = DAG.getConstant(0, dl, VT);
    else
      OtherOp = DAG.getAllOnesConstant(dl, VT);
    return true;
  }
  }
}

// Combine a constant select operand into its use:
//
//   (add (select cc, 0, c), x)  -> (select cc, x, (add, x, c))
//   (sub x, (select cc, 0, c))  -> (select cc, x, (sub, x, c))
//   (and (select cc, -1, c), x) -> (select cc, x, (and, x, c))  [AllOnes=1]
//   (or  (select cc, 0, c), x)  -> (select cc, x, (or, x, c))
//   (xor (select cc, 0, c), x)  -> (select cc, x, (xor, x, c))
//
// The transform is rejected if the select doesn't have a constant operand that
// is null, or all ones when AllOnes is set.
//
// Also recognize sext/zext from i1:
//
//   (add (zext cc), x) -> (select cc (add x, 1), x)
//   (add (sext cc), x) -> (select cc (add x, -1), x)
//
// These transformations eventually create predicated instructions.
static SDValue combineSelectAndUse(SDNode *N, SDValue Slct, SDValue OtherOp,
                                   TargetLowering::DAGCombinerInfo &DCI,
                                   bool AllOnes) {
  SelectionDAG &DAG = DCI.DAG;
  EVT VT = N->getValueType(0);
  SDValue NonConstantVal;
  SDValue CCOp;
  bool SwapSelectOps;
  if (!isConditionalZeroOrAllOnes(Slct.getNode(), AllOnes, CCOp, SwapSelectOps,
                                  NonConstantVal, DAG))
    return SDValue();

  // Slct is now know to be the desired identity constant when CC is true.
  SDValue TrueVal = OtherOp;
  SDValue FalseVal =
      DAG.getNode(N->getOpcode(), SDLoc(N), VT, OtherOp, NonConstantVal);
  // Unless SwapSelectOps says CC should be false.
  if (SwapSelectOps)
    std::swap(TrueVal, FalseVal);

  return DAG.getNode(ISD::SELECT, SDLoc(N), VT, CCOp, TrueVal, FalseVal);
}

// Attempt combineSelectAndUse on each operand of a commutative operator N.
static SDValue
combineSelectAndUseCommutative(SDNode *N, TargetLowering::DAGCombinerInfo &DCI,
                               bool AllOnes) {
  SDValue N0 = N->getOperand(0);
  SDValue N1 = N->getOperand(1);
  if (N0.getNode()->hasOneUse())
    if (SDValue Result = combineSelectAndUse(N, N0, N1, DCI, AllOnes))
      return Result;
  if (N1.getNode()->hasOneUse())
    if (SDValue Result = combineSelectAndUse(N, N1, N0, DCI, AllOnes))
      return Result;
  return SDValue();
}

// PerformSUBCombine - Target-specific dag combine xforms for ISD::SUB.
static SDValue PerformSUBCombine(SDNode *N,
                                 TargetLowering::DAGCombinerInfo &DCI) {
  SDValue N0 = N->getOperand(0);
  SDValue N1 = N->getOperand(1);

  // fold (sub x, (select cc, 0, c)) -> (select cc, x, (sub, x, c))
  if (N1.getNode()->hasOneUse())
    if (SDValue Result = combineSelectAndUse(N, N1, N0, DCI, /*AllOnes=*/false))
      return Result;

  return SDValue();
}

SDValue DadaoTargetLowering::PerformDAGCombine(SDNode *N,
                                               DAGCombinerInfo &DCI) const {
  switch (N->getOpcode()) {
  default:
    break;
  case ISD::ADD:
  case ISD::OR:
  case ISD::XOR:
    return combineSelectAndUseCommutative(N, DCI, /*AllOnes=*/false);
  case ISD::AND:
    return combineSelectAndUseCommutative(N, DCI, /*AllOnes=*/true);
  case ISD::SUB:
    return PerformSUBCombine(N, DCI);
  }

  return SDValue();
}

void DadaoTargetLowering::computeKnownBitsForTargetNode(
    const SDValue Op, KnownBits &Known, const APInt &DemandedElts,
    const SelectionDAG &DAG, unsigned Depth) const {
  unsigned BitWidth = Known.getBitWidth();
  switch (Op.getOpcode()) {
  default:
    break;
  case DadaoISD::SETCC:
    Known = KnownBits(BitWidth);
    Known.Zero.setBits(1, BitWidth);
    break;
  case DadaoISD::SELECT_CC:
    KnownBits Known2;
    Known = DAG.computeKnownBits(Op->getOperand(0), Depth + 1);
    Known2 = DAG.computeKnownBits(Op->getOperand(1), Depth + 1);
    Known = KnownBits::commonBits(Known, Known2);
    break;
  }
}

TargetLoweringBase::AtomicExpansionKind
DadaoTargetLowering::shouldExpandAtomicRMWInIR(AtomicRMWInst *AI) const {
  return AtomicExpansionKind::NotAtomic;
}

TargetLoweringBase::AtomicExpansionKind
DadaoTargetLowering::shouldExpandAtomicCmpXchgInIR(AtomicCmpXchgInst *AI) const {
  return AtomicExpansionKind::NotAtomic;
}
