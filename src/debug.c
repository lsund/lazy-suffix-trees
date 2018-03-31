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


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include "types.h"
#include "io.h"
#include "debug.h"
#include "error.h"

/*EE
  This module defines functions for handling debug levels and
  other information related to producing debugging messages.
  The debug mechanism is only available if the \texttt{DEBUG} compiler
  flag is used.
*/

#ifdef DEBUG

// DEBUGLEVEL
static Sint debuglevel = 0;

// DEBUGWHERE
static BOOL debugwhere = False;

// Debug log file
static FILE *debugfileptr = NULL;

// Get the value of DEBUGLEVEL
Sint getdebuglevel(void) {
  return debuglevel;
}

// Get the value of DEBUGWHERE
BOOL getdebugwhere(void) {
  return debugwhere;
}

// Set DEBUGLEVEL
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
}


// Set debug file name
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


// Lookup output pointer
FILE *getdbgfp(void)
{
  if(debugfileptr == NULL)
  {
    fprintf(stderr,"DEBUGLEVELSET not called\n");
    exit(EXIT_FAILURE);
  }
  return debugfileptr;
}


// Print the arguments in argv
static void showargs(char *argv[], Argctype argc)
{
    Argctype argnum;
    for(argnum = 0; argnum < argc; argnum++) {
        fprintf(stderr,"%s ",argv[argnum]);
    }
    (void) putc('\n',stderr);
}


// Print how many patterns were searched of length 1-n
void showpatternstat(Uint *patternstat)
{
    Uint i;

    for(i=0; i<= (Uint) MAXPATTERNLEN; i++) {
        if(patternstat[i] > 0) {
            printf("%lu patterns of length %lu\n",(Showuint) patternstat[i],
                    (Showuint) i);
        }
    }
}


// Print the successor of the root
void showrootchildtab(void)
{
    Uint i;

    for (i=0; i<=UCHAR_MAX; i++) {
        if (rootchildtab[i] != UNDEFINEDSUCC) {
            if (rootchildtab[i] & LEAFBIT) {
                printf("#(%lu)%c-successor of root is leaf %lu\n",
                        (Showuint) i,
                        (char) i,
                        (Showuint) (rootchildtab[i] & ~LEAFBIT));
            } else {
                printf("#(%lu)%c-successor of root is branch %ld\n",
                        (Showuint) i,
                        (char) i,
                        (Showsint) rootchildtab[i]);
            }
        }
    }
    printf("#~-successor of root is leaf %lu\n",(Showuint) textlen);
}


// Print a string representation of the suffix tree
void showstree(void)
{
    Uint leftpointer, *nodeptr = stree;

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


// How long is the edge below this node?
static Uint getedgelen(Uint *nodeptr)
{
    return GETLP(stree + GETFIRSTCHILD(nodeptr)) - GETLP(nodeptr);
}


static void scanedgelist(Uint firstchar,Uint *firstsucc,int indent) {
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


// Print a string representation of a subtree
void showsubtree(Uint *father,int indent) {
  Uint *nodeptr;
  Uchar *cptr;

  DEBUG1(5,"showsubtree(%lu)\n",(Showuint) NODEINDEX(father));
  nodeptr = stree + GETFIRSTCHILD(father);
  for(cptr = characters; cptr < characters+alphasize; cptr++)
  {
    scanedgelist((Uint) *cptr,nodeptr,indent);
  }
  scanedgelist((Uint) (UCHAR_MAX+1),nodeptr,indent);
}


// Print a string representation of a tree
void showtree(void)
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
                nodeptr = stree + *rcptr;
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


// Print the alphabet and its size
void printAlpha (char *s)
{
    Uint i, j, occ[UCHAR_MAX+1] = {0};
    char *scopy;

    for (scopy = s; *scopy != '\0'; scopy++) {
        occ[(int) *scopy]++;
    }
    printf("alpha=(");
    for (j=0, i=0; i<=(Uint) UCHAR_MAX; i++) {
        if (occ[i] > 0) {
            (void) putchar((char) i);
            j++;
        }
    }
    printf(")\nalphasize=%lu\n",(Showuint) j);
    for (i=0; i<=(Uint) UCHAR_MAX; i++) {
        if (occ[i] > 0) {
            printf("'%c':%lu\n",(char) i,(Showuint) occ[i]);
        }
    }
}


#endif
