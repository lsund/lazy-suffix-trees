#include "test.h"

Wchar *wtext;
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
    wtext = malloc(sizeof(Wchar) * MAXTEXTLEN);
    wint_t c;
    textlen = 0;
    while ((c = fgetwc(in)) != WEOF) {
        wtext[textlen] = c;
        textlen++;
    }
    wtext[textlen + 1] = '\0';
    max_codepoint = get_max(wtext, textlen);
    fclose(in);
    Wchar **patterns = (Wchar **) malloc(sizeof(Wchar *) * MAX_PATTERNS);
    int npatterns  = file_to_strings(patternfile, &patternslen, MAX_PATTERNS, &patterns);
    init();

    Uint exists_n = 0;
    for (Uint j = 0; j < (Uint) npatterns; j++) {

        Wchar *current_pattern = patterns[j];
        Uint patternlen = strlenw(current_pattern);

        bool exists = find_pattern(current_pattern, patternlen);

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
    int maxpatterns = 100;

    Uint patternslen;
    setlocale(LC_ALL, "en_US.utf8");
    FILE *in = fopen(textfile, "r");
    wtext = malloc(sizeof(Wchar) * MAXTEXTLEN);
    wint_t c;
    textlen = 0;
    while ((c = fgetwc(in)) != WEOF) {
        wtext[textlen] = c;
        textlen++;
    }
    wtext[textlen + 1] = '\0';
    max_codepoint = get_max(wtext, textlen);
    fclose(in);
    Wchar **patterns = (Wchar **) malloc(sizeof(Wchar *) * MAX_PATTERNS);
    int npatterns  = file_to_strings(patternfile, &patternslen, MAX_PATTERNS, &patterns);
    init();

    int exists_n = 0, rexists_n = 0;
    for (Uint j = 0; j < min(npatterns, maxpatterns); j++) {

        Wchar *current_pattern = patterns[j];
        Uint patternlen = strlenw(current_pattern);

        Wchar *end = current_pattern + patternlen;

        bool exists = find_pattern(current_pattern, patternlen);
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

void *utest_leaves()
{
    setlocale(LC_ALL, "en_US.utf8");
    FILE *in = fopen("data/mini/smyth.txt", "r");
    wtext = malloc(sizeof(Wchar) * MAXTEXTLEN);
    wint_t c;
    textlen = 0;
    while ((c = fgetwc(in)) != WEOF) {
        wtext[textlen] = c;
        textlen++;
    }
    wtext[textlen + 1] = '\0';
    max_codepoint = get_max(wtext, textlen);
    fclose(in);
    Wchar *patterns[14];
    init();

    Sint numbers[14];

    // Leaf patterns
    /* patterns[0] = L"111"; */
    /* numbers[0]  = 2; */
    /* patterns[1] = L"112"; */
    /* numbers[1]  = 3; */
    /* patterns[2] = L"121"; */
    /* numbers[2]  = 0; */
    /* patterns[3] = L"1221"; */
    /* numbers[3]  = 4; */
    /* patterns[4] = L"1222"; */
    /* numbers[4]  = 7; */
    /* patterns[5] = L"1$"; */
    /* numbers[5]  = 11; */
    /* patterns[6] = L"211"; */
    /* numbers[6]  = 1; */
    /* patterns[7] = L"212"; */
    /* numbers[7]  = 6; */
    /* patterns[8] = L"21$"; */
    /* numbers[8]  = 10; */
    /* patterns[9] = L"2212"; */
    /* numbers[9]  = 5; */
    /* patterns[10] = L"221$"; */
    /* numbers[10]  = 9; */
    /* patterns[11] = L"222"; */
    /* numbers[11]  = 8; */

    // Inner patterns
    patterns[12] = L"22";
    numbers[12]  = 0;
    Wchar *current_pattern = patterns[12];
    Uint patternlen = strlenw(current_pattern);
    Sint num = find_startindices(current_pattern, patternlen);
    mu_assert("Should have correct number.", num == numbers[12]);

    /* for (int i = 0; i < 13; i++) { */
    /*     Wchar *current_pattern = patterns[i]; */
    /*     Uint patternlen = strlenw(current_pattern); */
    /*     Sint num = find_startindices(current_pattern, patternlen); */
    /*     mu_assert("Should have correct number.", num == numbers[i]); */
    /* } */

    return NULL;
}

char *test_search()
{
    /* mu_run_utest(utest_search); */

    mu_run_utest(utest_leaves);

    return NULL;
}

