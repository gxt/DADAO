/*
 * Definitions of target machine for GNU compiler, for DADAO.
 * Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
 * Contributed by Guan Xuetao <gxt@pku.edu.cn>
 */

#define IN_TARGET_CODE 1

#include "config.h"
#include "system.h"
#include "coretypes.h"
#include "backend.h"
#include "target.h"
#include "rtl.h"
#include "tree.h"
#include "stringpool.h"
#include "attribs.h"
#include "df.h"
#include "memmodel.h"
#include "tm_p.h"
#include "insn-config.h"
#include "optabs.h"
#include "regs.h"
#include "emit-rtl.h"
#include "recog.h"
#include "diagnostic-core.h"
#include "output.h"
#include "varasm.h"
#include "stor-layout.h"
#include "calls.h"
#include "explow.h"
#include "expr.h"
#include "dwarf2.h"
#include "tm-constrs.h"
#include "builtins.h"

/* This file should be included last.  */
#include "target-def.h"

/* We'd need a current_function_has_landing_pad.  It's marked as such when
   a nonlocal_goto_receiver is expanded.  Not just a C++ thing, but
   mostly.  */
#define DADAO_CFUN_HAS_LANDING_PAD (cfun->machine->has_landing_pad != 0)

/* For the default ABI, we rename registers at output-time to fill the gap
   between the (statically partitioned) saved registers and call-clobbered
   registers.  In effect this makes unused call-saved registers to be used
   as call-clobbered registers.  The benefit comes from keeping the number
   of local registers (value of rL) low, since there's a cost of
   increasing rL and clearing unused (unset) registers with lower numbers.
   Don't translate while outputting the prologue.  */
#define DADAO_OUTPUT_REGNO(N)	(N)

/* The canonical saved comparison operands for non-cc0 machines, set in
   the compare expander.  */
rtx dadao_compare_op0;
rtx dadao_compare_op1;

/* Declarations of locals.  */

/* Intermediate for insn output.  */
static int dadao_output_destination_register;

static void dadao_output_shiftvalue_op_from_str
  (FILE *, const char *, int64_t);
static void dadao_output_shifted_value (FILE *, int64_t);
static void dadao_output_condition (FILE *, const_rtx, int);
static void dadao_output_octa (FILE *, int64_t, int);
static void dadao_emit_sp_add (HOST_WIDE_INT offset);

#undef TARGET_HAVE_SPECULATION_SAFE_VALUE
#define TARGET_HAVE_SPECULATION_SAFE_VALUE speculation_safe_value_not_needed


/* XXX gccint Chapter 18: Target Description Macros and Functions */

/* XXX gccint 18.1 Node: The Global targetm Variable */

/* gcc_target MUST be at the end of this file, since TARGET_* macros will be used in TARGET_INITIALIZER */
/* struct gcc_target targetm = TARGET_INITIALIZER; */

/* XXX gccint 18.2 Node: Controlling the Compilation Driver, gcc */
/* (empty) */

/* XXX gccint 18.3 Node: Run-time Target Specification */
/* (empty) */

/* XXX gccint 18.4 Node: Defining data structures for per-function information */

/* Set the per-function data.  */
static struct machine_function * dd_init_machine_status (void)
{
	return ggc_cleared_alloc<machine_function> ();
}

/* INIT_EXPANDERS.  */
void dadao_init_expanders (void)
{
	init_machine_status = dd_init_machine_status;
}

/* XXX gccint 18.5 Node: Storage Layout */

/* Worker function for TARGET_PROMOTE_FUNCTION_MODE.  */
static machine_mode dd_promote_function_mode (const_tree type ATTRIBUTE_UNUSED,
                            machine_mode mode, int *punsignedp ATTRIBUTE_UNUSED,
                            const_tree fntype ATTRIBUTE_UNUSED, int for_return)
{
  /* Apparently not doing TRT if int < register-size.  FIXME: Perhaps
     FUNCTION_VALUE and LIBCALL_VALUE needs tweaking as some ports say.  */
  if (for_return == 1)
    return mode;

  /* Promotion of modes currently generates slow code, extending before
     operation, so we do it only for arguments.  */
  if (GET_MODE_CLASS (mode) == MODE_INT
      && GET_MODE_SIZE (mode) < 8)
    return DImode;
  else
    return mode;
}

#undef	TARGET_PROMOTE_FUNCTION_MODE
#define	TARGET_PROMOTE_FUNCTION_MODE		dd_promote_function_mode

/* XXX gccint 18.6 Node: Layout of Source Language Data Types */
/* (empty) */

/* XXX gccint 18.7 Node: Register Usage */
/* (empty) */

/* XXX gccint 18.8 Node: Register Classes */

static reg_class_t dd_secondary_reload (bool in_p ATTRIBUTE_UNUSED,
		rtx x ATTRIBUTE_UNUSED, reg_class_t rclass,
		machine_mode reload_mode ATTRIBUTE_UNUSED,
		secondary_reload_info *sri ATTRIBUTE_UNUSED)
{
	return NO_REGS;
}

#undef	TARGET_SECONDARY_RELOAD
#define	TARGET_SECONDARY_RELOAD			dd_secondary_reload

#undef	TARGET_LRA_P
#define	TARGET_LRA_P				hook_bool_void_false

/* XXX gccint 18.9 Node: Stack Layout and Calling Conventions */

/* XXX gccint 18.9.1 Node: Basic Stack Layout */

/* Implement TARGET_STARTING_FRAME_OFFSET.  */
static HOST_WIDE_INT dd_starting_frame_offset (void)
{
  /* The old frame pointer is in the slot below the new one, so
     FIRST_PARM_OFFSET does not need to depend on whether the
     frame-pointer is needed or not.  We have to adjust for the register
     stack pointer being located below the saved frame pointer.
     Similarly, we store the return address on the stack too, for
     exception handling, and always if we save the register stack pointer.  */
  return
    (-8 + (DADAO_CFUN_HAS_LANDING_PAD ? -16 : 0));
}

#undef	TARGET_STARTING_FRAME_OFFSET
#define	TARGET_STARTING_FRAME_OFFSET		dd_starting_frame_offset

/* DYNAMIC_CHAIN_ADDRESS.  */
rtx dadao_dynamic_chain_address (rtx frame)
{
  /* FIXME: the frame-pointer is stored at offset -8 from the current
     frame-pointer.  Unfortunately, the caller assumes that a
     frame-pointer is present for *all* previous frames.  There should be
     a way to say that that cannot be done, like for RETURN_ADDR_RTX.  */
  return plus_constant (Pmode, frame, -8);
}

/* SETUP_FRAME_ADDRESSES.  */
void dadao_setup_frame_addresses (void)
{
  /* Nothing needed at the moment.  */
}

/* RETURN_ADDR_RTX.  */
rtx dadao_return_addr_rtx (int count, rtx frame ATTRIBUTE_UNUSED)
{
  return count == 0
    ? get_hard_reg_initial_val (Pmode, DADAO_RETURN_ADDRESS_REGNUM)
    : NULL_RTX;
}

/* XXX gccint 18.9.2 Node: Exception Handling Support */
/* (empty) */

/* XXX gccint 18.9.3 Node: Specifying How Stack Checking is Done */
/* (empty) */

