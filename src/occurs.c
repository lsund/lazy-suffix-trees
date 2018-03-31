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


static void collectpositions(ArrayUint *resultpos, Uint *firstsucc)
{
    Uint leftpointer, *nodeptr = firstsucc;

    while(True) {

        if (ISLEAF(nodeptr)) {
            leftpointer = GETLP(nodeptr);
            STOREINARRAY(resultpos, Uint, 256, leftpointer);
            if (ISRIGHTMOSTCHILD(nodeptr)) {
                break;
            }
            nodeptr++;
        } else {
            collectpositions(resultpos, nodeptr);
            if (ISRIGHTMOSTCHILD(nodeptr)) {
                break;
            }
            nodeptr += BRANCHWIDTH;
        }
    }
}


static Uint firstchildlp(Uint *nodeptr)
{
    Uint *firstchildptr = stree + GETFIRSTCHILD(nodeptr);

    if (!ISLEAF(firstchildptr) && ISUNEVALUATED(firstchildptr)) {
        return GETLPUNEVAL(firstchildptr);
    } else {
        return GETLP(firstchildptr);
    }
}


static void evaluate_root(Uint textlen)
{
    if(!rootevaluated)
    {
        counting_sort0();
        evalrootsuccedges(suffixes, suffixes + textlen - 1);
        rootevaluated = True;
    }
}

///////////////////////////////////////////////////////////////////////////////
// Public Interface


BOOL occurslazy(
        Uchar *text,
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

    Uint *nodeptr;
    Uchar *lefttext;

    // Checks if there is an a-edge from the root
    CHECK_A_EDGE;

    Uint v;
    if (ISUNEVALUATED(nodeptr)) {
        v = NODEINDEX(nodeptr);
        evaluatenodelazy(v);
        nodeptr = stree + v;
    }

    Uint leftpointer = GETLP(nodeptr);
    lefttext = text + GETLP(nodeptr);
    edgelen = firstchildlp(nodeptr) - leftpointer;
    CHECKBRANCHEDGE;

    while(True)
    {
        if(probe > rightpattern)   // check for empty word
        {
            return True;
        }

        firstchar = *probe;
        nodeptr   = stree + GETFIRSTCHILD(nodeptr);

        while(True)
        {
            if(ISLEAF(nodeptr)) {

                leftpointer = GETLP(nodeptr);
                lefttext = text + leftpointer;
                CHECKLEAFEDGE;

                if(ISRIGHTMOSTCHILD(nodeptr)) {
                    return False;
                }
                nodeptr++;
            } else {

                if(ISUNEVALUATED(nodeptr)) {
                    leftpointer = GETLPUNEVAL(nodeptr);
                } else {
                    leftpointer = GETLP(nodeptr);
                }
                lefttext = text + leftpointer;
                edgechar = *lefttext;
                if(edgechar == firstchar)
                {
                    break;
                }
                if(ISRIGHTMOSTCHILD(nodeptr))
                {
                    return False;
                }
                nodeptr += BRANCHWIDTH;
            }
        }
        if(ISUNEVALUATED(nodeptr)) {
            v = NODEINDEX(nodeptr);
            evaluatenodelazy(v);
            nodeptr = stree + v;
        }
        edgelen = firstchildlp(nodeptr) - leftpointer;
        CHECKBRANCHEDGE;
    }
}

BOOL occurseager(
        Uchar *text,
        Uchar *leftpattern,
        Uchar *rightpattern)
{
  Uint *nodeptr, edgelen, newleftpointer, leftpointer, prefixlen;
  Uchar *lefttext, *probe = leftpattern, firstchar, edgechar;

  if(probe > rightpattern)   // check for empty word
  {
    return True;
  }
  firstchar = *probe;
  CHECK_A_EDGE;
  leftpointer = GETLP(nodeptr);
  lefttext = text + leftpointer;
  nodeptr = stree + GETFIRSTCHILD(nodeptr);
  newleftpointer = GETLP(nodeptr);
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
      leftpointer = GETLP(nodeptr);
      lefttext = text + leftpointer;
      if(ISLEAF(nodeptr))
      {
        CHECKLEAFEDGE;
        if(ISRIGHTMOSTCHILD(nodeptr))
        {
          return False;
        }
        nodeptr++;
      } else
      {
        edgechar = *lefttext;
        if(edgechar == firstchar)
        {
          break;
        }
        if(ISRIGHTMOSTCHILD(nodeptr))
        {
          return False;
        }
        nodeptr += BRANCHWIDTH;
      }
    }
    nodeptr = stree + GETFIRSTCHILD(nodeptr);
    newleftpointer = GETLP(nodeptr);
    edgelen = newleftpointer - leftpointer;
    CHECKBRANCHEDGE;
  }
}


BOOL occurrenceseager(
        void *state,
        Uchar *text,
        Uchar *leftpattern,
        Uchar *rightpattern)
{
    Uint *nodeptr, edgelen, newleftpointer, leftpointer, prefixlen;
    Uchar *lefttext, *probe = leftpattern, firstchar, edgechar;
    ArrayUint *resultpos = (ArrayUint *) state;

    resultpos->nextfreeUint = 0;
    if(probe > rightpattern)   // check for empty word
    {
        return True;
    }
    firstchar = *probe;
    CHECKROOTCHILDWITHPOS;
    leftpointer = GETLP(nodeptr);
    lefttext = text + leftpointer;
    nodeptr = stree + GETFIRSTCHILD(nodeptr);
    newleftpointer = GETLP(nodeptr);
    edgelen = newleftpointer - leftpointer;
    CHECKBRANCHEDGEWITHPOS;
    while(True)
    {
        if(probe > rightpattern)   // check for empty word
        {
            STOREINARRAY(resultpos,Uint,256,newleftpointer);
            return True;
        }
        firstchar = *probe;
        while(True)
        {
            leftpointer = GETLP(nodeptr);
            lefttext = text + leftpointer;
            if(ISLEAF(nodeptr))
            {
                CHECKLEAFEDGEWITHPOS;
                if(ISRIGHTMOSTCHILD(nodeptr))
                {
                    return False;
                }
                nodeptr++;
            } else
            {
                edgechar = *lefttext;
                if(edgechar == firstchar)
                {
                    break;
                }
                if(ISRIGHTMOSTCHILD(nodeptr))
                {
                    return False;
                }
                nodeptr += BRANCHWIDTH;
            }
        }
        nodeptr = stree + GETFIRSTCHILD(nodeptr);
        newleftpointer = GETLP(nodeptr);
        edgelen = newleftpointer - leftpointer;
        CHECKBRANCHEDGEWITHPOS;
    }
}

