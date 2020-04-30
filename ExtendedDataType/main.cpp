#include "StringMath.h"
#include <iostream>
#include <BitArray.h>
using namespace std;

int main()
{
	BitArray bit1("11110110110");
	BitArray bit2("111");
	BitArray bit3 = bit1 * bit2;
}