CCFLAGS = -Wall -Wextra -Wvla -Werror -g -lm -std=c99

.PHONY: all, clean

all: libhashmap.a libhashmap_tests.a
clean:
	rm *.o *.a

libhashmap.a: hashmap.o
	ar rcs $@ $^

libhashmap_tests.a: test_suite.o
	ar rcs $@ $^

test_suite.o: test_suite.c test_suite.h hashmap.o hash_funcs.h test_pairs.h
	gcc $(CCFLAGS) -c $<

hashmap.o: hashmap.c hashmap.h hash_funcs.h vector.o pair.o
	gcc $(CCFLAGS) -c $<

vector.o: vector.c vector.h
	gcc $(CCFLAGS) -c $<

pair.o: pair.c pair.h
	gcc $(CCFLAGS) -c $<