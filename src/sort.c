
#include "sort.h"

// This array points to a list of suffixes

// Determine size for each group
static void get_count(Uchar **left, Uchar **right, Uint prefixlen)
{

    Uchar **c;
    for (c = left; c <= right; c++) {
        // drop the common prefix
        *c += prefixlen;
        occurrence[(Uint) **c]++;
    }
}


static void set_group_bounds(Uchar **left, Uchar **right, Uchar ***upper_bounds)
{
    // Sort buffer points to a sufficiently big allocated memory block
    Uchar **lower_bound = sort_buffer;
    Uchar **suffix;

    for (suffix = left; suffix <= right; suffix++) {

        Uint head = (Uint) **suffix;
        if (occurrence[head] > 0) {

            // 'allocate' the upper bound for the current character.
            // upper_bounds[head] now points to a allocated memory address,
            // enough space in distance from the last group
            upper_bounds[head] = lower_bound + occurrence[head] - 1;
            lower_bound        = upper_bounds[head] + 1;
            occurrence[head]   = '\0';

        }
    }
}


static void insert_suffixes(Uchar **left, Uchar **right, Uchar ***upper_bounds)
{
    Uchar **suffix;
    for (suffix = right; suffix >= left; suffix--) {
        // Decrement the pointer to bounds for this specific character,
        // then insert the suffix pointed to by c.
        //
        // This fills up the slot allocated for this group with suffix tree
        // addresses, end to start.
        Uchar head = **suffix;
        *(upper_bounds[head]--) = *suffix;
    }
}


void counting_sort(Uchar **left, Uchar **right, Uint prefixlen)
{
    Uchar **i = NULL;
    Uchar **j = NULL;


    Uchar **upper_bounds[UCHAR_MAX + 1];


    // Shortest suffix is sentinel, skip
    if (*right + prefixlen == sentinel) {
        *right += prefixlen;
        right--;
    }

    // Get the count (groupsize) of the different characters
    get_count(left, right, prefixlen);

    // determine right bound for each group
    set_group_bounds(left, right, upper_bounds);

    // insert suffixes into buffer
    insert_suffixes(left, right, upper_bounds);

    // copy grouped suffixes back
    for (i = left, j = sort_buffer; i <= right; i++, j++) {
        *i = *j;
    }
}

void create_suffix_groups(void)
{
    Uchar *c, **nextFree = suffixes;
    Uint a;

    Uchar **upper_bounds[UCHAR_MAX + 1];

    // determine size for each group
    for (c = text; c < text + textlen; c++) {
        occurrence[(Uint) *c]++;
    }

    for (c = characters; c < characters + alphasize; c++) {
        a = (Uint) *c;
        upper_bounds[a] = nextFree + occurrence[a] - 1;
        nextFree = upper_bounds[a] + 1;
        occurrence[a] = 0;
    }

    // insert suffixes into array
    for (c = text + textlen - 1; c >= text; c--) {
        *(upper_bounds[(Uint) *c]--) = c;
    }

    // suffix \$ is the largest suffix
    suffixes[textlen] = sentinel;
}
