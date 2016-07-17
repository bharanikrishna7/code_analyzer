#ifndef ACTIONSANDRULES_H
#define ACTIONSANDRULES_H
///////////////////////////////////////////////////////////////////////
//  ActionsAndRules.h - declares new parsing rules and actions       //
//  Version :	   2.0                                               //
//  Language:      Visual C++, Visual Sudio 15                       //
//  Platform:      MSI GE62 2QD, Core-i7, Windows 10		         //
//  Application:   Parallel Dependency Analysis						 //
//  Author:		   Venkata Chekuri, SUID : 356579351                 //
//                 vbchekur@syr.edu                                  //
///////////////////////////////////////////////////////////////////////
/*
 *  Module Operations: 
 *  ------------------
 *  This module defines several action classes.  Its classes provide
 *  specialized services needed for specific applications.  The modules
 *  Parser, SemiExpression, and Tokenizer, are intended to be reusable
 *  without change.  This module provides a place to put extensions of
 *  these facilities and is not expected to be reusable.
 *
 *  Public Interface:
 *  -----------------
 *  Toker t(someFile);              // create tokenizer instance
 *  SemiExp se(&t);                 // create a SemiExp attached to tokenizer
 *  Parser parser(se);              // now we have a parser
 *  Rule1 r1;                       // create instance of a derived Rule class
 *  Action1 a1;                     // create a derived action
 *  r1.addAction(&a1);              // register action with the rule
 *  parser.addRule(&r1);            // register rule with parser
 *  while(se.getSemiExp())          // get semi-expression
 *  parser.parse();					// and parse it
 *
 *  Required files:
 *  ---------------
 *  Parser.h, Parser.cpp, ScopeStack.h, ScopeStack.cpp,
 *  ActionsAndRules.h, ActionsAndRules.cpp, ConfigureParser.cpp,
 *  ItokCollection.h, SemiExpression.h, SemiExpression.cpp, Tokenizer.h, Tokenizer.cpp
 * 
 *  Build commands:
 *  ---------------
 *  devenv Parser.sln /rebuild debug
 *
 *  Maintenance History:
 *  --------------------
 *  ver 1.0 : 17 March 2016
 *  - first release
 *  ver 2.0 : 1 April 2016
 *  - Added Type Table creation along with AST
 *  - Added rules to detect enum, function type (member / global), typedef, alias
 *
 * Credits:
 * --------
 * I would like to give credits to Prof. Jim W Fawcett Syracuse University
 * , CST 4-187) for providing a started code and explaining how Parallel
 * Dependency Analysis project should work.
 *
 */

#include <queue>
#include <string>
#include <sstream>
#include "Parser.h"
#include "TypeTable.h"
#include "ScopeStack.h"
#include "../AST/AST.h"
#include "../SemiExpression/ITokCollection.h"
#include "../Tokenizer/Tokenizer.h"
#include "../SemiExpression/SemiExp.h"

/////////////////////////////////////////////////
/* Repository instance is used to share resources 
 among all actions */
/*
 * ToDo:
 * Make Type Table generation process not depend on other projects
 * for complete partial type table creation.
 *
 */
class Repository  // application specific
{
  ScopeStack<element*> stack;
  Scanner::Toker* p_Toker;
  AST *tree;
  TTable *table;
  static Repository* instance;
public:
	bool VERBOSE = false;
	Repository(Scanner::Toker* pToker)
  {
	instance = this;
	p_Toker = pToker;
	tree = new AST();
	table = new TTable();
	element* root = new element;
	root->name = "global";
	root->type = "namespace";
	root->lineCountBegin = 0;
	tree->setRoot(root);
	stack.push(root);
  }
  TTable* getTable() {
	  return table;
  }
  AST* getTree() {
	  return tree;
  }
  static Repository* getInsance()
  {
	  return instance;
  }
  ScopeStack<element*>& scopeStack()
  {
    return stack;
  }
  Scanner::Toker* Toker()
  {
    return p_Toker;
  }
  size_t lineCount()
  {
    return (size_t)(p_Toker->currentLineCount());
  }
  void setVerbose(bool paramV) {
	  VERBOSE = paramV;
  }
};

