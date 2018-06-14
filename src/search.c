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

#include "search.h"

Wchar *wtext,
        *sentinel,
        **suffixes,
        **recurse_suffixes;

Uint *vertices, root_children[MAX_CHARS + 1], n_recursed, new_suffixes,
     *leaf_nums, n_leafnums;

bool root_evaluated;
bool evaluated = false;

static Pattern init_pattern(Wchar *patt_start, Wchar *patt_end)
{
    Pattern patt;
    patt.head   = *patt_start;
    patt.cursor = patt_start;
    patt.start = patt_start;
    patt.end    = patt_end;
    return patt;
}


static Vertex first_child_lp(VertexP vertex)
{
    VertexP child = vertices + CHILD(vertex);

    if (!IS_LEAF(child) && IS_UNEVALUATED(child)) {
        return OFFSET_UNEVAL(child);
    } else {
        return OFFSET(child);
    }
}


static bool no_root_edge(Pattern patt)
{
    return (root_children[patt.head]) == UNDEFINEDSUCC;
}


static bool is_empty(Pattern patt)
{
    return patt.cursor > patt.end;
}

static bool is_prefix(Uint prefixlen, Uint edgelen)
{
    return prefixlen != edgelen - 1;
}


static bool is_matched(Pattern patt, Uint len)
{
    return (Uint) (patt.end - patt.cursor) == len;
}


static bool match(Wchar *text_cursor, Pattern patt)
{
    // Changed below to plus 1
    Uint len = lcp(patt.cursor + 1, patt.end, text_cursor + 1, sentinel - 1);
    return is_matched(patt, len);
}


static Uint offset(VertexP vertex)
{
    if(IS_UNEVALUATED(vertex)) {
        return OFFSET_UNEVAL(vertex);
    } else {
        return OFFSET(vertex);
    }
}

static Vertex edge_length(VertexP vertex)
{
    Uint lp = OFFSET(vertex);
    return first_child_lp(vertex) - lp;
}



void traverse(VertexP cursor, Uint matchedlen)
{
    Uint depth = 0;

    while(true) {

        if (!IS_LEAF(cursor)) {
            Uint edgelen = edge_length(cursor);
            depth += edgelen;
            cursor    = vertices + CHILD(cursor);
        }

        if (IS_LEAF(cursor)) {
            printf("%lu\n", n_leafnums);
            leaf_nums[n_leafnums++] = OFFSET(cursor) - depth - matchedlen;
            if(IS_LASTCHILD(cursor)) {
                break;
            }
            cursor += LEAF_VERTEXSIZE;
        } else {
            traverse(cursor, matchedlen + depth);
            if(IS_LASTCHILD(cursor)) {
                break;
            } else {
                cursor += INNER_VERTEXSIZE;
            }
        }
    }
}
static void eval_if_uneval(VertexP *vertex, void (*eval_fun)(Vertex))
{
    if(IS_UNEVALUATED(*vertex)) {
        evaluated = true;
        Uint index = INDEX(*vertex);
        *vertex = vertices + index;
        eval_fun(index);
    }
}


static Uint prefixlen(Uint *vertex, Pattern patt, Uint edgelen)
{
    Wchar *text_cursor = wtext + OFFSET(vertex);
    return lcp(patt.cursor + 1, patt.end, text_cursor + 1, text_cursor + edgelen - 1);
}


static Match try_match_leaf(Pattern patt, Uint *vertex)
{
    Wchar *text_cursor = wtext + OFFSET(vertex);
    /* printf("Matching leaf: %ls with %ls\n", text_cursor, patt.cursor); */
    if (*text_cursor == patt.head) {
        return make_match(match(text_cursor, patt));
    }
    if (text_cursor == sentinel || IS_LASTCHILD(vertex)) {
        return unsuccessful_match();
    }
    return incomplete_match();
}

static Match match_rootedge(Pattern *patt, VertexP *cursor)
{
    Match res;
    res.done     = false;
    res.success  = false;

    if(is_empty(*patt)) {
        return successful_match();
    }

    eval_root();

    if (no_root_edge(*patt)) {
        return unsuccessful_match();
    }

    Vertex rootchild  = root_children[patt->head];
    Wchar *text_start = wtext + WITHOUT_LEAFBIT(rootchild);
    if (IS_LEAF(&rootchild)) {
        return make_match(match(text_start, *patt));
    }

    *cursor  = vertices + rootchild;

    eval_if_uneval(cursor, eval_branch);
    Uint edgelen = edge_length(*cursor);
    Uint plen    = prefixlen(*cursor, *patt, edgelen);

    if(is_prefix(plen, edgelen)) {
        return make_match(is_matched(*patt, plen));
    } else {
        patt->cursor += edgelen;
    }
    return res;
}


