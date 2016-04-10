///////////////////////////////////////////////////////////////////////
//  ActionsAndRules.h - declares new parsing rules and actions       //
//  Version :	   2.0                                               //
//  Language:      Visual C++, Visual Sudio 15                       //
//  Platform:      MSI GE62 2QD, Core-i7, Windows 10		         //
//  Application:   Parallel Dependency Analysis						 //
//  Author:		   Venkata Chekuri, SUID : 356579351                 //
//                 vbchekur@syr.edu                                  //
///////////////////////////////////////////////////////////////////////

#include "ActionsAndRules.h"
Repository* Repository::instance = nullptr;

#ifdef TEST_ACTIONSANDRULES
////////////////////////////////////////////////
// TEST STUB
#include <iostream>
#include "Tokenizer.h"
#include "SemiExpression.h"

int main(int argc, char* argv[])
{
  std::cout << "\n  Testing ActionsAndRules class\n "
            << std::string(30,'=') << std::endl;

  try
  {
    std::queue<std::string> resultsQ;
    PreprocToQ ppq(resultsQ);
    PreprocStatement pps;
    pps.addAction(&ppq);

    FunctionDefinition fnd;
    PrettyPrintToQ pprtQ(resultsQ);
    fnd.addAction(&pprtQ);

    Toker toker("../ActionsAndRules.h");
    SemiExp se(&toker);
    Parser parser(&se);
    parser.addRule(&pps);
    parser.addRule(&fnd);
    while(se.get())
      parser.parse();
    size_t len = resultsQ.size();
    for(size_t i=0; i<len; ++i)
    {
      std::cout << "\n  " << resultsQ.front().c_str();
      resultsQ.pop();
    }
    std::cout << "\n\n";
  }
  catch(std::exception& ex)
  {
    std::cout << "\n\n  " << ex.what() << "\n\n";
  }
}
#endif
