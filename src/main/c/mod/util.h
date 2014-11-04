#ifndef UTIL_H_
#define UTIL_H_

#include <time.h>

void R20_pause(long pause);

struct timespec R20_currentTime();

int R20_killSignal;

void R20_initTerminator(void);

#endif /* UTIL_H_ */
