//autor Filip Czaplicki
//zadanie zaliczeniowe nr 7 dla studentow JNP1 2015/2016

#ifndef BST_IMP_H
#define BST_IMP_H

#include "bst.h"

template<typename T>
BST<T>::BST(std::initializer_list <T>) {
	//TODO
}

template<typename T>
template<typename Iter>
BST<T>::BST(Iter, Iter) {
	//TODO
}

template<typename T>
BST<T>::BST(T value, BST<T> left, BST<T> right) {
	//TODO
}

template<typename T>
BST<T> BST<T>::left() const {
	//TODO
	return *this;
}

template<typename T>
BST<T> BST<T>::right() const {
	//TODO
	return *this;
}

template<typename T>
T const &BST<T>::value() const {
	//TODO
	return m_root->m_value;
}

template<typename T>
bool BST<T>::empty() const {
	//TODO
	return true;
}

template<typename T>
T const &BST<T>::min() const {
	//TODO
	return m_root->m_value;
}

template<typename T>
T const &BST<T>::max() const {
	//TODO
	return m_root->m_value;
}

template<typename T>
template<typename Acc, typename Functor>
Acc BST<T>::fold(Acc a, Functor f) const {
	//TODO
	return *this;
}

template<typename T>
BST<T> BST<T>::find(T const &t) const {
	//TODO
	return *this;
}

template<typename T>
std::size_t BST<T>::size() const {
	//TODO
	return 0;
}

template<typename T>
std::size_t BST<T>::height() const {
	//TODO
	return 0;
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
std::ostream &operator<<(std::ostream &os, BST<T>) {
	//TODO
	return os;
}

template<typename T>
T max_diff(BST<T> tree) {
	//TODO
	return tree.value();
}

#endif //BST_IMP_H
