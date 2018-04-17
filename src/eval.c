
#include "eval.h"

wchar_t   *wtext, *sentinel, **current_sortbuffer;

Uint    *next_free, root_children[UCHAR_MAX + 1];


static bool skip_sentinel(wchar_t ***right)
{
    if(**right == sentinel) {
        right--;
        return true;
    }
    return false;
}

static void create_inner_vertex(Uint *firstbranch, wchar_t **l, wchar_t **r)
{
    if(*firstbranch == UNDEFREFERENCE) {
        *firstbranch = INDEX(next_free);
    }
    STOREBOUNDARIES(next_free, l, r);
    // store l and r. resume later with this unevaluated node
    next_free += BRANCHWIDTH;
}

static Uint create_leaf_vertex(wchar_t **l)
{
    Uint leafnum = SUFFIXNUMBER(l);
    SETLEAF(next_free, leafnum);
    next_free++;
    return leafnum;
}


static Uint create_sentinel_vertex(wchar_t **right, Uint **previousnode)
{
    Uint leafnum = create_leaf_vertex(right + 1);
    *previousnode = next_free;
    return leafnum;
}


static void get_bound(wchar_t ***bound_ptr, wchar_t **probe, wchar_t **right, wchar_t firstchar)
{
    wchar_t **bound;
    for(bound = probe; bound < right && **(bound + 1) == firstchar; bound++) {
        ;
    }
    *bound_ptr = bound;
}


static Uint evalsuccedges(wchar_t **left, wchar_t **right)
{
    wchar_t firstchar, **bound, **probe;
    Uint firstbranch = UNDEFREFERENCE, *previousnode = NULL;

    allocstree();
    bool sentineledge = skip_sentinel(&right);


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
// `evaledges`, and in addition it initialized `root_children`
Uint evalrootsuccedges(wchar_t **left, wchar_t **right)
{
    wchar_t firstchar, **r, **l;
    Uint *rptr, leafnum, firstbranch = UNDEFREFERENCE;

    for(rptr = root_children; rptr <= root_children + UCHAR_MAX; rptr++) {
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
            root_children[firstchar] = INDEX(next_free);
            next_free += BRANCHWIDTH;
        } else {
            // Create leaf
            leafnum = SUFFIXNUMBER(l);
            SETLEAF(next_free,leafnum);
            root_children[firstchar] = leafnum | LEAFBIT;
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
    wchar_t **left, **right;

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
