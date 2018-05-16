#ifndef MATCH_H
#define MATCH_H

#include "types.h"

typedef struct match {
    bool done;
    bool success;
} Match;

Match incomplete_match();

Match successful_match();

Match unsuccessful_match();

Match make_match(bool success);

#endif
