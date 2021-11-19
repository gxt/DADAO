;; vim: filetype=lisp

;; GCC machine description for DADAO floating-point
;; Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
;; Contributed by Guan Xuetao <gxt@pku.edu.cn>

(define_insn "mov_rv2rv"
  [(set (match_operand:DI 0 "rv_class_operand" "= Rv")
        (match_operand:DI 1 "rv_class_operand" "  Rv"))]
	""
	"rv_v2v	%0, %1")

(define_insn "mov_rg2rv"
  [(set (match_operand:DI 0 "rv_class_operand" "= Rv")
        (match_operand:DI 1 "rg_class_operand" "  Rg"))]
	""
	"rv_g2v	%0, %1")

(define_insn "mov_rv2rg"
  [(set (match_operand:DI 0 "rg_class_operand" "= Rg")
        (match_operand:DI 1 "rv_class_operand" "  Rv"))]
	""
	"rv_v2g	%0, %1")
