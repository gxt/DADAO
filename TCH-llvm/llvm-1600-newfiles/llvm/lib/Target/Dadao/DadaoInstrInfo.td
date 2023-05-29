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

// Addressing modes.
def ADDR_RRRI : ComplexPattern<i64, 3, "selectAddrRRRI", [], []>;
def ADDR_RRII : ComplexPattern<i64, 3, "selectAddrRRII", [frameindex], []>;

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
  let MIOperandInfo = (ops GPRB:$base, imm12:$offset, LO12);
  let ParserMatchClass = MemRegImmAsmOperand;
  let PrintMethod   = "printMemRRIIOperand";
}

// -------------------------------------------------- //
// ALU instructions
// -------------------------------------------------- //
class ALU_ORRR<bits<6> minorop, string OpcString, SDNode OpNode>
  : Inst_ORRR<0b00010000, minorop, (outs GPRD:$RdHb), (ins GPRD:$RdHc, GPRD:$RdHd),
            OpcString, "$RdHb, $RdHc, $RdHd",
            [(set GPRD:$RdHb, (OpNode GPRD:$RdHc, GPRD:$RdHd))]>;

let isCommutable = 1 in {
  def AND_ORRR  : ALU_ORRR<0b001000,  "and", and>;
  def ORR_ORRR  : ALU_ORRR<0b001001,  "orr", or>;
  def XOR_ORRR  : ALU_ORRR<0b001010,  "xor", xor>;
  def XNOR_ORRR : Inst_ORRR<0b00010000, 0b001011, (outs GPRD:$RdHb), (ins GPRD:$RdHc, GPRD:$RdHd),
                            "xnor", "$RdHb, $RdHc, $RdHd",
                            [(set GPRD:$RdHb, (not (xor GPRD:$RdHc, GPRD:$RdHd)))]>;
}

class ALU_ORRI<bits<6> minorop, string OpcString, RegisterClass GPRx, RegisterClass GPRy>
  : Inst_ORRI<0b00010000, minorop, (outs GPRx:$RxHb), (ins GPRy:$RyHc),
              OpcString, "$RxHb, $RyHc, 0",
              [(set GPRx:$RxHb, GPRy:$RyHc)]> {
}

// TODO: ONLY imm6 == 0 permitted, fixme later
let Imm6 = 0b000000 in {
  def RD2RD_ORRI : ALU_ORRI<0b101000, "rd2rd", GPRD, GPRD>;
  def RD2RB_ORRI : ALU_ORRI<0b101001, "rd2rb", GPRD, GPRB>;
  def RB2RD_ORRI : ALU_ORRI<0b101010, "rb2rd", GPRB, GPRD>;
  def RB2RB_ORRI : ALU_ORRI<0b101011, "rb2rb", GPRB, GPRB>;
}

