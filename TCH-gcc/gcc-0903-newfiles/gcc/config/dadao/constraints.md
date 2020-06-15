;; vim: filetype=lisp

;; DADAO constraints
;; Copyright (C) 2020-2033 by Guan Xuetao (AT) Peking Univ.
;;
;; Contributed by:
;;   2020:
;;	Guan Xuetao <gxt@pku.edu.cn>

;; Register class constrains
(define_register_constraint "Rg" "GENERAL_REGS"
	"General registers")

(define_register_constraint "Rp" "POINTER_REGS"
	"Pointer registers")

(define_register_constraint "Rf" "FLOATING_REGS"
	"Floating-point registers")

(define_register_constraint "Rv" "VECTOR_REGS"
	"Vector registers")

(define_register_constraint "Rs" "SPECIAL_REGS"
	"Special registers")

;; Immediate constrains
(define_constraint "Id"
	"A 12-bit signed integer"
	(and (match_code "const_int")
	     (match_test "IN_RANGE (ival, -0x800, 0x7FF)")))

(define_constraint "It"
	"A 18-bit signed integer"
	(and (match_code "const_int")
	     (match_test "IN_RANGE (ival, -0x40000, 0x3FFFF)")))

(define_constraint "Iz"
	"The value 0."
	(and (match_code "const_int")
	     (match_test "ival == 0")))

;; Negative immediate constrains
(define_constraint "Nd"
	"Negative 12-bit integer"
	(and (match_code "const_int")
	     (match_test "IN_RANGE (ival, -4095, 0)")))


;; Unsigned immediate constrains
(define_constraint "Us"
	"A 6-bit unsigned integer"
	(and (match_code "const_int")
	     (match_test "IN_RANGE (ival, 0, 0x1F)")))

(define_constraint "Ud"
	"A 12-bit unsigned integer"
	(and (match_code "const_int")
	     (match_test "IN_RANGE (ival, 0, 4095)")))

(define_constraint "Uw"
	"Shiftable wyde integer"
	(and (match_code "const_int")
	     (match_test "dadao_shiftable_wyde_value (ival)")))

;; Special register constrains
(define_register_constraint "Sy" "REMAINDER_REG"
	"@internal")

(define_register_constraint "Sz" "HIMULT_REG"
	"@internal")

;; Unhandled
(define_constraint "Tti"
  "A 8-bit unsigned integer"
  (and (match_code "const_int")
       (match_test "IN_RANGE (ival, 0, 255)")))

(define_constraint "J"
  "A 16-bit unsigned integer."
  (and (match_code "const_int")
       (match_test "IN_RANGE (ival, 0, 65535)")))

(define_constraint "O"
  "The value 3, 5, 9, or 17."
  (and (match_code "const_int")
       (ior (match_test "ival == 3")
	    (match_test "ival == 5")
	    (match_test "ival == 9")
	    (match_test "ival == 17"))))

;; FIXME: Iz (or G) is redundant.
(define_constraint "G"
  "Floating-point zero."
  (and (match_code "const_double")
       (match_test "op == CONST0_RTX (mode)")))

(define_constraint "Ttr"
  "@internal"
  (and (not (match_code "const_int,const_double"))
       (match_test "dadao_constant_address_p (op)")))

(define_address_constraint "Au"
  "@internal"
  (match_operand 0 "dadao_address_operand"))

(define_constraint "Yf"
  "@internal"
  (match_operand 0 "frame_pointer_operand"))
