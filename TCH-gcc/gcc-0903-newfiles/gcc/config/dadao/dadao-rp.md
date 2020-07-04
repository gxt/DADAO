;; vim: filetype=lisp

;; GCC machine description for DADAO rp class
;; Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
;; Contributed by Guan Xuetao <gxt@pku.edu.cn>

(define_insn "*addrp"
  [(set      (match_operand:DI 0 "rp_class_operand"  "= Rp, Rp")
    (plus:DI (match_operand:DI 1 "rp_class_operand"  "% Rp, Rp")
             (match_operand:DI 2 "dd_rg_s12_operand" "  Id, Rg")))]
	""
	"@
	rp_add	%0, %1, %2
	rp_add	%0, %1, %2")

(define_insn "dd_ld_rp"
  [(set (match_operand:DI 0 "rp_class_operand" "= Rp")
        (match_operand:DI 1 "memory_operand"   "   m"))]
	""
	"rp_ldo	%0, %1")

(define_insn "dd_st_rp"
  [(set (match_operand:DI 0 "memory_operand"   "=  m")
        (match_operand:DI 1 "rp_class_operand" "  Rp"))]
	""
	"rp_sto	%1, %0")

(define_insn "dd_get_offset"
  [(set (match_operand:DI 0 "rg_class_operand"  "= Rg, Rg")
        (match_operand:DI 1 "immediate_operand" "   s, Ai"))]
	""
	"@
	geta	%0, %1
	geta	%0, %1")

(define_insn "dd_get_addr"
  [(set (match_operand:DI 0 "rp_class_operand"  "= Rp, Rp")
        (match_operand:DI 1 "immediate_operand" "   s, Ai"))]
	""
	"@
	geta	datao1, %1	\;	rp_add	%0, rp0, datao1
	geta	datao1, %1	\;	rp_add	%0, rp0, datao1")

;; TODO: SHOULD removed lator
(define_insn "*addrp2rg"
  [(set      (match_operand:DI 0 "rg_class_operand"  "= Rg, Rg")
    (plus:DI (match_operand:DI 1 "rp_class_operand"  "% Rp, Rp")
             (match_operand:DI 2 "dd_rg_s12_operand" "  Id, Rg")))]
	""
	"@
	rp_p2g	datao1, %1	\;	add	%0, datao1, %2
	rp_p2g	datao1, %1	\;	add	%0, datao1, %2")

;; TODO: SHOULD removed lator, handling condition: imm beyond s12
(define_insn "*addrp2rg_2"
  [(set      (match_operand:DI 0 "rg_class_operand"  "= Rg")
    (plus:DI (match_operand:DI 1 "rp_class_operand"  "% Rp")
             (match_operand:DI 2 "immediate_operand" "   i")))]
	""
	"rp_p2g	datao1, %1	\;	seto	%0, %2	\;	add	%0, datao1, %0")

;; TODO: SHOULD removed lator
(define_insn "*addrp2rp"
  [(set      (match_operand:DI 0 "rp_class_operand" "= Rp")
    (plus:DI (match_operand:DI 1 "rp_class_operand" "%  0")
             (match_operand:DI 2 "rp_class_operand" "  Rp")))]
	""
	"rp_p2g	datao1, %1	\;	rp_add	%0, %0, datao1")

;; TODO: SHOULD removed lator, handling condition: imm beyond s12
(define_insn "*addrp2rp_2"
  [(set      (match_operand:DI 0 "rp_class_operand"  "= Rp")
    (plus:DI (match_operand:DI 1 "rp_class_operand"  "% Rp")
             (match_operand:DI 2 "immediate_operand" "   i")))]
	""
	"seto	datao1, %2	\;	rp_add	%0, %1, datao1")
