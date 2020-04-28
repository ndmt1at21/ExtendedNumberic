#pragma once
class Bit
{
private:
	bool m_bBit;

public:
	Bit();
	Bit(bool bit);
	Bit(const Bit& bit);
	~Bit();

	// Assign operator
	Bit& operator=(const Bit& bit);

	// Compare operator
	bool operator==(const Bit& bit) const;
	bool operator!=(const Bit& bit) const;

	// Bitwise operator
	Bit operator|(const Bit& bit);
	Bit operator&(const Bit& bit);
	Bit operator^(const Bit& bit) const;
	Bit operator~();

	Bit add(const Bit& bit, Bit& carry);
	bool isBit0();
	bool isBit1();
};

