#include "isr.h"
#include "uart.h"


/*
 * Define ECHO_ZZZ to have a periodic reminder that this code is polling
 * the UART, actively. This means the processor is running continuously.
 * Polling is of course not the way to go, the processor should halt in
 * a low-power state and wake-up only to handle an interrupt from the UART.
 * But this would require setting up interrupts...
 */
#define ECHO_ZZZ

/* Polling the PIC (via mmio reads) */
int pic_next_raised_irq(){
  
  return UART0_IRQ;
  
}


/* Re-enabling interrupts at the PIC */
void pic_enable_all_irqs(){
  //TODO
  irqs_enable();
}




/* Interrupt service routine, upcalled from assembly */
void __attribute__((interrupt)) irq_handler(){
  CC = 'g';
  int count = 1;
  for(;;){
    int irq = pic_next_raised_irq();
    
    switch(irq){
      case -1:
        pic_enable_all_irqs();
        return;
      case UART0_IRQ:

        uart0_isr();
        irqs_enable();
        break;
      default:
        break;
    }

  }
}


void irqs_setup(){
    //      setup of the IRQ mode on the processor
    _irqs_setup();
    //      enable the UART0 IRQ
    uint32_t* pic_int_enable = (uint32_t*) (PIC_INT_ENABLE);
  *pic_int_enable = 1 << 12 ; // Enable UART0 IRQ

  
}

/* Turns on interrupts: processor can be interrupted by the VIC */
void irqs_enable(){
    _irqs_enable();
}

/* Turns off */
void irqs_disable(){
    _irqs_disable();
}