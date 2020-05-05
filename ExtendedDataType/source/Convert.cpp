#include "Convert.h"
#include <map>

std::string Convert::DecToBin(const std::string& decimal, uint precision)
{
	if (!checkValidDec(decimal))
		throw std::logic_error("error format");

	StringMath dec(decimal);
	StringMath absDec(dec.abs());
	StringMath intPart = absDec.getInt();
	StringMath fracPart = absDec.getFraction();

	// Convert int part to bin
	std::string binInt;
	while (intPart != 0)
	{
		binInt += (intPart % 2).to_string();
		intPart = intPart.div(2, 0);
	}
	std::reverse(binInt.begin(), binInt.end());


	//Covert fraction part to bin
	std::string binFrac;
	if (absDec.getPosPoint() != NO_POINT)
	{
		if (precision != 0)
			binFrac.push_back('.');

		uint countPre = 0;
		while (countPre < precision)
		{
			fracPart = fracPart * 2;
			binFrac += fracPart.getInt().to_string();
			fracPart = fracPart.getFraction().to_string();

			countPre++;
		}
		
	}

	std::string result;
	if (dec.isNegative())
		result.push_back('-');
	result += binInt + binFrac;

	return result;
}

std::string Convert::BinToDec(const std::string& bits, uint precision)
{
	if (!checkValidBits(bits))
		throw std::logic_error("error formar");

	StringMath bitStr(bits);
	StringMath absBits(bitStr.abs());
	
	StringMath intPart = absBits.getInt();
	StringMath fracPart = absBits.getFraction();
	long sizeIntPart = absBits.getNumDigitInt();
	long sizeFracPart = absBits.getNumDigitFractional();

	std::string result;
	if (bitStr.isNegative())
		result.push_back('-');

	// Convert bin int part to dec
	StringMath decInt(0);
	StringMath valPowInt(1);
	for (long i = sizeIntPart - 1; i >= 0; i--)
	{
		if (intPart[i] == '1')
			decInt = decInt + valPowInt;
		valPowInt = valPowInt * 2;
	}
	result += decInt.to_string();

	//Covert fraction part to bin
	StringMath binFrac(0);
	StringMath valPowFrac(1);
	if (absBits.getPosPoint() != NO_POINT)
	{
		for (long i = 0; i < sizeFracPart; i++)
		{
			valPowFrac = valPowFrac.div(2, precision);
			if (fracPart[i + 2] == '1')
				binFrac = binFrac + valPowFrac;
		}
		result.push_back('.');
		
		for (uint i = 2; i < fracPart.to_string().size(); i++)
			result += binFrac[i];
	}

	return result;
}

std::string Convert::BinToHex(const std::string& bits)
{
	if (!checkValidBits(bits))
		throw std::logic_error("error formart");

	std::map<std::string, char> mapBinToHex
	{
		{"0000", '0'}, {"0001", '1'}, {"0010", '2'}, {"0011", '3'},
		{"0100", '4'}, {"0101", '5'}, {"0110", '6'}, {"0111", '7'},
		{"1000", '8'}, {"1001", '9'}, {"1010", 'A'}, {"1011", 'B'},
		{"1100", 'C'}, {"1101", 'D'}, {"1110", 'E'}, {"1111", 'F'}
	};

	long lenNormalize = 0;
	uint i = 0;
	while (true)
	{
		if (bits.length() <= 4 * i)
			break;
		i++;
	}
	lenNormalize = 4 * i;

	StringMath tmp(bits);
	if (tmp.getPosPoint() != NO_POINT)
		return bits;
	
	std::string result;
	std::string group4Bits;

	for (long i = bits.length() - 1; i >= 0; i--)
	{
		group4Bits.push_back(bits[i]);

		if (group4Bits.length() == 4)
		{
			std::reverse(group4Bits.begin(), group4Bits.end());
			result.push_back(mapBinToHex[group4Bits]);
			group4Bits.clear();
		}
	}

	while (group4Bits.length() != 4)
		group4Bits.push_back('0');
	std::reverse(group4Bits.begin(), group4Bits.end());
	result.push_back(mapBinToHex[group4Bits]);

	std::reverse(result.begin(), result.end());
	return result;
}

std::string Convert::DecToHex(const std::string& str)
{
	std::string bits = DecToBin(str, 0);
	std::string result = BinToHex(bits);

	return result;
}

bool Convert::checkValidDec(const std::string& dec)
{
	StringMath s(dec);
	return true;
}

bool Convert::checkValidBits(const std::string& bits)
{
	StringMath s(bits);

	uint i = 0;
	if (s.isNegative()) i = 1;

	for (i; i < s.getPosPoint(); i++)
	{
		if (bits[i] > '1' || bits[i] < '0')
			return false;
	}

	for (i = s.getPosPoint() + 1; i < bits.length(); i++)
		if (bits[i] > '1' || bits[i] < '0')
			return false;

	return true;
}