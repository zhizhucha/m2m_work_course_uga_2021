
QEMU=qemu-system-arm

TOOLCHAIN=arm-none-eabi

ASFLAGS= -mcpu=arm926ej-s -g 
CFLAGS= -c -mcpu=arm926ej-s -g -nostdlib
LDFLAGS= -T kernel.ld 

MACHINE=versatilepb
MACHINE=versatileab

MEMORY=1M

# Notice that we link with our own linker script: test.ld
all: startup.o main.o kprintf.o
	$(TOOLCHAIN)-ld $(LDFLAGS) startup.o main.o kprintf.o -o kernel.elf
	$(TOOLCHAIN)-objcopy -O binary kernel.elf kernel.bin

clean: 
	rm -f startup.o test.o kernel.elf

startup.o: startup.s
	$(TOOLCHAIN)-as $(ASFLAGS) startup.s -o startup.o

kprintf.o: kprintf.c
	$(TOOLCHAIN)-gcc $(CFLAGS) kprintf.c -o kprintf.o

main.o: main.c
	$(TOOLCHAIN)-gcc $(CFLAGS) main.c -o main.o

run: all
	$(QEMU) -M $(MACHINE) -m $(MEMORY) -nographic -kernel kernel.bin -serial mon:stdio

debug: all
	
	$(QEMU) -M $(MACHINE) -m $(MEMORY) -nographic -kernel kernel.bin -serial mon:stdio -gdb tcp::1234 -S

debug-cli:
	gdb-multiarch -q --nh -ex 'set architecture arm' -ex 'file kernel.elf' -ex 'target remote localhost:1234' -ex 'layout split' -ex 'layout regs'

kill:
	pkill qemu-system-arm

