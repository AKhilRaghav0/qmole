; ModuleID = '<stdin>'

%0 = type { i32, i32 }

@somestr = constant [11 x i8] c"hello world"
@array = constant [2 x i32] [i32 12, i32 52]
@0 = constant %0 { i32 4, i32 3 }

define [2 x i32]* @testfunction(i32 %i0, i32 %j0) {
  ret [2 x i32]* @array
}

define i8* @otherfunc(i32, double) {
  %somestr = getelementptr [11 x i8]* @somestr, i64 0, i64 0
  ret i8* %somestr
}

define i8* @yetanotherfunc(i32, double) {
  ret i8* null
}

define i32 @negativeUnsigned() {
  ret i32 -1
}

define i32 @largeSigned() {
  ret i32 -394967296
}
