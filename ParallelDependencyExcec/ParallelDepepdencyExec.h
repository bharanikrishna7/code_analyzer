#ifndef PARADEPEXEC_H
#define PARADEPEXEC_H
//////////////////////////////////////////////////////////////////////////////
// ParallelDependencyExec.h - displays that all project requirements have	//
//							  been met										//
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
* Executive class which displays that all project requirements have been met.
*
* VERBOSE MODE:
* -------------
* - Set the VERBOSE variable to enable VERBOSE mode
* - Features:
*		> Same as ParallelDependencyAnalysis project.
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
*																				    execute all the necessary functions to get Dependency Analysis
*																				   for all files
* void appendResults(std::string _fileNMCur, std::vector<resultType>* results);	// Will append results of analysis for each file to finalResults
* void setVerbose(bool paramV);													// Will set VERBOSE parameter value.
*
* Required files:
* ---------------
* FileSystem.h, FileSystem.cpp, GenTypeTable.h, GenTypeTable.cpp, ActionsAndRules.h, ActionsAndRules.cpp,
* ConfigureParser.h, ConfigureParser.cpp, Utilities.h, Utilities.cpp, Tokenizer.h, Tokenizer.cpp,
* Metrics.h, Metrics.cpp, Parser.h, Parser.cpp, AST.h, AST.cpp, itokcollection.h, SemiExp.h, SemiExp.cpp,
* TypeTable.h, TypeTable.cpp, DependencyAnalysis.h, DependencyAnalysis.cpp, ParallelDependencyAnalysis.h,
* ParallelDependencyAnalysis.cpp, ParallelDependencyExec.h, ParallelDependencyExec.cpp, Threadpool.h, 
* Threadpool.cpp, Task.h, Task.cpp
*
* Build command:
* --------------
* devenv ParallelDependencyExec.sln /rebuild debug
*
* Maintenance History:
* --------------------
* ver 1.0 : 6 April 16
* - first release
*
*/
#include "../ParallelDependencyAnalysis/ParallelDependencyAnalysis.h"
#include "../FileSystem-Windows/FileSystemDemo/FileSystem.h"
#include "../AnalyzeFile/DependencyAnalysis.h"
#include "../SemiExp/itokcollection.h"
#include "../Parser/ActionsAndRules.h"
#include "../Parser/ConfigureParser.h"
#include "../Utilities/Utilities.h"
#include "../Tokenizer/Tokenizer.h"
#include "../TypeTable/TypeTable.h"
#include "../Metrics/Metrics.h"
#include "../Parser/Parser.h"
#include "../SemiExp/SemiExp.h"
#include "../AST/AST.h"
#include <vector>
#include <unordered_map>

class ParallelDependencExec {
public:
	void performAnalysis(std::string path, std::vector<std::string> pattern);
	void setVerbose(bool paramV);
	bool VERBOSE = false;
};
#endif