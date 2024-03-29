; RUN: llc -o %t.bc -filetype=obj -mtriple=dadao %s
; RUN: llvm-objdump -d -S %t.bc | FileCheck %s

;; Ensure that Dadao can be compiled using llc and then objdumped to
;; assembly. This is a smoke test to exercise the basics of the MC
;; implementation in Dadao.

; CHECK-LABEL: smoketest
; CHECK: st %fp, [--%sp]
define i32 @smoketest(i32 %x, i32 %y)  {
  %z = add i32 %x, %y
  ret i32 %z
}
