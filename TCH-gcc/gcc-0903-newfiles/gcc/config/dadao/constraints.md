;; vim: filetype=lisp

;; DADAO constraints
;; Copyright (C) 2020-2033 by Guan Xuetao (AT) Peking Univ.
;;
;; Contributed by:
;;   2020:
;;	Guan Xuetao <gxt@pku.edu.cn>

(define_register_constraint "x" "SYSTEM_REGS"
  "@internal")

(define_register_constraint "y" "REMAINDER_REG"
  "@internal")

(define_register_constraint "z" "HIMULT_REG"
  "@internal")

(define_constraint "Tti"
  "A 8-bit unsigned integer"
  (and (match_code "const_int")
       (match_test "IN_RANGE (ival, 0, 255)")))

(define_constraint "J"
  "A 16-bit unsigned integer."
  (and (match_code "const_int")
       (match_test "IN_RANGE (ival, 0, 65535)")))

(define_constraint "L"
  "@internal"
  (and (match_code "const_int")
       (match_test "dadao_shiftable_wyde_value (ival)")))

(define_constraint "M"
  "The value 0."
  (and (match_code "const_int")
       (match_test "ival == 0")))

(define_constraint "N"
  "@internal"
  (and (match_code "const_int")
       (match_test "dadao_shiftable_wyde_value (~ival)")))

(define_constraint "O"
  "The value 3, 5, 9, or 17."
  (and (match_code "const_int")
       (ior (match_test "ival == 3")
	    (match_test "ival == 5")
	    (match_test "ival == 9")
	    (match_test "ival == 17"))))

(define_constraint "Pbc"
  "A 12-bit unsigned integer"
  (and (match_code "const_int")
       (match_test "IN_RANGE (ival, 0, 4095)")))

(define_constraint "Pnn"
  "An integer between -4095 and 0."
  (and (match_code "const_int")
       (match_test "IN_RANGE (ival, -4095, 0)")))

;; FIXME: M (or G) is redundant.

(define_constraint "G"
  "Floating-point zero."
  (and (match_code "const_double")
       (match_test "op == CONST0_RTX (mode)")))

;; Ttr asks whether x is to be loaded with GETA or something else.  Right
;; now, only a SYMBOL_REF and LABEL_REF can fit for
;; TARGET_BASE_ADDRESSES.
;;
;; Only constant symbolic addresses apply.  With TARGET_BASE_ADDRESSES,
;; we just allow straight LABEL_REF or SYMBOL_REFs with SYMBOL_REF_FLAG
;; set right now; only function addresses and code labels.  If we change
;; to let SYMBOL_REF_FLAG be set on other symbols, we have to check
;; inside CONST expressions.  When TARGET_BASE_ADDRESSES is not in
;; effect, a "raw" constant check together with dadao_constant_address_p
;; is all that's needed; we want all constant addresses to be loaded
;; with GETA then.

(define_constraint "Ttr"
  "@internal"
  (and (not (match_code "const_int,const_double"))
       (match_test "dadao_constant_address_p (op)")
       (ior (match_test "!TARGET_BASE_ADDRESSES")
	    (match_code "label_ref")
	    (and (match_code "symbol_ref")
		 (match_test "SYMBOL_REF_FLAG (op)")))))

;; FIXME: L (or S) is redundant.

(define_constraint "S"
  "@internal"
  (and (match_code "const_int,const_double")
       (match_test "dadao_shiftable_wyde_value (dadao_intval (op))")))

;; FIXME: N (or Ttt) is redundant.

(define_constraint "Ttt"
  "@internal"
  (and (match_code "const_int,const_double")
       (match_test "dadao_shiftable_wyde_value (~dadao_intval (op))")))

(define_address_constraint "U"
  "@internal"
  (match_operand 0 "dadao_address_operand"))

(define_constraint "Yf"
  "@internal"
  (match_operand 0 "frame_pointer_operand"))
