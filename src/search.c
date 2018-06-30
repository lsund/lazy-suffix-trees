#include "search.h"

///////////////////////////////////////////////////////////////////////////////
// Functions


static Match try_match_leaf(Pattern patt, Uint *vertex)
{
    Wchar *text_cursor = text.fst + LEFTBOUND(vertex);
    if (*text_cursor == patt.head) {
        return match_leaf(text_cursor, patt);
    } else if (text_cursor == text.lst || IS_LASTCHILD(vertex)) {
        return exhausted_match();
    } else {
        return failed_match();
    }
}

static Match match_rootedge(Pattern *patt, VertexP *cursor)
{
    Match match;
    match.done     = false;
    match.success  = false;

    if(pattern_is_empty(*patt)) {
        match.done = true;
        match.success = true;
        return match;
    }

    eval_root();

    if (!has_root_edge(patt->head)) {
        return exhausted_match();
    }

    Vertex rootchild  = st.rs[patt->head];
    Wchar *text_start = text.fst + WITHOUT_LEAFBIT(rootchild);
    if (IS_LEAF(&rootchild)) {
        return match_leaf(text_start, *patt);
    }

    *cursor  = st.vs.fst + rootchild;

    eval_if_uneval(cursor, eval_branch);
    Uint edgelen = edge_length(*cursor);
    Uint plen    = inner_lcp(*cursor, *patt, edgelen);

    if(plen != edgelen - 1) {
        match.done = true;
        match.success = pattern_has_length(*patt, plen);
        return match;
    } else {
        patt->cursor += edgelen;
    }
    return match;
}


static void find_next_child()
{
}

bool search_aux(Pattern patt)
{

    VertexP current_vertex;
    Match rootmatch = match_rootedge(&patt, &current_vertex);

    if (rootmatch.done) {
        return rootmatch.success;
    }

    while(!pattern_is_empty(patt)) {

        patt.head = *patt.cursor;
        current_vertex    = st.vs.fst + CHILD(current_vertex);

        while(true) {

            if (IS_LEAF(current_vertex)) {

                Match match = try_match_leaf(patt, current_vertex);
                if (match.done) {
                    return match.success;
                } else {
                    current_vertex += LEAF_VERTEXSIZE;
                }

            } else {
                Wchar firstchar = *(text.fst + text_leftbound(current_vertex));
                if(firstchar == patt.head) {
                    break;
                }
                if(IS_LASTCHILD(current_vertex)) {
                    return false;
                } else {
                    current_vertex += INNER_VERTEXSIZE;
                }
            }
        }

        eval_if_uneval(&current_vertex, eval_branch);
        Uint edgelen = edge_length(current_vertex);
        Uint plen    = inner_lcp(current_vertex, patt, edgelen);

        if(plen !=  edgelen - 1) {
            return pattern_has_length(patt, plen);
        }

        patt.cursor += edgelen;
    }
    return true;
}

static bool copy_pattern(Wchar *pattern, Wchar *current_pattern, Uint len)
{
    *(pattern + len) = '\0';
    for(Uint i = 0; i < len; i++) {

        pattern[i] = current_pattern[i];

    }

    return false;
}


///////////////////////////////////////////////////////////////////////////////
// Public API


bool search(Wchar *current_pattern, Uint patternlen)
{
    Wchar pattern[MAXPATTERNLEN + 1];
    copy_pattern(pattern, current_pattern, patternlen);
    Pattern patt = pattern_init(pattern, pattern + patternlen - 1);

    return search_aux(patt);
}


// Search for many patterns in the tree
void search_many(const char *path, int npatterns, Wchar ***patterns_ptr)
{
    Wchar **patterns = *patterns_ptr;
    FILE *fp        = open_append(path);

    for(int j = 0; j < npatterns; j++) {

        Wchar *current_pattern = patterns[j];
        Uint patternlen = wcslen(current_pattern);

        search(current_pattern, patternlen);

    }
    fclose(fp);
}
