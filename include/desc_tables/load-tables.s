; It's the assembly file which has functions loading 
; Interrupt Descriptors Table and Global Descriptors Table

global load_idt;
global load_gdt;


load_idt:
	mov edx, [esp + 4]
	lidt [edx]
	sti
	ret


load_gdt:
	mov eax, [esp+4]
	lgdt [eax]	
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax
	ret


