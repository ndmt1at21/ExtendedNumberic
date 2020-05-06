#include "StringMath.h"
#include "Convert.h"
#include "QInt.h"
#include <iostream>
#include <BitArray.h>
#include "TestConsole.h"

using namespace std;

int main()
{
	QInt a(7);
	QInt b(-3);
	QInt c = a % b;
	std::string d = c.to_dec();
}