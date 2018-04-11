
#ifndef GLOBALS_H
#define GLOBALS_H

#include "types.h"

///////////////////////////////////////////////////////////////////////////////
// Global Strings

// points to input string `t` of length `n`
extern Uchar *text;

// points to `t[n] = undefined`
extern Uchar *sentinel;

// characters in `t` in alphabetical order
extern Uchar characters[UCHAR_MAX + 1];

// array of pointers to suffixes of `t`
extern Uchar **suffixes;

// pointers into suffixes depend on this variable
extern Uchar **suffixbase;

// buffer for sorting suffixes in `src/sort.c`
extern Uchar **sort_buffer;

// space to be used by `sort_buffer`
extern Uchar **sbufferspace;

///////////////////////////////////////////////////////////////////////////////
// Global Integers

// length of `t`
extern Uint  textlen;

// size of the alphabet `A`
extern Uint alphasize;

// index of characters in the alphabet `A`
extern Uint alphaindex[UCHAR_MAX + 1];

// number of occurrences of all characters
extern Uint occurrence[UCHAR_MAX + 1];

// table to hold suffix tree representation
extern Uint *stree;

// number of integers in `stree` allocated
extern Uint streesize;

// pointer to next unused element in `stree`
extern Uint *next_free;

// number of elements in `sbufferspace`
extern Uint sbufferwidth;

// maximal number of elements in `sbufferspace`
extern Uint maxsbufferwidth;

// number of unprocessed suffixes (for eager evaluation)
extern Uint suffixessize;

// when reached, then move and halve space for suffixes
extern Uint maxunusedsuffixes;

// constant time access to successors of `root`
extern Uint rootchildtab[UCHAR_MAX + 1];

// flag indicating that the root has been evaluated
extern Bool  rootevaluated;

#endif
