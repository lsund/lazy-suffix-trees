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
#
# Modified by Ludvig Sundström 2018 under permission by Stefan Kurtz

CC=gcc
LD=${CC}
INCLUDE=-I'include'

mode=64bit

CFLAGS+=-m64 -DSIXTYFOURBITS $(INCLUDE)

# CFLAGS+=-O3 -Wall

CFLAGS+=-DDEBUG
CFLAGS+=-g

OBJ= obj/naive_search.o\
     obj/clock.o\
     obj/io.o\
     obj/get_alpha.o \
	 obj/match.o\
	 obj/insert.o\
     obj/spaceman.o\
	 obj/pattern.o\
	 obj/util.o\
     obj/sort.o\
     obj/lcp.o\
	 obj/eval.o\
	 obj/search.o\
	 obj/stree.o\
	 obj/init.o


TEST_OBJ = obj/test_search.o obj/test_experiments.o


all: dirs wotd

dirs:
	mkdir -p obj bin

wotd: dirs ${OBJ}
	${CC} ${CFLAGS} ${INCLUDE} ${OBJ} src/main.c -o bin/$@

test: dirs ${OBJ} ${TEST_OBJ}
	${CC} ${CFLAGS} ${INCLUDE} ${OBJ} ${TEST_OBJ} test/test.c -o bin/test

runtest: clean test
	./bin/test

memcheck: clean all
	valgrind -v --leak-check=full --show-leak-kinds=all ./bin/wotd ~/Data/testdata/members/diffsize/005.txt /home/lsund/Data/testdata/members/10kpatterns/p5.txt

clean:
	rm -rf obj bin

obj/%.o:src/%.c
	$(CC) $(CFLAGS) -c src/$*.c -o $@

obj/test_%.o:test/test_%.c
	$(CC) $(CFLAGS) -c test/test_$*.c -o $@
