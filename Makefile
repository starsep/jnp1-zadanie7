COMPILER=g++
CFLAGS=-Wall -Wunused -Wshadow -pedantic -O2 -std=c++11 -g

all: example

example: sample_bst_usage.cpp bst.h bst_imp.h
	$(COMPILER) $(CFLAGS) -o $@ sample_bst_usage.cpp

clean:
	rm -f example
