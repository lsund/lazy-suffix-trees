#include "util.h"


Uint rand_range(Uint a, Uint b)
{
    return a == b ? a : (a + (drand48() * (double) (b - a + 1)));
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


void reverse(Wchar *s, Uint len)
{
    Wchar *fst, *back, tmp;

    for(fst = s, back = s + len - 1; fst < back; fst++, back--) {
        tmp   = *fst;
        *fst  = *back;
        *back = tmp;
    }
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
