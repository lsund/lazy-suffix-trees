
#include "sort.h"

Uchar **bound[UCHAR_MAX + 1];

// Determine size for each group
static void get_count(Uchar **arr, Uint len)
{
    // This can be too small
    /* for (c = left; c <= right; c++) { */
    /*     // drop the common prefix */
    /*     occurrence[(Uint) **c]++; */
    /* } */
    for (Uint i = 0; i <= len; i++) {
        // drop the common prefix
        /* *c += prefixlen; */
        /* printf("arr: %d\n", arr[i]); */
        occurrence[(Uint) *arr[i]]++;
    }
}

static void set_bounds(Uchar **arr, Uint len)
{
    Uint a;
    Uchar **nextFree = sbuffer;
    for (Uint i = 0; i <= len; i++) {
        a = (Uint) *arr[i];
        if (occurrence[a] > 0) {
            bound[a] = nextFree + occurrence[a] - 1;
            nextFree = bound[a] + 1;
            occurrence[a] = 0;
        }
    }
}


static void insert_suffixes(Uchar **arr, Uint len)
{

    for (int i = len; i >= 0; i--) {
        *(bound[(Uint) *arr[i]]--) = arr[i];
    }
}


void counting_sort(Uchar **left, Uchar **right, Uint prefixlen)
{
    Uchar **c = NULL;
    Uchar **j = NULL;

    // Shortest suffix is sentinel, skip
    if (*right + prefixlen == sentinel) {
        *right += prefixlen;
        right--;
    }

    Uchar *arr[500000];
    c = left;
    for (int i = 0; i <= right - left; i++, c++) {
        *c += prefixlen;
        arr[(Uint) i] = *c;
    }

    get_count(arr, right - left);

    // determine right bound for each group
    set_bounds(arr, right - left);

    // insert suffixes into buffer
    insert_suffixes(arr, right - left);

    // copy grouped suffixes back
    for (c = left, j = sbuffer; c <= right; c++, j++) {
        *c = *j;
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
