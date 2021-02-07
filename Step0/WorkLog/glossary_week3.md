--------- GLOSSARY ---------------------------


`IRQ` : (Interrupt Request). A hardware signal sent to the processor, that stops temporally a running program so that it can execute an `Interrupt Handler`.

`ISR` : (Interrupt Service Routine). Also called interrupt handler. A software process invoked by an interrupt request from hardware device. Its role is to handle the interrupt and sends it to the CPU. Once the ISR is finished, the interrupted process is resume.

`PIC` : //TODO

`VIC` : (Vector Interrupt Controller). See ARM.
		VIC provides a software interface to the interrupt system. In ARM system, two levels of interrupt are availlable : `FIQ` and `IRQ`.

`FIQ` : Used for fast, low latency interrupt handling

`MMIO` : (Memory-Mapped I/O) Uses the same address space to address both memory and i/o devices. A memory area associated with an I/O device, such that when you write or read from the addres, you communication data with it.

`cpsr` : (Current Program Status Register) Holds processoor status and control informations
