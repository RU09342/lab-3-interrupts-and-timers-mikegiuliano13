// Loads configurations for all MSP430 boards
#include <msp430.h>

volatile unsigned int press = 0;
volatile unsigned int count = 0;
volatile unsigned int i = 0;

void frequencyCalc(int t);

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
	
	// Disables default high-impedance mode
	// Done by clearing the register PM5CTL0
	// and unlocking I/O pins  (~LOCKLPM5)
	PM5CTL0 &= ~LOCKLPM5;

    P1DIR = BIT0; // Set P1.0 as output
    P5REN |= BIT6; // Connects the on-board resistor to P5.6
    P5OUT = BIT6; // Sets up P5.6 as pull-up resistor
	P1OUT |= 0x00;
    
    P5IES |= BIT6; // Interrupts when you press button
    P5IE |= BIT6; // Enable interrupt on button pin
    P5IFG &= BIT6; // Clear interrupt flag
    
    //TB0CTL = Timer B0 chosen for use
    //TBSSEL_1 Selects SMCLK as clock source
    //MC_1 Count-up mode
    TB0CCTL0 = CCIE;
    TB0CTL = TBSSEL_2 + MC_1;

    frequencyCalc(10);    // initialize timer to 10Hz

    __enable_interrupt(); // MUST BE ENABLED IN ADDITION TO GIE
    __bis_SR_register(LPM0 + GIE); // enable interrupts in LPM0

}

// Initializes the timer to an arbitrary visible frequency
void frequencyCalc(int t)
{
    int x;
    x = (10000000/8) / t;
    TB0CCR0 = x;
}

// Interrupt subroutine
// Called whenever button is pressed
#pragma vector = PORT5_VECTOR
__interrupt void PORT_5(void)
{
    press = 1;
    i = 0; // Reset timing counter

    P5IFG &= ~BIT6; // Clear interrupt flag
}

// Interrupt subroutine
// Called at every timer interval
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer_B0(void)
{
    if (press == 1)
    {
        if (P5IN & BIT6) // Reset when the button is high
        {
            press = 0;
            count = 0;
        }

        else    // Increment i on each timer interrupt
                // WHILE button is being pressed
        {
            P1OUT |= BIT0; // visual LED indication of button being held down
            i++;
        }


    }

    else // Set LED blink duration to button hold duration
    {
        if (count == i) // If the current duration == the button hold duration
        {
            P1OUT ^= BIT0; //Toggle LED
            count = 0;  // Reset count
        }

        else
            count++;    // Otherwise, keep counting up
    }
}