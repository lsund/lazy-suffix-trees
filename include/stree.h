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
#include "bitvector.h"

///////////////////////////////////////////////////////////////////////////////
// Getters

#define INDEX(N)            ((Uint) ((N) - stree))

// Bit determining if leaf
#define LEAFBIT                 FIRSTBIT

// right most child of succ list
#define RIGHTMOSTCHILDBIT       SECONDBIT

// unevaluated branching node
#define UNEVALUATEDBIT          FIRSTBIT

// LP is the leaf set of on edge plus the length of the string leading up to it
#define GET_LP(P)                ((*(P)) & ~(LEAFBIT | RIGHTMOSTCHILDBIT))

// The number for the first child of the vertex pointer
#define FIRST_CHILD(P)        (*((P) + 1))

// startposition of suffix
#define SUFFIXNUMBER(L)         ((Uint) (*(L) - wtext))

// The left boundry of the remaining suffixes
#define GETLEFTBOUNDARY(P)      (suffixes + *(P))

// The right boundry of the remaining suffixes
#define GETRIGHTBOUNDARY(P)     (suffixes + ((*((P) + 1)) & ~UNEVALUATEDBIT))

// The lp number of an unevaluated vertex
#define GET_LP_UNEVAL(N)          SUFFIXNUMBER(GETLEFTBOUNDARY(N))

///////////////////////////////////////////////////////////////////////////////
// Setters

#define SETLP(P, LP)             *(P) = (*(P) & RIGHTMOSTCHILDBIT) | (LP)

#define SETFIRSTCHILD(P,C)      *((P)+1) = C

#define SETLEAF(P,L)            *(P) = (L) | LEAFBIT

// Store the left and right boundaries for the remaining suffixes
#define STOREBOUNDARIES(P,L,R)  *(P) = (Uint) ((L) - suffixes);\
                                *((P)+1) = ((R) - suffixes) | UNEVALUATEDBIT


///////////////////////////////////////////////////////////////////////////////
// Queries

#define IS_LEAF(P)              ((*(P)) & LEAFBIT)

#define IS_RIGHTMOST(P)         ((*(P)) & RIGHTMOSTCHILDBIT)

#define IS_UNEVALUATED(P)       ((*((P)+1)) & UNEVALUATEDBIT)

#endif
