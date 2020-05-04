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
	StringMath(int number);
	StringMath(long number);
	StringMath(float number);
	StringMath(double number);
	StringMath(const StringMath& strMath);

	StringMath& operator=(const StringMath& rhs);
	StringMath operator*(const StringMath& rhs);
	StringMath div(long divisor, uint precision);
	StringMath operator%(long divisor);
	StringMath operator+(const StringMath& rhs);
	StringMath operator-(const StringMath& rhs);
	StringMath operator-();

	bool operator>(const StringMath& rhs) const;
	bool operator==(const StringMath& rhs) const;
	bool operator!=(const StringMath& rhs) const;

	char operator[](long index);
	StringMath abs() const;
	bool isNegative() const;
	bool isPositive() const;
	StringMath getInt() const;
	StringMath getFraction() const;
	uint getPosPoint() const;
	uint getNumDigitFractional() const;
	uint getNumDigitInt() const;
	std::string to_string() const;

protected:
	bool validData(const std::string& decimalNumber);
	void normalize(std::string& decimalNumber);
};

StringMath pow(const StringMath& decNum, long pow, uint precision);

