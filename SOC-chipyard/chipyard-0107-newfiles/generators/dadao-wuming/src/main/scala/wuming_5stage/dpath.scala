//**************************************************************************
// RISCV Processor 5-Stage Datapath
//--------------------------------------------------------------------------
//
// Christopher Celio
// 2012 Jan 13
//
// TODO refactor stall, kill, fencei, flush signals. They're more confusing than they need to be.

package wuming.stage5

import chisel3._
import chisel3.util._

import freechips.rocketchip.config.Parameters
import freechips.rocketchip.rocket.{CSR, CSRFile, Causes}
import freechips.rocketchip.tile.CoreInterrupts

import Constants._
import wuming.common._

class DatToCtlIo(implicit val conf: WumingCoreParams) extends Bundle()
{
   val dec_inst    = Output(UInt(conf.instlen.W))
   val dec_valid   = Output(Bool())
   val exe_cond_eq = Output(Bool())
   val exe_cond_z  = Output(Bool())
   val exe_cond_p  = Output(Bool())
   val exe_cond_n  = Output(Bool())
   val exe_cf_type = Output(UInt(3.W))
   val exe_cd_type = Output(UInt(4.W))
   val exe_inst_misaligned = Output(Bool())

   val mem_ctrl_dmem_val = Output(Bool())
   val mem_data_misaligned = Output(Bool())
   val mem_store = Output(Bool())

   val csr_eret = Output(Bool())
   val csr_interrupt = Output(Bool())
   
   val inst_rasp_excp  = Output(Bool())
}

class DpathIo(implicit val p: Parameters, val conf: WumingCoreParams) extends Bundle
{
   val ddpath = Flipped(new DebugDPath())
   val imem = new MemPortIo(conf.instlen)
   val dmem = new MemPortIo(conf.xprlen)
   val ctl  = Flipped(new CtlToDatIo())
   val dat  = new DatToCtlIo()
   val interrupt = Input(new CoreInterrupts())
   val hartid = Input(UInt())
   val reset_vector = Input(UInt())
}

class DatPath(implicit val p: Parameters, val conf: WumingCoreParams) extends Module
{
   val io = IO(new DpathIo())
   io := DontCare

   //**********************************
   // Exception handling values (all read during mem_stage)
   val mem_tval_data_ma = Wire(UInt(conf.xprlen.W))
   // val mem_tval_inst_ma = Wire(UInt(conf.xprlen.W))

   //**********************************

   //**********************************
   // Pipeline State Registers

   // Instruction Fetch State
   val if_reg_pc             = RegInit(io.reset_vector)

   // Instruction Decode State
   val dec_reg_valid         = RegInit(false.B)
   val dec_reg_inst          = RegInit(BUBBLE)
   val dec_reg_pc            = RegInit(0.asUInt(conf.xprlen.W))

   // Execute State
   val exe_reg_valid         = RegInit(false.B)
   val exe_reg_inst          = RegInit(BUBBLE)
   val exe_reg_pc            = RegInit(0.asUInt(conf.xprlen.W))
   val exe_reg_op1_data      = Reg(UInt(conf.xprlen.W))
   val exe_reg_op2_data      = Reg(UInt(conf.xprlen.W))
   val exe_reg_rdha_data     = Reg(UInt(conf.xprlen.W))
   val exe_reg_rdhb_data     = Reg(UInt(conf.xprlen.W))
   val exe_reg_imms12_data   = Reg(UInt(conf.xprlen.W))
   val exe_reg_imms18_data   = Reg(UInt(conf.xprlen.W))
   val exe_reg_imms24_data   = Reg(UInt(conf.xprlen.W))
   val exe_reg_mw_data       = Reg(UInt(conf.xprlen.W))
   val exe_reg_ha_addr       = Reg(UInt(6.W))
   val exe_reg_hb_addr       = Reg(UInt(6.W))
   val exe_reg_hc_addr       = Reg(UInt(6.W))
   val exe_reg_hd_addr       = Reg(UInt(6.W))
   val exe_reg_ctrl_cf_type  = RegInit(CF_X)
   val exe_reg_ctrl_cd_type  = RegInit(COND_N)
   val exe_reg_ctrl_op2_sel  = Reg(UInt())
   val exe_reg_ctrl_alu_fun  = Reg(UInt())
   val exe_reg_ctrl_wb_sel   = Reg(UInt())
   val exe_reg_ctrl_mem_val  = RegInit(false.B)
   val exe_reg_ctrl_mem_fcn  = RegInit(M_X)
   val exe_reg_ctrl_mem_typ  = RegInit(MT_X)
   val exe_reg_ctrl_csr_cmd  = RegInit(CSR.N)
   val exe_reg_ret_target    = Reg(UInt(conf.xprlen.W))
   val exe_wydemask          = Reg(UInt(conf.xprlen.W))

