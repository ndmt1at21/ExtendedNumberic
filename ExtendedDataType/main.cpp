#include "StringMath.h"
#include "Convert.h"
#include <iostream>
#include <BitArray.h>

using namespace std;

int main()
{
	StringMath str1("01011010");
	std::string cvt = Convert::BinToHex(str1.to_string());
}