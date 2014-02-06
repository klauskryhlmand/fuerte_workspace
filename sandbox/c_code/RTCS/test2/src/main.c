/*
 * main.c
 *
 *  Created on: Oct 29, 2013
 *      Author: daniel
 */
#include <inttypes.h>
#include <avr/io.h>

# define F_CPU 16000000UL // CPU speed on

#include <util/delay.h>
#include <avr/cpufunc.h>


#define sbi(a, b) (a) |= (1 << (b))
#define cbi(a, b) (a) &= ~(1 << (b))
#define toggleBit(thisRegister, bit) (thisRegister) ^= (1 << (bit))

int main(void)
{
	DDRB |= (1<<DDB0);

	while(1)
	{
		toggleBit(PORTB, PB0);
//		_delay_us(25000);   //wait for 25000 micro seconds

		//PORTB &= ~(1<<PB0);
		_delay_ms(500);   //wait for half second
		//PORTB |= (1<<PB0);
		//_delay_ms(500);   //wait for half second
	};
    return 0;

}

//int main(void)
//{
//	volatile int i;                         /* definer tæller variabel */
//    DDRB |= (1<<DDB0);              /* sæt PB0 (RoboCard LED) som udgang */
//    for (;;)                        /* start på uendelig løkke */
//    {
//    	//toggleBit(PORTB,PB0);
//        PORTB &= ~(1<<PB0);         /* tænd LED ved at sætte PB0 til GND */
//        for (i=0; i<30000; i++)    /* tæl op for at lave en kort pause */
//        {
//        //	_NOP();
//        }
//       // toggleBit(PORTB,PB0);
//        PORTB |= (1<<PB0);          /* sluk LED ved at sætte PB0 til VCC */
//        for (i=0; i<30000; i++)    /* tæl op for at lave en kort pause */
//        {
//		//	_NOP();
//		}
//    }
//    return 0;                       /* skal altid med i main funktionen */
//}
