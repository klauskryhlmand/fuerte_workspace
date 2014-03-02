/*
 * scheduler.h
 *
 *  Created on: Mar 2, 2014
 *      Author: daniel
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

struct task{
	INT16U time;
	INT16U nextRun;
	void (*functionPtr)(void);
};

void timer1_init(void);

void aliveTask(void);

void aliveTask2(void);

void schedulSetup(void);

void scheduler();


#endif /* SCHEDULER_H_ */
