// See LICENSE for license details.

package wuming.common

import chisel3._
import chisel3.util._

/* Automatically generated by parse-opcodes */
object Instructions {

   def LDBS		= BitPat("b00110000????????????????????????")
   def LDWS		= BitPat("b00110001????????????????????????")
   def LDTS		= BitPat("b00110010????????????????????????")
   def LDBU		= BitPat("b01000000????????????????????????")
   def LDWU		= BitPat("b01000001????????????????????????")
   def LDTU		= BitPat("b01000010????????????????????????")
   def STB		= BitPat("b00111000????????????????????????")
   def STW		= BitPat("b00111001????????????????????????")
   def LDOrb		= BitPat("b01000011????????????????????????")
   def LDOrd		= BitPat("b00110011????????????????????????")
   def STOrb		= BitPat("b01001011????????????????????????")
   def STOrd		= BitPat("b00111011????????????????????????")
   def LDT		= BitPat("b01010010????????????????????????")
   def LDOrf		= BitPat("b01010011????????????????????????")
   def STTrf		= BitPat("b01011010????????????????????????")
   def STTrd		= BitPat("b00111010????????????????????????")
   def STOrf		= BitPat("b01011011????????????????????????")
   def LDMBS		= BitPat("b00110100????????????????????????")
   def LDMWS		= BitPat("b00110101????????????????????????")
   def LDMTS		= BitPat("b00110110????????????????????????")
   def LDMBU		= BitPat("b01000100????????????????????????")
   def LDMWU		= BitPat("b01000101????????????????????????")
   def LDMTU		= BitPat("b01000110????????????????????????")
   def STMB		= BitPat("b00111100????????????????????????")
   def STMW		= BitPat("b00111101????????????????????????")
   def LDMOrb		= BitPat("b01000111????????????????????????")
   def LDMOrd		= BitPat("b00110111????????????????????????")
   def STMOrb		= BitPat("b01001111????????????????????????")
   def STMOrd		= BitPat("b00111111????????????????????????")
   def LDMT		= BitPat("b01010110????????????????????????")
   def LDMOrf		= BitPat("b01010111????????????????????????")
   def STMTrf		= BitPat("b01011110????????????????????????")
   def STMTrd		= BitPat("b00111110????????????????????????")
   def STMOrf		= BitPat("b01011111????????????????????????")
   def LDMOra		= BitPat("b01100111????????????????????????")
   def STMOra		= BitPat("b01101111????????????????????????")
   def RD2RD		= BitPat("b00010000101000??????????????????")
   def RD2RB		= BitPat("b00010000101001??????????????????")
   def RB2RD		= BitPat("b00010000101010??????????????????")
   def RB2RB		= BitPat("b00010000101011??????????????????")
   def RD2RF		= BitPat("b00010000110001??????????????????")
   def RF2RD		= BitPat("b00010000110010??????????????????")
   def RF2RF		= BitPat("b00010000110011??????????????????")
   def RD2RA		= BitPat("b00010000111001??????????????????")
   def RA2RD		= BitPat("b00010000111010??????????????????")
   def CSP1		= BitPat("b00010000110110??????????????????")
   def CSNP1		= BitPat("b00010000110111??????????????????")
   def CSEQ		= BitPat("b00100110????????????????????????")
   def CSNE		= BitPat("b00100111????????????????????????")
   def CSNrf		= BitPat("b00100001????????????????????????")
   def CSNrd		= BitPat("b00100000????????????????????????")
   def CSZrf		= BitPat("b00100011????????????????????????")
   def CSZrd		= BitPat("b00100010????????????????????????")
   def CSPrf		= BitPat("b00100101????????????????????????")
   def CSPrd		= BitPat("b00100100????????????????????????")
   def SETOW		= BitPat("b00010111????????????????????????")
   def ORWrb		= BitPat("b01001100????????????????????????")
   def ORWrd		= BitPat("b00010100????????????????????????")
   def ANDNWrb		= BitPat("b01001101????????????????????????")
   def ANDNWrd		= BitPat("b00010101????????????????????????")
   def SETZWrb		= BitPat("b01001110????????????????????????")
   def SETZWrd		= BitPat("b00010110????????????????????????")
   def SETW		= BitPat("b01010100????????????????????????")
   def MULS		= BitPat("b00011100????????????????????????")
   def MULU		= BitPat("b00011101????????????????????????")
   def DIVS		= BitPat("b00011110????????????????????????")
   def DIVU		= BitPat("b00011111????????????????????????")
   def ADRP		= BitPat("b01001000????????????????????????")
   def ADDIrb		= BitPat("b01001001????????????????????????")
   def ADDIrd		= BitPat("b00011001????????????????????????")
   def ANDI		= BitPat("b00011000????????????????????????")
   def ADDrb		= BitPat("b00010000101110??????????????????")
   def ADDrd		= BitPat("b00011010????????????????????????")
   def SUBrb		= BitPat("b00010000101111??????????????????")
   def SUBrd		= BitPat("b00011011????????????????????????")
   def CMPSr		= BitPat("b00010000100100??????????????????")
   def CMPSi		= BitPat("b00010010????????????????????????")
   def CMPUr		= BitPat("b00010000100101??????????????????")
   def CMPUi		= BitPat("b00010011????????????????????????")
   def CMP		= BitPat("b00010000101101??????????????????")
   def AND		= BitPat("b00010000001000??????????????????")
   def ORR		= BitPat("b00010000001001??????????????????")
   def XOR		= BitPat("b00010000001010??????????????????")
   def XNOR		= BitPat("b00010000001011??????????????????")
   def SHLUi		= BitPat("b00010000011001??????????????????")
   def SHLUr		= BitPat("b00010000010001??????????????????")
   def SHRSi		= BitPat("b00010000011010??????????????????")
   def SHRSr		= BitPat("b00010000010010??????????????????")
   def SHRUi		= BitPat("b00010000011011??????????????????")
   def SHRUr		= BitPat("b00010000010011??????????????????")
   def EXTSi		= BitPat("b00010000011100??????????????????")
   def EXTSr		= BitPat("b00010000010100??????????????????")
   def EXTZi		= BitPat("b00010000011101??????????????????")
   def EXTZr		= BitPat("b00010000010101??????????????????")
   def FTCLS		= BitPat("b01010000000000??????????????????")
   def FOCLS		= BitPat("b01010000001000??????????????????")
   def FT2FO		= BitPat("b01010000000001??????????????????")
   def FO2FT		= BitPat("b01010000001001??????????????????")
   def FT2IT		= BitPat("b01010000110000??????????????????")
   def FT2IO		= BitPat("b01010000110001??????????????????")
   def FT2UT		= BitPat("b01010000110010??????????????????")
   def FT2UO		= BitPat("b01010000110011??????????????????")
   def FO2IT		= BitPat("b01010000111000??????????????????")
   def FO2IO		= BitPat("b01010000111001??????????????????")
   def FO2UT		= BitPat("b01010000111010??????????????????")
   def FO2UO		= BitPat("b01010000111011??????????????????")
   def IT2FT		= BitPat("b01010000110100??????????????????")
   def IO2FT		= BitPat("b01010000110101??????????????????")
   def UT2FT		= BitPat("b01010000110110??????????????????")
   def UO2FT		= BitPat("b01010000110111??????????????????")
   def IT2FO		= BitPat("b01010000111100??????????????????")
   def IO2FO		= BitPat("b01010000111101??????????????????")
   def UT2FO		= BitPat("b01010000111110??????????????????")
   def UO2FO		= BitPat("b01010000111111??????????????????")
   def FTSQRT		= BitPat("b01010000000110??????????????????")
   def FOSQRT		= BitPat("b01010000001110??????????????????")
   def FTADD		= BitPat("b01010000010000??????????????????")
   def FTSUB		= BitPat("b01010000010001??????????????????")
   def FTMUL		= BitPat("b01010000010010??????????????????")
   def FTDIV		= BitPat("b01010000010011??????????????????")
   def FOADD		= BitPat("b01010000011000??????????????????")
   def FOSUB		= BitPat("b01010000011001??????????????????")
   def FOMUL		= BitPat("b01010000011010??????????????????")
   def FODIV		= BitPat("b01010000011011??????????????????")
   def FTSGNN		= BitPat("b01010000010110??????????????????")
   def FTSGNJ		= BitPat("b01010000010111??????????????????")
   def FOSGNN		= BitPat("b01010000011110??????????????????")
   def FOSGNJ		= BitPat("b01010000011111??????????????????")
   def FTQCMP		= BitPat("b01010000100000??????????????????")
   def FTSCMP		= BitPat("b01010000100001??????????????????")
   def FOQCMP		= BitPat("b01010000101000??????????????????")
   def FOSCMP		= BitPat("b01010000101001??????????????????")
   def FTMADD		= BitPat("b01011000????????????????????????")
   def FOMADD		= BitPat("b01011001????????????????????????")
   def SWYM		= BitPat("b00010000000000??????????????????")
   def UNIMP		= BitPat("b00010000111111??????????????????")
   def BRN		= BitPat("b00101000????????????????????????")
   def BRNN		= BitPat("b00101001????????????????????????")
   def BRZ		= BitPat("b00101010????????????????????????")
   def BRNZ		= BitPat("b00101011????????????????????????")
   def BRP		= BitPat("b00101100????????????????????????")
   def BRNP		= BitPat("b00101101????????????????????????")
   def RET		= BitPat("b01101110????????????????????????")
   def BREQ		= BitPat("b00101110????????????????????????")
   def BRNE		= BitPat("b00101111????????????????????????")
   def JUMPr		= BitPat("b01100101????????????????????????")
   def JUMPi		= BitPat("b01100100????????????????????????")
   def CALLr		= BitPat("b01101101????????????????????????")
   def CALLi		= BitPat("b01101100????????????????????????")
   def CPCO		= BitPat("b01110001????????????????????????")
   def TRAP		= BitPat("b01110110????????????????????????")
   def ESCAPE		= BitPat("b01110111????????????????????????")
   def CPLD		= BitPat("b01110100????????????????????????")
   def CPST		= BitPat("b01110101????????????????????????")
   def CPRD		= BitPat("b01110010????????????????????????")
   def CPWR		= BitPat("b01110011????????????????????????")
   def FENCE		= BitPat("b01110000000000??????????????????")
   def LRO_NN		= BitPat("b01110000100000??????????????????")
   def LRO_NR		= BitPat("b01110000100001??????????????????")
   def LRO_AN		= BitPat("b01110000100010??????????????????")
   def LRO_AR		= BitPat("b01110000100011??????????????????")
   def SCO_NN		= BitPat("b01110000101000??????????????????")
   def SCO_NR		= BitPat("b01110000101001??????????????????")
   def SCO_AN		= BitPat("b01110000101010??????????????????")
   def SCO_AR		= BitPat("b01110000101011??????????????????")

