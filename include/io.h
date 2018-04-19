
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
#include "search.h"

///////////////////////////////////////////////////////////////////////////////
// Macros

// Writing binary mode
#define WRITEMODE  "wb"
// Reading binary mode
#define READMODE   "rb"
// Appending binary mode
#define APPENDMODE "ab"

///////////////////////////////////////////////////////////////////////////////
// Functions


Uint file_to_strings(char *name, Uint *textlen, Uint nlines, wchar_t ***words);

// Opens the path for appending, erasing any prior content of the same file
FILE *open_append(const char *path);

void freetextspace();

#endif

