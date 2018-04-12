
#include "sort.h"

// This array points to a list of suffixes

// Determine size for each group
static void get_count(Uchar **left, Uchar **right, Uint prefixlen)
{
    Uchar **suffix_probe;
    for (suffix_probe = left; suffix_probe <= right; suffix_probe++) {
        // drop the common prefix
        *suffix_probe += prefixlen;
        suffixhead_occurrence[(Uint) **suffix_probe]++;
    }
}


static void set_group_bounds(Uchar **left, Uchar **right, Uchar ***upper_bounds)
{
    // Sort buffer is already allocated, a sufficiently large memory block for
    // all suffix poniters.
    Uchar **lower_bound = current_sortbuffer;
    Uchar **suffix_probe;

    for (suffix_probe = left; suffix_probe <= right; suffix_probe++) {

        Uint head = **suffix_probe;
        if (suffixhead_occurrence[head] > 0) {

            // 'allocate' the upper bound for the current character.
            // upper_bounds[head] now points to a allocated memory address,
            // enough space in distance from the last group
            upper_bounds[head] = lower_bound + suffixhead_occurrence[head] - 1;
            lower_bound        = upper_bounds[head] + 1;
            suffixhead_occurrence[head]   = '\0';
        }
    }
}


static void insert_suffixes(Uchar **left, Uchar **right, Uchar ***upper_bounds)
{
    Uchar **suffix_probe;
    for (suffix_probe = right; suffix_probe >= left; suffix_probe--) {
        // Decrement the pointer to bounds for this specific character,
        // then insert the suffix pointed to by c.
        //
        // This fills up the slot allocated for this group with suffix tree
        // addresses, end to start.
        Uint head               = **suffix_probe;
        *(upper_bounds[head]--) = *suffix_probe;
    }
}


void counting_sort(Uchar **left, Uchar **right, Uint prefixlen)
{
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

    // Create buffer
    insert_suffixes(left, right, upper_bounds);

    Uchar **suffix_probe = left;
    Uchar **buffer_probe = current_sortbuffer;

    // copy grouped suffixes back from buffer
    while (suffix_probe <= right) {

        *suffix_probe = *buffer_probe;

        suffix_probe++;
        buffer_probe++;
    }
}

void create_suffix_groups(void)
{
    Uchar *c, **nextFree = suffixes;
    Uint a;

    Uchar **upper_bounds[UCHAR_MAX + 1];

    // determine size for each group
    for (c = text; c < text + textlen; c++) {
        suffixhead_occurrence[(Uint) *c]++;
    }

    for (c = characters; c < characters + alphasize; c++) {
        a               = (Uint) *c;
        upper_bounds[a] = nextFree + suffixhead_occurrence[a] - 1;
        nextFree        = upper_bounds[a] + 1;
        suffixhead_occurrence[a]   = 0;
    }

    // insert suffixes into array
    for (c = text + textlen - 1; c >= text; c--) {
        *(upper_bounds[(Uint) *c]--) = c;
    }

    // suffix \$ is the largest suffix
    suffixes[textlen] = sentinel;
}
