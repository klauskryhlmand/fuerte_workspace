/*
 * encoder.h
 *
 *  Created on: 10/08/2011
 *      Author: Computer
 */

#ifndef ENCODER_H_
#define ENCODER_H_

#include "../inc/binary.h"

void encoder_init(void);
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/

INT8S get_left(void);
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/

INT8S get_right(void);
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/

void timer1_init(void);

//void initTimer1 (float freq, long ps);
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/

#endif /* ENCODER_H_ */
