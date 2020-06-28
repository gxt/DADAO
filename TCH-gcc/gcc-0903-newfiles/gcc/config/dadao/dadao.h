/*
 * Definitions of target machine for GNU compiler, for DADAO.
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */
#ifndef GCC_DADAO_H
#define GCC_DADAO_H

#ifndef	OBJECT_FORMAT_ELF
#error	DADAO ONLY SUPPORT ELF FORMAT!
#endif

#define	DADAO_REG_RG_START			0x00
#define	DADAO_REG_RP_START			0x40
#define	DADAO_REG_RF_START			0x80
#define	DADAO_REG_RV_START			0xC0
#define	DADAO_REG_RS_START			0x100

#define	DADAO_MAX_ARGS_IN_REGS			16
#define	DADAO_EH_RETURN_DATA_REGNUM		(DADAO_REG_RG_START + 0x04)
#define	DADAO_FIRST_ARG_REGNUM			(DADAO_REG_RG_START + 0x10)
#define	DADAO_RETURN_VALUE_REGNUM		(DADAO_REG_RG_START + 0x1F)

/* FIXME: This one isn't fully implemented yet.  Return values larger than
   one register are passed by reference in DADAO_STRUCT_VALUE_REGNUM by the
   caller, except for return values of type "complex".  */
#define	DADAO_PROGRAM_COUNTER_REGNUM		(DADAO_REG_RP_START + 0x00)
#define	DADAO_STACK_POINTER_REGNUM		(DADAO_REG_RP_START + 0x01)
#define	DADAO_FRAME_POINTER_REGNUM		(DADAO_REG_RP_START + 0x02)
#define DADAO_ARG_POINTER_REGNUM		(DADAO_REG_RP_START + 0x03)
#define	DADAO_EH_RETURN_HANDLER_REGNUM		(DADAO_REG_RP_START + 0x04)
#define	DADAO_RETURN_ADDRESS_REGNUM		(DADAO_REG_RP_START + 0x3E)
#define	DADAO_STRUCT_VALUE_REGNUM		(DADAO_REG_RP_START + 0x3F)

#define DADAO_rO_REGNUM				(DADAO_REG_RS_START + 6)

#define DADAO_FUNCTION_ARG_SIZE(MODE, TYPE) \
 ((MODE) != BLKmode ? GET_MODE_SIZE (MODE) : int_size_in_bytes (TYPE))

/* Per-function machine data.  This is normally an opaque type just
   defined and used in the tm.c file, but we need to see the definition in
   dadao.md too.  */
struct GTY(()) machine_function
 {
   int has_landing_pad;
   int in_prologue;
 };

/* For these target macros, there is no generic documentation here.  You
   should read `Using and Porting GCC' for that.  Only comments specific
   to the DADAO target are here.

   There are however references to the specific texinfo node (comments
   with "Node:"), so there should be little or nothing amiss.  Probably
   the opposite, since we don't have to care about old littering and
   soon outdated generic comments.  */

/* XXX gccint Chapter 18: Target Description Macros and Functions */

/* XXX gccint 18.1 Node: The Global targetm Variable */
/* (empty) */

/* XXX gccint 18.2 Node: Controlling the Compilation Driver, gcc */

/* User symbols are in the same name-space as built-in symbols, but we
   don't need the built-in symbols, so remove those and instead apply
   stricter operand checking.  Don't warn when expanding insns.  */
#define ASM_SPEC			"-x"

/* Pass on -mset-program-start=N and -mset-data-start=M to the linker.
   Provide default program start 0x400000 unless -mno-set-program-start.
   Don't do this if linking relocatably, with -r.  For a final link,
   produce mmo, unless ELF is requested or when linking relocatably.  */
#define LINK_SPEC						\
 "%{mset-program-start=*:--defsym __.DADAO.start..text=%*}	\
  %{mset-data-start=*:--defsym __.DADAO.start..data=%*}		\
  %{!mset-program-start=*:					\
    %{!mno-set-program-start:					\
     %{!r:--defsym __.DADAO.start..text=0x400000}}}		\
  %{melf|r:-m elf64_dadao}"

/* FIXME: There's no provision for profiling here.  */
#define STARTFILE_SPEC			"crti%O%s crtbegin%O%s"
#define ENDFILE_SPEC			"crtend%O%s crtn%O%s"

