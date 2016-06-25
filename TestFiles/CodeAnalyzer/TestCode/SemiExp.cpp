//////////////////////////////////////////////////////////////////////////////
// SemiExpression.cpp - collect tokens for analysis							//
// Version :	  1.0                                                       //
// Language:      Visual C++, Visual Studio 2015							//
// Platform:      MSI GE62 2QD, Core-i7, Windows 10							//
// Application:   Code Parser with AST										//
// Author:		  Venkata Chekuri, SUID : 356579351							//
//                vbchekur@syr.edu											//
//////////////////////////////////////////////////////////////////////////////


#include "SemiExp.h"

using namespace Scanner;
using Token = std::string;

///////////////////////////////////////////////////////////
// initialize semiExpression with existing toker reference 
SemiExp::SemiExp(Toker* pToker) : _pToker(pToker) {}

/////////////////////////////////////////////
// returns position of tok in semiExpression 
size_t SemiExp::find(const std::string& tok)
{
	for (size_t i = 0; i < length(); ++i)
		if (_tokens[i] == tok)
			return i;
	return length();
}

///////////////////////////////////////
// push token onto back end of SemiExp 
void SemiExp::push_back(const std::string& tok)
{
	_tokens.push_back(tok);
}

////////////////////////////////////
// removes token passed as argument 
bool SemiExp::remove(const std::string& tok)
{
	std::vector<Token>::iterator iter = _tokens.begin();
	while (iter != _tokens.end())
	{
		if (tok == *iter)
		{
			_tokens.erase(iter);
			return true;
		}
		++iter;
	}
	return false;
}

///////////////////////////////////////////////////
// removes token at nth position of semiExpression 
bool SemiExp::remove(size_t n)
{
	if (n < 0 || n >= length())
		return false;
	std::vector<Token>::iterator iter = _tokens.begin() + n;
	_tokens.erase(iter);
	return true;
}

/////////////////////////////////////////////////
// removes newlines from front of semiExpression 
void SemiExp::trimFront()
{
	while (length() > 1)
	{
		if (_tokens[0] == "\n")
			remove(0);
		else
			break;
	}
}

//////////////////////////////////////
// transform all tokens to lower case 
void SemiExp::toLower()
{
	for (auto& token : _tokens)
	{
		for (auto& chr : token)
		{
			chr = tolower(chr);
		}
	}
}

/////////////////////////////
// clear contents of SemiExp 
void SemiExp::clear()
{
	if (VERBOSE) { std::cout << "\n [VERBOSE] : Clear contents of SemiExp "; }
	_tokens.clear();
}

////////////////////////////
// is this token a comment? 
bool SemiExp::isComment(const std::string& tok)
{
	return _pToker->isComment(tok);
}

///////////////////////////////////////////////
// return count of newlines retrieved by Toker 
size_t SemiExp::currentLineCount()
{
	if (_pToker == nullptr)
		return 0;
	/*
	*  Tokenizer has collected first non-state char when exiting eatChars()
	*  so we need to subtract 1 from the Toker's line count.
	*/
	return _pToker->currentLineCount() - 1;
}

/////////////////////////////////
// helps folding for expressions 
bool SemiExp::isSemiColonBetweenParens()
{
	size_t openParenPosition = find("(");
	size_t semiColonPosition = find(";");
	size_t closedParenPosition = find(")");

	if (openParenPosition < semiColonPosition && semiColonPosition < closedParenPosition)
		return true;
	return false;
}

///////////////////////////////////////////////////////
// fills semiExpression collection from attached toker 
bool SemiExp::get(bool clear)
{
	bool ok = getHelper(clear);

	if (hasFor && isSemiColonBetweenParens())
	{
		if (VERBOSE) { std::cout << "\n [VERBOSE] : Testing for for Loop Condition (Special Case)"; }
		getHelper(false);  // add test for loop termination
		getHelper(false);  // add counter increment
	}
	return ok;
}

/////////////////////////////////////////
// is token a SemiExpression terminator? 
bool SemiExp::isTerminator(const std::string& token)
{
	if (token == "{" || token == "}" || token == ";")
		return true;

	if (token == "\n")
	{
		trimFront();
		if (_tokens[0] == "#")
			return true;
	}

	if (length() < 2)
		return false;

	if (token == ":" && length() > 0 && _tokens[length() - 2] == "public")
		return true;

	if (token == ":" && length() > 0 && _tokens[length() - 2] == "protected")
		return true;

	if (token == ":" && length() > 0 && _tokens[length() - 2] == "private")
		return true;

	return false;
}

/////////////////////////////////////////////////////////
// does all the work of collecting tokens for collection 
bool SemiExp::getHelper(bool clear)
{
	hasFor = false;
	if (_pToker == nullptr)
		throw(std::logic_error("no Toker reference"));
	if (clear)
		_tokens.clear();
	while (true)
	{
		std::string token = _pToker->getTok();
		if (token == "")
			break;
		_tokens.push_back(token);

		if (token == "for")
			hasFor = true;

		if (isTerminator(token))
			return true;
	}
	return false;
}

////////////////////////////////////////
// read only indexing of SemiExpression 
Token SemiExp::operator[](size_t n) const
{
	if (n < 0 || n >= _tokens.size())
		throw(std::invalid_argument("index out of range"));
	return _tokens[n];
}

////////////////////////////////////////
// writeable indexing of SemiExpression 
Token& SemiExp::operator[](size_t n)
{
	if (n < 0 || n >= _tokens.size())
		throw(std::invalid_argument("index out of range"));
	return _tokens[n];
}

/////////////////////////////////////////////
// return number of tokens in semiExpression 
size_t SemiExp::length()
{
	return _tokens.size();
}

////////////////////////////////////////
// display collection tokens on console 
std::string SemiExp::show(bool showNewLines)
{
	std::ostringstream out;
	out << "\n  ";
	for (auto token : _tokens)
		if (token != "\n" || showNewLines == true)
			out << token << " ";
	out << "\n";
	return out.str();
}

/////////////////////////////////////
// set the value of VERBOSE variable
void SemiExp::setVerbose(bool paramV) {
	VERBOSE = paramV;
	_pToker->setVerbose(VERBOSE);
}

/////////////////////////////////////
// get the value of VERBOSE variable
bool SemiExp::getVerbose() {
	return VERBOSE;
}

#ifdef TEST_SEMIEXP
/* Include Namespace Utilities to use StringHelper class */
using namespace Utilities;

///////////////////////////
// Main Test Stub function
int main()
{
	Timer time;
	time.StartClock();

	StringHelper::Title("\n Testing SemiExp class", '=');
	
	Toker toker;
	toker.setVerbose(true);
	std::string fileSpec = "SemiExp.cpp";

	std::fstream in(fileSpec);
	if (!in.good())
	{
		std::cout << "\n Unable to read file \"" << fileSpec << "\"\n\n";
		return 1;
	}
	else
	{
		std::cout << "\n Processing file \"" << fileSpec << "\"\n";
	}
	toker.attach(&in);

	SemiExp semi(&toker);
	semi.setVerbose(true);
	while (semi.get())
	{
		std::cout << "\n -- semiExpression -- at line number " << semi.currentLineCount();
		std::cout << semi.show();
	}
	/*
	May have collected tokens, but reached end of stream
	before finding SemiExp terminator.
	*/
	if (semi.length() > 0)
	{
		std::cout << "\n  -- semiExpression --";
		semi.show(true);
	}
	std::cout << "\n\n\n [Execution Time] : " << time.StopClock() << " ms";
	std::cout << "\n\n ";
	system("pause");
	return 0;
}
#endif