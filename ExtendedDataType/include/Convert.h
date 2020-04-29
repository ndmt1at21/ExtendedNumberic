#pragma once
#include "StringMath.h"
#include "BitArray.h"

class Convert
{
public:
	static std::string DecToBin(const std::string& decimal);
	static std::string BinToDec(const std::string& bits);
	
	static std::string BinToHex(const std::string& bits);
	static std::string DecToHex(const std::string& str);
};

