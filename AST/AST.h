#ifndef AST_H
#define AST_H
//////////////////////////////////////////////////////////////////////////////
// AST.h - defines the abstract syntax tree which holds the node of			//
//		   "node" defined below												//
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
 * This package contains a structure "node" and a class definition. The node type
 * is an application specific type designed to hold whatever information you need
 * to hold as a node of the AST.
 *
 * Public Interface:
 * -----------------
 * void treeWalk(node* element);								// will perform tree walk
 * void setRoot(node* element);									// set a node as root of AST
 * node* getRoot();												// get the root node of AST
 * size_t getComplexity(node* element);							// get the complexity of element
 * void scopes(node* element, std::vector<std::string>& vec);	// walk through all the scopes (Similar to tree walk)
 *
 * Required Files:
 * ---------------
 * AST.h, AST.cpp
 *
 * Build Command:
 * --------------
 * devenv AST.sln /rebuild debug
 *
 * Maintenance History:
 * --------------------
 * ver 1.0 : 17 March 2016
 * - first release
 *
 */

#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include "../Utilities/Utilities.h"

// node is a application specific type.  It must provide a function:
// std::string show() that returns a string display of its parts. see
// test stub for an example.

/* The element parent in the node structure will be associated only with
 * nodes of type function. This is present to aid the design of TypeTable
 * for dependency analysis among files.
 *
 * For all nodes other than type function they will have "-" in parent (Though
 * this can be used to store it's parent Scope, but AST is there to maintain
 * the structure).
*/
struct node
{
	std::string type;
	std::string name;
	size_t lineCountBegin;
	size_t lineCountEnd;
	size_t lineCount;
	size_t complexity;
	std::vector<node*> children;

	std::string show()
	{
		std::ostringstream temp;
		temp << "(";
		temp << type;
		temp << ", ";
		temp << name;
		temp << ", ";
		temp << lineCountBegin;
		temp << ", ";
		temp << lineCountEnd;
		temp << ", ";
		temp << lineCount;
		temp << ", ";
		temp << complexity;
		temp << ")";
		return temp.str();
	}

	void addChild(node* newNode) {
		children.push_back(newNode); 
	}

	 std::vector<node*>* getChildren(){
		 return &children; 
	 }

	void deleteChild(node* parent, node* dChild)
	{if (parent == nullptr)
			return;
		
		std::vector<node*>::iterator itr;
		std::vector<node*>* _children = parent->getChildren();

		for (itr = _children->begin(); itr != _children->end(); )
		{
			if (*(itr) == dChild)
			{
				delete *itr;
				itr = _children->erase(itr);
			}
			else
			{
				deleteChild(*itr, dChild);
				itr++;
			}
		}
	}
		
};

// AST Class declaration
class AST{
public:
	AST() {}
	void treeWalk(node* element);
	void setRoot(node* element);
	node* getRoot();
	size_t getComplexity(node* element);
	void scopes(node* element, std::vector<std::string>& vec);
private:
	node *root;
};

#endif
