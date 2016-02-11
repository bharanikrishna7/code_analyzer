/////////////////////////////////////////////////////////////////////
// Tokenizer.cpp - read words from a std::stream                   //
// Version:     1.0                                                //
// Language:    C++, Visual Studio 2015                            //
// Application: Parser component, CSE687 - Object Oriented Design  //
// Author:      Venkata Chekuri, SUID : 356579351                  //
//              vbchekur@syr.edu                                   //
/////////////////////////////////////////////////////////////////////


#include "Tokenizer.h"
#include <iostream>
#include <cctype>
#include <string>

namespace Scanner
{
  class ConsumeState
  {
  public:
    ConsumeState();
    ConsumeState(const ConsumeState&) = delete;
    ConsumeState& operator=(const ConsumeState&) = delete;
    virtual ~ConsumeState();
    void attach(std::istream* pIn) { _pIn = pIn; }
    virtual void eatChars() = 0;
    void consumeChars() {
      _pState->eatChars();
      _pState = nextState();
    }
    bool canRead() { return _pIn->good(); }
    std::string getTok() { return token; }
    bool hasTok() { return token.size() > 0; }
    ConsumeState* nextState();
	ConsumeState* Phase1Comp(char currChar);
	ConsumeState* Phase2Comp(char curChar, char chNext);
	ConsumeState* PhaseSpComp(char curChar, char chNext);
	void setShowComments(bool value);
	void reset();
	void setSpecialSingleChars(std::string);
	void setSpecialCharPairs(std::string);
  protected:
	bool showComments = false;
    static std::string token;
    static std::istream* _pIn;
    static int prevChar;
    static int currChar;
    static ConsumeState* _pState;
    static ConsumeState* _pEatCppComment;
    static ConsumeState* _pEatCComment;
	static ConsumeState* _pEatString;
	static ConsumeState* _pEatChar;
    static ConsumeState* _pEatWhitespace;
    static ConsumeState* _pEatPunctuator;
    static ConsumeState* _pEatAlphanum;
    static ConsumeState* _pEatNewline;
	static ConsumeState* _pEatSpecialSingle;
	static ConsumeState* _pEatSpecialDouble;
	static bool first;
	std::vector<char> _specialSingle = { '<', '>', '[', ']', '(', ')', '{', '}', ':', '=', '+', '-', '*' };
	std::vector<std::string> _specialDouble = { "<<", ">>", "::", "++", "--", "==", "+=", "-=", "*=", "/=", "\\n" };
  };
}

using namespace Scanner;

int ConsumeState::prevChar;
int ConsumeState::currChar;

std::string ConsumeState::token;
std::istream* ConsumeState::_pIn = nullptr;
ConsumeState* ConsumeState::_pState = nullptr;
ConsumeState* ConsumeState::_pEatCppComment = nullptr;
ConsumeState* ConsumeState::_pEatCComment = nullptr;
ConsumeState* ConsumeState::_pEatString = nullptr;
ConsumeState* ConsumeState::_pEatChar = nullptr;
ConsumeState* ConsumeState::_pEatWhitespace = nullptr;
ConsumeState* ConsumeState::_pEatPunctuator = nullptr;
ConsumeState* ConsumeState::_pEatAlphanum = nullptr;
ConsumeState* ConsumeState::_pEatSpecialSingle = nullptr;
ConsumeState* ConsumeState::_pEatSpecialDouble = nullptr;
ConsumeState* ConsumeState::_pEatNewline;
bool ConsumeState::first = true;

void testLog(const std::string& msg);

void ConsumeState::reset()
{
	first = true;
}

void ConsumeState::setShowComments(bool value)
{
	showComments = value;
}

