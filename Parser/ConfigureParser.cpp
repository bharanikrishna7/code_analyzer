//////////////////////////////////////////////////////////////////////////////
// ConfigureParser.h - builds and configures parsers						//
// Version :	  1.1                                                       //
// Language:      Visual C++, Visual Studio 2015							//
// Platform:      MSI GE62 2QD, Core-i7, Windows 10							//
// Application:   Parallel Dependency Analysis								//
// Author:		  Venkata Chekuri, SUID : 356579351							//
//                vbchekur@syr.edu											//
//////////////////////////////////////////////////////////////////////////////

#include <fstream>
#include "Parser.h"
#include "../SemiExp/SemiExp.h"
#include "../Tokenizer/Tokenizer.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"

using namespace Scanner;
//----< destructor releases all parts >------------------------------
ConfigParseToConsole::~ConfigParseToConsole()
{
  // when Builder goes out of scope, everything must be deallocated
  delete pHandlePush;
  delete pBeginningOfScope;
  delete pHandlePop;
  delete pEndOfScope;
  delete pPrintFunction;
  delete pPushFunction;
  delete pFunctionDefinition;
  delete pStructureDefinition;
  delete pPrintStructure;
  delete pPushStructure;
  delete pClassDefinition;
  delete pPushClass;
  delete pPrintClass;
  delete pEnumDefinition;
  delete pPushEnum;
  delete pPrintEnum;
  delete pNamespaceDefinition;
  delete pPushNamespace;
  delete pPrintNamespace;
  delete pConditionalDefinition;
  delete pPushConditional;
  delete pPrintConditional;
  delete pTryDefinition;
  delete pPushTry;
  delete pPrintTry;
  delete pDeclaration;
  delete pShowDeclaration;
  delete pExecutable;
  delete pShowExecutable;
  delete pRepo;
  delete pParser;
  delete pSemi;
  delete pToker;
  pIn->close();
  delete pIn;
}

////////////////////////////////////////////////
// attach toker to a file stream or stringstream
bool ConfigParseToConsole::Attach(const std::string& name, bool isFile)
{
  if(pToker == 0)
    return false;
  pIn = new std::ifstream(name);
  if (!pIn->good())
    return false;
  return pToker->attach(pIn);
}

////////////////////////////////////////////////
// Here's where all the parts get assembled 
Parser* ConfigParseToConsole::Build()
{
  try
  {
    // add Parser's main parts
    pToker = new Toker;
    pToker->returnComments(false);
    pSemi = new SemiExp(pToker);
    pParser = new Parser(pSemi);
    pRepo = new Repository(pToker);
	
	DefaultRules(pParser);					// configure to manage scope these must come first - they return true on match so rule checking continues
	EnumRules(pParser);						// configure to detect and act on enum block scopes  *** [ For both AST + TypeTable ]
	ClassRules(pParser);					// configure to detect and act on classes  *** [ For both AST + TypeTable ]
	StructureRules(pParser);				// configure to detect and act on structures  *** [ For both AST + TypeTable ]
	FunctionRules(pParser);					// configure to detect and act on function definitions
											// these will stop further rule checking by returning false *** [ For both AST + TypeTable ]
	TypeDefDetectRules(pParser);			// configure to detect typedef declarations *** [ Only For TypeTable ]
	AliasDetectRules(pParser);				// configure to detect alias declarations *** [ Only For TypeTable ]
	ExecDeclarRules(pParser);				// configure to detect and act on declarations and Executables *** [ Does nothing, just detects and passes over ]
	NamespaceRules(pParser);				// configure to detect and act on Namespaces *** [ AST + TypeTable]
	ConditionalRules(pParser);				// configure to detect and act on conditional scopes *** [ For AST Only ]
	TryBlockRules(pParser);					// configure to detect and act on try block scopes *** [ For AST Only ]
	return pParser;
  }
  catch(std::exception& ex)
  {
    std::cout << "\n\n  " << ex.what() << "\n\n";
    return 0;
  }
}

//////////////////////////////////////////////////////////
// Function to define rules to manage scope
void ConfigParseToConsole::DefaultRules(Parser* parser)
{
	pBeginningOfScope = new BeginningOfScope();
	pHandlePush = new HandlePush(pRepo);
	pBeginningOfScope->addAction(pHandlePush);
	pParser->addRule(pBeginningOfScope);
	pEndOfScope = new EndOfScope();
	pHandlePop = new HandlePop(pRepo);
	pEndOfScope->addAction(pHandlePop);
	parser->addRule(pEndOfScope);
}

//////////////////////////////////////////////////////////
// Function to define rules to manage Functions
void ConfigParseToConsole::FunctionRules(Parser* parser)
{
	pFunctionDefinition = new FunctionDefinition;
	pPushFunction = new PushFunction(pRepo);  // no action
	pFunctionDefinition->addAction(pPushFunction);
	if (VERBOSE)
	{
		pPrintFunction = new PrintFunction(pRepo);
		pFunctionDefinition->addAction(pPrintFunction);
	}
	parser->addRule(pFunctionDefinition);
}

//////////////////////////////////////////////////////////
// Function to define rules to manage Structures
void ConfigParseToConsole::StructureRules(Parser* parser)
{
	pStructureDefinition = new StructureDefinition;
	pPushStructure = new PushStructure(pRepo);
	pStructureDefinition->addAction(pPushStructure);
	if (VERBOSE)
	{
		pPrintStructure = new PrintStructure(pRepo);
		pStructureDefinition->addAction(pPrintStructure);
	}
	parser->addRule(pStructureDefinition);
}