/* XXX gccint 18.3 Node: Run-time Target Specification */

#define TARGET_CPU_CPP_BUILTINS()				\
	do {							\
		builtin_define ("__dadao__");			\
		builtin_define ("__DADAO__");			\
	} while (0)

/* XXX gccint 18.4 Node: Defining data structures for per-function information */
#define INIT_EXPANDERS			dadao_init_expanders ()

/* XXX gccint 18.5 Node: Storage Layout */
#define	BITS_BIG_ENDIAN			0
#define	BYTES_BIG_ENDIAN		1
#define	WORDS_BIG_ENDIAN		1
#define	FLOAT_WORDS_BIG_ENDIAN		1
#define	UNITS_PER_WORD			8

#define	PARM_BOUNDARY			64
#define	STACK_BOUNDARY			64
#define	FUNCTION_BOUNDARY		32
#define	BIGGEST_ALIGNMENT		64

#define	STRICT_ALIGNMENT		1

/* XXX gccint 18.6 Node: Layout of Source Language Data Types */
#define	INT_TYPE_SIZE			32
#define	LONG_LONG_TYPE_SIZE		64

#define	FLOAT_TYPE_SIZE			32
#define	DOUBLE_TYPE_SIZE		64
#define	LONG_DOUBLE_TYPE_SIZE		64

#define	DEFAULT_SIGNED_CHAR		1

/* XXX gccint 18.7 Node: Register Usage */

/* XXX gccint 18.7.1 Node: Basic Characteristics of Registers */
#define	FIRST_PSEUDO_REGISTER		(64 * 5)

#define FIXED_REGISTERS {				\
	/* 0 ~ 63 */					\
	1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, \
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
	/* 64 ~ 127 */					\
	1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, \
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
	/* 128 ~ 191 */					\
	1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, \
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
	/* 192 ~ 255 */					\
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	/* 256 ~ 319 */					\
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
}

#define CALL_USED_REGISTERS {				\
	/* 0 ~ 63 */					\
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	\
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	\
	/* 64 ~ 127 */					\
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	\
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	\
	/* 128 ~ 191 */					\
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	\
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	\
	/* 192 ~ 255 */					\
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	/* 256 ~ 319 */					\
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
}

#define	PC_REGNUM				DADAO_PROGRAM_COUNTER_REGNUM

/* XXX gccint 18.7.2 Node: Order of Allocation of Registers */
/* (empty) */

/* XXX gccint 18.7.3 Node: How Values Fit in Registers */
/* (empty) */

/* XXX gccint 18.7.4 Node: Handling Leaf Functions */
/* (empty) */

/* XXX gccint 18.7.5 Node: Registers That Form a Stack */
/* (empty) */

/* XXX gccint 18.8 Node: Register Classes */

enum reg_class {
	NO_REGS,
	GENERAL_REGS,
	POINTER_REGS,
	FLOATING_REGS,
	VECTOR_REGS,
	SPECIAL_REGS,
	ALL_REGS,
	LIM_REG_CLASSES };

#define N_REG_CLASSES			((int) LIM_REG_CLASSES)

#define REG_CLASS_NAMES	{			\
	"NO_REGS",				\
	"GENERAL_REGS",				\
	"POINTER_REGS",				\
	"FLOATING_REGS",			\
	"VECTOR_REGS",				\
	"SPECIAL_REGS",				\
	"ALL_REGS"}

/* Note that the contents of each item is always 32 bits.  */
#define REG_CLASS_CONTENTS {								\
	/* Rg	Rg	Rp	Rp	Rf	Rf	Rv	Rv	Rs	Rs */	\
	{ 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0},	\
	{~0,	~0,	 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0},	\
	{ 0,	 0,	~0,	~0,	 0,	 0,	 0,	 0,	 0,	 0},	\
	{ 0,	 0,	 0,	 0,	~0,	~0,	 0,	 0,	 0,	 0},	\
	{ 0,	 0,	 0,	 0,	 0,	 0,	~0,	~0,	 0,	 0},	\
	{ 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0,	~0,	~0},	\
	{~0,	~0,	~0,	~0,	~0,	~0,	~0,	~0,	~0,	~0}}

