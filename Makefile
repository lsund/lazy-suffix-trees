#
# Copyright by Stefan Kurtz (C) 1999
# =====================================
# You may use, copy and distribute this file freely as long as you
#  - do not change the file,
#  - leave this copyright notice in the file,
#  - do not make any profit with the distribution of this file
#  - give credit where credit is due
# You are not allowed to copy or distribute this file otherwise
# The commercial usage and distribution of this file is prohibited
# Please report bugs and suggestions to <kurtz@zbh.uni-hamburg.de>
#

CC=gcc
LD=${CC}

# use the following line if you want to compile the source for 64 bit
# architectures

mode=64bit

# use the following line if you want to compile the source for 32 bits
# architectures

# mode=32bit

 CFLAGS+=-m64 -DSIXTYFOURBITS
 LDFLAGS+=-m64

# CFLAGS+=-O3 -Wall -Werror -DNOSPACEBOOKKEEPING
CFLAGS+=-O3 -Wall -DNOSPACEBOOKKEEPING

# SPLINTFLAGS=-DDEBUG -f Splintoptions
SPLINTFLAGS=-DDEBUG

OBJ=wotd.o\
    reverse.o\
    readfile.o\
    searchpat.o\
    clock.o\
    filehandle.o\
    seterror.o\
    getAlpha.o

OBJDBG=wotd.dbg.o\
       reverse.dbg.o\
       readfile.dbg.o\
       searchpat.dbg.o\
       clock.dbg.o\
       filehandle.dbg.o\
       seterror.dbg.o\
       getAlpha.dbg.o\
       debug.dbg.o

all:wotd.splint wotd.x wotd.dbg.x

wotd.x:${OBJ}
	${LD} ${LDFLAGS} ${OBJ} -o $@

wotd.dbg.x:${OBJDBG}
	${LD} ${LDFLAGS} ${OBJDBG} -o $@

clean:
	rm -f *.[ox] wotd.splint wotd.tex wotd.log wotd.pdf wotd.aux

cleansrc:
	rm -f *.h `ls *.c | grep -v wotd.c`

wotd.splint:wotd.c
	splint ${SPLINTFLAGS} wotd.c
	touch wotd.splint

wotd.pdf:wotd.c
	c2lit.x wotd.c | lit2alt.x -C > wotd.tex
	pdflatex wotd.tex
	pdflatex wotd.tex

%.dbg.o:%.c
	$(CC) $(CFLAGS) -DDEBUG -c $*.c -o $@

