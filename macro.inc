%define _lw 0
%define pc r15
%define w r14
%define rstack r13
%define stack r12

%macro create_link 0
  %%_lw: dq _lw        
  %define _lw %%_lw
%endmacro

%macro native 3
  section .data
    w_ %+ %2:            
	    create_link       
	    db %1, 0           
      db %3             
	  xt_ %+ %2:         
      dq i_ %+ %2     
  section .text
	 i_ %+ %2:         
%endmacro

%macro native 2
  native %1, %2, 0
%endmacro

%macro colon 3
  section .data
	 w_ %+ %2:
	  create_link
	  db %1, 0
    db %3                
	 xt_ %+ %2:
	  dq i_docol          
%endmacro

%macro colon 2
  colon %1, %2, 0
%endmacro

%macro branch0 1
  dq xt_branch0
  dq %1
%endmacro

%macro branch 1
  dq xt_branch
  dq %1
%endmacro

%macro const 2
%defstr %%__cnst_str %1
native %%__cnst_str, %1
    push qword %2
    jmp next
%endmacro

%macro rpush 1
  sub rstack, 8
  mov qword [rstack], %1
%endmacro

%macro rpop 1
  mov %1, qword [rstack]
  add rstack, 8
%endmacro

