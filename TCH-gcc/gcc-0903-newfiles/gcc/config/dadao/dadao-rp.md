;; vim: filetype=lisp

;; GCC machine description for DADAO rp class
;; Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
;; Contributed by Guan Xuetao <gxt@pku.edu.cn>

(define_insn "*addrp"
  [(set      (match_operand:DI 0 "rp_class_operand"  "= Rp, Rp")
    (plus:DI (match_operand:DI 1 "rp_class_operand"  "% Rp, Rp")
             (match_operand:DI 2 "dd_rg_s12_operand" "  Ie, Rg")))]
	""
	"@
	add.rp	%0, %1, %2
	add.rp	%0, %1, %2")
