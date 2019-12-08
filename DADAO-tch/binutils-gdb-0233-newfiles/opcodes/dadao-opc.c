/*
 * dadao-opc.c -- Definitions for dadao opcodes.
 * Copyright (C) 2019-2033 Guan Xuetao (AT) Peking Univ.
 *
 * Contributed by:
 *   2019:
 *	Guan Xuetao <gxt@pku.edu.cn>
 */

#include "sysdep.h"
#include "opcode/dadao.h"

/* The dadao processor's 16-bit instructions come in two forms:

  FORM 1 instructions start with a 0 bit...

    0oooooooaaaabbbb
    0              F

   ooooooo - form 1 opcode number
   aaaa    - operand A
   bbbb    - operand B

  FORM 2 instructions start with bits "10"...

    10ooaaaavvvvvvvv
    0              F

   oo       - form 2 opcode number
   aaaa     - operand A
   vvvvvvvv - 8-bit immediate value

  FORM 3 instructions start with a bits "11"...

    11oooovvvvvvvvvv
    0              F

   oooo         - form 3 opcode number
   vvvvvvvvvv   - 10-bit immediate value.  */

const dadao_opc_info_t dadao_form1_opc_info[128] =
  {
    { 0x00, DADAO_BAD,     "bad" },  // Reserved as bad.
    { 0x01, DADAO_F1_A4,   "ldi.l" },
    { 0x02, DADAO_F1_AB,   "mov" },
    { 0x03, DADAO_F1_M,    "jsra" },
    { 0x04, DADAO_F1_NARG, "ret" },
    { 0x05, DADAO_F1_AB,   "add" },
    { 0x06, DADAO_F1_AB,   "push" },
    { 0x07, DADAO_F1_AB,   "pop" },
    { 0x08, DADAO_F1_A4,   "lda.l" },
    { 0x09, DADAO_F1_4A,   "sta.l" },
    { 0x0a, DADAO_F1_ABi,  "ld.l" },
    { 0x0b, DADAO_F1_AiB,  "st.l" },
    { 0x0c, DADAO_F1_ABi2, "ldo.l" },
    { 0x0d, DADAO_F1_AiB2, "sto.l" },
    { 0x0e, DADAO_F1_AB,   "cmp" },
    { 0x0f, DADAO_F1_NARG, "nop" },
    { 0x10, DADAO_F1_AB,   "sex.b" },
    { 0x11, DADAO_F1_AB,   "sex.s" },
    { 0x12, DADAO_F1_AB,   "zex.b" },
    { 0x13, DADAO_F1_AB,   "zex.s" },
    { 0x14, DADAO_F1_AB,   "umul.x" },
    { 0x15, DADAO_F1_AB,   "mul.x" },
    { 0x16, DADAO_BAD,     "bad" },
    { 0x17, DADAO_BAD,     "bad" },
    { 0x18, DADAO_BAD,     "bad" },
    { 0x19, DADAO_F1_A,    "jsr" },
    { 0x1a, DADAO_F1_M,    "jmpa" },
    { 0x1b, DADAO_F1_A4,   "ldi.b" },
    { 0x1c, DADAO_F1_ABi,  "ld.b" },
    { 0x1d, DADAO_F1_A4,   "lda.b" },
    { 0x1e, DADAO_F1_AiB,  "st.b" },
    { 0x1f, DADAO_F1_4A,   "sta.b" },
    { 0x20, DADAO_F1_A4,   "ldi.s" },
    { 0x21, DADAO_F1_ABi,  "ld.s" },
    { 0x22, DADAO_F1_A4,   "lda.s" },
    { 0x23, DADAO_F1_AiB,  "st.s" },
    { 0x24, DADAO_F1_4A,   "sta.s" },
    { 0x25, DADAO_F1_A,    "jmp" },
    { 0x26, DADAO_F1_AB,   "and" },
    { 0x27, DADAO_F1_AB,   "lshr" },
    { 0x28, DADAO_F1_AB,   "ashl" },
    { 0x29, DADAO_F1_AB,   "sub" },
    { 0x2a, DADAO_F1_AB,   "neg" },
    { 0x2b, DADAO_F1_AB,   "or" },
    { 0x2c, DADAO_F1_AB,   "not" },
    { 0x2d, DADAO_F1_AB,   "ashr" },
    { 0x2e, DADAO_F1_AB,   "xor" },
    { 0x2f, DADAO_F1_AB,   "mul" },
    { 0x30, DADAO_F1_4,    "swi" },
    { 0x31, DADAO_F1_AB,   "div" },
    { 0x32, DADAO_F1_AB,   "udiv" },
    { 0x33, DADAO_F1_AB,   "mod" },
    { 0x34, DADAO_F1_AB,   "umod" },
    { 0x35, DADAO_F1_NARG, "brk" },
    { 0x36, DADAO_F1_ABi2, "ldo.b" },
    { 0x37, DADAO_F1_AiB2, "sto.b" },
    { 0x38, DADAO_F1_ABi2, "ldo.s" },
    { 0x39, DADAO_F1_AiB2, "sto.s" },
    { 0x3a, DADAO_BAD,     "bad" },
    { 0x3b, DADAO_BAD,     "bad" },
    { 0x3c, DADAO_BAD,     "bad" },
    { 0x3d, DADAO_BAD,     "bad" },
    { 0x3e, DADAO_BAD,     "bad" },
    { 0x3f, DADAO_BAD,     "bad" },
    { 0x40, DADAO_BAD,     "bad" },
    { 0x41, DADAO_BAD,     "bad" },
    { 0x42, DADAO_BAD,     "bad" },
    { 0x43, DADAO_BAD,     "bad" },
    { 0x44, DADAO_BAD,     "bad" },
    { 0x45, DADAO_BAD,     "bad" },
    { 0x46, DADAO_BAD,     "bad" },
    { 0x47, DADAO_BAD,     "bad" },
    { 0x48, DADAO_BAD,     "bad" },
    { 0x49, DADAO_BAD,     "bad" },
    { 0x4a, DADAO_BAD,     "bad" },
    { 0x4b, DADAO_BAD,     "bad" },
    { 0x4c, DADAO_BAD,     "bad" },
    { 0x4d, DADAO_BAD,     "bad" },
    { 0x4e, DADAO_BAD,     "bad" },
    { 0x4f, DADAO_BAD,     "bad" },
    { 0x50, DADAO_BAD,     "bad" },
    { 0x51, DADAO_BAD,     "bad" },
    { 0x52, DADAO_BAD,     "bad" },
    { 0x53, DADAO_BAD,     "bad" },
    { 0x54, DADAO_BAD,     "bad" },
    { 0x55, DADAO_BAD,     "bad" },
    { 0x56, DADAO_BAD,     "bad" },
    { 0x57, DADAO_BAD,     "bad" },
    { 0x58, DADAO_BAD,     "bad" },
    { 0x59, DADAO_BAD,     "bad" },
    { 0x5a, DADAO_BAD,     "bad" },
    { 0x5b, DADAO_BAD,     "bad" },
    { 0x5c, DADAO_BAD,     "bad" },
    { 0x5d, DADAO_BAD,     "bad" },
    { 0x5e, DADAO_BAD,     "bad" },
    { 0x5f, DADAO_BAD,     "bad" },
    { 0x60, DADAO_BAD,     "bad" },
    { 0x61, DADAO_BAD,     "bad" },
    { 0x62, DADAO_BAD,     "bad" },
    { 0x63, DADAO_BAD,     "bad" },
    { 0x64, DADAO_BAD,     "bad" },
    { 0x65, DADAO_BAD,     "bad" },
    { 0x66, DADAO_BAD,     "bad" },
    { 0x67, DADAO_BAD,     "bad" },
    { 0x68, DADAO_BAD,     "bad" },
    { 0x69, DADAO_BAD,     "bad" },
    { 0x6a, DADAO_BAD,     "bad" },
    { 0x6b, DADAO_BAD,     "bad" },
    { 0x6c, DADAO_BAD,     "bad" },
    { 0x6d, DADAO_BAD,     "bad" },
    { 0x6e, DADAO_BAD,     "bad" },
    { 0x6f, DADAO_BAD,     "bad" },
    { 0x70, DADAO_BAD,     "bad" },
    { 0x71, DADAO_BAD,     "bad" },
    { 0x72, DADAO_BAD,     "bad" },
    { 0x73, DADAO_BAD,     "bad" },
    { 0x74, DADAO_BAD,     "bad" },
    { 0x75, DADAO_BAD,     "bad" },
    { 0x76, DADAO_BAD,     "bad" },
    { 0x77, DADAO_BAD,     "bad" },
    { 0x78, DADAO_BAD,     "bad" },
    { 0x79, DADAO_BAD,     "bad" },
    { 0x7a, DADAO_BAD,     "bad" },
    { 0x7b, DADAO_BAD,     "bad" },
    { 0x7c, DADAO_BAD,     "bad" },
    { 0x7d, DADAO_BAD,     "bad" },
    { 0x7e, DADAO_BAD,     "bad" },
    { 0x7f, DADAO_BAD,     "bad" }
  };

