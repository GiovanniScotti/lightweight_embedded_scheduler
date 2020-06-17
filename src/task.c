#include "task.h"
#include "stm32h7xx_hal.h"

#define TASK_PERIOD_HZ(hz) (1000000 / (hz))
#define TASK_PERIOD_MS(ms) ((ms) * 1000)
#define TASK_PERIOD_US(us) (us)

#define DEFINE_TASK(taskNameArg, taskFuncArg, desiredPeriodArg, staticPriorityArg) { \
    .taskName = taskNameArg, \
    .taskFunc = taskFuncArg, \
    .desiredPeriodUs = desiredPeriodArg, \
    .staticPriority = staticPriorityArg  \
}


static void taskSystem() {}
static void task1() {
	HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
}

static void task2() {

}


static task_t tasksArray[TASK_COUNT] = {
	[TASK_SYSTEM] = DEFINE_TASK("SYSTEM", taskSystem, TASK_PERIOD_HZ(10), TASK_PRIORITY_MEDIUM),
	[TASK_1] = DEFINE_TASK("TASK_1", task1, TASK_PERIOD_HZ(2), TASK_PRIORITY_LOW),
	[TASK_2] = DEFINE_TASK("TASK_2", task2, TASK_PERIOD_HZ(1000), TASK_PRIORITY_LOW)
};


////////////////////////////////////////////////////////////////
// PUBLIC INTERFACE
////////////////////////////////////////////////////////////////

task_t *getTask(const taskId_e taskId) {
    return &tasksArray[taskId];
}
