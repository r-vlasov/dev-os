CC = gcc
CFLAGS = -m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
             -nostartfiles -nodefaultlibs -Wall -Wextra  -c
DIR1 = src


%.o: %.c
	$(CC) $(CFLAGS)  $< -o $@
run:
	$(kheap.o)
	$(MAKE) -C $(DIR1)
	mv *.o ..




	

