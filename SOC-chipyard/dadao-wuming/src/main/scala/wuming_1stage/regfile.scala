//**************************************************************************
// DADAO Processor Register File
//--------------------------------------------------------------------------
//

package wuming.stage1

import chisel3._
import chisel3.util._


import Constants._
import wuming.common._

class RFileIo(implicit val conf: WumingCoreParams) extends Bundle()
{
   val push_data = Input(UInt(conf.xprlen.W))
   val pop_data  = Output(UInt(conf.xprlen.W))

   val ras_top  = Input(UInt(BITS_HEXA.W))
   val ras_push = Input(Bool())
   val ras_pop  = Input(Bool())

   val dm_addr  = Input(UInt(BITS_HEXA.W))
   val dm_rdata = Output(UInt(conf.xprlen.W))
   val dm_wdata = Input(UInt(conf.xprlen.W))
   val dm_en    = Input(Bool())
}

class RegFileA(implicit val conf: WumingCoreParams) extends Module
{
   val io = IO(new RFileIo())

   val RA_MEM = Mem(NR_REGS, UInt(conf.xprlen.W))

   when (io.ras_push && (io.ras_top =/= 0.U))
   {
      RA_MEM(io.ras_top) := io.push_data
   }

   when (io.dm_en && (io.dm_addr =/= 0.U))
   {
      RA_MEM(io.dm_addr) := io.dm_wdata
   }

   io.pop_data := Mux((io.ras_top =/= 0.U), RA_MEM(io.ras_top), 0.U)
   io.dm_rdata := Mux((io.dm_addr =/= 0.U), RA_MEM(io.dm_addr), 0.U)
}
