/**
 * @addtogroup Main
 * @brief     The main source code and control loop of the pannel.
 * @{
 * @file      main.c
 * @author    Iowa State University uCube team
 * @author    Andy Snawerdt
 * @author    Jeramie Vens
 * @author    Kris Burney
 * @date      Mar 15, 2015
 * @brief     Main software control loop
 * @copyright Copyright (C) 2015 ISU uCube team
 * @details   Main entry point of the program, contains main control loop.
 */

/*
    Master-MCU
    Copyright (C) 2015  Iowa State University uCube team

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <msp430.h>
#include <stdint.h>
#include "include/I2C/Panel_i2c.h"


void main_loop(void);

/**
 * @brief     Main entry point of the program.
 * @details   Initialize MCU, set up system tick and busy wait.
 */
int main(void)
{
	WDTCTL = WDTPW + WDTHOLD;						// Disable watchdog timers

	P2DIR = 0xFF;									// Initialize port 2 pin 6 to output
	P2SEL = 0;
	P2SEL2 = 0;
	P2OUT = 0;										// initializing the port pin to 0

	BCSCTL3 |= LFXT1S_2;							// Operate master clock on VLOCLK

	IFG1 &= ~OFIFG;									// Clear oscillator fault interrupt flag

	_bis_SR_register(SCG1 + SCG0);					// Wait 50 us for clock system to respond to change

	BCSCTL2 |= SELM_3 + DIVM_3;						//Source master clock from VLOCLK
    //__bis_SR_register(/*CPUOFF + */GIE);          // Enter LPM0 w/ interrupts
	P2OUT = 0x40;


    P1SEL = BIT1 | BIT2 | BIT4;	//Port 1 pin 5: System Clock pin 6: Serial in pin 7: Serial out
    P1SEL2 = BIT1 | BIT2 | BIT4;// Port 1 pin 5, pin 6, pin 7UCA0CTL1 = UCSWRST;

    UCA0CTL0 |= UCCKPH + UCMSB + UCSYNC ;//+ UCMODE_2;  // 3-pin, 8-bit SPI slave
	UCA0CTL1 |= UCSSEL_2;                     // SMCLK
	UCA0BR0 |= 0x02;                          // /2
	UCA0BR1 = 0;                              //
	//UCB0MCTL = 0;                             // No modulation
	UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
	__enable_interrupt();
	IFG2 = 0;
	IE2 = 0x01;//UCB0TXIE;						  	// enable interrupt



	while (1)
	{
		P2OUT ^= 0xC0;								// Toggle LED on and off
		_delay_cycles(1000);
	}
}
/**
 * @brief     Main control loop.
 * @details   This function gets called once every millisecond and contains the
 *            main software control loop.
 * @todo      Impliment this
 */
void main_loop(void)
{
#warning main loop not implimented


}

#pragma vector=USCIAB0RX_VECTOR
__interrupt void USCIA0RX_ISR(void)
{
	if(IFG2 | 0x01)
		UCA0TXBUF = UCA0RXBUF;
}

/// @}

