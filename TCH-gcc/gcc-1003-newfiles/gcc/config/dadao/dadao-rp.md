;; vim: filetype=lisp

;; GCC machine description for DADAO rp class
;; Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
;; Contributed by Guan Xuetao <gxt@pku.edu.cn>

(define_insn "mov_rp2rp"
  [(set (match_operand:DI 0 "rp_class_operand" "= Rp")
        (match_operand:DI 1 "rp_class_operand" "  Rp"))]
	""
	"rp2rp	%0, %1, 0")

(define_insn "mov_rg2rp"
  [(set (match_operand:DI 0 "rp_class_operand" "= Rp")
        (match_operand:DI 1 "rg_class_operand" "  Rg"))]
	""
	"rg2rp	%0, %1, 0")

(define_insn "mov_rp2rg"
  [(set (match_operand:DI 0 "rg_class_operand" "= Rg")
        (match_operand:DI 1 "rp_class_operand" "  Rp"))]
	""
	"rp2rg	%0, %1, 0")

(define_insn "addrp"
  [(set      (match_operand:DI 0 "rp_class_operand" "=Rp")
    (plus:DI (match_operand:DI 1 "rp_class_operand" "%Rp")
             (match_operand:DI 2 "dd_rg_s18_operand")))]
	""
	{
	  if (!satisfies_constraint_It(operands[2]))
	    return "addrp	%0, %1, %2";
	  else
	    if (REGNO(operands[0]) == REGNO(operands[1]))
		 return "addrp	%1, %2";
	    else return "addrp	%0, %2	\;addrp	%0, %1, rg0	\;";
	})

(define_insn "addrp_large_scale"
  [(set      (match_operand:DI 0 "rp_class_operand"  "= Rp")
    (plus:DI (match_operand:DI 1 "rp_class_operand"  "% Rp")
             (match_operand:DI 2 "immediate_operand" "  i")))]
	""
	"setrp	%0, %2	\;addrp	%0, %1, rg0	\;")

(define_insn "addrp_ctry"
  [(set      (match_operand:DI 0 "rp_class_operand"  "=Rp")
    (plus:DI (match_operand:DI 1 "rg_class_operand"  "%Rg")
             (match_operand:DI 2 "rp_class_operand"  " Rp")))]
	""
	"addrp	%0, %2, %1")

(define_insn "dd_ld_rp"
  [(set (match_operand:DI 0 "rp_class_operand")
        (match_operand:DI 1 "memory_operand" "Wi,Wz,Wg"))]
	""
	"@
	ldrp	%0, %1
	ldrp	%0, %1
	ldmrp	%0, %1, 0")

(define_insn "dd_st_rp"
  [(set (match_operand:DI 0 "memory_operand" "=Wi,Wz,Wg")
        (match_operand:DI 1 "rp_class_operand"))]
	""
	"@
	strp	%1, %0
	strp	%1, %0
	stmrp	%1, %0, 0")

(define_expand "store_address"
  [(set      (match_operand:DI 0 "memory_operand"    "=m")
    (plus:DI (match_operand:DI 1 "rp_class_operand"  "Rp")
             (match_operand:DI 2 "dd_rg_s12_operand" "Id")))]
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
    (plus:DI (match_operand:DI 1 "rp_class_operand"  "Rp")
	     (match_operand:DI 2 "dd_rg_s12_operand" "Id")))
	(clobber (reg:DI 71))]
	""
	"addrp	rp7, %1, rg0	\;addrp	rp7, %2	\;strp	rp7, %0	\;")

(define_insn "dd_get_offset"
  [(set (match_operand:DI 0 "rg_class_operand")
        (match_operand:DI 1 "immediate_operand"))]
	""
	"setrg	%0, %1")

(define_insn "dd_get_addr"
  [(set (match_operand:DI 0 "rp_class_operand")
        (match_operand:DI 1 "immediate_operand"))]
	""
	"setrp	%0, %1")

(define_expand "dd_plus_rp"
  [(set      (match_operand:DI 0 "rg_class_operand")
    (plus:DI (match_operand:DI 1 "rp_class_operand")
             (match_operand:DI 2 "nonmemory_operand")))]
	""
	"{
	  if (satisfies_constraint_It(operands[2]) ||
	     (REG_P (operands[2]) &&
	      REGNO_REG_CLASS(REGNO(operands[2])) == GENERAL_REGS))
	    emit_insn (gen_addrp2rg (operands[0], operands[1], operands[2]));
	  if (GET_CODE (operands[2]) == CONST_INT &&
	     !satisfies_constraint_It (operands[2]))
	   emit_insn (gen_addrp2rg_large_scale (operands[0], operands[1], operands[2]));
	}")

(define_insn "addrp2rg"
  [(set      (match_operand:DI 0 "rg_class_operand"  "= Rg, Rg")
    (plus:DI (match_operand:DI 1 "rp_class_operand"  "% Rp, Rp")
	     (match_operand:DI 2 "dd_rg_s18_operand" "  It, Rg")))
	(clobber (reg:DI 71))]
	""
	"@
	setrp	rp7, %2	\;addrp	rp7, %1, rg0	\;rp2rg	%0, rp7, 0	\;
	rp2rg	%0, %1, 0	\;add	rg0, %0, %2, %0	\;")

(define_insn "addrp2rg_large_scale"
  [(set      (match_operand:DI 0 "rg_class_operand")
    (plus:DI (match_operand:DI 1 "rp_class_operand")
             (match_operand:DI 2 "immediate_operand")))
	(clobber (reg:DI 7))]
	""
	"rp2rg	%0, %1, 0	\;setrg	rg7, %2	\;add	rg0, %0, rg7, %0	\;")
