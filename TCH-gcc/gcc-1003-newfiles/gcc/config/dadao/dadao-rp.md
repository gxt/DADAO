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

(define_insn "*addrp"
  [(set      (match_operand:DI 0 "rp_class_operand"  "= Rp, Rp")
    (plus:DI (match_operand:DI 1 "rp_class_operand"  "% Rp, Rp")
             (match_operand:DI 2 "dd_rg_s12_operand" "  Id, Rg")))]
	"" "")
;	"@
;	rp_add	%0, %1, %2
;	rp_add	%0, %1, %2")

(define_insn "dd_ld_rp"
  [(set (match_operand:DI 0 "rp_class_operand" "= Rp,Rp,Rp")
        (match_operand:DI 1 "memory_operand"   "  Wi,Wz,Wg"))]
	""
	"@
	ldrp	%0, %1
	ldrp	%0, %1
	ldmrp	%0, %1, 0")

(define_insn "dd_st_rp"
  [(set (match_operand:DI 0 "memory_operand"   "=  m")
        (plus:DI (match_operand:DI 1 "rp_class_operand"	 "Rp")
		 (match_operand:DI 2 "dd_rg_s12_operand" "Id")))]
	""
	"strp	%0, %1, %2")

(define_insn "dd_get_offset"
  [(set (match_operand:DI 0 "rg_class_operand"  "= Rg, Rg")
        (match_operand:DI 1 "immediate_operand" "   s, Ai"))]
	"" "")
;	"@
;	geta	%0, %1
;	geta	%0, %1")

(define_insn "dd_get_addr"
  [(set (match_operand:DI 0 "rp_class_operand"  "= Rp, Rp")
        (match_operand:DI 1 "immediate_operand" "   s, Ai"))]
	"" "")
;	"@
;	geta	rg1, %1	\;	rp_add	%0, rp0, rg1
;	geta	rg1, %1	\;	rp_add	%0, rp0, rg1")

;; TODO: SHOULD removed lator
(define_insn "*addrp2rg"
  [(set      (match_operand:DI 0 "rg_class_operand"  "= Rg, Rg")
    (plus:DI (match_operand:DI 1 "rp_class_operand"  "% Rp, Rp")
             (match_operand:DI 2 "dd_rg_s12_operand" "  Id, Rg")))]
	""
	"@
	rp2rg	rg1, %1, 0	\;	add	%0, %2
	rp2rg	rg1, %1, 0	\;	add	%0, %2")

;; TODO: SHOULD removed lator, handling condition: imm beyond s12
(define_insn "*addrp2rg_2"
  [(set      (match_operand:DI 0 "rg_class_operand"  "= Rg")
    (plus:DI (match_operand:DI 1 "rp_class_operand"  "% Rp")
             (match_operand:DI 2 "immediate_operand" "   i")))]
	""
	"rp2rg	rg1, %1, 0	\;	setrg	%0, %2	\;	add	rg0, %0, rg1, %0")

;; TODO: SHOULD removed lator
(define_insn "*addrp2rp"
  [(set      (match_operand:DI 0 "rp_class_operand" "= Rp")
    (plus:DI (match_operand:DI 1 "rp_class_operand" "%  0")
             (match_operand:DI 2 "rp_class_operand" "  Rp")))]
	"" "")
;	"rp2rg	rg1, %1, 0	\;	rp_add	%0, %0, rg1")

;; TODO: SHOULD removed lator, handling condition: imm beyond s12
(define_insn "*addrp2rp_2"
  [(set      (match_operand:DI 0 "rp_class_operand"  "= Rp")
    (plus:DI (match_operand:DI 1 "rp_class_operand"  "% Rp")
             (match_operand:DI 2 "immediate_operand" "   i")))]
	"" "")
;	"setrg	rg1, %2	\;	rp_add	%0, %1, rg1")
