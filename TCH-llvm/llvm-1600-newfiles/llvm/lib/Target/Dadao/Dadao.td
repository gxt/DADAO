//===- Dadao.td - Describe the Dadao Target Machine --------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Target-independent interfaces which we are implementing
//===----------------------------------------------------------------------===//

include "llvm/Target/Target.td"

//===----------------------------------------------------------------------===//
// Register File, Calling Conv, Instruction Descriptions
//===----------------------------------------------------------------------===//

include "DadaoSchedule.td"
include "DadaoRegisterInfo.td"
include "DadaoCallingConv.td"
include "DadaoInstrInfo.td"

def DadaoInstrInfo : InstrInfo;

//===----------------------------------------------------------------------===//
// Dadao processors supported.
//===----------------------------------------------------------------------===//

def : ProcessorModel<"generic", DadaoSchedModel, []>;
def : ProcessorModel<"v11", DadaoSchedModel, []>;

def DadaoInstPrinter : AsmWriter {
  string AsmWriterClassName  = "InstPrinter";
  bit isMCAsmWriter = 1;
}

//===----------------------------------------------------------------------===//
// Declare the target which we are implementing
//===----------------------------------------------------------------------===//

def Dadao : Target {
  // Pull in Instruction Info:
  let InstructionSet = DadaoInstrInfo;
  let AssemblyWriters = [DadaoInstPrinter];
}
