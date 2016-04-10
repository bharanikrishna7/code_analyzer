#ifndef UTILITIES_H
#define UTILITIES_H
//////////////////////////////////////////////////////////////////////////////
// Utilities.h - small, generally usefule, helper classes					//
// Version :	  1.0                                                       //
// Language:      Visual C++, Visual Studio 2015							//
// Platform:      MSI GE62 2QD, Core-i7, Windows 10							//
// Application:   Parallel Dependency Analysis								//
// Author:		  Venkata Chekuri, SUID : 356579351							//
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
 * ver 1.1 : 8 February 16
 * - first release
 *
 * Credits:
 * --------
 * I would like to give credits to Prof. Jim W Fawcett Syracuse University
 * , CST 4-187) for providing a started code and explaining how Parallel 
 * Dependency Analysis project should work.
 *
 */
#include <string>
#include <vector>
#include <sstream>
#include <functional>
#include <algorithm>  
#include <cctype>
#include <locale>

namespace Utilities
{
  class StringHelper
  {
  public:
    static std::vector<std::string> split(const std::string& src);
    static void Title(const std::string& src, char underline = '-');
    static void title(const std::string& src);
	static std::string ltrim(std::string &s);
	static std::string rtrim(std::string &s);
	static std::string lrtrim(std::string &s);
  };

  void putline();

  template <typename T>
  class Converter
  {
  public:
    static std::string toString(const T& t);
    static T toValue(const std::string& src);
  };

  template <typename T>
  std::string Converter<T>::toString(const T& t)
  {
    std::ostringstream out;
    out << t;
    return out.str();
  }

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
