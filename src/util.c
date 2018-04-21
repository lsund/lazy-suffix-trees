
#include "util.h"

void showpattern(Uchar *w, Uint wlen)
{
  (void) fwrite(w,sizeof(Uchar),(size_t) wlen,stderr);
}

void printtime()
{
    fprintf(stdout, "time: %.2f\n", getruntime());
}


Uint randlen(Uint m_min, Uint m_max)
{
    return m_min == m_max ? m_min : (m_min + (drand48() * (double) (m_max-m_min+1)));
}


size_t strlenw(Wchar *s)
{
    size_t i = 0;
    while (s[i]) {
        i++;
    }
    return i;
}

void parse_Uint(char *str, Uint *d)
{
    if(sscanf(str,"%ld", d) != 1 || d < 0) {
        fprintf(stderr, "Could not read int\n");
        exit(EXIT_FAILURE);
    }
}

void parse_float(char *str, float *f)
{
    if(sscanf(str,"%f", f) != 1 || f < 0) {
        fprintf(stderr, "Could not read float\n");
        exit(EXIT_FAILURE);
    }
}

