
#include "util.h"

void showpattern(Uchar *w, Uint wlen)
{
  (void) fwrite(w,sizeof(Uchar),(size_t) wlen,stderr);
}

Uint wotdtreesize(
        Uchar *textarg,
        Uint textlenarg,
        Uchar *alphabet,
        Uint alphasizearg
        )
{
    text      = textarg;
    textlen   = textlenarg;
    alphasize = alphasizearg;

    memcpy(characters, alphabet, sizeof(Uchar) * alphasize);
    inittree();
    evaluateeager();

    FREE(suffixes);

    return maxstacksize;
}

void printtime()
{
    fprintf(stdout, "time: %.2f\n", getruntime() / (double) ITER);
}
