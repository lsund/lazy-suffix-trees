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

#ifndef EXPERIM_H
#define EXPERIM_H
#ifndef ITER
#define ITER 1
#endif
#define SHOWTIME(P)\
        printf("TIME %s %s %.2f\n",P,filename,getruntime()/(double) ITER)

#ifdef NOSPACEBOOKKEEPING
#define SHOWSPACE(P) /* Nothing */
#define SHOWMMSPACE(P) /* Nothing */
#else
#define SHOWSPACE(P)\
        printf("SPACE %s %s %lu\n",P,filename,(Showuint) getspacepeak())

#define SHOWMMSPACE(P)\
        printf("MMSPACE %s %s %lu\n",P,filename,(Showuint) mmgetspacepeak());

#endif

#define ITERATE(S)   { Uint i; for(i=0; i<ITER; i++) { S } }

#endif
