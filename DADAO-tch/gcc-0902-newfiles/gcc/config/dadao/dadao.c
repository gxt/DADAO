/*
 * Target Code for DADAO.
 * Copyright (C) 2019-2033 Guan Xuetao (AT) Peking Univ.
 *
 * Contributed by:
 *   2019:
 *	Shi Yining <1700012728@pku.edu.cn>
 *	Guan Xuetao <gxt@pku.edu.cn>
 */

#define IN_TARGET_CODE 1

#include<stdio.h>
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
#include "regs.h"
#include "memmodel.h"
#include "emit-rtl.h"
#include "diagnostic-core.h"
#include "output.h"
#include "stor-layout.h"
#include "varasm.h"
#include "calls.h"
#include "expr.h"
#include "builtins.h"


/* This file should be included last.  */
#include "target-def.h"

#define DADAO_DEBUG 0
#if DADAO_DEBUG
#   define debug_printf(format,...) printf("[debug]" format,##__VA_ARGS__)
#else
#   define debug_printf(format,...)
#endif

bool dadao_debug_func(int id)
{
	debug_printf("%d\n",id);
	return true;
}

static bool dadao_valid_imm12_p(rtx x)
{
	if (!CONST_INT_P(x)) return false;
	int val = INTVAL(x);
	return val <= 2047 && val >= -2048;
}

static bool dadao_valid_scale_p(rtx x)
{
	if (!CONST_INT_P(x)) return false;
	unsigned int val = INTVAL(x);
	return  val <= 4;
}

/* Implement TARGET_LEGITIMATE_ADDRESS_P.  */
bool
dadao_legitimate_address_p(machine_mode mode, rtx x, bool strict_p)
{
	int code = GET_CODE (x);
	switch (code){
		case REG:
			return strict_p ? REGNO_OK_FOR_BASE_P(REGNO(x)) : true;
		case PLUS:
			if (!REG_P(XEXP(x,0))) return false;
			if (strict_p && ! REGNO_OK_FOR_BASE_P(REGNO(XEXP(x,0)))) return false;
			x =  XEXP(x,1);
			if (GET_CODE(x) == CONST_INT)
				return dadao_valid_imm12_p(x);
			else if (GET_CODE(x) == REG)
				return true;
			else if (GET_CODE(x) == ASHIFT){
				rtx scale = XEXP(x,1);
				x = XEXP(x,0);
				return GET_CODE(x) == REG && dadao_valid_scale_p(scale);
			}
			return false;
		case CONST:
		case LABEL_REF:
		case SYMBOL_REF:
			return true;
	}
	return false;
}

#undef TARGET_LEGITIMATE_ADDRESS_P
#define TARGET_LEGITIMATE_ADDRESS_P dadao_legitimate_address_p

void dadao_expand_conditional_branch(rtx_code code,rtx r1,rtx r2,rtx label)
{
	if (code == EQ || code == NE) return;
	rtx x = gen_reg_rtx(DImode);
	switch (code) {
		case LT:
			emit_insn(gen_setlt(x,r1,r2));
			break;
		case LTU:
			emit_insn(gen_setltu(x,r1,r2));
			break;
		case GT:
			emit_insn(gen_setgt(x,r1,r2));
			break;
		case GTU:
			emit_insn(gen_setgtu(x,r1,r2));
			break;
		case LE:
			emit_insn(gen_setle(x,r1,r2));
			break;
		case LEU:
			emit_insn(gen_setleu(x,r1,r2));
			break;
		case GE:
			emit_insn(gen_setge(x,r1,r2));
			break;
		case GEU:
			emit_insn(gen_setgeu(x,r1,r2));
			break;
		default:
			gcc_unreachable();
	}
	emit_jump_insn(gen_condjump(x,label));

	return;
}

//where to place the return value in function call and lib call
static rtx
dadao_function_value (const_tree valtype,
	const_tree fntype_or_decl,bool outgoing)
{
	return gen_rtx_REG (TYPE_MODE (valtype), DADAO_RDnn_V0);
}

#undef TARGET_FUNCTION_VALUE
#define TARGET_FUNCTION_VALUE dadao_function_value

