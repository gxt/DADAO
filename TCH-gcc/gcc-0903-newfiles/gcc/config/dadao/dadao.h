/*
 * Definitions of target machine for GNU compiler, for DADAO.
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */

#ifndef GCC_DADAO_H
#define GCC_DADAO_H

/* First, some local helper macros.  Note that the "default" value of
   FIXED_REGISTERS, CALL_USED_REGISTERS, REG_ALLOC_ORDER and
   REG_CLASS_CONTENTS depend on these values.  */
#define DADAO_RESERVED_GNU_ARG_0_REGNUM 231
#define DADAO_FIRST_ARG_REGNUM		(DADAO_RESERVED_GNU_ARG_0_REGNUM)
#define DADAO_FIRST_INCOMING_ARG_REGNUM (DADAO_RESERVED_GNU_ARG_0_REGNUM)
#define DADAO_MAX_ARGS_IN_REGS 16

/* FIXME: This one isn't fully implemented yet.  Return values larger than
   one register are passed by reference in DADAO_STRUCT_VALUE_REGNUM by the
   caller, except for return values of type "complex".  */
#define DADAO_MAX_REGS_FOR_VALUE 16
#define DADAO_RETURN_VALUE_REGNUM (DADAO_RESERVED_GNU_ARG_0_REGNUM)
#define DADAO_OUTGOING_RETURN_VALUE_REGNUM (DADAO_RESERVED_GNU_ARG_0_REGNUM)
#define DADAO_STRUCT_VALUE_REGNUM 251
#define DADAO_FRAME_POINTER_REGNUM		65
#define DADAO_STACK_POINTER_REGNUM		66
#define DADAO_LAST_GENERAL_REGISTER 255
#define DADAO_INCOMING_RETURN_ADDRESS_REGNUM DADAO_rJ_REGNUM
#define DADAO_HIMULT_REGNUM 258
#define DADAO_REMAINDER_REGNUM DADAO_rR_REGNUM
#define DADAO_ARG_POINTER_REGNUM		261
#define DADAO_rO_REGNUM 262

/* Four registers; "ideally, these registers should be call-clobbered", so
   just grab a bunch of the common clobbered registers.  FIXME: Last
   registers of return-value should be used, with an error if there's a
   return-value (that collides in size).  */
#define DADAO_EH_RETURN_DATA_REGNO_START (DADAO_STRUCT_VALUE_REGNUM - 4)

/* Try to keep the definitions from running away on their own.  */
#if (DADAO_EH_RETURN_DATA_REGNO_START \
     != DADAO_RESERVED_GNU_ARG_0_REGNUM + DADAO_MAX_ARGS_IN_REGS)
 #error DADAO register definition inconsistency
#endif

#if (DADAO_MAX_REGS_FOR_VALUE + DADAO_MAX_ARGS_IN_REGS > 32)
 #error DADAO parameters and return values bad, more than 32 registers
#endif

/* This chosen as "a call-clobbered hard register that is otherwise
   untouched by the epilogue".  */
#define DADAO_EH_RETURN_STACKADJ_REGNUM 252

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

#define	TARGET_DEFAULT_TARGET_FLAGS	(MASK_BASE_ADDRESSES)

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
#define	PCC_BITFIELD_TYPE_MATTERS	1

/* XXX gccint 18.6 Node: Layout of Source Language Data Types */
#define	INT_TYPE_SIZE			32
#define	LONG_LONG_TYPE_SIZE		64

#define	FLOAT_TYPE_SIZE			32
#define	DOUBLE_TYPE_SIZE		64
#define	LONG_DOUBLE_TYPE_SIZE		64

#define	DEFAULT_SIGNED_CHAR		0

/* XXX gccint 18.7 Node: Register Usage */

/* XXX gccint 18.7.1 Node: Basic Characteristics of Registers */
#define	FIRST_PSEUDO_REGISTER		(256 + 32)

#define FIXED_REGISTERS {				\
	/* 0 ~ 63 */					\
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
	0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	/* 64 ~ 127 */					\
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	/* 128 ~ 191 */					\
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	/* 192 ~ 255 */					\
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 1, \
	/* 256 ~ 287 */					\
	1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
}

#define CALL_USED_REGISTERS {				\
	/* 0 ~ 63 */					\
	1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,	\
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	/* 64 ~ 127 */					\
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	/* 128 ~ 191 */					\
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	/* 192 ~ 255 */					\
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	/* 256 ~ 287 */					\
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
	1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, \
}

#define	PC_REGNUM			64

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
	REMAINDER_REG,
	HIMULT_REG,
	SYSTEM_REGS,
	ALL_REGS,
	LIM_REG_CLASSES };

