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
#define LEFTBOUND(R)               ((*(R)) & ~(MSB | SECOND_MSB))
#define RIGHTBOUND(R)              (*((R) + 1))
#define CHILD(R)                        RIGHTBOUND(R)

#define SUFFIX_LEFTBOUND(R)         (text.ss + LEFTBOUND(R))
#define SUFFIX_RIGHTBOUND(R)        (text.ss + (CHILD(R) & ~MSB))

#define SUFFIX_INDEX(R)             ((Uint) (*(R) - text.fst))
#define MAKE_LEFTBOUND(R)           SUFFIX_INDEX(SUFFIX_LEFTBOUND(R))

#define SET_LEFTBOUND(R, O)        *(R) = (*(R) & SECOND_MSB) | (O)

///////////////////////////////////////////////////////////////////////////////
// Queries

#define IS_LEAF(R)                      ((*(R)) & MSB)
#define IS_LASTCHILD(R)                 ((*(R)) & SECOND_MSB)
#define IS_UNEVALUATED(R)               (CHILD(R) & MSB)

///////////////////////////////////////////////////////////////////////////////
// Getters

// This is the left pointer, defined as the minimum leaf under P, plus the
// length of the path to its parent.  To retrieve the edge labels in constant
// time, it suffices to store the left pointer for all nodes.

#endif
