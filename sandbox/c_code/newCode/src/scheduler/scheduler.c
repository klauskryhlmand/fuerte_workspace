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

#define numberOfTask 5 // must set number of task here
INT8S nextTask = -1;
BOOLEAN schedulerNotRun = FALSE;

struct task allTask[numberOfTask];

unsigned char message[4];

INT8U desired_speed_left = 0;
INT8U desired_speed_right = 0;
unsigned char direction_left;
unsigned char direction_right;
unsigned char enable_left;
unsigned char enable_right;

#define twoEncoderTicklength 20
#define controlerTimeStep 20 // can not be more than 1000. Will cause devices by 0


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

INT8U pwmTestSpeed = 100;
BOOLEAN pwmDir = TRUE;
void pwmtestTask()
{
	TOGGLE_BIT(PORTE,PE5);
	set_pwm_speed_direction(pwmTestSpeed,'r');
	set_pwm_speed_direction(pwmTestSpeed,'l');

//	if(pwmDir == TRUE)
//	{
//		pwmTestSpeed = pwmTestSpeed + 1;
//	}else {
//		--pwmTestSpeed;
//		TOGGLE_BIT(PORTE,PE5);
//	}
//
//	if(pwmTestSpeed < 101)
//	{
//		pwmTestSpeed = 100;
//		pwmDir = TRUE;
//	}
//	if(pwmTestSpeed > 130)
//	{
//		pwmTestSpeed = 130;
//		pwmDir = FALSE;
//	}

pwmTestSpeed--;
	if (pwmTestSpeed == 0) {
		pwmTestSpeed = 100;
	}
}



INT16S storeEncoderLeft = 0;
INT16S storeEncoderRight = 0;

INT16S lastEnconderWantedLeft = 0;
INT16S lastEnconderWantedRight = 0;

INT8U localDesiredSpeedLeft = 0;
INT8U localDesiredSpeedRight = 0;

INT16S enconderWantedLeft = 0;
INT16S enconderWantedRight = 0;

INT16S tempTickLeft = 0;
INT16S tempTickRight = 0;

INT16S errorLeft = 0;
INT16S errorRight = 0;

INT16S errorLeftOld = 0;
INT16S errorRightOld = 0;

//INT16S acumulatedErrorLeft = 0;
//INT16S acumulatedErrorRight = 0;

INT16S errorLeftIntegrated = 0;
INT16S errorRightIntegrated = 0;

INT16S errorLeftDerivative = 0;
INT16S errorRightDerivative = 0;

//INT16S tempSpeed_r = 0;
//INT16S tempSpeed_l = 0;

INT16U tempErrorSpeed_r = 100;
INT16U tempErrorSpeed_l = 100;

#define I_led 10
#define P_led 10
//#define D_led 10