static bool
dadao_return_in_memory (const_tree type, const_tree fntype ATTRIBUTE_UNUSED)
{
	const HOST_WIDE_INT size = int_size_in_bytes (type);
	return (size == -1 || size > UNITS_PER_WORD);
}

#undef TARGET_RETURN_IN_MEMORY
#define TARGET_RETURN_IN_MEMORY dadao_return_in_memory

static rtx
dadao_libcall_value (machine_mode mode,const_rtx fun)
{
	return gen_rtx_REG (mode, DADAO_RDnn_V0);
}

#undef TARGET_LIBCALL_VALUE
#define TARGET_LIBCALL_VALUE dadao_libcall_value

static bool
dadao_function_value_regno_p (const unsigned int regno)
{
	return (regno == DADAO_RDnn_V0 || regno == DADAO_RDnn_V1);
}

#undef TARGET_FUNCTION_VALUE_REGNO_P
#define TARGET_FUNCTION_VALUE_REGNO_P dadao_function_value_regno_p

bool
dadao_function_arg_regno_p (int regno)
{
	return (regno >= DADAO_RDnn_ARG_FIRST && regno <= DADAO_RDnn_ARG_LAST);
}

//define how to pass function param
static bool
dadao_pass_by_reference (cumulative_args_t cum,
		machine_mode mode, const_tree type, bool namedD)
{
	unsigned HOST_WIDE_INT size = type ? int_size_in_bytes(type) : GET_MODE_SIZE(mode);
	return size > UNITS_PER_WORD;
}

#undef TARGET_PASS_BY_REFERENCE
#define TARGET_PASS_BY_REFERENCE dadao_pass_by_reference

static int
dadao_arg_partial_bytes (cumulative_args_t cum_v,
		machine_mode mode,tree type, bool named)
{
	return 0;
}

#undef TARGET_ARG_PARTIAL_BYTES
#define TARGET_ARG_PARTIAL_BYTES dadao_arg_partial_bytes

void dadao_init_cumulative_args(CUMULATIVE_ARGS* cum)
{
  cum->cnt_d = DADAO_RDnn_ARG_FIRST;
  cum->cnt_b = DADAO_RBnn_ARG_FIRST;
}

static rtx
dadao_function_arg (cumulative_args_t cum_v, machine_mode mode,
		    const_tree type,bool named)
{
	CUMULATIVE_ARGS cum = *get_cumulative_args (cum_v);
	if (POINTER_TYPE_P(type))
	{
		if (cum.cnt_b >= DADAO_RBnn_ARG_FIRST && cum.cnt_b <= DADAO_RBnn_ARG_LAST)
			return gen_rtx_REG (mode, cum.cnt_b);
		else
			return NULL_RTX;
	} else {
		if (cum.cnt_d >= DADAO_RDnn_ARG_FIRST && cum.cnt_d <= DADAO_RDnn_ARG_LAST)
			return gen_rtx_REG (mode, cum.cnt_d);
		else
			return NULL_RTX;
	}
}

#undef TARGET_FUNCTION_ARG
#define TARGET_FUNCTION_ARG dadao_function_arg

static void
dadao_function_arg_advance (cumulative_args_t cum_v, machine_mode mode,
			    const_tree type, bool named)
{
	CUMULATIVE_ARGS *cum = get_cumulative_args (cum_v);
	if (POINTER_TYPE_P(type))
		cum->cnt_b = cum->cnt_b + 1;
	else
		cum->cnt_d = cum->cnt_d + 1;
}

#undef TARGET_FUNCTION_ARG_ADVANCE
#define TARGET_FUNCTION_ARG_ADVANCE dadao_function_arg_advance

//the frame pointer can not be eliminated
#undef TARGET_FRAME_POINTER_REQUIRED
#define TARGET_FRAME_POINTER_REQUIRED hook_bool_void_true

//asm output format
static void
dadao_print_operand (FILE *file, rtx x, int code)
{
	if (GET_CODE(x) == REG) {
		if (REGNO (x) > FIRST_PSEUDO_REGISTER)
			internal_error ("internal error: bad register: %d", REGNO (x));
		fprintf(file, "%s", reg_names[REGNO(x)]);
	}
	else if (GET_CODE(x) == MEM) {
		output_address (GET_MODE(XEXP(x,0)), XEXP(x,0));
	}
	else if (CONSTANT_P(x)) {
		output_addr_const(file,x);
	}
	return;
}

