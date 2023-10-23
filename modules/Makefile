KDIR		:= /lib/modules/`uname -r`/build
MODNAME		:= dump_meminfo

obj-m		:= $(MODNAME).o
$(MODNAME)-objs	:= main.o v2p.o mem_profile.o


all:
	make -C $(KDIR) M=`pwd` modules

clean:
	make -C $(KDIR) M=`pwd` clean

test:
	sudo insmod $(MODNAME).ko opcode=$(OPCODE) pid=$(PID) vaddr_head=$(VADDR_START) vaddr_end=$(VADDR_END)
	sudo rmmod $(MODNAME)

search_huge:
	sudo insmod $(MODNAME).ko opcode=$(OPCODE) pid=$(PID) vaddr_head=$(VADDR_HEAD) vaddr_end=$(VADDR_END)
	sudo rmmod $(MODNAME)