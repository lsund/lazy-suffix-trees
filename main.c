#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <sys/types.h>
#include "include/types.h"
#include "include/debugdef.h"
#include "include/spacedef.h"
#include "include/experim.h"
#include "include/intbits.h"
#include "include/args.h"
#include "include/arraydef.h"
#include "include/fhandledef.h"
#include "include/protodef.h"
#include "include/preprocdef.h"
#include "include/globalvar.h"
#include <string.h>

int main(int argc,char *argv[])
{
    float rho, readfloat;
    Uint minpat, maxpat;
    char *filename, *patternfile;
    BOOL evaleager;
    Scaninteger readint;

    CHECKARGNUM(7,"(-lazy|-eager) rho minpat maxpat filename patternfile");
    DEBUGLEVELSET;
    if(strcmp(argv[1],"-lazy") != 0 && strcmp(argv[1],"-eager") != 0)
    {
        fprintf(stderr,"Illegal option \"%s\"\n",argv[1]);
        exit(EXIT_FAILURE);
    }
    if(strcmp(argv[1],"-eager") == 0)
    {
        evaleager = True;
    } else
    {
        evaleager = False;
    }

    PARSEFLOATARG(argv[2]);
    rho = readfloat;
    PARSEINTARG(argv[3]);
    minpat = (Uint) readint;
    PARSEINTARG(argv[4]);
    maxpat = (Uint) readint;
    filename = argv[5];
    text = (Uchar *) file2String(filename, &textlen);

    if(text == NULL) {
        fprintf(stderr, "%s: Cannot open file %s\n", argv[0], filename);
        exit(EXIT_FAILURE);
    }

    patternfile = argv[6];
    int size = 128;
    char **patterns = (char **) malloc(sizeof(char *) * size);
    int i = file2Array(patternfile, &patternslen, size, &patterns);

    if(textlen > MAXTEXTLEN) {
        fprintf(stderr,"Textlen = %lu > maximal textlen = %lu\n",
                (Showuint) textlen,(Showuint) MAXTEXTLEN);

        exit(EXIT_FAILURE);
    }

    /* wotd_benchmark(evaleager,argv,argc,rho, minpat, maxpat); */

    wotd(evaleager, &patterns, i);

    freetextspace(text,textlen);

    for (;i>=0;i--) {
        free(patterns[i]);
    }
    free(patterns);

    if(evaleager)
    {
        SHOWTIME("wotdeager");
        SHOWSPACE("wotdeager");
    } else
    {
        SHOWTIME("wotdlazy");
        SHOWSPACE("wotdlazy");
    }
    return EXIT_SUCCESS;
}
