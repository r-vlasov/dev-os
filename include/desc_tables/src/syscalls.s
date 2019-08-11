 // Writing the messages
global 	__syscall_128

extern 	syscall_write_handler


__syscall_128:
	cli;
	jmp syscall_write;





syscall_write:
	pusha
	mov	ax, ds
	push	eax
	mov	ax, 0x10
	mov	ds, ax
	mov	fs, ax
	mov 	gs, ax
	call	syscall_write_handler

	pop 	eax
	mov 	ds, ax
	mov 	es, ax
	mov 	fs, ax
	mov 	gs, ax
	
	popa
	iret	
