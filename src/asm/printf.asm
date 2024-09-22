    extern printf
    extern exit

section .text

global main
main:

    mov rax, 0
    mov rdi, fmt
    mov rsi, 45
    call printf

    mov rdi, 0
    call exit

section .data

fmt: db "This is the number: %d", 10, 0
