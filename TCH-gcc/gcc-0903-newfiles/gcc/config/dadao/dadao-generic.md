;; vim: filetype=lisp

;; GCC machine description for DADAO
;; Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
;; Contributed by Guan Xuetao <gxt@pku.edu.cn>

(define_insn "nop"
  [(const_int 0)]
	""
	"nop")

(define_insn "mov<mode>"
  [(set (match_operand:QHSD 0 "register_operand" "= Rg, Rs, Rg")
        (match_operand:QHSD 1 "register_operand" "  Rg, Rg, Rs"))]
	""
	"@
	or	%0, %1, 0
	put	%0, %1
	get	%0, %1")

