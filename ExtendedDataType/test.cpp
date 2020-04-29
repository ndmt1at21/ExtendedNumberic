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

	Bit& operator=(const Bit& bit);
	bool operator==(const Bit& bit) const;
	bool operator!=(const Bit& bit) const;
	Bit operator|(const Bit& bit);
	Bit operator&(const Bit& bit);
	Bit operator^(const Bit& bit) const;
	Bit operator~();

	bool isBit0();
	bool isBit1();
};

