#pragma once
#include <BitArray.h>
#include <Convert.h>

class QInt
{
private:
	BitArray m_data;

public:
	QInt();
	QInt(const std::string& dec);
	QInt(char n);
	QInt(int n);
	QInt(long n);
	QInt(const QInt& qInt);
	~QInt();

	QInt& operator=(const QInt& rhs);
	QInt operator+(const QInt& rhs) const;
	QInt operator-(const QInt& rhs) const;
	QInt operator*(const QInt& rhs) const;
	QInt operator/(const QInt& rhs) const;
	QInt operator%(const QInt& rhs) const;
	QInt& operator+=(const QInt& rhs);
	QInt& operator-=(const QInt& rhs);
	QInt& operator*=(const QInt& rhs);
	QInt& operator/=(const QInt& rhs);

	QInt operator|(const QInt& rhs) const;
	QInt operator&(const QInt& rhs) const;
	QInt operator^(const QInt& rhs) const;
	QInt operator~() const;
	QInt operator>>(uint nBits) const;
	QInt operator<<(uint nBits) const;
	QInt RoL() const;
	QInt RoR() const;
};

