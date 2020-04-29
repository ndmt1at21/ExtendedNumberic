#include "StringMath.h"
#include <iostream>
#include <BitArray.h>
using namespace std;

int main()
{
	BitArray bit1("110");
	BitArray bit2("101");
	BitArray bit3 = bit1 - bit2;
}