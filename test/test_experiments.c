#include "test.h"

static void print_staticstics(FILE *out, const char *textfile, const char *patternfile)
{
    fprintf(out, "%s\n%s\n\n", textfile, patternfile);
    printtime(out);
    Uint vertex = (st.vs.nxt - st.vs.fst) * 8;
    Uint suffixes = text.len * 8;
    Uint sbsize = sb.size * 8;
    Uint sbgroupsize = MAX_CHARS * 8;
    Uint txt = text.len * sizeof(Wchar);
    Uint chars = MAX_CHARS * sizeof(Wchar);
    double tot =
        (vertex + suffixes + sbsize + sbgroupsize + txt + chars + chars);
    fprintf(out, "st.vs.fst:        %.2f Mb\n", MEGABYTES(vertex));
    fprintf(out, "sb.size:          %.2f Mb\n", MEGABYTES(sbsize));
    fprintf(out, "sb.groupsize:     %.2f Mb\n", MEGABYTES(sbgroupsize));
    fprintf(out, "text.fst:         %.2f Mb\n", MEGABYTES(txt));
    fprintf(out, "text.ss:          %.2f Mb\n", MEGABYTES(suffixes));
    fprintf(out, "st.cs:            %.2f Mb\n", MEGABYTES(chars));
    fprintf(out, "st.rs:            %.2f Mb\n", MEGABYTES(chars));
    fprintf(out, "filesize:         %.2f Mb\n", MEGABYTES(text.len));
    fprintf(out, "tot allocated:    %.2f Mb\n", MEGABYTES(tot));
    fprintf(out, "space/byte        %.2f\n\n", tot / text.len);
}

char *log_runtime(FILE *out, char *textfile, char *patternfile)
{
    file_to_string(textfile);
    Wchar **patterns = (Wchar **) malloc(sizeof(Wchar *) * MAX_PATTERNS);
    Uint npatterns  = file_to_strings(patternfile, MAX_PATTERNS, &patterns);
    STree st;

    initclock();
    init();

    for (Uint j = 0; j < npatterns; j++) {

        Wchar *current_pattern = patterns[j];
        find_pattern(current_pattern, wcslen(current_pattern));

    }
    print_staticstics(out, textfile, patternfile);

    for (int i = npatterns; i >= 0; i--) {
        free(patterns[i]);
    }

    free(patterns);
    destroy(&st);
    free(text.fst);
    return NULL;
}

char *test_experiments()
{
    setlocale(LC_ALL, "en_US.utf8");

    char *tfiles[10];
    char *pfiles[10];

    pfiles[0] = "/home/lsund/Data/testdata/members/patterns-diffsize/p20.txt";

    tfiles[0] = "/home/lsund/Data/testdata/members/diffsize/001.txt";
    tfiles[1] = "/home/lsund/Data/testdata/members/diffsize/002.txt";
    tfiles[2] = "/home/lsund/Data/testdata/members/diffsize/004.txt";
    tfiles[3] = "/home/lsund/Data/testdata/members/diffsize/008.txt";
    tfiles[4] = "/home/lsund/Data/testdata/members/diffsize/016.txt";
    tfiles[5] = "/home/lsund/Data/testdata/members/diffsize/032.txt";
    tfiles[6] = "/home/lsund/Data/testdata/members/diffsize/064.txt";
    tfiles[7] = "/home/lsund/Data/testdata/members/diffsize/128.txt";
    /* tfiles[8] = "/home/lsund/Data/testdata/members/diffsize/001.txt"; */
    /* tfiles[9] = "/home/lsund/Data/testdata/members/diffsize/001.txt"; */

    const char *outfile = "data/experiments.txt";
    FILE *out = fopen(outfile, "w");
    for (int i = 0; i < 6; i++) {
        log_runtime(out, tfiles[i], pfiles[0]);
    }
    fclose(out);

    return NULL;
}
