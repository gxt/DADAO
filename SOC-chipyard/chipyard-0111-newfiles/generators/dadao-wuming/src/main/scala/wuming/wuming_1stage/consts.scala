//**************************************************************************
// RISCV Processor Constants
//--------------------------------------------------------------------------
//
// Christopher Celio
// 2011 May 28

package wuming.stage1
package constants
{

import chisel3._
import chisel3.util._


trait WumingProcConstants
{
   // for debugging, print out the commit information.
   // can be compared against the riscv-isa-run Spike ISA simulator's commit logger.
   val PRINT_COMMIT_LOG = false
}

trait ScalarOpConstants
{
   //************************************
   // Control Signals

   val Y      = true.B
   val N      = false.B

   // PC Select Signal
   val PC_4     = 0.asUInt(3.W)  // PC + 4
   val PC_EXCP  = 1.asUInt(3.W)  // exception
   val PC_BR12  = 2.asUInt(3.W)  // branch_target for imms12
   val PC_BR18  = 3.asUInt(3.W)  // branch_target for imms18
   val PC_IIII  = 4.asUInt(3.W)  // jump/call iiii target
   val PC_RRII  = 5.asUInt(3.W)  // jump/call rrii target
   val PC_RASP  = 6.asUInt(3.W)  // pop from RA regfile

   // Control flow signal
   val CF_X     = 0.asUInt(3.W)  // Next
   val CF_BR12  = 1.asUInt(3.W)  // Branch with offset 12-bit
   val CF_BR18  = 2.asUInt(3.W)  // Branch with offset 18-bit
   val CF_JUMPI = 3.asUInt(3.W)  // Jump iiii
   val CF_JUMPR = 4.asUInt(3.W)  // Jump rrii
   val CF_CALLI = 5.asUInt(3.W)  // Call iiii
   val CF_CALLR = 6.asUInt(3.W)  // Call rrii
   val CF_RET   = 7.asUInt(3.W)  // Return

   // Conditional func
   val COND_X   = 0.asUInt(4.W)   // Always
   val COND_N   = 8.asUInt(4.W)   // Condition Negative
   val COND_NN  = 9.asUInt(4.W)   // Condition Not Negative
   val COND_Z   = 10.asUInt(4.W)  // Condition Zero
   val COND_NZ  = 11.asUInt(4.W)  // Condition Not Zero
   val COND_P   = 12.asUInt(4.W)  // Condition Positive
   val COND_NP  = 13.asUInt(4.W)  // Condition Not Positive
   val COND_NE  = 14.asUInt(4.W)  // Condition NotEqual
   val COND_EQ  = 15.asUInt(4.W)  // Condition Equal

   // Instruction Type
   val REG_RD    = 0.asUInt(4.W)
   val REG_RB    = 1.asUInt(4.W)
   val REG_RF    = 2.asUInt(4.W)
   val REG_RA    = 3.asUInt(4.W)
   val REG_MRD   = 4.asUInt(4.W)
   val REG_MRB   = 5.asUInt(4.W)
   val REG_MRF   = 6.asUInt(4.W)
   val REG_MRA   = 7.asUInt(4.W)
   val RAS_PUSH  = 8.asUInt(4.W)
   val RAS_POP   = 9.asUInt(4.W)
   val REG_CSR   = 15.asUInt(4.W)
   val REG_X     = 0.asUInt(4.W)

   // Operand 1 Select Signal
   val OP1_RDHA = 1.asUInt(4.W)
   val OP1_RDHB = 2.asUInt(4.W)
   val OP1_RDHC = 3.asUInt(4.W)
   val OP1_RBHA = 4.asUInt(4.W)
   val OP1_RBHB = 5.asUInt(4.W)
   val OP1_RBHC = 6.asUInt(4.W)
   val OP1_RFHA = 7.asUInt(4.W)
   val OP1_PC   = 15.asUInt(4.W)
   val OP1_X    = 0.asUInt(4.W)

   // Operand 2 Select Signal
   val OP2_RDHC   = 1.asUInt(4.W)
   val OP2_RDHD   = 2.asUInt(4.W)
   val OP2_RBHC   = 3.asUInt(4.W)
   val OP2_RBHD   = 4.asUInt(4.W)
   val OP2_RFHC   = 5.asUInt(4.W)
   val OP2_IMMU6  = 8.asUInt(4.W)  // immu6 source in hd field
   val OP2_IMMU12 = 9.asUInt(4.W)  // immu12 source in hc/hd field
   val OP2_IMMS12 = 10.asUInt(4.W)  // imms12 source in hc/hd field
   val OP2_IMMS18 = 11.asUInt(4.W)  // imms18 source in hb/hc/hd field
   val OP2_WYDE   = 15.asUInt(4.W)
   val OP2_X      = 0.asUInt(4.W)

   // Register File Write Enable Signal
   val REN_0   = false.B
   val REN_1   = true.B
   val REN_X   = false.B

   // ALU Operation Signal
   val ALU_ADD   = 1.asUInt(5.W)
   val ALU_SUB   = 2.asUInt(5.W)
   val ALU_CMPS  = 3.asUInt(5.W)
   val ALU_CMPU  = 4.asUInt(5.W)
   val ALU_MULS  = 5.asUInt(5.W)
   val ALU_MULU  = 6.asUInt(5.W)
   val ALU_DIVS  = 7.asUInt(5.W)
   val ALU_DIVU  = 8.asUInt(5.W)
   val ALU_SLL   = 10.asUInt(5.W)
   val ALU_SRL   = 11.asUInt(5.W)
   val ALU_SRA   = 12.asUInt(5.W)
   val ALU_AND   = 13.asUInt(5.W)
   val ALU_OR    = 14.asUInt(5.W)
   val ALU_XOR   = 15.asUInt(5.W)
   val ALU_XNOR  = 16.asUInt(5.W)
   val ALU_EXTS  = 17.asUInt(5.W)
   val ALU_EXTZ  = 18.asUInt(5.W)
   val ALU_SETOW = 20.asUInt(5.W)
   val ALU_ANDNW = 21.asUInt(5.W)
   val ALU_SETW  = 22.asUInt(5.W)
   val ALU_CSET  = 23.asUInt(5.W)
   val ALU_COPY2 = 24.asUInt(5.W)
   val ALU_ADRP  = 31.asUInt(5.W)
   val ALU_X     = 0.asUInt(5.W)

   // Writeback Select Signal
   val WB_RDHA  = 1.asUInt(4.W)
   val WB_RDHB  = 2.asUInt(4.W)
   val WB_RDHC  = 3.asUInt(4.W)
   val WB_RBHA  = 4.asUInt(4.W)
   val WB_RBHB  = 5.asUInt(4.W)
   val WB_RFHA  = 6.asUInt(4.W)
   val WB_RFHB  = 7.asUInt(4.W)
   val WB_HAHB  = 8.asUInt(4.W)   // two dest regs: rdha + rdhb
   val WB_RDMM  = 9.asUInt(4.W)
   val WB_RBMM  = 10.asUInt(4.W)
   val WB_RA    = 11.asUInt(4.W)
   val WB_CSR   = 15.asUInt(4.W)
   val WB_X     = 0.asUInt(4.W)

   // Memory Function Type (Read,Write,Fence) Signal
   val MWR_R   = 0.asUInt(2.W)
   val MWR_W   = 1.asUInt(2.W)
   val MWR_F   = 2.asUInt(2.W)
   val MWR_X   = 0.asUInt(2.W)

   // Memory Enable Signal
   val MEN_0   = false.B
   val MEN_1   = true.B
   val MEN_X   = false.B

   // Memory Mask Type Signal
   val MSK_B   = 0.asUInt(3.W)
   val MSK_BU  = 1.asUInt(3.W)
   val MSK_H   = 2.asUInt(3.W)
   val MSK_HU  = 3.asUInt(3.W)
   val MSK_W   = 4.asUInt(3.W)
   val MSK_X   = 4.asUInt(3.W)


   // Cache Flushes & Sync Primitives
   val M_N      = 0.asUInt(3.W)
   val M_SI     = 1.asUInt(3.W)   // synch instruction stream
   val M_SD     = 2.asUInt(3.W)   // synch data stream
   val M_FA     = 3.asUInt(3.W)   // flush all caches
   val M_FD     = 4.asUInt(3.W)   // flush data cache

   // Memory Functions (read, write, fence)
   val MT_READ  = 0.asUInt(2.W)
   val MT_WRITE = 1.asUInt(2.W)
   val MT_FENCE = 2.asUInt(2.W)

}

}

