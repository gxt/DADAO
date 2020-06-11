;; vim: filetype=lisp

;; GCC machine description for DADAO generic insns
;; Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
;; Contributed by Guan Xuetao <gxt@pku.edu.cn>

(define_insn "nop"
  [(const_int 0)]
	""
	"nop")

(define_insn "mov<mode>"
  [(set (match_operand:QHSD 0 "register_operand" "= Rg, Rp, Rg, Rf, Rg, Rv, Rg, Rs, Rg")
        (match_operand:QHSD 1 "register_operand" "  Rg, Rg, Rp, Rg, Rf, Rg, Rv, Rg, Rs"))]
	""
	"@
	or	%0, %1, 0
	put.rp	%0, %1
	get.rp	%0, %1
	put.rf	%0, %1
	get.rf	%0, %1
	put.rv	%0, %1
	get.rv	%0, %1
	put.rs	%0, %1
	get.rs	%0, %1")
