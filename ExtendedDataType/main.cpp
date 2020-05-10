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
	BitArray bit1("101");
	BitArray bit2("1010");
	std::string b = Convert::BinToDec((bit1-bit2).to_string(), 0);
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