/////////////////////////////////////////////////
/* rule to detect beginning of anonymous scope */
class BeginningOfScope : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    if(pTc->find("{") < pTc->length())
    {
      doActions(pTc);
      return true;
    }
    return true;
  }
};

//////////////////////////////////////////////////
/* action to handle scope stack at end of scope */
class HandlePush : public IAction
{
  Repository* p_Repos;
public:
  HandlePush(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    //std::cout << "\n--BeginningOfScope rule";
    element* elem = new element;
    elem->type = "unknown";
    elem->name = "anonymous";
    elem->lineCountBegin = p_Repos->lineCount();
    p_Repos->scopeStack().top()->addChild(elem);
	p_Repos->scopeStack().push(elem);
  }
};

/////////////////////////////////
/* rule to detect end of scope */
class EndOfScope : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    if(pTc->find("}") < pTc->length())
    {
      doActions(pTc);
      return true;
    }
    return true;
  }
};

//////////////////////////////////////////////////
/* action to handle scope stack at end of scope */
class HandlePop : public IAction
{
  Repository* p_Repos;
public:
  HandlePop(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    if(p_Repos->scopeStack().size() == 0)
      return;
    element* elem = p_Repos->scopeStack().pop();
	elem->lineCountEnd = p_Repos->lineCount();
	elem->lineCount = elem->lineCountEnd - elem->lineCountBegin + 1;
	elem->complexity = p_Repos->getTree()->getComplexity(elem);
	// For correct namespace assignment to the type table elements
	if (elem->type == "namespace") { p_Repos->getTable()->addEntry("EOS", "namespace EOS"); }

	if (p_Repos->VERBOSE) {
		if (elem->type == "function" || elem->type == "function (member)" || elem->type == "structure" ||
			elem->type == "class" || elem->type == "namespace" || elem->type == "conditional" ||
			elem->type == "enum" || elem->type == "enum (strong)")
		{
			std::cout << "\n [VERBOSE] : End of Scope \"" << elem->name << "\" Scope begin Line = " << elem->lineCountBegin;
			std::cout << ", Scope end Line = " << elem->lineCountEnd << ", Scope Size = " << elem->lineCount;
		}
	}
  }
};

////////////////////////////////////////////
/* rule to detect preprocessor statements */
class PreprocStatement : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    if(pTc->find("#") < pTc->length())
    {
      doActions(pTc);
      return true;
    }
    return true;
  }
};

/////////////////////////////////////////////////////////////////////
/* action to print preprocessor statement to console (Depreceated) */
class PrintPreproc : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
	  std::cout << "\n\n  Preproc Stmt: " << pTc->show().c_str();
  }
};

//////////////////////////////////////////
/* rule to detect structure definitions */
class StructureDefinition : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc.find("struct") < tc.length())
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

/////////////////////////////////////
/* rule to detect enum definitions */
class EnumDefinition : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc.find("enum") < tc.length())
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

//////////////////////////////////////////
/* rule to detect namespace definitions */
class NamespaceDefinition : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc.find("namespace") < tc.length() && !(tc.find("using") < tc.length()))
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

//////////////////////////////////////
/* rule to detect class definitions */
class ClassDefinition : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		if (pTc->find("class") < pTc->length() && !(pTc->find("enum") < pTc->find("class")))
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

//////////////////////////////////////////////////
/* rule to detect Conditional Scope definitions */
class ConditionalDefinition : public IRule
{
public:
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "catch", "do" };
		for (int i = 0; i<6; ++i)
			if (tok == keys[i])
				return true;
		return false;
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			size_t len = tc.find("(");
			if (len < tc.length() && isSpecialKeyWord(tc[len - 1]))
			{
				doActions(pTc);
				return true;
			}
		}
		return true;
	}
};

