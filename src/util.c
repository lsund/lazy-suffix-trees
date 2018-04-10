
#include "util.h"

void showpattern(Uchar *w, Uint wlen)
{
  (void) fwrite(w,sizeof(Uchar),(size_t) wlen,stderr);
}

void printtime()
{
    fprintf(stdout, "time: %.2f\n", getruntime() / (double) ITER);
}


Uint randlen(Uint m_min, Uint m_max)
{
    return m_min == m_max ? m_min : (m_min + (drand48() * (double) (m_max-m_min+1)));
}


