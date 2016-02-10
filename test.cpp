#include <algorithm>
#include <iostream>
#include <sstream>
#include <forward_list>
#include <vector>
#include <random>

#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext.hpp>
#include <boost/range/numeric.hpp>
#include <boost/range/adaptors.hpp>

#include "bst_imp.h"

using namespace boost::adaptors;

class average_accumulator {
    int m_value;
    int m_cnt;

public:
    average_accumulator() :
        m_value(0), m_cnt(0) {}

    void operator+=(bool add) {
        m_value += add ? 1 : 0;
        m_cnt += 1;
    }

    friend std::ostream & operator<<(std::ostream &os, average_accumulator a) {
        os << a.m_value << "/" << a.m_cnt;
        return os;
    }

    int get_points() const {
        assert(m_cnt > 0);
        return m_value / m_cnt;
    }
};

template <typename Test>
int run_test(Test test, std::string const & name) {
    average_accumulator score{};

    try {
        score = test();
    } catch (...) {
        std::cout << name << " test failed with exception" << std::endl;
        return 0;
    }

    std::cout << name << " test score " << score << std::endl;
    return score.get_points();
}


template <typename T>
bool print_equal(std::vector<T> init, BST<T> bst) {
    std::vector<T> data(init);
    boost::sort(data);

    auto val = boost::accumulate(data | transformed(
                [](T t){return std::to_string(t) + " ";}), std::string{});
    val = std::string(val.begin(), val.end() - 1);

    std::stringstream stream_test;

    stream_test << bst;
    auto test = stream_test.str();
    test.erase(test.find_last_not_of(" \n\r\t")+1);

    return test == val;
}

template <typename T>
bool fold_equal(std::vector<T> init, BST<T> bst) {
    std::vector<T> data(init);
    using Iter = decltype(data.begin());
    boost::sort(data);

    return bst.fold(std::make_pair(data.begin(), true),
            [](std::pair<Iter, bool> acc, T t){
                return std::make_pair(acc.first + 1, acc.second && t == *(acc.first));
            }).second;
}

template <typename F>
average_accumulator random_test(F fun) {
    auto const TESTS_NR = 1000;
    std::default_random_engine rng{};
    auto dice = std::bind(std::uniform_int_distribution<int>(0,TESTS_NR), std::ref(rng));

    average_accumulator score{};
    for (int i = 0; i < TESTS_NR; ++i) {
        std::vector<int> v(TESTS_NR);
        boost::generate(v, dice);
        BST<int> bst(v.begin(), v.end());
        score += fun(v, bst);
    };

    return score;
}

//construction + fold + print
//to jest template, żeby kompilowały się rozwiązania nie implementujące pełnego interfejsu
template <template <class> class T>
average_accumulator construction_test() {
    return random_test(
            []( std::vector<int> const & v, T<int> bst){
                return  fold_equal(v, bst) && print_equal(v, bst);
            });
}

//    3       |
//   / \      |
//  1   4     |
//   \   \    |
//    2   7   |
//     \      |
//      3     |
std::forward_list<int> inst1_data() {
    return {3,4,1,2,7,3};
}

//to jest template, żeby kompilowały się rozwiązania nie implementujące pełnego interfejsu
template <template <class> class T>
T<int> inst1() {
    auto data = inst1_data();
    return T<int>(data.begin(), data.end());
}

//      4     |
//     / \    |
//    2   5   |
//   / \ /    |
//  1  4 5    |
// /  /       |
//1  3        |
std::forward_list<int>  inst2_data() {
    return {4, 2, 4, 1, 3, 5 , 1, 5};
}

//to jest template, żeby kompilowały się rozwiązania nie implementujące pełnego interfejsu
template <template <class> class T>
T<int> inst2() {
    auto data = inst2_data();
    return T<int>(data.begin(), data.end());
}

//             4      |
//           /  \     |
//           2   5    |
//         /  \ /     |
//        1   4 5     |
//       /\  /        |
//      1 2 3         |
//     /              |
//    1               |
//   /                |
//  1                 |
// /                  |
// 1                  |
std::forward_list<int>  inst3_data() {
    auto add = {1,1,1,2};
    auto data = inst2_data();
    data.insert_after(data.end(), add.begin(), add.end());
    return data;
}

//to jest template, żeby kompilowały się rozwiązania nie implementujące pełnego interfejsu
template <template <class> class T>
T<int> inst3() {
    return (((1 + (inst2<T>() + 2)) + 1) + 1);
}

