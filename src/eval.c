
#include "eval.h"

Uchar   *text, *sentinel, **suffixbase, **sbuffer;

Uint    *nextfreeentry,
        rootchildtab[UCHAR_MAX + 1],
        suffixessize,
        maxunusedsuffixes,
        leafcount,
        branchcount,
        lastrootchild;


static Uint evalsuccedges(Uchar **left,Uchar **right)
{
    Uchar firstchar, **r, **l;
    Uint leafnum, firstbranch = UNDEFREFERENCE, *previousnode = NULL;
    BOOL sentineledge = False;

    allocstree();
    if(*right == sentinel)
    {
        right--;  // skip the smallest suffix
        sentineledge = True;
    }
    for(l=left; l<=right; l=r+1)
    {
        for(firstchar=**l,r=l; r<right && **(r+1)==firstchar; r++)
        {
            /* nothing */ ;
        }
        previousnode = nextfreeentry;
        // create branching node
        if(r > l)
        {
            if(firstbranch == UNDEFREFERENCE)
            {
                firstbranch = NODEINDEX(nextfreeentry);
            }
            STOREBOUNDARIES(nextfreeentry,l,r);
            // store l and r. resume later with this unevaluated node
            nextfreeentry += BRANCHWIDTH;
            DEBUGCODE(1,branchcount++);
        } else // create leaf
        {
            leafnum = SUFFIXNUMBER(l);
            SETLEAF(nextfreeentry,leafnum);
            nextfreeentry++;
            DEBUGCODE(1,leafcount++);
        }
    }
    if(sentineledge)
    {
        leafnum = SUFFIXNUMBER(right+1);
        SETLEAF(nextfreeentry,leafnum);
        previousnode = nextfreeentry++;
        DEBUGCODE(1,leafcount++);
    }
    NOTSUPPOSEDTOBENULL(previousnode);
    *previousnode |= RIGHTMOSTCHILDBIT;
    return firstbranch;
}

Uint evalrootsuccedges(Uchar **left,Uchar **right)
{
    Uchar firstchar, **r, **l;
    Uint *rptr, leafnum, firstbranch = UNDEFREFERENCE;

    for(rptr = rootchildtab; rptr <= rootchildtab + UCHAR_MAX; rptr++)
    {
        *rptr = UNDEFINEDSUCC;
    }
    for(l=left; l<=right; l=r+1) // first phase
    {
        for(firstchar=**l,r=l; r<right && **(r+1)==firstchar; r++)
        {
            /* nothing */ ;
        }
        if(r > l) // create branching node
        {
            if(firstbranch == UNDEFREFERENCE)
            {
                firstbranch = NODEINDEX(nextfreeentry);
            }
            STOREBOUNDARIES(nextfreeentry,l,r);
            // store l and r. resume later with this unevaluated branch node
            rootchildtab[firstchar] = NODEINDEX(nextfreeentry);
            DEBUGCODE(1,lastrootchild = NODEINDEX(nextfreeentry));
            nextfreeentry += BRANCHWIDTH;
            DEBUGCODE(1,branchcount++);
        } else // create leaf
        {
            leafnum = SUFFIXNUMBER(l);
            SETLEAF(nextfreeentry,leafnum);
            rootchildtab[firstchar] = leafnum | LEAFBIT;
            nextfreeentry++;
            DEBUGCODE(1,leafcount++);
        }
    }
    SETLEAF(nextfreeentry,textlen | RIGHTMOSTCHILDBIT);
    nextfreeentry++;
    DEBUGCODE(1,leafcount++);
    return firstbranch;
}

static Uint evaluatenodeeager(Uint node)
{
    Uint prefixlen, *nodeptr, unusedsuffixes;
    Uchar **left, **right;

    DEBUG1(3,"#evaluatenodeeager(%lu)\n",(Showuint) node);
    nodeptr = stree + node;
    left = GETLEFTBOUNDARY(nodeptr);
    right = GETRIGHTBOUNDARY(nodeptr);
    SETLP(nodeptr,SUFFIXNUMBER(left));
    SETFIRSTCHILD(nodeptr,NODEINDEX(nextfreeentry));

    unusedsuffixes = (Uint) (left - suffixes);
    if(suffixessize > UintConst(10000) && unusedsuffixes > maxunusedsuffixes)
    {
        Uint tmpdiff, width = (Uint) (right - left + 1);
        Uchar **i, **j;
        for(i=left, j=suffixes; i<suffixes+suffixessize; i++, j++)
        {
            *j = *i;  // move remaining suffixes to the left
        }
        suffixessize -= unusedsuffixes;
        maxunusedsuffixes = suffixessize >> 1;
        tmpdiff = (Uint) (suffixes - suffixbase);
        DEBUG2(2,"#move[%lu,%lu] ",(Showuint) (left-suffixes),
                (Showuint) (left-suffixes+suffixessize));
        DEBUG1(2,"[0,%lu]\n",(Showuint) suffixessize);
        ALLOC(suffixes,suffixes,Uchar *,suffixessize);
        suffixbase = suffixes - (tmpdiff + unusedsuffixes);
        left = suffixes;
        right = suffixes + width - 1;
    }
    sbuffer = getsbufferspaceeager(left, right);
    prefixlen = grouplcp(left,right);
    counting_sort(left,right,prefixlen);
    return evalsuccedges(left,right);
}

void evaluatenodelazy(Uint node)
{
    Uint prefixlen, *nodeptr;
    Uchar **left, **right;

    DEBUG1(3,"#evaluatenodelazy(%lu)\n",(Showuint) node);
    nodeptr = stree + node;
    left = GETLEFTBOUNDARY(nodeptr);
    right = GETRIGHTBOUNDARY(nodeptr);
    SETLP(nodeptr,SUFFIXNUMBER(left));
    SETFIRSTCHILD(nodeptr,NODEINDEX(nextfreeentry));

    sbuffer = getsbufferspacelazy(left,right);
    prefixlen = grouplcp(left,right);
    counting_sort(left,right,prefixlen);
    (void) evalsuccedges(left,right);
}

static Uint getnextbranch(Uint previousbranch)
{
    Uint *nodeptr = stree + previousbranch;

    if(ISRIGHTMOSTCHILD(nodeptr))
    {
        return UNDEFREFERENCE;
    }
    nodeptr += BRANCHWIDTH;
    while(True)
    {
        if(ISLEAF(nodeptr))
        {
            if(ISRIGHTMOSTCHILD(nodeptr))
            {
                return UNDEFREFERENCE;
            }
            nodeptr++;
        } else
        {
            return NODEINDEX(nodeptr);
        }
    }
}

void evaluateeager(void)
{
  Uint firstbranch, nextbranch, node, stacktop=0, stackalloc=0, *stack = NULL;

  counting_sort0();
  firstbranch = evalrootsuccedges(suffixes,suffixes+textlen-1);
  if(firstbranch != UNDEFREFERENCE)
  {
    PUSHNODE(firstbranch);
    while(NOTSTACKEMPTY)
    {
      POPNODE(node);
      while(node != UNDEFREFERENCE)
      {
        if((nextbranch = getnextbranch(node)) != UNDEFREFERENCE)
        {
          PUSHNODE(nextbranch);
        }
        node = evaluatenodeeager(node);
      }
    }
    FREE(stack);
  }
}