///////////////////////////////////////////
/* rule to detect Try blocks definitions */
class TryDefinition : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc.find("try") < tc.length())
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

//////////////////////////////////////////////////////
/* rule to detect typedef (not associated with AST) */
class TypeDefDefinition : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc.find("typedef") < tc.length())
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

//////////////////////////////////////////////////////
/* rule to detect aliases (not associated with AST) */
class AliasDefinition : public IRule
{
public:
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc.find("using") < tc.length() && tc.find("=") < tc.length())
		{
			doActions(pTc);
			return true;
		}
		return true;
	}
};

///////////////////////////////////////////////////////////////////////////////////
/* action to add typedef declarations to the Typetable (not associated with AST) */
class AddEntryTypeDef : public IAction
{
	Repository* p_Repos;
public:
	AddEntryTypeDef(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		if (pTc->find(",") < pTc->length()) {
			// Multiple declarations are present i.e something like - typedef int x,y;
			for (size_t index = pTc->find(";") - 1; index > 0; index--) {
				if (p_Repos->VERBOSE) { std::cout << "\n [VERBOSE] : Adding Typedef \"" << (*pTc)[index] << "\" to Partial Type Table"; }
				p_Repos->getTable()->addEntry((*pTc)[index], "typedef");
				index--;
				if ((*pTc)[index] != ",")
					break;
			}
		}
		else
		{
			p_Repos->getTable()->addEntry((*pTc)[pTc->find(";") - 1], "typedef");
		}
	}
};

/////////////////////////////////////////////////////////////////////////////////
/* action to add alias declarations to the Typetable (not associated with AST) */
class AddEntryAlias : public IAction
{
	Repository* p_Repos;
public:
	AddEntryAlias(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		if (p_Repos->VERBOSE) { std::cout << "\n [VERBOSE] : Adding Alias \"" << (*pTc)[pTc->find("using") + 1] << "\" to Partial Type Table"; }
		p_Repos->getTable()->addEntry((*pTc)[pTc->find("using") + 1], "alias");
	}
};

//////////////////////////////////////////////////////////
/* action to push Try blocks scope name onto ScopeStack */
class PushTry : public IAction
{
	Repository* p_Repos;
public:
	PushTry(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		// pop anonymous scope
		element* dElem = p_Repos->scopeStack().pop();
		p_Repos->scopeStack().top()->deleteChild(p_Repos->scopeStack().top(), dElem);

		// push try scope
		element* elem = new element;
		elem->type = "conditional";
		elem->name = "try";
		elem->lineCountBegin = p_Repos->lineCount();
		p_Repos->scopeStack().top()->addChild(elem);
		p_Repos->scopeStack().push(elem);
		if (p_Repos->VERBOSE) { std::cout << "\n [VERBOSE] : Adding Conditional Block of Type \"Try-Catch\" to Scope Stack"; }
	}
};

///////////////////////////////////////////////////////////
/* action to push conditional scope name onto ScopeStack */
class PushConditional : public IAction
{
	Repository* p_Repos;
public:
	PushConditional(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		// pop anonymous scope
		element* dElem = p_Repos->scopeStack().pop();
		p_Repos->scopeStack().top()->deleteChild(p_Repos->scopeStack().top(), dElem);

		// push conditional scope
		std::string name = (*pTc)[pTc->find("(") - 1];
		element* elem = new element;
		elem->type = "conditional";
		elem->name = name;
		elem->lineCountBegin = p_Repos->lineCount();
		p_Repos->scopeStack().top()->addChild(elem);
		p_Repos->scopeStack().push(elem);
		if (p_Repos->VERBOSE) { std::cout << "\n [VERBOSE] : Adding Conditional Block of Type \"" << name << "\" to Scope Stack"; }
	}
};

////////////////////////////////////////////////////////////////
/* action to send semi-expression that starts a Try scope def 
to console (Depreceated) */
class PrintTry : public IAction
{
	Repository* p_Repos;
public:
	PrintTry(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		std::cout << "\n  TryDef: " << pTc->show().c_str();
	}
};

