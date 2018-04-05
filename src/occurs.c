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


static Bool no_successor(Uchar patt_head)
{
    return (rootchildtab[patt_head]) == UNDEFINEDSUCC;
}


static Bool check_a_edge(
                Uint rootchild,
                Uchar **text_probe,
                Uchar *patt_probe,
                Uchar *patt_end)
{
    *text_probe = text + (rootchild & ~LEAFBIT);
    Uint len = lcp(patt_probe + 1, patt_end, *text_probe + 1, sentinel - 1);
    return ((Uint) (patt_end - patt_probe) == len);
}


static Bool finished(Uchar *patt_probe, Uchar *patt_end)
{
    return patt_probe > patt_end;
}

///////////////////////////////////////////////////////////////////////////////
// Public Interface


Bool search(Uchar *patt, Uchar *patt_end)
{


    Uchar *text_probe, *patt_probe;
    Uchar patt_head;

    // Check for empty word
    if(patt > patt_end) {
        return True;
    } else {
        patt_probe = patt;
        patt_head  = *patt;
    }

    evaluate_root_lazy();

    Uint *vertex;
    Uint rootchild;

    if (no_successor(patt_head)) {
        return False;
    } else {
        rootchild = rootchildtab[patt_head];
    }

    if (IS_LEAF(&rootchild)) {
        return check_a_edge(rootchild, &text_probe, patt_probe, patt_end);
    }

    vertex = stree + rootchild;

    Uint vertex_num;

    if (IS_UNEVALUATED(vertex)) {

        vertex_num = INDEX(vertex);
        eval_node(vertex_num);
        vertex = stree + vertex_num;

    }

    Uint lp = GET_LP(vertex);
    text_probe = text + GET_LP(vertex);

    Uint edgelen = first_child_lp(vertex) - lp;
    Uint prefixlen;
    // Tries to match the remainder of the pattern with the current branch edge
    prefixlen = lcp(patt_probe+1,patt_end,text_probe+1,text_probe+edgelen-1);
    if(prefixlen == edgelen - 1) {
        patt_probe += edgelen;
    } else {
        return (prefixlen == (Uint) (patt_end - patt_probe));
    }

    Uchar edgechar;

    while(!finished(patt_probe, patt_end)) {

        patt_head = *patt_probe;
        vertex   = stree + FIRST_CHILD(vertex);

        while(True) {

            if (IS_LEAF(vertex)) {

                lp = GET_LP(vertex);
                text_probe = text + lp;
                MATCH_LEAF_EDGE;

                if(IS_RIGHTMOST(vertex)) {
                    return False;
                }
                vertex++;

            } else {

                if(IS_UNEVALUATED(vertex)) {
                    lp = GET_LP_UNEVAL(vertex);
                } else {
                    lp = GET_LP(vertex);
                }
                text_probe = text + lp;
                edgechar = *text_probe;
                if(edgechar == patt_head)
                {
                    break;
                }
                if(IS_RIGHTMOST(vertex))
                {
                    return False;
                }
                vertex += BRANCHWIDTH;
            }
        }
        if(IS_UNEVALUATED(vertex)) {
            vertex_num = INDEX(vertex);
            eval_node(vertex_num);
            vertex = stree + vertex_num;
        }
        edgelen = first_child_lp(vertex) - lp;
        prefixlen = lcp(patt_probe+1,patt_end,text_probe+1,text_probe+edgelen-1);
        if(prefixlen == edgelen - 1) {
            patt_probe += edgelen;
        } else {
            return (prefixlen == (Uint) (patt_end - patt_probe));
        }
    }

    return True;
}

