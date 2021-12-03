;; vim: filetype=lisp

;; GCC machine description for DADAO floating-point
;; Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
;; Contributed by Guan Xuetao <gxt@pku.edu.cn>

(define_insn "mov_rr2rr"
  [(set (match_operand:DI 0 "rr_class_operand" "= Rr")
        (match_operand:DI 1 "rr_class_operand" "  Rr"))]
	""
	"rr_r2r	%0, %1")

(define_insn "mov_rg2rr"
  [(set (match_operand:DI 0 "rr_class_operand" "= Rr")
        (match_operand:DI 1 "rg_class_operand" "  Rg"))]
	""
	"rr_g2r	%0, %1")

(define_insn "mov_rr2rg"
  [(set (match_operand:DI 0 "rg_class_operand" "= Rg")
        (match_operand:DI 1 "rr_class_operand" "  Rr"))]
	""
	"rr_r2g	%0, %1")
