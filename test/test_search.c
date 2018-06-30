#include "test.h"

int maxpatterns = 500;


static Uint min(const Uint a, const Uint b)
{
    return a < b ? a : b;
}

char *test_count(char *patternfile, char *textfile, Uint count)
{
    setlocale(LC_ALL, "en_US.utf8");
    FILE *in = fopen(textfile, "r");
    text.fst = malloc(sizeof(Wchar) * MAXTEXTLEN);
    wint_t c;
    text.len = 0;
    while ((c = fgetwc(in)) != WEOF) {
        text.fst[text.len] = c;
        text.len++;
    }
    text.fst[text.len + 1] = '\0';
    text.maxc = get_max(text.fst, text.len);
    fclose(in);
    Wchar **patterns = (Wchar **) malloc(sizeof(Wchar *) * MAX_PATTERNS);
    int npatterns  = file_to_strings(patternfile, MAX_PATTERNS, &patterns);
    init();

    Uint exists_n = 0;
    for (Uint j = 0; j < (Uint) npatterns; j++) {

        Wchar *current_pattern = patterns[j];
        Uint patternlen = wcslen(current_pattern);

        bool exists = search(current_pattern, patternlen);

        exists ? exists_n++ : (void) 0;
    }
    for (int i = npatterns - 1; i >= 0; i--) {
        free(patterns[i]);
    }
    free(patterns);

    mu_assert("Count should be correct", exists_n == count);

    return NULL;
}

char *compare_vs_naive(char *patternfile, char *textfile)
{
    setlocale(LC_ALL, "en_US.utf8");
    FILE *in = fopen(textfile, "r");
    text.fst = malloc(sizeof(Wchar) * MAXTEXTLEN);
    wint_t c;
    text.len = 0;
    while ((c = fgetwc(in)) != WEOF) {
        text.fst[text.len] = c;
        text.len++;
    }
    text.fst[text.len + 1] = '\0';
    text.maxc = get_max(text.fst, text.len);
    fclose(in);
    Wchar **patterns = (Wchar **) malloc(sizeof(Wchar *) * MAX_PATTERNS);
    int npatterns  = file_to_strings(patternfile, MAX_PATTERNS, &patterns);
    init();

    int exists_n = 0, rexists_n = 0;
    for (Uint j = 0; j < min(npatterns, maxpatterns); j++) {

        Wchar *current_pattern = patterns[j];
        Uint patternlen = wcslen(current_pattern);

        Wchar *end = current_pattern + patternlen;

        bool exists = search(current_pattern, patternlen);
        bool rexists = naive_search(current_pattern, end);

        exists ? exists_n++ : (void) 0;
        rexists ? rexists_n++ : (void) 0;

        if (rexists != exists) {
            printf("really exists %d exists %d\n", rexists, exists);
            printf("Fail on: %ls\n", patterns[j]);
        }
        mu_assert(
            "Naive and suffix tree sourch should be the same.",
            rexists == exists
        );
    }
    for (int i = npatterns - 1; i >= 0; i--) {
        free(patterns[i]);
    }
    free(patterns);
    return NULL;
}

char *utest_search()
{

    char *error;

    mu_message(DATA, "Trivial patterns\n");
    error = compare_vs_naive(
                "data/mini/patt.txt",
                "data/mini/text.txt"
            );
    if (error) return error;

    mu_message(DATA, "Random existing patterns\n");
    error = compare_vs_naive(
                "/home/lsund/Data/testdata/members/random-patterns.txt",
                "/home/lsund/Data/testdata/members/diffsize/005.txt"
            );
    if (error) return error;

    mu_message(DATA, "Random non-existing patterns\n");
    error = compare_vs_naive(
                "/home/lsund/Data/testdata/members/random-patterns-non-existing.txt",
                "/home/lsund/Data/testdata/members/diffsize/005.txt"
            );
    if (error) return error;

    mu_message(DATA, "Akz patterns\n");
    error = compare_vs_naive(
                "/home/lsund/Data/testdata/akz/10000.txt",
                "/home/lsund/Data/testdata/akz/data.xml"
            );
    if (error) return error;

    mu_message(DATA, "Akz patterns, take 2\n");
    error = compare_vs_naive(
                "/home/lsund/Data/testdata/doctronic/diffsize/12000.txt",
                "/home/lsund/Data/testdata/doctronic/data-diffsize/small.xml");
    if (error) return error;

    mu_message(DATA, "Count: Akz patterns\n");
    error = test_count(
                "/home/lsund/Data/testdata/akz/10000.txt",
                "/home/lsund/Data/testdata/akz/data.xml",
                174
            );
    if (error) return error;

    mu_message(DATA, "Count: Akz patterns, take 2\n");
    error = test_count(
                "/home/lsund/Data/testdata/doctronic/diffsize/12000.txt",
                "/home/lsund/Data/testdata/doctronic/data-diffsize/small.xml",
                170
            );

    if (error) return error;
    return NULL;
}

char *test_search()
{
    mu_run_utest(utest_search);

    /* mu_run_utest(utest_leaves); */

    return NULL;
}

