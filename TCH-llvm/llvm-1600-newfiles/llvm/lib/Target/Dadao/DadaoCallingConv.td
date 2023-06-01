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
  CCIfNotVarArg<CCIfInReg<CCIfType<[i64], CCAssignToReg<[ RD8, RD9, RD10, RD11, RD12, RD13, RD14 ]>>>>,

  // Otherwise they are assigned to the stack in 8-byte aligned units.
  CCAssignToStack<8, 8>
]>;

// Dadao 32-bit Fast Calling convention.
def CC_Dadao32_Fast : CallingConv<[
  // Promote i8/i16/i32 args to i64
  CCIfType<[ i8, i16, i32 ], CCPromoteToType<i64>>,

  // Put arguments in registers.
  CCIfNotVarArg<CCIfType<[i64], CCAssignToReg<[ RD8, RD9, RD10, RD11, RD12, RD13, RD14 ]>>>,

  // Otherwise they are assigned to the stack in 8-byte aligned units.
  CCAssignToStack<8, 8>
]>;

// Dadao 32-bit C return-value convention.
def RetCC_Dadao32 : CallingConv<[
  CCIfType<[i64], CCAssignToReg<[RDRV]>>
]>;

def CSR: CalleeSavedRegs<(add)>;
