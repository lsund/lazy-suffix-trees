
#include "sort.h"

Uchar **bound[UCHAR_MAX + 1];


Uint *charmap;

static void map_add(Uchar a)
{
    *(charmap + a) = (Uint) a;
}

// Determine size for each group
static void determine_groupsize(Uchar **c, Uchar **left, Uchar **right, Uint prefixlen)
{
    for (c = left; c <= right; c++) {
        // drop the common prefix
        *c += prefixlen;
        /* printf("%zu\n", sizeof(Uchar)); */
        occurrence[(Uint) **c]++;
    }
}


static void determine_bounds(Uchar **i, Uchar **left, Uchar **right)
{
    Uint a;
    Uchar **nextFree = sbuffer;
    for (i = left; i <= right; i++) {
        a = (Uint) **i;
        if (occurrence[a] > 0) {
            bound[a] = nextFree + occurrence[a] - 1;
            nextFree = bound[a] + 1;
            occurrence[a] = 0;
        }
    }
}


static void insert_suffixes(Uchar **i, Uchar **left, Uchar **right)
{
    for (i = right; i >= left; i--) {
        *(bound[(Uint) **i]--) = *i;
    }
}


void counting_sort(Uchar **left, Uchar **right, Uint prefixlen)
{
    Uchar **i = NULL;
    Uchar **j = NULL;

    // Shortest suffix is sentinel, skip
    if (*right + prefixlen == sentinel) {
        *right += prefixlen;
        right--;
    }

    if (False) {
        charmap = calloc(UCHAR_MAX + 1, sizeof(Uint));
        // Add characters to map
        for (Uchar i = 0; i < UCHAR_MAX; i++) {
            map_add(i);
        }
        for (Uchar i = 0; i < UCHAR_MAX; i++) {
            printf("%zu\n", charmap[i]);
        }
        exit(EXIT_SUCCESS);
    }

    determine_groupsize(i, left, right, prefixlen);

    // determine right bound for each group
    determine_bounds(i, left, right);

    // insert suffixes into buffer
    insert_suffixes(i, left, right);

    // copy grouped suffixes back
    for (i = left, j = sbuffer; i <= right; i++, j++) {
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
