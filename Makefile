OBJECTS = kmain.o startloader.o kheap.o
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
             -nostartfiles -nodefaultlibs -Wall -Wextra  -c
AS = nasm
ASFLAGS = -f elf32

%.o: %.c
	$(CC) $(CFLAGS)  $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

DIR1 = drv
DIR2 = lib
DIR3 = include


include: lib
	$(MAKE) -C $(DIR3)
	mv *.o obj

lib: drv
	$(MAKE) -C $(DIR2)

drv: run
	$(MAKE) -C $(DIR1)

run: $(OBJECTS)
