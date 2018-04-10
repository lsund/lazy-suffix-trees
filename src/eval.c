
#include "eval.h"

Uchar   *text, *sentinel, **suffixbase, **sort_buffer;

Uint    *next_free,
        rootchildtab[UCHAR_MAX + 1],
        suffixessize,
        maxunusedsuffixes;


static Uint evalsuccedges(Uchar **left,Uchar **right)
{
    Uchar firstchar, **r, **l;
    Uint leafnum, firstbranch = UNDEFREFERENCE, *previousnode = NULL;
    Bool sentineledge = False;

    allocstree();
    if(*right == sentinel) {
        // skip the smallest suffix
        right--;
        sentineledge = True;
    }
    for(l=left; l<=right; l=r+1) {
        for(firstchar=**l, r = l; r < right && **(r + 1) == firstchar; r++)
        {
            ;
        }
        previousnode = next_free;
        // create branching node
        if(r > l) {
            if(firstbranch == UNDEFREFERENCE) {
                firstbranch = INDEX(next_free);
            }
            STOREBOUNDARIES(next_free,l,r);
            // store l and r. resume later with this unevaluated node
            next_free += BRANCHWIDTH;
        } else // create leaf
        {
            leafnum = SUFFIXNUMBER(l);
            SETLEAF(next_free,leafnum);
            next_free++;
        }
    }
    if(sentineledge)
    {
        leafnum = SUFFIXNUMBER(right+1);
        SETLEAF(next_free,leafnum);
        previousnode = next_free++;
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
                firstbranch = INDEX(next_free);
            }
            STOREBOUNDARIES(next_free,l,r);
            // store l and r. resume later with this unevaluated branch node
            rootchildtab[firstchar] = INDEX(next_free);
            next_free += BRANCHWIDTH;
        } else // create leaf
        {
            leafnum = SUFFIXNUMBER(l);
            SETLEAF(next_free,leafnum);
            rootchildtab[firstchar] = leafnum | LEAFBIT;
            next_free++;
        }
    }
    SETLEAF(next_free,textlen | RIGHTMOSTCHILDBIT);
    next_free++;
    return firstbranch;
}

void eval_node(Uint node)
{
    Uint prefixlen, *vertex;
    Uchar **left, **right;

    DEBUG1(3,"#eval_node(%lu)\n",(Showuint) node);
    vertex = stree + node;
    left   = GETLEFTBOUNDARY(vertex);
    right  = GETRIGHTBOUNDARY(vertex);
    SETLP(vertex,SUFFIXNUMBER(left));
    SETFIRSTCHILD(vertex,INDEX(next_free));

    sort_buffer = getsbufferspacelazy(left,right);
    prefixlen = grouplcp(left,right);
    counting_sort(left,right,prefixlen);
    (void) evalsuccedges(left,right);
}

