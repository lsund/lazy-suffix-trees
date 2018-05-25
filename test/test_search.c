#include "test.h"

Wchar *wtext;
Uint textlen, max_codepoint, n_leafnums;

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
    wtext[textlen- 1] = '\0';
    max_codepoint = get_max(wtext, textlen);
    fclose(in);
    Wchar *patterns[14];
    init();

    Uint numbers[100][14];
    Uint lens[14];

    patterns[0] = L"1";
    numbers[0][0] = 2;
    numbers[0][1] = 3;
    numbers[0][2] = 0;
    numbers[0][3] = 4;
    numbers[0][4] = 7;
    numbers[0][5] = 11;
    lens[0] = 6;

    patterns[1]   = L"11";
    numbers[1][0] = 2;
    numbers[1][1] = 3;
    lens[1]       = 2;

    patterns[2]   = L"111";
    numbers[2][0] = 2;
    lens[2]       = 1;

    patterns[3]   = L"112";
    numbers[3][0] = 3;
    lens[3]       = 1;


    /* patterns[1]   = L"12"; */
    /* numbers[1][0] = 2; */
    /* numbers[1][1] = 3; */
    /* lens[1]       = 2; */

    for (int i = 0; i < 4; i++) {
        evaluated = false;
        Wchar *current_pattern = patterns[i];
        Uint patternlen = strlenw(current_pattern);
        find_startindices(current_pattern, patternlen);
        mu_assert("Should contain correct number.",
                contains(numbers[i], lens[i], leaf_nums));
        printf("%d %lu %lu\n", i, lens[i], n_leafnums);
        mu_assert("Should find correct number of leaves.",
                lens[i] == n_leafnums);
        free(leaf_nums);
    }


    return NULL;
}

char *test_search()
{
    /* mu_run_utest(utest_search); */

    mu_run_utest(utest_leaves);

    return NULL;
}

