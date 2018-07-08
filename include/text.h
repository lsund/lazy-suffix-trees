#ifndef EXTERNS_H
#define EXTERNS_H

#include "types.h"
#include "config.h"

// A Text represents the text, or the string to build a suffix tree from.
typedef struct text {
    // Points to the start of the allocated block, the first character in the
    // text..
    Wchar *fst;
    // Points to the last of the allocated black, the last character in the
    // text.
    Wchar *lst;
    // The number of characters in the text.
    Uint len;
    // The number of distinct characters in the text, the alphabet size.
    Uint asize;
    // The distinct characters in alphabetical order.
    Wchar cs[MAX_CHARS + 1];
    // The maximum integer value of all characters.
    Uint maxc;
    // Points to the first suffix of the text
    Suffix *ss;
} Text;

extern Text text;

// Get the right leftbound in the sufix buffer
#define SUFFIX_LEFTBOUND(R)             (text.ss + LEFTBOUND(R))
// Get the right rightbound in the suffix buffer
#define SUFFIX_RIGHTBOUND(R)            (text.ss + (FIRSTCHILD(R) & ~MSB))

// The suffix number of a given suffix
#define SUFFIX_INDEX(R)                 ((Uint) (*(R) - text.fst))

// If the leftbound is not set for a suffix, then it is just the suffix index
// for that suffix.
#define MAKE_LEFTBOUND(R)               SUFFIX_INDEX(SUFFIX_LEFTBOUND(R))

#endif
