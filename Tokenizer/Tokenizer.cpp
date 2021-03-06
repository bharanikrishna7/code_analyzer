//////////////////////////////////////////////////////////////////////////////
// Tokenizer.cpp - read words from a std::stream							//
// Version :	  1.0                                                       //
// Language:      Visual C++, Visual Studio 2015							//
// Platform:      MSI GE62 2QD, Core-i7, Windows 10							//
// Application:   Code Parser with AST										//
// Author:		  Venkata Bharani Krishna Chekuri						    //
//                vbchekur@syr.edu											//
//////////////////////////////////////////////////////////////////////////////

#include "Tokenizer.h"
#include "..\Utilities\Utilities.h"

/* For using the classes and functions present in Utilities */
using namespace Utilities;

namespace Scanner
{
	//////////////////////////////////////////////
	// Context is a shared data storage facility.
	/*
	 * - with Context we don't need static members to share
	 *   state pointes etc.  Context holds and shares all that.
	 * - Context is an entirely private facility used by ConsumeState
	 *   and its derived instances, so there is no need for encapsulation
	 *   and we use a struct.  This won't be appropriate if we handed
	 *   the context to clients of Toker.
	 */
	struct Context
	{
		Context();
		~Context();
		std::string token;
		std::istream* _pIn;
		std::vector<std::string> _oneCharTokens =
		{
		  "\n", "<", ">", "{", "}", "[", "]", "(", ")", ":", "=", "+", "-", "*", "."
		};
		std::vector<std::string> _twoCharTokens =
		{
		  "<<", ">>", "::", "++", "--", "==", "+=", "-=", "*=", "/="
		};
		int prevChar;
		int currChar;
		bool _doReturnComments;
		size_t _lineCount;
		ConsumeState* _pState;
		ConsumeState* _pEatCppComment;
		ConsumeState* _pEatCComment;
		ConsumeState* _pEatWhitespace;
		ConsumeState* _pEatPunctuator;
		ConsumeState* _pEatAlphanum;
		ConsumeState* _pEatSpecialCharacters;
		ConsumeState* _pEatDQString;
		ConsumeState* _pEatSQString;
	};

	/////////////////////////////////////
	// private ConsumeState worker class
	/*
	 *   Nota Bene:
	 *   - At exit the eatChars() member should return with
	 *     currChar set to the first non-state character.
	 *   - Each derived class must insure that this condition
	 *     is meet.
	 *
	 *  This ConsumeState class is nearly identical to the
	 *  the ConsumeState that uses static member sharing.
	 *  We've just replaced the static members with references
	 *  to those members stored in the context.
	 */
	class ConsumeState
	{
		friend class Toker;
	public:
		using Token = std::string;
		ConsumeState();
		ConsumeState(const ConsumeState&) = delete;
		ConsumeState& operator=(const ConsumeState&) = delete;
		virtual ~ConsumeState();
		void attach(std::istream* pIn) { _pContext->_pIn = pIn; }
		virtual void eatChars() = 0;
		void consumeChars() {
			_pContext->_pState->eatChars();
			_pContext->_pState = nextState();
		}
		bool canRead() { return _pContext->_pIn->good(); }
		std::string getTok() { return _pContext->token; }
		bool hasTok() { return _pContext->token.size() > 0; }
		ConsumeState* nextState();
		void returnComments(bool doReturnComments = false);
		size_t currentLineCount();
		void setSpecialTokens(const std::string& commaSeparatedString);
		void setContext(Context* pContext);
		ConsumeState* commentHelper(char curChar, char chNext);
		ConsumeState* stringHelper(char curChar, char chNext);
	protected:
		Context* _pContext;
		bool collectChar();
		bool isOneCharToken(Token tok);
		bool isTwoCharToken(Token tok);
		Token makeString(int ch);
	};
}

using namespace Scanner;
using Token = std::string;

////////////////////////////////////////////////////////
/* used by Toker to provide Consumer with Context ptr */
void ConsumeState::setContext(Context* pContext)
{
	_pContext = pContext;
}

