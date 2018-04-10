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

#include "debug.h"

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
static Bool debugwhere = False;

// Debug log file
static FILE *debugfileptr = NULL;

// Get the value of DEBUGLEVEL
Sint getdebuglevel(void) {
  return debuglevel;
}

// Get the value of DEBUGWHERE
Bool getdebugwhere(void) {
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
    Uint leftpointer, *vertex = stree;

    showrootchildtab();
    while(vertex < next_free)
    {
        if(IS_LEAF(vertex))
        {
            printf("#%lu: ",(Showuint) INDEX(vertex));
            leftpointer = GET_LP(vertex);
            printf(" Leaf %lu",(Showuint) leftpointer);
            if(IS_RIGHTMOST(vertex))
            {
                printf(" (last)");
            }
            vertex++;
        } else
        {
            printf("#%lu: ",(Showuint) INDEX(vertex));
            leftpointer = GET_LP(vertex);
            printf(" Branch(%lu,%lu)",(Showuint) leftpointer,
                    (Showuint) FIRST_CHILD(vertex));
            if(IS_RIGHTMOST(vertex))
            {
                printf(" (last)");
            }
            vertex += BRANCHWIDTH;
        }
        (void) putchar('\n');
    }
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
