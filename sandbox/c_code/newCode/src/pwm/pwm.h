/*
 * pwm.h
 *
 *  Created on: 05/08/2011
 *      Author: Computer
 */

#ifndef PWM_H_
#define PWM_H_

#include "../inc/binary.h"
#include "../inc/emp_type.h"
#include "../inc/glob_def.h"

void pwm_init(void);
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/

void set_pwm_speed_direction(INT8U speed,INT8U one_char);
/*****************************************************************************
*   Input    :
*   Output   :
*   Function :
******************************************************************************/

INT8U get_current_speed(unsigned char side);

#endif /* PWM_H_ */
