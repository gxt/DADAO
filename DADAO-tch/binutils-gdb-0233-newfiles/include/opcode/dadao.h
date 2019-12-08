/*
 * Definitions for decoding the dadao opcode table.
 * Copyright (C) 2019-2033 Guan Xuetao (AT) Peking Univ.
 *
 * Contributed by:
 *   2019:
 *	Guan Xuetao <gxt@pku.edu.cn>
 */

/* Form 1 instructions come in different flavors:

    Some have no arguments                          (DADAO_F1_NARG)
    Some only use the A operand                     (DADAO_F1_A)
    Some use A and B registers                      (DADAO_F1_AB)
    Some use A and consume a 4 byte immediate value (DADAO_F1_A4)
    Some use just a 4 byte immediate value          (DADAO_F1_4)
    Some use just a 4 byte memory address           (DADAO_F1_M)
    Some use B and an indirect A                    (DADAO_F1_AiB)
    Some use A and an indirect B                    (DADAO_F1_ABi)
    Some consume a 4 byte immediate value and use X (DADAO_F1_4A)
    Some use B and an indirect A plus 2 byte offset (DADAO_F1_AiB2)
    Some use A and an indirect B plus 2 byte offset (DADAO_F1_ABi2)

  Form 2 instructions also come in different flavors:

    Some have no arguments                          (DADAO_F2_NARG)
    Some use the A register and an 8-bit value      (DADAO_F2_A8V)

  Form 3 instructions also come in different flavors:

    Some have no arguments                          (DADAO_F3_NARG)
    Some have a 10-bit PC relative operand          (DADAO_F3_PCREL).  */

#define DADAO_F1_NARG 0x100
#define DADAO_F1_A    0x101
#define DADAO_F1_AB   0x102
/* #define DADAO_F1_ABC  0x103 */
#define DADAO_F1_A4   0x104
#define DADAO_F1_4    0x105
#define DADAO_F1_AiB  0x106
#define DADAO_F1_ABi  0x107
#define DADAO_F1_4A   0x108
#define DADAO_F1_AiB2 0x109
#define DADAO_F1_ABi2 0x10a
#define DADAO_F1_M    0x10b

#define DADAO_F2_NARG 0x200
#define DADAO_F2_A8V  0x201

#define DADAO_F3_NARG  0x300
#define DADAO_F3_PCREL 0x301

#define DADAO_BAD     0x400

typedef struct dadao_opc_info_t
{
  short         opcode;
  unsigned      itype;
  const char *  name;
} dadao_opc_info_t;

extern const dadao_opc_info_t dadao_form1_opc_info[128];
extern const dadao_opc_info_t dadao_form2_opc_info[4];
extern const dadao_opc_info_t dadao_form3_opc_info[16];
