#ifndef SEMIEXPRESSION_H
#define SEMIEXPRESSION_H
//////////////////////////////////////////////////////////////////////////////
// SemiExpression.h - collect tokens for analysis							//
// Version :	  1.0                                                       //
// Language:      Visual C++, Visual Studio 2015							//
// Platform:      MSI GE62 2QD, Core-i7, Windows 10							//
// Application:   Code Parser with AST										//
// Author:		  Venkata Chekuri, SUID : 356579351							//
//                vbchekur@syr.edu											//
//////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides a public SemiExp class that collects and makes
* available sequences of tokens.  SemiExp uses the services of a Toker
* class to acquire tokens.  Each call to SemiExp::get() returns a
* sequence of tokens that ends in {, }, ;, and '\n' if the line begins
* with #.  There are three additiona termination conditions: a sequence
* of tokens that ends with : and the immediately preceding token is
* public, protected, or private.
*
* Each semiexpression returns just the right tokens to analyze one
* C++ grammatical construct, e.g., class definition, function definition,
* declaration, etc.
*
* Required Files:
* ---------------
* SemiExpression.h, SemiExpression.cpp, Tokenizer.h, Tokenizer.cpp,
* Utilities.h, Utilities.cpp
*
* Build Command:
* --------------
* devenv Project1.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 8 Feb 16
* - first release
* ver 1.1 : 31 May 16
* - Updated comments so they are easier to understand.
*/

#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <exception>
#include "ITokCollection.h"
#include "../Tokenizer/Tokenizer.h"
#include "../Utilities/Utilities.h"
namespace Scanner
{
	class SemiExp : public ITokCollection
	{
	public:
		SemiExp(Toker* pToker = nullptr);
		SemiExp(const SemiExp&) = delete;
		SemiExp& operator=(const SemiExp&) = delete;
		bool get(bool clear = true);
		std::string& operator[](size_t n);
		std::string operator[](size_t n) const;
		size_t length();
		size_t find(const std::string& tok);
		void trimFront();
		void toLower();
		bool remove(const std::string& tok);
		bool remove(size_t n);
		void push_back(const std::string& tok);
		void clear();
		bool isComment(const std::string& tok);
		std::string show(bool showNewLines = false);
		size_t currentLineCount();
		bool getVerbose();
		void setVerbose(bool paramV);
	private:
		bool isTerminator(const std::string& tok);
		bool getHelper(bool clear = false);
		bool isSemiColonBetweenParens();
		bool hasFor = false;
		std::vector<std::string> _tokens;
		Toker* _pToker;
		bool VERBOSE = false;
	};
}
#endif
