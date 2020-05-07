#pragma once
#include "BitArray.h"
#include "StringMath.h"
#include "Convert.h"
#include <string>

#define BIT_LENGTH 128
#define NUM_BIT_SIGN 1
#define NUM_BIT_EXP 15
#define NUM_BIT_FRAC 112
#define BIAS pow(2, NUM_BIT_EXP - 1) - 1
#define PRECISION floor((NUM_BIT_FRAC + 1) * log10(2))

enum MODE
{
	bin = 0, 
	dec = 1
};

class QFloat
{
private:
	BitArray m_data;
	
public:
	QFloat();
	QFloat(const std::string& strNum, MODE mode = MODE::dec);
	QFloat(float fnum);
	QFloat(double dnum);
	QFloat(const QFloat& qFloat);

	QFloat& operator=(const QFloat& qFloat);
	std::string to_dec();
	std::string to_bin();
	std::string to_string();

protected:
	bool isNegative();
	bool isPositive();
	bool isZero();
	bool isDenormalize();
	bool isInf();
	bool isNaN();

	void setBitSign();
	Bit getBitSign();
	void setBitExp(uint idx);
	Bit getBitExp(uint idx);
	void setBitFrac(uint idx);
	Bit getBitFrac(uint idx);

	BitArray getFraction();
	BitArray getExp();

private:
	std::vector<std::string> normalize(const std::string dec);
};

