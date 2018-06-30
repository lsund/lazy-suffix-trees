/*****************************************************************************

* File Name: test.c

* Author: Ludvig Sundström

* Description:

* Creation Date: 16-07-2015

*****************************************************************************/

#include "test.h"

int entry_index;
int tests_run, utests_run;

static char *test()
{
    mu_message(MODULE, "search\n");
    mu_run_test(test_search);
    /* mu_message(MODULE, "experiments\n"); */
    /* mu_run_test(test_experiments); */
    return 0;
}


void mu_message(enum MessageOption option, const char *s)
{
    switch (option) {
        case MODULE:
            fprintf(
                stdout,
                ANSI_COLOR_YELLOW "Testing Module: %s" ANSI_COLOR_RESET,
                s
            );
            break;
        case UNIT:
            fprintf(
                stdout,
                ANSI_COLOR_CYAN "\tTesting Unit: %s" ANSI_COLOR_RESET,
                s
            );
            break;
        case DATA:
            fprintf(
                stdout,
                ANSI_COLOR_GREY "\t%s" ANSI_COLOR_RESET,
                s
            );
            break;
        case PASSING:
            fprintf(
                stdout,
                ANSI_COLOR_GREEN "\t\tPassed" ANSI_COLOR_RESET "\n"
            );
            break;
        case STATISTICS:
            fprintf(
                stdout,
                ANSI_COLOR_RESET "%s" ANSI_COLOR_RESET,
                s
            );
            break;
        case PREPEND:
            fprintf(
                stdout, ANSI_COLOR_GREY "%s" ANSI_COLOR_RESET,
                s
            );
            break;
        case ERROR:
            fprintf(
                stdout, ANSI_COLOR_RED   "Error >> %s" ANSI_COLOR_RESET "\n",
                s
            );
            break;
        case TESTSPASSED:
            fprintf(
                stdout,
                ANSI_COLOR_GREEN "ALL TESTS PASSED" ANSI_COLOR_RESET "\n"
            );
            break;
    }
}

static char *all_tests() {
    char *result;
    if ((result = test())) { return result; }

    return 0;
}

int main(int argc, char **argv) {
    char *result;
    char buf1[128], buf2[128];
    argc        = 0;
    argv        = NULL;
    entry_index = -1;
    tests_run   = 0;
    utests_run  = 0;
    result      = all_tests();
    if (result != 0) {
        mu_message(ERROR, result);
    } else {
        sprintf(buf1, "Modules tested: %d\n", tests_run);
        sprintf(buf2, "Unit tests run: %d\n", utests_run);
        mu_message(STATISTICS, buf1);
        mu_message(STATISTICS, buf2);
        mu_message(TESTSPASSED, "");
    }
    return result != 0;
}

