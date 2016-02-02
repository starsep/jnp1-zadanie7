//autor Filip Czaplicki
//zadanie zaliczeniowe nr 7 dla studentow JNP1 2015/2016

#ifndef BST_IMP_H
#define BST_IMP_H

#include "bst.h"
#include <exception>

template<typename T>
BST<T>::BST(std::initializer_list<T> list) :
	BST<T>(list.begin(), list.end()) {
}

template <typename Iter>
static BST<Iter> build_iter_tree(Iter begin, Iter end) {
	if (begin == end) {
		return BST<Iter>();
	}
	Iter element = begin;
	begin++;
	return build_iter_tree(begin, end) + element;
}

template<typename T, typename Iter>
static BST<T> build_tree(Iter begin, Iter end) {
	auto add_element_to_tree = [](BST<T> tree, Iter iter) {
		return tree + *iter;
	};
	return build_iter_tree<Iter>(begin, end).fold(BST<T>(), add_element_to_tree);
}

template<typename T>
template<typename Iter>
BST<T>::BST(Iter begin, Iter end) :
	m_root(build_tree<T, Iter>(begin, end).m_root) {
}

template<typename T>
BST<T>::BST(T value, BST<T> left, BST<T> right) :
	m_root(std::make_shared<const Node>(Node(value, left.m_root, right.m_root))) {
}

template<typename T>
BST<T> BST<T>::left() const {
	if (empty()) {
		throw std::logic_error("BST::left(): Empty BST");
	}
	return BST<T>(m_root->m_left);
}

template<typename T>
BST<T> BST<T>::right() const {
	if (empty()) {
		throw std::logic_error("BST::right(): Empty BST");
	}
	return BST<T>(m_root->m_right);
}

template<typename T>
T const &BST<T>::value() const {
	if (empty()) {
		throw std::logic_error("BST::value(): Empty BST");
	}
	return m_root->m_value;
}

template<typename T>
bool BST<T>::empty() const {
	return m_root == nullptr;
}

template<typename T>
T const &BST<T>::min() const {
	if (empty()) {
		throw std::logic_error("BST::min(): Empty BST");
	}
	BST<T> l(left());
	return l.empty() ? value() : l.min();
}

template<typename T>
T const &BST<T>::max() const {
	if (empty()) {
		throw std::logic_error("BST::max(): Empty BST");
	}
	BST<T> r(right());
	return r.empty() ? value() : r.max();
}

template<typename T>
template<typename Acc, typename Functor>
Acc BST<T>::fold(Acc a, Functor f) const {
	if (empty()) {
		return a;
	}
	Acc fold_left = left().fold(a, f);
	Acc acc = f(fold_left, value());
	return right().fold(acc, f);
}

template<typename T>
BST<T> BST<T>::find(T const &t) const {
	if (empty()) {
		throw std::logic_error("BST::find(): Empty BST");
	}
	if (value() == t) {
		return *this;
	}
	return (t < value()) ? left().find(t) : right().find(t);
}

template<typename T>
std::size_t BST<T>::size() const {
	if (empty()) {
		return 0;
	}
	return 1 + left().size() + right().size();
}

template<typename T>
std::size_t BST<T>::height() const {
	if (empty()) {
		return 0;
	}
	return 1 + std::max(left().height(), right().height());
}

template<typename T>
BST<T> spine(BST<T> tree) {
	auto add_element_to_spine = [](BST<T> acc, T element) {
		return BST<T>(element, acc, BST<T>());
	};
	return tree.fold(BST<T>(), add_element_to_spine);
}

template<typename T>
BST<T> operator+(T element, BST<T> tree) {
	return tree + element;
}

template<typename T>
BST<T> operator+(BST<T> tree, T element) {
	if (tree.empty()) {
		return BST<T>(element, BST<T>(), BST<T>());
	}
	if (element <= tree.value()) {
		return BST<T>(tree.value(), tree.left() + element, tree.right());
	}
	return BST<T>(tree.value(), tree.left(), tree.right() + element);
}

template<typename T>
std::ostream &operator<<(std::ostream &os, BST<T> tree) {
	if (tree.empty()) {
		return os;
	}
	os << tree.left();
	os << tree.value() << ' ';
	os << tree.right();
	return os;
}

template<typename T>
T max_diff(BST<T> tree) {
	//faster than checking tree.size() < 2
	if (tree.empty() || (tree.left().empty() && tree.right().empty())) {
		throw std::logic_error("max_diff(): BST.size() < 2");
	}
	auto find_max_diff = [](std::pair<T, T> acc, T t) {
		return std::make_pair(std::max(acc.first, t - acc.second), t);
	};
	T min = tree.min();
	return tree.fold(std::make_pair(min - min, tree.min()), find_max_diff).first;
}

#endif //BST_IMP_H
