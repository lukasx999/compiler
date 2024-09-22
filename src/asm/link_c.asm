global    main
extern puts

section   .text
main:
    call puts
    ret



section   .data
message:
    db "Hello, World", 10 ; chr(10) == '\n'
