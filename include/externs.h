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

typedef struct text {
    Wchar *content;
    Wchar **suffixes;
    Wchar *sentinel;    // Last character, points to UNDEF
    // number of occurences for the first character of the current suffixes
    Uint len;
    Uint alphasize;
    Wchar characters[MAX_CHARS + 1]; // characters in alphabetical order
    Uint max_charval;
} Text;

// points to input string `t` of length `n`
extern Text text;

///////////////////////////////////////////////////////////////////////////////
// Buffer for sorting


typedef struct sortbuffer {
    // space to be used by `sort_buffer`
    Wchar **content;
    Uint suffixhead_count[MAX_CHARS + 1];
    // number of elements
    Uint size;
    Uint maxsize;
} Sortbuffer;

extern Sortbuffer sortbuffer;

// maximal number of elements in `sbufferspace`
extern Uint max_sortbufferwidth;

///////////////////////////////////////////////////////////////////////////////
// Suffix Tree

typedef struct table {
    Uint *first;
    Uint *next;
    Uint size;
} Table;


//  Tree representation
extern Table vertices;

// constant time access to successors of `root`
extern Uint root_children[MAX_CHARS + 1];

// flag indicating that the root has been evaluated
extern bool root_evaluated;

#endif
