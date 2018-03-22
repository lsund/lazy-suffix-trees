/*
  Copyright by Stefan Kurtz (C) 1997-2003
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

//\Ignore{

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include "types.h"
#include "fhandledef.h"
#include "debugdef.h"
#include "errordef.h"
#include "failures.h"
#include "megabytes.h"

#include "filehandle.pr"

#define MAXPATTERNLEN 1024

//}

/*EE
  This module defines functions for handling debug levels and
  other information related to producing debugging messages.
  The debug mechanism is only available if the \texttt{DEBUG} compiler
  flag is used.
*/

#ifdef DEBUG

static Sint debuglevel = 0;        // the value of \texttt{DEBUGLEVEL}
static BOOL debugwhere = False;    // the value of \texttt{DEBUGWHERE}
/*@null@*/ static FILE
           *debugfileptr = NULL;  // the file pointer to show the debug info

/*EE
  The following function returns the \texttt{DEBUGLEVEL}.
*/

Sint getdebuglevel(void)
{
  return debuglevel;
}

/*EE
  The following function returns the value of \texttt{DEBUGWHERE}.
*/

BOOL getdebugwhere(void)
{
  return debugwhere;
}

/*EE
  The following function sets the debug level by looking up the
  environment variable \texttt{DEBUGLEVEL}. Moreover, the environment
  variable \texttt{DEBUGWHERE} is read and \texttt{debugwhere} is set
  accordingly.
*/

void setdebuglevel(void)
{
  char *envstring;
  Scaninteger readint;

  debugfileptr = stdout;
  if((envstring = getenv("DEBUGLEVEL")) != NULL)
  {
    if(!(strlen(envstring) == (size_t) 1 &&
       isdigit((Ctypeargumenttype) *envstring)))
    {
      fprintf(stderr,"environment variable DEBUGLEVEL=%s, ",envstring);
      fprintf(stderr,"it must be a digit between 0 and %lu\n",
              (Showuint) MAXDEBUGLEVEL);
      exit(EXIT_FAILURE);
    }
    if (sscanf(envstring,"%ld",&readint) != 1 || readint < 0 ||
        readint > (Scaninteger) MAXDEBUGLEVEL)
    {
      fprintf(stderr,"environment variable DEBUGLEVEL=%s, ",envstring);
      fprintf(stderr,"it must be a digit between 0 and %lu\n",
              (Showuint) MAXDEBUGLEVEL);
      exit(EXIT_FAILURE);
    }
    debuglevel = (Sint) readint;
  }
  if((envstring = getenv("DEBUGWHERE")) != (char *) NULL)
  {
    if(strcmp(envstring,"on") == 0)
    {
      debugwhere = True;
    } else
    {
      if(strcmp(envstring,"off") == 0)
      {
        debugwhere = False;
      } else
      {
        fprintf(stderr,"environment variable DEBUGWHERE=%s, ",envstring);
        fprintf(stderr,"it must be set to \"on\" or \"off\"\n");
        exit(EXIT_FAILURE);
      }
    }
  }
  CHECKALLTYPESIZES
#ifdef WITHSYSCONF
  showmemsize();
#endif
}

/*EE
  The following function opens the given filename for writing the debug
  messages  to. It also sets the debug level.
  This function is called only very rarely. If only \texttt{setdebuglevel}
  is called, then the output goes to standard output.
*/

void setdebuglevelfilename(char *filename)
{
  debugfileptr = CREATEFILEHANDLE(filename,WRITEMODE);
  if(debugfileptr == NULL)
  {
    fprintf(stderr,"%s\n",messagespace());
    NOTSUPPOSED;
  }
  setdebuglevel();
}

/*EE
  The following function looks up the output pointer.
*/

FILE *getdbgfp(void)
{
  if(debugfileptr == NULL)
  {
    fprintf(stderr,"DEBUGLEVELSET not called\n");
    exit(EXIT_FAILURE);
  }
  return debugfileptr;
}

/*EE
  The following function closes the debug output pointer, if it is not
  standard out.
*/

void debugclosefile(void)
{
  if(debugfileptr == NULL)
  {
    fprintf(stderr,"cannot close debugfileptr\n");
    exit(EXIT_FAILURE);
  }
  if(DELETEFILEHANDLE(debugfileptr) != 0)
  {
    fprintf(stderr,"%s\n",messagespace());
    NOTSUPPOSED;
  }
}

static void showargs(char *argv[], Argctype argc)
{
    Argctype argnum;
    for(argnum = 0; argnum < argc; argnum++)
    {
        fprintf(stderr,"%s ",argv[argnum]);
    }
    (void) putc('\n',stderr);
}

static void showpatternstat(Uint *patternstat)
{
    Uint i;

    for(i=0; i<= (Uint) MAXPATTERNLEN; i++)
    {
        if(patternstat[i] > 0)
        {
            printf("%lu patterns of length %lu\n",(Showuint) patternstat[i],
                    (Showuint) i);
        }
    }
}


extern Uint *rootchildtab, textlen;

void showrootchildtab(void)
{
    Uint i;

    for(i=0; i<=UCHAR_MAX; i++)
    {
        if(rootchildtab[i] != UNDEFINEDSUCC)
        {
            if(rootchildtab[i] & LEAFBIT)
            {
                printf("#(%lu)%c-successor of root is leaf %lu\n",
                        (Showuint) i,
                        (char) i,
                        (Showuint) (rootchildtab[i] & ~LEAFBIT));
            } else
            {
                printf("#(%lu)%c-successor of root is branch %ld\n",
                        (Showuint) i,
                        (char) i,
                        (Showsint) rootchildtab[i]);
            }
        }
    }
    printf("#~-successor of root is leaf %lu\n",(Showuint) textlen);
}

#endif