///////////////////////////////////////////////////////////////////////////////
// Public Interface

bool search(Wchar *patt_start, Wchar *patt_end)
{

    Pattern patt = init_pattern(patt_start, patt_end);

    VertexP cursor;
    Match rootmatch = match_rootedge(&patt, &cursor);

    if (rootmatch.done) {
        return rootmatch.success;
    }

    while(!is_empty(patt)) {

        patt.head = *patt.cursor;
        cursor    = vertices + CHILD(cursor);

        while(true) {

            if (IS_LEAF(cursor)) {

                Match match = try_match_leaf(patt, cursor);

                if (match.done) {
                    return match.success;
                } else {
                    cursor += LEAF_VERTEXSIZE;
                }

            } else {
                Wchar firstchar = *(wtext + offset(cursor));
                if(firstchar == patt.head) {
                    break;
                }
                if(IS_LASTCHILD(cursor)) {
                    return false;
                } else {
                    cursor += INNER_VERTEXSIZE;
                }
            }
        }

        eval_if_uneval(&cursor, eval_branch);
        Uint edgelen = edge_length(cursor);
        Uint plen    = prefixlen(cursor, patt, edgelen);

        if(is_prefix(plen, edgelen)) {
            return is_matched(patt, plen);
        }

        patt.cursor += edgelen;
    }
    return true;
}

static void recurse(VertexP cursor, Uint matched, Uint edgelen, Uint number)
{
    /* Wchar *text_cursor = wtext + offset(cursor); */
    /* printf("RECURSING AT: %ls\n\n", text_cursor); */
    while (n_recursed > 0 && number > 0) {
        n_recursed--;
        number--;
        /* printf("Recursing with: %ls\n", recurse_suffixes[n_recursed]); */
        printf("Recursed: %lu\n", n_recursed);
        find_leafnums(
                recurse_suffixes[n_recursed],
                recurse_suffixes[n_recursed] + wcslen(recurse_suffixes[n_recursed]) - 1,
                cursor,
                matched + edgelen, true);
    }
}


bool find_leafnums(
        Wchar *patt_start,
        Wchar *patt_end,
        VertexP start,
        Uint matched,
        bool recursing)
{
    Pattern patt = init_pattern(patt_start, patt_end);

    VertexP cursor;

    if (!start) {
        eval_root();
        Vertex rootchild  = root_children[patt.head];
        cursor  = vertices + rootchild;

        eval_if_uneval(&cursor, eval_branch_suffixes);
        Uint edgelen = edge_length(cursor);
        Uint plen    = prefixlen(cursor, patt, edgelen);

        if(is_prefix(plen, edgelen)) {
            make_match(is_matched(patt, plen));
        } else {
            patt.cursor += edgelen;
        }

        if (is_empty(patt)) {
            recurse(cursor, matched, edgelen, new_suffixes);
        }

    } else {
        cursor = start;
    }

    while(!is_empty(patt)) {

        patt.head = *patt.cursor;
        cursor    = vertices + CHILD(cursor);

        while(true) {

            if (IS_LEAF(cursor)) {

                Match match = try_match_leaf(patt, cursor);

                if (match.done) {
                    Uint remaining = patt.end - patt.cursor;
                    if (recursing) {
            printf("%lu\n", n_leafnums);
                        leaf_nums[n_leafnums++] = OFFSET(cursor) - matched;
                    } else {
            printf("%lu\n", n_leafnums);
                        leaf_nums[n_leafnums++] = OFFSET(cursor) - matched + remaining;
                    }
                    return true;
                } else {
                    cursor += LEAF_VERTEXSIZE;
                }

            } else {
                Wchar firstchar = *(wtext + offset(cursor));
                if(firstchar == patt.head) {
                    break;
                }
                if(IS_LASTCHILD(cursor)) {
                    return false;
                } else {
                    cursor += INNER_VERTEXSIZE;
                }
            }
        }
        eval_if_uneval(&cursor, eval_branch_suffixes);
        Uint edgelen = edge_length(cursor);
        Uint plen    = prefixlen(cursor, patt, edgelen);

        if(is_prefix(plen, edgelen)) {
            printf("XXXXXXXXXXX\n");
            return true;
        }

        patt.cursor += edgelen;
        if (is_empty(patt) || recursing) {
            recurse(cursor, matched, edgelen, new_suffixes);
            break;
        }
    }
    if (!evaluated) {
        traverse(cursor, patt.end - patt.start);
    }
    return true;
}


