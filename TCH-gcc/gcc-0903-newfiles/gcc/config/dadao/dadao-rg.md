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
             (match_operand:DI 2 "dd_ii_ri_operand" "  RgId, In, Iw")))]
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
             (match_operand:DI 2 "dd_ii_ri_operand" "  RgId")))
   (clobber  (match_scratch:DI 3                    "=    z"))]
	""
	"mulu	%0, %1, %2")