// TODO: RdHa SHOULD be handled
class ALU_RRRR<bits<8> majorop, string OpcString, SDNode OpNode>
  : Inst_RRRR<majorop, (outs GPRD:$RdHa, GPRD:$RdHb), (ins GPRD:$RdHc, GPRD:$RdHd),
            OpcString, "$RdHa, $RdHb, $RdHc, $RdHd",
            [(set GPRD:$RdHb, (OpNode GPRD:$RdHc, GPRD:$RdHd)),
             (set GPRD:$RdHa, 0)]> {
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
def ADDI_RRII : ALU_RRII<0b00011001, "addi", add, GPRD, GPRD, i64lo12z>;
def ADDI_RB_RRII : ALU_RRII<0b01001001, "addi", add, GPRB, GPRB, i64lo12z>;

// -------------------------------------------------- //
// WYDE instructions
// -------------------------------------------------- //

class ORW_RWII<bits<8> majorop, bits<2> wpos, string OpcString, string WPosString,
              RegisterClass GPRx, PatLeaf wposExt>
  : Inst_RWII<majorop, wpos, (outs GPRx:$RxHa_wb), (ins GPRx:$RxHa, wposExt:$Imm16),
              OpcString, "$RxHa, " # WPosString # ", $Imm16",
              [(set GPRD:$RxHa_wb, (or GPRD:$RxHa, wposExt:$Imm16))]> {
  let Constraints = "$RxHa_wb = $RxHa";
}

def ORW_RWII_W0   : ORW_RWII<0b00010100, 0b00, "orw", "w0", GPRD, i64w0>;
def ORW_RWII_W1   : ORW_RWII<0b00010100, 0b01, "orw", "w1", GPRD, i64w1>;
def ORW_RWII_W2   : ORW_RWII<0b00010100, 0b10, "orw", "w2", GPRD, i64w2>;
def ORW_RWII_W3   : ORW_RWII<0b00010100, 0b11, "orw", "w3", GPRD, i64w3>;

class ANDNW_RWII<bits<8> majorop, bits<2> wpos, string OpcString, string WPosString,
                  RegisterClass GPRx, PatLeaf wposExt>
  : Inst_RWII<majorop, wpos, (outs GPRx:$RxHa_wb), (ins GPRx:$RxHa, wposExt:$Imm16),
            OpcString, "$RxHa, " # WPosString # ", $Imm16",
            [(set GPRD:$RxHa_wb, (and (i64 GPRD:$RxHa), (not wposExt:$Imm16)))]> {
  let Constraints = "$RxHa_wb = $RxHa";
}

def ANDNW_RWII_W0 : ANDNW_RWII<0b00010101, 0b00, "andnw", "w0", GPRD, i64w0>;
def ANDNW_RWII_W1 : ANDNW_RWII<0b00010101, 0b01, "andnw", "w1", GPRD, i64w1>;
def ANDNW_RWII_W2 : ANDNW_RWII<0b00010101, 0b10, "andnw", "w2", GPRD, i64w2>;
def ANDNW_RWII_W3 : ANDNW_RWII<0b00010101, 0b11, "andnw", "w3", GPRD, i64w3>;

class SETZW_RWII<bits<8> majorop, bits<2> wpos, string OpcString, string WPosString,
              RegisterClass GPRx, PatLeaf wposExt>
  : Inst_RWII<majorop, wpos, (outs GPRx:$RxHa), (ins wposExt:$Imm16),
              OpcString, "$RxHa, " # WPosString # ", $Imm16",
              [(set GPRD:$RxHa, wposExt:$Imm16)]>;

def SETZW_RWII_W0   : SETZW_RWII<0b00010110, 0b00, "setzw", "w0", GPRD, i64w0>;
def SETZW_RWII_W1   : SETZW_RWII<0b00010110, 0b01, "setzw", "w1", GPRD, i64w1>;
def SETZW_RWII_W2   : SETZW_RWII<0b00010110, 0b10, "setzw", "w2", GPRD, i64w2>;
def SETZW_RWII_W3   : SETZW_RWII<0b00010110, 0b11, "setzw", "w3", GPRD, i64w3>;

class SETOW_RWII<bits<8> majorop, bits<2> wpos, string OpcString, string WPosString,
              RegisterClass GPRx, PatLeaf wposExt>
  : Inst_RWII<majorop, wpos, (outs GPRx:$RxHa), (ins wposExt:$Imm16),
              OpcString, "$RxHa, " # WPosString # ", $Imm16",
              [(set GPRD:$RxHa, (and (not 0), wposExt:$Imm16))]>;

def SETOW_RWII_W0   : SETOW_RWII<0b00010111, 0b00, "setow", "w0", GPRD, i64w0>;
def SETOW_RWII_W1   : SETOW_RWII<0b00010111, 0b01, "setow", "w1", GPRD, i64w1>;
def SETOW_RWII_W2   : SETOW_RWII<0b00010111, 0b10, "setow", "w2", GPRD, i64w2>;
def SETOW_RWII_W3   : SETOW_RWII<0b00010111, 0b11, "setow", "w3", GPRD, i64w3>;

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

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------

// -------------------------------------------------- //
// Instruction Operands and Patterns
// -------------------------------------------------- //

//  These are target-independent nodes, but have target-specific formats.
def SDT_DadaoCallSeqStart : SDCallSeqStart<[SDTCisVT<0, i64>,
                                            SDTCisVT<1, i64>]>;
def SDT_DadaoCallSeqEnd   : SDCallSeqEnd<[SDTCisVT<0, i64>,
                                          SDTCisVT<1, i64>]>;
def SDT_DadaoCall         : SDTypeProfile<0, -1, [SDTCisVT<0, i64>]>;
def SDT_DadaoSetFlag      : SDTypeProfile<0,  2, [SDTCisSameAs<0, 1>]>;
def SDT_DadaoSelectCC     : SDTypeProfile<1,  3, [SDTCisSameAs<0, 1>,
                                                  SDTCisSameAs<1, 2>]>;
def SDT_DadaoSetCC        : SDTypeProfile<1,  1, [SDTCisVT<0, i64>,
                                                  SDTCisVT<1, i64>]>;
def SDT_DadaoBrCC         : SDTypeProfile<0,  2, [SDTCisVT<0, OtherVT>,
                                                  SDTCisVT<1, i64>]>;
def SDT_DadaoAdjDynAlloc  : SDTypeProfile<1,  1, [SDTCisVT<0, i64>,
                                                  SDTCisVT<1, i64>]>;

def Call             : SDNode<"DadaoISD::CALL", SDT_DadaoCall,
                              [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue,
                               SDNPVariadic]>;
def RetFlag          : SDNode<"DadaoISD::RET_FLAG", SDTNone,
                              [SDNPHasChain, SDNPOptInGlue, SDNPVariadic]>;
def CallSeqStart     : SDNode<"ISD::CALLSEQ_START", SDT_DadaoCallSeqStart,
                              [SDNPHasChain, SDNPOutGlue]>;
def CallSeqEnd       : SDNode<"ISD::CALLSEQ_END", SDT_DadaoCallSeqEnd,
                              [SDNPHasChain, SDNPOptInGlue, SDNPOutGlue]>;
def DadaoSetFlag     : SDNode<"DadaoISD::SET_FLAG", SDT_DadaoSetFlag,
                              [SDNPOutGlue]>;
def DadaoBrCC        : SDNode<"DadaoISD::BR_CC", SDT_DadaoBrCC,
                              [SDNPHasChain, SDNPInGlue]>;
def DadaoSelectCC    : SDNode<"DadaoISD::SELECT_CC", SDT_DadaoSelectCC,
                              [SDNPInGlue]>;
def DadaoSetCC       : SDNode<"DadaoISD::SETCC", SDT_DadaoSetCC,
                              [SDNPInGlue]>;
def DadaoHi          : SDNode<"DadaoISD::HI", SDTIntUnaryOp>;
def DadaoLo          : SDNode<"DadaoISD::LO", SDTIntUnaryOp>;
def DadaoAdjDynAlloc : SDNode<"DadaoISD::ADJDYNALLOC", SDT_DadaoAdjDynAlloc>;

// Extract bits 16-31 (high-end) of an immediate value.
// Transformation function: shift the immediate value down into the low bits.
def HI16 : SDNodeXForm<imm, [{
  return CurDAG->getTargetConstant((uint64_t)N->getZExtValue() >> 16, SDLoc(N),
                                   MVT::i64);
}]>;

def NEG : SDNodeXForm<imm, [{
  return CurDAG->getTargetConstant(-N->getSExtValue(), SDLoc(N), MVT::i64);
}]>;

// Branch targets
def BrTargetAsmOperand : AsmOperandClass {
  let Name = "BrTarget";
}
def BrTarget   : Operand<OtherVT> {
  let ParserMatchClass = BrTargetAsmOperand;
  let EncoderMethod = "getBranchTargetOpValue";
  let DecoderMethod = "decodeBranch";
}

def CallTargetAsmOperand : AsmOperandClass {
  let Name = "CallTarget";
}
def CallTarget : Operand<i64> {
  let ParserMatchClass = CallTargetAsmOperand;
  let EncoderMethod = "getBranchTargetOpValue";
  let DecoderMethod = "decodeBranch";
}

def ImmShiftAsmOperand : AsmOperandClass { let Name = "ImmShift"; }
def immShift : Operand<i64>, PatLeaf<(imm), [{
    int Imm = N->getSExtValue();
    return Imm >= -31 && Imm <= 31;}]> {
  let ParserMatchClass = ImmShiftAsmOperand;
  let DecoderMethod = "decodeShiftImm";
}

def i64neg16 : Operand<i64>, PatLeaf<(i64 imm), [{
    // i64neg16 predicate - true if the 32-bit immediate is negative and can
    // be represented by a 16 bit integer.
    int Imm = N->getSExtValue();
    return (Imm < 0) && (isInt<16>(Imm));}], LO16> {
  let ParserMatchClass = LoImm16AsmOperand;
}

def LoImm16AndAsmOperand : AsmOperandClass { let Name = "LoImm16And"; }
def i64lo16and : Operand<i64>, PatLeaf<(i64 imm), [{
    // i64lo16 predicate - true if the 32-bit immediate has the rightmost 16
    // bits set and the leftmost 16 bits 1's.
    return (N->getZExtValue() >= 0xFFFF0000UL);}], LO16> {
  let ParserMatchClass = LoImm16AndAsmOperand;
  let PrintMethod = "printLo16AndImmOperand";
}

def HiImm16AsmOperand : AsmOperandClass { let Name = "HiImm16"; }
def i64hi16 : Operand<i64>, PatLeaf<(i64 imm), [{
    // i64hi16 predicate - true if the 32-bit immediate has only leftmost 16
    // bits set.
    return ((N->getZExtValue() & 0xFFFF0000UL) == N->getZExtValue());}], HI16> {
  let ParserMatchClass = HiImm16AsmOperand;
  let PrintMethod = "printHi16ImmOperand";
}

def HiImm16AndAsmOperand : AsmOperandClass { let Name = "HiImm16And"; }
def i64hi16and : Operand<i64>, PatLeaf<(i64 imm), [{
    // i64lo16 predicate - true if the 32-bit immediate has the leftmost 16
    // bits set and the rightmost 16 bits 1's.
    return ((N->getZExtValue() & 0xFFFFUL) == 0xFFFFUL);}], HI16> {
  let ParserMatchClass = HiImm16AndAsmOperand;
  let PrintMethod = "printHi16AndImmOperand";
}

def AluOp : Operand<i64> {
  let PrintMethod = "printAluOperand";
}

def CCOp : Operand<i64> {
  let PrintMethod = "printCCOperand";
}

// Predicate operand. Default to 0 = true.
def CondCodeOperand : AsmOperandClass { let Name = "CondCode"; }

def pred : PredicateOperand<i64, (ops i64imm), (ops (i64 0))> {
  let PrintMethod = "printPredicateOperand";
  let ParserMatchClass = CondCodeOperand;
  let DecoderMethod = "decodePredicateOperand";
}

let hasSideEffects = 0, Inst = 0x00000001 in
  def NOP : InstDadao<(outs), (ins), "nop", []>;

// Special NOPs to change logging level in vdadao.
let hasSideEffects = 0, Inst = 0x00000002 in
  def LOG0 : InstDadao<(outs), (ins), "log_0", []>;
let hasSideEffects = 0, Inst = 0x00000003 in
  def LOG1 : InstDadao<(outs), (ins), "log_1", []>;
let hasSideEffects = 0, Inst = 0x00000004 in
  def LOG2 : InstDadao<(outs), (ins), "log_2", []>;
let hasSideEffects = 0, Inst = 0x00000005 in
  def LOG3 : InstDadao<(outs), (ins), "log_3", []>;
let hasSideEffects = 0, Inst = 0x00000006 in
  def LOG4 : InstDadao<(outs), (ins), "log_4", []>;

// -------------------------------------------------- //
// ALU instructions
// -------------------------------------------------- //
multiclass ALUbase<bits<3> subOp, string AsmStr,
                   PatLeaf LoExt, PatLeaf HiExt,
                   list<dag> loPattern, list<dag> hiPattern> {
  // Register Immediate
  let H = 0 in
    def LO : InstRI<subOp, (outs GPR:$Rd), (ins GPR:$Rs1, LoExt:$imm16),
                    !strconcat(AsmStr, "\t$Rs1, $imm16, $Rd"),
                    loPattern>;
  let H = 1 in
    def HI : InstRI<subOp, (outs GPR:$Rd), (ins GPR:$Rs1, HiExt:$imm16),
                    !strconcat(AsmStr, "\t$Rs1, $imm16, $Rd"),
                    hiPattern>;

}

multiclass ALUarith<bits<3> subOp, string AsmStr, SDNode OpNode,
                    PatLeaf LoExt, PatLeaf HiExt> {
  defm I_ : ALUbase<subOp, AsmStr, LoExt, HiExt, [], []>;

  // Register Register
  let JJJJJ = 0 in
    def R : InstRR<subOp, (outs GPR:$Rd), (ins GPR:$Rs1, GPR:$Rs2, pred:$DDDI),
                   !strconcat(AsmStr, "$DDDI\t$Rs1, $Rs2, $Rd"),
                   [(set GPR:$Rd, (OpNode GPR:$Rs1, GPR:$Rs2))]>;
}

multiclass ALUlogic<bits<3> subOp, string AsmStr, SDNode OpNode,
                    PatLeaf LoExt, PatLeaf HiExt> {
  defm I_ : ALUbase<subOp, AsmStr, LoExt, HiExt,
                    [(set GPR:$Rd, (OpNode GPR:$Rs1, LoExt:$imm16))],
                    [(set GPR:$Rd, (OpNode GPR:$Rs1, HiExt:$imm16))]>;

  // Register Register
  let JJJJJ = 0 in
    def R : InstRR<subOp, (outs GPR:$Rd), (ins GPR:$Rs1, GPR:$Rs2, pred:$DDDI),
                   !strconcat(AsmStr, "$DDDI\t$Rs1, $Rs2, $Rd"),
                   [(set GPR:$Rd, (OpNode GPR:$Rs1, GPR:$Rs2))]>;
}

// Non flag setting ALU operations
let isAsCheapAsAMove = 1, F = 0 in {
  let isCommutable = 1 in {
    defm ADD_ : ALUarith<0b000, "add", add, i64lo16z, i64hi16>;
  }
  defm SUB_ : ALUarith<0b010,   "sub", sub, i64lo16z, i64hi16>;
  let isCommutable = 1 in {
    defm OR_  : ALUlogic<0b101,  "or",  or, i64lo16z, i64hi16>;
    defm XOR_ : ALUlogic<0b110, "xor", xor, i64lo16z, i64hi16>;
  }
}

def : Pat<(add GPR:$Rs1, i64lo16z:$imm),
          (ADD_I_LO GPR:$Rs1, i64lo16z:$imm)>;

def : Pat<(sub GPR:$Rs1, i64lo16z:$imm),
          (SUB_I_LO GPR:$Rs1, i64lo16z:$imm)>;

def : Pat<(add GPR:$Rs1, i64hi16:$imm),
          (ADD_I_HI GPR:$Rs1, i64hi16:$imm)>;

def : Pat<(sub GPR:$Rs1, i64hi16:$imm),
          (SUB_I_HI GPR:$Rs1, i64hi16:$imm)>;

// Change add/sub with negative number to sub/add
def : Pat<(add GPR:$Rs1, i64neg16:$imm),
          (SUB_I_LO GPR:$Rs1, (NEG $imm))>;
def : Pat<(sub GPR:$Rs1, i64neg16:$imm),
          (ADD_I_LO GPR:$Rs1, (NEG $imm))>;

// Flag (incl. carry) setting addition and subtraction
let F = 1, Defs = [SR] in {
  defm ADD_F_ : ALUarith<0b000, "add.f", addc, i64lo16z, i64hi16>;
  defm SUB_F_ : ALUarith<0b010, "sub.f", subc, i64lo16z, i64hi16>;
}

def : Pat<(addc GPR:$Rs1, i64lo16z:$imm),
          (ADD_F_I_LO GPR:$Rs1, i64lo16z:$imm)>;

def : Pat<(subc GPR:$Rs1, i64lo16z:$imm),
          (SUB_F_I_LO GPR:$Rs1, i64lo16z:$imm)>;

def : Pat<(addc GPR:$Rs1, i64hi16:$imm),
          (ADD_F_I_HI GPR:$Rs1, i64hi16:$imm)>;

def : Pat<(subc GPR:$Rs1, i64hi16:$imm),
          (SUB_F_I_HI GPR:$Rs1, i64hi16:$imm)>;

// Flag setting ALU operations
let isAsCheapAsAMove = 1, F = 1, Defs = [SR] in {
  let isCommutable = 1 in {
    defm OR_F_  : ALUlogic<0b101,  "or.f",   or, i64lo16z, i64hi16>;
    defm XOR_F_ : ALUlogic<0b110, "xor.f",  xor, i64lo16z, i64hi16>;
  }
}

def : InstAlias<"mov $src, $dst", (ADD_R GPR:$dst, GPR:$src, R0, 0)>;

let isAsCheapAsAMove = 1, Rs1 = R0.Num, isCodeGenOnly = 1, H = 1, F = 0,
  isReMaterializable = 1 in
  def MOVHI : InstRI<0b000, (outs GPR:$Rd), (ins i64hi16:$imm16),
                     "mov\t$imm16, $Rd",
                     [(set GPR:$Rd, i64hi16:$imm16)]>;

def : InstAlias<"mov $imm16, $dst", (ADD_I_LO GPR:$dst, R0, i64lo16z:$imm16)>;
def : InstAlias<"mov $imm16, $dst", (ADD_I_HI GPR:$dst, R0, i64hi16:$imm16)>;

// Shift instructions
class ShiftRI<string AsmStr, list<dag> Pattern>
  : InstRI<0b111, (outs GPR:$Rd), (ins GPR:$Rs1, immShift:$imm16),
           !strconcat(AsmStr, "\t$Rs1, $imm16, $Rd"), Pattern> {
  let isReMaterializable = 1;
}

let F = 0 in {
  let H = 0 in
    def SL_I : ShiftRI<"sh", [(set GPR:$Rd, (shl GPR:$Rs1, immShift:$imm16))]>;
  let H = 1 in
    def SA_I : ShiftRI<"sha", []>;
}
def : Pat<(srl GPR:$Rs1, immShift:$imm), (SL_I GPR:$Rs1, (NEG $imm))>;
def : Pat<(sra GPR:$Rs1, immShift:$imm), (SA_I GPR:$Rs1, (NEG $imm))>;

let F = 1, Defs = [SR] in {
  let H = 0 in
    def SL_F_I : ShiftRI<"sh.f", []>;
  let H = 1 in
    def SA_F_I : ShiftRI<"sha.f", []>;
}

class ShiftRR<string AsmStr, list<dag> Pattern>
  : InstRR<0b111, (outs GPR:$Rd), (ins GPR:$Rs1, GPR:$Rs2, pred:$DDDI), AsmStr,
           Pattern>;

let F = 0 in {
  let JJJJJ = 0b10000 in
    def SHL_R : ShiftRR<"sh$DDDI\t$Rs1, $Rs2, $Rd",
                        [(set GPR:$Rd, (shl GPR:$Rs1, GPR:$Rs2))]>;
  let isCodeGenOnly = 1 in {
    let JJJJJ = 0b10000 in
      def SRL_R : ShiftRR<"sh$DDDI\t$Rs1, $Rs2, $Rd", []>;
  }
  let JJJJJ = 0b11000 in
    def SRA_R : ShiftRR<"sha$DDDI\t$Rs1, $Rs2, $Rd", []>;
}

let F = 1, Defs = [SR] in {
  let JJJJJ = 0b10000 in
    def SHL_F_R : ShiftRR<"sh.f$DDDI\t$Rs1, $Rs2, $Rd", []>;
  let isCodeGenOnly = 1 in {
    let JJJJJ = 0b10000 in
      def SRL_F_R : ShiftRR<"sh.f$DDDI\t$Rs1, $Rs2, $Rd", []>;
  }
  let JJJJJ = 0b11000 in
    def SRA_F_R : ShiftRR<"sha.f$DDDI\t$Rs1, $Rs2, $Rd", []>;
}

// Expand shift-right operations
def : Pat<(srl GPR:$Rs1, GPR:$Rs2),
          (SRL_R GPR:$Rs1, (SUB_R R0, GPR:$Rs2))>;
def : Pat<(sra GPR:$Rs1, GPR:$Rs2),
          (SRA_R GPR:$Rs1, (SUB_R R0, GPR:$Rs2))>;

// -------------------------------------------------- //
// BRANCH instructions
// -------------------------------------------------- //

let isBranch = 1, isBarrier = 1, isTerminator = 1 in {
  def BT : InstBR<(outs), (ins BrTarget:$addr),
                  "bt\t$addr",
                  [(br bb:$addr)]> {
    let DDDI = 0b0000;
  }
  let Uses = [SR] in
    def BRCC : InstBR<(outs), (ins BrTarget:$addr, CCOp:$DDDI),
                      "b$DDDI\t$addr",
                      [(DadaoBrCC bb:$addr, imm:$DDDI)]>;

  let isIndirectBranch = 1 in {
    def JR : InstRR<0b101, (outs), (ins GPR:$Rs2), "bt\t$Rs2",
                    [(brind GPR:$Rs2)]> {
      let Rs1 = R0.Num;
      let Rd = R2.Num;
      let F = 0;
      let JJJJJ = 0;
      let DDDI = 0;
    }
  }
}

// -------------------------------------------------- //
// Condition/SF instructions
// -------------------------------------------------- //

// Instructions to set flags used in lowering comparisons.
multiclass SF<bits<3> op2Val, string AsmStr> {
  let F = 1, Rd = R0.Num, JJJJJ = 0, Defs = [SR], DDDI = 0 in
    def _RR : InstRR<op2Val, (outs), (ins GPR:$Rs1, GPR:$Rs2),
                     !strconcat(AsmStr, "\t$Rs1, $Rs2, %r0"),
                     [(DadaoSetFlag (i64 GPR:$Rs1), (i64 GPR:$Rs2))]>;
  let F = 1, Rd = R0.Num, H = 0, Defs = [SR] in
    def _RI_LO : InstRI<op2Val, (outs), (ins GPR:$Rs1, i64lo16z:$imm16),
                     !strconcat(AsmStr, "\t$Rs1, $imm16, %r0"),
                     [(DadaoSetFlag (i64 GPR:$Rs1), i64lo16z:$imm16)]>;
  let F = 1, Rd = R0.Num, H = 1, Defs = [SR] in
    def _RI_HI : InstRI<op2Val, (outs), (ins GPR:$Rs1, i64hi16:$imm16),
                     !strconcat(AsmStr, "\t$Rs1, $imm16, %r0"),
                     [(DadaoSetFlag (i64 GPR:$Rs1), i64hi16:$imm16)]>;
}
let isCodeGenOnly = 1, isCompare = 1 in {
  defm SFSUB_F : SF<0b010, "sub.f">;
}

// Jump and link
let isCall = 1, isCodeGenOnly = 1, Uses = [SP],
    Defs = [RCA] in {
  def CALL : Pseudo<(outs), (ins CallTarget:$addr), "", []>;
  def CALLR : Pseudo<(outs), (ins GPR:$Rs1), "", [(Call GPR:$Rs1)]>;
}

let isReturn = 1, isTerminator = 1, isBarrier = 1,
    Uses = [RCA] in {
  def RET : InstRM<0b0, (outs), (ins),
                   "ld\t-4[%fp], %pc ! return",
                   [(RetFlag)]> {
    let Rd = PC.Num;
    let Rs1 = FP.Num;
    let P = 1;
    let Q = 0;
    let imm16 = -4;
  }
}

// ADJCALLSTACKDOWN/UP implicitly use/def SP because they may be expanded into
// a stack adjustment and the codegen must know that they may modify the stack
// pointer before prolog-epilog rewriting occurs.
// Pessimistically assume ADJCALLSTACKDOWN / ADJCALLSTACKUP will become
// sub / add which can clobber SP.
let Defs = [SP], Uses = [SP] in {
  def ADJCALLSTACKDOWN : Pseudo<(outs), (ins i64imm:$amt1, i64imm:$amt2),
                                "#ADJCALLSTACKDOWN $amt1 $amt2",
                                [(CallSeqStart timm:$amt1, timm:$amt2)]>;
  def ADJCALLSTACKUP   : Pseudo<(outs), (ins i64imm:$amt1, i64imm:$amt2),
                                "#ADJCALLSTACKUP $amt1 $amt2",
                                [(CallSeqEnd timm:$amt1, timm:$amt2)]>;
}

let Defs = [SP], Uses = [SP] in {
  def ADJDYNALLOC : Pseudo<(outs GPR:$dst), (ins GPR:$src),
                           "#ADJDYNALLOC $dst $src",
                           [(set GPR:$dst, (DadaoAdjDynAlloc GPR:$src))]>;
}

let Uses = [SR] in {
  def SCC : InstSCC<(outs GPR:$Rs1), (ins CCOp:$DDDI),
                    "s$DDDI\t$Rs1",
                    [(set (i64 GPR:$Rs1), (DadaoSetCC imm:$DDDI))]>;
}

// Select with hardware support
let Uses = [SR], isSelect = 1 in {
  def SELECT : InstRR<0b111, (outs GPR:$Rd),
                      (ins GPR:$Rs1, GPR:$Rs2, CCOp:$DDDI),
                      "sel.$DDDI $Rs1, $Rs2, $Rd",
                      [(set (i64 GPR:$Rd),
                       (DadaoSelectCC (i64 GPR:$Rs1), (i64 GPR:$Rs2),
                                      (imm:$DDDI)))]> {
    let JJJJJ = 0;
    let F = 0;
  }
}

let isBranch = 1, isBarrier = 1, isTerminator = 1,
    isIndirectBranch = 1, Uses = [SR] in {
  def BRIND_CC : InstRR<0b101, (outs), (ins GPR:$Rs1, CCOp:$DDDI),
                        "b$DDDI\t$Rs1", []> {
    let F = 0;
    let JJJJJ = 0;
    let Rd = PC.Num;
    let Rs2 = R0.Num;
  }

  def BRIND_CCA : InstRR<0b101, (outs), (ins GPR:$Rs1, GPR:$Rs2, CCOp:$DDDI),
                         "b${DDDI}\t$Rs1 add $Rs2", []> {
    let F = 0;
    let Rd = PC.Num;
    let JJJJJ = 0;
  }
}

// TODO: This only considers the case where BROFF is an immediate and not where
// it is a register. Add support for register relative branching.
let isBranch = 1, isBarrier = 1, isTerminator = 1, Rs1 = 0,
    Uses = [SR] in
  def BRR : InstBRR<(outs), (ins i16imm:$imm16, CCOp:$DDDI),
                    "b${DDDI}.r\t$imm16", []>;

let F = 0 in {
// Population Count (POPC)
def POPC: InstSpecial<0b001, (outs GPR:$Rd), (ins GPR:$Rs1),
                      "popc\t$Rs1, $Rd",
                      [(set GPR:$Rd, (ctpop GPR:$Rs1))]>;

// Count Leading Zeros (LEADZ)
def LEADZ: InstSpecial<0b010, (outs GPR:$Rd), (ins GPR:$Rs1),
                       "leadz\t$Rs1, $Rd", [(set GPR:$Rd, (ctlz GPR:$Rs1))]>;

// Count Trailing Zeros (TRAILZ)
def TRAILZ : InstSpecial<0b011, (outs GPR:$Rd), (ins GPR:$Rs1),
                         "trailz\t$Rs1, $Rd",
                         [(set GPR:$Rd, (cttz GPR:$Rs1))]>;
}

//===----------------------------------------------------------------------===//
// Non-Instruction Patterns
//===----------------------------------------------------------------------===//

// unsigned 16-bit immediate
def : Pat<(i64 i64lo16z:$imm), (OR_I_LO (i64 R0), imm:$imm)>;

// arbitrary immediate
def : Pat<(i64 imm:$imm), (OR_I_LO (MOVHI (HI16 imm:$imm)), (LO16 imm:$imm))>;

// Calls
def : Pat<(Call tglobaladdr:$dst), (CALL tglobaladdr:$dst)>;
def : Pat<(Call texternalsym:$dst), (CALL texternalsym:$dst)>;

// Loads
def : Pat<(extloadi8  ADDR_RRII:$src), (i64 (LDBU_RRII ADDR_RRII:$src))>;
def : Pat<(extloadi16 ADDR_RRII:$src), (i64 (LDWU_RRII ADDR_RRII:$src))>;
def : Pat<(extloadi32 ADDR_RRII:$src), (i64 (LDTU_RRII ADDR_RRII:$src))>;
// Loads up to 32-bits are already atomic.
// TODO: This is a workaround for a particular failing case and should be
// handled more generally.
def : Pat<(atomic_load_8  ADDR_RRII:$src), (i64 (LDBU_RRII ADDR_RRII:$src))>;

// GlobalAddress, ExternalSymbol, Jumptable, ConstantPool
def : Pat<(DadaoHi tglobaladdr:$dst), (MOVHI tglobaladdr:$dst)>;
def : Pat<(DadaoLo tglobaladdr:$dst), (OR_I_LO (i64 R0), tglobaladdr:$dst)>;
def : Pat<(DadaoHi texternalsym:$dst), (MOVHI texternalsym:$dst)>;
def : Pat<(DadaoLo texternalsym:$dst), (OR_I_LO (i64 R0), texternalsym:$dst)>;
def : Pat<(DadaoHi tblockaddress:$dst), (MOVHI tblockaddress:$dst)>;
def : Pat<(DadaoLo tblockaddress:$dst), (OR_I_LO (i64 R0), tblockaddress:$dst)>;
def : Pat<(DadaoHi tjumptable:$dst), (MOVHI tjumptable:$dst)>;
def : Pat<(DadaoLo tjumptable:$dst), (OR_I_LO (i64 R0), tjumptable:$dst)>;
def : Pat<(DadaoHi tconstpool:$dst), (MOVHI tconstpool:$dst)>;
def : Pat<(DadaoLo tconstpool:$dst), (OR_I_LO (i64 R0), tconstpool:$dst)>;

def : Pat<(or GPR:$hi, (DadaoLo tglobaladdr:$lo)),
          (OR_I_LO GPR:$hi, tglobaladdr:$lo)>;
def : Pat<(or GPR:$hi, (DadaoLo texternalsym:$lo)),
          (OR_I_LO GPR:$hi, texternalsym:$lo)>;
def : Pat<(or GPR:$hi, (DadaoLo tblockaddress:$lo)),
          (OR_I_LO GPR:$hi, tblockaddress:$lo)>;
def : Pat<(or GPR:$hi, (DadaoLo tjumptable:$lo)),
          (OR_I_LO GPR:$hi, tjumptable:$lo)>;
def : Pat<(or GPR:$hi, (DadaoLo tconstpool:$lo)),
          (OR_I_LO GPR:$hi, tconstpool:$lo)>;
