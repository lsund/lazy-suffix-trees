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

Wchar *wtext,
        *sentinel,
        **suffixes;

Uint *vertices, root_children[MAX_CHARS + 1];

bool root_evaluated;

static Pattern init_pattern(Wchar *patt_start, Wchar *patt_end)
{
    Pattern patt;
    patt.head  = *patt_start;
    patt.cursor = patt_start;
    patt.end   = patt_end;
    return patt;
}


static Vertex first_child_lp(VertexP vertex)
{
    VertexP child = vertices + CHILD(vertex);

    if (!IS_LEAF(child) && IS_UNEVALUATED(child)) {
        return OFFSET_UNEVAL(child);
    } else {
        return OFFSET(child);
    }
}


static bool no_root_edge(Pattern patt)
{
    return (root_children[patt.head]) == UNDEFINEDSUCC;
}


static bool is_empty(Pattern patt)
{
    return patt.cursor > patt.end;
}

static bool is_prefix(Uint prefixlen, Uint edgelen)
{
    return prefixlen != edgelen - 1;
}


static bool is_matched(Pattern patt, Uint len)
{
    return (Uint) (patt.end - patt.cursor) == len;
}


static bool match(Wchar *text_cursor, Pattern patt)
{
    // Changed below to plus 1
    Uint len = lcp(patt.cursor + 1, patt.end, text_cursor + 1, sentinel - 1);
    return is_matched(patt, len);
}


static Vertex edge_length(VertexP vertex)
{
    Uint lp = OFFSET(vertex);
    return first_child_lp(vertex) - lp;
}


static Uint offset(VertexP vertex)
{
    if(IS_UNEVALUATED(vertex)) {
        return OFFSET_UNEVAL(vertex);
    } else {
        return OFFSET(vertex);
    }
}


static void eval_vertexp(VertexP *vertex)
{
    if(IS_UNEVALUATED(*vertex)) {
        Uint vertex_num = INDEX(*vertex);
        eval_vertex(vertex_num);
        *vertex = vertices + vertex_num;
    }

}


static Uint prefixlen(Uint *vertex, Pattern patt, Uint edgelen)
{
    Wchar *text_cursor = wtext + OFFSET(vertex);
    return lcp(patt.cursor + 1, patt.end, text_cursor + 1, text_cursor + edgelen - 1);
}


static Match try_match_leaf(Pattern patt, Uint *vertex)
{
    Wchar *text_cursor = wtext + OFFSET(vertex);
    if(*text_cursor == patt.head) {
        return make_match(match(text_cursor, patt));
    }
    if(text_cursor == sentinel || IS_LASTCHILD(vertex)) {
        return unsuccessful_match();
    }
    return incomplete_match();
}

static Match match_rootedge(Pattern *patt, VertexP *cursor)
{
    Match res;
    res.done     = false;
    res.success  = false;

    if(is_empty(*patt)) {
        return successful_match();
    }

    eval_root();

    if (no_root_edge(*patt)) {
        return unsuccessful_match();
    }

    Vertex rootchild  = root_children[patt->head];
    Wchar *text_start = wtext + WITHOUT_LEAFBIT(rootchild);
    if (IS_LEAF(&rootchild)) {
        return make_match(match(text_start, *patt));
    }

    *cursor  = vertices + rootchild;

    eval_vertexp(cursor);
    Uint edgelen = edge_length(*cursor);
    Uint plen    = prefixlen(*cursor, *patt, edgelen);

    if(is_prefix(plen, edgelen)) {
        return make_match(is_matched(*patt, plen));
    } else {
        patt->cursor += edgelen;
    }
    return res;
}


///////////////////////////////////////////////////////////////////////////////
// Public Interface


bool search(Wchar *patt_start, Wchar *patt_end)
{

    Pattern patt = init_pattern(patt_start, patt_end);

    VertexP cursor;
    Match rootmatch = match_rootedge(&patt, &cursor);

    if (rootmatch.done) {
        return rootmatch.success;
    }

    while(!is_empty(patt)) {

        patt.head = *patt.cursor;
        cursor    = vertices + CHILD(cursor);

        while(true) {

            if (IS_LEAF(cursor)) {

                Match match = try_match_leaf(patt, cursor);

                if (match.done) {
                    return match.success;
                } else {
                    cursor += LEAF_VERTEXSIZE;
                }

            } else {
                Wchar firstchar = *(wtext + offset(cursor));
                if(firstchar == patt.head) {
                    break;
                }
                if(IS_LASTCHILD(cursor)) {
                    return false;
                } else {
                    cursor += INNER_VERTEXSIZE;
                }
            }
        }

        eval_vertexp(&cursor);
        Uint edgelen = edge_length(cursor);
        Uint plen    = prefixlen(cursor, patt, edgelen);

        if(is_prefix(plen, edgelen)) {
            return is_matched(patt, plen);
        }

        patt.cursor += edgelen;
    }
    return true;
}
