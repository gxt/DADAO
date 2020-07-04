;; vim: filetype=lisp

;; GCC machine description for DADAO
;; Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
;; Contributed by Guan Xuetao <gxt@pku.edu.cn>

;; The original PO technology requires these to be ordered by speed,
;; so that assigner will pick the fastest.

;; See file "rtl.def" for documentation on define_insn, match_*, et al.

;; Uses of UNSPEC in this file:
;; UNSPEC_VOLATILE:
;;
;;	1	nonlocal_goto_receiver
;;

;; The order of insns is as in Node: Standard Names, with smaller modes
;; before bigger modes.

(define_constants
  [(DD_RA_REG		126)
   (DADAO_fp_rO_OFFSET	-24)]
)

(include "iterators.md")
(include "predicates.md")
(include "constraints.md")

(include "dadao-rg.md")
(include "dadao-rp.md")
(include "dadao-rf.md")
(include "dadao-cc.md")

(define_insn "nop"
  [(const_int 0)]
	""
	"nop")

;; TODO: are there rp2rf, rf2rv, rv2rs, ... requirements?
(define_insn "mov<mode>"
  [(set (match_operand:QHSD 0 "register_operand" "= Rg, Rp, Rf, Rv, Rp, Rf, Rv, Rg, Rg, Rg")
        (match_operand:QHSD 1 "register_operand" "  Rg, Rp, Rf, Rv, Rg, Rg, Rg, Rp, Rf, Rv"))]
	""
	"@
	or	%0, %1, 0
	rp_p2p	%0, %1
	fo_2fo	%0, %1
	rv_v2v	%0, %1
	rp_g2p	%0, %1
	fo_g2f	%0, %1
	rv_g2v	%0, %1
	rp_p2g	%0, %1
	fo_f2g	%0, %1
	rv_v2g	%0, %1")

(define_insn "dd_ld_<mode>"
  [(set (match_operand:QHSD 0 "rg_class_operand" "= Rg")
        (match_operand:QHSD 1 "memory_operand"   "   m"))]
	""
	"ld<bwto>	%0, %1")

(define_insn "dd_st_<mode>"
  [(set (match_operand:QHSD 0 "memory_operand"   "=  m")
        (match_operand:QHSD 1 "rg_class_operand" "  Rg"))]
	""
	"st<bwto>	%1, %0")

;; TODO: hope following insns will be optimized during rtl process
(define_insn "*dd_st_2_<mode>"
  [(set (match_operand:QHSD 0 "memory_operand" "")
        (match_operand:QHSD 1 "general_operand" ""))]
	""
	"")

(define_expand "call"
  [(parallel [(call (match_operand:SI 0 "memory_operand" "")
		    (match_operand 1 "general_operand" ""))
	      (use (match_operand 2 "general_operand" ""))
	      (clobber (match_dup 4))])
   (set (match_dup 4) (match_dup 3))]
  ""
  "
{
  /* The caller checks that the operand is generally valid as an
     address, but at -O0 nothing makes sure that it's also a valid
     call address for a *call*; a dadao_symbolic_or_address_operand.
     Force into a register if it isn't.  */
  if (!dadao_symbolic_or_address_operand (XEXP (operands[0], 0),
					 GET_MODE (XEXP (operands[0], 0))))
    operands[0]
      = replace_equiv_address (operands[0],
			       force_reg (Pmode, XEXP (operands[0], 0)));

  /* Since the epilogue 'uses' the return address, and it is clobbered
     in the call, and we set it back after every call (all but one setting
     will be optimized away), integrity is maintained.  */
  operands[3]
    = dadao_get_hard_reg_initial_val (Pmode, DADAO_RETURN_ADDRESS_REGNUM);

  /* FIXME: There's a bug in gcc which causes NULL to be passed as
     operand[2] when we get out of registers, which later confuses gcc.
     Work around it by replacing it with const_int 0.  Possibly documentation
     error too.  */
  if (operands[2] == NULL_RTX)
    operands[2] = const0_rtx;

  operands[4] = gen_rtx_REG (DImode, DADAO_RETURN_ADDRESS_REGNUM);
}")

(define_expand "call_value"
  [(parallel [(set (match_operand 0 "" "")
		   (call (match_operand:SI 1 "memory_operand" "")
			 (match_operand 2 "general_operand" "")))
	      (use (match_operand 3 "general_operand" ""))
	      (clobber (match_dup 5))])
   (set (match_dup 5) (match_dup 4))]
  ""
  "
{
  /* The caller checks that the operand is generally valid as an
     address, but at -O0 nothing makes sure that it's also a valid
     call address for a *call*; a dadao_symbolic_or_address_operand.
     Force into a register if it isn't.  */
  if (!dadao_symbolic_or_address_operand (XEXP (operands[1], 0),
					 GET_MODE (XEXP (operands[1], 0))))
    operands[1]
      = replace_equiv_address (operands[1],
			       force_reg (Pmode, XEXP (operands[1], 0)));

  /* Since the epilogue 'uses' the return address, and it is clobbered
     in the call, and we set it back after every call (all but one setting
     will be optimized away), integrity is maintained.  */
  operands[4]
    = dadao_get_hard_reg_initial_val (Pmode, DADAO_RETURN_ADDRESS_REGNUM);

  /* FIXME: See 'call'.  */
  if (operands[3] == NULL_RTX)
    operands[3] = const0_rtx;

  /* FIXME: Documentation bug: operands[3] (operands[2] for 'call') is the
     *next* argument register, not the number of arguments in registers.
     (There used to be code here where that mattered.)  */

  operands[5] = gen_rtx_REG (DImode, DADAO_RETURN_ADDRESS_REGNUM);
}")

;; Don't use 'p' here.  A 'p' must stand first in constraints, or reload
;; messes up, not registering the address for reload.  Several C++
;; testcases, including g++.brendan/crash40.C.  FIXME: This is arguably a
;; bug in gcc.  Note line ~2612 in reload.c, that does things on the
;; condition <<else if (constraints[i][0] == 'p')>> and the comment on
;; ~3017 that says:
;; <<   case 'p':
;;	     /* All necessary reloads for an address_operand
;;	        were handled in find_reloads_address.  */>>
;; Sorry, I have not dug deeper.  If symbolic addresses are used
;; rarely compared to addresses in registers, disparaging the
;; first ("p") alternative by adding ? in the first operand
;; might do the trick.  We define 'Au' as a synonym to 'p', but without the
;; caveats (and very small advantages) of 'p'.
;; As of r190682 still so: newlib/libc/stdlib/dtoa.c ICEs if "p" is used.
(define_insn "*call_real"
  [(call (mem:SI
	  (match_operand:DI 0 "dadao_symbolic_or_address_operand" "s,RpAu"))
	 (match_operand 1 "" ""))
   (use (match_operand 2 "" ""))
   (clobber (reg:DI DD_RA_REG))]
  ""
  "@
	call	%0
	call	%2, %a0")

(define_insn "*call_value_real"
  [(set (match_operand 0 "register_operand" "=r,r")
	(call (mem:SI
	       (match_operand:DI 1 "dadao_symbolic_or_address_operand" "s,RpAu"))
	      (match_operand 2 "" "")))
  (use (match_operand 3 "" ""))
  (clobber (reg:DI DD_RA_REG))]
  ""
  "@
	call	%1
	call	%3, %a1")

;; I hope untyped_call and untyped_return are not needed for DADAO.
;; Users of Objective-C will notice.

; Generated by GCC.
(define_expand "return"
  [(return)]
  "dadao_use_simple_return ()"
  "")

; Generated by the epilogue expander.
(define_insn "*expanded_return"
  [(return)]
  ""
	"ret")

(define_expand "prologue"
  [(const_int 0)]
  ""
  "dadao_expand_prologue (); DONE;")

; Note that the (return) from the expander itself is always the last insn
; in the epilogue.
(define_expand "epilogue"
  [(return)]
  ""
  "dadao_expand_epilogue ();")

(define_insn "jump"
  [(set (pc) (label_ref (match_operand 0 "" "")))]
  ""
	"jump	%0")

(define_insn "indirect_jump"
  [(set (pc) (match_operand 0 "address_operand" "p"))]
  ""
	"jump	rg63, %a0")

;; FIXME: This is just a jump, and should be expanded to one.
(define_insn "tablejump"
  [(set (pc) (match_operand:DI 0 "address_operand" "p"))
   (use (label_ref (match_operand 1 "" "")))]
  ""
	"jump	rg63, %a0")

;; The only peculiar thing is that the register stack has to be unwound at
;; nonlocal_goto_receiver.  At each function that has a nonlocal label, we
;; save at function entry the location of the "alpha" register stack
;; pointer, rO, in a stack slot known to that function (right below where
;; the frame-pointer would be located).
;; In the nonlocal goto receiver, we unwind the register stack by a series
;; of "pop 0,0" until rO equals the saved value.  (If it goes lower, we
;; should die with a trap.)
(define_expand "nonlocal_goto_receiver"
  [(parallel [(unspec_volatile [(match_dup 1)] 1)
	      (clobber (scratch:DI))
	      (clobber (reg:DI DD_RA_REG))])
   (set (reg:DI DD_RA_REG) (match_dup 0))]
  ""
  "
{
  operands[0]
    = dadao_get_hard_reg_initial_val (Pmode, DADAO_RETURN_ADDRESS_REGNUM);

  /* We need the frame-pointer to be live or the equivalent
     expression, so refer to it in the pattern.  We can't use a MEM
     (that may contain out-of-range offsets in the final expression)
     for fear that middle-end will legitimize it or replace the address
     using temporary registers (which are not revived at this point).  */
  operands[1] = frame_pointer_rtx;

  /* Mark this function as containing a landing-pad.  */
  cfun->machine->has_landing_pad = 1;
}")

;; GCC can insist on using saved registers to keep the slot address in
;; "across" the exception, or (perhaps) to use saved registers in the
;; address and re-use them after the register stack unwind, so it's best
;; to form the address ourselves.
(define_insn "*nonlocal_goto_receiver_expanded"
  [(unspec_volatile [(match_operand:DI 1 "frame_pointer_operand" "Sf")] 1)
   (clobber (match_scratch:DI 0 "=&r"))
   (clobber (reg:DI DD_RA_REG))]
  ""
{
  rtx my_operands[3];
  const char *my_template
    = "	geta	rg63, 0f	\;\
	rp_g2p	rp62, rg63	\;\
	ldo	rg63, %a0, 0	\;\
0:	ret";

  my_operands[1] = operands[0];
  my_operands[2] = GEN_INT (-DADAO_fp_rO_OFFSET);

  if (operands[1] == hard_frame_pointer_rtx)
    {
      dadao_output_register_setting (asm_out_file, REGNO (operands[0]),
				    DADAO_fp_rO_OFFSET, 1);
      my_operands[0]
	= gen_rtx_PLUS (Pmode, hard_frame_pointer_rtx, operands[0]);
    }
  else
    {
      int64_t offs = INTVAL (XEXP (operands[1], 1));
      offs += DADAO_fp_rO_OFFSET;

      if (insn_const_int_ok_for_constraint (offs, CONSTRAINT_Id))
	my_operands[0]
	  = gen_rtx_PLUS (Pmode, stack_pointer_rtx, GEN_INT (offs));
      else
	{
	  dadao_output_register_setting (asm_out_file, REGNO (operands[0]),
					offs, 1);
	  my_operands[0]
	    = gen_rtx_PLUS (Pmode, stack_pointer_rtx, operands[0]);
	}
    }

  output_asm_insn (my_template, my_operands);
  return "";
})
