/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* PROJECT: 		motor_test
* AUTHOR: 		Daniel Lidekilde Ravn
* DESCRIPTION: 	See module specification file (.h-file).
*
* Change Log:
******************************************************************************
* Date    Id    Change
* 11/08/2011
*--------------------
* 
* 
*****************************************************************************/

/***************************** Include files *******************************/
#include "pwm/pwm.h"
#include "avr/io.h"
#include "util/delay.h"
/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
INT8U dumme = 0;
/*****************************   Functions   *******************************/

int main(void)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
	DDRB |= (1<<DDB0)|(1<<DDB1)|(1<<DDB2);
	DDRD |= (1<<DDD2);
	pwm_init();
//	PORTB |= (1<<PB0);
//	SET_BIT_HIGH(PORTD,PD3);
//	PORTB &= ~(1<<PB0);
	PORTB |= (1<<PB1);
	PORTB |= (1<<PB2);
	PORTD |= (1<<PD2);
	set_pwm_speed_direction(99,'r');
	while(1)
	{
		PORTB ^= (1<<PB0);

		for(INT8U j = 100; j <= 200; j++)
		{
			for(INT32U i = 0; i < 100000; i++)
			{
				if(i == 99999)
				{
					set_pwm_speed_direction(j,'r');
//					PORTB |= (1<<PB0);
				}
			}
		}

		PORTB ^= (1<<PB0);

		for(INT8U j = 200; j > 0; j--)
		{
			for(INT32U i = 0; i < 100000; i++)
			{
				if(i == 99999)
				{
					set_pwm_speed_direction(j,'r');
//					PORTB ^= (1<<PB0);
				}
			}
		}

		PORTB ^= (1<<PB0);

		for(INT8U j = 0; j < 101; j++)
		{
			for(INT32U i = 0; i < 100000; i++)
			{
				if(i == 99999)
				{
					set_pwm_speed_direction(j,'r');
//					PORTB &= ~(1<<PB0);
				}
			}
		}

	}

}


/****************************** End Of Module *******************************/