   // Memory State
   val mem_reg_valid         = RegInit(false.B)
   val mem_reg_pc            = Reg(UInt(conf.xprlen.W))
   val mem_reg_inst          = RegInit(BUBBLE)
   val mem_reg_op1_data      = Reg(UInt(conf.xprlen.W))
   val mem_reg_op2_data      = Reg(UInt(conf.xprlen.W))
   val mem_reg_mw_data       = Reg(UInt(conf.xprlen.W))
   val mem_reg_alu_out     = Reg(UInt(conf.xprlen.W))
   val mem_reg_alu_out2      = Reg(UInt(conf.xprlen.W))
   val mem_reg_ha_addr       = Reg(UInt(6.W))
   val mem_reg_hb_addr       = Reg(UInt(6.W))
   val mem_reg_hc_addr       = Reg(UInt(6.W))
   val mem_reg_hd_addr       = Reg(UInt(6.W))
   val mem_reg_ctrl_mem_val  = RegInit(false.B)
   val mem_reg_ctrl_mem_fcn  = RegInit(M_X)
   val mem_reg_ctrl_mem_typ  = RegInit(MT_X)
   val mem_reg_ctrl_wb_sel   = Reg(UInt())
   val mem_reg_ctrl_csr_cmd  = RegInit(CSR.N)

   // Writeback State
   val wb_reg_valid          = RegInit(false.B)
   val wb_reg_ha_addr       = Reg(UInt(6.W))
   val wb_reg_hb_addr       = Reg(UInt(6.W))
   val wb_reg_hc_addr       = Reg(UInt(6.W))
   val wb_reg_hd_addr       = Reg(UInt(6.W))
   val wb_reg_wb_data        = Reg(UInt(conf.xprlen.W))
   val wb_reg_wb_data2       = Reg(UInt(conf.xprlen.W))
   val wb_reg_ctrl_wb_sel    = Reg(UInt(4.W))


   //**********************************
   // Instruction Fetch Stage
   val if_pc_next          = Wire(UInt(conf.xprlen.W))
   val exception_target    = Wire(UInt(conf.xprlen.W))
   val exe_br12_target     = Wire(UInt(conf.xprlen.W))
   val exe_br18_target     = Wire(UInt(conf.xprlen.W))
   val exe_iiii_target     = Wire(UInt(conf.xprlen.W))
   val exe_rrii_target     = Wire(UInt(conf.xprlen.W))
   val exe_ret_target      = Wire(UInt(conf.xprlen.W))

   // Instruction fetch buffer
   val if_buffer_in = Wire(new DecoupledIO(new MemResp(conf.instlen)))
   if_buffer_in.bits := io.imem.resp.bits
   if_buffer_in.valid := io.imem.resp.valid
   assert(!(if_buffer_in.valid && !if_buffer_in.ready), "Instruction backlog")

   val if_buffer_out = Queue(if_buffer_in, entries = 1, pipe = false, flow = true)
   if_buffer_out.ready := !io.ctl.dec_stall && !io.ctl.full_stall

   // Instruction PC buffer
   val if_pc_buffer_in = Wire(new DecoupledIO(UInt(conf.xprlen.W)))
   if_pc_buffer_in.bits := if_reg_pc
   if_pc_buffer_in.valid := if_buffer_in.valid

   val if_pc_buffer_out = Queue(if_pc_buffer_in, entries = 1, pipe = false, flow = true)
   if_pc_buffer_out.ready := if_buffer_out.ready

   // Instruction fetch kill flag buffer
   val if_reg_killed = RegInit(false.B)
   when ((io.ctl.pipeline_kill || io.ctl.if_kill) && !if_buffer_out.fire)
   {
      if_reg_killed := true.B
   }
   when (if_reg_killed && if_buffer_out.fire)
   {
      if_reg_killed := false.B
   }

   // Do not change the PC again if the instruction is killed in previous cycles (when the PC has changed)
   when ((if_buffer_in.fire && !if_reg_killed) || io.ctl.if_kill || io.ctl.pipeline_kill)
   {
      if_reg_pc := if_pc_next
   }

   val if_pc_plus4 = (if_reg_pc + 4.asUInt(conf.xprlen.W))

   if_pc_next := Mux(io.ctl.exe_pc_sel === PC_4,      if_pc_plus4,
                 Mux(io.ctl.exe_pc_sel === PC_BR12,   exe_br12_target,
                 Mux(io.ctl.exe_pc_sel === PC_BR18,   exe_br18_target,
                 Mux(io.ctl.exe_pc_sel === PC_IIII,   exe_iiii_target,
                 Mux(io.ctl.exe_pc_sel === PC_RRII,   exe_rrii_target,
                 Mux(io.ctl.exe_pc_sel === PC_RASP,   exe_ret_target,
                 /*Mux(io.ctl.exe_pc_sel === PC_EXC*/ exception_target))))))//)))

