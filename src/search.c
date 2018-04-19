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


#include "search.h"


///////////////////////////////////////////////////////////////////////////////
// Functions


static bool copy_pattern(wchar_t *pattern, wchar_t *current_pattern, Uint len)
{
    *(pattern + len) = '\0';
    for(Uint i = 0; i < len; i++) {

        pattern[i] = current_pattern[i];

    }

    return false;
}


///////////////////////////////////////////////////////////////////////////////
// Public API


// Search for one pattern in the tree
bool try_search(wchar_t *current_pattern, Uint patternlen)
{

    wchar_t pattern[MAXPATTERNLEN + 1];
    copy_pattern(pattern, current_pattern, patternlen);

    return search(pattern, pattern + patternlen - 1);

}


// Search for many patterns in the tree
void try_search_patterns(const char *path, int npatterns, wchar_t ***patterns_ptr)
{
    int noccurs     = 0;
    wchar_t **patterns = *patterns_ptr;
    FILE *fp        = open_append(path);

    for(int j = 0; j < npatterns; j++) {

        wchar_t *current_pattern = patterns[j];
        Uint patternlen = strlenw(current_pattern);

        bool exists = try_search(current_pattern, patternlen);

        if (exists) {
            fprintf(fp, "%ls\n", patterns[j]);
            noccurs++;
        }
    }

    printf("noccurs: %d\n", noccurs);
    printtime();

    fclose(fp);
}
