# Makefile
obj-m += sysaddr.o

KDIR ?= /lib/modules/$(shell uname -r)/build
MODULE = sysaddr.ko

all:
	$(MAKE) -C $(KDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean

test: all
	@echo "=== Clearing dmesg ==="
	sudo dmesg -C
	@echo "=== Inserting module ==="
	sudo insmod $(MODULE)
	@sleep 1
	@echo "=== Latest dmesg output ==="
	sudo dmesg
	@echo "=== Removing module ==="
	sudo rmmod sysaddr
	@echo "=== Test complete ==="