#undef TARGET_PRINT_OPERAND
#define TARGET_PRINT_OPERAND dadao_print_operand

static void
dadao_print_operand_address (FILE *file,machine_mode m, rtx x)
{
	rtx addend;
	switch (GET_CODE (x)){
	case REG:
		fprintf (file, "(%s)", reg_names[REGNO (x)]);
		break;
	case PLUS:
		addend = XEXP(x,1);
		x = XEXP(x,0);
		if (GET_CODE(addend) == REG)
			fprintf(file,"(%s,%s)",reg_names[REGNO(x)],reg_names[REGNO(addend)]);
		else if (GET_CODE(addend) == ASHIFT) {
				int scale = INTVAL(XEXP(addend,1));
				int rdnum = REGNO(XEXP(addend,0));
				fprintf(file,"(%s,%s,%d)",reg_names[REGNO(x)],reg_names[rdnum],scale);
			} else if (GET_CODE(addend) == CONST_INT) {
				fprintf (file, "%ld(%s)", INTVAL(addend),reg_names[REGNO(x)]);
			} else if (GET_CODE(addend) == SYMBOL_REF) {
				output_addr_const(file,addend);
				fprintf (file, "(%s)",reg_names[REGNO(x)]);
			} else if (GET_CODE(addend) == CONST) {
				addend = XEXP(addend,0);
				if (GET_CODE (XEXP(addend,0)) == SYMBOL_REF
				&& CONST_INT_P (XEXP (addend, 1))) {
					output_addr_const(file, XEXP (addend, 0));
					fprintf (file,"+%ld(%s)",INTVAL(XEXP(addend,1)),reg_names[REGNO(x)]);
				}
			}
			break;
		default:
			output_addr_const (file, x);
			break;
    }
}

#undef TARGET_PRINT_OPERAND_ADDRESS
#define TARGET_PRINT_OPERAND_ADDRESS dadao_print_operand_address

/* nest functions */
static void
dadao_trampoline_init (rtx m_tramp, tree fndecl, rtx chain_value)
{
	// TODO to be finished
	;
}

#undef TARGET_TRAMPOLINE_INIT
#define TARGET_TRAMPOLINE_INIT dadao_trampoline_init

/* Store function data.  */
struct GTY(()) machine_function
 {
   /* Number of bytes saved on the stack for callee saved registers.  */
   int callee_saved_reg_size;

   /* Number of bytes saved on the stack for local variables.  */
   int local_vars_size;

   /* The sum of 2 sizes: locals vars and padding byte for saving the
    * registers.  Used in expand_prologue () and expand_epilogue().  */
   int size_for_adjusting_sp;
 };

/* Zero initialization is OK for all current fields.  */

static struct machine_function *
dadao_init_machine_status (void)
{
  return ggc_cleared_alloc<machine_function> ();
}


/* The TARGET_OPTION_OVERRIDE worker.  */
static void
dadao_option_override (void)
{
  /* Set the per-function-data initializer.  */
  init_machine_status = dadao_init_machine_status;
}

#undef TARGET_OPTION_OVERRIDE
#define TARGET_OPTION_OVERRIDE dadao_option_override


/* Compute the size of the local area and the size to be adjusted by the
 * prologue and epilogue.  */

