#include "uart.h"


unsigned char CC = 'a';

void uart0_init_regs(){
  UART0_IMSC = 1<<4; // Enable RXIM interrupt
}


/*
  Acknowledge the uart interrupt in uart0_isr
*/
uart_irq_ack(int uart){
  //TODO
}


/*
  Uart Interrupt Service Request
*/
void uart0_isr(){

  unsigned char c = uart_get_byte(UART0);
  while(c != '\0'){
    //TODO: Implement buffer
   CC = c;
    continue; 
  }
  uart_irq_ack(UART0);
}


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
