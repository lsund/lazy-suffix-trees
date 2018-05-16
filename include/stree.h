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

#define LEAF_VERTEXSIZE  1
#define INNER_VERTEXSIZE 2

#define ROOT vertices

#define WITHOUT_LEAFBIT(V)        ((V) & ~LEAFBIT)

///////////////////////////////////////////////////////////////////////////////
// Vertices
#define REF_TO_INDEX(P)            ((Uint) ((P) - ROOT))

#define LEAFBIT                 FIRSTBIT

// Bit determining if the node is the rightmost child of its parent. This is
// stored in the first integer, ie its left pointer.
#define LASTCHILDBIT       SECONDBIT

// Bit determining if the inner node is unevaluated. This is stored in the
// second integer of the vertex, ie. its first child.
#define UNEVALUATEDBIT          FIRSTBIT

#define WITH_LASTCHILDBIT(V)    ((V) | LASTCHILDBIT)

///////////////////////////////////////////////////////////////////////////////
// Getters

// This is the left pointer, defined as the minimum leaf under P, plus the
// length of the path to its parent.  To retrieve the edge labels in constant
// time, it suffices to store the left pointer for all nodes.
//
// A node is referenced by the index(lp(u)), disregarding the first and second
// bit.
#define GET_LP(V)               ((*(V)) & ~(LEAFBIT | LASTCHILDBIT))

// For each branching vertex, we additionally need constant time access to the
// child of a vertex with the smallest left pointer. This accesss is provided
// by the following reference.
//
// The algorithm only needs access to this child in order to evaluate all
// children of V. The children are sorted according to first character, and
// laid out in `vertices` sequentially, so the algorithm can iterate over these,
// setting RIGHTMOSTBIT to the last vertex.
#define CHILD(V)       (*((V) + 1))

// The left boundry of the remaining suffixes
#define GET_LEFTB(V)            (suffixes + *(V))

// The right boundry of the remaining suffixes, stripping of the first bit.
//
// Note that the right boundry is defined as the adrees starting at suffixes,
// with the offset of the first child of V.
#define GET_RIGHTB(V)           (suffixes + (CHILD(V) & ~UNEVALUATEDBIT))

// startposition of suffix
#define SUFFIX_INDEX(L)         ((Uint) (*(L) - wtext))

// The lp number of an unevaluated vertex V. This is just the index for the
// left-bound of V.
#define GET_LP_UNEVAL(V)        SUFFIX_INDEX(GET_LEFTB(V))

// Given a LP value and a vertex V, set the lp-value for V, by ORing the two
// integers. V is first ANDed with its second bit, to reset all bits except
// the rightmost one.
#define SET_LP(V, LP)           *(V) = (*(V) & LASTCHILDBIT) | (LP)

// Set the leaf number of V to N. The first bit is also set to 1, since this is
// a leaf.
#define SET_LEAF(V, N)          *(V) = (N) | LEAFBIT

// Each vertex needs two integers allocated for it, one for its left suffix
// boundary and one for its left suffix boundary. This macro sets the left and
// the right boundary for the two following positions in P.
#define SET_BOUNDARIES(P, L, R)\
    *(P) = (Uint) ((L) - suffixes);\
    *((P) + 1) = ((R) - suffixes) | UNEVALUATEDBIT


// Retrieve the MSB of LP(V)
#define IS_LEAF(V)              ((*(V)) & LEAFBIT)

// Retrieve the second MSB of LP(V)
#define IS_RIGHTMOST(V)         ((*(V)) & LASTCHILDBIT)

// Retrieve the MSB of FIRSTCHILD(V)
#define IS_UNEVALUATED(V)       ((*((V) + 1)) & UNEVALUATEDBIT)

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
