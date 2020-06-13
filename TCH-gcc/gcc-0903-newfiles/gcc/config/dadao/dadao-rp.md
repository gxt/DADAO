;; vim: filetype=lisp

;; GCC machine description for DADAO rp class
;; Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
;; Contributed by Guan Xuetao <gxt@pku.edu.cn>

(define_insn "*addrp"
  [(set      (match_operand:DI 0 "rp_class_operand"  "= Rp, Rp")
    (plus:DI (match_operand:DI 1 "rp_class_operand"  "%  0, Rp")
             (match_operand:DI 2 "dd_rg_s12_operand" "  Ie, Rg")))]
	""
	"@
	add.rp	%0, %2
	add.rp	%0, %1, %2")

;; TODO: following should be removed after
(define_insn "*addrp2rg"
  [(set      (match_operand:DI 0 "rg_class_operand"  "= Rg, Rg")
    (plus:DI (match_operand:DI 1 "rp_class_operand"  "% Rp, Rp")
             (match_operand:DI 2 "dd_rg_s12_operand" "  Ie, Rg")))]
	""
	"@
	get.rp	datao, %1	\;	add	%0, datao, %2
	get.rp	datao, %1	\;	add	%0, datao, %2")
