#include "wotd.h"

Uchar *text,                   // points to input string \(t\) of length \(n\)
      *sentinel,               // points to \(t[n]\) which is undefined
      characters[UCHAR_MAX + 1], // characters in \(t\) in alphabetical order
      **suffixes,              // array of pointers to suffixes of \(t\)
      **suffixbase,            // pointers into suffixes are w.r.t.\ this var
      **sbuffer,               // buffer to sort suffixes in \texttt{sortByChar}
      **sbufferspace = NULL;  // space to be used by \texttt{sbuffer}

Uint lastrootchild, maxwidth, branchcount, leafcount;

Uint  textlen,                 // length of \(t\)
      maxstacksize,
      alphasize,               // size of alphabet \(\Sigma\)
      alphaindex[UCHAR_MAX + 1], // index of characters in \(\Sigma\)
      occurrence[UCHAR_MAX + 1], // number of occurrences of character
      *streetab = NULL,        // table to hold suffix tree representation
      streetabsize,            // number of integers in \texttt{streetab} allocated
      *nextfreeentry,          // pointer to next unused element in \texttt{streetab}
      sbufferwidth,            // number of elements in \texttt{sbufferspace}
      maxsbufferwidth,         // maximal number of elements in \texttt{sbufferspace}
      suffixessize,            // number of unprocessed suffixes (for eager)
      maxunusedsuffixes,       // when reached, then move and halve space for suffixes
      rootchildtab[UCHAR_MAX + 1]; // constant time access to successors of \emph{root}

BOOL  rootevaluated;   // flag indicating that the root has been evaluated

#ifdef DEBUG

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

static void showpattern(Uchar *w, Uint wlen)
{
  (void) fwrite(w,sizeof(Uchar),(size_t) wlen,stderr);
}

static void wotd_benchmark(
                BOOL evaleager,
                char *argv[],
                int argc,
                float rho,
                Uint minpat,
                Uint maxpat
            )
{
    ArrayUint resultpos;
    inittree();
    if(evaleager)
    {
        initclock();
        evaluateeager();
        DEBUGCODE(3,showstreetab());
        DEBUGCODE(3,showtree());
        FREESPACE(suffixes);
    }
    INITARRAY(&resultpos,Uint);

    if(maxpat > 0 && maxpat <= textlen && rho != 0.0)
    {
        searchpattern(
                evaleager ? occurseager : occurslazy,
                argv,
                argc,
                (void *) &resultpos,
                text,
                textlen,
                rho,
                minpat,
                maxpat,
                showpattern,
                NULL
                );
    }

    FREEARRAY(&resultpos,Uint);
    DEBUG3(2,"#maxstack=%lu %lu %lu ",
            (Showuint) maxstacksize,
            (Showuint) textlen,
            (Showuint) NODEINDEX(nextfreeentry));
    DEBUG2(2,"%lu %.2f ",(Showuint) maxwidth,(double) maxwidth/textlen);
    DEBUG2(2,"%lu %.2f\n",(Showuint) sbufferwidth,(double) sbufferwidth/textlen);
    DEBUG4(2,"#q=%lu l=%lu %lu %.2f\n",
            (Showuint) branchcount,
            (Showuint) leafcount,
            (Showuint) (branchcount*BRANCHWIDTH+leafcount),
            (double) (4*(branchcount*BRANCHWIDTH+ leafcount))/textlen);
}

void wotd(BOOL evaleager, char ***patterns_p, int npatterns)
{

    char **patterns = *patterns_p;
    ArrayUint resultpos;

    if(!evaleager) {

        initclock();
        inittree();

    } else {

        inittree();
        initclock();
        evaluateeager();
        DEBUGCODE(3,showstreetab());
        DEBUGCODE(3,showtree());
        FREESPACE(suffixes);
    }

    INITARRAY(&resultpos,Uint);

    int j;
    for(j = 0; j < npatterns; j++) {
        printf("%s\n", patterns[j]);
        search_one_pattern(
                evaleager ? occurseager : occurslazy,
                (void *) &resultpos,
                text,
                textlen,
                strlen(patterns[j]),
                patterns[j]
            );
    }

    FREEARRAY(&resultpos,Uint);
    DEBUG3(2,"#maxstack=%lu %lu %lu ",
            (Showuint) maxstacksize,
            (Showuint) textlen,
            (Showuint) NODEINDEX(nextfreeentry));
    DEBUG2(2,"%lu %.2f ",(Showuint) maxwidth,(double) maxwidth/textlen);
    DEBUG2(2,"%lu %.2f\n",(Showuint) sbufferwidth,(double) sbufferwidth/textlen);
    DEBUG4(2,"#q=%lu l=%lu %lu %.2f\n",
            (Showuint) branchcount,
            (Showuint) leafcount,
            (Showuint) (branchcount*BRANCHWIDTH+leafcount),
            (double) (4*(branchcount*BRANCHWIDTH+ leafcount))/textlen);
}