/* XXX gccint 18.9.4 Node: Registers That Address the Stack Frame */
/* (empty) */

/* XXX gccint 18.9.5 Node: Eliminating Frame Pointer and Arg Pointer */

/* FIXME: Is this requirement built-in?  Anyway, we should try to get rid
   of it; we can deduce the value.  */
static bool dd_frame_pointer_required (void)
{
  return (cfun->has_nonlocal_label);
}

#undef	TARGET_FRAME_POINTER_REQUIRED
#define	TARGET_FRAME_POINTER_REQUIRED			dd_frame_pointer_required

/* The difference between the (imaginary) frame pointer and the stack
   pointer.  Used to eliminate the frame pointer.  */
int dadao_initial_elimination_offset (int fromreg, int toreg)
{
  int regno;
  int fp_sp_offset = (get_frame_size () + crtl->outgoing_args_size + 7) & ~7;

  /* There is no actual offset between these two virtual values, but for
     the frame-pointer, we have the old one in the stack position below
     it, so the offset for the frame-pointer to the stack-pointer is one
     octabyte larger.  */
  if (fromreg == DADAO_ARG_POINTER_REGNUM
      && toreg == FRAME_POINTER_REGNUM)
    return 0;

  /* The difference is the size of local variables plus the size of
     outgoing function arguments that would normally be passed as
     registers but must be passed on stack because we're out of
     function-argument registers.  Only global saved registers are
     counted; the others go on the register stack.

     The frame-pointer is counted too if it is what is eliminated, as we
     need to balance the offset for it from TARGET_STARTING_FRAME_OFFSET.

     Also add in the slot for the register stack pointer we save if we
     have a landing pad.

     Unfortunately, we can't access $0..$14, from unwinder code easily, so
     store the return address in a frame slot too.  FIXME: Only for
     non-leaf functions.  FIXME: Always with a landing pad, because it's
     hard to know whether we need the other at the time we know we need
     the offset for one (and have to state it).

     We have to do alignment here; get_frame_size will not return a
     multiple of STACK_BOUNDARY.  FIXME: Add note in manual.  */

  return fp_sp_offset
    + (DADAO_CFUN_HAS_LANDING_PAD ? 16 : 0)
    + (fromreg == DADAO_ARG_POINTER_REGNUM ? 0 : 8);
}

/* XXX gccint 18.9.6 Node: Passing Function Arguments on the Stack */
/* (empty) */

/* XXX gccint 18.9.7 Node: Passing Arguments in Registers */

/* Return an rtx for a function argument to go in a register, and 0 for
   one that must go on stack.  */
static rtx dd_function_arg (cumulative_args_t argsp_v,
			machine_mode mode, const_tree type,
			bool named ATTRIBUTE_UNUSED)
{
  CUMULATIVE_ARGS *argsp = get_cumulative_args (argsp_v);

  /* Last-argument marker.  */
  if (type == void_type_node)
    return (argsp->regs < DADAO_MAX_ARGS_IN_REGS)
      ? gen_rtx_REG (mode, DADAO_FIRST_ARG_REGNUM + argsp->regs)
      : NULL_RTX;

  return (argsp->regs < DADAO_MAX_ARGS_IN_REGS
	  && !targetm.calls.must_pass_in_stack (mode, type)
	  && (GET_MODE_BITSIZE (mode) <= 64
	      || argsp->lib
	      || TARGET_LIBFUNC))
    ? gen_rtx_REG (mode, DADAO_FIRST_ARG_REGNUM + argsp->regs)
    : NULL_RTX;
}

#undef	TARGET_FUNCTION_ARG
#define	TARGET_FUNCTION_ARG		dd_function_arg

/* Returns nonzero for everything that goes by reference, 0 for
   everything that goes by value.  */
static bool dd_pass_by_reference (cumulative_args_t argsp_v, machine_mode mode,
			const_tree type, bool named ATTRIBUTE_UNUSED)
{
  CUMULATIVE_ARGS *argsp = get_cumulative_args (argsp_v);

  /* FIXME: Check: I'm not sure the must_pass_in_stack check is
     necessary.  */
  if (targetm.calls.must_pass_in_stack (mode, type))
    return true;

  if (DADAO_FUNCTION_ARG_SIZE (mode, type) > 8
      && !TARGET_LIBFUNC
      && (!argsp || !argsp->lib))
    return true;

  return false;
}

#undef	TARGET_PASS_BY_REFERENCE
#define	TARGET_PASS_BY_REFERENCE	dd_pass_by_reference

static void dd_function_arg_advance (cumulative_args_t argsp_v, machine_mode mode,
			const_tree type, bool named ATTRIBUTE_UNUSED)
{
  CUMULATIVE_ARGS *argsp = get_cumulative_args (argsp_v);
  int arg_size = DADAO_FUNCTION_ARG_SIZE (mode, type);

  argsp->regs = ((targetm.calls.must_pass_in_stack (mode, type)
		  || (arg_size > 8
		      && !TARGET_LIBFUNC
		      && !argsp->lib))
		 ? (DADAO_MAX_ARGS_IN_REGS) + 1
		 : argsp->regs + (7 + arg_size) / 8);
}

#undef	TARGET_FUNCTION_ARG_ADVANCE
#define	TARGET_FUNCTION_ARG_ADVANCE	dd_function_arg_advance

/* XXX gccint 18.9.8 Node: How Scalar Function Values Are Returned */

/* Implements TARGET_FUNCTION_VALUE.  */
static rtx dd_function_value (const_tree valtype,
			const_tree func ATTRIBUTE_UNUSED,
			bool outgoing ATTRIBUTE_UNUSED)
{
  machine_mode mode = TYPE_MODE (valtype);

  return gen_rtx_REG (mode, DADAO_RETURN_VALUE_REGNUM);
}

#undef	TARGET_FUNCTION_VALUE
#define	TARGET_FUNCTION_VALUE		dd_function_value

/* Implements TARGET_LIBCALL_VALUE.  */
static rtx dd_libcall_value (machine_mode mode,
		    const_rtx fun ATTRIBUTE_UNUSED)
{
  return gen_rtx_REG (mode, DADAO_RETURN_VALUE_REGNUM);
}

#undef	TARGET_LIBCALL_VALUE
#define	TARGET_LIBCALL_VALUE		dd_libcall_value

/* Implements TARGET_FUNCTION_VALUE_REGNO_P.  */
static bool dd_function_value_regno_p (const unsigned int regno)
{
  return regno == DADAO_RETURN_VALUE_REGNUM;
}

#undef	TARGET_FUNCTION_VALUE_REGNO_P
#define	TARGET_FUNCTION_VALUE_REGNO_P	dd_function_value_regno_p

/* XXX gccint 18.9.9 Node: How Large Values Are Returned */

/* Worker function for TARGET_STRUCT_VALUE_RTX.  */
static rtx dd_struct_value_rtx (tree fntype ATTRIBUTE_UNUSED,
		       int incoming ATTRIBUTE_UNUSED)
{
  return gen_rtx_REG (Pmode, DADAO_STRUCT_VALUE_REGNUM);
}

#undef	TARGET_STRUCT_VALUE_RTX
#define	TARGET_STRUCT_VALUE_RTX		dd_struct_value_rtx

