#ifndef GET_ALPHA_H
#define GET_ALPHA_H

#include <stdio.h>
#include <limits.h>
#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <locale.h>

#include "types.h"
#include "config.h"
#include "text.h"


// Iterate over the text and store the counts of each character. Then iterate
// over all possible characters and store their occurences in the `alpha`
// variable..
void get_characters(Wchar *alpha, Uint *alphasize);

// Get the character with the maximum value in the entire text. In the case of
// UTF-8, this value is the decimal value of the symbols code-point.
Uint get_max(Wchar *text, Uint textlen);


#endif
