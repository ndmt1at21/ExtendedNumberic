#include "Convert.h"

std::string Convert::DecToBin(const std::string& decimal, uint precision)
{
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

std::string Convert::BinToDec(const std::string& bits)
{
	std::string result;
	return result;
}

std::string Convert::BinToHex(const std::string& bits)
{
	std::string result;
	return result;
}

std::string Convert::DecToHex(const std::string& str)
{
	std::string result;
	return result;
}