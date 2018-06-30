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

Uint file_to_strings(char *name, Uint nlines, Wchar ***words);

void file_to_string(const char *filename);

FILE *open_append(const char *path);

void free_text();

#endif
