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

;; TODO
;; temporary register usage has to be settled
;; rg6 is one temporary reg replacement

(define_insn "addrp_lscal"
  [(set      (match_operand:DI 0 "rp_class_operand")
    (plus:DI (match_operand:DI 1 "rp_class_operand")
             (match_operand:DI 2 "immediate_operand")))]
	"!satisfies_constraint_It(operands[2])"
	{
	  return "setrg	rg7, %2	\;addrp	%0, %1, rg7	\;";
	})

(define_insn "addrp_ctry"
  [(set      (match_operand:DI 0 "rp_class_operand"  "=Rp")
    (plus:DI (match_operand:DI 1 "rg_class_operand"  "%Rg")
             (match_operand:DI 2 "rp_class_operand"  " Rp")))]
	""
	"addrp	%0, %2, %1")

(define_insn "dd_ld_rp"
  [(set (match_operand:DI 0 "rp_class_operand" "=Rp")
        (match_operand:DI 1 "memory_operand"   "  m"))]
	""
	{
	  if (satisfies_constraint_Wg(operands[1]))
	  	return	"ldmrp	%0, %1, 0";
	  return	"ldrp	%0, %1";
	})

(define_insn "dd_st_rp"
  [(set (match_operand:DI 0 "memory_operand"   "=m")
        (match_operand:DI 1 "rp_class_operand" "Rp"))]
	""
	{
	  if (satisfies_constraint_Wg(operands[0]))
		return	"stmrp	%1, %0, 0";
	  return	"strp	%1, %0";
	})

(define_insn "dd_st_addr"
  [(set (match_operand:DI 0 "memory_operand")
        (plus:DI (match_operand:DI 1 "rp_class_operand"	 "Rp")
		 (match_operand:DI 2 "dd_rg_s12_operand" "Id")))]
	""
	"")

(define_insn "dd_get_offset"
  [(set (match_operand:DI 0 "rg_class_operand"  "= Rg, Rg")
        (match_operand:DI 1 "immediate_operand" "   s, Ai"))]
	"" "")
;	"@
;	geta	%0, %1
;	geta	%0, %1")

;; FIXME
;; Adding Rp-i for reload-pass
(define_insn "dd_get_addr"
  [(set (match_operand:DI 0 "rp_class_operand"  "= Rp, Rp, Rp")
        (match_operand:DI 1 "immediate_operand" "   s, Ai,  i"))]
	""
	"")
