/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* PROJECT: 		encoder_mod
* AUTHOR: 		Daniel Lidekilde Ravn
* DESCRIPTION: 	See module specification file (.h-file).
*
* Change Log:
******************************************************************************
* Date    Id    Change
* 21/03/2014
*--------------------
* 
* 
*****************************************************************************/

/***************************** Include files *******************************/
#include "avr/io.h"
#include "avr/interrupt.h"
#include "encoder.h"
#include "../inc/binary.h"
/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
INT16U moved_right = 0;
INT16U moved_left = 0;
INT16U Dummy = 0;
/*****************************   Functions   *******************************/

void encoder_init(void)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
	EICRA |= (1<<ISC00) | (1<<ISC01);	// sets INT0 on pin PD0 to interrupt on flank to high
	EICRA |= (1<<ISC10) | (1<<ISC11);	// sets INT1 on pin PD1 to interrupt on flank to high
	EIMSK |= (1<<INT0) | (1<<INT1);		// enables interrupt for INT0 and INT1

	SREG |= (1<<7);	// //enables global interrupt
}


INT16U get_left(void)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
	Dummy = moved_left;
	moved_left = 0;
	return Dummy;
}

INT16U get_right(void)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
	Dummy = moved_right;
	moved_right = 0;
	return Dummy;
}




ISR(INT0_vect)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
//	cli();
	if(TEST_BIT_HIGH(PINA,PA0))
	{
		moved_left = moved_left + 1;
	}
	if(TEST_BIT_LOW(PINA,PA0))
	{
		moved_left = moved_left - 1;
	}
//	sei();
}

ISR(INT1_vect)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
//	cli();
	if(TEST_BIT_HIGH(PINA,PA1))
	{
		moved_right = moved_right - 1;
	}
	if(TEST_BIT_LOW(PINA,PA1))
	{
		moved_right = moved_right + 1;
	}
//	sei();
}

/****************************** End Of Module *******************************/
