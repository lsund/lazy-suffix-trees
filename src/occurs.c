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


#include "occurs.h"


static Pattern init_pattern(Uchar *patt_start, Uchar *patt_end)
{
    Pattern patt;
    patt.head  = *patt_start;
    patt.probe = patt_start;
    patt.end   = patt_end;
    return patt;
}


static Uint first_child_lp(Uint *vertex)
{
    Uint *child = stree + FIRST_CHILD(vertex);

    if (!IS_LEAF(child) && IS_UNEVALUATED(child)) {
        return GET_LP_UNEVAL(child);
    } else {
        return GET_LP(child);
    }
}


static void evaluate_root_lazy()
{
    if(!rootevaluated) {

        create_suffix_groups();
        evalrootsuccedges(suffixes, suffixes + textlen - 1);
        rootevaluated = True;

    }
}


static Bool no_successor(Pattern patt)
{
    return (rootchildtab[patt.head]) == UNDEFINEDSUCC;
}


static Bool empty_pattern(Pattern patt)
{
    return patt.probe > patt.end;
}


static Bool pattern_matched(Pattern patt, Uint len)
{
    return (Uint) (patt.end - patt.probe) == len;
}


static Bool match(Uchar *text_probe, Pattern patt)
{
    Uint len = lcp(patt.probe, patt.end, text_probe, sentinel - 1);
    return pattern_matched(patt, len);
}


static Bool is_prefix(Uint prefixlen, Uint edgelen)
{
    return prefixlen != edgelen - 1;
}


static Bool check_a_edge(
                Uint rootchild,
                Uchar **text_probe,
                Pattern patt
            )
{
    *text_probe = text + (rootchild & ~LEAFBIT);
    return match(*text_probe, patt);
}


static Bool edge_length(Uint *vertex)
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


static void eval_if_uneval(Uint **vertex, Uint *vertex_num)
{
    if(IS_UNEVALUATED(*vertex)) {
        *vertex_num = INDEX(*vertex);
        eval_node(*vertex_num);
        *vertex = stree + *vertex_num;
    }

}


static void update_lengths(
        Uint *vertex,
        Pattern patt,
        Uint *edgelen,
        Uint *prefixlen)
{
    Uchar *text_probe = text + GET_LP(vertex);
    *edgelen = edge_length(vertex);
    *prefixlen = lcp(patt.probe+1, patt.end, text_probe+1, text_probe+*edgelen-1);
}


static Result try_match_leaf(Uchar *text_probe, Pattern patt, Uint *vertex)
{
    Result res;
    res.def = False;
    res.val = False;
    if(text_probe == sentinel || IS_RIGHTMOST(vertex)) {
        res.val = False;
        res.def = True;
    }
    if(*text_probe == patt.head) {
        res.val = match(text_probe, patt);
        res.def = True;
    }
    return res;
}


///////////////////////////////////////////////////////////////////////////////
// Public Interface


Bool search(Uchar *patt_start, Uchar *patt_end)
{


    Uchar *text_probe;
    Uint edgelen, prefixlen;
    Uint *vertex;
    Uint vertex_num;

    Pattern patt = init_pattern(patt_start, patt_end);

    if(empty_pattern(patt)) {
        return True;
    }

    evaluate_root_lazy();

    if (no_successor(patt)) {
        return False;
    }

    Uint rootchild = rootchildtab[patt.head];

    if (IS_LEAF(&rootchild)) {
        return check_a_edge(rootchild, &text_probe, patt);
    }

    vertex = stree + rootchild;
    eval_if_uneval(&vertex, &vertex_num);
    update_lengths(vertex, patt, &edgelen, &prefixlen);

    if(is_prefix(prefixlen, edgelen)) {
        return pattern_matched(patt, prefixlen);
    }

    patt.probe += edgelen;

    while(!empty_pattern(patt)) {

        patt.head = *patt.probe;
        vertex   = stree + FIRST_CHILD(vertex);

        Uint lp;

        while(True) {

            if (IS_LEAF(vertex)) {

                text_probe = text + GET_LP(vertex);

                Result leaf_matched = try_match_leaf(text_probe, patt, vertex);

                if (leaf_matched.def == True) {
                    return leaf_matched.val;
                }

                vertex++;

            } else {

                Uchar edgechar;
                lp         = get_lp(vertex);
                text_probe = text + lp;
                edgechar   = *text_probe;

                if(edgechar == patt.head) {
                    break;
                }

                if(IS_RIGHTMOST(vertex)) {
                    return False;
                }
                vertex += BRANCHWIDTH;
            }
        }

        eval_if_uneval(&vertex, &vertex_num);

        update_lengths(vertex, patt, &edgelen, &prefixlen);

        if(is_prefix(prefixlen, edgelen)) {
            return pattern_matched(patt, prefixlen);
        }

        patt.probe += edgelen;
    }

    return True;
}

