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

   val ras_push    = Input(Bool())
   val ras_pop     = Input(Bool())
   val rapop_data  = Output(UInt(conf.xprlen.W))
   val rapush_data = Input(UInt(conf.xprlen.W))
   val ras_top     = Output(UInt(conf.xprlen.W))

   val waddr       = Input(UInt(6.W))
   val wdata       = Input(UInt(conf.xprlen.W))
   val wrben       = Input(Bool())
   val wrden       = Input(Bool())
   val wrfen       = Input(Bool())
   val w2addr      = Input(UInt(6.W))
   val w2data      = Input(UInt(conf.xprlen.W))
   val w2en        = Input(Bool())
}

class RegisterFile(implicit val conf: WumingCoreParams) extends Module
{
   val io = IO(new RFileIo())

   val regfileD = Mem(NR_REGS, UInt(conf.xprlen.W))
   val regfileB = Mem(NR_REGS, UInt(conf.xprlen.W))
   val regfileA = Mem(NR_REGS, UInt(conf.xprlen.W))
   val regfileF = Mem(NR_REGS, UInt(conf.xprlen.W))
   
   val RASR_TOP = RegInit(0.U(BITS_HEXA.W))

   val push_ptr  = RASR_TOP + 1.U
   val pop_ptr   = RASR_TOP

   when (io.ras_push)
   {
      RASR_TOP   := push_ptr
   }
   when (io.ras_pop)
   {
      RASR_TOP   := pop_ptr - 1.U
   }

   when (io.ras_push && (push_ptr =/= 0.U))
   {
      regfileA(push_ptr) := io.rapush_data
   }
   io.ras_top := RASR_TOP
   io.rapop_data := Mux((pop_ptr =/= 0.U), regfileA(pop_ptr), 0.U)

   when (io.wrden && (io.waddr =/= 0.U))
   {
      regfileD(io.waddr) := io.wdata
   }
   when (io.wrben && (io.waddr =/= 0.U))
   {
      regfileB(io.waddr) := io.wdata
   }
   when (io.wrfen)
   {
      regfileF(io.waddr) := io.wdata
   }
   when (io.w2en && (io.w2addr =/= 0.U))
   {
      regfileD(io.w2addr) := io.w2data
   }

   io.rdha_data := Mux((io.ha_addr =/= 0.U), regfileD(io.ha_addr), 0.U)
   io.rdhb_data := Mux((io.hb_addr =/= 0.U), regfileD(io.hb_addr), 0.U)
   io.rdhc_data := Mux((io.hc_addr =/= 0.U), regfileD(io.hc_addr), 0.U)
   io.rdhd_data := Mux((io.hd_addr =/= 0.U), regfileD(io.hd_addr), 0.U)

   io.rbha_data := Mux((io.ha_addr =/= 0.U), regfileB(io.ha_addr), 0.U)
   io.rbhb_data := Mux((io.hb_addr =/= 0.U), regfileB(io.hb_addr), 0.U)
   io.rbhc_data := Mux((io.hc_addr =/= 0.U), regfileB(io.hc_addr), 0.U)
   io.rbhd_data := Mux((io.hd_addr =/= 0.U), regfileB(io.hd_addr), 0.U)

   io.rfha_data := regfileF(io.ha_addr)
   io.rfhb_data := regfileF(io.hb_addr)
   io.rfhc_data := regfileF(io.hc_addr)

   printf("\tregfile:%x %x d:%d b:%d f:%d rd9:%x\n",
         io.wdata,
         io.waddr,
         io.wrden,
         io.wrben,
         io.wrfen,
         regfileD(9.U)
      )
}
