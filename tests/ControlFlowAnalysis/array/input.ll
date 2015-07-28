; ModuleID = 'test.bc'
target datalayout = "e-m:e-p:32:32-f64:32:64-f80:32-n8:16:32-S128"
target triple = "i386-pc-linux-gnu"

@A = common global [10 x i32] zeroinitializer, align 4

; Function Attrs: nounwind
define i32 @straightline() #0 {
  store volatile i32 1, i32* getelementptr inbounds ([10 x i32]* @A, i32 0, i32 5), align 4
  %array_load = load volatile i32* getelementptr inbounds ([10 x i32]* @A, i32 0, i32 1), align 4
  %cond = icmp ne i32 %array_load, 0
  br i1 %cond, label %if.then, label %if.else

if.then:
  br label %returnpoint

if.else:
  br label %returnpoint 

returnpoint:			  
  %retval = phi i32 [ 0, %if.else ], [ 1, %if.then ]
  ret i32 %retval 
}

attributes #0 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"Ubuntu clang version 3.5.0-4ubuntu2~trusty2 (tags/RELEASE_350/final) (based on LLVM 3.5.0)"}
