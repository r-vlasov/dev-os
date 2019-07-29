;	 Interrupt Service Routines(ISR) are used to save thread state,
;	     execute interrupt handler and resumes thread execution



;Exception # 	Description 				Error Code?
;0		Division By Zero Exception 		No
;1 		Debug Exception 			No
;2 		Non Maskable Interrupt Exception 	No
;3 		Breakpoint Exception 			No
;4 		Into Detected Overflow Exception 	No
;5 		Out of Bounds Exception 		No
;6 		Invalid Opcode Exception 		No
;7 		No Coprocessor Exception 		No
;8 		Double Fault Exception 			Yes
;9 		Coprocessor Segment Overrun Exception 	No
;10 		Bad TSS Exception 			Yes
;11 		Segment Not Present Exception 		Yes
;12 		Stack Fault Exception 			Yes
;13 		General Protection Fault Exception 	Yes
;14 		Page Fault Exception 			Yes
;15 		Unknown Interrupt Exception 		No
;16 		Coprocessor Fault Exception 		No
;17 		Alignment Check Exception (486+) 	No
;18 		Machine Check Exception (Pentium/586+) 	No
;19 to 31 	Reserved Exceptions 			No


; Definitions

global __isr0
global __isr1
global __isr2
global __isr3
global __isr4
global __isr5
global __isr6
global __isr7
global __isr8
global __isr9
global __isr10
global __isr11
global __isr12
global __isr13
global __isr14
global __isr15
global __isr16
global __isr17
global __isr18
global __isr19
global __isr20
global __isr21
global __isr22
global __isr23
global __isr24
global __isr25
global __isr26
global __isr27
global __isr28
global __isr29
global __isr30
global __isr31



extern isr



; Implementations

; Some exceptions will push error code onto the stack, so we push 'byte 0' to align stack frame

;0		Division By Zero Exception 		No
__isr0:
	cli
	push byte 0
	push byte 0
	jmp __common_handler

;1 		Debug Exception 			No
__isr1:
	cli
	push byte 0
	push byte 1
	jmp __common_handler

;2 		Non Maskable Interrupt Exception 	No
__isr2:
	cli
	push byte 0
	push byte 2
	jmp __common_handler

;3 		Breakpoint Exception 			No
__isr3:
	cli
	push byte 0
	push byte 3
	jmp __common_handler

;4 		Into Detected Overflow Exception 	No
__isr4:
	cli
	push byte 0
	push byte 4
	jmp __common_handler

;5 		Out of Bounds Exception 		No
__isr5:
	cli
	push byte 0
	push byte 5
	jmp __common_handler

;6 		Invalid Opcode Exception 		No
__isr6:
	cli
	push byte 0
	push byte 6
	jmp __common_handler

;7 		No Coprocessor Exception 		No
__isr7:
	cli
	push byte 0
	push byte 7
	jmp __common_handler

;8 		Double Fault Exception 			Yes
__isr8:
	cli
	push byte 8
	jmp __common_handler

;9 		Coprocessor Segment Overrun Exception 	No
__isr9:
	cli
	push byte 0
	push byte 9
	jmp __common_handler

;10 		Bad TSS Exception 			Yes
__isr10:
	cli
	push byte 10
	jmp __common_handler

;11 		Segment Not Present Exception 		Yes
__isr11:
	cli
	push byte 11
	jmp __common_handler

;12 		Stack Fault Exception 			Yes
__isr12:
	cli
	push byte 12
	jmp __common_handler

;13 		General Protection Fault Exception 	Yes
__isr13:
	cli
	push byte 13
	jmp __common_handler

;14 		Page Fault Exception 			Yes
__isr14:
	cli
	push byte 14
	jmp __common_handler

;15 		Unknown Interrupt Exception 		No
__isr15:
	cli
	push byte 0
	push byte 15
	jmp __common_handler

;16 		Coprocessor Fault Exception 		No
__isr16:
	cli
	push byte 0
	push byte 16
	jmp __common_handler

;17 		Alignment Check Exception (486+) 	No
__isr17:
	cli
	push byte 0
	push byte 17
	jmp __common_handler

;18 		Machine Check Exception (Pentium/586+) 	No
__isr18:
	cli
	push byte 0
	push byte 18
	jmp __common_handler

;19 to 31 	Reserved Exceptions 			No



;				Common handler of 0-31 interrupts
; When processor receive an interrupt signal, it saves the state of the main registers(eip, esp, cs, eflags)
; Processor push them onto the stack.
; Then then it finds the address of the transiton to the interrupt handler in the IDT and
; 	makes the transition to the address


__common_handler:
	pusha 		; pushing edi, esi, ebp, .... , eax
	
	mov ax, ds
	push eax	; we can't "push ds"
	

	; Loading data segment offset designed for the kernel
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	call isr	; common handler describes in C-prog

	pop eax
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax

	popa
	add esp, 8  ; Clear the stack of error code and interrupt number
	iret
	