//////////////////////////////////////////////////////////
// Function to define rules to manage Enumerators(enum)
void ConfigParseToConsole::EnumRules(Parser* parser)
{
	pEnumDefinition = new EnumDefinition;
	pPushEnum = new PushEnum(pRepo);
	pEnumDefinition->addAction(pPushEnum);
	if (VERBOSE)
	{
		pPrintEnum = new PrintEnum(pRepo);
		pEnumDefinition->addAction(pPrintEnum);
	}
	parser->addRule(pEnumDefinition);
}

///////////////////////////////////////////////////////////////////
// Function to define rules to manage Executables and Declarations
void ConfigParseToConsole::ExecDeclarRules(Parser* parser)
{
	pDeclaration = new Declaration;
	pShowDeclaration = new ShowDeclaration;
	pDeclaration->addAction(pShowDeclaration);
	pParser->addRule(pDeclaration);
	pExecutable = new Executable;
	pShowExecutable = new ShowExecutable;
	pExecutable->addAction(pShowExecutable);
	parser->addRule(pExecutable);
}

///////////////////////////////////////////////////////////////////
// Function to define rules to manage Classes
void ConfigParseToConsole::ClassRules(Parser* parser)
{
	pClassDefinition = new ClassDefinition;
	pPushClass = new PushClass(pRepo);
	pClassDefinition->addAction(pPushClass);
	if (VERBOSE)
	{
		pPrintClass = new PrintClass(pRepo);
		pClassDefinition->addAction(pPrintClass);
	}
	parser->addRule(pClassDefinition);
}

///////////////////////////////////////////////////////////////////
// Function to define rules to manage Namespace
void ConfigParseToConsole::NamespaceRules(Parser* parser)
{
	pNamespaceDefinition = new NamespaceDefinition;
	pPushNamespace = new PushNamespace(pRepo);
	pNamespaceDefinition->addAction(pPushNamespace);
	if (VERBOSE)
	{
		pPrintNamespace = new PrintNamespace(pRepo);
		pNamespaceDefinition->addAction(pPrintNamespace);
	}
	parser->addRule(pNamespaceDefinition);
}

///////////////////////////////////////////////////////////////////
// Function to define rules to manage Conditional Scope
void ConfigParseToConsole::ConditionalRules(Parser* parser)
{
	pConditionalDefinition = new ConditionalDefinition;
	pPushConditional = new PushConditional(pRepo);
	pConditionalDefinition->addAction(pPushConditional);
	if (VERBOSE)
	{
		pPrintConditional = new PrintConditional(pRepo);
		pConditionalDefinition->addAction(pPrintConditional);
	}
	parser->addRule(pConditionalDefinition);
}

///////////////////////////////////////////////////////////////////
// Function to define rules to manage Try Block Scope
void ConfigParseToConsole::TryBlockRules(Parser* parser)
{
	pTryDefinition = new TryDefinition;
	pPushTry = new PushTry(pRepo);
	pTryDefinition->addAction(pPushTry);
	if (VERBOSE)
	{
		pPrintTry = new PrintTry(pRepo);
		pTryDefinition->addAction(pPrintTry);
	}
	parser->addRule(pTryDefinition);
}

/////////////////////////////////////////////////////////////////////
// Function to define rules to detect and act on Typedef declarations
void ConfigParseToConsole::TypeDefDetectRules(Parser* parser) {
	pTypeDefDefinition = new TypeDefDefinition;
	pAddEntryTypeDef = new AddEntryTypeDef(pRepo);
	pTypeDefDefinition->addAction(pAddEntryTypeDef);
	if (VERBOSE)
	{
		pPrintDeclaration = new PrintDeclaration(pRepo);
		pTypeDefDefinition->addAction(pPrintDeclaration);
	}
	parser->addRule(pTypeDefDefinition);
}

///////////////////////////////////////////////////////////////////
// Function to define rules to detect and act on Alias declarations
void ConfigParseToConsole::AliasDetectRules(Parser* parser) {
	pAliasDefinition = new AliasDefinition;
	pAddEntryAlias = new AddEntryAlias(pRepo);
	pAliasDefinition->addAction(pAddEntryAlias);
	if (VERBOSE)
	{
		pPrintDeclaration = new PrintDeclaration(pRepo);
		pAliasDefinition->addAction(pPrintDeclaration);
	}
	parser->addRule(pAliasDefinition);
}

#ifdef TEST_CONFIGUREPARSER
////////////////////////////////////////////////
// TEST STUB
#include <queue>
#include <string>

int main(int argc, char* argv[])
{
  std::cout << "\n  Testing ConfigureParser module\n "
            << std::string(32,'=') << std::endl;

  // collecting tokens from files, named on the command line

  if(argc < 2)
  {
    std::cout 
      << "\n  please enter name of file to process on command line\n\n";
    return 1;
  }

  for(int i=1; i<argc; ++i)
  {
    std::cout << "\n  Processing file " << argv[i];
    std::cout << "\n  " << std::string(16 + strlen(argv[i]),'-');

    ConfigParseToConsole configure;
    Parser* pParser = configure.Build();
    try
    {
      if(pParser)
      {
        if(!configure.Attach(argv[i]))
        {
          std::cout << "\n  could not open file " << argv[i] << std::endl;
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
      std::cout << "\n\n";
    }
    catch(std::exception& ex)
    {
      std::cout << "\n\n    " << ex.what() << "\n\n";
    }
    std::cout << "\n\n";
  }
}

#endif
