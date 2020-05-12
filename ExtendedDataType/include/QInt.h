#pragma once
#include <BitArray.h>
#include <Convert.h>
#include "Mode.h"
#include <iostream>

#define BIT_LENGTH 128 

class QInt
{
private:
	BitArray m_data;

public:
	QInt();
	QInt(const std::string& strNum, MODE mode = MODE::dec);
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

	bool operator==(const QInt& rhs) const;
	bool operator!=(const QInt& rhs) const;
	bool operator>(const QInt& rhs) const;
	bool operator<(const QInt& rhs) const;
	bool operator>=(const QInt& rhs) const;
	bool operator<= (const QInt & rhs) const;

	QInt operator|(const QInt& rhs) const;
	QInt operator&(const QInt& rhs) const;
	QInt operator^(const QInt& rhs) const;
	QInt operator~() const;
	QInt operator-() const;
	QInt operator>>(uint nBits) const;
	QInt operator<<(uint nBits) const;
	QInt RoL(uint n) const;
	QInt RoR(uint n) const;

	friend std::istream& operator>>(std::istream& in, QInt& qInt);
	friend std::ostream& operator<<(std::ostream& out, const QInt& qInt);
	std::string to_string() const;
	std::string to_bin() const;
	std::string to_dec() const;
	std::string to_hex() const;

	bool isNegative() const;
	bool isPositive() const;
};

QInt abs(QInt qInt);