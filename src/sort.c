#include "sort.h"


// The current sortbuffer
Suffix      *curr_sb;


// Determine the group sizes for the suffixes between left and right
static void set_groupsize(Suffix *left, Suffix *right, Uint plen)
{
    Suffix *curr_suffix;
    // Iterate the interval
    for (curr_suffix = left; curr_suffix <= right; curr_suffix++) {
        // drop the common prefix
        *curr_suffix += plen;

        Uint fst = **curr_suffix;
        sb.groupsize[fst]++;
    }
}


// Set the upper and lower bounds for each group, delimited by leaft and right
static void set_group_bounds(Suffix *left, Suffix *right, Wchar ***upper_bounds)
{
    // `curr_sb` is already allocated, a sufficiently large memory
    // block for all suffix pointers.
    Suffix *lower_bound = curr_sb;
    Suffix *curr_suffix;

    // Iterate the interval
    for (curr_suffix = left; curr_suffix <= right; curr_suffix++) {

        Uint fst = **curr_suffix;
        if (sb.groupsize[fst] > 0) {

            // 'allocate' the upper bound for the current character.
            // upper_bounds[fst] now points to a allocated memory address,
            // enough space in distance from the last group.
            upper_bounds[fst] = lower_bound + sb.groupsize[fst] - 1;
            lower_bound       = upper_bounds[fst] + 1;
            sb.groupsize[fst] = 0;
        }
    }
}


// Insert the suffixes into the correct positions, depending on `upper_bonuds`
static void insert_suffixes(Suffix *left, Suffix *right, Wchar ***upper_bounds)
{
    Suffix *curr_suffix;
    for (curr_suffix = right; curr_suffix >= left; curr_suffix--) {
        // Decrement the pointer to bounds for this specific character,
        // then insert the suffix pointed to by c.
        //
        // This fills up the slot allocated for this group with suffix tree
        // addresses, end to start.
        Uint fst               = **curr_suffix;
        *(upper_bounds[fst]--) = *curr_suffix;
    }
}


///////////////////////////////////////////////////////////////////////////////
// Public API


void counting_sort(Suffix *left, Suffix *right)
{
    Suffix *upper_bounds[MAX_CHARS + 1];
    curr_sb = alloc_sortbuffer(left, right);

    // These suffixes belong to the same group, so the common prefix length
    // first has to be calculated.
    Uint plen = suffixgroup_lcp(left, right);

    // Shortest suffix is sentinel, skip
    if (*right + plen == text.lst) {
        *right += plen;
        right--;
    }

    // Get the count (groupsize) of the different characters
    set_groupsize(left, right, plen);

    // determine right bound for each group
    set_group_bounds(left, right, upper_bounds);

    // insert sorted
    insert_suffixes(left, right, upper_bounds);

    Suffix *curr_suffix = left;
    Suffix *curr_buffersuffix = curr_sb;

    // copy grouped suffixes back from buffer
    while (curr_suffix <= right) {

        *curr_suffix = *curr_buffersuffix;

        curr_suffix++;
        curr_buffersuffix++;
    }
}


void initialize_suffixes(void)
{
    Wchar **upper_bounds[MAX_CHARS + 1];

    // Compute number of suffixes starting with each character.
    Suffix s;
    for (s = text.fst; s < text.fst + text.len; s++) {
        sb.groupsize[(Uint) *s]++;
    }

    // For each character a, store the suffix which is at offset groupsize[a]
    // in upper_bounds.
    Uint a;
    Suffix *next = text.ss;
    for (Wchar *c = text.cs; c < text.cs + text.asize; c++) {
        a               = (Uint) *c;
        upper_bounds[a] = next + sb.groupsize[a] - 1;
        next            = upper_bounds[a] + 1;
        sb.groupsize[a] = 0;
    }

    // Fill the suffix array with the suffix groups
    for (s = text.fst + text.len - 1; s >= text.fst; s--) {
        *(upper_bounds[*s]--) = s;
    }

    // set the longest suffix
    text.ss[text.len] = text.lst;
}
