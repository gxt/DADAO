;; vim: filetype=lisp

;; GCC machine description for DADAO rb class
;; Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
;; Contributed by Guan Xuetao <gxt@pku.edu.cn>

(define_insn "mov_rb2rb"
  [(set (match_operand:DI 0 "rb_class_operand" "= Rb")
        (match_operand:DI 1 "rb_class_operand" "  Rb"))]
	""
	"rb2rb	%0, %1, 1")

(define_insn "mov_rd2rb"
  [(set (match_operand:DI 0 "rb_class_operand" "= Rb")
        (match_operand:DI 1 "rd_class_operand" "  Rd"))]
	""
	"rd2rb	%0, %1, 1")

(define_insn "mov_rb2rd"
  [(set (match_operand:DI 0 "rd_class_operand" "= Rd")
        (match_operand:DI 1 "rb_class_operand" "  Rb"))]
	""
	"rb2rd	%0, %1, 1")

(define_insn "addrb_rd"
  [(set      (match_operand:DI 0 "rb_class_operand" "=Rb,Rb")
    (plus:DI (match_operand:DI 1 "rb_class_operand" "%Rb,Rb")
             (match_operand:DI 2 "dd_rd_ls_operand" " Rd, i")))]
	""
	"@
	add	%0, %1, %2
	setrd	rd7, %2	\;add	%0, %1, rd7")

(define_insn "subrb_rd"
  [(set      (match_operand:DI 0 "rb_class_operand" "=Rb")
   (minus:DI (match_operand:DI 1 "rb_class_operand" "%Rb")
             (match_operand:DI 2 "rd_class_operand" "Rd")))]
	""
	"sub	%0, %1, %2")

(define_insn "addrb_imm"
  [(set       (match_operand:DI 0 "rb_class_operand"  "= Rb")
     (plus:DI (match_operand:DI 1 "rb_class_operand"  "% Rb")
              (match_operand:DI 2 "dd_sign_18_operand"   "i")))]
  	""
	{
        if(INTVAL(operands[2]) > 0x7ff || INTVAL(operands[2]) < -0x800)
                return "setrd\trd7, %2 \;add\t %0, %1, rd7";
        else
                return "addi\t%0, %1, %2";
	})

(define_insn "subrb_imm"
  [(set       (match_operand:DI 0 "rb_class_operand"  "= Rb")
    (minus:DI (match_operand:DI 1 "rb_class_operand"  "% Rb")
              (match_operand:DI 2 "dd_sign_18_operand"   "i")))]
  	""
	{
        if(INTVAL(operands[2]) > 0x7ff || INTVAL(operands[2]) < -0x800)
                return "setrd\trd7, %n2 \;add\t %0, %1, rd7";
        else
                return "addi\t%0, %1, %n2";
        })


(define_insn "addrb_ctry"
  [(set      (match_operand:DI 0 "rb_class_operand"  "=Rb")
    (plus:DI (match_operand:DI 1 "rd_class_operand"  "%Rd")
             (match_operand:DI 2 "rb_class_operand"  " Rb")))]
	""
	"add	%0, %2, %1")

; XXX
(define_insn "subrb_ctry"
  [(set      (match_operand:DI 0 "rb_class_operand"  "=Rb")
   (minus:DI (match_operand:DI 1 "rd_class_operand"  "%Rd")
             (match_operand:DI 2 "rb_class_operand"  " Rb")))]
	""
	"sub	%0, %2, %1")

(define_insn "dd_ld_rb"
  [(set (match_operand:DI 0 "rb_class_operand" "=Rb,Rb")
        (match_operand:DI 1 "memory_operand"    "Wg, m"))]
	""
	"@
	ldmo	%0, %1, 1
	ldo	%0, %1")

(define_insn "dd_st_rb"
  [(set (match_operand:DI 0 "memory_operand"  "=Wg, m")
        (match_operand:DI 1 "rb_class_operand" "Rb,Rb"))]
	""
	"@
	stmo	%1, %0, 1
	sto	%1, %0")

(define_insn "dd_ldrbimm"
  [(set (match_operand:DI 0 "rb_class_operand" "=Rb")
        (match_operand:DI 1 "immediate_operand" ""))]
	""
	"setrb	%0, %1");

(define_insn "addrb2rd"
  [(set      (match_operand:DI 0 "rd_class_operand"  "= Rd, Rd")
    (plus:DI (match_operand:DI 1 "rb_class_operand"  "% Rb, Rb")
	     (match_operand:DI 2 "dd_rd_s18_operand" "  It, Rd")))]
	""
        {
          if(GET_CODE(operands[2]) == CONST_INT)
          {
                if(INTVAL(operands[2])> 0x7ff || INTVAL(operands[2])< -0x800)
                return "setrd\trd7, %2 \; rb2rd\t%0, %1, 1\;add\trd0, %0, %0, rd7";
                else
                return "rb2rd\t%0, %1, 1\;addi\t%0, %0, %2";
          }
          else
          {
                return "rb2rd\t%0, %1, 1\;add\trd0,%0, %0, %2";
          }
        })



(define_insn "subrb2rd"
  [(set      (match_operand:DI 0 "rd_class_operand"  "= Rd, Rd")
   (minus:DI (match_operand:DI 1 "rb_class_operand"  "% Rb, Rb")
	     (match_operand:DI 2 "dd_rd_s18_operand" "  It, Rd")))]
	""
        {
          if(GET_CODE(operands[2]) == CONST_INT)
          {
                if(INTVAL(operands[2]) > 0x7ff || INTVAL(operands[2]) < -0x800)
                return "setrd\trd7,%2\;rb2rd\t%0, %1, 1\;sub\trd0, %0, %0, %2";
                else
                return "rb2rd\t%0, %1, 1\;addi\t%0, %0, %n2";
          }
          else
          {
                return "rb2rd\t%0, %1, 1\t\;sub\trd0, %0, %0, %2";
          }
        })



;; Shall it stays ?
(define_insn "addrb2rd_larde_scale"
  [(set      (match_operand:DI 0 "rd_class_operand" "=Rd")
    (plus:DI (match_operand:DI 1 "rb_class_operand" "=Rb")
             (match_operand:DI 2 "const_int_operand"  "i")))]
	""
	"rb2rd\t%0, %1, 1\t\;setrd\trd7, %2\;add\trd0, %0, rd7, %0")
