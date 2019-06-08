%include "colon.inc"

;(a b -- [a + b])
native "+", plus
	pop rax
	pop rdx
	add rax, rdx
	push rax
	jmp next

;(a b -- [a - b])
native "-", minus 
	pop rdx
	pop rax
	sub rax, rdx
	push rax
	jmp next

;(a b -- [a * b])
native "*", mul
	pop rax
	pop rdx
	imul rdx
	push rax
	jmp next 

;(a b -- [a / b])
native "/", div
	pop rcx
	pop rax
	cqo
	idiv rcx
	push rax
	jmp next 

;(a b -- [a mod b])
native "%", mod
	pop rax
	pop rcx
	cqo
	idiv rcx
	push rdx
	jmp next 

;(a b -- if (a==b) 1 else 0)
native "=", equals 
	pop rdx
	pop rax
	cmp rax, rdx
	sete al
	movzx rax, al
	push rax
	jmp next

;(a b -- if (a<b) 1 else 0)
native "<", less
	pop rdx
	pop rax
	cmp rax, rdx
	setl al
	movzx rax, al
	push rax
	jmp next 

;(a b -- if (a>b) 1 else 0)
native ">", greater
	pop rdx
	pop rax
	cmp rax, rdx
	setg al
	movzx rax, al
	push rax
	jmp next

;(a -- )
native "drop", drop
	pop rax
	jmp next

;(a b -- b a)
native "swap", swap 
	pop rax
	pop rdx
	push rax
	push rdx
	jmp next

;(a -- a a)
native "dup", dup 
	push qword [rsp]
	jmp next

;(a -- !a)
native "not", not 
	pop rax
	test rax, rax
	jz .zero
	push 0
	jmp next
.zero:
	push 1
	jmp next


;(a b -- [a & b])
native "and", and 
	pop rdx
	pop rax
	and rax, rdx
	push rax
	jmp next


;(a b -- [a | b])
native "or", or 
	pop rdx
	pop rax
	or rax, rdx
	push rax
	jmp next

;(a b -- [a && b])
native "land", land 
	pop rdx
	pop rax
	test rax, rax
	jz .zero
	push rdx
	jmp next
.zero:
	push 0
	jmp next

;(wtf)
native "lor", lor
	pop rdx
	pop rax
	test rax, rax
	jnz .not_zero
	push rdx
	jmp next
.not_zero:
	push 1
	jmp next
  
native ".", dot
    pop rdi
    call print_int
    jmp next
  
native ">r", to_r
    pop rax
	sub rstack, 8
	mov qword [rstack], rax    
	jmp next
  
native "r>", from_r
	mov rax, qword [rstack]
	push rax
	add rstack, 8    
	jmp next
  
native "r@", cp_from_r
    push qword [rstack]
	jmp next

;( addr -- value )
native "@", read_value
	pop rax
	push qword [rax]
	jmp next    


;( val addr -- )
native "!", write_value
    pop rax
	pop rdx
	mov [rax], rdx
	jmp next

;( addr -- char )
native "c@", read_byte
	pop rax
	movzx rdx, byte [rax]
	push rdx
	jmp next

;( char addr -- )
native "c!", write_byte
	pop rax
	pop rdx
	mov [rax], dl    
	jmp next

;( xt -- )
native "execute", exec
	pop rax
	mov w, rax	
	jmp [w]

;( xt -- )
;(!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1)
native "forth_dp", forth_dp
	
;( xt -- )
;(!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1)
native "branch", branch
	
;( xt -- )
;(!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1)
native "0branch", 0branch
	
native "exit", exit
	mov pc, [rstack]
	add rstack, 8    
	jmp next

native "lit", lit
	push qword [pc]
	add pc, 8	
	jmp next

;( xt -- )
;(!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1)
native "forth_sp", forth_sp
	
;( xt -- )
;(!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1)
native "forth_stack-base", forth_stack_base
	
;( call_num a1 a2 a3 a4 a5 a6 -- new_rax new_rdx)
native "syscall", syscall
	pop r9
	pop r8
	pop r10
	pop rdx
	pop rsi
	pop rdi
	pop rax
	syscall
	push rax
	push rdx    
	jmp next

;( xt -- )
;(!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!1)
native ".S", show_stack
	

native "bye", bye
	mov rax, 60
	xor rdi, rdi
	syscall

native "docol", docol 
	sub rstack, 8
	mov [rstack], pc
	add w, 8
	mov pc, w
	jmp next

next: 
	mov w, [pc]
	add pc, 8
	jmp [w]

