#ifndef ANALYZESINGLEFILE_H
#define ANALYZESINGLEFILE_H
//////////////////////////////////////////////////////////////////////////////
// AnalyzeSingleFile.h - analyzes a files and provides metrics				//
// Version :	  1.0                                                       //
// Language:      Visual C++, Visual Studio 2015							//
// Platform:      MSI GE62 2QD, Core-i7, Windows 10							//
// Application:   Parallel Dependency Analysis								//
// Author:		  Venkata Chekuri, SUID : 356579351							//
//                vbchekur@syr.edu											//
//////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* This module will provide functions to analyze a file by generating an AST and providing
* details of each logical scope present in the file. Also it'll help to set some fields in
* the partial type table (present in repository).
*
* Public Interface:
* =================
* void printFunctions(Repository* repo);								// Will print the list of functions present in the file
* void analyze(std::string fileName);									// Will analyze the file using parser and print AST and functions
* void printTree(Repository* repo);										// Will Print the AST
* bool fileExists(const std::string& filename);							// Will validate the existance of file
* void setTable(Repository* repo);										// Will copy the partial type table's reference from repository repo
* void printTable(Repository* repo);									// Will print partial type table present in the repository repo
* void setTypeTableFields(Repository* repo, std::string filename);		// Will set the namespace and the filename fields for all elements
*																		   in the repository repo.
*
* Required files:
* ===============
* Metrics.h, Metrics.cpp, ConfigureParser.h,ConfigureParser.cpp, Parser.h, Parser.cpp, ActionsAndRules.h,
* ActionsAndRules.cpp, SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp, Utilities.h, Utilities.cpp
* TypeTable.h, TypeTable.cpp
*
* Build command:
* ==============
* devenv Metrics.sln /rebuild debug
*
* Maintenance History:
* ====================
* ver 1.0 : 17 March 16
* - first release
*
* ver 2.0 : 2 April 16
* - Added methods to do type table operations.
*
* vec 2.1 : 11 June 16
* - Renamed some methods.
* - Renamed class from Metrics to AnalyzeSingleFile
* - Replaced the PrettyHelper with StringHelper (Print to Console)
*
* Credits:
* --------
* I would like to give credits to Prof. Jim W Fawcett Syracuse University
* , CST 4-187) for providing a started code and explaining how Parallel
* Dependency Analysis project should work.
*
*/

#include <iomanip>
#include <sys/stat.h>

#include "../AST/AST.h"
#include "../Parser/Parser.h"
#include"../Temp/FileSystem.h"
#include "../Parser/TypeTable.h"
#include "../Parser/ScopeStack.h"
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Tokenizer.h"
#include "../Parser/ActionsAndRules.h"
#include "../Parser/ConfigureParser.h"
#include "../SemiExpression/SemiExp.h"
#include "../SemiExpression/itokcollection.h"

class AnalyzeSingleFile {
public:
	AnalyzeSingleFile() {};
	void setVerbose(bool paramV);
	void ShowResult(bool paramV);
	std::vector<Entry>* getTable();
	void setTable(Repository* repo);
	void printTree(Repository* repo);
	void printTable(Repository* repo);
	void analyze(std::string fileName);
	void printFunctions(Repository* repo);
	bool fileExists(const std::string& filename);
	void setTypeTableFields(Repository* repo, std::string filename);
private:
	bool VERBOSE = false;
	bool _ShowResult = false;
	std::vector<Entry> partialTT;
};
#endif