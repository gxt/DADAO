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
  [(set      (match_operand:DI 0 "rb_class_operand" "=Rb")
    (plus:DI (match_operand:DI 1 "rb_class_operand" "%Rb")
             (match_operand:DI 2 "rd_class_operand" "Rd")))]
	""
	"addrb	%0, %1, %2")


(define_insn "addrb_imm"
  [(parallel[
        (set       (match_operand:DI 0 "rb_class_operand"  "= Rb")
          (plus:DI (match_operand:DI 1 "rb_class_operand"  "% Rb")
                   (match_operand:DI 2 "immediate_operand" "  i")))
        (clobber (reg:DI 7))])]
        ""
        {
          if (!satisfies_constraint_It(operands[2]))
                return  "setrd  rd7, %2 \;addrb %0, %1, rd7";
          if (operands[1] == operands[0])
                return  "addrb  %0, %2";
          else
                return  "rb2rb  %0, %1, 0       \;addrb %0, %2";
        })

(define_insn "addrb_imm_prereload"
  [(parallel[
	(set	   (match_operand:DI 0 "rb_class_operand"  "= Rb")
    	  (plus:DI (match_operand:DI 1 "rb_class_operand"  "% Rb")
		   (match_operand:DI 2 "immediate_operand" "  i")))
	(clobber (match_scratch:DI 3 "=&r"))])]
	"!reload_completed"
	{
	  if (!satisfies_constraint_It(operands[2]))
		return	"setrd	%3, %2	\;addrb	%0, %1, %3";
	  if (operands[1] == operands[0])
		return	"addrb	%0, %2";
	  else
		return	"rb2rb	%0, %1, 0	\;addrb	%0, %2";
	})

(define_insn "addrb_ctry"
  [(set      (match_operand:DI 0 "rb_class_operand"  "=Rb")
    (plus:DI (match_operand:DI 1 "rd_class_operand"  "%Rd")
             (match_operand:DI 2 "rb_class_operand"  " Rb")))]
	""
	"addrb	%0, %2, %1")

(define_insn "dd_ld_rb"
  [(set (match_operand:DI 0 "rb_class_operand")
        (match_operand:DI 1 "memory_operand" "Wi,Wz,Wg"))]
	""
	"@
	ldrb	%0, %1
	ldrb	%0, %1
	ldmrb	%0, %1, 0")

(define_insn "dd_st_rb"
  [(set (match_operand:DI 0 "memory_operand" "=Wi,Wz,Wg")
        (match_operand:DI 1 "rb_class_operand"))]
	""
	"@
	strb	%1, %0
	strb	%1, %0
	stmrb	%1, %0, 0")

(define_expand "store_address"
  [(set      (match_operand:DI 0 "memory_operand"    "=m")
    (plus:DI (match_operand:DI 1 "rb_class_operand"  "Rb")
             (match_operand:DI 2 "dd_rd_s12_operand" "Id")))]
	""
	"{
	  if (satisfies_constraint_It(operands[2])) {
	     emit_insn (gen_dd_store_address (operands[0], operands[1], operands[2]));
	     DONE;
	  }
	  FAIL;
	}")

; Only used in optimization
(define_insn "dd_store_address"
  [(set      (match_operand:DI 0 "memory_operand"    "=m")
    (plus:DI (match_operand:DI 1 "rb_class_operand"  "Rb")
	     (match_operand:DI 2 "dd_rd_s12_operand" "Id")))
	(clobber (reg:DI 71))]
	""
	"addrb	rb7, %1, rd0	\;addrb	rb7, %2	\;strb	rb7, %0	\;")

(define_insn "dd_get_offset"
  [(set (match_operand:DI 0 "rd_class_operand")
        (match_operand:DI 1 "immediate_operand"))]
	""
	"setrd	%0, %1")

(define_insn "dd_get_addr"
  [(set (match_operand:DI 0 "rb_class_operand")
        (match_operand:DI 1 "immediate_operand"))]
	""
	"setrb	%0, %1")

(define_expand "dd_plus_rb"
  [(set      (match_operand:DI 0 "rd_class_operand")
    (plus:DI (match_operand:DI 1 "rb_class_operand")
             (match_operand:DI 2 "nonmemory_operand")))]
	""
	"{
	  if (satisfies_constraint_It(operands[2]) ||
	     (REG_P (operands[2]) &&
	      REGNO_REG_CLASS(REGNO(operands[2])) == GENERAL_REGS))
	    emit_insn (gen_addrb2rd (operands[0], operands[1], operands[2]));
	  if (GET_CODE (operands[2]) == CONST_INT &&
	     !satisfies_constraint_It (operands[2]))
	   emit_insn (gen_addrb2rd_larde_scale (operands[0], operands[1], operands[2]));
	}")

(define_insn "addrb2rd"
  [(set      (match_operand:DI 0 "rd_class_operand"  "= Rd, Rd")
    (plus:DI (match_operand:DI 1 "rb_class_operand"  "% Rb, Rb")
	     (match_operand:DI 2 "dd_rd_s18_operand" "  It, Rd")))
	(clobber (reg:DI 71))]
	""
	"@
	setrb	rb7, %2	\;addrb	rb7, %1, rd0	\;rb2rd	%0, rb7, 0	\;
	rb2rd	%0, %1, 0	\;add	rd0, %0, %2, %0	\;")

(define_insn "addrb2rd_larde_scale"
  [(set      (match_operand:DI 0 "rd_class_operand")
    (plus:DI (match_operand:DI 1 "rb_class_operand")
             (match_operand:DI 2 "immediate_operand")))
	(clobber (reg:DI 7))]
	""
	"rb2rd	%0, %1, 0	\;setrd	rd7, %2	\;add	rd0, %0, rd7, %0	\;")
