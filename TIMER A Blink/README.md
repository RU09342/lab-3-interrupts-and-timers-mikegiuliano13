# Lab 3: Timer A Blink

## General Structure

This program uses one timer module to blink two LEDs at different rates.
The program uses a count variable mechanism to trigger the green LED on every
other timer event. This means it blinks at half the rate of the red LED.

## Important Distinctions

Generally, differences were in the pinouts for pull-up resistors and buttons.
The MSP430FR2311 and MSP430FR5994 required the use of Timer B instead of Timer A.

### MSP430G2553
//---------------------------------------------------------------------------------------

// Loads configurations for all MSP430 boards
#include <msp430.h>

volatile unsigned int count = 0;

void frequencyCalc(int t);

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    P1DIR = BIT0 + BIT6; // Set P1.0 and P1.6 as output
	P1OUT = 0x00;
    
    //TA0CTL = Timer A0 chosen for use
    //TASSEL_2 Selects SMCLK as clock source
	//ID_2 is a clock pre-divider of 4
    //MC_1 Count-up mode
    TA0CCTL0 = CCIE;
    TA0CTL = TASSEL_2 + MC_1 + ID_2;
	
	frequencyCalc(1);    // sets timer frequency at 1 Hz

    __enable_interrupt(); // MUST BE ENABLED IN ADDITION TO GIE
    __bis_SR_register(LPM0 + GIE); // enable interrupts in LPM0

}

// Function that converts 10 MHz SMCLK to desired frequency
// Simply input the number of Hz you want as input parameter
void frequencyCalc(int t)
{
    int x;
    x = 250000 / t;
    TA0CCR0 = x; // ex. t = 10 --> (10^6 [Hz] / 4) / 25000 = 10 Hz
}

// Interrupt subroutine
// Called at every timer interval
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A0(void)
{

    // Set LED blink duration according to frequencyCalc
            P1OUT ^= BIT0; //Toggle LED
			
	if (count == 0)
        {
            P1OUT ^= BIT6; //Toggle green LED
            count++;  // Reset count
        }
    else
        count = 0;
			
}

### MSP430FR2311
//---------------------------------------------------------------------------------------

// Loads configurations for all MSP430 boards
#include <msp430.h>

volatile unsigned int count = 0;

void frequencyCalc(int t);

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
	
	PM5CTL0 &= ~LOCKLPM5; // Disables high-impedance mode

    P1DIR = BIT0; // Set P1.0 as output
	P2DIR = BIT0; // Set P2.0 as output
	P1OUT = 0x00;
	P2OUT = 0x00;
    
    //TB0CTL = Timer B0 chosen for use
    //TBSSEL_2 Selects SMCLK as clock source
	//ID_2 is a clock pre-divider of 4
    //MC_1 Count-up mode
    TB0CCTL0 = CCIE;
    TB0CTL = TBSSEL_2 + MC_1 + ID_2;
	
	frequencyCalc(1);    // sets timer frequency at 1 Hz

    __enable_interrupt(); // MUST BE ENABLED IN ADDITION TO GIE
    __bis_SR_register(LPM0 + GIE); // enable interrupts in LPM0

}

// Function that converts 10 MHz SMCLK to desired frequency
// Simply input the number of Hz you want as input parameter
void frequencyCalc(int t)
{
    int x;
    x = 250000 / t;
    TB0CCR0 = x; // ex. t = 10 --> (10^6 [Hz] / 4) / 25000 = 10 Hz
}

// Interrupt subroutine
// Called at every timer interval
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer_B0(void)
{

    // Set LED blink duration according to frequencyCalc
            P1OUT ^= BIT0; //Toggle LED
			
	if (count == 0)
        {
            P2OUT ^= BIT0; //Toggle green LED
            count++;  // Reset count
        }
    else
        count = 0;
			
}

### MSP430F5529
//---------------------------------------------------------------------------------------

// Loads configurations for all MSP430 boards
#include <msp430.h>

volatile unsigned int count = 0;

void frequencyCalc(int t);

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer

    P1DIR = BIT0; // Set P1.0 as output
	P4DIR = BIT7; // Set P4.7 as output
	P1OUT = 0x00;
	P4OUT = 0x00;
    
    //TA0CTL = Timer A0 chosen for use
    //TASSEL_2 Selects SMCLK as clock source
	//ID_2 is a clock pre-divider of 4
    //MC_1 Count-up mode
    TA0CCTL0 = CCIE;
    TA0CTL = TASSEL_2 + MC_1 + ID_2;
	
	frequencyCalc(1);    // sets timer frequency at 1 Hz

    __enable_interrupt(); // MUST BE ENABLED IN ADDITION TO GIE
    __bis_SR_register(LPM0 + GIE); // enable interrupts in LPM0

}