/* Depending on currChar and chNext determines what will be the next state of
 * the program
*/
ConsumeState* ConsumeState::nextState()
{
  if (!(_pIn->good()))
  {
    return nullptr;
  }
  int chNext = _pIn->peek();
  if (chNext == EOF)
  {
    _pIn->clear();
  }
  if (currChar == '\"' || currChar == '\'' || (std::isspace(currChar) && currChar != '\n'))
	  return Phase1Comp(currChar);
  
  if ((currChar == '/' && chNext == '/') || (currChar == '/' && chNext == '*') || (std::isalnum(currChar) || (currChar == '_' && std::isalnum(_pIn->peek()))))
	  return Phase2Comp(currChar, chNext);

  if ((std::find(_specialDouble.begin(), _specialDouble.end(), std::string(1, currChar).append(std::string(1, chNext))) != _specialDouble.end()) ||
	  (std::find(_specialSingle.begin(), _specialSingle.end(), currChar) != _specialSingle.end()))
	  return PhaseSpComp(currChar, chNext);
 
  if (ispunct(currChar))
  {
	  testLog("state: eatPunctuator");
	  return _pEatPunctuator;
  }

  if (currChar == '\n')
  {
	  testLog("state: eatNewLine");
	  return _pEatNewline;
  }

  if (!_pIn->good())
  {
    return _pEatWhitespace;
  }

  throw(std::logic_error("invalid type"));
}

/* Bunch of statements grouped together to reduce size of nextState()
 * 
 * Also these are Priority #1 checks when determining next state.
*/
ConsumeState* ConsumeState::Phase1Comp(char currChar)
{
	if (currChar == '\"')
	{
		testLog("state: eatString");
		return _pEatString;
	}

	if (currChar == '\'')
	{
		testLog("state: eatString");
		return _pEatChar;
	}

	if (std::isspace(currChar) && currChar != '\n')
	{
		testLog("state: eatWhitespace");
		return _pEatWhitespace;
	}

	throw(std::logic_error("invalid type"));
}

/* Bunch of statements grouped together to reduce size of nextState()
*
* Also these are Priority #2 checks when determining next state.
*/
ConsumeState* ConsumeState::Phase2Comp(char currChar, char chNext)
{
	if (currChar == '/' && chNext == '/')
	{
		testLog("state: eatCppComment");
		return _pEatCppComment;
	}

	if (currChar == '/' && chNext == '*')
	{
		testLog("state: eatCComment");
		return _pEatCComment;
	}

	if (std::isalnum(currChar) || (currChar == '_' && std::isalnum(_pIn->peek())))
	{
		testLog("state: eatAlphanum");
		return _pEatAlphanum;
	}

	throw(std::logic_error("invalid type"));
}

/* Statements associated with special characters grouped together to reduce size of nextState()
*
* Also these are Priority #3 checks when determining next state.
*/
ConsumeState* ConsumeState::PhaseSpComp(char curChar, char chNext)
{
	if (std::find(_specialDouble.begin(), _specialDouble.end(), std::string(1, currChar).append(std::string(1, chNext))) != _specialDouble.end())
	{
		testLog("state: Special Double Character");
		return _pEatSpecialDouble;
	}
	if (std::find(_specialSingle.begin(), _specialSingle.end(), currChar) != _specialSingle.end())
	{
		testLog("state: Special Single Character");
		return _pEatSpecialSingle;
	}

	throw(std::logic_error("invalid type"));
}

// Removes whitespace which is not '\n'
class EatWhitespace : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
	//std::cout << "\n  eating whitespace";
    do {
      if (!_pIn->good())  // end of stream
        return;
      currChar = _pIn->get();
    } while (std::isspace(currChar) && currChar != '\n');
  }
};

// Tokenizes or Removes CPP comments
class EatCppComment : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
	if (showComments)
	{
		token += currChar;
		//std::cout << "\n  eating C++ comment";
		do {
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
			token += currChar;
		} while (currChar != '\n');
	}

	do {
		if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();
	} while (currChar != '\n');

  }
};

// Tokenizes or Removes C Comments
class EatCComment : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
	//std::cout << "\n  eating C comment";
	if (showComments)
	{
		do {
			token += currChar;
			if (!_pIn->good())  // end of stream
				return;
			currChar = _pIn->get();
		} while (currChar != '*' || _pIn->peek() != '/');
		token += currChar;
		token += currChar = _pIn->get();
		currChar = _pIn->get();
	}

	do {
		if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();
	} while (currChar != '*' || _pIn->peek() != '/');
	currChar = _pIn->get();
	currChar = _pIn->get();
  }
};

