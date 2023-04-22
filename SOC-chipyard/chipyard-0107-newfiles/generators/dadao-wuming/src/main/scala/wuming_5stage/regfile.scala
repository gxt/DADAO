//**************************************************************************
// RISCV Processor Register File
//--------------------------------------------------------------------------
//

package wuming.stage5

import chisel3._
import chisel3.util._


import Constants._
import wuming.common._

class RFileIo(implicit val conf: WumingCoreParams) extends Bundle()
{
   val rs1_addr = Input(UInt(5.W))
   val rs1_data = Output(UInt(conf.xprlen.W))
   val rs2_addr = Input(UInt(5.W))
   val rs2_data = Output(UInt(conf.xprlen.W))
   val dm_addr = Input(UInt(5.W))
   val dm_rdata = Output(UInt(conf.xprlen.W))
   val dm_wdata = Input(UInt(conf.xprlen.W))
   val dm_en = Input(Bool())

   val waddr    = Input(UInt(5.W))
   val wdata    = Input(UInt(conf.xprlen.W))
   val wen      = Input(Bool())
}

class RegisterFile(implicit val conf: WumingCoreParams) extends Module
{
   val io = IO(new RFileIo())

   val regfileD = Mem(NR_REGS, UInt(conf.xprlen.W))
   val regfileB = Mem(NR_REGS, UInt(conf.xprlen.W))
   val regfileA = Mem(NR_REGS, UInt(conf.xprlen.W))
   val regfileF = Mem(NR_REGS, UInt(conf.xprlen.W))

   when (io.wen && (io.waddr =/= 0.U))
   {
      regfileD(io.waddr) := io.wdata
   }

   when (io.dm_en && (io.dm_addr =/= 0.U))
   {
      regfileD(io.dm_addr) := io.dm_wdata
   }

   io.rs1_data := Mux((io.rs1_addr =/= 0.U), regfileD(io.rs1_addr), 0.U)
   io.rs2_data := Mux((io.rs2_addr =/= 0.U), regfileD(io.rs2_addr), 0.U)
   io.dm_rdata := Mux((io.dm_addr =/= 0.U), regfileD(io.dm_addr), 0.U)

}

class S_RFileIo(implicit val conf: WumingCoreParams) extends Bundle()
{
   val ha_addr = Input(UInt(6.W))
   val hb_addr = Input(UInt(6.W))
   val hc_addr = Input(UInt(6.W))
   val hd_addr = Input(UInt(6.W))

   val rdha_data = Output(UInt(conf.xprlen.W))
   val rdhb_data = Output(UInt(conf.xprlen.W))
   val rdhc_data = Output(UInt(conf.xprlen.W))
   val rdhd_data = Output(UInt(conf.xprlen.W))

   val rbha_data = Output(UInt(conf.xprlen.W))
   val rbhb_data = Output(UInt(conf.xprlen.W))
   val rbhc_data = Output(UInt(conf.xprlen.W))
   val rbhd_data = Output(UInt(conf.xprlen.W))
   
   val rfha_data = Output(UInt(conf.xprlen.W))
   val rfhb_data = Output(UInt(conf.xprlen.W))
   val rfhc_data = Output(UInt(conf.xprlen.W))

   val rapop_data = Output(UInt(conf.xprlen.W))

   // val dm_addr = Input(UInt(5.W))
   // val dm_rdata = Output(UInt(conf.xprlen.W))
   // val dm_wdata = Input(UInt(conf.xprlen.W))
   // val dm_en = Input(Bool())

   // val waddr    = Input(UInt(5.W))
   // val wdata    = Input(UInt(conf.xprlen.W))
   // val wen      = Input(Bool())
   val waddr       = Input(UInt(6.W))
   val wdata       = Input(UInt(conf.xprlen.W))
   val wrben       = Input(Bool())
   val wrden       = Input(Bool())
   val wrfen       = Input(Bool())
   val w2addr      = Input(UInt(6.W))
   val w2data      = Input(UInt(conf.xprlen.W))
   val w2en        = Input(Bool())
}

class S_RegisterFile(implicit val conf: WumingCoreParams) extends Module
{
   val io = IO(new S_RFileIo())

   val regfileD = Mem(NR_REGS, UInt(conf.xprlen.W))
   val regfileB = Mem(NR_REGS, UInt(conf.xprlen.W))
   val regfileA = Mem(NR_REGS, UInt(conf.xprlen.W))
   val regfileF = Mem(NR_REGS, UInt(conf.xprlen.W))
   
   val RASR_TOP = RegInit(0.U(BITS_HEXA.W))

   val pop_ptr   = RASR_TOP

   io.rapop_data := Mux((pop_ptr =/= 0.U), regfileA(pop_ptr), 0.U)

   when (io.wrden && (io.waddr =/= 0.U))
   {
      regfileD(io.waddr) := io.wdata
   }
   when (io.wrben && (io.waddr =/= 0.U))
   {
      regfileB(io.waddr) := io.wdata
   }
   when (io.wrfen && (io.waddr =/= 0.U))
   {
      regfileF(io.waddr) := io.wdata
   }
   when (io.w2en && (io.w2addr =/= 0.U))
   {
      regfileD(io.w2addr) := io.w2data
   }


   // when (io.dm_en && (io.dm_addr =/= 0.U))
   // {
   //    regfileD(io.dm_addr) := io.dm_wdata
   // }

   io.rdha_data := Mux((io.ha_addr =/= 0.U), regfileD(io.ha_addr), 0.U)
   io.rdhb_data := Mux((io.hb_addr =/= 0.U), regfileD(io.hb_addr), 0.U)
   io.rdhc_data := Mux((io.hc_addr =/= 0.U), regfileD(io.hc_addr), 0.U)
   io.rdhd_data := Mux((io.hd_addr =/= 0.U), regfileD(io.hd_addr), 0.U)

   io.rbha_data := Mux((io.ha_addr =/= 0.U), regfileB(io.ha_addr), 0.U)
   io.rbhb_data := Mux((io.hb_addr =/= 0.U), regfileB(io.hb_addr), 0.U)
   io.rbhc_data := Mux((io.hc_addr =/= 0.U), regfileB(io.hc_addr), 0.U)
   io.rbhd_data := Mux((io.hd_addr =/= 0.U), regfileB(io.hd_addr), 0.U)

   io.rfha_data := Mux((io.ha_addr =/= 0.U), regfileF(io.ha_addr), 0.U)
   io.rfhb_data := Mux((io.hb_addr =/= 0.U), regfileF(io.hb_addr), 0.U)
   io.rfhc_data := Mux((io.hc_addr =/= 0.U), regfileF(io.hc_addr), 0.U)

   // io.dm_rdata := Mux((io.dm_addr =/= 0.U), regfileD(io.dm_addr), 0.U)

}
