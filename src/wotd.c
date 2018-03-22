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

