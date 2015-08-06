; ModuleID = 'input.bc'
target datalayout = "e-m:e-p:32:32-f64:32:64-f80:32-n8:16:32-S128"
target triple = "i386-pc-linux-gnu"

; Function Attrs: nounwind
define i32 @long_chain(i32 %control, i32 %data) #0 {
  %con1 = add i32 1, %control
  %dat1 = add i32 1, %data
  br label %uncond1

uncond1:                                          ; preds = %0
  %con2 = add i32 1, %con1
  %dat2 = add i32 1, %dat1
  br label %uncond2

uncond2:                                          ; preds = %uncond1
  %con3 = add i32 1, %con2
  %dat3 = add i32 1, %dat2
  br label %uncond3

uncond3:                                          ; preds = %uncond2
  %con4 = add i32 1, %con3
  %dat4 = add i32 1, %dat3
  br label %cond1

cond1:                                            ; preds = %uncond3
  %conditional = icmp eq i32 %con4, 5
  br i1 %conditional, label %if.then, label %if.else

if.then:                                          ; preds = %cond1
  ret i32 %dat4

if.else:                                          ; preds = %cond1
  %res = mul i32 %dat4, 2
  ret i32 %res
}

attributes #0 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"Ubuntu clang version 3.5.0-4ubuntu2~trusty2 (tags/RELEASE_350/final) (based on LLVM 3.5.0)"}
