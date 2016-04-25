# Points to the root of the linux source directory
LINUX_BASE = ../RPi-Linux-4.1.13/linux/
INSPECT_BASE = $(PWD)/src

all:
	make -C $(LINUX_BASE) SUBDIRS=$(INSPECT_BASE) modules

	# Userspace control process for inspection module
	gcc -Wall -o tskdmp $(INSPECT_BASE)/control.c $(INSPECT_BASE)/usr_utils.c
	gcc -Wall -o test $(INSPECT_BASE)/test.c
	
clean:
	rm -f *.o
	rm -f $(INSPECT_BASE)/*.o
	rm -f $(INSPECT_BASE)/*.ko
	rm -f $(INSPECT_BASE)/*.mod.c
	rm -f $(INSPECT_BASE)/modules.order
	rm -f $(INSPECT_BASE)/Module.symvers
	rm -f tskdmp
