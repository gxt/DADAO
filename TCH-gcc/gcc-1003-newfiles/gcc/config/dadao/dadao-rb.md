;; vim: filetype=lisp

;; GCC machine description for DADAO rb class
;; Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
;; Contributed by Guan Xuetao <gxt@pku.edu.cn>

(define_insn "mov_rb2rb"
  [(set (match_operand:DI 0 "rb_class_operand" "= Rb")
        (match_operand:DI 1 "rb_class_operand" "  Rb"))]
	""
	"rb2rb	%0, %1, 0")

(define_insn "mov_rd2rb"
  [(set (match_operand:DI 0 "rb_class_operand" "= Rb")
        (match_operand:DI 1 "rd_class_operand" "  Rd"))]
	""
	"rd2rb	%0, %1, 0")

(define_insn "mov_rb2rd"
  [(set (match_operand:DI 0 "rd_class_operand" "= Rd")
        (match_operand:DI 1 "rb_class_operand" "  Rb"))]
	""
	"rb2rd	%0, %1, 0")

(define_insn "addrb_rd"
  [(set      (match_operand:DI 0 "rb_class_operand" "=Rb,Rb")
    (plus:DI (match_operand:DI 1 "rb_class_operand" "%Rb,Rb")
             (match_operand:DI 2 "dd_rd_ls_operand" " Rd, i")))]
	""
	"@
	addrb	%0, %1, %2
	move	rd7, %2	\;addrb	%0, %1, rd7")

(define_insn "subrb_rd"
  [(set      (match_operand:DI 0 "rb_class_operand" "=Rb")
   (minus:DI (match_operand:DI 1 "rb_class_operand" "%Rb")
             (match_operand:DI 2 "rd_class_operand" "Rd")))]
	""
	"subrb	%0, %1, %2")

(define_insn "addrb_imm"
  [(set       (match_operand:DI 0 "rb_class_operand"  "= Rb")
     (plus:DI (match_operand:DI 1 "rb_class_operand"  "% Rb")
              (match_operand:DI 2 "dd_sign_18_operand"   "i")))]
  ""
  { if (REGNO(operands[1])
     == REGNO(operands[0])) return  "addi\t%0, %2";
    else return "rb2rb\t%0, %1, 0\t\;addi\t%0, %2";
  })

(define_insn "subrb_imm"
  [(set       (match_operand:DI 0 "rb_class_operand"  "= Rb")
    (minus:DI (match_operand:DI 1 "rb_class_operand"  "% Rb")
              (match_operand:DI 2 "dd_sign_18_operand"   "i")))]
  ""
  { if (REGNO(operands[1])
     == REGNO(operands[0])) return  "addi\t%0, %n2";
    else return "rb2rb\t%0, %1, 0\t\;addi\t%0, %n2";
  })

(define_insn "addrb_ctry"
  [(set      (match_operand:DI 0 "rb_class_operand"  "=Rb")
    (plus:DI (match_operand:DI 1 "rd_class_operand"  "%Rd")
             (match_operand:DI 2 "rb_class_operand"  " Rb")))]
	""
	"addrb	%0, %2, %1")

; XXX
(define_insn "subrb_ctry"
  [(set      (match_operand:DI 0 "rb_class_operand"  "=Rb")
   (minus:DI (match_operand:DI 1 "rd_class_operand"  "%Rd")
             (match_operand:DI 2 "rb_class_operand"  " Rb")))]
	""
	"subrb	%0, %2, %1")

(define_insn "dd_ld_rb"
  [(set (match_operand:DI 0 "rb_class_operand" "=Rb,Rb")
        (match_operand:DI 1 "memory_operand"    "Wg, m"))]
	""
	"@
	ldmrb	%0, %1, 0
	ldrb	%0, %1")

(define_insn "dd_st_rb"
  [(set (match_operand:DI 0 "memory_operand"  "=Wg, m")
        (match_operand:DI 1 "rb_class_operand" "Rb,Rb"))]
	""
	"@
	stmrb	%1, %0, 0
	strb	%1, %0")

(define_insn "dd_ldrbimm"
  [(set (match_operand:DI 0 "rb_class_operand" "=Rb")
        (match_operand:DI 1 "immediate_operand" ""))]
	""
	"move	%0, %1");

(define_insn "addrb2rd"
  [(set      (match_operand:DI 0 "rd_class_operand"  "= Rd, Rd")
    (plus:DI (match_operand:DI 1 "rb_class_operand"  "% Rb, Rb")
	     (match_operand:DI 2 "dd_rd_s18_operand" "  It, Rd")))]
	""
	"@
	rb2rd	%0, %1, 0	\;addi	%0, %2
	rb2rd	%0, %1, 0	\;add	rd0, %0, %2, %0")

(define_insn "subrb2rd"
  [(set      (match_operand:DI 0 "rd_class_operand"  "= Rd, Rd")
   (minus:DI (match_operand:DI 1 "rb_class_operand"  "% Rb, Rb")
	     (match_operand:DI 2 "dd_rd_s18_operand" "  It, Rd")))]
	""
	"@
	rb2rd	%0, %1, 0	\;addi	%0, %n2
	rb2rd	%0, %1, 0	\;sub	rd0, %0, %0, %2")

;; Shall it stays ?
(define_insn "addrb2rd_larde_scale"
  [(set      (match_operand:DI 0 "rd_class_operand" "=Rd")
    (plus:DI (match_operand:DI 1 "rb_class_operand" "=Rb")
             (match_operand:DI 2 "const_int_operand"  "i")))]
	""
	"rb2rd\t%0, %1, 0\t\;move\trd7, %2\t\;add\trd0, %0, rd7, %0")
