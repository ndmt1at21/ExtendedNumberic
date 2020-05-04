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

StringMath::StringMath(int number) :StringMath(std::to_string(number))
{
	
}

StringMath::StringMath(long number) :StringMath(std::to_string(number))
{

}

StringMath::StringMath(float number) :StringMath(std::to_string(number))
{

}

StringMath::StringMath(double number) :StringMath(std::to_string(number))
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

StringMath StringMath::div(long divisor, uint precision)
{
	std::string result;
	
	bool sign1 = isNegative();
	bool sign2 = divisor > 0 ? 0 : 1;
	bool sign = sign1 ^ sign2;
	if (sign) 
		result.push_back('-');

	StringMath decNum = this->abs();
	unsigned long numDigitInt = decNum.getNumDigitInt();
	divisor = std::abs(divisor);

	// Find prefix of number that is larger 
	// than divisor. 
	unsigned long idx = 0;
	long temp = decNum[idx] - '0';
	while (temp < divisor && idx != numDigitInt - 1)
	{
		temp = temp * 10 + (decNum[++idx] - '0');
	}

	unsigned long countPrecision = 0;
	unsigned long mod = 0;
	while (countPrecision <= precision)
	{
		if (idx == numDigitInt)
		{
			result.push_back('.');
			idx++;
		}
		else
		{
			result.push_back(temp / divisor + '0');
			mod = temp % divisor;
			idx++;
			
			if (decNum[idx] != '.')
			{
				temp = mod * 10 + (decNum[idx] - '0');
			}
			else
			{
				result.push_back('.');
				temp = mod * 10 + (decNum[++idx] - '0');
			}

			if (idx >= numDigitInt)
				countPrecision++;
		}
	}

	normalize(result);
	return StringMath(result);
}

StringMath StringMath::operator+(const StringMath& rhs)
{
	if (isNegative() && rhs.isNegative()) {}
	else if (isNegative())
		return rhs.abs() - this->abs();
	else if (rhs.isNegative())
		return this->abs() - rhs.abs();
	
	StringMath num1(this->abs());
	StringMath num2(rhs.abs());

	uint maxLenInt = std::max(num1.getNumDigitInt(), num2.getNumDigitInt());
	uint maxLenFrac = std::max(num1.getNumDigitFractional(), num2.getNumDigitFractional());
	if (num2 > num1) std::swap(num1, num2);
	
	std::string result;
	result.reserve(maxLenInt + maxLenFrac);

	uint a, b, sum, carry, posLast;
	a = b = sum = carry = 0;
	
	if (maxLenFrac == 0)	posLast = maxLenInt - 1;
	else					posLast = maxLenInt + maxLenFrac;

	uint distance = num1.getNumDigitInt() - num2.getNumDigitInt();
	for (long i = posLast; i >= 0; i--) 
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
		result.push_back(carry + '0');

	if (isNegative() && rhs.isNegative())
		result.push_back('-');

	std::reverse(result.begin(), result.end());
	normalize(result);

	return StringMath(result);
}

StringMath StringMath::operator-(const StringMath& rhs)
{
	if (isNegative() && rhs.isPositive())
	{
		StringMath result = this->abs() + rhs.abs();
		result.m_sLongNumber.insert(result.m_sLongNumber.begin(), '-');
		return result;
	}
	else if (rhs.isNegative())
		return *this + rhs.abs();

	StringMath num1(this->abs());
	StringMath num2(rhs.abs());

	uint maxLenInt = std::max(num1.getNumDigitInt(), num2.getNumDigitInt());
	uint maxLenFrac = std::max(num1.getNumDigitFractional(), num2.getNumDigitFractional());

	bool sign = 0;
	if (num2 > num1)
	{
		std::swap(num1, num2);
		sign = 1;
	}

	std::string result;
	result.reserve(maxLenInt + maxLenFrac);

	int a, b, sub, carry, posLast;
	a = b = sub = carry = 0;

	if (maxLenFrac == 0)	posLast = maxLenInt - 1;
	else					posLast = maxLenInt + maxLenFrac;

	uint distance = num1.getNumDigitInt() - num2.getNumDigitInt();
	for (long i = posLast; i >= 0; i--) 
	{
		a = num1[i];
		b = num2[i - distance];

		if (a != '.' && b != '.')
		{
			sub = (a - '0') - ((b - '0') + carry);
			
			if (sub < 0)
			{
				sub += 10;
				carry = 1;
			}
			else
				carry = 0;

			result.push_back(sub + '0');
		}
		else
			result.push_back('.');
	}

	if (sign)
		result.push_back('-');
	std::reverse(result.begin(), result.end());
	normalize(result);

	return StringMath(result);
}

StringMath StringMath::operator-()
{
	StringMath result;

	if (isNegative())
		result = this->abs();
	else
	{
		result = *this;
		result.m_sLongNumber.insert(result.m_sLongNumber.begin(), '-');
	}
	normalize(result.m_sLongNumber);

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

bool StringMath::operator==(const StringMath& rhs) const
{
	if (m_sLongNumber == rhs.m_sLongNumber)
		return true;

	return false;
}

bool StringMath::operator!=(const StringMath& rhs) const
{
	return !(*this == rhs);
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

std::string StringMath::to_string() const
{
	return m_sLongNumber;
}

StringMath StringMath::getInt() const
{
	uint posPoint = getPosPoint();

	if (posPoint == NO_POINT)
		return *this;

	std::string result;
	result.reserve(posPoint);
	for (uint i = 0; i < posPoint; i++)
		result += m_sLongNumber[i];
	
	return result;
}

StringMath StringMath::getFraction() const
{
	uint posPoint = getPosPoint();

	if (posPoint == NO_POINT)
		return "0";

	std::string result;
	result.reserve(posPoint + 2);
	result += "0.";
	for (uint i = posPoint + 1; i < m_sLongNumber.size(); i++)
		result += m_sLongNumber[i];

	return result;
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

	StringMath dec = StringMath(decimalNumber);
	uint posPoint = dec.getPosPoint();
	bool isNeg = dec.isNegative();

	if (posPoint == NO_POINT)
	{
		uint posStart = isNeg;
		while (true)
		{
			if (decimalNumber[posStart] == '0')
				decimalNumber.erase(decimalNumber.begin() + posStart);
			else
				break;
		}
	}
	else
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

		uint posStart = isNeg;
		while (true)
		{
			if (decimalNumber[posStart] == '0' && decimalNumber[posStart + 1] != '.')
				decimalNumber.erase(decimalNumber.begin() + posStart);
			else
				break;
		}
	}

	if (decimalNumber.size() == 0)
		decimalNumber.push_back('0');
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
	
	return m_sLongNumber.size() - 1 - posPoint;
}

uint StringMath::getNumDigitInt() const
{
	uint posPoint = getPosPoint();

	if (posPoint != NO_POINT)
	{
		if (isNegative())
			return posPoint - 1;
		return posPoint;
	}
	
	if (isNegative())
		return  m_sLongNumber.size() - 1;

	return m_sLongNumber.size();
}

StringMath pow(const StringMath& decNum, long pow, uint precision)
{
	StringMath result(1);
	if (pow >= 0)
	{
		for (long i = 0; i < pow; i++)
			result = result * decNum;
	}
	else
	{
		for (long i = 0; i < pow; i++)
			result.div(std::abs(pow), precision);
	}

	return result;
}