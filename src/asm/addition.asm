    section .text
    global main
main:
    push rbp
    mov rbp, rsp

    ; 1+2+3
    ; mov rax, 1
    ; add rax, 2
    ; add rax, 3

    ; 1+2+3
    ; xor rax, rax
    ; add rax, 1
    ; add rax, 2
    ; add rax, 3

    ; 1 2 +
    mov dword [rbp-4], 1
    mov dword [rbp-8], 2
    mov rax, dword [rbp-4]
    add rax, dword [rbp-8]




    pop rbp
    mov rax, 0
    ret

    section .data