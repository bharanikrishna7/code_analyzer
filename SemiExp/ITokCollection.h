#ifndef ITOKCOLLECTION_H
#define ITOKCOLLECTION_H
//////////////////////////////////////////////////////////////////////////////
// ITokCollection.h - package for the ITokCollection interface				//
// Version :	  1.0                                                       //
// Language:      Visual C++, Visual Studio 2015							//
// Platform:      MSI GE62 2QD, Core-i7, Windows 10							//
// Application:   Parallel Dependency Analysis								//
// Author:		  Venkata Chekuri, SUID : 356579351							//
//                vbchekur@syr.edu											//
//////////////////////////////////////////////////////////////////////////////
/*
 *  Module Purpose:
 *  ---------------
 *  ITokCollection is an interface that supports substitution of different
 *  types of scanners for parsing.  In this solution, we illustrate that
 *  by binding two different types of collections, SemiExp and XmlParts,
 *  to this interface.  This is illustrated in the test stubs for the
 *  SemiExpression and XmlElementParts modules.
 *
 *  Maintenance History:
 *  --------------------
 *  ver 1.0 : 8 Feb 16
 *  - first release
 *
 *  Credits:
 *  --------
 *  I would like to give credits to Prof. Jim W Fawcett Syracuse University
 *  , CST 4-187) for providing a started code and explaining how Parallel 
 *  Dependency Analysis project should work.
 *
 */
#include <string> 

struct ITokCollection
{
  virtual bool get(bool clear=true)=0;
  virtual size_t length()=0;
  virtual std::string& operator[](size_t n) = 0;
  virtual std::string operator[](size_t n) const = 0;
  virtual size_t find(const std::string& tok)=0;
  virtual void push_back(const std::string& tok)=0;
  virtual bool remove(const std::string& tok)=0;
  virtual bool remove(size_t i)=0;
  virtual void toLower()=0;
  virtual void trimFront()=0;
  virtual void clear()=0;
  virtual std::string show(bool showNewLines=false)=0;
  virtual bool isComment(const std::string& tok) = 0;
  virtual ~ITokCollection() {};
};

#endif
