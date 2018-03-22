/*
  This file is generated. Do not edit.

  Copyright by Stefan Kurtz (C) 2003
  =====================================
  You may use, copy and distribute this file freely as long as you
   - do not change the file,
   - leave this copyright notice in the file,
   - do not make any profit with the distribution of this file
   - give credit where credit is due
  You are not allowed to copy or distribute this file otherwise
  The commercial usage and distribution of this file is prohibited
  Please report bugs and suggestions to <kurtz@zbh.uni-hamburg.de>

*/

#ifndef PROTODEF_H
#define PROTODEF_H

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "fhandledef.h"

#ifdef __cplusplus
extern "C" {
#endif
void reverseinplace(Uchar *s,Uint len);

void reversecomplement(Uchar *s,Uint len);

void complement(Uchar *s,Uint len);

void reversestring(char *s, Uint m, char *sreverse);

int fileOpen(char *name, Uint *textlen, BOOL writefile);

/*@null@*/ caddr_t fileParts(int fd,Uint offset,Uint len,BOOL writemap);

void freetextspace(Uchar *text, Uint textlen);

/*@null@*/ caddr_t genfile2String(char *name, Uint *textlen,
                                  BOOL writefile, BOOL writemap);

/*@null@*/ caddr_t file2String(char *name, Uint *textlen);

int file2Array(char *name, Uint *textlen, int nlines, char ***words);

Sint getdebuglevel(void);

BOOL getdebugwhere(void);

void setdebuglevel(void);

void setdebuglevelfilename(char *filename);

FILE *getdbgfp(void);

void debugclosefile(void);

void searchpatterngeneric(
       BOOL(*reallyoccurs)(void *,Uchar *,Uint,Uchar *,Uchar *),
       BOOL(*occurs)(void *,Uchar *,Uint,Uchar *,Uchar *),
       /*@unused@*/ char *argv[],
       /*@unused@*/ Argctype argc,
       void *occursinfo,
       Uchar *text,Uint textlen,float trialpercentage,
       Uint minpatternlen,
       Uint maxpatternlen,
       void (*showpattern)(Uchar *,Uint),
       void *showpatterninfo,
       Uchar *pattern);

void searchpattern(BOOL(*occurs)(void *,Uchar *,Uint,Uchar *,Uchar *),
                   char *argv[],Argctype argc,
                   void *occursinfo,Uchar *text,Uint textlen,
                   float trialpercentage,
                   Uint minpatternlen,
                   Uint maxpatternlen,
                   void (*showpattern)(Uchar *,Uint),
                   void *showpatterninfo);

void searchpatternapprox(void(*apm)(void *,Uint,Uchar *,Uint,Uchar *,Uint),
                         /*@unused@*/ char *argv[],
                         /*@unused@*/ Argctype argc,
                         void *occursinfo,
                         float errorrate,
                         Uchar *text,Uint textlen,float trialpercentage,
                         Uint minpatternlen,
                         Uint maxpatternlen);

void search_one_pattern(
    BOOL (*occurs) (void *,Uchar *,Uint,Uchar *,Uchar *),
    void *occursinfo,
    Uchar *text,
    Uint textlen,
    Uint patternlen,
    char *mypattern
);

void initclock(void);

double getruntime(void);

Uint getclockticks(void);

/*@null@*/ FILE *createfilehandle(char *file,
                                  const Uint line,
                                  const char *path,
                                  const char *mode);

Sint maketmpfile(char *file,
                 Uint line,
                 Tmpfiledesc *tmpfiledesc,
                 char *fileprefix);

void wraptmpfiledesc(Tmpfiledesc *tmpfiledesc);

Sint deletefilehandle(char *file,
                      Uint line,
                      FILE *fp);

Sint writetofilehandle(char *file,
                       Uint line,
                       void *ptr,
                       Uint size,
                       Uint nmemb,
                       FILE *stream);

Sint readfromfilehandle(char *file,
                       Uint line,
                       void *ptr,
                       Uint size,
                       Uint nmemb,
                       FILE *stream);

void checkfilehandles(void);

Sint maxerrormsg(void);

char *messagespace(void);

void seterror(Sint code);

Sint geterror(void);

void reseterror(void);

void getAlpha (char *s, char *alpha, Uint *alphaSize);

void getUchars(Uchar *text, Uint textlen, Uchar *alpha, Uint *alphasize);

void printAlpha (char *s);

#ifdef __cplusplus
}
#endif
#endif