//////////////////////////////////////////////////////////////////////////
/* action to send semi-expression that defines a typedef/alias to console 
(Depreceated) */
class PrintDeclaration : public IAction
{
	Repository* p_Repos;
public:
	PrintDeclaration(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		std::cout << "\n  Following Declaration is added to the TypeTable:" << pTc->show().c_str();
	}
};

//////////////////////////////////////////////////////////////////////
/* action to send semi-expression that starts a conditional scope def 
to console (Depreceated) */
class PrintConditional : public IAction
{
	Repository* p_Repos;
public:
	PrintConditional(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		std::cout << "\n  ConditionalDef: " << pTc->show().c_str();
	}
};

/////////////////////////////////////////
/* rule to detect function definitions */
class FunctionDefinition : public IRule
{
public:
  bool isSpecialKeyWord(const std::string& tok)
  {
    const static std::string keys[]
      = { "for", "while", "switch", "if", "catch", "do", "try" };
    for(int i=0; i<7; ++i)
      if(tok == keys[i])
        return true;
    return false;
  }
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    if(tc[tc.length()-1] == "{")
    {
      size_t len = tc.find("(");
      if(len < tc.length() && !isSpecialKeyWord(tc[len-1]))
      {
        doActions(pTc);
        return true;
      }
    }
    return true;
  }
};

///////////////////////////////////////////////////
/* action to push structure name onto ScopeStack */
class PushStructure : public IAction
{
	Repository* p_Repos;
public:
	PushStructure(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		// pop anonymous scope
		element* dElem = p_Repos->scopeStack().pop();
		p_Repos->scopeStack().top()->deleteChild(p_Repos->scopeStack().top(), dElem);

		// push structure scope
		std::string name = (*pTc)[pTc->find("struct") + 1];
		element* elem = new element;
		elem->type = "structure";
		elem->name = name;
		elem->lineCountBegin = p_Repos->lineCount();
		p_Repos->scopeStack().top()->addChild(elem);
		p_Repos->scopeStack().push(elem);
		p_Repos->getTable()->addEntry(elem->name, elem->type);
		if (p_Repos->VERBOSE) { std::cout << "\n [VERBOSE] : Adding Structure \"" << name << "\" to Scope Stack"; }
	}
};

