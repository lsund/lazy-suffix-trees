#include "test.h"

wchar_t *wtext;
Uint textlen, max_codepoint;

static Uint min(const Uint a, const Uint b)
{
    return a < b ? a : b;
}

char *test_count(char *patternfile, char *textfile, Uint count)
{
    Uint patternslen;
    setlocale(LC_ALL, "en_US.utf8");
    FILE *in = fopen(textfile, "r");
    wtext = malloc(sizeof(wchar_t) * MAXTEXTLEN);
    wint_t c;
    textlen = 0;
    while ((c = fgetwc(in)) != WEOF) {
        wtext[textlen] = c;
        textlen++;
    }
    wtext[textlen + 1] = '\0';
    max_codepoint = get_max(wtext, textlen);
    fclose(in);
    wchar_t **patterns = (wchar_t **) malloc(sizeof(wchar_t *) * MAX_PATTERNS);
    int npatterns  = file_to_strings(patternfile, &patternslen, MAX_PATTERNS, &patterns);
    inittree();

    Uint exists_n = 0;
    for (Uint j = 0; j < (Uint) npatterns; j++) {

        wchar_t *current_pattern = patterns[j];
        Uint patternlen = strlenw(current_pattern);

        bool exists = try_search(current_pattern, patternlen);

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
    int maxpatterns = 50;

    Uint patternslen;
    setlocale(LC_ALL, "en_US.utf8");
    FILE *in = fopen(textfile, "r");
    wtext = malloc(sizeof(wchar_t) * MAXTEXTLEN);
    wint_t c;
    textlen = 0;
    while ((c = fgetwc(in)) != WEOF) {
        wtext[textlen] = c;
        textlen++;
    }
    wtext[textlen + 1] = '\0';
    max_codepoint = get_max(wtext, textlen);
    fclose(in);
    wchar_t **patterns = (wchar_t **) malloc(sizeof(wchar_t *) * MAX_PATTERNS);
    int npatterns  = file_to_strings(patternfile, &patternslen, MAX_PATTERNS, &patterns);
    inittree();

    int exists_n = 0, rexists_n = 0;
    for (Uint j = 0; j < min(npatterns, maxpatterns); j++) {

        wchar_t *current_pattern = patterns[j];
        Uint patternlen = strlenw(current_pattern);

        wchar_t *end = current_pattern + patternlen;

        bool exists = try_search(current_pattern, patternlen);
        bool rexists = naive_search(current_pattern, end);

        exists ? exists_n++ : (void) 0;
        rexists ? rexists_n++ : (void) 0;

        if (rexists != exists) {
            printf("%d %d\n", rexists, exists);
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

    mu_message(DATA, "Random existing patterns\n");
    error = compare_vs_naive("data/random-patterns.txt", "data/dataset/005.txt");
    if (error) return error;

    mu_message(DATA, "Random non-existing patterns\n");
    error = compare_vs_naive(
                "data/random-patterns-non-existing.txt",
                "data/dataset/005.txt"
            );
    if (error) return error;

    mu_message(DATA, "Akz patterns\n");
    error = compare_vs_naive("data/10000.txt", "data/data.xml");
    if (error) return error;

    mu_message(DATA, "Count: Akz patterns\n");
    error = test_count("data/10000.txt", "data/data.xml", 174);
    if (error) return error;

    return NULL;
}

char *test_search()
{
    mu_run_utest(utest_search);

    return NULL;
}

