/*
 * scheduler.c
 *
 *  Created on: Mar 2, 2014
 *      Author: daniel
 */

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "../inc/glob_def.h"
#include "../inc/emp_type.h"
#include "../inc/binary.h"


#include "scheduler.h"

#include "../pwm/pwm.h"
#include "../inc/util.h"
#include "../uart/avr_serial.h"


#include <stdio.h>

INT16U timer_tick = 0;
INT16U systikOverflowCompare = 0;

INT8U numberOfTask = 0;
INT8S nextTask = -1;
BOOLEAN schedulerNotRun = FALSE;

struct task (*allTask);


void sendTikTime()
{
	INT16U timeCopy = timer_tick;
	unsigned char c = 'T';
	unsigned char l = 0xFF & timeCopy;
	timeCopy = timeCopy >> 4;
	unsigned char h = 0xFF & timeCopy;

	serial_tx(c);
	serial_tx(h);
	serial_tx(l);
}

//void timer1_init(void)
//{
//	TCCR1B |= (1<<WGM12) | (1<<COM1A1) /** | (1<<WGM13) | (1<<CS12)**/ | (1<<CS10); // current setting is no prescaler table for possibly settings can
//	// be found in data sheet for AT90CAN128 on page 139
//	TIMSK1 |= (1<<OCIE1A); //| (1<<ICES1); // enable comper on OCR1A
//
//	OCR1A = 16000;
//	sei();
//	SREG |= (1<<7);
//}

void timer0_init(void)
{
	TCCR0A |= (1<<WGM01) | (1<<COM0A1) | (1<<CS00) /** | (1<<CS02); **/ | (1<<CS01); // current setting is no prescaler table for possibly settings can
	// be found in data sheet for AT90CAN128 on page 139
	TIMSK0 |= (1<<OCIE0A); // enable comper on OCR1A

	OCR0A = 250;
	sei();
	//	SREG |= (1<<7);
}

//ISR(TIMER1_COMPA_vect)
///*****************************************************************************
// *   Input    :
// *   Output   :
// *   Function :
// ******************************************************************************/
//{
////	cli();
////	++timer_tick;
////	schedulerNotRun = TRUE;
////	sei();
//}

ISR(TIMER0_COMP_vect)
/*****************************************************************************
 *   Input    :
 *   Output   :
 *   Function :
 ******************************************************************************/
{
	cli();
	++timer_tick;
	schedulerNotRun = TRUE;
	sei();
}

void initAliveTasks()
{
	DDRE |= (1<<DDE6);
	DDRE |= (1<<DDE5);
}

void aliveTask(void)
{
	TOGGLE_BIT(PORTE,PD6);
}

void aliveTask2(void)
{
	serial_tx('a');
	serial_tx('l');
	serial_tx('i');
	serial_tx('v');
	serial_tx('e');

	serial_tx(' ');

	serial_tx('t');

	sendTikTime();

	serial_tx('\n');

}


INT8U pwmTestSpeed = 0;
void pwmtestTask()
{
	TOGGLE_BIT(PORTE,PD5);
	set_pwm_speed_direction(pwmTestSpeed,'r');
	set_pwm_speed_direction(pwmTestSpeed,'l');
	pwmTestSpeed = pwmTestSpeed + 10;
	if (pwmTestSpeed > 200) {
		pwmTestSpeed = 0;
	}
}

void schedulSetup()
{
	initAliveTasks();

	numberOfTask = 3; // must set number of task here

	struct task allTaks[numberOfTask];

	struct task aliveTaskStruck;
	aliveTaskStruck.time = 50;
	aliveTaskStruck.nextRun = 0;
	aliveTaskStruck.functionPtr = &aliveTask;

	struct task aliveTaskStruck2;
	aliveTaskStruck2.time = 2000;
	aliveTaskStruck2.nextRun = 0;
	aliveTaskStruck2.functionPtr = &aliveTask2;

	struct task pwmTest;
	pwmTest.time = 500;
	pwmTest.nextRun = 0;
	pwmTest.functionPtr = &pwmtestTask;


	allTaks[0] = aliveTaskStruck;
	allTaks[1] = aliveTaskStruck2;
	allTaks[2] = pwmTest;

	allTask = allTaks;
	timer_tick = 0;
	timer0_init();
}

void scheduler()
{
	if(schedulerNotRun == TRUE)
	{
		schedulerNotRun = FALSE;

		//		 deals with rescheduling task when timer_tick overflows
		if(timer_tick == 60000)
		{
			timer_tick = 0;
			serial_tx('d');
			INT16U remainder = 0;
			for (int i = 0; i < numberOfTask; ++i) {
				if(allTask[i].nextRun <= 6000)
				{
					remainder = 60000 - allTask[i].nextRun;
	//				if (remainder > allTask[i].time) {
						remainder = remainder % allTask[i].time;
	//				}
					allTask[i].nextRun = allTask[i].time - remainder;
				}else {
					allTask[i].nextRun = allTask[i].nextRun - 60000;
				}
			}
		}

		// schedules next task or runs currently scheduled task
		if (nextTask == -1) {
			INT16U theNext = 0xFFFF;
			for (int i = 0; i < numberOfTask; ++i) {
					if(allTask[i].nextRun + allTask[i].time < theNext) {	// when near to overflow can cause long task to go over there
						nextTask = i;										// by be scheduled many times current fix only alwos for 5s task delay
						theNext = allTask[i].nextRun + allTask[i].time;		// perment fix is need Daniel Lindekilde Ravn
					}
			}
			allTask[nextTask].nextRun += allTask[nextTask].time;
		}
		else {
			if (timer_tick > allTask[nextTask].nextRun) {
				(*allTask[nextTask].functionPtr)();
//				serial_tx('r');
				nextTask = -1;
			}
		}
	}
}


