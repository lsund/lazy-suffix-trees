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

#define LEAF_VERTEXSIZE     1
#define INNER_VERTEXSIZE    3

#define ROOT                vertices

///////////////////////////////////////////////////////////////////////////////
// Vertices
// Offset in the table
#define INDEX(P)                        ((Uint) ((P) - ROOT))

#define LEAFBIT                         MSB

// Bit determining if the node is the rightmost child of its parent. This is
// stored in the first integer, ie its left pointer.
#define LASTCHILDBIT                    SECOND_MSB

// Bit determining if the inner node is unevaluated. This is stored in the
// second integer of the vertex, ie. its first child.
#define UNEVALUATEDBIT                  MSB

#define WITH_LASTCHILDBIT(V)            ((V) | LASTCHILDBIT)
#define WITH_LEAFBIT(V)                 ((V) | LEAFBIT)
#define WITH_LEAF_AND_LASTCHILDBIT(V)   ((V) | LEAFBIT | LASTCHILDBIT)
#define WITH_UNEVALBIT(V)               ((V) | UNEVALUATEDBIT)

#define WITHOUT_LEAFBIT(V)              ((V) & ~LEAFBIT)


// Evaluated vertices
#define OFFSET(P)                       ((*(P)) & ~(LEAFBIT | LASTCHILDBIT))
#define CHILD(P)                        (*((P) + 1))
#define LEAF_INDEX(P)                   (*((P) + 2))
#define OFFSET_UNEVAL(V)                SUFFIX_INDEX(LEFT_BOUND(V))

#define SET_OFFSET(V, O)                *(V) = (*(V) & LASTCHILDBIT) | (O)

// Unevaluated vertices
#define LEFT_BOUND(P)            (suffixes + OFFSET(P))
#define RIGHT_BOUND(P)           (suffixes + (CHILD(P) & ~UNEVALUATEDBIT))

///////////////////////////////////////////////////////////////////////////////
// Queries

#define IS_LEAF(V)              ((*(V)) & LEAFBIT)
#define IS_LASTCHILD(V)         ((*(V)) & LASTCHILDBIT)
#define IS_UNEVALUATED(V)       (CHILD(V) & UNEVALUATEDBIT)

///////////////////////////////////////////////////////////////////////////////
// Getters

// This is the left pointer, defined as the minimum leaf under P, plus the
// length of the path to its parent.  To retrieve the edge labels in constant
// time, it suffices to store the left pointer for all nodes.

// startposition of suffix
#define SUFFIX_INDEX(P)                 ((Uint) (*(P) - wtext))

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

void stree_destroy();

#endif
