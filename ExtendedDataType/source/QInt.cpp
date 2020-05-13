#include "QInt.h"
#include "Convert.h"
#include "Mode.h"
#include <algorithm>

QInt::QInt() :m_data(BIT_LENGTH)
{

}

QInt::QInt(const std::string& strNum, MODE mode)
{
	if (mode == MODE::dec)
	{
		std::string bits = Convert::DecToBin(strNum, 0);
		std::string absBits = StringMath(bits).abs().to_string();

		m_data = BitArray(bits);
		m_data.resize(BIT_LENGTH);

		if (bits[0] == '-')
			m_data = ~m_data + BitArray("1");
	}
	else if (mode == MODE::bin)
	{
		m_data = BitArray(strNum);
		m_data.resize(BIT_LENGTH);
	}
	else if (mode == MODE::hex)
	{
		std::string bits = Convert::HexToBin(strNum);
		m_data = BitArray(bits);
		m_data.resize(BIT_LENGTH);
	}

}

QInt::QInt(char n)
{
	uchar* bytes = new uchar[BIT_LENGTH / 8]{ 0 };

	char tmp = std::abs(n);
	std::memcpy(bytes, &tmp, sizeof(char));
	m_data = BitArray(bytes, BIT_LENGTH / 8);

	if (n < 0)
		m_data = ~m_data + BitArray("1");
	delete[] bytes;
}

QInt::QInt(int n)
{
	uchar* bytes = new uchar[BIT_LENGTH / 8]{ 0 };

	int tmp = std::abs(n);
	std::memcpy(bytes, &tmp, sizeof(int));
	m_data = BitArray(bytes, BIT_LENGTH / 8);

	if (n < 0)
		m_data = ~m_data + BitArray("1");
	delete[] bytes;
}

QInt::QInt(long n)
{
	uchar* bytes = new uchar[BIT_LENGTH / 8]{ 0 };

	long tmp = std::abs(n);
	std::memcpy(bytes, &tmp, sizeof(long));
	m_data = BitArray(bytes, BIT_LENGTH / 8);

	if (n < 0)
		m_data = ~m_data + BitArray("1");
	delete[] bytes;
}

QInt::QInt(const QInt& qInt)
{
	m_data = qInt.m_data;
}

QInt::~QInt()
{

}

QInt& QInt::operator=(const QInt& rhs)
{
	m_data = rhs.m_data;
	return *this;
}

QInt QInt::operator+(const QInt& rhs) const
{
	QInt result;
	result.m_data = this->m_data + rhs.m_data;

	return result;
}

QInt QInt::operator-(const QInt& rhs) const
{
	return (*this + (~rhs + 1));
}

QInt QInt::operator*(const QInt& rhs) const
{
	BitArray m(m_data);
	BitArray mNeg(~m_data + BitArray("1"));
	BitArray r(rhs.m_data);

	uint x = m.getBitLength();
	uint y = r.getBitLength();

	BitArray A(x + y);
	BitArray S(x + y);
	BitArray P(x + y);

	uint j = A.getBitLength();
	for (uint i = 0; i < x; i++)
	{
		j--;
		if (m.getBit(x - 1 - i).isBit1())
			A.setBit(j);

		if (mNeg.getBit(x - 1 - i).isBit1())
			S.setBit(j);
	}

	j = P.getBitLength() - x;
	for (uint i = 0; i < y; i++)
	{
		j--;
		if (r.getBit(y - 1 - i).isBit1())
			P.setBit(j);
	}

	uint nStep = 0;
	Bit bitAdd(0);
	while (nStep < y)
	{
		Bit bit0 = bitAdd;
		Bit bit1 = P.getLSB();

		if (bit1.isBit0() && bit0.isBit1())
			P = P + A;
		else if (bit1.isBit1() && bit0.isBit0())
			P = P + S;

		Bit prevMSB = P.getMSB();
		bitAdd = bit1;
		P = P >> 1;
		if (prevMSB.isBit1())
			P.setMSB();

		nStep++;
	}

	QInt result;
	result.m_data = P;
	result.m_data.resize(BIT_LENGTH);

	return result;
}

QInt QInt::operator/(const QInt& rhs) const
{
	if (rhs == QInt(0))
		return QInt(0);

	QInt remainder; // A
	QInt dividend = abs(*this); // Q
	QInt divisor = abs(rhs); // M

	uint k = BIT_LENGTH;
	while (k > 0)
	{
		Bit msbDiv = dividend.m_data.getMSB();
		remainder = remainder << 1;
		dividend = dividend << 1;
		if (msbDiv.isBit1())	
			remainder.m_data.setLSB();

		remainder = remainder - divisor;
		if (remainder.isNegative())
		{
			remainder = remainder + divisor;
		}
		else
		{
			dividend.m_data.setLSB();
		}

		k--;
	}

	if (isNegative() ^ rhs.isNegative())
		dividend = ~dividend + 1;

	return dividend;
}