//////////////////////////////////////////////
/* action to push enum name onto ScopeStack */
class PushEnum : public IAction
{
	Repository* p_Repos;
public:
	PushEnum(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	bool checkNoName(const std::string& tok)
	{
		if (tok == "\n")
			return true;
		return false;
	}
	void doAction(ITokCollection*& pTc)
	{
		// pop anonymous scope
		element* dElem = p_Repos->scopeStack().pop();
		p_Repos->scopeStack().top()->deleteChild(p_Repos->scopeStack().top(), dElem);
		element* elem = new element;
		std::string name;
		if(pTc->find("class") < pTc->length()) { 
			name = (*pTc)[pTc->find("enum") + 2];
			elem->type = "enum (strong)";
		}
		else {
			name = (*pTc)[pTc->find("enum") + 1];
			elem->type = "enum";
		}
		if (checkNoName(name))
			name = "---";
		elem->name = name;
		elem->lineCountBegin = p_Repos->lineCount();
		// push enum scope
		p_Repos->scopeStack().top()->addChild(elem);
		p_Repos->scopeStack().push(elem);
		p_Repos->getTable()->addEntry(elem->name, elem->type);
		if (p_Repos->VERBOSE) { std::cout << "\n [VERBOSE] : Adding Enumerator \"" << name << "\" to Scope Stack"; }
	}
};

///////////////////////////////////////////////////
/* action to push namespace name onto ScopeStack */
class PushNamespace : public IAction
{
	Repository* p_Repos;
public:
	PushNamespace(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		// pop anonymous scope
		element* dElem = p_Repos->scopeStack().pop();
		p_Repos->scopeStack().top()->deleteChild(p_Repos->scopeStack().top(), dElem);

		// push namespace scope
		std::string name = (*pTc)[pTc->find("namespace") + 1];
		element* elem = new element;
		elem->type = "namespace";
		elem->name = name;
		elem->lineCountBegin = p_Repos->lineCount();
		p_Repos->scopeStack().top()->addChild(elem);
		p_Repos->scopeStack().push(elem);
		p_Repos->getTable()->addEntry(elem->name, elem->type, "-");
		if (p_Repos->VERBOSE) { std::cout << "\n [VERBOSE] : Adding Namespace \"" << name << "\" to Scope Stack"; }
	}
};

///////////////////////////////////////////////
/* action to push class name onto ScopeStack */
class PushClass : public IAction
{
	Repository* p_Repos;
public:
	PushClass(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		// pop anonymous scope
		element* dElem = p_Repos->scopeStack().pop();
		p_Repos->scopeStack().top()->deleteChild(p_Repos->scopeStack().top(), dElem);

		// push class scope
		std::string name = (*pTc)[pTc->find("class") + 1];
		element* elem = new element;
		elem->type = "class";
		elem->name = name;
		elem->lineCountBegin = p_Repos->lineCount();
		p_Repos->scopeStack().top()->addChild(elem);
		p_Repos->scopeStack().push(elem);
		p_Repos->getTable()->addEntry(elem->name, elem->type);
		if (p_Repos->VERBOSE) { std::cout << "\n [VERBOSE] : Adding Class \"" << name << "\" to Scope Stack"; }
	}
};

//////////////////////////////////////////////////
/* action to push function name onto ScopeStack */
class PushFunction : public IAction
{
  Repository* p_Repos;
public:
  PushFunction(Repository* pRepos) {
    p_Repos = pRepos;
  }
  bool isLambda(const std::string& tok) {
	  if (tok == "]")
		  return true;
	  return false;
  }
  void doAction(ITokCollection*& pTc) {
    // pop anonymous scope

	element* dElem = p_Repos->scopeStack().pop();
	p_Repos->scopeStack().top()->deleteChild(p_Repos->scopeStack().top(), dElem);

    // push function scope
	element* elem = new element;
	std::string name = (*pTc)[pTc->find("(") - 1];
	elem->type = "function";
	
	if (isLambda(name)) {
		name = "(Lambda)";
	}

	elem->name = name;
	elem->lineCountBegin = p_Repos->lineCount();
	p_Repos->scopeStack().top()->addChild(elem);
	p_Repos->scopeStack().push(elem);
	if (pTc->find("::") < pTc->length()) {
		p_Repos->getTable()->addEntry(elem->name, "function (member)");
		if (p_Repos->VERBOSE) { std::cout << "\n [VERBOSE] : Adding Function(member) \"" << name << "\" to Scope Stack"; }
	}
	else {
		p_Repos->getTable()->addEntry(elem->name, "function");
		if (p_Repos->VERBOSE) { std::cout << "\n [VERBOSE] : Adding Function \"" << name << "\" to Scope Stack"; }
	}
  }
};

//////////////////////////////////////////////////////////
/* action to print semi-expression that starts a class def 
to console (Depreceated) */
class PrintClass : public IAction
{
	Repository* p_Repos;
public:
	PrintClass(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		std::cout << "\n  ClassDef: " << pTc->show().c_str();
		std::cout << "  Class added to the TypeTable\n";
	}
};

//////////////////////////////////////////////////////////
/* action to print semi-expression that starts a class def 
to console (Depreceated) */
class PrintNamespace : public IAction
{
	Repository* p_Repos;
public:
	PrintNamespace(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		std::cout << "\n  NamespaceDef: " << pTc->show().c_str();
		std::cout << "  Namespace added to the TypeTable\n";
	}
};

//////////////////////////////////////////////////////////////
/* action to print semi-expression that starts a structure def 
to console (Depreceated) */
class PrintStructure : public IAction
{
	Repository* p_Repos;
public:
	PrintStructure(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		std::cout << "\n  StructDef: " << pTc->show().c_str();
		std::cout << "  Structure added to the TypeTable\n";
	}
};

/////////////////////////////////////////////////////////
/* action to send semi-expression that starts a enum def 
to console (Depreceated) */
class PrintEnum : public IAction
{
	Repository* p_Repos;
public:
	PrintEnum(Repository* pRepos)
	{
		p_Repos = pRepos;
	}
	void doAction(ITokCollection*& pTc)
	{
		std::cout << "\n  EnumDef: " << pTc->show().c_str();
		std::cout << "  Enumerator added to the TypeTable\n";
	}
};

/////////////////////////////////////////////////////////////
/* action to send semi-expression that starts a function def 
to console (Depreceated) */
class PrintFunction : public IAction
{
  Repository* p_Repos;
public:
  PrintFunction(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
	  std::cout << "\n  FuncDef: " << pTc->show().c_str();
	  std::cout << " Function added to the TypeTable\n";
  }
};

///////////////////////////////////////////////////////////
// action to send signature of a function def to console */
class PrettyPrintFunction : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    pTc->remove("public");
    pTc->remove(":");
    pTc->trimFront();
    int len = pTc->find(")");
    std::cout << "\n\n  Pretty Stmt:    ";
    for(int i=0; i<len+1; ++i)
      std::cout << (*pTc)[i] << " ";
    std::cout << "\n";
  }
};

