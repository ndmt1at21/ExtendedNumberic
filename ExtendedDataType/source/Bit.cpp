#include "Bit.h"

Bit::Bit()
{
	m_bBit = 0;
}

Bit::Bit(bool bit)
{
	m_bBit = bit;
}

Bit::Bit(const Bit& bit)
{
	m_bBit = bit.m_bBit;
}

Bit::~Bit()
{

}

Bit Bit::operator=(const Bit& bit)
{
	m_bBit = bit.m_bBit;
}

Bit Bit::add(const Bit& bit, Bit& carry)
{
	char countBit1 = 0;
	if (m_bBit)			countBit1++;
	if (bit.m_bBit)		countBit1++;
	if (carry.m_bBit)	countBit1++;

	Bit result = m_bBit ^ bit.m_bBit ^ carry.m_bBit;
	if (countBit1 >= 2)	carry = 1;
	else				carry = 0;

	return result;
}

bool Bit::operator==(const Bit& bit) const
{
	return (m_bBit == bit.m_bBit);
}

bool Bit::operator!=(const Bit& bit) const
{
	return (!operator==(bit));
}

Bit Bit::operator|(const Bit& bit)
{
	return (m_bBit | bit.m_bBit);
}

Bit Bit::operator&(const Bit& bit)
{
	return (m_bBit & bit.m_bBit);
}

Bit Bit::operator^(const Bit& bit) const
{
	return (m_bBit ^ bit.m_bBit);
}

Bit Bit::operator~()
{
	return (~m_bBit);
}

bool Bit::isBit0()
{
	return (!m_bBit);
}

bool Bit::isBit1()
{
	return m_bBit;
}