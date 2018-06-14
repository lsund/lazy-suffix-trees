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

Text text;

// Let x be the string that starts at address `start1` and ends at `end1`
// Let y be the string that starts at address `start2` and ends at `end2`
// Then this function returns the longest common prefix of x and y.
Uint lcp(Wchar *start1, Wchar *end1, Wchar *start2, Wchar *end2) {

    Wchar *probe1 = start1, *probe2 = start2;

    while(probe1 <= end1 && probe2 <= end2 && *probe1 == *probe2)
    {
        probe1++;
        probe2++;
    }

    return (Uint) (probe1 - start1);
}


// The longest common prefix for the group of suffixes, laying between adresses
// `left` and `right`
Uint grouplcp(Wchar **left, Wchar **right) {

    Uint j = UINT(1);

    while (true) {

        if(*right + j == text.sentinel) {
            return j;
        }

        Wchar cmpchar = *(*left + j);
        Wchar **text_probe;
        for(text_probe = left + 1; text_probe <= right; text_probe++) {

            if(*(*text_probe + j) != cmpchar) {
                return j;
            }
        }

        j++;
    }
}