#define N_REG_CLASSES			((int) LIM_REG_CLASSES)

#define REG_CLASS_NAMES	{			\
	"NO_REGS",				\
	"GENERAL_REGS",				\
	"POINTER_REGS",				\
	"FLOATING_REGS",			\
	"VECTOR_REGS",				\
	"REMAINDER_REG",			\
	"HIMULT_REG",				\
	"SYSTEM_REGS",				\
	"ALL_REGS"}

/* Note that the contents of each item is always 32 bits.  */
#define REG_CLASS_CONTENTS {							\
	{ 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0,	0},	\
	{~0,	~0,	~0,	~0,	~0,	~0,	~0,	~0,	0x20},	\
	{ 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0,	0},	\
	{ 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0,	0},	\
	{ 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0,	0},	\
	{ 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0,	0x10},	\
	{ 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0,	4},	\
	{ 0,	 0,	 0,	 0,	 0,	 0,	 0,	 0,	~0},	\
	{~0,	~0,	~0,	~0,	~0,	~0,	~0,	~0,	~0}}

#define REGNO_REG_CLASS(REGNO)						\
 ((REGNO) <= DADAO_LAST_GENERAL_REGISTER				\
  || (REGNO) == DADAO_ARG_POINTER_REGNUM				\
  ? GENERAL_REGS							\
  : (REGNO) == DADAO_REMAINDER_REGNUM ? REMAINDER_REG			\
  : (REGNO) == DADAO_HIMULT_REGNUM ? HIMULT_REG : SYSTEM_REGS)

#define BASE_REG_CLASS		GENERAL_REGS
#define INDEX_REG_CLASS		GENERAL_REGS

#define REGNO_OK_FOR_BASE_P(REGNO)					\
 ((REGNO) <= DADAO_LAST_GENERAL_REGISTER				\
  || (REGNO) == DADAO_ARG_POINTER_REGNUM				\
  || (reg_renumber[REGNO] > 0						\
      && reg_renumber[REGNO] <= DADAO_LAST_GENERAL_REGISTER))

#define REGNO_OK_FOR_INDEX_P(REGNO) REGNO_OK_FOR_BASE_P (REGNO)

#define CLASS_MAX_NREGS(CLASS, MODE)				targetm.hard_regno_nregs (CLASS, MODE)

/* XXX gccint 18.9 Node: Stack Layout and Calling Conventions */

/* XXX gccint 18.9.1 Node: Basic Stack Layout */
#define STACK_GROWS_DOWNWARD			1
#define FRAME_GROWS_DOWNWARD			1

#define FIRST_PARM_OFFSET(FUNDECL)		0

#define DYNAMIC_CHAIN_ADDRESS(FRAMEADDR)	dadao_dynamic_chain_address (FRAMEADDR)

#define SETUP_FRAME_ADDRESSES()			dadao_setup_frame_addresses ()

#define RETURN_ADDR_RTX(COUNT, FRAME)		dadao_return_addr_rtx (COUNT, FRAME)

/* It's in rJ before we store it somewhere.  */
#define INCOMING_RETURN_ADDR_RTX		gen_rtx_REG (Pmode, DADAO_INCOMING_RETURN_ADDRESS_REGNUM)

/* XXX gccint 18.9.2 Node: Exception Handling Support */
#define EH_RETURN_DATA_REGNO(N)			dadao_eh_return_data_regno (N)
#define EH_RETURN_STACKADJ_RTX			dadao_eh_return_stackadj_rtx ()
#define EH_RETURN_HANDLER_RTX			dadao_eh_return_handler_rtx ()

#define ASM_PREFERRED_EH_DATA_FORMAT(CODE, GLOBAL)	dadao_asm_preferred_eh_data_format (CODE, GLOBAL)

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
#define EPILOGUE_USES(REGNO)		((REGNO) == DADAO_INCOMING_RETURN_ADDRESS_REGNUM)

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


/* XXX */

/* Node: Costs */

#define SLOW_BYTE_ACCESS 0


/* Node: Sections */

/* This must be a constant string, since it's used in crtstuff.c.  */
#define TEXT_SECTION_ASM_OP \
 "\t.text"

/* FIXME: Not documented.  */
#define DATA_SECTION_ASM_OP \
 dadao_data_section_asm_op ()

#define READONLY_DATA_SECTION_ASM_OP	"\t.section\t.rodata"

/* Node: PIC */
/* (empty) */


/* Node: File Framework */
#define ASM_COMMENT_START "#"

