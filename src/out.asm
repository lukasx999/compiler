section .text

;--- function ---
global main
main:
	push rbp
	mov rbp, rsp
	pop rbp
	ret
;--- end ---

;--- function ---
foo:
	push rbp
	mov rbp, rsp
	pop rbp
	ret
;--- end ---

;--- function ---
bar:
	push rbp
	mov rbp, rsp
	pop rbp
	ret
;--- end ---
section .data
