
#include "eval.h"

Uchar   *text, *sentinel, **suffixbase, **current_sortbuffer;

Uint    *next_free,
        rootchildtab[UCHAR_MAX + 1],
        suffixessize,
        maxunusedsuffixes;


static Bool skip_sentinel(Uchar ***right)
{
    if(**right == sentinel) {
        right--;
        return True;
    }
    return False;
}

static void create_inner_vertex(Uint *firstbranch, Uchar **l, Uchar **r)
{
    if(*firstbranch == UNDEFREFERENCE) {
        *firstbranch = INDEX(next_free);
    }
    STOREBOUNDARIES(next_free, l, r);
    // store l and r. resume later with this unevaluated node
    next_free += BRANCHWIDTH;
}

static Uint create_leaf_vertex(Uchar **l)
{
    Uint leafnum = SUFFIXNUMBER(l);
    SETLEAF(next_free, leafnum);
    next_free++;
    return leafnum;
}


static Uint create_sentinel_vertex(Uchar **right, Uint **previousnode)
{
    Uint leafnum = create_leaf_vertex(right + 1);
    *previousnode = next_free;
    return leafnum;
}


static void get_bound(Uchar ***bound_ptr, Uchar **probe, Uchar **right, Uchar firstchar)
{
    Uchar **bound;
    for(bound = probe; bound < right && **(bound + 1) == firstchar; bound++) {
        ;
    }
    *bound_ptr = bound;
}


static Uint evalsuccedges(Uchar **left, Uchar **right)
{
    Uchar firstchar, **bound, **probe;
    Uint firstbranch = UNDEFREFERENCE, *previousnode = NULL;

    allocstree();
    Bool sentineledge = skip_sentinel(&right);


    for (probe = left; probe <= right; probe = bound + 1) {

        firstchar = **probe;
        get_bound(&bound, probe, right, firstchar);
        previousnode = next_free;

        if(bound > probe) {
            create_inner_vertex(&firstbranch, probe, bound);
        } else {
            create_leaf_vertex(probe);
        }
    }

    if (sentineledge) {
        create_sentinel_vertex(right, &previousnode);
    }
    *previousnode |= RIGHTMOSTCHILDBIT;

    return firstbranch;
}


// Evaluates all outgoing edges from the root. This is a specialization of
// `evaledges`, and in addition it initialized `rootchildtab`
Uint evalrootsuccedges(Uchar **left, Uchar **right)
{
    Uchar firstchar, **r, **l;
    Uint *rptr, leafnum, firstbranch = UNDEFREFERENCE;

    for(rptr = rootchildtab; rptr <= rootchildtab + UCHAR_MAX; rptr++) {
        *rptr = UNDEFINEDSUCC;
    }
    for(l = left; l <= right; l = r + 1) {
        // First Phase
        for(firstchar=**l,r=l; r<right && **(r+1)==firstchar; r++)
        {
            ;
        }
        if(r > l) {
            // Create branching node
            if(firstbranch == UNDEFREFERENCE) {
                firstbranch = INDEX(next_free);
            }
            STOREBOUNDARIES(next_free, l, r);
            // store l and r. resume later with this unevaluated branch node
            rootchildtab[firstchar] = INDEX(next_free);
            next_free += BRANCHWIDTH;
        } else {
            // Create leaf
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

    vertex = stree + node;
    left   = GETLEFTBOUNDARY(vertex);
    right  = GETRIGHTBOUNDARY(vertex);
    SETLP(vertex,SUFFIXNUMBER(left));
    SETFIRSTCHILD(vertex,INDEX(next_free));

    current_sortbuffer = get_sortbuffer(left, right);
    prefixlen = grouplcp(left,right);
    counting_sort(left,right,prefixlen);
    (void) evalsuccedges(left,right);
}
