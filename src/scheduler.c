#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "scheduler.h"
#include "system.h"

// Scheduler status.
static scheduler_t scheduler;


static void queueClear(void) {
	memset(scheduler.taskQueueArray, 0, sizeof(scheduler.taskQueueArray));
	scheduler.taskQueuePos = 0;
	scheduler.taskQueueSize = 0;
	return;
}


static bool queueContains(task_t *task) {
    for (uint32_t i = 0; i < scheduler.taskQueueSize; i++) {
        if (scheduler.taskQueueArray[i] == task) {
            return true;
        }
    }
    return false;
}


static bool queueAdd(task_t *task) {
    if ((scheduler.taskQueueSize >= TASK_COUNT) || queueContains(task)) {
        return false;
    }
    for (uint32_t i = 0; i <= scheduler.taskQueueSize; i++) {
        if (scheduler.taskQueueArray[i] == NULL ||
        	scheduler.taskQueueArray[i]->staticPriority < task->staticPriority) {

            memmove(&scheduler.taskQueueArray[i+1], &scheduler.taskQueueArray[i],
            		sizeof(task) * (scheduler.taskQueueSize - i));
            scheduler.taskQueueArray[i] = task;
            scheduler.taskQueueSize++;
            return true;
        }
    }
    return false;
}


static bool queueRemove(task_t *task) {
    for (uint32_t i = 0; i < scheduler.taskQueueSize; i++) {
        if (scheduler.taskQueueArray[i] == task) {
            memmove(&scheduler.taskQueueArray[i], &scheduler.taskQueueArray[i+1],
            		sizeof(task) * (scheduler.taskQueueSize - i));
            scheduler.taskQueueSize--;
            return true;
        }
    }
    return false;
}


static task_t *queueFirst(void) {
	scheduler.taskQueuePos = 0;
    // Guaranteed to be NULL if queue is empty.
    return scheduler.taskQueueArray[0];
}


static task_t *queueNext(void) {
	// Guaranteed to be NULL at end of queue.
    return scheduler.taskQueueArray[++scheduler.taskQueuePos];
}


static void schedulerExecuteTask(task_t *selectedTask, timeUs_t currentTimeUs) {
    timeUs_t taskExecutionTimeUs = 0;

    if (selectedTask) {
    	// Updates task's scheduling variables.
        selectedTask->taskLatestDeltaTimeUs =
        	cmpTimeUs(currentTimeUs, selectedTask->lastExecutedAtUs);
        selectedTask->lastExecutedAtUs = currentTimeUs;
        selectedTask->dynamicPriority = 0;

        // Executes task.
        const timeUs_t currentTimeBeforeTaskCallUs = micros();
		selectedTask->taskFunc();
		taskExecutionTimeUs = cmpTimeUs(micros(), currentTimeBeforeTaskCallUs);

		// Updates statistics.
		selectedTask->movingSumExecutionTimeUs +=
			taskExecutionTimeUs - selectedTask->movingSumExecutionTimeUs /
			TASK_STATS_MOVING_SUM_COUNT;
		selectedTask->movingSumDeltaTimeUs +=
			selectedTask->taskLatestDeltaTimeUs - selectedTask->movingSumDeltaTimeUs /
			TASK_STATS_MOVING_SUM_COUNT;
		selectedTask->totalExecutionTimeUs += taskExecutionTimeUs;
		selectedTask->maxExecutionTimeUs =
			MAX(selectedTask->maxExecutionTimeUs, taskExecutionTimeUs);
		selectedTask->movingAverageCycleTimeUs += 0.05f *
			(selectedTask->taskLatestDeltaTimeUs - selectedTask->movingAverageCycleTimeUs);
    }

    return;
}


////////////////////////////////////////////////////////////////
// PUBLIC INTERFACE
////////////////////////////////////////////////////////////////

void schedulerInit(void) {
	cycleCounterInit();

    queueClear();

    /* ADD SYSTEM TASKS HERE */
    queueAdd(getTask(TASK_SYSTEM));
    queueAdd(getTask(TASK_1));
    queueAdd(getTask(TASK_2));
    return;
}


void schedulerUpdate(void) {
	const timeUs_t schedulerStartTimeUs = micros();
    timeUs_t currentTimeUs = schedulerStartTimeUs;
    task_t *selectedTask = NULL;
    uint16_t selectedTaskDynamicPriority = 0;
    uint16_t waitingTasks = 0;

    for (task_t *task = queueFirst(); task != NULL; task = queueNext()) {
		// TODO: manage event-driven tasks.

		// Time-driven tasks.
		task->taskAgeCycles = ((currentTimeUs - task->lastExecutedAtUs) /
			task->desiredPeriodUs);
		if (task->taskAgeCycles > 0) {
			task->dynamicPriority = 1 + task->staticPriority * task->taskAgeCycles;
			// A task is waiting when needs to be executed after desiredPeriodUs
			// microseconds from its last execution.
			waitingTasks++;
		}

		if (task->dynamicPriority > selectedTaskDynamicPriority) {
			selectedTaskDynamicPriority = task->dynamicPriority;
			selectedTask = task;
		}
    }

    // Used to estimate the system load.
    scheduler.totalWaitingTasksSamples++;
    scheduler.totalWaitingTasks += waitingTasks;

    if (selectedTask) {
        schedulerExecuteTask(selectedTask, currentTimeUs);
    }

	return;
}
