#include "wotd.h"

extern Uchar *text;
extern Uint textlen;

int main(int argc,char *argv[])
{
    char *filename, *patternfile;
    BOOL evaleager;
    Uint patternslen;

    CHECKARGNUM(5, "(-lazy|-eager) filename patternfile (bench|run)");
    DEBUGLEVELSET;

    if (strcmp(argv[1], "-lazy") != 0 && strcmp(argv[1],"-eager") != 0) {
        ERROR("Must provide -lazy or -eager as first argument");
    }

    evaleager = strcmp(argv[1], "-eager") == 0;

    filename = argv[2];
    text = (Uchar *) file2String(filename, &textlen);

    if(text == NULL) {
        ERROR("Cannot open file");
    }

    patternfile     = argv[3];
    int size        = 128;
    char **patterns = (char **) malloc(sizeof(char *) * size);
    int i           = file2Array(patternfile, &patternslen, size, &patterns);

    if(textlen > MAXTEXTLEN) {
        ERROR("Text too large, see MAXTEXTLEN");
    }

    const char *path = "data/out.txt";

    if (strcmp(argv[4], "bench") == 0) {

        int minpat = 5;
        int maxpat = 1000;

        run_benchmark(path, evaleager, 5, minpat, maxpat);

    } else {

        run_patterns(path, evaleager, i, &patterns);

    }

    freetextspace(text, textlen);

    for (; i >= 0; i--) {
        free(patterns[i]);
    }
    free(patterns);

    return EXIT_SUCCESS;
}
