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
	DDRB |= (1<<DDB5); // enable PB5 (led pin) as output
	DDRB |= (1<<DDB6); // enable PB6 (led pin) as output
	DDRE |= (1<<DDE3); // enable PE3 (led pin) as output
	DDRE |= (1<<DDE4); // enable PE4 (led pin) as output

	TCCR1A|= (1<<WGM10);	//  Phase Correct
	TCCR1A|= (1<<WGM11);	// 10 bit
//	TCCR1A|= (1<<WGM12); 	// CTC mode if only the only set
	TCCR1A|= (1<<COM1A1);	
	TCCR1A|= (1<<COM1B1);	
	TCCR1A|= (1<<COM1C1);	

/*	TCCR3A|= (1<<WGM30);	
	TCCR3A|= (1<<WGM31);
	TCCR3A|= (1<<COM3A1);	
	TCCR3A|= (1<<COM3B1);	
	TCCR3A|= (1<<COM3C1);	
*/
	
	TCCR1B |= (1<<CS10);
//	TCCR1B |= (1<<CS11);	// clock/1024 (15khz) see page 139
//	TCCR1B |= (1<<CS12);
/*
	TCCR3B |= (1<<CS30);	
//	TCCR3B |= (1<<CS31);	// clock/1024 (15khz) see page 139
	TCCR3B |= (1<<CS32);
*/
	SET_BIT_HIGH(PORTE,PE3);
	SET_BIT_HIGH(PORTE,PE4);

	OCR1AL = 255;	//PB5  // with current can only get very low pwm must changes to use HIGH regiset to OCR1AH
	OCR1AH = 3;
	OCR1BL = 255;	//PB6
	OCR1BH = 3;
	OCR1CL = 255;

	//OCR3AL = 255;	//PE3
	//OCR3BL = 255;	//PE4
	//OCR3CL = 255;
	
	
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
			SET_BIT_LOW(PORTE,PE3);
			OCR1AL = ((new_speed - (new_speed/33) * 33) * 255 ) / 33;
			OCR1AH = new_speed/33;
		}
		else if(speed == 100)
		{
			SET_BIT_HIGH(PORTE,PE3);
			OCR1AL = 255;
			OCR1AH = 3;
		}
		else if(100 < speed && 200 >= speed)
		{
			new_speed = speed-100;
			SET_BIT_HIGH(PORTE,PE3);
			OCR1AL = ((new_speed - (new_speed/33) * 33) * 255 ) / 33;
			OCR1AH = new_speed/33;
		}
	}
	else if (one_char == 'l') // why add this check Klaus ?
	{
		if(speed <= 99)
		{
			new_speed = speed;
			SET_BIT_LOW(PORTE,PE4);
			OCR1BL = ((new_speed - (new_speed/33) * 33) * 255 ) / 33;
			OCR1BH = new_speed/33;
		}
		else if(speed == 100)
		{
			SET_BIT_HIGH(PORTE,PE4);
			OCR1BL = 255;
			OCR1BH = 3;
		}
		else if(100 < speed && 200 >= speed)
		{
			new_speed = speed-100;
			SET_BIT_HIGH(PORTE,PE4);
			OCR1BL = ((new_speed - (new_speed/33) * 33) * 255 ) / 33;
			OCR1BH = new_speed/33;
		}
	}
}

/****************************** End Of Module *******************************/
