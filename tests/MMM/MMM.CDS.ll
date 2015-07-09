; ModuleID = 'MMM.stitchedup.bc'
target datalayout = "e-m:e-p:32:32-f64:32:64-f80:32-n8:16:32-S128"
target triple = "i386-pc-linux-gnu"

; Function Attrs: nounwind
define void @MMM([100 x float]* %A, [100 x float]* %B, [100 x float]* %C) #0 {
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  %k = alloca i32, align 4
  br label %1

; <label>:1                                       ; preds = %17, %0
  %2 = load i32* %i, align 4
  %3 = icmp slt i32 %2, 100
  br i1 %3, label %4, label %18

; <label>:4                                       ; preds = %1
  br label %5

; <label>:5                                       ; preds = %15, %4
  %6 = load i32* %j, align 4
  %7 = icmp slt i32 %6, 100
  br i1 %7, label %8, label %16

; <label>:8                                       ; preds = %5
  br label %9

; <label>:9                                       ; preds = %13, %8
  %10 = load i32* %k, align 4
  %11 = icmp slt i32 %10, 100
  br i1 %11, label %12, label %14

; <label>:12                                      ; preds = %9
  br label %13

; <label>:13                                      ; preds = %12
  br label %9

; <label>:14                                      ; preds = %9
  br label %15

; <label>:15                                      ; preds = %14
  br label %5

; <label>:16                                      ; preds = %5
  br label %17

; <label>:17                                      ; preds = %16
  br label %1

; <label>:18                                      ; preds = %1
  ret void
}

attributes #0 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"Ubuntu clang version 3.5.0-4ubuntu2~trusty2 (tags/RELEASE_350/final) (based on LLVM 3.5.0)"}