QInt QInt::operator%(const QInt& rhs) const
{
	QInt remainder = *this - (*this / rhs) * rhs;
	return remainder;
}

QInt& QInt::operator+=(const QInt& rhs)
{
	*this = *this + rhs;
	return *this;
}

QInt& QInt::operator-=(const QInt& rhs)
{
	*this = *this - rhs;
	return *this;
}

QInt& QInt::operator*=(const QInt& rhs)
{
	*this = *this * rhs;
	return *this;
}

QInt& QInt::operator/=(const QInt& rhs)
{
	*this = *this / rhs;
	return *this;
}

bool QInt::operator==(const QInt& rhs) const
{
	for (uint i = 0; i < m_data.getBitLength(); i++)
		if (m_data.getBit(i) != rhs.m_data.getBit(i))
			return false;

	return true;
}

bool QInt::operator!=(const QInt& rhs) const
{
	return !(this->operator==(rhs));
}

bool QInt::operator>(const QInt& rhs) const
{
	if (isNegative() && rhs.isPositive())
		return false;

	if (isPositive() && rhs.isNegative())
		return true;

	if (*this == rhs)
		return false;

	for (long i = m_data.getBitLength() - 2; i >= 0; i--)
	{
		if (m_data.getBit(i).isBit1() && rhs.m_data.getBit(i).isBit0())
			return true;
		else if (m_data.getBit(i).isBit0() && rhs.m_data.getBit(i).isBit1())
			return false;
	}

	return false;
}

bool QInt::operator<(const QInt& rhs) const
{
	if (isPositive() && rhs.isNegative())
		return false;

	if (isNegative() && rhs.isPositive())
		return true;

	if (*this == rhs)
		return false;

	for (long i = m_data.getBitLength() - 2; i >= 0; i--)
	{
		if (m_data.getBit(i).isBit1() && rhs.m_data.getBit(i).isBit0())
			return false;
		else if (m_data.getBit(i).isBit0() && rhs.m_data.getBit(i).isBit1())
			return true;
	}

	return false;
}

bool QInt::operator>=(const QInt& rhs) const
{
	return !(*this < rhs);
}

bool QInt::operator<=(const QInt& rhs) const
{
	return !(*this > rhs);
}

QInt QInt::operator|(const QInt& rhs) const
{
	QInt result;
	result.m_data = m_data | rhs.m_data;

	return result;
}

QInt QInt::operator&(const QInt& rhs) const
{
	QInt result;
	result.m_data = m_data & rhs.m_data;

	return result;
}

QInt QInt::operator^(const QInt& rhs) const
{
	QInt result;
	result.m_data = m_data ^ rhs.m_data;

	return result;
}

QInt QInt::operator~() const
{
	QInt result;
	result.m_data = ~m_data;

	return result;
}

QInt QInt::operator-() const
{
	QInt result;
	result.m_data = ~m_data + BitArray("1");
	return result;
}

QInt QInt::operator>>(uint nBits) const
{
	QInt result;
	result.m_data = m_data >> nBits;

	return result;
}

QInt QInt::operator<<(uint nBits) const
{
	QInt result;
	result.m_data = m_data << nBits;

	return result;
}

QInt QInt::RoL(uint n) const
{
	QInt num1 = *this << n;
	QInt num2 = *this >> (BIT_LENGTH - n);
	QInt result = num1 | num2;

	return result;
}

QInt QInt::RoR(uint n) const
{
	QInt num1 = *this >> n;
	QInt num2 = *this << (BIT_LENGTH - n);
	QInt result = num1 | num2;

	return result;
}

std::istream& operator>>(std::istream& in, QInt& qInt)
{
	std::string dec;
	std::getline(in, dec, '\n');

	std::string bits = Convert::DecToBin(dec, 0);
	qInt.m_data = BitArray(bits);

	return in;
}

std::ostream& operator<<(std::ostream& out, const QInt& qInt)
{
	out << qInt.to_dec();
	return out;
}

std::string QInt::to_string() const
{
	return to_dec();
}

std::string QInt::to_bin() const
{
	return m_data.to_string();
}

std::string QInt::to_dec() const
{
	std::string bits;
	if (isNegative())
	{
		bits.push_back('-');
		bits += (~m_data + BitArray("1")).to_string();
	}
	else
		bits = m_data.to_string();

	std::string dec = Convert::BinToDec(bits, 0);
	return dec;
}

std::string QInt::to_hex() const
{
	std::string bits = m_data.to_string();
	std::string hex = Convert::BinToHex(bits);
	return hex;
}

bool QInt::isNegative() const
{
	if (m_data.getMSB().isBit1())
		return true;
	return false;
}

bool QInt::isPositive() const
{
	return (!isNegative());
}

QInt abs(QInt qInt)
{
	if (qInt.isNegative())
		return ~qInt + 1;
	return qInt;
}