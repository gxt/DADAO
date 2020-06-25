;; vim: filetype=lisp

;; Code and mode itertator and attribute definitions for DADAO
;; Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
;; Contributed by Guan Xuetao <gxt@pku.edu.cn>

(define_mode_iterator	QHSD	[QI HI SI DI])
(define_mode_attr	bwto	[(QI "b") (HI "w") (SI "t") (DI "o")])

(define_code_iterator RG_ARITH [and ior xor])
(define_code_attr rg_arith_insn [(and "and") (ior "or") (xor "xor")])

(define_code_iterator RG_SHIFT [ashift ashiftrt lshiftrt])
(define_code_attr rg_shift_name [(ashift "ashl") (ashiftrt "ashr") (lshiftrt "lshr")])
(define_code_attr rg_shift_insn [(ashift "slu")  (ashiftrt "sr")   (lshiftrt "sru")])

(define_code_iterator CCSS_TYPE [eq ne lt le gt ge])
(define_code_attr ccss_type_insn [(eq "z") (ne "nz") (lt "n") (le "np") (gt "p") (ge "nn")])

(define_code_iterator CCUU_TYPE [eq ne ltu leu gtu geu])
(define_code_attr ccuu_type_insn [(eq "z") (ne "nz") (ltu "n") (leu "np") (gtu "p") (geu "nn")])
