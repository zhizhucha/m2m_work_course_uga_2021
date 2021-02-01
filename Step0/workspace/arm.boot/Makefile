
QEMU=qemu-system-arm

TOOLCHAIN=arm-none-eabi

ASFLAGS= -mcpu=arm926ej-s -g 
CFLAGS= -c -mcpu=arm926ej-s -g -nostdlib
LDFLAGS= -T kernel.ld 

MACHINE=versatilepb
MACHINE=versatileab

MEMORY=1M

# Notice that we link with our own linker script: test.ld
all: startup.o main.o kprintf.o exception.o uart.o isr.o
	$(TOOLCHAIN)-ld $(LDFLAGS) startup.o main.o kprintf.o exception.o uart.o isr.o -o kernel.elf
	$(TOOLCHAIN)-objcopy -O binary kernel.elf kernel.bin

clean: 
	rm -f startup.o test.o kernel.elf

startup.o: startup.s
	$(TOOLCHAIN)-as $(ASFLAGS) startup.s -o startup.o

exception.o: exception.s
	$(TOOLCHAIN)-as $(ASFLAGS) exception.s -o exception.o

kprintf.o: kprintf.c
	$(TOOLCHAIN)-gcc $(CFLAGS) kprintf.c -o kprintf.o

main.o: main.c uart.h
	$(TOOLCHAIN)-gcc $(CFLAGS) main.c -o main.o

uart.o: uart.c uart.h
	$(TOOLCHAIN)-gcc $(CFLAGS) uart.c -o uart.o

isr.o: isr.c isr.h
	$(TOOLCHAIN)-gcc $(CFLAGS) isr.c -o isr.o

run: all
	$(QEMU) -M $(MACHINE) -m $(MEMORY) -nographic -kernel kernel.bin -serial mon:stdio

debug: all
	
	$(QEMU) -M $(MACHINE) -m $(MEMORY) -nographic -kernel kernel.bin -serial mon:stdio -gdb tcp::1234 -S

debug-cli:
	gdb-multiarch -q -ex 'set architecture arm' -ex 'file kernel.elf' -ex 'target remote localhost:1234' -ex 'layout split' -ex 'layout regs'

kill:
	pkill qemu-system-arm

