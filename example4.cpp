#include "bst_imp.h"
#include "VeryLongInt/very_long_int.h"
#include <iostream>
#include <vector>
#include <set>
#include <algorithm>
#include <cassert>
#include <sstream>

const int TRIES = 100;
const int N = 1000;
const int seed = 1337; //time(nullptr);

#define check_except(ABC) \
	do { \
		try { \
			ABC; \
		} \
		catch (std::logic_error e) { \
			std::cerr << "what() w " #ABC << ":--- " << e.what() << std::endl; \
			break; \
		} \
		assert(!"No exception"); \
	} while(false);

template<typename T>
std::vector<T> make_data() {
	std::vector<T> data;
	const int begin_int = rand() % N;
	const T begin(begin_int);
	const T end(begin_int + N);
	const T one(1);
	for (T i = begin; i <= end; i += one) {
		data.push_back(i);
	}
	std::random_shuffle(data.begin(), data.end());
	return data;
}

template<typename T>
std::ostream &operator<<(std::ostream &os, std::vector<T> v) {
	return os;
}

template<typename T>
bool operator==(const BST<T> &x, const BST<T> &y) {
	std::stringstream s_x, s_y;
	s_x << x;
	s_y << y;
	return s_x.str() == s_y.str();
}

template<typename T>
bool check_vec_spine(typename std::vector<T>::const_iterator begin, typename std::vector<T>::const_iterator end, const BST<T> &spine) {
	if (begin == end && spine.empty()) {
		return true;
	}
	if (begin == end || spine.empty()) {
		return false;
	}
	auto it = end;
	it--;
	if (*it != spine.value()) {
		return false;
	}
	return check_vec_spine(begin, it, spine.left());
}

template<typename T>
bool operator==(const std::vector<T> &v, const BST<T> &bst) {
	return check_vec_spine(v.begin(), v.end(), spine(bst));
}

template<typename T>
bool operator==(const BST<T> &bst, const std::vector<T> &v) {
	return v == bst;
}


template<typename T>
void check1() {
	BST<T> empty;
	BST<T> bst1({T(rand() % N), T(rand() % N), T(rand() % N), T(rand() % N), T(rand() % N)});
	T x(rand() % N);
	BST<T> bst2(x, empty, bst1);
	assert(bst2.left() == empty);
	assert(bst2.right() == bst1);
	assert(x == bst2.value());
	
	assert(empty.empty());
	assert(!bst1.empty());
	assert(!bst2.empty());
}

template<typename T>
void check_max_diff(const std::vector<T> &sorted, const BST<T> &bst) {
	if (sorted.size() < 2) {
		check_except(max_diff(bst));
		return;
	}
	T result = sorted[1] - sorted[0];
	for (size_t i = 0; i < sorted.size() - 1; i++) {
		result = std::max(result, sorted[i + 1] - sorted[i]);
	}
	assert(max_diff(bst) == result);
}

template<typename T>
void check(std::vector<T> data) {
	std::vector<T> sorted(data.begin(), data.end());
	std::sort(sorted.begin(), sorted.end());
	BST<T> bst(data.begin(), data.end());
	
	check_max_diff(sorted, bst);
	
	assert(bst.size() == data.size());
	if (!data.empty()) {	
		assert(bst.min() == sorted.front());
		assert(bst.max() == sorted.back());
	}
	else {
		check_except(bst.min());
		check_except(bst.max());
		check_except(bst.value());
		check_except(bst.left());
		check_except(bst.right());
	}
	assert(bst == sorted);
	T x(rand() % N);
	assert(bst + x == x + bst);
	std::cout << "Height(): " << bst.height() << '\n';
}

template<typename T>
void test() {
	check<T>(make_data<T>());
	check<T>(std::vector<T>());
	check1<T>();
}

int main() {
	srand(seed);
	for (int i = 0; i < TRIES; i++) {
		test<int>();
		test<double>();
		test<VeryLongInt>();
	}
	return 0;
}