/////////////////////////////////////
/* replace one and two char tokens */
void ConsumeState::setSpecialTokens(const std::string& commaSeparatedTokens)
{
	std::vector<Token> result = StringHelper::split(commaSeparatedTokens);
	_pContext->_oneCharTokens.clear();
	_pContext->_twoCharTokens.clear();
	for (std::string item : result)
	{
		if (item.size() == 1 || item == "\n")
			_pContext->_oneCharTokens.push_back(item);
		if (item.size() >= 2)
			_pContext->_twoCharTokens.push_back(item);
	}
}

/////////////////////////////////////////////////////
/* return number of newlines collected from stream */
size_t ConsumeState::currentLineCount() { return _pContext->_lineCount; }

///////////////////////////////////
/* collect character from stream */
bool ConsumeState::collectChar()
{
	if (_pContext->_pIn->good())
	{
		_pContext->prevChar = _pContext->currChar;
		_pContext->currChar = _pContext->_pIn->get();
		if (_pContext->currChar == '\n')
			++(_pContext->_lineCount);
		return true;
	}
	return false;
}

////////////////////////////////////////////
/* logs to console if TEST_LOG is defined */
void testLog(const std::string& msg);  // forward declaration

//////////////////////////////////////////////
/* turn on\off returning comments as tokens */
void ConsumeState::returnComments(bool doReturnComments)
{
	_pContext->_doReturnComments = doReturnComments;
}

/////////////////////////////////////////////////////
/* is tok one of the special one character tokens? */ 
bool ConsumeState::isOneCharToken(Token tok)
{
	for (size_t i = 0; i < _pContext->_oneCharTokens.size(); ++i)
		if (_pContext->_oneCharTokens[i] == tok)
			return true;
	return false;
}

/////////////////////////////////////////////////////
/* is tok one of the special two character tokens? */
bool ConsumeState::isTwoCharToken(Token tok)
{
	for (size_t i = 0; i < _pContext->_twoCharTokens.size(); ++i)
		if (_pContext->_twoCharTokens[i] == tok)
			return true;
	return false;
}

/////////////////////////////////////////
/* make a string with this one integer */
ConsumeState::Token ConsumeState::makeString(int ch)
{
	Token temp;
	return temp += ch;
}

////////////////////////////////////
/* decide which state to use next */
ConsumeState* ConsumeState::nextState()
{
	if (!(_pContext->_pIn->good()))
	{
		return nullptr;
	}
	int chNext = _pContext->_pIn->peek();
	if (chNext == EOF)
	{
		_pContext->_pIn->clear();
		// if peek() reads end of file character, EOF, then eofbit is set and
		// _pIn->good() will return false.  clear() restores state to good
	}
	if (isOneCharToken(makeString(_pContext->currChar)))
	{
		return _pContext->_pEatSpecialCharacters;
	}
	if (std::isspace(_pContext->currChar) && _pContext->currChar != '\n')
	{
		testLog("state: eatWhitespace");
		return _pContext->_pEatWhitespace;
	}
	if ((_pContext->currChar == '/' && chNext == '/') || (_pContext->currChar == '/' && chNext == '*'))
	{
		return commentHelper(_pContext->currChar, chNext);
	}
	if (std::isalnum(_pContext->currChar) || _pContext->currChar == '_')
	{
		testLog("state: eatAlphanum");
		return _pContext->_pEatAlphanum;
	}
	if ((_pContext->currChar == '\"' && _pContext->prevChar != '\\') || (_pContext->currChar == '\'' && _pContext->prevChar != '\\'))
	{
		return stringHelper(_pContext->currChar, _pContext->prevChar);
	}
	if (ispunct(_pContext->currChar))
	{
		testLog("state: eatPunctuator");
		return _pContext->_pEatPunctuator;
	}
	if (!_pContext->_pIn->good())
	{
		return _pContext->_pEatWhitespace;
	}
	std::string error = "invalid type, currChar = " + Converter<char>::toString(_pContext->currChar);
	throw(std::logic_error(error.c_str()));
}

