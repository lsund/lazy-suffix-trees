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
 */

/*
 * Modified by Ludvig Sundström 2018 with permission from Stefan Kurtz
 * For full source control tree, see https://github.com/lsund/wotd
 *
 */

#include "search.h"

STree st;

static Vertex leftmost_child_textbound(VertexP vertex)
{
    VertexP child = st.vs.fst + CHILD(vertex);

    if (!IS_LEAF(child) && IS_UNEVALUATED(child)) {
        return MAKE_TEXT_LEFTBOUND(child);
    } else {
        return TEXT_LEFTBOUND(child);
    }
}


static bool no_root_edge(Pattern patt)
{
    return (st.rs[patt.head]) == UNDEF;
}


static bool is_empty(Pattern patt)
{
    return patt.cursor > patt.end;
}

static Uint text_leftbound(VertexP vertex)
{
    if(IS_UNEVALUATED(vertex)) {
        return MAKE_TEXT_LEFTBOUND(vertex);
    } else {
        return TEXT_LEFTBOUND(vertex);
    }
}

static Vertex edge_length(VertexP vertex)
{
    return leftmost_child_textbound(vertex) - TEXT_LEFTBOUND(vertex);
}


static void eval_if_uneval(VertexP *vertex, void (*eval_fun)(VertexP))
{
    if(IS_UNEVALUATED(*vertex)) {
        Uint index = INDEX(*vertex);
        *vertex = st.vs.fst + index;
        eval_fun(*vertex);
    }
}


static Match try_match_leaf(Pattern patt, Uint *vertex)
{
    Wchar *text_cursor = text.content + TEXT_LEFTBOUND(vertex);
    if (*text_cursor == patt.head) {
        return match_leaf(text_cursor, patt);
    } else if (text_cursor == text.sentinel || IS_LASTCHILD(vertex)) {
        return exhausted_match();
    } else {
        return failed_match();
    }
}

static Match match_rootedge(Pattern *patt, VertexP *cursor)
{
    Match match;
    match.done     = false;
    match.success  = false;

    if(is_empty(*patt)) {
        match.done = true;
        match.success = true;
        return match;
    }

    eval_root();

    if (no_root_edge(*patt)) {
        return exhausted_match();
    }

    Vertex rootchild  = st.rs[patt->head];
    Wchar *text_start = text.content + WITHOUT_LEAFBIT(rootchild);
    if (IS_LEAF(&rootchild)) {
        return match_leaf(text_start, *patt);
    }

    *cursor  = st.vs.fst + rootchild;

    eval_if_uneval(cursor, eval_branch);
    Uint edgelen = edge_length(*cursor);
    Uint plen    = inner_lcp(*cursor, *patt, edgelen);

    if(plen != edgelen - 1) {
        match.done = true;
        match.success = pattern_has_length(*patt, plen);
        return match;
    } else {
        patt->cursor += edgelen;
    }
    return match;
}


static void find_next_child()
{
}


///////////////////////////////////////////////////////////////////////////////
// Public Interface

bool search(Pattern patt)
{

    VertexP current_vertex;
    Match rootmatch = match_rootedge(&patt, &current_vertex);

    if (rootmatch.done) {
        return rootmatch.success;
    }

    while(!is_empty(patt)) {

        patt.head = *patt.cursor;
        current_vertex    = st.vs.fst + CHILD(current_vertex);

        while(true) {

            if (IS_LEAF(current_vertex)) {

                Match match = try_match_leaf(patt, current_vertex);
                if (match.done) {
                    return match.success;
                } else {
                    current_vertex += LEAF_VERTEXSIZE;
                }

            } else {
                Wchar firstchar = *(text.content + text_leftbound(current_vertex));
                if(firstchar == patt.head) {
                    break;
                }
                if(IS_LASTCHILD(current_vertex)) {
                    return false;
                } else {
                    current_vertex += INNER_VERTEXSIZE;
                }
            }
        }

        eval_if_uneval(&current_vertex, eval_branch);
        Uint edgelen = edge_length(current_vertex);
        Uint plen    = inner_lcp(current_vertex, patt, edgelen);

        if(plen !=  edgelen - 1) {
            return pattern_has_length(patt, plen);
        }

        patt.cursor += edgelen;
    }
    return true;
}
