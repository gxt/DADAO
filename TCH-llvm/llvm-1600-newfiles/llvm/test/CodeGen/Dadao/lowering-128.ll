; RUN: llc -march=dadao < %s | FileCheck %s

; Tests that lowering wide registers (128 bits or more) works on Dadao.
; The emitted assembly is not checked, we just do a smoketest.

target datalayout = "E-m:e-p:64:64-i64:64-a:0:64-n64-S128"
target triple = "dadao"

; CHECK-LABEL: add128:
define i128 @add128(i128 %x, i128 %y) {
  %a = add i128 %x, %y
  ret i128 %a
}

; CHECK-LABEL: immshift128:
define i128 @immshift128(i1 %sel) unnamed_addr {
  %a = add i128 0, 340282366920938463463374007431768209319
  %b = add i128 0, 340282366920938463463374607431768209320
  %c = select i1 %sel, i128 %a, i128 %b
  %d = shl i128 %a, 10
  ret i128 %d
}
