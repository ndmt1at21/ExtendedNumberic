#include "BitArray.h"
#include <iostream>
#include <algorithm>

BitArray::BitArray()
{
	m_vBlockBit.resize(1);
	m_vBlockBit[0] = 0;
}

BitArray::BitArray(uint bitLength)
{
	normalizeBitLength(bitLength);
	
	uint nBlocks = (bitLength >> 3);
	m_vBlockBit.resize(nBlocks);

	for (uint i = 0; i < (uint)m_vBlockBit.size(); i++)
		m_vBlockBit[i] = 0;
}

BitArray::BitArray(const Bit* bits, uint bitLength)
{
	normalizeBitLength(bitLength);

	uint nBlocks = (bitLength >> 3);
	m_vBlockBit.resize(nBlocks);

	for (uint i = 0; i < bitLength; i++)
	{
		if (bits[bitLength - 1 - i].isBit1())
			this->setBit(i);
	}
}

BitArray::BitArray(const std::string& bits)
{
	uint bitLength = (uint)bits.length();
	normalizeBitLength(bitLength);

	uint nBlocks = (bitLength >> 3);
	m_vBlockBit.resize(nBlocks);

	for (uint i = 0; i < bits.length(); i++)
	{
		if (bits[bits.length() - 1 - i] == '1')
			this->setBit(i);
	}
}

BitArray::BitArray(const uchar* bytes, uint nBytes)
{
	if (nBytes == 0)
		throw std::logic_error("Bit length not NULL");

	m_vBlockBit.resize(nBytes);
	for (uint i = 0; i < (uint)m_vBlockBit.size(); i++)
		m_vBlockBit[i] = bytes[i];
}

BitArray::BitArray(const BitArray& bitArr)
{
	m_vBlockBit = bitArr.m_vBlockBit;
}


BitArray::~BitArray()
{

}


void BitArray::setBit(uint index)
{
	if (index >= 0 && index < m_vBlockBit.size() << 3)
	{
		uint nRealIndex = index % 8;
		uint nRealBlock = index / 8;
		m_vBlockBit[nRealBlock] |= (1 << nRealIndex);
	}
}

Bit BitArray::getBit(uint index) const
{
	if (index >= 0 && index < m_vBlockBit.size() << 3)
	{
		uint nRealIndex = index % 8;
		uint nRealBlock = index / 8;
		return (m_vBlockBit[nRealBlock] >> nRealIndex) & 1;
	}

	return Bit(0);
}

void BitArray::setBlock(uint index, uchar value)
{
	if (index >= 0 && index < m_vBlockBit.size())
		m_vBlockBit[index] = value;
}

uchar BitArray::getBlock(uint index) const
{
	if (index >= 0 && index < m_vBlockBit.size())
		return m_vBlockBit[index];

	return 0;
}

Bit BitArray::getLSB() const
{
	return getBit(0);
}

Bit BitArray::getMSB() const
{
	return getBit(getBitLength() - 1);
}

uint BitArray::getNumBlock() const
{
	return uint(m_vBlockBit.size());
}

void BitArray::setLSB()
{
	setBit(0);
}

void BitArray::setMSB()
{
	setBit(getBitLength() - 1);
}

uint BitArray::getBitLength() const
{
	return (m_vBlockBit.size() << 3);
}

void BitArray::resize(uint newBitLength)
{
	uint minLen = newBitLength > getBitLength() ? getBitLength() : newBitLength;
	BitArray newBitArray(newBitLength);

	for (uint i = 0; i < minLen; i++)
	{
		if (this->getBit(i).isBit1())
			newBitArray.setBit(i);
	}

	m_vBlockBit = newBitArray.m_vBlockBit;
}

void BitArray::clear()
{
	for (uint i = 0; i < (uint)m_vBlockBit.size(); i++)
		m_vBlockBit[i] = 0;
}

std::string BitArray::toString()
{
	std::string result(getBitLength(), '0');
	for (uint i = getBitLength() - 1; i >= 0; i++)
		if (getBit(i).isBit1())
			result[i] = '1';

	return result;
}

BitArray& BitArray::operator=(const BitArray& rhs)
{
	this->clear();
	for (uint i = 0; i < getBitLength(); i++)
		if (rhs.getBit(i).isBit1())
			this->setBit(i);

	return *this;
}

