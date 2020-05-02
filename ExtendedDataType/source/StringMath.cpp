#include "StringMath.h"
#include <algorithm>
#include <vector>

StringMath::StringMath()
{
	m_sLongNumber = "0";
}

StringMath::StringMath(const std::string& decimalNumber)
{
	if (validData(decimalNumber))
		m_sLongNumber = decimalNumber;
	else 
		throw std::logic_error("Error data");
}

StringMath::StringMath(const char* decimalNumber) : StringMath(std::string(decimalNumber))
{
	
}

StringMath::StringMath(const StringMath& strMath)
{
	m_sLongNumber = strMath.m_sLongNumber;
}

StringMath& StringMath::operator=(const StringMath& rhs)
{
	m_sLongNumber = rhs.m_sLongNumber;
	return *this;
}

StringMath StringMath::operator*(const StringMath& rhs)
{
	if (rhs == StringMath("0") || *this == StringMath("0"))
		return "0";

	StringMath abs1 = this->abs();
	StringMath abs2 = rhs.abs();

	std::string num1 = abs1.m_sLongNumber;
	std::string num2 = abs2.m_sLongNumber;
	if (abs1.getPosPoint() != NO_POINT)
		num1.erase(num1.begin() + abs1.getPosPoint());
	if (abs2.getPosPoint() != NO_POINT)
		num2.erase(num2.begin() + abs2.getPosPoint());
	
	uint len1 = num1.length();
	uint len2 = num2.length();


	// will keep the result number in vector 
	// in reverse order 
	std::vector<int> result(len1 + len2, 0);

	// Below two indexes are used to find positions 
	// in result.  
	int i_n1 = 0;
	int i_n2 = 0;

	for (int i = len1 - 1; i >= 0; i--)
	{
		int carry = 0;

		int n1 = num1[i] - '0';

		// To shift position to left after every 
		// multiplication of a digit in num2 
		i_n2 = 0;

		if (n1)
			// Go from right to left in num2              
			for (int j = len2 - 1; j >= 0; j--)
			{
				// Take current digit of second number 

				int n2 = num2[j] - '0';
				int sum = n1 * n2 + result[i_n1 + i_n2] + carry;
				carry = sum / 10;
				result[i_n1 + i_n2] = sum % 10;

				i_n2++;
			}

		// store carry in next cell 
		if (carry > 0)
			result[i_n1 + i_n2] += carry;

		// To shift position to left after every 
		// multiplication of a digit in num1. 
		i_n1++;
	}

	// generate the result string 
	std::string tmp;
	for (long i = result.size() - 1; i >= 0; i--)
		tmp.push_back(result[i] + '0');
	uint posPoint = result.size() - (this->getNumDigitFractional() + rhs.getNumDigitFractional());
	if (posPoint >= 0 && posPoint < result.size())
		tmp.insert(posPoint, ".");

	normalize(tmp);
	
	bool sign = isNegative() ^ rhs.isNegative();
	std::string ans;
	
	if (sign) ans.push_back('-');
	ans += tmp;

	return StringMath(ans);
}

StringMath StringMath::operator/(const StringMath& divisor)
{
	StringMath result;
	return result;
}

StringMath StringMath::operator+(const StringMath& rhs)
{
	if (isNegative() && rhs.isNegative()) {}
	else if (isNegative() || rhs.isNegative())
		return *this - rhs;
	
	StringMath num1(this->abs());
	StringMath num2(rhs.abs());

	uint maxLenInt = std::max(num1.getNumDigitInt(), num2.getNumDigitInt());
	uint maxLenFrac = std::max(num1.getNumDigitFractional(), num2.getNumDigitFractional());

	if (num1.getNumDigitInt() < num2.getNumDigitInt())
		std::swap(num1, num2);
	uint distance = num1.getNumDigitInt() - num2.getNumDigitInt();
	
	std::string result;
	result.reserve(maxLenInt + maxLenFrac);

	uint a, b, sum, carry;
	a = b = sum = carry = 0;
	for (long i = maxLenInt + maxLenFrac; i >= 0; i--) // + 1 (,)
	{
		a = num1[i];
		b = num2[i - distance];

		if (a != '.' && b != '.')
		{
			sum = (a - '0') + (b - '0') + carry;
			result.push_back(sum % 10 + '0');
			carry = sum / 10;
		}
		else
			result.push_back('.');
	}

	if (carry != 0)
		result.push_back(carry);

	if (isNegative() && rhs.isNegative())
		result.push_back('-');

	std::reverse(result.begin(), result.end());
	return StringMath(result);
}

StringMath StringMath::operator-(const StringMath& rhs)
{
	StringMath result;
	return result;
}

