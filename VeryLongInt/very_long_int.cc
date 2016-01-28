#include "very_long_int.h"
#include <iostream>
#include <algorithm>
#include <cstring>

//static initialization

VeryLongInt VeryLongInt::nan = VeryLongInt();

//private methods

const VeryLongInt &VeryLongInt::getNaN() {
	nan.makeNaN();
	return nan;
}

void VeryLongInt::makeNaN() {
	isNaN = true;
	bitRep.clear();
}

bool VeryLongInt::isZero() const {
	return bitRep.size() == 1 && !bitRep[0];
}

void VeryLongInt::removeLeadingZeroes() {
	while (bitRep.size() > 1 && !bitRep.back()) {
		bitRep.pop_back();
	}
}

void VeryLongInt::divide(const VeryLongInt &number, VeryLongInt &quotient, VeryLongInt &rest) {
	while (!bitRep.empty()) {
		rest <<= 1;
		rest.bitRep[0] = bitRep.back();
		bitRep.pop_back();
		rest.removeLeadingZeroes();
		quotient <<= 1;
		if (rest >= number) {
			quotient.bitRep[0] = true;
			rest -= number;
		}
		else {
			quotient.bitRep[0] = false;
		}
		quotient.removeLeadingZeroes();
	}
	rest.removeLeadingZeroes();
}

//public methods
//constructors

VeryLongInt::VeryLongInt()
	: isNaN(false) {
	bitRep.push_back(0);
}

VeryLongInt::VeryLongInt(const VeryLongInt &source)
	: bitRep(source.bitRep),
	  isNaN(source.isNaN) {
}

VeryLongInt::VeryLongInt(VeryLongInt &&source)
	: bitRep(std::move(source.bitRep)),
	  isNaN(std::move(source.isNaN)) {
}

VeryLongInt::VeryLongInt(int number) :
	VeryLongInt(static_cast<long long>(number)) {
}

VeryLongInt::VeryLongInt(long number) :
	VeryLongInt(static_cast<long long>(number)) {
}

VeryLongInt::VeryLongInt(long long number) {
	if (number < 0) {
		makeNaN();
		return;
	}
	*this = VeryLongInt(static_cast<unsigned long long>(number));
}

VeryLongInt::VeryLongInt(unsigned number) :
	VeryLongInt(static_cast<unsigned long long>(number)) {
}

VeryLongInt::VeryLongInt(unsigned long number) :
	VeryLongInt(static_cast<unsigned long long>(number)) {
}

VeryLongInt::VeryLongInt(unsigned long long number) {
	isNaN = false;
	for (; number > 0; number /= 2) {
		bitRep.push_back(number % 2);
	}
	if (bitRep.empty()) { //0
		bitRep.push_back(0);
	}
}

VeryLongInt::VeryLongInt(const std::string &number) {
	bool onlyDigits = std::find_if_not(number.begin(), number.end(), [](char c) {
		return std::isdigit(c);
	}) == number.end();
	bool isCorrectNumber = !number.empty() && onlyDigits;

	if (!isCorrectNumber) {
		makeNaN();
		return;
	}
	//założenie: wiodące zera są ok jako argument
	char *copy = new char[number.size() + 1];
	std::strcpy(copy, number.c_str());
	char *endOfCopy = copy + number.size();
	std::for_each(copy, endOfCopy, [](char &c) {
		c -= '0';
	});

	char *lastDigit = copy + number.size() - 1;
	auto nonZero = [](char c) {
		return c != '\0';
	};
	size_t firstNonZeroIndex = std::find_if(copy, copy + number.size(), nonZero) - copy;
	while (firstNonZeroIndex != number.size()) {
		bitRep.push_back(*lastDigit % 2);
		bool rest = 0;
		std::for_each(copy + firstNonZeroIndex, endOfCopy, [&rest](char &c) {
			c += rest * 10;
			rest = c % 2;
			c /= 2;
		});
		firstNonZeroIndex = std::find_if(copy + firstNonZeroIndex, copy + number.size(), nonZero) - copy;
	}
	isNaN = false;
	if (bitRep.empty()) {
		bitRep.push_back(false);
	}
	delete[] copy;
}

VeryLongInt::VeryLongInt(const char *number) {
	if (number == nullptr) {
		makeNaN();
		return;
	}
	*this = VeryLongInt(std::string(number));
}

