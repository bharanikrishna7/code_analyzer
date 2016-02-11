///////////////////////////////////////////////////////////////////////
// SemiExpression.cpp - collect tokens for analysis                  //
// Version:     1.0                                                  //
// Language:    C++, Visual Studio 2015                              //
// Application: Parser component, CSE687 - Object Oriented Design    //
// Author:      Venkata Chekuri, SUID : 356579351                    //
//              vbchekur@syr.edu                                     //
///////////////////////////////////////////////////////////////////////

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <exception>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include "SemiExp.h"
#include "../Tokenizer/Tokenizer.h"

using namespace Scanner;

SemiExp::SemiExp(Toker* pToker) : _pToker(pToker) {}

/* This function reads the input stream and loads the tokens to _tokens vector
 * ignoreFor variable is used to check for the condition of for loop. If there
 * is a for loop then 2 ';' symbols are ignored while checking for semi-expression
 * ending condition.
*/
bool SemiExp::get(bool clear)
{
  if (_pToker == nullptr)
    throw(std::logic_error("no Toker reference"));
  _tokens.clear();
  index = -1;
  int ignoreFor = 2;
  while (true)
  {
    std::string token = _pToker->getTok();
    if (token == "")
      break;
    _tokens.push_back(token);
	index++;
	if (_tokens.back() == "for")
		ignoreFor = 0;
	if (_tokens[index] == ";" && ignoreFor > 1)
	{
		ignoreFor++;
		return true;
	}
	if (token == ":" && _tokens.size() > 1)
	{
		if (_tokens[index - 1] == "public" || _tokens[index - 1] == "private" || _tokens[index - 1] == "protected")
			return true;
	}
	if (token == "{" || token == "}" || token == std::string(1, static_cast<char>(23)))
		return otherGetComp(token);
  }
  return false;
}

// Function to reduce the size of the get() function.
bool SemiExp::otherGetComp(std::string token)
{
	if (token == "{")
		return true;

	if (token == "}")
		return true;

	if (token == std::string(1, static_cast<char>(23)))
	{
		_tokens.pop_back();
		index--;
		return true;
	}

	return false;
}

// Returns a token at a specific index
Token& SemiExp::operator[](size_t n)
{
  if (n < 0 || n >= _tokens.size())
    throw(std::invalid_argument("index out of range"));
  return _tokens[n];
}

// Returns the size of the vector _tokens
size_t SemiExp::length()
{
  return _tokens.size();
}

/* Returns the index of an element which is asked to be found.
 * If found correct index will be returned and if not found then
 * the size of the vector is returned.
*/
size_t SemiExp::find(const std::string& findTok)
{
	size_t index = 0;
	for (; index < _tokens.size(); index++)
	{
		if (_tokens[index].compare(findTok) == 0)
			return index;
	}
	
	// Since size_t cannot contain -ve values hence setting index to
	// size of the vector _tokens.
	return index;
}

/* Having this function will enable external methods to add elements to
 * _tokens without directly exposing it.
*/
void SemiExp::push_back(const std::string& tok)
{
	_tokens.push_back(tok);
}

bool SemiExp::remove(const std::string& tok)
{
	if (std::find(_tokens.begin(), _tokens.end(), tok) != _tokens.end())
	{
		_tokens.erase(std::find(_tokens.begin(), _tokens.end(), tok));
		return true;
	}
	// Didnot erase that element since it was not found in the vector _tokens
	return false;
}

/* Removes the element at index which is passed to the function. If the index is
 * out of bounds then nothing is removed and this function will return false to 
 * notify no elemet has been removed.
 * Also here we are assuming that the index goes from 0 to _tokens.size()-1
*/
bool SemiExp::remove(size_t index)
{
	if (index >= 0 && index <= _tokens.size())
	{
		_tokens.erase(_tokens.begin() + index);
		return true;
	}
	// Didnot erase that element since it was not found in the vector _tokens
	return false;
}

// This function will remove all the leading whitespaces of all the tokens in the vector _tokens
void SemiExp::trimFront()
{
	for (size_t index = 0; index < _tokens.size(); index++)
	{ 
		std::string tok = _tokens[index];
		tok = tok.substr(tok.find_first_not_of('\n'));
		tok = tok.substr(tok.find_first_not_of('\r'));
		tok = tok.substr(tok.find_first_not_of('\t'));
		tok = tok.substr(tok.find_first_not_of(' '));

		_tokens[index] = tok;
	}
}

// This function will remove all the elements present in the vector _tokens
void SemiExp::clear()
{
	_tokens.clear();
}

/* This function will change the case of all the characters (alphabets) in all the _tokens
 * to lower case
*/
void SemiExp::toLower()
{
	for (size_t index = 0; index < _tokens.size(); index++)
	{
		std::string tok = _tokens[index];
		std::transform(tok.begin(), tok.end(), tok.begin(), ::tolower);
		_tokens[index] = tok;
	}
}


// Prints all the tokens in a semi-expressions
std::string SemiExp::show(bool showNewLines)
{
	std::string accum = std::string("");
	accum.append("\n  ");
	if (showNewLines)
	{
		for (auto token : _tokens)
			if (token != "\n")
				accum.append(token + "\n  ");
			else
				accum.append("\\n\n ");
		return accum;
	}

	for (auto token : _tokens)
		if (token != "\n")
			accum.append(token + "\n  ");
	accum.append("\n");
	return accum;
}

/* Verifies whether a string is only whitespaces. This is used to make sure
 * no semi-expression which has only whitespace tokens are not printed when
 * show() is called
*/
bool SemiExp::isNotOnlySpace(std::string aString)
{
	std::string checkOnlySpaces = aString.substr(aString.find_first_not_of(' '), aString.find_last_not_of(' '));
	checkOnlySpaces = checkOnlySpaces.substr(checkOnlySpaces.find_first_not_of('\n'), checkOnlySpaces.find_last_not_of('\n'));
	checkOnlySpaces = checkOnlySpaces.substr(checkOnlySpaces.find_first_not_of('\t'), checkOnlySpaces.find_last_not_of('\t'));
	checkOnlySpaces = checkOnlySpaces.substr(checkOnlySpaces.find_first_not_of('\r'), checkOnlySpaces.find_last_not_of('\r'));

	if (checkOnlySpaces == "")
		return false;
	else
		return true;
}

// No function in current release.
bool SemiExp::merge(const std::string& firstTok, const std::string& secondTok)
{
	return true;
}

//#define TEST_SEMIEXPR
#ifdef TEST_SEMIEXPR

int main()
{
  Toker toker;
  std::string fileSpec = "../Tokenizer/Tokenizer.cpp";
  std::fstream in(fileSpec);
  if (!in.good())
  {
    std::cout << "\n  can't open file " << fileSpec << "\n\n";
    return 1;
  }
  toker.attach(&in);

  SemiExp semi(&toker);
  while(semi.get())
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
	std::cout << "\n\n";
  }

  return 0;
}
#endif