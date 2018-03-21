/*
  Copyright by Stefan Kurtz (C) 1999-2003
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

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif /* HAVE_CONFIG_H */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>
#include "types.h"
#include "debugdef.h"
#include "chardef.h"
#include "boyermoore.h"
#include "debug.c"

#include "reverse.h"

#define MAXPATTERNLEN 1024

void searchpattern_benchmark(
        BOOL (*reallyoccurs) (void *,Uchar *,Uint,Uchar *,Uchar *),
        BOOL (*occurs) (void *,Uchar *,Uint,Uchar *,Uchar *),
        char *argv[],
        Argctype argc,
        void *occursinfo,
        Uchar *text,Uint textlen,
        float trialpercentage,
        Uint minpatternlen,
        Uint maxpatternlen,
        void (*showpattern)(void *,Uchar *,Uint),
        void *showpatterninfo,
        Uchar *mypattern
        )
{
    Uint pcount, j, trials, start, patternlen, patternstat[MAXPATTERNLEN+1] = {0};
    BOOL special, patternoccurs;
    Uchar pattern[MAXPATTERNLEN+1];
#ifdef DEBUG
    BOOL patternreallyoccurs;
#endif

    if(maxpatternlen > (Uint) MAXPATTERNLEN)
    {
        fprintf(stderr,"maxpatternlen=%lu > %lu\n",
                (Showuint) maxpatternlen,(Showuint) MAXPATTERNLEN);
        exit(EXIT_FAILURE);
    }
    if(maxpatternlen < minpatternlen)
    {
        fprintf(stderr,"maxpatternlen=%lu < %lu\n",(Showuint) maxpatternlen,
                (Showuint) minpatternlen);
        exit(EXIT_FAILURE);
    }
    if(textlen <= maxpatternlen)
    {
        fprintf(stderr,"textlen=%lu <= maxpatternlen = %lu\n",
                (Showuint) textlen,
                (Showuint) maxpatternlen);
        exit(EXIT_FAILURE);
    }

    if(trialpercentage >= 0.0) {
        trials = (Uint) (trialpercentage * textlen);
    } else {
        fprintf(stderr,"trialpercentage negative %f", trialpercentage);
        exit(EXIT_FAILURE);
    }

    printf("# trials %lu minpat %lu maxpat %lu\n",
            (Showuint) trials,
            (Showuint) minpatternlen,
            (Showuint) maxpatternlen);

    srand48(42349421);

    for(pcount=0; pcount < trials; pcount++)
    {
        if(minpatternlen == maxpatternlen) {
            patternlen = minpatternlen;
        } else {
            patternlen = (Uint) (minpatternlen +
                    (drand48() *
                     (double) (maxpatternlen-minpatternlen+1)));
        }
        patternstat[patternlen]++;

        start = (Uint) (drand48() * (double) (textlen-patternlen));

        if(start > textlen - patternlen) {
            fprintf(stderr,"Not enough characters left\n");
            exit(EXIT_FAILURE);
        }

        special = False;

        // Make patttern
        for(j = 0; j < patternlen; j++) {

            pattern[j] = text[start+j];

            if(ISSPECIAL(pattern[j])) {
                special = True;
                break;
            }
        }

        if(!special)
        {
            // Every second pattern
            if(pcount & 1)
            {
                reverse(pattern, patternlen); // Reverse every second string
            }

            patternoccurs =
                occurs(occursinfo, text, textlen, pattern, pattern+patternlen-1);


#ifdef DEBUG
            patternreallyoccurs =
                reallyoccurs(occursinfo,text,textlen,pattern,pattern+patternlen-1);
            if(patternoccurs != patternreallyoccurs)
            {
                showargs(argv,argc);
                fprintf(stderr,"pattern %lu: \"",(Showuint) pcount);\
                    showpattern(showpatterninfo,pattern,patternlen);
                fprintf(stderr,"\" %s found, this is not supposed to happen\n",
                        patternreallyoccurs ? "not" : "");
                exit(EXIT_FAILURE);
            }
#endif
        }
    }

    DEBUGCODE(1,showpatternstat(&patternstat[0]));

    DEBUG1(1,"%lu pattern processed as expected\n",(Showuint) trials);
}

    void search_one_pattern(
        BOOL (*occurs) (void *,Uchar *,Uint,Uchar *,Uchar *),
        void *occursinfo,
        Uchar *text,
        Uint textlen,
        Uint patternlen,
        char *mypattern
    )
{
    Uint j;
    BOOL special, patternoccurs;
    Uchar pattern[MAXPATTERNLEN+1];

    if(patternlen > (Uint) MAXPATTERNLEN)
    {
        fprintf(stderr,"maxpatternlen=%lu > %lu\n",
                (Showuint) patternlen,(Showuint) MAXPATTERNLEN);
        exit(EXIT_FAILURE);
    }
    if(textlen <= patternlen)
    {
        fprintf(stderr,"textlen=%lu <= maxpatternlen = %lu\n",
                (Showuint) textlen,
                (Showuint) patternlen);
        exit(EXIT_FAILURE);
    }

    special = False;
    char c;

    // Make patttern
    for(j = 0; j < patternlen; j++) {

        pattern[j] = mypattern[j];

        if(ISSPECIAL(pattern[j])) {
            special = True;
            c = pattern[j];
            break;
        }
    }

    if(!special) {

        patternoccurs =
            occurs(occursinfo, text, textlen, pattern, pattern+patternlen-1);
        printf("%d\n", patternoccurs);

    } else {
        fprintf(stderr, "Special character: %c", c);
        exit(EXIT_FAILURE);
    }
}

