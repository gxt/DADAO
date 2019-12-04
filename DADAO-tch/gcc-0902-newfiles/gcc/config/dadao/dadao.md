;; Machine description for DADAO
;; Copyright (C) 2019-2033 by Guan Xuetao (AT) Peking Univ.
;;
;; Contributed by:
;;   2019:
;;	Shi Yining <1700012728@pku.edu.cn>
;;	Guan Xuetao <gxt@pku.edu.cn>

;; -------------------------------------------------------------------------
;; DADAO specific constraints, predicates and attributes
;; -------------------------------------------------------------------------

(include "constraints.md")
(include "predicates.md")

; Most instructions are two bytes long.
(define_attr "length" "" (const_int 4))


(define_mode_iterator GPR [QI HI SI DI])
(define_mode_attr d [(QI "1b") (HI "2b") (SI "4b") (DI "8b")])

;; -------------------------------------------------------------------------
;; no-op instruction
;; -------------------------------------------------------------------------

(define_insn "nop"
  [(const_int 0)]
  ""
  "nop")

;; -------------------------------------------------------------------------
;; Move instructions
;; -------------------------------------------------------------------------

(define_expand "mov<mode>"
   [(set (match_operand:GPR 0 "general_operand" "")
         (match_operand:GPR 1 "general_operand" ""))]
   ""
  "
{
  /* If this is a store, force the value into a register.  */
  if (! (reload_in_progress || reload_completed)) {
    if (MEM_P (operands[0])) {
      operands[1] = force_reg (<MODE>mode, operands[1]);
      if (MEM_P (XEXP (operands[0], 0)))
        operands[0] = gen_rtx_MEM (DImode, force_reg (DImode, XEXP (operands[0], 0)));
    } else {
      if (MEM_P (operands[1]) && MEM_P (XEXP (operands[1], 0)))
        operands[1] = gen_rtx_MEM (DImode, force_reg (DImode, XEXP (operands[1], 0)));
    }
  }
}")

(define_insn "*mov<mode>"
  [(set (match_operand:GPR 0 "nonimmediate_operand" "=D,D,B,B,r,m,D")
	(match_operand:GPR 1 "dadao_general_movsrc_operand" "D,B,D,B,m,r,i"))]
  "register_operand (operands[0], <MODE>mode) || register_operand (operands[1], <MODE>mode)"
  "@
  mov.dd \\t=%0, %1
  mov.dp \\t=%0, %1
  mov.pd \\t=%0, %1
  mov.pp \\t=%0, %1
  load.<d> \\t=%0, %1
  store.<d> \\t=%0, %1
  movi     \\t=%0, %1"
)

(define_insn "extend<mode>di2"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(sign_extend:DI (match_operand:GPR 1 "nonimmediate_operand" "r")))]
  ""
  "sex.<d>\\t=%0, %1"
)

(define_insn "zero_extend<mode>di2"
  [(set (match_operand:DI 0 "register_operand" "=D,D,B,B,D")
	(zero_extend:DI (match_operand:GPR 1 "nonimmediate_operand" "D,B,D,B,m")))]
  ""
  "@
  mov.dd \\t=%0, %1
  mov.dp \\t=%0, %1
  mov.pd \\t=%0, %1
  mov.pp \\t=%0, %1
  load.<d>\\t=%0, %1"
)

;; -------------------------------------------------------------------------
;; Arithmetic instructions
;; -------------------------------------------------------------------------

(define_insn "add<mode>3"
  [(set (match_operand:GPR 0 "register_operand" "=D,D,B")
     (plus:GPR (match_operand:GPR 1 "register_operand" "D,0,0")
               (match_operand:GPR 2 "dadao_arith_operand" "D,i,i")))]
  ""
  "@
  add\\t=%0, %1, %2
  add.rd\\t=%0, %2
  add.rp\\t=%0, %2"
)

(define_insn "sub<mode>3"
  [(set (match_operand:GPR 0 "register_operand" "=D,D,B")
     (minus:GPR (match_operand:GPR 1 "register_operand" "D,0,0")
                (match_operand:GPR 2 "dadao_arith_operand" "D,i,i")))]
  ""
  "@
  sub\\t=%0, %1, %2
  sub.rd\\t=%0, %2
  sub.rp\\t=%0, %2"
)

