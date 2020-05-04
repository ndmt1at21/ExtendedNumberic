#include "StringMath.h"
#include "Convert.h"
#include <iostream>
#include <BitArray.h>

using namespace std;

int main()
{
	StringMath str1("-10.51");
	std::string cvt = Convert::DecToBin(str1.to_string(), 100);
}