//const methods

size_t VeryLongInt::numberOfBinaryDigits() const {
	return bitRep.size();
}

bool VeryLongInt::isValid() const {
	return !isNaN;
}

VeryLongInt::operator bool() const {
	return isValid() && !isZero();
}

//operator=

VeryLongInt &VeryLongInt::operator=(const VeryLongInt &source) {
	isNaN = source.isNaN;
	bitRep = source.bitRep;
	return *this;
}

//compound arithmetic operators

VeryLongInt &VeryLongInt::operator+=(const VeryLongInt &number) {
	if (isNaN || number.isNaN) {
		makeNaN();
		return *this;
	}
	const std::vector<bool> &numberRep = number.bitRep;
	bitRep.resize(std::max(bitRep.size(), numberRep.size()) + 1);
	bool rest = false;
	for (size_t i = 0; i < numberRep.size(); i++) {
		char sum = bitRep[i];
		sum += rest;
		sum += numberRep[i];
		bitRep[i] = sum % 2;
		rest = sum / 2;
	}
	for (size_t i = numberRep.size(); rest; i++) {
		if (bitRep[i]) {
			bitRep[i] = false;
		}
		else {
			bitRep[i] = true;
			rest = false;
		}
	}
	removeLeadingZeroes();
	return *this;
}

VeryLongInt &VeryLongInt::operator-=(const VeryLongInt &number) {
	if (isNaN || number.isNaN || number > (*this)) {
		makeNaN();
		return *this;
	}
	const std::vector<bool> &numberRep = number.bitRep;
	for (size_t i = numberRep.size() - 1; i + 1 > 0; i--) {
		if (numberRep[i]) {
			size_t j = i;
			while (!bitRep[j]) {
				bitRep[j] = true;
				j++;
			}
			bitRep[j] = false;
		}
	}
	removeLeadingZeroes();
	return *this;
}

VeryLongInt &VeryLongInt::operator*=(const VeryLongInt &number) {
	if (isNaN || number.isNaN) {
		makeNaN();
		return *this;
	}
	if (isZero()) {
		return *this;
	}
	std::vector<bool> copy = bitRep;
	VeryLongInt numberCopy = number;
	*this = Zero();
	for (bool powerInSum : copy) {
		if (powerInSum) {
			*this += numberCopy;
		}
		numberCopy <<= 1;
	}
	return *this;
}

VeryLongInt &VeryLongInt::operator/=(const VeryLongInt &number) {
	if (isNaN || !number) {
		makeNaN();
		return *this;
	}
	if (isZero()) {
		return *this;
	}
	VeryLongInt ret = VeryLongInt();
	VeryLongInt temp = VeryLongInt();
	divide(number, ret, temp);
	bitRep = ret.bitRep;

	return *this;
}

VeryLongInt &VeryLongInt::operator%=(const VeryLongInt &number) {
	if (isNaN || !number) {
		makeNaN();
		return *this;
	}
	if (isZero()) {
		return *this;
	}
	VeryLongInt ret = VeryLongInt();
	VeryLongInt temp = VeryLongInt();
	divide(number, temp, ret);
	bitRep = ret.bitRep;

	return *this;
}

VeryLongInt &VeryLongInt::operator<<=(unsigned long long number) {
	if (isNaN || isZero()) {
		return *this;
	}
	const std::vector<bool> copy = bitRep;
	bitRep.clear();
	bitRep.resize(copy.size() + number);
	for (size_t i = 0; i < copy.size(); i++) {
		bitRep[number + i] = copy[i];
	}
	return *this;
}

VeryLongInt &VeryLongInt::operator>>=(unsigned long long number) {
	if (isNaN || isZero()) {
		return *this;
	}
	if (number < bitRep.size()) {
		for (size_t i = 0; i + number < bitRep.size(); i++) {
			bitRep[i] = bitRep[i + number];
		}
		bitRep.resize(bitRep.size() - number);
		removeLeadingZeroes();
	}
	else {
		bitRep.resize(1);
		bitRep[0] = false;
	}
	return *this;
}

//friend comparison operators

bool operator==(const VeryLongInt &x, const VeryLongInt &y) {
	if (x.isNaN || y.isNaN || x.bitRep.size() != y.bitRep.size()) {
		return false;
	}
	for (size_t i = 0; i < x.bitRep.size(); i++) {
		if (x.bitRep[i] != y.bitRep[i]) {
			return false;
		}
	}
	return true;
}

