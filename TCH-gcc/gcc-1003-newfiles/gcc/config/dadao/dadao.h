/*
 * Definitions of target machine for GNU compiler, for DADAO.
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */
#ifndef GCC_DADAO_H
#define GCC_DADAO_H

#define OBJECT_FORMAT_ELF

#define	DADAO_REG_RD_START			0x00
#define	DADAO_REG_RB_START			0x40
#define	DADAO_REG_RF_START			0x80
#define	DADAO_REG_RA_START			0xC0
#define	DADAO_REG_RS_START			0x100

#define	DADAO_MAX_ARGS_IN_REGS			16
#define	DADAO_CONST_ZERO_REGNUM			(DADAO_REG_RD_START + 0x00)
#define	DADAO_EH_RETURN_DATA_REGNUM		(DADAO_REG_RD_START + 0x04)
#define	DADAO_FIRST_ARG_REGNUM			(DADAO_REG_RD_START + 0x10)
#define	DADAO_RETURN_VALUE_REGNUM		(DADAO_REG_RD_START + 0x1F)

/* FIXME: This one isn't fully implemented yet.  Return values larger than
   one register are passed by reference in DADAO_STRUCT_VALUE_REGNUM by the
   caller, except for return values of type "complex".  */
#define	DADAO_PROGRAM_COUNTER_REGNUM		(DADAO_REG_RB_START + 0x00)
#define	DADAO_STACK_POINTER_REGNUM		(DADAO_REG_RB_START + 0x01)
#define	DADAO_FRAME_POINTER_REGNUM		(DADAO_REG_RB_START + 0x02)
#define DADAO_ARG_POINTER_REGNUM		(DADAO_REG_RB_START + 0x03)
#define	DADAO_EH_RETURN_HANDLER_REGNUM		(DADAO_REG_RB_START + 0x04)
#define DADAO_STATIC_CHAIN_REGNUM		(DADAO_REG_RB_START + 0x05)
#define	DADAO_RETURN_ADDRESS_REGNUM		(DADAO_REG_RB_START + 0x3E)
#define	DADAO_STRUCT_VALUE_REGNUM		(DADAO_REG_RB_START + 0x3F)

#define DADAO_FUNCTION_ARG_SIZE(MODE, TYPE) \
 ((MODE) != BLKmode ? GET_MODE_SIZE (MODE) : int_size_in_bytes (TYPE))

/* Per-function machine data.  This is normally an opaque type just
   defined and used in the tm.c file, but we need to see the definition in
   dadao.md too.  */
struct GTY(()) machine_function
 {
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
#ifndef STARTFILE_SPEC
#define STARTFILE_SPEC			"crti%O%s crtbegin%O%s"
#endif

/* Provide a STARTFILE_SPEC appropriate for ELF.  Here we add the
   (even more) magical crtbegin.o file which provides part of the
   support for getting C++ file-scope static object constructed
   before entering `main'.  */

#if (DEFAULT_LIBC==LIBC_GLIBC)
  #undef STARTFILE_SPEC
  #ifdef HAVE_LD_PIE
	#define STARTFILE_SPEC \
	  "%{!shared: %{pg|p:gcrt1.o%s;pie:Scrt1.o%s;:crt1.o%s}}\
	   crti.o%s %{static:crtbeginT.o%s;shared|pie:crtbeginS.o%s;:crtbegin.o%s}"
  #else
	#define STARTFILE_SPEC \
	  "%{!shared: %{pg|p:gcrt1.o%s;:crt1.o%s}}\
	   crti.o%s %{static:crtbeginT.o%s;shared|pie:crtbeginS.o%s;:crtbegin.o%s}"
  #endif
#endif

/* Provide a ENDFILE_SPEC appropriate for ELF.  Here we tack on the
   magical crtend.o file which provides part of the support for
   getting C++ file-scope static object constructed before entering
   `main', followed by a normal ELF "finalizer" file, `crtn.o'.  */

#undef	ENDFILE_SPEC
#define ENDFILE_SPEC \
   "%{shared|pie:crtendS.o%s;:crtend.o%s} crtn.o%s"

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
#define	FIRST_PSEUDO_REGISTER		(64 * 4)

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
	RETURN_REGS,
	ALL_REGS,
	LIM_REG_CLASSES };

#define N_REG_CLASSES			((int) LIM_REG_CLASSES)

#define REG_CLASS_NAMES	{			\
	"NO_REGS",				\
	"GENERAL_REGS",				\
	"POINTER_REGS",				\
	"FLOATING_REGS",			\
	"RETURN_REGS",				\
	"ALL_REGS"}

/* Note that the contents of each item is always 32 bits.  */
#define REG_CLASS_CONTENTS {							\
	/* Rd	Rd	Rb	Rb	Rf	Rf	Ra	Ra */		\
	{ 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0},		\
	{~0,	~0,	 0,	 0,	 0,	 0,	 0,	 0},		\
	{ 0,	 0,	~0,	~0,	 0,	 0,	 0,	 0},		\
	{ 0,	 0,	 0,	 0,	~0,	~0,	 0,	 0},		\
	{ 0,	 0,	 0,	 0,	 0,	 0,	~0,	~0},		\
	{~0,	~0,	~0,	~0,	~0,	~0,	~0,	~0}}

