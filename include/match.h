#ifndef MATCH_H
#define MATCH_H

#include "types.h"
#include "pattern.h"
#include "lcp.h"
#include "stree.h"

// This structure represents the result of matching a string with an edge in
// the suffix tree.
typedef struct match {
    // `done` is set to true to indicate that there is nothing more to be done
    // while matching. If it's not set, the algorithm might try to match the
    // next edge and so on.
    bool done;
    // `success` is set to true if the characters in the string exactly matched
    // the characters on the edge label.
    bool success;
} Match;

// Returns an exhausted match, a failed match such that there is nothing more
// to do.
Match exhausted_match();

// Returns a done match, such that there is nothing more to be done after
// matching. The success of the match is determined by the longest common
// prefix of the label and the pattern.
Match make_done_match(Wchar *label, Pattern patt);

// Matches the edge under v with the pattern p, if this edge is a leaf edge.
// Returns a match with fields set to represent the result.
Match match_leafedge(VertexP v, Pattern patt);

// Matches the edge under v with the pattern p, if this edge is not a leaf
// edge. Returns a match with fields set to represent the result.
Match match_edge(VertexP v, Pattern patt);

#endif
