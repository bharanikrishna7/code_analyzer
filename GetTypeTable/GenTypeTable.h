#ifndef GENTT_H
#define GENTT_H
//////////////////////////////////////////////////////////////////////////////
// GenTypeTable.h - Generates Final Type Table [MAP]						//
// Version :	  1.0														//
// Language:      Visual C++, Visual Studio 2015							//
// Platform:      MSI GE62 2QD, Core-i7, Windows 10							//
// Application:   Parallel Dependency Analysis								//
// Author:		  Venkata Chekuri, SUID : 356579351							//
//                vbchekur@syr.edu											//
//////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ------------------
* This module provides methods to generate final typetable associated with a bunch of files.
*
* Public Interface:
* -----------------
* Methods :-
* ----------
* void makeDepTable();												// Will create Final Type Table using Partial Type Tables
* void showFinTable();												// Will display final type table on console
* std::vector<std::string> returnFiles();							// Will return reference to files
* void setFiles(std::vector<std::string> _files);					// Will set the files variable
* void getAnalysis(std::vector<std::string> _files);				// Performs metrics analysis to obtain the Partial Type Table
* void storepTypeTables(std::vector<Entry> pTypeTable);				// Will push back entry to pTypeTables.
* std::unordered_map <std::string, Entry>* returnFinTable();		// Will return Final Type Table
* void setVerbose(bool paramV);
*
* Required files:
* ---------------
* Metrics.h, Metrics.cpp, ConfigureParser.h,ConfigureParser.cpp, Parser.h, Parser.cpp, ActionsAndRules.h,
* ActionsAndRules.cpp, SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp, Utilities.h, Utilities.cpp
* TypeTable.h, TypeTable.cpp, GenTypeTable.h, GenTypeTable.cpp
*
* Build command:
* --------------
* devenv TypeAnalysis.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 30 March 16
* - first release
*
*/
#include "../FileSystem-Windows/FileSystemDemo/FileSystem.h"
#include "../SemiExp/itokcollection.h"
#include "../Parser/ActionsAndRules.h"
#include "../Parser/ConfigureParser.h"
#include "../ScopeStack/ScopeStack.h"
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Tokenizer.h"
#include "../DataStore/DataStore.h"
#include "../Metrics/Metrics.h"
#include "../FileMgr/FileMgr.h"
#include "../SemiExp/SemiExp.h"
#include "../Parser/Parser.h"
#include "../AST/AST.h"
#include "../TypeTable/TypeTable.h"
#include <unordered_map>
#include <vector>

class GenTypeTable {
public:
	void makeDepTable();
	void showFinTable();
	std::vector<std::string> returnFiles();
	void setFiles(std::vector<std::string> _files);
	void getAnalysis(std::vector<std::string> _files);
	void storepTypeTables(std::vector<Entry> pTypeTable);
	std::unordered_map <std::string, Entry>* returnFinTable();
	void setVerbose(bool paramV);
	bool VERBOSE = false;
private:
	std::vector<std::string> files;
	std::vector<std::vector<Entry>> pTypeTables;
	std::unordered_map <std::string, Entry> fTypeTable;
};
#endif // !GENTT_H