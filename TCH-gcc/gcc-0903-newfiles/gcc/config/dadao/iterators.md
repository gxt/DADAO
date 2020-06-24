;; vim: filetype=lisp

;; Code and mode itertator and attribute definitions for DADAO
;; Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
;; Contributed by Guan Xuetao <gxt@pku.edu.cn>

(define_mode_iterator	QHSD	[QI HI SI DI])
(define_mode_attr	bwto	[(QI "b") (HI "w") (SI "t") (DI "o")])

(define_code_iterator RG_ARITH [and ior xor])
(define_code_attr rg_arith_insn [(and "and") (ior "or") (xor "xor")])