   // for a fencei, refetch the if_pc (assuming no stall, no branch, and no exception)
   when (io.ctl.fencei && io.ctl.exe_pc_sel === PC_4 &&
         !io.ctl.dec_stall && !io.ctl.full_stall && !io.ctl.pipeline_kill)
   {
      if_pc_next := if_reg_pc
   }

   // Instruction Memory
   io.imem.req.valid := if_buffer_in.ready
   io.imem.req.bits.fcn := M_XRD
   io.imem.req.bits.typ := MT_TU
   io.imem.req.bits.addr := if_reg_pc

   when (io.ctl.pipeline_kill)
   {
      dec_reg_valid := false.B
      dec_reg_inst := BUBBLE
   }
   .elsewhen (!io.ctl.dec_stall && !io.ctl.full_stall)
   {
      when (io.ctl.if_kill || if_reg_killed)
      {
         dec_reg_valid := false.B
         dec_reg_inst := BUBBLE
      }
      .elsewhen (if_buffer_out.valid)
      {
         dec_reg_valid := true.B
         dec_reg_inst := if_buffer_out.bits.data
      }
      .otherwise
      {
         dec_reg_valid := false.B
         dec_reg_inst := BUBBLE
      }

      dec_reg_pc := if_pc_buffer_out.bits
   }
   .otherwise 
   {
      when (io.ctl.if_kill || if_reg_killed)
      {
         dec_reg_valid := false.B
         dec_reg_inst := BUBBLE
      }
   }


   //**********************************
   // Decode Stage

   val dec_ha_addr  = dec_reg_inst(HA_MSB, HA_LSB)
   val dec_hb_addr  = dec_reg_inst(HB_MSB, HB_LSB)
   val dec_hc_addr  = dec_reg_inst(HC_MSB, HC_LSB)
   val dec_hd_addr  = dec_reg_inst(HD_MSB, HD_LSB)

   // Register File
   val regfile = Module(new RegisterFile())
   regfile.io.ha_addr := dec_ha_addr
   regfile.io.hb_addr := dec_hb_addr
   regfile.io.hc_addr := dec_hc_addr
   regfile.io.hd_addr := dec_hd_addr
   val rf_rdha_data = regfile.io.rdha_data
   val rf_rdhb_data = regfile.io.rdhb_data
   val rf_rdhc_data = regfile.io.rdhc_data
   val rf_rdhd_data = regfile.io.rdhd_data

   val rf_rbha_data = regfile.io.rbha_data
   val rf_rbhb_data = regfile.io.rbhb_data
   val rf_rbhc_data = regfile.io.rbhc_data
   val rf_rbhd_data = regfile.io.rbhd_data

   val rf_rfha_data = regfile.io.rfha_data
   val rf_rfhb_data = regfile.io.rfhb_data
   val rf_rfhc_data = regfile.io.rfhc_data

   exe_reg_ret_target := regfile.io.rapop_data
   val dec_pc_plus4    = (dec_reg_pc + 4.U)(conf.xprlen-1,0)
                     
   regfile.io.ras_pop   := (io.ctl.dec_reg_grp === RAS_POP && !(io.ctl.dec_stall || io.ctl.full_stall || io.ctl.pipeline_kill || io.ctl.dec_kill))
   io.dat.inst_rasp_excp := (((regfile.io.ras_push) && (regfile.io.ras_top === 63.U)) || ((regfile.io.ras_pop) && (regfile.io.ras_top === 0.U)))

   regfile.io.ras_push := (io.ctl.dec_reg_grp === RAS_PUSH && !(io.ctl.dec_stall || io.ctl.full_stall || io.ctl.pipeline_kill || io.ctl.dec_kill))
   regfile.io.rapush_data := dec_pc_plus4

   regfile.io.waddr := MuxCase(wb_reg_ha_addr, Array(
                         (wb_reg_ctrl_wb_sel === WB_RDHB) -> wb_reg_hb_addr,
                         (wb_reg_ctrl_wb_sel === WB_RDHC) -> wb_reg_hc_addr,
                         (wb_reg_ctrl_wb_sel === WB_RBHB) -> wb_reg_hb_addr,
                         (wb_reg_ctrl_wb_sel === WB_RFHB) -> wb_reg_hb_addr,
                         (wb_reg_ctrl_wb_sel === WB_HAHB) -> wb_reg_hb_addr,
                         (wb_reg_ctrl_wb_sel === WB_CSR)  -> wb_reg_hd_addr,
                      ))
   regfile.io.wrden := MuxCase(N, Array(
                         (wb_reg_ctrl_wb_sel === WB_RDHA) -> Y,
                         (wb_reg_ctrl_wb_sel === WB_RDHB) -> Y,
                         (wb_reg_ctrl_wb_sel === WB_RDHC) -> Y,
                         (wb_reg_ctrl_wb_sel === WB_HAHB) -> Y,
                         (wb_reg_ctrl_wb_sel === WB_RDMM) -> Y,
                         (wb_reg_ctrl_wb_sel === WB_CSR)  -> Y
                      ))
   regfile.io.wrben := MuxCase(N, Array(
                         (wb_reg_ctrl_wb_sel === WB_RBHA) -> Y,
                         (wb_reg_ctrl_wb_sel === WB_RBHB) -> Y,
                         (wb_reg_ctrl_wb_sel === WB_RBMM) -> Y
                      ))
   regfile.io.wrfen := MuxCase(N, Array(
                         (wb_reg_ctrl_wb_sel === WB_RFHA) -> Y,
                         (wb_reg_ctrl_wb_sel === WB_RFHB) -> Y
                      ))
   regfile.io.wdata := wb_reg_wb_data
   regfile.io.w2en  := MuxCase(N, Array(
                         (wb_reg_ctrl_wb_sel === WB_HAHB) -> Y
                      ))
   regfile.io.w2addr := wb_reg_ha_addr
   regfile.io.w2data := wb_reg_wb_data2


