
    %macro PRINT 1
        mov rdi, fmt
        mov rsi, %1
        call printf
    %endmacro

    extern printf

    section .text
    global main
main:
    push rbp
    mov rbp, rsp


    mov DWORD [rbp-4], 45
    mov DWORD [rbp-8], 123

    PRINT [rbp-8]

    pop rbp
    mov rax, 0
    ret

    section .data
fmt: db "this is the value: %d", 10, 0