/* These aren't currently functional.  We just keep them as markers.  */
#define ASM_APP_ON "#APP\n"
#define ASM_APP_OFF "#NO_APP\n"

#define OUTPUT_QUOTED_STRING(STREAM, STRING) \
 dadao_output_quoted_string (STREAM, STRING, strlen (STRING))

#define TARGET_ASM_NAMED_SECTION default_elf_asm_named_section

/* Node: Uninitialized Data */

#define ASM_OUTPUT_ALIGNED_COMMON(ST, N, S, A) \
 dadao_asm_output_aligned_common (ST, N, S, A)

#define ASM_OUTPUT_ALIGNED_LOCAL(ST, N, S, A) \
 dadao_asm_output_aligned_local (ST, N, S, A)


/* Node: Label Output */

#define ASM_OUTPUT_LABEL(STREAM, NAME) \
 dadao_asm_output_label (STREAM, NAME)

#define ASM_OUTPUT_INTERNAL_LABEL(STREAM, NAME) \
 dadao_asm_output_internal_label (STREAM, NAME)

#define ASM_DECLARE_REGISTER_GLOBAL(STREAM, DECL, REGNO, NAME) \
 dadao_asm_declare_register_global (STREAM, DECL, REGNO, NAME)

#define GLOBAL_ASM_OP "\t.global "

#define ASM_WEAKEN_LABEL(STREAM, NAME) \
 dadao_asm_weaken_label (STREAM, NAME)

#define MAKE_DECL_ONE_ONLY(DECL) \
 dadao_make_decl_one_only (DECL)

#define ASM_OUTPUT_LABELREF(STREAM, NAME) \
 dadao_asm_output_labelref (STREAM, NAME)

#define ASM_GENERATE_INTERNAL_LABEL(LABEL, PREFIX, NUM) \
 sprintf (LABEL, "*%s%ld", PREFIX, (long)(NUM))

#define ASM_OUTPUT_DEF(STREAM, NAME, VALUE) \
 dadao_asm_output_def (STREAM, NAME, VALUE)

/* Node: Macros for Initialization */
/* We're compiling to ELF and linking to MMO; fundamental ELF features
   that GCC depend on are there.  */

/* These must be constant strings, since they're used in crtstuff.c.  */
#define INIT_SECTION_ASM_OP "\t.section .init,\"ax\""

#define FINI_SECTION_ASM_OP "\t.section .fini,\"ax\""

#define OBJECT_FORMAT_ELF


/* Node: Instruction Output */

#define REGISTER_NAMES {								\
	"$0", "$1", "$2", "$3", "$4", "$5", "$6", "$7",					\
	"$8", "$9", "$10", "$11", "$12", "$13", "$14", "$15",				\
	"$16", "$17", "$18", "$19", "$20", "$21", "$22", "$23",				\
	"$24", "$25", "$26", "$27", "$28", "$29", "$30", "$31",				\
	"$32", "$33", "$34", "$35", "$36", "$37", "$38", "$39",				\
	"$40", "$41", "$42", "$43", "$44", "$45", "$46", "$47",				\
	"$48", "$49", "$50", "$51", "$52", "$53", "$54", "$55",				\
	"$56", "$57", "$58", "$59", "$60", "$61", "$62", "$63",				\
	"$64", "$65", "$66", "$67", "$68", "$69", "$70", "$71",				\
	"$72", "$73", "$74", "$75", "$76", "$77", "$78", "$79",				\
	"$80", "$81", "$82", "$83", "$84", "$85", "$86", "$87",				\
	"$88", "$89", "$90", "$91", "$92", "$93", "$94", "$95",				\
	"$96", "$97", "$98", "$99", "$100", "$101", "$102", "$103",			\
	"$104", "$105", "$106", "$107", "$108", "$109", "$110", "$111",			\
	"$112", "$113", "$114", "$115", "$116", "$117", "$118", "$119",			\
	"$120", "$121", "$122", "$123", "$124", "$125", "$126", "$127",			\
	"$128", "$129", "$130", "$131", "$132", "$133", "$134", "$135",			\
	"$136", "$137", "$138", "$139", "$140", "$141", "$142", "$143",			\
	"$144", "$145", "$146", "$147", "$148", "$149", "$150", "$151",			\
	"$152", "$153", "$154", "$155", "$156", "$157", "$158", "$159",			\
	"$160", "$161", "$162", "$163", "$164", "$165", "$166", "$167",			\
	"$168", "$169", "$170", "$171", "$172", "$173", "$174", "$175",			\
	"$176", "$177", "$178", "$179", "$180", "$181", "$182", "$183",			\
	"$184", "$185", "$186", "$187", "$188", "$189", "$190", "$191",			\
	"$192", "$193", "$194", "$195", "$196", "$197", "$198", "$199",			\
	"$200", "$201", "$202", "$203", "$204", "$205", "$206", "$207",			\
	"$208", "$209", "$210", "$211", "$212", "$213", "$214", "$215",			\
	"$216", "$217", "$218", "$219", "$220", "$221", "$222", "$223",			\
	"$224", "$225", "$226", "$227", "$228", "$229", "$230", "$231",			\
	"$232", "$233", "$234", "$235", "$236", "$237", "$238", "$239",			\
	"$240", "$241", "$242", "$243", "$244", "$245", "$246", "$247",			\
	"$248", "$249", "$250", "$251", "$252", "$253", "$254", "$255",			\
	"rD",  "rE",  "rH",  "rJ",  "rR",  "ap_!BAD!", "rO", "N.A.7",			\
	"N.A.8", "N.A.9", "N.A.10", "N.A.11", "N.A.12", "N.A.13", "N.A.14", "N.A.15",	\
	"N.A.16", "N.A.17", "N.A.18", "N.A.19", "N.A.20", "N.A.21", "N.A.22", "N.A.23",	\
	"N.A.24", "N.A.25", "N.A.26", "N.A.27", "N.A.28", "N.A.29", "N.A.30", "N.A.31" }

