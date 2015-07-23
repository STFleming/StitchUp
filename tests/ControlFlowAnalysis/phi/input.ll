; ModuleID = 'input.bc'
target datalayout = "e-m:e-p:32:32-f64:32:64-f80:32-n8:16:32-S128"
target triple = "i386-pc-linux-gnu"

; Function Attrs: nounwind
define i32 @phi_check(i32 %control, i32 %data) #0 {
  %con1 = add i32 1, %control
  %conditional = icmp eq i32 %con1,10 
  br i1 %conditional, label %if.then, label %if.else

if.then:                                          ; preds = %cond1
  br label %phi_location

if.else:                                          ; preds = %cond1
  br label %phi_location

phi_location:
   %con2 = phi i32 [ 0, %if.then ], [ 1, %if.else ]
   %final_cond = icmp eq i32 %control, 0
   br i1 %final_cond, label %return1, label %return2

return1:
  ret i32 0

return2:
  ret i32 1

}

attributes #0 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"Ubuntu clang version 3.5.0-4ubuntu2~trusty2 (tags/RELEASE_350/final) (based on LLVM 3.5.0)"}
