#include "QFloat.h"
#include <sstream>
QFloat::QFloat() :m_data(BIT_LENGTH)
{

}

QFloat::QFloat(const std::string& strNum, MODE mode)
{
	if (mode == MODE::dec)
	{
		m_data = BitArray(BIT_LENGTH);

		std::vector<std::string> standardize = normalize(strNum);
		bool sign = standardize[0][0] - '0';
		std::string fractionBit = standardize[2];
		
		std::stringstream ss(standardize[1]);
		int exp = 0;
		ss >> exp;

		if (exp == 0 && fractionBit == "0")
		{
			if (strNum[0] == '-')
				setBitSign();
		}
		else
		{
			exp += BIAS;
			std::string expBit = Convert::DecToBin(std::to_string(exp), 0);

			if (sign)
				setBitSign();

			for (uint i = 0; i < expBit.size(); i++)
			{
				if (expBit[expBit.size() - 1 - i] == '1')
					setBitExp(i);
			}

			for (uint i = 0; i < fractionBit.size(); i++)
				if (fractionBit[i] == '1')
					setBitFrac(NUM_BIT_FRAC - 1 - i);
		}
	}
	else if (mode == MODE::bin)
	{
		m_data = BitArray(strNum);
		m_data.resize(BIT_LENGTH);
	}
}

QFloat::QFloat(const QFloat& qFloat)
{
	m_data = qFloat.m_data;
}

QFloat& QFloat::operator=(const QFloat& qFloat)
{
	m_data = qFloat.m_data;
	return *this;
}

std::string QFloat::to_dec()
{
	if (isZero())
	{
		if (isNegative())
			return "-0";
		return "0";
	}
	
	if (isInf())
	{
		if (isNegative())
			return "-Inf";
		return "Inf";
	}

	if (isNaN())
		return "NaN";

	if (isDenormalize())
	{
		std::string fraction = Convert::BinToDec(getFraction(), LIMIT_NUM);

		StringMath pow2Exp = 1;
		for (uint i = 0; i < std::abs(0 - BIAS + 1); i++)
				pow2Exp = pow2Exp.div(2, LIMIT_NUM);

		StringMath result = pow2Exp * StringMath(fraction);
		std::string dec = result.getInt().to_string();
		
		if (result.getPosPoint() != NO_POINT)
			dec.push_back('.');

		uint countPre = 0;
		for (uint i = result.getPosPoint() + 1; i < result.length(); i++)
		{
			if (countPre == LIMIT_NUM)
				break;
			dec += result[i];
			countPre++;
		}
	
		if (isNegative())
			return "-" + dec;
		return dec;
	}
	else
	{
		std::string exponent = Convert::BinToDec(getExp(), 0);
		std::string fraction = Convert::BinToDec(getFraction(), LIMIT_NUM);

		std::stringstream ss(exponent);
		int realExp = 0;
		ss >> realExp;
		realExp -= BIAS;

		StringMath pow2Exp = 1;
		for (uint i = 0; i < std::abs(realExp); i++)
		{
			if (realExp > 0)
				pow2Exp = pow2Exp * 2;
			else
				pow2Exp = pow2Exp.div(2, LIMIT_NUM);
		}

		StringMath result = pow2Exp * (StringMath(fraction) + 1);
		std::string dec = result.to_string();

		if (isNegative())
			return "-" + dec;
		return dec;
	}
}

std::string QFloat::to_bin()
{
	return m_data.to_string();
}

std::string QFloat::to_string()
{
	return to_dec();
}

bool QFloat::isNegative()
{
	if (m_data.getBit(BIT_LENGTH - 1).isBit1())
		return true;
	return false;
}

bool QFloat::isPositive()
{
	return (!isNegative());
}

bool QFloat::isZero()
{
	bool isExpAllZero = true;
	bool isFracAllZero = true;

	for (uint i = 0; i < NUM_BIT_FRAC; i++)
	{
		if (getBitFrac(i).isBit1())
		{
			isFracAllZero = false;
			break;
		}
	}

	for (uint i = 0; i < NUM_BIT_EXP; i++)
	{
		if (getBitExp(i).isBit1())
		{
			isExpAllZero = false;
			break;
		}
	}

	if (isExpAllZero && isFracAllZero)
		return true;

	return false;
}

bool QFloat::isDenormalize()
{
	bool isExpAllZero = true;
	bool isFracAllZero = true;

	for (uint i = 0; i < NUM_BIT_FRAC; i++)
	{
		if (getBitFrac(i).isBit1())
		{
			isFracAllZero = false;
			break;
		}
	}

	for (uint i = 0; i < NUM_BIT_EXP; i++)
	{
		if (getBitExp(i).isBit1())
		{
			isExpAllZero = false;
			break;
		}
	}

	if (isExpAllZero && !isFracAllZero)
		return true;

	return false;
}

