
#ifndef GLOBALS_H
#define GLOBALS_H

#include "types.h"
#include "basedef.h"

///////////////////////////////////////////////////////////////////////////////
// The text

// points to input string `t` of length `n`
extern Uchar *text;

// length of `t`
extern Uint  textlen;

// array of pointers to suffixes of `t`
extern Uchar **suffixes;

// size of the alphabet `A`
extern Uint alphasize;

// points to `t[n] = undefined`
extern Uchar *sentinel;

// characters in `t` in alphabetical order
extern Uchar characters[MAX_CHARS + 1];

extern Uint indices[MAX_CHARS + 1];

// number of occurrences for the first character of the current suffixes
extern Uint suffixhead_count[MAX_CHARS + 1];

///////////////////////////////////////////////////////////////////////////////
// Sorting Buffer

// buffer for sorting suffixes in `src/sort.c`
extern Uchar **current_sortbuffer;

// space to be used by `sort_buffer`
extern Uchar **sortbuffer;

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
