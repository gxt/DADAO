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