////////////////////////////////
/* rule to detect declaration */
class Declaration : public IRule          // declar ends in semicolon
{                                         // has type, name, modifiers &
public:                                   // initializers.  So eliminate
  bool isModifier(const std::string& tok) // modifiers and initializers.
  {                                       // If you have two things left
    const size_t numKeys = 21;            // its declar else executable.
    const static std::string keys[numKeys] = {
      "const", "extern", "friend", "mutable", "signed", "static",
      "typedef", "typename", "unsigned", "volatile", "&", "*", "std", "::",
      "public", "protected", "private", ":", "typename", "++", "--"
    };
    for (int i = 0; i<numKeys; ++i)
    if (tok == keys[i])
      return true;
    return false;
  }
  void condenseTemplateTypes(ITokCollection& tc)
  {
    size_t start = tc.find("<");
    size_t end = tc.find(">");
    if (start >= end || start == 0)
      return;
    else
    {
      if (end == tc.length())
        end = tc.find(">::");
      if (end == tc.length())
        return;
    }
    std::string save = tc[end];
    std::string tok = tc[start - 1];
    for (size_t i = start; i < end + 1; ++i)
      tok += tc[i];
    for (size_t i = start; i < end + 1; ++i)
      tc.remove(start);
    if (save == ">::")
    {
      tok += tc[start + 1];
      tc.remove(start);
    }
    tc[start - 1] = tok;
  }
  bool isSpecialKeyWord(const std::string& tok)
  {
	  const static std::string keys[]
		  = { "for", "while", "switch", "if", "catch", "do", "try" };
	  for (int i = 0; i<7; ++i)
		  if (tok == keys[i])
			  return true;
	  return false;
  }
  void removeInvocationParens(ITokCollection& tc)
  {
    size_t start = tc.find("(");
    size_t end = tc.find(")");
    if (start >= end || end == tc.length() || start == 0)
      return;
    if (isSpecialKeyWord(tc[start - 1]))
      return;
    for (size_t i = start; i < end + 1; ++i)
      tc.remove(start);
  }
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& in = *pTc;
    Scanner::SemiExp tc;
    for (size_t i = 0; i<in.length(); ++i)
      tc.push_back(in[i]);
    if (tc[tc.length() - 1] == ";" && tc.length() > 2)
    {
      removeInvocationParens(tc);
      condenseTemplateTypes(tc);

      // remove modifiers, comments, newlines, returns, and initializers
      Scanner::SemiExp se;
      for (size_t i = 0; i < tc.length(); ++i)
      {
		if ((isModifier(tc[i])) || (se.isComment(tc[i]) || tc[i] == "\n" || tc[i] == "return"))
		  continue;
        if (tc[i] == "=" || tc[i] == ";")
          break;
        else
          se.push_back(tc[i]);
      }

      if (se.length() == 2)  // type & name, so declaration
      {
        doActions(pTc);
        return true;
      }
    }
    return true;
  }
};

