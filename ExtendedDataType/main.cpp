#include "StringMath.h"
#include "Convert.h"
#include "QInt.h"
#include <iostream>
#include <BitArray.h>

using namespace std;

int main()
{
	QInt a("A", MODE::hex);
	QInt b("123456");
	QInt c = a + b;

	std::string e = a.to_string();
	std::string d = b.to_dec();
	std::string result = c.to_string();
}