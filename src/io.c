#include "io.h"

static int open_file(const char *name, Uint *textlen, bool writefile)
{
    struct stat buf;
    int fd = open(name,(writefile) ? O_RDWR : O_RDONLY);

    if (fd == -1) {
        fprintf(stderr, "fileOpen: Cannot open \"%s\"", name);
        return EXIT_FAILURE;
    }

    int statres = fstat(fd,&buf);
    if (statres == -1) {
        fprintf(stderr, "fstat(fd = %d) failed", fd);
        return EXIT_FAILURE;
    }

    *textlen = (Uint) buf.st_size;
    return fd;
}


///////////////////////////////////////////////////////////////////////////////
// Public API


void text_initialize(const char *filename)
{
    FILE *in = fopen(filename, "r");
    int fd = fileno(in);

    struct stat buf;
    fstat(fd, &buf);

    text.len = (Uint) buf.st_size;
    text.fst = malloc(sizeof(Wchar) * (text.len + 1));

    if(text.fst == NULL) {
        fprintf(stderr,"Cannot open file %s\n", filename);
        exit(EXIT_FAILURE);
    }

    Uint c;
    text.len = 0;
    while ((c = fgetwc(in)) != WEOF) {
        text.fst[text.len] = c;
        text.len++;
    }
    text.fst[text.len] = '\0';
    text.lst = text.fst + text.len - 1;
    fclose(in);

    if(text.len == 0) {
        fprintf(stderr,"file \"%s\" is empty\n", filename);
        exit(EXIT_FAILURE);
    }

}


Uint patterns_initialize(char *name, Uint nlines, Wchar ***patterns)
{
    Wchar **words = *patterns;
    Uint len;
    int fd = open_file(name, &len, false);

    if (fd < 0) {
        return -1;
    }

    FILE *fp = fopen(name, "r");
    if (fp == NULL) {
        fprintf(stderr,"Error opening file.\n");
        exit(EXIT_FAILURE);
    }

    Uint i;
    for (i = 0; i < nlines; i++) {
        Uint j;

        // Allocate space for the next line
        words[i] = (Wchar *) malloc(MAX_PATTERNLEN * sizeof(Wchar));

        if (words[i] == NULL) {
            fprintf(stderr,"Out of memory (3).\n");
            exit(4);
        }

        wint_t c;
        j = 0;
        do  {
            c = fgetwc(fp);
            if (c == WEOF) {
                *patterns = words;
                fclose(fp);
                return i;
            }
            words[i][j] = c;
            j++;
            if ((int) j > MAX_PATTERNLEN) {
                fprintf(stderr, "Line too long!: %lu\n", j);
                exit(EXIT_FAILURE);
            }
        } while (c != 10);

        words[i][j - 1] = 0;
    }
    fprintf(stderr, "Warning, not all patterns were read\n");
    *patterns = words;
    fclose(fp);
    return i;
}


FILE *truncate_open_append(const char *path)
{
    fclose(fopen(path, "w"));
    return fopen(path, "a");
}


void text_destroy()
{
  (void) munmap((caddr_t) text.fst, (size_t) text.len);
  free(text.fst);
}
