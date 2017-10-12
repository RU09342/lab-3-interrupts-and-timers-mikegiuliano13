// Loads configurations for all MSP430 boards
#include <msp430.h>
 
int main(void) {
    
  // Stops the watchdog timer
  WDTCTL = WDTPW + WDTHOLD;
  
  // Sets up bit 0 of P1 as output
  P1DIR |= BIT0;
  
  // Initializes BIT0 to 0
  P1OUT = 0x00;
  
  // Declares loop variable as volatile
  // This prevents compiler from removing it for being "stuck" in a loop
  // when in fact it is working as intended
  volatile unsigned int i;

  // No parameters means it loops forever
  for (;;) {
      
    // Toggle bit 0 of P1 using XOR
    P1OUT ^= BIT0;
    
    // Delay for a while
    for (i = 0; i < 0x6000; i++); //note to self: can also use __delay_cycles(10000);
  }
  
}