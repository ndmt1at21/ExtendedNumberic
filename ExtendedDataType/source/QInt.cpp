#include "QInt.h"
#include "Convert.h"
#include <algorithm>

QInt::QInt() :m_data()
{

}

QInt::QInt(const std::string& dec)
{
	std::string bits = Convert::DecToBin(dec, 0);
	m_data = BitArray(bits);
}

QInt::QInt(char n)
{
	m_data = BitArray((uchar*)n, sizeof(char));
}

QInt::QInt(int n)
{
	m_data = BitArray((uchar*)n, sizeof(int));
}

QInt::QInt(long n)
{
	m_data = BitArray((uchar*)n, sizeof(long));
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
	uint maxBitLen = std::max(m_data.getBitLength(), rhs.m_data.getBitLength());
	QInt result;
	BitArray bitArrResult(maxBitLen);

	Bit carry = 0;
	for (uint i = 0; i < maxBitLen; i++)
	{
		Bit bit1 = m_data.getBit(i);
		Bit bit2 = rhs.m_data.getBit(i);
		Bit sum = (bit1 ^ bit2) ^ carry;

		char tmp = (char)bit1 + (char)bit2 + (char)carry;
		if (tmp >= 2)	carry = 1;
		else			carry = 0;

		if (sum.isBit1())
			bitArrResult.setBit(i);
	}

	result.m_data = bitArrResult;
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

	return result;
}

QInt QInt::operator/(const QInt& rhs) const
{

}

QInt QInt::operator%(const QInt& rhs) const
{

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

QInt QInt::RoL() const
{
	QInt result;
	Bit msb = m_data.getMSB();
	
	result.m_data = m_data;
	result = result << 1;
	if (msb.isBit1())
		result.m_data.setLSB();

	return result;
}

QInt QInt::RoR() const
{
	QInt result;
	Bit lsb = m_data.getLSB();

	result.m_data = m_data;
	result = result >> 1;
	if (lsb.isBit1())
		result.m_data.setMSB();

	return result;
}