////////////////////////////////////////
/* class that solves for next state when 
c/c++ style comment conditions appear */
ConsumeState* ConsumeState::commentHelper(char curChar, char chNext)
{
	if (curChar == '/' && chNext == '/')
	{
		testLog("state: eatCppComment");
		return _pContext->_pEatCppComment;
	}
	if (curChar == '/' && chNext == '*')
	{
		testLog("state: eatCComment");
		return _pContext->_pEatCComment;
	}
	return _pContext->_pEatWhitespace;
}

////////////////////////////////////////////////
/* class that solves for next state when 
single/double quoted string conditions appear */
ConsumeState* ConsumeState::stringHelper(char curChar, char chNext)
{
	if (_pContext->currChar == '\"' && _pContext->prevChar != '\\')
	{
		return _pContext->_pEatDQString;
	}
	if (_pContext->currChar == '\'' && _pContext->prevChar != '\\')
	{
		return _pContext->_pEatSQString;
	}
	return _pContext->_pEatWhitespace;
}

////////////////////////////////////
/* class that consumes whitespace */
class EatWhitespace : public ConsumeState
{
public:
	EatWhitespace(Context* pContext)
	{
		_pContext = pContext;
	}
	virtual void eatChars()
	{
		_pContext->token.clear();
		do {
			if (!collectChar())
				return;
		} while (std::isspace(_pContext->currChar) && _pContext->currChar != '\n');
	}
};

/////////////////////////////////////
/* class that consumes C++ comment */
class EatCppComment : public ConsumeState
{
public:
	EatCppComment(Context* pContext)
	{
		_pContext = pContext;
	}
	virtual void eatChars()
	{
		_pContext->token.clear();
		do {
			if (_pContext->_doReturnComments)
				_pContext->token += _pContext->currChar;
			if (!collectChar())
				return;
		} while (_pContext->currChar != '\n');
	}
};

///////////////////////////////////
/* class that consumes C comment */
class EatCComment : public ConsumeState
{
public:
	EatCComment(Context* pContext)
	{
		_pContext = pContext;
	}
	virtual void eatChars()
	{
		_pContext->token.clear();
		do {
			if (_pContext->_doReturnComments)
				_pContext->token += _pContext->currChar;
			if (!collectChar())
				return;
		} while (_pContext->currChar != '*' || _pContext->_pIn->peek() != '/');
		if (_pContext->_doReturnComments)
			_pContext->token += _pContext->currChar;
		if (!collectChar())      // get terminating '/'
			return;
		if (_pContext->_doReturnComments)
			_pContext->token += _pContext->currChar;
		collectChar();           // get first non-comment char
	}
};

/////////////////////////////////
/* class that eats punctuators */
class EatPunctuator : public ConsumeState
{
public:
	EatPunctuator(Context* pContext)
	{
		_pContext = pContext;
	}
	virtual void eatChars()
	{
		_pContext->token.clear();
		do {
			if (_pContext->currChar == '\"' && _pContext->prevChar != '\\')    // start of double quoted string
			{
				return;
			}
			if (_pContext->currChar == '\'' && _pContext->prevChar != '\\')    // start of single quoted string
			{
				return;
			}
			Token temp = makeString(_pContext->currChar);
			if (isOneCharToken(temp))
			{
				return;
			}
			_pContext->token += _pContext->currChar;
			if (!_pContext->_pIn->good())  // end of stream
			{
				return;
			}
			if (!collectChar())
				return;
		} while (ispunct(_pContext->currChar));
	}
};

///////////////////////////////////////////
/* class that eats alphanumeric characters. 
(Treats underscore '_' as alphanumeric 
character) */
class EatAlphanum : public ConsumeState
{
public:
	EatAlphanum(Context* pContext)
	{
		_pContext = pContext;
	}
	virtual void eatChars()
	{
		_pContext->token.clear();
		do {
			_pContext->token += _pContext->currChar;
			if (!collectChar())
				return;
		} while (isalnum(_pContext->currChar) || _pContext->currChar == '_');
	}
};

//////////////////////////////////////////////////
/* class that eats one and two character tokens */
class EatSpecialCharacters : public ConsumeState
{
public:
	EatSpecialCharacters(Context* pContext)
	{
		_pContext = pContext;
	}
	virtual void eatChars()
	{
		_pContext->token.clear();
		_pContext->token += _pContext->currChar;
		Token temp = makeString(_pContext->currChar) += _pContext->_pIn->peek();
		if (isTwoCharToken(temp))
		{
			collectChar();
			_pContext->token += _pContext->currChar;
		}
		collectChar();
	}
};

