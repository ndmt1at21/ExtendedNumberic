#pragma once
#include <vector>
#include <string>
#include "Bit.h"

typedef unsigned char uchar;
typedef unsigned int uint;

class BitArray
{
private:
	std::vector<uchar> m_vBlockBit;

public:
	BitArray();
	BitArray(uint bitLength = 8);
	BitArray(const uchar* bytes, uint nBytes);
	BitArray(const Bit* bits, uint bitLength);
	BitArray(const std::string& bits);
	BitArray(const BitArray& bitArr);
	~BitArray();

	void setBit(uint index);
	Bit getBit(uint index) const;
	void setBlock(uint index, uchar value);
	uchar getBlock(uint index) const;

	uint getNumBlock() const;
	uint getBitLength() const;
	bool resize(uint newBitLength);
	void clear();

	BitArray operator=(const BitArray& rhs);
	BitArray operator+(const BitArray& rhs);
	BitArray operator-(const BitArray& rhs);
	BitArray operator*(const BitArray& rhs);
	BitArray operator/(const BitArray& rhs);
	BitArray& operator+=(const BitArray& rhs);
	BitArray& operator-=(const BitArray& rhs);
	BitArray& operator*=(const BitArray& rhs);
	BitArray& operator/=(const BitArray& rhs);

	BitArray operator|(const BitArray& rhs);
	BitArray operator&(const BitArray& rhs);
	BitArray operator^(const BitArray& rhs);
	BitArray operator~();
	BitArray operator>>(uint nBits);
	BitArray operator<<(uint nBits);

protected:
	// Assert bitLength true. Ex: bitLen = 10 -> normal -> 16
	void normalizeBitLength(uint& bitLength);
};

