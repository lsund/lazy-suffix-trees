
#ifndef READFILE_H
#define READFILE_H

#include "types.h"
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include "types.h"
#include "debug.h"

int fileOpen(char *name, Uint *textlen, BOOL writefile);

caddr_t fileParts(int fd,Uint offset,Uint len,BOOL writemap);

void freetextspace(Uchar *text, Uint textlen);

caddr_t genfile2String(char *name, Uint *textlen,
                                  BOOL writefile, BOOL writemap);

caddr_t file2String(char *name, Uint *textlen);

int file2Array(char *name, Uint *textlen, int nlines, char ***words);

#endif
