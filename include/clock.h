#ifndef CLOCK_H
#define CLOCK_H

#include "types.h"

// Initialize the clock
void clock_init(void);

// The integer number of clock ticks
Uint getclockticks(void);

// The time in seconds since the clock was initialized
double getruntime(void);

#endif
