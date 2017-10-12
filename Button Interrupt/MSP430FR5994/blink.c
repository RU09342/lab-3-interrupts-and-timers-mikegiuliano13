#include <msp430.h>
 
volatile unsigned int i, delay, mode;
 
int main(void) {
    
  // Stops the watchdog timer
  WDTCTL = WDTPW + WDTHOLD;
  
  // Disables default high-impedance mode
  // Done by clearing the register PM5CTL0
  // and unlocking I/O pins  (~LOCKLPM5)
  PM5CTL0 &= ~LOCKLPM5;
  
  // Sets up P1.0 and P1.1 as outputs
  P1DIR |= BIT0 + BIT1;
  
  // Initializes P1.0 and P1.1 as 0
  // Sets Resistor to pullup (1 is low)
  P5OUT = BIT6;
  
  P5IE |= BIT6; // Interrupt Enabled on P5.6 (the button)
  P5IFG &= ~BIT6; // CLears interrupt flag once button P5.6 is pressed
  
  P5REN |= BIT6; // Enable Pull Up on (P5.6)
  P5IES |= BIT6; // P5.6 Hi/lo edge
 
__bis_SR_register(GIE); // Enables all integrupts (Global)

  // Initializes the amount of delay between LED changes
  delay = 0x6000;
  
  // Used in case statement to set speed of LED, initially set to slowest
  mode = 0;

  // No parameters means it loops forever
  for (;;) {
    
    if (mode != 0) {  
    // toggle LEDs
    P1OUT ^= BIT0 + BIT1;
    
    // delay for a while
    for (i = 0; i < delay; i++);
    }
  }
  
}

// Interrupt subroutine
// Called whenever button is pressed
#pragma vector=PORT5_VECTOR
__interrupt void Port_5(void)
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
    
    P5IFG &= ~BIT6; // P5.6 interrupt flag cleared
	
}