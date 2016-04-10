//////////////////////////////////////////////////////////////////////////////
//  Parser.h - Analyzes C++ language constructs								//
// Version :	  1.0                                                       //
// Language:      Visual C++, Visual Studio 2015							//
// Platform:      MSI GE62 2QD, Core-i7, Windows 10							//
// Application:   Parallel Dependency Analysis								//
// Author:		  Venkata Chekuri, SUID : 356579351							//
//                vbchekur@syr.edu											//
//////////////////////////////////////////////////////////////////////////////

#include <string>
#include <iostream>
#include "Parser.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"
#include "../SemiExp/SemiExp.h"
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Tokenizer.h"
#include "../FileSystem-Windows/FileSystemDemo/FileSystem.h"

using namespace Scanner;
using namespace Utilities;

////////////////////////////////////////////////
// register parsing rule 
void Parser::addRule(IRule* pRule)
{
  rules.push_back(pRule);
}

////////////////////////////////////////////////
// get next ITokCollection 
bool Parser::next() 
{ 
  // default operation of doFold(pTokColl) is to simply return
  // - code analysis provides specific rules for handling
  //   for statements and operator expressions which are
  //   bound in ConfigureParser.cpp

  bool succeeded = pTokColl->get();
  if(!succeeded)
    return false;
  return true;
}

//////////////////////////////////////////////////
// parse the SemiExp by applying all rules to it 
bool Parser::parse()
{
  bool succeeded = false;
  for(size_t i=0; i<rules.size(); ++i)
  {
    if(rules[i]->doTest(pTokColl))
      succeeded = true;
  }
  return succeeded;
}

////////////////////////////////////////////////
// register action with a rule 
void IRule::addAction(IAction *pAction)
{
  actions.push_back(pAction);
}

////////////////////////////////////////////////
// invoke all actions associated with a rule 
void IRule::doActions(ITokCollection*& pTokColl)
{
  if(actions.size() > 0)
    for(size_t i=0; i<actions.size(); ++i)
      actions[i]->doAction(pTokColl);
}


#ifdef TEST_PARSER
////////////////////////////////////////////////
// TEST STUB
#include <queue>
#include <string>
#define Util StringHelper

void printAST()
{
	Repository* repo = Repository::getInsance();
	//Retriving Root Element to set its linecount and Complexity.
	node* rootElem = repo->getTree()->getRoot();
	rootElem->lineCountEnd = repo->lineCount();
	rootElem->lineCount = rootElem->lineCountEnd - rootElem->lineCountBegin + 1;
	rootElem->complexity = repo->getTree()->getComplexity(rootElem);
	//Printing AST on the command prompt
	Util::title("AST :");
	putline();
	repo->getTree()->treeWalk(repo->getTree()->getRoot());
}

int main(int argc, char* argv[])
{
  Util::Title("Testing Parser Class", '=');
  putline();

  // collecting tokens from files, named on the command line
  if(argc < 2)
  {
    std::cout 
      << "\n  please enter name of file to process on command line\n\n";
    return 1;
  }

  for(int i=1; i<argc; ++i)
  {
    std::string fileSpec = FileSystem::Path::getFullFileSpec(argv[i]);
    std::string msg = "Processing file" + fileSpec;
    Util::title(msg);
    putline();
    ConfigParseToConsole configure;
    Parser* pParser = configure.Build();
    try
    {
      if(pParser)
      {
        if(!configure.Attach(argv[i]))
        {
          std::cout << "\n  could not open file " << fileSpec << std::endl;
          continue;
        }
      }
      else
      {
        std::cout << "\n\n  Parser not built\n\n";
        return 1;
      }
      // now that parser is built, use it
      while(pParser->next())
        pParser->parse();
	  //Print AST on console
	  printAST();
    }
    catch(std::exception& ex)
    {
      std::cout << "\n\n    " << ex.what() << "\n\n";
    }
  }
  std::cout << "\n";
}

#endif
