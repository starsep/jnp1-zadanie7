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

template<typename T>
template<typename Iter>
BST<T>::BST(Iter, Iter) {
	//TODO
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
	const NodePtr &l = m_root->m_left;
	if (l == nullptr) {
		return BST<T>();
	}
	return BST<T>(l->m_value, l->m_left, l->m_right);
}

template<typename T>
BST<T> BST<T>::right() const {
	if (empty()) {
		throw std::logic_error("BST::right(): Empty BST");
	}
	const NodePtr &r = m_root->m_right;
	if (r == nullptr) {
		return BST<T>();
	}
	return BST<T>(r->m_value, r->m_left, r->m_right);
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
	return left().empty() ? value() : left().min();
}

template<typename T>
T const &BST<T>::max() const {
	if (empty()) {
		throw std::logic_error("BST::max(): Empty BST");
	}
	return right().empty() ? value() : right().max();
}

template<typename T>
template<typename Acc, typename Functor>
Acc BST<T>::fold(Acc a, Functor f) const {
	//TODO
	return *this;
}

template<typename T>
BST<T> BST<T>::find(T const &t) const {
	if (empty()) {
		return BST<T>();
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
	//TODO
	return tree;
}

template<typename T>
BST<T> operator+(T, BST<T> tree) {
	//TODO
	return tree;
}

template<typename T>
BST<T> operator+(BST<T> tree, T) {
	//TODO
	return tree;
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
	//TODO
	return tree.value();
}

#endif //BST_IMP_H
