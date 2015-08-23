; ModuleID = 'input.stitchedup.bc'
target datalayout = "e-m:e-p:32:32-f64:32:64-f80:32-n8:16:32-S128"
target triple = "i386-pc-linux-gnu"

; Function Attrs: nounwind
define i32 @conditional_branch(i32 %a, i32 %b, i32 %c) #0 {
  %cmp = icmp eq i32 %a, 1
  br i1 %cmp, label %if.then, label %if.else

if.then:                                          ; preds = %0
  ret i32 undef

if.else:                                          ; preds = %0
  ret i32 undef
}

attributes #0 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"Ubuntu clang version 3.5.0-4ubuntu2~trusty2 (tags/RELEASE_350/final) (based on LLVM 3.5.0)"}
