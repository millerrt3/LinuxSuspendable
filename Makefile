LINUX_BASE = ../RPi-Linux-4.1.13/linux/
MODULE_BASE = $(PWD)/module

all:
	make -C $(LINUX_BASE) SUBDIRS=$(PWD)/module modules
	gcc -o app main.c -I.
	
clean:
	rm -f $(MODULE_BASE)/lkm_suspendable.ko
	rm -f $(MODULE_BASE)/lkm_suspendable.o
	rm -f $(MODULE_BASE)/lkm_suspendable.mod.c
	rm -f $(MODULE_BASE)/lkm_suspendable.mod.o
	rm -f $(MODULE_BASE)/modules.order
	rm -f $(MODULE_BASE)/Module.symvers
	rm -f *.o
	rm app
