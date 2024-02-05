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
  def RA#i : DadaoRegRAddr<i, "ra"#i>, DwarfRegNum<[!add(i, 128)]>;
}

let Namespace = "Dadao" in {
  def RDZERO : DadaoRegData<0, "rdzero", ["RD0"]>, DwarfRegAlias<RD0>;
  def RDRV   : DadaoRegData<31,  "rdrv", ["RD31"]>, DwarfRegAlias<RD31>;
  def RBIP   : DadaoRegBase<0,   "rbip", ["RB0"]>, DwarfRegAlias<RB0>;
  def RBSP   : DadaoRegBase<1,   "rbsp", ["RB1"]>, DwarfRegAlias<RB1>;
  def RBFP   : DadaoRegBase<2,   "rbfp", ["RB2"]>, DwarfRegAlias<RB2>;
  def RBGP   : DadaoRegBase<3,   "rbgp", ["RB3"]>, DwarfRegAlias<RB3>;
  def RBTP   : DadaoRegBase<4,   "rbtp", ["RB4"]>, DwarfRegAlias<RB4>;
  def RBCA   : DadaoRegBase<7,   "rbca", ["RB7"]>, DwarfRegAlias<RB7>;
  def RAVV   : DadaoRegRAddr<0,  "ravv", ["RA0"]>, DwarfRegAlias<RA0>;
  def RASP   : DadaoRegRAddr<63, "rasp", ["RA63"]>, DwarfRegAlias<RA63>;
}

let AllocationPriority = 3 in
def GPRD : RegisterClass<"Dadao", [i64], 64,
    (add
     (sequence "RD%i", 16, 31),     // temporary registers
     (sequence "RD%i", 32, 63),     // callee saved registers
     (sequence "RD%i", 8, 14),      // registers for passing arguments
     RD31, RDRV,                    // return value
     RD7,                           // FIXME: conditional reg
     RD6, RD5, RD4, RD3, RD2, RD1,  // reserved
     RD0, RDZERO                    // constant 0
    )>;

def GPRB : RegisterClass<"Dadao", [i64], 64,
    (add
     (sequence "RB%i", 16, 31),     // temporary registers
     (sequence "RB%i", 32, 63),     // callee saved registers
     (sequence "RB%i", 8, 15),      // registers for passing arguments
     RB7, RBCA,                     // constant addresses
     RB6, RB5,                      // reserved
     RB4, RBTP,                     // thread pointer
     RB3, RBGP,                     // global pointer
     RB2, RBFP,                     // frame pointer
     RB1, RBSP,                     // stack pointer
     RB0, RBIP                      // instruction pointer
    )>;

let isAllocatable = false in
def GPRA : RegisterClass<"Dadao", [i64], 64,
    (add RA0, RAVV,                 // oVerflow/underflow Vector
     (sequence "RA%i", 1, 62),      // RA stack
     RA63, RASP                     // RA stack pointer
    )>;

// Define a temp register to capture the dependencies between the set flag
// and setcc instructions
// TODO: Every general RD can be used as RDCC

// Guorui Wen: In DadaoInstrInfo.td, Many CMP instructions implicitly define (write to) register RDCC.
// However, RDCC doesn't belong to any physical register class, causing failed assertion when
// execution reaches llvm/lib/CodeGen/SelectionDAG/InstrEmitter.cpp:1136

def RDCC  : DadaoRegData<7, "rdcc">;
