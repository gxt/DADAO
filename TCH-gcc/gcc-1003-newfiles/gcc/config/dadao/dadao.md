;; vim: filetype=lisp

;; GCC machine description for DADAO
;; Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
;; Contributed by Guan Xuetao <gxt@pku.edu.cn>

;; The original PO technology requires these to be ordered by speed,
;; so that assigner will pick the fastest.

(define_constants
	[(DD_IP_RD		  7)
	 (DD_IP_RB		 71)
	 (DD_RA_REG		126)]
)

(include "iterators.md")
(include "predicates.md")
(include "constraints.md")

(include "dadao-rd.md")
(include "dadao-rb.md")
(include "dadao-rf.md")
(include "dadao-ra.md")
(include "dadao-cc.md")

(define_insn "nop"
  [(const_int 0)]
	""
	"swym")

(define_expand "mov<mode>"
  [(set (match_operand:QHSD 0 "")
	(match_operand:QHSD 1 ""))]
  ""
{
  if (can_create_pseudo_p ())
    {
      if (!REG_P (operands[0]))
	operands[1] = force_reg (<MODE>mode, operands[1]);
    }
})

(define_insn "dadao_dload<mode>"
  [(set (match_operand:QHSD 0 "rd_class_operand" "=Rd")
	(match_operand:QHSD 1 "memory_operand"    " m"))]
  ""
  {
    return dadao_print_ldst_operand
		(GET_MODE (operands[0]),
			   operands[0], operands[1], true);
  })

(define_insn "dadao_dstore<mode>"
  [(set (match_operand:QHSD 0 "memory_operand"   "=m")
	(match_operand:QHSD 1 "rd_class_operand" "Rd"))]
  ""
  {
    return dadao_print_ldst_operand
		(GET_MODE (operands[0]),
			   operands[0], operands[1], false);
  })

;; Floating-point move insn pattern
;; --------------------------------

(define_expand "mov<mode>"
  [(set (match_operand:SFDF 0 "")
	(match_operand:SFDF 1 ""))]
  ""
{
  if (can_create_pseudo_p ())
    {
      if (!REG_P (operands[0]))
	{
	  operands[1] = force_reg (<MODE>mode, operands[1]);
	}
    }
})

;; Floating-point data-reg load / store
;; ------------------------------------

(define_insn "dadao_dload<mode>"
  [(set (match_operand:SFDF 0 "rd_class_operand" "=Rd")
	(match_operand:SFDF 1 "memory_operand"   "  m"))]
  ""
  {
    return dadao_print_ldst_operand
		(GET_MODE (operands[0]),
			   operands[0], operands[1], true);
  })

(define_insn "dadao_dstore<mode>"
  [(set (match_operand:SFDF 0 "memory_operand"   "=m")
	(match_operand:SFDF 1 "rd_class_operand" "Rd"))]
  ""
  {
    return dadao_print_ldst_operand
		(GET_MODE (operands[0]),
			   operands[0], operands[1], false);
  })

;; Floating-point floating-reg load / store
;; ----------------------------------------

(define_insn "dadao_fload<mode>"
  [(set (match_operand:SFDF 0 "rf_class_operand" "=Rf")
	(match_operand:SFDF 1 "memory_operand"   "  m"))]
  ""
  "")

(define_insn "dadao_fstore<mode>"
  [(set (match_operand:SFDF 0 "memory_operand"   "=m")
	(match_operand:SFDF 1 "rf_class_operand" "Rf"))]
  ""
  "")

(define_expand "call"
  [(parallel [(call (match_operand 0 "memory_operand" "")
		    (match_operand 1 "general_operand" ""))
	      (use  (match_operand 2 "" ""))
	      (clobber (reg:DI DD_RA_REG))])]
	""
{
	if (!dadao_symbolic_or_address_operand (XEXP (operands[0], 0),
				GET_MODE (XEXP (operands[0], 0))))
	operands[0] = replace_equiv_address (operands[0],
				force_reg (Pmode, XEXP (operands[0], 0)));

	if (operands[2] == NULL_RTX)	operands[2] = const0_rtx;
})

