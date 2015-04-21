#include <msp430.h> 
//#include <intrinsics.h>

volatile unsigned short values[24];
void start_transmition();
void led_to_buffer(void);
volatile unsigned char sending_done = 0;
union
{
	unsigned char data[36];
	struct __attribute__((__packed__))
	{
		unsigned led01 : 12;
		unsigned led02 : 12;
		unsigned led03 : 12;
		unsigned led04 : 12;
		unsigned led05 : 12;
		unsigned led06 : 12;
		unsigned led07 : 12;
		unsigned led08 : 12;
		unsigned led09 : 12;
		unsigned led10 : 12;
		unsigned led11 : 12;
		unsigned led12 : 12;
		unsigned led13 : 12;
		unsigned led14 : 12;
		unsigned led15 : 12;
		unsigned led16 : 12;
		unsigned led17 : 12;
		unsigned led18 : 12;
		unsigned led19 : 12;
		unsigned led20 : 12;
		unsigned led21 : 12;
		unsigned led22 : 12;
		unsigned led23 : 12;
		unsigned led24 : 12;
	};
}	tlc_data;
volatile unsigned char buffer[36];
unsigned char led_data[24];

unsigned char BYTE_SEND =  0x00;
/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    int i=0,j=0,k=1, dir=1;
    long m = 0;

    P1OUT |= BIT3;				//XLAT Port 3 Pin 0
    P1DIR |= BIT3;				//XLAT PORT 3 Pin 0
    P1SEL = BIT1 | BIT2 | BIT4;	//Port 1 pin 5: System Clock pin 6: Serial in pin 7: Serial out
    P1SEL2 = BIT1 | BIT2 | BIT4;// Port 1 pin 5, pin 6, pin 7
    P1OUT |= BIT5;				// Port 2 pin 5
    P1DIR |= BIT5;				// Port 2 pin 5

    UCA0CTL1 = UCSWRST;
    UCA0CTL0 |= UCCKPH + UCMSB + UCMST + UCSYNC;  // 3-pin, 8-bit SPI master
	UCA0CTL1 |= UCSSEL_2;                     // SMCLK
	UCA0BR0 |= 0x02;                          // /2
	UCA0BR1 = 0;                              //
	//UCB0MCTL = 0;                             // No modulation
	UCA0CTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
	__enable_interrupt();
	IFG2 = 0;
	IE2 = 0x02;//UCB0TXIE;						  	// enable interrupt

   // P1OUT &= ~BIT5;

	while(1)
	{
		for(j=0; j<24; j++)
		{
			led_data[j] = 0x00;
		}
		switch(i)
		{
		case 0:
			led_data[0] = 0xFF;
			led_data[3] = 0xFF;
			led_data[6] = 0xFF;
			led_data[9] = 0xFF;
			led_data[12] = 0xFF;
			led_data[15] = 0xFF;
			led_data[18] = 0xFF;
			led_data[21] = 0xFF;
			i++;
			//break;
		case 1:
			led_data[0] = 0xFF;
			led_data[3] = 0xFF;
			led_data[6] = 0xFF;
			led_data[9] = 0xFF;
			led_data[12] = 0xFF;
			led_data[15] = 0xFF;
			led_data[18] = 0xFF;
			led_data[21] = 0xFF;
			led_data[1] = 0xFF;
			led_data[4] = 0xFF;
			led_data[7] = 0xFF;
			led_data[10] = 0xFF;
			led_data[13] = 0xFF;
			led_data[16] = 0xFF;
			led_data[19] = 0xFF;
			led_data[22] = 0xFF;
			i++;
			//break;
		case 2:
			led_data[1] = 0xFF;
			led_data[4] = 0xFF;
			led_data[7] = 0xFF;
			led_data[10] = 0xFF;
			led_data[13] = 0xFF;
			led_data[16] = 0xFF;
			led_data[19] = 0xFF;
			led_data[22] = 0xFF;
			i++;
			//break;
		case 3:
			led_data[1] = 0xFF;
			led_data[4] = 0xFF;
			led_data[7] = 0xFF;
			led_data[10] = 0xFF;
			led_data[13] = 0xFF;
			led_data[16] = 0xFF;
			led_data[19] = 0xFF;
			led_data[22] = 0xFF;
			led_data[2] = 0xFF;
			led_data[5] = 0xFF;
			led_data[8] = 0xFF;
			led_data[11] = 0xFF;
			led_data[14] = 0xFF;
			led_data[17] = 0xFF;
			led_data[20] = 0xFF;
			led_data[23] = 0xFF;
			i++;
			//break;
		case 4:
			led_data[2] = 0xFF;
			led_data[5] = 0xFF;
			led_data[8] = 0xFF;
			led_data[11] = 0xFF;
			led_data[14] = 0xFF;
			led_data[17] = 0xFF;
			led_data[20] = 0xFF;
			led_data[23] = 0xFF;
			i++;
			//break;
		case 5:
			led_data[2] = 0xFF;
			led_data[5] = 0xFF;
			led_data[8] = 0xFF;
			led_data[11] = 0xFF;
			led_data[14] = 0xFF;
			led_data[17] = 0xFF;
			led_data[20] = 0xFF;
			led_data[23] = 0xFF;
			led_data[0] = 0xFF;
			led_data[3] = 0xFF;
			led_data[6] = 0xFF;
			led_data[9] = 0xFF;
			led_data[12] = 0xFF;
			led_data[15] = 0xFF;
			led_data[18] = 0xFF;
			led_data[21] = 0xFF;
			i=0;
			//break;

		}
		led_to_buffer();
		sending_done = 0;
		start_transmition();
		while(!sending_done);
		sending_done = 0;
		P1OUT &= ~BIT5;
		//while(1);
		BYTE_SEND += 1;
		//BYTE_SEND = 0xCC;
		for(m=0; m<40000 ; m++);
	}
	while(1);

	return 0;
}


void start_transmition(void)
{
	P1OUT &= (~BIT3); // Select Device
	UCA0TXBUF = BYTE_SEND; //buffer[0];// tlc_data.data[0];	// Send first byte
	return;
}

void led_to_buffer(void)
{
	int i, j=0, k=0;
	for(i=0;i<36;i++)
	{
		switch(j)
		{
		case 0:
			buffer[i] = led_data[k];
			k++;
			j++;
			break;
		case 1:
			buffer[i] = (led_data[k]>>4) & 0x0F;
			j++;
			break;
		case 2:
			buffer[i] = led_data[k]<<4;
			j=0;
			k++;
			break;
		}
	}
}

#pragma vector=USCIAB0TX_VECTOR
__interrupt void USCIA0TX_ISR(void)
{
	static int i = 1;
	if(i < 36*3+1)
	{
		_delay_cycles(10);
		UCA0TXBUF = BYTE_SEND;//buffer[i];
		i++;
	}
	else
	{
		IFG2 = 0;
		_delay_cycles(100);
		sending_done = 1;
		P1OUT |= (BIT3); // Unselect Device
		i = 1;
	}
}
