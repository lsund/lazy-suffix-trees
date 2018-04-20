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

#ifndef STREE_H
#define STREE_H

#include "types.h"

#define INDEX(N)            ((Uint) ((N) - stree))

///////////////////////////////////////////////////////////////////////////////
// Info About vertices

// Instead of storing seperately if a vertex is a leaf, a rightmostchild or
// unevaluated

// Bit determining if the vertex is a leaf
#define LEAFBIT                 FIRSTBIT

// Bit determining if the inner node is unevaluated
#define UNEVALUATEDBIT          FIRSTBIT

// Bit determining if the node is the rightmost child of its parent
#define RIGHTMOSTCHILDBIT       SECONDBIT

///////////////////////////////////////////////////////////////////////////////
// Getters

// LP is the leaf set of on edge plus the length of the string leading up to it
#define GET_LP(P)                ((*(P)) & ~(LEAFBIT | RIGHTMOSTCHILDBIT))

// The number for the first child of the vertex pointer
#define GET_FIRSTCHILD(P)        (*((P) + 1))

// The left boundry of the remaining suffixes
#define GET_LEFTBOUNDARY(P)      (suffixes + *(P))

// The right boundry of the remaining suffixes
#define GET_RIGHTBOUNDARY(P)     (suffixes + ((*((P) + 1)) & ~UNEVALUATEDBIT))

// The lp number of an unevaluated vertex
#define GET_LP_UNEVAL(N)          SUFFIX_STARTINDEX(GET_LEFTBOUNDARY(N))

// startposition of suffix
#define SUFFIX_STARTINDEX(L)         ((Uint) (*(L) - wtext))

///////////////////////////////////////////////////////////////////////////////
// Setters

#define SET_LP(P, LP)             *(P) = (*(P) & RIGHTMOSTCHILDBIT) | (LP)

#define SET_FIRSTCHILD(P,C)      *((P) + 1) = C

#define SET_LEAF(P,L)            *(P) = (L) | LEAFBIT

// Create a new vertex
//
// Each vertex needs two integers allocated for it, one for its left suffix
// boundary and one for its left suffix boundary. This macro sets the left and
// the right boundary for the two following positions in P.
#define STORE_SUFFIX_BOUNDARIES(P, L, R)  *(P) = (Uint) ((L) - suffixes);\
                                 *((P) + 1) = ((R) - suffixes) | UNEVALUATEDBIT

///////////////////////////////////////////////////////////////////////////////
// Queries

#define IS_LEAF(P)              ((*(P)) & LEAFBIT)

#define IS_RIGHTMOST(P)         ((*(P)) & RIGHTMOSTCHILDBIT)

#define IS_UNEVALUATED(P)       ((*((P)+1)) & UNEVALUATEDBIT)

#endif
