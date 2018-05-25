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
    int maxpatterns = 1000;

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

char *compare_vs_naive_smyth()
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
    Wchar *patterns[30];
    init();

    patterns[0]  = L"1";
    patterns[1]  = L"11";
    patterns[2]  = L"111";
    patterns[3]  = L"112";
    patterns[4]  = L"12";
    patterns[5]  = L"121";
    patterns[6]  = L"122";
    patterns[7]  = L"1221";
    patterns[8]  = L"1222";
    patterns[9]  = L"2";
    patterns[10] = L"21";
    patterns[11] = L"211";
    patterns[12] = L"212";
    patterns[13] = L"21$";
    patterns[14] = L"22";
    patterns[15] = L"221";
    patterns[16] = L"2212";
    patterns[17] = L"221$";
    patterns[18] = L"222";
    patterns[19]  = L"1112";
    /* patterns[19] = L"222"; */
    /* patterns[20] = L"1"; */

    Uint naive_numbers[20];
    for (int i = 0; i < 20; i++) {
        evaluated = false;
        Wchar *current_pattern = patterns[i];
        Uint patternlen = strlenw(current_pattern);
        Uint n_found = naive_find_all(
                current_pattern,
                current_pattern + patternlen,
                naive_numbers);
        find_startindices(current_pattern, patternlen);
        mu_assert("Should find correct number of leaves.",
                n_found == n_leafnums);
        mu_assert("Should contain correct number.",
                contains(naive_numbers, n_found, leaf_nums));
        free(leaf_nums);
    }
    return NULL;
}

char *compare_vs_naive_leaves(char *patternfile, char *textfile)
{
    setlocale(LC_ALL, "en_US.utf8");
    FILE *in = fopen(textfile, "r");
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
    init();

    Uint naive_numbers[20];

    Uint patternslen;
    Wchar **patterns = (Wchar **) malloc(sizeof(Wchar *) * MAX_PATTERNS);
    int npatterns  = file_to_strings(patternfile, &patternslen, MAX_PATTERNS, &patterns);
    Wchar *current_pattern = patterns[0];
    Uint patternlen = strlenw(current_pattern);
    Uint n_found = naive_find_all(
            current_pattern,
            current_pattern + patternlen,
            naive_numbers);
    find_startindices(current_pattern, patternlen);
    printf("N leaf nums: %lu\n", n_leafnums);
    for (Uint i = 0; i < n_leafnums; i++) {
        printf("Found in tree: %lu\n", leaf_nums[i]);
    }
    mu_assert("Should contain correct number.",
            contains(naive_numbers, n_found, leaf_nums));

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
    char *error;
    error = compare_vs_naive_leaves(
                "/home/lsund/Data/testdata/members/random-patterns.txt",
                "/home/lsund/Data/testdata/members/diffsize/005.txt"
            );
    if (error) return error;

    error = compare_vs_naive_smyth();
    if (error) return error;

    return NULL;
}

char *test_search()
{
    /* mu_run_utest(utest_search); */

    mu_run_utest(utest_leaves);

    return NULL;
}

