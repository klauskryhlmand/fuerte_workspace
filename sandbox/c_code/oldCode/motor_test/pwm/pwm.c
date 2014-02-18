/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* PROJECT: 		pwm_mod
* AUTHOR: 		Daniel Lidekilde Ravn
* DESCRIPTION: 	See module specification file (.h-file).
*
* Change Log:
******************************************************************************
* Date    Id    Change
* 05/08/2011
*--------------------
*
*
*****************************************************************************/

/***************************** Include files *******************************/
#include <avr/io.h>
#include "pwm.h"
/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
INT16U new_speed = 0;
/*****************************   Functions   *******************************/

void pwm_init(void)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
	DDRD |= (1<<DDD4); // enable PD4 (led pin) as output
	DDRD |= (1<<DDD5); // enable PD5 (led pin) as output
	DDRD |= (1<<DDD6); // enable PD6 (led pin) as output
	DDRD |= (1<<DDD3); // enable PD3 (led pin) as output skal laves om til PD7

	TCCR0A|= (1<<WGM00);	//can be found i data sheath on page 102
	TCCR0A|= (1<<WGM01);
//	TCCR0A|= (1<<COM0A0);	//set OC0A to inverted output if add
	TCCR0A|= (1<<COM0A1);	//set OC0A to non-inverted output
//	TCCR0A|= (1<<COM0B0);	//set OC0A to inverted output if add
	TCCR0A|= (1<<COM0B1);	//set OC0B to inverted output

//	TCCR0B |= (1<<CS00);	// see page 105 for prescaler set up
	TCCR0B |= (1<<CS01);
//	TCCR0B |= (1<<CS02);

	SET_BIT_HIGH(PORTD,PD3);
	SET_BIT_HIGH(PORTD,PD4);
	OCR0A = 255;
	OCR0B = 255;
}

void set_pwm_speed_direction(INT8U speed,INT8U one_char)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
	if(one_char == 'r')
	{
		if(speed <= 99)
		{
			new_speed = speed;
			SET_BIT_LOW(PORTD,PD4);
			OCR0A = 255 - (new_speed*255)/100;
		}
		if(speed == 100)
		{
			SET_BIT_HIGH(PORTD,PD4);
			OCR0A = 255;
		}
		if(100 < speed && 200 >= speed)
		{
			new_speed = speed;
			SET_BIT_HIGH(PORTD,PD4);
			OCR0A = 255 - ((new_speed)*255)/100;
		}
	}
	else
	{
		if(speed <= 99)
		{
			new_speed = speed;
			SET_BIT_LOW(PORTD,PD3);
			OCR0B = 255 - (new_speed*255)/100;
		}
		if(speed == 100)
		{
			SET_BIT_HIGH(PORTD,PD3);
			OCR0B = 255;
		}
		if(100 < speed && 200 >= speed)
		{
			new_speed = speed;
			SET_BIT_HIGH(PORTD,PD3);
			OCR0B = 255 - ((new_speed)*255)/100;
		}
	}
}

/****************************** End Of Module *******************************/
