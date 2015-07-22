; ModuleID = 'input.bc'
target datalayout = "e-m:e-p:32:32-f64:32:64-f80:32-n8:16:32-S128"
target triple = "i386-pc-linux-gnu"

; Function Attrs: nounwind
define i32 @unconditional_branch(i32 %a) #0 {
  %1 = alloca i32, align 4
  %tmp = alloca i32, align 4
  store i32 %a, i32* %1, align 4
  %2 = load i32* %1, align 4
  %3 = add nsw i32 %2, 5
  store i32 %3, i32* %tmp, align 4
  %4 = load i32* %tmp, align 4
  br label %branched

branched:
  ret i32 0 
}

attributes #0 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"Ubuntu clang version 3.5.0-4ubuntu2~trusty2 (tags/RELEASE_350/final) (based on LLVM 3.5.0)"}
