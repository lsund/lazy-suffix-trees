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


#include "lcp.h"

wchar_t *sentinel;


Uint lcp(wchar_t *start1, wchar_t *end1, wchar_t *start2, wchar_t *end2) {

    wchar_t *probe1 = start1, *probe2 = start2;

    while(probe1 <= end1 && probe2 <= end2 && *probe1 == *probe2)
    {
        probe1++;
        probe2++;
    }

    return (Uint) (probe1 - start1);
}


Uint grouplcp(wchar_t **left, wchar_t **right) {

    wchar_t cmpchar, **i;
    Uint j = UintConst(1);

    while (true) {

        if(*right + j == sentinel) {
            return j;
        }

        cmpchar = *(*left + j);
        for(i = left + 1; i <= right; i++) {

            if(*(*i + j) != cmpchar) {
                return j;
            }
        }

        j++;
    }
}
