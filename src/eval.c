
#include "eval.h"

Uchar   *text, *sentinel, **suffixbase, **sort_buffer;

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
        previousnode = nextfreeentry;
        // create branching node
        if(r > l) {
            if(firstbranch == UNDEFREFERENCE) {
                firstbranch = INDEX(nextfreeentry);
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
                firstbranch = INDEX(nextfreeentry);
            }
            STOREBOUNDARIES(nextfreeentry,l,r);
            // store l and r. resume later with this unevaluated branch node
            rootchildtab[firstchar] = INDEX(nextfreeentry);
            DEBUGCODE(1,lastrootchild = INDEX(nextfreeentry));
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

void eval_node(Uint node)
{
    Uint prefixlen, *vertex;
    Uchar **left, **right;

    DEBUG1(3,"#eval_node(%lu)\n",(Showuint) node);
    vertex = stree + node;
    left = GETLEFTBOUNDARY(vertex);
    right = GETRIGHTBOUNDARY(vertex);
    SETLP(vertex,SUFFIXNUMBER(left));
    SETFIRSTCHILD(vertex,INDEX(nextfreeentry));

    sort_buffer = getsbufferspacelazy(left,right);
    prefixlen = grouplcp(left,right);
    counting_sort(left,right,prefixlen);
    (void) evalsuccedges(left,right);
}

