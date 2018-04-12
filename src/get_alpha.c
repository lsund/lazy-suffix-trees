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

void get_characters(Uchar *text, Uint textlen, Uchar *alpha, Uint *alphasize)
{
    Uint occ[UCHAR_MAX + 1] = {0};
    Uchar *text_probe;

    for (text_probe = text; text_probe < text + textlen; text_probe++) {
        occ[(Uint) *text_probe]++;
    }

    Uint i, j;
    for (j = 0, i = 0; i <= UCHAR_MAX; i++) {
        if (occ[i] > 0) {
            alpha[j++] = (Uchar) i;
        }
    }
    *alphasize = j;
}
