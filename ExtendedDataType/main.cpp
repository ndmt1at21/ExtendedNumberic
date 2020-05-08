#include "StringMath.h"
#include "Convert.h"
#include "QInt.h"
#include "QFloat.h"
#include <iostream>
#include <BitArray.h>
#include "TestConsole.h"

using namespace std;

int main()
{
	QFloat a("0.1");
	std::string b = a.to_dec();
}