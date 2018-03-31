/*
 * Copyright by Stefan Kurtz (C) 1999-2003
 * =====================================
 * You may use, copy and distribute this file freely as long as you
 * - do not change the file,
 * - leave this copyright notice in the file,
 * - do not make any profit with the distribution of this file
 * - give credit where credit is due
 * You are not allowed to copy or distribute this file otherwise
 * The commercial usage and distribution of this file is prohibited
 * Please report bugs and suggestions to <kurtz@zbh.uni-hamburg.de>
 *
 * ======================================
 *
 * Modified by Ludvig Sundström 2018 with permission from Stefan Kurtz
 * For full source control tree, see https://github.com/lsund/wotd
 *
 */


#include "io.h"


// Open file in readmode, return file descriptor. The length of the file is
// stored in `textlen`. If `writefile` is true if the file should also be
// opened for triting.
static int fileOpen(char *name, Uint *textlen, BOOL writefile)
{
    int fd;
    struct stat buf;

    if((fd = open(name,(writefile) ? O_RDWR : O_RDONLY)) == -1) {
        ERROR1("fileOpen: Cannot open \"%s\"",name);
        return -1;
    }
    if(fstat(fd,&buf) == -1) {
        ERROR2("file \"%s\": fstat(fd = %d) failed",name,fd);
        return -2;
    }
    *textlen = (Uint) buf.st_size;
    return fd;
}

// Allocates `len` bytes starting at offset in the file `fd` into memory.
caddr_t fileParts(int fd, Uint offset, Uint len, BOOL writemap)
{
    caddr_t addr;

    addr = (caddr_t) mmap(
                        (caddr_t) 0,
                        (size_t) len,
                        writemap ? (PROT_READ | PROT_WRITE) : PROT_READ,
                        MAP_PRIVATE,fd,(off_t) offset
                    );
    if (addr == MAP_FAILED) {
        ERROR4("fileParts(fd = %d, left = %ld, len = %ld, %s) failed",
                fd,
                (long) offset,
                (long) len,
                writemap ? "writable map" : "readable map");
        return NULL;
    }
    return addr;
}


// Frees the text specified
void freetextspace(Uchar *text, Uint textlen) {
  (void) munmap((caddr_t) text,(size_t) textlen);
}


// Return the pointer to the contents of a file, as a string.
caddr_t file2String(char *name, Uint *textlen)
{
    int fd;

    fd = fileOpen(name, textlen, False);
    if(fd < 0) {
        return NULL;
    }
    return fileParts(fd, 0, *textlen, False);
}

int file2Array(char *name, Uint *textlen, int size, char ***wordsp)
{
    char **words = *wordsp;
    int fd = fileOpen(name, textlen, False);
    if (fd < 0) {
        return -1;
    }
    int max_line_len = 100;

    /* Allocate lines of text */
    if (words==NULL)
    {
        fprintf(stderr,"Out of memory (1).\n");
        exit(1);
    }

    FILE *fp = fopen(name, "r");
    if (fp == NULL)
    {
        fprintf(stderr,"Error opening file.\n");
        exit(EXIT_FAILURE);
    }

    int i;
    for (i = 0; 1; i++)
    {
        int j;

        /* Have we gone over our line allocation? */
        if (i >= size)
        {
            int new_size;

            /* Double our allocation and re-allocate */
            new_size = size * 2;
            words = (char **) realloc(words,sizeof(char*) * new_size);
            if (!words)
            {
                fprintf(stderr,"Out of memory.\n");
                exit(3);
            }
            size = new_size;
        }

        /* Allocate space for the next line */
        words[i] = malloc(max_line_len);

        if (words[i] == NULL)
        {
            fprintf(stderr,"Out of memory (3).\n");
            exit(4);
        }

        if (!fgets(words[i], max_line_len-1, fp)) {
            break;
        }

        /* Get rid of CR or LF at end of line */
        for (j=strlen(words[i])-1;j>=0 && (words[i][j]=='\n' || words[i][j]=='\r');j--) {
            ;
        }
        words[i][j + 1]='\0';
    }

    *wordsp = words;

    fclose(fp);

    return i;
}
