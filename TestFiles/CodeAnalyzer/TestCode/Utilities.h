#ifndef UTILITIES_H
#define UTILITIES_H
//////////////////////////////////////////////////////////////////////////////
// Utilities.h - small, generally useful, helper classes					//
// Version :	  1.0                                                       //
// Language:      Visual C++, Visual Studio 2015							//
// Platform:      MSI GE62 2QD, Core-i7, Windows 10							//
// Application:   Code Parser with AST										//
// Author:		  Venkata Bharani Krishna Chekuri							//
//                vbchekur@syr.edu											//
//////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides classes StringHelper and Converter and a global
* function putline().  This class will be extended continuously for
* awhile to provide convenience functions for general C++ applications.
*
* Build Process:
* --------------
* Required Files: Utilities.h, Utilities.cpp
*
* Build Command: devenv Utilities.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 8 February 16
* - first release.
* ver 1.1 : 5 May 16
* - Proper commenting of code.
* - Updated Test Stub to be easier to understand.
* - Removed functions : title & test stub display helpers.
*	> From now on use Title instead of title function.
*	> Test Stub display helper functions removing will have
*	  no effect on other dependant files.
*/
#include <ctime>
#include <string>
#include <vector>  
#include <cctype>
#include <locale>
#include <cctype>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <functional>

namespace Utilities
{
	class StringHelper
	{
	public:
		static std::vector<std::string> split(const std::string& src);
		static void Title(const std::string& src, char underline = '-');
		static std::string ltrim(std::string &s);
		static std::string rtrim(std::string &s);
		static std::string lrtrim(std::string &s);
	};

	void putline();
	
	class Timer {
	public:
		bool StartClock();
		int  StopClock();
	private:
		int time = 0;
	};

	// Coverter Class 
	template <typename T>
	class Converter
	{
	public:
		static std::string toString(const T& t);
		static T toValue(const std::string& src);
	};

	////////////////////////////////////////////////////
	// Function to convert Variable of type T to string
	template <typename T>
	std::string Converter<T>::toString(const T& t)
	{
		std::ostringstream out;
		out << t;
		return out.str();
	}

	////////////////////////////////////////////////////
	// Function to convert string to variable of type T
	template<typename T>
	T Converter<T>::toValue(const std::string& src)
	{
		std::istringstream in(src);
		T t;
		in >> t;
		return t;
	}
}
#endif
