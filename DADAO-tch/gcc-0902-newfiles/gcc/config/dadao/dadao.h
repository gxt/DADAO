/* Target Definitions for DADAO. */
#ifndef GCC_DADAO_H
#define GCC_DADAO_H

#define BITS_BIG_ENDIAN			0
#define BYTES_BIG_ENDIAN		1
#define WORDS_BIG_ENDIAN		1

#define CHAR_TYPE_SIZE			8
#define SHORT_TYPE_SIZE			16
#define INT_TYPE_SIZE			32
#define LONG_TYPE_SIZE			64
#define LONG_LONG_TYPE_SIZE		64

#define FLOAT_TYPE_SIZE			32
#define DOUBLE_TYPE_SIZE		64

#define BIGGEST_ALIGNMENT		64

/* all instruction is 32 bits */
#define FUNCTION_BOUNDARY		32
/* stack aligned to 64 bits */
#define STACK_BOUNDARY			64
/* param aligned to 64 bits */
#define PARM_BOUNDARY			64

#define UNITS_PER_WORD			8
#define MOVE_MAX			8

#define PCC_BITFIELD_TYPE_MATTERS	1
#define STRICT_ALIGNMENT		1
#define SLOW_BYTE_ACCESS		1
#define DEFAULT_SIGNED_CHAR		0

#define FUNCTION_MODE			SImode
#define CASE_VECTOR_MODE		DImode
#define Pmode				DImode

/* register desciption */
#define FIRST_PSEUDO_REGISTER		(64 * 2)

#define REGISTER_NAMES {						\
  /* reg-data */							\
  "zero", "rd01", "rd02", "rd03", "rd04", "rd05", "rd06", "rd07",	\
  "rd08", "rd09", "rd10", "rd11", "rd12", "rd13", "rd14", "rd15",	\
  "rd16", "rd17", "rd18", "rd19", "rd20", "rd21", "rd22", "rd23",	\
  "rd24", "rd25", "rd26", "rd27", "rd28", "rd29", "rd30", "rd31",	\
  "rd32", "rd33", "rd34", "rd35", "rd36", "rd37", "rd38", "rd39",	\
  "rd40", "rd41", "rd42", "rd43", "rd44", "rd45", "rd46", "rd47",	\
  "rd48", "rd49", "rd50", "rd51", "rd52", "rd53", "rd54", "rd55",	\
  "rd56", "rd57", "rd58", "rd59", "rd60", "rd61", "rd62", "rd63",	\
  /* reg-pointer */							\
  "null", "rp01", "rp02", "rp03", "rp04", "rp05", "rp06", "rp07",	\
  "rp08", "rp09", "rp10", "rp11", "rp12", "rp13", "rp14", "rp15",	\
  "rp16", "rp17", "rp18", "rp19", "rp20", "rp21", "rp22", "rp23",	\
  "rp24", "rp25", "rp26", "rp27", "rp28", "rp29", "rp30", "rp31",	\
  "rp32", "rp33", "rp34", "rp35", "rp36", "rp37", "rp38", "rp39",	\
  "rp40", "rp41", "rp42", "rp43", "rp44", "rp45", "rp46", "rp47",	\
  "rp48", "rp49", "rp50", "rp51", "rp52", "rp53", "rp54", "rp55",	\
  "rp56", "rp57", "rp58", "rp59", "rp60", "rp61", "rp62", "rp63",	\
  /* pseudo registers */						\
  "ra00" /* SHOULD NEVER BE USED DIRECTLY */				\
}

#define FIXED_REGISTERS {						\
  /* rd    4           8          12          16 */			\
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,			\
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,			\
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,			\
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,			\
  /* rp    4           8          12          16 */			\
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,			\
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,			\
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,			\
  0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,			\
  /* others */								\
  1									\
}

#define CALL_USED_REGISTERS {						\
  /* rd    4           8          12          16 */			\
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,			\
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,			\
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,			\
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,			\
  /* rp    4           8          12          16 */			\
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,			\
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,			\
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,			\
  0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,			\
  /* others */								\
  1									\
}

enum reg_class
{
  NO_REGS,				/* no registers in set */
  DATA_REGS,				/* data registers */
  POINTER_REGS,				/* pointer registers */
  ALL_REGS,				/* all registers */
  LIM_REG_CLASSES			/* max value + 1 */
}
 
#define REG_CLASS_NAMES {		\
  "NO_REGS",				\
  "DATA_REGS",				\
  "POINTER_REGS",			\
  "ALL_REGS"				\
}

#define REG_CLASS_CONTENTS {								\
  { 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000 }, /* NO_REGS */		\
  { 0xffffffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000 }, /* DATA_REGS */	\
  { 0x00000000, 0x00000000, 0xffffffff, 0xffffffff, 0x00000000 }, /* POINTER_REGS */	\
  { 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0x00000001 }  /* ALL_REGS */	\
}

#define N_REG_CLASSES			(int)LIM_REG_CLASSES
#define REGNO_REG_CLASS(R)		( ALL_REGS )

/* Assemble related */
#define ASM_COMMENT_START		"#"
#define ASM_APP_ON			" #APP\n"
#define ASM_APP_OFF			" #NO_APP\n"

#define GLOBAL_ASM_OP			"\t.global\t"

#define TEXT_SECTION_ASM_OP		"\t.text"
#define DATA_SECTION_ASM_OP		"\t.data"
#define BSS_SECTION_ASM_OP		"\t.bss"
#define READONLY_DATA_SECTION_ASM_OP	"\t.section\t.rodata"
#define SBSS_SECTION_ASM_OP		"\t.section\t.sbss,\"aw\",@nobits"
#define SDATA_SECTION_ASM_OP		"\t.section\t.sdata,\"aw\",@progbits"

#define ASM_OUTPUT_ALIGN(STREAM,LOG)	fprintf (STREAM, "\t.align\t%d\n", (LOG))

#endif /* GCC_DADAO_H */
