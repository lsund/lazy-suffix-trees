/*
  Copyright by Stefan Kurtz (C) 1999-2003
  =====================================
  You may use, copy and distribute this file freely as long as you
   - do not change the file,
   - leave this copyright notice in the file,
   - do not make any profit with the distribution of this file
   - give credit where credit is due
  You are not allowed to copy or distribute this file otherwise
  The commercial usage and distribution of this file is prohibited
  Please report bugs and suggestions to <kurtz@zbh.uni-hamburg.de>
*/

/*
 * Modified by Ludvig Sundström 2018 with permission from Stefan Kurtz.
 * For full source control tree, see https://github.com/lsund/wotd
 */


#include "pattern_searcher.h"

Uint *leaf_nums, n_leafnums;

///////////////////////////////////////////////////////////////////////////////
// Functions


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


bool find_pattern(Wchar *current_pattern, Uint patternlen)
{
    Wchar pattern[MAXPATTERNLEN + 1];
    copy_pattern(pattern, current_pattern, patternlen);
    Pattern patt = init_pattern(pattern, pattern + patternlen - 1);

    return search(patt);
}


// Search for many patterns in the tree
void search_patterns(const char *path, int npatterns, Wchar ***patterns_ptr)
{
    int noccurs     = 0;
    Wchar **patterns = *patterns_ptr;
    FILE *fp        = open_append(path);

    for(int j = 0; j < npatterns; j++) {

        Wchar *current_pattern = patterns[j];
        Uint patternlen = wcslen(current_pattern);

        bool exists = find_pattern(current_pattern, patternlen);

        if (exists) {
            noccurs++;
        }
    }

    printf("noccurs: %d\n", noccurs);
    printtime();

    fclose(fp);
}
