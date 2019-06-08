section .text

global exit
global string_length
global print_string
global print_char
global print_newline
global print_uint
global print_int
global string_equals
global parse_uint
global parse_int
global read_word
global string_copy
global print_error

exit: 
    xor rdi, rdi
    mov rax, 60
    syscall

string_length:
	xor rax, rax
	.loop:
		cmp byte[rdi+rax],0
		jz .exit
		inc rax
		jmp .loop
	.exit:	
	ret

print_string:
    xor rax, rax
	call string_length
	mov rsi, rdi
	mov rdi, 1
	mov rdx, rax
	mov rax, 1
	syscall
    ret


print_char:
	xor rax, rax
	push rdi
	mov rax, 1
	mov rdi, 1
	mov rsi, rsp
	mov rdx, 1
	syscall
	pop rdi
	ret

print_newline:
	xor rax, rax
	mov rdi, 10
	call print_char	
	ret


print_uint:
	xor rax, rax
    push rcx
	mov rcx, rsp
	sub rsp, 21
	mov rax, rdi
	mov rdi, 10
	mov byte[rcx], 0
	.loop:
		dec rcx
		xor rdx, rdx
		div rdi
		add rdx,'0'
		mov byte[rcx], dl
		cmp rax, 0
		jnz .loop
	.exit:
		mov rdi, rcx
		call print_string
		add rsp, 21
		pop rcx
    ret


print_int:
	xor rax, rax
	cmp rdi, 0
	jns .print
	push rdi
	.neg: 
		mov rdi,'-'
		call print_char
		pop rdi
		neg rdi
	.print:		
		call print_uint   
	ret

string_equals:
    mov al, byte [rdi]
    cmp al, byte [rsi]
    jne .fail
    inc rdi
    inc rsi
    test al, al
    jnz string_equals
    mov rax, 1
    ret
.fail:
    xor rax, rax
    ret 


read_char:
	push 0	
	xor rax, rax
	xor rdi, rdi
	mov rsi, rsp
	mov rdx, 1
	syscall
	pop rax
    ret 

section .data
word_buffer times 256 db 0

section .text

read_word:
	push r13		;word length
    xor r13, r13
	push r14		;buf size
	mov r14, rdi
    push r15 		;buf adr
    mov r15, rsi
    dec r15
    .space:
	    push rdi
    	call read_char
	    pop rdi
	    cmp al, 32
	    je .space
	    cmp al, 9
	    je .space
	    cmp al, 10
	    je .space 
	    cmp al, 13 
	    je .space
    	cmp al, 0
	    jz .success
    .char:
    	mov byte[r14+r13], al
	    inc r13
	    push rdi
	    call read_char
	    pop rdi
	    cmp al, 32
	    je .success
	    cmp al, 9
	    je .success
	    cmp al, 10
    	je .success 
	    cmp al, 13
	    je .success
	    test al, al
	    jz .success
	    cmp r13, r15
    	je .fail
	    jmp .char
    .success:
    	mov byte[r14+r13], 0
	    mov rax, r14 
	    mov rdx, r13 
		jmp .end
    .fail:
    	xor rax, rax
	.end:
	    pop r15
		pop r14
	    pop r13
	    ret


; rdi points to a string
; returns rax: number, rdx : length
parse_uint:
	xor rax, rax
	xor rcx, rcx	
	xor rdx, rdx
	mov r10, 10
	.loop:
		mov dl, byte[rdi+rcx]
		sub rdx, '0'
		cmp rdx, 9
		ja .end
		cmp rdx, 0
		jb .end
		inc rcx
		imul rax, 10
		add rax, rdx
		jmp .loop
	.end:
		mov rdx, rcx
		ret

; rdi points to a string
; returns rax: number, rdx : length
parse_int:
    xor rax, rax
	xor rdx, rdx
	mov dl, byte[rdi]
	cmp rdx, '-'
	jnz .pos
	inc rdi
	call parse_uint
	neg rax
	inc rdx
	jmp .exit
	.pos:
		call parse_uint
	.exit:
    ret 


string_copy:
	call string_length
	mov rcx, rax	
	cmp rdx, rax
	jb .fail
	mov rcx, rdx
	xchg rsi, rdi
	rep movsb
	mov byte[rdi+rax],0
	jmp .exit
	.fail:
		xor rax, rax
	.exit:
	ret

print_error:
    xor rax, rax
	call string_length
	mov rsi, rdi
	mov rdi, 2
	mov rdx, rax
	mov rax, 1
	syscall
    ret

