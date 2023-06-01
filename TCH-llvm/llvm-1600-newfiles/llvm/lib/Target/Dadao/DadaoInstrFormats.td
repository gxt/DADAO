//===- DadaoInstrFormats.td - Dadao Instruction Formats ----*- tablegen -*-===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//

class Inst_Dadao<dag outs, dag ins, string opcstr, string oprstr, list<dag> pattern>
    : Instruction {
  field bits<32> Inst;
  field bits<32> SoftFail = 0;
  let Size = 4;

  let Namespace = "Dadao";
  let DecoderNamespace = "Dadao";

  bits<8> MajorOpcode;
  bits<6> HexA;       // also Minor Opcode
  bits<6> HexB;
  bits<6> HexC;
  bits<6> HexD;

  let Inst{31 - 24} = MajorOpcode;
  let Inst{23 - 18} = HexA;
  let Inst{17 - 12} = HexB;
  let Inst{11 -  6} = HexC;
  let Inst{ 5 -  0} = HexD;

  dag OutOperandList = outs;
  dag InOperandList = ins;
  let AsmString = opcstr # "\t" # oprstr;
  let Pattern = pattern;
}

class Inst_ORRR<bits<8> majorop, bits<6> minorop, dag outs, dag ins, string opcstr, string oprstr, list<dag> pattern>
    : Inst_Dadao<outs, ins, opcstr, oprstr, pattern> {

  bits<6> RxHb;
  bits<6> RyHc;
  bits<6> RzHd;

  let MajorOpcode = majorop;
  let HexA = minorop;
  let HexB = RxHb;
  let HexC = RyHc;
  let HexD = RzHd;
}

class Inst_ORRI<bits<8> majorop, bits<6> minorop, dag outs, dag ins, string opcstr, string oprstr, list<dag> pattern>
    : Inst_Dadao<outs, ins, opcstr, oprstr, pattern> {

  bits<6> RxHb;
  bits<6> RyHc;
  bits<6> Imm6;

  let MajorOpcode = majorop;
  let HexA = minorop;
  let HexB = RxHb;
  let HexC = RyHc;
  let HexD = Imm6;
}

class Inst_OIII<bits<8> majorop, bits<6> minorop, dag outs, dag ins, string opcstr, string oprstr, list<dag> pattern>
    : Inst_Dadao<outs, ins, opcstr, oprstr, pattern> {

  bits<18> Imm18;

  let MajorOpcode = majorop;
  let HexA = minorop;
  let HexB = Imm18{17 - 12};
  let HexC = Imm18{11 -  6};
  let HexD = Imm18{ 5 -  0};
}

class Inst_RRRR<bits<8> majorop, dag outs, dag ins, string opcstr, string oprstr, list<dag> pattern>
    : Inst_Dadao<outs, ins, opcstr, oprstr, pattern> {

  bits<6> RdHa;
  bits<6> RdHb;
  bits<6> RdHc;
  bits<6> RdHd;

  let MajorOpcode = majorop;
  let HexA = RdHa;
  let HexB = RdHb;
  let HexC = RdHc;
  let HexD = RdHd;
}

class Inst_RRRI<bits<8> majorop, dag outs, dag ins, string opcstr, string oprstr, list<dag> pattern>
    : Inst_Dadao<outs, ins, opcstr, oprstr, pattern> {

  bits<6> RxHa;
  bits<6> RbHb;
  bits<6> RdHc;
  bits<6> Imm6;

  let MajorOpcode = majorop;
  let HexA = RxHa;
  let HexB = RbHb;
  let HexC = RdHc;
  let HexD = Imm6;
}

class Inst_RRII<bits<8> majorop, dag outs, dag ins, string opcstr, string oprstr, list<dag> pattern>
    : Inst_Dadao<outs, ins, opcstr, oprstr, pattern> {

  bits<6> RxHa;
  bits<6> RyHb;
  bits<12> Imm12;

  let MajorOpcode = majorop;
  let HexA = RxHa;
  let HexB = RyHb;
  let HexC = Imm12{11 - 6};
  let HexD = Imm12{ 5 - 0};
}

class Inst_RIII<bits<8> majorop, dag outs, dag ins, string opcstr, string oprstr, list<dag> pattern>
    : Inst_Dadao<outs, ins, opcstr, oprstr, pattern> {

  bits<6> RxHa;
  bits<18> Imm18;

  let MajorOpcode = majorop;
  let HexA = RxHa;
  let HexB = Imm18{17 - 12};
  let HexC = Imm18{11 -  6};
  let HexD = Imm18{ 5 -  0};
}

class Inst_RWII<bits<8> majorop, bits<2> wpos, dag outs, dag ins, string opcstr, string oprstr, list<dag> pattern>
    : Inst_Dadao<outs, ins, opcstr, oprstr, pattern> {

  bits<6> RxHa;
  bits<16> Imm16;

  let MajorOpcode = majorop;
  let HexA = RxHa;
  let HexB{5-4} = wpos;
  let HexB{3-0} = Imm16{15 - 12};
  let HexC = Imm16{11 - 6};
  let HexD = Imm16{ 5 - 0};
}

class Inst_IIII<bits<8> majorop, dag outs, dag ins, string opcstr, string oprstr, list<dag> pattern>
    : Inst_Dadao<outs, ins, opcstr, oprstr, pattern> {

  bits<24> Imm24;

  let MajorOpcode = majorop;
  let HexA = Imm24{23 - 18};
  let HexB = Imm24{17 - 12};
  let HexC = Imm24{11 -  6};
  let HexD = Imm24{ 5 -  0};
}

// Pseudo instructions
class Pseudo<dag outs, dag ins, string opcstr, string oprstr, list<dag> pattern>
    : Inst_Dadao<outs, ins, opcstr, oprstr, pattern> {
  let isPseudo = 1;
}
