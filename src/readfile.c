/*
  Copyright by Stefan Kurtz (C) 1998
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
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <unistd.h>
#include "types.h"
#include "debugdef.h"
#include "errordef.h"

#ifdef __cplusplus
#define THEMAPFAILED 0
#else
#endif

/*
  Open then file name, and deliver a file descriptor. The length of the
  file is written at the address textlen. The argument writefile is
  True, if and only if the file is to be openend for reading and writing.
*/

int fileOpen(char *name, Uint *textlen, BOOL writefile)
{
  int fd;
  struct stat buf;

  if((fd = open(name,(writefile) ? O_RDWR : O_RDONLY)) == -1)
  {
     ERROR1("fileOpen: Cannot open \"%s\"",name);
     return -1;
  }
  if(fstat(fd,&buf) == -1)
  {
     ERROR2("file \"%s\": fstat(fd = %d) failed",name,fd);
     return -2;
  }
  *textlen = (Uint) buf.st_size;
  return fd;
}

/*
  The following functions maps len bytes starting at
  offset offset from the file (or other object) specified by
  fd into memory. The mapping is writable iff writemap is True.
  The offset must be a multiple of the page size.
*/

/*@null@*/ caddr_t fileParts(int fd,Uint offset,Uint len,BOOL writemap)
{
  caddr_t addr;

#ifdef WITHSYSCONF
#ifdef DEBUG
  long pagesize = sysconf((Sysconfargtype)_SC_PAGESIZE);
  if(((long) offset) % pagesize != 0)
  {
    ERROR2("offset=%lu is not a multiple of pagesize %ld",
           (Showuint) offset,
           (Showsint) pagesize);
    return NULL;
  }
#endif
#endif
  addr = (caddr_t) mmap((caddr_t) 0,
              (size_t) len,
              writemap ? (PROT_READ | PROT_WRITE) : PROT_READ,
              MAP_PRIVATE,fd,(off_t) offset);
  if(addr == (caddr_t) MAP_FAILED)
  {
    ERROR4("fileParts(fd = %d, left = %ld, len = %ld, %s) failed",fd,
            (long) offset,(long) len,
            writemap ? "writable map" : "readable map");
    return NULL;
  }
  return addr;
}

/*
  Deletes the mappings for the specified address range.
*/

void freetextspace(Uchar *text, Uint textlen)
{
  (void) munmap((caddr_t) text,(size_t) textlen);
}

/*
  Generic file2String function:
  Open the file and map the entire file. The parameters writefile and
  writemap specify if the file is writeable or the map is writeable.
*/

/*@null@*/ caddr_t genfile2String(char *name, Uint *textlen,
                                  BOOL writefile, BOOL writemap)
{
  int fd;

  fd = fileOpen(name,textlen,writefile);
  if(fd < 0)
  {
    return NULL;
  }
  return fileParts(fd,0,*textlen,writemap);
}

/*
  file2String such tha file and map are not writable.
*/

/*@null@*/ caddr_t file2String(char *name, Uint *textlen)
{
  return genfile2String(name,textlen,False,False);
}
