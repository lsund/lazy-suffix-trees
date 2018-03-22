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

static void showstreetab(void)
{
  Uint leftpointer, *nodeptr = streetab;

  showrootchildtab();
  while(nodeptr < nextfreeentry)
  {
    if(ISLEAF(nodeptr))
    {
      printf("#%lu: ",(Showuint) NODEINDEX(nodeptr));
      leftpointer = GETLP(nodeptr);
      printf(" Leaf %lu",(Showuint) leftpointer);
      if(ISRIGHTMOSTCHILD(nodeptr))
      {
        printf(" (last)");
      }
      nodeptr++;
    } else
    {
      printf("#%lu: ",(Showuint) NODEINDEX(nodeptr));
      leftpointer = GETLP(nodeptr);
      printf(" Branch(%lu,%lu)",(Showuint) leftpointer,
                                (Showuint) GETFIRSTCHILD(nodeptr));
      if(ISRIGHTMOSTCHILD(nodeptr))
      {
        printf(" (last)");
      }
      nodeptr += BRANCHWIDTH;
    }
    (void) putchar('\n');
  }
}

static Uint getedgelen(Uint *nodeptr)
{
  return GETLP(streetab + GETFIRSTCHILD(nodeptr)) - GETLP(nodeptr);
}

static void showsubtree(Uint *father,int indent);

static void scanedgelist(Uint firstchar,Uint *firstsucc,int indent)
{
  Uint leftpointer, edgelen, *nodeptr = firstsucc;
  Uchar *lefttext;

  while(True)
  {
    if(ISLEAF(nodeptr))
    {
      leftpointer = GETLP(nodeptr);
      lefttext = text + leftpointer;
      if(lefttext == sentinel)
      {
        if(firstchar == (Uint) (UCHAR_MAX+1))
        {
          printf("%*s~\n",indent,"");
        }
      } else
      {
        if(firstchar == *lefttext)
        {
          printf("%*s",indent,"");
          showstring(lefttext, sentinel);
          (void) putchar('\n');
        }
      }
      if(ISRIGHTMOSTCHILD(nodeptr))
      {
        break;
      }
      nodeptr++;
    } else
    {
      leftpointer = GETLP(nodeptr);
      lefttext = text + leftpointer;
      if(firstchar == *lefttext)
      {
        edgelen = getedgelen(nodeptr);
        printf("%*s",indent,"");
        showstring(lefttext,lefttext + edgelen - 1);
        (void) putchar('\n');
        showsubtree(nodeptr,indent+6);
      }
      if(ISRIGHTMOSTCHILD(nodeptr))
      {
        break;
      }
      nodeptr += BRANCHWIDTH;
    }
  }
}

static void showsubtree(Uint *father,int indent)
{
  Uint *nodeptr;
  Uchar *cptr;

  DEBUG1(5,"showsubtree(%lu)\n",(Showuint) NODEINDEX(father));
  nodeptr = streetab + GETFIRSTCHILD(father);
  for(cptr = characters; cptr < characters+alphasize; cptr++)
  {
    scanedgelist((Uint) *cptr,nodeptr,indent);
  }
  scanedgelist((Uint) (UCHAR_MAX+1),nodeptr,indent);
}

static void showtree(void)
{
  Uint leafnum, edgelen, *nodeptr, *rcptr;
  Uchar *lefttext;

  for(rcptr = rootchildtab; rcptr <= rootchildtab+UCHAR_MAX; rcptr++)
  {
    if(*rcptr != UNDEFINEDSUCC)
    {
      if(ISLEAF(rcptr))
      {
        leafnum = *rcptr & ~LEAFBIT;
        lefttext = text + leafnum;
        showstring(lefttext,sentinel);
        (void) putchar('\n');
      } else
      {
        nodeptr = streetab + *rcptr;
        lefttext = text + GETLP(nodeptr);
        edgelen = getedgelen(nodeptr);
        showstring(lefttext,lefttext + edgelen - 1);
        (void) putchar('\n');
        showsubtree(nodeptr,6);
      }
    }
  }
  printf("~\n");
}

#endif
