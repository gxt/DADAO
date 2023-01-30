package wuming.common
package constants
{

import chisel3._
import chisel3.util._
import scala.math._

trait DADAOConstants
{
   val BITS_HEXA  = 6
   val BITS_INST  = 32

   val BITS_BYTE  = (8*1)
   val BITS_WYDE  = (8*2)
   val BITS_TETRA = (8*4)
   val BITS_OCTA  = (8*8)
   val BITS_DOCTA = (8*16)

   val NR_REGS    = 64

   // abstract out instruction decode magic numbers
   val HA_MSB     = (4*6) - 1
   val HA_LSB     = (3*6)
   val HB_MSB     = (3*6) - 1
   val HB_LSB     = (2*6)
   val HC_MSB     = (2*6) - 1
   val HC_LSB     = (1*6)
   val HD_MSB     = (1*6) - 1
   val HD_LSB     = (0*6)

   // Wyde position need 2 bits, and each wyde 16 bits
   val WP_MSB     = HB_MSB
   val WP_LSB     = HB_MSB - 1
   val WYDE_MSB   = HB_MSB - 2
   val WYDE_LSB   = HD_LSB

   val CP_RGRC_ADDR_MSB = HB_MSB
   val CP_RGRC_ADDR_LSB = HC_LSB

   // The Bubble Instruction (Machine generated NOP)
   // Insert (SWYM 5) which is different from software compiler
   // generated NOPs which are (SWYM 0).
   // Reasoning for this is to let visualizers and stat-trackers differentiate
   // between software NOPs and machine-generated Bubbles in the pipeline.
   val BUBBLE  = 0x1005.U(32.W)
}

}


