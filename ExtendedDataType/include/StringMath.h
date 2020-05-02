#pragma once
#include <string>
#include <iostream>

#define NO_POINT 0

typedef unsigned int uint;

class StringMath
{
private:
	std::string m_sLongNumber;

public:
	StringMath();
	StringMath(const std::string& decimalNumber);
	StringMath(const char* decimalNumber);
	StringMath(const StringMath& strMath);

	StringMath& operator=(const StringMath& rhs);
	StringMath operator*(const StringMath& rhs);
	StringMath operator/(const StringMath& divisor);
	StringMath operator+(const StringMath& rhs);
	StringMath operator-(const StringMath& rhs);

	bool operator>(const StringMath& rhs) const;
	bool operator<(const StringMath& rhs) const;
	bool operator>=(const StringMath& rhs) const;
	bool operator<=(const StringMath& rhs) const;
	bool operator==(const StringMath& rhs) const;
	bool operator!=(const StringMath& rhs) const;

	char operator[](long index);
	StringMath abs() const;
	bool isNegative() const;
	bool isPositive() const;

protected:
	bool validData(const std::string& decimalNumber);
	void normalize(std::string& decimalNumber);

	uint getPosPoint() const;
	uint getNumDigitFractional() const;
	uint getNumDigitInt() const;
};
