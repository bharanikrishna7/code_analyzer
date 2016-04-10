//////////////////////////////////////////////////////////////////////////////
// Utilities.h - small, generally usefule, helper classes					//
// Version :	  1.0                                                       //
// Language:      Visual C++, Visual Studio 2015							//
// Platform:      MSI GE62 2QD, Core-i7, Windows 10							//
// Application:   Parallel Dependency Analysis								//
// Author:		  Venkata Chekuri, SUID : 356579351							//
//                vbchekur@syr.edu											//
//////////////////////////////////////////////////////////////////////////////

#include <cctype>
#include <iostream>
#include "Utilities.h"

using namespace Utilities;

std::function<void(const std::string&)> Title =
  [](auto src) { StringHelper::Title(src, '='); };

std::function<void(const std::string&)> title =
  [](auto src) { StringHelper::Title(src, '-'); };

///////////////////////////////////////////
// Generate title format for string
void StringHelper::title(const std::string& src)
{
  std::cout << "\n  " << src;
  std::cout << "\n " << std::string(src.size() + 2, '-');
}

///////////////////////////////////////////
// Generate Title format for string
void StringHelper::Title(const std::string& src, char underline)
{
  std::cout << "\n  " << src;
  std::cout << "\n " << std::string(src.size() + 2, underline);
}

///////////////////////////////////////////
// Left Trim
std::string StringHelper::ltrim(std::string &s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), std::not1(std::ptr_fun<int, int>(std::isspace))));
	return s;
}

///////////////////////////////////////////
// Right Trim
std::string StringHelper::rtrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), std::not1(std::ptr_fun<int, int>(std::isspace))).base(), s.end());
	return s;
}

///////////////////////////////////////////
// Left Right Trim
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
    if(src[index] != '\0')
      temp += src[index];
  } while (index++ < src.size());
  if(temp.size() > 0)
    accum.push_back(temp);
  return accum;
}

///////////////////////////////////////////
// Print newline to console
void Utilities::putline()
{
  std::cout << "\n";
}

#ifdef TEST_UTILITIES
////////////////////
// TEST STUB
int main()
{
  Title("Testing Utilities Package");
  putline();

  title("test StringHelper::split(std::string)");

  std::string test = "a, \n, bc, de, efg, i, j k lm nopq rst";
  std::cout << "\n  test string = " << test;
  
  std::vector<std::string> result = StringHelper::split(test);
  
  std::cout << "\n";
  for (auto item : result)
  {
    if (item == "\n")
      std::cout << "\n  " << "newline";
    else
      std::cout << "\n  " << item;
  }
  std::cout << "\n";

  title("test std::string Converter<T>::toString(T)");

  std::string conv1 = Converter<double>::toString(3.1415927);
  std::string conv2 = Converter<int>::toString(73);
  std::string conv3 = Converter<std::string>::toString("a_test_string plus more");

  std::cout << "\n  Converting from values to strings: ";
  std::cout << conv1 << ", " << conv2 << ", " << conv3;
  putline();

  title("test T Converter<T>::toValue(std::string)");

  std::cout << "\n  Converting from strings to values: ";
  std::cout << Converter<double>::toValue(conv1) << ", ";
  std::cout << Converter<int>::toValue(conv2) << ", ";
  std::cout << Converter<std::string>::toValue(conv3);

  std::cout << "\n\n";
  return 0;
}
#endif
