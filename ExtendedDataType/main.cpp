#include "StringMath.h"
#include "Convert.h"
#include <iostream>
#include <BitArray.h>

using namespace std;

int main()
{
	BitArray bit1("1111");
	BitArray bit2("1101");
	BitArray bit3 = (bit1 * bit2);
	std::string str = bit3.to_string();}