/* XXX gccint 18.9.10 Node: Caller-Saves Register Allocation */
/* (empty) */

/* XXX gccint 18.9.11 Node: Function Entry and Exit */

/* Make a note that we've seen the beginning of the prologue.  This
   matters to whether we'll translate register numbers as calculated by
   dadao_reorg.  */
static void dd_asm_function_prologue (FILE *)
{
  cfun->machine->in_prologue = 1;
}

#undef	TARGET_ASM_FUNCTION_PROLOGUE
#define	TARGET_ASM_FUNCTION_PROLOGUE		dd_asm_function_prologue

/* Make a note that we've seen the end of the prologue.  */
static void dd_asm_function_end_prologue (FILE *stream ATTRIBUTE_UNUSED)
{
  cfun->machine->in_prologue = 0;
}

#undef	TARGET_ASM_FUNCTION_END_PROLOGUE
#define	TARGET_ASM_FUNCTION_END_PROLOGUE	dd_asm_function_end_prologue

/* TARGET_ASM_FUNCTION_EPILOGUE.  */
static void dd_asm_function_epilogue (FILE *stream)
{
  /* Emit an \n for readability of the generated assembly.  */
  fputc ('\n', stream);
}

#undef	TARGET_ASM_FUNCTION_EPILOGUE
#define	TARGET_ASM_FUNCTION_EPILOGUE		dd_asm_function_epilogue

/* XXX gccint 18.9.12 Node: Generating Code for Profiling */

/* FUNCTION_PROFILER.  */
void dadao_function_profiler (FILE *stream ATTRIBUTE_UNUSED,
			int labelno ATTRIBUTE_UNUSED)
{
  sorry ("function_profiler support for DADAO");
}

/* XXX gccint 18.9.13 Node: Permitting tail calls */
/* (empty) */

/* XXX gccint 18.9.14 Node: Shrink-wrapping separate components */
/* (empty) */

/* XXX gccint 18.9.15 Node: Stack smashing protection */
/* (empty) */

/* XXX gccint 18.9.16 Node: Miscellaneous register hooks */
/* (empty) */

/* XXX gccint 18.10 Node: Implementing the Varargs Macros */

/* Worker function for TARGET_SETUP_INCOMING_VARARGS.  For the moment,
   let's stick to pushing argument registers on the stack.  Later, we
   can parse all arguments in registers, to improve performance.  */
static void dd_setup_incoming_varargs (cumulative_args_t args_so_farp_v,
			machine_mode mode, tree vartype, int *pretend_sizep,
			int second_time ATTRIBUTE_UNUSED)
{
  CUMULATIVE_ARGS *args_so_farp = get_cumulative_args (args_so_farp_v);

  /* The last named variable has been handled, but
     args_so_farp has not been advanced for it.  */
  if (args_so_farp->regs + 1 < DADAO_MAX_ARGS_IN_REGS)
    *pretend_sizep = (DADAO_MAX_ARGS_IN_REGS - (args_so_farp->regs + 1)) * 8;

  /* We assume that one argument takes up one register here.  That should
     be true until we start messing with multi-reg parameters.  */
  if ((7 + (DADAO_FUNCTION_ARG_SIZE (mode, vartype))) / 8 != 1)
    internal_error ("DADAO Internal: Last named vararg would not fit in a register");
}

#undef	TARGET_SETUP_INCOMING_VARARGS
#define	TARGET_SETUP_INCOMING_VARARGS		dd_setup_incoming_varargs

/* XXX gccint 18.11 Node: Support for Nested Functions */

/* TARGET_ASM_TRAMPOLINE_TEMPLATE.  */
static void dd_asm_trampoline_template (FILE *stream)
{
  /* Read a value into the static-chain register and jump somewhere.  The
     static chain is stored at offset 16, and the function address is
     stored at offset 24.  */

  fprintf (stream, "\tgeta	rg63, 1f\n");
  fprintf (stream, "\tldo	rg63, rg63, 0\n");
  fprintf (stream, "\tcall	rp0, rg63, 0\n");
  fprintf (stream, "1:\t.dd.o64	0\n");
}

#undef	TARGET_ASM_TRAMPOLINE_TEMPLATE
#define	TARGET_ASM_TRAMPOLINE_TEMPLATE		dd_asm_trampoline_template

/* Set the static chain and function pointer field in the trampoline.
   We also SYNCID here to be sure (doesn't matter in the simulator, but
   some day it will).  */
static void dd_trampoline_init (rtx m_tramp, tree fndecl, rtx static_chain)
{
  rtx fnaddr = XEXP (DECL_RTL (fndecl), 0);
  rtx mem;

  emit_block_move (m_tramp, assemble_trampoline_template (),
		   GEN_INT (2*UNITS_PER_WORD), BLOCK_OP_NORMAL);

  mem = adjust_address (m_tramp, DImode, 2*UNITS_PER_WORD);
  emit_move_insn (mem, static_chain);
  mem = adjust_address (m_tramp, DImode, 3*UNITS_PER_WORD);
  emit_move_insn (mem, fnaddr);
}

#undef	TARGET_TRAMPOLINE_INIT
#define	TARGET_TRAMPOLINE_INIT			dd_trampoline_init

/* XXX gccint 18.12 Node: Implicit Calls to Library Routines */
/* (empty) */

/* XXX gccint 18.13 Node: Addressing Modes */

/* We must exclude constant addresses that have an increment that is not a
   multiple of four bytes because of restrictions of the GETA
   instruction.  */
int dadao_constant_address_p (rtx x)
{
  RTX_CODE code = GET_CODE (x);
  int addend = 0;
  /* When using "base addresses", anything constant goes.  */
  int constant_ok = 0;

  switch (code)
    {
    case LABEL_REF:
    case SYMBOL_REF:
      return 1;

    case HIGH:
      /* FIXME: Don't know how to dissect these.  Avoid them for now,
	 except we know they're constants.  */
      return constant_ok;

    case CONST_INT:
      addend = INTVAL (x);
      break;

    case CONST_DOUBLE:
      if (GET_MODE (x) != VOIDmode)
	/* Strange that we got here.  FIXME: Check if we do.  */
	return constant_ok;
      addend = CONST_DOUBLE_LOW (x);
      break;

    case CONST:
      /* Note that expressions with arithmetic on forward references don't
	 work in dadaoal.  People using gcc assembly code with dadaoal might
	 need to move arrays and such to before the point of use.  */
      if (GET_CODE (XEXP (x, 0)) == PLUS)
	{
	  rtx x0 = XEXP (XEXP (x, 0), 0);
	  rtx x1 = XEXP (XEXP (x, 0), 1);

	  if ((GET_CODE (x0) == SYMBOL_REF
	       || GET_CODE (x0) == LABEL_REF)
	      && (GET_CODE (x1) == CONST_INT
		  || (GET_CODE (x1) == CONST_DOUBLE
		      && GET_MODE (x1) == VOIDmode)))
	    addend = dadao_intval (x1);
	  else
	    return constant_ok;
	}
      else
	return constant_ok;
      break;

    default:
      return 0;
    }

  return constant_ok || (addend & 3) == 0;
}

