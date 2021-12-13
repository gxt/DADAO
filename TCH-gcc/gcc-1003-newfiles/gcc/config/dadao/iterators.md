;; vim: filetype=lisp

;; Code and mode itertator and attribute definitions for DADAO
;; Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
;; Contributed by Guan Xuetao <gxt@pku.edu.cn>

(define_mode_iterator	QHSD		[QI HI SI DI])
(define_mode_attr	bwto		[(QI "b") (HI "w") (SI "t") (DI "o")])

(define_mode_iterator	SFDF		[SF DF])
(define_mode_attr	ftfo		[(SF "ft") (DF "fo")])

(define_code_iterator	RG_ARITH	[and ior xor])
(define_code_attr	rg_arith_insn	[(and "and") (ior "orr") (xor "xor")])

(define_code_iterator	RG_SHIFT	[ashift ashiftrt lshiftrt])
(define_code_attr	rg_shift_name	[(ashift "ashl") (ashiftrt "ashr") (lshiftrt "lshr")])
(define_code_attr	rg_shift_insn	[(ashift "shlu")  (ashiftrt "shrs")   (lshiftrt "shru")])

(define_mode_iterator	CC_ANY		[CCSS CCUU CCFF])

(define_code_iterator	CCSS_TYPE	[eq ne lt le gt ge])
(define_code_attr	ccss_type_insn	[(eq "z") (ne "nz") (lt "n") (le "np") (gt "p") (ge "nn")])

(define_code_iterator	CCUU_TYPE	[eq ne ltu leu gtu geu])
(define_code_attr	ccuu_type_insn	[(eq "z") (ne "nz") (ltu "n") (leu "np") (gtu "p") (geu "nn")])

(define_code_iterator	CCFF_TYPE	[eq ne lt le gt ge ordered unordered])
(define_code_attr	ccff_type_insn	[(eq "eq") (ne "ne") (lt "lt") (le "le") (gt "gt") (ge "ge") (ordered "or") (unordered "un")])
