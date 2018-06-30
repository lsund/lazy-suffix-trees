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


Uint leaf_lcp(Wchar *text_cursor, Pattern patt)
{
    return lcp(patt.cursor + 1, patt.end, text_cursor + 1, text.lst - 1);
}


Uint inner_lcp(Uint *vertex, Pattern patt, Uint edgelen)
{
    Wchar *text_cursor = text.fst + LEFTBOUND(vertex);
    return lcp(patt.cursor + 1, patt.end, text_cursor + 1, text_cursor + edgelen - 1);
}


Match match_leaf(Wchar *text_cursor, Pattern patt)
{
    Match res;
    res.done = true;
    // Changed below to plus 1
    Uint len = lcp(patt.cursor + 1, patt.end, text_cursor + 1, text.lst - 1);
    res.success = pattern_has_length(patt, len);
    return res;
}
