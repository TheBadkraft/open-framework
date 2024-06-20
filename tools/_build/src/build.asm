include		'header.inc'

public main

include		'libc.inc'
extrn		build

main:
   	mov			rdi, .title
    mov			rax, 0			; returns str len
 	call 	printf
	call	build

	mov			eax, 	syscall_exit
	xor			edi, 	edi
	syscall

section 	'.data'
    .title: 	db		"OF Build Tool", 		10,		0
