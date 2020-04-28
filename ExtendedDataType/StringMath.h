#pragma once
#include <string>
#include <iostream>

typedef unsigned int uint;

class StringMath
{
private:
	std::string m_sLongNumber;

public:
	StringMath(const std::string& longNumber);
	StringMath(const char* longNumber);
	StringMath(const StringMath& strMath);

	StringMath operator*(const StringMath& rhs);
	StringMath operator/(int divisor);
	friend std::ostream& operator<<(std::ostream& out, const StringMath& str);

	StringMath pow();
	StringMath abs() const;
	bool isNegative() const;

protected:
	std::string normalize(const std::string longNumber);
};

