#ifndef ISR_H
#define ISR_H
#include <stdint.h>

#define VICIRQSTATUS 0x000


#define PIC_BASE         0x10140000

#define PIC_IRQ_STATUS      0x10140000 //[read] IRQ status register 
#define PIC_FIQ_STATUS      0x10140004 //[read] FIG status register
#define PIC_RAW_INTR        0x10140008 // [read] Raw interrupt status register
#define PIC_INT_ENABLE      0x10140010 // [read/write] Interrupt enable register
#define PIC_INT_EN_CLEAR    0x10140014// [write] Interrupt enable clear register


/* Interrupt signals to PIC (Primary Interrupt Controller) */
#define WATCHDOG_IRQ    0   // PIC_0

#define UART0_IRQ       12 // PIC_12
#define UART0_IRQ_MASK (1<<UART0_IRQ) 
extern unsigned char CC;


//Assembly method calls
void _irqs_setup(void);
void _irqs_enable(void);
void _copy_vector(void);
void _wfi();




void uart_init_regs(int uart);


void irq_handler();


/**
 * Setup interrupts on the processor
 * In our case, we also enable the UART0 IRQ
 */
void irqs_setup();

/**
 * Enable interrupts on the processor
 */
void irqs_enable();

/**
 * Disable interrupts on the processor
 */
void irqs_disable();



#endif