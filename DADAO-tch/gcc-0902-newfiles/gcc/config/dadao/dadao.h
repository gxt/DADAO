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
#define LONG_DOUBLE_TYPE_SIZE		64

#define BIGGEST_ALIGNMENT		64

/* all instruction is 32 bits */
#define FUNCTION_BOUNDARY		LONG_LONG_TYPE_SIZE
/* stack aligned to 64 bits */
#define STACK_BOUNDARY			LONG_LONG_TYPE_SIZE
/* param aligned to 64 bits */
#define PARM_BOUNDARY			LONG_LONG_TYPE_SIZE
/* Alignment of field after `int : 0' in a structure.  */
#define EMPTY_FIELD_BOUNDARY		LONG_LONG_TYPE_SIZE
/* Every structures size must be a multiple of 8 bits.  */
#define STRUCTURE_SIZE_BOUNDARY		LONG_LONG_TYPE_SIZE

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
  /* reg-base */							\
  "null", "rb01", "rb02", "rb03", "rb04", "rb05", "rb06", "rb07",	\
  "rb08", "rb09", "rb10", "rb11", "rb12", "rb13", "rb14", "rb15",	\
  "rb16", "rb17", "rb18", "rb19", "rb20", "rb21", "rb22", "rb23",	\
  "rb24", "rb25", "rb26", "rb27", "rb28", "rb29", "rb30", "rb31",	\
  "rb32", "rb33", "rb34", "rb35", "rb36", "rb37", "rb38", "rb39",	\
  "rb40", "rb41", "rb42", "rb43", "rb44", "rb45", "rb46", "rb47",	\
  "rb48", "rb49", "rb50", "rb51", "rb52", "rb53", "rb54", "rb55",	\
  "rb56", "rb57", "rb58", "rb59", "rb60", "rb61", "rb62", "rb63",	\
  /* reg-return address */						\
  "ra00", /* SHOULD NEVER BE USED DIRECTLY */				\
  /* pseudo registers */						\
  "argp" /* ARG_POINTER_REGNUM */					\
}

#define DADAO_RDnn_ZERO			(0)
#define DADAO_RDnn_V0			(1)
#define DADAO_RDnn_V1			(2)
#define DADAO_RBnn_NULL			(64)
#define DADAO_RBnn_V0			(64 + 1)
#define DADAO_RBnn_V1			(64 + 2)
#define DADAO_RBnn_RSVD			(64 + 56)
#define DADAO_RBnn_FP			(64 + 62)
#define DADAO_RBnn_SP			(64 + 63)
#define DADAO_PSEUDO_ARGP		(64 + 64 + 1)

#define FIXED_REGISTERS {						\
  /* rd    4           8          12          16 */			\
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,			\
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,			\
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,			\
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,			\
  /* rb    4           8          12          16 */			\
  1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,			\
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,			\
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,			\
  0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,			\
  /* others */								\
  1, 1									\
}

#define CALL_USED_REGISTERS {						\
  /* rd    4           8          12          16 */			\
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,			\
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,			\
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,			\
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,			\
  /* rb    4           8          12          16 */			\
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,			\
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,			\
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,			\
  0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1,			\
  /* others */								\
  1, 1									\
}

enum reg_class
{
  NO_REGS,				/* no registers in set */
  DATA_REGS,				/* data registers */
  BASE_REGS,				/* base registers */
  ALL_REGS,				/* all registers */
  LIM_REG_CLASSES			/* max value + 1 */
};
 
#define REG_CLASS_NAMES {		\
  "NO_REGS",				\
  "DATA_REGS",				\
  "BASE_REGS",				\
  "ALL_REGS"				\
}

#define REG_CLASS_CONTENTS {								\
  { 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000 }, /* NO_REGS */		\
  { 0xffffffff, 0xffffffff, 0x00000000, 0x00000000, 0x00000000 }, /* DATA_REGS */	\
  { 0x00000000, 0x00000000, 0xffffffff, 0xffffffff, 0x00000000 }, /* BASE_REGS */	\
  { 0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff, 0x00000003 }  /* ALL_REGS */	\
}

#define N_REG_CLASSES			(int)LIM_REG_CLASSES
#define REGNO_REG_CLASS(R)		( ALL_REGS )

#define BASE_REG_CLASS			BASE_REGS
#define INDEX_REG_CLASS			DATA_REGS
#define MAX_REGS_PER_ADDRESS		2

#define REGNO_OK_FOR_INDEX_P(NUM)	((NUM) < DADAO_RBnn_NULL && (NUM) >= DADAO_RDnn_V0)
#define REGNO_OK_FOR_BASE_P(NUM)	((NUM) < DADAO_RBnn_RSVD && (NUM) >= DADAO_RBnn_V0)

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

/* Function related */
#define STACK_GROWS_DOWNWARD		1
#define FRAME_GROWS_DOWNWARD		1

#define STACK_POINTER_REGNUM		DADAO_RBnn_SP
#define FRAME_POINTER_REGNUM		DADAO_RBnn_FP
#define ARG_POINTER_REGNUM		DADAO_PSEUDO_ARGP

#define ELIMINABLE_REGS {						\
  { ARG_POINTER_REGNUM,			STACK_POINTER_REGNUM},		\
  { ARG_POINTER_REGNUM,			FRAME_POINTER_REGNUM}}

#define FUNCTION_ARG_REGNO_P(r)		dadao_function_arg_regno_p(r)

#define CUMULATIVE_ARGS			unsigned int

#define INITIAL_ELIMINATION_OFFSET(FROM, TO, OFFSET)			\
  (OFFSET) = dadao_initial_elimination_offset ((FROM), (TO))

#define INIT_CUMULATIVE_ARGS(CUM,FNTYPE,LIBNAME,FNDECL,N_NAMED_ARGS)	\
  dadao_init_cumulative_args((CUM))

#define ACCUMULATE_OUTGOING_ARGS			1
#define REG_PARM_STACK_SPACE(FNDECL)			0
#define OUTGOING_REG_PARM_STACK_SPACE(FNTYPE)		1
#define FIRST_PARM_OFFSET(F)				0

#define TRAMPOLINE_SIZE					64
#define TRAMPOLINE_ALIGNMENT				64

/* Profiling NOT support yet */
#define FUNCTION_PROFILER(FILE,LABELNO)			(abort (), 0)

#define EH_RETURN_DATA_REGNO(N)				INVALID_REGNUM
#define EH_RETURN_HANDLER_RTX				gen_rtx_REG(DImode, DADAO_RBnn_RSVD)

/* Run-time Target Specification */
#define TARGET_CPU_CPP_BUILTINS() {			\
  builtin_define_std ("dadao");				\
  builtin_define_std ("DADAO");				\
  builtin_define ("__DADAO_BIG_ENDIAN__");		\
}

#endif /* GCC_DADAO_H */
