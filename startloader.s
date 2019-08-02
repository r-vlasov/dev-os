    global start;		    ; The entry symbol for ELF
    extern kmain; 		    ; kmain is defined in C-file
    global GET_EIP;
    global multiboot_spec 	    ; so we need to use it in C-code
    global copy_page_physical


    MBOOT_HEADER_MAGIC 	equ 	0x1BADB002   					; define the magic number constant
    MBOOT_HEADER_FLAGS  equ 	0x0         					; multiboot flags
    MBOOT_CHECKSUM     	equ 	-( MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS) 	; calculate the checksum
                            ; (magic number + checksum + flags should equal 0)
    KERNEL_STACK_SIZE equ 4096     						; size of stack in bytes
    
section ._mbHeader
    	align 4
    multiboot_spec:
	dd MBOOT_HEADER_MAGIC
	dd MBOOT_HEADER_FLAGS
	dd MBOOT_CHECKSUM
	dd multiboot_spec

section .text
    start:				; the loader label (defined as entry point in linker script)    
	mov	esp, kernel_stack + KERNEL_STACK_SIZE    
        push    esp 
        push	ebx			; push into the stack the address of the structure recieved from the loade;r	
	;push 	eax 			; push into the stack the identifier
        call kmain  


    GET_EIP:
	pop	eax
	jmp	eax



copy_page_physical:
   push ebx              ; According to __cdecl, we must preserve the contents of EBX.
   pushf                 ; push EFLAGS, so we can pop it and reenable interrupts
                         ; later, if they were enabled anyway.
   cli                   ; Disable interrupts, so we aren't interrupted.
                         ; Load these in BEFORE we disable paging!
   mov ebx, [esp+12]     ; Source address
   mov ecx, [esp+16]     ; Destination address

   mov edx, cr0          ; Get the control register...
   and edx, 0x7fffffff   ; and...
   mov cr0, edx          ; Disable paging.

   mov edx, 1024         ; 1024*4bytes = 4096 bytes to copy

.loop:
   mov eax, [ebx]        ; Get the word at the source address
   mov [ecx], eax        ; Store it at the dest address
   add ebx, 4            ; Source address += sizeof(word)
   add ecx, 4            ; Dest address += sizeof(word)
   dec edx               ; One less word to do
   jnz .loop

   mov edx, cr0          ; Get the control register again
   or  edx, 0x80000000   ; and...
   mov cr0, edx          ; Enable paging.

   popf                  ; Pop EFLAGS back.
   pop ebx               ; Get the original value of EBX back.
   ret


    
section .bss
    align 4                                     ; align at 4 bytes
    kernel_stack:                               ; label points to beginning of memory
        resb KERNEL_STACK_SIZE                  ; reserve stack for the kernel



 