const dadao_opc_info_t dadao_form2_opc_info[4] =
  {
    { 0x00, DADAO_F2_A8V,  "inc" },
    { 0x01, DADAO_F2_A8V,  "dec" },
    { 0x02, DADAO_F2_A8V,  "gsr" },
    { 0x03, DADAO_F2_A8V,  "ssr" }
  };

const dadao_opc_info_t dadao_form3_opc_info[16] =
  {
    { 0x00, DADAO_F3_PCREL,"beq" },
    { 0x01, DADAO_F3_PCREL,"bne" },
    { 0x02, DADAO_F3_PCREL,"blt" },
    { 0x03, DADAO_F3_PCREL,"bgt" },
    { 0x04, DADAO_F3_PCREL,"bltu" },
    { 0x05, DADAO_F3_PCREL,"bgtu" },
    { 0x06, DADAO_F3_PCREL,"bge" },
    { 0x07, DADAO_F3_PCREL,"ble" },
    { 0x08, DADAO_F3_PCREL,"bgeu" },
    { 0x09, DADAO_F3_PCREL,"bleu" },
    { 0x0a, DADAO_BAD,     "bad" },
    { 0x0b, DADAO_BAD,     "bad" },
    { 0x0c, DADAO_BAD,     "bad" },
    { 0x0d, DADAO_BAD,     "bad" },
    { 0x0e, DADAO_BAD,     "bad" },
    { 0x0f, DADAO_BAD,     "bad" }  // Reserved as bad.
  };
