#ifndef SEMIEXPRESSION_H
#define SEMIEXPRESSION_H
///////////////////////////////////////////////////////////////////////
// SemiExpression.h - collect tokens for analysis                    //
// Version: 1.0                                                      //
// Language:    C++, Visual Studio 2015                              //
// Platform:    MSI GE62 - 2QD, Windows 10                           //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Author:      Venkata Chekuri, SUID : 356579351                    //
//              vbchekur@syr.edu                                     //
///////////////////////////////////////////////////////////////////////
/*
 * Credits:
 * --------
 * I would like to give credits to Prof. Jim W Fawcett Syracuse University
 * , CST 4-187) for providing a started code and explaining how the Lexical
 * Scanner should work. 
 *
 * Package Operations:
 * -------------------
 * This package provides a public SemiExp class that collects and makes
 * available sequences of tokens.  SemiExp uses the services of a Toker
 * class to acquire tokens.  Each call to SemiExp::get() returns a 
 * sequence of tokens that ends in { or } or ; or '\n' if the line begins 
 * with #. 
 *
 *
 * Build Process:
 * --------------
 * Required Files: 
 *   SemiExpression.h, SemiExpression.cpp, Tokenizer.h, Tokenizer.cpp
 * 
 * Build Command: devenv Project1.sln /rebuild debug
 *
 * Maintenance History:
 * --------------------
 * ver 1.0 : 08 Feb 2016
 * - Initial Release
 * - Build on top of Prof. Jim fawcett's 02 Feb 2016 code.
 *
 * Planned Additions and Changes:
 * ------------------------------
 * - Preformance improvements while printing output. Currently this code
 * scans for whitespaces before printing a semi expression, since printing
 * a semi expression which has only whitespace is useless. There are four
 * different trims happening in isNotOnlySpace() to check for each of the
 * whitespace condition space, \n, \t, \r. This needs to be done in a single
 * check.
 *
*/

#include <vector>
#include "../Tokenizer/Tokenizer.h"
#include "ITokCollection.h"
#include "../ITokCollection/ITokCollection.h"

namespace Scanner
{
  using Token = std::string;

  class SemiExp : public ITokCollection
  {
  public:
    SemiExp(Toker* pToker = nullptr);
    SemiExp(const SemiExp&) = delete;
    SemiExp& operator=(const SemiExp&) = delete;
    bool get(bool clear = true);
    Token& operator[](size_t n);
    size_t length();
	size_t find(const std::string& tok);
	void push_back(const std::string& tok);
	bool remove(const std::string& tok);
	bool remove(size_t);
	void trimFront();
	void clear();
	void toLower();
	bool merge(const std::string& firstTok, const std::string& secondTok);
	std::string show(bool showNewLines = false);
	bool isNotOnlySpace(std::string str);
	bool otherGetComp(std::string token);
  private:
    std::vector<Token> _tokens;
    Toker* _pToker;
	int index;
  };
}
#endif
