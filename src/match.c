#include "match.h"


Text text;


Match failed_match()
{
    Match res;
    res.done = false;
    res.success = false;
    return res;
}


Match successful_match()
{
    Match res;
    res.done = true;
    res.success = true;
    return res;
}


Match exhausted_match()
{
    Match res;
    res.done = true;
    res.success = false;
    return res;
}


Uint leaf_lcp(Wchar *textp, Pattern patt)
{
    return lcp(patt.current + 1, patt.end, textp + 1, text.lst - 1);
}


Uint inner_lcp(Uint *vertex, Pattern patt, Uint edgelen)
{
    Wchar *textp = text.fst + LEFTBOUND(vertex);
    return lcp(patt.current + 1, patt.end, textp + 1, textp + edgelen - 1);
}


Match make_match(Wchar *textp, Pattern patt)
{
    Match res;
    res.done = true;
    Uint len = lcp(patt.current + 1, patt.end, textp + 1, text.lst - 1);
    res.success = pattern_has_length(patt, len);
    return res;
}


Match match_leafedge(VertexP vertex, Pattern patt)
{
    Wchar *leftbound = text.fst + LEFTBOUND(vertex);
    if (*leftbound == patt.head) {
        return make_match(leftbound, patt);
    } else if (leftbound == text.lst || IS_LASTCHILD(vertex)) {
        return exhausted_match();
    } else {
        return failed_match();
    }
}


Match match_edge(VertexP vertex, Pattern patt)
{
    Match res;
    res.done = false;
    res.success = false;
    Uint edgelen = edge_length(vertex);
    Uint plen    = inner_lcp(vertex, patt, edgelen);
    if (plen != edgelen - 1) {
        res.done = true;
        res.success = pattern_has_length(patt, plen);
    }
    return res;
}