////////////////////////////////////////////////
/* Helper class for EatDQString & EatSQString */
bool isEscapted(std::string tok)
{
	size_t backSlashCount = 0;
	for (size_t i = tok.size() - 1; i >= 0; --i)
	{
		if (tok[i] == '\\')
			++backSlashCount;
		else break;
	}
	return (backSlashCount % 2 == 1);
}

//////////////////////////////////////////
/* class that eats double quoted string */
class EatDQString : public ConsumeState
{
public:
	EatDQString(Context* pContext)
	{
		_pContext = pContext;
	}
	virtual void eatChars()
	{
		_pContext->token.clear();
		do
		{
			_pContext->token += _pContext->currChar;
			if (!collectChar())
				return;
		} while (_pContext->currChar != '\"' || isEscapted(_pContext->token));
		_pContext->token += _pContext->currChar;
		collectChar();
	}
};

//////////////////////////////////////////
/* class that eats single quoted string */
class EatSQString : public ConsumeState
{
public:
	EatSQString(Context* pContext)
	{
		_pContext = pContext;
	}
	virtual void eatChars()
	{
		_pContext->token.clear();
		do
		{
			_pContext->token += _pContext->currChar;
			if (!collectChar())
				return;
		} while (_pContext->currChar != '\'' || isEscapted(_pContext->token));
		_pContext->token += _pContext->currChar;
		collectChar();
	}
};

///////////////////////////////////
/* construct shared data storage */
Context::Context()
{
	_pEatAlphanum = new EatAlphanum(this);
	_pEatCComment = new EatCComment(this);
	_pEatCppComment = new EatCppComment(this);
	_pEatPunctuator = new EatPunctuator(this);
	_pEatWhitespace = new EatWhitespace(this);
	_pEatSpecialCharacters = new EatSpecialCharacters(this);
	_pEatDQString = new EatDQString(this);
	_pEatSQString = new EatSQString(this);
	_pState = _pEatWhitespace;
	_lineCount = 1;
	_doReturnComments = false;
}

/////////////////////////////
/* return shared resources */
Context::~Context()
{
	delete _pEatAlphanum;
	delete _pEatCComment;
	delete _pEatCppComment;
	delete _pEatPunctuator;
	delete _pEatWhitespace;
	delete _pEatSpecialCharacters;
	delete _pEatDQString;
	delete _pEatSQString;
}

////////////////////////////////////////////////////
/* no longer has anything to do - will be removed */
ConsumeState::ConsumeState() {}

////////////////////////////////////////////////////
/* no longer has anything to do - will be removed */
ConsumeState::~ConsumeState() {}

/////////////////////////////////////////////////////
/* construct toker starting in state EatWhitespace */
Toker::Toker()
{
	_pContext = new Context();
	pConsumer = _pContext->_pEatWhitespace;
	pConsumer->setContext(_pContext);
}

/////////////////////////////////////////
/* destructor deletes the current state. 
Deleting any state deletes them all since 
derived destructor implicitly invokes the 
base destructor */
Toker::~Toker()
{
	delete _pContext;
	_pContext = nullptr;
}

/////////////////////////////////////////////
/* attach tokenizer to stream. Stream can be 
either a std::fstream or std::stringstream */
bool Toker::attach(std::istream* pIn)
{
	if (VERBOSE) { std::cout << "\n [VERBOSE] : Attaching fstream/stringstream\n"; }
	if (pIn != nullptr && pIn->good())
	{
		if (VERBOSE) { std::cout << "\n [VERBOSE] : Successfully Attached fstream/stringstream\n"; }
		pConsumer->attach(pIn);
		//_pContext->_pIn = pIn;
		return true;
	}
	if (VERBOSE) { std::cout << "\n [VERBOSE] : Unable to Attach stream\n"; }
	return false;
}

