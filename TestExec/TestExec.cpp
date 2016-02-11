///////////////////////////////////////////////////////////////////////
// TestExec.cpp - Package for the testing and verifying the code     //
// Version:     1.0                                                  //
// Language:    C++, Visual Studio 2015                              //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Author:      Venkata Chekuri, SUID : 356579351                    //
//              vbchekur@syr.edu                                     //
///////////////////////////////////////////////////////////////////////

#include "../Tokenizer/Tokenizer.h"
#include "../SemiExp/SemiExp.h"
#include "TestExec.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace Scanner;

Scanner::TestExec::TestExec(){}

Scanner::TestExec::~TestExec(){}

void TestExec::Test4()
{
	std::cout << "\n ------------------------------";
	std::cout << "\n Demonstrating Requirement #4:";
	std::cout << "\n ------------------------------";
	std::cout << "\n \n Reading tokens from File : ../Test/test4.test\n";
	std::string fileSpec = "./Test/test4.test";

	std::ifstream in(fileSpec);
	if (!in.good())
	{
		std::cout << "\n  can't open " << fileSpec << "\n\n";
		fileSpec = "../Test/test4.test";
	}


	Toker toker;
	toker.attach(&in);
	do
	{
		std::string tok = toker.getTok();
		/* Skipping the additional token which has been added to
		* allow proper differenciation between some special cases
		* ex : \n followed by #
		*/
		if (tok == std::string(1, (char)23))
			continue;

		if (tok == "\n")
			tok = "newline";
		std::cout << "\n -- " << tok;
	} while (in.good());

	std::cout << "\n\n\n ";
	return;
}

void TestExec::Test5()
{
	std::cout << "\n ------------------------------";
	std::cout << "\n Demonstrating Requirement #5:";
	std::cout << "\n ------------------------------";

	std::cout << "\n\n Calling getTok() in Tokenizer's toker produces a single token. \n This can be seen by looking at Test Demonstration of Requirement #4\n\n\n ";
}

void TestExec::Test6()
{
	std::cout << "\n ------------------------------";
	std::cout << "\n Demonstrating Requirement #6:";
	std::cout << "\n ------------------------------";

	std::cout << "\n\n This Requirement will be demonstrated with Requirement #7\n\n\n ";
}

void TestExec::Test7()
{
	std::cout << "\n ------------------------------";
	std::cout << "\n Demonstrating Requirement #7:";
	std::cout << "\n ------------------------------";

	Toker toker;
	toker.reset();
	std::string fileSpec = "./Test/test7.test";
	std::cout << "\n\n Reading from File " << fileSpec << std::endl;
	std::fstream in(fileSpec);
	if (!in.good())
	{
		std::cout << "\n  can't open file " << fileSpec << "\n\n";
		return;
	}
	toker.attach(&in);

	SemiExp semi(&toker);
	while (semi.get())
	{
		std::string tokensString = semi.show();
		if (semi.isNotOnlySpace(tokensString))
		{
			std::cout << "\n  -- semiExpression --";
			std::cout << tokensString;
		}
	}

	if (semi.length() > 0)
	{
		std::string tokensString = semi.show();
		if (semi.isNotOnlySpace(tokensString))
		{
			std::cout << "\n  -- semiExpression --";
			std::cout << tokensString;
		}
		std::cout << "\n\n\n ";
	}
	return;
}

void TestExec::Test8()
{
	std::cout << "\n ------------------------------";
	std::cout << "\n Demonstrating Requirement #8:";
	std::cout << "\n ------------------------------";
	
	Toker toker;
	toker.reset();
	std::string fileSpec = "./Test/test8.test";
	std::cout << "\n\n Reading from File " << fileSpec << std::endl;
	std::fstream in(fileSpec);
	if (!in.good())
	{
		std::cout << "\n  can't open file " << fileSpec << "\n\n";
		return;
	}
	toker.attach(&in);

	SemiExp semi(&toker);
	while (semi.get())
	{
		std::string tokensString = semi.show();
		if (semi.isNotOnlySpace(tokensString))
		{
			std::cout << "\n  -- semiExpression --";
			std::cout << tokensString;
		}
	}

	if (semi.length() > 0)
	{
		std::string tokensString = semi.show();
		if (semi.isNotOnlySpace(tokensString))
		{
			std::cout << "\n  -- semiExpression --";
			std::cout << tokensString;
		}
		std::cout << "\n\n ";
	}
	std::cout << "\n ";
}

