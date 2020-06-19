////////////////////////////////////////////////////////////////
// PLATFORM-DEPENDENT COMPILATION UNIT
////////////////////////////////////////////////////////////////

#include "system.h"
#include "stm32h7xx_hal.h"


// Up time in milliseconds.
extern __IO uint32_t uwTick;

// Clock cycles per microsecond.
static volatile uint32_t usTicks = 0;


// Cycle counter initialization.
void cycleCounterInit(void) {
    usTicks = HAL_RCC_GetSysClockFreq() / 1000000;
    return;
}


// Returns system uptime in microseconds (rollover in 71 minutes).
uint32_t micros(void) {
    uint32_t ms, cycle_cnt;
    do {
        ms = uwTick;
        cycle_cnt = SysTick->VAL;
    } while (ms != uwTick);
    return (ms * 1000) + (usTicks * 1000 - cycle_cnt) / usTicks;
}


// Returns system uptime in milliseconds (rollover in 49 days).
uint32_t millis(void) {
    return uwTick;
}
