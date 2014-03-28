/*
 * scheduler.c
 *
 *  Created on: Mar 2, 2014
 *      Author: daniel
 *     version: 0.1
 */

#include <inttypes.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#include "../inc/glob_def.h"
#include "../inc/emp_type.h"
#include "../inc/binary.h"


#include "scheduler.h"

#include "../pwm/pwm.h"
#include "../encoder/encoder.h"
#include "../uart/avr_serial.h"


INT16U timer_tick = 0;
INT16U systikOverflowCompare = 0;

INT8U numberOfTask = 0;
INT8S nextTask = -1;
BOOLEAN schedulerNotRun = FALSE;

struct task (*allTask);

unsigned char message[4];

INT8U desired_speed_left = 0;
INT8U desired_speed_right = 0;
unsigned char direction_left;
unsigned char direction_right;

#define twoEncoderTicklength 20
#define controlerTimeStep 1000 // can not be more than 1000. Will cause devices by 0


#define GET_8_LOW_BIT(word,out)    out = (0xFF & word)
#define GET_8_HIGH_BIT(word,out)    out = (0xFF & (value >> 8))

void uart_send_INT16S(INT16S value, unsigned char handle_part1, unsigned char handle_part2)
{
	unsigned char l;
	GET_8_LOW_BIT(value,l);
	unsigned char h;
	GET_8_HIGH_BIT(value,h);

	serial_tx(handle_part1);
	serial_tx(handle_part2);
	serial_tx(h);
	serial_tx(l);
}

void uart_send_INT16U(INT16U value, unsigned char handle_part1, unsigned char handle_part2)
{
	unsigned char l;
	GET_8_LOW_BIT(value,l);
	unsigned char h;
	GET_8_HIGH_BIT(value,h);

	serial_tx(handle_part1);
	serial_tx(handle_part2);
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
	TOGGLE_BIT(PORTE,PE6);
}

void aliveTask2(void)
{
//	TOGGLE_BIT(PORTE,PE5);
//	serial_tx('a');
//	serial_tx('l');
//	serial_tx('i');
//	serial_tx('v');
//	serial_tx('e');
//
//	uart_send_INT16U(timer_tick,'T','T');
//
//	uart_send_INT16U(get_left(),'E','L');
//
//	uart_send_INT16U(get_right(),'E','R');

}

//INT8U pwmTestSpeed = 0;
//void pwmtestTask()
//{
//	TOGGLE_BIT(PORTE,PE5);
//	set_pwm_speed_direction(100,'r');
//	set_pwm_speed_direction(100,'l');
//	pwmTestSpeed = pwmTestSpeed + 10;
//	if (pwmTestSpeed > 200) {
//		pwmTestSpeed = 0;
//	}
//}



INT16S storeEncoderLeft = 0;
INT16S storeEncoderRight = 0;

INT16S lastEnconderWantedLeft = 0;
INT16S lastEnconderWantedRight = 0;

void speedControleTask()
{
	TOGGLE_BIT(PORTE,PE5);
	INT8U localDesiredSpeedLeft = desired_speed_left;
	INT8U localDesiredSpeedRight = desired_speed_right;

	INT16S enconderWantedLeft = 0;
	INT16S enconderWantedRight = 0;
	if(direction_left == 'b')
	{
		enconderWantedLeft = -1 * ((localDesiredSpeedLeft) / twoEncoderTicklength) / (1000 / (controlerTimeStep));
	}else if(direction_left == 'f'){
		enconderWantedLeft = ((localDesiredSpeedLeft) / twoEncoderTicklength) / (1000 / (controlerTimeStep));
	}

	if (direction_right == 'b') {
		enconderWantedRight = -1 * ((localDesiredSpeedRight) / twoEncoderTicklength) / (1000 / (controlerTimeStep));
	}else if(direction_right == 'f'){
		enconderWantedRight = ((localDesiredSpeedRight) / twoEncoderTicklength) / (1000 / (controlerTimeStep));
	}

	INT16S tempTickLeft = get_left();
	INT16S tempTickRight = get_right();

	INT8S errorLeft = lastEnconderWantedLeft - tempTickLeft;
	INT8S errorRight = lastEnconderWantedRight - tempTickRight;

	lastEnconderWantedLeft = enconderWantedLeft;
	lastEnconderWantedRight = enconderWantedRight;


	set_pwm_speed_direction((INT8U)enconderWantedRight,'r');
	set_pwm_speed_direction((INT8U)enconderWantedLeft,'l');

//	enconderWantedLeft += errorLeft;
//	enconderWantedRight += errorRight;

//	set_pwm_speed_direction(get_current_speed('r') + errorRight,'r');
//	set_pwm_speed_direction(get_current_speed('l') + errorLeft,'l');

}