BitArray BitArray::operator+(const BitArray& rhs) const
{
	uint maxBitLen = std::max(this->getBitLength(), rhs.getBitLength());
	BitArray bitArrResult(maxBitLen);

	Bit carry = 0;
	for (uint i = 0; i < maxBitLen; i++)
	{
		Bit bit1 = this->getBit(i);
		Bit bit2 = rhs.getBit(i);
		Bit result = (bit1 ^ bit2) ^ carry;
		
		char tmp = (char)bit1 + (char)bit2 + (char)carry;
		if (tmp >= 2)	carry = 1;
		else			carry = 0;

		if (result.isBit1())
			bitArrResult.setBit(i);
	}

	return bitArrResult;
}

BitArray BitArray::operator-(const BitArray& rhs) const
{
	BitArray rhsTwoComp = rhs.toTwoComplement();
	BitArray result = *this + rhsTwoComp;
	
	return result;
}

// Algorithm: Booth (refer Wiki)
BitArray BitArray::operator*(const BitArray& rhs) const
{
	BitArray m(*this);
	BitArray mNeg(this->toTwoComplement());
	BitArray r(rhs);

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

	return P;
}

BitArray BitArray::operator/(const BitArray& rhs) const
{
	BitArray result(getBitLength());
	return result;
}

BitArray& BitArray::operator+=(const BitArray& rhs)
{
	*this = this->operator+(rhs);
	return *this;
}

BitArray& BitArray::operator-=(const BitArray& rhs)
{
	BitArray result(getBitLength());
	return result;
}

BitArray& BitArray::operator*=(const BitArray& rhs)
{
	BitArray result(getBitLength());
	return result;
}

BitArray& BitArray::operator/=(const BitArray& rhs)
{
	BitArray result(getBitLength());
	return result;
}

BitArray BitArray::operator|(const BitArray& rhs) const
{
	BitArray bitArrResult(this->getBitLength());
	for (uint i = 0; i < this->getNumBlock(); i++)
		bitArrResult.setBlock(i, this->getBlock(i) | rhs.getBlock(i));
	
	return bitArrResult;
}

BitArray BitArray::operator&(const BitArray& rhs) const
{
	BitArray bitArrResult(this->getBitLength());
	for (uint i = 0; i < this->getNumBlock(); i++)
		bitArrResult.setBlock(i, this->getBlock(i) | rhs.getBlock(i));

	return bitArrResult;
}

BitArray BitArray::operator^(const BitArray& rhs) const
{
	BitArray bitArrResult(this->getBitLength());
	for (uint i = 0; i < this->getNumBlock(); i++)
		bitArrResult.setBlock(i, this->getBlock(i) ^ rhs.getBlock(i));

	return bitArrResult;
}

BitArray BitArray::operator~() const
{
	BitArray bitArrResult(this->getBitLength());

	for (uint i = 0; i < this->getNumBlock(); i++)
		bitArrResult.setBlock(i, ~(this->getBlock(i)));

	return bitArrResult;
}

BitArray BitArray::operator>>(uint nBits) const
{
	BitArray bitArrResult(getBitLength());

	for (uint i = 0; i < getBitLength(); i++)
	{
		Bit bit = this->getBit(i);
		if (bit.isBit1())
			bitArrResult.setBit(i - nBits);
	}

	return bitArrResult;
}

BitArray BitArray::operator<<(uint nBits) const
{
	BitArray bitArrResult(getBitLength());

	for (uint i = 0; i < getBitLength(); i++)
	{
		Bit bit = this->getBit(i);
		if (bit.isBit1())
			bitArrResult.setBit(i + nBits);
	}

	return bitArrResult;
}

void BitArray::normalizeBitLength(uint& bitLength)
{
	if (bitLength == 0)
		throw std::logic_error("Bit length not NULL");

	uint bitLenAccept[10] = {8, 16, 24, 32, 64, 128, 256, 512, 1024, 2048};
	int n = sizeof(bitLenAccept) / sizeof(uint);
	for (int i = 0; i < n; i++)
		if (bitLength <= bitLenAccept[i])
		{
			bitLength = bitLenAccept[i];
			break;
		}
}

BitArray BitArray::toTwoComplement() const
{
	BitArray numOne("1");
	BitArray result = ~(*this) + numOne;
	return result;
}