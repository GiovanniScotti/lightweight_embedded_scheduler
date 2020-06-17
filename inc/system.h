#ifndef __SYSTEM_H_
#define __SYSTEM_H_

#include <stdint.h>


////////////////////////////////////////////////////////////////
// FUNCTIONS
////////////////////////////////////////////////////////////////

void cycleCounterInit(void);
uint32_t micros(void);
uint32_t millis(void);

#endif /* __SYSTEM_H_ */