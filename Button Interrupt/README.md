# Lab 3: Button Interrupt

## General Structure

All five boards follow the same general format for achieving a three-speed
LED toggle. It starts out with both LEDs off, then when the button is pressed, 
they begin to blink slowly. Pressing again further icreases the speed to medium, 
and another press to fast speed.

A further press will stop the LEDs from blinking to restart the cycle again. 
However, it only ensures that they are not blinking, NOT that they are OFF. 
If this happens, the user can reset the program by hitting the RESET button
so that they are in fact off. As a design decision, it was chosen to leave this
behavior in due to the ease of the reset button being readily available.

## Important Distinctions

Generally, differences were in the pinouts for pull-up resistors and buttons.

### MSP430G2553

// Loads configurations for all MSP430 boards
#include <msp430.h>
 
// Declares loop variable as volatile
// This prevents compiler from removing it for being "stuck" in a loop
// when in fact it is working as intended
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
}

### MSP430FR2311

#include <msp430.h>
 
volatile unsigned int i, delay, mode;
 
int main(void) {
    
  // Stops the watchdog timer
  WDTCTL = WDTPW + WDTHOLD;
  
  // Disables default high-impedance mode
  // Done by clearing the register PM5CTL0
  // and unlocking I/O pins  (~LOCKLPM5)
  PM5CTL0 &= ~LOCKLPM5;
  
  // Sets up P1.0 and P2.0 as outputs
  P1DIR |= BIT0;
  P2DIR |= BIT0;
  
  // Initializes P1.0 and P2.0 as 0
  // Also sets P1.1 (button) as output
  P1OUT = BIT1;
  P2OUT = 0x00;
  
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
    // toggle LEDs
    P1OUT ^= BIT0;
	P2OUT ^= BIT0;
    
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

### MSP430F5529

#include <msp430.h>
 
volatile unsigned int i, delay, mode;
 
int main(void) {
    
  // Stops the watchdog timer
  WDTCTL = WDTPW + WDTHOLD;
  
  // Sets up P1.0 and P4.7 as outputs
  P1DIR |= BIT0;
  P4DIR |= BIT7;
  
  // Initializes P1.0 and P4.7 as 0
  // Also sets P1.1 (button) as output
  P1OUT = BIT1;
  P4OUT = 0x00;
  
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
    // toggle LEDs
    P1OUT ^= BIT0;
	P4OUT ^= BIT7;
    
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

### MSP430FR5994

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
  P1OUT = BIT6;
  
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
    
    P5IFG &= ~BIT6; // P5.6 interrupt flag cleared
	
}

### MSP430FR6989

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