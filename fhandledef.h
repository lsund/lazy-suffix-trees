/*
  Copyright by Stefan Kurtz (C) 2003
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

#ifndef FHANDLEDEF_H
#define FHANDLEDEF_H
#include <stdio.h>
#include "types.h"
#include "errordef.h"

#define TMPFILESUFFIX        "XXXXXX"
#define NUMBEROFX            strlen(TMPFILESUFFIX)
#define TMPFILEPREFIX        "/tmp/Vmatch"

#define WRITEMODE  "wb"   // writing in binary mode, important for MS-Windows
#define READMODE   "rb"   // reading in binary mode, important for MS-Windows
#define APPENDMODE "ab"   // appending in binary mode, important for MS-Windows

typedef struct
{
  char *tmpfilenamebuffer;
  Uint tmpfilenamelength;
  FILE *tmpfileptr;
} Tmpfiledesc;

//}

/*
  This file defines macros to simplify the calls to the
  functions 
  \begin{itemize}
  \item
  \texttt{createfilehandle}, 
  \item
  \texttt{maketmpfile},
  \item
  \texttt{deletefilehandle},
  \item
  \texttt{writetofilehandle},
  \item 
  and \texttt{readfromfilehandle}.
  \end{itemize}
*/

#define CREATEFILEHANDLE(PATH,MODE)\
        createfilehandle(__FILE__,(Uint) __LINE__,PATH,MODE)

#define MAKETMPFILE(TMPFILEDESC,FILEPREFIX)\
        maketmpfile(__FILE__,(Uint) __LINE__,TMPFILEDESC,FILEPREFIX)

#define DELETEFILEHANDLE(FP)\
        deletefilehandle(__FILE__,(Uint) __LINE__,FP)

#define WRITETOFILEHANDLE(PTR,SZ,NMEMB,FP)\
        writetofilehandle(__FILE__,(Uint) __LINE__,PTR,SZ,NMEMB,FP)

#define READFROMFILEHANDLE(PTR,SZ,NMEMB,FP)\
        readfromfilehandle(__FILE__,(Uint) __LINE__,PTR,SZ,NMEMB,FP)

#define DECLAREREADFUNCTION(TYPE)\
        static Sint readnext ## TYPE(TYPE *read ## TYPE,FILE *fp)\
        {\
          if(fread(read ## TYPE,sizeof(TYPE),(size_t) 1,fp) != (size_t) 1)\
          {\
            if(feof(fp))\
            {\
              ERROR1("unexpected end of file when trying to read %s",#TYPE);\
              return (Sint) -1;\
            }\
            if(ferror(fp))\
            {\
              ERROR1("error when trying to read next %s",#TYPE);\
              return (Sint) -2;\
            }\
          }\
          return 0;\
        }

//\IgnoreLatex{

#endif

//}
