
#include "sort.h"


///////////////////////////////////////////////////////////////////////////////
// Globals


Wchar       **current_sortbuffer;

Uint        occurrence[MAX_CHARS + 1];


///////////////////////////////////////////////////////////////////////////////
// Private functions


// Increases the count for this particular character
static void increase_count(Uint codepoint)
{
    /* int i = get_index(codepoint); */
    Uint i = codepoint;
    sortbuffer.suffixhead_count[i]++;
}


// Determine size for each group
static void get_count(Wchar **left, Wchar **right, Uint prefixlen)
{
    Wchar **suffix_probe;
    for (suffix_probe = left; suffix_probe <= right; suffix_probe++) {
        // drop the common prefix
        *suffix_probe += prefixlen;

        Uint head      = **suffix_probe;
        // Here, a conversion is needed to get the right index. We cannot just
        // index on the value of the character, it needs to be mapped to an
        // integer range, say 1 - 4096
        increase_count((Uint) head);
    }
}


// Set the upper and lower bounds for each group, delimited by leaft and right
static void set_group_bounds(Wchar **left, Wchar **right, Wchar ***upper_bounds)
{
    // `current_sortbuffer` is already allocated, a sufficiently large memory
    // block for all suffix pointers.
    Wchar **lower_bound = current_sortbuffer;
    Wchar **suffix_probe;

    for (suffix_probe = left; suffix_probe <= right; suffix_probe++) {

        Uint head = **suffix_probe;
        if (sortbuffer.suffixhead_count[head] > 0) {

            // 'allocate' the upper bound for the current character.
            // upper_bounds[head] now points to a allocated memory address,
            // enough space in distance from the last group
            upper_bounds[head] = lower_bound + sortbuffer.suffixhead_count[head] - 1;
            lower_bound        = upper_bounds[head] + 1;
            sortbuffer.suffixhead_count[head]   = 0;
        }
    }
}


// Insert the suffixes into the correct positions, depending on `upper_bonuds`
static void insert_suffixes(Wchar **left, Wchar **right, Wchar ***upper_bounds)
{
    Wchar **suffix_probe;
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


// Sorts the suffixes of a group, bounded between left and right.
void counting_sort(Wchar **left, Wchar **right)
{
    Wchar **upper_bounds[MAX_CHARS + 1];
    current_sortbuffer = alloc_sortbuffer(left, right);

    // These suffixes belong to the same group, so the common prefix length
    // first has to be calculated.
    Uint prefixlen = grouplcp(left, right);

    // Shortest suffix is sentinel, skip
    if (*right + prefixlen == text.lst) {
        *right += prefixlen;
        right--;
    }

    // Get the count (groupsize) of the different characters
    get_count(left, right, prefixlen);

    // determine right bound for each group
    set_group_bounds(left, right, upper_bounds);

    // Create buffer
    insert_suffixes(left, right, upper_bounds);

    Wchar **suffix_probe = left;
    Wchar **buffer_probe = current_sortbuffer;

    // copy grouped suffixes back from buffer
    while (suffix_probe <= right) {

        *suffix_probe = *buffer_probe;

        suffix_probe++;
        buffer_probe++;
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
        sortbuffer.suffixhead_count[(Uint) *c]++;
    }

    for (c = text.cs; c < text.cs + text.asize; c++) {
        a                        = (Uint) *c;
        upper_bounds[a]          = nextFree + sortbuffer.suffixhead_count[a] - 1;
        nextFree                 = upper_bounds[a] + 1;
        sortbuffer.suffixhead_count[a] = 0;
    }

    // insert suffixes into array
    for (c = text.fst + text.len - 1; c >= text.fst; c--) {
        *(upper_bounds[(Uint) *c]--) = c;
    }

    // suffix \$ is the largest suffix
    text.ss[text.len] = text.lst;
}
