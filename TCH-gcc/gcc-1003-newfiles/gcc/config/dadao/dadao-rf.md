;; vim: filetype=lisp

;; GCC machine description for DADAO floating-point
;; Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
;; Contributed by Guan Xuetao <gxt@pku.edu.cn>

;; Note that we move around the float as a collection of bits; no
;; conversion to double.

(define_insn "mov_rr_ft"
  [(set (match_operand:SI 0 "register_operand" "= Rf, Rd")
        (match_operand:SI 1 "register_operand" "  Rd, Rf"))]
	"REGNO_REG_CLASS(REGNO(operands[0])) == FLOATING_REGS ||
	 REGNO_REG_CLASS(REGNO(operands[1])) == FLOATING_REGS"
	"@
	rd2rf	%0, %1, 0
	rf2rd	%0, %1, 0")

(define_insn "mov_rr_fo"
  [(set (match_operand:DI 0 "register_operand" "= Rf, Rd")
        (match_operand:DI 1 "register_operand" "  Rd, Rf"))]
	"REGNO_REG_CLASS(REGNO(operands[0])) == FLOATING_REGS ||
	 REGNO_REG_CLASS(REGNO(operands[1])) == FLOATING_REGS"
	"@
	rd2rf	%0, %1, 0
	rf2rd	%0, %1, 0")

(define_insn "dd_st<mode>"
  [(set (match_operand:SFDF 0 "memory_operand"	 "= Wi,Wz,Wg")
	(match_operand:SFDF 1 "rf_class_operand" "  Rf,Rf,Rf"))]
	""
	"@
	st<ftfo>	%1, %0
	st<ftfo>	%1, %0
	stm<ftfo>	%1, %0, 0")

(define_insn "dd_st<mode>_m2m"
  [(set (match_operand:SFDF 0 "memory_operand" "= Wi,Wz,Wg,m,m,m,m")
	(match_operand:SFDF 1 "memory_operand" "  m,m,m,m,Wi,Wz,Wg"))]
	"!reload_completed"
	"")

(define_insn "dd_st<mode>_i2m"
  [(set (match_operand:SFDF 0 "memory_operand"	  "= Wi,Wz,Wg")
	(match_operand:SFDF 1 "immediate_operand" "   i, i, i"))]
	"!reload_completed"
	"@
	setrd	rd1, %1	\;rd2rf	rf1, rd1, 0	\;st<ftfo>	rf1, %0
	setrd   rd1, %1 \;rd2rf rf1, rd1, 0	\;st<ftfo>	rf1, %0
	setrd   rd1, %1 \;rd2rf rf1, rd1, 0	\;stm<ftfo>	rf1, %0, 0")

(define_insn "dd_ld<mode>"
  [(set (match_operand:SFDF 0 "register_operand" "= Rd,Rf")
	(match_operand:SFDF 1 "memory_operand"))]
	""
	"")

(define_insn "mov<mode>imm"
  [(set (match_operand:SFDF 0 "register_operand" "= Rf,Rd")
	(match_operand:SFDF 1 "const_double_operand"))]
	""
	"@
	setrd	rd1, %1\;rd2rf	%0, rd1, 0
	setrd	%0, %1")

; TODO
(define_insn "mov<mode>"
  [(set (match_operand:SFDF 0 "nonimmediate_operand" "= Rd,Rd,Rf,Wi,Wz,Wg")
	(match_operand:SFDF 1 "general_operand"      "  Rd,Rf,Rd,Rd,Rd,Rd"))]
	""
	"@
	orr	%0, %1, rd0
	rf2rd	%0, %1, 0
	rd2rf	%0, %1, 0
	sto	%1, %0
	sto	%1, %0
	stmo	%1, %0, 0")

(define_insn "add<mode>3"
  [(set        (match_operand:SFDF 0 "rf_class_operand" "= Rf")
    (plus:SFDF (match_operand:SFDF 1 "rf_class_operand" "% Rf")
               (match_operand:SFDF 2 "rf_class_operand" "  Rf")))]
	""
	"<ftfo>add	%0, %1, %2")

(define_insn "sub<mode>3"
  [(set         (match_operand:SFDF 0 "rf_class_operand" "= Rf")
    (minus:SFDF (match_operand:SFDF 1 "rf_class_operand" "  Rf")
                (match_operand:SFDF 2 "rf_class_operand" "  Rf")))]
	""
	"<ftfo>sub	%0, %1, %2")

(define_insn "mul<mode>3"
  [(set        (match_operand:SFDF 0 "rf_class_operand" "= Rf")
    (mult:SFDF (match_operand:SFDF 1 "rf_class_operand" "  Rf")
               (match_operand:SFDF 2 "rf_class_operand" "  Rf")))]
	""
	"<ftfo>mul	%0, %1, %2")

(define_insn "div<mode>3"
  [(set       (match_operand:SFDF 0 "rf_class_operand" "= Rf")
    (div:SFDF (match_operand:SFDF 1 "rf_class_operand" "  Rf")
              (match_operand:SFDF 2 "rf_class_operand" "  Rf")))]
	""
	"<ftfo>div	%0, %1, %2")

(define_insn "abs<mode>2"
  [(set       (match_operand:SFDF 0 "rf_class_operand" "= Rf")
    (abs:SFDF (match_operand:SFDF 1 "rf_class_operand" "  Rf")))]
	""
	"<ftfo>abs	%0, %1, 0")

(define_insn "neg<mode>2"
  [(set       (match_operand:SFDF 0 "rf_class_operand" "= Rf")
    (neg:SFDF (match_operand:SFDF 1 "rf_class_operand" "  Rf")))]
	""
	"<ftfo>neg	%0, %1, 0")

(define_insn "sqrt<mode>2"
  [(set        (match_operand:SFDF 0 "rf_class_operand" "= Rf")
    (sqrt:SFDF (match_operand:SFDF 1 "rf_class_operand" "  Rf")))]
	""
	"<ftfo>sqrt	%0, %1, 0")

(define_insn "floatdi<mode>2"
  [(set         (match_operand:SFDF 0 "rf_class_operand" "= Rf")
    (float:SFDF (match_operand:DI   1 "rd_class_operand" "  Rd")))]
	"" "")
;	"<ftfo>_i2f	%1, %0")

(define_insn "floatundi<mode>2"
  [(set                  (match_operand:SFDF 0 "rf_class_operand" "= Rf")
    (unsigned_float:SFDF (match_operand:DI   1 "rd_class_operand" "  Rd")))]
	"" "")
;	"<ftfo>_u2f	%1, %0")

(define_insn "fix<mode>di2"
  [(set     (match_operand:DI   0 "rd_class_operand" "= Rd")
    (fix:DI (match_operand:SFDF 1 "rf_class_operand" "  Rf")))]
	"" "")
;	"<ftfo>_f2i	%0, %1")

(define_insn "fix<mode>undi2"
  [(set              (match_operand:DI   0 "rd_class_operand" "= Rd")
    (unsigned_fix:DI (match_operand:SFDF 1 "rf_class_operand" "  Rf")))]
	"" "")
;	"<ftfo>_f2u	%0, %1")

(define_insn "truncdfsf2"
  [(set                (match_operand:SF 0 "rf_class_operand" "= Rf")
    (float_truncate:SF (match_operand:DF 1 "rf_class_operand" "  Rf")))]
	""
	"fo2ft	%0, %1, 0")

(define_insn "extendsfdf2"
  [(set              (match_operand:DF 0 "rf_class_operand" "= Rf")
    (float_extend:DF (match_operand:SF 1 "rf_class_operand" "  Rf")))]
	""
	"ft2fo	%0, %1, 0")

(define_insn "*fcmp_<ccff_type_insn>_<mode>"
  [(set             (match_operand:CCFF 0 "rd_class_operand" "= Rd")
    (CCFF_TYPE:CCFF (match_operand:SFDF 1 "rf_class_operand" "  Rf")
                    (match_operand:SFDF 2 "rf_class_operand" "  Rf")))]
	""
	"<ftfo>c<ccff_type_insn>	%0, %1, %2")

(define_insn "*br_ccff"
  [(set (pc)
    (if_then_else
      (match_operator 1 "ccff_comparison_operator"
        [(match_operand:CCFF 2 "rd_class_operand" "Rd") (const_int 0)])
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
