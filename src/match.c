#include "match.h"


Text text;


static Match failed_match()
{
    Match res;
    res.done = false;
    res.success = false;
    return res;
}


Match exhausted_match()
{
    Match res;
    res.done = true;
    res.success = false;
    return res;
}


Match make_done_match(Wchar *label, Pattern patt)
{
    Match res;
    res.done = true;
    Uint len = lcp(patt.current + 1, patt.end, label + 1, text.lst - 1);
    res.success = pattern_has_length(patt, len);
    return res;
}


Match match_leafedge(VertexP v, Pattern patt)
{
    Wchar *leftb = text.fst + LEFTBOUND(v);
    if (*leftb == patt.head) {
        return make_done_match(leftb, patt);
    } else if (leftb == text.lst || IS_LASTCHILD(v)) {
        return exhausted_match();
    } else {
        return failed_match();
    }
}


Match match_edge(VertexP v, Pattern patt)
{
    Match res;
    res.done = false;
    res.success = false;
    Uint edgelen = edge_length(v);

    Wchar *textp = text.fst + LEFTBOUND(v);
    Uint plen =  lcp(patt.current + 1, patt.end, textp + 1, textp + edgelen - 1);
    if (plen != edgelen - 1) {
        res.done = true;
        res.success = pattern_has_length(patt, plen);
    }
    return res;
}