/////////////////////////////////////////////
/* collect token generated by ConsumeState */
std::string Toker::getTok()
{
	while (true)
	{
		if (!pConsumer->canRead())
			return "";
		pConsumer->consumeChars();
		if (pConsumer->hasTok())
			break;
	}
	return pConsumer->getTok();
}

//////////////////////////////////////////////
/* has toker reached the end of its stream? */
bool Toker::canRead() { return pConsumer->canRead(); }

/////////////////////////////////////////////
/* start/stop returning comments as tokens */
void Toker::returnComments(bool doReturnComments)
{
	if (VERBOSE) { std::cout << "\n [VERBOSE] : Enable Return Comments : " << doReturnComments << "\n"; }
	pConsumer->returnComments(doReturnComments);
}

//////////////////////////////
/* is this token a comment? */
bool Toker::isComment(const std::string& tok)
{
	if (tok.find("//") < tok.size() || tok.find("/*") < tok.size())
		return true;
	return false;
}

//////////////////////////////////////////////////
/* return number of newlines pulled from stream */
size_t Toker::currentLineCount()
{
	return pConsumer->currentLineCount();
}

/////////////////////////////////
/* set one and two char tokens */
void Toker::setSpecialTokens(const std::string& commaSeparatedTokens)
{
	if (VERBOSE) { std::cout << "\n [VERBOSE] : Updating Special Tokens\n"; }
	pConsumer->setSpecialTokens(commaSeparatedTokens);
}

///////////////////////////////////////
/* get the value of VERBOSE variable */
bool Toker::getVerbose() {
	return VERBOSE;
}

////////////////////////////////////
/* set the VERBOSE variable value */
void Toker::setVerbose(bool paramV) {
	VERBOSE = paramV;
}

/////////////////////////////////////////////////////////
/* debugging output to console if TEST_LOG is #defined */
void testLog(const std::string& msg)
{
#ifdef TEST_LOG
	std::cout << "\n  " << msg;
#endif
}

/* TEST STUB */
#ifdef TEST_TOKENIZER

#include <fstream>
#include <exception>

///////////////////////////////////////////////////////
/* Function to Test the Toker with input file stream */
void test_Toker(Toker &toker, std::ifstream &in)
{
	toker.returnComments();
	toker.attach(&in);
	
	std::cout << "\n current line count = " << toker.currentLineCount();
	
	do
	{
		std::string tok = toker.getTok();
		if (tok == "\n")
			tok = "newline";
		std::cout << "\n -- " << tok;
	} while (in.good());

	std::cout << "\n current line count = " << toker.currentLineCount();
	putline();
}

//////////////////////////////////////////
/* Function to Set Special Tokens and then 
test the Toker with input file stream */
void test_SetSpecialTokens(std::ifstream &in, bool verbose) {
	StringHelper::Title("Testing change of special characters");
	
	std::string newSpecialChars = "., :, +, +=, \n, {, }";
	std::cout << "\n new special tokens: " << newSpecialChars;
	
	Toker toker;
	toker.setVerbose(verbose);
	toker.setSpecialTokens(newSpecialChars);
	in.clear();
	in.seekg(std::ios::beg);
	test_Toker(toker, in);
	putline();
}

////////////////////////
/* Main Test Function */
int main()
{
	Timer time;
	time.StartClock();

	StringHelper::Title("Testing Tokenizer", '=');
	std::string fileSpec = "../TestFiles/Tokenizer/Test.txt";

	try
	{
		std::ifstream in(fileSpec);
		// Check if file exists and read persmission is granted
		// if file doesnot exist then terminate the test
		if (!in.good())
		{
			std::cout << "\n  can't open " << fileSpec << "\n\n ";
			system("pause");
			return 1;
		}

		// Declare Toker and test toker with FileSpec file
		Toker toker;
		toker.setVerbose(true);
		test_Toker(toker, in);
		
		// Set New Special Tokens and then test the toker with
		// FileSpec file
		test_SetSpecialTokens(in, toker.getVerbose());

	}
	catch (std::logic_error& ex)
	{
		std::cout << "\n [EXCEPTION] : " << ex.what();
	}

	std::cout << "\n [Execution Time] : " << time.StopClock() << " ms";
	std::cout << "\n\n ";
	system("pause");

	return 0;
}
#endif