   // immediates

   val immu6 = dec_reg_inst(HD_MSB, HD_LSB)
   val immu12 = dec_reg_inst(HC_MSB, HD_LSB)
   val imms12 = Cat(Fill(52, dec_reg_inst(HC_MSB)), dec_reg_inst(HC_MSB, HD_LSB))
   val imms18 = Cat(Fill(46, dec_reg_inst(HB_MSB)), dec_reg_inst(HB_MSB, HD_LSB))
   val imms24 = Cat(Fill(40, dec_reg_inst(HA_MSB)), dec_reg_inst(HA_MSB, HD_LSB))

   val dec_wydeposition = dec_reg_inst(WP_MSB, WP_LSB).asUInt() << 4
   val dec_wyde16       = dec_reg_inst(WYDE_MSB, WYDE_LSB)
   val dec_wydemask     = Fill(64, 1.U) & ~(Fill(16, 1.U) << dec_wydeposition)

   // Operand 2 Mux
   val dec_alu_op2 = MuxCase(0.U, Array(
               (io.ctl.op2_sel === OP2_RDHB)   -> rf_rdhb_data,
               (io.ctl.op2_sel === OP2_RDHC)   -> rf_rdhc_data,
               (io.ctl.op2_sel === OP2_RDHD)   -> rf_rdhd_data,
               (io.ctl.op2_sel === OP2_RBHC)   -> rf_rbhc_data,
               (io.ctl.op2_sel === OP2_RBHD)   -> rf_rbhd_data,
               (io.ctl.op2_sel === OP2_RFHC)   -> rf_rfhc_data,
               (io.ctl.op2_sel === OP2_IMMU6)  -> immu6,
               (io.ctl.op2_sel === OP2_IMMU12) -> immu12,
               (io.ctl.op2_sel === OP2_IMMS12) -> imms12,
               (io.ctl.op2_sel === OP2_IMMS18) -> imms18,
               (io.ctl.op2_sel === OP2_WYDE)   -> (dec_wyde16 << dec_wydeposition)
               )).asUInt()

   // Bypass Muxes
   val exe_alu_out  = Wire(UInt(conf.xprlen.W))
   val alu_out2       = Wire(UInt(conf.xprlen.W))

   val dec_op1_data = Wire(UInt(conf.xprlen.W))
   val dec_op2_data = Wire(UInt(conf.xprlen.W))

   val dec_mw_data = MuxCase(0.U, Array(
                        (io.ctl.dec_reg_grp === REG_MRD) -> rf_rdha_data,
                        (io.ctl.dec_reg_grp === REG_MRB) -> rf_rbha_data,
                        (io.ctl.dec_reg_grp === REG_MRF) -> rf_rfha_data,
                        (io.ctl.dec_reg_grp === REG_RD)  -> rf_rdha_data,
                        (io.ctl.dec_reg_grp === REG_RB)  -> rf_rbha_data,
                        (io.ctl.dec_reg_grp === REG_RF)  -> rf_rfha_data,
                     ))

   if (USE_FULL_BYPASSING)
   {
      dec_op1_data := 0.U
      dec_op2_data := 0.U
   }
   else
   {
      // Rely only on control interlocking to resolve hazards
      dec_op1_data := MuxCase(0.U, Array(
                     ((io.ctl.op1_sel === OP1_RDHA)) -> rf_rdha_data,
                     ((io.ctl.op1_sel === OP1_RDHB)) -> rf_rdhb_data,
                     ((io.ctl.op1_sel === OP1_RDHC)) -> rf_rdhc_data,
                     
                     ((io.ctl.op1_sel === OP1_RBHA)) -> rf_rbha_data,
                     ((io.ctl.op1_sel === OP1_RBHB)) -> rf_rbhb_data,
                     ((io.ctl.op1_sel === OP1_RBHC)) -> rf_rbhc_data,

                     ((io.ctl.op1_sel === OP1_RFHA)) -> rf_rfha_data,
                     ((io.ctl.op1_sel === OP1_PC))  -> dec_reg_pc
                     ))
      dec_op2_data := dec_alu_op2
   }


