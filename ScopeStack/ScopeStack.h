#ifndef SCOPESTACK_H
#define SCOPESTACK_H
//////////////////////////////////////////////////////////////////////////////
// ScopeStack.h - implements template stack holding specified element type	//
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
 * This package contains one class: ScopeStack<element>.  The element type
 * is an application specific type designed to hold whatever information you
 * need to stack.
 *
 * Public Interface:
 * -----------------
 * ScopeStack<element> stack;
 * element elem;						// define some element type
 * stack.push(elem);					// Push element into ScopeStack stack
 * element popped = stack.pop();		// Remove element from ScopeStack stack
 *
 * Required Files:
 * ---------------
 * ScopeStack.h, ScopeStack.cpp
 *
 * Build Command:
 * --------------
 * devenv ScopeStack.sln /rebuild debug
 *
 * Maintenance History:
 * --------------------
 * ver 1.0 : 17 March 16
 * - first release
 *
 * Credits:
 * --------
 * I would like to give credits to Prof. Jim W Fawcett Syracuse University
 * , CST 4-187) for providing a started code and explaining how Parallel 
 * Dependency Analysis project should work.
 *
 */
#include <list>

// element is a application specific type.  It must provide a function:
// std::string show() that returns a string display of its parts. see
// test stub for an example.
template<typename element>
class ScopeStack {
public:
  typename typedef std::list<element>::iterator iterator;
  void push(const element& item);
  element pop();
  element top();
  size_t size();
  iterator begin();
  iterator end();
private:
  std::list<element> stack;
};

template<typename element>
typename inline size_t ScopeStack<element>::size() { return stack.size(); }

template<typename element>
typename inline ScopeStack<element>::iterator ScopeStack<element>::begin() { return stack.begin(); }

template<typename element>
typename inline ScopeStack<element>::iterator ScopeStack<element>::end() { return stack.end(); }

// Push an element into ScopeStack
template<typename element>
void ScopeStack<element>::push(const element& item)
{
  stack.push_back(item);
}

// Get the top element from the ScopeStack and remove it from ScopeStack
template<typename element>
element ScopeStack<element>::pop() 
{ 
  element item = stack.back(); 
  stack.pop_back(); 
  return item; 
}

// Get the top element from the ScopeStack
template<typename element>
element ScopeStack<element>::top()
{
  return stack.back();
}

// Will print the entire stack.
template<typename element>
void showStack(ScopeStack<element>& stack, bool indent = true)
{
  if (stack.size() == 0)
  {
    std::cout << "\n  ScopeStack is empty";
    return;
  }
  ScopeStack<element>::iterator iter = stack.begin();
  while (iter != stack.end())
  {
    std::string strIndent = std::string(2 * stack.size(), ' ');
    if (!indent)
      strIndent = "";
    element temp = *iter;
    std::cout << "\n  " << strIndent << temp->show();
    ++iter;
  }
}

// Will print the entire stack.
template<typename element>
void showStack(ScopeStack<element*>& stack, bool indent = true)
{
  if (stack.size() == 0)
  {
    std::cout << "\n  ScopeStack is empty";
    return;
  }
  ScopeStack<element*>::iterator iter = stack.begin();
  while (iter != stack.end())
  {
    std::string strIndent = std::string(2 * stack.size(), ' ');
    if (!indent)
      strIndent = "";
    std::cout << "\n  " << strIndent << ((*iter)->show());
    ++iter;
  }
}
#endif
