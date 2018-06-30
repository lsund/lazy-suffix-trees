
#include "sort.h"


///////////////////////////////////////////////////////////////////////////////
// Globals


Suffix      *curr_sb;

Uint        occurrence[MAX_CHARS + 1];


///////////////////////////////////////////////////////////////////////////////
// Private functions


// Determine size for each group
static void set_groupsize(Suffix *left, Suffix *right, Uint plen)
{
    Suffix *curr_suffix;
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

    for (curr_suffix = left; curr_suffix <= right; curr_suffix++) {

        Uint head = **curr_suffix;
        if (sb.groupsize[head] > 0) {

            // 'allocate' the upper bound for the current character.
            // upper_bounds[head] now points to a allocated memory address,
            // enough space in distance from the last group
            upper_bounds[head] = lower_bound + sb.groupsize[head] - 1;
            lower_bound        = upper_bounds[head] + 1;
            sb.groupsize[head]   = 0;
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
        Uint head               = **curr_suffix;
        *(upper_bounds[head]--) = *curr_suffix;
    }
}


// Sorts the suffixes of a group, bounded between left and right.
void counting_sort(Suffix *left, Suffix *right)
{
    Suffix *upper_bounds[MAX_CHARS + 1];
    curr_sb = alloc_sortbuffer(left, right);


    // These suffixes belong to the same group, so the common prefix length
    // first has to be calculated.
    Uint plen = grouplcp(left, right);

    // Shortest suffix is sentinel, skip
    if (*right + plen == text.lst) {
        *right += plen;
        right--;
    }

    // Get the count (groupsize) of the different characters
    set_groupsize(left, right, plen);

    // determine right bound for each group
    set_group_bounds(left, right, upper_bounds);

    // Create buffer
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


// Determines the groups for all suffixes of the input string
//
// Scan all uffixes, determining the size of each group and then sorts the all
// suffixes into the array suffixes.
void create_suffix_groups(void)
{
    Wchar *c, **nextFree = text.ss;
    Uint a;

    Wchar **upper_bounds[MAX_CHARS + 1];

    // determine size for each group
    for (c = text.fst; c < text.fst + text.len; c++) {
        sb.groupsize[(Uint) *c]++;
    }

    for (c = text.cs; c < text.cs + text.asize; c++) {
        a                        = (Uint) *c;
        upper_bounds[a]          = nextFree + sb.groupsize[a] - 1;
        nextFree                 = upper_bounds[a] + 1;
        sb.groupsize[a] = 0;
    }

    // insert suffixes into array
    for (c = text.fst + text.len - 1; c >= text.fst; c--) {
        *(upper_bounds[*c]--) = c;
    }

    // suffix \$ is the largest suffix
    text.ss[text.len] = text.lst;
}
