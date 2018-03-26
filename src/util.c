
#include "util.h"

// Print a string to stdout
void showstring(Uchar *left, Uchar *right)
{
  Uchar *ptr;
  for(ptr = left; ptr <= right; ptr++)
  {
    if(ptr == sentinel)
    {
      (void) putchar('~');
    } else
    {
      (void) putchar((Fputcfirstargtype) *ptr);
    }
  }
}


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

    DEBUGCODE(3,showstree());
    DEBUGCODE(3,showtree());
    FREESPACE(suffixes);

    return maxstacksize;
}
