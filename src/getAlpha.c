/*
  Copyright by Stefan Kurtz (C) 1994
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
#include <stdio.h>
#include <limits.h>
#include "types.h"

void getAlpha (char *s, char *alpha, Uint *alphaSize)
{
  Uint occ[UCHAR_MAX+1] = {0}, i, j;
  char *scopy;

  for (scopy = s; *scopy != '\0'; scopy++)
  {
    occ[(int) *scopy]++;
  }
  for (j=0, i=0; i<=(Uint) UCHAR_MAX; i++)
  {
    if (occ[i] > 0)
    {
      alpha[j++] = (char) i;
    }
  }
  alpha[j] = '\0';
  *alphaSize = j;
}

void getUchars(Uchar *text, Uint textlen, Uchar *alpha, Uint *alphasize)
{
  Uint occ[UCHAR_MAX+1] = {0}, i, j;
  Uchar *tptr;

  for (tptr = text; tptr < text+textlen; tptr++)
  {
    occ[(Uint) *tptr]++;
  }
  for (j=0, i=0; i<=UCHAR_MAX; i++)
  {
    if(occ[i] > 0)
    {
      alpha[j++] = (Uchar) i;
    }
  }
  *alphasize = j;
}

#ifdef DEBUG
void printAlpha (char *s)
{
  Uint i, j, occ[UCHAR_MAX+1] = {0};
  char *scopy;

  for (scopy = s; *scopy != '\0'; scopy++)
  {
    occ[(int) *scopy]++;
  }
  printf("alpha=(");
  for (j=0, i=0; i<=(Uint) UCHAR_MAX; i++)
  {
    if (occ[i] > 0)
    {
      (void) putchar((char) i);
      j++;
    }
  }
  printf(")\nalphasize=%lu\n",(Showuint) j);
  for(i=0; i<=(Uint) UCHAR_MAX; i++)
  {
    if(occ[i] > 0)
    {
      printf("'%c':%lu\n",(char) i,(Showuint) occ[i]);
    }
  }
}
#endif
