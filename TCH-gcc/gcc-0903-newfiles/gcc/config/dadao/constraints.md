;; vim: filetype=lisp

;; DADAO constraints
;; Copyright (C) 2020-2033 by Guan Xuetao (AT) Peking Univ.
;;
;; Contributed by:
;;   2020:
;;	Guan Xuetao <gxt@pku.edu.cn>

;; Register class constraints
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

;; Special register constraints
(define_constraint "Sf"
	"@internal"
	(match_operand 0 "frame_pointer_operand"))

;; Immediate constraints
;; s: single 6-bit data
;; d: double 6-bit data
;; t: triple 6-bit data
;; z: zero
;; w: wyde-size data
(define_constraint "Id"
	"A 12-bit signed integer"
	(and (match_code "const_int")
	     (match_test "IN_RANGE (ival, -0x800, 0x7FF)")))

(define_constraint "It"
	"A 18-bit signed integer"
	(and (match_code "const_int")
	     (match_test "IN_RANGE (ival, -0x40000, 0x3FFFF)")))

(define_constraint "Iw"
	"Shiftable wyde integer"
	(and (match_code "const_int")
	     (match_test "dadao_shiftable_wyde_value (ival)")))

(define_constraint "Iz"
	"The value 0."
	(and (match_code "const_int")
	     (match_test "ival == 0")))

;; Negative immediate constraints
(define_constraint "Nd"
	"Negative 12-bit integer"
	(and (match_code "const_int")
	     (match_test "IN_RANGE (ival, -4095, 0)")))


;; Unsigned immediate constraints (DONT use U as prefix, since constraint type ERROR)
(define_constraint "Js"
	"A 6-bit unsigned integer"
	(and (match_code "const_int")
	     (match_test "IN_RANGE (ival, 0, 0x1F)")))

(define_constraint "Jd"
	"A 12-bit unsigned integer"
	(and (match_code "const_int")
	     (match_test "IN_RANGE (ival, 0, 4095)")))

;; Floating Point immediate constraints
;; FIXME: Iz (or Gz) is redundant.
(define_constraint "Gz"
	"Floating-point zero."
	(and (match_code "const_double")
	     (match_test "op == CONST0_RTX (mode)")))

;; Address constraints
(define_constraint "Ai"
	"@internal"
	(and (not (match_code "const_int,const_double"))
	          (match_test "dadao_constant_address_p (op)")))

(define_address_constraint "Au"
	"@internal"
	(match_operand 0 "dadao_address_operand"))