(define_expand "call_value"
  [(parallel [(set (match_operand 0 "" "")
		   (call (match_operand 1 "memory_operand" "")
			 (match_operand 2 "general_operand" "")))
	      (use (match_operand 3 "" ""))
	      (clobber (reg:DI DD_RA_REG))])]
	""
{
	if (!dadao_symbolic_or_address_operand (XEXP (operands[1], 0),
				GET_MODE (XEXP (operands[1], 0))))
	operands[1] = replace_equiv_address (operands[1],
				force_reg (Pmode, XEXP (operands[1], 0)));

	if (operands[3] == NULL_RTX)	operands[3] = const0_rtx;
})

(define_insn "dd_call_real"
  [(call (mem:SI
	  (match_operand:DI 0 "dadao_symbolic_or_address_operand" "s,RbAu"))
	 (match_operand 1 "" ""))
   (use (match_operand 2 "" ""))
   (clobber (reg:DI DD_RA_REG))]
  ""
  {
    if (GET_CODE (operands[0]) == SYMBOL_REF)
      {
	return "call	%0";
      }
    else
      {
	if (REG_P (operands[0]))
	  {
	    return "call	%0, rd0, 0";
	  }
	else if (GET_CODE (operands[0]) == CONST)
	  {
	// FIXME
	    return "call	%0";
	  }
	else if (GET_CODE (operands[0]) == PLUS)
	  {
	    fprintf (asm_out_file, "\tcall	");
	    rtx op0 = XEXP (operands[0], 0);
	    rtx op1 = XEXP (operands[0], 1);
	    fprintf (asm_out_file, "%s, ", reg_names[REGNO(op0)]);
	    fprintf (asm_out_file, "%s, %d\n", reg_names[REGNO(operands[2])], INTVAL(op1));
	    return "";
	  }
	return "call	%0, %a2";
      }
  })

(define_insn "dd_call_value_real"
  [(set (match_operand 0 "register_operand" "=r,r")
	(call (mem:SI
	       (match_operand:DI 1 "dadao_symbolic_or_address_operand" "s,RbAu"))
	      (match_operand 2 "" "")))
  (use (match_operand 3 "" ""))
  (clobber (reg:DI DD_RA_REG))]
  ""
  {
    if (GET_CODE (operands[1]) == SYMBOL_REF)
      {
	return "call	%1";
      }
    else
      {
	if (REG_P (operands[1]))
	  {
	    return "call	%1, rd0, 0";
	  }
	else if (GET_CODE (operands[1]) == CONST)
	  {
	// FIXME
	    return "call	%1";
	  }
	else if (GET_CODE (operands[1]) == PLUS)
	  {
	    fprintf (asm_out_file, "\tcall	");
	    rtx op0 = XEXP (operands[1], 0);
	    rtx op1 = XEXP (operands[1], 1);
	    fprintf (asm_out_file, "%s, ", reg_names[REGNO(op0)]);
	    fprintf (asm_out_file, "%s, %d\n", reg_names[REGNO(operands[3])], INTVAL(op1));
	    return "";
	  }
	return "call 	%1, %a3";
      }
  })

;; I hope untyped_call and untyped_return are not needed for DADAO.
;; Users of Objective-C will notice.

; Generated by GCC.
(define_expand "return"
  [(return)]
  "dadao_use_simple_return ()"
  "")

; Generated by the epilogue expander.
(define_insn "expanded_return"
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
  {
    if (GET_CODE (operands[0]) == PLUS)
    {
      rtx base = XEXP (operands[0], 0);
      rtx offset = XEXP (operands[0], 1);

      gcc_assert (REG_P (base) && (REG_P (offset) || CONST_INT_P (offset)));

      int off_flag = CONST_INT_P (offset);

      fprintf (asm_out_file, "\tjump\t%s, %s, 0\n",
		reg_names[REGNO(base)], (off_flag) ? "rd0" : reg_names[REGNO(offset)],
					(off_flag) ? INTVAL(offset) : 0);
      return "";
    }
    else return "jump\t%0";
  })

;; FIXME: This is just a jump, and should be expanded to one.
(define_insn "tablejump"
  [(set (pc) (match_operand:DI 0 "address_operand" "p"))
   (use (label_ref (match_operand 1 "" "")))]
  ""
  "jump	%0")
