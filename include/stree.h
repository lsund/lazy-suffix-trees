#ifndef STREE_H
#define STREE_H

#include "types.h"
#include "config.h"
#include "text.h"
#include "streedef.h"

// Constant representing an undefined value
#define UNDEF                   UINT_MAX

// The number of integers required to represent a leaf.
#define LEAF_VERTEXSIZE                 1
// The number of integers required to represent an inner vertex.
#define INNER_VERTEXSIZE                2

// The value represented by the most significant bit of an unsigned integer.
#define MSB                             (UINT(1) << (INT_SIZE - 1))
// The value represented by the second most significant bit of an unsigned
// integer.
#define SECOND_MSB (MSB >> 1)

///////////////////////////////////////////////////////////////////////////////
// Query and modify vertices

// Mark a vertex to be the last child
#define MAKE_LASTCHILD(V)               ((V) | SECOND_MSB)
// Mark a vertex to be a leaf
#define MAKE_LEAF(V)                    ((V) | MSB)
// Mark a vertex to be unevaluated
#define MAKE_UNEVAL_VERTEX(V)           ((V) | MSB)
// Mark a vertex to be an inner vertex
#define MAKE_INNER(V)                   ((V) & ~MSB)

// Is this vertex a leaf?
#define IS_LEAF(R)                      ((*(R)) & MSB)
// Is this vertex the last child?
#define IS_LASTCHILD(R)                 ((*(R)) & SECOND_MSB)
// Is this vertex unevaluated?
#define IS_UNEVALUATED(R)               (FIRSTCHILD(R) & MSB)

// Get the pointer to the leftbound.
// If the label of (VP -> FIRSTCHILD(VP)) is x, then this is the smallest index
// i such that i is an occurence of x in the text.
#define LEFTBOUND(VP)                    ((*(VP)) & ~(MSB | SECOND_MSB))
// Get the ponter to the rightbound, defined as LEFTBOUND(VP) + |x| where x is
// as above.
#define RIGHTBOUND(VP)                   (*((VP) + 1))
// The first child of a vertex is equal to its rightbound
#define FIRSTCHILD(VP)                   RIGHTBOUND(VP)
// Set the leftbound of a vertex
#define SET_LEFTBOUND(R, O)             *(R) = IS_LASTCHILD(R) | (O)

///////////////////////////////////////////////////////////////////////////////
// Utilities

// Does the suffix tree have a a root edge starting with c?
bool has_root_edge(Wchar c);

// The leftbound of a vertex.
Uint leftbound(VertexP vertex);

// The length of the edge leading to its first child.
Vertex edge_length(VertexP vertex);

// Does this suffix point to the sentinel, i.e. the last suffix?
bool is_sentinel(Suffix **s);

#endif
