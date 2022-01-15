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

(define_expand "dd_addrb_imm"
  [(set       (match_operand:DI 0 "rb_class_operand"  "= Rb")
     (plus:DI (match_operand:DI 1 "rb_class_operand"  "% Rb")
              (match_operand:DI 2 "const_int_operand" "  i")))]
        ""
	"{
		if (!satisfies_constraint_It (operands[2])) {
			if (can_create_pseudo_p ()) operands[2] = force_reg (DImode, operands[2]);
			else {
				rtx ip = gen_rtx_REG (DImode, 7);
				emit_insn (gen_rtx_SET (ip, operands[2]));
				operands[2] = ip;
			}
		}
	}")

(define_insn "addrb_imm"
  [(set       (match_operand:DI 0 "rb_class_operand"  "= Rb")
     (plus:DI (match_operand:DI 1 "rb_class_operand"  "% Rb")
              (match_operand:DI 2 "dd_sign_18_operand"   "i")))]
        ""
        {
          if (operands[1] == operands[0])
                return  "addrb	%0, %2";
          else
                return  "rb2rb  %0, %1, 0       \;addrb	%0, %2";
       })

(define_insn "addrb_ctry"
  [(set      (match_operand:DI 0 "rb_class_operand"  "=Rb")
    (plus:DI (match_operand:DI 1 "rd_class_operand"  "%Rd")
             (match_operand:DI 2 "rb_class_operand"  " Rb")))]
	""
	"addrb	%0, %2, %1")

(define_insn "dd_ld_rb"
  [(set (match_operand:DI 0 "rb_class_operand" "=Rb")
        (match_operand:DI 1 "memory_operand"     "m"))]
	""
	{
	  if (satisfies_constraint_Wg(operands[1])) {
                return "ldmrb   %0, %1, 0";
          }
          else
                return "ldrb    %0, %1";
	})

(define_insn "dd_st_rb"
  [(set (match_operand:DI 0 "memory_operand"   "=m")
        (match_operand:DI 1 "rb_class_operand" "Rb"))]
	""
	{
	  if (satisfies_constraint_Wg(operands[0])) {
		return "stmrb	%1, %0, 0";
	  }
	  else
		return "strb	%1, %0";
	})

(define_expand "store_address"
  [(set      (match_operand:DI 0 "memory_operand"    "=m")
    (plus:DI (match_operand:DI 1 "rb_class_operand"  "Rb")
             (match_operand:DI 2 "dd_rd_s12_operand" "")))]
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
	{
	  if (!satisfies_constraint_Wg(operands[0]))
	    {
		return "addrb	rb7, %1, rd0	\;addrb	rb7, %2	\;strb	rb7, %0	\;";
	    }
	  else
		return "addrb   rb7, %1, rd0    \;addrb rb7, %2 \;stmrb  rb7, %0, 0 \;";
	})

(define_insn "dd_rb_get_imm"
  [(set (match_operand:DI 0 "rb_class_operand" "=Rb")
        (match_operand:DI 1 "const_int_operand"  ""))]
	""
	"swym");

(define_insn "dd_rb_get_label"
  [(set (match_operand:DI 0 "rb_class_operand" "=Rb")
        (match_operand:DI 1 "dd_label_operand"  ""))]
	""
	"swym");

(define_insn "dd_rb_get_local_symbol"
  [(set (match_operand:DI 0 "rb_class_operand" "=Rb")
        (match_operand:DI 1 "local_symbolic_operand"  ""))]
	""
	"swym");

(define_insn "dd_rb_get_global_symbol"
  [(set (match_operand:DI 0 "rb_class_operand" "=Rb")
        (match_operand:DI 1 "global_symbolic_operand"  ""))]
	""
	"swym");

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
	swym
	swym")
;	setrb	rb7, %2	\;addrb	rb7, %1, rd0	\;rb2rd	%0, rb7, 0	\;
;	rb2rd	%0, %1, 0	\;add	rd0, %0, %2, %0	\;")

(define_insn "addrb2rd_larde_scale"
  [(set      (match_operand:DI 0 "rd_class_operand")
    (plus:DI (match_operand:DI 1 "rb_class_operand")
             (match_operand:DI 2 "immediate_operand")))
	(clobber (reg:DI 7))]
	""
	"rb2rd	%0, %1, 0	\;setrd	rd7, %2	\;add	rd0, %0, rd7, %0	\;")