static void
dadao_compute_frame (void)
{
  /* For aligning the local variables.  */
  int stack_alignment = STACK_BOUNDARY / BITS_PER_UNIT;
  int padding_locals;
  int regno;

  /* Padding needed for each element of the frame.  */
  cfun->machine->local_vars_size = get_frame_size ();

  /* Align to the stack alignment.  */
  padding_locals = cfun->machine->local_vars_size % stack_alignment;
  if (padding_locals > 0)
    cfun->machine->local_vars_size += (stack_alignment-padding_locals);

  cfun->machine->callee_saved_reg_size = 0;

  /* Save callee-saved registers.  */
  for (regno = 0; regno < FIRST_PSEUDO_REGISTER; regno++)
    if (df_regs_ever_live_p (regno) && (! call_used_regs[regno]))
      cfun->machine->callee_saved_reg_size += 8;

  cfun->machine->size_for_adjusting_sp = 8 + //reserved for fp
    crtl->args.pretend_args_size
    + cfun->machine->local_vars_size
    + cfun->machine->callee_saved_reg_size 
      + (HOST_WIDE_INT) crtl->outgoing_args_size;
}
//	prologue and epilogue
void dadao_expand_prologue()
{
  int regno,offset = 0;
  rtx insn;
  dadao_compute_frame();
  rtx regsp = gen_rtx_REG(Pmode,DADAO_RBnn_SP);
  rtx regfp = gen_rtx_REG(Pmode,DADAO_RBnn_FP);

  offset -= 8;
  insn = emit_insn(gen_movdi(gen_rtx_MEM(Pmode,gen_rtx_PLUS(Pmode,regsp,GEN_INT(offset))),regfp)); 
  RTX_FRAME_RELATED_P (insn) = 1;

  insn = emit_insn(gen_movdi(regfp,regsp));
  RTX_FRAME_RELATED_P (insn) = 1;
  insn = emit_insn(gen_subdi3(regsp,regsp,GEN_INT(cfun->machine->size_for_adjusting_sp))); 
  RTX_FRAME_RELATED_P (insn) = 1;
  for (regno = 0;regno < FIRST_PSEUDO_REGISTER; regno++)
    {
      if (!call_used_regs[regno] && df_regs_ever_live_p(regno))
        {
          rtx reg_src = gen_rtx_REG(DImode,regno);
	  offset -= 8;
          rtx mem_tar = gen_rtx_MEM(Pmode,gen_rtx_PLUS(Pmode,regfp,GEN_INT(offset)));
          insn = emit_insn(gen_movdi(mem_tar,reg_src));
          RTX_FRAME_RELATED_P (insn) = 1;
        }
    }
  return;
}


void dadao_expand_epilogue()
{
  int regno,offset;
  rtx insn;
  rtx regsp = gen_rtx_REG(Pmode,DADAO_RBnn_SP);
  rtx regfp = gen_rtx_REG(Pmode,DADAO_RBnn_FP);


  for (regno = 0,offset = -8;regno < FIRST_PSEUDO_REGISTER;regno++)
    {
      if (!call_used_regs[regno] && df_regs_ever_live_p(regno))
        {
          rtx reg_tar = gen_rtx_REG(DImode,regno);
	  offset -= 8;
          rtx mem_src = gen_rtx_MEM(Pmode,gen_rtx_PLUS(Pmode,regfp,GEN_INT(offset)));
          insn = emit_insn(gen_movdi(reg_tar,mem_src));
          RTX_FRAME_RELATED_P (insn) = 1;
        }
    }
  
  insn = emit_insn(gen_movdi(regsp,regfp));
  RTX_FRAME_RELATED_P (insn) = 1;

  offset = -8;
  insn = emit_insn(gen_movdi(regfp,gen_rtx_MEM(Pmode,gen_rtx_PLUS(Pmode,regsp,GEN_INT(offset))))); 
  RTX_FRAME_RELATED_P (insn) = 1;

  emit_jump_insn(gen_returner());
  return;
}


int
dadao_initial_elimination_offset (int from, int to)
{
  int dif;
  if ((from) == ARG_POINTER_REGNUM && (to) == FRAME_POINTER_REGNUM)
    dif = 0;
  else if ((from) == ARG_POINTER_REGNUM && (to) == STACK_POINTER_REGNUM)
    {
      dadao_compute_frame();
      return -cfun->machine->callee_saved_reg_size;
    }
  else abort();
  return dif;
}


bool dadao_offset_address_p (rtx x)
{
  x = XEXP (x, 0);

  if (GET_CODE (x) == PLUS) {
    x = XEXP (x, 1);

    if (GET_CODE (x) == CONST_INT) {
      unsigned int v = INTVAL (x) & 0xFFFF8000;
      return (v == 0xFFFF8000 || v == 0x00000000);
    }
  }
  return 0;
}

//initialization of targetm
struct gcc_target targetm = TARGET_INITIALIZER;

//used for machine-function
#include "gt-dadao.h"
