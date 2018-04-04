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


static void generate_lps(ArrayUint *resultpos, Uint *firstsucc)
{
    Uint lp;
    Uint *vertex = firstsucc;

    while(True) {

        if (IS_LEAF(vertex)) {

            lp = GET_LP(vertex);
            ARRAY_STORE(resultpos, Uint, CELL_SIZE, lp);

            if (IS_RIGHTMOST(vertex)) {
                break;
            } else {
                vertex++;
            }

        } else {

            generate_lps(resultpos, vertex);

            if (IS_RIGHTMOST(vertex)) {
                break;
            }

            vertex += BRANCHWIDTH;
        }
    }
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


static void evaluate_root_lazy(Uint textlen)
{
    if(!rootevaluated) {

        create_suffix_groups();
        evalrootsuccedges(suffixes, suffixes + textlen - 1);
        rootevaluated = True;

    }
}

///////////////////////////////////////////////////////////////////////////////
// Public Interface


Bool search_lazy(Uchar *left, Uchar *right)
{


    Uchar *lefttext;

    // Check for empty word

    Uchar *probe;
    Uchar firstchar;
    CHECK_EMPTY;

    evaluate_root_lazy(textlen);

    Uint *vertex;
    CHECK_A_EDGE;

    Uint vertex_num;
    if (IS_UNEVALUATED(vertex)) {

        vertex_num = INDEX(vertex);
        eval_node(vertex_num);
        vertex = stree + vertex_num;

    }

    Uint lp = GET_LP(vertex);
    lefttext = text + GET_LP(vertex);

    Uint edgelen = first_child_lp(vertex) - lp;
    Uint prefixlen;
    CHECKBRANCHEDGE;

    Uchar edgechar;
    while(True)
    {
        if(probe > right)   // check for empty word
        {
            return True;
        }

        firstchar = *probe;
        vertex   = stree + FIRST_CHILD(vertex);

        while(True)
        {
            if(IS_LEAF(vertex)) {

                lp = GET_LP(vertex);
                lefttext = text + lp;
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
                lefttext = text + lp;
                edgechar = *lefttext;
                if(edgechar == firstchar)
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
        CHECKBRANCHEDGE;
    }
}

Bool search_eager(Uchar *left, Uchar *right)
{
    Uint *vertex, edgelen, newleftpointer, lp, prefixlen;
    Uchar *lefttext, *probe = left, firstchar, edgechar;

    if(probe > right)   // check for empty word
    {
        return True;
    }
    firstchar = *probe;
    CHECK_A_EDGE;
    lp = GET_LP(vertex);
    lefttext = text + lp;
    vertex = stree + FIRST_CHILD(vertex);
    newleftpointer = GET_LP(vertex);
    edgelen = newleftpointer - lp;
    CHECKBRANCHEDGE;
    while(True)
    {
        if(probe > right)   // check for empty word
        {
            return True;
        }
        firstchar = *probe;
        while(True)
        {
            lp = GET_LP(vertex);
            lefttext = text + lp;
            if(IS_LEAF(vertex))
            {
                MATCH_LEAF_EDGE;
                if(IS_RIGHTMOST(vertex))
                {
                    return False;
                }
                vertex++;
            } else
            {
                edgechar = *lefttext;
                if(edgechar == firstchar)
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
        vertex = stree + FIRST_CHILD(vertex);
        newleftpointer = GET_LP(vertex);
        edgelen = newleftpointer - lp;
        CHECKBRANCHEDGE;
    }
}


Bool occurrenceseager(void *state, Uchar *left, Uchar *right)
{
    Uint *vertex, edgelen, newleftpointer, lp, prefixlen;
    Uchar *lefttext, *probe = left, firstchar, edgechar;
    ArrayUint *resultpos = (ArrayUint *) state;

    resultpos->nextfreeUint = 0;
    if(probe > right)   // check for empty word
    {
        return True;
    }
    firstchar = *probe;
    CHECK_A_EDGE_POS;

    lp = GET_LP(vertex);
    lefttext = text + lp;
    vertex = stree + FIRST_CHILD(vertex);
    newleftpointer = GET_LP(vertex);
    edgelen = newleftpointer - lp;
    CHECKBRANCHEDGEWITHPOS;
    while(True)
    {
        if(probe > right)   // check for empty word
        {
            ARRAY_STORE(resultpos, Uint, CELL_SIZE, newleftpointer);
            return True;
        }
        firstchar = *probe;
        while(True)
        {
            lp = GET_LP(vertex);
            lefttext = text + lp;
            if(IS_LEAF(vertex))
            {
                CHECKLEAFEDGEWITHPOS;
                if(IS_RIGHTMOST(vertex)) {
                    return False;
                }
                vertex++;
            } else
            {
                edgechar = *lefttext;
                if(edgechar == firstchar) {
                    break;
                }
                if(IS_RIGHTMOST(vertex)) {
                    return False;
                }
                vertex += BRANCHWIDTH;
            }
        }
        vertex = stree + FIRST_CHILD(vertex);
        newleftpointer = GET_LP(vertex);
        edgelen = newleftpointer - lp;
        CHECKBRANCHEDGEWITHPOS;
    }
}

