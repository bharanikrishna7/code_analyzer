#ifndef CONFIGUREPARSER_H
#define CONFIGUREPARSER_H
//////////////////////////////////////////////////////////////////////////////
// ConfigureParser.h - builds and configures parsers						//
// Version :	  1.0                                                       //
// Language:      Visual C++, Visual Studio 2015							//
// Platform:      MSI GE62 2QD, Core-i7, Windows 10							//
// Application:   Code Parser with AST										//
// Author:		  Venkata Chekuri, SUID : 356579351							//
//                vbchekur@syr.edu											//
//////////////////////////////////////////////////////////////////////////////
/*
 * Module Operations:
 * -----------------
 * This module builds and configures parsers.  It builds the parser
 * parts and configures them with application specific rules and actions.
 *
 * Set the VERBOSE variable value in the ConfigParseToConsole class to TRUE
 * if it is desired for the output to attach print actions to the respective
 * rules.
 * 
 * Public Interface:
 * -----------------
 * ConfigParseToConsole conConfig;
 * conConfig.Build();
 * conConfig.Attach(someFileName);
 * void DefaultRules(Parser* parser);			// Helper function to hold Default scope rules
 * void FunctionRules(Parser* parser);			// Helper function to hold Function scope rules
 * void StructureRules(Parser* parser);			// Helper function to hold Structure scope rules
 * void ExecDeclarRules(Parser* parser);		// Helper function to hold Executable and Declaration rules
 * void ClassRules(Parser* parser);				// Helper function to hold Class scope rules
 * void NamespaceRules(Parser* parser);			// Helper function to hold Namespace scope rules
 * void ConditionalRules(Parser* parser);		// Helper function to hold conditional scope rules
 * void TryBlockRules(Parser* parser);			// Helper function to hold try scope rules
 *
 * Required files:
 * ---------------
 * ConfigureParser.h, ConfigureParser.cpp, Parser.h, Parser.cpp,
 * ActionsAndRules.h, ActionsAndRules.cpp,
 * SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp
 * 
 * Build command:
 * --------------
 * devenv Parser.sln /rebuild debug
 *
 * Maintenance History:
 * --------------------
 * ver 1.0 : 17 March 16
 * - first release
 *
 */

#include <fstream>
#include "Parser.h"
#include "../SemiExp/SemiExp.h"
#include "../Tokenizer/Tokenizer.h"
#include "ActionsAndRules.h"

typedef int32 type1, type2;
using aliasTrial = std::vector<int>;

///////////////////////////////////////////////////////////////
// build parser that writes its output to console
class ConfigParseToConsole : IBuilder
{
public:
  ConfigParseToConsole() {};
  ~ConfigParseToConsole();
  bool Attach(const std::string& name, bool isFile=true);
  
  //These functions will hold rules for the parser
  void DefaultRules(Parser* parser);
  void FunctionRules(Parser* parser);
  void StructureRules(Parser* parser);
  void ExecDeclarRules(Parser* parser);
  void ClassRules(Parser* parser);
  void NamespaceRules(Parser* parser);
  void ConditionalRules(Parser* parser);
  void TryBlockRules(Parser* parser);
  void EnumRules(Parser* parser);
  Parser* Build();
private:
  // Builder must hold onto all the pieces
  std::ifstream* pIn;
  Scanner::Toker* pToker;
  Scanner::SemiExp* pSemi;
  Parser* pParser;
  Repository* pRepo;

  // add Rules and Actions
  BeginningOfScope* pBeginningOfScope;
  HandlePush* pHandlePush;
  EndOfScope* pEndOfScope;
  HandlePop* pHandlePop;
  FunctionDefinition* pFunctionDefinition;
  PushFunction* pPushFunction;
  PrintFunction* pPrintFunction;
  Declaration* pDeclaration;
  ShowDeclaration* pShowDeclaration;
  Executable* pExecutable;
  ShowExecutable* pShowExecutable;
  StructureDefinition* pStructureDefinition;
  PushStructure* pPushStructure;
  PrintStructure* pPrintStructure;
  ClassDefinition* pClassDefinition;
  PushClass* pPushClass;
  PrintClass* pPrintClass;
  NamespaceDefinition* pNamespaceDefinition;
  PushNamespace* pPushNamespace;
  PrintNamespace* pPrintNamespace;
  ConditionalDefinition* pConditionalDefinition;
  PushConditional* pPushConditional;
  PrintConditional* pPrintConditional;
  TryDefinition* pTryDefinition;
  PushTry* pPushTry;
  PrintTry* pPrintTry;
  EnumDefinition* pEnumDefinition;
  PushEnum* pPushEnum;
  PrintEnum* pPrintEnum;

  // prohibit copies and assignments
  ConfigParseToConsole(const ConfigParseToConsole&) = delete;
  ConfigParseToConsole& operator=(const ConfigParseToConsole&) = delete;

  // Verbose Variable. Set it to true to attach print actions.
  bool VERBOSE = true;
};
#endif
