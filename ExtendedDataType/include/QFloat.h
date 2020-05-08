#pragma once
#include "BitArray.h"
#include "StringMath.h"
#include "Convert.h"
#include "Mode.h"
#include <string>

#define BIT_LENGTH 128
#define NUM_BIT_SIGN 1
#define NUM_BIT_EXP 15
#define NUM_BIT_FRAC 112
#define BIAS pow(2, NUM_BIT_EXP - 1) - 1
#define PRECISION (NUM_BIT_FRAC + 1) * log10(2) // num digit can retain
#define LIMIT_NUM 4932

class QFloat
{
private:
	BitArray m_data;
	
public:
	QFloat();
	QFloat(const std::string& strNum, MODE mode = MODE::dec);
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

	std::string getFraction();
	std::string getExp();

private:
	std::vector<std::string> normalize(const std::string dec);
};

