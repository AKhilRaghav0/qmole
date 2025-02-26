; ModuleID = '<stdin>'

declare i32 @atoi(i8*)

define i64 @fib(i64 %n) {
  %1 = icmp ult i64 %n, 2
  br i1 %1, label %BaseCase, label %RecurseCase

BaseCase:                                         ; preds = %0
  ret i64 1

RecurseCase:                                      ; preds = %0
  %n2 = sub i64 %n, 2
  %n1 = sub i64 %n, 1
  %f2 = call i64 @fib(i64 %n2)
  %f1 = call i64 @fib(i64 %n1)
  %result = add i64 %f2, %f1
  ret i64 %result
}

define i64 @realmain(i32 %argc, i8** %argv) {
; <label>:0
  %1 = icmp eq i32 %argc, 2
  br i1 %1, label %HasArg, label %Continue

HasArg:                                           ; preds = %0
  %n1 = add i32 1, 1
  br label %Continue

Continue:                                         ; preds = %HasArg, %0
  %n = phi i32 [ %n1, %HasArg ], [ 1, %0 ]
  %N = sext i32 %n to i64
  %F = call i64 @fib(i64 %N)
  ret i64 %F
}

define i64 @trampoline(i64 %n, i64 (i64)* %fibfunc) {
  %F = call i64 %fibfunc(i64 %n)
  ret i64 %F
}

define i32 @main() {
  %Result = call i64 @trampoline(i64 10, i64 (i64)* @fib)
  %Result.upgrd.1 = trunc i64 %Result to i32
  ret i32 %Result.upgrd.1
}
