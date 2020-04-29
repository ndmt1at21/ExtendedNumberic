#pragma once
#include <string>
#include <iostream>

typedef unsigned int uint;

class StringMath
{
private:
	std::string m_sLongNumber;

public:
	StringMath();
	StringMath(const std::string& longNumber);
	StringMath(const char* longNumber);
	StringMath(const StringMath& strMath);

	StringMath& operator=(const StringMath& rhs);
	StringMath operator*(const StringMath& rhs);
	StringMath operator/(int divisor);
	StringMath operator+(const StringMath& rhs);
	StringMath operator-(const StringMath& rhs);

	StringMath abs() const;
	bool isNegative() const;
	bool isPositive() const;

protected:
	std::string normalize(const std::string longNumber);
	StringMath add(const StringMath& numPositive2);
};

//StringMath pow(const StringMath& base, int exp);

