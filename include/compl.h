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

//\IgnoreLatex{

#ifndef COMPL_H
#define COMPL_H
#include <stdio.h>
#include <stdlib.h>

//}

/*
  The following macro defines a switch statement assigning to the
  variable \texttt{VL} the complement of \texttt{VR}.
*/

#define ASSIGNCOMPLEMENT(VL,VR)\
        switch(VR)\
        {\
          case 'A' : VL = (Uchar) 'T'; break;\
          case 'C' : VL = (Uchar) 'G'; break;\
          case 'G' : VL = (Uchar) 'C'; break;\
          case 'T' : VL = (Uchar) 'A'; break;\
          case 'a' : VL = (Uchar) 't'; break;\
          case 'c' : VL = (Uchar) 'g'; break;\
          case 'g' : VL = (Uchar) 'c'; break;\
          case 't' : VL = (Uchar) 'a'; break;\
          default  : fprintf(stderr,"Complement of '%c' not defined\n",VR);\
                     exit(EXIT_FAILURE);\
        }

//\IgnoreLatex{

#endif

//}
