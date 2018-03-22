#include "occurs.h"

// Checks if there is an a-edge out from the root
#define CHECKROOTCHILD\
        {\
          Uint rootchild;\
          if((rootchild = rootchildtab[firstchar]) == UNDEFINEDSUCC)\
          {\
            return False;\
          }\
          if(rootchild & LEAFBIT)\
          {\
            lefttext = text + (rootchild & ~LEAFBIT);\
            if ((Uint) (rightpattern-lpatt) ==\
                        lcp(lpatt + 1, rightpattern, lefttext+1,sentinel-1))\
            {\
              return True;\
            }\
            return False;\
          }\
          nodeptr = streetab + rootchild;\
        }


// Tries to match the remainder of the pattern with the current leaf edge
#define CHECKLEAFEDGE\
        if(lefttext == sentinel)\
        {\
          return False;\
        }\
        edgechar = *lefttext;\
        if(edgechar == firstchar)\
        {\
          if((Uint) (rightpattern - lpatt) ==\
                     lcp(lpatt+1,rightpattern,lefttext+1,sentinel-1))\
          {\
            return True;\
          }\
          return False;\
        }

// Tries to match the remainder of the pattern with the current branch edge
#define CHECKBRANCHEDGE\
        prefixlen = UintConst(1)+\
                    lcp(lpatt+1,rightpattern,lefttext+1,lefttext+edgelen-1);\
        if(prefixlen == edgelen)\
        {\
          lpatt += edgelen;\
        } else\
        {\
          if(prefixlen == (Uint) (rightpattern - lpatt + 1))\
          {\
            return True;\
          }\
          return False;\
        }


static Uint firstchildlp(Uint *nodeptr)
{
  Uint *firstchildptr = streetab + GETFIRSTCHILD(nodeptr);

  if(!ISLEAF(firstchildptr) && ISUNEVALUATED(firstchildptr))
  {
    return GETLPUNEVAL(firstchildptr);
  } else
  {
    return GETLP(firstchildptr);
  }
}

BOOL occurslazy(
        void *state,
        Uchar *text,
        Uint textlen,
        Uchar *leftpattern,
        Uchar *rightpattern
        )
{
    Uint leftpointer, node, *nodeptr, edgelen, prefixlen;
    Uchar *lefttext, *lpatt = leftpattern, firstchar, edgechar;

    if(lpatt > rightpattern)   // check for empty word
    {
        return True;
    }
    firstchar = *lpatt;

    if(!rootevaluated)
    {
        sortByChar0();
        (void) evalrootsuccedges(suffixes,suffixes+textlen-1);
        rootevaluated = True;
    }

    CHECKROOTCHILD;

    if(ISUNEVALUATED(nodeptr))
    {
        node = NODEINDEX(nodeptr);
        evaluatenodelazy(node);
        nodeptr = streetab + node;
    }

    leftpointer = GETLP(nodeptr);
    lefttext = text + GETLP(nodeptr);
    edgelen = firstchildlp(nodeptr) - leftpointer;
    CHECKBRANCHEDGE;

    while(True)
    {
        if(lpatt > rightpattern)   // check for empty word
        {
            return True;
        }

        firstchar = *lpatt;
        nodeptr = streetab + GETFIRSTCHILD(nodeptr);

        while(True)
        {
            if(ISLEAF(nodeptr))
            {
                leftpointer = GETLP(nodeptr);
                lefttext = text + leftpointer;
                CHECKLEAFEDGE;
                if(ISRIGHTMOSTCHILD(nodeptr))
                {
                    return False;
                }
                nodeptr++;
            } else
            {
                if(ISUNEVALUATED(nodeptr))
                {
                    leftpointer = GETLPUNEVAL(nodeptr);
                } else
                {
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
        if(ISUNEVALUATED(nodeptr))
        {
            node = NODEINDEX(nodeptr);
            evaluatenodelazy(node);
            nodeptr = streetab + node;
        }
        edgelen = firstchildlp(nodeptr) - leftpointer;
        CHECKBRANCHEDGE;
    }
}

BOOL occurseager(/*@unused@*/ void *state,Uchar *text,
                        /*@unused@*/ Uint textlen,
                        Uchar *leftpattern,Uchar *rightpattern)
{
  Uint *nodeptr, edgelen, newleftpointer, leftpointer, prefixlen;
  Uchar *lefttext, *lpatt = leftpattern, firstchar, edgechar;

  if(lpatt > rightpattern)   // check for empty word
  {
    return True;
  }
  firstchar = *lpatt;
  CHECKROOTCHILD;
  leftpointer = GETLP(nodeptr);
  lefttext = text + leftpointer;
  nodeptr = streetab + GETFIRSTCHILD(nodeptr);
  newleftpointer = GETLP(nodeptr);
  edgelen = newleftpointer - leftpointer;
  CHECKBRANCHEDGE;
  while(True)
  {
    if(lpatt > rightpattern)   // check for empty word
    {
      return True;
    }
    firstchar = *lpatt;
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
    nodeptr = streetab + GETFIRSTCHILD(nodeptr);
    newleftpointer = GETLP(nodeptr);
    edgelen = newleftpointer - leftpointer;
    CHECKBRANCHEDGE;
  }
}