#define REGNO_REG_CLASS(REGNO)							\
	( (REGNO) >= FIRST_PSEUDO_REGISTER	? NO_REGS			\
	: (REGNO) < 0x40			? GENERAL_REGS			\
	: (REGNO) < 0x80			? POINTER_REGS			\
	: (REGNO) < 0xC0			? FLOATING_REGS			\
	: (REGNO) < 0x100			? VECTOR_REGS : SPECIAL_REGS)

#define BASE_REG_CLASS				POINTER_REGS
#define INDEX_REG_CLASS				GENERAL_REGS

#define REGNO_OK_FOR_BASE_P(REGNO)		((REGNO_REG_CLASS(REGNO) == POINTER_REGS)	\
		|| (reg_renumber && (REGNO_REG_CLASS(reg_renumber[REGNO]) == POINTER_REGS)))
#define REGNO_OK_FOR_INDEX_P(REGNO)		((REGNO_REG_CLASS(REGNO) == GENERAL_REGS)	\
		|| (reg_renumber && (REGNO_REG_CLASS(reg_renumber[REGNO]) == GENERAL_REGS)))

#define CLASS_MAX_NREGS(CLASS, MODE)		targetm.hard_regno_nregs (CLASS, MODE)

/* XXX gccint 18.9 Node: Stack Layout and Calling Conventions */

/* XXX gccint 18.9.1 Node: Basic Stack Layout */
#define STACK_GROWS_DOWNWARD			1
#define FRAME_GROWS_DOWNWARD			1

#define FIRST_PARM_OFFSET(FUNDECL)		0

#define DYNAMIC_CHAIN_ADDRESS(FRAMEADDR)	dadao_dynamic_chain_address (FRAMEADDR)

#define SETUP_FRAME_ADDRESSES()			dadao_setup_frame_addresses ()

#define RETURN_ADDR_RTX(COUNT, FRAME)		dadao_return_addr_rtx (COUNT, FRAME)

#define INCOMING_RETURN_ADDR_RTX		gen_rtx_REG (Pmode, DADAO_RETURN_ADDRESS_REGNUM)

/* XXX gccint 18.9.2 Node: Exception Handling Support */
#define	EH_RETURN_DATA_REGNO(N)			(((N) < 4) ? (DADAO_EH_RETURN_DATA_REGNUM + (N)) : INVALID_REGNUM)
#define	EH_RETURN_HANDLER_RTX			gen_rtx_REG (Pmode, DADAO_EH_RETURN_HANDLER_REGNUM)

/* XXX gccint 18.9.3 Node: Specifying How Stack Checking is Done */
/* (empty) */

/* XXX gccint 18.9.4 Node: Registers That Address the Stack Frame */
#define	STACK_POINTER_REGNUM			DADAO_STACK_POINTER_REGNUM
#define	FRAME_POINTER_REGNUM			DADAO_FRAME_POINTER_REGNUM
#define	ARG_POINTER_REGNUM			DADAO_ARG_POINTER_REGNUM

/* XXX gccint 18.9.5 Node: Eliminating Frame Pointer and Arg Pointer */

/* The frame-pointer is stored in a location that either counts to the
   offset of incoming parameters, or that counts to the offset of the
   frame, so we can't use a single offset.  We therefore eliminate those
   two separately.  */
#define ELIMINABLE_REGS {					\
		{ARG_POINTER_REGNUM,	STACK_POINTER_REGNUM},	\
		{ARG_POINTER_REGNUM,	FRAME_POINTER_REGNUM},	\
		{FRAME_POINTER_REGNUM,	STACK_POINTER_REGNUM}}

#define INITIAL_ELIMINATION_OFFSET(FROM, TO, OFFSET)		\
		(OFFSET) = dadao_initial_elimination_offset (FROM, TO)

/* XXX gccint 18.9.6 Node: Passing Function Arguments on the Stack */

#define ACCUMULATE_OUTGOING_ARGS		1

/* XXX gccint 18.9.7 Node: Passing Arguments in Registers */

typedef struct { int regs; int lib; }		CUMULATIVE_ARGS;

#define INIT_CUMULATIVE_ARGS(CUM, FNTYPE, LIBNAME, INDIRECT, N_NAMED_ARGS)	\
	((CUM).regs = 0, (CUM).lib = ((LIBNAME) != 0))

