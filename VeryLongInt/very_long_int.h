#ifndef VERY_LONG_INT_H
#define VERY_LONG_INT_H

#include <cstddef>
#include <ostream>
#include <string>
#include <vector>

class VeryLongInt {
private:
	//private variables
	static VeryLongInt nan;
	std::vector<bool> bitRep;
	bool isNaN;

	//private methods
	static const VeryLongInt & getNaN();
	void makeNaN();
	bool isZero() const;
	void removeLeadingZeroes();
	void divide(const VeryLongInt &, VeryLongInt &, VeryLongInt &);
public:
	//constructors
	VeryLongInt();
	VeryLongInt(const VeryLongInt &);
	VeryLongInt(VeryLongInt &&);
	VeryLongInt(int);
	VeryLongInt(long);
	VeryLongInt(long long);
	VeryLongInt(unsigned);
	VeryLongInt(unsigned long);
	VeryLongInt(unsigned long long);
	explicit VeryLongInt(const std::string &);
	explicit VeryLongInt(const char *);
	explicit VeryLongInt(bool) = delete;
	explicit VeryLongInt(char) = delete;
	explicit VeryLongInt(char16_t) = delete;
	explicit VeryLongInt(char32_t) = delete;
	explicit VeryLongInt(double) = delete;
	explicit VeryLongInt(float) = delete;
	explicit VeryLongInt(long double) = delete;

	//const methods
	size_t numberOfBinaryDigits() const;
	bool isValid() const;
	explicit operator bool() const;

	//operator=
	VeryLongInt & operator=(const VeryLongInt&);

	//compound arithmetic operators
	VeryLongInt & operator+=(const VeryLongInt &);
	VeryLongInt & operator-=(const VeryLongInt &);
	VeryLongInt & operator*=(const VeryLongInt &);
	VeryLongInt & operator/=(const VeryLongInt &);
	VeryLongInt & operator%=(const VeryLongInt &);
	VeryLongInt & operator<<=(unsigned long long);
	VeryLongInt & operator>>=(unsigned long long);

	//friend comparison operators
	friend bool operator==(const VeryLongInt &, const VeryLongInt &);
	friend bool operator<(const VeryLongInt &, const VeryLongInt &);

	//ostream operator
	friend std::ostream & operator<<(std::ostream &, const VeryLongInt &);

	//"singleton" functions
	friend const VeryLongInt & Zero();
	friend const VeryLongInt & NaN();

	//default destructor
	~VeryLongInt() = default;
};

//"singleton" functions
const VeryLongInt & Zero();
const VeryLongInt & NaN();

//arithmetic operators
const VeryLongInt operator+(const VeryLongInt &, const VeryLongInt &);
const VeryLongInt operator-(const VeryLongInt &, const VeryLongInt &);
const VeryLongInt operator*(const VeryLongInt &, const VeryLongInt &);
const VeryLongInt operator/(const VeryLongInt &, const VeryLongInt &);
const VeryLongInt operator%(const VeryLongInt &, const VeryLongInt &);
const VeryLongInt operator<<(const VeryLongInt &, unsigned long long);
const VeryLongInt operator>>(const VeryLongInt &, unsigned long long);

//comparison operators
bool operator!=(const VeryLongInt &, const VeryLongInt &);
bool operator<=(const VeryLongInt &, const VeryLongInt &);
bool operator>(const VeryLongInt &, const VeryLongInt &);
bool operator>=(const VeryLongInt &, const VeryLongInt &);

#endif //VERY_LONG_INT_H