bool StringMath::operator>(const StringMath& rhs) const
{
	uint minLen = std::min(rhs.m_sLongNumber.length(), m_sLongNumber.length());
	bool sign1 = isNegative();
	bool sign2 = rhs.isNegative();

	if (sign1 && !sign2)
		return false;
	else if (!sign1 && sign2)
		return true;
	else if (!sign1 && !sign2)
	{
		if (getNumDigitInt() > rhs.getNumDigitInt())
			return true;
		else if (getNumDigitInt() < rhs.getNumDigitInt())
			return false;
		else
		{
			for (uint i = 0; i < minLen; i++)
				if (m_sLongNumber[i] != '.' && m_sLongNumber[i] > rhs.m_sLongNumber[i])
					return true;
		}
	}
	else if (sign1 && sign2)
	{
		if (getNumDigitInt() > rhs.getNumDigitInt())
			return false;
		else if (getNumDigitInt() < rhs.getNumDigitInt())
			return true;
		else
		{
			for (uint i = 1; i < minLen; i++)
				if (m_sLongNumber[i] != '.' && m_sLongNumber[i] <= rhs.m_sLongNumber[i])
					return true;
		}
	}

	return false;
}

bool StringMath::operator<(const StringMath& rhs) const
{
	return true;
}

bool StringMath::operator>=(const StringMath& rhs) const
{
	return true;
}
bool StringMath::operator<=(const StringMath& rhs) const
{
	return true;
}

bool StringMath::operator==(const StringMath& rhs) const
{
	if (m_sLongNumber == rhs.m_sLongNumber)
		return true;

	return false;
}

bool StringMath::operator!=(const StringMath& rhs) const
{
	return true;
}

StringMath StringMath::abs() const
{
	StringMath result = *this;
	if (isNegative())
		result.m_sLongNumber.erase(result.m_sLongNumber.begin());
	else
		result.m_sLongNumber = m_sLongNumber;

	return result;
}

char StringMath::operator[](long index)
{
	if (index >= 0 && index < m_sLongNumber.size())
		return m_sLongNumber[index];
	return '0';
}

bool StringMath::isNegative() const
{
	if (m_sLongNumber[0] == '-')
		return true;
	return false;
}

bool StringMath::isPositive() const
{
	return (!isNegative());
}

bool StringMath::validData(const std::string& decimalNumber)
{
	if (decimalNumber.length() == 0)
		return false;

	uint countNeg = 0;
	uint countPoint = 0;
	std::string digits = "0123456789";

	for (uint i = 0; i < decimalNumber.size(); i++)
	{
		if (decimalNumber[i] == '-')		
			countNeg++;
		else if (decimalNumber[i] == '.')	
			countPoint++;
		else if (digits.find(decimalNumber[i], 0) == std::string::npos)
			return false;
	}

	if (countNeg == 1 && decimalNumber[0] != '-')
		return false;
	
	if (countPoint == 1 && decimalNumber[0] == '.' || 
		decimalNumber[decimalNumber.size() - 1] == '.')
		return false;

	if (countNeg > 1 || countPoint > 1)
		return false;

	return true;
}

void StringMath::normalize(std::string& decimalNumber)
{
	if (decimalNumber.size() == 0)
		throw std::logic_error("Error data");

	uint posPoint = StringMath(decimalNumber).getPosPoint();

	if (posPoint != NO_POINT)
	{
		while (true)
		{
			if (decimalNumber[decimalNumber.size() - 1] == '0')
				decimalNumber.erase(decimalNumber.end() - 1);
			else
				break;
		}

		if (decimalNumber[decimalNumber.size() - 1] == '.')
			decimalNumber.erase(decimalNumber.end() - 1);

		while (true)
		{
			if (decimalNumber[0] == '0' && decimalNumber[1] != '.')
				decimalNumber.erase(decimalNumber.begin());
			else
				break;
		}
	}
	else
	{
		while (true)
		{
			if (decimalNumber[0] == '0')
				decimalNumber.erase(decimalNumber.begin());
			else
				break;
		}
	}
}

uint StringMath::getPosPoint() const
{
	size_t index = m_sLongNumber.find('.', 0);
	if (index == std::string::npos)
		return NO_POINT;

	return (uint)index;
}

uint StringMath::getNumDigitFractional() const
{
	uint posPoint = getPosPoint();
	if (posPoint == NO_POINT)
		return 0;
	
	return m_sLongNumber.length() - 1 - posPoint;
}

uint StringMath::getNumDigitInt() const
{
	uint posPoint = getPosPoint();

	if (isNegative())
		return posPoint - 1;

	return posPoint;
}
