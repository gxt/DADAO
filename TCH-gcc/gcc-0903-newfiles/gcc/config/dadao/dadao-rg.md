;; vim: filetype=lisp

;; GCC machine description for DADAO rg class
;; Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
;; Contributed by Guan Xuetao <gxt@pku.edu.cn>

(define_insn "<code>di3"
  [(set          (match_operand:DI 0 "rg_class_operand" "=   Rg")
    (RG_ARITH:DI (match_operand:DI 1 "rg_class_operand" "%   Rg")
                 (match_operand:DI 2 "dd_ii_ri_operand" "  JdRg")))]
	""
	"<rg_arith_insn>	%0, %1, %2")

;; seto is pseudo insn, so we place ? before Rg
(define_insn "mov_ri<mode>"
  [(set (match_operand:QHSD 0 "rg_class_operand"  "= Rg, Rg, ??Rg")
        (match_operand:QHSD 1 "const_int_operand" "  Iw, Nd,    n"))]
	""
	"@
	%s1	%0, %v1
	subu	%0, zero, %n1
	seto	%0, %1")

(define_insn "adddi3"
  [(set      (match_operand:DI 0 "rg_class_operand" "=   Rg, Rg, Rg")
    (plus:DI (match_operand:DI 1 "rg_class_operand" "%   Rg, Rg,  0")
             (match_operand:DI 2 "dd_ii_ri_operand" "  JdRg, Nd, Iw")))]
	""
	"@
	addu	%0, %1, %2
	subu	%0, %1, %n2
	%i2	%0, %v2")

(define_insn "subdi3"
  [(set       (match_operand:DI 0 "rg_class_operand" "= Rg")
    (minus:DI (match_operand:DI 1 "rg_class_operand" "  Rg")
              (match_operand:DI 2 "rg_class_operand" "  Rg")))]
	""
	"subu	%0, %1, %2")

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
	"div	%0, %1, %2, %3")

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
	"subu	%0, zero, %1")

(define_insn "one_cmpldi2"
  [(set     (match_operand:DI 0 "rg_class_operand" "= Rg")
    (not:DI (match_operand:DI 1 "rg_class_operand" "  Rg")))]
	""
	"not	%0, %1, zero")

(define_insn "*nor"
  [(set             (match_operand:DI 0 "rg_class_operand" "=   Rg")
    (and:DI (not:DI (match_operand:DI 1 "rg_class_operand" "%   Rg"))
            (not:DI (match_operand:DI 2 "dd_ii_ri_operand" "  JdRg"))))]
	""
	"or	%0, %1, %2	\;	not	%0, %0, 0")
