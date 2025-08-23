KDIR ?= /lib/modules/$(shell uname -r)/build
PWD  := $(shell pwd)

.PHONY: all clean test

obj-m := irq9.o kcaller.o

all:
	$(MAKE) -C $(KDIR) M=$(PWD) KBUILD_EXTMOD=$(PWD) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) KBUILD_EXTMOD=$(PWD) clean

test: all
	sudo dmesg -C
	sudo insmod irq9.ko
	sudo insmod kcaller.ko
	sleep 2
	sudo dmesg
	sudo rmmod kcaller
	sudo rmmod irq9

