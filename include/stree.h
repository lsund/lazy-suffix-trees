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
#define INNER_VERTEXSIZE    2

#define ROOT                vertices.first

///////////////////////////////////////////////////////////////////////////////
// Vertices
// Offset in the table
#define INDEX(P)                        ((Uint) ((P) - ROOT))

#define LEAFBIT                         MSB

#define WITH_LASTCHILDBIT(V)            ((V) | SECOND_MSB)
#define WITH_LEAFBIT(V)                 ((V) | LEAFBIT)
#define WITH_LEAF_AND_LASTCHILDBIT(V)   ((V) | LEAFBIT | SECOND_MSB)
#define WITH_UNEVALBIT(V)               ((V) | MSB)

#define WITHOUT_LEAFBIT(V)              ((V) & ~LEAFBIT)


// Evaluated vertices
#define TEXT_OFFSET(P)           ((*(P)) & ~(LEAFBIT | SECOND_MSB)) // lp
#define CHILD(P)                 (*((P) + 1))

#define SET_OFFSET(V, O)         *(V) = (*(V) & SECOND_MSB) | (O)

// Unevaluated vertices
#define SUFFIX_OFFSET(P)            (suffixes + TEXT_OFFSET(P))
#define SUFFIX_BOUND(P)           (suffixes + (CHILD(P) & ~MSB))
// startposition of suffix
#define SUFFIX_INDEX(P)          ((Uint) (*(P) - wtext))
#define MAKE_TEXT_OFFSET(P)      SUFFIX_INDEX(SUFFIX_OFFSET(P))

///////////////////////////////////////////////////////////////////////////////
// Queries

#define IS_LEAF(V)              ((*(V)) & LEAFBIT)
#define IS_LASTCHILD(V)         ((*(V)) & SECOND_MSB)
#define IS_UNEVALUATED(V)       (CHILD(V) & MSB)

///////////////////////////////////////////////////////////////////////////////
// Getters

// This is the left pointer, defined as the minimum leaf under P, plus the
// length of the path to its parent.  To retrieve the edge labels in constant
// time, it suffices to store the left pointer for all nodes.


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
