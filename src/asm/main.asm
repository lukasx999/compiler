    global    _start

    section   .text
_start:

    # .write:
    ; write(fd, buf, bufsize)
    mov       rax, 1
    mov       rdi, 1 ; fd
    mov       rsi, message ; buf
    mov       rdx, 13 ; bufsize
    syscall
    ; push 5
    ; push 6
    # mov 5, rdi
    # mov 6, rdx
    ; .label:
    ; jmp .label
    ; exit(code)
    mov rax, 60
    mov rdi, 0 ; exit code
    syscall

    section   .data
message:
    db "Hello, World", 10 ; chr(10) == '\n'
