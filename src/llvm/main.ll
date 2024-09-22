target triple = "x86_64-pc-linux-gnu"

@.str = constant [14 x i8] c"hello, world\0A\00"

define i32 @main() {
    call i32 @printf(ptr @.str)
    ret i32 0
}

declare i32 @printf()
