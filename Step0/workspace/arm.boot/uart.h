#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <stddef.h>
/**
 * Look at the document describing the Versatile Application Board:
 *
 *    Versatile Application Baseboard for ARM926EJ-S (HBI-0118)
 *
 * Also referenced as DUI0225, downloadable from:
 *
 *  http://infocenter.arm.com/help/index.jsp?topic=/com.arm.doc.dui0225d/I999714.html
 *
 * Look at the memory map, section 4.1, page 137, to find the base addresses for all
 * the devices. We only use here the UARTs.
 */
#define UART0 0x101f1000
#define UART1 0x101f2000
#define UART2 0x101f3000

/**
 * PL011_T UART
 *     http://infocenter.arm.com/help/topic/com.arm.doc.ddi0183f/DDI0183.pdf
 *
 * UARTDR: Data Register   (0x00)
 *    To read received bytes
 *    To write bytes to send
 *    Bit Fields:
 *      15:12 reserved
 *      11:08 error flags
 *       7:0  data bits
 * UARTFR:  Flag Register  (0x18)
 *    Bit Fields:
 *      7:  TXFE  transmit FIFO empty
 *      6:  RXFF  receive FIFO full
 *      5:  TXFF  transmit FIFO full
 *      4:  RXFE  receive FIFO empty
 *      3:  BUSY  set when the UART is busy transmitting data
 */


#define UART_DR 0x00
#define UART_FR 0x18

/**
 * Receive interrupt mask. A read returns the current mask for the UARTRXINTR interrupt.
 * On a write of 1, the mask of the UARTRXINTR interrupt is set. A write of 0 clears the mask.
 */
#define UART_IMSC 0x38  // [r/w] Masked interrupt status register

#define UART_TXFE (1<<7)
#define UART_RXFF (1<<6)
#define UART_TXFF (1<<5)
#define UART_RXFE (1<<4)
#define UART_BUSY (1<<3)

extern unsigned char CC;

//Interrupt for RX event
#define UART_RXRIS (1<<4) // Or RXIM Receive Interrupt Mask

// [read/write] Interrupt mask set/clear register [:11]
#define UART_MIS 0x040 
// [read] Raw interrupt status register [:11]
#define UART_RIS 0x03C 

//[write] Interrupt Clear Register [:11]
#define UART_TICR 0x044 




#define UART0_MIS (*((volatile uint32_t *) (UART0 + 0x040)))
#define MAX_CHARS 256


//Circullar buffer
// It is used to contains the bytes retrieved from the uart
extern volatile uint8_t buffer[MAX_CHARS];
extern int buffer_head;
extern int buffer_tail;


/**
 * Initialization of uart regs.
 * Enable the generation of interrupts for the given uart
 * We only want interrupt generation for the receive event
 */
void uart_init_regs(int uart);

/**
 * Handler for uart interrupts
 * It is called from the irq_handler.
 */
void uart_isr(int uart);

/**
 * Retrieve a byte from the given uart, this is a non-blocking call
 * Its returns null char if uart buffer is empty
 */
unsigned char  uart_get_byte(int uart);

/**
 * Sends a character through the given uart, this is a blocking call.
 * The code spins until there is room in the UART TX FIFO queue to send
 * the character.
 */
void uart_send(int uart, unsigned char s);

/**
 * This is a wrapper function, provided for simplicity,
 * it sends a C string through the given uart.
 */
void uart_send_string(int uart, const unsigned char *s);
#endif