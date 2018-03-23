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
    obj/reverse.o\
    obj/readfile.o\
    obj/search.o\
    obj/clock.o\
    obj/filehandle.o\
    obj/seterror.o\
    obj/getAlpha.o \
    obj/spaceman.o\
    obj/counting_sort.o\
    obj/lcp.o\
    obj/wotd.o\
	obj/eval.o\
	obj/occurs.o\
	obj/init.o\
    obj/main.o


OBJ2 = obj/treesize.o

# all:wotd.splint wotd wotd.dbg.x
all:dirs wotd

dirs:
	mkdir -p obj bin

wotd: ${OBJ}
	${CC} ${LDFLAGS} ${OBJ} -o bin/$@

remake: clean all
	./bin/wotd -lazy data/data.xml data/10000.txt

clean:
	rm -rf obj bin

obj/%.o:src/%.c
	$(CC) $(CFLAGS) -c src/$*.c -o $@


# wotd.splint:wotd.c
# 	splint ${SPLINTFLAGS} wotd.c
# 	touch wotd.splint