void TestExec::Test9()
{
	std::cout << "\n ------------------------------";
	std::cout << "\n Demonstrating Requirement #8:";
	std::cout << "\n ------------------------------";

	std::vector<std::string> vec = { "VEC1", "VEC2", "vec3", "    VEC4" };
	std::cout << "\n\n Pushing the following tokens to Semi Expression which implements iTokCollection.h interface\n ";

	for (int index = 0; index < vec.size(); index++)
	{
		std::cout << vec[index];
		if (index < vec.size() - 1)
			std::cout << ", ";
	}
	std::cout << std::endl;

	SemiExp semi;
	for (int index = 0; index < vec.size(); index++)
		semi.push_back(vec[index]);
	
	std::cout << "\n\n Semi Expression is populated with the above displayed tokens";

	std::cout << "\n Demonstrating toLower()\n ";
	semi.toLower();
	for (int index = 0; index < semi.length(); index++)
	{
		std::cout << semi[index];
		if (index < semi.length() - 1)
			std::cout << ", ";
	}
	std::cout << std::endl;
	
	std::cout << "\n Demonstrating Length() : ";
	std::cout << "\n Length of semi-expression = " << semi.length();
	
	std::cout << "\n Demonstrating trimFront() :\n ";
	semi.trimFront();
	for (int index = 0; index < semi.length(); index++)
	{
		std::cout << semi[index];
		if (index < semi.length() - 1)
			std::cout << ", ";
	}
	std::cout << std::endl;

	Test9Contd();
}

void TestExec::Test9Contd()
{
	SemiExp semi;
	std::vector<std::string> vec = { "vec1", "vec2", "vec3", "vec4" };

	for (int index = 0; index < vec.size(); index++)
		semi.push_back(vec[index]);

	std::cout << "\n Demonstrating find() :";
	std::cout << "Find vec3 in Semi = " << semi.find("vec3");
	std::cout << std::endl;

	std::cout << "\n Demonstrating remove()";
	semi.remove(1);
	std::cout << "\n Removing element at index #1\n ";
	for (int index = 0; index < semi.length(); index++)
	{
		std::cout << semi[index];
		if (index < semi.length() - 1)
			std::cout << ", ";
	}
	std::cout << std::endl;

	std::cout << "\n Demonstrating clear() :";
	semi.clear();
	std::cout << "\n Size of semi-expression = " << semi.length();
	std::cout << "\n\n\n ";
}

void TestExec::Test10()
{
	std::cout << "\n This Requirement is satisfied by this code amd it is evident from the demonstration of previous requirements.";
	std::cout << "\n But if it still required, a complete test will be run for \"Tokenizer.cpp\". Do you still want to run this test ?";
	std::cout << "\n (Y) <- Default or (n)\n ";
	std::string c = "y";
	std::getline(std::cin, c);

	if (!(c[0] == 'n' || c[0] == 'N') || c[0] == '\n')
	{
		Toker toker;
		toker.reset();
		std::string fileSpec = "./Tokenizer/Tokenizer.cpp";
		std::fstream in(fileSpec);
		if (!in.good())
		{
			std::cout << "\n  can't open file " << fileSpec << "\n\n";
			return;
		}
		toker.attach(&in);

		SemiExp semi(&toker);
		while (semi.get())
		{
			std::string tokensString = semi.show();
			// Making sure that no empty semi - expressions are printed
			if (semi.isNotOnlySpace(tokensString))
			{
				std::cout << "\n  -- semiExpression --";
				std::cout << tokensString;
			}
		}

		/*
		May have collected tokens, but reached end of stream
		before finding SemiExp terminator.
		*/
		if (semi.length() > 0)
		{
			std::string tokensString = semi.show();
			// Making sure that no empty semi expressions are printed
			if (semi.isNotOnlySpace(tokensString))
			{
				std::cout << "\n  -- semiExpression --";
				std::cout << tokensString;
			}
			std::cout << "\n\n ";
		}
	}
}

#define TEST_TESTEXEC
#ifdef TEST_TESTEXEC

int main()
{
	std::cout << "\n .............\n .............\n BEGIN TESTING\n .............\n .............\n\n";
	system("pause");
	TestExec test;
	test.Test4();
	system("pause");
	test.Test5();
	system("pause");
	test.Test6();
	system("pause");
	test.Test7();
	system("pause");
	test.Test8();
	system("pause");
	test.Test9();
	system("pause");
	test.Test10();
	system("pause");
	std::cout << "\n .................\n .................\n TESTING COMPLETED\n .................\n .................\n\n\n ";
	system("pause");
}
#endif // TEST_TESTEXEC
