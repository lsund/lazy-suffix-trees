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


static void evaluate_root(Uint textlen)
{
    if(!rootevaluated) {

        counting_sort0();
        evalrootsuccedges(suffixes, suffixes + textlen - 1);
        rootevaluated = True;

    }
}

///////////////////////////////////////////////////////////////////////////////
// Public Interface


Bool occurslazy(
        Uchar *leftpattern,
        Uchar *rightpattern
    )
{
    Uint edgelen, prefixlen;
    Uchar firstchar, edgechar;

    Uchar *probe = leftpattern;
    // Check for empty word
    if(probe > rightpattern) {
        return True;
    } else {
        firstchar = *probe;
    }

    evaluate_root(textlen);

    Uint *vertex;
    Uchar *lefttext;

    // Checks if there is an a-edge from the root
    CHECK_A_EDGE;

    Uint v;
    if (IS_UNEVALUATED(vertex)) {
        v = NODEINDEX(vertex);
        evaluatenodelazy(v);
        vertex = stree + v;
    }

    Uint leftpointer = GET_LP(vertex);
    lefttext = text + GET_LP(vertex);
    edgelen = first_child_lp(vertex) - leftpointer;
    CHECKBRANCHEDGE;

    while(True)
    {
        if(probe > rightpattern)   // check for empty word
        {
            return True;
        }

        firstchar = *probe;
        vertex   = stree + FIRST_CHILD(vertex);

        while(True)
        {
            if(IS_LEAF(vertex)) {

                leftpointer = GET_LP(vertex);
                lefttext = text + leftpointer;
                MATCH_LEAF_EDGE;

                if(IS_RIGHTMOST(vertex)) {
                    return False;
                }
                vertex++;
            } else {

                if(IS_UNEVALUATED(vertex)) {
                    leftpointer = GET_LP_UNEVAL(vertex);
                } else {
                    leftpointer = GET_LP(vertex);
                }
                lefttext = text + leftpointer;
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
            v = NODEINDEX(vertex);
            evaluatenodelazy(v);
            vertex = stree + v;
        }
        edgelen = first_child_lp(vertex) - leftpointer;
        CHECKBRANCHEDGE;
    }
}

Bool occurseager(
        Uchar *leftpattern,
        Uchar *rightpattern)
{
  Uint *vertex, edgelen, newleftpointer, leftpointer, prefixlen;
  Uchar *lefttext, *probe = leftpattern, firstchar, edgechar;

  if(probe > rightpattern)   // check for empty word
  {
    return True;
  }
  firstchar = *probe;
  CHECK_A_EDGE;
  leftpointer = GET_LP(vertex);
  lefttext = text + leftpointer;
  vertex = stree + FIRST_CHILD(vertex);
  newleftpointer = GET_LP(vertex);
  edgelen = newleftpointer - leftpointer;
  CHECKBRANCHEDGE;
  while(True)
  {
    if(probe > rightpattern)   // check for empty word
    {
      return True;
    }
    firstchar = *probe;
    while(True)
    {
      leftpointer = GET_LP(vertex);
      lefttext = text + leftpointer;
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
    edgelen = newleftpointer - leftpointer;
    CHECKBRANCHEDGE;
  }
}


Bool occurrenceseager(
        void *state,
        Uchar *leftpattern,
        Uchar *rightpattern)
{
    Uint *vertex, edgelen, newleftpointer, leftpointer, prefixlen;
    Uchar *lefttext, *probe = leftpattern, firstchar, edgechar;
    ArrayUint *resultpos = (ArrayUint *) state;

    resultpos->nextfreeUint = 0;
    if(probe > rightpattern)   // check for empty word
    {
        return True;
    }
    firstchar = *probe;
    CHECK_A_EDGE_POS;

    leftpointer = GET_LP(vertex);
    lefttext = text + leftpointer;
    vertex = stree + FIRST_CHILD(vertex);
    newleftpointer = GET_LP(vertex);
    edgelen = newleftpointer - leftpointer;
    CHECKBRANCHEDGEWITHPOS;
    while(True)
    {
        if(probe > rightpattern)   // check for empty word
        {
            ARRAY_STORE(resultpos, Uint, CELL_SIZE, newleftpointer);
            return True;
        }
        firstchar = *probe;
        while(True)
        {
            leftpointer = GET_LP(vertex);
            lefttext = text + leftpointer;
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
        edgelen = newleftpointer - leftpointer;
        CHECKBRANCHEDGEWITHPOS;
    }
}

