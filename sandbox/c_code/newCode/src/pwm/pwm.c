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
INT8U current_speed_left = 0;
INT8U current_speed_right = 0;
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

	DDRA |= (1<<DDB2); // enable PA2 (led pin) as output for enable left
	DDRA |= (1<<DDB3); // enable PA3 (led pin) as output for enable right

	SET_BIT_LOW(PORTA,PA2);
	SET_BIT_LOW(PORTA,PA3);

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
	
	set_pwm_speed_direction(100,'r');
	set_pwm_speed_direction(100,'l');
	
}

INT8U get_current_speed(unsigned char side)
{
	if(side == 'r')
	{
		return current_speed_right;
	}
	else if (side == 'l') {
		return current_speed_left;
	}
	return 0;
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
		current_speed_right = speed;
		if(speed <= 99)
		{
			new_speed = speed;
			if(new_speed == 0)
			{
				new_speed = 1;
			}
			SET_BIT_LOW(PORTE,PE3);
			OCR1AL = ((((100 - new_speed) - ((100 - new_speed)/25) * 25) * 255 ) / 25);
			OCR1AH = ((100 - new_speed)/25);
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
//			if(new_speed == 100)
//			{
//				new_speed = 99;
//			}
			SET_BIT_HIGH(PORTE,PE3);
			OCR1AL = ((((100 - new_speed) - ((100 - new_speed)/25) * 25) * 255 ) / 25);
			OCR1AH = ((100 - new_speed)/25);
		}
	}
	else if (one_char == 'l')
	{
		current_speed_left = speed;
		if(speed <= 99)
		{
			new_speed = speed;
			if(new_speed == 0)
			{
				new_speed = 1;
			}
			SET_BIT_LOW(PORTE,PE4);
			OCR1BL =  ((((100 - new_speed) - ((100 - new_speed)/25) * 25) * 255 ) / 25);
			OCR1BH = (new_speed/25);
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
//			if(new_speed == 100)
//			{
//				new_speed = 99;
//			}
			SET_BIT_HIGH(PORTE,PE4);
			OCR1BL = ((((100 - new_speed) - ((100 - new_speed)/25) * 25) * 255 ) / 25);
			OCR1BH = ((100 - new_speed)/25);
		}
	}
}

/****************************** End Of Module *******************************/