   when ((io.ctl.dec_stall && !io.ctl.full_stall) || io.ctl.pipeline_kill)
   {
      // (kill exe stage)
      // insert NOP (bubble) into Execute stage on front-end stall (e.g., hazard clearing)
      exe_reg_valid         := false.B
      exe_reg_inst          := BUBBLE
      exe_reg_ctrl_mem_val  := false.B
      exe_reg_ctrl_mem_fcn  := M_X
      exe_reg_ctrl_csr_cmd  := CSR.N
      exe_reg_ctrl_cf_type  := CF_X
      exe_reg_ctrl_cd_type  := COND_X
      exe_reg_ctrl_wb_sel   := WB_X
   }
   .elsewhen(!io.ctl.dec_stall && !io.ctl.full_stall)
   {
      // no stalling...
      exe_reg_pc            := dec_reg_pc
      exe_reg_op1_data      := dec_op1_data
      exe_reg_op2_data      := dec_op2_data
      exe_reg_rdha_data     := rf_rdha_data
      exe_reg_rdhb_data     := rf_rdhb_data
      exe_reg_imms12_data   := imms12
      exe_reg_imms18_data   := imms18
      exe_reg_imms24_data   := imms24
      exe_reg_ha_addr       := dec_ha_addr
      exe_reg_hb_addr       := dec_hb_addr
      exe_reg_hc_addr       := dec_hc_addr
      exe_reg_hd_addr       := dec_hd_addr
      exe_reg_mw_data       := dec_mw_data
      exe_reg_ctrl_op2_sel  := io.ctl.op2_sel
      exe_reg_ctrl_alu_fun  := io.ctl.alu_fun
      exe_wydemask          := dec_wydemask

      when (io.ctl.dec_kill)
      {
         exe_reg_valid         := false.B
         exe_reg_inst          := BUBBLE
         exe_reg_ctrl_mem_val  := false.B
         exe_reg_ctrl_mem_fcn  := M_X
         exe_reg_ctrl_csr_cmd  := CSR.N
         exe_reg_ctrl_cf_type  := CF_X
         exe_reg_ctrl_cd_type  := COND_X
         exe_reg_ctrl_wb_sel   := WB_X
      }
      .otherwise
      {
         exe_reg_valid         := dec_reg_valid
         exe_reg_inst          := dec_reg_inst
         exe_reg_ctrl_mem_val  := io.ctl.mem_val
         exe_reg_ctrl_mem_fcn  := io.ctl.mem_fcn
         exe_reg_ctrl_mem_typ  := io.ctl.mem_typ
         exe_reg_ctrl_csr_cmd  := io.ctl.csr_cmd
         exe_reg_ctrl_cf_type  := io.ctl.cf_type
         exe_reg_ctrl_cd_type  := io.ctl.cd_type
         exe_reg_ctrl_wb_sel   := io.ctl.wb_sel
      }
   }

   //**********************************
   // Execute Stage

   val exe_alu_op1 = exe_reg_op1_data.asUInt()
   val exe_alu_op2 = exe_reg_op2_data.asUInt()

   // ALU
   val alu_shamt     = exe_alu_op2(5,0).asUInt()
   val exe_adder_out = (exe_alu_op1 + exe_alu_op2)(conf.xprlen-1,0)

   val cond_yes   =           Mux(exe_reg_ctrl_cd_type === COND_N ,  Mux( io.dat.exe_cond_n ,  Y, N),
                              Mux(exe_reg_ctrl_cd_type === COND_Z ,  Mux( io.dat.exe_cond_z ,  Y, N),
                              Mux(exe_reg_ctrl_cd_type === COND_P ,  Mux( io.dat.exe_cond_p ,  Y, N),
                              Mux(exe_reg_ctrl_cd_type === COND_EQ,  Mux( io.dat.exe_cond_eq,  Y, N),
                              Mux(exe_reg_ctrl_cd_type === COND_NE,  Mux(!io.dat.exe_cond_eq,  Y, N), N)))))

