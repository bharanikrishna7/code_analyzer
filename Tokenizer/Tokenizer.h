#ifndef TOKENIZER_H
#define TOKENIZER_H
///////////////////////////////////////////////////////////////////////
// Tokenizer.h - read words from a std::stream                       //
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
 * This package provides a public Toker class and private ConsumeState class.
 * Toker reads words from a std::stream, throws away whitespace and comments
 * and returns words from the stream in the order encountered.  Quoted
 * strings and certain punctuators and newlines are returned as single tokens.
 *
 * This is based on the State Design Pattern. 
 *
 * It collects tokens based on their types. The tokens are categorized into
 * following types :-
 * alphanumeric       - The alphanumeric characters which are not a part of comment,
 *                      string, or character. This includes '_' followed by an alpha-
 *                      numeric character since the variable naming scheme allows this.
 * specialSingle      - These tokens have been set to <, >, [, ], (, ), {, }, :, =, +,
 *                      -, *, \n but a user can set them using function 
 *                      setSpecialSingleChars()
 * specialDouble      - These tokens have been set to <<, >>, ::, ++, --, ==, +=, -=,
 *                      *=, /= but user can set them using function setSpecialCharPairs()
 * punctuator         - These characters include all the punctuator symbols but whhenever " or'
 *                      not preceeded by \(escape) or _ followed by alphanumeric character comes
 *                      then they are not considered as punctuator since they are used to identify
 *                      other kind of tokens.
 * Cpp Style Comments - // followed by charactes in the line.
 * C Stype Comments   - /* followed by characters till the commend block ends.
 * String             - These characters which succeed * till again * appears which is not preceded
 *                      by \(escape) sequence.
 * Character          - This will include a single character which follows ' symbol. It is assumed
 *                      that the code which is being analyzed is syntactically correct. i.e - There
 *                      will be no occurences of 'asdf' or something like that and it'll always be
 *                      'x' or '\t' (or similar).
 *
 * Build Process:
 * --------------
 * Required Files: Tokenizer.h, Tokenizer.cpp
 * Build Command: devenv Tokenizer.sln /rebuild debug
 *
 * Maintenance History:
 * --------------------
 * ver 1.0 : 08 Feb 2016
 * - Initial Release
 * - Build on top of Prof. Jim fawcett's 02 Feb 2016 code.
 *
 * Planned Additions and Changes:
 * ------------------------------
 * - Better way to handle end of certain tokens ex : \n followed
 *   by #. In this implementation a special character ASCII - 23
 *   is being introduced as a token to understand this situation.
 *
 */

#include <iosfwd>
#include <string>
#include <vector>

namespace Scanner
{
  class ConsumeState;

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
	void reset();
  private:
    ConsumeState* pConsumer;
  };
}
#endif
#define a