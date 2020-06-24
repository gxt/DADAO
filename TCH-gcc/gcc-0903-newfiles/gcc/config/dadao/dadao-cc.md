;; vim: filetype=lisp

;; GCC machine description for DADAO cc-related
;; Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
;; Contributed by Guan Xuetao <gxt@pku.edu.cn>

;; We need to be able to move around the values used as condition codes.
;; First spotted as reported in
;; <URL:http://gcc.gnu.org/ml/gcc-bugs/2003-03/msg00008.html> due to
;; changes in loop optimization.  The file machmode.def says they're of
;; size 4 QI.  Valid bit-patterns correspond to integers -1, 0 and 1, so
;; we treat them as signed entities; see dadao-modes.def.  The following
;; expanders should cover all MODE_CC modes, and expand for this pattern.
(define_insn "*movcc_expanded"
  [(set (match_operand 0 "nonimmediate_operand" "=Rg,Rs,Rg,Rp,Rg,Rg,m")
	(match_operand 1 "nonimmediate_operand"  "Rg,Rg,Rs,Rg,Rp,m,Rg"))]
	"GET_MODE_CLASS (GET_MODE (operands[0])) == MODE_CC
	&& GET_MODE_CLASS (GET_MODE (operands[1])) == MODE_CC"
  "@
	or	%0, %1, 0
	put.rs	%0, %1
	get.rs	%0, %1
	put.rp	%0, %1
	get.rp	%0, %1
	ldt	%0, %1
	stt	%1, %0")

(define_expand "movcc"
  [(set (match_operand:CC 0 "nonimmediate_operand" "")
	(match_operand:CC 1 "nonimmediate_operand" ""))]
  ""
  "")

(define_expand "movcc_uns"
  [(set (match_operand:CC_UNS 0 "nonimmediate_operand" "")
	(match_operand:CC_UNS 1 "nonimmediate_operand" ""))]
  ""
  "")

(define_expand "movcc_fp"
  [(set (match_operand:CC_FP 0 "nonimmediate_operand" "")
	(match_operand:CC_FP 1 "nonimmediate_operand" ""))]
  ""
  "")

(define_expand "movcc_fpeq"
  [(set (match_operand:CC_FPEQ 0 "nonimmediate_operand" "")
	(match_operand:CC_FPEQ 1 "nonimmediate_operand" ""))]
  ""
  "")

(define_expand "movcc_fun"
  [(set (match_operand:CC_FUN 0 "nonimmediate_operand" "")
	(match_operand:CC_FUN 1 "nonimmediate_operand" ""))]
  ""
  "")

(define_insn "*cmps"
  [(set         (match_operand:CC 0 "rg_class_operand" "=   Rg")
    (compare:CC (match_operand:DI 1 "rg_class_operand" "    Rg")
                (match_operand:DI 2 "dd_rg_s12_operand" "  IdRg")))]
	""
	"cmp	%0, %1, %2")

(define_insn "*cmps2"
  [(set         (match_operand:CC 0 "rg_class_operand" "= Rg")
    (compare:CC (match_operand:DI 1 "rg_class_operand" "  Rg")
                (match_operand:DI 2 "immediate_operand" "  i")))]
	""
	"seto	datao1, %2	\;	cmp	%0, %1, datao1")

(define_insn "*cmpu"
  [(set             (match_operand:CC_UNS 0 "rg_class_operand" "=   Rg")
    (compare:CC_UNS (match_operand:DI     1 "rg_class_operand" "    Rg")
                    (match_operand:DI     2 "dd_rg_u12_operand" "  JdRg")))]
	""
	"cmpu	%0, %1, %2")

(define_expand "movdicc"
  [(set (match_dup 4) (match_dup 5))
   (set (match_operand:DI 0 "rg_class_operand" "")
	(if_then_else:DI
	 (match_operand 1 "comparison_operator" "")
	 (match_operand:DI 2 "dd_rg_u12_operand" "")
	 (match_operand:DI 3 "dd_rg_u12_operand" "")))]
  ""
  "
{
  enum rtx_code code = GET_CODE (operands[1]);
  if (code == LE || code == GE)
    FAIL;

  operands[4] = dadao_gen_compare_reg (code, XEXP (operands[1], 0),
				      XEXP (operands[1], 1));
  operands[5] = gen_rtx_COMPARE (GET_MODE (operands[4]),
				 XEXP (operands[1], 0),
				 XEXP (operands[1], 1));
  operands[1] = gen_rtx_fmt_ee (code, VOIDmode, operands[4], const0_rtx);
}")

;; FIXME: Is this the right way to do "folding" of CCmode -> DImode?
(define_insn "*movdicc_real_foldable"
  [(set (match_operand:DI 0 "rg_class_operand" "=Rg,Rg,Rg,Rg")
	(if_then_else:DI
	 (match_operator 2 "dadao_foldable_comparison_operator"
			 [(match_operand:DI 3 "rg_class_operand" "Rg,Rg,Rg,Rg")
			  (const_int 0)])
	 (match_operand:DI 1 "dd_rg_u12_operand" "JdRg,0 ,JdRg,GzIz")
	 (match_operand:DI 4 "dd_rg_u12_operand" "0 ,JdRg,GzIz,JdRg")))]
  ""
  "@
	cs.%d2	%0, %3, %1
	cs.%D2	%0, %3, %4
	xor	%0, %0, %0	\;	cs.%d2	%0, %3, %1
	xor	%0, %0, %0	\;	cs.%D2	%0, %3, %4")

(define_insn "*movdicc_real_reversible"
  [(set
    (match_operand:DI 0 "rg_class_operand"	   "=Rg,Rg,Rg,Rg")
    (if_then_else:DI
     (match_operator 2 "dadao_comparison_operator"
      [(match_operand 3 "dadao_reg_cc_operand"	    "Rg,Rg,Rg,Rg")
      (const_int 0)])
     (match_operand:DI 1 "dd_rg_u12_operand" "JdRg,0 ,JdRg,GzIz")
     (match_operand:DI 4 "dd_rg_u12_operand" "0 ,JdRg,GzIz,JdRg")))]
  "REVERSIBLE_CC_MODE (GET_MODE (operands[3]))"
  "@
	cs.%d2	%0, %3, %1
	cs.%D2	%0, %3, %4
	xor	%0, %0, %0	\;	cs.%d2	%0, %3, %1
	xor	%0, %0, %0	\;	cs.%D2	%0, %3, %4")

(define_insn "*movdicc_real_nonreversible"
  [(set
    (match_operand:DI 0 "rg_class_operand"	   "=Rg,Rg")
    (if_then_else:DI
     (match_operator 2 "dadao_comparison_operator"
      [(match_operand 3 "dadao_reg_cc_operand"	    "Rg,Rg")
      (const_int 0)])
     (match_operand:DI 1 "dd_rg_u12_operand" "JdRg,JdRg")
     (match_operand:DI 4 "dadao_rf_or_0_operand" "0 ,GzIz")))]
  "!REVERSIBLE_CC_MODE (GET_MODE (operands[3]))"
  "@
	cs.%d2	%0, %3, %1
	xor	%0, %0, %0	\;	cs.%d2	%0, %3, %1")

;; FIXME: scc insns will probably help, I just skip them
;; right now.  Revisit.

(define_expand "cbranchdi4"
  [(set (match_dup 4)
        (match_op_dup 5
         [(match_operand:DI 1 "rg_class_operand" "")
          (match_operand:DI 2 "dd_rg_u12_operand" "")]))
   (set (pc)
        (if_then_else
              (match_operator 0 "ordered_comparison_operator"
               [(match_dup 4)
                (const_int 0)])
              (label_ref (match_operand 3 "" ""))
              (pc)))]
  ""
  "
{
  operands[4] = dadao_gen_compare_reg (GET_CODE (operands[0]),
                                      operands[1], operands[2]);
  operands[5] = gen_rtx_fmt_ee (COMPARE,
                                GET_MODE (operands[4]),
                                operands[1], operands[2]);
}")


;; FIXME: we can emit an unordered-or-*not*-equal compare in one insn, but
;; there's no RTL code for it.  Maybe revisit in future.

;; FIXME: Odd/Even matchers?
(define_insn "*bCC_foldable"
  [(set (pc)
	(if_then_else
	 (match_operator 1 "dadao_foldable_comparison_operator"
			 [(match_operand:DI 2 "rg_class_operand" "Rg")
			  (const_int 0)])
	 (label_ref (match_operand 0 "" ""))
	 (pc)))]
  ""
	"br.%d1	%2, %0")

(define_insn "*bCC"
  [(set (pc)
	(if_then_else
	 (match_operator 1 "dadao_comparison_operator"
			 [(match_operand 2 "dadao_reg_cc_operand" "Rg")
			  (const_int 0)])
	 (label_ref (match_operand 0 "" ""))
	 (pc)))]
  ""
	"br.%d1	%2, %0")

(define_insn "*bCC_inverted_foldable"
  [(set (pc)
	(if_then_else
	 (match_operator 1 "dadao_foldable_comparison_operator"
			 [(match_operand:DI 2 "rg_class_operand" "Rg")
			  (const_int 0)])
		      (pc)
		      (label_ref (match_operand 0 "" ""))))]
;; REVERSIBLE_CC_MODE is checked by dadao_foldable_comparison_operator.
  ""
	"br.%D1	%2, %0")

(define_insn "*bCC_inverted"
  [(set (pc)
	(if_then_else
	 (match_operator 1 "dadao_comparison_operator"
			 [(match_operand 2 "dadao_reg_cc_operand" "Rg")
			  (const_int 0)])
	 (pc)
	 (label_ref (match_operand 0 "" ""))))]
  "REVERSIBLE_CC_MODE (GET_MODE (operands[2]))"
	"br.%D1	%2, %0")

