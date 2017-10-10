# Lab 3: Button Based Delay

## General Structure

This program changes the rate of blinking led based on how long you hold the button. 
Short press = faster blinking, long press = slower blinking.

## Important Distinctions

Generally, differences were in the pinouts for pull-up resistors and buttons.
The MSP430FR2311 and MSP430FR5994 required the use of Timer B instead of Timer A.

### MSP430G2553
//---------------------------------------------------------------------------------------

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
    P1REN |= BIT3; // Connects the on-board resistor to P1.3
    P1OUT = BIT3; // Sets up P1.3 as pull-up resistor
    
    P1IES |= BIT3; // Interrupts when you press button
    P1IE |= BIT3; // Enable interrupt on button pin
    P1IFG &= BIT3; // Clear interrupt flag
    
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

    P1IFG &= ~BIT3; // Clear interrupt flag
}

// Interrupt subroutine
// Called at every timer interval
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A0(void)
{
    if (press == 1)
    {
        if (P1IN & BIT3) // Reset when the button is high
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

### MSP430FR2311
//---------------------------------------------------------------------------------------

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
    P1REN |= BIT1; // Connects the on-board resistor to P1.1
    P1OUT = BIT1; // Sets up P1.1 as pull-up resistor
    
    P1IES |= BIT1; // Interrupts when you press button
    P1IE |= BIT1; // Enable interrupt on button pin
    P1IFG &= BIT1; // Clear interrupt flag
    
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
#pragma vector = PORT1_VECTOR
__interrupt void PORT_1(void)
{
    press = 1;
    i = 0; // Reset timing counter

    P1IFG &= ~BIT1; // Clear interrupt flag
}

// Interrupt subroutine
// Called at every timer interval
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer_B0(void)
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

### MSP430FR5529
//---------------------------------------------------------------------------------------

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

### MSP430FR5994
//---------------------------------------------------------------------------------------

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

### MSP430FR6989
//---------------------------------------------------------------------------------------

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
    P1REN |= BIT1; // Connects the on-board resistor to P1.1
    P1OUT = BIT1; // Sets up P1.1 as pull-up resistor
    
    P1IES |= BIT1; // Interrupts when you press button
    P1IE |= BIT1; // Enable interrupt on button pin
    P1IFG &= BIT1; // Clear interrupt flag
    
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

## Extra Work Reset Button on MSP430FR5529
//---------------------------------------------------------------------------------------

A custom reset button is created using the button attached to P2.1 and enabling it
as an interrupt using the PORT2_VECTOR. When the button is pressed, the interrupt
subroutine resets press and count just as they were when th program started. It also
changes i (the variable that tracks how long the duration variable is held down)
back to its 10 Hz value. Thus, a custom reset function is created.

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