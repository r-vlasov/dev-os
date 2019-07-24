
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
             -nostartfiles -nodefaultlibs -Wall -Wextra  -c
AS = nasm
ASFLAGS = -f elf32

%.o: %.c
	$(CC) $(CFLAGS)  $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

DIR1 = desc_tables 
DIR2 = virtual_memory
DIR3 = heap
run:	
	$(MAKE) -C $(DIR1)
	$(MAKE) -C $(DIR2)
	$(MAKE) -C $(DIR3)
	mv *.o ..


	

