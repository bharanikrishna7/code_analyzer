#ifndef PARADEPANAL_H
#define PARADEPANAL_H
//////////////////////////////////////////////////////////////////////////////
// ParallelDependencyAnalysis.h - Create Type Table and Analyze all files	//
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
* This module will analyze files in a given path by generating a Final Type Table and then
* performing dependency analysis for each of these files.
*
* VERBOSE MODE:
* -------------
* - Set the VERBOSE variable to enable VERBOSE mode
* - Features:
*		> Will print the partial type tables to console
*		> Will print the AST to console
*		> Will print functions to console
*		> Display the progress ( ex: Pass begin, Pass end, etc)
*
* Public Interface:
* -----------------
* Methods :-
* ----------
* void pass1();																	// Will perform Pass 1 : Generate Final Type Table
* void pass2();																	// Will perform Pass 2 : Analyze files for dependency
* void showFinResults();														// Will display the results of analysis to console
* std::vector<std::string> returnFiles();										// Will return the reference to class member files
* void setFiles(std::string path, std::vector<std::string> pattern);			// Will set the files variable based on path and patterns
* void PerformAnalysis(std::string path, std::vector<std::string> pattern);		// This class is a wrapper for all classes. It'll sequentially
*																				// execute all the necessary functions to get Dependency Analysis
*																				// for all files
* void appendResults(std::string _fileNMCur, std::vector<resultType>* results);	// Will append results of analysis for each file to finalResults
* void setVerbose(bool paramV);													// Will set VERBOSE parameter value.
*
* Required files:
* ---------------
* FileSystem.h, FileSystem.cpp, GenTypeTable.h, GenTypeTable.cpp, ActionsAndRules.h, ActionsAndRules.cpp,
* ConfigureParser.h, ConfigureParser.cpp, Utilities.h, Utilities.cpp, Tokenizer.h, Tokenizer.cpp,
* Metrics.h, Metrics.cpp, Parser.h, Parser.cpp, AST.h, AST.cpp, itokcollection.h, SemiExp.h, SemiExp.cpp,
* TypeTable.h, TypeTable.cpp, DependencyAnalysis.h, DependencyAnalysis.cpp, ParallelDependencyAnalysis.h,
* ParallelDependencyAnalysis.cpp, Threadpool.h, Threadpool.cpp, Task.h, Task.cpp
*
* Build command:
* --------------
* devenv ParallelDependencyAnalysis.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 6 April 16
* - first release
*
*/

#include "../Temp/FileMgr.h"
#include "../Temp/DataStore.h"
#include "../Temp/FileSystem.h"

#include "../AST/AST.h"
#include "../Parser/Parser.h"
#include "../Parser/TypeTable.h"
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Tokenizer.h"
#include "../ThreadPool/ThreadPool.h"
#include "../SemiExpression/SemiExp.h"
#include "../Parser/ActionsAndRules.h"
#include "../Parser/ConfigureParser.h"
#include "../ThreadPool/ConcurrentQueue.h"
#include "../SemiExpression/ITokCollection.h"
#include "../AnalyzeSingleFile/AnalyzeSingleFile.h"

#include <vector>
#include <map>

struct ResultType {
	std::string _FileName;
	std::vector<std::string> _tokens;
};

class CodeAnalyzer {
public:
	void setFiles(std::string path, std::vector<std::string> pattern);
	void setVerbose(bool paramV);
	void GenerateFinalTypeTable();
	void AppendTypeTable(std::vector<Entry>* pTT);
	void ShowFinalTable();
	void DependencyAnalysis();
	void AnalyzeFileDepedency(std::string file);
	void ProcessTok(std::string filename, std::string token);
	void AppendResult(std::string& FileName, std::string& DepFileName, std::string& token);
	void ShowFinalReport();
	void Process(const std::string &path, std::vector<std::string> &pattern, bool paramV);
	bool VERBOSE = false;
private:
	std::vector<std::string> _files;
	std::vector<std::vector<Entry>*> _PartialTypeTables;
	std::multimap<std::string, Entry> _FinalTypeTable;
	std::vector<std::pair<std::string, std::vector<ResultType>>> _FinalResults;
};
#endif