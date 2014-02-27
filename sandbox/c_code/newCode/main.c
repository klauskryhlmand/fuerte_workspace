/*
 * main.c
 *
 *  Created on: Feb 27, 2014
 *      Author: daniel
 */

#include <inttypes.h>
#include <avr/io.h>

# define F_CPU 16000000UL // CPU speed on

#include <util/delay.h>
#include <avr/cpufunc.h>

#include "avr/interrupt.h"

#define sbi(a, b) (a) |= (1 << (b))
#define cbi(a, b) (a) &= ~(1 << (b))

#define SET_BIT_LOW(word,bit_number)    word &= ~(0x01 << bit_number)
#define SET_BIT_HIGH(word,bit_number)   word |=  (1<< bit_number)

#define TEST_BIT_HIGH(word,bit_number)  (word & (0x01 << bit_number))
#define TEST_BIT_LOW(word,bit_number)   (!(TEST_BIT_HIGH(word,bit_number)))

#define toggleBit(thisRegister, bit) (thisRegister) ^= (1 << (bit))

int timer_tick = 0;

void timer1_init(void)
{
	TCCR1B |= (1<<WGM12) | (1<<WGM13) | (1<<CS12) | (1<<CS10); // WGM12 enables CTC, sets prescaler to 1024
	TIMSK1 |= (1<<OCIE1A); //| (1<<ICES1); // enable comper on OCR1A


	OCR1A = 20000;
	sei();
	//	SREG |= (1<<7);
}

void encoder_init(void)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
	EICRA |= (1<<ISC00) | (1<<ISC01);	// sets INT0 on pin PD2 to interrupt on flank to high
	EICRA |= (1<<ISC10) | (1<<ISC11);	// sets INT1 on pin PD3 to interrupt on flank to high
	EIMSK |= (1<<INT0) | (1<<INT1);		// enables interrupt for INT0 and INT1

	SREG |= (1<<7);	// //enables global interrupt
}

ISR(TIMER1_COMPA_vect)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
//	timer_tick++;
	toggleBit(PORTE, PE6);
//	TIMSK1 |= (1<<OCIE1A);
//	TIFR1 |= (1<<OCF1A);
//	OCR1A = 20000;
//	TIFR1 &= ~(1<<OCF1A);
// sei();
}

int main(void)
{
	DDRE |= (1<<DDE6);
	DDRE |= (1<<DDE5);

	toggleBit(PORTE,PE5);

//	encoder_init();
	timer1_init();


	while(1)
	{

		if (TEST_BIT_HIGH(TIFR1,OCF1A)) {

		}

//		toggleBit(PORTE,PE6);
//		if(timer_tick > 5)
//		{
//			_delay_ms(1000);
//			toggleBit(PORTE, PE6);
//			timer_tick = 0;
//		}
//		_delay_us(25000);   //wait for 25000 micro seconds

		//PORTB &= ~(1<<PB0);
//		_delay_ms(1000);   //wait for half second
		//PORTB |= (1<<PB0);
		//_delay_ms(500);   //wait for half second
	};
    return 0;

}


