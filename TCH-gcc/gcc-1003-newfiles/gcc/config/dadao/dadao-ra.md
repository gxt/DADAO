;; vim: filetype=lisp

;; GCC machine description for DADAO floating-point
;; Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
;; Contributed by Guan Xuetao <gxt@pku.edu.cn>

(define_insn "mov_rd2ra"
  [(set (match_operand:DI 0 "ra_class_operand" "= Ra")
        (match_operand:DI 1 "rd_class_operand" "  Rd"))]
	""
	"rd2ra	%0, %1, 1")

(define_insn "mov_ra2rd"
  [(set (match_operand:DI 0 "rd_class_operand" "= Rd")
        (match_operand:DI 1 "ra_class_operand" "  Ra"))]
	""
	"ra2rd	%0, %1, 1")
