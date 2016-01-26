#COMPILER=g++
COMPILER=clang++-3.7
CFLAGS=-Wall -Wunused -Wshadow -pedantic -O2 -std=c++11 -g

all: example example2

example: example.cpp bst.h bst_imp.h
	$(COMPILER) $(CFLAGS) -o $@ example.cpp

example2: sample_bst_usage.cpp bst.h bst_imp.h
	$(COMPILER) $(CFLAGS) -o $@ sample_bst_usage.cpp

clean:
	rm -f example
