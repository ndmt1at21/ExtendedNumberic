#include "StringMath.h"
#include "Convert.h"
#include "QInt.h"
#include <iostream>
#include <BitArray.h>

using namespace std;

int main()
{
	QInt a(100);
	QInt b(-1101);
	QInt c = a - b;

	std::string e = b.to_string();
	std::string d = b.to_dec();
	std::string result = c.to_string();
}