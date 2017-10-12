#include <msp430.h>
 
volatile unsigned int i, delay, mode;
 
int main(void) {
    
  // Stops the watchdog timer
  WDTCTL = WDTPW + WDTHOLD;
  
  // Disables default high-impedance mode
  // Done by clearing the register PM5CTL0
  // and unlocking I/O pins  (~LOCKLPM5)
  PM5CTL0 &= ~LOCKLPM5;
  
  // Sets up P1.0 and P9.7 as output
  P1DIR |= BIT0;
  P9DIR |= BIT7;
  
  // Initializes P1.0 and P9.7 to 0
  // Also sets P1.1 (button) as output
  P1OUT = BIT1;
  P9OUT = 0x00;
  
  P1IE |= BIT1; // Interrupt Enabled on P1.1 (the button)
  P1IFG &= ~BIT1; // CLears interrupt flag once button P1.1 is pressed
  
  P1REN |= BIT1; // Enable Pull Up on (P1.1)
  P1IES |= BIT1; // P1.1 Hi/lo edge
 
__bis_SR_register(GIE); // Enables all integrupts (Global)

  // Initializes the amount of delay between LED changes
  delay = 0x6000;
  
  // Used in case statement to set speed of LED, initially set to slowest
  mode = 0;

  // No parameters means it loops forever
  for (;;) {
    
    if (mode != 0) {  
    // toggle bit 0 of P1
    P1OUT ^= BIT0;
	P9OUT ^= BIT7;
    
    // delay for a while
    for (i = 0; i < delay; i++);
    }
  }
  
}

// Interrupt subroutine
// Called whenever button is pressed
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    mode++; // Increase mode
    switch (mode) {
    case 0:
        break;
    case 1:
        delay = 0x6000; // Slow timing
        break;
    case 2:
        delay = 0x3500; // Medium timing
        break;
    case 3:
        delay = 0x1000; // Fast timing
        break;
    default:
		// Catchall in case of error (should not end up here)
		// Resets mode and sets to slow timing
        delay = 0x6000;
        mode = 0;
	}
    
    P1IFG &= ~BIT1; // P1.1 interrupt flag cleared
}