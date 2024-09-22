    extern exit

    section .text

    global main


function:
    push rbp

    pop rbp
    ret 5

main:

    call function

    mov rdi, 0
    call exit
