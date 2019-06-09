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

;(a b -- [a || b])
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
	cmp     rsp, [stackHead]
	je      .error
	pop     rdi
	call    print_int
	call    print_newline
	jmp     .f
	.error:
	mov     rdi, underflow
	call    print_string
	call    print_newline
	.f:
	jmp     next
  
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

native "execute", exec
	pop rax
	mov w, rax	
	jmp [w]

native "branch", branch
	mov pc, [pc]
    jmp next

	
native "branch0", branch0
	pop rax
    test rax, rax
    jnz .skip
    mov pc, [pc]
    jmp next
    .skip:
    add pc, 8
    jmp next
	
native "exit", exit
	mov pc, [rstack]
	add rstack, 8    
	jmp next

native "lit", lit
	push qword [pc]
	add pc, 8	
	jmp next

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

native ".S", show_stack
	mov rdx, rsp
    .loop:
    mov rdi, [rdx]
    push rdx
    call print_int
    call print_newline
    pop rdx
    add rdx, 8
    cmp rdx, [stack_start]
    jl .loop
    jmp next
	

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

native 'execute', execute
	pop rax
	mov w, rax
	jmp [rax]

native "word", word
	pop rdi
	call read_word
	push rdx
	jmp next

native "prints", prints
	pop rdi
	call print_string
	jmp next

native "printc", printc
	pop rdx
	pop rsi
	mov rdi, 1
	mov rax, 1
	syscall
	jmp next

native "count", count
	pop rdi
	call string_length
	push rax
	jmp next
	
native "number", number
	call read_word
	mov rax, rdi
	call parse_int
	push rax
	jmp next

native "find", find
	pop rdi               
	call find_word
	push rax
	jmp next

native "error", error
	mov  rdi, unknown_word
	call print_string
	call print_newline
	jmp next

native "isInstant", isInstant
	pop rax
	push rax
	xor rdx, rdx
	mov dl, byte[rax - 1]
	push rdx
	jmp next

native "parse_int", parse_int
	pop rdi
	call parse_int         
	push rax              
	push rdx
	jmp next

native "pushmode", pushmode
	xor eax, eax
	mov rax, [mode]
	push rax
	jmp next

native "pushmode_address", pushmode_address
	xor eax, eax
	mov rax, mode
	push rax
	jmp next

native ",", comma
	mov rax, [here]
	pop qword [rax]
	add qword [here], 8
	jmp next

native "isbranch", isbranch
	pop rax
	push rax
	cmp byte[rax - 1], 2
	jne .continue
	mov byte[was_branch], 1
	.continue:
	jmp next

native "init", init
	mov rstack, rstack_start
	mov stack, stack_start
	xor eax, eax
	mov rax, [in_fd]
	rpush rax
	xor eax, eax
	pop rax
	mov [in_fd], rax
	mov qword[mode], 0
	mov pc, xt_run
	cmp qword [stack_start], 0
	je  .first
	mov rsp, [stack_start]
	jmp next
	.first:
	mov [stack_start], rsp
	jmp next

native "buffer", buffer
	push qword input_buf
	jmp next

native "savenum", savenum
	pop rax
	mov [here], rax
	xor eax, eax
	mov rax, here
	add rax, 8
	mov qword[here], rax
	jmp next

native "push_lw", push_lastword
	push last_word
	jmp next

native "unsetbranch", unsetbranch
	mov byte[was_branch], 0
	jmp next

native "wasbranch", wasbranch
	xor rdx, rdx
	mov dl, byte[was_branch]
	push rdx
	jmp next

native "create", create
	mov rcx, [last_word]
	mov rsi, [here]
	mov [rsi], rcx
	mov [last_word], rsi
	add rsi, 8
	pop rdi
	push rsi
	call string_copy
	pop rsi
	push rsi
	mov rdi, rsi
	call string_length
	pop rsi
	add rsi, rax
	inc rsi              
	pop rax
	mov [rsi], al
	inc rsi
	mov [here], rsi
	jmp next

native "cfa", cfa
	pop rsi
    add rsi, 9
    .loop:
    mov al, [rsi]
    test al, al
    jz .end
    inc rsi
    jmp .loop
    .end:
    add rsi, 2
    push rsi
    jmp next


colon ":", colon
	.restart:
	dq xt_buffer
	dq xt_word
	branch0 .restart
	dq xt_lit, 0         
	dq xt_buffer
	dq xt_create
	dq xt_lit, 1, xt_pushmode_address
	dq xt_write_value
	dq xt_lit, i_docol
	dq xt_comma
	dq xt_exit

colon ";", semicolon, 1
	dq xt_lit, 0, xt_pushmode_address
	dq xt_write_value
	dq xt_lit, xt_exit
	dq xt_comma
	dq xt_exit

colon "'", tick, 1
	dq xt_buffer
	dq xt_word
	dq xt_drop
	dq xt_buffer
	dq xt_find, xt_dup
	branch0 .no_such_word
	dq xt_cfa
	dq xt_pushmode
	branch0 .interpret
	dq xt_lit, xt_lit
	dq xt_comma
	dq xt_comma
	.interpret:
	dq xt_exit
	.no_such_word:
	dq xt_error
	dq xt_exit
;-----------------------------------------




