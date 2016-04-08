# Points to the root of the linux source directory
LINUX_BASE = ../RPi-Linux-4.1.13/linux/
MODULE_BASE = $(PWD)/module
INSPECT_BASE = $(PWD)/inspect
UAPI_BASE = $(PWD)/uapi

all:
	make -C $(LINUX_BASE) SUBDIRS=$(MODULE_BASE) modules
	make -C $(LINUX_BASE) SUBDIRS=$(INSPECT_BASE) modules
	make -C uapi

	# Userspace control process for inspection module
	gcc -o inspect/control inspect/control.c
	
clean:
	rm -f *.o
	rm -f $(MODULE_BASE)/*.o
	rm -f $(INSPECT_BASE)/*.o
