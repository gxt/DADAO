//**************************************************************************
// RISCV Processor Constants
//--------------------------------------------------------------------------
//
// Christopher Celio
// 2011 Feb 1

package wuming.stage5
package constants
{

import chisel3._
import chisel3.util._


trait WumingProcConstants
{
   // for debugging, print out the commit information.
   // can be compared against the riscv-isa-run Spike ISA simulator's commit logger.
   val PRINT_COMMIT_LOG = false

   //************************************
   // Machine Parameters
   val USE_FULL_BYPASSING = true  // turn on full bypassing (only stalls
                                  // on load-use). Otherwise rely
                                  // entirely on interlocking to handle
                                  // pipeline hazards.
}

trait ScalarOpConstants
{

   //************************************
   // Control Signals
   val Y        = true.B
   val N        = false.B

   // PC Select Signal
   val PC_4     = 0.asUInt(2.W)  // PC + 4
   val PC_BRJMP = 1.asUInt(2.W)  // brjmp_target
   val PC_JALR  = 2.asUInt(2.W)  // jump_reg_target
   val PC_EXC   = 3.asUInt(2.W)  // exception

   // SimRISC PC Select Signal
   val S_PC_4     = 0.asUInt(3.W)  // PC + 4
   val S_PC_EXCP  = 1.asUInt(3.W)  // exception
   val S_PC_BR12  = 2.asUInt(3.W)  // branch_target for imms12
   val S_PC_BR18  = 3.asUInt(3.W)  // branch_target for imms18
   val S_PC_IIII  = 4.asUInt(3.W)  // jump/call iiii target
   val S_PC_RRII  = 5.asUInt(3.W)  // jump/call rrii target
   val S_PC_RASP  = 6.asUInt(3.W)  // pop from RA regfile

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

   // Branch Type
   val BR_N     = 0.asUInt(4.W)  // Next
   val BR_NE    = 1.asUInt(4.W)  // Branch on NotEqual
   val BR_EQ    = 2.asUInt(4.W)  // Branch on Equal
   val BR_GE    = 3.asUInt(4.W)  // Branch on Greater/Equal
   val BR_GEU   = 4.asUInt(4.W)  // Branch on Greater/Equal Unsigned
   val BR_LT    = 5.asUInt(4.W)  // Branch on Less Than
   val BR_LTU   = 6.asUInt(4.W)  // Branch on Less Than Unsigned
   val BR_J     = 7.asUInt(4.W)  // Jump
   val BR_JR    = 8.asUInt(4.W)  // Jump Register

   // RS1 Operand Select Signal
   val OP1_RS1   = 0.asUInt(2.W) // Register Source #1
   val OP1_PC    = 1.asUInt(2.W) // PC
   val OP1_IMZ   = 2.asUInt(2.W) // Zero-extended Immediate from RS1 field, for use by CSRI instructions
   val OP1_X     = 0.asUInt(2.W)
   
   // SimRISC Operand 1 Select Signal
   val S_OP1_RDHA = 1.asUInt(4.W)
   val S_OP1_RDHB = 2.asUInt(4.W)
   val S_OP1_RDHC = 3.asUInt(4.W)
   val S_OP1_RBHA = 4.asUInt(4.W)
   val S_OP1_RBHB = 5.asUInt(4.W)
   val S_OP1_RBHC = 6.asUInt(4.W)
   val S_OP1_RFHA = 7.asUInt(4.W)
   val S_OP1_PC   = 15.asUInt(4.W)
   val S_OP1_X    = 0.asUInt(4.W)

   // RS2 Operand Select Signal
   val OP2_RS2    = 0.asUInt(3.W) // Register Source #2
   val OP2_ITYPE  = 1.asUInt(3.W) // immediate, I-type
   val OP2_STYPE  = 2.asUInt(3.W) // immediate, S-type
   val OP2_SBTYPE = 3.asUInt(3.W) // immediate, B
   val OP2_UTYPE  = 4.asUInt(3.W) // immediate, U-type
   val OP2_UJTYPE = 5.asUInt(3.W) // immediate, J-type
   val OP2_X      = 0.asUInt(3.W)

   // SimRISC Operand 2 Select Signal
   val S_OP2_RDHC   = 1.asUInt(4.W)
   val S_OP2_RDHD   = 2.asUInt(4.W)
   val S_OP2_RBHC   = 3.asUInt(4.W)
   val S_OP2_RBHD   = 4.asUInt(4.W)
   val S_OP2_RFHC   = 5.asUInt(4.W)
   val S_OP2_IMMU6  = 8.asUInt(4.W)  // immu6 source in hd field
   val S_OP2_IMMU12 = 9.asUInt(4.W)  // immu12 source in hc/hd field
   val S_OP2_IMMS12 = 10.asUInt(4.W)  // imms12 source in hc/hd field
   val S_OP2_IMMS18 = 11.asUInt(4.W)  // imms18 source in hb/hc/hd field
   val S_OP2_WYDE   = 15.asUInt(4.W)
   val S_OP2_X      = 0.asUInt(4.W)

   // Register Operand Output Enable Signal
   val OEN_0   = false.B
   val OEN_1   = true.B

   // Register File Write Enable Signal
   val REN_0   = false.B
   val REN_1   = true.B
   val REN_X   = false.B

   // ALU Operation Signal
   val ALU_ADD    = 0.asUInt(4.W)
   val ALU_SUB    = 1.asUInt(4.W)
   val ALU_SLL    = 2.asUInt(4.W)
   val ALU_SRL    = 3.asUInt(4.W)
   val ALU_SRA    = 4.asUInt(4.W)
   val ALU_AND    = 5.asUInt(4.W)
   val ALU_OR     = 6.asUInt(4.W)
   val ALU_XOR    = 7.asUInt(4.W)
   val ALU_SLT    = 8.asUInt(4.W)
   val ALU_SLTU   = 9.asUInt(4.W)
   val ALU_COPY_1 = 10.asUInt(4.W)
   val ALU_COPY_2 = 11.asUInt(4.W)
   val ALU_X      = 0.asUInt(4.W)

   // SimRISC ALU Operation Signal
   val S_ALU_ADD   = 1.asUInt(5.W)
   val S_ALU_SUB   = 2.asUInt(5.W)
   val S_ALU_CMPS  = 3.asUInt(5.W)
   val S_ALU_CMPU  = 4.asUInt(5.W)
   val S_ALU_MULS  = 5.asUInt(5.W)
   val S_ALU_MULU  = 6.asUInt(5.W)
   val S_ALU_DIVS  = 7.asUInt(5.W)
   val S_ALU_DIVU  = 8.asUInt(5.W)
   val S_ALU_SLL   = 10.asUInt(5.W)
   val S_ALU_SRL   = 11.asUInt(5.W)
   val S_ALU_SRA   = 12.asUInt(5.W)
   val S_ALU_AND   = 13.asUInt(5.W)
   val S_ALU_OR    = 14.asUInt(5.W)
   val S_ALU_XOR   = 15.asUInt(5.W)
   val S_ALU_XNOR  = 16.asUInt(5.W)
   val S_ALU_EXTS  = 17.asUInt(5.W)
   val S_ALU_EXTZ  = 18.asUInt(5.W)
   val S_ALU_SETOW = 20.asUInt(5.W)
   val S_ALU_ANDNW = 21.asUInt(5.W)
   val S_ALU_SETW  = 22.asUInt(5.W)
   val S_ALU_CSET  = 23.asUInt(5.W)
   val S_ALU_COPY2 = 24.asUInt(5.W)
   val S_ALU_ADRP  = 31.asUInt(5.W)
   val S_ALU_X     = 0.asUInt(5.W)

   // Writeback Select Signal
   val WB_ALU  = 0.asUInt(2.W)
   val WB_MEM  = 1.asUInt(2.W)
   val WB_PC4  = 2.asUInt(2.W)
   val WB_CSR  = 3.asUInt(2.W)
   val WB_X    = 0.asUInt(2.W)
   
   // SimRISC Writeback Select Signal
   val S_WB_RDHA  = 1.asUInt(4.W)
   val S_WB_RDHB  = 2.asUInt(4.W)
   val S_WB_RDHC  = 3.asUInt(4.W)
   val S_WB_RBHA  = 4.asUInt(4.W)
   val S_WB_RBHB  = 5.asUInt(4.W)
   val S_WB_RFHA  = 6.asUInt(4.W)
   val S_WB_RFHB  = 7.asUInt(4.W)
   val S_WB_HAHB  = 8.asUInt(4.W)   // two dest regs: rdha + rdhb
   val S_WB_RDMM  = 9.asUInt(4.W)
   val S_WB_RBMM  = 10.asUInt(4.W)
   val S_WB_RA    = 11.asUInt(4.W)
   val S_WB_CSR   = 15.asUInt(4.W)
   val S_WB_X     = 0.asUInt(4.W)

   val RFX     = 0.asUInt(3.W)
   val RFA     = 1.asUInt(3.W)
   val RFD     = 2.asUInt(3.W)
   val RFB     = 3.asUInt(3.W)
   val RFF     = 4.asUInt(3.W)
   val RFM     = 5.asUInt(3.W)
   val RFX2    = 6.asUInt(3.W)

   // Memory Write Signal
   val MWR_0   = false.B
   val MWR_1   = true.B
   val MWR_X   = false.B

   // SimRISC Memory Function Type (Read,Write,Fence) Signal
   val S_MWR_R   = 0.asUInt(2.W)
   val S_MWR_W   = 1.asUInt(2.W)
   val S_MWR_F   = 2.asUInt(2.W)
   val S_MWR_X   = 0.asUInt(2.W)

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

   // RISCV related, SHOULD removed after dev
   val RD_MSB  = 11
   val RD_LSB  = 7
   val RS1_MSB = 19
   val RS1_LSB = 15
   val RS2_MSB = 24
   val RS2_LSB = 20

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

