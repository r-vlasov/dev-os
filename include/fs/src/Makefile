OBJECTS = vfs.o initrd_loader.o
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
             -nostartfiles -nodefaultlibs -Wall -Wextra  -c
AS = nasm
ASFLAGS = -f elf32


DIR1 = src


%.o: %.c
	$(CC) $(CFLAGS)  $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@


run: 	$(OBJECTS)
	mv *.o ..




	