#define FUNCTION_ARG_REGNO_P(REGNO)						\
	(((REGNO) >= (DADAO_FIRST_ARG_REGNUM)) &&				\
	 ((REGNO) < ((DADAO_FIRST_ARG_REGNUM) + (DADAO_MAX_ARGS_IN_REGS))))

/* XXX gccint 18.9.8 Node: How Scalar Function Values Are Returned */
/* (empty) */

/* XXX gccint 18.9.9 Node: How Large Values Are Returned */
/* (empty) */

/* XXX gccint 18.9.10 Node: Caller-Saves Register Allocation */
/* (empty) */

/* XXX gccint 18.9.11 Node: Function Entry and Exit */

/* We need to say that the epilogue uses the return address, so the
   initial-value machinery restores it.  FIXME: Some targets
   conditionalize on "reload_completed &&".  Investigate difference.
   FIXME: Not needed if nonlocal_goto_stack_level.  */
#define EPILOGUE_USES(REGNO)			((REGNO) == DADAO_RETURN_ADDRESS_REGNUM)

/* XXX gccint 18.9.12 Node: Generating Code for Profiling */

#define FUNCTION_PROFILER(FILE, LABELNO)	dadao_function_profiler (FILE, LABELNO)

/* XXX gccint 18.9.13 Node: Permitting tail calls */
/* (empty) */

/* XXX gccint 18.9.14 Node: Shrink-wrapping separate components */
/* (empty) */

/* XXX gccint 18.9.15 Node: Stack smashing protection */
/* (empty) */

/* XXX gccint 18.9.16 Node: Miscellaneous register hooks */
/* (empty) */

/* XXX gccint 18.10 Node: Implementing the Varargs Macros */
/* (empty) */

/* XXX gccint 18.11 Node: Support for Nested Functions */

#define	TRAMPOLINE_SIZE				(4*UNITS_PER_WORD)
#define	TRAMPOLINE_ALIGNMENT			BITS_PER_WORD

/* XXX gccint 18.12 Node: Implicit Calls to Library Routines */
/* (empty) */

/* XXX gccint 18.13 Node: Addressing Modes */

#define	CONSTANT_ADDRESS_P(X)			dadao_constant_address_p (X)

#define	MAX_REGS_PER_ADDRESS			2

/* XXX gccint 18.14 Node: Anchored Addresses */
/* (empty) */

/* XXX gccint 18.15 Node: Condition Code Status */
/* XXX gccint 18.15.1 Node: Representation of condition codes using (cc0) */
/* (empty) */
/* XXX gccint 18.15.2 Node: Representation of condition codes using registers */

#define SELECT_CC_MODE(OP, X, Y)		dadao_select_cc_mode (OP, X, Y)

#define REVERSIBLE_CC_MODE(MODE)		dadao_reversible_cc_mode (MODE)

/* XXX gccint 18.16 Node: Describing Relative Costs of Operations */

#define SLOW_BYTE_ACCESS			0

/* XXX gccint 18.17 Node: Adjusting the Instruction Scheduler */
/* (empty) */

/* XXX gccint 18.18 Node: Dividing the Output into Sections (Texts, Data, ...) */

#define TEXT_SECTION_ASM_OP			"\t.text"
#define DATA_SECTION_ASM_OP			"\t.data"
#define BSS_SECTION_ASM_OP			"\t.section\t.bss"

/* XXX gccint 18.19 Node: Position Independent Code */
/* (empty) */

/* XXX gccint 18.20 Node: Defining the Output Assembler Language */
/* XXX gccint 18.20.1 Node: The Overall Framework of an Assembler File */
#define ASM_COMMENT_START			"#"

/* These aren't currently functional.  We just keep them as markers.  */
#define ASM_APP_ON				"#APP\n"
#define ASM_APP_OFF				"#NO_APP\n"

/* XXX gccint 18.20.2 Node: Output of Data */
/* (empty) */

/* XXX gccint 18.20.3 Node: Output of Uninitialized Variables */
/* (empty) */

/* XXX gccint 18.20.4 Node: Output and Generation of Labels */

/* Undocumented, see TARGET_ASM_GLOBALIZE_LABEL in gccint 18.20.4 */
#define	GLOBAL_ASM_OP				"\t.global\t"

/* XXX gccint 18.20.5 Node: How Initialization Functions Are Handled */
/* (empty) */

/* XXX gccint 18.20.6 Node: Macros Controlling Initialization Routines */
/* (empty) */

