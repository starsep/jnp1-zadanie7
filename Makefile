COMPILER=g++
#CFLAGS=-Wall -Wunused -Wshadow -pedantic -std=c++11 -g -fsanitize=address -O3 -fno-omit-frame-pointer -g
#CFLAGS=-Wall -Wunused -Wshadow -pedantic -O0 -std=c++11 -ggdb3
CFLAGS=-Wall -Wunused -Wshadow -pedantic -O3 -std=c++11

all: example1 example2 example3 example4 example5 example6 example7

example%: example%.cpp bst.h bst_imp.h very_long_int.o
	$(COMPILER) $(CFLAGS) -o $@ $@.cpp very_long_int.o

very_long_int.o: VeryLongInt/very_long_int.h VeryLongInt/very_long_int.cc
	$(COMPILER) $(CFLAGS) -c VeryLongInt/very_long_int.cc

clean:
	rm -f example example1 example2 example3 example4 example5 example6 example7 *.o
