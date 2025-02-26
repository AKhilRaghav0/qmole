; ModuleID = '<stdin>'

%FunTy = type i32 ()

@bar = external global i32

@foo1 = alias i32* @bar
@foo2 = alias i32* @bar
@foo3 = alias i32* @foo2
@bar_f = alias weak %FunTy* @foo_f
@bar_ff = alias %FunTy* @bar_f
@bar_i = alias internal i32* @bar
@A = alias bitcast (i32* @bar to i64*)

declare i32 @foo_f()

define i32 @test() {
entry:
  %tmp = load i32* @foo1
  %tmp1 = load i32* @foo2
  %tmp0 = load i32* @bar_i
  %tmp2 = call i32 @foo_f()
  %tmp3 = add i32 %tmp, %tmp2
  %tmp4 = call i32 @bar_f()
  %tmp5 = add i32 %tmp3, %tmp4
  %tmp6 = add i32 %tmp1, %tmp5
  %tmp7 = add i32 %tmp6, %tmp0
  ret i32 %tmp7
}
