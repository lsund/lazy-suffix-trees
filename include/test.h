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
 * ======================================
 *
 * Modified by Ludvig Sundström 2018 with permission from Stefan Kurtz
 * For full source control tree, see https://github.com/lsund/wotd
 *
 */

#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>

#include "minunit.h"
#include "search.h"
#include "io.h"
#include "naive_search.h"
#include "util.h"

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
char *test_experiments();

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
