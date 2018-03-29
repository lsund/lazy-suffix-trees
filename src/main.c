#include "wotd.h"

extern Uchar *text;
extern Uint textlen;

int main(int argc,char *argv[])
{
    char *filename, *patternfile;
    BOOL evaleager;
    Uint patternslen;

    CHECKARGNUM(4,"(-lazy|-eager) filename patternfile");
    DEBUGLEVELSET;

    if (strcmp(argv[1],"-lazy") != 0 && strcmp(argv[1],"-eager") != 0) {
        fprintf(stderr,"Illegal option \"%s\"\n",argv[1]);
        exit(EXIT_FAILURE);
    }
    if (strcmp(argv[1],"-eager") == 0) {
        evaleager = True;
    } else {
        evaleager = False;
    }

    filename = argv[2];
    text = (Uchar *) file2String(filename, &textlen);

    if(text == NULL) {
        fprintf(stderr, "%s: Cannot open file %s\n", argv[0], filename);
        exit(EXIT_FAILURE);
    }

    patternfile = argv[3];
    int size = 128;
    char **patterns = (char **) malloc(sizeof(char *) * size);
    int i = file2Array(patternfile, &patternslen, size, &patterns);

    if(textlen > MAXTEXTLEN) {
        fprintf(stderr,"Textlen = %lu > maximal textlen = %lu\n",
                (Showuint) textlen,(Showuint) MAXTEXTLEN);

        exit(EXIT_FAILURE);
    }

    /* int minpat = 5; */
    /* int maxpat = 1000; */
    /* for (int k = 1; k < 20; k++) { */
    /*     int trials = k * 10000; */
    /*     wotd_benchmark(evaleager, trials, minpat, maxpat); */
    /* } */

    wotd(evaleager, i, &patterns);

    freetextspace(text, textlen);

    for (;i>=0;i--) {
        free(patterns[i]);
    }
    free(patterns);

    return EXIT_SUCCESS;
}
