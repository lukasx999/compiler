    section .text
    global main
main:
    push rbp
    mov rbp, rsp


    ; foo
    mov rax, 1
    mov rdi, 1
    mov rsi, msg
    mov rdx, len
    syscall

    ; bar
    mov rax, 1
    mov rdi, 1
    mov rsi, msg2
    mov rdx, len2
    syscall


    pop rbp
    mov rax, 0
    ret



    section .data

msg: db "Foo", 10
len: db $-msg

msg2: db "Bar", 10
len2: db $-msg2
