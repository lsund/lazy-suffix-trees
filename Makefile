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

testrun: clean all
	./bin/wotd ./test-data/members.xml ./test-data/rand-patterns.txt

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
