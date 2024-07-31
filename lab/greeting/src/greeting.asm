; include the library defaults, and the
; library main include:
include '%HT_CORE%/ht_defaults.inc'
include '%HT_CORE%/ht.inc'

public _start
_start:
	; every HeavyThing program needs to start with a call to initialise it
	call		ht$init

	mov	rdi, 	.greeting
	call		string$to_stdoutln

	mov	eax, 	syscall_exit
	xor	edi, 	edi
	syscall

cleartext .greeting, 'Greetings, OFWorld'

; include the global data segment:
include '%HT_CORE%/ht_data.inc'
