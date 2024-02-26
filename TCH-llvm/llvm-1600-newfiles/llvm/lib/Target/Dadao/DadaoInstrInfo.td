//===-- DadaoInstrInfo.td - Target Description for Dadao Target -----------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===----------------------------------------------------------------------===//
//
// This file describes the Dadao instructions in TableGen format.
//
//===----------------------------------------------------------------------===//

//===----------------------------------------------------------------------===//
// Instruction format superclass
//===----------------------------------------------------------------------===//

include "DadaoInstrFormats.td"

//  These are target-independent nodes, but have target-specific formats.
def SDT_DadaoSetFlag      : SDTypeProfile<0,  3, [SDTCisSameAs<0, 1>]>;
def SDT_DadaoBrCC         : SDTypeProfile<0,  4, [
  SDTCisVT<0, i64>, SDTCisSameAs<1, 2>, SDTCisVT<3, OtherVT>
]>;
def SDT_DadaoSetCC        : SDTypeProfile<1,  1, [SDTCisVT<0, i64>,
                                                  SDTCisVT<1, i64>]>;
def SDT_DadaoSelectCC     : SDTypeProfile<1,  3, [SDTCisSameAs<0, 1>,
                                                  SDTCisSameAs<1, 2>]>;
def SDT_DadaoCall         : SDTypeProfile<0, -1, [SDTCisVT<0, i64>]>;

def DadaoSetFlag     : SDNode<"DadaoISD::SET_FLAG", SDT_DadaoSetFlag,
                              [SDNPOutGlue]>;
def DadaoBrCC        : SDNode<"DadaoISD::BR_CC", SDT_DadaoBrCC,
                              [SDNPHasChain]>;
def DadaoSetCC       : SDNode<"DadaoISD::SETCC", SDT_DadaoSetCC,
                              [SDNPInGlue]>;
def DadaoSelectCC    : SDNode<"DadaoISD::SELECT_CC", SDT_DadaoSelectCC,
                              [SDNPInGlue]>;
def DadaoCall        : SDNode<"DadaoISD::CALL", SDT_DadaoCall,
                              [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue,
                               SDNPVariadic]>;
