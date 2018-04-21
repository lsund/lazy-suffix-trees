
#include "sort.h"


///////////////////////////////////////////////////////////////////////////////
// Globals


wchar_t     *wtext,
            *sentinel,
            **suffixes,
            characters[MAX_CHARS + 1],
            **current_sortbuffer;

Uint        textlen,
            alphasize,
            occurrence[MAX_CHARS + 1],
            suffixhead_count[MAX_CHARS + 1];

///////////////////////////////////////////////////////////////////////////////
// Private functions


// Increases the count for this particular character
static void increase_count(Uint codepoint)
{
    /* int i = get_index(codepoint); */
    Uint i = codepoint;
    suffixhead_count[i]++;
}


// Determine size for each group
static void get_count(wchar_t **left, wchar_t **right, Uint prefixlen)
{
    wchar_t **suffix_probe;
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
static void set_group_bounds(wchar_t **left, wchar_t **right, wchar_t ***upper_bounds)
{
    // `current_sortbuffer` is already allocated, a sufficiently large memory
    // block for all suffix pointers.
    wchar_t **lower_bound = current_sortbuffer;
    wchar_t **suffix_probe;

    for (suffix_probe = left; suffix_probe <= right; suffix_probe++) {

        Uint head = **suffix_probe;
        if (suffixhead_count[head] > 0) {

            // 'allocate' the upper bound for the current character.
            // upper_bounds[head] now points to a allocated memory address,
            // enough space in distance from the last group
            upper_bounds[head] = lower_bound + suffixhead_count[head] - 1;
            lower_bound        = upper_bounds[head] + 1;
            suffixhead_count[head]   = 0;
        }
    }
}


// Insert the suffixes into the correct positions, depending on `upper_bonuds`
static void insert_suffixes(wchar_t **left, wchar_t **right, wchar_t ***upper_bounds)
{
    wchar_t **suffix_probe;
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
void counting_sort(wchar_t **left, wchar_t **right)
{
    wchar_t **upper_bounds[MAX_CHARS + 1];
    current_sortbuffer = alloc_sortbuffer(left, right);

    // These suffixes belong to the same group, so the common prefix length
    // first has to be calculated.
    Uint prefixlen = grouplcp(left, right);

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

    wchar_t **suffix_probe = left;
    wchar_t **buffer_probe = current_sortbuffer;

    // copy grouped suffixes back from buffer
    while (suffix_probe <= right) {

        *suffix_probe = *buffer_probe;

        suffix_probe++;
        buffer_probe++;
    }
}


// Determines the groups for all suffixes of the input string
//
// Scan all suffixes, determining the size of each group and then sorts the all
// suffixes into the array suffixes.
void create_suffix_groups(void)
{
    wchar_t *c, **nextFree = suffixes;
    Uint a;

    wchar_t **upper_bounds[MAX_CHARS + 1];

    // determine size for each group
    for (c = wtext; c < wtext + textlen; c++) {
        suffixhead_count[(Uint) *c]++;
    }

    for (c = characters; c < characters + alphasize; c++) {
        a                        = (Uint) *c;
        upper_bounds[a]          = nextFree + suffixhead_count[a] - 1;
        nextFree                 = upper_bounds[a] + 1;
        suffixhead_count[a] = 0;
    }

    // insert suffixes into array
    for (c = wtext + textlen - 1; c >= wtext; c--) {
        *(upper_bounds[(Uint) *c]--) = c;
    }

    // suffix \$ is the largest suffix
    suffixes[textlen] = sentinel;
}
