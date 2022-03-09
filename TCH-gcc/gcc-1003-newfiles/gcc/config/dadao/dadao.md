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

(define_expand "movdi"
  [(set (match_operand:DI 0 "general_operand" "")
        (match_operand:DI 1 "general_operand" ""))]
	""
	"{
	  if (satisfies_constraint_Ao (operands[1]))
	    {
		emit_insn (gen_rtx_SET (gen_rtx_REG (DImode, 71), operands[1]));
		emit_insn (gen_rtx_SET (gen_rtx_REG (DImode, 7), gen_rtx_REG (DImode, 71)));
		operands[1] = gen_rtx_REG (DImode, 7);
	    }
	  if (MEM_P (operands[0]))
	    {
		if (!REG_P (operands[1]))
		  {
		    if (can_create_pseudo_p ())
		      {
			rtx ip = gen_reg_rtx (DImode);
			emit_insn (gen_movdi (ip, operands[1]));
			operands[1] = ip;
		      }
		    else
		      {
			rtx ip = gen_rtx_REG (DImode, 7);
			emit_insn (gen_movdi (ip, operands[1]));
			operands[1] = ip;
		      }
		  }
		DONE;
	    }
	  else if (MEM_P (operands[1]))
	    {
		if (!REG_P (operands[0]))
		  {
		    if (can_create_pseudo_p ())
                      {
			rtx ip = gen_reg_rtx (DImode);
			emit_insn (gen_movdi (ip, operands[0]));
			operands[0] = ip;
                      }
                    else
                      {
                        rtx ip = gen_rtx_REG (DImode, 7);
                        emit_insn (gen_movdi (ip, operands[0]));
                        operands[0] = ip;
                      }
		  }
		DONE;
	    }
	}")

(define_insn "mov<mode>"
  [(set (match_operand:LTO 0 "nonimmediate_operand" "=m,Rd,Rd")
        (match_operand:LTO 1 "general_operand"	    "Rd,Rd, m"))]
	""
	{
	  if (MEM_P (operands[0]) &&
	      REG_P (operands[1]) &&
	      REGNO_REG_CLASS (REGNO(operands[1])) == GENERAL_REGS)
	    {
		if (satisfies_constraint_Wg (operands[0])) return "stm<qhs>	%1, %0, 0";
		else if
		   (satisfies_constraint_Wm (operands[0]))
		  {
			rtx base = dadao_expand_memmov_indirect (operands, asm_out_file);
			fprintf (asm_out_file, "\tstm<qhs>	%s, %s, %s, 0\n",
				 reg_names[REGNO (operands[1])], reg_names[REGNO (base)], reg_names[DD_IP_RD]);
			return "";
		  }
		else	return "st<qhs>	%1, %0";
	    }
          if (MEM_P (operands[1]) &&
	      REG_P (operands[0]) &&
	      REGNO_REG_CLASS (REGNO(operands[0])) == GENERAL_REGS)
	    {
                if (satisfies_constraint_Wg (operands[1])) return "ldm<qhs>u	%0, %1, 0";
		else if
		   (satisfies_constraint_Wm (operands[1]))
		   {
			rtx base = dadao_expand_memmov_indirect (operands, asm_out_file);

			fprintf (asm_out_file,
				 "\tldm<qhs>	%s, %s, %s, 0\n", reg_names[REGNO (operands[0])],
								  reg_names[REGNO (base)],
								  reg_names[DD_IP_RD]);
			return "";
		   }
                else	return "ld<qhs>u	%0, %1";
            }
	  if (REG_P (operands[0]) && REGNO_REG_CLASS (REGNO(operands[0])) == GENERAL_REGS &&
	      REG_P (operands[1]) && REGNO_REG_CLASS (REGNO(operands[1])) == GENERAL_REGS)
		return "orr	%0, %1, rd0";

	  return "";
	})

(define_insn "dd_ld<mode>"
  [(set (match_operand:LTO 0 "rd_class_operand" "=Rd")
        (match_operand:LTO 1 "memory_operand"   " m"))]
	""
	{
          if (satisfies_constraint_Wg (operands[1]))
		return "ldm<qhs>u	%0, %1, 0";
	  else if (satisfies_constraint_Wm (operands[1]))
	    {
	      rtx base = dadao_expand_memmov_indirect (operands, asm_out_file);
	      fprintf (asm_out_file, "\tldm<qhs>	%s, %s, %s, 0\n",
				   reg_names[REGNO (operands[0])], reg_names[REGNO (base)], reg_names[DD_IP_RD]);
	      return "";
	    }
	  else return "ld<qhs>u	%0, %1";
	})

(define_insn "dd_st<mode>"
  [(set (match_operand:QHSD 0 "memory_operand"   "=m")
        (match_operand:QHSD 1 "rd_class_operand" "Rd"))]
	""
	{
	  if (satisfies_constraint_Wg (operands[0])) return "stm<bwto>	%1, %0, 0";
	  else if
	     (satisfies_constraint_Wm (operands[0]))
	     {
		rtx base = dadao_expand_memmov_indirect (operands, asm_out_file);
		fprintf (asm_out_file, "\tstm<bwto>	%s, %s, %s, 0\n",
			 reg_names[REGNO (operands[1])], reg_names[REGNO (base)], reg_names[DD_IP_RD]);
		return "";
	      }
	  else	return "st<bwto>	%1, %0";
	})

(define_insn "dd_lddi"
  [(set (match_operand:DI 0 "rd_class_operand" "=Rd")
        (match_operand:DI 1 "memory_operand"   " m"))]
	""
	{
          if (satisfies_constraint_Wg (operands[1]))
		return "ldmo	%0, %1, 0";
	  else if (satisfies_constraint_Wm (operands[1]))
	    {
	      rtx base = dadao_expand_memmov_indirect (operands, asm_out_file);
	      fprintf (asm_out_file, "\tldmo	%s, %s, %s, 0\n",
				   reg_names[REGNO (operands[0])], reg_names[REGNO (base)], reg_names[DD_IP_RD]);
	      return "";
	    }
	  else return "ldo	%0, %1";
	})

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
	    return "call	%0, %a2";
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
	    return "call	%1, %a3";
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
	return "call	%1, %a3";
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

      gcc_assert (REG_P (base) && REG_P (offset));

      fprintf (asm_out_file, "\tjump	%s, %s, 0\n",
			reg_names[REGNO(base)],
			reg_names[REGNO(offset)]);

      return "";
    }
    else
        return "jump	%0";
  })

;; FIXME: This is just a jump, and should be expanded to one.
(define_insn "tablejump"
  [(set (pc) (match_operand:DI 0 "address_operand" "p"))
   (use (label_ref (match_operand 1 "" "")))]
  ""
  "jump	%0")
