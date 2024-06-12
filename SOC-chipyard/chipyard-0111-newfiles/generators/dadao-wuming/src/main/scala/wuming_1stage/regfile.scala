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
   val push_data = Input(UInt(BITS_OCTA.W))
   val pop_data  = Output(UInt(BITS_OCTA.W))

   val ras_top  = Output(UInt(BITS_HEXA.W))
   val ras_push = Input(Bool())
   val ras_pop  = Input(Bool())
}

class RegFileA(implicit val conf: WumingCoreParams) extends Module
{
   val io = IO(new RFileIo())

   val RA_MEM = Mem(NR_REGS, UInt(BITS_OCTA.W))

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
      RA_MEM(push_ptr) := io.push_data
   }

   io.ras_top  := RASR_TOP
   io.pop_data := Mux((pop_ptr =/= 0.U), RA_MEM(pop_ptr), 0.U)
}
