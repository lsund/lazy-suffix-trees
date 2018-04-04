
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

// Writing binary mode
#define WRITEMODE  "wb"
// Reading binary mode
#define READMODE   "rb"
// Appending binary mode
#define APPENDMODE "ab"

#define INCFILEHANDLES 16

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
       createmode[3],
       *createfile;
  Uint createline;
} Filehandle;


///////////////////////////////////////////////////////////////////////////////
// Functions


caddr_t fileParts(int fd,Uint offset,Uint len,Bool writemap);

void freetextspace(Uchar *text, Uint textlen);

caddr_t genfile2String(char *name, Uint *textlen,
                                  Bool writefile, Bool writemap);

caddr_t file2String(char *name, Uint *textlen);

int file2Array(char *name, Uint *textlen, int nlines, char ***words);

FILE *createfilehandle(char *file, Uint line, char *path, char *mode);

// Opens the path for appending, erasing any prior content of the same file
FILE *open_append(const char *path);

#endif

