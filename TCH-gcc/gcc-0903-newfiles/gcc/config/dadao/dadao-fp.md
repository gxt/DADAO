;; vim: filetype=lisp

;; GCC machine description for DADAO floating-point
;; Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
;; Contributed by Guan Xuetao <gxt@pku.edu.cn>

;; Note that we move around the float as a collection of bits; no
;; conversion to double.
(define_insn "movsf"
 [(set (match_operand:SF 0 "nonimmediate_operand" "=r,r,x,r,r,m,??r")
       (match_operand:SF 1 "general_operand"	   "r,G,r,x,m,r,F"))]
  ""
  "@
	or	%0, %1, 0
	setwl	%0, 0
	put	%0, %1
	get	%0, %1
	ldt	%0, %1
	stt	%1, %0
   %r0%I1")

(define_insn "movdf"
  [(set (match_operand:DF 0 "nonimmediate_operand" "=r,r,x,r,r,m,??r")
	(match_operand:DF 1 "general_operand"	    "r,G,r,x,m,r,F"))]
  ""
  "@
	or	%0, %1, 0
	setwl	%0, 0
	put	%0, %1
	get	%0, %1
	ldo	%0, %1
	sto	%1, %0
   %r0%I1")

