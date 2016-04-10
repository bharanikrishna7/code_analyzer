#ifndef TOKENIZER_H
#define TOKENIZER_H
//////////////////////////////////////////////////////////////////////////////
// Tokenizer.h - read words from a std::stream								//
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
 * - first release
 *
 * Credits:
 * --------
 * I would like to give credits to Prof. Jim W Fawcett Syracuse University
 * , CST 4-187) for providing a started code and explaining how Parallel 
 * Dependency Analysis project should work.
 *
 */
#include <iosfwd>
#include <string>

namespace Scanner
{
  class ConsumeState;    // private worker class
  struct Context;        // private shared data storage

  class Toker
  {
  public:
    Toker();
    Toker(const Toker&) = delete;
    ~Toker();
    Toker& operator=(const Toker&) = delete;
    bool attach(std::istream* pIn);
    std::string getTok();
    bool canRead();
    void returnComments(bool doReturnComments = true);
    bool isComment(const std::string& tok);
    size_t currentLineCount();
    void setSpecialTokens(const std::string& commaSeparatedString);
  private:
    ConsumeState* pConsumer;
    Context* _pContext;
  };
}
#endif
