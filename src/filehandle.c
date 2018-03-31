/*
 * Copyright by Stefan Kurtz (C) 1999-2003
 * =====================================
 * You may use, copy and distribute this file freely as long as you
 * - do not change the file,
 * - leave this copyright notice in the file,
 * - do not make any profit with the distribution of this file
 * - give credit where credit is due
 * You are not allowed to copy or distribute this file otherwise
 * The commercial usage and distribution of this file is prohibited
 * Please report bugs and suggestions to <kurtz@zbh.uni-hamburg.de>
 *
 * ======================================
 *
 * Modified by Ludvig Sundström 2018 with permission from Stefan Kurtz
 * For full source control tree, see https://github.com/lsund/wotd
 *
 */

#include "io.h"

// The table of filehandles
static Filehandle *filehandle = NULL;

// Number of allocated filehandles
static Uint allocatedFilehandle = 0;

// Number of open files
static Uint currentopen = 0;


// The following three tables store important information to
// generate meaningfull error messages.
static Uint fromfileptr2filedesc(
        char *file,
        Uint line,
        BOOL existing,
        FILE *fp
    )
{

    FUNCTIONCALL;

    Filedesctype fd;
    fd = fileno(fp);
    if (fd == -1) {
        fprintf(stderr,"cannot find filedescriptor: %s\n", strerror(errno));
        exit(EXIT_FAILURE);
    }
    if (existing) {
        if (allocatedFilehandle <= (Uint) fd) {
            fprintf(stderr,"file %s, line %lu: cannot open file: fd=%lu, "
                    "not enough file handles available\n",
                    file,
                    (Showuint) line,
                    (Showuint) fd);
            exit(EXIT_FAILURE);
        } else {
            if(filehandle[fd].createfile == NULL)
            {
                fprintf(stderr,"file %s, line %lu: cannot open file: fd=%lu, "
                        "file handle not occurpied\n",
                        file,
                        (Showuint) line,
                        (Showuint) fd);
                exit(EXIT_FAILURE);
            }
        }
    }
    return (Uint) fd;
}

/*@null@*/ static char *fileptr2filename(char *file,
        Uint line,
        FILE *fp)
{
    FUNCTIONCALL;

    if(fp == stdout)
    {
        return "stdout";
    }
    if(fp == stderr)
    {
        return "stderr";
    }
    NOTSUPPOSEDTOBENULL(filehandle);
    return filehandle[fromfileptr2filedesc(file,line,True,fp)].createfile;
}

#define INCFILEHANDLES 16

static void assignfilehandleinformation(char *file,
                                        Uint line,
                                        const Uint fd,
                                        const char *path,
                                        const char *mode)
{
  FUNCTIONCALL;

  while(fd >= allocatedFilehandle)
  {
    Uint i;

    ALLOC(filehandle,
                     filehandle,
                     Filehandle,
                     allocatedFilehandle+INCFILEHANDLES);
    for(i=allocatedFilehandle; i<allocatedFilehandle+INCFILEHANDLES; i++)
    {
      filehandle[i].createfile = NULL;
    }
    allocatedFilehandle += INCFILEHANDLES;
  }
  NOTSUPPOSEDTOBENULL(filehandle);
  if(filehandle[fd].createfile != NULL)
  {
    fprintf(stderr,"file %s, line %lu: open file \"%s\" with mode \"%s\": "
                   "handle %lu already occupied\n",
           file,(Showuint) line,path,mode,(Showuint) fd);
    NOTSUPPOSED;
  }
  strcpy(filehandle[fd].createmode,mode);
  if(strlen(path) > PATH_MAX)
  {
    fprintf(stderr,"file %s, line %lu: cannot open file \"%s\": "
                   "path is too long\n",file,(Showuint) line,path);
    NOTSUPPOSED;
  }
  strcpy(filehandle[fd].path,path);
  filehandle[fd].createfile = file;
  filehandle[fd].createline = line;
  DEBUG5(2,"# file %s, line %lu: assign file \"%s\" for"
           " mode=%s to file handle %lu\n",
           file,(Showuint) line,path,mode,(Showuint) fd);
  currentopen++;
}

/*EE
  The following functions opens a file named \texttt{path}.
  The mode of the file is given by \texttt{mode}. The file
  pointer for the file is returned, or \texttt{NULL}, if the
  file could not be opened. Moreover, with each file created,
  a file handle is generated storing all relevant information.
*/

FILE *createfilehandle(
        char *file,
        const Uint line,
        const char *path,
        const char *mode
    )
{
  FILE *fp;

  FUNCTIONCALL;
  fp = fopen(path, mode);
  if(fp == NULL)
  {
    ERROR2("cannot open file \"%s\": %s",path,strerror(errno));
    return NULL;
  }
  assignfilehandleinformation(file,
                              line,
                              fromfileptr2filedesc(file,line,False,fp),
                              path,
                              mode);
  return fp;
}


