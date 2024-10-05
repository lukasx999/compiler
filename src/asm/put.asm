    section .text

    global main
main:


    mov rax, 1
    mov rdi, 1
    mov rsi, msg
    mov rdx, 4
    syscall



    mov rax, 0
    ret


    section .data
msg: db "Foo", 10
