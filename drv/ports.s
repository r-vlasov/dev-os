; It is the assembly file which has 2 function helped to hardware-communication

global outport;
global inport;

outport:
	mov edx, [esp + 4]
	mov al, [esp + 8]
	out dx, al
	ret

inport:
	mov edx, [esp + 4]
	in al, dx
	ret

