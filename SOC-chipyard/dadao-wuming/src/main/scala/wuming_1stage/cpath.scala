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
                             List(N, BR_X  , REG_X  , OP1_X  ,  OP2_X  , ALU_X   , WB_X   , REN_0, MEN_0, M_X  , MT_X,  CSR.N),
               Array(       /* val  |  BR  |  reg   |  op1   |   op2     |  ALU    |  wb  | rf   | mem  | mem  | mask |  csr  */
                            /* inst | type |   set  |   sel  |    sel    |   fcn   |  sel | wen  |  en  |  wr  | type |  cmd  */
                  SWYM    -> List(Y, BR_X  , REG_X  , OP1_X  , OP2_X  ,  ALU_X    , WB_X  , REN_0, MEN_0, M_X  , MT_X,  CSR.N),

                  CPRD    -> List(Y, BR_X  , REG_RD ,  OP1_RS1, OP2_X   , ALU_COPY1, WB_CSR, REN_1, MEN_0, M_X ,  MT_X,  CSR.R),
                  CPWR    -> List(Y, BR_X  , REG_RD ,  OP1_RS1, OP2_X   , ALU_COPY1, WB_CSR, REN_1, MEN_0, M_X ,  MT_X,  CSR.W),

                  AND     -> List(Y, BR_X  , REG_RD ,  OP1_RDHC, OP2_RDHD, ALU_AND ,  WB_RDHB, REN_1, MEN_0, M_X  , MT_X,  CSR.N),
                  ORR     -> List(Y, BR_X  , REG_RD ,  OP1_RDHC, OP2_RDHD, ALU_OR  ,  WB_RDHB, REN_1, MEN_0, M_X  , MT_X,  CSR.N),
                  XOR     -> List(Y, BR_X  , REG_RD ,  OP1_RDHC, OP2_RDHD, ALU_XOR ,  WB_RDHB, REN_1, MEN_0, M_X  , MT_X,  CSR.N),

                  SHLUr   -> List(Y, BR_X  , REG_RD ,  OP1_RDHC, OP2_RDHD , ALU_SLL ,  WB_RDHB, REN_1, MEN_0, M_X  , MT_X,  CSR.N),
                  SHLUi   -> List(Y, BR_X  , REG_RD ,  OP1_RDHC, OP2_IMMU6, ALU_SLL ,  WB_RDHB, REN_1, MEN_0, M_X  , MT_X,  CSR.N),
                  SHRSr   -> List(Y, BR_X  , REG_RD ,  OP1_RDHC, OP2_RDHD , ALU_SRA ,  WB_RDHB, REN_1, MEN_0, M_X  , MT_X,  CSR.N),
                  SHRSi   -> List(Y, BR_X  , REG_RD ,  OP1_RDHC, OP2_IMMU6, ALU_SRA ,  WB_RDHB, REN_1, MEN_0, M_X  , MT_X,  CSR.N),
                  SHRUr   -> List(Y, BR_X  , REG_RD ,  OP1_RDHC, OP2_RDHD , ALU_SRL ,  WB_RDHB, REN_1, MEN_0, M_X  , MT_X,  CSR.N),
                  SHRUi   -> List(Y, BR_X  , REG_RD ,  OP1_RDHC, OP2_IMMU6, ALU_SRL ,  WB_RDHB, REN_1, MEN_0, M_X  , MT_X,  CSR.N),

                  ADDrb   -> List(Y, BR_X  , REG_RB ,  OP1_RBHC, OP2_RDHD , ALU_ADD ,  WB_RBHB, REN_1, MEN_0, M_X  , MT_X,  CSR.N),
                  SUBrb   -> List(Y, BR_X  , REG_RB ,  OP1_RBHC, OP2_RDHD , ALU_SUB ,  WB_RBHB, REN_1, MEN_0, M_X  , MT_X,  CSR.N),

                  ADDIrd  -> List(Y, BR_X  , REG_RD ,  OP1_RDHA, OP2_IMMS18, ALU_ADD ,  WB_RDHA, REN_1, MEN_0, M_X  , MT_X,  CSR.N),
                  ADDIrb  -> List(Y, BR_X  , REG_RB ,  OP1_RBHA, OP2_IMMS18, ALU_ADD ,  WB_RBHA, REN_1, MEN_0, M_X  , MT_X,  CSR.N),

                  ADDrd   -> List(Y, BR_X  , REG_RD ,  OP1_RDHC, OP2_RDHD , ALU_ADD ,  WB_HAHB, REN_1, MEN_0, M_X  , MT_X,  CSR.N),
                  SUBrd   -> List(Y, BR_X  , REG_RD ,  OP1_RDHC, OP2_RDHD , ALU_SUB ,  WB_HAHB, REN_1, MEN_0, M_X  , MT_X,  CSR.N),

                  LDBS    -> List(Y, BR_X  , REG_RD ,  OP1_RBHB, OP2_IMMS12, ALU_ADD ,  WB_RDMM, REN_1, MEN_1, M_XRD, MT_B,  CSR.N),
                  LDWS    -> List(Y, BR_X  , REG_RD ,  OP1_RBHB, OP2_IMMS12, ALU_ADD ,  WB_RDMM, REN_1, MEN_1, M_XRD, MT_W,  CSR.N),
                  LDTS    -> List(Y, BR_X  , REG_RD ,  OP1_RBHB, OP2_IMMS12, ALU_ADD ,  WB_RDMM, REN_1, MEN_1, M_XRD, MT_T,  CSR.N),
                  LDO     -> List(Y, BR_X  , REG_RD ,  OP1_RBHB, OP2_IMMS12, ALU_ADD ,  WB_RDMM, REN_1, MEN_1, M_XRD, MT_O,  CSR.N),
                  LDBU    -> List(Y, BR_X  , REG_RD ,  OP1_RBHB, OP2_IMMS12, ALU_ADD ,  WB_RDMM, REN_1, MEN_1, M_XRD, MT_BU, CSR.N),
                  LDWU    -> List(Y, BR_X  , REG_RD ,  OP1_RBHB, OP2_IMMS12, ALU_ADD ,  WB_RDMM, REN_1, MEN_1, M_XRD, MT_WU, CSR.N),
                  LDTU    -> List(Y, BR_X  , REG_RD ,  OP1_RBHB, OP2_IMMS12, ALU_ADD ,  WB_RDMM, REN_1, MEN_1, M_XRD, MT_TU, CSR.N),

                  STB     -> List(Y, BR_X  , REG_RD ,  OP1_RBHB, OP2_IMMS12, ALU_ADD ,  WB_X  , REN_0, MEN_1, M_XWR, MT_B,  CSR.N),
                  STW     -> List(Y, BR_X  , REG_RD ,  OP1_RBHB, OP2_IMMS12, ALU_ADD ,  WB_X  , REN_0, MEN_1, M_XWR, MT_W,  CSR.N),
                  STT     -> List(Y, BR_X  , REG_RD ,  OP1_RBHB, OP2_IMMS12, ALU_ADD ,  WB_X  , REN_0, MEN_1, M_XWR, MT_T,  CSR.N),
                  STO     -> List(Y, BR_X  , REG_RD ,  OP1_RBHB, OP2_IMMS12, ALU_ADD ,  WB_X  , REN_0, MEN_1, M_XWR, MT_O,  CSR.N),

                  LDRB    -> List(Y, BR_X  , REG_RB ,  OP1_RBHB, OP2_IMMS12, ALU_ADD ,  WB_RBMM, REN_1, MEN_1, M_XRD, MT_O,  CSR.N),
                  STRB    -> List(Y, BR_X  , REG_RB ,  OP1_RBHB, OP2_IMMS12, ALU_ADD ,  WB_X  , REN_0, MEN_1, M_XWR, MT_O,  CSR.N),

                  CMPSi   -> List(Y, BR_X  , REG_RD ,  OP1_RDHB, OP2_IMMS12, ALU_CMPS ,  WB_RDHA, REN_1, MEN_0, M_X  , MT_X,  CSR.N),
                  CMPSr   -> List(Y, BR_X  , REG_RD ,  OP1_RDHC, OP2_RDHD  , ALU_CMPS ,  WB_RDHB, REN_1, MEN_0, M_X  , MT_X,  CSR.N),
                  CMPUi   -> List(Y, BR_X  , REG_RD ,  OP1_RDHB, OP2_IMMU12, ALU_CMPU ,  WB_RDHA, REN_1, MEN_0, M_X  , MT_X,  CSR.N),
                  CMPUr   -> List(Y, BR_X  , REG_RD ,  OP1_RDHC, OP2_RDHD  , ALU_CMPU ,  WB_RDHB, REN_1, MEN_0, M_X  , MT_X,  CSR.N),
                  CMP     -> List(Y, BR_X  , REG_RB ,  OP1_RBHC, OP2_RBHD  , ALU_CMPU ,  WB_RDHB, REN_1, MEN_0, M_X  , MT_X,  CSR.N),

                  BREQ    -> List(Y, BR_EQ , REG_X  ,  OP1_X  , OP2_X   , ALU_X   ,  WB_X  , REN_0, MEN_0, M_X  , MT_X,  CSR.N),
                  BRNE    -> List(Y, BR_NE , REG_X  ,  OP1_X  , OP2_X   , ALU_X   ,  WB_X  , REN_0, MEN_0, M_X  , MT_X,  CSR.N),
                  BRN     -> List(Y, BR_N  , REG_X  ,  OP1_X  , OP2_X   , ALU_X   ,  WB_X  , REN_0, MEN_0, M_X  , MT_X,  CSR.N),
                  BRNN    -> List(Y, BR_NN , REG_X  ,  OP1_X  , OP2_X   , ALU_X   ,  WB_X  , REN_0, MEN_0, M_X  , MT_X,  CSR.N),
                  BRZ     -> List(Y, BR_Z  , REG_X  ,  OP1_X  , OP2_X   , ALU_X   ,  WB_X  , REN_0, MEN_0, M_X  , MT_X,  CSR.N),
                  BRNZ    -> List(Y, BR_NZ , REG_X  ,  OP1_X  , OP2_X   , ALU_X   ,  WB_X  , REN_0, MEN_0, M_X  , MT_X,  CSR.N),
                  BRP     -> List(Y, BR_P  , REG_X  ,  OP1_X  , OP2_X   , ALU_X   ,  WB_X  , REN_0, MEN_0, M_X  , MT_X,  CSR.N),
                  BRNP    -> List(Y, BR_NP , REG_X  ,  OP1_X  , OP2_X   , ALU_X   ,  WB_X  , REN_0, MEN_0, M_X  , MT_X,  CSR.N),

                  JUMPi   -> List(Y, BR_JMPI, REG_X  ,  OP1_X  , OP2_X   , ALU_X   ,  WB_X , REN_0, MEN_0, M_X  , MT_X,  CSR.N),
                  JUMPr   -> List(Y, BR_JMPR, REG_X  ,  OP1_X  , OP2_X   , ALU_X   ,  WB_X , REN_0, MEN_0, M_X  , MT_X,  CSR.N),

                  AUIPC   -> List(Y, BR_X  , REG_X  ,  OP1_IMU, OP2_PC  , ALU_ADD ,  WB_ALU, REN_1, MEN_0, M_X ,  MT_X,  CSR.N),
                  LUI     -> List(Y, BR_X  , REG_X  ,  OP1_IMU, OP2_X   , ALU_COPY1, WB_ALU, REN_1, MEN_0, M_X ,  MT_X,  CSR.N),

                  //JAL     -> List(Y, BR_J  , REG_X  ,  OP1_X  , OP2_X   , ALU_X   ,  WB_PC4, REN_1, MEN_0, M_X  , MT_X,  CSR.N),
                  //JALR    -> List(Y, BR_JR , REG_X  ,  OP1_RS1, OP2_IMI , ALU_X   ,  WB_PC4, REN_1, MEN_0, M_X  , MT_X,  CSR.N),

                  ECALL   -> List(Y, BR_X  , REG_X  ,  OP1_X  , OP2_X  ,  ALU_X    , WB_X  , REN_0, MEN_0, M_X  , MT_X,  CSR.I),
                  MRET    -> List(Y, BR_X  , REG_X  ,  OP1_X  , OP2_X  ,  ALU_X    , WB_X  , REN_0, MEN_0, M_X  , MT_X,  CSR.I),
                  DRET    -> List(Y, BR_X  , REG_X  ,  OP1_X  , OP2_X  ,  ALU_X    , WB_X  , REN_0, MEN_0, M_X  , MT_X,  CSR.I),
                  EBREAK  -> List(Y, BR_X  , REG_X  ,  OP1_X  , OP2_X  ,  ALU_X    , WB_X  , REN_0, MEN_0, M_X  , MT_X,  CSR.I),
                  WFI     -> List(Y, BR_X  , REG_X  ,  OP1_X  , OP2_X  ,  ALU_X    , WB_X  , REN_0, MEN_0, M_X  , MT_X,  CSR.N), // implemented as a NOP

                  FENCE_I -> List(Y, BR_X  , REG_X  ,  OP1_X  , OP2_X  ,  ALU_X    , WB_X  , REN_0, MEN_0, M_X  , MT_X,  CSR.N),
                  FENCE   -> List(Y, BR_X  , REG_X  ,  OP1_X  , OP2_X  ,  ALU_X    , WB_X  , REN_0, MEN_0, M_X  , MT_X,  CSR.N)
                  // we are already sequentially consistent, so no need to honor the fence instruction
                  ))

   // Put these control signals into variables
   val (cs_val_inst: Bool) :: cs_br_type         :: cs_reg_group          :: cs_op1_sel :: cs_op2_sel :: cs0 = csignals
   val cs_alu_fun          :: cs_wb_sel          :: (cs_rf_wen: Bool)     ::               cs1 = cs0
   val (cs_mem_en: Bool)   :: cs_mem_fcn         :: cs_msk_sel            :: cs_csr_cmd :: Nil = cs1

   // Branch Logic
   val ctrl_pc_sel_no_xept =  Mux(io.dat.csr_interrupt ,  PC_EXC,
                              Mux(cs_br_type === BR_X  ,  PC_4,
                              Mux(cs_br_type === BR_N  ,  Mux( io.dat.br_n ,  PC_BR18, PC_4),
                              Mux(cs_br_type === BR_NN ,  Mux(!io.dat.br_n ,  PC_BR18, PC_4),
                              Mux(cs_br_type === BR_Z  ,  Mux( io.dat.br_z ,  PC_BR18, PC_4),
                              Mux(cs_br_type === BR_NZ ,  Mux(!io.dat.br_z ,  PC_BR18, PC_4),
                              Mux(cs_br_type === BR_P  ,  Mux( io.dat.br_p ,  PC_BR18, PC_4),
                              Mux(cs_br_type === BR_NP ,  Mux(!io.dat.br_p ,  PC_BR18, PC_4),
                              Mux(cs_br_type === BR_EQ ,  Mux( io.dat.br_eq,  PC_BR12, PC_4),
                              Mux(cs_br_type === BR_NE ,  Mux(!io.dat.br_eq,  PC_BR12, PC_4),
                              Mux(cs_br_type === BR_JMPI ,  PC_JMPI,
                              Mux(cs_br_type === BR_JMPR ,  PC_JMPR,
                                                          PC_4))))))))))))
   val ctrl_pc_sel = Mux(io.ctl.exception || io.dat.csr_eret, PC_EXC, ctrl_pc_sel_no_xept)

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
   io.ctl.wb_sel   := cs_wb_sel
   io.ctl.reg_grp  := cs_reg_group
   io.ctl.rf_wen   := Mux(stall || io.ctl.exception, false.B, cs_rf_wen)

   // convert CSR instructions with raddr1 == 0 to read-only CSR commands
   val rs1_addr = io.dat.inst(RS1_MSB, RS1_LSB)

   io.ctl.csr_cmd  := Mux(stall, CSR.N, cs_csr_cmd)

   // Memory Requests
   io.dmem.req.valid    := mem_en && !io.ctl.exception
   io.dmem.req.bits.fcn := cs_mem_fcn
   io.dmem.req.bits.typ := cs_msk_sel

   // Exception Handling ---------------------
   io.ctl.pc_sel_no_xept := ctrl_pc_sel_no_xept
   val illegal = (!cs_val_inst && io.imem.resp.valid)

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
