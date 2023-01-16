;; vim: filetype=lisp

;; GCC machine description for DADAO rd class
;; Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
;; Contributed by Guan Xuetao <gxt@pku.edu.cn>

(define_insn "rd2rd_<mode>"
  [(set (match_operand:QHSD 0 "rd_class_operand" "=Rd")
        (match_operand:QHSD 1 "rd_class_operand" " Rd"))]
	""
	"rd2rd	%0, %1, 0")

(define_insn "<code>di3"
  [(set          (match_operand:DI 0 "rd_class_operand" "=   Rd")
    (RD_ARITH:DI (match_operand:DI 1 "rd_class_operand" "%   Rd")
                 (match_operand:DI 2 "rd_class_operand" "    Rd")))]
	""
	"<rd_arith_insn>	%0, %1, %2")

(define_insn "dd_ldimm_symbolic"
  [(set (match_operand:DI 0 "register_operand" "=g,Rb<,Rd")
        (match_operand:DI 1 "symbolic_operand" ""))]
	""
	"move	%0, %1")

(define_insn "dd_ldimm_const<mode>"
  [(set (match_operand:QHSD 0 "rd_class_operand"  "=Rd")
        (match_operand:QHSD 1 "const_int_operand" "  i"))]
	""
	"move	%0, %1")

(define_expand "adddi3"
  [(set      (match_operand:DI 0 "register_operand")
    (plus:DI (match_operand:DI 1 "register_operand")
             (match_operand:DI 2 "general_operand")))]
  ""
{
  if (can_create_pseudo_p())
    {
      if (MEM_P (operands[2])
	  || (CONSTANT_P (operands[2])
		&& (LABEL_REF_P (operands[2])
		|| SYMBOL_REF_P (operands[2])))
	  || (CONSTANT_P (operands[2])
		&& !CONST_INT_P (operands[2])))
	operands[2] =	\
		force_reg (Pmode, operands[2]);
    }
})