void speedControleTask()
{
	TOGGLE_BIT(PORTE,PE5);
	localDesiredSpeedLeft = desired_speed_left;
	localDesiredSpeedRight = desired_speed_right;

	if(localDesiredSpeedLeft == 0)
	{
		tempErrorSpeed_l = 100;
		set_pwm_speed_direction(tempErrorSpeed_r,'l');
		errorLeftIntegrated = 0;
		errorLeftOld = 0;
		errorLeft = 0;
		lastEnconderWantedLeft = 0;
	}
	if(localDesiredSpeedRight == 0)
	{
		tempErrorSpeed_r = 100;
		set_pwm_speed_direction(tempErrorSpeed_r,'r');
		errorRightIntegrated = 0;
		errorRightOld = 0;
		errorRight = 0;
		lastEnconderWantedRight = 0;
	}

	if(enable_left == 'e')
	{
		SET_BIT_HIGH(PORTA,PA3);
	}
	else {
		SET_BIT_LOW(PORTA,PA3);
	}

	if(enable_right == 'e')
	{
		SET_BIT_HIGH(PORTA,PA2);
	}
	else {
		SET_BIT_LOW(PORTA,PA2);
	}

	enconderWantedLeft = 0;
	enconderWantedRight = 0;
	if(direction_left == 'b')
	{
		enconderWantedLeft = -1 * ((localDesiredSpeedLeft) * twoEncoderTicklength) / (1000 / (controlerTimeStep));
//		uart_send_INT16S(enconderWantedLeft,'b','l');
	}else if(direction_left == 'f'){
		enconderWantedLeft = ((localDesiredSpeedLeft) * twoEncoderTicklength) / (1000 / (controlerTimeStep));
//		uart_send_INT16S(enconderWantedLeft,'f','l');
	}

	if (direction_right == 'b') {
		enconderWantedRight = -1 * ((localDesiredSpeedRight) * twoEncoderTicklength) / (1000 / (controlerTimeStep));
//		uart_send_INT16S(enconderWantedRight,'b','r');
	}else if(direction_right == 'f'){
		enconderWantedRight = ((localDesiredSpeedRight) * twoEncoderTicklength) / (1000 / (controlerTimeStep));
//		uart_send_INT16S(enconderWantedRight,'f','r');
	}

	tempTickLeft = get_left();
	tempTickRight = get_right();

	storeEncoderLeft += tempTickLeft;
	storeEncoderRight += tempTickRight;

	errorLeft = (lastEnconderWantedLeft - tempTickLeft);
	errorRight = (lastEnconderWantedRight - tempTickRight);

//	errorLeftDerivative = errorLeft - errorLeftOld;
//	errorRightDerivative = errorRight - errorRightOld;

//	uart_send_INT16S(errorLeft,'e','l');
//	uart_send_INT16S(errorRight,'e','r');

	errorLeftIntegrated += errorLeft*2;
	errorRightIntegrated += errorRight*2;

//		if(errorLeft * errorLeft < 0)
//		{
//			errorLeft = errorLeft/20;
//		}
//
//		if(errorRight * errorRight < 0)
//		{
//			errorRight = errorRight/20;
//		}

//	if(errorLeftIntegrated * errorLeft < 0)
//	{
//		errorLeftIntegrated = errorLeft;
//	}else
//	if(errorLeftIntegrated > I_led * 50)
//	{
//		errorLeftIntegrated = I_led * 50;
//	}else if (errorLeftIntegrated < I_led * - 50) {
//		errorLeftIntegrated = I_led * -50;
//	}
//
//////	if(errorRightIntegrated * errorRight < 0)
//////	{
//////		errorRightIntegrated = errorRight;
//////	}else
//	if(errorRightIntegrated > I_led * 50)
//	{
//		errorRightIntegrated = I_led * 50;
//	}else if (errorRightIntegrated < I_led * - 50) {
//		errorRightIntegrated = I_led * - 50;
//	}


//	errorLeft = 0;
//	errorRight = 0;

//	errorLeftDerivative = 0;
//	errorRightDerivative = 0;

	lastEnconderWantedLeft = enconderWantedLeft;
	lastEnconderWantedRight = enconderWantedRight;

//	lastEnconderWantedLeft = lastEnconderWantedLeft - (errorLeftOld * 100) / 99;
//	lastEnconderWantedRight = lastEnconderWantedRight - (errorRightOld * 100) / 99;

	errorLeftOld = errorLeft/P_led + (errorLeftIntegrated/100)/I_led;
	errorRightOld = errorRight/P_led + (errorRightIntegrated/100)/I_led;
//	tempErrorSpeed_r = 100;
//	tempErrorSpeed_l = 100;

 if (errorLeftOld > 0) {
	 uart_send_INT16U(errorLeftOld,'e','l');
}

 if (errorRightOld > 0) {
 	 uart_send_INT16U(errorRightOld,'e','r');
 }

//	if (errorRight * errorRight > 242 ) {
		if(100 + errorRight/P_led + errorRightIntegrated/I_led < 0)
		{
			set_pwm_speed_direction(0,'r');
		}else if ( 100 + errorRight/P_led + errorRightIntegrated/I_led > 200) {
			set_pwm_speed_direction(200,'r');
		}
		else {
			tempErrorSpeed_r = (INT8U)( errorRight/P_led + errorRightIntegrated/I_led + 100);
//			uart_send_INT16U(tempErrorSpeed_r,'R','P');
			set_pwm_speed_direction(tempErrorSpeed_r,'r');
		}
//	}


//	if (errorLeft * errorLeft > 242 ) {
		if (100 + errorLeft/P_led + errorLeftIntegrated/I_led < 0) {
			set_pwm_speed_direction(0,'l');
		}else if (100 + errorLeft/P_led + errorLeftIntegrated/I_led > 200) {
			set_pwm_speed_direction(200,'l');
		}
		else {
			tempErrorSpeed_l =(INT8U)( 100 + errorLeft/P_led + errorLeftIntegrated/I_led);
//			uart_send_INT16U(tempErrorSpeed_l,'L','P');
			set_pwm_speed_direction(tempErrorSpeed_l,'l');
		}
//	}
////	if(tempErrorSpeed_r + errorRight/P_led + errorRightDerivative/D_led < 0)
////	{
////		set_pwm_speed_direction(0,'r');
////	}else if (errorRight/P_led + tempErrorSpeed_r + errorRightDerivative/D_led > 200) {
////		set_pwm_speed_direction(200,'r');
////	}
////	else {
////		tempErrorSpeed_r = (INT8U)(tempErrorSpeed_r + errorRight/P_led + errorRightDerivative/D_led);
////		uart_send_INT16U(tempErrorSpeed_r,'R','P');
////		set_pwm_speed_direction(tempErrorSpeed_r,'r');
////	}
////
////
////	if (tempErrorSpeed_l + errorLeft/P_led +  errorLeftDerivative/D_led < 0) {
////		set_pwm_speed_direction(0,'l');
////	}else if (tempErrorSpeed_l + errorLeft/P_led + errorLeftDerivative/D_led > 200) {
////		set_pwm_speed_direction(200,'l');
////	}
////	else {
////		tempErrorSpeed_l =(INT8U)(tempErrorSpeed_l + errorLeft/P_led + errorLeftDerivative/D_led);
////		uart_send_INT16U(tempErrorSpeed_l,'L','P');
////		set_pwm_speed_direction(tempErrorSpeed_l,'l');
////	}
//
////	if(tempErrorSpeed_r + errorRight/P_led + errorRightIntegrated/I_led + errorRightDerivative/D_led < 0)
////	{
////		set_pwm_speed_direction(0,'r');
////	}else if (errorRight/P_led + tempErrorSpeed_r + errorRightIntegrated/I_led + errorRightDerivative/D_led > 200) {
////		set_pwm_speed_direction(200,'r');
////	}
////	else {
////		tempErrorSpeed_r = (INT8U)(tempErrorSpeed_r + errorRight/P_led + errorRightIntegrated/I_led + errorRightDerivative/D_led);
////		uart_send_INT16U(tempErrorSpeed_r,'R','P');
////		set_pwm_speed_direction(tempErrorSpeed_r,'r');
////	}
////
////
////	if (tempErrorSpeed_l + errorLeft/P_led + errorLeftIntegrated/I_led + errorLeftDerivative/D_led < 0) {
////		set_pwm_speed_direction(0,'l');
////	}else if (tempErrorSpeed_l + errorLeft/P_led + errorLeftIntegrated/I_led + errorLeftDerivative/D_led > 200) {
////		set_pwm_speed_direction(200,'l');
////	}
////	else {
////		tempErrorSpeed_l =(INT8U)(tempErrorSpeed_l + errorLeft/P_led + errorLeftIntegrated/I_led + errorLeftDerivative/D_led); //errorLeft +
////		uart_send_INT16U(tempErrorSpeed_l,'L','P');
////		set_pwm_speed_direction(tempErrorSpeed_l,'l');
////	}
//

}


