extern puts
extern printf
extern exit

section   .text


say_hello:
    push rbp ; create stack frame

    mov rdi, message
    call puts

    pop rbp ; pop stack frame
    mov rax, 0
    ret


global main
main:
    push rbp
    .label:

    ; write(fd, buf, bufsize)
    mov rax, 1
    mov rdi, 1 ; fd
    mov rsi, message ; buf
    mov rdx, 13 ; bufsize
    syscall

    call say_hello

    push 5
    pop rdx
    inc rdx
    add rdx, 5
    imul rdx, 5

    mov rdx, 1
    add rdx, 2

    mov rdi, formatstring
    mov rsi, rdx
    mov rax, 0
    call printf


    ;mov rdi, 5
    ;mov rsi, 3
    ;cmp rdi, rsi
    ;je .label



    pop rbp
    mov rdi, 0
    call exit

section   .data
message: db "Hello, World", 10, 0 ; chr(10) == '\n'
formatstring: db "this is my number: %d", 10, 0 ; chr(10) == '\n'
