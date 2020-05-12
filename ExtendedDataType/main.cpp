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
	if (argc == 4)
	{
		std::string linkInput = argv[1];
		std::string linkOutput = argv[2];
		std::string type = argv[3];

		if (type == "1")
			testQInt(linkInput, linkOutput);
		else if (type == "2")
			testQFloat(linkInput, linkOutput);
	};
}