// Function that converts 10 MHz SMCLK to desired frequency
// Simply input the number of Hz you want as input parameter
void frequencyCalc(int t)
{
    int x;
    x = 250000 / t;
    TA0CCR0 = x; // ex. t = 10 --> (10^6 [Hz] / 4) / 25000 = 10 Hz
}

// Interrupt subroutine
// Called at every timer interval
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A0(void)
{

    // Set LED blink duration according to frequencyCalc
            P1OUT ^= BIT0; //Toggle LED
			
	if (count == 0)
        {
            P4OUT ^= BIT7; //Toggle green LED
            count++;  // Reset count
        }
    else
        count = 0;
			
}

### MSP430FR5994
//---------------------------------------------------------------------------------------

// Loads configurations for all MSP430 boards
#include <msp430.h>

volatile unsigned int count = 0;

void frequencyCalc(int t);

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
	
	PM5CTL0 &= ~LOCKLPM5; // Disables high-impedance mode

    P1DIR = BIT0 + BIT1; // Set P1.0 and P1.1 as output
	P1OUT = 0x00;
    
    //TB0CTL = Timer B0 chosen for use
    //TBSSEL_2 Selects SMCLK as clock source
	//ID_2 is a clock pre-divider of 4
    //MC_1 Count-up mode
    TB0CCTL0 = CCIE;
    TB0CTL = TBSSEL_2 + MC_1 + ID_2;
	
	frequencyCalc(1);    // sets timer frequency at 1 Hz

    __enable_interrupt(); // MUST BE ENABLED IN ADDITION TO GIE
    __bis_SR_register(LPM0 + GIE); // enable interrupts in LPM0

}

// Function that converts 10 MHz SMCLK to desired frequency
// Simply input the number of Hz you want as input parameter
void frequencyCalc(int t)
{
    int x;
    x = 250000 / t;
    TB0CCR0 = x; // ex. t = 10 --> (10^6 [Hz] / 4) / 25000 = 10 Hz
}

// Interrupt subroutine
// Called at every timer interval
#pragma vector = TIMER0_B0_VECTOR
__interrupt void Timer_B0(void)
{

    // Set LED blink duration according to frequencyCalc
            P1OUT ^= BIT0; //Toggle LED
			
	if (count == 0)
        {
            P1OUT ^= BIT1; //Toggle green LED
            count++;  // Reset count
        }
    else
        count = 0;
			
}

### MSP430FR6989
//---------------------------------------------------------------------------------------

// Loads configurations for all MSP430 boards
#include <msp430.h>

volatile unsigned int count = 0;

void frequencyCalc(int t);

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // Stop watchdog timer
	
	PM5CTL0 &= ~LOCKLPM5;

    P1DIR = BIT0; // Set P1.0 as output
	P9DIR = BIT7; // Set P9.7 as output
	P1OUT = 0x00;
	P9OUT = 0x00;
    
    //TA0CTL = Timer A0 chosen for use
    //TASSEL_2 Selects SMCLK as clock source
	//ID_2 is a clock pre-divider of 4
    //MC_1 Count-up mode
    TA0CCTL0 = CCIE;
    TA0CTL = TASSEL_2 + MC_1 + ID_2;
	
	frequencyCalc(1);    // sets timer frequency at 1 Hz

    __enable_interrupt(); // MUST BE ENABLED IN ADDITION TO GIE
    __bis_SR_register(LPM0 + GIE); // enable interrupts in LPM0

}

// Function that converts 10 MHz SMCLK to desired frequency
// Simply input the number of Hz you want as input parameter
void frequencyCalc(int t)
{
    int x;
    x = 250000 / t;
    TA0CCR0 = x; // ex. t = 10 --> (10^6 [Hz] / 4) / 25000 = 10 Hz
}

// Interrupt subroutine
// Called at every timer interval
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A0(void)
{

    // Set LED blink duration according to frequencyCalc
            P1OUT ^= BIT0; //Toggle LED
			
	if (count == 0)
        {
            P9OUT ^= BIT7; //Toggle green LED
            count++;  // Reset count
        }
    else
        count = 0;
			
}