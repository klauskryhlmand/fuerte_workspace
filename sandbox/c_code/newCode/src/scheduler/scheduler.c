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


INT16U timer_tick = 0;
INT16U systikOverflowCompare = 0;

INT8U numberOfTask = 0;
INT8S nextTask = -1;
BOOLEAN schedulerNotRun = FALSE;

struct task (*allTask);


void timer1_init(void)
{
//	TCCR1B |= (1<<WGM12) | (1<<COM1A1) /** | (1<<WGM13) | (1<<CS12)**/ | (1<<CS10); // current setting is no prescaler table for possibly settings can
//	// be found in data sheet for AT90CAN128 on page 139
//	TIMSK1 |= (1<<OCIE1A); //| (1<<ICES1); // enable comper on OCR1A
//
//	OCR1A = 16000;
//	sei();
	//	SREG |= (1<<7);
}

void timer0_init(void)
{
	TCCR0A |= (1<<WGM01) | (1<<COM0A1) | (1<<CS00) /** | (1<<CS02); **/ | (1<<CS01); // current setting is no prescaler table for possibly settings can
	// be found in data sheet for AT90CAN128 on page 139
	TIMSK0 |= (1<<OCIE0A); // enable comper on OCR1A

	OCR0A = 250;
	sei();
	//	SREG |= (1<<7);
}

ISR(TIMER1_COMPA_vect)
/*****************************************************************************
 *   Input    :
 *   Output   :
 *   Function :
 ******************************************************************************/
{
//	cli();
//	++timer_tick;
//	schedulerNotRun = TRUE;
//	sei();
}

ISR(TIMER0_COMP_vect)
/*****************************************************************************
 *   Input    :
 *   Output   :
 *   Function :
 ******************************************************************************/
{
//	cli();
	++timer_tick;
	schedulerNotRun = TRUE;
//	sei();
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
//	serial_tx_string(intToCharArray(42));
	serial_tx('e');
//	TOGGLE_BIT(PORTE,PD5);
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
	pwmTest.time = 5000;
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
			INT16U remainder = 0;
			for (int i = 0; i < numberOfTask; ++i) {
				remainder = 0xFFFF - allTask[i].nextRun;
//				if (remainder > allTask[i].time) {
					remainder = remainder % allTask[i].time;
//				}
				allTask[i].nextRun = allTask[i].time - remainder;
			}
			serial_tx('t');
		}

		// schedules next task or runs currently scheduled task
		if (nextTask == -1) {
			INT16U theNext = 0xFFFF;
			for (int i = 0; i < numberOfTask; i++) {
				if(allTask[i].nextRun + allTask[i].time < theNext) {
					nextTask = i;
					theNext = allTask[i].nextRun + allTask[i].time;
				}
			}
			serial_tx('d');
			allTask[nextTask].nextRun += allTask[nextTask].time;
		}
		else {
			if (timer_tick > allTask[nextTask].nextRun) {
				(*allTask[nextTask].functionPtr)();
				nextTask = -1;
			}
		}
	}
}

//				if (i == nextTask) {
//					if (allTask[i].nextRun > 0xF000) {
//						remainder = 0xFFFF - allTask[i].nextRun;
//						if (remainder > allTask[i].time) {
//							remainder = remainder % allTask[i].time;
//						}
//						allTask[i].nextRun = allTask[i].time - remainder;
//					}
//				}else {
//					remainder = 0xFFFF - allTask[i].nextRun;
//					if (remainder > allTask[i].time) {
//						remainder = remainder % allTask[i].time;
//					}
//					allTask[i].nextRun = allTask[i].time - remainder;
//				}
//				if(0xFFFF > allTask[i].nextRun)
//				{
//				remainder = 0xFFFF - allTask[i].nextRun;
//				}

//		if(timer_tick > systikOverflowCompare)
//		{
//			systikOverflowCompare = timer_tick;
//		}
//		else {
//			INT16U remainder = 0;
//
//			for (int i = 0; i < numberOfTask; i++) {
//				remainder = 0xFFFF - allTask[i].nextRun;
//				if (remainder > allTask[i].time) {
//					remainder = remainder % allTask[i].time;
//				}
//				allTask[i].nextRun = allTask[i].time - remainder;
//			}
//			//			for (int i = 0; i < numberOfTask; i++) {
//			//				if (remainder > allTask[i].time) {
//			//					remainder = remainder % allTask[i].time;
//			//				}
//			//				allTask[i].nextRun = allTask[i].time - remainder;
//			//			}
//		}

