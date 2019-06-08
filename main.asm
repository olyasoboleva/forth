%define pc r15
%define w r14
%define rstack r13

%include "lib.asm"
%include "words.inc"
global _start

section .bss
resq 1023
rstack_start: resq 1

section .text
_start: 
	mov rstack, rstack_start
	mov pc, program
	push 1
	push 2
	push 3
	push 4
	push 0
	push 2
	push 1
	push xt_plus
	jmp next

program:
	dq xt_exec, xt_dot, xt_bye
