; NOTE: Assertions have been autogenerated by utils/update_llc_test_checks.py
; RUN: llc < %s | FileCheck %s

; Test custom lowering for 32-bit integer multiplication.

target datalayout = "E-m:e-p:32:32-i64:64-a:0:32-n32-S64"
target triple = "dadao"

define i32 @f6(i32 inreg %a) #0 {
; CHECK-LABEL: f6:
; CHECK:       ! %bb.0:
; CHECK-NEXT:    st %fp, [--%sp]
; CHECK-NEXT:    add %sp, 0x8, %fp
; CHECK-NEXT:    sub %sp, 0x8, %sp
; CHECK-NEXT:    sh %r6, 0x1, %r3
; CHECK-NEXT:    sh %r6, 0x3, %r9
; CHECK-NEXT:    sub %r9, %r3, %rv
; CHECK-NEXT:    ld -4[%fp], %pc ! return
; CHECK-NEXT:    add %fp, 0x0, %sp
; CHECK-NEXT:    ld -8[%fp], %fp
  %1 = mul nsw i32 %a, 6
  ret i32 %1
}

define i32 @f7(i32 inreg %a) #0 {
; CHECK-LABEL: f7:
; CHECK:       ! %bb.0:
; CHECK-NEXT:    st %fp, [--%sp]
; CHECK-NEXT:    add %sp, 0x8, %fp
; CHECK-NEXT:    sub %sp, 0x8, %sp
; CHECK-NEXT:    sh %r6, 0x3, %r3
; CHECK-NEXT:    sub %r3, %r6, %rv
; CHECK-NEXT:    ld -4[%fp], %pc ! return
; CHECK-NEXT:    add %fp, 0x0, %sp
; CHECK-NEXT:    ld -8[%fp], %fp
  %1 = mul nsw i32 %a, 7
  ret i32 %1
}

define i32 @f8(i32 inreg %a) #0 {
; CHECK-LABEL: f8:
; CHECK:       ! %bb.0:
; CHECK-NEXT:    st %fp, [--%sp]
; CHECK-NEXT:    add %sp, 0x8, %fp
; CHECK-NEXT:    sub %sp, 0x8, %sp
; CHECK-NEXT:    sh %r6, 0x3, %rv
; CHECK-NEXT:    ld -4[%fp], %pc ! return
; CHECK-NEXT:    add %fp, 0x0, %sp
; CHECK-NEXT:    ld -8[%fp], %fp
  %1 = shl nsw i32 %a, 3
  ret i32 %1
}

define i32 @f9(i32 inreg %a) #0 {
; CHECK-LABEL: f9:
; CHECK:       ! %bb.0:
; CHECK-NEXT:    st %fp, [--%sp]
; CHECK-NEXT:    add %sp, 0x8, %fp
; CHECK-NEXT:    sub %sp, 0x8, %sp
; CHECK-NEXT:    sh %r6, 0x3, %r3
; CHECK-NEXT:    add %r3, %r6, %rv
; CHECK-NEXT:    ld -4[%fp], %pc ! return
; CHECK-NEXT:    add %fp, 0x0, %sp
; CHECK-NEXT:    ld -8[%fp], %fp
  %1 = mul nsw i32 %a, 9
  ret i32 %1
}

define i32 @f10(i32 inreg %a) #0 {
; CHECK-LABEL: f10:
; CHECK:       ! %bb.0:
; CHECK-NEXT:    st %fp, [--%sp]
; CHECK-NEXT:    add %sp, 0x8, %fp
; CHECK-NEXT:    sub %sp, 0x8, %sp
; CHECK-NEXT:    sh %r6, 0x1, %r3
; CHECK-NEXT:    sh %r6, 0x3, %r9
; CHECK-NEXT:    add %r9, %r3, %rv
; CHECK-NEXT:    ld -4[%fp], %pc ! return
; CHECK-NEXT:    add %fp, 0x0, %sp
; CHECK-NEXT:    ld -8[%fp], %fp
  %1 = mul nsw i32 %a, 10
  ret i32 %1
}

define i32 @f1280(i32 inreg %a) #0 {
; CHECK-LABEL: f1280:
; CHECK:       ! %bb.0:
; CHECK-NEXT:    st %fp, [--%sp]
; CHECK-NEXT:    add %sp, 0x8, %fp
; CHECK-NEXT:    sub %sp, 0x8, %sp
; CHECK-NEXT:    sh %r6, 0x8, %r3
; CHECK-NEXT:    sh %r6, 0xa, %r9
; CHECK-NEXT:    add %r9, %r3, %rv
; CHECK-NEXT:    ld -4[%fp], %pc ! return
; CHECK-NEXT:    add %fp, 0x0, %sp
; CHECK-NEXT:    ld -8[%fp], %fp
  %1 = mul nsw i32 %a, 1280
  ret i32 %1
}

