    section .text
global foo
foo:
    push rbp
    mov rbp, rsp

    pop rbp
    mov rax, 45
    ret
