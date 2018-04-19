
#ifndef EXTERNS_H
#define EXTERNS_H

#include "types.h"
#include "basedef.h"

///////////////////////////////////////////////////////////////////////////////
// The text

// points to input string `t` of length `n`
/* extern Uchar *text; */

// points to input string `t` of length `n`
extern wchar_t *wtext;

// Maximal codepoint value of `t`
extern Uint max_codepoint;

// length of `t`
extern Uint  textlen;

// array of pointers to suffixes of `t`
extern wchar_t **suffixes;

// size of the alphabet `A`
extern Uint alphasize;

// points to `t[n] = undefined`
extern wchar_t *sentinel;

// characters in `t` in alphabetical order
extern wchar_t characters[MAX_CHARS + 1];

extern Uint indices[MAX_CHARS + 1];

// number of occurrences for the first character of the current suffixes
extern Uint suffixhead_count[MAX_CHARS + 1];

///////////////////////////////////////////////////////////////////////////////
// Sorting Buffer

// buffer for sorting suffixes in `src/sort.c`
extern wchar_t **current_sortbuffer;

// space to be used by `sort_buffer`
extern wchar_t **sortbuffer;

// number of elements in `sbufferspace`
extern Uint sortbufferwidth;

// maximal number of elements in `sbufferspace`
extern Uint max_sortbufferwidth;

///////////////////////////////////////////////////////////////////////////////
// Suffix tree representation

//  Tree representation
extern Uint *stree;

// number of integers in `stree` allocated
extern Uint streesize;

// pointer to next unused element in `stree`
extern Uint *next_free;

// constant time access to successors of `root`
extern Uint root_children[MAX_CHARS + 1];

// flag indicating that the root has been evaluated
extern bool root_evaluated;

#endif