void commands()
{
	// read encoder left regiset
	if(message[0] == 'R' && message[1] == 'E' && message[2] == 'L' && message[3] == 'R')
	{
		uart_send_INT16S(storeEncoderLeft,'E','L');
	}
	// read encoder left regiset
	if(message[0] == 'R' && message[1] == 'E' && message[2] == 'R' && message[3] == 'R')
	{
		uart_send_INT16S(storeEncoderRight,'E','R');
	}

	// set moter left enable
	if(message[0] == 'S' && message[1] == 'M' && message[2] == 'L' && message[3] == 'E')
	{
		SET_BIT_HIGH(PORTA,PA2);
	}
	// set moter right enable
	if(message[0] == 'S' && message[1] == 'M' && message[2] == 'R' && message[3] == 'E')
	{
		SET_BIT_HIGH(PORTA,PA3);
	}

	// set moter left disenable
	if(message[0] == 'S' && message[1] == 'M' && message[2] == 'L' && message[3] == 'D')
	{
		SET_BIT_LOW(PORTA,PA2);
	}
	// set moter right disenable
	if(message[0] == 'S' && message[1] == 'M' && message[2] == 'R' && message[3] == 'D')
	{
		SET_BIT_LOW(PORTA,PA3);
	}

	//Set Desired Motor Speed
	if(message[0] == 'S' && message[1] == 'D' && message[2] == 'M' && message[3] == 'S')
	{
		INT8U templ = 0;
		INT8U tempr = 0;
		if(serial_rx_avail())
		{
			templ = (INT8U)serial_rx();
			tempr = (INT8U)serial_rx();
		}
		desired_speed_left = templ;
		desired_speed_right = tempr;
	}

	//Set Desired Speed and Direction
		if(message[0] == 'S' && message[1] == 'D' && message[2] == 'S' && message[3] == 'D')
		{


//			unsigned char temp[4];
//
//			INT8U i = 0;
//			while(i < 4)
//			{
//				if(serial_rx_avail())
//				{
//					temp[i] = serial_rx();
//					i++;
//				}
//			}
//
//			desired_speed_left = (INT8U)temp[0];
//			desired_speed_right = (INT8U)temp[1];
//			direction_left = temp[2];
//			direction_right = temp[3];

			INT8U templ = 0;
			unsigned char direction_l;
			INT8U tempr = 0;
			unsigned char direction_r;
			if(serial_rx_avail())
			{
				templ = (INT8U)serial_rx();
				tempr = (INT8U)serial_rx();
				direction_l = serial_rx();
				direction_r = serial_rx();
			}
			desired_speed_left = templ;
			desired_speed_right = tempr;
			direction_left = direction_l;
			direction_right = direction_r;
		}

}



void resiveTask()
{
	int i = 0;
	while(serial_rx_avail())
	{
		message[i++] = serial_rx();
		if(i > 3)
		{
			commands();
			i = 0;
		}
	}
}

void dummyTask()
{

}

void schedulSetup()
{
	initAliveTasks();

	numberOfTask = 5; // must set number of task here

	struct task allTaks[numberOfTask];

	struct task aliveTaskStruck;
	aliveTaskStruck.time = 50;
	aliveTaskStruck.nextRun = 0;
	aliveTaskStruck.functionPtr = &aliveTask;

	struct task aliveTaskStruck2;
	aliveTaskStruck2.time = 500;
	aliveTaskStruck2.nextRun = 0;
	aliveTaskStruck2.functionPtr = &aliveTask2;

//	struct task pwmTest;
//	pwmTest.time = 200;
//	pwmTest.nextRun = 0;
//	pwmTest.functionPtr = &pwmtestTask;

	struct task pid;
	pid.time = controlerTimeStep;
	pid.nextRun = 0;
	pid.functionPtr = &speedControleTask;

	struct task resiveTaskStruct;
	resiveTaskStruct.time = 10;
	resiveTaskStruct.nextRun = 0;
	resiveTaskStruct.functionPtr = &resiveTask;

	struct task dummy;
	dummy.time = 20000;
	dummy.nextRun = 0;
	dummy.functionPtr = &dummyTask;


	allTaks[0] = aliveTaskStruck;
	allTaks[1] = aliveTaskStruck2;
	allTaks[2] = resiveTaskStruct;
	allTaks[3] = pid;
	allTaks[4] = dummy;

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
//			serial_tx('d');
			INT16U remainder = 0;
			for (int i = 0; i < numberOfTask; ++i) {
				if(allTask[i].nextRun <= 60000)
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


