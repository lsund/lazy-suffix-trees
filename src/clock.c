/*
  Copyright by Stefan Kurtz (C) 1999-2003
  =====================================
  You may use, copy and distribute this file freely as long as you
   - do not change the file,
   - leave this copyright notice in the file,
   - do not make any profit with the distribution of this file
   - give credit where credit is due
  You are not allowed to copy or distribute this file otherwise
  The commercial usage and distribution of this file is prohibited
  Please report bugs and suggestions to <kurtz@zbh.uni-hamburg.de>
*/

/*
 * Modified by Ludvig Sundström 2018 with permission from Stefan Kurtz.
 * For full source control tree, see https://github.com/lsund/wotd
 */

#include <stdio.h>
#include <time.h>
#include "types.h"


// Start and stoptime
static clock_t startclock, stopclock;


// Initialize the clock
void initclock(void)
{
  startclock = clock();
}


// The time since th clock was initialized
double getruntime(void)
{
   stopclock = clock();
   return (double) (stopclock - startclock) / (double) CLOCKS_PER_SEC;
}


// The clockticks since the clock was initialized
Uint getclockticks(void)
{
   stopclock = clock();
   return (Uint) (stopclock - startclock);
}
