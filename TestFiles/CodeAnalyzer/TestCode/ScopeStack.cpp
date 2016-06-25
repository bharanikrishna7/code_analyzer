//////////////////////////////////////////////////////////////////////////////
// ScopeStack.h - implements template stack holding specified element type	//
// Version :	  1.0                                                       //
// Language:      Visual C++, Visual Studio 2015							//
// Platform:      MSI GE62 2QD, Core-i7, Windows 10							//
// Application:   Parallel Dependency Analysis								//
// Author:		  Venkata Chekuri, SUID : 356579351							//
//                vbchekur@syr.edu											//
//////////////////////////////////////////////////////////////////////////////

//----< test stub >------------------------------------------------------

#ifdef TEST_SCOPESTACK

#include "ScopeStack.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "../Utilities/Utilities.h"

using PrettyHelper = Utilities::StringHelper;

struct element
{
	std::string type;
	std::string name;
	size_t startLineCount = 0;
	size_t endLineCount = 0;
	std::string show()
	{
		std::ostringstream temp;
		temp << "(";
		temp << type;
		temp << ", ";
		temp << name;
		temp << ", ";
		temp << startLineCount;
		temp << ", ";
		temp << endLineCount;
		temp << ")";
		return temp.str();
	}
	std::vector<element*> _children;
};

void TreeWalk(element* pItem)
{
	static size_t indentLevel = 0;
	std::cout << "\n  " << std::string(2 * indentLevel, ' ') << pItem->show();
	auto iter = pItem->_children.begin();
	++indentLevel;
	while (iter != pItem->_children.end())
	{
		TreeWalk(*iter);
		++iter;
	}
	--indentLevel;
}

int main()
{
	PrettyHelper::Title("\n  Testing Scope Stack", '=');
	PrettyHelper::Title("\n  pushing items onto ScopeStack instance after adding startLineCount", '-');
	ScopeStack<element*> testStack;
	element* pItem = new element;
	pItem->type = "function";
	pItem->name = "fun1";
	pItem->startLineCount = 33;
	testStack.push(pItem);
	pItem = new element;
	pItem->type = "if";
	pItem->name = "";
	pItem->startLineCount = 38;
	(testStack.top()->_children).push_back(pItem);
	testStack.push(pItem);
	pItem = new element;
	pItem->type = "for";
	pItem->name = "";
	pItem->startLineCount = 40;
	(testStack.top()->_children).push_back(pItem);
	testStack.push(pItem);
	showStack<element>(testStack, false);
	std::cout << std::endl;
	PrettyHelper::Title("\n  Popping two items off ScopeStack after adding endLineCount", '-');
	testStack.top()->endLineCount = 50;
	std::cout << "\n  " << testStack.pop()->show();
	testStack.top()->endLineCount = 55;
	std::cout << "\n  " << testStack.pop()->show() << "\n";
	PrettyHelper::Title("\n  Pushing another item onto ScopeStack after adding startLineElement", '-');
	pItem = new element;
	pItem->type = "while";
	pItem->name = "";
	pItem->startLineCount = 60;
	(testStack.top()->_children).push_back(pItem);
	testStack.push(pItem);
	PrettyHelper::Title("\n  Stack now contains:", '-');
	showStack<element>(testStack, false);
	std::cout << "\n";
	PrettyHelper::Title("\n  Popping last elements off stack after adding endLineCount", '-');
	testStack.top()->endLineCount = 70;
	element* pTop = testStack.pop();
	std::cout << "\n  " << pTop->show();
	testStack.top()->endLineCount = 75;
	pTop = testStack.pop();
	std::cout << "\n  " << pTop->show() << "\n";
	PrettyHelper::Title("\n  Walking simulated AST tree:", '-');
	TreeWalk(pTop);
	std::cout << "\n\n";
}

#endif
