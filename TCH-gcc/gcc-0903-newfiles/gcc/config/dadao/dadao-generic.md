;; vim: filetype=lisp

;; GCC machine description for DADAO generic insns
;; Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
;; Contributed by Guan Xuetao <gxt@pku.edu.cn>

(define_insn "nop"
  [(const_int 0)]
	""
	"nop")

;; TODO: are there rp2rf, rf2rv, rv2rs, ... requirements?
(define_insn "mov<mode>"
  [(set (match_operand:QHSD 0 "register_operand" "= Rg, Rp, Rf, Rv, Rs, Rp, Rf, Rv, Rs, Rg, Rg, Rg, Rg")
        (match_operand:QHSD 1 "register_operand" "  Rg, Rp, Rf, Rv, Rs, Rg, Rg, Rg, Rg, Rp, Rf, Rv, Rs"))]
	""
	"@
	or	%0, %1, 0
	mov.rp	%0, %1
	mov.rf	%0, %1
	mov.rv	%0, %1
	mov.rs	%0, %1
	put.rp	%0, %1
	put.rf	%0, %1
	put.rv	%0, %1
	put.rs	%0, %1
	get.rp	%0, %1
	get.rf	%0, %1
	get.rv	%0, %1
	get.rs	%0, %1")
