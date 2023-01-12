package wuming.common
package constants
{

import chisel3._
import chisel3.util._
import scala.math._

trait DADAOConstants
{
   val BITS_INST = 32
   val BITS_HEXA = 6
   val BITS_OCTA  = 64
   val BITS_TETRA = 32
   val BITS_WYDE  = 16
   val BITS_BYTE  = 8

   // abstract out instruction decode magic numbers
   val HA_MSB = 23
   val HA_LSB = 18
   val HB_MSB = 17
   val HB_LSB = 12
   val HC_MSB = 11
   val HC_LSB = 6
   val HD_MSB = 5
   val HD_LSB = 0

   val WP_MSB = HB_MSB
   val WP_LSB = HB_MSB - 1
   val WYDE_MSB = HB_MSB - 2
   val WYDE_LSB = HD_LSB

   val CP_RGRC_ADDR_MSB = HB_MSB
   val CP_RGRC_ADDR_LSB = HC_LSB

   // RISCV related
   val RD_MSB  = 11
   val RD_LSB  = 7
   val RS1_MSB = 19
   val RS1_LSB = 15
   val RS2_MSB = 24
   val RS2_LSB = 20

   val X0 = 0.U

   // The Bubble Instruction (Machine generated NOP)
   // Insert (XOR x0,x0,x0) which is different from software compiler
   // generated NOPs which are (ADDI x0, x0, 0).
   // Reasoning for this is to let visualizers and stat-trackers differentiate
   // between software NOPs and machine-generated Bubbles in the pipeline.
   val BUBBLE  = 0x4033.U(32.W)
}

}


