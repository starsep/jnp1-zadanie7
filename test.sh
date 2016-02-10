#!/bin/bash
for i in construction_test simple_ops_test spine_test max_diff_test max_diff_double_test validation_test; do g++ -o test -std=c++11 -Wall -DTEST=$i test.cpp; echo $i; ./test ; done
