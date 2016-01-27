COMPILER=g++
#CFLAGS=-Wall -Wunused -Wshadow -pedantic -std=c++11 -g -fsanitize=address -O3 -fno-omit-frame-pointer -g
#CFLAGS=-Wall -Wunused -Wshadow -pedantic -O0 -std=c++11 -ggdb3
CFLAGS=-Wall -Wunused -Wshadow -pedantic -O3 -std=c++11 -g

all: example1 example2 example3

example%: example%.cpp bst.h bst_imp.h
	echo $(CFLAGS)
	$(COMPILER) $(CFLAGS) -o $@ $@.cpp

clean:
	rm -f example example1 example2 example3
