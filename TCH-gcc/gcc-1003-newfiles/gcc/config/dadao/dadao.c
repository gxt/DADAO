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

/* For the default ABI, we rename registers at output-time to fill the gap
   between the (statically partitioned) saved registers and call-clobbered
   registers.  In effect this makes unused call-saved registers to be used
   as call-clobbered registers.  The benefit comes from keeping the number
   of local registers (value of rL) low, since there's a cost of
   increasing rL and clearing unused (unset) registers with lower numbers.
   Don't translate while outputting the prologue.  */
#define DADAO_OUTPUT_REGNO(N)	(N)

/* Declarations of locals.  */
/* Intermediate for insn output.  */
static void dadao_output_octa (FILE *, int64_t, int);

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

static bool dd_hard_regno_mode_ok (unsigned int regno, machine_mode mode)
{
	if (REGNO_REG_CLASS(regno) == POINTER_REGS)
		return (mode == Pmode);

	if (REGNO_REG_CLASS(regno) == FLOATING_REGS)
		return (GET_MODE_CLASS (mode) == MODE_FLOAT);

	return (REGNO_REG_CLASS(regno) == GENERAL_REGS);
}

#undef	TARGET_HARD_REGNO_MODE_OK
#define	TARGET_HARD_REGNO_MODE_OK		dd_hard_regno_mode_ok

/* XXX gccint 18.8 Node: Register Classes */

