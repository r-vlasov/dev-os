

DIR1 = EasyOS
DIR2 = EasyOS/obj

build:
	$(MAKE) -C $(DIR1)

run:
	$(MAKE) -C $(DIR2)
	bochs

start: 
	$(MAKE) build
	$(MAKE) run
