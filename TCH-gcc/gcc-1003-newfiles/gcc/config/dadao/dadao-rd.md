;; vim: filetype=lisp

;; GCC machine description for DADAO rd class
;; Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
;; Contributed by Guan Xuetao <gxt@pku.edu.cn>

(define_insn "<code>di3"
  [(set          (match_operand:DI 0 "rd_class_operand" "=   Rd")
    (RD_ARITH:DI (match_operand:DI 1 "rd_class_operand" "%   Rd")
                 (match_operand:DI 2 "rd_class_operand" "    Rd")))]
	""
	"<rd_arith_insn>	%0, %1, %2")

;; setrd is pseudo insn, so we place ? before Rd
(define_insn "mov_ri<mode>"
  [(set (match_operand:QHSD 0 "rd_class_operand"  "=??Rd")
        (match_operand:QHSD 1 "const_int_operand" "    n"))]
	""
	"setrd	%0, %1")

;; FIXME
(define_expand "adddi3"
  [(set      (match_operand:DI 0 "register_operand"	"= Rd,Rd,Rd,Rb,Rb,Rb")
    (plus:DI (match_operand:DI 1 "register_operand"	"% Rd,Rd,Rd,Rb,Rb,Rb")
             (match_operand:DI 2 "general_operand"	"  Rd, i,It,Rd, i,It")))]
	""
	"{
		if (REG_P(operands[0]) &&
		    REGNO_REG_CLASS(REGNO(operands[0]))==POINTER_REGS) {
			if (REG_P(operands[2]) &&
			    REGNO_REG_CLASS(REGNO(operands[2]))==GENERAL_REGS) 
				emit_insn (gen_addrb_rd(operands[0], operands[1], operands[2]));
			else	emit_insn (gen_addrb_imm(operands[0], operands[1], operands[2]));
			DONE;
		}
		if (REG_P(operands[0]) &&
		    REGNO_REG_CLASS(REGNO(operands[0]))==GENERAL_REGS) {
			if (REG_P(operands[2]) &&
			    REGNO_REG_CLASS(REGNO(operands[2]))==GENERAL_REGS)
				emit_insn (gen_dd_addrd(operands[0], operands[1], operands[2]));
			DONE;
		}
	}")

(define_insn "dd_addrd"
  [(set      (match_operand:DI 0 "rd_class_operand" "=Rd")
    (plus:DI (match_operand:DI 1 "rd_class_operand" "%Rd")
             (match_operand:DI 2 "general_operand"  "iRd")))]
	""
	{
	  if (REG_P(operands[2])) return "add	rd0, %0, %1, %2";
	  if (GET_CODE(operands[2])==CONST_INT) {
		if (!satisfies_constraint_It(operands[2]))	return "setrd	%0, %2	\;add	rd0, %0, %1, %0";
		else {
			if (operands[0] == operands[1])		return "add	%0, %2";
			return "setrd   %0, %2  \;add   rd0, %0, %1, %0";
		}
	  }
	})

;; FIXME
(define_insn "subdi3"
  [(set       (match_operand:DI 0 "rd_class_operand"	"= Rd,Rd")
    (minus:DI (match_operand:DI 1 "rd_class_operand"	"  Rd,Rd")
              (match_operand:DI 2 "general_operand"	"  Rd, i")))]
	""
	"@
	sub	rd0, %0, %1, %2
	setrd	%0, %2	\;add	rd0, %0, %1, %0	\;")

(define_insn "muldi3"
  [(set      (match_operand:DI 0 "rd_class_operand" "= Rd")
    (mult:DI (match_operand:DI 1 "rd_class_operand" "% Rd")
             (match_operand:DI 2 "rd_class_operand" "  Rd")))
   (set      (match_scratch:DI 3                    "= Rd")
             (us_mult:DI (match_dup 1) (match_dup 2)))]
	""
	"mulu	%0, %1, %2, %3")

(define_insn "divmoddi4"
  [(set     (match_operand:DI 0 "rd_class_operand" "= Rd")
    (div:DI (match_operand:DI 1 "rd_class_operand" "  Rd")
            (match_operand:DI 2 "rd_class_operand" "  Rd")))
   (set     (match_operand:DI 3 "rd_class_operand" "= Rd")
            (mod:DI (match_dup 1) (match_dup 2)))]
	""
	"divs	%0, %1, %2, %3")

(define_insn "udivmoddi4"
  [(set      (match_operand:DI 0 "rd_class_operand" "= Rd")
    (udiv:DI (match_operand:DI 1 "rd_class_operand" "  Rd")
             (match_operand:DI 2 "rd_class_operand" "  Rd")))
   (set      (match_operand:DI 3 "rd_class_operand" "= Rd")
             (umod:DI (match_dup 1) (match_dup 2)))]
	""
	"divu	%0, %1, %2, %3")

(define_insn "<rd_shift_name>di3"
  [(set          (match_operand:DI 0 "rd_class_operand" "=   Rd")
    (RD_SHIFT:DI (match_operand:DI 1 "rd_class_operand" "    Rd")
                 (match_operand:DI 2 "dd_rd_u6_operand" "  JsRd")))]
	""
	"<rd_shift_insn>	%0, %1, %2")

(define_insn "negdi2"
  [(set     (match_operand:DI 0 "rd_class_operand" "= Rd")
    (neg:DI (match_operand:DI 1 "rd_class_operand" "  Rd")))]
	""
	"sub	rd0, %0, rd0, %1")

(define_insn "one_cmpldi2"
  [(set     (match_operand:DI 0 "rd_class_operand" "= Rd")
    (not:DI (match_operand:DI 1 "rd_class_operand" "  Rd")))]
	""
	"not	%0, %1, rd0")

(define_insn "*nor"
  [(set             (match_operand:DI 0 "rd_class_operand" "=   Rd")
    (and:DI (not:DI (match_operand:DI 1 "rd_class_operand" "%   Rd"))
            (not:DI (match_operand:DI 2 "rd_class_operand" "    Rd"))))]
	""
	"orr	%0, %1, %2	\;	not	%0, %0, rd0")
