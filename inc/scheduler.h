#ifndef __SCHEDULER_H_
#define __SCHEDULER_H_

#include "task.h"


////////////////////////////////////////////////////////////////
// STRUCTURES
////////////////////////////////////////////////////////////////

typedef struct {
	// TASK QUEUE.
	uint32_t taskQueuePos;
	uint32_t taskQueueSize;
	task_t *taskQueueArray[TASK_COUNT + 1];

	// STATISTICS.
	uint32_t totalWaitingTasks;
	uint32_t totalWaitingTasksSamples;
} scheduler_t;


////////////////////////////////////////////////////////////////
// FUNCTIONS
////////////////////////////////////////////////////////////////

void schedulerInit(void);
void schedulerUpdate(void);

#endif /* __SCHEDULER_H_ */
