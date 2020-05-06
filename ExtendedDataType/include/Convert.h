#pragma once
#include "StringMath.h"
#include "BitArray.h"

class Convert
{
public:
	static std::string DecToBin(const std::string& decimal, uint precision);
	static std::string BinToDec(const std::string& bits, uint precision);
	
	static std::string BinToHex(const std::string& bits);
	static std::string DecToHex(const std::string& str);
	static std::string HexToBin(const std::string& hex);
};

