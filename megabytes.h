/*
  Copyright by Stefan Kurtz (C) 2000-2003
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

//\IgnoreLatex{

#ifndef MEGABYTES_H
#define MEGABYTES_H
#include "types.h"

//}

/*
  The following macro transforms bytes into megabytes.
*/

#define MEGABYTES(V)  ((double) (V)/((UintConst(1) << 20) - 1))

//\IgnoreLatex{

#endif

//}
