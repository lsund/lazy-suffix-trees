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
#include "basedef.h"
#include "externs.h"


// Determines the index of a vertex. V is the adress pointing to the vertex's
// position in the text. By subtracting the adress of the tree, the index is
// obtained.
#define INDEX(V)            ((Uint) ((V) - stree))

// Instead of storing seperately if a vertex is a leaf, a rightmostchild or
// unevaluated, this information is encoded in the MSB and the second MSB of
// the number.

// Bit determining if the vertex is a leaf. This is storde in the first
// integer.
#define LEAFBIT                 FIRSTBIT

// Bit determining if the node is the rightmost child of its parent. This is
// stored in the first integer.
#define RIGHTMOSTCHILDBIT       SECONDBIT

// Bit determining if the inner node is unevaluated. This is stored in the
// second integer.
#define UNEVALUATEDBIT          FIRSTBIT

///////////////////////////////////////////////////////////////////////////////
// Getters

// This is the left pointer, defined as the minimum leaf under P, plus the
// length of the path to its parent.  To retrieve the edge labels in constant
// time, it suffices to store the left pointer for all nodes.
//
// A node is referenced by the index(lp(u))
#define GET_LP(P)               ((*(P)) & ~(LEAFBIT | RIGHTMOSTCHILDBIT))

// For each branching vertex, we additionally need constant time access to the
// child of a vertex with the smallest left pointer. This accesss is provided
// by the following reference.
#define GET_FIRSTCHILD(P)       (*((P) + 1))

// The left boundry of the remaining suffixes
#define GET_LEFTB(P)            (suffixes + *(P))

// The right boundry of the remaining suffixes.
#define GET_RIGHTB(P)           (suffixes + ((*((P) + 1)) & ~UNEVALUATEDBIT))

// startposition of suffix
#define SUFFIX_INDEX(L)         ((Uint) (*(L) - wtext))

// The lp number of an unevaluated vertex
#define GET_LP_UNEVAL(N)        SUFFIX_INDEX(GET_LEFTB(N))

#define SET_LP(P, LP)           *(P) = (*(P) & RIGHTMOSTCHILDBIT) | (LP)

#define SET_FIRSTCHILD(P, C)    *((P) + 1) = C

#define SET_LEAF(P, L)          *(P) = (L) | LEAFBIT

// Each branching vertex requires two integers, one for the left boundary and
// one for the right boundary
#define BRANCHWIDTH             UintConst(2)

// Each vertex needs two integers allocated for it, one for its left suffix
// boundary and one for its left suffix boundary. This macro sets the left and
// the right boundary for the two following positions in P.
#define SET_BOUNDARIES(P, L, R)\
    *(P) = (Uint) ((L) - suffixes);\
    *((P) + 1) = ((R) - suffixes) | UNEVALUATEDBIT

#define IS_LEAF(P)              ((*(P)) & LEAFBIT)

#define IS_RIGHTMOST(P)         ((*(P)) & RIGHTMOSTCHILDBIT)

#define IS_UNEVALUATED(P)       ((*((P)+1)) & UNEVALUATEDBIT)

void create_root_leaf(Wchar firstchar, Wchar **left);

void create_inner_vertex(
        Wchar firstchar,
        Wchar **left,
        Wchar **right,
        bool root
    );

Uint create_leaf_vertex(Wchar firstchar, Wchar **left, bool root);

Uint create_sentinel_vertex(Wchar **right, Uint **previousnode);

void init_root_children();

#endif