(define_insn "mul<mode>3"
  [(set (match_operand:GPR 0 "register_operand" "=D")
     (mult:GPR (match_operand:GPR 1 "register_operand" "D")
                  (match_operand:GPR 2 "register_operand" "D")))]
  ""
{
  return "mul\\t=%0, %1, %2";
})

(define_insn "div<mode>3"
  [(set (match_operand:GPR 0 "register_operand" "=D")
     (div:GPR (match_operand:GPR 1 "register_operand" "D")
              (match_operand:GPR 2 "register_operand" "D")))]
  ""
{
  return "div\\t=%0, %1, %2";
})


(define_insn "udiv<mode>3"
  [(set (match_operand:GPR 0 "register_operand" "=D")
     (udiv:GPR (match_operand:GPR 1 "register_operand" "D")
               (match_operand:GPR 2 "register_operand" "D")))]
  ""
{
  return "div\\t=%0, %1, %2";
})

(define_insn "mod<mode>3"
  [(set (match_operand:GPR 0 "register_operand" "=D")
     (mod:GPR (match_operand:GPR 1 "register_operand" "D")
              (match_operand:GPR 2 "register_operand" "D")))]
  ""
{
  return "mod\\t=%0, %1, %2";
})

(define_insn "umod<mode>3"
  [(set (match_operand:GPR 0 "register_operand" "=D")
     (umod:GPR (match_operand:GPR 1 "register_operand" "D")
               (match_operand:GPR 2 "register_operand" "D")))]
  ""
{
  return "umod\\t=%0, %1, %2";
})


;; -------------------------------------------------------------------------
;; Logical operators
;; -------------------------------------------------------------------------

(define_insn "and<mode>3"
  [(set (match_operand:GPR 0 "register_operand" "=D,D,B")
     (and:GPR (match_operand:GPR 1 "register_operand" "D,0,0")
              (match_operand:GPR 2 "dadao_arith_operand" "D,i,i")))]
  ""
  "@
  and\\t=%0, %1, %2
  and.rd\\t=%0, %2
  and.rp\\t=%0, %2"
)

(define_insn "xor<mode>3"
  [(set (match_operand:GPR 0 "register_operand" "=D")
     (xor:GPR (match_operand:GPR 1 "register_operand" "D")
              (match_operand:GPR 2 "register_operand" "D")))]
  ""
{
  return "xor\\t=%0, %1, %2";
})

(define_insn "ior<mode>3"
  [(set (match_operand:GPR 0 "register_operand" "=D,D,B")
     (ior:GPR (match_operand:GPR 1 "register_operand" "D,0,0")
              (match_operand:GPR 2 "dadao_arith_operand" "D,i,i")))]
  ""
  "@
  ior\\t=%0, %1, %2
  ior.rd\\t=%0, %2
  ior.rp\\t=%0, %2"
)

(define_insn "one_cmpldi2"
  [(set (match_operand:DI 0 "register_operand" "=D")
    (not:DI (match_operand:DI 1 "register_operand" "D")))]
  ""
  "not\\t=%0, %1"
)

;; -------------------------------------------------------------------------
;; Shifters
;; -------------------------------------------------------------------------

(define_insn "ashldi3"
  [(set (match_operand:DI 0 "register_operand" "=D,D")
     (ashift:DI (match_operand:DI 1 "register_operand" "D,D")
                (match_operand:DI 2 "dadao_shift_operand" "D,i")))]
  ""
  "@
  sh.ll\\t=%0, %1, %2
  sh.ll\\t=%0, %1, %2"
)

(define_insn "ashrdi3"
  [(set (match_operand:DI 0 "register_operand" "=D,D")
     (ashiftrt:DI (match_operand:DI 1 "register_operand" "D,D")
                  (match_operand:DI 2 "dadao_shift_operand" "D,i")))]
  ""
  "@
  sh.ra\\t=%0, %1, %2
  sh.ra\\t=%0, %1, %2"
)

