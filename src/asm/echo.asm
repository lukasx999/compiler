    section .text
    global main
main:
    push rbp
    mov rbp, rsp


    mov rax, 1
    mov rdi, 1
    mov rsi, msg
    mov rdx, len
    syscall


    pop rbp
    mov rax, 0
    ret



    section .data

msg: db "Hello, World"
len: db $-msg
