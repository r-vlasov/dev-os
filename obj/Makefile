OBJECTS =  kmain.o isrs.o startloader.o ports.o interrupt_desc_table.o global_desc_table.o load-tables.o string.o drivers.o keyboard.o tty.o 
OBJECTS1 = *.o
CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
             -nostartfiles -nodefaultlibs -Wall -Wextra  -c
LDFLAGS = -T link.ld -melf_i386
AS = nasm
ASFLAGS = -f elf


%.o: %.c
	$(CC) $(CFLAGS)  $< -o $@

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@


run: 		clean

clean:		makeiso
	rm -rf *.o kernel.elf os.iso
makeiso:	os.iso
	cp os.iso ../../os.iso
os.iso: 	kernel.elf
	cp kernel.elf ../../iso/boot/kernel.elf
	genisoimage -R                              \
                    -b boot/grub/stage2_eltorito    \
                    -no-emul-boot                   \
                    -boot-load-size 4               \
                    -A os                           \
                    -input-charset utf8             \
                    -quiet                          \
                    -boot-info-table                \
                    -o os.iso                       \
                    ../../iso
kernel.elf: 
	ld $(LDFLAGS) $(OBJECTS1) -o kernel.elf




