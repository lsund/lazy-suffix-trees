#include "search.h"

static Match match_rootedge(VertexP *current_vertex, Pattern *patt)
{
    Match match;
    match.done     = false;
    match.success  = false;

    if (pattern_is_empty(*patt)) {
        match.done = true;
        match.success = true;
        return match;
    }

    eval_root();

    if (!has_root_edge(patt->fst)) {
        return exhausted_match();
    }

    Vertex root_child  = st.rs[patt->fst];
    Wchar *leftb = text.fst + MAKE_INNER(root_child);
    if (IS_LEAF(&root_child)) {
        return make_done_match(leftb, *patt);
    }

    *current_vertex  = st.vs.fst + root_child;
    eval_if_uneval(current_vertex);

    Match root_match = match_edge(*current_vertex, *patt);

    if (root_match.done) {
        return root_match;
    } else {
        patt->current += edge_length(*current_vertex);
    }

    return match;
}


static bool sample_random(Wchar *pattern, Uint len)
{
    Uint start = (Uint) (drand48() * (double) (text.len - len));

    if(start > text.len - len) {
        fprintf(stderr,"Not enough characters left\n");
        exit(EXIT_FAILURE);
    }

    for(Uint j = 0; j < len; j++) {
        pattern[j] = text.fst[start + j];
    }

    pattern[len] = '\0';
    return false;
}


///////////////////////////////////////////////////////////////////////////////
// Public API


bool search(Wchar *pattern, Uint len)
{
    Pattern patt = pattern_init(pattern, pattern + len - 1);

    VertexP current_vertex;
    Match rootmatch = match_rootedge(&current_vertex, &patt);

    if (rootmatch.done) {
        return rootmatch.success;
    }

    while(!pattern_is_empty(patt)) {

        patt.fst = *patt.current;
        current_vertex    = st.vs.fst + FIRSTCHILD(current_vertex);

        while(true) {

            if (IS_LEAF(current_vertex)) {

                Match match = match_leafedge(current_vertex, patt);
                if (match.done) {
                    return match.success;
                } else {
                    current_vertex += LEAF_VERTEXSIZE;
                }

            } else {
                Wchar firstchar = *(text.fst + leftbound(current_vertex));
                if(firstchar == patt.fst) {
                    break;
                }
                if(IS_LASTCHILD(current_vertex)) {
                    return false;
                } else {
                    current_vertex += INNER_VERTEXSIZE;
                }
            }
        }

        eval_if_uneval(&current_vertex);

        Match match = match_edge(current_vertex, patt);
        if (match.done) {
            return match.success;
        } else {
            patt.current += edge_length(current_vertex);
        }
    }
    return true;
}


// Search for many patterns in the tree
void search_many(int npatterns, Wchar ***patternsp)
{
    Wchar **patterns = *patternsp;

    for(int j = 0; j < npatterns; j++) {

        Wchar *patt = patterns[j];
        Uint len = wcslen(patt);

        bool exists = search(patt, len);

        if (VERBOSE) {
            char *s_exists = exists ? "true" : "false";
            fprintf(stdout, "'%ls': %s\n", patt, s_exists);
        }
    }
}


void search_random(const char *path, Uint n, Uint minlen, Uint maxlen)
{
    if (maxlen > text.len) {
        fprintf(stderr, "Max pattern length must be smaller than the text length");
    }

    FILE *fp = truncate_open_append(path);
    Uint len;

    // Magic number seed
    srand48(42349421);

    for(Uint i = 0; i < n; i++) {

        Wchar pattern[maxlen + 1];
        len = rand_range(minlen, maxlen);

        sample_random(pattern, len);

        if (i & 1) {
            reverse(pattern, len);
        }

        search(pattern, len);
    }
    fclose(fp);
}
