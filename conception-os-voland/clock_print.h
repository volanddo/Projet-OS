#ifndef __CLOCK_PRINT__
#define __CLOCK_PRINT__

#include <inttypes.h>

uint32_t get_time();

void clock_print(const char * s, int len);

void tic_PIT(void);

void setClockFreq(void);

#endif