/* XXX gccint 18.20.7 Node: Output of Assembler Instructions */

#define	REGISTER_NAMES {							\
	"rg0", "rg1", "rg2", "rg3", "rg4", "rg5", "rg6", "rg7",			\
	"rg8", "rg9", "rg10", "rg11", "rg12", "rg13", "rg14", "rg15",		\
	"rg16", "rg17", "rg18", "rg19", "rg20", "rg21", "rg22", "rg23",		\
	"rg24", "rg25", "rg26", "rg27", "rg28", "rg29", "rg30", "rg31",		\
	"rg32", "rg33", "rg34", "rg35", "rg36", "rg37", "rg38", "rg39",		\
	"rg40", "rg41", "rg42", "rg43", "rg44", "rg45", "rg46", "rg47",		\
	"rg48", "rg49", "rg50", "rg51", "rg52", "rg53", "rg54", "rg55",		\
	"rg56", "rg57", "rg58", "rg59", "rg60", "rg61", "rg62", "rg63",		\
	"rp0", "rp1", "rp2", "rp3", "rp4", "rp5", "rp6", "rp7",			\
	"rp8", "rp9", "rp10", "rp11", "rp12", "rp13", "rp14", "rp15",		\
	"rp16", "rp17", "rp18", "rp19", "rp20", "rp21", "rp22", "rp23",		\
	"rp24", "rp25", "rp26", "rp27", "rp28", "rp29", "rp30", "rp31",		\
	"rp32", "rp33", "rp34", "rp35", "rp36", "rp37", "rp38", "rp39",		\
	"rp40", "rp41", "rp42", "rp43", "rp44", "rp45", "rp46", "rp47",		\
	"rp48", "rp49", "rp50", "rp51", "rp52", "rp53", "rp54", "rp55",		\
	"rp56", "rp57", "rp58", "rp59", "rp60", "rp61", "rp62", "rp63",		\
	"rf0", "rf1", "rf2", "rf3", "rf4", "rf5", "rf6", "rf7",			\
	"rf8", "rf9", "rf10", "rf11", "rf12", "rf13", "rf14", "rf15",		\
	"rf16", "rf17", "rf18", "rf19", "rf20", "rf21", "rf22", "rf23",		\
	"rf24", "rf25", "rf26", "rf27", "rf28", "rf29", "rf30", "rf31",		\
	"rf32", "rf33", "rf34", "rf35", "rf36", "rf37", "rf38", "rf39",		\
	"rf40", "rf41", "rf42", "rf43", "rf44", "rf45", "rf46", "rf47",		\
	"rf48", "rf49", "rf50", "rf51", "rf52", "rf53", "rf54", "rf55",		\
	"rf56", "rf57", "rf58", "rf59", "rf60", "rf61", "rf62", "rf63",		\
	"rv0", "rv1", "rv2", "rv3", "rv4", "rv5", "rv6", "rv7",			\
	"rv8", "rv9", "rv10", "rv11", "rv12", "rv13", "rv14", "rv15",		\
	"rv16", "rv17", "rv18", "rv19", "rv20", "rv21", "rv22", "rv23",		\
	"rv24", "rv25", "rv26", "rv27", "rv28", "rv29", "rv30", "rv31",		\
	"rv32", "rv33", "rv34", "rv35", "rv36", "rv37", "rv38", "rv39",		\
	"rv40", "rv41", "rv42", "rv43", "rv44", "rv45", "rv46", "rv47",		\
	"rv48", "rv49", "rv50", "rv51", "rv52", "rv53", "rv54", "rv55",		\
	"rv56", "rv57", "rv58", "rv59", "rv60", "rv61", "rv62", "rv63",		\
	"rs0",  "rE",  "rs2",  "rs3",  "rs4",  "rs5", "rO", "rs7",		\
	"rs8", "rs9", "rs10", "rs11", "rs12", "rs13", "rs14", "rs15",		\
	"rs16", "rs17", "rs18", "rs19", "rs20", "rs21", "rs22", "rs23",		\
	"rs24", "rs25", "rs26", "rs27", "rs28", "rs29", "rs30", "rs31",		\
	"rs32", "rs33", "rs34", "rs35", "rs36", "rs37", "rs38", "rs39",		\
	"rs40", "rs41", "rs42", "rs43", "rs44", "rs45", "rs46", "rs47",		\
	"rs48", "rs49", "rs50", "rs51", "rs52", "rs53", "rs54", "rs55",		\
	"rs56", "rs57", "rs58", "rs59", "rs60", "rs61", "rs62", "rs63"}