#define ADDITIONAL_REGISTER_NAMES			\
 {{"sp", 66}, {"rD", 256}, {"rE", 257},	\
  {"rH", 258}, {"rJ", DADAO_rJ_REGNUM}, {"rO", DADAO_rO_REGNUM}}

#define ASM_OUTPUT_REG_PUSH(STREAM, REGNO) \
 dadao_asm_output_reg_push (STREAM, REGNO)

#define ASM_OUTPUT_REG_POP(STREAM, REGNO) \
 dadao_asm_output_reg_pop (STREAM, REGNO)


/* Node: Dispatch Tables */

/* We define both types, since SImode is the better, but DImode the only
   possible for dadaoal so that's the one actually used.  */
#define ASM_OUTPUT_ADDR_DIFF_ELT(STREAM, BODY, VALUE, REL) \
 dadao_asm_output_addr_diff_elt (STREAM, BODY, VALUE, REL)

#define ASM_OUTPUT_ADDR_VEC_ELT(STREAM, VALUE) \
 dadao_asm_output_addr_vec_elt (STREAM, VALUE)


/* Node: Exception Region Output */
/* (empty) */

/* Node: Alignment Output */

#define ASM_OUTPUT_SKIP(STREAM, NBYTES) \
 dadao_asm_output_skip (STREAM, NBYTES)

#define ASM_OUTPUT_ALIGN(STREAM, POWER) \
 dadao_asm_output_align (STREAM, POWER)


/* Node: All Debuggers */

#define DBX_REGISTER_NUMBER(REGNO) \
 dadao_dbx_register_number (REGNO)


/* Node: DBX Options */
/* (empty) */
/* Node: DBX Hooks */
/* (empty) */
/* Node: File Names and DBX */
/* (empty) */


/* Node: DWARF */
#define DWARF2_DEBUGGING_INFO 1
#define DWARF2_ASM_LINE_DEBUG_INFO 1

/* Node: Misc */

/* There's no way to get a PC-relative offset into tables for SImode, so
   for the moment we have absolute entries in DImode.
   When we're going ELF, these should be SImode and 1.  */
#define CASE_VECTOR_MODE DImode
#define CASE_VECTOR_PC_RELATIVE 0

#define WORD_REGISTER_OPERATIONS 1

/* We have a choice, which makes this yet another parameter to tweak.  The
   gut feeling is currently that SIGN_EXTEND wins; "int" is more frequent
   than "unsigned int", and we have signed characters.  FIXME: measure.  */
#define LOAD_EXTEND_OP(MODE) (TARGET_ZERO_EXTEND ? ZERO_EXTEND : SIGN_EXTEND)

#define MOVE_MAX 8

/* ??? DADAO allows a choice of STORE_FLAG_VALUE.  Revisit later,
   we don't have scc expanders yet.  */

#define Pmode DImode

#define FUNCTION_MODE QImode

/* These are checked.  */
#define DOLLARS_IN_IDENTIFIERS 0
#define NO_DOLLAR_IN_LABEL
#define NO_DOT_IN_LABEL

#endif /* GCC_DADAO_H */
/*
 * Local variables:
 * eval: (c-set-style "gnu")
 * indent-tabs-mode: t
 * End:
 */
