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
INCLUDE=-I'include'

mode=64bit

CFLAGS+=-m64 -DSIXTYFOURBITS $(INCLUDE)
LDFLAGS+=-m64

# CFLAGS+=-O3 -Wall -Werror -DNOSPACEBOOKKEEPING
CFLAGS+=-O3 -Wall -DNOSPACEBOOKKEEPING

# CFLAGS+=-DDEBUG
CFLAGS+=-g

# SPLINTFLAGS=-DDEBUG -f Splintoptions
SPLINTFLAGS=-DDEBUG

OBJ= obj/boyermoore.o\
	obj/wotd.o\
    obj/reverse.o\
    obj/readfile.o\
    obj/searchpat.o\
    obj/clock.o\
    obj/filehandle.o\
    obj/seterror.o\
    obj/getAlpha.o


OBJ2 = obj/treesize.o

all:wotd.splint wotd.x wotd.dbg.x

dirs:
	mkdir -p obj bin

wotd.x:dirs ${OBJ}
	${CC} ${LDFLAGS} main.c ${OBJ} -o bin/$@

wotd.dbg.x:${OBJDBG}
	${CC} ${LDFLAGS} ${OBJDBG} -o $@

clean:
	rm -f obj/*.o bin/*.x
	rmdir obj bin

wotd.splint:wotd.c
	splint ${SPLINTFLAGS} wotd.c
	touch wotd.splint

obj/%.o:src/%.c
	$(CC) $(CFLAGS) -c src/$*.c -o $@