#define	ADDITIONAL_REGISTER_NAMES {		\
	{"sp", STACK_POINTER_REGNUM},		\
	{"rE", 257},				\
	{"rO", DADAO_rO_REGNUM}}

/* PRINT_OPERAND:		see TARGET_PRINT_OPERAND */
/* PRINT_OPERAND_ADDRESS:	see TARGET_PRINT_OPERAND_ADDRESS */

#define	ASM_OUTPUT_REG_PUSH(STREAM, REGNO)				\
	do {								\
		fprintf (STREAM, "\tsubu\t%s, %s, 8\n",			\
			reg_names[STACK_POINTER_REGNUM],		\
			reg_names[STACK_POINTER_REGNUM]);		\
		fprintf (STREAM, "\tsto\t%s, %s, 0\n",			\
			reg_names[(REGNO)],				\
			reg_names[STACK_POINTER_REGNUM]);		\
	} while (0)

#define	ASM_OUTPUT_REG_POP(STREAM, REGNO)				\
	do {								\
		fprintf (STREAM, "\tldo\t%s, %s, 0\n",			\
			reg_names[(REGNO)],				\
			reg_names[STACK_POINTER_REGNUM]);		\
		fprintf (STREAM, "\taddu\t%s, %s, 8\n",			\
			reg_names[STACK_POINTER_REGNUM],		\
			reg_names[STACK_POINTER_REGNUM]);		\
	} while (0)

/* XXX gccint 18.20.8 Node: Output of Dispatch Tables */
#define	ASM_OUTPUT_ADDR_DIFF_ELT(STREAM, BODY, VALUE, REL)		\
	fprintf (STREAM, "\t.dd.t32\tL%d-L%d\n", VALUE, REL)

#define	ASM_OUTPUT_ADDR_VEC_ELT(STREAM, VALUE)				\
	fprintf (STREAM, "\t.dd.o64\tL%d\n", VALUE)

/* XXX gccint 18.20.9 Node: Assembler Commands for Exception Regions */
/* (empty) */

/* XXX gccint 18.20.10 Node: Assembler Commands for Alignment */

#define	ASM_OUTPUT_ALIGN(STREAM, POWER)					\
	fprintf (STREAM, "%s%d\n", ALIGN_ASM_OP, POWER)

/* XXX gccint 18.21 Node: Controlling Debugging Information Format */
/* (empty) */

/* XXX gccint 18.22 Node: Cross Compilation and Floating Point */
/* (empty) */

/* XXX gccint 18.23 Node: Mode Switching Instructions */
/* (empty) */

/* XXX gccint 18.24 Node: Defining target-specific uses of __attribute__ */
/* (empty) */

/* XXX gccint 18.25 Node: Emulating TLS */
/* (empty) */

/* XXX gccint 18.26 Node: Defining coprocessor specifics for MIPS targets */
/* (empty) */

/* XXX gccint 18.27 Node: Parameters for Precompiled Header Validity Checking */
/* (empty) */

/* XXX gccint 18.28 Node: C++ ABI parameters */
/* (empty) */

/* XXX gccint 18.29 Node: D ABI parameters */
/* (empty) */

/* XXX gccint 18.30 Node: Adding support for named address spaces */
/* (empty) */

/* XXX gccint 18.31 Node: Miscellaneous Parameters */

#define	CASE_VECTOR_MODE			DImode
#define	CASE_VECTOR_PC_RELATIVE			0

#define	WORD_REGISTER_OPERATIONS		1

/* We have a choice, which makes this yet another parameter to tweak.  The
   gut feeling is currently that SIGN_EXTEND wins; "int" is more frequent
   than "unsigned int", and we have signed characters. */
#define	LOAD_EXTEND_OP(MODE)			SIGN_EXTEND

#define	MOVE_MAX				8

/* ??? DADAO allows a choice of STORE_FLAG_VALUE.  Revisit later,
   we don't have scc expanders yet.  */

#define	Pmode					DImode
#define	FUNCTION_MODE				SImode

#endif /* GCC_DADAO_H */