define i32 @fm6(i32 inreg %a) #0 {
; CHECK-LABEL: fm6:
; CHECK:       ! %bb.0:
; CHECK-NEXT:    st %fp, [--%sp]
; CHECK-NEXT:    add %sp, 0x8, %fp
; CHECK-NEXT:    sub %sp, 0x8, %sp
; CHECK-NEXT:    sh %r6, 0x3, %r3
; CHECK-NEXT:    sh %r6, 0x1, %r9
; CHECK-NEXT:    sub %r9, %r3, %rv
; CHECK-NEXT:    ld -4[%fp], %pc ! return
; CHECK-NEXT:    add %fp, 0x0, %sp
; CHECK-NEXT:    ld -8[%fp], %fp
  %1 = mul nsw i32 %a, -6
  ret i32 %1
}

define i32 @fm7(i32 inreg %a) #0 {
; CHECK-LABEL: fm7:
; CHECK:       ! %bb.0:
; CHECK-NEXT:    st %fp, [--%sp]
; CHECK-NEXT:    add %sp, 0x8, %fp
; CHECK-NEXT:    sub %sp, 0x8, %sp
; CHECK-NEXT:    sh %r6, 0x3, %r3
; CHECK-NEXT:    sub %r6, %r3, %rv
; CHECK-NEXT:    ld -4[%fp], %pc ! return
; CHECK-NEXT:    add %fp, 0x0, %sp
; CHECK-NEXT:    ld -8[%fp], %fp
  %1 = mul nsw i32 %a, -7
  ret i32 %1
}

define i32 @fm8(i32 inreg %a) #0 {
; CHECK-LABEL: fm8:
; CHECK:       ! %bb.0:
; CHECK-NEXT:    st %fp, [--%sp]
; CHECK-NEXT:    add %sp, 0x8, %fp
; CHECK-NEXT:    sub %sp, 0x8, %sp
; CHECK-NEXT:    sh %r6, 0x3, %r3
; CHECK-NEXT:    sub %r0, %r3, %rv
; CHECK-NEXT:    ld -4[%fp], %pc ! return
; CHECK-NEXT:    add %fp, 0x0, %sp
; CHECK-NEXT:    ld -8[%fp], %fp
  %1 = mul nsw i32 %a, -8
  ret i32 %1
}

define i32 @fm9(i32 inreg %a) #0 {
; CHECK-LABEL: fm9:
; CHECK:       ! %bb.0:
; CHECK-NEXT:    st %fp, [--%sp]
; CHECK-NEXT:    add %sp, 0x8, %fp
; CHECK-NEXT:    sub %sp, 0x8, %sp
; CHECK-NEXT:    sh %r6, 0x3, %r3
; CHECK-NEXT:    add %r6, %r3, %r3
; CHECK-NEXT:    sub %r0, %r3, %rv
; CHECK-NEXT:    ld -4[%fp], %pc ! return
; CHECK-NEXT:    add %fp, 0x0, %sp
; CHECK-NEXT:    ld -8[%fp], %fp
  %1 = mul nsw i32 %a, -9
  ret i32 %1
}

define i32 @fm10(i32 inreg %a) #0 {
; CHECK-LABEL: fm10:
; CHECK:       ! %bb.0:
; CHECK-NEXT:    st %fp, [--%sp]
; CHECK-NEXT:    add %sp, 0x8, %fp
; CHECK-NEXT:    sub %sp, 0x8, %sp
; CHECK-NEXT:    sh %r6, 0x3, %r3
; CHECK-NEXT:    sh %r6, 0x1, %r9
; CHECK-NEXT:    add %r9, %r3, %r3
; CHECK-NEXT:    sub %r0, %r3, %rv
; CHECK-NEXT:    ld -4[%fp], %pc ! return
; CHECK-NEXT:    add %fp, 0x0, %sp
; CHECK-NEXT:    ld -8[%fp], %fp
  %1 = mul nsw i32 %a, -10
  ret i32 %1
}

define i32 @h1(i32 inreg %a) #0 {
; CHECK-LABEL: h1:
; CHECK:       ! %bb.0:
; CHECK-NEXT:    st %fp, [--%sp]
; CHECK-NEXT:    add %sp, 0x8, %fp
; CHECK-NEXT:    sub %sp, 0x8, %sp
; CHECK-NEXT:    mov 0xaaaa0000, %r3
; CHECK-NEXT:    add %pc, 0x10, %rca
; CHECK-NEXT:    st %rca, [--%sp]
; CHECK-NEXT:    bt __mulsi3
; CHECK-NEXT:    or %r3, 0xaaab, %r7
; CHECK-NEXT:    ld -4[%fp], %pc ! return
; CHECK-NEXT:    add %fp, 0x0, %sp
; CHECK-NEXT:    ld -8[%fp], %fp
  %1 = mul i32 %a, -1431655765
  ret i32 %1
}
