#include <msp430g2553.h>
 
volatile unsigned int i, delay, mode;
 
int main(void) {
    
  // Stops the watchdog timer
  WDTCTL = WDTPW + WDTHOLD;
  
  // Sets up bits 0 and 6 of P1 as output
  P1DIR |= BIT0 + BIT6;
  
  // Initializes BIT0 and BIT6 to 0
  // Also sets P1.3 (button) as output
  P1OUT = BIT3;
  
  P1IE |= BIT3; // Interrupt Enabled on P1.3 (the button)
  P1IFG &= ~BIT3; // CLears interrupt flag once button P1.3 is pressed
  
  P1REN |= BIT3; // Enable Pull Up on (P1.3)
  P1IES |= BIT3; // P1.3 Hi/lo edge
 
__bis_SR_register(GIE); // Enables all integrupts (Global)

  // Initializes the amount of delay between LED changes
  delay = 0x6000;
  
  // Used in case statement to set speed of LED, initially set to slowest
  mode = 0;

  // No parameters means it loops forever
  for (;;) {
    
    if (mode != 0) {  
    // toggle bit 0 of P1
    P1OUT ^= BIT0 + BIT6;
    
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
    
    P1IFG &= ~BIT3; // P1.3 interrupt flag cleared