bool operator<(const VeryLongInt &x, const VeryLongInt &y) {
	if (x.isNaN || y.isNaN) {
		return false;
	}
	if (x.bitRep.size() != y.bitRep.size()) {
		return x.bitRep.size() < y.bitRep.size();
	}
	for (size_t i = x.bitRep.size() - 1; i + 1 > 0; i--) {
		if (x.bitRep[i] != y.bitRep[i]) {
			return y.bitRep[i];
		}
	}
	return false;
}

//ostream operator

std::ostream &operator<<(std::ostream &ostream, const VeryLongInt &number) {
	if (number.isNaN) {
		ostream << "NaN";
		return ostream;
	}
	if (number.isZero()) {
		ostream << '0';
		return ostream;
	}
	auto multiplyBy2 = [](std::string &decimal) {
		char rest = 0;
		for (size_t i = 0; i < decimal.size(); i++) {
			decimal[i] *= 2;
			decimal[i] += rest;
			rest = decimal[i] / 10;
			decimal[i] -= rest * 10;
		}
		if (rest) {
			decimal += rest;
		}
	};
	auto removeLeadingZeroesFromDecimal = [](std::string &decimal) {
		while (decimal.size() > 1 && decimal.back() == 0) {
			decimal.pop_back();
		}
	};
	auto addToDecimal = [removeLeadingZeroesFromDecimal](std::string &decimal, const std::string &add) {
		decimal.resize(std::max(decimal.size(), add.size()) + 1);
		for (size_t i = 0; i < add.size(); i++) {
			decimal[i] += add[i];
			if (decimal[i] >= 10) {
				decimal[i + 1]++;
				decimal[i] -= 10;
			}
		}
		removeLeadingZeroesFromDecimal(decimal);
	};

	std::string convertedToDecimal;
	std::string powerOf2;
	powerOf2 += char(1);
	for (bool powerInSum : number.bitRep) {
		if (powerInSum) {
			addToDecimal(convertedToDecimal, powerOf2);
		}
		multiplyBy2(powerOf2);
	}
	for (size_t i = 0; i < convertedToDecimal.size(); i++) {
		convertedToDecimal[i] += '0';
	}
	std::reverse(convertedToDecimal.begin(), convertedToDecimal.end());
	ostream << convertedToDecimal;
	return ostream;
}

//"singleton" functions

const VeryLongInt &Zero() {
	static const VeryLongInt zero = VeryLongInt();
	return zero;
}

const VeryLongInt &NaN() {
	return VeryLongInt::getNaN();
}

//arithmetic operators

const VeryLongInt operator+(const VeryLongInt &x, const VeryLongInt &y) {
	return VeryLongInt(x) += y;
}

const VeryLongInt operator-(const VeryLongInt &x, const VeryLongInt &y) {
	return VeryLongInt(x) -= y;
}

const VeryLongInt operator*(const VeryLongInt &x, const VeryLongInt &y) {
	return VeryLongInt(x) *= y;
}

const VeryLongInt operator/(const VeryLongInt &x, const VeryLongInt &y) {
	return VeryLongInt(x) /= y;
}

const VeryLongInt operator%(const VeryLongInt &x, const VeryLongInt &y) {
	return VeryLongInt(x) %= y;
}

const VeryLongInt operator<<(const VeryLongInt &x, unsigned long long y) {
	return VeryLongInt(x) <<= y;
}

const VeryLongInt operator>>(const VeryLongInt &x, unsigned long long y) {
	return VeryLongInt(x) >>= y;
}

//comparison operators

bool operator!=(const VeryLongInt &x, const VeryLongInt &y) {
	if (!x.isValid() || !y.isValid()) {
		return false;
	}
	return !(x == y);
}

bool operator<=(const VeryLongInt &x, const VeryLongInt &y) {
	if (!x.isValid() || !y.isValid()) {
		return false;
	}
	return (x < y) || (x == y);
}

bool operator>(const VeryLongInt &x, const VeryLongInt &y) {
	if (!x.isValid() || !y.isValid()) {
		return false;
	}
	return !(x <= y);
}

bool operator>=(const VeryLongInt &x, const VeryLongInt &y) {
	if (!x.isValid() || !y.isValid()) {
		return false;
	}
	return !(x < y);
}
