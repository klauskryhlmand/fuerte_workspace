/*
 * scheduler.h
 *
 *  Created on: Mar 2, 2014
 *      Author: daniel
 *     version: 0.1
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

struct task{
	INT16U time;
	INT16U nextRun;
	void (*functionPtr)(void);
};
void uart_send_INT16S(INT16S value, unsigned char handle_part1, unsigned char handle_part2);
void uart_send_INT16U(INT16U value, unsigned char handle_part1, unsigned char handle_part2);

//void aliveTask(void);
//
//void aliveTask2(void);

void schedulSetup(void);

void scheduler();


#endif /* SCHEDULER_H_ */