def DadaoRet         : SDNode<"DadaoISD::RET_FLAG", SDTNone,
                              [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;

// Extract bits 0-5 (low-end) of an immediate value.
def LO06 : SDNodeXForm<imm, [{
  return CurDAG->getTargetConstant((uint64_t)N->getZExtValue() & 0x3FULL,
                                   SDLoc(N), MVT::i64);
}]>;

// Extract bits 0-11 (low-end) of an immediate value.
def LO12 : SDNodeXForm<imm, [{
  return CurDAG->getTargetConstant((uint64_t)N->getZExtValue() & 0xFFFULL,
                                   SDLoc(N), MVT::i64);
}]>;

// Extract bits 0-15 (low-end) of an immediate value.
def LO16 : SDNodeXForm<imm, [{
  return CurDAG->getTargetConstant((uint64_t)N->getZExtValue() & 0xffff,
                                   SDLoc(N), MVT::i64);
}]>;

// Extract bits 0-17 (low-end) of an immediate value.
def LO18 : SDNodeXForm<imm, [{
  return CurDAG->getTargetConstant((uint64_t)N->getZExtValue() & 0x3FFFFULL,
                                   SDLoc(N), MVT::i64);
}]>;

def NEG : SDNodeXForm<imm, [{
  return CurDAG->getTargetConstant(-N->getSExtValue(), SDLoc(N), MVT::i64);
}]>;

def ImmHex1AsmOperand : AsmOperandClass { let Name = "ImmHex1"; }
def i64h1 : Operand<i64>, PatLeaf<(imm), [{
    return N->getZExtValue() <= 63;}]> {
  let ParserMatchClass = ImmHex1AsmOperand;
}

def Imm12AsmOperand : AsmOperandClass { let Name = "Imm12"; }
def imm12 : Operand<i64>, PatLeaf<(imm), [{
    return isInt<12>(N->getSExtValue()); }]> {
  let ParserMatchClass = Imm12AsmOperand;
}

def LoImm12AndAsmOperand : AsmOperandClass { let Name = "LoImm12And"; }
def i64lo12and : Operand<i64>, PatLeaf<(i64 imm), [{
    // i64lo12and predicate - true if the 64-bit immediate has the rightmost 12
    // bits set and the leftmost 12 bits 1's.
    return (N->getZExtValue() >= (~0xFFFULL));}], LO12> {
  let ParserMatchClass = LoImm12AndAsmOperand;
  let PrintMethod = "printLo12AndImmOperand";
}

def LoImm12AsmOperand : AsmOperandClass { let Name = "LoImm12"; }
def i64lo12z : Operand<i64>, PatLeaf<(i64 imm), [{
    // i64lo12z predicate - true if the 64-bit immediate has only rightmost 12 bits set.
    return ((N->getZExtValue() & 0xFFFULL) == N->getZExtValue());}], LO12> {
  let ParserMatchClass = LoImm12AsmOperand;
}
def i64lo12s : Operand<i64>, PatLeaf<(i64 imm), [{
    // i64lo12s predicate - true if the 64-bit immediate has only rightmost 12 bits set.
    return ((int64_t)(N->getSExtValue() & 0xFFFULL) == N->getSExtValue());}], LO12> {
  let ParserMatchClass = LoImm12AsmOperand;
}

def i64neg12 : Operand<i64>, PatLeaf<(i64 imm), [{
    // i64neg12 predicate - true if the immediate is negative and can
    // be represented by a 12 bit integer.
    int Imm = N->getSExtValue();
    return (Imm < 0) && (isInt<12>(Imm));}], LO12> {
  let ParserMatchClass = LoImm12AsmOperand;
}

def LoImm18AsmOperand : AsmOperandClass { let Name = "LoImm18"; }
def i64lo18z : Operand<i64>, PatLeaf<(i64 imm), [{
    // i64lo18z predicate - true if the 64-bit immediate has only rightmost 18 bits set.
    return ((N->getZExtValue() & 0x3FFFFULL) == N->getZExtValue());}], LO18> {
  let ParserMatchClass = LoImm18AsmOperand;
}
def i64lo18s : Operand<i64>, PatLeaf<(i64 imm), [{
    // i64lo18s predicate - true if the 64-bit immediate has only rightmost 18 bits set.
    return ((int64_t)(N->getSExtValue() & 0x3FFFFULL) == N->getSExtValue());}], LO18> {
  let ParserMatchClass = LoImm18AsmOperand;
}

def LoImm16AsmOperand : AsmOperandClass { let Name = "LoImm16"; }
def i64lo16z : Operand<i64>, PatLeaf<(i64 imm), [{
    // i64lo16 predicate - true if the 32-bit immediate has only rightmost 16
    // bits set.
    return ((N->getZExtValue() & 0xFFFFUL) == N->getZExtValue());}], LO16> {
  let ParserMatchClass = LoImm16AsmOperand;
}
def i64lo16s : Operand<i64>, PatLeaf<(i64 imm), [{
    // i64lo16 predicate - true if the 32-bit immediate has only rightmost 16
    // bits set.
    return ((int64_t)(N->getSExtValue() & 0xFFFFUL) == N->getSExtValue());}], LO16> {
  let ParserMatchClass = LoImm16AsmOperand;
}

// Extract w0/w1/w2/w3 of an immediate value.
def Wyde0 : SDNodeXForm<imm, [{ return CurDAG->getTargetConstant(((uint64_t)N->getZExtValue() >> 00) & 0xFFFF, SDLoc(N), MVT::i64); }]>;
def Wyde1 : SDNodeXForm<imm, [{ return CurDAG->getTargetConstant(((uint64_t)N->getZExtValue() >> 16) & 0xFFFF, SDLoc(N), MVT::i64); }]>;
def Wyde2 : SDNodeXForm<imm, [{ return CurDAG->getTargetConstant(((uint64_t)N->getZExtValue() >> 32) & 0xFFFF, SDLoc(N), MVT::i64); }]>;
def Wyde3 : SDNodeXForm<imm, [{ return CurDAG->getTargetConstant(((uint64_t)N->getZExtValue() >> 48) & 0xFFFF, SDLoc(N), MVT::i64); }]>;

def ImmWyde0AsmOperand : AsmOperandClass { let Name = "ImmWyde0"; }
def i64w0 : Operand<i64>, PatLeaf<(i64 imm), [{
    // i64w0 predicate - true if the immediate has only wyde 0 set.
    return ((N->getZExtValue() & (0xFFFFULL << 00)) == N->getZExtValue());}], Wyde0> {
  let ParserMatchClass = ImmWyde0AsmOperand;
}

def ImmWyde1AsmOperand : AsmOperandClass { let Name = "ImmWyde1"; }
def i64w1 : Operand<i64>, PatLeaf<(i64 imm), [{
    // i64w1 predicate - true if the immediate has only wyde 1 set.
    return ((N->getZExtValue() & (0xFFFFULL << 16)) == N->getZExtValue());}], Wyde1> {
  let ParserMatchClass = ImmWyde1AsmOperand;
}

def ImmWyde2AsmOperand : AsmOperandClass { let Name = "ImmWyde2"; }
def i64w2 : Operand<i64>, PatLeaf<(i64 imm), [{
    // i64w2 predicate - true if the immediate has only wyde 2 set.
    return ((N->getZExtValue() & (0xFFFFULL << 32)) == N->getZExtValue());}], Wyde2> {
  let ParserMatchClass = ImmWyde2AsmOperand;
}

def ImmWyde3AsmOperand : AsmOperandClass { let Name = "ImmWyde3"; }
def i64w3 : Operand<i64>, PatLeaf<(i64 imm), [{
    // i64w3 predicate - true if the immediate has only wyde 3 set.
    return ((N->getZExtValue() & (0xFFFFULL << 48)) == N->getZExtValue());}], Wyde3> {
  let ParserMatchClass = ImmWyde3AsmOperand;
}

// Predicate operand for wyde position. Default to 0 = w0.
def WydePositionOperand : AsmOperandClass { let Name = "WydePosition"; }
class wposPred<int Default = 0> : PredicateOperand<i64, (ops i64imm), (ops (i64 Default))> {
  let PrintMethod = "printWPosOperand";
  let ParserMatchClass = WydePositionOperand;
  let DecoderMethod = "decodeWPosOperand";
}
def wposPred0 : wposPred <0>;
def wposPred1 : wposPred <1>;
def wposPred2 : wposPred <2>;
def wposPred3 : wposPred <3>;

// Addressing modes.
def ADDR_RRRI : ComplexPattern<i64, 3, "selectAddrRRRI", [], []>;
def ADDR_RRII : ComplexPattern<i64, 2, "selectAddrRRII", [frameindex], []>;

// Address operands.
def MemRegRegAsmOperand : AsmOperandClass {
  let Name = "MemRegReg";
  let ParserMethod  = "parseMemoryOperand";
}
def MEM_RRRI : Operand<i64> {
  let DecoderMethod = "decodeRRRIMemoryValue";
  let EncoderMethod = "getRRRIMemoryOpValue";
  let MIOperandInfo = (ops GPRB:$Op1, GPRD:$Op2, LO06);
  let ParserMatchClass = MemRegRegAsmOperand;
  let PrintMethod   = "printMemRRRIOperand";
}

// Address operands
def MemRegImmAsmOperand : AsmOperandClass {
  let Name = "MemRegImm";
  let ParserMethod  = "parseMemoryOperand";
}
def MEM_RRII : Operand<i64> {
  let DecoderMethod = "decodeRRIIMemoryValue";
  let EncoderMethod = "getRRIIMemoryOpValue";
  let MIOperandInfo = (ops GPRB:$base, imm12:$offset);
  let ParserMatchClass = MemRegImmAsmOperand;
  let PrintMethod   = "printMemRRIIOperand";
}

// -------------------------------------------------- //
// NOOP instructions
// -------------------------------------------------- //

def SWYM_OIII  : Inst_OIII<0b00010000, 0b000000, (outs), (ins i64lo18z:$Imm18), "swym", "$Imm18", []> {
  let hasSideEffects = 0;
}

// -------------------------------------------------- //
// ALU instructions
// -------------------------------------------------- //
class ALU_ORRR<bits<6> minorop, string OpcString, SDNode OpNode>
  : Inst_ORRR<0b00010000, minorop, (outs GPRD:$RxHb), (ins GPRD:$RyHc, GPRD:$RzHd),
            OpcString, "$RxHb, $RyHc, $RzHd",
            [(set GPRD:$RxHb, (OpNode GPRD:$RyHc, GPRD:$RzHd))]>;

let isCommutable = 1 in {
  def AND_ORRR  : ALU_ORRR<0b001000,  "and", and>;
  def ORR_ORRR  : ALU_ORRR<0b001001,  "orr", or>;
  def XOR_ORRR  : ALU_ORRR<0b001010,  "xor", xor>;
  def XNOR_ORRR : Inst_ORRR<0b00010000, 0b001011, (outs GPRD:$RxHb), (ins GPRD:$RyHc, GPRD:$RzHd),
                            "xnor", "$RxHb, $RyHc, $RzHd",
                            [(set GPRD:$RxHb, (not (xor GPRD:$RyHc, GPRD:$RzHd)))]>;
}

def SHLU_ORRR   : ALU_ORRR<0b010001, "shlu", shl>;
def SHRS_ORRR   : ALU_ORRR<0b010010, "shrs", sra>;
def SHRU_ORRR   : ALU_ORRR<0b010011, "shru", srl>;

class ALU_RB_ORRR<bits<6> minorop, string OpcString, SDNode OpNode, RegisterClass GPRx, RegisterClass GPRz>
  : Inst_ORRR<0b00010000, minorop, (outs GPRx:$RxHb), (ins GPRx:$RyHc, GPRz:$RzHd),
            OpcString, "$RxHb, $RyHc, $RzHd",
            [(set GPRx:$RxHb, (OpNode GPRx:$RyHc, GPRz:$RzHd))]>;

def ADD_RB_ORRR : ALU_RB_ORRR<0b101110, "add", add, GPRB, GPRD>;
def SUB_RB_ORRR : ALU_RB_ORRR<0b101111, "sub", sub, GPRB, GPRD>;

class R2R_ORRI<bits<6> minorop, string OpcString, RegisterClass GPRx, RegisterClass GPRy>
  : Inst_ORRI<0b00010000, minorop, (outs GPRx:$RxHb), (ins GPRy:$RyHc),
              OpcString, "$RxHb, $RyHc", []>;

// TODO: ONLY imm6 == 1 permitted, fixme later
let Imm6 = 0b000001 in {
  def RD2RD_ORRI : R2R_ORRI<0b101000, "setrd", GPRD, GPRD>;
  def RD2RB_ORRI : R2R_ORRI<0b101001, "setrb", GPRB, GPRD>;
  def RB2RD_ORRI : R2R_ORRI<0b101010, "setrd", GPRD, GPRB>;
  def RB2RB_ORRI : R2R_ORRI<0b101011, "setrb", GPRB, GPRB>;
}

class SHIFT_ORRI<bits<6> minorop, string OpcString, SDNode OpNode>
  : Inst_ORRI<0b00010000, minorop, (outs GPRD:$RxHb), (ins GPRD:$RyHc, i64h1:$Imm6),
              OpcString, "$RxHb, $RyHc, $Imm6", [(set GPRD:$RxHb, (OpNode GPRD:$RyHc, i64h1:$Imm6))]>;

def SHLU_ORRI   : SHIFT_ORRI<0b011001, "shlu", shl>;
def SHRS_ORRI   : SHIFT_ORRI<0b011010, "shrs", sra>;
def SHRU_ORRI   : SHIFT_ORRI<0b011011, "shru", srl>;

// TODO: RdHa SHOULD be handled
class ALU_RRRR<bits<8> majorop, string OpcString, SDNode OpNode>
  : Inst_RRRR<majorop, (outs GPRD:$RdHa, GPRD:$RdHb), (ins GPRD:$RdHc, GPRD:$RdHd),
            OpcString, "$RdHa, $RdHb, $RdHc, $RdHd",
            [(set GPRD:$RdHb, (OpNode GPRD:$RdHc, GPRD:$RdHd)),
             (set GPRD:$RdHa, (DadaoSetCC 0))]> {
}

let isCommutable = 1 in {
  def ADD_RRRR : ALU_RRRR<0b00011010, "add", add>;
}
def SUB_RRRR : ALU_RRRR<0b00011011, "sub", sub>;

class ALU_RRII<bits<8> majorop, string OpcString, SDNode OpNode, RegisterClass GPRx, RegisterClass GPRy, PatLeaf LoExt>
  : Inst_RRII<majorop, (outs GPRx:$RxHa), (ins GPRy:$RyHb, LoExt:$Imm12),
            OpcString, "$RxHa, $RyHb, $Imm12",
            [(set GPRx:$RxHa, (OpNode GPRy:$RyHb, LoExt:$Imm12))]> {
}

def ANDI_RRII : ALU_RRII<0b00011000, "andi", and, GPRD, GPRD, i64lo12and>;
def ADDI_RRII : ALU_RRII<0b00011001, "addi", add, GPRD, GPRD, imm12>;
def ADDI_RB_RRII : ALU_RRII<0b01001001, "addi", add, GPRB, GPRB, imm12>;

def : Pat<(sub GPRD:$Rs1, i64neg12:$imm),
          (ADDI_RRII GPRD:$Rs1, (NEG $imm))>;
def : Pat<(sub GPRB:$Rs1, i64neg12:$imm),
          (ADDI_RB_RRII GPRB:$Rs1, (NEG $imm))>;

// -------------------------------------------------- //
// Mul Div Rem instructions
// -------------------------------------------------- //

def MULS_RRRR
  : Inst_RRRR<0b00011100, (outs GPRD:$RdHa, GPRD:$RdHb), (ins GPRD:$RdHc, GPRD:$RdHd),
            "muls", "$RdHa, $RdHb, $RdHc, $RdHd",
            []>;
def MULU_RRRR
  : Inst_RRRR<0b00011101, (outs GPRD:$RdHa, GPRD:$RdHb), (ins GPRD:$RdHc, GPRD:$RdHd),
            "mulu", "$RdHa, $RdHb, $RdHc, $RdHd",
            []>;
def DIVS_RRRR
  : Inst_RRRR<0b00011110, (outs GPRD:$RdHa, GPRD:$RdHb), (ins GPRD:$RdHc, GPRD:$RdHd),
            "divs", "$RdHa, $RdHb, $RdHc, $RdHd",
            []>;
def DIVU_RRRR
  : Inst_RRRR<0b00011111, (outs GPRD:$RdHa, GPRD:$RdHb), (ins GPRD:$RdHc, GPRD:$RdHd),
            "divu", "$RdHa, $RdHb, $RdHc, $RdHd",
            []>;

// -------------------------------------------------- //
// Compare instructions
// -------------------------------------------------- //

class COMPARE_ORRR<bits<6> minorop, string OpcString, RegisterClass GPRcmp>
  : Inst_ORRR<0b00010000, minorop, (outs GPRD:$RxHb), (ins GPRcmp:$RyHc, GPRcmp:$RzHd), OpcString, "$RxHb, $RyHc, $RzHd",
             [(DadaoSetFlag GPRD:$RxHb, GPRcmp:$RyHc, GPRcmp:$RzHd)]> {

  let isCompare = 1;
}

def CMPS_ORRR   : COMPARE_ORRR<0b100100, "cmps", GPRD>;
// TODO: CMPU should be impled with new SET_FLAG
def CMPU_ORRR   : COMPARE_ORRR<0b100101, "cmpu", GPRD>;
def CMP_ORRR    : COMPARE_ORRR<0b101101, "cmp",  GPRB>;

class COMPARE_RRII<bits<8> majorop, string OpcString, PatLeaf LoExt>
  : Inst_RRII<majorop, (outs), (ins GPRD:$RxHa, GPRD:$RyHb, LoExt:$Imm12), OpcString, "$RxHa, $RyHb, $Imm12",
             [(DadaoSetFlag GPRD:$RxHa, GPRD:$RyHb, LoExt:$Imm12)]> {

  let Defs = [RDCC];
  let isCompare = 1;
}

def CMPS_RRII   : COMPARE_RRII<0b00010010, "cmps", i64lo12s>;
def CMPU_RRII   : COMPARE_RRII<0b00010011, "cmpu", i64lo12z>;

// -------------------------------------------------- //
// Conditional Set instructions
// -------------------------------------------------- //

class CONDSET_RRRR<bits<8> majorop, string OpcString>
  : Inst_RRRR<majorop, (outs GPRD:$RdHb), (ins GPRD:$RdHa, GPRD:$RdHc, GPRD:$RdHd),
              OpcString, "$RdHa, $RdHb, $RdHc, $RdHd",
              [(set (i64 GPRD:$RdHb), (DadaoSelectCC (i64 GPRD:$RdHc), (i64 GPRD:$RdHd),
                                      (imm:$RdHa)))]> {
  }

def CSN     : CONDSET_RRRR<0b00100000, "csn">;
def CSZ     : CONDSET_RRRR<0b00100010, "csz">;
def CSP     : CONDSET_RRRR<0b00100100, "csp">;

// -------------------------------------------------- //
// WYDE instructions
// -------------------------------------------------- //

class ORW_RWII<bits<8> majorop, bits<2> wpos, RegisterClass GPRx, PatLeaf wposExt, wposPred Pred>
  : Inst_RWII<majorop, wpos, (outs GPRx:$RxHa_wb), (ins GPRx:$RxHa, Pred:$WPos, wposExt:$Imm16),
              "orw", "$RxHa, $WPos, $Imm16",
              [(set GPRx:$RxHa_wb, (or GPRx:$RxHa, wposExt:$Imm16))]> {
  let Constraints = "$RxHa_wb = $RxHa";
}

def ORW_RWII_W0   : ORW_RWII<0b00010100, 0b00, GPRD, i64w0, wposPred0>;
def ORW_RWII_W1   : ORW_RWII<0b00010100, 0b01, GPRD, i64w1, wposPred1>;
def ORW_RWII_W2   : ORW_RWII<0b00010100, 0b10, GPRD, i64w2, wposPred2>;
def ORW_RWII_W3   : ORW_RWII<0b00010100, 0b11, GPRD, i64w3, wposPred3>;

def ORW_RB_RWII_W0 : ORW_RWII<0b01001100, 0b00, GPRB, i64w0, wposPred0>;
def ORW_RB_RWII_W1 : ORW_RWII<0b01001100, 0b01, GPRB, i64w1, wposPred1>;
def ORW_RB_RWII_W2 : ORW_RWII<0b01001100, 0b10, GPRB, i64w2, wposPred2>;
def ORW_RB_RWII_W3 : ORW_RWII<0b01001100, 0b11, GPRB, i64w3, wposPred3>;

class ANDNW_RWII<bits<8> majorop, bits<2> wpos, RegisterClass GPRx, PatLeaf wposExt, wposPred Pred>
  : Inst_RWII<majorop, wpos, (outs GPRx:$RxHa_wb), (ins GPRx:$RxHa, Pred:$WPos, wposExt:$Imm16),
            "andnw", "$RxHa, $WPos, $Imm16",
            [(set GPRx:$RxHa_wb, (and (i64 GPRx:$RxHa), (not wposExt:$Imm16)))]> {
  let Constraints = "$RxHa_wb = $RxHa";
}

def ANDNW_RWII_W0 : ANDNW_RWII<0b00010101, 0b00, GPRD, i64w0, wposPred0>;
def ANDNW_RWII_W1 : ANDNW_RWII<0b00010101, 0b01, GPRD, i64w1, wposPred1>;
def ANDNW_RWII_W2 : ANDNW_RWII<0b00010101, 0b10, GPRD, i64w2, wposPred2>;
def ANDNW_RWII_W3 : ANDNW_RWII<0b00010101, 0b11, GPRD, i64w3, wposPred3>;

def ANDNW_RB_RWII_W0 : ANDNW_RWII<0b01001101, 0b00, GPRB, i64w0, wposPred0>;
def ANDNW_RB_RWII_W1 : ANDNW_RWII<0b01001101, 0b01, GPRB, i64w1, wposPred1>;
def ANDNW_RB_RWII_W2 : ANDNW_RWII<0b01001101, 0b10, GPRB, i64w2, wposPred2>;
def ANDNW_RB_RWII_W3 : ANDNW_RWII<0b01001101, 0b11, GPRB, i64w3, wposPred3>;

class SETZW_RWII<bits<8> majorop, bits<2> wpos, RegisterClass GPRx, PatLeaf wposExt, wposPred Pred>
  : Inst_RWII<majorop, wpos, (outs GPRx:$RxHa), (ins Pred:$WPos, wposExt:$Imm16),
              "setzw", "$RxHa, $WPos, $Imm16",
              [(set GPRx:$RxHa, wposExt:$Imm16)]>;

def SETZW_RWII_W0   : SETZW_RWII<0b00010110, 0b00, GPRD, i64w0, wposPred0>;
def SETZW_RWII_W1   : SETZW_RWII<0b00010110, 0b01, GPRD, i64w1, wposPred1>;
def SETZW_RWII_W2   : SETZW_RWII<0b00010110, 0b10, GPRD, i64w2, wposPred2>;
def SETZW_RWII_W3   : SETZW_RWII<0b00010110, 0b11, GPRD, i64w3, wposPred3>;

def SETZW_RB_RWII_W0 : SETZW_RWII<0b01001110, 0b00, GPRB, i64w0, wposPred0>;
def SETZW_RB_RWII_W1 : SETZW_RWII<0b01001110, 0b01, GPRB, i64w1, wposPred1>;
def SETZW_RB_RWII_W2 : SETZW_RWII<0b01001110, 0b10, GPRB, i64w2, wposPred2>;
def SETZW_RB_RWII_W3 : SETZW_RWII<0b01001110, 0b11, GPRB, i64w3, wposPred3>;

class SETOW_RWII<bits<8> majorop, bits<2> wpos, PatLeaf wposExt, wposPred Pred>
  : Inst_RWII<majorop, wpos, (outs GPRD:$RxHa), (ins Pred:$WPos, wposExt:$Imm16),
              "setow", "$RxHa, $WPos, $Imm16",
              [(set GPRD:$RxHa, (and (not 0), wposExt:$Imm16))]>;

def SETOW_RWII_W0   : SETOW_RWII<0b00010111, 0b00, i64w0, wposPred0>;
def SETOW_RWII_W1   : SETOW_RWII<0b00010111, 0b01, i64w1, wposPred1>;
def SETOW_RWII_W2   : SETOW_RWII<0b00010111, 0b10, i64w2, wposPred2>;
def SETOW_RWII_W3   : SETOW_RWII<0b00010111, 0b11, i64w3, wposPred3>;

// arbitrary immediate
def : Pat<(i64 imm:$imm), (ORW_RWII_W3 (ORW_RWII_W2 (ORW_RWII_W1 (SETZW_RWII_W0
                          (Wyde0 imm:$imm)), (Wyde1 imm:$imm)), (Wyde2 imm:$imm)), (Wyde3 imm:$imm))>;

// -------------------------------------------------- //
// LOAD instructions
// -------------------------------------------------- //

class LOAD_RRII<bits<8> majorop, string OpcString, RegisterClass GPRx, PatFrag OpNode, ValueType Ty>
  : Inst_RRII<majorop, (outs GPRx:$RxHa), (ins MEM_RRII:$src),
            OpcString, "$RxHa, $src",
            [(set (Ty GPRx:$RxHa), (OpNode ADDR_RRII:$src))]> {
  bits<18> src;
  let RyHb  = src{17 - 12};
  let Imm12 = src{11 -  0};
  let mayLoad = 1;
  let isReMaterializable = 1;
}

def LDBS_RRII : LOAD_RRII<0b00110000, "ldbs", GPRD, sextloadi8,  i64>;
def LDWS_RRII : LOAD_RRII<0b00110001, "ldws", GPRD, sextloadi16, i64>;
def LDTS_RRII : LOAD_RRII<0b00110010, "ldts", GPRD, sextloadi32, i64>;
def LDO_RRII  : LOAD_RRII<0b00110011, "ldo",  GPRD, load, i64>;

def LDBU_RRII : LOAD_RRII<0b01000000, "ldbu", GPRD, zextloadi8,  i64>;
def LDWU_RRII : LOAD_RRII<0b01000001, "ldwu", GPRD, zextloadi16, i64>;
def LDTU_RRII : LOAD_RRII<0b01000010, "ldtu", GPRD, zextloadi32, i64>;
def LDRB_RRII : LOAD_RRII<0b01000011, "ldrb", GPRB, load, i64>;

class LOAD_RRRI<bits<8> majorop, string OpcString, RegisterClass GPRx, PatFrag OpNode, ValueType Ty>
  : Inst_RRRI<majorop, (outs GPRx:$RxHa), (ins MEM_RRRI:$src),
            OpcString, "$RxHa, $src",
            [(set (Ty GPRx:$RxHa), (OpNode ADDR_RRRI:$src))]> {
  bits<18> src;
  let RbHb = src{17 - 12};
  let RdHc = src{11 -  6};
  let Imm6 = src{ 5 -  0};
  let mayLoad = 1;
}

// TODO: ONLY imm6 == 0 permitted, fixme later
let Imm6 = 0b000000 in {
  def LDMBS_RRRI : LOAD_RRRI<0b00110100, "ldmbs", GPRD, sextloadi8,  i64>;
  def LDMWS_RRRI : LOAD_RRRI<0b00110101, "ldmws", GPRD, sextloadi16, i64>;
  def LDMTS_RRRI : LOAD_RRRI<0b00110110, "ldmts", GPRD, sextloadi32, i64>;
  def LDMO_RRRI  : LOAD_RRRI<0b00110111, "ldmo",  GPRD, load, i64>;

  def LDMBU_RRRI : LOAD_RRRI<0b01000100, "ldmbu", GPRD, zextloadi8,  i64>;
  def LDMWU_RRRI : LOAD_RRRI<0b01000101, "ldmwu", GPRD, zextloadi16, i64>;
  def LDMTU_RRRI : LOAD_RRRI<0b01000110, "ldmtu", GPRD, zextloadi32, i64>;
  def LDMRB_RRRI : LOAD_RRRI<0b01000111, "ldmrb", GPRB, load, i64>;
}

// -------------------------------------------------- //
// STORE instructions
// -------------------------------------------------- //

class STORE_RRII<bits<8> majorop, string OpcString, RegisterClass GPRx, PatFrag OpNode, ValueType Ty>
  : Inst_RRII<majorop, (outs), (ins GPRx:$RxHa, MEM_RRII:$dst),
            OpcString, "$RxHa, $dst",
            [(OpNode (Ty GPRx:$RxHa), ADDR_RRII:$dst)]> {
  bits<18> dst;
  let RyHb  = dst{17 - 12};
  let Imm12 = dst{11 -  0};
  let mayStore = 1;
}

def STB_RRII  : STORE_RRII<0b00111000, "stb",  GPRD, truncstorei8,  i64>;
def STW_RRII  : STORE_RRII<0b00111001, "stw",  GPRD, truncstorei16, i64>;
def STT_RRII  : STORE_RRII<0b00111010, "stt",  GPRD, truncstorei32, i64>;
def STO_RRII  : STORE_RRII<0b00111011, "sto",  GPRD, store, i64>;
def STRB_RRII : STORE_RRII<0b01001011, "strb", GPRB, store, i64>;

class STORE_RRRI<bits<8> majorop, string OpcString, RegisterClass GPRx, PatFrag OpNode, ValueType Ty>
  : Inst_RRRI<majorop, (outs), (ins GPRx:$RxHa, MEM_RRRI:$dst),
            OpcString, "$RxHa, $dst",
            [(OpNode (Ty GPRx:$RxHa), ADDR_RRRI:$dst)]> {
  bits<18> dst;
  let RbHb = dst{17 - 12};
  let RdHc = dst{11 -  6};
  let Imm6 = dst{ 5 -  0};
  let mayStore = 1;
}

// TODO: ONLY imm6 == 0 permitted, fixme later
let Imm6 = 0b000000 in {
  def STMB_RRRI  : STORE_RRRI<0b00111100, "stmb",  GPRD, truncstorei8,  i64>;
  def STMW_RRRI  : STORE_RRRI<0b00111101, "stmw",  GPRD, truncstorei16, i64>;
  def STMT_RRRI  : STORE_RRRI<0b00111110, "stmt",  GPRD, truncstorei32, i64>;
  def STMO_RRRI  : STORE_RRRI<0b00111111, "stmo",  GPRD, store, i64>;
  def STMRB_RRRI : STORE_RRRI<0b01001111, "stmrb", GPRB, store, i64>;
}

// -------------------------------------------------- //
// BRANCH instructions
// -------------------------------------------------- //

// Branch targets
// TODO: BrTarget SHOULD BE relative addr, NOT absolute addr
def BrTargetAsmOperand : AsmOperandClass {
  let Name = "BrTarget";
}
def BrTarget   : Operand<OtherVT> {
  let ParserMatchClass = BrTargetAsmOperand;
  let EncoderMethod = "getBranchTargetOpValue";
  let DecoderMethod = "decodeBranch";
}

class BRANCH_RIII<bits<8> majorop, string OpcString>
  : Inst_RIII<majorop, (outs), (ins BrTarget:$Imm18, GPRD:$RxHa), OpcString, "$RxHa, $Imm18",
              []> {
    let isBranch = 1;
    let isBarrier = 1;
    let isTerminator = 1;
  }

// TODO: NOT finised
def BRN_RIII  : BRANCH_RIII<0b00101000, "brn">;
def BRNN_RIII : BRANCH_RIII<0b00101001, "brnn">;
def BRZ_RIII  : BRANCH_RIII<0b00101010, "brz">;
def BRNZ_RIII : BRANCH_RIII<0b00101011, "brnz">;
def BRP_RIII  : BRANCH_RIII<0b00101100, "brp">;
def BRNP_RIII : BRANCH_RIII<0b00101101, "brnp">;

class BRANCH_RRII<bits<8> majorop, string OpcString>
  : Inst_RRII<majorop, (outs), (ins BrTarget:$Imm12, GPRD:$RxHa, GPRD:$RyHb), OpcString, "$RxHa, $Imm12",
              []> {
    let isBranch = 1;
    let isBarrier = 1;
    let isTerminator = 1;
  }

def BREQ_RRII : BRANCH_RRII<0b00101110, "breq">;
def BRNE_RRII : BRANCH_RRII<0b00101111, "brne">;

def JUMP_IIII : Inst_IIII<0b01100100, (outs), (ins BrTarget:$Imm24), "jump", "$Imm24", [(br bb:$Imm24)]> {
  let isBranch = 1;
  let isBarrier = 1;
  let isTerminator = 1;
}

def JUMP_RRII : Inst_RRII<0b01100101, (outs), (ins GPRB:$RxHa, GPRD:$RyHb, i64lo12s:$Imm12),
      "jump", "$RxHa, $RyHb, $Imm12",
      [(brind (add (add GPRB:$RxHa, GPRD:$RyHb), i64lo12s:$Imm12))]> {
  let isBranch = 1;
  let isBarrier = 1;
  let isTerminator = 1;
  let isIndirectBranch = 1;
}

// -------------------------------------------------- //
// CALL and RET instructions
// -------------------------------------------------- //

def CallTargetAsmOperand : AsmOperandClass {
  let Name = "CallTarget";
}
def CallTarget : Operand<i64> {
  let ParserMatchClass = CallTargetAsmOperand;
  let EncoderMethod = "getBranchTargetOpValue";
  let DecoderMethod = "decodeBranch";
}

def CALL_IIII : Inst_IIII<0b01101100, (outs), (ins CallTarget:$Imm24), "call", "$Imm24", [(DadaoCall bb:$Imm24)]> {
  let isCall = 1;
  let isCodeGenOnly = 0;
  let Uses = [RASP];
}

def CALL_RRII : Inst_RRII<0b01101101, (outs), (ins GPRB:$RxHa, GPRD:$RyHb, i64lo12s:$Imm12),
      "call", "$RxHa, $RyHb, $Imm12",
      [(DadaoCall GPRB:$RxHa, GPRD:$RyHb, i64lo12s:$Imm12)]> {
  let isCall = 1;
  let isCodeGenOnly = 0;
  let Defs = [RASP];
}

/*
def RET_RIII : Inst_RIII<0b01101110, (outs GPRD:$RxHa), (ins i64lo18s:$Imm18),
                "ret", "$RxHa, $Imm18",
                [(set GPRD:$RxHa, (i64 i64lo18s:$Imm18)), (DadaoRet)]> {
  let isReturn = 1;
  let isTerminator = 1;
  let isBarrier = 1;
  let Uses = [RASP];
}
*/

def _RET_FIXME : Inst_Dadao<(outs), (ins), "ret", "", [(DadaoRet)]> {
  let MajorOpcode = 0b01101110;
  let HexA = 0;
  let HexB = 0;
  let HexC = 0;
  let HexD = 0;
  let isReturn = 1;
  let isTerminator = 1;
  let isBarrier = 1;
  let Uses = [RASP];
}

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

//  These are target-independent nodes, but have target-specific formats.
def SDT_DadaoCallSeqStart : SDCallSeqStart<[SDTCisVT<0, i64>,
                                            SDTCisVT<1, i64>]>;
def SDT_DadaoCallSeqEnd   : SDCallSeqEnd<[SDTCisVT<0, i64>,
                                          SDTCisVT<1, i64>]>;
def SDT_DadaoAdjDynAlloc  : SDTypeProfile<1,  1, [SDTCisVT<0, i64>,
                                                  SDTCisVT<1, i64>]>;

def CallSeqStart     : SDNode<"ISD::CALLSEQ_START", SDT_DadaoCallSeqStart,
                              [SDNPHasChain, SDNPOutGlue]>;
def CallSeqEnd       : SDNode<"ISD::CALLSEQ_END", SDT_DadaoCallSeqEnd,
                              [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue]>;
def DadaoHi          : SDNode<"DadaoISD::HI", SDTIntUnaryOp>;
def DadaoLo          : SDNode<"DadaoISD::LO", SDTIntUnaryOp>;
def DadaoAdjDynAlloc : SDNode<"DadaoISD::ADJDYNALLOC", SDT_DadaoAdjDynAlloc>;

// ADJCALLSTACKDOWN/UP implicitly use/def SP because they may be expanded into
// a stack adjustment and the codegen must know that they may modify the stack
// pointer before prolog-epilog rewriting occurs.
// Pessimistically assume ADJCALLSTACKDOWN / ADJCALLSTACKUP will become
// sub / add which can clobber SP.
let Defs = [RBSP], Uses = [RBSP] in {
  def ADJCALLSTACKDOWN : Pseudo<(outs), (ins i64imm:$amt1, i64imm:$amt2),
                                "#ADJCALLSTACKDOWN", "$amt1 $amt2",
                                [(CallSeqStart timm:$amt1, timm:$amt2)]>;
  def ADJCALLSTACKUP   : Pseudo<(outs), (ins i64imm:$amt1, i64imm:$amt2),
                                "#ADJCALLSTACKUP", "$amt1 $amt2",
                                [(CallSeqEnd timm:$amt1, timm:$amt2)]>;
  def ADJDYNALLOC      : Pseudo<(outs GPRD:$dst), (ins GPRD:$src),
                                "#ADJDYNALLOC", "$dst $src",
                                [(set GPRD:$dst, (DadaoAdjDynAlloc GPRD:$src))]>;
}

//===----------------------------------------------------------------------===//
// Non-Instruction Patterns
//===----------------------------------------------------------------------===//

// Calls
def : Pat<(DadaoCall tglobaladdr:$dst), (CALL_IIII tglobaladdr:$dst)>;
def : Pat<(DadaoCall texternalsym:$dst), (CALL_IIII texternalsym:$dst)>;

// Loads
def : Pat<(extloadi8  ADDR_RRII:$src), (i64 (LDBU_RRII ADDR_RRII:$src))>;
def : Pat<(extloadi16 ADDR_RRII:$src), (i64 (LDWU_RRII ADDR_RRII:$src))>;
def : Pat<(extloadi32 ADDR_RRII:$src), (i64 (LDTU_RRII ADDR_RRII:$src))>;
// Loads up to 32-bits are already atomic.
// TODO: This is a workaround for a particular failing case and should be
// handled more generally.
def : Pat<(atomic_load_8  ADDR_RRII:$src), (i64 (LDBU_RRII ADDR_RRII:$src))>;

// GlobalAddress, ExternalSymbol, Jumptable, ConstantPool
def : Pat<(DadaoHi tglobaladdr:$dst),   (SETZW_RWII_W1 tglobaladdr:$dst)>;
def : Pat<(DadaoLo tglobaladdr:$dst),   (ORW_RWII_W0 (i64 RD0), tglobaladdr:$dst)>;
def : Pat<(DadaoHi texternalsym:$dst),  (SETZW_RWII_W1 texternalsym:$dst)>;
def : Pat<(DadaoLo texternalsym:$dst),  (ORW_RWII_W0 (i64 RD0), texternalsym:$dst)>;
def : Pat<(DadaoHi tblockaddress:$dst), (SETZW_RWII_W1 tblockaddress:$dst)>;
def : Pat<(DadaoLo tblockaddress:$dst), (ORW_RWII_W0 (i64 RD0), tblockaddress:$dst)>;
def : Pat<(DadaoHi tjumptable:$dst),    (SETZW_RWII_W1 tjumptable:$dst)>;
def : Pat<(DadaoLo tjumptable:$dst),    (ORW_RWII_W0 (i64 RD0), tjumptable:$dst)>;
def : Pat<(DadaoHi tconstpool:$dst),    (SETZW_RWII_W1 tconstpool:$dst)>;
def : Pat<(DadaoLo tconstpool:$dst),    (ORW_RWII_W0 (i64 RD0), tconstpool:$dst)>;

def : Pat<(or GPRD:$hi, (DadaoLo tglobaladdr:$lo)),   (ORW_RWII_W0 GPRD:$hi, tglobaladdr:$lo)>;
def : Pat<(or GPRD:$hi, (DadaoLo texternalsym:$lo)),  (ORW_RWII_W0 GPRD:$hi, texternalsym:$lo)>;
def : Pat<(or GPRD:$hi, (DadaoLo tblockaddress:$lo)), (ORW_RWII_W0 GPRD:$hi, tblockaddress:$lo)>;
def : Pat<(or GPRD:$hi, (DadaoLo tjumptable:$lo)),    (ORW_RWII_W0 GPRD:$hi, tjumptable:$lo)>;
def : Pat<(or GPRD:$hi, (DadaoLo tconstpool:$lo)),    (ORW_RWII_W0 GPRD:$hi, tconstpool:$lo)>;
