; ModuleID = 'junk.bc'
target datalayout = "e-m:e-p:32:32-f64:32:64-f80:32-n8:16:32-S128"
target triple = "i386-pc-linux-gnu"

@A = common global [10 x i32] zeroinitializer, align 4

; Function Attrs: nounwind
define i32 @array2() #0 {
  %retval= alloca i32, align 4
  %i = alloca i32, align 4
  %j = alloca i32, align 4
  store i32 0, i32* %i, align 4
  br label %BB0

BB0:
  %I3 = load i32* %i, align 4
  %I4 = icmp slt i32 %I3, 10
  br i1 %I4, label %BB1, label %BB3

BB1:
  %I6 = load i32* %i, align 4
  %I7 = getelementptr inbounds [10 x i32]* @A, i32 0, i32 %I6
  store volatile i32 1, i32* %I7, align 4
  br label %BB2

BB2:
  %I9 = load i32* %i, align 4
  %I10 = add nsw i32 %I9, 1
  store i32 %I10, i32* %i, align 4
  br label %BB0

BB3:
  store i32 0, i32* %j, align 4
  br label %BB4

BB4:
  %I13 = load i32* %j, align 4
  %I14 = icmp slt i32 %I13, 10
  br i1 %I14, label %BB5, label %BB9

BB5:
  %I16 = load i32* %j, align 4
  %I17 = getelementptr inbounds [10 x i32]* @A, i32 0, i32 %I16
  %I18 = load volatile i32* %I17, align 4
  %I19 = icmp ne i32 %I18, 0
  br i1 %I19, label %BB6, label %BB7

BB6:
  store i32 1, i32* %retval
  br label %BBKING

BB7:
  br label %BB8

BB8:
  %I23 = load i32* %j, align 4
  %I24 = add nsw i32 %I23, 1
  store i32 %I24, i32* %j, align 4
  br label %BB4

BB9:
  store i32 0, i32* %retval
  br label %BBKING

BBKING:
  %I27 = load i32* %retval
  ret i32 %I27
}

attributes #0 = { nounwind "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "stack-protector-buffer-size"="8" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.ident = !{!0}

!0 = metadata !{metadata !"Ubuntu clang version 3.5.0-4ubuntu2~trusty2 (tags/RELEASE_350/final) (based on LLVM 3.5.0)"}
