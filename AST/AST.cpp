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

#include "AST.h"

using PrettyHelper = Utilities::StringHelper;

/////////////////////////////////////////////////////////////
// Set newNode as root for the AST
void AST::setRoot(node* newNode)
{
	root = newNode;
}

/////////////////////////////////////////////////////////////
// Return root node pointer of the AST
node* AST::getRoot()
{
	return root;
}

/////////////////////////////////////////////////////////////
// Perform a DFS for the AST and print the nodes to console
void AST::treeWalk(node* element)
{
	static size_t indentLevel = 0;
	std::cout << "\n  " << std::string(2 * indentLevel, ' ') << element->show();
	auto iter = element->children.begin();
	++indentLevel;
	while (iter != element->children.end())
	{
		treeWalk(*iter);
		++iter;
	}
	--indentLevel;
}

///////////////////////////////////////////////////////////////////////////////////////
// Perform a DFS for the AST and pushback these nodes to the vector pointer 'element'
void AST::scopes(node* element, std::vector<std::string>& vec)
{
	vec.push_back(element->show().substr(1, element->show().length() - 2));
	auto iter = element->children.begin();
	while (iter != element->children.end())
	{
		scopes(*iter, vec);
		++iter;
	}
}

/////////////////////////////////////////////////////////////
// Return the complexity of node 'element'
size_t AST::getComplexity(node* element)
{
	size_t count = 1;
	std::vector<node*> *v = element->getChildren();
	for (node* child : *v)
	{
		count += AST::getComplexity(child);
	}
	return count;
}

#ifdef TEST_AST 
////////////////////////////////////////
// TEST STUB
void main() {
	node* root = new node;//Created an example root for the testing.
	root->name = "Root";
	root->type = "rootType";
	root->lineCountBegin = 0;
	root->lineCountEnd = 49;
	root->lineCount = root->lineCountEnd - root->lineCountBegin + 1;
	node* elem1 = new node; // Elements  for testing.
	elem1->name = "elem1";
	elem1->type = "elemType";
	elem1->lineCountBegin = 3;
	elem1->lineCountEnd = 6;
	elem1->lineCount = elem1->lineCountEnd - elem1->lineCountEnd + 1;
	node* elem2 = new node;
	elem2->name = "elem2";
	elem2->type = "elem2Type";
	elem2->lineCountBegin = 7;
	elem2->lineCountEnd = 13;
	elem2->lineCount = elem2->lineCountEnd - elem2->lineCountBegin + 1;
	node* elem3 = new node;
	elem3->name = "elem3";
	elem3->type = "elem3Type";
	elem3->lineCountBegin = 14;
	elem3->lineCountEnd = 21;
	elem3->lineCount = elem3->lineCountEnd - elem3->lineCountBegin + 1;
	node* elem4 = new node;
	elem4->name = "elem4";
	elem4->type = "function";
	elem4->lineCountBegin = 22;
	elem4->lineCountEnd = 48;
	elem4->lineCount = elem4->lineCountEnd - elem4->lineCountBegin + 1;
	AST *test_tree = new AST;
	test_tree->setRoot(root);
	root->addChild(elem1);
	root->addChild(elem2);
	elem1->addChild(elem3);
	elem2->addChild(elem4);
	root->complexity = test_tree->getComplexity(root); // setting complexity
	elem1->complexity = test_tree->getComplexity(elem1);
	elem2->complexity = test_tree->getComplexity(elem2);
	elem3->complexity = test_tree->getComplexity(elem3);
	elem4->complexity = test_tree->getComplexity(elem4);
	test_tree->treeWalk(test_tree->getRoot()); // testing tree walk
	std::cout << "\n\n";
}

#endif
