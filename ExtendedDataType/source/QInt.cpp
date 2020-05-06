#include "QInt.h"
#include "Convert.h"
#include <algorithm>

QInt::QInt() :m_data(BIT_LENGTH)
{

}

QInt::QInt(const std::string& dec)
{
	std::string bits = Convert::DecToBin(dec, 0);
	std::string absBits = StringMath(bits).abs().to_string();
	
	m_data = BitArray(bits);
	m_data.resize(BIT_LENGTH);

	if (bits[0] == '-')
		m_data = ~m_data + BitArray("1");
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

	return result;
}

QInt QInt::operator/(const QInt& rhs) const
{
	QInt result;
	return result;
}

QInt QInt::operator%(const QInt& rhs) const
{
	QInt result;
	return result;
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
	return (!isPositive());
}