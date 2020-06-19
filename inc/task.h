#ifndef __TASK_H_
#define __TASK_H_

#include <stdint.h>
#include "time.h"


////////////////////////////////////////////////////////////////
// DEFINITIONS
////////////////////////////////////////////////////////////////

#define TASK_STATS_MOVING_SUM_COUNT 32


////////////////////////////////////////////////////////////////
// ENUMERATIONS
////////////////////////////////////////////////////////////////

typedef enum {
	// Disables dynamic scheduling, task is executed only if
	// no other task is active this cycle.
    TASK_PRIORITY_IDLE   = 0,
    TASK_PRIORITY_LOW    = 50,
    TASK_PRIORITY_MEDIUM = 100,
    TASK_PRIORITY_HIGH   = 200,
    TASK_PRIORITY_MAX    = 255
} taskPriority_e;


/* ADD NEW TASKS HERE */
typedef enum {
    TASK_SYSTEM = 0,
	TASK_1      = 1,
	TASK_2      = 2,

	TASK_COUNT
} taskId_e;


////////////////////////////////////////////////////////////////
// STRUCTURES
////////////////////////////////////////////////////////////////

typedef struct {
    // CONFIGURATION.
    const char *taskName;
    void (*taskFunc)(void);
    timeDelta_t desiredPeriodUs;		// Target period of execution.
    const int8_t staticPriority;		// dynamicPriority grows in steps of this size.

    // SCHEDULING.
    // Measurement of how old task was last executed,
    // used to avoid task starvation.
    uint16_t dynamicPriority;
    uint16_t taskAgeCycles;
    timeDelta_t taskLatestDeltaTimeUs;	// Last time interval between two successive executions.
    timeUs_t lastExecutedAtUs;			// Last time of invocation.
    timeUs_t lastSignaledAtUs;			// Time of invocation event for event-driven tasks.

    // STATISTICS.
    float    movingAverageCycleTimeUs;
    timeUs_t movingSumExecutionTimeUs;  // Moving sum over x samples.
    timeUs_t movingSumDeltaTimeUs;  	// Moving sum over x samples.
    timeUs_t maxExecutionTimeUs;
    timeUs_t totalExecutionTimeUs;    	// Total time consumed by task since boot.
} task_t;


////////////////////////////////////////////////////////////////
// FUNCTIONS
////////////////////////////////////////////////////////////////

task_t *getTask(const taskId_e taskId);

#endif /* __TASK_H_ */
