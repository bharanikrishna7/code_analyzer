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

/////////////////////////////////////
/* Set newNode as root for the AST */
void AST::setRoot(element* newNode)
{
	root = newNode;
}

/////////////////////////////////////////
/* Return root node pointer of the AST */
element* AST::getRoot()
{
	return root;
}

//////////////////////////////////////////////////////////////
/* Perform a DFS for the AST and print the nodes to console */
void AST::treeWalk(element* element)
{
	static size_t indentLevel = 0;
	std::cout << "\n " << std::string(2 * indentLevel, ' ') << element->show();
	auto iter = element->children.begin();
	++indentLevel;
	while (iter != element->children.end())
	{
		treeWalk(*iter);
		++iter;
	}
	--indentLevel;
}

////////////////////////////////////////////////////////////////////////////////////////
/* Perform a DFS for the AST and pushback these nodes to the vector pointer 'element' */
void AST::scopes(element* element, std::vector<std::string>& vec)
{
	vec.push_back(element->show().substr(1, element->show().length() - 2));
	auto iter = element->children.begin();
	while (iter != element->children.end())
	{
		scopes(*iter, vec);
		++iter;
	}
}

/////////////////////////////////////////////
/* Return the complexity of node 'element' */
size_t AST::getComplexity(element* node)
{
	size_t count = 1;
	std::vector<element*> *v = node->getChildren();
	for (element* child : *v)
	{
		count += AST::getComplexity(child);
	}
	return count;
}

////////////////////////////////////////////////////
/* Helper function for setTreeComplexity function */
bool AST::setTreeComplexityHelper(element* element) {
	for (auto child : element->children) {
		setTreeComplexityHelper(child);
	}
	element->complexity = getComplexity(element);

	return true;
}

/////////////////////////////////////////////////
/* Set the complexity for all nodes in the AST */
bool AST::setTreeComplexity() {
	return setTreeComplexityHelper(root);
}

#ifdef TEST_AST

#include "../Utilities/Utilities.h"

using namespace Utilities;

/////////////////////////////////////////////////////
/* Create an element with information provided to it. 
Function for testing purpose only */
element* setElem(std::string name, std::string type, int lcBegin, int lcEnd) {
	element *newElem = new element;
	
	newElem->name = name;
	newElem->type = type;
	newElem->lineCountBegin = lcBegin;
	newElem->lineCountEnd = lcEnd;
	newElem->lineCount = lcEnd - lcBegin;

	return newElem;
}

///////////////
/* TEST STUB */
void main() {
	Timer timer;
	timer.StartClock();

	// Created an example root for the testing.
	element* root = setElem("Root", "RootType", 0, 49);
	// Other node(s) in the AST
	element* elem1 = setElem("Elem1", "NodeType", 3, 15);
	element* elem2 = setElem("Elem2", "NodeType2", 18, 40);
	element* elem3 = setElem("Elem3", "NodeType3", 21, 25);
	element* elem4 = setElem("Elem4", "NodeType4", 37, 39);
	// Declarijng an AST
	AST *test_tree = new AST;
	// Associating each node with other node(s) through AST
	test_tree->setRoot(root);
	root->addChild(elem1);
	root->addChild(elem2);
	elem2->addChild(elem3);
	elem2->addChild(elem4);
	// Set Complexity of all nodes in the Tree
	test_tree->setTreeComplexity();
	// testing tree walk
	test_tree->treeWalk(test_tree->getRoot()); 
	std::cout << "\n\n [Execution Time] : " << timer.StopClock() << " ms\n ";
	system("pause");
}
#endif