/* Return 1 if the address is OK, otherwise 0.  */
static bool dd_legitimate_address_p (machine_mode mode ATTRIBUTE_UNUSED,
			   rtx x, bool strict_checking)
{
#define	_DD_LEGITIMATE_ADDR_RC(X, XC)	( REG_P(X) &&				\
	( (REGNO_REG_CLASS(REGNO(X)) == XC) ||					\
	( !strict_checking && (REGNO (X) >= FIRST_PSEUDO_REGISTER)) ||		\
	( strict_checking && (REGNO_REG_CLASS(reg_renumber[REGNO(X)]) == XC))))

	/* (mem rp) */
	if (_DD_LEGITIMATE_ADDR_RC(x, POINTER_REGS))
		return 1;

	if (GET_CODE(x) == PLUS) {
		rtx x1 = XEXP (x, 0);
		rtx x2 = XEXP (x, 1);

		/* (mem (plus (rp) (?))) */
		if (!_DD_LEGITIMATE_ADDR_RC(x1, POINTER_REGS))
			return 0;

		/* (mem (plus (rp) (rg))) */
		if (_DD_LEGITIMATE_ADDR_RC(x2, GENERAL_REGS))
			return 1;

		/* (mem (plus (rp) (-0x800, 0x7FF))) */
		if (satisfies_constraint_Id (x2))
			return 1;
	}

	return 0;
#undef	_DD_LEGITIMATE_ADDR_RC
}

#undef	TARGET_LEGITIMATE_ADDRESS_P
#define	TARGET_LEGITIMATE_ADDRESS_P		dd_legitimate_address_p

/* Implement TARGET_LEGITIMATE_CONSTANT_P.  */
static bool dd_legitimate_constant_p (machine_mode mode ATTRIBUTE_UNUSED, rtx x)
{
  RTX_CODE code = GET_CODE (x);

  /* We must allow any number due to the way the cse passes works; if we
     do not allow any number here, general_operand will fail, and insns
     will fatally fail recognition instead of "softly".  */
  if (code == CONST_INT || code == CONST_DOUBLE)
    return 1;

  return CONSTANT_ADDRESS_P (x);
}

#undef	TARGET_LEGITIMATE_CONSTANT_P
#define	TARGET_LEGITIMATE_CONSTANT_P		dd_legitimate_constant_p

/* XXX gccint 18.14 Node: Anchored Addresses */
/* (empty) */

/* XXX gccint 18.15 Node: Condition Code Status */
/* XXX gccint 18.15.1 Node: Representation of condition codes using (cc0) */
/* (empty) */
/* XXX gccint 18.15.2 Node: Representation of condition codes using registers */

/* SELECT_CC_MODE.  */
machine_mode dadao_select_cc_mode (RTX_CODE op, rtx x, rtx y ATTRIBUTE_UNUSED)
{
  /* We use CCSSmode, CCUUmode, CC_FPmode, CC_FPEQmode and CC_FUNmode to
     output different compare insns.  Note that we do not check the
     validity of the comparison here.  */

  if (GET_MODE_CLASS (GET_MODE (x)) == MODE_FLOAT)
    {
      if (op == ORDERED || op == UNORDERED || op == UNGE
	  || op == UNGT || op == UNLE || op == UNLT)
	return CC_FUNmode;

      if (op == EQ || op == NE)
	return CC_FPEQmode;

      return CC_FPmode;
    }

  if (op == GTU || op == LTU || op == GEU || op == LEU)
    return CCUUmode;

  return CCSSmode;
}

/* REVERSIBLE_CC_MODE.  */
int dadao_reversible_cc_mode (machine_mode mode)
{
  /* That is, all integer and the EQ, NE, ORDERED and UNORDERED float
     compares.  */
  return mode != CC_FPmode;
}

/* XXX gccint 18.16 Node: Describing Relative Costs of Operations */

static int dd_register_move_cost (machine_mode mode ATTRIBUTE_UNUSED,
			 reg_class_t from,
			 reg_class_t to)
{
	if ((from == GENERAL_REGS) && (to == GENERAL_REGS))	return 2;
	if ((from == GENERAL_REGS) || (to == GENERAL_REGS))	return 4;

	return 6;
}

#undef	TARGET_REGISTER_MOVE_COST
#define	TARGET_REGISTER_MOVE_COST		dd_register_move_cost

/* TARGET_RTX_COSTS.  */
static bool dd_rtx_costs (rtx x ATTRIBUTE_UNUSED,
		machine_mode mode ATTRIBUTE_UNUSED,
		int outer_code ATTRIBUTE_UNUSED,
		int opno ATTRIBUTE_UNUSED,
		int *total ATTRIBUTE_UNUSED,
		bool speed ATTRIBUTE_UNUSED)
{
  /* For the time being, this is just a stub and we'll accept the
     generic calculations, until we can do measurements, at least.
     Say we did not modify any calculated costs.  */
  return false;
}

#undef	TARGET_RTX_COSTS
#define	TARGET_RTX_COSTS			dd_rtx_costs

#undef	TARGET_ADDRESS_COST
#define	TARGET_ADDRESS_COST			hook_int_rtx_mode_as_bool_0

/* XXX gccint 18.17 Node: Adjusting the Instruction Scheduler */
/* (empty) */

/* XXX gccint 18.18 Node: Dividing the Output into Sections (Texts, Data, ...) */

static void dd_encode_section_info (tree decl, rtx rtl, int first)
{
  /* Test for an external declaration, and do nothing if it is one.  */
  if ((TREE_CODE (decl) == VAR_DECL
       && (DECL_EXTERNAL (decl) || TREE_PUBLIC (decl)))
      || (TREE_CODE (decl) == FUNCTION_DECL && TREE_PUBLIC (decl)))
    ;
  else if (first && DECL_P (decl))
    {
      const char *str = XSTR (XEXP (rtl, 0), 0);
      int len = strlen (str);
      char *newstr = XALLOCAVEC (char, len + 2);
      newstr[0] = '.';
      strcpy (newstr + 1, str);
      XSTR (XEXP (rtl, 0), 0) = ggc_alloc_string (newstr, len + 1);
    }

  /* Set SYMBOL_REF_FLAG for things that we want to access with GETA.  We
     may need different options to reach for different things with GETA.
     For now, functions and things we know or have been told are constant.  */
  if (TREE_CODE (decl) == FUNCTION_DECL
      || TREE_CONSTANT (decl)
      || (TREE_CODE (decl) == VAR_DECL
	  && TREE_READONLY (decl)
	  && !TREE_SIDE_EFFECTS (decl)
	  && (!DECL_INITIAL (decl)
	      || TREE_CONSTANT (DECL_INITIAL (decl)))))
    SYMBOL_REF_FLAG (XEXP (rtl, 0)) = 1;
}

#undef	TARGET_ENCODE_SECTION_INFO
#define	TARGET_ENCODE_SECTION_INFO		dd_encode_section_info

/* XXX gccint 18.19 Node: Position Independent Code */
/* (empty) */

/* XXX gccint 18.20 Node: Defining the Output Assembler Language */
/* XXX gccint 18.20.1 Node: The Overall Framework of an Assembler File */
/* (empty) */

/* XXX gccint 18.20.2 Node: Output of Data */

