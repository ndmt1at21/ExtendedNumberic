#pragma once
#include <BitArray.h>
#include <Convert.h>
#include <iostream>

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

	friend std::istream& operator>>(std::istream& in, QInt& qInt);
	friend std::ostream& operator<<(std::ostream& out, const QInt& qInt);
	std::string to_string() const;
	std::string to_dec() const;
	std::string to_hex() const;

protected:
	bool isNegative() const;
	bool isPositive() const;
};