void searchpattern(
        BOOL(*occurs) (void *,Uchar *,Uint,Uchar *,Uchar *),
        char *argv[],
        Argctype argc,
        void *occursinfo,
        Uchar *text,
        Uint textlen,
        float trialpercentage,
        Uint minpatternlen,
        Uint maxpatternlen,
        void (*showpattern) (void *,Uchar *,Uint),
        void *showpatterninfo
    )
{
#ifdef DEBUG
  searchpattern_benchmark(bmhsearch,
#else
  searchpattern_benchmark(NULL,
#endif
                       occurs,
                       argv,
                       argc,
                       occursinfo,
                       text,
                       textlen,
                       trialpercentage,
                       minpatternlen,
                       maxpatternlen,
                       showpattern,
                       showpatterninfo,
                       NULL);
}

void searchpatternapprox(
        void(*apm)(void *,Uint,Uchar *,Uint,Uchar *,Uint),
        char *argv[],
        Argctype argc,
        void *occursinfo,
        float errorrate,
        Uchar *text,Uint textlen,float trialpercentage,
        Uint minpatternlen,
        Uint maxpatternlen
    )
{
  Uint i, trials, start, patternlen, threshold;
  Uchar pattern[MAXPATTERNLEN+1];

  if(maxpatternlen > (Uint) MAXPATTERNLEN)
  {
    fprintf(stderr,"maxpatternlen=%lu > %lu\n",
            (Showuint) maxpatternlen,(Showuint) MAXPATTERNLEN);
    exit(EXIT_FAILURE);
  }
  if(textlen <= maxpatternlen)
  {
    fprintf(stderr,"textlen=%lu <= maxpatternlen = %lu\n",
                   (Showuint) textlen,
                   (Showuint) maxpatternlen);
    exit(EXIT_FAILURE);
  }
  if(trialpercentage >= 0.0)
  {
    trials = (Uint) (trialpercentage * textlen);
  } else
  {
    trials = (Uint) -trialpercentage;
  }
  DEBUG2(2,"#trials %lu maxpat %lu\n",(Showuint) trials,
                                      (Showuint) maxpatternlen);
  srand48(42349421);

  for(i=0; i < trials; i++)
  {
    patternlen = minpatternlen +
                 (Uint) (drand48() * (double) (maxpatternlen-minpatternlen+1));
    threshold = (Uint) (errorrate * patternlen);
    DEBUG2(3,"m=%lu, k=%lu\n",(Showuint) patternlen,(Showuint) threshold);
    if(threshold >= minpatternlen)
    {
      fprintf(stderr,"threshold=%lu <= minpatternlen = %lu\n",
                  (Showuint) threshold,
                  (Showuint) minpatternlen);
      exit(EXIT_FAILURE);
    }
    start = (Uint) (drand48() * (double) (textlen-patternlen));
    memcpy(pattern,text + start,(size_t) patternlen);
    pattern[patternlen] = (Uchar) '\0';
    if(i & 1)
    {
      reverse(pattern,patternlen);
    }
    apm(occursinfo,threshold,pattern,patternlen,text,textlen);
  }
  DEBUG1(1,"%lu pattern processed as expected\n",
                  (Showuint) trials);
}
