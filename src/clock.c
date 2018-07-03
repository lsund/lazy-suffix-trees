#include <stdio.h>
#include <time.h>
#include "types.h"


static clock_t startclock, stopclock;


void initclock(void)
{
  startclock = clock();
}


Uint getclockticks(void)
{
   stopclock = clock();
   return (Uint) (stopclock - startclock);
}


double getruntime(void)
{
   stopclock = clock();
   return (double) (stopclock - startclock) / (double) CLOCKS_PER_SEC;
}
