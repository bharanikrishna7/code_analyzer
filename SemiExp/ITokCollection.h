#ifndef ITOKCOLLECTION_H
#define ITOKCOLLECTION_H
//////////////////////////////////////////////////////////////////////
// ITokCollection.h - package for the ITokCollection interface      //
// Version: 1.0                                                     //
// Language:    C++, Visual Studio 2015                             //
// Platform:    MSI GE62 - 2QD, Windows 10                          //
// Application: Parser component, CSE687 - Object Oriented Design   //
// Author:      Venkata Chekuri, SUID : 356579351                   //
//              vbchekur@syr.edu                                    //
//////////////////////////////////////////////////////////////////////
/*
 * Credits:
 * --------
 * I would like to give credits to Prof. Jim W Fawcett Syracuse University
 * , CST 4-187) for providing a started code and explaining how the Lexical
 * Scanner should work.
 *
 * Package Operation:
 * ------------------
 * ITokCollection is an interface that supports substitution of different
 * types of scanners for parsing.  In this solution, we illustrate that
 * by binding two different types of collections, SemiExp and XmlParts,
 * to this interface.  This is illustrated in the test stubs for the
 * SemiExpression and XmlElementParts modules.
 * Note that we only use the SemiExpression collector in the parser and
 * all of the higher level executives.  We certainly could bind the
 * XmlParts class to parser's ITokCollector pointer, but didn't due to
 * lack of time.
 *
 * Maintenance History:
 * --------------------
 * ver 1.0 : 8 Feb 16
 * - first release
 * - Build on top of Prof. Jim fawcett's 02 Jan 11 code.
*/

#include <string>
namespace Scanner
{
	struct ITokCollection
	{
		virtual bool get(bool clear = true) = 0;
		virtual size_t length() = 0;
		virtual std::string& operator[](size_t n) = 0;
		virtual size_t find(const std::string& tok) = 0;
		virtual void push_back(const std::string& tok) = 0;
		virtual bool merge(const std::string& firstTok, const std::string& secondTok) = 0;
		virtual bool remove(const std::string& tok) = 0;
		virtual bool remove(size_t i) = 0;
		virtual void toLower() = 0;
		virtual void trimFront() = 0;
		virtual void clear() = 0;
		virtual std::string show(bool showNewLines = false) = 0;
		virtual ~ITokCollection() {};
	};
}
#endif