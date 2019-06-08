global find_word
global cfa
extern string_equals

section .text
find_word:
	xor rax, rax
.loop:
	test rsi, rsi
	jz .end
	push rdi
	push rsi
	add rsi, 8
	call string_equals
	pop rsi
	pop rdi
	test rax, rax
	jnz .end
	mov rsi, [rsi]
	jmp .loop	
.end:
	mov rax, rsi
	ret

cfa:
	add rdi, 8
.loop:
	mov rax, [rdi]
	inc rdi
	test al, 0xFF
	jnz .loop 
mov rax, rdi
ret

