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

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

class InstDadao<dag outs, dag ins, string asmstr, list<dag> pattern>
    : Instruction {
  field bits<32> Inst;
  field bits<32> SoftFail = 0;
  let Size = 4;

  let Namespace = "Dadao";
  let DecoderNamespace = "Dadao";

  bits<4> Opcode;
  let Inst{31 - 28} = Opcode;

  dag OutOperandList = outs;
  dag InOperandList = ins;
  let AsmString = asmstr;
  let Pattern = pattern;
}

//------------------------------------------------------------------------------
// Register Immediate (RI)
//------------------------------------------------------------------------------
// Encoding:
//           -----------------------------------------------------------------
//           |0.A.A.A| . . . . | . . . . |F.H| . . . . . . . . . . . . . . . |
//           -----------------------------------------------------------------
//            opcode     Rd        Rs1                constant (16)
//
// Action:
//           Rd <- Rs1 op constant
//
// Except for shift instructions, `H' determines whether the constant
// is in the high (1) or low (0) word.  The other halfword is 0x0000,
// except for the `AND' instruction (`AAA' = 100), for which the other
// halfword is 0xFFFF, and shifts (`AAA' = 111), for which the constant is
// sign extended.
//
// `F' determines whether the instruction modifies (1) or does not
// modify (0) the program flags.
//
// `AAA' specifies the operation: `add' (000), `addc' (001), `sub'
// (010), `subb' (011), `and' (100), `or' (101), `xor' (110), or `shift'
// (111).  For the shift, `H' specifies a logical (0) or arithmetic (1)
// shift.  The amount and direction of the shift are determined by the
// sign extended constant interpreted as a two's complement number.  The
// shift operation is defined only for the range of:
//      31 ... 0 -1 ... -31
//      \      / \        /
//        left     right
//        shift    shift
//
// If and only if the `F' bit is 1, RI instructions modify the
// condition bits, `Z' (Zero), `N' (Negative), `V' (oVerflow), and `C'
// (Carry), according to the result.  If the flags are updated, they are
// updated as follows:
// `Z'
//      is set if the result is zero and cleared otherwise.
//
// `N'
//      is set to the most significant bit of the result.
//
// `V'
//      For arithmetic instructions (`add', `addc', `sub', `subb') `V' is
//      set if the sign (most significant) bits of the input operands are
//      the same but different from the sign bit of the result and cleared
//      otherwise.  For other RI instructions, `V' is cleared.
//
// `C'
//      For arithmetic instructions, `C' is set/cleared if there is/is_not
//      a carry generated out of the most significant when performing the
//      twos-complement addition (`sub(a,b) == a + ~b + 1', `subb(a,b) ==
//      a + ~b + `C'').  For left shifts, `C' is set to the least
//      significant bit discarded by the shift operation.  For all other
//      operations, `C' is cleared.
//
// A Jump is accomplished by `Rd' being `pc', and it has one shadow.
//
// The all-0s word is the instruction `R0 <- R0 + 0', which is a no-op.
class InstRI<bits<3> op, dag outs, dag ins, string asmstr,
             list<dag> pattern>
    : InstDadao<outs, ins, asmstr, pattern>, Sched<[WriteALU]> {
  let Itinerary = IIC_ALU;
  bits<5> Rd;
  bits<5> Rs1;
  bit F;
  bit H;
  bits<16> imm16;

  let Opcode{3} = 0;
  let Opcode{2 - 0} = op;
  let Inst{27 - 23} = Rd;
  let Inst{22 - 18} = Rs1;
  let Inst{17} = F;
  let Inst{16} = H;
  let Inst{15 - 0} = imm16;
}

//------------------------------------------------------------------------------
// Register Memory (RM)
//------------------------------------------------------------------------------
// Encoding:
//          -----------------------------------------------------------------
//          |1.0.0.S| . . . . | . . . . |P.Q| . . . . . . . . . . . . . . . |
//          -----------------------------------------------------------------
//           opcode     Rd        Rs1                 constant (16)
//
// Action:
//        Rd <- Memory(ea)      (Load)    see below for the
//        Memory(ea) <- Rd      (Store)   definition of ea.
//
// `S' determines whether the instruction is a Load (0) or a Store (1).
// Loads appear in Rd one cycle after this instruction executes.  If the
// following instruction reads Rd, that instruction will be delayed by 1
// clock cycle.
//
//   PQ      operation
//   --      ------------------------------------------
//   00      ea = Rs1
//   01      ea = Rs1,             Rs1 <- Rs1 + constant
//   10      ea = Rs1 + constant
//   11      ea = Rs1 + constant,  Rs1 <- Rs1 + constant
//
// The constant is sign-extended for this instruction.
//
// A Jump is accomplished by `Rd' being `pc', and it has *two* delay slots.
class InstRM<bit S, dag outs, dag ins, string asmstr, list<dag> pattern>
    : InstDadao<outs, ins, asmstr, pattern> {
  bits<5> Rd;
  bits<5> Rs1;
  bits<1> P;
  bits<1> Q;
  bits<16> imm16;
  // Dummy variables to allow multiclass definition of RM and RRM
  bits<2> YL;
  bit E;

  let Opcode{3 - 1} = 0b100;
  let Opcode{0} = S;
  let Inst{27 - 23} = Rd;
  let Inst{22 - 18} = Rs1;
  let Inst{17} = P;
  let Inst{16} = Q;
  let Inst{15 - 0} = imm16;
}

//------------------------------------------------------------------------------
// Special instructions (popc, leadz, trailz)
//------------------------------------------------------------------------------
// Encoding:
//         -----------------------------------------------------------------
//         |1.1.0.1|    Rd   |   Rs1   |F.-| . . . . | . . | . . . . | OP  |
//         -----------------------------------------------------------------
//          opcode      Rd       Rs1
// Action:
//         Rd <- Perform action encoded in OP on Rs1
//   OP is one of:
//      0b001 POPC   Population count;
//      0b010 LEADZ  Count number of leading zeros;
//      0b011 TRAILZ Count number of trailing zeros;
class InstSpecial<bits<3> op, dag outs, dag ins, string asmstr,
                  list<dag> pattern> : InstDadao<outs, ins, asmstr,
                  pattern>, Sched<[WriteALU]> {
  let Itinerary = IIC_ALU;
  bit F;
  bits<5> Rd;
  bits<5> Rs1;

  let Opcode = 0b1101;
  let Inst{27 - 23} = Rd;
  let Inst{22 - 18} = Rs1;
  let Inst{17} = F;
  let Inst{16 - 3} = 0;
  let Inst{2 - 0} = op;
}

// Pseudo instructions
class Pseudo<dag outs, dag ins, string asmstr, list<dag> pattern>
    : InstDadao<outs, ins, asmstr, pattern> {
  let Inst{15 - 0} = 0;
  let isPseudo = 1;
}
