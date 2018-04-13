#include "test.h"

char *utest_search()
{
    return NULL;
}

char *test_search()
{
    mu_run_utest(utest_search);

    return NULL;
}

