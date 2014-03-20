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
* 10/08/2011
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
INT8S moved_right = 0;
INT8S moved_left = 0;
INT8S Dummy = 0;
/*****************************   Functions   *******************************/

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


INT8S get_left(void)
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

INT8S get_right(void)
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
	if(TEST_BIT_HIGH(PINC,PC3))
	{
		moved_right--;
	}
	if(TEST_BIT_LOW(PINC,PC3))
	{
		moved_right++;
	}

}

ISR(INT1_vect)
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/
{
	if(TEST_BIT_HIGH(PINB,PB2))
	{
		moved_left--;
	}
	if(TEST_BIT_LOW(PINB,PB2))
	{
		moved_left++;
	}
}

/****************************** End Of Module *******************************/