(define_insn "dd_addrd_mem"
  [(set      (match_operand:DI 0 "rd_class_operand" "=Rd,Rd")
    (plus:DI (match_operand:DI 1 "rd_class_operand" "%Rd,Rd")
             (match_operand:DI 2 "memory_operand"    "Wg,m")))]
        ""
	"@
	ldmo	%0, %2, 0	\;add	rd0, %0, %1, %0
	ldo	%0, %2	\;add	rd0, %0, %1, %0")

(define_insn "dd_addrd_const"
  [(set      (match_operand:DI 0 "rd_class_operand" "=Rd")
    (plus:DI (match_operand:DI 1 "rd_class_operand" "%Rd")
             (match_operand:DI 2 "dd_sign_18_operand" "i")))]
  ""
  {
	if(INTVAL(operands[2])>0x7ff || INTVAL(operands[2])<-0x800)
         return "move\trd7, %2 \;add\trd0, %0, %1, rd7";
	else
	 return "addi   %0, %1, %2";
  })
  

(define_insn "dd_addrd_regls"
  [(set      (match_operand:DI 0 "rd_class_operand" "=Rd,Rd")
    (plus:DI (match_operand:DI 1 "rd_class_operand" "%Rd,Rd")
             (match_operand:DI 2 "dd_rd_ls_operand"  "Rd, i")))]
  ""
  "@
  add	rd0, %0, %1, %2
  move	rd7, %2	\;add	rd0, %0, %1, rd7")

(define_insn "dd_addrd"
  [(set      (match_operand:DI 0 "rd_class_operand" "=Rd")
    (plus:DI (match_operand:DI 1 "rd_class_operand" "%Rd")
             (match_operand:DI 2 "rd_class_operand"  "Rd")))]
	""
	"add	rd0, %0, %1, %2")

(define_expand "subdi3"
  [(set      (match_operand:DI 0 "register_operand")
   (minus:DI (match_operand:DI 1 "register_operand")
             (match_operand:DI 2 "general_operand")))]
  ""
{
  if (can_create_pseudo_p())
    {
      if (MEM_P (operands[2])
	  || (CONSTANT_P (operands[2])
		&& (LABEL_REF_P (operands[2])
		|| SYMBOL_REF_P (operands[2])))
	  || (CONSTANT_P (operands[2])
		&& !CONST_INT_P (operands[2])))
	operands[2] =	\
		force_reg (Pmode, operands[2]);
    }
})

(define_insn "dd_subrd_mem"
  [(set      (match_operand:DI 0 "rd_class_operand" "=Rd,Rd")
   (minus:DI (match_operand:DI 1 "rd_class_operand" "%Rd,Rd")
             (match_operand:DI 2 "memory_operand"    "Wg,m")))]
        ""
	"@
	ldmo	%0, %2, 0	\;sub	rd0, %0, %1, %0
	ldo	%0, %2	\;sub	rd0, %0, %1, %0")

(define_insn "dd_subrd_const"
  [(set      (match_operand:DI 0 "rd_class_operand" "=Rd")
   (minus:DI (match_operand:DI 1 "rd_class_operand" "%Rd")
             (match_operand:DI 2 "dd_sign_18_operand" "i")))]
  	""
	{
        if(INTVAL(operands[2])>0x7ff || INTVAL(operands[2])<-0x800)
		return "move\trd7, %n2 \;add\trd0, %0, %1, rd7";
	else
		return "addi   %0, %1, %n2";
	})

(define_insn "dd_subrd_regls"
  [(set      (match_operand:DI 0 "rd_class_operand" "=Rd,Rd")
   (minus:DI (match_operand:DI 1 "rd_class_operand" "%Rd,Rd")
             (match_operand:DI 2 "dd_rd_ls_operand"  "Rd, i")))]
	""
	"@
	sub	rd0, %0, %1, %2
	move	rd7, %n2	\;add	rd0, %0, %1, rd7")

(define_insn "muldi3"
  [(set      (match_operand:DI 0 "rd_class_operand" "= Rd")
    (mult:DI (match_operand:DI 1 "rd_class_operand" "% Rd")
             (match_operand:DI 2 "rd_class_operand" "  Rd")))
   (set      (match_scratch:DI 3                    "= Rd")
             (us_mult:DI (match_dup 1) (match_dup 2)))]
	""
	"mulu	%3, %0, %1, %2")

(define_insn "divmoddi4"
  [(set     (match_operand:DI 0 "rd_class_operand" "= Rd")
    (div:DI (match_operand:DI 1 "rd_class_operand" "  Rd")
            (match_operand:DI 2 "rd_class_operand" "  Rd")))
   (set     (match_operand:DI 3 "rd_class_operand" "= Rd")
            (mod:DI (match_dup 1) (match_dup 2)))]
	""
	"divs	%3, %0, %1, %2")

;; FIXME
;; %1 should be as large as (8 * 2) bytes
(define_insn "udivmoddi4"
  [(set      (match_operand:DI 0 "rd_class_operand" "= Rd")
    (udiv:DI (match_operand:DI 1 "rd_class_operand" "  Rd")
             (match_operand:DI 2 "rd_class_operand" "  Rd")))
   (set      (match_operand:DI 3 "rd_class_operand" "= Rd")
             (umod:DI (match_dup 1) (match_dup 2)))]
	""
	"divu	%3, %0, %1, %2")

(define_insn "<rd_shift_name>di3"
  [(set          (match_operand:DI 0 "rd_class_operand" "=   Rd")
    (RD_SHIFT:DI (match_operand:DI 1 "rd_class_operand" "    Rd")
                 (match_operand:DI 2 "dd_rd_u6_operand" "  JsRd")))]
	""
	"<rd_shift_insn>	%0, %1, %2")

(define_insn "negdi2"
  [(set     (match_operand:DI 0 "rd_class_operand" "= Rd")
    (neg:DI (match_operand:DI 1 "rd_class_operand" "  Rd")))]
	""
	"sub	rd0, %0, rd0, %1")

(define_insn "one_cmpldi2"
  [(set     (match_operand:DI 0 "rd_class_operand" "= Rd")
    (not:DI (match_operand:DI 1 "rd_class_operand" "  Rd")))]
	""
	"not	%0, %1, rd0")

(define_insn "*nor"
  [(set             (match_operand:DI 0 "rd_class_operand" "=   Rd")
    (and:DI (not:DI (match_operand:DI 1 "rd_class_operand" "%   Rd"))
            (not:DI (match_operand:DI 2 "rd_class_operand" "    Rd"))))]
	""
	"orr	%0, %1, %2	\;	not	%0, %0, rd0")