#define REGNO_REG_CLASS(REGNO)							\
	( (REGNO) >= FIRST_PSEUDO_REGISTER	? NO_REGS			\
	: (REGNO) < 0x40			? GENERAL_REGS			\
	: (REGNO) < 0x80			? POINTER_REGS			\
	: (REGNO) < 0xC0			? FLOATING_REGS : RETURN_REGS)

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
#define STATIC_CHAIN_REGNUM			DADAO_STATIC_CHAIN_REGNUM

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

#define REVERSIBLE_CC_MODE(MODE)		1

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
	"rd0", "rd1", "rd2", "rd3", "rd4", "rd5", "rd6", "rd7",			\
	"rd8", "rd9", "rd10", "rd11", "rd12", "rd13", "rd14", "rd15",		\
	"rd16", "rd17", "rd18", "rd19", "rd20", "rd21", "rd22", "rd23",		\
	"rd24", "rd25", "rd26", "rd27", "rd28", "rd29", "rd30", "rd31",		\
	"rd32", "rd33", "rd34", "rd35", "rd36", "rd37", "rd38", "rd39",		\
	"rd40", "rd41", "rd42", "rd43", "rd44", "rd45", "rd46", "rd47",		\
	"rd48", "rd49", "rd50", "rd51", "rd52", "rd53", "rd54", "rd55",		\
	"rd56", "rd57", "rd58", "rd59", "rd60", "rd61", "rd62", "rd63",		\
	"rb0", "rb1", "rb2", "rb3", "rb4", "rb5", "rb6", "rb7",			\
	"rb8", "rb9", "rb10", "rb11", "rb12", "rb13", "rb14", "rb15",		\
	"rb16", "rb17", "rb18", "rb19", "rb20", "rb21", "rb22", "rb23",		\
	"rb24", "rb25", "rb26", "rb27", "rb28", "rb29", "rb30", "rb31",		\
	"rb32", "rb33", "rb34", "rb35", "rb36", "rb37", "rb38", "rb39",		\
	"rb40", "rb41", "rb42", "rb43", "rb44", "rb45", "rb46", "rb47",		\
	"rb48", "rb49", "rb50", "rb51", "rb52", "rb53", "rb54", "rb55",		\
	"rb56", "rb57", "rb58", "rb59", "rb60", "rb61", "rb62", "rb63",		\
	"rf0", "rf1", "rf2", "rf3", "rf4", "rf5", "rf6", "rf7",			\
	"rf8", "rf9", "rf10", "rf11", "rf12", "rf13", "rf14", "rf15",		\
	"rf16", "rf17", "rf18", "rf19", "rf20", "rf21", "rf22", "rf23",		\
	"rf24", "rf25", "rf26", "rf27", "rf28", "rf29", "rf30", "rf31",		\
	"rf32", "rf33", "rf34", "rf35", "rf36", "rf37", "rf38", "rf39",		\
	"rf40", "rf41", "rf42", "rf43", "rf44", "rf45", "rf46", "rf47",		\
	"rf48", "rf49", "rf50", "rf51", "rf52", "rf53", "rf54", "rf55",		\
	"rf56", "rf57", "rf58", "rf59", "rf60", "rf61", "rf62", "rf63",		\
	"ra0", "ra1", "ra2", "ra3", "ra4", "ra5", "ra6", "ra7",			\
	"ra8", "ra9", "ra10", "ra11", "ra12", "ra13", "ra14", "ra15",		\
	"ra16", "ra17", "ra18", "ra19", "ra20", "ra21", "ra22", "ra23",		\
	"ra24", "ra25", "ra26", "ra27", "ra28", "ra29", "ra30", "ra31",		\
	"ra32", "ra33", "ra34", "ra35", "ra36", "ra37", "ra38", "ra39",		\
	"ra40", "ra41", "ra42", "ra43", "ra44", "ra45", "ra46", "ra47",		\
	"ra48", "ra49", "ra50", "ra51", "ra52", "ra53", "ra54", "ra55",		\
	"ra56", "ra57", "ra58", "ra59", "ra60", "ra61", "ra62", "ra63"}

#define	ADDITIONAL_REGISTER_NAMES {		\
	{"zero", DADAO_CONST_ZERO_REGNUM},	\
	{"sp", STACK_POINTER_REGNUM},		\
	{"fp", FRAME_POINTER_REGNUM},		\
	{"pc", PC_REGNUM}}

/* PRINT_OPERAND:		see TARGET_PRINT_OPERAND */
/* PRINT_OPERAND_ADDRESS:	see TARGET_PRINT_OPERAND_ADDRESS */

#define	ASM_OUTPUT_REG_PUSH(STREAM, REGNO)				\
	do {								\
		fprintf (STREAM, "\tadd\t%s, -8\n",			\
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
		fprintf (STREAM, "\tadd\t%s, 8\n",			\
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