//height, max, min, left, right, value, empty, find
//to jest template, żeby kompilowały się rozwiązania nie implementujące pełnego interfejsu
template <template <class> class T>
average_accumulator simple_ops_test() {
    std::default_random_engine rng{};
    auto score =
            random_test(
            [&rng]( std::vector<int> const & v, BST<int> bst){
                std::uniform_int_distribution<int> dist(0,v.size()-1);
                auto x = v[dist(rng)];
                return  bst.find(x).value() == x;
            });

    {
//    3       |
//   / \      |
//  1   4     |
//   \   \    |
//    2   7   |
//     \      |
//      3     |
        auto bst = inst1<T>();
        score += bst.height() == 4;
        score += bst.min() == 1;
        score += bst.max() == 7;
        score += bst.left().value() == 1;
        score += bst.left().right().value() == 2;
        score += bst.left().right().right().value() == 3;
        score += bst.right().value() == 4;
        score += bst.right().right().value() == 7;
        score += !bst.empty();
    }

    {
        auto bst = inst2<T>();
        score += bst.height() == 4;
        score += bst.min() == 1;
        score += bst.max() == 5;
        score += bst.left().value() == 2;
        score += bst.right().value() == 5;
        score += !bst.empty();
    }
    {
        auto bst = inst3<T>();
        score += bst.height() == 7;
        score += bst.min() == 1;
        score += bst.max() == 5;
        score += bst.left().value() == 2;
        score += bst.right().value() == 5;
        score += !bst.empty();
    }
    score += BST<int>{}.empty();

    return score;
}


template <typename T>
bool is_spine(BST<T> t) {
    return t.empty() ? true : t.right().empty() && is_spine(t.left());
}

//size, spine
//to jest template, żeby kompilowały się rozwiązania nie implementujące pełnego interfejsu
template <template <class> class T>
average_accumulator spine_test() {
    return random_test(
            []( std::vector<int> const & v, T<int> bst){
                return  bst.size() == v.size()
                     && fold_equal(v, spine(bst))
                     && is_spine(spine(bst));
            });

}

//max_diff
//to jest template, żeby kompilowały się rozwiązania nie implementujące pełnego interfejsu
template <template <class> class T>
average_accumulator max_diff_test() {
    return random_test(
            []( std::vector<int>  & v, T<int> bst){
                boost::sort(v);
                std::vector<int> diffs(v.size());
                boost::adjacent_difference(v, diffs.begin());

                return  max_diff(bst) == *std::max_element(diffs.begin() + 1, diffs.end());
            });
}

//to jest template, żeby kompilowały się rozwiązania nie implementujące pełnego interfejsu
template <template <class> class T>
average_accumulator max_diff_double_test() {
    average_accumulator score{};
    auto const EPS = 1e-6;
    score += std::abs(max_diff(T<double>({-0.5, 0})) - 0.5) < EPS;
    score += std::abs(max_diff(T<double>({0.1, 0.5})) - 0.4) < EPS;
    score += std::abs(max_diff(T<double>({-0.2, 0.1, 0.5})) - 0.4) < EPS;
    return score;
}

template <typename F>
bool fail(F f) {
    try {
        f();
        return false;
    } catch (std::logic_error const & e) {
        return true;
    } catch (...) {
        return false;
    }
}

//to jest template, żeby kompilowały się rozwiązania nie implementujące pełnego interfejsu
template <template <class> class T>
average_accumulator validation_test() {
    average_accumulator score{};

    score += fail([](){ T<int>{}.value();});
    score += fail([](){ T<int>{}.left();});
    score += fail([](){ T<int>{}.right();});
    score += fail([](){ T<int>{}.min();});
    score += fail([](){ T<int>{}.max();});
    score += fail([](){ max_diff(T<int>{});});
    score += fail([](){ max_diff(T<int>{{1}});});

    //akceptujemy dwie semantyki metody find
    //ale zachowanie musi być spójne
    auto find_failed = fail([](){ T<int>{1,2,3}.find(42);});
    find_failed = fail([](){ T<int>{}.find(42);}) && find_failed;
    score += find_failed || ( T<int>{1,2,3}.find(42).empty()
                          &&  T<int>{}.find(42).empty());

    return score;
}

#define xstr(s) str(s)
#define str(s) #s

int main() {
    //TEST symbol should be defined and euqal to one of :
    // - construction_test
    // - simple_ops_test
    // - spine_test
    // - max_diff_test
    // - max_diff_double_test
    // - validation_test

    //returns 0 if succeded
    return run_test(TEST<BST>, xstr(TEST)) - 1;
}