  /* TODO: Add your custom instruction encoding here */
}
object Causes {
  val misaligned_fetch = 0x0
  val fetch_access = 0x1
  val illegal_instruction = 0x2
  val breakpoint = 0x3
  val misaligned_load = 0x4
  val load_access = 0x5
  val misaligned_store = 0x6
  val store_access = 0x7
  val user_ecall = 0x8
  val machine_ecall = 0xb
  val all = {
    val res = collection.mutable.ArrayBuffer[Int]()
    res += misaligned_fetch
    res += fetch_access
    res += illegal_instruction
    res += breakpoint
    res += misaligned_load
    res += load_access
    res += misaligned_store
    res += store_access
    res += user_ecall
    res += machine_ecall
    res.toArray
  }
}
object CSRs {
  val cycle = 0xc00
  val instret = 0xc02
  val hpmcounter3 = 0xc03
  val hpmcounter4 = 0xc04
  val hpmcounter5 = 0xc05
  val hpmcounter6 = 0xc06
  val hpmcounter7 = 0xc07
  val hpmcounter8 = 0xc08
  val hpmcounter9 = 0xc09
  val hpmcounter10 = 0xc0a
  val hpmcounter11 = 0xc0b
  val hpmcounter12 = 0xc0c
  val hpmcounter13 = 0xc0d
  val hpmcounter14 = 0xc0e
  val hpmcounter15 = 0xc0f
  val hpmcounter16 = 0xc10
  val hpmcounter17 = 0xc11
  val hpmcounter18 = 0xc12
  val hpmcounter19 = 0xc13
  val hpmcounter20 = 0xc14
  val hpmcounter21 = 0xc15
  val hpmcounter22 = 0xc16
  val hpmcounter23 = 0xc17
  val hpmcounter24 = 0xc18
  val hpmcounter25 = 0xc19
  val hpmcounter26 = 0xc1a
  val hpmcounter27 = 0xc1b
  val hpmcounter28 = 0xc1c
  val hpmcounter29 = 0xc1d
  val hpmcounter30 = 0xc1e
  val hpmcounter31 = 0xc1f
  val mstatus = 0x300
  val misa = 0x301
  val medeleg = 0x302
  val mideleg = 0x303
  val mie = 0x304
  val mtvec = 0x305
  val mscratch = 0x340
  val mcounteren = 0x306
  val mepc = 0x341
  val mcause = 0x342
  val mtval = 0x343
  val mip = 0x344
  val tselect = 0x7a0
  val tdata1 = 0x7a1
  val tdata2 = 0x7a2
  val tdata3 = 0x7a3
  val dcsr = 0x7b0
  val dpc = 0x7b1
  val dscratch = 0x7b2
  val mcycle = 0xb00
  val minstret = 0xb02
  val mhpmcounter3 = 0xb03
  val mhpmcounter4 = 0xb04
  val mhpmcounter5 = 0xb05
  val mhpmcounter6 = 0xb06
  val mhpmcounter7 = 0xb07
  val mhpmcounter8 = 0xb08
  val mhpmcounter9 = 0xb09
  val mhpmcounter10 = 0xb0a
  val mhpmcounter11 = 0xb0b
  val mhpmcounter12 = 0xb0c
  val mhpmcounter13 = 0xb0d
  val mhpmcounter14 = 0xb0e
  val mhpmcounter15 = 0xb0f
  val mhpmcounter16 = 0xb10
  val mhpmcounter17 = 0xb11
  val mhpmcounter18 = 0xb12
  val mhpmcounter19 = 0xb13
  val mhpmcounter20 = 0xb14
  val mhpmcounter21 = 0xb15
  val mhpmcounter22 = 0xb16
  val mhpmcounter23 = 0xb17
  val mhpmcounter24 = 0xb18
  val mhpmcounter25 = 0xb19
  val mhpmcounter26 = 0xb1a
  val mhpmcounter27 = 0xb1b
  val mhpmcounter28 = 0xb1c
  val mhpmcounter29 = 0xb1d
  val mhpmcounter30 = 0xb1e
  val mhpmcounter31 = 0xb1f
  val mucounteren = 0x320
  val mhpmevent3 = 0x323
  val mhpmevent4 = 0x324
  val mhpmevent5 = 0x325
  val mhpmevent6 = 0x326
  val mhpmevent7 = 0x327
  val mhpmevent8 = 0x328
  val mhpmevent9 = 0x329
  val mhpmevent10 = 0x32a
  val mhpmevent11 = 0x32b
  val mhpmevent12 = 0x32c
  val mhpmevent13 = 0x32d
  val mhpmevent14 = 0x32e
  val mhpmevent15 = 0x32f
  val mhpmevent16 = 0x330
  val mhpmevent17 = 0x331
  val mhpmevent18 = 0x332
  val mhpmevent19 = 0x333
  val mhpmevent20 = 0x334
  val mhpmevent21 = 0x335
  val mhpmevent22 = 0x336
  val mhpmevent23 = 0x337
  val mhpmevent24 = 0x338
  val mhpmevent25 = 0x339
  val mhpmevent26 = 0x33a
  val mhpmevent27 = 0x33b
  val mhpmevent28 = 0x33c
  val mhpmevent29 = 0x33d
  val mhpmevent30 = 0x33e
  val mhpmevent31 = 0x33f
  val mvendorid = 0xf11
  val marchid = 0xf12
  val mimpid = 0xf13
  val mhartid = 0xf14
  val cycleh = 0xc80
  val instreth = 0xc82
  val hpmcounter3h = 0xc83
  val hpmcounter4h = 0xc84
  val hpmcounter5h = 0xc85
  val hpmcounter6h = 0xc86
  val hpmcounter7h = 0xc87
  val hpmcounter8h = 0xc88
  val hpmcounter9h = 0xc89
  val hpmcounter10h = 0xc8a
  val hpmcounter11h = 0xc8b
  val hpmcounter12h = 0xc8c
  val hpmcounter13h = 0xc8d
  val hpmcounter14h = 0xc8e
  val hpmcounter15h = 0xc8f
  val hpmcounter16h = 0xc90
  val hpmcounter17h = 0xc91
  val hpmcounter18h = 0xc92
  val hpmcounter19h = 0xc93
  val hpmcounter20h = 0xc94
  val hpmcounter21h = 0xc95
  val hpmcounter22h = 0xc96
  val hpmcounter23h = 0xc97
  val hpmcounter24h = 0xc98
  val hpmcounter25h = 0xc99
  val hpmcounter26h = 0xc9a
  val hpmcounter27h = 0xc9b
  val hpmcounter28h = 0xc9c
  val hpmcounter29h = 0xc9d
  val hpmcounter30h = 0xc9e
  val hpmcounter31h = 0xc9f
  val mcycleh = 0xb80
  val minstreth = 0xb82
  val mhpmcounter3h = 0xb83
  val mhpmcounter4h = 0xb84
  val mhpmcounter5h = 0xb85
  val mhpmcounter6h = 0xb86
  val mhpmcounter7h = 0xb87
  val mhpmcounter8h = 0xb88
  val mhpmcounter9h = 0xb89
  val mhpmcounter10h = 0xb8a
  val mhpmcounter11h = 0xb8b
  val mhpmcounter12h = 0xb8c
  val mhpmcounter13h = 0xb8d
  val mhpmcounter14h = 0xb8e
  val mhpmcounter15h = 0xb8f
  val mhpmcounter16h = 0xb90
  val mhpmcounter17h = 0xb91
  val mhpmcounter18h = 0xb92
  val mhpmcounter19h = 0xb93
  val mhpmcounter20h = 0xb94
  val mhpmcounter21h = 0xb95
  val mhpmcounter22h = 0xb96
  val mhpmcounter23h = 0xb97
  val mhpmcounter24h = 0xb98
  val mhpmcounter25h = 0xb99
  val mhpmcounter26h = 0xb9a
  val mhpmcounter27h = 0xb9b
  val mhpmcounter28h = 0xb9c
  val mhpmcounter29h = 0xb9d
  val mhpmcounter30h = 0xb9e
  val mhpmcounter31h = 0xb9f
  val all = {
    val res = collection.mutable.ArrayBuffer[Int]()
    res += cycle
    res += instret
    res += hpmcounter3
    res += hpmcounter4
    res += hpmcounter5
    res += hpmcounter6
    res += hpmcounter7
    res += hpmcounter8
    res += hpmcounter9
    res += hpmcounter10
    res += hpmcounter11
    res += hpmcounter12
    res += hpmcounter13
    res += hpmcounter14
    res += hpmcounter15
    res += hpmcounter16
    res += hpmcounter17
    res += hpmcounter18
    res += hpmcounter19
    res += hpmcounter20
    res += hpmcounter21
    res += hpmcounter22
    res += hpmcounter23
    res += hpmcounter24
    res += hpmcounter25
    res += hpmcounter26
    res += hpmcounter27
    res += hpmcounter28
    res += hpmcounter29
    res += hpmcounter30
    res += hpmcounter31
    res += mstatus
    res += misa
    res += medeleg
    res += mideleg
    res += mie
    res += mtvec
    res += mscratch
    res += mepc
    res += mcause
    res += mtval
    res += mip
    res += tselect
    res += tdata1
    res += tdata2
    res += tdata3
    res += dcsr
    res += dpc
    res += dscratch
    res += mcycle
    res += minstret
    res += mhpmcounter3
    res += mhpmcounter4
    res += mhpmcounter5
    res += mhpmcounter6
    res += mhpmcounter7
    res += mhpmcounter8
    res += mhpmcounter9
    res += mhpmcounter10
    res += mhpmcounter11
    res += mhpmcounter12
    res += mhpmcounter13
    res += mhpmcounter14
    res += mhpmcounter15
    res += mhpmcounter16
    res += mhpmcounter17
    res += mhpmcounter18
    res += mhpmcounter19
    res += mhpmcounter20
    res += mhpmcounter21
    res += mhpmcounter22
    res += mhpmcounter23
    res += mhpmcounter24
    res += mhpmcounter25
    res += mhpmcounter26
    res += mhpmcounter27
    res += mhpmcounter28
    res += mhpmcounter29
    res += mhpmcounter30
    res += mhpmcounter31
    res += mucounteren
    res += mhpmevent3
    res += mhpmevent4
    res += mhpmevent5
    res += mhpmevent6
    res += mhpmevent7
    res += mhpmevent8
    res += mhpmevent9
    res += mhpmevent10
    res += mhpmevent11
    res += mhpmevent12
    res += mhpmevent13
    res += mhpmevent14
    res += mhpmevent15
    res += mhpmevent16
    res += mhpmevent17
    res += mhpmevent18
    res += mhpmevent19
    res += mhpmevent20
    res += mhpmevent21
    res += mhpmevent22
    res += mhpmevent23
    res += mhpmevent24
    res += mhpmevent25
    res += mhpmevent26
    res += mhpmevent27
    res += mhpmevent28
    res += mhpmevent29
    res += mhpmevent30
    res += mhpmevent31
    res += mvendorid
    res += marchid
    res += mimpid
    res += mhartid
    res.toArray
  }
  val all32 = {
    val res = collection.mutable.ArrayBuffer(all:_*)
    res += cycleh
    res += instreth
    res += hpmcounter3h
    res += hpmcounter4h
    res += hpmcounter5h
    res += hpmcounter6h
    res += hpmcounter7h
    res += hpmcounter8h
    res += hpmcounter9h
    res += hpmcounter10h
    res += hpmcounter11h
    res += hpmcounter12h
    res += hpmcounter13h
    res += hpmcounter14h
    res += hpmcounter15h
    res += hpmcounter16h
    res += hpmcounter17h
    res += hpmcounter18h
    res += hpmcounter19h
    res += hpmcounter20h
    res += hpmcounter21h
    res += hpmcounter22h
    res += hpmcounter23h
    res += hpmcounter24h
    res += hpmcounter25h
    res += hpmcounter26h
    res += hpmcounter27h
    res += hpmcounter28h
    res += hpmcounter29h
    res += hpmcounter30h
    res += hpmcounter31h
    res += mcycleh
    res += minstreth
    res += mhpmcounter3h
    res += mhpmcounter4h
    res += mhpmcounter5h
    res += mhpmcounter6h
    res += mhpmcounter7h
    res += mhpmcounter8h
    res += mhpmcounter9h
    res += mhpmcounter10h
    res += mhpmcounter11h
    res += mhpmcounter12h
    res += mhpmcounter13h
    res += mhpmcounter14h
    res += mhpmcounter15h
    res += mhpmcounter16h
    res += mhpmcounter17h
    res += mhpmcounter18h
    res += mhpmcounter19h
    res += mhpmcounter20h
    res += mhpmcounter21h
    res += mhpmcounter22h
    res += mhpmcounter23h
    res += mhpmcounter24h
    res += mhpmcounter25h
    res += mhpmcounter26h
    res += mhpmcounter27h
    res += mhpmcounter28h
    res += mhpmcounter29h
    res += mhpmcounter30h
    res += mhpmcounter31h
    res.toArray
  }
}

