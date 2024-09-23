




    section .text

child:
    ; write(1, msg, len)
    mov rax, 1
    mov rdi, 1
    mov rsi, str_child
    mov rdx, child_len
    syscall
    jmp after

parent:
    ; write(1, msg, len)
    mov rax, 1
    mov rdi, 1
    mov rsi, str_parent
    mov rdx, parent_len
    syscall
    jmp after


    global main
main:


    ; fork()
    mov rax, 57
    syscall

    ; if rax == 0; goto child else goto parent
    cmp rax, 0
    je child
    jmp parent

after:

    ; exit(0)
    mov rax, 0
    ret

    section .data

str_child: db "child", 10, 0
child_len: db $-str_child
str_parent: db "parent", 10, 0
parent_len: db $-str_parent
