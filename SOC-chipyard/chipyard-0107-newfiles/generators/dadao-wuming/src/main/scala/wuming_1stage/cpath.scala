//**************************************************************************
// RISCV Processor Control Path
//--------------------------------------------------------------------------
//
// Christopher Celio

package wuming.stage1

import chisel3._
import chisel3.util._

import freechips.rocketchip.rocket.{CSR, Causes}

import wuming.common._
import wuming.common.Instructions._
import wuming.stage1.Constants._

class CtlToDatIo extends Bundle()
{
   val stall     = Output(Bool())
   val dmiss     = Output(Bool())
   val pc_sel    = Output(UInt(PC_4.getWidth.W))
   val op1_sel   = Output(UInt(OP1_X.getWidth.W))
   val op2_sel   = Output(UInt(OP2_X.getWidth.W))
   val alu_fun   = Output(UInt(ALU_X.getWidth.W))
   val cnd_fun   = Output(UInt(COND_X.getWidth.W))
   val wb_sel    = Output(UInt(WB_X.getWidth.W))
   val reg_grp   = Output(UInt(REG_X.getWidth.W))
   val rf_wen    = Output(Bool())
   val csr_cmd   = Output(UInt(CSR.SZ.W))
   val exception = Output(Bool())
   val exception_cause = Output(UInt(32.W))
   val pc_sel_no_xept = Output(UInt(PC_4.getWidth.W))    // Use only for instuction misalignment detection
}

class CpathIo(implicit val conf: WumingCoreParams) extends Bundle()
{
   val dcpath = Flipped(new DebugCPath())
   val imem = new MemPortIo(conf.instlen)
   val dmem = new MemPortIo(conf.xprlen)
   val dat  = Flipped(new DatToCtlIo())
   val ctl  = new CtlToDatIo()
}


class CtlPath(implicit val conf: WumingCoreParams) extends Module
{
  val io = IO(new CpathIo())
  io := DontCare