bool QFloat::isInf()
{
	bool isExpAllOne = true;
	bool isFracAllZero = true;

	for (uint i = 0; i < NUM_BIT_FRAC; i++)
	{
		if (getBitFrac(i).isBit1())
		{
			isFracAllZero = false;
			break;
		}
	}

	for (uint i = 0; i < NUM_BIT_EXP; i++)
	{
		if (getBitExp(i).isBit0())
		{
			isExpAllOne = false;
			break;
		}
	}

	if (isExpAllOne && isFracAllZero)
		return true;

	return false;
}

bool QFloat::isNaN()
{
	bool isExpAllOne = true;
	bool isFracAllZero = true;

	for (uint i = 0; i < NUM_BIT_FRAC; i++)
	{
		if (getBitFrac(i).isBit1())
		{
			isFracAllZero = false;
			break;
		}
	}

	for (uint i = 0; i < NUM_BIT_EXP; i++)
	{
		if (getBitExp(i).isBit0())
		{
			isExpAllOne = false;
			break;
		}
	}

	if (isExpAllOne && !isFracAllZero)
		return true;

	return false;
}

void QFloat::setBitSign()
{
	m_data.setMSB();
}

Bit QFloat::getBitSign()
{
	return (m_data.getMSB());
}

void QFloat::setBitExp(uint idx)
{
	if (idx >= 0 && idx < NUM_BIT_EXP)
		m_data.setBit(idx + NUM_BIT_FRAC);
}

Bit QFloat::getBitExp(uint idx)
{
	if (idx >= 0 && idx < NUM_BIT_EXP)
		return m_data.getBit(idx + NUM_BIT_FRAC);
	return Bit(0);
}

void QFloat::setBitFrac(uint idx)
{
	if (idx >= 0 && idx < NUM_BIT_FRAC)
		m_data.setBit(idx);
}

Bit QFloat::getBitFrac(uint idx)
{
	if (idx >= 0 && idx < NUM_BIT_FRAC)
		return m_data.getBit(idx);
	return Bit(0);
}

std::string QFloat::getFraction()
{
	std::string fraction;
	for (uint i = 0; i < NUM_BIT_FRAC; i++)
		fraction += (char)getBitFrac(NUM_BIT_FRAC - 1 - i) + '0';
		
	return "0." + fraction;
}
std::string QFloat::getExp()
{
	BitArray exp(NUM_BIT_EXP);
	for (uint i = 0; i < NUM_BIT_EXP; i++)
		if (getBitExp(i).isBit1())
			exp.setBit(i);

	return exp.to_string();
}

std::vector<std::string> QFloat::normalize(const std::string dec)
{
	bool sign = StringMath(dec).isNegative();
	StringMath absDec = StringMath(dec).abs();
	std::string bits = Convert::DecToBin(absDec.to_string(), NUM_BIT_FRAC + std::abs(0 - BIAS + 1));
	StringMath absDecBin(bits);
	StringMath intPart = absDecBin.getInt();
	StringMath fracPart = absDecBin.getFraction();
	
	int exp = 0;
	std::string mantissa;

	// Check int part, before position point
	long posPoint = absDecBin.getPosPoint();

	if (posPoint == NO_POINT)
		posPoint = absDecBin.length();

	// Normalize & Inf, NaN
	if (intPart != 0)
	{
		long newPosPoint = 0;
		for (uint i = 0; i < posPoint; i++)
		{
			if (intPart[i] == '1')
			{
				newPosPoint = i + 1;
				break;
			}
		}

		for (long i = newPosPoint; i < absDecBin.length(); i++)
		{
			if (absDecBin[i] != '.')
				mantissa += absDecBin[i];
		}
		exp = posPoint - newPosPoint;
	}
	// Denormalize or have 1 after point
	else
	{
		for (uint i = posPoint + 1; i < absDecBin.length(); i++)
		{
			exp--;
			if (exp == 0 - BIAS + 1 || absDecBin[i] == '1')
				break;
		}

		long j = 0;
		for (long i = posPoint + std::abs(exp) + 1; i < absDecBin.length(); i++)
		{
			if (j == NUM_BIT_FRAC)
				break;
			mantissa += absDecBin[i];

			j++;
		}

		// Round
		if (j == NUM_BIT_FRAC)
		{
			BitArray tmp(mantissa);
			if (sign)	tmp = tmp - BitArray("1");
			else		tmp = tmp + BitArray("1");

			std::string resultTmp = tmp.to_string();
			long j = mantissa.size() - 1;
			for (long i = resultTmp.size() - 1; i >= 0; i--)
			{
				if (j < 0)
					break;
				mantissa[j--] = resultTmp[i];
			}
		}
	}

	std::vector<std::string> result(3);
	result[0] = std::to_string(sign);
	result[1] = std::to_string(exp);
	
	if (mantissa.size() == 0)
		mantissa.push_back('0');
	result[2] = mantissa;

	return result;
}