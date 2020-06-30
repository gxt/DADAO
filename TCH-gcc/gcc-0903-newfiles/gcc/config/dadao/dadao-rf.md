;; vim: filetype=lisp

;; GCC machine description for DADAO floating-point
;; Copyright (C) 2020-2033 Guan Xuetao (AT) Peking Univ.
;; Contributed by Guan Xuetao <gxt@pku.edu.cn>

;; Note that we move around the float as a collection of bits; no
;; conversion to double.

(define_mode_iterator	SFDF	[SF DF])

(define_insn "mov<mode>"
 [(set (match_operand:SFDF 0 "nonimmediate_operand" "= Rg, Rg, Rf, Rf, Rf, Rg, Rf,  m,  m, ??Rg, ??Rf")
       (match_operand:SFDF 1 "general_operand"      "  Rg, Rf, Rg, Rf, Gz,  m,  m, Rg, Rf,    F,    F"))]
	""
	"@
	or	%0, %1, 0
	get.rf	%0, %1
	put.rf	%0, %1
	mov.rf	%0, %1
	put.rf	%0, zero
	ldt	%0, %1
	ldt.rf	%0, %1
	stt	%1, %0
	stt.rf	%1, %0
	seto	%0, %1
	seto	datao1, %1	\;	put.rf	%0, datao1")

(define_insn "adddf3"
  [(set      (match_operand:DF 0 "rf_class_operand" "= Rf")
    (plus:DF (match_operand:DF 1 "rf_class_operand" "% Rf")
             (match_operand:DF 2 "rf_class_operand" "  Rf")))]
	""
	"fadd	%0, %1, %2")

(define_insn "subdf3"
  [(set       (match_operand:DF 0 "rf_class_operand" "= Rf")
    (minus:DF (match_operand:DF 1 "rf_class_operand" "  Rf")
              (match_operand:DF 2 "rf_class_operand" "  Rf")))]
	""
	"fsub	%0, %1, %2")

(define_insn "muldf3"
  [(set      (match_operand:DF 0 "rf_class_operand" "= Rf")
    (mult:DF (match_operand:DF 1 "rf_class_operand" "  Rf")
             (match_operand:DF 2 "rf_class_operand" "  Rf")))]
	""
	"fmul	%0, %1, %2")

(define_insn "divdf3"
  [(set     (match_operand:DF 0 "rf_class_operand" "= Rf")
    (div:DF (match_operand:DF 1 "rf_class_operand" "  Rf")
            (match_operand:DF 2 "rf_class_operand" "  Rf")))]
	""
	"fdiv	%0, %1, %2")

;; FIXME: Is "frem" doing the right operation for moddf3?
(define_insn "moddf3"
  [(set     (match_operand:DF 0 "rf_class_operand" "= Rf")
    (mod:DF (match_operand:DF 1 "rf_class_operand" "  Rf")
            (match_operand:DF 2 "rf_class_operand" "  Rf")))]
	""
	"frem	%0, %1, %2")

(define_expand "negdf2"
  [(parallel [(set (match_operand:DF 0 "rf_class_operand" "= Rf")
           (neg:DF (match_operand:DF 1 "rf_class_operand" "  Rf")))
              (use (match_dup 2))])]
  ""
{
  /* Emit bit-flipping sequence to be IEEE-safe wrt. -+0.  */
  operands[2] = force_reg (DImode, GEN_INT ((HOST_WIDE_INT) 1 << 63));
})

(define_insn "*expanded_negdf2"
  [(set     (match_operand:DF 0 "rf_class_operand" "= Rf")
    (neg:DF (match_operand:DF 1 "rf_class_operand" "  Rf")))
      (use  (match_operand:DI 2 "rg_class_operand" "  Rg"))]
	""
	"get.rf	datao1, %1	\;	xor	datao1, datao1, %2	\;	put.rf	%0, datao1")

;; FIXME: define_expand for absdi2?

;; (define_insn "absdf2"
;;   [(set (match_operand:DF 0 "rf_class_operand" "=Rf")
;; 	(abs:DF (match_operand:DF 1 "rf_class_operand" "0")))]
;;   ""
;; 	"andnwh	%0, 0x8000")

(define_insn "sqrtdf2"
  [(set      (match_operand:DF 0 "rf_class_operand" "= Rf")
    (sqrt:DF (match_operand:DF 1 "rf_class_operand" "  Rf")))]
	""
	"fsqrt	%0, %1, 0")

(define_insn "*fcmp"
  [(set            (match_operand:CC_FP 0 "rg_class_operand" "= Rg")
    (compare:CC_FP (match_operand:DF    1 "rf_class_operand" "  Rf")
                   (match_operand:DF    2 "rf_class_operand" "  Rf")))]
	""
	"fcmp%e0	%0, %1, %2")

;; FIXME: for -mieee, add fsub %0,%1,%1\;fsub %0,%2,%2 before to
;; make signalling compliant.
(define_insn "*feql"
  [(set              (match_operand:CC_FPEQ 0 "rg_class_operand" "= Rg")
    (compare:CC_FPEQ (match_operand:DF      1 "rf_class_operand" "  Rf")
                     (match_operand:DF      2 "rf_class_operand" "  Rf")))]
	""
	"feql%e0	%0, %1, %2")

(define_insn "*fun"
  [(set             (match_operand:CC_FUN 0 "rg_class_operand" "= Rg")
    (compare:CC_FUN (match_operand:DF     1 "rf_class_operand" "  Rf")
                    (match_operand:DF     2 "rf_class_operand" "  Rf")))]
	""
	"fun%e0	%0, %1, %2")

;; In order to get correct rounding, we have to use SFLOT and SFLOTU for
;; conversion.  They do not convert to SFmode; they convert to DFmode,
;; with rounding as of SFmode.  They are not usable as is, but we pretend
;; we have a single instruction but emit two.

;; Note that this will (somewhat unexpectedly) create an inexact
;; exception if rounding is necessary - has to be masked off in crt0?
(define_expand "floatdisf2"
  [(parallel [(set (match_operand:SF 0 "nonimmediate_operand" "=Rfm")
		   (float:SF
		    (match_operand:DI 1 "rg_class_operand" "Rg")))
	      ;; Let's use a DI scratch, since SF don't generally get into
	      ;; registers.  Dunno what's best; it's really a DF, but that
	      ;; doesn't logically follow from operands in the pattern.
	      (clobber (match_scratch:DF 2 "=&Rf"))])]
  ""
  "
{
  if (GET_CODE (operands[0]) != MEM)
    {
      rtx stack_slot;

      /* FIXME: This stack-slot remains even at -O3.  There must be a
	 better way.  */
      stack_slot
	= validize_mem (assign_stack_temp (SFmode,
					   GET_MODE_SIZE (SFmode)));
      emit_insn (gen_floatdisf2 (stack_slot, operands[1]));
      emit_move_insn (operands[0], stack_slot);
      DONE;
    }
}")

(define_insn "*floatdisf2_real"
  [(set (match_operand:SF 0 "memory_operand" "=m")
	(float:SF
	 (match_operand:DI 1 "rg_class_operand" "Rg")))
   (clobber (match_scratch:DF 2 "=&Rf"))]
  ""
	"sflot	%2, %1, 0\;\
	stt.rf	%2, %0")

(define_expand "floatunsdisf2"
  [(parallel [(set (match_operand:SF 0 "nonimmediate_operand" "=Rfm")
		   (unsigned_float:SF
		    (match_operand:DI 1 "rg_class_operand" "Rg")))
	      ;; Let's use a DI scratch, since SF don't generally get into
	      ;; registers.  Dunno what's best; it's really a DF, but that
	      ;; doesn't logically follow from operands in the pattern.
	      (clobber (scratch:DF))])]
  ""
  "
{
  if (GET_CODE (operands[0]) != MEM)
    {
      rtx stack_slot;

      /* FIXME: This stack-slot remains even at -O3.  Must be a better
	 way.  */
      stack_slot
	= validize_mem (assign_stack_temp (SFmode,
					   GET_MODE_SIZE (SFmode)));
      emit_insn (gen_floatunsdisf2 (stack_slot, operands[1]));
      emit_move_insn (operands[0], stack_slot);
      DONE;
    }
}")

(define_insn "*floatunsdisf2_real"
  [(set (match_operand:SF 0 "memory_operand" "=m")
	(unsigned_float:SF
	 (match_operand:DI 1 "rg_class_operand" "Rg")))
   (clobber (match_scratch:DF 2 "=&Rf"))]
  ""
	"sflotu	%2, %1, 0\;\
	stt.rf	%2, %0")

;; Note that this will (somewhat unexpectedly) create an inexact
;; exception if rounding is necessary - has to be masked off in crt0?
(define_insn "floatdidf2"
  [(set (match_operand:DF 0 "rf_class_operand" "=Rf")
	(float:DF
	 (match_operand:DI 1 "rg_class_operand" "Rg")))]
  ""
	"flot	%0, %1, 0")

(define_insn "floatunsdidf2"
  [(set (match_operand:DF 0 "rf_class_operand" "=Rf")
	(unsigned_float:DF
	 (match_operand:DI 1 "rg_class_operand" "Rg")))]
  ""
	"flotu	%0, %1, 0")

(define_insn "ftruncdf2"
  [(set (match_operand:DF 0 "rf_class_operand" "=Rf")
	(fix:DF (match_operand:DF 1 "rf_class_operand" "Rf")))]
  ""
  ;; ROUND_OFF
	"fint	%0, %1, 1")

;; Note that this will (somewhat unexpectedly) create an inexact
;; exception if rounding is necessary - has to be masked off in crt0?
(define_insn "fix_truncdfdi2"
  [(set (match_operand:DI 0 "rg_class_operand" "=Rg")
	(fix:DI (fix:DF (match_operand:DF 1 "rf_class_operand" "Rf"))))]
  ""
  ;; ROUND_OFF
	"fix	%0, %1, 1")

(define_insn "fixuns_truncdfdi2"
  [(set (match_operand:DI 0 "rg_class_operand" "=Rg")
	(unsigned_fix:DI
	 (fix:DF (match_operand:DF 1 "rf_class_operand" "Rf"))))]
  ""
  ;; ROUND_OFF
	"fixu	%0, %1, 1")

;; It doesn't seem like it's possible to have memory_operand as a
;; predicate here (testcase: libgcc2 floathisf).  FIXME:  Shouldn't it be
;; possible to do that?  Bug in GCC?  Anyway, this used to be a simple
;; pattern with a memory_operand predicate, but was split up with a
;; define_expand with the old pattern as "anonymous".
;; FIXME: Perhaps with TARGET_SECONDARY_MEMORY_NEEDED?
(define_expand "truncdfsf2"
  [(set (match_operand:SF 0 "nonimmediate_operand")
	(float_truncate:SF (match_operand:DF 1 "rf_class_operand")))]
  ""
  "
{
  if (GET_CODE (operands[0]) != MEM)
    {
      /* FIXME: There should be a way to say: 'put this in operands[0]
	 but *after* the expanded insn'.  */
      rtx stack_slot;

      /* There is no sane destination but a register here, if it wasn't
	 already MEM.  (It's too hard to get fatal_insn to work here.)  */
      if (! REG_P (operands[0]))
	internal_error (\"DADAO Internal: Bad truncdfsf2 expansion\");

      /* FIXME: This stack-slot remains even at -O3.  Must be a better
	 way.  */
      stack_slot
	= validize_mem (assign_stack_temp (SFmode,
					   GET_MODE_SIZE (SFmode)));
      emit_insn (gen_truncdfsf2 (stack_slot, operands[1]));
      emit_move_insn (operands[0], stack_slot);
      DONE;
    }
}")

(define_insn "*truncdfsf2_real"
  [(set (match_operand:SF 0 "memory_operand" "=m")
	(float_truncate:SF (match_operand:DF 1 "rf_class_operand" "Rf")))]
  ""
	"stt.rf	%1, %0")

;; Same comment as for truncdfsf2.
(define_expand "extendsfdf2"
  [(set (match_operand:DF 0 "rf_class_operand")
	(float_extend:DF (match_operand:SF 1 "nonimmediate_operand")))]
  ""
  "
{
  if (GET_CODE (operands[1]) != MEM)
    {
      rtx stack_slot;

      /* There is no sane destination but a register here, if it wasn't
	 already MEM.  (It's too hard to get fatal_insn to work here.)  */
      if (! REG_P (operands[0]))
	internal_error (\"DADAO Internal: Bad extendsfdf2 expansion\");

      /* FIXME: This stack-slot remains even at -O3.  There must be a
	 better way.  */
      stack_slot
	= validize_mem (assign_stack_temp (SFmode,
					   GET_MODE_SIZE (SFmode)));
      emit_move_insn (stack_slot, operands[1]);
      emit_insn (gen_extendsfdf2 (operands[0], stack_slot));
      DONE;
    }
}")

(define_insn "*extendsfdf2_real"
  [(set (match_operand:DF 0 "rf_class_operand" "=Rf")
	(float_extend:DF (match_operand:SF 1 "memory_operand" "m")))]
  ""
	"ldt.rf	%0, %1")

(define_expand "cbranchdf4"
  [(set (match_dup 4)
        (match_op_dup 5
         [(match_operand:DF 1 "rf_class_operand" "")
          (match_operand:DF 2 "rf_class_operand" "")]))
   (set (pc)
        (if_then_else
              (match_operator 0 "float_comparison_operator"
               [(match_dup 4)
                (const_int 0)])
              (label_ref (match_operand 3 "" ""))
              (pc)))]
  ""
  "
{
  /* The head comment of optabs.c:can_compare_p says we're required to
     implement this, so we have to clean up the mess here.  */
  if (GET_CODE (operands[0]) == LE || GET_CODE (operands[0]) == GE)
    {
      enum rtx_code ltgt_code = GET_CODE (operands[0]) == LE ? LT : GT;
      emit_cmp_and_jump_insns (operands[1], operands[2], ltgt_code, NULL_RTX,
			       DFmode, 0, operands[3]);
      emit_cmp_and_jump_insns (operands[1], operands[2], EQ, NULL_RTX,
			       DFmode, 0, operands[3]);
      DONE;
    }

  operands[4] = dadao_gen_compare_reg (GET_CODE (operands[0]),
                                      operands[1], operands[2]);
  operands[5] = gen_rtx_fmt_ee (COMPARE,
                                GET_MODE (operands[4]),
                                operands[1], operands[2]);
}")

