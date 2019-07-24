OBJECTS = drivers.o ports.o
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
             -nostartfiles -nodefaultlibs -Wall -Wextra  -c
AS = nasm
ASFLAGS = -f elf32


DIR1 = keyboard
DIR2 = teletype


%.o: %.c
	$(CC) $(CFLAGS)  $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@


run:	keyboard
	mv *.o ..

keyboard: teletype
	$(MAKE) -C $(DIR1)

teletype: $(OBJECTS)
	$(MAKE) -C $(DIR2)


	

