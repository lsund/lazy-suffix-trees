
#include "eval.h"

Wchar   *wtext, *sentinel, **recurse_suffixes;

Uint    root_children[MAX_CHARS + 1], n_recursed, new_suffixes;


static bool skip_sentinel(Wchar ***rightb)
{
    if(**rightb == sentinel) {
        rightb--;
        return true;
    }
    return false;
}


static void get_rightb(Wchar ***ret, Wchar **curr, Wchar **rightb, Wchar first)
{
    Wchar **probe = curr;
    while (probe < rightb && **(probe + 1) == first) {
        probe++;
    }
    *ret = probe;
}


static void create_edges(Wchar **leftb, Wchar **rightb, Uint **previous, bool isroot)
{
    Wchar **curr_leftb  = NULL;
    Wchar **curr_rightb = NULL;

    for (curr_leftb = leftb; curr_leftb <= rightb; curr_leftb = curr_rightb + 1) {

        Wchar first = **curr_leftb;
        get_rightb(&curr_rightb, curr_leftb, rightb, first);
        *previous = vertices.next;

        if (curr_rightb > curr_leftb) {
            create_inner_vertex(first, curr_leftb, curr_rightb, isroot);
        } else {
            create_leaf_vertex(first, curr_leftb, isroot);
        }
    }
}
static void eval_edges(Wchar **leftb, Wchar **rightb, bool isroot)
{
    bool sentineledge    = skip_sentinel(&rightb);

    if (isroot) {
        init_root_children();
    } else {
        alloc_extend_stree();
    }

    Uint *previous       = NULL;
    create_edges(leftb, rightb, &previous, isroot);

    if (sentineledge) {
        create_sentinel_vertex(rightb, &previous);
    }

    if (isroot) {
        *vertices.next = WITH_LEAF_AND_LASTCHILDBIT(textlen);
        vertices.next += LEAF_VERTEXSIZE;
    } else {
        *previous = WITH_LASTCHILDBIT(*previous);
    }
}


static void eval_vertex(Vertex vertex_val, Wchar ***leftb, Wchar ***rightb)
{
    VertexP vertex = vertices.first + vertex_val;
    *leftb   = LEFT_BOUND(vertex);
    *rightb  = RIGHT_BOUND(vertex);

    SET_OFFSET(vertex, SUFFIX_INDEX(*leftb));
    CHILD(vertex) =  INDEX(vertices.next);

    counting_sort(*leftb, *rightb);
}


void eval_root()
{
    if (!root_evaluated) {
        create_suffix_groups();
        eval_edges(suffixes, suffixes + textlen - 1, true);
        root_evaluated = true;
    }
}

void eval_branch(Vertex vertex_val)
{
    Wchar **leftb;
    Wchar **rightb;
    eval_vertex(vertex_val, &leftb, &rightb);
    eval_edges(leftb, rightb, false);
}
