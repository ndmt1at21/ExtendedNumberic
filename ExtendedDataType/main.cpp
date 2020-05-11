#include "StringMath.h"
#include "Convert.h"
#include "QInt.h"
#include "QFloat.h"
#include <iostream>
#include <BitArray.h>
#include "TestConsole.h"

using namespace std;

int main(int argc, char* argv[])
{
	QInt a("-0");
	QInt b("-0");
	QInt c = a - b;
	std::string f = c.to_dec();
	/*if (argc == 4)
	{
		std::string linkInput = argv[1];
		std::string linkOutput = argv[2];
		std::string type = argv[3];

		if (type == "0")
			testQInt(linkInput, linkOutput);
		else if (type == "1")
			testQFloat(linkInput, linkOutput);
	};*/
}