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

#ifndef FHANDLEDEF_H
#define FHANDLEDEF_H
#include <stdio.h>
#include "types.h"
#include "error.h"

#define TMPFILESUFFIX        "XXXXXX"
#define NUMBEROFX            strlen(TMPFILESUFFIX)
#define TMPFILEPREFIX        "/tmp/Vmatch"

// The maximal length of the string specifying the open mode of the
// call to function createfilehandle.
#define MAXOPENMODE 2

#define WRITEMODE  "wb"   // writing in binary mode, important for MS-Windows
#define READMODE   "rb"   // reading in binary mode, important for MS-Windows
#define APPENDMODE "ab"   // appending in binary mode, important for MS-Windows

typedef struct
{
  char *tmpfilenamebuffer;
  Uint tmpfilenamelength;
  FILE *tmpfileptr;
} Tmpfiledesc;

// This file contains functions to store file handles for user opened files. A
// file handle consists of
// 1. The filepointer
// 2. A string describing the open mode, corresponding to the second argument
//    of fopen.
// 3. The line number and the program file call where the open function was
//    done.
//
// `file` and `line` if they occur are always the filename and linenumber,
// where the function is called from.
typedef struct filehandle {
  char path[PATH_MAX + 1],
       createmode[MAXOPENMODE + 1],
       *createfile;
  Uint createline;
} Filehandle;


///////////////////////////////////////////////////////////////////////////////
// Functions


FILE *createfilehandle(
        char *file,
        const Uint line,
        const char *path,
        const char *mode
    );


Sint deletefilehandle(
        char *file,
        Uint line,
        FILE *fp
    );

void wraptmpfiledesc(Tmpfiledesc *tmpfiledesc);

///////////////////////////////////////////////////////////////////////////////
// Macros


#define CREATEFILEHANDLE(PATH,MODE)\
        createfilehandle(__FILE__,(Uint) __LINE__,PATH,MODE)

#define DELETEFILEHANDLE(FP)\
        deletefilehandle(__FILE__,(Uint) __LINE__,FP)

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

#endif