/* Tokenizes strings present in the input stream
 *
 * Also checks are done using 'if' statements to make sure that
 * no special case is missed
*/
class EatString : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		prevChar = currChar;
		currChar = _pIn->get();
		//std::cout << "\n  eating String";
		while (!(currChar == '\"' && !(prevChar == '\\')))
		{
			if (!_pIn->good())  // end of stream
				return;

			if (prevChar != '\\' && currChar == '\"')
				break;

			if (currChar == '\"' && prevChar == '\\')
			{
				int occurences = 0;
				for (int i = token.size() - 1; i > 0 && token[i] == '\\'; i--);

				if (occurences % 2 == 0)
					break;
			}

			if (currChar == '\\' && (_pIn->peek() == '\\' || _pIn->peek() == '\"' || _pIn->peek() == '\''))
			{
				prevChar = currChar;
				currChar = _pIn->get();
				token += currChar;
			}
			else {
				token += currChar;
			}
			prevChar = currChar;
			currChar = _pIn->get();
		}
		currChar = _pIn->get();
	}
};

/* Tokenizes Punctuators except the one included in special.
 *
 * Multiple checks using if blocks ensure that no special condition is missed
*/
class EatPunctuator : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
	//std::cout << "\n  eating punctuator";
    do {
      token += currChar;
	  if (!_pIn->good())  // end of stream
        return;
      currChar = _pIn->get();

	  if (currChar == '_' && isalnum(_pIn->peek()))
		  break;
	  
	  if (std::find(_specialSingle.begin(), _specialSingle.end(), currChar) != _specialSingle.end())
		  break;

	  if (std::find(_specialDouble.begin(), _specialDouble.end(), std::string(1, currChar).append(std::string(1, _pIn->peek()))) != _specialDouble.end())
		  break;

    } while (ispunct(currChar) && token[token.size() - 1] != '}' && currChar != '\"' && currChar != '\'');
  }
};

/* Tokenizes chatacter which succeed "'" symbol
 * If blocks here help in preserving \n \t etc symbols with
 * with escape sequence as character.
*/
class EatCharacter : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating single character";
		prevChar = currChar;
		currChar = _pIn->get();
		while (!(currChar == '\'' && !(prevChar == '\\')))
		{
			if (!_pIn->good())  // end of stream
				return;

			if (prevChar != '\\' && currChar == '\'')
				break;

			if (currChar == '\'' && prevChar == '\\')
			{
				int occurences = 0;
				for (int i = token.size() - 1; i > 0 && token[i] == '\\'; i--);
				
				if (occurences % 2 == 0)
					break;
			}
			
			if (currChar == '\\' && (_pIn->peek() == '\\' || _pIn->peek() == '\"' || _pIn->peek() == '\''))
			{
				prevChar = currChar;
				currChar = _pIn->get();
				token += currChar;
			} else {
				token += currChar;
			}
			prevChar = currChar;
			currChar = _pIn->get();
		}
		currChar = _pIn->get();
	}
};

/* Tokenizes alphanumeric characters. ALso including the one which
 * has an "_" symbol
*/
class EatAlphanum : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
	//std::cout << "\n  eating alphanum";
    do {
      token += currChar;
      if (!_pIn->good())  // end of stream
        return;
      currChar = _pIn->get();
    } while (isalnum(currChar) || currChar == '_');
  }
};

// Tokenizes special single characters
class EatSpecialSingle : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating special character";
		token += currChar;
		if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();
	}
};

// Tokenizes special character pair
class EatSpecialDouble : public ConsumeState
{
public:
	virtual void eatChars()
	{
		token.clear();
		//std::cout << "\n  eating special character";
		token += currChar;
		currChar = _pIn->get();
		token += currChar;
		if (!_pIn->good())  // end of stream
			return;
		currChar = _pIn->get();
	}
};