   exe_alu_out := MuxCase(0.U, Array(
                  (exe_reg_ctrl_alu_fun === ALU_ADD) -> exe_adder_out,
                  (exe_reg_ctrl_alu_fun === ALU_SUB) -> (exe_alu_op1 - exe_alu_op2).asUInt(),
                  (exe_reg_ctrl_alu_fun === ALU_CMPS) -> Mux(exe_alu_op1 === exe_alu_op2, 0.U, Mux(exe_alu_op1.asSInt() < exe_alu_op2.asSInt(), ~0.U(BITS_OCTA.W), 1.U(BITS_OCTA.W))).asUInt(),
                  (exe_reg_ctrl_alu_fun === ALU_CMPU) -> Mux(exe_alu_op1 === exe_alu_op2, 0.U, Mux(exe_alu_op1.asUInt() < exe_alu_op2.asUInt(), ~0.U(BITS_OCTA.W), 1.U(BITS_OCTA.W))).asUInt(),
                  (exe_reg_ctrl_alu_fun === ALU_MULS) -> ((exe_alu_op1.asSInt() * exe_alu_op2.asSInt())(BITS_OCTA-1, 0)).asUInt(),
                  (exe_reg_ctrl_alu_fun === ALU_MULU) -> ((exe_alu_op1.asUInt() * exe_alu_op2.asUInt())(BITS_OCTA-1, 0)).asUInt(),
                  (exe_reg_ctrl_alu_fun === ALU_DIVS) -> (exe_alu_op1.asSInt() / exe_alu_op2.asSInt()).asUInt(),
                  (exe_reg_ctrl_alu_fun === ALU_DIVU) -> (exe_alu_op1.asUInt() / exe_alu_op2.asUInt()).asUInt(),
                  (exe_reg_ctrl_alu_fun === ALU_SLL) -> ((exe_alu_op1 << alu_shamt)(conf.xprlen-1, 0)).asUInt(),
                  (exe_reg_ctrl_alu_fun === ALU_SRL) -> (exe_alu_op1 >> alu_shamt).asUInt(),
                  (exe_reg_ctrl_alu_fun === ALU_SRA) -> (exe_alu_op1.asSInt() >> alu_shamt).asUInt(),
                  (exe_reg_ctrl_alu_fun === ALU_AND) -> (exe_alu_op1 & exe_alu_op2).asUInt(),
                  (exe_reg_ctrl_alu_fun === ALU_OR) -> (exe_alu_op1 | exe_alu_op2).asUInt(),
                  (exe_reg_ctrl_alu_fun === ALU_XOR) -> (exe_alu_op1 ^ exe_alu_op2).asUInt(),
                  (exe_reg_ctrl_alu_fun === ALU_XNOR) -> (exe_alu_op1 ^ ~exe_alu_op2).asUInt(),
                  (exe_reg_ctrl_alu_fun === ALU_EXTS) -> ((exe_alu_op1 << alu_shamt)(BITS_OCTA-1, 0).asSInt() >> alu_shamt).asUInt(),
                  (exe_reg_ctrl_alu_fun === ALU_EXTZ) -> ((exe_alu_op1 << alu_shamt)(BITS_OCTA-1, 0).asUInt() >> alu_shamt).asUInt(),
                  (exe_reg_ctrl_alu_fun === ALU_SETOW) -> (exe_alu_op2 | exe_wydemask).asUInt(),
                  (exe_reg_ctrl_alu_fun === ALU_ANDNW) -> (exe_alu_op1 & ~exe_alu_op2).asUInt(),
                  (exe_reg_ctrl_alu_fun === ALU_SETW) -> ((exe_alu_op1 & exe_wydemask) | exe_alu_op2).asUInt(),
                  (exe_reg_ctrl_alu_fun === ALU_CSET) -> Mux(cond_yes, exe_alu_op1, exe_alu_op2).asUInt(),
                  (exe_reg_ctrl_alu_fun === ALU_COPY2) -> exe_alu_op2.asUInt(),
                  (exe_reg_ctrl_alu_fun === ALU_ADRP) -> ((((exe_alu_op1 >> 12.U) + exe_alu_op2) << 12.U)).asUInt(),
                  ))

      
   alu_out2    := MuxCase(0.U, Array(
                  (exe_reg_ctrl_alu_fun === ALU_ADD)    -> (Cat(Fill(64, exe_alu_op1(63)), exe_alu_op1) + Cat(Fill(64, exe_alu_op2(63)), exe_alu_op2))(127, 64).asUInt(),
                  (exe_reg_ctrl_alu_fun === ALU_SUB)    -> (Cat(Fill(64, exe_alu_op1(63)), exe_alu_op1) - Cat(Fill(64, exe_alu_op2(63)), exe_alu_op2))(127, 64).asUInt(),
                  (exe_reg_ctrl_alu_fun === ALU_MULS)   -> ((exe_alu_op1.asSInt() * exe_alu_op2.asSInt())(127, 64)).asUInt(),
                  (exe_reg_ctrl_alu_fun === ALU_MULU)   -> ((exe_alu_op1.asUInt() * exe_alu_op2.asUInt())(127, 64)).asUInt(),
                  (exe_reg_ctrl_alu_fun === ALU_DIVS)   -> (exe_alu_op1.asSInt() % exe_alu_op2.asSInt()).asUInt(),
                  (exe_reg_ctrl_alu_fun === ALU_DIVU)   -> (exe_alu_op1.asUInt() % exe_alu_op2.asUInt()).asUInt(),
                  ))

   // Branch/Jump Target Calculation

