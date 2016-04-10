#ifndef DEPANAL_H
#define DEPANAL_H
//////////////////////////////////////////////////////////////////////////////
// DependencyyAnalysis.h -	Analyze files to check dependency				//
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
* This module provides methods to perform dependency analysis for a file. It is
* desired to use this with TypeAnalysis package (since the latter package will 
* be generating the Final Type Table) but it can be used as standalone too.
*
* Also provides a resultType structure which will be used for holding the results.
* 
* Public Interface:
* -----------------
* Structure :-
* ------------
* resultType																			// Structure which helps to hold the results.
*
* Methods :-
* ----------
* void ShowResult();																	// Will print the results to the screen.
* size_t findVal(std::string _key);														// Will check if the results vector has an element with specific key.
* void processTok(std::string& tok);													// Will process token obtained from the Toker.
* void processFile(std::string fileNM);													// Will begin analysis of the file provided to it as a parameter.
* std::vector<resultType>* returnResults();												// Will return the results vector.
* void AppendResult(std::string _string1, std::string _string2);						// Will append the parameters (filename and token) to the results vector.
* void copyMAP(std::unordered_map <std::string, Entry>* fTypeTable);					// Will copy by reference an unordered map to the class variable TypeTable.
* 
* void addToTypeTable(std::string _name, std::string _type, std::string _filename, std::string _namespace); **					
*																						// Will allow adding values to TypeTable. This is an optional method and
*																						// will be available only with the test stub. (Since we would not like to have
*																						// a function which allows data TypeTable manipualtion to end user)
* 
* Required files:
* ---------------
* FileSystem.h, FileSystem.cpp, GenTypeTable.h, GenTypeTable.cpp, ActionsAndRules.h, ActionsAndRules.cpp,
* ConfigureParser.h, ConfigureParser.cpp, Utilities.h, Utilities.cpp, Tokenizer.h, Tokenizer.cpp,
* Metrics.h, Metrics.cpp, Parser.h, Parser.cpp, AST.h, AST.cpp, itokcollection.h, SemiExp.h, SemiExp.cpp,
* DependencyAnalysis.h, DependencyAnalysis.cpp
*
* Build command:
* --------------
* devenv DependencyAnalysis.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 5 April 16
* - first release
*
*/

#include "../FileSystem-Windows/FileSystemDemo/FileSystem.h"
#include "../GetTypeTable/GenTypeTable.h"
#include "../Parser/ActionsAndRules.h"
#include "../Parser/ConfigureParser.h"
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Tokenizer.h"
#include "../TypeTable/TypeTable.h"
#include "../Metrics/Metrics.h"
#include "../Parser/Parser.h"
#include "../AST/AST.h"
#include "../SemiExp/itokcollection.h"
#include "../SemiExp/SemiExp.h"
#include <vector>
#include <unordered_map>

struct resultType {
	std::string _filename;
	std::vector<std::string> _tokens;
};

class DependencyAnalysis {
public:
	void ShowResult();
	size_t findVal(std::string _key);
	void processTok(std::string& tok);
	void processFile(std::string fileNM);
	std::vector<resultType>* returnResults();
	void AppendResult(std::string _string1, std::string _string2);
	void copyMAP(std::unordered_map <std::string, Entry>* fTypeTable);
	void addToTypeTable(std::string _name, std::string _type, std::string _filename, std::string _namespace);
	bool VERBOSE = false;
private:
	std::unordered_map <std::string, Entry> TypeTable;
	std::vector<resultType> results;
};

#endif // !DEPANAL_H