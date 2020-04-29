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

Bit& Bit::operator=(const Bit& bit)
{
	m_bBit = bit.m_bBit;
	return *this;
}

Bit::operator bool() const
{
	return m_bBit;
}

Bit::operator int() const
{
	return (int)m_bBit;
}

Bit::operator char() const
{
	return (char)m_bBit;
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
	if (isBit0())
		return 1;
	return 0;
}

bool Bit::isBit0() const
{
	return (!m_bBit);
}

bool Bit::isBit1() const
{
	return m_bBit;
}