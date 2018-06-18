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
#include "text.h"
#include "streedef.h"

#define LEAF_VERTEXSIZE     1
#define INNER_VERTEXSIZE    2

#define ROOT                st.vs.fst

///////////////////////////////////////////////////////////////////////////////
// Vertices
// Offset in the table
#define INDEX(P)                        ((Uint) ((P) - ROOT))

#define MAKE_LASTCHILD(V)               ((V) | SECOND_MSB)
#define MAKE_LEAF(V)                    ((V) | MSB)
#define MAKE_LEAF_LASTCHILD(V)          ((V) | MSB | SECOND_MSB)
#define MAKE_UNEVAL_VERTEX(V)           ((V) | MSB)

#define WITHOUT_LEAFBIT(V)              ((V) & ~MSB)


// Evaluated vertices
#define LEFTBOUND(V)               ((*(V)) & ~(MSB | SECOND_MSB))
#define RIGHTBOUND(V)              (*((V) + 1))
#define CHILD(V)                        RIGHTBOUND(V)

// Unevaluated vertices
#define SUFFIX_LEFTBOUND(V)             (text.suffixes + LEFTBOUND(V))
#define SUFFIX_RIGHTBOUND(V)            (text.suffixes + (CHILD(V) & ~MSB))
// startposition of suffix
#define SUFFIX_INDEX(V)                 ((Uint) (*(V) - text.content))
#define MAKE_LEFTBOUND(V)          SUFFIX_INDEX(SUFFIX_LEFTBOUND(V))

#define SET_LEFTBOUND(V, O)        *(V) = (*(V) & SECOND_MSB) | (O)

///////////////////////////////////////////////////////////////////////////////
// Queries

#define IS_LEAF(V)                      ((*(V)) & MSB)
#define IS_LASTCHILD(V)                 ((*(V)) & SECOND_MSB)
#define IS_UNEVALUATED(V)               (CHILD(V) & MSB)

///////////////////////////////////////////////////////////////////////////////
// Getters

// This is the left pointer, defined as the minimum leaf under P, plus the
// length of the path to its parent.  To retrieve the edge labels in constant
// time, it suffices to store the left pointer for all nodes.

#endif
