
#include "util.h"

void showpattern(Uchar *w, Uint wlen)
{
  (void) fwrite(w,sizeof(Uchar),(size_t) wlen,stderr);
}

void printtime(FILE *file)
{
    fprintf(file, "time: %.2f\n", getruntime());
}


Uint randlen(Uint m_min, Uint m_max)
{
    return m_min == m_max ? m_min : (m_min + (drand48() * (double) (m_max-m_min+1)));
}


size_t strlenw(Wchar *s)
{
    size_t i = 0;
    while (s[i] != 0) {
        i++;
    }
    if (i > 1000) {
        fprintf(stderr, "Line too long: %lu\n", i);
        exit(EXIT_FAILURE);
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

bool contains(Uint *arr, Uint len, Uint *vals)
{
    for (Uint i = 0; i < len; i++) {
        for (Uint j = 0; j < len; j++) {
            if (arr[i] == vals[j]) {
                break;
            }
            if (j == len - 1) {
                return false;
            }
        }
    }
    return true;
}
