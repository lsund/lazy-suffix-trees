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


#include "get_alpha.h"

wchar_t *wtext;

Uint textlen;

// Iterate over the text and store the counts of each character. Then iterate
// over 1->256 (all uchars) and store them in the `alpha` parameter, if the
// occurence is greater than 0.
void get_characters(wchar_t *alpha, Uint *alphasize)
{
    Uint counts[MAX_CHARS + 1] = {0};
    wchar_t *text_probe;

    Uint max = 0;
    for (text_probe = wtext; text_probe < wtext + textlen; text_probe++) {
        if ((Uint) *text_probe > max) {
            max = *text_probe;
            printf("%lu\n", max);
        }
        counts[(Uint) *text_probe]++;
    }

    Uint i, j;
    for (j = 0, i = 0; i <= MAX_CHARS; i++) {
        if (counts[i] > 0) {
            alpha[j++] = (wchar_t) i;
        }
    }
    *alphasize = j;
}


Uint get_max(wchar_t *text, Uint textlen)
{

    wchar_t *text_probe;
    Uint max = 0;
    for (text_probe = text; text_probe < text + textlen; text_probe++) {
        if ((Uint) *text_probe > max) {
            printf("New max: %lu\n", (Uint) *text_probe);
            max = *text_probe;
        }
    }
    return max;
}


