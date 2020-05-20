;; Predicate definitions for DADAO
;; Copyright (C) 2019-2033 by Guan Xuetao (AT) Peking Univ.
;;
;; Contributed by:
;;   2019:
;;	Shi Yining <1700012728@pku.edu.cn>
;;	Guan Xuetao <gxt@pku.edu.cn>

;; -------------------------------------------------------------------------
;; Predicates
;; -------------------------------------------------------------------------

;; Nonzero if OP can be source of a simple move operation.

(define_predicate "dadao_general_movsrc_operand"
  (match_code "mem,const_int,reg,subreg,symbol_ref,label_ref,const")
{
  /* Any (MEM LABEL_REF) is OK.  That is a pc-relative load.  */
  return true;

  return general_operand (op, mode);
})

;; Nonzero if OP can be an operand to an add/inc/dec instruction.

(define_predicate "dadao_arith_operand"
  (ior (match_code "reg")
       (and (match_code "const_int")
	    (match_test "IN_RANGE (INTVAL (op), 0, 0xfff)"))))

(define_predicate "dadao_shift_operand"
  (ior (match_code "reg")
       (and (match_code "const_int")
	    (match_test "IN_RANGE (INTVAL (op), 0, 0x3f)"))))
