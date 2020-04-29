#include "BitArray.h"
#include <iostream>

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
		uint nRealBlock = (m_vBlockBit.size() - 1) - (index >> 3);
		m_vBlockBit[nRealBlock] |= (1 << nRealIndex);
	}
}

Bit BitArray::getBit(uint index) const
{
	if (index >= 0 && index < m_vBlockBit.size() << 3)
	{
		uint nRealIndex = index % 8;
		uint nRealBlock = (m_vBlockBit.size() - 1) - (index >> 3);
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

uint BitArray::getNumBlock() const
{
	return uint(m_vBlockBit.size());
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
	BitArray bitArr(rhs.getBitLength());
	for (uint i = 0; i < bitArr.getBitLength(); i++)
		if (rhs.getBit(i).isBit1())
			this->setBit(i);

	m_vBlockBit = bitArr.m_vBlockBit;
	return *this;
}

BitArray BitArray::operator+(const BitArray& rhs) const
{
	BitArray bitArrResult(this->getBitLength());

	Bit carry = 0;
	for (uint i = 0; i < this->getBitLength(); i++)
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
	BitArray result(getBitLength());
	BitArray rhsTwoComp = rhs.toTwoComplement();
	result = *this + rhsTwoComp;
	return result;
}

BitArray BitArray::operator*(const BitArray& rhs) const
{
	BitArray result(getBitLength());
	return result;
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