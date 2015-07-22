; ModuleID = 'test.bc'
target datalayout = "e-m:e-p:32:32-f64:32:64-f80:32-n8:16:32-S128"
target triple = "i386-pc-linux-gnu"

; Function Attrs: nounwind
define i32 @straightline(i32 %a) #0 {
  %1 = alloca i32, align 4
  %2 = alloca i32, align 4
  %ptr = alloca i32*, align 4
  %tmp = alloca i32, align 4
  store i32 %a, i32* %2, align 4
  %3 = load i32* %2, align 4
  %4 = add nsw i32 %3, 5
  store i32 %4, i32* %tmp, align 4
  store i32* %tmp, i32** %ptr, align 4
  %5 = load i32** %ptr, align 4
  %6 = load i32* %5, align 4
  %7 = icmp sgt i32 %6, 10
  br i1 %7, label %8, label %9

; <label>:8                                       ; preds = %0
  store i32 5, i32* %1
  br label %10

; <label>:9                                       ; preds = %0
  store i32 4, i32* %1
  br label %10

; <label>:10                                      ; preds = %9, %8
  %11 = load i32* %1
  ret i32 %11
}

attributes #0 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"Ubuntu clang version 3.5.0-4ubuntu2~trusty2 (tags/RELEASE_350/final) (based on LLVM 3.5.0)"}
