
#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdlib.h>

#include "minunit.h"
#include "search.h"
#include "io.h"
#include "boyermoore.h"

#define ANSI_COLOR_RED      "\x1b[31m"
#define ANSI_COLOR_RESET    "\x1b[0m"
#define ANSI_COLOR_RED      "\x1b[31m"
#define ANSI_COLOR_GREEN    "\x1b[32m"
#define ANSI_COLOR_YELLOW   "\x1b[33m"
#define ANSI_COLOR_BLUE     "\x1b[34m"
#define ANSI_COLOR_MAGENTA  "\x1b[35m"
#define ANSI_COLOR_CYAN     "\x1b[36m"
#define ANSI_COLOR_GREY     "\x1b[37m"
#define ANSI_COLOR_RESET    "\x1b[0m"

char *test_search();

enum MessageOption {
    MODULE,
    UNIT,
    DATA,
    STATISTICS,
    PASSING,
    PREPEND,
    ERROR,
    TESTSPASSED
};

void mu_message(enum MessageOption, const char *s);

extern int tests_run;
extern int utests_run;

// Testing works the following way: Each public function in each module is
// tested. private functions are tested indirectly, because of the testing of
// the public functions. The public functions are tested for abilities to handle
// all inputs and that the outputs make sense.

#endif