   val br12_offset     = exe_reg_imms12_data << 2.U
   val br18_offset     = exe_reg_imms18_data << 2.U
   val br24_offset     = exe_reg_imms24_data << 2.U
   exe_br12_target    := exe_reg_pc + br12_offset
   exe_br18_target    := exe_reg_pc + br18_offset
   exe_iiii_target    := exe_reg_pc + br24_offset
   exe_rrii_target    := exe_alu_out + br12_offset
   exe_ret_target     := exe_reg_ret_target

   // Instruction misalign detection
   // In control path, instruction misalignment exception is always raised in the next cycle once the misaligned instruction reaches
   // execution stage, regardless whether the pipeline stalls or not
   // ( May be unnessisary in SimRISC )

   val exe_pc_plus4    = (exe_reg_pc + 4.U)(conf.xprlen-1,0)

   when (io.ctl.pipeline_kill)
   {
      mem_reg_valid         := false.B
      mem_reg_inst          := BUBBLE
      mem_reg_ctrl_mem_val  := false.B
      mem_reg_ctrl_csr_cmd  := false.B
      mem_reg_ctrl_wb_sel   := WB_X
   }
   .elsewhen (!io.ctl.full_stall)
   {
      mem_reg_valid         := exe_reg_valid
      mem_reg_pc            := exe_reg_pc
      mem_reg_inst          := exe_reg_inst
      mem_reg_alu_out     := Mux((exe_reg_ctrl_wb_sel === WB_RA), exe_pc_plus4, exe_alu_out)
      mem_reg_alu_out2      := alu_out2
      mem_reg_mw_data       := exe_reg_mw_data
      mem_reg_op1_data      := exe_reg_op1_data
      mem_reg_op2_data      := exe_reg_op2_data
      mem_reg_ha_addr       := exe_reg_ha_addr
      mem_reg_hb_addr       := exe_reg_hb_addr
      mem_reg_hc_addr       := exe_reg_hc_addr
      mem_reg_hd_addr       := exe_reg_hd_addr
      mem_reg_ctrl_mem_val  := exe_reg_ctrl_mem_val
      mem_reg_ctrl_mem_fcn  := exe_reg_ctrl_mem_fcn
      mem_reg_ctrl_mem_typ  := exe_reg_ctrl_mem_typ
      mem_reg_ctrl_wb_sel   := exe_reg_ctrl_wb_sel
      mem_reg_ctrl_csr_cmd  := exe_reg_ctrl_csr_cmd
   }

   //**********************************
   // Memory Stage

   // Control Status Registers
   // The CSRFile can redirect the PC so it's easiest to put this in Execute for now.
   val csr = Module(new CSRFile(perfEventSets=CSREvents.events))
   csr.io := DontCare
   csr.io.decode(0).inst := MuxCase(mem_reg_inst, Array(
                          (mem_reg_inst(31, 24) === "b01110110".U)  -> "h00000073".U, /* TRAP   -> SCALL */
                          (mem_reg_inst(31, 24) === "b01110111".U)  -> "h30200073".U, /* EXCAPE -> MRET  */
                          ))
   csr.io.rw.addr   := MuxCase(mem_reg_inst(CP_RGRC_ADDR_MSB,CP_RGRC_ADDR_LSB), Array(
                          (mem_reg_inst(31, 24) === "b01110110".U)  -> "h000".U,      /* TRAP   -> SCALL */
                          (mem_reg_inst(31, 24) === "b01110111".U)  -> "h302".U,      /* EXCAPE -> MRET  */
                          ))
   csr.io.rw.wdata  := mem_reg_alu_out
   csr.io.rw.cmd    := mem_reg_ctrl_csr_cmd

   csr.io.retire    := wb_reg_valid
   csr.io.exception := io.ctl.mem_exception
   csr.io.pc        := mem_reg_pc
   exception_target := csr.io.evec

   csr.io.tval := MuxCase(0.U, Array(
                  (io.ctl.mem_exception_cause === Causes.illegal_instruction.U) -> RegNext(exe_reg_inst),
                  // (io.ctl.mem_exception_cause === Causes.misaligned_fetch.U)    -> mem_tval_inst_ma,
                  (io.ctl.mem_exception_cause === Causes.misaligned_store.U)    -> mem_tval_data_ma,
                  (io.ctl.mem_exception_cause === Causes.misaligned_load.U)     -> mem_tval_data_ma,
                  ))

   // Interrupt rising edge detector (output trap signal for one cycle on rising edge)
   val reg_interrupt_handled = RegNext(csr.io.interrupt, false.B)
   val interrupt_edge = csr.io.interrupt && !reg_interrupt_handled

   csr.io.interrupts := io.interrupt
   csr.io.hartid := io.hartid
   io.dat.csr_interrupt := interrupt_edge
   csr.io.cause := Mux(io.ctl.mem_exception, io.ctl.mem_exception_cause, csr.io.interrupt_cause)
   csr.io.ungated_clock := clock

   io.dat.csr_eret := csr.io.eret
   // TODO replay? stall?

