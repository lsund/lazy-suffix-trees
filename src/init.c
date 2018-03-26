
#include "init.h"

void inittree(void)
{
    Uint i;

    DEBUGCODE(1, maxstacksize=maxwidth=branchcount=leafcount=0);
    getUchars(text, textlen, characters, &alphasize);
    sentinel = text+textlen;
    streesize = BRANCHWIDTH;

    ALLOC(stree,stree,Uint,streesize + MAXSUCCSPACE);
    nextfreeentry = stree;
    suffixessize = textlen + 1;
    maxunusedsuffixes = suffixessize >> 1;

    ALLOC(suffixes,NULL,Uchar *, suffixessize);
    suffixbase = suffixes;
    sbufferwidth = 0;
    maxsbufferwidth = textlen >> 8;
    rootevaluated = False;

    for (i = 0; i < alphasize; i++) {
        alphaindex[(Uint) characters[i]] = i;
    }
    for (i = 0; i <= UCHAR_MAX; i++) {
        occurrence[i] = 0;
    }
}

