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
    Wchar *patterns[20];
    init();

    Uint numbers[100][20];
    Uint lens[20];

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

    patterns[4]   = L"12";
    numbers[4][0] = 0;
    numbers[4][1] = 4;
    numbers[4][2] = 7;
    lens[4]       = 3;

    patterns[5]   = L"121";
    numbers[5][0] = 0;
    lens[5]       = 1;

    patterns[6]   = L"122";
    numbers[6][0] = 4;
    numbers[6][1] = 7;
    lens[6]       = 2;


    patterns[7]   = L"1221";
    numbers[7][0] = 4;
    lens[7]       = 1;


    patterns[8]   = L"1222";
    numbers[8][0] = 7;
    lens[8]       = 1;

    patterns[9]   = L"2";
    numbers[9][0] = 1;
    numbers[9][1] = 6;
    numbers[9][2] = 10;
    numbers[9][3] = 5;
    numbers[9][4] = 9;
    numbers[9][5] = 8;
    lens[9]       = 6;

    patterns[10]   = L"21";
    numbers[10][0] = 1;
    numbers[10][1] = 6;
    numbers[10][2] = 10;
    lens[10]       = 3;

    patterns[11]   = L"211";
    numbers[11][0] = 1;
    lens[11]       = 1;

    patterns[12]   = L"212";
    numbers[12][0] = 6;
    lens[12]       = 1;

    patterns[13]   = L"21$";
    numbers[13][0] = 10;
    lens[13]       = 1;

    patterns[14]   = L"22";
    numbers[14][0] = 5;
    numbers[14][1] = 9;
    numbers[14][2] = 8;
    lens[14]       = 3;

    patterns[15]   = L"221";
    numbers[15][0] = 5;
    numbers[15][1] = 9;
    lens[15]       = 2;

    patterns[16]   = L"2212";
    numbers[16][0] = 5;
    lens[16]       = 1;

    patterns[17]   = L"221$";
    numbers[17][0] = 9;
    lens[17]       = 1;

    patterns[18]   = L"222";
    numbers[18][0] = 8;
    lens[18]       = 1;

    for (int i = 0; i < 19; i++) {
        evaluated = false;
        Wchar *current_pattern = patterns[i];
        Uint patternlen = strlenw(current_pattern);
        printf("pattern: %ls\n", current_pattern);
        find_startindices(current_pattern, patternlen);
        mu_assert("Should contain correct number.",
                contains(numbers[i], lens[i], leaf_nums));
        /* printf("%d %lu %lu\n", i, lens[i], n_leafnums); */
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

