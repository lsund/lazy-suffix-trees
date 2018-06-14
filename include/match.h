#ifndef MATCH_H
#define MATCH_H

#include "types.h"
#include "pattern.h"
#include "lcp.h"
#include "stree.h"

typedef enum { SUCCESS, FAIL, EXHAUSTED } Matchtype;

typedef struct match {
    bool done;
    bool success;
} Match;

Match failed_match();

Match successful_match();

Match exhausted_match();

Match match_leaf(Wchar *text_cursor, Pattern patt);

Uint inner_lcp(Uint *vertex, Pattern patt, Uint edgelen);

#endif
