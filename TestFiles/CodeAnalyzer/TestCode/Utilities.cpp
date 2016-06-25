//////////////////////////////////////////////////////////////////////////////
// Utilities.cpp - small, generally useful, helper classes					//
// Version :	  1.0                                                       //
// Language:      Visual C++, Visual Studio 2015							//
// Platform:      MSI GE62 2QD, Core-i7, Windows 10							//
// Application:   Code Parser with AST										//
// Author:		  Venkata Bharani Krishna Chekuri							//
//                vbchekur@syr.edu											//
//////////////////////////////////////////////////////////////////////////////

#include "Utilities.h"

using namespace Utilities;

//////////////////////////////////////////////////////////////////////
// Display Title with a character underline (default character is '_')
void StringHelper::Title(const std::string& src, char underline)
{
	std::cout << "\n " << src;
	std::cout << "\n " << std::string(src.size() + 1, underline);
}

//////////////////////////
// Function for Left Trim
std::string StringHelper::ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

///////////////////////////
// Function for Right Trim
std::string StringHelper::rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

////////////////////////////////
// Function for Left Right Trim
std::string StringHelper::lrtrim(std::string &s) {
	return ltrim(rtrim(s));
}

//////////////////////////////////////////////////////////////////
// Split a string at ',' and return these sub-strings as a vector
std::vector<std::string> StringHelper::split(const std::string& src)
{
	std::vector<std::string> accum;
	std::string temp;
	size_t index = 0;
	do
	{
		while ((isspace(src[index]) || src[index] == ',') && src[index] != '\n')
		{
			++index;
			if (temp.size() > 0)
			{
				accum.push_back(temp);
				temp.clear();
			}
		}
		if (src[index] != '\0')
			temp += src[index];
	} while (index++ < src.size());
	if (temp.size() > 0)
		accum.push_back(temp);
	return accum;
}

////////////////////////////////////////////
// Print newline to console (pointless TBH)
void Utilities::putline()
{
	std::cout << "\n";
}

///////////////////
// Start the timer 
bool Timer::StartClock() {
	time = clock();
	return true;
}

////////////////////////////////////////////////
// Stop Timer and Return the time elapsed in ms
int Timer::StopClock() {
	time = clock() - time;
	return time / double(CLOCKS_PER_SEC) * 1000;
}

#ifdef TEST_UTILITIES
///////////////////////
// Test Trim Functions
void test_trim() {
	StringHelper::Title("Test Trim Operations", '-');
	// The trim operation is returns reference so the object passed to it is modified
	// for testing purpose we are again ang again assigning values to the same string (trim)
	std::cout << "\n ^ is begining of string";
	std::cout << "\n $ is end of string\n";
	std::string trim = "\t\t This is the \ttrim test string\t\t";

	std::cout << "\n Original String is\t : ^" << trim << "$";
	std::cout << "\n Left Trim	\t : ^" << StringHelper::ltrim(trim) << "$";

	trim = "\t\t This is the \toriginal string\t\t";
	std::cout << "\n Right Trim\t\t : ^" << StringHelper::rtrim(trim) << "$";

	trim = "\t\t This is the \toriginal string\t\t";
	std::cout << "\n LR Trim\t\t : ^" << StringHelper::lrtrim(trim) << "$";

	std::cout << "\n\n ";
}

////////////////////////////
// Test Converter Functions
void test_converter() {
	StringHelper::Title("Test std::string Converter<T>::toString(T)",'-');

	std::string conv1 = Converter<double>::toString(3.1415927);
	std::string conv2 = Converter<int>::toString(73);
	std::string conv3 = Converter<std::string>::toString("a_test_string plus more");

	std::cout << "\n Converting from values to strings: ";
	std::cout << conv1 << ", " << conv2 << ", " << conv3;
	putline();

	StringHelper::Title("Test T Converter<T>::toValue(std::string)", '-');

	std::cout << "\n Converting from strings to values: ";
	std::cout << Converter<double>::toValue(conv1) << ", ";
	std::cout << Converter<int>::toValue(conv2) << ", ";
	std::cout << Converter<std::string>::toValue(conv3);

	std::cout << "\n\n ";
}

///////////////////////
// Test Split Function
void test_split() {
	StringHelper::Title("Test StringHelper::split(std::string)", '-');

	std::string test = "a, \n, bc, de, efg, i, j k lm nopq rst";
	std::cout << "\n  test string = " << test;

	std::vector<std::string> result = StringHelper::split(test);

	std::cout << "\n";
	for (auto item : result)
	{
		if (item == "\n")
			std::cout << "\n " << "newline";
		else
			std::cout << "\n " << item;
	}

	std::cout << "\n\n ";
}

//////////////////////
// Main Test Function
int main()
{
	Timer time;
	time.StartClock();
	
	StringHelper::Title("Testing Utilities Package", '=');
	putline();

	// Test string split
	test_split();
	// Test string converter
	test_converter();
	// Test string trim
	test_trim();

	std::cout << "\n [Execution Time] : " << time.StopClock() << " ms";
	std::cout << "\n ";
	system("pause");
	return 0;
}
#endif
