obj-m += cool.o
ccflags-y += -Wno-unused-result -mavx2

all: cool.c
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