static reg_class_t dd_secondary_reload (bool in_p ATTRIBUTE_UNUSED,
		rtx x ATTRIBUTE_UNUSED,
		reg_class_t rclass ATTRIBUTE_UNUSED,
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

/* RETURN_ADDR_RTX.  */
rtx dadao_return_addr_rtx (int count, rtx frame ATTRIBUTE_UNUSED)
{
  return get_hard_reg_initial_val (Pmode, DADAO_RETURN_ADDRESS_REGNUM - count);
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
  if ((!crtl->is_leaf) || (get_frame_size()!=0)
     || (cfun->stdarg) || (!crtl->sp_is_unchanging)
     || (profile_flag))
    return true;

  return false;
}

#undef	TARGET_FRAME_POINTER_REQUIRED
#define	TARGET_FRAME_POINTER_REQUIRED			dd_frame_pointer_required

/* The difference between the (imaginary) frame pointer and the stack
   pointer.  Used to eliminate the frame pointer.  */

/* for DADAO architecture:
 * |	    ....	 |
 * |---------------------| <--{old "stack" pointer}
 * | old "frame" pointer | := [optional] if frame_pointer_required
 * |---------------------| := "rd32"-"rd63" "rb32"-"rb63"
 * |  callee saved regs  |    "rf32"-"rf63"
 * |---------------------|
 * |			 | := ["stdargs.h"] ["varargs.h"]
 * |  variable argument  |    args which must pass on the stack
 * |			 |
 * |---------------------| <--{new "arg" pointer} (eliminated "sp"|"fp")
 * |---------------------| <--{new "frame" pointer} (same as "arg")
 * |  incoming argument  |
 * |---------------------||---
 * |   local variables   |    \
 * |---------------------||----==> := "get_frame_size()"
 * |  caller saved regs  |    /
 * |---------------------||---
 * |  outgoing argument  | := optional "outgoing_args_size"
 * |---------------------| <--{new "stack" pointer}
 *
 * FIXME ???
 * Only {ARG_POINTER}   to {STACK_POINTER}
 *      {ARG_POINTER}   to {FRAME_POINTER}
 *      {FRAME_POINTER} to {STACK_POINTER}
 * can be eliminated.
 */
int dadao_initial_elimination_offset (int fromreg, int toreg)
{
  if (fromreg == DADAO_ARG_POINTER_REGNUM)
    return 0;

  /* hard_frame_pointer = frame_pointer
   *
   * We are going to store the local variables
   * as well as outgoing args and caller-saved
   * registers. They can be computed by adding
   * the length of outgoing_args_size & fsize.
   */
  int fp_sp_offset = (crtl->outgoing_args_size
			+ get_frame_size() + 7) & ~7;
  return fp_sp_offset;
}

/* XXX gccint 18.9.6 Node: Passing Function Arguments on the Stack */
/* (empty) */

/* XXX gccint 18.9.7 Node: Passing Arguments in Registers */

bool dd_can_eliminate (const int from_reg, const int to_reg)
{
  return (from_reg == ARG_POINTER_REGNUM && to_reg == STACK_POINTER_REGNUM)
	  ? ! frame_pointer_needed
	  : true;
}

#undef TARGET_CAN_ELIMINATE
#define TARGET_CAN_ELIMINATE dd_can_eliminate

/* Return an rtx for a function argument to go in a register, and 0 for
   one that must go on stack.  */
static rtx dd_function_arg (cumulative_args_t argsp_v,
			    const function_arg_info &arg)
{
  CUMULATIVE_ARGS *argsp = get_cumulative_args (argsp_v);
  unsigned int DADAO_ARG_REGNUM_P = DADAO_FIRST_ARG_REGNUM;
  unsigned int DADAO_ARG_REGNUM_M = DADAO_MAX_ARGS_IN_REGS;

  if (arg.mode == VOIDmode) return const0_rtx;

  /* Last-argument marker.  */
  if (arg.end_marker_p ())
    {
      if (argsp->regs < DADAO_ARG_REGNUM_M)
	return gen_rtx_REG (arg.mode, DADAO_ARG_REGNUM_P + argsp->regs);
      else
	internal_error ("internal compiler error: args overflow.");
    }

  return (argsp->regs < DADAO_ARG_REGNUM_M
	  && !targetm.calls.must_pass_in_stack (arg)
	  && (GET_MODE_BITSIZE (arg.mode) <= 64
	      || argsp->lib
	      || TARGET_LIBFUNC))
    ? gen_rtx_REG (arg.mode, DADAO_ARG_REGNUM_P + argsp->regs)
    : NULL_RTX;
}

#undef	TARGET_FUNCTION_ARG
#define	TARGET_FUNCTION_ARG		dd_function_arg

/* Return true if we should pass an argument on the stack rather than
   in registers.  */

static bool
dd_must_pass_in_stack (const function_arg_info &arg)
{
  return arg.mode == BLKmode || arg.aggregate_type_p ();
}

#undef TARGET_MUST_PASS_IN_STACK
#define TARGET_MUST_PASS_IN_STACK	dd_must_pass_in_stack

/* Returns nonzero for everything that goes by reference, 0 for
   everything that goes by value.  */
static bool dd_pass_by_reference (cumulative_args_t argsp_v,
				  const function_arg_info &arg)
{
  CUMULATIVE_ARGS *argsp = get_cumulative_args (argsp_v);

  /* FIXME: Check: I'm not sure the must_pass_in_stack check is
     necessary.  */
  if (targetm.calls.must_pass_in_stack (arg))
    return true;

  if (DADAO_FUNCTION_ARG_SIZE (arg.mode, arg.type) > 8
      && !TARGET_LIBFUNC
      && (!argsp || !argsp->lib))
    return true;

  return false;
}

#undef	TARGET_PASS_BY_REFERENCE
#define	TARGET_PASS_BY_REFERENCE	dd_pass_by_reference

static void dd_function_arg_advance (cumulative_args_t argsp_v,
				     const function_arg_info &arg)
{
  CUMULATIVE_ARGS *argsp = get_cumulative_args (argsp_v);
  int arg_size = DADAO_FUNCTION_ARG_SIZE (arg.mode, arg.type);

  argsp->regs = ((targetm.calls.must_pass_in_stack (arg)
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
				       const function_arg_info &arg,
				       int *pretend_sizep,
				       int second_time)
{
  CUMULATIVE_ARGS *args_so_farp = get_cumulative_args (args_so_farp_v);

  dd_function_arg_advance (pack_cumulative_args (args_so_farp), arg);

  if (args_so_farp->regs < DADAO_MAX_ARGS_IN_REGS)
  {
    int first_reg_offset = args_so_farp->regs;
    if (!second_time)
    {
      rtx ptr = virtual_incoming_args_rtx;
      rtx mem = gen_rtx_MEM (BLKmode, ptr);
      emit_insn (gen_blockage ());

      move_block_from_reg (first_reg_offset + DADAO_FIRST_ARG_REGNUM,
		           mem, DADAO_MAX_ARGS_IN_REGS - first_reg_offset);
      emit_insn (gen_blockage ());
    }

    *pretend_sizep = (DADAO_MAX_ARGS_IN_REGS - (args_so_farp->regs)) * 8;
  }

  /* We assume that one argument takes up one register here.  That should
     be true until we start messing with multi-reg parameters.  */
  if ((7 + (DADAO_FUNCTION_ARG_SIZE (arg.mode, arg.type))) / 8 != 1)
    internal_error ("DADAO Internal: Last named vararg would not fit in a register");
}

#undef	TARGET_SETUP_INCOMING_VARARGS
#define	TARGET_SETUP_INCOMING_VARARGS		dd_setup_incoming_varargs

/* XXX gccint 18.11 Node: Support for Nested Functions */
/* (empty) */

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

#define	_DD_LEGITIMATE_ADDR_GENERAL(X, XC) (REG_P(X) && (REGNO_REG_CLASS(REGNO(X)) == XC))

static bool dd_judge_legitimate_address (rtx);

/* Return 1 if the address is OK, otherwise 0.  */
static bool dd_legitimate_address_p (machine_mode mode ATTRIBUTE_UNUSED,
			   rtx x, bool strict_checking)
{
#define	_DD_LEGITIMATE_ADDR_RC(X, XC)	( REG_P(X) &&				\
	( (REGNO_REG_CLASS(REGNO(X)) == XC) ||					\
	( !strict_checking && (REGNO (X) >= FIRST_PSEUDO_REGISTER)) ||		\
	( strict_checking && (REGNO_REG_CLASS(reg_renumber[REGNO(X)]) == XC))))

	/* (mem rb) */
	if (_DD_LEGITIMATE_ADDR_RC(x, POINTER_REGS))
		return 1;

	if (GET_CODE(x) != PLUS)
		return 0;

	rtx x1 = XEXP (x, 0);
	rtx x2 = XEXP (x, 1);

	/* (mem (plus (rb) (rd))) */
	if (_DD_LEGITIMATE_ADDR_RC(x1, POINTER_REGS) && _DD_LEGITIMATE_ADDR_RC(x2, GENERAL_REGS))
		return 1;

	/* (mem (plus (rb) (-0x800, 0x7FF))) */
	if (_DD_LEGITIMATE_ADDR_RC(x1, POINTER_REGS) && satisfies_constraint_Id (x2))
		return 1;

	/* (mem (plus (rd) (rb))) */
	if (_DD_LEGITIMATE_ADDR_RC(x2, POINTER_REGS) && _DD_LEGITIMATE_ADDR_RC(x1, GENERAL_REGS))
		return 1;

	/* (mem (plus (rb) (mem))) */
	if (((REGNO_REG_CLASS(REGNO(x1)) == POINTER_REGS) && (MEM_P(x2)))
	 || (_DD_LEGITIMATE_ADDR_GENERAL(x2, POINTER_REGS) && (MEM_P(x1))))
		return dd_judge_legitimate_address (MEM_P(x1) ? x1 : x2);

	return 0;
#undef	_DD_LEGITIMATE_ADDR_RC
}

#undef	TARGET_LEGITIMATE_ADDRESS_P
#define	TARGET_LEGITIMATE_ADDRESS_P		dd_legitimate_address_p

bool dd_load_legitimate_address_rbrd (rtx x)
{
	if (GET_CODE(x) != PLUS)
		return 0;
	rtx x1 = XEXP (x, 0);
	rtx x2 = XEXP (x, 1);
	if (_DD_LEGITIMATE_ADDR_GENERAL(x1, POINTER_REGS)
	 && _DD_LEGITIMATE_ADDR_GENERAL(x2, GENERAL_REGS))
		return 1;
	if (_DD_LEGITIMATE_ADDR_GENERAL(x2, POINTER_REGS)
	 && _DD_LEGITIMATE_ADDR_GENERAL(x1, GENERAL_REGS))
		return 1;
	return 0;
}

static
bool dd_judge_legitimate_address (rtx x)
{
	if (!MEM_P(x))
		return 0;

	rtx x1 = XEXP (x, 0);

	if (_DD_LEGITIMATE_ADDR_GENERAL(x1, POINTER_REGS))
		return 1;

	if (GET_CODE(x1) == PLUS)
	{
		rtx op0 = XEXP (x1, 0);
		rtx op1 = XEXP (x1, 1);

		if (_DD_LEGITIMATE_ADDR_GENERAL(op0, POINTER_REGS)
		 && _DD_LEGITIMATE_ADDR_GENERAL(op1, GENERAL_REGS))
			return 1;

		if (_DD_LEGITIMATE_ADDR_GENERAL(op1, POINTER_REGS)
		 && _DD_LEGITIMATE_ADDR_GENERAL(op0, GENERAL_REGS))
			return 1;

		if (_DD_LEGITIMATE_ADDR_GENERAL(op0, POINTER_REGS)
		 && satisfies_constraint_Id(op1))
			return 1;

		if ((_DD_LEGITIMATE_ADDR_GENERAL(op0, POINTER_REGS) && MEM_P(op1))
		 || (_DD_LEGITIMATE_ADDR_GENERAL(op1, POINTER_REGS) && MEM_P(op0)))
		{
			if (MEM_P(op0))
			{
				rtx tp = op1;
				op1 = op0;
				op0 = tp;
			}
			if (dd_judge_legitimate_address(op1))
				return 1;
		}
	}

	return 0;
}

#undef	_DD_LEGITIMATE_ADDR_RC

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
  /* We use CCSSmode, CCUUmode, CCFFmode to output different compare insns.
     Note that we do not check the validity of the comparison here.  */

  if (GET_MODE_CLASS (GET_MODE (x)) == MODE_FLOAT)
    return CCFFmode;

  if (op == GTU || op == LTU || op == GEU || op == LEU)
    return CCUUmode;

  return CCSSmode;
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
      newstr[0] = '_';
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
void dd_print_operand_address (FILE *stream, machine_mode /*mode*/, rtx x)
{
	if (GET_CODE (x) == CONST_INT) {
		/* (const) FIXME testsuite: gcc.c-torture/execute/multi-ix.c
		 * Fatal asm insn: call rbx, mem (const)
		 * Change (const): call rbx, rd0, const
		 */
		fprintf (stream, "%s, %d", reg_names[0], INTVAL(x));
		return;
	}
	if (REG_P (x)) {
		/* (mem rb) */
		fprintf (stream, "%s, 0", reg_names[DADAO_OUTPUT_REGNO (REGNO (x))]);
		return;
	} else if (GET_CODE (x) == PLUS) {
		rtx x1 = XEXP (x, 0);
		rtx x2 = XEXP (x, 1);

#define	_DD_REG_CLASS(X, XC)	( REG_P(X) && (REGNO_REG_CLASS(REGNO(X)) == XC))
		/* (mem (plus (rb) (rd))) */
		if (_DD_REG_CLASS(x1, POINTER_REGS) && _DD_REG_CLASS(x2, GENERAL_REGS)) {
			fprintf (stream, "%s, %s", reg_names[DADAO_OUTPUT_REGNO (REGNO (x1))],
						reg_names[DADAO_OUTPUT_REGNO (REGNO (x2))]);
			return;
		}

		/* (mem (plus (rd) (rb))) */
		if (_DD_REG_CLASS(x2, POINTER_REGS) && _DD_REG_CLASS(x1, GENERAL_REGS)) {
			fprintf (stream, "%s, %s", reg_names[DADAO_OUTPUT_REGNO (REGNO (x2))],
						reg_names[DADAO_OUTPUT_REGNO (REGNO (x1))]);
			return;
		}

		/* (mem (plus (rb) (-0x800, 0x7FF))) */
		if (_DD_REG_CLASS(x1, POINTER_REGS) && satisfies_constraint_Id (x2)) {
			fprintf (stream, "%s, ", reg_names[DADAO_OUTPUT_REGNO (REGNO (x1))]);
			output_addr_const (stream, x2);
			return;
		}
#undef	_DD_REG_CLASS
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
  /* Nothing more will have to be added in as override
   * optimization options for -O2 or for debugging now. */
  flag_pic = 0;
}

#undef	TARGET_OPTION_OVERRIDE
#define	TARGET_OPTION_OVERRIDE				dd_option_override

/* Fix newlib's ffs recursively calling itself.
 * Copied from gcc/config/mmix/mmix.c .
 * See commit e6bf23cce3ed63800f4545cd9a79e3500b24625c. */
static void
dadao_init_libfuncs (void)
{
  set_optab_libfunc (ffs_optab, SImode, "__ffsdi2");
}

#undef TARGET_INIT_LIBFUNCS
#define TARGET_INIT_LIBFUNCS dadao_init_libfuncs

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

/* XXX End of gccint Chapter 18: Target Description Macros and Functions */

/* Wrapper for get_hard_reg_initial_val since integrate.h isn't included
   from insn-emit.c.  */

rtx
dadao_get_hard_reg_initial_val (machine_mode mode, int regno)
{
  return get_hard_reg_initial_val (mode, regno);
}

int
dadao_use_simple_return (void)
{
  int stack_space_to_allocate
    = (crtl->outgoing_args_size
       + crtl->args.pretend_args_size
       + get_frame_size () + 7) & ~7;

  if (!reload_completed)
    return 0;

  if (frame_pointer_needed)
    stack_space_to_allocate += 8;

  return stack_space_to_allocate == 0;
}

/* Return whether REGNO a Callee-Saved or Fixed register which must be saved/restored. */

static inline bool
dadao_call_saved_reg_p (unsigned int regno)
{
  return !call_used_or_fixed_reg_p (regno) && df_regs_ever_live_p (regno);
}

/* Expands the function prologue into RTX.  */

void
dadao_expand_prologue (void)
{
  /* callee_saved_regs_p = number of Callee_Saved regs
   * which will have to be stored on the frame page.
   * */
  int callee_saved_regs_p = 0;
  int regs_saved_offsets  = 0;

  /* get_frame_size = argsize + varsize */
  HOST_WIDE_INT fsize = get_frame_size ();
  machine_mode mode;
  rtx x;

  /* Stored the old Frame Pointer register at the beginning
   * of the new Frame storage layout, if FP actually exists.
   * */
  if (frame_pointer_needed)
  {
    regs_saved_offsets -= 8;
    emit_move_insn (gen_rtx_MEM (Pmode,
			    	 plus_constant (Pmode,
						stack_pointer_rtx, regs_saved_offsets)),
		    frame_pointer_rtx);
  }

  /* Stored all used-Callee_Saved register beyond FP if FP exists.
   * If FP does not exist, stored these registers before the vars
   * or args are stored onto the stack by referencing SP as base.
   * */
  for (int rtype = 0; rtype < 3; rtype++)
  {
    for (unsigned regno = 32 + rtype * 64;
		  regno < 64 + rtype * 64; regno++)
    {
	if (dadao_call_saved_reg_p (regno))
        {
	  /* Float Callee_Saved rf must be stored under floatmode */
	  mode = rtype > 1 ? E_DFmode : E_DImode;

	  x = gen_rtx_MEM (mode,
			   plus_constant (Pmode,
				   	  stack_pointer_rtx,
					  regs_saved_offsets +
					  (-8 * ++callee_saved_regs_p)));
	  emit_move_insn (x, gen_rtx_REG (mode, regno));
        }
    }
  }

  /* SP will have to Increase 'SIZEOF(64/8) * CALLEE_SAVED_REGS_P'
   * to allocate space for the Callee_Saved registers. If FP exists,
   * FP will have to be the new SP, which means Callee_Saved regs
   * are stored beyond the Frame Pointer register.
   * */
  if (callee_saved_regs_p)
    regs_saved_offsets -= callee_saved_regs_p * UNITS_PER_WORD;

  if (crtl->args.pretend_args_size)
    regs_saved_offsets -= crtl->args.pretend_args_size;

  if (regs_saved_offsets)
    emit_insn (gen_adddi3 (stack_pointer_rtx,
			   stack_pointer_rtx, GEN_INT (regs_saved_offsets)));

  if (frame_pointer_needed)
  {
    emit_move_insn (frame_pointer_rtx, stack_pointer_rtx);
  }

  /* Make local variable space.  */
  if (fsize)
    emit_insn (gen_adddi3 (stack_pointer_rtx, stack_pointer_rtx,
			   GEN_INT (-fsize)));
}

/* Expands the function epilogue into RTX.  */

void
dadao_expand_epilogue (void)
{
  int callee_saved_regs_p = 0;
  int regs_saved_frame_offsets = 0;

  HOST_WIDE_INT fsize = get_frame_size ();
  machine_mode mode;
  rtx x, reg;

  /* Garbage collection: local variables and varargs */
  if (fsize + crtl->args.pretend_args_size)
  {
    regs_saved_frame_offsets += (fsize + crtl->args.pretend_args_size);
    emit_insn (gen_adddi3 (stack_pointer_rtx,
			   stack_pointer_rtx, GEN_INT (regs_saved_frame_offsets)));
  }

  /* We've stored all of the used-Callee_Saved registers in a
   * order that smaller the regno is, further the reg will be
   * stored from the new SP / FP during the function prologue.
   * So when we try to restored these reg, since the new SP is
   * the only address that we know, the only way to recovered
   * the context is to load these regs by scanning the reglist
   * backwards. Regs with the bigger regno will be stored first.
   * */
  for (int rtype = 2; rtype >= 0; --rtype)
  for (unsigned int regno = 63 + rtype * 64;
		    regno > 31 + rtype * 64; --regno)
    if (dadao_call_saved_reg_p (regno))
      {
	mode = rtype > 1 ? E_DFmode : E_DImode;

	/* NOTICE:
	 * offset from stack_pointer_rtx to the callee-saved
	 * register storing area may be too large for some
	 * function which contains many local variables to
	 * get stored. So we need to compute the offset first
	 */
	x = gen_rtx_MEM (mode,
			 plus_constant (Pmode, stack_pointer_rtx,
					(8 * callee_saved_regs_p++)));
	emit_move_insn (gen_rtx_REG (mode, regno), x);
      }

  /* Garbage collection: callee-saved registers */
  if (callee_saved_regs_p)
    emit_insn (gen_adddi3 (stack_pointer_rtx,
			   stack_pointer_rtx, GEN_INT (callee_saved_regs_p * 8)));

  if (frame_pointer_needed)
  {
    emit_insn (gen_rtx_SET (frame_pointer_rtx,
			    gen_rtx_MEM (Pmode, stack_pointer_rtx)));
    emit_insn (gen_adddi3 (stack_pointer_rtx,
			   stack_pointer_rtx, GEN_INT (8)));
  }
}

/* X and Y are two things to compare using CODE.  Return the rtx for
   the cc-reg in the proper mode.  */

rtx
dadao_gen_compare_reg (RTX_CODE code, rtx x, rtx y)
{
  machine_mode ccmode = SELECT_CC_MODE (code, x, y);
  return gen_reg_rtx (ccmode);
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

/* This function returns the container for indirect memory access.
 * This function is going to print the mem operand which acted as
 * an offset during the indirect memory access procedure. It will
 * use SRC as the memory operand while DEST as the data container. */

static rtx
dadao_print_indirect_operand (rtx dest, rtx src)
{
  FILE *stream = asm_out_file;

  gcc_assert (MEM_P (src));

  rtx addr = XEXP (src, 0);

  if (REG_P (addr))
  {
    asm_fprintf (stream, "\tldo\t%s, %s, 0",
		 reg_names[REGNO(dest)],
		 reg_names[REGNO(addr)]);
    return dest;
  }
  if (GET_CODE (addr) == PLUS)
  {
    rtx op0 = XEXP (addr, 0);
    rtx op1 = XEXP (addr, 1);

    if (MEM_P (op1))	/* Indirect's indirect. */
    {			/* rd7 as IP clobber rd */
      rtx reg;		/* can be used >=1 time */
      reg = gen_rtx_REG (Pmode, 7);
      reg = dadao_print_indirect_operand (reg, op1);
      op1 = reg;
    }

    if (REG_P (op1))
    {
      asm_fprintf (stream,
	    "\tldmo\t%s, %s, %s, 1\n",
	      reg_names[REGNO(dest)],
	      reg_names[REGNO(op0)], reg_names[REGNO(op1)]);
    }
    else if (CONST_INT_P (op1))
    {
      asm_fprintf (stream,
	    "\tldo\t%s, %s, %d\n",
	      reg_names[REGNO(dest)],
	      reg_names[REGNO(op0)], INTVAL(op1));
    }
    else
      goto err;

    return dest;
  }
err:
  gcc_unreachable ();
  return NULL_RTX;
}

/*
 * E_QImode = [ (7 % 4) == 3 ] --> b
 * E_HImode = [ (8 % 4) == 0 ] --> w
 * E_SImode = [ (9 % 4) == 1 ] --> t
 * E_DImode = [(10 % 4) == 2 ] --> o
 */
static const char * const ldst_mmodes[] =
{ "w", "t", "o", "b" };

/* This function returns the assembly output for load / store operations.
 * MODE is the machine mode of this ld/st op pair; DEST is destination op
 * SRC is source op; STRICT is to seperate the loading / storing pattern.
 * When strict is true then the function is for loading, otherwise for storing. */

const char*
dadao_print_ldst_operand (machine_mode mode,
			  rtx dest, rtx src, bool strict)
{
  FILE *stream = asm_out_file;
#define _DD_SWAP_OP_(OP0, OP1) ((REG_P (OP0) && REG_P (OP1))		\
		  && (REGNO_REG_CLASS(REGNO(OP0)) == GENERAL_REGS)	\
		  && (REGNO_REG_CLASS(REGNO(OP1)) == POINTER_REGS))

  /* This function handles Floating 'load' / 'store' instructions,
   * whose non-memory operation is data-type register, with normal
   * ldo / ldmo / sto / stmo assembly instructions. So Float modes
   * here are all treated as E_DImode. */
  
  if (GET_MODE_CLASS (mode) \
		== MODE_FLOAT) mode = E_DImode;
  if (strict)
    {
      const char * ldst_suffix = \
	      (mode == E_DImode) ? "" : "u";

      rtx addr = XEXP (src, 0);

      if (REG_P (addr))
        {
	  asm_fprintf (stream, "\tld%s%s",
		       ldst_mmodes [mode % 4],
		       ldst_suffix);

	  return "%0, %1";
	}
      else if
	 (GET_CODE (addr) == PLUS)
        {
	  rtx op0 = XEXP (addr, 0);
	  rtx op1 = XEXP (addr, 1);

	  if (_DD_SWAP_OP_(op0, op1))
	    std::swap (op0, op1);

	  switch (GET_CODE (op1))
	    {
		case CONST_INT:
		  asm_fprintf (stream, "\tld%s%s",
			       ldst_mmodes [mode % 4],
			       ldst_suffix);

		  return "%0, %1";
		case MEM:
		  strict = false;
		  /*
		   * This module describes the case when the program
		   * is loading the data through one INDIRECT memory
		   * access. It is hardly seemed without On (n >= 2)
		   * The legitimate address would be like: rb + mem.
		   */
		  rtx reg;

		  /* This register is clobber to save the data loaded
		   * the internal memory operand which was used as an
		   * offset in the process of an indirect mem access.
		   */
		  reg = gen_rtx_REG (DImode, 7);
		  reg = dadao_print_indirect_operand (reg, op1);

		  /* Fall through */
		case REG:
		  asm_fprintf (stream, "\tldm%s%s",
			       ldst_mmodes [mode % 4],
			       ldst_suffix);

		  if (!strict)      /* indirect memory access loading */
		  {		    /* dest: rd (strict = 1 -> load)  */
		    asm_fprintf (stream, "\t%s, %s, rd7, 1\n",
				    reg_names[REGNO(dest)], reg_names[REGNO(op0)]);
		    return "";	    /* ldm<bwto>(u) rdx, rby, rd7, 1  */
		  }
		  return "%0, %1, 1";
		default:
		  gcc_unreachable ();
	    }
	}
      else
	gcc_unreachable ();
    }
  else
    {
      rtx addr = XEXP (dest, 0);

      if (REG_P (addr))
        {
	  asm_fprintf (stream, "\tst%s",
		       ldst_mmodes [mode % 4]);
	  return "%1, %0";
	}
      else if
	 (GET_CODE (addr) == PLUS)
	{
	  rtx op0 = XEXP (addr, 0);
	  rtx op1 = XEXP (addr, 1);

	  if (_DD_SWAP_OP_(op0, op1))
	    std::swap (op0, op1);

          switch (GET_CODE (op1))
            {
              case CONST_INT:
		asm_fprintf (stream, "\tst%s",
			     ldst_mmodes [mode % 4]);
                return "%1, %0";
	      case MEM:
		  strict = true;
		  /*
		   * This module describes the case when the program
		   * is loading the data through one INDIRECT memory
		   * access. It is hardly seemed without On (n >= 2)
		   * The legitimate address would be like: rb + mem.
		   */
		  rtx reg;

		  /* This register is clobber to save the data loaded
		   * the internal memory operand which was used as an
		   * offset in the process of an indirect mem access.
		   */
		  reg = gen_rtx_REG (DImode, 7);
		  reg = dadao_print_indirect_operand (reg, op1);

		  /* Fall through */
              case REG:
		asm_fprintf (stream, "\tstm%s",
			     ldst_mmodes [mode % 4]);
		  if (strict)       /* indirect memory access storing */
		  {		    /* dest: rd (strict = 1 -> store) */
		    asm_fprintf (stream, "\t%s, %s, rd7, 1\n",
				    reg_names[REGNO(src)], reg_names[REGNO(op0)]);
		    return "";	    /* stm<bwto> rdx, rby, rd7, 1     */
		  }
                return "%1, %0, 1";
              default:
		gcc_unreachable ();
            }
	}
      else
	gcc_unreachable ();
    }
#undef _DD_SWAP_OPERATOR_
}

const char*
dadao_print_fldst_operand (machine_mode mode,
			   rtx dest, rtx src, bool strict)
{
  FILE *stream = asm_out_file;
#define _DD_SWAP_OP_(OP0, OP1) ((REG_P (OP0) && REG_P (OP1))		\
		  && (REGNO_REG_CLASS(REGNO(OP0)) == GENERAL_REGS)	\
		  && (REGNO_REG_CLASS(REGNO(OP1)) == POINTER_REGS))

  const char *suffix = (mode == E_SFmode) ? "t" : "o";
  if (strict) /* float load: SET (REG_OP 0) with (MEM_OP 1) */
  {
    rtx addr = XEXP (src, 0);

    if (REG_P (addr))
      {
	asm_fprintf (stream, "\tld%s", suffix);
	return "%0, %1";
      }
    else if (GET_CODE (addr) == PLUS)
      {
	rtx op0 = XEXP (addr, 0);
	rtx op1 = XEXP (addr, 1);

	if (_DD_SWAP_OP_(op0, op1))
	  std::swap (op0, op1);

	switch (GET_CODE (op1))
	  {
	    case CONST_INT:
	      asm_fprintf (stream, "\tld%s", suffix);
	      return "%0, %1";
	    case MEM:
	      strict = false;
	      /*
	       * This module describes the case when the program
	       * is loading the data through one INDIRECT memory
	       * access. It is hardly seemed without On (n >= 2)
	       * The legitimate address would be like: rb + mem.
	       */
	      rtx reg;

	      /* This register is clobber to save the data loaded
	       * the internal memory operand which was used as an
	       * offset in the process of an indirect mem access.
	       */
	      reg = gen_rtx_REG (DImode, 7);
	      reg = dadao_print_indirect_operand (reg, op1);

	      /* Fall through */
	    case REG:
	      asm_fprintf (stream, "\tldmf%s", suffix);
	      if (!strict)
		{
		  asm_fprintf (stream, "\t%s, %s, rd7, 1",
				  reg_names[REGNO(dest)], reg_names[REGNO(op0)]);
		  return "";
		}
	      return "%0, %1, 1";
	    default:
	      gcc_unreachable ();
	  }
      }
    else
      gcc_unreachable ();
  }
  else
  {
    rtx addr = XEXP (dest, 0);
    if (REG_P (addr))
      {
	asm_fprintf (stream, "\tst%s", suffix);
	return "%1, %0";
      }
    else if (GET_CODE (addr) == PLUS)
      {
	rtx op0 = XEXP (addr, 0);
	rtx op1 = XEXP (addr, 1);

	if (_DD_SWAP_OP_(op0, op1))
	  std::swap (op0, op1);

        switch (GET_CODE (op1))
          {
            case CONST_INT:
	      asm_fprintf (stream, "\tst%s", suffix);
              return "%1, %0";
	    case MEM:
	      strict = true;
	      /*
	       * This module describes the case when the program
	       * is loading the data through one INDIRECT memory
	       * access. It is hardly seemed without On (n >= 2)
	       * The legitimate address would be like: rb + mem.
	       */
	      rtx reg;

	      /* This register is clobber to save the data loaded
	       * the internal memory operand which was used as an
	       * offset in the process of an indirect mem access.
	       */
	      reg = gen_rtx_REG (DImode, 7);
	      reg = dadao_print_indirect_operand (reg, op1);

	      /* Fall through */
            case REG:
	      asm_fprintf (stream, "\tstmf%s", suffix);
	      if (strict)       /* indirect memory access storing */
		{		/* dest: rd (strict = 1 -> store) */
		  asm_fprintf (stream, "\t%s, %s, rd7, 1\n",
			       reg_names[REGNO(src)], reg_names[REGNO(op0)]);
		    return "";	    /* stm<ftfo> rfx, rby, rd7, 1     */
		  }
              return "%1, %0, 1";
            default:
	      gcc_unreachable ();
          }
      }
    else
      gcc_unreachable ();
  }

#undef _DD_SWAP_OP_
  return "";
}

const char *
dadao_print_call_operand (rtx * op, bool strict)
{
  int callobj = (strict) ? GET_CODE(op[0]) : GET_CODE(op[1]);
  FILE * file = asm_out_file;

  switch (callobj)
  {
    case REG:	/* REGS=(rbx)+(rdx)+(INT) */
    case SUBREG:	return (strict) ? "call\t%0, rd0, 0":
					  "call\t%1, rd0, 0";
    case CONST:	/* CONST=(SYMBOL_REF)+(INT) */
    case SYMBOL_REF:	return (strict) ? "call\t%0":
					  "call\t%1";
    case LABEL_REF: /* fall through: illegal call addr.
		     * we can't call a symbolic operand
		     * in data section, so let it fail. */
    default:	gcc_unreachable ();
    case PLUS:
      { rtx BIADDR = (strict) ? op[0]: op[1];
	rtx OFFSET = (strict) ? op[2]: op[3];

	if (REG_P (XEXP (BIADDR, 1)))
	  fprintf (file, "\tcall\t%s, %s, %d\n",
		  reg_names[REGNO (XEXP (BIADDR, 0))],
		  reg_names[REGNO (XEXP (BIADDR, 1))], INTVAL (OFFSET));
	else if (CONSTANT_P (XEXP (BIADDR, 1)))
	  fprintf (file, "\tcall\t%s, rd0, %d\n",
		  reg_names[REGNO (XEXP (BIADDR, 0))],
		  INTVAL (OFFSET) + INTVAL (XEXP (BIADDR, 1)));
	else
	  gcc_unreachable ();
      }
  };
  return "";
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


