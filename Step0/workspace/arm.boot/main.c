#include "main.h"
#include "uart.h"
#include "isr.h"

/**
 * For now, we just feed a new line when receiving the carriage return in the buffer.
 */
void interpret(unsigned char* buffer,int buffer_tail, int eol){
  uart_send_string(UART0, "\n\r");

}

/**
 * This is the C entry point, upcalled once the hardware has been setup properly
 * in assembly language, see the startup.s file.
 */
void c_entry() {
  _copy_vector();
  uart_init_regs(UART0);
  irqs_setup();
  irqs_enable();

  int i = 0;
  int count = 0;
  uart_send_string(UART0, "\nHello world!\n");
  //uart_send_string(UART0, "\nQuit with \"C-a c\" and then type in \"quit\".\n");
  int line_cnt = 0;
  while (1) {
     int eol = buffer_tail;
   
    kprintf("%c", '\r');
    //unsigned char cur_line[256];

    while(eol!=buffer_head){
      int ccode = (int) buffer[eol];
      //kprintf(" %d " , ccode);

      eol = (eol + 1) % MAX_CHARS;
      if(buffer[eol] == '\r'){
        line_cnt++;
        interpret(buffer,buffer_tail,eol);
        eol = (eol+1) % MAX_CHARS;
        buffer_tail = eol;
      }else{

        //Detecting delete key
        //TODO: There is still a problem , when we push twice the key
        if( (int)(buffer[eol]) == 27 && 
            (int)(buffer[ (eol +1) % MAX_CHARS]) == 91 && 
             (int)(buffer[ (eol +2) % MAX_CHARS]) == 51 && 
              (int)(buffer[ (eol +3) % MAX_CHARS]) == 126){
                //kprintf("Test: %c", 'g');
                eol = (eol+4) % MAX_CHARS;
                kprintf("%c",27);
                kprintf("%c",91);
                kprintf("%c",68);

                kprintf("%c",32);

                kprintf("%c",27);
                kprintf("%c",91);
                kprintf("%c",68);
        }else{
           kprintf("%c", buffer[eol]);
        }

       
      }
    }
    
    //kprintf("%s", buffer);
   

    unsigned char c;
    //int ia = (int)(CC);
    //  kprintf("%d ", ia);
    //uart_send_string(UART0, "OK_");
    //uart_send_string(UART0, &CC);
    _wfi();
  
    
  }
}