////////////////////////////////
/* action to show declaration */
class ShowDeclaration : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    // remove comments
    Scanner::SemiExp se;
    for (size_t i = 0; i<tc.length(); ++i)
      if (!se.isComment(tc[i]))
        se.push_back(tc[i]);
    // show cleaned semiExp
  }
};

///////////////////////////////
/* rule to detect executable */
class Executable : public IRule           // declar ends in semicolon
{                                         // has type, name, modifiers &
public:                                   // initializers.  So eliminate
  bool isModifier(const std::string& tok) // modifiers and initializers.
  {                                       // If you have two things left
    const size_t numKeys = 21;            // its declar else executable.
    const static std::string keys[numKeys] = {
      "const", "extern", "friend", "mutable", "signed", "static",
      "typedef", "typename", "unsigned", "volatile", "&", "*", "std", "::",
      "public", "protected", "private", ":", "typename", "++", "--"
    };
    for (int i = 0; i<numKeys; ++i)
    if (tok == keys[i])
      return true;
    return false;
  }
  void condenseTemplateTypes(ITokCollection& tc)
  {
    size_t start = tc.find("<");
    size_t end = tc.find(">");
    if (start >= end || start == 0)
      return;
    else
    {
      if (end == tc.length())
        end = tc.find(">::");
      if (end == tc.length())
        return;
    }
    std::string save = tc[end];
    std::string tok = tc[start - 1];
    for (size_t i = start; i < end+1; ++i)
      tok += tc[i];
    for (size_t i = start; i < end+1; ++i)
      tc.remove(start);
    if (save == ">::")
    {
      tok += tc[start + 1];
      tc.remove(start);
    }
    tc[start - 1] = tok;
  }
  bool isSpecialKeyWord(const std::string& tok)
  {
	  const static std::string keys[]
		  = { "for", "while", "switch", "if", "catch", "do", "try" };
	  for (int i = 0; i<7; ++i)
		  if (tok == keys[i])
			  return true;
	  return false;
  }
  void removeInvocationParens(ITokCollection& tc)
  {
    size_t start = tc.find("(");
    size_t end = tc.find(")");
    if (start >= end || end == tc.length() || start == 0)
      return;
    if (isSpecialKeyWord(tc[start - 1]))
      return;
    for (size_t i = start; i < end + 1; ++i)
      tc.remove(start);
  }
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& in = *pTc;
    Scanner::SemiExp tc;
    for (size_t i = 0; i<in.length(); ++i)
      tc.push_back(in[i]);

    if (tc[tc.length() - 1] == ";" && tc.length() > 2)
    {
      removeInvocationParens(tc);
      condenseTemplateTypes(tc);

      // remove modifiers, comments, newlines, returns, and initializers
      Scanner::SemiExp se;
      for (size_t i = 0; i < tc.length(); ++i)
      {
		if ((isModifier(tc[i])) || (se.isComment(tc[i]) || tc[i] == "\n" || tc[i] == "return"))
		  continue;
        if (tc[i] == "=" || tc[i] == ";")
          break;
        else
          se.push_back(tc[i]);
      }
      if (se.length() != 2)  // not a declaration
      {
        doActions(pTc);
        return true;
      }
    }
    return true;
  }
};

///////////////////////////////
/* action to show executable */
class ShowExecutable : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    // remove comments
    Scanner::SemiExp se;
    for (size_t i = 0; i < tc.length(); ++i)
    {
      if (!se.isComment(tc[i]))
        se.push_back(tc[i]);
    }
    // show cleaned semiExp
  }
};

#endif