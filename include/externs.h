/*
 * Copyright by Stefan Kurtz (C) 1999-2003
 * =====================================
 * You may use, copy and distribute this file freely as long as you
 * - do not change the file,
 * - leave this copyright notice in the file,
 * - do not make any profit with the distribution of this file
 * - give credit where credit is due
 * You are not allowed to copy or distribute this file otherwise
 * The commercial usage and distribution of this file is prohibited
 * Please report bugs and suggestions to <kurtz@zbh.uni-hamburg.de>
 *
 * ======================================
 *
 * Modified by Ludvig Sundström 2018 with permission from Stefan Kurtz
 * For full source control tree, see https://github.com/lsund/wotd
 *
 */

#ifndef EXTERNS_H
#define EXTERNS_H

#include "types.h"
#include "basedef.h"


// This module contains global variables that span more than one source file.


///////////////////////////////////////////////////////////////////////////////
// The text

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
// Buffer for sorting

// space to be used by `sort_buffer`
extern wchar_t **sortbuffer;

// number of elements in `sbufferspace`
extern Uint sortbufferwidth;

// maximal number of elements in `sbufferspace`
extern Uint max_sortbufferwidth;

///////////////////////////////////////////////////////////////////////////////
// Suffix Tree

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
