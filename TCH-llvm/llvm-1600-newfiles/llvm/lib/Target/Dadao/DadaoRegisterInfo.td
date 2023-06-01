//===- DadaoRegisterInfo.td - Dadao Register defs ------------*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//  Declarations that describe the Dadao register file
//===----------------------------------------------------------------------===//

// Registers are identified with 6-bit ID numbers.
class DadaoRegData<bits<6> num, string n, list<string> altNames = []> : Register<n, altNames> {
  field bits<6> Num;
  let Num = num;
  let Namespace = "Dadao";
}

class DadaoRegBase<bits<6> num, string n, list<string> altNames = []> : Register<n, altNames> {
  field bits<6> Num;
  let Num = num;
  let Namespace = "Dadao";
}

class DadaoRegRAddr<bits<6> num, string n, list<string> altNames = []> : Register<n, altNames> {
  field bits<6> Num;
  let Num = num;
  let Namespace = "Dadao";
}

foreach i = 0-63 in {
  def RD#i : DadaoRegData<i, "rd"#i>, DwarfRegNum<[i]>;
}

foreach i = 0-63 in {
  def RB#i : DadaoRegBase<i, "rb"#i>, DwarfRegNum<[!add(i, 64)]>;
}

foreach i = 0-63 in {
  def RA#i : DadaoRegBase<i, "ra"#i>, DwarfRegNum<[!add(i, 128)]>;
}

let Namespace = "Dadao" in {
  def RDZERO : DadaoRegData<0, "rdzero", ["RD0"]>, DwarfRegAlias<RD0>;
  def RBIP   : DadaoRegBase<0,   "rbip", ["RB0"]>, DwarfRegAlias<RB0>;
  def RBSP   : DadaoRegBase<0,   "rbsp", ["RB1"]>, DwarfRegAlias<RB1>;
  def RBFP   : DadaoRegBase<0,   "rbfp", ["RB2"]>, DwarfRegAlias<RB2>;
  def RBGP   : DadaoRegBase<0,   "rbgp", ["RB3"]>, DwarfRegAlias<RB3>;
  def RBTP   : DadaoRegBase<0,   "rbtp", ["RB4"]>, DwarfRegAlias<RB4>;
  def RAVV   : DadaoRegRAddr<0,  "ravv", ["RA0"]>, DwarfRegAlias<RA0>;
  def RASP   : DadaoRegRAddr<0,  "rasp", ["RA63"]>, DwarfRegAlias<RA63>;
}

def GPRD : RegisterClass<"Dadao", [i64], 64,
    (add
     (sequence "RD%i", 16, 31),     // temporary registers
     (sequence "RD%i", 32, 63),     // callee saved registers
     (sequence "RD%i", 8, 15),      // registers for passing arguments
     RD7,                           // FIXME: conditional reg
     RD6, RD5, RD4, RD3, RD2, RD1,  // reserved
     RD0, RDZERO                    // constant 0
    )>;

def GPRB : RegisterClass<"Dadao", [i64], 64,
    (add
     (sequence "RB%i", 16, 31),     // temporary registers
     (sequence "RB%i", 32, 63),     // callee saved registers
     (sequence "RB%i", 8, 15),      // registers for passing arguments
     RB7, RB6, RB5,                 // reserved
     RB4, RBTP,                     // thread pointer
     RB3, RBGP,                     // global pointer
     RB2, RBFP,                     // frame pointer
     RB1, RBSP,                     // stack pointer
     RB0, RBIP                      // instruction pointer
    )>;

def GPRA : RegisterClass<"Dadao", [i64], 64,
    (add RA0, RAVV,                 // oVerflow/underflow Vector
     (sequence "RA%i", 1, 62),      // RA stack
     RA63, RASP                     // RA stack pointer
    )>;

// Define a temp register to capture the dependencies between the set flag
// and setcc instructions
// TODO: Every general RD can be used as RDCC
def RDCC  : DadaoRegData<7, "rdcc">;

//===----------------------------------------------------------------------===//

// Registers are identified with 5-bit ID numbers.
class DadaoReg<bits<5> num, string n, list<Register> subregs = [],
               list<string> altNames = []> : Register<n, altNames> {
  field bits<5> Num;
  let Num = num;
  let Namespace = "Dadao";
  let SubRegs = subregs;
}

let Namespace = "Dadao" in {
  def sub_32 : SubRegIndex<32>;
}

// Integer registers
foreach i = 0-31 in {
  def R#i : DadaoReg<i, "r"#i>, DwarfRegNum<[i]>;
}

// Register aliases
let SubRegIndices = [sub_32] in {
  def PC  : DadaoReg< 2,  "pc",  [R2]>,  DwarfRegAlias<R2>;
  def SP  : DadaoReg< 4,  "sp",  [R4]>,  DwarfRegAlias<R4>;
  def FP  : DadaoReg< 5,  "fp",  [R5]>,  DwarfRegAlias<R5>;
  def RV  : DadaoReg< 8,  "rv",  [R8]>,  DwarfRegAlias<R8>;
  def RR1 : DadaoReg<10, "rr1", [R10]>, DwarfRegAlias<R10>;
  def RR2 : DadaoReg<11, "rr2", [R11]>, DwarfRegAlias<R11>;
  def RCA : DadaoReg<15, "rca", [R15]>, DwarfRegAlias<R15>;
}

// Define a status register to capture the dependencies between the set flag
// and setcc instructions
def SR : DadaoReg< 0, "sw">;

// Register classes.
def GPR : RegisterClass<"Dadao", [i64], 32,
    (add R3, R9, R12, R13, R14, R16, R17,
     (sequence "R%i", 20, 31),
     R6, R7, R18, R19, // registers for passing arguments
     R15, RCA, // register for constant addresses
     R10, RR1, R11, RR2, // programmer controlled registers
     R8,  RV,  // return value
     R5,  FP,  // frame pointer
     R4,  SP,  // stack pointer
     R2,  PC,  // program counter
     R1,       // all 1s (0xffffffff)
     R0        // constant 0
    )>;

// Condition code register class
def CCR : RegisterClass<"Dadao", [i64], 32, (add SR)> {
  let CopyCost = -1; // Don't allow copying of status registers
  let isAllocatable = 0;
}