#undef	TARGET_ASM_ALIGNED_HI_OP
#define	TARGET_ASM_ALIGNED_HI_OP		"\t.dd.w16\t"
#undef	TARGET_ASM_ALIGNED_SI_OP
#define	TARGET_ASM_ALIGNED_SI_OP		"\t.dd.t32\t"
#undef	TARGET_ASM_ALIGNED_DI_OP
#define	TARGET_ASM_ALIGNED_DI_OP		"\t.dd.o64\t"

/* XXX gccint 18.20.3 Node: Output of Uninitialized Variables */
/* (empty) */

/* XXX gccint 18.20.4 Node: Output and Generation of Labels */
/* (empty) */

/* XXX gccint 18.20.5 Node: How Initialization Functions Are Handled */
/* (empty) */

/* XXX gccint 18.20.6 Node: Macros Controlling Initialization Routines */
/* (empty) */

/* XXX gccint 18.20.7 Node: Output of Assembler Instructions */

/* TARGET_PRINT_OPERAND.  */
static void dd_print_operand (FILE *stream, rtx x, int code)
{
  /* When we add support for different codes later, we can, when needed,
     drop through to the main handler with a modified operand.  */
  rtx modified_x = x;
  int regno = x != NULL_RTX && REG_P (x) ? REGNO (x) : 0;

  switch (code)
    {
      /* Unrelated codes are in alphabetic order.  */
    case 'B':
      if (GET_CODE (x) != CONST_INT)
	fatal_insn ("DADAO Internal: Expected a CONST_INT, not this", x);
      fprintf (stream, "%d", (int) (INTVAL (x) & 0xff));
      return;

    case 'i':
      dadao_output_shiftvalue_op_from_str (stream, "inc", (uint64_t) dadao_intval (x));
      return;

    case 's':
      dadao_output_shiftvalue_op_from_str (stream, "set", (uint64_t) dadao_intval (x));
      return;

    case 'd':
    case 'D':
      dadao_output_condition (stream, x, (code == 'D'));
      return;

    case 'e':
      /* Output an extra "e" to make fcmpe, fune.  */
      if (TARGET_FCMP_EPSILON)
	fprintf (stream, "e");
      return;

    case 'r':
      /* Store the register to output a constant to.  */
      if (! REG_P (x))
	fatal_insn ("DADAO Internal: Expected a register, not this", x);
      dadao_output_destination_register = DADAO_OUTPUT_REGNO (regno);
      return;

    case 'I':
      /* Output the constant.  Note that we use this for floats as well.  */
      if (GET_CODE (x) != CONST_INT && (GET_CODE (x) != CONST_DOUBLE
	      || (GET_MODE (x) != VOIDmode && GET_MODE (x) != DFmode
		  && GET_MODE (x) != SFmode)))
	fatal_insn ("DADAO Internal: Expected a constant, not this", x);
      dadao_output_register_setting (stream,
				    dadao_output_destination_register,
				    dadao_intval (x), 0);
      return;

    case 'v':
      dadao_output_shifted_value (stream, (int64_t) dadao_intval (x));
      return;

    case 'V':
      dadao_output_shifted_value (stream, (int64_t) ~dadao_intval (x));
      return;

    case 'W':
      if (GET_CODE (x) != CONST_INT)
	fatal_insn ("DADAO Internal: Expected a CONST_INT, not this", x);
      fprintf (stream, "0x%x", (int) (INTVAL (x) & 0xffff));
      return;

    case 0:
      /* Nothing to do.  */
      break;

    default:
      /* Presumably there's a missing case above if we get here.  */
      internal_error ("DADAO Internal: Missing %qc case in dd_print_operand", code);
    }

  switch (GET_CODE (modified_x))
    {
    case REG:
      regno = REGNO (modified_x);
      if (regno >= FIRST_PSEUDO_REGISTER)
	internal_error ("DADAO Internal: Bad register: %d", regno);
      fprintf (stream, "%s", reg_names[DADAO_OUTPUT_REGNO (regno)]);
      return;

    case MEM:
      output_address (GET_MODE (modified_x), XEXP (modified_x, 0));
      return;

	case CONST_INT:
		fprintf (stream, HOST_WIDE_INT_PRINT_DEC, INTVAL (modified_x));
		return;

    case CONST_DOUBLE:
      /* Do somewhat as CONST_INT.  */
      dadao_output_octa (stream, dadao_intval (modified_x), 0);
      return;

    case CONST:
      output_addr_const (stream, modified_x);
      return;

    default:
      /* No need to test for all strange things.  Let output_addr_const do
	 it for us.  */
      if (CONSTANT_P (modified_x)
	  /* Strangely enough, this is not included in CONSTANT_P.
	     FIXME: Ask/check about sanity here.  */
	  || LABEL_P (modified_x))
	{
	  output_addr_const (stream, modified_x);
	  return;
	}

      /* We need the original here.  */
      fatal_insn ("DADAO Internal: Cannot decode this operand", x);
    }
}

#undef	TARGET_PRINT_OPERAND
#define	TARGET_PRINT_OPERAND				dd_print_operand

/* TARGET_PRINT_OPERAND_ADDRESS.  */
static void dd_print_operand_address (FILE *stream, machine_mode /*mode*/, rtx x)
{
  if (REG_P (x))
    {
      /* I find the generated assembly code harder to read without the ",0".  */
      fprintf (stream, "%s, 0", reg_names[DADAO_OUTPUT_REGNO (REGNO (x))]);
      return;
    }
  else if (GET_CODE (x) == PLUS)
    {
      rtx x1 = XEXP (x, 0);
      rtx x2 = XEXP (x, 1);

      if (REG_P (x1))
	{
	  fprintf (stream, "%s, ", reg_names[DADAO_OUTPUT_REGNO (REGNO (x1))]);

	  if (REG_P (x2))
	    {
	      fprintf (stream, "%s", reg_names[DADAO_OUTPUT_REGNO (REGNO (x2))]);
	      return;
	    }
	  else if (satisfies_constraint_Id (x2))
	    {
	      output_addr_const (stream, x2);
	      return;
	    }
	}
    }

  fatal_insn ("DADAO Internal: This is not a recognized address", x);
}

#undef	TARGET_PRINT_OPERAND_ADDRESS
#define	TARGET_PRINT_OPERAND_ADDRESS			dd_print_operand_address

/* XXX gccint 18.20.8 Node: Output of Dispatch Tables */
/* (empty) */

/* XXX gccint 18.20.9 Node: Assembler Commands for Exception Regions */
/* (empty) */

/* XXX gccint 18.20.10 Node: Assembler Commands for Alignment */
/* (empty) */

/* XXX gccint 18.21 Node: Controlling Debugging Information Format */
/* (empty) */

/* XXX gccint 18.22 Node: Cross Compilation and Floating Point */
/* (empty) */

/* XXX gccint 18.23 Node: Mode Switching Instructions */
/* (empty) */

/* XXX gccint 18.24 Node: Defining target-specific uses of __attribute__ */

static void dd_option_override (void)
{
	/* Should we err or should we warn?  Hmm.  At least we must neutralize
	   it.  For example the wrong kind of case-tables will be generated with
	   PIC; we use absolute address items for dadaoal compatibility.  FIXME:
	   They could be relative if we just elide them to after all pertinent
	   labels.  */
	if (flag_pic) {
		warning (0, "%<-f%s%> not supported: ignored", (flag_pic > 1) ? "PIC" : "pic");
		flag_pic = 0;
	}
}