// Removes new lines from token collection process
// Also checks for special case in Requirement #7
class EatNewline : public ConsumeState
{
public:
  virtual void eatChars()
  {
    token.clear();
    //std::cout << "\n  eating alphanum";
    //token += currChar;
	if (_pIn->peek() == '#')
		token += (static_cast<char>(23));

    if (!_pIn->good())  // end of stream
      return;
    currChar = _pIn->get();
  }
};

// Initializing state machine
ConsumeState::ConsumeState()
{
//  static bool first = true;
  if (first)
  {
    first = false;
    _pEatAlphanum = new EatAlphanum();
    _pEatCComment = new EatCComment();
    _pEatCppComment = new EatCppComment();
	_pEatChar = new EatCharacter();
    _pEatPunctuator = new EatPunctuator();
    _pEatWhitespace = new EatWhitespace();
    _pEatNewline = new EatNewline();
	_pEatString = new EatString();
	_pEatSpecialSingle = new EatSpecialSingle();
	_pEatSpecialDouble = new EatSpecialDouble();
    _pState = _pEatWhitespace;
  }
}

// Removing the states which have been created/ initialized.
ConsumeState::~ConsumeState()
{
  //static bool first = true;
  if (first)
  {
    first = false;
    delete _pEatAlphanum;
    delete _pEatCComment;
    delete _pEatCppComment;
    delete _pEatPunctuator;
    delete _pEatWhitespace;
    delete _pEatNewline;
	delete _pEatChar;
	delete _pEatString;
	delete _pEatSpecialSingle;
	delete _pEatSpecialDouble;
  }
}

// Method to modify special single characters
void ConsumeState::setSpecialSingleChars(std::string ssc)
{
	_specialDouble.clear();
	for (int index = 0; index < ssc.length(); index++)
	{
		if (ssc[index] == '\\' && (index < ssc.length()-1))
		{
			_specialDouble.push_back(ssc.substr(index, 2));
			index++;
			continue;
		}

		_specialSingle.push_back(ssc[index]);
	}
}

// Method to verify special character pairs
void ConsumeState::setSpecialCharPairs(std::string scp)
{
	if (scp.length() % 2 == 0)
	{
		std::cout << "\n Invalid input. The string should have even number of characters";
	}
	else
	{
		_specialDouble.clear();
		for (int index = 0; index < scp.length(); index += 2)
		{
			_specialDouble.push_back(scp.substr(index, 2));
		}
	}
}

Toker::Toker() : pConsumer(new EatWhitespace()) {}
Toker::~Toker() { delete pConsumer; }

/* This function will reset the state of the Tokenizer.
 * It has been implemented to address the issue of not
 * being able to initialize a new tokenizer when already
 * a tokenizer is used previously.
*/
void Toker::reset()
{
	pConsumer->reset();
	pConsumer = new EatWhitespace();
}

bool Toker::attach(std::istream* pIn)
{
  if (pIn != nullptr && pIn->good())
  {
    pConsumer->attach(pIn);
    return true;
  }
  return false;
}

// Gets a single token
std::string Toker::getTok()
{
  while(true) 
  {
    if (!pConsumer->canRead())
      return "";
    pConsumer->consumeChars();
    if (pConsumer->hasTok())
      break;
  }
  return pConsumer->getTok();
}

bool Toker::canRead() { return pConsumer->canRead(); }

// Just another log display to console function.
void testLog(const std::string& msg)
{
#ifdef TEST_LOG
  std::cout << "\n  " << msg;
#endif
}

//----< test stub >--------------------------------------------------
//#define TEST_TOKENIZER
#ifdef TEST_TOKENIZER

#include <fstream>

int main()
{
  std::string fileSpec = "../Tokenizer/Tokenizer.cpp";
  std::ifstream in(fileSpec);
  if (!in.good())
  {
    std::cout << "\n  can't open " << fileSpec << "\n\n";
    return 1;
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

  std::cout << "\n\n";
  return 0;
}
#endif