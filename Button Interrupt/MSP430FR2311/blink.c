// Loads configurations for all MSP430 boards
#include <msp430.h>
 
int main(void) {
    
  // Stops the watchdog timer
  WDTCTL = WDTPW + WDTHOLD;
  
  // Disables default high-impedance mode
  // Done by clearing the register PM5CTL0
  // and unlocking I/O pins  (~LOCKLPM5)
  PM5CTL0 &= ~LOCKLPM5;
  
  // Sets up bit 0 of P1 as output
  P1DIR |= BIT0;
  
  // Initializes BIT0 (LED) to 0
  // Sets up P1.1 as pull-up resistor
  // as opposed to pull-down
  P1OUT = BIT1;
  
  // Connects the on-board resistor to P1.1
  P1REN |= BIT1;
  
  // Declares loop variable as volatile
  // This prevents compiler from removing it for being "stuck" in a loop
  // when in fact it is working as intended
  volatile unsigned int i;

  // No parameters means it loops forever
  for (;;) {
    
	//Runs when the value of the port input is the same as the input
	//i.e. the button is not pressed
	while(P1IN & BIT1)
        {
            // Output is set to the opposite of what the LED is
			// since button press is low, release is high
			P1OUT &= ~BIT0;
        }
    P1OUT |= 0x01;	// Toggles the LED
  }
  
}