   // Add your own uarch counters here!
   csr.io.counters.foreach(_.inc := false.B)


   // Data misalignment detection
   // For example, if type is 3 (word), the mask is ~(0b111 << (3 - 1)) = ~0b100 = 0b011.
   val misaligned_mask = Wire(UInt(3.W))
   misaligned_mask := ~(7.U(3.W) << (mem_reg_ctrl_mem_typ - 1.U)(1, 0))
   io.dat.mem_data_misaligned := (misaligned_mask & mem_reg_alu_out.asUInt.apply(2, 0)).orR && mem_reg_ctrl_mem_val
   io.dat.mem_store := mem_reg_ctrl_mem_fcn === M_XWR
   mem_tval_data_ma := mem_reg_alu_out.asUInt

   // WB Mux

   wb_reg_wb_data := MuxCase(mem_reg_alu_out, Array(
                  (mem_reg_ctrl_wb_sel === WB_RDMM) -> io.dmem.resp.bits.data,
                  (mem_reg_ctrl_wb_sel === WB_RBMM) -> io.dmem.resp.bits.data,
                  (mem_reg_ctrl_wb_sel === WB_CSR)  -> csr.io.rw.rdata
                  ))
   wb_reg_wb_data2 := MuxCase(0.U, Array(
                  (mem_reg_ctrl_wb_sel === WB_HAHB) -> mem_reg_alu_out2
                  ))


   //**********************************
   // Writeback Stage

   when (!io.ctl.full_stall)
   {
      wb_reg_valid         := mem_reg_valid && !io.ctl.mem_exception && !interrupt_edge
      wb_reg_ha_addr       := mem_reg_ha_addr
      wb_reg_hb_addr       := mem_reg_hb_addr
      wb_reg_hc_addr       := mem_reg_hc_addr
      wb_reg_hd_addr       := mem_reg_hd_addr
      wb_reg_ctrl_wb_sel   := mem_reg_ctrl_wb_sel
   }
   .otherwise
   {
      wb_reg_valid         := false.B
      wb_reg_ctrl_wb_sel   := WB_X
   }



   //**********************************
   // External Signals

   // datapath to controlpath outputs
   io.dat.dec_valid  := dec_reg_valid
   io.dat.dec_inst   := dec_reg_inst
   io.dat.exe_cond_eq := (exe_reg_rdha_data === exe_reg_rdhb_data)
   io.dat.exe_cond_z  := (exe_reg_rdha_data === 0.U)
   io.dat.exe_cond_p  := (exe_reg_rdha_data.asSInt() > 0.S)
   io.dat.exe_cond_n  := (exe_reg_rdha_data.asSInt() < 0.S)
   io.dat.exe_cf_type := exe_reg_ctrl_cf_type
   io.dat.exe_cd_type := exe_reg_ctrl_cd_type

   io.dat.mem_ctrl_dmem_val := mem_reg_ctrl_mem_val

   // datapath to data memory outputs
   io.dmem.req.valid     := mem_reg_ctrl_mem_val && !io.dat.mem_data_misaligned
   io.dmem.req.bits.addr := mem_reg_alu_out.asUInt()
   io.dmem.req.bits.fcn  := mem_reg_ctrl_mem_fcn
   io.dmem.req.bits.typ  := mem_reg_ctrl_mem_typ
   io.dmem.req.bits.data := mem_reg_mw_data

   val wb_reg_inst = RegNext(mem_reg_inst)

   printf("-\n")
   /*

   printf("Cyc= %d [%d] pc[%x] W[%d=%x] O1[%x] O2[%x] inst[%x] %c%c%c DASM(%x)\n",
      csr.io.time(31,0),
      csr.io.retire,
      RegNext(mem_reg_pc),
      wb_reg_ha_addr,
      wb_reg_wb_data,
      // wb_reg_ctrl_rf_wen,
      RegNext(mem_reg_op1_data),
      RegNext(mem_reg_op2_data),
      wb_reg_inst,
      MuxCase(Str(" "), Seq(
         io.ctl.pipeline_kill -> Str("K"),
         io.ctl.full_stall -> Str("F"),
         io.ctl.dec_stall -> Str("S"))),
      MuxLookup(io.ctl.exe_pc_sel, Str("?"), Seq(
         PC_BR12 -> Str("b"),
         PC_BR18 -> Str("B"),
         PC_IIII -> Str("I"),
         PC_RRII -> Str("R"),
         PC_RASP -> Str("A"),
         PC_EXCP -> Str("E"),
         PC_4 -> Str(" "))),
      Mux(csr.io.exception, Str("X"), Str(" ")),
      wb_reg_inst)

   printf("\t %x | %x | %x | %x | %x\n",
         if_buffer_out.bits.data,
         dec_reg_inst,
         exe_reg_inst,
         mem_reg_inst,
         wb_reg_inst,
      )

   printf("---------------------------------------------------\n")

   */
}
