/*
  Copyright by Stefan Kurtz (C) 1998-2003
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include "compl.h"
#include "types.h"

void reverseinplace(Uchar *s,Uint len)
{
  Uchar *front, *back, tmp;

  for(front = s, back = s + len - 1; front < back; front++, back--)
  {
    tmp = *front;
    *front = *back;
    *back = tmp;
  }
}

void reversecomplement(Uchar *s,Uint len)
{
  Uchar *front, *back, tmp = 0;

  for(front = s, back = s + len - 1; front <= back; front++, back--)
  {
    ASSIGNCOMPLEMENT(tmp,*front);
    ASSIGNCOMPLEMENT(*front,*back);
    *back = tmp;
  }
}

void complement(Uchar *s,Uint len)
{
  Uchar *sptr;

  for(sptr = s; sptr < s + len; sptr++)
  {
    ASSIGNCOMPLEMENT(*sptr,*sptr);
  }
}

void reversestring(char *s, Uint m, char *sreverse)
{
  char *sp;

  for(sreverse += m-1, *(sreverse+1) = '\0', sp = s; 
      *sp != '\0'; 
      *sreverse-- = *sp++)
    /*NOTHING*/;
  sreverse++;
}