void commands()
{
	// read encoder left regiset
	if(message[0] == 'R' && message[1] == 'E' && message[2] == 'L' && message[3] == 'R')
	{
		uart_send_INT16S(storeEncoderLeft,'E','L');
		storeEncoderLeft = 0;
	}
	// read encoder left regiset
	if(message[0] == 'R' && message[1] == 'E' && message[2] == 'R' && message[3] == 'R')
	{
		uart_send_INT16S(storeEncoderRight,'E','R');
		storeEncoderRight = 0;
	}

	// set moter left enable
	if(message[0] == 'S' && message[1] == 'M' && message[2] == 'L' && message[3] == 'E')
	{
		SET_BIT_HIGH(PORTA,PA3);
	}
	// set moter right enable
	if(message[0] == 'S' && message[1] == 'M' && message[2] == 'R' && message[3] == 'E')
	{
		SET_BIT_HIGH(PORTA,PA2);
	}

	// set moter left disenable
	if(message[0] == 'S' && message[1] == 'M' && message[2] == 'L' && message[3] == 'D')
	{
		SET_BIT_LOW(PORTA,PA3);
	}
	// set moter right disenable
	if(message[0] == 'S' && message[1] == 'M' && message[2] == 'R' && message[3] == 'D')
	{
		SET_BIT_LOW(PORTA,PA2);
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


			unsigned char temp[6];

			INT8U i = 0;
			while(i < 6)
			{
				if(serial_rx_avail())
				{
					temp[i] = serial_rx();
					i++;
				}
			}

			desired_speed_left = (INT8U)temp[0];
			desired_speed_right = (INT8U)temp[1];
			direction_left = temp[2];
			direction_right = temp[3];
			enable_left = temp[4];
			enable_right = temp[5];

//			INT8U templ = 0;
//			unsigned char direction_l;
//			INT8U tempr = 0;
//			unsigned char direction_r;
//			if(serial_rx_avail())
//			{
//				templ = (INT8U)serial_rx();
//				tempr = (INT8U)serial_rx();
//				direction_l = serial_rx();
//				direction_r = serial_rx();
//			}
//			desired_speed_left = templ;
//			desired_speed_right = tempr;
//			direction_left = direction_l;
//			direction_right = direction_r;
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
	return;
}

void schedulSetup()
{
	initAliveTasks();

//	struct task allTaks[numberOfTask];

	enable_left = 'd';
	enable_right = 'd';

	struct task aliveTaskStruck;
	aliveTaskStruck.time = 50;
	aliveTaskStruck.nextRun = 0;
	aliveTaskStruck.functionPtr = &aliveTask;

	struct task aliveTaskStruck2;
	aliveTaskStruck2.time = 500;
	aliveTaskStruck2.nextRun = 0;
	aliveTaskStruck2.functionPtr = &aliveTask2;

	struct task pwmTest;
	pwmTest.time = 500;
	pwmTest.nextRun = 0;
	pwmTest.functionPtr = &pwmtestTask;

	struct task pid;
	pid.time = controlerTimeStep;
	pid.nextRun = 0;
	pid.functionPtr = &speedControleTask;

	struct task resiveTaskStruct;
	resiveTaskStruct.time = 20;
	resiveTaskStruct.nextRun = 0;
	resiveTaskStruct.functionPtr = &resiveTask;

	struct task dummy;
	dummy.time = 3000;
	dummy.nextRun = 0;
	dummy.functionPtr = &dummyTask;


	allTask[0] = aliveTaskStruck;
	allTask[1] = aliveTaskStruck2;
	allTask[2] = resiveTaskStruct;
	allTask[3] = pid;
//	allTask[3] = pwmTest;
	allTask[4] = dummy;

//	allTask = allTaks;
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


