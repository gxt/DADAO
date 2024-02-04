//===- DadaoCallingConv.td - Calling Conventions Dadao -------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This describes the calling conventions for the Dadao architectures.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Return Value Calling Conventions
//===----------------------------------------------------------------------===//

// Dadao 32-bit C Calling convention.
def CC_Dadao32 : CallingConv<[
  // Promote i8/i16/i32 args to i64
  CCIfType<[i8, i16, i32], CCPromoteToType<i64>>,
  // Put argument in registers if marked 'inreg' and not a vararg call.
  CCIfNotVarArg<CCIfInReg<CCIfType<[i64], CCAssignToReg<[RD16, RD17, RD18, RD19, RD20, RD21, RD22, RD23, RD24, RD25, RD26, RD27, RD28, RD29, RD30, RD31]>>>>,
  // Otherwise they are assigned to the stack in 8-byte aligned units.
  CCAssignToStack<8, 8>
]>;


def CC_Dadao32_Fast : CallingConv<[
  // Promote i8/i16/i32 args to i64
  CCIfType<[i8, i16, i32], CCPromoteToType<i64>>,
  // Put argument in registers if marked 'inreg' and not a vararg call.
  CCIfNotVarArg<CCIfInReg<CCIfType<[i64], CCAssignToReg<[RD16, RD17, RD18, RD19, RD20, RD21, RD22, RD23, RD24, RD25, RD26, RD27, RD28, RD29, RD30, RD31]>>>>,
  // Otherwise they are assigned to the stack in 8-byte aligned units.
  CCAssignToStack<8, 8>
]>;

// Dadao 32-bit C return-value convention.
def RetCC_Dadao32 : CallingConv<[
  CCIfType<[i64], CCAssignToReg<[RDRV]>>
]>;

def CSR: CalleeSavedRegs<(add (sequence "RD%u", 32, 63))>;
