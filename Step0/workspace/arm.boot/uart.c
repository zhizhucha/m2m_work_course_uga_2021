#include "uart.h"
volatile uint8_t buffer[MAX_CHARS] = {0};
int buffer_head = 0;
int buffer_tail = 0;
unsigned char CC = 'a';


/**
 * Initialization of uart regs.
 * Enable the generation of interrupts for the given uart
 */
void uart_init_regs(int uart){

 int* uart_imsc_addr =  (int*)(uart + UART_IMSC);
 *(uart_imsc_addr) = UART_RXRIS ; // Enable interrupt generation for Receive event
   
}

  


/**
 * Handler for uart interrupts
 * It is called from the irq_handler.
 */
void uart_isr(int uart){
  unsigned char c = uart_get_byte(uart);
  while(c != '\0'){
    int next = (buffer_head + 1) % MAX_CHARS;
    if(next==buffer_tail)return;
    buffer[buffer_head] = c;
    buffer_head = next;
    CC = c;
    c = uart_get_byte(UART0);
  }
}

/**
 * Retrieve a byte from the given uart, this is a non-blocking call
 * Its returns null char if uart buffer is empty
 */
unsigned char  uart_get_byte(int uart){
  unsigned short* uart_fr = (unsigned short*) (uart + UART_FR);
  unsigned short* uart_dr = (unsigned short*) (uart + UART_DR);
  if(*uart_fr & UART_RXFE)return '\0';
  else{
    return (*uart_dr & 0xff);
  }
}

/**
 * Sends a character through the given uart, this is a blocking call.
 * The code spins until there is room in the UART TX FIFO queue to send
 * the character.
 */
void uart_send(int uart, unsigned char s) {
  unsigned short* uart_fr = (unsigned short*) (uart + UART_FR);
  unsigned short* uart_dr = (unsigned short*) (uart + UART_DR);
  while (*uart_fr & UART_TXFF)
    ;
  *uart_dr = s;
}

/**
 * This is a wrapper function, provided for simplicity,
 * it sends a C string through the given uart.
 */
void uart_send_string(int uart, const unsigned char *s) {
  while (*s != '\0') {
    uart_send(uart, *s);
    s++;
  }
}
