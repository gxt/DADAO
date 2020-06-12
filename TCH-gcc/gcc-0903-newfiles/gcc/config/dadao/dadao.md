;; vim: filetype=lisp

;; GCC machine description for DADAO
;; Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
;; Contributed by Guan Xuetao <gxt@pku.edu.cn>

;; The original PO technology requires these to be ordered by speed,
;; so that assigner will pick the fastest.

;; See file "rtl.def" for documentation on define_insn, match_*, et al.

;; Uses of UNSPEC in this file:
;; UNSPEC_VOLATILE:
;;
;;	1	nonlocal_goto_receiver
;;

;; The order of insns is as in Node: Standard Names, with smaller modes
;; before bigger modes.

(define_constants
  [(DADAO_rJ_REGNUM 259)
   (DADAO_rR_REGNUM 260)
   (DADAO_fp_rO_OFFSET -24)]
)

(define_mode_iterator	QHSD	[QI HI SI DI])
(define_mode_attr	bwto	[(QI "b") (HI "w") (SI "t") (DI "o")])

;; Operand and operator predicates.

(include "predicates.md")
(include "constraints.md")

(include "dadao-generic.md")
(include "dadao-rg.md")
(include "dadao-rp.md")
(include "dadao-rf.md")

;; FIXME: Can we remove the reg-to-reg for smaller modes?  Shouldn't they
;; be synthesized ok?
(define_insn "*movqi"
  [(set (match_operand:QI 0 "nonimmediate_operand" "=r,m")
        (match_operand:QI 1 "general_operand"       "m,r"))]
  ""
  "@
	ldb%U0	%0, %1
	stb	%1, %0")

(define_insn "*movhi"
  [(set (match_operand:HI 0 "nonimmediate_operand" "=r,m")
        (match_operand:HI 1 "general_operand"       "m,r"))]
  ""
  "@
	ldw%U0	%0, %1
	stw	%1, %0")

;; gcc.c-torture/compile/920428-2.c fails if there's no "n".
(define_insn "*movsi"
  [(set (match_operand:SI 0 "nonimmediate_operand" "=r,m")
        (match_operand:SI 1 "general_operand"       "m,r"))]
  ""
  "@
	ldt%U0	%0, %1
	stt	%1, %0")

;; We assume all "s" are addresses.  Does that hold?
(define_insn "*movdi"
  [(set (match_operand:DI 0 "nonimmediate_operand" "=r,m,  Rp,Rp")
	(match_operand:DI 1 "general_operand"	    "m,r,Ttr,s"))]
  ""
  "@
	ldo	%0, %1
	sto	%1, %0
	geta	%0, %1
	geta	%0, %1")

;; We need to be able to move around the values used as condition codes.
;; First spotted as reported in
;; <URL:http://gcc.gnu.org/ml/gcc-bugs/2003-03/msg00008.html> due to
;; changes in loop optimization.  The file machmode.def says they're of
;; size 4 QI.  Valid bit-patterns correspond to integers -1, 0 and 1, so
;; we treat them as signed entities; see dadao-modes.def.  The following
;; expanders should cover all MODE_CC modes, and expand for this pattern.
(define_insn "*movcc_expanded"
  [(set (match_operand 0 "nonimmediate_operand" "=r,Rs,r,r,m")
	(match_operand 1 "nonimmediate_operand"  "r,r,Rs,m,r"))]
  "GET_MODE_CLASS (GET_MODE (operands[0])) == MODE_CC
   && GET_MODE_CLASS (GET_MODE (operands[1])) == MODE_CC"
  "@
	or	%0, %1, 0
	put	%0, %1
	get	%0, %1
	ldt	%0, %1
	stt	%1, %0")

(define_expand "movcc"
  [(set (match_operand:CC 0 "nonimmediate_operand" "")
	(match_operand:CC 1 "nonimmediate_operand" ""))]
  ""
  "")

(define_expand "movcc_uns"
  [(set (match_operand:CC_UNS 0 "nonimmediate_operand" "")
	(match_operand:CC_UNS 1 "nonimmediate_operand" ""))]
  ""
  "")

(define_expand "movcc_fp"
  [(set (match_operand:CC_FP 0 "nonimmediate_operand" "")
	(match_operand:CC_FP 1 "nonimmediate_operand" ""))]
  ""
  "")

(define_expand "movcc_fpeq"
  [(set (match_operand:CC_FPEQ 0 "nonimmediate_operand" "")
	(match_operand:CC_FPEQ 1 "nonimmediate_operand" ""))]
  ""
  "")

(define_expand "movcc_fun"
  [(set (match_operand:CC_FUN 0 "nonimmediate_operand" "")
	(match_operand:CC_FUN 1 "nonimmediate_operand" ""))]
  ""
  "")

;; FIXME: Should we define_expand for smin, smax, umin, umax using a
;; nifty conditional sequence?

;; FIXME:  When TImode works for other reasons (like cross-compiling from
;; a 32-bit host), add back umulditi3 and umuldi3_highpart here.

;; FIXME: Check what's really reasonable for the mod part.

;; One day we might persuade GCC to expand divisions with constants the
;; way DADAO does; giving the remainder the sign of the divisor.  But even
;; then, it might be good to have an option to divide the way "everybody
;; else" does.  Perhaps then, this option can be on by default.  However,
;; it's not likely to happen because major (C, C++, Fortran) language
;; standards in effect at 2002-04-29 reportedly demand that the sign of
;; the remainder must follow the sign of the dividend.

(define_insn "divmoddi4"
  [(set (match_operand:DI 0 "rg_class_operand" "=Rg")
	(div:DI (match_operand:DI 1 "rg_class_operand" "Rg")
		(match_operand:DI 2 "dd_ii_ri_operand" "RgId")))
   (set (match_operand:DI 3 "rg_class_operand" "=Sy")
	(mod:DI (match_dup 1) (match_dup 2)))]
  ;; Do the library stuff later.
  "TARGET_KNUTH_DIVISION"
	"div	%0, %1, %2")

(define_insn "udivmoddi4"
  [(set (match_operand:DI 0 "rg_class_operand" "=Rg")
	(udiv:DI (match_operand:DI 1 "rg_class_operand" "Rg")
		 (match_operand:DI 2 "dd_ii_ri_operand" "RgId")))
   (set (match_operand:DI 3 "rg_class_operand" "=Sy")
	(umod:DI (match_dup 1) (match_dup 2)))]
  ""
	"divu	%0, %1, %2")

(define_expand "divdi3"
  [(parallel
    [(set (match_operand:DI 0 "rg_class_operand" "=&Rg")
	  (div:DI (match_operand:DI 1 "rg_class_operand" "Rg")
		  (match_operand:DI 2 "rg_class_operand" "Rg")))
     (clobber (scratch:DI))
     (clobber (scratch:DI))
     (clobber (reg:DI DADAO_rR_REGNUM))])]
  "! TARGET_KNUTH_DIVISION"
  "")

;; The %2-is-%1-case is there just to make sure things don't fail.  Could
;; presumably happen with optimizations off; no evidence.
(define_insn "*divdi3_nonknuth"
  [(set (match_operand:DI 0 "rg_class_operand" "=&Rg,&Rg")
	(div:DI (match_operand:DI 1 "rg_class_operand" "Rg,Rg")
		(match_operand:DI 2 "rg_class_operand" "1,Rg")))
   (clobber (match_scratch:DI 3 "=1,1"))
   (clobber (match_scratch:DI 4 "=2,2"))
   (clobber (reg:DI DADAO_rR_REGNUM))]
  "! TARGET_KNUTH_DIVISION"
  "@
	setwl	%0, 1
	xor	rg63, %1, %2	\;\
	subu	%0, zero, %2	\;\
	csn	%2, %2, %0	\;\
	subu	%0, zero, %1	\;\
	csn	%1, %1, %0	\;\
	divu	%0, %1, %2	\;\
	subu	%1, zero, %0	\;\
	csn	%0, rg63, %1")

(define_expand "moddi3"
  [(parallel
    [(set (match_operand:DI 0 "rg_class_operand" "=&Rg")
	  (mod:DI (match_operand:DI 1 "rg_class_operand" "Rg")
		  (match_operand:DI 2 "rg_class_operand" "Rg")))
     (clobber (scratch:DI))
     (clobber (scratch:DI))
     (clobber (reg:DI DADAO_rR_REGNUM))])]
  "! TARGET_KNUTH_DIVISION"
  "")

;; The %2-is-%1-case is there just to make sure things don't fail.  Could
;; presumably happen with optimizations off; no evidence.
(define_insn "*moddi3_nonknuth"
  [(set (match_operand:DI 0 "rg_class_operand" "=&Rg,&Rg")
	(mod:DI (match_operand:DI 1 "rg_class_operand" "Rg,Rg")
		(match_operand:DI 2 "rg_class_operand" "1,Rg")))
   (clobber (match_scratch:DI 3 "=1,1"))
   (clobber (match_scratch:DI 4 "=2,2"))
   (clobber (reg:DI DADAO_rR_REGNUM))]
  "! TARGET_KNUTH_DIVISION"
  "@
	setwl	%0, 0
	subu	%0, zero, %2	\;\
	csn	%2, %2, %0	\;\
	subu	rg63, zero, %1	\;\
	csn	%1, %1, rg63	\;\
	divu	%1, %1, %2	\;\
	get	%0, rR\;\
	subu	%2, zero, %0	\;\
	csnn	%0, rg63, %2")

;; When the user-patterns expand, the resulting insns will match the
;; patterns below.

;; We can fold the signed-compare where the register value is
;; already equal to (compare:CCTYPE (reg) (const_int 0)).
;;  We can't do that at all for floating-point, due to NaN, +0.0
;; and -0.0, and we can only do it for the non/zero test of
;; unsigned, so that has to be done another way.
;;  FIXME: Perhaps a peep2 changing CCcode to a new code, that
;; gets folded here.
(define_insn "*cmpdi_folded"
  [(set (match_operand:CC 0 "register_operand" "=r")
	(compare:CC
	 (match_operand:DI 1 "register_operand" "r")
	 (const_int 0)))]
  ;; FIXME: Can we test equivalence any other way?
  ;; FIXME: Can we fold any other way?
  "REG_P (operands[0]) && REG_P (operands[1])
   && REGNO (operands[1]) == REGNO (operands[0])"
  ;; DADAO FIXME: remove folded label and %% prefix
	"cmp	%0, %1, 0")

(define_insn "*cmps"
  [(set (match_operand:CC 0 "register_operand" "=r")
	(compare:CC
	 (match_operand:DI 1 "register_operand" "r")
	 (match_operand:DI 2 "dd_ii_ri_operand" "rId")))]
  ""
	"cmp	%0, %1, %2")

(define_insn "*cmpu"
  [(set (match_operand:CC_UNS 0 "register_operand" "=r")
	(compare:CC_UNS
	 (match_operand:DI 1 "register_operand" "r")
	 (match_operand:DI 2 "dd_ii_ri_operand" "rId")))]
  ""
	"cmpu	%0, %1, %2")

(define_expand "movdicc"
  [(set (match_dup 4) (match_dup 5))
   (set (match_operand:DI 0 "register_operand" "")
	(if_then_else:DI
	 (match_operand 1 "comparison_operator" "")
	 (match_operand:DI 2 "dadao_reg_or_8bit_operand" "")
	 (match_operand:DI 3 "dadao_reg_or_8bit_operand" "")))]
  ""
  "
{
  enum rtx_code code = GET_CODE (operands[1]);
  if (code == LE || code == GE)
    FAIL;

  operands[4] = dadao_gen_compare_reg (code, XEXP (operands[1], 0),
				      XEXP (operands[1], 1));
  operands[5] = gen_rtx_COMPARE (GET_MODE (operands[4]),
				 XEXP (operands[1], 0),
				 XEXP (operands[1], 1));
  operands[1] = gen_rtx_fmt_ee (code, VOIDmode, operands[4], const0_rtx);
}")

;; FIXME: Is this the right way to do "folding" of CCmode -> DImode?
(define_insn "*movdicc_real_foldable"
  [(set (match_operand:DI 0 "register_operand" "=r,r,r,r")
	(if_then_else:DI
	 (match_operator 2 "dadao_foldable_comparison_operator"
			 [(match_operand:DI 3 "register_operand" "r,r,r,r")
			  (const_int 0)])
	 (match_operand:DI 1 "dadao_reg_or_8bit_operand" "rTti,0 ,rTti,GM")
	 (match_operand:DI 4 "dadao_reg_or_8bit_operand" "0 ,rTti,GM,rTti")))]
  ""
  "@
	cs%d2	%0, %3, %1
	cs%D2	%0, %3, %4
	xor	%0, %0, %0	\;	cs%d2	%0, %3, %1
	xor	%0, %0, %0	\;	cs%D2	%0, %3, %4")

(define_insn "*movdicc_real_reversible"
  [(set
    (match_operand:DI 0 "register_operand"	   "=r ,r ,r ,r")
    (if_then_else:DI
     (match_operator
      2 "dadao_comparison_operator"
      [(match_operand 3 "dadao_reg_cc_operand"	    "r ,r ,r ,r")
      (const_int 0)])
     (match_operand:DI 1 "dadao_reg_or_8bit_operand" "rTti,0 ,rTti,GM")
     (match_operand:DI 4 "dadao_reg_or_8bit_operand" "0 ,rTti,GM,rTti")))]
  "REVERSIBLE_CC_MODE (GET_MODE (operands[3]))"
  "@
	cs%d2	%0, %3, %1
	cs%D2	%0, %3, %4
	xor	%0, %0, %0	\;	cs%d2	%0, %3, %1
	xor	%0, %0, %0	\;	cs%D2	%0, %3, %4")

(define_insn "*movdicc_real_nonreversible"
  [(set
    (match_operand:DI 0 "register_operand"	   "=r ,r")
    (if_then_else:DI
     (match_operator
      2 "dadao_comparison_operator"
      [(match_operand 3 "dadao_reg_cc_operand"	    "r ,r")
      (const_int 0)])
     (match_operand:DI 1 "dadao_reg_or_8bit_operand" "rTti,rTti")
     (match_operand:DI 4 "dadao_reg_or_0_operand" "0 ,GM")))]
  "!REVERSIBLE_CC_MODE (GET_MODE (operands[3]))"
  "@
	cs%d2	%0, %3, %1
	xor	%0, %0, %0	\;	cs%d2	%0, %3, %1")

;; FIXME: scc insns will probably help, I just skip them
;; right now.  Revisit.

(define_expand "cbranchdi4"
  [(set (match_dup 4)
        (match_op_dup 5
         [(match_operand:DI 1 "register_operand" "")
          (match_operand:DI 2 "dadao_reg_or_8bit_operand" "")]))
   (set (pc)
        (if_then_else
              (match_operator 0 "ordered_comparison_operator"
               [(match_dup 4)
                (const_int 0)])
              (label_ref (match_operand 3 "" ""))
              (pc)))]
  ""
  "
{
  operands[4] = dadao_gen_compare_reg (GET_CODE (operands[0]),
                                      operands[1], operands[2]);
  operands[5] = gen_rtx_fmt_ee (COMPARE,
                                GET_MODE (operands[4]),
                                operands[1], operands[2]);
}")


;; FIXME: we can emit an unordered-or-*not*-equal compare in one insn, but
;; there's no RTL code for it.  Maybe revisit in future.

;; FIXME: Odd/Even matchers?
(define_insn "*bCC_foldable"
  [(set (pc)
	(if_then_else
	 (match_operator 1 "dadao_foldable_comparison_operator"
			 [(match_operand:DI 2 "register_operand" "r")
			  (const_int 0)])
	 (label_ref (match_operand 0 "" ""))
	 (pc)))]
  ""
  "B%d1 %2,%0")

(define_insn "*bCC"
  [(set (pc)
	(if_then_else
	 (match_operator 1 "dadao_comparison_operator"
			 [(match_operand 2 "dadao_reg_cc_operand" "r")
			  (const_int 0)])
	 (label_ref (match_operand 0 "" ""))
	 (pc)))]
  ""
  "B%d1 %2,%0")

(define_insn "*bCC_inverted_foldable"
  [(set (pc)
	(if_then_else
	 (match_operator 1 "dadao_foldable_comparison_operator"
			 [(match_operand:DI 2 "register_operand" "r")
			  (const_int 0)])
		      (pc)
		      (label_ref (match_operand 0 "" ""))))]
;; REVERSIBLE_CC_MODE is checked by dadao_foldable_comparison_operator.
  ""
  "B%D1 %2,%0")

(define_insn "*bCC_inverted"
  [(set (pc)
	(if_then_else
	 (match_operator 1 "dadao_comparison_operator"
			 [(match_operand 2 "dadao_reg_cc_operand" "r")
			  (const_int 0)])
	 (pc)
	 (label_ref (match_operand 0 "" ""))))]
  "REVERSIBLE_CC_MODE (GET_MODE (operands[2]))"
  "B%D1 %2,%0")

(define_expand "call"
  [(parallel [(call (match_operand:SI 0 "memory_operand" "")
		    (match_operand 1 "general_operand" ""))
	      (use (match_operand 2 "general_operand" ""))
	      (clobber (match_dup 4))])
   (set (match_dup 4) (match_dup 3))]
  ""
  "
{
  /* The caller checks that the operand is generally valid as an
     address, but at -O0 nothing makes sure that it's also a valid
     call address for a *call*; a dadao_symbolic_or_address_operand.
     Force into a register if it isn't.  */
  if (!dadao_symbolic_or_address_operand (XEXP (operands[0], 0),
					 GET_MODE (XEXP (operands[0], 0))))
    operands[0]
      = replace_equiv_address (operands[0],
			       force_reg (Pmode, XEXP (operands[0], 0)));

  /* Since the epilogue 'uses' the return address, and it is clobbered
     in the call, and we set it back after every call (all but one setting
     will be optimized away), integrity is maintained.  */
  operands[3]
    = dadao_get_hard_reg_initial_val (Pmode,
				     DADAO_INCOMING_RETURN_ADDRESS_REGNUM);

  /* FIXME: There's a bug in gcc which causes NULL to be passed as
     operand[2] when we get out of registers, which later confuses gcc.
     Work around it by replacing it with const_int 0.  Possibly documentation
     error too.  */
  if (operands[2] == NULL_RTX)
    operands[2] = const0_rtx;

  operands[4] = gen_rtx_REG (DImode, DADAO_INCOMING_RETURN_ADDRESS_REGNUM);
}")

(define_expand "call_value"
  [(parallel [(set (match_operand 0 "" "")
		   (call (match_operand:SI 1 "memory_operand" "")
			 (match_operand 2 "general_operand" "")))
	      (use (match_operand 3 "general_operand" ""))
	      (clobber (match_dup 5))])
   (set (match_dup 5) (match_dup 4))]
  ""
  "
{
  /* The caller checks that the operand is generally valid as an
     address, but at -O0 nothing makes sure that it's also a valid
     call address for a *call*; a dadao_symbolic_or_address_operand.
     Force into a register if it isn't.  */
  if (!dadao_symbolic_or_address_operand (XEXP (operands[1], 0),
					 GET_MODE (XEXP (operands[1], 0))))
    operands[1]
      = replace_equiv_address (operands[1],
			       force_reg (Pmode, XEXP (operands[1], 0)));

  /* Since the epilogue 'uses' the return address, and it is clobbered
     in the call, and we set it back after every call (all but one setting
     will be optimized away), integrity is maintained.  */
  operands[4]
    = dadao_get_hard_reg_initial_val (Pmode,
				     DADAO_INCOMING_RETURN_ADDRESS_REGNUM);

  /* FIXME: See 'call'.  */
  if (operands[3] == NULL_RTX)
    operands[3] = const0_rtx;

  /* FIXME: Documentation bug: operands[3] (operands[2] for 'call') is the
     *next* argument register, not the number of arguments in registers.
     (There used to be code here where that mattered.)  */

  operands[5] = gen_rtx_REG (DImode, DADAO_INCOMING_RETURN_ADDRESS_REGNUM);
}")

;; Don't use 'p' here.  A 'p' must stand first in constraints, or reload
;; messes up, not registering the address for reload.  Several C++
;; testcases, including g++.brendan/crash40.C.  FIXME: This is arguably a
;; bug in gcc.  Note line ~2612 in reload.c, that does things on the
;; condition <<else if (constraints[i][0] == 'p')>> and the comment on
;; ~3017 that says:
;; <<   case 'p':
;;	     /* All necessary reloads for an address_operand
;;	        were handled in find_reloads_address.  */>>
;; Sorry, I have not dug deeper.  If symbolic addresses are used
;; rarely compared to addresses in registers, disparaging the
;; first ("p") alternative by adding ? in the first operand
;; might do the trick.  We define 'U' as a synonym to 'p', but without the
;; caveats (and very small advantages) of 'p'.
;; As of r190682 still so: newlib/libc/stdlib/dtoa.c ICEs if "p" is used.
(define_insn "*call_real"
  [(call (mem:SI
	  (match_operand:DI 0 "dadao_symbolic_or_address_operand" "s,RpU"))
	 (match_operand 1 "" ""))
   (use (match_operand 2 "" ""))
   (clobber (reg:DI DADAO_rJ_REGNUM))]
  ""
  "@
	call	%0
	call	%2, %a0")

(define_insn "*call_value_real"
  [(set (match_operand 0 "register_operand" "=r,r")
	(call (mem:SI
	       (match_operand:DI 1 "dadao_symbolic_or_address_operand" "s,RpU"))
	      (match_operand 2 "" "")))
  (use (match_operand 3 "" ""))
  (clobber (reg:DI DADAO_rJ_REGNUM))]
  ""
  "@
	call	%1
	call	%3, %a1")

;; I hope untyped_call and untyped_return are not needed for DADAO.
;; Users of Objective-C will notice.

; Generated by GCC.
(define_expand "return"
  [(return)]
  "dadao_use_simple_return ()"
  "")

; Generated by the epilogue expander.
(define_insn "*expanded_return"
  [(return)]
  ""
	"ret")

(define_expand "prologue"
  [(const_int 0)]
  ""
  "dadao_expand_prologue (); DONE;")

; Note that the (return) from the expander itself is always the last insn
; in the epilogue.
(define_expand "epilogue"
  [(return)]
  ""
  "dadao_expand_epilogue ();")

(define_insn "jump"
  [(set (pc) (label_ref (match_operand 0 "" "")))]
  ""
	"jump	%0")

(define_insn "indirect_jump"
  [(set (pc) (match_operand 0 "address_operand" "p"))]
  ""
	"jump	rp63, %a0")

;; FIXME: This is just a jump, and should be expanded to one.
(define_insn "tablejump"
  [(set (pc) (match_operand:DI 0 "address_operand" "p"))
   (use (label_ref (match_operand 1 "" "")))]
  ""
	"jump	rp63, %a0")

;; The only peculiar thing is that the register stack has to be unwound at
;; nonlocal_goto_receiver.  At each function that has a nonlocal label, we
;; save at function entry the location of the "alpha" register stack
;; pointer, rO, in a stack slot known to that function (right below where
;; the frame-pointer would be located).
;; In the nonlocal goto receiver, we unwind the register stack by a series
;; of "pop 0,0" until rO equals the saved value.  (If it goes lower, we
;; should die with a trap.)
(define_expand "nonlocal_goto_receiver"
  [(parallel [(unspec_volatile [(match_dup 1)] 1)
	      (clobber (scratch:DI))
	      (clobber (reg:DI DADAO_rJ_REGNUM))])
   (set (reg:DI DADAO_rJ_REGNUM) (match_dup 0))]
  ""
  "
{
  operands[0]
    = dadao_get_hard_reg_initial_val (Pmode,
				     DADAO_INCOMING_RETURN_ADDRESS_REGNUM);

  /* We need the frame-pointer to be live or the equivalent
     expression, so refer to it in the pattern.  We can't use a MEM
     (that may contain out-of-range offsets in the final expression)
     for fear that middle-end will legitimize it or replace the address
     using temporary registers (which are not revived at this point).  */
  operands[1] = frame_pointer_rtx;

  /* Mark this function as containing a landing-pad.  */
  cfun->machine->has_landing_pad = 1;
}")

;; GCC can insist on using saved registers to keep the slot address in
;; "across" the exception, or (perhaps) to use saved registers in the
;; address and re-use them after the register stack unwind, so it's best
;; to form the address ourselves.
(define_insn "*nonlocal_goto_receiver_expanded"
  [(unspec_volatile [(match_operand:DI 1 "frame_pointer_operand" "Yf")] 1)
   (clobber (match_scratch:DI 0 "=&r"))
   (clobber (reg:DI DADAO_rJ_REGNUM))]
  ""
{
  rtx my_operands[3];
  const char *my_template
    = "	geta	rg63, 0f\;\
	put	rJ, rg63\;\
	ldo	rg63, %a0, 0\n\
0:\;	get	%1, rO\;\
	cmpu	%1, %1, rg63	\;\
	bnp	%1, 1f\;\
	ret\n1:";

  my_operands[1] = operands[0];
  my_operands[2] = GEN_INT (-DADAO_fp_rO_OFFSET);

  if (operands[1] == hard_frame_pointer_rtx)
    {
      dadao_output_register_setting (asm_out_file, REGNO (operands[0]),
				    DADAO_fp_rO_OFFSET, 1);
      my_operands[0]
	= gen_rtx_PLUS (Pmode, hard_frame_pointer_rtx, operands[0]);
    }
  else
    {
      int64_t offs = INTVAL (XEXP (operands[1], 1));
      offs += DADAO_fp_rO_OFFSET;

      if (insn_const_int_ok_for_constraint (offs, CONSTRAINT_Tti))
	my_operands[0]
	  = gen_rtx_PLUS (Pmode, stack_pointer_rtx, GEN_INT (offs));
      else
	{
	  dadao_output_register_setting (asm_out_file, REGNO (operands[0]),
					offs, 1);
	  my_operands[0]
	    = gen_rtx_PLUS (Pmode, stack_pointer_rtx, operands[0]);
	}
    }

  output_asm_insn (my_template, my_operands);
  return "";
})

(define_insn "*nor"
  [(set (match_operand:DI 0 "register_operand" "=r")
	(and:DI
	 (not:DI (match_operand:DI 1 "register_operand" "%r"))
	 (not:DI (match_operand:DI 2 "dadao_reg_or_8bit_operand" "rTti"))))]
  ""
	"or	%0, %1, %2	\;	not	%0, %0, 0")

;; Local Variables:
;; mode: lisp
;; indent-tabs-mode: t
;; End:
