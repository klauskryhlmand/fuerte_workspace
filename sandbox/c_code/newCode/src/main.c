/*
 * main.c
 *
 *  Created on: Feb 27, 2014
 *      Author: Daniel Lindekilde Ravn
 */

#include <inttypes.h>
#include <avr/io.h>

#include "pwm/pwm.h"

# define F_CPU 16000000UL // clock frekvns   on

#include <util/delay.h>
#include <avr/cpufunc.h>

//#include <avr/interrupt.h>

#include "inc/emp_type.h"
#include "inc/glob_def.h"

#include "scheduler/scheduler.h"



int main(void)
{
	pwm_init();
	schedulSetup();


	while(1)
	{
		scheduler();
	};

	return 0;

}


