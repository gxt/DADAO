;; vim: filetype=lisp

;; GCC machine description for DADAO floating-point
;; Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
;; Contributed by Guan Xuetao <gxt@pku.edu.cn>

;; Note that we move around the float as a collection of bits; no
;; conversion to double.

(define_insn "mov_rr_ft"
  [(set (match_operand:SI 0 "register_operand" "= Rf, Rg")
        (match_operand:SI 1 "register_operand" "  Rg, Rf"))]
	"REGNO_REG_CLASS(REGNO(operands[0])) == FLOATING_REGS ||
	 REGNO_REG_CLASS(REGNO(operands[1])) == FLOATING_REGS"
	"@
	ft_g2f	%0, %1
	ft_f2g	%0, %1")

(define_insn "mov_rr_fo"
  [(set (match_operand:DI 0 "register_operand" "= Rf, Rg")
        (match_operand:DI 1 "register_operand" "  Rg, Rf"))]
	"REGNO_REG_CLASS(REGNO(operands[0])) == FLOATING_REGS ||
	 REGNO_REG_CLASS(REGNO(operands[1])) == FLOATING_REGS"
	"@
	rg2rf	%0, %1
	rf2rg	%0, %1")

(define_insn "mov<mode>"
 [(set (match_operand:SFDF 0 "nonimmediate_operand" "= Rg, Rg, Rf, Rf, Rf, Rg, Rf,  m,  m, ??Rg, ??Rf")
       (match_operand:SFDF 1 "general_operand"      "  Rg, Rf, Rg, Rf, Gz,  m,  m, Rg, Rf,    F,    F"))]
	""
	"@
	orr	%0, %1, rg0
	<ftfo>_f2g	%0, %1
	<ftfo>_g2f	%0, %1
	<ftfo>_2<ftfo>	%0, %1
	<ftfo>_g2f	%0, zero
	ldt	%0, %1
	<ftfo>_ld	%0, %1
	stt	%1, %0
	<ftfo>_st	%1, %0
	seto	%0, %1
	seto	datao1, %1	\;	<ftfo>_g2f	%0, datao1")

(define_insn "add<mode>3"
  [(set        (match_operand:SFDF 0 "rf_class_operand" "= Rf")
    (plus:SFDF (match_operand:SFDF 1 "rf_class_operand" "% Rf")
               (match_operand:SFDF 2 "rf_class_operand" "  Rf")))]
	""
	"<ftfo>_add	%0, %1, %2")

(define_insn "sub<mode>3"
  [(set         (match_operand:SFDF 0 "rf_class_operand" "= Rf")
    (minus:SFDF (match_operand:SFDF 1 "rf_class_operand" "  Rf")
                (match_operand:SFDF 2 "rf_class_operand" "  Rf")))]
	""
	"<ftfo>_sub	%0, %1, %2")

(define_insn "mul<mode>3"
  [(set        (match_operand:SFDF 0 "rf_class_operand" "= Rf")
    (mult:SFDF (match_operand:SFDF 1 "rf_class_operand" "  Rf")
               (match_operand:SFDF 2 "rf_class_operand" "  Rf")))]
	""
	"<ftfo>_mul	%0, %1, %2")

(define_insn "div<mode>3"
  [(set       (match_operand:SFDF 0 "rf_class_operand" "= Rf")
    (div:SFDF (match_operand:SFDF 1 "rf_class_operand" "  Rf")
              (match_operand:SFDF 2 "rf_class_operand" "  Rf")))]
	""
	"<ftfo>_div	%0, %1, %2")

(define_insn "abs<mode>2"
  [(set       (match_operand:SFDF 0 "rf_class_operand" "= Rf")
    (abs:SFDF (match_operand:SFDF 1 "rf_class_operand" "  Rf")))]
	""
	"<ftfo>_abs	%0, %1")

(define_insn "neg<mode>2"
  [(set       (match_operand:SFDF 0 "rf_class_operand" "= Rf")
    (neg:SFDF (match_operand:SFDF 1 "rf_class_operand" "  Rf")))]
	""
	"<ftfo>_neg	%0, %1")

(define_insn "sqrt<mode>2"
  [(set        (match_operand:SFDF 0 "rf_class_operand" "= Rf")
    (sqrt:SFDF (match_operand:SFDF 1 "rf_class_operand" "  Rf")))]
	""
	"<ftfo>_sqrt	%0, %1")

(define_insn "floatdi<mode>2"
  [(set         (match_operand:SFDF 0 "rf_class_operand" "= Rf")
    (float:SFDF (match_operand:DI   1 "rg_class_operand" "  Rg")))]
	""
	"<ftfo>_i2f	%1, %0")

(define_insn "floatundi<mode>2"
  [(set                  (match_operand:SFDF 0 "rf_class_operand" "= Rf")
    (unsigned_float:SFDF (match_operand:DI   1 "rg_class_operand" "  Rg")))]
	""
	"<ftfo>_u2f	%1, %0")

(define_insn "fix<mode>di2"
  [(set     (match_operand:DI   0 "rg_class_operand" "= Rg")
    (fix:DI (match_operand:SFDF 1 "rf_class_operand" "  Rf")))]
	""
	"<ftfo>_f2i	%0, %1")

(define_insn "fix<mode>undi2"
  [(set              (match_operand:DI   0 "rg_class_operand" "= Rg")
    (unsigned_fix:DI (match_operand:SFDF 1 "rf_class_operand" "  Rf")))]
	""
	"<ftfo>_f2u	%0, %1")

(define_insn "truncdfsf2"
  [(set                (match_operand:SF 0 "rf_class_operand" "= Rf")
    (float_truncate:SF (match_operand:DF 1 "rf_class_operand" "  Rf")))]
	""
	"fo_2ft	%0, %1")

(define_insn "extendsfdf2"
  [(set              (match_operand:DF 0 "rf_class_operand" "= Rf")
    (float_extend:DF (match_operand:SF 1 "rf_class_operand" "  Rf")))]
	""
	"ft_2fo	%0, %1")

(define_insn "*fcmp_<ccff_type_insn>_<mode>"
  [(set             (match_operand:CCFF 0 "rg_class_operand" "= Rg")
    (CCFF_TYPE:CCFF (match_operand:SFDF 1 "rf_class_operand" "  Rf")
                    (match_operand:SFDF 2 "rf_class_operand" "  Rf")))]
	""
	"<ftfo>c_<ccff_type_insn>	%0, %1, %2")

(define_insn "*br_ccff"
  [(set (pc)
    (if_then_else
      (match_operator 1 "ccff_comparison_operator"
        [(match_operand:CCFF 2 "rg_class_operand" "Rg") (const_int 0)])
      (label_ref (match_operand 0 "" ""))
      (pc)))]
	""
	"brp	%2, %0")

(define_expand "cbranch<mode>4"
  [(set (match_dup 4)
        (match_op_dup 5 [(match_operand:SFDF 1 "rf_class_operand" "")
                         (match_operand:SFDF 2 "rf_class_operand" "")]))
   (set (pc)
        (if_then_else    (match_operator     0 "ccff_comparison_operator"
                        [(match_dup 4) (const_int 0)])
          (label_ref     (match_operand      3 "" ""))
          (pc)))]
	""
	"
{
	operands[4] = gen_reg_rtx (CCFFmode);
	operands[5] = gen_rtx_fmt_ee (GET_CODE(operands[0]), CCFFmode, operands[1], operands[2]);
}")
