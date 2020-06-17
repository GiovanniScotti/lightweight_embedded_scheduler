#ifndef __TIME_H_
#define __TIME_H_

#include <stdint.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))

typedef int32_t timeDelta_t;
typedef uint32_t timeUs_t;


static inline timeDelta_t cmpTimeUs(timeUs_t a, timeUs_t b) {
	return (timeDelta_t)(a - b);
}

#endif /* __TIME_H_ */
