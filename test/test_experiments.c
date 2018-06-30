#include "test.h"

char *tfiles[30];
char *pfiles[30];

static void print_staticstics(FILE *out, const char *textfile, const char *patternfile)
{
    printf("%s\n%s\n", textfile, patternfile);
    Uint vertex = (st.vs.nxt - st.vs.fst) * 8;
    Uint suffixes = text.len * 8;
    Uint sbsize = sb.size * 8;
    Uint sbgroupsize = MAX_CHARS * 8;
    Uint txt = text.len * sizeof(Wchar);
    Uint chars = MAX_CHARS * sizeof(Wchar);
    double tot =
        (vertex + suffixes + sbsize + sbgroupsize + txt + chars + chars);
    fprintf(out, "vertices             %lu\n", vertex);
    fprintf(out, "time             %.2f\n", getruntime());
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

    pfiles[ 0] = "/home/lsund/Data/testdata/members/10kpatterns/diffalpha/116";
    pfiles[ 1] = "/home/lsund/Data/testdata/members/10kpatterns/diffalpha/117";
    pfiles[ 2] = "/home/lsund/Data/testdata/members/10kpatterns/diffalpha/118";
    pfiles[ 3] = "/home/lsund/Data/testdata/members/10kpatterns/diffalpha/119";
    pfiles[ 4] = "/home/lsund/Data/testdata/members/10kpatterns/diffalpha/120";
    pfiles[ 5] = "/home/lsund/Data/testdata/members/10kpatterns/diffalpha/121";
    pfiles[ 6] = "/home/lsund/Data/testdata/members/10kpatterns/diffalpha/122";
    pfiles[ 7] = "/home/lsund/Data/testdata/members/10kpatterns/diffalpha/123";
    pfiles[ 8] = "/home/lsund/Data/testdata/members/10kpatterns/diffalpha/124";
    pfiles[ 9] = "/home/lsund/Data/testdata/members/10kpatterns/diffalpha/125";
    pfiles[10] = "/home/lsund/Data/testdata/members/10kpatterns/diffalpha/126";
    pfiles[11] = "/home/lsund/Data/testdata/members/10kpatterns/diffalpha/127";
    pfiles[12] = "/home/lsund/Data/testdata/members/10kpatterns/diffalpha/128";
    pfiles[13] = "/home/lsund/Data/testdata/members/10kpatterns/diffalpha/129";
    pfiles[14] = "/home/lsund/Data/testdata/members/10kpatterns/diffalpha/130";
    pfiles[15] = "/home/lsund/Data/testdata/members/10kpatterns/diffalpha/131";
    pfiles[16] = "/home/lsund/Data/testdata/members/10kpatterns/diffalpha/132";
    pfiles[17] = "/home/lsund/Data/testdata/members/10kpatterns/diffalpha/133";
    pfiles[18] = "/home/lsund/Data/testdata/members/10kpatterns/diffalpha/134";
    pfiles[19] = "/home/lsund/Data/testdata/members/10kpatterns/diffalpha/135";
    pfiles[20] = "/home/lsund/Data/testdata/members/10kpatterns/diffalpha/136";

    tfiles[ 0] = "/home/lsund/Data/testdata/members/diffalpha/members";
    tfiles[ 1] = "/home/lsund/Data/testdata/members/diffalpha/members117";
    tfiles[ 2] = "/home/lsund/Data/testdata/members/diffalpha/members118";
    tfiles[ 3] = "/home/lsund/Data/testdata/members/diffalpha/members119";
    tfiles[ 4] = "/home/lsund/Data/testdata/members/diffalpha/members120";
    tfiles[ 5] = "/home/lsund/Data/testdata/members/diffalpha/members121";
    tfiles[ 6] = "/home/lsund/Data/testdata/members/diffalpha/members122";
    tfiles[ 7] = "/home/lsund/Data/testdata/members/diffalpha/members123";
    tfiles[ 8] = "/home/lsund/Data/testdata/members/diffalpha/members124";
    tfiles[ 9] = "/home/lsund/Data/testdata/members/diffalpha/members125";
    tfiles[10] = "/home/lsund/Data/testdata/members/diffalpha/members126";
    tfiles[11] = "/home/lsund/Data/testdata/members/diffalpha/members127";
    tfiles[12] = "/home/lsund/Data/testdata/members/diffalpha/members128";
    tfiles[13] = "/home/lsund/Data/testdata/members/diffalpha/members129";
    tfiles[14] = "/home/lsund/Data/testdata/members/diffalpha/members130";
    tfiles[15] = "/home/lsund/Data/testdata/members/diffalpha/members131";
    tfiles[16] = "/home/lsund/Data/testdata/members/diffalpha/members132";
    tfiles[17] = "/home/lsund/Data/testdata/members/diffalpha/members133";
    tfiles[18] = "/home/lsund/Data/testdata/members/diffalpha/members134";
    tfiles[19] = "/home/lsund/Data/testdata/members/diffalpha/members135";
    tfiles[20] = "/home/lsund/Data/testdata/members/diffalpha/members136";

    tfiles[0] = "/home/lsund/Data/testdata/members/diffsize/001.txt";
    tfiles[1] = "/home/lsund/Data/testdata/members/diffsize/005.txt";
    tfiles[2] = "/home/lsund/Data/testdata/members/diffsize/010.txt";
    tfiles[3] = "/home/lsund/Data/testdata/members/diffsize/015.txt";
    tfiles[4] = "/home/lsund/Data/testdata/members/diffsize/020.txt";
    tfiles[5] = "/home/lsund/Data/testdata/members/diffsize/025.txt";
    tfiles[6] = "/home/lsund/Data/testdata/members/diffsize/030.txt";
    tfiles[7] = "/home/lsund/Data/testdata/members/diffsize/035.txt";
    tfiles[8] = "/home/lsund/Data/testdata/members/diffsize/040.txt";
    tfiles[9] = "/home/lsund/Data/testdata/members/diffsize/045.txt";
    tfiles[10] = "/home/lsund/Data/testdata/members/diffsize/050.txt";

    tfiles[ 0] = "/home/lsund/Data/testdata/random-10MB/out49";
    tfiles[ 1] = "/home/lsund/Data/testdata/random-10MB/out51";
    tfiles[ 2] = "/home/lsund/Data/testdata/random-10MB/out53";
    tfiles[ 3] = "/home/lsund/Data/testdata/random-10MB/out55";
    tfiles[ 4] = "/home/lsund/Data/testdata/random-10MB/out57";
    tfiles[ 5] = "/home/lsund/Data/testdata/random-10MB/out65";
    tfiles[ 6] = "/home/lsund/Data/testdata/random-10MB/out67";
    tfiles[ 7] = "/home/lsund/Data/testdata/random-10MB/out69";
    tfiles[ 8] = "/home/lsund/Data/testdata/random-10MB/out72";
    tfiles[ 9] = "/home/lsund/Data/testdata/random-10MB/out77";
    tfiles[10] = "/home/lsund/Data/testdata/random-10MB/out79";
    tfiles[11] = "/home/lsund/Data/testdata/random-10MB/out81";
    tfiles[12] = "/home/lsund/Data/testdata/random-10MB/out83";
    tfiles[13] = "/home/lsund/Data/testdata/random-10MB/out85";
    tfiles[14] = "/home/lsund/Data/testdata/random-10MB/out87";
    tfiles[15] = "/home/lsund/Data/testdata/random-10MB/out88";


    pfiles[ 0] = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/100.txt";
    pfiles[ 1] = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/200.txt";
    pfiles[ 2] = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/300.txt";
    pfiles[ 3] = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/400.txt";
    pfiles[ 4] = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/500.txt";
    pfiles[ 5] = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/600.txt";
    pfiles[ 6] = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/700.txt";
    pfiles[ 7] = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/800.txt";
    pfiles[ 8] = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/900.txt";
    pfiles[ 9] = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/1000.txt";
    pfiles[10] = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/1100.txt";
    pfiles[11] = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/1200.txt";
    pfiles[12] = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/1300.txt";
    pfiles[13] = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/1400.txt";
    pfiles[14] = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/1500.txt";
    pfiles[15] = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/1600.txt";
    pfiles[16] = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/1700.txt";
    pfiles[17] = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/1800.txt";
    pfiles[18] = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/1900.txt";
    pfiles[19] = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/2000.txt";
    pfiles[20] = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/2100.txt";
    pfiles[21] = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/2200.txt";
    pfiles[22] = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/2300.txt";
    pfiles[23] = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/2400.txt";
    pfiles[24] = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/2500.txt";
    pfiles[25] = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/2600.txt";
    pfiles[26] = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/2700.txt";
    pfiles[27] = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/2800.txt";
    pfiles[28] = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/2900.txt";
    pfiles[29] = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/3000.txt";


    pfiles[ 0] = "/home/lsund/Data/testdata/random-10MB/patterns/patt49";
    pfiles[ 1] = "/home/lsund/Data/testdata/random-10MB/patterns/patt51";
    pfiles[ 2] = "/home/lsund/Data/testdata/random-10MB/patterns/patt53";
    pfiles[ 3] = "/home/lsund/Data/testdata/random-10MB/patterns/patt55";
    pfiles[ 4] = "/home/lsund/Data/testdata/random-10MB/patterns/patt57";
    pfiles[ 5] = "/home/lsund/Data/testdata/random-10MB/patterns/patt65";
    pfiles[ 6] = "/home/lsund/Data/testdata/random-10MB/patterns/patt67";
    pfiles[ 7] = "/home/lsund/Data/testdata/random-10MB/patterns/patt69";
    pfiles[ 8] = "/home/lsund/Data/testdata/random-10MB/patterns/patt72";
    pfiles[ 9] = "/home/lsund/Data/testdata/random-10MB/patterns/patt77";
    pfiles[10] = "/home/lsund/Data/testdata/random-10MB/patterns/patt79";
    pfiles[11] = "/home/lsund/Data/testdata/random-10MB/patterns/patt81";
    pfiles[12] = "/home/lsund/Data/testdata/random-10MB/patterns/patt83";
    pfiles[13] = "/home/lsund/Data/testdata/random-10MB/patterns/patt85";
    pfiles[14] = "/home/lsund/Data/testdata/random-10MB/patterns/patt87";
    pfiles[15] = "/home/lsund/Data/testdata/random-10MB/patterns/patt88";

    pfiles[0]  = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/10000.txt";
    pfiles[ 1] = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/20000.txt";
    pfiles[ 2] = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/30000.txt";
    pfiles[ 3] = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/40000.txt";
    pfiles[ 4] = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/50000.txt";
    pfiles[ 5] = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/60000.txt";
    pfiles[ 6] = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/70000.txt";
    pfiles[ 7] = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/80000.txt";
    pfiles[ 8] = "/home/lsund/Data/testdata/members/100kpatterns/diffsize/90000.txt";

    pfiles[0] = "/home/lsund/Data/testdata/members/10kpatterns/p5.txt";
    pfiles[1] = "/home/lsund/Data/testdata/members/10kpatterns/p10.txt";
    pfiles[2] = "/home/lsund/Data/testdata/members/10kpatterns/p15.txt";
    pfiles[3] = "/home/lsund/Data/testdata/members/10kpatterns/p20.txt";
    pfiles[4] = "/home/lsund/Data/testdata/members/10kpatterns/p25.txt";
    pfiles[5] = "/home/lsund/Data/testdata/members/10kpatterns/p30.txt";
    pfiles[6] = "/home/lsund/Data/testdata/members/10kpatterns/p35.txt";
    pfiles[7] = "/home/lsund/Data/testdata/members/10kpatterns/p40.txt";
    pfiles[8] = "/home/lsund/Data/testdata/members/10kpatterns/p45.txt";
    pfiles[9] = "/home/lsund/Data/testdata/members/10kpatterns/p50.txt";
    pfiles[10] = "/home/lsund/Data/testdata/members/10kpatterns/p60.txt";
    pfiles[11] = "/home/lsund/Data/testdata/members/10kpatterns/p70.txt";
    pfiles[12] = "/home/lsund/Data/testdata/members/10kpatterns/p80.txt";
    pfiles[13] = "/home/lsund/Data/testdata/members/10kpatterns/p90.txt";

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

    const char *outfile =
        "/home/lsund/Data/testdata/gpsource/members/GK-space.txt";
    FILE *out = fopen(outfile, "w");
    for (int i = 0; i < 21; i++) {
        log_runtime(out, tfiles[i], pfiles[i]);
    }
    fclose(out);

    return NULL;
}

