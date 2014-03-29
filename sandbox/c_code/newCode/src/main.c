/*
 * main.c
 *
 *  Created on: Feb 27, 2014
 *      Author: Daniel Lindekilde Ravn
 */

#include <inttypes.h>
#include <avr/io.h>

#include "pwm/pwm.h"

#include "encoder/encoder.h"

# define F_CPU 16000000UL // clock frekvns   on

#include "uart/avr_serial.h"

#include "inc/emp_type.h"
#include "inc/glob_def.h"

#include "scheduler/scheduler.h"


int main(void)
{
	encoder_init();
	pwm_init();
	schedulSetup();
	serial_init();

	serial_tx('B');
	serial_tx('o');
	serial_tx('o');
	serial_tx('t');
	serial_tx('i');
	serial_tx('n');
	serial_tx('g');
	serial_tx('\n');


	while(1)
	{

		scheduler();
	};

	return 0;

}


