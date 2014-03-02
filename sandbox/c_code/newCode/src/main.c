/*
 * main.c
 *
 *  Created on: Feb 27, 2014
 *      Author: Daniel Lindekilde Ravn
 */

#include <inttypes.h>
#include <avr/io.h>

# define F_CPU 16000000UL // clock frekvns   on

#include <util/delay.h>
#include <avr/cpufunc.h>

#include <avr/interrupt.h>

#include "inc/emp_type.h"
#include "inc/glob_def.h"
//#include "inc/emp_type.h"
//#include "inc/glob_def.h"

#define sbi(a, b) (a) |= (1 << (b))
#define cbi(a, b) (a) &= ~(1 << (b))

#define SET_BIT_LOW(word,bit_number)    word &= ~(0x01 << bit_number)
#define SET_BIT_HIGH(word,bit_number)   word |=  (1<< bit_number)

#define TEST_BIT_HIGH(word,bit_number)  (word & (0x01 << bit_number))
#define TEST_BIT_LOW(word,bit_number)   (!(TEST_BIT_HIGH(word,bit_number)))

#define toggleBit(thisRegister, bit) (thisRegister) ^= (1 << (bit))


struct task{
	INT16U time;
	INT16U nextRun;
	void (*functionPtr)(void);
};



INT16U timer_tick = 0;
INT16U systikOverflowCompare = 0;

INT8U numberOfTask = 0;
INT8S nextTask = -1;
BOOLEAN schedulerNotRun = FALSE;

struct task (*allTask);


void timer1_init(void)
{
	TCCR1B |= (1<<WGM12) /** | (1<<WGM13) | (1<<CS12)**/ | (1<<CS10); // WGM12 enables CTC, sets prescaler to 1024
	TIMSK1 |= (1<<OCIE1A); //| (1<<ICES1); // enable comper on OCR1A

	OCR1A = 16000;
	sei();
	//	SREG |= (1<<7);
}

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

	SREG |= (1<<7);	//enables global interrupt
}


ISR(TIMER1_COMPA_vect)
/*****************************************************************************
 *   Input    :
 *   Output   :
 *   Function :
 ******************************************************************************/
{
	cli();
	timer_tick++;
	//	toggleBit(PORTE,PE5);
	schedulerNotRun = TRUE;
	TCNT1=0x00;
	sei();
}

void aliveTask(void)
{
	toggleBit(PORTE,PD6);
}

void aliveTask2(void)
{
	toggleBit(PORTE,PD5);
}


void schedulSetup()
{
	numberOfTask = 2; // must set number of task here

	struct task allTaks[numberOfTask];

	struct task aliveTaskStruck;
	aliveTaskStruck.time = 50;
	aliveTaskStruck.nextRun = 0;
	aliveTaskStruck.functionPtr = &aliveTask;

	struct task aliveTaskStruck2;
	aliveTaskStruck2.time = 2000;
	aliveTaskStruck2.nextRun = 0;
	aliveTaskStruck2.functionPtr = &aliveTask2;


	allTaks[0] = aliveTaskStruck;
	allTaks[1] = aliveTaskStruck2;

	allTask = allTaks;
}

void scheduler()
{
	if(schedulerNotRun == TRUE)
	{
		schedulerNotRun = FALSE;

		// deals with rescheduling task when timer_tick overflows
		if(timer_tick > systikOverflowCompare)
		{
			systikOverflowCompare = timer_tick;
		}
		else {
			systikOverflowCompare = timer_tick;
			INT16U remainder = 0;
			for (int i = 0; i < numberOfTask; ++i) {
//				if (allTask[i].nextRun > 0xF000) {
					remainder = 0xFFFF - allTask[i].nextRun;
					if (remainder > allTask[i].time) {
						remainder = remainder % allTask[i].time;
					}
					allTask[i].nextRun = allTask[i].time - remainder;
//				}
			}
		}

		// schedules next task or runs currently scheduled task
		if (nextTask == -1) {
			INT16U theNext = 0xFFFF;
			for (int i = 0; i < numberOfTask; ++i) {
				if(allTask[i].nextRun + allTask[i].time < theNext) {
					nextTask = i;
					theNext = allTask[i].nextRun + allTask[i].time;
				}
			}
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

int main(void)
{
	DDRE |= (1<<DDE6);
	DDRE |= (1<<DDE5);

	//	encoder_init();
	timer1_init();
	schedulSetup();


	while(1)
	{
				scheduler();
	};

	return 0;

}


