;; vim: filetype=lisp

;; GCC machine description for DADAO rg class
;; Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
;; Contributed by Guan Xuetao <gxt@pku.edu.cn>

(define_insn "<code>di3"
  [(set          (match_operand:DI 0 "rg_class_operand" "=   Rg")
    (RG_ARITH:DI (match_operand:DI 1 "rg_class_operand" "%   Rg")
                 (match_operand:DI 2 "rg_class_operand" "    Rg")))]
	""
	"<rg_arith_insn>	%0, %1, %2")

;; setrg is pseudo insn, so we place ? before Rg
(define_insn "mov_ri<mode>"
  [(set (match_operand:QHSD 0 "rg_class_operand"  "=??Rg")
        (match_operand:QHSD 1 "const_int_operand" "    n"))]
	""
	"setrg	%0, %1")

;; FIXME
;; Fill in the output_template & constraint forms
(define_insn "adddi3"
  [(set      (match_operand:DI 0 "register_operand"	"= Rg,Rp,Rg,Rp")
    (plus:DI (match_operand:DI 1 "register_operand"	"  Rg,Rp,Rg,Rp")
             (match_operand:DI 2 "dd_arith_spns_op"	"  Rg,Rg,It,It")))]
	""
	"@
	add	rg0, %0, %1, %2
	addrp	%0, %1, %2
	* {	\
	return (operands[1] == operands[0]) ? \"add	%0, %2	\" :	\
					      \"add	%1, %2	\;rg2rg	%0, %1, 0	\;\" ;	}
	* {	\
	return (operands[1] == operands[0]) ? \"addrp	%0, %2	\" :	\
					      \"addrp	%1, %2	\;rp2rp	%0, %1, 0	\;\" ;	}")

;; FIXME
(define_insn "subdi3"
  [(set       (match_operand:DI 0 "rg_class_operand"	"= Rg,Rg")
    (minus:DI (match_operand:DI 1 "rg_class_operand"	"  Rg,Rg")
              (match_operand:DI 2 "general_operand"	"  Rg, i")))]
	""
	"@
	sub	rg0, %0, %1, %2
	setrg	rg7, %2	\;sub	rg0, %0, %1, rg7	\;")

(define_insn "muldi3"
  [(set      (match_operand:DI 0 "rg_class_operand" "= Rg")
    (mult:DI (match_operand:DI 1 "rg_class_operand" "% Rg")
             (match_operand:DI 2 "rg_class_operand" "  Rg")))
   (set      (match_scratch:DI 3                    "= Rg")
             (us_mult:DI (match_dup 1) (match_dup 2)))]
	""
	"mulu	%0, %1, %2, %3")

(define_insn "divmoddi4"
  [(set     (match_operand:DI 0 "rg_class_operand" "= Rg")
    (div:DI (match_operand:DI 1 "rg_class_operand" "  Rg")
            (match_operand:DI 2 "rg_class_operand" "  Rg")))
   (set     (match_operand:DI 3 "rg_class_operand" "= Rg")
            (mod:DI (match_dup 1) (match_dup 2)))]
	""
	"divs	%0, %1, %2, %3")

(define_insn "udivmoddi4"
  [(set      (match_operand:DI 0 "rg_class_operand" "= Rg")
    (udiv:DI (match_operand:DI 1 "rg_class_operand" "  Rg")
             (match_operand:DI 2 "rg_class_operand" "  Rg")))
   (set      (match_operand:DI 3 "rg_class_operand" "= Rg")
             (umod:DI (match_dup 1) (match_dup 2)))]
	""
	"divu	%0, %1, %2, %3")

(define_insn "<rg_shift_name>di3"
  [(set          (match_operand:DI 0 "rg_class_operand" "=   Rg")
    (RG_SHIFT:DI (match_operand:DI 1 "rg_class_operand" "    Rg")
                 (match_operand:DI 2 "dd_rg_u6_operand" "  JsRg")))]
	""
	"<rg_shift_insn>	%0, %1, %2")

(define_insn "negdi2"
  [(set     (match_operand:DI 0 "rg_class_operand" "= Rg")
    (neg:DI (match_operand:DI 1 "rg_class_operand" "  Rg")))]
	""
	"sub	rg0, %0, rg0, %1")

(define_insn "one_cmpldi2"
  [(set     (match_operand:DI 0 "rg_class_operand" "= Rg")
    (not:DI (match_operand:DI 1 "rg_class_operand" "  Rg")))]
	""
	"not	%0, %1, rg0")

(define_insn "*nor"
  [(set             (match_operand:DI 0 "rg_class_operand" "=   Rg")
    (and:DI (not:DI (match_operand:DI 1 "rg_class_operand" "%   Rg"))
            (not:DI (match_operand:DI 2 "rg_class_operand" "    Rg"))))]
	""
	"orr	%0, %1, %2	\;	not	%0, %0, rg0")
