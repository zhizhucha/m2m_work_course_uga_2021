#include "main.h"
#include "uart.h"
#include "isr.h"



/**
 * This is the C entry point, upcalled once the hardware has been setup properly
 * in assembly language, see the startup.s file.
 */
void c_entry() {
  uart0_init_regs();
  irqs_setup();

  int i = 0;
  int count = 0;
  uart_send_string(UART0, "\nHello world!\n");
  uart_send_string(UART0, "\nQuit with \"C-a c\" and then type in \"quit\".\n");
  
  while (1) {
    count++;
    if(count > 30)break;
    unsigned char c;
    //uart_send_string(UART0, "OK_");
    uart_send_string(UART0, &CC);
    _wfi();

    
  }
}
