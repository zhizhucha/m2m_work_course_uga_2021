
# Introducing Interrupts

## Preface by Pr. Olivier Gruber

The introduction of interrupt handling in the code will be a profound change,
as explained during our lecture on this topic.

First, you are discovering the hardware support for exception handling, interrupts and traps. This means understanding the different modes of execution of the processor and how it changes from one mode to another.

Second, you are discovering the software challenges introduced by interrupts, namely the race conditions introduced by the any-time and any-where properties of interrupts. This requires careful programming, cleanly separating what code executes in order to interrupts and the rest of the code that represents the normal execution.

Therefore, we strongly suggest that you adopt an overall structure of your code that will help understand and master this separation:

  - uart.c & uart.h
  - isr.c & isr.h
  - exception.s

The QEMU VersatilePB machine corresponds to the
[Versatile Application Baseboard for ARM926EJ-S User Guide
HBI-0118](https://developer.arm.com/documentation/dui0225/latest)
Other related documents will be linked in the paragraphs below.

Understand that these documents are targetting two audiences:
hardware integrators and software developers. So make sure you
focus on the part that are relevants for a software developer,
not the parts that are for hardware integrators.

<!--================================================================-->
## Processor Exceptions


You can look at ARMv5 Architecture Reference Manual
[DDI0100I](https://developer.arm.com/documentation/ddi0100/latest/).
As you can see, it is big (1138 pages).
Look at the Section *Programmer's model* and in particular
at the Section *Processor modes* and the Section *Exceptions*.  

The file *"exception.s"* is given to you. Still, you need to understand it.

Make sure you understand the different modes of operation for
the ARM processors.

Understand the difference between the interrupt request handlers (_irq_handler)
and the interrupt service routine (isr).

Understand the wait for interrupt function.

<!--================================================================-->
## Global Interrupt Service Routine

Look a the PrimeCell Vectored Interrupt Controller (PL190)
Technical Reference Manual
[DDI0181](https://developer.arm.com/documentation/ddi0181/latest).

We suggest that you create the usual pair of files: isr.c and isr.h

We suggest the following *C functions*:

```C
   void irq_handler();
   void irqs_setup();
   void irqs_enable();
   void irqs_disable();
```

The first is the interrupt service routine, upcalled from assembly.
Its role is to dialog with the VIC to identify which device controllers
have raised which interrupts and to call the appropiate ISRs for these
devices.

The second is the initial setup that is necessary to do before
the irqs can be enabled on the processor:

  - setup of the IRQ mode on the processor
  - enable the UART0 IRQ

The third turns on the interrupts on the processor, meaning the processor
can be interrupted by the VIC.

The fourth turns off the interrupts on the processor, meaning the processor
cannot be interrupted by the VIC.

<!--================================================================-->
## UART Interrupt Service Routine

This is the support, at the level of the UART controller,
for the UART-specific interrupts.

Look a the PL011 Technical Reference Manual
[DDI013G](https://developer.arm.com/documentation/ddi0183/latest/).

But also look at
[Versatile Application Baseboard for ARM926EJ-S User Guide
HBI-0118](https://developer.arm.com/documentation/dui0225/latest),
at the hardware description (Section 3) and subsection on the interrupts
(Section 3.10), specifically at the Figure 3.23 page 111.

From the general ISR, you
will need to determine which UART has raised an interrupt
so that you can pass the UART number to this function:

```
    void uart_isr(uint8_t uartno);
```

You will also need to turn on the interrupts on the UART,
otherwise, the UART will not generate interrupt requests
to the VIC which will therefore not interrupt the processor.

Last but not least, you need to device your communication channel
between the UART interrupt service routine and the actual normal
execution flow.



## Enabling IRQ for UART

We need to enable the interrupts in three place:
  - In ARM cpsr ( IRQs)
  - In the Interrupt Controller ( The UART IRQ)
  - In UART registers for the chosen events ( Interrupt Generation)


We want to trigger interrupt when receiving byte from the UART. Then its must respond to RX.

```
  /* Interrupt Mask Set/Clean Register */
  #define UART0_IMSC (*((volatile uint32_t *)(UART0_BASE_ADDR + 0x038)))
  
```

```
  UART0_IMSC = 1<<4; // Enable RXIM interrupt
```




<!--================================================================-->
# GitHub Markdown Cheat Sheet

You can have a look at the following [link](https://github.com/adam-p/markdown-here/wiki/Markdown-Cheatsheet#code).

For a comment:  
> (empty line)      
> [comment]: # (This actually is the most platform independent comment)

Or use the HTML comment syntax:  
> <\!-- This is a comment -->