#undef	TARGET_OPTION_OVERRIDE
#define	TARGET_OPTION_OVERRIDE				dd_option_override

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
/* (empty) */

/* XXX End of gccint Chapter 18: Target Description Macros and Functions */


/* Wrapper for get_hard_reg_initial_val since integrate.h isn't included
   from insn-emit.c.  */

rtx
dadao_get_hard_reg_initial_val (machine_mode mode, int regno)
{
  return get_hard_reg_initial_val (mode, regno);
}

/* Nonzero when the function epilogue is simple enough that a single
   "POP %d,0" should be used even within the function.  */

int
dadao_use_simple_return (void)
{
  int regno;

  int stack_space_to_allocate
    = (crtl->outgoing_args_size
       + crtl->args.pretend_args_size
       + get_frame_size () + 7) & ~7;

  if (!reload_completed)
    return 0;

  if (frame_pointer_needed)
    stack_space_to_allocate += 8;

  if (DADAO_CFUN_HAS_LANDING_PAD)
    stack_space_to_allocate += 16;

  return stack_space_to_allocate == 0;
}


/* Expands the function prologue into RTX.  */

void
dadao_expand_prologue (void)
{
  HOST_WIDE_INT locals_size = get_frame_size ();
  int regno;
  HOST_WIDE_INT stack_space_to_allocate
    = (crtl->outgoing_args_size
       + crtl->args.pretend_args_size
       + locals_size + 7) & ~7;
  HOST_WIDE_INT offset = -8;

  /* If we do have a frame-pointer, add room for it.  */
  if (frame_pointer_needed)
    stack_space_to_allocate += 8;

  /* If we have a non-local label, we need to be able to unwind to it, so
     store the current register stack pointer.  Also store the return
     address if we do that.  */
  if (DADAO_CFUN_HAS_LANDING_PAD)
    stack_space_to_allocate += 16;

  /* Make sure we don't get an unaligned stack.  */
  if ((stack_space_to_allocate % 8) != 0)
    internal_error ("stack frame not a multiple of 8 bytes: %wd",
		    stack_space_to_allocate);

  if (crtl->args.pretend_args_size)
    {
      int dadao_first_vararg_reg
	= (DADAO_FIRST_ARG_REGNUM
	   + (DADAO_MAX_ARGS_IN_REGS
	      - crtl->args.pretend_args_size / 8));

      for (regno
	     = DADAO_FIRST_ARG_REGNUM + DADAO_MAX_ARGS_IN_REGS - 1;
	   regno >= dadao_first_vararg_reg;
	   regno--)
	{
	  if (offset < 0)
	    {
	      HOST_WIDE_INT stack_chunk
		= stack_space_to_allocate > (256 - 8)
		? (256 - 8) : stack_space_to_allocate;

	      dadao_emit_sp_add (-stack_chunk);
	      offset += stack_chunk;
	      stack_space_to_allocate -= stack_chunk;
	    }

	  /* These registers aren't actually saved (as in "will be
	     restored"), so don't tell DWARF2 they're saved.  */
	  emit_move_insn (gen_rtx_MEM (DImode,
				       plus_constant (Pmode, stack_pointer_rtx,
						      offset)),
			  gen_rtx_REG (DImode, regno));
	  offset -= 8;
	}
    }

  /* Store the frame-pointer.  */

  if (frame_pointer_needed)
    {
      rtx insn;

      if (offset < 0)
	{
	  /* Get 8 less than otherwise, since we need to reach offset + 8.  */
	  HOST_WIDE_INT stack_chunk
	    = stack_space_to_allocate > (256 - 8 - 8)
	    ? (256 - 8 - 8) : stack_space_to_allocate;

	  dadao_emit_sp_add (-stack_chunk);

	  offset += stack_chunk;
	  stack_space_to_allocate -= stack_chunk;
	}

      insn = emit_move_insn (gen_rtx_MEM (DImode,
					  plus_constant (Pmode,
							 stack_pointer_rtx,
							 offset)),
			     hard_frame_pointer_rtx);
      RTX_FRAME_RELATED_P (insn) = 1;
      insn = emit_insn (gen_adddi3 (hard_frame_pointer_rtx,
				    stack_pointer_rtx,
				    GEN_INT (offset + 8)));
      RTX_FRAME_RELATED_P (insn) = 1;
      offset -= 8;
    }

  if (DADAO_CFUN_HAS_LANDING_PAD)
    offset -= 8;

  if (DADAO_CFUN_HAS_LANDING_PAD)
    {
      /* Store the register defining the numbering of local registers, so
	 we know how long to unwind the register stack.  */

      if (offset < 0)
	{
	  /* Get 8 less than otherwise, since we need to reach offset + 8.  */
	  HOST_WIDE_INT stack_chunk
	    = stack_space_to_allocate > (256 - 8 - 8)
	    ? (256 - 8 - 8) : stack_space_to_allocate;

	  dadao_emit_sp_add (-stack_chunk);

	  offset += stack_chunk;
	  stack_space_to_allocate -= stack_chunk;
	}

      /* We don't tell dwarf2 about this one; we just have it to unwind
	 the register stack at landing pads.  FIXME: It's a kludge because
	 we can't describe the effect of the PUSHJ and PUSHGO insns on the
	 register stack at the moment.  Best thing would be to handle it
	 like stack-pointer offsets.  Better: some hook into dwarf2out.c
	 to produce DW_CFA_expression:s that specify the increment of rO,
	 and unwind it at eh_return (preferred) or at the landing pad.
	 Then saves to $0..$G-1 could be specified through that register.

      emit_move_insn (gen_rtx_REG (DImode, 63),
		      gen_rtx_REG (DImode,
				   DADAO_rO_REGNUM));
      emit_move_insn (gen_rtx_MEM (DImode,
				   plus_constant (Pmode, stack_pointer_rtx,
						  offset)),
		      gen_rtx_REG (DImode, 63));
      offset -= 8;  */
    }

  /* After the return-address and the frame-pointer, we have the local
     variables.  They're the ones that may have an "unaligned" size.  */
  offset -= (locals_size + 7) & ~7;

  /* Finally, allocate room for outgoing args and local vars if room
     wasn't allocated above.  */
  if (stack_space_to_allocate)
    dadao_emit_sp_add (-stack_space_to_allocate);
}

/* Expands the function epilogue into RTX.  */

