

    extern foo


    section .text

global main
main:
    push rbp
    mov rbp, rsp

    mov rax, 1
    mov rdi, 1
    mov rsi, str
    mov rdx, len
    syscall

    call foo

    mov rax, 0
    pop rbp
    ret



    section .data
str:
    db "Hello, World", 10, 0
len:
    db $-str
