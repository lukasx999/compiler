    section .text
    global main
main:
    push rbp
    mov rbp, rsp

    ; 1+2+3
    mov rax, 1
    add rax, 2
    add rax, 3
    sub rax, 1
    add rax, 1
    imul rax, 2

    ; 6 / 2
    mov rax, 10
    mov rdx, 10
    idiv rdx



    pop rbp
    mov rax, 0
    ret

    section .data
