;; vim: filetype=lisp

;; Operand and operator predicates for the GCC DADAO port.
;; Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.

;; register class operands
(define_predicate "rg_class_operand"
  (match_code "reg, subreg")
{
	if (!register_operand(op, mode))	return 0;
	if (GET_CODE(op) == SUBREG)		op = SUBREG_REG(op);
	if (!HARD_REGISTER_P(op))		return 1;
	return (REGNO_REG_CLASS(REGNO(op)) == GENERAL_REGS);
})

(define_predicate "rp_class_operand"
  (and (match_code "reg")
       (match_test "REGNO_REG_CLASS (REGNO (op)) == POINTER_REGS")))

(define_predicate "rf_class_operand"
  (match_code "reg, subreg")
{
	if (!register_operand(op, mode))	return 0;
	if (GET_CODE(op) == SUBREG)		op = SUBREG_REG(op);
	if (!HARD_REGISTER_P(op))		return 1;
	return (REGNO_REG_CLASS(REGNO(op)) == FLOATING_REGS);
})

(define_predicate "rr_class_operand"
  (and (match_code "reg")
       (match_test "REGNO_REG_CLASS (REGNO (op)) == RETURN_REGS")))

;; TODO: ri add shift-imm handler
(define_predicate "dd_rg_u12_operand"
  (ior (match_operand 0 "rg_class_operand")
       (and (match_code "const_int")
            (match_test "satisfies_constraint_Jd(op)"))))

(define_predicate "dd_rg_s12_operand"
  (ior (match_operand 0 "rg_class_operand")
       (and (match_code "const_int")
            (match_test "satisfies_constraint_Id(op)"))))

(define_predicate "dd_rg_s18_operand"
  (ior (match_operand 0 "rg_class_operand")
       (and (match_code "const_int")
            (match_test "satisfies_constraint_It(op)"))))

(define_predicate "dd_rg_u18_operand"
  (ior (match_operand 0 "rg_class_operand")
       (and (match_code "const_int")
            (match_test "satisfies_constraint_Jt(op)"))))

(define_predicate "dd_rg_u6_operand"
  (ior (match_operand 0 "rg_class_operand")
       (and (match_code "const_int")
            (match_test "satisfies_constraint_Js(op)"))))

(define_predicate "dd_rf_u6_operand"
  (ior (match_operand 0 "rf_class_operand")
       (and (match_code "const_int")
            (match_test "satisfies_constraint_Js(op)"))))

;; Return 1 if OP is a valid comparison operator for "cbranch" instructions.
(define_predicate "ccff_comparison_operator"
  (match_code "ne, eq, le, ge, lt, gt, ordered, unordered"))

;; True if this is an address_operand or a symbolic operand.

(define_predicate "dadao_symbolic_or_address_operand"
  (match_code "symbol_ref, label_ref, const, subreg, reg, plus")
{
  switch (GET_CODE (op))
    {
    case SYMBOL_REF:
    case LABEL_REF:
      return 1;
    case CONST:
      /* The reason why this body still is C.  */
      op = XEXP (op, 0);
      if ((GET_CODE (XEXP (op, 0)) == SYMBOL_REF
	   || GET_CODE (XEXP (op, 0)) == LABEL_REF)
	  && (GET_CODE (XEXP (op, 1)) == CONST_INT
	      || (GET_CODE (XEXP (op, 1)) == CONST_DOUBLE
		  && GET_MODE (XEXP (op, 1)) == VOIDmode)))
	return 1;
      /* Fall through.  */
    default:
      return dadao_address_operand (op, mode);
    }
})

;; True if this is a memory address, possibly strictly.
;; See also comment above the "*call_real" pattern.

(define_predicate "dadao_address_operand"
  (if_then_else (match_test "reload_in_progress || reload_completed")
    (match_test "strict_memory_address_p (Pmode, op)")
    (match_test "memory_address_p (Pmode, op)")))

(define_predicate "frame_pointer_operand"
  (ior
   (and
    (match_code "reg")
    (match_test "op == hard_frame_pointer_rtx || op == frame_pointer_rtx"))
   (and
    (match_code "plus")
    (match_code "reg" "0")
    (match_code "const_int" "1")
    (match_test "XEXP (op, 0) == stack_pointer_rtx"))))
