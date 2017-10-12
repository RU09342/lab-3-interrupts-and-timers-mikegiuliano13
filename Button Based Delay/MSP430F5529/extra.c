// Loads configurations for all MSP430 boards
#include <msp430.h>

volatile unsigned int press = 0;
volatile unsigned int count = 0;
volatile unsigned int i = 0;

void frequencyCalc(int t);

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    P1DIR = BIT0; // Set P1.0 as output
    P1REN |= BIT1; // Connects the on-board resistor to P1.1
    P1OUT = BIT1; // Sets up P1.1 as pull-up resistor
    
    P1IES |= BIT1; // Interrupts when you press button
    P1IE |= BIT1; // Enable interrupt on button pin
    P1IFG &= BIT1; // Clear interrupt flag
	
    P2REN |= BIT1; // Connects the on-board resistor to P2.1
    P2OUT = BIT1; // Sets up P2.1 as pull-up resistor
    
    P2IES |= BIT1; // Interrupts when you press button
    P2IE |= BIT1; // Enable interrupt on button pin
    P2IFG &= BIT1; // Clear interrupt flag
    
    //TA0CTL = Timer A0 chosen for use
    //TASSEL_1 Selects SMCLK as clock source
    //MC_1 Count-up mode
    TA0CCTL0 = CCIE;
    TA0CTL = TASSEL_2 + MC_1;

    frequencyCalc(10);    // initialize timer to 10Hz

    __enable_interrupt(); // MUST BE ENABLED IN ADDITION TO GIE
    __bis_SR_register(LPM0 + GIE); // enable interrupts in LPM0

}

// Initializes the timer to an arbitrary visible frequency
void frequencyCalc(int t)
{
    int x;
    x = (10000000/8) / t;
    TA0CCR0 = x;
}

// Interrupt subroutine
// Called whenever button is pressed
#pragma vector = PORT1_VECTOR
__interrupt void PORT_1(void)
{
    press = 1;
    i = 0; // Reset timing counter

    P1IFG &= ~BIT1; // Clear interrupt flag
}

// RESET interrupt subroutine
// Called whenever button is pressed
#pragma vector = PORT2_VECTOR
__interrupt void PORT_2(void)
{
    press = 0;
    count = 0;
	i = TA0CCR0  / 100000; // sets i back to 10 Hz

    P2IFG &= ~BIT1; // Clear interrupt flag
}

// Interrupt subroutine
// Called at every timer interval
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A0(void)
{
    if (press == 1)
    {
        if (P1IN & BIT1) // Reset when the button is high
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