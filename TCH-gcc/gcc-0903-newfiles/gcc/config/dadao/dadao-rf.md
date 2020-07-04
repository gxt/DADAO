;; vim: filetype=lisp

;; GCC machine description for DADAO floating-point
;; Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
;; Contributed by Guan Xuetao <gxt@pku.edu.cn>

;; Note that we move around the float as a collection of bits; no
;; conversion to double.

(define_mode_iterator	SFDF	[SF DF])
(define_mode_attr	ftfo	[(SF "ft") (DF "fo")])

(define_insn "mov<mode>"
 [(set (match_operand:SFDF 0 "nonimmediate_operand" "= Rg, Rg, Rf, Rf, Rf, Rg, Rf,  m,  m, ??Rg, ??Rf")
       (match_operand:SFDF 1 "general_operand"      "  Rg, Rf, Rg, Rf, Gz,  m,  m, Rg, Rf,    F,    F"))]
	""
	"@
	or	%0, %1, 0
	<ftfo>_get	%0, %1
	<ftfo>_put	%0, %1
	<ftfo>_2<ftfo>	%0, %1
	<ftfo>_put	%0, zero
	ldt	%0, %1
	<ftfo>_ld	%0, %1
	stt	%1, %0
	<ftfo>_st	%1, %0
	seto	%0, %1
	seto	datao1, %1	\;	<ftfo>_put	%0, datao1")

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

(define_insn "*fcmp"
  [(set            (match_operand:CC_FP 0 "rg_class_operand" "= Rg")
    (compare:CC_FP (match_operand:DF    1 "rf_class_operand" "  Rf")
                   (match_operand:DF    2 "rf_class_operand" "  Rf")))]
	""
	"fcmp%e0	%0, %1, %2")

(define_insn "*feql"
  [(set              (match_operand:CC_FPEQ 0 "rg_class_operand" "= Rg")
    (compare:CC_FPEQ (match_operand:DF      1 "rf_class_operand" "  Rf")
                     (match_operand:DF      2 "rf_class_operand" "  Rf")))]
	""
	"feql%e0	%0, %1, %2")

(define_insn "*fun"
  [(set             (match_operand:CC_FUN 0 "rg_class_operand" "= Rg")
    (compare:CC_FUN (match_operand:DF     1 "rf_class_operand" "  Rf")
                    (match_operand:DF     2 "rf_class_operand" "  Rf")))]
	""
	"fun%e0	%0, %1, %2")

(define_expand "cbranchdf4"
  [(set (match_dup 4)
        (match_op_dup 5
         [(match_operand:DF 1 "rf_class_operand" "")
          (match_operand:DF 2 "rf_class_operand" "")]))
   (set (pc)
        (if_then_else
              (match_operator 0 "float_comparison_operator"
               [(match_dup 4)
                (const_int 0)])
              (label_ref (match_operand 3 "" ""))
              (pc)))]
  ""
  "
{
  /* The head comment of optabs.c:can_compare_p says we're required to
     implement this, so we have to clean up the mess here.  */
  if (GET_CODE (operands[0]) == LE || GET_CODE (operands[0]) == GE)
    {
      enum rtx_code ltgt_code = GET_CODE (operands[0]) == LE ? LT : GT;
      emit_cmp_and_jump_insns (operands[1], operands[2], ltgt_code, NULL_RTX,
			       DFmode, 0, operands[3]);
      emit_cmp_and_jump_insns (operands[1], operands[2], EQ, NULL_RTX,
			       DFmode, 0, operands[3]);
      DONE;
    }

  operands[4] = dadao_gen_compare_reg (GET_CODE (operands[0]),
                                      operands[1], operands[2]);
  operands[5] = gen_rtx_fmt_ee (COMPARE,
                                GET_MODE (operands[4]),
                                operands[1], operands[2]);
}")

