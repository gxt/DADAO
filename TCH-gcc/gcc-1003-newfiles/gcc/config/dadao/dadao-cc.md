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
  [(set (match_operand 0 "nonimmediate_operand" "=Rg,Rp,Rg,Rg,m")
	(match_operand 1 "nonimmediate_operand"  "Rg,Rg,Rp,m,Rg"))]
	"GET_MODE_CLASS (GET_MODE (operands[0])) == MODE_CC
		&& GET_MODE_CLASS (GET_MODE (operands[1])) == MODE_CC"
	"@
	or	%0, %1, 0
	rp_g2p	%0, %1
	rp_p2g	%0, %1
	ldt	%0, %1
	stt	%1, %0")

(define_expand "mov<mode>"
  [(set (match_operand:CC_ANY 0 "nonimmediate_operand" "")
	(match_operand:CC_ANY 1 "nonimmediate_operand" ""))]
	""
	"")

(define_insn "*cmpss"
  [(set           (match_operand:CCSS 0 "rg_class_operand"  "=   Rg")
    (compare:CCSS (match_operand:DI   1 "rg_class_operand"  "    Rg")
                  (match_operand:DI   2 "dd_rg_s12_operand" "  IdRg")))]
	""
	"cmp	%0, %1, %2")

(define_insn "*cmpss2"
  [(set           (match_operand:CCSS 0 "rg_class_operand"  "= Rg")
    (compare:CCSS (match_operand:DI   1 "rg_class_operand"  "  Rg")
                  (match_operand:DI   2 "const_int_operand" "   i")))]
	""
	"seto	datao1, %2	\;	cmp	%0, %1, datao1")

(define_insn "*cmpuu"
  [(set           (match_operand:CCUU 0 "rg_class_operand"  "=   Rg")
    (compare:CCUU (match_operand:DI   1 "rg_class_operand"  "    Rg")
                  (match_operand:DI   2 "dd_rg_u12_operand" "  JdRg")))]
	""
	"cmpu	%0, %1, %2")

(define_expand "movdicc"
  [(set (match_dup 4) (match_dup 5))
   (set              (match_operand:DI 0 "rg_class_operand" "")
    (if_then_else:DI (match_operand    1 "ordered_comparison_operator" "")
                     (match_operand:DI 2 "rg_class_operand" "")
                     (match_operand:DI 3 "rg_class_operand" "")))]
	""
{
	enum rtx_code code = GET_CODE (operands[1]);

	if ((code == LTU) || (code == LEU) || (code == GTU) || (code == GEU)) {
		operands[4] = gen_reg_rtx(CCUUmode);
		operands[5] = gen_rtx_COMPARE (CCUUmode, XEXP (operands[1], 0), XEXP (operands[1], 1));
	} else {
		operands[4] = gen_reg_rtx(CCSSmode);
		operands[5] = gen_rtx_COMPARE (CCSSmode, XEXP (operands[1], 0), XEXP (operands[1], 1));
	}

	operands[1] = gen_rtx_fmt_ee (code, VOIDmode, operands[4], const0_rtx);
})

(define_insn "*movdicc_<ccss_type_insn>"
  [(set (match_operand:DI            0 "rg_class_operand" "= Rg")
    (if_then_else:DI
      (CCSS_TYPE (match_operand:CCSS 1 "rg_class_operand" "  Rg") (const_int 0))
      (match_operand:DI              2 "rg_class_operand" "  Rg")
      (match_operand:DI              3 "rg_class_operand" "  Rg")))]
	""
	"cs_<ccss_type_insn>	%0, %1, %2, %3")

(define_insn "*movdicc_<ccuu_type_insn>"
  [(set (match_operand:DI            0 "rg_class_operand" "= Rg")
    (if_then_else:DI
      (CCUU_TYPE (match_operand:CCUU 1 "rg_class_operand" "  Rg") (const_int 0))
      (match_operand:DI              2 "rg_class_operand" "  Rg")
      (match_operand:DI              3 "rg_class_operand" "  Rg")))]
	""
	"cs_<ccuu_type_insn>	%0, %1, %2, %3")

(define_expand "cbranchdi4"
  [(set (match_dup 4)
        (match_op_dup 5 [(match_operand:DI 1 "rg_class_operand" "")
                         (match_operand:DI 2 "dd_rg_u12_operand" "")]))
   (set (pc)
     (if_then_else (match_operator 0 "ordered_comparison_operator" [(match_dup 4) (const_int 0)])
                   (label_ref (match_operand 3 "" ""))
                   (pc)))]
	""
{
	enum rtx_code code = GET_CODE (operands[0]);

	if ((code == LTU) || (code == LEU) || (code == GTU) || (code == GEU)) {
		operands[4] = gen_reg_rtx(CCUUmode);
		operands[5] = gen_rtx_fmt_ee (COMPARE, CCUUmode, operands[1], operands[2]);
	} else {
		operands[4] = gen_reg_rtx(CCSSmode);
		operands[5] = gen_rtx_fmt_ee (COMPARE, CCSSmode, operands[1], operands[2]);
	}
})

(define_insn "*br_ss_<ccss_type_insn>"
  [(set (pc)
    (if_then_else
      (CCSS_TYPE (match_operand:CCSS 1 "rg_class_operand" "Rg") (const_int 0))
      (label_ref (match_operand 0 "" ""))
      (pc)))]
	""
	"br_<ccss_type_insn>	%1, %0")

(define_insn "*br_uu_<ccuu_type_insn>"
  [(set (pc)
    (if_then_else
      (CCUU_TYPE (match_operand:CCUU 1 "rg_class_operand" "Rg") (const_int 0))
      (label_ref (match_operand 0 "" ""))
      (pc)))]
	""
	"br_<ccuu_type_insn>	%1, %0")