void
dadao_expand_epilogue (void)
{
  HOST_WIDE_INT locals_size = get_frame_size ();
  int regno;
  HOST_WIDE_INT stack_space_to_deallocate
    = (crtl->outgoing_args_size
       + crtl->args.pretend_args_size
       + locals_size + 7) & ~7;

  /* The first address to access is beyond the outgoing_args area.  */
  HOST_WIDE_INT offset = crtl->outgoing_args_size;

  /* Add in the space for register stack-pointer.  If so, always add room
     for the saved PC.  */
  if (DADAO_CFUN_HAS_LANDING_PAD)
    stack_space_to_deallocate += 16;

  /* Add in the frame-pointer.  */
  if (frame_pointer_needed)
    stack_space_to_deallocate += 8;

  /* Make sure we don't get an unaligned stack.  */
  if ((stack_space_to_deallocate % 8) != 0)
    internal_error ("stack frame not a multiple of octabyte: %wd",
		    stack_space_to_deallocate);

  /* Here is where the local variables were.  As in the prologue, they
     might be of an unaligned size.  */
  offset += (locals_size + 7) & ~7;

  /* The saved register stack pointer is just below the frame-pointer
     register.  We don't need to restore it "manually"; the POP
     instruction does that.  */
  if (DADAO_CFUN_HAS_LANDING_PAD)
    offset += 16;

  /* Get back the old frame-pointer-value.  */
  if (frame_pointer_needed)
    {
      if (offset > 255)
	{
	  dadao_emit_sp_add (offset);

	  stack_space_to_deallocate -= offset;
	  offset = 0;
	}

      emit_move_insn (hard_frame_pointer_rtx,
		      gen_rtx_MEM (DImode,
				   plus_constant (Pmode, stack_pointer_rtx,
						  offset)));
      offset += 8;
    }

  /* We do not need to restore pretended incoming args, just add back
     offset to sp.  */
  if (stack_space_to_deallocate != 0)
    dadao_emit_sp_add (stack_space_to_deallocate);

}

/* Output an optimal sequence for setting a register to a specific
   constant.  Used in an alternative for const_ints in movdi, and when
   using large stack-frame offsets.

   Use do_begin_end to say if a line-starting TAB and newline before the
   first insn and after the last insn is wanted.  */

void
dadao_output_register_setting (FILE *stream,
			      int regno,
			      int64_t value,
			      int do_begin_end)
{
  if (do_begin_end)
    fprintf (stream, "\t");

  if (insn_const_int_ok_for_constraint (value, CONSTRAINT_Nd))
    fprintf (stream, "subu	%s, zero, %" PRId64, reg_names[regno], -value);
  else if (dadao_shiftable_wyde_value ((uint64_t) value))
    {
      /* First, the one-insn cases.  */
      dadao_output_shiftvalue_op_from_str (stream, "set",
					  (uint64_t)
					  value);
      fprintf (stream, "\t%s, ", reg_names[regno]);
      dadao_output_shifted_value (stream, (uint64_t) value);
    }
  else if (dadao_shiftable_wyde_value (-(uint64_t) value))
    {
      /* We do this to get a bit more legible assembly code.  The next
	 alternative is mostly redundant with this.  */

      dadao_output_shiftvalue_op_from_str (stream, "set",
					  -(uint64_t)
					  value);
      fprintf (stream, " %s,", reg_names[regno]);
      dadao_output_shifted_value (stream, -(uint64_t) value);
      fprintf (stream, "\n\tsubu	%s, zero, %s", reg_names[regno],
	       reg_names[regno]);
    }
  else if (dadao_shiftable_wyde_value (~(uint64_t) value))
    {
      /* Slightly more expensive, the two-insn cases.  */

      /* FIXME: We could of course also test if 0..255-N or ~(N | 1..255)
	 is shiftable, or any other one-insn transformation of the value.
	 FIXME: Check first if the value is "shiftable" by two loading
	 with two insns, since it makes more readable assembly code (if
	 anyone else cares).  */

      dadao_output_shiftvalue_op_from_str (stream, "set",
					  ~(uint64_t)
					  value);
      fprintf (stream, "\t%s,", reg_names[regno]);
      dadao_output_shifted_value (stream, ~(uint64_t) value);
      fprintf (stream, "\n\tnot	%s, %s, 0", reg_names[regno],
	       reg_names[regno]);
    }
  else
    {
	fprintf (stream, "seto	%s, ", reg_names[regno]);
	dadao_output_octa (stream, value, 0);
    }

  if (do_begin_end)
    fprintf (stream, "\n");
}

/* Return 1 if value is 0..65535*2**(16*N) for N=0..3.
   else return 0.  */

int
dadao_shiftable_wyde_value (uint64_t value)
{
  /* Shift by 16 bits per group, stop when we've found two groups with
     nonzero bits.  */
  int i;
  int has_candidate = 0;

  for (i = 0; i < 4; i++)
    {
      if (value & 65535)
	{
	  if (has_candidate)
	    return 0;
	  else
	    has_candidate = 1;
	}

      value >>= 16;
    }

  return 1;
}

/* X and Y are two things to compare using CODE.  Return the rtx for
   the cc-reg in the proper mode.  */

rtx
dadao_gen_compare_reg (RTX_CODE code, rtx x, rtx y)
{
  machine_mode ccmode = SELECT_CC_MODE (code, x, y);
  return gen_reg_rtx (ccmode);
}

/* Local (static) helper functions.  */

static void
dadao_emit_sp_add (HOST_WIDE_INT offset)
{
  rtx insn;

  if (offset < 0)
    {
      /* Negative stack-pointer adjustments are allocations and appear in
	 the prologue only.  We mark them as frame-related so unwind and
	 debug info is properly emitted for them.  */
      if (offset > -255)
	insn = emit_insn (gen_adddi3 (stack_pointer_rtx,
				      stack_pointer_rtx,
				      GEN_INT (offset)));
      else
	{
	  rtx tmpr = gen_rtx_REG (DImode, 63);
	  RTX_FRAME_RELATED_P (emit_move_insn (tmpr, GEN_INT (offset))) = 1;
	  insn = emit_insn (gen_adddi3 (stack_pointer_rtx,
					stack_pointer_rtx, tmpr));
	}
      RTX_FRAME_RELATED_P (insn) = 1;
    }
  else
    {
      /* Positive adjustments are in the epilogue only.  Don't mark them
	 as "frame-related" for unwind info.  */
      if (insn_const_int_ok_for_constraint (offset, CONSTRAINT_Iw))
	emit_insn (gen_adddi3 (stack_pointer_rtx,
			       stack_pointer_rtx,
			       GEN_INT (offset)));
      else
	{
	  rtx tmpr = gen_rtx_REG (DImode, 63);
	  emit_move_insn (tmpr, GEN_INT (offset));
	  insn = emit_insn (gen_adddi3 (stack_pointer_rtx,
					stack_pointer_rtx, tmpr));
	}
    }
}

/* Print operator suitable for doing something with a shiftable
   wyde.  The type of operator is passed as an asm output modifier.  */

static void
dadao_output_shiftvalue_op_from_str (FILE *stream,
				    const char *mainop,
				    int64_t value)
{
  static const char *const op_part[] = {"wl", "wk", "wj", "wh"};
  int i;

  if (! dadao_shiftable_wyde_value (value))
    {
      char s[sizeof ("0xffffffffffffffff")];
      sprintf (s, "%#" PRIx64, value);
      internal_error ("DADAO Internal: %s is not a shiftable int", s);
    }

  for (i = 0; i < 4; i++)
    {
      /* We know we're through when we find one-bits in the low
	 16 bits.  */
      if (value & 0xffff)
	{
	  fprintf (stream, "%s%s", mainop, op_part[i]);
	  return;
	}
      value >>= 16;
    }

  /* No bits set?  Then it must have been zero.  */
  fprintf (stream, "%swl", mainop);
}

/* Print a 64-bit value, optionally prefixed by assembly pseudo.  */

