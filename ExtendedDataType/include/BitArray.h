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
	BitArray(uint bitLength);
	BitArray(const uchar* bytes, uint nBytes);
	BitArray(const Bit* bits, uint bitLength);
	BitArray(const std::string& bits);
	BitArray(const BitArray& bitArr);
	~BitArray();

	void setBit(uint index);
	Bit getBit(uint index) const;
	void setBlock(uint index, uchar value);
	uchar getBlock(uint index) const;
	Bit getLSB() const;
	Bit getMSB() const;
	void setLSB();
	void setMSB();

	uint getNumBlock() const;
	uint getBitLength() const;
	void resize(uint newBitLength);
	void clear();

	BitArray& operator=(const BitArray& rhs);
	BitArray operator+(const BitArray& rhs) const;
	BitArray operator-(const BitArray& rhs) const;

	BitArray operator|(const BitArray& rhs) const;
	BitArray operator&(const BitArray& rhs) const;
	BitArray operator^(const BitArray& rhs) const;
	BitArray operator~() const;
	BitArray operator>>(uint nBits) const;
	BitArray operator<<(uint nBits) const;

	std::string to_string() const;

protected:
	// Assert bitLength true. Ex: bitLen = 10 -> normal -> 16
	void normalizeBitLength(uint& bitLength);
};

