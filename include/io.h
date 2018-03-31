
#ifndef IO_H
#define IO_H

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "spaceman.h"
#include "types.h"
#include "debug.h"
#include "error.h"

///////////////////////////////////////////////////////////////////////////////
// Macros


#define TMPFILESUFFIX        "XXXXXX"

#define NUMBEROFX            strlen(TMPFILESUFFIX)

#define TMPFILEPREFIX        "/tmp/Vmatch"

// The maximal length of the string specifying the open mode of the
// call to function createfilehandle.
#define MAXOPENMODE 2

// Writing binary mode
#define WRITEMODE  "wb"
// Reading binary mode
#define READMODE   "rb"
// Appending binary mode
#define APPENDMODE "ab"

#define CREATEFILEHANDLE(PATH,MODE)\
        createfilehandle(__FILE__,(Uint) __LINE__,PATH,MODE)

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

///////////////////////////////////////////////////////////////////////////////
// Structs

// A handle for  user opened files. A file handle consists of
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


int fileOpen(char *name, Uint *textlen, BOOL writefile);

caddr_t fileParts(int fd,Uint offset,Uint len,BOOL writemap);

void freetextspace(Uchar *text, Uint textlen);

caddr_t genfile2String(char *name, Uint *textlen,
                                  BOOL writefile, BOOL writemap);

caddr_t file2String(char *name, Uint *textlen);

int file2Array(char *name, Uint *textlen, int nlines, char ***words);

FILE *createfilehandle(
        char *file,
        const Uint line,
        const char *path,
        const char *mode
    );

#endif

