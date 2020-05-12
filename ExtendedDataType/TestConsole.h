#pragma once
#include <fstream>
#include <string>
#include <QInt.h>
#include "QFloat.h"
#include <sstream>

using namespace std;

void testQInt(const std::string& linkInput, const std::string& linkOutput)
{
	std::ifstream inFile(linkInput);
	std::ofstream outFile(linkOutput);
	if (inFile.fail())
		return;

	std::string srcBase;
	std::string dstBase;
	std::string typeOperator;
	std::string line;
	std::vector<std::string> opt;

	while (true)
	{
		if (!getline(inFile, line, '\n'))
			break;

		stringstream ss(line);
		std::string tmp;
		while (getline(ss, tmp, ' '))
			opt.push_back(tmp);

		std::string result;
		bool compareResult = true;
		
		// Unary operator and Convert 
		if (opt.size() == 3)
		{ 
			QInt qInt;
			srcBase = opt[0];

			if (srcBase == "2")			qInt = QInt(opt[2], MODE::bin);
			else if (srcBase == "10")	qInt = QInt(opt[2], MODE::dec);
			else if (srcBase == "16")	qInt = QInt(opt[2], MODE::hex);

			// Unary operator
			typeOperator = opt[1];
			if (typeOperator == "~" || typeOperator == "-")
			{
				QInt tmp;
				if (typeOperator == "~")		tmp = ~qInt;
				else if (typeOperator == "-")	tmp = -qInt;

				if (srcBase == "2")				result = (~qInt).to_bin();
				else if (srcBase == "10")		result = (~qInt).to_dec();
				else if (srcBase == "16")		result = (~qInt).to_hex();
			}
			// Convert srcBase to dstBase
			else
			{
				dstBase = opt[1];
				if (dstBase == "2")			result = qInt.to_bin();
				else if (dstBase == "10")	result = qInt.to_dec();
				else if (dstBase == "16")	result = qInt.to_hex();
			}
		}
		// Binary operator
		else if (opt.size() == 4)
		{
			QInt n1;
			QInt n2;
			QInt qInt;

			srcBase = opt[0];
			typeOperator = opt[2];

			if (srcBase == "2")
			{
				n1 = QInt(opt[1], MODE::bin);
				n2 = QInt(opt[3], MODE::bin);
			}
			else if (srcBase == "10")
			{
				n1 = QInt(opt[1], MODE::dec);
				n2 = QInt(opt[3], MODE::dec);
			}
			else if (srcBase == "16")
			{
				n1 = QInt(opt[1], MODE::hex);
				n2 = QInt(opt[3], MODE::hex);
			}

			if (typeOperator == "+")		qInt = n1 + n2;
			else if (typeOperator == "-")	qInt = n1 - n2;
			else if (typeOperator == "*")	qInt = n1 * n2;
			else if (typeOperator == "/")	qInt = n1 / n2;
			else if (typeOperator == "~")	qInt = ~n1;
			else if (typeOperator == "&")	qInt = n1 & n2;
			else if (typeOperator == "|")	qInt = n1 | n2;
			else if (typeOperator == ">>")
			{
				stringstream ss(opt[3]);
				uint a;
				ss >> a;

				qInt = n1 >> a;
			}
			else if (typeOperator == "<<")
			{
				stringstream ss(opt[3]);
				uint a;
				ss >> a;

				qInt = n1 << a;
			}
			else if (typeOperator == "ror")
			{
				stringstream ss(opt[3]);
				uint a;
				ss >> a;

				qInt = n1.RoR(a);
			}
			else if (typeOperator == "rol")
			{
				stringstream ss(opt[3]);
				uint a;
				ss >> a;

				qInt = n1.RoL(a);
			}
			else if (typeOperator == "==")	compareResult = n1 == n2;
			else if (typeOperator == ">")	compareResult = n1 > n2;
			else if (typeOperator == "<")	compareResult = n1 < n2;
			else if (typeOperator == ">=")	compareResult = n1 >= n2;
			else if (typeOperator == "<=")	compareResult = n1 <= n2;

			if (srcBase == "2")			result = qInt.to_bin();
			else if (srcBase == "10")	result = qInt.to_dec();
			else if (srcBase == "16")	result = qInt.to_hex();
		}

		if (typeOperator == "==" || typeOperator == ">" || typeOperator == "<" ||
			typeOperator == ">=" || typeOperator == "<=")
		{
			outFile << compareResult << "\n";
		}
		else
		{
			outFile << result << "\n";
		}

		srcBase.clear();
		dstBase.clear();
		typeOperator.clear();
		line.clear();
		opt.clear();
		result.clear();
	}
}

void testQFloat(const std::string& linkInput, const std::string& linkOutput)
{
	std::ifstream inFile(linkInput);
	std::ofstream outFile(linkOutput);
	if (inFile.fail())
		return;

	std::string srcBase;
	std::string dstBase;
	std::string data;
	std::string line;

	while (true)
	{
		if (!getline(inFile, line, '\n'))
			break;

		stringstream ss(line);
		std::getline(ss, srcBase, ' ');
		std::getline(ss, dstBase, ' ');
		std::getline(ss, data, ' ');

		std::string result;

		if (srcBase == "10")
		{
			QFloat qFloat(data, MODE::dec);
			result = qFloat.to_bin();
		}
		else if (srcBase == "2")
		{
			QFloat qFloat(data, MODE::bin);
			result = qFloat.to_dec();
		}
		outFile << result << "\n";

		srcBase.clear();
		dstBase.clear();
		line.clear();
	}
}