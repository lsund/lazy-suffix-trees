#include "search.h"

void searchpatternapprox(
        void(*apm)(void *,Uint,Uchar *,Uint,Uchar *,Uint),
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
