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
#include <wchar.h>
#include <stdbool.h>
#include "spaceman.h"
#include "types.h"
#include "search.h"

// Writing binary mode
#define WRITEMODE  "wb"
// Reading binary mode
#define READMODE   "rb"
// Appending binary mode
#define APPENDMODE "ab"

// Load the patterns in the file `filename` into the `patterns` variable. The
// input file is assumed to consist of a number of newline separated patterns,
// each not longer than MAX_PATTERNLEN as defined in config.h and the total
// number of patterns not longer than MAX_NPATTERNS as defined in config.h.
// `patterns` will give access to a list of pointers, each pointing to a
// separate patterns as read in from the file. `nlines` will take on the value
// of the number of lines read from the file.
Uint patterns_initialize(char *filename, Uint nlines, Wchar ***patterns);

// Initializes the global variable `text` depending on the content of
// `filename`, setting its respective fields to the correct values.
void text_initialize(const char *filename);

// Truncates the file, then open it for appending. Returned is the FILE pointer
// to the opened file.
FILE *truncate_open_append(const char *path);

// 'Destroys' the global variable `text` by freeing and unmapping its allocated
// memory.
void text_destroy();

#endif
