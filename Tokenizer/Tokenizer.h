#pragma once
#ifndef TOKENIZER_H
#define TOKENIZER_H
//////////////////////////////////////////////////////////////////////////////
// Tokenizer.h - read words from a std::stream								//
// Version :	  1.0                                                       //
// Language:      Visual C++, Visual Studio 2015							//
// Platform:      MSI GE62 2QD, Core-i7, Windows 10							//
// Application:   Code Parser with AST										//
// Author:		  Venkata Bharani Krishna Chekuri						    //
//                vbchekur@syr.edu											//
//////////////////////////////////////////////////////////////////////////////
/*
* Package Operations:
* -------------------
* This package provides a public Toker class and private ConsumeState class.
* Toker reads words from a std::stream, throws away whitespace and optionally
* throws away comments.
*
* Toker returns words from the stream in the order encountered.  Quoted
* strings and certain punctuators and newlines are returned as single tokens.
*
* This is a new version, based on the State Design Pattern.  Older versions
* exist, based on an informal state machine design.
*
* Build Process:
* --------------
* Required Files: Tokenizer.h, Tokenizer.cpp
*
* Build Command: devenv Tokenizer.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 8 February 16
* - first release.
* ver 1.1 : 31 May 16
* - Proper commenting of code.
* - Updated Test Stub to be easier to understand.
* - Removed title function since Title function covers it's functionality
*/

#include <iosfwd>
#include <string>
#include <cctype>
#include <vector>
#include <iostream>

namespace Scanner
{
	class ConsumeState;    // private worker class
	struct Context;        // private shared data storage

	// Toker Class
	class Toker
	{
	public:
		Toker();
		~Toker();
		bool canRead();
		bool getVerbose(); 
		std::string getTok();
		size_t currentLineCount();
		void setVerbose(bool paramV);
		Toker(const Toker&) = delete;
		bool attach(std::istream* pIn);
		bool isComment(const std::string& tok);
		Toker& operator=(const Toker&) = delete;
		void returnComments(bool doReturnComments = true);
		void setSpecialTokens(const std::string& commaSeparatedString);
	private:
		ConsumeState* pConsumer;
		Context* _pContext;
		bool VERBOSE;
	};
}
#endif