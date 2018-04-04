
#include "sort.h"

Uchar **bound[UCHAR_MAX + 1];

void counting_sort(Uchar **left, Uchar **right, Uint prefixlen)
{
    Uchar **i, **j, **nextFree = sbuffer;
    Uint a;

    // Shortest suffix is sentinel, skip
    if (*right + prefixlen == sentinel) {
        *right += prefixlen;
        right--;
    }
    // Determine size for each group
    for (i = left; i <= right; i++) {
        // drop the common prefix
        *i += prefixlen;
        occurrence[(Uint) **i]++;
    }
    // determine right bound for each group
    for (i = left; i <= right; i++) {
        a = (Uint) **i;
        if (occurrence[a] > 0) {
            bound[a] = nextFree + occurrence[a] - 1;
            nextFree = bound[a] + 1;
            occurrence[a] = 0;
        }
    }
    // insert suffixes into buffer
    for (i = right; i >= left; i--) {
        *(bound[(Uint) **i]--) = *i;
    }
    // copy grouped suffixes back
    for (i=left, j = sbuffer; i <= right; i++, j++) {
        *i = *j;
    }
}

void create_suffix_groups(void)
{
    Uchar *c, **nextFree = suffixes;
    Uint a;

    // determine size for each group
    for (c = text; c < text + textlen; c++) {
        occurrence[(Uint) *c]++;
    }

    for (c = characters; c < characters + alphasize; c++) {
        a = (Uint) *c;
        bound[a] = nextFree + occurrence[a] - 1;
        nextFree = bound[a] + 1;
        occurrence[a] = 0;
    }

    // insert suffixes into array
    for (c = text + textlen - 1; c >= text; c--) {
        *(bound[(Uint) *c]--) = c;
    }

    // suffix \$ is the largest suffix
    suffixes[textlen] = sentinel;
}