static void
dadao_output_octa (FILE *stream, int64_t value, int do_begin_end)
{
  if (do_begin_end)
    fprintf (stream, "\t.dd.o64 ");

  /* Provide a few alternative output formats depending on the number, to
     improve legibility of assembler output.  */
  if ((value < (int64_t) 0 && value > (int64_t) -10000)
      || (value >= (int64_t) 0 && value <= (int64_t) 16384))
    fprintf (stream, "%d", (int) value);
  else if (value > (int64_t) 0
	   && value < ((int64_t) 1 << 31) * 2)
    fprintf (stream, "0x%x", (unsigned int) value);
  else if (sizeof (HOST_WIDE_INT) == sizeof (int64_t))
    /* We need to avoid the not-so-universal "0x" prefix; we need the
       pure hex-digits together with the dadaoal "#" hex prefix. DADAO changed */
    fprintf (stream, "0x" HOST_WIDE_INT_PRINT_HEX_PURE,
	     (HOST_WIDE_INT) value);
  else /* Need to avoid the hex output; there's no ...WIDEST...HEX_PURE.  */
    fprintf (stream, "%" PRIu64, value);

  if (do_begin_end)
    fprintf (stream, "\n");
}

/* Print the presumed shiftable wyde argument shifted into place (to
   be output with an operand).  */

static void
dadao_output_shifted_value (FILE *stream, int64_t value)
{
  int i;

  if (! dadao_shiftable_wyde_value (value))
    {
      char s[16+2+1];
      sprintf (s, "%#" PRIx64, value);
      internal_error ("DADAO Internal: %s is not a shiftable int", s);
    }

  for (i = 0; i < 4; i++)
    {
      /* We know we're through when we find one-bits in the low 16 bits.  */
      if (value & 0xffff)
	{
	  fprintf (stream, "0x%x", (int) (value & 0xffff));
	  return;
	}

    value >>= 16;
  }

  /* No bits set?  Then it must have been zero.  */
  fprintf (stream, "0");
}

/* Output an DADAO condition name corresponding to an operator
   and operands:
   (comparison_operator [(comparison_operator ...) (const_int 0)])
   which means we have to look at *two* operators.

   The argument "reversed" refers to reversal of the condition (not the
   same as swapping the arguments).  */

static void
dadao_output_condition (FILE *stream, const_rtx x, int reversed)
{
  struct cc_conv
  {
    RTX_CODE cc;

    /* The normal output cc-code.  */
    const char *const normal;

    /* The reversed cc-code, or NULL if invalid.  */
    const char *const reversed;
  };

  struct cc_type_conv
  {
    machine_mode cc_mode;

    /* Terminated with {UNKNOWN, NULL, NULL} */
    const struct cc_conv *const convs;
  };

#undef CCEND
#define CCEND {UNKNOWN, NULL, NULL}

  static const struct cc_conv cc_fun_convs[]
    = {{ORDERED, "Z", "P"},
       {UNORDERED, "P", "Z"},
       CCEND};
  static const struct cc_conv cc_fp_convs[]
    = {{GT, "P", NULL},
       {LT, "N", NULL},
       CCEND};
  static const struct cc_conv cc_fpeq_convs[]
    = {{NE, "Z", "P"},
       {EQ, "P", "Z"},
       CCEND};
  static const struct cc_conv cc_uns_convs[]
    = {{GEU, "NN", "N"},
       {GTU, "P", "NP"},
       {LEU, "NP", "P"},
       {LTU, "N", "NN"},
       CCEND};
  static const struct cc_conv cc_signed_convs[]
    = {{NE, "NZ", "Z"},
       {EQ, "Z", "NZ"},
       {GE, "NN", "N"},
       {GT, "P", "NP"},
       {LE, "NP", "P"},
       {LT, "N", "NN"},
       CCEND};
  static const struct cc_conv cc_di_convs[]
    = {{NE, "NZ", "Z"},
       {EQ, "Z", "NZ"},
       {GE, "NN", "N"},
       {GT, "P", "NP"},
       {LE, "NP", "P"},
       {LT, "N", "NN"},
       {GTU, "NZ", "Z"},
       {LEU, "Z", "NZ"},
       CCEND};
#undef CCEND

  static const struct cc_type_conv cc_convs[]
    = {{E_CC_FUNmode, cc_fun_convs},
       {E_CC_FPmode, cc_fp_convs},
       {E_CC_FPEQmode, cc_fpeq_convs},
       {E_CCUUmode, cc_uns_convs},
       {E_CCSSmode, cc_signed_convs},
       {E_DImode, cc_di_convs}};

  size_t i;
  int j;

  machine_mode mode = GET_MODE (XEXP (x, 0));
  RTX_CODE cc = GET_CODE (x);

  for (i = 0; i < ARRAY_SIZE (cc_convs); i++)
    {
      if (mode == cc_convs[i].cc_mode)
	{
	  for (j = 0; cc_convs[i].convs[j].cc != UNKNOWN; j++)
	    if (cc == cc_convs[i].convs[j].cc)
	      {
		const char *dadao_cc
		  = (reversed ? cc_convs[i].convs[j].reversed
		     : cc_convs[i].convs[j].normal);

		if (dadao_cc == NULL)
		  fatal_insn ("DADAO Internal: Trying to output invalidly\
 reversed condition:", x);

		fprintf (stream, "%s", dadao_cc);
		return;
	      }

	  fatal_insn ("DADAO Internal: What's the CC of this?", x);
	}
    }

  fatal_insn ("DADAO Internal: What is the CC of this?", x);
}

/* Return the bit-value for a const_int or const_double.  */

int64_t
dadao_intval (const_rtx x)
{
  if (GET_CODE (x) == CONST_INT)
    return INTVAL (x);

  /* We make a little song and dance because converting to long long in
     gcc-2.7.2 is broken.  I still want people to be able to use it for
     cross-compilation to DADAO.  */
  if (GET_CODE (x) == CONST_DOUBLE && GET_MODE (x) == VOIDmode)
    return CONST_DOUBLE_HIGH (x);

  if (GET_CODE (x) == CONST_DOUBLE)
    {
      if (GET_MODE (x) == DFmode)
	{
	  long bits[2];

	  REAL_VALUE_TO_TARGET_DOUBLE (*CONST_DOUBLE_REAL_VALUE (x), bits);

	  /* The double cast is necessary to avoid getting the long
	     sign-extended to unsigned long long(!) when they're of
	     different size (usually 32-bit hosts).  */
	  return
	    ((uint64_t) (unsigned long) bits[0]
	     << (uint64_t) 32U)
	    | (uint64_t) (unsigned long) bits[1];
	}
      else if (GET_MODE (x) == SFmode)
	{
	  long bits;
	  REAL_VALUE_TO_TARGET_SINGLE (*CONST_DOUBLE_REAL_VALUE (x), bits);

	  return (unsigned long) bits;
	}
    }

  fatal_insn ("DADAO Internal: This is not a constant:", x);
}

/* XXX gccint 18.1 Node: The Global targetm Variable */
/* gcc_target MUST be at the end of this file, since TARGET_* macros will be used in TARGET_INITIALIZER */
struct gcc_target targetm = TARGET_INITIALIZER;

/*
 * Local variables:
 * eval: (c-set-style "gnu")
 * indent-tabs-mode: t
 * End:
 */
