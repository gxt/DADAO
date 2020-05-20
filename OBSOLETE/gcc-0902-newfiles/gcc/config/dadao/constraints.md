;; Constraint definitions for DADAO
;; Copyright (C) 2019-2033 by Guan Xuetao (AT) Peking Univ.
;;
;; Contributed by:
;;   2019:
;;	Shi Yining <1700012728@pku.edu.cn>
;;	Guan Xuetao <gxt@pku.edu.cn>

;; -------------------------------------------------------------------------
;; Constraints
;; -------------------------------------------------------------------------

(define_register_constraint "D"
 "DATA_REGS"
 "data register constraint")

(define_register_constraint "B"
 "BASE_REGS"
 "base register constraint")
