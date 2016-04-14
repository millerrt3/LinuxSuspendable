# Points to the root of the linux source directory
LINUX_BASE = ../RPi-Linux-4.1.13/linux/
INSPECT_BASE = $(PWD)/inspect

all:
	make -C $(LINUX_BASE) SUBDIRS=$(INSPECT_BASE) modules

	# Userspace control process for inspection module
	gcc -Wall -o control inspect/control.c
	
clean:
	rm -f *.o
	rm -f $(INSPECT_BASE)/*.o
	rm -f control
