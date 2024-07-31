include     "header.inc"

public main

section 	'.text'
extrn       arg_test

main:
    pop     
    push    rbp             ; align stack
    push    .arg1           ; 2nd parameter (pointer to array)
    xor			rax, rax
    call 	arg_test

	  mov			eax, 	syscall_exit
	  xor			edi, 	edi
	  syscall

section 	'.data'
    .arg1: 	db		"arg1", 		0
    .argv:  dq    .arg1,      0