   val csignals =
      ListLookup(io.dat.inst,
                             List(N, CF_X     , COND_X  , REG_X    , OP1_X    , OP2_X     , ALU_X     , WB_X    , REN_0, MEN_0, M_X  , MT_X  , CSR.N),
               Array(      /* val  |  Control |  cond   |  reg     |  op1     |   op2     |  ALU      |  wb     | rf   | mem  | mem  | mask  |  csr  */
                           /* inst |    flow  |   fcn   |   set    |   sel    |    sel    |   fcn     |  sel    | wen  |  en  |  wr  | type  |  cmd  */
                  SWYM    -> List(Y, CF_X     , COND_X  , REG_X    , OP1_X    , OP2_X     , ALU_X     , WB_X    , REN_0, MEN_0, M_X  , MT_X  , CSR.N),
                  CPCO    -> List(Y, CF_X     , COND_X  , REG_X    , OP1_X    , OP2_X     , ALU_X     , WB_X    , REN_0, MEN_0, M_X  , MT_X  , CSR.N),

                  CPRD    -> List(Y, CF_X     , COND_X  , REG_CSR  , OP1_X    , OP2_X     , ALU_X     , WB_CSR  , REN_1, MEN_0, M_X  , MT_X   , CSR.R),
                  CPWR    -> List(Y, CF_X     , COND_X  , REG_CSR  , OP1_X    , OP2_RDHD  , ALU_COPY2 , WB_X    , REN_0, MEN_0, M_X  , MT_X   , CSR.W),

                  RD2RD   -> List(Y, CF_X     , COND_X  , REG_MRD  , OP1_X    , OP2_RDHC  , ALU_COPY2 , WB_RDHB , REN_1, MEN_0, M_X  , MT_X   , CSR.N),
                  RD2RB   -> List(Y, CF_X     , COND_X  , REG_MRB  , OP1_X    , OP2_RDHC  , ALU_COPY2 , WB_RBHB , REN_1, MEN_0, M_X  , MT_X   , CSR.N),
                  RB2RD   -> List(Y, CF_X     , COND_X  , REG_MRD  , OP1_X    , OP2_RBHC  , ALU_COPY2 , WB_RDHB , REN_1, MEN_0, M_X  , MT_X   , CSR.N),
                  RB2RB   -> List(Y, CF_X     , COND_X  , REG_MRB  , OP1_X    , OP2_RBHC  , ALU_COPY2 , WB_RBHB , REN_1, MEN_0, M_X  , MT_X   , CSR.N),
                  RD2RF   -> List(Y, CF_X     , COND_X  , REG_MRF  , OP1_X    , OP2_RDHC  , ALU_COPY2 , WB_RFHB , REN_1, MEN_0, M_X  , MT_X   , CSR.N),
                  RF2RD   -> List(Y, CF_X     , COND_X  , REG_MRD  , OP1_X    , OP2_RFHC  , ALU_COPY2 , WB_RDHB , REN_1, MEN_0, M_X  , MT_X   , CSR.N),
                  RF2RF   -> List(Y, CF_X     , COND_X  , REG_MRF  , OP1_X    , OP2_RFHC  , ALU_COPY2 , WB_RFHB , REN_1, MEN_0, M_X  , MT_X   , CSR.N),

                  AND     -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RDHC , OP2_RDHD  , ALU_AND   , WB_RDHB , REN_1, MEN_0, M_X  , MT_X   , CSR.N),
                  ORR     -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RDHC , OP2_RDHD  , ALU_OR    , WB_RDHB , REN_1, MEN_0, M_X  , MT_X   , CSR.N),
                  XOR     -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RDHC , OP2_RDHD  , ALU_XOR   , WB_RDHB , REN_1, MEN_0, M_X  , MT_X   , CSR.N),
                  XNOR    -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RDHC , OP2_RDHD  , ALU_XNOR  , WB_RDHB , REN_1, MEN_0, M_X  , MT_X   , CSR.N),

                  ANDI    -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RDHB , OP2_IMMU12, ALU_AND   , WB_RDHA , REN_1, MEN_0, M_X  , MT_X   , CSR.N),

                  SHLUr   -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RDHC , OP2_RDHD  , ALU_SLL   , WB_RDHB , REN_1, MEN_0, M_X  , MT_X   , CSR.N),
                  SHLUi   -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RDHC , OP2_IMMU6 , ALU_SLL   , WB_RDHB , REN_1, MEN_0, M_X  , MT_X   , CSR.N),
                  SHRSr   -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RDHC , OP2_RDHD  , ALU_SRA   , WB_RDHB , REN_1, MEN_0, M_X  , MT_X   , CSR.N),
                  SHRSi   -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RDHC , OP2_IMMU6 , ALU_SRA   , WB_RDHB , REN_1, MEN_0, M_X  , MT_X   , CSR.N),
                  SHRUr   -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RDHC , OP2_RDHD  , ALU_SRL   , WB_RDHB , REN_1, MEN_0, M_X  , MT_X   , CSR.N),
                  SHRUi   -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RDHC , OP2_IMMU6 , ALU_SRL   , WB_RDHB , REN_1, MEN_0, M_X  , MT_X   , CSR.N),
                  EXTSr   -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RDHC , OP2_RDHD  , ALU_EXTS  , WB_RDHB , REN_1, MEN_0, M_X  , MT_X   , CSR.N),
                  EXTSi   -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RDHC , OP2_IMMU6 , ALU_EXTS  , WB_RDHB , REN_1, MEN_0, M_X  , MT_X   , CSR.N),
                  EXTZr   -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RDHC , OP2_RDHD  , ALU_EXTZ  , WB_RDHB , REN_1, MEN_0, M_X  , MT_X   , CSR.N),
                  EXTZi   -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RDHC , OP2_IMMU6 , ALU_EXTZ  , WB_RDHB , REN_1, MEN_0, M_X  , MT_X   , CSR.N),

                  ADDrb   -> List(Y, CF_X     , COND_X  , REG_RB   , OP1_RBHC , OP2_RDHD  , ALU_ADD   , WB_RBHB , REN_1, MEN_0, M_X  , MT_X   , CSR.N),
                  SUBrb   -> List(Y, CF_X     , COND_X  , REG_RB   , OP1_RBHC , OP2_RDHD  , ALU_SUB   , WB_RBHB , REN_1, MEN_0, M_X  , MT_X   , CSR.N),

                  ADDIrd  -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RDHB , OP2_IMMS12, ALU_ADD   , WB_RDHA , REN_1, MEN_0, M_X  , MT_X   , CSR.N),
                  ADDIrb  -> List(Y, CF_X     , COND_X  , REG_RB   , OP1_RBHB , OP2_IMMS12, ALU_ADD   , WB_RBHA , REN_1, MEN_0, M_X  , MT_X   , CSR.N),

                  ADDrd   -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RDHC , OP2_RDHD  , ALU_ADD   , WB_HAHB , REN_1, MEN_0, M_X  , MT_X   , CSR.N),
                  SUBrd   -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RDHC , OP2_RDHD  , ALU_SUB   , WB_HAHB , REN_1, MEN_0, M_X  , MT_X   , CSR.N),

                  MULS    -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RDHC , OP2_RDHD  , ALU_MULS  , WB_HAHB , REN_1, MEN_0, M_X  , MT_X   , CSR.N),
                  MULU    -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RDHC , OP2_RDHD  , ALU_MULU  , WB_HAHB , REN_1, MEN_0, M_X  , MT_X   , CSR.N),
                  DIVS    -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RDHC , OP2_RDHD  , ALU_DIVS  , WB_HAHB , REN_1, MEN_0, M_X  , MT_X   , CSR.N),
                  DIVU    -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RDHC , OP2_RDHD  , ALU_DIVU  , WB_HAHB , REN_1, MEN_0, M_X  , MT_X   , CSR.N),

                  LDBS    -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RBHB , OP2_IMMS12, ALU_ADD   , WB_RDMM , REN_1, MEN_1, M_XRD, MT_B   , CSR.N),
                  LDWS    -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RBHB , OP2_IMMS12, ALU_ADD   , WB_RDMM , REN_1, MEN_1, M_XRD, MT_W   , CSR.N),
                  LDTS    -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RBHB , OP2_IMMS12, ALU_ADD   , WB_RDMM , REN_1, MEN_1, M_XRD, MT_T   , CSR.N),
                  LDO     -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RBHB , OP2_IMMS12, ALU_ADD   , WB_RDMM , REN_1, MEN_1, M_XRD, MT_O   , CSR.N),
                  LDBU    -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RBHB , OP2_IMMS12, ALU_ADD   , WB_RDMM , REN_1, MEN_1, M_XRD, MT_BU  , CSR.N),
                  LDWU    -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RBHB , OP2_IMMS12, ALU_ADD   , WB_RDMM , REN_1, MEN_1, M_XRD, MT_WU  , CSR.N),
                  LDTU    -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RBHB , OP2_IMMS12, ALU_ADD   , WB_RDMM , REN_1, MEN_1, M_XRD, MT_TU  , CSR.N),

                  LDMBS   -> List(Y, CF_X     , COND_X  , REG_MRD  , OP1_RBHB , OP2_RDHC  , ALU_ADD   , WB_RDMM , REN_1, MEN_1, M_XRD, MT_B   , CSR.N),
                  LDMWS   -> List(Y, CF_X     , COND_X  , REG_MRD  , OP1_RBHB , OP2_RDHC  , ALU_ADD   , WB_RDMM , REN_1, MEN_1, M_XRD, MT_W   , CSR.N),
                  LDMTS   -> List(Y, CF_X     , COND_X  , REG_MRD  , OP1_RBHB , OP2_RDHC  , ALU_ADD   , WB_RDMM , REN_1, MEN_1, M_XRD, MT_T   , CSR.N),
                  LDMO    -> List(Y, CF_X     , COND_X  , REG_MRD  , OP1_RBHB , OP2_RDHC  , ALU_ADD   , WB_RDMM , REN_1, MEN_1, M_XRD, MT_O   , CSR.N),
                  LDMBU   -> List(Y, CF_X     , COND_X  , REG_MRD  , OP1_RBHB , OP2_RDHC  , ALU_ADD   , WB_RDMM , REN_1, MEN_1, M_XRD, MT_BU  , CSR.N),
                  LDMWU   -> List(Y, CF_X     , COND_X  , REG_MRD  , OP1_RBHB , OP2_RDHC  , ALU_ADD   , WB_RDMM , REN_1, MEN_1, M_XRD, MT_WU  , CSR.N),
                  LDMTU   -> List(Y, CF_X     , COND_X  , REG_MRD  , OP1_RBHB , OP2_RDHC  , ALU_ADD   , WB_RDMM , REN_1, MEN_1, M_XRD, MT_TU  , CSR.N),

                  STB     -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RBHB , OP2_IMMS12, ALU_ADD   , WB_X    , REN_0, MEN_1, M_XWR, MT_B   , CSR.N),
                  STW     -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RBHB , OP2_IMMS12, ALU_ADD   , WB_X    , REN_0, MEN_1, M_XWR, MT_W   , CSR.N),
                  STT     -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RBHB , OP2_IMMS12, ALU_ADD   , WB_X    , REN_0, MEN_1, M_XWR, MT_T   , CSR.N),
                  STO     -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RBHB , OP2_IMMS12, ALU_ADD   , WB_X    , REN_0, MEN_1, M_XWR, MT_O   , CSR.N),

                  STMB    -> List(Y, CF_X     , COND_X  , REG_MRD  , OP1_RBHB , OP2_RDHC  , ALU_ADD   , WB_X    , REN_0, MEN_1, M_XWR, MT_B   , CSR.N),
                  STMW    -> List(Y, CF_X     , COND_X  , REG_MRD  , OP1_RBHB , OP2_RDHC  , ALU_ADD   , WB_X    , REN_0, MEN_1, M_XWR, MT_W   , CSR.N),
                  STMT    -> List(Y, CF_X     , COND_X  , REG_MRD  , OP1_RBHB , OP2_RDHC  , ALU_ADD   , WB_X    , REN_0, MEN_1, M_XWR, MT_T   , CSR.N),
                  STMO    -> List(Y, CF_X     , COND_X  , REG_MRD  , OP1_RBHB , OP2_RDHC  , ALU_ADD   , WB_X    , REN_0, MEN_1, M_XWR, MT_O   , CSR.N),

                  LDRB    -> List(Y, CF_X     , COND_X  , REG_RB   , OP1_RBHB , OP2_IMMS12, ALU_ADD   , WB_RBMM , REN_1, MEN_1, M_XRD, MT_O   , CSR.N),
                  STRB    -> List(Y, CF_X     , COND_X  , REG_RB   , OP1_RBHB , OP2_IMMS12, ALU_ADD   , WB_X    , REN_0, MEN_1, M_XWR, MT_O   , CSR.N),

                  LDMRB   -> List(Y, CF_X     , COND_X  , REG_MRB  , OP1_RBHB , OP2_RDHC  , ALU_ADD   , WB_RBMM , REN_1, MEN_1, M_XRD, MT_O   , CSR.N),
                  STMRB   -> List(Y, CF_X     , COND_X  , REG_MRB  , OP1_RBHB , OP2_RDHC  , ALU_ADD   , WB_X    , REN_0, MEN_1, M_XWR, MT_O   , CSR.N),

                  CMPSi   -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RDHB , OP2_IMMS12, ALU_CMPS  , WB_RDHA , REN_1, MEN_0, M_X  , MT_X   , CSR.N),
                  CMPSr   -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RDHC , OP2_RDHD  , ALU_CMPS  , WB_RDHB , REN_1, MEN_0, M_X  , MT_X   , CSR.N),
                  CMPUi   -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RDHB , OP2_IMMU12, ALU_CMPU  , WB_RDHA , REN_1, MEN_0, M_X  , MT_X   , CSR.N),
                  CMPUr   -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RDHC , OP2_RDHD  , ALU_CMPU  , WB_RDHB , REN_1, MEN_0, M_X  , MT_X   , CSR.N),
                  CMP     -> List(Y, CF_X     , COND_X  , REG_RB   , OP1_RBHC , OP2_RBHD  , ALU_CMPU  , WB_RDHB , REN_1, MEN_0, M_X  , MT_X   , CSR.N),

                  CSN     -> List(Y, CF_X     , COND_N  , REG_RD   , OP1_RDHC , OP2_RDHD  , ALU_CSET  , WB_RDHB , REN_1, MEN_0, M_X  , MT_X   , CSR.N),
                  CSZ     -> List(Y, CF_X     , COND_Z  , REG_RD   , OP1_RDHC , OP2_RDHD  , ALU_CSET  , WB_RDHB , REN_1, MEN_0, M_X  , MT_X   , CSR.N),
                  CSP     -> List(Y, CF_X     , COND_P  , REG_RD   , OP1_RDHC , OP2_RDHD  , ALU_CSET  , WB_RDHB , REN_1, MEN_0, M_X  , MT_X   , CSR.N),
                  CSEQ    -> List(Y, CF_X     , COND_NE , REG_RD   , OP1_RDHC , OP2_RDHD  , ALU_CSET  , WB_RDHC , REN_1, MEN_0, M_X  , MT_X   , CSR.N),
                  CSNE    -> List(Y, CF_X     , COND_EQ , REG_RD   , OP1_RDHC , OP2_RDHD  , ALU_CSET  , WB_RDHC , REN_1, MEN_0, M_X  , MT_X   , CSR.N),

                  BREQ    -> List(Y, CF_BR12  , COND_EQ , REG_X    , OP1_X    , OP2_X     , ALU_X     , WB_X    , REN_0, MEN_0, M_X  , MT_X   , CSR.N),
                  BRNE    -> List(Y, CF_BR12  , COND_NE , REG_X    , OP1_X    , OP2_X     , ALU_X     , WB_X    , REN_0, MEN_0, M_X  , MT_X   , CSR.N),
                  BRN     -> List(Y, CF_BR18  , COND_N  , REG_X    , OP1_X    , OP2_X     , ALU_X     , WB_X    , REN_0, MEN_0, M_X  , MT_X   , CSR.N),
                  BRNN    -> List(Y, CF_BR18  , COND_NN , REG_X    , OP1_X    , OP2_X     , ALU_X     , WB_X    , REN_0, MEN_0, M_X  , MT_X   , CSR.N),
                  BRZ     -> List(Y, CF_BR18  , COND_Z  , REG_X    , OP1_X    , OP2_X     , ALU_X     , WB_X    , REN_0, MEN_0, M_X  , MT_X   , CSR.N),
                  BRNZ    -> List(Y, CF_BR18  , COND_NZ , REG_X    , OP1_X    , OP2_X     , ALU_X     , WB_X    , REN_0, MEN_0, M_X  , MT_X   , CSR.N),
                  BRP     -> List(Y, CF_BR18  , COND_P  , REG_X    , OP1_X    , OP2_X     , ALU_X     , WB_X    , REN_0, MEN_0, M_X  , MT_X   , CSR.N),
                  BRNP    -> List(Y, CF_BR18  , COND_NP , REG_X    , OP1_X    , OP2_X     , ALU_X     , WB_X    , REN_0, MEN_0, M_X  , MT_X   , CSR.N),

                  JUMPi   -> List(Y, CF_JUMPI , COND_X  , REG_X    , OP1_X    , OP2_X     , ALU_X     , WB_X    , REN_0, MEN_0, M_X  , MT_X   , CSR.N),
                  JUMPr   -> List(Y, CF_JUMPR , COND_X  , REG_X    , OP1_X    , OP2_X     , ALU_X     , WB_X    , REN_0, MEN_0, M_X  , MT_X   , CSR.N),
                  CALLi   -> List(Y, CF_CALLI , COND_X  , RAS_PUSH , OP1_X    , OP2_X     , ALU_X     , WB_RA   , REN_1, MEN_0, M_X  , MT_X   , CSR.N),
                  CALLr   -> List(Y, CF_CALLR , COND_X  , RAS_PUSH , OP1_X    , OP2_X     , ALU_X     , WB_RA   , REN_1, MEN_0, M_X  , MT_X   , CSR.N),
                  RET     -> List(Y, CF_RET   , COND_X  , RAS_POP  , OP1_X    , OP2_IMMS18, ALU_COPY2 , WB_RDHA , REN_1, MEN_0, M_X  , MT_X   , CSR.N),

                  SETZWrd -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_X    , OP2_WYDE  , ALU_COPY2 , WB_RDHA , REN_1, MEN_0, M_X , MT_X   , CSR.N),
                  SETOW   -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_X    , OP2_WYDE  , ALU_SETOW , WB_RDHA , REN_1, MEN_0, M_X , MT_X   , CSR.N),
                  ORWrd   -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RDHA , OP2_WYDE  , ALU_OR    , WB_RDHA , REN_1, MEN_0, M_X , MT_X   , CSR.N),
                  ANDNWrd -> List(Y, CF_X     , COND_X  , REG_RD   , OP1_RDHA , OP2_WYDE  , ALU_ANDNW , WB_RDHA , REN_1, MEN_0, M_X , MT_X   , CSR.N),
                  SETZWrb -> List(Y, CF_X     , COND_X  , REG_RB   , OP1_X    , OP2_WYDE  , ALU_COPY2 , WB_RBHA , REN_1, MEN_0, M_X , MT_X   , CSR.N),
                  ORWrb   -> List(Y, CF_X     , COND_X  , REG_RB   , OP1_RBHA , OP2_WYDE  , ALU_OR    , WB_RBHA , REN_1, MEN_0, M_X , MT_X   , CSR.N),
                  ANDNWrb -> List(Y, CF_X     , COND_X  , REG_RB   , OP1_RBHA , OP2_WYDE  , ALU_ANDNW , WB_RBHA , REN_1, MEN_0, M_X , MT_X   , CSR.N),
                  SETW    -> List(Y, CF_X     , COND_X  , REG_RF   , OP1_RFHA , OP2_WYDE  , ALU_SETW  , WB_RFHA , REN_1, MEN_0, M_X , MT_X   , CSR.N),

                  ADRP    -> List(Y, CF_X     , COND_X  , REG_X    , OP1_PC   , OP2_IMMS18, ALU_ADRP  , WB_RBHA , REN_1, MEN_0, M_X , MT_X   , CSR.N),

                  TRAP    -> List(Y, CF_X     , COND_X  , REG_X    , OP1_X    , OP2_X     , ALU_X     , WB_X    , REN_0, MEN_0, M_X , MT_X   , CSR.I),
                  ESCAPE  -> List(Y, CF_X     , COND_X  , REG_X    , OP1_X    , OP2_X     , ALU_X     , WB_X    , REN_0, MEN_0, M_X , MT_X   , CSR.I),
                  ))

   // Put these control signals into variables
   val (cs_val_inst: Bool) :: cs_ctrl_flow       :: cs_cond_fun           :: cs_reg_group          :: cs_op1_sel :: cs_op2_sel :: cs0 = csignals
   val cs_alu_fun          :: cs_wb_sel          :: (cs_rf_wen: Bool)     ::               cs1 = cs0
   val (cs_mem_en: Bool)   :: cs_mem_fcn         :: cs_msk_sel            :: cs_csr_cmd :: Nil = cs1

   // Branch Logic
   val br_taken =             Mux(cs_cond_fun === COND_N , Mux( io.dat.cond_n , Y, N),
                              Mux(cs_cond_fun === COND_NN, Mux(!io.dat.cond_n , Y, N),
                              Mux(cs_cond_fun === COND_Z , Mux( io.dat.cond_z , Y, N),
                              Mux(cs_cond_fun === COND_NZ, Mux(!io.dat.cond_z , Y, N),
                              Mux(cs_cond_fun === COND_P , Mux( io.dat.cond_p , Y, N),
                              Mux(cs_cond_fun === COND_NP, Mux(!io.dat.cond_p , Y, N),
                              Mux(cs_cond_fun === COND_EQ, Mux( io.dat.cond_eq, Y, N),
                              Mux(cs_cond_fun === COND_NE, Mux(!io.dat.cond_eq, Y, N), N))))))))

   val ctrl_pc_sel_no_xept =  Mux(io.dat.csr_interrupt     , PC_EXCP,
                              Mux(cs_ctrl_flow === CF_X    , PC_4,
                              Mux(cs_ctrl_flow === CF_BR18 , Mux( br_taken, PC_BR18, PC_4),
                              Mux(cs_ctrl_flow === CF_BR12 , Mux( br_taken, PC_BR12, PC_4),
                              Mux(cs_ctrl_flow === CF_JUMPI, PC_IIII,
                              Mux(cs_ctrl_flow === CF_JUMPR, PC_RRII,
                              Mux(cs_ctrl_flow === CF_CALLI, PC_IIII,
                              Mux(cs_ctrl_flow === CF_CALLR, PC_RRII,
                              Mux(cs_ctrl_flow === CF_RET  , PC_RASP,
                                                             PC_4)))))))))
   val ctrl_pc_sel = Mux(io.ctl.exception || io.dat.csr_eret, PC_EXCP, ctrl_pc_sel_no_xept)

   // mem_en suppression: no new memory request shall be issued after the memory operation of the current instruction is done.
   // Once we get a new instruction, we reset this flag.
   val reg_mem_en = RegInit(false.B)
   when (io.dmem.resp.valid) {
      reg_mem_en := false.B
   } .elsewhen (io.imem.resp.valid) {
      reg_mem_en := cs_mem_en
   }
   val mem_en = Mux(io.imem.resp.valid, cs_mem_en, reg_mem_en)

   val data_misaligned = Wire(Bool())
   io.ctl.dmiss := !((mem_en && (io.dmem.resp.valid || data_misaligned)) || !mem_en)
   val stall =  io.dat.imiss || io.ctl.dmiss


   // Set the data-path control signals
   io.ctl.stall    := stall
   io.ctl.pc_sel   := ctrl_pc_sel
   io.ctl.op1_sel  := cs_op1_sel
   io.ctl.op2_sel  := cs_op2_sel
   io.ctl.alu_fun  := cs_alu_fun
   io.ctl.cnd_fun  := cs_cond_fun
   io.ctl.wb_sel   := cs_wb_sel
   io.ctl.reg_grp  := cs_reg_group
   io.ctl.rf_wen   := Mux(stall || io.ctl.exception, false.B, cs_rf_wen)

   // convert CSR instructions to read-only CSR commands
   io.ctl.csr_cmd  := Mux(stall, CSR.N, cs_csr_cmd)

   // Memory Requests
   io.dmem.req.valid    := mem_en && !io.ctl.exception
   io.dmem.req.bits.fcn := cs_mem_fcn
   io.dmem.req.bits.typ := cs_msk_sel

   // Exception Handling ---------------------
   io.ctl.pc_sel_no_xept := ctrl_pc_sel_no_xept
   val illegal = ((!cs_val_inst && io.imem.resp.valid) || io.dat.inst_multi_reg || io.dat.inst_rasp_excp)

   // Data misalignment detection
   // For example, if type is 3 (word), the mask is ~(0b111 << (3 - 1)) = ~0b100 = 0b011.
   val misaligned_mask = Wire(UInt(3.W))
   misaligned_mask := ~(7.U(3.W) << (cs_msk_sel - 1.U)(1, 0))
   data_misaligned := (misaligned_mask & io.dat.mem_address_low).orR && mem_en
   val mem_store = cs_mem_fcn === M_XWR

   // Set exception flag and cause
   // Exception priority matters!
   io.ctl.exception := illegal || io.dat.inst_misaligned || data_misaligned
   io.ctl.exception_cause :=  Mux(illegal,                Causes.illegal_instruction.U,
                              Mux(io.dat.inst_misaligned, Causes.misaligned_fetch.U,
                              Mux(mem_store,              Causes.misaligned_store.U,
                                                          Causes.misaligned_load.U
                              )))

}
