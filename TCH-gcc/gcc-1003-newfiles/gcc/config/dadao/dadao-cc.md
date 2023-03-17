;; vim: filetype=lisp

;; GCC machine description for DADAO cc-related
;; Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
;; Contributed by Guan Xuetao <gxt@pku.edu.cn>

;; We need to be able to move around the values used as condition codes.
;; First spotted as reported in
;; <URL:http://gcc.gnu.org/ml/gcc-bugs/2003-03/msg00008.html> due to
;; changes in loop optimization.  The file machmode.def says they're of
;; size 4 QI.  Valid bit-patterns correspond to integers -1, 0 and 1, so
;; we treat them as signed entities; see dadao-modes.def.  The following
;; expanders should cover all MODE_CC modes, and expand for this pattern.
(define_insn "*movcc_expanded"
  [(set (match_operand 0 "nonimmediate_operand" "=Rd,Rb,Rd,Rd,m")
	(match_operand 1 "nonimmediate_operand"  "Rd,Rd,Rb,m,Rd"))]
	"GET_MODE_CLASS (GET_MODE (operands[0])) == MODE_CC
		&& GET_MODE_CLASS (GET_MODE (operands[1])) == MODE_CC"
	"@
	orr	%0, %1, rd0
	rd2rb	%0, %1, 1
	rb2rd	%0, %1, 1
	ldtu	%0, %1
	stt	%1, %0")

(define_expand "mov<mode>"
  [(set (match_operand:CC_ANY 0 "nonimmediate_operand" "")
	(match_operand:CC_ANY 1 "nonimmediate_operand" ""))]
	""
	"")

(define_insn "*cmpss"
  [(set           (match_operand:CCSS 0 "rd_class_operand"  "=   Rd")
    (compare:CCSS (match_operand:DI   1 "rd_class_operand"  "    Rd")
                  (match_operand:DI   2 "dd_rd_s12_operand" "  IdRd")))]
	""
	"cmps	%0, %1, %2")

(define_insn "*cmpss2"
  [(set           (match_operand:CCSS 0 "rd_class_operand"  "= Rd")
    (compare:CCSS (match_operand:DI   1 "rd_class_operand"  "  Rd")
                  (match_operand:DI   2 "const_int_operand" "   i")))]
	""
	"setrd	%0, %2	\;cmps	%0, %1, %0")

(define_insn "*cmpuu"
  [(set           (match_operand:CCUU 0 "rd_class_operand"  "=   Rd")
    (compare:CCUU (match_operand:DI   1 "rd_class_operand"  "    Rd")
                  (match_operand:DI   2 "dd_rd_u12_operand" "  JdRd")))]
	""
	"cmpu	%0, %1, %2")

(define_expand "movdicc"
  [(set (match_dup 4) (match_dup 5))
   (set              (match_operand:DI 0 "rd_class_operand" "")
    (if_then_else:DI (match_operand    1 "ordered_comparison_operator" "")
                     (match_operand:DI 2 "rd_class_operand" "")
                     (match_operand:DI 3 "rd_class_operand" "")))]
	""
{
	enum rtx_code code = GET_CODE (operands[1]);
	machine_mode mode = GET_MODE (XEXP(operands[1], 0));

	if ((code == LTU) || (code == LEU) || (code == GTU) || (code == GEU)) {
		operands[4] = gen_reg_rtx(CCUUmode);
		operands[5] = gen_rtx_COMPARE (CCUUmode, XEXP (operands[1], 0), XEXP (operands[1], 1));
	} else {
		operands[4] = gen_reg_rtx(mode);
		operands[5] = gen_rtx_COMPARE (mode, XEXP (operands[1], 0), XEXP (operands[1], 1));
	}

	operands[1] = gen_rtx_fmt_ee (code, VOIDmode, operands[4], const0_rtx);
	DONE;
})

(define_insn "*movdicc_<ccss_type_insn>"
  [(set (match_operand:DI            0 "rd_class_operand" "= Rd")
    (if_then_else:DI
      (CCSS_TYPE (match_operand:CCSS 1 "rd_class_operand" "  Rd") (const_int 0))
      (match_operand:DI              2 "rd_class_operand" "  Rd")
      (match_operand:DI              3 "rd_class_operand" "  Rd")))]
	""
	"cs<ccss_type_insn>	%0, %1, %2, %3")

(define_insn "*movdicc_<ccuu_type_insn>"
  [(set (match_operand:DI            0 "rd_class_operand" "= Rd")
    (if_then_else:DI
      (CCUU_TYPE (match_operand:CCUU 1 "rd_class_operand" "  Rd") (const_int 0))
      (match_operand:DI              2 "rd_class_operand" "  Rd")
      (match_operand:DI              3 "rd_class_operand" "  Rd")))]
	""
	"cs<ccuu_type_insn>	%0, %1, %2, %3")

(define_expand "cbranchdi4"
  [(set (match_dup 4)
        (match_op_dup 5 [(match_operand:DI 1 "rd_class_operand" "")
                         (match_operand:DI 2 "dd_rd_u12_operand" "")]))
   (set (pc)
     (if_then_else (match_operator 0 "ordered_comparison_operator" [(match_dup 4) (const_int 0)])
                   (label_ref (match_operand 3 "" ""))
                   (pc)))]
	""
{
	enum rtx_code code = GET_CODE (operands[0]);

	if ((code == LTU) || (code == LEU) || (code == GTU) || (code == GEU)) {
		operands[4] = gen_reg_rtx(CCUUmode);
		operands[5] = gen_rtx_fmt_ee (COMPARE, CCUUmode, operands[1], operands[2]);
	} else {
		operands[4] = gen_reg_rtx(CCSSmode);
		operands[5] = gen_rtx_fmt_ee (COMPARE, CCSSmode, operands[1], operands[2]);
	}
})

(define_insn "*br_ss_<ccss_type_insn>"
  [(set (pc)
    (if_then_else
      (CCSS_TYPE (match_operand:CCSS 1 "rd_class_operand" "Rd") (const_int 0))
      (label_ref (match_operand 0 "" ""))
      (pc)))]
	""
	"br<ccss_type_insn>	%1, %0")

(define_insn "*br_uu_<ccuu_type_insn>"
  [(set (pc)
    (if_then_else
      (CCUU_TYPE (match_operand:CCUU 1 "rd_class_operand" "Rd") (const_int 0))
      (label_ref (match_operand 0 "" ""))
      (pc)))]
	""
	"br<ccuu_type_insn>	%1, %0")