(define_insn "lshrdi3"
  [(set (match_operand:DI 0 "register_operand" "=D,D")
     (lshiftrt:DI (match_operand:DI 1 "register_operand" "D,D")
                  (match_operand:DI 2 "dadao_shift_operand" "D,i")))]
  ""
  "@
  sh.rl\\t=%0, %1, %2
  sh.rl\\t=%0, %1, %2"
)

(define_insn "rotrdi3"
  [(set (match_operand:DI 0 "register_operand" "=D,D")
     (rotatert:DI (match_operand:DI 1 "register_operand" "D,D")
                  (match_operand:DI 2 "dadao_shift_operand" "D,i")))]
  ""
  "@
  sh.rr\\t=%0, %1, %2
  sh.rr\\t=%0, %1, %2"
)

;; -------------------------------------------------------------------------
;; Compare instructions
;; -------------------------------------------------------------------------

(define_expand "cbranchdi4"
  [(set (pc)
    (if_then_else (match_operator 0 "comparison_operator"
                 [(match_operand:DI 1 "register_operand")
                  (match_operand:DI 2 "register_operand")])
      (label_ref (match_operand 3 ""))
  (pc)))]
  ""
{
  if (GET_CODE(operands[0]) != EQ && GET_CODE(operands[1]) != NE ) {
	dadao_expand_conditional_branch(GET_CODE(operands[0]),operands[1],operands[2],operands[3]);
	DONE;
  }
})

(define_code_iterator cond [lt ltu gt gtu le leu ge geu])
(define_code_attr cc [(lt "lt") (ltu "bl") (gt "gt") (gtu "ab")
                      (le "le") (leu "be") (ge "ge") (geu "ae")])

;;used for built-in function
(define_insn "set<code>"
  [(set (match_operand:DI 0 "" "=D")
    (if_then_else
       (cond (match_operand:DI 1 "" "D")
             (match_operand:DI 2 "" "D"))
       (const_int 1)
       (const_int 0)))]
  ""
{
  return "set.<cc>\\t=%0, %1, %2";
}
)

(define_insn "condjump"
  [(set (pc)
    (if_then_else (eq (match_operand 0)
      (const_int 1))
      (label_ref (match_operand 1))
      (pc)))]
  ""
{
  return "bne\\t%0, $zero, %l1";
}
)

(define_insn "*cbranch"
  [(set (pc)
    (if_then_else (match_operator 0 "comparison_operator"
                 [(match_operand:DI 1 "register_operand")
                  (match_operand:DI 2 "register_operand")])
      (label_ref (match_operand 3 ""))
      (pc)))]
  ""
{
  if (GET_CODE(operands[0]) == EQ)
    return "beq\\t%1, %2, %l3";
  else if (GET_CODE(operands[0]) == NE)
    return "bne\\t%1, %2, %l3";
  else return "UNREACHABLE";
}
)

;; -------------------------------------------------------------------------
;; Branch instructions
;; -------------------------------------------------------------------------

;; -------------------------------------------------------------------------
;; Call and Jump instructions
;; -------------------------------------------------------------------------

(define_insn "indirect_jump"
  [(set (pc) (match_operand:DI 0 "nonimmediate_operand" ""))]
  ""
  "jump\\t%0")

(define_insn "jump"
  [(set (pc) (label_ref (match_operand 0 "" "")))]
  ""
  "jump\\t%l0"
 )

(define_insn "call"
  [(call (match_operand:DI 0 "memory_operand" "m")
		(match_operand 1 "general_operand" ""))]
  ""
  "call\\t%0"
)

(define_insn "call_value"
  [(set (match_operand 0 "" "")
  (call (match_operand:DI 1 "memory_operand" "m")
        (match_operand 2 "general_operand" "")))]
  ""
  "call\\t%1"
)

;; -------------------------------------------------------------------------
;; Prologue & Epilogue
;; -------------------------------------------------------------------------

(define_expand "prologue"
  [(clobber (const_int 0))]
  ""
  "
{
  dadao_expand_prologue ();
  DONE;
}
")

(define_expand "epilogue"
  [(return)]
  ""
  "
{
  dadao_expand_epilogue ();
  DONE;
}
")

(define_insn "returner"
  [(return)]
  "reload_completed"
  "ret")
