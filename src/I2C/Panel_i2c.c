/**
 * @addtogroup I2C
 * @{
 * @file      Panel_i2c.c
 * @author    Iowa State University uCube team
 * @author    Andy Snawerdt
 * @author    Jeramie Vens
 * @author    Kris Burney
 * @date      Mar 28, 2015
 * @brief     I2C functions
 * @copyright Copyright (C) 2015 ISU uCube team
 * @details   This file contains all the I2C functions such as initialization,
 *            sending, and recieving data from the I2C bus.
 */

/*
    Panel-display
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
#include "../../include/I2C/Panel_i2c.h"


static volatile unsigned char flag = 0;
static volatile char* buffer;
static volatile unsigned char i = 0;
static volatile uint16_t latch_addr;
static volatile uint8_t slave_addr;
volatile enum {
	STATE_START_LOW,
	STATE_LATCH_LOW,
	STATE_START_HIGH,
	STATE_RECEIVE,
}rx_state;

/**
 * @brief     Initialization for I2C.
 * @details   Initializes the I2C0 bus by enabling the chosen I2C module. Resets
 *            the module, enables the master module, uses the system clock,
 *            and sets the data transfer rate.
 */
void I2C_init()
{

	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	P1SEL |= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0
	P1SEL2|= BIT6 + BIT7;                     // Assign I2C pins to USCI_B0
	//while (UCB0CTL1 & UCTXSTP);               // Ensure stop condition got sent
	UCB0CTL1 |= UCSWRST;                      // Enable SW reset
	UCB0CTL0 = UCMST + UCMODE_3 + UCSYNC;     // I2C Master, synchronous mode
	UCB0CTL1 = UCSSEL_2 + UCSWRST;            // Use SMCLK, keep SW reset
    UCB0BR0 = 12;                             // fSCL = SMCLK/12 = ~100kHz
    UCB0BR1 = 0;

}

unsigned char volatile * I2C_receive(uint8_t slave_address, char *array, uint16_t latch) //From FRAM, Change name of function (conflicting with Master-MCU I2C declaration)
{
	rx_state = STATE_START_LOW;
	slave_addr = slave_address;
    UCB0I2CSA = slave_addr;                   // Slave Address Selected
    UCB0CTL1 &= ~UCSWRST;                     // Clear SW reset, resume operation

    UCB0CTL1 |= UCTXSTT;                      // I2C start condition

	IE2 |= UCB0TXIE;						  // Enable TX interrupt
	latch_addr = latch;
    UCB0TXBUF = latch_addr >> 8;			  // Sends latch address MSB



    //UCNACKIFG

    //while (UCB0CTL1 & UCTXSTP);               // Ensure stop condition got sent

    //while (UCB0CTL1 & UCTXSTT);               // Ensure start condition got sent
//    UCB0CTL1 |= UCTXSTP;                      // I2C stop condition
    buffer = array;
    flag = 0;
    i = 0;
    IE2 |= UCB0RXIE;                          // Enable RX interrupt
    return &flag;
}


// USCI_B0 Data ISR
#pragma vector = USCIAB0TX_VECTOR
__interrupt void USCIAB0TX_ISR(void)
{
	switch(rx_state)
	{
	case STATE_LATCH_LOW:
		UCB0TXBUF = latch_addr & 0x00FF;		  // Send the LSB of the latch location
		rx_state = STATE_START_HIGH;					  // Change to the start_HIGH state
		break;
	case STATE_START_HIGH :
	    UCB0I2CSA = slave_addr;                   // Slave Address Selected
	    rx_state = STATE_RECEIVE;						  // Change to the receive state
		break;
	case STATE_RECEIVE :
		*buffer++ = UCB0RXBUF;                      // Get RX data
		rx_state = STATE_START_HIGH;
		i++;
		if(i > 63)
		{
			i = 0;
			flag = 1;
			IE2 &= ~UCB0RXIE;                       // Enable RX interrupt
		}
	}
//	if(UCB0RXIFG)
//	{
//		*buffer++ = UCB0RXBUF;                      // Get RX data
//		i++;
//		if (i > 63)
//		{
//			i = 0;
//			flag = 1;
//			IE2 &= ~UCB0RXIE;                       // Enable RX interrupt
//		}
//		//__bic_SR_register_on_exit(CPUOFF);        // Exit LPM0
//		}
//	else if(UCB0TXIFG)
//	{
//		UCB0TXBUF = latch_addr & 0x00FF;	  // Sends latch address LSB
//	}
}
/// @}

