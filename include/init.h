#ifndef INIT_H
#define INIT_H

#include "text.h"
#include "types.h"
#include "spaceman.h"
#include "io.h"
#include "get_alpha.h"
#include "search.h"
#include "streedef.h"
#include "sortbuffer.h"

// Initializes the global variable `st` which represents the suffix tree. It
// does so by initializing the alphabet, root children, sortbuffer by
// allocating enough space to start computation and setting the static fields
// to sound initial values.
void init();

// 'Destroy' the global variable `st` by freeing all memory allocated to it and
// unsetting its fields.
void destroy();

#endif
