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
    patt.probe = patt_start;
    patt.end   = patt_end;
    return patt;
}


static Uint first_child_lp(Uint *vertex)
{
    Uint *child = vertices + CHILD(vertex);

    if (!IS_LEAF(child) && IS_UNEVALUATED(child)) {
        return GET_LP_UNEVAL(child);
    } else {
        return GET_LP(child);
    }
}


static bool no_root_edge(Pattern patt)
{
    return (root_children[patt.head]) == UNDEFINEDSUCC;
}


static bool empty_pattern(Pattern patt)
{
    return patt.probe > patt.end;
}


static bool match_length(Pattern patt, Uint len)
{
    return (Uint) (patt.end - patt.probe) == len;
}


static bool match(Wchar *text_probe, Pattern patt)
{
    // Changed below to plus 1
    Uint len = lcp(patt.probe + 1, patt.end, text_probe + 1, sentinel - 1);
    return match_length(patt, len);
}


static bool is_prefix(Uint prefixlen, Uint edgelen)
{
    return prefixlen != edgelen - 1;
}


static bool match_a_edge(
                Uint rootchild,
                Wchar **text_probe,
                Pattern patt
            )
{
    *text_probe = wtext + (rootchild & ~LEAFBIT);
    return match(*text_probe, patt);
}


static Uint edge_length(Uint *vertex)
{
    Uint lp = GET_LP(vertex);
    return first_child_lp(vertex) - lp;
}


static Uint get_lp(Uint *vertex)
{
    if(IS_UNEVALUATED(vertex)) {
        return GET_LP_UNEVAL(vertex);
    } else {
        return GET_LP(vertex);
    }
}


static Uint eval_vertex(Uint **vertex)
{
    Uint vertex_num;
    if(IS_UNEVALUATED(*vertex)) {
        vertex_num = INDEX(*vertex);
        eval_node(vertex_num);
        *vertex = vertices + vertex_num;
    }
    vertex_num = -1;
    return vertex_num;

}


static void update_lengths(
        Uint *vertex,
        Pattern patt,
        Uint *edgelen,
        Uint *prefixlen)
{
    Wchar *text_probe = wtext + GET_LP(vertex);
    *edgelen = edge_length(vertex);
    *prefixlen = lcp(patt.probe+1, patt.end, text_probe+1, text_probe+*edgelen-1);
}


static Result try_match_leaf(Pattern patt, Uint *vertex)
{
    Wchar *text_probe = wtext + GET_LP(vertex);
    Result res;
    res.def = false;
    res.val = false;
    if(text_probe == sentinel || IS_RIGHTMOST(vertex)) {
        res.val = false;
        res.def = true;
    }
    if(*text_probe == patt.head) {
        res.val = match(text_probe, patt);
        res.def = true;
    }
    return res;
}


///////////////////////////////////////////////////////////////////////////////
// Public Interface


bool search(Wchar *patt_start, Wchar *patt_end)
{

    Pattern patt = init_pattern(patt_start, patt_end);

    if(empty_pattern(patt)) {
        return true;
    }

    evaluate_root();
    if (no_root_edge(patt)) {
        return false;
    }
    Uint rootchild = root_children[patt.head];

    Wchar *text_probe;
    if (IS_LEAF(&rootchild)) {
        return match_a_edge(rootchild, &text_probe, patt);
    }

    Uint *vertex = vertices + rootchild;
    Uint vertex_num = eval_vertex(&vertex);

    Uint edgelen;
    Uint prefixlen;
    update_lengths(vertex, patt, &edgelen, &prefixlen);

    if(is_prefix(prefixlen, edgelen)) {
        return match_length(patt, prefixlen);
    }

    patt.probe += edgelen;

    while(!empty_pattern(patt)) {

        patt.head = *patt.probe;
        vertex    = vertices + CHILD(vertex);

        while(true) {

            if (IS_LEAF(vertex)) {

                Result match = try_match_leaf(patt, vertex);

                if (match.def) {
                    return match.val;
                } else {
                    vertex += LEAF_VERTEXSIZE;
                }

            } else {

                Uint lp         = get_lp(vertex);
                text_probe      = wtext + lp;
                Wchar firstchar = *text_probe;

                if(firstchar == patt.head) {
                    break;
                }
                if(IS_RIGHTMOST(vertex)) {
                    return false;
                } else {
                    vertex += INNER_VERTEXSIZE;
                }
            }
        }

        vertex_num = eval_vertex(&vertex);
        update_lengths(vertex, patt, &edgelen, &prefixlen);

        if(is_prefix(prefixlen, edgelen)) {
            return match_length(patt, prefixlen);
        }

        patt.probe += edgelen;
    }

    return true;
}

