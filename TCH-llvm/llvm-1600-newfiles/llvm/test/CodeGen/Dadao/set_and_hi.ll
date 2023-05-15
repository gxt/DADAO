; RUN: llc < %s | FileCheck %s

; Test matching of and_hi.

target datalayout = "E-m:e-p:64:64-i64:64-a:0:64-n64-S128"
target triple = "dadao"

@x = common global i32 0, align 4

; CHECK-LABEL: setandhi:
; CHECK: mov 0xfffffe4a, %r{{[0-9]+}}
define void @setandhi() #0 {
  store volatile i32 -438, i32* @x, align 4
  ret void
}
