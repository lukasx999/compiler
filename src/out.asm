section .text

;--- function ---
foo:
	push rbp
	mov rbp, rsp
	pop rbp
	ret
;--- end ---

;--- function ---
global main
main:
	push rbp
	mov rbp, rsp

;--- echo ---
	mov rax, 1
	mov rdi, 1
	mov rsi, strlit_text_0
	mov rdx, strlit_len_0
	syscall
;--- end ---

;--- echo ---
	mov rax, 1
	mov rdi, 1
	mov rsi, strlit_text_1
	mov rdx, strlit_len_1
	syscall
;--- end ---
	pop rbp
	ret
;--- end ---
section .data
	strlit_text_0: db "foo", 10
	strlit_len_0: db $-strlit_text_0
	strlit_text_1: db "bar", 10
	strlit_len_1: db $-strlit_text_1
