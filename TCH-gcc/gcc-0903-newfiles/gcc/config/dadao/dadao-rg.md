;; vim: filetype=lisp

;; GCC machine description for DADAO rg class
;; Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
;; Contributed by Guan Xuetao <gxt@pku.edu.cn>

;; seto is pseudo insn, so we place ? before Rg
(define_insn "mov_ri<mode>"
  [(set (match_operand:QHSD 0 "rg_class_operand"  "= Rg, Rg, ??Rg")
        (match_operand:QHSD 1 "const_int_operand" "  Iw, In,    n"))]
	""
	"@
	%s1	%0, %v1
	subu	%0, zero, %n1
	seto	%0, %1")

(define_insn "adddi3"
  [(set      (match_operand:DI 0 "rg_class_operand" "=   Rg, Rg, Rg")
    (plus:DI (match_operand:DI 1 "rg_class_operand" "%   Rg, Rg,  0")
             (match_operand:DI 2 "dd_ii_ri_operand" "  IdRg, In, Iw")))]
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
  [(set      (match_operand:DI 0 "rg_class_operand" "=   Rg")
    (mult:DI (match_operand:DI 1 "rg_class_operand" "%   Rg")
             (match_operand:DI 2 "dd_ii_ri_operand" "  IdRg")))
   (clobber  (match_scratch:DI 3                    "=   Sz"))]
	""
	"mulu	%0, %1, %2")

(define_insn "anddi3"
  [(set     (match_operand:DI 0 "rg_class_operand" "=   Rg")
    (and:DI (match_operand:DI 1 "rg_class_operand" "%   Rg")
            (match_operand:DI 2 "dd_ii_ri_operand" "  IdRg")))]
	""
	"and	%0, %1, %2")

(define_insn "iordi3"
  [(set     (match_operand:DI 0 "rg_class_operand" "=   Rg")
    (ior:DI (match_operand:DI 1 "rg_class_operand" "%   Rg")
            (match_operand:DI 2 "dd_ii_ri_operand" "  IdRg")))]
	""
	"or	%0, %1, %2")

(define_insn "xordi3"
  [(set     (match_operand:DI 0 "rg_class_operand" "=   Rg")
    (xor:DI (match_operand:DI 1 "rg_class_operand" "%   Rg")
            (match_operand:DI 2 "dd_ii_ri_operand" "  IdRg")))]
	""
	"xor	%0, %1, %2")

(define_insn "ashldi3"
  [(set        (match_operand:DI 0 "rg_class_operand" "=   Rg")
    (ashift:DI (match_operand:DI 1 "rg_class_operand" "    Rg")
               (match_operand:DI 2 "dd_rg_u6_operand" "  IsRg")))]
	""
	"slu	%0, %1, %2")

(define_insn "ashrdi3"
  [(set          (match_operand:DI 0 "rg_class_operand" "=   Rg")
    (ashiftrt:DI (match_operand:DI 1 "rg_class_operand" "    Rg")
                 (match_operand:DI 2 "dd_rg_u6_operand" "  IsRg")))]
	""
	"sr	%0, %1, %2")

(define_insn "lshrdi3"
  [(set          (match_operand:DI 0 "rg_class_operand" "=   Rg")
    (lshiftrt:DI (match_operand:DI 1 "rg_class_operand" "    Rg")
                 (match_operand:DI 2 "dd_rg_u6_operand" "  IsRg")))]
	""
	"sru	%0, %1, %2")

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
